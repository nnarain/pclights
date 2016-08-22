#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <vector>

template<typename DataLinkT>
class Protocol
{
public:

	template<typename... Args>
	Protocol(Args&... args) :
		link_(args...)
	{
	}

	void setColor(uint8_t r, uint8_t g, uint8_t b)
	{
		uint8_t data[] = { 'P', 'L', 0, 0, 3, r, g, b };
		link_.write(data, 8);
	}

	void setPixel(uint8_t pixel, uint8_t r, uint8_t g, uint8_t b)
	{
		uint8_t data[] = { 'P', 'L', 1, 0, 4, pixel, r, g, b };
		link_.write(data, 9);
	}

	void setLevel(uint8_t level)
	{
		uint8_t data[] = { 'P', 'L', 2, 0, 1, level };
		link_.write(data, 6);
	}

private:
	DataLinkT link_;
};

#endif 