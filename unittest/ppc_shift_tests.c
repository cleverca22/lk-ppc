/*
 * Copyright (c) 2013, Google, Inc. All rights reserved
 *
 * Use of this source code is governed by a MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT
 */
/*
 * Functions for unit tests.  See lib/unittest/include/unittest.h for usage.
 */
#include <lib/unittest.h>

#include <lk/debug.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>

#if LK_DEBUGLEVEL > 2
#define TEST_FAILURE_CASES 1
#else
#define TEST_FAILURE_CASES 0
#endif


extern void unittest_printf(const char* format, ...);

// Get contents of the CR register
extern uint32_t getCR(void);

// Get contents of the XER register
extern uint64_t getXER(void);

// Clear CR register
extern void clearCR(void);

// Clear XER register
extern void clearXER(void);

static bool test_shift(void) {
  BEGIN_TEST;

  uint64_t testInts64[] = {
      0x0000000000000000LL, 0x0000000000000001LL, 0x0000000000000002LL, 0xFFFFFFFFFFFFFFFFLL,  //  0  1  2  3
      0xFFFFFFFFFFFFFFFELL, 0x0003333300330033LL, 0x000000FFFFF00000LL, 0x1000000000000000LL,  //  4  5  6  7
      0x1FFFFFFFFFFFFFFFLL, 0x4238572200000000LL, 0x7000000000000000LL, 0x0000000072233411LL,  //  8  9 10 11
      0x7FFFFFFFFFFFFFFFLL, 0x8000000000000000LL, 0x8000000000000001LL, 0x0123456789ABCDEFLL,  // 12 13 14 15
  };

  // Show results
#define PRINT1(name, i, GPR) \
    unittest_printf( name"([%02d]) -> %016llX [%08X]\n", i, *(uint64_t*)&GPR, getCR());
#define PRINT2(name, i, j, GPR) \
    unittest_printf( name"([%02d],[%02d]) -> %016llX [%08X]\n", i, j, *(uint64_t*)&GPR, getCR());
#define PRINT3(name, i, j, k, GPR) \
    unittest_printf( name"([%02d],[%02d],[%02d]) -> %016llX [%08X]\n", i, j, k, *(uint64_t*)&GPR, getCR());
#define PRINT4(name, i, j, k, l, GPR) \
    unittest_printf( name"([%02d],[%02d],[%02d],[%02d]) -> %016llX [%08X]\n", i, j, k, l, *(uint64_t*)&GPR, getCR());

// Iterate
#define ITERATE1(x) \
    for (uint32_t i=0; i<sizeof(testInts64)/sizeof(testInts64[0]); i++) \
        {uint64_t r0, r1=testInts64[i]; clearCR(); x;}

#define ITERATE2(x) \
    for (uint32_t i=0; i<sizeof(testInts64)/sizeof(testInts64[0]); i++) \
        for (uint32_t j=0; j<sizeof(testInts64)/sizeof(testInts64[0]); j++) \
            {uint64_t r0, r1=testInts64[i], r2=testInts64[j]; clearCR(); x;}

// Integer Shift Instructions
  ITERATE2(__asm__("sld     %0,%1,%2" : "=r"(r0) : "r"(r1), "r"(r2));  PRINT2("sld    ", i, j, r0));
  ITERATE2(__asm__("sld.    %0,%1,%2" : "=r"(r0) : "r"(r1), "r"(r2));  PRINT2("sld.   ", i, j, r0));
  ITERATE2(__asm__("slw     %0,%1,%2" : "=r"(r0) : "r"(r1), "r"(r2));  PRINT2("slw    ", i, j, r0));
  ITERATE2(__asm__("slw.    %0,%1,%2" : "=r"(r0) : "r"(r1), "r"(r2));  PRINT2("slw.   ", i, j, r0));
  ITERATE2(__asm__("srd     %0,%1,%2" : "=r"(r0) : "r"(r1), "r"(r2));  PRINT2("srd    ", i, j, r0));
  ITERATE2(__asm__("srd.    %0,%1,%2" : "=r"(r0) : "r"(r1), "r"(r2));  PRINT2("srd.   ", i, j, r0));
  ITERATE2(__asm__("srw     %0,%1,%2" : "=r"(r0) : "r"(r1), "r"(r2));  PRINT2("srw    ", i, j, r0));
  ITERATE2(__asm__("srw.    %0,%1,%2" : "=r"(r0) : "r"(r1), "r"(r2));  PRINT2("srw.   ", i, j, r0));
  ITERATE1(__asm__("sradi   %0,%1,%2" : "=r"(r0) : "r"(r1), "i"(0));   PRINT2("sradi  ", i, 0, r0));
  ITERATE1(__asm__("sradi   %0,%1,%2" : "=r"(r0) : "r"(r1), "i"(1));   PRINT2("sradi  ", i, 1, r0));
  ITERATE1(__asm__("sradi   %0,%1,%2" : "=r"(r0) : "r"(r1), "i"(63));  PRINT2("sradi  ", i, 63, r0));
  ITERATE1(__asm__("sradi.  %0,%1,%2" : "=r"(r0) : "r"(r1), "i"(0));   PRINT2("sradi. ", i, 0, r0));
  ITERATE1(__asm__("sradi.  %0,%1,%2" : "=r"(r0) : "r"(r1), "i"(1));   PRINT2("sradi. ", i, 1, r0));
  ITERATE1(__asm__("sradi.  %0,%1,%2" : "=r"(r0) : "r"(r1), "i"(63));  PRINT2("sradi. ", i, 63, r0));
  ITERATE1(__asm__("srawi   %0,%1,%2" : "=r"(r0) : "r"(r1), "i"(0));   PRINT2("srawi  ", i, 0, r0));
  ITERATE1(__asm__("srawi   %0,%1,%2" : "=r"(r0) : "r"(r1), "i"(1));   PRINT2("srawi  ", i, 1, r0));
  ITERATE1(__asm__("srawi   %0,%1,%2" : "=r"(r0) : "r"(r1), "i"(31));  PRINT2("srawi  ", i, 31, r0));
  ITERATE1(__asm__("srawi.  %0,%1,%2" : "=r"(r0) : "r"(r1), "i"(0));   PRINT2("srawi. ", i, 0, r0));
  ITERATE1(__asm__("srawi.  %0,%1,%2" : "=r"(r0) : "r"(r1), "i"(1));   PRINT2("srawi. ", i, 1, r0));
  ITERATE1(__asm__("srawi.  %0,%1,%2" : "=r"(r0) : "r"(r1), "i"(31));  PRINT2("srawi. ", i, 31, r0));
  ITERATE2(__asm__("srad    %0,%1,%2" : "=r"(r0) : "r"(r1), "r"(r2));  PRINT2("srad   ", i, j, r0));
  ITERATE2(__asm__("srad.   %0,%1,%2" : "=r"(r0) : "r"(r1), "r"(r2));  PRINT2("srad.  ", i, j, r0));
  ITERATE2(__asm__("sraw    %0,%1,%2" : "=r"(r0) : "r"(r1), "r"(r2));  PRINT2("sraw   ", i, j, r0));
  ITERATE2(__asm__("sraw.   %0,%1,%2" : "=r"(r0) : "r"(r1), "r"(r2));  PRINT2("sraw.  ", i, j, r0));


  END_TEST;
}

BEGIN_TEST_CASE(ppc_shift)
RUN_TEST(test_shift);
END_TEST_CASE(ppc_shift)