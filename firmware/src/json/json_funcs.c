/******************************************************************************
  @File Name
    json_funcs.c

  @Summary
    JSON Data Functions

  @Description

    NOTE:   The functions in this file were created as they were required etc. 
            The code can be refactored to have one general Write command, which 
            takes the JSON key as a parameter, and similar refactoring for Read, 
            Info, Command function etc.
    
*******************************************************************************/
#include "json.h"
#include "json_data.h"
#include <ctype.h>          // for isdigit() ?

// .............................................................................

// JSON Keys - Common Profile
char json_key_Mode_Id[] = "mID"; 
char json_key_Firmware_Ver[] = "firmV"; 
char json_key_Serial_Num[] = "SN"; 
char json_key_Com_State[] = "cSta"; 
char json_key_Blue_Enabled[] = "blEn"; 
char json_key_Audio_Vol[] = "aVol"; 
char json_key_Batt_Level[] = "battL"; 
char json_key_Batt_Min[] = "battMin"; 
char json_key_Cues_Enabled[] = "aCEn"; 
char json_key_Charge_Status[] = "chSt"; 
char json_key_Signal_Level[] = "sLvl"; 
char json_key_Audio_Pwr_Off_Time[] = "aPwTim"; 
char json_key_Audio_Pwr_Off_Enabled[] = "aPwEn"; 
char json_key_Disp_Menu_Time[] = "dMT"; 
char json_key_Mic_Enabled[] = "mEn"; 
char json_key_Mic_Connected[] = "micCon"; 
char json_key_Eq_Presets_X[] = "eqPx";
char json_key_Eq_Preset_Id_X[] = "eqId";
char json_key_Eq_Preset_Vals[] = "eqPv";
char json_key_Eq_Preset_Names[] = "eqPn";
char json_key_Eq_Preset_Defined[] = "eqPd";
char json_key_Dfu_File_Size[] = "dfuFS";
char json_key_Dfu_File_Crc[] = "dfuFC";
char json_key_Dfu_Segment_size[] = "dfuSS";
char json_key_Prof_In_Use[] = "pID"; 
char json_key_Prof_Count[] = "pCnt"; 
char json_key_Prof_Names[] = "pNms"; 
// JSON Keys - Other Profile
char json_key_Prof_Enabled[] = "pEn"; 
char json_key_Prof_Name[] = "pName"; 
char json_key_Prof_State[] = "pSta"; 
char json_key_Disp_On[] = "dOn"; 
char json_key_Blue_Power_Up[] = "blOnPW"; 
char json_key_Disp_Bright[] = "dBri"; 
char json_key_Chat_Mix[] = "cMix"; 
char json_key_Media_Mix[] = "mMix"; 
char json_key_Mic_Volume[] = "mVol"; 
char json_key_Mic_Side_Tone[] = "mSt"; 
char json_key_Mic_Noise_Gate[] = "mNG"; 
char json_key_Mic_Noise_Reduction[] = "mNR"; 
char json_key_Mic_Vol_Norm[] = "mVN"; 
char json_key_Mic_Led_Bright[] = "mLB"; 
char json_key_ANC_Mode[] = "anc"; 
char json_key_Eq_Preset_En[] = "eqPe"; 
char json_key_Audio_Surround[] = "aSurr"; 
char json_key_Light_Effect[] = "lEff"; 
char json_key_Light_En[] = "lEn"; 
char json_key_Light_Color_Mode[] = "lCM"; 
char json_key_Light_Solid_Color[] = "lSC"; 
char json_key_Light_Gradient_Color[] = "lGC"; 
char json_key_Light_Rate[] = "lRate"; 
char json_key_Light_Bright[] = "lBri"; 
char json_key_Light_Auto_Off[] = "lAO"; 
char json_key_Eq_Preset_In_Use[] = "eqPID"; 

// JSON Command Keys
char json_key_cmd_Profile_Read[] = "ProRd"; 
char json_key_cmd_Profile_Restore[] = "ProFc"; 
char json_key_cmd_Profile_Create[] = "ProCr"; 
char json_key_cmd_Profile_Delete[] = "ProDl"; 
char json_key_cmd_Profile_Duplicate[] = "ProDp"; 
char json_key_cmd_Dfu_Commit[] = "dfuCm";

char json_val_off[] = "off"; 
char json_val_low[] = "low"; 
char json_val_med[] = "med"; 
char json_val_high[] = "high"; 


// .............................................................................
// NOTES:
// Will change these functions below so that there is a single WRITE and a
// single READ function, which takes extra parameter: Key_id
// Will add Action functions - the functions that communicate with Airoha (or
// DPS, or Display, or Microphone, etc).
// For now, will call these from the individual Write functions.
// Eventually, the Write function can have new parameters:
// Action: t/f
// Notify: t/f (notify Core)
// SO if Core changes something, we set Notify to false, but if Display or Button
// changes something, we might set Notify to true. IN all three cases we will
// set Action to Ture.
// However, there may be cases when we do NOT want to action.
// For example, if Core changes the Profile In Use, so actions will probably
// take place (ANC Mode, EQ Presets etc.), but we don't want to send INFO
// messages to Core. Core should read back the entire Profile instead.
// .............................................................................

// *****************************************************************************
//
// *****************************************************************************

// **********************************************************************************************************************************************************
// **********************************************************************************************************************************************************
// Read / Write / Info functions, per Key - COMMON DATA SET
// **********************************************************************************************************************************************************
// **********************************************************************************************************************************************************

// .............................................................................

// %%%%%%%%%%%%%%%%%%%%%%
// Mode ID
// %%%%%%%%%%%%%%%%%%%%%%

// .............................................................................
int json_fn_r_mode_id(uint8_t p_indx, char *val_str)
{
    // Ignore profile index
    strcpy(val_str, data_com.mode_id);
       
    return 0;
}

// .............................................................................

// %%%%%%%%%%%%%%%%%%%%%%
// Firmware Version
// %%%%%%%%%%%%%%%%%%%%%%

// .............................................................................
int json_fn_r_fw_ver_id(uint8_t p_indx, char *val_str)
{
    // Ignore profile index
    strcpy(val_str, data_com.fw_ver_left);
       
    return 0;
}

// .............................................................................

// .............................................................................

// %%%%%%%%%%%%%%%%%%%%%%
// Serial Number
// %%%%%%%%%%%%%%%%%%%%%%

// .............................................................................
int json_fn_r_ser_num(uint8_t p_indx, char *val_str)
{
    // Get a copy of the Left MCU Serial Number
    mcu_ser_num_get(val_str);
    
    return 0;
}
// .............................................................................

// %%%%%%%%%%%%%%%%%%%%%%
// Common Data State
// %%%%%%%%%%%%%%%%%%%%%%

// .............................................................................
int json_fn_r_com_data_state(uint8_t p_indx, char *val_str)
{
    int err = 0;
    uint8_t u8_val;

    // Ignore profile index
    u8_val = data_com.com_state;
    
    // Convert uint8_t value to string
    err = utils_uint8_to_str(val_str, u8_val); 

    if (err < 0) {
        return -1;
    }
       
    return 0;
}
// .............................................................................


// %%%%%%%%%%%%%%%%%%%%%%
// Bluetooth Enabled
// %%%%%%%%%%%%%%%%%%%%%%

// .............................................................................
int json_fn_w_blue_enable(uint8_t p_indx, char *val_str)
{
    int err = 0;
    bool b_val;
    
    // Convert value string to bool
    err = utils_str_to_bool(val_str, &b_val);
    if (err < 0) {
        return -1;
    }
            
    // Ignore profile index
    data_com.blue_enabled = b_val;  
    
    return 0;
}
// .............................................................................
int json_fn_r_blue_enable(uint8_t p_indx, char *val_str)
{
    int err = 0;
    bool b_val;

    // Ignore profile index
    b_val = data_com.blue_enabled;

    // Convert uint8_t value to string
    err = utils_bool_to_str(val_str, b_val); 
    
    if (err < 0) {
        return -1;
    }
       
    return 0;
}
// .............................................................................
void json_fn_i_blue_enable()
{
    int err = 0;
    bool b_val;
    char val_str[JSON_DATA_VAL_STR_MAX_LEN];
    
    // Toggle bluetooth enable status
    b_val = !data_com.blue_enabled;
    data_com.blue_enabled = b_val;
    
    // Convert uint8_t value to string
    err = utils_bool_to_str(val_str, b_val); 
    if (err < 0) {
        return;
    }
    // Add key/val to INFO arrays
    err = json_add_i_key_val(json_key_Blue_Enabled, strlen(json_key_Blue_Enabled), val_str, strlen(val_str));
            
}
// .............................................................................
bool blue_enable_get()
{
    return(data_com.blue_enabled);
}
// .............................................................................
void blue_enable_set(bool val)
{
    data_com.blue_enabled = val;
}
// .............................................................................

// %%%%%%%%%%%%%%%%%%%%%%
// Audio Volume
// %%%%%%%%%%%%%%%%%%%%%%

// .............................................................................
int json_fn_a_audio_vol(int i_val)
{
    Q_comm_simp_t qs_comm_simp_msg;
    
    // Inform the Airoha chip
    
    // Send message to COMMS task
    qs_comm_simp_msg.id = COMMS_AUDIO_VOLUME;
    qs_comm_simp_msg.i_val = i_val;
    osQueueSendToBack(Q_comm_simp, &qs_comm_simp_msg, 10);
    
    return 0;
}
// .............................................................................
int json_fn_w_audio_vol(uint8_t p_indx, char *val_str)
{
    int err = 0;
    uint8_t u8_val;
    
    // Convert value string to uint8_t
    err = utils_str_to_uint8(val_str, &u8_val);
    if (err < 0) {
        return -1;
    }
            
    // Ignore profile index
    data_com.audio_vol = u8_val;
    
    return 0;
}
// .............................................................................
int json_fn_r_audio_vol(uint8_t p_indx, char *val_str)
{
    int err = 0;
    uint8_t u8_val;

    // Ignore profile index
    u8_val = data_com.audio_vol;

    // Convert uint8_t value to string
    err = utils_uint8_to_str(val_str, u8_val); 

    if (err < 0) {
        return -1;
    }
       
    return 0;
}
// .............................................................................
void json_fn_i_audio_vol(int pos)
{
    int err = 0;
    char val_str[JSON_DATA_VAL_STR_MAX_LEN];
    
    if (pos == 0) {
        // No change, so ignore)
        return;
    }
    if ((pos > 100) || (pos < -100)) {
        // Out of range, so ignore)
        return;
    }
    
    // Convert position value to string
    err = utils_int8_to_str(val_str, pos); 
    if (err < 0) {
        return;
    }
    
    // Action: Send message to Airoha chip
    json_fn_a_audio_vol(pos);
}



// .............................................................................

// %%%%%%%%%%%%%%%%%%%%%%
// Battery Level
// %%%%%%%%%%%%%%%%%%%%%%

// .............................................................................
int json_fn_w_batt_lvl(uint8_t p_indx, char *val_str)
{
    int err = 0;
    uint8_t u8_val;
    
    // Convert value string to uint8_t
    err = utils_str_to_uint8(val_str, &u8_val);
    if (err < 0) {
        return -1;
    }
            
    // Ignore profile index
    data_com.batt_lvl = u8_val;
    
    return 0;
}
// .............................................................................
int json_fn_r_batt_lvl(uint8_t p_indx, char *val_str)
{
    int err = 0;
    uint8_t u8_val;

    // Ignore profile index
    u8_val = data_com.batt_lvl;

    // Convert uint8_t value to string
    err = utils_uint8_to_str(val_str, u8_val); 

    if (err < 0) {
        return -1;
    }
       
    return 0;
}
// .............................................................................
void json_fn_i_batt_lvl()
{
    int err = 0;
    uint8_t u8_val;
    char val_str[JSON_DATA_VAL_STR_MAX_LEN];
    
    u8_val = data_com.batt_lvl;
    
    // Convert uint8_t value to string 
    err = utils_uint8_to_str(val_str, u8_val); 
    if (err < 0) {
        return;
    }
    // Add key/val to INFO arrays
    err = json_add_i_key_val(json_key_Batt_Level, strlen(json_key_Batt_Level), val_str, strlen(val_str));
    
}
// .............................................................................

// %%%%%%%%%%%%%%%%%%%%%%
// Battery Minutes
// %%%%%%%%%%%%%%%%%%%%%%

// .............................................................................
int json_fn_w_batt_min(uint8_t p_indx, char *val_str)
{
    int err = 0;
    uint16_t u16_val;
    
    // Convert value string to uint16_t
    err = utils_str_to_uint16(val_str, &u16_val);
    if (err < 0) {
        return -1;
    }
            
    // Ignore profile index
    data_com.batt_mins = u16_val;
    
    return 0;
}
// .............................................................................
int json_fn_r_batt_min(uint8_t p_indx, char *val_str)
{
    int err = 0;
    uint16_t u16_val;

    // Ignore profile index
    u16_val = data_com.batt_mins;

    // Convert uint16_t value to string
    err = utils_uint16_to_str(val_str, u16_val); 

    if (err < 0) {
        return -1;
    }
       
    return 0;
}
// .............................................................................
void json_fn_i_batt_min()
{
    int err = 0;
    uint16_t mins;
    char val_str[JSON_DATA_VAL_STR_MAX_LEN];
    
    
    // Update battery mins according to change in encoder position 
    mins = data_com.batt_mins;
    mins += 73;
    mins %= 1000;
    
    // Update value
    data_com.batt_mins = mins;

    // Convert uint16_t value to string 
    err = utils_uint16_to_str(val_str, mins); 
    if (err < 0) {
        return;
    }
    // Add key/val to INFO arrays
    err = json_add_i_key_val(json_key_Batt_Min, strlen(json_key_Batt_Min), val_str, strlen(val_str));
    
}
// .............................................................................

// %%%%%%%%%%%%%%%%%%%%%%
// Audio Cues Enabled
// %%%%%%%%%%%%%%%%%%%%%%

// .............................................................................
int json_fn_w_audio_cue_enable(uint8_t p_indx, char *val_str)
{
    int err = 0;
    bool b_val;
    
    // Convert value string to bool
    err = utils_str_to_bool(val_str, &b_val);
    if (err < 0) {
        return -1;
    }
            
    // Ignore profile index
    data_com.audio_cues_enabled = b_val;  
    
    return 0;
}
// .............................................................................
int json_fn_r_audio_cue_enable(uint8_t p_indx, char *val_str)
{
    int err = 0;
    bool b_val;

    // Ignore profile index
    b_val = data_com.audio_cues_enabled;

    // Convert uint8_t value to string
    err = utils_bool_to_str(val_str, b_val); 
    
    if (err < 0) {
        return -1;
    }
       
    return 0;
}
// .............................................................................
void json_fn_i_audio_cue_enable()
{
    int err = 0;
    bool b_val;
    char val_str[JSON_DATA_VAL_STR_MAX_LEN];
    
    // Toggle audio cues enable status
    b_val = !data_com.audio_cues_enabled;
    data_com.audio_cues_enabled = b_val;
    
    // Convert uint8_t value to string
    err = utils_bool_to_str(val_str, b_val); 
    if (err < 0) {
        return;
    }
    // Add key/val to INFO arrays
    err = json_add_i_key_val(json_key_Cues_Enabled, strlen(json_key_Cues_Enabled), val_str, strlen(val_str));
            
}
// .............................................................................
bool audio_cue_enable_get()
{
    return(data_com.audio_cues_enabled);
}
// .............................................................................
void audio_cue_enable_set(bool val)
{
    data_com.audio_cues_enabled = val;
}
// .............................................................................

// %%%%%%%%%%%%%%%%%%%%%%
// Charging Status
// %%%%%%%%%%%%%%%%%%%%%%

// .............................................................................
int json_fn_r_charge_status(uint8_t p_indx, char *val_str)
{
    int err = 0;
    bool b_val;

    // Ignore profile index
    b_val = data_com.charge_stat;

    // Convert uint8_t value to string
    err = utils_bool_to_str(val_str, b_val); 
    
    if (err < 0) {
        return -1;
    }
       
    return 0;
}
// .............................................................................
void json_fn_i_charge_status()
{
    int err = 0;
    bool b_val;
    char val_str[JSON_DATA_VAL_STR_MAX_LEN];
    
    // Toggle bluetooth enable status
    b_val = !data_com.charge_stat;
    data_com.charge_stat = b_val;
    
    // Convert uint8_t value to string
    err = utils_bool_to_str(val_str, b_val); 
    if (err < 0) {
        return;
    }
    // Add key/val to INFO arrays
    err = json_add_i_key_val(json_key_Charge_Status, strlen(json_key_Charge_Status), val_str, strlen(val_str));
            
}

// %%%%%%%%%%%%%%%%%%%%%%
// Signal Level
// %%%%%%%%%%%%%%%%%%%%%%

// .............................................................................
int json_fn_r_sig_lvl(uint8_t p_indx, char *val_str)
{
    int err = 0;
    uint8_t u8_val;

    // Ignore profile index
    u8_val = data_com.signal_lvl;

    // Convert uint8_t value to string
    err = utils_uint8_to_str(val_str, u8_val); 

    if (err < 0) {
        return -1;
    }
       
    return 0;
}
// .............................................................................
void json_fn_i_sig_lvl()
{
    int err = 0;
    uint8_t u8_val;
    char val_str[JSON_DATA_VAL_STR_MAX_LEN];
    
    u8_val = data_com.signal_lvl;
    
    // Convert uint8_t value to string 
    err = utils_uint8_to_str(val_str, u8_val); 
    if (err < 0) {
        return;
    }
    // Add key/val to INFO arrays
    err = json_add_i_key_val(json_key_Signal_Level, strlen(json_key_Signal_Level), val_str, strlen(val_str));
    
}
// .............................................................................


// %%%%%%%%%%%%%%%%%%%%%%
// Audio Power Off Timeout
// %%%%%%%%%%%%%%%%%%%%%%

// .............................................................................
int json_fn_w_audio_pwr_tim(uint8_t p_indx, char *val_str)
{
    int err = 0;
    uint16_t u16_val;
    
    // Convert value string to uint16_t
    err = utils_str_to_uint16(val_str, &u16_val);
    if (err < 0) {
        return -1;
    }
    // Verify Power Off time is within allowed limits
    if (( u16_val < APWOFF_TIME_MIN) || (u16_val > APWOFF_TIME_MAX)) {
        return -2;
    }
    
    // Ignore profile index
    data_com.auto_pwr_off_tim = u16_val;
    
    return 0;
}
// .............................................................................
int json_fn_r_audio_pwr_tim(uint8_t p_indx, char *val_str)
{
    int err = 0;
    uint16_t u16_val;

    // Ignore profile index
    u16_val = data_com.auto_pwr_off_tim;

    // Convert uint16_t value to string
    err = utils_uint16_to_str(val_str, u16_val); 

    if (err < 0) {
        return -1;
    }
       
    return 0;
}
// .............................................................................
void json_fn_i_audio_pwr_tim()
{
    int err = 0;
    uint16_t u16_val;
    char val_str[JSON_DATA_VAL_STR_MAX_LEN];
    
    u16_val = data_com.auto_pwr_off_tim;
    
    // Convert uint16_t value to string 
    err = utils_uint16_to_str(val_str, u16_val); 
    if (err < 0) {
        return;
    }
    // Add key/val to INFO arrays
    err = json_add_i_key_val(json_key_Audio_Pwr_Off_Time, strlen(json_key_Audio_Pwr_Off_Time), val_str, strlen(val_str));
    
}
// .............................................................................

// %%%%%%%%%%%%%%%%%%%%%%
// Audio Power Off Enable
// %%%%%%%%%%%%%%%%%%%%%%

// .............................................................................
int json_fn_w_audio_pwr_en(uint8_t p_indx, char *val_str)
{
    int err = 0;
    bool b_val;
    
    // Convert value string to bool
    err = utils_str_to_bool(val_str, &b_val);
    if (err < 0) {
        return -1;
    }
            
    // Ignore profile index
    data_com.auto_pwr_off_en = b_val;  
    
    return 0;
}
// .............................................................................
int json_fn_r_audio_pwr_en(uint8_t p_indx, char *val_str)
{
    int err = 0;
    bool b_val;
    
    // Ignore profile index
    b_val = data_com.auto_pwr_off_en;

    // Convert bool value to string
    err = utils_bool_to_str(val_str, b_val); 
    if (err < 0) {
        return -1;
    }
    
    // Ignore profile index
    data_com.auto_pwr_off_en = b_val;
    
    return 0;
}
// .............................................................................
void json_fn_i_audio_pwr_en()
{
    int err = 0;
    bool b_val;
    char val_str[JSON_DATA_VAL_STR_MAX_LEN];
    
    // Toggle bluetooth enable status
    b_val = !data_com.auto_pwr_off_en;
    data_com.auto_pwr_off_en = b_val;
    
    // Convert bool value to string
    err = utils_bool_to_str(val_str, b_val); 
    if (err < 0) {
        return;
    }
    // Add key/val to INFO arrays
    err = json_add_i_key_val(json_key_Audio_Pwr_Off_Enabled, strlen(json_key_Audio_Pwr_Off_Enabled), val_str, strlen(val_str));
    
}
// .............................................................................

// %%%%%%%%%%%%%%%%%%%%%%
// Display Menu Timeout
// %%%%%%%%%%%%%%%%%%%%%%

// .............................................................................
int json_fn_w_disp_menu_tim(uint8_t p_indx, char *val_str)
{
    int err = 0;
    uint16_t u16_val;
    
    // Convert value string to uint16_t
    err = utils_str_to_uint16(val_str, &u16_val);
    if (err < 0) {
        return -1;
    }
            
    // Ignore profile index
    data_com.display_menu_timeout = u16_val;
    
    return 0;
}
// .............................................................................
int json_fn_r_disp_menu_tim(uint8_t p_indx, char *val_str)
{
    int err = 0;
    uint16_t u16_val;

    // Ignore profile index
    u16_val = data_com.display_menu_timeout;

    // Convert uint16_t value to string
    err = utils_uint16_to_str(val_str, u16_val); 

    if (err < 0) {
        return -1;
    }
       
    return 0;
}
// .............................................................................
void json_fn_i_disp_menu_tim()
{
    int err = 0;
    uint16_t u16_val;
    char val_str[JSON_DATA_VAL_STR_MAX_LEN];
    
    u16_val = data_com.display_menu_timeout;
    
    // Convert uint16_t value to string 
    err = utils_uint16_to_str(val_str, u16_val); 
    if (err < 0) {
        return;
    }
    // Add key/val to INFO arrays
    err = json_add_i_key_val(json_key_Disp_Menu_Time, strlen(json_key_Disp_Menu_Time), val_str, strlen(val_str));
    
}
// .............................................................................

// %%%%%%%%%%%%%%%%%%%%%%
// Mic Enabled
// %%%%%%%%%%%%%%%%%%%%%%

// .............................................................................
int json_fn_w_mic_enable(uint8_t p_indx, char *val_str)
{
    int err = 0;
    bool b_val;
    
    // Convert value string to bool
    err = utils_str_to_bool(val_str, &b_val);
    if (err < 0) {
        return -1;
    }
            
    // Ignore profile index
    data_com.mic_enabled = b_val;  
    
    return 0;
}
// .............................................................................
int json_fn_r_mic_enable(uint8_t p_indx, char *val_str)
{
    int err = 0;
    bool b_val;

    // Ignore profile index
    b_val = data_com.mic_enabled;

    // Convert uint8_t value to string
    err = utils_bool_to_str(val_str, b_val); 
    
    if (err < 0) {
        return -1;
    }
       
    return 0;
}
// .............................................................................
void json_fn_i_mic_enable()
{
    int err = 0;
    bool b_val;
    char val_str[JSON_DATA_VAL_STR_MAX_LEN];
    
    // Toggle microphone mute status
    b_val = !data_com.mic_enabled;
    data_com.mic_enabled = b_val;
    
    // Convert uint8_t value to string
    err = utils_bool_to_str(val_str, b_val); 
    if (err < 0) {
        return;
    }
    // Add key/val to INFO arrays
    err = json_add_i_key_val(json_key_Mic_Enabled, strlen(json_key_Mic_Enabled), val_str, strlen(val_str));
            
}
// .............................................................................
bool mic_enable_get()
{
    return(data_com.mic_enabled);
}
// .............................................................................
void mic_enable_set(bool val)
{
    data_com.mic_enabled = val;
}

// %%%%%%%%%%%%%%%%%%%%%%
// Mic Connected
// %%%%%%%%%%%%%%%%%%%%%%

// .............................................................................
int json_fn_r_mic_conn(uint8_t p_indx, char *val_str)
{
    int err = 0;
    bool b_val;

    // Ignore profile index
    b_val = data_com.mic_connected;

    // Convert uint8_t value to string
    err = utils_bool_to_str(val_str, b_val); 
    
    if (err < 0) {
        return -1;
    }
       
    return 0;
}
// .............................................................................
void json_fn_i_mic_conn()
{
    int err = 0;
    bool b_val;
    char val_str[JSON_DATA_VAL_STR_MAX_LEN];
    
    // Toggle microphone mute status
    b_val = !data_com.mic_connected;
    data_com.mic_connected = b_val;
    
    // Convert uint8_t value to string
    err = utils_bool_to_str(val_str, b_val); 
    if (err < 0) {
        return;
    }
    // Add key/val to INFO arrays
    err = json_add_i_key_val(json_key_Mic_Connected, strlen(json_key_Mic_Connected), val_str, strlen(val_str));
            
}
// .............................................................................
bool mic_conn_get()
{
    return(data_com.mic_connected);
}
// .............................................................................

// %%%%%%%%%%%%%%%%%%%%%%
// EQ Presets
// %%%%%%%%%%%%%%%%%%%%%%
// .............................................................................

// .............................................................................
// Update one EQ Preset
// Logic:
// - Compare values and see if any changes
// - Compare the 'define' state and see if changed
// If any changes:
// If this EQ Preset is assigned to current Profile In Use AND EQs Enabled (globally)
// - if Defined -> Disable: 
// - - current Profile IN Use needs to be assigned first available, defined EQ Preset
// - - and then inform DSP
// - if Defined -> Enable: 
// - - not sure how this can happen - could flag an error
// - - or just accept this, and send values to DSP
// - if Data changed
// - - if EQ Preset is not defined, this is an error, or just ignore
// - - if EQ Preset is defined, then update values, and inform DSP
// Either this EQ Preset is NOT assigned to current Profile In Use and/or EQs Disabled (globally)
// - can just go ahead and update the data
// .............................................................................
int json_fn_w_eq_preset_X(uint8_t p_indx, data_eq_pre_t *eq_data)
{
    uint8_t p_in_use;
    uint8_t eq_in_use;
    uint8_t eq_new;
    bool b_eq_enabled;
    bool b_data_updated = false;
    bool b_name_updated = false;
    bool b_define_updated = false;
    int err;

    // p_indx is used for EQ Preset value 1..n
    if ((p_indx == 0) || (p_indx > MAX_EQ_PRESETS)) {
        return -1;
    }
    
    // Get the current set up
    p_in_use = data_com.prfl_in_use;
    b_eq_enabled = data_pro[p_in_use-1].eq_pre_enabled;
    eq_in_use = data_pro[p_in_use-1].eq_pre_in_use;

    // Update data, and set flag is anything has changed
    for (int i = 0; i < EQ_ARRAY_SIZE; i++) {
        if (data_com.eq_pre_data[p_indx-1].vals[i] != eq_data->vals[i]) {
            b_data_updated = true;
        }
        data_com.eq_pre_data[p_indx-1].vals[i] = eq_data->vals[i];
    }
    if (strcmp(data_com.eq_pre_data[p_indx-1].name, eq_data->name) != 0) { 
        b_name_updated = true;
    }
    sprintf(data_com.eq_pre_data[p_indx-1].name, eq_data->name);
    if (data_com.eq_pre_data[p_indx-1].defined != eq_data->defined)
    {
        b_define_updated = true;
    }
    data_com.eq_pre_data[p_indx-1].defined = eq_data->defined;
    
    // Now see if DSP needs to be notified
    
    if ((p_indx == eq_in_use) && (b_eq_enabled)) {
        // This is the EQ Preset that is assigned to the current Profile IN Use
        // AND Eq Presets are enabled for this profile, then we need to knwo if anything has been changed:
        if (b_define_updated) {
            // Change in 'define' state
            if (eq_data->defined) {
                // (TODO: Should be an error, but allow it for now...)
                // Notify DSP of (a) EQ Values then (b) Enable status
                err = json_fn_a_eq_preset_vals(p_indx);
                if (err < 0) {
                    return -2;
                }
                err = json_fn_a_eq_preset_en(p_indx);
                if (err < 0) {
                    return -3;
                }            
            } else {
                // Need to assign first available EQ Preset to this Profile
//                err = json_eq_presets_first_defined(&eq_new);
                if (err < 0) {
                    return -4;
                }
                // Assign new EQ Preset to the current Profile In Use
                data_pro[p_in_use-1].eq_pre_in_use = eq_new;
                // Send new values to DSP
                err = json_fn_a_eq_preset_vals(eq_new);
                if (err < 0) {
                    return -5;
                }
            }
        } else if (b_data_updated) {
            // Only data updated
            // Send values to DSP
            err = json_fn_a_eq_preset_vals(p_indx);
            if (err < 0) {
                return -6;
            }
        }
        
    }

    return 0;
}
// .............................................................................
// Update all EQ Presets
// .............................................................................
int json_fn_w_eq_presets_X(uint8_t p_indx, char *val_str)
{
    // Ignore p_indx
    // if this is called, have already updated everything
    
    return 0;
}
// .............................................................................
// Read one or all EQ Presets
// .............................................................................
int json_fn_r_eq_presets_X(uint8_t p_indx, char *val_str)
{
    int err = 0;
    bool b_val;
    char eq_state_str[8];
    char temp_eq_str[128];
    int len;
    bool b_all_undefined = true;
    
    // p_indx is EQ Preset index (1.. MAX_EQ_PRESETS)
    // 0 = read all
    if (p_indx > MAX_EQ_PRESETS) {
        return -1;
    }
    
    if (p_indx > 0) {
        // Read one EQ Preset

        // Build string for EQ Preset data
    
        // Example string: {\"eqId\": \"1\",\"eqPn\": \"eq name 1\",\"eqPd\": \"true\",\"eqPv\": [\"9\",\"8\",\"7\",\"6\",\"5\"]}
        
        // Preset state
        b_val = data_com.eq_pre_data[p_indx-1].defined;

        err = utils_bool_to_str(eq_state_str, b_val);
        if (err < 0) {
            return -2;
        }
        sprintf(json_tx_eq_preset_x_str, "{\"eqId\":\"%d\",\"eqPn\":\"%s\",\"eqPd\":\"%s\",\"eqPv\":[\"%d\",\"%d\",\"%d\",\"%d\",\"%d\"]}", p_indx, data_com.eq_pre_data[p_indx-1].name, eq_state_str, data_com.eq_pre_data[p_indx-1].vals[0], data_com.eq_pre_data[p_indx-1].vals[1], data_com.eq_pre_data[p_indx-1].vals[2], data_com.eq_pre_data[p_indx-1].vals[3], data_com.eq_pre_data[p_indx-1].vals[4]);

        // Pre-populate val_str[] so not returning empty string
        val_str[0] = '0';
        val_str[1] = '\0';

    } else {
        // Read all EQ Presets
        
        // STart of string
        strcpy(json_tx_eq_preset_x_str, "[");
        
        for (int i = 0; i < MAX_EQ_PRESETS; i++) {
            
            if (data_com.eq_pre_data[i].defined) {
                // Preset state
                b_val = data_com.eq_pre_data[i].defined;

                err = utils_bool_to_str(eq_state_str, b_val);
                if (err < 0) {
                    return -2;
                }
                sprintf(temp_eq_str, "{\"eqId\":\"%d\",\"eqPn\":\"%s\",\"eqPd\":\"%s\",\"eqPv\":[\"%d\",\"%d\",\"%d\",\"%d\",\"%d\"]},", i+1, data_com.eq_pre_data[i].name, eq_state_str, data_com.eq_pre_data[i].vals[0], data_com.eq_pre_data[i].vals[1], data_com.eq_pre_data[i].vals[2], data_com.eq_pre_data[i].vals[3], data_com.eq_pre_data[i].vals[4]);

                // Build the string by adding this EQ Preset
                strcat(json_tx_eq_preset_x_str,temp_eq_str);
                
                // At least one EQ Preset slot is defined
                b_all_undefined = false;
            }
        
        }
        // Replace last comma with ]
        len = strlen(json_tx_eq_preset_x_str);
        json_tx_eq_preset_x_str[len-1] = ']';
        // Close the string
        json_tx_eq_preset_x_str[len] = '\0';
        
        if (b_all_undefined) {
            // No EQ Presets defined, so overwrite resulting string
            strcpy(json_tx_eq_preset_x_str, "[]");
        }
    }

    return 0;
}
// .............................................................................

// %%%%%%%%%%%%%%%%%%%%%%
// Equaliser Preset Values
// %%%%%%%%%%%%%%%%%%%%%%

// .............................................................................
// Inform DSP of updated values
// .............................................................................
int json_fn_a_eq_preset_vals(uint8_t p_indx)
{
    if ((p_indx == 0) || (p_indx > MAX_EQ_PRESETS)) {
        return -1;
    }

    // data_com.eq_pre_data[p_indx-1].vals
    
    // TODO: Send new values to DSP
    
    return 0;
}


// %%%%%%%%%%%%%%%%%%%%%%
// DFU File Size
// %%%%%%%%%%%%%%%%%%%%%%

// .............................................................................
int json_fn_w_dfu_file_size(uint8_t p_indx, char *val_str)
{
    int err = 0;
    uint32_t u32_val;
    
    // Convert value string to uint32_t
    err = utils_str_to_int32(val_str, &u32_val);
    if (err < 0) {
        return -1;
    }
            
    // Ignore profile index
    data_com.dfu_file_size = u32_val;
    
    return 0;
}
// .............................................................................
int json_fn_r_dfu_file_size(uint8_t p_indx, char *val_str)
{
    int err = 0;
    uint32_t u32_val;

    // Ignore profile index
    u32_val = data_com.dfu_file_size;

    // Convert uint32_t value to string
    err = utils_uint32_to_str(val_str, u32_val); 

    if (err < 0) {
        return -1;
    }
       
    return 0;
}
// .............................................................................

// %%%%%%%%%%%%%%%%%%%%%%
// DFU File CRC
// %%%%%%%%%%%%%%%%%%%%%%

// .............................................................................
int json_fn_w_dfu_file_crc(uint8_t p_indx, char *val_str)
{
    int err = 0;
    uint16_t u16_val;
    
    // Convert value string to uint16_t
    err = utils_str_to_uint16(val_str, &u16_val);
    if (err < 0) {
        return -1;
    }
            
    // Ignore profile index
    data_com.dfu_file_crc = u16_val;
    
    return 0;
}
// .............................................................................
int json_fn_r_dfu_file_crc(uint8_t p_indx, char *val_str)
{
    int err = 0;
    uint16_t u16_val;

    // Ignore profile index
    u16_val = data_com.dfu_file_crc;

    // Convert uint16_t value to string
    err = utils_uint16_to_str(val_str, u16_val); 

    if (err < 0) {
        return -1;
    }
       
    return 0;
}
// .............................................................................

// %%%%%%%%%%%%%%%%%%%%%%
// DFU Segment Size
// %%%%%%%%%%%%%%%%%%%%%%

// .............................................................................
int json_fn_r_dfu_seq_size(uint8_t p_indx, char *val_str)
{
    int err = 0;
    uint16_t u16_val;

    // Ignore profile index
    u16_val = data_com.dfu_segment_size;

    // Convert uint16_t value to string
    err = utils_uint16_to_str(val_str, u16_val); 

    if (err < 0) {
        return -1;
    }
       
    return 0;
}
// .............................................................................


// %%%%%%%%%%%%%%%%%%%%%%
// Profile In Use
// %%%%%%%%%%%%%%%%%%%%%%

// .............................................................................
// If selected profile is not enabled, then either (a) reject, or (b) accept
// and enable
// Q: Is this writeable? 
// A: Yes, has to be - as Core user can just select a different profile from 
//      the dropdown menu
// .............................................................................
int json_fn_w_prof_in_use(uint8_t p_indx, char *val_str)
{
    int err = 0;
    uint8_t p_indx_new;
    
    // Ignore profile index. The value string contains the index (1..n) of the profile to switch to)

    // Convert value string to uint8_t
    err = utils_str_to_uint8(val_str, &p_indx_new);
    if (err < 0) {
        return -1;
    }
    
    if (data_pro[p_indx_new-1].pro_en == false) {
        // Selected profile is disabled, so can't switch to that profile
        return -2;
    }
    
    // Update the prfoile in use
    err = json_profiles_change_profie(p_indx_new);
    if (err < 0) {
        // Something went wrong when trying to switch to requested profile
        return -3;
    }
    
    // After sending the success response, generate an info message so that the
    // Core app knows which profile is the current Profile In Use
    k_sleep(K_MSEC(10));
    json_fn_i_prof_in_use();
    
    return 0;
}
// .............................................................................
int json_fn_r_prof_in_use(uint8_t p_indx, char *val_str)
{
    int err = 0;
    uint8_t u8_val;

    // Ignore profile index
    u8_val = data_com.prfl_in_use;

    // Convert uint8_t value to string
    err = utils_uint8_to_str(val_str, u8_val); 

    if (err < 0) {
        return -1;
    }
       
    return 0;
}
// .............................................................................
void json_fn_i_prof_in_use()
{
    int err = 0;
    uint8_t u8_val;
    char val_str[JSON_DATA_VAL_STR_MAX_LEN];
    
    u8_val = data_com.prfl_in_use;

    // Convert uint8_t value to string 
    err = utils_uint8_to_str(val_str, u8_val); 
    if (err < 0) {
        return;
    }
    // Add key/val to INFO arrays
    err = json_add_i_key_val(json_key_Prof_In_Use, strlen(json_key_Prof_In_Use), val_str, strlen(val_str));
    
}
// .............................................................................

// %%%%%%%%%%%%%%%%%%%%%%
// Profile Count
// %%%%%%%%%%%%%%%%%%%%%%

// .............................................................................
int json_fn_r_prof_count(uint8_t p_indx, char *val_str)
{
    int err = 0;
    uint8_t u8_val;

    // Ignore profile index
    u8_val = data_com.prfl_count;

    // Convert uint8_t value to string
    err = utils_uint8_to_str(val_str, u8_val); 

    if (err < 0) {
        return -1;
    }
       
    return 0;
}
// .............................................................................

// %%%%%%%%%%%%%%%%%%%%%%
// Profile Names
// %%%%%%%%%%%%%%%%%%%%%%

// .............................................................................
int json_fn_r_prof_names(uint8_t p_indx, char *val_str)
{
    int err = 0;

    // Create string which contains Profile ID and Name for every enabled Profile
    err = json_build_prof_names_str();

    if (err < 0) {
        return -1;
    }
    
    // Resulting string is in 'json_key_Prof_Names'
       
    return 0;
}
// .............................................................................
void json_fn_i_prof_names()
{
    int err = 0;
    char val_str[4];
    
    // Create string which contains Profile ID and Name for every enabled Profile
    err = json_build_prof_names_str();
    
    if (err < 0) {
        return;
    }

    // Pre-populate val_str[] so not passing empty string to function
    val_str[0] = ' ';
    val_str[1] = '\0';

    // Add key/val to INFO arrays
    err = json_add_i_key_val(json_key_Prof_Names, strlen(json_key_Prof_Names), val_str, strlen(val_str));
    
}
// .............................................................................

// **********************************************************************************************************************************************************
// **********************************************************************************************************************************************************
// Read / Write / Info functions, per Key - PROFILES
// **********************************************************************************************************************************************************
// **********************************************************************************************************************************************************

// %%%%%%%%%%%%%%%%%%%%%%
// Profile Enabled
// %%%%%%%%%%%%%%%%%%%%%%

// .............................................................................
// Enabling / Disabling a Profile...
//
// Rules: 
//
// 1. If Enabling (setting true), then straightforward - just set true.
//
// 2. If disabling (setting false):
//
// 3. If already false, nothing to do.
//
// 4. If currently true/enabled, then need to see if this is the Profile and 
// verify there is at least one other Profile enabled:
//
// 5. If this is NOT the profile in use and there is another profile enabled,
// then straightforward - just set false.
// 6. If this is NOT the profile in use and there is no other profile enabled,
// then this indicates a more fundamental issue, and should be flagged as
// error (although could decide to also leave it at true, but update the 
// Profile In Use).
// 7. If this IS the profile in use, and there is another enabled profile, then 
// set this profile to false/disabled, and set that other Profile to the Profile
// in Use.
// 8. If this IS the profile in use, and there is no other enabled profile, then
// ignore this write - and leave the as true/enabled. Maybe send back an
// error to reflect this.
// 
// .............................................................................
int json_fn_w_prof_enabled(uint8_t p_indx, char *val_str)
{
    int err = 0;
    bool b_val;
    uint8_t p_id_in_use;
    uint8_t p_id_other_en;
    
    // Note: p_indx is used for Profile Index (1..n)
    if ((p_indx == 0)|| (p_indx > MAX_PROFILE)) {
        return -1;
    }

    // Convert value string to boolean
    err = utils_str_to_bool(val_str, &b_val);
    if (err < 0) {
        return -1;
    }
    
    // Ready to go through the rules....
    
    // 1. If Enabling (setting true), then straightforward - just set true.
    if (b_val) {
        data_pro[p_indx-1].pro_en = true;  
    } else {
        // 2. If disabling (setting false):
        if (data_pro[p_indx-1].pro_en == false) {
            // 3. If already false, nothing to do.
        } else {
            // 4. If currently true/enabled, then need to see if this is the Profile and 
            // verify there is at least one other Profile enabled:
            
            // Get id of other enabled Profiel, if any
            p_id_other_en = json_profiles_get_other_enabled_profile(p_indx);
            // Get id of current Profile In Use
            p_id_in_use = data_com.prfl_in_use;
            
            if (p_indx != p_id_in_use) {
                // This is not the current Profile In Use
                if (p_id_other_en != 0) {
                    // 5. If this is NOT the profile in use and there is another profile enabled,
                    // then straightforward - just set false.
                    data_pro[p_indx-1].pro_en = false;  
                } else {
                    // 6. If this is NOT the profile in use and there is no other profile enabled,
                    // then this indicates a more fundamental issue, and should be flagged as
                    // error (although could decide to also leave it at true, but update the 
                    // Profile In Use).
                    return -2;
                }
            } else {
                // This IS the current Profile In Use
                if (p_id_other_en != 0) {
                    // 7. If this IS the profile in use, and there is another enabled profile, then 
                    // set this profile to false/disabled, and set that other Profile to the Profile
                    // in Use.
                    err = json_profiles_change_profie(p_id_other_en);
                    if (err < 0) {
                        // Something went wrong when trying to swap profiles
                        return -3;
                    }
                    // Inform the Core app
                    json_fn_i_prof_in_use();
                    // Disable the current profile
                    data_pro[p_indx-1].pro_en = false;  
                } else {
                    // 8. If this IS the profile in use, and there is no other enabled profile, then
                    // ignore this write - and leave the as true/enabled. Maybe send back an
                    // error to reflect this.
                    return -4;
                }
            }
        }
    }
    
    return 0;
}
// .............................................................................
int json_fn_r_prof_enabled(uint8_t p_indx, char *val_str)
{
    int err = 0;
    bool b_val;

    b_val = data_pro[p_indx-1].pro_en;

    // Convert boolean value to string
    err = utils_bool_to_str(val_str, b_val); 
    
    if (err < 0) {
        return -1;
    }
       
    return 0;
}
// .............................................................................

// %%%%%%%%%%%%%%%%%%%%%%
// Profile Name
// %%%%%%%%%%%%%%%%%%%%%%

// .............................................................................
int json_fn_w_prof_name(uint8_t p_indx, char *val_str)
{
    int len;
    
    // Note: p_indx is used for Profile Index (1..n)
    if ((p_indx == 0)|| (p_indx > MAX_PROFILE)) {
        return -1;
    }

    len = strlen(val_str);
    
    if ((len == 0) || (len > JSON_DATA_VAL_STR_MAX_LEN)) {
        return -2;
    }
    
    // Update profile name
    strcpy(data_pro[p_indx-1].prfl_name, val_str);

    return 0;
    
}
// .............................................................................

int json_fn_r_prof_name(uint8_t p_indx, char *val_str)
{
    // Note: p_indx is used for Profile Index (1..n)
    if ((p_indx == 0)|| (p_indx > MAX_PROFILE)) {
        return -1;
    }
    
    // Copy profile name
    strcpy(val_str, data_pro[p_indx-1].prfl_name);

    return 0;
}
// .............................................................................

// .............................................................................
// %%%%%%%%%%%%%%%%%%%%%%
// Profile State
// %%%%%%%%%%%%%%%%%%%%%%

// .............................................................................
int json_fn_r_pro_data_state(uint8_t p_indx, char *val_str)
{
    int err = 0;
    uint8_t u8_val;

    // Ignore profile index
    u8_val = data_pro[p_indx-1].pro_state;
    
    // Convert uint8_t value to string
    err = utils_uint8_to_str(val_str, u8_val); 

    if (err < 0) {
        return -1;
    }
       
    return 0;
}
// .............................................................................

// %%%%%%%%%%%%%%%%%%%%%%
// Display On
// %%%%%%%%%%%%%%%%%%%%%%

// .............................................................................
int json_fn_w_disp_on(uint8_t p_indx, char *val_str)
{
    int err = 0;
    bool b_val;
    
    // Convert value string to boolean
    err = utils_str_to_bool(val_str, &b_val);
    if (err < 0) {
        return -1;
    }
            
    data_pro[p_indx-1].display_on = b_val;  
    
    return 0;
}
// .............................................................................
int json_fn_r_disp_on(uint8_t p_indx, char *val_str)
{
    int err = 0;
    bool b_val;

    b_val = data_pro[p_indx-1].display_on;

    // Convert boolean value to string
    err = utils_bool_to_str(val_str, b_val); 
    
    if (err < 0) {
        return -1;
    }
       
    return 0;
}
// .............................................................................
void json_fn_i_disp_on()
{
    int err = 0;
    bool b_val;
    char val_str[JSON_DATA_VAL_STR_MAX_LEN];
    
    if ((data_com.prfl_in_use == 0) || (data_com.prfl_in_use > MAX_PROFILE)) {
        return;
    }
    
    // Toggle display on status
    b_val = !data_pro[data_com.prfl_in_use-1].display_on;
    data_pro[data_com.prfl_in_use-1].display_on = b_val;
    
    // Convert uint8_t value to string
    err = utils_bool_to_str(val_str, b_val); 
    if (err < 0) {
        return;
    }
    // Add key/val to INFO arrays
    err = json_add_i_key_val(json_key_Disp_On, strlen(json_key_Disp_On), val_str, strlen(val_str));
            
}
// .............................................................................

// %%%%%%%%%%%%%%%%%%%%%%
// Bluetooth on Power Up
// %%%%%%%%%%%%%%%%%%%%%%

// .............................................................................
int json_fn_w_blue_pwr_up(uint8_t p_indx, char *val_str)
{
    int err = 0;
    bool b_val;
    
    // Convert value string to bool
    err = utils_str_to_bool(val_str, &b_val);
    if (err < 0) {
        return -1;
    }
            
    data_pro[p_indx-1].blue_on_pwr_up = b_val;  
    
    return 0;
}
// .............................................................................
int json_fn_r_blue_pwr_up(uint8_t p_indx, char *val_str)
{
    int err = 0;
    bool b_val;

    // Ignore profile index
    b_val = data_pro[p_indx-1].blue_on_pwr_up;

    // Convert uint8_t value to string
    err = utils_bool_to_str(val_str, b_val); 
    
    if (err < 0) {
        return -1;
    }
       
    return 0;
}
// .............................................................................
void json_fn_i_blue_pwr_up()
{
    int err = 0;
    bool b_val;
    char val_str[JSON_DATA_VAL_STR_MAX_LEN];
    
    if ((data_com.prfl_in_use == 0) || (data_com.prfl_in_use > MAX_PROFILE)) {
        return;
    }
    
    // Toggle bluetooth on power up status
    b_val = !data_pro[data_com.prfl_in_use-1].blue_on_pwr_up;
    data_pro[data_com.prfl_in_use-1].blue_on_pwr_up = b_val;
    
    // Convert uint8_t value to string
    err = utils_bool_to_str(val_str, b_val); 
    if (err < 0) {
        return;
    }
    // Add key/val to INFO arrays
    err = json_add_i_key_val(json_key_Blue_Power_Up, strlen(json_key_Blue_Power_Up), val_str, strlen(val_str));
            
}
// .............................................................................

// %%%%%%%%%%%%%%%%%%%%%%
// Display Brightness
// %%%%%%%%%%%%%%%%%%%%%%

// .............................................................................
int json_fn_a_disp_bright(uint8_t u8_val)
{
    u8_val = u8_val / 12;
    u8_val &= 0x07;
//    led_disp_pwm(u8_val);                   // local left backlight
    printf("[led disp %d]\n", u8_val);      // remote right backlight
    
    return 0;
}
// .............................................................................
int json_fn_n_disp_bright(uint8_t u8_val)
{
    int err;
    char val_str[JSON_DATA_VAL_STR_MAX_LEN];

    // Convert uint8_t value to string 
    err = utils_uint8_to_str(val_str, u8_val); 
    if (err < 0) {
        return -1;
    }
    // Add key/val to INFO arrays
    err = json_add_i_key_val(json_key_Disp_Bright, strlen(json_key_Disp_Bright), val_str, strlen(val_str));
    
    return 0;
}
// .............................................................................
int json_fn_w_disp_bright(uint8_t p_indx, char *val_str)
{
    int err = 0;
    uint8_t u8_val;
    
    // Convert value string to uint8_t
    err = utils_str_to_uint8(val_str, &u8_val);
    if (err < 0) {
        return -1;
    }
            
    data_pro[p_indx-1].display_bright = u8_val;
    
    // Add action!
    json_fn_a_disp_bright(u8_val);
    
    return 0;
}
// .............................................................................
int json_fn_r_disp_bright(uint8_t p_indx, char *val_str)
{
    int err = 0;
    uint8_t u8_val;

    u8_val = data_pro[p_indx-1].display_bright;

    // Convert uint8_t value to string
    err = utils_uint8_to_str(val_str, u8_val); 

    if (err < 0) {
        return -1;
    }
       
    return 0;
}
// .............................................................................

// %%%%%%%%%%%%%%%%%%%%%%
// Chat Mix
// %%%%%%%%%%%%%%%%%%%%%%

// .............................................................................
void json_fn_a_chat_mix(int i_val)
{
    Q_comm_simp_t qs_comm_simp_msg;
	printf(" A  \n");

    // Inform the Airoha chip
    // Send message to COMMS task
    qs_comm_simp_msg.id = COMMS_CHAT_MIX;
    qs_comm_simp_msg.i_val = i_val;
    osQueueSendToBack(Q_comm_simp, &qs_comm_simp_msg, 10);
	//printf("simp message id:0x%02x, val:%d \n ",qs_comm_simp_msg.id,qs_comm_simp_msg.i_val);
}
void json_fn_n_chat_mix(uint8_t u8_val)
{
    int err;
    char val_str[JSON_DATA_VAL_STR_MAX_LEN];
    printf(" N  \n");
    // Convert position value to string
    err = utils_uint8_to_str(val_str, u8_val); 
    if (err < 0) {
        return;
    }
   //printf("val_str %s\n",val_str);
    err = json_add_i_key_val(json_key_Chat_Mix, strlen(json_key_Chat_Mix), val_str, strlen(val_str));    
}
// .............................................................................
int json_fn_w_chat_mix(uint8_t p_indx, char *val_str)
{
    int err = 0;
    uint8_t u8_val;
    
    // Convert value string to uint8_t
    err = utils_str_to_uint8(val_str, &u8_val);
    if (err < 0) {
        return -1;
    }
            
    data_pro[p_indx-1].chat_mix = u8_val;
    
    return 0;
}
// .............................................................................
int json_fn_r_chat_mix(uint8_t p_indx, char *val_str)
{
    int err = 0;
    uint8_t u8_val;

    u8_val = data_pro[p_indx-1].chat_mix;

    // Convert uint8_t value to string
    err = utils_uint8_to_str(val_str, u8_val); 

    if (err < 0) {
        return -1;
    }
       
    return 0;
}
// .............................................................................
void json_fn_i_chat_mix(int pos)
{
    int err = 0;
    uint8_t u8_val;
    int i_val;
    
    if (pos == 0) {
        // No change, so ignore)
        return;
    }
    if ((pos > 100) || (pos < -100)) {
        // Out of range, so ignore)
        return;
    }
    if ((data_com.prfl_in_use == 0) || (data_com.prfl_in_use > MAX_PROFILE)) {
        // Invalid profile
        return;
    }    
    
    // Take a copy of current Chatmix setting
    u8_val = data_pro[data_com.prfl_in_use-1].chat_mix;
    
    // Want to increment/decrement from this value
    i_val = (int)u8_val;
    i_val += pos;
    if (i_val < 0) {
        i_val = 0;
    }
    if (i_val > 100) {
        i_val = 100;
    }
    u8_val = (uint8_t)i_val;
    
    if (u8_val == data_pro[data_com.prfl_in_use-1].chat_mix) {
        // No change - must have been at min/max
        return;
    }
    
    // Modify the setting
    data_pro[data_com.prfl_in_use-1].chat_mix = u8_val;

    // Action: Send message to Airoha chip
    json_fn_a_chat_mix(i_val);
    
    // Notify Core app
    json_fn_n_chat_mix(u8_val);
}
// .............................................................................

// %%%%%%%%%%%%%%%%%%%%%%
// Media Mix
// %%%%%%%%%%%%%%%%%%%%%%

// .............................................................................
int json_fn_w_media_mix(uint8_t p_indx, char *val_str)
{
    int err = 0;
    uint8_t u8_val;
    
    // Convert value string to uint8_t
    err = utils_str_to_uint8(val_str, &u8_val);
    if (err < 0) {
        return -1;
    }
            
    data_pro[p_indx-1].media_mix = u8_val;
    
    return 0;
}
// .............................................................................
int json_fn_r_media_mix(uint8_t p_indx, char *val_str)
{
    int err = 0;
    uint8_t u8_val;

    u8_val = data_pro[p_indx-1].media_mix;

    // Convert uint8_t value to string
    err = utils_uint8_to_str(val_str, u8_val); 

    if (err < 0) {
        return -1;
    }
       
    return 0;
}
// .............................................................................
void json_fn_i_media_mix()
{
    int err = 0;
    uint8_t u8_val;
    char val_str[JSON_DATA_VAL_STR_MAX_LEN];
    
    if ((data_com.prfl_in_use == 0) || (data_com.prfl_in_use > MAX_PROFILE)) {
        return;
    }
    
    u8_val = data_pro[data_com.prfl_in_use-1].media_mix;

    // Convert uint8_t value to string
    err = utils_uint8_to_str(val_str, u8_val); 
    if (err < 0) {
        return;
    }

    // Add key/val to INFO arrays
    err = json_add_i_key_val(json_key_Media_Mix, strlen(json_key_Media_Mix), val_str, strlen(val_str));
}
// .............................................................................

// %%%%%%%%%%%%%%%%%%%%%%
// Microphone Volume
// %%%%%%%%%%%%%%%%%%%%%%

// .............................................................................
int json_fn_w_mic_vol(uint8_t p_indx, char *val_str)
{
    int err = 0;
    uint8_t u8_val;
    
    // Convert value string to uint8_t
    err = utils_str_to_uint8(val_str, &u8_val);
    if (err < 0) {
        return -1;
    }
            
    data_pro[p_indx-1].mic_vol = u8_val;
    
    return 0;
}
// .............................................................................
int json_fn_r_mic_vol(uint8_t p_indx, char *val_str)
{
    int err = 0;
    uint8_t u8_val;

    u8_val = data_pro[p_indx-1].mic_vol;

    // Convert uint8_t value to string
    err = utils_uint8_to_str(val_str, u8_val); 

    if (err < 0) {
        return -1;
    }
       
    return 0;
}
// .............................................................................
void json_fn_i_mic_vol()
{
    int err = 0;
    uint8_t u8_val;
    char val_str[JSON_DATA_VAL_STR_MAX_LEN];
    
    if ((data_com.prfl_in_use == 0) || (data_com.prfl_in_use > MAX_PROFILE)) {
        return;
    }
    
    u8_val = data_pro[data_com.prfl_in_use-1].mic_vol;

    // Convert uint8_t value to string
    err = utils_uint8_to_str(val_str, u8_val); 
    if (err < 0) {
        return;
    }

    // Add key/val to INFO arrays
    err = json_add_i_key_val(json_key_Mic_Volume, strlen(json_key_Mic_Volume), val_str, strlen(val_str));
}
// .............................................................................

// %%%%%%%%%%%%%%%%%%%%%%
// Microphone Sidetone
// %%%%%%%%%%%%%%%%%%%%%%
// .............................................................................

// .............................................................................
int json_fn_w_mic_sidetone(uint8_t p_indx, char *val_str)
{
    uint8_t u8_side;
    
    if (!strcmp(val_str, json_val_off)) { 
        u8_side = 0;
    }
    else if (!strcmp(val_str, json_val_low)) { 
        u8_side = 1;
    }
    else if (!strcmp(val_str, json_val_med)) { 
        u8_side = 2;
    }
    else if (!strcmp(val_str, json_val_high)) { 
        u8_side = 3;
    }
    else { 
        // Invalid setting / string
        return -1;
    }
    
    if (data_pro[p_indx-1].mic_side_tone == u8_side) {
        // Not an error, but nothing to do
        return 0;
    }
    
    // Update setting
    data_pro[p_indx-1].mic_side_tone = u8_side;
    
    // Do something with u8_side: send message to AIroha 
    
    
    return 0;
}
// .............................................................................
int json_fn_r_mic_sidetone(uint8_t p_indx, char *val_str)
{
    switch (data_pro[p_indx-1].mic_side_tone) {
        case 1:
        strcpy(val_str,json_val_low);
            break;
        case 2:
        strcpy(val_str,json_val_med);
            break;
        case 3:
        strcpy(val_str,json_val_high);
            break;
        case 0:
        default:
            data_pro[p_indx-1].mic_side_tone = 0;
            strcpy(val_str,json_val_off);
            break;
    }

    return 0;
}
// .............................................................................
void json_fn_i_mic_sidetone()
{
    int err = 0;
    uint8_t u8_val;
    char val_str[JSON_DATA_VAL_STR_MAX_LEN];
    
    if ((data_com.prfl_in_use == 0) || (data_com.prfl_in_use > MAX_PROFILE)) {
        return;
    }
    
    u8_val = data_pro[data_com.prfl_in_use-1].mic_side_tone;
    
    // For now, just increment value 0..3
    u8_val = (u8_val + 1) % 4;
    
    data_pro[data_com.prfl_in_use-1].mic_side_tone = u8_val;

    switch (u8_val) {
        case 1:
            strcpy(val_str,json_val_low);
            break;
        case 2:
            strcpy(val_str,json_val_med);
            break;
        case 3:
            strcpy(val_str,json_val_high);
            break;
        case 0:
        default:
            data_pro[data_com.prfl_in_use-1].mic_side_tone = 0;
            strcpy(val_str,json_val_off);
            break;
    }

    // Add key/val to INFO arrays
    err = json_add_i_key_val(json_key_Mic_Side_Tone, strlen(json_key_Mic_Side_Tone), val_str, strlen(val_str));
}
// .............................................................................

// %%%%%%%%%%%%%%%%%%%%%%
// Microphone Noisegate
// %%%%%%%%%%%%%%%%%%%%%%
// .............................................................................

// .............................................................................
int json_fn_w_mic_noisegate(uint8_t p_indx, char *val_str)
{
    uint8_t u8_side;
    
    if (!strcmp(val_str, json_val_off)) { 
        u8_side = 0;
    }
    else if (!strcmp(val_str, json_val_low)) { 
        u8_side = 1;
    }
    else if (!strcmp(val_str, json_val_med)) { 
        u8_side = 2;
    }
    else if (!strcmp(val_str, json_val_high)) { 
        u8_side = 3;
    }
    else { 
        // Invalid setting / string
        return -1;
    }
    
    if (data_pro[p_indx-1].mic_noise_gate == u8_side) {
        // Not an error, but nothing to do
        return 0;
    }
    
    // Update setting
    data_pro[p_indx-1].mic_noise_gate = u8_side;
    
    // Do something with u8_side: send message to AIroha 
    
    
    return 0;
}
// .............................................................................
int json_fn_r_mic_noisegate(uint8_t p_indx, char *val_str)
{
    switch (data_pro[p_indx-1].mic_noise_gate) {
        case 1:
        strcpy(val_str,json_val_low);
            break;
        case 2:
        strcpy(val_str,json_val_med);
            break;
        case 3:
        strcpy(val_str,json_val_high);
            break;
        case 0:
        default:
            data_pro[p_indx-1].mic_noise_gate = 0;
            strcpy(val_str,json_val_off);
            break;
    }

    return 0;
}
// .............................................................................
void json_fn_i_mic_noisegate()
{
    int err = 0;
    uint8_t u8_val;
    char val_str[JSON_DATA_VAL_STR_MAX_LEN];
    
    if ((data_com.prfl_in_use == 0) || (data_com.prfl_in_use > MAX_PROFILE)) {
        return;
    }
    
    u8_val = data_pro[data_com.prfl_in_use-1].mic_noise_gate;
    
    // For now, just increment value 0..3
    u8_val = (u8_val + 1) % 4;
    
    data_pro[data_com.prfl_in_use-1].mic_noise_gate = u8_val;

    switch (u8_val) {
        case 1:
            strcpy(val_str,json_val_low);
            break;
        case 2:
            strcpy(val_str,json_val_med);
            break;
        case 3:
            strcpy(val_str,json_val_high);
            break;
        case 0:
        default:
            data_pro[data_com.prfl_in_use-1].mic_noise_gate = 0;
            strcpy(val_str, json_val_off);
            break;
    }

    // Add key/val to INFO arrays
    err = json_add_i_key_val(json_key_Mic_Noise_Gate, strlen(json_key_Mic_Noise_Gate), val_str, strlen(val_str));
}
// .............................................................................

// %%%%%%%%%%%%%%%%%%%%%%
// Microphone Noise Reduction
// %%%%%%%%%%%%%%%%%%%%%%

// .............................................................................
int json_fn_w_mic_noise_red(uint8_t p_indx, char *val_str)
{
    int err = 0;
    bool b_val;
    
    // Convert value string (on/off) to bool
    err = utils_on_off_str_to_bool(val_str, &b_val);
    if (err < 0) {
        return -1;
    }
            
    data_pro[p_indx-1].mic_noise_reduction = b_val;  
    
    return 0;
}
// .............................................................................
int json_fn_r_mic_noise_red(uint8_t p_indx, char *val_str)
{
    int err = 0;
    bool b_val;

    b_val = data_pro[p_indx-1].mic_noise_reduction;

    // Convert uint8_t value to string (on/off))
    err = utils_bool_to_on_off_str(val_str, b_val); 
    
    if (err < 0) {
        return -1;
    }
       
    return 0;
}
// .............................................................................
void json_fn_i_mic_noise_red()
{
    int err = 0;
    bool b_val;
    char val_str[JSON_DATA_VAL_STR_MAX_LEN];
    
    if ((data_com.prfl_in_use == 0) || (data_com.prfl_in_use > MAX_PROFILE)) {
        return;
    }
    
    // Toggle display on status
    b_val = !data_pro[data_com.prfl_in_use-1].mic_noise_reduction;
    data_pro[data_com.prfl_in_use-1].mic_noise_reduction = b_val;
    
    // Convert uint8_t value to string
    err = utils_bool_to_on_off_str(val_str, b_val); 
    if (err < 0) {
        return;
    }
    // Add key/val to INFO arrays
    err = json_add_i_key_val(json_key_Mic_Noise_Reduction, strlen(json_key_Mic_Noise_Reduction), val_str, strlen(val_str));
            
}
// .............................................................................

// %%%%%%%%%%%%%%%%%%%%%%
// Microphone Volume Normalisation
// %%%%%%%%%%%%%%%%%%%%%%

// .............................................................................
int json_fn_w_mic_vol_norm(uint8_t p_indx, char *val_str)
{
    int err = 0;
    bool b_val;
    
    // Convert value string to bool
    err = utils_on_off_str_to_bool(val_str, &b_val);
    if (err < 0) {
        return -1;
    }
            
    data_pro[p_indx-1].mic_vol_norm = b_val;  
    
    return 0;
}
// .............................................................................
int json_fn_r_mic_vol_norm(uint8_t p_indx, char *val_str)
{
    int err = 0;
    bool b_val;

    b_val = data_pro[p_indx-1].mic_vol_norm;

    // Convert uint8_t value to string
    err = utils_bool_to_on_off_str(val_str, b_val); 
    
    if (err < 0) {
        return -1;
    }
       
    return 0;
}
// .............................................................................
void json_fn_i_mic_vol_norm()
{
    int err = 0;
    bool b_val;
    char val_str[JSON_DATA_VAL_STR_MAX_LEN];
    
    if ((data_com.prfl_in_use == 0) || (data_com.prfl_in_use > MAX_PROFILE)) {
        return;
    }
    
    // Toggle display on status
    b_val = !data_pro[data_com.prfl_in_use-1].mic_vol_norm;
    data_pro[data_com.prfl_in_use-1].mic_vol_norm = b_val;
    
    // Convert uint8_t value to string
    err = utils_bool_to_on_off_str(val_str, b_val); 
    if (err < 0) {
        return;
    }
    // Add key/val to INFO arrays
    err = json_add_i_key_val(json_key_Mic_Vol_Norm, strlen(json_key_Mic_Vol_Norm), val_str, strlen(val_str));
            
}
// .............................................................................

// %%%%%%%%%%%%%%%%%%%%%%
// Microphone LED Brightness
// %%%%%%%%%%%%%%%%%%%%%%

// .............................................................................
int json_fn_w_mic_led_bright(uint8_t p_indx, char *val_str)
{
    int err = 0;
    uint8_t u8_val;
    
    // Convert value string to uint8_t
    err = utils_str_to_uint8(val_str, &u8_val);
    if (err < 0) {
        return -1;
    }
            
    data_pro[p_indx-1].mic_led_bright = u8_val;
    
    return 0;
}
// .............................................................................
int json_fn_r_mic_led_bright(uint8_t p_indx, char *val_str)
{
    int err = 0;
    uint8_t u8_val;

    u8_val = data_pro[p_indx-1].mic_led_bright;

    // Convert uint8_t value to string
    err = utils_uint8_to_str(val_str, u8_val); 

    if (err < 0) {
        return -1;
    }
       
    return 0;
}
// .............................................................................
void json_fn_i_mic_led_bright()
{
    int err = 0;
    uint8_t u8_val;
    char val_str[JSON_DATA_VAL_STR_MAX_LEN];
    
    if ((data_com.prfl_in_use == 0) || (data_com.prfl_in_use > MAX_PROFILE)) {
        return;
    }
    
    u8_val = data_pro[data_com.prfl_in_use-1].mic_led_bright;

    // Convert uint8_t value to string
    err = utils_uint8_to_str(val_str, u8_val); 
    if (err < 0) {
        return;
    }

    // Add key/val to INFO arrays
    err = json_add_i_key_val(json_key_Mic_Led_Bright, strlen(json_key_Mic_Led_Bright), val_str, strlen(val_str));
}
// .............................................................................

// %%%%%%%%%%%%%%%%%%%%%%
// ANC Mode
// %%%%%%%%%%%%%%%%%%%%%%

// .............................................................................
int json_fn_w_anc_mode(uint8_t p_indx, char *val_str)
{
    int err = 0;
    uint8_t u8_val;
    
    // Convert value string to uint8_t
    err = utils_str_to_uint8(val_str, &u8_val);
    if (err < 0) {
        return -1;
    }
    
    // 1: Noise Cancelation, 2: Transparency Mode, 3: Normal Mode
    if ((u8_val < 1) || (u8_val > 3)) {
        return -2;
    }
    
    if (data_pro[p_indx-1].anc_mode == u8_val) {
        // Not an error, but nothing to do
        return 0;
    }
            
    data_pro[p_indx-1].anc_mode = u8_val;
    
    return 0;
}
// .............................................................................
int json_fn_r_anc_mode(uint8_t p_indx, char *val_str)
{
    int err = 0;
    uint8_t u8_val;

    u8_val = data_pro[p_indx-1].anc_mode;

    // Convert uint8_t value to string
    err = utils_uint8_to_str(val_str, u8_val); 

    if (err < 0) {
        return -1;
    }
       
    return 0;
}
// .............................................................................
void json_fn_i_anc_mode()
{
    int err = 0;
    uint8_t u8_val;
    char val_str[JSON_DATA_VAL_STR_MAX_LEN];
    
    if ((data_com.prfl_in_use == 0) || (data_com.prfl_in_use > MAX_PROFILE)) {
        return;
    }
    
    u8_val = data_pro[data_com.prfl_in_use-1].anc_mode;
    
    // For now, increment mode
    u8_val = u8_val + 1;
    // 1: Noise Cancelation, 2: Transparency Mode, 3: Normal Mode
    if (u8_val > 3) {
        u8_val = 1;
    }

    // Convert uint8_t value to string
    err = utils_uint8_to_str(val_str, u8_val); 
    if (err < 0) {
        return;
    }

    // Add key/val to INFO arrays
    err = json_add_i_key_val(json_key_ANC_Mode, strlen(json_key_ANC_Mode), val_str, strlen(val_str));
}
// .............................................................................

// %%%%%%%%%%%%%%%%%%%%%%
// Audio Surround
// %%%%%%%%%%%%%%%%%%%%%%

// .............................................................................
int json_fn_w_audio_surround(uint8_t p_indx, char *val_str)
{
    int err = 0;
    bool b_val;
    
    // Convert value string to bool
    err = utils_on_off_str_to_bool(val_str, &b_val);
    if (err < 0) {
        return -1;
    }
            
    data_pro[p_indx-1].audio_surr = b_val;  
    
    return 0;
}
// .............................................................................
int json_fn_r_audio_surround(uint8_t p_indx, char *val_str)
{
    int err = 0;
    bool b_val;

    b_val = data_pro[p_indx-1].audio_surr;

    // Convert uint8_t value to string
    err = utils_bool_to_on_off_str(val_str, b_val); 
    
    if (err < 0) {
        return -1;
    }
       
    return 0;
}
// .............................................................................
void json_fn_i_audio_surround()
{
    int err = 0;
    bool b_val;
    char val_str[JSON_DATA_VAL_STR_MAX_LEN];
    
    if ((data_com.prfl_in_use == 0) || (data_com.prfl_in_use > MAX_PROFILE)) {
        return;
    }
    
    // Toggle display on status
    b_val = !data_pro[data_com.prfl_in_use-1].audio_surr;
    data_pro[data_com.prfl_in_use-1].audio_surr = b_val;
    
    // Convert uint8_t value to string
    err = utils_bool_to_on_off_str(val_str, b_val); 
    if (err < 0) {
        return;
    }
    // Add key/val to INFO arrays
    err = json_add_i_key_val(json_key_Audio_Surround, strlen(json_key_Audio_Surround), val_str, strlen(val_str));
            
}
// .............................................................................

// %%%%%%%%%%%%%%%%%%%%%%
// Lighting Effect
// %%%%%%%%%%%%%%%%%%%%%%

// .............................................................................
int json_fn_w_light_effect(uint8_t p_indx, char *val_str)
{
    int err = 0;
    uint8_t u8_val;
    
    // Convert value string to uint8_t
    err = utils_str_to_uint8(val_str, &u8_val);
    if (err < 0) {
        return -1;
    }

    // 1: None, 2: Static, 3: Breath, 4: Cycle
    if ((u8_val < 1) || (u8_val > 4)) {
        return -2;
    }
    
    if (data_pro[p_indx-1].light_effect == u8_val) {
        // Not an error, but nothing to do
        return 0;
    }
            
    data_pro[p_indx-1].light_effect = u8_val;
    
    return 0;
}
// .............................................................................
int json_fn_r_light_effect(uint8_t p_indx, char *val_str)
{
    int err = 0;
    uint8_t u8_val;

    u8_val = data_pro[p_indx-1].light_effect;

    // Convert uint8_t value to string
    err = utils_uint8_to_str(val_str, u8_val); 

    if (err < 0) {
        return -1;
    }
       
    return 0;
}
// .............................................................................
void json_fn_i_light_effect()
{
    int err = 0;
    uint8_t u8_val;
    char val_str[JSON_DATA_VAL_STR_MAX_LEN];
    
    if ((data_com.prfl_in_use == 0) || (data_com.prfl_in_use > MAX_PROFILE)) {
        return;
    }
    
    u8_val = data_pro[data_com.prfl_in_use-1].light_effect;
    
    // For now, increment mode
    u8_val = u8_val + 1;
    // 1: None, 2: Static, 3: Breath, 4: Cycle
    if (u8_val > 4) {
        u8_val = 1;
    }

    // Convert uint8_t value to string
    err = utils_uint8_to_str(val_str, u8_val); 
    if (err < 0) {
        return;
    }

    // Add key/val to INFO arrays
    err = json_add_i_key_val(json_key_Light_Effect, strlen(json_key_Light_Effect), val_str, strlen(val_str));
}
// .............................................................................

// %%%%%%%%%%%%%%%%%%%%%%
// Lighting Enabled
// %%%%%%%%%%%%%%%%%%%%%%

// .............................................................................
int json_fn_w_light_en(uint8_t p_indx, char *val_str)
{
    int err = 0;
    bool b_val;
    
    // Convert value string to bool
    err = utils_str_to_bool(val_str, &b_val);
    if (err < 0) {
        return -1;
    }
            
    data_pro[p_indx-1].light_enabled = b_val;  
    
    return 0;
}
// .............................................................................
int json_fn_r_light_en(uint8_t p_indx, char *val_str)
{
    int err = 0;
    bool b_val;

    b_val = data_pro[p_indx-1].light_enabled;

    // Convert uint8_t value to string
    err = utils_bool_to_str(val_str, b_val); 
    
    if (err < 0) {
        return -1;
    }
       
    return 0;
}
// .............................................................................
void json_fn_i_light_en()
{
    int err = 0;
    bool b_val;
    char val_str[JSON_DATA_VAL_STR_MAX_LEN];
    
    if ((data_com.prfl_in_use == 0) || (data_com.prfl_in_use > MAX_PROFILE)) {
        return;
    }
    
    // Toggle display on status
    b_val = !data_pro[data_com.prfl_in_use-1].light_enabled;
    data_pro[data_com.prfl_in_use-1].light_enabled = b_val;
    
    // Convert uint8_t value to string
    err = utils_bool_to_str(val_str, b_val); 
    if (err < 0) {
        return;
    }
    // Add key/val to INFO arrays
    err = json_add_i_key_val(json_key_Light_En, strlen(json_key_Light_En), val_str, strlen(val_str));
            
}
// .............................................................................

// %%%%%%%%%%%%%%%%%%%%%%
// Lighting Colour Mode
// %%%%%%%%%%%%%%%%%%%%%%

// .............................................................................
int json_fn_w_light_colour_mode(uint8_t p_indx, char *val_str)
{
    int err = 0;
    uint8_t u8_val;
    
    // Convert value string to uint8_t
    err = utils_str_to_uint8(val_str, &u8_val);
    if (err < 0) {
        return -1;
    }

    // 1: Solid, 2: Gradient
    if ((u8_val < 1) || (u8_val > 2)) {
        return -2;
    }
    
    if (data_pro[p_indx-1].light_color_mode == u8_val) {
        // Not an error, but nothing to do
        return 0;
    }
            
    data_pro[p_indx-1].light_color_mode = u8_val;
    
    return 0;
}
// .............................................................................
int json_fn_r_light_colour_mode(uint8_t p_indx, char *val_str)
{
    int err = 0;
    uint8_t u8_val;

    u8_val = data_pro[p_indx-1].light_color_mode;

    // Convert uint8_t value to string
    err = utils_uint8_to_str(val_str, u8_val); 

    if (err < 0) {
        return -1;
    }
       
    return 0;
}
// .............................................................................
void json_fn_i_light_colour_mode()
{
    int err = 0;
    uint8_t u8_val;
    char val_str[JSON_DATA_VAL_STR_MAX_LEN];
    
    if ((data_com.prfl_in_use == 0) || (data_com.prfl_in_use > MAX_PROFILE)) {
        return;
    }
    
    u8_val = data_pro[data_com.prfl_in_use-1].light_color_mode;
    
    // For now, increment mode
    u8_val = u8_val + 1;
    // 1: Solid, 2: Gradient
    if (u8_val > 2) {
        u8_val = 1;
    }

    // Convert uint8_t value to string
    err = utils_uint8_to_str(val_str, u8_val); 
    if (err < 0) {
        return;
    }

    // Add key/val to INFO arrays
    err = json_add_i_key_val(json_key_Light_Color_Mode, strlen(json_key_Light_Color_Mode), val_str, strlen(val_str));
}
// .............................................................................

// %%%%%%%%%%%%%%%%%%%%%%
// Lighting Solid Colour
// %%%%%%%%%%%%%%%%%%%%%%

// .............................................................................
int json_fn_w_light_solid_colour(uint8_t p_indx, char *val_str)
{
    int err = 0;
    uint32_t u32_val;
    
    // Convert value string to uint8_t
    err = utils_str_to_uint32(val_str, &u32_val);
    if (err < 0) {
        return -1;
    }

    if (u32_val > 0xFFFFFF) {
        return -2;
    }
    
    if (data_pro[p_indx-1].light_solid_color == u32_val) {
        // Not an error, but nothing to do
        return 0;
    }
            
    data_pro[p_indx-1].light_solid_color = u32_val;
    
    return 0;
}
// .............................................................................
int json_fn_r_light_solid_colour(uint8_t p_indx, char *val_str)
{
    int err = 0;
    uint32_t u32_val;

    u32_val = data_pro[p_indx-1].light_solid_color;

    // Convert uint32_t value to string
    err = utils_uint32_to_str(val_str, u32_val); 

    if (err < 0) {
        return -1;
    }
       
    return 0;
}
// .............................................................................
void json_fn_i_light_solid_colour()
{
    int err = 0;
    uint32_t u32_val;
    char val_str[JSON_DATA_VAL_STR_MAX_LEN];
    
    if ((data_com.prfl_in_use == 0) || (data_com.prfl_in_use > MAX_PROFILE)) {
        return;
    }
    
    u32_val = data_pro[data_com.prfl_in_use-1].light_solid_color;
    
    // For now, increment colur by a bit...
    u32_val = u32_val + 10000;
    // 1: Solid, 2: Gradient
    if (u32_val > 0xFFFFFF) {
        u32_val -= 0xFFFFFF;
    }

    // Convert uint32_t value to string
    err = utils_uint32_to_str(val_str, u32_val); 
    if (err < 0) {
        return;
    }

    // Add key/val to INFO arrays
    err = json_add_i_key_val(json_key_Light_Solid_Color, strlen(json_key_Light_Solid_Color), val_str, strlen(val_str));
}
// .............................................................................

// %%%%%%%%%%%%%%%%%%%%%%
// Lighting Gradient Colour
// %%%%%%%%%%%%%%%%%%%%%%

// .............................................................................
int json_fn_w_light_grad_colour(uint8_t p_indx, char *val_str)
{
    int err = 0;
    uint8_t u8_val;
    
    // Convert value string to uint8_t
    err = utils_str_to_uint8(val_str, &u8_val);
    if (err < 0) {
        return -1;
    }

    // 1: Glorious, 2: OG, 3: Pink G, 4: Nebula
    if ((u8_val < 1) || (u8_val > 4)) {
        return -2;
    }
    
    if (data_pro[p_indx-1].light_grad_color == u8_val) {
        // Not an error, but nothing to do
        return 0;
    }
            
    data_pro[p_indx-1].light_grad_color = u8_val;
    
    return 0;
}
// .............................................................................
int json_fn_r_light_grad_colour(uint8_t p_indx, char *val_str)
{
    int err = 0;
    uint8_t u8_val;

    u8_val = data_pro[p_indx-1].light_grad_color;

    // Convert uint8_t value to string
    err = utils_uint8_to_str(val_str, u8_val); 

    if (err < 0) {
        return -1;
    }
       
    return 0;
}
// .............................................................................
void json_fn_i_light_grad_colour()
{
    int err = 0;
    uint8_t u8_val;
    char val_str[JSON_DATA_VAL_STR_MAX_LEN];
    
    if ((data_com.prfl_in_use == 0) || (data_com.prfl_in_use > MAX_PROFILE)) {
        return;
    }
    
    u8_val = data_pro[data_com.prfl_in_use-1].light_grad_color;
    
    // For now, increment mode
    u8_val = u8_val + 1;
    // 1: Glorious, 2: OG, 3: Pink G, 4: Nebula
    if (u8_val > 4) {
        u8_val = 1;
    }

    // Convert uint8_t value to string
    err = utils_uint8_to_str(val_str, u8_val); 
    if (err < 0) {
        return;
    }

    // Add key/val to INFO arrays
    err = json_add_i_key_val(json_key_Light_Gradient_Color, strlen(json_key_Light_Gradient_Color), val_str, strlen(val_str));
}
// .............................................................................

// %%%%%%%%%%%%%%%%%%%%%%
// Lighting Rate
// %%%%%%%%%%%%%%%%%%%%%%

// .............................................................................
int json_fn_w_light_rate(uint8_t p_indx, char *val_str)
{
    int err = 0;
    uint8_t u8_val;
    
    // Convert value string to uint8_t
    err = utils_str_to_uint8(val_str, &u8_val);
    if (err < 0) {
        return -1;
    }
            
    data_pro[p_indx-1].light_rate = u8_val;
    
    return 0;
}
// .............................................................................
int json_fn_r_light_rate(uint8_t p_indx, char *val_str)
{
    int err = 0;
    uint8_t u8_val;

    u8_val = data_pro[p_indx-1].light_rate;

    // Convert uint8_t value to string
    err = utils_uint8_to_str(val_str, u8_val); 

    if (err < 0) {
        return -1;
    }
       
    return 0;
}
// .............................................................................
void json_fn_i_light_rate()
{
    int err = 0;
    uint8_t u8_val;
    char val_str[JSON_DATA_VAL_STR_MAX_LEN];
    
    if ((data_com.prfl_in_use == 0) || (data_com.prfl_in_use > MAX_PROFILE)) {
        return;
    }
    
    u8_val = data_pro[data_com.prfl_in_use-1].light_rate;

    // Convert uint8_t value to string
    err = utils_uint8_to_str(val_str, u8_val); 
    if (err < 0) {
        return;
    }

    // Add key/val to INFO arrays
    err = json_add_i_key_val(json_key_Light_Rate, strlen(json_key_Light_Rate), val_str, strlen(val_str));
}
// .............................................................................

// %%%%%%%%%%%%%%%%%%%%%%
// Lighting Brightness
// %%%%%%%%%%%%%%%%%%%%%%

// .............................................................................
int json_fn_w_light_bright(uint8_t p_indx, char *val_str)
{
    int err = 0;
    uint8_t u8_val;
    
    // Convert value string to uint8_t
    err = utils_str_to_uint8(val_str, &u8_val);
    if (err < 0) {
        return -1;
    }
            
    data_pro[p_indx-1].light_bright = u8_val;
    
    return 0;
}
// .............................................................................
int json_fn_r_light_bright(uint8_t p_indx, char *val_str)
{
    int err = 0;
    uint8_t u8_val;

    u8_val = data_pro[p_indx-1].light_bright;

    // Convert uint8_t value to string
    err = utils_uint8_to_str(val_str, u8_val); 

    if (err < 0) {
        return -1;
    }
       
    return 0;
}
// .............................................................................
void json_fn_i_light_bright()
{
    int err = 0;
    uint8_t u8_val;
    char val_str[JSON_DATA_VAL_STR_MAX_LEN];
    
    if ((data_com.prfl_in_use == 0) || (data_com.prfl_in_use > MAX_PROFILE)) {
        return;
    }
    
    u8_val = data_pro[data_com.prfl_in_use-1].light_bright;

    // Convert uint8_t value to string
    err = utils_uint8_to_str(val_str, u8_val); 
    if (err < 0) {
        return;
    }

    // Add key/val to INFO arrays
    err = json_add_i_key_val(json_key_Light_Bright, strlen(json_key_Light_Bright), val_str, strlen(val_str));
}
// .............................................................................

// %%%%%%%%%%%%%%%%%%%%%%
// Lighting Auto Off
// %%%%%%%%%%%%%%%%%%%%%%

// .............................................................................
int json_fn_w_light_auto_off(uint8_t p_indx, char *val_str)
{
    int err = 0;
    bool b_val;
    
    // Convert value string to bool
    err = utils_on_off_str_to_bool(val_str, &b_val);
    if (err < 0) {
        return -1;
    }
            
    data_pro[p_indx-1].light_auto_off = b_val;  
    
    return 0;
}
// .............................................................................
int json_fn_r_light_auto_off(uint8_t p_indx, char *val_str)
{
    int err = 0;
    bool b_val;

    b_val = data_pro[p_indx-1].light_auto_off;

    // Convert uint8_t value to string
    err = utils_bool_to_on_off_str(val_str, b_val); 
    
    if (err < 0) {
        return -1;
    }
       
    return 0;
}
// .............................................................................
void json_fn_i_light_auto_off()
{
    int err = 0;
    bool b_val;
    char val_str[JSON_DATA_VAL_STR_MAX_LEN];
    
    if ((data_com.prfl_in_use == 0) || (data_com.prfl_in_use > MAX_PROFILE)) {
        return;
    }
    
    // Toggle display on status
    b_val = !data_pro[data_com.prfl_in_use-1].light_auto_off;
    data_pro[data_com.prfl_in_use-1].light_auto_off = b_val;
    
    // Convert uint8_t value to string
    err = utils_bool_to_on_off_str(val_str, b_val); 
    if (err < 0) {
        return;
    }
    // Add key/val to INFO arrays
    err = json_add_i_key_val(json_key_Light_Auto_Off, strlen(json_key_Light_Auto_Off), val_str, strlen(val_str));
            
}
// .............................................................................

// %%%%%%%%%%%%%%%%%%%%%%
// Equaliser Preset Enabled
// %%%%%%%%%%%%%%%%%%%%%%

// .............................................................................
// Inform DSP of updated Enabled status
// .............................................................................
int json_fn_a_eq_preset_en(uint8_t p_indx)
{
    int err = 0;
    bool b_val;
    char val_str[JSON_DATA_VAL_STR_MAX_LEN];
    
    b_val = data_pro[p_indx-1].eq_pre_enabled;

    // Convert bool value to string
    err = utils_bool_to_str(val_str, b_val); 
    if (err < 0) {
        return -1;
    }
            
    // TODO: Send message to DSP
    
    return 0;
}
// .............................................................................
// Enable/Disable EQ Presets for this Profile
// Logic:
// If Enable
// - if profile in use - send EQ values, then send enable
// - - but first make sure that this Profile's EQ Preset is defined
// - - (if not, find first defined preset to use)
// - if not profile in use, just do it
// If Disable:
// - if profile in use - send disable to DSP
// - if not profile in use, just do it
// .............................................................................
int json_fn_w_eq_preset_en(uint8_t p_indx, char *val_str)
{
    int err = 0;
    bool b_val;
    uint8_t p_indx_in_use;
    uint8_t eq_in_use;
    uint8_t eq_id;
    
    // Convert value string to bool
    err = utils_str_to_bool(val_str, &b_val);
    if (err < 0) {
        return -1;
    }
    
    // Note: p_indx is used for Profile Index (1..n)
    if ((p_indx == 0)|| (p_indx > MAX_PROFILE)) {
        return -2;
    }
            
    p_indx_in_use = data_com.prfl_in_use;
    
    // Update value
    data_pro[p_indx-1].eq_pre_enabled = b_val;  

    if (b_val) {
        // Enable
        if (p_indx == p_indx_in_use) {
            // This profile IS the profile in use
            eq_in_use = data_pro[p_indx-1].eq_pre_in_use;
            if (data_com.eq_pre_data[eq_in_use].defined) {
                // The specified EQ Preset is defined
            } else {
                // The specified EQ Preset is NOT defined
                // FInd the first available:
 //               err = json_eq_presets_first_defined(&eq_id);
                if (err < 0) {
                    // No EQ Preset defined!
                    return -3;
                }
                // Update this Profile's EQ Preset
                data_pro[p_indx-1].eq_pre_in_use = eq_id;
            }
            // Notify DSP of (a) EQ Values then (b) Enable status
            err = json_fn_a_eq_preset_vals(p_indx);
            if (err < 0) {
                return -3;
            }
            err = json_fn_a_eq_preset_en(p_indx);
            if (err < 0) {
                return -4;
            }
        } else {
            // This profile is NOT the profile in use - nothing to do
        }
    } else {
        // Disable
        if (p_indx == p_indx_in_use) {
            // This profile IS the profile in use
            // Notify DSP of (a) Enable status
            err = json_fn_a_eq_preset_en(p_indx);
            if (err < 0) {
                return -5;
            }
        } else {
            // This profile is NOT the profile in use - nothing to do
        }
    }
    
    return 0;
}
// .............................................................................
int json_fn_r_eq_preset_en(uint8_t p_indx, char *val_str)
{
    int err = 0;
    bool b_val;

    b_val = data_pro[p_indx-1].eq_pre_enabled;

    // Convert bool value to string
    err = utils_bool_to_str(val_str, b_val); 
    
    if (err < 0) {
        return -1;
    }
       
    return 0;
}
// .............................................................................
int json_fn_i_eq_preset_en(uint8_t p_indx)
{
    int err = 0;
    bool b_val;
    char val_str[JSON_DATA_VAL_STR_MAX_LEN];

    if ((data_com.prfl_in_use == 0) || (data_com.prfl_in_use > MAX_PROFILE)) {
        return -1;
    }

    b_val = data_pro[p_indx-1].eq_pre_enabled;

    // Convert bool value to string
    err = utils_bool_to_str(val_str, b_val); 
    if (err < 0) {
        return -1;
    }
       
    // Add key/val to INFO arrays
    err = json_add_i_key_val(json_key_Eq_Preset_En, strlen(json_key_Eq_Preset_En), val_str, strlen(val_str));
    if (err < 0) {
        return -2;
    }

    return 0;
}
// .............................................................................

// .............................................................................
// %%%%%%%%%%%%%%%%%%%%%%
// Equaliser Preset In Use
// %%%%%%%%%%%%%%%%%%%%%%

// .............................................................................
// Modifying the EQ Preset in use for this Profile
// Logic:
// If Presets Enabled for this Profile
// - go ahead and assign new Preset
// - - but first make sure that the specified EQ Preset is defined
// - - (if not, error)
// - if preset is defined and now assigned:
// - - if this Profile is not the profile in use, nothing to do
// - - if this Profile IS the profile in use, notify DSP of new values
// If Presets Disabled for this Profile
// - go ahead and assign new Preset
// - - but first make sure that the specified EQ Preset is defined
// - - (if not, error)
// .............................................................................
int json_fn_w_eq_preset_in_use(uint8_t p_indx, char *val_str)
{
    int err = 0;
    uint8_t u8_val;
    uint8_t eq_in_use;
    uint8_t p_in_use;
    
    // Convert value string to uint8_t
    err = utils_str_to_uint8(val_str, &u8_val);
    if (err < 0) {
        return -1;
    }

    if ((u8_val < 1) || (u8_val > MAX_EQ_PRESETS)) {
        // Invalid EQ Preset index
        return -2;
    }
    
    eq_in_use = data_pro[p_indx-1].eq_pre_in_use;
    
    if (eq_in_use == u8_val) {
        // Not an error, but nothing to do
        return 0;
    }
    
    p_in_use = data_com.prfl_in_use;
    
    if (data_pro[p_indx-1].eq_pre_enabled) {
        // Presets Enabled for this Profile
        if (data_com.eq_pre_data[u8_val-1].defined) {
            // The selected new EQ Preset is defined
            // Assign it
            data_pro[p_indx-1].eq_pre_in_use = u8_val;
            if (p_indx == p_in_use) {
                // THis profile is the Profile In Use
                // Notify DSP of EQ Values
                err = json_fn_a_eq_preset_vals(p_indx);
                if (err < 0) {
                    return -3;
                }
            } else {
                // THis profile is NOT the Profile In Use - nothing else to do
            }
        } else {
            // The selected new EQ Preset is NOT defined
            return -4;
        }
    } else {
        // Presets Disabled for this Profile
        if (data_com.eq_pre_data[u8_val-1].defined) {
            // The selected new EQ Preset is defined
            // Assign it, then nothing else to do
            data_pro[p_indx-1].eq_pre_in_use = u8_val;
        } else {
            // The selected new EQ Preset is NOT defined
            return -5;
        }
    }
    
    return 0;
}
// .............................................................................
int json_fn_r_eq_preset_in_use(uint8_t p_indx, char *val_str)
{
    int err = 0;
    uint8_t u8_val;

    u8_val = data_pro[p_indx-1].eq_pre_in_use;

    // Convert uint8_t value to string
    err = utils_uint8_to_str(val_str, u8_val); 

    if (err < 0) {
        return -1;
    }
       
    return 0;
}
// .............................................................................
void json_fn_i_eq_preset_in_use()
{
    int err = 0;
    uint8_t u8_val;
    char val_str[JSON_DATA_VAL_STR_MAX_LEN];
    
    if ((data_com.prfl_in_use == 0) || (data_com.prfl_in_use > MAX_PROFILE)) {
        return;
    }
    
    u8_val = data_pro[data_com.prfl_in_use-1].eq_pre_in_use;
    
    // For now, increment mode
    u8_val = u8_val + 1;
    // There are 6 presets
    if (u8_val > 6) {
        u8_val = 1;
    }

    // Convert uint8_t value to string
    err = utils_uint8_to_str(val_str, u8_val); 
    if (err < 0) {
        return;
    }

    // Add key/val to INFO arrays
    err = json_add_i_key_val(json_key_Eq_Preset_In_Use, strlen(json_key_Eq_Preset_In_Use), val_str, strlen(val_str));
}
// .............................................................................


// **********************************************************************************************************************************************************
// **********************************************************************************************************************************************************
// Commands
// **********************************************************************************************************************************************************
// **********************************************************************************************************************************************************

// .............................................................................

// %%%%%%%%%%%%%%%%%%%%%%
// Profile - Read
// %%%%%%%%%%%%%%%%%%%%%%

// .............................................................................
// Generate the data required to build a response string for a JSON Profile Read.
// To do this, we add each key, associated with Profile data, to the following
// array, data_rx_key_strs[], and then trigger a a JSON read. The JSON read will
// use all these stored keys, to append all the keys and their associated 
// values, to a new JSON message for sending back to the Core app.
// Inputs:
//  p_indx:     0        Read the Common Dataset
//              1..n     Read Profile n
//  val_str:    not used
// .............................................................................
int json_fn_c_profile_read(uint8_t p_indx, char *val_str)
{
    int err = 0;
    uint8_t count;
    
    if (p_indx > MAX_PROFILE) {
        // Profile Index out of range
        json_fn_send_status_error(ERR_JSON_PROFILE_INDX);
        return -1;
    }
    
    // Clear the RX Key arrays. Below, will add ech rcvd key to this array
    key_val_rx_arr_indx_init(); 
    
    count = 0;
    
    if (p_indx == 0) {
        // Common Dataset
        err = json_add_rx_key(json_key_Mode_Id, strlen(json_key_Mode_Id));
        count++;
        err = json_add_rx_key(json_key_Firmware_Ver, strlen(json_key_Firmware_Ver));
        count++;
        err = json_add_rx_key(json_key_Serial_Num, strlen(json_key_Serial_Num));
        count++;
        err = json_add_rx_key(json_key_Com_State, strlen(json_key_Com_State));
        count++;
        err = json_add_rx_key(json_key_Blue_Enabled, strlen(json_key_Blue_Enabled));
        count++;
        err = json_add_rx_key(json_key_Audio_Vol, strlen(json_key_Audio_Vol));
        count++;
        err = json_add_rx_key(json_key_Batt_Level, strlen(json_key_Batt_Level));
        count++;
        err = json_add_rx_key(json_key_Batt_Min, strlen(json_key_Batt_Min));
        count++;
        err = json_add_rx_key(json_key_Cues_Enabled, strlen(json_key_Cues_Enabled));
        count++;
        err = json_add_rx_key(json_key_Charge_Status, strlen(json_key_Charge_Status));
        count++;
        err = json_add_rx_key(json_key_Signal_Level, strlen(json_key_Signal_Level));
        count++;
        err = json_add_rx_key(json_key_Audio_Pwr_Off_Time, strlen(json_key_Audio_Pwr_Off_Time));
        count++;
        err = json_add_rx_key(json_key_Audio_Pwr_Off_Enabled, strlen(json_key_Audio_Pwr_Off_Enabled));
        count++;
        err = json_add_rx_key(json_key_Disp_Menu_Time, strlen(json_key_Disp_Menu_Time));
        count++;
        err = json_add_rx_key(json_key_Mic_Enabled, strlen(json_key_Mic_Enabled));
        count++;
        err = json_add_rx_key(json_key_Mic_Connected, strlen(json_key_Mic_Connected));
        count++;

        err = json_add_rx_key(json_key_Eq_Presets_X, strlen(json_key_Eq_Presets_X));
        count++;

        err = json_add_rx_key(json_key_Prof_In_Use, strlen(json_key_Prof_In_Use));
        count++;
        err = json_add_rx_key(json_key_Prof_Count, strlen(json_key_Prof_Count));
        count++;            
        err = json_add_rx_key(json_key_Prof_Names, strlen(json_key_Prof_Names));
        count++;            
    } else {
        // Profiles
        err = json_add_rx_key(json_key_Prof_Enabled, strlen(json_key_Prof_Enabled));
        count++;
        err = json_add_rx_key(json_key_Prof_Name, strlen(json_key_Prof_Name));
        count++;
        err = json_add_rx_key(json_key_Prof_State, strlen(json_key_Prof_State));
        count++;
        err = json_add_rx_key(json_key_Disp_On, strlen(json_key_Disp_On));
        count++;
        err = json_add_rx_key(json_key_Blue_Power_Up, strlen(json_key_Blue_Power_Up));
        count++;
        err = json_add_rx_key(json_key_Disp_Bright, strlen(json_key_Disp_Bright));
        count++;
        err = json_add_rx_key(json_key_Chat_Mix, strlen(json_key_Chat_Mix));
        count++;
        err = json_add_rx_key(json_key_Media_Mix, strlen(json_key_Media_Mix));
        count++;
        err = json_add_rx_key(json_key_Mic_Volume, strlen(json_key_Mic_Volume));
        count++;
        err = json_add_rx_key(json_key_Mic_Side_Tone, strlen(json_key_Mic_Side_Tone));
        count++;
        err = json_add_rx_key(json_key_Mic_Noise_Gate, strlen(json_key_Mic_Noise_Gate));
        count++;
        err = json_add_rx_key(json_key_Mic_Noise_Reduction, strlen(json_key_Mic_Noise_Reduction));
        count++;
        err = json_add_rx_key(json_key_Mic_Vol_Norm, strlen(json_key_Mic_Vol_Norm));
        count++;
        err = json_add_rx_key(json_key_Mic_Led_Bright, strlen(json_key_Mic_Led_Bright));
        count++;
        err = json_add_rx_key(json_key_ANC_Mode, strlen(json_key_ANC_Mode));
        count++;
        err = json_add_rx_key(json_key_Eq_Preset_En, strlen(json_key_Eq_Preset_En));
        count++;
        err = json_add_rx_key(json_key_Audio_Surround, strlen(json_key_Audio_Surround));
        count++;
        err = json_add_rx_key(json_key_Light_Effect, strlen(json_key_Light_Effect));
        count++;
        err = json_add_rx_key(json_key_Light_En, strlen(json_key_Light_En));
        count++;
        err = json_add_rx_key(json_key_Light_Color_Mode, strlen(json_key_Light_Color_Mode));
        count++;
        err = json_add_rx_key(json_key_Light_Solid_Color, strlen(json_key_Light_Solid_Color));
        count++;
        err = json_add_rx_key(json_key_Light_Gradient_Color, strlen(json_key_Light_Gradient_Color));
        count++;
        err = json_add_rx_key(json_key_Light_Rate, strlen(json_key_Light_Rate));
        count++;
        err = json_add_rx_key(json_key_Light_Bright, strlen(json_key_Light_Bright));
        count++;
        err = json_add_rx_key(json_key_Light_Auto_Off, strlen(json_key_Light_Auto_Off));
        count++;
        err = json_add_rx_key(json_key_Eq_Preset_In_Use, strlen(json_key_Eq_Preset_In_Use));
        count++;

    }

    // Force a read for all the keys we just added to the array
    err = json_func_process_read(p_indx, count);
        
    if (err < 0) {
        json_fn_send_status_error(ERR_JSON_PROFILE_READ);
    }
        
    return 0;
}
// .............................................................................

// %%%%%%%%%%%%%%%%%%%%%%
// Profile - Restore (to factory defaults))
// %%%%%%%%%%%%%%%%%%%%%%

// .............................................................................
// Inputs:
//  p_indx:     0        Restore all profiles (and common dataset)
//              1..n     Restore specified Profile
//  val_str:    not used
// .............................................................................
int json_fn_c_profile_restore(uint8_t p_indx, char *val_str)
{
    int err = 0;
    
    if (p_indx > MAX_PROFILE) {
        // Profile Index out of range
        json_fn_send_status_error(ERR_JSON_PROFILE_INDX);
        return -1;
    }
    
    if (p_indx == 0) {
        // Restore all data to factory settings
        json_data_init();
    } else {
        // Restore selected profile data to factory settings
        json_data_init_profile(p_indx);
    }

    // Create string which contains Profile ID and Name for every enabled Profile
    err = json_build_prof_names_str();

    if (err < 0) {
        json_fn_send_status_error(ERR_JSON_PROFILE_NAMES_RESTORE);
        return -2;
    }
    // Send status response, which includes list of enabled Profiles (id and name))
    json_fn_send_status_success();
    
    // After sending the success response, generate an info message so that the
    // Core app knows which profile is the current Profile In Use
    k_sleep(K_MSEC(10));
    json_fn_i_prof_names();
    
    return 0;
}
    
// .............................................................................

// %%%%%%%%%%%%%%%%%%%%%%
// Profile - Create
// %%%%%%%%%%%%%%%%%%%%%%

// .............................................................................
// Create a new profile, with default data, and make this profile the profile
// in use
// Only do this if there is at least one available (disabled) profile)
// Inputs:
//  p_indx:     not used
//  val_str:    Profile Name for the new profile
// .............................................................................
int json_fn_c_profile_create(uint8_t p_indx, char *val_str)
{
    int len = 0;
    int err = 0;
    uint8_t p_id_new;
    uint8_t p_id_previous;
    
    // Ignore profile index
    
    len = strlen(val_str);
    
    if ((len == 0) || (len > JSON_DATA_VAL_STR_MAX_LEN)) {
        json_fn_send_status_error(ERR_JSON_CMD_VAL);
        return -1;
    }

    // See if there is a unused/disabled profile that we can use
    p_id_new = json_profiles_get_disabled();
    
    if (p_id_new == 0) {
        // All profiles are already in use
        json_fn_send_status_error(ERR_JSON_NO_FREE_PROFILE);
        return -2;
    }

    // Create new profile (1..n))
    json_data_init_profile(p_id_new);
    
    // New profile is enabled
    data_pro[p_id_new-1].pro_en = true;
    // Update new profile's name
    strcpy(data_pro[p_id_new-1].prfl_name, val_str);
    // Remember which profile is the current Profile In Use
    p_id_previous = data_com.prfl_in_use;
    // Now set this new profile to the Profile In Use
    data_com.prfl_in_use = p_id_new;
    
    // TODO: Compare the two profiles (previous and current Profile In Use)
    // and take appropriate action for any parameters that have changed
    
    // Finished all work
    // Create string which contains Profile ID and Name for every enabled Profile
    err = json_build_prof_names_str();

    if (err < 0) {
        json_fn_send_status_error(ERR_JSON_PROFILE_NAMES_CREATE);
        return -3;
    }
    // Send status response, which includes list of enabled Profiles (id and name))
    json_fn_send_status_success();
    
    // After sending the success response, generate an info message so that the
    // Core app knows which profile is the current Profile In Use
    k_sleep(K_MSEC(10));
    json_fn_i_prof_names();

    return 0;
}

// .............................................................................

// %%%%%%%%%%%%%%%%%%%%%%
// Profile - Delete
// %%%%%%%%%%%%%%%%%%%%%%

// .............................................................................
// Delete the current Profile In Use
// If this is the only enabled profile, then create a new defaulted profile
// (Profile 1))
// Inputs:
//  p_indx:     not used
//  val_str:    not used
// .............................................................................
int json_fn_c_profile_delete(uint8_t p_indx, char *val_str)
{
    uint8_t p_id_previous;
    uint8_t p_id_new;
    int err = 0;
    
    p_id_previous = data_com.prfl_in_use;

    if ((p_id_previous == 0) || (p_id_previous > MAX_PROFILE)) {
        json_fn_send_status_error(ERR_JSON_PROFILE_INDX);
        return -1;
    }
    
    // Make sure Profile In Use is currently enabled
    if (data_pro[p_id_previous-1].pro_en == false) {
        json_fn_send_status_error(ERR_JSON_DELETE_PROFILE);
        return -2;
    }
    // Disable this profile
    data_pro[p_id_previous-1].pro_en = false;
    // Determine which profile will become the new profile
    p_id_new = json_profiles_get_enabled();

    if (p_id_new == 0) {
        // All profiles are already disabled
        // Use the first profile
        p_id_new = 1;
        // TODO: Before defaulting Profile 1, see if previous Profile In Use was
        // also Profile 1 - if it was, then have to keep a copy of Profile 1's
        // data before switching over
        
        // Create new profile (1..n))
        json_data_init_profile(p_id_new);    
        // Enable he new profile
        data_pro[p_id_new-1].pro_en = true;
        
    }
    // Update the Profile In Use
    data_com.prfl_in_use = p_id_new;
    
    // TODO: Compare previous profile's data with new profile's data, 
    // and take appropriate action for any parameters that have changed

    // Create string which contains Profile ID and Name for every enabled Profile
    err = json_build_prof_names_str();

    if (err < 0) {
        json_fn_send_status_error(ERR_JSON_PROFILE_NAMES_DELETE);
        return -3;
    }
    // Send status response, which includes list of enabled Profiles (id and name))
    json_fn_send_status_success();

    // After sending the success response, generate an info message so that the
    // Core app knows which profile is the current Profile In Use
    k_sleep(K_MSEC(10));
    json_fn_i_prof_names();
    
    return 0;
}

// .............................................................................

// %%%%%%%%%%%%%%%%%%%%%%
// Profile - Duplicate
// %%%%%%%%%%%%%%%%%%%%%%

// .............................................................................
// Duplicate the current Profile In Use
// Copy the Profile Name (input parameter) to this new profile
// Make the newly created Profile the new Profile In Use
// Inputs:
//  p_indx:     not used
//  val_str:    Profile Name
// .............................................................................
int json_fn_c_profile_duplicate(uint8_t p_indx, char *val_str)
{
    int len = 0;
    int err = 0;
    uint8_t p_id_new;
    uint8_t p_id_previous;
    
    // Ignore profile index
    
    p_id_previous = data_com.prfl_in_use;

    len = strlen(val_str);
    
    if ((len == 0) || (len > JSON_DATA_VAL_STR_MAX_LEN)) {
        json_fn_send_status_error(ERR_JSON_CMD_VAL);
        return -1;
    }

    // Make sure Profile In Use is currently enabled
    if (data_pro[p_id_previous-1].pro_en == false) {
        json_fn_send_status_error(ERR_JSON_DUPLICATE_PROFILE);
        return -2;
    }

    // See if there is a unused/disabled profile that we can use
    p_id_new = json_profiles_get_disabled();
    
    if (p_id_new == 0) {
        // All profiles are already in use
        json_fn_send_status_error(ERR_JSON_NO_FREE_PROFILE);
        return -3;
    }

    // Create new profile (1..n))
    json_profiles_duplicate_profie(p_id_new);
    
    // New profile is already enabled because old profile was enabled

    // Update new profile's name
    strcpy(data_pro[p_id_new-1].prfl_name, val_str);
    // Remember which profile is the current Profile In Use
    p_id_previous = data_com.prfl_in_use;
    // Now set this new profile to the Profile In Use
    data_com.prfl_in_use = p_id_new;
    
    // TODO: Compare the two profiles (previous and current Profile In Use)
    // and take appropriate action for any parameters that have changed
    
    // Finished all work
    // Create string which contains Profile ID and Name for every enabled Profile
    err = json_build_prof_names_str();

    if (err < 0) {
        json_fn_send_status_error(ERR_JSON_PROFILE_NAMES_DUPLICATE);
        return -4;
    }
    // Send status response, which includes list of enabled Profiles (id and name))
    json_fn_send_status_success();
    
    // After sending the success response, generate an info message so that the
    // Core app knows which profile is the current Profile In Use
    k_sleep(K_MSEC(10));
    json_fn_i_prof_names();

    return 0;}

// .............................................................................

// %%%%%%%%%%%%%%%%%%%%%%
// DFU - Commit File
// %%%%%%%%%%%%%%%%%%%%%%

// .............................................................................
// Command from Core to commit the DFU file
// i.e. Core has transferred a .bin file to MCU. This file should now be in 
// external Flash. This command is instructing the MCU to now go ahead and write
// this file to Program Memory.
// MCU should 
//  (a) check that it has a valid file in external flash, 
//  (b) perform any necessary final tasks
//  (c) send response to Core
//  (d) delay a short while, to allow response to be sent
//  (e) write bootloader signature to RAM, and perform soft reset, to enable 
//      bootloader to update program memory
//  p_indx:     not used
//  val_str:    not used
// .............................................................................
int json_fn_c_dfu_commit(uint8_t p_indx, char *val_str)
{
    // Ignore profile index
    
    //  (a) check that it has a valid file in external flash, 
    if (is_dfu_file_ok() == false)
    {
        // We don't currently have a verified .bin file in external Flash
        json_fn_send_status_error(ERR_DFU_NO_FILE);
        return -1;
    }
    
    //  (b) perform any necessary final tasks

    //  (c) send response to Core
    json_fn_send_status_success();

    //  (d) delay a short while, to allow response to be sent
    osDelayMs(1000);
    
    //  (e) write bootloader signature to RAM, and perform soft reset, to enable 
    //      bootloader to update program memory
    dfu_bootloader_trigger();

    return 0;}

// .............................................................................


// **********************************************************************************************************************************************************
// **********************************************************************************************************************************************************
// **********************************************************************************************************************************************************



// .............................................................................
