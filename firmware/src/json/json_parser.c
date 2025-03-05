/******************************************************************************
  @File Name
    json_parser.c

  @Summary
    JSON Parser

  @Description

    
*******************************************************************************/
#include "json.h"
#include "json_parser.h"
#include "json_data.h"
#include "jsmn.h"
#include <ctype.h> 

// .............................................................................

jsmn_parser parser_inst;
jsmntok_t tokens1[JSON_MAX_TOKENS];                 // Maximum number of tokens expected
uint8_t global_dataset_indx;

char json_word_OPERATION[] = "op";
char json_word_WRITE[] = "w";
char json_word_READ[] = "r";
char json_word_INFO[] = "i";
char json_word_COMMAND[] = "c";
char json_word_CMD[] = "cmd";
char json_word_PROFILE[] = "pid";
char json_word_STATUS[] = "sts";
char json_word_SUCCESS[] = "success";
char json_word_ERROR[] = "error";

char json_rx_key_str[JSON_DATA_KEY_STR_MAX_LEN+1];
char json_rx_val_str[JSON_DATA_VAL_STR_MAX_LEN+1];

uint8_t json_rx_send_num;                           // Send Num received in header of JSON message
uint8_t json_rx_ack_num;                            // Ack Num received in header of JSON message


// .............................................................................
// Allow other files to read the Send Sequence number that was received in the 
// JSON message from Core app.
// This function is needed when building a JSON response message to send back
// to the Core app.
// .............................................................................
uint8_t json_parse_send_num_get()
{
    return (json_rx_send_num);
}


// .............................................................................
// Compare token string with the provided string, and see if they are the same.
//
// Example call:
//  json_parse_eq(json_rx_buf, &tokens1[1], json_word_OPERATION) == 0)
//
// Inputs:
//  json    Pointer to start of JSON string
//  tok     Pointer to struct, which contains information on the token
//          (type, start, end, size)
//  s       String to compare token string with.
// Returns:
 // 0   The two strings are the same
//  -1  The strings are different
// .............................................................................
static int json_parse_eq(const char *json, jsmntok_t *tok, const char *s)
{
    if (tok->type == JSMN_STRING && (int)strlen(s) == tok->end - tok->start &&
        strncmp(json + tok->start, s, tok->end - tok->start) == 0)
    {
        return 0;
    }
    return -1;
}

// .........................................................................................................
// Given a JSON string, and a token, extract the token string from the JSON string
//
// For example, if this is the start of a received JSION string:
//  {
//    "op": "w",
//    "pid": "0",
// Then associated token strings are listed below:
//
//      token index     token string
//      [1]             "op"
//      [2]             "w"
//      [3]             "pid"
//      [4]             "0"
// .........................................................................................................
int json_parse_get_token_str(char *json_str, jsmntok_t *token, char *token_str, int max_len)
{
    int len;

    len = token->end - token->start;
    if (len <= 0)
    {
        return -1;
    }
    if (len > max_len)
    {
        return -2;
    }
    // Copy substring to new string
    strncpy(token_str, json_str + token->start, len);
    // Manually add the null terminator
    token_str[len] = '\0';
    return (len);
}

// ................................................................................
// Given a JSON string, want to know which profile it belongs to.
//
// For example, if this is the start of a received JSION string:
//  {
//    "op": "w",
//    "pid": "0",
// then the token index references the "0" after the "pid"
//
// The profile, or dataset, will be 0 for common data, and 1..n for a Profile.
// Inputs:
//   json:	original JSON string
//   tok:	token
// Outputs:
//   ds_indx: profile num
// Returns
//   err	0 = success, else error
// ................................................................................

int json_parse_get_dataset(const char *json, jsmntok_t *tok, uint8_t *ds_indx)
{
    int tok_len;
    char profile_ch;
    uint8_t prof_num;

    // Length of the token
    tok_len = tok->end - tok->start;
    // Token should just contain the Profile Number (1 digit)
    if (tok_len != 1)
    {
         // Token length does not match the pattern
        return (-1); 
    }     

    profile_ch = json[tok->start]; 
    if (isdigit(profile_ch))
    {
        // Token matches the pattern
        prof_num = (uint8_t)(profile_ch - '0');
        *ds_indx = prof_num;
        return (0);  
    } else {
        // Received char is not a number
        return (-2);         
    }
}

// ................................................................................
// EQ Preset write received, so parse and process the received key/value pair.
// Note: The "vaLue" will contain one or more sub key/value paIrs.
//
// When parsing EQ Preset key/value data, this function extracts and verifies 
// the format of any of the sub-keys (eqId, eqPn, eqPd, eqPv), and eventually 
// copies any received data into its local copy for EQ Preset data
// (data_com.eq_pre_data[]).
//
// Example JSON EQ Preset write message:
// 
// {
//   "op": "w",
//   "pid": "0",
//   "data": {
//     "eqPx":[{
//       "eqId": "4",
//       "eqPn": "eq name 4",
//       "eqPd": "false",
//       "eqPv": ["1","2","3","4","5"],
//     },{
//       "eqId": "5",
//       "eqPn": "eq name 5",
//       "eqPd": "false",
//       "eqPv": ["4","8","7","6","5"],
//     }]
//   }
//
// How this looks when parsed:
//                                          token index     token type
//                                          -----------     ----------
//  {                                       [0]             OBJ
//      "op": "w",                          [1][2]          STR.STR
//      "pid": "0",                         [3][4]          STR.STR
//      "data": {                           [5][6]          STR.OBJ
//        "eqPx":[                          [7][8]          STR.ARR
//         {                                [9]             OBJ
//          "eqId": "4",                    [10][11]        STR.STR
//          "eqPn": "eq name 4",            [12][13]        STR.STR
//          "eqPd": "false",                [14][15]        STR.STR
//          "eqPv": ["1","2","3","4","5"],	[16][17][18..22]STR.ARR.STR..STR
//        },{                               [23]            OBJ
//          "eqId": "5",                    [24][25]        STR.STR
//          "eqPn": "eq name 5",            [26][27]        STR.STR
//          "eqPd": "false",                [28][29]        STR.STR
//          "eqPv": ["4","8","7","6","5"],	[30][31][32..36]STR.ARR.STR..STR
//        }
//       ]
//      }
//  }
//
//
// Inputs:
//  tokens      Pointer to tokens table
//  t_indx      Token index to start with ([7] above)
//  num_t       Total number of NESTED tokenss
//  rx_buf      Buffer containing the received JSON string
// Outputs
//  t_cnt       Number of tokens processed (so we know how much to increment t_indx by)
// Returns:
//  err         < 0 = fail, 0 = success
//
// NOTE: Will need to check, when cycling through the nested tokens, that we do not 
// exceed the max/total token count (based on num_t and t_indx))
// ................................................................................
int json_parse_kv_eq_preset(jsmntok_t *tokens, uint8_t t_indx, int num_t, char *rx_buf, uint8_t *t_cnt)
{
    int len_key = 0;
    int len_val = 0;
    uint8_t tk_indx;
    int err = 0;
    char key_str[JSON_DATA_KEY_STR_MAX_LEN+1];
    char val_str[JSON_DATA_VAL_STR_MAX_LEN+1];    
    bool b_updated = false;                 // Assume nothing will be written/updated
    bool b_keep_going_outer = true;
    bool b_keep_going_inner = true;
    data_eq_pre_t eq_data;
    bool b_indx_rcvd;
    bool b_vals_rcvd;
    bool b_name_rcvd;
    bool b_state_rcvd;
    uint8_t eq_id;
    int len;
    
    // Token index - we need to keep track of the number of tokens we process
    tk_indx = t_indx;
    *t_cnt = 0;                              // Number of tokens incremented through

    
    // ...........................................
    // [7] "eqPx"
    // Extract the first key (to json_rx_key_str) and its length
    len_key = json_parse_get_token_str(rx_buf, &tokens[tk_indx], key_str, JSON_DATA_KEY_STR_MAX_LEN);

    // For now, only supporting EQ Preset key
    if (strcmp(key_str, json_key_Eq_Presets_X) != 0) {
        // 2-d array is not EQ Presets
        json_fn_send_status_error(ERR_JSON_EQ_PRESET_1);
        return (-1);
    }
    tk_indx++;
    // ...........................................
    // [8] ARR [
    if (tokens[tk_indx].type != JSMN_ARRAY) {
        // Expecting start of array
        json_fn_send_status_error(ERR_JSON_EQ_PRESET_2);
        return (-2);
    }   
    // ...........................................
    // Move on to start of nested keys
    tk_indx++;
    
    while (b_keep_going_outer) 
    {
        // [9] OBJ {
        if (tokens[tk_indx].type != JSMN_OBJECT) {
            // No more preset groups, so finished
            b_keep_going_outer = false;
            break;
        }   
        tk_indx++;
        
        b_keep_going_inner = true;
        // Assume will not receive valid data for this EQ Preset
        b_indx_rcvd = false;
        b_vals_rcvd = false;;
        b_name_rcvd = false;;
        b_state_rcvd = false;;
        
        // Now search inner loop for key/vals....
        while (b_keep_going_inner) 
        {
            if (tk_indx >= (num_t - 1)) {                                                           // <-- check this!!!
                // Must have at least 2 tokens before continuing, at start of each loop
                // (e.g. if num_t is 10, then indices are 0..9, so current index can be 8 at most)
                b_keep_going_inner = false;
                break;
            }
            if ( (tokens[tk_indx].type == JSMN_OBJECT) || (tokens[tk_indx].type == JSMN_UNDEFINED) ) {
                // No more key/vals to read with this preset, so move on to next (or finsih))
               b_keep_going_inner = false;
               break;
            }
        
            // Extract the first nest key and value (to json_rx_key_str and json_rx_val_str) and their lengths
            len_key = json_parse_get_token_str(rx_buf, &tokens[tk_indx++], key_str, JSON_DATA_KEY_STR_MAX_LEN);
            len_val = json_parse_get_token_str(rx_buf, &tokens[tk_indx], val_str, JSON_DATA_VAL_STR_MAX_LEN);
            // Note: Didn't increment tk_indx
            
            // Have already processed the eqPx key, now want to detect and process any of the following:
            // eqId: eqPreset ID - e.g. "1"
            // eqPn: eqPreset Name - e.g. "My eqPreset"
            // eqPv: eqPreset Values (array) - e.g. ["1","2","-3,"4","5"]
            // eqPd: eqPreset Defined - e.g. "true" (i.e. enabled or not))
            
            if (strcmp(key_str, json_key_Eq_Preset_Id_X) == 0) 
            {
                // eqId: eqPreset ID
                // Convert string to value. Value should be valid and in range
                err = utils_str_to_uint8(val_str, &eq_id);
                // EQ Preset ID is aved in: eq_id
                if (err < 0) {
                    // Can't convert to id
                    json_fn_send_status_error(ERR_JSON_EQ_PRESET_3);
                    return (-3);
                }
                if ((eq_id == 0) || (eq_id > MAX_EQ_PRESETS)) {
                    // ID is out of range
                    json_fn_send_status_error(ERR_JSON_EQ_PRESET_4);
                    return (-4);
                }
                // Received valid Id
                b_indx_rcvd = true;
                // Move on to next key
                tk_indx++;
            } 
            else if (strcmp(key_str, json_key_Eq_Preset_Vals) == 0) 
            {
                // eqPv: eqPreset Values
                if (tk_indx >= (num_t - EQ_ARRAY_SIZE)) {      // <-- check this
                    // Not enough tokens remaining
                    json_fn_send_status_error(ERR_JSON_EQ_PRESET_5);
                    return (-5);
                }
                if (tokens[tk_indx].type != JSMN_ARRAY) {
                    // This token should be array (square bracket for start of array)
                    json_fn_send_status_error(ERR_JSON_EQ_PRESET_6);
                    return (-6);
                }
                tk_indx++;
                // Grab the values, if they are valid etc.
                for (int i = 0; i < EQ_ARRAY_SIZE; i++) {
                    if (tokens[tk_indx].type != JSMN_STRING) {
                        // Token should be a string
                        json_fn_send_status_error(ERR_JSON_EQ_PRESET_7);
                        return (-7);
                    }
                    len_val = json_parse_get_token_str(rx_buf, &tokens[tk_indx], val_str, JSON_DATA_VAL_STR_MAX_LEN);
                    if (len_val == 0) {
                        // Invalid length
                        json_fn_send_status_error(ERR_JSON_EQ_PRESET_8);
                        return (-8);
                    }
                    // Convert and copy to eq data
                    err = utils_str_to_int8(val_str, &eq_data.vals[i]);
                    // EQ Preset Values are saved in: eq_data.vals[]
                    if (err < 0) {
                        // Conversion failed
                        json_fn_send_status_error(ERR_JSON_EQ_PRESET_9);
                        return (-9);
                    }
                    // Received valid values
                    b_vals_rcvd = true;
                    tk_indx++;
                }
                b_updated = true;                
            }
            else if (strcmp(key_str, json_key_Eq_Preset_Names) == 0) 
            {
                // eqPn: eqPreset Name
                // Make sure name is not too long
                len = strlen(val_str);
                if (len >= JSON_DATA_VAL_STR_MAX_LEN) {
                    // EQ Preset Name is too long
                    json_fn_send_status_error(ERR_JSON_EQ_PRESET_10);
                    return (-10);
                }
                // Update value
                strcpy(eq_data.name, val_str);
                // EQ Preset Name is saved in: eq_data.name
                // Received valid Name
                b_name_rcvd = true;
                // At least one write/update
                b_updated = true;
                // Move on to next key
                tk_indx++;
            }
            else if (strcmp(key_str, json_key_Eq_Preset_Defined) == 0)
            {
                // eqPd: eqPreset Defined
                // Convert received value to boolean
                err = utils_str_to_bool(val_str, &eq_data.defined) ;
                // EQ Preset Defined is saved in: eq_data.defined
                if (err < 0) {
                    // Failed to convert state
                    json_fn_send_status_error(ERR_JSON_EQ_PRESET_11);
                    return (-11);
                }
                // Received valid State
                b_state_rcvd = true;
                // At least one write/update
                b_updated = true;
                // Move on to next key
                tk_indx++;
            } else {
                // Unknown key - error for now
                json_fn_send_status_error(ERR_JSON_EQ_PRESET_12);
                return (-12);
            }
        }
        
        if ((b_indx_rcvd) && (b_updated)) {
            // Receive valid index, and at least one key/val pair, so write
            // the data
            if (!b_vals_rcvd) {
                for (int i = 0; i < EQ_ARRAY_SIZE; i++) {
                    eq_data.vals[i] = data_com.eq_pre_data[eq_id-1].vals[i];
                }
            }
            if (!b_name_rcvd) {
                strcpy(eq_data.name, data_com.eq_pre_data[eq_id-1].name);
            }
            if (!b_state_rcvd) {
                eq_data.defined = data_com.eq_pre_data[eq_id-1].defined;
            }
            // Copy any unwritten data to eq_data, so original data not overwritten with unknown data
            err = json_fn_w_eq_preset_X(eq_id, &eq_data);
            if (err < 0) {
                // Failed to update EQ Preset Data
                json_fn_send_status_error(ERR_JSON_EQ_PRESET_13);
                return (-13);
            }
        }
   
    }
    
    if (b_updated) {
        // At least one successful write/update
        *t_cnt = tk_indx - t_indx;
    }
    return 0;
}

// ................................................................................
// Have received a JSON WRITE message
// Go through each key-value pair, and update local copies.
// Will also have to call an action function associated with each key that
// is being written
// ................................................................................
int json_parse_kv_pairs(jsmntok_t *tokens, uint8_t t_indx, int num_t, char *rx_buf)
{
    bool keep_going;
    int len_key = 0;
    int len_val = 0;
    int err = 0;
    uint8_t key_cntr = 0;
    char val_str[4];
    keep_going = true;
    uint8_t t_cnt;

    // Get here if have received a WRITE message.
    // We expect one or more Key/Value pairs now
    // Cycle through each Key/Value pair
    
    // Clear the Rx key/value arrays (by resetting the array index)
    key_val_rx_arr_indx_init();

    while (keep_going == true)
    {
        k_sleep(K_MSEC(100));
        
        if ((tokens[t_indx].type == JSMN_STRING) && (tokens[t_indx+1].type == JSMN_STRING))
        {
            // This looks like there is a single Value associated with the key....
            // Extract the key and value (to json_rx_key_str and json_rx_val_str) and their lengths
            len_key = json_parse_get_token_str(rx_buf, &tokens[t_indx], json_rx_key_str, JSON_DATA_KEY_STR_MAX_LEN);
            len_val = json_parse_get_token_str(rx_buf, &tokens[t_indx+1], json_rx_val_str, JSON_DATA_VAL_STR_MAX_LEN);
            if ((len_key < 0) || (len_val < 0)) {
                keep_going = false;
            } else {
                // Key/value pair seems to be valid.
                // Add key/value pair to array, so can call functions later and build response
                err = json_add_rx_key_val(json_rx_key_str, len_key, json_rx_val_str, len_val);
                if (err == 0) {
                    key_cntr++;
                }
                // Have processed two more tokens from the JSON message
                t_indx += 2;
                if (t_indx >= num_t) {
                    keep_going = false;
                }
            }
        }
        else if ((tokens[t_indx].type == JSMN_STRING) && (tokens[t_indx+1].type == JSMN_ARRAY))
        //else if ((tokens[t_indx].type == JSMN_STRING) && (tokens[t_indx+1].type == JSMN_OBJECT))
        {
            // This looks like there is an array associated with the key....
            // Which means, for this application, that this should be a WRITE 
            // for EQ Presets (as this is the only array write currently supported by the code).
            
            // TODO: Update the following comment with better a bette rdescription.
            // Parse the EQ Preset write key/values now, and process the received data immediately.
            err = json_parse_kv_eq_preset(tokens, t_indx, num_t, rx_buf, &t_cnt);
            
            if (err < 0) {
                return (-1);
            }
            if (t_cnt > 0) {
                // There is at least one more token to process
                // Pre-populate val_str[] so not passing empty string to function
                val_str[0] = ' ';
                val_str[1] = '\0';
                // Add EQ Preset key to Rx Key Array
                err = json_add_rx_key_val(json_key_Eq_Presets_X, strlen(json_key_Eq_Presets_X), val_str, strlen(val_str));
                if (err < 0) {
                    // Failed to add EQ Preset key
                    json_fn_send_status_error(ERR_JSON_ARRAY_SIZE_2);
                    return (-3);
                }
                key_cntr++;
                t_indx += t_cnt;
                // Have processed several more tokens in the JSONI message (depends on size of EQ Presets array)
                if (t_indx >= num_t) {
                    keep_going = false;
                }
            } else {
                // no tokens to process, so nothing to do
            }
        }
        else
        {
            // Invalid format
            keep_going = false;
        }
        
    }

    if (key_cntr > 0) {
        // At least one key accepted, so process the writes(s) and build the JSON response string
        err = json_func_process_write(global_dataset_indx, key_cntr);
    }
    if (err < 0) {
        json_fn_send_status_error(ERR_JSON_KV_ERR);
    }

    return (err);
}

// ................................................................................
// Have received a JSON READ message
// Add each key to the Rx Key Array. This array will be used later, to call 
// the associated read function for each key in the array, and build the 
// required JSON response.
// ................................................................................
int json_parse_read(jsmntok_t *tokens, uint8_t t_indx, int num_t, char *rx_buf)
{
    bool keep_going;
    int num_parsed = 0;
    int len_key = 0;
    int err = 0;
    uint8_t key_cntr = 0;

    keep_going = true;

    // Clear the Rx key/value arrays (by resetting the array index)
    key_val_rx_arr_indx_init();

    // Cycle through each key
    while (keep_going == true)
    {
        // TODO: I don't think this delay is required
        k_sleep(K_MSEC(100));

        // Only expecting strings - i.e. the key names for each variable to be read
        if (tokens[t_indx].type == JSMN_STRING)
        {
            // Get the key
            // Extract the key (to json_rx_key_str) and its length
            len_key = json_parse_get_token_str(rx_buf, &tokens[t_indx], json_rx_key_str, JSON_DATA_KEY_STR_MAX_LEN);
            if (len_key < 0) {
                json_fn_send_status_error(ERR_JSON_KEY_LEN);
                return (-1);
            } else {
                // The key is valid.
                // Get the variable value (in string format) associated with the key.
                // This will also append the key/value pair to the success response that is being built.
                
                // Add key to array, so can call read function later for each key and build response
                err = json_add_rx_key(json_rx_key_str, len_key);
                if (err == 0) {
                    // Keep track of how many Keys have been added to the array
                    key_cntr++;
                }
                // Have processed one more token in the JSON message
                t_indx += 1;
                if (t_indx >= num_t) {
                    keep_going = false;
                }
            }
        } else {
            // Invalid format
            keep_going = false;
        }
    }
    
    if (key_cntr > 0) {
        // At least one key accepted, so process the read(s) and build the JSON response string
        err = json_func_process_read(global_dataset_indx, key_cntr);
    }
    if (err < 0) {
        json_fn_send_status_error(ERR_JSON_READ_ERR);
    }
    
    return (num_parsed);
}

// ................................................................................
// Have received a JSON COMMAND message.
// Extract the command, and call the associated function.
// Note: For now, this function only supports ONE command per message.
// ................................................................................
int json_parse_command(jsmntok_t *tokens, uint8_t t_indx, int num_t, char *rx_buf)
{
    int len_key = 0;
    int len_val = 0;
    int err = 0;
    uint8_t key_cntr = 0;

    // Get here if have received a COMMAND.
    // We expect one Key/Value pair now.
    
    // Clear the Rx key/value arrays (by resetting the array index)
    key_val_rx_arr_indx_init();

    // We expect 2 strings now (command's Key and Value)
    if ((tokens[t_indx].type == JSMN_STRING) && (tokens[t_indx+1].type == JSMN_STRING))
    {
        // Extract the key and value (to json_rx_key_str and json_rx_val_str) and their lengths
        len_key = json_parse_get_token_str(rx_buf, &tokens[t_indx], json_rx_key_str, JSON_DATA_KEY_STR_MAX_LEN);
        len_val = json_parse_get_token_str(rx_buf, &tokens[t_indx+1], json_rx_val_str, JSON_DATA_VAL_STR_MAX_LEN);
        
        if (len_key < 0) {
            // Invalid key format
            json_fn_send_status_error(ERR_JSON_CMD_KEY);
            return (-1);
        }
        if (len_val < 0) {
            // Invalid value format
            json_fn_send_status_error(ERR_JSON_CMD_VAL);
            return (-2);
        }
        
        // Add key to array, so can call functions associated with keys later and build response
        // In this case, for a command, will only be adding the one key
        err = json_add_rx_key_val(json_rx_key_str, len_key, json_rx_val_str, len_val);
        if (err == 0) {
            key_cntr++;
        }
    } else {
        // Invalid key format
        json_fn_send_status_error(ERR_JSON_CMD_FORMAT);
        return (-3);
    }

    if (key_cntr > 0) {
        // At least one key accepted (in this case, for a command, there will 
        // only be one or zero commands), so process the command(s) and build 
        // the JSON response string
        err = json_func_process_cmd(global_dataset_indx, key_cntr);
    }
    // The above function will look after sending success/fail message

    return (err);    
}

// .............................................................................
// Parse the received JSON message.
// Inputs
//  json_rx_buf:    Received JSON string
//                  This string will have been isolated from a buffer received 
//                  from Airoha
//  -   Parse the received message
//  -   Identify all the tokens (start of Object, Array, String, Primitive) in 
//      the JSON string
//  -   Identify if the received message is a Write, Read or Command
//  -   Identify which profile/dataset the JSON operation is for (e.g. common 
//      data, or one of the profiles)
//  -   Call the appropriate parse function to extract/process each Read key, 
//      or Write key/value pair, or Command key, and eventually call the 
//      associated function to carry oput the Read / Write / Command
// .............................................................................
int json_parse_msg(char *json_rx_buf)
{
    int num_token;
    int err;
    uint8_t state;
    uint8_t tok_indx = 0;
    int len;
    
    if (json_rx_buf == NULL)
    {
        // No buffer received - send error response back to Core
        json_fn_send_status_error(ERR_JSON_PARSE_NULL_BUFF);
        return -1;
    }
    len = strlen(json_rx_buf);
    if (len <= 0)
    {
        // Something wrong with buffer length
        json_fn_send_status_error(ERR_JSON_PARSE_BUFF_LEN);
        return -2;
    }

    // Initialise the JSMN parser module
    jsmn_init(&parser_inst);
    
    // Parse the received JSON string, to identify all the tokens
    num_token = jsmn_parse(&parser_inst, json_rx_buf, len, tokens1, sizeof(tokens1) / sizeof(tokens1[0]));
    
    if (num_token < 0)
    {
        // No tokens found
        json_fn_send_status_error(ERR_JSON_PARSE_NUM_TOKEN);
        return -3;
    }

    // Assume the top-level element is an object
    if (num_token < 1 || tokens1[0].type != JSMN_OBJECT)
    {
        // First top-level element is not an object
        json_fn_send_status_error(ERR_JSON_PARSE_NOT_OBJECT);
        return -4;
    }

    // Have verified that 1st token is OBJECT
    // Cycle through the rest of the tokens

    state = STATE_EXPECT_OP_OR_STATUS;
    
    // Examine first few tokens to see what type of message has been received
    
    // Note: The code below is highly-strict logic regarding the expected format of the received JSON
    
    // Check 1: See if this is a READ message.......
    
    // For READ, expect the format below, and at least one key, so minimum 8 tokens
    //         [0]  [1]  [2] [3]   [4]  [5]   [6]  [7]
    // Format: OBJ | op | r | pid | n | data | [ | key1 | key 2 ....
    if ((num_token >= 8) &&
        ((tokens1[1].type == JSMN_STRING) && (json_parse_eq(json_rx_buf, &tokens1[1], json_word_OPERATION) == 0)) &&
        ((tokens1[2].type == JSMN_STRING) && (json_parse_eq(json_rx_buf, &tokens1[2], json_word_READ) == 0)) &&
        ((tokens1[3].type == JSMN_STRING) && (json_parse_eq(json_rx_buf, &tokens1[3], json_word_PROFILE) == 0)) &&
        ( tokens1[4].type == JSMN_STRING) &&    // profile number
        ((tokens1[5].type == JSMN_STRING) && (json_parse_eq(json_rx_buf, &tokens1[5], "data") == 0)) &&
        ( tokens1[6].type == JSMN_ARRAY) ) 
    { 
        // It looks like a READ message...
        // Identify which profile/dataset the JSON operation is for (e.g. common data, or one of the profiles)
        // (2nd value should indicate profile number (0..n))
        if (json_parse_get_dataset(json_rx_buf, &tokens1[4], &global_dataset_indx) == 0)
        {
            // Looks like a valid READ message...
            // op | read | profile | n | data| obj
            state = STATE_EXPECT_READ_KEYS;
            // Index to first key
            tok_indx = 7;
        } else {
            // error - unexpected format
            json_fn_send_status_error(ERR_JSON_PARSE_UNEXPECTED_FORMAT_READ);
            state = STATE_ERROR;
        }
    }
    
    // Check 2: See if this is a WRITE message.......
    
    // For WRITE, expect the format below, and at least one key/value pair, so minimum 9 tokens
    //         [0]  [1]  [2] [3]   [4]  [5]   [6]  [7]   [8]     [9]
    // Format: OBJ | op | w | pid | n | data | { | key1 | val1 | key 2 | val2 ....
    else if ((num_token >= 9) &&
        ((tokens1[1].type == JSMN_STRING) && (json_parse_eq(json_rx_buf, &tokens1[1], json_word_OPERATION) == 0)) &&
        ((tokens1[2].type == JSMN_STRING) && (json_parse_eq(json_rx_buf, &tokens1[2], json_word_WRITE) == 0)) &&
        ((tokens1[3].type == JSMN_STRING) && (json_parse_eq(json_rx_buf, &tokens1[3], json_word_PROFILE) == 0)) &&
        ( tokens1[4].type == JSMN_STRING) &&    // profile number
        ((tokens1[5].type == JSMN_STRING) && (json_parse_eq(json_rx_buf, &tokens1[5], "data") == 0)) &&
        ( tokens1[6].type == JSMN_OBJECT) ) 
    {
        // It looks like a WRITE message...
        // Identify which profile/dataset the JSON operation is for (e.g. common data, or one of the profiles)
        // (2nd value should indicate profile number (0..n))
        if (json_parse_get_dataset(json_rx_buf, &tokens1[4], &global_dataset_indx) == 0)
        {
            // Looks like a valid WRITE message...
            // op | write | profile | n | data| obj
            state = STATE_EXPECT_WRITE_KV_PAIRS;
            // Index to first key
            tok_indx = 7;
        } else {
            // error - unexpected foramt
            json_fn_send_status_error(ERR_JSON_PARSE_UNEXPECTED_FORMAT_WRITE);
            state = STATE_ERROR;
        }
    }

    // Check 3: See if this is a COMMMAND message.......
    
    // For COMMMAND, expect the format below, and at least one key/value pair, so minimum 9 tokens
    //         [0]  [1]  [2] [3]   [4]  [5]   [6]  [7]   [8]     [9]
    // Format: OBJ | op | c | pid | n | data | { | key1 | val1 | key 2 | val2 ....
    else if ((num_token >= 9) &&
        ((tokens1[1].type == JSMN_STRING) && (json_parse_eq(json_rx_buf, &tokens1[1], json_word_OPERATION) == 0)) &&
        ((tokens1[2].type == JSMN_STRING) && (json_parse_eq(json_rx_buf, &tokens1[2], json_word_COMMAND) == 0)) &&
        ((tokens1[3].type == JSMN_STRING) && (json_parse_eq(json_rx_buf, &tokens1[3], json_word_PROFILE) == 0)) &&
        ( tokens1[4].type == JSMN_STRING) &&    // profile number
        ((tokens1[5].type == JSMN_STRING) && (json_parse_eq(json_rx_buf, &tokens1[5], "data") == 0)) &&
        ( tokens1[6].type == JSMN_OBJECT) ) 
    {
        // It looks like a COMMMAND message...
        // Identify which profile/dataset the JSON operation is for (e.g. common data, or one of the profiles)
        // (2nd value should indicate profile number (0..n))
        if (json_parse_get_dataset(json_rx_buf, &tokens1[4], &global_dataset_indx) == 0)
        {
            // Looks like a valid COMMMAND message...
            // op | write | profile | n | data| obj
            state = STATE_EXPECT_COMMAND;
            // Index to first key
            tok_indx = 7;
        } else {
            // error - unexpected foramt
            json_fn_send_status_error(ERR_JSON_PARSE_UNEXPECTED_FORMAT_COMMAND);
            state = STATE_ERROR;
        }
    }
    
    // Unsupported/unknown message type.......
    
    else {
        // error - unexpected format
        json_fn_send_status_error(ERR_JSON_PARSE_UNEXPECTED_FORMAT);
        state = STATE_ERROR;
    }    
    
    // Have now determined what type of message has been received, so now process the rest of the message

    switch (state)
    {
        // ..................................................................
        case STATE_EXPECT_READ_KEYS:
            // Parse this READ message, and append all requested k/v pairs to success response
            err = json_parse_read(tokens1, tok_indx, num_token, json_rx_buf);
            if (err < 0)
            {
                return (err);
            }
            break;
        // ..................................................................
        case STATE_EXPECT_WRITE_KV_PAIRS:

            // Parse this WRITE message, and will build a status response
            err = json_parse_kv_pairs(tokens1, tok_indx, num_token, json_rx_buf);
            if (err < 0)
            {
                return (err);
            }
            break;
        // ..................................................................
        case STATE_EXPECT_COMMAND:
            // Parse this COMMAND message, and append function name to success response
            err = json_parse_command(tokens1, tok_indx, num_token, json_rx_buf);
            if (err < 0)
            {
                return (err);
            }
            break;
        // ..................................................................
        case STATE_ERROR:
            break;
        // ..................................................................
        default:
            json_fn_send_status_error(ERR_JSON_PARSE_UNEXPECTED_STATE);
            return -8;
            break;
        // ..................................................................
    }
    
    return 0;
}

// .............................................................................
// This function processes packet received from Core app (via Airoha AB1577).
// Packet is in JSON Protocol Format.
//
//  PAYLOAD: JSON
//
// Steps:
//  1: Verify source/destination, and sequence numbers etc.
//  2. Extract received Message ID and Value
//  3. Call function associated with the received Message ID
//
// Inputs:
//  rx_pkt:     Received packet (from Core/Airoha)
//  src_id      Received Source ID
//  dest_id     Received Destination ID
//  send_num    Received Send Number
//  ack_num     Received ACK Number
// Returns:
//      err:    0:      success
//               -ve:   error
// .............................................................................
int json_rx_process(uint8_t *rx_pkt, uint8_t src_id, uint8_t dest_id, uint8_t send_num, uint8_t ack_num)
{
    int err = 0;
    uint8_t *json_array;
    uint16_t pay_len;

    if (rx_pkt == NULL)
    {
        // Nothing to do
        return -1;
    }
    
    // Verify source/destination: only interested in packets from Core app (for now))
    if (src_id != COMMS_ID_CORE)
    {
        // Packet is not from valid source
        return -3;
    }
    if (dest_id != COMMS_ID_MCU)
    {
        // Packet is not for MCU
        return -4;
    }

   
    // Get pointer to start of JSON message
    json_array = &rx_pkt[COMMS_BYTE_7_DATA];
    // Get payload length
    pay_len = (rx_pkt[COMMS_BYTE_0_PAYLOAD_HI] * 256) + rx_pkt[COMMS_BYTE_1_PAYLOAD_LO];
    // Insert null char at end of payload
    json_array[pay_len] = 0;
    
    // Remember the Send Num and Ack Num recveived in header of received message.
    // Will be needed if generating a response.
    json_rx_send_num = send_num;            // Send Num received in header of JSON message
    json_rx_ack_num = ack_num;              // Ack Num received in header of JSON message    
    
    // Parse the JSON string
    err = json_parse_msg((char *)json_array);
    if (err < 0) {
        // parsing error
        return -5;
    }

    // Success
    return 0;
}

// .............................................................................

// *****************************************************************************
