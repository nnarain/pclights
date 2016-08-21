
/**
    PC Lights Firmware

    @author Natesh Narain
    @date August 20 2016
*/

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#include "SerialStream.h"

#define PIN 6
#define NUM_PIXELS 60

#define SIGNATURE ((('P' & 0xFFFF) << 8) | 'L')

enum Command
{
    CLEAR = 0,
    SET_COLOR = 1
};

struct Header
{
    uint16_t signature;
    uint8_t command;
    uint16_t payload_length;
};

static Adafruit_NeoPixel pixels(NUM_PIXELS, PIN, NEO_GRB + NEO_KHZ800);

void clear()
{
    for(int i = 0; i < NUM_PIXELS; ++i)
    {
        pixels.setPixelColor(i, 0, 0, 0);
    }

    pixels.show();
}

void setColor()
{
    char buffer[3];
    Serial.readBytes(buffer, 3);

    for(int i = 0; i < NUM_PIXELS; ++i)
    {
        pixels.setPixelColor(i, buffer[0], buffer[1], buffer[2]);
    }

    pixels.show();
}

Header readHeader()
{
    Header header;
    char buffer[sizeof(Header)];
    Serial.readBytes(buffer, sizeof(Header));

    SerialStream ss(buffer);
    ss >> header.signature >> header.command >> header.payload_length;

    return header;
}

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
    // check if a header is available
    if((unsigned int)Serial.available() >= sizeof(Header))
    {
        // get the header
        Header header = readHeader();

        // verify the signature is correct
        if(header.signature == SIGNATURE)
        {
            switch(header.command)
            {
                case CLEAR:
                    clear();
                    break;
                case SET_COLOR:
                    setColor();
                    break;
            }
        }
    }
}
