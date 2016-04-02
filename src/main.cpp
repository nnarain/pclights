
#include <iostream>

#include <simplelogger/simplelogger.h>
#include <simplelogger/logger/console_logger.h>

using namespace simplelogger;

int main()
{
	registerLogger<ConsoleLogger>();

	LOG_INFO("Test info message\n");
	LOG_DEBUG("Test debug message\n");
	LOG_WARN("Test warn message\n");
	LOG_ERROR("Test error message\n");

	releaseLoggers();
	return 0;
}
