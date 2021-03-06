#ifndef vm_h
#define vm_h

#include <switch.h>

#include "uv.h"
#include "wren.h"

typedef struct {
  switch_core_session_t *session;
  switch_stream_handle_t *stream;
} freeswitch_t;

// Executes the Wren script at [path] in a new VM.
WrenInterpretResult runFile(const char* path, freeswitch_t *stream);

// Runs the Wren interactive REPL.
WrenInterpretResult runRepl();

// Gets the currently running VM.
WrenVM* getVM();

// Gets the event loop the VM is using.
uv_loop_t* getLoop();

// Get the exit code the CLI should exit with when done.
int getExitCode();

// Set the exit code the CLI should exit with when done.
void setExitCode(int exitCode);

// Adds additional callbacks to use when binding foreign members from Wren.
//
// Used by the API test executable to let it wire up its own foreign functions.
// This must be called before calling [createVM()].
void setTestCallbacks(WrenBindForeignMethodFn bindMethod,
                      WrenBindForeignClassFn bindClass,
                      void (*afterLoad)(WrenVM* vm));

#endif
