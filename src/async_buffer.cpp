#include "async_buffer.h"


AsyncBuffer::AsyncBuffer(std::size_t size) : 
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

AsyncBuffer::~AsyncBuffer()
{
}

void AsyncBuffer::beginRead()
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

void AsyncBuffer::read(uint8_t* data, size_t nbytes)
{
	boost::mutex::scoped_lock lock(buffer_mutex_);

	for(auto i = 0; i < nbytes; ++i)
	{
		data[i] = buffer_[read_idx_];
		read_idx_ = (read_idx_ + 1) % size_;
	}

	bytes_available_ -= nbytes;
}

size_t AsyncBuffer::available()
{
	return bytes_available_;
}

void AsyncBuffer::setSerialPort(boost::asio::serial_port& serial_port)
{
	serial_ = &serial_port;
}


void AsyncBuffer::readHandler(const boost::system::error_code &error, std::size_t bytes_transferred)
{
	boost::mutex::scoped_lock lock(buffer_mutex_);

	if(!error)
	{
		// copy read bytes into the main buffer
		for(auto i = 0; i < bytes_transferred; ++i)
		{
			buffer_[write_idx_] = read_buffer_[i];
			write_idx_ = (write_idx_ + 1) % size_;
		}

		bytes_available_ +=  bytes_transferred;
	}

	beginRead();
}