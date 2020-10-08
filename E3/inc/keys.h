/*=============================================================================
 * Copyright (c) 2020, Fabiola de las Casas <fabioladelascasas@gmail.com>
 * All rights reserved.
 * License: Free
 * Date: 2020/09/30
 * Version: v1.1
 *===========================================================================*/

#ifndef KEYS_H_
#define KEYS_H_

#include "FreeRTOS.h"
#include "semphr.h"
#include "sapi.h"

/* public macros ================================================================= */
#define KEYS_INVALID_TIME   -1
#define LED_ERROR 		LEDR
#define MAX_SEM 3

#define PRIORITY 1
#define SIZE     1
#define STR_AUX  20

#define MAX_RATE_MS 3000
#define MAX_RATE pdMS_TO_TICKS(MAX_RATE_MS)
#define MSG_ERROR_TASK   "Error al crear las tareas. \n\r"
#define MSG_ERROR_SEM	 "Error al crear los semaforos.\r\n"
#define MSG_ERROR_QUEUE	 "Error al crear las colas.\r\n"

/* types ================================================================= */
typedef enum
{
	STATE_BUTTON_UP,
	STATE_BUTTON_DOWN,
	STATE_BUTTON_FALLING,
	STATE_BUTTON_RISING
} keys_ButtonState_t;

typedef struct
{
	gpioMap_t tecla;			//config
} t_key_config;

typedef struct
{
	keys_ButtonState_t state;   //variables

	TickType_t time_down;		//timestamp of the last High to Low transition of the key
	TickType_t time_up;		    //timestamp of the last Low to High transition of the key
	TickType_t time_diff;	    //variables
} t_key_data;

typedef struct
{
	TickType_t 	tiempo_medido;
	uint32_t 	indice;
} t_TiempoLed;

/* methods ================================================================= */
void keys_Init( void );
TickType_t get_diff();
void clear_diff();
void tecla_led_init(void);
void leds_Init( void );

#endif /* PDM_ANTIRREBOTE_MEF_INC_DEBOUNCE_H_ */
