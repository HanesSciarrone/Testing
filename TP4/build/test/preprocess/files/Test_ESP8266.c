#include "build/temp/_Test_ESP8266.c"
#include "src/ESP8266.h"
#include "/var/lib/gems/2.5.0/gems/ceedling-0.30.0/vendor/unity/src/unity.h"


















uint32_t respondidos;





typedef struct transaction_s

{

 uint8_t *command;

 uint8_t result;

 uint8_t *response;

}transaction_t;





transaction_t *transactions;

int actual = 0;

int totalTransaction = 1;



enum

{

 STATE_TRANSMMIT,

 STATE_RECEIVE,

}state;

int8_t MockSend(const uint8_t *data, uint32_t length)

{

 char mensaje[64];



 if ( actual < totalTransaction && state == STATE_TRANSMMIT)

 {

  sprintf(mensaje, "Transaccion %d", actual);

  printf("%s\r\n", transactions[actual].command);

  UnityAssertEqualMemory(( const void*)((transactions[actual].command)), ( const void*)((data)), (UNITY_UINT32)((length)), 1, ((mensaje)), (UNITY_UINT)(50), UNITY_ARRAY_TO_ARRAY);



  respondidos = 0;

  state = STATE_RECEIVE;

  return transactions[actual].result;

 }

}

int32_t MockReceive(uint8_t *data, uint32_t length)

{

 int32_t largo = strlen((char *)transactions[actual].response);



 if (state != STATE_RECEIVE)

 {

  return 0;

 }



 if (respondidos < largo)

 {

  memcpy(data, &transactions[actual].response[respondidos], length);

  respondidos += length;



  if ( respondidos == largo )

  {

   actual++;

   state = STATE_TRANSMMIT;

  }

  return length;

 }

}





const ESP8266_CommInterface_s interface = {

 .send = MockSend,

 .recv = MockReceive

};









static uint8_t WifiModule_Init(void);



static uint8_t WifiModule_Connect(void);







static uint8_t WifiModule_Init(void)

{



 if ( ESP8266_SetEcho(0) != ESP8266_OK)

 {

  return 0;

 }





 if ( ESP8266_SetModeWIFI((uint8_t *)"1") != ESP8266_OK)

 {

  return 0;

 }





 if ( ESP8266_SetMultipleConnection((uint8_t *)"0") != ESP8266_OK)

 {

  return 0;

 }



 return 1;

}



static uint8_t WifiModule_Connect(void)

{

 ESP8266_NetworkParameters_s network;

 ESP8266_ServerParameters_s server;





 network.ssid = "Name network";

 network.password = "Password network";





 server.protocol = "TCP";

 server.host = "mqtt.eclipse.org";

 server.port = 1883;





 if ( ESP8266_ConnectionNetwork(&network) != ESP8266_OK )

 {

  return 0;

 }



 if (ESP8266_ConnectionServer(&server) != ESP8266_OK)

 {

  return 0;

 }



 return 1;

}











void setUp(void)

{

 actual = 0;

 state = STATE_TRANSMMIT;



 ESP8266_CommInterface_Init(&interface);

}















void test_Config_Init(void)

{

 transaction_t secuencia[] = {

  {.command = "ATE0\r\n", .result = 0, .response = "OK\r\n"},

  {.command = "AT+CWMODE=1\r\n", .result = 0, .response = "OK\r\n"},

  {.command = "AT+CIPMUX=0\r\n", .result = 0, .response = "OK\r\n"}

 };



 transactions = secuencia;

 totalTransaction = 3;



 UnityAssertEqualNumber((UNITY_INT)((1)), (UNITY_INT)((WifiModule_Init())), (

((void *)0)

), (UNITY_UINT)(184), UNITY_DISPLAY_STYLE_INT);

}













void test_ConnectionWifi(void)

{

 transaction_t sequence[] = {

  {.command = "AT+CWJAP_CUR=\"Name network\",\"Password network\"\r\n", .result = 0, .response = "WIFI CONNECTED\r\n"},

  {.command = "AT+CIPSTART=\"TCP\",\"mqtt.eclipse.org\",1883\r\n", .result = 0, .response = "OK\r\n"}

 } ;



 transactions = sequence;

 totalTransaction = 2;



 UnityAssertEqualNumber((UNITY_INT)((1)), (UNITY_INT)((WifiModule_Connect())), (

((void *)0)

), (UNITY_UINT)(202), UNITY_DISPLAY_STYLE_INT);

}













void test_SendMessageServer(void)

{

 transaction_t sequence[] = {

  {.command = "AT+CIPSEND=26\r\n", .result = 0, .response = "OK\r\n>"},

  {.command = "Probe ceedling with driver", .result = 0, .response = "SEND OK\r\n"}

 };



 transactions = sequence;

 totalTransaction = 2;



 UnityAssertEqualNumber((UNITY_INT)((ESP8266_OK)), (UNITY_INT)((ESP8266_SentData("Probe ceedling with driver", 26))), (

((void *)0)

), (UNITY_UINT)(220), UNITY_DISPLAY_STYLE_INT);

}
