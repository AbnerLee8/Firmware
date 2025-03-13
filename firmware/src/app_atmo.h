/*******************************************************************************
  MPLAB Harmony Application Header File

  Company:
    Microchip Technology Inc.

  File Name:
    app_atmo.h

  Summary:
    This header file provides prototypes and definitions for the application.

  Description:
    This header file provides function prototypes and data type definitions for
    the application.  Some of these are required by the system (such as the
    "APP_ATMO_Initialize" and "APP_ATMO_Tasks" prototypes) and some of them are only used
    internally by the application (such as the "APP_ATMO_STATES" definition).  Both
    are defined here for convenience.
*******************************************************************************/

#ifndef _APP_ATMO_H
#define _APP_ATMO_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "configuration.h"
#include "app.h"
// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

extern "C" {

#endif
// DOM-IGNORE-END
#define USER_RGB_NUMBER 6
// #define USER_RGB_CODE0  0X40
// #define USER_RGB_CODE1  0X70
#define USER_RGB_CODE0  0X60
#define USER_RGB_CODE1  0X78
#define BIT(n) (0x01<<n)
typedef struct _USER_RGB_COLOUR_{
    unsigned char g;
    unsigned char r;
    unsigned char b;
}RGB_COLOUR;

typedef struct _USER_RGB_SPI_DATA_COLOUR_{
    unsigned char g[8];
    unsigned char r[8];
    unsigned char b[8];
}SPI_COLOUR;

typedef struct _USER_RGB_CODE_{
    unsigned char code_0;
    unsigned char code_1;
}RGB_CODE;

typedef struct _USER_RGB_INFO_{
    unsigned short time_id;
    unsigned char power_off;
    unsigned char init_flag;
    unsigned char rend_flag;
    unsigned char updata_flag;//???
    unsigned char   updata_only;//??
    RGB_CODE code;
    //unsigned char spi_port;//spi
    unsigned short number;//???????
    unsigned short spi_scan_time;//spi ??
    
    #if USER_RGB_BUFF_MALLOC_EN
    RGB_COLOUR *rgb_buff;//[USER_RGB_NUMBER];//????? ??buff
    SPI_COLOUR *spi_buff;//[USER_RGB_NUMBER] __attribute__((aligned(4)));//spi ????buff
    #elif (defined(USER_RGB_NUMBER) && USER_RGB_NUMBER)
    RGB_COLOUR rgb_buff[USER_RGB_NUMBER];//????? ??buff
    SPI_COLOUR spi_buff[USER_RGB_NUMBER] __attribute__((aligned(4)));//spi ????buff
    #endif
}RGB_INFO;
// *****************************************************************************
// *****************************************************************************
// Section: Type Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Application states

  Summary:
    Application states enumeration

  Description:
    This enumeration defines the valid application states.  These states
    determine the behavior of the application at various times.
*/

typedef enum
{
    /* Application's state machine's initial state. */
    APP_ATMO_STATE_INIT=0,
    APP_ATMO_STATE_SERVICE_TASKS,
    /* TODO: Define states used by the application state machine. */

} APP_ATMO_STATES;


// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    Application strings and buffers are be defined outside this structure.
 */

typedef struct
{
    /* The application's current state */
    APP_ATMO_STATES state;

    /* TODO: Define any additional data used by the application. */

} APP_ATMO_DATA;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Routines
// *****************************************************************************
// *****************************************************************************
/* These routines are called by drivers when certain events occur.
*/

// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_ATMO_Initialize ( void )

  Summary:
     MPLAB Harmony application initialization routine.

  Description:
    This function initializes the Harmony application.  It places the
    application in its initial state and prepares it to run so that its
    APP_ATMO_Tasks function can be called.

  Precondition:
    All other system initialization routines should be called before calling
    this routine (in "SYS_Initialize").

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
    APP_ATMO_Initialize();
    </code>

  Remarks:
    This routine must be called from the SYS_Initialize function.
*/

void APP_ATMO_Initialize ( void );


/*******************************************************************************
  Function:
    void APP_ATMO_Tasks ( void )

  Summary:
    MPLAB Harmony Demo application tasks function

  Description:
    This routine is the Harmony Demo application's tasks function.  It
    defines the application's state machine and core logic.

  Precondition:
    The system and application initialization ("SYS_Initialize") should be
    called before calling this.

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
    APP_ATMO_Tasks();
    </code>

  Remarks:
    This routine must be called from SYS_Tasks() routine.
 */

void APP_ATMO_Tasks( void );

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif /* _APP_ATMO_H */

/*******************************************************************************
 End of File
 */

