/*=============================================================================
 * Copyright (c) 2020, Fabiola de las Casas <fabioladelascasas@gmail.com>
 * All rights reserved.
 * License: Free
 * Date: 2020/09/10
 * Version: v1.1
 *===========================================================================*/



#ifndef PDM_ANTIRREBOTE_MEF_INC_DEBOUNCE_H_
#define PDM_ANTIRREBOTE_MEF_INC_DEBOUNCE_H_

#include "FreeRTOS.h"

void fsmButtonInit( void );
void fsmButtonUpdate( gpioMap_t tecla );
TickType_t get_diff();
void clear_diff();

#endif /* PDM_ANTIRREBOTE_MEF_INC_DEBOUNCE_H_ */
