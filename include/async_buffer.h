
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

	AsyncBuffer(std::size_t size) : 
		serial_(nullptr),
		size_(size),
		write_idx_(0),
		read_idx_(0),
		read_buffer_(0),
		bytes_available_(0)
	{
		buffer_.resize(size);
		read_buffer_.resize(size);
	}

	~AsyncBuffer()
	{
	}

	void beginRead()
	{
		if(serial_ != nullptr)
		{
			serial_->async_read_some(
				boost::asio::buffer(&read_buffer_[0], size_),
				boost::bind(
					&AsyncBuffer::readHandler, this,
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred
				)
			);
		}
	}

	void read(uint8_t* data, size_t nbytes)
	{
		boost::mutex::scoped_lock lock(buffer_mutex_);

		for(auto i = 0; i < nbytes; ++i)
		{
/*			if(buffer_[read_idx_] != 0)
				LOG_DEBUG("R: %c", buffer_[read_idx_]);*/
			data[i] = buffer_[read_idx_];
			read_idx_ = (read_idx_ + 1) % size_;
		}

		bytes_available_ -= nbytes;
	}

	size_t available()
	{
		return bytes_available_;
	}

	void setSerialPort(boost::asio::serial_port& serial_port)
	{
		serial_ = &serial_port;
	}

private:
	void readHandler(const boost::system::error_code &error, std::size_t bytes_transferred)
	{
		static int count = 0;
		boost::mutex::scoped_lock lock(buffer_mutex_);

		if(!error)
		{
			// copy read bytes into the main buffer
			for(auto i = 0; i < bytes_transferred; ++i)
			{
/*				if(read_buffer_[i] != 0)
					LOG_DEBUG("W: %c", read_buffer_[i]);*/
				buffer_[write_idx_] = read_buffer_[i];
				write_idx_ = (write_idx_ + 1) % size_;
			}

			bytes_available_ +=  bytes_transferred;
		}

		count++;
		if(count < 20)
			beginRead();
	}

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
