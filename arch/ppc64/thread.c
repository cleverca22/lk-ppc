#include <kernel/thread.h>
#include <string.h>

static void initial_thread_func(void) __NO_RETURN;
static void initial_thread_func(void) {
  thread_t *ct = get_current_thread();
  spin_unlock(&thread_lock);
  arch_enable_ints();
  int ret = ct->entry(ct->arg);
  thread_exit(ret);
}

void arch_thread_initialize(thread_t *t) {
  memset(&t->arch, 0, sizeof(struct arch_thread));
  t->arch.lr = (uint64_t)&initial_thread_func;
  t->arch.sp = (uint64_t)((t->stack + t->stack_size) - 8);
}

void arch_dump_thread(thread_t *t) {
}

void ppc64_context_switch(struct arch_thread *oldsp, struct arch_thread *newsp);

void arch_context_switch(thread_t *oldthread, thread_t *newthread) {
  ppc64_context_switch(&oldthread->arch, &newthread->arch);
}
