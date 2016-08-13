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
#include "DxMemory.h"

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// �}�N����`------------------------------------------------------------------

// �f�[�^�^��`----------------------------------------------------------------

// �y�C���g�p�f�[�^
struct PAINTDATA
{
	short					x, y ;								// �v�f�_
} ;

// �f�[�^�錾------------------------------------------------------------------

// �֐��v���g�^�C�v�錾--------------------------------------------------------

// �v���O����------------------------------------------------------------------

#ifdef DX_NON_2DDRAW

extern void DrawFillBoxMemImg( MEMIMG *DestImg, const RECT *FillRect, unsigned int Color )
{
	return ;
}

extern void DrawLineMemImg( MEMIMG *DestImg, int x1, int y1, int x2, int y2, unsigned int Color )
{
	return ;
}

extern void DrawLineBoxMemImg( MEMIMG *DestImg, int x1, int y1, int x2, int y2, unsigned int Color )
{
	return ;
}

extern void DrawPixelSetMemImg( MEMIMG *DestImg, const POINTDATA *PointData, int Num )
{
	return ;
}

extern void DrawLineSetMemImg( MEMIMG *DestImg, const LINEDATA *LineData, int Num )
{
	return ;
}

extern void DrawPixelMemImg( MEMIMG *DestImg, int x, int y, unsigned int Color )
{
	return ;
}

extern void PaintMemImg( MEMIMG *DestImg, int x, int y, unsigned int FillColor, ULONGLONG BoundaryColor )
{
	return ;
}

extern void DrawCircleMemImg( MEMIMG *DestImg, int x, int y, int r, unsigned int Color, int FillFlag )
{
	return ;
}

extern void DrawOvalMemImg( MEMIMG *DestImg, int x, int y, int rx, int ry, unsigned int Color, int FillFlag )
{
	return ;
}

#else

#define PAINTMEMIMG_ND( TYPE, ADDNUM )	\
/* �`�惋�[�v�Ɋ|���� */\
while( PWriteIndex != PReadIndex )\
{\
	/* �X�^�b�N�ɋl�܂ꂽ�f�[�^��ǂݏo�� */\
	x2 = PData[PReadIndex].x ;\
	y2 = PData[PReadIndex].y ;\
	PReadIndex ++ ;\
	if( PReadIndex == PAINTBUFFERSIZE ) PReadIndex = 0 ;\
\
	/* �T�[�`�A�h���X���Z�o */\
	SrcBPT  = SrcBP  + ( x2 * (ADDNUM) ) + y2 * SrcPitch  ;\
	DestBPT = DestBP + ( x2 * (ADDNUM) ) + y2 * DestPitch ;\
\
	/* �������ɃT�[�`����I����Ă����玟�Ɉڂ� */\
	if( *((TYPE *)SrcBPT) == (DWORD)BoundaryColorD ) continue ;\
\
	/* ���[�܂ňړ� */\
	while( *((TYPE *)SrcBPT) != (DWORD)BoundaryColorD && x2 > MemImgManage.DrawArea.left )\
	{\
		x2 -- ;\
		SrcBPT  -= (ADDNUM) ;\
		DestBPT -= (ADDNUM) ;\
	}\
	if( *((TYPE *)SrcBPT) == (DWORD)BoundaryColorD )\
	{\
		x2 ++ ;\
		SrcBPT  += (ADDNUM) ;\
		DestBPT += (ADDNUM) ;\
	}\
\
	/* �t���O������ */\
	UpSarchFlag = DownSarchFlag = FALSE ;\
\
	/* �E�Ɍ������đ����J�n */\
	while( MemImgManage.DrawArea.right > x2 && *((TYPE *)SrcBPT) != (DWORD)BoundaryColorD )\
	{\
		/* ���݂̍��W��h�� */\
		*((TYPE *)SrcBPT)  = (TYPE)BoundaryColorD ;\
		*((TYPE *)DestBPT) = (TYPE)FillColor ;\
\
		/* �㒲�׃t���O���|��Ă��āA�X�ɏ�ɋ��E�F���Ȃ��ꍇ�́A��̃s�N�Z���� */\
		/* �V���ȃT�[�`�|�C���g�Ƃ��ăX�^�b�N�ɐς� */\
		if( y2 - 1 >= MemImgManage.DrawArea.top )\
		{\
			if( *((TYPE *)( SrcBPT - SrcPitch )) != (DWORD)BoundaryColorD )\
			{\
				if( UpSarchFlag == FALSE )\
				{\
					PData[PWriteIndex].x = ( short )x2 ;\
					PData[PWriteIndex].y = ( short )( y2 - 1 ) ;\
					UpSarchFlag = TRUE ;\
\
					PWriteIndex ++ ;\
					if( PWriteIndex == PAINTBUFFERSIZE ) PWriteIndex = 0 ;\
				}\
			}\
			else\
			{\
				UpSarchFlag = FALSE ;\
			}\
		}\
\
		/* �����׃t���O���|��Ă��āA�X�ɉ��ɋ��E�F���Ȃ��ꍇ�́A���̃s�N�Z���� */\
		/* �V���ȃT�[�`�|�C���g�Ƃ��ăX�^�b�N�ɐς� */\
		if( y2 + 1 < MemImgManage.DrawArea.bottom )\
		{\
			if( *((TYPE *)( SrcBPT + SrcPitch )) != (DWORD)BoundaryColorD )\
			{\
				if( DownSarchFlag == FALSE )\
				{\
					PData[PWriteIndex].x = ( short )x2 ;\
					PData[PWriteIndex].y = ( short )( y2 + 1 ) ;\
					DownSarchFlag = TRUE ;\
\
					PWriteIndex ++ ;\
					if( PWriteIndex == PAINTBUFFERSIZE ) PWriteIndex = 0 ;\
				}\
			}\
			else\
			{\
				DownSarchFlag = FALSE ;\
			}\
		}\
\
		/* �A�h���X�C���N�������g */\
		SrcBPT  += (ADDNUM) ;\
		DestBPT += (ADDNUM) ;\
		x2 ++ ;\
	}\
}


#define PAINTMEMIMG2_ND( TYPE, ADDNUM )	\
/* �w��_�̐F���擾���� */\
BoundaryColorD = (DWORD)*((TYPE *)(DestBP  + ( x * (ADDNUM) ) + y * DestPitch))  ;\
if( FillColor != BoundaryColorD )\
{\
	/* �`�惋�[�v�Ɋ|���� */\
	while( PWriteIndex != PReadIndex )\
	{\
		/* �X�^�b�N�ɋl�܂ꂽ�f�[�^��ǂݏo�� */\
		x2 = PData[PReadIndex].x ;\
		y2 = PData[PReadIndex].y ;\
		PReadIndex ++ ;\
		if( PReadIndex == PAINTBUFFERSIZE ) PReadIndex = 0 ;\
	\
		/* �T�[�`�A�h���X���Z�o */\
		DestBPT = DestBP + ( x2 * (ADDNUM) ) + y2 * DestPitch ;\
	\
		/* �������ɃT�[�`����I����Ă����玟�Ɉڂ� */\
		if( *((TYPE *)DestBPT) != (DWORD)BoundaryColorD ) continue ;\
	\
		/* ���[�܂ňړ� */\
		while( *((TYPE *)DestBPT) == (DWORD)BoundaryColorD && x2 > MemImgManage.DrawArea.left )\
		{\
			x2 -- ;\
			DestBPT -= (ADDNUM) ;\
		}\
		if( *((TYPE *)DestBPT) != (DWORD)BoundaryColorD )\
		{\
			x2 ++ ;\
			DestBPT += (ADDNUM) ;\
		}\
	\
		/* �t���O������ */\
		UpSarchFlag = DownSarchFlag = FALSE ;\
	\
		/* �E�Ɍ������đ����J�n */\
		while( MemImgManage.DrawArea.right > x2 && *((TYPE *)DestBPT) == (DWORD)BoundaryColorD )\
		{\
			/* ���݂̍��W��h�� */\
			*((TYPE *)DestBPT) = (TYPE)FillColor ;\
	\
			/* �㒲�׃t���O���|��Ă��āA�X�ɏ�ɋ��E�F���Ȃ��ꍇ�́A��̃s�N�Z���� */\
			/* �V���ȃT�[�`�|�C���g�Ƃ��ăX�^�b�N�ɐς� */\
			if( y2 - 1 >= MemImgManage.DrawArea.top )\
			{\
				if( *((TYPE *)( DestBPT - DestPitch )) == (DWORD)BoundaryColorD )\
				{\
					if( UpSarchFlag == FALSE )\
					{\
						PData[PWriteIndex].x = ( short )x2 ;\
						PData[PWriteIndex].y = ( short )( y2 - 1 ) ;\
						UpSarchFlag = TRUE ;\
	\
						PWriteIndex ++ ;\
						if( PWriteIndex == PAINTBUFFERSIZE ) PWriteIndex = 0 ;\
					}\
				}\
				else\
				{\
					UpSarchFlag = FALSE ;\
				}\
			}\
	\
			/* �����׃t���O���|��Ă��āA�X�ɉ��ɋ��E�F���Ȃ��ꍇ�́A���̃s�N�Z���� */\
			/* �V���ȃT�[�`�|�C���g�Ƃ��ăX�^�b�N�ɐς� */\
			if( y2 + 1 < MemImgManage.DrawArea.bottom )\
			{\
				if( *((TYPE *)( DestBPT + DestPitch )) == (DWORD)BoundaryColorD )\
				{\
					if( DownSarchFlag == FALSE )\
					{\
						PData[PWriteIndex].x = ( short )x2 ;\
						PData[PWriteIndex].y = ( short )( y2 + 1 ) ;\
						DownSarchFlag = TRUE ;\
	\
						PWriteIndex ++ ;\
						if( PWriteIndex == PAINTBUFFERSIZE ) PWriteIndex = 0 ;\
					}\
				}\
				else\
				{\
					DownSarchFlag = FALSE ;\
				}\
			}\
	\
			/* �A�h���X�C���N�������g */\
			DestBPT += (ADDNUM) ;\
			x2 ++ ;\
		}\
	}\
}

// �w��_���狫�E�F������Ƃ���܂œh��Ԃ�
extern void PaintMemImg( MEMIMG *DestImg, int x, int y, unsigned int FillColor, ULONGLONG BoundaryColor )
{
	int x2, y2 ;
	int DestPitch, SrcPitch ;
	PAINTDATA *PData ;
	int PWriteIndex, PReadIndex ;
	int UpSarchFlag, DownSarchFlag ;
	MEMIMG WorkImg ;
	unsigned int BoundaryColorD = ( DWORD )BoundaryColor ;

	union
	{
		BYTE *SrcBPT ;
		WORD *SrcWPT ;
		DWORD *SrcDPT ;
	} ;
	union
	{
		BYTE *DestBPT ;
		WORD *DestWPT ;
		DWORD *DestDPT ;
	} ;
	union
	{
		BYTE *SrcBP ;
		WORD *SrcWP ;
		DWORD *SrcDP ;
	} ;
	union
	{
		BYTE *DestBP ;
		WORD *DestWP ;
		DWORD *DestDP ;
	} ;
	
	// �`��悪���`�����l���t���p���b�g�t���������ꍇ�̓G���[
	if( DestImg->Base->UsePalette == 1 || DestImg->Base->UseAlpha == 1 ) return ;

	// �o�b�t�@�̊m��
	PData = ( PAINTDATA * )DXALLOC( sizeof( PAINTDATA ) * PAINTBUFFERSIZE ) ;
	if( PData == NULL ) return;

	// �C���f�b�N�X�J�E���^������
	PWriteIndex = PReadIndex = 0 ;

	// �ŏ��̑����f�[�^������
	PData[0].x = ( short )x ;
	PData[0].y = ( short )y ;
	PWriteIndex ++ ;

	// �`��
	if( BoundaryColor == -1 )
	{
		// �P���C���̃s�b�`����ۑ�
		DestPitch = ( int )DestImg->Base->Pitch ;

		// �`���ƕ`�挳�̊�_�f�[�^�A�h���X���擾
		DestBP = DestImg->UseImage ;

		switch( DestImg->Base->ColorType )
		{
		case 0 :	// 16bit
			PAINTMEMIMG2_ND( WORD, 2 ) ;
			break ;
			
		case 1 :	// 32bit
			PAINTMEMIMG2_ND( DWORD, 4 ) ;
			break ;
		}
	}
	else
	{
		// ��Ɨp�C���[�W���쐬����
		_MEMSET( &WorkImg, 0, sizeof( WorkImg ) ) ;
		if( InitializeMemImg( &WorkImg, ( int )DestImg->Width, ( int )DestImg->Height, -1, 0, DestImg->Base->ColorType, FALSE, FALSE ) == -1 )
			return ;

		// �P���C���̃s�b�`����ۑ�
		SrcPitch  = ( int )WorkImg.Base->Pitch ;
		DestPitch = ( int )DestImg->Base->Pitch ;

		// �`���ƕ`�挳�̊�_�f�[�^�A�h���X���擾
		SrcBP  = WorkImg.UseImage ;
		DestBP = DestImg->UseImage ;

		// ��Ɨp�C���[�W�̈�ɕ`���̓��e���R�s�[����
		BltMemImg( &WorkImg, DestImg, NULL, 0, 0 ) ;

		switch( DestImg->Base->ColorType )
		{
		case 0 :	// 16bit
			PAINTMEMIMG_ND( WORD, 2 ) ;
			break ;
			
		case 1 :	// 32bit
			PAINTMEMIMG_ND( DWORD, 4 ) ;
			break ;
		}

		// ��Ɨp�C���[�W���폜
		TerminateMemImg( &WorkImg ) ;
	}

	// �������̉��
	DXFREE( PData ) ;

	// �I��
	return ;
}

// �C���[�W�ɓ_��`�悷��
#define DST16	*DestWP
#define SRC16	Color
#define DSTP16	DestWP

#define SRCP32	(BYTE *)&Color
#define DSTP32	DestBP
extern void DrawPixelMemImg( MEMIMG *DestImg, int x, int y, unsigned int Color )
{
	DWORD *RateTable1 ;
	int *RateTable2 ;
	DWORD Src1, Data, DataD ;
	union
	{
		BYTE SrcB[4] ;
		DWORD SrcD ;
	} ;
	union
	{
		BYTE  *DestBP ;
		WORD  *DestWP ;
		DWORD *DestDP ;
	} ;
	
	// �`��悪���`�����l���t���p���b�g�t���������ꍇ�̓G���[
	if( DestImg->Base->UsePalette == 1 || DestImg->Base->UseAlpha == 1 ) return ;

	// �͂ݏo������
	if( (DWORD)( ( x - MemImgManage.DrawArea.left ) | ( MemImgManage.DrawArea.right  - x ) | 
		         ( y - MemImgManage.DrawArea.top  ) | ( MemImgManage.DrawArea.bottom - y ) ) & 0x80000000 ) return ;
	
	// �`���A�h���X���Z�b�g
	DestBP = DestImg->UseImage + x * DestImg->Base->ColorDataP->PixelByte + y * DestImg->Base->Pitch ;
	
	// �`��̏���
	RateTable1 = MemImgManage.RateTable[MemImgManage.BlendParam] ;
	RateTable2 = &MemImgManage.RateTable2[MemImgManage.BlendParam][256] ;

	// �_��`�悷��
	switch( DestImg->Base->ColorType )
	{
	case 0 :	// 16bit ���[�h
		// �`��P�x�𔽉f
		if( ( MemImgManage.bDrawBright & 0xffffff ) != 0xffffff )
		{
			Color = RUP16( ( RBOT16( Color ) * MemImgManage.DrawBright.Red   ) >> 8 ) |
					GUP16( ( GBOT16( Color ) * MemImgManage.DrawBright.Green ) >> 8 ) |
					BUP16( ( BBOT16( Color ) * MemImgManage.DrawBright.Blue  ) >> 8 ) ;
		}
		
		// �u�����h���[�h�ɂ���ď����𕪊�
		switch( MemImgManage.BlendMode )
		{
		case DX_BLENDMODE_NOBLEND :		// �u�����h����
NOTEX_NOMALDRAW_C16_NOPAL_BNO:
			CODE_NBI_C16_NBR_NAC_BNO( SRC16, DST16, DSTP16 )
			break ;

		case DX_BLENDMODE_ALPHA :		// ���u�����h
			if( MemImgManage.BlendParam == 255 ) goto NOTEX_NOMALDRAW_C16_NOPAL_BNO ;
			else
			if( MemImgManage.BlendParam == 0 ) return ;
			
			CODE_NBI_C16_NBR_NAC_BAL( SRC16, DST16, DSTP16 )
			break ;

		case DX_BLENDMODE_ADD :		// ���Z�u�����h
			if( MemImgManage.BlendParam == 0 ) return ;
			
			CODE_NBI_C16_NBR_NAC_BAD( SRC16, DST16, DSTP16 )
			break ;

		case DX_BLENDMODE_SUB :		// ���Z�u�����h
			if( MemImgManage.BlendParam == 0 ) return ;
			
			CODE_NBI_C16_NBR_NAC_BSB( SRC16, DST16, DSTP16 )
			break ;

		case DX_BLENDMODE_MUL :		// ��Z�u�����h
			CODE_NBI_C16_NBR_NAC_BML( SRC16, DST16, DSTP16 )
			break ;

		case DX_BLENDMODE_MULA :	// ��Z+���u�����h
			CODE_NBI_C16_NBR_NAC_BMA( SRC16, DST16, DSTP16 )
			break ;

		case DX_BLENDMODE_INVSRC :	// �`�挳�̐F�𔽓]�{���u�����h
			CODE_NBI_C16_NBR_NAC_BIS( SRC16, DST16, DSTP16 )
			break ;
		}
		break ;
		
	case 1 :	// 32bit ���[�h
		// �`��P�x�𔽉f
		if( ( MemImgManage.bDrawBright & 0xffffff ) != 0xffffff )
		{
			((BYTE *)&Color)[2] = ( BYTE )( ( ((BYTE *)&Color)[2] * MemImgManage.DrawBright.Red   ) >> 8 ) ;
			((BYTE *)&Color)[1] = ( BYTE )( ( ((BYTE *)&Color)[1] * MemImgManage.DrawBright.Green ) >> 8 ) ;
			((BYTE *)&Color)[0] = ( BYTE )( ( ((BYTE *)&Color)[0] * MemImgManage.DrawBright.Blue  ) >> 8 ) ;
		}
		
		// �u�����h���[�h�ɂ���ď����𕪊�
		switch( MemImgManage.BlendMode )
		{
		case DX_BLENDMODE_NOBLEND :		// �u�����h����
NOTEX_NOMALDRAW_C32_NOPAL_BNO:
			CODE_NBI_C32_NBR_NAC_BNO( SRCP32, DSTP32 )
			break ;

		case DX_BLENDMODE_ALPHA :		// ���u�����h
			if( MemImgManage.BlendParam == 255 ) goto NOTEX_NOMALDRAW_C32_NOPAL_BNO ;
			else
			if( MemImgManage.BlendParam == 0 ) return ;
			
			CODE_NBI_C32_NBR_NAC_BAL( SRCP32, DSTP32 )
			break ;

		case DX_BLENDMODE_ADD :		// ���Z�u�����h
			if( MemImgManage.BlendParam == 0 ) return ;
			
			CODE_NBI_C32_NBR_NAC_BAD( SRCP32, DSTP32 )
			break ;

		case DX_BLENDMODE_SUB :		// ���Z�u�����h
			if( MemImgManage.BlendParam == 0 ) return ;
			
			CODE_NBI_C32_NBR_NAC_BSB( SRCP32, DSTP32 )
			break ;

		case DX_BLENDMODE_MUL :		// ��Z�u�����h
			CODE_NBI_C32_NBR_NAC_BML( SRCP32, DSTP32 )
			break ;

		case DX_BLENDMODE_MULA :	// ��Z+���u�����h
			CODE_NBI_C32_NBR_NAC_BMA( SRCP32, DSTP32 )
			break ;

		case DX_BLENDMODE_INVSRC :	// �`�挳�̐F�𔽓]�{���u�����h
			CODE_NBI_C32_NBR_NAC_BIS( SRCP32, DSTP32 )
			break ;
		}
		break ;
	}

	// �I��
	return ;
}
#undef DST16
#undef SRC16
#undef DSTP16

#undef SRCP32
#undef DSTP32



// �C���[�W�ɒ��g�̂���{�b�N�X��`�悷��
extern void DrawFillBoxMemImg( MEMIMG *DestImg, int x1, int y1, int x2, int y2, unsigned int Color )
{
	RECT rect ;

	rect.left   = x1 ;
	rect.top    = y1 ;
	rect.right  = x2 ;
	rect.bottom = y2 ;
	DrawFillBoxMemImg( DestImg, &rect, Color ) ;
}


#define DRAWFILLBOXMEMIMG_ND( DRAW, DESTADDNUM )	\
do{\
	wtemp = Width ;\
	do{\
		DRAW\
		DestBP += (DESTADDNUM) ;\
	}while( -- wtemp ) ;\
	DestBP += DestAddPitch ;\
}while( -- Height ) ;


// �C���[�W�ɒ��g�̂���{�b�N�X��`�悷��
#define DST16	*DestWP
#define SRC16	Color
#define DSTP16	DestWP

#define SRCP32	(BYTE *)&Color
#define DSTP32	DestBP
extern void DrawFillBoxMemImg( MEMIMG *DestImg, const RECT *FillRect, unsigned int Color )
{
	RECT DrawRect, TempRect ;
	DWORD Width, Height ;
	DWORD *RateTable1 ;
	int *RateTable2 ;
	DWORD Src1, Data, DataD ;
	DWORD_PTR DestPP ;
	DWORD DestAddPitch, wtemp ;
	union
	{
		BYTE SrcB[4] ;
		DWORD SrcD ;
	} ;
	union
	{
		BYTE  *DestBP ;
		WORD  *DestWP ;
		DWORD *DestDP ;
	} ;

	// �`��悪���`�����l���t���p���b�g�t���������ꍇ�̓G���[
	if( DestImg->Base->UsePalette == 1 || DestImg->Base->UseAlpha == 1 ) return ;

	// ��`�� NULL ��������S��
	if( FillRect == NULL )
	{
		FillRect = &TempRect ;
		TempRect.left   = 0 ;
		TempRect.top    = 0 ;
		TempRect.right  = ( LONG )DestImg->Width ;
		TempRect.bottom = ( LONG )DestImg->Height ;
		DrawRect = TempRect ;
	}
	else
	{
		// �`���`�̃Z�b�g
		if( FillRect->left > FillRect->right )
		{
			DrawRect.right = FillRect->left ;
			DrawRect.left  = FillRect->right ;
		}
		else
		{
			DrawRect.left  = FillRect->left ;
			DrawRect.right = FillRect->right ;
		}

		if( FillRect->top > FillRect->bottom )
		{
			DrawRect.bottom = FillRect->top ;
			DrawRect.top    = FillRect->bottom ;
		}
		else
		{
			DrawRect.top    = FillRect->top ;
			DrawRect.bottom = FillRect->bottom ;
		}
	}

	// �N���b�s���O
	RectClipping_Inline( &DrawRect , &MemImgManage.DrawArea ) ;
	Width  = ( DWORD )( DrawRect.right  - DrawRect.left ) ;
	Height = ( DWORD )( DrawRect.bottom - DrawRect.top  ) ;
	if( ( Width == 0 ) || ( Height == 0 ) ) return ;

	// �]����A�h���X�̃Z�b�g
	DestBP       = DestImg->UseImage + DrawRect.left * DestImg->Base->ColorDataP->PixelByte + DrawRect.top * DestImg->Base->Pitch ;
	DestAddPitch = DestImg->Base->Pitch - Width * DestImg->Base->ColorDataP->PixelByte ;
	DestPP		 = (DWORD_PTR)DestBP ;

	// �`��̏���
	RateTable1 = MemImgManage.RateTable[MemImgManage.BlendParam] ;
	RateTable2 = &MemImgManage.RateTable2[MemImgManage.BlendParam][256] ;

	// �J���[���[�h�ɂ���ď����𕪊�
	switch( DestImg->Base->ColorType )
	{
	case 0 :	// 16bit ���[�h
		// �`��P�x�𔽉f
		if( ( MemImgManage.bDrawBright & 0xffffff ) != 0xffffff )
		{
			Color = RUP16( ( RBOT16( Color ) * MemImgManage.DrawBright.Red   ) >> 8 ) |
					GUP16( ( GBOT16( Color ) * MemImgManage.DrawBright.Green ) >> 8 ) |
					BUP16( ( BBOT16( Color ) * MemImgManage.DrawBright.Blue  ) >> 8 ) ;
		}
		
		// �u�����h���[�h�ɂ���ď����𕪊�
		switch( MemImgManage.BlendMode )
		{
		case DX_BLENDMODE_NOBLEND :		// �u�����h����
NOTEX_NOMALDRAW_C16_NOPAL_BNO:
#ifdef DX_NON_INLINE_ASM
			DRAWFILLBOXMEMIMG_ND( CODE_NBI_C16_NBR_NAC_BNO( SRC16, DST16, DSTP16 ), 2 )
#else
			_asm
			{
				PUSHF
				CLD
				MOV EDI, DestPP
				MOV EAX, Color
				MOV EDX, EAX
				SHL EDX, 16
				OR EAX, EDX
				MOV EDX, Width
				MOV EBX, Height
			LABEL_NBI_C16_NBR_NAC_BNO_1:
				MOV ECX, EDX
				SHR ECX, 1
				JZ LABEL_NBI_C16_NBR_NAC_BNO_2
				REP STOSD
			LABEL_NBI_C16_NBR_NAC_BNO_2:
				MOV ECX, EDX
				AND ECX, 1
				JZ LABEL_NBI_C16_NBR_NAC_BNO_3
				MOV [EDI], AX
				ADD EDI, 2
			LABEL_NBI_C16_NBR_NAC_BNO_3:
				ADD EDI, DestAddPitch
				DEC EBX
				JNZ LABEL_NBI_C16_NBR_NAC_BNO_1
				POPF
			}
#endif
			break ;

		case DX_BLENDMODE_ALPHA :		// ���u�����h
			if( MemImgManage.BlendParam == 255 ) goto NOTEX_NOMALDRAW_C16_NOPAL_BNO ;
			else
			if( MemImgManage.BlendParam == 0 ) return ;
			
			DRAWFILLBOXMEMIMG_ND( CODE_NBI_C16_NBR_NAC_BAL( SRC16, DST16, DSTP16 ), 2 )
			break ;

		case DX_BLENDMODE_ADD :		// ���Z�u�����h
			if( MemImgManage.BlendParam == 0 ) return ;
			
			DRAWFILLBOXMEMIMG_ND( CODE_NBI_C16_NBR_NAC_BAD( SRC16, DST16, DSTP16 ), 2 )
			break ;

		case DX_BLENDMODE_SUB :		// ���Z�u�����h
			if( MemImgManage.BlendParam == 0 ) return ;
			
			DRAWFILLBOXMEMIMG_ND( CODE_NBI_C16_NBR_NAC_BSB( SRC16, DST16, DSTP16 ), 2 )
			break ;

		case DX_BLENDMODE_MUL :		// ��Z�u�����h
			DRAWFILLBOXMEMIMG_ND( CODE_NBI_C16_NBR_NAC_BML( SRC16, DST16, DSTP16 ), 2 )
			break ;

		case DX_BLENDMODE_MULA :	// ��Z+���u�����h
			DRAWFILLBOXMEMIMG_ND( CODE_NBI_C16_NBR_NAC_BMA( SRC16, DST16, DSTP16 ), 2 )
			break ;

		case DX_BLENDMODE_INVSRC :	// �`�挳�̐F�𔽓]�{���u�����h
			DRAWFILLBOXMEMIMG_ND( CODE_NBI_C16_NBR_NAC_BIS( SRC16, DST16, DSTP16 ), 2 )
			break ;
		}
		break ;
		
	case 1 :	// 32bit ���[�h
		if( ( MemImgManage.bDrawBright & 0xffffff ) != 0xffffff )
		{
			((BYTE *)&Color)[2] = ( BYTE )( ( ((BYTE *)&Color)[2] * MemImgManage.DrawBright.Red   ) >> 8 ) ;
			((BYTE *)&Color)[1] = ( BYTE )( ( ((BYTE *)&Color)[1] * MemImgManage.DrawBright.Green ) >> 8 ) ;
			((BYTE *)&Color)[0] = ( BYTE )( ( ((BYTE *)&Color)[0] * MemImgManage.DrawBright.Blue  ) >> 8 ) ;
		}
		
		// �u�����h���[�h�ɂ���ď����𕪊�
		switch( MemImgManage.BlendMode )
		{
		case DX_BLENDMODE_NOBLEND :		// �u�����h����
NOTEX_NOMALDRAW_C32_NOPAL_BNO:
#ifdef DX_NON_INLINE_ASM
			DRAWFILLBOXMEMIMG_ND( CODE_NBI_C32_NBR_NAC_BNO( SRCP32, DSTP32 ), 4 )
#else
			_asm
			{
				PUSHF
				CLD
				MOV EDI, DestPP
				MOV EAX, Color
				MOV EDX, Width
				MOV EBX, Height
			LABEL_NBI_C32_NBR_NAC_BNO_1:
				MOV ECX, EDX
				REP STOSD
				ADD EDI, DestAddPitch
				DEC EBX
				JNZ LABEL_NBI_C32_NBR_NAC_BNO_1
				POPF
			}
#endif
			break ;

		case DX_BLENDMODE_ALPHA :		// ���u�����h
			if( MemImgManage.BlendParam == 255 ) goto NOTEX_NOMALDRAW_C32_NOPAL_BNO ;
			else
			if( MemImgManage.BlendParam == 0 ) return ;
			
			DRAWFILLBOXMEMIMG_ND( CODE_NBI_C32_NBR_NAC_BAL( SRCP32, DSTP32 ), 4 )
			break ;

		case DX_BLENDMODE_ADD :		// ���Z�u�����h
			if( MemImgManage.BlendParam == 0 ) return ;
			
			DRAWFILLBOXMEMIMG_ND( CODE_NBI_C32_NBR_NAC_BAD( SRCP32, DSTP32 ), 4 )
			break ;

		case DX_BLENDMODE_SUB :		// ���Z�u�����h
			if( MemImgManage.BlendParam == 0 ) return ;
			
			DRAWFILLBOXMEMIMG_ND( CODE_NBI_C32_NBR_NAC_BSB( SRCP32, DSTP32 ), 4 )
			break ;

		case DX_BLENDMODE_MUL :		// ��Z�u�����h
			DRAWFILLBOXMEMIMG_ND( CODE_NBI_C32_NBR_NAC_BML( SRCP32, DSTP32 ), 4 )
			break ;

		case DX_BLENDMODE_MULA :	// ��Z+���u�����h
			DRAWFILLBOXMEMIMG_ND( CODE_NBI_C32_NBR_NAC_BMA( SRCP32, DSTP32 ), 4 )
			break ;

		case DX_BLENDMODE_INVSRC :	// �`�挳�̐F�𔽓]�{���u�����h
			DRAWFILLBOXMEMIMG_ND( CODE_NBI_C32_NBR_NAC_BIS( SRCP32, DSTP32 ), 4 )
			break ;
		}
		break ;
	}

	// �I��
	return ;
}
#undef DST16
#undef SRC16
#undef DSTP16

#undef SRCP32
#undef DSTP32




#define DRAWLINEMEMIMG_ND( DRAW )	\
{\
	if( DrawSizeX >= DrawSizeY )\
	{\
		j = DrawSizeX / 2 ;\
		i = DrawSizeX ;\
		do{\
			DRAW\
			DestBP += AddX ;\
			j      += DrawSizeY ;\
			if( j >= DrawSizeX ){\
				DestBP += AddY ;\
				j      -= DrawSizeX ;\
			}\
		}while( -- i ) ;\
	}else{\
		j = DrawSizeY / 2 ;\
		i = DrawSizeY ;\
		do{\
			DRAW\
			DestBP += AddY ;\
			j      += DrawSizeX ;\
			if( j >= DrawSizeY ){\
				DestBP += AddX ;\
				j      -= DrawSizeY ;\
			}\
		}while( -- i ) ;\
	}\
}

// �C���[�W�ɐ���`�悷��
#define DST16	*DestWP
#define SRC16	Color
#define DSTP16	DestWP

#define SRCP32	(BYTE *)&Color
#define DSTP32	DestBP
extern void DrawLineMemImg( MEMIMG *DestImg, int x1, int y1, int x2, int y2, unsigned int Color )
{
	RECT DrawArea ;
	int i, j ;
	int DrawSizeX , DrawSizeY ;
	int AddX, AddY ; 
	DWORD *RateTable1 ;
	int *RateTable2 ;
	DWORD Src1, Data, DataD ;
	DWORD DestPitch ;
	union
	{
		BYTE SrcB[4] ;
		DWORD SrcD ;
	} ;
	union
	{
		BYTE  *DestBP ;
		WORD  *DestWP ;
		DWORD *DestDP ;
	} ;

	DrawArea = MemImgManage.DrawArea ;

	// �N���b�s���O����
	{
		double d1 , d2 ;
		double b1 , b2 ;

		// �`��G���A���O��Ă����ꍇ�͕`�悵�Ȃ�
		{
			int HiX , LwX , LwY , HiY ;

			if( x2 > x1 ){ HiX = x2 ; LwX = x1 ; }else{ HiX = x1 ; LwX = x2 ; }
			if( y2 > y1 ){ HiY = y2 ; LwY = y1 ; }else{ HiY = y1 ; LwY = y2 ; }

			if( ( ( HiX            - DrawArea.left ) | ( HiY             - DrawArea.top ) |
				  ( DrawArea.right - ( LwX + 1 )   ) | ( DrawArea.bottom - ( LwY + 1 )  ) ) & 0x80000000 ) return ;
//			if( DrawArea.left  >  HiX || DrawArea.top    >  HiY ||
//				DrawArea.right <= LwX || DrawArea.bottom <= LwY ) return ;
		}

		// ���̌X�����擾
		d1 = ( y2 == y1 ) ? 0 : ( x2 - x1 ) / ( double )( y2 - y1 ) ;	// x
		d2 = ( x2 == x1 ) ? 0 : ( y2 - y1 ) / ( double )( x2 - x1 ) ;	// y
		b1 = ( x1 - d1 * y1 ) ;		// x
		b2 = ( y1 - d2 * x1 ) ;		// y

		if( DrawArea.left   >  x1 ){ x1 = DrawArea.left ;       y1 = _DTOL( x1 * d2 + b2 ) ;}
		else
		if( DrawArea.right  <= x1 ){ x1 = DrawArea.right - 1 ;  y1 = _DTOL( x1 * d2 + b2 ) ;}

		if( DrawArea.left   >  x2 ){ x2 = DrawArea.left - 1 ;   y2 = _DTOL( x2 * d2 + b2 ) ;}
		else
		if( DrawArea.right  <= x2 ){ x2 = DrawArea.right  ;     y2 = _DTOL( x2 * d2 + b2 ) ;}
			
		if( DrawArea.top    >  y1 ){ y1 = DrawArea.top ;        x1 = _DTOL( y1 * d1 + b1 ) ;}
		else
		if( DrawArea.bottom <= y1 ){ y1 = DrawArea.bottom - 1 ; x1 = _DTOL( y1 * d1 + b1 ) ;}

		if( DrawArea.top    >  y2 ){ y2 = DrawArea.top - 1 ;    x2 = _DTOL( y2 * d1 + b1 ) ;}
		else
		if( DrawArea.bottom <= y2 ){ y2 = DrawArea.bottom ;     x2 = _DTOL( y2 * d1 + b1 ) ;}

		if( DrawArea.left   >  x1 )	 x1 = DrawArea.left ;
		else
		if( DrawArea.right  <= x1 )	 x1 = DrawArea.right - 1 ; 

		if( DrawArea.left   >  x2 )	 x2 = DrawArea.left - 1 ;
		else
		if( DrawArea.right  <= x2 )	 x2 = DrawArea.right ; 
			
		if( DrawArea.top    >  y1 )	 y1 = DrawArea.top ; 
		else
		if( DrawArea.bottom <= y1 )	 y1 = DrawArea.bottom - 1 ;

		if( DrawArea.top    >  y2 )	 y2 = DrawArea.top  - 1  ;
		else
		if( DrawArea.bottom <= y2 )	 y2 = DrawArea.bottom  ; 
	}

	// �`��T�C�Y���擾
	DrawSizeX = x2 - x1 ;
	DrawSizeY = y2 - y1 ;
	if( DrawSizeX == 0 && DrawSizeY == 0 ) return ; 

	// �i�s�������Z�b�g
	if( DrawSizeX < 0 ){ AddX = -1 ; DrawSizeX = -DrawSizeX ; }else{ AddX = 1 ; }
	if( DrawSizeY < 0 ){ AddY = -1 ; DrawSizeY = -DrawSizeY ; }else{ AddY = 1 ; }
	AddX *= DestImg->Base->ColorDataP->PixelByte ;
	AddY *= DestImg->Base->Pitch ;

	// �]����̃A�h���X�Ȃǂ��Z�b�g
	DestBP    = DestImg->UseImage + x1 * DestImg->Base->ColorDataP->PixelByte + y1 * DestImg->Base->Pitch ;
	DestPitch = DestImg->Base->Pitch ;

	// �`��̏���
	j = 0 ;
	RateTable1 = MemImgManage.RateTable[MemImgManage.BlendParam] ;
	RateTable2 = &MemImgManage.RateTable2[MemImgManage.BlendParam][256] ;

	// �J���[���[�h�ɂ���ď����𕪊�
	switch( DestImg->Base->ColorType )
	{
	case 0 :	// 16bit ���[�h
		// �`��P�x�𔽉f
		if( ( MemImgManage.bDrawBright & 0xffffff ) != 0xffffff )
		{
			Color = RUP16( ( RBOT16( Color ) * MemImgManage.DrawBright.Red   ) >> 8 ) |
					GUP16( ( GBOT16( Color ) * MemImgManage.DrawBright.Green ) >> 8 ) |
					BUP16( ( BBOT16( Color ) * MemImgManage.DrawBright.Blue  ) >> 8 ) ;
		}
		
		// �u�����h���[�h�ɂ���ď����𕪊�
		switch( MemImgManage.BlendMode )
		{
		case DX_BLENDMODE_NOBLEND :		// �u�����h����
NOTEX_NOMALDRAW_C16_NOPAL_BNO:
			DRAWLINEMEMIMG_ND( CODE_NBI_C16_NBR_NAC_BNO( SRC16, DST16, DSTP16 ) )
			break ;

		case DX_BLENDMODE_ALPHA :		// ���u�����h
			if( MemImgManage.BlendParam == 255 ) goto NOTEX_NOMALDRAW_C16_NOPAL_BNO ;
			else
			if( MemImgManage.BlendParam == 0 ) return ;
			
			DRAWLINEMEMIMG_ND( CODE_NBI_C16_NBR_NAC_BAL( SRC16, DST16, DSTP16 ) )
			break ;

		case DX_BLENDMODE_ADD :		// ���Z�u�����h
			if( MemImgManage.BlendParam == 0 ) return ;
			
			DRAWLINEMEMIMG_ND( CODE_NBI_C16_NBR_NAC_BAD( SRC16, DST16, DSTP16 ) )
			break ;

		case DX_BLENDMODE_SUB :		// ���Z�u�����h
			if( MemImgManage.BlendParam == 0 ) return ;
			
			DRAWLINEMEMIMG_ND( CODE_NBI_C16_NBR_NAC_BSB( SRC16, DST16, DSTP16 ) )
			break ;

		case DX_BLENDMODE_MUL :		// ��Z�u�����h
			DRAWLINEMEMIMG_ND( CODE_NBI_C16_NBR_NAC_BML( SRC16, DST16, DSTP16 ) )
			break ;

		case DX_BLENDMODE_MULA :	// ��Z+���u�����h
			DRAWLINEMEMIMG_ND( CODE_NBI_C16_NBR_NAC_BMA( SRC16, DST16, DSTP16 ) )
			break ;

		case DX_BLENDMODE_INVSRC :	// �`�挳�̐F�𔽓]�{���u�����h
			DRAWLINEMEMIMG_ND( CODE_NBI_C16_NBR_NAC_BIS( SRC16, DST16, DSTP16 ) )
			break ;
		}
		break ;
		
	case 1 :	// 32bit ���[�h
		// �`��P�x�𔽉f
		if( ( MemImgManage.bDrawBright & 0xffffff ) != 0xffffff )
		{
			((BYTE *)&Color)[2] = ( BYTE )( ( ((BYTE *)&Color)[2] * MemImgManage.DrawBright.Red   ) >> 8 ) ;
			((BYTE *)&Color)[1] = ( BYTE )( ( ((BYTE *)&Color)[1] * MemImgManage.DrawBright.Green ) >> 8 ) ;
			((BYTE *)&Color)[0] = ( BYTE )( ( ((BYTE *)&Color)[0] * MemImgManage.DrawBright.Blue  ) >> 8 ) ;
		}
		
		// �u�����h���[�h�ɂ���ď����𕪊�
		switch( MemImgManage.BlendMode )
		{
		case DX_BLENDMODE_NOBLEND :		// �u�����h����
NOTEX_NOMALDRAW_C32_NOPAL_BNO:
			DRAWLINEMEMIMG_ND( CODE_NBI_C32_NBR_NAC_BNO( SRCP32, DSTP32 ) )
			break ;

		case DX_BLENDMODE_ALPHA :		// ���u�����h
			if( MemImgManage.BlendParam == 255 ) goto NOTEX_NOMALDRAW_C32_NOPAL_BNO ;
			else
			if( MemImgManage.BlendParam == 0 ) return ;
			
			DRAWLINEMEMIMG_ND( CODE_NBI_C32_NBR_NAC_BAL( SRCP32, DSTP32 ) )
			break ;

		case DX_BLENDMODE_ADD :		// ���Z�u�����h
			if( MemImgManage.BlendParam == 0 ) return ;
			
			DRAWLINEMEMIMG_ND( CODE_NBI_C32_NBR_NAC_BAD( SRCP32, DSTP32 ) )
			break ;

		case DX_BLENDMODE_SUB :		// ���Z�u�����h
			if( MemImgManage.BlendParam == 0 ) return ;
			
			DRAWLINEMEMIMG_ND( CODE_NBI_C32_NBR_NAC_BSB( SRCP32, DSTP32 ) )
			break ;

		case DX_BLENDMODE_MUL :		// ��Z�u�����h
			DRAWLINEMEMIMG_ND( CODE_NBI_C32_NBR_NAC_BML( SRCP32, DSTP32 ) )
			break ;

		case DX_BLENDMODE_MULA :	// ��Z+���u�����h
			DRAWLINEMEMIMG_ND( CODE_NBI_C32_NBR_NAC_BMA( SRCP32, DSTP32 ) )
			break ;

		case DX_BLENDMODE_INVSRC :	// �`�挳�̐F�𔽓]�{���u�����h
			DRAWLINEMEMIMG_ND( CODE_NBI_C32_NBR_NAC_BIS( SRCP32, DSTP32 ) )
			break ;
		}
		break ;
	}

	// �I��
	return ;
}
#undef DST16
#undef SRC16
#undef DSTP16

#undef SRCP32
#undef DSTP32


// �C���[�W�ɘg�����̃{�b�N�X��`�悷��
extern void DrawLineBoxMemImg( MEMIMG *DestImg, int x1, int y1, int x2, int y2, unsigned int Color )
{
	DrawLineMemImg( DestImg, x1,		y1,		x2,		y1,		Color ) ;
	DrawLineMemImg( DestImg, x2 - 1,	y1 + 1, x2 - 1, y2,		Color ) ;
	DrawLineMemImg( DestImg, x1,		y2 - 1, x2 - 1,	y2 - 1, Color ) ;
	DrawLineMemImg( DestImg, x1,		y1 + 1,	x1,		y2 - 1,	Color ) ;
}

// �C���[�W�ɓ_�̏W����`�悷��
extern void DrawPixelSetMemImg( MEMIMG *DestImg, const POINTDATA *PointData, int Num )
{
	int i ;
	DWORD DestPitch ;
	RECT DrawArea ;
	union
	{
		BYTE *DestBP ;
		WORD *DestWP ;
		DWORD *DestDP ;
	} ;
	
	DestBP    = DestImg->UseImage ;
	DestPitch = DestImg->Base->Pitch ;
	DrawArea  = MemImgManage.DrawArea ;

	switch( DestImg->Base->ColorType )
	{
	case 0 :	// 16bit
		for( i = 0 ; i < Num ; i ++, PointData ++ )
		{
			if( ( ( PointData->x - DrawArea.left ) | ( DrawArea.right  - ( PointData->x + 1 ) ) |
				  ( PointData->y - DrawArea.top  ) | ( DrawArea.bottom - ( PointData->y + 1 ) ) ) & 0x80000000 ) continue ;

			*((WORD *)( DestBP + PointData->x * 2 + PointData->y * DestPitch )) = ( WORD )PointData->color ;
		}
		break ;
		
	case 1 :	// 32bit
		for( i = 0 ; i < Num ; i ++, PointData ++ )
		{
			if( ( ( PointData->x - DrawArea.left ) | ( DrawArea.right  - ( PointData->x + 1 ) ) |
				  ( PointData->y - DrawArea.top  ) | ( DrawArea.bottom - ( PointData->y + 1 ) ) ) & 0x80000000 ) continue ;

			*((DWORD *)( DestBP + PointData->x * 4 + PointData->y * DestPitch )) = ( DWORD )PointData->color ;
		}
		break ;
	}
}

// �C���[�W�ɐ��̏W����`�悷��
extern void DrawLineSetMemImg( MEMIMG *DestImg, const LINEDATA *LineData, int Num )
{
	int i ;
	for( i = 0 ; i < Num ; i ++, LineData ++ )
	{
		DrawLineMemImg( DestImg, LineData->x1, LineData->y1, LineData->x2, LineData->y2, LineData->color ) ;
	}
}

#define DRAWCIRCLEMEMIMG_FILL_SHRED_ND( DRAW, DESTADDNUM )	\
	if( !( ( x2 < DrawArea.left ) | ( x1 >= DrawArea.right  ) |\
		   ( y1 < DrawArea.top  ) | ( y1 >= DrawArea.bottom ) | LineDrawBuf[ y1 ] ) )\
	{\
		if( x1 < DrawArea.left  ) x1 = DrawArea.left  ;\
		if( x2 > DrawArea.right ) x2 = DrawArea.right ;\
\
		LineDrawBuf[ y1 ] = 1 ;\
\
		C = x2 - x1 + 1 ;\
		DrawBP = DestBP + DestPitch * y1 + x1 * (DESTADDNUM) ;\
		do{\
			DRAW\
			DrawBP += (DESTADDNUM) ;\
		}while( -- C ) ;\
	}


#define DRAWCIRCLEMEMIMG_FILL_ND( DRAW, DESTADDNUM )	\
{\
	x2 = Dx + x ; x1 = -Dx + x ; y1 = Dy + y ;\
	DRAWCIRCLEMEMIMG_FILL_SHRED_ND( DRAW, DESTADDNUM )\
\
	if( F >= 0 )\
	{\
		x2 = Dy + x ; x1 = -Dy + x ; y1 = Dx + y ;\
		DRAWCIRCLEMEMIMG_FILL_SHRED_ND( DRAW, DESTADDNUM )\
\
		x2 = Dy + x ; x1 = -Dy + x ; y1 = -Dx + y;\
		DRAWCIRCLEMEMIMG_FILL_SHRED_ND( DRAW, DESTADDNUM )\
\
		Dx -- ;\
		F -= 4 * Dx ;\
	}\
\
	Dy ++ ;\
	F += 4 * Dy + 2 ;\
\
	while( Dx >= Dy )\
	{\
		x2 = Dx + x ; x1 = -Dx + x ; y1 = Dy + y ;\
		DRAWCIRCLEMEMIMG_FILL_SHRED_ND( DRAW, DESTADDNUM )\
\
		x2 = Dx + x ; x1 = -Dx + x ; y1 = -Dy + y;\
		DRAWCIRCLEMEMIMG_FILL_SHRED_ND( DRAW, DESTADDNUM )\
\
		if( F >= 0 )\
		{\
			x2 = Dy + x ; x1 = -Dy + x ; y1 = Dx + y ;\
			DRAWCIRCLEMEMIMG_FILL_SHRED_ND( DRAW, DESTADDNUM )\
\
			x2 = Dy + x ; x1 = -Dy + x ; y1 = -Dx + y;\
			DRAWCIRCLEMEMIMG_FILL_SHRED_ND( DRAW, DESTADDNUM )\
\
			Dx -- ;\
			F -= 4 * Dx ;\
		}\
\
		Dy ++ ;\
		F += 4 * Dy + 2 ;\
	}\
}



#define DRAWCIRCLEMEMIMG_PSET_SHRED_ND( DRAW, DESTADDNUM )	\
	if( ( ( (DWORD)( x1 - DrawArea.left ) <= (DWORD)( DrawArea.right  - DrawArea.left ) ) &&\
		  ( (DWORD)( y1 - DrawArea.top  ) <= (DWORD)( DrawArea.bottom - DrawArea.top  ) ) ) )\
	{\
		DrawBP = DestBP + DestPitch * y1 + x1 * (DESTADDNUM) ;\
		DRAW\
	}

#define DRAWCIRCLEMEMIMG_PSET_ND( DRAW, DESTADDNUM )	\
{\
	x1 = -Dx + x ; y1 = Dy + y ;\
	DRAWCIRCLEMEMIMG_PSET_SHRED_ND( DRAW, DESTADDNUM )\
\
	x1 = Dx + x ;\
	DRAWCIRCLEMEMIMG_PSET_SHRED_ND( DRAW, DESTADDNUM )\
\
	x1 = Dy + x ; y1 = Dx + y ;\
	DRAWCIRCLEMEMIMG_PSET_SHRED_ND( DRAW, DESTADDNUM )\
\
	x1 = -Dy + x ; y1 = -Dx + y;\
	DRAWCIRCLEMEMIMG_PSET_SHRED_ND( DRAW, DESTADDNUM )\
\
	if( F >= 0 )\
	{\
		Dx -- ;\
		F -= 4 * Dx ;\
	}\
\
	Dy ++ ;\
	F += 4 * Dy + 2 ;\
\
	while( Dx >= Dy )\
	{\
		x1 = -Dx + x ; y1 = Dy + y ;\
		DRAWCIRCLEMEMIMG_PSET_SHRED_ND( DRAW, DESTADDNUM )\
\
		x1 = Dx + x ;\
		DRAWCIRCLEMEMIMG_PSET_SHRED_ND( DRAW, DESTADDNUM )\
\
		x1 = -Dx + x ; y1 = -Dy + y;\
		DRAWCIRCLEMEMIMG_PSET_SHRED_ND( DRAW, DESTADDNUM )\
\
		x1 = Dx + x ;\
		DRAWCIRCLEMEMIMG_PSET_SHRED_ND( DRAW, DESTADDNUM )\
\
\
		x1 = -Dy + x ; y1 = Dx + y ;\
		DRAWCIRCLEMEMIMG_PSET_SHRED_ND( DRAW, DESTADDNUM )\
\
		x1 = Dy + x ;\
		DRAWCIRCLEMEMIMG_PSET_SHRED_ND( DRAW, DESTADDNUM )\
\
		x1 = -Dy + x ; y1 = -Dx + y;\
		DRAWCIRCLEMEMIMG_PSET_SHRED_ND( DRAW, DESTADDNUM )\
\
		x1 = Dy + x ;\
		DRAWCIRCLEMEMIMG_PSET_SHRED_ND( DRAW, DESTADDNUM )\
\
		if( F >= 0 )\
		{\
			Dx -- ;\
			F -= 4 * Dx ;\
		}\
\
		Dy ++ ;\
		F += 4 * Dy + 2 ;\
	}\
}



// �C���[�W�ɉ~��`�悷��
#define DST16	*DrawWP
#define SRC16	Color
#define DSTP16	DrawWP

#define SRCP32	(BYTE *)&Color
#define DSTP32	DrawBP
extern void DrawCircleMemImg( MEMIMG *DestImg, int x, int y, int r, unsigned int Color, int FillFlag )
{
	BYTE *LineDrawBuf ;
	RECT DrawArea ;
	DWORD Src1, Data, DataD ;
	DWORD *RateTable1 ;
	DWORD DestPitch ;
	int *RateTable2 ;
	int Dx, Dy, F ;
	int x1, x2, y1, C ;
	union
	{
		BYTE SrcB[4] ;
		DWORD SrcD ;
	} ;
	union
	{
		BYTE  *DrawBP ;
		WORD  *DrawWP ;
		DWORD *DrawDP ;
	} ;
	union
	{
		BYTE  *DestBP ;
		WORD  *DestWP ;
		DWORD *DestDP ;
	} ;

	// �`��̈�̃Z�b�g
	DrawArea = MemImgManage.DrawArea ;
	DrawArea.right  -- ;
	DrawArea.bottom -- ;

	// �]����̃A�h���X���Z�b�g
	DestBP    = DestImg->UseImage ;
	DestPitch = DestImg->Base->Pitch ;

	// �`��̏���
	RateTable1 = MemImgManage.RateTable[MemImgManage.BlendParam] ;
	RateTable2 = &MemImgManage.RateTable2[MemImgManage.BlendParam][256] ;

	// �����l�Z�b�g
	Dx = r ;
	Dy = 0 ;
	F = -2 * r + 3 ;

	// �h��Ԃ����ǂ����ŏ����𕪊�
	if( FillFlag )
	{
		if( ( LineDrawBuf = (BYTE *)DXCALLOC( ( size_t )( DrawArea.bottom + 1 ) ) ) == NULL )
			return ;

		// �J���[���[�h�ɂ���ď����𕪊�
		switch( DestImg->Base->ColorType )
		{
		case 0 :	// 16bit ���[�h
			// �`��P�x�𔽉f
			if( ( MemImgManage.bDrawBright & 0xffffff ) != 0xffffff )
			{
				Color = RUP16( ( RBOT16( Color ) * MemImgManage.DrawBright.Red   ) >> 8 ) |
						GUP16( ( GBOT16( Color ) * MemImgManage.DrawBright.Green ) >> 8 ) |
						BUP16( ( BBOT16( Color ) * MemImgManage.DrawBright.Blue  ) >> 8 ) ;
			}
			
			// �u�����h���[�h�ɂ���ď����𕪊�
			switch( MemImgManage.BlendMode )
			{
			case DX_BLENDMODE_NOBLEND :		// �u�����h����
FILL_NOMALDRAW_C16_NOPAL_BNO:
				DRAWCIRCLEMEMIMG_FILL_ND( CODE_NBI_C16_NBR_NAC_BNO( SRC16, DST16, DSTP16 ), 2 )
				break ;

			case DX_BLENDMODE_ALPHA :		// ���u�����h
				if( MemImgManage.BlendParam == 255 ) goto FILL_NOMALDRAW_C16_NOPAL_BNO ;
				else
				if( MemImgManage.BlendParam == 0 ) goto LINEEND ;
				
				DRAWCIRCLEMEMIMG_FILL_ND( CODE_NBI_C16_NBR_NAC_BAL( SRC16, DST16, DSTP16 ), 2 )
				break ;

			case DX_BLENDMODE_ADD :		// ���Z�u�����h
				if( MemImgManage.BlendParam == 0 ) goto LINEEND ;
				
				DRAWCIRCLEMEMIMG_FILL_ND( CODE_NBI_C16_NBR_NAC_BAD( SRC16, DST16, DSTP16 ), 2 )
				break ;

			case DX_BLENDMODE_SUB :		// ���Z�u�����h
				if( MemImgManage.BlendParam == 0 ) goto LINEEND ;
				
				DRAWCIRCLEMEMIMG_FILL_ND( CODE_NBI_C16_NBR_NAC_BSB( SRC16, DST16, DSTP16 ), 2 )
				break ;

			case DX_BLENDMODE_MUL :		// ��Z�u�����h
				DRAWCIRCLEMEMIMG_FILL_ND( CODE_NBI_C16_NBR_NAC_BML( SRC16, DST16, DSTP16 ), 2 )
				break ;

			case DX_BLENDMODE_MULA :	// ��Z+���u�����h
				DRAWCIRCLEMEMIMG_FILL_ND( CODE_NBI_C16_NBR_NAC_BMA( SRC16, DST16, DSTP16 ), 2 )
				break ;

			case DX_BLENDMODE_INVSRC :	// �`�挳�̐F�𔽓]�{���u�����h
				DRAWCIRCLEMEMIMG_FILL_ND( CODE_NBI_C16_NBR_NAC_BIS( SRC16, DST16, DSTP16 ), 2 )
				break ;
			}
			break ;
			
		case 1 :	// 32bit ���[�h
			if( ( MemImgManage.bDrawBright & 0xffffff ) != 0xffffff )
			{
				((BYTE *)&Color)[2] = ( BYTE )( ( ((BYTE *)&Color)[2] * MemImgManage.DrawBright.Red   ) >> 8 ) ;
				((BYTE *)&Color)[1] = ( BYTE )( ( ((BYTE *)&Color)[1] * MemImgManage.DrawBright.Green ) >> 8 ) ;
				((BYTE *)&Color)[0] = ( BYTE )( ( ((BYTE *)&Color)[0] * MemImgManage.DrawBright.Blue  ) >> 8 ) ;
			}
			
			// �u�����h���[�h�ɂ���ď����𕪊�
			switch( MemImgManage.BlendMode )
			{
			case DX_BLENDMODE_NOBLEND :		// �u�����h����
FILL_NOMALDRAW_C32_NOPAL_BNO:
				DRAWCIRCLEMEMIMG_FILL_ND( CODE_NBI_C32_NBR_NAC_BNO( SRCP32, DSTP32 ), 4 )
				break ;

			case DX_BLENDMODE_ALPHA :		// ���u�����h
				if( MemImgManage.BlendParam == 255 ) goto FILL_NOMALDRAW_C32_NOPAL_BNO ;
				else
				if( MemImgManage.BlendParam == 0 ) goto LINEEND ;
				
				DRAWCIRCLEMEMIMG_FILL_ND( CODE_NBI_C32_NBR_NAC_BAL( SRCP32, DSTP32 ), 4 )
				break ;

			case DX_BLENDMODE_ADD :		// ���Z�u�����h
				if( MemImgManage.BlendParam == 0 ) goto LINEEND ;
				
				DRAWCIRCLEMEMIMG_FILL_ND( CODE_NBI_C32_NBR_NAC_BAD( SRCP32, DSTP32 ), 4 )
				break ;

			case DX_BLENDMODE_SUB :		// ���Z�u�����h
				if( MemImgManage.BlendParam == 0 ) goto LINEEND ;
				
				DRAWCIRCLEMEMIMG_FILL_ND( CODE_NBI_C32_NBR_NAC_BSB( SRCP32, DSTP32 ), 4 )
				break ;

			case DX_BLENDMODE_MUL :		// ��Z�u�����h
				DRAWCIRCLEMEMIMG_FILL_ND( CODE_NBI_C32_NBR_NAC_BML( SRCP32, DSTP32 ), 4 )
				break ;

			case DX_BLENDMODE_MULA :	// ��Z+���u�����h
				DRAWCIRCLEMEMIMG_FILL_ND( CODE_NBI_C32_NBR_NAC_BMA( SRCP32, DSTP32 ), 4 )
				break ;

			case DX_BLENDMODE_INVSRC :	// �`�挳�̐F�𔽓]�{���u�����h
				DRAWCIRCLEMEMIMG_FILL_ND( CODE_NBI_C32_NBR_NAC_BIS( SRCP32, DSTP32 ), 4 )
				break ;
			}
			break ;
		}

LINEEND:
		DXFREE( LineDrawBuf ) ;
	}
	else
	{
		// �J���[���[�h�ɂ���ď����𕪊�
		switch( DestImg->Base->ColorType )
		{
		case 0 :	// 16bit ���[�h
			// �`��P�x�𔽉f
			if( ( MemImgManage.bDrawBright & 0xffffff ) != 0xffffff )
			{
				Color = RUP16( ( RBOT16( Color ) * MemImgManage.DrawBright.Red   ) >> 8 ) |
						GUP16( ( GBOT16( Color ) * MemImgManage.DrawBright.Green ) >> 8 ) |
						BUP16( ( BBOT16( Color ) * MemImgManage.DrawBright.Blue  ) >> 8 ) ;
			}
			
			// �u�����h���[�h�ɂ���ď����𕪊�
			switch( MemImgManage.BlendMode )
			{
			case DX_BLENDMODE_NOBLEND :		// �u�����h����
PSET_NOMALDRAW_C16_NOPAL_BNO:
				DRAWCIRCLEMEMIMG_PSET_ND( CODE_NBI_C16_NBR_NAC_BNO( SRC16, DST16, DSTP16 ), 2 )
				break ;

			case DX_BLENDMODE_ALPHA :		// ���u�����h
				if( MemImgManage.BlendParam == 255 ) goto PSET_NOMALDRAW_C16_NOPAL_BNO ;
				else
				if( MemImgManage.BlendParam == 0 ) return ;
				
				DRAWCIRCLEMEMIMG_PSET_ND( CODE_NBI_C16_NBR_NAC_BAL( SRC16, DST16, DSTP16 ), 2 )
				break ;

			case DX_BLENDMODE_ADD :		// ���Z�u�����h
				if( MemImgManage.BlendParam == 0 ) return ;
				
				DRAWCIRCLEMEMIMG_PSET_ND( CODE_NBI_C16_NBR_NAC_BAD( SRC16, DST16, DSTP16 ), 2 )
				break ;

			case DX_BLENDMODE_SUB :		// ���Z�u�����h
				if( MemImgManage.BlendParam == 0 ) return ;
				
				DRAWCIRCLEMEMIMG_PSET_ND( CODE_NBI_C16_NBR_NAC_BSB( SRC16, DST16, DSTP16 ), 2 )
				break ;

			case DX_BLENDMODE_MUL :		// ��Z�u�����h
				DRAWCIRCLEMEMIMG_PSET_ND( CODE_NBI_C16_NBR_NAC_BML( SRC16, DST16, DSTP16 ), 2 )
				break ;

			case DX_BLENDMODE_MULA :	// ��Z+���u�����h
				DRAWCIRCLEMEMIMG_PSET_ND( CODE_NBI_C16_NBR_NAC_BMA( SRC16, DST16, DSTP16 ), 2 )
				break ;

			case DX_BLENDMODE_INVSRC :	// �`�挳�̐F�𔽓]�{���u�����h
				DRAWCIRCLEMEMIMG_PSET_ND( CODE_NBI_C16_NBR_NAC_BIS( SRC16, DST16, DSTP16 ), 2 )
				break ;
			}
			break ;
			
		case 1 :	// 32bit ���[�h
			if( ( MemImgManage.bDrawBright & 0xffffff ) != 0xffffff )
			{
				((BYTE *)&Color)[2] = ( BYTE )( ( ((BYTE *)&Color)[2] * MemImgManage.DrawBright.Red   ) >> 8 ) ;
				((BYTE *)&Color)[1] = ( BYTE )( ( ((BYTE *)&Color)[1] * MemImgManage.DrawBright.Green ) >> 8 ) ;
				((BYTE *)&Color)[0] = ( BYTE )( ( ((BYTE *)&Color)[0] * MemImgManage.DrawBright.Blue  ) >> 8 ) ;
			}
			
			// �u�����h���[�h�ɂ���ď����𕪊�
			switch( MemImgManage.BlendMode )
			{
			case DX_BLENDMODE_NOBLEND :		// �u�����h����
PSET_NOMALDRAW_C32_NOPAL_BNO:
				DRAWCIRCLEMEMIMG_PSET_ND( CODE_NBI_C32_NBR_NAC_BNO( SRCP32, DSTP32 ), 4 )
				break ;

			case DX_BLENDMODE_ALPHA :		// ���u�����h
				if( MemImgManage.BlendParam == 255 ) goto PSET_NOMALDRAW_C32_NOPAL_BNO ;
				else
				if( MemImgManage.BlendParam == 0 ) return ;
				
				DRAWCIRCLEMEMIMG_PSET_ND( CODE_NBI_C32_NBR_NAC_BAL( SRCP32, DSTP32 ), 4 )
				break ;

			case DX_BLENDMODE_ADD :		// ���Z�u�����h
				if( MemImgManage.BlendParam == 0 ) return ;
				
				DRAWCIRCLEMEMIMG_PSET_ND( CODE_NBI_C32_NBR_NAC_BAD( SRCP32, DSTP32 ), 4 )
				break ;

			case DX_BLENDMODE_SUB :		// ���Z�u�����h
				if( MemImgManage.BlendParam == 0 ) return ;
				
				DRAWCIRCLEMEMIMG_PSET_ND( CODE_NBI_C32_NBR_NAC_BSB( SRCP32, DSTP32 ), 4 )
				break ;

			case DX_BLENDMODE_MUL :		// ��Z�u�����h
				DRAWCIRCLEMEMIMG_PSET_ND( CODE_NBI_C32_NBR_NAC_BML( SRCP32, DSTP32 ), 4 )
				break ;

			case DX_BLENDMODE_MULA :	// ��Z+���u�����h
				DRAWCIRCLEMEMIMG_PSET_ND( CODE_NBI_C32_NBR_NAC_BMA( SRCP32, DSTP32 ), 4 )
				break ;

			case DX_BLENDMODE_INVSRC :	// �`�挳�̐F�𔽓]�{���u�����h
				DRAWCIRCLEMEMIMG_PSET_ND( CODE_NBI_C32_NBR_NAC_BIS( SRCP32, DSTP32 ), 4 )
				break ;
			}
			break ;
		}
	}

	// �I��
	return ;
}
#undef DST16
#undef SRC16
#undef DSTP16

#undef SRCP32
#undef DSTP32



#define DRAWOVALMEMIMG_FILL_RX_ND( DRAW, DESTADDNUM )	\
{\
	x1 = -Dx + x ; x2 = Dx + x ; y1 = Dy + y ;\
	DRAWCIRCLEMEMIMG_FILL_SHRED_ND( DRAW, DESTADDNUM )\
\
	Df = 0 ;\
	if( F < 0 ){\
		yc += ry ;\
		if( yc >= rc ){\
			Dy ++ ;\
			yc -= rc ;\
			Df  = 1  ;\
		}\
		F += 4 * Dy + 2 ;\
		H += 4 * Dy     ;\
	}else{\
		if( H >= 0 ){\
			Dx -- ;\
			F -= 4 * Dx     ;\
			H -= 4 * Dx - 2 ;\
		}else{\
			Dx -- ;\
			yc += ry ;\
			if( yc >= rc ){\
				Dy ++ ;\
				yc -= rc ;\
				Df  = 1  ;\
			}\
			F += 4 * Dy - 4 * Dx + 2 ;\
			H += 4 * Dy - 4 * Dx + 2 ;\
		}\
	}\
\
	while( Dx >= 0 )\
	{\
		if( Df )\
		{\
			x1 = -Dx + x ; x2 = Dx + x ; y1 = Dy + y ;\
			DRAWCIRCLEMEMIMG_FILL_SHRED_ND( DRAW, DESTADDNUM )\
\
			y1 = -Dy + y ;\
			DRAWCIRCLEMEMIMG_FILL_SHRED_ND( DRAW, DESTADDNUM )\
		}\
\
		Df = 0 ;\
		if( F < 0 ){\
			yc += ry ;\
			if( yc >= rc ){\
				Dy ++ ;\
				yc -= rc ;\
				Df = 1 ;\
			}\
			F += 4 * Dy + 2 ;\
			H += 4 * Dy ;\
		}else{\
			if( H >= 0 ){\
				Dx -- ;\
				F -= 4 * Dx ;\
				H -= 4 * Dx - 2 ;\
			}else{\
				Dx -- ;\
				yc += ry ;\
				if( yc >= rc ){\
					Dy ++ ;\
					yc -= rc ;\
					Df = 1 ;\
				}\
				F += 4 * Dy - 4 * Dx + 2 ;\
				H += 4 * Dy - 4 * Dx + 2 ;\
			}\
		}\
	}\
\
	x1 = -Dx + x ; x2 = Dx + x ; y1 = -Dy + y;\
	DRAWCIRCLEMEMIMG_FILL_SHRED_ND( DRAW, DESTADDNUM )\
}


#define DRAWOVALMEMIMG_FILL_RY_ND( DRAW, DESTADDNUM )	\
{\
	x1 = -Dx + x ; x2 = Dx + x ; y1 = Dy + y ;\
	DRAWCIRCLEMEMIMG_FILL_SHRED_ND( DRAW, DESTADDNUM )\
\
	Df = 0 ;\
	if( F < 0 ){\
		Df = 1 ;\
		yc += ry ;\
		Dy ++ ;\
		F += 4 * Dy + 2 ;\
		H += 4 * Dy ;\
	}else{\
		if( H >= 0 ){\
			xc += rx ;\
			if( xc >= rc ){\
				Dx -- ;\
				xc -= rc ;\
			}\
			F -= 4 * Dx ;\
			H -= 4 * Dx - 2 ;\
		}else{\
			xc += rx ;\
			if( xc >= rc ){\
				Dx -- ;\
				xc -= rc ;\
			}\
			Dy ++ ;\
			Df = 1 ;\
\
			F += 4 * Dy - 4 * Dx + 2 ;\
			H += 4 * Dy - 4 * Dx + 2 ;\
		}\
	}\
\
	while( Dx > 0 )\
	{\
		if( Df )\
		{\
			x1 = -Dx + x ; x2 = Dx + x ; y1 = Dy + y ;\
			DRAWCIRCLEMEMIMG_FILL_SHRED_ND( DRAW, DESTADDNUM )\
\
			y1 = -Dy + y ;\
			DRAWCIRCLEMEMIMG_FILL_SHRED_ND( DRAW, DESTADDNUM )\
		}\
\
		Df = 0 ;\
		if( F < 0 ){\
			Df = 1 ;\
			yc += ry ;\
			Dy ++ ;\
			F += 4 * Dy + 2 ;\
			H += 4 * Dy ;\
		}else{\
			if( H >= 0 ){\
				xc += rx ;\
				if( xc >= rc ){\
					Dx -- ;\
					xc -= rc ;\
				}\
				F -= 4 * Dx ;\
				H -= 4 * Dx - 2 ;\
			}else{\
				xc += rx ;\
				if( xc >= rc ){\
					Dx -- ;\
					xc -= rc ;\
				}\
				Dy ++ ;\
				Df = 1 ;\
\
				F += 4 * Dy - 4 * Dx + 2 ;\
				H += 4 * Dy - 4 * Dx + 2 ;\
			}\
		}\
	}\
\
	x1 = -Dx + x ; x2 = Dx + x ; y1 = -Dy + y ;\
	DRAWCIRCLEMEMIMG_FILL_SHRED_ND( DRAW, DESTADDNUM )\
}


#define DRAWOVALMEMIMG_PSET_RX_ND( DRAW, DESTADDNUM )	\
{\
	x1 = -Dx + x ; y1 = Dy + y ;\
	DRAWCIRCLEMEMIMG_PSET_SHRED_ND( DRAW, DESTADDNUM )\
	x1 = Dx + x ;\
	DRAWCIRCLEMEMIMG_PSET_SHRED_ND( DRAW, DESTADDNUM )\
\
	Df = 0 ;\
\
	if( F < 0 ){\
		yc += ry ;\
		if( yc >= rc ){\
			Dy ++ ;\
			yc -= rc ;\
			Df = 1 ;\
		}\
		F += 4 * Dy + 2 ;\
		H += 4 * Dy ;\
	}else{\
		Df = 1 ;\
\
		if( H >= 0 ){\
			Dx -- ;\
			F -= 4 * Dx ;\
			H -= 4 * Dx - 2 ;\
		}else{\
			Dx -- ;\
			yc += ry ;\
			if( yc >= rc ){\
				Dy ++ ;\
				yc -= rc ;\
			}\
			F += 4 * Dy - 4 * Dx + 2 ;\
			H += 4 * Dy - 4 * Dx + 2 ;\
		}\
	}\
\
	while( Dx > 0 )\
	{\
		if( Df )\
		{\
			x1 = -Dx + x ; y1 = Dy + y ;\
			DRAWCIRCLEMEMIMG_PSET_SHRED_ND( DRAW, DESTADDNUM )\
			x1 = Dx + x ;\
			DRAWCIRCLEMEMIMG_PSET_SHRED_ND( DRAW, DESTADDNUM )\
\
			x1 = -Dx + x ; y1 = -Dy + y ;\
			DRAWCIRCLEMEMIMG_PSET_SHRED_ND( DRAW, DESTADDNUM )\
			x1 = Dx + x ;\
			DRAWCIRCLEMEMIMG_PSET_SHRED_ND( DRAW, DESTADDNUM )\
		}\
\
		Df = 0 ;\
		if( F < 0 ){\
			yc += ry ;\
			if( yc >= rc ){\
				Dy ++ ;\
				yc -= rc ;\
				Df = 1 ;\
			}\
			F += 4 * Dy + 2 ;\
			H += 4 * Dy ;\
		}else{\
			Df = 1 ;\
\
			if( H >= 0 ){\
				Dx -- ;\
				F -= 4 * Dx ;\
				H -= 4 * Dx - 2 ;\
			}else{\
				Dx -- ;\
				yc += ry ;\
				if( yc >= rc ){\
					Dy ++ ;\
					yc -= rc ;\
				}\
				F += 4 * Dy - 4 * Dx + 2 ;\
				H += 4 * Dy - 4 * Dx + 2 ;\
			}\
		}\
	}\
\
	x1 = Dx + x ; y1 = -Dy + y ;\
	DRAWCIRCLEMEMIMG_PSET_SHRED_ND( DRAW, DESTADDNUM )\
\
	x1 = -Dx + x ; y1 = Dy + y ;\
	DRAWCIRCLEMEMIMG_PSET_SHRED_ND( DRAW, DESTADDNUM )\
}


#define DRAWOVALMEMIMG_PSET_RY_ND( DRAW, DESTADDNUM )	\
{\
	x1 = -Dx + x ; y1 = Dy + y ;\
	DRAWCIRCLEMEMIMG_PSET_SHRED_ND( DRAW, DESTADDNUM )\
	x1 = Dx + x ;\
	DRAWCIRCLEMEMIMG_PSET_SHRED_ND( DRAW, DESTADDNUM )\
\
	Df = 0 ;\
\
	if( F < 0 ){\
		Df = 1 ;\
\
		Dy ++ ;\
		F += 4 * Dy + 2 ;\
		H += 4 * Dy ;\
	}else{\
		if( H >= 0 ){\
			xc += rx ;\
			if( xc >= rc ){\
				xc -= rc ;\
				Dx -- ;\
				Df = 1 ;\
			}\
			F -= 4 * Dx ;\
			H -= 4 * Dx - 2 ;\
		}else{\
			Df = 1 ;\
			xc += rx ;\
			if( xc >= rc ){\
				xc -= rc ;\
				Dx -- ;\
			}\
\
			Dy ++ ;\
			F += 4 * Dy - 4 * Dx + 2 ;\
			H += 4 * Dy - 4 * Dx + 2 ;\
		}\
	}\
\
	while( Dx > 0 )\
	{\
		if( Df )\
		{\
			x1 = -Dx + x ; y1 = Dy + y ;\
			DRAWCIRCLEMEMIMG_PSET_SHRED_ND( DRAW, DESTADDNUM )\
			x1 = Dx + x ;\
			DRAWCIRCLEMEMIMG_PSET_SHRED_ND( DRAW, DESTADDNUM )\
			x1 = -Dx + x ; y1 = -Dy + y ;\
			DRAWCIRCLEMEMIMG_PSET_SHRED_ND( DRAW, DESTADDNUM )\
			x1 = Dx + x ;\
			DRAWCIRCLEMEMIMG_PSET_SHRED_ND( DRAW, DESTADDNUM )\
		}\
\
		Df = 0 ;\
\
		if( F < 0 ){\
			Df = 1 ;\
\
			Dy ++ ;\
			F += 4 * Dy + 2 ;\
			H += 4 * Dy ;\
		}else{\
			if( H >= 0 ){\
				xc += rx ;\
				if( xc >= rc ){\
					xc -= rc ;\
					Dx -- ;\
					Df = 1 ;\
				}\
				F -= 4 * Dx ;\
				H -= 4 * Dx - 2 ;\
			}else{\
				Df = 1 ;\
				xc += rx ;\
				if( xc >= rc ){\
					xc -= rc ;\
					Dx -- ;\
				}\
\
				Dy ++ ;\
				F += 4 * Dy - 4 * Dx + 2 ;\
				H += 4 * Dy - 4 * Dx + 2 ;\
			}\
		}\
	}\
\
	x1 = Dx + x ; y1 = -Dy + y ;\
	DRAWCIRCLEMEMIMG_PSET_SHRED_ND( DRAW, DESTADDNUM )\
	x1 = -Dx + x ; y1 = Dy + y ;\
	DRAWCIRCLEMEMIMG_PSET_SHRED_ND( DRAW, DESTADDNUM )\
}




// �C���[�W�ɑȉ~��`�悷��
#define DST16	*DrawWP
#define SRC16	Color
#define DSTP16	DrawWP

#define SRCP32	(BYTE *)&Color
#define DSTP32	DrawBP
extern void DrawOvalMemImg( MEMIMG *DestImg, int x, int y, int rx, int ry, unsigned int Color, int FillFlag )
{
	int DestPitch ;
	DWORD Src1, Data, DataD ;
	BYTE *LineDrawBuf ;
	RECT DrawArea ;
	DWORD *RateTable1 ;
	int *RateTable2 ;
	int Dx, Dy, F , H ;
	int x1 , x2 , y1 , Df ;
	int yc , xc , rc , r ;
	int C ;
	
	union
	{
		BYTE SrcB[4] ;
		DWORD SrcD ;
	} ;
	union
	{
		BYTE  *DrawBP ;
		WORD  *DrawWP ;
		DWORD *DrawDP ;
	} ;
	union
	{
		BYTE  *DestBP ;
		WORD  *DestWP ;
		DWORD *DestDP ;
	} ;

	if( ( rx == 0 ) | ( ry == 0 ) ) return ; 

	// �`��̈�̃Z�b�g
	DrawArea = MemImgManage.DrawArea ;
	DrawArea.right  -- ;
	DrawArea.bottom -- ;
	
	// �␳
	if( rx < 0 ) rx = -rx ;
	if( ry < 0 ) ry = -ry ;

	// �]����̃A�h���X���Z�b�g
	DestBP    = DestImg->UseImage ;
	DestPitch = ( int )DestImg->Base->Pitch ;

	// �`��̏���
	RateTable1 = MemImgManage.RateTable[MemImgManage.BlendParam] ;
	RateTable2 = &MemImgManage.RateTable2[MemImgManage.BlendParam][256] ;

	// �`��J�n
	if( FillFlag )
	{
		if( ( LineDrawBuf = ( BYTE * )DXCALLOC( ( size_t )( DrawArea.bottom + 1 ) ) ) == NULL )
			return ;

		if( rx >= ry )
		{
			// �����l���Z�b�g
			rc = ( rx * rx ) / ry ;

			xc = 0 ; yc = 0 ;
			r = rx ;
			Df = 1 ;
	
			Dx = r ;
			Dy = 0;
			F = -2 * r + 1 + 2 * 1;
			H = -4 * r + 2 + 1;
			
			// �J���[���[�h�ɂ���ď����𕪊�
			switch( DestImg->Base->ColorType )
			{
			case 0 :	// 16bit ���[�h
				// �`��P�x�𔽉f
				if( ( MemImgManage.bDrawBright & 0xffffff ) != 0xffffff )
				{
					Color = RUP16( ( RBOT16( Color ) * MemImgManage.DrawBright.Red   ) >> 8 ) |
							GUP16( ( GBOT16( Color ) * MemImgManage.DrawBright.Green ) >> 8 ) |
							BUP16( ( BBOT16( Color ) * MemImgManage.DrawBright.Blue  ) >> 8 ) ;
				}
				
				// �u�����h���[�h�ɂ���ď����𕪊�
				switch( MemImgManage.BlendMode )
				{
				case DX_BLENDMODE_NOBLEND :		// �u�����h����
FILL_RX_NOMALDRAW_C16_NOPAL_BNO:
					DRAWOVALMEMIMG_FILL_RX_ND( CODE_NBI_C16_NBR_NAC_BNO( SRC16, DST16, DSTP16 ), 2 )
					break ;

				case DX_BLENDMODE_ALPHA :		// ���u�����h
					if( MemImgManage.BlendParam == 255 ) goto FILL_RX_NOMALDRAW_C16_NOPAL_BNO ;
					else
					if( MemImgManage.BlendParam == 0 ) goto LINEEND ;
					
					DRAWOVALMEMIMG_FILL_RX_ND( CODE_NBI_C16_NBR_NAC_BAL( SRC16, DST16, DSTP16 ), 2 )
					break ;

				case DX_BLENDMODE_ADD :		// ���Z�u�����h
					if( MemImgManage.BlendParam == 0 ) goto LINEEND ;
					
					DRAWOVALMEMIMG_FILL_RX_ND( CODE_NBI_C16_NBR_NAC_BAD( SRC16, DST16, DSTP16 ), 2 )
					break ;

				case DX_BLENDMODE_SUB :		// ���Z�u�����h
					if( MemImgManage.BlendParam == 0 ) goto LINEEND ;
					
					DRAWOVALMEMIMG_FILL_RX_ND( CODE_NBI_C16_NBR_NAC_BSB( SRC16, DST16, DSTP16 ), 2 )
					break ;

				case DX_BLENDMODE_MUL :		// ��Z�u�����h
					DRAWOVALMEMIMG_FILL_RX_ND( CODE_NBI_C16_NBR_NAC_BML( SRC16, DST16, DSTP16 ), 2 )
					break ;

				case DX_BLENDMODE_MULA :	// ��Z+���u�����h
					DRAWOVALMEMIMG_FILL_RX_ND( CODE_NBI_C16_NBR_NAC_BMA( SRC16, DST16, DSTP16 ), 2 )
					break ;

				case DX_BLENDMODE_INVSRC :	// �`�挳�̐F�𔽓]�{���u�����h
					DRAWOVALMEMIMG_FILL_RX_ND( CODE_NBI_C16_NBR_NAC_BIS( SRC16, DST16, DSTP16 ), 2 )
					break ;
				}
				break ;
				
			case 1 :	// 32bit ���[�h
				if( ( MemImgManage.bDrawBright & 0xffffff ) != 0xffffff )
				{
					((BYTE *)&Color)[2] = ( BYTE )( ( ((BYTE *)&Color)[2] * MemImgManage.DrawBright.Red   ) >> 8 ) ;
					((BYTE *)&Color)[1] = ( BYTE )( ( ((BYTE *)&Color)[1] * MemImgManage.DrawBright.Green ) >> 8 ) ;
					((BYTE *)&Color)[0] = ( BYTE )( ( ((BYTE *)&Color)[0] * MemImgManage.DrawBright.Blue  ) >> 8 ) ;
				}
				
				// �u�����h���[�h�ɂ���ď����𕪊�
				switch( MemImgManage.BlendMode )
				{
				case DX_BLENDMODE_NOBLEND :		// �u�����h����
FILL_RX_NOMALDRAW_C32_NOPAL_BNO:
					DRAWOVALMEMIMG_FILL_RX_ND( CODE_NBI_C32_NBR_NAC_BNO( SRCP32, DSTP32 ), 4 )
					break ;

				case DX_BLENDMODE_ALPHA :		// ���u�����h
					if( MemImgManage.BlendParam == 255 ) goto FILL_RX_NOMALDRAW_C32_NOPAL_BNO ;
					else
					if( MemImgManage.BlendParam == 0 ) goto LINEEND ;
					
					DRAWOVALMEMIMG_FILL_RX_ND( CODE_NBI_C32_NBR_NAC_BAL( SRCP32, DSTP32 ), 4 )
					break ;

				case DX_BLENDMODE_ADD :		// ���Z�u�����h
					if( MemImgManage.BlendParam == 0 ) goto LINEEND ;
					
					DRAWOVALMEMIMG_FILL_RX_ND( CODE_NBI_C32_NBR_NAC_BAD( SRCP32, DSTP32 ), 4 )
					break ;

				case DX_BLENDMODE_SUB :		// ���Z�u�����h
					if( MemImgManage.BlendParam == 0 ) goto LINEEND ;
					
					DRAWOVALMEMIMG_FILL_RX_ND( CODE_NBI_C32_NBR_NAC_BSB( SRCP32, DSTP32 ), 4 )
					break ;

				case DX_BLENDMODE_MUL :		// ��Z�u�����h
					DRAWOVALMEMIMG_FILL_RX_ND( CODE_NBI_C32_NBR_NAC_BML( SRCP32, DSTP32 ), 4 )
					break ;

				case DX_BLENDMODE_MULA :	// ��Z+���u�����h
					DRAWOVALMEMIMG_FILL_RX_ND( CODE_NBI_C32_NBR_NAC_BMA( SRCP32, DSTP32 ), 4 )
					break ;

				case DX_BLENDMODE_INVSRC :	// �`�挳�̐F�𔽓]�{���u�����h
					DRAWOVALMEMIMG_FILL_RX_ND( CODE_NBI_C32_NBR_NAC_BIS( SRCP32, DSTP32 ), 4 )
					break ;
				}
				break ;
			}
		}
		else
		{
			// �����l���Z�b�g
			rc = ( ry * ry ) / rx ;

			xc = 0 ; yc = 0 ;
			r = rx ;
			Df = 1 ;
	
			Dx = r ;
			Dy = 0;
			F = -2 * r + 1 + 2 * 1;
			H = -4 * r + 2 + 1;

			// �J���[���[�h�ɂ���ď����𕪊�
			switch( DestImg->Base->ColorType )
			{
			case 0 :	// 16bit ���[�h
				// �`��P�x�𔽉f
				if( ( MemImgManage.bDrawBright & 0xffffff ) != 0xffffff )
				{
					Color = RUP16( ( RBOT16( Color ) * MemImgManage.DrawBright.Red   ) >> 8 ) |
							GUP16( ( GBOT16( Color ) * MemImgManage.DrawBright.Green ) >> 8 ) |
							BUP16( ( BBOT16( Color ) * MemImgManage.DrawBright.Blue  ) >> 8 ) ;
				}
				
				// �u�����h���[�h�ɂ���ď����𕪊�
				switch( MemImgManage.BlendMode )
				{
				case DX_BLENDMODE_NOBLEND :		// �u�����h����
FILL_RY_NOMALDRAW_C16_NOPAL_BNO:
					DRAWOVALMEMIMG_FILL_RY_ND( CODE_NBI_C16_NBR_NAC_BNO( SRC16, DST16, DSTP16 ), 2 )
					break ;

				case DX_BLENDMODE_ALPHA :		// ���u�����h
					if( MemImgManage.BlendParam == 255 ) goto FILL_RY_NOMALDRAW_C16_NOPAL_BNO ;
					else
					if( MemImgManage.BlendParam == 0 ) goto LINEEND ;
					
					DRAWOVALMEMIMG_FILL_RY_ND( CODE_NBI_C16_NBR_NAC_BAL( SRC16, DST16, DSTP16 ), 2 )
					break ;

				case DX_BLENDMODE_ADD :		// ���Z�u�����h
					if( MemImgManage.BlendParam == 0 ) goto LINEEND ;
					
					DRAWOVALMEMIMG_FILL_RY_ND( CODE_NBI_C16_NBR_NAC_BAD( SRC16, DST16, DSTP16 ), 2 )
					break ;

				case DX_BLENDMODE_SUB :		// ���Z�u�����h
					if( MemImgManage.BlendParam == 0 ) goto LINEEND ;
					
					DRAWOVALMEMIMG_FILL_RY_ND( CODE_NBI_C16_NBR_NAC_BSB( SRC16, DST16, DSTP16 ), 2 )
					break ;

				case DX_BLENDMODE_MUL :		// ��Z�u�����h
					DRAWOVALMEMIMG_FILL_RY_ND( CODE_NBI_C16_NBR_NAC_BML( SRC16, DST16, DSTP16 ), 2 )
					break ;

				case DX_BLENDMODE_MULA :	// ��Z+���u�����h
					DRAWOVALMEMIMG_FILL_RY_ND( CODE_NBI_C16_NBR_NAC_BMA( SRC16, DST16, DSTP16 ), 2 )
					break ;

				case DX_BLENDMODE_INVSRC :	// �`�挳�̐F�𔽓]�{���u�����h
					DRAWOVALMEMIMG_FILL_RY_ND( CODE_NBI_C16_NBR_NAC_BIS( SRC16, DST16, DSTP16 ), 2 )
					break ;
				}
				break ;
				
			case 1 :	// 32bit ���[�h
				if( ( MemImgManage.bDrawBright & 0xffffff ) != 0xffffff )
				{
					((BYTE *)&Color)[2] = ( BYTE )( ( ((BYTE *)&Color)[2] * MemImgManage.DrawBright.Red   ) >> 8 ) ;
					((BYTE *)&Color)[1] = ( BYTE )( ( ((BYTE *)&Color)[1] * MemImgManage.DrawBright.Green ) >> 8 ) ;
					((BYTE *)&Color)[0] = ( BYTE )( ( ((BYTE *)&Color)[0] * MemImgManage.DrawBright.Blue  ) >> 8 ) ;
				}
				
				// �u�����h���[�h�ɂ���ď����𕪊�
				switch( MemImgManage.BlendMode )
				{
				case DX_BLENDMODE_NOBLEND :		// �u�����h����
FILL_RY_NOMALDRAW_C32_NOPAL_BNO:
					DRAWOVALMEMIMG_FILL_RY_ND( CODE_NBI_C32_NBR_NAC_BNO( SRCP32, DSTP32 ), 4 )
					break ;

				case DX_BLENDMODE_ALPHA :		// ���u�����h
					if( MemImgManage.BlendParam == 255 ) goto FILL_RY_NOMALDRAW_C32_NOPAL_BNO ;
					else
					if( MemImgManage.BlendParam == 0 ) goto LINEEND ;
					
					DRAWOVALMEMIMG_FILL_RY_ND( CODE_NBI_C32_NBR_NAC_BAL( SRCP32, DSTP32 ), 4 )
					break ;

				case DX_BLENDMODE_ADD :		// ���Z�u�����h
					if( MemImgManage.BlendParam == 0 ) goto LINEEND ;
					
					DRAWOVALMEMIMG_FILL_RY_ND( CODE_NBI_C32_NBR_NAC_BAD( SRCP32, DSTP32 ), 4 )
					break ;

				case DX_BLENDMODE_SUB :		// ���Z�u�����h
					if( MemImgManage.BlendParam == 0 ) goto LINEEND ;
					
					DRAWOVALMEMIMG_FILL_RY_ND( CODE_NBI_C32_NBR_NAC_BSB( SRCP32, DSTP32 ), 4 )
					break ;

				case DX_BLENDMODE_MUL :		// ��Z�u�����h
					DRAWOVALMEMIMG_FILL_RY_ND( CODE_NBI_C32_NBR_NAC_BML( SRCP32, DSTP32 ), 4 )
					break ;

				case DX_BLENDMODE_MULA :	// ��Z+���u�����h
					DRAWOVALMEMIMG_FILL_RY_ND( CODE_NBI_C32_NBR_NAC_BMA( SRCP32, DSTP32 ), 4 )
					break ;

				case DX_BLENDMODE_INVSRC :	// �`�挳�̐F�𔽓]�{���u�����h
					DRAWOVALMEMIMG_FILL_RY_ND( CODE_NBI_C32_NBR_NAC_BIS( SRCP32, DSTP32 ), 4 )
					break ;
				}
				break ;
			}
		}

LINEEND:
		DXFREE( LineDrawBuf ) ;
	}
	else
	{
		if( rx >= ry )
		{
			// �����l���Z�b�g
			rc = ( rx * rx ) / ry ;

			xc = 0 ; yc = 0 ;
			r = rx ;
			Df = 1 ;
	
			Dx = r ;
			Dy = 0;
			F = -2 * r + 1 + 2 * 1;
			H = -4 * r + 2 + 1;

			// �J���[���[�h�ɂ���ď����𕪊�
			switch( DestImg->Base->ColorType )
			{
			case 0 :	// 16bit ���[�h
				// �`��P�x�𔽉f
				if( ( MemImgManage.bDrawBright & 0xffffff ) != 0xffffff )
				{
					Color = RUP16( ( RBOT16( Color ) * MemImgManage.DrawBright.Red   ) >> 8 ) |
							GUP16( ( GBOT16( Color ) * MemImgManage.DrawBright.Green ) >> 8 ) |
							BUP16( ( BBOT16( Color ) * MemImgManage.DrawBright.Blue  ) >> 8 ) ;
				}
				
				// �u�����h���[�h�ɂ���ď����𕪊�
				switch( MemImgManage.BlendMode )
				{
				case DX_BLENDMODE_NOBLEND :		// �u�����h����
PSET_RX_NOMALDRAW_C16_NOPAL_BNO:
					DRAWOVALMEMIMG_PSET_RX_ND( CODE_NBI_C16_NBR_NAC_BNO( SRC16, DST16, DSTP16 ), 2 )
					break ;

				case DX_BLENDMODE_ALPHA :		// ���u�����h
					if( MemImgManage.BlendParam == 255 ) goto PSET_RX_NOMALDRAW_C16_NOPAL_BNO ;
					else
					if( MemImgManage.BlendParam == 0 ) return ;
					
					DRAWOVALMEMIMG_PSET_RX_ND( CODE_NBI_C16_NBR_NAC_BAL( SRC16, DST16, DSTP16 ), 2 )
					break ;

				case DX_BLENDMODE_ADD :		// ���Z�u�����h
					if( MemImgManage.BlendParam == 0 ) return ;
					
					DRAWOVALMEMIMG_PSET_RX_ND( CODE_NBI_C16_NBR_NAC_BAD( SRC16, DST16, DSTP16 ), 2 )
					break ;

				case DX_BLENDMODE_SUB :		// ���Z�u�����h
					if( MemImgManage.BlendParam == 0 ) return ;
					
					DRAWOVALMEMIMG_PSET_RX_ND( CODE_NBI_C16_NBR_NAC_BSB( SRC16, DST16, DSTP16 ), 2 )
					break ;

				case DX_BLENDMODE_MUL :		// ��Z�u�����h
					DRAWOVALMEMIMG_PSET_RX_ND( CODE_NBI_C16_NBR_NAC_BML( SRC16, DST16, DSTP16 ), 2 )
					break ;

				case DX_BLENDMODE_MULA :	// ��Z+���u�����h
					DRAWOVALMEMIMG_PSET_RX_ND( CODE_NBI_C16_NBR_NAC_BMA( SRC16, DST16, DSTP16 ), 2 )
					break ;

				case DX_BLENDMODE_INVSRC :	// �`�挳�̐F�𔽓]�{���u�����h
					DRAWOVALMEMIMG_PSET_RX_ND( CODE_NBI_C16_NBR_NAC_BIS( SRC16, DST16, DSTP16 ), 2 )
					break ;
				}
				break ;
				
			case 1 :	// 32bit ���[�h
				if( ( MemImgManage.bDrawBright & 0xffffff ) != 0xffffff )
				{
					((BYTE *)&Color)[2] = ( BYTE )( ( ((BYTE *)&Color)[2] * MemImgManage.DrawBright.Red   ) >> 8 ) ;
					((BYTE *)&Color)[1] = ( BYTE )( ( ((BYTE *)&Color)[1] * MemImgManage.DrawBright.Green ) >> 8 ) ;
					((BYTE *)&Color)[0] = ( BYTE )( ( ((BYTE *)&Color)[0] * MemImgManage.DrawBright.Blue  ) >> 8 ) ;
				}
				
				// �u�����h���[�h�ɂ���ď����𕪊�
				switch( MemImgManage.BlendMode )
				{
				case DX_BLENDMODE_NOBLEND :		// �u�����h����
PSET_RX_NOMALDRAW_C32_NOPAL_BNO:
					DRAWOVALMEMIMG_PSET_RX_ND( CODE_NBI_C32_NBR_NAC_BNO( SRCP32, DSTP32 ), 4 )
					break ;

				case DX_BLENDMODE_ALPHA :		// ���u�����h
					if( MemImgManage.BlendParam == 255 ) goto PSET_RX_NOMALDRAW_C32_NOPAL_BNO ;
					else
					if( MemImgManage.BlendParam == 0 ) return ;
					
					DRAWOVALMEMIMG_PSET_RX_ND( CODE_NBI_C32_NBR_NAC_BAL( SRCP32, DSTP32 ), 4 )
					break ;

				case DX_BLENDMODE_ADD :		// ���Z�u�����h
					if( MemImgManage.BlendParam == 0 ) return ;
					
					DRAWOVALMEMIMG_PSET_RX_ND( CODE_NBI_C32_NBR_NAC_BAD( SRCP32, DSTP32 ), 4 )
					break ;

				case DX_BLENDMODE_SUB :		// ���Z�u�����h
					if( MemImgManage.BlendParam == 0 ) return ;
					
					DRAWOVALMEMIMG_PSET_RX_ND( CODE_NBI_C32_NBR_NAC_BSB( SRCP32, DSTP32 ), 4 )
					break ;

				case DX_BLENDMODE_MUL :		// ��Z�u�����h
					DRAWOVALMEMIMG_PSET_RX_ND( CODE_NBI_C32_NBR_NAC_BML( SRCP32, DSTP32 ), 4 )
					break ;

				case DX_BLENDMODE_MULA :	// ��Z+���u�����h
					DRAWOVALMEMIMG_PSET_RX_ND( CODE_NBI_C32_NBR_NAC_BMA( SRCP32, DSTP32 ), 4 )
					break ;

				case DX_BLENDMODE_INVSRC :	// �`�挳�̐F�𔽓]�{���u�����h
					DRAWOVALMEMIMG_PSET_RX_ND( CODE_NBI_C32_NBR_NAC_BIS( SRCP32, DSTP32 ), 4 )
					break ;
				}
				break ;
			}
		}
		else
		{
			// �����l���Z�b�g
			rc = ( ry * ry ) / rx ;

			xc = 0 ; yc = 0 ;
			r = rx ;
			Df = 1 ;
	
			Dx = r ;
			Dy = 0;
			F = -2 * r + 1 + 2 * 1;
			H = -4 * r + 2 + 1;

			// �J���[���[�h�ɂ���ď����𕪊�
			switch( DestImg->Base->ColorType )
			{
			case 0 :	// 16bit ���[�h
				// �`��P�x�𔽉f
				if( ( MemImgManage.bDrawBright & 0xffffff ) != 0xffffff )
				{
					Color = RUP16( ( RBOT16( Color ) * MemImgManage.DrawBright.Red   ) >> 8 ) |
							GUP16( ( GBOT16( Color ) * MemImgManage.DrawBright.Green ) >> 8 ) |
							BUP16( ( BBOT16( Color ) * MemImgManage.DrawBright.Blue  ) >> 8 ) ;
				}
				
				// �u�����h���[�h�ɂ���ď����𕪊�
				switch( MemImgManage.BlendMode )
				{
				case DX_BLENDMODE_NOBLEND :		// �u�����h����
PSET_RY_NOMALDRAW_C16_NOPAL_BNO:
					DRAWOVALMEMIMG_PSET_RY_ND( CODE_NBI_C16_NBR_NAC_BNO( SRC16, DST16, DSTP16 ), 2 )
					break ;

				case DX_BLENDMODE_ALPHA :		// ���u�����h
					if( MemImgManage.BlendParam == 255 ) goto PSET_RY_NOMALDRAW_C16_NOPAL_BNO ;
					else
					if( MemImgManage.BlendParam == 0 ) return ;
					
					DRAWOVALMEMIMG_PSET_RY_ND( CODE_NBI_C16_NBR_NAC_BAL( SRC16, DST16, DSTP16 ), 2 )
					break ;

				case DX_BLENDMODE_ADD :		// ���Z�u�����h
					if( MemImgManage.BlendParam == 0 ) return ;
					
					DRAWOVALMEMIMG_PSET_RY_ND( CODE_NBI_C16_NBR_NAC_BAD( SRC16, DST16, DSTP16 ), 2 )
					break ;

				case DX_BLENDMODE_SUB :		// ���Z�u�����h
					if( MemImgManage.BlendParam == 0 ) return ;
					
					DRAWOVALMEMIMG_PSET_RY_ND( CODE_NBI_C16_NBR_NAC_BSB( SRC16, DST16, DSTP16 ), 2 )
					break ;

				case DX_BLENDMODE_MUL :		// ��Z�u�����h
					DRAWOVALMEMIMG_PSET_RY_ND( CODE_NBI_C16_NBR_NAC_BML( SRC16, DST16, DSTP16 ), 2 )
					break ;

				case DX_BLENDMODE_MULA :	// ��Z+���u�����h
					DRAWOVALMEMIMG_PSET_RY_ND( CODE_NBI_C16_NBR_NAC_BMA( SRC16, DST16, DSTP16 ), 2 )
					break ;

				case DX_BLENDMODE_INVSRC :	// �`�挳�̐F�𔽓]�{���u�����h
					DRAWOVALMEMIMG_PSET_RY_ND( CODE_NBI_C16_NBR_NAC_BIS( SRC16, DST16, DSTP16 ), 2 )
					break ;
				}
				break ;
				
			case 1 :	// 32bit ���[�h
				if( ( MemImgManage.bDrawBright & 0xffffff ) != 0xffffff )
				{
					((BYTE *)&Color)[2] = ( BYTE )( ( ((BYTE *)&Color)[2] * MemImgManage.DrawBright.Red   ) >> 8 ) ;
					((BYTE *)&Color)[1] = ( BYTE )( ( ((BYTE *)&Color)[1] * MemImgManage.DrawBright.Green ) >> 8 ) ;
					((BYTE *)&Color)[0] = ( BYTE )( ( ((BYTE *)&Color)[0] * MemImgManage.DrawBright.Blue  ) >> 8 ) ;
				}
				
				// �u�����h���[�h�ɂ���ď����𕪊�
				switch( MemImgManage.BlendMode )
				{
				case DX_BLENDMODE_NOBLEND :		// �u�����h����
PSET_RY_NOMALDRAW_C32_NOPAL_BNO:
					DRAWOVALMEMIMG_PSET_RY_ND( CODE_NBI_C32_NBR_NAC_BNO( SRCP32, DSTP32 ), 4 )
					break ;

				case DX_BLENDMODE_ALPHA :		// ���u�����h
					if( MemImgManage.BlendParam == 255 ) goto PSET_RY_NOMALDRAW_C32_NOPAL_BNO ;
					else
					if( MemImgManage.BlendParam == 0 ) return ;
					
					DRAWOVALMEMIMG_PSET_RY_ND( CODE_NBI_C32_NBR_NAC_BAL( SRCP32, DSTP32 ), 4 )
					break ;

				case DX_BLENDMODE_ADD :		// ���Z�u�����h
					if( MemImgManage.BlendParam == 0 ) return ;
					
					DRAWOVALMEMIMG_PSET_RY_ND( CODE_NBI_C32_NBR_NAC_BAD( SRCP32, DSTP32 ), 4 )
					break ;

				case DX_BLENDMODE_SUB :		// ���Z�u�����h
					if( MemImgManage.BlendParam == 0 ) return ;
					
					DRAWOVALMEMIMG_PSET_RY_ND( CODE_NBI_C32_NBR_NAC_BSB( SRCP32, DSTP32 ), 4 )
					break ;

				case DX_BLENDMODE_MUL :		// ��Z�u�����h
					DRAWOVALMEMIMG_PSET_RY_ND( CODE_NBI_C32_NBR_NAC_BML( SRCP32, DSTP32 ), 4 )
					break ;

				case DX_BLENDMODE_MULA :	// ��Z+���u�����h
					DRAWOVALMEMIMG_PSET_RY_ND( CODE_NBI_C32_NBR_NAC_BMA( SRCP32, DSTP32 ), 4 )
					break ;

				case DX_BLENDMODE_INVSRC :	// �`�挳�̐F�𔽓]�{���u�����h
					DRAWOVALMEMIMG_PSET_RY_ND( CODE_NBI_C32_NBR_NAC_BIS( SRCP32, DSTP32 ), 4 )
					break ;
				}
				break ;
			}
		}
	}

	// �I��
	return ;
}
#undef DST16
#undef SRC16
#undef DSTP16

#undef SRCP32
#undef DSTP32



#endif

#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE
