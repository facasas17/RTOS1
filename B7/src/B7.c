/*=============================================================================
 * Copyright (c) 2020, Fabiola de las Casas <fabioladelascasas@gmail.com>
 * All rights reserved.
 * License: Free
 * Date: 2020/09/10
 * Version: v1.1
 *===========================================================================*/

/*=====[Inclusions of function dependencies]=================================*/

#include "FreeRTOS.h"
#include "task.h"
#include "FreeRTOSConfig.h"

#include "sapi.h"
#include "fsm_debounce.h"

/*=====[Definition & macros of public constants]==============================*/

#define RATE 40
#define LED_RATE pdMS_TO_TICKS(RATE)    // Converts a time in milliseconds to a time in ticks.
#define BUTTON_RATE 1

DEBUG_PRINT_ENABLE;

/*=====[Definitions of extern global functions]==============================*/

// Prototipo de funcion de la tarea
void task_led( void* taskParmPtr );
void task_tecla( void* taskParmPtr );

/*=====[Main function, program entry point after power on or reset]==========*/

int main(void)
{
    // ---------- CONFIGURACIONES ------------------------------
	boardConfig();									// Inicializar y configurar la plataforma

	debugPrintConfigUart( UART_USB, 115200 );		// UART for debug messages

	// Crear tareas en freeRTOS
	BaseType_t resA =
	xTaskCreate (
				task_led,					// Funcion de la tarea a ejecutar
				( const char *)"task_led",	// Nombre de la tarea como String amigable para el usuario
				configMINIMAL_STACK_SIZE*2,	// Cantidad de stack de la tarea
				0,							// Parametros de tarea
				tskIDLE_PRIORITY+1,			// Prioridad de la tarea
				0							// Puntero a la tarea creada en el sistema
	);

	// Crear tareas en freeRTOS
	BaseType_t resB =
	xTaskCreate (
				task_tecla,					// Funcion de la tarea a ejecutar
				( const char *)"task_tecla",	// Nombre de la tarea como String amigable para el usuario
				configMINIMAL_STACK_SIZE*2,	// Cantidad de stack de la tarea
				0,							// Parametros de tarea
				tskIDLE_PRIORITY+1,			// Prioridad de la tarea
				0							// Puntero a la tarea creada en el sistema
	);

	// Gestión de errores
	if( (resA || resB) == pdFAIL )
	{
		gpioWrite(LEDR, ON);
		printf("Error al crear las tareas.\r\n");
		while(1);

	}

    // Iniciar scheduler
    vTaskStartScheduler();					// Enciende tick | Crea idle y pone en ready | Evalua las tareas creadas | Prioridad mas alta pasa a running

    while(1)
    {

    }
    return 0;
}

void task_tecla( void* taskParmPtr )
{
	while(1)
	{
		fsmButtonUpdate( TEC1 );
	 	vTaskDelay( BUTTON_RATE / portTICK_RATE_MS );
	}
}

void task_led( void* taskParmPtr )
{
    TickType_t xPeriodicity =  1000 / portTICK_RATE_MS;			// Tarea periodica cada 1000 ms

    TickType_t xLastWakeTime = xTaskGetTickCount();

	while(1)
	{
		TickType_t dif = get_diff();

		if( dif == 0 )
		{
			vTaskDelay( LED_RATE / portTICK_RATE_MS);
		}
		else
		{
			gpioWrite( LEDB, ON );
			vTaskDelay( dif );
			gpioWrite( LEDB, OFF);

	        // Envia la tarea al estado bloqueado durante xPeriodicity (delay periodico)
	        vTaskDelayUntil( &xLastWakeTime , xPeriodicity );
		}
	}
}


