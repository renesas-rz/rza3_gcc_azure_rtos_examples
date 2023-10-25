/*
                  Linker File for Renesas FSP
*/

/* Memory allocation example using sdram. */
INCLUDE memory_regions.ld

SRAM_RESERVED_LENGTH = 0xE000;
SRAM_MMU_START = SRAM_START + SRAM_RESERVED_LENGTH;
SRAM_MMU_LENGTH = 0x5000;
SRAM_CODE_START = SRAM_MMU_START + SRAM_MMU_LENGTH;
SRAM_CODE_LENGTH = (SRAM_START + SRAM_LENGTH) - SRAM_CODE_START;

FSP_DDR_OFFSET = 0x001FFFC0;
FSP_HEADER_LENGTH = 0x200;
FSP_HEADER_START = DDR_START + FSP_DDR_OFFSET - FSP_HEADER_LENGTH;

ENTRY_S_START = DDR_START + FSP_DDR_OFFSET;
ENTRY_S_LENGTH = 0x1000;
CACHEDRAM_S_START = ENTRY_S_START + ENTRY_S_LENGTH;
CACHEDRAM_S_LENGTH = DDR_LENGTH - (CACHEDRAM_S_START - DDR_START);

UNCACHED_OFFSET = 0x40000000;
UNCACHEDRAM_S_START = UNCACHED_OFFSET + DDR_START;
UNCACHEDRAM_S_LENGTH = DDR_LENGTH;

/* Define memory regions. */
MEMORY {
    SRAM_MMU          (rw)  : ORIGIN = SRAM_MMU_START,      LENGTH = SRAM_MMU_LENGTH
    SRAM              (rwx) : ORIGIN = SRAM_CODE_START,     LENGTH = SRAM_CODE_LENGTH
    HEADER_REGION     (rx)  : ORIGIN = FSP_HEADER_START,    LENGTH = FSP_HEADER_LENGTH
    ENTRY_S           (rx)  : ORIGIN = ENTRY_S_START,       LENGTH = ENTRY_S_LENGTH
    CACHED_RAM_S      (rwx) : ORIGIN = CACHEDRAM_S_START,   LENGTH = CACHEDRAM_S_LENGTH
    UNCACHED_RAM_S    (rwx) : ORIGIN = UNCACHEDRAM_S_START, LENGTH = UNCACHEDRAM_S_LENGTH
}

ENTRY(Start)
SECTIONS
{
    .data.sram_mmu : {
        __ttbr0_base = .;
        . += 0x00000020;
        . = ALIGN( 0x1000 );
        __level2_table_1_base = .;
        . += 0x00001000;
        __level2_table_2_base = .;
        . += 0x00001000;
        __level2_table_3_base = .;
        . += 0x00001000;
        __level2_table_4_base = .;
        . += 0x00001000;
    } > SRAM_MMU

    .text.sram : {
        __sram_exec_start = .;
        __sram_load_start = LOADADDR(.text.sram) + ( . - ADDR(.text.sram) );

        KEEP(* (VECTOR_TABLE))          /*asm*/
        KEEP(* (VECTOR_TABLE_FREERTOS)) /*asm*/

        . = ALIGN( 0x8 );

        KEEP(* (IRQ_FIQ_HANDLER))       /*asm*/

        * (FASTCODE)

        . = ALIGN( 0x8 );
        __sram_load_end = LOADADDR(.text.sram) + ( . - ADDR(.text.sram) );
    } > SRAM AT > CACHED_RAM_S

    .rodata.header : {
        QUAD(Start)
        QUAD(0xFFFFFFFFFFFFFFFF-Start)
        QUAD(_prog_start)
        QUAD(0xFFFFFFFFFFFFFFFF-_prog_start)
        QUAD(_payload_size)
        QUAD(0xFFFFFFFFFFFFFFFF-_payload_size)
        FILL(0x00)
        . += 0x1B0;
        QUAD(0x4120505346205a52)
        QUAD(0x69746163696c7070)
        QUAD(0x0000000000006e6f)
        QUAD(0x0000000000000000)
    } > HEADER_REGION

    .text.secure_program_entry : {
        _prog_start = .;
        KEEP(* (PROGRAM_ENTRY))     /*asm*/
    } > ENTRY_S

    .text : ALIGN( 0x8 ) {
        KEEP(* (RESET_HANDLER))     /*asm*/
        . = ALIGN( 0x80 );
        * (INIT_SECTION)            /*asm*/
        "*/sections.o"       (.rodata .rodata.*)

        . = ALIGN( 0x8 );

        * (.text .text.*)
        * (.rodata .rodata.*)

        . = ALIGN( 0x8 );
        /* preinit data */
        PROVIDE_HIDDEN(__preinit_array_start = .);
        KEEP(*(.preinit_array))
        PROVIDE_HIDDEN(__preinit_array_end = .);
        . = ALIGN( 0x8 );
        /* init data */
        PROVIDE_HIDDEN(__init_array_start = .);
        KEEP(*(SORT(.init_array.*)))
        KEEP(*(.init_array))
        PROVIDE_HIDDEN(__init_array_end = .);
        . = ALIGN( 0x8 );
        /* finit data */
        PROVIDE_HIDDEN(__fini_array_start = .);
        KEEP(*(SORT(.fini_array.*)))
        KEEP(*(.fini_array))
        PROVIDE_HIDDEN(__fini_array_end = .);

    } > CACHED_RAM_S

    .ARM.extab :
    {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } > CACHED_RAM_S

    .ARM.exidx :
    {
        __exidx_start = .;
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
        __exidx_end = .;
    } > CACHED_RAM_S

    .data : ALIGN( 0x20 ) {

        * (.data .data.*)
        * (.application_vectors)

        . = ALIGN( 0x8 );
        _data_end = .;
    } > CACHED_RAM_S

    _payload_size = _data_end - _prog_start;

    .bss (NOLOAD) : ALIGN( 0x20 ) {
        __bss_start = .;
        . = ALIGN( 0x8 );
        __bss_end = .;

    } > CACHED_RAM_S

    .stack (NOLOAD) : ALIGN( 0x10 ) {
        __stack_start = .;
        . += 0x00002000;
        . = ALIGN( 0x10 );
        __stack_end = .;

    } > CACHED_RAM_S

    .bss2 (NOLOAD) : ALIGN( 0x20 ) {
        __bss2_start = .;

        * (.bss .bss.*)
        * (COMMON)

        . = ALIGN( 0x8 );
        __bss2_end = .;
    } > CACHED_RAM_S

    .heap (NOLOAD) : ALIGN( 0x10 ) {
        end = .;
        __HeapBase = .;
        . = . + 0x00002000;
        __HeapLimit = .;
        /* heap end */

        _cached_end = .;
    } > CACHED_RAM_S

    .bss.uncached_RAM (NOLOAD) : ALIGN( 0x20 ) {
        /* Skip used size in cached area */
        . = UNCACHEDRAM_S_START + (_cached_end - DDR_START);

        __uncached_RAM_start = .;

        * (UNCACHED_BSS)

        . = ALIGN( 0x8 );
        __uncached_RAM_end = .;
    } > UNCACHED_RAM_S

    .uncached_RAM_USB (NOLOAD) : ALIGN( 0x8 ) {
        __uncached_RAM_USB_start = .;
        KEEP(*(.uncached_RAM_USB*))
        __uncached_RAM_USB_end = .;
    } > UNCACHED_RAM_S

    /*
     * The startup code uses the end of this region to calculate
     * the top of memory - don't place any RAM regions after it
     */
    __top_of_ram = .;
}
/*EOF*/
