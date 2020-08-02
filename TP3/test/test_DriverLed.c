/**
  * Prender y apagar un led
  * Prender y apagar algunos leds
  * Asegurarse que un led no afecta los demas
  * Prender todos los leds simultaneamente
  * Apagar todos los leds simultaneamente
  * Consultar el estado de un led prendido
  * Consultar el estado de un led apagado  
  * Parámetros son invalidos
  * Parámetros en valores de borde
  */

#include "unity.h"
#include "DriverLed.h"

//! @test Todos los leds empiezan apagados
void test_All_Led_Initialice_Off(void)
{
   uint16_t virtual_led = 0xFFFF;

   DriverLed_Create(&virtual_led);
   TEST_ASSERT_EQUAL_HEX16(0x0000, virtual_led);
}

//! @test Encender un led
void test_TurnOn_Led(void)
{
   uint16_t virtual_led;

   DriverLed_Create(&virtual_led);
   DriverLed_TurnOn(1);
   TEST_ASSERT_EQUAL_HEX16(0x0001, virtual_led);
}

//! @test Prender y apagar un led
void test_TunrOn_and_TurnOff_some_led(void)
{
   uint16_t virtual_led;
   DriverLed_Create(&virtual_led);
   DriverLed_TurnOn(1);
   DriverLed_TurnOff(0);
   TEST_ASSERT_EQUAL_HEX16(0x0000, virtual_led);
}