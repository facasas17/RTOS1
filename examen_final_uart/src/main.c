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
#include "leds.h"

#include "sapi.h"
#include "protocol.h"

/*==================[ Definiciones de datos globales]=========================*/
QueueHandle_t  uart_queueLED1;	// Cola para pasar el duty ingresado por UART para el LED1
QueueHandle_t  uart_queueLED3;	// Cola para pasar el duty ingresado por UART para el LED3

/*==================[ Funcion Principal]====================*/

int main( void )
{
   // ---------- CONFIGURACIONES ------------------------------
   boardConfig();  // Inicializar y configurar la plataforma

   /* Creo las colas y gestiono errores */
   uart_queueLED1 = xQueueCreate(1, sizeof(uint32_t));
   configASSERT( uart_queueLED1 != NULL );

   uart_queueLED3 = xQueueCreate(1, sizeof(uint32_t));
   configASSERT( uart_queueLED3 != NULL );

   /* Inicializo uart y creo la tarea */
   taskUart_init();

   /* inicializo driver de teclas */
   keys_Init();

   /* Inicializo driver de leds */
   leds_init();

   // Iniciar scheduler
   vTaskStartScheduler();					// Enciende tick | Crea idle y pone en ready | Evalua las tareas creadas | Prioridad mas alta pasa a running

   /* realizar un assert con "false" es equivalente al while(1) */
   configASSERT( 0 );
   return 0;
}


/* hook que se ejecuta si al necesitar un objeto dinamico, no hay memoria disponible */
void vApplicationMallocFailedHook()
{
    printf( "Malloc Failed Hook!\n" );
    configASSERT( 0 );
}
