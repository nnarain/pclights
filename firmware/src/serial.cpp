#include "serial.h"

#define BAUD_RATE(baud) ((F_CPU / 16 / (baud))-1)

Serial::Serial(uint16_t baud) : baud_(baud)
{
}

Serial::~Serial()
{
}

void Serial::initialize()
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

int Serial::read()
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

void Serial::write(uint8_t * data, size_t len)
{
	sci::write(data, len);
}

