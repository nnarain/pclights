#ifndef SERIAL_H
#define SERIAL_H

#include <boost/asio.hpp>

#include <string>

class Serial 
{
private:
	boost::asio::serial_port serial_;
	std::string device_name_;
	boost::asio::serial_port_base::baud_rate baud_;

public:
	Serial(boost::asio::io_service& io, const std::string& device_name, unsigned int baud);
	
	~Serial();

	void init();
	void write(uint8_t * buffer, size_t len);
};

#endif // !SERIAL_H

