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

static bool test_cmp(void) {
  BEGIN_TEST;

  uint64_t testInts64[] = {
      0x0000000000000000LL, 0x0000000000000001LL, 0xFFFFFFFFFFFFFFFFLL, 0x00000000FFFFFFFFLL,  //  0  1  2  3
      0x7FFFFFFFFFFFFFFFLL, 0x000000007FFFFFFFLL, 0x8000000000000000LL, 0x0000000080000000LL,  //  4  5  6  7
      0xFFFFFFFF00000000LL, 0x7FFFFFFF00000000LL, 0x0123456789ABCDEFLL, 0x0000000072233411LL,  //  8  9 10 11
      0x0003333300330033LL, 0x8000000100000001LL, 0x4238572200000000LL, 0x000000FFFFF00000LL,  // 12 13 14 15
  };

  // Show results
#define PRINT1(name, i) \
    unittest_printf(name"([%02d]) -> [%08X]\n", i, getCR());
#define PRINT2(name, i, j) \
    unittest_printf(name"([%02d],[%02d]) -> [%08X]\n", i, j, getCR());
#define PRINT3(name, i, j, k) \
    unittest_printf(name"([%02d],[%02d],[%02d]) -> [%08X]\n", i, j, k, getCR());
#define PRINT4(name, i, j, k, l) \
    unittest_printf(name"([%02d],[%02d],[%02d],[%02d]) -> [%08X]\n", i, j, k, l, getCR());

// Iterate
#define ITERATE1(x) \
    for (uint32_t i=0; i<sizeof(testInts64)/sizeof(testInts64[0]); i++) \
        {uint64_t r1=testInts64[i]; clearCR(); x;}

#define ITERATE2(x) \
    for (uint32_t i=0; i<sizeof(testInts64)/sizeof(testInts64[0]); i++) \
        for (uint32_t j=0; j<sizeof(testInts64)/sizeof(testInts64[0]); j++) \
            {uint64_t r1=testInts64[i], r2=testInts64[j]; clearCR(); x;}


  // Integer Compare Instructions
  ITERATE1(__asm__("cmpi   cr0,%0,%1,%2" : : "i"(0), "r"(r1), "i"(0));      PRINT3("cmpi ", 0, i, 0));
  ITERATE1(__asm__("cmpi   cr1,%0,%1,%2" : : "i"(0), "r"(r1), "i"(1));      PRINT3("cmpi ", 0, i, 1));
  ITERATE1(__asm__("cmpi   cr2,%0,%1,%2" : : "i"(0), "r"(r1), "i"(-1));      PRINT3("cmpi ", 0, i, -1));
  ITERATE1(__asm__("cmpi   cr3,%0,%1,%2" : : "i"(0), "r"(r1), "i"(32767));  PRINT3("cmpi ", 0, i, 32767));
  ITERATE1(__asm__("cmpi   cr4,%0,%1,%2" : : "i"(0), "r"(r1), "i"(-32768));  PRINT3("cmpi ", 0, i, -32768));
  ITERATE1(__asm__("cmpi   cr5,%0,%1,%2" : : "i"(0), "r"(r1), "i"(-32767));  PRINT3("cmpi ", 0, i, -32767));
  ITERATE1(__asm__("cmpi   cr6,%0,%1,%2" : : "i"(1), "r"(r1), "i"(0));      PRINT3("cmpi ", 1, i, 0));
  ITERATE1(__asm__("cmpi   cr7,%0,%1,%2" : : "i"(1), "r"(r1), "i"(1));      PRINT3("cmpi ", 1, i, 1));
  ITERATE1(__asm__("cmpi   cr0,%0,%1,%2" : : "i"(1), "r"(r1), "i"(-1));      PRINT3("cmpi ", 1, i, -1));
  ITERATE1(__asm__("cmpi   cr1,%0,%1,%2" : : "i"(1), "r"(r1), "i"(32767));  PRINT3("cmpi ", 1, i, 32767));
  ITERATE1(__asm__("cmpi   cr2,%0,%1,%2" : : "i"(1), "r"(r1), "i"(-32768));  PRINT3("cmpi ", 1, i, -32768));
  ITERATE1(__asm__("cmpi   cr3,%0,%1,%2" : : "i"(1), "r"(r1), "i"(-32767));  PRINT3("cmpi ", 1, i, -32767));
  ITERATE2(__asm__("cmp    cr4,%0,%1,%2" : : "i"(0), "r"(r1), "r"(r2));      PRINT3("cmp  ", 0, i, j));
  ITERATE2(__asm__("cmp    cr5,%0,%1,%2" : : "i"(1), "r"(r1), "r"(r2));      PRINT3("cmp  ", 1, i, j));
  ITERATE1(__asm__("cmpli  cr6,%0,%1,%2" : : "i"(0), "r"(r1), "i"(0));      PRINT3("cmpli", 0, i, 0));
  ITERATE1(__asm__("cmpli  cr7,%0,%1,%2" : : "i"(0), "r"(r1), "i"(1));      PRINT3("cmpli", 0, i, 1));
  ITERATE1(__asm__("cmpli  cr0,%0,%1,%2" : : "i"(0), "r"(r1), "i"(0xffff));  PRINT3("cmpli", 0, i, 0xffff));
  ITERATE1(__asm__("cmpli  cr1,%0,%1,%2" : : "i"(0), "r"(r1), "i"(0x7fff));  PRINT3("cmpli", 0, i, 0x7fff));
  ITERATE1(__asm__("cmpli  cr2,%0,%1,%2" : : "i"(0), "r"(r1), "i"(0x8000));  PRINT3("cmpli", 0, i, 0x8000));
  ITERATE1(__asm__("cmpli  cr3,%0,%1,%2" : : "i"(0), "r"(r1), "i"(0x8001));  PRINT3("cmpli", 0, i, 0x8001));
  ITERATE1(__asm__("cmpli  cr4,%0,%1,%2" : : "i"(1), "r"(r1), "i"(0));      PRINT3("cmpli", 1, i, 0));
  ITERATE1(__asm__("cmpli  cr5,%0,%1,%2" : : "i"(1), "r"(r1), "i"(1));      PRINT3("cmpli", 1, i, 1));
  ITERATE1(__asm__("cmpli  cr6,%0,%1,%2" : : "i"(1), "r"(r1), "i"(0xffff));  PRINT3("cmpli", 1, i, 0xffff));
  ITERATE1(__asm__("cmpli  cr7,%0,%1,%2" : : "i"(1), "r"(r1), "i"(0x7fff));  PRINT3("cmpli", 1, i, 0x7fff));
  ITERATE1(__asm__("cmpli  cr0,%0,%1,%2" : : "i"(1), "r"(r1), "i"(0x8000));  PRINT3("cmpli", 1, i, 0x8000));
  ITERATE1(__asm__("cmpli  cr1,%0,%1,%2" : : "i"(1), "r"(r1), "i"(0x8001));  PRINT3("cmpli", 1, i, 0x8001));
  ITERATE2(__asm__("cmpl   cr2,%0,%1,%2" : : "i"(0), "r"(r1), "r"(r2));      PRINT3("cmpl ", 0, i, j));
  ITERATE2(__asm__("cmpl   cr3,%0,%1,%2" : : "i"(1), "r"(r1), "r"(r2));      PRINT3("cmpl ", 1, i, j));


  END_TEST;
}

BEGIN_TEST_CASE(ppc_cmp)
RUN_TEST(test_cmp);
END_TEST_CASE(ppc_cmp)