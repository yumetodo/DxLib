// -------------------------------------------------------------------------------
// 
// 		�c�w���C�u����		�������C���[�W����p�v���O����
// 
// 				Ver 3.16d
// 
// -------------------------------------------------------------------------------

// �c�w���C�u�����쐬���p��`
#define __DX_MAKE

// �C���N���[�h----------------------------------------------------------------
#include "DxMemImg.h"
#include "DxLib.h"
#include "DxStatic.h"
#include "DxBaseFunc.h"
#include "DxGraphics.h"

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// �}�N����`------------------------------------------------------------------

// �f�[�^�^��`----------------------------------------------------------------

// �f�[�^�錾------------------------------------------------------------------

// �֐��v���g�^�C�v�錾--------------------------------------------------------

// �v���O����------------------------------------------------------------------

#ifndef DX_NON_GRAPHICS

#ifdef DX_NON_2DDRAW

extern void DrawPolygonMemImg( MEMIMG *DestImg, MEMIMG *ZImg, const MEMIMG *SrcImg, const VERTEX_2D *Vertex3, int TransFlag, const MEMIMG *BlendImg, int ZWriteFlag, int PerspectiveEnable, int GouraudShadeMode, int ScissorTest )
{
	return ;
}

#else

#define DRAWPOLYGONMEMIMG_UBI_ND( DRAW, SRCADDNUM, DESTADDNUM )	\
{\
	/*y = StartY ;*/\
	ycon = 0 ;\
	for( ; ycon < NumY ; ycon ++,/*y ++,*/\
		 RHWT += RHWPY,\
		 UT += UPY,\
		 VT += VPY,\
		 ZT += ZPY,\
		 RT += RPY,\
		 GT += GPY,\
		 BT += BPY,\
		 AT += APY,\
		 ZBufPT += ZPitch,\
		 DestPT += DestPitch\
	 )\
	{\
		xcon = StartX_NumX[ycon][1] ;\
		if( xcon == 0 ) continue ;\
		x = StartX_NumX[ycon][0] ;\
		\
		RHW = RHWPX * x + RHWPX / 2 + RHWT ;\
		Z   = ZPX   * x + ZPX   / 2 + ZT ;\
		U   = UPX   * x + UPX   / 2 + UT ;\
		V   = VPX   * x + VPX   / 2 + VT ;\
		R   = RPX   * x + RPX   / 2 + RT ;\
		G   = GPX   * x + GPX   / 2 + GT ;\
		B   = BPX   * x + BPX   / 2 + BT ;\
		A   = APX   * x + APX   / 2 + AT ;\
		ZBPT = x * 2 + ZBufPT ;\
		DestBPT = x * (DESTADDNUM) + DestPT ;\
		if( RHW == 0 ) continue ;\
		\
		for( ; xcon > 0 ; xcon --, /*x ++,*/\
		 RHW += RHWPX,\
		 U += UPX,\
		 V += VPX,\
		 Z += ZPX,\
		 R += RPX,\
		 G += GPX,\
		 B += BPX,\
		 A += APX,\
		 ZBPT += 2,\
		 DestBPT += (DESTADDNUM)\
		  )\
		{\
			if( ZImg )\
			{\
				if( *ZWPT < (WORD)( Z >> 8 ) )\
					continue ;\
				if( ZWriteFlag ) *ZWPT = ( WORD )( Z >> 8 ) ;\
			}\
\
			INV = ( DWORD )( (1 << 29) / RHW ) ;\
			tu = ( int )( ( ( U >> 10 ) * texWidth  * INV ) >> 19 ) ;\
			tv = ( int )( ( ( V >> 10 ) * texHeight * INV ) >> 19 ) ;\
\
			if( (DWORD)( ( tu | ( texWidth - tu - 1 ) ) | ( tv | ( texHeight - tv - 1 ) ) ) & 0x80000000 )\
				continue ;\
\
			SrcBPT  = SrcBP + tu * (SRCADDNUM) + tv * SrcPitch ;\
			BlendBPT = BlendBP + tu + tv * BlendPitch ;\
\
			DRAW\
		}\
	}\
}

#define DRAWPOLYGONMEMIMG_NBI_ND( DRAW, SRCADDNUM, DESTADDNUM )	\
{\
	/*y = StartY ;*/\
	ycon = 0 ;\
	for( ; ycon < NumY ; ycon ++,/* y ++,*/\
		 RHWT += RHWPY,\
		 UT += UPY,\
		 VT += VPY,\
		 ZT += ZPY,\
		 RT += RPY,\
		 GT += GPY,\
		 BT += BPY,\
		 AT += APY,\
		 ZBufPT += ZPitch,\
		 DestPT += DestPitch\
	 )\
	{\
		xcon = StartX_NumX[ycon][1] ;\
		if( xcon == 0 ) continue ;\
		x = StartX_NumX[ycon][0] ;\
		\
		RHW = RHWPX * x + RHWPX / 2 + RHWT ;\
		Z   = ZPX   * x + ZPX   / 2 + ZT ;\
		U   = UPX   * x + UPX   / 2 + UT ;\
		V   = VPX   * x + VPX   / 2 + VT ;\
		R   = RPX   * x + RPX   / 2 + RT ;\
		G   = GPX   * x + GPX   / 2 + GT ;\
		B   = BPX   * x + BPX   / 2 + BT ;\
		A   = APX   * x + APX   / 2 + AT ;\
		ZBPT = x * 2 + ZBufPT ;\
		DestBPT = x * (DESTADDNUM) + DestPT ;\
		if( RHW == 0 ) continue ;\
		\
		for( ; xcon > 0 ; xcon --, /*x ++,*/\
		 RHW += RHWPX,\
		 U += UPX,\
		 V += VPX,\
		 Z += ZPX,\
		 R += RPX,\
		 G += GPX,\
		 B += BPX,\
		 A += APX,\
		 ZBPT += 2,\
		 DestBPT += (DESTADDNUM)\
		  )\
		{\
			if( ZImg )\
			{\
				if( *ZWPT < (WORD)( Z >> 8 ) )\
					continue ;\
				if( ZWriteFlag ) *ZWPT = ( WORD )( Z >> 8 ) ;\
			}\
\
			INV = ( DWORD )( (1 << 29) / RHW ) ;\
			tu = ( int )( ( ( U >> 10 ) * texWidth  * INV ) >> 19 ) ;\
			tv = ( int )( ( ( V >> 10 ) * texHeight * INV ) >> 19 ) ;\
\
			if( (DWORD)( ( tu | ( texWidth - tu - 1 ) ) | ( tv | ( texHeight - tv - 1 ) ) ) & 0x80000000 )\
				continue ;\
\
			SrcBPT  = SrcBP + tu * (SRCADDNUM) + tv * SrcPitch ;\
\
			DRAW\
		}\
	}\
}

/*
#define DRAWPOLYGONMEMIMG_UBI_ND( DRAW, SRCADDNUM, DESTADDNUM )	\
{\
	y = StartY ;\
	for( ycon = 0 ; ycon < NumY ; ycon ++ )\
	{\
		xcon = StartX_NumX[ycon][1] ;\
		x = StartX_NumX[ycon][0] ;\
		if( x < DrawRectLeft )\
		{\
			xcon -= DrawRectLeft - x ;\
			x = DrawRectLeft ;\
		}\
		if( x + xcon > DrawRectRight )\
		{\
			xcon = DrawRectRight - x ;\
		}\
		\
		for( ; xcon > 0 ; xcon --, x ++ )\
		{\
			rhw = rhwPX * x + rhwPY * y + rhw0 ;\
			u = tuPX * x + tuPY * y + tu0 ;\
			v = tvPX * x + tvPY * y + tv0 ;\
			r = rPX * x + rPY * y + r0 ;\
			g = gPX * x + gPY * y + g0 ;\
			b = bPX * x + bPY * y + b0 ;\
			a = aPX * x + aPY * y + a0 ;\
			\
			z = 1.0F / rhw ;\
			if( z > 1.0F || z < 0.0F ) continue ;\
			tu = _FTOL( u * z ) ;\
			tv = _FTOL( v * z ) ;\
			if( ( ( tu | ( texWidth - tu ) ) | ( tv | ( texHeight - tv ) ) ) & 0x80000000 ) continue ;\
			\
			SrcBPT = SrcBP + tu * (SRCADDNUM) + tv * SrcPitch ;\
			BlendBPT = BlendBP + tu + tv * BlendPitch ;\
			DestBPT = DestBP + x * (DESTADDNUM) + y * DestPitch ;\
			MemImgManage.DrawBright.Red   = _FTOL( r ) ;\
			MemImgManage.DrawBright.Green = _FTOL( g ) ;\
			MemImgManage.DrawBright.Blue  = _FTOL( b ) ;\
			MemImgManage.BlendParam = _FTOL( a ) ;\
			DRAW\
		}\
		\
		y ++ ;\
		ycon ++ ;\
	}\
}


#define DRAWPOLYGONMEMIMG_NBI_ND( DRAW, SRCADDNUM, DESTADDNUM )	\
{\
	y = StartY ;\
	for( ycon = 0 ; ycon < NumY ; ycon ++ )\
	{\
		xcon = StartX_NumX[ycon][1] ;\
		x = StartX_NumX[ycon][0] ;\
		if( x < DrawRectLeft )\
		{\
			xcon -= DrawRectLeft - x ;\
			x = DrawRectLeft ;\
		}\
		if( x + xcon > DrawRectRight )\
		{\
			xcon = DrawRectRight - x ;\
		}\
		\
		for( ; xcon > 0 ; xcon --, x ++ )\
		{\
			rhw = rhwPX * x + rhwPY * y + rhw0 ;\
			u = tuPX * x + tuPY * y + tu0 ;\
			v = tvPX * x + tvPY * y + tv0 ;\
			r = rPX * x + rPY * y + r0 ;\
			g = gPX * x + gPY * y + g0 ;\
			b = bPX * x + bPY * y + b0 ;\
			a = aPX * x + aPY * y + a0 ;\
			\
			z = 1.0F / rhw ;\
			if( z > 1.0F || z < 0.0F ) continue ;\
			tu = _FTOL( u * z * texWidth - 0.5F ) ;\
			tv = _FTOL( v * z * texHeight - 0.5F ) ;\
			if( (DWORD)( ( tu | ( texWidth - tu ) ) | ( tv | ( texHeight - tv ) ) ) & 0x80000000 ) continue ;\
			\
			SrcBPT = SrcBP + tu * (SRCADDNUM) + tv * SrcPitch ;\
			DestBPT = DestBP + x * (DESTADDNUM) + y * DestPitch ;\
			MemImgManage.DrawBright.Red   = _FTOL( r ) ;\
			MemImgManage.DrawBright.Green = _FTOL( g ) ;\
			MemImgManage.DrawBright.Blue  = _FTOL( b ) ;\
			MemImgManage.BlendParam = _FTOL( a ) ;\
			DRAW\
		}\
		\
		y ++ ;\
		ycon ++ ;\
	}\
}
*/


// �C���[�W���g�p���ă|���S����`�悷��( �R�c��ԂɂR�c�|���S����`�悷��̂��ړI )
#define PAL16	PaletteWP[ *SrcBPT ]
#define DST16	*DestWPT
#define SRC16	*SrcWPT
#define SRCA16	SrcBPT[2]
#define DSTP16	DestWPT

#define PALP32	(BYTE *)&PaletteDP[ *SrcBPT ]
#define SRCP32	SrcBPT
#define DSTP32	DestBPT

#define AB		(A / COLORQUALI)
#define RB		(R / COLORQUALI)
#define GB		(G / COLORQUALI)
#define BB		(B / COLORQUALI)

#define BLND	*BlendBPT

#define MAXLINE		(4096)
#define RHWQUALI	(1 << 26)
#define UVQUALI		(1 << 26)
#define ZQUALI		(1 << 24)
#define COLORQUALI	(1 << 14)

extern void DrawPolygonMemImg( MEMIMG *DestImg, MEMIMG *ZImg, const MEMIMG *SrcImg, const VERTEX_2D *Vertex3, int TransFlag, const MEMIMG *BlendImg, int ZWriteFlag, int PerspectiveEnable, int GouraudShadeMode, int ScissorTest )
{
	DWORD *RateTable1 ;
	int *RateTable2 ;
	DWORD Src1, Src2, Data, DataD ;
	DWORD *RedTable , *GreenTable , *BlueTable ;
	DWORD SrcPitch, DestPitch, BlendPitch, ZPitch ;
	DWORD TransColor ;
	MEMIMG TempMemImg;
	bool UseTempMemImg = false;

	int DrawRectRight,  DrawRectLeft ;
	int DrawRectBottom, DrawRectTop ;

	float tuPX, tuPY, tvPX, tvPY ;		// a, b, c, d �̑���A�X�N���[�����W����e�N�X�`�����W�ɕϊ�����ׂ̂���
	float tu0, tv0 ;					// U00, V00 �̑���A�ϊ���̃e�N�X�`�����W�̌��_�ƂȂ���W
	float rhwPX, rhwPY ;				// �X�N���[�����W���� RHW �ɕϊ�����ׂ̂���
	float rhw0 ;						// �ϊ���� RHW �̌��_�ƂȂ�l
	float zPX, zPY ;					// �X�N���[�����W���� Z �ɕϊ�����ׂ̂���
	float z0 ;							// �ϊ���� Z �̌��_�ƂȂ�l
	float rPX, rPY, gPX, gPY, bPX, bPY, aPX, aPY ;	// �X�N���[�����W����F�l�ɕϊ�����ׂ̂���
	float r0, g0, b0, a0 ;				// �ϊ���̐F�l�̌��_�ƂȂ�l
	float px0, py0 ;					// x0, y0 �̑���A�e�N�X�`�����W�ɕϊ�����ۂ̃X�N���[�����W�̌��_�ƂȂ���W
	float StartPLX[2], StartPLY[2], StartPRX[2], StartPRY[2] ;	// BS1X, BS2Y ���̑���A�e���_�̕`��J�n���W
//	float PLX, PLY, PRX, PRY ;			// B1X, B1Y, B2X, B2Y �̑���A�������̍��E�̍��W��ێ�����
	float AddPLX[2], AddPRX[2] ;		// BAA1X ���̑���A�P���C�����ɕ`��J�n���W�ɉ��Z����l
//	float AdPLX, AdPRX ;				// BA1X, BA2X �̑���A���݂̃t�F�[�Y�Ŏg�p���� AddPLX, AddPRX �̒l��ۑ�����
	static short StartX_NumX[MAXLINE][2] ; // �e���C���̊J�n�w���W�Ə����h�b�g��( 0:�J�n�w���W  1:�����h�b�g�� )
	int StartY, NumY ;					// �������J�n����x���W�ƁA�������郉�C����
	int tu, tv ;
//	float rhw, u, v, z, r, g, b, a ;
	int x, /*y,*/ xcon, ycon ;
	int texWidth, texHeight ;

	int RHW, RHWPX, RHWPY, RHW0, RHWT ;
	int U, UT, UPX, UPY, U0 ;
	int V, VT, VPX, VPY, V0 ;
	int Z, ZT, ZPX, ZPY, Z0 ;
	int R, RT, RPX, RPY, R0 ;
	int G, GT, GPX, GPY, G0 ;
	int B, BT, BPX, BPY, B0 ;
	int A, AT, APX, APY, A0 ;
	BYTE *DestPT, *ZBufPT ;
	DWORD INV ; 

	const VERTEX_2D *YTable[3], *XTable[3], *ZTable[3] ;

	union
	{
		BYTE *ZBP ;
		WORD *ZWP ;
	} ;
	union
	{
		BYTE *ZBPT ;
		WORD *ZWPT ;
	} ;
	BYTE *BlendBP = NULL ;
	BYTE *BlendBPT ;
	union
	{
		BYTE SrcB[4] ;
		DWORD SrcD ;
	} ;
	union
	{
		BYTE  *SrcBPT ;
		WORD  *SrcWPT ;
		DWORD *SrcDPT ;
	} ;
	union
	{
		BYTE  *PaletteBP ;
		WORD  *PaletteWP ;
		DWORD *PaletteDP ;
	} ;
	union
	{
		BYTE  *SrcBP ;
		WORD  *SrcWP ;
		DWORD *SrcDP ;
	} ;
	union
	{
		BYTE *DestBP ;
		WORD *DestWP ;
		DWORD *DestDP ;
	} ;
	union
	{
		BYTE *DestBPT ;
		WORD *DestWPT ;
		DWORD *DestDPT ;
	} ;

	// �V�U�����O�e�X�g�̎w�肪����ꍇ�̓V�U�����O�̕K�v�����邩����
	// �K�v������ꍇ�̓V�U�����O���s��
	if( ScissorTest == TRUE )
	{
		int SrcVertNum, DestVertNum, Flag ;
		VERTEX_2D Check[2][7], TempVert[3], *ChkSrc, *ChkDst ;
		float a ;
		int ia, k, j ;
		VERTEX_2D *dvP, *dvN ;
		int cflag[7], all ;

		// ����
		all  = cflag[0] = Vertex3[0].pos.z < 0.0F ;
		all += cflag[1] = Vertex3[1].pos.z < 0.0F ;
		all += cflag[2] = Vertex3[2].pos.z < 0.0F ;
		all += cflag[3] = Vertex3[0].pos.z > 1.0F ;
		all += cflag[4] = Vertex3[1].pos.z > 1.0F ;
		all += cflag[5] = Vertex3[2].pos.z > 1.0F ;

		// �S�Ă̒��_���N���b�s���O�Ώۂ̏ꍇ�͂����ŏI��
		if( all >= 3 ) return ;
		
		// �V�U�����O�̕K�v������ꍇ�̓V�U�����O
		if( all != 0 )
		{
			// �����ɗ����ƌ������Ƃ̓V�U�����O���K�v�Ƃ�������
			SrcVertNum = 3 ;
			DestVertNum = 3 ;
			Flag = 1 ;
			Check[0][0] = Vertex3[0] ;
			Check[0][1] = Vertex3[1] ;
			Check[0][2] = Vertex3[2] ;
			Check[0][0].rhw = 1.0F / Check[0][0].rhw ;
			Check[0][0].pos.x *= Check[0][0].rhw ;
			Check[0][0].pos.y *= Check[0][0].rhw ;
			Check[0][0].pos.z *= Check[0][0].rhw ;
			Check[0][1].rhw = 1.0F / Check[0][1].rhw ;
			Check[0][1].pos.x *= Check[0][1].rhw ;
			Check[0][1].pos.y *= Check[0][1].rhw ;
			Check[0][1].pos.z *= Check[0][1].rhw ;
			Check[0][2].rhw = 1.0F / Check[0][2].rhw ;
			Check[0][2].pos.x *= Check[0][2].rhw ;
			Check[0][2].pos.y *= Check[0][2].rhw ;
			Check[0][2].pos.z *= Check[0][2].rhw ;
			for( k = 0 ; k < 2 ; k ++ )
			{
				ChkDst = Check[Flag] ;
				Flag ^= 1 ;
				ChkSrc = Check[Flag] ;
				ChkSrc[SrcVertNum] = ChkSrc[0] ;

				SrcVertNum = DestVertNum ;
				DestVertNum = 0 ;
				switch( k )
				{
				case 0 :	// �y�{�N���b�v��
					cflag[0] = ChkSrc[0].pos.z > ChkSrc[0].rhw ;
					for( j = 0 ; j < SrcVertNum ; j ++, ChkSrc ++ )
					{
						cflag[j+1] = ChkSrc[1].pos.z > ChkSrc[1].rhw ;
						if( cflag[j] == TRUE  && cflag[j+1] == TRUE ) continue ;
						if( cflag[j] == FALSE && cflag[j+1] == FALSE )
						{
							ChkDst[DestVertNum] = ChkSrc[0] ;
							DestVertNum ++ ;
							continue ;
						}

						if( cflag[j] == FALSE )
						{
							ChkDst[DestVertNum] = ChkSrc[0] ;
							DestVertNum ++ ;

							dvP = &ChkSrc[0] ;
							dvN = &ChkSrc[1] ;
						}
						else
						{
							dvP = &ChkSrc[1] ;
							dvN = &ChkSrc[0] ;
						}

						a = ( 1.0F - dvP->pos.z / dvP->rhw ) / ( dvN->pos.z / dvN->rhw - dvP->pos.z / dvP->rhw ) ;
						ia = (BYTE)_FTOL( a * 255 ) ;
						ChkDst[DestVertNum].pos.x = dvP->pos.x + ( dvN->pos.x - dvP->pos.x ) * a ; 
						ChkDst[DestVertNum].pos.y = dvP->pos.y + ( dvN->pos.y - dvP->pos.y ) * a ; 
						ChkDst[DestVertNum].pos.z = dvP->pos.z + ( dvN->pos.z - dvP->pos.z ) * a ; 
						ChkDst[DestVertNum].u     = dvP->u     + ( dvN->u     - dvP->u     ) * a ; 
						ChkDst[DestVertNum].v     = dvP->v     + ( dvN->v     - dvP->v     ) * a ; 
						ChkDst[DestVertNum].rhw   = dvP->rhw   + ( dvN->rhw   - dvP->rhw   ) * a ; 
						((BYTE *)&ChkDst[DestVertNum].color)[0] = ( BYTE )( ( ( ((BYTE *)&dvP->color)[0] << 8 ) + ( ( ((BYTE *)&dvN->color)[0] - ((BYTE *)&dvP->color)[0] ) * ia ) ) >> 8 ) ;
						((BYTE *)&ChkDst[DestVertNum].color)[1] = ( BYTE )( ( ( ((BYTE *)&dvP->color)[1] << 8 ) + ( ( ((BYTE *)&dvN->color)[0] - ((BYTE *)&dvP->color)[1] ) * ia ) ) >> 8 ) ; 
						((BYTE *)&ChkDst[DestVertNum].color)[2] = ( BYTE )( ( ( ((BYTE *)&dvP->color)[2] << 8 ) + ( ( ((BYTE *)&dvN->color)[0] - ((BYTE *)&dvP->color)[2] ) * ia ) ) >> 8 ) ; 
						((BYTE *)&ChkDst[DestVertNum].color)[3] = ( BYTE )( ( ( ((BYTE *)&dvP->color)[3] << 8 ) + ( ( ((BYTE *)&dvN->color)[0] - ((BYTE *)&dvP->color)[3] ) * ia ) ) >> 8 ) ; 
						DestVertNum ++ ;
					}
					break ;

				case 1 :	// �y�|�N���b�v��
					cflag[0] = ChkSrc[0].pos.z < 0.0F ;
					for( j = 0 ; j < SrcVertNum ; j ++, ChkSrc ++ )
					{
						cflag[j+1] = ChkSrc[1].pos.z < 0.0F ;
						if( cflag[j] == TRUE  && cflag[j+1] == TRUE ) continue ;
						if( cflag[j] == FALSE && cflag[j+1] == FALSE )
						{
							ChkDst[DestVertNum] = ChkSrc[0] ;
							DestVertNum ++ ;
							continue ;
						}

						if( cflag[j] == FALSE )
						{
							ChkDst[DestVertNum] = ChkSrc[0] ;
							DestVertNum ++ ;

							dvP = &ChkSrc[1] ;
							dvN = &ChkSrc[0] ;
						}
						else
						{
							dvP = &ChkSrc[0] ;
							dvN = &ChkSrc[1] ;
						}

						a = -dvP->pos.z / ( dvN->pos.z - dvP->pos.z ) ;
						ia = (BYTE)_FTOL( a * 255 ) ;
						ChkDst[DestVertNum].pos.x = dvP->pos.x + ( dvN->pos.x - dvP->pos.x ) * a ; 
						ChkDst[DestVertNum].pos.y = dvP->pos.y + ( dvN->pos.y - dvP->pos.y ) * a ; 
						ChkDst[DestVertNum].pos.z = 0.0F /*dvP->pos.z + ( dvN->pos.z - dvP->pos.z ) * a*/ ; 
						ChkDst[DestVertNum].u     = dvP->u     + ( dvN->u     - dvP->u     ) * a ; 
						ChkDst[DestVertNum].v     = dvP->v     + ( dvN->v     - dvP->v     ) * a ; 
						ChkDst[DestVertNum].rhw   = dvP->rhw   + ( dvN->rhw   - dvP->rhw   ) * a ; 
						((BYTE *)&ChkDst[DestVertNum].color)[0] = ( BYTE )( ( ( ((BYTE *)&dvP->color)[0] << 8 ) + ( ( ((BYTE *)&dvN->color)[0] - ((BYTE *)&dvP->color)[0] ) * ia ) ) >> 8 ) ;
						((BYTE *)&ChkDst[DestVertNum].color)[1] = ( BYTE )( ( ( ((BYTE *)&dvP->color)[1] << 8 ) + ( ( ((BYTE *)&dvN->color)[1] - ((BYTE *)&dvP->color)[1] ) * ia ) ) >> 8 ) ; 
						((BYTE *)&ChkDst[DestVertNum].color)[2] = ( BYTE )( ( ( ((BYTE *)&dvP->color)[2] << 8 ) + ( ( ((BYTE *)&dvN->color)[2] - ((BYTE *)&dvP->color)[2] ) * ia ) ) >> 8 ) ; 
						((BYTE *)&ChkDst[DestVertNum].color)[3] = ( BYTE )( ( ( ((BYTE *)&dvP->color)[3] << 8 ) + ( ( ((BYTE *)&dvN->color)[3] - ((BYTE *)&dvP->color)[3] ) * ia ) ) >> 8 ) ; 
						DestVertNum ++ ;
					}
					break ;
				}
			}

			for( j = 0 ; j < DestVertNum ; j ++ )
			{
				ChkDst[j].rhw = 1.0F / ChkDst[j].rhw ;
				ChkDst[j].pos.x *= ChkDst[j].rhw ;
				ChkDst[j].pos.y *= ChkDst[j].rhw ;
				ChkDst[j].pos.z *= ChkDst[j].rhw ;
			}

			// �V�U�����O�������ʂ��ēx���̊֐��ɓn��
			DrawPolygonMemImg( DestImg, ZImg, SrcImg, &ChkDst[0], TransFlag, BlendImg, PerspectiveEnable, FALSE ) ;
			for( j = 3 ; j < DestVertNum ; j ++ )
			{
				TempVert[0] = ChkDst[j] ;
				TempVert[1] = ChkDst[0] ;
				TempVert[2] = ChkDst[j-1] ;
				DrawPolygonMemImg( DestImg, ZImg, SrcImg, TempVert, TransFlag, BlendImg, PerspectiveEnable, FALSE ) ;
			}
			
			// �V�U�����O�����ꍇ�͂����ŏI��
			return ;
		}
	}

	// �y�o�b�t�@���`���Ɠ����T�C�Y�ł͂Ȃ�������G���[
	if( ZImg != NULL && ( ZImg->Width != DestImg->Width || ZImg->Height != DestImg->Height ) ) return ;

	// �e�N�X�`���������ꍇ�͏������e�N�X�`�����ł����グ��
	if( SrcImg == NULL )
	{
		UseTempMemImg = true;
		_MEMSET( &TempMemImg, 0, sizeof( TempMemImg ) );
		InitializeMemImg( &TempMemImg, 8, 8, -1, 0, DestImg->Base->ColorType, FALSE, FALSE, FALSE );
		DrawFillBoxMemImg( &TempMemImg, 0, 0, 8, 8, 0xffffffff );
		SrcImg = &TempMemImg;
	}

	// �J���[�^�C�v���Ⴄ���A�`��悪���`�����l���t���p���b�g�t���������ꍇ�̓G���[
	if( SrcImg->Base->ColorType != DestImg->Base->ColorType || DestImg->Base->UsePalette == 1 || DestImg->Base->UseAlpha == 1 ) return ;

	// ��ԏ�ɗ��Ă�����W�ɂ���ĕ`��N�_���W��������
	{
		const VERTEX_2D *LP, *RP ;
		int DMODEL, DMODER ;

		DMODEL = DMODER = 0 ;

		// XTable �� x �̏��������̃A�h���X��������
		if( Vertex3[0].pos.x > Vertex3[1].pos.x ){
			if( Vertex3[1].pos.x > Vertex3[2].pos.x ){
				XTable[0] = &Vertex3[2] ;
				XTable[1] = &Vertex3[1] ;
				XTable[2] = &Vertex3[0] ;
			}else{
				XTable[0] = &Vertex3[1] ;
				if( Vertex3[2].pos.x > Vertex3[0].pos.x ){
					XTable[1] = &Vertex3[0] ;
					XTable[2] = &Vertex3[2] ;
				}else{
					XTable[1] = &Vertex3[2] ;
					XTable[2] = &Vertex3[0] ;
				}
			}
		}else{
			if( Vertex3[0].pos.x > Vertex3[2].pos.x ){
				XTable[0] = &Vertex3[2] ;
				XTable[1] = &Vertex3[0] ;
				XTable[2] = &Vertex3[1] ;
			}else{
				XTable[0] = &Vertex3[0] ;
				if( Vertex3[2].pos.x > Vertex3[1].pos.x ){
					XTable[1] = &Vertex3[1] ;
					XTable[2] = &Vertex3[2] ;
				}else{
					XTable[1] = &Vertex3[2] ;
					XTable[2] = &Vertex3[1] ;
				}
			}
		}

		// YTable �� y �̏��������̃A�h���X��������
		if( Vertex3[0].pos.y > Vertex3[1].pos.y ){
			if( Vertex3[1].pos.y > Vertex3[2].pos.y ){
				YTable[0] = &Vertex3[2] ;
				YTable[1] = &Vertex3[1] ;
				YTable[2] = &Vertex3[0] ;
			}else{
				YTable[0] = &Vertex3[1] ;
				if( Vertex3[2].pos.y > Vertex3[0].pos.y ){
					YTable[1] = &Vertex3[0] ;
					YTable[2] = &Vertex3[2] ;
				}else{
					YTable[1] = &Vertex3[2] ;
					YTable[2] = &Vertex3[0] ;
				}
			}
		}else{
			if( Vertex3[0].pos.y > Vertex3[2].pos.y ){
				YTable[0] = &Vertex3[2] ;
				YTable[1] = &Vertex3[0] ;
				YTable[2] = &Vertex3[1] ;
			}else{
				YTable[0] = &Vertex3[0] ;
				if( Vertex3[2].pos.y > Vertex3[1].pos.y ){
					YTable[1] = &Vertex3[1] ;
					YTable[2] = &Vertex3[2] ;
				}else{
					YTable[1] = &Vertex3[2] ;
					YTable[2] = &Vertex3[1] ;
				}
			}
		}

		// ZTable �� z �̏��������̃A�h���X��������
		if( Vertex3[0].pos.z > Vertex3[1].pos.z ){
			if( Vertex3[1].pos.z > Vertex3[2].pos.z ){
				ZTable[0] = &Vertex3[2] ;
				ZTable[1] = &Vertex3[1] ;
				ZTable[2] = &Vertex3[0] ;
			}else{
				ZTable[0] = &Vertex3[1] ;
				if( Vertex3[2].pos.z > Vertex3[0].pos.z ){
					ZTable[1] = &Vertex3[0] ;
					ZTable[2] = &Vertex3[2] ;
				}else{
					ZTable[1] = &Vertex3[2] ;
					ZTable[2] = &Vertex3[0] ;
				}
			}
		}else{
			if( Vertex3[0].pos.z > Vertex3[2].pos.z ){
				ZTable[0] = &Vertex3[2] ;
				ZTable[1] = &Vertex3[0] ;
				ZTable[2] = &Vertex3[1] ;
			}else{
				ZTable[0] = &Vertex3[0] ;
				if( Vertex3[2].pos.z > Vertex3[1].pos.z ){
					ZTable[1] = &Vertex3[1] ;
					ZTable[2] = &Vertex3[2] ;
				}else{
					ZTable[1] = &Vertex3[2] ;
					ZTable[2] = &Vertex3[1] ;
				}
			}
		}

		// ���S�ɉ�ʊO�ɏo�Ă��Ȃ������ׂ�
		if( XTable[2]->pos.x < (float)MemImgManage.DrawArea.left || XTable[0]->pos.x >= (float)MemImgManage.DrawArea.right ||
			YTable[2]->pos.y < (float)MemImgManage.DrawArea.top  || YTable[0]->pos.y >= (float)MemImgManage.DrawArea.bottom ||
			ZTable[2]->pos.z < 0.0F                              || ZTable[0]->pos.z >= 1.0F )
			goto END ;

		// �T�C�Y���Q�h�b�g�ȉ��������牽�������ɏI��
		if( ( YTable[2]->pos.y - YTable[0]->pos.y < 1.1F ) ||
			( XTable[2]->pos.x - XTable[0]->pos.x < 1.1F ) ) goto END ;

		// �X�N���[�����W����UV���W�ARHW�ARGBA �����߂�ׂ̌v�Z�l���Z�o����
		// Vx = V1x * p1 + V2x * p2 ;
		// Vy = V1y * p1 + V2y * p2 ;
		// Tu = T1u * p1 + T2u * p2 ;
		// Tv = T1v * p1 + T2v * p2 ;
		// ���̎����� Vx �� Vy ����e�l�ɕϊ��ł���Q�l���Z�o���Ă��܂��B
		// �S�� px20 * py10 - py20 * px10 ----(1) �Ŋ����Ă���̂́A�Е���
		// �{���Ȃ� py10 * px20 - px10 * py20 ----(2) �Ȃ̂ł����A�ǂ����
		// �O�ς̌v�Z�ɓ������A���x�v���X�}�C�i�X���Ⴄ�����Ȃ̂� (2) ��
		// �����Ă��镔���̕�����S�Ĕ��]���邱�ƂŁA�S�Ă̏��Z�� (1) �ōς܂��Ă��܂�
		{
			float tmp ;
			float px20, py20, px10, py10 ;
			float tu20, tu10, tv20, tv10 ;
			float rhw20, rhw10 ;
			float z20, z10 ;
			float r20, r10, g20, g10, b20, b10, a20, a10 ;
			
			px20 = XTable[2]->pos.x - XTable[0]->pos.x ;
			py20 = XTable[2]->pos.y - XTable[0]->pos.y ;
			px10 = XTable[1]->pos.x - XTable[0]->pos.x ;
			py10 = XTable[1]->pos.y - XTable[0]->pos.y ;
			
			tu20  = ( XTable[2]->u * XTable[2]->rhw ) - ( XTable[0]->u * XTable[0]->rhw ) ;
			tu10  = ( XTable[1]->u * XTable[1]->rhw ) - ( XTable[0]->u * XTable[0]->rhw ) ;
			tv20  = ( XTable[2]->v * XTable[2]->rhw ) - ( XTable[0]->v * XTable[0]->rhw ) ;
			tv10  = ( XTable[1]->v * XTable[1]->rhw ) - ( XTable[0]->v * XTable[0]->rhw ) ;
			rhw20 = XTable[2]->rhw   - XTable[0]->rhw ;
			rhw10 = XTable[1]->rhw   - XTable[0]->rhw ;
			z20   = XTable[2]->pos.z - XTable[0]->pos.z ;
			z10   = XTable[1]->pos.z - XTable[0]->pos.z ;
			r20   = (float)( ((BYTE *)&XTable[2]->color)[2] - ((BYTE *)&XTable[0]->color)[2] ) ;
			r10   = (float)( ((BYTE *)&XTable[1]->color)[2] - ((BYTE *)&XTable[0]->color)[2] ) ;
			g20   = (float)( ((BYTE *)&XTable[2]->color)[1] - ((BYTE *)&XTable[0]->color)[1] ) ;
			g10   = (float)( ((BYTE *)&XTable[1]->color)[1] - ((BYTE *)&XTable[0]->color)[1] ) ;
			b20   = (float)( ((BYTE *)&XTable[2]->color)[0] - ((BYTE *)&XTable[0]->color)[0] ) ;
			b10   = (float)( ((BYTE *)&XTable[1]->color)[0] - ((BYTE *)&XTable[0]->color)[0] ) ;
			a20   = (float)( ((BYTE *)&XTable[2]->color)[3] - ((BYTE *)&XTable[0]->color)[3] ) ;
			a10   = (float)( ((BYTE *)&XTable[1]->color)[3] - ((BYTE *)&XTable[0]->color)[3] ) ;

			tmp = px10 * py20 - px20 * py10;
			if( tmp == 0 ) goto END ;

			tmp   = 1.0F / tmp ;
			tuPX  = (tu10  * py20 - tu20  * py10) * tmp ;	tuPY  = (tu20  * px10 - tu10  * px20) * tmp ;
			tvPX  = (tv10  * py20 - tv20  * py10) * tmp ;	tvPY  = (tv20  * px10 - tv10  * px20) * tmp ;
			rhwPX = (rhw10 * py20 - rhw20 * py10) * tmp ;	rhwPY = (rhw20 * px10 - rhw10 * px20) * tmp ;
			zPX   = (z10   * py20 - z20   * py10) * tmp ;	zPY   = (z20   * px10 - z10   * px20) * tmp ;
			rPX   = (r10   * py20 - r20   * py10) * tmp ;	rPY   = (r20   * px10 - r10   * px20) * tmp ;
			gPX   = (g10   * py20 - g20   * py10) * tmp ;	gPY   = (g20   * px10 - g10   * px20) * tmp ;
			bPX   = (b10   * py20 - b20   * py10) * tmp ;	bPY   = (b20   * px10 - b10   * px20) * tmp ;
			aPX   = (a10   * py20 - a20   * py10) * tmp ;	aPY   = (a20   * px10 - a10   * px20) * tmp ;

			px0  = XTable[0]->pos.x ;
			py0  = XTable[0]->pos.y ;
			tu0  = ( XTable[0]->u * XTable[0]->rhw ) - tuPX * px0 - tuPY * py0 ;
			tv0  = ( XTable[0]->v * XTable[0]->rhw ) - tvPX * px0 - tvPY * py0 ;
			rhw0 = XTable[0]->rhw   - rhwPX * px0 - rhwPY * py0 ;
			z0   = XTable[0]->pos.z - zPX   * px0 - zPY   * py0 ;
			r0   = ((BYTE *)&XTable[0]->color)[2] - rPX * px0 - rPY * py0 ;
			g0   = ((BYTE *)&XTable[0]->color)[1] - gPX * px0 - gPY * py0 ;
			b0   = ((BYTE *)&XTable[0]->color)[0] - bPX * px0 - bPY * py0 ;
			a0   = ((BYTE *)&XTable[0]->color)[3] - aPX * px0 - aPY * py0 ;
		}

		// �C���M�����[�p����
		if( _FTOL( YTable[0]->pos.y ) == _FTOL( YTable[1]->pos.y ) )
		{
			// �㕔�Q���_�̂x�����������ꍇ�̏���
			DMODEL = DMODER = 1 ;

			// ��Q���_�̂ǂ��炪�����łǂ��炪�E�������ׂ�
			if( YTable[0]->pos.x > YTable[1]->pos.x )
			{
				LP = YTable[1] ;
				RP = YTable[0] ;
			}
			else
			{
				LP = YTable[0] ;
				RP = YTable[1] ;
			}

			// ���E�̒��_�̊J�n���W���Z�b�g
			StartPLX[1] = LP->pos.x ; StartPLY[1] = LP->pos.y ;
			StartPRX[1] = RP->pos.x ; StartPRY[1] = RP->pos.y ;

			// ���E�̒��_�̂P���C�����Ƃ̂w���W�̉��Z�l���Z�o
			AddPLX[1] = ( YTable[2]->pos.x - StartPLX[1] ) / ( YTable[2]->pos.y - StartPLY[1] ) ;
			AddPRX[1] = ( YTable[2]->pos.x - StartPRX[1] ) / ( YTable[2]->pos.y - StartPRY[1] ) ;
		}
		else
		// �����Q���_�̂x�����������ꍇ�̏���
		if( _FTOL( YTable[1]->pos.y ) == _FTOL( YTable[2]->pos.y ) )
		{
			DMODEL = DMODER = 1 ;

			// �����J�n���W�͍��E�Ƃ������ɂȂ�
			StartPLX[1] = StartPRX[1] = YTable[0]->pos.x ;
			StartPLY[1] = StartPRY[1] = YTable[0]->pos.y ;

			// �����̂Q���_���ǂ��炪�����łǂ��炪�E�������ׂ�
			if( YTable[1]->pos.x > YTable[2]->pos.x )
			{
				LP = YTable[2] ;
				RP = YTable[1] ;
			}
			else
			{
				LP = YTable[1] ;
				RP = YTable[2] ;
			}

			// ���E�̒��_�̂P���C�����Ƃ̂w���W�̉��Z�l���Z�o
			AddPLX[1] = ( LP->pos.x - StartPLX[1] ) / ( LP->pos.y - StartPLY[1] ) ;
			AddPRX[1] = ( RP->pos.x - StartPRX[1] ) / ( RP->pos.y - StartPRY[1] ) ;
		}
		else
		// ����ȃP�[�X�ł͂Ȃ��ꍇ
		{
			// �r���ɒ��p�_��ʂ�͍̂������A�E�����ɂ���ď����𕪊�
			if( ( YTable[1]->pos.x - YTable[0]->pos.x ) / ( YTable[1]->pos.y - YTable[0]->pos.y ) <
				( YTable[2]->pos.x - YTable[0]->pos.x ) / ( YTable[2]->pos.y - YTable[0]->pos.y ) ) 
			{
				// ���p�_��ʂ�͍̂���
				DMODEL = 0 ;
				DMODER = 1 ;

				// ���E�̊J�n�_�͂ǂ������ԏ�̒��_
				StartPLX[0] = StartPRX[1] = YTable[0]->pos.x ;
				StartPLY[0] = StartPRY[1] = YTable[0]->pos.y ;

				// �����̐��̂P���C�����Ƃɉ��Z����w���W�l���Z�o
				AddPLX[0] = ( YTable[1]->pos.x - StartPLX[0] ) / ( YTable[1]->pos.y - StartPLY[0] ) ;

				// �E���̑B�̂P���C�����Ƃɉ��Z����w���W�l���Z�o
				AddPRX[1] = ( YTable[2]->pos.x - StartPRX[1] ) / ( YTable[2]->pos.y - StartPRY[1] ) ;

				
				// �����������p�_��ʂ�̂ŁA���p�_����I�_�Ɍ��������̊J�n�_���Z�b�g
				StartPLX[1] = YTable[1]->pos.x ;
				StartPLY[1] = YTable[1]->pos.y ;

				// �����̒��p�_����I�_�֌��������̂P���C�����Ƃɉ��Z����w���W�l���Z�o
				AddPLX[1] = ( YTable[2]->pos.x - StartPLX[1] ) / ( YTable[2]->pos.y - StartPLY[1] ) ;
			}
			else
			{
				// ���p�_��ʂ�͉̂E��
				DMODEL = 1 ;
				DMODER = 0 ;

				// ���E�̊J�n�_�͂ǂ������ԏ�̒��_
				StartPLX[1] = StartPRX[0] = YTable[0]->pos.x ;
				StartPLY[1] = StartPRY[0] = YTable[0]->pos.y ;

				// �E���̑B�̂P���C�����Ƃɉ��Z����w���W�l���Z�o
				AddPRX[0] = ( YTable[1]->pos.x - StartPRX[0] ) / ( YTable[1]->pos.y - StartPRY[0] ) ;

				// �����̐��̂P���C�����Ƃɉ��Z����w���W�l���Z�o
				AddPLX[1] = ( YTable[2]->pos.x - StartPLX[1] ) / ( YTable[2]->pos.y - StartPLY[1] ) ;

				
				// �E���������p�_��ʂ�̂ŁA���p�_����I�_�Ɍ��������̊J�n�_���Z�b�g
				StartPRX[1] = YTable[1]->pos.x + 1 ;
				StartPRY[1] = YTable[1]->pos.y ;

				// �E���̒��p�_����I�_�֌��������̂P���C�����Ƃɉ��Z����w���W�l���Z�o
				AddPRX[1] = ( YTable[2]->pos.x - StartPRX[1] ) / ( YTable[2]->pos.y - StartPRY[1] ) ;
			}
		}
		
		// �`�揈�����J�n����x���W�ƁA�e���C���ł̕`��J�n�w���W�A�`��h�b�g�����Z�o
		{
			int y, lx, rx, addLX, addRX, endY, cly, cry ;
			
			// �`��J�n�x���W���Z�o
			StartY = ( _FTOL( YTable[0]->pos.y * QUALI ) + QUALI / 2 ) / QUALI ;
			endY   = _FTOL( YTable[2]->pos.y * QUALI ) ;

			// ��ʂ���͂ݏo���Ă�����␳
			if( StartY < MemImgManage.DrawArea.top ) StartY = MemImgManage.DrawArea.top ;
			if( endY > MemImgManage.DrawArea.bottom * QUALI ) endY = MemImgManage.DrawArea.bottom * QUALI ;

			// �����x���W�̃Z�b�g
			y      = StartY * QUALI + QUALI / 2 ;

			// ���E�̃��C���̏������W�Ɖ��Z���W���Z�b�g
			lx     = _FTOL( ( StartPLX[DMODEL] + ( ( (float)StartY + 0.5F ) - StartPLY[DMODEL] ) * AddPLX[DMODEL] ) * QUALI ) ;
			rx     = _FTOL( ( StartPRX[DMODER] + ( ( (float)StartY + 0.5F ) - StartPLY[DMODEL] ) * AddPRX[DMODER] ) * QUALI ) ;
			addLX  = _FTOL( AddPLX[DMODEL]    * QUALI ) ;
			addRX  = _FTOL( AddPRX[DMODER]    * QUALI ) ;

			// ���p�_�l���Z�b�g( ���̒��p�_�͏I�_�̏ꍇ�͕K�����B���Ȃ��l(4096 * QUALI)���Z�b�g���Ă��� )
			cly = ( DMODEL == 0 ) ? _FTOL( StartPLY[1] * QUALI ) : 4096 * QUALI ;
			cry = ( DMODER == 0 ) ? _FTOL( StartPRY[1] * QUALI ) : 4096 * QUALI ;

			NumY = 0 ;
			while( endY > y )
			{
				// �`��J�n�w���W�ƁA�`��h�b�g�����Z�o
				StartX_NumX[NumY][0] = (short)( ( lx + QUALI / 2 ) / QUALI ) ;
				StartX_NumX[NumY][1] = (short)( ( rx - QUALI / 2 ) / QUALI - StartX_NumX[NumY][0] + 1 ) ;

				// ��ʂ���͂ݏo���Ă�����␳
				if( StartX_NumX[NumY][0] < MemImgManage.DrawArea.left )
				{
					StartX_NumX[NumY][1] -= (short)( MemImgManage.DrawArea.left - StartX_NumX[NumY][0] ) ;
					StartX_NumX[NumY][0] = (short)( MemImgManage.DrawArea.left ) ;
				}
				if( StartX_NumX[NumY][0] + StartX_NumX[NumY][1] > MemImgManage.DrawArea.right )
				{
					StartX_NumX[NumY][1] = (short)( MemImgManage.DrawArea.right - StartX_NumX[NumY][0] ) ;
				}
				
				// �����`�悷��h�b�g�����O�������ꍇ�̓L�����Z��
				if( StartX_NumX[NumY][1] <= 0 ) StartX_NumX[NumY][1] = 0 ;
				NumY ++ ;
				
				rx += addRX ;
				lx += addLX ;

				y += QUALI ;

				// ���������̃��C�������p�_�𒴂��Ă����瑖�����郉�C����ύX����
				if( cly <= y )
				{
					// ���E�̃��C���̏������W�Ɖ��Z���W���Z�b�g
					lx     = _FTOL( ( StartPLX[1] + ( (float)y / QUALI - StartPLY[1] ) * AddPLX[1] ) * QUALI ) ;
					addLX  = _FTOL( AddPLX[1] * QUALI ) ;

					// ���̒��p�_�͖����̂ŕK�����B���Ȃ��l(4096 * QUALI)���Z�b�g���Ă���
					cly = 4096 * QUALI ;
				}
				
				// �����E���̃��C�������p�_�𒴂��Ă����瑖�����郉�C����ύX����
				if( cry <= y )
				{
					// ���E�̃��C���̏������W�Ɖ��Z���W���Z�b�g
					rx     = _FTOL( ( StartPRX[1] + ( (float)y / QUALI - StartPRY[1] ) * AddPRX[1] ) * QUALI ) ;
					addRX  = _FTOL( AddPRX[1] * QUALI ) ;

					// ���̒��p�_�͖����̂ŕK�����B���Ȃ��l(4096 * QUALI)���Z�b�g���Ă���
					cry = 4096 * QUALI ;
				}
			}
		}
	}

	// �]�����A�]����̃A�h���X���Z�b�g
	if( SrcImg != NULL )
	{
		SrcBP     = SrcImg->UseImage ;
		SrcPitch  = SrcImg->Base->Pitch ;
	}

	DestBP    = DestImg->UseImage ;
	DestPitch = DestImg->Base->Pitch ;
	
	if( ZImg != NULL )
	{
		ZBP        = ZImg->UseImage ;
		ZPitch     = ZImg->Base->Pitch ;
	}

	if( BlendImg != NULL )
	{
		BlendBP    = BlendImg->UseImage ;
		BlendPitch = BlendImg->Base->Pitch ;
	}

	PaletteBP  = (BYTE *)SrcImg->Base->Palette ;
	TransColor = SrcImg->Base->TransColor ;
	RateTable1 = MemImgManage.RateTable[MemImgManage.BlendParam] ;
	RateTable2 = &MemImgManage.RateTable2[MemImgManage.BlendParam][256] ;
	RedTable   = MemImgManage.RateTable[MemImgManage.DrawBright.Red] ;
	GreenTable = MemImgManage.RateTable[MemImgManage.DrawBright.Green] ;
	BlueTable  = MemImgManage.RateTable[MemImgManage.DrawBright.Blue] ;

	// �`�揀��
	{
		DrawRectRight  = MemImgManage.DrawArea.right ;
		DrawRectLeft   = MemImgManage.DrawArea.left ;
		DrawRectBottom = MemImgManage.DrawArea.bottom ;
		DrawRectTop    = MemImgManage.DrawArea.top ;

		RHWPX = _DTOL( (double)rhwPX * RHWQUALI ) ;
		RHWPY = _DTOL( (double)rhwPY * RHWQUALI ) ;
		RHW0  = _DTOL( (double)rhw0  * RHWQUALI ) ;

		UPX = _DTOL( (double)tuPX * UVQUALI ) ;
		UPY = _DTOL( (double)tuPY * UVQUALI ) ;
		U0  = _DTOL( (double)tu0  * UVQUALI ) ;

		VPX = _DTOL( (double)tvPX * UVQUALI ) ;
		VPY = _DTOL( (double)tvPY * UVQUALI ) ;
		V0  = _DTOL( (double)tv0  * UVQUALI ) ;
		
		ZPX = _DTOL( (double)zPX * ZQUALI ) ;
		ZPY = _DTOL( (double)zPY * ZQUALI ) ;
		Z0  = _DTOL( (double)z0  * ZQUALI ) ;

		RPX = _DTOL( (double)rPX * COLORQUALI ) ;
		RPY = _DTOL( (double)rPY * COLORQUALI ) ;
		R0  = _DTOL( (double)r0  * COLORQUALI ) ;

		GPX = _DTOL( (double)gPX * COLORQUALI ) ;
		GPY = _DTOL( (double)gPY * COLORQUALI ) ;
		G0  = _DTOL( (double)g0  * COLORQUALI ) ;

		BPX = _DTOL( (double)bPX * COLORQUALI ) ;
		BPY = _DTOL( (double)bPY * COLORQUALI ) ;
		B0  = _DTOL( (double)b0  * COLORQUALI ) ;

		APX = _DTOL( (double)aPX * COLORQUALI ) ;
		APY = _DTOL( (double)aPY * COLORQUALI ) ;
		A0  = _DTOL( (double)a0  * COLORQUALI ) ;

		RHWT = RHW0 + RHWPY * StartY + RHWPY / 2 ;
		UT = U0 + UPY * StartY + UPY / 2 ;
		VT = V0 + VPY * StartY + VPY / 2 ;
		ZT = Z0 + ZPY * StartY + ZPY / 2 ;
		RT = R0 + RPY * StartY + RPY / 2 ;
		GT = G0 + GPY * StartY + GPY / 2 ;
		BT = B0 + BPY * StartY + BPY / 2 ;
		AT = A0 + APY * StartY + APY / 2 ;
		ZBufPT = ZBP    + StartY * ZPitch ;
		DestPT = DestBP + StartY * DestPitch ;

		texWidth  = ( int )SrcImg->Width  ;
		texHeight = ( int )SrcImg->Height ;
	}
	
	// �O���[�V�F�[�f�B���O�̏ꍇ�Ƃ����łȂ��ꍇ�ŏ����𕪊�
	if( GouraudShadeMode == FALSE )
	{
		// �O���[�V�F�[�f�B���O�ł͂Ȃ��ꍇ

		// �J���[���[�h�ɂ���ď����𕪊�
		switch( SrcImg->Base->ColorType )
		{
		case 0 :	// 16bit ���[�h
			if( SrcImg->Base->UsePalette == 1 )
			// �p���b�g������ꍇ
			{
				// �u�����h���[�h�ɂ���ď����𕪊�
				switch( MemImgManage.BlendMode )
				{
				case DX_BLENDMODE_NOBLEND :		// �u�����h����
	NOMALDRAW_C16_USEPAL_BNO:
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C16_NBR_NAC_BNO( PAL16, DST16, DSTP16, BLND ), 1, 2 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C16_NBR_NAC_BNO( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C16_UBR_NAC_BNO( PAL16, DST16, DSTP16, BLND ), 1, 2 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C16_UBR_NAC_BNO( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C16_NBR_NAC_BNO( PAL16, DST16, DSTP16 ), 1, 2 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C16_NBR_NAC_BNO( PAL16, DST16, DSTP16 ), 1, 2 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C16_UBR_NAC_BNO( PAL16, DST16, DSTP16 ), 1, 2 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C16_UBR_NAC_BNO( PAL16, DST16, DSTP16 ), 1, 2 )
						}
					}
					break ;
					
				case DX_BLENDMODE_ALPHA :		// ���u�����h
					if( MemImgManage.BlendParam == 255 ) goto NOMALDRAW_C16_USEPAL_BNO ;
					else
					if( MemImgManage.BlendParam == 0 ) goto END ;
					
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C16_NBR_NAC_BAL( PAL16, DST16, DSTP16, BLND ), 1, 2 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C16_NBR_NAC_BAL( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C16_UBR_NAC_BAL( PAL16, DST16, DSTP16, BLND ), 1, 2 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C16_UBR_NAC_BAL( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C16_NBR_NAC_BAL( PAL16, DST16, DSTP16 ), 1, 2 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C16_NBR_NAC_BAL( PAL16, DST16, DSTP16 ), 1, 2 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C16_UBR_NAC_BAL( PAL16, DST16, DSTP16 ), 1, 2 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C16_UBR_NAC_BAL( PAL16, DST16, DSTP16 ), 1, 2 )
						}
					}
					break ;
					
				case DX_BLENDMODE_ADD :			// ���Z�u�����h
					if( MemImgManage.BlendParam == 0 ) goto END ;
					
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C16_NBR_NAC_BAD( PAL16, DST16, DSTP16, BLND ), 1, 2 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C16_NBR_NAC_BAD( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C16_UBR_NAC_BAD( PAL16, DST16, DSTP16, BLND ), 1, 2 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C16_UBR_NAC_BAD( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C16_NBR_NAC_BAD( PAL16, DST16, DSTP16 ), 1, 2 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C16_NBR_NAC_BAD( PAL16, DST16, DSTP16 ), 1, 2 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C16_UBR_NAC_BAD( PAL16, DST16, DSTP16 ), 1, 2 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C16_UBR_NAC_BAD( PAL16, DST16, DSTP16 ), 1, 2 )
						}
					}
					break ;

				case DX_BLENDMODE_SUB :			// ���Z�u�����h
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C16_NBR_NAC_BSB( PAL16, DST16, DSTP16, BLND ), 1, 2 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C16_NBR_NAC_BSB( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C16_UBR_NAC_BSB( PAL16, DST16, DSTP16, BLND ), 1, 2 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C16_UBR_NAC_BSB( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C16_NBR_NAC_BSB( PAL16, DST16, DSTP16 ), 1, 2 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C16_NBR_NAC_BSB( PAL16, DST16, DSTP16 ), 1, 2 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C16_UBR_NAC_BSB( PAL16, DST16, DSTP16 ), 1, 2 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C16_UBR_NAC_BSB( PAL16, DST16, DSTP16 ), 1, 2 )
						}
					}
					break ;
					
				case DX_BLENDMODE_MUL :			// ��Z�u�����h
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C16_NBR_NAC_BML( PAL16, DST16, DSTP16, BLND ), 1, 2 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C16_NBR_NAC_BML( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C16_UBR_NAC_BML( PAL16, DST16, DSTP16, BLND ), 1, 2 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C16_UBR_NAC_BML( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C16_NBR_NAC_BML( PAL16, DST16, DSTP16 ), 1, 2 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C16_NBR_NAC_BML( PAL16, DST16, DSTP16 ), 1, 2 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C16_UBR_NAC_BML( PAL16, DST16, DSTP16 ), 1, 2 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C16_UBR_NAC_BML( PAL16, DST16, DSTP16 ), 1, 2 )
						}
					}
					break ;

				case DX_BLENDMODE_MULA :		// ��Z+���u�����h
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C16_NBR_NAC_BMA( PAL16, DST16, DSTP16, BLND ), 1, 2 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C16_NBR_NAC_BMA( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C16_UBR_NAC_BMA( PAL16, DST16, DSTP16, BLND ), 1, 2 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C16_UBR_NAC_BMA( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C16_NBR_NAC_BMA( PAL16, DST16, DSTP16 ), 1, 2 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C16_NBR_NAC_BMA( PAL16, DST16, DSTP16 ), 1, 2 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C16_UBR_NAC_BMA( PAL16, DST16, DSTP16 ), 1, 2 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C16_UBR_NAC_BMA( PAL16, DST16, DSTP16 ), 1, 2 )
						}
					}
					break ;

				case DX_BLENDMODE_INVSRC :		// �`�挳�̐F�𔽓]�{���u�����h
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C16_NBR_NAC_BIS( PAL16, DST16, DSTP16, BLND ), 1, 2 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C16_NBR_NAC_BIS( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C16_UBR_NAC_BIS( PAL16, DST16, DSTP16, BLND ), 1, 2 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C16_UBR_NAC_BIS( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C16_NBR_NAC_BIS( PAL16, DST16, DSTP16 ), 1, 2 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C16_NBR_NAC_BIS( PAL16, DST16, DSTP16 ), 1, 2 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C16_UBR_NAC_BIS( PAL16, DST16, DSTP16 ), 1, 2 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C16_UBR_NAC_BIS( PAL16, DST16, DSTP16 ), 1, 2 )
						}
					}
					break ;
				}
			}
			else
			// �p���b�g�������ꍇ
			{
				// �u�����h���[�h�ɂ���ď����𕪊�
				switch( MemImgManage.BlendMode )
				{
				case DX_BLENDMODE_NOBLEND :		// �u�����h����
	NOMALDRAW_C16_NOPAL_BNO:
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_UAC_BNO_ACK( SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_UAC_BNO(     SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_NAC_BNO_TCK( SRC16, DST16, DSTP16, BLND ), 2, 2 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_NAC_BNO(     SRC16, DST16, DSTP16, BLND ), 2, 2 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BNO_ACK( SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BNO(     SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BNO_TCK( SRC16, DST16, DSTP16, BLND ), 2, 2 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BNO(     SRC16, DST16, DSTP16, BLND ), 2, 2 )
							}
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_UAC_BNO_ACK( SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_UAC_BNO(     SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_NAC_BNO_TCK( SRC16, DST16, DSTP16 ), 2, 2 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BNO( SRC16, DST16, DSTP16 ), 2, 2 )
/*								else
								{
									y = StartY ;
									ycon = 0 ;
									for( ; ycon < NumY ; ycon ++, y ++,
										 RHWT += RHWPY,
										 UT += UPY,
										 VT += VPY,
										 ZT += ZPY,
										 RT += RPY,
										 GT += GPY,
										 BT += BPY,
										 AT += APY,
										 ZBufPT += ZPitch,
										 DestPT += DestPitch
									 )
									{
										xcon = StartX_NumX[ycon][1] ;
										if( xcon == 0 ) continue ;
										x = StartX_NumX[ycon][0] ;

										RHW = RHWPX * x + ( RHWPX >> 1 ) + RHWT ;
										Z   = ZPX   * x + ( ZPX   >> 1 ) + ZT ;
										U   = UPX   * x + ( UPX   >> 1 ) + UT ;
										V   = VPX   * x + ( VPX   >> 1 ) + VT ;
										R   = RPX   * x + ( RPX   >> 1 ) + RT ;
										G   = GPX   * x + ( GPX   >> 1 ) + GT ;
										B   = BPX   * x + ( BPX   >> 1 ) + BT ;
										A   = APX   * x + ( APX   >> 1 ) + AT ;
										ZBPT = x * 2 + ZBufPT ;
										DestBPT = x * 2 + DestPT ;
										if( RHW == 0 )
											continue ;

										for( ; xcon ; xcon --,
										 RHW += RHWPX,
										 U += UPX,
										 V += VPX,
										 Z += ZPX,
										 R += RPX,
										 G += GPX,
										 B += BPX,
										 A += APX,
										 ZBPT += 2,
										 DestBPT += 2
										  )
										{
											if( *ZWPT >= (WORD)( Z >> 8 ) )
											{
												*ZWPT = Z >> 8 ;

												INV = (1 << 29) / RHW ;
												tu = ( ( U >> 10 ) * texWidth  * INV ) >> 19 ;
												tv = ( ( V >> 10 ) * texHeight * INV ) >> 19 ;

												if( (DWORD)( ( tu | ( texWidth - tu - 1 ) ) | ( tv | ( texHeight - tv - 1 ) ) ) & 0x80000000 )
													continue ;

												SrcBPT  = SrcBP + tu * 2 + tv * SrcPitch ;

												CODE_NBI_C16_UBR_NAC_BNO_NTBL( SRC16, DST16, DSTP16, (R / COLORQUALI), (G / COLORQUALI), (B / COLORQUALI) )
											}
										}
									}
								}*/
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BNO_ACK( SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BNO(     SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BNO_TCK( SRC16, DST16, DSTP16 ), 2, 2 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BNO(     SRC16, DST16, DSTP16 ), 2, 2 )
							}
						}
					}
					break ;

				case DX_BLENDMODE_ALPHA :		// ���u�����h
					if( MemImgManage.BlendParam == 255 ) goto NOMALDRAW_C16_NOPAL_BNO ;
					else
					if( MemImgManage.BlendParam == 0 ) goto END ;
					
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_UAC_BAL_ACK( SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_UAC_BAL(     SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_NAC_BAL_TCK( SRC16, DST16, DSTP16, BLND ), 2, 2 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_NAC_BAL(     SRC16, DST16, DSTP16, BLND ), 2, 2 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BAL_ACK( SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BAL(     SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BAL_TCK( SRC16, DST16, DSTP16, BLND ), 2, 2 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BAL(     SRC16, DST16, DSTP16, BLND ), 2, 2 )
							}
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_UAC_BAL_ACK( SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_UAC_BAL(     SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_NAC_BAL_TCK( SRC16, DST16, DSTP16 ), 2, 2 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_NAC_BAL(     SRC16, DST16, DSTP16 ), 2, 2 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BAL_ACK( SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BAL(     SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BAL_TCK( SRC16, DST16, DSTP16 ), 2, 2 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BAL(     SRC16, DST16, DSTP16 ), 2, 2 )
							}
						}
					}
					break ;

				case DX_BLENDMODE_ADD :		// ���Z�u�����h
					if( MemImgManage.BlendParam == 0 ) goto END ;
					
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_UAC_BAD_ACK( SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_UAC_BAD(     SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_NAC_BAD_TCK( SRC16, DST16, DSTP16, BLND ), 2, 2 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_NAC_BAD(     SRC16, DST16, DSTP16, BLND ), 2, 2 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BAD_ACK( SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BAD(     SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BAD_TCK( SRC16, DST16, DSTP16, BLND ), 2, 2 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BAD(     SRC16, DST16, DSTP16, BLND ), 2, 2 )
							}
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_UAC_BAD_ACK( SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_UAC_BAD(     SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_NAC_BAD_TCK( SRC16, DST16, DSTP16 ), 2, 2 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_NAC_BAD(     SRC16, DST16, DSTP16 ), 2, 2 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BAD_ACK( SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BAD(     SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BAD_TCK( SRC16, DST16, DSTP16 ), 2, 2 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BAD(     SRC16, DST16, DSTP16 ), 2, 2 )
							}
						}
					}
					break ;

				case DX_BLENDMODE_SUB :		// ���Z�u�����h
					if( MemImgManage.BlendParam == 0 ) goto END ;
					
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_UAC_BSB_ACK( SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_UAC_BSB(     SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_NAC_BSB_TCK( SRC16, DST16, DSTP16, BLND ), 2, 2 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_NAC_BSB(     SRC16, DST16, DSTP16, BLND ), 2, 2 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BSB_ACK( SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BSB(     SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BSB_TCK( SRC16, DST16, DSTP16, BLND ), 2, 2 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BSB(     SRC16, DST16, DSTP16, BLND ), 2, 2 )
							}
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_UAC_BSB_ACK( SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_UAC_BSB(     SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_NAC_BSB_TCK( SRC16, DST16, DSTP16 ), 2, 2 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_NAC_BSB(     SRC16, DST16, DSTP16 ), 2, 2 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BSB_ACK( SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BSB(     SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BSB_TCK( SRC16, DST16, DSTP16 ), 2, 2 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BSB(     SRC16, DST16, DSTP16 ), 2, 2 )
							}
						}
					}
					break ;

				case DX_BLENDMODE_MUL :		// ��Z�u�����h
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_UAC_BML_ACK( SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_UAC_BML(     SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_NAC_BML_TCK( SRC16, DST16, DSTP16, BLND ), 2, 2 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_NAC_BML(     SRC16, DST16, DSTP16, BLND ), 2, 2 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BML_ACK( SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BML(     SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BML_TCK( SRC16, DST16, DSTP16, BLND ), 2, 2 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BML(     SRC16, DST16, DSTP16, BLND ), 2, 2 )
							}
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_UAC_BML_ACK( SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_UAC_BML(     SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_NAC_BML_TCK( SRC16, DST16, DSTP16 ), 2, 2 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_NAC_BML(     SRC16, DST16, DSTP16 ), 2, 2 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BML_ACK( SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BML(     SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BML_TCK( SRC16, DST16, DSTP16 ), 2, 2 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BML(     SRC16, DST16, DSTP16 ), 2, 2 )
							}
						}
					}
					break ;

				case DX_BLENDMODE_MULA :	// ��Z+���u�����h
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_UAC_BMA_ACK( SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_UAC_BMA(     SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_NAC_BMA_TCK( SRC16, DST16, DSTP16, BLND ), 2, 2 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_NAC_BMA(     SRC16, DST16, DSTP16, BLND ), 2, 2 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BMA_ACK( SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BMA(     SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BMA_TCK( SRC16, DST16, DSTP16, BLND ), 2, 2 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BMA(     SRC16, DST16, DSTP16, BLND ), 2, 2 )
							}
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_UAC_BMA_ACK( SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_UAC_BMA(     SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_NAC_BMA_TCK( SRC16, DST16, DSTP16 ), 2, 2 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_NAC_BMA(     SRC16, DST16, DSTP16 ), 2, 2 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BMA_ACK( SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BMA(     SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BMA_TCK( SRC16, DST16, DSTP16 ), 2, 2 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BMA(     SRC16, DST16, DSTP16 ), 2, 2 )
							}
						}
					}
					break ;

				case DX_BLENDMODE_INVSRC :		// �`�挳�̐F�𔽓]�{���u�����h
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_UAC_BIS_ACK( SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_UAC_BIS(     SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_NAC_BIS_TCK( SRC16, DST16, DSTP16, BLND ), 2, 2 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_NAC_BIS(     SRC16, DST16, DSTP16, BLND ), 2, 2 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BIS_ACK( SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BIS(     SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BIS_TCK( SRC16, DST16, DSTP16, BLND ), 2, 2 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BIS(     SRC16, DST16, DSTP16, BLND ), 2, 2 )
							}
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_UAC_BIS_ACK( SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_UAC_BIS(     SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_NAC_BIS_TCK( SRC16, DST16, DSTP16 ), 2, 2 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_NAC_BIS(     SRC16, DST16, DSTP16 ), 2, 2 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BIS_ACK( SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BIS(     SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BIS_TCK( SRC16, DST16, DSTP16 ), 2, 2 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BIS(     SRC16, DST16, DSTP16 ), 2, 2 )
							}
						}
					}
					break ;
				}
			}
			break ;
			
		case 1 :	// 32bit ���[�h
			if( SrcImg->Base->UsePalette == 1 )
			// �p���b�g������ꍇ
			{
				// �u�����h���[�h�ɂ���ď����𕪊�
				switch( MemImgManage.BlendMode )
				{
				case DX_BLENDMODE_NOBLEND :		// �u�����h����
	NOMALDRAW_C32_USEPAL_BNO:
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C32_NBR_NAC_BNO( PALP32, DSTP32, BLND ), 1, 4 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C32_NBR_NAC_BNO( PALP32, DSTP32, BLND ), 1, 4 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C32_UBR_NAC_BNO( PALP32, DSTP32, BLND ), 1, 4 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C32_UBR_NAC_BNO( PALP32, DSTP32, BLND ), 1, 4 )
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C32_NBR_NAC_BNO( PALP32, DSTP32 ), 1, 4 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C32_NBR_NAC_BNO( PALP32, DSTP32 ), 1, 4 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C32_UBR_NAC_BNO( PALP32, DSTP32 ), 1, 4 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C32_UBR_NAC_BNO( PALP32, DSTP32 ), 1, 4 )
						}
					}
					break ;
					
				case DX_BLENDMODE_ALPHA :		// ���u�����h
					if( MemImgManage.BlendParam == 255 ) goto NOMALDRAW_C32_USEPAL_BNO ;
					else
					if( MemImgManage.BlendParam == 0 ) goto END ;
					
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C32_NBR_NAC_BAL( PALP32, DSTP32, BLND ), 1, 4 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C32_NBR_NAC_BAL( PALP32, DSTP32, BLND ), 1, 4 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C32_UBR_NAC_BAL( PALP32, DSTP32, BLND ), 1, 4 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C32_UBR_NAC_BAL( PALP32, DSTP32, BLND ), 1, 4 )
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C32_NBR_NAC_BAL( PALP32, DSTP32 ), 1, 4 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C32_NBR_NAC_BAL( PALP32, DSTP32 ), 1, 4 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C32_UBR_NAC_BAL( PALP32, DSTP32 ), 1, 4 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C32_UBR_NAC_BAL( PALP32, DSTP32 ), 1, 4 )
						}
					}
					break ;
					
				case DX_BLENDMODE_ADD :			// ���Z�u�����h
					if( MemImgManage.BlendParam == 0 ) goto END ;
					
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C32_NBR_NAC_BAD( PALP32, DSTP32, BLND ), 1, 4 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C32_NBR_NAC_BAD( PALP32, DSTP32, BLND ), 1, 4 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C32_UBR_NAC_BAD( PALP32, DSTP32, BLND ), 1, 4 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C32_UBR_NAC_BAD( PALP32, DSTP32, BLND ), 1, 4 )
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C32_NBR_NAC_BAD( PALP32, DSTP32 ), 1, 4 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C32_NBR_NAC_BAD( PALP32, DSTP32 ), 1, 4 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C32_UBR_NAC_BAD( PALP32, DSTP32 ), 1, 4 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C32_UBR_NAC_BAD( PALP32, DSTP32 ), 1, 4 )
						}
					}
					break ;

				case DX_BLENDMODE_SUB :			// ���Z�u�����h
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C32_NBR_NAC_BSB( PALP32, DSTP32, BLND ), 1, 4 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C32_NBR_NAC_BSB( PALP32, DSTP32, BLND ), 1, 4 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C32_UBR_NAC_BSB( PALP32, DSTP32, BLND ), 1, 4 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C32_UBR_NAC_BSB( PALP32, DSTP32, BLND ), 1, 4 )
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C32_NBR_NAC_BSB( PALP32, DSTP32 ), 1, 4 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C32_NBR_NAC_BSB( PALP32, DSTP32 ), 1, 4 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C32_UBR_NAC_BSB( PALP32, DSTP32 ), 1, 4 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C32_UBR_NAC_BSB( PALP32, DSTP32 ), 1, 4 )
						}
					}
					break ;
					
				case DX_BLENDMODE_MUL :			// ��Z�u�����h
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C32_NBR_NAC_BML( PALP32, DSTP32, BLND ), 1, 4 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C32_NBR_NAC_BML( PALP32, DSTP32, BLND ), 1, 4 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C32_UBR_NAC_BML( PALP32, DSTP32, BLND ), 1, 4 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C32_UBR_NAC_BML( PALP32, DSTP32, BLND ), 1, 4 )
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C32_NBR_NAC_BML( PALP32, DSTP32 ), 1, 4 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C32_NBR_NAC_BML( PALP32, DSTP32 ), 1, 4 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C32_UBR_NAC_BML( PALP32, DSTP32 ), 1, 4 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C32_UBR_NAC_BML( PALP32, DSTP32 ), 1, 4 )
						}
					}
					break ;

				case DX_BLENDMODE_MULA :		// ��Z+���u�����h
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C32_NBR_NAC_BMA( PALP32, DSTP32, BLND ), 1, 4 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C32_NBR_NAC_BMA( PALP32, DSTP32, BLND ), 1, 4 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C32_UBR_NAC_BMA( PALP32, DSTP32, BLND ), 1, 4 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C32_UBR_NAC_BMA( PALP32, DSTP32, BLND ), 1, 4 )
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C32_NBR_NAC_BMA( PALP32, DSTP32 ), 1, 4 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C32_NBR_NAC_BMA( PALP32, DSTP32 ), 1, 4 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C32_UBR_NAC_BMA( PALP32, DSTP32 ), 1, 4 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C32_UBR_NAC_BMA( PALP32, DSTP32 ), 1, 4 )
						}
					}
					break ;

				case DX_BLENDMODE_INVSRC :		// �`�挳�̐F�𔽓]�{���u�����h
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C32_NBR_NAC_BIS( PALP32, DSTP32, BLND ), 1, 4 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C32_NBR_NAC_BIS( PALP32, DSTP32, BLND ), 1, 4 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C32_UBR_NAC_BIS( PALP32, DSTP32, BLND ), 1, 4 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C32_UBR_NAC_BIS( PALP32, DSTP32, BLND ), 1, 4 )
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C32_NBR_NAC_BIS( PALP32, DSTP32 ), 1, 4 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C32_NBR_NAC_BIS( PALP32, DSTP32 ), 1, 4 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C32_UBR_NAC_BIS( PALP32, DSTP32 ), 1, 4 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C32_UBR_NAC_BIS( PALP32, DSTP32 ), 1, 4 )
						}
					}
					break ;
				}
			}
			else
			// �p���b�g�������ꍇ
			{
				// �u�����h���[�h�ɂ���ď����𕪊�
				switch( MemImgManage.BlendMode )
				{
				case DX_BLENDMODE_NOBLEND :		// �u�����h����
	NOMALDRAW_C32_NOPAL_BNO:
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_UAC_BNO_ACK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_UAC_BNO(     SRCP32, DSTP32, BLND ), 4, 4 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_NAC_BNO_TCK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_NAC_BNO(     SRCP32, DSTP32, BLND ), 4, 4 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BNO_ACK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BNO(     SRCP32, DSTP32, BLND ), 4, 4 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BNO_TCK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BNO(     SRCP32, DSTP32, BLND ), 4, 4 )
							}
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_UAC_BNO_ACK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_UAC_BNO(     SRCP32, DSTP32 ), 4, 4 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_NAC_BNO_TCK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_NAC_BNO(     SRCP32, DSTP32 ), 4, 4 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BNO_ACK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BNO(     SRCP32, DSTP32 ), 4, 4 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BNO_TCK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BNO(     SRCP32, DSTP32 ), 4, 4 )
							}
						}
					}
					break ;

				case DX_BLENDMODE_ALPHA :		// ���u�����h
					if( MemImgManage.BlendParam == 255 ) goto NOMALDRAW_C32_NOPAL_BNO ;
					else
					if( MemImgManage.BlendParam == 0 ) goto END ;
					
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_UAC_BAL_ACK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_UAC_BAL(     SRCP32, DSTP32, BLND ), 4, 4 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_NAC_BAL_TCK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_NAC_BAL(     SRCP32, DSTP32, BLND ), 4, 4 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BAL_ACK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BAL(     SRCP32, DSTP32, BLND ), 4, 4 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BAL_TCK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BAL(     SRCP32, DSTP32, BLND ), 4, 4 )
							}
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_UAC_BAL_ACK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_UAC_BAL(     SRCP32, DSTP32 ), 4, 4 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_NAC_BAL_TCK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_NAC_BAL(     SRCP32, DSTP32 ), 4, 4 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BAL_ACK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BAL(     SRCP32, DSTP32 ), 4, 4 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BAL_TCK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BAL(     SRCP32, DSTP32 ), 4, 4 )
							}
						}
					}
					break ;

				case DX_BLENDMODE_ADD :		// ���Z�u�����h
					if( MemImgManage.BlendParam == 0 ) goto END ;
					
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_UAC_BAD_ACK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_UAC_BAD(     SRCP32, DSTP32, BLND ), 4, 4 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_NAC_BAD_TCK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_NAC_BAD(     SRCP32, DSTP32, BLND ), 4, 4 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BAD_ACK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BAD(     SRCP32, DSTP32, BLND ), 4, 4 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BAD_TCK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BAD(     SRCP32, DSTP32, BLND ), 4, 4 )
							}
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_UAC_BAD_ACK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_UAC_BAD(     SRCP32, DSTP32 ), 4, 4 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_NAC_BAD_TCK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_NAC_BAD(     SRCP32, DSTP32 ), 4, 4 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BAD_ACK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BAD(     SRCP32, DSTP32 ), 4, 4 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BAD_TCK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BAD(     SRCP32, DSTP32 ), 4, 4 )
							}
						}
					}
					break ;

				case DX_BLENDMODE_SUB :		// ���Z�u�����h
					if( MemImgManage.BlendParam == 0 ) goto END ;
					
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_UAC_BSB_ACK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_UAC_BSB(     SRCP32, DSTP32, BLND ), 4, 4 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_NAC_BSB_TCK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_NAC_BSB(     SRCP32, DSTP32, BLND ), 4, 4 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BSB_ACK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BSB(     SRCP32, DSTP32, BLND ), 4, 4 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BSB_TCK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BSB(     SRCP32, DSTP32, BLND ), 4, 4 )
							}
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_UAC_BSB_ACK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_UAC_BSB(     SRCP32, DSTP32 ), 4, 4 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_NAC_BSB_TCK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_NAC_BSB(     SRCP32, DSTP32 ), 4, 4 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BSB_ACK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BSB(     SRCP32, DSTP32 ), 4, 4 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BSB_TCK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BSB(     SRCP32, DSTP32 ), 4, 4 )
							}
						}
					}
					break ;

				case DX_BLENDMODE_MUL :		// ��Z�u�����h
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_UAC_BML_ACK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_UAC_BML(     SRCP32, DSTP32, BLND ), 4, 4 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_NAC_BML_TCK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_NAC_BML(     SRCP32, DSTP32, BLND ), 4, 4 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BML_ACK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BML(     SRCP32, DSTP32, BLND ), 4, 4 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BML_TCK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BML(     SRCP32, DSTP32, BLND ), 4, 4 )
							}
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_UAC_BML_ACK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_UAC_BML(     SRCP32, DSTP32 ), 4, 4 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_NAC_BML_TCK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_NAC_BML(     SRCP32, DSTP32 ), 4, 4 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BML_ACK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BML(     SRCP32, DSTP32 ), 4, 4 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BML_TCK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BML(     SRCP32, DSTP32 ), 4, 4 )
							}
						}
					}
					break ;

				case DX_BLENDMODE_MULA :	// ��Z+���u�����h
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_UAC_BMA_ACK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_UAC_BMA(     SRCP32, DSTP32, BLND ), 4, 4 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_NAC_BMA_TCK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_NAC_BMA(     SRCP32, DSTP32, BLND ), 4, 4 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BMA_ACK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BMA(     SRCP32, DSTP32, BLND ), 4, 4 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BMA_TCK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BMA(     SRCP32, DSTP32, BLND ), 4, 4 )
							}
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_UAC_BMA_ACK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_UAC_BMA(     SRCP32, DSTP32 ), 4, 4 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_NAC_BMA_TCK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_NAC_BMA(     SRCP32, DSTP32 ), 4, 4 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BMA_ACK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BMA(     SRCP32, DSTP32 ), 4, 4 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BMA_TCK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BMA(     SRCP32, DSTP32 ), 4, 4 )
							}
						}
					}
					break ;

				case DX_BLENDMODE_INVSRC :		// �`�挳�̐F�𔽓]�{���u�����h
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_UAC_BIS_ACK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_UAC_BIS(     SRCP32, DSTP32, BLND ), 4, 4 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_NAC_BIS_TCK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_NAC_BIS(     SRCP32, DSTP32, BLND ), 4, 4 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BIS_ACK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BIS(     SRCP32, DSTP32, BLND ), 4, 4 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BIS_TCK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BIS(     SRCP32, DSTP32, BLND ), 4, 4 )
							}
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_UAC_BIS_ACK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_UAC_BIS(     SRCP32, DSTP32 ), 4, 4 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_NAC_BIS_TCK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_NAC_BIS(     SRCP32, DSTP32 ), 4, 4 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BIS_ACK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BIS(     SRCP32, DSTP32 ), 4, 4 )
							}else{
								if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BIS_TCK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BIS(     SRCP32, DSTP32 ), 4, 4 )
							}
						}
					}
					break ;
				}
			}
			break ;
		}
	}
	else
	{
		// �O���[�V�F�[�f�B���O�̏ꍇ

		// �J���[���[�h�ɂ���ď����𕪊�
		switch( SrcImg->Base->ColorType )
		{
		case 0 :	// 16bit ���[�h
			if( SrcImg->Base->UsePalette == 1 )
			// �p���b�g������ꍇ
			{
				// �u�����h���[�h�ɂ���ď����𕪊�
				switch( MemImgManage.BlendMode )
				{
				case DX_BLENDMODE_NOBLEND :		// �u�����h����
					if( BlendImg != NULL ){
						if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C16_UBR_NAC_BNO_NTBL( PAL16, DST16, DSTP16, BLND, RB, GB, BB ), 1, 2 )
						else				DRAWPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C16_UBR_NAC_BNO_NTBL( PAL16, DST16, DSTP16, BLND, RB, GB, BB ), 1, 2 )
					}else{
						if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C16_UBR_NAC_BNO_NTBL( PAL16, DST16, DSTP16, RB, GB, BB ), 1, 2 )
						else				DRAWPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C16_UBR_NAC_BNO_NTBL( PAL16, DST16, DSTP16, RB, GB, BB ), 1, 2 )
					}
					break ;
					
				case DX_BLENDMODE_ALPHA :		// ���u�����h
					if( BlendImg != NULL ){
						if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C16_UBR_NAC_BAL_NTBL( PAL16, DST16, DSTP16, BLND, AB, RB, GB, BB ), 1, 2 )
						else				DRAWPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C16_UBR_NAC_BAL_NTBL( PAL16, DST16, DSTP16, BLND, AB, RB, GB, BB ), 1, 2 )
					}else{
						if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C16_UBR_NAC_BAL_NTBL( PAL16, DST16, DSTP16, AB, RB, GB, BB ), 1, 2 )
						else				DRAWPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C16_UBR_NAC_BAL_NTBL( PAL16, DST16, DSTP16, AB, RB, GB, BB ), 1, 2 )
					}
					break ;
					
				case DX_BLENDMODE_ADD :			// ���Z�u�����h
					if( BlendImg != NULL ){
						if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C16_UBR_NAC_BAD_NTBL( PAL16, DST16, DSTP16, BLND, AB, RB, GB, BB ), 1, 2 )
						else				DRAWPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C16_UBR_NAC_BAD_NTBL( PAL16, DST16, DSTP16, BLND, AB, RB, GB, BB ), 1, 2 )
					}else{
						if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C16_UBR_NAC_BAD_NTBL( PAL16, DST16, DSTP16, AB, RB, GB, BB ), 1, 2 )
						else				DRAWPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C16_UBR_NAC_BAD_NTBL( PAL16, DST16, DSTP16, AB, RB, GB, BB ), 1, 2 )
					}
					break ;

				case DX_BLENDMODE_SUB :			// ���Z�u�����h
					if( BlendImg != NULL ){
						if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C16_UBR_NAC_BSB_NTBL( PAL16, DST16, DSTP16, BLND, AB, RB, GB, BB ), 1, 2 )
						else				DRAWPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C16_UBR_NAC_BSB_NTBL( PAL16, DST16, DSTP16, BLND, AB, RB, GB, BB ), 1, 2 )
					}else{
						if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C16_UBR_NAC_BSB_NTBL( PAL16, DST16, DSTP16, AB, RB, GB, BB ), 1, 2 )
						else				DRAWPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C16_UBR_NAC_BSB_NTBL( PAL16, DST16, DSTP16, AB, RB, GB, BB ), 1, 2 )
					}
					break ;
					
				case DX_BLENDMODE_MUL :			// ��Z�u�����h
					if( BlendImg != NULL ){
						if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C16_UBR_NAC_BML_NTBL( PAL16, DST16, DSTP16, BLND, RB, GB, BB ), 1, 2 )
						else				DRAWPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C16_UBR_NAC_BML_NTBL( PAL16, DST16, DSTP16, BLND, RB, GB, BB ), 1, 2 )
					}else{
						if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C16_UBR_NAC_BML_NTBL( PAL16, DST16, DSTP16, RB, GB, BB ), 1, 2 )
						else				DRAWPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C16_UBR_NAC_BML_NTBL( PAL16, DST16, DSTP16, RB, GB, BB ), 1, 2 )
					}
					break ;

				case DX_BLENDMODE_MULA :		// ��Z+���u�����h
					if( BlendImg != NULL ){
						if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C16_UBR_NAC_BMA_NTBL( PAL16, DST16, DSTP16, BLND, AB, RB, GB, BB ), 1, 2 )
						else				DRAWPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C16_UBR_NAC_BMA_NTBL( PAL16, DST16, DSTP16, BLND, AB, RB, GB, BB ), 1, 2 )
					}else{
						if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C16_UBR_NAC_BMA_NTBL( PAL16, DST16, DSTP16, AB, RB, GB, BB ), 1, 2 )
						else				DRAWPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C16_UBR_NAC_BMA_NTBL( PAL16, DST16, DSTP16, AB, RB, GB, BB ), 1, 2 )
					}
					break ;

				case DX_BLENDMODE_INVSRC :		// �`�挳�̐F�𔽓]�{���u�����h
					if( BlendImg != NULL ){
						if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C16_UBR_NAC_BIS_NTBL( PAL16, DST16, DSTP16, BLND, AB, RB, GB, BB ), 1, 2 )
						else				DRAWPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C16_UBR_NAC_BIS_NTBL( PAL16, DST16, DSTP16, BLND, AB, RB, GB, BB ), 1, 2 )
					}else{
						if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C16_UBR_NAC_BIS_NTBL( PAL16, DST16, DSTP16, AB, RB, GB, BB ), 1, 2 )
						else				DRAWPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C16_UBR_NAC_BIS_NTBL( PAL16, DST16, DSTP16, AB, RB, GB, BB ), 1, 2 )
					}
					break ;
				}
			}
			else
			// �p���b�g�������ꍇ
			{
				// �u�����h���[�h�ɂ���ď����𕪊�
				switch( MemImgManage.BlendMode )
				{
				case DX_BLENDMODE_NOBLEND :		// �u�����h����
					if( BlendImg != NULL ){
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BNO_ACK_NTBL( SRC16, SRCA16, DST16, DSTP16, BLND, RB, GB, BB ), 4, 2 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BNO_NTBL(     SRC16, SRCA16, DST16, DSTP16, BLND, RB, GB, BB ), 4, 2 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BNO_TCK_NTBL( SRC16, DST16, DSTP16, BLND, RB, GB, BB ), 2, 2 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BNO_NTBL(     SRC16, DST16, DSTP16, BLND, RB, GB, BB ), 2, 2 )
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BNO_ACK_NTBL( SRC16, SRCA16, DST16, DSTP16, RB, GB, BB ), 4, 2 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BNO_NTBL(     SRC16, SRCA16, DST16, DSTP16, RB, GB, BB ), 4, 2 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BNO_TCK_NTBL( SRC16, DST16, DSTP16, RB, GB, BB ), 2, 2 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BNO_NTBL(     SRC16, DST16, DSTP16, RB, GB, BB ), 2, 2 )
						}
					}
					break ;

				case DX_BLENDMODE_ALPHA :		// ���u�����h
					if( BlendImg != NULL ){
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BAL_ACK_NTBL( SRC16, SRCA16, DST16, DSTP16, BLND, AB, RB, GB, BB ), 4, 2 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BAL_NTBL(     SRC16, SRCA16, DST16, DSTP16, BLND, AB, RB, GB, BB ), 4, 2 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BAL_TCK_NTBL( SRC16, DST16, DSTP16, BLND, AB, RB, GB, BB ), 2, 2 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BAL_NTBL(     SRC16, DST16, DSTP16, BLND, AB, RB, GB, BB ), 2, 2 )
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BAL_ACK_NTBL( SRC16, SRCA16, DST16, DSTP16, AB, RB, GB, BB ), 4, 2 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BAL_NTBL(     SRC16, SRCA16, DST16, DSTP16, AB, RB, GB, BB ), 4, 2 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BAL_TCK_NTBL( SRC16, DST16, DSTP16, AB, RB, GB, BB ), 2, 2 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BAL_NTBL(     SRC16, DST16, DSTP16, AB, RB, GB, BB ), 2, 2 )
						}
					}
					break ;

				case DX_BLENDMODE_ADD :		// ���Z�u�����h
					if( BlendImg != NULL ){
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BAD_ACK_NTBL( SRC16, SRCA16, DST16, DSTP16, BLND, AB, RB, GB, BB ), 4, 2 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BAD_NTBL(     SRC16, SRCA16, DST16, DSTP16, BLND, AB, RB, GB, BB ), 4, 2 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BAD_TCK_NTBL( SRC16, DST16, DSTP16, BLND, AB, RB, GB, BB ), 2, 2 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BAD_NTBL(     SRC16, DST16, DSTP16, BLND, AB, RB, GB, BB ), 2, 2 )
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BAD_ACK_NTBL( SRC16, SRCA16, DST16, DSTP16, AB, RB, GB, BB ), 4, 2 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BAD_NTBL(     SRC16, SRCA16, DST16, DSTP16, AB, RB, GB, BB ), 4, 2 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BAD_TCK_NTBL( SRC16, DST16, DSTP16, AB, RB, GB, BB ), 2, 2 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BAD_NTBL(     SRC16, DST16, DSTP16, AB, RB, GB, BB ), 2, 2 )
						}
					}
					break ;

				case DX_BLENDMODE_SUB :		// ���Z�u�����h
					if( BlendImg != NULL ){
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BSB_ACK_NTBL( SRC16, SRCA16, DST16, DSTP16, BLND, AB, RB, GB, BB ), 4, 2 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BSB_NTBL(     SRC16, SRCA16, DST16, DSTP16, BLND, AB, RB, GB, BB ), 4, 2 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BSB_TCK_NTBL( SRC16, DST16, DSTP16, BLND, AB, RB, GB, BB ), 2, 2 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BSB_NTBL(     SRC16, DST16, DSTP16, BLND, AB, RB, GB, BB ), 2, 2 )
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BSB_ACK_NTBL( SRC16, SRCA16, DST16, DSTP16, AB, RB, GB, BB ), 4, 2 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BSB_NTBL(     SRC16, SRCA16, DST16, DSTP16, AB, RB, GB, BB ), 4, 2 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BSB_TCK_NTBL( SRC16, DST16, DSTP16, AB, RB, GB, BB ), 2, 2 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BSB_NTBL(     SRC16, DST16, DSTP16, AB, RB, GB, BB ), 2, 2 )
						}
					}
					break ;

				case DX_BLENDMODE_MUL :		// ��Z�u�����h
					if( BlendImg != NULL ){
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BML_ACK_NTBL( SRC16, SRCA16, DST16, DSTP16, BLND, RB, GB, BB ), 4, 2 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BML_NTBL(     SRC16, SRCA16, DST16, DSTP16, BLND, RB, GB, BB ), 4, 2 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BML_TCK_NTBL( SRC16, DST16, DSTP16, BLND, RB, GB, BB ), 2, 2 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BML_NTBL(     SRC16, DST16, DSTP16, BLND, RB, GB, BB ), 2, 2 )
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BML_ACK_NTBL( SRC16, SRCA16, DST16, DSTP16, RB, GB, BB ), 4, 2 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BML_NTBL(     SRC16, SRCA16, DST16, DSTP16, RB, GB, BB ), 4, 2 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BML_TCK_NTBL( SRC16, DST16, DSTP16, RB, GB, BB ), 2, 2 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BML_NTBL(     SRC16, DST16, DSTP16, RB, GB, BB ), 2, 2 )
						}
					}
					break ;

				case DX_BLENDMODE_MULA :	// ��Z+���u�����h
					if( BlendImg != NULL ){
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BMA_ACK_NTBL( SRC16, SRCA16, DST16, DSTP16, BLND, AB, RB, GB, BB ), 4, 2 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BMA_NTBL(     SRC16, SRCA16, DST16, DSTP16, BLND, AB, RB, GB, BB ), 4, 2 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BMA_TCK_NTBL( SRC16, DST16, DSTP16, BLND, AB, RB, GB, BB ), 2, 2 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BMA_NTBL(     SRC16, DST16, DSTP16, BLND, AB, RB, GB, BB ), 2, 2 )
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BMA_ACK_NTBL( SRC16, SRCA16, DST16, DSTP16, AB, RB, GB, BB ), 4, 2 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BMA_NTBL(     SRC16, SRCA16, DST16, DSTP16, AB, RB, GB, BB ), 4, 2 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BMA_TCK_NTBL( SRC16, DST16, DSTP16, AB, RB, GB, BB ), 2, 2 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BMA_NTBL(     SRC16, DST16, DSTP16, AB, RB, GB, BB ), 2, 2 )
						}
					}
					break ;

				case DX_BLENDMODE_INVSRC :		// �`�挳�̐F�𔽓]�{���u�����h
					if( BlendImg != NULL ){
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BIS_ACK_NTBL( SRC16, SRCA16, DST16, DSTP16, BLND, AB, RB, GB, BB ), 4, 2 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BIS_NTBL(     SRC16, SRCA16, DST16, DSTP16, BLND, AB, RB, GB, BB ), 4, 2 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BIS_TCK_NTBL( SRC16, DST16, DSTP16, BLND, AB, RB, GB, BB ), 2, 2 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BIS_NTBL(     SRC16, DST16, DSTP16, BLND, AB, RB, GB, BB ), 2, 2 )
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BIS_ACK_NTBL( SRC16, SRCA16, DST16, DSTP16, AB, RB, GB, BB ), 4, 2 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BIS_NTBL(     SRC16, SRCA16, DST16, DSTP16, AB, RB, GB, BB ), 4, 2 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BIS_TCK_NTBL( SRC16, DST16, DSTP16, AB, RB, GB, BB ), 2, 2 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BIS_NTBL(     SRC16, DST16, DSTP16, AB, RB, GB, BB ), 2, 2 )
						}
					}
					break ;
				}
			}
			break ;
			
		case 1 :	// 32bit ���[�h
			if( SrcImg->Base->UsePalette == 1 )
			// �p���b�g������ꍇ
			{
				// �u�����h���[�h�ɂ���ď����𕪊�
				switch( MemImgManage.BlendMode )
				{
				case DX_BLENDMODE_NOBLEND :		// �u�����h����
					if( BlendImg != NULL ){
						if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C32_UBR_NAC_BNO_NTBL( PALP32, DSTP32, BLND, RB, GB, BB ), 1, 4 )
						else				DRAWPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C32_UBR_NAC_BNO_NTBL( PALP32, DSTP32, BLND, RB, GB, BB ), 1, 4 )
					}else{
						if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C32_UBR_NAC_BNO_NTBL( PALP32, DSTP32, RB, GB, BB ), 1, 4 )
						else				DRAWPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C32_UBR_NAC_BNO_NTBL( PALP32, DSTP32, RB, GB, BB ), 1, 4 )
					}
					break ;
					
				case DX_BLENDMODE_ALPHA :		// ���u�����h
					if( BlendImg != NULL ){
						if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C32_UBR_NAC_BAL_NTBL( PALP32, DSTP32, BLND, AB, RB, GB, BB ), 1, 4 )
						else				DRAWPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C32_UBR_NAC_BAL_NTBL( PALP32, DSTP32, BLND, AB, RB, GB, BB ), 1, 4 )
					}else{
						if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C32_UBR_NAC_BAL_NTBL( PALP32, DSTP32, AB, RB, GB, BB ), 1, 4 )
						else				DRAWPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C32_UBR_NAC_BAL_NTBL( PALP32, DSTP32, AB, RB, GB, BB ), 1, 4 )
					}
					break ;
					
				case DX_BLENDMODE_ADD :			// ���Z�u�����h
					if( BlendImg != NULL ){
						if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C32_UBR_NAC_BAD_NTBL( PALP32, DSTP32, BLND, AB, RB, GB, BB ), 1, 4 )
						else				DRAWPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C32_UBR_NAC_BAD_NTBL( PALP32, DSTP32, BLND, AB, RB, GB, BB ), 1, 4 )
					}else{
						if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C32_UBR_NAC_BAD_NTBL( PALP32, DSTP32, AB, RB, GB, BB ), 1, 4 )
						else				DRAWPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C32_UBR_NAC_BAD_NTBL( PALP32, DSTP32, AB, RB, GB, BB ), 1, 4 )
					}
					break ;

				case DX_BLENDMODE_SUB :			// ���Z�u�����h
					if( BlendImg != NULL ){
						if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C32_UBR_NAC_BSB_NTBL( PALP32, DSTP32, BLND, AB, RB, GB, BB ), 1, 4 )
						else				DRAWPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C32_UBR_NAC_BSB_NTBL( PALP32, DSTP32, BLND, AB, RB, GB, BB ), 1, 4 )
					}else{
						if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C32_UBR_NAC_BSB_NTBL( PALP32, DSTP32, AB, RB, GB, BB ), 1, 4 )
						else				DRAWPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C32_UBR_NAC_BSB_NTBL( PALP32, DSTP32, AB, RB, GB, BB ), 1, 4 )
					}
					break ;
					
				case DX_BLENDMODE_MUL :			// ��Z�u�����h
					if( BlendImg != NULL ){
						if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C32_UBR_NAC_BML_NTBL( PALP32, DSTP32, BLND, RB, GB, BB ), 1, 4 )
						else				DRAWPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C32_UBR_NAC_BML_NTBL( PALP32, DSTP32, BLND, RB, GB, BB ), 1, 4 )
					}else{
						if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C32_UBR_NAC_BML_NTBL( PALP32, DSTP32, RB, GB, BB ), 1, 4 )
						else				DRAWPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C32_UBR_NAC_BML_NTBL( PALP32, DSTP32, RB, GB, BB ), 1, 4 )
					}
					break ;

				case DX_BLENDMODE_MULA :		// ��Z+���u�����h
					if( BlendImg != NULL ){
						if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C32_UBR_NAC_BMA_NTBL( PALP32, DSTP32, BLND, AB, RB, GB, BB ), 1, 4 )
						else				DRAWPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C32_UBR_NAC_BMA_NTBL( PALP32, DSTP32, BLND, AB, RB, GB, BB ), 1, 4 )
					}else{
						if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C32_UBR_NAC_BMA_NTBL( PALP32, DSTP32, AB, RB, GB, BB ), 1, 4 )
						else				DRAWPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C32_UBR_NAC_BMA_NTBL( PALP32, DSTP32, AB, RB, GB, BB ), 1, 4 )
					}
					break ;

				case DX_BLENDMODE_INVSRC :		// �`�挳�̐F�𔽓]�{���u�����h
					if( BlendImg != NULL ){
						if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( if( *SrcBP != TransColor )	CODE_UBI_C32_UBR_NAC_BIS_NTBL( PALP32, DSTP32, BLND, AB, RB, GB, BB ), 1, 4 )
						else				DRAWPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C32_UBR_NAC_BIS_NTBL( PALP32, DSTP32, BLND, AB, RB, GB, BB ), 1, 4 )
					}else{
						if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( if( *SrcBP != TransColor )	CODE_NBI_C32_UBR_NAC_BIS_NTBL( PALP32, DSTP32, AB, RB, GB, BB ), 1, 4 )
						else				DRAWPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C32_UBR_NAC_BIS_NTBL( PALP32, DSTP32, AB, RB, GB, BB ), 1, 4 )
					}
					break ;
				}
			}
			else
			// �p���b�g�������ꍇ
			{
				// �u�����h���[�h�ɂ���ď����𕪊�
				switch( MemImgManage.BlendMode )
				{
				case DX_BLENDMODE_NOBLEND :		// �u�����h����
					if( BlendImg != NULL ){
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BNO_ACK_NTBL( SRCP32, DSTP32, BLND, RB, GB, BB ), 4, 4 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BNO_NTBL(     SRCP32, DSTP32, BLND, RB, GB, BB ), 4, 4 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BNO_TCK_NTBL( SRCP32, DSTP32, BLND, RB, GB, BB ), 4, 4 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BNO_NTBL(     SRCP32, DSTP32, BLND, RB, GB, BB ), 4, 4 )
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BNO_ACK_NTBL( SRCP32, DSTP32, RB, GB, BB ), 4, 4 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BNO_NTBL(     SRCP32, DSTP32, RB, GB, BB ), 4, 4 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BNO_TCK_NTBL( SRCP32, DSTP32, RB, GB, BB ), 4, 4 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BNO_NTBL(     SRCP32, DSTP32, RB, GB, BB ), 4, 4 )
						}
					}
					break ;

				case DX_BLENDMODE_ALPHA :		// ���u�����h
					if( BlendImg != NULL ){
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BAL_ACK_NTBL( SRCP32, DSTP32, BLND, AB, RB, GB, BB ), 4, 4 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BAL_NTBL(     SRCP32, DSTP32, BLND, AB, RB, GB, BB ), 4, 4 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BAL_TCK_NTBL( SRCP32, DSTP32, BLND, AB, RB, GB, BB ), 4, 4 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BAL_NTBL(     SRCP32, DSTP32, BLND, AB, RB, GB, BB ), 4, 4 )
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BAL_ACK_NTBL( SRCP32, DSTP32, AB, RB, GB, BB ), 4, 4 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BAL_NTBL(     SRCP32, DSTP32, AB, RB, GB, BB ), 4, 4 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BAL_TCK_NTBL( SRCP32, DSTP32, AB, RB, GB, BB ), 4, 4 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BAL_NTBL(     SRCP32, DSTP32, AB, RB, GB, BB ), 4, 4 )
						}
					}
					break ;

				case DX_BLENDMODE_ADD :		// ���Z�u�����h
					if( BlendImg != NULL ){
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BAD_ACK_NTBL( SRCP32, DSTP32, BLND, AB, RB, GB, BB ), 4, 4 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BAD_NTBL(     SRCP32, DSTP32, BLND, AB, RB, GB, BB ), 4, 4 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BAD_TCK_NTBL( SRCP32, DSTP32, BLND, AB, RB, GB, BB ), 4, 4 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BAD_NTBL(     SRCP32, DSTP32, BLND, AB, RB, GB, BB ), 4, 4 )
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BAD_ACK_NTBL( SRCP32, DSTP32, AB, RB, GB, BB ), 4, 4 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BAD_NTBL(     SRCP32, DSTP32, AB, RB, GB, BB ), 4, 4 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BAD_TCK_NTBL( SRCP32, DSTP32, AB, RB, GB, BB ), 4, 4 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BAD_NTBL(     SRCP32, DSTP32, AB, RB, GB, BB ), 4, 4 )
						}
					}
					break ;

				case DX_BLENDMODE_SUB :		// ���Z�u�����h
					if( BlendImg != NULL ){
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BSB_ACK_NTBL( SRCP32, DSTP32, BLND, AB, RB, GB, BB ), 4, 4 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BSB_NTBL(     SRCP32, DSTP32, BLND, AB, RB, GB, BB ), 4, 4 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BSB_TCK_NTBL( SRCP32, DSTP32, BLND, AB, RB, GB, BB ), 4, 4 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BSB_NTBL(     SRCP32, DSTP32, BLND, AB, RB, GB, BB ), 4, 4 )
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BSB_ACK_NTBL( SRCP32, DSTP32, AB, RB, GB, BB ), 4, 4 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BSB_NTBL(     SRCP32, DSTP32, AB, RB, GB, BB ), 4, 4 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BSB_TCK_NTBL( SRCP32, DSTP32, AB, RB, GB, BB ), 4, 4 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BSB_NTBL(     SRCP32, DSTP32, AB, RB, GB, BB ), 4, 4 )
						}
					}
					break ;

				case DX_BLENDMODE_MUL :		// ��Z�u�����h
					if( BlendImg != NULL ){
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BML_ACK_NTBL( SRCP32, DSTP32, BLND, RB, GB, BB ), 4, 4 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BML_NTBL(     SRCP32, DSTP32, BLND, RB, GB, BB ), 4, 4 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BML_TCK_NTBL( SRCP32, DSTP32, BLND, RB, GB, BB ), 4, 4 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BML_NTBL(     SRCP32, DSTP32, BLND, RB, GB, BB ), 4, 4 )
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BML_ACK_NTBL( SRCP32, DSTP32, RB, GB, BB ), 4, 4 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BML_NTBL(     SRCP32, DSTP32, RB, GB, BB ), 4, 4 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BML_TCK_NTBL( SRCP32, DSTP32, RB, GB, BB ), 4, 4 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BML_NTBL(     SRCP32, DSTP32, RB, GB, BB ), 4, 4 )
						}
					}
					break ;

				case DX_BLENDMODE_MULA :	// ��Z+���u�����h
					if( BlendImg != NULL ){
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BMA_ACK_NTBL( SRCP32, DSTP32, BLND, AB, RB, GB, BB ), 4, 4 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BMA_NTBL(     SRCP32, DSTP32, BLND, AB, RB, GB, BB ), 4, 4 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BMA_TCK_NTBL( SRCP32, DSTP32, BLND, AB, RB, GB, BB ), 4, 4 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BMA_NTBL(     SRCP32, DSTP32, BLND, AB, RB, GB, BB ), 4, 4 )
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BMA_ACK_NTBL( SRCP32, DSTP32, AB, RB, GB, BB ), 4, 4 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BMA_NTBL(     SRCP32, DSTP32, AB, RB, GB, BB ), 4, 4 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BMA_TCK_NTBL( SRCP32, DSTP32, AB, RB, GB, BB ), 4, 4 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BMA_NTBL(     SRCP32, DSTP32, AB, RB, GB, BB ), 4, 4 )
						}
					}
					break ;

				case DX_BLENDMODE_INVSRC :		// �`�挳�̐F�𔽓]�{���u�����h
					if( BlendImg != NULL ){
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BIS_ACK_NTBL( SRCP32, DSTP32, BLND, AB, RB, GB, BB ), 4, 4 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BIS_NTBL(     SRCP32, DSTP32, BLND, AB, RB, GB, BB ), 4, 4 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BIS_TCK_NTBL( SRCP32, DSTP32, BLND, AB, RB, GB, BB ), 4, 4 )
							else				DRAWPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BIS_NTBL(     SRCP32, DSTP32, BLND, AB, RB, GB, BB ), 4, 4 )
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BIS_ACK_NTBL( SRCP32, DSTP32, AB, RB, GB, BB ), 4, 4 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BIS_NTBL(     SRCP32, DSTP32, AB, RB, GB, BB ), 4, 4 )
						}else{
							if( TransFlag )		DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BIS_TCK_NTBL( SRCP32, DSTP32, AB, RB, GB, BB ), 4, 4 )
							else				DRAWPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BIS_NTBL(     SRCP32, DSTP32, AB, RB, GB, BB ), 4, 4 )
						}
					}
					break ;
				}
			}
			break ;
		}
	}

END:
	if( UseTempMemImg )
		TerminateMemImg( &TempMemImg );

/*	else
	// �e�N�X�`���������ꍇ
	{
		// �`�揀��
		if( SrcImg->Base->ColorType == 0 )
		{
			ColorD = RUP16( RT( RBOT16( Color ) ) ) |
				     GUP16( GT( GBOT16( Color ) ) ) |
				   	 BUP16( BT( BBOT16( Color ) ) ) ;
		}
		else
		{
			ColorD = Color ;
			ColorB[0] = (BYTE)RT( ColorB[0] ) ;
			ColorB[1] = (BYTE)GT( ColorB[1] ) ;
			ColorB[2] = (BYTE)BT( ColorB[2] ) ;
		}
		SrcBPT = ColorB ;

		// �J���[���[�h�ɂ���ď����𕪊�
		switch( SrcImg->Base->ColorType )
		{
		case 0 :	// 16bit ���[�h
			// �u�����h���[�h�ɂ���ď����𕪊�
			switch( MemImgManage.BlendMode )
			{
			case DX_BLENDMODE_NOBLEND :		// �u�����h����
NOTEX_NOMALDRAW_C16_NOPAL_BNO:
				DRAWBASICPOLYGONMEMIMG_NTEX_NBI_ND( CODE_NBI_C16_NBR_NAC_BNO( SRC16, DST16, DSTP16 ), 2 )
				break ;

			case DX_BLENDMODE_ALPHA :		// ���u�����h
				if( MemImgManage.BlendParam == 255 ) goto NOTEX_NOMALDRAW_C16_NOPAL_BNO ;
				else
				if( MemImgManage.BlendParam == 0 ) return ;
				
				DRAWBASICPOLYGONMEMIMG_NTEX_NBI_ND( CODE_NBI_C16_NBR_NAC_BAL( SRC16, DST16, DSTP16 ), 2 )
				break ;

			case DX_BLENDMODE_ADD :		// ���Z�u�����h
				if( MemImgManage.BlendParam == 0 ) return ;
				
				DRAWBASICPOLYGONMEMIMG_NTEX_NBI_ND( CODE_NBI_C16_NBR_NAC_BAD( SRC16, DST16, DSTP16 ), 2 )
				break ;

			case DX_BLENDMODE_SUB :		// ���Z�u�����h
				if( MemImgManage.BlendParam == 0 ) return ;
				
				DRAWBASICPOLYGONMEMIMG_NTEX_NBI_ND( CODE_NBI_C16_NBR_NAC_BSB( SRC16, DST16, DSTP16 ), 2 )
				break ;

			case DX_BLENDMODE_MUL :		// ��Z�u�����h
				DRAWBASICPOLYGONMEMIMG_NTEX_NBI_ND( CODE_NBI_C16_NBR_NAC_BML( SRC16, DST16, DSTP16 ), 2 )
				break ;
			}
			break ;
			
		case 1 :	// 32bit ���[�h
			// �u�����h���[�h�ɂ���ď����𕪊�
			switch( MemImgManage.BlendMode )
			{
			case DX_BLENDMODE_NOBLEND :		// �u�����h����
NOTEX_NOMALDRAW_C32_NOPAL_BNO:
				DRAWBASICPOLYGONMEMIMG_NTEX_NBI_ND( CODE_NBI_C32_NBR_NAC_BNO( SRCP32, DSTP32 ), 4 )
				break ;

			case DX_BLENDMODE_ALPHA :		// ���u�����h
				if( MemImgManage.BlendParam == 255 ) goto NOTEX_NOMALDRAW_C32_NOPAL_BNO ;
				else
				if( MemImgManage.BlendParam == 0 ) return ;
				
				DRAWBASICPOLYGONMEMIMG_NTEX_NBI_ND( CODE_NBI_C32_NBR_NAC_BAL( SRCP32, DSTP32 ), 4 )
				break ;

			case DX_BLENDMODE_ADD :		// ���Z�u�����h
				if( MemImgManage.BlendParam == 0 ) return ;
				
				DRAWBASICPOLYGONMEMIMG_NTEX_NBI_ND( CODE_NBI_C32_NBR_NAC_BAD( SRCP32, DSTP32 ), 4 )
				break ;

			case DX_BLENDMODE_SUB :		// ���Z�u�����h
				if( MemImgManage.BlendParam == 0 ) return ;
				
				DRAWBASICPOLYGONMEMIMG_NTEX_NBI_ND( CODE_NBI_C32_NBR_NAC_BSB( SRCP32, DSTP32 ), 4 )
				break ;

			case DX_BLENDMODE_MUL :		// ��Z�u�����h
				DRAWBASICPOLYGONMEMIMG_NTEX_NBI_ND( CODE_NBI_C32_NBR_NAC_BML( SRCP32, DSTP32 ), 4 )
				break ;
			}
			break ;
		}
	}
*/
	// �I��
	return ;
}
#undef PAL16
#undef DST16
#undef SRC16
#undef SRCA16
#undef DSTP16

#undef PALP32
#undef SRCP32
#undef DSTP32

#undef BLND

#endif

#endif // DX_NON_GRAPHICS

#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE
