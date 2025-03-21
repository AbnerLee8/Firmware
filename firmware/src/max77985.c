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

// ............................................................................
// .............................................................................
// Return 0 for success, return 1 for errors.
// .............................................................................
int charger_write(uint8_t *buf, int len)
{
    I2C_C_Write(I2C_CHARGER_ADDR, buf, len);
    while (I2C_C_IsBusy()) {
        osDelayMs(1);
    }
    return I2C_C_ErrorGet();
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
void charger_init(void)
{
  int ret;
  ret = charger_onoff(BATT_CHARGER_ON);
  if(ret){
    printf("charger error\n");
  }else{
    printf("charger ok\n");
  }
}

//uinit charger
void charger_uinit(void)
{
  charger_onoff(BATT_CHARGER_OFF);
}