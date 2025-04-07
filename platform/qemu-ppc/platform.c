#include <app.h>
#include <arch/cpu_regs.h>
#include <lib/cbuf.h>
#include <lib/io.h>
#include <lk/console_cmd.h>
#include <lk/reg.h>
#include <platform/debug.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arch/hypercalls.h>

//#define UART_DR 0x3f8
#define UART_DR (0xe0000000ULL + 0x4500ULL + 0)

#if WITH_KERNEL_VM
#include <kernel/vm.h>

struct mmu_initial_mapping mmu_initial_mappings[] = {
  {
    .phys = 16<<20,
    .virt = 16<<20,
    .size = 2<<20,
    .flags = 0,
    .name = "memory",
  },
  { 0 }
};

static pmm_arena_t arena = {
    .name = "sdram",
    .base = 16 << 20,
    .size = 16 << 20,
    .flags = PMM_ARENA_FLAG_KMAP,
};
#endif

static int cmd_p(int argc, const console_cmd_args *argv);
static int cmd_x(int argc, const console_cmd_args *argv);

STATIC_COMMAND_START
STATIC_COMMAND("p", "", &cmd_p)
STATIC_COMMAND("x", "", &cmd_x)
STATIC_COMMAND_END(platform);

void platform_early_init(void) {
  pmm_add_arena(&arena);
}

static int cmd_p(int argc, const console_cmd_args *argv) {
  puts("hello");
#define printreg(name) printf(#name ": 0x%016llx\n", name ## _read())
  printreg(xer);
  printreg(lr);
  printreg(ctr);
  printreg(dsisr);
  printreg(dar);
  printreg(sdr1);
  printreg(uctrl);
  //printreg(ctrl);
  printreg(pvr);
  printreg(tbl);
  printreg(tbu);
  printreg(pir);
  return 0;
}

typedef struct {
  uint64_t pte0, pte1;
} pte_t;

typedef struct {
  pte_t e[8];
} pteg_t;

static void map_page(uint64_t vpn, uint64_t physical) {
  uint64_t avpn = vpn >> 11;
  uint64_t pte0 = (avpn << 7) | 1;
  uint64_t pte1 = physical | 0x10 | 2;
  uint64_t hash = vpn; // TODO, masking
#if 0
  pteg_t *p = page_table;
  p[hash].e[0].pte0 = pte0;
  p[hash].e[0].pte1 = pte1;
#else
  puts("hentering");
  uint64_t ret = h_enter(0, (0&7) | (hash << 3), pte0, pte1);
  printf("ret 0x%llx\n", ret);
#endif
  printf("PTE[0x%llx] = 0x%llx 0x%llx\n", hash, pte0, pte1);
}

#if 0
void mmu_setup(void) {
  void *page_table = memalign(256<<10, 256<<10);
  memset(page_table, 0, 256<<10);
  printf("%p\n", page_table);
  sdr1_write(page_table);
}
#else
void mmu_setup(void) {
}
#endif

extern uint8_t _start, _end;

static int cmd_x(int argc, const console_cmd_args *argv) {
  mmu_setup();

  uint64_t virt_start = (uint64_t)&_start;
  uint64_t virt_end = ROUNDUP((uint64_t)&_end, 4096);
  uint64_t phys_start = 16 << 20;

  virt_end += 1<<20; // since activating late, the heap has grown

  printf("mapping 0x%llx to 0x%llx->0x%llx\n", phys_start, virt_start, virt_end);
  printf("vpn range 0x%llx -> 0x%llx\n", virt_start>>12, virt_end >> 12);

  for (uint64_t vpn = virt_start >> 12; vpn < (virt_end>>12); vpn++) {
    map_page(vpn, vpn << 12);
  }

  slbmte(0, 1, 1, 0, 0, 0, 0, 1, 0);

  msr_write(1ULL<<63 | 1ULL<<4 | 1ULL<<5);

  //while (true) {
  //}

  //sdr1_write(0);
  //free(page_table);
  return 0;
}

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
// also, bit 45/18, MSR_POW, can trigger sleep
// SPR_HID0 can contain a deepnap/doze/nap flag
// see check_pow_970


APP_START(platform_rx)
  .entry = hyper_serial_rx_loop,
APP_END
