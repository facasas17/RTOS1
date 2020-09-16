/*=============================================================================
 * Copyright (c) 2020, Fabiola de las Casas <fabioladelascasas@gmail.com>
 * All rights reserved.
 * License: Free
 * Date: 2020/09/16
 * Version: v1.1
 *===========================================================================*/

#ifndef POSGRADO_RTOS_C4_MODULAR_INC_FSM_DEBOUNCE_H_
#define POSGRADO_RTOS_C4_MODULAR_INC_FSM_DEBOUNCE_H_

/*==================[inclusiones]============================================*/
#include "sapi.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"

/*==================[definiciones y macros]==================================*/
#define DEBOUNCE_TIME 40

/*==================[definiciones de datos]=========================*/
typedef enum
{
    STATE_BUTTON_UP,
    STATE_BUTTON_DOWN,
    STATE_BUTTON_FALLING,
    STATE_BUTTON_RISING
} fsmButtonState_t;

/*==================[prototipos de funciones]====================*/
void fsmButtonError( void );
void fsmButtonInit( void );
void fsmButtonUpdate( gpioMap_t tecla );
void buttonPressed( void );
void buttonReleased( void );

TickType_t get_diff();
void clear_diff();

#endif /* POSGRADO_RTOS_C4_MODULAR_INC_FSM_DEBOUNCE_H_ */
