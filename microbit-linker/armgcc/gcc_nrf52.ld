/* Linker script to configure memory regions. */

SEARCH_DIR(.)
GROUP(-lgcc -lc -lnosys)

MEMORY
{
  FLASH (rx) : ORIGIN = 0x0, LENGTH = 0x1000
  RAM (rwx) :  ORIGIN = 0x20000000, LENGTH = 0x20020000-0x20000000
}

SECTIONS
{
  .saep_error_code_Segment : {KEEP(*(.saep_error_code_Section))} > RAM
}  INSERT AFTER .data;

INCLUDE "nrf_common.ld"
