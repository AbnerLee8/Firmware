/* app_sns.h */
#ifndef _MAX77985_H
#define _MAX77985_H

#include "app.h"
//#include "chsc5xxx/semi_touch_device.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility
extern "C" {
#endif
// DOM-IGNORE-END
typedef  enum BATT_CHARGER_STATE{
    BATT_CHARGER_OFF = 0,
    BATT_CHARGER_ON = 1
}BATT_CHARGER_STATE_E;

int charger_write(uint8_t *buf, int len);
int charger_onoff(uint8_t onoff);
void charger_init(void);
void charger_uinit(void);
void charger_lock(bool lock);
void charger_set_CC(unsigned mA);
//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif /* _MAX77985_H */
