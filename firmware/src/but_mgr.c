/*******************************************************************************
  File Name:
    but_mgr.c

  Summary:
    Button Manager

  Description:
 *******************************************************************************/
#include "but_mgr.h"

#define BTC_STATE_UNKNOWN           0       // Unknown / Error
#define BTC_STATE_DISCONNECTED      1       // No call, no connectoion
#define BTC_STATE_CONNECTED         2       // No call, connected - possibly music playing
#define BTC_STATE_CALL_INCOMING     3       // Call Incoming 
#define BTC_STATE_CALL_IN_PROGRESS  4       // Call In Progress 

static uint8_t btc_state;
static bool btc_pairing;

// .............................................................................
// BTC: Actions associated with button presses
// .............................................................................
void btc_but_init()
{
    btc_state = BTC_STATE_DISCONNECTED;
    btc_pairing = false;
}

// .............................................................................
// Initiate Bluetotoch action (because user has pressed button).
// Send relevant Bluetooth message to Airoha Communications Task via 
// Q_comm_simp queue (which will be sent to the AB1571 or AB1577).
// .............................................................................
void btc_but_action(uint8_t msg) 
{
    Q_comm_simp_t qs_comm_simp_msg;
    uint8_t msg_id = COMMS_NONE;
    int msg_val = 0;
    
    switch (msg) {
        case BTC_BUT_ANS_END_PLAY_PAUSE:
            // Right Enc - Single Press
            if (btc_state == BTC_STATE_CALL_INCOMING) {
                // Answer/Accept Call
                msg_id = COMMS_CALL_ANSWER_REQ;
            }
            else if (btc_state == BTC_STATE_CALL_IN_PROGRESS) {
                // End Call
                msg_id = COMMS_CALL_END_REQ;
            }
            else {
                // Play/Pause Music
                msg_id = COMMS_TRACK_PLAY_PAUSE_REQ;
            }
            // TODO: Do we need to add logic for state BTC_STATE_DISCONNECTED?
            // i.e. ignore button press
            break;
        case BTC_BUT_REJ:
            // Right Enc - Long Press (2s)
            if (btc_state == BTC_STATE_CALL_INCOMING) {
                // Reject Call
                msg_id =  COMMS_CALL_REJECT_REQ;
            }
            // TODO: Do we need to add logic for state BTC_STATE_DISCONNECTED?
            // i.e. ignore button press
            break;
        case BTC_BUT_FORWARD:
            // Right Enc - Double Click
            // Music Forward
            msg_id =  COMMS_TRACK_FORWARD_REQ;
            // TODO: Do we need to add logic for other states?
            // i.e. ignore button press
            break;
        case BTC_BUT_BACKWARD:
            // Right Enc - Triple Click
            // Music Backward
            msg_id =  COMMS_TRACK_BACKWARD_REQ;
            // TODO: Do we need to add logic for other states?
            // i.e. ignore button press
            break;
        case BTC_BUT_CONNECT:
            // Right Mfb - Long Press (3s)
            // Might end up sending 2 msgs here...
            if ((btc_state == BTC_STATE_CONNECTED) || (btc_pairing == true)) {
                // Cancel current connection first
                qs_comm_simp_msg.id = COMMS_BTC_DISCONNECT_REQ;
                qs_comm_simp_msg.i_val = 0;
                osQueueSendToBack(Q_comm_simp, &qs_comm_simp_msg, 10);
            }
            msg_id =  COMMS_BTC_CONNECT_REQ;
            // TODO: Not sure if this logic is safe
            break;
        default:
            break;
    }
    
    if (msg_id == COMMS_NONE) {
        // No message to send
        return;
    }
    
    // Send message to COMMS task
    qs_comm_simp_msg.id = msg_id;
    qs_comm_simp_msg.i_val = msg_val;
    osQueueSendToBack(Q_comm_simp, &qs_comm_simp_msg, 10);

}
// .............................................................................
// Airoha 1577 has sent update on BTC State.
// Note: Airoha usually sends APP_A2DP_PLAYING when music started, and 
// APP_CONNECTED when music stopped For now, we won't use this information.
// TODO: We night need to use this later.
// .............................................................................
void btc_state_notify(int msg) 
{
    //Q_led_t qs_led;
    uint8_t u_msg;
    
    u_msg = (uint8_t)msg;
            
    switch (u_msg) {
        case APP_BT_OFF:                    // BTC Off
            btc_state = BTC_STATE_DISCONNECTED;
            btc_pairing = false;
//            led_set_state(qs_led, LED_BLUE, LED_MODE_OFF, 0, 0, 0);
            break;
       case APP_DISCONNECTED:              // // BTC Disconnected
            // If were pairing: Right LED fail mode
            btc_state = BTC_STATE_DISCONNECTED;
            btc_pairing = false;
 //           led_set_state(qs_led, LED_BLUE, LED_MODE_OFF, 0, 0, 0);
            break;
        case APP_CONNECTABLE:               // BTC is pairing mode, phone can search and connect to it
            // If just entered this state: Right LED flashing mode
            btc_state = BTC_STATE_DISCONNECTED;
            btc_pairing = true;
 //           led_set_state(qs_led, LED_BLUE, LED_MODE_FLASH, LED_RATE_FAST, 0, 0);
            break;
        case APP_CONNECTED:                 // BTC Paired (no call or music)
            // If just entered this state: Right LED on (forever/dimmed?)
            btc_state = BTC_STATE_CONNECTED;
            btc_pairing = false;
//            led_set_state(qs_led, LED_BLUE, LED_MODE_ON, 0, 0, 0);
            break;
        case APP_HFP_INCOMING:              // Incoming Call
            // If audio cues managed by Airoha, nothing to do - just remember state
            btc_state = BTC_STATE_CALL_INCOMING;
            break;
        case APP_HFP_OUTGOING:              // Making an outgoing call through connected BTC enabled phone (before answer)
            // Nothing to do?
            break;
        case APP_HFP_CALL_ACTIVE :          // Call In Progress
            // If audio cues managed by Airoha, nothing to do - just remember state
            btc_state = BTC_STATE_CALL_IN_PROGRESS;
            break;
        case APP_A2DP_PLAYING:              // Music Playing 
            // Can only use this if Airoha also can send Music Not Playing?
            btc_state = BTC_STATE_CONNECTED;
        default:
            break;
    }
}
// .............................................................................

// .............................................................................
// Task initializations.
// This function runs BEFORE task is created and scheduler started!.
// .............................................................................
void BUT_MGR_Initialize ( void )
{
    btc_but_init();
}

// .............................................................................
// Button manager task
//
// Monitor Q_bm_but queue for button event mesasgaes from Button and Encoder 
// Task (APP_ENC_Tasks).
// Monitor Q_bm_comm queue for Bluetooth State messages (e.g. APP_CONNECTED) 
// from Airoha Communications Task (APP_COM_Tasks), and call the BTC State 
// Notify function to update the local copy of the BTC State.
// .............................................................................
void BUT_MGR_Tasks ( void )
{
    Q_but_t qs_but;
    Q_comm_simp_t qs_comm_msg;
    Q_comm_simp_t qs_comm_msg_2;
    Q_mic_t qs_mic;

    // Task waits until the necessary Queues have been enabled
    while (Q_bm_but == NULL) osDelayMs(10);
    while (Q_bm_comm == NULL) osDelayMs(10);
    while (Q_enc_mcu == NULL) osDelayMs(10);
    while (Q_mic == NULL) osDelayMs(10);
    
    while(1) {
        osDelayMs(1);

        // Messages to the Button Manager, from button/encoder task - button events
        if (osQueueReceive(Q_bm_but, &qs_but, 1) == pdPASS)
        {
            DBGprintf("BUTENC %d %d\n", qs_but.event, qs_but.pos);
            
            switch (qs_but.event) {
                //
                // LEFT SIDE
                //
                case LEFT_MFB_1CLICK:
                    // Microphone Mute/Unmute
                    qs_mic.event = MIC_MUTE_TOGGLE;
                    osQueueSendToBack(Q_mic, &qs_mic, 10);
                    break;
                case LEFT_MFB_2CLICK:
                    break;
                case LEFT_MFB_3CLICK:
                    break;
                case LEFT_MFB_LONGPRESS:
                    break;
                case LEFT_ENC_1CLICK:
                    // Send a specific event to Power Manager for displaying Battery Status
                    qs_but.event = BAT_STATUS;
                    osQueueSendToBack(Q_pwr, &qs_but, 10);
                    break;
                case LEFT_ENC_2CLICK:
                    // Send message to COMMS task
                    qs_comm_msg_2.id = COMMS_24GHZ_CONNECT;
                    qs_comm_msg_2.i_val = 0;
                    osQueueSendToBack(Q_comm_simp, &qs_comm_msg_2, 10);                    
                    break;
                case LEFT_ENC_3CLICK:
                    // Send a specific event to Power Manager for turning the SLEEP ON / OFF
                    //qs_but.event = SLEEP_ON_OFF;
                    //osQueueSendToBack(Q_pwr, &qs_but, 10);
                    break;
                case LEFT_ENC_LONGPRESS:
                    // Send a specific event to Power Manager for turning the power ON / OFF
                    qs_but.event = POWER_ON_OFF;
                    osQueueSendToBack(Q_pwr, &qs_but, 10);
                    break;
                case LEFT_ENCODER:
                case LEFT_ENCODER_STOP:
                    // Update ChatMix
//                    json_fn_i_chat_mix(qs_but.pos);
                    break;
                //
                // RIGHT SIDE
                //
                case RIGHT_MFB_1CLICK:
                    // Cycle to the next ANC Mode
//                    dsp_anc_mode_cycle();
                    break;
                    
                case RIGHT_MFB_2CLICK:
                    break;
                case RIGHT_MFB_3CLICK:
                    break;
                case RIGHT_MFB_LONGPRESS:
                    // BTC Connect request
                    btc_but_action(BTC_BUT_CONNECT);
                    break;
                case RIGHT_ENC_1CLICK:
                     //led_set_state(qs_led, LED_BLUE, LED_MODE_FLASH, LED_RATE_FAST, 3, 0);       // !rai!
                    // Answer/End Call or Play/Pause Music
                    btc_but_action(BTC_BUT_ANS_END_PLAY_PAUSE);
                    break;
                case RIGHT_ENC_2CLICK:
                     //led_set_state(qs_led, LED_BLUE, LED_MODE_FLASH, LED_RATE_FAST, 5, 0);       // !rai!
                    // Music Forward
                    btc_but_action(BTC_BUT_FORWARD);
                    break;
                case RIGHT_ENC_3CLICK:
                    //led_set_state(qs_led, LED_BLUE, LED_MODE_FLASH, LED_RATE_FAST, 7, 0);       // !rai!
                    // Music Backward
                    btc_but_action(BTC_BUT_BACKWARD);
                    break;
                case RIGHT_ENC_LONGPRESS:
                    //led_set_state(qs_led, LED_BLUE, LED_MODE_FLASH, LED_RATE_FAST, 10, 0);       // !rai!
                    // Reject Call
                    btc_but_action(BTC_BUT_REJ);
                    break;
                case RIGHT_ENCODER:
                case RIGHT_ENCODER_STOP:
                    // Update Audio Volume
                    json_fn_i_audio_vol(qs_but.pos);
                    break;
                //
                // LEFT + RIGHT
                //                    
                case LEFT_RIGHT_ENC_1CLICK:
                    break;
                default:
                    break;
            }
        }
        
        // Messages to the Button Manager - from COMMS task
        if (osQueueReceive(Q_bm_comm, &qs_comm_msg, 1) == pdPASS)
        {
            /* debugging printf */
            printf("BUTMSG %d %d\n", qs_comm_msg.id, qs_comm_msg.i_val);
            if (qs_comm_msg.id == COMMS_BTC_STATE_UPDATE) {
                btc_state_notify(qs_comm_msg.i_val);
            }
        }  
    }
}
// .............................................................................
