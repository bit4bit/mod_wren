#ifndef WREN_WRAP_H
#define WREN_WRAP_H

#include <switch.h>

switch_status_t wren_parse_and_interpret(const char *filename, switch_stream_handle_t *stream);

#endif /*WREN_WRAP_H*/
