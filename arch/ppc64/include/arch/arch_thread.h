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
  uint64_t r21; // 72
  uint64_t r22; // 80
  uint64_t r23; // 88
  uint64_t r24; // 96
  uint64_t r25; // 104
  uint64_t r26; // 112
  uint64_t r27; // 120
  uint64_t r28; // 128
  uint64_t r29; // 136
  uint64_t r30; // 144
  uint64_t r31; // 152
};
