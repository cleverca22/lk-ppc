#pragma once

#include <stdint.h>

#define makereg(from, to, name) static inline uint64_t name ## _read(void) { \
  uint64_t t; \
  __asm__ volatile (#from " %0" : "=r"(t)); \
  return t; \
} \
static inline void name ## _write(uint64_t value) { \
  __asm__ volatile (#to " %0" : : "r"(value)); \
}

#define make_spr(name, id) static inline uint64_t name ## _read(void) { \
  uint64_t t; \
  __asm__ volatile ("mfspr %0, " #id : "=r"(t)); \
  return t; \
}

makereg(mfxer, mtxer, xer);
makereg(mftbl, mttbl, tbl); // lower 32bits?
makereg(mftbu, mttbu, tbu); // upper 32bits?
makereg(mfspr, mtspr, spr);
//makereg(mfpir, mtpir, pir);

make_spr(pir, 1023);
