	.cpu arm7tdmi
	.fpu softvfp
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 1
	.eabi_attribute 30, 6
	.eabi_attribute 18, 4
	.file	"main.c"
	.bss
	.align	2
times.1283:
	.space	4
	.text
	.align	2
	.global	Isr_TIMER0
	.type	Isr_TIMER0, %function
Isr_TIMER0:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	stmfd	sp!, {fp, lr}
	add	fp, sp, #4
	ldr	r3, .L3
	ldr	r3, [r3, #0]
	add	r2, r3, #1
	ldr	r3, .L3
	str	r2, [r3, #0]
	ldr	r3, .L3
	ldr	r3, [r3, #0]
	mov	r0, r3
	bl	wheel
	mov	r3, #-536870912
	add	r3, r3, #16384
	mov	r2, #1
	str	r2, [r3, #0]
	sub	sp, fp, #4
	ldmfd	sp!, {fp, lr}
	bx	lr
.L4:
	.align	2
.L3:
	.word	times.1283
	.size	Isr_TIMER0, .-Isr_TIMER0
	.align	2
	.global	Delay
	.type	Delay, %function
Delay:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	@ link register save eliminated.
	str	fp, [sp, #-4]!
	add	fp, sp, #0
	sub	sp, sp, #12
	str	r0, [fp, #-8]
	b	.L6
.L7:
	ldr	r3, [fp, #-8]
	sub	r3, r3, #1
	str	r3, [fp, #-8]
.L6:
	ldr	r3, [fp, #-8]
	cmp	r3, #0
	bne	.L7
	add	sp, fp, #0
	ldmfd	sp!, {fp}
	bx	lr
	.size	Delay, .-Delay
	.align	2
	.global	main
	.type	main, %function
main:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	stmfd	sp!, {fp, lr}
	add	fp, sp, #4
	mov	r2, #-536870912
	add	r2, r2, #2080768
	add	r2, r2, #416
	mov	r3, #-536870912
	add	r3, r3, #2080768
	add	r3, r3, #416
	ldr	r3, [r3, #0]
	orr	r3, r3, #1
	str	r3, [r2, #0]
	bl	set_pll
	mvn	r3, #4080
	sub	r3, r3, #3
	mov	r2, #0
	str	r2, [r3, #0]
	bl	stepperinit
	mov	r3, #1073741824
	sub	r3, r3, #16384
	mov	r2, #0
	str	r2, [r3, #0]
	mov	r3, #16384
	add	r3, r3, #-536870900
	mov	r2, #0
	str	r2, [r3, #0]
	mov	r3, #-536870912
	add	r3, r3, #16384
	add	r3, r3, #24
	mov	r2, #61952
	str	r2, [r3, #0]
	mov	r3, #-536870912
	add	r3, r3, #16384
	add	r3, r3, #20
	mov	r2, #3
	str	r2, [r3, #0]
	mvn	r3, #16192
	sub	r3, r3, #-1073741781
	mov	r2, #2
	str	r2, [r3, #0]
	mov	r3, #0
	sub	r3, r3, #4080
	mov	r2, #16
	str	r2, [r3, #0]
	mov	r0, #4
	ldr	r1, .L12
	mov	r2, #15
	mov	r3, #0
	bl	RegisterVector
	bl	IrqEnable
	mov	r3, #16384
	add	r3, r3, #-536870908
	mov	r2, #1
	str	r2, [r3, #0]
.L10:
	b	.L10
.L13:
	.align	2
.L12:
	.word	Isr_TIMER0
	.size	main, .-main
	.comm	dstnsa,4,4
	.comm	dstnsb,4,4
	.comm	status,4,4
	.ident	"GCC: (Sourcery G++ Lite 2009q1-161) 4.3.3"
