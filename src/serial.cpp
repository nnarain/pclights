
#include "serial.h"

#include <simplelogger/simplelogger.h>

#include <boost/date_time/posix_time/posix_time_types.hpp>

Serial::Serial(boost::asio::io_service& io, std::string port, size_t baud) :
	serial_(io),
	port_(port),
	baud_(baud),
	buffer_(1024)
{
}

Serial::~Serial()
{
}

bool Serial::initialize()
{
	try
	{
		// open the serial port
		serial_.open(port_);

		// set options
		serial_.set_option(baud_);
		serial_.set_option(serial_port_base::character_size(8));
		serial_.set_option(serial_port_base::stop_bits(serial_port_base::stop_bits::one));
		serial_.set_option(serial_port_base::parity(serial_port_base::parity::none));
		serial_.set_option(serial_port_base::flow_control(serial_port_base::flow_control::none));

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

uint32_t Serial::time()
{
	boost::posix_time::ptime time = boost::posix_time::microsec_clock::local_time();
	boost::posix_time::time_duration duration(time.time_of_day());

	return duration.total_milliseconds();
}

int Serial::read()
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

void Serial::write(uint8_t* data, size_t len)
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

void Serial::writeHandler(const boost::system::system_error& error, std::size_t bytes_transferred)
{
	// nothing...
}
