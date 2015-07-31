/**
 *  @file   main.c
 *  @brief  main file of TTimerExample project
 *  @author Rafael Dias <rdmeneze@gmail.com>
 *  @date   jul/2015 
 */


#include "BoardLeds.h"
#include "BoardKeys.h"
#include "TTimer.h"
#include "SysProcessor.h"
#include "util.h"

/*****************************************************************************/

uint32_t TaskMain( void* lpParam ); // task to manipulate the leds and buttons

uint32_t TaskBlink( void* lpParam );    // dummy task

/*****************************************************************************/

/**
 * @brief main function
 * @return none
 */
int main()
{   
    uint32_t dwTimerHandle;
    
    SetSystemClock();  /* configure the system clock  */    
    
    TTimerCfgTimeOut( 500 );  /* initialize the ttimer engine  */
    
    BrdLedsInit();  /* initialize the board leds engine */
    BrdKeyInit();   /* initialize the board keys engine */
    
    BrdLedsSetState( USR_LED0, 0 );
    BrdLedsSetState( USR_LED1, 0 );
    BrdLedsSetState( USR_LED2, 0 );
    BrdLedsSetState( USR_LED3, 0 );

    // create a periodic timer to execute TaskMain
    TTimerRegisterCallBack( TTIMER_1MS_INTERVAL, TimerPeriodic, TaskMain, NULL, &dwTimerHandle );
    TTimerStart( dwTimerHandle );

    // create a periodic timer to execute TaskBlink
    TTimerRegisterCallBack( TTIMER_1SEC_INTERVAL, TimerPeriodic, TaskBlink, NULL, &dwTimerHandle );
    TTimerStart( dwTimerHandle );
    
    for( ;; );
}

/******************************************************************************/

uint32_t TaskMain( void* lpParam )
{
    struct STSwStatus
    {
        uint8_t bCurrentStatus;
        uint8_t bPrevStatus;
    };
    
    uint8_t bCounter;
    static uint8_t bLedStatus=0;
    static UsrLedType tUsrLed[] = {USR_LED0, USR_LED1};
    static struct STSwStatus stSwStatus[] = 
    {
        [0] = 
        {
            .bCurrentStatus = 0,
            .bPrevStatus    = 0,
        },
        [1] = 
        {
            .bCurrentStatus = 0,
            .bPrevStatus    = 0,
        }
    };
    
    for ( bCounter = 0; bCounter < GET_ARRAY_LEN( stSwStatus ); bCounter++ )
    {
        stSwStatus[bCounter].bPrevStatus = stSwStatus[bCounter].bCurrentStatus;
        stSwStatus[bCounter].bCurrentStatus = BrdKeyRead( (UsrSwType)bCounter );
        
        if( (stSwStatus[bCounter].bPrevStatus == 0) && (stSwStatus[bCounter].bCurrentStatus == 1) )
        {
            bLedStatus ^= (1<<bCounter);
            BrdLedsSetState( tUsrLed[bCounter], (bLedStatus & (1<<bCounter)) == (1<<bCounter) ); 
        }
    }
    
    return 0;
}

/******************************************************************************/

uint32_t TaskBlink( void* lpParam )
{
    static uint8_t bCounter = 0;
    
    BrdLedsSetState( USR_LED3, (bCounter++) & 1 );
    
    return 0;
}

/******************************************************************************/
