/*=============================================================================
 * Copyright (c) 2020, Fabiola de las Casas <fabioladelascasas@gmail.com>
 * All rights reserved.
 * License: Free
 * Date: 2020/10/14
 * Version: v1.1
 *===========================================================================*/

#ifndef KEYS_H_
#define KEYS_H_

/*==================[ Inclusiones]============================================*/
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"

#include "sapi.h"

/*==================[ Definiciones de datos ]=========================*/
#define N_KEYS   4		// Cantidad de teclas a usar

#define KEYS_INVALID_TIME   -1

#define TEC1_INDEX  0
#define TEC2_INDEX  1
#define TEC3_INDEX  2
#define TEC4_INDEX  3

#define DEBOUNCE_TIME_MS	40
#define DEBOUNCE_TIME		DEBOUNCE_TIME_MS/portTICK_RATE_MS

/*==================[ Definiciones de estructuras ]=========================*/

typedef struct
{
    TickType_t time_down;				//timestamp of the last High to Low transition of the key
    TickType_t time_up;		    		//timestamp of the last Low to High transition of the key
    TickType_t time_diff;	    		//variables

    SemaphoreHandle_t isr_signal;   	//almacenara el handle del semaforo creado para una cierta tecla
    SemaphoreHandle_t pressed_signal;	// Semaforo para indicar que se presiono y libero una tecla

} t_key_data;

/*==================[ Prototipos de funciones publicas ]======================*/

TickType_t get_diff( uint32_t index );
void keys_Init( void );
void keys_Update_Isr( uint32_t index );
void task_tecla( void* taskParmPtr );

void keys_isr_fall( uint32_t index );
void keys_isr_rise( uint32_t index );

#endif /* KEYS_H_ */
