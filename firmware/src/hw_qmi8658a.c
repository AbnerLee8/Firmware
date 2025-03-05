/******************************************************************************
  @File Name
    hw_qmi8658a.c

  @Summary
    QMI8658A chip driver (accelerometer)

  @Description
    
*******************************************************************************/
#include "hw_qmi8658a.h"

// .............................................................................
// Low-level function to read from Acceleroemter
// return 0 for success, return 1 for errors.
// .............................................................................
int qmi8658a_read(uint8_t addr, uint8_t *rxbuf, int count)
{
    int ret;
    ret = accel_write(&addr, 1);
    if (ret) return ret;
    ret = accel_read(rxbuf, count);  
    return ret;
}

// .............................................................................
// Low-level function to write to Acceleroemter
// Return 0 for success, return 1 for errors.
// .............................................................................
int qmi8658a_write(uint8_t addr, uint8_t txdata)
{
    int ret;
    uint8_t buf[2];
    buf[0] = addr;
    buf[1] = txdata;
    ret = accel_write(buf, 2);
    return ret;
}

// .............................................................................
// Read and verify device id (QMI8658A_CHIP_ID_VAL, QMI8658A_REV_ID_VAL)
// .............................................................................
int qmi8658a_id_check(void)
{
    int ret, i;
    uint8_t device_id[2] = {0,0};

    for (i = 0; i < QMI8658A_ID_CHECK_COUNT; i++) {
        ret = qmi8658a_read(RA_WHO_AM_I, device_id, 2);
        if (ret) {
            DBGprintf("accel device_id read error\n");
            continue;
        }
        if (device_id[0] == QMI8658A_CHIP_ID_VAL && device_id[1] == QMI8658A_REV_ID_VAL) {
            DBGprintf("accel device_id=0x%02X,0x%02X", device_id[0], device_id[1]);
            return 0;
        }
    }
    DBGprintf("(UNKNOW_CHIP_ID!)\n");
    return 1;
}

// .............................................................................
// Enable address auto-increment
// .............................................................................
void qmi8658a_autoinc()
{
    uint8_t stat;
    qmi8658a_read(RA_CTRL1, &stat, 1);
    stat |= 0x40;
    qmi8658a_write(RA_CTRL1, stat);
}

// .............................................................................
// Disable accelerometer
// .............................................................................
void qmi8658a_disable_accel()
{
    uint8_t stat;
    qmi8658a_read(RA_CTRL7, &stat, 1);
    stat &= ~0x01;
    qmi8658a_write(RA_CTRL7, stat);
}
// .............................................................................
// Enable accelerometer
// .............................................................................
void qmi8658a_enable_accel()
{
    uint8_t stat;
    qmi8658a_read(RA_CTRL7, &stat, 1);
    stat |= 0x01;
    qmi8658a_write(RA_CTRL7, stat);
}

// .............................................................................
// Enable gyrometer
// .............................................................................
void qmi8658a_enable_gyro()
{
    uint8_t stat;
    qmi8658a_read(RA_CTRL7, &stat, 1);
    stat |= 0x02;
    qmi8658a_write(RA_CTRL7, stat);
}

// .............................................................................
// WCtrl9 (Write ? CTRL9 Protocol)
// 1.   The host needs to provide the required data for this command to the 
//      QMI8658A. 
//      The host typically does this by placing the data in a set of registers 
//      called the CAL registers. 
//      Maximum eight CAL registers are used.
// 2.   Write Ctrl9 register 0x0A with the appropriate Command value.
// 3.   The Device will set STATUSINT.bit7 to 1 once it has executed the 
//      appropriate function 
//      based on the command value.
// 4.   The host must acknowledge this by writing CTRL_CMD_ACK (0x00) to 
//      CTRL9 register, 
//      STATUSINT.bit7 (CmdDone) will be reset to 0 on receiving the 
//      CTRL_CMD_ACK command.
// 5.   If any data is expected from the device, it will be available at this 
//      time. 
//      The location of the data is specified separately for each of the 
//      Commands.
// .............................................................................
void qmi8658a_command(uint8_t cmd)
{
    uint8_t stat;

    qmi8658a_write(RA_CTRL8, BM_CTRL9_HandShake_Type_STATUSINTbit7);
            
    // write the command
    qmi8658a_write(RA_CTRL9, cmd);
    // wait for CmdDone = 1
    while (1) {
        qmi8658a_read(RA_STATUSINT, &stat, 1);
        if (stat & 0x80) { // CmdDone==1
            break;            
        }
        osDelayMs(2);
    }
    DBGprintf("accel cmd_done=1\n");

    // write the command acknowledge
    qmi8658a_write(RA_CTRL9, CTRL_CMD_ACK);
    // wait for CmdDone = 0
    while (1) {
        qmi8658a_read(RA_STATUSINT, &stat, 1);
        if ((stat & 0x80) == 0) { // CmdDone==0
            break;            
        }
        osDelayMs(2);
    }    
    DBGprintf("accel cmd_done=0\n");
}

// .............................................................................
// The accelerometer can be configured to trigger an interrupt on:
//  (a) No Motion,  (b) Significant Motion, or (c) Any Motion.
// Configure the accelerometer for Any Motion detection.
// Inputs:
//  thd:    slope threshold, 1-byte format unsigned, 5-bits fraction
//          (1/32 = 0.03125g)
//  win:    Defines the minimum number of consecutive samples (duration) that 
//          the absolute of the slope of the enabled axis/axes data should keep 
//          higher than the threshold
// .............................................................................
void qmi8658a_any_motion(uint8_t thd, uint8_t win)
{
    qmi8658a_write(RA_CAL1_L, thd); // AnyMotionXThr.
    qmi8658a_write(RA_CAL1_H, thd); // AnyMotionYThr.
    qmi8658a_write(RA_CAL2_L, thd); // AnyMotionZThr.
    qmi8658a_write(RA_CAL2_H, 0); // NoMotionXThr.
    qmi8658a_write(RA_CAL3_L, 0); // NoMotionYThr
    qmi8658a_write(RA_CAL3_H, 0); // NoMotionZThr.
    qmi8658a_write(RA_CAL4_L, BM_AnyMotionEnX | BM_AnyMotionEnY | BM_AnyMotionEnZ); // MOTION_MODE_CTRL
    qmi8658a_write(RA_CAL4_H, 1); // first command
    qmi8658a_command(CTRL_CMD_CONFIGURE_MOTION);

    qmi8658a_write(RA_CAL1_L, win); // AnyMotionWindow.
    qmi8658a_write(RA_CAL1_H, 0); // NoMotionWindow.
    qmi8658a_write(RA_CAL2_L, 0); // SigMotionWaitWindow [7:0].
    qmi8658a_write(RA_CAL2_H, 0); // SigMotionWaitWindow [15:8].
    qmi8658a_write(RA_CAL3_L, 0); // SigMotionConfirmWindow[7:0]
    qmi8658a_write(RA_CAL3_H, 0); // SigMotionConfirmWindow[15:8]
    qmi8658a_write(RA_CAL4_L, 0); // N/A
    qmi8658a_write(RA_CAL4_H, 2); // second command
    qmi8658a_command(CTRL_CMD_CONFIGURE_MOTION);

    qmi8658a_write(RA_CTRL2, 12); // aODR 
    qmi8658a_write(RA_CTRL1, 1<<3); // INT1_EN, INT1 output enable (default is HiZ)
    qmi8658a_write(RA_CTRL8, BM_Any_Motion_EN | BM_ACTIVITY_INT_SEL_INT1); // enable any-motion detection (INT1))
}

// .............................................................................
// Read the 'any-motion' status bit from STATUS1
// .............................................................................
int qmi8658a_any_motion_status()
{
    uint8_t stat;
    qmi8658a_read(RA_STATUS1, &stat, 1);
    return ((stat & 0x20) == 0x20);
}
// .............................................................................
