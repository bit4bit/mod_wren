#include <switch.h>

#include "wren.h"

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
  const char *level = wrenGetSlotString(vm, 0);
  const char *msg = wrenGetSlotString(vm, 1);

  consoleLog(level, msg);
}
