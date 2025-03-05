/******************************************************************************
  @File Name
    large_files.c

  @Summary
    Large File Transfer - associated functions

  @Description

    
*******************************************************************************/
#include "json/json.h"

// .............................................................................

#define IMG_MAX_SEGMENTS        1000        // Arbitrary max. number of segments, for testing purposes.
#define IMG_MIN_SIZE            4           // Arbitrary min. image size, for testing purposes.

#define DFU_MIN_FILE_SIZE       0x1000      // DFU - min file size allowed
#define DFU_MAX_FILE_SIZE       0x100000    // DFU - max file size allowed
#define DFU_MIN_SEG_SIZE        0x200       // DFU - min segment size allowed
#define DFU_MAX_SEG_SIZE        0x1000      // DFU - max segment size allowed
#define DFU_MIN_SEG_TOTAL       40          // DFU - min total number of segments
#define DFU_MAX_SEG_TOTAL       2000        // DFU - max total number of segments

#define BTL_TRIGGER_PATTERN     (0x5048434DUL)
#define BTL_TRIGGER_RAM_START   0x20000000
#define BTL_TRIGGER_LEN         16

static uint32_t *ramStart = (uint32_t *)BTL_TRIGGER_RAM_START;

uint32_t rx_seg_tot_expect = 0;         // DFU - Expected total number of segments to receive
bool rx_dfu_file_ok = false;            // DFU - only set to true when complete file received

// .............................................................................

uint16_t seg_id_saved = 0;          // Segment ID. Received in first Segment for current image. This should then be the same for all segments for this image.
uint16_t seg_tot_saved = 0;         // Total number of segments. This value is saved from 1st segment received, and should not change.
uint16_t seg_num_exp = 1;           // Segment Number expected next. Begins with 1. Should reach Segment Total for last segment.
uint16_t img_w_saved = 0;           // Image Width. Received in image header, in first Segment.
uint16_t img_h_saved = 0;           // Image Height. Received in image header, in first Segment.
uint8_t img_id_saved[16];           // Image ID. 16-byte IMahe ID, received in image header, in first Segment.

char imageFileName1[] = "dfu_file.bin";
char imagePathName1[] = "/mnt/mem1/dfu_file.bin";

// .............................................................................
// Read DFU File Status
// Returns true if valid/verified .bin file stored in external Flash
// .............................................................................
bool is_dfu_file_ok()
{
    return (rx_dfu_file_ok);
}

// .............................................................................
// This function is called when a JSON command to 'commit the DFU is received
// from Core app.
//
// It sets the bootloader signature in RAM, and initiate soft reset.
// THe signature will be checked on startup, so if the signature is written
// to just before soft reset, then on boot, the MCU will trigger the bootloader.
// .............................................................................
void dfu_bootloader_trigger(void)
{
    uint32_t i;
    
    if (rx_dfu_file_ok == false)
    {
        // Don't have a verified .bin file in external flash, 
        // so no point going further
        return;
    }

    ramStart[0] = BTL_TRIGGER_PATTERN;
    ramStart[1] = BTL_TRIGGER_PATTERN;
    ramStart[2] = BTL_TRIGGER_PATTERN;
    ramStart[3] = BTL_TRIGGER_PATTERN;

    // Cheap delay. This should give at leat 1 ms delay.
    for (i = 0; i < 2000; i++)
    {
        asm("nop");
    }

    // Perform reset
    NVIC_SystemReset();
}
// .............................................................................



// .............................................................................
// Process a received Image Message
// This will be one segment of many.
// 
// Format of Image segment File
// Each segment has a 5-byte header:
// [0] Segment ID
// [1,2] Segment No.
// [3,4] Segment Total
// And then for the first segment, it will include the image header
// [0,1] Image Width
// [2,3] Image Height
// [4..19] Image Id
//
// NOTE: Temporarily modifying this, because only have access to older NodeJS
//  script, so Segment Number and Segment Total are (temporarily) 1-byte each
//  not 2-byte.
//
// Need to reset Segment Number Expected and Segment Total
// If segment is ok (crc etc), and it is the first, extract (keep a copy) of
// its Width, Height, ID, and also the Segment Total. Make sure Segment Total
// is valid (not 0, and not too big).
// Then if this is the first segment, write the Image Data to a new file.
// On every subsequent Segment, verify it is in sequence, then append it to the
// file in external flash. We are checking the CRC for each segment, so I don't
// think we have an overall CRC.
// We can send an ACK for each segment received (for now), and maybe also a 
// final success when everything has been received and the file has been 
// successfully written to Flash.
// Also, at the very end, we should read back the file from Flash and verify its
// contents - this is just for now, while using test file with known contents.
//
// .............................................................................
int img_rx_process(uint8_t *rx_pkt, uint8_t src_id, uint8_t dest_id, uint8_t send_num, uint8_t ack_num)
{
    uint8_t rx_seg_id;
    uint16_t rx_seg_num;
    uint16_t rx_seg_tot;
    uint32_t dfu_f_size;
    uint16_t dfu_seg_size;
    int pay_len;
    int img_len;
    int res;
    int len;

    if (rx_pkt == NULL)
    {
        // Nothing to do
        return -1;
    }
    // Verify source: only interested in packets from Core app (for now))
    if (src_id != COMMS_ID_CORE)
    {
        // Packet is not from valid source
        return -2;
    }
    // Verify destination: only interested in packets for the MCU
    if (dest_id != COMMS_ID_MCU)
    {
        // Packet is not for MCU
        return -3;
    }
    
    // Take a copy of the received Segment information
    rx_seg_id = rx_pkt[7];
    rx_seg_num = (rx_pkt[8] * 256) + rx_pkt[9];
    rx_seg_tot = (rx_pkt[10] * 256) + rx_pkt[11];
    
    // Calculate total payload length of received message
    pay_len = (rx_pkt[COMMS_BYTE_0_PAYLOAD_HI] * 256) + rx_pkt[COMMS_BYTE_1_PAYLOAD_LO];
    if ((pay_len < 6) || (pay_len > 4096))
    {
        // pay_len should include 5 byte header (segment info, and at least one byte of data)
        // TODO: Adjust this for new image header
        return -3;
    }
    
    // See which segment we are expecting next
    if (seg_num_exp == 1) {
        // We have not yet started
        if (rx_seg_num != 1) {
            // Expected first segment, which this isn't, so error
            return -10;
        }
        if ((rx_seg_tot == 0) || (rx_seg_tot > IMG_MAX_SEGMENTS)) {
            // Invalid Segment Total
            return -11;
        }
        if (pay_len < (COMMS_HEADER_SIZE + IMG_MIN_SIZE)) {
            // Not enough bytes for header and min. image size
            return -12;
        }

        // Clear flag, until full valid file received
        rx_dfu_file_ok = false;
        
        // Other preliminary checks....
        dfu_f_size = json_data_dfu_file_size_read();
        if ((dfu_f_size < DFU_MIN_FILE_SIZE) || (dfu_f_size > DFU_MAX_FILE_SIZE))
        {
            // File Size not acceptable
            return -13;
        }
        dfu_seg_size = json_data_dfu_segment_size_read();
        if ((dfu_seg_size < DFU_MIN_SEG_SIZE) || (dfu_seg_size > DFU_MAX_SEG_SIZE))
        {
            // Segment Size not acceptable
            return -14;
        }
        // Calculate expected number of segments
        rx_seg_tot_expect = (dfu_f_size + (dfu_seg_size-1)) / dfu_seg_size;
        if ((rx_seg_tot_expect < DFU_MIN_SEG_TOTAL) || (rx_seg_tot_expect > DFU_MAX_SEG_TOTAL))
        {
            // Segment Total Count not acceptable
            return -15;
        }
        
        // For debugging:
        //app_mem_read_dir();

        // Fist segment, so empty / delete the image file:
        if (app_mem_file_exists(imagePathName1)) {
            app_mem_file_del(imagePathName1);
        }
        
    } else {
        // We have already started receiving
    }

    if (rx_seg_num != seg_num_exp)
    {
        // Out of sequence
        // TODO: Send error ....
        return -4;
    }
   
    img_len = pay_len - 5;          // Segment header is 5 bytes
    
    // Append received data to image file.
    res = app_mem_file_app(imageFileName1, &rx_pkt[12], img_len);       // When segment header is 5 bytes

    // Have we received the last segment?
    if (seg_num_exp == (uint16_t)rx_seg_tot_expect) {   
        // This should be the last segment

        // TODO: Call a function to verify received file integrity (calculate and compare CRCs)
        // If error, return here, otherwise set flag
        
        // Assume file is valid!
        rx_dfu_file_ok = true;
    }
    
    // Here, if we have received a segment successfully.
    
    // Get ready for next segment
    seg_num_exp++;
    
    json_fn_send_status_success();

    // Note: If this was the last segment, then the rx_dfu_file_ok flag has been 
    // set. This will be checked if the Core app subsequently sends a command
    // for the MCU to trigger its bootload update.

    // Success
    return 0;
}
// .............................................................................
