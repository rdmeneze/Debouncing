#ifndef BOARD_LEDS_H
#define BOARD_LEDS_H

#include "defs.h"

typedef enum
{
    USR_LED0, 
    USR_LED1, 
    USR_LED2, 
    USR_LED3
} UsrLedType;    
    

uint32_t BrdLedsInit( void );

uint32_t BrdLedsSetState( UsrLedType led, uint8_t bState );


#endif 
