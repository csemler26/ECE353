/*
 * task_mk11_s1.c
 *
 *  Created on: April 16th, 2021
 *      Author: Colleen Staeger
 */
#include <main.h>

TaskHandle_t Task_mkII_s1_Handle;

/*******************************************************
 * Initialize hardware resources used to control S1
 *  on the MKII
 **********************************************************/
 void MKII_S1_Init(void) {

        //Configure as input
        P5->DIR &= ~BIT1;
 }


/******************************************************************************
 * De-bounce switch S1.
 *****************************************************************************/
bool debounce_s1(void)
{
    static uint8_t debounce_state = 0x00;

    // Shift the de-bounce variable to the left
    debounce_state = debounce_state << 1;

    // If S1 is being pressed, set the LSBit of debounce_state to a 1;
    if((P5->IN & BIT1) == 0)
    {
        debounce_state |= 0x01;
    }

    // If the de-bounce variable is equal to 0x7F, dig up spot on board.
    if(debounce_state == 0x7F)
    {
        return true;
    }
    else
    {
        return false;
    }

}

/******************************************************************************
 * De-bounce switch S1.  If is has been pressed, dig up current spot on the board
 *****************************************************************************/
void Task_mkII_s1(void *pvParameters)
{
    MOVE_CMD_t cmd;

    while(1)
    {
        //If S1 is pressed, send a message
        if(debounce_s1())
        {
           cmd = S1_PRESS;

           //Send msg to Queue_Minesweeper
           xQueueSendToBack(Queue_Mine_Sweeper, &cmd, portMAX_DELAY);
        }

        //Delay for 10ms using vTaskDelay
        vTaskDelay(pdMS_TO_TICKS(10));

    }

}

