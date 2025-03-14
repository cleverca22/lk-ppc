#pragma once

#include <lk/compiler.h>
#include <lk/debug.h>

static inline void arch_enable_ints(void) {
  __asm__ volatile("mtmsrd %0, 1": : "r"(0x200 | 0x8000));
}
static inline void arch_disable_ints(void) {
  __asm__ volatile("mtmsrd %0, 1": : "r"(0x200));
}

static inline struct thread *arch_get_current_thread(void) {
  struct thread *thread_reg;
  __asm__ volatile("add %0, %%r11, %1" : "=r"(thread_reg) : "r"(0));
  //printf("arch_get_current_thread() == 0x%llx\n", (uint64_t)thread_reg);
  return thread_reg;
}

static inline void arch_set_current_thread(struct thread *t) {
  //printf("arch_set_current_thread(0x%llx)\n", (uint64_t)t);
  __asm__ volatile ("add %%r11, %0, %1" : : "r"(t), "r"(0));
}

static inline bool arch_ints_disabled(void) {
  uint32_t state;

  __asm__ volatile("mfmsr %0": "=r" (state));
  return !(state & 0x8000);
}

static inline uint arch_curr_cpu_num(void) {
  uint64_t pir;
  __asm__("mfspr %0, 1023" : "=r"(pir));
  return pir;
}
