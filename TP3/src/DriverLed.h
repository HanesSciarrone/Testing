#include <stdint.h>
#include <stdbool.h>

/** @brief Initialize address of leds and turn off all */
void DriverLed_Create(uint16_t *address);

/** @brief Turn on led */
void DriverLed_TurnOn(uint16_t numberled);

/** @brief Turn off led */
void DriverLed_TurnOff(uint16_t numberled);

/** @brief Turn on all led */
void DriverLed_TurnOnAll(void);

/** @brief Turn off all led */
void DriverLed_TurnOffAll(void);

/** @brief Read state when led is on */
bool DriverLed_ReadStateOn(uint16_t numberled);

/** @brief Read state when led is off */
bool DriverLed_ReadStateOff(uint16_t numberled);