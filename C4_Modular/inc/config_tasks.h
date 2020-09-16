/*=============================================================================
 * Copyright (c) 2020, Fabiola de las Casas <fabioladelascasas@gmail.com>
 * All rights reserved.
 * License: Free
 * Date: 2020/09/16
 * Version: v1.1
 *===========================================================================*/

#ifndef POSGRADO_RTOS_C4_MODULAR_INC_CONFIG_TASKS_H_
#define POSGRADO_RTOS_C4_MODULAR_INC_CONFIG_TASKS_H_

/*==================[inclusiones]============================================*/
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "sapi.h"
#include "semphr.h"

#include "fsm_debounce.h"

/*==================[definiciones y macros]==================================*/
#define MSG_ERROR_TASK 		"Error al crear las tareas.\r\n"
#define MSG_ERROR_SEMPHR 	"Error al crear los semaforos.\r\n"

/*==================[definiciones de datos]=========================*/

/*==================[prototipos de tareas]====================*/
void crear_tarea( TaskFunction_t tarea, const char * const nom_tarea, uint8_t stack_size, void * const param, uint8_t prioridad, TaskHandle_t * const puntero );
void crear_semaforo( SemaphoreHandle_t *semaforo );

#endif /* POSGRADO_RTOS_C4_MODULAR_INC_CONFIG_TASKS_H_ */
