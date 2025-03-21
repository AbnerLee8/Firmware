/*******************************************************************************
  File Name:
    app_mic.c

  Summary:
    Microphones task.
 *******************************************************************************/
#include "app_mic.h"

volatile bool mic_muted;
volatile bool boom_mic_connected;

// .............................................................................
// Enable (unmute) the Boom Mic
// .............................................................................
void mic_boom_enable()
{
    printf("mic boom en\n");
    VDD_MIC_EN_Set();
    //dsp_boom_mic_enable();
    
}

// .............................................................................
// Disable (mute) Mute the Boom Mic
// .............................................................................
void mic_boom_disable()
{
     printf("mic boom dis\n");
    VDD_MIC_EN_Clear();
    //dsp_boom_mic_disable();
}

// .............................................................................
// Enable (unmute) the Talk Mics
// .............................................................................
void mic_talk_enable()
{
     printf("talk mic en\n");
    //dsp_talk_mics_enable();
}

// .............................................................................
// Disable (mute) the Talk Mics
// .............................................................................
void mic_talk_disable()
{
    printf("talk mic dis\n");
    //dsp_talk_mics_disable();
}

// .............................................................................
// Toggle mute status
// Called when user presses button to mute/unmute the microphone
// Cases....                                        Boom    Talk
// --------                                         ------- ---------
// 1. Boom mic is connected and Mic just muted:     Disable Disable
// 2. Boom mic is connected and Mic just unmuted:   ENABLE  Disable
// 3. Boom mic is disconnected and Mic just muted:  Disable Disable
// 4. Boom mic is disconnected and Mic just unmuted:Disable ENABLE
// .............................................................................
void mic_mute_toggle(uint8_t toggle_event)
{
    Q_led_t qs_led_mic;
    Q_comm_simp_t qs_comm_simp_msg;
    uint8_t msg_id;
    int msg_val;
    
    if (toggle_event != MIC_MUTE_TOGGLE) {
        // Invalid message
        return;
    }
    
    // Toggle to mic mute status
    mic_muted = !mic_muted;
    
    if (boom_mic_connected) {
        // Boom mic is connected
        if (mic_muted) {
            // 1. Boom mic is connected and Mic just muted
            // Mic just muted - disable all mics
            mic_boom_disable();
            mic_talk_disable();
           // dsp_mute_all_mics();                
            
            // MIC Led ON
            led_set_state(qs_led_mic, LED_MIC, LED_MODE_ON, 0, 0, 0);
        } else {
            // 2. Boom mic is connected and Mic just unmuted
            // Mic just unmuted - enable the boom mic, and disable the talk mics
            mic_boom_enable();
            mic_talk_disable();
            //dsp_using_boom_mic();
            
            // MIC Led OFF
            led_set_state(qs_led_mic, LED_MIC, LED_MODE_OFF, 0, 0, 0);
        }
    } else {
        // Boom mic is disconnected
        // Boom MIC Led OFF (no point having it on when boom mic disconnected!)
        led_set_state(qs_led_mic, LED_MIC, LED_MODE_OFF, 0, 0, 0);
        if (mic_muted) {
            // 3. Boom mic is disconnected and Mic just muted
            // Mic just muted - disable all mics
            mic_boom_disable();
            mic_talk_disable();
            //dsp_mute_all_mics();                
            
        } else {
            // 4. Boom mic is disconnected and Mic just unmuted
            // Mic just unmuted - disable boom mic, enable the talk mics
            mic_boom_disable();
            mic_talk_enable();
            //dsp_using_dmics();
        }
    }
    
    // Inform Airoha of mute change
    if (mic_muted) {
        msg_id = COMMS_MIC_MUTED;
    } else {
        msg_id = COMMS_MIC_UNMUTED;
    }
    msg_val = 0;
    // Send message to COMMS task
    qs_comm_simp_msg.id = msg_id;
    qs_comm_simp_msg.i_val = msg_val;
    osQueueSendToBack(Q_comm_simp, &qs_comm_simp_msg, 10);
}

// .............................................................................
// See if any change in MIC Boom connected state
// NOTE: MCI DET pin is LOW when Microphone (Boom) is connected
//
// Cases...                                         Boom    Talk
// --------                                         ------- ---------
// 1. Boom mic just disconnected, and mic muted:    Disable Disable
// 2. Boom mic just disconnected, and mic unmuted   Disable ENABLE
// 3. Boom mic just connected, and mic muted        Disable Disable
// 4. Boom mic just connected, and mic unmuted      ENABLE  Disable
// .............................................................................
void mic_monitor_connection()
{
    Q_led_t qs_led_mic;

    if (!MIC_DET_Get()) {
        // Boom Mic is currently disconnected
        if (boom_mic_connected) {
            
           // BOOM Microphone has just been disconnected:
            
            boom_mic_connected = false;
            // Boom MIC Led OFF (no point having it on when boom mic disconnected!)
            led_set_state(qs_led_mic, LED_MIC, LED_MODE_OFF, 0, 0, 0);
            if (mic_muted) {
                // 1. Boom mic just disconnected, and mic muted
                // Make sure all mic's disabled
               // dsp_mute_all_mics();                
                
            } else {
                // 2. Boom mic just disconnected, and mic unmuted
                // Not muted, so need to disable the boom mic and enable the talk mics
                //dsp_using_dmics();        
            }
        }
    } else {
        // Boom MIC is  connected
        if (!boom_mic_connected) {

           // BOOM Microphone has just been connected:

            boom_mic_connected = true;
            if (mic_muted) {
                // 3. Boom mic just connected, and mic muted
                // Make sure all mic's still disabled
               // dsp_mute_all_mics();                
                
                // Boom MIC Led ON
                led_set_state(qs_led_mic, LED_MIC, LED_MODE_ON, 0, 0, 0);
            } else {
                // 4. Boom mic just connected, and mic unmuted
                // Make sure boom mic Enabled and Talk mics Disabled
                //dsp_using_boom_mic();
                // Make sure Boom MIC Led is OFF
                led_set_state(qs_led_mic, LED_MIC, LED_MODE_OFF, 0, 0, 0);
            }
        }
    }
}

// .............................................................................
// Initial setup
// .............................................................................
void mic_init()
{
    Q_led_t qs_led_mic;
    
    // TODO: The initial mute status will eventually be read from the saved/loaded Profile

    mic_muted = false;
    boom_mic_connected = false;
    // Make sure Boom MIC Led is OFF
    led_set_state(qs_led_mic, LED_MIC, LED_MODE_OFF, 0, 0, 0);
    // Make sure Boom mic Enabled, and Talk mics Disabled
    //dsp_using_dmics();               
    
}
    
// .............................................................................
// Task initializations.
// This function runs BEFORE task is created and scheduler started!.
// .............................................................................
void APP_MIC_Initialize ( void )
{
}

// .............................................................................
// Microphone task
//
// Monitor microphone connection (see if any change in MIC Boom connected state).
// Mute / unmute microphones (boom mic, dmics).
// Update Boom Mic LED.
// Monitor for any Mic Mute/Unmute messages on the Q_mic queue (sent by  
// BUT_MGR_Tasks, when user presses Mic Mute button).
// If any update on Microphone Mute status, puts message in Q_comm_simp queue 
// (which will be sent to the AB1577).
// .............................................................................
void APP_MIC_Tasks ( void )
{
    Q_mic_t qs_mic;

    // Task waits Power Task has powered everything up
    while (!GlobalPowerOn) {
        osDelayMs(100);
    }
    // Task waits until the necessary Queues have been enabled
    while (Q_mic == NULL) osDelayMs(10);
    while (Q_led == NULL) osDelayMs(10);
    
    mic_init();
    
    while(1) {
        
        // Monitor Microphone Boom connection Status
        mic_monitor_connection();
        
        if (osQueueReceive(Q_mic, &qs_mic, 10) == pdPASS)
        {
            if (qs_mic.event == MIC_MUTE_TOGGLE) {
                // Process the Mic Mute toggle event
                mic_mute_toggle(qs_mic.event);
                printf("Toggle mute status\n");
            }
        }
        OS_SLEEP(10); //osDelayMs(10);
    }
}
// .............................................................................
