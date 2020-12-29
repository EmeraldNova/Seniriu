#include "sl_def.h"

/*---- [1.This part must not be modified] ------------------------------------*/
const void *PCM_Work = (void *)SoundRAM + 0x78000; /* PCM Stream Address      */
const Uint32 PCM_WkSize = 0x8000;                  /* PCM Stream Size         */
const void *SlaveStack = (void *)0x06001e00;       /* SlaveSH2  StackPointer  */
const void *TransList = (void *)0x060fb800;        /* DMA Transfer Table      */
const void *MasterStack = (void *)0x060ffc00;      /* MasterSH2 StackPointer  */
/*----------------------------------------------------------------------------*/

#define MAX_VERTICES 2000 /* number of vertices that can be used */
#define MAX_POLYGONS 1800 /* number of polygons that can be used */

#define _Byte_ sizeof(Uint8)
#define _LongWord_ sizeof(Uint32)
#define _Sprite_ (sizeof(Uint16) * 18)

const struct
{
    char SortList[(_LongWord_ * 3) * (MAX_POLYGONS + 6)];
    char Zbuffer[_LongWord_ * 512];
    char SpriteBuf[_Sprite_ * ((MAX_POLYGONS + 6) * 2)];
    char Pbuffer[(_LongWord_ * 4) * MAX_VERTICES];
    char CLOfstBuf[(_Byte_ * 32 * 3) * 32];
    char CommandBuf[(_LongWord_ * 8) * MAX_POLYGONS];
} WorkArea;

const Uint16 MaxVertices = MAX_VERTICES;
const Uint16 MaxPolygons = MAX_POLYGONS;
const void *SortList = WorkArea.SortList;
const Uint32 SortListSize = sizeof(WorkArea.SortList);
const void *Zbuffer = WorkArea.Zbuffer;
const void *SpriteBuf = WorkArea.SpriteBuf;
const Uint32 SpriteBufSize = sizeof(WorkArea.SpriteBuf);
const void *Pbuffer = WorkArea.Pbuffer;
const void *CLOfstBuf = WorkArea.CLOfstBuf;
const void *CommandBuf = WorkArea.CommandBuf;

/*----------------------------------------------------------*/
#define MAX_EVENTS 1 /* number of events that can be used   */
const Uint16 EventSize = sizeof(EVENT);
const Uint16 MaxEvents = MAX_EVENTS;
EVENT EventBuf[MAX_EVENTS];
EVENT *RemainEvent[MAX_EVENTS];

#define MAX_WORKS 1 /* number of works that can be used    */
const Uint16 WorkSize = sizeof(WORK);
const Uint16 MaxWorks = MAX_WORKS;
WORK WorkBuf[MAX_WORKS];
WORK *RemainWork[MAX_WORKS];
/*---------------------------------------------------------*/
