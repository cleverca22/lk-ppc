#include <lk/reg.h>
#include <stdio.h>
#include <stdbool.h>

#define UART_BASE 0xEA001000ULL

void init_uart(void) {
  // Set UART to 115400, 8, N, 1
  *REG32(UART_BASE+0x1C) = 0xE6010000;
}

void platform_early_init(void) {
  init_uart();
}

void platform_dputc(char c) {
  while (!((*REG32(UART_BASE+0x18)) & (1<<25)));
  *REG32(UART_BASE+0x14) = (c << 24) & 0xFF000000;
}

uint32_t kbhit(void) {
  uint32_t status;
  do {
    status = *REG32(UART_BASE+0x18);
  } while (status & ~0x03000000);

  return !!(status & (1<<24));
}

int platform_dgetc(char *c, bool wait) {
  while (!kbhit());
  *c = (*REG32(UART_BASE+0x10)) >> 24;
  return -0;
}
