OUTPUT_FORMAT("elf64-sparc")
ENTRY(start)

SECTIONS {
	.boot 0x4000: AT (0x4000) {
		*(BOOTSTRAP);
		*(.text);
		*(.rodata);
		*(.rodata.*);
		*(.data);               /* initialized data */
		*(.sdata);
		*(.sdata2);
		*(.sbss);
		*(.bss);                /* uninitialized static variables */
		*(COMMON);
		*(.kernel_image);
		*(.ns_image);
		*(.init_image);
		*(.loader_image);
		*(.devmap_image);
		*(.rd_image);
		*(.vfs_image);
		*(.fat_image);
		*(.initrd_image);

	}
	
	/DISCARD/ : {
		*(.comment);
		*(.note*);
	}
}
