/*
 * main.h
 *
 *  Created on: April 16, 2021
 *      Author: Colleen Staeger
 */

#ifndef MAIN_H_
#define MAIN_H_

#include "msp.h"
#include "msp432p401r.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

/* RTOS header files */
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>

/* ECE353 Includes */
#include <lcd.h>
#include <minesweeper_images.h>
#include <task_accelerometer.h>
#include <task_minesweeper.h>
#include <task_buzzer.h>
#include <task_mk11_s1.h>
#include <task_opt3001.h>
#include <i2c.h>


#endif /* MAIN_H_ */
