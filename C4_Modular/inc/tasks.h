/*=============================================================================
 * Copyright (c) 2020, Fabiola de las Casas <fabioladelascasas@gmail.com>
 * All rights reserved.
 * License: Free
 * Date: 2020/09/16
 * Version: v1.1
 *===========================================================================*/

#ifndef POSGRADO_RTOS_C4_MODULAR_INC_TASKS_H_
#define POSGRADO_RTOS_C4_MODULAR_INC_TASKS_H_

/*==================[inclusiones]============================================*/
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "sapi.h"
#include "semphr.h"

#include "fsm_debounce.h"

/*==================[definiciones y macros]==================================*/
#define BUTTON_RATE_MS 		1
#define LED_RATE_MS			500
#define LED_WAIT_SEMPH_MS	1000

#define LED_RATE 		pdMS_TO_TICKS(LED_RATE_MS)
#define BUTTON_RATE 	pdMS_TO_TICKS(BUTTON_RATE_MS)
#define LED_WAIT_RATE	pdMS_TO_TICKS(LED_WAIT_SEMPH_MS)

/*==================[definiciones de datos]=========================*/

/*==================[prototipos de tareas]====================*/
void tarea_led( void* taskParmPtr );
void tarea_tecla( void* taskParmPtr );

#endif /* POSGRADO_RTOS_C4_MODULAR_INC_TASKS_H_ */
