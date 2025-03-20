#include <app.h>
#include <lib/cbuf.h>
#include <lib/io.h>
#include <lk/reg.h>
#include <platform/debug.h>
#include <stdio.h>
#include <string.h>

//#define UART_DR 0x3f8
#define UART_DR (0xe0000000ULL + 0x4500ULL + 0)

// look for spapr_register_hypercall() in qemu
#define H_GET_TERM_CHAR         0x54
#define H_PUT_TERM_CHAR         0x58

void do_hypercall4(uint32_t opcode, uint64_t a, uint64_t b, uint64_t c, uint64_t d);

void platform_dputc(char c) {
  //*REG8(UART_DR) = c;
  do_hypercall4(H_PUT_TERM_CHAR, 0, 1, ((uint64_t)c) << (64-8), 0);
}

int platform_dgetc(char *c, bool wait) {
  while (true) {
    uint64_t len, part0, part1, status;
    asm volatile ("mr %%r3, %4\n mr %%r4, %5 \n sc 1\n mr %0, %%r3\n mr %1, %%r4\n mr %2, %%r5\n mr %3, %%r6" : "=r"(status), "=r"(len), "=r"(part0), "=r"(part1) : "r"(H_GET_TERM_CHAR), "r"(0));
    if (len == 0) continue;
    char buffer[17];
    memcpy(buffer, &part0, 8);
    memcpy(buffer+8, &part1, 8);
    buffer[16] = 0;
    printf("len %lld %llx %llx '%s'\n", len, part0, part1, buffer);
  }
  *c = 0;
  return 0;
}

void hyper_serial_rx_loop(const struct app_descriptor *, void *) {
  while (true) {
    uint64_t len, part0, part1, status;
    asm volatile ("mr %%r3, %4\n mr %%r4, %5 \n sc 1\n mr %0, %%r3\n mr %1, %%r4\n mr %2, %%r5\n mr %3, %%r6" : "=r"(status), "=r"(len), "=r"(part0), "=r"(part1) : "r"(H_GET_TERM_CHAR), "r"(0));
    if (len == 0) {
      thread_yield();
      continue;
    }
    char buffer[17];
    memcpy(buffer, &part0, 8);
    memcpy(buffer+8, &part1, 8);
    buffer[16] = 0;
    //printf("status %lld, len %lld %llx %llx '%s'\n", status, len, part0, part1, buffer);
    while (cbuf_space_avail(&console_input_cbuf) < len) puts("spinning for space");
    cbuf_write(&console_input_cbuf, buffer, len, true);
  }
}

// TODO, H_CEDE for arch_idle?


APP_START(platform_rx)
  .entry = hyper_serial_rx_loop,
APP_END
