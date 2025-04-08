#include <arch.h>
#include <lk/debug.h>

void __WEAK arch_idle(void) {
    asm volatile("nop");
}

void clear_bss(void) {
  extern uint8_t __bss_start, __bss_end;
  for (uint8_t *t = &__bss_start; t < &__bss_end; t++) {
    *t = 0;
  }
}

void arch_early_init(void) {
}

void arch_init(void) {
}

void arch_chain_load(void *entry, ulong arg0, ulong arg1, ulong arg2, ulong arg3) {
  panic("unimplemented");
}

void arch_clean_cache_range(addr_t start, size_t len) {
  //PANIC_UNIMPLEMENTED;
}
