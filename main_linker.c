/*
    MIT License

    Copyright (c) 2020 Micro:bit Educational Foundation

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/

#include <stdint.h>

#include "nrf.h"

#include "MicroBitSAPanic.h"


// Location of error code is:
// bin: last 4 bytes is error_code
// hex: last 4 bytes of last data record

// Linker script has
// SECTIONS
// {
//   .saep_error_code_Segment : {KEEP(*(.saep_error_code_Section))} > RAM
// }  INSERT AFTER .data;


int error_code __attribute__((section(".saep_error_code_Section"))) = 70;

int main(void)
{
    microbit_sapanic( error_code); // will never return
}
