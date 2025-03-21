/*******************************************************************************
  MPLAB Harmony Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app_atmo.c

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

#include "app_atmo.h"

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
    This structure should be initialized by the APP_ATMO_Initialize function.

    Application strings and buffers are be defined outside this structure.
*/

APP_ATMO_DATA app_atmoData;

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


// .............................................................................
// Atmosphere LED SPI transfer function.
// Call this to initiate read or write (after setting up buffer etc.))
// .............................................................................
void atmo_xfer(uint8_t *wrbuf, uint8_t *rdbuf, uint len)
{
    SERCOM6_SPI_WriteRead (wrbuf, len, rdbuf, len);
}

// .............................................................................
// Atmosphere LED  Write function
// Inputs:
//  wrbuf:  Start of data buffer containing the data to write
//  len:    Number of bytes to write
// .............................................................................
static uint8_t atmo_wrbuf[256];
static uint8_t atmo_rdbuf[256];
void atmo_write(  uint8_t *wrbuf, uint len)
{

//    if (len > 256) len = 256;

   for (uint i = 0; i < len; i++) {
        atmo_wrbuf[i] = wrbuf[i];
   }
    atmo_xfer(atmo_wrbuf, atmo_rdbuf, len);
}
RGB_INFO *_this_rgb = NULL;

//?
void user_rgb_set_only(SPI_COLOUR *spi_buff,RGB_COLOUR *rgb,RGB_CODE *code)
{
    if(!spi_buff || !rgb){
        return;
    }

    for(int i=0;i<8;i++){
        if(rgb->r&BIT(7-i)){
            spi_buff->r[i]=code->code_1;
        }else{
            spi_buff->r[i]=code->code_0;
        }
        if(rgb->g&BIT(7-i)){
            spi_buff->g[i]=code->code_1;
        }else{
            spi_buff->g[i]=code->code_0;
        }
        if(rgb->b&BIT(7-i)){
            spi_buff->b[i]=code->code_1;
        }else{
            spi_buff->b[i]=code->code_0;
        }
    }
}

void user_rgb_set_all(void *priv)
{
    RGB_INFO *rgb = (RGB_INFO *)priv;

    if(!rgb || !rgb->spi_buff || !rgb->rgb_buff || !rgb->number){
        return;
    }
    
    SPI_COLOUR *P=rgb->spi_buff;


    rgb->rend_flag = 1;
    for(int i=0;i<rgb->number;i++){
        user_rgb_set_only(P+i,rgb->rgb_buff+i,&(rgb->code));
    }
    rgb->rend_flag = 0;

    return;
}


void user_rgb_colour_only_set(void *priv,RGB_COLOUR *colour,signed short number)
{
    RGB_INFO *rgb = (RGB_INFO *)priv;
    signed short mapp_number = number;

    if(!rgb || !colour){
        return;
    }

    mapp_number%=rgb->number;

    mapp_number = mapp_number<0?(rgb->number+mapp_number):mapp_number;

    rgb->rgb_buff[mapp_number]=*colour;

    return;
}


void user_rgb_same_colour(void *priv,RGB_COLOUR *colour)
{
    RGB_INFO *rgb = (RGB_INFO *)priv;
    if(!rgb || !colour || !rgb->number){
        return;
    }

    for(int i=0;i<rgb->number;i++){
        user_rgb_colour_only_set(rgb,colour,i);
    }
}

void user_rgb_clear_colour(void *priv)
{
    RGB_INFO *rgb = (RGB_INFO *)priv;
    RGB_COLOUR colour={0,0,0};

    if(!rgb || !rgb->number){
        return;
    }

    user_rgb_same_colour(rgb,&colour);
}

void user_rgb_spi_send_once(void)
{
    if(!_this_rgb || !_this_rgb->number || !_this_rgb->init_flag || _this_rgb->power_off ||
        !_this_rgb->spi_buff || _this_rgb->updata_flag){
        return;
    }
    
    user_rgb_set_all(_this_rgb);

   // int send_ret = 
    atmo_write((uint8_t*)_this_rgb->spi_buff,sizeof(SPI_COLOUR)*(_this_rgb->number));
    
//    if(send_ret<0){
//        printf("user rgb spi send data error\n");
//    }

    if(0xff != _this_rgb->updata_only && _this_rgb->updata_only){
        _this_rgb->updata_only -=1;
    }
    // r_printf("spi send data ok");
}

void user_rgb_send(void *priv)
{
    RGB_INFO *rgb = (RGB_INFO *)priv;
    if(!rgb){
        return;
    }

    if(rgb->power_off){
        return;
    }

    unsigned short timeout_add_time = rgb->spi_scan_time;

    // printf(">>>> clck spi %d sys %d\n",clk_get("spi"),clk_get("sys"));
    if(rgb->updata_only/*timer_get_ms()>3000*/){        
        user_rgb_spi_send_once();
    }else{
        timeout_add_time = 10;
    }

//    rgb -> time_id = sys_s_hi_timerout_add(rgb,user_rgb_send,timeout_add_time);
}

void user_rgb_send_ctrl(unsigned char cmd)
{
    if(!_this_rgb){
        return;
    }

    if(0 == cmd){
        _this_rgb->updata_only = 0;
    }else if(0xff == cmd){
        _this_rgb->updata_only = 0xff;
    }else if(cmd){
        _this_rgb->updata_only = cmd;
    }

    if(_this_rgb->updata_only){
        user_rgb_spi_send_once();
    }
}


void *user_rgb_dev_init(void *priv)
{
    RGB_INFO *rgb = (RGB_INFO *)priv;
//    if(!rgb || SPI_MAX_HW_NUM<=(rgb->spi_port)){
//        return NULL;
//    }
    _this_rgb = rgb;
    
//    g_printf(">>>>>>>>>> _this_rgb %x %x",_this_rgb,priv);

//    int ret = spi_open(rgb->spi_port);
//    if(ret<0){
//        return NULL;
//    }

    if(!(_this_rgb->spi_scan_time)){
        _this_rgb->spi_scan_time = 100;
    }

    user_rgb_send_ctrl(0xff);
    _this_rgb->init_flag = true;

//    sys_s_hi_timerout_add(_this_rgb,user_rgb_send,_this_rgb->spi_scan_time);//??spi?????????????????? ???rgb????????

    return (void *)_this_rgb;
}

void user_rgb_dev_exit(void)
{
    // local_irq_disable();

    if(_this_rgb){
        user_rgb_clear_colour(_this_rgb);
        _this_rgb->updata_flag = 0;//???? user_rgb_send_ctrl ??updata_flag?1
        user_rgb_send_ctrl(1);

        _this_rgb->init_flag = 0;
        if(_this_rgb -> time_id){
//            sys_s_hi_timeout_del(_this_rgb -> time_id);
            _this_rgb -> time_id = 0;
        }
//        spi_close(_this_rgb->spi_port);
        _this_rgb = NULL;  
    }


    // local_irq_enable();
}

// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_ATMO_Initialize ( void )

  Remarks:
    See prototype in app_atmo.h.
 */

void APP_ATMO_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    app_atmoData.state = APP_ATMO_STATE_INIT;



    /* TODO: Initialize your application's state machine and other
     * parameters.
     */
}


/******************************************************************************
  Function:
    void APP_ATMO_Tasks ( void )

  Remarks:
    See prototype in app_atmo.h.
 */
static uint8_t rst_code=0x00;
//static uint8_t spi_wrbuf[24]={0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,
//                              0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,
//                              0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60};
static uint8_t spi_wrbuf[3][48]={{0xEE,0xEE,0xEE,0xEE,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0xEE,0xEE,0xEE,0xEE,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0xEE,0xEE,0xEE,0xEE,0xEE,0xEE,0xEE,0xEE,0xEE,0xEE,0xEE,0xEE,0x88,0x88,0x88,0x88},
                                 {0x88,0x88,0x88,0x88,0xEE,0xEE,0xEE,0xEE,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0xEE,0xEE,0xEE,0xEE,0xEE,0xEE,0xEE,0xEE,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0xEE,0xEE,0xEE,0xEE,0xEE,0xEE,0xEE,0xEE},
                                 {0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0xEE,0xEE,0xEE,0xEE,0xEE,0xEE,0xEE,0xEE,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0xEE,0xEE,0xEE,0xEE,0x88,0x88,0x88,0x88,0xEE,0xEE,0xEE,0xEE,0x88,0x88,0x88,0x88,0xEE,0xEE,0xEE,0xEE},
                                };
//static uint8_t spi_wrbuf[3][48]={{0x77,0x77,0x77,0x77,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x77,0x77,0x77,0x77,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x77,0x77,0x77,0x77,0x77,0x77,0x77,0x77,0x77,0x77,0x77,0x77,0x11,0x11,0x11,0x11},
//                                 {0x11,0x11,0x11,0x11,0x77,0x77,0x77,0x77,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x77,0x77,0x77,0x77,0x77,0x77,0x77,0x77,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x77,0x77,0x77,0x77,0x77,0x77,0x77,0x77},
//                                 {0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x77,0x77,0x77,0x77,0x77,0x77,0x77,0x77,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x77,0x77,0x77,0x77,0x11,0x11,0x11,0x11,0x77,0x77,0x77,0x77,0x11,0x11,0x11,0x11,0x77,0x77,0x77,0x77},
//                                };
// static uint8_t spi_wrbuf[3][24]={{0xEE,0xEE,0xEE,0xEE,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0xEE,0xEE,0xEE,0xEE,0x88,0x88,0x88,0x88},
//                                  {0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0xEE,0xEE,0xEE,0xEE,0xEE,0xEE,0xEE,0xEE,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88},
//                                  {0x88,0x88,0x88,0x88,0xEE,0xEE,0xEE,0xEE,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0xEE,0xEE,0xEE,0xEE},
//                                };
//static uint8_t spi_wrbuf[2][48]={{0xEE,0xEE,0xEE,0xEE,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0xEE,0xEE,0xEE,0xEE,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0xEE,0xEE,0xEE,0xEE,0xEE,0xEE,0xEE,0xEE,0xEE,0xEE,0xEE,0xEE,0x88,0x88,0x88,0x88},
//                                 {0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88},
//                                };
//static uint8_t spi_wrbuf[3][96]={{0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,
//                                  0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,
//                                  0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,
//                                  0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60},
//                                  
//                                  {0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,
//                                  0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,
//                                  0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,
//                                  0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78},
//                                  
//                                  {0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,
//                                  0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,
//                                  0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,
//                                  0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78}
//                                 };
void APP_ATMO_Tasks ( void )
{
    uint8_t i=0;
    while(1) {
        osDelayMs(1000);           // 10 ms delay, used for scheduling below
        //atmo_write(0x33,1);
       // atmo_write(0x55,1);
        
        //atmo_write(spi_wrbuf,24);
        for(uint8_t j=0;j<100;j++)
        atmo_write(&rst_code,1);
 
        atmo_write(spi_wrbuf[i],48);
        i++;
        if(i>=3)
          i=0;
        //atmo_write(0xCC,1);
    /* Check the application's current state. */
//    switch ( app_atmoData.state )
//    {
//        /* Application's initial state. */
//        case APP_ATMO_STATE_INIT:
//        {
//            bool appInitialized = true;
//
//
//            if (appInitialized)
//            {
//
//                app_atmoData.state = APP_ATMO_STATE_SERVICE_TASKS;
//            }
//            break;
//        }
//
//        case APP_ATMO_STATE_SERVICE_TASKS:
//        {
//         static uint8_t spi_wrbuf[12]={255,0,0,0,255,0,0,0,255,255,255,0};
//         atmo_write((uint8_t*)spi_wrbuf,sizeof(spi_wrbuf));
//            break;
//        }
//
//        /* TODO: implement your application state machine.*/
//
//
//        /* The default state should never be executed. */
//        default:
//        {
//            /* TODO: Handle error in application's state machine. */
//            break;
//        }
//    }
    }
}


/*******************************************************************************
 End of File
 */
