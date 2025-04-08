#include <arch/mmu.h>
#include <stdio.h>

status_t arch_mmu_init_aspace(arch_aspace_t *aspace, vaddr_t base, size_t size, uint flags) {
  printf("arch_mmu_init_aspace(%p, 0x%x, 0x%x, 0x%x)\n", aspace, base, size, flags);
  return 0;
}

status_t arch_mmu_destroy_aspace(arch_aspace_t *aspace) {
  printf("arch_mmu_destroy_aspace(%p)\n", aspace);
  return 0;
}

int arch_mmu_map(arch_aspace_t *aspace, vaddr_t vaddr, paddr_t paddr, uint count, uint flags) {
  printf("arch_mmu_map(%p, 0x%x, 0x%x, %d, 0x%x)\n", aspace, vaddr, paddr, count, flags);
  return 0;
}

int arch_mmu_unmap(arch_aspace_t *aspace, vaddr_t vaddr, uint count) {
  printf("arch_mmu_unmap(%p, 0x%x, %d)\n", aspace, vaddr, count);
  return 0;
}

status_t arch_mmu_query(arch_aspace_t *aspace, vaddr_t vaddr, paddr_t *paddr, uint *flags) {
  printf("arch_mmu_query(%p, 0x%x, 0x%x, 0x%x)\n", aspace, vaddr, paddr, flags);
  *paddr = vaddr;
  *flags = 0;
  return 0;
}

void arch_mmu_context_switch(arch_aspace_t *aspace) {
  printf("arch_mmu_context_switch(%p)\n", aspace);
}
