
#ifndef SERIAL_H
#define SERIAL_H

#include <stddef.h>

#include "sci.h"

#define BAUD_RATE(baud) ((F_CPU / 16 / (baud))-1)

class Serial
{
public:

	Serial(uint16_t baud) : baud_(baud)
	{
	}

	~Serial()
	{
	}

	void initialize()
	{
		// enable rx and tx
		sci::rx_enable::set();
		sci::tx_enable::set();

		// frame format
		sci::frame_format::write(sci::frame_options::DATA8);
		sci::stop_bits_1::set();

		// set baud rate
		sci::baud::write((uint16_t)BAUD_RATE(baud_));
	}

	int read()
	{
		if(sci::data_available::isSet())
		{
			return (int)sci::getc();
		}
		else
		{
			return -1;
		}
	}

	void write(uint8_t * data, size_t len)
	{
		sci::write(data, len);
	}

private:
	uint16_t baud_;
};

#endif
