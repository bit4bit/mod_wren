#ifndef WREN_WRAP_H
#define WREN_WRAP_H

#include <switch.h>
#include <vm.h>

switch_status_t wren_run_file(const char *filename, freeswitch_t *freeswitch);

#endif /*WREN_WRAP_H*/
