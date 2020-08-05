#include "DriverLed.h"

static uint16_t *ledAddress;
static uint16_t ledsImage;

#define ALL_LED_OFF			0x0000
#define ALL_LED_ON			0xFFFF
#define LEDS_BIT_ON			1	
#define LEDS_TO_BITS_OFFSET	1	
#define FIRST_LED			0
#define LAST_LED			16	

#define true	1
#define false	0	

/**
  * @brief Convert number of led to bit of represent what is the led inside of mask
  */
static uint16_t ConvertLedNumberToBit(uint16_t numberled)
{
	return (LEDS_BIT_ON << (numberled - LEDS_TO_BITS_OFFSET));
}

static void updateHardware(void)
{
	*ledAddress = ledsImage;
}

static uint8_t IsLedOutOfBound(uint16_t numberled)
{
	return (numberled <= FIRST_LED || numberled > LAST_LED);
}

void SetLedImageBit(uint16_t numberled)
{
	ledsImage |= ConvertLedNumberToBit(numberled);
}

void ClearLedImageBit(uint16_t numberled)
{
	ledsImage &= ~ConvertLedNumberToBit(numberled);
}

void DriverLed_Create(uint16_t *address)
{
	ledAddress = address;
	ledsImage = ALL_LED_OFF;
	updateHardware();
}

void DriverLed_TurnOn(uint16_t numberled)
{
	if (IsLedOutOfBound(numberled))
	{
		return;
	}

	SetLedImageBit(numberled); 
	updateHardware(); 
}

void DriverLed_TurnOff(uint16_t numberled)
{
	if (IsLedOutOfBound(numberled))
	{
		return;
	}

	ClearLedImageBit(numberled);
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

uint8_t DriverLed_ReadStateOn(uint16_t numberled)
{
	if(IsLedOutOfBound(numberled))
	{
		return false;
	}

	return ledsImage & (ConvertLedNumberToBit(numberled));
}

uint8_t DriverLed_ReadStateOff(uint16_t numberled)
{
	return !DriverLed_ReadStateOn(numberled);
}