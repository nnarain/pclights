
#include "configuration.h"

#ifdef _WIN32
#	include <windows.h>
#	include <Shlwapi.h>
#	include <ShlObj.h>
#endif

#include <iostream>
#include <exception>
#include <sstream>
#include <cstdlib>

#include <boost/filesystem.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace fs = boost::filesystem;
namespace pt = boost::property_tree;

Configuration::Configuration()
{
}

void Configuration::loadConfigFile(const std::string& config_file)
{
	load(config_file);
}

void Configuration::loadDefaultConfigFile()
{
	fs::path config_path(getDefaultPath());
	fs::path folder("pclights");
	fs::path file_name("pclights.conf");
	fs::path file_path = config_path / folder / file_name;

	load(file_path.string());
}

void Configuration::load(const std::string& file_path)
{
	pt::read_json(file_path, root_);
}

std::string Configuration::getDefaultPath()
{
#ifdef _WIN32
	TCHAR path[MAX_PATH];

	if (!SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, path)))
	{
		throw std::runtime_error("Unable to get default configuration file path");
	}

	return std::string(path);

#elif __unix
	return std::string(std::getenv("HOME"));
#else
#	error "Default path not specified on this OS"
#endif
}