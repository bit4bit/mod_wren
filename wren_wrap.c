#include "wren_wrap.h"
#include <vm.h>

switch_status_t wren_run_file(const char *filename, switch_stream_handle_t *stream)
{
  WrenInterpretResult result;
  char *file = (char*) filename;

  // try run file from freeswitch scripts directory
  if (!switch_is_file_path(file)) {
    char *fdup = NULL;
    fdup = switch_mprintf("%s/%s", SWITCH_GLOBAL_dirs.script_dir, file);
    switch_assert(fdup);
    file = fdup;
  }

  switch_assert(file);
  result = runFile(file, stream);
  
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
