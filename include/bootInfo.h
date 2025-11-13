#pragma once
#include "commonTypes.h"
#include "commonUtils.h"
#include "commonUART.h"

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
	char buff[24];
	htos(bi->dtb_phys, buff);
	uart_puts(buff);
	memset(buff, 0, 24);
	uart_putc('\n');

	uitos(cpu_id(bi->MPIDR), buff);
	uart_puts(buff);
	memset(buff, 0, 24);
	uart_putc('\n');

	htos(bi->ram_base, buff);
	uart_puts(buff);
	memset(buff, 0, 24);
	uart_putc('\n');

	uitos(bi->ram_size, buff);
	uart_puts(buff);
	memset(buff, 0, 24);
	uart_putc('\n');
}
