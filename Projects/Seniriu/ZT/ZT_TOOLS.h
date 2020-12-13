#ifndef ZT_TOOLS_H
#define ZT_TOOLS_H

/***
Modified from Z-TREME ENGINE tool file
***/
void ztClearText(void);
void ztLoadVDP2bmp(char * filename, Uint16 *VDP2RAM, unsigned short paletteId, Uint8 NBGtype);
void ztFont2NBG3(void);
void Cel2VRAM( Uint8 *Cel_Data , void *Cell_Adr , Uint32 suu );
void Map2VRAM( Uint16 *Map_Data , void *Map_Adr , Uint16 suuj , Uint16 suui , Uint16 palnum ,Uint32 mapoff);
void Pal2CRAM( Uint16 *Pal_Data , void *Col_Adr , Uint32 suu );
void memcpy_w(volatile Sint16 *dst, Sint16 *src, Sint32 ctn_size);
void memcpy_l(volatile Sint32 *dst, Sint32 *src, Sint32 cnt_size);
void memset_w(volatile Sint16 *dest, Sint16 pattern, Sint32 cnt_size);
void memset_l(volatile Sint32 *dest, Sint32 pattern, Sint32 cnt_size);
void fadeIn(void);
void fadeOut(Uint8 clearBG);
FIXED CheckOnPlane(POINT camPt, POINT PlanePt1, VECTOR PlaneNormal);

#endif // ZT_TOOLS_H
