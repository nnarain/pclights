
#include "sci.h"
#include <avr/interrupt.h>

#ifndef SERIAL_BUFFER_SIZE
#define SERIAL_BUFFER_SIZE 64
#endif

static uint8_t serial_buffer[SERIAL_BUFFER_SIZE];
static size_t read_idx = 0;
static size_t write_idx = 0;
static size_t bytes_available = 0;

static inline size_t next(size_t idx);

namespace sci
{
	void putc(char c)
	{
		data_register_empty::waitUntilSet();
		UDR0 = c;
		transmit_complete::waitUntilSet();
	}

	void puts(const char * str)
	{
		while(*str)
		{
			putc(*str++);
		}
	}

	char getc()
	{
		char c = (char)serial_buffer[write_idx];
		write_idx = next(write_idx);
		bytes_available--;

		return c;
	}

	void gets(char * str, size_t max)
	{
		size_t read_count = 0;
		while(read_count < max)
		{
			char c = getc();
			*str = c;
			str++;

			if(!c) break;
		}
	}

	void write(uint8_t* data, size_t len)
	{
		while(len--)
		{
			putc((char)*data++);
		}
	}

	void read(uint8_t* data, size_t nbytes)
	{
		while(nbytes--)
		{
			*data++ = (uint8_t)getc();
		}
	}

	size_t available()
	{
		return bytes_available;
	}
}

ISR(USART_RX_vect)
{
	serial_buffer[read_idx] = UDR0;
	read_idx = next(read_idx);
	bytes_available++;
}

static inline size_t next(size_t idx)
{
	return (idx + 1) % SERIAL_BUFFER_SIZE;
}