OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
SEARCH_DIR(.)

ENTRY(__XC32_RESET_HANDLER_NAME)

#define ROM_START 0x0
#define ROM_SIZE  49152  /* Adjust size as needed */

#define RAM_START (0x20000000 + 16) /* Leave 16 bytes for bootloader trigger */
#define RAM_SIZE  (0x40000 - 16)

MEMORY
{
  rom (rx) : ORIGIN = ROM_START, LENGTH = ROM_SIZE
  ram (rwx) : ORIGIN = RAM_START, LENGTH = RAM_SIZE
}

SECTIONS
{
    /* Vector table at the beginning of RAM */
    .vectors :
    {
        . = ALIGN(4);
        KEEP(*(.vectors .vectors.*))
        _sfixed = .;
    } > ram AT > rom

    /* Place executable code in ROM */
    .text :
    {
        . = ALIGN(4);
        *(.text)
        *(.text.*)
        *(.glue_7t) *(.glue_7)
        *(.gnu.linkonce.r.*)
        *(.ARM.extab* .gnu.linkonce.armextab.*)
        . = ALIGN(4);
        _efixed = .;  /* End of text section */
    } > rom

    /* Place read-only data in ROM */
    .rodata :
    {
        . = ALIGN(4);
        *(.rodata)
        *(.rodata.*)
        . = ALIGN(4);
    } > rom

    /* .ARM.exidx section for exception tables */
    PROVIDE_HIDDEN (__exidx_start = .);
    .ARM.exidx :
    {
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
    } > rom
    PROVIDE_HIDDEN (__exidx_end = .);

    /* Initialized data in RAM, loaded from ROM */
    .data :
    {
        . = ALIGN(4);
        __data_start__ = .;
        _sdata = .;
        *(.data)
        *(.data.*)
        *(.dinit)
        . = ALIGN(4);
        __data_end__ = .;
        _edata = .;
    } > ram AT > rom

    /* Uninitialized data in RAM (cleared by startup code) */
    .bss (NOLOAD) :
    {
        . = ALIGN(4);
        __bss_start__ = .;
        _sbss = .;
        *(COMMON)
        *(.bss)
        *(.bss.*)
        . = ALIGN(4);
        __bss_end__ = .;
        _ebss = .;
    } > ram

    /* End of RAM */
    . = ALIGN(4);
    _end = .;
    _ram_end_ = ORIGIN(ram) + LENGTH(ram) - 1;
}
