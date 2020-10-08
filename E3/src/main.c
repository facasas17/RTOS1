/*=============================================================================
 * Copyright (c) 2020, Fabiola de las Casas <fabioladelascasas@gmail.com>
 * All rights reserved.
 * License: Free
 * Date: 2020/09/30
 * Version: v1.1
 *===========================================================================*/

/*==================[inclusiones]============================================*/
#include "main.h"

/*==================[definiciones de datos externos]=========================*/
DEBUG_PRINT_ENABLE;

/*==================[funcion principal]======================================*/

// FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE ENCENDIDO O RESET.
int main( void )
{
    // ---------- CONFIGURACIONES ------------------------------

	boardConfig();								// Inicializar y configurar la plataforma

	debugPrintConfigUart( UART, BAUD_RATE );	// UART for debug messages
	printf( MSG_WELCOME );

	keys_Init();								// Inicializar estructura de datos
	leds_Init();

    // Iniciar scheduler
    vTaskStartScheduler();						// Enciende tick | Crea idle y pone en ready | Evalua las tareas creadas | Prioridad mas alta pasa a running

    // ---------- REPETIR POR SIEMPRE --------------------------
    while( TRUE )
    {
        // Si cae en este while 1 significa que no pudo iniciar el scheduler
    }
    return TRUE;
}
/*==================[fin del archivo]========================================*/
