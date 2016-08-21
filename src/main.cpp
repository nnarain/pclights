
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

void setColor(Serial& comm, uint8_t r, uint8_t g, uint8_t b)
{
	uint8_t data[] = { 'P', 'L', 1, 0, 3, r, g, b };
	comm.write(data, 8);
}

void clear(Serial& comm)
{
	setColor(comm, 0, 0, 0);
}

void setPixel(Serial& comm, uint8_t pixel, uint8_t r, uint8_t g, uint8_t b)
{
	uint8_t data[] = { 'P', 'L', 2, 0, 4, pixel, r, g, b };
	comm.write(data, 8);
}

void setLevel(Serial& comm, uint8_t level)
{
	uint8_t data[] = { 'P', 'L', 3, 0, 1, level };
	comm.write(data, 6);
}

int main(int argc, char * argv[])
{
	boost::asio::io_service io;

	std::string device_name = "COM4";
	unsigned int baud_rate = 9600;

	Serial comm(io, device_name, baud_rate);

	clear(comm);
	setColor(comm, 255, 255, 0);



    return 0;
}
