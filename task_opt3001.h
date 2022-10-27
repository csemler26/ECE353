/*
 * task_opt3001.h
 *
 *  Created on: April 19, 2021
 *      Author: Colleen Staeger
 */

#ifndef TASK_OPT3001_H
#define TASK_OPT3001_H

#include <main.h>

#define OPT3001_SLAVE_ADDRESS 0x44

#define OPT3001_CONT 0x300

#define OPT_INTERRUPT_PIN 11
#define RESULT_REG 0x00
#define CONFIG_REG 0x01
#define LOWLIMIT_REG 0x02
#define HIGHLIMIT_REG 0x03
#define MANUFACTUREID_REG 0x7E
#define DEVICEID_REG 0x7F

extern TaskHandle_t Task_Opt3001_Read_Handle;

/******************************************************************************
 * Initialize the opt3001 ambient light sensor on the MKII.  This function assumes
 * that the I2C interface has already been configured to operate at 100KHz.
 ******************************************************************************/
void Task_Opt3001_Init(void);

/******************************************************************************
 * Returns the current ambient light in lux
 ******************************************************************************/
void Task_Opt3001_Read(void *pvParameters);

#endif /* TASK_OPT3001_H */
