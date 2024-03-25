// clang-format off
#include "postgres.h"
#include "fmgr.h"
// clang-format on
#include "xact.h"

PG_MODULE_MAGIC;

// A transaction callback function

void xact_cb(XactEvent event, void *arg) {
  if (event == XACT_EVENT_COMMIT) {
    elog(INFO, "Transaction committed");
  } else if (event == XACT_EVENT_ABORT) {
    elog(INFO, "Transaction aborted");
  }
}

void _PG_init(void) {
  elog(INFO, "hello from _PG_init");
  RegisterXactCallback(xact_cb, NULL);
}