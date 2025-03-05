/******************************************************************************
  @File Name
    json_parser.c

  @Summary
    JSON Parser

  @Description

    
*******************************************************************************/
#include "json.h"
#include "json_build.h"
#include "json_data.h"
#include "jsmn.h"
#include <ctype.h> 

// .............................................................................

// Exported...

char json_tx_eq_preset_x_str[2048];                                         // Holds data for one EQ Preset (id, name, state, value)
char json_rx_prof_names[JSON_DATA_VAL_PROF_NAMES_STR_MAX_LEN];              // Holds profile names and their indices, in response from a comamnd from Core

// Internal...

static uint8_t json_send_seq_num = 0;         // Used for Send Seq Num, when sending asynch message to Core/Airoha

char json_tx_eq_preset_val_str[256];                                        // Holds EQ Preset values in a string 
                                                                            // Example string: [[\"1\",\"2\",\"3\",\"4\",\"5\"],[\"2\",\"3\",\"4\",\"5\",\"6\"],[\"3\",\"4\",\"5\",\"6\",\"7\"],[\"4\",\"5\",\"6\",\"7\",\"8\"],[\"5\",\"6\",\"7\",\"8\",\"9\"],[\"6\",\"7\",\"8\",\"9\",\"1\"]]
char json_i_eq_preset_val_str[256];                                         // Holds EQ Preset values in a string 
char json_tx_eq_preset_nam_str[400];                                        // Holds EQ Preset names in a string 
                                                                            // Example string: [\"name1\",\"name2\",\"name3\",\"name4\",\"name5\",\"name6\"]
char json_i_eq_preset_nam_str[400];                                         // Holds EQ Preset names in a string 
char json_i_eq_preset_x_str[256];                                           

char json_key_val_str[2048];                                           

uint8_t json_tx_w_str[JSON_TX_MSG_STR_LEN];                                 // JSON Tx String, for sending WRITE response (e.g. status success)
uint8_t json_tx_r_str[JSON_TX_MSG_STR_LEN];                                 // JSON Tx String, for sending READ response
uint8_t json_tx_i_str[JSON_TX_MSG_STR_LEN];                                 // JSON Tx String, for sending INFO message
uint8_t json_tx_e_str[128];                                                 // JSON Tx String, for sending ERROR message - doesn't need to be too large


char json_status_success_str[] = "{\"sts\":\"success\"}";                   // Basic status success response (to a Write msg)
char json_status_error_start_str[] = "{\"sts\":\"error\",\"msg\":\"";
char json_status_error_end_str[] = "\"}";
char json_read_success_start_str[] = "{\"sts\":\"success\",\"data\":{";     // Start of success response (to Read msg)
char json_read_success_end_str[] = "}}";
char json_info_start_str[] = "{\"op\":\"i\",\"pid\":\""; 
char json_info_end_str[] = "}}";

uint16_t json_img_seq_cntr = 0;

// .............................................................................
// Send message to Airoha
// Build packet using JSON Protocol Format
//
//  PayloadLen|Src|Dest|SendNum|AckNum|PktType|PAYLOAD|Crc|
//
//  PAYLOAD: JSIN String
//
// Inputs:
//  json_arr:   JSON array/message to send (payload)
//  json_len:   Length of message
// Outputs:
//  tx_buf      Buffer containing full formatted message, ready for tx
//  tx_buf_len  Length of formatted message
// Returns:
//      err:    0:      success
//               -ve:   error
// .............................................................................
int json_tx_msg(uint8_t *tx_buf, uint16_t *tx_buf_len, uint8_t *json_arr, uint16_t json_len, uint8_t msg_id, uint8_t send_num)
{
    int res;
    
    // Asume failure :(
    res = -1;
    
    if (msg_id == COMMS_JSON_INFO) {
        // Internally generated INFO message
        // Use our internal Send Num, and set ACK to 0xFF
        res = comms_pkt_build(json_arr, json_len, tx_buf, tx_buf_len, COMMS_PKT_TYPE_JSON, COMMS_ID_MCU, COMMS_ID_CORE, json_send_seq_num, COMMS_PKT_NUM_NONE);
        json_send_seq_num++;
        if (json_send_seq_num > 250)
        {
            json_send_seq_num = 0;
        }
    } else {
        // THis is a response to a JSON read/write, so use the received Send Num as the ACK
        res = comms_pkt_build(json_arr, json_len, tx_buf, tx_buf_len, COMMS_PKT_TYPE_JSON, COMMS_ID_MCU, COMMS_ID_CORE, COMMS_PKT_NUM_NONE, send_num);
        
    }
   
    return (res);
    
}

// .............................................................................
// Construct a string that contains the Profile ID and Name, for each
// enabled string.
// Format is"
//" "n,name1,n,name2,n,name3"
// This is needed for sending all the Profile IDs and Names. This is one of the 
// exceptions (see json_build_info_msg_str()) when sending key/value pairs back 
// to the Core app. This particular value (profile id *& name info) needs to be 
// build separately and appended as a single value in the JSON Info response.
// .............................................................................
int json_build_prof_names_str()
{
    int i;
    int total_len = 0;
    int len;
    char temp_str[JSON_DATA_VAL_STR_MAX_LEN + 8];                   // Max length for Profile Name and some extra for the index, quotes, comma etc.
    uint8_t count = 0;
    
    // Initialize the result string
    strcpy(json_rx_prof_names, "{");
//    json_rx_prof_names[0] = '\0';

    // Cycle through each profile, and if enabled add its ID and Profiel Name to the string
    for (i = 0; i < MAX_PROFILE; ++i) {
        if (data_pro[i].pro_en) {
            len = strlen(data_pro[i].prfl_name);
            if (len > JSON_DATA_VAL_STR_MAX_LEN) {
                // Profile Name is too long
                return -1;
            }
            // Create string with profile index and name
            sprintf(temp_str, "\"ID_%d\":\"%s\",", i+1, data_pro[i].prfl_name);
            len = strlen(temp_str);
            total_len += len;
            if (total_len > JSON_DATA_VAL_PROF_NAMES_STR_MAX_LEN-3) {
                // Overall/final string will be too long
                return -2;
            }
            // Append index and name to final/full string
            strcat(json_rx_prof_names, temp_str);
            // Keep count of how many have been added
            count++;
        }
    }
    if (count == 0) {
        // No profiles enabled
        return -3;
    }
    // Get total length, so can overwrite the last comma
    len = strlen(json_rx_prof_names);
    // Close the JSON array
    json_rx_prof_names[len-1] = '}';
    json_rx_prof_names[len] = '\0';
    //strcat(full_str, "]");    

    return 0;
}

// .............................................................................
// Construct a JSON success status message
// .............................................................................
int json_build_success_str()
{
    int len;
    
    len = strlen(json_status_success_str);
    strncpy((char *)json_tx_w_str, json_status_success_str, len);
    return (len);
}

// .............................................................................
// Construct a JSON error status message
// .............................................................................
int json_build_error_str(uint8_t err_no)
{
    int len;
    char err_msg[64];
    
    switch (err_no) {
        case ERR_JSON_SOME_GOOD_SOME_BAD:
            strcpy(err_msg, "ERR_JSON_SOME_GOOD_SOME_BAD");
            break;
        case ERR_JSON_SOME_GOOD_SOME_ADMIN:
            strcpy(err_msg, "ERR_JSON_SOME_GOOD_SOME_ADMIN");
            break;
        case ERR_JSON_NO_GOOD_SOME_BAD:
            strcpy(err_msg, "ERR_JSON_NO_GOOD_SOME_BAD");
            break;
        case ERR_JSON_ARRAY_SIZE_1:
            strcpy(err_msg, "ERR_JSON_ARRAY_SIZE_1");
            break;
        case ERR_JSON_ARRAY_SIZE_2:
            strcpy(err_msg, "ERR_JSON_ARRAY_SIZE_2");
            break;
        case ERR_JSON_ARRAY_SIZE_3:
            strcpy(err_msg, "ERR_JSON_ARRAY_SIZE_3");
            break;
        case ERR_JSON_ARRAY_SIZE_4:
            strcpy(err_msg, "ERR_JSON_ARRAY_SIZE_4");
            break;
        case ERR_JSON_ARRAY_SIZE_5:
            strcpy(err_msg, "ERR_JSON_ARRAY_SIZE_5");
            break;
        case ERR_JSON_EQ_PRESET_1:
            strcpy(err_msg, "ERR_JSON_EQ_PRESET_1");
            break;
        case ERR_JSON_EQ_PRESET_2:
            strcpy(err_msg, "ERR_JSON_EQ_PRESET_2");
            break;
        case ERR_JSON_EQ_PRESET_3:
            strcpy(err_msg, "ERR_JSON_EQ_PRESET_3");
            break;
        case ERR_JSON_EQ_PRESET_4:
            strcpy(err_msg, "ERR_JSON_EQ_PRESET_4");
            break;
        case ERR_JSON_EQ_PRESET_5:
            strcpy(err_msg, "ERR_JSON_EQ_PRESET_5");
            break;
        case ERR_JSON_EQ_PRESET_6:
            strcpy(err_msg, "ERR_JSON_EQ_PRESET_6");
            break;
        case ERR_JSON_EQ_PRESET_7:
            strcpy(err_msg, "ERR_JSON_EQ_PRESET_7");
            break;
        case ERR_JSON_EQ_PRESET_8:
            strcpy(err_msg, "ERR_JSON_EQ_PRESET_8");
            break;
        case ERR_JSON_EQ_PRESET_9:
            strcpy(err_msg, "ERR_JSON_EQ_PRESET_9");
            break;
        case ERR_JSON_EQ_PRESET_10:
            strcpy(err_msg, "ERR_JSON_EQ_PRESET_10");
            break;
        case ERR_JSON_EQ_PRESET_11:
            strcpy(err_msg, "ERR_JSON_EQ_PRESET_11");
            break;
        case ERR_JSON_EQ_PRESET_12:
            strcpy(err_msg, "ERR_JSON_EQ_PRESET_12");
            break;
        case ERR_JSON_EQ_PRESET_13:
            strcpy(err_msg, "ERR_JSON_EQ_PRESET_13");
            break;
        case ERR_JSON_EQ_PRESET_14:
            strcpy(err_msg, "ERR_JSON_EQ_PRESET_14");
            break;
        case ERR_JSON_KV_ERR:
            strcpy(err_msg, "ERR_JSON_KV_ERR");
            break;
        case ERR_JSON_KEY_LEN:
            strcpy(err_msg, "ERR_JSON_KEY_LEN");
            break;
        case ERR_JSON_READ_ERR:
            strcpy(err_msg, "ERR_JSON_READ_ERR");
            break;
        case ERR_JSON_CMD_MSG:
            strcpy(err_msg, "ERR_JSON_CMD_MSG");
            break;
        case ERR_JSON_CMD_KEY:
            strcpy(err_msg, "ERR_JSON_CMD_KEY");
            break;
        case ERR_JSON_CMD_VAL:
            strcpy(err_msg, "ERR_JSON_CMD_VAL");
            break;
        case ERR_JSON_CMD_FORMAT:
            strcpy(err_msg, "ERR_JSON_CMD_FORMAT");
            break;
        case ERR_JSON_CMD_UNKNOWN:
            strcpy(err_msg, "ERR_JSON_CMD_UNKNOWN");
            break;
        case ERR_JSON_PROFILE_INDX:
            strcpy(err_msg, "ERR_JSON_PROFILE_INDX");
            break;
        case ERR_JSON_PROFILE_READ:
            strcpy(err_msg, "ERR_JSON_PROFILE_READ");
            break;
        case ERR_JSON_PROFILE_NAMES_RESTORE:
            strcpy(err_msg, "ERR_JSON_PROFILE_NAMES_RESTORE");
            break;
        case ERR_JSON_PROFILE_NAMES_CREATE:
            strcpy(err_msg, "ERR_JSON_PROFILE_NAMES_CREATE");
            break;
        case ERR_JSON_PROFILE_NAMES_DELETE:
            strcpy(err_msg, "ERR_JSON_PROFILE_NAMES_DELETE");
            break;
        case ERR_JSON_PROFILE_NAMES_DUPLICATE:
            strcpy(err_msg, "ERR_JSON_PROFILE_NAMES_DUPLICATE");
            break;
        case ERR_JSON_DELETE_PROFILE:
            strcpy(err_msg, "ERR_JSON_DELETE_PROFILE");
            break;
        case ERR_JSON_DUPLICATE_PROFILE:
            strcpy(err_msg, "ERR_JSON_DUPLICATE_PROFILE");
            break;
        case ERR_JSON_PARSE_NULL_BUFF:
            strcpy(err_msg, "ERR_JSON_PARSE_NULL_BUFF");
            break;
        case ERR_JSON_PARSE_BUFF_LEN:
            strcpy(err_msg, "ERR_JSON_PARSE_BUFF_LEN");
            break;
        case ERR_JSON_PARSE_NUM_TOKEN:
            strcpy(err_msg, "ERR_JSON_PARSE_NUM_TOKEN");
            break;
        case ERR_JSON_PARSE_NOT_OBJECT:
            strcpy(err_msg, "ERR_JSON_PARSE_NOT_OBJECT");
            break;
        case ERR_JSON_PARSE_UNEXPECTED_FORMAT_READ:
            strcpy(err_msg, "ERR_JSON_PARSE_UNEXPECTED_FORMAT_READ");
            break;
        case ERR_JSON_PARSE_UNEXPECTED_FORMAT_WRITE:
            strcpy(err_msg, "ERR_JSON_PARSE_UNEXPECTED_FORMAT_WRITE");
            break;
        case ERR_JSON_PARSE_UNEXPECTED_FORMAT:
            strcpy(err_msg, "ERR_JSON_PARSE_UNEXPECTED_FORMAT");
            break;
        case ERR_JSON_PARSE_UNEXPECTED_STATE:
            strcpy(err_msg, "ERR_JSON_PARSE_UNEXPECTED_STATE");
            break;
        case ERR_JSON_NO_FREE_PROFILE:
            strcpy(err_msg, "ERR_JSON_NO_FREE_PROFILE");
            break;
        case ERR_PKT_VERIFY:
            strcpy(err_msg, "ERR_PKT_VERIFY");
            break;
        case ERR_PKT_UNKNOWN:
            strcpy(err_msg, "ERR_PKT_UNKNOWN");
            break;
        case ERR_DFU_NO_FILE:
            strcpy(err_msg, "ERR_DFU_NO_FILE");
            break;
        case ERR_IMG_DBG_1:
            strcpy(err_msg, "ERR_IMG_DBG_1");
            break;
        case ERR_IMG_DBG_2:
            strcpy(err_msg, "ERR_IMG_DBG_2");
            break;
        case ERR_IMG_DBG_3:
            strcpy(err_msg, "ERR_IMG_DBG_3");
            break;
        case ERR_IMG_DBG_4:
            strcpy(err_msg, "ERR_IMG_DBG_4");
            break;

        default:
            return -1;
            break;
    }
    sprintf((char *)json_tx_e_str, "%s%s%s", json_status_error_start_str, err_msg, json_status_error_end_str);
    len = strlen((char *)json_tx_e_str);

    return (len);

};

// .............................................................................
// Construct a JSON Read Response message
// Format:
//  "{"sts":"success","data":{"key1":"val1","key2":"val2","key4":"val4"}}"
// Cycle through each key/value pair in data_tx_key_strs[]. THese are the
// key/value pairs saved when the JSON Read message was processed.
// So, now append each key/value pair to an overall response message
// (json_tx_r_str).
// Most values were stored in the data_tx_val_strs[] array, whiel the Read was
// being processed, but there are exceptions, when the values were more 
// complicated - e.g. Profile Names, and these have been saved on other 
// array/strings.
// .............................................................................

int json_build_resp_read_str()
{
    uint16_t indx;
    int len;
    int len_key;
    int len_val;
    int len_key_val;
    int i;
    
    if ((data_tx_key_val_indx == 0) || (data_tx_key_val_indx >= JSON_DATA_KEYS_MAX)) {
        // Tx array index invalid
        return -1;
    }
    
    indx = 0;
    
    // Start building the response in json_tx_r_str
    len = strlen(json_read_success_start_str);
    // strncat() needs to know the 'end' of the string to append to, so clear the string
    json_tx_i_str[0] = '\0';
    strncat((char *)json_tx_r_str, json_read_success_start_str, len);
    indx = len;
    
    // Cycle through each key/val pair, append these strings
    // Exception:
    // - For some keys, we don't append the values stored in data_tx_val_strs[].
    // - Instead we append the values which have been saved in other arrays/strings (e.g. json_tx_eq_preset_val_str))
    for (i = 0; i < data_tx_key_val_indx; i++) {
        // For each key/val pair, append these strings
        len_key = strlen(data_tx_key_strs[i]);
        len_val = strlen(data_tx_val_strs[i]);
        if ((len_key != 0) && (len_val != 0)) {
            if (!strcmp(data_tx_key_strs[i], json_key_Prof_Names)) {
                // Exception: Use 'json_rx_prof_names' string instead of data_tx_val_strs[i]
                sprintf((char *)&json_key_val_str, "%s%s%s%s%s", "\"", data_tx_key_strs[i], "\":", json_rx_prof_names, ",");
            } else if (!strcmp(data_tx_key_strs[i], json_key_Eq_Preset_Vals)) {
                // Exception: Use 'json_tx_eq_preset_val_str' string instead of data_tx_val_strs[i]
                sprintf((char *)&json_key_val_str, "%s%s%s%s%s", "\"", data_tx_key_strs[i], "\":", json_tx_eq_preset_val_str, ",");
            } else if (!strcmp(data_tx_key_strs[i], json_key_Eq_Preset_Names)) {
                // Exception: Use 'json_tx_eq_preset_nam_str' string instead of data_tx_val_strs[i]
                sprintf((char *)&json_key_val_str, "%s%s%s%s%s", "\"", data_tx_key_strs[i], "\":", json_tx_eq_preset_nam_str, ",");
            } else if (!strcmp(data_tx_key_strs[i], json_key_Eq_Preset_Defined)) {
                sprintf((char *)&json_key_val_str, "%s%s%s%s%s", "\"", data_tx_key_strs[i], "\":", data_tx_val_strs[i], ",");
            } else if (!strcmp(data_tx_key_strs[i], json_key_Eq_Presets_X)) {
                // Exception: Use 'json_tx_eq_preset_x_str' string instead of data_tx_val_strs[i]
                sprintf((char *)&json_key_val_str, "%s%s%s%s%s", "\"", data_tx_key_strs[i], "\":", json_tx_eq_preset_x_str, ",");
            } else {
                sprintf((char *)&json_key_val_str, "%s%s%s%s%s", "\"", data_tx_key_strs[i], "\":\"", data_tx_val_strs[i], "\",");
            }
            
            // Added key and val string and 6 other chars (4 quotes, 1 colon, 1 comma))
            len_key_val = strlen(json_key_val_str);
            // Now that we have built a string containing the key/value pair (and quotes, colon, comma), 
            // append this to the larger json_tx_r_str that we are building for the full read response.
            memcpy(&json_tx_r_str[indx], json_key_val_str, len_key_val);            
            
            // Update index to overall response string (json_tx_r_str)
            indx += len_key_val;
        }
    }

    if (indx != len) {
        // Only do the following if we actually added any key/value pairs
        // Remove the last comma
        indx--;
    }

    // Add closing brackets
    len = strlen(json_read_success_end_str);
    sprintf((char *)&json_tx_r_str[indx], "%s", json_read_success_end_str);
    indx += len;
    
    return indx;
}

// .............................................................................
// Build full JSON Info message
// Format:
//  "{"op":"i","pid":"n", "data":{"key1":"val1","key2":"val2","key4":"val4"}}"
// .............................................................................

int json_build_info_msg_str()
{
    uint16_t indx;
    int len;
    int len_key;
    int len_val;
    int len_key_val;
    int i;
    char key_val_str[JSON_DATA_KEY_STR_MAX_LEN + JSON_DATA_VAL_PROF_NAMES_STR_MAX_LEN + 8];
    char dataset_str[48];
    
    if ((data_i_key_val_indx == 0) || (data_i_key_val_indx >= JSON_DATA_KEYS_MAX)) {
        return -1;
    }
    
    indx = 0;
    
    // Start building the response in json_tx_i_str
    if ((data_com.prfl_in_use == 0) || (data_com.prfl_in_use > MAX_PROFILE)) {
        return -2;
    }
    sprintf(dataset_str, "%s%d%s\":{", json_info_start_str, data_com.prfl_in_use, "\",\"data");
    len = strlen(dataset_str);
    // strncat() needs to know the 'end' of the string to append to, so clear the string
    json_tx_i_str[0] = '\0';
    strncat((char *)json_tx_i_str, dataset_str, len);
    
    indx = len;
    
    for (i = 0; i < data_i_key_val_indx; i++) {
        // For each key/val pair, append these strings
        len_key = strlen(data_i_key_strs[i]);
        len_val = strlen(data_i_val_strs[i]);
        if ((len_key != 0) && (len_val != 0)) {
            
            if (!strcmp(data_i_key_strs[i], json_key_Prof_Names)) {
                // Use 'json_rx_prof_names' string instead of data_tx_val_strs[i]
                sprintf((char *)&key_val_str, "%s%s%s%s%s", "\"", data_i_key_strs[i], "\":", json_rx_prof_names, ",");
            } else if (!strcmp(data_i_key_strs[i], json_key_Eq_Preset_Vals)) {
                // Use 'json_i_eq_preset_val_str' string instead of data_tx_val_strs[i]
                sprintf((char *)&key_val_str, "%s%s%s%s%s", "\"", data_i_key_strs[i], "\":", json_i_eq_preset_val_str, ",");
            } else if (!strcmp(data_i_key_strs[i], json_key_Eq_Preset_Names)) {
                // Use 'json_i_eq_preset_nam_str' string instead of data_tx_val_strs[i]
                sprintf((char *)&key_val_str, "%s%s%s%s%s", "\"", data_i_key_strs[i], "\":", json_i_eq_preset_nam_str, ",");
            } else if (!strcmp(data_i_key_strs[i], json_key_Eq_Preset_Defined)) {
                sprintf((char *)&key_val_str, "%s%s%s%s%s", "\"", data_i_key_strs[i], "\":", data_i_val_strs[i], ",");
            } else if (!strcmp(data_i_key_strs[i], json_key_Eq_Presets_X)) {
                // Use 'json_tx_eq_preset_x_str' string instead of data_tx_val_strs[i]
                sprintf((char *)&key_val_str, "%s%s%s%s%s", "\"", data_i_key_strs[i], "\":", json_i_eq_preset_x_str, ",");
            } else {
                sprintf((char *)&key_val_str, "%s%s%s%s%s", "\"", data_i_key_strs[i], "\":\"", data_i_val_strs[i], "\",");
                //sprintf((char *)&key_val_str, "%s%s%s%s%s", "\"", data_i_key_strs[i], "\":\"", data_i_val_strs[i], "\",");
            }
            // Added key and val string and 6 other chars (4 quotes, 1 colon, 1 comma))
            len_key_val = strlen(key_val_str);
            
            // Strip the main_str at the specified index
            json_tx_i_str[indx] = '\0';
            // Append sub string to main string
            strncat((char *)json_tx_i_str + indx, key_val_str, len_key_val);            
            
            indx += len_key_val;
        }
    }

    if (indx != len) {
        // Only do the following if we actually added any key/value pairs
        // Remove the last comma
        indx--;
    }

    // Add closing brackets
    len = strlen(json_info_end_str);
    sprintf((char *)&json_tx_i_str[indx], "%s", json_info_end_str);
    indx += len;
    
    return indx;
}


// .............................................................................
// Build full JSON Image response message
// .............................................................................

int json_build_resp_img_str(uint16_t seg_no, uint16_t seg_total)
{
    uint16_t indx;
    int len;
    int len_key_val;
    char key_val_str[JSON_DATA_KEY_STR_MAX_LEN + JSON_DATA_VAL_PROF_NAMES_STR_MAX_LEN + 8];
    
    
    indx = 0;
    
    // Start building the response in json_tx_r_str
    len = strlen(json_read_success_start_str);
    // strncat() needs to know the 'end' of the string to append to, so clear the string
    json_tx_i_str[0] = '\0';
    strncat((char *)json_tx_r_str, json_read_success_start_str, len);
    indx = len;

    json_img_seq_cntr++;

    sprintf((char *)&key_val_str, "%s%s%s%d%s", "\"", "seg_cnt", "\":\"", json_img_seq_cntr, "\",");
    len_key_val = strlen(key_val_str);
    memcpy(&json_tx_r_str[indx], key_val_str, len_key_val);            
    indx += len_key_val;
    
    sprintf((char *)&key_val_str, "%s%s%s%d%s", "\"", "seg_num", "\":\"", seg_no, "\",");
    len_key_val = strlen(key_val_str);
    memcpy(&json_tx_r_str[indx], key_val_str, len_key_val);            
    indx += len_key_val;
    sprintf((char *)&key_val_str, "%s%s%s%d%s", "\"", "seg_tot", "\":\"", seg_total, "\"");
    len_key_val = strlen(key_val_str);
    memcpy(&json_tx_r_str[indx], key_val_str, len_key_val);            
    indx += len_key_val;

    // Add closing brackets
    len = strlen(json_read_success_end_str);
    sprintf((char *)&json_tx_r_str[indx], "%s", json_read_success_end_str);
    indx += len;
    
    return indx;
}

// .............................................................................
// Construct a JSON tx string
// Look at which type of message needs to be built, and call the appropriate JSON build function.
// 
// Inputs:
//  tx_buf      Buffer to write to
//  tx_buf_len  DFata Length to write to
//  id          Indicates wjhat type of JSON message is to be built (e.g. JSON Read Response)
//  send_num    Send Sequence Number fo this message
//  err_no      Error ID, if sending JSON error message
//  seg_num     Segment number for this JSON message, if this is an Image (or Large File - will be used for DFU)
//  seg_tot     Total number of segments in this file (if sending image/DFU))
// Outputs:
//  tx_buf      Buffer containing full formatted message, ready for tx
//  tx_buf_len  Length of formatted message
// .............................................................................
int json_build_tx_str(uint8_t *tx_buf, uint16_t *tx_buf_len, uint8_t id, uint8_t send_num, uint8_t err_no, uint16_t seg_num, uint16_t seg_tot)
{
    int len = 0;
    int res = -1;
    
    switch (id) 
    {
        case COMMS_JSON_SUCESS:
            // Need to build JSON Success Status message
            len = json_build_success_str();
            if (len > 0) {
                res = json_tx_msg(tx_buf, tx_buf_len, json_tx_w_str, len, id, send_num);
            }
            break;
        case COMMS_JSON_ERROR:
            // Need to build JSON Error Status message
            len = json_build_error_str(err_no);
            if (len > 0) {
                res = json_tx_msg(tx_buf, tx_buf_len, json_tx_e_str, len, id, send_num);
            }
            break;
        case COMMS_JSON_READ_RESP:
            // Need to build full read JSON Read Response message
            len = json_build_resp_read_str();
            if (len > 0) {
                res = json_tx_msg(tx_buf, tx_buf_len, json_tx_r_str, len, id, send_num);
            }
            break;
        case COMMS_JSON_INFO:
            // Need to build a JSON Info message
            len = json_build_info_msg_str();
            if (len > 0) {
                res = json_tx_msg(tx_buf, tx_buf_len, json_tx_i_str, len, id, send_num);
            }
            // Clear the INFO key/val arrays
            key_val_i_arr_indx_init();
            break;
        case COMMS_JSON_IMAGE:
            // Need to build full JSON Image response message
            len = json_build_resp_img_str(seg_num, seg_tot);
            if (len > 0) {
                res = json_tx_msg(tx_buf, tx_buf_len, json_tx_r_str, len, id, send_num);
            }
            break;
        default:
            break;
    }
    return (res);
}
// .............................................................................

// *****************************************************************************
