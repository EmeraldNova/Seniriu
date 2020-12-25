/*
** Seniriu
** Matthew Suttinger & Emerald Nova (emeraldnovagames@gmail.com)
** 
** This work is licensed under a Attribution-NonCommercial 4.0 International License
** More info at: https://creativecommons.org/licenses/by-nc/4.0/legalcode
*/

#include "display.h"

//	Variables
//	(full) Field of View
ANGLE FOV = DEGtoANG(90.0);

//	Depth shading table to make things darker with distance
static Uint16 DepthData[32] = {
    GRTBL( 16, 16, 16 ),
    GRTBL( 16, 16, 16 ),
    GRTBL( 16, 16, 16 ),
    GRTBL( 16, 16, 16 ),
    GRTBL( 16, 16, 16 ),
    GRTBL( 16, 16, 16 ),
    GRTBL( 16, 16, 16 ),
    GRTBL( 16, 16, 16 ),
    GRTBL( 16, 16, 16 ),
    GRTBL( 16, 16, 16 ),
    GRTBL( 16, 16, 16 ),
    GRTBL( 16, 16, 16 ),
    GRTBL( 16, 16, 16 ),
    GRTBL( 16, 16, 16 ),
    GRTBL( 16, 16, 16 ),
    GRTBL( 16, 16, 16 ),
    GRTBL( 15, 15, 15 ),
    GRTBL( 14, 14, 14 ),
    GRTBL( 13, 13, 13 ),
    GRTBL( 12, 12, 12 ),
    GRTBL( 11, 11, 11 ),
    GRTBL( 10, 10, 10 ),
    GRTBL(  9,  9,  9 ),
    GRTBL(  8,  8,  8 ),
    GRTBL(  7,  7,  7 ),
    GRTBL(  6,  6,  6 ),
    GRTBL(  5,  5,  5 ),
    GRTBL(  4,  4,  4 ),
    GRTBL(  3,  3,  3 ),
    GRTBL(  2,  2,  2 ),
    GRTBL(  1,  1,  1 ),
    GRTBL(  0,  0,  0 ),
};

