/*=============================================================================
 * Copyright (c) 2020, Fabiola de las Casas <fabioladelascasas@gmail.com>
 * All rights reserved.
 * License: Free
 * Date: 2020/10/14
 * Version: v1.1
 *===========================================================================*/

/*==================[ Inclusiones]============================================*/
#include "leds.h"

/*==================[ Definiciones de datos externos]=========================*/
extern t_key_data keys_data[N_KEYS];
extern QueueHandle_t  uart_queueLED1;
extern QueueHandle_t  uart_queueLED3;

/*==================[Funciones]====================*/
void task_led1( void* taskParmPtr )
{
	uint32_t duty_received;
    TickType_t duty_cycle   = INIT_DUTY;
    TickType_t xPeriodicity = LED_PERIOD;

    TickType_t xLastWakeTime = xTaskGetTickCount();

    while( TRUE )
    {
    	// Consulto sin delay por el semaforo de la TEC1
        if( xSemaphoreTake( keys_data[TEC2_INDEX].pressed_signal, 0 ) == pdPASS )
        {
        	if (duty_cycle >= LED_PERIOD)	// Verifico el duty_cycle actual para no saturar por encima del periodo
        		duty_cycle = LED_PERIOD;
        	else
        		duty_cycle = duty_cycle + pdMS_TO_TICKS(1);  // Incremento en 1ms el duty_cycle del LED1
        }
        // Consulto sin delay por el semaforo de la TEC0
        if( xSemaphoreTake( keys_data[TEC1_INDEX].pressed_signal, 0 ) == pdPASS )
        {
        	if (duty_cycle <= 0)	// Verifico el duty_cycle actual para no saturar
        		duty_cycle = 0;
        	else
        		duty_cycle = duty_cycle - pdMS_TO_TICKS(1);	// Decremento en 1ms el duty cycle
        }
        if (xQueueReceive( uart_queueLED1 , &duty_received, 0) == pdPASS)
        {
        	duty_cycle = pdMS_TO_TICKS(duty_received);
        }

        gpioWrite( LED1, ON );
        vTaskDelay( duty_cycle );
        gpioWrite( LED1, OFF );

        // Envia la tarea al estado bloqueado durante xPeriodicity (delay periodico de 20ms)
        vTaskDelayUntil( &xLastWakeTime, xPeriodicity );
    }
}

void task_led2( void* taskParmPtr )
{
	uint32_t duty_received;
    TickType_t duty_cycle   = INIT_DUTY;
    TickType_t xPeriodicity = LED_PERIOD;

    TickType_t xLastWakeTime = xTaskGetTickCount();

    while( TRUE )
    {
    	// Consulto sin delay por el semaforo de la TEC4
        if( xSemaphoreTake( keys_data[TEC4_INDEX].pressed_signal, 0 ) == pdPASS )
        {
        	if (duty_cycle >= LED_PERIOD)	// Verifico el duty_cycle actual para no saturar por encima del periodo
        		duty_cycle = LED_PERIOD;
        	else
        		duty_cycle = duty_cycle + pdMS_TO_TICKS(1);	// Incremento en 1ms el duty cycle

        }
        // Consulto sin delay por el semaforo de la TEC3
        if( xSemaphoreTake( keys_data[TEC3_INDEX].pressed_signal, 0 ) == pdPASS )
        {
        	if (duty_cycle <= 0)	// Verifico el duty_cycle actual para no saturar
        		duty_cycle = 0;
        	else
        		duty_cycle = duty_cycle - pdMS_TO_TICKS(1);	// Decremento en 1ms el duty cycle
        }
        if (xQueueReceive( uart_queueLED3 , &duty_received, 0) == pdPASS)
        {
        	duty_cycle = pdMS_TO_TICKS(duty_received);
        }
        else duty_cycle = duty_cycle;

        gpioWrite( LED3, ON );
        vTaskDelay( duty_cycle );
        gpioWrite( LED3, OFF );

        // Envia la tarea al estado bloqueado durante xPeriodicity (delay periodico de 20ms)
        vTaskDelayUntil( &xLastWakeTime, xPeriodicity );
    }
}

void leds_init( void )
{
	uint32_t i = 0;
	BaseType_t res;

	res = xTaskCreate (
			task_led1,						// Funcion de la tarea a ejecutar
			( const char * )"task_led1",	// Nombre de la tarea como String amigable para el usuario
			configMINIMAL_STACK_SIZE*2,		// Cantidad de stack de la tarea
			0,								// Parametros de tarea
			tskIDLE_PRIORITY+1,				// Prioridad de la tarea
			0								// Puntero a la tarea creada en el sistema
	   	   );

	// Gestión de errores
	configASSERT( res == pdPASS );

	res = xTaskCreate (
			task_led2,						// Funcion de la tarea a ejecutar
			( const char * )"task_led2",	// Nombre de la tarea como String amigable para el usuario
			configMINIMAL_STACK_SIZE*2,		// Cantidad de stack de la tarea
			0,								// Parametros de tarea
			tskIDLE_PRIORITY+1,				// Prioridad de la tarea
			0								// Puntero a la tarea creada en el sistema
	   	   );

	// Gestión de errores
	configASSERT( res == pdPASS );
}
