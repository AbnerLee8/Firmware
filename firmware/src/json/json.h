/* json.h */
#ifndef _JSON_H
#define _JSON_H

#include "app.h"

/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif

// .............................................................................
// Defines
// .............................................................................

#define DATA_STATE_FACTORY              1
#define DATA_STATE_MODIFIED             2

#define STATE_EXPECT_OP_OR_STATUS       0
#define STATE_EXPECT_READ_KEYS          1
#define STATE_EXPECT_WRITE_KV_PAIRS     2
#define STATE_EXPECT_INFO_KV_PAIRS      3
#define STATE_EXPECT_COMMAND            4
#define STATE_ERROR                     7

#define RESP_TYPE_STATUS                1
#define RESP_TYPE_INFO                  2
    
#define MAX_PROFILE                     5           // Max number of programmable/save-able Profiles
//#define MAX_EQ_PRESETS                  6           // Max number fo Equaliser Profiles
#define MAX_EQ_PRESETS                  14           // Max number fo Equaliser Profiles
#define EQ_ARRAY_SIZE                   5           // Number of values (dB settings) saved per EQ Profile
#define EQ_STR_SIZE                     6           // Enough chars to represnt signed 8-bit int, in string (e.g. "-127")

#define DS_COMMON           1                       // Paramater is singular/global
#define DS_PROFILE          2                       // Parameter belongs to the specified profile
 
#define JSON_TX_MSG_STR_LEN	        AIROHA_MAX_PKT_SIZE // Maximum size of a complete JSON message

#define JSON_DATA_KEYS_MAX          32              // Max number of keys allowed in one JSON messages
#define JSON_DATA_KEY_STR_MAX_LEN   16              // Max length of any one JSON Key string
#define JSON_DATA_VAL_STR_MAX_LEN   64              // Max length of any one JSON Value string
#define JSON_DATA_VAL_PROF_NAMES_STR_MAX_LEN    256 // Max length of all profiel names and their indices, sent in response to a command/request from Core

#define MSG_TYPE_READ               0
#define MSG_TYPE_WRITE              1
#define MSG_TYPE_INFO               2
#define MSG_TYPE_CMND               3
#define MSG_TYPE_STATUS_SUCCESS     4
#define MSG_TYPE_STATUS_ERR         5

enum json_err {
    ERR_JSON_SOME_GOOD_SOME_BAD = 0,
    ERR_JSON_SOME_GOOD_SOME_ADMIN,
    ERR_JSON_NO_GOOD_SOME_BAD,
    ERR_JSON_ARRAY_SIZE_1,
    ERR_JSON_ARRAY_SIZE_2,
    ERR_JSON_ARRAY_SIZE_3,
    ERR_JSON_ARRAY_SIZE_4,
    ERR_JSON_ARRAY_SIZE_5,
    ERR_JSON_EQ_PRESET_1,
    ERR_JSON_EQ_PRESET_2,
    ERR_JSON_EQ_PRESET_3,
    ERR_JSON_EQ_PRESET_4,
    ERR_JSON_EQ_PRESET_5,
    ERR_JSON_EQ_PRESET_6,
    ERR_JSON_EQ_PRESET_7,
    ERR_JSON_EQ_PRESET_8,
    ERR_JSON_EQ_PRESET_9,
    ERR_JSON_EQ_PRESET_10,
    ERR_JSON_EQ_PRESET_11,
    ERR_JSON_EQ_PRESET_12,
    ERR_JSON_EQ_PRESET_13,
    ERR_JSON_EQ_PRESET_14,
    ERR_JSON_KV_ERR,
    ERR_JSON_KEY_LEN,
    ERR_JSON_READ_ERR,
    ERR_JSON_CMD_MSG,
    ERR_JSON_CMD_KEY,
    ERR_JSON_CMD_VAL,
    ERR_JSON_CMD_FORMAT,
    ERR_JSON_CMD_UNKNOWN,
    ERR_JSON_PROFILE_INDX,
    ERR_JSON_PROFILE_READ,
    ERR_JSON_PROFILE_NAMES_RESTORE,
    ERR_JSON_PROFILE_NAMES_CREATE,
    ERR_JSON_PROFILE_NAMES_DELETE,
    ERR_JSON_PROFILE_NAMES_DUPLICATE,
    ERR_JSON_DELETE_PROFILE,
    ERR_JSON_DUPLICATE_PROFILE,
    ERR_JSON_PARSE_NULL_BUFF,
    ERR_JSON_PARSE_BUFF_LEN,
    ERR_JSON_PARSE_NUM_TOKEN,
    ERR_JSON_PARSE_NOT_OBJECT,
    ERR_JSON_PARSE_UNEXPECTED_FORMAT_READ,
    ERR_JSON_PARSE_UNEXPECTED_FORMAT_WRITE,
    ERR_JSON_PARSE_UNEXPECTED_FORMAT_COMMAND,
    ERR_JSON_PARSE_UNEXPECTED_FORMAT,
    ERR_JSON_PARSE_UNEXPECTED_STATE,
    ERR_JSON_NO_FREE_PROFILE,
    ERR_PKT_VERIFY,
    ERR_PKT_UNKNOWN,
    ERR_DFU_NO_FILE,
    ERR_IMG_DBG_1,
    ERR_IMG_DBG_2,
    ERR_IMG_DBG_3,
    ERR_IMG_DBG_4,

};

// .............................................................................
// Structs
// .............................................................................

typedef struct
{
    int8_t vals[EQ_ARRAY_SIZE];                 // EQ Preset data - array of values for each preset
    char name[JSON_DATA_VAL_STR_MAX_LEN];       // EQ Preset names
    bool defined;                               // EQ Preset defined - flag for each preset)
} data_eq_pre_t;
        
// .............................................................................
// Strings
// .............................................................................

// JSON Keys - Common Profile
extern char json_key_Mode_Id[]; 
extern char json_key_Firmware_Ver[];
extern char json_key_Serial_Num[];
extern char json_key_Com_State[];
extern char json_key_Blue_Enabled[];
extern char json_key_Audio_Vol[];
extern char json_key_Batt_Level[];
extern char json_key_Batt_Min[];
extern char json_key_Cues_Enabled[];
extern char json_key_Charge_Status[]; 
extern char json_key_Signal_Level[]; 
extern char json_key_Audio_Pwr_Off_Time[];
extern char json_key_Audio_Pwr_Off_Enabled[];
extern char json_key_Disp_Menu_Time[];
extern char json_key_Mic_Enabled[];
extern char json_key_Mic_Connected[];
extern char json_key_Eq_Presets_X[];
extern char json_key_Eq_Preset_Id_X[];
extern char json_key_Eq_Preset_Vals[];
extern char json_key_Eq_Preset_Names[];
extern char json_key_Eq_Preset_Defined[];
extern char json_key_Prof_In_Use[];
extern char json_key_Dfu_File_Size[];
extern char json_key_Dfu_File_Crc[];
extern char json_key_Dfu_Segment_size[];
extern char json_key_Prof_Count[];
extern char json_key_Prof_Names[]; 
// JSON Keys - Other Profile;
extern char json_key_Prof_Enabled[]; 
extern char json_key_Prof_Name[]; 
extern char json_key_Prof_State[];
extern char json_key_Disp_On[];
extern char json_key_Blue_Power_Up[];
extern char json_key_Disp_Bright[];
extern char json_key_Chat_Mix[];
extern char json_key_Media_Mix[];
extern char json_key_Mic_Volume[];
extern char json_key_Mic_Side_Tone[];
extern char json_key_Mic_Noise_Gate[];
extern char json_key_Mic_Noise_Reduction[];
extern char json_key_Mic_Vol_Norm[];
extern char json_key_Mic_Led_Bright[];
extern char json_key_ANC_Mode[];
extern char json_key_Eq_Preset_En[];
extern char json_key_Audio_Surround[] ;
extern char json_key_Light_Effect[];
extern char json_key_Light_En[];
extern char json_key_Light_Color_Mode[];
extern char json_key_Light_Solid_Color[];
extern char json_key_Light_Gradient_Color[];
extern char json_key_Light_Rate[];
extern char json_key_Light_Bright[];
extern char json_key_Light_Auto_Off[];
extern char json_key_Eq_Preset_In_Use[];
// JSON Commands
extern char json_key_cmd_Profile_Read[]; 
extern char json_key_cmd_Profile_Restore[]; 
extern char json_key_cmd_Profile_Create[];
extern char json_key_cmd_Profile_Delete[];
extern char json_key_cmd_Profile_Duplicate[];
extern char json_key_cmd_Dfu_Commit[];

// .............................................................................
// Variables
// .............................................................................

extern char data_rx_key_strs[JSON_DATA_KEYS_MAX][JSON_DATA_KEY_STR_MAX_LEN];        // Keys received in JSON read/write message
extern char data_rx_val_strs[JSON_DATA_KEYS_MAX][JSON_DATA_VAL_STR_MAX_LEN];        // Vals received in JSON write message
extern char data_tx_key_strs[JSON_DATA_KEYS_MAX][JSON_DATA_KEY_STR_MAX_LEN];        // Keys to send in JSON read response
extern char data_tx_val_strs[JSON_DATA_KEYS_MAX][JSON_DATA_VAL_STR_MAX_LEN];        // Vals to send in JSON read response
extern char data_i_key_strs[JSON_DATA_KEYS_MAX][JSON_DATA_KEY_STR_MAX_LEN];         // Keys to send in JSON INFO aysnch message
extern char data_i_val_strs[JSON_DATA_KEYS_MAX][JSON_DATA_VAL_STR_MAX_LEN];         // Vals to send in JSON INFO aysnch message
extern uint8_t data_rx_key_val_indx;                                                // Index that indicates how many keys/vals in rx array
extern uint8_t data_tx_key_val_indx;                                                // Index that indicates how many keys/vals in tx array
extern uint8_t data_i_key_val_indx;                                                 // Index that indicates how many keys/vals in info array
extern char json_rx_arr_eq_val_str[MAX_EQ_PRESETS][EQ_ARRAY_SIZE][EQ_STR_SIZE];     // One entry per EQ Preset.

extern int8_t json_rx_eq_pre_vals[MAX_EQ_PRESETS][EQ_ARRAY_SIZE];                   // EQ Preset values - array of values for each preset
extern char json_rx_eq_pre_names[MAX_EQ_PRESETS][JSON_DATA_VAL_STR_MAX_LEN];        // EQ Preset names
extern bool json_rx_eq_pre_states[MAX_EQ_PRESETS];                                  // EQ Preset states - flag for each preset
extern char json_tx_eq_preset_x_str[2048];                                           // Holds data for one EQ Preset (id, name, state, value)

extern char json_rx_prof_names[JSON_DATA_VAL_PROF_NAMES_STR_MAX_LEN];               // Holds profile names and their indices, in response from a comamnd from Core


// .............................................................................
// Prototypes
// .............................................................................


void key_val_tx_arr_indx_init();
void key_val_rx_arr_indx_init();
void key_val_i_arr_indx_init();
uint8_t key_val_i_arr_indx_get();
int json_add_i_key_val(char *key_str, uint8_t key_len, char *val_str, uint8_t val_len);

void json_data_init_common();
void json_data_init_profile(uint8_t indx);
void json_data_init();
int json_profiles_change_profie(uint8_t indx_new);
void json_profiles_duplicate_profie(uint8_t indx_new);
int json_eq_presets_first_defined(uint8_t *eq_id);
int json_eq_presets_switch(uint8_t indx_old, uint8_t indx_req, uint8_t *indx_new);

uint16_t json_data_auto_pwr_off_tim_read();
bool json_data_auto_pwr_off_en_read();

uint32_t json_data_dfu_file_size_read();
uint16_t json_data_dfu_file_crc_read();
uint16_t json_data_dfu_segment_size_read();
void json_eq_presets_read(int8_t *vals);

void json_fn_send_status_success();
void json_fn_send_status_error(uint8_t err_no);

int json_func_process_write(uint8_t p_indx, uint8_t num_key);
int json_func_process_read(uint8_t p_indx, uint8_t num_key);
int json_build_prof_names_str();
int json_func_process_cmd(uint8_t p_indx, uint8_t num_key);
int json_add_rx_key_val(char *key_str, uint8_t key_len, char *val_str, uint8_t val_len);
int json_add_rx_key(char *key_str, uint8_t key_len);
int json_add_tx_key_val(char *key_str, uint8_t key_len, char *val_str, uint8_t val_len);
int json_add_tx_key(char *key_str, uint8_t key_len);

uint8_t json_parse_send_num_get();
int json_build_tx_str(uint8_t *tx_buf, uint16_t *tx_buf_len, uint8_t id, uint8_t send_num, uint8_t err_no, uint16_t seg_num, uint16_t seg_tot);
void json_fn_send_info_message();
void json_fn_send_status_image(uint16_t seg_num, uint16_t seg_tot);

// WRITE | READ | INFO

// COMMON
int json_fn_r_mode_id(uint8_t p_indx, char *val_str);
int json_fn_r_fw_ver_id(uint8_t p_indx, char *val_str);
int json_fn_r_ser_num(uint8_t p_indx, char *val_str);
int json_fn_r_com_data_state(uint8_t p_indx, char *val_str);
int json_fn_w_blue_enable(uint8_t p_indx, char *val_str);
int json_fn_r_blue_enable(uint8_t p_indx, char *val_str);
void json_fn_i_blue_enable();
bool blue_enable_get();
void blue_enable_set(bool val);
int json_fn_a_audio_vol(int i_val);
int json_fn_w_audio_vol(uint8_t p_indx, char *val_str);
int json_fn_r_audio_vol(uint8_t p_indx, char *val_str);
void json_fn_i_audio_vol(int pos);
int json_fn_w_batt_lvl(uint8_t p_indx, char *val_str);
int json_fn_r_batt_lvl(uint8_t p_indx, char *val_str);
void json_fn_i_batt_lvl();
int json_fn_w_batt_min(uint8_t p_indx, char *val_str);
int json_fn_r_batt_min(uint8_t p_indx, char *val_str);
void json_fn_i_batt_min();
int json_fn_w_audio_cue_enable(uint8_t p_indx, char *val_str);
int json_fn_r_audio_cue_enable(uint8_t p_indx, char *val_str);
void json_fn_i_audio_cue_enable();
bool audio_cue_enable_get();
void audio_cue_enable_set(bool val);
int json_fn_r_charge_status(uint8_t p_indx, char *val_str);
void json_fn_i_charge_status();
int json_fn_r_sig_lvl(uint8_t p_indx, char *val_str);
void json_fn_i_sig_lvl();
int json_fn_w_audio_pwr_tim(uint8_t p_indx, char *val_str);
int json_fn_r_audio_pwr_tim(uint8_t p_indx, char *val_str);
void json_fn_i_audio_pwr_tim();
int json_fn_w_disp_menu_tim(uint8_t p_indx, char *val_str);
int json_fn_r_disp_menu_tim(uint8_t p_indx, char *val_str);
void json_fn_i_disp_menu_tim();
int json_fn_w_mic_enable(uint8_t p_indx, char *val_str);
int json_fn_r_mic_enable(uint8_t p_indx, char *val_str);
void json_fn_i_mic_enable();
bool mic_enable_get();
void mic_enable_set(bool val);
int json_fn_r_mic_conn(uint8_t p_indx, char *val_str);

int json_fn_w_eq_preset_X(uint8_t p_indx, data_eq_pre_t *eq_data);
int json_fn_w_eq_presets_X(uint8_t p_indx, char *val_str);
int json_fn_r_eq_presets_X(uint8_t p_indx, char *val_str);
int json_fn_a_eq_preset_vals(uint8_t p_indx);

int json_fn_w_dfu_file_size(uint8_t p_indx, char *val_str);
int json_fn_r_dfu_file_size(uint8_t p_indx, char *val_str);
int json_fn_w_dfu_file_crc(uint8_t p_indx, char *val_str);
int json_fn_r_dfu_file_crc(uint8_t p_indx, char *val_str);
int json_fn_r_dfu_seq_size(uint8_t p_indx, char *val_str);

void json_fn_i_mic_conn();
bool mic_conn_get();
int json_fn_w_prof_in_use(uint8_t p_indx, char *val_str);
int json_fn_r_prof_in_use(uint8_t p_indx, char *val_str);
void json_fn_i_prof_in_use();
int json_fn_r_prof_count(uint8_t p_indx, char *val_str);
int json_fn_r_prof_names(uint8_t p_indx, char *val_str);
void json_fn_i_prof_names();

// PROFILE
int json_fn_w_prof_enabled(uint8_t p_indx, char *val_str);
int json_fn_r_prof_enabled(uint8_t p_indx, char *val_str);
int json_fn_w_prof_name(uint8_t p_indx, char *val_str);
int json_fn_r_prof_name(uint8_t p_indx, char *val_str);
int json_fn_r_pro_data_state(uint8_t p_indx, char *val_str);
int json_fn_w_disp_on(uint8_t p_indx, char *val_str);
int json_fn_r_disp_on(uint8_t p_indx, char *val_str);
void json_fn_i_disp_on();
int json_fn_w_blue_pwr_up(uint8_t p_indx, char *val_str);
int json_fn_r_blue_pwr_up(uint8_t p_indx, char *val_str);
void json_fn_i_blue_pwr_up();
int json_fn_a_disp_bright(uint8_t u8_val);
int json_fn_n_disp_bright(uint8_t u8_val);
int json_fn_w_disp_bright(uint8_t p_indx, char *val_str);
int json_fn_r_disp_bright(uint8_t p_indx, char *val_str);
void json_fn_a_chat_mix(int i_val);
void json_fn_n_chat_mix(uint8_t u8_val);
int json_fn_w_chat_mix(uint8_t p_indx, char *val_str);
int json_fn_r_chat_mix(uint8_t p_indx, char *val_str);
void json_fn_i_chat_mix(int pos);
int json_fn_w_media_mix(uint8_t p_indx, char *val_str);
int json_fn_r_media_mix(uint8_t p_indx, char *val_str);
void json_fn_i_media_mix();
int json_fn_w_mic_vol(uint8_t p_indx, char *val_str);
int json_fn_r_mic_vol(uint8_t p_indx, char *val_str);
void json_fn_i_mic_vol();
int json_fn_w_mic_sidetone(uint8_t p_indx, char *val_str);
int json_fn_r_mic_sidetone(uint8_t p_indx, char *val_str);
void json_fn_i_mic_sidetone();
int json_fn_w_mic_noisegate(uint8_t p_indx, char *val_str);
int json_fn_r_mic_noisegate(uint8_t p_indx, char *val_str);
void json_fn_i_mic_noisegate();
int json_fn_w_mic_noise_red(uint8_t p_indx, char *val_str);
int json_fn_r_mic_noise_red(uint8_t p_indx, char *val_str);
void json_fn_i_mic_noise_red();
int json_fn_w_mic_vol_norm(uint8_t p_indx, char *val_str);
int json_fn_r_mic_vol_norm(uint8_t p_indx, char *val_str);
void json_fn_i_mic_vol_norm();
int json_fn_w_mic_led_bright(uint8_t p_indx, char *val_str);
int json_fn_r_mic_led_bright(uint8_t p_indx, char *val_str);
void json_fn_i_mic_led_bright();
int json_fn_w_anc_mode(uint8_t p_indx, char *val_str);
int json_fn_r_anc_mode(uint8_t p_indx, char *val_str);
void json_fn_i_anc_mode();
int json_fn_w_audio_surround(uint8_t p_indx, char *val_str);
int json_fn_r_audio_surround(uint8_t p_indx, char *val_str);
void json_fn_i_audio_surround();
int json_fn_w_light_effect(uint8_t p_indx, char *val_str);
int json_fn_r_light_effect(uint8_t p_indx, char *val_str);
void json_fn_i_light_effect();
int json_fn_w_light_en(uint8_t p_indx, char *val_str);
int json_fn_r_light_en(uint8_t p_indx, char *val_str);
void json_fn_i_light_en();
int json_fn_w_light_colour_mode(uint8_t p_indx, char *val_str);
int json_fn_r_light_colour_mode(uint8_t p_indx, char *val_str);
void json_fn_i_light_colour_mode();
int json_fn_w_light_solid_colour(uint8_t p_indx, char *val_str);
int json_fn_r_light_solid_colour(uint8_t p_indx, char *val_str);
void json_fn_i_light_solid_colour();
int json_fn_w_light_grad_colour(uint8_t p_indx, char *val_str);
int json_fn_r_light_grad_colour(uint8_t p_indx, char *val_str);
void json_fn_i_light_grad_colour();
int json_fn_w_light_rate(uint8_t p_indx, char *val_str);
int json_fn_r_light_rate(uint8_t p_indx, char *val_str);
void json_fn_i_light_rate();
int json_fn_w_light_bright(uint8_t p_indx, char *val_str);
int json_fn_r_light_bright(uint8_t p_indx, char *val_str);
void json_fn_i_light_bright();
int json_fn_w_light_auto_off(uint8_t p_indx, char *val_str);
int json_fn_r_light_auto_off(uint8_t p_indx, char *val_str);
void json_fn_i_light_auto_off();
int json_fn_a_eq_preset_en(uint8_t p_indx);
int json_fn_w_eq_preset_en(uint8_t p_indx, char *val_str);
int json_fn_r_eq_preset_en(uint8_t p_indx, char *val_str);
int json_fn_i_eq_preset_en(uint8_t p_indx);
int json_fn_w_eq_preset_in_use(uint8_t p_indx, char *val_str);
int json_fn_r_eq_preset_in_use(uint8_t p_indx, char *val_str);
void json_fn_i_eq_preset_in_use();

// COMMANDS
int json_fn_c_profile_read(uint8_t p_indx, char *val_str);
int json_fn_c_profile_restore(uint8_t p_indx, char *val_str);
int json_fn_c_profile_create(uint8_t p_indx, char *val_str);
int json_fn_c_profile_delete(uint8_t p_indx, char *val_str);
int json_fn_c_profile_duplicate(uint8_t p_indx, char *val_str);
int json_fn_c_dfu_commit(uint8_t p_indx, char *val_str);

// UTILE
uint8_t json_profiles_get_disabled();
uint8_t json_profiles_get_enabled();
uint8_t json_profiles_get_other_enabled_profile(uint8_t p_indx);
int json_eq_presets_swap(uint8_t eq_indx_old, uint8_t eq_indx_req);

/* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _JSON_H */
