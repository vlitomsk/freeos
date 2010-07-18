OUTPUT_FORMAT("elf32-i386")
ENTRY(start)
phys = 0x00100000;
SECTIONS
{
   . = phys;
   .__mbHeader : AT ( ADDR( .__mbHeader ) ) {
      mboot = .;
      code = .;
      *(.__mbHeader)
   }
  .text : AT(ADDR(.text)) {
    *(.text)
    *(.rodata)
    . = ALIGN(4096);
  }
  .data : AT(ADDR(.text) + (data - code))
  {
    data = .;
    *(.data)
    . = ALIGN(4096);
  }
  .bss : AT(ADDR(.text) + (bss - code))
  {
    bss = .;
    *(.bss)
    . = ALIGN(4096);
  }
  end = .;
}
