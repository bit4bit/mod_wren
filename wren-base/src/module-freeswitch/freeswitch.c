#include <switch.h>

#include "wren.h"
#include <vm.h>

static void consoleLog(const char *level_str, const char *msg)
{      
  switch_log_level_t level = SWITCH_LOG_DEBUG;
  if (level_str) {
    level = switch_log_str2level(level_str);
    if (level == SWITCH_LOG_INVALID) {
      level = SWITCH_LOG_DEBUG;
    }
  }
  switch_log_printf(SWITCH_CHANNEL_LOG, level, "%s", switch_str_nil(msg));
}

void freeswitchConsoleLog(WrenVM *vm)
{
  const char *level = wrenGetSlotString(vm, 1);
  const char *msg = wrenGetSlotString(vm, 2);

  consoleLog(level, msg);
}

void freeswitchApiExecute(WrenVM *vm)
{
  freeswitch_t *fs = wrenGetUserData(vm);
  assert(fs != NULL);
  assert(fs->session != NULL);

  switch_stream_handle_t stream = { 0 };
  SWITCH_STANDARD_STREAM(stream);
  
  const char *cmd = wrenGetSlotString(vm, 1);
  const char *arg = wrenGetSlotString(vm, 2);

  switch_api_execute(cmd, arg, fs->session, &stream);
  wrenSetSlotString(vm, 0, (char *) stream.data);
}
