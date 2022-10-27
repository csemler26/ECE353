/*
 * task_mkII_s1.h
 *
 *  Created on: April 16, 2021
 *      Author: Colleen Staeger
 */

#ifndef TASK_MKII_S1_H_
#define TASK_MKII_S1_H_

#include "main.h"

extern TaskHandle_t Task_mkII_s1_Handle;

void MKII_S1_Init(void);

void Task_mkII_s1(void *pvParameters);

#endif /* TASK_MKII_S1_H_ */
