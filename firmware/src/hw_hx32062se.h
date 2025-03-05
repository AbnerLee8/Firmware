/* hw_hx32062se.h */
#ifndef _HW_HX32062SE_H
#define _HW_HX32062SE_H

#include "app.h"

/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif

/* The I2C interface */
extern int wear_write(uint8_t *buf, uint len);
extern int wear_read(uint8_t *buf, uint len);

#define HX_ARRAY_SIZE(a) ((int)(sizeof(a)/sizeof((a)[0])))

#define HX32062SE_DATA_LOCK         (1)
#define HX32062SE_DATA_UNLOCK       (0)
#define HX32062SE_PS_STATUS_NEAR    (1)
#define HX32062SE_PS_STATUS_FAR     (0)
#define HX32062SE_PROX_STATE_INIT   (-1)
#define HX32062SE_MAX_PS_THRESHOLD  (0xFFFF)

#if 0
// This is the original
// 0x37 is missing and we get exactly 0x37!
#define REV_CORE_ID        0x32
#define REV_A_ID           0x33
#define REV_B_ID           0x34
#define REV_C_ID           0x35
#define REV_D_ID           0x36
#define REV1S2_CORE_ID     0x38
#define REV1S2_B_ID        0x39
#define REV1S2_C_ID        0x3A
#else
// This is my guess:
#define REV_CORE_ID         0x32
#define REV_A_ID            0x33
#define REV_B_ID            0x34
#define REV_C_ID            0x35
#define REV_D_ID            0x36
#define REV1S2_CORE_ID      0x37
#define REV1S2_A_ID         0x38
#define REV1S2_B_ID         0x39
#define REV1S2_C_ID         0x3A
#define REV_ID_START        0x32
#define REV_ID_END          0x3A
#endif

#define HX32062SE_CHIP_ID_VAL                 0x38
#define HX32062SE_ID_CHECK_COUNT       5

// register list:
#define RA_00_DEVICE_ID                       0x00
#define RA_01_ALS1_DATA0                      0x01
#define RA_02_ALS1_DATA1                      0x02
#define RA_03_ALS1_DATA2                      0x03
#define RA_04_ALS2_DATA0                      0x04
#define RA_05_ALS2_DATA1                      0x05
#define RA_06_ALS2_DATA2                      0x06
#define RA_07_ALS3_DATA0                      0x08
#define RA_08_ALS3_DATA1                      0x09
#define RA_0D_PS_RAW_DATA0                    0x0D
#define RA_0E_PS_RAW_DATA1                    0x0E
#define RA_0F_PS_RAW_DATA2                    0x0F
#define RA_10_PS_BL_DATA0                     0x10
#define RA_11_PS_BL_DATA1                     0x11
#define RA_12_PS_BL_DATA2                     0x12
#define RA_69_PS_DIFF_DATA0                   0x69
#define RA_6A_PS_DIFF_DATA1                   0x6A
#define RA_13_DATA_RDY_CTRL0                  0x13
#define RA_14_DATA_RDY_CTRL1                  0x14
#define RA_15_DATA_RDY_FLAG0                  0x15
#define RA_16_DATA_RDY_FLAG1                  0x16
#define RA_17_DATA_RDY_FLAG2                  0x17
#define RA_18_DATA_RDY_FLAG3                  0x18
#define RA_19_AFE_AMP_CHOP_CTRL               0x19
#define RA_1A_GLOBAL_CTRL0                    0x1A
#define RA_1B_GLOBAL_CTRL1                    0x1B
#define RA_1C_GLOBAL_CTRL2                    0x1C
#define RA_1D_GLOBAL_CTRL3                    0x1D
#define RA_1E_GLOBAL_CTRL4                    0x1E
#define RA_1F_GLOBAL_CTRL5                    0x1F
#define RA_20_ALS1_CTRL0                      0x20
#define RA_21_ALS1_CTRL1                      0x21
#define RA_25_ALS1_THR_INT_HIGH0              0x25
#define RA_27_ALS1_THR_INT_LOW0               0x27
#define RA_29_ALS1_GAIN_CFG                   0x29
#define RA_2A_ALS1_AFE_OFFSET0                0x2A
#define RA_2B_ALS1_AFE_OFFSET1                0x2B
#define RA_2C_ALS1_AFE_OFFSET2                0x2C
#define RA_30_ALS2_CTRL0                      0x30
#define RA_31_ALS2_CTRL1                      0x31
#define RA_33_ALS2_CTRL3                      0x33
#define RA_3E_ALS2_CTRL5                      0x3E
#define RA_39_ALS2_GAIN_CFG                   0x39
#define RA_3A_ALS1_OFFSET0                    0x3A
#define RA_3B_ALS1_OFFSET1                    0x3B
#define RA_3C_ALS1_OFFSET2                    0x3C
#define RA_3D_ALS1_OFFSET3                    0x3D
#define RA_4A_ALS3_GAIN_CFG                   0x4A
#define RA_4C_ALSEN_CTRL1                     0x4C
#define RA_4D_ALS3_OFFSET0	                  0x4d
#define RA_4E_ALS3_OFFSET1                    0x4e
#define RA_4F_ALS3_OFFSET2                    0x4f
#define RA_45_ALS_CFG0                    	  0x45
#define RA_46_ALS_CFG1	                      0x46
#define RA_47_ALS_CFG2	                      0x47
#define RA_48_ALS_CFG3	                      0x48
#define RA_50_PS_CTRL0                        0x50
#define RA_51_PS_CTRL1                        0x51
#define RA_52_PS_CTRL2                        0x52
#define RA_53_PS_CTRL3                        0x53
#define RA_54_PS_CTRL4                        0x54
#define RA_55_PS_CTRL5                        0x55
#define RA_56_PS_CTRL6                        0x56
#define RA_58_PS_CTRL8                        0x58
#define RA_59_PS_CTRL9                        0x59
#define RA_5A_PS_GAIN_CFG0                    0x5A
#define RA_5B_PS_GAIN_CFG1                    0x5B
#define RA_5C_PS_CLK_DIV_NUM                  0x5C
#define RA_5F_PS_THR_INT_NUM                  0x5F
#define RA_60_PROX_THR_HIGH1_0                0x60
#define RA_61_PROX_THR_HIGH1_1                0x61
#define RA_62_PROX_THR_LOW1_0                 0x62
#define RA_63_PROX_THR_LOW1_1                 0x63
#define RA_64_PS_OFFSET_0                     0x64
#define RA_65_PS_OFFSET_1                     0x65
#define RA_66_PS_OFFSET_2                     0x66
#define RA_67_PS_DIG_GAIN                     0x67
#define RA_68_PS_OFFSET_CALI                  0x68
#define RA_6E_PS_LP_SIG_THRES0	              0x6E
#define RA_6F_PS_LP_SIG_THRES1	              0x6F
#define RA_70_PS_ALPHA_CFG0                   0x70
#define RA_71_PS_ALPHA_CFG1                   0x71
#define RA_72_PS_BL_FT_THRES0                 0x72
#define RA_74_PS_PROX_THRES_CFG	              0x74
#define RA_75_PS_PROX_INT_NUM_CFG             0x75
#define RA_76_AFE_AMP3_CFG	                  0x76
#define RA_77_PS_CTRL10	                      0x77
#define RA_78_PS_CTRL11	                      0x78
#define RA_79_PS_CTRL12                       0x79
#define RA_7A_PS_CTRL13                       0x7A
#define RA_7B_PS_CTRL14                       0x7B
#define RA_80_ODR_CFG0                        0x80
#define RA_81_FSM_FLAG0                       0x81
#define RA_82_FSM_FLAG1                       0x82
#define RA_83_DATA_LOCK_CFG                   0x83
#define RA_87_INT_WIDTH_CFG0                  0x87
#define RA_88_INT_WIDTH_CFG1                  0x88
#define RA_8A_ALS1_DIV_CLK_CTRL0              0x8A
#define RA_8B_ALS1_DIV_CLK_CTRL1              0x8B
#define RA_8C_ALS2_DIV_CLK_CTRL0              0x8C
#define RA_8D_ALS2_DIV_CLK_CTRL1              0x8D
#define RA_90_ANALOG_MEM0_WRDATA_7_0          0x90
#define RA_91_ANALOG_MEM0_WRDATA_15_8         0x91
#define RA_92_ANALOG_MEM0_WRDATA_23_16        0x92
#define RA_93_ANALOG_MEM0_WRDATA_31_24        0x93
#define RA_94_ANALOG_MEM1_WRDATA_7_0          0x94
#define RA_95_ANALOG_MEM1_WRDATA_15_8         0x95
#define RA_96_ANALOG_MEM1_WRDATA_23_16        0x96
#define RA_97_ANALOG_MEM1_WRDATA_31_24        0x97
#define RA_98_ANALOG_PWE_PULSE_CYCLE7_0       0x98
#define RA_99_ANALOG_PWE_PULSE_CYCLE12_8      0x99
#define RA_9A_MEM_GLOBAL_CTRL                 0x9A
#define RA_9C_DEBUG_MEM_ADC_FSM               0x9C

// analog registers:
#define RW_E0_REG_ADDR_E0H                    0xE0
#define RW_E1_REG_ADDR_E1H                    0xE1
#define RW_E2_REG_ADDR_E2H                    0xE2
#define RW_E3_REG_ADDR_E3H                    0xE3
#define RW_E4_REG_ADDR_E4H                    0xE4
#define RW_E5_REG_ADDR_E5H                    0xE5
#define RW_E6_REG_ADDR_E6H                    0xE6
#define RW_E7_REG_ADDR_E7H                    0xE7
#define RW_E8_REG_ADDR_E8H                    0xE8
#define RW_E9_REG_ADDR_E9H                    0xE9
#define RW_EA_REG_ADDR_EAH                    0xEA
#define RW_EB_REG_ADDR_EBH                    0xEB
#define RW_EC_REG_ADDR_ECH                    0xEC
#define RW_ED_REG_ADDR_EDH                    0xED
#define RW_EE_REG_ADDR_EEH                    0xEE
#define RW_EF_REG_ADDR_EFH                    0xEF
#define RW_F0_REG_ADDR_F0H                    0xF0

struct hx32062se_data {
    int irq_;
    int irq_gpio;

    uint8_t ps_enable;
    uint16_t ps_thd_h;
    uint16_t ps_thd_l;
    int8_t ps_status;//????-1????????
    int8_t ps_status_pre;
    int32_t ps_raw_data;//???
    int32_t ps_bl_data; //???
    uint32_t ps_diff_data;
    uint32_t ps_offset_data;
    uint8_t ps_polling_enable;
    uint32_t ps_alg_cnt;

    uint8_t als_enable;
    uint32_t als1_lp_data;
    int32_t als2_lp_data;
    uint16_t als3_lp_data;
//    uint32_t als4_lp_data;
    uint32_t als_lux;
    uint32_t als_lux_pre;
    uint8_t als_polling_enable;
    uint32_t als_polling_period_ms;
    uint32_t als_alg_cnt;
    uint32_t als_agc_cnt;
    uint8_t  als_en_flag_oneshot;

    uint32_t als_factory_data;
    uint32_t ps_factory_data;
    uint8_t als_debounce_on;
    uint32_t als_debounce_time;
    uint64_t als_debounce_end;
    uint32_t ps_samp_first;
};

struct hx32062se_addr_val_pair {
    uint8_t addr;
    uint8_t val;
};

int hx32062se_id_check(void);
void hx32062se_reg_init(void);
int hx32062se_als_disable(void);
int hx32062se_ps_enable(void);
int hx32062se_set_ps_thd_l(uint16_t thd_l);
int hx32062se_set_ps_thd_h(uint16_t thd_h);
void hx32062se_data_lock(uint8_t lock_flag);
int8_t hx32062se_ps_sample(void);

/* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _HW_HX32062SE_H */
