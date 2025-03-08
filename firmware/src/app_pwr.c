/*******************************************************************************
  File Name:
    app_pwr.c

  Summary:
    Power task.

  Description:
 *******************************************************************************/
#include "app_pwr.h"
#include "json\json.h"

/*
extern void dsp_init();
extern void dsp_setup();
*/
extern void air_init();

// Battery Value (last saved battery status)
volatile uint8_t bat_status;

// .............................................................................
// Power State Machine
// .............................................................................
#define PWR_STATE_CHECK_POWER_BUTTON        0
#define PWR_STATE_TIMED_POWER_BUTTON_CHECK  1
#define PWR_STATE_POWER_ON                  2
#define PWR_STATE_DELAY_ON                  3
#define PWR_STATE_ON                        4
#define PWR_STATE_STANDBY                   5
#define PWR_STATE_DELAY_POWER_OFF           6
#define PWR_STATE_OFF                       7

#define HEADSET_STATE_STANDBY       0
#define HEADSET_STATE_ON            1
#define HEADSET_STATE_SLEEP         2
#define HEADSET_STATE_OFF           3

#define WEAR_SENSOR_FAR                 0
#define WEAR_SENSOR_NEAR                1

uint8_t pwr_state = PWR_STATE_CHECK_POWER_BUTTON;
uint8_t state_before_delay_off = PWR_STATE_CHECK_POWER_BUTTON;
int wear_status = WEAR_SENSOR_FAR;
bool b_power_button = false;
bool b_standby_first_time = true;

#define ACCEL_INACTIVE_MAX          30
uint8_t accel_inactive_cntr = 0;
bool b_accel_motion = false;
bool b_accel_active = true;
// .............................................................................

// .............................................................................
// Enable 1.8V general power rail (including right board)
// .............................................................................
void PowerEnable_1V8()
{
     printf("PwrEN1V8\n");
    osDelayMs(10);
    VDD_1V8_EN_Set();
}

// .............................................................................
// Enable 3.3V general power rail (including right board)
// .............................................................................
void PowerEnable_3V3()
{
    printf("PwrEn3V3\n");
    osDelayMs(10);
    U3V3_VDD_EN_Set();
}

// .............................................................................
// Enable 3.3V power for DSP AMP
// It needs the general 3V3 rail as the source.
// .............................................................................
void PowerEnable_AMP()
{
    printf("PwrEnAMP\n");
    osDelayMs(10);
    //DSP_PWR_EN_Set();
    AUDIO_PWR_EN_Set();
    osDelayMs(10);
    //ADAU1860_PD_Set(); // wake dsp up from power down
    osDelayMs(50);

}

// .............................................................................
// Airoha power control
// .............................................................................
void PowerEnable_Airoha()
{
    printf("PwrEnAiro\n");
    //RESET_77_Clear();   // keep the airoha in reset
    REGEN_77_Set();     // airoha regulator enable
    osDelayMs(1000);
    REGEN_77_Clear();     // release the airoha from reset
    //REGEN_77_Set();
    // Activate the 5V boost for the little batt charger in Airoha
    osDelayMs(10);
    U5V_DC_DC_EN_Set();
}
// .............................................................................

/*
// ********************
// Unused functions.... 
// ********************
 
// ..................................................
// Reset the right MCU
// ..................................................
void ResetRightMCU()
{
    MCU_RESET_U6_Clear();
    osDelayMs(1000);
    MCU_RESET_U6_Set();
}
// ..................................................
void PowerDisable_1V8()
{
    osDelayMs(10);
    VDD_1V8_EN_Clear();
}
// ..................................................
void PowerDisable_3V3()
{
    osDelayMs(10);
    U3V3_VDD_EN_Clear();
}
// ..................................................
void PowerDisable_AMP()
{
    osDelayMs(10);
    ADAU1860_PD_Clear(); // dsp power down
    osDelayMs(10);
    DSP_PWR_EN_Clear();
}
// ..................................................
void PowerDisable_Airoha()
{
    osDelayMs(10);
    REGEN_77_Clear();   // airoha regulator enable
    osDelayMs(10);
    RESET_77_Clear();   // keep the airoha in reset
    osDelayMs(50);
    U5V_DC_DC_EN_Clear();
}
// ..................................................
void SleepDisableSystem()
{
    PowerEnable_3V3();
    PowerEnable_AMP();
    PowerEnable_Airoha();
    ResetRightMCU();
    dsp_init(); // init dsp
    dsp_setup();
    air_init(); // init airoha
    
}
// ..................................................
void SleepEnableSystem()
{
    // deinit dsp...
    // deinit airoha...
    //PowerDisable_Airoha();
    PowerDisable_AMP();
    //PowerDisable_3V3();
    //MCU_PB16_1V8_EN_Clear();
}
// ..................................................
// General power enable/disable
// ..................................................
void PowerEnableSystem()
{
    PowerEnable_1V8();
    SleepDisableSystem();
}
// ..................................................
void PowerDisableSystem()
{
    SleepEnableSystem();
    PowerDisable_1V8();
}
// ..................................................
*/

// .............................................................................
// Start timer, so we can monitor the Power Button long enough to allow a
// long press to occur (so just over 3 seconds).
// .............................................................................
void power_state_timed_power_button_check()
{
    // Monitor left encoder button for just over 3 seconds
    enc_power_task_timer_start(TMR_PWR_BUTTON_CHECK);
}
// .............................................................................
// Power up everything, and delay for a short time at the end, to allow
// wear sensor to initialise.
// .............................................................................
void power_state_power_on()
{
    Q_comm_simp_t qs_comm_simp_msg;

    enc_power_task_timer_stop();

    // Full power up as before
    bat_status = 255;
    PowerEnable_1V8();
    PowerEnable_3V3();
    PowerEnable_AMP();
    PowerEnable_Airoha();
    GlobalPowerOn = 1;
    battery_charging = 1;

    osDelayMs(10);

    // Send message to Airoha, via COMMS task
    qs_comm_simp_msg.id = COMMS_HEADSET_STATE;
    qs_comm_simp_msg.i_val = HEADSET_STATE_ON;
    osQueueSendToBack(Q_comm_simp, &qs_comm_simp_msg, 10);
    // Delay sending wake up message
    enc_power_task_timer_start(TMR_PWR_DELAY_POWER_ON);
}
// .............................................................................
// Wear Sensor -> OFF
// Inform Airoha, and start inactivity timer
// .............................................................................
void power_state_standby()
{
    Q_comm_simp_t qs_comm_simp_msg;

    // Send message to Airoha, via COMMS task
    qs_comm_simp_msg.id = COMMS_HEADSET_STATE;
    qs_comm_simp_msg.i_val = HEADSET_STATE_STANDBY;
    osQueueSendToBack(Q_comm_simp, &qs_comm_simp_msg, 10);
    // Start inactivity (no motion) timer fro this state
    enc_power_task_timer_start(TMR_PWR_STANDBY);
    
    b_standby_first_time = true;
}
// .............................................................................
// Headset has been motionless for some time - move from mid-power to low-power
// .............................................................................
void power_state_standby2()
{
    // Restart inactivity (no motion) timer for this state
    enc_power_task_timer_start(TMR_PWR_RESTING);
    
    b_standby_first_time = false;
}

// .............................................................................
// Wear Sensor -> ON
// Inform Airoha, and stop inactivity timer
// .............................................................................
void power_state_on() 
{
    Q_comm_simp_t qs_comm_simp_msg;

    // Send message to Airoha, via COMMS task
    qs_comm_simp_msg.id = COMMS_HEADSET_STATE;
    qs_comm_simp_msg.i_val = HEADSET_STATE_ON;
    osQueueSendToBack(Q_comm_simp, &qs_comm_simp_msg, 10);
    // Stop the inactivity (no motion) timer
    enc_power_task_timer_stop();
}
// .............................................................................
// Going to power down, but first inform Airoha, and allow enough time for
// Airoha to receive the message
// .............................................................................
void power_state_delay_power_off()
{
    Q_comm_simp_t qs_comm_simp_msg;

    // Send message to Airoha, via COMMS task
    qs_comm_simp_msg.id = COMMS_HEADSET_STATE;
    qs_comm_simp_msg.i_val = HEADSET_STATE_OFF;     // !dbg!
    osQueueSendToBack(Q_comm_simp, &qs_comm_simp_msg, 10);
    // Give Airoha chip some time before actually going to sleep / powering off
    enc_power_task_timer_start(TMR_PWR_DELAY_POWER_OFF);
}
// .............................................................................
// Power down fully, by removing the 1.8 V latch
// .............................................................................
void power_state_off()
{
    // Unlatch power to ourselves
    MCU_PB16_1V8_EN_Clear();
}
// .............................................................................

// .............................................................................
// Task initializations.
// This function runs BEFORE task is created and scheduler started.
// .............................................................................
void APP_PWR_Initialize ( void )
{
}

// .............................................................................
// The Power Task.
// 
// Queue summary:
// 
// QUEUE        RECEIVES
// -----        --------
// Q_pwr        Owned by: Power task
//              Receives: Battery Update events (from Right MCU) - from MCU task
//              Receives: Battery, Power and Sleep requests - from Button Manager
// Q_pwr_sns    Owned by: Power task
//              Receives: Wear Sensor status and Accelerometer motion state - from Sensor task
// Q_enc        Owned by: Encoder task
//              Receives: Left Encoder events - from Left Encoder Callback
// Q_enc_mcu    Owned by: Encoder task
//              Receives: Encoder events from Right MCU - from MCU task
// Q_led        Owned by: Encoder task
//              Receives: Requests to update an LED (red, green, mic, blue) - from any task
// Q_bm_but     Owned by: Button Manager
//              Receives: Left Encoder events (from Q_enc_mcu) - from Encoder task
//              Receives: Left Button events - from Encoder task
//              Receives: Dual button events - from Encoder task
//              Receives: Events from Q_enc_mcu (Right MCU button/encoder events) - from Encoder task
// Q_bm_comm    Owned by: Button Manager
//              Receives: ?Simple Protocol? msgs received from Airoha - from COMS task
// Q_mcu_init   Owned by: MCU task
//              Receives: Initial button/encoder configuration for RIght MCU - from Encoder task
// Q_mcu_led    Owned by: MCU task
//              Receives: Requests to update Blue LED (from Q_led) - from Encoder Task
// Q_comm_simp  Owned by: COM task
//              Receives: BTC messages to send to Airoha (simple protocol) - from Button Manager
// Q_comm_jsn   Owned by: COM task
//              Receives: JSON messages to send to Core App (via Airoha) - from COMS task
// Q_mic        Owned by: MIC task
//              Receives: Requests to mute/unmute Microphone - from Button Manager
// .............................................................................
void APP_PWR_Tasks ( void )
{
    Q_but_t qs_but;
   // Q_sensors_t qs_sns;
    Q_led_t qs_led_r;
    Q_led_t qs_led_g;
    uint16_t bat_v;
    uint8_t bat_s;
   // bool b_accel;
    bool b_timeout;

    // Create all the queues here
    Q_pwr = osQueueCreate(16, sizeof(Q_but_t));
   // Q_pwr_sns = osQueueCreate(16, sizeof(Q_sensors_t));
    Q_enc = osQueueCreate(16, sizeof(Q_but_t));
    Q_enc_mcu = osQueueCreate(16, sizeof(Q_but_t));
    Q_led = osQueueCreate(16, sizeof(Q_led_t));
    Q_bm_but = osQueueCreate(16, sizeof(Q_but_t));
    Q_bm_comm = osQueueCreate(16, sizeof(Q_comm_simp_t));
    Q_mcu_init = osQueueCreate(16, sizeof(button_config_t));
    Q_mcu_led = osQueueCreate(16, sizeof(Q_led_t));
    Q_comm_simp = osQueueCreate(16, sizeof(Q_comm_simp_t));
    Q_comm_jsn = osQueueCreate(16, sizeof(Q_comm_jsn_t));
    Q_mic = osQueueCreate(16, sizeof(Q_mic_t));
        
    pwr_state = PWR_STATE_CHECK_POWER_BUTTON;
   // wear_status = WEAR_SENSOR_FAR;
   // b_accel_active = false;
    
    b_l_enc_clear_flags();
    
    while(1) {
        
        // Assumptions, each time around in the loop
        b_power_button = false;             // Will be true, if user presses Power Button (Left Encoder long-press)
        b_accel_motion = false;             // Will be true, if Accelerometer detects motion
        b_timeout = false;                  // Will be true, if state machine timer (below) times out
        
        // Processes any Wear Sensor and Accelerometer status messages 
        // received from the Sensor Task.
//        if (osQueueReceive(Q_pwr_sns, &qs_sns, 10) == pdPASS) {
//            wear_status = qs_sns.wear_status;
//            
//            b_accel = qs_sns.b_accel;
//            if (b_accel == true) {
//                b_accel_motion = true;                  // Motion now
//                b_accel_active = true;                  // Activity in the last N seconds
//                accel_inactive_cntr = 0;                // Reset inactivity counter
//            } else {
//                                                        // No motion now
//                if (b_accel_active == true) {           // Have flagged inactivity in last N seconds
//                    accel_inactive_cntr++;              // Counting inactivity to see if reach max
//                    if (accel_inactive_cntr >= ACCEL_INACTIVE_MAX) {
//                        b_accel_active = false;        // Inactive for the last N seconds
//                    }
//                }
//            }
//        }
        
        // ?Processes any Power On, Power Off, Power Timeout and Battery Update 
        // messages received from other tasks.
        if (osQueueReceive(Q_pwr, &qs_but, 10) == pdPASS)
        {
            if (qs_but.event == POWER_ON_OFF) {
                // User has pressed Power button (left encoder, long-press))
                b_power_button = true;
            }
            else if (qs_but.event == POWER_TIMEOUT) {
                // This state machine's timer has expired
                b_timeout = true;
            }
            else if (qs_but.event == BAT_UPDATE) {
                // Battery Value Update received from Right MCU
                // Take a copy of Battery Value (mV))
                bat_v = (uint16_t)qs_but.pos;
                DBGprintf("Pwr Tsk: Bat V %d\n", bat_v);
                if (bat_v < 4000) {
                    bat_s = 0;
                } else {
                    bat_s = 1;
                }
                if (bat_status != bat_s) {
                    // Battery Status has changed
                    bat_status = bat_s;
                    // Do something
                    if (bat_status == 0) {
                        // Battery BAD - Red LED on (forever), Green LED off
                        //led_set_state(qs_led_g, LED_GREEN, LED_MODE_FLASH, LED_RATE_SLOW, 5, 0);
                    } else {
                        // Battery GOOD - Green LED on (forever), Red LED off
                        //led_set_state(qs_led_g, LED_GREEN, LED_MODE_FLASH, LED_RATE_FAST, 3, 0);
                    }
                }
            }
            else if (qs_but.event == BAT_STATUS) {
                // Battery Status request received from ENCODER task
                DBGprintf("Pwr Tsk: Bat S\n");
                // Do something
                if (bat_status == 0) {
                    // Battery BAD - Red LED on (forever), Green LED off
                    //led_set_state(qs_led_g, LED_GREEN, LED_MODE_FLASH, LED_RATE_SLOW, 5, 0);
                } else {
                    // Battery GOOD - Green LED on (forever), Red LED off
                    //led_set_state(qs_led_g, LED_GREEN, LED_MODE_FLASH, LED_RATE_FAST, 3, 0);
                };
            }
        }
        
        // Headset State - state machine...
        switch (pwr_state) {
            
            case PWR_STATE_CHECK_POWER_BUTTON:
                // MCU  just booted up
                // So has AB1577
                // Determine if this is because of Left Encoder Press or USB-C connect
                
                if (b_l_enc_has_been_processed()) {
                    // Only check button events if encoder task has processed button at least once
                    if (b_l_enc_has_been_pressed()) {
                        // Left Encoder was pressed, so monitor this now
                        power_state_timed_power_button_check();
                        pwr_state = PWR_STATE_TIMED_POWER_BUTTON_CHECK;
                    } else {
                        // No Encoder press, so must be USB-C, so go straight to power on
                        pwr_state = PWR_STATE_POWER_ON;
                    }
                } else {
                    // Stay in this state until left encoder button has been 
                    // checked at least once by the encoder task
                }
                break;
                
            case PWR_STATE_TIMED_POWER_BUTTON_CHECK:
                // Booted up because of Left Encoder press
                if (b_power_button) {
                    // Left Encoder pressed for > 3 seconds, so power up fully
                    // Power up everything
                    pwr_state = PWR_STATE_POWER_ON;
                } else if (b_l_enc_has_been_released()) {
                    // Button has been released, soo not pressed long enough, so power down
                    // Send message to Airoha, and start timer
                    power_state_delay_power_off();
                    pwr_state = PWR_STATE_DELAY_POWER_OFF;
                } else if (b_timeout) {
                    // Probably a bug if we get here.
                    // Power up everything
                    pwr_state = PWR_STATE_POWER_ON;
					
                }
                
                break;
                
            case PWR_STATE_POWER_ON:
                // Power up everything up                
                power_state_power_on();
                // Update state
                led_set_state(qs_led_r, LED_RED, LED_MODE_OFF, 0, 0, 0);                    // Red:     Off
                led_set_state(qs_led_g, LED_GREEN, LED_MODE_ON, 0, 0, 0);                   // Green:   On
                // Allow short delay for wear sensor to initialise, before checking its state
                pwr_state = PWR_STATE_DELAY_ON;
                break;
                
            case PWR_STATE_DELAY_ON:
                //printf("PWR_STATE_DELAY_ON \n");
                if (b_timeout) {
                    // Finished the small delay after wake up, now go to Power state
                    // so can check Wear Sensor and Power button
                    pwr_state = PWR_STATE_ON;
					printf("time out power on \n");
                }
                break;
                
            case PWR_STATE_ON:
                // Only get here from Standby
                //if (wear_status == WEAR_SENSOR_FAR) {
//                if ((wear_status == WEAR_SENSOR_FAR) || (b_accel_active == false)) {
//                    // Headset removed from head, so inform Airoha, and start Inactivty timer
//                    power_state_standby();
//                    accel_sensitivity(ACCEL_LO_SENSE_THD, ACCEL_LO_SENSE_WIN);      // threshold = n/32g (0.125g), window = n (number of samples)
//                    // Update state
//                    led_set_state(qs_led_r, LED_RED, LED_MODE_OFF, 0, 0, 0);                    // Red:     Off
//                    led_set_state(qs_led_g, LED_GREEN, LED_MODE_FLASH, LED_RATE_SLOW, 0, 0);    // Green:   Slow Blinking
//                    pwr_state = PWR_STATE_STANDBY;
//                } else if (b_power_button) {
					if (b_power_button) {
                    // User has pressed button to Power Down. Send message to Airoha, then small delay
                    // (to allow Airoha to play "Power Down" audio cue, before MCU puts most of the system in Sleep / Power Off)
                    power_state_delay_power_off();
                    // Update state
                    led_set_state(qs_led_r, LED_RED, LED_MODE_FLASH, LED_RATE_FAST, 0, 0);      // Red:     Fast Blinking
                    led_set_state(qs_led_g, LED_GREEN, LED_MODE_OFF, 0, 0, 0);                  // Green:   Off
                    pwr_state = PWR_STATE_DELAY_POWER_OFF;
                }
                break;
                
            case PWR_STATE_STANDBY:
                // There are two stages in this state: Standby and Resting
                // The programmed Standby is separated into 1/4 and 3/4 of the total time.
                // The first quarter is Standby time, which will be mid-power. This is also when
                // we send a message to Airoha.
                // The three quarters after (if headset still motionless and not on head) is three
                // quarters of the programmed time, which will be low-power.
                //if (wear_status == WEAR_SENSOR_NEAR) {
//                if ((wear_status == WEAR_SENSOR_NEAR) && (b_accel_active == true)) {
//                    // Headset just oput on head, so inform Airoha
//                    power_state_on();
//                    accel_sensitivity(ACCEL_HI_SENSE_THD, ACCEL_HI_SENSE_WIN);      // threshold = n/32g (0.125g), window = n (number of samples)
//                    // Update state
//                    led_set_state(qs_led_r, LED_RED, LED_MODE_OFF, 0, 0, 0);                    // Red:     Off
//                    led_set_state(qs_led_g, LED_GREEN, LED_MODE_ON, 0, 0, 0);                   // Green:   On
//                    pwr_state = PWR_STATE_ON;
//                } else if (b_power_button) {
					if (b_power_button) {
                    // User has pressed button to Power Down. Send message to Airoha, then small delay
                    // (to allow Airoha to play "Power Down" audio cue, before MCU puts most of the system in Sleep / Power Off)
                    power_state_delay_power_off();
                    // Update state
                    led_set_state(qs_led_r, LED_RED, LED_MODE_FLASH, LED_RATE_FAST, 0, 0);      // Red:     Fast Blinking
                    led_set_state(qs_led_g, LED_GREEN, LED_MODE_OFF, 0, 0, 0);                  // Green:   Off
                    pwr_state = PWR_STATE_DELAY_POWER_OFF;
				  }
//                } else if (b_timeout) {
//                    if (b_standby_first_time) {
//                        b_standby_first_time = false;
//                        power_state_standby2();
//                        led_set_state(qs_led_r, LED_RED, LED_MODE_FLASH, LED_RATE_FAST, 0, 0);                    // Red:     Fast Blinking
//                        led_set_state(qs_led_g, LED_GREEN, LED_MODE_FLASH, LED_RATE_SLOW, 0, 0);    // Green:   Slow Blinking
//                    } else {
//                        // Inactivity timeout - so no accelerometer motion for a long time. Send message to Airoha, then small delay
//                        // (to allow Airoha to play "Power Down" audio cue, before MCU puts most of the system in Sleep / Power Off)
//                        power_state_delay_power_off();
//                        // Update state
//                        led_set_state(qs_led_r, LED_RED, LED_MODE_FLASH, LED_RATE_FAST, 0, 0);      // Red:     Fast Blinking
//                        led_set_state(qs_led_g, LED_GREEN, LED_MODE_OFF, 0, 0, 0);                  // Green:   Off
//                        pwr_state = PWR_STATE_DELAY_POWER_OFF;
//                    }
//                }
                break;

            case PWR_STATE_DELAY_POWER_OFF:
                if (b_timeout) {
                    
                    led_set_state(qs_led_r, LED_RED, LED_MODE_OFF, 0, 0, 0);                    // Red:     Off
                    led_set_state(qs_led_g, LED_GREEN, LED_MODE_OFF, 0, 0, 0);                  // Green:   Off
                    osDelayMs(10);
                    power_state_off();
                    // Update state
                    pwr_state = PWR_STATE_OFF;
					printf("PWR_STATE_DELAY_POWER_OFF \n");
                }
                break;
                
            default:
                break;
        }
       
        // Small delay
        osDelayMs(10); 
    }
}
// .............................................................................
