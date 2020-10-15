/*=============================================================================
 * Copyright (c) 2020, Fabiola de las Casas <fabioladelascasas@gmail.com>
 * All rights reserved.
 * License: Free
 * Date: 2020/10/14
 * Version: v1.1
 *===========================================================================*/
#ifndef LEDS_H_
#define LEDS_H_

/*==================[ Inclusiones]============================================*/
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"

#include "sapi.h"

#include "keys.h"

/*==================[ Definiciones de datos ]=========================*/

#define INIT_PERIOD pdMS_TO_TICKS( 500 )	// 500ms en Ticks
#define TASK_PERIOD pdMS_TO_TICKS( 1000 )	// 1000ms en Ticks


/*==================[ Definiciones de funciones publicas]=========================*/

void task_led( void* taskParmPtr );
void leds_init( void );





#endif /* POSGRADO_RTOS_V2_MULTILED_KEYS_SEMPHR_INC_LEDS_H_ */
