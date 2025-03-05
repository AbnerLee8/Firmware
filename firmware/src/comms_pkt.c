/******************************************************************************
  @File Name
    comms_pkt.c

  @Summary
    Verify packets received from Airoha/Core, and build packets to send 
    to Airoha/Core

  @Description

  Functions used when receiving messages from, ands sending messages to, the 
  Airoha chips.
  - Verify format and contents of received messages.
  - Build a packet, in SIMPEL or JSON format, that can be sent to 
    Airoha / Core app.

  |PayloadLen|Src|Dest|SendNum|AckNum|PktType|PAYLOAD|Crc|
    
*******************************************************************************/
#include "comms_pkt.h"


#define PKT_MAX_LEN         4096
#define PKT_HEADER_LEN      9         // Len(2) + Src/Dst(2) + Seq(2) + Type(1) + CRC(2) = 11
#define CRC16_POLY          0x8005

// .............................................................................
//
//      Packet format
//
//      -----------------------------
//        0     Payload Len - Hi Byte   COMMS_BYTE_0_PAYLOAD_HI
//        1     Payload Len - Lo Byte   COMMS_BYTE_1_PAYLOAD_LO
//        2     Source ID               COMMS_BYTE_2_SRC_ID
//        3     Destination ID          COMMS_BYTE_3_DST_ID
//        4     Send Seq. Num           COMMS_BYTE_4_SEND_NUM
//        5     Ack Seq. Num            COMMS_BYTE_5_ACK_NUM
//        6     Packet Type             COMMS_BYTE_6_PKT_TYPE
//        7     Data                    COMMS_BYTE_7_DATA
//      len-2   CRC - Hi Byte
//      len-1   CRC - Lo Byte
//      -----------------------------
//
// .............................................................................

// .............................................................................
// CRC 16 for packets
// CRC is calculated  over all bytes except the CRC itself
//
// |PayloadLen|Src|Dest|SendNum|AckNum|PktType|PAYLOAD|Crc|
//
// |<.................CRC Calculation................>|
//
// .............................................................................
// Define the CRC calculation function


uint16_t comms_pkt_crc16(uint8_t *data, size_t length)
{
    uint16_t crc = 0;
    int i;
    
    while (length--) {
        crc ^= *data++ << 8;
        for (i = 0; i < 8; i++) {
            if (crc & 0x8000) {
                crc = (crc << 1) ^ CRC16_POLY;
            } else {
                crc = crc << 1;
            }
        }
    }

    return crc;
}

// .............................................................................
// Packet received via Airoha UART
// - Verify integrity of the packet 
//   Verify src/dst ids, packet len, crc
// - Verify sequence numbers
// Inputs:
//  rx_pkt      Received packet, with transport layer (flags, ids, crc etc.)
// Outputs:
//  pkt_type    Received Packet Type: JSON, DFU, SIMPLE, etc
//  src_id      Received Source ID
//  dest_id     Received Destination ID
//  send_num    Received Send Number
//  ack_num     Received ACK Number
// Returns      0:      success
//              else:   error
// .............................................................................
int comms_pkt_verify(uint8_t *rx_pkt, uint8_t *pkt_type, uint8_t *src_id, uint8_t *dest_id, u_int8_t *send_num, u_int8_t *ack_num)
{
    int pay_len;
    int pkt_len;
    uint16_t calc_crc_val;
    uint16_t rx_crc_val;
    uint8_t *crc_start;

    if (rx_pkt == NULL)
    {
        return -1;
    }

    // Verify payload length
    pay_len = (rx_pkt[COMMS_BYTE_0_PAYLOAD_HI] * 256) + rx_pkt[COMMS_BYTE_1_PAYLOAD_LO];
    if (pay_len > PKT_MAX_LEN)
    {
        return -3;
    }

    // Update total packet length, based on received payload length and defined header bytes:
    pkt_len = pay_len + PKT_HEADER_LEN;

    // Take a copy of the received CRC value
    rx_crc_val = (rx_pkt[pkt_len-2] * 256) + rx_pkt[pkt_len-1];
    // Pointer to where our CRC calculation will begin
    crc_start = &rx_pkt[COMMS_BYTE_0_PAYLOAD_HI];
    // Pointer to where our CRC calculation ends (strip off the end)
    rx_pkt[pkt_len-2] = '\0';
    // Calculate crc
    // Length of CRC calculation is full packet less flags (2 bytes) and CRC value itself (2 bytes))
    calc_crc_val = comms_pkt_crc16(crc_start, pkt_len-2);

    *src_id = rx_pkt[COMMS_BYTE_2_SRC_ID];
    *dest_id = rx_pkt[COMMS_BYTE_3_DST_ID];
    *send_num = rx_pkt[COMMS_BYTE_4_SEND_NUM];
    *ack_num = rx_pkt[COMMS_BYTE_5_ACK_NUM];
    *pkt_type = rx_pkt[COMMS_BYTE_6_PKT_TYPE];

    return 0;
}

// .............................................................................
// Given a buffer containing the data/payload, add the transport layer details.
// i.e. Build a packet, in SIMPLE or JSON format, that can be sent to 
// Airoha / Core app/
// Inputs:
//  data_msg    Buffer containing the data/payload to be sent
//  tx_pkt      Buffer to build the resulting new packet in
//  pkt_type    Packet Type: JSON, DFU, SIMPLE, etc
//  src_id      Source ID to include in packet
//  dest_id     Destination ID to include in packet
//  send_num    Send Number to include in packet
//  ack_num     ACK Number to include in packet
// Output:
//  tx_pkt      Resulting packet will be available here
// Returns      0:      success
//              else:   error
// .............................................................................
int comms_pkt_build(uint8_t *data_msg, uint16_t num_bytes, uint8_t *tx_pkt, uint16_t *tx_pkt_len, u_int8_t pkt_type, u_int8_t src_id, u_int8_t dest_id, u_int8_t send_num, u_int8_t ack_num)
{
    uint16_t tx_len;
    u_int8_t pay_len_hi;
    u_int8_t pay_len_lo;
    u_int8_t crc_hi;
    u_int8_t crc_lo;
    uint16_t crc_val;
    uint8_t *crc_start;
    uint8_t *data_start;
    
    if (num_bytes < 1)
    {
        // Nothing to do
        return -1;
    }
    if (num_bytes > PKT_MAX_LEN)
    {
        // Too long
        return -2;
    }
    if (tx_pkt == NULL)
    {
        return -3;
    }
    if ((pkt_type < COMMS_PKT_TYPE_JSON) || (pkt_type > COMMS_PKT_TYPE_SIMPLE))
    {
        return -4;
    }
    
    // Where the CRC calculation will start:
    crc_start = &tx_pkt[COMMS_BYTE_0_PAYLOAD_HI];
    // Where the DATA will go (will be copied to):
    data_start = &tx_pkt[COMMS_BYTE_7_DATA];

    // Calculate payload length:
    pay_len_hi = (u_int8_t)( (num_bytes >> 8) & 0x00FF );
    pay_len_lo = (u_int8_t)(num_bytes & 0x00FF);

    // Start filling out the tx packet:

    // Payload
    tx_pkt[COMMS_BYTE_0_PAYLOAD_HI] = pay_len_hi;
    tx_pkt[COMMS_BYTE_1_PAYLOAD_LO] = pay_len_lo;
     // Source ID = this MCU
    tx_pkt[COMMS_BYTE_2_SRC_ID] = src_id;
    // Destination ID - received source id
    tx_pkt[COMMS_BYTE_3_DST_ID] = dest_id;
    // Send Seq Number - always NONE for a response from us
    tx_pkt[COMMS_BYTE_4_SEND_NUM] = send_num;
    // Ack Sequence Number = received send seq num
    tx_pkt[COMMS_BYTE_5_ACK_NUM] = ack_num;
    // Packet Type
    tx_pkt[COMMS_BYTE_6_PKT_TYPE] = pkt_type;
    // Copy payload data to the tx data
    strcpy((char *)data_start, (char *)data_msg);

    // Total length of packet to be constructed:
    // Len(2) + Src/Dst(2) + Seq(2) + Type(1) + CRC(2) = 11
    tx_len = (uint16_t)(num_bytes + PKT_HEADER_LEN);

    // Calculate CRC:

    // First, temporarily snip of end of packet
    tx_pkt[tx_len-2] = '\0';
    // Then calculate
    // Length of CRC calculation is full packet less flags (2 bytes) and CRC value itself (2 bytes))
    crc_val = comms_pkt_crc16(crc_start, tx_len-2);
    crc_hi = (u_int8_t)( (crc_val >> 8) & 0x00FF );
    crc_lo = (u_int8_t)(crc_val & 0x00FF);
    // Then fill in CRC
    tx_pkt[tx_len-2] = crc_hi;
    tx_pkt[tx_len-1] = crc_lo;

    *tx_pkt_len = tx_len;

    return 0;

}
// .............................................................................


