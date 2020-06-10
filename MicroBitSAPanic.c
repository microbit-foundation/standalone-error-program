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

#include "nrf_gpio.h"

#include "MicroBitSAPanic.h"
#include "MicroBitSAPanicFont.h"


static const unsigned char c_width = 5;
static const unsigned char c_height = 5;
static const unsigned char c_row[5] = { 21, 22, 15, 24, 19 };
static const unsigned char c_col[5] = { 28, 11, 31, 37, 30 };

static const uint8_t c_face[5] = { 0x1B, 0x1B, 0x0, 0x0E, 0x11};


const uint8_t *microbit_sapanic_msg_font_bytes( int code, int msgIdx)
{
    if ( msgIdx > 1)
    {
        int d;
        for ( d = 100; msgIdx > 2; msgIdx--)
            d /= 10;
        return pendolino3_digits + MICROBIT_FONT_WIDTH * ( ( code / d) % 10);
    }
    return msgIdx ? pendolino3_E : c_face;
}


int microbit_sapanic_testColInRow( const uint8_t *bits, int col, int row)
{
    return bits[ row] & ( 1 << ( 4 - col)) ? 0 : 1;
}


void microbit_sapanic_wait( int cycles)
{
    for ( int i = 0; i < cycles; i++)
    {
        nrf_gpio_pin_out_read( c_row[0]);
    }
}


void microbit_sapanic_configure()
{
    for ( int i = 0; i < c_width; i++)
      nrf_gpio_cfg_output( c_col[i]);

    for ( int i = 0; i < c_height; i++)
      nrf_gpio_cfg_output( c_row[i]);
}


void microbit_sapanic_clear()
{
    for ( int i = 0; i < c_width; i++)
      nrf_gpio_pin_write( c_col[i], 1);

    for ( int i = 0; i < c_height; i++)
      nrf_gpio_pin_write( c_row[i], 0);
}


void microbit_sapanic_display( const uint8_t *bytes)
{
    for ( int pass = 0; pass < 50; pass++)
    {
        for ( int row = 0; row < c_height; row++)
        {
            nrf_gpio_pin_write( c_row[ row], 1);
            
            for ( int col = 0; col < c_width; col++)
            {
                nrf_gpio_pin_write( c_col[ col], microbit_sapanic_testColInRow( bytes, col, row));
            }
            
            microbit_sapanic_wait( 20000);
            microbit_sapanic_clear();
        }
    }
}


void microbit_sapanic( int code, int iterations)
{
    microbit_sapanic_configure();
    microbit_sapanic_clear();
    
    while ( 1)
    {
        for ( int msgIdx = 0; msgIdx < 5; msgIdx++)
        {
            microbit_sapanic_display( microbit_sapanic_msg_font_bytes( code, msgIdx));
            microbit_sapanic_wait( 10000);
        }

        if ( iterations)
        {
            iterations--;
            if ( !iterations)
                break;
        }
    }
}
