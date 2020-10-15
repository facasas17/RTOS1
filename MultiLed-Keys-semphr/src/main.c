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

#include "keys.h"
#include "leds.h"

#include "sapi.h"

/*==================[ Funcion Principal]====================*/

int main( void )
{
   uint32_t i;
   BaseType_t res;

   // ---------- CONFIGURACIONES ------------------------------
   boardConfig();  // Inicializar y configurar la plataforma

   /* inicializo driver de leds */
   leds_init();

   /* inicializo driver de teclas */
   keys_Init();

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
