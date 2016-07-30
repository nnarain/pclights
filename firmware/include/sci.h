
#ifndef SCI_H
#define SCI_H

#include <avr/io.h>
#include <stdperiph/stdperiph.h>
#include <stddef.h>

#define PORT_ADDR(port) ((uint16_t)&(port))

#define DDRB_ADDR   0x04
#define PORTB_ADDR  0x05
#define UCSR0A_ADDR 0xC0
#define UCSR0B_ADDR 0xC1
#define UCSR0C_ADDR 0xC2
#define UBRR0H_ADDR 0xC5
#define UBRR0L_ADDR 0xC6

/**
	Hardware abstraction for serial communication interface

	Non-interrupt driven
*/
namespace sci
{
	/* Data Register Status */
	typedef stdperiph::BitRef<UCSR0A_ADDR, RXC0>  data_available;
	typedef stdperiph::BitRef<UCSR0A_ADDR, TXC0>  transmit_complete;
	typedef stdperiph::BitRef<UCSR0A_ADDR, UDRE0> data_register_empty;

	/* Communcation Enable */
	typedef stdperiph::BitRef<UCSR0B_ADDR, RXEN0> rx_enable;
	typedef stdperiph::BitRef<UCSR0B_ADDR, TXEN0> tx_enable;

	typedef stdperiph::BitRef<UCSR0B_ADDR, RXCIE0> rxi_enable;
	typedef stdperiph::BitRef<UCSR0B_ADDR, TXCIE0> txi_enable;

	/* Frame Format */
	typedef stdperiph::BitGroup<UCSR0C_ADDR, UCSZ00, UCSZ01-UCSZ00> frame_format;

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
	typedef stdperiph::BitRef<UCSR0C_ADDR, 3> stop_bits_1; ///< Set for 1 stop bit, clear 2 stop bits

	/* Baud */
	typedef stdperiph::CombinedRegister<UBRR0H_ADDR, UBRR0L_ADDR> baud;

	/* SCI Prototypes */

	void putc(char c);
	void puts(const char * str);
	char getc();
	void gets(char * str, size_t max);

	void write(uint8_t* data, size_t len);
	void read(uint8_t* data, size_t nbytes);
	size_t available();

	void setBaudPrescaler(uint32_t baud);
}

#endif
