ENTRY(_start)

SECTIONS {
	. = 0x40080000;

	.text.boot : { *(.text.boot) }

	.text : { *(.text.*) }
	.data : { *(.data.*) }
	.rodata : { *(.rodata.*) }
	.bss : { *(.bss.*) }

	. = ALIGN(8);
	. = . + 0x8000;

	ld_stack_ptr = .;
}
