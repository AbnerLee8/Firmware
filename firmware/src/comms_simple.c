/******************************************************************************
  @File Name
    comms_simple.c

  @Summary
    Functions associated with SIMPLE Message protocol.

  @Description

    Process SIMPLE Format message received from Airoha
    - Verify format of received message.
    - Pass the received message on to the appropriate MCU task. 
    Build SIMPLE Format messages to be sent back to Airoha.
 
  |PayloadLen|Src|Dest|SendNum|AckNum|PktType|PAYLOAD|Crc|
    
*******************************************************************************/
#include "comms_simple.h"

// .............................................................................
// Defines
// .............................................................................

// Incoming messages
#define SIMPLE_MSG_BTC_STATE            3
// Outgoing messages
#define SIMPLE_MSG_MIC_MUTED            1
#define SIMPLE_MSG_MIC_UNMUTED          2
#define SIMPLE_MSG_CALL_ANSWER          5
#define SIMPLE_MSG_CALL_REJECT          6
#define SIMPLE_MSG_CALL_END             7
#define SIMPLE_MSG_AUDIO_VOLUME         8
#define SIMPLE_MSG_TRACK_PLAY_PAUSE     9
#define SIMPLE_MSG_TRACK_FORWARD        10
#define SIMPLE_MSG_TRACK_BACKWARD       11
#define SIMPLE_MSG_AUDIO_CUE_PLAY       12
#define SIMPLE_MSG_BTC_CONNECT_REQ      13

#define SIMPLE_MSG_HEADSET_STATE        15
#define SIMPLE_MSG_CHAT_MIX             16

#define SIMPLE_MSG_24GHZ_CONNECT        18
    
    // Destination for received message
#define DEST_NONE               0
#define DEST_BUT_MNGR           1
#define DEST_RIGHT_MCU           1
   

static uint8_t simple_send_seq_num = 0; // Used for Send Seq Num, when sending asynch message to Core/Airoha

// .............................................................................
// Pass the received message on to the appropriate MCU task.
// .............................................................................
void simple_rx_pass_to_task(uint8_t msg_id, uint8_t msg_val)
{
    Q_comm_simp_t qs_comm_msg;
    uint8_t dest;
    
    dest= DEST_NONE;
    
    switch (msg_id)
    {
        case SIMPLE_MSG_BTC_STATE:
            // Update in Bluetooth State
            qs_comm_msg.id = COMMS_BTC_STATE_UPDATE;
            qs_comm_msg.i_val = (int)msg_val;
            dest = DEST_BUT_MNGR;
            break;
        default:
            // Ignore
            break;
    }    
    
    if (dest == DEST_BUT_MNGR)
    {
        // Send the message on the the Button Manager task
        osQueueSendToBack(Q_bm_comm, &qs_comm_msg, 10);
    }
}

// .............................................................................
// This function processes packet received from Airoha UART.
// Packet is in SIMPLE Protocol Format
//
//  |PayloadLen|Src|Dest|SendNum|AckNum|PktType|PAYLOAD|Crc|
//
//  PAYLOAD: Msg ID | Msg Val
//           1 byte | 1 byte
//
// Steps:
//  1: Verify the source/destination, and sequence numbers etc.
//  2. Extract the received Message ID and Value
//  3. Call the function associated with the received Message ID
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
int simple_rx_process(uint8_t *rx_pkt, uint8_t src_id, uint8_t dest_id, uint8_t send_num, uint8_t ack_num)
{
    int err = 0;
    uint8_t msg_id;
    uint8_t msg_val;

    if (rx_pkt == NULL)
    {
        // Nothing to do
        return -1;
    }
    
    // Verify source: only interested in packets from Airoha
    if ((src_id != COMMS_ID_AB1577) && (src_id != COMMS_ID_AB1571))
    {
        // Packet is not from valid source
        return -2;
    }
    // Verify destination: only interested in packets for this MCU
    if (dest_id != COMMS_ID_MCU)
    {
        // Packet is not for MCU
        return -3;
    }

    // Extract the received msg id & val
    msg_id = rx_pkt[COMMS_BYTE_7_SIMPLE_MSG_ID];
    msg_val = rx_pkt[COMMS_BYTE_8_SIMPLE_MSG_VAL];
    // Pass the received message on to the appropriate MCU task
    simple_rx_pass_to_task(msg_id, msg_val);

    // Success
    return 0;
}


// .............................................................................
// Send message to Airoha
// Build packet using SIMPLE Protocol Format
//
//  |PayloadLen|Src|Dest|SendNum|AckNum|PktType|PAYLOAD|Crc|
//
// Use the provided mshg_id and msg_val to determine which Airoha chip to send
// the message to.
//
// Inputs:
//  tx_buf:     Buffer to write to
//  tx_buf_len; Resulting number of bytes written to tx buffer
//  msg_id      Internal Message id - will be converted to the
//              protocol's Simple Msg ID
//              Simpe message's value (all SImple messages comtain Msg ID 
//              and Msg Val))
// Returns:
//      err:    0:      success
//               -ve:   error
// .............................................................................
int simple_tx_msg(uint8_t *tx_buf, uint16_t *tx_buf_len, uint8_t msg_id, int msg_val)
{
    bool b_continue = true;
    int res = 0;
    uint8_t raw_msg[2];
    uint8_t dest_id;
    
    switch (msg_id) {
        // AB1577
        case  COMMS_MIC_MUTED:
            raw_msg[0] = SIMPLE_MSG_MIC_MUTED;
            raw_msg[1] = 0;
            dest_id = COMMS_ID_AB1577;
            break;
        case COMMS_MIC_UNMUTED:
            raw_msg[0] = SIMPLE_MSG_MIC_UNMUTED;
            raw_msg[1] = 0;
            dest_id = COMMS_ID_AB1577;
            break;
        case COMMS_AUDIO_VOLUME:
            raw_msg[0] = SIMPLE_MSG_AUDIO_VOLUME;
            raw_msg[1] = (uint8_t)msg_val;
            dest_id = COMMS_ID_AB1577;
            break;
        case COMMS_CHAT_MIX:
            raw_msg[0] = SIMPLE_MSG_CHAT_MIX;
            raw_msg[1] = (uint8_t)msg_val;
            dest_id = COMMS_ID_AB1577;
            break;
        case COMMS_HEADSET_STATE:
            raw_msg[0] = SIMPLE_MSG_HEADSET_STATE;
            raw_msg[1] = msg_val;
            dest_id = COMMS_ID_AB1577;
            break;
        // AB1571
        case COMMS_CALL_ANSWER_REQ:
            raw_msg[0] = SIMPLE_MSG_CALL_ANSWER;
            raw_msg[1] = 0;
            dest_id = COMMS_ID_AB1571;
            break;
        case COMMS_CALL_REJECT_REQ:
            raw_msg[0] = SIMPLE_MSG_CALL_REJECT;
            raw_msg[1] = 0;
            dest_id = COMMS_ID_AB1571;
            break;
        case COMMS_CALL_END_REQ:
            raw_msg[0] = SIMPLE_MSG_CALL_END;
            raw_msg[1] = 0;
            dest_id = COMMS_ID_AB1571;
            break;
        case COMMS_TRACK_PLAY_PAUSE_REQ:
            raw_msg[0] = SIMPLE_MSG_TRACK_PLAY_PAUSE;
            raw_msg[1] = 0;
            dest_id = COMMS_ID_AB1571;
            break;
        case COMMS_TRACK_FORWARD_REQ:
            raw_msg[0] = SIMPLE_MSG_TRACK_FORWARD;
            raw_msg[1] = 0;
            dest_id = COMMS_ID_AB1571;
            break;
        case COMMS_TRACK_BACKWARD_REQ:
            raw_msg[0] = SIMPLE_MSG_TRACK_BACKWARD;
            raw_msg[1] = 0;
            dest_id = COMMS_ID_AB1571;
            break;
        case COMMS_BTC_CONNECT_REQ:
            raw_msg[0] = SIMPLE_MSG_BTC_CONNECT_REQ;
            raw_msg[1] = 0;
            dest_id = COMMS_ID_AB1571;
            break;
        case COMMS_AUDIO_CUE:
            raw_msg[0] = SIMPLE_MSG_AUDIO_CUE_PLAY;
            raw_msg[1] = 0;
            dest_id = COMMS_ID_AB1571;
            break;
        case COMMS_24GHZ_CONNECT:
            raw_msg[0] = SIMPLE_MSG_24GHZ_CONNECT;
            raw_msg[1] = 0;
            dest_id = COMMS_ID_AB1577;
            break;
        default:
            b_continue = false;
            break;
    }  
    if (!b_continue) {
        // Undefined message
        return -1;
    }
    
    // Build the message to send to AIroha (including adding sequence numbers, cc etc.)
    res = comms_pkt_build(raw_msg, 2, tx_buf, tx_buf_len, COMMS_PKT_TYPE_SIMPLE, COMMS_ID_MCU, dest_id, simple_send_seq_num, COMMS_PKT_NUM_NONE);
    
    // Increment send sequence number after each send
    simple_send_seq_num++;
    if (simple_send_seq_num > 250)
    {
        simple_send_seq_num = 0;
    }
    
    return (res);
    
}
// .............................................................................

