/*
 * task_minesweeper.c
 *
 *  Created on: Apr 14, 2021
 *      Author: chris
 */
#include <main.h>

TaskHandle_t Task_Mine_Sweeper_Handle;
QueueHandle_t Queue_Mine_Sweeper;

// initialize boards
// bombs are indicated by a int of 9
int board1[5][5] = {
                    {1,1,1,1,1},
                    {2,9,1,1,9},
                    {9,2,1,1,1},
                    {1,1,1,1,1},
                    {0,0,1,9,1}
};

int board2[5][5] = {
                    {9,1,1,1,1},
                    {2,2,2,9,1},
                    {1,9,2,1,1},
                    {1,1,2,1,1},
                    {0,0,1,9,1}
};

int board3[5][5] = {
                    {1,1,2,1,1},
                    {2,9,2,9,1},
                    {3,9,3,1,1},
                    {9,2,1,0,0},
                    {1,1,0,0,0}
};

int board4[5][5] = {
                    {0,1,1,1,0},
                    {1,2,9,2,1},
                    {1,9,4,9,1},
                    {1,2,9,2,1},
                    {0,1,1,1,0}
};

int board5[5][5] = {
                    {9,1,0,1,9},
                    {1,1,0,1,1},
                    {0,0,0,0,0},
                    {1,1,0,1,1},
                    {9,1,0,1,9}
};

// initialize temp variables
int selectedSpots[5][5];
int numOfMoves = 0;
int numOfBombs = 4;
int boardSize = 5;
int numOfTotalFree = 21;
int currentBoard = 0;
bool light = true;         // true indicates light room; false indicates dark room
int x = 3;
int y = 3;

/******************************************************************************
 * This function will initialize the LCD
 ******************************************************************************/
void Task_Minesweeper_Init(void)
{
    //Create Mine_Sweeper Queue
    Queue_Mine_Sweeper = xQueueCreate(GAME_QUEUE_LENGTH, sizeof(MOVE_CMD_t));

    // initialize LCD
    Crystalfontz128x128_Init();
}

///*******************************************************************************
//* Function Name: minesweeper_example_board
//********************************************************************************
//* Summary: Prints out an example of what the minesweeper board looks like
//* Returns:
//*  Nothing
//*******************************************************************************/
//void minesweeper_example_board()
//{
//    // Top Row
//    lcd_draw_image(COL_ONE, ROW_ONE, SQUARE_SIZE, SQUARE_SIZE, One_Bitmaps, LCD_COLOR_RED, LCD_COLOR_BLACK);
//    lcd_draw_image(COL_TWO,ROW_ONE,SQUARE_SIZE,SQUARE_SIZE,Two_Bitmaps,LCD_COLOR_YELLOW,LCD_COLOR_BLACK);
//    lcd_draw_image(COL_THREE,ROW_ONE,SQUARE_SIZE,SQUARE_SIZE,Three_Bitmaps,LCD_COLOR_GREEN,LCD_COLOR_BLACK);
//    lcd_draw_image(COL_FOUR,ROW_ONE,SQUARE_SIZE,SQUARE_SIZE,Four_Bitmaps,LCD_COLOR_YELLOW,LCD_COLOR_BLACK);
//    lcd_draw_image(COL_FIVE, ROW_ONE, SQUARE_SIZE, SQUARE_SIZE, Five_Bitmaps, LCD_COLOR_GREEN, LCD_COLOR_BLACK);
//
//    // Second Row
//    lcd_draw_image(COL_ONE, ROW_TWO, SQUARE_SIZE, SQUARE_SIZE, Six_Bitmaps, LCD_COLOR_RED, LCD_COLOR_BLACK);
//    lcd_draw_image(COL_TWO,ROW_TWO,SQUARE_SIZE,SQUARE_SIZE,Seven_Bitmaps,LCD_COLOR_YELLOW,LCD_COLOR_BLACK);
//    lcd_draw_image(COL_THREE,ROW_TWO,SQUARE_SIZE,SQUARE_SIZE,Eight_Bitmaps,LCD_COLOR_GREEN,LCD_COLOR_BLACK);
//    lcd_draw_image(COL_FOUR,ROW_TWO,SQUARE_SIZE,SQUARE_SIZE,bombBitmaps,LCD_COLOR_YELLOW,LCD_COLOR_BLACK);
//    lcd_draw_image(COL_FIVE,ROW_TWO,SQUARE_SIZE,SQUARE_SIZE,One_Bitmaps,LCD_COLOR_GREEN,LCD_COLOR_BLACK);
//
//    // Center Row
//    lcd_draw_image(COL_ONE,ROW_THREE,SQUARE_SIZE,SQUARE_SIZE,One_Bitmaps,LCD_COLOR_RED,LCD_COLOR_BLACK);
//    lcd_draw_image(COL_TWO,ROW_THREE,SQUARE_SIZE,SQUARE_SIZE,One_Bitmaps,LCD_COLOR_YELLOW,LCD_COLOR_BLACK);
//    lcd_draw_image(COL_THREE,ROW_THREE,SQUARE_SIZE,SQUARE_SIZE,One_Bitmaps,LCD_COLOR_GREEN,LCD_COLOR_BLACK);
//    lcd_draw_image(COL_FOUR,ROW_THREE,SQUARE_SIZE,SQUARE_SIZE,One_Bitmaps,LCD_COLOR_YELLOW,LCD_COLOR_BLACK);
//    lcd_draw_image(COL_FIVE,ROW_THREE,SQUARE_SIZE,SQUARE_SIZE,One_Bitmaps,LCD_COLOR_GREEN,LCD_COLOR_BLACK);
//
//    // Fourth Row
//    lcd_draw_image(COL_ONE,ROW_FOUR,SQUARE_SIZE,SQUARE_SIZE,One_Bitmaps,LCD_COLOR_RED,LCD_COLOR_BLACK);
//    lcd_draw_image(COL_TWO,ROW_FOUR,SQUARE_SIZE,SQUARE_SIZE,One_Bitmaps,LCD_COLOR_YELLOW,LCD_COLOR_BLACK);
//    lcd_draw_image(COL_THREE,ROW_FOUR,SQUARE_SIZE,SQUARE_SIZE,One_Bitmaps,LCD_COLOR_GREEN,LCD_COLOR_BLACK);
//    lcd_draw_image(COL_FOUR,ROW_FOUR,SQUARE_SIZE,SQUARE_SIZE,One_Bitmaps,LCD_COLOR_YELLOW,LCD_COLOR_BLACK);
//    lcd_draw_image(COL_FIVE,ROW_FOUR,SQUARE_SIZE,SQUARE_SIZE,One_Bitmaps,LCD_COLOR_GREEN,LCD_COLOR_BLACK);
//
//    // Lower Row
//    lcd_draw_image(COL_ONE,ROW_FIVE,SQUARE_SIZE,SQUARE_SIZE,One_Bitmaps,LCD_COLOR_RED,LCD_COLOR_BLACK);
//    lcd_draw_image(COL_TWO,ROW_FIVE,SQUARE_SIZE,SQUARE_SIZE,One_Bitmaps,LCD_COLOR_YELLOW,LCD_COLOR_BLACK);
//    lcd_draw_image(COL_THREE,ROW_FIVE,SQUARE_SIZE,SQUARE_SIZE,One_Bitmaps,LCD_COLOR_GREEN,LCD_COLOR_BLACK);
//    lcd_draw_image(COL_FOUR,ROW_FIVE,SQUARE_SIZE,SQUARE_SIZE,One_Bitmaps,LCD_COLOR_YELLOW,LCD_COLOR_BLACK);
//    lcd_draw_rectangle(COL_FIVE, ROW_FIVE, SQUARE_SIZE, SQUARE_SIZE, LCD_COLOR_GRAY);
//}

/*
 * Helper function to draw images
 * Parameters:
 *      x: horizontal location
 *      y: vertical location
 */
void drawSpot(bool isCursor)
{
    // temp variables to hold y and x INDEX values
    int tempY = y - 1; // y is always 1-5
    int tempX = x - 1; // x is always 1-5

    // get the number at the spot on the current board
    int num;
    //board 1
    if (currentBoard == 1)
    {
        num = board1[tempY][tempX];
    }
    //board 2
    else if (currentBoard == 2)
    {
        num = board2[tempY][tempX];
    }
    //board 3
    else if (currentBoard == 3)
    {
        num = board3[tempY][tempX];
    }
    //board 4
    else if (currentBoard == 4)
    {
        num = board4[tempY][tempX];
    }
    //board 5
    else if (currentBoard == 5)
    {
        num = board5[tempY][tempX];
    }

    // Determine row and col
    uint16_t row = 0;
    if (y == 1)
        row = ROW_ONE;
    else if (y == 2)
        row = ROW_TWO;
    else if (y == 3)
        row = ROW_THREE;
    else if (y == 4)
        row = ROW_FOUR;
    else if (y == 5)
        row = ROW_FIVE;

    uint16_t col = 0;
    if (x == 1)
        col = COL_ONE;
    else if (x == 2)
        col = COL_TWO;
    else if (x == 3)
        col = COL_THREE;
    else if (x == 4)
        col = COL_FOUR;
    else if (x == 5)
        col = COL_FIVE;

    // if in light room, use blue
    // if in a dark room, use magenta
    uint16_t color = LCD_COLOR_MAGENTA;
    if (light == true)
    {
        color = LCD_COLOR_BLUE2;
    }

    // Case 1: check to see if spot has been selected already
    if (selectedSpots[tempY][tempX] == 1)
    {
        // Case 1.1: selected spot is also cursor
        if (isCursor)
        {
            lcd_draw_rectangle(col, row, SQUARE_SIZE, SQUARE_SIZE, LCD_COLOR_RED);
            // draw number accordingly
            if (num == 1)
            {
                lcd_draw_image(col, row, One_Width, One_Height, One_Bitmaps, color, LCD_COLOR_RED);
            }
            else if (num == 2)
            {
                lcd_draw_image(col, row, Two_Width, Two_Height, Two_Bitmaps, color, LCD_COLOR_RED);
            }
            else if (num == 3)
            {
                lcd_draw_image(col, row, Three_Width, Three_Height, Three_Bitmaps, color, LCD_COLOR_RED);
            }
            else if (num == 4)
            {
                lcd_draw_image(col, row, Four_Width, Four_Height, Four_Bitmaps, color, LCD_COLOR_RED);
            }
            else if (num == 5)
            {
                lcd_draw_image(col, row, Five_Width, Five_Height, Five_Bitmaps, color, LCD_COLOR_RED);
            }
            else if (num == 6)
            {
                lcd_draw_image(col, row, Six_Width, Six_Height, Six_Bitmaps, color, LCD_COLOR_RED);
            }
            else if (num == 7)
            {
                lcd_draw_image(col, row, Seven_Width, Seven_Height, Seven_Bitmaps, color, LCD_COLOR_RED);
            }
            else if (num == 8)
            {
                lcd_draw_image(col, row, Eight_Width, Eight_Height, Eight_Bitmaps, color, LCD_COLOR_RED);
            }
            else if (num == 9)
            {
                lcd_draw_image(col, row, bombWidthPixels, bombHeightPixels, bombBitmaps, color, LCD_COLOR_RED);
            }
        }
        // Case 1.2: selected spot is NOT cursor
        else
        {
            lcd_draw_rectangle(col, row, SQUARE_SIZE, SQUARE_SIZE, LCD_COLOR_WHITE);
            // draw number accordingly
            if (num == 1)
            {
                lcd_draw_image(col, row, One_Width, One_Height, One_Bitmaps, color, LCD_COLOR_WHITE);
            }
            else if (num == 2)
            {
                lcd_draw_image(col, row, Two_Width, Two_Height, Two_Bitmaps, color, LCD_COLOR_WHITE);
            }
            else if (num == 3)
            {
                lcd_draw_image(col, row, Three_Width, Three_Height, Three_Bitmaps, color, LCD_COLOR_WHITE);
            }
            else if (num == 4)
            {
                lcd_draw_image(col, row, Four_Width, Four_Height, Four_Bitmaps, color, LCD_COLOR_WHITE);
            }
            else if (num == 5)
            {
                lcd_draw_image(col, row, Five_Width, Five_Height, Five_Bitmaps, color, LCD_COLOR_WHITE);
            }
            else if (num == 6)
            {
                lcd_draw_image(col, row, Six_Width, Six_Height, Six_Bitmaps, color, LCD_COLOR_WHITE);
            }
            else if (num == 7)
            {
                lcd_draw_image(col, row, Seven_Width, Seven_Height, Seven_Bitmaps, color, LCD_COLOR_WHITE);
            }
            else if (num == 8)
            {
                lcd_draw_image(col, row, Eight_Width, Eight_Height, Eight_Bitmaps, color, LCD_COLOR_WHITE);
            }
            else if (num == 9)
            {
                lcd_draw_image(col, row, bombWidthPixels, bombHeightPixels, bombBitmaps, color, LCD_COLOR_WHITE);
            }
        }
    }
    // Case 2: spot has not been selected yet
    else
    {
        // Case 2.1: non-selected spot is also cursor
        if (isCursor)
        {
            lcd_draw_rectangle(col, row, SQUARE_SIZE, SQUARE_SIZE, LCD_COLOR_GREEN);
        }
        // Case 2.2: non-selected spot is not the cursor
        else
        {
            lcd_draw_rectangle(col, row, SQUARE_SIZE, SQUARE_SIZE, LCD_COLOR_GRAY);
        }
    }
}

/*
 * Helper function to reset board and draw starting state
 */
void resetBoard()
{
    // reset selected 2d array
    int r;
    int c;
    for(r = 0; r < boardSize; r++)
    {
        for (c = 0; c < boardSize; c++)
        {
            selectedSpots[r][c] = 0;
        }
    }

    // redraw board
    lcd_draw_rectangle(COL_THREE, ROW_THREE, SCREEN_X, SCREEN_Y, LCD_COLOR_BLACK);
    // row 1
    lcd_draw_rectangle(COL_ONE, ROW_ONE, SQUARE_SIZE, SQUARE_SIZE, LCD_COLOR_GRAY);
    lcd_draw_rectangle(COL_TWO, ROW_ONE, SQUARE_SIZE, SQUARE_SIZE, LCD_COLOR_GRAY);
    lcd_draw_rectangle(COL_THREE, ROW_ONE, SQUARE_SIZE, SQUARE_SIZE, LCD_COLOR_GRAY);
    lcd_draw_rectangle(COL_FOUR, ROW_ONE, SQUARE_SIZE, SQUARE_SIZE, LCD_COLOR_GRAY);
    lcd_draw_rectangle(COL_FIVE, ROW_ONE, SQUARE_SIZE, SQUARE_SIZE, LCD_COLOR_GRAY);
    // row 2
    lcd_draw_rectangle(COL_ONE, ROW_TWO, SQUARE_SIZE, SQUARE_SIZE, LCD_COLOR_GRAY);
    lcd_draw_rectangle(COL_TWO, ROW_TWO, SQUARE_SIZE, SQUARE_SIZE, LCD_COLOR_GRAY);
    lcd_draw_rectangle(COL_THREE, ROW_TWO, SQUARE_SIZE, SQUARE_SIZE, LCD_COLOR_GRAY);
    lcd_draw_rectangle(COL_FOUR, ROW_TWO, SQUARE_SIZE, SQUARE_SIZE, LCD_COLOR_GRAY);
    lcd_draw_rectangle(COL_FIVE, ROW_TWO, SQUARE_SIZE, SQUARE_SIZE, LCD_COLOR_GRAY);
    // row 3
    lcd_draw_rectangle(COL_ONE, ROW_THREE, SQUARE_SIZE, SQUARE_SIZE, LCD_COLOR_GRAY);
    lcd_draw_rectangle(COL_TWO, ROW_THREE, SQUARE_SIZE, SQUARE_SIZE, LCD_COLOR_GRAY);
    lcd_draw_rectangle(COL_THREE, ROW_THREE, SQUARE_SIZE, SQUARE_SIZE, LCD_COLOR_GRAY);
    lcd_draw_rectangle(COL_FOUR, ROW_THREE, SQUARE_SIZE, SQUARE_SIZE, LCD_COLOR_GRAY);
    lcd_draw_rectangle(COL_FIVE, ROW_THREE, SQUARE_SIZE, SQUARE_SIZE, LCD_COLOR_GRAY);
    // row 4
    lcd_draw_rectangle(COL_ONE, ROW_FOUR, SQUARE_SIZE, SQUARE_SIZE, LCD_COLOR_GRAY);
    lcd_draw_rectangle(COL_TWO, ROW_FOUR, SQUARE_SIZE, SQUARE_SIZE, LCD_COLOR_GRAY);
    lcd_draw_rectangle(COL_THREE, ROW_FOUR, SQUARE_SIZE, SQUARE_SIZE, LCD_COLOR_GRAY);
    lcd_draw_rectangle(COL_FOUR, ROW_FOUR, SQUARE_SIZE, SQUARE_SIZE, LCD_COLOR_GRAY);
    lcd_draw_rectangle(COL_FIVE, ROW_FOUR, SQUARE_SIZE, SQUARE_SIZE, LCD_COLOR_GRAY);
    // row 5
    lcd_draw_rectangle(COL_ONE, ROW_FIVE, SQUARE_SIZE, SQUARE_SIZE, LCD_COLOR_GRAY);
    lcd_draw_rectangle(COL_TWO, ROW_FIVE, SQUARE_SIZE, SQUARE_SIZE, LCD_COLOR_GRAY);
    lcd_draw_rectangle(COL_THREE, ROW_FIVE, SQUARE_SIZE, SQUARE_SIZE, LCD_COLOR_GRAY);
    lcd_draw_rectangle(COL_FOUR, ROW_FIVE, SQUARE_SIZE, SQUARE_SIZE, LCD_COLOR_GRAY);
    lcd_draw_rectangle(COL_FIVE, ROW_FIVE, SQUARE_SIZE, SQUARE_SIZE, LCD_COLOR_GRAY);
}

/*
 * Helper function to determine if game was lost
 */
bool didGameEnd()
{
    // get the number at the spot on the current board
    int num;

    // temp variables to hold INDEX of x and y
    int tempX = x - 1;
    int tempY = y - 1;

    //board 1
    if (currentBoard == 1)
    {
        num = board1[tempY][tempX];
    }
    //board 2
    else if (currentBoard == 2)
    {
        num = board2[tempY][tempX];
    }
    //board 3
    else if (currentBoard == 3)
    {
        num = board3[tempY][tempX];
    }
    //board 4
    else if (currentBoard == 4)
    {
        num = board4[tempY][tempX];
    }
    //board 5
    else if (currentBoard == 5)
    {
        num = board5[tempY][tempX];
    }

    // game will be lost if num equals 9 (a bomb)
    if (num == 9)
    {
        //reset numOfMoves
        numOfMoves = 0;
        lcd_draw_image(ROW_THREE, COL_THREE, loserWidth, loserHeight, loserBitmaps, LCD_COLOR_RED, LCD_COLOR_BLACK);
        playLosingSound();
        return true;
    }
    // check if user won the game
    else if (numOfMoves == numOfTotalFree)
    {
        //reset numOfMoves
        numOfMoves = 0;
        lcd_draw_image(ROW_THREE, COL_THREE, winnerWidth, winnerHeight, winnerBitmaps, LCD_COLOR_YELLOW, LCD_COLOR_BLACK);
        playWiningSound();
        return true;
    }

    // game did not end
    return false;
}

/*
 * Helper function to determine if cursor's spot is valid
 */
bool checkValid(int col, int row)
{
    int output;
    output = selectedSpots[row][col];
    bool returnVal;
    if (output == 0)
    {
        returnVal = true;
    }
    else
    {
        returnVal = false;
    }

    return returnVal;
}

/******************************************************************************
 * Function that is main part of game.
 ******************************************************************************/
void Task_MineSweeper(void *pvParameters)
{
    // Local temp variables
    MOVE_CMD_t msg;
    bool spotIsValid;
    bool endGame;

    //never leave task
    while (1)
    {
        // start a new game
        resetBoard();
        endGame = false;
        spotIsValid = true;
        numOfMoves = 0;
        x = 3;
        y = 3;
        drawSpot(true);
        currentBoard++;      // cycle through all five boards
        if (currentBoard > 5)
            currentBoard = 1;

        while (endGame == false)
        {

            //wait for a task
            //  8 possibilities are (Up, Down, Left, Right, Center, Bright light, Dark light, or S1 Press)
            xQueueReceive(Queue_Mine_Sweeper, &msg, portMAX_DELAY);

            // check cases
            // Case 1 - joystick at center
            if (msg == MOVE_CENTER)
            {
                // do nothing
            }
            // Case 2 - joystick up
            else if (msg == MOVE_UP)
            {
                // redraw spot where cursor was
                drawSpot(false);
                y--;
                if (y < 1)
                {
                    y = boardSize;
                }
                drawSpot(true);
            }
            // Case 3 - joystick down
            else if (msg == MOVE_DOWN)
            {
                // redraw spot where cursor was
                drawSpot(false);
                y++;
                if (y > boardSize)
                {
                    y = 1;
                }
                drawSpot(true);
            }
            // Case 4 - joystick right
            else if (msg == MOVE_RIGHT)
            {
                // redraw spot where cursor was
                drawSpot(false);
                x++;
                if (x > boardSize)
                {
                    x = 1;
                }
                drawSpot(true);
            }
            // Case 5 - joystick left
            else if (msg == MOVE_LEFT)
            {
                // redraw spot where cursor was
                drawSpot(false);
                x--;
                if (x < 1)
                {
                    x = boardSize;
                }
                drawSpot(true);
            }
            // Case 6 - Light room
            else if (msg == YES_LIGHT)
            {
                light = true;
                //update cursor color accordingly
                drawSpot(true);
            }
            // Case 7 - Dark room
            else if (msg == NO_LIGHT)
            {
                light = false;
                //update cursor color accordingly
                drawSpot(true);
            }
            // Case 8 - button pressed
            else if (msg == S1_PRESS)
            {
                if (spotIsValid)
                {
                    numOfMoves++;
                    // temp vars for INDEX of y and x
                    int tempX = x - 1;
                    int tempY = y - 1;
                    selectedSpots[tempY][tempX] = 1;
                    drawSpot(true);

                    endGame = didGameEnd();
                }
            }
        }

        // wait for a new button press
        msg = MOVE_CENTER;
        xQueueReceive(Queue_Mine_Sweeper, &msg, portMAX_DELAY);
        while (msg != S1_PRESS)
        {
            xQueueReceive(Queue_Mine_Sweeper, &msg, portMAX_DELAY);
        }
        msg = MOVE_CENTER;
    }
}


