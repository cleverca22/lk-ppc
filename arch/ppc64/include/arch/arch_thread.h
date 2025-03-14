#pragma once

#include <sys/types.h>

struct arch_thread {
  uint64_t lr;  // 0
  uint64_t sp;  // 8
  uint64_t r14; // 16
  uint64_t r15; // 24
  uint64_t r16; // 32
  uint64_t r17; // 40
  uint64_t r18; // 48
  uint64_t r19; // 56
  uint64_t r20; // 64
  uint64_t r21;
  uint64_t r22;
  uint64_t r23;
  uint64_t r24;
  uint64_t r25;
  uint64_t r26;
  uint64_t r27;
  uint64_t r28;
  uint64_t r29;
  uint64_t r30;
  uint64_t r31;
};
