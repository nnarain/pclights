
#include <avr/io.h>
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

int main()
{
	indicator_callback::mode(stdperiph::BitMode::OUTPUT);
	indicator_start::mode(stdperiph::BitMode::OUTPUT);
	led::mode(stdperiph::BitMode::OUTPUT);

	MessageClient<Serial, 128, 128> client(BAUD);

	Subscriber<stdmsgs::ColorRGB> sub_set_led("set_color", &setColorCallback);
	client.subscribe(&sub_set_led);

	// Publisher<stdmsgs::ColorRGB> pub_set_color("set_color", &client);

	indicator_start::low();
	led::low();

	client.initialize();

	indicator_start::high();
	indicator_callback::low();

	// leds.begin();
	// leds.clear();
	// leds.show();

	// stdmsgs::ColorRGB color;
	// color.r = 0xFF;
	// color.g = 0xFF;
	// color.b = 0x00;



//	pub_set_color.publish(color);

//	setColorCallback(color);

	for(;;)
	{
		client.spinOnce();

		// for(int i = 0; i < 3; ++i)
		// {
		// 	for(int j = 0; j < 60; ++j)
		// 	{
		// 		leds.setPixel(j, colors[i]);
		// 		leds.show();
		// 		_delay_ms(100);
		// 	}
		// }
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


