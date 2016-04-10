
#ifndef ASYNC_BUFFER_H
#define ASYNC_BUFFER_H

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>

#include <simplelogger/simplelogger.h>

#include <vector>

class AsyncBuffer
{

public:

	AsyncBuffer(std::size_t size);
	~AsyncBuffer();

	void beginRead();

	void read(uint8_t* data, size_t nbytes);

	size_t available();

	void setSerialPort(boost::asio::serial_port& serial_port);

private:
	void readHandler(const boost::system::error_code &error, std::size_t bytes_transferred);

private:
	boost::asio::serial_port * serial_;

	std::vector<uint8_t> buffer_;
	std::vector<uint8_t> read_buffer_;

	boost::mutex buffer_mutex_;

	const size_t size_;

	volatile size_t write_idx_;
	volatile size_t read_idx_;

	volatile size_t bytes_available_;	
};

#endif // ASYNC_BUFFER_H
