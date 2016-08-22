#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <string>
#include <fstream>

#include <boost/property_tree/ptree.hpp>

class Configuration
{
public:
	Configuration();

	void loadConfigFile(const std::string& config_file);
	void loadDefaultConfigFile();

	template<typename T>
	T get(const std::string& name)
	{
		return root_.get<T>(name);
	}

private:
	void load(const std::string& file_path);
	std::string getDefaultPath();

private:
	boost::property_tree::ptree root_;
};

#endif
