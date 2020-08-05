#include <stdint.h>

void DriverLed_Create(uint16_t *address);
void DriverLed_TurnOn(uint16_t numberled);
void DriverLed_TurnOff(uint16_t numberled);
void DriverLed_TurnOnAll(void);
void DriverLed_TurnOffAll(void);
uint8_t DriverLed_ReadStateOn(uint16_t numberled);
uint8_t DriverLed_ReadStateOff(uint16_t numberled);