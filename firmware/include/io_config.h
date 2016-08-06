#ifndef IO_CONFIG_H
#define IO_CONFIG_H

#include <avr/io.h>

#include <stdperiph/stdperiph.h>

// TODO: remove these defines...
#define REG_BASE 0x20
#define PORTB_ADDR (REG_BASE + 0x05)
#define DDRB_ADDR  (REG_BASE + 0x04)

GPIO(PORTB_ADDR, DDRB_ADDR, GpioB);

typedef stdperiph::GpioPinRef<GpioB, 5> led;

#endif // IO_CONFIG_H
