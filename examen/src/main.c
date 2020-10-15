/*=============================================================================
 * Copyright (c) 2020, Fabiola de las Casas <fabioladelascasas@gmail.com>
 * All rights reserved.
 * License: Free
 * Date: 2020/10/14
 * Version: v1.1
 *===========================================================================*/

/*==================[ Inclusiones]============================================*/
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "keys.h"

#include "sapi.h"

extern t_key_data keys_data[N_KEYS];

SemaphoreHandle_t mutex_uart;

void task_uart(void * taskParmPtr);
/*==================[ Funcion Principal]====================*/

int main( void )
{
   uint32_t i;
   BaseType_t res;

   // ---------- CONFIGURACIONES ------------------------------
   boardConfig();  // Inicializar y configurar la plataforma
   uartConfig( UART_USB, 115200 );

   mutex_uart = xSemaphoreCreateMutex();
   xSemaphoreGive(mutex_uart);

   // Crear tareas en freeRTOS
   res = xTaskCreate (
		   	   	   task_uart,              		// Funcion de la tarea a ejecutar
				   ( const char * )"task_uart",   	// Nombre de la tarea como String amigable para el usuario
				   configMINIMAL_STACK_SIZE*2, 		// Cantidad de stack de la tarea
				   0,                    	// Parametros de tarea
				   tskIDLE_PRIORITY+1,         		// Prioridad de la tarea
				   0                     			// Puntero a la tarea creada en el sistema
   	   	   );
   // Gestión de errores
   configASSERT( res == pdPASS );

   /* inicializo driver de teclas */
   keys_Init();

   // Iniciar scheduler
   vTaskStartScheduler();					// Enciende tick | Crea idle y pone en ready | Evalua las tareas creadas | Prioridad mas alta pasa a running

   /* realizar un assert con "false" es equivalente al while(1) */
   configASSERT( 0 );
   return 0;
}

void task_uart(void * taskParmPtr)
{
	TickType_t xLastWakeTime = xTaskGetTickCount();

	uint32_t time_dif = 0;

	while(TRUE)
	{
		char data[20];
		/* Espero la señal del semaforo de ambas tecladas presionadas */
		xSemaphoreTake(keys_data[0].pressed_signal, portMAX_DELAY);
		xSemaphoreTake(keys_data[1].pressed_signal, portMAX_DELAY);

		if( keys_data[0].time_down > keys_data[1].time_down)
		{
			time_dif = (keys_data[1].time_down - keys_data[0].time_down);//*portTICK_RATE_MS ;
			sprintf(data, "{0:1:%u}\n\r",time_dif );
		}
		else
		{
			time_dif = (keys_data[0].time_down - keys_data[1].time_down);//*portTICK_RATE_MS ;
			sprintf(&data[0], "{1:0:%u}\n\r",time_dif );
		}

		xSemaphoreTake(mutex_uart, portMAX_DELAY);
		printf("%s", data);
		xSemaphoreGive(mutex_uart);

	    gpioWrite( LED1, ON );
	    vTaskDelay( 500 );
	    gpioWrite( LED1, OFF );

	    vTaskDelayUntil( &xLastWakeTime, 1000 );
	}

}


/* hook que se ejecuta si al necesitar un objeto dinamico, no hay memoria disponible */
void vApplicationMallocFailedHook()
{
    printf( "Malloc Failed Hook!\n" );
    configASSERT( 0 );
}
