	.section .vectors, "ax"
	.align 11
	.global __vector_table_el1
	.extern kprintf
__vector_table_el1:
	b default_sync_sp0
	.space 128-4
	b default_irq_sp0
	.space 128-4
	b default_fiq_sp0
	.space 128-4
	b default_serr_sp0
	.space 128-4

	b default_sync_spx
	.space 128-4
	b default_irq_spx
	.space 128-4
	b default_fiq_spx
	.space 128-4
	b default_serr_spx
	.space 128-4

	b default_sync_a64
	.space 128-4
	b default_irq_a64
	.space 128-4
	b default_fiq_a64
	.space 128-4
	b default_serr_a64
	.space 128-4

	b default_sync_a32
	.space 128-4
	b default_irq_a32
	.space 128-4
	b default_fiq_a32
	.space 128-4
	b default_serr_a32
	.space 128-4

1:
	wfe
	b 	1b

	.align 7
default_sync_sp0:
default_sync_a32:
default_sync_a64:
default_sync_spx:
	sub	sp, sp, #256

	stp	x0, x1, [sp, #0*16]
	stp	x2, x3, [sp, #1*16]
	stp	x4, x5, [sp, #2*16]
	stp	x6, x7, [sp, #3*16]
	stp	x8, x9, [sp, #4*16]
	stp	x10, x11, [sp, #5*16]
	stp	x12, x13, [sp, #6*16]
	stp	x14, x15, [sp, #7*16]
	stp	x16, x17, [sp, #8*16]
	stp	x18, x19, [sp, #9*16]
	stp	x20, x21, [sp, #10*16]
	stp	x22, x23, [sp, #11*16]
	stp	x24, x25, [sp, #12*16]
	stp	x26, x27, [sp, #13*16]
	stp	x28, x29, [sp, #14*16]
	str	x30, [sp, #15*16]

	mov	x0, sp

	mrs	x1, esr_el1
	mrs	x2, elr_el1
	mrs	x3, far_el1
	mrs	x4, spsr_el1

	bl	handleSyncException

	b 	1b

	.align 7
default_irq_sp0:
default_irq_a32:
default_irq_a64:
default_irq_spx:

	sub	sp, sp, #256

	stp	x0, x1, [sp, #0*16]
	stp	x2, x3, [sp, #1*16]
	stp	x4, x5, [sp, #2*16]
	stp	x6, x7, [sp, #3*16]
	stp	x8, x9, [sp, #4*16]
	stp	x10, x11, [sp, #5*16]
	stp	x12, x13, [sp, #6*16]
	stp	x14, x15, [sp, #7*16]
	stp	x16, x17, [sp, #8*16]
	stp	x18, x19, [sp, #9*16]
	stp	x20, x21, [sp, #10*16]
	stp	x22, x23, [sp, #11*16]
	stp	x24, x25, [sp, #12*16]
	stp	x26, x27, [sp, #13*16]
	stp	x28, x29, [sp, #14*16]
	str	x30, [sp, #15*16]

	mov	x0, sp

	bl	handleIRQ

	ldp	x0, x1, [sp], #16
	ldp	x2, x3, [sp], #16
	ldp	x4, x5, [sp], #16
	ldp	x6, x7, [sp], #16
	ldp	x8, x9, [sp], #16
	ldp	x10, x11, [sp], #16
	ldp	x12, x13, [sp], #16
	ldp	x14, x15, [sp], #16
	ldp	x16, x17, [sp], #16
	ldp	x18, x19, [sp], #16
	ldp	x20, x21, [sp], #16
	ldp	x22, x23, [sp], #16
	ldp	x24, x25, [sp], #16
	ldp	x26, x27, [sp], #16
	ldp	x28, x29, [sp], #16
	ldr	x30, [sp], #16

	eret

	.align 7
default_fiq_sp0:
default_fiq_spx:
default_fiq_a64:
default_fiq_a32:
	stp	x29, x30, [sp, #-16]!

	bl	handleFIQ

	b	1b

	.align 7
default_serr_sp0:
default_serr_a32:
default_serr_a64:
default_serr_spx:
	sub	sp, sp, #256

	stp	x0, x1, [sp, #0*16]
	stp	x2, x3, [sp, #1*16]
	stp	x4, x5, [sp, #2*16]
	stp	x6, x7, [sp, #3*16]
	stp	x8, x9, [sp, #4*16]
	stp	x10, x11, [sp, #5*16]
	stp	x12, x13, [sp, #6*16]
	stp	x14, x15, [sp, #7*16]
	stp	x16, x17, [sp, #8*16]
	stp	x18, x19, [sp, #9*16]
	stp	x20, x21, [sp, #10*16]
	stp	x22, x23, [sp, #11*16]
	stp	x24, x25, [sp, #12*16]
	stp	x26, x27, [sp, #13*16]
	stp	x28, x29, [sp, #14*16]
	str	x30, [sp, #15*16]

	mov	x0, sp

	mrs	x1, esr_el1
	mrs	x2, elr_el1
	mrs	x3, far_el1
	mrs	x4, spsr_el1

	bl	handleSError

	b	1b


