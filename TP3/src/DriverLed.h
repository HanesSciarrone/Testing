#include <stdint.h>

void DriverLed_Create(uint16_t *address);
void DriverLed_TurnOn(uint16_t numberled);
void DriverLed_TurnOff(uint16_t numberled);
void DriverLed_TurnOnAll(void);
void DriverLed_TurnOffAll(void);