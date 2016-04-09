
#if 1
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

#endif

#if 0

#include <avr/io.h>
#include "sci.h"
 
#define USART_BAUDRATE 9600
#define BAUD_PRESCALE (((F_CPU/(USART_BAUDRATE*16UL)))-1)
 
int main(void)
{
	char recieved_byte;

//	UCSR0B |= (1<<RXEN0)  | (1<<TXEN0);
	sci::rx_enable::set();
	sci::tx_enable::set();
//	UCSR0C |= (1<<UCSZ00) | (1<<UCSZ01);
	sci::frame_format::write(sci::frame_options::DATA8);
	sci::stop_bits_1::set();
/*	UBRR0H  = (BAUD_PRESCALE >> 8);
	UBRR0L  = BAUD_PRESCALE;*/
	sci::baud::write((uint16_t)BAUD_RATE(9600));

	for(;;)
	{
		// wait until a byte is ready to read
	//	while( ( UCSR0A & ( 1 << RXC0 ) ) == 0 ){}

		// grab the byte from the serial port
	//	recieved_byte = UDR0;
		recieved_byte = sci::getc();

		// wait until the port is ready to be written to
	//	while( ( UCSR0A & ( 1 << UDRE0 ) ) == 0 ){}

		// write the byte to the serial port
	//	UDR0 = recieved_byte;
		sci::putc(recieved_byte);
	}
	return 0;   /* never reached */
}

#endif