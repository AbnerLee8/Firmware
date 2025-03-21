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
#define USER_RGB_NUMBER 4
// #define USER_RGB_CODE0  0X40
// #define USER_RGB_CODE1  0X70
//#define USER_RGB_CODE0  0X60
//#define USER_RGB_CODE1  0X7E
#define USER_RGB_CODE0  0X8
#define USER_RGB_CODE1  0XE


#define USER_RGB_BUFF_MALLOC_EN 0//rgb buffer、spi buff dynamic malloc memory or not

#define BIT(n) (0x01<<n)
typedef struct _USER_RGB_COLOUR_{
    unsigned char r;
    unsigned char g;  
    unsigned char b;
}RGB_COLOUR;

typedef struct _USER_RGB_SPI_DATA_COLOUR_{
    unsigned char r[4];
    unsigned char g[4];
    unsigned char b[4];
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
    unsigned char updata_flag;//light colour update flag
    unsigned char updata_only;//only update data once
    RGB_CODE code;
    unsigned short number;//light numbers
    
    #if USER_RGB_BUFF_MALLOC_EN
    RGB_COLOUR *rgb_buff;//[USER_RGB_NUMBER];//rgb light buff
    SPI_COLOUR *spi_buff;//[USER_RGB_NUMBER] __attribute__((aligned(4)));//spi data buff
    #elif (defined(USER_RGB_NUMBER) && USER_RGB_NUMBER)
    RGB_COLOUR rgb_buff[USER_RGB_NUMBER];//rgb light buff
    SPI_COLOUR spi_buff[USER_RGB_NUMBER] __attribute__((aligned(4)));//spi data buff
    #endif
}RGB_INFO;

#define USER_RGB_LOOP_MODE_1    1//all
#define USER_RGB_LOOP_MODE_2    2//Non monochromatic flicker cycle
#define USER_RGB_LOOP_MODE_3    3//monochromatic flicker cycle
	
#define USER_RGB_SPI_SEND_SCAN_TIME  (10)//Interval time
	

    /*Maximum brightness level, please note that the data type value should not exceed the range. Normally, use 8-bit LED beads with a maximum value not exceeding 255. Use 4k LED beads. Please modify the data type level*/
#define RGB_BRIGHTNESS_LEVEL  (180)

	/*
	Used as a fade in/fade out threshold for controlling lights
	Divide brightness into several levels (segments)
	eg.
	RGB_BRIGHTNESS_LEVEL ：120
	RGB_BRIGHTNESS_SEGMENT：4
	Lights with brightness less than 120/4 are considered fade in
	Brightness greater than 120/4 and less than (120/4) * 2 is considered fading out
	*/
#define RGB_BRIGHTNESS_SEGMENT (5)
	
	typedef struct _USER_RGB_DISPLAY_DATA_{
		int sys_vol_max;
		int sys_vol;
		int bass;
		unsigned short display_time;//interrupt Interval time unit seconds
	}RGB_DISPLAY_DATA;
	
	typedef enum{
		USER_RGB_MODE_1=1,//Rhythm gradient rotation
		USER_RGB_MODE_2,//Symmetrical lifting and lowering
		USER_RGB_MODE_3,//Flowing light display
		USER_RGB_MODE_4,//Tricolor rotation
		USER_RGB_MODE_5,//Panchromatic respiration
		USER_RGB_MODE_6,//spectrum
		USER_RGB_MODE_OFF,//turn off
		USER_RGB_MODE_7,//start  
		USER_RGB_MODE_8,//poweroff
		USER_RGB_MODE_9,//white blink
		USER_RGB_MODE_MAX,
	
		USER_RGB_FM_MODE,//FM
		USER_RGB_AUTO_SW,//auto switch
		USER_RGB_SYS_VOL,//sys volume
		USER_RGB_EQ_BASS,//BASS state
		USER_RGB_STATUS,
		USER_RGB_STATUS_LOCK,
		USER_RGB_STATUS_ULOCK,
		USER_RGB_NULL,
	}USER_GRB_MODE;
	
	typedef struct _USER_RGB_FUN_{
		RGB_INFO *info;
		bool interrupt;//Interrupt flag is used for displaying volume and other non modal effects
		unsigned short interrupt_id;//Interrupt task id bass, vol display common this id
		
    #if(defined(USER_RGB_NUMBER) && USER_RGB_NUMBER)
		unsigned char brightness_table[USER_RGB_NUMBER];//light brightness level table
    #endif
		unsigned char step_value; //light brightness level step value
	
		USER_GRB_MODE cur_mode;//current mode
		USER_GRB_MODE mode_lock;//lock mode
		unsigned short mode_scan_time;//Mode scanning time
		RGB_COLOUR cur_colour;//current colour
		int light_number;//dac energy lights
		unsigned short freq;//Monochromatic RGB flicker frequency
		unsigned short dac_energy_scan_id;//Dac energy scanning time id
	}RGB_FUN;
	
void user_rgb_fun_init(void);
unsigned char user_rgb_mode_set_or_get(USER_GRB_MODE mode,void *priv);
void user_rgb_display_vol(unsigned char vol,unsigned short display_time);
void user_rgb_display_bass(unsigned char bass,unsigned short display_time);
void user_rgb_fun_exit(void);

void *user_rgb_dev_init(void *priv);
void user_rgb_dev_exit(void);

void *user_rgb_dev_init(void *priv);
void user_rgb_dev_exit(void);


//Set the color of a single light
void user_rgb_colour_only_set(void *priv, RGB_COLOUR *colour, signed short number);
//Set to the same color
void user_rgb_same_colour(void *priv,RGB_COLOUR *colour);
//clear colour
void user_rgb_clear_colour(void *priv);
//send once spi buffer data
void user_rgb_spi_send_once(void);

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
    APP_ATMO_STATE_SHOW_ONECE,
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

