/*
 * Configurar el ESP82266 para no realice ecos de los comandos.
 * Configurar el ESP82266 para comportarse como cliente.
 * Configurar el ESP8266 para que solo soporte 1 conexión
 * El ESP8266 debe conectarse a la red Wi-fi y a un servidor.
 * El ESP8266 debe enviar mensajes al servidor.
 * El ESP8266 debe recibir las respuestas del servicio.
 * El ESP8266 debe realizar un close del cliente una vez terminada la operación
 */

#include "unity.h"
#include "ESP8266.h"
#include <stdio.h>
#include <string.h>

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

/**
 * @brief Function used to simulate send function on
 * UART and process won't be success.
 * 
 * @param[in]   data Message to sent of modelue ESP8266
 * @param[in]  lenghth Size of message.
 *
 * \return Return 0 on success, 1 otherwise. 
 */
int8_t MockSend(const uint8_t *data, uint32_t length)
{
	char mensaje[64];
	if ( actual < transaccionesTotales && state == STATE_TRANSMMIT)
	{
		sprintf(mensaje, "Transaccion %d", actual);
		printf("%s\r\n", transaccion[actual].commando);
		TEST_ASSERT_EQUAL_MEMORY_MESSAGE(transaccion[actual].commando, data, length, mensaje);

		respondidos = 0;
		state = STATE_RECEIVE;
		return transaccion[actual].resultado;
	}
}

/**
 * @brief Function used to simulate send function on
 * UART and process will be success.
 * 
 * @param[in]   data Message to sent of modelue ESP8266
 * @param[in]  lenghth Size of message.
 *
 * \return Return 0 on success, -1 otherwise.
 */
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

// /**
//  * @brief Function used to simulate receive function on
//  * UART and process won't be success.
//  * 
//  * @param[in,out]   msg Message received of modelue ESP8266
//  * @param[in]      lenghth Size of buffer where message will storage.
//  *
//  * \return Length of data received
//  */
// int32_t RecvMessageFail(uint8_t *msg, uint32_t length)
// {
// 	strncpy((char *msg), "ERROR\r\n", 7);

// 	return 7;
// }

// /**
//  * @brief Function used to simulate receive function on
//  * UART and process will be success.
//  * 
//  * @param[in,out]   msg Message to sent of modelue ESP8266
//  * @param[in]      lenghth Size of buffer where message will storage.
//  *
//  * \return Length of data received
//  */
// int32_t RecvMessageOk(uint8_t *msg, uint32_t length)
// {
// 	strncpy((char *msg), "OK\r\n", 4);

// 	return 4;
// }


void setUp(void)
{
	actual = 0;
	state = STATE_TRANSMMIT;

	ESP8266_CommInterface_Init(&interface);
}

// void test_Config_Echo(void)
// {
// 	strcpy(comandoEsperado,"ATE0\r\n");
// 	strcpy(respuesta, "OK\r\n");

// 	TEST_ASSERT_EQUAL(ESP8266_OK, ESP8266_SetEcho(0));
// }

void test_Config_Echo(void)
{
	transaccion_t secuencia[] =	{
		{.commando = "ATE0\r\n", .resultado = 0, .respuesta = "OK\r\n"}
	};

	transaccion = secuencia;
	transaccionesTotales = 1;

	TEST_ASSERT_EQUAL(ESP8266_OK, ESP8266_SetEcho(0));
}