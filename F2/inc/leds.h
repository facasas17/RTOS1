/*
 * leds.h
 *
 *  Created on: 8 oct. 2020
 *      Author: Fabiola De Las Casas
 */

#ifndef POSGRADO_RTOS_F2_INC_LEDS_H_
#define POSGRADO_RTOS_F2_INC_LEDS_H_

#include "FreeRTOS.h"
#include "semphr.h"
#include "sapi.h"

/* public macros ================================================================= */
#define INIT_PERIOD pdMS_TO_TICKS( 500 )	// 500ms en Ticks
#define TASK_PERIOD pdMS_TO_TICKS( 1000 )	// 1000ms en Ticks

/* types ================================================================= */
/*typedef struct
{
	gpioMap_t led;
	uint32_t indice;
}t_Led;
*/
/* methods ================================================================= */
// Prototipo de funcion de la tarea
void task_led( void* taskParmPtr );

void led_init( void );



#endif /* POSGRADO_RTOS_F2_INC_LEDS_H_ */
