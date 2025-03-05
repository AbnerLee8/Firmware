/******************************************************************************
  @File Name
    json_proc.c

  @Summary
    JSON Data Functions

  @Description

    
*******************************************************************************/
#include "json.h"
#include "json_data.h"
#include <ctype.h>

// .............................................................................

char data_rx_key_strs[JSON_DATA_KEYS_MAX][JSON_DATA_KEY_STR_MAX_LEN];       // Keys received in JSON read/write message
char data_rx_val_strs[JSON_DATA_KEYS_MAX][JSON_DATA_VAL_STR_MAX_LEN];       // Vals received in JSON write message
char data_tx_key_strs[JSON_DATA_KEYS_MAX][JSON_DATA_KEY_STR_MAX_LEN];       // Keys to send in JSON read response
char data_tx_val_strs[JSON_DATA_KEYS_MAX][JSON_DATA_VAL_STR_MAX_LEN];       // Vals to send in JSON read response
char data_i_key_strs[JSON_DATA_KEYS_MAX][JSON_DATA_KEY_STR_MAX_LEN];        // Keys to send in JSON INFO aysnch message
char data_i_val_strs[JSON_DATA_KEYS_MAX][JSON_DATA_VAL_STR_MAX_LEN];        // Vals to send in JSON INFO aysnch message
uint8_t data_rx_key_val_indx;                                               // Index that indicates how many keys/vals in rx array
uint8_t data_tx_key_val_indx;                                               // Index that indicates how many keys/vals in tx array
uint8_t data_i_key_val_indx;                                                // Index that indicates how many keys/vals in info array

// .............................................................................

// *****************************************************************************
// *****************************************************************************
// JSON key and key/val arrays
//
// When a JSON read or write message received, first clear the RX Key array.
// Then cycle through the JSON message and add each received key to the RX key
// array.
// For the READ, first initialise the TX Key array, then cycle though the RX key
// array, and call the associated read function of each key. Each time, we will
// add the resulting key/value pair to the TX Key array. Will eventually cycle 
// through the TX Key array to use the key/val pairs to build the Read Response
// JSON string.
// For the WRITE, will cycle though the keys and call the associated WRITE 
// function for each. ANd if all goes well, will eventually just build a generic
// Success Status Response string.
//
// NOTE: Need to look at the code that builds an INFO message (when e.g. encoder
// is rotated, or button pressed), as this code should also use these RX and TX
//Key arrays.
// *****************************************************************************
// *****************************************************************************

// .............................................................................
// Initialise RX Keys array
// .............................................................................
void key_val_rx_arr_indx_init()
{
    data_rx_key_val_indx = 0;
}
// .............................................................................
// Add received key/val strings to RX arrays
// .............................................................................
int json_add_rx_key_val(char *key_str, uint8_t key_len, char *val_str, uint8_t val_len)
{
    
    if ((key_len == 0) || (key_len > JSON_DATA_KEY_STR_MAX_LEN)) {
        return -1;
    }
    if ((val_len == 0) || (val_len > JSON_DATA_VAL_STR_MAX_LEN)) {
        return -2;
    }
    if (data_rx_key_val_indx > JSON_DATA_KEYS_MAX-1) {
        return -3;
    }
    strcpy(data_rx_key_strs[data_rx_key_val_indx],key_str);
    strcpy(data_rx_val_strs[data_rx_key_val_indx],val_str);
    data_rx_key_val_indx++;
    
    return 0;
}
// .............................................................................
// Add received key strings to RX array
// .............................................................................
int json_add_rx_key(char *key_str, uint8_t key_len)
{
    
    if ((key_len == 0) || (key_len > JSON_DATA_KEY_STR_MAX_LEN)) {
        return -1;
    }
    if (data_rx_key_val_indx > JSON_DATA_KEYS_MAX-1) {
        return -2;
    }
    strcpy(data_rx_key_strs[data_rx_key_val_indx],key_str);
    data_rx_key_val_indx++;
    
    return 0;
}

// .............................................................................
// Initialise TX Keys array
// .............................................................................
void key_val_tx_arr_indx_init()
{
    data_tx_key_val_indx = 0;
}
// .............................................................................
// Add key/val strings to TX arrays for transmission
// .............................................................................
int json_add_tx_key_val(char *key_str, uint8_t key_len, char *val_str, uint8_t val_len)
{
    
    if ((key_len == 0) || (key_len > JSON_DATA_KEY_STR_MAX_LEN)) {
        return -1;
    }
    if ((val_len == 0) || (val_len > JSON_DATA_VAL_STR_MAX_LEN)) {
        return -2;
    }
    if (data_tx_key_val_indx > JSON_DATA_KEYS_MAX-1) {
        return -3;
    }
    strcpy(data_tx_key_strs[data_tx_key_val_indx],key_str);
    strcpy(data_tx_val_strs[data_tx_key_val_indx],val_str);
    data_tx_key_val_indx++;
    
    return 0;
}
// .............................................................................
// Add key strings to TX array for transmission
// .............................................................................
int json_add_tx_key(char *key_str, uint8_t key_len)
{
    
    if ((key_len == 0) || (key_len > JSON_DATA_KEY_STR_MAX_LEN)) {
        return -1;
    }
    if (data_tx_key_val_indx > JSON_DATA_KEYS_MAX-1) {
        return -2;
    }
    strcpy(data_tx_key_strs[data_tx_key_val_indx],key_str);
    data_tx_key_val_indx++;
    
    return 0;
}

// .............................................................................
// Initialise INFO Keys array
// .............................................................................
void key_val_i_arr_indx_init()
{
    data_i_key_val_indx = 0;
}
// .............................................................................
// Read INFO Keys array index
// .............................................................................
uint8_t key_val_i_arr_indx_get()
{
    return (data_i_key_val_indx);
}
// .............................................................................
// Add received key/val strings to INFO arrays for transmission
// .............................................................................
int json_add_i_key_val(char *key_str, uint8_t key_len, char *val_str, uint8_t val_len)
{
    int i;
    bool b_already_updated = false;
    
    if ((key_len == 0) || (key_len > JSON_DATA_KEY_STR_MAX_LEN)) {
        return -1;
    }
    if ((val_len == 0) || (val_len > JSON_DATA_VAL_STR_MAX_LEN)) {
        return -2;
    }
    if (data_i_key_val_indx > JSON_DATA_KEYS_MAX-1) {
        return -3;
    }
    
    // If key already added to key array, overwrite/update its associated value
    if (data_i_key_val_indx > 0) {
        for (i = 0; i < data_i_key_val_indx; i++) {
            if (!strcmp(data_i_key_strs[i], key_str)) { 
                strcpy(data_i_val_strs[i],val_str);
                b_already_updated = true;
                break;
            }
        }
    }
    if (!b_already_updated) {
        strcpy(data_i_key_strs[data_i_key_val_indx],key_str);
        strcpy(data_i_val_strs[data_i_key_val_indx],val_str);
        data_i_key_val_indx++;
    }

    return 0;
}

// *****************************************************************************
// *****************************************************************************
// General functions
// *****************************************************************************
// *****************************************************************************

// .............................................................................
// Send JSON STATUS SUCCESS response to Core
// .............................................................................
void json_fn_send_status_success()
{
    Q_comm_jsn_t qs_comm_msg;
    uint8_t send_num;
    
    send_num = json_parse_send_num_get();
    
    qs_comm_msg.id = COMMS_JSON_SUCESS;
    qs_comm_msg.send_num = send_num;

    osQueueSendToBack(Q_comm_jsn, &qs_comm_msg, 10);
    
}
    
// .............................................................................
// Send JSON STATUS ERROR response to Core
// .............................................................................
void json_fn_send_status_error(uint8_t err_no)
{
    Q_comm_jsn_t qs_comm_msg;
    uint8_t send_num;
    
    send_num = json_parse_send_num_get();
    
    qs_comm_msg.id = COMMS_JSON_ERROR;
    qs_comm_msg.send_num = send_num;
    qs_comm_msg.err_no = err_no;

    osQueueSendToBack(Q_comm_jsn, &qs_comm_msg, 10);
    
}
// .............................................................................
// Send JSON READ SUCCESS response to Core
// .............................................................................

void json_fn_send_read_response()
{
    Q_comm_jsn_t qs_comm_msg;
    uint8_t send_num;
    
    send_num = json_parse_send_num_get();
    
    qs_comm_msg.id = COMMS_JSON_READ_RESP;
    qs_comm_msg.send_num = send_num;

    osQueueSendToBack(Q_comm_jsn, &qs_comm_msg, 10);
    
}

// .............................................................................
// Send JSON INFO message to Core
// .............................................................................

void json_fn_send_info_message()
{
    Q_comm_jsn_t qs_comm_msg;
  
    qs_comm_msg.id = COMMS_JSON_INFO;
    qs_comm_msg.send_num = 0;

    osQueueSendToBack(Q_comm_jsn, &qs_comm_msg, 10);
    
}

// .............................................................................
// Send JSON IMAGE status response to Core
// .............................................................................
void json_fn_send_status_image(uint16_t seg_num, uint16_t seg_tot)
{
    Q_comm_jsn_t qs_comm_msg;
    
    
    qs_comm_msg.id = COMMS_JSON_IMAGE;
    qs_comm_msg.seg_no = seg_num;
    qs_comm_msg.seg_tot = seg_tot;

    osQueueSendToBack(Q_comm_jsn, &qs_comm_msg, 10);
    
}
// .............................................................................


// *****************************************************************************
// THe following functions are called from the JSON Parser, to process received
// JSON Write / Read / Info messages.
// *****************************************************************************

// .............................................................................
// Process received JSON Write message.
// Have received a number of key/val pairs in JSON write, and have stored the 
// key/val strings in arrays: data_rx_key_strs{} and data_rx_val_strs[].
// Now, cycle through each key/value pair, process these, by calling the 
// associated write function for each key.
// If all goes well, send s status success JSON response.
// If any errors in processing, will just skip that individual key, but keep
// processing the remaining keys.
// Inputs: 
//  p_indx      Profile Index (0 = common, 1..n = profiles)
//  num_key     Indicates number of key/val strings received /stored in the array
// Outputs:
//  Will send a STATUS message:
//      SUCCESS
//      ERROR - ERR_JSON_SOME_GOOD_SOME_BAD
//      ERROR - ERR_JSON_NO_GOOD_SOME_BAD
// .............................................................................
int json_func_process_write(uint8_t p_indx, uint8_t num_key)
{
    int err = 0;
    int i;
    bool b_processed;
    uint8_t good_cnt = 0;
    uint8_t bad_cnt = 0;
    
    if (p_indx > MAX_PROFILE) {
        // Invalid profile index.
        return -1;
    }
    if (num_key > JSON_DATA_KEYS_MAX) {
        // Invalid number of keys.
        return -2;
    }
    
    // Cycle through each of the received keys (in data_rx_key_strs[])
    for (i = 0; i < num_key; i++) 
    {
        // assume success
        err = 0;
        b_processed = false;
        
        // COMMON
        
        if (!strcmp(data_rx_key_strs[i], json_key_Blue_Enabled)) {    
            err = json_fn_w_blue_enable(p_indx, data_rx_val_strs[i]);
            b_processed = true;
        }
        else if (!strcmp(data_rx_key_strs[i], json_key_Audio_Vol)) {    
            err = json_fn_w_audio_vol(p_indx, data_rx_val_strs[i]);
            b_processed = true;
        }
        else if (!strcmp(data_rx_key_strs[i], json_key_Batt_Level)) {    
            err = json_fn_w_batt_lvl(p_indx, data_rx_val_strs[i]);
            b_processed = true;
        }        
        else if (!strcmp(data_rx_key_strs[i], json_key_Batt_Min)) {    
            err = json_fn_w_batt_min(p_indx, data_rx_val_strs[i]);
            b_processed = true;
        }        
        else if (!strcmp(data_rx_key_strs[i], json_key_Cues_Enabled)) {    
            err = json_fn_w_audio_cue_enable(p_indx, data_rx_val_strs[i]);
            b_processed = true;
        }        
        else if (!strcmp(data_rx_key_strs[i], json_key_Audio_Pwr_Off_Time)) {    
            err = json_fn_w_audio_pwr_tim(p_indx, data_rx_val_strs[i]);
            b_processed = true;
        }        
        else if (!strcmp(data_rx_key_strs[i], json_key_Audio_Pwr_Off_Enabled)) {    
            err = json_fn_w_audio_pwr_en(p_indx, data_rx_val_strs[i]);
            b_processed = true;
        }        
        else if (!strcmp(data_rx_key_strs[i], json_key_Disp_Menu_Time)) {    
            err = json_fn_w_disp_menu_tim(p_indx, data_rx_val_strs[i]);
            b_processed = true;
        }        
        else if (!strcmp(data_rx_key_strs[i], json_key_Mic_Enabled)) {    
            err = json_fn_w_mic_enable(p_indx, data_rx_val_strs[i]);
            b_processed = true;
        }        
        else if (!strcmp(data_rx_key_strs[i], json_key_Mic_Connected)) {    
            err = json_fn_w_mic_enable(p_indx, data_rx_val_strs[i]);
            b_processed = true;
        }      
        
        else if (!strcmp(data_rx_key_strs[i], json_key_Eq_Presets_X)) {    
            err = json_fn_w_eq_presets_X(p_indx, data_rx_val_strs[i]);
            b_processed = true;
        }        

        else if (!strcmp(data_rx_key_strs[i], json_key_Dfu_File_Size)) {    
            err = json_fn_w_dfu_file_size(p_indx, data_rx_val_strs[i]);
            b_processed = true;
        }      
        else if (!strcmp(data_rx_key_strs[i], json_key_Dfu_File_Crc)) {    
            err = json_fn_w_dfu_file_crc(p_indx, data_rx_val_strs[i]);
            b_processed = true;
        }      
        
        else if (!strcmp(data_rx_key_strs[i], json_key_Prof_In_Use)) {    
            err = json_fn_w_prof_in_use(p_indx, data_rx_val_strs[i]);
            b_processed = true;
        }        

        // PROFILES

        else if (!strcmp(data_rx_key_strs[i], json_key_Prof_Enabled)) {    
            err = json_fn_w_prof_enabled(p_indx, data_rx_val_strs[i]);
            b_processed = true;
        }        
        else if (!strcmp(data_rx_key_strs[i], json_key_Prof_Name)) {    
            err = json_fn_w_prof_name(p_indx, data_rx_val_strs[i]);
            b_processed = true;
        }        
        else if (!strcmp(data_rx_key_strs[i], json_key_Disp_On)) {    
            err = json_fn_w_disp_on(p_indx, data_rx_val_strs[i]);
            b_processed = true;
        }
        else if (!strcmp(data_rx_key_strs[i], json_key_Blue_Power_Up)) {    
            err = json_fn_w_blue_pwr_up(p_indx, data_rx_val_strs[i]);
            b_processed = true;
        }
        else if (!strcmp(data_rx_key_strs[i], json_key_Disp_Bright)) {    
            err = json_fn_w_disp_bright(p_indx, data_rx_val_strs[i]);
            b_processed = true;
        }
        else if (!strcmp(data_rx_key_strs[i], json_key_Chat_Mix)) {    
            err = json_fn_w_chat_mix(p_indx, data_rx_val_strs[i]);
            b_processed = true;
        }
        else if (!strcmp(data_rx_key_strs[i], json_key_Media_Mix)) {    
            err = json_fn_w_media_mix(p_indx, data_rx_val_strs[i]);
            b_processed = true;
        }
        else if (!strcmp(data_rx_key_strs[i], json_key_Mic_Volume)) {    
            err = json_fn_w_mic_vol(p_indx, data_rx_val_strs[i]);
            b_processed = true;
        }
        else if (!strcmp(data_rx_key_strs[i], json_key_Mic_Side_Tone)) {    
            err = json_fn_w_mic_sidetone(p_indx, data_rx_val_strs[i]);
            b_processed = true;
        }
        else if (!strcmp(data_rx_key_strs[i], json_key_Mic_Noise_Gate)) {    
            err = json_fn_w_mic_noisegate(p_indx, data_rx_val_strs[i]);
            b_processed = true;
        }
        else if (!strcmp(data_rx_key_strs[i], json_key_Mic_Noise_Reduction)) {    
            err = json_fn_w_mic_noise_red(p_indx, data_rx_val_strs[i]);
            b_processed = true;
        }
        else if (!strcmp(data_rx_key_strs[i], json_key_Mic_Vol_Norm)) {    
            err = json_fn_w_mic_vol_norm(p_indx, data_rx_val_strs[i]);
            b_processed = true;
        }
        else if (!strcmp(data_rx_key_strs[i], json_key_Mic_Led_Bright)) {    
            err = json_fn_w_mic_led_bright(p_indx, data_rx_val_strs[i]);
            b_processed = true;
        }
        else if (!strcmp(data_rx_key_strs[i], json_key_ANC_Mode)) {    
            err = json_fn_w_anc_mode(p_indx, data_rx_val_strs[i]);
            b_processed = true;
        }
        else if (!strcmp(data_rx_key_strs[i], json_key_Eq_Preset_En)) {    
            err = json_fn_w_eq_preset_en(p_indx, data_rx_val_strs[i]);
            b_processed = true;
        }
        else if (!strcmp(data_rx_key_strs[i], json_key_Audio_Surround)) {    
            err = json_fn_w_audio_surround(p_indx, data_rx_val_strs[i]);
            b_processed = true;
        }
        else if (!strcmp(data_rx_key_strs[i], json_key_Light_Effect)) {    
            err = json_fn_w_light_effect(p_indx, data_rx_val_strs[i]);
            b_processed = true;
        }
        else if (!strcmp(data_rx_key_strs[i], json_key_Light_En)) {    
            err = json_fn_w_light_en(p_indx, data_rx_val_strs[i]);
            b_processed = true;
        }
        else if (!strcmp(data_rx_key_strs[i], json_key_Light_Color_Mode)) {    
            err = json_fn_w_light_colour_mode(p_indx, data_rx_val_strs[i]);
            b_processed = true;
        }
        else if (!strcmp(data_rx_key_strs[i], json_key_Light_Solid_Color)) {    
            err = json_fn_w_light_solid_colour(p_indx, data_rx_val_strs[i]);
            b_processed = true;
        }
        else if (!strcmp(data_rx_key_strs[i], json_key_Light_Gradient_Color)) {    
            err = json_fn_w_light_grad_colour(p_indx, data_rx_val_strs[i]);
            b_processed = true;
        }
        else if (!strcmp(data_rx_key_strs[i], json_key_Light_Rate)) {    
            err = json_fn_w_light_rate(p_indx, data_rx_val_strs[i]);
            b_processed = true;
        }
        else if (!strcmp(data_rx_key_strs[i], json_key_Light_Bright)) {    
            err = json_fn_w_light_bright(p_indx, data_rx_val_strs[i]);
            b_processed = true;
        }
        else if (!strcmp(data_rx_key_strs[i], json_key_Light_Auto_Off)) {    
            err = json_fn_w_light_auto_off(p_indx, data_rx_val_strs[i]);
            b_processed = true;
        }
        else if (!strcmp(data_rx_key_strs[i], json_key_Eq_Preset_In_Use)) {    
            err = json_fn_w_eq_preset_in_use(p_indx, data_rx_val_strs[i]);
            b_processed = true;
        }
        
        if (b_processed) {
            // Key and value were processed
            if (err == 0) {
                // Processed successfully
                good_cnt++;
            } else {
                // Problem with key/val pair
                bad_cnt++;
            }
        }
    }
    
    // Finished cycling through all received Keys. Now determine what type of 
    // response to send back.
    if (good_cnt > 0) {
        // One or more keys were processed successfully.
        
        // Something was written to - so no longer factory defaulted, so
        // remember this.
        // Explanation: data_com.com_state is a state that can be interrogated 
        // by the Core app, so that the Core app can determine if ANY of the 
        // headset's programmable data has been modified (since factory default)
        data_com.com_state = DATA_STATE_MODIFIED;
        if (p_indx != 0) {
            // Profile 1..n, stored as 0..n-1
            data_pro[p_indx-1].pro_state = DATA_STATE_MODIFIED;
            // Explanation: data_pro[p_indx-1].pro_state is a state that can be 
            // interrogated by the Core app, so that the Core app can determine 
            // if a particular Profile's programmable data has been modified 
            // (since factory default)
        }
        if (bad_cnt == 0) {
            // No failed key processing, so send Success response.
            json_fn_send_status_success();
        } else {
            // One or more successes, but one or more fails
            json_fn_send_status_error(ERR_JSON_SOME_GOOD_SOME_BAD);
        }
    } else {
        if (bad_cnt > 0) {
            // No successes, just one or more fails
            json_fn_send_status_error(ERR_JSON_NO_GOOD_SOME_BAD);
        }
    }
    
    return 0;
}

// .............................................................................
// Process received JSON READ message.
// Have received a number of keys in the JSON read, and have stored the 
// key strings in arrays (data_rx_key_strs[]).
// Now, cycle through each of these saved keys, processing each one by calling 
// the associated read function for each key.
// If all goes well, send a status response with the requested values 
// (along with their keys) in a JSON response
// Inputs: 
//  p_indx      Profile Index (0 = common, 1..n = profiles)
//  num_key     Indicates number of key/val strings received /stored in the array
// Outputs:
//  Will send either a READ RESPOSNE (with key/val pairs), or 
// will send ERROR status response:
//      ERROR - ERR_JSON_SOME_GOOD_SOME_BAD
//      ERROR - ERR_JSON_SOME_GOOD_SOME_ADMIN
//      ERROR - ERR_JSON_NO_GOOD_SOME_BAD
// .............................................................................
int json_func_process_read(uint8_t p_indx, uint8_t num_key)
{
    int err;
    int i;
    bool b_processed;
    uint8_t good_cnt = 0;
    uint8_t bad_cnt = 0;
    uint8_t admin_cnt = 0;
    char val_str[JSON_DATA_VAL_STR_MAX_LEN];
    
    if (p_indx > MAX_PROFILE) {
        // Invalid profile index
        return -1;
    }
    if (num_key > JSON_DATA_KEYS_MAX) {
        // Invalid number of keys
        return -2;
    }
    
    // Initialise index to tx key val array
    // As the code cycles through each received Key, it adds the JSON response
    // information (the key, and the value) to these two arrays:
    // data_tx_key_strs[] and data_tx_val_strs[]. When finished cycling through 
    // all the received keys, these two tx arrays will be used when building 
    // the JSON Read Resposne message.
    key_val_tx_arr_indx_init();

    // Check each of the defined keys
    for (i = 0; i < num_key; i++) 
    {
        // assume success
        err = 0;
        b_processed = false;
        
        // COMMON DATASET
        
        if (!strcmp(data_rx_key_strs[i], json_key_Mode_Id)) { 
            err = json_fn_r_mode_id(p_indx, val_str);
            b_processed = true;
        }
        else if (!strcmp(data_rx_key_strs[i], json_key_Firmware_Ver)) { 
            err = json_fn_r_fw_ver_id(p_indx, val_str);
            b_processed = true;
        }
        else if (!strcmp(data_rx_key_strs[i], json_key_Serial_Num)) { 
            err = json_fn_r_ser_num(p_indx, val_str);
            b_processed = true;
        }
        else if (!strcmp(data_rx_key_strs[i], json_key_Com_State)) { 
            err = json_fn_r_com_data_state(p_indx, val_str);
            b_processed = true;
        }
        else if (!strcmp(data_rx_key_strs[i], json_key_Blue_Enabled)) { 
            err = json_fn_r_blue_enable(p_indx, val_str);
            b_processed = true;
        }
        else if (!strcmp(data_rx_key_strs[i], json_key_Audio_Vol)) { 
            err = json_fn_r_audio_vol(p_indx, val_str);
            b_processed = true;
        }
        else if (!strcmp(data_rx_key_strs[i], json_key_Batt_Level)) { 
            err = json_fn_r_batt_lvl(p_indx, val_str);
            b_processed = true;
        }
        else if (!strcmp(data_rx_key_strs[i], json_key_Batt_Min)) { 
            err = json_fn_r_batt_min(p_indx, val_str);
            b_processed = true;
        }
        else if (!strcmp(data_rx_key_strs[i], json_key_Cues_Enabled)) { 
            err = json_fn_r_audio_cue_enable(p_indx, val_str);
            b_processed = true;
        }
        else if (!strcmp(data_rx_key_strs[i], json_key_Charge_Status)) { 
            err = json_fn_r_charge_status(p_indx, val_str);
            b_processed = true;
        }
        else if (!strcmp(data_rx_key_strs[i], json_key_Signal_Level)) { 
            err = json_fn_r_sig_lvl(p_indx, val_str);
            b_processed = true;
        }
        else if (!strcmp(data_rx_key_strs[i], json_key_Audio_Pwr_Off_Time)) { 
            err = json_fn_r_audio_pwr_tim(p_indx, val_str);
            b_processed = true;
        }
        else if (!strcmp(data_rx_key_strs[i], json_key_Audio_Pwr_Off_Enabled)) { 
            err = json_fn_r_audio_pwr_en(p_indx, val_str);
            b_processed = true;
        }
        else if (!strcmp(data_rx_key_strs[i], json_key_Disp_Menu_Time)) { 
            err = json_fn_r_disp_menu_tim(p_indx, val_str);
            b_processed = true;
        }
        else if (!strcmp(data_rx_key_strs[i], json_key_Mic_Enabled)) { 
            err = json_fn_r_mic_enable(p_indx, val_str);
            b_processed = true;
        }
        else if (!strcmp(data_rx_key_strs[i], json_key_Mic_Connected)) { 
            err = json_fn_r_mic_conn(p_indx, val_str);
            b_processed = true;
        }
        
        else if (!strcmp(data_rx_key_strs[i], json_key_Eq_Presets_X)) { 
            err = json_fn_r_eq_presets_X(p_indx, val_str);
            // Pre-populate val_str[] so not passing empty string to function
            val_str[0] = ' ';
            val_str[1] = '\0';
            b_processed = true;
        }

        else if (!strcmp(data_rx_key_strs[i], json_key_Dfu_File_Size)) { 
            err = json_fn_r_dfu_file_size(p_indx, val_str);
            b_processed = true;
        }
        else if (!strcmp(data_rx_key_strs[i], json_key_Dfu_File_Crc)) { 
            err = json_fn_r_dfu_file_crc(p_indx, val_str);
            b_processed = true;
        }
        else if (!strcmp(data_rx_key_strs[i], json_key_Dfu_Segment_size)) { 
            err = json_fn_r_dfu_seq_size(p_indx, val_str);
            b_processed = true;
        }
        
        else if (!strcmp(data_rx_key_strs[i], json_key_Prof_In_Use)) { 
            err = json_fn_r_prof_in_use(p_indx, val_str);
            b_processed = true;
        }
        else if (!strcmp(data_rx_key_strs[i], json_key_Prof_Count)) { 
            err = json_fn_r_prof_count(p_indx, val_str);
            b_processed = true;
        }
        else if (!strcmp(data_rx_key_strs[i], json_key_Prof_Names)) { 
            err = json_fn_r_prof_names(p_indx, val_str);
            // Special case, resulting value string is not 'val_str',
            // but is 'json_rx_prof_names', but will add 'val_str'
            // with this key now, which is ok, but just make sure it
            // does not have zero len
            val_str[0] = ' ';
            val_str[1] = '\0';
            b_processed = true;
        }

        // PROFILES

        else if (!strcmp(data_rx_key_strs[i], json_key_Prof_Enabled)) { 
            err = json_fn_r_prof_enabled(p_indx, val_str);
            b_processed = true;
        }
        else if (!strcmp(data_rx_key_strs[i], json_key_Prof_Name)) { 
            err = json_fn_r_prof_name(p_indx, val_str);
            b_processed = true;
        }
        else if (!strcmp(data_rx_key_strs[i], json_key_Prof_State)) { 
            err = json_fn_r_pro_data_state(p_indx, val_str);
            b_processed = true;
        }
        else if (!strcmp(data_rx_key_strs[i], json_key_Disp_On)) { 
            err = json_fn_r_disp_on(p_indx, val_str);
            b_processed = true;
        }
        else if (!strcmp(data_rx_key_strs[i], json_key_Blue_Power_Up)) { 
            err = json_fn_r_blue_pwr_up(p_indx, val_str);
            b_processed = true;
        }
        else if (!strcmp(data_rx_key_strs[i], json_key_Disp_Bright)) { 
            err = json_fn_r_disp_bright(p_indx, val_str);
            b_processed = true;
        }
        else if (!strcmp(data_rx_key_strs[i], json_key_Chat_Mix)) { 
            err = json_fn_r_chat_mix(p_indx, val_str);
            b_processed = true;
        }
        else if (!strcmp(data_rx_key_strs[i], json_key_Media_Mix)) { 
            err = json_fn_r_media_mix(p_indx, val_str);
            b_processed = true;
        }
        else if (!strcmp(data_rx_key_strs[i], json_key_Mic_Volume)) { 
            err = json_fn_r_mic_vol(p_indx, val_str);
            b_processed = true;
        }
        else if (!strcmp(data_rx_key_strs[i], json_key_Mic_Side_Tone)) { 
            err = json_fn_r_mic_sidetone(p_indx, val_str);
            b_processed = true;
        }
        else if (!strcmp(data_rx_key_strs[i], json_key_Mic_Noise_Gate)) { 
            err = json_fn_r_mic_noisegate(p_indx, val_str);
            b_processed = true;
        }
        else if (!strcmp(data_rx_key_strs[i], json_key_Mic_Noise_Reduction)) { 
            err = json_fn_r_mic_noise_red(p_indx, val_str);
            b_processed = true;
        }
        else if (!strcmp(data_rx_key_strs[i], json_key_Mic_Vol_Norm)) { 
            err = json_fn_r_mic_vol_norm(p_indx, val_str);
            b_processed = true;
        }
        else if (!strcmp(data_rx_key_strs[i], json_key_Mic_Led_Bright)) { 
            err = json_fn_r_mic_led_bright(p_indx, val_str);
            b_processed = true;
        }
        else if (!strcmp(data_rx_key_strs[i], json_key_ANC_Mode)) { 
            err = json_fn_r_anc_mode(p_indx, val_str);
            b_processed = true;
        }
        else if (!strcmp(data_rx_key_strs[i], json_key_Eq_Preset_En)) { 
            err = json_fn_r_eq_preset_en(p_indx, val_str);
            b_processed = true;
        }
        else if (!strcmp(data_rx_key_strs[i], json_key_Audio_Surround)) { 
            err = json_fn_r_audio_surround(p_indx, val_str);
            b_processed = true;
        }
        else if (!strcmp(data_rx_key_strs[i], json_key_Light_Effect)) { 
            err = json_fn_r_light_effect(p_indx, val_str);
            b_processed = true;
        }
        else if (!strcmp(data_rx_key_strs[i], json_key_Light_En)) { 
            err = json_fn_r_light_en(p_indx, val_str);
            b_processed = true;
        }
        else if (!strcmp(data_rx_key_strs[i], json_key_Light_Color_Mode)) { 
            err = json_fn_r_light_colour_mode(p_indx, val_str);
            b_processed = true;
        }
        else if (!strcmp(data_rx_key_strs[i], json_key_Light_Solid_Color)) { 
            err = json_fn_r_light_solid_colour(p_indx, val_str);
            b_processed = true;
        }
        else if (!strcmp(data_rx_key_strs[i], json_key_Light_Gradient_Color)) { 
            err = json_fn_r_light_grad_colour(p_indx, val_str);
            b_processed = true;
        }
        else if (!strcmp(data_rx_key_strs[i], json_key_Light_Rate)) { 
            err = json_fn_r_light_rate(p_indx, val_str);
            b_processed = true;
        }
        else if (!strcmp(data_rx_key_strs[i], json_key_Light_Bright)) { 
            err = json_fn_r_light_bright(p_indx, val_str);
            b_processed = true;
        }
        else if (!strcmp(data_rx_key_strs[i], json_key_Light_Auto_Off)) { 
            err = json_fn_r_light_auto_off(p_indx, val_str);
            b_processed = true;
        }
        else if (!strcmp(data_rx_key_strs[i], json_key_Eq_Preset_In_Use)) { 
            err = json_fn_r_eq_preset_in_use(p_indx, val_str);
            b_processed = true;
        }
        
        
        if (b_processed) {
            // Key was processed
            if (err == 0) {
                // Key was processed successfully, so add key/val pair to tx array
                err = json_add_tx_key_val(data_rx_key_strs[i], strlen(data_rx_key_strs[i]), val_str, strlen(val_str));
                if (err >= 0) {
                    // Processed successfully
                    good_cnt++;
                } else {
                    // Should have been good, but maybe array is full?
                    admin_cnt++;
                }
            } else {
                // Problem with key, so increment 'bad' counter
                bad_cnt++;
            }
        }
                    
                    
    }
    
    // Finished cycling through all received Keys. Now determine what type of 
    // response to send back.
    if (good_cnt > 0) {
        // One or more keys were processed successfully.
        if (bad_cnt > 0) {
            // However, also one or more fails
            json_fn_send_status_error(ERR_JSON_SOME_GOOD_SOME_BAD);
        } else if (admin_cnt > 0) {
            // However, one or more admin issues 0 e.g. array is full
            json_fn_send_status_error(ERR_JSON_SOME_GOOD_SOME_ADMIN);
        } else {
            // All successes!
            json_fn_send_read_response();
        }
        
    } else {
        // No keys processed successfully.
        if (bad_cnt > 0) {
            // One or more failed keys.
            json_fn_send_status_error(ERR_JSON_NO_GOOD_SOME_BAD);
        }
    }
    return 0;
}

// .............................................................................
// Process received JSON Command message.
// This message, for now, contains ONE key/value pair.
// Examine the 'key' to determine which command function to call.
// If all goes well, send s status success JSON response, else send Error.
// Inputs: 
//  p_indx      Profile Index (0 = common, 1..n = profiles)
//  num_key     Indicates number of key/val strings received /stored in 
//              the array
// Outputs:
//  Will send a STATUS message:
//      SUCCESS
//      ERROR - ERR_JSON_CMD_UNKNOWN
// .............................................................................
int json_func_process_cmd(uint8_t p_indx, uint8_t num_key)
{
    int err = 0;
    bool b_processed;
    
    if (p_indx > MAX_PROFILE) {
        // Invalid profile index
        return -1;
    }
    if (num_key > JSON_DATA_KEYS_MAX) {
        // Invalid number of keys in JSON message
        return -2;
    }
    
    b_processed = false;

    // Note; For now, the command can only contain ONE key/value pair
    
    // Based on the received key (e.g. "ProRd" to read an entire Profile),
    // determine which command function to call.
    if (!strcmp(data_rx_key_strs[0], json_key_cmd_Profile_Read)) {    
        err = json_fn_c_profile_read(p_indx, data_rx_val_strs[0]);
        b_processed = true;
    }
    else if (!strcmp(data_rx_key_strs[0], json_key_cmd_Profile_Restore)) {    
        err = json_fn_c_profile_restore(p_indx, data_rx_val_strs[0]);
        b_processed = true;
    }
    else if (!strcmp(data_rx_key_strs[0], json_key_cmd_Profile_Create)) {    
        err = json_fn_c_profile_create(p_indx, data_rx_val_strs[0]);
        b_processed = true;
    }
    else if (!strcmp(data_rx_key_strs[0], json_key_cmd_Profile_Delete)) {    
        err = json_fn_c_profile_delete(p_indx, data_rx_val_strs[0]);
        b_processed = true;
    }
    else if (!strcmp(data_rx_key_strs[0], json_key_cmd_Profile_Duplicate)) {    
        err = json_fn_c_profile_duplicate(p_indx, data_rx_val_strs[0]);
        b_processed = true;
    }
    else if (!strcmp(data_rx_key_strs[0], json_key_cmd_Dfu_Commit)) {    
        err = json_fn_c_dfu_commit(p_indx, data_rx_val_strs[0]);
        b_processed = true;
    }
    
    if (!b_processed) {
        // Unknown command
        json_fn_send_status_error(ERR_JSON_CMD_UNKNOWN);
        return -3;
    }
    
    return 0;    
}
// .............................................................................
