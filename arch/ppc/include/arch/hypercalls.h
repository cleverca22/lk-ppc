#pragma once

// look for spapr_register_hypercall() in qemu
#define H_ENTER                 0x08
#define H_GET_TERM_CHAR         0x54
#define H_PUT_TERM_CHAR         0x58

uint64_t do_hypercall4(uint32_t opcode, uint64_t a, uint64_t b, uint64_t c, uint64_t d);

#define H_EXACT           (1ULL<<(63-24))       /* Use exact PTE or return H_PTEG_FULL */
static inline uint64_t h_enter(uint64_t flags, uint64_t ptex, uint64_t pte0, uint64_t pte1) {
  return do_hypercall4(H_ENTER, flags, ptex, pte0, pte1);
}
