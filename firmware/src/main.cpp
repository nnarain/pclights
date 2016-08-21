
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
    SET_COLOR = 0,
    SET_PIXEL = 1,
    SET_LEVEL = 2,
    SET_RLE   = 3
};

struct Header
{
    uint16_t signature;
    uint8_t command;
    uint16_t payload_length;
};

// -----------------------------------------------------------------------------
// Prototypes
//------------------------------------------------------------------------------
Header readHeader();
void clear();
void setColor();
void setPixel();
void setLevel();
void setRLE();

// -----------------------------------------------------------------------------
// Globals
//------------------------------------------------------------------------------
static Adafruit_NeoPixel pixels(NUM_PIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup()
{
    Serial.begin(9600);

    pixels.begin();
    pixels.clear();
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
                case SET_COLOR:
                    setColor();
                    break;
                case SET_PIXEL:
                    setPixel();
                    break;
                case SET_LEVEL:
                    setLevel();
                    break;
                case SET_RLE:
                    setRLE();
                    break;
            }
        }
    }
}

void clear()
{
    for(int i = 0; i < NUM_PIXELS; ++i)
    {
        pixels.setPixelColor(i, 0, 0, 0);
    }

    pixels.show();
}

void setPixel()
{
    char buffer[4];
    Serial.readBytes(buffer, 4);

    pixels.setPixelColor(buffer[0], buffer[1], buffer[2], buffer[3]);
    pixels.show();
}

void setLevel()
{
    char level;
    Serial.readBytes(&level, 1);

    pixels.setBrightness((uint8_t)level);
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

void setRLE()
{
    
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
