/*=============================================================================
 * Copyright (c) 2020, Fabiola de las Casas <fabioladelascasas@gmail.com>
 * All rights reserved.
 * License: Free
 * Date: 2020/09/16
 * Version: v1.1
 *===========================================================================*/

/*==================[inclusiones]============================================*/
#include "config_tasks.h"

/*==================[definiciones y macros]==================================*/

/*==================[definiciones de datos internos]=========================*/

/*==================[definiciones de datos externos]=========================*/

/*==================[declaraciones de funciones internas]====================*/

/*==================[declaraciones de funciones externas]====================*/

/*==================[funciones]====================*/
void crear_tarea(  TaskFunction_t tarea, const char * const nom_tarea, uint8_t stack_size, void * const param, uint8_t prioridad, TaskHandle_t * const puntero )
{
    BaseType_t res_A =
    		xTaskCreate(
    				tarea,                     				// Funcion de la tarea a ejecutar
					( const char * )nom_tarea,  			// Nombre de la tarea como String amigable para el usuario
					configMINIMAL_STACK_SIZE*stack_size, 	// Cantidad de stack de la tarea
					param,                      			// Parametros de tarea
					tskIDLE_PRIORITY+prioridad,         	// Prioridad de la tarea
					puntero                     			// Puntero a la tarea creada en el sistema
    		);

    // Gestion de errores de tareas
	if(res_A == pdFAIL)
	{
		gpioWrite( LEDR, ON );
		printf( MSG_ERROR_TASK );
		while(TRUE);						// VER ESTE LINK: https://pbs.twimg.com/media/BafQje7CcAAN5en.jpg
	}
}


void crear_semaforo( SemaphoreHandle_t *semaforo )
{
	*semaforo = xSemaphoreCreateBinary();

	// Gestion de errores de semaforos
	if( *semaforo == NULL)
	{
		gpioWrite( LEDR, ON );
		printf( MSG_ERROR_SEMPHR );
		while(TRUE);						// VER ESTE LINK: https://pbs.twimg.com/media/BafQje7CcAAN5en.jpg
	}
}
