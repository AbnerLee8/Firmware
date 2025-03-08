/*******************************************************************************
  File Name:
    app_mcu.c

  Summary:
    MCU to MCU communications task.

  Description:
 *******************************************************************************/
#include "app_mcu.h"

#define UART_RX_SIZE    256
uint8_t uart_rxbuf[UART_RX_SIZE];
uint8_t uart_byte[1];
BFIFO uart_rxfifo;

static uint scanstat;
volatile int enc_pos;
volatile uint8_t ev_but;
cmd_t Cmd;

// .............................................................................
bool mcu_uart_scan(int c, cmd_t *pcmd)
{
    bool res = false;
    switch (scanstat) {
        case 0: // wait for '['
            if (c == '[') {
                pcmd->idx = 0;
                pcmd->parno = 0;
                scanstat = 1;
            }
            break;
        case 1: // id, space delimited, or ']' for no parameter
            if (c == ']') { // command with no parameter
                pcmd->idstr[pcmd->idx] = 0;
                scanstat = 0;
                res = true;
            } 
            else if (c == ' ') { // there are parameters
                pcmd->idstr[pcmd->idx] = 0;
                pcmd->idx = 0;
                scanstat = 2;
            }
            else if (c > ' ' && c < 128) {
                if (pcmd->idx < CMD_ID_MAX-1) {
                    pcmd->idstr[pcmd->idx++] = c;
                }
            }
            break;
        case 2: // parameter, space delimited, or ']' for the last parameter
            if (c == ']') { // last parameter
                pcmd->parstr[pcmd->parno][pcmd->idx] = 0;
                pcmd->parno++;
                scanstat = 0;
                res = true;
            } 
            else if (c == ' ') { // there are more parameters
                pcmd->parstr[pcmd->parno][pcmd->idx] = 0;
                pcmd->idx = 0;
                if (pcmd->parno < CMD_PARNO_MAX-1) {
                    pcmd->parno++;
                }
            }
            else if (c > ' ' && c < 128) {
                if (pcmd->idx < CMD_PAR_MAX-1) {
                    pcmd->parstr[pcmd->parno][pcmd->idx++] = c;
                }
            }
            break;
        default:
            break;
    }
    return res;
}

// .............................................................................
// Non-blocking receive
// return -1 if no character received (rx fifo empty)
// .............................................................................
int mcu_uart_getchar()
{
	return bfifo_get(&uart_rxfifo);
}

// .............................................................................
void mcu_uart_read_callback( uintptr_t context )
{
   // bfifo_put(&uart_rxfifo, uart_byte[0]);
   // SERCOM1_USART_Read(uart_byte, 1);
}
// .............................................................................
void mcu_uart_init()
{
   // bfifo_init(&uart_rxfifo, uart_rxbuf, UART_RX_SIZE);
   // SERCOM1_USART_ReadCallbackRegister(mcu_uart_read_callback, 0);
    //SERCOM1_USART_Read(uart_byte, 1);
}
// .............................................................................


// .............................................................................
// Task initializations.
// This function runs BEFORE task is created and scheduler started!.
// .............................................................................
void APP_MCU_Initialize ( void )
{
}

// .............................................................................
// The MCU-MCU Task
// .............................................................................
void APP_MCU_Tasks ( void )
{
    Q_but_t qs_but;
    Q_led_t qs_led;
    button_config_t but_config;
    int ev, pos;
    int bat_v;

    // Task waits Power Task has powered everything up
    while (!GlobalPowerOn) {
        osDelayMs(100);
    }
    
    // Task waits until the necessary Queues have been enabled
    while (Q_enc_mcu == NULL) osDelayMs(10);
    while (Q_mcu_init == NULL) osDelayMs(10);
    while (Q_mcu_led == NULL) osDelayMs(10);

    // Initialise MCU UART FIFO and Callback function
    mcu_uart_init();
    
    // When the Encoder task is initialised (before this task), the Encoder
    // task sends three messages to this MCU task, to be passed on to the
    // Right MCU. These three messages have initialisation settings for the
    // following:
    // Right Encoder (e.g. steps per revolution)
    // Right Key Button (e.g. number of clicks enabled)
    // Right MFB Button (e.g. number of clicks enabled
    // SO the code below retrieves these three messages and passes the 
    // information to the RIght MCU, using the printf() functioon.
//    for (int i = 0; i < 3; i++) {
//        if (osQueueReceive(Q_mcu_init, &but_config, 50) == pdPASS) {
//            printf("[bconfig %d %d %d %d %d %d]\n",
//                        but_config.click_no_enable,
//                        but_config.long_press_enable,
//                        but_config.min_press_time,
//                        but_config.min_release_time,
//                        but_config.inter_click_time,
//                        but_config.long_press_time);
//        }
//    }
    
    while(1) {
        OS_SLEEP(1); //osDelayMs(1);        

        while (1) {
            // Process any messages coming from the Right MCU
            
            int c = mcu_uart_getchar();
            if (c == -1) break;
            // See if full command received
            if (mcu_uart_scan(c, &Cmd)) { 
                // A command is received:
                if (!strcmp(Cmd.idstr, "encbut")) {
                    /* encoder and button events */                    
                    sscanf(Cmd.parstr[0], "%d", &ev);
                    sscanf(Cmd.parstr[1], "%d", &pos);
                    qs_but.event = ev;
                    qs_but.pos = pos;
                    // Pass the information on to the Encoder task
                    osQueueSendToBack(Q_enc_mcu, &qs_but, 10);
                }
                else if (!strcmp(Cmd.idstr, "R")) {
                    // Debugging printf for the right side, 
                    // the right side sends the debug messages as "R" commands                     
                    if (Cmd.parno > 1)
                    {
                        // See if Battery Value received: "R V %dmV"
                        if (strcmp(Cmd.parstr[0], "V") == 0) {
                            sscanf(Cmd.parstr[1], "%d", &bat_v);
                            qs_but.event = BAT_UPDATE;
                            qs_but.pos = bat_v;
                            osQueueSendToBack(Q_pwr, &qs_but, 10);
                        }
                    }
                    // Debug output
                    /*
                    DBGprintf("R");
                    for (uint i = 0; i < Cmd.parno; i++) {
                        DBGprintf(" %s", Cmd.parstr[i]);
                    }
                    DBGprintf("\n");                    
                    */
                }
            }
        }
        
        // Check the MCU LED queue to see if any LEDs update commands received,
        // that need to be passed to the right MCU.
        if (osQueueReceive(Q_mcu_led, &qs_led, 1) == pdPASS)
        {
            // Yes. pass the LED command on to the Right MCU
            printf("[led blue %d %d %d %d]\n",
                    qs_led.mode,
                    qs_led.rate,
                    qs_led.cnt,
                    qs_led.dur);        
        }        
    }
}
// .............................................................................
