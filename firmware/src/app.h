/* app.h */
#ifndef _APP_H
#define _APP_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "configuration.h"
#include "definitions.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "fifo.h"

/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif

#define osDelayMs(x)  vTaskDelay(x/portTICK_PERIOD_MS)

#define osQueueCreate(n,z)  xQueueCreate(n,z)
#define osQueueSendToBack(q,x,t)  xQueueSendToBack(q,(void*)x,(TickType_t)t)
#define osQueueSendToBackFromISR(q,x)  xQueueSendToBackFromISR(q,(void*)x,NULL)
#define osQueueReceive(q,x,t)  xQueueReceive(q,x,(TickType_t)t)    
   
// .............................................................................
// Global milliseconds counter
// .............................................................................
extern volatile uint32_t MS;

// .............................................................................
// Port to IMME JSON implementation
// .............................................................................
#define LOG_INF printf
//#define LOG_INF(...)
#define k_sleep(x)
    
//#define DBGprintf printf    
#define DBGprintf(...)
    
// .............................................................................
// Power
// .............................................................................
extern int GlobalPowerOn;    
extern int GlobalSleep;   


#define OS_SLEEP(x) do{ do{ osDelayMs(x); }while(GlobalSleep); }while(0)

// .............................................................................
//* Timers
// .............................................................................

#define TMR_PWR_BUTTON_CHECK    1
#define TMR_PWR_DELAY_POWER_ON  2
#define TMR_PWR_STANDBY         3
#define TMR_PWR_RESTING         4
#define TMR_PWR_DELAY_POWER_OFF 5

void enc_power_task_timer_start(uint8_t tmr_id);
void enc_power_task_timer_stop();

void comms_tx_delay_manage();

// .............................................................................
// JSON 
// .............................................................................
typedef struct {
    uint8_t id;
    uint8_t send_num;
    uint8_t err_no;
    uint16_t seg_no;
    uint16_t seg_tot;
} Q_comm_jsn_t;

void mcu_ser_num_get(char *ser_str);

// .............................................................................
// buttons
// .............................................................................
typedef struct {
    uint8_t click_no_enable;    // 0,1,2
    uint8_t long_press_enable;  // 0,1
    uint8_t min_press_time;     // 10ms ticks
	uint8_t min_release_time;   // 10ms ticks
    uint16_t inter_click_time;  // 10ms ticks
    uint16_t long_press_time;   // 10ms ticks
} button_config_t;

extern button_config_t BUTconfig[4];

extern bool b_l_enc_clear_flags();
extern bool b_l_enc_has_been_processed();
extern bool b_l_enc_has_been_pressed();
extern bool b_l_enc_has_been_released();

// .............................................................................
// Accelerometer and Wear Sensor
// .............................................................................
typedef struct {
    int wear_status;            // Wear Sensor STatus: Near / Far / Init (needs cOnfig)
    bool b_accel;               // true if accelerometer registered movement in last Sensor Task cycle
} Q_sensors_t;

extern void accel_sensitivity(uint8_t thd, uint8_t win);

#define ACCEL_LO_SENSE_THD      8
#define ACCEL_LO_SENSE_WIN      8
#define ACCEL_HI_SENSE_THD      2
#define ACCEL_HI_SENSE_WIN      2

// .............................................................................
// LED Mngr
// .............................................................................
typedef struct {
    uint8_t state;      // Off / On
    uint8_t mode;       // Off / On / Flash
    uint16_t duty_cnt;  // 10ms, Counter incremented every 10ms
    uint16_t duty_lim;  // 10ms, When counter reaches limit, if ON turn OFF, if FLASHING toggle state
    uint8_t rpt_cnt;    // (if flashing) Increment 'repeat counter' every On->Off cycle
    uint8_t rpt_lim;    // (if flashing) Flash Mode over when 'repeat counter' reaches this limit
} led_info_t;

 // LED Config
#define LED_MODE_NONE   0x01
#define LED_MODE_OFF    0x02
#define LED_MODE_ON     0x04
#define LED_MODE_FLASH  0x08
#define LED_RATE_SLOW   0x10
#define LED_RATE_FAST   0x20

#define WEAR_THRESHOLD_HI   0x7530      // 30,000
#define WEAR_THRESHOLD_LO   0x2710      // 10,000


// .............................................................................
// led id's
// .............................................................................
#define LED_RED     0       // Left
#define LED_GREEN   1       // Left
#define LED_MIC     2       // Left
#define LED_BLUE    3       // Right

#define TICKS_MS(x) (x/10)

// .............................................................................
// encoders
// .............................................................................
typedef struct {
    uint16_t steps_per_revolution;  // usually 20, 30, hardware property
	uint16_t timeout;               // 10ms ticks
} encoder_config_t;

#define ENC_STOP_TIME (500 / 10)    // 500ms

extern encoder_config_t ENCconfig[2];

// .............................................................................
// Buttons and encoders queue
// .............................................................................
typedef struct {
    uint8_t event;
    int pos;
} Q_but_t;

/* LEDs */
typedef struct {
    uint8_t id;     // LED Id
    uint8_t mode;   // LED Mode (Off, On, Flash))
    uint8_t rate;   // (if flashing) LED Rate (Slow , Fast)
    uint8_t cnt;    // (if flashing) LED Count
    uint16_t dur;   // LED Duration (10ms ticks)
} Q_led_t;

void led_set_state(Q_led_t qs_led, uint8_t id, uint8_t mode, uint8_t rate, uint8_t cnt, uint16_t dur);

// .............................................................................
// ANC Modes
// .............................................................................
//#define DSP_ANC_MODE_UNDEFINED          0
//#define DSP_ANC_MODE_NOISE_CANCELATION  1
//#define DSP_ANC_MODE_TRANSPARENT        2
//#define DSP_ANC_MODE_NORMAL             3
//
//
//#define DMICS_ALL_OFF                   0
//#define DMIC0_ON                        1
//#define DMIC1_ON                        2
//#define DMIC2_ON                        3
//#define DMIC3_ON                        4
//#define DMIC4_ON                        5
//#define DMIC5_ON                        6
//#define DMICS_ALL_ON                    7
//
//
//extern void dsp_anc_mode_cycle();

// .............................................................................
// Microphones
// .............................................................................

typedef struct {
    uint8_t event;
} Q_mic_t;
/*
extern void dsp_using_boom_mic();
extern void dsp_using_dmics();
extern void dsp_mute_all_mics();
extern void dsp_talk_mics_enable();
extern void dsp_talk_mics_disable();
extern void dsp_boom_mic_enable();
extern void dsp_boom_mic_disable();
extern void dsp_eq_presets_update();
*/

// .............................................................................
// Power
// .............................................................................
#define POWER_ON_OFF    0x01
#define BAT_UPDATE      0x02
#define BAT_STATUS      0x03
#define SLEEP_ON_OFF    0x04
#define POWER_TIMEOUT   0x08

#define APWOFF_TIME_MIN     1               // Power off time (seconds) - min value
#define APWOFF_TIME_MAX     7200            // Power off time (seconds) - max valuer
#define APWOFF_TIME_DEFAULT 1800            // Power off time (seconds) - default time  30 minutes


// .............................................................................
// buttons and encoders
// .............................................................................
enum {
    RIGHT_MFB_1CLICK = 1,
    RIGHT_MFB_2CLICK = 2 ,
    RIGHT_MFB_3CLICK = 3,
    RIGHT_MFB_LONGPRESS = 4,
    RIGHT_ENC_1CLICK = 5,
    RIGHT_ENC_2CLICK = 6,
    RIGHT_ENC_3CLICK = 7,
    RIGHT_ENC_LONGPRESS = 8,
    RIGHT_ENCODER = 9,
    RIGHT_ENCODER_STOP = 10,
    LEFT_MFB_1CLICK = 11,
    LEFT_MFB_2CLICK = 12,
    LEFT_MFB_3CLICK = 13,
    LEFT_MFB_LONGPRESS = 14,
    LEFT_ENC_1CLICK = 15,
    LEFT_ENC_2CLICK = 16,
    LEFT_ENC_3CLICK = 17,
    LEFT_ENC_LONGPRESS = 18,
    LEFT_ENCODER = 19,
    LEFT_ENCODER_STOP = 20,
    LEFT_RIGHT_ENC_1CLICK = 21,
};
// .............................................................................
// Microphone
// .............................................................................
#define MIC_MUTE_TOGGLE 0x01


// .............................................................................
// The queues
// .............................................................................
extern QueueHandle_t Q_pwr;
//extern QueueHandle_t Q_pwr_sns;
extern QueueHandle_t Q_enc;
extern QueueHandle_t Q_enc_mcu;
extern QueueHandle_t Q_led;
extern QueueHandle_t Q_bm_but;
extern QueueHandle_t Q_bm_comm;
extern QueueHandle_t Q_mcu_init;
extern QueueHandle_t Q_mcu_led;
extern QueueHandle_t Q_comm_simp;
extern QueueHandle_t Q_comm_jsn;
extern QueueHandle_t Q_mic;

// .............................................................................
// Leds
// .............................................................................
extern void led_red_pwm(uint8_t pwm);
extern void led_green_pwm(uint8_t pwm);
extern void led_mic_pwm(uint8_t pwm);
extern void led_atmo_pwm(uint8_t pwm);

// .............................................................................
// Button Manager
// .............................................................................

// BTC States
#define APP_BT_OFF                          0       // BTC Off
#define APP_DISCONNECTED                    1       // BTC Disconnected
#define APP_CONNECTABLE                     2       // BTC is pairing mode, phone can search and connect to it
#define APP_CONNECTED                       3       // BTC Connected (no call or music)
#define APP_HFP_INCOMING                    4       // Incoming call (before answer)
#define APP_HFP_OUTGOING                    5       // - don't care: Making an outgoing call through connected BTC enabled phone (before answer)
#define APP_HFP_CALL_ACTIVE                 6       // Call In Progress
//#define APP_HFP_CALL_ACTIVE_WITHOUT_SCO     7       // - don't care
//#define APP_HFP_TWC_INCOMING                8       // - don't care
//#define APP_HFP_TWC_OUTGOING                9       // - don't care
//#define APP_HFP_MULTIPARTY_CALL             10      // - don't care
//#define APP_WIRED_MUSIC_PLAY                16      // - don't care
#define APP_A2DP_PLAYING                    24      // Music Playing
//#define APP_STATE_HELD_ACTIVE               25      // - don't care
//#define APP_STATE_FIND_ME                   26      // - don't care
//#define APP_STATE_VA                        27      // - don't care
//#define APP_ULTRA_LOW_LATENCY_PLAYING       28      // - don't care
//#define APP_LE_AUDIO_BIS_PLAYING            29      // - don't care

// .............................................................................
// BTC Button Actions
// .............................................................................
enum {
    BTC_BUT_ANS_END_PLAY_PAUSE = 1,
    BTC_BUT_REJ,
    BTC_BUT_FORWARD,
    BTC_BUT_BACKWARD,
    BTC_BUT_CONNECT,
    BTC_BUT_ANC_CYCLE,

};
// .............................................................................

// .............................................................................
// Charger
// .............................................................................
extern uint8_t battery_charging;

// .............................................................................
// JSON and Simple Comms
// .............................................................................

int comms_pkt_verify(uint8_t *rx_pkt, uint8_t *pkt_type, uint8_t *src_id, uint8_t *dest_id, u_int8_t *send_num, u_int8_t *ack_num);
int comms_pkt_build(uint8_t *data_msg, uint16_t num_bytes, uint8_t *tx_pkt, uint16_t *tx_pkt_len, u_int8_t pkt_type, u_int8_t src_id, u_int8_t dest_id, u_int8_t send_num, u_int8_t ack_num);

// .............................................................................
// Airoha Comms
// .............................................................................

#define AIROHA_MAX_PKT_SIZE     4096        // Allow enough size for complete JSON message


//#define COMMS_BYTE_0_START_FLAG     0
#define COMMS_BYTE_0_PAYLOAD_HI     0
#define COMMS_BYTE_1_PAYLOAD_LO     1
#define COMMS_BYTE_2_SRC_ID         2
#define COMMS_BYTE_3_DST_ID         3
#define COMMS_BYTE_4_SEND_NUM       4
#define COMMS_BYTE_5_ACK_NUM        5
#define COMMS_BYTE_6_PKT_TYPE       6
#define COMMS_BYTE_7_DATA           7

#define COMMS_HEADER_SIZE           COMMS_BYTE_7_DATA

#define COMMS_BYTE_7_SIMPLE_MSG_ID  7   // For SIMPLE protocol
#define COMMS_BYTE_8_SIMPLE_MSG_VAL 8

extern uint8_t airoha_rx_msg[AIROHA_MAX_PKT_SIZE]; // instead of a queue?
extern uint8_t airoha_tx_msg[AIROHA_MAX_PKT_SIZE]; // instead of a queue?

// .............................................................................
// Simple Protocol Comms
// .............................................................................

typedef struct {
    uint8_t id;
    int i_val;
} Q_comm_simp_t;

enum {
    COMMS_NONE = 0,
    // From left MCU
    COMMS_BAT_STATUS_HI,
    COMMS_BAT_STATUS_LO,
    COMMS_MIC_MUTED,
    COMMS_MIC_UNMUTED,
    COMMS_CHAT_MIX,
    COMMS_AUDIO_VOLUME,     // <-- right
    COMMS_HEADSET_STATE,
    COMMS_24GHZ_CONNECT,
    // From right MCU
    COMMS_CALL_ANSWER_REQ,
    COMMS_CALL_REJECT_REQ,
    COMMS_CALL_END_REQ,
    COMMS_TRACK_PLAY_PAUSE_REQ,
    COMMS_TRACK_FORWARD_REQ,
    COMMS_TRACK_BACKWARD_REQ,
    COMMS_BTC_DISCONNECT_REQ,
    COMMS_BTC_CONNECT_REQ,
    // From either MCU
    COMMS_AUDIO_CUE,
    // For right MCU
    COMMS_BTC_STATE_UPDATE,
    // For sending JSON
    COMMS_JSON_SUCESS,          // 16
    COMMS_JSON_READ_RESP,
    COMMS_JSON_INFO,
    COMMS_JSON_ERROR,
    COMMS_JSON_IMAGE

};

// Packet type, sent between MCU and Airoha/Core
#define COMMS_PKT_TYPE_JSON     1
#define COMMS_PKT_TYPE_IMG      2
#define COMMS_PKT_TYPE_DFU      3
#define COMMS_PKT_TYPE_SIMPLE   4

#define COMMS_ID_CORE       1
#define COMMS_ID_AIROHA     2
#define COMMS_ID_MCU        3

#define COMMS_ID_AB1577     2
#define COMMS_ID_AB1571     4

#define COMMS_PKT_NUM_NONE  255

extern int simple_rx_process(uint8_t *rx_pkt, uint8_t src_id, uint8_t dest_id, uint8_t send_num, uint8_t ack_num);
extern int simple_tx_msg(uint8_t *tx_buf, uint16_t *tx_buf_len, uint8_t msg_id, int msg_val);
extern int json_rx_process(uint8_t *rx_pkt, uint8_t src_id, uint8_t dest_id, uint8_t send_num, uint8_t ack_num);

extern bool is_dfu_file_ok();
extern void dfu_bootloader_trigger();
extern int img_rx_process(uint8_t *rx_pkt, uint8_t src_id, uint8_t dest_id, uint8_t send_num, uint8_t ack_num);

           
// .............................................................................
// Memory / file system
// .............................................................................

extern void app_mem_read_dir();
extern bool app_mem_file_exists(const char *filePath);
extern int app_mem_file_write(char *fileName, const uint8_t *data_to_w, size_t data_len);
extern int app_mem_file_app(char *fileName, const uint8_t *data_to_w, size_t data_len);
extern int app_mem_file_read(const char *fileName, uint8_t *data_to_r, size_t data_size);
extern int app_mem_file_del(const char *fileName);
extern int app_mem_file_empty(char *fileName);
extern int app_mem_file_size(char *fileName);
extern int app_mem_file_rename(char *fileNameOld, char *fileNameNew);

// .............................................................................
// Display images
// .............................................................................
extern const unsigned char gImage_Orientation[];
extern const unsigned char gImage_Card1[];
extern const unsigned char gImage_Card2[];
extern const unsigned char gImage_Card3[];
extern const unsigned char gImage_Card4[];
extern const char pixel_perfect_1[];
extern const char pixel_perfect_2[];
extern const char pixel_perfect_3[];
extern const char pixel_perfect_4[];
extern const char pixel_perfect_5[];
extern const char pixel_perfect_6[];

// .............................................................................
// Utils
// .............................................................................
extern int utils_str_to_uint32(char *msg, uint32_t *u32_val);
extern int utils_str_to_int32(char *msg, int32_t *i32_val);
extern int utils_str_to_uint16(char *msg, uint16_t *u16_val);
extern int utils_str_to_int8(char *msg, int8_t *i8_val);
extern int utils_str_to_uint8(char *msg, uint8_t *u8_val);
extern int utils_str_to_bool(char *msg, bool *b_val);
extern int utils_on_off_str_to_bool(char *msg, bool *b_val);
extern int utils_uint32_to_str(char *msg, uint32_t u32_val);
extern int utils_uint16_to_str(char *msg, uint16_t u16_val);
extern int utils_int8_to_str(char *msg, int8_t i8_val);
extern int utils_uint8_to_str(char *msg, uint8_t u8_val);
extern int utils_bool_to_str(char *msg, bool b_val);
extern int utils_bool_to_on_off_str(char *msg, bool b_val);

// .............................................................................

/* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _APP_H */
