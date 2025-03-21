#include <platform/timer.h>
#include <arch/cpu_regs.h>
#include <lk/err.h>

static inline uint64_t mftb(void) {
  uint32_t l, u;
  l = tbl_read();
  u = tbu_read();
  return (((uint64_t)u) << 32) | l;
}

lk_bigtime_t current_time_hires(void) {
  return mftb()/50;
}

lk_time_t current_time(void) {
  return mftb()/50/1000;
}

status_t platform_set_oneshot_timer (platform_timer_callback callback, void *arg, lk_time_t interval) {
  return NO_ERROR;
}

void platform_stop_timer(void) {
}
