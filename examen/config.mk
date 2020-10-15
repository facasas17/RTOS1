# Compile options
VERBOSE=n
OPT=g
USE_NANO=Y
SEMIHOST=n
USE_FPU=y

# Libraries
USE_LPCOPEN=y
USE_SAPI=y
DEFINES+=SAPI_USE_INTERRUPTS

USE_FREERTOS=y
FREERTOS_HEAP_TYPE=1
LOAD_INRAM=n

# Tell SAPI to use FreeRTOS SYSTICK
DEFINES+=TICK_OVER_RTOS
DEFINES+=USE_FREERTOS