/*
 * leds.c
 *
 *  Created on: 8 oct. 2020
 *      Author: Fabiola De Las Casas
 */

/*==================[ Inclusions ]============================================*/
#include "FreeRTOS.h"
#include "task.h"

#include "leds.h"
#include "keys.h"
#include "sapi.h"

/*=====[ Definitions of private data types ]===================================*/

/*=====[Definition macros of private constants]==============================*/

/*=====[Prototypes (declarations) of private functions]======================*/

/*=====[Definitions of private global variables]=============================*/

/*=====[Definitions of public global variables]==============================*/

//t_Led leds[] = {{.led=LEDR, .indice=0}, {.led=LED1, .indice=1}, {.led=LED2, .indice=2}, {.led=LED3, .indice=3}};

/*=====[prototype of private functions]=================================*/

/*=====[Implementations of public functions]=================================*/

void led_init( void )
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
    configASSERT( res == pdPASS );
}

void task_led( void* taskParmPtr )
{
    TickType_t dif =   INIT_PERIOD;
    TickType_t xPeriodicity = TASK_PERIOD; // Tarea periodica cada TASK_PERIOD ms

    TickType_t xLastWakeTime = xTaskGetTickCount();

    while( 1 )
    {
        if( key_pressed( TEC1_INDEX ) )
            dif = get_diff(TEC1_INDEX);
        else
        	if( key_pressed( TEC2_INDEX ) )
        		dif = get_diff(TEC2_INDEX);
        	else
        		if( key_pressed( TEC3_INDEX ) )
        			dif = get_diff(TEC3_INDEX);
        		else
        			if( key_pressed( TEC4_INDEX ) )
        				dif = get_diff(TEC4_INDEX);

        gpioWrite( LEDB, ON );
        vTaskDelay( dif );
        gpioWrite( LEDB, OFF );

        // Envia la tarea al estado bloqueado durante xPeriodicity (delay periodico)
        vTaskDelayUntil( &xLastWakeTime, 2*dif );
    }
}
