/*
 * task_buzzer.c
 *
 *  Created on: April 19th, 2021
 *      Author: Colleen Staeger
 */

#include <main.h>

Note_t Song[] =
{

     {NOTE_G6,ONE_QUARTER,true},  // Tone, Time, Delay
     //NOTE_C
     {NOTE_C7,ONE_QUARTER,true},
     //NOTE_E
     {NOTE_E7,ONE_QUARTER,true},
     //NOTE_G
     {NOTE_G7,ONE_HALF,true},
     //NOTE_E
     {NOTE_E7,ONE_QUARTER,true},
     //NOTE_G
     {NOTE_G7,ONE_HALF,true},
};

Note_t Lose[] =
{
     {NOTE_F7,ONE_HALF,true},  // Tone, Time, Delay
     //NOTE_C
     {NOTE_E7,ONE_HALF,true},
     //NOTE_E
     {NOTE_D7S,ONE_HALF,true},
     //NOTE_G
     {NOTE_D7,THREE_EIGTH,true},
};

/******************************************************************************
 * This function will play a song on the Buzzer when game is lost.
 ******************************************************************************/
void playLosingSound(void)
{
    //loop through the array
    uint16_t i;

    for(i = 0; i < SONG_LOSE_NOTES; i++)
    {
        music_play_lose(i);
    }


}
/******************************************************************************
 * This function will play fanfare on the Buzzer when game is won.
 ******************************************************************************/
void playWiningSound(void)
{
    //loop through the array
    uint16_t i;

    for(i = 0; i < SONG_WIN_NOTES; i++)
    {
        music_play_note(i);
    }


}

//***************************************************************
// This function returns how long an individual  notes is played
//***************************************************************
uint32_t music_get_time_delay(measure_time_t time)
{
    uint32_t time_return = 0;

    time_return  = MEASURE_DURATION * MEASURE_RATIO;

    switch(time)
    {
        case ONE_QUARTER:
        {
            time_return  = time_return / 4;
            break;
        }
        case ONE_HALF:
        {
            time_return  = time_return / 2;
            break;
        }
        case ONE_EIGTH:
        {
            time_return  = time_return / 8;
            break;
        }
        case THREE_EIGTH:
        {
            time_return = time_return * 3;
            time_return  = time_return / 8;
            break;
        }
        default:
        {
            break;
        }
    }

    return time_return - DELAY_AMOUNT;

}


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
static void music_play_lose(uint16_t note_index)
{

    //determine period of PWM pulse
    ece353_MKII_Buzzer_Init(Lose[note_index].period);

    //determine time note plays
    uint32_t TIME_PLAYED = music_get_time_delay(Lose[note_index].time);

    //turn buzzer on
    ece353_MKII_Buzzer_On();

    //play for the specified duration
    T32_1_wait(TIME_PLAYED);

    //turn buzzer off
    ece353_MKII_Buzzer_Off();

    //determine if delay is needed
    if (Lose[note_index].delay == true)
    {
        //add a period of silence
        T32_1_wait(DELAY_AMOUNT);
        return;
    }
    else
    {
        return;
    }
}
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
static void music_play_note(uint16_t note_index)
{

    //determine period of PWM pulse
    ece353_MKII_Buzzer_Init(Song[note_index].period);

    //determine time note plays
    uint32_t TIME_PLAYED = music_get_time_delay(Song[note_index].time);

    //turn buzzer on
    ece353_MKII_Buzzer_On();

    //play for the specified duration
    T32_1_wait(TIME_PLAYED);

    //turn buzzer off
    ece353_MKII_Buzzer_Off();

    //determine if delay is needed
    if (Song[note_index].delay == true)
    {
        //add a period of silence
        T32_1_wait(DELAY_AMOUNT);
        return;
    }
    else
    {
        return;
    }
}

/*********************************************************
 *
 * Busy waits for a given number of clock cycles
 *
 * Parameters:
 *     ticks: Number of ticks to wait
 * Returns:
 *     None
 * *********************************************************/
void T32_1_wait(uint32_t ticks)
{

    //Stop the timer
    TIMER32_1->CONTROL = 0;

    //Set the timer to be a 32-bit, one-shot
    TIMER32_1->CONTROL = TIMER32_CONTROL_ONESHOT | TIMER32_CONTROL_SIZE;

    //Set the load register
    TIMER32_1->LOAD = ticks;

    //Start the timer
    TIMER32_1->CONTROL |= TIMER32_CONTROL_ENABLE;

    //Wait until it reaches 0
    while(TIMER32_1->VALUE != 0) {
        //timer is still counting, so wait
    }
}
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
void ece353_MKII_Buzzer_Init(uint16_t ticks_period){

    //Set P2.7 to be a GPIO OUTPUT pin
    P2->DIR |= BIT7;

    //the TIMERA PWM
    //P2.7 <----> TA0.4
    P2->SEL0 |= BIT7;
    P2->SEL1 &= ~BIT7;

    //Turn off TA0
    TIMER_A0->CTL = 0;

    //Set the period of the timer.
    TIMER_A0->CCR[0] = ticks_period - 1;

    //Configure BUZZER Duty Cycle to 50%
    TIMER_A0->CCR[4] = (ticks_period/2) - 1;

    //Configure TA0.4 for RESET/SET Mode
    TIMER_A0->CCTL[4] = TIMER_A_CCTLN_OUTMOD_7;

    //Select the master clock as the timer source
    TIMER_A0->CTL= TIMER_A_CTL_SSEL__SMCLK;

}
/***************************************************************
 *
 * Turns the Buzzer On
 *
 * Parameters:
 *      None
 * Returns:
 *      None
 ****************************************************************/
void ece353_MKII_Buzzer_On(void){

    //Clear the current Mode Control Bit
    TIMER_A0->CTL &= ~TIMER_A_CTL_MC_MASK;

    //Set Mode Control to UP and Clear the current count
    TIMER_A0->CTL |= TIMER_A_CTL_MC__UP | TIMER_A_CTL_CLR;
}
/*****************************************************************
   * Turns the Buzzer Off
  *
  * Parameters:
  *      None
  * Returns:
  *      None
  ****************************************************************/
 void ece353_MKII_Buzzer_Off(void){

     //Turn off the timer
     TIMER_A0->CTL &= ~TIMER_A_CTL_MC_MASK;

 }
