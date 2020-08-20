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

// Width and height of LEDs
#ifndef microbit_sapanic_ROWS
#define microbit_sapanic_ROWS 5
#endif

#ifndef microbit_sapanic_COLS
#define microbit_sapanic_COLS 5
#endif

#ifndef microbit_sapanic_COLMAX
#define microbit_sapanic_COLMAX 4
#endif

// LED rows pinmap
#ifndef microbit_sapanic_PINROW0
#define microbit_sapanic_PINROW0 21
#endif

#ifndef microbit_sapanic_PINROW1
#define microbit_sapanic_PINROW1 22
#endif

#ifndef microbit_sapanic_PINROW2
#define microbit_sapanic_PINROW2 15
#endif

#ifndef microbit_sapanic_PINROW3
#define microbit_sapanic_PINROW3 24
#endif

#ifndef microbit_sapanic_PINROW4
#define microbit_sapanic_PINROW4 19
#endif

// LED columns pinmap
#ifndef microbit_sapanic_PINCOL0
#define microbit_sapanic_PINCOL0 28
#endif

#ifndef microbit_sapanic_PINCOL1
#define microbit_sapanic_PINCOL1 11
#endif

#ifndef microbit_sapanic_PINCOL2
#define microbit_sapanic_PINCOL2 31
#endif

#ifndef microbit_sapanic_PINCOL3
#define microbit_sapanic_PINCOL3 37
#endif

#ifndef microbit_sapanic_PINCOL4
#define microbit_sapanic_PINCOL4 30
#endif


// Pin output values for LED row off and on
#ifndef microbit_sapanic_ROWOFF
#define microbit_sapanic_ROWOFF  0
#endif

#ifndef microbit_sapanic_ROWON
#define microbit_sapanic_ROWON   1
#endif


// Pin output values for LED column off and on
#ifndef microbit_sapanic_COLOFF
#define microbit_sapanic_COLOFF  1
#endif

#ifndef microbit_sapanic_COLON
#define microbit_sapanic_COLON   0
#endif


// length of message: face, E, code digit, code digit, code digit
#ifndef microbit_sapanic_MSGLEN
#define microbit_sapanic_MSGLEN         4
#endif

// position of first code digit
#ifndef microbit_sapanic_MSG1STDIGIT
#define microbit_sapanic_MSG1STDIGIT    1
#endif

// divisor for first digit
#ifndef microbit_sapanic_DIVMAX
#define microbit_sapanic_DIVMAX         100
#endif

// divisor base
#ifndef microbit_sapanic_DIVBASE
#define microbit_sapanic_DIVBASE        10
#endif

// Number of burn passes for each character
#ifndef microbit_sapanic_PASSES
#define microbit_sapanic_PASSES         50
#endif

// Delay cycles for each row
#ifndef microbit_sapanic_ROWDELAY
#define microbit_sapanic_ROWDELAY       20000
#endif

// Delay cycles after each character
#ifndef microbit_sapanic_CHARDELAY
#define microbit_sapanic_CHARDELAY      1000000 // 100000000ul == 20s
#endif


static const uint8_t c_width    = microbit_sapanic_COLS;
static const uint8_t c_height   = microbit_sapanic_ROWS;

static const uint8_t c_rowPin[ microbit_sapanic_ROWS]  =
{
    microbit_sapanic_PINROW0,
    microbit_sapanic_PINROW1,
    microbit_sapanic_PINROW2,
    microbit_sapanic_PINROW3,
    microbit_sapanic_PINROW4
};

static const uint8_t c_colPin[ microbit_sapanic_COLS] =
{
    microbit_sapanic_PINCOL0,
    microbit_sapanic_PINCOL1,
    microbit_sapanic_PINCOL2,
    microbit_sapanic_PINCOL3,
    microbit_sapanic_PINCOL4
};

// Bitmap for a sad face
static const uint8_t c_face[ MICROBIT_FONT_WIDTH] = { 0x1B, 0x1B, 0x0, 0x0E, 0x11};


static inline const uint8_t *microbit_sapanic_msg_font_bytes( int code, int msgIdx)
{
    if ( msgIdx >= microbit_sapanic_MSG1STDIGIT)
    {
        int d;
        for ( d = microbit_sapanic_DIVMAX; msgIdx > microbit_sapanic_MSG1STDIGIT; msgIdx--)
            d /= microbit_sapanic_DIVBASE;
        return pendolino3_digits + MICROBIT_FONT_WIDTH * ( ( code / d) % microbit_sapanic_DIVBASE);
    }
    return c_face;
}


int microbit_sapanic_testColInRow( const uint8_t *bits, int col, int row)
{
    return bits[ row] & ( 1 << ( microbit_sapanic_COLMAX - col)) ? microbit_sapanic_COLON : microbit_sapanic_COLOFF;
}


void microbit_sapanic_wait( int cycles)
{
    for ( int i = 0; i < cycles; i++)
    {
        nrf_gpio_pin_out_read( c_rowPin[0]);
    }
}


static inline void microbit_sapanic_configure()
{
    for ( int i = 0; i < c_width; i++)
      nrf_gpio_cfg_output( c_colPin[i]);

    for ( int i = 0; i < c_height; i++)
      nrf_gpio_cfg_output( c_rowPin[i]);
}


void microbit_sapanic_clear()
{
    for ( int i = 0; i < c_width; i++)
      nrf_gpio_pin_write( c_colPin[i], microbit_sapanic_COLOFF);

    for ( int i = 0; i < c_height; i++)
      nrf_gpio_pin_write( c_rowPin[i], microbit_sapanic_ROWOFF);
}


static inline void microbit_sapanic_display( const uint8_t *bytes)
{
    for ( int pass = 0; pass < microbit_sapanic_PASSES; pass++)
    {
        for ( int row = 0; row < c_height; row++)
        {
            nrf_gpio_pin_write( c_rowPin[ row], microbit_sapanic_ROWON);
            
            for ( int col = 0; col < c_width; col++)
            {
                nrf_gpio_pin_write( c_colPin[ col], microbit_sapanic_testColInRow( bytes, col, row));
            }
            
            microbit_sapanic_wait( microbit_sapanic_ROWDELAY);
            microbit_sapanic_clear();
        }
    }
}


#if microbit_sapanic_ITERATIONS

void microbit_sapanic( int code, int iterations)
{
    microbit_sapanic_configure();
    microbit_sapanic_clear();
    
    while ( 1)
    {
        for ( int msgIdx = 0; msgIdx < microbit_sapanic_MSGLEN; msgIdx++)
        {
            microbit_sapanic_display( microbit_sapanic_msg_font_bytes( code, msgIdx));
            microbit_sapanic_wait( microbit_sapanic_CHARDELAY);
        }

        if ( iterations)
        {
            iterations--;
            if ( !iterations)
                break;
        }
    }
}

#else // microbit_sapanic_ITERATIONS

void microbit_sapanic( int code)
{
    microbit_sapanic_configure();
    microbit_sapanic_clear();
    
    while ( 1)
    {
        for ( int msgIdx = 0; msgIdx < microbit_sapanic_MSGLEN; msgIdx++)
        {
            microbit_sapanic_display( microbit_sapanic_msg_font_bytes( code, msgIdx));
            microbit_sapanic_wait( microbit_sapanic_CHARDELAY);
        }
    }
}

#endif // microbit_sapanic_ITERATIONS
