/*=============================================================================
 * Copyright (c) 2020, Fabiola de las Casas <fabioladelascasas@gmail.com>
 * All rights reserved.
 * License: Free
 * Date: 2020/09/10
 * Version: v1.1
 *===========================================================================*/

/*==================[ Inclusions ]============================================*/

#include "sapi.h"
#include "FreeRTOS.h"

/*=====[ Definitions of private data types ]===================================*/

typedef enum
{
    STATE_BUTTON_UP,
    STATE_BUTTON_DOWN,
    STATE_BUTTON_FALLING,
    STATE_BUTTON_RISING
} fsmButtonState_t;

/*=====[Definition macros of private constants]==============================*/

#define DEBOUNCE_TIME 40

/*=====[Prototypes (declarations) of private functions]======================*/

static void fsmButtonError( void );
static void buttonPressed( void );
static void buttonReleased( void );

/*=====[Definitions of private global variables]=============================*/

static TickType_t time_down;
static TickType_t time_up;

/*=====[Definitions of public global variables]==============================*/

fsmButtonState_t fsmButtonState;
TickType_t time_diff;

/*=====[Implementations of public functions]=================================*/

TickType_t get_diff()
{
	return time_diff;
}

void clear_diff()
{
	time_diff = 0;
}

void fsmButtonInit( void )
{
    fsmButtonState = BUTTON_UP;  // Set initial state
}


// FSM Update State Function
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

/*=====[Implementations of private functions]================================*/

/* accion de el evento de tecla pulsada */
static void buttonPressed( void )
{
	time_down = xTaskGetTickCount();
}

/* accion de el evento de tecla liberada */
static void buttonReleased( void )
{
	time_up = xTaskGetTickCount();
	time_diff = time_up - time_down;
}

static void fsmButtonError( void )
{
    fsmButtonState = BUTTON_UP;
}

