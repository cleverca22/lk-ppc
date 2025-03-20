.section .text.boot
.global _start
_start:
  lis %r1, __stack_bottom@h
  ori %r1, %r1, __stack_bottom@l

  lis %r2, .TOC.@h
  ori %r2, %r2, .TOC.@l

  lis %r3, clear_bss@h
  ori %r3, %r3, clear_bss@l
  mtctr %r3
  bctrl

  lis %r3, lk_main@h
  ori %r3, %r3, lk_main@l
  mtctr %r3
  bctr
  b .

.section .text.hypercall
.global do_hypercall4
do_hypercall4:
  b do_hypercall
.global do_hypercall
do_hypercall:
  sc 1
  blr

.text
.global ppc64_context_switch
// r3, old thread
// r4, new thread
ppc64_context_switch:
  nop
  nop
  mflr %r5
  std %r5, 0(%r3)
  std %r1, 8(%r3)
  std %r14, 16(%r3)
  std %r15, 24(%r3)
  std %r16, 32(%r3)
  std %r17, 40(%r3)
  std %r18, 48(%r3)
  std %r19, 56(%r3)
  std %r20, 64(%r3)
  std %r21, 72(%r3)
  std %r22, 80(%r3)
  std %r23, 88(%r3)
  std %r24, 96(%r3)
  std %r25, 104(%r3)
  std %r26, 112(%r3)
  std %r27, 120(%r3)
  std %r28, 128(%r3)
  std %r29, 136(%r3)
  std %r30, 144(%r3)
  std %r31, 152(%r3)

  ld %r5, 0(%r4)
  ld %r1, 8(%r4)
  ld %r14, 16(%r4)
  ld %r15, 24(%r4)
  ld %r16, 32(%r4)
  ld %r17, 40(%r4)
  ld %r18, 48(%r4)
  ld %r19, 56(%r4)
  ld %r20, 64(%r4)
  ld %r21, 72(%r4)
  ld %r22, 80(%r4)
  ld %r23, 88(%r4)
  ld %r24, 96(%r4)
  ld %r25, 104(%r4)
  ld %r26, 112(%r4)
  ld %r27, 120(%r4)
  ld %r28, 128(%r4)
  ld %r29, 136(%r4)
  ld %r30, 144(%r4)
  ld %r31, 152(%r4)
  mtlr %r5
  blr
