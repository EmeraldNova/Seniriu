/**XL2 Animation Interpolation system with ANORM.h lookup table**/

//	Include
#include "animate.h"
//	Any attempt to sanitize this include will break the game. Likely due to a lack of ANORM.c
#include "ZT/ANORM.h"


//Sint8 SynchConst;
Sint32 framerate;


//	Functions
void display_animated(int ent_ID, animationControl * ani_con)
{
	/*
		Display Animated Model
		
		Handles animations by keyframe/frame. INput entity ID and pointer to animation controller
	*/
	
	//	Master entity pointer
	entity_t *currentModel = &entities[ent_ID];
	//	pData pointer
	XPDATA *currentPDATA = (XPDATA*)entities[ent_ID].pol[0];
	
	//	Animation sequence to use
	Uint8 anidex = ani_con->currentAni;
	Uint8 num_frames = ani_con->last_frame[anidex];
	
	//	Curent frame in sequence in animation_sequence
	(ani_con->currentFrm)++;
	if(ani_con->currentFrm > num_frames)
	{
		ani_con->currentFrm = 0;
	}
	
	//	Set keyframe
	Uint8 currentKeyFrm = ani_con->frames[anidex][ani_con->currentFrm];
	
	//	Set get compressed vertex pointers for current and next frames
	compVert *curFrameCV = 
		(compVert*)currentModel->animation[currentKeyFrm]->cVert;
	
	//	Indexing register per quad
	register Uint32    i;
	
	//	Pointers to point table and keyframes
	Sint32 *dst = currentPDATA->pntbl[0];
	Sint16 *src = curFrameCV[0];
	
	//	Write animation verts to current PDATA
	for (i = 0; i < currentPDATA->nbPoint*sizeof(POINT); i += sizeof(int))
	{
		*dst++ = (*src)<<8;
		*src++;
	}
	
	//	Take normal lookup table and apply it
	//	Current PDATA's normals
	*dst = currentPDATA->pltbl[0].norm[0];
	//	Normals for current animation frame
	Uint8 *src2 = currentModel->animation[currentKeyFrm]->cNorm;
	
	//	Apply normals to quads
	for (i = 0; i < currentPDATA->nbPolygon; i++)
	{
		//	Normal XYZ
		*dst++ = ANORMS[*src2][X];
		*dst++ = ANORMS[*src2][Y];
		*dst++ = ANORMS[*src2++][Z];
		
		//	Skip over 16 bit vertex IDs
		*dst++;
		*dst++;
	}
	
	//	Draw command
	slPutPolygon((PDATA*)currentPDATA);
	
	return;
}


