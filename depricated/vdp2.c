

#define		RBG0RB_CEL_ADR			(VDP2_VRAM_A0            )
#define		RBG0RB_MAP_ADR			(VDP2_VRAM_B0            )
#define		RBG0RB_COL_ADR			(VDP2_COLRAM    + 0x00200)

#define		RBG0RA_CEL_ADR			(RBG0RB_CEL_ADR + 0x06e80)
#define		RBG0RA_MAP_ADR			(RBG0RB_MAP_ADR + 0x02000)
#define		RBG0RA_COL_ADR			(RBG0RB_COL_ADR + 0x00200)

#define		RBG0_KTB_ADR			(VDP2_VRAM_A1            )
#define		RBG0_PRA_ADR			(VDP2_VRAM_A1   + 0x1ff00)
#define		RBG0_PRB_ADR			(RBG0_PRA_ADR   + 0xf0080)

#define		BACK_COL_ADR			(VDP2_VRAM_A1   + 0x1fffe)
#define		LINECOLOR				(VDP2_VRAM_A0   + 0x04000)


#include	"VDP2/sora4.cel"
#include	"VDP2/sora4.map"
#include	"VDP2/sora4.pal"
#include	"VDP2/tuti2.cel"
#include	"VDP2/tuti2.map"
#include	"VDP2/tuti2.pal"

#include "ZT/ZT_COMMON.h"

void	init_vdp2()
{
	slRparaInitSet((void *)RBG0_PRA_ADR);
	slMakeKtable((void *)RBG0_KTB_ADR);
	slCharRbg0(COL_TYPE_256 , CHAR_SIZE_1x1);
	slPageRbg0((void *)RBG0RB_CEL_ADR , 0 , PNB_1WORD|CN_12BIT);

	slPlaneRA(PL_SIZE_1x1);
	sl1MapRA((void *)RBG0RA_MAP_ADR);
	slOverRA(0);
	slKtableRA((void *)RBG0_KTB_ADR , K_MODE0 | K_FIX | K_LINE | K_2WORD | K_ON | K_LINECOL);
	Cel2VRAM(tuti_cel , (void *)RBG0RA_CEL_ADR , 65536);
	Map2VRAM(tuti_map , (void *)RBG0RA_MAP_ADR , 64 , 64 , 2 , 884);
	Pal2CRAM(tuti_pal , (void *)RBG0RA_COL_ADR , 160);

	slRparaMode(RA);
	slBack1ColSet((void *)BACK_COL_ADR , 0);

    slColorCalc( CC_ADD | CC_TOP | NBG2ON | RBG0ON);
	slColorCalcOn( NBG2ON | RBG0ON);
	slScrTransparent(NBG2ON | RBG0ON);
	slColRateLNCL(0x00);
	
	//slZoomNbg0(toFIXED(1), toFIXED(0.5));
	slScrAutoDisp(NBG0ON | RBG0ON );
}
