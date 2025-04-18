#pragma once

#include <stdint.h>
#include <stdbool.h>

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
} \
static inline void name ## _write(uint64_t value) { \
  __asm__ volatile ("mtspr " #id ", %0" : : "r"(value)); \
}

static inline void msr_write(uint64_t value) {
  __asm__ volatile ("tlbie %%r0\nsync\nmtmsrd %0, 0\ntlbie %%r0\nsync\nisync": : "r"(value));
}

static inline void slbmte(uint64_t vsid, bool ks, bool kp, bool n, bool l, bool c, uint64_t esid, bool v, uint16_t index) {
  uint64_t rs = (vsid << 12);
  if (ks) rs |= 1ULL << 11;
  if (kp) rs |= 1ULL << 10;
  if (n) rs |= 1ULL << 9;
  if (l) rs |= 1ULL << 8;
  if (c) rs |= 1ULL << 7;

  uint64_t rb = esid << 28;
  if (v) rb |= 1ULL << 27;
  rb |= index;

  __asm__ volatile ("slbmte %0, %1" : : "r"(rs), "r"(rb));
}

//makereg(mfxer, mtxer, xer);
makereg(mftbl, mttbl, tbl); // full 64bits
makereg(mftbu, mttbu, tbu); // upper 32bits
//makereg(mfpir, mtpir, pir);

make_spr(xer, 1);
make_spr(lr, 8);
make_spr(ctr, 9); // a syscall stores the NIP to LR, and the MSR to CTR, rfscv restores them
make_spr(dsisr, 18);
// data storage interupt status register, why a load/store causd a fault
make_spr(dar, 19);
// data address register, the addr that caused a fault
make_spr(sdr1, 25); // 0x19, the physical addr that the root page table starts at
// 0:4, htable size, must be 0-28
// addr must be 256kb aligned
// at minimum:
//   11bits from the hash are used as an index
//   the min pagetable size is 256kb (2^11 PTEG's of 128 bytes each)
// see section 4.5 in book3
// a PTEG (page table entry group?) is 8 PTE's totalling 128 bytes, 2x64bit each
make_spr(uctrl, 136); // 0x88
make_spr(ctrl, 152); // 0x98
make_spr(pvr, 287);

make_spr(hrmor, 313); // HRMOR

make_spr(lpcr, 318); // 0x13e

make_spr(hid0, 1008); //0x3f0, 1<<22=nap, 1<<23=doze, 1<<24=deepnap
make_spr(pir, 1023);
