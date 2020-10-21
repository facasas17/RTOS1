/*=============================================================================
 * Copyright (c) 2020, Fabiola de las Casas <fabioladelascasas@gmail.com>
 * All rights reserved.
 * License: Free
 * Date: 2020/10/14
 * Version: v1.1
 *===========================================================================*/

/*==================[ Inclusiones ]============================================*/
#include "keys.h"

/*==================[Prototypes (declarations) of private functions]======================*/
static void keys_isr_config( void );

/*==================[Definici�n de variables publicas globales ]==============================*/

t_key_data keys_data[N_KEYS];

/*==================[Implementations of public functions]=================================*/

void keys_Init( void )
{
    uint32_t i;
    BaseType_t res;

    /* Inicializo la estructura de teclas y las tareas cada tecla */
    for( i = 0 ; i < N_KEYS ; i++ )
    {
    	keys_data[i].isr_signal     = xSemaphoreCreateBinary();	// Semaforo para indicar flanco positivo o negativo de la tecla
    	keys_data[i].pressed_signal = xSemaphoreCreateBinary();  // Semaforo para indicar que se apreto y solto la tecla

    	// Gesti�n de errores de los semaforos
    	configASSERT( keys_data[i].isr_signal != NULL );
    	configASSERT( keys_data[i].pressed_signal != NULL );

    	// Crear tareas en freeRTOS
    	res = xTaskCreate (
    		   	   	   task_tecla,              		// Funcion de la tarea a ejecutar
					   ( const char * )"task_tecla",   	// Nombre de la tarea como String amigable para el usuario
					   configMINIMAL_STACK_SIZE*2, 		// Cantidad de stack de la tarea
					   (void *)i,                    	// Parametros de tarea
					   tskIDLE_PRIORITY+1,         		// Prioridad de la tarea
					   0                     			// Puntero a la tarea creada en el sistema
       	   	   );
    	// Gesti�n de errores de tareas
    	configASSERT( res == pdPASS );
    }
    keys_isr_config();		// Configuro todas las interrupciones a usar
}

void keys_Update_Isr( uint32_t index )
{
	/* Espero una se�al de la tecla presionada */
	xSemaphoreTake( keys_data[index].isr_signal, portMAX_DELAY );

    /* Espero una se�al de la tecla liberada*/
    xSemaphoreTake( keys_data[index].isr_signal, portMAX_DELAY );

    /* Libero semaforo de tecla pulsada */
    xSemaphoreGive( keys_data[index].pressed_signal ) ;
}

void task_tecla( void* taskParmPtr )
{
    uint32_t index = (uint32_t)taskParmPtr;

    while( TRUE )
    {
        keys_Update_Isr( index );
        vTaskDelay( DEBOUNCE_TIME );
    }
}

/*==================[ Implememtacion de funciones privadas]=================================*/
void keys_isr_config( void )
{
    //Inicializamos las interrupciones (LPCopen)
    Chip_PININT_Init( LPC_GPIO_PIN_INT );

    //Inicializamos de cada evento de interrupcion (LPCopen)

    // TEC1 FALL
    Chip_SCU_GPIOIntPinSel( 0, 0, 4 ); 	//(Canal 0 a 7, Puerto GPIO, Pin GPIO)
    Chip_PININT_SetPinModeEdge( LPC_GPIO_PIN_INT, PININTCH0 ); //Se configura el canal para que se active por flanco
    Chip_PININT_EnableIntLow( LPC_GPIO_PIN_INT, PININTCH0 ); //Se configura para que el flanco sea el de bajada

    // TEC1 RISE
    Chip_SCU_GPIOIntPinSel( 1, 0, 4 );	//(Canal 0 a 7, Puerto GPIO, Pin GPIO)
    Chip_PININT_SetPinModeEdge( LPC_GPIO_PIN_INT, PININTCH1 ); //Se configura el canal para que se active por flanco
    Chip_PININT_EnableIntHigh( LPC_GPIO_PIN_INT, PININTCH1 ); //En este caso el flanco es de subida

    // TEC2 FALL
    Chip_SCU_GPIOIntPinSel( 2, 0, 8 );
    Chip_PININT_SetPinModeEdge( LPC_GPIO_PIN_INT, PININTCH2 );
    Chip_PININT_EnableIntLow( LPC_GPIO_PIN_INT, PININTCH2 );

    // TEC2 RISE
    Chip_SCU_GPIOIntPinSel( 3, 0, 8 );
    Chip_PININT_SetPinModeEdge( LPC_GPIO_PIN_INT, PININTCH3 );
    Chip_PININT_EnableIntHigh( LPC_GPIO_PIN_INT, PININTCH3 );

    // TEC3 FALL
    Chip_SCU_GPIOIntPinSel( 4, 0, 9 );
    Chip_PININT_SetPinModeEdge( LPC_GPIO_PIN_INT, PININTCH4 );
    Chip_PININT_EnableIntLow( LPC_GPIO_PIN_INT, PININTCH4 );

    // TEC3 RISE
    Chip_SCU_GPIOIntPinSel( 5, 0, 9 );
    Chip_PININT_SetPinModeEdge( LPC_GPIO_PIN_INT, PININTCH5 );
    Chip_PININT_EnableIntHigh( LPC_GPIO_PIN_INT, PININTCH5 );

    // TEC4 FALL
    Chip_SCU_GPIOIntPinSel( 6, 1, 9 );
    Chip_PININT_SetPinModeEdge( LPC_GPIO_PIN_INT, PININTCH6 );
    Chip_PININT_EnableIntLow( LPC_GPIO_PIN_INT, PININTCH6 );

    // TEC4 RISE
    Chip_SCU_GPIOIntPinSel( 7, 1, 9 );
    Chip_PININT_SetPinModeEdge( LPC_GPIO_PIN_INT, PININTCH7 );
    Chip_PININT_EnableIntHigh( LPC_GPIO_PIN_INT, PININTCH7 );

    //Una vez que se han configurado los eventos para cada canal de interrupcion
    //Se activan las interrupciones para que comiencen a llamar al handler
    NVIC_SetPriority( PIN_INT0_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY );
    NVIC_EnableIRQ( PIN_INT0_IRQn );
    NVIC_SetPriority( PIN_INT1_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY );
    NVIC_EnableIRQ( PIN_INT1_IRQn );

    NVIC_SetPriority( PIN_INT2_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY );
    NVIC_EnableIRQ( PIN_INT2_IRQn );
    NVIC_SetPriority( PIN_INT3_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY );
    NVIC_EnableIRQ( PIN_INT3_IRQn );
    NVIC_SetPriority( PIN_INT4_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY );
    NVIC_EnableIRQ( PIN_INT4_IRQn );
    NVIC_SetPriority( PIN_INT5_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY );
    NVIC_EnableIRQ( PIN_INT5_IRQn );
    NVIC_SetPriority( PIN_INT6_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY );
    NVIC_EnableIRQ( PIN_INT6_IRQn );
    NVIC_SetPriority( PIN_INT7_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY );
    NVIC_EnableIRQ( PIN_INT7_IRQn );
}

void GPIO0_IRQHandler( void )   //asociado a tec1
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE; //Comenzamos definiendo la variable

    if ( Chip_PININT_GetFallStates( LPC_GPIO_PIN_INT ) & PININTCH0 ) //Verificamos que la interrupciÃ³n es la esperada
    {
        Chip_PININT_ClearIntStatus( LPC_GPIO_PIN_INT, PININTCH0 ); //Borramos el flag de interrupciÃ³n

        xSemaphoreGiveFromISR( keys_data[TEC1_INDEX].isr_signal, &xHigherPriorityTaskWoken ); // Entrego semaforo que indica tecla pulsada
    }
    portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
}

void GPIO1_IRQHandler( void )  //asociado a tec1
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    if ( Chip_PININT_GetRiseStates( LPC_GPIO_PIN_INT ) & PININTCH1 )
    {
        Chip_PININT_ClearIntStatus( LPC_GPIO_PIN_INT, PININTCH1 );

        xSemaphoreGiveFromISR( keys_data[TEC1_INDEX].isr_signal, &xHigherPriorityTaskWoken ); // Entrego semaforo que indica tecla soltada
    }

    portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
}

void GPIO2_IRQHandler( void )	//asociado a tec2
{
    UBaseType_t uxSavedInterruptStatus;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE; //Comenzamos definiendo la variable

    if ( Chip_PININT_GetFallStates( LPC_GPIO_PIN_INT ) & PININTCH2 ) //Verificamos que la interrupciÃ³n es la esperada
    {
        Chip_PININT_ClearIntStatus( LPC_GPIO_PIN_INT, PININTCH2 ); //Borramos el flag de interrupciÃ³n

        xSemaphoreGiveFromISR( keys_data[TEC2_INDEX].isr_signal, &xHigherPriorityTaskWoken );
    }

    portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
}

void GPIO3_IRQHandler( void )	//asociado a tec2
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    if ( Chip_PININT_GetRiseStates( LPC_GPIO_PIN_INT ) & PININTCH3 )
    {
        Chip_PININT_ClearIntStatus( LPC_GPIO_PIN_INT, PININTCH3 );
        //codigo a ejecutar si ocurriÃ³ la interrupciÃ³n

        xSemaphoreGiveFromISR( keys_data[TEC2_INDEX].isr_signal, &xHigherPriorityTaskWoken );
    }
    portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
}

void GPIO4_IRQHandler( void )	//asociado a tec3
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE; //Comenzamos definiendo la variable


    if ( Chip_PININT_GetFallStates( LPC_GPIO_PIN_INT ) & PININTCH4 ) //Verificamos que la interrupciÃ³n es la esperada
    {
        Chip_PININT_ClearIntStatus( LPC_GPIO_PIN_INT, PININTCH4 ); //Borramos el flag de interrupciÃ³n

        xSemaphoreGiveFromISR( keys_data[TEC3_INDEX].isr_signal, &xHigherPriorityTaskWoken );
    }

    portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
}

void GPIO5_IRQHandler( void )	//asociado a tec3
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    if ( Chip_PININT_GetRiseStates( LPC_GPIO_PIN_INT ) & PININTCH5 )
    {
        Chip_PININT_ClearIntStatus( LPC_GPIO_PIN_INT, PININTCH5 );

        xSemaphoreGiveFromISR( keys_data[TEC3_INDEX].isr_signal, &xHigherPriorityTaskWoken );
    }
    portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
}

void GPIO6_IRQHandler( void )	//asociado a tec4
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE; //Comenzamos definiendo la variable

    if ( Chip_PININT_GetFallStates( LPC_GPIO_PIN_INT ) & PININTCH6 ) //Verificamos que la interrupciÃ³n es la esperada
    {
        Chip_PININT_ClearIntStatus( LPC_GPIO_PIN_INT, PININTCH6 ); //Borramos el flag de interrupciÃ³n

        xSemaphoreGiveFromISR( keys_data[TEC4_INDEX].isr_signal, &xHigherPriorityTaskWoken );
    }

    portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
}

void GPIO7_IRQHandler( void )	//asociado a tec4
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    if ( Chip_PININT_GetRiseStates( LPC_GPIO_PIN_INT ) & PININTCH7 )
    {
        Chip_PININT_ClearIntStatus( LPC_GPIO_PIN_INT, PININTCH7 );

        xSemaphoreGiveFromISR( keys_data[TEC4_INDEX].isr_signal, &xHigherPriorityTaskWoken );
    }
    portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
}
