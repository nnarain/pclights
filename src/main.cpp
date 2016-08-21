
#include <iostream>
#include <string>
#include <thread>
#include <chrono>

#include <boost/asio.hpp>

#include "serial.h"

void delay(unsigned int ms)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

void clear(Serial& comm)
{
	uint8_t data[] = { 'P', 'L', 0, 0, 0 };
	comm.write(data, 5);
}

void setColor(Serial& comm, uint8_t r, uint8_t g, uint8_t b)
{
	uint8_t data[] = { 'P', 'L', 1, 0, 3, r, g, b };
	comm.write(data, 8);
}

int main(int argc, char * argv[])
{
	boost::asio::io_service io;

	std::string device_name = "COM4";
	unsigned int baud_rate = 9600;

	Serial comm(io, device_name, baud_rate);

	clear(comm);
	delay(1000);
	setColor(comm, 255, 0, 0);
	delay(1000);
	clear(comm);
	delay(1000);
	setColor(comm, 0, 255, 0);


    return 0;
}
