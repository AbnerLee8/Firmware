/* hw_qmi8658a.h */
#ifndef _HW_QMI8658A_H
#define _HW_QMI8658A_H

#include "app.h"

/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif

/* The I2C interface */
extern int accel_write(uint8_t *buf, uint len);
extern int accel_read(uint8_t *buf, uint len);
extern int accel_writeread(uint8_t *wrbuf, uint wrlen, uint8_t *rdbuf, uint rdlen);

#define QMI8658A_CHIP_ID_VAL                 0x05
#define QMI8658A_REV_ID_VAL                  0x7C
#define QMI8658A_ID_CHECK_COUNT     5

// General Purpose Registers
#define RA_WHO_AM_I     0   // Device Identifier
#define RA_REVISION_ID  1   // Device Revision ID
// Setup and Control Registers
#define RA_CTRL1        2   // SPI Interface and Sensor Enable
#define RA_CTRL2        3   // Accelerometer: Output Data Rate, Full Scale, Self-Test
#define RA_CTRL3        4   // Gyroscope: Output Data Rate, Full Scale, Self-Test
#define RA_CTRL5        6   // Low pass filter setting
#define RA_CTRL7        8   // Enable Sensors
#define RA_CTRL8        9   // Motion Detection Control
#define RA_CTRL9        10  // Host Commands
// Host Controlled Calibration Registers (See CTRL9, Usage is Optional)
#define RA_CAL1_L       11  // Calibration Register CAL1_L ? lower 8 bits. CAL1_H ? upper 8 bits.
#define RA_CAL1_H       12
#define RA_CAL2_L       13  // Calibration Register CAL2_L ? lower 8 bits. CAL2_H ? upper 8 bits.
#define RA_CAL2_H       14
#define RA_CAL3_L       15  // Calibration Register CAL3_L ? lower 8 bits. CAL3_H ? upper 8 bits.
#define RA_CAL3_H       16
#define RA_CAL4_L       17  // Calibration Register CAL4_L ? lower 8 bits. CAL4_H ? upper 8 bits.
#define RA_CAL4_H       18
// FIFO Registers
#define RA_FIFO_WTM_TH      19  // FIFO watermark level, in ODRs
#define RA_FIFO_CTRL        20  // FIFO Setup
#define RA_FIFO_SMPL_CNT    21 // FIFO sample count LSBs
#define RA_FIFO_STATUS      22  // FIFO Status
#define RA_FIFO_DATA        23  // FIFO Data
// Status Registers
#define RA_STATUSINT    45  // Sensor Data Availability with the Locking mechanism, CmdDone (CTRL9 protocol bit).
#define RA_STATUS0      46  // Output Data Over Run and Data Availability.
#define RA_STATUS1      47  // Miscellaneous Status: Any Motion, No Motion, Significant Motion, Pedometer, Tap.
// Timestamp Register
#define RA_TIMESTAMP_LOW    48 // Sample Time Stamp TIMESTAMP_LOW ? lower 8 bits. TIMESTAMP_MID ? middle 8 bits. TIMESTAMP_HIGH ? upper 8 bits
#define RA_TIMESTAMP_MID    49
#define RA_TIMESTAMP_HIGH   50
// Data Output Registers (16 bits 2?s Complement Except COD Sensor Data)
#define RA_TEMP_L   51  // Temperature Output Data TEMP_L ? lower 8 bits. TEMP_H ? upper 8 bits 
#define RA_TEMP_H   52
#define RA_AX_L     53  // X-axis Acceleration AX_L ? lower 8 bits. AX_H ? upper 8 bits 
#define RA_AX_H     54
#define RA_AY_L     55  // Y-axis Acceleration AY_L ? lower 8 bits. AY_H ? upper 8 bits 
#define RA_AY_H     56
#define RA_AZ_L     57  // Z-axis Acceleration AZ_L ? lower 8 bits. AZ_H ? upper 8 bits 
#define RA_AZ_H     58
#define RA_GX_L     59  // X-axis Angular Rate GX_L ? lower 8 bits. GX_H ? upper 8 bits 
#define RA_GX_H     60
#define RA_GY_L     61  // Y-axis Angular Rate GY_L ? lower 8 bits. GY_H ? upper 8 bits 
#define RA_GY_H     62
#define RA_GZ_L     63  // Z-axis Angular Rate GZ_L ? lower 8 bits. GZ_H ? upper 8 bits 
#define RA_GZ_H     64
// COD Indication and General Purpose Registers
#define RA_COD_STATUS 70  // Calibration-On-Demand status register 
#define RA_dQW_L    73  // General purpose register 
#define RA_dQW_H    74  // General purpose register 
#define RA_dQX_L    75  // General purpose register 
#define RA_dQX_H    76  // Reserved 
#define RA_dQY_L    77  // General purpose register 
#define RA_dQY_H    78  // Reserved 
#define RA_dQZ_L    79  // Reserved 
#define RA_dQZ_H    80  // Reserved 
#define RA_dVX_L    81  // General purpose register 
#define RA_dVX_H    82  // General purpose register 
#define RA_dVY_L    83  // General purpose register 
#define RA_dVY_H    84  // General purpose register 
#define RA_dVZ_L    85  // General purpose register 
#define RA_dVZ_H    86  // General purpose register
// Activity Detection Output Registers
#define RA_TAP_STATUS       89  // Axis, direction, number of detected Tap 
#define RA_STEP_CNT_LOW     90  // Low byte of step count of Pedometer 
#define RA_STEP_CNT_MIDL    91  // Middle byte of step count of Pedometer 
#define RA_STEP_CNT_HIGH    92  // High byte of step count of Pedometer 
// Reset Register 
#define RA_RESET    96  // Soft Reset Register

// List of CTRL9 Commands
#define CTRL_CMD_ACK                    0x00 // Ctrl9 Acknowledgement. Host acknowledges to QMI8658, to end the protocol.
#define CTRL_CMD_RST_FIFO               0x04 // Ctrl9 Reset FIFO from Host
#define CTRL_CMD_REQ_FIFO               0x05 // Ctrl9R Get FIFO data from Device
#define CTRL_CMD_WRITE_WOM_SETTING      0x08 // WCtrl9 Set up and enable Wake on Motion (WoM)
#define CTRL_CMD_ACCEL_HOST_DELTA_OFFSET 0x09 //WCtrl9 Change accelerometer offset
#define CTRL_CMD_GYRO_HOST_DELTA_OFFSET 0x0A // WCtrl9 Change gyroscope offset
#define CTRL_CMD_CONFIGURE_TAP          0x0C // WCtrl9 Configure Tap detection
#define CTRL_CMD_CONFIGURE_PEDOMETER    0x0D // WCtrl9 Configure Pedometer
#define CTRL_CMD_CONFIGURE_MOTION       0x0E // WCtrl9 Configure Any Motion / No Motion / Significant Motion detection
#define CTRL_CMD_RESET_PEDOMETER        0x0F // WCtrl9 Reset pedometer count (step count)
#define CTRL_CMD_COPY_USID              0x10 // Ctrl9R Copy USID and FW Version to UI registers
#define CTRL_CMD_SET_RPU                0x11 // WCtrl9 Configures IO pull-ups
#define CTRL_CMD_AHB_CLOCK_GATING       0x12 // WCtrl9 Internal AHB clock gating switch
#define CTRL_CMD_ON_DEMAND_CALIBRATION  0xA2 // WCtrl9 On-Demand Calibration on gyroscope
#define CTRL_CMD_APPLY_GYRO_GAINS       0xAA // WCtrl9 Restore the saved Gyroscope gains
        
// MOTION_MODE_CTRL bits
#define BM_NoMotionAxisLogic    0x80
#define BM_NoMotionEnZ          0x40
#define BM_NoMotionEnY          0x20
#define BM_NoMotionEnX          0x10
#define BM_AnyMotionAxisLogic   0x08
#define BM_AnyMotionEnZ         0x04
#define BM_AnyMotionEnY         0x02
#define BM_AnyMotionEnX         0x01

// CTRL8 Motion Detection Control Bits
#define BM_CTRL9_HandShake_Type_INT1           0x00
#define BM_CTRL9_HandShake_Type_STATUSINTbit7  0x80
#define BM_ACTIVITY_INT_SEL_INT2    0x00
#define BM_ACTIVITY_INT_SEL_INT1    0x40
#define BM_Pedo_EN          0x10
#define BM_Sig_Motion_EN    0x08
#define BM_No_Motion_EN     0x04
#define BM_Any_Motion_EN    0x02
#define BM_Tap_EN           0x01

void qmi8658a_autoinc();
int qmi8658a_id_check(void);
void qmi8658a_disable_accel();
void qmi8658a_enable_accel();
void qmi8658a_enable_gyro();
void qmi8658a_any_motion(uint8_t thd, uint8_t win);
int qmi8658a_any_motion_status();

/* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _HW_QMI8658A_H */
