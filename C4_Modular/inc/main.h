/*=============================================================================
 * Copyright (c) 2020, Fabiola de las Casas <fabioladelascasas@gmail.com>
 * All rights reserved.
 * License: Free
 * Date: 2020/09/16
 * Version: v1.1
 *===========================================================================*/

#ifndef _MAIN_H_
#define _MAIN_H_

/*==================[inclusiones]============================================*/
#include "FreeRTOS.h"
#include "task.h"
#include "sapi.h"
#include "semphr.h"
#include "FreeRTOSConfig.h"

#include "fsm_debounce.h"
#include "tasks.h"
#include "config_tasks.h"
/*==================[definiciones y macros]==================================*/
#define MSG_WELCOME "Ejercicio C_6.\r\n"
#define BAUD_RATE 115200

#define PRIORITY 1
#define SIZE     2

#endif /* _MAIN_H_ */
