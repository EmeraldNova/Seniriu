/**XL2 Animation Interpolation system with ANORM.h lookup table**/

#include "animate.h"
#include "ZT/ANORM.h"

void display_animated_model(animationControl * animCtrl, entity_t * currentModel)
{
	//	Returns if there's only 1 mesh
	if(currentModel->nbMeshes < 1)
	{
		return;
	}
	
	//	Animation quality
	static Uint8 ANIM_QUAL = 4;
	//	Local Animation Rate
	Uint8 localArate;
	//	Pointer to current model's first polygon
	XPDATA *currentPDATA = currentModel->pol[0];
	 
	/**Sets the animation data**/
	///Variable interpolation set
	if(animCtrl->uniform == false)
	{
		
		localArate = animCtrl->arate[animCtrl->currentKeyFrm];
	}
	else
	{
		//	Animation rate set, Identical to SGL framerate number
		//	FPS = 60/loclArate
		localArate = 2;
	}
	
	//	Advance current frame according to framrate and animation rate
	animCtrl->currentFrm += (localArate * framerate)>>1;
	//	Division by 8 is a magic number for a compression ratio for the animation
	animCtrl->currentKeyFrm = (animCtrl->currentFrm>>3);
	
	//	Check if animation is odone
	if (animCtrl->currentKeyFrm >= animCtrl->endFrm)
	{
		//	Frame advanced past endframe, loop back to start frame and continue progress
		animCtrl->currentFrm -= (animCtrl->endFrm - animCtrl->startFrm)<<3;
		animCtrl->currentKeyFrm = animCtrl->currentFrm>>3;
	}
	else if(animCtrl->currentKeyFrm < animCtrl->startFrm)
	{
		//	Curent frame is before animation start, move up to animation start
		animCtrl->currentKeyFrm = animCtrl->startFrm;
		animCtrl->currentFrm += (animCtrl->endFrm-animCtrl->startFrm)<<3;
	}
	
	//	Advance the key frame
	Uint8 nextKeyFrm = animCtrl->currentKeyFrm+1;
	
	
	if (nextKeyFrm >= animCtrl->endFrm)
	{
		//	Past end of animation, reset to animation start
		nextKeyFrm = animCtrl->startFrm;
	}
	else if (nextKeyFrm <= animCtrl->startFrm)
	{
		//	Before start frame, reset to just after start frame
		nextKeyFrm = animCtrl->startFrm+1;
	}
	
	//	Set get compressed vertex pointers for current and next frames
	compVert *curKeyFrame = (compVert*)currentModel->animation[animCtrl->currentKeyFrm]->cVert;
	compVert *nextKeyFrame = (compVert*)currentModel->animation[nextKeyFrm]->cVert;
	
	///Don't touch this!
	Uint32 compHelp = (animCtrl->currentFrm)-(animCtrl->currentKeyFrm<<3);
	  
	/**Uncompress the vertices and apply linear interpolation**/
	register Uint32    i;
	//	Pointer for an 32 bit integer we're about to write, changes with use
	Sint32 *dst=currentPDATA->pntbl[0];
	Sint16 *src=curKeyFrame[0];
	Sint16 *nxt=nextKeyFrame[0];
	
	///Decompression, loop through every point, convert int to fixed
	for (i = 0; i < currentPDATA->nbPoint*sizeof(POINT); i += sizeof(int))
	{
		*dst++ = (*src+(((*nxt-*src)*compHelp)>>ANIM_QUAL))<<8;
		*src++;
		*nxt++;
	}
	
	//	Take normal lookup table and apply it
	*dst = currentPDATA->pltbl[0].norm[0];
	Uint8 *src2 = currentModel->animation[animCtrl->currentKeyFrm]->cNorm;
	
	//	Interpolation of normals
	for (i = 0; i < currentPDATA->nbPolygon; i++)    {

		*dst++=ANORMS[*src2][X];
		*dst++=ANORMS[*src2][Y];
		*dst++=ANORMS[*src2++][Z];
		*dst++;
		*dst++;
	}

	//	Draw command
	slPutPolygon((PDATA*)currentPDATA);
}


/*
//	Definition of an animation example

run.uniform = false;
run.arate[17] = 2;
run.arate[18] = 3;
run.arate[19] = 4;
run.arate[20] = 4;
run.arate[21] = 2;
run.arate[22] = 3;
run.arate[23] = 4;
run.arate[24] = 4;
run.startFrm = 17;
run.currentFrm = run.startFrm  * ANIM_CONST;
run.endFrm=25;

stop.uniform = false;
stop.arate[38] = 0;
stop.startFrm = 38;
stop.currentFrm = stop.startFrm * ANIM_CONST;
stop.endFrm = 38;
*/
