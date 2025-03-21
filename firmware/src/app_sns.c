/*******************************************************************************
  File Name:
    app_sns.c

  Summary:
    Sensors task.
 * touch chip
 * wear sensor (IR)
 * charger
 * accelerometer

  Description:
 *******************************************************************************/
#include "app_sns.h"
#include "hw_hx32062se.h"
#include "hw_qmi8658a.h"
#include "max77985.h"
#include "cw221x.h"
// ...........................................
// Charger
// ...........................................
//#define I2C_CHARGER_ADDR    0x6B
uint8_t battery_charging;

// ...........................................
// Wear Sensor
// ...........................................
#define I2C_WEAR_ADDR       0x2B

#define I2C_Write(s,wa,wn)  SERCOM3_I2C_Write(s,wa,wn)
#define I2C_Read(s,ra,rn)   SERCOM3_I2C_Read(s,ra,rn)
#define I2C_WriteRead(s,wa,wn,ra,rn)    SERCOM3_I2C_WriteRead(s,wa,wn,ra,rn)
#define I2C_IsBusy()        SERCOM3_I2C_IsBusy()
#define I2C_ErrorGet()      SERCOM3_I2C_ErrorGet()

// Define debounce times for FAR (0: remove from head) and NEAR (1: place on head)
// 3 samples per sec, so e.g. 6 = seconds
#define WEAR_DEBOUNCE_MAX_FAR       6
#define WEAR_DEBOUNCE_MAX_NEAR      3
int8_t wear_state_new;
int8_t wear_state_old;
int8_t wear_state_dbnc;
uint8_t wear_dbnc_cntr;
uint8_t wear_buf[256];

uint32_t g_INT_G_cnt_previous;

uint8_t schedule_index = 0;

// ...........................................
// Accelerometer
// ...........................................
#define I2C_G_ADDR        0x6B

#define I2C_G_Write(s,wa,wn)  SERCOM4_I2C_Write(s,wa,wn)
#define I2C_G_Read(s,ra,rn)   SERCOM4_I2C_Read(s,ra,rn)
#define I2C_G_WriteRead(s,wa,wn,ra,rn)    SERCOM4_I2C_WriteRead(s,wa,wn,ra,rn)
#define I2C_G_IsBusy()        SERCOM4_I2C_IsBusy()
#define I2C_G_ErrorGet()      SERCOM4_I2C_ErrorGet()

volatile uint32_t INT_G_cnt;
// ...........................................


/*******************************************************************************/

/*
// ************************
// Unused code
// ************************

// ...........................................
void touch_init()
{
    INT_TOUCH_L_OutputEnable();
    INT_TOUCH_L_Clear();
    RST_TOUCH_L_Clear();
    osDelayMs(10);
    RST_TOUCH_L_Set();
    osDelayMs(10);
    INT_TOUCH_L_InputEnable();
    osDelayMs(50);
}
// ...........................................
int touch_write(uint8_t *buf, uint len)
{
    I2C_Write(I2C_TOUCH_ADDR, buf, len);
    while (I2C_IsBusy()) {
        osDelayMs(1);
    }
    return I2C_ErrorGet();
}
// ...........................................
int touch_read(uint8_t *buf, uint len)
{
    I2C_Read(I2C_TOUCH_ADDR, buf, len);
    while (I2C_IsBusy()) {
        osDelayMs(1);
    }
    return I2C_ErrorGet();
}
// ...........................................
int touch_readfrom(uint addr, uint8_t *buf, uint len)
{
    uint8_t wrbuf[2];
    wrbuf[0] = addr >> 8; // register high address byte
    wrbuf[1] = addr; // register low address byte
    touch_write(wrbuf, 2);
    return touch_read(buf, len);
}

// ...........................................
int touch_writeto(uint addr, uint8_t *buf, uint len)
{
    uint8_t i, wrbuf[32];
    wrbuf[0] = addr >> 8; // register high address byte
    wrbuf[1] = addr; // register low address byte
    for (i = 0; i < len; i++) {
        wrbuf[i + 2] = buf[i];
    }
    touch_write(wrbuf, len + 2);
}
// ...........................................

// ...........................................
void charger_init()
{
}
// ...........................................
int charger_write(uint8_t *buf, uint len)
{
    I2C_Write(I2C_CHARGER_ADDR, buf, len);
    while (I2C_IsBusy()) {
        osDelayMs(1);
    }
    return I2C_ErrorGet();
}
// ...........................................
int charger_read(uint8_t *buf, uint len)
{
    I2C_Read(I2C_CHARGER_ADDR, buf, len);
    while (I2C_IsBusy()) {
        osDelayMs(1);
    }
    return I2C_ErrorGet();
}
// ...........................................
int charger_writeread(uint8_t *wrbuf, uint wrlen, uint8_t *rdbuf, uint rdlen)
{
    I2C_WriteRead(I2C_CHARGER_ADDR, wrbuf, wrlen, rdbuf, rdlen);
    while (I2C_IsBusy()) {
        osDelayMs(1);
    }
    return I2C_ErrorGet();
}
// ...........................................
// onoff = 0 -> turn the charger OFF
// onoff = 1 -> turn the charger ON
// ...........................................
void charger_onoff(unsigned onoff)
{
    uint8_t wrbuf[4];

    wrbuf[0] = 0x16; // CHG_CNFG_00
    wrbuf[1] = 0x04 + (onoff & 0x01); // 0x04:charger=OFF 0x05:charger=ON
    charger_write(wrbuf, 2);
}
// ...........................................
// some registers are protected
// lock = 1 -> locked, protected
// lock = 0 -> unlocked
// ...........................................
void charger_lock(bool lock)
{
    uint8_t wrbuf[4];

    wrbuf[0] = 0x1C; // CHG_CNFG_06
    wrbuf[1] = lock ? 0x00 : 0x0C;
    charger_write(wrbuf, 2);
}
// ...........................................
#define CHARGER_LOCK()      charger_lock(true)
#define CHARGER_UNLOCK()    charger_lock(false)
// ...........................................
// mA --> milliampers (100..3500 for MAX77985)
// ...........................................
void charger_set_CC(unsigned mA)
{
    uint8_t wrbuf[4];

    if (mA < 100) mA = 100;
    if (mA > 3500) mA = 3500;
    
    CHARGER_UNLOCK();
    
    wrbuf[0] = 0x18;        // CHG_CNFG_02 - fast charging current
    wrbuf[1] = mA / 50;
    charger_write(wrbuf, 2);
    
    wrbuf[0] = 0x1F;        // CHG_CNFG_09 - input limit
    wrbuf[1] = mA / 50;     // this will set the limit 50mA higher than CC
    charger_write(wrbuf, 2);

    CHARGER_LOCK();
}
// ...........................................
*/

// *****************************************************************************
// Wear Sesnor
// *****************************************************************************

// .............................................................................
// Initialise the Wear Sensor
// .............................................................................
void wear_init()
{
    hx32062se_id_check();       // read and verify device id (between 0x32 and 0x3A)
    hx32062se_reg_init();       // write to / intialise all the registers
	hx32062se_als_disable();    // disable ALS (ambient light sensor)
	hx32062se_ps_enable();      // enable proximity sensor
}

// ********************************************
// I2C Read/Write functions for Wear Sensor
// ********************************************

// .............................................................................
int wear_write(uint8_t *buf, uint len)
{
    I2C_Write(I2C_WEAR_ADDR, buf, len);
    while (I2C_IsBusy()) {
        osDelayMs(1);
    }
    return I2C_ErrorGet();
}
// .............................................................................
int wear_read(uint8_t *buf, uint len)
{
    I2C_Read(I2C_WEAR_ADDR, buf, len);
    while (I2C_IsBusy()) {
        osDelayMs(1);
    }
    return I2C_ErrorGet();
}
// .............................................................................
int wear_writeread(uint8_t *wrbuf, uint wrlen, uint8_t *rdbuf, uint rdlen)
{
    I2C_WriteRead(I2C_WEAR_ADDR, wrbuf, wrlen, rdbuf, rdlen);
    while (I2C_IsBusy()) {
        osDelayMs(1);
    }
    return I2C_ErrorGet();
}
// .............................................................................

// *****************************************************************************
// Accelerometer
// *****************************************************************************

// .............................................................................
// Accelerometer call back function.
// We are only interested in movement yes/no, so we just increment a counter
// every time the accelerometer detects any motion.
// .............................................................................
void EIC_PIN14_Callback(uintptr_t context)
{
    INT_G_cnt++;
}

// .............................................................................
// The accelerometer can be configured to trigger an interrupt on:
//  (a) No Motion,  (b) Significant Motion, or (c) Any Motion.
// We have configured it for Any Motion.
// -	See the call qmi8658a_any_motion(thd, win)
// -	This function writes TWICE to the CTRL_9 command register
// -	The FIRST command sets the THRESHOLD value
// -	The SECOND command sets the WINDOW
// The threshold value defines the SLOPE of the acceleration required ? the 
// larger the slope value, the bigger the movement required, to generate the 
// interrupt. The units are 1/32.
// The window defines the DURATION, so it is a form of debounce.
// .............................................................................
void accel_init()
{
    EIC_CallbackRegister(EIC_PIN_14, EIC_PIN14_Callback, 0);
    EIC_InterruptEnable(EIC_PIN_14);
    
    qmi8658a_autoinc();                                             // enable address auto-increment
    qmi8658a_id_check();                                            // read and verify device id	(QMI8658A_CHIP_ID_VAL, QMI8658A_REV_ID_VAL)
    //qmi8658a_any_motion(4,3);                                     // threshold = 4/32g (0.125g), window = 3 (number of samples)
    qmi8658a_any_motion(ACCEL_HI_SENSE_THD, ACCEL_HI_SENSE_WIN);    // threshold = n/32g (0.125g), window = n (number of samples)
    qmi8658a_enable_accel();                                        // enable
}

// .............................................................................
// Use the following function to modify the acclerometer sensitivity
// AFTER it has been previously enabled
// .............................................................................
void accel_sensitivity(uint8_t thd, uint8_t win)
{
    qmi8658a_disable_accel();       // disable (temporarily)
    qmi8658a_any_motion(thd,win);   // set the trheshold and window values for Any Motion
    qmi8658a_enable_accel();        // enable
}

// ********************************************
// I2C Read/Write functions for Accelerometer
// ********************************************

// .............................................................................
int accel_write(uint8_t *buf, uint len)
{
    I2C_G_Write(I2C_G_ADDR, buf, len);
    while (I2C_G_IsBusy()) {
        osDelayMs(1);
    }
    return I2C_G_ErrorGet();
}
// .............................................................................
int accel_read(uint8_t *buf, uint len)
{
    I2C_G_Read(I2C_G_ADDR, buf, len);
    while (I2C_G_IsBusy()) {
        osDelayMs(1);
    }
    return I2C_G_ErrorGet();
}
// .............................................................................
int accel_writeread(uint8_t *wrbuf, uint wrlen, uint8_t *rdbuf, uint rdlen)
{
    I2C_G_WriteRead(I2C_G_ADDR, wrbuf, wrlen, rdbuf, rdlen);
    while (I2C_G_IsBusy()) {
        osDelayMs(1);
    }
    return I2C_G_ErrorGet();
}
// .............................................................................

// *****************************************************************************


// .............................................................................
// Task initializations.
// This function runs BEFORE task is created and scheduler started.
// .............................................................................
void APP_SNS_Initialize ( void )
{
}


// .............................................................................
// Sensor task
//
// This task manages the Wear Sensor and Accelerometer sensors, and Touch 
// Sensor input.
//
// The task?s main loop schedules the following:
//  Reading the latest Wear Sensor status
//  Reading the latest Accelerometer data
//  Sending this data (wear sensor and accelerometer) to the Power task (via 
//  the Q_pwr_sns queue).
//
// Note:   This task?s scheduler is very crude ? it uses a counter in the task 
//          loop. This could be changed at a later date to use a schedule 
//          counter managed by the TimerCallback() function in the Encoder file.
// .............................................................................
void APP_SNS_Tasks ( void )
{
    Q_sensors_t qs_sns;
    uint8_t wrbuf[8], rdbuf[16];
    //uint8_t chg;
    uint32_t INT_G_cnt_previous;

    // Task waits Power Task has powered everything up
    while (!GlobalPowerOn) {
        osDelayMs(100);
    }
    /*
    touch_init();
    wrbuf[0] = 26; // Xmax
    wrbuf[1] = 0;
    wrbuf[2] = 14; // Ymax
    wrbuf[3] = 0;
    wrbuf[4] = 1; // touch no.    
    touch_writeto(0x8048, wrbuf, 5);
    wrbuf[0] = 1; // enable touch key?
    touch_writeto(0x804E, wrbuf, 1);
    */ 
    
    wear_init();            // Wear Sensor
    osDelayMs(1000);
    //semi_touch_init();      // Touch Screen
    
    //charger_init();
    //osDelayMs(100);
    //chg = battery_charging;
    //charger_onoff(chg);
    //charger_set_CC(1000); // set charging current to 1A
            
    // Accelerometer
    accel_init();
    //init cw221x(batt)
    batt_cw221x_init();

    //init charger
    charger_init();
    osDelayMs(20);
    // Always save last movement counter, so can tell if any movement since last check
    INT_G_cnt_previous = INT_G_cnt;
    
    // Wear Sensor
    wear_state_new = 0;
    wear_state_old = 0;
    wear_state_dbnc = 0;
    wear_dbnc_cntr = 0;

    schedule_index = 0;
    
    while(1) 
    {
        OS_SLEEP(10);           // 10 ms delay, used for scheduling below
        schedule_index++;

#if 0        
        // .......................................
        // Testing the Touch
        // .......................................
        //OS_SLEEP(2000); //osDelayMs(2000);
        // Touch
        wrbuf[0] = 0x81; // register high address byte
        wrbuf[1] = 0x40; // register low address byte
        DBGprintf("touch wr [0x8140] %d\n", touch_write(wrbuf,2));
        DBGprintf("touch rd %d %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X\n", 
            touch_read(rdbuf,11),
            rdbuf[0], rdbuf[1], rdbuf[2], rdbuf[3], rdbuf[4], rdbuf[5],
            rdbuf[6], rdbuf[7], rdbuf[8], rdbuf[9], rdbuf[10]);
        wrbuf[0] = 0x81; // register high address byte
        wrbuf[1] = 0x4E; // register low address byte
        DBGprintf("touch wr [0x814E] %d\n", touch_write(wrbuf,2));
        DBGprintf("touch rd %d %02X\n", touch_read(rdbuf,1), rdbuf[0]);

        touch_readfrom(0x8048, rdbuf, 5);
        DBGprintf("touch Xmax=%d Ymax=%d No=%d\n", 
                (int)256*rdbuf[1]+rdbuf[0], 
                (int)256*rdbuf[3]+rdbuf[2],
                (int)rdbuf[4]); 
        
        
        wrbuf[0] = 0x80; // register high address byte
        wrbuf[1] = 0x48; // register low address byte
        touch_write(wrbuf,2);
        // .......................................
#endif        

#if 1        
        // .......................................
        // Wear Sensor
        // .......................................
        // Wear sensor
        // Sample the wear sensor every 250 ms
        //if (schedule_index == 25) {
        if ((schedule_index == 30) || (schedule_index == 60) || (schedule_index == 90)) 
        {
            wear_state_new = hx32062se_ps_sample();
            
            // Debounce the wear sensor
            if (wear_state_new == 0) {
                if (wear_state_old == 1) {
                    // Reset debounce counter
                    wear_dbnc_cntr= 0;
                } else {
                    if (wear_state_dbnc == 1) {
                        // Current state is different from debounced state
                        wear_dbnc_cntr++;
                        if (wear_dbnc_cntr >= WEAR_DEBOUNCE_MAX_FAR) {
                            // Just reached debounced state, set flag
                            wear_dbnc_cntr = 0;
                            wear_state_dbnc = 0;
                        }
                    }
                }
            } else {
                if (wear_state_old == 0) {
                    // Reset debounce counter
                    wear_dbnc_cntr= 0;
                } else {
                    if (wear_state_dbnc == 0) {
                        // Current state is different from debounced state
                        wear_dbnc_cntr++;
                        if (wear_dbnc_cntr >= WEAR_DEBOUNCE_MAX_NEAR) {
                            // Just reached debounced state, set flag
                            wear_dbnc_cntr = 0;
                            wear_state_dbnc = 1;
                        }
                    }
                }
            }
            wear_state_old = wear_state_new;
            }
        // .......................................
#endif

#if 0        
        // .......................................
        // Charger
        // .......................................
        OS_SLEEP(2000); //osDelayMs(2000);
        wrbuf[0] = 0x00; // register address byte
        DBGprintf("charger rd %d %02X\n", charger_writeread(wrbuf,1,rdbuf,1), rdbuf[0]);
        DBGprintf("charging %s\n", battery_charging ? "ON" : "OFF");
        if (battery_charging != chg) {
            chg = battery_charging;
            charger_onoff(chg);
        }
        // .......................................
#endif

#if 1        
        // .......................................
        // Accelerometer
        // .......................................
        if (schedule_index == 50) {                    // 600 ms
            wrbuf[0] = 0x35;                        // register address byte
            accel_writeread(wrbuf,1,rdbuf,6);       // X: [0..1], Y: [2..3], Z: [4..5]
            if (g_INT_G_cnt_previous != INT_G_cnt) {
                qs_sns.b_accel = true;
            } else {
                qs_sns.b_accel = false;
            }
            g_INT_G_cnt_previous = INT_G_cnt;
        }
        // .......................................
#endif
        
        
        if (schedule_index >= 100) {                   // 1 second
            schedule_index = 0;
            // Send latest Accelerometer and Wear Sensor data to Power task
//            qs_sns.wear_status = wear_state_dbnc;
//            osQueueSendToBack(Q_pwr_sns, &qs_sns, 10);
        }
        
        
#if 0        
        // .......................................
        if(!INT_TOUCH_L_Get())
        {
            semi_touch_irq_handler_imp();
        }        
        // .......................................
#endif
        
    }
        // .......................................
}
// .............................................................................
