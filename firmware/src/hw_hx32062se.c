/******************************************************************************
  @File Name
    hw_hx32062se.c

  @Summary
    hx32062s chip driver (wear sensor)

  @Description
    
*******************************************************************************/
#include "hw_hx32062se.h"

// .............................................................................
// Initial settings for Wear Sensor
// THis is an array of Register and Value pairs, which will be written to the 
// device when the init function, hx32062se_reg_init() is called.
// .............................................................................
const struct hx32062se_addr_val_pair hx32062se_reg_init_list[] = {
	// write to file
    //PS CONFIG
    {RA_64_PS_OFFSET_0,                  0x00}, //ps offset
    {RA_65_PS_OFFSET_1,                  0x00}, //ps offset
    {RA_66_PS_OFFSET_2,                  0x00}, //ps offset
    {RA_72_PS_BL_FT_THRES0,              0xFF}, //BL firsttime
    {RA_53_PS_CTRL3,                     0xF4}, //LED PULSE WIDTH f4=63us, 80=34us, 38=16us
    {RA_52_PS_CTRL2,                     0x52}, //PS ANALOG AVG = 0
    {RA_55_PS_CTRL5,                     0xA5}, //PS DIGITAL AVG = 1
    {RA_50_PS_CTRL0,                     0x74}, //PS NRESET
    {RA_5C_PS_CLK_DIV_NUM,               0x22}, //ADC Freq 5MHz, ver0.9.3
    {RW_E0_REG_ADDR_E0H,                 0x00}, // 00
    {RW_E1_REG_ADDR_E1H,                 0xD7}, //LED CURRENT 125mA
    {RW_E2_REG_ADDR_E2H,                 0xC8},
    {RW_E3_REG_ADDR_E3H,                 0x00}, //LED CURRENT 0mA    0xF8 = 15ma
    {RW_E4_REG_ADDR_E4H,                 0x80}, //
    {RW_E7_REG_ADDR_E7H,                 0x00}, //  0x53
    {RW_E9_REG_ADDR_E9H,                 0x10}, //IZTAT TC Control, ver0.9.3
    {RW_EA_REG_ADDR_EAH,                 0x47}, //LED_CHOP(bit 6) ON, CLK DIV=4, 20M=47,5M=67;
    {RW_EB_REG_ADDR_EBH,                 0x04}, //LED current 20% up, VCM OFF
    {RW_EC_REG_ADDR_ECH,                 0x2C},
    {RW_ED_REG_ADDR_EDH,                 0x81}, // <7:3> cint sel 0 - 26pf 1 - 13pf 81
    {RW_EF_REG_ADDR_EFH,                 0xBF}, //enable all photodiodes  BF
    {RW_F0_REG_ADDR_F0H,                 0x0F}, //
    {RA_5A_PS_GAIN_CFG0,                 0x00}, // ana avg
    {RA_5B_PS_GAIN_CFG1,                 0x01}, // dig avg
    //ALS CONFIG
    {RA_20_ALS1_CTRL0,                   0x30}, //als1 avg 8 0:1 1:2 2:4 3:8 4:16 5:32 6:64. .
    {RA_21_ALS1_CTRL1,                   0x10}, //als1 osr 256  0:32 1:64  2:128  3:256  4:512, default modify ver0.9.1
    {RA_30_ALS2_CTRL0,                   0x30}, //als2 avg 8 0:1 1:2 2:4 3:8 4:16 5:32 6:64
    {RA_31_ALS2_CTRL1,                   0x10}, //als2 osr 256  0:32 1:64  2:128  3:256  4:512  default modify ver0.9.1
    // write to file

    //GENERAL CONFIG
    {RA_1D_GLOBAL_CTRL3,                 0x10}, //OSC1P6M ENABLE <4>
    {RA_80_ODR_CFG0,                     0x14}, //12 odr = 100ms 0x14 = 200ms
    {RA_83_DATA_LOCK_CFG,                0x00}, //man lock
    {RA_1F_GLOBAL_CTRL5,                 0x80}, //rev
    {RA_19_AFE_AMP_CHOP_CTRL,            0x00}, //

    //PS ????
    {RA_1D_GLOBAL_CTRL3,                 0xD0}, //
    {RA_1E_GLOBAL_CTRL4,                 0xC0}, //

    //ALS CONFIG
    {RA_8A_ALS1_DIV_CLK_CTRL0,	         0xD0}, //als1 sample clk: 2:800k 4:400K 8:200K 16:100K 32:50K 64:25K 128:12.5K 256:6.25K  //04  02
    {RA_8B_ALS1_DIV_CLK_CTRL1,	         0x00},
    {RA_8C_ALS2_DIV_CLK_CTRL0,	         0xD0}, //als2 sample clk: 2:800k 4:400K 8:200K 16:100K 32:50K 64:25K 128:12.5K 256:6.25K
    {RA_8D_ALS2_DIV_CLK_CTRL1,	         0x00},
    {RA_2A_ALS1_AFE_OFFSET0,             0x01}, //als1 offset
    {RA_2B_ALS1_AFE_OFFSET1,             0xFC}, //als1 offset
    {RA_2C_ALS1_AFE_OFFSET2,             0x01}, //als1 offset

    {RA_3A_ALS1_OFFSET0,                 0x00}, //als1 offset
    {RA_3B_ALS1_OFFSET1,                 0x00}, //als1 offset
    {RA_3C_ALS1_OFFSET2,                 0x00}, //als1 offset
    {RA_3D_ALS1_OFFSET3,                 0x00}, //als1 offset

    {RA_4D_ALS3_OFFSET0,                 0x00}, //als3 offset
    {RA_4E_ALS3_OFFSET1,                 0x00}, //als3 offset
    {RA_4F_ALS3_OFFSET2,                 0x00}, //als3 offset

    {RA_29_ALS1_GAIN_CFG,                0x02}, //als1 gain 01:1X 02:2X 03:4X 04:8X 05:16X 06:32X
    {RA_39_ALS2_GAIN_CFG,                0x02}, //als2 gain 01:1X 02:2X 03:4X 04:8X 05:16X 06:32X
    {RA_4A_ALS3_GAIN_CFG,                0x02}, //als3 gain 01:1X 02:2X 03:4X 04:8X 05:16X 06:32X
    {RA_46_ALS_CFG1,                     0x00}, //B?C 0
    {RA_47_ALS_CFG2,                     0x00}, //B?C 0
    {RA_48_ALS_CFG3,                     0x18}, //als_dig_gain bit4: 0 efuse;1 registry
    {RA_67_PS_DIG_GAIN,                  0x18}, //ps_dig_gain  bit4: 0 efuse;1 registry
//    // int set
    {RA_14_DATA_RDY_CTRL1,               0x02}, //PS threshold int enabled  02
    {RA_60_PROX_THR_HIGH1_0,             0xD0}, //PS high thr <7:0>
    {RA_61_PROX_THR_HIGH1_1,             0x07}, //PS high thr <15:8>
    {RA_62_PROX_THR_LOW1_0,              0xF4}, //PS low thr <7:0>
    {RA_63_PROX_THR_LOW1_1,              0x01}, //PS low thr <15:8>
//    {RA_45_ALS_CFG0,                     0x00}, //bit<4> 0 - bl dis  1 - bl en
//    {RA_71_PS_ALPHA_CFG1,                0x33}, //bit<7:4> ps_bl_dn_alpha bit<3:0> ps_bl_up_alpha 0-0 1-1 2-1/2  3-1/4  4-1/8  5-1/16  6-1/32  B-1/1024
//    {RA_72_PS_BL_FT_THRES0,              0x01}, //high thres RA_72_PS_BL_FT_THRES0<7:0>,8d1
//    {RA_7B_PS_CTRL14,                    0x10}, //low thres 1d1,RA_7B_PS_CTRL14<15:8>,8d0
    //PS LP
    {RA_58_PS_CTRL8,                     0x80}, //<7> lp_en 1-enable 0-disable A0
    {RA_6E_PS_LP_SIG_THRES0,             0x32}, //PS_LP_signal_thres<7:0>*4
    {RA_70_PS_ALPHA_CFG0,                0x04}, //<2:0> ps_lp_alpha  0-1  1-1/2  2-1/4  3-1/8  4-1/16  5-1/32  6-1/64  7-1/128
};

// .............................................................................
// This struct holds all the settings for the wear sensor.
// .............................................................................
struct hx32062se_data hx32062se_pdata = {
    .irq_                     = 0,
    .irq_gpio                 = 0,

    .ps_enable                = 0,
    .ps_thd_h                 = WEAR_THRESHOLD_HI,
    .ps_thd_l                 = WEAR_THRESHOLD_LO,    
    .ps_status                = HX32062SE_PS_STATUS_FAR,
    .ps_status_pre            = HX32062SE_PS_STATUS_FAR,
    .ps_raw_data              = 0,
    .ps_bl_data               = 0,
    .ps_diff_data             = 0,
    .ps_offset_data           = 0,
    .ps_polling_enable        = 0,
    .ps_alg_cnt               = 0,

    .als_enable               = 0,
    .als1_lp_data             = 0,
    .als2_lp_data             = 0,
    .als3_lp_data             = 0,

    .als_lux                  = 0,
    .als_lux_pre              = 0,
    .als_polling_enable       = 1,
    .als_polling_period_ms    = 0,
    .als_alg_cnt              = 0,
    .als_agc_cnt              = 0,

    .als_factory_data         = 0,
    .ps_factory_data          = 0,
    .als_debounce_on          = 0,
    .als_debounce_time        = 0,
    .als_debounce_end         = 0,
};

// .............................................................................
// Low-level function to read from the sensor.
// Return 0 for success, return 1 for errors.
// .............................................................................
int hx32062se_read(uint8_t addr, uint8_t *rxbuf, int count)
{
    int ret;
    ret = wear_write(&addr, 1);
    if (ret) return ret;
    ret = wear_read(rxbuf, count);  
    return ret;
}

// .............................................................................
// Low-level function to write to the sensor.
// Return 0 for success, return 1 for errors.
// .............................................................................
int hx32062se_write(uint8_t addr, uint8_t *txbuf, int count)
{
    int ret, i;
    uint8_t buf[count + 1];
    buf[0] = addr;
    for (i = 0; i < count; i++) {
        buf[i + 1] = txbuf[i];
    }
    ret = wear_write(buf, count + 1);
    return ret;
}

// .............................................................................
// Read and verify device id (between 0x32 and 0x3A)
// .............................................................................
int hx32062se_id_check(void)
{
    int ret, i;
    uint8_t device_id = 0;

    for (i = 0; i < HX32062SE_ID_CHECK_COUNT; i++) {
        ret = hx32062se_read(RA_00_DEVICE_ID, &device_id, 1);
        if (ret) {
            DBGprintf("wear device_id read error\n");
            continue;
        }
        if (device_id >= REV_ID_START && device_id <= REV_ID_END) {
            break;
        }
    }

    DBGprintf("wear device_id=0x%02X", device_id);
    if (REV_CORE_ID == device_id) {
        DBGprintf("(REV_CORE)\n");
        return 0;
    }
    if (REV_A_ID == device_id) {
		DBGprintf("(REV_A)\n");
        return 0;
    }
	if (REV_B_ID == device_id) {
		DBGprintf("(REV_B)\n");
        return 0;
    }
	if (REV_C_ID == device_id) {
		DBGprintf("(REV_C)\n");
        return 0;
    }
	if (REV_D_ID == device_id) {
		DBGprintf("(REV_D)\n");
        return 0;
    }
    if (REV1S2_CORE_ID == device_id) {
        DBGprintf("(REV1S2_CORE)\n");
        return 0;
    }
    if (REV1S2_A_ID == device_id) {
		DBGprintf("(REV1S2_A)\n");
        return 0;
    }
	if (REV1S2_B_ID == device_id) {
		DBGprintf("(REV1S2_B)\n");
        return 0;
    }
	if (REV1S2_C_ID == device_id) {
		DBGprintf("(REV1S2_C)\n");
        return 0;
	}
    DBGprintf("(UNKNOW_CHIP_ID!)\n");
    return 1;
}

// .............................................................................
// Initialise wear sensor with pre-defined default/init values.
// .............................................................................
void hx32062se_reg_init(void)
{
    int ret, i;

    for (i = 0; i < HX_ARRAY_SIZE(hx32062se_reg_init_list); i++) {
        ret = hx32062se_write(hx32062se_reg_init_list[i].addr, &hx32062se_reg_init_list[i].val, 1);
        if (ret) {
            DBGprintf("wear write error\n");
        }
    }

    DBGprintf("wear init done! list_size=%d\n", HX_ARRAY_SIZE(hx32062se_reg_init_list));
    return;
}

// .............................................................................
// Disable the sensor's ALS (ambient light sensor)
// .............................................................................
int hx32062se_als_disable(void)
{
    int ret;
    uint8_t buf[1] = {0};

    ret = hx32062se_read(RA_4C_ALSEN_CTRL1, buf, 1);
    if (ret) {
        DBGprintf("wear read failed\n");
    }

    buf[0] &= ~0x07;
    ret = hx32062se_write(RA_4C_ALSEN_CTRL1, buf, 1);
    if (ret) {
        DBGprintf("wear write failed\n");
    }
    
    hx32062se_pdata.als_enable = 0;
    
    DBGprintf("wear ALS disabled\n");

    return 0;
}

// .............................................................................
// Enable the sensor's PS (proximity sensor)
// .............................................................................
int hx32062se_ps_enable(void)
{
    int ret;
    uint8_t buf[1] = {0};

    ret = hx32062se_read(RA_4C_ALSEN_CTRL1, buf, 1);
    if (ret) {
        DBGprintf("wear read failed\n");
    }

    buf[0] |= 0x08;
    ret = hx32062se_write(RA_4C_ALSEN_CTRL1, buf, 1);
    if (ret) {
        DBGprintf("wear write failed\n");
    }
    
    hx32062se_set_ps_thd_h(hx32062se_pdata.ps_thd_h);
    hx32062se_set_ps_thd_l(hx32062se_pdata.ps_thd_l);
    DBGprintf("wear PS threshold set: HT=%d, LT=%d\n", hx32062se_pdata.ps_thd_h, hx32062se_pdata.ps_thd_l);

    hx32062se_pdata.ps_status = HX32062SE_PROX_STATE_INIT;
    hx32062se_pdata.ps_status_pre = HX32062SE_PROX_STATE_INIT;
    hx32062se_pdata.ps_enable = 1;
    hx32062se_pdata.ps_alg_cnt = 0;
    
    DBGprintf("wear PS enabled\n");
    
    return 0;
}

// .............................................................................
 // Set the Proximity Sensor's High Threshold
// .............................................................................
int hx32062se_set_ps_thd_h(uint16_t thd_h)
{
    int ret;
    uint8_t val_lsb = thd_h;        // low  8bits
    uint8_t val_msb = thd_h >> 8;   // high 8bits

    ret = hx32062se_write(RA_60_PROX_THR_HIGH1_0, &val_lsb, 1);
    if (ret) {
        DBGprintf("wear write failed\n");
    }
    ret = hx32062se_write(RA_61_PROX_THR_HIGH1_1, &val_msb, 1);
    if (ret) {
        DBGprintf("wear write failed\n");
    }
    return ret;
}

// .............................................................................
 // Set the Proximity Sensor's Low Threshold
// .............................................................................
int hx32062se_set_ps_thd_l(uint16_t thd_l)
{
    int ret;
    uint8_t val_lsb = thd_l;        // low  8bits
    uint8_t val_msb = thd_l >> 8;   // high 8bits

    ret = hx32062se_write(RA_62_PROX_THR_LOW1_0, &val_lsb, 1);
    if (ret) {
        DBGprintf("wear write failed\n");
    }
    ret = hx32062se_write(RA_63_PROX_THR_LOW1_1, &val_msb, 1);
    if (ret) {
        DBGprintf("wear write failed\n");
    }
    return ret;
}

// .............................................................................
void hx32062se_data_lock(uint8_t lock_flag)
{
    int ret;
    uint8_t buf[1] = {0};

    ret = hx32062se_read(RA_83_DATA_LOCK_CFG, buf, 1);
    if (ret) {
        DBGprintf("wear read failed\n");
    }
    if (HX32062SE_DATA_UNLOCK == lock_flag) {
        buf[0] &= ~0x02;
    } else { // Lock!
        buf[0] |= 0x02;
    }
    ret = hx32062se_write(RA_83_DATA_LOCK_CFG, buf, 1);
    if (ret) {
        DBGprintf("wear write failed\n");
    }
}

// .............................................................................
// Read the wear sensor's latest data
// .............................................................................
int8_t hx32062se_ps_sample(void)
{
    int ret = -1;
    uint8_t buf[6] = {0};
    int32_t data = 0;

    hx32062se_pdata.ps_status_pre = hx32062se_pdata.ps_status;

    hx32062se_data_lock(HX32062SE_DATA_LOCK);
    ret = hx32062se_read(RA_0D_PS_RAW_DATA0, buf, 6);
    if (0 == ret) {
        data = ((buf[2] << 16) | (buf[1] << 8) | (buf[0]));
        hx32062se_pdata.ps_raw_data = (data > 0xFFFF) ? (data - (0x1FFFF + 1)) : data;
        data = ((buf[5] << 16) | (buf[4] << 8) | (buf[3]));
        hx32062se_pdata.ps_bl_data = (data > 0xFFFF) ? (data - (0x1FFFF + 1)) : data;
    }
    ret = hx32062se_read(RA_64_PS_OFFSET_0, buf, 3);
    if (0 == ret) {
		data = ((buf[2] << 16) | (buf[1] << 8) | (buf[0]));
        hx32062se_pdata.ps_offset_data = (data > 0xFFFF) ? (data - (0x1FFFF + 1)) : data;
    }
    ret = hx32062se_read(RA_69_PS_DIFF_DATA0, buf, 5);
    if (0 == ret) {
        hx32062se_pdata.ps_diff_data = ((buf[1] << 8) | (buf[0]));
    }

    ret = hx32062se_read(RA_18_DATA_RDY_FLAG3, buf, 1);
    if (ret) {
        DBGprintf("wear read failed\n");
    }
		
    DBGprintf("wear ps data=%d ", hx32062se_pdata.ps_diff_data);
		
    if (2 == (buf[0] & 0x03)) {
        hx32062se_pdata.ps_status = HX32062SE_PS_STATUS_FAR;
    } else if (1 == (buf[0] & 0x03)) {
        hx32062se_pdata.ps_status = HX32062SE_PS_STATUS_NEAR;
    } else {
        hx32062se_pdata.ps_status = hx32062se_pdata.ps_status_pre;
    }
		
    hx32062se_data_lock(HX32062SE_DATA_UNLOCK);
    ret = hx32062se_read(RA_60_PROX_THR_HIGH1_0, buf, 2);
    DBGprintf("high_thd=%d ", (int32_t)(buf[0] | buf[1]<<8));
    ret = hx32062se_read(RA_62_PROX_THR_LOW1_0, buf, 2);
    DBGprintf("low_thd=%d ", (int32_t)(buf[0] | buf[1]<<8));
    DBGprintf("raw=%d bl=%d diff=%d offset=%d status_pre=%d status=%d\n",
              hx32062se_pdata.ps_raw_data, hx32062se_pdata.ps_bl_data,
              hx32062se_pdata.ps_diff_data, hx32062se_pdata.ps_offset_data,
              hx32062se_pdata.ps_status_pre, hx32062se_pdata.ps_status);
    
    return(hx32062se_pdata.ps_status);
}
// .............................................................................
