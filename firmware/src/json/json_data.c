/******************************************************************************
  @File Name
    json_data.c

  @Summary
    JSON Data

  @Description

    
*******************************************************************************/
#include "json.h"
#include "json_data.h"

// .............................................................................

data_com_t data_com;                        // Common dataset
data_pro_t data_pro[MAX_PROFILE];           // Profile data

char ser_num_str[33];

uint8_t dbgTst_cntr = 0;        // !dbg!

// *****************************************************************************
// Read the SAM551 serial number
// *****************************************************************************
void mcu_ser_num_read(void) 
{
     uint32_t ser_num[4];
     
   // The unique serial number is stored in the DSU
    ser_num[0] = *(uint32_t*)0x008061FC;
    ser_num[1] = *(uint32_t*)0x00806010;
    ser_num[2] = *(uint32_t*)0x00806014;
    ser_num[3] = *(uint32_t*)0x00806018;
    
    // Convert to strings:
    snprintf(ser_num_str, sizeof(ser_num_str), "%08X%08X%08X%08X", 
             (unsigned int)ser_num[0], 
             (unsigned int)ser_num[1], 
             (unsigned int)ser_num[2], 
             (unsigned int)ser_num[3]);
}
// *****************************************************************************
// Get a copy of the the SAM551 serial number
// *****************************************************************************
void mcu_ser_num_get(char *ser_str) 
{
    strcpy(ser_str, ser_num_str);
}

// .............................................................................
// Initialise all data belonging to Common Dataset
// .............................................................................
void json_data_init_eq_presets()
{
    for (int i = 0; i < MAX_EQ_PRESETS; i++)
    {
        switch (i) {
            case 0:
                for (int j = 0; j < EQ_ARRAY_SIZE; j++) {
                    data_com.eq_pre_data[i].vals[j] = j+1;
                }
                strcpy(data_com.eq_pre_data[i].name, "eqPreset One");
                data_com.eq_pre_data[i].defined = true;
                break;
            case 1:
                for (int j = 0; j < EQ_ARRAY_SIZE; j++) {
                    data_com.eq_pre_data[i].vals[j] = j+2;
                }
                strcpy(data_com.eq_pre_data[i].name, "eqPreset Two");
                data_com.eq_pre_data[i].defined = true;
                break;
            case 2:
                for (int j = 0; j < EQ_ARRAY_SIZE; j++) {
                    data_com.eq_pre_data[i].vals[j] = j+3;
                }
                strcpy(data_com.eq_pre_data[i].name, "eqPreset Three");
                data_com.eq_pre_data[i].defined = true;
                break;
            case 3:
                for (int j = 0; j < EQ_ARRAY_SIZE; j++) {
                    data_com.eq_pre_data[i].vals[j] = j+4;
                }
                strcpy(data_com.eq_pre_data[i].name, "eqPreset Four");
                data_com.eq_pre_data[i].defined = true;
                break;
            case 4:
                for (int j = 0; j < EQ_ARRAY_SIZE; j++) {
                    data_com.eq_pre_data[i].vals[j] = j+5;
                }
                strcpy(data_com.eq_pre_data[i].name, "eqPreset Five");
                data_com.eq_pre_data[i].defined = true;
                break;
            default:
                for (int j = 0; j < EQ_ARRAY_SIZE; j++) {
                    data_com.eq_pre_data[i].vals[j] = 0;
                }
                strcpy(data_com.eq_pre_data[i].name, "noPresetName");
                data_com.eq_pre_data[i].defined = false;
                break;
        }
    }
    
}
// .............................................................................
// Initialise all data belonging to Common Dataset
// .............................................................................
void json_data_init_common()
{
    strcpy(data_com.mode_id, "6");
    strcpy(data_com.fw_ver_left, "01.01.01");
    strcpy(data_com.fw_ver_right, "02.02.02");
    //strcpy(p_dat->ser_num, "noSerial");
    strcpy(data_com.ser_num, "ABCdefGH1234");
    data_com.com_state = DATA_STATE_FACTORY;

    data_com.blue_enabled = false;
    data_com.audio_vol = 50;
    data_com.batt_lvl = 0;
    data_com.batt_mins = 0;
    data_com.audio_cues_enabled = false;
    data_com.auto_pwr_off_tim = APWOFF_TIME_DEFAULT;
    data_com.auto_pwr_off_en = true;
    data_com.charge_stat = false;
    data_com.signal_lvl = 0;
    data_com.display_menu_timeout = 0;
    data_com.mic_enabled = true;
    data_com.mic_connected = false;
    json_data_init_eq_presets();
    /*
    for (int i = 0; i < MAX_EQ_PRESETS; i++)
    {
        for (int j = 0; j < EQ_ARRAY_SIZE; j++) {
            data_com.eq_pre_data[i].vals[j] = 0;
        }
        strcpy(data_com.eq_pre_data[i].name, "noPresetName");
        data_com.eq_pre_data[i].defined = false;
    }
    */
    data_com.dfu_file_size = 0;
    data_com.dfu_file_crc = 0;
    data_com.dfu_segment_size = 2048;
    data_com.prfl_in_use = 1;             // Value is 1..n
    data_com.prfl_count = MAX_PROFILE;
}

// .............................................................................
// Initialise all data belonging to selected Profile 1..n
// Input
//  indx    1..n
// .............................................................................
void json_data_init_profile(uint8_t indx)
{
    if ((indx == 0) || (indx > MAX_PROFILE)) {
        return;
    }

    // Profile Index 1..n, stored in arrays [0..n-1]
    indx--;
    
    // First profile, initially Enabled, all other profiles initially disabled
    data_pro[indx].pro_en = (indx == 0) ? true : false;
    strcpy(data_pro[indx].prfl_name, "noProfileName");
    
    data_pro[indx].pro_state = DATA_STATE_FACTORY;
    data_pro[indx].display_on = false;
    data_pro[indx].blue_on_pwr_up = false;
    data_pro[indx].display_bright = 0;
    data_pro[indx].chat_mix = 50;
    data_pro[indx].media_mix = 0;
    data_pro[indx].mic_vol = 0;
    data_pro[indx].mic_side_tone = 0;
    data_pro[indx].mic_noise_gate = 0;
    data_pro[indx].mic_noise_reduction = false;
    data_pro[indx].mic_vol_norm = false;
    data_pro[indx].mic_led_bright = 0;
    data_pro[indx].anc_mode = 0;
    data_pro[indx].eq_pre_enabled = false;
    data_pro[indx].audio_surr = false;
    data_pro[indx].light_effect = 0;
    data_pro[indx].light_enabled = false;
    data_pro[indx].light_color_mode = 0;
    data_pro[indx].light_solid_color = 0;
    data_pro[indx].light_grad_color = 0;
    data_pro[indx].light_rate = 0;
    data_pro[indx].light_bright = 0;
    data_pro[indx].light_auto_off = false;
    data_pro[indx].eq_pre_in_use = 1;
    data_pro[indx].artwork_mode = 0;
    data_pro[indx].artwork_seq_dur = 0;
}
// .............................................................................
// Initialise all data
// .............................................................................

void json_data_init()
{
    json_data_init_common();
    for (int i = 1; i < MAX_PROFILE+1; i++)
    {
        json_data_init_profile(i);
    }
}
// .............................................................................

   
// *****************************************************************************
// Read / write functions to alow other modules access the data
// *****************************************************************************

// AUDIO AUTO POWER OFF

// .............................................................................
uint16_t json_data_auto_pwr_off_tim_read()
{
    return(data_com.auto_pwr_off_tim);
}
// .............................................................................
bool json_data_auto_pwr_off_en_read()
{
    return(data_com.auto_pwr_off_en);
}
// .............................................................................

// DFU

// .............................................................................
uint32_t json_data_dfu_file_size_read()
{
    return(data_com.dfu_file_size);
}
// .............................................................................
uint16_t json_data_dfu_file_crc_read()
{
    return(data_com.dfu_file_crc);
}
// .............................................................................
uint16_t json_data_dfu_segment_size_read()
{
    return(data_com.dfu_segment_size);
}
// .............................................................................
