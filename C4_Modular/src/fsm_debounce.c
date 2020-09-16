/*=============================================================================
 * Copyright (c) 2020, Fabiola de las Casas <fabioladelascasas@gmail.com>
 * All rights reserved.
 * License: Free
 * Date: 2020/09/16
 * Version: v1.1
 *===========================================================================*/

/*==================[inclusiones]============================================*/
#include "fsm_debounce.h"

/*==================[definiciones y macros]==================================*/

/*==================[definiciones de datos internos]=========================*/
fsmButtonState_t fsmButtonState;

TickType_t tiempo_down;
TickType_t tiempo_up;
TickType_t tiempo_diff;

/*==================[definiciones de datos externos]=========================*/
extern SemaphoreHandle_t sem_tec_pulsada;

/*==================[declaraciones de funciones internas]====================*/

/*==================[declaraciones de funciones externas]====================*/

/*==================[funciones]====================*/

TickType_t get_diff()
{
	return tiempo_diff;
}

void clear_diff()
{
	tiempo_diff = 0;
}

/* accion de el evento de tecla pulsada */
void buttonPressed( void )
{
	tiempo_down = xTaskGetTickCount();
}

/* accion de el evento de tecla liberada */
void buttonReleased( void )
{
	tiempo_up = xTaskGetTickCount();
	tiempo_diff = tiempo_up - tiempo_down;
	if (tiempo_diff > 0)
	{
		xSemaphoreGive(  sem_tec_pulsada );
	}
}

void fsmButtonError( void )
{
    fsmButtonState = BUTTON_UP;
}

void fsmButtonInit( void )
{
    fsmButtonState = BUTTON_UP;  // Set initial state
}

// FSM Update Sate Function
void fsmButtonUpdate( gpioMap_t tecla )
{
    static uint8_t contFalling = 0;
    static uint8_t contRising = 0;

    switch( fsmButtonState )
    {
        case STATE_BUTTON_UP:
            /* CHECK TRANSITION CONDITIONS */
            if( !gpioRead( tecla ) )
            {
                fsmButtonState = STATE_BUTTON_FALLING;
            }
            break;

        case STATE_BUTTON_FALLING:
            /* ENTRY */

            /* CHECK TRANSITION CONDITIONS */
            if( contFalling >= DEBOUNCE_TIME )
            {
                if( !gpioRead( tecla ) )
                {
                    fsmButtonState = STATE_BUTTON_DOWN;

                    /* ACCION DEL EVENTO !*/
                    buttonPressed();
                }
                else
                {
                    fsmButtonState = STATE_BUTTON_UP;
                }

                contFalling = 0;
            }

            contFalling++;

            /* LEAVE */
            break;

        case STATE_BUTTON_DOWN:
			/* CHECK TRANSITION CONDITIONS */
			if( gpioRead( tecla ) )
			{
				fsmButtonState = STATE_BUTTON_RISING;
			}
			break;

        case STATE_BUTTON_RISING:
            /* ENTRY */

            /* CHECK TRANSITION CONDITIONS */

            if( contRising >= DEBOUNCE_TIME )
            {
                if( gpioRead( tecla ) )
                {
                    fsmButtonState = STATE_BUTTON_UP;

                    /* ACCION DEL EVENTO ! */
                    buttonReleased();
                }
                else
                {
                    fsmButtonState = STATE_BUTTON_DOWN;
                }
                contRising = 0;
            }
            contRising++;

            /* LEAVE */
            break;

        default:
            fsmButtonError();
            break;
    }
}
