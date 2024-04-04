// clang-format off
#include <postgres.h>
#include <fmgr.h>
/* These are always necessary for a bgworker */
#include "miscadmin.h"
#include "postmaster/bgworker.h"
#include "postmaster/interrupt.h"
#include "storage/ipc.h"
#include "storage/latch.h"
#include "storage/lwlock.h"
#include "storage/proc.h"
#include "storage/shmem.h"
#include <signal.h>

// clang-format on

#include <stdio.h>

PG_MODULE_MAGIC;

// PG_FUNCTION_INFO_V1(pg_bgw_workers_launch);

// Datum pg_bgw_workers_launch(PG_FUNCTION_ARGS) { PG_RETURN_INT32(0); }

volatile sig_atomic_t got_SIGTERM = 0;

void handle_sigterm(int sig) { got_SIGTERM = 1; }

PGDLLEXPORT void entry_point(void) pg_attribute_noreturn();

void entry_point(void) {
  elog(INFO, "pg-bgw-workers entry_point");
  signal(SIGTERM, handle_sigterm);

  while (!got_SIGTERM) {
    elog(INFO, "pg-bgw-workers running");
    pg_usleep(1000000L);
  }
  elog(INFO, "pg-bgw-workers shutting down");
}

void _PG_init(void);

void _PG_init() {
  BackgroundWorker worker;
  elog(INFO, "pg-bgw-workers initialization");

  memset(&worker, 0, sizeof(worker));
  strncpy(worker.bgw_name, "pg_bgw_workers", BGW_MAXLEN);
  worker.bgw_flags =
      BGWORKER_SHMEM_ACCESS | BGWORKER_BACKEND_DATABASE_CONNECTION;
  worker.bgw_start_time = BgWorkerStart_RecoveryFinished;
  worker.bgw_restart_time = BGW_NEVER_RESTART;
  sprintf(worker.bgw_library_name, "pg_bgw_workers");
  sprintf(worker.bgw_function_name, "entry_point");
  worker.bgw_notify_pid = 0;
  RegisterBackgroundWorker(&worker);
}