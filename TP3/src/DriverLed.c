#include "DriverLed.h"

static uint16_t *ledAddress;

void DriverLed_Create(uint16_t *address)
{
	ledAddress = address;
	*ledAddress = 0x0000;
}

void DriverLed_TurnOn(uint16_t numberled)
{
	*ledAddress = 1;
}

void DriverLed_TurnOff(uint16_t numberled)
{
	*ledAddress = 0;
}