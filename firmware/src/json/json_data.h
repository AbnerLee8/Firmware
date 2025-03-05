/* json_data.h */
#ifndef _JSON_DATA_H
#define _JSON_DATA_H

#include "app.h"

/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif

// .............................................................................
// Defines
// .............................................................................

// .............................................................................
// Types
// .............................................................................

typedef struct
{
    // Data: Common .....................

    char mode_id[JSON_DATA_VAL_STR_MAX_LEN];
    char fw_ver_left[JSON_DATA_VAL_STR_MAX_LEN];
    char fw_ver_right[JSON_DATA_VAL_STR_MAX_LEN];
    char ser_num[JSON_DATA_VAL_STR_MAX_LEN];
    uint8_t com_state;

    bool blue_enabled;
    uint8_t audio_vol;
    uint8_t batt_lvl;
    uint16_t batt_mins;
    bool audio_cues_enabled;
    uint16_t auto_pwr_off_tim;
    bool auto_pwr_off_en;
    bool charge_stat;
    uint8_t signal_lvl;
    uint16_t display_menu_timeout;
    bool mic_enabled;
    bool mic_connected;
    data_eq_pre_t eq_pre_data[MAX_EQ_PRESETS];
    uint32_t dfu_file_size;
    uint16_t dfu_file_crc;
    uint16_t dfu_segment_size;
    uint8_t prfl_in_use;
    uint8_t prfl_count;

} data_com_t;

extern data_com_t data_com;

typedef struct
{
    // Data: Profile ....................

    bool pro_en;
    char prfl_name[JSON_DATA_VAL_STR_MAX_LEN];
    uint8_t pro_state;
    bool display_on;
    bool blue_on_pwr_up;
    uint8_t display_bright;
    uint8_t chat_mix;
    uint8_t media_mix;
    uint8_t mic_vol;
    uint8_t mic_side_tone;
    uint8_t mic_noise_gate;
    bool mic_noise_reduction;
    bool mic_vol_norm;
    uint8_t mic_led_bright;
    uint8_t anc_mode;
    bool eq_pre_enabled;
    bool audio_surr;
    uint8_t light_effect;
    bool light_enabled;
    uint8_t light_color_mode;
    uint32_t light_solid_color;
    uint8_t light_grad_color;
    uint8_t light_rate;
    uint8_t light_bright;
    bool light_auto_off;
    uint8_t eq_pre_in_use;
    uint8_t artwork_mode;
    uint8_t artwork_seq_dur;

} data_pro_t;


extern data_pro_t data_pro[];


// .............................................................................
// Prototypes
// .............................................................................



/* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _JSON_DATA_H */
