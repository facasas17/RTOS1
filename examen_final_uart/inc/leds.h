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

#define LED_PERIOD pdMS_TO_TICKS( 20 )	// 20ms en Ticks
#define INIT_DUTY  pdMS_TO_TICKS( 10 )  // Duty cycle inicial de 50%


/*==================[ Definiciones de funciones publicas]=========================*/

void task_led1( void* taskParmPtr );
void task_led2( void* taskParmPtr );
void leds_init( void );


#endif /* LEDS_H_ */
