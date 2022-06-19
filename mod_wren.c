#include <freeswitch/switch.h>

SWITCH_MODULE_LOAD_FUNCTION(mod_wren_load);
SWITCH_MODULE_SHUTDOWN_FUNCTION(mod_wren_shutdown);
SWITCH_MODULE_DEFINITION(mod_wren, mod_wren_load, mod_wren_shutdown, NULL);

SWITCH_MODULE_LOAD_FUNCTION(mod_wren_load)
{
  *module_interface = switch_loadable_module_create_module_interface(pool, modname);

  return SWITCH_STATUS_SUCCESS;
}

SWITCH_MODULE_SHUTDOWN_FUNCTION(mod_wren_shutdown)
{
	return SWITCH_STATUS_UNLOAD;
}
