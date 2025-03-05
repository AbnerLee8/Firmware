/*******************************************************************************
  File Name
    app.c

  Summary
    Common application resources.

  Description
 *******************************************************************************/
#include "app.h"

/*----------------------------------------------------------------------------*/
/* Global milliseconds counter */
volatile uint32_t MS;

// .............................................................................
// Power
// .............................................................................
int GlobalPowerOn = 0;    
int GlobalSleep = 0;

// .............................................................................
// Buttons
// .............................................................................
button_config_t BUTconfig[4];

// .............................................................................
// Encoders
// .............................................................................
encoder_config_t ENCconfig[2];

// .............................................................................
// The queues
// .............................................................................
QueueHandle_t Q_pwr;
QueueHandle_t Q_pwr_sns;
QueueHandle_t Q_enc;
QueueHandle_t Q_enc_mcu;
QueueHandle_t Q_led;
QueueHandle_t Q_bm_but;
QueueHandle_t Q_bm_comm;
QueueHandle_t Q_mcu_init;
QueueHandle_t Q_mcu_led;    
QueueHandle_t Q_comm_simp;
QueueHandle_t Q_comm_jsn;
QueueHandle_t Q_mic;
uint8_t airoha_rx_msg[AIROHA_MAX_PKT_SIZE]; // instead of a queue?
uint8_t airoha_tx_msg[AIROHA_MAX_PKT_SIZE]; // instead of a queue?

// .............................................................................

