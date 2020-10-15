/*=============================================================================
 * Copyright (c) 2020, Fabiola de las Casas <fabioladelascasas@gmail.com>
 * All rights reserved.
 * License: Free
 * Date: 2020/10/14
 * Version: v1.1
 *===========================================================================*/

/*==================[ Inclusiones]============================================*/
#include "leds.h"

/*==================[ Definiciones de datos internos]=========================*/

const gpioMap_t leds[] = { LEDB, LED1, LED2, LED3};

#define N_LEDS   sizeof(leds)/sizeof(leds[0])

/*==================[ Definiciones de datos externos]=========================*/
extern t_key_data keys_data[N_KEYS];

/*==================[Funciones]====================*/
void task_led( void* taskParmPtr )
{
    uint32_t index = (uint32_t) taskParmPtr;
    TickType_t dif =   INIT_PERIOD;
    TickType_t xPeriodicity = TASK_PERIOD; // Tarea periodica cada 1000 ms

    TickType_t xLastWakeTime = xTaskGetTickCount();

    while( TRUE )
    {
        if( xSemaphoreTake( keys_data[index].pressed_signal, 0 ) == pdPASS )
        {
            dif = get_diff( index );
        }

        gpioWrite( leds[index], ON );
        vTaskDelay( dif );
        gpioWrite( leds[index], OFF );

        // Envia la tarea al estado bloqueado durante xPeriodicity (delay periodico)
        vTaskDelayUntil( &xLastWakeTime, 2*dif );
    }
}

void leds_init( void )
{
	uint32_t i = 0;
	BaseType_t res;

	for( i = 0 ; i < N_LEDS ; i++ ) {
	   // Crear tareas en freeRTOS
	   res = xTaskCreate (
	      task_led,						// Funcion de la tarea a ejecutar
	      ( const char * )"task_led",	// Nombre de la tarea como String amigable para el usuario
	      configMINIMAL_STACK_SIZE*2,	// Cantidad de stack de la tarea
	      (void*)i,						// Parametros de tarea
	      tskIDLE_PRIORITY+1,			// Prioridad de la tarea
	      0								// Puntero a la tarea creada en el sistema
	   );

	   // Gestión de errores
	   configASSERT( res == pdPASS );
	}
}
