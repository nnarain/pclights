
#include "sci.h"

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
		data_available::waitUntilSet();
		return UDR0;
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
		for(size_t bytes_read = 0; bytes_read <= nbytes; ++bytes_read)
		{
			*data = getc();
			data++;
		}
	}
}
