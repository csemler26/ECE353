/*
 * task_minesweeper.h
 *
 *  Created on: Apr 14, 2021
 *      Author: chris
 */

#ifndef TASK_MINESWEEPER_H_
#define TASK_MINESWEEPER_H_

#include "main.h"
#include "lcd.h"

extern TaskHandle_t Task_Mine_Sweeper_Handle;
extern QueueHandle_t Queue_Mine_Sweeper;

typedef enum {
    MOVE_LEFT,
    MOVE_RIGHT,
    MOVE_UP,
    MOVE_DOWN,
    MOVE_CENTER,
    S1_PRESS,
    NO_LIGHT,
    YES_LIGHT
}MOVE_CMD_t;

#define GAME_QUEUE_LENGTH  2

#define SCREEN_X                132
#define SCREEN_Y                132

#define SQUARE_SIZE             24        // 24x24 pixels per square

#define SCREEN_CENTER_COL       ((SCREEN_X/2)-1)
#define SCREEN_CENTER_ROW       ((SCREEN_Y/2)-1)

#define PADDING                 2

#define COL_ONE                 (SCREEN_CENTER_COL - (2 * (SQUARE_SIZE + PADDING)))
#define COL_TWO                 (SCREEN_CENTER_COL - SQUARE_SIZE - PADDING)
#define COL_THREE               (SCREEN_CENTER_COL)
#define COL_FOUR                (SCREEN_CENTER_COL + SQUARE_SIZE + PADDING)
#define COL_FIVE                (SCREEN_CENTER_COL + (2 * (SQUARE_SIZE + PADDING)))

#define ROW_ONE                 (SCREEN_CENTER_ROW - (2 * (SQUARE_SIZE + PADDING)))
#define ROW_TWO                 (SCREEN_CENTER_ROW - SQUARE_SIZE - PADDING)
#define ROW_THREE               (SCREEN_CENTER_ROW)
#define ROW_FOUR                (SCREEN_CENTER_ROW + SQUARE_SIZE + PADDING)
#define ROW_FIVE                (SCREEN_CENTER_ROW + (2 * (SQUARE_SIZE + PADDING)))

/******************************************************************************
 * This function will initialize the LCD
 ******************************************************************************/
void Task_Minesweeper_Init(void);

/*******************************************************************************
* Function Name: minesweeper_example_board
********************************************************************************
* Summary: Prints out an example of what the minesweeper board looks like
* Returns:
*  Nothing
*******************************************************************************/
void minesweeper_example_board(void);

/******************************************************************************
 * Function that is main part of game.
 ******************************************************************************/
void Task_MineSweeper(void *pvParameters);

#endif /* TASK_MINESWEEPER_H_ */
