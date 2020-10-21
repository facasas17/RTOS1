/* Copyright 2020, Franco Bucafusco
 * All rights reserved.
 *
 * This file is part of sAPI Library.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/*==================[ Inclusiones]============================================*/
#include "protocol.h"

/*==================[ Definiciones de datos internos]=========================*/

uartMap_t         uart_used;
SemaphoreHandle_t new_frame_signal;
SemaphoreHandle_t mutex;

char buffer[FRAME_MAX_SIZE];
uint16_t index;

/*==================[ Definiciones de datos externos]=========================*/
extern QueueHandle_t  uart_queueLED1;
extern QueueHandle_t  uart_queueLED3;

void protocol_rx_event( void *noUsado  )
{
	( void* ) noUsado;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    /* leemos el caracter recibido */
    char c = uartRxRead( UART_USB );

    BaseType_t signaled = xSemaphoreTakeFromISR( mutex , &xHigherPriorityTaskWoken );

    if( signaled )
    {
        if( FRAME_MAX_SIZE-1==index )
        {
            /* reinicio el paquete */
            index = 0;
        }

        if( c=='[' )
        {
            if( index!=0 )
            {
                /* 1er byte del frame*/
            	index = 0;
            }
            buffer[index] = c;
            /* incremento el indice */
            index++;
        }
        else if( c==']' )
        {
            /* solo cierro el fin de frame si al menos se recibio un start.*/
            if( index>=1 )
            {
                /* se termino el paquete - guardo el dato */
                buffer[index] = c;

                /* incremento el indice */
                index++;

                /* Deshabilito todas las interrupciones de UART_USB */
                uartCallbackClr( uart_used, UART_RECEIVE );

                /* seÃ±alizo a la aplicacion */
                xSemaphoreGiveFromISR( new_frame_signal, &xHigherPriorityTaskWoken );
            }

        }
        else
        {
            /* Sigo leyendo */
            if( index>=1 )
            {
                /* guardo el dato */
                buffer[index] = c;

                /* incremento el indice */
                index++;
            }
        }
        xSemaphoreGiveFromISR( mutex, &xHigherPriorityTaskWoken );
    }
    portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
}

void uart_init( uartMap_t uart, uint32_t baudRate )
{
    /* CONFIGURO EL DRIVER */

    uart_used = uart;

    /* Inicializar la UART_USB junto con las interrupciones de Tx y Rx */
    uartConfig( uart, baudRate );

    /* Seteo un callback al evento de recepcion y habilito su interrupcion */
    uartCallbackSet( uart, UART_RECEIVE, protocol_rx_event, NULL );

    /* Habilito todas las interrupciones de UART_USB */
    uartInterrupt( uart, true );

    /* CONFIGURO LA PARTE LOGICA */
    index = 0;
    new_frame_signal = xSemaphoreCreateBinary();
    mutex = xSemaphoreCreateMutex();
}

void protocol_wait_frame()
{
    xSemaphoreTake( new_frame_signal, portMAX_DELAY );
    xSemaphoreTake( mutex, 0 );
}

void  protocol_get_frame_ref( char** data, uint16_t* size )
{
    *data = buffer;
    *size = index;
}

void protocol_discard_frame()
{
    /* indico que se puede inciar un paquete nuevo */
    index = 0;

    /* libero la seccion critica, para que el handler permita ejecutarse */
    xSemaphoreGive( mutex );

    /* limpio cualquier interrupcion que hay ocurrido durante el procesamiento */
    uartClearPendingInterrupt( uart_used );

    /* habilito isr rx  de UART_USB */
    uartCallbackSet( uart_used, UART_RECEIVE, protocol_rx_event, NULL );
}

void task_uart( void* pvParameters )
{
    char* data;
    uint16_t size;
    uint32_t  duty;

    while( TRUE )
    {
        protocol_wait_frame();

        protocol_get_frame_ref( &data, &size );

        protocol_discard_frame();

        if( data[2] == 'A' )	// Chequeo si me llego una A y seteo el duty al máximo de 20ms
        	duty = 20;
        else
        	duty = (data[2]-'0')*2;	// Para pasar de % a ms
        if( data[1] == '0' )
        	xQueueSend(uart_queueLED1, &duty, portMAX_DELAY);
        else if( data[1] == '1' )
        	xQueueSend(uart_queueLED3, &duty, portMAX_DELAY);

    }
}

void taskUart_init()
{
	BaseType_t res;

	uart_init( UART_USB, BAUD_RATE );

	res = xTaskCreate(
			   task_uart,                  // Funcion de la tarea a ejecutar
			   ( const char * )"task_uart", // Nombre de la tarea como String amigable para el usuario
			   configMINIMAL_STACK_SIZE*2,   // Cantidad de stack de la tarea
			   0,                            // Parametros de tarea
			   tskIDLE_PRIORITY+1,           // Prioridad de la tarea
			   0                             // Puntero a la tarea creada en el sistema
	   	   );
	// Gestión de errores
	configASSERT( res == pdPASS );
}
