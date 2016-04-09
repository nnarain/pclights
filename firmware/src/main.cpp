
#include <avr/io.h>
#include <util/delay.h>

#include <stdperiph/gpio_port.h>

#include <WS2812/Ws2812Driver.h>

#include "sci.h"

#define BAUD_RATE(baud) ((F_CPU / 16 / (baud))-1)

GPIO(PORTB, DDRB, GpioB);

typedef Ws2812Driver<GpioB, 5, 60> LedsDriver;

void initSci();

int main()
{
	LedsDriver leds;
	leds.begin();

	initSci();

	for(;;)
	{
		char c = sci::getc();
		sci::putc(c);
	}

	return 0;
}

void initSci()
{
	// enable rx and tx
	sci::rx_enable::set();
	sci::tx_enable::set();

	// frame format
	sci::frame_format::write(sci::frame_options::DATA8);
	sci::stop_bits_1::set();

	// set baud rate
	sci::baud::write((uint16_t)BAUD_RATE(9600));
}


