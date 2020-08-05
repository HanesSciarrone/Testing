#include "unity.h"
#include "DriverLed.h"

static uint16_t virtual_led;

void setUp(void) 
{
   DriverLed_Create(&virtual_led);
}

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
   DriverLed_TurnOn(12);
   TEST_ASSERT_EQUAL_HEX16(0x0800, virtual_led);
}

//! @test Prender y apagar un led
void test_TunrOn_and_TurnOff_led(void)
{
   DriverLed_TurnOn(1);
   DriverLed_TurnOff(1);
   TEST_ASSERT_EQUAL_HEX16(0x0000, virtual_led);
}

//! @test Prender y apagar algunos leds
void test_TunrOn_and_TurnOff_some_led(void)
{
   DriverLed_TurnOn(8);
   DriverLed_TurnOn(9);
   DriverLed_TurnOff(8);
   TEST_ASSERT_EQUAL_HEX16(0x0100, virtual_led);
}

//! @test Prender todos los leds simultaneamente
void test_TurnOn_all_leds(void)
{
   DriverLed_TurnOnAll();
   TEST_ASSERT_EQUAL_HEX16(0xFFFF, virtual_led);
}

//! @test Apagar todos los leds simultaneamente
void test_TurnOff_all_leds(void) 
{
   DriverLed_TurnOffAll();
   TEST_ASSERT_EQUAL_HEX16(0x0000, virtual_led);   
}

//! @test Asegurarse que un led no afecta los demas
void test_Led_dont_affect_the_rest(void)
{
   DriverLed_TurnOnAll();
   DriverLed_TurnOff(9);
   TEST_ASSERT_EQUAL_HEX16(0xFEFF, virtual_led);
}

//! @test Consultar el estado de un led prendido
void test_Read_state_led_on(void)
{
   TEST_ASSERT_FALSE(DriverLed_ReadStateOn(12));
   DriverLed_TurnOn(12);
   TEST_ASSERT_TRUE(DriverLed_ReadStateOn(12));
}

//! @test Consultar el estado de un led apagado  
void test_Read_state_led_off(void)
{
   TEST_ASSERT_TRUE(DriverLed_ReadStateOff(5));
   DriverLed_TurnOn(5);
   TEST_ASSERT_FALSE(DriverLed_ReadStateOff(5));
}

//! @test Parámetros en valores de borde
void test_Lower_and_upper_bound(void)
{
   DriverLed_TurnOn(1);
   DriverLed_TurnOn(16);
   TEST_ASSERT_EQUAL_HEX16(0X8001, virtual_led);
}

//! @test Parámetros son invalidos
void test_Invalid_parameters_(void)
{
   DriverLed_TurnOn(-1);
   TEST_ASSERT_EQUAL_HEX16(0X0000, virtual_led);
   DriverLed_TurnOn(0);
   TEST_ASSERT_EQUAL_HEX16(0X0000, virtual_led);
   DriverLed_TurnOn(17);
   TEST_ASSERT_EQUAL_HEX16(0X0000, virtual_led);
   DriverLed_TurnOn(3141);
   TEST_ASSERT_EQUAL_HEX16(0X0000, virtual_led);
}