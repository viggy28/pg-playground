
// Keep postgres.h as the first header file to avoid compilation errors
// clang-format off
#include "postgres.h"
#include "fmgr.h"
#include "utils/memutils.h"
// clang-format on
#include "utils/builtins.h"

PG_MODULE_MAGIC;

void say_hello(void *arg) { elog(INFO, "hello from a child process !"); }

void _PG_init() {
  // When a new backend gets forked from the postmaster process
  // PostmasterContext gets deleted

  // Allocate a new struct "callback" in the PostmasterContext and initialize
  // func argument with the callback function "say_hello"
  // When PostmasterContext is deleted, it cleans up "callback" struct also
  MemoryContext oldcontext = MemoryContextSwitchTo(PostmasterContext);
  MemoryContextCallback *callback = palloc(sizeof(*callback));
  callback->func = say_hello;
  MemoryContextRegisterResetCallback(PostmasterContext, callback);
  MemoryContextSwitchTo(oldcontext);
}
