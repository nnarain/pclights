
#ifndef SERIAL_H
#define SERIAL_H

#include "async_buffer.h"

#include <simplelogger/simplelogger.h>

#include <boost/asio.hpp>

#include <string>

class Serial
{
public:

	Serial(boost::asio::io_service& io, std::string port, size_t baud) :
		serial_(io),
		port_(port),
		baud_(baud),
		buffer_(1024)
	{
	}

	~Serial()
	{
	}

	bool initialize()
	{
		try
		{
			// open the serial port
			serial_.open(port_);

			// set options
			serial_.set_option(baud_);

			// start reading from serial port
			buffer_.setSerialPort(serial_);
			buffer_.beginRead();
		}
		catch(boost::system::system_error& e)
		{
			LOG_ERROR("Serial init failed", e.what());
			LOG_INFO("Do you have the correct permissions?");

			return false;
		}

		return true;
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
		serial_.async_write_some(
			boost::asio::buffer(data, len),
		 	boost::bind(
		 		&Serial::writeHandler, this,
		 		boost::asio::placeholders::error, 
		 		boost::asio::placeholders::bytes_transferred
		 	)
		);
	}

private:
	void writeHandler(const boost::system::system_error& error, std::size_t bytes_transferred)
	{
		// nothing...
	}

	boost::asio::serial_port serial_;
	std::string port_;
	boost::asio::serial_port_base::baud_rate baud_;

	AsyncBuffer buffer_;

};

#endif // SERIAL_H
