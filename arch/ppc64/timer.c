#include <platform/timer.h>
#include <arch/cpu_regs.h>
#include <lk/err.h>
#include <stdio.h>

lk_bigtime_t current_time_hires(void) {
  return tbl_read()/50;
}

lk_time_t current_time(void) {
  return tbl_read()/50/1000;
}

status_t platform_set_oneshot_timer (platform_timer_callback callback, void *arg, lk_time_t interval) {
  printf("setting timer for %d ms\n", interval);
  return NO_ERROR;
}

void platform_stop_timer(void) {
}
