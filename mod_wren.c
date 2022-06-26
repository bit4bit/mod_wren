#include <assert.h>

#include <freeswitch/switch.h>

#include "wren_wrap.h"

// https://github.com/wren-lang/wren-cli/blob/main/src/cli/vm.c

SWITCH_MODULE_LOAD_FUNCTION(mod_wren_load);
SWITCH_MODULE_SHUTDOWN_FUNCTION(mod_wren_shutdown);
SWITCH_MODULE_DEFINITION(mod_wren, mod_wren_load, mod_wren_shutdown, NULL);


SWITCH_STANDARD_API(wrenrun_api)
{
  freeswitch_t fs = {0};
  fs.stream = stream;

  return wren_run_file(cmd, &fs);
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
