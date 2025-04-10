/*
 * Set the architecture to mips.
 */
OUTPUT_ARCH(mips)

/*
 * Set the ENTRY point of the program to _start.
 */
ENTRY(_start)

SECTIONS {
	/* ----- MOS EXERCISE 3 exc-entry-lds AFTER exc-entry BEGIN ----- */
	/* ----- MOS BLANK BEGIN ----- */
	. = 0x80000000;
	.tlb_miss_entry : {
		*(.text.tlb_miss_entry)
	}

	. = 0x80000180;
	.exc_gen_entry : {
		*(.text.exc_gen_entry)
	}
	/* ----- MOS BLANK END ----- */
	/* ----- MOS EXERCISE END ----- */

	/* ----- MOS EXERCISE 1 basic-lds AFTER readelf BEGIN ----- */
	/* fill in the correct address of the key sections: text, data, bss. */
	/* Hint: The loading address can be found in the memory layout. And the data section
	 *       and bss section are right after the text section, so you can just define
	 *       them after the text section.
	 */
	/* Step 1: Set the loading address of the text section to the location counter ".". */
	/* ----- MOS BLANK BEGIN ----- */
	. = 0x80020000;
	/* ----- MOS BLANK END ----- */

	/* Step 2: Define the text section. */
	/* ----- MOS BLANK BEGIN ----- */
	.text : {
		*(.text)
	}
	/* ----- MOS BLANK END ----- */

	/* Step 3: Define the data section. */
	/* ----- MOS BLANK BEGIN ----- */
	.data : {
		*(.data)
	}
	/* ----- MOS BLANK END ----- */

	bss_start = .;
	/* Step 4: Define the bss section. */
	/* ----- MOS BLANK BEGIN ----- */
	.bss  : {
		*(.bss)
	}
	/* ----- MOS BLANK END ----- */
	/* ----- MOS EXERCISE END ----- */

	bss_end = .;
	. = 0x80800000;
	end = . ;
}
