
/**
	PC Lights MCU firmware

	@author Natesh Narain <nnaraindev@gmail.com>
	@date August 6 2016
*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "io_config.h"
#include "serial.h"

#define BAUD_RATE 9600


int main()
{
	led::mode(stdperiph::BitMode::OUTPUT);
	led::low();
	
	Serial serial(BAUD_RATE);
	serial.initialize();

	sei();
	
	for(;;)
	{
		if(sci::available() > 0)
		{
			led::high();
		}
	}
	
	return 0;
}


