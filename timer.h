/*
** Seniriu
** Matthew Suttinger & Emerald Nova (johannesfetz@gmail.com)
** 
** This work is licensed under a Attribution-NonCommercial 4.0 International License
** More info at: https://creativecommons.org/licenses/by-nc/4.0/legalcode
*/

#ifndef __TIMER_H__
#define __TIMER_H__

#include <jo/jo.h>

volatile Uint8 * mcuFrcHigh;
extern FIXED time;
extern FIXED oldtime;

//	Functions
void poll_HighFreq(void);

#endif
