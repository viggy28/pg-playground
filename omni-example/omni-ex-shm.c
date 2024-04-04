
// Keep postgres.h as the first header file to avoid compilation errors
// clang-format off
#include "postgres.h"
#include "fmgr.h"
#include "utils/memutils.h"
// clang-format on
#include "utils/builtins.h"

#include "omni_v0.h"

PG_MODULE_MAGIC;
OMNI_MAGIC;

OMNI_MODULE_INFO(.name = "omni-ex-shm", .version = "0.0.1",
                 .identity = "7005f29d-22ef-4c81-aff5-975dac62ad39");

PG_FUNCTION_INFO_V1(dummy);

Datum hello(PG_FUNCTION_ARGS) {
  PG_RETURN_TEXT_P(cstring_to_text("Hello from omni-ex-shm!!"));
}

static void init_strcpy(const omni_handle *handle, void *ptr, void *data,
                        bool allocated) {
  if (allocated) {
    strcpy((char *)ptr, (char *)data);
  }
}

void _Omni_init(const omni_handle *handle) {
  bool found;
  elog(INFO, "omni-ex-shm initialization");
  handle->allocate_shmem(handle, "shmem allocation omni", 128, init_strcpy,
                         "hello", &found);
}
