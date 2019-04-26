/*
** Seniriu
** Matthew Suttinger & Emerald Nova (emeraldnovagames@gmail.com)
** 
** This work is licensed under a Attribution-NonCommercial 4.0 International License
** More info at: https://creativecommons.org/licenses/by-nc/4.0/legalcode
*/

#include "timer.h"
#include <jo/jo.h>

// Pointer to SH2 Registers, found by Johannes Fetz, contributed by Ponut64
// High Free Running Counter Register (FCR), counts up to 255, then iterates FCR Low
volatile Uint8 * SH2FRCHigh = (volatile Uint8 *)0xfffffe12;
// Low Free Running Counter Register (FCR), increases every time FCR high reaches 256
volatile Uint8 * SH2FRCLow = (volatile Uint8 *)0xfffffe13;
// Time Control Register (TCR)
volatile Uint8 * SH2TCR = (volatile Uint8 *)0xfffffe16;
// System clock
volatile unsigned int * SysClockReg = (volatile unsigned int*)0x6000324;
//	Time tracking in seconds
FIXED time = toFIXED(0.0);
FIXED oldtime = toFIXED(0.0);
static Uint16 delta_time = 1;

//	Get high frequency count from chip and translate to seconds
/*
void poll_HighFreq(void)
{
	oldtime = time;
	//	7680 = 30 (correction factor) * 256 (Uint8 range)
	time += slDivFX(toFIXED(7680.0),toFIXED(*SH2FRCHigh));
}
*/

//	JO Engine sourced timer adaptation
void timer(void)
{
	//	Set old time for iteration
	oldtime = time;
	
	// Get System Clock Value
	unsigned int SysClock = (*(volatile unsigned int*)0x6000324);
	
	
	FIXED time_add =
		toFIXED(
		//	Clock speed from framerate based on resolution register and NSTC vs PAL
		(((*(Uint16 *)0x25f80004 & 0x1) == 0x1) ?
			((SysClock == 0) ? (float)0.037470726 : (float)0.035164835 ) :
			((SysClock == 0) ? (float)0.037210548 : (float)0.03492059 ))
			*
		//	Get Free Running Counter value
		((*(volatile unsigned char *)SH2FRCHigh) << 8 | (*(volatile unsigned char *)SH2FRCLow))
		*
		//	Determine if clock is on 1/8, 1/32, or 1/128 of count
		(8 << (((*(volatile unsigned char *)RegisterTCR) & 3) << 1)) /
		//	Set to s
		1000000);

	//	Set new time
	time += time_add;
	//time = jo_get_ticks();
	
	//	Reset FRC's
	(*(volatile unsigned char *)SH2FRCHigh) = 0 >> 8;
	(*(volatile unsigned char *)SH2FRCLow) = 0;
}
