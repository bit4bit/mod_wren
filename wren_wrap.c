#include "wren_wrap.h"
#include <vm.h>

switch_status_t wren_parse_and_interpret(const char* filename, switch_stream_handle_t *stream)
{
  WrenInterpretResult result;

  result = runFile("test/test.wren", stream);
  
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
