/**XL2 Animation Interpolation system with ANORM.h lookup table**/

#ifndef __ANIMATE_H__
#define __ANIMATE_H__

#include <jo/jo.h>
#include "ZT/ZT_COMMON.h"
#include "game_object.h"

#define ANIM_CONST (8)
//	Maximum number of animations supported by a model
#define MAX_ANI (50)		

typedef struct
{
    Bool uniform;
    Uint8 arate[256];
    Uint16 currentFrm;
    Uint8 currentKeyFrm;
    Uint8 startFrm;
    Uint8 endFrm;
} animationControl;

//	Animation master list, row = entity, col = animation
extern animationControl animationMaster[MAX_MODELS][MAX_ANI];
/*
	0	-	Idle
	1	-	Walk
	2	-	Run
*/

//	Framerate definitions (different integer types for different context)
extern Sint8 SynchConst;
Sint32 framerate;
/****/

void display_animated_model(game_object *obj, int ani_ID);


#endif
