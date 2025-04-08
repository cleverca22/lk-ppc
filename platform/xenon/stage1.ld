ENTRY(_start)

MEMORY {
  ram (rwx) : ORIGIN = 0x10000000, LENGTH = 0x1E000000
}

SECTIONS {
  .text : ALIGN(4) {
    KEEP(*(.text.boot));
    *(.text)
    *(.text.*)
  } >ram =0

  .rodata : ALIGN(4) {
    *(.rodata)
    *(.rodata.*)
  } >ram

  .data : ALIGN(4) {
    *(.data)
    *(.data.*)
    __ctor_list = .;
    KEEP(*(.ctors .init_array))
    __ctor_end = .;
    __dtor_list = .;
    KEEP(*(.dtors .fini_array))
    __dtor_end = .;
  } >ram

  .sdata : ALIGN(4) {
    *(.sdata)
    *(.sdata.*)
  } >ram

  __bss_start = .;
  .bss : ALIGN(4) {
    *(.bss)
    *(.bss.*)
  } >ram

  .sbss : ALIGN(4) {
    *(.sbss)
    *(.sbss.*)
  } >ram
  __bss_end = .;

  .stack : ALIGN(4) {
    . += 8k;
    __stack_bottom = .;
  } >ram
  _end = .;
}
