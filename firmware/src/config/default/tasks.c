/*******************************************************************************
 System Tasks File

  File Name:
    tasks.c

  Summary:
    This file contains source code necessary to maintain system's polled tasks.

  Description:
    This file contains source code necessary to maintain system's polled tasks.
    It implements the "SYS_Tasks" function that calls the individual "Tasks"
    functions for all polled MPLAB Harmony modules in the system.

  Remarks:
    This file requires access to the systemObjects global data structure that
    contains the object handles to all MPLAB Harmony module objects executing
    polled in the system.  These handles are passed into the individual module
    "Tasks" functions to identify the instance of the module to maintain.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2018 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *******************************************************************************/
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "configuration.h"
#include "definitions.h"
#include "sys_tasks.h"


// *****************************************************************************
// *****************************************************************************
// Section: RTOS "Tasks" Routine
// *****************************************************************************
// *****************************************************************************
/* Handle for the APP_ATMO_Tasks. */
TaskHandle_t xAPP_ATMO_Tasks;

static void lAPP_ATMO_Tasks(  void *pvParameters  )
{   
    while(true)
    {
        APP_ATMO_Tasks();
        vTaskDelay(50U / portTICK_PERIOD_MS);
    }
}
/* Handle for the APP_SNS_Tasks. */
TaskHandle_t xAPP_SNS_Tasks;

static void lAPP_SNS_Tasks(  void *pvParameters  )
{   
    while(true)
    {
        APP_SNS_Tasks();
        vTaskDelay(50U / portTICK_PERIOD_MS);
    }
}
/* Handle for the APP_MIC_Tasks. */
TaskHandle_t xAPP_MIC_Tasks;

static void lAPP_MIC_Tasks(  void *pvParameters  )
{   
    while(true)
    {
        APP_MIC_Tasks();
        vTaskDelay(50U / portTICK_PERIOD_MS);
    }
}
/* Handle for the APP_ENC_Tasks. */
TaskHandle_t xAPP_ENC_Tasks;

static void lAPP_ENC_Tasks(  void *pvParameters  )
{   
    while(true)
    {
        APP_ENC_Tasks();
        vTaskDelay(50U / portTICK_PERIOD_MS);
    }
}
/* Handle for the APP_PWR_Tasks. */
TaskHandle_t xAPP_PWR_Tasks;

static void lAPP_PWR_Tasks(  void *pvParameters  )
{   
    while(true)
    {
        APP_PWR_Tasks();
        vTaskDelay(50U / portTICK_PERIOD_MS);
    }
}
/* Handle for the APP_COM_Tasks. */
TaskHandle_t xAPP_COM_Tasks;

static void lAPP_COM_Tasks(  void *pvParameters  )
{   
    while(true)
    {
        APP_COM_Tasks();
        vTaskDelay(50U / portTICK_PERIOD_MS);
    }
}
/* Handle for the APP_MEM_Tasks. */
TaskHandle_t xAPP_MEM_Tasks;

static void lAPP_MEM_Tasks(  void *pvParameters  )
{   
    while(true)
    {
        APP_MEM_Tasks();
        vTaskDelay(50U / portTICK_PERIOD_MS);
    }
}
/* Handle for the BUT_MGR_Tasks. */
TaskHandle_t xBUT_MGR_Tasks;

static void lBUT_MGR_Tasks(  void *pvParameters  )
{   
    while(true)
    {
        BUT_MGR_Tasks();
        vTaskDelay(50U / portTICK_PERIOD_MS);
    }
}

/* Handle for the APP_JSON_Tasks. */
TaskHandle_t xAPP_JSON_Tasks;

static void lAPP_JSON_Tasks(  void *pvParameters  )
{   
    while(true)
    {
        APP_JSON_Tasks();
        vTaskDelay(50U / portTICK_PERIOD_MS);
    }
}


static void lSYS_FS_Tasks(  void *pvParameters  )
{
    while(true)
    {
        SYS_FS_Tasks();
        vTaskDelay(10U / portTICK_PERIOD_MS);
    }
}





// *****************************************************************************
// *****************************************************************************
// Section: System "Tasks" Routine
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void SYS_Tasks ( void )

  Remarks:
    See prototype in system/common/sys_module.h.
*/
void SYS_Tasks ( void )
{
    /* Maintain system services */
    
    (void) xTaskCreate( lSYS_FS_Tasks,
        "SYS_FS_TASKS",
        SYS_FS_STACK_SIZE,
        (void*)NULL,
        SYS_FS_PRIORITY,
        (TaskHandle_t*)NULL
    );



    /* Maintain Device Drivers */
    


    /* Maintain Middleware & Other Libraries */
    

    /* Maintain the application's state machine. */
        /* Create OS Thread for APP_ATMO_Tasks. */
    (void) xTaskCreate((TaskFunction_t) lAPP_ATMO_Tasks,
                "APP_ATMO_Tasks",
                512,
                NULL,
                1,
                &xAPP_ATMO_Tasks);

    /* Create OS Thread for APP_SNS_Tasks. */
    (void) xTaskCreate((TaskFunction_t) lAPP_SNS_Tasks,
                "APP_SNS_Tasks",
                512,
                NULL,
                1,
                &xAPP_SNS_Tasks);

    /* Create OS Thread for APP_MIC_Tasks. */
    (void) xTaskCreate((TaskFunction_t) lAPP_MIC_Tasks,
                "APP_MIC_Tasks",
                512,
                NULL,
                1,
                &xAPP_MIC_Tasks);

    /* Create OS Thread for APP_ENC_Tasks. */
    (void) xTaskCreate((TaskFunction_t) lAPP_ENC_Tasks,
                "APP_ENC_Tasks",
                512,
                NULL,
                1,
                &xAPP_ENC_Tasks);

    /* Create OS Thread for APP_PWR_Tasks. */
    (void) xTaskCreate((TaskFunction_t) lAPP_PWR_Tasks,
                "APP_PWR_Tasks",
                512,
                NULL,
                1,
                &xAPP_PWR_Tasks);

    /* Create OS Thread for APP_COM_Tasks. */
    (void) xTaskCreate((TaskFunction_t) lAPP_COM_Tasks,
                "APP_COM_Tasks",
                512,
                NULL,
                1,
                &xAPP_COM_Tasks);

    /* Create OS Thread for APP_MEM_Tasks. */
    (void) xTaskCreate((TaskFunction_t) lAPP_MEM_Tasks,
                "APP_MEM_Tasks",
                512,
                NULL,
                1,
                &xAPP_MEM_Tasks);

    /* Create OS Thread for BUT_MGR_Tasks. */
    (void) xTaskCreate((TaskFunction_t) lBUT_MGR_Tasks,
                "BUT_MGR_Tasks",
                512,
                NULL,
                1,
                &xBUT_MGR_Tasks);

    /* Create OS Thread for APP_JSON_Tasks. */
    (void) xTaskCreate((TaskFunction_t) lAPP_JSON_Tasks,
                "APP_JSON_Tasks",
                512,
                NULL,
                1,
                &xAPP_JSON_Tasks);



    /* Start RTOS Scheduler. */
    
     /**********************************************************************
     * Create all Threads for APP Tasks before starting FreeRTOS Scheduler *
     ***********************************************************************/
    vTaskStartScheduler(); /* This function never returns. */

}

/*******************************************************************************
 End of File
 */

