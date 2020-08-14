#include "build/temp/_Test_ESP8266.c"
#include "src/ESP8266.h"
#include "/var/lib/gems/2.5.0/gems/ceedling-0.30.0/vendor/unity/src/unity.h"






uint8_t comandoEsperado[64];

uint8_t respuesta[64];

uint8_t resultSend = ESP8266_OK;

uint32_t respondidos;



typedef struct transaccion_s

{

 uint8_t *commando;

 uint8_t resultado;

 uint8_t *respuesta;

}transaccion_t;



transaccion_t *transaccion;

int actual = 0;

int transaccionesTotales = 1;



enum

{

 STATE_TRANSMMIT,

 STATE_RECEIVE,

}state = STATE_TRANSMMIT;

int8_t MockSend(const uint8_t *data, uint32_t length)

{

 char mensaje[64];

 if ( actual < transaccionesTotales && state == STATE_TRANSMMIT)

 {

  sprintf(mensaje, "Transaccion %d", actual);

  UnityAssertEqualMemory(( const void*)((transaccion[actual].commando)), ( const void*)((data)), (UNITY_UINT32)((length)), 1, ((mensaje)), (UNITY_UINT)(53), UNITY_ARRAY_TO_ARRAY);



  respondidos = 0;

  state = STATE_RECEIVE;

  return transaccion[actual].resultado;

 }

}

int32_t MockReceive(uint8_t *data, uint32_t length)

{

 int32_t largo = strlen((char *)transaccion[actual].respuesta);



 if (state != STATE_RECEIVE)

 {

  return 0;

 }



 if ( respondidos < largo)

 {

  memcpy(data, &transaccion[actual].respuesta[respondidos], length);

  respondidos += length;

  return length;

 }

 else

 {

  actual++;

  state = STATE_TRANSMMIT;

  return 0;

 }

}



const ESP8266_CommInterface_s interface = {

 .send = MockSend,

 .recv = MockReceive

};

void setUp(void)

{

 actual = 0;

 state = STATE_TRANSMMIT;



 ESP8266_CommInterface_Init(&interface);

}

void test_Config_Echo(void)

{

 transaccion_t secuencia[] = {

  {.commando = "ATE0\r\n", .resultado = 0, .respuesta = "OK\r\n"}

 };



 transaccion = secuencia;

 transaccionesTotales = 1;



 UnityAssertEqualNumber((UNITY_INT)((ESP8266_OK)), (UNITY_INT)((ESP8266_SetEcho(0))), (

((void *)0)

), (UNITY_UINT)(156), UNITY_DISPLAY_STYLE_INT);

}
