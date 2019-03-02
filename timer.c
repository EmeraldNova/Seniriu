/*
** Seniriu
** Matthew Suttinger & Emerald Nova ()
** 
** This work is licensed under a Attribution-NonCommercial 4.0 International License
** More info at: https://creativecommons.org/licenses/by-nc/4.0/legalcode
*/

#include "timer.h"
#include <jo/jo.h>

//	Poitner to MCU High Frequency update timer, found by Johannes Fetz, contributed by Ponut64
volatile Uint8 * mcuFrcHigh = (volatile Uint8 *)4294966802;
FIXED time = toFIXED(0.0);
FIXED oldtime = toFIXED(0.0);

//	Get high frequency count from chip and translate to seconds
void poll_HighFreq(void)
{
	oldtime = time;
	//	7680 = 30 (correction factor) * 256 (Uint8 range)
	time += slDivFX(toFIXED(7680.0),toFIXED(*mcuFrcHigh));
}
