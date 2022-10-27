/*
 * task_accelerometer.c
 *
 *  Created on: April 16, 2021
 *      Author: Colleen Staeger
 */

#include <main.h>

 TaskHandle_t Task_Accelerometer_Handle;
 TaskHandle_t Task_Accelerometer_Timer_Handle;

 volatile uint32_t ACC_X_DIR = 0;
 volatile uint32_t ACC_Y_DIR = 0;

/******************************************************************************
* Initialize the Accelerometer
******************************************************************************/
 void Task_Accelerometer_Init(void)
 {
     // Configure the X direction as an analog input pin.
     P6->SEL0 |= BIT1;
     P6->SEL1 |= BIT1;

     // Configure the Y direction as an analog input pin.
     P4->SEL0 |= BIT0;
     P4->SEL1 |= BIT0;

     //Configure the Z direction as an analog input pin.
     P4->SEL0 |= BIT2;
     P4->SEL1 |= BIT2;

     // Configure CTL0 to sample 16-times in pulsed sample mode.
     ADC14->CTL0 = ADC14_CTL0_SHT0_2 | ADC14_CTL0_SHP | ADC14_CTL0_CONSEQ_1;

     // Configure CTL1 to return 12-bit values
     ADC14->CTL1 = ADC14_CTL1_RES_2;         // Use sampling timer, 12-bit conversion results

     // Associate the X direction analog signal with MEM[0]
     ADC14->MCTL[0] |= ADC14_MCTLN_INCH_14;

     // Associate the Y direction analog signal with MEM[1]
     ADC14->MCTL[1] |= ADC14_MCTLN_INCH_13 | ADC14_MCTLN_EOS;

     // Enable interrupts AFTER a value is written into MEM[1].
     ADC14->IER0 |=  ADC14_IER0_IE1;

     // Enable ADC Interrupt
     NVIC_EnableIRQ(ADC14_IRQn);

     // Interrupt priorities must NOT be set to 0.  It is preferred if they are set to a value that is > than 1 too.
     NVIC_SetPriority(ADC14_IRQn,2);

     // Turn ADC ON
     ADC14->CTL0 |= ADC14_CTL0_ON;

 }

 /******************************************************************************
 * Used to start an ADC14 Conversion
 ******************************************************************************/
 void Task_Accelerometer_Timer(void *pvParameters)
 {
     while(1)
     {

         //Start the ADC conversion
         ADC14->CTL0 |= ADC14_CTL0_SC | ADC14_CTL0_ENC;

         //Delay 5mS
         vTaskDelay(pdMS_TO_TICKS(5));

     }
 }


/******************************************************************************
* Bottom Half Task.  Examines the ADC data from the accelerometer on the MKII
******************************************************************************/
void Task_Accelerometer_Bottom_Half(void *pvParameters)
{
    MOVE_CMD_t game_cmd;
    MOVE_CMD_t prev_cmd = MOVE_CENTER;

    while(1)
      {
          /*
           * Wait until we get a task notification from the ADC14 ISR
           */
          ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

          /*
           * Set the cmd variable to one of the following values based
           * on the values of ACC_X_DIR and ACC_Y_DIR
           */
          if(ACC_X_DIR < VOLT_0P85)
          {
             game_cmd = MOVE_LEFT;
          }
          else if(ACC_X_DIR > VOLT_2P50)
          {
             game_cmd = MOVE_RIGHT;
          }
          else if(ACC_Y_DIR < VOLT_0P85)
          {
             game_cmd = MOVE_DOWN;
          }
          else if(ACC_Y_DIR > VOLT_2P50)
          {
              game_cmd = MOVE_UP;
          }
          else
          {
              game_cmd = MOVE_CENTER;
          }

          /*
           * Send cmd to Queue_Console if joystick is not in center position
           */
          if(game_cmd != prev_cmd){

              xQueueSendToBack(Queue_Mine_Sweeper, &game_cmd, portMAX_DELAY);

          }

          //Update the prev_dir of the accelerometer
          prev_cmd = game_cmd;

      }

}


/******************************************************************************
* Top Half of ADC14 Handler.
******************************************************************************/
void ADC14_IRQHandler(void)
{
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;

        ACC_X_DIR = ADC14->MEM[0]; // Read the value and clear the interrupt
        ACC_Y_DIR = ADC14->MEM[1]; // Read the value and clear the interrupt

        /*
         * Send a task notification to Task_Joystick_Bottom_Half
         */
        vTaskNotifyGiveFromISR(
                Task_Accelerometer_Handle,
                &xHigherPriorityTaskWoken
        );

        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}



