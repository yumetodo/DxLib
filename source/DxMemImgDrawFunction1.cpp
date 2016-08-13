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
#include "DxMath.h"
#include "DxLib.h"
#include "DxStatic.h"
#include "DxBaseFunc.h"

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// �}�N����`------------------------------------------------------------------

// �f�[�^�^��`----------------------------------------------------------------

// �f�[�^�錾------------------------------------------------------------------

// �֐��v���g�^�C�v�錾--------------------------------------------------------

// �v���O����------------------------------------------------------------------

#ifdef DX_NON_2DDRAW

extern void DrawRotationMemImg( MEMIMG *DestImg, const MEMIMG *SrcImg, int x, int y, float Angle, float EnlageRate, int TransFlag, int TurnFlag, const MEMIMG *BlendImg )
{
	return ;
}

extern void DrawTransformMemImg( MEMIMG *DestImg, const MEMIMG *SrcImg, const POINT *pos4, int TransFlag, const MEMIMG *BlendImg )
{
	return ;
}

extern void DrawBasicPolygonMemImg( MEMIMG *DestImg, const MEMIMG *SrcImg, const DX_POINTDATA *pos3, int TransFlag, const MEMIMG *BlendImg, unsigned int Color )
{
	return ;
}

#else











/*
#define DRAWROTATIONMEMIMG_UBI_ND( DRAW, SRCADDNUM, DESTADDNUM )	\
{\
	do{\
		GSX = GDX2 ;\
		GSY = GDY2 ;\
		do{\
			if( (DWORD)GDX2 < (DWORD)SizeX && (DWORD)GDY2 < (DWORD)SizeY ){\
				SrcBPT   = SrcBP   + ( GDX2 / QUALI ) * (SRCADDNUM) + ( GDY2 / QUALI ) * SrcPitch   ;\
				BlendBPT = BlendBP + ( GDX2 / QUALI )               + ( GDY2 / QUALI ) * BlendPitch ;\
				DRAW\
			}\
			GDX2 += GAADX ;\
			GDY2 += GAADY ;\
			DestBPT += (DESTADDNUM) ;\
		}while( -- C ) ;\
		C = EndC ;\
		GDX2 = GSX += GLADDX ;\
		GDY2 = GSY += GLADDY ;\
		DestBPT = DestBP += DestPitch ;\
	}while( -- C2 ) ;\
}
*/
#define DRAWROTATIONMEMIMG_UBI_ND( DRAW, SRCADDNUM, DESTADDNUM )	\
{\
	do{\
		do{\
			if( (DWORD)GDX2 < (DWORD)SizeX && (DWORD)GDY2 < (DWORD)SizeY ){\
				SrcBPT   = (BYTE *)SrcLineP[ GDY2 / QUALI ] + ( GDX2 / QUALI ) * (SRCADDNUM) ;\
				BlendBPT = (BYTE *)BlendLineP[ GDY2 / QUALI ] + ( GDX2 / QUALI ) ;\
				DRAW\
			}\
			GDX2 += GAADX ;\
			GDY2 += GAADY ;\
			DestBPT += (DESTADDNUM) ;\
		}while( -- C ) ;\
		C = EndC ;\
		GDX2 += GLADDX ;\
		GDY2 += GLADDY ;\
		DestBPT += DestAddPitch ;\
	}while( -- C2 ) ;\
}

/*
#define DRAWROTATIONMEMIMG_NBI_ND( DRAW, SRCADDNUM, DESTADDNUM )	\
{\
	do{\
		GSX = GDX2 ;\
		GSY = GDY2 ;\
		do{\
			if( (DWORD)GDX2 < (DWORD)SizeX && (DWORD)GDY2 < (DWORD)SizeY ){\
				SrcBPT   = SrcBP   + ( GDX2 / QUALI ) * (SRCADDNUM) + ( GDY2 / QUALI ) * SrcPitch   ;\
				DRAW\
			}\
			GDX2 += GAADX ;\
			GDY2 += GAADY ;\
			DestBPT += (DESTADDNUM) ;\
		}while( -- C ) ;\
		C = EndC ;\
		GDX2 = GSX += GLADDX ;\
		GDY2 = GSY += GLADDY ;\
		DestBPT = DestBP += DestPitch ;\
	}while( -- C2 ) ;\
}
*/
#define DRAWROTATIONMEMIMG_NBI_ND( DRAW, SRCADDNUM, DESTADDNUM )	\
{\
	do{\
		do{\
			if( (DWORD)GDX2 < (DWORD)SizeX && (DWORD)GDY2 < (DWORD)SizeY ){\
				SrcBPT   = (BYTE *)SrcLineP[ GDY2 / QUALI ] + ( GDX2 / QUALI ) * (SRCADDNUM) ;\
				DRAW\
			}\
			GDX2 += GAADX ;\
			GDY2 += GAADY ;\
			DestBPT += (DESTADDNUM) ;\
		}while( -- C ) ;\
		C = EndC ;\
		GDX2 += GLADDX ;\
		GDY2 += GLADDY ;\
		DestBPT += DestAddPitch ;\
	}while( -- C2 ) ;\
}

#define PAL16	PaletteWP[ *SrcBPT ]
#define DST16	*DestWPT
#define SRC16	*SrcWPT
#define SRCA16	SrcBPT[2]
#define DSTP16	DestWPT

#define PALP32	(BYTE *)&PaletteDP[ *SrcBPT ]
#define SRCP32	SrcBPT
#define DSTP32	DestBPT

#define BLND	*BlendBPT
// �C���[�W����]�`�悷��
extern void DrawRotationMemImg( MEMIMG *DestImg, const MEMIMG *SrcImg, int x, int y, float Angle, float EnlageRate, int TransFlag, int TurnFlag, const MEMIMG *BlendImg )
{
	RECT DrawRect ;
	DWORD *RateTable1 ;
	int *RateTable2 ;
	DWORD_PTR DestPP, PalettePP ;
	DWORD Src1, Src2, Data, DataD ;
	DWORD *RedTable , *GreenTable , *BlueTable ;
	DWORD SrcPitch, DestPitch, BlendPitch, DestAddPitch ;
	DWORD TransColor ;
	int BD1X , BD1Y ;						// �`�撆�ɃJ�E���^�Ƃ��Ďg���x�N�g���ϐ�(�`���p) 
	int GDX , GDY ;							// �`�撆�ɃJ�E���^�Ƃ��Ďg���x�N�g���ϐ�(�`�挳�p)
	int GLADDX , GLADDY  ;					// �g�p���̕`�挳�Q�Ɨp���Z�x�N�g��
	int GAADX , GAADY ;						// �O���t�B�b�N���A�P�`�悲�Ƃ̉��Z�l
	int MaxY , MaxX ;
	int C, EndC, C2, EndC2 ;
	int GDX2, GDY2 ;
	int SizeX, SizeY ;
	int GLADDX2, GLADDY2 ;
	int GAADY2, GAADX2 ;
	static DWORD PaletteTemp[256] ;
	static DWORD_PTR SrcLineP[4096], BlendLineP[4096] ;

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

	// �J���[�^�C�v���Ⴄ���A�`��悪���`�����l���t���p���b�g�t���������ꍇ�̓G���[
	if( SrcImg->Base->ColorType != DestImg->Base->ColorType || DestImg->Base->UsePalette == 1 || DestImg->Base->UseAlpha == 1 ) return ;
	
	// �X�L�������C���A���S���Y��(���ǂ�)
	{
		int px[4] , py[4] ;
		int Cos , Sin ;
		int SinD , CosD ;
		float SinF, CosF ;

		_SINCOS_PLATFORM( -Angle, &SinF, &CosF ) ;
		SinD = _FTOL( SinF * QUALI ) ;
		CosD = _FTOL( CosF * QUALI ) ;

		// ��]�p�r�����A�b�����l�𓾂�
		_SINCOS_PLATFORM( (float)Angle, &SinF, &CosF ) ;
		Sin = _FTOL( SinF * QUALI ) ;
		Cos = _FTOL( CosF * QUALI ) ;

		// �`����W�𓾂�
		{
			int xt , yt ;
			float SizeXF, SizeYF ;
			float SizeXFCos, SizeXFSin, SizeYFCos, SizeYFSin ;

			xt = x * QUALI ;
			yt = y * QUALI ;
			SizeXF = SrcImg->Width  * EnlageRate / 2 ;
			SizeYF = SrcImg->Height * EnlageRate / 2 ;
			SizeXFCos = SizeXF * Cos ;
			SizeXFSin = SizeXF * Sin ;
			SizeYFCos = SizeYF * Cos ;
			SizeYFSin = SizeYF * Sin ;

			px[0] = _FTOL( -SizeXFCos - (-SizeYFSin) ) + xt ;
			py[0] = _FTOL( -SizeXFSin + (-SizeYFCos) ) + yt ;

			px[1] = _FTOL( -SizeXFCos -   SizeYFSin  ) + xt ;
			py[1] = _FTOL( -SizeXFSin +   SizeYFCos  ) + yt ;

			px[2] = _FTOL(  SizeXFCos -   SizeYFSin  ) + xt ;
			py[2] = _FTOL(  SizeXFSin +   SizeYFCos  ) + yt ;

			px[3] = _FTOL(  SizeXFCos - (-SizeYFSin) ) + xt ;
			py[3] = _FTOL(  SizeXFSin + (-SizeYFCos) ) + yt ;

			// ���]�t���O�������Ă����� DrawTransformMemImg �֐��ɂ܂�����
			if( TurnFlag )
			{
				POINT point[4] ;

				point[0].x = px[3] / QUALI ;	point[0].y = py[3] / QUALI ;
				point[1].x = px[0] / QUALI ;	point[1].y = py[0] / QUALI ;
				point[2].x = px[2] / QUALI ;	point[2].y = py[2] / QUALI ;
				point[3].x = px[1] / QUALI ;	point[3].y = py[1] / QUALI ;
				DrawTransformMemImg( DestImg, SrcImg, point, TransFlag, BlendImg ) ;
				return ;
			}

			GAADX = _FTOL( Cos  / EnlageRate ) ;
			GAADY = _FTOL( SinD / EnlageRate ) ;
		}

		// ��ԏ�ɗ��Ă�����W�ɂ���ĕ`��N�_���W��������
		{
			int i , MaxY2 , MinY2 , MaxX2 , MinX2 ;

			MaxY2 = 0 ; MinY2 = 0 ;
			MaxX2 = 0 ; MinX2 = 0 ; 
			for( i = 1 ; i < 4 ; i ++ )
			{
				if( py[MaxY2] < py[i] ) MaxY2 = i ;
				if( py[MinY2] > py[i] ) MinY2 = i ;

				if( px[MaxX2] < px[i] ) MaxX2 = i ;
				if( px[MinX2] > px[i] ) MinX2 = i ;
			}


			BD1X = (int)( px[MinX2] / QUALI ) - 2 ; 
			BD1Y = (int)( py[MinY2] / QUALI ) - 2 ; 

			MaxX = (int)( px[MaxX2] / QUALI ) + 4 ;
			MaxY = (int)( py[MaxY2] / QUALI ) + 4 ;

			{
				DrawRect.left   = BD1X ;
				DrawRect.top    = BD1Y ;
				DrawRect.right  = MaxX ;
				DrawRect.bottom = MaxY ;
				RectClipping_Inline( &DrawRect , &MemImgManage.DrawArea ) ; 
				if( DrawRect.left == DrawRect.right || DrawRect.top == DrawRect.bottom ) return ;

				BD1X = DrawRect.left ;  BD1Y = DrawRect.top ;
				MaxX = DrawRect.right ; MaxY = DrawRect.bottom ;

				py[MaxY2] = MaxY * QUALI ;
				py[MinY2] = BD1Y * QUALI ;
			}

			{
				int xt , yt, h ;
				int GX1 , GX2 , GY1 , GY2 ;

				xt = (int)( BD1X - x ) ;
				yt = (int)( BD1Y - y ) ;

				GX1 = ( int )( _FTOL( ( CosD * xt - SinD * yt ) / EnlageRate ) + SrcImg->Width  / 2 * QUALI ) ;
				GY1 = ( int )( _FTOL( ( SinD * xt + CosD * yt ) / EnlageRate ) + SrcImg->Height / 2 * QUALI ) ;

				xt = (int)( BD1X - x ) ;
				yt = (int)( (int)( py[MaxY2] / QUALI ) - y ) ; 

				GX2 = ( int )( _FTOL( ( CosD * xt - SinD * yt ) / EnlageRate ) + SrcImg->Width  / 2 * QUALI ) ;
				GY2 = ( int )( _FTOL( ( SinD * xt + CosD * yt ) / EnlageRate ) + SrcImg->Height / 2 * QUALI ) ;

				h = (int)( py[MaxY2] - py[MinY2] ) / QUALI ;
				if( h == 0 ) return ;
				GLADDX = ( GX2 - GX1 ) / h ;
				GLADDY = ( GY2 - GY1 ) / h ;

				GDX = GX1 ;
				GDY = GY1 ;
			}
		}
	}
		
	// �]�����A�]����̃A�h���X���Z�b�g
	SrcBP        = SrcImg->UseImage ;
	SrcPitch     = SrcImg->Base->Pitch ;

	DestBP       = DestImg->UseImage + BD1X * DestImg->Base->ColorDataP->PixelByte + BD1Y * DestImg->Base->Pitch ;
	DestPitch    = DestImg->Base->Pitch ;
	DestPP       = (DWORD_PTR)DestBP ;

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
	PalettePP  = (DWORD_PTR)PaletteBP ;

	// �]�����̊e���C���̐擪�A�h���X���Z�o
	{
		DWORD_PTR addres, h, i ;

		h  = SrcImg->Height ;
		addres = (DWORD_PTR)SrcBP ;
		for( i = 0 ; i < h ; i ++, addres += SrcPitch )
			SrcLineP[i] = (DWORD_PTR)addres ;

		if( BlendImg != NULL )
		{
			h  = BlendImg->Height ;
			addres = (DWORD_PTR)BlendBP ;
			for( i = 0 ; i < h ; i ++, addres += BlendPitch )
				BlendLineP[i] = (DWORD_PTR)addres ;
		}
	}

	// �p���b�g�̃R�s�[
	if( SrcImg->Base->UsePalette == 1 )
	{
		_MEMCPY( PaletteTemp, SrcImg->Base->Palette, ( size_t )( SrcImg->Base->ColorNum * ( SrcImg->Base->ColorType == 0 ? 2 : 4 ) ) ) ;
	}
	
	// �`�揀��
	{
		GLADDX2 = GLADDX ;
		GLADDY2 = GLADDY ;
		GAADY2 = GAADY ;
		GAADX2 = GAADX ;

		SizeX = ( int )( SrcImg->Width  * QUALI ) ;
		SizeY = ( int )( SrcImg->Height * QUALI ) ;
			
		EndC = MaxX - BD1X ;
		C = EndC ;

		EndC2 = MaxY - BD1Y ;
		C2 = EndC2 ;

		GDX2 = GDX ;
		GDY2 = GDY ;

		DestBPT = DestBP ;

		GLADDX -= EndC * GAADX ;
		GLADDY -= EndC * GAADY ;
		DestAddPitch = DestPitch - EndC * DestImg->Base->ColorDataP->PixelByte ;
	}

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
						if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C16_NBR_NAC_BNO( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						else				DRAWROTATIONMEMIMG_UBI_ND( 								CODE_UBI_C16_NBR_NAC_BNO( PAL16, DST16, DSTP16, BLND ), 1, 2 )
					}else{
						if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C16_UBR_NAC_BNO( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						else				DRAWROTATIONMEMIMG_UBI_ND( 								CODE_UBI_C16_UBR_NAC_BNO( PAL16, DST16, DSTP16, BLND ), 1, 2 )
					}
				}else{
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( TransFlag )
						{
#ifdef DX_NON_INLINE_ASM
							DRAWROTATIONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C16_NBR_NAC_BNO( PAL16, DST16, DSTP16 ), 1, 2 )
#else
							_asm
							{
								MOV EDI, DestPP
								MOV ESI, GDX2
								MOV EDX, GDY2
							LABEL_NBI_C16_NBR_NAC_BNO_PAL_TR_1:
								MOV ECX, EndC
							LABEL_NBI_C16_NBR_NAC_BNO_PAL_TR_2:
								CMP ESI, SizeX
								JAE LABEL_NBI_C16_NBR_NAC_BNO_PAL_TR_3
								CMP EDX, SizeY
								JAE LABEL_NBI_C16_NBR_NAC_BNO_PAL_TR_3

								MOV EBX, EDX
								MOV EAX, ESI
								SHR EBX, 18
								SHR EAX, 18
								MOV EBX, [SrcLineP + EBX * 4]
								MOVZX EBX, byte ptr [EAX + EBX]
								CMP EBX, TransColor
								JE LABEL_NBI_C16_NBR_NAC_BNO_PAL_TR_3
								MOV AX, word ptr [PaletteTemp + EBX * 2]
								MOV [EDI], AX

							LABEL_NBI_C16_NBR_NAC_BNO_PAL_TR_3:
								ADD ESI, GAADX
								ADD EDX, GAADY
								ADD EDI, 2
								DEC ECX
								JNZ LABEL_NBI_C16_NBR_NAC_BNO_PAL_TR_2

								ADD ESI, GLADDX
								ADD EDX, GLADDY
								ADD EDI, DestAddPitch
								DEC C2
								JNZ LABEL_NBI_C16_NBR_NAC_BNO_PAL_TR_1
							}
#endif
						}
						else
						{
#ifdef DX_NON_INLINE_ASM
							DRAWROTATIONMEMIMG_NBI_ND( 								CODE_NBI_C16_NBR_NAC_BNO( PAL16, DST16, DSTP16 ), 1, 2 )
#else
							_asm
							{
								MOV EDI, DestPP
								MOV ESI, GDX2
								MOV EDX, GDY2
							LABEL_NBI_C16_NBR_NAC_BNO_PAL_NTR_1:
								MOV ECX, EndC
							LABEL_NBI_C16_NBR_NAC_BNO_PAL_NTR_2:
								CMP ESI, SizeX
								JAE LABEL_NBI_C16_NBR_NAC_BNO_PAL_NTR_3
								CMP EDX, SizeY
								JAE LABEL_NBI_C16_NBR_NAC_BNO_PAL_NTR_3

								MOV EBX, EDX
								MOV EAX, ESI
								SHR EBX, 18
								SHR EAX, 18
								MOV EBX, [SrcLineP + EBX * 4]
								MOVZX EBX, byte ptr [EAX + EBX]
								MOV AX, word ptr [PaletteTemp + EBX * 2]
								MOV [EDI], AX

							LABEL_NBI_C16_NBR_NAC_BNO_PAL_NTR_3:
								ADD ESI, GAADX
								ADD EDX, GAADY
								ADD EDI, 2
								DEC ECX
								JNZ LABEL_NBI_C16_NBR_NAC_BNO_PAL_NTR_2

								ADD ESI, GLADDX
								ADD EDX, GLADDY
								ADD EDI, DestAddPitch
								DEC C2
								JNZ LABEL_NBI_C16_NBR_NAC_BNO_PAL_NTR_1
							}
#endif
						}
					}else{
						if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C16_UBR_NAC_BNO( PAL16, DST16, DSTP16 ), 1, 2 )
						else				DRAWROTATIONMEMIMG_NBI_ND( 								CODE_NBI_C16_UBR_NAC_BNO( PAL16, DST16, DSTP16 ), 1, 2 )
					}
				}
				break ;
				
			case DX_BLENDMODE_ALPHA :		// ���u�����h
				if( MemImgManage.BlendParam == 255 ) goto NOMALDRAW_C16_USEPAL_BNO ;
				else
				if( MemImgManage.BlendParam == 0 ) return ;
				
				if( BlendImg != NULL ){
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C16_NBR_NAC_BAL( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						else				DRAWROTATIONMEMIMG_UBI_ND( 								CODE_UBI_C16_NBR_NAC_BAL( PAL16, DST16, DSTP16, BLND ), 1, 2 )
					}else{
						if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C16_UBR_NAC_BAL( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						else				DRAWROTATIONMEMIMG_UBI_ND( 								CODE_UBI_C16_UBR_NAC_BAL( PAL16, DST16, DSTP16, BLND ), 1, 2 )
					}
				}else{
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C16_NBR_NAC_BAL( PAL16, DST16, DSTP16 ), 1, 2 )
						else				DRAWROTATIONMEMIMG_NBI_ND( 								CODE_NBI_C16_NBR_NAC_BAL( PAL16, DST16, DSTP16 ), 1, 2 )
					}else{
						if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C16_UBR_NAC_BAL( PAL16, DST16, DSTP16 ), 1, 2 )
						else				DRAWROTATIONMEMIMG_NBI_ND( 								CODE_NBI_C16_UBR_NAC_BAL( PAL16, DST16, DSTP16 ), 1, 2 )
					}
				}
				break ;
				
			case DX_BLENDMODE_ADD :			// ���Z�u�����h
				if( MemImgManage.BlendParam == 0 ) return ;
				
				if( BlendImg != NULL ){
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C16_NBR_NAC_BAD( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						else				DRAWROTATIONMEMIMG_UBI_ND( 								CODE_UBI_C16_NBR_NAC_BAD( PAL16, DST16, DSTP16, BLND ), 1, 2 )
					}else{
						if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C16_UBR_NAC_BAD( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						else				DRAWROTATIONMEMIMG_UBI_ND( 								CODE_UBI_C16_UBR_NAC_BAD( PAL16, DST16, DSTP16, BLND ), 1, 2 )
					}
				}else{
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C16_NBR_NAC_BAD( PAL16, DST16, DSTP16 ), 1, 2 )
						else				DRAWROTATIONMEMIMG_NBI_ND( 								CODE_NBI_C16_NBR_NAC_BAD( PAL16, DST16, DSTP16 ), 1, 2 )
					}else{
						if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C16_UBR_NAC_BAD( PAL16, DST16, DSTP16 ), 1, 2 )
						else				DRAWROTATIONMEMIMG_NBI_ND( 								CODE_NBI_C16_UBR_NAC_BAD( PAL16, DST16, DSTP16 ), 1, 2 )
					}
				}
				break ;

			case DX_BLENDMODE_SUB :			// ���Z�u�����h
				if( BlendImg != NULL ){
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C16_NBR_NAC_BSB( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						else				DRAWROTATIONMEMIMG_UBI_ND( 								CODE_UBI_C16_NBR_NAC_BSB( PAL16, DST16, DSTP16, BLND ), 1, 2 )
					}else{
						if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C16_UBR_NAC_BSB( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						else				DRAWROTATIONMEMIMG_UBI_ND( 								CODE_UBI_C16_UBR_NAC_BSB( PAL16, DST16, DSTP16, BLND ), 1, 2 )
					}
				}else{
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C16_NBR_NAC_BSB( PAL16, DST16, DSTP16 ), 1, 2 )
						else				DRAWROTATIONMEMIMG_NBI_ND( 								CODE_NBI_C16_NBR_NAC_BSB( PAL16, DST16, DSTP16 ), 1, 2 )
					}else{
						if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C16_UBR_NAC_BSB( PAL16, DST16, DSTP16 ), 1, 2 )
						else				DRAWROTATIONMEMIMG_NBI_ND( 								CODE_NBI_C16_UBR_NAC_BSB( PAL16, DST16, DSTP16 ), 1, 2 )
					}
				}
				break ;
				
			case DX_BLENDMODE_MUL :			// ��Z�u�����h
				if( BlendImg != NULL ){
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C16_NBR_NAC_BML( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						else				DRAWROTATIONMEMIMG_UBI_ND( 								CODE_UBI_C16_NBR_NAC_BML( PAL16, DST16, DSTP16, BLND ), 1, 2 )
					}else{
						if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C16_UBR_NAC_BML( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						else				DRAWROTATIONMEMIMG_UBI_ND( 								CODE_UBI_C16_UBR_NAC_BML( PAL16, DST16, DSTP16, BLND ), 1, 2 )
					}
				}else{
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C16_NBR_NAC_BML( PAL16, DST16, DSTP16 ), 1, 2 )
						else				DRAWROTATIONMEMIMG_NBI_ND( 								CODE_NBI_C16_NBR_NAC_BML( PAL16, DST16, DSTP16 ), 1, 2 )
					}else{
						if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C16_UBR_NAC_BML( PAL16, DST16, DSTP16 ), 1, 2 )
						else				DRAWROTATIONMEMIMG_NBI_ND( 								CODE_NBI_C16_UBR_NAC_BML( PAL16, DST16, DSTP16 ), 1, 2 )
					}
				}
				break ;

			case DX_BLENDMODE_MULA :		// ��Z+���u�����h
				if( BlendImg != NULL ){
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C16_NBR_NAC_BMA( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						else				DRAWROTATIONMEMIMG_UBI_ND( 								CODE_UBI_C16_NBR_NAC_BMA( PAL16, DST16, DSTP16, BLND ), 1, 2 )
					}else{
						if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C16_UBR_NAC_BMA( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						else				DRAWROTATIONMEMIMG_UBI_ND( 								CODE_UBI_C16_UBR_NAC_BMA( PAL16, DST16, DSTP16, BLND ), 1, 2 )
					}
				}else{
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C16_NBR_NAC_BMA( PAL16, DST16, DSTP16 ), 1, 2 )
						else				DRAWROTATIONMEMIMG_NBI_ND( 								CODE_NBI_C16_NBR_NAC_BMA( PAL16, DST16, DSTP16 ), 1, 2 )
					}else{
						if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C16_UBR_NAC_BMA( PAL16, DST16, DSTP16 ), 1, 2 )
						else				DRAWROTATIONMEMIMG_NBI_ND( 								CODE_NBI_C16_UBR_NAC_BMA( PAL16, DST16, DSTP16 ), 1, 2 )
					}
				}
				break ;

			case DX_BLENDMODE_INVSRC :		// �`�挳�̐F�𔽓]�{���u�����h
				if( BlendImg != NULL ){
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C16_NBR_NAC_BIS( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						else				DRAWROTATIONMEMIMG_UBI_ND( 								CODE_UBI_C16_NBR_NAC_BIS( PAL16, DST16, DSTP16, BLND ), 1, 2 )
					}else{
						if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C16_UBR_NAC_BIS( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						else				DRAWROTATIONMEMIMG_UBI_ND( 								CODE_UBI_C16_UBR_NAC_BIS( PAL16, DST16, DSTP16, BLND ), 1, 2 )
					}
				}else{
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C16_NBR_NAC_BIS( PAL16, DST16, DSTP16 ), 1, 2 )
						else				DRAWROTATIONMEMIMG_NBI_ND( 								CODE_NBI_C16_NBR_NAC_BIS( PAL16, DST16, DSTP16 ), 1, 2 )
					}else{
						if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C16_UBR_NAC_BIS( PAL16, DST16, DSTP16 ), 1, 2 )
						else				DRAWROTATIONMEMIMG_NBI_ND( 								CODE_NBI_C16_UBR_NAC_BIS( PAL16, DST16, DSTP16 ), 1, 2 )
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
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_UAC_BNO_ACK( SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_UAC_BNO(     SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_NAC_BNO_TCK( SRC16, DST16, DSTP16, BLND ), 2, 2 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_NAC_BNO(     SRC16, DST16, DSTP16, BLND ), 2, 2 )
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BNO_ACK( SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BNO(     SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BNO_TCK( SRC16, DST16, DSTP16, BLND ), 2, 2 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BNO(     SRC16, DST16, DSTP16, BLND ), 2, 2 )
						}
					}
				}else{
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_UAC_BNO_ACK( SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_UAC_BNO(     SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
						}else{
							if( TransFlag )
							{
#ifdef DX_NON_INLINE_ASM
								DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_NAC_BNO_TCK( SRC16, DST16, DSTP16 ), 2, 2 )
#else
								_asm
								{
									MOV EDI, DestPP
									MOV ESI, GDX2
									MOV EDX, GDY2
								LABEL_NBI_C16_NBR_NAC_BNO_NPAL_TR_1:
									MOV ECX, EndC
								LABEL_NBI_C16_NBR_NAC_BNO_NPAL_TR_2:
									CMP ESI, SizeX
									JAE LABEL_NBI_C16_NBR_NAC_BNO_NPAL_TR_3
									CMP EDX, SizeY
									JAE LABEL_NBI_C16_NBR_NAC_BNO_NPAL_TR_3

									MOV EBX, EDX
									MOV EAX, ESI
									SHR EBX, 18
									SHR EAX, 17
									MOV EBX, [SrcLineP + EBX * 4]
									AND EAX, 0xfffffffe
									MOVZX EBX, word ptr [EAX + EBX]
									CMP EBX, TransColor
									JE LABEL_NBI_C16_NBR_NAC_BNO_NPAL_TR_3
									MOV [EDI], BX

								LABEL_NBI_C16_NBR_NAC_BNO_NPAL_TR_3:
									ADD ESI, GAADX
									ADD EDX, GAADY
									ADD EDI, 2
									DEC ECX
									JNZ LABEL_NBI_C16_NBR_NAC_BNO_NPAL_TR_2

									ADD ESI, GLADDX
									ADD EDX, GLADDY
									ADD EDI, DestAddPitch
									DEC C2
									JNZ LABEL_NBI_C16_NBR_NAC_BNO_NPAL_TR_1
								}
#endif
							}
							else
							{
#ifdef DX_NON_INLINE_ASM
								DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_NAC_BNO(     SRC16, DST16, DSTP16 ), 2, 2 )
#else
								_asm
								{
									MOV EDI, DestPP
									MOV ESI, GDX2
									MOV EDX, GDY2
								LABEL_NBI_C16_NBR_NAC_BNO_NPAL_NTR_1:
									MOV ECX, EndC
								LABEL_NBI_C16_NBR_NAC_BNO_NPAL_NTR_2:
									CMP ESI, SizeX
									JAE LABEL_NBI_C16_NBR_NAC_BNO_NPAL_NTR_3
									CMP EDX, SizeY
									JAE LABEL_NBI_C16_NBR_NAC_BNO_NPAL_NTR_3

									MOV EBX, EDX
									MOV EAX, ESI
									SHR EBX, 18
									SHR EAX, 17
									MOV EBX, [SrcLineP + EBX * 4]
									AND EAX, 0xfffffffe
									MOV BX, [EAX + EBX]
									MOV [EDI], BX

								LABEL_NBI_C16_NBR_NAC_BNO_NPAL_NTR_3:
									ADD ESI, GAADX
									ADD EDX, GAADY
									ADD EDI, 2
									DEC ECX
									JNZ LABEL_NBI_C16_NBR_NAC_BNO_NPAL_NTR_2

									ADD ESI, GLADDX
									ADD EDX, GLADDY
									ADD EDI, DestAddPitch
									DEC C2
									JNZ LABEL_NBI_C16_NBR_NAC_BNO_NPAL_NTR_1
								}
#endif
							}
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BNO_ACK( SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BNO(     SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BNO_TCK( SRC16, DST16, DSTP16 ), 2, 2 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BNO(     SRC16, DST16, DSTP16 ), 2, 2 )
						}
					}
				}
				break ;

			case DX_BLENDMODE_ALPHA :		// ���u�����h
				if( MemImgManage.BlendParam == 255 ) goto NOMALDRAW_C16_NOPAL_BNO ;
				else
				if( MemImgManage.BlendParam == 0 ) return ;
				
				if( BlendImg != NULL ){
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_UAC_BAL_ACK( SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_UAC_BAL(     SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_NAC_BAL_TCK( SRC16, DST16, DSTP16, BLND ), 2, 2 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_NAC_BAL(     SRC16, DST16, DSTP16, BLND ), 2, 2 )
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BAL_ACK( SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BAL(     SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BAL_TCK( SRC16, DST16, DSTP16, BLND ), 2, 2 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BAL(     SRC16, DST16, DSTP16, BLND ), 2, 2 )
						}
					}
				}else{
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_UAC_BAL_ACK( SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_UAC_BAL(     SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_NAC_BAL_TCK( SRC16, DST16, DSTP16 ), 2, 2 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_NAC_BAL(     SRC16, DST16, DSTP16 ), 2, 2 )
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BAL_ACK( SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BAL(     SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BAL_TCK( SRC16, DST16, DSTP16 ), 2, 2 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BAL(     SRC16, DST16, DSTP16 ), 2, 2 )
						}
					}
				}
				break ;

			case DX_BLENDMODE_ADD :		// ���Z�u�����h
				if( MemImgManage.BlendParam == 0 ) return ;
				
				if( BlendImg != NULL ){
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_UAC_BAD_ACK( SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_UAC_BAD(     SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_NAC_BAD_TCK( SRC16, DST16, DSTP16, BLND ), 2, 2 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_NAC_BAD(     SRC16, DST16, DSTP16, BLND ), 2, 2 )
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BAD_ACK( SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BAD(     SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BAD_TCK( SRC16, DST16, DSTP16, BLND ), 2, 2 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BAD(     SRC16, DST16, DSTP16, BLND ), 2, 2 )
						}
					}
				}else{
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_UAC_BAD_ACK( SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_UAC_BAD(     SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_NAC_BAD_TCK( SRC16, DST16, DSTP16 ), 2, 2 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_NAC_BAD(     SRC16, DST16, DSTP16 ), 2, 2 )
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BAD_ACK( SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BAD(     SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BAD_TCK( SRC16, DST16, DSTP16 ), 2, 2 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BAD(     SRC16, DST16, DSTP16 ), 2, 2 )
						}
					}
				}
				break ;

			case DX_BLENDMODE_SUB :		// ���Z�u�����h
				if( MemImgManage.BlendParam == 0 ) return ;
				
				if( BlendImg != NULL ){
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_UAC_BSB_ACK( SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_UAC_BSB(     SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_NAC_BSB_TCK( SRC16, DST16, DSTP16, BLND ), 2, 2 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_NAC_BSB(     SRC16, DST16, DSTP16, BLND ), 2, 2 )
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BSB_ACK( SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BSB(     SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BSB_TCK( SRC16, DST16, DSTP16, BLND ), 2, 2 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BSB(     SRC16, DST16, DSTP16, BLND ), 2, 2 )
						}
					}
				}else{
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_UAC_BSB_ACK( SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_UAC_BSB(     SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_NAC_BSB_TCK( SRC16, DST16, DSTP16 ), 2, 2 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_NAC_BSB(     SRC16, DST16, DSTP16 ), 2, 2 )
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BSB_ACK( SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BSB(     SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BSB_TCK( SRC16, DST16, DSTP16 ), 2, 2 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BSB(     SRC16, DST16, DSTP16 ), 2, 2 )
						}
					}
				}
				break ;

			case DX_BLENDMODE_MUL :		// ��Z�u�����h
				if( BlendImg != NULL ){
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_UAC_BML_ACK( SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_UAC_BML(     SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_NAC_BML_TCK( SRC16, DST16, DSTP16, BLND ), 2, 2 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_NAC_BML(     SRC16, DST16, DSTP16, BLND ), 2, 2 )
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BML_ACK( SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BML(     SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BML_TCK( SRC16, DST16, DSTP16, BLND ), 2, 2 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BML(     SRC16, DST16, DSTP16, BLND ), 2, 2 )
						}
					}
				}else{
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_UAC_BML_ACK( SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_UAC_BML(     SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_NAC_BML_TCK( SRC16, DST16, DSTP16 ), 2, 2 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_NAC_BML(     SRC16, DST16, DSTP16 ), 2, 2 )
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BML_ACK( SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BML(     SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BML_TCK( SRC16, DST16, DSTP16 ), 2, 2 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BML(     SRC16, DST16, DSTP16 ), 2, 2 )
						}
					}
				}
				break ;

			case DX_BLENDMODE_MULA :	// ��Z+���u�����h
				if( BlendImg != NULL ){
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_UAC_BMA_ACK( SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_UAC_BMA(     SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_NAC_BMA_TCK( SRC16, DST16, DSTP16, BLND ), 2, 2 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_NAC_BMA(     SRC16, DST16, DSTP16, BLND ), 2, 2 )
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BMA_ACK( SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BMA(     SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BMA_TCK( SRC16, DST16, DSTP16, BLND ), 2, 2 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BMA(     SRC16, DST16, DSTP16, BLND ), 2, 2 )
						}
					}
				}else{
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_UAC_BMA_ACK( SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_UAC_BMA(     SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_NAC_BMA_TCK( SRC16, DST16, DSTP16 ), 2, 2 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_NAC_BMA(     SRC16, DST16, DSTP16 ), 2, 2 )
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BMA_ACK( SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BMA(     SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BMA_TCK( SRC16, DST16, DSTP16 ), 2, 2 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BMA(     SRC16, DST16, DSTP16 ), 2, 2 )
						}
					}
				}
				break ;

			case DX_BLENDMODE_INVSRC :		// �`�挳�̐F�𔽓]�{���u�����h
				if( BlendImg != NULL ){
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_UAC_BIS_ACK( SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_UAC_BIS(     SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_NAC_BIS_TCK( SRC16, DST16, DSTP16, BLND ), 2, 2 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_NAC_BIS(     SRC16, DST16, DSTP16, BLND ), 2, 2 )
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BIS_ACK( SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BIS(     SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BIS_TCK( SRC16, DST16, DSTP16, BLND ), 2, 2 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BIS(     SRC16, DST16, DSTP16, BLND ), 2, 2 )
						}
					}
				}else{
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_UAC_BIS_ACK( SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_UAC_BIS(     SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_NAC_BIS_TCK( SRC16, DST16, DSTP16 ), 2, 2 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_NAC_BIS(     SRC16, DST16, DSTP16 ), 2, 2 )
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BIS_ACK( SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BIS(     SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BIS_TCK( SRC16, DST16, DSTP16 ), 2, 2 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BIS(     SRC16, DST16, DSTP16 ), 2, 2 )
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
						if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C32_NBR_NAC_BNO( PALP32, DSTP32, BLND ), 1, 4 )
						else				DRAWROTATIONMEMIMG_UBI_ND( 								CODE_UBI_C32_NBR_NAC_BNO( PALP32, DSTP32, BLND ), 1, 4 )
					}else{
						if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C32_UBR_NAC_BNO( PALP32, DSTP32, BLND ), 1, 4 )
						else				DRAWROTATIONMEMIMG_UBI_ND( 								CODE_UBI_C32_UBR_NAC_BNO( PALP32, DSTP32, BLND ), 1, 4 )
					}
				}else{
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( TransFlag )
						{
#ifdef DX_NON_INLINE_ASM
							DRAWROTATIONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C32_NBR_NAC_BNO( PALP32, DSTP32 ), 1, 4 )
#else
							_asm
							{
								MOV EDI, DestPP
								MOV ESI, GDX2
								MOV EDX, GDY2
							LABEL_NBI_C32_NBR_NAC_BNO_PAL_TR_1:
								MOV ECX, EndC
							LABEL_NBI_C32_NBR_NAC_BNO_PAL_TR_2:
								CMP ESI, SizeX
								JAE LABEL_NBI_C32_NBR_NAC_BNO_PAL_TR_3
								CMP EDX, SizeY
								JAE LABEL_NBI_C32_NBR_NAC_BNO_PAL_TR_3

								MOV EBX, EDX
								MOV EAX, ESI
								SHR EBX, 18
								SHR EAX, 18
								MOV EBX, [SrcLineP + EBX * 4]
								MOVZX EBX, byte ptr [EAX + EBX]
								CMP EBX, TransColor
								JE LABEL_NBI_C32_NBR_NAC_BNO_PAL_TR_3
								MOV EAX, dword ptr [PaletteTemp + EBX * 4]
								MOV [EDI], EAX

							LABEL_NBI_C32_NBR_NAC_BNO_PAL_TR_3:
								ADD ESI, GAADX
								ADD EDX, GAADY
								ADD EDI, 4
								DEC ECX
								JNZ LABEL_NBI_C32_NBR_NAC_BNO_PAL_TR_2

								ADD ESI, GLADDX
								ADD EDX, GLADDY
								ADD EDI, DestAddPitch
								DEC C2
								JNZ LABEL_NBI_C32_NBR_NAC_BNO_PAL_TR_1
							}
#endif
						}
						else
						{
#ifdef DX_NON_INLINE_ASM
							DRAWROTATIONMEMIMG_NBI_ND( 								CODE_NBI_C32_NBR_NAC_BNO( PALP32, DSTP32 ), 1, 4 )
#else
							_asm
							{
								MOV EDI, DestPP
								MOV ESI, GDX2
								MOV EDX, GDY2
							LABEL_NBI_C32_NBR_NAC_BNO_PAL_NTR_1:
								MOV ECX, EndC
							LABEL_NBI_C32_NBR_NAC_BNO_PAL_NTR_2:
								CMP ESI, SizeX
								JAE LABEL_NBI_C32_NBR_NAC_BNO_PAL_NTR_3
								CMP EDX, SizeY
								JAE LABEL_NBI_C32_NBR_NAC_BNO_PAL_NTR_3

								MOV EBX, EDX
								MOV EAX, ESI
								SHR EBX, 18
								SHR EAX, 18
								MOV EBX, [SrcLineP + EBX * 4]
								MOVZX EBX, byte ptr [EAX + EBX]
								MOV EAX, dword ptr [PaletteTemp + EBX * 4]
								MOV [EDI], EAX

							LABEL_NBI_C32_NBR_NAC_BNO_PAL_NTR_3:
								ADD ESI, GAADX
								ADD EDX, GAADY
								ADD EDI, 4
								DEC ECX
								JNZ LABEL_NBI_C32_NBR_NAC_BNO_PAL_NTR_2

								ADD ESI, GLADDX
								ADD EDX, GLADDY
								ADD EDI, DestAddPitch
								DEC C2
								JNZ LABEL_NBI_C32_NBR_NAC_BNO_PAL_NTR_1
							}
#endif
						}
					}else{
						if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C32_UBR_NAC_BNO( PALP32, DSTP32 ), 1, 4 )
						else				DRAWROTATIONMEMIMG_NBI_ND( 								CODE_NBI_C32_UBR_NAC_BNO( PALP32, DSTP32 ), 1, 4 )
					}
				}
				break ;
				
			case DX_BLENDMODE_ALPHA :		// ���u�����h
				if( MemImgManage.BlendParam == 255 ) goto NOMALDRAW_C32_USEPAL_BNO ;
				else
				if( MemImgManage.BlendParam == 0 ) return ;
				
				if( BlendImg != NULL ){
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C32_NBR_NAC_BAL( PALP32, DSTP32, BLND ), 1, 4 )
						else				DRAWROTATIONMEMIMG_UBI_ND( 								CODE_UBI_C32_NBR_NAC_BAL( PALP32, DSTP32, BLND ), 1, 4 )
					}else{
						if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C32_UBR_NAC_BAL( PALP32, DSTP32, BLND ), 1, 4 )
						else				DRAWROTATIONMEMIMG_UBI_ND( 								CODE_UBI_C32_UBR_NAC_BAL( PALP32, DSTP32, BLND ), 1, 4 )
					}
				}else{
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C32_NBR_NAC_BAL( PALP32, DSTP32 ), 1, 4 )
						else				DRAWROTATIONMEMIMG_NBI_ND( 								CODE_NBI_C32_NBR_NAC_BAL( PALP32, DSTP32 ), 1, 4 )
					}else{
						if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C32_UBR_NAC_BAL( PALP32, DSTP32 ), 1, 4 )
						else				DRAWROTATIONMEMIMG_NBI_ND( 								CODE_NBI_C32_UBR_NAC_BAL( PALP32, DSTP32 ), 1, 4 )
					}
				}
				break ;
				
			case DX_BLENDMODE_ADD :			// ���Z�u�����h
				if( MemImgManage.BlendParam == 0 ) return ;
				
				if( BlendImg != NULL ){
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C32_NBR_NAC_BAD( PALP32, DSTP32, BLND ), 1, 4 )
						else				DRAWROTATIONMEMIMG_UBI_ND( 								CODE_UBI_C32_NBR_NAC_BAD( PALP32, DSTP32, BLND ), 1, 4 )
					}else{
						if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C32_UBR_NAC_BAD( PALP32, DSTP32, BLND ), 1, 4 )
						else				DRAWROTATIONMEMIMG_UBI_ND( 								CODE_UBI_C32_UBR_NAC_BAD( PALP32, DSTP32, BLND ), 1, 4 )
					}
				}else{
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C32_NBR_NAC_BAD( PALP32, DSTP32 ), 1, 4 )
						else				DRAWROTATIONMEMIMG_NBI_ND( 								CODE_NBI_C32_NBR_NAC_BAD( PALP32, DSTP32 ), 1, 4 )
					}else{
						if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C32_UBR_NAC_BAD( PALP32, DSTP32 ), 1, 4 )
						else				DRAWROTATIONMEMIMG_NBI_ND( 								CODE_NBI_C32_UBR_NAC_BAD( PALP32, DSTP32 ), 1, 4 )
					}
				}
				break ;

			case DX_BLENDMODE_SUB :			// ���Z�u�����h
				if( BlendImg != NULL ){
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C32_NBR_NAC_BSB( PALP32, DSTP32, BLND ), 1, 4 )
						else				DRAWROTATIONMEMIMG_UBI_ND( 								CODE_UBI_C32_NBR_NAC_BSB( PALP32, DSTP32, BLND ), 1, 4 )
					}else{
						if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C32_UBR_NAC_BSB( PALP32, DSTP32, BLND ), 1, 4 )
						else				DRAWROTATIONMEMIMG_UBI_ND( 								CODE_UBI_C32_UBR_NAC_BSB( PALP32, DSTP32, BLND ), 1, 4 )
					}
				}else{
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C32_NBR_NAC_BSB( PALP32, DSTP32 ), 1, 4 )
						else				DRAWROTATIONMEMIMG_NBI_ND( 								CODE_NBI_C32_NBR_NAC_BSB( PALP32, DSTP32 ), 1, 4 )
					}else{
						if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C32_UBR_NAC_BSB( PALP32, DSTP32 ), 1, 4 )
						else				DRAWROTATIONMEMIMG_NBI_ND( 								CODE_NBI_C32_UBR_NAC_BSB( PALP32, DSTP32 ), 1, 4 )
					}
				}
				break ;
				
			case DX_BLENDMODE_MUL :			// ��Z�u�����h
				if( BlendImg != NULL ){
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C32_NBR_NAC_BML( PALP32, DSTP32, BLND ), 1, 4 )
						else				DRAWROTATIONMEMIMG_UBI_ND( 								CODE_UBI_C32_NBR_NAC_BML( PALP32, DSTP32, BLND ), 1, 4 )
					}else{
						if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C32_UBR_NAC_BML( PALP32, DSTP32, BLND ), 1, 4 )
						else				DRAWROTATIONMEMIMG_UBI_ND( 								CODE_UBI_C32_UBR_NAC_BML( PALP32, DSTP32, BLND ), 1, 4 )
					}
				}else{
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C32_NBR_NAC_BML( PALP32, DSTP32 ), 1, 4 )
						else				DRAWROTATIONMEMIMG_NBI_ND( 								CODE_NBI_C32_NBR_NAC_BML( PALP32, DSTP32 ), 1, 4 )
					}else{
						if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C32_UBR_NAC_BML( PALP32, DSTP32 ), 1, 4 )
						else				DRAWROTATIONMEMIMG_NBI_ND( 								CODE_NBI_C32_UBR_NAC_BML( PALP32, DSTP32 ), 1, 4 )
					}
				}
				break ;

			case DX_BLENDMODE_MULA :		// ��Z+���u�����h
				if( BlendImg != NULL ){
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C32_NBR_NAC_BMA( PALP32, DSTP32, BLND ), 1, 4 )
						else				DRAWROTATIONMEMIMG_UBI_ND( 								CODE_UBI_C32_NBR_NAC_BMA( PALP32, DSTP32, BLND ), 1, 4 )
					}else{
						if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C32_UBR_NAC_BMA( PALP32, DSTP32, BLND ), 1, 4 )
						else				DRAWROTATIONMEMIMG_UBI_ND( 								CODE_UBI_C32_UBR_NAC_BMA( PALP32, DSTP32, BLND ), 1, 4 )
					}
				}else{
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C32_NBR_NAC_BMA( PALP32, DSTP32 ), 1, 4 )
						else				DRAWROTATIONMEMIMG_NBI_ND( 								CODE_NBI_C32_NBR_NAC_BMA( PALP32, DSTP32 ), 1, 4 )
					}else{
						if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C32_UBR_NAC_BMA( PALP32, DSTP32 ), 1, 4 )
						else				DRAWROTATIONMEMIMG_NBI_ND( 								CODE_NBI_C32_UBR_NAC_BMA( PALP32, DSTP32 ), 1, 4 )
					}
				}
				break ;

			case DX_BLENDMODE_INVSRC :		// �`�挳�̐F�𔽓]�{���u�����h
				if( BlendImg != NULL ){
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C32_NBR_NAC_BIS( PALP32, DSTP32, BLND ), 1, 4 )
						else				DRAWROTATIONMEMIMG_UBI_ND( 								CODE_UBI_C32_NBR_NAC_BIS( PALP32, DSTP32, BLND ), 1, 4 )
					}else{
						if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C32_UBR_NAC_BIS( PALP32, DSTP32, BLND ), 1, 4 )
						else				DRAWROTATIONMEMIMG_UBI_ND( 								CODE_UBI_C32_UBR_NAC_BIS( PALP32, DSTP32, BLND ), 1, 4 )
					}
				}else{
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C32_NBR_NAC_BIS( PALP32, DSTP32 ), 1, 4 )
						else				DRAWROTATIONMEMIMG_NBI_ND( 								CODE_NBI_C32_NBR_NAC_BIS( PALP32, DSTP32 ), 1, 4 )
					}else{
						if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C32_UBR_NAC_BIS( PALP32, DSTP32 ), 1, 4 )
						else				DRAWROTATIONMEMIMG_NBI_ND( 								CODE_NBI_C32_UBR_NAC_BIS( PALP32, DSTP32 ), 1, 4 )
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
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_UAC_BNO_ACK( SRCP32, DSTP32, BLND ), 4, 4 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_UAC_BNO(     SRCP32, DSTP32, BLND ), 4, 4 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_NAC_BNO_TCK( SRCP32, DSTP32, BLND ), 4, 4 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_NAC_BNO(     SRCP32, DSTP32, BLND ), 4, 4 )
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BNO_ACK( SRCP32, DSTP32, BLND ), 4, 4 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BNO(     SRCP32, DSTP32, BLND ), 4, 4 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BNO_TCK( SRCP32, DSTP32, BLND ), 4, 4 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BNO(     SRCP32, DSTP32, BLND ), 4, 4 )
						}
					}
				}else{
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_UAC_BNO_ACK( SRCP32, DSTP32 ), 4, 4 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_UAC_BNO(     SRCP32, DSTP32 ), 4, 4 )
						}else{
							if( TransFlag )
							{
#ifdef DX_NON_INLINE_ASM
								DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_NAC_BNO_TCK( SRCP32, DSTP32 ), 4, 4 )
#else
								_asm
								{
									MOV EDI, DestPP
									MOV ESI, GDX2
									MOV EDX, GDY2
								LABEL_NBI_C32_NBR_NAC_BNO_NPAL_TR_1:
									MOV ECX, EndC
								LABEL_NBI_C32_NBR_NAC_BNO_NPAL_TR_2:
									CMP ESI, SizeX
									JAE LABEL_NBI_C32_NBR_NAC_BNO_NPAL_TR_3
									CMP EDX, SizeY
									JAE LABEL_NBI_C32_NBR_NAC_BNO_NPAL_TR_3

									MOV EBX, EDX
									MOV EAX, ESI
									SHR EBX, 18
									SHR EAX, 16
									MOV EBX, [SrcLineP + EBX * 4]
									AND EAX, 0xfffffffc
									MOV EBX, [EAX + EBX]
									CMP EBX, TransColor
									JE LABEL_NBI_C32_NBR_NAC_BNO_NPAL_TR_3
									MOV [EDI], EBX

								LABEL_NBI_C32_NBR_NAC_BNO_NPAL_TR_3:
									ADD ESI, GAADX
									ADD EDX, GAADY
									ADD EDI, 4
									DEC ECX
									JNZ LABEL_NBI_C32_NBR_NAC_BNO_NPAL_TR_2

									ADD ESI, GLADDX
									ADD EDX, GLADDY
									ADD EDI, DestAddPitch
									DEC C2
									JNZ LABEL_NBI_C32_NBR_NAC_BNO_NPAL_TR_1
								}
#endif
							}
							else
							{
#ifdef DX_NON_INLINE_ASM
								DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_NAC_BNO(     SRCP32, DSTP32 ), 4, 4 )
#else
								_asm
								{
									MOV EDI, DestPP
									MOV ESI, GDX2
									MOV EDX, GDY2
								LABEL_NBI_C32_NBR_NAC_BNO_NPAL_NTR_1:
									MOV ECX, EndC
								LABEL_NBI_C32_NBR_NAC_BNO_NPAL_NTR_2:
									CMP ESI, SizeX
									JAE LABEL_NBI_C32_NBR_NAC_BNO_NPAL_NTR_3
									CMP EDX, SizeY
									JAE LABEL_NBI_C32_NBR_NAC_BNO_NPAL_NTR_3

									MOV EBX, EDX
									MOV EAX, ESI
									SHR EBX, 18
									SHR EAX, 16
									MOV EBX, [SrcLineP + EBX * 4]
									AND EAX, 0xfffffffc
									MOV EBX, [EAX + EBX]
									MOV [EDI], EBX

								LABEL_NBI_C32_NBR_NAC_BNO_NPAL_NTR_3:
									ADD ESI, GAADX
									ADD EDX, GAADY
									ADD EDI, 4
									DEC ECX
									JNZ LABEL_NBI_C32_NBR_NAC_BNO_NPAL_NTR_2

									ADD ESI, GLADDX
									ADD EDX, GLADDY
									ADD EDI, DestAddPitch
									DEC C2
									JNZ LABEL_NBI_C32_NBR_NAC_BNO_NPAL_NTR_1
								}
#endif
							}
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BNO_ACK( SRCP32, DSTP32 ), 4, 4 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BNO(     SRCP32, DSTP32 ), 4, 4 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BNO_TCK( SRCP32, DSTP32 ), 4, 4 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BNO(     SRCP32, DSTP32 ), 4, 4 )
						}
					}
				}
				break ;

			case DX_BLENDMODE_ALPHA :		// ���u�����h
				if( MemImgManage.BlendParam == 255 ) goto NOMALDRAW_C32_NOPAL_BNO ;
				else
				if( MemImgManage.BlendParam == 0 ) return ;
				
				if( BlendImg != NULL ){
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_UAC_BAL_ACK( SRCP32, DSTP32, BLND ), 4, 4 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_UAC_BAL(     SRCP32, DSTP32, BLND ), 4, 4 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_NAC_BAL_TCK( SRCP32, DSTP32, BLND ), 4, 4 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_NAC_BAL(     SRCP32, DSTP32, BLND ), 4, 4 )
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BAL_ACK( SRCP32, DSTP32, BLND ), 4, 4 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BAL(     SRCP32, DSTP32, BLND ), 4, 4 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BAL_TCK( SRCP32, DSTP32, BLND ), 4, 4 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BAL(     SRCP32, DSTP32, BLND ), 4, 4 )
						}
					}
				}else{
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_UAC_BAL_ACK( SRCP32, DSTP32 ), 4, 4 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_UAC_BAL(     SRCP32, DSTP32 ), 4, 4 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_NAC_BAL_TCK( SRCP32, DSTP32 ), 4, 4 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_NAC_BAL(     SRCP32, DSTP32 ), 4, 4 )
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BAL_ACK( SRCP32, DSTP32 ), 4, 4 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BAL(     SRCP32, DSTP32 ), 4, 4 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BAL_TCK( SRCP32, DSTP32 ), 4, 4 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BAL(     SRCP32, DSTP32 ), 4, 4 )
						}
					}
				}
				break ;

			case DX_BLENDMODE_ADD :		// ���Z�u�����h
				if( MemImgManage.BlendParam == 0 ) return ;
				
				if( BlendImg != NULL ){
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_UAC_BAD_ACK( SRCP32, DSTP32, BLND ), 4, 4 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_UAC_BAD(     SRCP32, DSTP32, BLND ), 4, 4 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_NAC_BAD_TCK( SRCP32, DSTP32, BLND ), 4, 4 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_NAC_BAD(     SRCP32, DSTP32, BLND ), 4, 4 )
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BAD_ACK( SRCP32, DSTP32, BLND ), 4, 4 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BAD(     SRCP32, DSTP32, BLND ), 4, 4 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BAD_TCK( SRCP32, DSTP32, BLND ), 4, 4 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BAD(     SRCP32, DSTP32, BLND ), 4, 4 )
						}
					}
				}else{
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_UAC_BAD_ACK( SRCP32, DSTP32 ), 4, 4 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_UAC_BAD(     SRCP32, DSTP32 ), 4, 4 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_NAC_BAD_TCK( SRCP32, DSTP32 ), 4, 4 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_NAC_BAD(     SRCP32, DSTP32 ), 4, 4 )
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BAD_ACK( SRCP32, DSTP32 ), 4, 4 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BAD(     SRCP32, DSTP32 ), 4, 4 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BAD_TCK( SRCP32, DSTP32 ), 4, 4 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BAD(     SRCP32, DSTP32 ), 4, 4 )
						}
					}
				}
				break ;

			case DX_BLENDMODE_SUB :		// ���Z�u�����h
				if( MemImgManage.BlendParam == 0 ) return ;
				
				if( BlendImg != NULL ){
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_UAC_BSB_ACK( SRCP32, DSTP32, BLND ), 4, 4 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_UAC_BSB(     SRCP32, DSTP32, BLND ), 4, 4 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_NAC_BSB_TCK( SRCP32, DSTP32, BLND ), 4, 4 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_NAC_BSB(     SRCP32, DSTP32, BLND ), 4, 4 )
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BSB_ACK( SRCP32, DSTP32, BLND ), 4, 4 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BSB(     SRCP32, DSTP32, BLND ), 4, 4 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BSB_TCK( SRCP32, DSTP32, BLND ), 4, 4 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BSB(     SRCP32, DSTP32, BLND ), 4, 4 )
						}
					}
				}else{
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_UAC_BSB_ACK( SRCP32, DSTP32 ), 4, 4 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_UAC_BSB(     SRCP32, DSTP32 ), 4, 4 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_NAC_BSB_TCK( SRCP32, DSTP32 ), 4, 4 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_NAC_BSB(     SRCP32, DSTP32 ), 4, 4 )
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BSB_ACK( SRCP32, DSTP32 ), 4, 4 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BSB(     SRCP32, DSTP32 ), 4, 4 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BSB_TCK( SRCP32, DSTP32 ), 4, 4 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BSB(     SRCP32, DSTP32 ), 4, 4 )
						}
					}
				}
				break ;

			case DX_BLENDMODE_MUL :		// ��Z�u�����h
				if( BlendImg != NULL ){
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_UAC_BML_ACK( SRCP32, DSTP32, BLND ), 4, 4 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_UAC_BML(     SRCP32, DSTP32, BLND ), 4, 4 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_NAC_BML_TCK( SRCP32, DSTP32, BLND ), 4, 4 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_NAC_BML(     SRCP32, DSTP32, BLND ), 4, 4 )
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BML_ACK( SRCP32, DSTP32, BLND ), 4, 4 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BML(     SRCP32, DSTP32, BLND ), 4, 4 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BML_TCK( SRCP32, DSTP32, BLND ), 4, 4 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BML(     SRCP32, DSTP32, BLND ), 4, 4 )
						}
					}
				}else{
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_UAC_BML_ACK( SRCP32, DSTP32 ), 4, 4 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_UAC_BML(     SRCP32, DSTP32 ), 4, 4 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_NAC_BML_TCK( SRCP32, DSTP32 ), 4, 4 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_NAC_BML(     SRCP32, DSTP32 ), 4, 4 )
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BML_ACK( SRCP32, DSTP32 ), 4, 4 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BML(     SRCP32, DSTP32 ), 4, 4 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BML_TCK( SRCP32, DSTP32 ), 4, 4 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BML(     SRCP32, DSTP32 ), 4, 4 )
						}
					}
				}
				break ;

			case DX_BLENDMODE_MULA :	// ��Z+���u�����h
				if( BlendImg != NULL ){
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_UAC_BMA_ACK( SRCP32, DSTP32, BLND ), 4, 4 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_UAC_BMA(     SRCP32, DSTP32, BLND ), 4, 4 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_NAC_BMA_TCK( SRCP32, DSTP32, BLND ), 4, 4 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_NAC_BMA(     SRCP32, DSTP32, BLND ), 4, 4 )
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BMA_ACK( SRCP32, DSTP32, BLND ), 4, 4 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BMA(     SRCP32, DSTP32, BLND ), 4, 4 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BMA_TCK( SRCP32, DSTP32, BLND ), 4, 4 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BMA(     SRCP32, DSTP32, BLND ), 4, 4 )
						}
					}
				}else{
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_UAC_BMA_ACK( SRCP32, DSTP32 ), 4, 4 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_UAC_BMA(     SRCP32, DSTP32 ), 4, 4 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_NAC_BMA_TCK( SRCP32, DSTP32 ), 4, 4 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_NAC_BMA(     SRCP32, DSTP32 ), 4, 4 )
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BMA_ACK( SRCP32, DSTP32 ), 4, 4 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BMA(     SRCP32, DSTP32 ), 4, 4 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BMA_TCK( SRCP32, DSTP32 ), 4, 4 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BMA(     SRCP32, DSTP32 ), 4, 4 )
						}
					}
				}
				break ;

			case DX_BLENDMODE_INVSRC :		// �`�挳�̐F�𔽓]�{���u�����h
				if( BlendImg != NULL ){
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_UAC_BIS_ACK( SRCP32, DSTP32, BLND ), 4, 4 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_UAC_BIS(     SRCP32, DSTP32, BLND ), 4, 4 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_NAC_BIS_TCK( SRCP32, DSTP32, BLND ), 4, 4 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_NAC_BIS(     SRCP32, DSTP32, BLND ), 4, 4 )
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BIS_ACK( SRCP32, DSTP32, BLND ), 4, 4 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BIS(     SRCP32, DSTP32, BLND ), 4, 4 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BIS_TCK( SRCP32, DSTP32, BLND ), 4, 4 )
							else				DRAWROTATIONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BIS(     SRCP32, DSTP32, BLND ), 4, 4 )
						}
					}
				}else{
					if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_UAC_BIS_ACK( SRCP32, DSTP32 ), 4, 4 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_UAC_BIS(     SRCP32, DSTP32 ), 4, 4 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_NAC_BIS_TCK( SRCP32, DSTP32 ), 4, 4 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_NAC_BIS(     SRCP32, DSTP32 ), 4, 4 )
						}
					}else{
						if( SrcImg->Base->UseAlpha == 1 ){
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BIS_ACK( SRCP32, DSTP32 ), 4, 4 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BIS(     SRCP32, DSTP32 ), 4, 4 )
						}else{
							if( TransFlag )		DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BIS_TCK( SRCP32, DSTP32 ), 4, 4 )
							else				DRAWROTATIONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BIS(     SRCP32, DSTP32 ), 4, 4 )
						}
					}
				}
				break ;
			}
		}
		break ;
	}

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



#define DRAWBASICPOLYGONMEMIMG_UBI_ND( DRAW, SRCADDNUM, DESTADDNUM )	\
{\
	for(;;){\
		BSX1    = B1X ;\
		DestBPT = ( BYTE * )DestBP + xx2 * (DESTADDNUM) + B1Y * DestPitch ;\
		\
		if( xx2 < DrawRectLeft ){\
			C       -= (DWORD)( DrawRectLeft - xx2 ) ;\
			DestBPT += (DWORD)( DrawRectLeft - xx2 ) * (DESTADDNUM) ;\
			xx2     += (DWORD)( DrawRectLeft - xx2 ) ;\
		}\
\
		GX = a * ( xx2 - x0 ) + b * ( B1Y - y0 ) + U00 ;\
		GY = c * ( xx2 - x0 ) + d * ( B1Y - y0 ) + V00 ;\
\
		C ++ ;\
		if( ( B1Y >= DrawRectTop && B1Y < DrawRectBottom ) && ( xx2 < DrawRectRight ) && C > 0 ){\
			do{\
				if( (DWORD)GX < SizeX && (DWORD)GY < SizeY ){\
					SrcBPT   = SrcBP   + (GX / QUALI) * (SRCADDNUM) + (GY / QUALI) * SrcPitch   ;\
					BlendBPT = BlendBP + (GX / QUALI)               + (GY / QUALI) * BlendPitch ;\
					DRAW\
				}\
				GX      += GAADX ;\
				GY      += GAADY ;\
				DestBPT += (DESTADDNUM) ;\
				xx2 ++ ;\
			}while( -- C && xx2 < DrawRectRight ) ;\
		}\
\
		C = ( B2X += BA2X ) / QUALI - ( xx2 = ( ( B1X = BSX1 + BA1X ) / QUALI ) ) ;\
		B1Y ++ ;\
		B2Y ++ ;\
\
		C1 -- ;\
		C2 -- ;\
\
		if( C1 == 0 ){\
			if( DMODE1 == 1 ) break ;\
			DMODE1 = 1 ;\
			BA1X   = BAA1X[1] ;\
			xx2    = ( B1X = BS1X[1] ) / QUALI ;\
			B1Y    = BS1Y[1] / QUALI ;\
			C1     = NextC1 ;\
			C      = B2X / QUALI - xx2 ;\
		}\
\
		if( C2 == 0 ){\
			if( DMODE2 == 1 ) break ;\
			DMODE2 = 1 ;\
			BA2X   = BAA2X[1] ;\
			B2X    = BS2X[1]  ;\
			B2Y    = BS2Y[1] / QUALI ;\
			C2     = NextC2 ;\
			C      = B2X / QUALI - xx2 ;\
		}\
	}\
}


#define DRAWBASICPOLYGONMEMIMG_NBI_ND( DRAW, SRCADDNUM, DESTADDNUM )	\
{\
	for(;;){\
		BSX1    = B1X ;\
		DestBPT = ( BYTE * )DestBP + xx2 * (DESTADDNUM) + B1Y * DestPitch ;\
		\
		if( xx2 < DrawRectLeft ){\
			C       -= (DWORD)( DrawRectLeft - xx2 ) ;\
			DestBPT += (DWORD)( DrawRectLeft - xx2 ) * (DESTADDNUM) ;\
			xx2     += (DWORD)( DrawRectLeft - xx2 ) ;\
		}\
\
		GX = a * ( xx2 - x0 ) + b * ( B1Y - y0 ) + U00 ;\
		GY = c * ( xx2 - x0 ) + d * ( B1Y - y0 ) + V00 ;\
\
		C ++ ;\
		if( ( B1Y >= DrawRectTop && B1Y < DrawRectBottom ) && ( xx2 < DrawRectRight ) && C > 0 ){\
			do{\
				if( (DWORD)GX < SizeX && (DWORD)GY < SizeY ){\
					SrcBPT = SrcBP + (GX / QUALI) * (SRCADDNUM) + (GY / QUALI) * SrcPitch ;\
					DRAW\
				}\
				GX      += GAADX ;\
				GY      += GAADY ;\
				DestBPT += (DESTADDNUM) ;\
				xx2 ++ ;\
			}while( -- C && xx2 < DrawRectRight ) ;\
		}\
\
		C = ( B2X += BA2X ) / QUALI - ( xx2 = ( ( B1X = BSX1 + BA1X ) / QUALI ) ) ;\
		B1Y ++ ;\
		B2Y ++ ;\
\
		C1 -- ;\
		C2 -- ;\
\
		if( C1 == 0 ){\
			if( DMODE1 == 1 ) break ;\
			DMODE1 = 1 ;\
			BA1X   = BAA1X[1] ;\
			xx2    = ( B1X = BS1X[1] ) / QUALI ;\
			B1Y    = BS1Y[1] / QUALI ;\
			C1     = NextC1 ;\
			C      = B2X / QUALI - xx2 ;\
		}\
\
		if( C2 == 0 ){\
			if( DMODE2 == 1 ) break ;\
			DMODE2 = 1 ;\
			BA2X   = BAA2X[1] ;\
			B2X    = BS2X[1]  ;\
			B2Y    = BS2Y[1] / QUALI ;\
			C2     = NextC2 ;\
			C      = B2X / QUALI - xx2 ;\
		}\
	}\
}


#define DRAWBASICPOLYGONMEMIMG_NTEX_NBI_ND( DRAW, DESTADDNUM )	\
{\
	for(;;){\
		BSX1 = B1X ;\
		DestBPT = ( BYTE * )DestBP + xx2 * (DESTADDNUM) + B1Y * DestPitch ;\
\
		if( xx2 < DrawRectLeft ){\
			C       -= (DWORD)( DrawRectLeft - xx2 ) ;\
			DestBPT += (DWORD)( DrawRectLeft - xx2 ) * (DESTADDNUM) ;\
			xx2     += (DWORD)( DrawRectLeft - xx2 ) ;\
		}\
\
		C ++ ;\
		if( ( B1Y >= DrawRectTop && B1Y < DrawRectBottom ) && ( xx2 < DrawRectRight ) && C > 0 ){\
			do{\
				DRAW\
				DestBPT += (DESTADDNUM) ;\
				xx2 ++ ;\
			}while( -- C && xx2 < DrawRectRight );\
		}\
\
		C = ( B2X += BA2X ) / QUALI - ( xx2 = ( ( B1X = BSX1 + BA1X ) / QUALI ) ) ;\
		B1Y ++ ;\
		B2Y ++ ;\
\
		C1 -- ;\
		C2 -- ;\
\
		if( C1 == 0 ){\
			if( DMODE1 == 1 ) break ;\
			DMODE1 = 1 ;\
			BA1X   = BAA1X[1] ;\
			xx2    = ( B1X = BS1X[1] ) / QUALI  ;\
			B1Y    = BS1Y[1] / QUALI ;\
			C1     = NextC1 ;\
			C      = B2X / QUALI - xx2 ;\
		}\
\
		if( C2 == 0 ){\
			if( DMODE2 == 1 ) break ;\
			DMODE2 = 1 ;\
			BA2X   = BAA2X[1] ;\
			B2X    = BS2X[1]  ;\
			B2Y    = BS2Y[1] / QUALI  ;\
			C2     = NextC2 ;\
			C      = B2X / QUALI - xx2 ;\
		}\
	}\
}




// �C���[�W��ό`�`�悷��
#define PAL16	PaletteWP[ *SrcBPT ]
#define DST16	*DestWPT
#define SRC16	*SrcWPT
#define SRCA16	SrcBPT[2]
#define DSTP16	DestWPT

#define PALP32	(BYTE *)&PaletteDP[ *SrcBPT ]
#define SRCP32	SrcBPT
#define DSTP32	DestBPT

#define BLND	*BlendBPT
extern void DrawBasicPolygonMemImg( MEMIMG *DestImg, const MEMIMG *SrcImg, const DX_POINTDATA *pos3, int TransFlag, const MEMIMG *BlendImg, unsigned int Color )
{
	DWORD *RateTable1 ;
	int *RateTable2 ;
	DWORD Src1, Src2, Data, DataD ;
	DWORD *RedTable , *GreenTable , *BlueTable ;
	DWORD SrcPitch, DestPitch, BlendPitch ;
	DWORD TransColor ;
	int GX, GY, GAADX, GAADY ;
	int BD1X[2],BD1Y[2],BD2X[2],BD2Y[2] ;	// �`��x�N�g���P�A�Q(���T�C�Y�P��+�Œ菬���_��1000�̈ʂɂ���)
											// �X�N���[����Ɍ��ĂP�͕`��J�n�_���獶�������ɐL�т�x�N�g��
											// �Q�͕`��J�n�_����E���ɕ����ɉ��т�x�N�g��
											// �S�͂Q�̃x�N�g�����獶�������ɉ��т�x�N�g��
	int	BS1X[2] ,BS1Y[2] ,BS2X[2] ,BS2Y[2];	// �`��J�n���W
	int B1X , B1Y , B2X , B2Y;				// �`�撆�ɃJ�E���^�Ƃ��Ďg���x�N�g���ϐ�(�`���p) 
	int BA1X , BA1Y , BA2X , BA2Y ;			// �g�p���̃X�N���[���`��p�x�N�g��
	int BAA1X[2], BAA1Y[2], BAA2X[2], BAA2Y[2] ;// �g�p���̃X�N���[���`��p�x�N�g��
	int DMODE1 , DMODE2 ;					// �`����(0.��ڂ̃x�N�g���ɉ����ĕ`�撆 1.2�ڂ̃x�N�g���̉����ĕ`�撆)
	int a, b, c, d, V00, U00, x0, y0 ;
	int DrawRectRight,  DrawRectLeft ;
	int DrawRectBottom, DrawRectTop ;
	int BSX1, xx2 ;
	int C1, NextC1, C2, NextC2, C ;
	DWORD SizeX , SizeY ;
	
	union
	{
		BYTE  ColorB[4] ;
		DWORD ColorD ;
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
	
	// �J���[�^�C�v���Ⴄ���A�`��悪���`�����l���t���p���b�g�t���������ꍇ�̓G���[
	if( SrcImg != NULL && SrcImg->Base->ColorType != DestImg->Base->ColorType ) return ;
	if( DestImg->Base->UsePalette == 1 || DestImg->Base->UseAlpha == 1 ) return ;

	// �`�揀��
	{
		int px[4],py[4],grx[4],gry[4] ;		// �`���`�̊e���_�A(����A�����A�E���A�E��̏�) 

		// �`����W�𓾂�
		for( int i = 0 ; i < 3 ; i ++ )
		{
			px[i]  = pos3[i].x * QUALI + QUALI / 2 ;
			py[i]  = pos3[i].y * QUALI + QUALI / 2 ;
			grx[i] = pos3[i].u * QUALI ;
			gry[i] = pos3[i].v * QUALI ;
		}

		// ��ԏ�ɗ��Ă�����W�ɂ���ĕ`��N�_���W��������
		{
			double VVx[2] , VVy[2] , Vx , Vy ; 
			int i , j , k ;
			int a1 , a2 , a3 , a4;
			int YTable[3] , XTable[3] ;
			int y1 , y2 , y3 , x1 , x2 , x3 ;

			DMODE1 = DMODE2 = 0 ;

			for( i = 0 ; i < 3 ; i ++ ) YTable[i] = XTable[i] = i ;

			for( i = 0 ; i < 3 ; i ++ )
			{
				for( j = i + 1 ; j < 3 ; j ++ )
				{
					if( px[XTable[i]] > px[XTable[j]] )
					{
						int a ;

						a = XTable[j] ; 
						XTable[j] = XTable[i] ;
						XTable[i] = a ;
					}

					if( py[YTable[i]] > py[YTable[j]] )
					{
						int a ;

						a = YTable[j] ; 
						YTable[j] = YTable[i] ;
						YTable[i] = a ;
					}
				}
			}

			y1 = YTable[0] ; y2 = YTable[1] ; y3 = YTable[2] ;  
			x1 = XTable[0] ; x2 = XTable[1] ; x3 = XTable[2] ;  

			// UV�Z�o�p�s�� a , b , c, d ���Z�o����
			{
				int dx01 , dx02 , dy01 , dy02 , du01 , du02 , dv01 , dv02 , tmp1 , tmp2  ;

				dx01 = ( px[x3] - px[x1] ) / QUALI ;	du01 = ( grx[x3] - grx[x1] ) / QUALI ;
				dx02 = ( px[x2] - px[x1] ) / QUALI ;	du02 = ( grx[x2] - grx[x1] ) / QUALI ;
				dy01 = ( py[x3] - py[x1] ) / QUALI ;	dv01 = ( gry[x3] - gry[x1] ) / QUALI ;
				dy02 = ( py[x2] - py[x1] ) / QUALI ;	dv02 = ( gry[x2] - gry[x1] ) / QUALI ;

				tmp1  = dx01 * dy02 - dx02 * dy01;
				if( tmp1 == 0 ) return ;
				tmp2  = dx02 * dy01 - dx01 * dy02;

				a = _DTOL( (double)(du01 * dy02 - du02 * dy01) * QUALI / tmp1 );
				b = _DTOL( (double)(du02 * dx01 - du01 * dx02) * QUALI / tmp1 );
				c = _DTOL( (double)(dv01 * dy02 - dv02 * dy01) * QUALI / tmp1 );
				d = _DTOL( (double)(dv02 * dx01 - dv01 * dx02) * QUALI / tmp1 );
				x0 = px[x1] / QUALI ;
				y0 = py[x1] / QUALI ;
				U00 = grx[x1] ;//- a * x0 - b * y0 ;
				V00 = gry[x1] ;//- c * x0 - d * y0 ;
			}

			// �C���M�����[�p����
			if( py[y1] == py[y2] )
			{
				// �㕔�Q���_�̂x�����������ꍇ�̏���
				DMODE1 = DMODE2 = 1 ;

				// �Q�Ɛi�ݒl�𓾂�
				{
					VVx[0] = ( double )( px[y2] - px[y3] ) / QUALI ; VVy[0] = ( double )( py[y2] - py[y3] ) / QUALI ;
					VVx[1] = ( double )( px[y1] - px[y3] ) / QUALI ; VVy[1] = ( double )( py[y1] - py[y3] ) / QUALI ;	

					if( VVy[0] == 0 || VVy[1] == 0 ) return ;

					VVx[0] /= VVy[0] ;
					VVy[0] /= VVy[0] ;

					VVx[1] /= VVy[1] ;
					VVy[1] /= VVy[1] ;

					if( px[y1] > px[y2] )
					{
						a1 = 0 ; a2 = 1 ;
						a3 = y1 ; a4 = y2 ;
					}
					else
					{
						a1 = 1 ; a2 = 0 ;
						a3 = y2 ; a4 = y1 ;
					}

					Vx = VVx[a1] - VVx[a2] ; Vy = VVy[a1] - VVy[a2] ;
				}

				BS1X[1] = px[a4] ;	BS1Y[1] = py[a4] ;

				BS2X[1] = px[a3] ;	BS2Y[1] = py[a3] ;


				BD1X[1] = px[y3] - BS1X[1] ;					
				BD1Y[1] = py[y3] - BS1Y[1] ;
				if( ( k = BD1Y[1] / QUALI ) == 0 ) return ;
				BAA1X[1] = BD1X[1] / k ;		
				BAA1Y[1] = QUALI ;

				BD2X[1] = px[y3] - BS2X[1] ;					
				BD2Y[1] = py[y3] - BS2Y[1] ;
				if( BD2Y[1] / QUALI == 0 ) return ;
				BAA2X[1] = BD2X[1] / ( BD2Y[1] / QUALI );		
				BAA2Y[1] = QUALI ;
			}
			else
			{
				// �����Q���_�̂x�����������ꍇ�̏���
				if( py[y2] == py[y3] )
				{
					DMODE1 = DMODE2 = 1 ;

					// �Q�Ɛi�ݒl�𓾂�
					{
						VVx[0] = ( double )( px[y2] - px[y1] ) / QUALI ; VVy[0] = ( double )( py[y2] - py[y1] ) / QUALI ;
						VVx[1] = ( double )( px[y3] - px[y1] ) / QUALI ; VVy[1] = ( double )( py[y3] - py[y1] ) / QUALI ;	

						if( VVy[0] == 0 || VVy[1] == 0 ) return ;
						VVx[0] /= VVy[0] ;

						VVx[1] /= VVy[1] ;

						BS1X[1] = BS2X[1] = px[y1] ;
						BS1Y[1] = BS2Y[1] = py[y1] ;

						if( px[y2] > px[y3] )
						{	
							a1 = 0 ; a2 = 1 ;
							a3 = y3 ; a4 = y2 ;
						}
						else
						{
							a1 = 1 ; a2 = 0 ;
							a3 = y2 ; a4 = y3 ;
						}

						Vx = VVx[a1] - VVx[a2] ; Vy = VVy[a1] - VVy[a2] ;

						BD1X[1] = px[a3] - BS1X[1] ;
						BD1Y[1] = py[a3] - BS1Y[1] ;

						BD2X[1] = px[a4] - BS2X[1] ;
						BD2Y[1] = py[a4] - BS2Y[1] ;
					}

					if( ( k = BD1Y[1] / QUALI ) == 0 ) return ;
					BAA1X[1] = BD1X[1] / k ;
					BAA1Y[1] = QUALI ;

					if( BD2Y[1] / QUALI == 0 ) return ;
					BAA2X[1] = BD2X[1] / ( BD2Y[1] / QUALI ) ;
					BAA2Y[1] = QUALI ;
				}
				else
				{
					double ax  , bx ;

					if( ( ( ax = ( ( double )( py[y2] - py[y1] ) / QUALI ) ) == 0.0 ) || 
						( ( bx = ( ( double )( py[y3] - py[y1] ) / QUALI ) ) == 0.0 ) ) return ;

					ax = ( double )( px[y2] - px[y1] ) / ax ;
					bx = ( double )( px[y3] - px[y1] ) / bx ;

					// �ʏ폈��
					VVx[0] = ( double )( px[y2] - px[y1] ) / QUALI ; VVy[0] = ( double )( py[y2] - py[y1] ) / QUALI ;
					VVx[1] = ( double )( px[y3] - px[y1] ) / QUALI ; VVy[1] = ( double )( py[y3] - py[y1] ) / QUALI ;	

					if( VVy[0] == 0 || VVy[1] == 0 ) return ;
					VVx[0] /= VVy[0] ;
					VVy[0] /= VVy[0] ;

					VVx[1] /= VVy[1] ; 
					VVy[1] /= VVy[1] ;


					if( ax < bx ) 
					{
						DMODE1 = 0 ;
						DMODE2 = 1 ;

						// �Q�Ɛi�ݒl�𓾂�
						Vx = VVx[1] - VVx[0] ; Vy = VVy[1] - VVy[0] ;

						BS1X[0] = BS2X[1] = px[y1] ;
						BS1Y[0] = BS2Y[1] = py[y1] ;

						BD1X[0] = px[y2] - BS1X[0] ; 
						BD1Y[0] = py[y2] - BS1Y[0] ;

						if( ( k = BD1Y[0] / QUALI ) == 0 ) return ;
						BAA1X[0] = ( int )( BD1X[0] / k ) ;
						BAA1Y[0] = QUALI ;

						BS1X[1] = px[y2] ;
						BS1Y[1] = py[y2] ;
						BD1X[1] = px[y3] - BS1X[1] ; 
						BD1Y[1] = py[y3] - BS1Y[1] ;

						if( ( k = BD1Y[1] / QUALI ) == 0 ) return ;
						BAA1X[1] = ( int )( BD1X[1] / k ) ;
						BAA1Y[1] = QUALI ;

						BD2X[1] = px[y3] - BS2X[1] ; 
						BD2Y[1] = py[y3] - BS2Y[1] ;
						if( ( BD2Y[1] / QUALI ) == 0 ) return ;
						BAA2X[1] = ( int )( BD2X[1] / ( BD2Y[1] / QUALI ) ) ;
						BAA2Y[1] = QUALI ;
					}
					else
					{
						DMODE1 = 1 ;
						DMODE2 = 0 ;

						// �Q�Ɛi�ݒl�𓾂�
						Vx = VVx[0] - VVx[1] ; Vy = VVy[0] - VVy[1] ;

						BS1X[1] = BS2X[0] = px[y1] ;
						BS1Y[1] = BS2Y[0] = py[y1] ;

						BD2X[0] = px[y2] - BS2X[0] ; 
						BD2Y[0] = py[y2] - BS2Y[0] ;
						if( ( BD2Y[0] / QUALI ) == 0 ) return ;
						BAA2X[0] = BD2X[0] / ( BD2Y[0] / QUALI ) ;
						BAA2Y[0] = QUALI ;

						BS2X[1] = px[y2] + 1 ;
						BS2Y[1] = py[y2] ;
						BD2X[1] = px[y3] - BS2X[1] ; 
						BD2Y[1] = py[y3] - BS2Y[1] ;
						if( ( BD2Y[1] / QUALI ) == 0 ) return ;
						BAA2X[1] = BD2X[1] / ( BD2Y[1] / QUALI ) ;
						BAA2Y[1] = QUALI ;

						BD1X[1] = px[y3] - BS1X[1] ; 
						BD1Y[1] = py[y3] - BS1Y[1] ;

						if( ( k = BD1Y[1] / QUALI ) == 0 ) return ;
						BAA1X[1] = BD1X[1] / k ;
						BAA1Y[1] = QUALI ;
					}
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

	if( BlendImg != NULL )
	{
		BlendBP    = BlendImg->UseImage ;
		BlendPitch = BlendImg->Base->Pitch ;
	}

	if( SrcImg != NULL )
	{
		TransColor = SrcImg->Base->TransColor ;
		PaletteBP  = (BYTE *)SrcImg->Base->Palette ;
	}
	else
	{
		PaletteBP  = NULL ;
	}
	RateTable1 = MemImgManage.RateTable[MemImgManage.BlendParam] ;
	RateTable2 = &MemImgManage.RateTable2[MemImgManage.BlendParam][256] ;
	RedTable   = MemImgManage.RateTable[MemImgManage.DrawBright.Red] ;
	GreenTable = MemImgManage.RateTable[MemImgManage.DrawBright.Green] ;
	BlueTable  = MemImgManage.RateTable[MemImgManage.DrawBright.Blue] ;
	
	// �`�揀��
	{
		B1X = BS1X[DMODE1] ; B1Y = BS1Y[DMODE1] ;
		B2X = BS2X[DMODE2] ; B2Y = BS2Y[DMODE2] ;

		BA1X = BAA1X[DMODE1] ; BA1Y = BAA1Y[DMODE1] ;
		BA2X = BAA2X[DMODE2] ; BA2Y = BAA2Y[DMODE2] ;

		DrawRectRight  = MemImgManage.DrawArea.right ;
		DrawRectLeft   = MemImgManage.DrawArea.left ;
		DrawRectBottom = MemImgManage.DrawArea.bottom ;
		DrawRectTop    = MemImgManage.DrawArea.top ;
		GAADX = a ;
		GAADY = c ;

		NextC1 = ( DMODE1 == 0 ) ? BD1Y[1] / QUALI : 0 ;
		NextC2 = ( DMODE2 == 0 ) ? BD2Y[1] / QUALI : 0 ;
		C1 = BD1Y[DMODE1] / QUALI ;
		C2 = BD2Y[DMODE2] / QUALI ;

		B1Y /= QUALI ;
		C = B2X / QUALI - B1X / QUALI ; 

		xx2 = B1X / QUALI ;
	}

	// �摜������ꍇ�Ɩ����ꍇ�ŏ����𕪊�
	if( SrcImg != NULL )
	{
		SizeX = (DWORD)SrcImg->Width  * QUALI ;
		SizeY = (DWORD)SrcImg->Height * QUALI ;
		
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
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C16_NBR_NAC_BNO( PAL16, DST16, DSTP16, BLND ), 1, 2 )
							else				DRAWBASICPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C16_NBR_NAC_BNO( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						}else{
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C16_UBR_NAC_BNO( PAL16, DST16, DSTP16, BLND ), 1, 2 )
							else				DRAWBASICPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C16_UBR_NAC_BNO( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C16_NBR_NAC_BNO( PAL16, DST16, DSTP16 ), 1, 2 )
							else				DRAWBASICPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C16_NBR_NAC_BNO( PAL16, DST16, DSTP16 ), 1, 2 )
						}else{
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C16_UBR_NAC_BNO( PAL16, DST16, DSTP16 ), 1, 2 )
							else				DRAWBASICPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C16_UBR_NAC_BNO( PAL16, DST16, DSTP16 ), 1, 2 )
						}
					}
					break ;
					
				case DX_BLENDMODE_ALPHA :		// ���u�����h
					if( MemImgManage.BlendParam == 255 ) goto NOMALDRAW_C16_USEPAL_BNO ;
					else
					if( MemImgManage.BlendParam == 0 ) return ;
					
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C16_NBR_NAC_BAL( PAL16, DST16, DSTP16, BLND ), 1, 2 )
							else				DRAWBASICPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C16_NBR_NAC_BAL( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						}else{
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C16_UBR_NAC_BAL( PAL16, DST16, DSTP16, BLND ), 1, 2 )
							else				DRAWBASICPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C16_UBR_NAC_BAL( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C16_NBR_NAC_BAL( PAL16, DST16, DSTP16 ), 1, 2 )
							else				DRAWBASICPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C16_NBR_NAC_BAL( PAL16, DST16, DSTP16 ), 1, 2 )
						}else{
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C16_UBR_NAC_BAL( PAL16, DST16, DSTP16 ), 1, 2 )
							else				DRAWBASICPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C16_UBR_NAC_BAL( PAL16, DST16, DSTP16 ), 1, 2 )
						}
					}
					break ;
					
				case DX_BLENDMODE_ADD :			// ���Z�u�����h
					if( MemImgManage.BlendParam == 0 ) return ;
					
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C16_NBR_NAC_BAD( PAL16, DST16, DSTP16, BLND ), 1, 2 )
							else				DRAWBASICPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C16_NBR_NAC_BAD( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						}else{
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C16_UBR_NAC_BAD( PAL16, DST16, DSTP16, BLND ), 1, 2 )
							else				DRAWBASICPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C16_UBR_NAC_BAD( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C16_NBR_NAC_BAD( PAL16, DST16, DSTP16 ), 1, 2 )
							else				DRAWBASICPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C16_NBR_NAC_BAD( PAL16, DST16, DSTP16 ), 1, 2 )
						}else{
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C16_UBR_NAC_BAD( PAL16, DST16, DSTP16 ), 1, 2 )
							else				DRAWBASICPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C16_UBR_NAC_BAD( PAL16, DST16, DSTP16 ), 1, 2 )
						}
					}
					break ;

				case DX_BLENDMODE_SUB :			// ���Z�u�����h
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C16_NBR_NAC_BSB( PAL16, DST16, DSTP16, BLND ), 1, 2 )
							else				DRAWBASICPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C16_NBR_NAC_BSB( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						}else{
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C16_UBR_NAC_BSB( PAL16, DST16, DSTP16, BLND ), 1, 2 )
							else				DRAWBASICPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C16_UBR_NAC_BSB( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C16_NBR_NAC_BSB( PAL16, DST16, DSTP16 ), 1, 2 )
							else				DRAWBASICPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C16_NBR_NAC_BSB( PAL16, DST16, DSTP16 ), 1, 2 )
						}else{
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C16_UBR_NAC_BSB( PAL16, DST16, DSTP16 ), 1, 2 )
							else				DRAWBASICPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C16_UBR_NAC_BSB( PAL16, DST16, DSTP16 ), 1, 2 )
						}
					}
					break ;
					
				case DX_BLENDMODE_MUL :			// ��Z�u�����h
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C16_NBR_NAC_BML( PAL16, DST16, DSTP16, BLND ), 1, 2 )
							else				DRAWBASICPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C16_NBR_NAC_BML( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						}else{
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C16_UBR_NAC_BML( PAL16, DST16, DSTP16, BLND ), 1, 2 )
							else				DRAWBASICPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C16_UBR_NAC_BML( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C16_NBR_NAC_BML( PAL16, DST16, DSTP16 ), 1, 2 )
							else				DRAWBASICPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C16_NBR_NAC_BML( PAL16, DST16, DSTP16 ), 1, 2 )
						}else{
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C16_UBR_NAC_BML( PAL16, DST16, DSTP16 ), 1, 2 )
							else				DRAWBASICPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C16_UBR_NAC_BML( PAL16, DST16, DSTP16 ), 1, 2 )
						}
					}
					break ;

				case DX_BLENDMODE_MULA :		// ��Z+���u�����h
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C16_NBR_NAC_BMA( PAL16, DST16, DSTP16, BLND ), 1, 2 )
							else				DRAWBASICPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C16_NBR_NAC_BMA( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						}else{
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C16_UBR_NAC_BMA( PAL16, DST16, DSTP16, BLND ), 1, 2 )
							else				DRAWBASICPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C16_UBR_NAC_BMA( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C16_NBR_NAC_BMA( PAL16, DST16, DSTP16 ), 1, 2 )
							else				DRAWBASICPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C16_NBR_NAC_BMA( PAL16, DST16, DSTP16 ), 1, 2 )
						}else{
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C16_UBR_NAC_BMA( PAL16, DST16, DSTP16 ), 1, 2 )
							else				DRAWBASICPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C16_UBR_NAC_BMA( PAL16, DST16, DSTP16 ), 1, 2 )
						}
					}
					break ;

				case DX_BLENDMODE_INVSRC :		// �`�挳�̐F�𔽓]�{���u�����h
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C16_NBR_NAC_BIS( PAL16, DST16, DSTP16, BLND ), 1, 2 )
							else				DRAWBASICPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C16_NBR_NAC_BIS( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						}else{
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C16_UBR_NAC_BIS( PAL16, DST16, DSTP16, BLND ), 1, 2 )
							else				DRAWBASICPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C16_UBR_NAC_BIS( PAL16, DST16, DSTP16, BLND ), 1, 2 )
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C16_NBR_NAC_BIS( PAL16, DST16, DSTP16 ), 1, 2 )
							else				DRAWBASICPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C16_NBR_NAC_BIS( PAL16, DST16, DSTP16 ), 1, 2 )
						}else{
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C16_UBR_NAC_BIS( PAL16, DST16, DSTP16 ), 1, 2 )
							else				DRAWBASICPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C16_UBR_NAC_BIS( PAL16, DST16, DSTP16 ), 1, 2 )
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
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_UAC_BNO_ACK( SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_UAC_BNO(     SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_NAC_BNO_TCK( SRC16, DST16, DSTP16, BLND ), 2, 2 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_NAC_BNO(     SRC16, DST16, DSTP16, BLND ), 2, 2 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BNO_ACK( SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BNO(     SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BNO_TCK( SRC16, DST16, DSTP16, BLND ), 2, 2 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BNO(     SRC16, DST16, DSTP16, BLND ), 2, 2 )
							}
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_UAC_BNO_ACK( SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_UAC_BNO(     SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_NAC_BNO_TCK( SRC16, DST16, DSTP16 ), 2, 2 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_NAC_BNO(     SRC16, DST16, DSTP16 ), 2, 2 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BNO_ACK( SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BNO(     SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BNO_TCK( SRC16, DST16, DSTP16 ), 2, 2 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BNO(     SRC16, DST16, DSTP16 ), 2, 2 )
							}
						}
					}
					break ;

				case DX_BLENDMODE_ALPHA :		// ���u�����h
					if( MemImgManage.BlendParam == 255 ) goto NOMALDRAW_C16_NOPAL_BNO ;
					else
					if( MemImgManage.BlendParam == 0 ) return ;
					
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_UAC_BAL_ACK( SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_UAC_BAL(     SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_NAC_BAL_TCK( SRC16, DST16, DSTP16, BLND ), 2, 2 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_NAC_BAL(     SRC16, DST16, DSTP16, BLND ), 2, 2 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BAL_ACK( SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BAL(     SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BAL_TCK( SRC16, DST16, DSTP16, BLND ), 2, 2 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BAL(     SRC16, DST16, DSTP16, BLND ), 2, 2 )
							}
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_UAC_BAL_ACK( SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_UAC_BAL(     SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_NAC_BAL_TCK( SRC16, DST16, DSTP16 ), 2, 2 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_NAC_BAL(     SRC16, DST16, DSTP16 ), 2, 2 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BAL_ACK( SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BAL(     SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BAL_TCK( SRC16, DST16, DSTP16 ), 2, 2 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BAL(     SRC16, DST16, DSTP16 ), 2, 2 )
							}
						}
					}
					break ;

				case DX_BLENDMODE_ADD :		// ���Z�u�����h
					if( MemImgManage.BlendParam == 0 ) return ;
					
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_UAC_BAD_ACK( SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_UAC_BAD(     SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_NAC_BAD_TCK( SRC16, DST16, DSTP16, BLND ), 2, 2 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_NAC_BAD(     SRC16, DST16, DSTP16, BLND ), 2, 2 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BAD_ACK( SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BAD(     SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BAD_TCK( SRC16, DST16, DSTP16, BLND ), 2, 2 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BAD(     SRC16, DST16, DSTP16, BLND ), 2, 2 )
							}
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_UAC_BAD_ACK( SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_UAC_BAD(     SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_NAC_BAD_TCK( SRC16, DST16, DSTP16 ), 2, 2 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_NAC_BAD(     SRC16, DST16, DSTP16 ), 2, 2 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BAD_ACK( SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BAD(     SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BAD_TCK( SRC16, DST16, DSTP16 ), 2, 2 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BAD(     SRC16, DST16, DSTP16 ), 2, 2 )
							}
						}
					}
					break ;

				case DX_BLENDMODE_SUB :		// ���Z�u�����h
					if( MemImgManage.BlendParam == 0 ) return ;
					
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_UAC_BSB_ACK( SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_UAC_BSB(     SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_NAC_BSB_TCK( SRC16, DST16, DSTP16, BLND ), 2, 2 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_NAC_BSB(     SRC16, DST16, DSTP16, BLND ), 2, 2 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BSB_ACK( SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BSB(     SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BSB_TCK( SRC16, DST16, DSTP16, BLND ), 2, 2 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BSB(     SRC16, DST16, DSTP16, BLND ), 2, 2 )
							}
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_UAC_BSB_ACK( SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_UAC_BSB(     SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_NAC_BSB_TCK( SRC16, DST16, DSTP16 ), 2, 2 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_NAC_BSB(     SRC16, DST16, DSTP16 ), 2, 2 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BSB_ACK( SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BSB(     SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BSB_TCK( SRC16, DST16, DSTP16 ), 2, 2 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BSB(     SRC16, DST16, DSTP16 ), 2, 2 )
							}
						}
					}
					break ;

				case DX_BLENDMODE_MUL :		// ��Z�u�����h
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_UAC_BML_ACK( SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_UAC_BML(     SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_NAC_BML_TCK( SRC16, DST16, DSTP16, BLND ), 2, 2 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_NAC_BML(     SRC16, DST16, DSTP16, BLND ), 2, 2 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BML_ACK( SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BML(     SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BML_TCK( SRC16, DST16, DSTP16, BLND ), 2, 2 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BML(     SRC16, DST16, DSTP16, BLND ), 2, 2 )
							}
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_UAC_BML_ACK( SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_UAC_BML(     SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_NAC_BML_TCK( SRC16, DST16, DSTP16 ), 2, 2 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_NAC_BML(     SRC16, DST16, DSTP16 ), 2, 2 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BML_ACK( SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BML(     SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BML_TCK( SRC16, DST16, DSTP16 ), 2, 2 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BML(     SRC16, DST16, DSTP16 ), 2, 2 )
							}
						}
					}
					break ;

				case DX_BLENDMODE_MULA :	// ��Z+���u�����h
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_UAC_BMA_ACK( SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_UAC_BMA(     SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_NAC_BMA_TCK( SRC16, DST16, DSTP16, BLND ), 2, 2 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_NAC_BMA(     SRC16, DST16, DSTP16, BLND ), 2, 2 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BMA_ACK( SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BMA(     SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BMA_TCK( SRC16, DST16, DSTP16, BLND ), 2, 2 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BMA(     SRC16, DST16, DSTP16, BLND ), 2, 2 )
							}
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_UAC_BMA_ACK( SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_UAC_BMA(     SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_NAC_BMA_TCK( SRC16, DST16, DSTP16 ), 2, 2 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_NAC_BMA(     SRC16, DST16, DSTP16 ), 2, 2 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BMA_ACK( SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BMA(     SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BMA_TCK( SRC16, DST16, DSTP16 ), 2, 2 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BMA(     SRC16, DST16, DSTP16 ), 2, 2 )
							}
						}
					}
					break ;

				case DX_BLENDMODE_INVSRC :		// �`�挳�̐F�𔽓]�{���u�����h
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_UAC_BIS_ACK( SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_UAC_BIS(     SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_NAC_BIS_TCK( SRC16, DST16, DSTP16, BLND ), 2, 2 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_NBR_NAC_BIS(     SRC16, DST16, DSTP16, BLND ), 2, 2 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BIS_ACK( SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_UAC_BIS(     SRC16, SRCA16, DST16, DSTP16, BLND ), 4, 2 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BIS_TCK( SRC16, DST16, DSTP16, BLND ), 2, 2 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C16_UBR_NAC_BIS(     SRC16, DST16, DSTP16, BLND ), 2, 2 )
							}
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_UAC_BIS_ACK( SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_UAC_BIS(     SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_NAC_BIS_TCK( SRC16, DST16, DSTP16 ), 2, 2 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_NBR_NAC_BIS(     SRC16, DST16, DSTP16 ), 2, 2 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BIS_ACK( SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_UAC_BIS(     SRC16, SRCA16, DST16, DSTP16 ), 4, 2 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BIS_TCK( SRC16, DST16, DSTP16 ), 2, 2 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C16_UBR_NAC_BIS(     SRC16, DST16, DSTP16 ), 2, 2 )
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
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C32_NBR_NAC_BNO( PALP32, DSTP32, BLND ), 1, 4 )
							else				DRAWBASICPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C32_NBR_NAC_BNO( PALP32, DSTP32, BLND ), 1, 4 )
						}else{
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C32_UBR_NAC_BNO( PALP32, DSTP32, BLND ), 1, 4 )
							else				DRAWBASICPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C32_UBR_NAC_BNO( PALP32, DSTP32, BLND ), 1, 4 )
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C32_NBR_NAC_BNO( PALP32, DSTP32 ), 1, 4 )
							else				DRAWBASICPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C32_NBR_NAC_BNO( PALP32, DSTP32 ), 1, 4 )
						}else{
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C32_UBR_NAC_BNO( PALP32, DSTP32 ), 1, 4 )
							else				DRAWBASICPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C32_UBR_NAC_BNO( PALP32, DSTP32 ), 1, 4 )
						}
					}
					break ;
					
				case DX_BLENDMODE_ALPHA :		// ���u�����h
					if( MemImgManage.BlendParam == 255 ) goto NOMALDRAW_C32_USEPAL_BNO ;
					else
					if( MemImgManage.BlendParam == 0 ) return ;
					
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C32_NBR_NAC_BAL( PALP32, DSTP32, BLND ), 1, 4 )
							else				DRAWBASICPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C32_NBR_NAC_BAL( PALP32, DSTP32, BLND ), 1, 4 )
						}else{
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C32_UBR_NAC_BAL( PALP32, DSTP32, BLND ), 1, 4 )
							else				DRAWBASICPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C32_UBR_NAC_BAL( PALP32, DSTP32, BLND ), 1, 4 )
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C32_NBR_NAC_BAL( PALP32, DSTP32 ), 1, 4 )
							else				DRAWBASICPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C32_NBR_NAC_BAL( PALP32, DSTP32 ), 1, 4 )
						}else{
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C32_UBR_NAC_BAL( PALP32, DSTP32 ), 1, 4 )
							else				DRAWBASICPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C32_UBR_NAC_BAL( PALP32, DSTP32 ), 1, 4 )
						}
					}
					break ;
					
				case DX_BLENDMODE_ADD :			// ���Z�u�����h
					if( MemImgManage.BlendParam == 0 ) return ;
					
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C32_NBR_NAC_BAD( PALP32, DSTP32, BLND ), 1, 4 )
							else				DRAWBASICPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C32_NBR_NAC_BAD( PALP32, DSTP32, BLND ), 1, 4 )
						}else{
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C32_UBR_NAC_BAD( PALP32, DSTP32, BLND ), 1, 4 )
							else				DRAWBASICPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C32_UBR_NAC_BAD( PALP32, DSTP32, BLND ), 1, 4 )
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C32_NBR_NAC_BAD( PALP32, DSTP32 ), 1, 4 )
							else				DRAWBASICPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C32_NBR_NAC_BAD( PALP32, DSTP32 ), 1, 4 )
						}else{
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C32_UBR_NAC_BAD( PALP32, DSTP32 ), 1, 4 )
							else				DRAWBASICPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C32_UBR_NAC_BAD( PALP32, DSTP32 ), 1, 4 )
						}
					}
					break ;

				case DX_BLENDMODE_SUB :			// ���Z�u�����h
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C32_NBR_NAC_BSB( PALP32, DSTP32, BLND ), 1, 4 )
							else				DRAWBASICPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C32_NBR_NAC_BSB( PALP32, DSTP32, BLND ), 1, 4 )
						}else{
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C32_UBR_NAC_BSB( PALP32, DSTP32, BLND ), 1, 4 )
							else				DRAWBASICPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C32_UBR_NAC_BSB( PALP32, DSTP32, BLND ), 1, 4 )
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C32_NBR_NAC_BSB( PALP32, DSTP32 ), 1, 4 )
							else				DRAWBASICPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C32_NBR_NAC_BSB( PALP32, DSTP32 ), 1, 4 )
						}else{
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C32_UBR_NAC_BSB( PALP32, DSTP32 ), 1, 4 )
							else				DRAWBASICPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C32_UBR_NAC_BSB( PALP32, DSTP32 ), 1, 4 )
						}
					}
					break ;
					
				case DX_BLENDMODE_MUL :			// ��Z�u�����h
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C32_NBR_NAC_BML( PALP32, DSTP32, BLND ), 1, 4 )
							else				DRAWBASICPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C32_NBR_NAC_BML( PALP32, DSTP32, BLND ), 1, 4 )
						}else{
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C32_UBR_NAC_BML( PALP32, DSTP32, BLND ), 1, 4 )
							else				DRAWBASICPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C32_UBR_NAC_BML( PALP32, DSTP32, BLND ), 1, 4 )
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C32_NBR_NAC_BML( PALP32, DSTP32 ), 1, 4 )
							else				DRAWBASICPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C32_NBR_NAC_BML( PALP32, DSTP32 ), 1, 4 )
						}else{
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C32_UBR_NAC_BML( PALP32, DSTP32 ), 1, 4 )
							else				DRAWBASICPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C32_UBR_NAC_BML( PALP32, DSTP32 ), 1, 4 )
						}
					}
					break ;

				case DX_BLENDMODE_MULA :		// ��Z+���u�����h
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C32_NBR_NAC_BMA( PALP32, DSTP32, BLND ), 1, 4 )
							else				DRAWBASICPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C32_NBR_NAC_BMA( PALP32, DSTP32, BLND ), 1, 4 )
						}else{
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C32_UBR_NAC_BMA( PALP32, DSTP32, BLND ), 1, 4 )
							else				DRAWBASICPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C32_UBR_NAC_BMA( PALP32, DSTP32, BLND ), 1, 4 )
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C32_NBR_NAC_BMA( PALP32, DSTP32 ), 1, 4 )
							else				DRAWBASICPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C32_NBR_NAC_BMA( PALP32, DSTP32 ), 1, 4 )
						}else{
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C32_UBR_NAC_BMA( PALP32, DSTP32 ), 1, 4 )
							else				DRAWBASICPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C32_UBR_NAC_BMA( PALP32, DSTP32 ), 1, 4 )
						}
					}
					break ;

				case DX_BLENDMODE_INVSRC :		// �`�挳�̐F�𔽓]�{���u�����h
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C32_NBR_NAC_BIS( PALP32, DSTP32, BLND ), 1, 4 )
							else				DRAWBASICPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C32_NBR_NAC_BIS( PALP32, DSTP32, BLND ), 1, 4 )
						}else{
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( if( *SrcBPT != TransColor )	CODE_UBI_C32_UBR_NAC_BIS( PALP32, DSTP32, BLND ), 1, 4 )
							else				DRAWBASICPOLYGONMEMIMG_UBI_ND( 								CODE_UBI_C32_UBR_NAC_BIS( PALP32, DSTP32, BLND ), 1, 4 )
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C32_NBR_NAC_BIS( PALP32, DSTP32 ), 1, 4 )
							else				DRAWBASICPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C32_NBR_NAC_BIS( PALP32, DSTP32 ), 1, 4 )
						}else{
							if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( if( *SrcBPT != TransColor )	CODE_NBI_C32_UBR_NAC_BIS( PALP32, DSTP32 ), 1, 4 )
							else				DRAWBASICPOLYGONMEMIMG_NBI_ND( 								CODE_NBI_C32_UBR_NAC_BIS( PALP32, DSTP32 ), 1, 4 )
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
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_UAC_BNO_ACK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_UAC_BNO(     SRCP32, DSTP32, BLND ), 4, 4 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_NAC_BNO_TCK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_NAC_BNO(     SRCP32, DSTP32, BLND ), 4, 4 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BNO_ACK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BNO(     SRCP32, DSTP32, BLND ), 4, 4 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BNO_TCK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BNO(     SRCP32, DSTP32, BLND ), 4, 4 )
							}
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_UAC_BNO_ACK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_UAC_BNO(     SRCP32, DSTP32 ), 4, 4 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_NAC_BNO_TCK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_NAC_BNO(     SRCP32, DSTP32 ), 4, 4 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BNO_ACK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BNO(     SRCP32, DSTP32 ), 4, 4 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BNO_TCK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BNO(     SRCP32, DSTP32 ), 4, 4 )
							}
						}
					}
					break ;

				case DX_BLENDMODE_ALPHA :		// ���u�����h
					if( MemImgManage.BlendParam == 255 ) goto NOMALDRAW_C32_NOPAL_BNO ;
					else
					if( MemImgManage.BlendParam == 0 ) return ;
					
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_UAC_BAL_ACK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_UAC_BAL(     SRCP32, DSTP32, BLND ), 4, 4 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_NAC_BAL_TCK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_NAC_BAL(     SRCP32, DSTP32, BLND ), 4, 4 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BAL_ACK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BAL(     SRCP32, DSTP32, BLND ), 4, 4 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BAL_TCK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BAL(     SRCP32, DSTP32, BLND ), 4, 4 )
							}
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_UAC_BAL_ACK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_UAC_BAL(     SRCP32, DSTP32 ), 4, 4 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_NAC_BAL_TCK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_NAC_BAL(     SRCP32, DSTP32 ), 4, 4 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BAL_ACK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BAL(     SRCP32, DSTP32 ), 4, 4 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BAL_TCK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BAL(     SRCP32, DSTP32 ), 4, 4 )
							}
						}
					}
					break ;

				case DX_BLENDMODE_ADD :		// ���Z�u�����h
					if( MemImgManage.BlendParam == 0 ) return ;
					
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_UAC_BAD_ACK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_UAC_BAD(     SRCP32, DSTP32, BLND ), 4, 4 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_NAC_BAD_TCK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_NAC_BAD(     SRCP32, DSTP32, BLND ), 4, 4 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BAD_ACK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BAD(     SRCP32, DSTP32, BLND ), 4, 4 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BAD_TCK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BAD(     SRCP32, DSTP32, BLND ), 4, 4 )
							}
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_UAC_BAD_ACK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_UAC_BAD(     SRCP32, DSTP32 ), 4, 4 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_NAC_BAD_TCK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_NAC_BAD(     SRCP32, DSTP32 ), 4, 4 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BAD_ACK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BAD(     SRCP32, DSTP32 ), 4, 4 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BAD_TCK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BAD(     SRCP32, DSTP32 ), 4, 4 )
							}
						}
					}
					break ;

				case DX_BLENDMODE_SUB :		// ���Z�u�����h
					if( MemImgManage.BlendParam == 0 ) return ;
					
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_UAC_BSB_ACK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_UAC_BSB(     SRCP32, DSTP32, BLND ), 4, 4 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_NAC_BSB_TCK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_NAC_BSB(     SRCP32, DSTP32, BLND ), 4, 4 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BSB_ACK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BSB(     SRCP32, DSTP32, BLND ), 4, 4 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BSB_TCK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BSB(     SRCP32, DSTP32, BLND ), 4, 4 )
							}
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_UAC_BSB_ACK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_UAC_BSB(     SRCP32, DSTP32 ), 4, 4 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_NAC_BSB_TCK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_NAC_BSB(     SRCP32, DSTP32 ), 4, 4 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BSB_ACK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BSB(     SRCP32, DSTP32 ), 4, 4 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BSB_TCK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BSB(     SRCP32, DSTP32 ), 4, 4 )
							}
						}
					}
					break ;

				case DX_BLENDMODE_MUL :		// ��Z�u�����h
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_UAC_BML_ACK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_UAC_BML(     SRCP32, DSTP32, BLND ), 4, 4 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_NAC_BML_TCK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_NAC_BML(     SRCP32, DSTP32, BLND ), 4, 4 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BML_ACK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BML(     SRCP32, DSTP32, BLND ), 4, 4 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BML_TCK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BML(     SRCP32, DSTP32, BLND ), 4, 4 )
							}
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_UAC_BML_ACK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_UAC_BML(     SRCP32, DSTP32 ), 4, 4 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_NAC_BML_TCK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_NAC_BML(     SRCP32, DSTP32 ), 4, 4 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BML_ACK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BML(     SRCP32, DSTP32 ), 4, 4 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BML_TCK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BML(     SRCP32, DSTP32 ), 4, 4 )
							}
						}
					}
					break ;

				case DX_BLENDMODE_MULA :	// ��Z+���u�����h
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_UAC_BMA_ACK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_UAC_BMA(     SRCP32, DSTP32, BLND ), 4, 4 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_NAC_BMA_TCK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_NAC_BMA(     SRCP32, DSTP32, BLND ), 4, 4 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BMA_ACK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BMA(     SRCP32, DSTP32, BLND ), 4, 4 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BMA_TCK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BMA(     SRCP32, DSTP32, BLND ), 4, 4 )
							}
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_UAC_BMA_ACK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_UAC_BMA(     SRCP32, DSTP32 ), 4, 4 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_NAC_BMA_TCK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_NAC_BMA(     SRCP32, DSTP32 ), 4, 4 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BMA_ACK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BMA(     SRCP32, DSTP32 ), 4, 4 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BMA_TCK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BMA(     SRCP32, DSTP32 ), 4, 4 )
							}
						}
					}
					break ;

				case DX_BLENDMODE_INVSRC :		// �`�挳�̐F�𔽓]�{���u�����h
					if( BlendImg != NULL ){
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_UAC_BIS_ACK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_UAC_BIS(     SRCP32, DSTP32, BLND ), 4, 4 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_NAC_BIS_TCK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_NBR_NAC_BIS(     SRCP32, DSTP32, BLND ), 4, 4 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BIS_ACK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_UAC_BIS(     SRCP32, DSTP32, BLND ), 4, 4 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BIS_TCK( SRCP32, DSTP32, BLND ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_UBI_ND( CODE_UBI_C32_UBR_NAC_BIS(     SRCP32, DSTP32, BLND ), 4, 4 )
							}
						}
					}else{
						if( ( MemImgManage.bDrawBright & 0xffffff ) == 0xffffff ){
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_UAC_BIS_ACK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_UAC_BIS(     SRCP32, DSTP32 ), 4, 4 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_NAC_BIS_TCK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_NBR_NAC_BIS(     SRCP32, DSTP32 ), 4, 4 )
							}
						}else{
							if( SrcImg->Base->UseAlpha == 1 ){
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BIS_ACK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_UAC_BIS(     SRCP32, DSTP32 ), 4, 4 )
							}else{
								if( TransFlag )		DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BIS_TCK( SRCP32, DSTP32 ), 4, 4 )
								else				DRAWBASICPOLYGONMEMIMG_NBI_ND( CODE_NBI_C32_UBR_NAC_BIS(     SRCP32, DSTP32 ), 4, 4 )
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
	// �e�N�X�`���������ꍇ
	{
		// �`�揀��
		if( DestImg->Base->ColorType == 0 )
		{
			ColorD = RUP16( RTABLE( RBOT16( Color ) ) ) |
				     GUP16( GTABLE( GBOT16( Color ) ) ) |
				   	 BUP16( BTABLE( BBOT16( Color ) ) ) ;
		}
		else
		{
			ColorD = Color ;
			ColorB[0] = (BYTE)BTABLE( ColorB[0] ) ;
			ColorB[1] = (BYTE)GTABLE( ColorB[1] ) ;
			ColorB[2] = (BYTE)RTABLE( ColorB[2] ) ;
		}
		SrcBPT = ColorB ;

		// �J���[���[�h�ɂ���ď����𕪊�
		switch( DestImg->Base->ColorType )
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

			case DX_BLENDMODE_MULA :	// ��Z+���u�����h
				DRAWBASICPOLYGONMEMIMG_NTEX_NBI_ND( CODE_NBI_C16_NBR_NAC_BMA( SRC16, DST16, DSTP16 ), 2 )
				break ;

			case DX_BLENDMODE_INVSRC :		// �`�挳�̐F�𔽓]�{���u�����h
				DRAWBASICPOLYGONMEMIMG_NTEX_NBI_ND( CODE_NBI_C16_NBR_NAC_BIS( SRC16, DST16, DSTP16 ), 2 )
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

			case DX_BLENDMODE_MULA :	// ��Z+���u�����h
				DRAWBASICPOLYGONMEMIMG_NTEX_NBI_ND( CODE_NBI_C32_NBR_NAC_BMA( SRCP32, DSTP32 ), 4 )
				break ;

			case DX_BLENDMODE_INVSRC :		// �`�挳�̐F�𔽓]�{���u�����h
				DRAWBASICPOLYGONMEMIMG_NTEX_NBI_ND( CODE_NBI_C32_NBR_NAC_BIS( SRCP32, DSTP32 ), 4 )
				break ;
			}
			break ;
		}
	}

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


// �C���[�W��ό`�`�悷��
extern void DrawTransformMemImg( MEMIMG *DestImg, const MEMIMG *SrcImg, const POINT *pos4, int TransFlag, const MEMIMG *BlendImg )
{
	// �`��͈͂𓾂�
	{
		RECT DrawRect ;
		POINT pos[4] ;
		int i, MaxY, MinY, MaxX, MinX ;

		memcpy( pos, pos4, sizeof( pos ) ) ;

		// ��ԏ�ɗ��Ă�����W�ɂ���ĕ`��N�_���W��������
		{
			MaxY = 0 ; MinY = 0 ;
			MaxX = 0 ; MinX = 0 ; 
			for( i = 1 ; i < 4 ; i ++ )
			{
				if( pos[MaxY].y < pos[i].y ) MaxY = i ;
				if( pos[MinY].y > pos[i].y ) MinY = i ;

				if( pos[MaxX].x < pos[i].x ) MaxX = i ;
				if( pos[MinX].x > pos[i].x ) MinX = i ;
			}

			DrawRect.left   = pos[MinX].x - 2 ; 
			DrawRect.top    = pos[MinY].y - 2 ; 

			DrawRect.right  = pos[MaxX].x + 4;
			DrawRect.bottom = pos[MaxY].y + 4;
		}

		RectClipping_Inline( &DrawRect , &MemImgManage.DrawArea ) ;
		if( DrawRect.left == DrawRect.right || DrawRect.top == DrawRect.bottom ) return ;
	}

	// �`�悷��
	{
		DX_POINTDATA p[4] ;

		p[0].x = pos4[0].x ;
		p[0].y = pos4[0].y ;
		p[0].u = 0 ;
		p[0].v = 0 ;

		p[1].x = pos4[1].x ;
		p[1].y = pos4[1].y ;
		p[1].u = ( int )SrcImg->Width ;
		p[1].v = 0 ;

		p[2].x = pos4[2].x ;
		p[2].y = pos4[2].y ;
		p[2].u = 0 ;
		p[2].v = ( int )SrcImg->Height ;

		p[3].x = pos4[3].x ;
		p[3].y = pos4[3].y ;
		p[3].u = ( int )SrcImg->Width ;
		p[3].v = ( int )SrcImg->Height ;

		DrawBasicPolygonMemImg( DestImg, SrcImg, &p[0], TransFlag, BlendImg, 0 ) ;
		DrawBasicPolygonMemImg( DestImg, SrcImg, &p[1], TransFlag, BlendImg, 0 ) ;
	}

	// �I��
	return ;
}

#endif

#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE
