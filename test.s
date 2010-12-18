	.cpu arm7tdmi
	.fpu softvfp
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 1
	.eabi_attribute 30, 1
	.eabi_attribute 18, 4
	.file	"test.c"
	.text
	.align	2
	.global	ymzwrite0
	.type	ymzwrite0, %function
ymzwrite0:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	mvn	ip, #-1073741824
	sub	ip, ip, #12288
	mov	r3, #0
	str	r3, [ip, #-3947]
	str	r1, [ip, #-3947]
	add	r3, r1, #512
	str	r3, [ip, #-3947]
	and	r3, r0, #255
	add	r2, r3, #256
	str	r2, [ip, #-3947]
	add	r3, r3, #768
	str	r3, [ip, #-3947]
	add	r3, r1, #1
	str	r3, [ip, #-3947]
	add	r1, r1, #512
	add	r1, r1, #1
	str	r1, [ip, #-3947]
	mov	r0, r0, asr #8
	and	r3, r0, #255
	add	r3, r3, #256
	str	r3, [ip, #-3947]
	add	r0, r0, #768
	str	r0, [ip, #-3947]
	bx	lr
	.size	ymzwrite0, .-ymzwrite0
	.align	2
	.global	ymzwrite1
	.type	ymzwrite1, %function
ymzwrite1:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	mvn	r3, #-1073741824
	sub	r3, r3, #12288
	mov	r2, #0
	str	r2, [r3, #-3947]
	str	r1, [r3, #-3947]
	add	r1, r1, #512
	str	r1, [r3, #-3947]
	add	r2, r0, #256
	str	r2, [r3, #-3947]
	add	r0, r0, #768
	str	r0, [r3, #-3947]
	bx	lr
	.size	ymzwrite1, .-ymzwrite1
	.ident	"GCC: (Sourcery G++ Lite 2009q1-161) 4.3.3"
