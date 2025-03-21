/*******************************************************************************
  File Name:
    app_com.c

  Summary:
    Commmunications interface to/with Airoha chip.

  Description:
 *******************************************************************************/
#include "app_com.h"
#include "json/json.h"

#define AIR_RX_SIZE    2062         // works, when script is sending messages of 2048+14=1038
//#define AIR_RX_SIZE    1038       // works, when script is sending messages of 1024+14=1038
//#define AIR_RX_SIZE    1024       // works, when script is sending messages of 512+14=526
//#define AIR_RX_SIZE    256        // too small
uint8_t air_rxbuf[AIR_RX_SIZE];
uint8_t air_byte[1];
BFIFO air_rxfifo;

// Airoha messages
#define AIR_START   0x7F
#define AIR_STOP    0x7E
uint8_t comm_rx_buffer[AIROHA_MAX_PKT_SIZE];     // When full message received, copy from air_rx_buffer[] into this buffer, to free up air_rx_buffer
uint8_t air_rx_buffer[AIROHA_MAX_PKT_SIZE];     // Receiving from UART into this buffer
uint8_t air_tx_buffer[AIROHA_MAX_PKT_SIZE];     // When receive message tx tx, remove from Queue, and copy to this buffer (along with header, crc etc.))

// Comms Tx Timing
uint16_t comms_tx_delay_cntr = 0;
bool b_comms_tx_delaying = false;
#define TX_DELAY_CNT_MAX    2              // Delay time (units of 10 ms))

// *****************************************************************************
// Functions used to delay the time between consecutive message transmissions 
// to the Airoha chip, so that it is not overloaded.
// *****************************************************************************

// .............................................................................
// Initialise/zero the variables used.
// .............................................................................
void comms_tx_delay_init()
{
    comms_tx_delay_cntr = 0;
    b_comms_tx_delaying = false;
}
// .............................................................................
// See if delay is still active. Can we send the next message yet?
// .............................................................................
bool comms_tx_okay_to_tx()
{
    return(!b_comms_tx_delaying);
}
// .............................................................................
// Have just sent a message to the AIroha chip, so start the inter-0message 
// delay time.
// .............................................................................
void comms_tx_delay_start()
{
    comms_tx_delay_cntr = TX_DELAY_CNT_MAX;
    b_comms_tx_delaying = true;
}
// .............................................................................
// Manage the time between successive tx messages to the AIroha chip.
// THias function is called from the Encoder's TimerCallback function 
// (i.e. every 10 ms))
// .............................................................................
void comms_tx_delay_manage()
{
    if (b_comms_tx_delaying) {
        if (comms_tx_delay_cntr > 0) {
            comms_tx_delay_cntr--;
        }
        if (comms_tx_delay_cntr == 0) {
            b_comms_tx_delaying = false;
        }
    } else {
        comms_tx_delay_cntr = 0;
    }
}
// *****************************************************************************



// *****************************************************************************
// Functions for managing serial port between MCU and Airoha
// *****************************************************************************

// .............................................................................
// Get next char from the receive buffer (i.e. message from Airoha).
// Return -1 if nothing (FIFO is empty).
// IMPORTANT: This function is non-blocking.
// .............................................................................
int air_getchar()
{
	return bfifo_get(&air_rxfifo);
}
// .............................................................................
// Callback function, called automatically whenever a character received on
// serial port (from AIroha))
// .............................................................................
void air_read_callback( uintptr_t context )
{
    bfifo_put(&air_rxfifo, air_byte[0]);
    SERCOM1_USART_Read(air_byte, 1);
}

// .............................................................................
// Write a number of bytes to the port (to send to Airoha)
// .............................................................................
void air_write(uint8_t *buffer, size_t count)
{
    SERCOM1_USART_Write(buffer, count);
	printf("air ");
	for(int i=0;i<count;i++)
		printf(" 0x%02x ",buffer[i]);
	printf(" \n");
    while (SERCOM1_USART_WriteIsBusy()) {
        osDelayMs(1);
    }
}
// .............................................................................
// Initlaise function for the serial port..
// .............................................................................
void air_init()
{
    bfifo_init(&air_rxfifo, air_rxbuf, AIR_RX_SIZE);
    SERCOM1_USART_ReadCallbackRegister(air_read_callback, 0);
    SERCOM1_USART_Read(air_byte, 1);
}
// *****************************************************************************

// .............................................................................
// This function processes a packet received from Airoha UART.
// The source of this message could be from Core app, or from one of the
// Airoha chips (AB1577 / AB1571).
//
// Calls the verify function to verofy formnat, sequence numbers, CRTC etc.
// If everythiNG is valid, then call the function associated with the received 
// Message ID -0 i.e. see if message is SImple Messagwe (Airoha-MCU), or JSON
// message (Core app - MCU), or IMage file (Core app - MCU).)
//
// Inputs:
//  rx_pkt:     Received packet (from Core/Airoha)
// Returns:
//      err:    0:      success
//               -ve:   error
// .............................................................................
int comms_rx_process(uint8_t *rx_pkt)
{
    int err = 0;
    uint8_t pkt_type;
    uint8_t src_id;
    uint8_t dest_id;
    uint8_t send_num;
    uint8_t ack_num;
    
    if (rx_pkt == NULL)
    {
        // Nothing to do
        return -1;
    }
    
    // Verify the received message
    //  pkt_type    Received Packet Type: JSON, SIMPLE, etc
    //  src_id      Received Source ID
    //  dest_id     Received Destination ID
    //  send_num    Received Send Number
    //  ack_num     Received ACK Number    
    err = comms_pkt_verify(rx_pkt, &pkt_type, &src_id, &dest_id, &send_num, &ack_num);

    if (err < 0)
    {
        // Verification failed
        json_fn_send_status_error(ERR_PKT_VERIFY);
        return -2;
    }
 
    if (pkt_type == COMMS_PKT_TYPE_SIMPLE) {
        // Simple Protocol
        err = simple_rx_process(rx_pkt, src_id, dest_id, send_num, ack_num);
        if (err < 0) {
            // error
        }
    }
    else if (pkt_type == COMMS_PKT_TYPE_JSON) {
        // JSON Protocol
        err = json_rx_process(rx_pkt, src_id, dest_id, send_num, ack_num);
        if (err < 0) {
            // error
        }
    }
    else if (pkt_type == COMMS_PKT_TYPE_IMG) {
        err = img_rx_process(rx_pkt, src_id, dest_id, send_num, ack_num);
        return -4;
    }
    else {
        json_fn_send_status_error(ERR_PKT_UNKNOWN);
        return -3;
    }
    
    // Success
    return 0;
}

// .............................................................................
 // Task initializations.
 // This function runs BEFORE task is created and scheduler started!.
// .............................................................................
void APP_COM_Initialize ( void )
{
    json_data_init();
}


// .............................................................................
// Airoha-MCU Communications task
//
// Sets up serial port from communicating with Airoha chips.
// The main loop / state machine, processes messages received from ether Airoha 
// chip, and also  receives message svia twi queues, to be sent to the Airoa chips.
// Receiving:
// Assembles packet from chars received  from Airoha.
// When what it thinks might be a full packet is received, calls 
// comms_rx_process() to process/verify the packet.
// Sending:
// Simple Format messages, destined for either Airoha chip, can be received in 
// the Q_comm_simp queue, received from the Power task (e.g. Headset State 
// change), the Microphone task (e.g. Microphone muted), or the Button Manager 
// task (e.g. Call Answer request).
// JSON format messages, destined for the Core app, can be received in the 
// Q_comm_jsn queue, from the JSON functions (e.g. JSON read response).
// .............................................................................
void APP_COM_Tasks ( void )
{
    Q_comm_simp_t qs_comm_simp_msg;
    Q_comm_jsn_t qs_comm_jsn_msg;
    int c;
    uint16_t rx_indx;
    uint16_t rx_len_target;
    uint16_t tx_len;
    int res = 0;
    uint8_t id;
    int val;
    uint8_t err_no;
    uint16_t seg_no;
    uint16_t seg_tot;
    uint8_t rx_inactivity_cntr;;
    bool b_rx_chars_this_time;
    bool b_err;
    bool b_break;
    bool b_rx_process_now;
    bool rx_reset = false;
    uint16_t pay_len;
    uint8_t src_id;
    uint8_t dest_id;
    uint8_t send_num;

    // Task waits Power Task has powered everything up
    while (!GlobalPowerOn) {
        osDelayMs(100);
    }

    // Task waits until the necessary Queues have been enabled
    while (Q_comm_simp == NULL) osDelayMs(10);
    while (Q_comm_jsn == NULL) osDelayMs(10);

    // Initialise serial port between AIroa and MCU
    air_init();
    
    // Only reset state and index at very start, and also any time a FLAG received
    rx_indx = 0; 
    rx_inactivity_cntr = 0;
    
    // Initialise the variables associated with delaying Comms Tx to Airoha
    comms_tx_delay_init();
    
    while(1) {
        OS_SLEEP(1); //osDelayMs(1);
        
        // receiving a bunch of characters
        
        // ***********
        // RECEIVE
        // ***********
        
        if (rx_reset) {
            // Have processed a message, so reset the receive variables
            rx_reset = false;
            rx_indx = 0;
            rx_len_target = 0;
            rx_inactivity_cntr = 0;
        }
        
        // Flags to init for each cycle:
        b_rx_chars_this_time = false;
        b_rx_process_now = false;
        b_err = false;
        b_break = false;
        
        // Receive as many chars as possible
        while (1) { 
            // See if any (more) chars in UART buffer:)
            c = air_getchar();
            if (c == -1) {
                // Nothing received, will increment inactivity counter
                break;
            }
            // Have got a char - remember this!
            b_rx_chars_this_time = true;
            // Reset inactivity counter
            rx_inactivity_cntr = 0;
            // Add it to receive buffer
            air_rx_buffer[rx_indx++] = c;
            
            // Some sanity checks......
            
            // Payload Len
            if (rx_indx == 2) {
                // Take a copy of the payload legth
                pay_len = (air_rx_buffer[COMMS_BYTE_0_PAYLOAD_HI] * 256) + air_rx_buffer[COMMS_BYTE_1_PAYLOAD_LO];
                if (pay_len > AIROHA_MAX_PKT_SIZE) {
                    // Invalid length - want to reset state machine without breaking out of loop and pausing
                    b_err = true;
                } else {
                    // The expected full rx len = payload + [ payLen(2) + src/dest(2) + send/ack (2) + pktTtype (1) + crc (2) = 9 ]
                    rx_len_target = pay_len + 9;
                }
            }
            // Source/Destination
            if (rx_indx == 4) {
                src_id = air_rx_buffer[COMMS_BYTE_2_SRC_ID];
                dest_id = air_rx_buffer[COMMS_BYTE_3_DST_ID];
                if (dest_id != COMMS_ID_MCU) {
                    // Invalid destination - want to reset state machine without breaking out of loop and pausing
                    b_err = true;
                } else if ((src_id != COMMS_ID_CORE) && (src_id != COMMS_ID_AB1577) & (src_id != COMMS_ID_AB1571)) {
                    // Invalid source - want to reset state machine without breaking out of loop and pausing
                    b_err = true;
                }
            }
            // Rx Target
            if ((rx_len_target > 0) && (rx_indx >= rx_len_target)) {
                // Go ahead and process the received data
                b_rx_process_now = true;
            }
            // Max Len: Check for buffer overflow
            if (rx_indx >= AIROHA_MAX_PKT_SIZE) {
                // Invalid length - want to reset state machine AND break out of loop (to allow other tasks some processing)
                b_err = true;
                b_break = true;
            }
            
            if (b_err) {
                // Abandon the buffer and start again (without breaking out of loop))
                b_err= false;
                rx_indx = 0;
                rx_len_target = 0;
                rx_inactivity_cntr = 0;
            }
            
            if (b_rx_process_now) {
                // Have reached target length, so go ahead and process
                break;
            }
            
            
        }
        
        // Get here if:
        // (a) no char received (timeout), or
        // (b) have received expected number of bytes (target), or
        // (c) we broke out of loop just because we had received too many chars
        
        if (b_break) {
            // (c) we broke out of loop just because we had received too many chars
            rx_reset = true;
            // Set other flags to prevent any further rx processing being done on this cycle
            rx_indx = 0;
            b_rx_process_now= false;
        }
        
        // Inactivity: Check for timeout if nothing received in last cycle
        if (!b_rx_chars_this_time) {
            // (a) no char received (timeout):
            // Have we already received some data?
            if (rx_indx > 0) {
                // Yes we have - so increment cycle counter
                rx_inactivity_cntr++;
                if (rx_inactivity_cntr > 20) {      // 20 x 2ms+ is at least 40 ms
                    // We have been receiving chars, but we have waited/cycled too may times, so time to give up
                    // and just process what we have
                    b_rx_process_now = true;
                }
            }
        }
        
        // See if have received a message to process now...
        if (b_rx_process_now) {
            //(b) have received expected number of bytes (target):
            // Copy into different buffer first
            memcpy(comm_rx_buffer, air_rx_buffer, rx_indx);
            res = comms_rx_process(comm_rx_buffer);
            rx_reset = true;
        }
        
        // (c) we broke out of loop just because we had received too many chars
        // Nothing to do
        
        // ***********
        // TRANSMIT
        // ***********

        
        if (comms_tx_okay_to_tx()) {
            // Not delaying the interval between consecutive tx's to Airoha

            OS_SLEEP(1);		// Do we need this before attempting to transmit?

            // Check for Simple COmms messages from Button Manager / BTC Manager
            if (osQueueReceive(Q_comm_simp, &qs_comm_simp_msg, 10) == pdPASS) {
                id = qs_comm_simp_msg.id;
                val = qs_comm_simp_msg.i_val;
                // Assume failure
                res = simple_tx_msg(air_tx_buffer, &tx_len, id, val);              
                // Send message to Airoha
                if (res >= 0) {
                    // Delay the interval between consecutive tx's to Airoha
                    comms_tx_delay_start();
                    air_write(air_tx_buffer, tx_len);
                }
            }
        }

        if (comms_tx_okay_to_tx()) {
            // Not delaying the interval between consecutive tx's to Airoha

            OS_SLEEP(1);		// Do we need this before attempting to transmit?

            // Check for JSON Comms messages from this (comms) task
            if (osQueueReceive(Q_comm_jsn, &qs_comm_jsn_msg, 10) == pdPASS) {
                id = qs_comm_jsn_msg.id;
                send_num =  qs_comm_jsn_msg.send_num;
                err_no = qs_comm_jsn_msg.err_no;
                seg_no = qs_comm_jsn_msg.seg_no;
                seg_tot = qs_comm_jsn_msg.seg_tot;

                // Assume failure
                res = -1;
                res = json_build_tx_str(air_tx_buffer, &tx_len, id, send_num, err_no, seg_no, seg_tot);
                if (res >= 0) {
                    // Delay the interval between consecutive tx's to Airoha
                    comms_tx_delay_start();
                    air_write(air_tx_buffer, tx_len);
                }
            }
        }
        

    }
}

// .............................................................................
