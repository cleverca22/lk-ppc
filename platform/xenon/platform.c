#include <arch/cpu_regs.h>
#include <dev/display.h>
#include <lk/console_cmd.h>
#include <lk/err.h>
#include <lk/reg.h>
#include <platform/debug.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef WITH_LIB_GFX
#include <lib/gfx.h>
#endif

#define UART_BASE ((0x80000200ULL << 32) | 0xEA001000ULL)

static int cmd_p(int argc, const console_cmd_args *argv);
static int cmd_x(int argc, const console_cmd_args *argv);
static int cmd_f(int argc, const console_cmd_args *argv);

STATIC_COMMAND_START
STATIC_COMMAND("p", "", &cmd_p)
STATIC_COMMAND("x", "", &cmd_x)
STATIC_COMMAND("f", "", &cmd_f)
STATIC_COMMAND_END(platform);

void init_uart(void) {
  // Set UART to 115400, 8, N, 1
  *REG32(UART_BASE+0x1C) = 0xE6010000;
}

uint32_t *framebuffer = NULL;

void platform_early_init(void) {
  init_uart();
  printf("fb %p\n", framebuffer);
}

//int cmd_gfx(int argc, const console_cmd_args *argv);
void platform_init(void) {
  //console_cmd_args args[2] = {
  //  [0] = { .str = "gfx" },
  //  [1] = { .str = "rgb_bars" },
  //};
  //cmd_gfx(2, args);
  uint64_t x = pir_read();
  printf("PIR 0x%llx\n", x);
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

#ifdef WITH_LIB_GFX


#define WIDTH 1280
#define HEIGHT 720

uint32_t fix_color(uint32_t in) {
  uint8_t r = (in >> 16) & 0xff;
  uint8_t g = (in >> 8) & 0xff;
  uint8_t b = (in >> 0) & 0xff;
  return (r << 8) | (g << 16) | (b << 24);
}

struct ati_info {
  uint32_t unknown1[4];
  uint32_t base;
  uint32_t unknown2[8];
  uint32_t width;
  uint32_t height;
} __attribute__ ((__packed__));

void fb_init(void) {
  uint32_t size = WIDTH * HEIGHT * 4;
  framebuffer = malloc(size);
  bzero(framebuffer, size);
  printf("allocated fb to %p\n", framebuffer);
  struct ati_info *ai = (struct ati_info*)0xec806100ULL;
  printf("base: 0x%x\n", ai->base);
  printf("resolution: %dx%d\n", ai->width, ai->height);

  volatile uint32_t *fb = (uint32_t*)((1ULL << 63) | 0x1e000000);
  if (false) {
    for (int x=0; x<32; x++) {
      int out = (x & 0x3) | ((x & 0x1c) << 1);
      fb[out] = (x&1) ? 0 : 0xffffff00;
    }
  }
  for (int y=0; y<32; y++) {
    int out = ((y & ~1) * 64) | ((y&1) * 4);
    //fb[out] = (y&1) ? 0 : 0xffffff00;
  }
  fb[0]   = 0xff000000;
  fb[4]   = 0x00ff0000;
  fb[64]  = 0x0000ff00;
  fb[68]  = 0xff000000;
  fb[128] = 0x00ff0000;
  fb[132] = 0x0000ff00;
  fb[192] = 0xff000000;
  fb[196] = 0x00ff0000;
  //fb[201] = 0x0000ff00;
}

int xeFbConvert(int x, int y, int width) {
  return (((y >> 5) * 32 * width +
    ((x >> 5) << 10) +
    (x & 3) + ((y & 1) << 2) +
    (((x & 31) >> 2) << 3) +
    (((y & 31) >> 1) << 6))
    ^ ((y & 8) << 2));
}

void retile_framebuffer(uint starty, uint endy) {
#ifndef WITH_LIB_GFXCONSOLE
  printf("retile_framebuffer(%d, %d)\n", starty, endy);
#endif
  volatile uint32_t *fb = (uint32_t*)((1ULL << 63) | 0x1e000000);
  uint32_t *input = framebuffer;
  int stride = WIDTH;

  for (int y = starty; y < endy; y++) {
    int yoff = y * stride;

    //if (y > 0) continue;

    for (int x = 0; x < WIDTH; x++) {
      //if (x > 64) continue;
      int tilex = x / 32;
      int tilex2 = x % 32;
      uint32_t pixel = fix_color(input[yoff + x]);
      int out = (tilex * 1024) + (tilex2) + (y * 1024 * 2);
      out = xeFbConvert(x, y, WIDTH);
      //printf("%d,%x -> %d == %06x\n", x, y, out, pixel);
      fb[out] = pixel;
    }
  }
}

__WEAK status_t display_get_framebuffer(struct display_framebuffer *fb) {
  if (framebuffer == NULL) fb_init();

  const int w = WIDTH;
  const int h = HEIGHT;
  const gfx_format fmt = GFX_FORMAT_RGB_332;
  fb->image.pixels = framebuffer;
  fb->format = DISPLAY_FORMAT_ARGB_8888;
  fb->image.format = IMAGE_FORMAT_ARGB_8888;
  fb->image.rowbytes = 1280 * 4;
  fb->image.width = w;
  fb->image.height = h;
  fb->image.stride = 1280;
  fb->flush = retile_framebuffer;

  return NO_ERROR;
}
#endif

static int cmd_p(int argc, const console_cmd_args *argv) {
  volatile uint32_t *fb = (uint32_t*)((1ULL << 63) | 0x1e000000);
  int index = argv[1].u;
  //fb[index] = 0x0000ff00;
  puts("hello");
#define printreg(name) printf(#name ": 0x%016llx\n", name ## _read())
  printreg(hrmor);
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

uint64_t *page_table;

void mmu_setup(void) {
  page_table = memalign(256<<10, 256<<10);
  memset(page_table, 0, 256<<10);
  printf("%p\n", page_table);
  sdr1_write((uint64_t)page_table);
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
  uint64_t addr_upper = 1; // TODO
  uint64_t hash = (vpn ^ addr_upper) & 0x7ff;
  pteg_t *p = page_table;
  p[hash].e[0].pte0 = pte0;
  p[hash].e[0].pte1 = pte1;
  printf("mapping virt 0x%x -> phys 0x%x\n", (uint32_t)(vpn << 12), (uint32_t)physical);
  printf("PTE[0x%llx] = 0x%llx 0x%llx\n", hash, pte0, pte1);
}

extern uint8_t _start, _end;

static int cmd_x(int argc, const console_cmd_args *argv) {
  mmu_setup();

  uint64_t virt_start = ROUNDDOWN((uint64_t)&_start, 4096);
  uint64_t virt_end = ROUNDUP((uint64_t)&_end, 4096);
  uint64_t phys_start = virt_start;
  virt_end += 1<<20; // since activating late, the heap has grown, TODO improve
  printf("mapping 0x%llx to 0x%llx->0x%llx\n", phys_start, virt_start, virt_end);
  printf("vpn range 0x%llx -> 0x%llx\n", virt_start>>12, virt_end >> 12);
  for (uint64_t vpn = virt_start >> 12; vpn < (virt_end>>12); vpn++) {
    map_page(vpn, vpn << 12);
  }

  uint64_t lpcr = lpcr_read();
  lpcr &= ~0x400; // clear SW TLB bit
  lpcr_write(lpcr);

  const uint64_t vsid = 256 >> 8; // TODO
  const uint64_t esid = 1; // TODO
  slbmte(vsid, 1, 1, 0, 0, 0, esid, 1, 0);
  msr_write(1ULL<<63 | 1ULL<<60 | 1ULL<<4 | 1ULL<<5);
  return 0;
}

void test1(uint64_t *a, uint64_t *b, uint64_t *c);

static int cmd_f(int argc, const console_cmd_args *argv) {
  uint64_t a[] = { 0x11, 0x22 };
  uint64_t b[3];
  uint64_t c[3];
  test1(a, b, c);
  printf("0x%llx 0x%llx 0x%llx\n", b[0], b[1], b[2]);
  printf("0x%llx 0x%llx 0x%llx\n", c[0], c[1], c[2]);
  return 0;
}
