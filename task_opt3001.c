/*
 * task_opt3001.c
 *
 *  Created on: April 14, 2021
 *      Author: Colleen Staeger
 */

#include <main.h>
#include "math.h"

TaskHandle_t Task_Opt3001_Read_Handle;

/******************************************************************************
 * Initialize the opt3001 ambient light sensor on the MKII.  This function assumes
 * that the I2C interface has already been configured to operate at 100KHz.
 ******************************************************************************/

void Task_Opt3001_Init(void)
{
    // Initialize OPT3001

    i2c_write_16(OPT3001_SLAVE_ADDRESS, CONFIG_REG, 0xC710);
}


/******************************************************************************
 * Returns the current ambient light in lux and sends msg to game
 *
 * Delays 10ms between each reading
 *******************************************************************************/
void Task_Opt3001_Read(void *pvParameters)
{
    MOVE_CMD_t cmd;
    while(1)
    {

       // Read the Result register of OPT3001 and convert into Lux, then return.
       uint16_t readResult = i2c_read_16(OPT3001_SLAVE_ADDRESS, RESULT_REG);
       uint16_t exp = readResult >> 12;
       uint16_t result = readResult & 0x0FFF;


       float lux = (0.01 * pow(2, exp) * result);


       //If light sensor is covered, send msg to Queue
       if(lux < 10.0)
       {
           cmd = NO_LIGHT;

           xQueueSendToBack(Queue_Mine_Sweeper, &cmd, portMAX_DELAY);
       }
       // if not covered, send msg to Queue
       else
       {
           cmd = YES_LIGHT;

           xQueueSendToBack(Queue_Mine_Sweeper, &cmd, portMAX_DELAY);
       }

       //Delay 100mS
       vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
