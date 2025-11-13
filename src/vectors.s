	.section .vectors, "ax"
	.align 11
	.globl __vector_table_el1
__vector_table_el1:
	// 0: EL1t (SP0) synchronous
	b default_sync_sp0
	.space 128-4
	// 1: EL1t (SP0) IRQ
	b default_irq_sp0
	.space 128-4
	// 2: EL1t (SP0) FIQ
	b default_fiq_sp0
	.space 128-4
	// 3: EL1t (SP0) SError
	b default_serr_sp0
	.space 128-4

	// 4: EL1h (SPx) synchronous
	b default_sync_spx
	.space 128-4
	// 5: EL1h (SPx) IRQ
	b default_irq_spx
	.space 128-4
	// 6: EL1h (SPx) FIQ
	b default_fiq_spx
	.space 128-4
	// 7: EL1h (SPx) SError
	b default_serr_spx
	.space 128-4

	// 8: from lower EL (AArch64) synchronous
	b default_sync_a64
	.space 128-4
	// 9: from lower EL (AArch64) IRQ
	b default_irq_a64
	.space 128-4
	// 10: from lower EL (AArch64) FIQ
	b default_fiq_a64
	.space 128-4
	// 11: from lower EL (AArch64) SError
	b default_serr_a64
	.space 128-4

	// 12–15: from lower EL (AArch32) — not used
	b default_sync_a32
	.space 128-4
	b default_irq_a32
	.space 128-4
	b default_fiq_a32
	.space 128-4
	b default_serr_a32
	.space 128-4

	.align 7
	default_sync_sp0:
	default_irq_sp0:
	default_fiq_sp0:
	default_serr_sp0:
	default_sync_spx:
	default_irq_spx:
	default_fiq_spx:
	default_serr_spx:
	default_sync_a64:
	default_irq_a64:
	default_fiq_a64:
	default_serr_a64:
	default_sync_a32:
	default_irq_a32:
	default_fiq_a32:
	default_serr_a32:
	b .
