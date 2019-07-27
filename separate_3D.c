/*
** Seniriu
** Matthew Suttinger & Emerald Nova (emeraldnovagames@gmail.com)
** 
** This work is licensed under a Attribution-NonCommercial 4.0 International License
** More info at: https://creativecommons.org/licenses/by-nc/4.0/legalcode
*/

#include "separate_3D.h"

//	Collection of bounding boxes
b_box boxes[200];

//	Player bounding box, width 0.9
b_box player_bbox;

//	Cross Product: V3 = V1 X V2 = - V2 X V1
void x_product(FIXED V1[XYZ], FIXED V2[XYZ], FIXED V3[XYZ])
{
	V3[X] = slMulFX(V1[Y],V2[Z]) - slMulFX(V1[Z], V2[Y]);
    V3[Y] = -slMulFX(V1[X],V2[Z]) + slMulFX(V1[Z], V2[X]);
    V3[Z] = slMulFX(V1[X],V2[Y]) - slMulFX(V1[Y], V2[X]);
}

//	Resets rotation on corners given dimensions of bounding box
void reset_corners(b_box *box)
{
	//	Reset Corners Before Rotation
	box->corners[0][0] = -box->half[X];
	box->corners[0][1] = -box->half[Y];
	box->corners[0][2] = -box->half[Z];
	box->corners[1][0] = -box->half[X];
	box->corners[1][1] = -box->half[Y];
	box->corners[1][2] = box->half[Z];
	box->corners[2][0] = -box->half[X];
	box->corners[2][1] = box->half[Y];
	box->corners[2][2] = -box->half[Z];
	box->corners[3][0] = -box->half[X];
	box->corners[3][1] = box->half[Y];
	box->corners[3][2] = box->half[Z];
	box->corners[4][0] = box->half[X];
	box->corners[4][1] = -box->half[Y];
	box->corners[4][2] = -box->half[Z];
	box->corners[5][0] = box->half[X];
	box->corners[5][1] = -box->half[Y];
	box->corners[5][2] = box->half[Z];
	box->corners[6][0] = box->half[X];
	box->corners[6][1] = box->half[Y];
	box->corners[6][2] = -box->half[Z];
	box->corners[7][0] = box->half[X];
	box->corners[7][1] = box->half[Y];
	box->corners[7][2] = box->half[Z];
	
	//	Rotate
	//	To be added
	
	//	Calculate offsets based on game_object position, bounding box center offset
	FIXED x_offset = box->center[X] + object[(box->parent_ID)].position[X];
	FIXED y_offset = box->center[Y] + object[(box->parent_ID)].position[Y];
	FIXED z_offset = box->center[Z] + object[(box->parent_ID)].position[Z];
	
	//	Offset
	for(int i = 0; i <= 8; i++)
	{
		box->corners[i][0] = box->corners[i][0] + x_offset; 
		box->corners[i][1] = box->corners[i][1] + y_offset;
		box->corners[i][2] = box->corners[i][2] + z_offset;
	}
}

//	Sets up a bounding box given pointer to initialized one
void create_bbox(b_box *box, FIXED center[XYZ], FIXED x_dim, FIXED y_dim, FIXED z_dim, int parent_ID)
{
	//	Set center
	box->center[X] = center[X];
	box->center[Y] = center[Y];
	box->center[Z] = center[Z];
	
	//	Half dimensions
	box->half[X] = slMulFX(x_dim,32768);
	box->half[Y] = slMulFX(y_dim,32768);
	box->half[Z] = slMulFX(z_dim,32768);
	
	//	Set parent
	box->parent_ID = parent_ID;
	
	//	Set corners with half dimensions
	reset_corners(box);
}

//	Projecting 2D points onto line defined by scaled normal
void project_1D_bbox(b_box *box, FIXED scaled[XYZ], FIXED *extent)
{
	//	Writes an extent that described minimum and maximum extent of
	//	projection of bounding box corners onto 1D line. Overlapping extents
	//	will determine collision
	
	//	Calculate corners for boundign box
	reset_corners(box);
	
	//	Intialize value with dot product
	FIXED value = slMulFX(scaled[X], box->corners[0][0]) +
			slMulFX(scaled[Y], box->corners[0][1]) +
			slMulFX(scaled[Z], box->corners[0][2]);
			
	//	Initialzie extent
	*(extent) = value;
	*(extent + 1) = value;
	
	//	Add points to extent
	for(int i = 1; i < 8; i++)
	{
		value = slMulFX(scaled[X], box->corners[i][0]) +
			slMulFX(scaled[Y], box->corners[i][1]) +
			slMulFX(scaled[Z], box->corners[i][2]);
				
		//	Check for max or min
		if(value < *(extent))
		{
			//	Write min
			*(extent) = value;
		}
		else if(value > *(extent + 1))
		{
			*(extent + 1) = value;
		}
	}
}


//	Checks for separating axis from two bounding boxes. True if colliding.
bool separate_3D_bbox(b_box *box1, b_box *box2)
{
	//	Projection vectors
	VECTOR normal[15];
	VECTOR scaled[15];
	//	Extents to test against for collision
	FIXED extent1[2];
	FIXED extent2[2];
	//	Rotation Matrices
	//FIXED R1[3][3];
	//FIXED R2[3][3];
	MATRIX R1;
	MATRIX R2;
	
	//	Construct base normals
	//	X1
	normal[0][0] = 1 << 16;
	normal[0][1] = 0;
	normal[0][2] = 0;
	//	Y1
	normal[1][0] = 0;
	normal[1][1] = 1 << 16;
	normal[1][2] = 0;
	//	Z1
	normal[2][0] = 0;
	normal[2][1] = 0;
	normal[2][2] = 1 << 16;
	//	X2
	normal[3][0] = 1 << 16;
	normal[3][1] = 0;
	normal[3][2] = 0;
	//	Y2
	normal[4][0] = 0;
	normal[4][1] = 1 << 16;
	normal[4][2] = 0;
	//	Z2
	normal[5][0] = 0;
	normal[5][1] = 0;
	normal[5][2] = 1 << 16;
	
	/*
	//	Rotation Angles Box 1
	ANGLE RX1 = object[box1->parent_ID].theta[X];
	ANGLE RY1 = object[box1->parent_ID].theta[Y];
	ANGLE RZ1 = object[box1->parent_ID].theta[Z];
	//	Rotation Angles Box 2
	ANGLE RX2 = object[box2->parent_ID].theta[X];
	ANGLE RY2 = object[box2->parent_ID].theta[Y];
	ANGLE RZ2 = object[box2->parent_ID].theta[Z];
	
	//	Rotation Matrix 1
	slPushUnitMatrix();
	slRotX(RX1);
	slRotY(RY1);
	slRotZ(RZ1);
	slGetMatrix(R1);
	slPopMatrix();
	
	//	Rotation Matrix 2
	slPushUnitMatrix();
	slRotX(RX2);
	slRotY(RY2);
	slRotZ(RZ2);
	slGetMatrix(R2);
	slPopMatrix();
	*/
	
	//	Calculate Rotation Matrix Box 1
	/*
	R1[0][0] = 32768*(slCos(RY1 + RZ1) + slCos(RY1 - RZ1));
	R1[0][1] = -32768*(slSin(RZ1 + RX1) + slSin(RZ1 - RX1)) + 
				16384*(slCos(RX1-RY1-RZ1) - slCos(RX1+RY1-RZ1) + 
						slCos(RX1-RY1+RZ1) - slCos(RX1+RY1+RZ1));
	R1[0][2] = -32768*(slCos(RX1 - RZ1) - slCos(RX1 + RZ1)) + 
				16384*(-slSin(RX1-RY1-RZ1) + slSin(RX1+RY1-RZ1) - 
						slSin(RX1-RY1+RZ1) + slSin(RX1+RY1+RZ1));
	R1[1][0] = 32768*(slSin(RZ1 + RY1) + slSin(RZ1 - RY1));
	R1[1][1] = 32768*(slCos(RX1 + RZ1) + slCos(RX1 - RZ1)) + 
				16384*(-slSin(RX1-RY1-RZ1) + slSin(RX1+RY1-RZ1) + 
						slSin(RX1-RY1+RZ1) - slSin(RX1+RY1+RZ1));
	R1[1][2] = -32768*(slSin(RX1 + RZ1) + slSin(RX1 - RZ1)) + 
				16384*(-slCos(RX1-RY1-RZ1) + slCos(RX1+RY1-RZ1) + 
						slCos(RX1-RY1+RZ1) - slCos(RX1+RY1+RZ1));
	R1[2][0] = -slSin(RY1);
	R1[2][1] = 32768*(slSin(RX1 + RY1) + slSin(RX1 - RY1));
	R1[2][2] = 32768*(slCos(RX1 + RY1) + slCos(RX1 - RY1));
	*/
	
	//	Calculate Rotation Matrix Box 2
	/*
	R1[0][0] = 32768*(slCos(RY2 + RZ2) + slCos(RY2 - RZ2));
	R2[0][1] = -32768*(slSin(RZ2 + RX2) + slSin(RZ2 - RX2)) + 
				26384*(slCos(RX2-RY2-RZ2) - slCos(RX2+RY2-RZ2) + 
						slCos(RX2-RY2+RZ2) - slCos(RX2+RY2+RZ2));
	R2[0][2] = -32768*(slCos(RX2 - RZ2) - slCos(RX2 + RZ2)) + 
				26384*(-slSin(RX2-RY2-RZ2) + slSin(RX2+RY2-RZ2) - 
						slSin(RX2-RY2+RZ2) + slSin(RX2+RY2+RZ2));
	R2[1][0] = 32768*(slSin(RZ2 + RY2) + slSin(RZ2 - RY2));
	R2[1][1] = 32768*(slCos(RX2 + RZ2) + slCos(RX2 - RZ2)) + 
				26384*(-slSin(RX2-RY2-RZ2) + slSin(RX2+RY2-RZ2) + 
						slSin(RX2-RY2+RZ2) - slSin(RX2+RY2+RZ2));
	R2[1][2] = -32768*(slSin(RX2 + RZ2) + slSin(RX2 - RZ2)) + 
				26384*(-slCos(RX2-RY2-RZ2) + slCos(RX2+RY2-RZ2) + 
						slCos(RX2-RY2+RZ2) - slCos(RX2+RY2+RZ2));
	R2[2][0] = -slSin(RY2);
	R2[2][1] = 32768*(slSin(RX2 + RY2) + slSin(RX2 - RY2));
	R2[2][2] = 32768*(slCos(RX2 + RY2) + slCos(RX2 - RY2));
	*/

	//	Apply rotation to face normals
	//	R1 * normal[0]
	//	R1 * normal[0]
	//	R1 * normal[0]
	//	R2 * normal[1]
	//	R2 * normal[1]
	//	R2 * normal[1]
	
	//	Calculate cross products
	x_product(normal[0], normal[3], normal[6]);
	x_product(normal[0], normal[4], normal[7]);
	x_product(normal[0], normal[5], normal[8]);
	x_product(normal[1], normal[3], normal[9]);
	x_product(normal[1], normal[4], normal[10]);
	x_product(normal[1], normal[5], normal[11]);
	x_product(normal[2], normal[3], normal[12]);
	x_product(normal[2], normal[4], normal[13]);
	x_product(normal[2], normal[5], normal[14]);
	
	//	Loop through scaled normals for collision checks (15)
	for(int i = 0; i < 15; i++)
	{
		//	Construct a scaled (1/1.5, 1.5 == 98304) unit vector
		//	(1/1.5 = 2/3 == 43690.7)
		//	Required in case projection is very high in both x and y
		//	Worst case, SQRT(2) * Max FIXED
		scaled[i][1] = slMulFX(43690, normal[i][0]);
		scaled[i][2] = slMulFX(43690, normal[i][1]);
		scaled[i][3] = slMulFX(43690, normal[i][2]);
		
		//	Calculate projection extents projected onto normal
		project_1D_bbox(box1, scaled[i], extent1);
		project_1D_bbox(box2, scaled[i], extent2);
		
		//	Compare extents for overlap
		if(*(extent1) > *(extent2 + 1))
		{
			//	Minimum of extent 1 > max of extent 2, no collision
			return false;
		}
		if(*(extent2) > *(extent1 + 1))
		{
			//	Minimum of extent 2 > max of extent 1, no collision
			return false;
		}
	}
	
	//	No separating axis, collision is true
	return true;
}

//	Clones Boundign box to new location
void clone_bbox(int ID, int destination)
{
	//  Set Center
	boxes[destination].center[X] = boxes[ID].center[X];
	boxes[destination].center[Y] = boxes[ID].center[Y];
	boxes[destination].center[Z] = boxes[ID].center[Z];
	
	//  Set Half Dimension
    boxes[destination].half[X] = boxes[ID].half[X];
	boxes[destination].half[Y] = boxes[ID].half[Y];
	boxes[destination].half[Z] = boxes[ID].half[Z];

    //  Set Corners
    reset_corners(&boxes[destination]);
	
	//  Set Parent ID
    boxes[destination].parent_ID = boxes[ID].parent_ID;
}

//	Destroy Object
void destroy_bbox(int ID)
{
	//	Return if nonsensical ID
	if(ID >= num_object || ID < 0 || num_object <= 0)
	{
		return;
	}
	else
	{
		// Clone last object into destroyed object's lcoation
		clone_bbox(num_object-1, ID);
	}
}

//	Initialize player bounding box
void player_bbox_init(void)
{
	VECTOR center = {0, 0, 0};
	create_bbox(&player_bbox, center, 58983, 58983, 100000, -1);
}

// Print bounding box corners
void print_corners(int ID)
{
	for(int i = 0; i < 8; i++)
	{
		slPrintFX(boxes[ID].corners[i][0],slLocate(0,9 + i));
		slPrintFX(boxes[ID].corners[i][1],slLocate(13,9 + i));
		slPrintFX(boxes[ID].corners[i][2],slLocate(26,9 + i));
	}
}
