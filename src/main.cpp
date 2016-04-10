
#include "serial.h"

#include <iostream>
#include <string>

#include <simplelogger/simplelogger.h>
#include <simplelogger/logger/console_logger.h>

#include <serialmessages/message_server.h>

#include <boost/asio.hpp>
#include <boost/program_options.hpp>
#include <boost/thread.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/bind.hpp>

using namespace simplelogger;
using namespace serialmessages;

namespace options = boost::program_options;

int main(int argc, char * argv[])
{
	registerLogger<ConsoleLogger>();

	// setup options
	options::variables_map vm;

	std::string port;
	size_t baud;

	try
	{
		options::options_description desc("Options");
		desc.add_options()
			("help", "see options")
			("port", options::value<std::string>()->required(), "serial port ('/dev/ttyUSB0', 'COM3', etc)")
			("baud", options::value<size_t>()->required(),      "serial baud rate");

		options::store(options::parse_command_line(argc, argv, desc), vm);

		if(vm.count("help") || argc == 1)
		{
			std::cout << desc << std::endl;
			return 1;
		}

		options::notify(vm);
	}
	catch(options::error& e)
	{
		std::cout << e.what() << std::endl;
		std::cout << "--help to see options" << std::endl;
		return 1;
	}

	port = vm["port"].as<std::string>();
	baud = vm["baud"].as<size_t>();

	//
	
	boost::asio::io_service io_service;

	MessageServer<Serial> server(io_service, port, baud);

	LOG_INFO("Initializing message server on port %s", port.c_str());
	if(!server.initialize())
	{
		LOG_ERROR("Failed to initialize message server");
		return 1;
	}

	LOG_INFO("Starting IO Service Thread");
	boost::thread io_thread(boost::bind(&boost::asio::io_service::run, &io_service));

	while(true)
	{
		server.spinOnce();
	}

	io_thread.join();

	releaseLoggers();
	return 0;
}
