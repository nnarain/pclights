
#ifndef SERIAL_H
#define SERIAL_H

#include "async_buffer.h"

#include <boost/asio.hpp>

#include <string>

class Serial
{
public:

	Serial(boost::asio::io_service& io, std::string port, size_t baud);
	~Serial();

	bool initialize();

	int read();

	void write(uint8_t* data, size_t len);

private:
	void writeHandler(const boost::system::system_error& error, std::size_t bytes_transferred);

private:

	typedef boost::asio::serial_port_base serial_port_base;

	boost::asio::serial_port serial_;
	std::string port_;
	boost::asio::serial_port_base::baud_rate baud_;

	AsyncBuffer buffer_;

};

#endif // SERIAL_H
