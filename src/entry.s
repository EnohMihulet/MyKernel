	.section .text._start, "ax"
	.global _start
	.extern kernelMain
	.extern __bss_start
	.extern __bss_end
	.extern __stack_top
	.extern __vector_table_el1

_start:
	msr	daifset, #0b1111

	mrs	x0, CurrentEL
	lsr	x0, x0, #2

	cmp	x0, #2
	bne	setUpStack
	bl	EL2ToEL1

setUpStack:
	ldr	x1, =__stack_top
	mov	sp, x1

zeroBSS:
	ldr	x2, =__bss_start
	ldr	x3, =__bss_end
	mov	x4, xzr
zeroBSSLoop:
	cmp	x2, x3
	bhs	setUpExceptionVectors
	str	x4, [x2], #8
	b	zeroBSSLoop

setUpExceptionVectors:
	ldr	x5, =__vector_table_el1
	msr	VBAR_EL1, x5
	isb

//	msr	x6, CPACR_EL1
//	orr 	x6, x6, #(0b11 << 20)
//	msr	CPACR_EL1, x6
//	isb

	sub	sp, sp, #32
	mov	x7, #0x40000000

	mrs	x8, MPIDR_EL1

	mov	x9, #0x40000000
	mov	x10, #0

	str	x7, [sp, #0]
	str	x8, [sp, #8]
	str	x9, [sp, #16]
	str	x10, [sp, #24]

	mov	x0, sp
	
	bl	kernelMain

halt:
	wfe 
	b	halt

EL2ToEL1:
	mov	x1, #(1<<0) | (1<<1)
	msr	CNTHCTL_EL2, 	x1
	msr	CNTVOFF_EL2,	xzr

	mov	x1, #0
	msr	HCR_EL2, x1

	ldr	x1, =__stack_top
	msr	SP_EL1, x1

	mov	x1, #(0b0101)
	orr	x1, x1, #(0b1111<<6)
	msr	SPSR_EL2, x1

	adr	x1, setUpStack
	msr	ELR_EL2, x1
	eret

1:	ret
	

