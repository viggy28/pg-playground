
// Keep postgres.h as the first header file to avoid compilation errors
// clang-format off
#include "postgres.h"
#include "fmgr.h"
// clang-format on
#include "utils/builtins.h"

PG_MODULE_MAGIC;

PG_FUNCTION_INFO_V1(hello);

// Anything to do when the library is loaded
void _PG_init(void) { elog(INFO, "hello from _PG_init"); }

Datum hello(PG_FUNCTION_ARGS) {
  PG_RETURN_TEXT_P(cstring_to_text("Hello, world from shared library!!"));
}