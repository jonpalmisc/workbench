	.section ".text.boot"

	.extern ld_stack_ptr

	.global _start
_start:
	ldr     x30, =ld_stack_ptr
	mov     sp, x30

	bl      rs_main
