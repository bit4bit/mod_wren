#include <assert.h>

#include <freeswitch/switch.h>
#include <wren.h>

// https://github.com/wren-lang/wren-cli/blob/main/src/cli/vm.c

SWITCH_MODULE_LOAD_FUNCTION(mod_wren_load);
SWITCH_MODULE_SHUTDOWN_FUNCTION(mod_wren_shutdown);
SWITCH_MODULE_DEFINITION(mod_wren, mod_wren_load, mod_wren_shutdown, NULL);

switch_status_t wrenRunInterpret(WrenVM *vm, const char *module, const char *source)
{
  WrenInterpretResult result;

  result = wrenInterpret(vm, module, source);
  
  switch(result) {
  case WREN_RESULT_COMPILE_ERROR:
    switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_ERROR, "wren compile error\n");
    return SWITCH_STATUS_FALSE;
  case WREN_RESULT_RUNTIME_ERROR:
    switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_ERROR, "wren runtime error\n");
    return SWITCH_STATUS_FALSE;
  case WREN_RESULT_SUCCESS:
    return SWITCH_STATUS_SUCCESS;
    break;
  default:
    return SWITCH_STATUS_FALSE;
  }
}

static void wrenWriteFn(WrenVM *vm, const char* text)
{
  switch_stream_handle_t *stream = wrenGetUserData(vm);
  assert(stream != NULL);

  stream->write_function(stream, text);
}

static void wrenErrorFn(WrenVM *vm, WrenErrorType errorType,
                        const char *module, const int line,
                        const char *msg)
{
  switch(errorType) {
  case WREN_ERROR_COMPILE:
    switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_ERROR, "[%s line %d] %s\n", module, line, msg);
    break;
  case WREN_ERROR_STACK_TRACE:
    switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_ERROR, "[%s line %d] %s\n", module, line, msg);
    break;
  case WREN_ERROR_RUNTIME:
    switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_ERROR, "[Runtime] %s\n", msg);
    break;
  }
}


void wrenStreamAllocate(WrenVM *vm)
{
  switch_stream_handle_t *stream = wrenGetUserData(vm);

  switch_stream_handle_t **stream_attr = wrenSetSlotNewForeign(vm, 0, 0, sizeof(switch_stream_handle_t *));
  
  *stream_attr = stream;
}

void wrenStreamFileWrite(WrenVM *vm)
{
  switch_stream_handle_t **stream = wrenGetSlotForeign(vm, 0);
  if (*stream == NULL) {
    wrenSetSlotString(vm, 0, "Cannot write to stream.");
    wrenAbortFiber(vm, 0);
    return;
  }

  const char* text = wrenGetSlotString(vm, 1);
  (*stream)->write_function(*stream, text);
}

void wrenStreamStaticFileWrite(WrenVM *vm)
{
  switch_stream_handle_t *stream = wrenGetUserData(vm);
  if (stream == NULL) {
    wrenSetSlotString(vm, 0, "Cannot write to stream.");
    wrenAbortFiber(vm, 0);
    return;
  }

  const char* text = wrenGetSlotString(vm, 1);
  stream->write_function(stream, text);
}

WrenForeignClassMethods wrenBindForeignClass(
                                             WrenVM *vm, const char *module, const char* className)
{
  WrenForeignClassMethods methods;

  if(strcmp(className, "Stream") == 0) {
    methods.allocate = wrenStreamAllocate;
    methods.finalize = NULL;
  } else {
    methods.allocate = NULL;
    methods.finalize = NULL;
  }

  return methods;
}

WrenForeignMethodFn wrenBindForeignMethod(
                                          WrenVM *vm, const char *module, const char *className, bool isStatic, const char* signature)
{
  if (strcmp(className, "Stream") == 0) {
    if (isStatic && strcmp(signature, "write(_)") == 0) {
      return wrenStreamStaticFileWrite;
    }

    if (!isStatic && strcmp(signature, "write(_)") == 0) {
      return wrenStreamFileWrite;
    }
  }

  return NULL;
}

SWITCH_STANDARD_API(wrenrun_api)
{
  WrenConfiguration config;
  WrenVM *vm;
  switch_status_t result;

  wrenInitConfiguration(&config);
  config.bindForeignClassFn = wrenBindForeignClass;
  config.bindForeignMethodFn = wrenBindForeignMethod;
  config.writeFn = &wrenWriteFn;
  config.errorFn = &wrenErrorFn;

  vm = wrenNewVM(&config);
  wrenSetUserData(vm, stream);

  // preload core modules
  if (wrenRunInterpret(vm, "main", "class Stream { foreign static write(text) }\n") != SWITCH_STATUS_SUCCESS) {
    return SWITCH_STATUS_FALSE;
  }

  // run script
  result = wrenRunInterpret(vm, "main", "Stream.write(\"+OK\")");

  wrenFreeVM(vm);

  return result;
}

SWITCH_MODULE_LOAD_FUNCTION(mod_wren_load)
{
  switch_api_interface_t *api_interface;
  
  *module_interface = switch_loadable_module_create_module_interface(pool, modname);

  SWITCH_ADD_API(api_interface, "wrenrun", "run a script", wrenrun_api, "<filename>");
  return SWITCH_STATUS_SUCCESS;
}

SWITCH_MODULE_SHUTDOWN_FUNCTION(mod_wren_shutdown)
{
	return SWITCH_STATUS_UNLOAD;
}
