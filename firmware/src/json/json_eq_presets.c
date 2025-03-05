/******************************************************************************
  @File Name
    json_eq_presets.c

  @Summary
    JSON EQ Preset Functions

  @Description

    
*******************************************************************************/
#include "json.h"
#include "json_data.h"
#include "json_eq_presets.h"

// .............................................................................

// .............................................................................
// Swap EQ Presets and action any changes (i.e. inform DSP).
// Old preset index is saved in data_pro_copy.
// This function is called when Profiles have been switched. Each Profile links
// to an EQ Preset, so when Profile is switched, need to see if new Profile 
// links to a different EQ Preset, and of so, we need to switch to that preset 
// now.
// Inputs:
//  eq_indx_old:    Index of Old EQ Preset
//  eq_indx_req:    Index of Requested EQ Preset
// .............................................................................
int json_eq_presets_swap(uint8_t eq_indx_old, uint8_t eq_indx_req)
{
    int err;
    uint8_t p_indx;
    uint8_t indx_new;
    bool b_eq_diff;

    p_indx = data_com.prfl_in_use;
    
    if ((p_indx == 0) || (p_indx > MAX_PROFILE)) {
        return -1;
    }
    
    // Switch to new EQ Preset
    err = json_eq_presets_switch(eq_indx_old, eq_indx_req, &indx_new);
    if (err < 0) {
        // No enabled EQ Preset
        return -2;
    }
    // Now see if any change in actual EQ Preset Data, or the Enabled status
    // Assume no difference
    b_eq_diff = false;
    if (eq_indx_old != indx_new) {
        for (int i = 0; i < EQ_ARRAY_SIZE; i++) {
            if (data_com.eq_pre_data[eq_indx_old-1].vals[i] != data_com.eq_pre_data[indx_new-1].vals[i]) {
                // Values are different
                b_eq_diff = true;
                break;
            }
        }            
        
    }
    // Factors to look at now are: change in EQ Preset enabled state (for this profile), 
    // and change in EQ Preset data.
    // If there are changes, we should be careful about the order in which we inform the DSP.
    // If going from Preset disabled to enabled (for this profile), want to update the data/settings first
    // If going from Preset enabled to disabled (for this profile), do this first then update the data/settings.
    if ((!data_pro[eq_indx_old-1].eq_pre_enabled) && (data_pro[indx_new-1].eq_pre_enabled)) {
        // Going from disabled to enabled preset, want to update the data/settings first
        if (b_eq_diff) {
            // Update settings
            err = json_fn_a_eq_preset_vals(indx_new);
            if (err < 0) {
                return -2;
            }
            // Then update Enabled status
            err = json_fn_a_eq_preset_en(indx_new);
            if (err < 0) {
                return -3;
            }
            
        }
    } else if ((data_pro[eq_indx_old-1].eq_pre_enabled) && (!data_pro[indx_new-1].eq_pre_enabled)) {
        // Going from enabled to disabled preset, do this first then update the data/settings
        // Update enabled status
        err = json_fn_a_eq_preset_en(indx_new);
        if (err < 0) {
            return -4;
        }
        if (b_eq_diff) {
            // Then update data
            err = json_fn_a_eq_preset_vals(indx_new);
            if (err < 0) {
                return -5;
            }
        }
    } else {
        // No change in enabled status
        if (b_eq_diff) {
            // Update data
            err = json_fn_a_eq_preset_vals(indx_new);
            if (err < 0) {
                return -6;
            }
        }
    }

    return 0;
}

// .............................................................................
// EQ Presets: Find the first available defined EQ Preset
// Inputs:
//  eq_id:  Pointer to resulting index of EQ Preset (1..n))
// Returns:
//  0:  all ok   eq_id is valid
//  <0: none found
// .............................................................................
int json_eq_presets_first_defined(uint8_t *eq_id)
{
    // Move to the first defined EQ Preset
    for (int i = 0; i < MAX_EQ_PRESETS; i++) {
        if (data_com.eq_pre_data[i].defined) {
            // Found a defined EQ Preset
            *eq_id = i+1;
            return 0;
        }
    }
    return -1;
}

// .............................................................................
// EQ Presets: Switch to first enabled EQ Preset
// Inputs:
//  indx_old:   Index for existing EQ Preset
//  indx_req:   Index for requesting EQ Preset
//  indx_new:   (ptr to) Index for actual new EQ Preset
// Returns:
//  0: all ok   Have swapped successfully to indx_new
//              (even if indx_new is same as indx_old, or even if had to swap
//              to a different enabled EQ Preset)
//  <0: error
// .............................................................................
int json_eq_presets_switch(uint8_t indx_old, uint8_t indx_req, uint8_t *indx_new)
{
    int err;
    uint8_t eq_id;
    
    if ((indx_old == 0) || (indx_old > MAX_EQ_PRESETS)) {
        // Existing EQ Preset index out of range
        return -1;
    }
    if ((indx_req == 0) || (indx_req > MAX_EQ_PRESETS)) {
        // Requested EQ Preset index out of range
        return -2;
    }
    if (indx_old == indx_req) {
        // Same EQ Preset requested
        *indx_new = indx_old;
        return 0;
    }
    // We can move to this EQ Preset if this new EQ Preset is defined
    if (data_com.eq_pre_data[indx_req-1].defined) {
        // Yes, it is defined
        *indx_new = indx_req;
        return 0;
    }
    
    // Find first available defined EQ Preset
    err = json_eq_presets_first_defined(&eq_id);
    if (err == 0) {
        // Found a defined EQ Preset
        *indx_new = eq_id;
    } else {
        // No defined EQ Preset found, so move to the first Eq Preset but also 
        // Disable Presets for this Profile
        *indx_new = 1;
        data_pro[0].eq_pre_enabled = false;
    }
    return 0;
}
// .............................................................................
// Copy the Profile In Use's current EQ Preset values to supplied array
void json_eq_presets_read(int8_t *vals)
{
    uint8_t eq_indx;
    
    // The EQ Preset (index) in use by the current Profile
    eq_indx = data_pro[data_com.prfl_in_use-1].eq_pre_in_use;
    
    // Copy EQ Preset values to uint8_t buffer (of size EQ_ARRAY_SIZE)
    for (int i = 0; i < EQ_ARRAY_SIZE; i++) 
    {
        *vals++ = data_com.eq_pre_data[eq_indx-1].vals[i];
    }
}

// .............................................................................
