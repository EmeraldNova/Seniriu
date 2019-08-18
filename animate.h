/**XL2 Animation Interpolation system with ANORM.h lookup table**/

#ifndef __ANIMATE_H__
#define __ANIMATE_H__

#include <jo/jo.h>
#include "ZT/ZT_COMMON.h"

#define ANIM_CONST (8)

typedef struct
{
    Bool uniform;
    Uint8 arate[256];
    Uint16 currentFrm;
    Uint8 currentKeyFrm;
    Uint8 startFrm;
    Uint8 endFrm;
} animationControl;

//	Jelly animation
animationControl jelly_neutral;
animationControl jelly_damage;

//	Framerate definitions (different integer types for different context)
extern Sint8 SynchConst;
Sint32 framerate;
/****/

void display_animated_model(animationControl * animCtrl, entity_t * currentModel);


#endif
