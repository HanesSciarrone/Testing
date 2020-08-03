#include "DriverLed.h"

static uint16_t *ledAddress;
static uint16_t ledsImage;

#define ALL_LED_OFF			0x0000
#define ALL_LED_ON			0xFFFF
#define LEDS_BIT_ON			1	
#define LEDS_TO_BITS_OFFSET	1			

static uint16_t ConvertLedNumberToBit(uint16_t numberled)
{
	return (LEDS_BIT_ON << (numberled - LEDS_TO_BITS_OFFSET));
}

static void updateHardware(void)
{
	*ledAddress = ledsImage;
}

void DriverLed_Create(uint16_t *address)
{
	ledAddress = address;
	ledsImage = ALL_LED_OFF;
	updateHardware();
}

void DriverLed_TurnOn(uint16_t numberled)
{
	if (numberled <= 0 || numberled > 16)
	{
		return;
	}

	ledsImage |= ConvertLedNumberToBit(numberled); 
	updateHardware(); 
}

void DriverLed_TurnOff(uint16_t numberled)
{
	if (numberled <= 0 || numberled > 16)
	{
		return;
	}

	ledsImage &= ~ConvertLedNumberToBit(numberled); 
	updateHardware(); 
}

void DriverLed_TurnOnAll(void)
{
	ledsImage = ALL_LED_ON;
	updateHardware();
}

void DriverLed_TurnOffAll(void)
{
	ledsImage = ALL_LED_OFF;
	updateHardware();
}