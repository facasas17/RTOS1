/*=============================================================================
 * Copyright (c) 2020, Fabiola de las Casas <fabioladelascasas@gmail.com>
 * All rights reserved.
 * License: Free
 * Date: 2020/09/30
 * Version: v1.1
 *===========================================================================*/

/*==================[ Inclusions ]============================================*/
#include "FreeRTOS.h"
#include "task.h"
#include "sapi.h"
#include "keys.h"

/*=====[ Definitions of private data types ]===================================*/


/*=====[Definition macros of private constants]==============================*/
#define DEBOUNCE_TIME   40
#define key_count   sizeof(keys_config)/sizeof(keys_config[0])

/*=====[Prototypes (declarations) of private functions]======================*/

static void keys_ButtonError( uint32_t index );
static void buttonPressed( uint32_t index );
static void buttonReleased( uint32_t index );

/*=====[Definitions of private global variables]=============================*/

/*=====[Definitions of public global variables]==============================*/
const t_key_config  keys_config[] = { TEC1, TEC2, TEC3, TEC4 } ;
gpioMap_t leds[]   = {LEDB,LED1,LED2,LED3};

t_key_data keys_data[key_count];

QueueHandle_t queue_tec_pulsada;

// Defino estructura global para encolar punteros
static t_TiempoLed tiempo_led;

/*=====[prototype of private functions]=================================*/
void task_tecla( void* taskParmPtr );
void task_led( void* taskParmPtr );

/*=====[Implementations of public functions]=================================*/

// keys_ Update State Function
void keys_Update( uint32_t index )
{
	switch( keys_data[index].state )
	{
		case STATE_BUTTON_UP:
			/* CHECK TRANSITION CONDITIONS */
			if( !gpioRead( keys_config[index].tecla ) )
			{
				keys_data[index].state = STATE_BUTTON_FALLING;
			}
			break;

		case STATE_BUTTON_FALLING:
			/* ENTRY */

			/* CHECK TRANSITION CONDITIONS */
			if( !gpioRead( keys_config[index].tecla ) )
			{
				keys_data[index].state = STATE_BUTTON_DOWN;

				/* ACCION DEL EVENTO !*/
				buttonPressed( index );
			}
			else
			{
				keys_data[index].state = STATE_BUTTON_UP;
			}

			/* LEAVE */
			break;

		case STATE_BUTTON_DOWN:
			/* CHECK TRANSITION CONDITIONS */
			if( gpioRead( keys_config[index].tecla ) )
			{
				keys_data[index].state = STATE_BUTTON_RISING;
			}
			break;

		case STATE_BUTTON_RISING:
			/* ENTRY */

			/* CHECK TRANSITION CONDITIONS */

			if( gpioRead( keys_config[index].tecla ) )
			{
				keys_data[index].state = STATE_BUTTON_UP;

				/* ACCION DEL EVENTO ! */
				buttonReleased( index );
			}
			else
			{
				keys_data[index].state = STATE_BUTTON_DOWN;
			}

			/* LEAVE */
			break;

		default:
			keys_ButtonError( index );
			break;
	}
}

/*=====[Implementations of private functions]================================*/

/* accion de el evento de tecla pulsada */
static void buttonPressed( uint32_t index )
{
	TickType_t current_tick_count = xTaskGetTickCount();

	taskENTER_CRITICAL();
	keys_data[index].time_down = current_tick_count;
	taskEXIT_CRITICAL();
}

/* accion de el evento de tecla liberada */
static void buttonReleased( uint32_t index )
{
	// Defino el puntero con el cual voy a enviar la direcci�n de la estructura global tiempo_led
	static t_TiempoLed * ptrTiempo_led;

	TickType_t current_tick_count = xTaskGetTickCount();

	taskENTER_CRITICAL();
	keys_data[index].time_up    = current_tick_count;
	keys_data[index].time_diff  = keys_data[index].time_up - keys_data[index].time_down;
	// Copio a estructura gloabl los valores leidos
	tiempo_led.indice = index;
	tiempo_led.tiempo_medido = keys_data[index].time_diff;
	// Defino la direccion que apunta el puntero a encolar.
	ptrTiempo_led = &tiempo_led;

	taskEXIT_CRITICAL();

	if (keys_data[index].time_diff > 0)
	{
		xQueueSend( queue_tec_pulsada , &ptrTiempo_led,  portMAX_DELAY  );
	}
}

static void keys_ButtonError( uint32_t index )
{
	taskENTER_CRITICAL();
	keys_data[index].state = BUTTON_UP;
	taskEXIT_CRITICAL();
}

/*=====[Tareas]=================================*/

void keys_Init( void )
{
	BaseType_t res;

	for(int i=0; i <sizeof(keys_data);i++)
	{
		keys_data[i].state          = BUTTON_UP;  // Set initial state
		keys_data[i].time_down      = KEYS_INVALID_TIME;
		keys_data[i].time_up        = KEYS_INVALID_TIME;
		keys_data[i].time_diff      = KEYS_INVALID_TIME;
	}

	// Crear tareas en freeRTOS
	res = xTaskCreate (
			  task_tecla,					// Funcion de la tarea a ejecutar
			  ( const char * )"task_tecla",	// Nombre de la tarea como String amigable para el usuario
			  configMINIMAL_STACK_SIZE*2,	// Cantidad de stack de la tarea
			  0,							// Parametros de tarea
			  tskIDLE_PRIORITY+1,			// Prioridad de la tarea
			  0							// Puntero a la tarea creada en el sistema
		  );

	// Gestión de errores
	configASSERT( (res) == pdPASS );

	uint32_t hola =sizeof(t_TiempoLed);
	queue_tec_pulsada = xQueueCreate( 1 , sizeof(t_TiempoLed *));

	if( queue_tec_pulsada== NULL)
	{
		gpioWrite( LED_ERROR , ON );
		printf( MSG_ERROR_QUEUE );
		while(TRUE);						// VER ESTE LINK: https://pbs.twimg.com/media/BafQje7CcAAN5en.jpg
	}
}

void leds_Init( void )
{
	BaseType_t res;

	// Crear tareas en freeRTOS
	res = xTaskCreate (
			  task_led,						// Funcion de la tarea a ejecutar
			  ( const char * )"task_led",	// Nombre de la tarea como String amigable para el usuario
			  configMINIMAL_STACK_SIZE*2,	// Cantidad de stack de la tarea
			  0,							// Parametros de tarea
			  tskIDLE_PRIORITY+1,			// Prioridad de la tarea
			  0								// Puntero a la tarea creada en el sistema
		  );

	// Gestión de errores
	configASSERT( (res) == pdPASS );
}

void task_tecla( void* taskParmPtr )
{
	while( 1 )
	{
		for(int i=0; i <key_count;i++)
		{
			keys_Update( i );
		}
		vTaskDelay( DEBOUNCE_TIME / portTICK_RATE_MS );
	}
}

void task_led( void* taskParmPtr )
{
    // ---------- CONFIGURACIONES ------------------------------
	TickType_t xPeriodicity =  MAX_RATE;
	TickType_t xLastWakeTime = xTaskGetTickCount();
	t_TiempoLed *pxTiempo;	// Puntero para leer la cola

	t_TiempoLed auxPx;
	auxPx.tiempo_medido = 0;
	auxPx.indice = 0;
    // ---------- REPETIR POR SIEMPRE --------------------------
	while( TRUE )
	{
		if (xQueueReceive( queue_tec_pulsada , &pxTiempo, 0) == pdPASS)
		{
			if (pxTiempo->tiempo_medido > xPeriodicity)
				auxPx.tiempo_medido = xPeriodicity;
			else auxPx.tiempo_medido = pxTiempo->tiempo_medido;

			auxPx.indice = pxTiempo->indice;
		}
		gpioWrite( leds[auxPx.indice]  , 1 );
		vTaskDelay( auxPx.tiempo_medido );
		gpioWrite( leds[auxPx.indice]  , 0 );

		vTaskDelayUntil( &xLastWakeTime , xPeriodicity );
	}
}
