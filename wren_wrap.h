#ifndef WREN_WRAP_H
#define WREN_WRAP_H

#include <switch.h>

switch_status_t wren_run_file(const char *filename, switch_stream_handle_t *stream);

#endif /*WREN_WRAP_H*/
