/*******************************************************************************
  File Name:
    max77985.c

  Summary:
  max77985 charger
  Description:
 *******************************************************************************/
#include "max77985.h"
#include "hw_hx32062se.h"
#include "hw_qmi8658a.h"

// ...........................................
// Charger max77985
// ...........................................
#define I2C_CHARGER_ADDR      0x6B //0xD6

#define I2C_C_Write(s,wa,wn)  SERCOM5_I2C_Write(s,wa,wn)
#define I2C_C_Read(s,ra,rn)   SERCOM5_I2C_Read(s,ra,rn)
#define I2C_C_WriteRead(s,wa,wn,ra,rn)    SERCOM5_I2C_WriteRead(s,wa,wn,ra,rn)
#define I2C_C_IsBusy()        SERCOM5_I2C_IsBusy()
#define I2C_C_ErrorGet()      SERCOM5_I2C_ErrorGet()

#define CHARGER_LOCK()      charger_lock(true)
#define CHARGER_UNLOCK()    charger_lock(false)

// ............................................................................
// .............................................................................
// Return 0 for success, return 1 for errors.
// .............................................................................
int com5_iic_write(uint8_t *buf, int len)
{
    I2C_C_Write(I2C_CHARGER_ADDR, buf, len);
    while (I2C_C_IsBusy()) {
        osDelayMs(1);
    }
    return I2C_C_ErrorGet();
}
// ............................................................................
// .............................................................................
// Return 0 for success, return 1 for errors.
// .............................................................................
int charger_write(uint8_t *buf, int len)
{
    int ret;
    ret = com5_iic_write(buf,len);
    if(ret){
      ret = com5_iic_write(buf,len);
    }
    return ret;
}
// ............................................................................
// onoff = 0 -> turn the charger OFF
// onoff = 1 -> turn the charger ON
// ............................................................................
int charger_onoff(uint8_t onoff)
{
    int ret;
    uint8_t wrbuf[4];

    wrbuf[0] = 0x16; // CHG_CNFG_00
    wrbuf[1] = 0x04 + (onoff & 0x01); // 0x04:charger=OFF 0x05:charger=ON
    ret = charger_write(wrbuf, 2);
    return ret;
}

//init charger
int charger_init(void)
{
    int ret;
    ret = charger_onoff(BATT_CHARGER_ON);
    if(ret){
      printf("charger error\n");
    }else{
      printf("charger ok\n");
    }
    return ret;
}

//uinit charger
int charger_uinit(void)
{
    int ret;
    ret = charger_onoff(BATT_CHARGER_OFF);
    return ret;
}

// ............................................................................
// some registers are protected
// lock = 1 -> locked, protected
// lock = 0 -> unlocked
// ............................................................................
void charger_lock(bool lock)
{
    uint8_t wrbuf[4];

    wrbuf[0] = 0x1C; // CHG_CNFG_06
    wrbuf[1] = lock ? 0x00 : 0x0C;
    charger_write(wrbuf, 2);
}

// ............................................................................
// mA --> milliampers (100..3500 for MAX77985)
// ............................................................................
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
    wrbuf[1] = (mA / 50) + 2;     // this will set the limit 50mA higher than CC
    charger_write(wrbuf, 2);

    wrbuf[0] = 0x1A;        // CHG_CNFG_04 - termination voltage
    wrbuf[1] = 0x14 | 0x20;     // this will set the limit to 4.40V
    charger_write(wrbuf, 2);

    CHARGER_LOCK();
}

