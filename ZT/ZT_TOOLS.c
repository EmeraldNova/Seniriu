#include "ZT_COMMON.h"


/*****
File containing useful functions not specify to any other files.
Modified from Z-Treme engine
*****/

#define DEF_FRAMERATE (2)  //Link to your framerate value here

/**To do : modify values the memory directly**/
void ztClearText(void)
{
    Uint16 i;
    for (i=0; i<64; ++i)
        slPrint("                                                                ", slLocate(0, i));
}

void ztLoadVDP2bmp(char * filename, Uint16 *VDP2RAM, unsigned short paletteId, Uint8 NBGtype)
{
    char * ptr = (char*)LWRAM;

    Sint32 fid = GFS_NameToId((Sint8*)filename);
    if (fid < 0) {slPrint("ERROR", slLocate(10,5)); fadeIn(); return;}
    else GFS_Load(fid, 0, (void*)LWRAM, 512*512+44+512);

    img_t * img;
    img = (img_t*)(LWRAM);
    img->data = (Uint16*)(LWRAM+sizeof(img_t));

    if (img->cmode == COL_16) img->cmode=2;
    else img->cmode=1;

    if (img->nbcol <= 16) img->nbcol=16;
    else img->nbcol=256;

    int vdp2_width;
    if (img->width <= 512) vdp2_width=512;
    else vdp2_width=1024;

    Sint32 yy, xx;
    Uint8 *Cel_Data = (Uint8*)(LWRAM+sizeof(img_t));
	Uint8 *VRAM = (Uint8 *) VDP2RAM;

    for (yy = 0; yy < img->height; yy++)
    {
        for (xx=0; xx< img->width/img->cmode; xx++)
        {
            *(VRAM) = Cel_Data[xx+(yy*img->width/img->cmode)];//*(Cel_Data++);
            *(VRAM++);
        }
        *(VRAM+=(vdp2_width/img->cmode - (img->width/img->cmode)));
    }


    ptr = (char*)(LWRAM+sizeof(img_t)+(img->width*img->height/img->cmode));

    Pal2CRAM((unsigned short*)(ptr), (void*)(VDP2_COLRAM + (paletteId*0x0200)), img->nbcol);

    if (NBGtype==bmNBG1)
    {
        if (img->cmode == 2) slBitMapNbg1(COL_TYPE_16, BM_512x256, (void *)VDP2RAM); //Need some kind of offset
        else slBitMapNbg1(COL_TYPE_256, BM_512x512, (void *)VDP2RAM);
        slBMPalette(bmNBG1 , paletteId);  //This is the CRAM Offset (0 to 7) -> 2048 colors, each palette containing 256 colors. In 16 colors mode, only the first 16 colors can be choosen
    }
    else
    {
        if (img->cmode == 2) slBitMapNbg0(COL_TYPE_16, BM_512x256, (void *)VDP2RAM); //Need some kind of offset
        else slBitMapNbg0(COL_TYPE_256, BM_512x512, (void *)VDP2RAM);
        slBMPalette(bmNBG0 , paletteId);  //This is the CRAM Offset (0 to 7) -> 2048 colors, each palette containing 256 colors. In 16 colors mode, only the first 16 colors can be choosen
    }
    slScrPosNbg1(toFIXED(0.0), toFIXED(0.0));
    slScrPosNbg0(toFIXED(0.0), toFIXED(0.0));
    slScrAutoDisp(NBG0ON | NBG1ON | NBG3ON);
	slColOffsetA(0,0,0);
}

/** Swiches the text display to NBG3 to free up the NBG0 **/
void ztFont2NBG3(void)
{
    slCharNbg3(COL_TYPE_256, CHAR_SIZE_1x1);
    slPageNbg3((void*)0x25e60000, 0, PNB_1WORD|CN_10BIT );
    slPlaneNbg3(PL_SIZE_1x1);
    slMapNbg3((void*)0x25e76000, (void*)0x25e76000, (void*)0x25e76000, (void*)0x25e76000);

    slBitMapNbg0(COL_TYPE_16, BM_512x256, (void *)VDP2_VRAM_A0);
    slBMPalette(bmNBG0 , 0);
    slPriorityNbg0(5);
    slPriorityNbg1(4);
    slPriorityNbg2(1);
    slPriorityNbg3(7);
    slPrioritySpr0(6);
    slPriorityRbg0(2);
    //slScrAutoDisp(NBG0ON|NBG1ON|NBG3ON|SPRON);
}

void Cel2VRAM( Uint8 *Cel_Data , void *Cell_Adr , Uint32 suu )
{
	Uint32 i;
	Uint8 *VRAM;

	VRAM = (Uint8 *)Cell_Adr;

	for( i = 0; i < suu; i++ )
		*(VRAM++) = *(Cel_Data++);
}
void Map2VRAM( Uint16 *Map_Data , void *Map_Adr , Uint16 suuj , Uint16 suui , Uint16 palnum ,Uint32 mapoff)
{
	Uint16 i , j;
	Uint16 paloff;
	Uint16 *VRAM;

	paloff= palnum << 12;
	VRAM = (Uint16 *)Map_Adr;


	for( i = 0; i < suui; i++ ) {
		for( j = 0; j < suuj; j++ ) {
			*VRAM++ = (*Map_Data | paloff) + mapoff;
			Map_Data++;
		}
		VRAM += (64 - suuj);
	}
}
void Pal2CRAM( Uint16 *Pal_Data , void *Col_Adr , Uint32 suu )
{
	Uint16 i;
	Uint16 *VRAM;

	VRAM = (Uint16 *)Col_Adr;

	for( i = 0; i < suu; i++ )
		*(VRAM++) = *(Pal_Data++);
}

void memcpy_w(volatile Sint16 *dst, Sint16 *src, Sint32 ctn_size)
{
	register Sint32	i;

	for (i = 0; i < ctn_size; i+= sizeof(Sint16)) {
		*dst++ = *src++;
	}
}

void memset_w(volatile Sint16 *dest, Sint16 pattern, Sint32 cnt_size)
{
	register Sint32	i;

	for (i = 0; i < cnt_size; i+= sizeof(Sint16)) {
		*dest++ = pattern;
	}
}


void memcpy_l(volatile Sint32 *dst, Sint32 *src, Sint32 cnt_size)
{
	register Sint32	i;

	for (i = 0; i < cnt_size; i+= sizeof(Sint32)) {
		*dst++ = *src++;
	}
}

void memset_l(volatile Sint32 *dest, Sint32 pattern, Sint32 cnt_size)
{
	register Sint32	i;

	for (i = 0; i < cnt_size; i+= sizeof(Sint32)) {
		*dest++ = pattern;
	}
}

#define Color_offset (255)
void fadeIn(void)
{
    int i;
    slSynch(); //Clear framebuffer
    slBack1ColSet((void *) BACK_CRAM, CD_Black);
    slColOffsetB(-Color_offset,-Color_offset,-Color_offset);
    slColOffsetOn(NBG0ON | NBG1ON | NBG2ON | NBG3ON | SPRON | RBG0ON);
	slColOffsetBUse(NBG0ON | NBG1ON | NBG2ON | NBG3ON | SPRON | RBG0ON);

    slTVOn();
    for (i=Color_offset; i>=0;)
    {
       slColOffsetB(-i,-i,-i);
       slSynch();
       i-=(DEF_FRAMERATE*4);
    }
    slColOffsetB(0,0,0);

}

void fadeOut(Uint8 clearBG)
{

    int i;
    slBack1ColSet((void *) BACK_CRAM, CD_Black);
    slColOffsetOn(NBG0ON | NBG1ON | NBG2ON | NBG3ON | SPRON | RBG0ON);
	slColOffsetBUse(NBG0ON | NBG1ON | NBG2ON | NBG3ON | SPRON | RBG0ON);


    for (i=0; i<=Color_offset;)
    {
        slColOffsetB(-i,-i,-i);
        slSynch();
        i+=(DEF_FRAMERATE*4);
    }
    if (clearBG==true)
    {
        memset_w((Sint16*)VDP2_VRAM_A0, 0, 0x040000);
        memset_w((Sint16*)VDP2_VRAM_B0, 0, 0x020000);

    }
    ztClearText();
    slColOffsetB(-Color_offset,-Color_offset,-Color_offset);
    slScrPosNbg0(toFIXED(0.0), toFIXED(0.0));
    slScrPosNbg1(toFIXED(0.0), toFIXED(0.0));
    slTVOff();
}

FIXED CheckOnPlane(POINT camPt, POINT PlanePt1, VECTOR PlaneNormal)
{
    VECTOR ptVector;
    ptVector[0] = camPt[0] - PlanePt1[0];
    ptVector[1] = camPt[1] - PlanePt1[1];
    ptVector[2] = camPt[2] - PlanePt1[2];

    return slInnerProduct(ptVector, PlaneNormal);
}
