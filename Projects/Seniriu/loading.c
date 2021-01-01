/*
** Seniriu
** Matthew Suttinger & Emerald Nova (emeraldnovagames@gmail.com)
** 
** This work is licensed under a Attribution-NonCommercial 4.0 International License
** More info at: https://creativecommons.org/licenses/by-nc/4.0/legalcode
*/


//	Include
#include "loading.h"


//	Variables
//	Start index of interior room models
unsigned int inter_start = 0;
//	Start index of exterior room models
unsigned int exter_start = 5;
//	Start index of item models
unsigned int item_start = 8;
//	Start index of enemy models
unsigned int enemy_start = 12;
//	Start index of meshed enemy models
unsigned int enemy_meshed_start = 15;
//	Final loading index
unsigned int model_load_end = 15;
//	List of models to load
const Sint8 * filenames[NUM_FILES] = 
{
	//	0 - 4:		Room Components
	"FLOOR.ZTP",	//	Floor
	"CEIL.ZTP",		//	Ceiling
	"CORNER.ZTP",	//	Wall Corner
	"DOOR.ZTP",		//	Door
	"WNDW0.ZTP",	//	Window
	//	5 - 7:		Exterior Components
	"HULL0.ZTP",		//	Hull Wall
	"HLEX0.ZTP",		//	Hull Exterior Angle
	"HLIN0.ZTP",		//	Hull Interior Angle
	//	8 - 11:		Items
	"SABR0.ZTP",
	"SABR0.ZTP",
	"PHSR0.ZTP",
	"PHSR0.ZTP",
	//	12 - 14:	Enemies
	"UFO00.ZTP",
	"EYE00.ZTP",
	"PUFF0.ZTP",
	//	15 - 15:	Meshed Enemies
	"BLOBB.ZTP"
};


//	Functions
void load_models(void)
{
	/*
		Load Models
		
		Loads 3D models
	*/
	
	//	Start loading in LWRAM beginning
    void * currentAddress = (void*)LWRAM;
	
	
	//	Load ZTP file
	for(int ent_ID = 0; ent_ID <= model_load_end; ent_ID++)
	{
		currentAddress = ztLoad3Dmodel((Sint8*)filenames[ent_ID], currentAddress, ent_ID, false);
		
		//	Rectify vertex directions
		//	Number of quads in model
		int num_quads = entities[ent_ID].pol[0]->nbPolygon;
		//	Number of vertices in model
		int num_verts = entities[ent_ID].pol[0]->nbPoint;
		//	Pointer to point table
		POINT * pntbl_ptr = (entities[ent_ID].pol[0])->pntbl;
		//	Pointer to Polygon table
		POLYGON * pltbl_ptr = (entities[ent_ID].pol[0])->pltbl;
		
		//	Normal Components Absolute Value
		int norm_copy_abs[XYZ];
		//	Maximum absolute value of normal components
		int max_component = 0;
		
		//	Start loop
		for (int i = 0; i < num_quads; i++)
		{
			//	Assign vertices
			Uint16 * const vert_ID = pltbl_ptr[i].Vertices;
			
			//	Normal Components
			FIXED * const norm_copy = pltbl_ptr[i].norm;
			
			//	Number of permutations needed to sort
			int perm_num = 0;
			
			//	Find chief axis
			norm_copy_abs[X] = JO_ABS(norm_copy[X]);
			norm_copy_abs[Y] = JO_ABS(norm_copy[Y]);
			norm_copy_abs[Z] = JO_ABS(norm_copy[Z]);
			max_component = JO_MAX(JO_MAX(norm_copy_abs[X], norm_copy_abs[Y]), norm_copy_abs[Z]);
			
			//	Align directions
			int left_dir[XYZ] = {0, 0, 0};
			int up_dir[XYZ] = {0, 0, 0};
			
			//	Disable lighting
			entities[ent_ID].pol[0]->attbl[i].sort &= ~UseLight;
			
			//	Clear Sort
			entities[ent_ID].pol[0]->attbl[i].sort &= ~3;
			//	Max sort default
			entities[ent_ID].pol[0]->attbl[i].sort |= SORT_MAX;
			
			//	Special flags for select models
			if(ent_ID >= item_start )
			{
				//	Activate mesh on specific entity
				if( ent_ID >= enemy_meshed_start)
				{
					entities[ent_ID].pol[0]->attbl[i].atrb |= MESHon;
				}
				
				//	Change Z sort
				entities[ent_ID].pol[0]->attbl[i].sort &= ~3;
				entities[ent_ID].pol[0]->attbl[i].sort |= SORT_MIN;
			}
			
			//	Determine Chief Normal
			int norm;
			if((max_component == norm_copy_abs[X]) &&
				(norm_copy[X] < 0))
			{
				//	-X; Z left, -Y up
				norm = -1;
				left_dir[Z] = 1;
				up_dir[Y] = -1;
				
				//	Set aligned textures to mesh display
				//entities[ent_ID].pol[0]->attbl[i].atrb |= MESHon;
			}
			else if((max_component == norm_copy_abs[X]) &&
					(norm_copy[X] > 0))
			{
				//	X; -Z left, -Y up
				norm = 1;
				left_dir[Z] = -1;
				up_dir[Y] = -1;
			}
			else if((max_component == norm_copy_abs[Y]) &&
					(norm_copy[Y] > 0))
			{
				//	Y; -X left, -Z up
				norm = 2;
				left_dir[X] = -1;
				up_dir[Z] = -1;
			}
			else if((max_component == norm_copy_abs[Z]) &&
					(norm_copy[Z] < 0))
			{
				//	-Z; -X left, -Y up
				norm = -3;
				left_dir[X] = -1;
				up_dir[Y] = -1;
				
			}
			else if((max_component == norm_copy_abs[Z]) &&
					(norm_copy[Z] > 0))
			{
				//	Z; X left, -Y up
				norm = 3;
				left_dir[X] = 1;
				up_dir[Y] = -1;
			}
			else
			{
				//	-Y (Default); -X left, Z up
				norm = -2;
				left_dir[X] = -1;
				up_dir[Z] = 1;
				
				
			}
			
			//	Retrieve two most "Up" points
			FIXED dif[XYZ];
			FIXED difs[4];
			for(int i = 0; i < 4; i++)
			{
				//	Calculate difference
				dif[X] = pntbl_ptr[vert_ID[i]][X] -
						   pntbl_ptr[vert_ID[0]][X];
				dif[Y] = pntbl_ptr[vert_ID[i]][Y] -
						   pntbl_ptr[vert_ID[0]][Y];
				dif[Z] = pntbl_ptr[vert_ID[i]][Z] -
						   pntbl_ptr[vert_ID[0]][Z];
				
				//	Dot product with up direction
				difs[i] = up_dir[X]*dif[X] +
						  up_dir[Y]*dif[Y] +
						  up_dir[Z]*dif[Z];
			}
			//	Sort by most "Up"
			int up_index[2] = {0, 1};
			int hold_dex;
			for(int i = 1; i < 4; i++)
			{
				if(difs[i] > difs[up_index[1]])
				{
					//	Overwrite second highest
					up_index[1] = i;
				}
				if(difs[up_index[1]] > difs[up_index[0]])
				{
					//	Sort the top 2
					hold_dex = up_index[1];
					up_index[1] = up_index[0];
					up_index[0] = hold_dex;
				}
			}
			
			//	Find most "Left" of the two most "Up" verts
			//	Calculate difference
			dif[X] = pntbl_ptr[vert_ID[up_index[0]]][X] -
					   pntbl_ptr[vert_ID[up_index[1]]][X];
			dif[Y] = pntbl_ptr[vert_ID[up_index[0]]][Y] -
					   pntbl_ptr[vert_ID[up_index[1]]][Y];
			dif[Z] = pntbl_ptr[vert_ID[up_index[0]]][Z] -
					   pntbl_ptr[vert_ID[up_index[1]]][Z];
			//	Dot product with left direction
			difs[0] = left_dir[X]*dif[X] +
					  left_dir[Y]*dif[Y] +
					  left_dir[Z]*dif[Z];
					  
			//	Check for which index is most left
			if(difs[0] >= 0)
			{
				perm_num = up_index[0];
			}
			else
			{
				perm_num = up_index[1];
			}
			
			//	Temporary permutation vertex variable
			Uint16 * tmpVert;
			
			//	Permute (rotate) vertex indices until reoriented
			for(int j = 0; j < perm_num; j++)
			{
				tmpVert = pltbl_ptr[i].Vertices[0];
				pltbl_ptr[i].Vertices[0] = pltbl_ptr[i].Vertices[1];
				pltbl_ptr[i].Vertices[1] = pltbl_ptr[i].Vertices[2];
				pltbl_ptr[i].Vertices[2] = pltbl_ptr[i].Vertices[3];
				pltbl_ptr[i].Vertices[3] = tmpVert;
			}
		}
	}
	
	return;
}


