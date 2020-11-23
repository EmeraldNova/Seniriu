/**XL2 Animation Interpolation system with ANORM.h lookup table**/

#include "animate.h"
//	Any attempt to sanitize this include will break the game. Likely due to a lack of ANORM.c
#include "ZT/ANORM.h"

Sint32 framerate;

//	Animation master list, row = entity, col = animation
animationControl animationMaster[MAX_MODELS][MAX_ANI];

void display_animated_model(game_object *obj, int ani_ID)
{
	//	Master entity pointer
	//ani_ID = 0;
	entity_t *currentModel = &entities[obj->entity_ID];
	//	pData pointer
	XPDATA *currentPDATA = obj->pDataStart;
	//	Animation control pointer
	animationControl *animCtrl = &animationMaster[obj->entity_ID][ani_ID];
	
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
	//XPDATA *currentPDATA = pol[0];
	 
	/**Sets the animation data**/
	///Variable interpolation set
	//if(animCtrl->uniform == false)
	if(animationMaster[obj->entity_ID][ani_ID].uniform == false)
	{
		
		//localArate = animCtrl->arate[animCtrl->currentKeyFrm];
		localArate = 
			animationMaster[obj->entity_ID][ani_ID].arate[animationMaster[obj->entity_ID][ani_ID].currentKeyFrm];
	}
	else
	{
		//	Animation rate set, Identical to SGL framerate number
		//	FPS = 60/loclArate
		localArate = 2;
	}
	
	//	Advance current frame according to framrate and animation rate
	//animCtrl->currentFrm += (localArate * framerate)>>1;
	obj->currentFrm += (localArate * framerate)>>1;
	//	Division by 8 is a magic number for a compression ratio for the animation
	//animCtrl->currentKeyFrm = (animCtrl->currentFrm>>3);
	obj->currentKeyFrm = (obj->currentFrm>>3);
	
	//	Check if animation is odone
	//if (animCtrl->currentKeyFrm >= animCtrl->endFrm)
	if (obj->currentKeyFrm >= animationMaster[obj->entity_ID][ani_ID].endFrm)
	{
		//	Frame advanced past endframe, loop back to start frame and continue progress
		//animCtrl->currentFrm -= (animCtrl->endFrm - animCtrl->startFrm)<<3;
		obj->currentFrm -=
			(animationMaster[obj->entity_ID][ani_ID].endFrm - animationMaster[obj->entity_ID][ani_ID].startFrm)<<3;
		//animCtrl->currentKeyFrm = animCtrl->currentFrm>>3;
		obj->currentKeyFrm = obj->currentFrm>>3;
	}
	//else if(animCtrl->currentKeyFrm < animCtrl->startFrm)
	else if(obj->currentKeyFrm < animationMaster[obj->entity_ID][ani_ID].startFrm)
	{
		//	Curent frame is before animation start, move up to animation start
		//animCtrl->currentKeyFrm = animCtrl->startFrm;
		obj->currentKeyFrm = 
			animationMaster[obj->entity_ID][ani_ID].startFrm;
		//animCtrl->currentFrm += (animCtrl->endFrm-animCtrl->startFrm)<<3;
		obj->currentFrm += 
			(animationMaster[obj->entity_ID][ani_ID].endFrm-animationMaster[obj->entity_ID][ani_ID].startFrm)<<3;
	}
	
	//	Advance the key frame
	//Uint8 nextKeyFrm = animCtrl->currentKeyFrm+1;
	Uint8 nextKeyFrm = obj->currentKeyFrm+1;
	
	
	//if (nextKeyFrm >= animCtrl->endFrm)
	if (nextKeyFrm >= animationMaster[obj->entity_ID][ani_ID].endFrm)
	{
		//	Past end of animation, reset to animation start
		//nextKeyFrm = animCtrl->startFrm;
		nextKeyFrm = animationMaster[obj->entity_ID][ani_ID].startFrm;
	}
	//else if (nextKeyFrm <= animCtrl->startFrm)
	else if (nextKeyFrm <= animationMaster[obj->entity_ID][ani_ID].startFrm)
	{
		//	Before start frame, reset to just after start frame
		//nextKeyFrm = animCtrl->startFrm+1;
		nextKeyFrm = animationMaster[obj->entity_ID][ani_ID].startFrm+1;
	}
	
	//	Set get compressed vertex pointers for current and next frames
	//compVert *curKeyFrame = (compVert*)currentModel->animation[animCtrl->currentKeyFrm]->cVert;
	compVert *curKeyFrame = 
		(compVert*)currentModel->animation[obj->currentKeyFrm]->cVert;
	compVert *nextKeyFrame = (compVert*)currentModel->animation[nextKeyFrm]->cVert;
	
	///Don't touch this!
	//Uint32 compHelp = (animCtrl->currentFrm)-(animCtrl->currentKeyFrm<<3);
	Uint32 compHelp = (obj->currentFrm)-(obj->currentKeyFrm<<3);
	  
	/**Uncompress the vertices and apply linear interpolation**/
	register Uint32    i;
	//	Pointer for an 32 bit integer we're about to write, changes with use
	Sint32 *dst = currentPDATA->pntbl[0];
	Sint16 *src = curKeyFrame[0];
	Sint16 *nxt = nextKeyFrame[0];
	
	///Decompression, loop through every point, convert int to fixed
	for (i = 0; i < currentPDATA->nbPoint*sizeof(POINT); i += sizeof(int))
	{
		*dst++ = (*src+(((*nxt-*src)*compHelp)>>ANIM_QUAL))<<8;
		*src++;
		*nxt++;
	}
	
	//	Take normal lookup table and apply it
	*dst = currentPDATA->pltbl[0].norm[0];
	//Uint8 *src2 = currentModel->animation[animCtrl->currentKeyFrm]->cNorm;
	Uint8 *src2 = currentModel->animation[obj->currentKeyFrm]->cNorm;
	
	//	Interpolation of normals
	for (i = 0; i < currentPDATA->nbPolygon; i++)    {

		*dst++ = ANORMS[*src2][X];
		*dst++ = ANORMS[*src2][Y];
		*dst++ = ANORMS[*src2++][Z];
		
		//	Skip over 16 buit vertex IDs
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
