/*******************************************************************************
  File Name:
    app_com.c

  Summary:
    Commmunications interface to/with Airoha chip.

  Description:
 *******************************************************************************/
#include "app_json.h"
#include "json/json.h"

// .............................................................................
// This delay defines max frequency that we will send aysnch INFO
// messages to Core, and possibly Simple messages to Airoha/DSP.
// We don't want to send too often, e.g. while encoder being rotated to
// adjust volume.
#define ASYNC_MAX_SEND_FREQ 200     // ms

// .............................................................................
 //Task initializations.
// This function runs BEFORE task is created and scheduler started!.
// .............................................................................
void APP_JSON_Initialize ( void )
{
    // Clear the INFO key/val arrays
    key_val_i_arr_indx_init();
}

// .............................................................................
// JSON Task
// Looks after sending asynchronous JSON Info messages to the Core app.
// Whenever the MCU wants to send an Info message, the Key/Value pair(s) are 
// added to the arrays: data_i_key_strs[] and data_i_val_strs[], and then the 
// index for these arrays are incremented.
// The JSON Task just monitors this array index value, and whenever it is 
// non-zero, it knows there are one or more key/value pairs to build into a 
// JSON Info message, so it calls json_fn_send_info_message() to do this.
// ( Note: The Airoha ? MCU Communications Task (APP_COM_Tasks) is sent a 
// message via the Q_comm_jsn queue, and it is the APP_COM_Tasks that actually 
// builds the JSON info message from the key/value pairs in the arrays).
// .............................................................................
void APP_JSON_Tasks ( void )
{
    uint8_t indx;

    // Task waits until the necessary Queues have been enabled
    while (Q_enc == NULL) osDelayMs(10);
    while (Q_bm_but == NULL) osDelayMs(10);         // button manager - button events
    while (Q_bm_comm == NULL) osDelayMs(10);        // button manager - from comms
    while (Q_enc_mcu == NULL) osDelayMs(10);
    while (Q_mcu_init == NULL) osDelayMs(10);
    while (Q_led == NULL) osDelayMs(10);
    // Read serial number just once
    mcu_ser_num_read();
    
    while(1) {
        // Need to pace the maximum send rate for Info messages, so add a delay.
        osDelayMs(ASYNC_MAX_SEND_FREQ);
        
        // See if there have been any key/value pairs added for inclusion in an 
        // Info message.
        indx = key_val_i_arr_indx_get();
        
        if (indx > 0) {
            // Yes, so send a message to the APP COmsm Task, so that it will
            // build and send a JSON Info message
            json_fn_send_info_message();
        }
    }
}
// .............................................................................
