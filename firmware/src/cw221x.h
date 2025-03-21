
#ifndef _CW221X_H
#define _CW221X_H

#include "app.h"
//#include "chsc5xxx/semi_touch_device.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility
extern "C" {
#endif
// DOM-IGNORE-END
#define REG_CHIP_ID             0x00
#define REG_VCELL_H             0x02
#define REG_VCELL_L             0x03
#define REG_SOC_INT             0x04
#define REG_SOC_DECIMAL         0x05
#define REG_TEMP                0x06
#define REG_MODE_CONFIG         0x08
#define REG_GPIO_CONFIG         0x0A
#define REG_SOC_ALERT           0x0B
#define REG_TEMP_MAX            0x0C
#define REG_TEMP_MIN            0x0D
#define REG_CURRENT_H           0x0E
#define REG_CURRENT_L           0x0F
#define REG_T_HOST_H            0xA0
#define REG_T_HOST_L            0xA1
#define REG_USER_CONF           0xA2
#define REG_CYCLE_H             0xA4
#define REG_CYCLE_L             0xA5
#define REG_SOH                 0xA6
#define REG_IC_STATE            0xA7
#define REG_STB_CUR_H           0xA8
#define REG_STB_CUR_L           0xA9
#define REG_FW_VERSION          0xAB
#define REG_BAT_PROFILE         0x10

#define CONFIG_MODE_RESTART     0x30
#define CONFIG_MODE_ACTIVE      0x00
#define CONFIG_MODE_SLEEP       0xF0
#define CONFIG_UPDATE_FLG       0x80
#define IC_VCHIP_ID             0xA0
#define IC_READY_MARK           0x0C
#define IC_TEMP_READY           0x08
#define IC_VOL_CUR_READY        0x04

#define GPIO_SOC_IRQ_VALUE      0x0    /* 0x7F */
#define SIZE_OF_PROFILE         80

#define CW221X_ERROR_IIC      -1
#define CW221X_ERROR_CHIP_ID  -2
#define CW221X_ERROR_TIME_OUT -3
#define CW221X_NOT_ACTIVE          1
#define CW221X_PROFILE_NOT_READY   2
#define CW221X_PROFILE_NEED_UPDATE 3


int i2c_cw221x_write(uint8_t *buf, int len);
int i2c_cw221x_read(uint8_t *buf, int len);
int i2c_cw221x_writeread(uint8_t *wrbuf, int wrlen, uint8_t *rdbuf, int rdlen);
int batt_cw221x_init(void);
int batt_cw221x_action(void);
int batt_cw221x_sleep(void);
int batt_cw221x_temperature(void);
int batt_cw221x_soh_health(void);
int batt_cw221x_voltage(void);
//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif /* _CW221X_H */