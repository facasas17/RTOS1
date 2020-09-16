/*=============================================================================
 * Copyright (c) 2020, Fabiola de las Casas <fabioladelascasas@gmail.com>
 * All rights reserved.
 * License: Free
 * Date: 2020/09/16
 * Version: v1.1
 *===========================================================================*/

/*==================[inclusiones]============================================*/
#include "tasks.h"
/*==================[definiciones y macros]==================================*/

/*==================[definiciones de datos internos]=========================*/

/*==================[definiciones de datos externos]=========================*/
extern SemaphoreHandle_t sem_tec_pulsada;

/*==================[declaraciones de funciones internas]====================*/

/*==================[declaraciones de funciones externas]====================*/

/*==================[tareas]====================*/

void tarea_tecla( void* taskParmPtr )
{
	fsmButtonInit();

	while( TRUE )
	{
		fsmButtonUpdate( TEC1 );
	 	vTaskDelay( BUTTON_RATE );
	}
}

// Implementacion de funcion de la tarea
void tarea_led( void* taskParmPtr )
{
    // ---------- CONFIGURACIONES ------------------------------

    // ---------- REPETIR POR SIEMPRE --------------------------
    while( TRUE )
    {
    	if ( xSemaphoreTake( sem_tec_pulsada , LED_WAIT_RATE ) )
    	{
    		gpioWrite( LEDG , ON );
    		vTaskDelay( LED_RATE );
    		gpioWrite( LEDG , OFF );
    	}
    	else
    	{
    		gpioWrite( LEDR , ON );
    		vTaskDelay( LED_RATE );
    		gpioWrite( LEDR , OFF );
    	}
    				// Esperamos 1seg tecla

    	TickType_t dif = get_diff();
    	clear_diff();
    }
}
