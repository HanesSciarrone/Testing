/*
 * El ESP8266 debe recibir las respuestas del servicio.
 * El ESP8266 debe realizar un close del cliente una vez terminada la operación
 */

#include "unity.h"
#include "ESP8266.h"
#include <stdio.h>
#include <string.h>

/* -------------------- Global variable -------------------- */
uint32_t respondidos;	// Used to accumulate size of byte read

/** \struct Structure used to simulate message send/received */
typedef struct transaction_s
{
	uint8_t *command;	/*< Command send to ESP8266 */
	uint8_t result;  /*< Resulted of operation sent */
	uint8_t *response; /*< Response received from ESP8266 */
}transaction_t;


transaction_t *transactions;	/// Vector of message send and received
int actual = 0; 				/// Operation actual if I am going to sent more one
int totalTransaction = 1;		/// Total of operation that I do

enum
{
	STATE_TRANSMMIT,
	STATE_RECEIVE,
}state;


/**
 * @brief Function used to simulate send function on  UART
 * 
 * @param[in]  data    Message to sent of modelue ESP8266
 * @param[in]  lenghth Size of message.
 *
 * \return Return 0 on success, 1 otherwise. 
 */
int8_t MockSend(const uint8_t *data, uint32_t length)
{
	char mensaje[64];

	if ( actual < totalTransaction && state == STATE_TRANSMMIT)
	{
		sprintf(mensaje, "Transaccion %d", actual);
		printf("%s\r\n", transactions[actual].command);
		TEST_ASSERT_EQUAL_MEMORY_MESSAGE(transactions[actual].command, data, length, mensaje);

		respondidos = 0;
		state = STATE_RECEIVE;
		return transactions[actual].result;
	}
}

/**
 * @brief Function used to simulate receive function on UART
 * 
 * @param[in]  data    Buffer where Iĺl save messsage received from ESP8266
 * @param[in]  lenghth Size of buffer.
 *
 * \return Return total of length read or 0 otherwise.
 */
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

//** Variable used to inject mock function of sent and receive into of driver */
const ESP8266_CommInterface_s interface = {
	.send = MockSend, 
	.recv = MockReceive 
};

/* -------------------- Private function ------------------- */

/** @brief Function to initialize configuration of Wifi */
static uint8_t WifiModule_Init(void);
/** @brief Function to connect module of Wifi network and server */
static uint8_t WifiModule_Connect(void);



static uint8_t WifiModule_Init(void)
{
	// Configure module for avoid echo
	if ( ESP8266_SetEcho(0) != ESP8266_OK)
	{
		return 0;
	}

	// Configure module as Station Mode
	if ( ESP8266_SetModeWIFI((uint8_t *)"1") != ESP8266_OK)
	{
		return 0;
	}

	// Configure module as Station Mode
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

	// Connection parameters network
	network.ssid = "Name network";
	network.password = "Password network";

	// Server parameters
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



/* ------------------- Function ceedling ------------------- */

void setUp(void)
{
	actual = 0;
	state = STATE_TRANSMMIT;

	ESP8266_CommInterface_Init(&interface);
}

/** @brief Unit test of:
 *
 * Configure ESP8266 to don't echo.
 * Configure ESP8266 that it will behavior client.
 * Configure ESP8266 to soport only one connection
 */
void test_Config_Init(void)
{
	transaction_t secuencia[] =	{
		{.command = "ATE0\r\n", .result = 0, .response = "OK\r\n"},
		{.command = "AT+CWMODE=1\r\n", .result = 0, .response = "OK\r\n"},
		{.command = "AT+CIPMUX=0\r\n", .result = 0, .response = "OK\r\n"}
	};

	transactions = secuencia;
	totalTransaction = 3;

	TEST_ASSERT_EQUAL(1, WifiModule_Init());
}

/** @brief Unit test of:
 *
 * Conection to Wi-Fi
 * Connection with server
 */
void test_ConnectionWifi(void)
{
	transaction_t sequence[] = {
		{.command = "AT+CWJAP_CUR=\"Name network\",\"Password network\"\r\n", .result = 0, .response = "WIFI CONNECTED\r\n"},
		{.command = "AT+CIPSTART=\"TCP\",\"mqtt.eclipse.org\",1883\r\n", .result = 0, .response = "OK\r\n"}
	} ;

	transactions = sequence;
	totalTransaction = 2;

	TEST_ASSERT_EQUAL(1, WifiModule_Connect());
}

/**
  * @brief Unit test of:
  *
  *  Sent message to server. 
 */
void test_SendMessageServer(void)
{
	transaction_t sequence[] = {
		{.command = "AT+CIPSEND=26\r\n", .result = 0, .response = "OK\r\n>"},
		{.command = "Probe ceedling with driver", .result = 0, .response = "SEND OK\r\n"}
	};

	transactions = sequence;
	totalTransaction = 2;	

	TEST_ASSERT_EQUAL(ESP8266_OK, ESP8266_SentData("Probe ceedling with driver", 26));	
}