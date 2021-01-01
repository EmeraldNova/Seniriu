/*
** Seniriu
** Matthew Suttinger & Emerald Nova (emeraldnovagames@gmail.com)
** 
** This work is licensed under a Attribution-NonCommercial 4.0 International License
** More info at: https://creativecommons.org/licenses/by-nc/4.0/legalcode
*/

#ifndef __LOADING_H__
#define __LOADING_H__

//	Include
#include <jo/jo.h>
#include "ZT/ZT_COMMON.h"


//	Definitions
#define NUM_FILES	(32)
#define     OPEN_MAX    (Sint32)5
#define     DIR_MAX     (Sint32)25

//	Variables
//	Start index of interior room models
extern unsigned int inter_start;
//	Start index of exterior room models
extern unsigned int exter_start;
//	Start index of item models
extern unsigned int item_start;
//	Start index of enemy models
extern unsigned int enemy_start;
//	Start index of meshed enemy models
extern unsigned int enemy_meshed_start;
//	Final loading index
extern unsigned int model_load_end;
//	List of models to load
extern const Sint8 * filenames[NUM_FILES];
//	Track loading addresses in LWRAM
extern void * currentAddress;
//	XL2 added variables
extern GfsDirTbl gfsDirTbl;
extern GfsDirName gfsDirName[DIR_MAX];
extern Uint32 gfsLibWork[GFS_WORK_SIZE(OPEN_MAX)/sizeof(Uint32)];
extern Sint32 gfsDirN;


//	Structs


//	Functions
void load_models(void);

#endif
