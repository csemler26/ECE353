/*
 * music.h
 *
 *      Author: Colleen Staeger
 */

#ifndef TASK_BUZZER_H_
#define TASK_BUZZER_H_

#include <main.h>

#define NOTE_A5        27272.72 // 880 Hz
#define NOTE_A5S       25751.07 // 932 Hz
#define NOTE_B5        24291.5  // 988 Hz
#define NOTE_C6        22944.56 // 1046 Hz
#define NOTE_C6S       21641.12 // 1109 Hz
#define NOTE_D6        20425.53 // 1175 Hz
#define NOTE_D6S       19277.11 // 1245 Hz
#define NOTE_E6        18195.60 // 1319 Hz
#define NOTE_F6        17179.67 // 1397 Hz
#define NOTE_F6S       16216.22 // 1480 Hz
#define NOTE_G6        15306.12 // 1568 Hz
#define NOTE_G6S       14449.13 // 1661 Hz
#define NOTE_A6        13636.36 // 1760 Hz
#define NOTE_A6S       12868.63 // 1865 Hz
#define NOTE_B6        12145.75 // 1976 Hz
#define NOTE_C7        11466.8  // 2093 Hz
#define NOTE_C7S       10825.4  // 2217 Hz
#define NOTE_D7        10217.1  // 2349 Hz
#define NOTE_D7S       9642.4   // 2489 Hz
#define NOTE_E7        9101.25  // 2637 Hz
#define NOTE_F7        8589.83  // 2794 Hz
#define NOTE_F7S       8108.11  // 2960 Hz
#define NOTE_G7        7653.06 // 3136 Hz
#define NOTE_G7S       7224.56 // 3322 Hz


#define MEASURE_DURATION 12000000    // 500 milliseconds
#define MEASURE_RATIO           2    // 2/4 time
#define DELAY_AMOUNT       240000    // 10  milliseconds
#define SONG_WIN_NOTES          6
#define SONG_LOSE_NOTES         4

typedef enum measure_time_t {
    ONE_QUARTER,
    ONE_HALF,
    ONE_EIGTH,
    THREE_EIGTH
} measure_time_t;

typedef struct{
    uint32_t period;
    measure_time_t time;
    bool delay;
}Note_t;

typedef struct{
        uint32_t period;
        measure_time_t time;
        bool delay;
}Lose_t;

/******************************************************************************
 * This function will play a song on the Buzzer when game is lost.
 ******************************************************************************/
void playLosingSound(void);
/******************************************************************************
 * This function will play fanfare on the Buzzer when game is won.
 ******************************************************************************/
void playWiningSound(void);
//***************************************************************
// This function returns how long an individual  notes is played
//***************************************************************
uint32_t music_get_time_delay(measure_time_t time);
//***************************************************************************
// Plays a single note of the song based on note_index.  After
// the note is played, there is an optional delay in between
// notes.
//
// Examples
// Song[note_index].period       -- Used to determine the period of the
//                                  PWM pulse.
//
// Song[note_index].time         -- 1/4 or 1/2 time note.  Call
//                                  music_get_time_delay() to determine how
//                                  long to play the note
//
// Song[note_index].delay        -- If true, add a period of silence for
//                                  DELAY_AMOUNT
//
//                                  If false, return without delaying.
//*************************************************************************
static void music_play_note(uint16_t note_index);
static void music_play_lose(uint16_t note_index);
/*********************************************************
 *
 * Busy waits for a given number of clock cycles
 *
 * Parameters:
 *     ticks: Number of ticks to wait
 * Returns:
 *     None
 * *********************************************************/
void T32_1_wait(uint32_t ticks);
/*************************************************************
 *
 * Sets the PWM period of the Buzzer. The duty cycle will be
 * set to 50%
 *
 * Parameters:
 *         ticks_period      :       Period of PWM pulse
 * Returns:
 *          None
 **************************************************************/
void ece353_MKII_Buzzer_Init(uint16_t ticks_period);
/***************************************************************
 *
 * Turns the Buzzer On
 *
 * Parameters:
 *      None
 * Returns:
 *      None
 ****************************************************************/
void ece353_MKII_Buzzer_On(void);

/*****************************************************************
   * Turns the Buzzer Off
  *
  * Parameters:
  *      None
  * Returns:
  *      None
  ****************************************************************/
 void ece353_MKII_Buzzer_Off(void);

#endif

