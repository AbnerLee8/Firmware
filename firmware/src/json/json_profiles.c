/******************************************************************************
  @File Name
    json_profiles.c

  @Summary
    JSON Profile Functions

  @Description

    
*******************************************************************************/
#include "json.h"
#include "json_data.h"
#include "json_profiles.h"

// .............................................................................

data_pro_t data_pro_copy;                   // One struct, to copy Profile in to, for comparing

// .............................................................................

// .............................................................................
// Get id of first available/disabled profile
// Output:
//  0       None available
//  1..n    ID of available Profile
// .............................................................................
uint8_t json_profiles_get_disabled()
{
    int i;

    for (i = 0; i < MAX_PROFILE; i++) {
        if (data_pro[i].pro_en == false) {
            return (i+1);
        }
    }
    return 0;
}

// .............................................................................
// Get id of first enabled profile
// Output:
//  0       None available
//  1..n    ID of enabled Profile
// .............................................................................
uint8_t json_profiles_get_enabled()
{
    int i;

    for (i = 0; i < MAX_PROFILE; i++) {
        if (data_pro[i].pro_en == true) {
            return (i+1);
        }
    }
    return 0;
}
// .............................................................................
// Get id of first enabled profile, excluding the specified profile
// This is used when Core app attempts to disable a profile, and we first
// want to know if there is another (different) enabled profile or not.
// Input: 
//  p_indx  1...n (current profile))
// Output:
//  0       No other profile enabled
//  1..n    First other enabled profile
// .............................................................................
uint8_t json_profiles_get_other_enabled_profile(uint8_t p_indx)
{
    int i;

    // Note: p_indx is used for Profile Index (1..n)
    if ((p_indx == 0)|| (p_indx > MAX_PROFILE)) {
        return 0;
    }

    for (i = 0; i < MAX_PROFILE; i++) {
        if ((data_pro[i].pro_en == true) && (i != p_indx-1)) {
            return (i+1);
        }
    }
    return 0;
}

// .............................................................................
// Have switched to a different Profile.
// Now, must cycle through each of the Profile parameters, and take any
// appropriate action for any parameters that have changed (e.g. if, by changing
// profile, the ANC Mode selected has changed, then need to take the 
// appropriate actions associated with have a new AND Mode selected).
// So, compare the two Profiles (old and new Profile In Use)
// For any parameters that have changed, call the appropriate Action function.
// Note 1: 'data_pro_copy' struct already holds the previous Profile In Use's 
// data (the 'old' profile).
// Note 2: 'data_com.prfl_in_use' is the new profile that we are switching to.
// Returns:
//      0: success
//      -1: error
// .............................................................................
int json_profiles_new_profile_actions()
{
    uint8_t indx;
    uint8_t eq_indx_old;
    uint8_t eq_indx_req;
    int err;
    
    indx = data_com.prfl_in_use;
    
    if ((indx == 0) || (indx > MAX_PROFILE)) {
        return -1;
    }
    
    indx--;
    
    // TODO: Add action functiosn below, as required.
    
    // Only check the parameters we are interested in.
    
    /*
    if (data_pro[indx].pro_en != data_pro_copy.pro_en) {
    }
    if (!strcmp(data_pro[indx].prfl_name, data_pro_copy.prfl_name)) { 
    }
    if (data_pro[indx].display_on != data_pro_copy.display_on) {
    }
    if (data_pro[indx].blue_on_pwr_up != data_pro_copy.blue_on_pwr_up) {
    }
    */
    if (data_pro[indx].display_bright != data_pro_copy.display_bright) {
        json_fn_a_disp_bright(data_pro[indx].display_bright);
    }
    if (data_pro[indx].chat_mix != data_pro_copy.chat_mix) {
        json_fn_a_chat_mix(data_pro[indx].chat_mix);
    }
    /*
    if (data_pro[indx].media_mix != data_pro_copy.media_mix) {
    }
    if (data_pro[indx].mic_vol != data_pro_copy.mic_vol) {
    }
    if (data_pro[indx].mic_side_tone != data_pro_copy.mic_side_tone) {
    }
    if (data_pro[indx].mic_noise_gate != data_pro_copy.mic_noise_gate) {
    }
    if (data_pro[indx].mic_noise_reduction != data_pro_copy.mic_noise_reduction) {
    }
    if (data_pro[indx].mic_vol_norm != data_pro_copy.mic_vol_norm) {
    }
    if (data_pro[indx].mic_led_bright != data_pro_copy.mic_led_bright) {
    }
    if (data_pro[indx].anc_mode != data_pro_copy.anc_mode) {
    }
    if (data_pro[indx].eq_pre_enabled != data_pro_copy.eq_pre_enabled) {
    }
    if (data_pro[indx].audio_surr != data_pro_copy.audio_surr) {
    }
    if (data_pro[indx].light_effect != data_pro_copy.light_effect) {
    }
    if (data_pro[indx].light_enabled != data_pro_copy.light_enabled) {
    }
    if (data_pro[indx].light_color_mode != data_pro_copy.light_color_mode) {
    }
    if (data_pro[indx].light_solid_color != data_pro_copy.light_solid_color) {
    }
    if (data_pro[indx].light_grad_color != data_pro_copy.light_grad_color) {
    }
    if (data_pro[indx].light_rate != data_pro_copy.light_rate) {
    }
    if (data_pro[indx].light_bright != data_pro_copy.light_bright) {
    }
    if (data_pro[indx].light_auto_off != data_pro_copy.light_auto_off) {
    }
    if (data_pro[indx].eq_pre_in_use != data_pro_copy.eq_pre_in_use) {
    }

    if (data_pro[indx].artwork_mode != data_pro_copy.artwork_mode) {
    }
    if (data_pro[indx].artwork_seq_dur != data_pro_copy.artwork_seq_dur) {
    }
    */
    
    // Note: EQ Preset indices can be the same or different
    eq_indx_old = data_pro_copy.eq_pre_in_use;
    eq_indx_req = data_pro[indx].eq_pre_in_use;
//    err = json_eq_presets_swap(eq_indx_old, eq_indx_req);
    if (err < 0) {
        // Problem occurred when switching to new EQ Preset
        return -2;
    }

    return 0;
}

// .............................................................................
// Change Profile.
// Inputs:
//  indx_new    Profile ID of the new Profile In Use
// Returns:
//      0: success
//      -1: error
// .............................................................................
int json_profiles_change_profie(uint8_t indx_new)
{
    int err;
    
    // Step 1: Verify that new Profile ID exists and is not the same as the current Profile In Use
    if ((indx_new == 0) || (indx_new > MAX_PROFILE)) {
        // Profile ID out of range
        return -1;
    }
    if (indx_new == data_com.prfl_in_use) {
        // This profile is already the Profile In Use
        return -2;
    }
    // Step 2: Take a copy of the current Profile In Use
    data_pro_copy = data_pro[data_com.prfl_in_use-1];
    
    // Step 3: Assign the new Profile In Use and enable it
    data_com.prfl_in_use = indx_new;
    data_pro[indx_new-1].pro_en = true;
    
    // Step 4: Cycle through each of the Profile parameters, and take any
    // appropriate action for any changed parameters (e.g. ANC Mode)
    err = json_profiles_new_profile_actions();
    
    if (err < 0) {
        return -1;
    }
    
    return (0);
}

// .............................................................................
// Duplicate the current Profile In Use profile in to the specified profile
//  indx_new    1..n
// .............................................................................
void json_profiles_duplicate_profie(uint8_t indx_new)
{
    uint8_t indx_old;
    
    indx_old = data_com.prfl_in_use;

    if ((indx_old == 0) || (indx_old > MAX_PROFILE)) {
        return;
    }
    if ((indx_new == 0) || (indx_new > MAX_PROFILE)) {
        return;
    }
    if (indx_old == indx_new) {
        return;
    }

    // Profile Index 1..n, stored in arrays [0..n-1]
    indx_old--;
    indx_new--;
    
    // First profile, intially Enabled, all other profiles initially disabled
    data_pro[indx_new].pro_en = data_pro[indx_old].pro_en;
    strcpy(data_pro[indx_new].prfl_name, data_pro[indx_old].prfl_name);
    data_pro[indx_new].pro_state = data_pro[indx_old].pro_state;
    data_pro[indx_new].display_on = data_pro[indx_old].display_on;
    data_pro[indx_new].blue_on_pwr_up = data_pro[indx_old].blue_on_pwr_up;
    data_pro[indx_new].display_bright = data_pro[indx_old].display_bright;
    data_pro[indx_new].chat_mix = data_pro[indx_old].chat_mix;
    data_pro[indx_new].media_mix = data_pro[indx_old].media_mix;
    data_pro[indx_new].mic_vol = data_pro[indx_old].mic_vol;
    data_pro[indx_new].mic_side_tone = data_pro[indx_old].mic_side_tone;
    data_pro[indx_new].mic_noise_gate = data_pro[indx_old].mic_noise_gate;
    data_pro[indx_new].mic_noise_reduction = data_pro[indx_old].mic_noise_reduction;
    data_pro[indx_new].mic_vol_norm = data_pro[indx_old].mic_vol_norm;
    data_pro[indx_new].mic_led_bright = data_pro[indx_old].mic_led_bright;
    data_pro[indx_new].anc_mode = data_pro[indx_old].anc_mode;
    data_pro[indx_new].eq_pre_enabled = data_pro[indx_old].eq_pre_enabled;
    data_pro[indx_new].audio_surr = data_pro[indx_old].audio_surr;
    data_pro[indx_new].light_effect = data_pro[indx_old].light_effect;
    data_pro[indx_new].light_enabled = data_pro[indx_old].light_enabled;
    data_pro[indx_new].light_color_mode = data_pro[indx_old].light_color_mode;
    data_pro[indx_new].light_solid_color = data_pro[indx_old].light_solid_color;
    data_pro[indx_new].light_grad_color = data_pro[indx_old].light_grad_color;
    data_pro[indx_new].light_rate = data_pro[indx_old].light_rate;
    data_pro[indx_new].light_bright = data_pro[indx_old].light_bright;
    data_pro[indx_new].light_auto_off = data_pro[indx_old].light_auto_off;
    data_pro[indx_new].eq_pre_in_use = data_pro[indx_old].eq_pre_in_use;
    data_pro[indx_new].artwork_mode = data_pro[indx_old].artwork_mode;
    data_pro[indx_new].artwork_seq_dur = data_pro[indx_old].artwork_seq_dur;
}
// .............................................................................
