/*******************************************************************************
  MPLAB Harmony Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app_charge.c

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

#include "app_charge.h"
#include "max77985.h"
#include "cw221x.h"
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
    This structure should be initialized by the APP_CHARGE_Initialize function.

    Application strings and buffers are be defined outside this structure.
*/

//APP_CHARGE_DATA app_chargeData;

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


// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_CHARGE_Initialize ( void )

  Remarks:
    See prototype in app_charge.h.
 */

void APP_CHARGE_Initialize ( void )
{

}


/******************************************************************************
  Function:
    void APP_CHARGE_Tasks ( void )

  Remarks:
    See prototype in app_charge.h.
 */

void APP_CHARGE_Tasks ( void )
{

    // Task waits Power Task has powered everything up
    while (!GlobalPowerOn) {
        osDelayMs(100);
    }
    //init cw221x(batt)
    batt_cw221x_init();

    //init charger
    charger_init();
    osDelayMs(20);
    while(1) 
    {
      OS_SLEEP(10); //osDelayMs(10);
    }
}


/*******************************************************************************
 End of File
 */
