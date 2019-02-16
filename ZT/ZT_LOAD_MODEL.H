#ifndef ZT_LOAD_MODEL_H
#define ZT_LOAD_MODEL_H

#define MAX_MESHES (16)
#define MAX_MODELS (10)

/**Gouraud shading*/
#define	GRtbl(n)	    (0x1c38+(n))
#define	GRaddr	        0x1c38
#define GOUR_REAL_MAX   750

typedef     Sint16	compVert[XYZ];
typedef     Uint8   compNorm;

/*
typedef struct
{
    compVert * cVert;
    compNorm * cNorm;
    //POINT * cVert; //compressed TRANSLATION value for vertices : MSB = sign, 7 remaining bits as 6.1 fixed point (shift left 15)
}key_frame_t;
*/

typedef struct
{
 compVert * cVert;
 compNorm * cNorm;

} anim_struct;

typedef struct
{
    unsigned short  TOTAL_MESH; //total amount of PDATA
    short           TOT_TEXT;  //total amount of textures
    unsigned int    TEXT_SIZE;  //to quickly load from disk, that's the size of the textures in bytes
    unsigned int    PDATA_SIZE; //to quickly load from disk, total size of pdata in bytes
    FIXED           Origin[XYZ]; //Origin point used to "center" the culling/collision data and your model's position. Should be 0,0,0 unless you have an offset
    FIXED           Length[XYZ];  //Lenght along the x,y z axis. Together with the origin, that gives you the bounding box for quick broad collision testing
    /*Sint32          DUMMY1;
    Sint32          DUMMY2;*/
    unsigned short nbFrames;
    unsigned short framerate; //Interpolation factor of 2, means the number of frames between 2 key frames
    anim_struct *  animation;
} modelData_t;

typedef struct ztObj
{
    PDATA * pol;
    Sint16 objID;
    ANGLE rotation[XYZ];
    FIXED translation[XYZ];
    struct ztObj * child;
    struct ztObj * sibling;
} ztObject;

typedef struct
{
    FIXED   pos[XYZ];
    FIXED   length[XYZ];
    Uint16  nbMeshes;
    //
    Uint16 nbFrames;
    Uint16 AnimInterpolation;
    Uint16  DUMMY; //Maybe an identifier would be nice...
    anim_struct * animation[240];
    //ztObject * obj;  --> Maybe in the future
    XPDATA * pol[MAX_MESHES];
} entity_t;

/**Store all your PDATA meshes here**/
extern entity_t entities[MAX_MODELS];
extern unsigned int gouraudCounter;

/** This function loads a 3d polygonal model. Returns the last address in LWRAM (to allow loading multiple meshes)**/
void *      ztLoad3Dmodel(Sint8 * filename, void * startAddress, entity_t * model,  bool UseRealtimeGouraud);

#endif // ZT_LOAD_MODEL_H
