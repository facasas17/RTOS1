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

#ifndef KEYS_H_
#define KEYS_H_

#include "FreeRTOS.h"
#include "semphr.h"
#include "sapi.h"

/* public macros ================================================================= */
#define KEYS_INVALID_TIME   -1

#define KEYS_USE_ISR        1

#define TEC1_INDEX  0
#define TEC2_INDEX  1
#define TEC3_INDEX  2
#define TEC4_INDEX  3

#define DEBOUNCE_TIME   40

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
    uint32_t indice;
} t_key_config;

typedef struct
{
    keys_ButtonState_t state;   //variables
    TickType_t time_down;		//timestamp of the last High to Low transition of the key
    TickType_t time_up;		    //timestamp of the last Low to High transition of the key
    TickType_t time_diff;	    //variables

#if KEYS_USE_ISR==1
    SemaphoreHandle_t isr_signal;   //almacenara el handle del semaforo creado para una cierta tecla
#endif

    SemaphoreHandle_t pressed_signal;

} t_key_data;

/* methods ================================================================= */
void keys_Init( void );
TickType_t get_diff(uint32_t index);
void clear_diff(uint32_t index);
int key_pressed( uint32_t index );

// Prototipo de funcion de la tarea
void task_tecla( void* taskParmPtr );

#endif /* PDM_ANTIRREBOTE_MEF_INC_DEBOUNCE_H_ */
