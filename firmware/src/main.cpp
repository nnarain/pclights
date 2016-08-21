
/**
    PC Lights Firmware

    @author Natesh Narain
    @date August 20 2016
*/

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#define PIN 6
#define NUM_PIXELS 60

static Adafruit_NeoPixel pixels(NUM_PIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup()
{
    Serial.begin(9600);

    pixels.begin();
    pixels.clear();
    pixels.setPixelColor(0, 255, 0, 0);
    pixels.show();
}

void loop()
{
    if(Serial.available() >= 3)
    {
        char buffer[3];
        Serial.readBytes(buffer, 3);

        if(buffer[0] == 1 && buffer[1] == 2 && buffer[2] == 3)
        {
            for(int i = 0; i < NUM_PIXELS; ++i)
            {
                pixels.setPixelColor(i, 0, 255, 0);
                pixels.show();
                delay(50);
            }
        }
    }
}
