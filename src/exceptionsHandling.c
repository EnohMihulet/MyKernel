#include "../include/exceptionHandling.h"

void timerInit1hz() {
	u64 frq = read_cntfrq();
	
	write_cntp_tval((u32)frq);
	write_cntp_ctl(1);
}

void gicInit() {
	// Disable distributor & cpu interface while configuring
	mmio_write32(GICD_CTLR, 0x0);
	mmio_write32(GICC_CTLR, 0x0);

	// Set al linterrupts to Group 1 (non-secure) - SPIs, PPIs, SGIs
	mmio_write32(GICD_IGROUPR(0), 0xFFFFFFFF);

	// Enable the timer PPI
	u32 reg = mmio_read32(GICD_ISENABLER(0));
	reg |= (1u << TIMER_INTID);
	mmio_write32(GICD_ISENABLER(0), reg);
	
	// Set priority for interrupts (0 highest - 0xFF lowest)
	// Each IPRIORITY contains 4 interrupts (8bits each)
	u32 prioIdx = TIMER_INTID / 4;
	u32 prioShift = (TIMER_INTID % 4) * 8;
	u32 prioReg = mmio_read32(GICD_IPRIORITYR(prioIdx));
	prioReg &= ~(0xFFu << prioShift); // Mask out priority bits
	prioReg |= (0x80u << prioShift);  // Set priority to 0x80 (middle)
	mmio_write32(GICD_IPRIORITYR(prioIdx), prioReg);

	mmio_write32(GICC_PMR, 0xFF); // CPU Interface: accept all priorities <= 0x0FF
	mmio_write32(GICC_BPR, 0x0);  // No priority grouping

	mmio_write32(GICC_CTLR, 0x1); // Enable CPU interface
	mmio_write32(GICD_CTLR, 0x1); // Enable distributor
}

void dumpRegisters(RegistersFrame* regs) {
	for (u16 i = 0; i < 31; i++) {
		kprintf("x%u = 0x%lx\n", i, regs->r[i]);
	}
	kprintf("sp = 0x%lx\n", regs->sp);
}

void panic(RegistersFrame* regs, u64 ESR_EL1, u64 ELR_EL1, u64 FAR_EL1, u64 SPSR_EL1, char* panicMsg) {
	kprintf("KERNEL PANIC: %s\n", panicMsg);

	kprintf(EXCEPTION_STR, ESR_EL1, ELR_EL1, FAR_EL1, SPSR_EL1);
	dumpRegisters(regs);

	while (1) {
		__asm__ volatile("wfi");
	}
}

char* decodeDFSC(u32 dfsc) {
	u32 type = dfsc & 0b111100;
	switch (type) {
		case 0b000000: return "alignment fault";
		case 0b000100: return "translation fault";
		case 0b001000: return "access fault";
		case 0b001100: return "permission fault";
	}
	return "not recognized";
}

void handleSyncException(RegistersFrame* regs, u64 ESR_EL1, u64 ELR_EL1, u64 FAR_EL1, u64 SPSR_EL1) {
	u32 ec = (ESR_EL1 >> 26) & 0x3F;
	u32 il = (ESR_EL1 >> 25) & 0x1;
	u32 iss = ESR_EL1 & 0x1FFFFFF;

	kprintf(SYNC_MSG);

	switch (ec) {
	case EC_UNKNOWN:
		panic(regs, ESR_EL1, ELR_EL1, FAR_EL1, SPSR_EL1, "Unknown Exception Class.\n");
		break;
	case EC_WFX_TRAP: {
		u32 rn = (iss >> 5) & 0x1F;
		u32 rv = (iss >> 2) & 0x1;
		u32 ti = iss & 0x3;
		switch (ti) {
		case 0: kprintf("WFI trapped at %lx\n.", ELR_EL1); break;
		case 1: kprintf("WFE trapped at %lx\n.", ELR_EL1); break;
		case 2: kprintf("WFIT trapped at %lx\n.", ELR_EL1); break;
		case 3: kprintf("WFET trapped at %lx\n.", ELR_EL1); break;
		}
		if ((ti & 0b10) && rv) kprintf("Register Number: %u\n", rn);
		panic(regs, ESR_EL1, ELR_EL1, FAR_EL1, SPSR_EL1, "Unexpected WFx trap\n");
		break;
		}
	case EC_FP_ASIMD_ACCESS:
		panic(regs, ESR_EL1, ELR_EL1, FAR_EL1, SPSR_EL1, "Unexpected FP/SIMD access trap. FP should be enabled.\n");
		break;
	case EC_ILLEGAL_EXEC_STATE:
		panic(regs, ESR_EL1, ELR_EL1, FAR_EL1, SPSR_EL1, "Illegal Exectution State.\n");
		break;
	case EC_SVC_INSTR_AA64: {
		u16 imm = iss & 0xFFFF;
		kprintf("SVC trap at ELR=%lx, imm=0x%x\n", ELR_EL1, imm);
		panic(regs, ESR_EL1, ELR_EL1, FAR_EL1, SPSR_EL1, "Unexpected SVC.\n");
		break;
		}
	case EC_SYSREG_TRAP_AA64: {
		kprintf("Trapped MSR/MRS/System instruction at ELR=%lx\n", ELR_EL1);
		kprintf("ISS = 0x%x\n", iss);
		panic(regs, ESR_EL1, ELR_EL1, FAR_EL1, SPSR_EL1, "Unexpected System Intruction Trap.\n");
		break;
		}
	case EC_LOWER_EL_INSTR_ABORT: {
		u32 ifsc = iss & 0x3F;
		kprintf("Lower EL Instruction Abort: IFSC=0x%x\n", ifsc);
		panic(regs, ESR_EL1, ELR_EL1, FAR_EL1, SPSR_EL1, "Lower EL Instruction Abort.\n");
		break;
		}
	case EC_SAME_EL_INSTR_ABORT: {
		u32 ifsc = iss & 0x3F;
		kprintf("Same EL Instruction Abort: IFSC=0x%x\n", ifsc);
		panic(regs, ESR_EL1, ELR_EL1, FAR_EL1, SPSR_EL1, "Same EL Instruction Abort.\n");
		break;
		}
	case EC_PC_ALIGNMENT_FAULT:
		panic(regs, ESR_EL1, ELR_EL1, FAR_EL1, SPSR_EL1, "PC Alignment Fault.\n");
		break;
	case EC_LOWER_EL_DATA_ABORT: {
		u32 dfsc = iss & 0x3F;
		u16 wnr = (iss >> 6) & 0x1;
		kprintf("Lower EL Data Abort: DFSC=0x%x (%s), %s at 0x%lx\n", dfsc, decodeDFSC(dfsc), wnr ? "write" : "read", FAR_EL1);
		panic(regs, ESR_EL1, ELR_EL1, FAR_EL1, SPSR_EL1, "Lower EL Data Abort.\n");
		break;
		}
	case EC_SAME_EL_DATA_ABORT: {
		u32 dfsc = iss & 0x3F;
		u16 wnr = (iss >> 6) & 0x1;
		kprintf("Same EL Data Abort: DFSC=0x%x (%s), %s at 0x%lx\n", dfsc, decodeDFSC(dfsc), wnr ? "write" : "read", FAR_EL1);
		panic(regs, ESR_EL1, ELR_EL1, FAR_EL1, SPSR_EL1, "Same EL Data Abort.\n");
		break;
		}
	case EC_STACK_ALIGNMENT:
		panic(regs, ESR_EL1, ELR_EL1, FAR_EL1, SPSR_EL1, "SP Alignment Fault.\n");
		break;
	case EC_MEMORY_OPERATION: {
		panic(regs, ESR_EL1, ELR_EL1, FAR_EL1, SPSR_EL1, "Memory Operation Exception.\n");
		break;
		}
	case EC_BRK_INSTR_AA64: {
		u16 imm = iss & 0xFFFF;
		kprintf("BRK trapped at ELR=0x%lx, imm=0x%x\n", ELR_EL1, imm);
		panic(regs, ESR_EL1, ELR_EL1, FAR_EL1, SPSR_EL1, "BRK Instruction Execution\n");
		break;
		}
	default:
		kprintf("Exception Class: %u\n", ec);
		panic(regs, ESR_EL1, ELR_EL1, FAR_EL1, SPSR_EL1, "Unrecognized Exception Class.\n");
	}

	kprintf(EXCEPTION_STR, ESR_EL1, ELR_EL1, FAR_EL1, SPSR_EL1);
}

void handleIRQ(RegistersFrame* regs) {
	kprintf(IRQ_MSG);
	return;
}

void handleFIQ() {
	kprintf(FIQ_MSG);
	while (1) {
		__asm__ volatile("wfi");
	}
}

void handleSError(RegistersFrame* regs, u64 ESR_EL1, u64 ELR_EL1, u64 FAR_EL1, u64 SPSR_EL1) {
	panic(regs, ESR_EL1, ELR_EL1, FAR_EL1, SPSR_EL1, "SError Recieved.\n");

}
