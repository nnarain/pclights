
#include "serial.h"

#ifdef _WIN32
#	include <windows.h>
#endif

#include <exception>

Serial::Serial(boost::asio::io_service& io, const std::string& device_name, unsigned int baud) :
	serial_(io),
	device_name_(device_name),
	baud_(baud)
{
	serial_.open(device_name_);
	serial_.set_option(baud_);
	serial_.set_option(boost::asio::serial_port_base::character_size(8));
	serial_.set_option(boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one));
	serial_.set_option(boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::none));
}

void Serial::init()
{
}

void Serial::write(uint8_t * data, size_t len)
{
	boost::asio::write(
		serial_,
		boost::asio::buffer(data, len)
	);
}

Serial::~Serial() 
{
	serial_.close();
}
