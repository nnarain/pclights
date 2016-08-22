
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <sstream>
#include <cstdint>

#include <boost/asio.hpp>
#include <boost/program_options.hpp>
#include <boost/variant.hpp>

#include "serial.h"

namespace po = boost::program_options;

void delay(unsigned int ms)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

void setColor(Serial& comm, uint8_t r, uint8_t g, uint8_t b)
{
	uint8_t data[] = { 'P', 'L', 0, 0, 3, r, g, b };
	comm.write(data, 8);
}

void clear(Serial& comm)
{
	setColor(comm, 0, 0, 0);
}

void setPixel(Serial& comm, uint8_t pixel, uint8_t r, uint8_t g, uint8_t b)
{
	uint8_t data[] = { 'P', 'L', 1, 0, 4, pixel, r, g, b };
	comm.write(data, 8);
}

void setLevel(Serial& comm, uint8_t level)
{
	uint8_t data[] = { 'P', 'L', 2, 0, 1, level };
	comm.write(data, 6);
}

void setRLE(Serial& comm)
{
	uint8_t data[] = { 'P', 'L', 3, 0, 12, 5, 255, 0, 0, 5, 0, 255, 0, 5, 0, 0, 255 };
	comm.write(data, 17);
}

struct SetColorCommand
{
	unsigned int r, g, b;
};

using Command = boost::variant<SetColorCommand>;

class CommandVisitor : public boost::static_visitor<void>
{
public:
	CommandVisitor(Serial& serial) :
		comm_(serial)
	{
	}

	void operator()(const SetColorCommand& cmd) const
	{
		setColor(comm_, cmd.r, cmd.g, cmd.b);
	}

private:
	Serial& comm_;
};

Command parserCommands(po::variables_map& vm, int argc, char * argv[]);

int main(int argc, char * argv[])
{

	boost::asio::io_service io;

	std::string device_name = "COM4";
	unsigned int baud_rate = 9600;

	po::variables_map vm;

	Command cmd = parserCommands(vm, argc, argv);

	Serial comm(io, device_name, baud_rate);
	boost::apply_visitor(CommandVisitor(comm), cmd);

    return 0;
}

Command parserCommands(po::variables_map& vm, int argc, char * argv[])
{
	// set global options
	po::options_description global("Global Options");
	global.add_options()
		("command", po::value<std::string>(), "Command to run")
		("subargs", po::value<std::vector<std::string>>(), "Arguments to command")
		("port", po::value<std::string>(), "device name")
		("baud", po::value<unsigned int>(), "baud rate");

	po::positional_options_description pos;
	pos
		.add("command", 1)
		.add("subargs", -1);

	po::parsed_options parsed = po::command_line_parser(argc, argv)
		.options(global)
		.positional(pos)
		.allow_unregistered()
		.run();

	po::store(parsed, vm);

	std::string cmd = vm["command"].as<std::string>();

	// process data for individual commands

	if (cmd == "setcolor")
	{
		po::options_description setcolor_desc("Options for setcolor command");
		setcolor_desc.add_options()
			("color", "color to set light strip");
		
		// collect unrecognized options from the first pass
		std::vector<std::string> opts = po::collect_unrecognized(parsed.options, po::include_positional);
		// command will be include so remove
		opts.erase(opts.begin());

		po::positional_options_description pos_setcolor;
		pos_setcolor.add("color", 1);

		// parse again
		po::store(po::command_line_parser(opts)
			.options(setcolor_desc)
			.positional(pos_setcolor)
			.run(), 
			vm
		);

		std::string color = vm["color"].as<std::string>();

		std::stringstream ss;
		ss << color;

		SetColorCommand args;
		ss >> args.r >> args.g >> args.b;

		return args;
	}

	throw po::invalid_option_value(cmd);
}