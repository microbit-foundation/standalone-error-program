/* Linker script to configure memory regions. */

SEARCH_DIR(.)
GROUP(-lgcc -lc -lnosys)

MEMORY
{
  FLASH (rx) : ORIGIN = 0x20010000, LENGTH = 0x20011000-0x20010000
  RAM (rwx) :  ORIGIN = 0x20011000, LENGTH = 0x20012000-0x20011000
}

INCLUDE "nrf_common.ld"
