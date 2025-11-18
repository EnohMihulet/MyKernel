#pragma once
#include "commonTypes.h"
#include "commonUtils.h"
#include "printUtils.h"

typedef struct BootInfo {
	u64 dtb_phys;
	u64 MPIDR;
	u64 ram_base;
	u64 ram_size;
} BootInfo;

static u64 cpu_id(u64 MPIDR) {
	return MPIDR & 0xFF;
}

static inline void printBootInfo(const BootInfo* bi) {
	kprintf("dtb_phys: 0x%x\nMPIDR: %llu\nram_base: 0x%x\nram_size: %llu\n", bi->dtb_phys, bi->MPIDR, bi->ram_base, bi->ram_size);
	kprintf("cpu_id: %d\n", cpu_id(bi->MPIDR));
}
