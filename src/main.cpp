
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <sstream>
#include <cstdint>

#include <boost/asio.hpp>
#include <boost/program_options.hpp>
#include <boost/variant.hpp>

#include "protocol.h"
#include "serial.h"
#include "configuration.h"
#include "animation.h"

namespace po = boost::program_options;

/* Command line command structures */

struct SetColorCommand
{
	unsigned int r, g, b;
};

struct SetLevelCommand
{
	unsigned int level;
};

struct PlayCommand
{
	std::string animation_name;
};

using Command = boost::variant<SetColorCommand, SetLevelCommand, PlayCommand>;
using MyProtocol = Protocol<Serial>;

/* Visitor to use with boost::variant when parsing command line commands */

template<typename ProtocolT>
class CommandVisitor : public boost::static_visitor<void>
{
public:
	CommandVisitor(ProtocolT& protocol, Configuration& config) :
		comm_(protocol),
		config_(config)
	{
	}

	void operator()(const SetColorCommand& cmd) const
	{
		comm_.setColor(cmd.r, cmd.g, cmd.b);
	}

	void operator()(const SetLevelCommand& cmd) const
	{
		comm_.setLevel(cmd.level);
	}

	void operator()(const PlayCommand& cmd) const
	{
		std::string base = "animations." + cmd.animation_name;
		std::string file_key       = base + ".file";
		std::string frame_rate_key = base + ".frame_rate";

		std::string file = config_.get<std::string>(file_key);
		float frame_rate = config_.get<float>(frame_rate_key);

		Animation<MyProtocol> animation(comm_, file, frame_rate);
		animation.play();
	}

private:
	ProtocolT& comm_;
	Configuration& config_;
};

/**
	Parse command line for commands and arguments
*/
Command parseCommands(po::variables_map& vm, int argc, char * argv[]);

void parseCommandArguments(
	po::parsed_options& parsed,
	po::options_description& command_desc,
	po::positional_options_description& command_pos,
	po::variables_map& vm
);

template<typename T>
T getOption(const std::string& name, Configuration& config, po::variables_map& vm)
{
	if (vm.count(name) == 1)
	{
		return vm[name].as<T>();
	}
	else
	{
		return config.get<T>(name);
	}
}

int main(int argc, char * argv[])
{
	Configuration config;

	// parse command line options and commands
	po::variables_map vm;
	Command cmd = parseCommands(vm, argc, argv);

	try
	{
		// load config file either from command line option or default
		if (vm.count("config") == 1)
			config.loadConfigFile(vm["config"].as<std::string>());
		else
			config.loadDefaultConfigFile();
	}
	catch (std::runtime_error& err)
	{
		std::cerr << err.what() << std::endl;
		return 1;
	}

	std::string device_name = getOption<std::string>("port", config, vm);
	unsigned int baud_rate  = getOption<unsigned int>("baud", config, vm);

	boost::asio::io_service io;
	MyProtocol comm(io, device_name, baud_rate);

	boost::apply_visitor(CommandVisitor<MyProtocol>(comm, config), cmd);

    return 0;
}

Command parseCommands(po::variables_map& vm, int argc, char * argv[])
{
	// set global options
	po::options_description global("Global Options");
	global.add_options()
		("command", po::value<std::string>(), "Command to run")
		("subargs", po::value<std::vector<std::string>>(), "Arguments to command")
		("port", po::value<std::string>(), "device name")
		("baud", po::value<unsigned int>(), "baud rate")
		("config", po::value<std::string>(), "pclights configuration file");

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
		po::positional_options_description pos_setcolor;
		pos_setcolor.add("color", 1);

		parseCommandArguments(parsed, setcolor_desc, pos_setcolor, vm);

		std::string color = vm["color"].as<std::string>();
		std::stringstream ss;
		ss << color;
		SetColorCommand args;
		ss >> args.r >> args.g >> args.b;

		return args;
	}
	else if (cmd == "level")
	{
		po::options_description setlevel_desc("Options for setcolor command");
		setlevel_desc.add_options()
			("level", "adjust overall brightness level (0  - 100)%");
		po::positional_options_description setlevel_pos;
		setlevel_pos.add("level", 1);

		parseCommandArguments(parsed, setlevel_desc, setlevel_pos, vm);

		// get a scaling value from percent
		float scale = std::stof(vm["level"].as<std::string>()) / 100.0f;

		if (scale > 1) scale = 1;
		if (scale < 0) scale = 0;

		SetLevelCommand cmd;
		cmd.level = (unsigned int)(255.0f * scale);

		return cmd;
	}
	else if (cmd == "off")
	{
		SetColorCommand cmd;
		cmd.r = 0;
		cmd.g = 0;
		cmd.b = 0;

		return cmd;
	}
	else if (cmd == "on")
	{
		SetColorCommand cmd;
		cmd.r = 255;
		cmd.g = 255;
		cmd.b = 255;

		return cmd;
	}
	else if (cmd == "play")
	{
		po::options_description play_desc("Options for play command");
		play_desc.add_options()
			("name", "Animation name");
		po::positional_options_description play_pos;
		play_pos.add("name", 1);

		parseCommandArguments(parsed, play_desc, play_pos, vm);

		PlayCommand cmd;
		cmd.animation_name = vm["name"].as<std::string>();

		return cmd;
	}

	throw po::invalid_option_value(cmd);
}

void parseCommandArguments(
	po::parsed_options& parsed,
	po::options_description& command_desc,
	po::positional_options_description& command_pos,
	po::variables_map& vm)
{
	// collect unrecognized options from the first pass
	std::vector<std::string> opts = po::collect_unrecognized(parsed.options, po::include_positional);
	// command will be include so remove
	opts.erase(opts.begin());

	// parse again
	po::store(po::command_line_parser(opts)
		.options(command_desc)
		.positional(command_pos)
		.run(),
		vm
	);
}