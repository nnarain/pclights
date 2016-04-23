
#include <avr/io.h>
#include <util/delay.h>

#include <stdperiph/gpio_port.h>

#include <WS2812/Ws2812Driver.h>

#include <serialmessages/message_client.h>
#include <serialmessages/publisher.h>

#include "serial.h"

GPIO(PORTB, DDRB, GpioB);

typedef Ws2812Driver<GpioB, 5, 60> LedsDriver;

using namespace serialmessages;

int main()
{
	LedsDriver leds;
	leds.begin();

	MessageClient<Serial> client(9600);

	Publisher<stdmsgs::String> pub("test_topic", &client);

	stdmsgs::String msg;
	msg.data = "Hello World";

	client.initialize();

	pub.publish(msg);

	for(;;)
	{
		client.spinOnce();
	}

	return 0;
}



