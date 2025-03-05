/* app_mcu.h */
#ifndef _APP_MCU_H
#define _APP_MCU_H

#include "app.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility
extern "C" {
#endif
// DOM-IGNORE-END

#define CMD_ID_MAX      16
#define CMD_PAR_MAX     16
#define CMD_PARNO_MAX   16

typedef struct {
    char idstr[CMD_ID_MAX];
    char parstr[CMD_PARNO_MAX][CMD_PAR_MAX];
    uint parno;
    uint idx;
} cmd_t;


void APP_MCU_Initialize ( void );
void APP_MCU_Tasks( void );

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif /* _APP_MCU_H */
