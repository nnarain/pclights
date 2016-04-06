
#ifndef SERIAL_H
#define SERIAL_H

#include "async_buffer.h"

#include <boost/asio.hpp>

#include <string>

class Serial
{
public:

	Serial(boost::asio::io_service& io, std::string port, size_t baud) :
		serial_(io),
		port_(port),
		baud_(baud),
		buffer_(512)
	{
	}

	~Serial()
	{
	}

	void initialize()
	{
		// set options
		serial_.set_option(baud_);

		// open the serial port
		serial_.open(port_);

		// start reading from serial port
		buffer_.setSerialPort(serial_);
	}

	int read()
	{
		if(buffer_.available() > 0)
		{
			uint8_t data;
			buffer_.read(&data, 1);

			return (int)data;
		}
		else
		{
			return -1;
		}
	}

	void write(uint8_t* data, size_t len)
	{
		boost::asio::write(serial_, boost::asio::buffer(data, len));
	}

private:
	boost::asio::serial_port serial_;
	std::string port_;
	boost::asio::serial_port_base::baud_rate baud_;

	AsyncBuffer buffer_;

};

#endif // SERIAL_H
