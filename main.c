/*
 *  ======== main_freertos.c ========
 */
#include "main.h"

/*
 *  ======== main ========
 */
int main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer

    //Initialize Resources Needed on the Board
    Task_Accelerometer_Init();
    Task_Minesweeper_Init();
    MKII_S1_Init();
    i2c_init();
    Task_Opt3001_Init();



    __enable_irq();

    xTaskCreate
     (   Task_MineSweeper,
         "Task_Mine_Sweeper",
         configMINIMAL_STACK_SIZE,
         NULL,
         1,
         &Task_Mine_Sweeper_Handle
     );

    xTaskCreate
    (   Task_Accelerometer_Timer,
        "Task_Accelerometer_Timer",
        configMINIMAL_STACK_SIZE,
        NULL,
        2,
        &Task_Accelerometer_Timer_Handle
    );

    xTaskCreate
    (   Task_Accelerometer_Bottom_Half,
        "Task_Joystick",
        configMINIMAL_STACK_SIZE,
        NULL,
        3,
        &Task_Accelerometer_Handle
    );

    xTaskCreate
    (   Task_mkII_s1,
        "Task_mkII_s1",
        configMINIMAL_STACK_SIZE,
        NULL,
        2,
        &Task_mkII_s1_Handle
    );

    xTaskCreate
    (   Task_Opt3001_Read,
        "Task_Opt3001_Read",
        configMINIMAL_STACK_SIZE,
        NULL,
        2,
        &Task_Opt3001_Read_Handle
    );





    /* Start the FreeRTOS scheduler */
    vTaskStartScheduler();

    while(1){};
    return (0);
}

//*****************************************************************************
//
//! \brief Application defined malloc failed hook
//!
//! \param  none
//!
//! \return none
//!
//*****************************************************************************
void vApplicationMallocFailedHook()
{
    /* Handle Memory Allocation Errors */
    while(1)
    {
    }
}

//*****************************************************************************
//
//! \brief Application defined stack overflow hook
//!
//! \param  none
//!
//! \return none
//!
//*****************************************************************************
void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName)
{
    //Handle FreeRTOS Stack Overflow
    while(1)
    {
    }
}
