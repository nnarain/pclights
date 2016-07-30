
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include <stdperiph/stdperiph.h>

#include <WS2812/Ws2812Driver.h>

#include "serial.h"

#define RED   0xFF0000
#define GREEN 0x00FF00
#define BLUE  0x0000FF

static const int BAUD = 9600;

GPIO(PORTB, DDRB, GpioB);
typedef stdperiph::GpioPinRef<GpioB, 4> indicator_callback;
typedef stdperiph::GpioPinRef<GpioB, 3> indicator_start;
typedef stdperiph::GpioPinRef<GpioB, 5> led;

//typedef Ws2812Driver<GpioB, 5, 60> LedsDriver;

/// VARS ----------------------------------------------------------------------
//static LedsDriver leds;

int main()
{

	return 0;
}
