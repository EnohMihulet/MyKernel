ENTRY(_start)

SECTIONS {

	. = 0x40200000;

	.text : ALIGN(0x1000) {
		*(.text._start*)
		*(.text*)
		*(.vectors*)
	}

	.rodata : ALIGN(0x1000) {
		*(.rodata*)
	}

	.data : ALIGN(0x1000) {
		*(.data*)
	}

	.bss : ALIGN(0x1000) {
		__bss_start = .;
		*(.bss*) *(COMMON)
		__bss_end = .;
		
	}

	. = ALIGN(16);
	__stack_bottom = .;
	. = . + 0x4000;
	. = ALIGN(16);
	__stack_top = .;
}

