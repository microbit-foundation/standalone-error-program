#include "MicroBit.h"
#include "ram_led_error.h"

#include "nrf.h"
#include "hal/nrf_gpio.h"

MicroBit uBit;

void togglePixel( int x, int y);


void ramPanic( int error999)
{
  uBit.display.disable();
  target_disable_irq();
  for (int i=0; i<8; i++)
    NRF_GPIOTE->CONFIG[i] = 0;


  uint32_t *codePtr   = (uint32_t *) ram_led_error_address;
  uint8_t  *errorPtr  = (uint8_t  *) ram_led_error_address + ram_led_error_code;
  memcpy( codePtr, ram_led_error_data, ram_led_error_len);
  errorPtr[ 0] = error999 % 256;
  errorPtr[ 1] = error999 / 256;


  const uint32_t msp    = codePtr[0];
  const uint32_t reset  = codePtr[1];
  const uint32_t lr     = 0xFFFFFFFF;

  __set_CONTROL(0x00000000);
  __set_PRIMASK(0x00000000);
  __set_BASEPRI(0x00000000);
  __set_FAULTMASK(0x00000000);

  __ASM volatile ("MSR MSP, %[arg]" : : [arg] "r" (msp));
  __ASM volatile ("MOV LR,  %[arg]" : : [arg] "r" (lr) : "lr");
  __ASM volatile ("BX       %[arg]" : : [arg] "r" (reset));
}


void onButtonA(MicroBitEvent e)
{
  ramPanic(987);
}


void onButtonB(MicroBitEvent e)
{
  ramPanic(654);
}


void togglePixel( int x, int y)
{
    uBit.display.image.setPixelValue( x, y, uBit.display.image.getPixelValue( x, y) ? 0 : 255);
}


void forever()
{
    while (true)
    {
        togglePixel( 0, 0);
        uBit.sleep(1000);
    }
}


int  main() {
    uBit.init();

    uBit.messageBus.listen(MICROBIT_ID_BUTTON_A, MICROBIT_BUTTON_EVT_CLICK, onButtonA);
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_B, MICROBIT_BUTTON_EVT_CLICK, onButtonB);

    create_fiber( forever);

    release_fiber();
}
