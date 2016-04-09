
#ifndef SCI_H
#define SCI_H

#include <avr/io.h>
#include <stdperiph/stdperiph.h>
#include <stddef.h>

#define PORT_ADDR(port) ((uint16_t)&(port))

/**
	Hardware abstraction for serial communication interface

	Non-interrupt driven
*/
namespace sci
{
	/* Data Register Status */
	typedef stdperiph::BitRef<PORT_ADDR(UCSR0A), RXC0>  data_available;
	typedef stdperiph::BitRef<PORT_ADDR(UCSR0A), TXC0>  transmit_complete;
	typedef stdperiph::BitRef<PORT_ADDR(UCSR0A), UDRE0> data_register_empty;

	/* Communcation Enable */
	typedef stdperiph::BitRef<PORT_ADDR(UCSR0B), RXEN0> rx_enable;
	typedef stdperiph::BitRef<PORT_ADDR(UCSR0B), TXEN0> tx_enable;

	/* Frame Format */
	typedef stdperiph::BitGroup<PORT_ADDR(UCSR0C), UCSZ00, UCSZ01-UCSZ00> frame_format;

	namespace frame_options
	{
		enum
		{
			DATA5 = 0,
			DATA6 = 1,
			DATA7 = 2,
			DATA8 = 3,
			DATA9 = 7
		};
	}

	/* Stop bits */
	typedef stdperiph::BitRef<PORT_ADDR(UCSR0C), 3> stop_bits_1; ///< Set for 1 stop bit, clear 2 stop bits

	/* Baud */
	typedef stdperiph::CombinedRegister<PORT_ADDR(UBRR0H), PORT_ADDR(UBRR0L)> baud;

	/* SCI Prototypes */

	void putc(char c);
	void puts(const char * str);
	char getc();
	void gets(char * str, size_t max);

	void write(uint8_t* data, size_t len);
	void read(uint8_t* data, size_t nbytes);
}

#endif
