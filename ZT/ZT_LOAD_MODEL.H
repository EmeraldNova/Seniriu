#ifndef ZT_LOAD_MODEL_H
#define ZT_LOAD_MODEL_H

#define MAX_MESHES (40)
#define MAX_MODELS (40)

/**Gouraud shading*/
#define	GRtbl(n)	    (0x1c38+(n))
#define	GRaddr	        0x1c38
#define GOUR_REAL_MAX   768

//No Touchy Sound Ram Start!
#define SNDRAM  (631242752)
//
#define PQSRAM  (631287808)
#define PCMRAM	(631734272)
//Also the end of sound RAM
#define PCMEND	(631767039)
//
#define LWRAM	(2097152)
#define	HIMEM	(100679680)
//HIMEM has 376 sectors (752 KB)
#define HIEND	(101449728)
//Playback buffers start 352 KB into sound RAM. From 40Kb into sound RAM to 352 KB, area is OK for storing sound.
//Each buffer is 8 sectors / 16 KB
//This is sound RAM, addressable by the MC68EC000 / SCSP
/**MUSIC BUFFER REGION 160KB / 80 SECTORS / 5 * 32768 **/
///THIS IS UNNECESSARILY LARGE
#define PCMBUF1 (SNDRAM + 360448)
#define PCMBUF2 (PCMBUF1 + 32768)
#define PCMBUF3 (PCMBUF2 + 32768)
#define PCMBUF4	(PCMBUF3 + 32768)
#define PCMBUF5 (PCMBUF4 + 32768)
/**END MUSIC BUFFER REGION**/
///Each individual PCM buffer is 16384 bytes / 8 sectors for a total of 304 KB / 152 sectors from 19 buffers total.
///Each buffer can store 8 frames of 30720Hz single-channel audio or 16 frames of 15360Hz single-channel audio.
#define PCMBUF6		(SNDRAM + 40960)
#define PCMBUF7		(PCMBUF6 + 16384)
#define PCMBUF8		(PCMBUF7 + 16384)
#define PCMBUF9		(PCMBUF8 + 16384)
#define PCMBUF10	(PCMBUF9 + 16384)
#define PCMBUF11	(PCMBUF10 + 16384)
#define PCMBUF12	(PCMBUF11 + 16384)
#define PCMBUF13	(PCMBUF12 + 16384)
#define PCMBUF14	(PCMBUF13 + 16384)
#define PCMBUF15	(PCMBUF14 + 16384)
#define PCMBUF16	(PCMBUF15 + 16384)
#define PCMBUF17	(PCMBUF16 + 16384)
#define PCMBUF18	(PCMBUF17 + 16384)
#define PCMBUF19	(PCMBUF18 + 16384)
#define PCMBUF20	(PCMBUF19 + 16384)
#define PCMBUF21	(PCMBUF20 + 16384)
#define PCMBUF22	(PCMBUF21 + 16384)
#define PCMBUF23	(PCMBUF22 + 16384)
#define PCMBUF24	(PCMBUF23 + 16384)

#define MAP_TO_SCSP(sh2map_snd_adr) ((sh2map_snd_adr - SNDRAM)>>4)

//THIS IS TESTED
#define S3072KHZ	(31122)
#define S3072TIMER	(1024)
#define	S3072PALT	(1233)
#define S1536KHZ	(29074)
#define S1536TIMER	(512)
#define S1536PALT	(633)
//

typedef struct{
	void* rd_pcmbuf;
	int play_pcmbuf;
} snd_ring;

/**
PCM DATA STRUCTURE
Top lines: GFS Information
file_done : if there is actually data in this pcm data.
active : if this is actively being filled with data.
dstAddress : where the data is going to go.
fid : file ID. Use GFS_NameToId((Sint8*)name) on your file. Changing folders and such is possible but not covered here.

pitchword : The bitrate, converted into a pitch word for the sound CPU.
playsize : the size of data to be played back. NOTE: There are issues with play-sizes that run over 255 frames. Limitation of sound CPU? Dunno.
loctbl : [Archaic] Represents the order in your PCM buffer segments. A suggestion, that even I may not follow.
segments : the number of PCM buffer segments the file consumes. [Big files don't play that well..]
playtimer : an active timer of how long this PCM sound effect has been played.
frames : the number of frames this sound needs to play. The math is strange. The base factor is 1 frame per 16 KB, as derived from the reading process [8 sectors].
^< Explanation: For 15360 bitrates, 15360 bits * 16 bit PCM = 245760 raw bitrate / 8 bits = 30720 bytes/s / 30 = 1024 bytes per frame.
^< Explanation: For 30720 bitrates, 30720 bits * 16 bit PCM = 491520 raw bitrate / 8 bits = 61440 bytes/s / 30 = 2048 bytes per frame.
For 15.360KHz playback, 1KB is played per frame. For 30.720KHz playback, 2KB is played per frame.
**/
typedef struct{
	Bool	file_done;
	Bool	active;
	int	dstAddress;
	Sint8*	fid;
	
	int	pitchword;
	int	playsize;
	int	loctbl;
	int	segments;
	int	playtimer;
	int	frames;
} p64pcm;

extern p64pcm		pcm_slot[19];
extern snd_ring		music_buf[4];

extern Sint8*		music;
extern int			musicPitch;
extern int			musicTimer;
extern Uint8		CH_SND_NUM[8];
extern Uint8		buf_pos;
extern void*		curpcmbuf;
extern Sint8		buffers_filled;
extern Sint16		fetch_timer;
extern Sint16		music_frames;
extern Bool			m_trig;
extern Bool			chg_music;

// [sound] Functions go here
///Notice: In loading order, put sound requests below model requests.
void	p64SoundRequest(char* dirname, Sint8* name, Sint32 bitrate, Uint8 destBufSeg, Uint8 pcmNum, Bool dirChange);
void	sound_on_channel(Uint8 sound_number, Uint8 channel);
void	music_vblIn(Uint8 vol);
void	trigger_sound(Uint8 channel, Uint8 sound_number);
//


typedef     Sint16	compVert[XYZ];
typedef     Uint8   compNorm;

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
    unsigned short nbFrames;
    unsigned short framerate; //Interpolation factor of 2, means the number of frames between 2 key frames
    anim_struct *  animation;
} modelData_t;

typedef struct
{
    FIXED   pos[XYZ];
    FIXED   length[XYZ];
    Uint16 nbMeshes;
    Uint16 nbFrames;
    Uint16 AnimInterpolation;
    anim_struct * animation[240];
    XPDATA * pol[MAX_MESHES];
} entity_t;

typedef struct
{
	Bool	file_done;
	Bool	active;
	void *	dstAddress;
	entity_t * tmodel;
	Sint8 * filename;
}	request;

/**Store all your PDATA meshes here**/
extern entity_t entities[MAX_MODELS];
extern unsigned int gouraudCounter;

extern  request	requests[80];

void setTextures(Uint32 id, entity_t * model, unsigned int total_meshes, Bool UseRealtimeGouraud);

Uint16 loadTextures(void * startAddress, modelData_t * modelData);

void * loadPDATA(void * startAddress, entity_t * model, modelData_t * modelData);

void * loadAnimations(void * startAddress, entity_t * model, modelData_t * modelData);

/** This function loads a 3d polygonal model. Returns the last address in LWRAM (to allow loading multiple meshes)**/
void *      ztLoad3Dmodel(Sint8 * filename, void * startAddress, entity_t * model,  Bool UseRealtimeGouraud);
//Notice: In loading order, models come first.
void * ztModelRequest(char * dirname, Sint8 * name, void * startAddress, entity_t * model, Uint16 rqNum, Bool dirChange);

void	master_file_system(void(*game_code)(void));

#endif // ZT_LOAD_MODEL_H
