/* Linker script to configure memory regions. */

SEARCH_DIR(.)
GROUP(-lgcc -lc -lnosys)

MEMORY
{
  FLASH (rx) : ORIGIN = 0x0, LENGTH = 0x1000
  RAM (rwx) :  ORIGIN = 0x20000000, LENGTH = 0x20020000-0x20000000
}

INCLUDE "nrf_common.ld"
