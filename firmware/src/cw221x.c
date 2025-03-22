/*******************************************************************************
  File Name:
    cw221x.c

  Summary:
  max77985 charger
  Description:
 *******************************************************************************/
#include "cw221x.h"
#include "hw_hx32062se.h"
#include "hw_qmi8658a.h"

// ...........................................
// batt cw221x
// ...........................................
#define I2C_CW221X_ADDR      0x64 //0xD6

#define I2C_CW_Write(s,wa,wn)  SERCOM5_I2C_Write(s,wa,wn)
#define I2C_CW_Read(s,ra,rn)   SERCOM5_I2C_Read(s,ra,rn)
#define I2C_CW_WriteRead(s,wa,wn,ra,rn)    SERCOM5_I2C_WriteRead(s,wa,wn,ra,rn)
#define I2C_CW_IsBusy()        SERCOM5_I2C_IsBusy()
#define I2C_CW_ErrorGet()      SERCOM5_I2C_ErrorGet()

unsigned char config_cw221x_profile_info[SIZE_OF_PROFILE] = {
	0x5A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xB0, 0xAD, 0xB1, 0xC7, 0xBA, 0xC0, 0xA5, 0x72,
	0x58, 0xFF, 0xFF, 0xEC, 0xB1, 0x8C, 0x6D, 0x58,
	0x4D, 0x48, 0x42, 0x8B, 0x3E, 0xDA, 0xC8, 0xC7,
	0xCB, 0xCE, 0xCE, 0xCD, 0xCB, 0xC9, 0xC2, 0xD2,
	0xC4, 0xC4, 0xB0, 0xA0, 0x94, 0x8B, 0x84, 0x7A,
	0x73, 0x7E, 0x88, 0x92, 0xAB, 0x68, 0x4E, 0x46,
	0x20, 0x00, 0xAB, 0x10, 0x00, 0x90, 0x77, 0x00,
	0x00, 0x00, 0x64, 0x1B, 0xB3, 0x7D, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xDD,
};

// ********************************************
// I2C Read/Write functions for cw221x
// ********************************************

// ............................................................................
// .............................................................................
// Return 0 for success, return 1 for errors.
// .............................................................................
int iic_write(uint8_t *buf, int len)
{
    I2C_CW_Write(I2C_CW221X_ADDR, buf, len);
    while (I2C_CW_IsBusy()) {
        osDelayMs(1);
    }
    return I2C_CW_ErrorGet();
}

// ............................................................................
// .............................................................................
// Return 0 for success, return 1 for errors.
// .............................................................................
int iic_read(uint8_t *buf, int len)
{
    I2C_CW_Read(I2C_CW221X_ADDR, buf, len);
    while (I2C_CW_IsBusy()) {
        osDelayMs(1);
    }
    return I2C_CW_ErrorGet();
}

// ............................................................................
// .............................................................................
// Return 0 for success, return 1 for errors.
// .............................................................................
int iic_writeread(uint8_t *wrbuf, int wrlen, uint8_t *rdbuf, int rdlen)
{
    I2C_CW_WriteRead(I2C_CW221X_ADDR, wrbuf, wrlen, rdbuf, rdlen);
    while (I2C_CW_IsBusy()) {
        osDelayMs(1);
    }
    return I2C_CW_ErrorGet();
}


// .............................................................................
int i2c_cw221x_write(uint8_t *buf, int len)
{
    int ret;
    ret = iic_write(buf,len);
    if(ret){
        ret = iic_write(buf,len);
    }
    return ret;
}

int i2c_cw221x_read(uint8_t *buf, int len)
{
    int ret;
    ret = iic_read(buf,len);
    if(ret){
        ret = iic_read(buf,len);
    }
    return ret;
}

// .............................................................................
int i2c_cw221x_writeread(uint8_t *wrbuf, int wrlen, uint8_t *rdbuf, int rdlen)
{
    int ret;
    ret = iic_writeread(wrbuf,wrlen,rdbuf,rdlen);
    if(ret){
        ret = iic_writeread(wrbuf,wrlen,rdbuf,rdlen);
    }
    return ret;
}

int batt_cw221x_init(void)
{
    uint8_t wrbuf[2], rdbuf[2];
    wrbuf[0] = REG_CHIP_ID; // read the chip id
    if (i2c_cw221x_writeread(wrbuf,1, rdbuf,1)) {
        return -1;
    }
    printf("cw221x id %x\n",rdbuf[0]);

    osDelayMs(10);
    wrbuf[0] = REG_MODE_CONFIG; // read the chip mode
    if (i2c_cw221x_writeread(wrbuf,1, rdbuf,1)) {
        return -1;
    }
    printf("cw221x mode %x\n",rdbuf[0]);

    osDelayMs(10);
    wrbuf[0] = REG_SOC_ALERT; // read the chip soc
    if (i2c_cw221x_writeread(wrbuf,1, rdbuf,1)) {
        return -1;
    }
    printf("cw221x soc %x\n",rdbuf[0]);

    //osDelay(10);
    //batt_cw221x_sleep();

    osDelayMs(20);
    /* update new battery info */
    for (int i = 0; i < SIZE_OF_PROFILE; i++)
    {
        wrbuf[0] = REG_BAT_PROFILE + i; 
        wrbuf[1] = config_cw221x_profile_info[i]; 
        i2c_cw221x_write(wrbuf,2);
        osDelayMs(20);
    }

    /* set UPDATE_FLAG AND SOC INTTERRUP VALUE*/
    osDelayMs(20);
    wrbuf[0] = REG_SOC_ALERT; // config register
    wrbuf[1] = CONFIG_UPDATE_FLG | GPIO_SOC_IRQ_VALUE; 
    if (i2c_cw221x_write(wrbuf, 2)) {
        return -1;
    }

    /*close all interruptes*/
    osDelayMs(100);
    wrbuf[0] = REG_GPIO_CONFIG; // config register
    wrbuf[1] = 0x00; 
    if (i2c_cw221x_write(wrbuf, 2)) {
        return -1;
    }

    batt_cw221x_action();
    return 0;
}

// ............................................................................
// Enter the active mode after a power-up
// The fuel gauge chip is in shutdown mode at power-up
// return -1 if i2c error
// return 0 if ok
// ............................................................................
int batt_cw221x_action(void)
{
    uint8_t wrbuf[2];
    wrbuf[0] = 0x08; // config register
    wrbuf[1] = 0x30; // clear sleep
    if (i2c_cw221x_write(wrbuf, 2)) {
        return -1;
    }

    //cw221x_active
    osDelayMs(100);
    wrbuf[0] = 0x08; // config register
    wrbuf[1] = 0x00; // clear restart
    if (i2c_cw221x_write(wrbuf, 2)) {
        return -1;
    }
    return 0;
}

// ............................................................................
// Enter the sleep mode after a power-down
// The fuel gauge chip is in action mode at power-down
// return -1 if i2c error
// return 0 if ok
// ............................................................................
int batt_cw221x_sleep(void)
{
    uint8_t wrbuf[4];
    wrbuf[0] = 0x08; // config register
    wrbuf[1] = 0x30; // clear sleep
    if (i2c_cw221x_write(wrbuf, 2)) {
        return -1;
    }

    //cw221x_sleep
    osDelayMs(100);
    wrbuf[0] = 0x08; // config register
    wrbuf[1] = CONFIG_MODE_SLEEP; // enter sleep
    if (i2c_cw221x_write(wrbuf, 2)) {
        return -1;
    }
    return 0;
}

// ............................................................................
// Read the battery temp 
// return batt temp
// return -1 if i2c error
// ............................................................................
int batt_cw221x_temperature(void)
{
    uint8_t wrbuf[2], rdbuf[2];
    int T;
    wrbuf[0] = 0x06; // read the battery temperature
    if (i2c_cw221x_writeread(wrbuf,1, rdbuf,1)) {
        return -1;
    }
    
    T = (int)((rdbuf[0] * 10 / 2) - 400);
    return T;
}

// ............................................................................
// Read the battery soh health 
// return batt soc
// return -1 if i2c error
// ............................................................................
int batt_cw221x_soh_health(void)
{
	uint8_t wrbuf[2], rdbuf[2];
  int SOH;
  wrbuf[0] = 0xA6; // read the battery soh
  if (i2c_cw221x_writeread(wrbuf,1, rdbuf,1)) {
      return -1;
  }
	SOH = rdbuf[0] + 5;
	if(SOH >= 100){
		SOH = 100;
	}
	return SOH;
}

// ............................................................................
// Read the battery voltage
// return the voltage in mV
// return -1 if i2c error
// ............................................................................
int batt_cw221x_voltage(void)
{
    uint8_t wrbuf[4], rdbuf[4];
    int V;
    wrbuf[0] = 0x02; // read the battery voltage
    if (i2c_cw221x_writeread(wrbuf,1, rdbuf,2)) {
        return -1;
    }
    V = rdbuf[0]; // msb
    V = V * 256 + rdbuf[1]; // lsb
    V *= 3125;
    V /= 10000; // mV
    return V;
}

// ............................................................................
// Read the battery SOC (state of charge))
// return the SOC in 1%
// return -1 if i2c error
// ............................................................................
int batt_cw221x_soc(void)
{
    uint8_t wrbuf[2], rdbuf[2];
    int soc;
    wrbuf[0] = 0x04; // read the battery SOC (only the 1% register)
    if (i2c_cw221x_writeread(wrbuf,1, rdbuf,2)) {
        return -1;
    }

    soc = rdbuf[0]*256 + rdbuf[1];
    return soc;
}