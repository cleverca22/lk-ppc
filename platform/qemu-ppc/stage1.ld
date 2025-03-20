ENTRY(_start)

MEMORY {
  ram (rwx) : ORIGIN = 16M, LENGTH = 0x10000000
  load (rwx) : ORIGIN = 12M, LENGTH = 0x10000000
}

SECTIONS {
  .text : ALIGN(4) {
    KEEP(*(.text.boot));
    *(.text)
    *(.text.*)
    __end_text = .;
  } >ram AT>load =0

  .rodata : ALIGN(16) {
    *(.rodata)
    *(.rodata.*)
  } >ram AT>load

  lk_init : ALIGN(16) {
    *(lk_init)
  } >ram AT>load

  commands : ALIGN(16) {
    *(commands)
  } >ram AT>load

  apps : ALIGN(16) {
    *(apps)
  } >ram AT>load

  .toc : ALIGN(4) {
    *(.toc)
    *(.toc.*)
  } >ram AT>load

  .data : ALIGN(4) {
    *(.data)
    *(.data.*)
    __ctor_list = .;
    KEEP(*(.ctors .init_array))
    __ctor_end = .;
    __dtor_list = .;
    KEEP(*(.dtors .fini_array))
    __dtor_end = .;
  } >ram AT>load

  .sdata : ALIGN(4) {
    *(.sdata)
    *(.sdata.*)
  } >ram AT>load

  __bss_start = .;
  .bss : ALIGN(128) {
    *(.bss)
    *(.bss.*)
  } >ram

  .sbss : ALIGN(4) {
    *(.sbss)
    *(.sbss.*)
  } >ram
  __bss_end = .;

  .stack : ALIGN(64) {
    . += 8k;
    __stack_bottom = .;
  } >ram
  _end = .;

  /DISCARD/ : {
    *(.eh_frame .eh_frame.*)
  }
}
