/*******************************************************************************
  File Name:
    app_enc.c

  Summary:
    Encoder task. Encoder, buttons and leds.

  Description:
 *******************************************************************************/
#include "app_enc.h"
#include "json\json.h"
#include "hw_hx32062se.h"
#include <math.h>

// ........................................
// Encoder
// ........................................
volatile int old_pos;
volatile uint enc_tick;
volatile int enc_steps;
volatile led_info_t led_info[3];
volatile led_info_t led_new[3];

// ........................................
// LED
// ........................................
#define LED_STATE_OFF       0
#define LED_STATE_ON        1

// For now, default state for LEDS: Red=On, Green=Off, Mic=Off
// All LEDs off, by default
volatile uint8_t led_default[] = {LED_MODE_OFF, LED_MODE_OFF, LED_MODE_OFF};
volatile uint8_t led_mngr_cnt;

// Encoder Left & Right 1-click Combine Events
volatile uint8_t lr_timeout;            // Down counter, set to LR_TIMEOUT on Left or RIght 1-click event.
                                        // Counter decremented in TImer Callback.
                                        // If expires, set the lr_done flag.
volatile uint8_t lr_done;               // Checked in task's main loop. If set, time to generate events
                                        // depending on lr_left and lr_right flags.
volatile uint8_t lr_left;               // Left Encoder 1-click event NOT passed on to button manager immediately.
volatile uint8_t lr_right;              // Right Encoder 1-click event NOT passed on to button manager immediately.
                                        // Instead, set one of these flags start down counter.
                                        // In main loop, will check lr_done flag. If set, will send one or
                                        // more of these events to button manager:
                                        // lr_left set: Left 1-click, 
                                        // lr_rightset: Right 1-click
                                        // lr_left and lr_right: Left AND Right 1-Click (combine event))
#define LR_TIMEOUT  TICKS_MS(200)

// ........................................
// Buttons
// - implementation uses timer TC1 with 
//   1ms resolution
// ........................................
// Button scanning
#define BUT_UP_MAX  TICKS_MS(50)                // Min release time
#define BUT_DN_MAX  TICKS_MS(50)                // Min press time (short press)
#define BUT_DN_LONG TICKS_MS(3000)              // Long press time
#define BUT_DN_XLONG TICKS_MS(5000)             // Extra long press time        //1
uint but_dn_cnt[2];                             // Counter incremented by TimerCallback, to time duration press
uint but_up_cnt[2];                             // Counter incremented by TimerCallback, to time duration release
volatile uint8_t but_status[2];                 // Button status (e.g. BUT_STATUS_UP, see below)
#define BUT_STATUS_UP       0                   // Button released for min release time
#define BUT_STATUS_DN       1                   // Button pressed for min press time
#define BUT_STATUS_LONG     2                   // Button pressed for long press time
#define BUT_INTER_CLICK     3                   // Button released for more than inter-click time (so stop counting consecutive clicks and generate click event(s) now)
volatile uint8_t but_click[2];                  // Number of (consecutive) button clicks so far.
volatile uint8_t but_longp[2];                  // Flag is set when Long Press event occirs, and event(s) are generated.
                                                // Setting the flag means we don't try and generate any event on the button release.
volatile uint8_t but_event;                     // Bit-field for button events (e.g. MFB_1CLICK. See below).
#define MFB_1CLICK  0x01
#define MFB_2CLICK  0x02
#define MFB_3CLICK  0x04
#define MFB_LONGP   0x08
#define ENC_1CLICK  0x10
#define ENC_2CLICK  0x20
#define ENC_3CLICK  0x40
#define ENC_LONGP   0x80
const uint8_t events_1click[2] = {ENC_1CLICK, MFB_1CLICK};      // Define the event flags used for 1 click.
const uint8_t events_2click[2] = {ENC_2CLICK, MFB_2CLICK};      // Define the event flags used for 2 click.
const uint8_t events_3click[2] = {ENC_3CLICK, MFB_3CLICK};      // Define the event flags used for 3 click.
const uint8_t events_longp[2] = {ENC_LONGP, MFB_LONGP};         // Define the event flags used for long press.

// ........................................
// Timers
// ........................................

#define TMR_PWR_BUTTON_CHECK_VAL        TICKS_MS(5000)          // Monitor Left Encoder button for 3.5 seconds - waiting for long press event or button release
#define TMR_PWR_DELAY_POWER_ON_VAL      TICKS_MS(2000)          // Powering On everything, adding a short delay to allow Wear Sensor to initalise
#define TMR_PWR_DELAY_POWER_OFF_VAL     TICKS_MS(2000)          // Want to power down, but first give Airoha enough time to receive Power Off message
    
uint32_t tmr_pwr_dn_cnt;                                        // Used for down-counter for Power Task

// ........................................
// Left Encoder
// ........................................
bool b_l_enc_was_processed = false;
bool b_l_enc_was_pressed = false;
bool b_l_enc_was_released = false;

// .............................................................................
// Set of functions called by the Power Task state machine so it can know if 
// the MCU has been powered up by USB-C connection or by user butotn pressed
// (left encoder)
// .............................................................................
bool b_l_enc_clear_flags() {
    b_l_enc_was_processed = false;
    b_l_enc_was_pressed = false;
    b_l_enc_was_released = false;
}
// .............................................................................
bool b_l_enc_has_been_processed() {
    return (b_l_enc_was_processed);
}
// .............................................................................
bool b_l_enc_has_been_pressed() {
    return (b_l_enc_was_pressed);
}
// .............................................................................
bool b_l_enc_has_been_released() {
    return (b_l_enc_was_released);
}
// .............................................................................

// .............................................................................
// Encoder rotation detected (i.e. change of position)
// Position is a signed 16bit value.
// Negative if the rotation is counter-clock wise.
// Pass any event on to the main foreground task below, via the Q_enc queue.
// .............................................................................
void PDEC_callback(PDEC_QDEC_STATUS status, uintptr_t context)
{
    Q_but_t qs_enc;
    int pos, new_pos;

    new_pos = (int)(int16_t)PDEC_QDECPositionGet() / 2;
    if (new_pos != old_pos)
    {
        pos = new_pos - old_pos;
        old_pos = new_pos;
        if ((pos > 0 && enc_steps < 0) || (pos < 0 && enc_steps > 0)) { // sign change
            // generate the previous accumulated steps
            qs_enc.pos = enc_steps;
            qs_enc.event = LEFT_ENCODER; // generate a normal encoder event even if stop enabled
            osQueueSendToBackFromISR(Q_enc, &qs_enc);
            // start a new accumulation
            enc_steps = pos;
            enc_tick = ENCconfig[0].timeout + 1;
        }
        else {
            if (ENCconfig[0].timeout >= ENC_STOP_TIME) { // stop enabled
                enc_tick = ENCconfig[0].timeout; // start the timeout every step
            }
            else { // no stop
                if (enc_steps == 0) { // start the timeout at the first step
                    enc_tick = ENCconfig[0].timeout + 1; // make sure it's not zero
                }
            }
            enc_steps += pos;
        }
    }
}

// .............................................................................
// Called by Timer Callback function, when the callback function detects a 
// change in state of a button.
// Use this information to see if any button event (e.g. long press) needs
// to be signaled to the main task loop for processing.
//
// Inputs:
//  but:    0 - encoder button
//          1 - MFB
//  ev:     BUT_STATUS_DN - generated after .min_press_time
//          BUT_STATUS_LONG - generated after .long_press_time
//          BUT_STATUS_UP - generated after .min_release_time
//  The final events will be generated as bits in the but_event variable.
// .............................................................................
void but_event_manager(uint but, uint ev)
{
    if (ev == BUT_STATUS_DN) {
    }
    else if (ev == BUT_STATUS_LONG) {                       // Button Down Count reaches Long Press Count, and state was DOWN, so goes to LONG
        if (BUTconfig[but].long_press_enable) {             // Long Press enabled
            if (but_click[but] == 0) {                      // 1st click
                but_event |= events_longp[but];             // Flag the event: 0:ENC_LONGP, 1:MFB_LONGP
            }
            else if (but_click[but] == 1) {                 // 2nd click
                but_event |= events_2click[but];            // 0:ENC_2CLICK, 1:MFB_2CLICK
            }
            else if (but_click[but] == 2) {                 // 3rd click
                but_event |= events_3click[but];            // Flag the event: 0:ENC_3CLICK, 1:MFB_3CLICK
            }
            but_longp[but] = 1;                             // Long Press = true (this is set, so that the button release will be ignored))
            but_click[but] = 0;                             // Reset click count
        }
    }
    else if (ev == BUT_STATUS_UP) {                         // Button Up Count reaches Release Time, and state was DOWN or LONG
        if (but_longp[but] == 1) {                          // Clear long press state, if set - nothing else to do
            but_longp[but] = 0;                             // (i.e. we already generated whatever event(s) we needed to), so no event on release
        }
        else {                                              // Normal button release: counting quick/consecutive clicks
            // If click count (but_click[but]) reaches max configured click count (based on BUTconfig[but].click_no_enable)
            // then flag the event immediately (otherwise won't flag yet until next release, or inter-click timeout)
            but_click[but]++;                               // Increment click count
            if (but_click[but] == 1) {                      // 0 --> 1
                if (BUTconfig[but].click_no_enable == 0) {  // 1-click Enabled
                    but_event |= events_1click[but];        // Flag the event: 0:ENC_1CLICK, 1:MFB_1CLICK
            but_click[but] = 0;
        }
    }
            else if (but_click[but] == 2) {                 // 1 --> 2
                if (BUTconfig[but].click_no_enable == 1) {  // 2-click Enabled
                    but_event |= events_2click[but];        // Flag the event: 0:ENC_2CLICK, 1:MFB_2CLICK
                    but_click[but] = 0;
                }
            }
            else if (but_click[but] == 3) {                 // 2 --> 3
                if (BUTconfig[but].click_no_enable == 2) {  // 3-click Enabled
                    but_event |= events_3click[but];        // Flag the event: 0:ENC_3CLICK, 1:MFB_3CLICK
                    but_click[but] = 0;
                }
            }
        }
    }
    else if (ev == BUT_INTER_CLICK) {                       // Inter-click timeout, so end of any allowed sequence (too long between presses)
                                                            // Above, we would flag event immediately if button click count reached max configured
                                                            // But here, because of timeout, we just go with the current click count instead 
                                                            // (never reached max), and then reset everything
        if (but_click[but] == 1) {
            but_event |= events_1click[but];                // Flag the event: 0:ENC_1CLICK, 1:MFB_1CLICK
        }
        else if (but_click[but] == 2) {
            but_event |= events_2click[but];                // Flag the event: 0:ENC_2CLICK, 1:MFB_2CLICK
        }
        else if (but_click[but] == 3) {
            but_event |= events_3click[but];                // Flag the event: 0:ENC_3CLICK, 1:MFB_3CLICK
        }
        but_click[but] = 0;
    }
}

// .............................................................................
// ENCODER_L_KEY external interrupt
// .............................................................................
void EIC_PIN2_Callback(uintptr_t context)
{
    EIC_InterruptDisable(EIC_PIN_2);
    but_dn_cnt[0] = 0;
    but_up_cnt[0] = 0;
}

// .............................................................................
// MFB_L external interrupt
// .............................................................................
void EIC_PIN3_Callback(uintptr_t context)
{
    EIC_InterruptDisable(EIC_PIN_3);
    but_dn_cnt[1] = 0;
    but_up_cnt[1] = 0;
}

/*******************************************************************************
// LEDs (red and green)
 *******************************************************************************/

// .............................................................................
// Red LED - initialise
// .............................................................................
void led_red_init()
{
    led_info[LED_RED].mode = LED_MODE_OFF;
    led_info[LED_RED].duty_lim = 0;
    led_info[LED_RED].rpt_lim = 0;
    led_info[LED_RED].state = LED_STATE_OFF;
    led_info[LED_RED].duty_cnt = 0;
    led_info[LED_RED].rpt_cnt = 0;

    TC7_Compare8bitMatch1Set(0);
    TC7_CompareStart();
}

// .............................................................................
// Red LED - update (using PWM)
// .............................................................................
void led_red_pwm(uint8_t pwm)
{
    TC7_CompareStop();
    TC7_Compare8bitMatch1Set(pwm);
    TC7_CompareStart();
}

// .............................................................................
// Green LED - initialise
// .............................................................................
void led_green_init()
{
    led_info[LED_GREEN].mode = LED_MODE_OFF;
    led_info[LED_GREEN].duty_lim = 0;
    led_info[LED_GREEN].rpt_lim = 0;
    led_info[LED_GREEN].state = LED_STATE_OFF;
    led_info[LED_GREEN].duty_cnt = 0;
    led_info[LED_GREEN].rpt_cnt = 0;

    TC4_Compare8bitMatch1Set(0);
    TC4_CompareStart();
}

// .............................................................................
// Green LED - update (using PWM)
// .............................................................................
void led_green_pwm(uint8_t pwm)
{
    TC4_CompareStop();
    TC4_Compare8bitMatch1Set(pwm);
    TC4_CompareStart();
}

// ******************************************************************************
// LEDs (microphone)
// ******************************************************************************

// .............................................................................
// Microphone LED - initialise
// .............................................................................
void led_mic_init()
{
    led_info[LED_MIC].mode = LED_MODE_OFF;
    led_info[LED_MIC].duty_lim = 0;
    led_info[LED_MIC].rpt_lim = 0;
    led_info[LED_MIC].state = LED_STATE_OFF;
    led_info[LED_MIC].duty_cnt = 0;
    led_info[LED_MIC].rpt_cnt = 0;

    //TC0_Compare8bitMatch1Set(0);
    //TC0_CompareStart();
    LED_MIC_Clear();
}

// .............................................................................
// Microphone LED - turn off or on
// .............................................................................
void led_mic_pwm(uint8_t pwm)
{
    //TC0_CompareStop();
    //TC0_Compare8bitMatch1Set(pwm);
    //TC0_CompareStart();
    if (pwm == 0) {
        LED_MIC_Clear();
    } else {
        LED_MIC_Set();
    }
}

// ******************************************************************************
// LEDs (display brightness)
// ******************************************************************************

// .............................................................................
// Initialise display brightness
// .............................................................................
void led_disp_init()
{
    LCD_LED_L_Clear();
    osDelayMs(5);
}

// .............................................................................
// Set PWM value for LED brightness
// .............................................................................
void led_disp_stepdown()
{
    NVIC_INT_Disable();
    LCD_LED_L_Clear();
    __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
    __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
    __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
    __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
    LCD_LED_L_Set();
    __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
    __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
    __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
    __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
    NVIC_INT_Enable();
}

// .............................................................................
// Set display background brightness
// .............................................................................
void led_disp_pwm(uint8_t pwm)
{
    led_disp_init();
    if (pwm) {
        if (pwm > 7) pwm = 7;
        pwm = 17 - pwm;
        while (pwm--) {
            led_disp_stepdown();
        }
    }
    osDelayMs(5);
}

// ******************************************************************************
// LEDs
// ******************************************************************************

// .............................................................................
// Initialise variables associated with each LED, and set LEDs to initial 
// states.
// .............................................................................
void led_mngr_init()
{
    int i;

    for (i = 0; i <= 2; i++) {
        led_info[i].state = LED_STATE_OFF;
        led_info[i].mode = LED_MODE_OFF;
        led_info[i].duty_lim = 0;
        led_info[i].rpt_lim = 0;
        led_new[i].mode = LED_MODE_NONE;
    }
    led_info[LED_RED].mode = LED_MODE_OFF;
    led_red_pwm(0xFF);
    led_green_pwm(0x00);
    led_mic_pwm(0x00);
}

// .............................................................................
// LED Manager Update
// Called from the LED Update Manager, whenever it needs to set the state of a 
// specific LED.
//
// Inputs:
//  id      LED ID, [0] red, [1] green, [2] mic
//  state   ON or OFF
// .............................................................................
void led_mngr_set(uint8_t id, uint8_t state)
{
    switch (id) {
        case LED_RED:
            if (state == LED_STATE_ON) {
                led_red_pwm(0xFF);
            } else {
                led_red_pwm(0x00);
            }
            break;
        case LED_GREEN:
            if (state == LED_STATE_ON) {
                led_green_pwm(0xFF);
            } else {
                led_green_pwm(0x00);
            }
            break;
        case LED_MIC:
            if (state == LED_STATE_ON) {
                led_mic_pwm(0xFF);
            } else {
                led_mic_pwm(0x00);
            }
            break;
        default:
            break;
    }
}

// .............................................................................
// LED Manager Update
// Called on a scheduled basis (every 10 ms).
// This update function has two main jobs.
// 1. Manage any active LEDsa (timing, blinking etc.)
// 2. Check if any new LED settings received.
//
// LED numbering: [0] red, [1] green, [2] mic
// .............................................................................
void led_mngr_update() {
    int i;
    bool b_check_now = true;

    // 1. Manage any active LEDsa (timing, blinking etc.)
    for (i = 0; i <= 2; i++) {

        if (led_info[i].mode == LED_MODE_OFF) {
            // Verify LED is OFF
            if (led_info[i].state == LED_STATE_ON) {
                led_info[i].state = LED_STATE_OFF;
                led_mngr_set(i, LED_STATE_OFF);
            }
        } else if (led_info[i].mode == LED_MODE_ON) {
            // Verify LED is ON
            if (led_info[i].state == LED_STATE_OFF) {
                led_info[i].state = LED_STATE_ON;
                led_mngr_set(i, LED_STATE_ON);
            }
            if (led_info[i].duty_lim > 0) {
                // Not on forever, so see if have flashed enough
                if (++led_info[i].duty_cnt >= led_info[i].duty_lim) {
                    // Flash Mode is over
                    // Set to default LED state (On or Off forever)
                    if (led_default[i] == LED_MODE_OFF) {
                        led_info[i].mode = LED_MODE_OFF;
                        led_info[i].duty_lim = 0;
                        led_mngr_set(i, LED_STATE_OFF);                      
                    } else {
                        led_info[i].mode = LED_MODE_ON;
                    led_info[i].duty_lim = 0;
                        led_mngr_set(i, LED_STATE_ON);                      
                    }

                }
            }
        } else {
            // Flashing
            if (++led_info[i].duty_cnt >= led_info[i].duty_lim) {
                led_info[i].duty_cnt = 0;
                // Time to toggle LED
                if (led_info[i].state == LED_STATE_OFF) {
                    led_info[i].state = LED_STATE_ON;
                    led_mngr_set(i, LED_STATE_ON);
                } else {
                    led_info[i].state = LED_STATE_OFF;
                    led_mngr_set(i, LED_STATE_OFF);
                    if (led_info[i].rpt_lim > 0) {
                        // Not flashing forever, so see if have flashed enough
                        if (++led_info[i].rpt_cnt >= led_info[i].rpt_lim) {
                            // Flash Mode is over
                            // Set to default LED state (On or Off forever)
                            if (led_default[i] == LED_MODE_OFF) {
                                led_info[i].mode = LED_MODE_OFF;
                            led_info[i].duty_lim = 0;
                                led_mngr_set(i, LED_STATE_OFF);                      
                            } else {
                                led_info[i].mode = LED_MODE_ON;
                            led_info[i].duty_lim = 0;
                                led_mngr_set(i, LED_STATE_ON);                      
                            }
                        }
                    }
                }
            } else {
                // Don;t check for new LED setting while mid-cycle
                b_check_now = false;
            }
        }
    }

    // 2. Check if any new LED settings received.
    if (b_check_now) {
        // See if any LEDs mode etc. has been modified
        for (i = 0; i <= 2; i++) {
            if (led_new[i].mode != LED_MODE_NONE) {
                // LED mode etc. has been updated
                led_info[i].mode = led_new[i].mode;
                led_info[i].duty_lim = led_new[i].duty_lim;
                led_info[i].rpt_lim = led_new[i].rpt_lim;
                led_info[i].state = LED_STATE_OFF;
                led_info[i].duty_cnt = 0;
                led_info[i].rpt_cnt = 0;
                // Turn LED off
                led_mngr_set(i, LED_STATE_OFF);
                led_new[i].mode = LED_MODE_NONE;
            }
        }
    }
}

// .............................................................................
// LED Manager New
// Called when LED Set message received in Queue
// Set up LED info in led_new[]
// The led update manager (which is called on a scheduled basis) will compare 
// the existing LED mode/settings with the new/requested settings, and update 
// the LED's settings accordingly.
// .............................................................................
void led_mngr_new(Q_led_t led_cfg)
{
    int i;
    uint8_t led_new_mode = LED_MODE_NONE;

    i = led_cfg.id;

    if (i > 2) {
        // Invalid LED Id: [0] red, [1] green, [2] mic
        return;
    }
    if (led_cfg.mode == LED_MODE_OFF) {
        led_new_mode = LED_MODE_OFF;
    }
    else if (led_cfg.mode == LED_MODE_ON) {
        led_new_mode = LED_MODE_ON;
        led_new[i].duty_lim = led_cfg.dur;
    }
    else if (led_cfg.mode == LED_MODE_FLASH) {
        led_new_mode = LED_MODE_FLASH;
        if ((led_cfg.dur == 0) && (led_cfg.cnt == 0)) {
            // Flash forever
            if (led_cfg.rate == LED_RATE_FAST) {    // 2Hz: 1 period = 500 ms, 1 cycle = 250 ms
                led_new[i].duty_lim = TICKS_MS(250);// 250ms
            } else {                                // 1 Hz: 1 period = 1000 ms, 1 cycle = 500 ms
                led_new[i].duty_lim = TICKS_MS(500);// 500ms
            }
            led_new[i].rpt_lim = 0;
        } else {
            // Convert rate to duty
            if (led_cfg.cnt == 0) {
                // Flash for DURATION (ms))
                if (led_cfg.rate == LED_RATE_FAST) {    // 2Hz: 1 period = 500 ms, 1 cycle = 250 ms
                    led_new[i].duty_lim = TICKS_MS(250);// 250ms
                    led_new[i].rpt_lim = led_cfg.dur / TICKS_MS(500);
                } else {                                // 1 Hz: 1 period = 1000 ms, 1 cycle = 500 ms
                    led_new[i].duty_lim = TICKS_MS(500);// 500ms
                    led_new[i].rpt_lim = led_cfg.dur / TICKS_MS(1000);
                }
            } else {
                // Flash for COUNT
                if (led_cfg.rate == LED_RATE_FAST) {    // 2 Hz
                    led_new[i].duty_lim = TICKS_MS(250);
                    led_new[i].rpt_lim = led_cfg.cnt;
                } else {                                // 1 Hz: 1 period = 1000 ms, 1 cycle = 500 ms
                    led_new[i].duty_lim = TICKS_MS(500);
                    led_new[i].rpt_lim = led_cfg.cnt;
                }
            }
        }
    }
    led_new[i].duty_cnt = 0;
    led_new[i].rpt_cnt = 0;
    led_new[i].mode = led_new_mode;
}

// .............................................................................
// LED Set State
// Update the state/mode of the specified LED.
// This might be a Left MCYU LED, or a Right MCU LED.
// Either way, pass the information on to the Encoder task via the Q_led queue.
//
// Inputs:
//  qs_led:  structure to hold all the parameters and to pass on to the queue
//  id:      LED ID
//  mode:    On / OFF / Flash
//  rate:    (if flashing) Slow / Fast
//  cnt:     (if flashing) number of flashes
//  dur:     if flashing: number of 10ms ticks, instead of count  (0 = forever)
//              if ON, number of 10ms ticks to stay on (0 = forever)
// .............................................................................
void led_set_state(Q_led_t qs_led, uint8_t id, uint8_t mode, uint8_t rate, uint8_t cnt, uint16_t dur)
{
    qs_led.id = id;
    qs_led.mode = mode;
    qs_led.rate = rate;
    qs_led.cnt = cnt;
    qs_led.dur = dur;
    osQueueSendToBack(Q_led, &qs_led, 10);
}

// .............................................................................
// Start the down-counter used by the POwer Task
// Input: 
//  tmr_type    Indicates time duration required
// .............................................................................
void enc_power_task_timer_start(uint8_t tmr_type)
{
    uint16_t tim_s;
    uint32_t tim_ms;
    bool tim_en;
    
    // Assume we won't actually set the timer / down counter
    tmr_pwr_dn_cnt = 0;
    
    switch (tmr_type) {
        case TMR_PWR_BUTTON_CHECK:
            tmr_pwr_dn_cnt = TMR_PWR_BUTTON_CHECK_VAL;
            break;
        case TMR_PWR_DELAY_POWER_ON:
            tmr_pwr_dn_cnt = TMR_PWR_DELAY_POWER_ON_VAL;
            break;
        case TMR_PWR_STANDBY:
            // Only set the counter if Auto Power Off is enabled. and power off
            // time is within range
            tim_en = json_data_auto_pwr_off_en_read();
            if (tim_en == false) {
                // Not enabled
                break;
            }
            tim_s = json_data_auto_pwr_off_tim_read();
            // Standby time is 1/4 of full standby time
            // (Resting time is the other 3/4))
            if ((tim_s < APWOFF_TIME_MIN) || (tim_s > APWOFF_TIME_MAX)) {
                // Out of range
                break;
            }
            // COnvert ONE QUARTER of timeout to milliseconds
            tim_ms = tim_s * 250;
            tmr_pwr_dn_cnt = TICKS_MS(tim_ms);
            break;
        case TMR_PWR_RESTING:
            // Only set the counter if Auto Power Off is enabled. and power off
            // time is within range
            tim_en = json_data_auto_pwr_off_en_read();
            if (tim_en == false) {
                // Not enabled
                break;
            }
            tim_s = json_data_auto_pwr_off_tim_read();
            // Resting time is 3/4 of full standby time
            if ((tim_s < APWOFF_TIME_MIN) || (tim_s > APWOFF_TIME_MAX)) {
                // Out of range
                break;
            }
            // COovert THREE QUARTERS of timeout to milliseconds
            tim_ms = tim_s * 750;
            tmr_pwr_dn_cnt = TICKS_MS(tim_ms);
            //tmr_pwr_dn_cnt = TMR_PWR_STANDBY_VAL;
            break;
        case TMR_PWR_DELAY_POWER_OFF:
            tmr_pwr_dn_cnt = TMR_PWR_DELAY_POWER_OFF_VAL;
            break;
        default:
            break;
    }
}

// .............................................................................
// Cancel the down-counter used by the Power Task (for timing)
// .............................................................................
void enc_power_task_timer_stop()
{
    tmr_pwr_dn_cnt = 0;
}

// .............................................................................
// Callback timer.
// This is configured to be called every 10 ms.
// Check for any button state changes and call button event manager if there
// are.
// Also check for encoder rotation inactivity ('enc_tick' down-counter reaches 
// zero) and pass this event on to the main foregroud task loop below (via 
// the Q_enc queue))
// .............................................................................
void TimerCallback(TC_TIMER_STATUS status, uintptr_t context)
{
    Q_but_t qs_enc;
    Q_but_t qs_but;
    
    b_l_enc_was_processed = true;
    
    // BUTTONS
    // ENCODER_L_KEY
    if (ENCODER_L_KEY_Get()) {
        // button up
        if (b_l_enc_was_pressed) {
            // Button has been pressed, and is now released, so remember this
            // (used in power-up state machine)
            b_l_enc_was_released = true;
        }
        but_dn_cnt[0] = 0;                                                              // Up/Down Counters
        but_up_cnt[0]++;                                                                // Note: Can flag EITHER event depending on UP counter
        if (but_up_cnt[0] == BUTconfig[0].min_release_time) {
            if (but_status[0] == BUT_STATUS_DN || but_status[0] == BUT_STATUS_LONG) {
                but_status[0] = BUT_STATUS_UP;                                          // UP status & event
                but_event_manager(0, BUT_STATUS_UP);
            }
            EIC_InterruptEnable(EIC_PIN_2);
        }
        else if (but_up_cnt[0] == BUTconfig[0].inter_click_time) {
            but_event_manager(0, BUT_INTER_CLICK);                                      // Inter-click timeout (if counting sequence)
        }
    }
    else {
        // button down
        b_l_enc_was_pressed = true;
        but_up_cnt[0] = 0;                                                              // Up/Down Counters
        but_dn_cnt[0]++;                                                                // Note: Can flag EITHER event depending on DOWN counter
        if (but_dn_cnt[0] == BUTconfig[0].min_press_time) {
            if (but_status[0] == BUT_STATUS_UP) {
                but_status[0] = BUT_STATUS_DN;                                          // DOWN status & event
                but_event_manager(0, BUT_STATUS_DN);
            }
        }
        else if (but_dn_cnt[0] == BUTconfig[0].long_press_time) {
            if (but_status[0] == BUT_STATUS_DN) {
                but_status[0] = BUT_STATUS_LONG;                                        // LONG status & event
                but_event_manager(0, BUT_STATUS_LONG);
            }
        }
    }

    // MFB_L
    if (MFB_L_Get()) {
        // button up
        but_dn_cnt[1] = 0;                                                              // Up/Down Counters
        but_up_cnt[1]++;                                                                // Note: Can flag EITHER event depending on UP counter
        if (but_up_cnt[1] == BUTconfig[1].min_release_time) {
            if (but_status[1] == BUT_STATUS_DN || but_status[1] == BUT_STATUS_LONG) {
                but_status[1] = BUT_STATUS_UP;                                          // UP status & event
                but_event_manager(1, BUT_STATUS_UP);
            }
            EIC_InterruptEnable(EIC_PIN_3);
        }
        else if (but_up_cnt[1] == BUTconfig[1].inter_click_time) {
            but_event_manager(1, BUT_INTER_CLICK);                                      // Inter-click timeout (if counting sequence)
        }
    }
    else {
        // button down
        but_up_cnt[1] = 0;                                                              // Up/Down Counters
        but_dn_cnt[1]++;
        if (but_dn_cnt[1] == BUTconfig[1].min_press_time) {
            if (but_status[1] == BUT_STATUS_UP) {
                but_status[1] = BUT_STATUS_DN;                                          // DOWN status & event
                but_event_manager(1, BUT_STATUS_DN);
            }
        }
        else if (but_dn_cnt[1] == BUTconfig[1].long_press_time) {
            if (but_status[1] == BUT_STATUS_DN) {
                but_status[1] = BUT_STATUS_LONG;                                        // LONG status & event
                but_event_manager(1, BUT_STATUS_LONG);
            }
        }
    }

    // ENCODER
    if (enc_tick) {
        if (--enc_tick == 0) {
            qs_enc.pos = enc_steps;
            qs_enc.event = (ENCconfig[0].timeout >= ENC_STOP_TIME) ? LEFT_ENCODER_STOP : LEFT_ENCODER;
            osQueueSendToBackFromISR(Q_enc, &qs_enc);
            enc_steps = 0;
        }
    }

    // LEDS
    led_mngr_update();

    if (lr_timeout) {                       // Timeout was set to 200 ms on any Left or Right 1 click occurrence
        if (--lr_timeout == 0) {            // When it times out, will check left and right 1 click flags
            lr_done = 1;                    // Will generate either Left 1 CLick event OR Right 1 Click event OR Left&Right 1 Click event
        }
    }
    
    // Timers
    // Down-counter used by the Power Task for timing
    if (tmr_pwr_dn_cnt > 0) {                               // Allowing a number of seconds after sending Power Down message to Airoha,
        tmr_pwr_dn_cnt--;                                   // before actually powering down.
        if (tmr_pwr_dn_cnt == 0) {
            // Time to actually power down now
            qs_but.event = POWER_TIMEOUT;                   // Time up, so power down now
            osQueueSendToBackFromISR(Q_pwr, &qs_but);
        }
    }
    
    // Manage delay counter for comms tx to AIroha
    comms_tx_delay_manage();
}


// .............................................................................
// Initialise the left and right buttons, encoders, LEDs, and any timers 
// and callback functions required.
// .............................................................................
void enc_init()
{
    button_config_t but_config;

    // Initialise data and modules (e.g. comparator) associated with the LEDs, 
    // Microphone, DSP.
    // The left side encoder, buttons and leds are on the 1V8 power domain of 
    // the MCU leds
    led_red_init();
    led_green_init();
    led_mic_init();
    led_disp_init();
    led_mngr_init();

    // Configure: Left Encoder
    ENCconfig[0].steps_per_revolution = 20;
    ENCconfig[0].timeout = TICKS_MS(50);

    // Configure: Right Encoder
    ENCconfig[1].steps_per_revolution = 20;
    ENCconfig[1].timeout = TICKS_MS(50);
    // ... and send to RIght MCU
    memset(&but_config, 0, sizeof(button_config_t));
    but_config.click_no_enable = 0; // this is the encoder mark
    but_config.inter_click_time = ENCconfig[1].steps_per_revolution;
    but_config.long_press_time = ENCconfig[1].timeout;
    osQueueSendToBack(Q_mcu_init, &but_config, 10);

    // Configure: Left Encoder callback function
    PDEC_QDECCallbackRegister( PDEC_callback, 0 );
    PDEC_QDECStart();
    old_pos = (int)(int16_t)PDEC_QDECPositionGet();

    // Configure: ENCODER_L_KEY
    BUTconfig[0].click_no_enable = 3;
    BUTconfig[0].long_press_enable = 1;
    BUTconfig[0].min_press_time = BUT_DN_MAX;
    BUTconfig[0].min_release_time = BUT_UP_MAX;
    BUTconfig[0].long_press_time = BUT_DN_LONG;
    BUTconfig[0].inter_click_time = TICKS_MS(250);

    // Configure: MFB_L
    BUTconfig[1].click_no_enable = 2;
    BUTconfig[1].long_press_enable = 1;
    BUTconfig[1].min_press_time = BUT_DN_MAX;
    BUTconfig[1].min_release_time = BUT_UP_MAX;
    BUTconfig[1].long_press_time = BUT_DN_LONG;
    BUTconfig[1].inter_click_time = TICKS_MS(250);

    // Configure: ENCODER_R_KEY
    BUTconfig[2].click_no_enable = 3;
    BUTconfig[2].long_press_enable = 1;
    BUTconfig[2].min_press_time = BUT_DN_MAX;
    BUTconfig[2].min_release_time = BUT_UP_MAX;
    BUTconfig[2].long_press_time = BUT_DN_LONG;
    BUTconfig[2].inter_click_time = TICKS_MS(250);
    // ... and send to RIght MCU
    memcpy(&but_config, &BUTconfig[2], sizeof(button_config_t));
    but_config.click_no_enable += 100; // this is the ENCODER_R_KEY mark
    osQueueSendToBack(Q_mcu_init, &but_config, 10);

    // Configure: MFB_R
    BUTconfig[3].click_no_enable = 1;
    BUTconfig[3].long_press_enable = 1;
    BUTconfig[3].min_press_time = BUT_DN_MAX;
    BUTconfig[3].min_release_time = BUT_UP_MAX;
    BUTconfig[3].long_press_time = BUT_DN_LONG;
    BUTconfig[3].inter_click_time = TICKS_MS(250);
    // ... and send to RIght MCU
    memcpy(&but_config, &BUTconfig[3], sizeof(button_config_t));
    but_config.click_no_enable += 200; // this is the MFB_R mark
    osQueueSendToBack(Q_mcu_init, &but_config, 10);

    // Set up the call back functions
    TC1_TimerCallbackRegister(TimerCallback, 0);
    EIC_CallbackRegister(EIC_PIN_2, EIC_PIN2_Callback, 0);
    EIC_CallbackRegister(EIC_PIN_3, EIC_PIN3_Callback, 0);
    EIC_InterruptEnable(EIC_PIN_2);
    EIC_InterruptEnable(EIC_PIN_3);
    // Start the timer
    TC1_TimerStart();   
}
// .............................................................................
// Task initializations.
// This function runs BEFORE task is created and scheduler started.
// .............................................................................
void APP_ENC_Initialize ( void )
{
}

// .............................................................................
// The Encoder task
// .............................................................................
void APP_ENC_Tasks ( void )
{
    Q_but_t qs_enc;
    Q_but_t qs_but;
    Q_led_t qs_led;
    uint8_t butev;

    // Task waits until the necessary Queues have been enabled
    while (Q_enc == NULL) osDelayMs(10);
    while (Q_bm_but == NULL) osDelayMs(10);         // button manager - button events
    while (Q_bm_comm == NULL) osDelayMs(10);        // button manager - from comms
    while (Q_enc_mcu == NULL) osDelayMs(10);
    while (Q_mcu_init == NULL) osDelayMs(10);
    while (Q_led == NULL) osDelayMs(10);

    // Initialise the left and right buttons, encoders, LEDs, and any timers 
    // and callback functions required.
    enc_init();

    while(1) {
        
        osDelayMs(1);

        //------------------------------------------------------
        // Local Encoder
        // Process any button event messages received from the 
        // Left Encoder call back function. Pass these on to the 
        // button manager.
        //------------------------------------------------------
        if (osQueueReceive(Q_enc, &qs_enc, 10) == pdPASS) {
            qs_but.event = qs_enc.event;
            qs_but.pos = qs_enc.pos;
            osQueueSendToBack(Q_bm_but, &qs_but, 10);
        }

        //------------------------------------------------------
        // Local Buttons
        // Process any button events (e.g. double-click detected) 
        // indicated by the button event manager, and send these 
        // on to the button manager.
        //------------------------------------------------------
        // Get the button events
        NVIC_DisableIRQ(TC1_IRQn);
        butev = but_event;
        but_event = 0;
        NVIC_EnableIRQ(TC1_IRQn);
        // Send the events to the Button Manager
        qs_but.pos = 0;
        if (butev & MFB_1CLICK) {
            qs_but.event = LEFT_MFB_1CLICK;
            osQueueSendToBack(Q_bm_but, &qs_but, 10);
        }
        if (butev & MFB_2CLICK) {
            qs_but.event = LEFT_MFB_2CLICK;
            osQueueSendToBack(Q_bm_but, &qs_but, 10);
        }
        if (butev & MFB_3CLICK) {
            qs_but.event = LEFT_MFB_3CLICK;
            osQueueSendToBack(Q_bm_but, &qs_but, 10);
        }
        if (butev & MFB_LONGP) {
            qs_but.event = LEFT_MFB_LONGPRESS;
            osQueueSendToBack(Q_bm_but, &qs_but, 10);
        }
        if (butev & ENC_1CLICK) {                                       // Left Encoder 1-click: Don't immediately send event to button manager.
            lr_left = LEFT_ENC_1CLICK;                                  // Instead, set lr_left flag (to remember) and reset lt_timeout down timer
            lr_timeout = LR_TIMEOUT;                                    // Same happens below, if rx Right 1 Click event from Right MCU
        }
        if (butev & ENC_2CLICK) {
            qs_but.event = LEFT_ENC_2CLICK;
            osQueueSendToBack(Q_bm_but, &qs_but, 10);
        }
        if (butev & ENC_3CLICK) {
            qs_but.event = LEFT_ENC_3CLICK;
            osQueueSendToBack(Q_bm_but, &qs_but, 10);
        }
        if (butev & ENC_LONGP) {
            qs_but.event = LEFT_ENC_LONGPRESS;
            osQueueSendToBack(Q_bm_but, &qs_but, 10);
        }

        //------------------------------------------------------
        // Remote Encoder and Buttons
        // Process any button events received from the Right MCU 
        // (via the Q_enc_mcu queue), and pass these events on 
        // to the button manager.
        //------------------------------------------------------
        if (osQueueReceive(Q_enc_mcu, &qs_enc, 10) == pdPASS) {
            if (qs_enc.event == RIGHT_ENC_1CLICK) {                     // Same as Left 1 Click event. Don't pass on to button manager yet.
                lr_right = RIGHT_ENC_1CLICK;                            // Instead, set lr_right flag (to remember) and reset lt_timeout down timer
                lr_timeout = LR_TIMEOUT;                                // Same happens above, if rx Left 1 Click event
            } else {
                qs_but.event = qs_enc.event;
                qs_but.pos = qs_enc.pos;
                osQueueSendToBack(Q_bm_but, &qs_but, 10);
            }
        }

        // Manage button inactivity timeout (see lr_done, lr_timeout), so we 
        // can identify single button click events (including left AND right 
        // buttons pressed at same time). Notify button manager of any of 
        // these events.
        if (lr_done) {                                                          // 200 ms has expired. See if received Left and/or Right 1 click events in that time.
            lr_done = 0;                                                        // Note: Can generate all three events here (individual 1 click, and combined 1 click)
            if (lr_left == LEFT_ENC_1CLICK && lr_right == RIGHT_ENC_1CLICK) {   // Both!
                qs_but.event = LEFT_RIGHT_ENC_1CLICK;
                osQueueSendToBack(Q_bm_but, &qs_but, 10);    
                lr_left = 0;
                lr_right = 0;
            }
            if (lr_left == LEFT_ENC_1CLICK) {                                   // Left
                lr_left = 0;
                qs_but.event = LEFT_ENC_1CLICK;
                osQueueSendToBack(Q_bm_but, &qs_but, 10);
            }
            if (lr_right == RIGHT_ENC_1CLICK) {                                 // Right
                lr_right = 0;
                qs_but.event = RIGHT_ENC_1CLICK;
                osQueueSendToBack(Q_bm_but, &qs_but, 10);
            }            
        }
        
        //------------------------------------------------------
        // LEDs
        // Process any LED Update messages received in the Q_led 
        // queue (see the led_set_state() function). For Blue LED 
        // updates, pass these on to the Right MCU via the 
        // Q_mcu_led queue. For Left MCY LED updates, call 
        // led_mngr_new(0 to update the LED feedback.
        //------------------------------------------------------
        if (osQueueReceive(Q_led, &qs_led, 10) == pdPASS) {
            // Process the received LED message
            if (qs_led.id == LED_BLUE) {
                // Pass on to the RIght MCU
                osQueueSendToBack(Q_mcu_led, &qs_led, 10);
            } else {
                // LED is on the Left Side
            led_mngr_new(qs_led);
        }
        }
    }
}
// .............................................................................
