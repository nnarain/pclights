
#include <avr/io.h>
#include <util/delay.h>

#include <stdperiph/gpio_port.h>

#include <WS2812/Ws2812Driver.h>

#define RED   0xFF0000
#define GREEN 0x00FF00
#define BLUE  0x0000FF

GPIO(PORTB, DDRB, GpioB);

typedef Ws2812Driver<GpioB, 5, 60> LedsDriver;

int main()
{
	LedsDriver leds;
	leds.begin();

	uint32_t colors[] = { RED, RED | GREEN, GREEN, GREEN | BLUE, BLUE, BLUE | RED };
	int colors_len = 6;

	leds.clear();
	leds.show();

	for(;;)
	{
		for(int i = 0; i < colors_len; ++i)
		{
			for(int j = 0; j < LedsDriver::NUM_LEDS; ++j)
			{
				leds.setPixel(j, colors[i]);
				leds.show();
				_delay_ms(100);
			}
		}
	}

	return 0;
}
