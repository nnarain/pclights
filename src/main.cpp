
#include <iostream>
#include <string>

#include <boost/asio.hpp>

#include "serial.h"

int main(int argc, char * argv[])
{
	boost::asio::io_service io;

	std::string device_name = "COM4";
	unsigned int baud_rate = 9600;

	Serial comm_(io, device_name, baud_rate);
	uint8_t data[] = { 5 };

	std::cout << "-- sending a byte to the mcu" << std::endl;
	comm_.write(data, 1);

	std::cout << "-- finished writing to device" << std::endl;

    return 0;
}
