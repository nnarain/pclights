
#include <iostream>

#include <simplelogger/simplelogger.h>
#include <simplelogger/logger/console_logger.h>

using namespace simplelogger;

int main()
{
	registerLogger<ConsoleLogger>();

	for(int i = 0; i < 5; ++i)
	{
		log(Level::INFO, "Hello World\n", 12);
	}


	releaseLoggers();
	return 0;
}
