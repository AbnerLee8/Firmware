/*******************************************************************************
  MPLAB Harmony Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app_right_sns.c

  Summary:
    This file contains the source code for the MPLAB Harmony application.

  Description:
    This file contains the source code for the MPLAB Harmony application.  It
    implements the logic of the application's state machine and it may call
    API routines of other MPLAB Harmony modules in the system, such as drivers,
    system services, and middleware.  However, it does not call any of the
    system interfaces (such as the "Initialize" and "Tasks" functions) of any of
    the modules in the system or make any assumptions about when those functions
    are called.  That is the responsibility of the configuration-specific system
    files.
 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "app_right_sns.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the APP_RIGHT_SNS_Initialize function.

    Application strings and buffers are be defined outside this structure.
*/

APP_RIGHT_SNS_DATA app_right_snsData;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

/* TODO:  Add any necessary callback functions.
*/

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************


/* TODO:  Add any necessary local functions.
*/
// ********************************************
// I2C Read/Write functions for Wear Sensor
// ********************************************
#define LOGIC_I2C_ADDR       0x08
#define LOGIC_I2C_Write(s,wa,wn)  SERCOM5_I2C_Write(s,wa,wn)
#define LOGIC_I2C_Read(s,ra,rn)   SERCOM5_I2C_Read(s,ra,rn)
#define LOGIC_I2C_WriteRead(s,wa,wn,ra,rn)    SERCOM5_I2C_WriteRead(s,wa,wn,ra,rn)
#define LOGIC_I2C_IsBusy()        SERCOM5_I2C_IsBusy()
#define LOGIC_I2C_ErrorGet()      SERCOM5_I2C_ErrorGet()

// .............................................................................
int logic_SLG46536_write(uint8_t *buf, uint len)
{
    LOGIC_I2C_Write(LOGIC_I2C_ADDR, buf, len);
    while (LOGIC_I2C_IsBusy()) {
        osDelayMs(1);
    }
    return LOGIC_I2C_ErrorGet();
}
// .............................................................................
int logic_SLG46536_read(uint8_t *buf, uint len)
{
    LOGIC_I2C_Read(LOGIC_I2C_ADDR, buf, len);
    while (LOGIC_I2C_IsBusy()) {
        osDelayMs(1);
    }
    return LOGIC_I2C_ErrorGet();
}
// .............................................................................
int logic_SLG46536_writeread(uint8_t *wrbuf, uint wrlen, uint8_t *rdbuf, uint rdlen)
{
    LOGIC_I2C_WriteRead(LOGIC_I2C_ADDR, wrbuf, wrlen, rdbuf, rdlen);
    while (LOGIC_I2C_IsBusy()) {
        osDelayMs(1);
    }
    return LOGIC_I2C_ErrorGet();
}
bool logic_SLG46536_interrupted(void)
{
  if(!LOGIC_INT_47011_Get())
  	return(true);
  else return(false);
}


// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_RIGHT_SNS_Initialize ( void )

  Remarks:
    See prototype in app_right_sns.h.
 */

void APP_RIGHT_SNS_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    app_right_snsData.state = APP_RIGHT_SNS_STATE_INIT;



    /* TODO: Initialize your application's state machine and other
     * parameters.
     */
}


/******************************************************************************
  Function:
    void APP_RIGHT_SNS_Tasks ( void )

  Remarks:
    See prototype in app_right_sns.h.
 */

void APP_RIGHT_SNS_Tasks ( void )
{

    /* Check the application's current state. */
    switch ( app_right_snsData.state )
    {
        /* Application's initial state. */
        case APP_RIGHT_SNS_STATE_INIT:
        {
            bool appInitialized = true;


            if (appInitialized)
            {

                app_right_snsData.state = APP_RIGHT_SNS_STATE_SERVICE_TASKS;
            }
            break;
        }

        case APP_RIGHT_SNS_STATE_SERVICE_TASKS:
        {

            break;
        }

        /* TODO: implement your application state machine.*/


        /* The default state should never be executed. */
        default:
        {
            /* TODO: Handle error in application's state machine. */
            break;
        }
    }
}


/*******************************************************************************
 End of File
 */
