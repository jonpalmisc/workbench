ENTRY(_start)

SECTIONS
{
	. = 0x40000000;		/* RAM base for QEMU 'virt' board */

	.text : ALIGN(4K) {
		*(.text.boot)	/* Boot section */
		*(.text)	/* Kernel section */
		*(.text.*)	/* Additional code sections */
	}

	.rodata : ALIGN(4K) {
		*(.rodata.*)
	}

	.data : ALIGN(4K) {
		*(.data.*)
	}

	.bss : ALIGN(4K) {
		*(.bss COMMON)
	}

	. = ALIGN(16);		/* Align stack to 16 bytes */
	. += 0x1000;		/* Use a stack size of 4 kB */
	stack_top = .;
}
