
#ifndef ASYNC_BUFFER_H
#define ASYNC_BUFFER_H

#include <boost/asio.hpp>
#include <boost/bind.hpp>

class AsyncBuffer
{

public:

	AsyncBuffer(std::size_t size) : 
		serial_(nullptr),
		size_(size),
		write_idx_(0),
		read_buffer_(0),
		bytes_available_(0),
		read_handler_(this)
	{
		buffer_ = new uint8_t[size];
		read_buffer_ = new uint8_t[size];
	}

	~AsyncBuffer()
	{
		delete[] buffer_;
		delete[] read_buffer_;
	}

	void beginRead()
	{
		if(serial_ != nullptr)
			serial_->async_read_some(boost::asio::buffer(buffer_, size_), read_handler_);
	}

	void read(uint8_t* data, size_t len)
	{
		for(auto i = 0; i < len; ++i)
		{
			data[i] = buffer_[read_idx_];
			read_idx_ += (read_idx_ + 1) % size_;
		}
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
		if(!error)
		{
			// copy read bytes into the main buffer
			for(auto i = 0; i < bytes_transferred; ++i)
			{
				buffer_[write_idx_] = read_buffer_[i];
				write_idx_ += (write_idx_ + 1) % size_;
			}

			bytes_available_ = write_idx_ - read_idx_;
		}

		beginRead();
	}

private: 
	struct ReadHandler
	{
		AsyncBuffer* buffer;

		ReadHandler(AsyncBuffer* buffer) : buffer(buffer)
		{

		}

		void operator()(const boost::system::error_code& error, std::size_t bytes_transferred)
		{
			buffer->readHandler(error, bytes_transferred);
		}
	};

	friend ReadHandler;

private:
	boost::asio::serial_port * serial_;

	uint8_t* buffer_;
	uint8_t* read_buffer_;
	const size_t size_;

	size_t write_idx_;
	size_t read_idx_;

	size_t bytes_available_;

	ReadHandler read_handler_;
};

#endif // ASYNC_BUFFER_H
