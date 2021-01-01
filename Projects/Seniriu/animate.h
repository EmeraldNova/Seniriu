/**XL2 Animation Interpolation system with ANORM.h lookup table**/

#ifndef __ANIMATE_H__
#define __ANIMATE_H__

//	Includes
#include <jo/jo.h>
#include "ZT/ZT_COMMON.h"

//	Definitions
//	Constant used in animation calculation
#define ANIM_CONST (8)
//	Maximum number of animations supported by a model
#define MAX_ANI (10)
//	Maximum number of frames in an animation
#define MAX_FRAMES (128)

//	Structs
typedef struct
{
	/*
		Animation Control Struct
	*/
	
	//	Gives total number of frames in an animation
	Uint8 last_frame[MAX_ANI];
	//	Gives keyframe sequence for animation
    Uint8 frames[MAX_ANI][MAX_FRAMES];
	/*
		Animation index list
		0	-	Idle
		1	-	Walk
		2	-	Run
		3	-	Attack0
		4	-	Attack1
		5	-	Attack2
		6	-	
		7	-
		8	-
		9	-
	*/
	
	//	Current animation index
	Uint8 currentAni;
	//	Current frame index
    Uint8 currentFrm;
	//	Current keyframe
    Uint8 currentKeyFrm;
	
} animationControl;

//	Framerate definitions (different integer types for different context)
extern Sint8 SynchConst;
extern Sint32 framerate;


//	Functions
void display_animated(int ent_ID, animationControl * ani_con);

#endif
