
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include <stdperiph/stdperiph.h>

#include <WS2812/Ws2812Driver.h>

#include <serialmessages/message_client.h>
#include <serialmessages/subscriber.h>
#include <serialmessages/publisher.h>
#include <serialmessages/stdmsgs/color_rgb.h>

#include "serial.h"

#define RED   0xFF0000
#define GREEN 0x00FF00
#define BLUE  0x0000FF

static const int BAUD = 9600;

GPIO(PORTB, DDRB, GpioB);
typedef stdperiph::GpioPinRef<GpioB, 4> indicator_callback;
typedef stdperiph::GpioPinRef<GpioB, 3> indicator_start;
typedef stdperiph::GpioPinRef<GpioB, 5> led;

//typedef Ws2812Driver<GpioB, 5, 60> LedsDriver;

using namespace serialmessages;

/// VARS ----------------------------------------------------------------------
//static LedsDriver leds;

/// PROTOTYPES ----------------------------------------------------------------
void setColorCallback(const stdmsgs::ColorRGB& msg);

void errorCallback(uint8_t b)
{
	if(b)
		indicator_callback::high();
	else
		indicator_callback::low();
}

int main()
{
	indicator_callback::mode(stdperiph::BitMode::OUTPUT);
	indicator_start::mode(stdperiph::BitMode::OUTPUT);
	led::mode(stdperiph::BitMode::OUTPUT);

	indicator_callback::low();

	MessageClient<Serial, 64, 64> client(BAUD);

	Publisher<stdmsgs::ColorRGB> pub_color("set_color", &client);

	client.setLog(errorCallback);

	client.initialize();

	stdmsgs::ColorRGB color;
	color.r = 0xFF;
	color.g = 0xFF;
	color.b = 0x00;

	sei();

	for(;;)
	{
		pub_color.publish(color);
		client.spinOnce();
	}

	return 0;
}

void setColorCallback(const stdmsgs::ColorRGB& msg)
{
//	uint32_t color = ((msg.g & 0xFFFFFF) << 16) | ((msg.r & 0xFFFFFF) << 8) | (msg.b);
	
	// for(int i = 0; i < 60; ++i)
	// {
	// 	leds.setPixel(i, 0, 0, 255);
	// }

	// leds.show();

	indicator_callback::high();
}


