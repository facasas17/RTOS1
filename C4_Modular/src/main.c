/*=============================================================================
 * Copyright (c) 2020, Fabiola de las Casas <fabioladelascasas@gmail.com>
 * All rights reserved.
 * License: Free
 * Date: 2020/09/16
 * Version: v1.1
 *===========================================================================*/

/*==================[inclusiones]============================================*/

#include "main.h"

/*==================[definiciones y macros]==================================*/

/*==================[definiciones de datos internos]=========================*/
SemaphoreHandle_t sem_tec_pulsada;

/*==================[definiciones de datos externos]=========================*/

DEBUG_PRINT_ENABLE;

/*==================[declaraciones de funciones internas]====================*/

/*==================[declaraciones de funciones externas]====================*/

/*==================[funcion principal]======================================*/

// FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE ENCENDIDO O RESET.
int main( void )
{
    // ---------- CONFIGURACIONES ------------------------------
	boardConfig();									// Inicializar y configurar la plataforma

	debugPrintConfigUart( UART_USB, BAUD_RATE );		// UART for debug messages
	printf( MSG_WELCOME );

	crear_tarea(tarea_led, "tarea_led", SIZE, 0, PRIORITY, 0);
	crear_tarea(tarea_tecla, "tartarea_teclaea_led", SIZE, 0, PRIORITY, 0);

	crear_semaforo(&sem_tec_pulsada);

    // Iniciar scheduler
    vTaskStartScheduler();					// Enciende tick | Crea idle y pone en ready | Evalua las tareas creadas | Prioridad mas alta pasa a running

    // ---------- REPETIR POR SIEMPRE --------------------------
    while( TRUE )
    {
        // Si cae en este while 1 significa que no pudo iniciar el scheduler
    }

    // NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa se ejecuta
    // directamenteno sobre un microcontroladore y no es llamado por ningun
    // Sistema Operativo, como en el caso de un programa para PC.
    return 0;
}

/*==================[definiciones de funciones internas]=====================*/

/*==================[definiciones de funciones externas]=====================*/

/*==================[fin del archivo]========================================*/
