#pragma once
#include "commonTypes.h"
#include "printUtils.h"

static const char* SYNC_MSG = "EL1h synchronous exception.\n\n";
static const char* IRQ_MSG = "IRQ interupt\n";
static const char* FIQ_MSG = "FIQ interupt.\n";
static const char* SERR_MSG = "SError.\nESR=0x%lx ELR=0x%lx\n";
static const char* EXCEPTION_STR = "ESR=0x%lx ELR=0x%lx FAR=0x%lx SPSR=0x%lx\n\n";

#define GIC_DIST_BASE   0x08000000UL
#define GIC_CPU_BASE    0x08010000UL

// Distributor registers
#define GICD_CTLR       	(GIC_DIST_BASE + 0x000)
#define GICD_ISENABLER(n)	(GIC_DIST_BASE + 0x100 + 4*(n))
#define GICD_ICENABLER(n)	(GIC_DIST_BASE + 0x180 + 4*(n))
#define GICD_IPRIORITYR(n)	(GIC_DIST_BASE + 0x400 + 4*(n))
#define GICD_ITARGETSR(n)	(GIC_DIST_BASE + 0x800 + 4*(n))
#define GICD_IGROUPR(n)		(GIC_DIST_BASE + 0x080 + 4*(n))

// CPU interface registers
#define GICC_CTLR	(GIC_CPU_BASE + 0x000)
#define GICC_PMR        (GIC_CPU_BASE + 0x004)
#define GICC_BPR        (GIC_CPU_BASE + 0x008)
#define GICC_IAR        (GIC_CPU_BASE + 0x00C)
#define GICC_EOIR       (GIC_CPU_BASE + 0x010)

#define TIMER_INTID 30 // EL1 physical timer


#define EC_UNKNOWN 			0b000000
#define EC_WFX_TRAP			0b000001
#define EC_FP_ASIMD_ACCESS          	0b000111
#define EC_ILLEGAL_EXEC_STATE       	0b001110
#define EC_SVC_INSTR_AA64		0b010101
#define EC_SYSREG_TRAP_AA64         	0b011000
#define EC_LOWER_EL_INSTR_ABORT		0b100000
#define EC_SAME_EL_INSTR_ABORT		0b100001
#define EC_PC_ALIGNMENT_FAULT       	0b100010
#define EC_LOWER_EL_DATA_ABORT		0b100100
#define EC_SAME_EL_DATA_ABORT		0b100101
#define EC_STACK_ALIGNMENT		0b100110
#define EC_MEMORY_OPERATION		0b100111
#define EC_BRK_INSTR_AA64		0b111100


typedef struct RegistersFrame {
	u64 r[31];
	u64 sp;
} RegistersFrame;

static inline void setupExceptionVectors() {
	__asm__ volatile("ldr x5, =__vector_table_el1\n"
			 "msr	VBAR_EL1, x5\n"
			 "isb");
}

static inline u64 get_esr_el1() {
	u64 esr;
	__asm__ volatile("mrs %0, ESR_EL1" : "=r"(esr));
	return esr;
}

static inline u64 read_elr_el1()  {
	u64 elr;
	__asm__ volatile("mrs %0, ELR_EL1" : "=r"(elr));
	return elr;
}

static inline u64 read_far_el1() {
	u64 far;
	__asm__ volatile("mrs %0, FAR_EL1" : "=r"(far));
	return far;
}

static inline u64 read_spsr_el1() {
	u64 spsr;
	__asm__ volatile("mrs %0, SPSR_EL1" : "=r"(spsr));
	return spsr;
}

static inline u64 read_cntfrq() {
	u64 frq;
	__asm__ volatile("mrs %0, CNTFRQ_EL0" : "=r"(frq));
	return frq;
}

static inline void write_cntp_tval(u32 val) {
	asm volatile("msr CNTP_TVAL_EL0, %0" :: "r"((u64)val));
}

static inline void write_cntp_ctl(u32 val) {
	asm volatile("msr CNTP_CTL_EL0, %0" :: "r"((u64)val));
}

static inline void mmio_write32(u64 addr, u32 val) {
	*(volatile u32*)addr = val;
}

static inline u32 mmio_read32(u64 addr) {
	return *(volatile u32*)addr;
}

void timerInit1hz();

void gicInit();

static inline void enableIRQ() {
	__asm__ volatile("msr DAIFClr, #2"); // Enable IRQ
}

void dumpRegisters(RegistersFrame* regs);

void panic(RegistersFrame* regs, u64 ESR_EL1, u64 ELR_EL1, u64 FAR_EL1, u64 SPSR_EL1, char* panicMsg);

void parseESR_EL1(u64 ESR_EL1);

void handleSyncException(RegistersFrame* regs, u64 ESR_EL1, u64 ELR_EL1, u64 FAR_EL1, u64 SPSR_EL1);

void handleIRQ(RegistersFrame* regs);

void handleFIQ();

void handleSError(RegistersFrame* regs, u64 ESR_EL1, u64 ELR_EL1, u64 FAR_EL1, u64 SPSR_EL1);
