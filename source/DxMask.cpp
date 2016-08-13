//-----------------------------------------------------------------------------
// 
// 		�c�w���C�u����		�}�X�N�f�[�^�Ǘ��v���O����
// 
//  	Ver 3.16d
// 
//-----------------------------------------------------------------------------

// �c�w���C�u�����쐬���p��`
#define __DX_MAKE

#include "DxCompileConfig.h"

#ifndef DX_NON_MASK

// �C���N���[�h----------------------------------------------------------------
#include "DxLib.h"
#include "DxStatic.h"
#include "DxMask.h"
#include "DxFont.h"
#include "DxUseCLib.h"
#include "DxFile.h"
#include "DxBaseFunc.h"
#include "DxSystem.h"
#include "DxLog.h"
#include "DxGraphics.h"
#include "DxASyncLoad.h"
#include "DxMemory.h"
#include "DxMath.h"

#ifdef __WINDOWS__
#include "Windows/DxMaskWin.h"
#endif // __WINDOWS__

#ifdef __psp2__
#include "PSVita/DxMaskPSVita.h"
#endif // __psp2__

#ifdef __ORBIS__
#include "PS4/DxMaskPS4.h"
#endif // __ORBIS__

#ifdef __ANDROID__
#include "Android/DxMaskAndroid.h"
#endif // __ANDROID__

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// �}�N����`------------------------------------------------------------------

// �}�X�N�n���h���̗L�����`�F�b�N
#define MASKHCHK( HAND, MPOINT )		HANDLECHK(       DX_HANDLETYPE_GMASK, HAND, *( ( HANDLEINFO ** )&MPOINT ) )
#define MASKHCHK_ASYNC( HAND, MPOINT )	HANDLECHK_ASYNC( DX_HANDLETYPE_GMASK, HAND, *( ( HANDLEINFO ** )&MPOINT ) )

// �^��`----------------------------------------------------------------------

// �f�[�^�錾------------------------------------------------------------------

MASKMANAGEDATA MaskManageData ;

// �֐��v���g�^�C�v�錾 -------------------------------------------------------

// �v���O����------------------------------------------------------------------

// �}�X�N�����̏�����
extern int Mask_Initialize( void )
{
	if( MASKD.InitializeFlag == TRUE )
		return -1 ;

	// �}�X�N�n���h���Ǘ��f�[�^�̏�����
	InitializeHandleManage( DX_HANDLETYPE_GMASK, sizeof( MASKDATA ), MAX_MASK_NUM, Mask_InitializeHandle, Mask_TerminateHandle, L"Mask" ) ;

	// �������t���O�𗧂Ă�
	MASKD.InitializeFlag = TRUE ;

	// �I��
	return 0 ;
}

// �}�X�N�����̌�n��
extern int Mask_Terminate( void )
{
	if( MASKD.InitializeFlag == FALSE )
		return -1 ;

	// �}�X�N�X�N���[�����폜����
	NS_DeleteMaskScreen() ;

	// �쐬�����}�X�N�O���t�B�b�N���폜
	NS_InitMask() ;

	// �������t���O��|��
	MASKD.InitializeFlag = FALSE ;

	// �}�X�N�n���h���̌�n��
	TerminateHandleManage( DX_HANDLETYPE_GMASK ) ;

	// �I��
	return 0 ;
}

// �}�X�N�f�[�^������������
extern int NS_InitMask( void )
{
	if( MASKD.InitializeFlag == FALSE )
		return -1 ;

	// �n���h����������
	return AllHandleSub( DX_HANDLETYPE_GMASK ) ;
}

// �}�X�N�X�N���[�����쐬����
extern int NS_CreateMaskScreen( void )
{
	if( MASKD.InitializeFlag == FALSE )
		return -1 ;

	CheckActiveState() ;

	// �}�X�N�ۑ��p�T�[�t�F�X�ƃ}�X�N�`��p�T�[�t�F�X�̍쐬
	Mask_CreateScreenFunction( TRUE, GSYS.Screen.MainScreenSizeX, GSYS.Screen.MainScreenSizeY ) ; 

	// �}�X�N�T�[�t�F�X���쐬����Ă��邩�t���O�����Ă�
	MASKD.CreateMaskFlag = TRUE ;
	MASKD.MaskValidFlag = MASKD.MaskUseFlag && MASKD.CreateMaskFlag ;

//	// �����}�X�N�g�p�t���O�������Ă�����}�X�N��L���ɂ����Ƃ��s��
//	if( MASKD.MaskUseFlag )
		NS_SetUseMaskScreenFlag( TRUE ) ;

	// �I��
	return 0 ;
}

// �}�X�N�T�[�t�F�X�̍쐬
extern int Mask_CreateSurface( BYTE **MaskBuffer, int *BufferPitch, int Width, int Height, int *TransModeP )
{
	// �s�b�`�̎Z�o
	*BufferPitch = ( Width + 3 ) / 4 * 4 ;

	// �o�b�t�@�̍쐬
	*MaskBuffer = ( BYTE * )DXALLOC( ( size_t )( *BufferPitch * Height ) ) ;
	if( *MaskBuffer == NULL )
		return -1 ;

	// �o�b�t�@�̏�����
	_MEMSET( *MaskBuffer, 0, ( size_t )( *BufferPitch * Height ) ) ;

	// ���ߐF���[�h�̃Z�b�g
	*TransModeP = DX_MASKTRANS_BLACK ;

	// �I��
	return 0 ;
}

// �}�X�N�n���h���̏�����
extern int Mask_InitializeHandle( HANDLEINFO * )
{
	// ���ɉ��������I��
	return 0 ;
}

// �}�X�N�n���h���̌�n��
extern int Mask_TerminateHandle( HANDLEINFO *HandleInfo )
{
	MASKDATA *Mask ;

	Mask = ( MASKDATA * )HandleInfo ;

	// �}�X�N�p�f�[�^�̈�̉��
	if( Mask->SrcData )
	{
		DXFREE( Mask->SrcData ) ;
		Mask->SrcData = NULL ;
	}

	// �I��
	return 0 ;
}

// MakeMask �̎������֐�
static int Mask_MakeMask_Static(
	int MaskHandle,
	int Width,
	int Height,
	int /*ASyncThread*/
)
{
	MASKDATA *Mask ;

	if( MASKD.InitializeFlag == FALSE )
		return -1 ;

	if( MASKHCHK_ASYNC( MaskHandle, Mask ) )
		return -1 ;

	// �}�X�N�ۑ��p�T�[�t�F�X�̍쐬
	if( Mask_CreateSurface( &Mask->SrcData, &Mask->SrcDataPitch, Width, Height, &Mask->TransMode ) < 0 )
		return -1 ;

	// �f�[�^�̃Z�b�g
	Mask->MaskWidth = Width ;
	Mask->MaskHeight = Height ;

	// ����I��
	return 0 ;
}

#ifndef DX_NON_ASYNCLOAD

// MakeMask �̔񓯊��ǂݍ��݃X���b�h����Ă΂��֐�
static void Mask_MakeMask_ASync( ASYNCLOADDATA_COMMON *AParam )
{
	int MaskHandle ;
	int Width ;
	int Height ;
	int Addr ;
	int Result ;

	Addr = 0 ;
	MaskHandle = GetASyncLoadParamInt( AParam->Data, &Addr ) ;
	Width = GetASyncLoadParamInt( AParam->Data, &Addr ) ;
	Height = GetASyncLoadParamInt( AParam->Data, &Addr ) ;

	Result = Mask_MakeMask_Static( MaskHandle, Width, Height, TRUE ) ;

	DecASyncLoadCount( MaskHandle ) ;
	if( Result < 0 )
	{
		SubHandle( MaskHandle ) ;
	}
}
#endif // DX_NON_ASYNCLOAD

// �}�X�N�f�[�^�̒ǉ�
extern int Mask_MakeMask_UseGParam( int Width, int Height, int ASyncLoadFlag )
{
	int MaskHandle ;

	if( MASKD.InitializeFlag == FALSE )
		return -1 ;

	CheckActiveState() ;

	// �n���h���̍쐬
	MaskHandle = AddHandle( DX_HANDLETYPE_GMASK, FALSE, -1 ) ;
	if( MaskHandle == -1 )
	{
		return -1 ;
	}

#ifndef DX_NON_ASYNCLOAD
	if( ASyncLoadFlag )
	{
		ASYNCLOADDATA_COMMON *AParam = NULL ;
		int Addr ;

		// �p�����[�^�ɕK�v�ȃ������̃T�C�Y���Z�o
		Addr = 0 ;
		AddASyncLoadParamInt( NULL, &Addr, MaskHandle ) ;
		AddASyncLoadParamInt( NULL, &Addr, Width ) ;
		AddASyncLoadParamInt( NULL, &Addr, Height ) ;

		// �������̊m��
		AParam = AllocASyncLoadDataMemory( Addr ) ;
		if( AParam == NULL )
			goto ERR ;

		// �����ɕK�v�ȏ����Z�b�g
		AParam->ProcessFunction = Mask_MakeMask_ASync ;
		Addr = 0 ;
		AddASyncLoadParamInt( AParam->Data, &Addr, MaskHandle ) ;
		AddASyncLoadParamInt( AParam->Data, &Addr, Width ) ;
		AddASyncLoadParamInt( AParam->Data, &Addr, Height ) ;

		// �f�[�^��ǉ�
		if( AddASyncLoadData( AParam ) < 0 )
		{
			DXFREE( AParam ) ;
			AParam = NULL ;
			goto ERR ;
		}

		// �񓯊��ǂݍ��݃J�E���g���C���N�������g
		IncASyncLoadCount( MaskHandle, AParam->Index ) ;
	}
	else
#endif // DX_NON_ASYNCLOAD
	{
		if( Mask_MakeMask_Static( MaskHandle, Width, Height, FALSE ) < 0 )
			goto ERR ;
	}

	// �I��
	return MaskHandle ;

ERR :
	SubHandle( MaskHandle ) ;

	return -1 ;
}

// �}�X�N�f�[�^�̒ǉ�
extern int NS_MakeMask( int Width, int Height )
{
	return Mask_MakeMask_UseGParam( Width, Height, GetASyncLoadFlag() ) ;
}

// �}�X�N�f�[�^���폜
extern int NS_DeleteMask( int MaskHandle )
{
	return SubHandle( MaskHandle ) ;
}

#ifdef __WINDOWS__

// �}�X�N�f�[�^�T�[�t�F�X�ɂa�l�o�f�[�^���}�X�N�f�[�^�ƌ����Ăē]��
extern int NS_BmpBltToMask( HBITMAP Bmp, int BmpPointX, int BmpPointY, int MaskHandle )
{
	MASKDATA * MaskData ;
	BITMAP bm ;
	int ColorBitDepth ;
	
	if( MASKD.InitializeFlag == FALSE )
		return -1 ;

	if( MASKHCHK_ASYNC( MaskHandle, MaskData ) )
		return -1 ;

	// �r�b�g�}�b�v�f�[�^�̎擾
	GetObject( Bmp, sizeof( BITMAP ), ( void * )&bm ) ;
	bm.bmWidthBytes += bm.bmWidthBytes % 4 ? 4 - bm.bmWidthBytes % 4 : 0 ;

	// �]������
	ColorBitDepth = NS_GetScreenBitDepth() ;
	{
		BYTE *SrcPoint, *DestPoint ;
		int SrcAddIndex, DestAddIndex ;
		int DestHeight, DestWidth ;

		SrcPoint = ( BYTE *)bm.bmBits + BmpPointX * bm.bmBitsPixel / 8 + BmpPointY * bm.bmWidthBytes ;
		DestPoint = ( BYTE * )MaskData->SrcData ;

		SrcAddIndex = bm.bmWidthBytes - MaskData->MaskWidth * bm.bmBitsPixel / 8 ;
		DestAddIndex = MaskData->SrcDataPitch - MaskData->MaskWidth ;

		DestHeight = MaskData->MaskHeight ;
		DestWidth = MaskData->MaskWidth ;

#ifdef DX_NON_INLINE_ASM
		int i ;
		do
		{
			i = DestWidth ;
			do
			{
				*DestPoint = *SrcPoint ;
				DestPoint ++ ;
				SrcPoint += 3 ;
			}while( -- i != 0 ) ;
			DestPoint += DestAddIndex ;
			SrcPoint += SrcAddIndex ;
		}while( -- DestHeight != 0 ) ;
#else
		__asm{
			PUSH	EDI
			PUSH	ESI
			PUSHF
			CLD
			MOV		EDI, DestPoint
			MOV		ESI, SrcPoint
			MOV		EDX, DestHeight
			MOV		EBX, DestWidth
		LOOP81:
			MOV		ECX, EBX
		LOOP82:
			MOV		AL	, [ESI]
			MOV		[EDI], AL
			INC		EDI
			ADD		ESI, 3
			LOOP	LOOP82

			ADD		ESI, SrcAddIndex
			ADD		EDI, DestAddIndex
			DEC		EDX
			JNZ		LOOP81

			POPF
			POP		ESI
			POP		EDI
		}
#endif
	}

	// �I��
	return 0 ;
}

#endif // __WINDOWS_

// �}�X�N�n���h����BASEIMAGE�f�[�^��]������
extern int NS_GraphImageBltToMask( const BASEIMAGE *BaseImage, int ImageX, int ImageY, int MaskHandle )
{
	MASKDATA * MaskData ;
	int       UseTempBaseImage = FALSE ;
	BASEIMAGE TempBaseImage ;
	int       ColorBitDepth ;
	
	if( MASKD.InitializeFlag == FALSE )
		return -1 ;

	if( MASKHCHK_ASYNC( MaskHandle, MaskData ) )
		return -1 ;

	// BASEIMAGE ���t���J���[( 24bit )�ł�32bit�J���[�ł��Ȃ������ꍇ�͂ł͂Ȃ������ꍇ�̓t���J���[�摜�ɕϊ�
	if( BaseImage->ColorData.Format        != DX_BASEIMAGE_FORMAT_NORMAL ||
		BaseImage->ColorData.FloatTypeFlag != FALSE ||
		( BaseImage->ColorData.ColorBitDepth != 24 &&
		  BaseImage->ColorData.ColorBitDepth != 32 ) ||
		BaseImage->ColorData.RedMask       != 0xff0000 ||
		BaseImage->ColorData.GreenMask     != 0x00ff00 ||
		BaseImage->ColorData.BlueMask      != 0x0000ff )
	{
		UseTempBaseImage = TRUE ;
		NS_CreateRGB8ColorBaseImage( BaseImage->Width, BaseImage->Height, &TempBaseImage ) ;
		NS_BltBaseImage( 0, 0, ( BASEIMAGE * )BaseImage, &TempBaseImage ) ;
		BaseImage = &TempBaseImage ;
	}

	// �]������
	ColorBitDepth = NS_GetScreenBitDepth() ;
	{
		BYTE *SrcPoint,   *DestPoint ;
		int   SrcAddIndex, DestAddIndex ;
		int   DestHeight,  DestWidth ;

		SrcPoint  = ( BYTE * )BaseImage->GraphData + ImageX * BaseImage->ColorData.PixelByte + ImageY * BaseImage->Pitch ;
		DestPoint = ( BYTE * )MaskData->SrcData ;

		SrcAddIndex  = BaseImage->Pitch - MaskData->MaskWidth * BaseImage->ColorData.PixelByte ;
		DestAddIndex = MaskData->SrcDataPitch - MaskData->MaskWidth ;

		DestHeight = MaskData->MaskHeight ;
		DestWidth  = MaskData->MaskWidth ;

#ifdef DX_NON_INLINE_ASM
		int i ;
		if( BaseImage->ColorData.ColorBitDepth == 24 )
		{
			do
			{
				i = DestWidth ;
				do
				{
					*DestPoint = *SrcPoint ;
					DestPoint ++ ;
					SrcPoint += 3 ;
				}while( -- i != 0 ) ;
				DestPoint += DestAddIndex ;
				SrcPoint  += SrcAddIndex ;
			}while( -- DestHeight != 0 ) ;
		}
		else
		{
			do
			{
				i = DestWidth ;
				do
				{
					*DestPoint = *SrcPoint ;
					DestPoint ++ ;
					SrcPoint += 4 ;
				}while( -- i != 0 ) ;
				DestPoint += DestAddIndex ;
				SrcPoint  += SrcAddIndex ;
			}while( -- DestHeight != 0 ) ;
		}
#else
		if( BaseImage->ColorData.ColorBitDepth == 24 )
		{
			__asm{
				PUSH	EDI
				PUSH	ESI
				PUSHF
				CLD
				MOV		EDI, DestPoint
				MOV		ESI, SrcPoint
				MOV		EDX, DestHeight
				MOV		EBX, DestWidth
			LOOP81_24:
				MOV		ECX, EBX
			LOOP82_24:
				MOV		AL	, [ESI]
				MOV		[EDI], AL
				INC		EDI
				ADD		ESI, 3
				LOOP	LOOP82_24

				ADD		ESI, SrcAddIndex
				ADD		EDI, DestAddIndex
				DEC		EDX
				JNZ		LOOP81_24

				POPF
				POP		ESI
				POP		EDI
			}
		}
		else
		{
			__asm{
				PUSH	EDI
				PUSH	ESI
				PUSHF
				CLD
				MOV		EDI, DestPoint
				MOV		ESI, SrcPoint
				MOV		EDX, DestHeight
				MOV		EBX, DestWidth
			LOOP81_32:
				MOV		ECX, EBX
			LOOP82_32:
				MOV		AL	, [ESI]
				MOV		[EDI], AL
				INC		EDI
				ADD		ESI, 4
				LOOP	LOOP82_32

				ADD		ESI, SrcAddIndex
				ADD		EDI, DestAddIndex
				DEC		EDX
				JNZ		LOOP81_32

				POPF
				POP		ESI
				POP		EDI
			}
		}
#endif
	}

	// �t���J���[�摜�ɕϊ��������̂��g�p�����ꍇ�̓t���J���[�摜���������
	if( UseTempBaseImage )
	{
		NS_ReleaseBaseImage( &TempBaseImage ) ;
	}

	// �I��
	return 0 ;
}

// �}�X�N�̑傫���𓾂� 
extern int NS_GetMaskSize( int *WidthBuf, int *HeightBuf, int MaskHandle )
{
	MASKDATA * Mask ;
	
	if( MASKD.InitializeFlag == FALSE )
		return -1 ;

	if( MASKHCHK( MaskHandle, Mask ) )
		return -1 ;

	*WidthBuf = Mask->MaskWidth ;
	*HeightBuf = Mask->MaskHeight ;

	// �I��
	return 0 ;
}

// LoadMask �̎������֐�
static int Mask_LoadMask_Static(
	int MaskHandle,
	const wchar_t *FileName,
	int /*ASyncThread*/
)
{
	BASEIMAGE BaseImage ;
	MASKDATA * Mask ;

	if( MASKD.InitializeFlag == FALSE )
		return -1 ;

	if( MASKHCHK_ASYNC( MaskHandle, Mask ) )
		return -1 ;

	// �摜�̓ǂݍ��݂����݂�
	if( CreateGraphImageOrDIBGraph_WCHAR_T( FileName, NULL, 0, LOADIMAGE_TYPE_FILE, FALSE, FALSE, &BaseImage, NULL, NULL ) < 0 )
	{
		return -1 ;
	}

	// �ʏ�t�H�[�}�b�g�ł͂Ȃ�������ʏ�t�H�[�}�b�g�ɕϊ�
	if( BaseImage.ColorData.Format != DX_BASEIMAGE_FORMAT_NORMAL )
	{
		NS_ConvertNormalFormatBaseImage( &BaseImage ) ;
	}

	// �}�X�N�ۑ��p�T�[�t�F�X�̍쐬
	if( Mask_CreateSurface( &Mask->SrcData, &Mask->SrcDataPitch, BaseImage.Width, BaseImage.Height, &Mask->TransMode ) < 0 )
	{
		NS_ReleaseBaseImage( &BaseImage ) ;
		return -1 ;
	}

	// �f�[�^�̃Z�b�g
	Mask->MaskWidth  = BaseImage.Width ;
	Mask->MaskHeight = BaseImage.Height ;

	// �}�X�N�f�[�^��]��
	NS_GraphImageBltToMask( &BaseImage, 0, 0, MaskHandle ) ;

	// �摜�����
	NS_ReleaseBaseImage( &BaseImage ) ;

	// �I��
	return MaskHandle ;
}

#ifndef DX_NON_ASYNCLOAD
// LoadMask �̔񓯊��ǂݍ��݃X���b�h����Ă΂��֐�
static void Mask_LoadMask_ASync( ASYNCLOADDATA_COMMON *AParam )
{
	int MaskHandle ;
	const wchar_t *FileName ;
	int Addr ;
	int Result ;

	Addr = 0 ;
	MaskHandle = GetASyncLoadParamInt( AParam->Data, &Addr ) ;
	FileName = GetASyncLoadParamString( AParam->Data, &Addr ) ;

	Result = Mask_LoadMask_Static( MaskHandle, FileName, TRUE ) ;

	DecASyncLoadCount( MaskHandle ) ;
	if( Result < 0 )
	{
		SubHandle( MaskHandle ) ;
	}
}
#endif // DX_NON_ASYNCLOAD

// �}�X�N�f�[�^���摜�t�@�C������ǂݍ���
extern int Mask_LoadMask_UseGParam( const wchar_t *FileName, int ASyncLoadFlag )
{
	int MaskHandle ;

	if( MASKD.InitializeFlag == FALSE )
		return -1 ;

	CheckActiveState() ;

	// �n���h���̍쐬
	MaskHandle = AddHandle( DX_HANDLETYPE_GMASK, FALSE, -1 ) ;
	if( MaskHandle == -1 )
	{
		return -1 ;
	}

#ifndef DX_NON_ASYNCLOAD
	if( ASyncLoadFlag )
	{
		ASYNCLOADDATA_COMMON *AParam = NULL ;
		int Addr ;
		wchar_t FullPath[ 1024 ] ;

		ConvertFullPathW_( FileName, FullPath, sizeof( FullPath ) ) ;

		// �p�����[�^�ɕK�v�ȃ������̃T�C�Y���Z�o
		Addr = 0 ;
		AddASyncLoadParamInt( NULL, &Addr, MaskHandle ) ;
		AddASyncLoadParamString( NULL, &Addr, FullPath ) ;

		// �������̊m��
		AParam = AllocASyncLoadDataMemory( Addr ) ;
		if( AParam == NULL )
			goto ERR ;

		// �����ɕK�v�ȏ����Z�b�g
		AParam->ProcessFunction = Mask_LoadMask_ASync ;
		Addr = 0 ;
		AddASyncLoadParamInt( AParam->Data, &Addr, MaskHandle ) ;
		AddASyncLoadParamString( AParam->Data, &Addr, FullPath ) ;

		// �f�[�^��ǉ�
		if( AddASyncLoadData( AParam ) < 0 )
		{
			DXFREE( AParam ) ;
			AParam = NULL ;
			goto ERR ;
		}

		// �񓯊��ǂݍ��݃J�E���g���C���N�������g
		IncASyncLoadCount( MaskHandle, AParam->Index ) ;
	}
	else
#endif // DX_NON_ASYNCLOAD
	{
		if( Mask_LoadMask_Static( MaskHandle, FileName, FALSE ) < 0 )
			goto ERR ;
	}

	// �I��
	return MaskHandle ;

ERR :
	SubHandle( MaskHandle ) ;

	return -1 ;
}

// �}�X�N�f�[�^�����[�h����
extern int NS_LoadMask( const TCHAR *FileName )
{
#ifdef UNICODE
	return LoadMask_WCHAR_T(
		FileName
	) ;
#else
	int Result ;

	TCHAR_TO_WCHAR_T_STRING_ONE_BEGIN( FileName, return -1 )

	Result = LoadMask_WCHAR_T(
		UseFileNameBuffer
	) ;

	TCHAR_TO_WCHAR_T_STRING_END( FileName )

	return Result ;
#endif
}

// �}�X�N�f�[�^�����[�h����
extern int LoadMask_WCHAR_T( const wchar_t *FileName )
{
	return Mask_LoadMask_UseGParam( FileName, GetASyncLoadFlag() ) ;
}

// LoadDivMask �̎������֐�
static int Mask_LoadDivMask_Static(
	const wchar_t *FileName,
	int AllNum,
	int XNum,
	int YNum,
	int XSize,
	int YSize,
	int *HandleBuf,
	int /*ASyncThread*/
)
{
	MASKDATA * Mask ;
	BASEIMAGE BaseImage ;
	int i, j, k ;

	if( MASKD.InitializeFlag == FALSE )
		return -1 ;

	// �摜�̓ǂݍ��݂����݂�
	if( CreateGraphImageOrDIBGraph_WCHAR_T( FileName, NULL, 0, LOADIMAGE_TYPE_FILE, FALSE, FALSE, &BaseImage, NULL, NULL ) < 0 )
	{
		return -1 ;
	}

	// �T�C�Y�m�F
	if( XNum * XSize > BaseImage.Width || YNum * YSize > BaseImage.Height )
		goto ERR ;

	// �����]���J�n
	k = 0 ;
	for( i = 0 ; k != AllNum && i < YNum ; i ++ )
	{
		for( j = 0 ; k != AllNum && j < XNum ; j ++, k ++ )
		{
			if( MASKHCHK_ASYNC( HandleBuf[ k ], Mask ) )
				goto ERR ;

			// �}�X�N�̍쐬
			if( Mask_CreateSurface( &Mask->SrcData, &Mask->SrcDataPitch, XSize, YSize, &Mask->TransMode ) < 0 )
				goto ERR ;

			// �f�[�^�̃Z�b�g
			Mask->MaskWidth = XSize ;
			Mask->MaskHeight = YSize ;

			// �}�X�N�f�[�^�̓]��
			NS_GraphImageBltToMask( &BaseImage, XSize * j, YSize * i, HandleBuf[ k ] ) ;
		}
	}

	// �摜�����
	NS_ReleaseBaseImage( &BaseImage ) ;

	// �I��
	return 0 ;

	// �G���[�I��
ERR :
	// �摜�����
	NS_ReleaseBaseImage( &BaseImage ) ;
	
	return -1 ;
}

#ifndef DX_NON_ASYNCLOAD
// LoadDivMask �̔񓯊��ǂݍ��݃X���b�h����Ă΂��֐�
static void Mask_LoadDivMask_ASync( ASYNCLOADDATA_COMMON *AParam )
{
	const wchar_t *FileName ;
	int AllNum ;
	int XNum ;
	int YNum ;
	int XSize ;
	int YSize ;
	int *HandleBuf ;
	int Addr ;
	int i ;
	int Result ;

	Addr = 0 ;
	FileName = GetASyncLoadParamString( AParam->Data, &Addr ) ;
	AllNum = GetASyncLoadParamInt( AParam->Data, &Addr ) ;
	XNum = GetASyncLoadParamInt( AParam->Data, &Addr ) ;
	YNum = GetASyncLoadParamInt( AParam->Data, &Addr ) ;
	XSize = GetASyncLoadParamInt( AParam->Data, &Addr ) ;
	YSize = GetASyncLoadParamInt( AParam->Data, &Addr ) ;
	HandleBuf = ( int * )GetASyncLoadParamStruct( AParam->Data, &Addr ) ;

	Result = Mask_LoadDivMask_Static( FileName, AllNum, XNum, YNum, XSize, YSize, HandleBuf, TRUE ) ;

	for( i = 0 ; i < AllNum ; i ++ )
		DecASyncLoadCount( HandleBuf[ i ] ) ;

	if( Result < 0 )
	{
		for( i = 0 ; i < AllNum ; i ++ )
			NS_DeleteMask( HandleBuf[ i ] ) ;
	}
}
#endif // DX_NON_ASYNCLOAD

// �}�X�N���摜�t�@�C�����番���ǂ݂���
extern int Mask_LoadDivMask_UseGParam(
	const wchar_t *FileName,
	int AllNum,
	int XNum,
	int YNum,
	int XSize,
	int YSize,
	int *HandleBuf,
	int ASyncLoadFlag
)
{
	int i ;

	CheckActiveState() ;

	if( AllNum == 0 )
		return -1 ;

	// �O���t�B�b�N�n���h���̍쐬
	_MEMSET( HandleBuf, 0, sizeof( int ) * AllNum ) ;
	for( i = 0 ; i < AllNum ; i ++ )
	{
		HandleBuf[ i ] = AddHandle( DX_HANDLETYPE_GMASK, FALSE, -1 ) ;
		if( HandleBuf[ i ] < 0 )
		{
			goto ERR ;
		}
	}

#ifndef DX_NON_ASYNCLOAD
	if( ASyncLoadFlag )
	{
		ASYNCLOADDATA_COMMON *AParam = NULL ;
		int Addr ;
		wchar_t FullPath[ 1024 ] ;

		ConvertFullPathW_( FileName, FullPath, sizeof( FullPath ) ) ;

		// �p�����[�^�ɕK�v�ȃ������̃T�C�Y���Z�o
		Addr = 0 ;
		AddASyncLoadParamString( NULL, &Addr, FullPath ) ; 
		AddASyncLoadParamInt( NULL, &Addr, AllNum ) ;
		AddASyncLoadParamInt( NULL, &Addr, XNum ) ;
		AddASyncLoadParamInt( NULL, &Addr, YNum ) ;
		AddASyncLoadParamInt( NULL, &Addr, XSize ) ;
		AddASyncLoadParamInt( NULL, &Addr, YSize ) ;
		AddASyncLoadParamStruct( NULL, &Addr, HandleBuf, ( int )( sizeof( int ) * AllNum ) ) ;

		// �������̊m��
		AParam = AllocASyncLoadDataMemory( Addr ) ;
		if( AParam == NULL )
			goto ERR ;

		// �����ɕK�v�ȏ����Z�b�g
		AParam->ProcessFunction = Mask_LoadDivMask_ASync ;
		Addr = 0 ;
		AddASyncLoadParamString( AParam->Data, &Addr, FullPath ) ; 
		AddASyncLoadParamInt( AParam->Data, &Addr, AllNum ) ;
		AddASyncLoadParamInt( AParam->Data, &Addr, XNum ) ;
		AddASyncLoadParamInt( AParam->Data, &Addr, YNum ) ;
		AddASyncLoadParamInt( AParam->Data, &Addr, XSize ) ;
		AddASyncLoadParamInt( AParam->Data, &Addr, YSize ) ;
		AddASyncLoadParamStruct( AParam->Data, &Addr, HandleBuf, ( int )( sizeof( int ) * AllNum ) ) ;

		// �f�[�^��ǉ�
		if( AddASyncLoadData( AParam ) < 0 )
		{
			DXFREE( AParam ) ;
			AParam = NULL ;
			goto ERR ;
		}

		// �񓯊��ǂݍ��݃J�E���g���C���N�������g
		for( i = 0 ; i < AllNum ; i ++ )
			IncASyncLoadCount( HandleBuf[ i ], AParam->Index ) ;
	}
	else
#endif // DX_NON_ASYNCLOAD
	{
		if( Mask_LoadDivMask_Static( FileName, AllNum, XNum, YNum, XSize, YSize, HandleBuf, FALSE ) < 0 )
			goto ERR ;
	}

	// ����I��
	return 0 ;

ERR :
	for( i = 0 ; i < AllNum ; i ++ )
	{
		NS_DeleteMask( HandleBuf[ i ] ) ;
	}

	return -1 ;
}

// �}�X�N���摜���番���ǂ݂���
extern int NS_LoadDivMask( const TCHAR *FileName, int AllNum, int XNum, int YNum, int XSize, int YSize, int *HandleBuf )
{
#ifdef UNICODE
	return LoadDivMask_WCHAR_T(
		FileName, AllNum, XNum, YNum, XSize, YSize, HandleBuf
	) ;
#else
	int Result ;

	TCHAR_TO_WCHAR_T_STRING_ONE_BEGIN( FileName, return -1 )

	Result = LoadDivMask_WCHAR_T(
		UseFileNameBuffer, AllNum, XNum, YNum, XSize, YSize, HandleBuf
	) ;

	TCHAR_TO_WCHAR_T_STRING_END( FileName )

	return Result ;
#endif
}

// �}�X�N���摜���番���ǂ݂���
extern int LoadDivMask_WCHAR_T( const wchar_t *FileName, int AllNum, int XNum, int YNum, int XSize, int YSize, int *HandleBuf )
{
	return Mask_LoadDivMask_UseGParam( FileName, AllNum, XNum, YNum, XSize, YSize, HandleBuf, GetASyncLoadFlag() ) ;
}












// CreateMaskFromMem �̎������֐�
static int Mask_CreateMaskFromMem_Static(
	int         MaskHandle,
	const void *FileImage,
	int         FileImageSize,
	int       /*ASyncThread*/
)
{
	BASEIMAGE BaseImage ;
	MASKDATA * Mask ;

	if( MASKD.InitializeFlag == FALSE )
		return -1 ;

	if( MASKHCHK_ASYNC( MaskHandle, Mask ) )
		return -1 ;

	// ���[�h�����݂�
	if( NS_CreateBaseImage( NULL, FileImage, FileImageSize, LOADIMAGE_TYPE_MEM, &BaseImage, FALSE ) < 0 )
	{
		return -1 ;
	}
	
	// �}�X�N�ۑ��p�T�[�t�F�X�̍쐬
	if( Mask_CreateSurface( &Mask->SrcData, &Mask->SrcDataPitch, BaseImage.Width, BaseImage.Height, &Mask->TransMode ) < 0 )
	{
		NS_ReleaseBaseImage( &BaseImage ) ;
		return -1 ;
	}

	// �f�[�^�̃Z�b�g
	Mask->MaskWidth  = BaseImage.Width ;
	Mask->MaskHeight = BaseImage.Height ;

	// �}�X�N�f�[�^��]��
	NS_GraphImageBltToMask( &BaseImage, 0, 0, MaskHandle ) ;

	// BASEIMAGE �����
	NS_ReleaseBaseImage( &BaseImage ) ;

	// �I��
	return MaskHandle ;
}

#ifndef DX_NON_ASYNCLOAD
// CreateMaskFromMem �̔񓯊��ǂݍ��݃X���b�h����Ă΂��֐�
static void Mask_CreateMaskFromMem_ASync( ASYNCLOADDATA_COMMON *AParam )
{
	int         MaskHandle ;
	const void *FileImage ;
	int         FileImageSize ;
	int         Addr ;
	int         Result ;

	Addr = 0 ;
	MaskHandle    = GetASyncLoadParamInt(   AParam->Data, &Addr ) ;
	FileImage     = GetASyncLoadParamVoidP( AParam->Data, &Addr ) ;
	FileImageSize = GetASyncLoadParamInt(   AParam->Data, &Addr ) ;

	Result = Mask_CreateMaskFromMem_Static( MaskHandle, FileImage, FileImageSize, TRUE ) ;

	DecASyncLoadCount( MaskHandle ) ;
	if( Result < 0 )
	{
		SubHandle( MaskHandle ) ;
	}
}
#endif // DX_NON_ASYNCLOAD

// �}�X�N�f�[�^����������̉摜�t�@�C���C���[�W����ǂݍ���
extern int Mask_CreateMaskFromMem_UseGParam( const void *FileImage, int FileImageSize, int ASyncLoadFlag )
{
	int MaskHandle ;

	if( MASKD.InitializeFlag == FALSE )
		return -1 ;

	CheckActiveState() ;

	// �n���h���̍쐬
	MaskHandle = AddHandle( DX_HANDLETYPE_GMASK, FALSE, -1 ) ;
	if( MaskHandle == -1 )
	{
		return -1 ;
	}

#ifndef DX_NON_ASYNCLOAD
	if( ASyncLoadFlag )
	{
		ASYNCLOADDATA_COMMON *AParam = NULL ;
		int Addr ;

		// �p�����[�^�ɕK�v�ȃ������̃T�C�Y���Z�o
		Addr = 0 ;
		AddASyncLoadParamInt(        NULL, &Addr, MaskHandle ) ;
		AddASyncLoadParamConstVoidP( NULL, &Addr, FileImage ) ;
		AddASyncLoadParamInt(        NULL, &Addr, FileImageSize ) ;

		// �������̊m��
		AParam = AllocASyncLoadDataMemory( Addr ) ;
		if( AParam == NULL )
			goto ERR ;

		// �����ɕK�v�ȏ����Z�b�g
		AParam->ProcessFunction = Mask_CreateMaskFromMem_ASync ;
		Addr = 0 ;
		AddASyncLoadParamInt(        AParam->Data, &Addr, MaskHandle ) ;
		AddASyncLoadParamConstVoidP( AParam->Data, &Addr, FileImage ) ;
		AddASyncLoadParamInt(        AParam->Data, &Addr, FileImageSize ) ;

		// �f�[�^��ǉ�
		if( AddASyncLoadData( AParam ) < 0 )
		{
			DXFREE( AParam ) ;
			AParam = NULL ;
			goto ERR ;
		}

		// �񓯊��ǂݍ��݃J�E���g���C���N�������g
		IncASyncLoadCount( MaskHandle, AParam->Index ) ;
	}
	else
#endif // DX_NON_ASYNCLOAD
	{
		if( Mask_CreateMaskFromMem_Static( MaskHandle, FileImage, FileImageSize, FALSE ) < 0 )
			goto ERR ;
	}

	// �I��
	return MaskHandle ;

ERR :
	SubHandle( MaskHandle ) ;

	return -1 ;
}

// ��������ɂ���摜�t�@�C���C���[�W��ǂݍ��݃}�X�N�n���h�����쐬����
extern int NS_CreateMaskFromMem( const void *FileImage, int FileImageSize )
{
	return Mask_CreateMaskFromMem_UseGParam( FileImage, FileImageSize, GetASyncLoadFlag() ) ;
}












// CreateDivMaskFromMem �̎������֐�
static int Mask_CreateDivMaskFromMem_Static(
	const void *FileImage,
	int FileImageSize,
	int AllNum,
	int XNum,
	int YNum,
	int XSize,
	int YSize,
	int *HandleBuf,
	int /*ASyncThread*/
)
{
	BASEIMAGE BaseImage ;
	MASKDATA * Mask ;
	int i ;
	int j ;
	int k ;

	if( MASKD.InitializeFlag == FALSE )
		return -1 ;

	// ���[�h�����݂�
	if( NS_CreateBaseImage( NULL, FileImage, FileImageSize, LOADIMAGE_TYPE_MEM, &BaseImage, FALSE ) < 0 )
	{
		return -1 ;
	}

	// �T�C�Y�m�F
	if( XNum * XSize > BaseImage.Width || YNum * YSize > BaseImage.Height )
		goto ERR ;

	// �����]���J�n
	k = 0 ;
	for( i = 0 ; k != AllNum && i < YNum ; i ++ )
	{
		for( j = 0 ; k != AllNum && j < XNum ; j ++, k ++ )
		{
			if( MASKHCHK_ASYNC( HandleBuf[ k ], Mask ) )
				goto ERR ;

			// �}�X�N�̍쐬
			if( Mask_CreateSurface( &Mask->SrcData, &Mask->SrcDataPitch, XSize, YSize, &Mask->TransMode ) < 0 )
				goto ERR ;

			// �f�[�^�̃Z�b�g
			Mask->MaskWidth = XSize ;
			Mask->MaskHeight = YSize ;

			// �}�X�N�f�[�^�̓]��
			NS_GraphImageBltToMask( &BaseImage, XSize * j, YSize * i, HandleBuf[ k ] ) ;
		}
	}

	// BASEIMAGE �����
	NS_ReleaseBaseImage( &BaseImage ) ;

	// �I��
	return 0 ;

	// �G���[�I��
ERR :
	// BASEIMAGE �����
	NS_ReleaseBaseImage( &BaseImage ) ;
	
	return -1 ;
}

#ifndef DX_NON_ASYNCLOAD
// CreateDivMaskFromMem �̔񓯊��ǂݍ��݃X���b�h����Ă΂��֐�
static void Mask_CreateDivMaskFromMem_ASync( ASYNCLOADDATA_COMMON *AParam )
{
	const void *FileImage ;
	int         FileImageSize ;
	int         AllNum ;
	int         XNum ;
	int         YNum ;
	int         XSize ;
	int         YSize ;
	int        *HandleBuf ;
	int         Addr ;
	int         i ;
	int         Result ;

	Addr = 0 ;
	FileImage     =          GetASyncLoadParamVoidP(  AParam->Data, &Addr ) ;
	FileImageSize =          GetASyncLoadParamInt(    AParam->Data, &Addr ) ;
	AllNum        =          GetASyncLoadParamInt(    AParam->Data, &Addr ) ;
	XNum          =          GetASyncLoadParamInt(    AParam->Data, &Addr ) ;
	YNum          =          GetASyncLoadParamInt(    AParam->Data, &Addr ) ;
	XSize         =          GetASyncLoadParamInt(    AParam->Data, &Addr ) ;
	YSize         =          GetASyncLoadParamInt(    AParam->Data, &Addr ) ;
	HandleBuf     = ( int * )GetASyncLoadParamStruct( AParam->Data, &Addr ) ;

	Result = Mask_CreateDivMaskFromMem_Static( FileImage, FileImageSize, AllNum, XNum, YNum, XSize, YSize, HandleBuf, TRUE ) ;

	for( i = 0 ; i < AllNum ; i ++ )
		DecASyncLoadCount( HandleBuf[ i ] ) ;

	if( Result < 0 )
	{
		for( i = 0 ; i < AllNum ; i ++ )
			NS_DeleteMask( HandleBuf[ i ] ) ;
	}
}
#endif // DX_NON_ASYNCLOAD

// �}�X�N����������̉摜�t�@�C���C���[�W���番���ǂ݂���
extern int Mask_CreateDivMaskFromMem_UseGParam(
	const void *FileImage,
	int FileImageSize,
	int AllNum,
	int XNum,
	int YNum,
	int XSize,
	int YSize,
	int *HandleBuf,
	int ASyncLoadFlag
)
{
	int i ;

	CheckActiveState() ;

	if( AllNum == 0 )
		return -1 ;

	// �O���t�B�b�N�n���h���̍쐬
	_MEMSET( HandleBuf, 0, sizeof( int ) * AllNum ) ;
	for( i = 0 ; i < AllNum ; i ++ )
	{
		HandleBuf[ i ] = AddHandle( DX_HANDLETYPE_GMASK, FALSE, -1 ) ;
		if( HandleBuf[ i ] < 0 )
		{
			goto ERR ;
		}
	}

#ifndef DX_NON_ASYNCLOAD
	if( ASyncLoadFlag )
	{
		ASYNCLOADDATA_COMMON *AParam = NULL ;
		int Addr ;

		// �p�����[�^�ɕK�v�ȃ������̃T�C�Y���Z�o
		Addr = 0 ;
		AddASyncLoadParamConstVoidP( NULL, &Addr, FileImage ) ;
		AddASyncLoadParamInt(        NULL, &Addr, FileImageSize ) ;
		AddASyncLoadParamInt(        NULL, &Addr, AllNum ) ;
		AddASyncLoadParamInt(        NULL, &Addr, XNum ) ;
		AddASyncLoadParamInt(        NULL, &Addr, YNum ) ;
		AddASyncLoadParamInt(        NULL, &Addr, XSize ) ;
		AddASyncLoadParamInt(        NULL, &Addr, YSize ) ;
		AddASyncLoadParamStruct(     NULL, &Addr, HandleBuf, ( int )( sizeof( int ) * AllNum ) ) ;

		// �������̊m��
		AParam = AllocASyncLoadDataMemory( Addr ) ;
		if( AParam == NULL )
			goto ERR ;

		// �����ɕK�v�ȏ����Z�b�g
		AParam->ProcessFunction = Mask_CreateDivMaskFromMem_ASync ;
		Addr = 0 ;
		AddASyncLoadParamConstVoidP( AParam->Data, &Addr, FileImage ) ;
		AddASyncLoadParamInt(        AParam->Data, &Addr, FileImageSize ) ;
		AddASyncLoadParamInt(        AParam->Data, &Addr, AllNum ) ;
		AddASyncLoadParamInt(        AParam->Data, &Addr, XNum ) ;
		AddASyncLoadParamInt(        AParam->Data, &Addr, YNum ) ;
		AddASyncLoadParamInt(        AParam->Data, &Addr, XSize ) ;
		AddASyncLoadParamInt(        AParam->Data, &Addr, YSize ) ;
		AddASyncLoadParamStruct(     AParam->Data, &Addr, HandleBuf, ( int )( sizeof( int ) * AllNum ) ) ;

		// �f�[�^��ǉ�
		if( AddASyncLoadData( AParam ) < 0 )
		{
			DXFREE( AParam ) ;
			AParam = NULL ;
			goto ERR ;
		}

		// �񓯊��ǂݍ��݃J�E���g���C���N�������g
		for( i = 0 ; i < AllNum ; i ++ )
			IncASyncLoadCount( HandleBuf[ i ], AParam->Index ) ;
	}
	else
#endif // DX_NON_ASYNCLOAD
	{
		if( Mask_CreateDivMaskFromMem_Static( FileImage, FileImageSize, AllNum, XNum, YNum, XSize, YSize, HandleBuf, FALSE ) < 0 )
			goto ERR ;
	}

	// ����I��
	return 0 ;

ERR :
	for( i = 0 ; i < AllNum ; i ++ )
	{
		NS_DeleteMask( HandleBuf[ i ] ) ;
	}

	return -1 ;
}

// ��������ɂ���摜�t�@�C���C���[�W�𕪊��ǂݍ��݂��ă}�X�N�n���h�����쐬����
extern int NS_CreateDivMaskFromMem( const void *FileImage, int FileImageSize, int AllNum, int XNum, int YNum, int XSize, int YSize, int *HandleBuf )
{
	return Mask_CreateDivMaskFromMem_UseGParam( FileImage, FileImageSize, AllNum, XNum, YNum, XSize, YSize, HandleBuf, GetASyncLoadFlag() ) ;
}












#ifndef DX_NON_FONT

// �����w�肠��̕�������}�X�N�X�N���[���ɕ`�悷��
extern int NS_DrawFormatStringMask( int x, int y, int Flag, const TCHAR *FormatString, ... )
{
	va_list VaList ;
	TCHAR String[ 2048 ] ;
	
	if( MASKD.InitializeFlag == FALSE )
		return -1 ;

	// �ҏW��̕�������擾����
	{
		va_start( VaList, FormatString ) ;
		_TVSNPRINTF( String, sizeof( String ) / sizeof( TCHAR ), FormatString, VaList ) ;
		va_end( VaList ) ;
	}

	return NS_DrawStringMaskToHandle( x, y, Flag, NS_GetDefaultFontHandle(), String ) ;
}

// �����w�肠��̕�������}�X�N�X�N���[���ɕ`�悷��
extern int DrawFormatStringMask_WCHAR_T( int x, int y, int Flag, const wchar_t *FormatString, ... )
{
	va_list VaList ;
	wchar_t String[ 1024 ] ;
	
	if( MASKD.InitializeFlag == FALSE )
		return -1 ;

	// �ҏW��̕�������擾����
	{
		va_start( VaList, FormatString ) ;
		_VSWNPRINTF( String, sizeof( String ) / 2, FormatString, VaList ) ;
		va_end( VaList ) ;
	}

	return DrawStringMaskToHandle_WCHAR_T( x, y, Flag, NS_GetDefaultFontHandle(), String ) ;
}

// �����w�肠��̕�������}�X�N�X�N���[���ɕ`�悷��
extern int NS_DrawFormatStringMaskToHandle( int x, int y, int Flag, int FontHandle, const TCHAR *FormatString, ... )
{
	va_list VaList ;
	TCHAR String[ 2048 ] ;
	
	if( MASKD.InitializeFlag == FALSE )
		return -1 ;

	// �ҏW��̕�������擾����
	{
		va_start( VaList, FormatString ) ;
		_TVSNPRINTF( String, sizeof( String ) / sizeof( TCHAR ), FormatString, VaList ) ;
		va_end( VaList ) ;
	}

	return NS_DrawStringMaskToHandle( x, y, Flag, FontHandle, String ) ;
}

// �����w�肠��̕�������}�X�N�X�N���[���ɕ`�悷��
extern int DrawFormatStringMaskToHandle_WCHAR_T( int x, int y, int Flag, int FontHandle, const wchar_t *FormatString, ... )
{
	va_list VaList ;
	wchar_t String[ 2048 ] ;
	
	if( MASKD.InitializeFlag == FALSE )
		return -1 ;

	// �ҏW��̕�������擾����
	{
		va_start( VaList, FormatString ) ;
		_VSWNPRINTF( String, sizeof( String ) / 2, FormatString, VaList ) ;
		va_end( VaList ) ;
	}

	return DrawStringMaskToHandle_WCHAR_T( x, y, Flag, FontHandle, String ) ;
}

// ��������}�X�N�X�N���[���ɕ`�悷��(�t�H���g�n���h���w���)
extern int NS_DrawStringMaskToHandle( int x, int y, int Flag, int FontHandle, const TCHAR *String )
{
#ifdef UNICODE
	return DrawStringMaskToHandle_WCHAR_T(
		x, y, Flag, FontHandle, String
	) ;
#else
	int Result ;

	TCHAR_TO_WCHAR_T_STRING_ONE_BEGIN( String, return -1 )

	Result = DrawStringMaskToHandle_WCHAR_T(
		x, y, Flag, FontHandle, UseStringBuffer
	) ;

	TCHAR_TO_WCHAR_T_STRING_END( String )

	return Result ;
#endif
}

// ��������}�X�N�X�N���[���ɕ`�悷��(�t�H���g�n���h���w���)
extern int DrawStringMaskToHandle_WCHAR_T( int x, int y, int Flag, int FontHandle, const wchar_t *String )
{
	BASEIMAGE BaseImage ;
	unsigned int Color ;
	int Result ;
	SIZE DrawSize ;

	if( MASKD.InitializeFlag == FALSE )
		return -1 ;

	// �}�X�N�X�N���[�����쐬����Ă��Ȃ�������G���[
	if( MASKD.MaskBuffer == NULL )
		return -1 ;

	RefreshDefaultFont() ;

	// BASEIMAGE�f�[�^�̍쐬
	NS_CreatePaletteColorData( &BaseImage.ColorData ) ;
	NS_GetDrawScreenSize( &BaseImage.Width, &BaseImage.Height ) ;
	BaseImage.Pitch = MASKD.MaskBufferPitch ;
	BaseImage.GraphData = MASKD.MaskBuffer ;

	// �}�X�N�X�N���[����ɕ�����`��
	Color = ( DWORD )( Flag == 0 ? 0 : 0xff ) ;
	Result = FontCacheStringDrawToHandle_WCHAR_T(
		x, y, String, Color, Color, 
		&BaseImage, &GSYS.DrawSetting.DrawArea, FontHandle,
		FALSE, &DrawSize
	) ;

	RECT Rect ;

	if( GSYS.Setting.ValidHardware )
	{
		// �X�V
		Rect.left   = x ;
		Rect.top    = y ;
		Rect.right  = x + DrawSize.cx ;
		Rect.bottom = y + DrawSize.cy ;
		Mask_UpdateMaskImageTexture_PF( &Rect ) ;
	}

	return Result ;
}

// ��������}�X�N�X�N���[���ɕ`�悷��
extern int NS_DrawStringMask( int x, int y, int Flag, const TCHAR *String )
{
	return NS_DrawStringMaskToHandle( x, y, Flag, NS_GetDefaultFontHandle(), String ) ;
}

// ��������}�X�N�X�N���[���ɕ`�悷��
extern int DrawStringMask_WCHAR_T( int x, int y, int Flag, const wchar_t *String )
{
	return DrawStringMaskToHandle_WCHAR_T( x, y, Flag, NS_GetDefaultFontHandle(), String ) ;
}

#endif // DX_NON_FONT

// �}�X�N�X�N���[�����g�p�����t���O�̎擾
extern int NS_GetMaskUseFlag( void )
{
	return MASKD.MaskUseFlag ;
}

// �}�X�N�̐��l�ɑ΂�����ʂ��t�]������
extern int NS_SetMaskReverseEffectFlag( int ReverseFlag )
{
	MASKD.MaskReverseEffectFlag = ReverseFlag ;

	// �I��
	return 0 ;
}

// �}�X�N�g�p���[�h�̎擾
extern int NS_GetUseMaskScreenFlag( void )
{
	return MASKD.MaskUseFlag ;
}

// �}�X�N�X�N���[���𕜋�������
extern int Mask_ReCreateSurface( void )
{
	if( MASKD.InitializeFlag == FALSE )
		return -1 ;

	// ���C���̃}�X�N�T�[�t�F�X���쐬����
	Mask_CreateScreenFunction( MASKD.CreateMaskFlag, GSYS.DrawSetting.DrawSizeX, GSYS.DrawSetting.DrawSizeY ) ;

	// �����}�X�N�g�p�t���O�������Ă�����}�X�N��L���ɂ����Ƃ��s��
	if( MASKD.MaskUseFlag )
		NS_SetUseMaskScreenFlag( TRUE ) ;

	// �I��
	return 0 ;
}


// �}�X�N�p�^�[���̓W�J
extern int DrawMaskToDirectData_Base( int DrawPointX, int DrawPointY, void *DestBufP, int DestPitch, 
											int SrcWidth, int SrcHeight, const void *SrcData, int TransMode  )
{
	BYTE *DestP ,*SrcP ;
	BYTE TransColor ;
	int DestAddPoint ;
	int SrcAddPoint ;
	int BltWidth, BltHeight ;
	RECT Rect ;

	if( MASKD.InitializeFlag == FALSE )
		return -1 ;

	if( MASKD.MaskBuffer == NULL ) return 0 ;

	SETRECT( Rect, DrawPointX, DrawPointY, DrawPointX + SrcWidth, DrawPointY + SrcHeight ) ;
	RectClipping_Inline( &Rect, &GSYS.DrawSetting.DrawArea ) ;
	if( Rect.left == Rect.right || Rect.top == Rect.bottom ) return 0 ;
	Rect.left -= DrawPointX ;
	Rect.right -= DrawPointX ;
	Rect.top -= DrawPointY ;
	Rect.bottom -= DrawPointY ;
	if( Rect.left > 0 ) DrawPointX += Rect.left ;
	if( Rect.top > 0 ) DrawPointY += Rect.top ;

	// �]������
	BltWidth = Rect.right - Rect.left ;
	BltHeight = Rect.bottom - Rect.top ;
	SrcP = ( BYTE * )SrcData + Rect.left + Rect.top * SrcWidth ;
	SrcAddPoint = SrcWidth - BltWidth ;

	DestP = ( BYTE * )DestBufP + DrawPointX + DrawPointY * DestPitch ;
	DestAddPoint = DestPitch - BltWidth ;

	TransColor = ( BYTE )( TransMode == DX_MASKTRANS_WHITE ? 0xff : 0 ) ;

	// ���ߐF�����邩�ǂ����ŏ����𕪊�
	if( TransMode == DX_MASKTRANS_NONE )
	{
#ifdef DX_NON_INLINE_ASM
		int i ;
		do
		{
			i = BltWidth ;

			do
			{
				*DestP = *SrcP ;
				DestP ++ ;
				SrcP ++ ;
			}while( -- i != 0 ) ;

			DestP += DestAddPoint ;
			SrcP += SrcAddPoint ;

		}while( -- BltHeight != 0 ) ;
#else
		// ���ߐF�Ȃ��o�[�W����
		__asm{
			CLD
			MOV		EDI, DestP
			MOV		ESI, SrcP
			MOV		EDX, BltWidth
			MOV		EBX, BltHeight
			MOV		EAX, SrcAddPoint
			MOV		ECX, DestAddPoint
			PUSH	EBP
			MOV		EBP, ECX
		LOOP811:
			MOV		ECX, EDX
			REP		MOVSB
			ADD		EDI, EBP
			ADD		ESI, EAX
			DEC		EBX
			JNZ		LOOP811
			POP		EBP
		}
#endif
	}
	else
	{
		// ���ߐF����o�[�W����
		
#ifdef DX_NON_INLINE_ASM
		int i ;
		TransColor &= 0xff ;
		do
		{
			i = BltWidth ;

			do
			{
				if( *SrcP != TransColor )
					*DestP = *SrcP ;
				DestP ++ ;
				SrcP ++ ;
			}while( -- i != 0 ) ;

			DestP += DestAddPoint ;
			SrcP += SrcAddPoint ;

		}while( -- BltHeight != 0 ) ;
#else
		__asm{
			MOV		EDI, DestP
			MOV		ESI, SrcP
			MOV		BL, TransColor
	LOOP821:
			MOV		ECX, BltWidth
	LOOP822:
			MOV		AL, [ESI]
			TEST	AL, BL
			JZ		R821
			MOV		[EDI], AL
	R821:
			INC		EDI
			INC		ESI
			LOOP	LOOP822

			ADD		EDI, DestAddPoint
			ADD		ESI, SrcAddPoint
			DEC		BltHeight
			JNZ		LOOP821
		}
#endif
	}

	// �I��
	return 0 ;
}

// �}�X�N�̃f�[�^�𒼐ڃ}�X�N��ʑS�̂ɕ`�悷��
extern int NS_DrawFillMaskToDirectData( int x1, int y1, int x2, int y2,  int Width, int Height, const void *MaskData )
{
	int MaskHandle ;

	if( MASKD.InitializeFlag == FALSE )
		return -1 ;

	CheckActiveState() ;

	// ��̃}�X�N���쐬
	if( ( MaskHandle = NS_MakeMask( Width, Height ) ) == -1 ) return -1 ;

	// �}�X�N�Ƀf�[�^���Z�b�g
	NS_SetDataToMask( Width, Height, MaskData, MaskHandle ) ;

	// �}�X�N����ʑS�̂ɕ`��
	NS_DrawFillMask( x1, y1, x2, y2, MaskHandle ) ;

	// �}�X�N�n���h�����폜
	NS_DeleteMask( MaskHandle ) ;

	// �I��
	return 0 ;
}

// �}�X�N�̃f�[�^���}�X�N�ɓ]������
extern int NS_SetDataToMask( int Width, int Height, const void *MaskData, int MaskHandle )
{
	MASKDATA * Mask ;

	if( MASKD.InitializeFlag == FALSE )
		return -1 ;

	if( MASKHCHK( MaskHandle, Mask ) )
		return -1 ;
	
	if( Mask->MaskWidth != Width || Mask->MaskHeight != Height ) return -1 ;

	CheckActiveState() ;

	// �}�X�N�f�[�^���Ȃ������ꍇ�͏������I��
	if( MaskData == NULL ) return 0 ;

	// �}�X�N�O���t�B�b�N�Ƀ}�X�N�f�[�^��W�J
	DrawMaskToDirectData_Base( 0, 0, Mask->SrcData, Mask->SrcDataPitch,
							Width, Height, MaskData, DX_MASKTRANS_NONE ) ;

	// �I��
	return 0 ;
}



// �}�X�N��ʏ�̕`���Ԃ��擾����
extern int NS_GetMaskScreenData( int x1, int y1, int x2, int y2, int MaskHandle )
{
	MASKDATA * Mask ; 
	RECT Rect, Rect2 ;

	if( MASKD.InitializeFlag == FALSE )
		return -1 ;

	if( MASKHCHK( MaskHandle, Mask ) )
		return -1 ;

	if( MASKD.MaskBuffer == NULL )
		return 0 ;

	// �擾��`�̃Z�b�g
	SETRECT( Rect, x1, y1, x2, y2 ) ;
	NS_GetDrawScreenSize( ( int * )&Rect2.right, ( int * )&Rect2.bottom ) ;
	Rect2.left = 0 ; Rect2.top = 0 ;
	RectClipping_Inline( &Rect, &Rect2 ) ;

	if( Rect.left == Rect.right || Rect.top == Rect.bottom ) return -1 ;
	if( Rect.right - Rect.left != Mask->MaskWidth ||
		Rect.bottom - Rect.top != Mask->MaskHeight ) return -1 ;

	// �f�[�^�̓]��
	{
		BYTE *Dest, *Src ;
		int DestAddPitch, SrcAddPitch ;
		int DestWidth, DestHeight ;

		Rect.left -= x1 ;
		Rect.right -= x1 ;
		Rect.top -= y1 ;
		Rect.bottom -= y1 ;
		if( Rect.left > 0 ){ x1 += Rect.left ; }
		if( Rect.top  > 0 ){ y1 += Rect.top  ; }

		DestWidth = Rect.right - Rect.left ;
		DestHeight = Rect.bottom - Rect.top ;
		Dest = Mask->SrcData + Rect.left + ( Rect.top * Mask->SrcDataPitch ) ;
		Src = MASKD.MaskBuffer + x1 + ( y1 * MASKD.MaskBufferPitch ) ;

		SrcAddPitch = Mask->SrcDataPitch - DestWidth ;
		DestAddPitch = MASKD.MaskBufferPitch - DestWidth ;

#ifdef DX_NON_INLINE_ASM
		int i ;
		do
		{
			i = DestWidth ;
			do
			{
				*Dest = *Src ;
				Dest ++ ;
				Src ++ ;
			}while( -- i != 0 ) ;

			Dest += DestAddPitch ;
			Src += SrcAddPitch ;

		}while( -- DestHeight != 0 ) ;
#else
		__asm{
			CLD
			MOV		EDI, Dest
			MOV		ESI, Src
			MOV		EDX, DestWidth
			MOV		EBX, DestHeight
			MOV		EAX, SrcAddPitch
			MOV		ECX, DestAddPitch
			PUSH	EBP
			MOV		EBP, ECX
		LOOP1_b:
			MOV		ECX, EDX
			REP		MOVSB
			ADD		EDI, EBP
			ADD		ESI, EAX
			DEC		EBX
			JNZ		LOOP1_b
			POP		EBP
		}
#endif
	}

	// �I��
	return 0 ;
}

// �}�X�N�X�N���[�����폜����
extern int NS_DeleteMaskScreen( void )
{
	if( MASKD.InitializeFlag == FALSE )
		return -1 ;

//	CheckActiveState() ;

	// �����}�X�N�g�p�t���O�������Ă�����}�X�N�𖳌��ɂ����Ƃ��s��
	if( MASKD.MaskUseFlag )
		NS_SetUseMaskScreenFlag( FALSE ) ;

	// �}�X�N�p�T�[�t�F�X���폜����
	Mask_ReleaseSurface() ;

	// �}�X�N�T�[�t�F�X���쐬����Ă��邩�t���O��|��
	MASKD.CreateMaskFlag = FALSE ;
	MASKD.MaskValidFlag = MASKD.MaskUseFlag && MASKD.CreateMaskFlag ;

	// �I��
	return 0 ;
}














// �}�X�N�X�N���[�����쐬����֐�
extern int Mask_CreateScreenFunction( int MaskSurfaceFlag, int Width, int Height )
{
	bool   OldEnable = false ;
	BYTE  *MaskBufferOld = NULL ;
	int    MaskBufferPitchOld = 0 ;
	int    MaskBufferSizeXOld = 0 ;
	int    MaskBufferSizeYOld = 0 ;
	MEMIMG MaskDrawMemImgOld ;

	if( MaskSurfaceFlag == FALSE ) return 0 ;

	// �T�C�Y�̒���
	if( MASKD.MaskBuffer != NULL )
	{
		// �T�C�Y�͑傫���ق��ɍ��킹��
		if( Width  < MASKD.MaskBufferSizeX ) Width  = MASKD.MaskBufferSizeX ;
		if( Height < MASKD.MaskBufferSizeY ) Height = MASKD.MaskBufferSizeY ;
	}

	// ���Ƀ}�X�N�o�b�t�@�����݂��āA�����܂ł����傫�ȃT�C�Y�̃o�b�t�@���w�肳�ꂽ�ꍇ�̓o�b�t�@��傫����蒼��
	if( MASKD.MaskBuffer != NULL && ( Width > MASKD.MaskBufferSizeX || Height > MASKD.MaskBufferSizeY ) )
	{
		OldEnable = true ;

		MaskBufferOld      = MASKD.MaskBuffer ;
		MASKD.MaskBuffer   = NULL ;
		MaskBufferPitchOld = MASKD.MaskBufferPitch ;

		MaskBufferSizeXOld = MASKD.MaskBufferSizeX ;
		MaskBufferSizeYOld = MASKD.MaskBufferSizeY ;

		MaskDrawMemImgOld  = MASKD.MaskDrawMemImg ;
		_MEMSET( &MASKD.MaskDrawMemImg, 0, sizeof( MEMIMG ) ) ;

		// ���ˑ�����
		Mask_CreateScreenFunction_Timing0_PF() ;
	}

	MASKD.MaskBufferSizeX = Width ;
	MASKD.MaskBufferSizeY = Height ;

	// �}�X�N�ۑ��p�o�b�t�@�̍쐬
	if( MASKD.MaskBuffer == NULL )
	{
		// �}�X�N�o�b�t�@�̃s�b�`���Z�o( 4�̔{���ɂ��� )
		MASKD.MaskBufferPitch = ( Width + 3 ) / 4 * 4 ;

		// �������̊m��
		MASKD.MaskBuffer = ( BYTE * )DXALLOC( ( size_t )( MASKD.MaskBufferPitch * Height ) ) ;

		// �}�X�N�̃N���A
		_MEMSET( MASKD.MaskBuffer, 0, ( size_t )( MASKD.MaskBufferPitch * Height ) ) ;
	}

	// �n�[�h�E�G�A�̋@�\���g�p����ꍇ�̓e�N�X�`�����쐬����
	if( GSYS.Setting.ValidHardware == TRUE )
	{
		if( Mask_CreateScreenFunction_Timing1_PF( Width, Height ) < 0 )
		{
			return -1 ;
		}
	}
	else
	{
		// �`���p�T�[�t�F�X�̍쐬
		if( CheckValidMemImg( &MASKD.MaskDrawMemImg ) == FALSE )
		{
			// MEMIMG ���g�p����ꍇ�͉�ʂƓ����傫���� MEMIMG ���쐬����
			if( MakeMemImgScreen( &MASKD.MaskDrawMemImg, Width, Height, -1 ) < 0 )
				return DxLib_ErrorUTF16LE( "\xde\x30\xb9\x30\xaf\x30\x28\x75\x20\x00\x4d\x00\x45\x00\x4d\x00\x49\x00\x4d\x00\x47\x00\x20\x00\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x00"/*@ L"�}�X�N�p MEMIMG �̍쐬�Ɏ��s���܂���" @*/ ) ;

			MASKD.ValidMaskDrawMemImg = TRUE ;
		}
	}

	// ��蒼�����ꍇ�͈ȑO�̓��e���R�s�[����
	if( OldEnable )
	{
		int i ;

		// �}�X�N�o�b�t�@�̓��e���R�s�[
		for( i = 0 ; i < MaskBufferSizeYOld ; i ++ )
		{
			_MEMCPY( MASKD.MaskBuffer + MASKD.MaskBufferPitch * i, MaskBufferOld + MaskBufferPitchOld * i, ( size_t )MaskBufferSizeXOld ) ;
		}
		DXFREE( MaskBufferOld ) ;
		MaskBufferOld = NULL ;

		// �n�[�h�E�G�A�̋@�\���g�p����ꍇ�̓e�N�X�`�����쐬����
		if( GSYS.Setting.ValidHardware == TRUE )
		{
			if( Mask_CreateScreenFunction_Timing2_PF( MaskBufferSizeXOld, MaskBufferSizeYOld ) < 0 )
			{
				return -1 ;
			}
		}
		else
		{
			// �Â��`���pMEMIMG �̓��e��V���� MEMIMG �ɃR�s�[����
			DrawMemImg( &MASKD.MaskDrawMemImg, &MaskDrawMemImgOld, 0, 0, FALSE, NULL ) ;
			TerminateMemImg( &MaskDrawMemImgOld ) ;
		}
	}

	// �I��
	return 0 ;
}

// �}�X�N�X�N���[�����ꎞ�폜����
extern int Mask_ReleaseSurface( void )
{
	// �}�X�N�ۑ��p�o�b�t�@�̉��
	if( MASKD.MaskBuffer != NULL )
	{
		DXFREE( MASKD.MaskBuffer ) ;
		MASKD.MaskBuffer = NULL ;
	}

	// �}�X�N�`��p�� MEMIMG �����
	if( MASKD.ValidMaskDrawMemImg == TRUE )
	{
		TerminateMemImg( &MASKD.MaskDrawMemImg ) ;
		MASKD.ValidMaskDrawMemImg = FALSE ;
	}

	// ���ˑ�����
	Mask_ReleaseSurface_PF() ;

	// �I��
	return 0 ;
}

// �}�X�N�g�p���[�h��ύX
extern int NS_SetUseMaskScreenFlag( int ValidFlag )
{
	IMAGEDATA *Image ;

	if( MASKD.InitializeFlag == FALSE )
		return -1 ;

	// �}�X�N�X�N���[�����Ȃ��ꍇ�͂����ŏ������I��
	if( MASKD.MaskBuffer == NULL ) return -1 ;

	// �t���X�N���[���}�X�N�������s���Ă���ꍇ�̓}�X�N�`��̌��ʂ𔽉f������
	MASK_FULLSCREEN_MASKUPDATE

	// ���܂łƃt���O�������ꍇ�͖���
//	if( ValidFlag == MASKD.MaskUseFlag ) return 0 ;

	// �}�X�N���g�p���邩�̃t���O���Z�b�g
	MASKD.MaskUseFlag = ValidFlag ;
	MASKD.MaskValidFlag = MASKD.MaskUseFlag && MASKD.CreateMaskFlag ;

	// �n�[�h�E�G�A�A�N�Z�����[�^���L�����ǂ����ŏ����𕪊�
	if( GSYS.Setting.ValidHardware )
	{
		// �n�[�h�E�G�A�A�N�Z�����[�^���L���ȏꍇ

		// ���ˑ�����
		Mask_SetUseMaskScreenFlag_PF() ;
	}
	else
	{
		// �\�t�g�E�G�A�����_�����O�̏ꍇ

		// �`���̌���
		if( MASKD.MaskValidFlag )
		{
			// �}�X�N���L���ȏꍇ�̓}�X�N��`���ɂ���
			GSYS.SoftRender.TargetMemImg = &MASKD.MaskDrawMemImg ;
		}
		else
		{
			// �`���̉摜�f�[�^�A�h���X���擾����
			Image = Graphics_Image_GetData( GSYS.DrawSetting.TargetScreen[ 0 ] ) ;

			// ����ȊO�̏ꍇ�͕`��\�摜�����C���o�b�t�@
			GSYS.SoftRender.TargetMemImg = Image ? &Image->Soft.MemImg : &GSYS.SoftRender.MainBufferMemImg ;
		}
	}

	// �I��
	return 0 ;
}

// �}�X�N���g�p�����`��̑O�ɌĂԊ֐�
// ( ���łɃT�u�o�b�t�@���g�p�����`��G���A�@�\���g�p���Ă���ꍇ�̏���������Ă��܂��Ă���� )
extern int Mask_DrawBeginFunction( RECT Rect )
{
	// �t���X�N���[���}�X�N�������s���Ă���ꍇ�̓}�X�N�`��̌��ʂ𔽉f������
	MASK_FULLSCREEN_MASKUPDATE

	MASKD.MaskBeginFunctionCount ++ ;
	if( MASKD.MaskBeginFunctionCount == 1 )
	{
		// �`��G���A�ŃN���b�s���O
		RectClipping_Inline( &Rect, &GSYS.DrawSetting.DrawArea ) ;

		// �n�[�h�E�G�A���g�p���邩�ǂ����ŏ����𕪊�
		if( GSYS.Setting.ValidHardware )
		{
			// �n�[�h�E�G�A���g�p����ꍇ
			Mask_DrawBeginFunction_PF( &Rect ) ;
		}
		else
		{
			IMAGEDATA *Image ;

			// �n�[�h�E�G�A���g�p���Ȃ��ꍇ

			// �`���̉摜�f�[�^�A�h���X���擾����
			Image = Graphics_Image_GetData( GSYS.DrawSetting.TargetScreen[ 0 ] ) ;

			// �`��Ώ� MEMIMG ����}�X�N�����p MEMIMG �ɃC���[�W��]��
			if( MASKD.MaskValidFlag )
			{
				BltMemImg(
					&MASKD.MaskDrawMemImg, 
					Image ? &Image->Soft.MemImg : &GSYS.SoftRender.MainBufferMemImg,
					&Rect,
					Rect.left,
					Rect.top ) ;
			}
		}
	}

	// �I��
	return 0 ;
}

// �}�X�N���g�p�����`��̌�ɌĂԊ֐�
// ( ���łɃT�u�o�b�t�@���g�p�����`��G���A�@�\���g�p���Ă���ꍇ�̏���������Ă��܂��Ă���� )
extern int Mask_DrawAfterFunction( RECT Rect )
{
	MASKD.MaskBeginFunctionCount -- ;
	if( MASKD.MaskBeginFunctionCount == 0 )
	{
		// �`��G���A�ŃN���b�s���O
		RectClipping_Inline( &Rect, &GSYS.DrawSetting.DrawArea ) ;
		if( Rect.left == Rect.right || Rect.top == Rect.bottom ) return 0 ;

		// Direct3D ���g�p���Ă��邩�ǂ����ŏ����𕪊�
		// �n�[�h�E�G�A���g�p���邩�ǂ����ŏ����𕪊�
		if( GSYS.Setting.ValidHardware )
		{
			// �n�[�h�E�G�A���g�p����ꍇ
			Mask_DrawAfterFunction_PF( &Rect ) ;
		}
		else
		{
			IMAGEDATA *Image ;

			// �n�[�h�E�G�A���g�p���Ȃ��ꍇ

			// �}�X�N���|�����Ă��Ȃ�������`���ɓ]������
			if( MASKD.MaskValidFlag )
			{
				BYTE *Dest, *Src, *Mask ;
				int DestAddPitch, SrcAddPitch, MaskAddPitch ;
				int DestWidth, DestHeight ;
				MEMIMG *TargetMemImg ;
				int PixelByte ;

				// �`���̉摜�f�[�^�A�h���X���擾����
				Image = Graphics_Image_GetData( GSYS.DrawSetting.TargetScreen[ 0 ] ) ;
				
				TargetMemImg = Image ? &Image->Soft.MemImg : &GSYS.SoftRender.MainBufferMemImg ;
				PixelByte = TargetMemImg->Base->ColorDataP->PixelByte ;

				DestWidth = Rect.right - Rect.left ;
				DestHeight = Rect.bottom - Rect.top ;

				MaskAddPitch = MASKD.MaskBufferPitch                     - DestWidth ;
				SrcAddPitch  = ( int )( MASKD.MaskDrawMemImg.Base->Pitch - PixelByte * DestWidth ) ;
				DestAddPitch = ( int )( TargetMemImg->Base->Pitch        - PixelByte * DestWidth ) ;

				Mask = ( BYTE * )MASKD.MaskBuffer              + Rect.left             + Rect.top * MASKD.MaskBufferPitch ;
				Src  = ( BYTE * )MASKD.MaskDrawMemImg.UseImage + Rect.left * PixelByte + Rect.top * MASKD.MaskDrawMemImg.Base->Pitch ;
				Dest = ( BYTE * )TargetMemImg->UseImage        + Rect.left * PixelByte + Rect.top * TargetMemImg->Base->Pitch ;

				if( MASKD.MaskReverseEffectFlag == TRUE )
				{
#ifdef DX_NON_INLINE_ASM
					int i ;
					// ���ʔ��]��
					switch( PixelByte )
					{
					case 2 :
						do
						{
							i = DestWidth ;
							do
							{
								if( *Mask != 0 )
									*( ( WORD * )Dest ) = *( ( WORD * )Src ) ;

								Dest += 2 ;
								Src += 2 ;
								Mask ++ ;

							}while( -- i != 0 ) ;

							Dest += DestAddPitch ;
							Src += SrcAddPitch ;
							Mask += MaskAddPitch ;

						}while( -- DestHeight != 0 ) ;
						break ;

					case 4 :
						do
						{
							i = DestWidth ;
							do
							{
								if( *Mask != 0 )
									*( ( DWORD * )Dest ) = *( ( DWORD * )Src ) ;

								Dest += 4 ;
								Src += 4 ;
								Mask ++ ;

							}while( -- i != 0 ) ;

							Dest += DestAddPitch ;
							Src += SrcAddPitch ;
							Mask += MaskAddPitch ;

						}while( -- DestHeight != 0 ) ;
						break ;
					}
#else
					// ���ʔ��]��
					switch( PixelByte )
					{
					case 2 :
						__asm
						{
							MOV		EDI, Dest
							MOV		ESI, Src
							MOV		EDX, Mask
	LOOP_2_1:
							MOV		ECX, DestWidth
	LOOP_2_2:
							MOV		AL, [ EDX ]
							TEST	AL, 0xff
							JZ		LABEL_2_1
							MOV		BX, [ ESI ]
							MOV		[ EDI ], BX
	LABEL_2_1:
							ADD		ESI, 2
							ADD		EDI, 2
							INC		EDX
							DEC		ECX
							JNZ		LOOP_2_2

							ADD		ESI, SrcAddPitch
							ADD		EDI, DestAddPitch
							ADD		EDX, MaskAddPitch

							DEC		DestHeight
							JNZ		LOOP_2_1
						}
						break ;

					case 4 :
						__asm
						{
							MOV		EDI, Dest
							MOV		ESI, Src
							MOV		EDX, Mask
	LOOP_4_1:
							MOV		ECX, DestWidth
	LOOP_4_2:
							MOV		AL, [ EDX ]
							TEST	AL, 0xff
							JZ		LABEL_4_1
							MOV		EBX, [ ESI ]
							MOV		[ EDI ], EBX
	LABEL_4_1:
							ADD		ESI, 4
							ADD		EDI, 4
							INC		EDX
							DEC		ECX
							JNZ		LOOP_4_2

							ADD		ESI, SrcAddPitch
							ADD		EDI, DestAddPitch
							ADD		EDX, MaskAddPitch

							DEC		DestHeight
							JNZ		LOOP_4_1
						}
						break ;
					}
#endif
				}
				else
				{
#ifdef DX_NON_INLINE_ASM
					int i ;
					// ���ʔ��]���ĂȂ���
					switch( PixelByte )
					{
					case 2 :
						do
						{
							i = DestWidth ;
							do
							{
								if( *Mask == 0 )
									*( ( WORD * )Dest ) = *( ( WORD * )Src ) ;

								Dest += 2 ;
								Src += 2 ;
								Mask ++ ;

							}while( -- i != 0 ) ;

							Dest += DestAddPitch ;
							Src += SrcAddPitch ;
							Mask += MaskAddPitch ;

						}while( -- DestHeight != 0 ) ;
						break ;

					case 4 :
						do
						{
							i = DestWidth ;
							do
							{
								if( *Mask == 0 )
									*( ( DWORD * )Dest ) = *( ( DWORD * )Src ) ;

								Dest += 4 ;
								Src += 4 ;
								Mask ++ ;

							}while( -- i != 0 ) ;

							Dest += DestAddPitch ;
							Src += SrcAddPitch ;
							Mask += MaskAddPitch ;

						}while( -- DestHeight != 0 ) ;
						break ;
					}
#else
					// ���ʔ��]���ĂȂ���
					switch( PixelByte )
					{
					case 2 :
						__asm
						{
							MOV		EDI, Dest
							MOV		ESI, Src
							MOV		EDX, Mask
	LOOP_2_1R:
							MOV		ECX, DestWidth
	LOOP_2_2R:
							MOV		AL, [ EDX ]
							TEST	AL, 0xff
							JNZ		LABEL_2_1R
							MOV		BX, [ ESI ]
							MOV		[ EDI ], BX
	LABEL_2_1R:
							ADD		ESI, 2
							ADD		EDI, 2
							INC		EDX
							DEC		ECX
							JNZ		LOOP_2_2R

							ADD		ESI, SrcAddPitch
							ADD		EDI, DestAddPitch
							ADD		EDX, MaskAddPitch

							DEC		DestHeight
							JNZ		LOOP_2_1R
						}
						break ;

					case 4 :
						__asm
						{
							MOV		EDI, Dest
							MOV		ESI, Src
							MOV		EDX, Mask
	LOOP_4_1R:
							MOV		ECX, DestWidth
	LOOP_4_2R:
							MOV		AL, [ EDX ]
							TEST	AL, 0xff
							JNZ		LABEL_4_1R
							MOV		EBX, [ ESI ]
							MOV		[ EDI ], EBX
	LABEL_4_1R:
							ADD		ESI, 4
							ADD		EDI, 4
							INC		EDX
							DEC		ECX
							JNZ		LOOP_4_2R

							ADD		ESI, SrcAddPitch
							ADD		EDI, DestAddPitch
							ADD		EDX, MaskAddPitch

							DEC		DestHeight
							JNZ		LOOP_4_1R
						}
						break ;
					}
#endif
				}
			}
		}
	}

	// �I��
	return 0 ;
}

// �}�X�N���Z�b�g����
extern int NS_DrawMask( int x, int y, int MaskHandle, int TransMode )
{
	MASKDATA * Mask ; 
	RECT Rect, MovRect ;

	if( MASKD.InitializeFlag == FALSE )
		return -1 ;

	if( MASKHCHK( MaskHandle, Mask ) )
		return -1 ;

	if( MASKD.MaskBuffer == NULL )
		return 0 ;

	// �N���b�s���O����
	SETRECT( Rect, x, y, x + Mask->MaskWidth, y + Mask->MaskHeight ) ;
	RectClipping_Inline( &Rect, &GSYS.DrawSetting.DrawArea ) ;
	MovRect = Rect ;
	if( Rect.left == Rect.right || Rect.top == Rect.bottom ) return 0 ;
	Rect.left   -= x ;
	Rect.right  -= x ;
	Rect.top    -= y ;
	Rect.bottom -= y ;
	if( Rect.left > 0 ){ x += Rect.left ; }
	if( Rect.top  > 0 ){ y += Rect.top  ; }

	// �]������
	{
		BYTE *Dest, *Src ;
		int DestAddPitch, SrcAddPitch ;
		int DestWidth, DestHeight, DWordNum, BNum ;
#ifdef DX_NON_INLINE_ASM
		int i ;
#endif

		DestWidth = Rect.right - Rect.left ;
		DestHeight = Rect.bottom - Rect.top ;
		DWordNum = DestWidth / 4 ;
		BNum = DestWidth % 4 ;
		Src = Mask->SrcData + Rect.left + ( Rect.top * Mask->SrcDataPitch ) ;
		Dest = MASKD.MaskBuffer + x + ( y * MASKD.MaskBufferPitch ) ;

		SrcAddPitch = Mask->SrcDataPitch - DestWidth ;
		DestAddPitch = MASKD.MaskBufferPitch - DestWidth ;


		switch( TransMode )
		{
		case DX_MASKTRANS_NONE :
			// ���ߐF�Ȃ��]��
#ifdef DX_NON_INLINE_ASM
			do
			{
				i = DestWidth ;
				do
				{
					*Dest = *Src ;
					Dest ++ ;
					Src ++ ;
				}while( -- i != 0 ) ;
				Dest += DestAddPitch ;
				Src += SrcAddPitch ;
			}while( -- DestHeight != 0 ) ;
#else
			__asm{
				CLD
				MOV		EDI, Dest
				MOV		ESI, Src
				MOV		EDX, DestWidth
				MOV		EBX, DestHeight
				MOV		EAX, SrcAddPitch
				MOV		ECX, DestAddPitch
				PUSH	EBP
				MOV		EBP, ECX
			LOOP1_a:
				MOV		ECX, EDX
				REP		MOVSB
				ADD		EDI, EBP
				ADD		ESI, EAX
				DEC		EBX
				JNZ		LOOP1_a
				POP		EBP
			}
#endif
			break ;

		case DX_MASKTRANS_BLACK :
			// �����ߐF
#ifdef DX_NON_INLINE_ASM
			do
			{
				if( DWordNum != 0 )
				{
					i = DWordNum ;
					do
					{
						*( ( DWORD * )Dest ) |= *( ( DWORD * )Src ) ;
						Dest += 4 ;
						Src += 4 ;
					}while( -- i != 0 ) ;
				}

				if( BNum != 0 )
				{
					i = BNum ;
					do
					{
						*Dest |= *Src ;
						Dest ++ ;
						Src ++ ;
					}while( -- i != 0 ) ;
				}

				Dest += DestAddPitch ;
				Src += SrcAddPitch ;
			}while( -- DestHeight != 0 ) ;
#else
			__asm{
				MOV		EDI, Dest
				MOV		ESI, Src
				MOV		EDX, BNum
				SHL		EDX, 30
				OR		EDX, DWordNum
				MOV		EBX, DestHeight

			LOOP1B:
				MOV		ECX, EDX
				AND		ECX, 0x3fffffff // ((1 << 30) - 1)
				CMP		ECX, 0
				JE		R10B

			LOOP2B:
				MOV		EAX, [ESI]
				OR		[EDI], EAX

				ADD		EDI, 4
				ADD		ESI, 4

				DEC		ECX
				JNZ		LOOP2B

			R10B:

				MOV		ECX, EDX
				SHR		ECX, 30
				CMP		ECX, 0
				JE		R11B

			LOOP3B:
				MOV		AL, [ESI]
				OR		[EDI], AL
				INC		ESI
				INC		EDI
				DEC		ECX
				JNZ		LOOP3B

			R11B:

				ADD		EDI, DestAddPitch
				ADD		ESI, SrcAddPitch
				DEC		EBX
				JNZ		LOOP1B
			}
#endif
			break ;

		case DX_MASKTRANS_WHITE :
			// �����ߐF
#ifdef DX_NON_INLINE_ASM
			do
			{
				if( DWordNum != 0 )
				{
					i = DWordNum ;
					do
					{
						*( ( DWORD * )Dest ) &= *( ( DWORD * )Src ) ;
						Dest += 4 ;
						Src += 4 ;
					}while( -- i != 0 ) ;
				}

				if( BNum != 0 )
				{
					i = BNum ;
					do
					{
						*Dest &= *Src ;
						Dest ++ ;
						Src ++ ;
					}while( -- i != 0 ) ;
				}

				Dest += DestAddPitch ;
				Src += SrcAddPitch ;
			}while( -- DestHeight != 0 ) ;
#else
			__asm
			{
				MOV		EDI, Dest
				MOV		ESI, Src
				MOV		EDX, BNum
				SHL		EDX, 30
				OR		EDX, DWordNum
				MOV		EBX, DestHeight

			LOOP1W:
				MOV		ECX, EDX
				AND		ECX, 0x3fffffff // ((1 << 30) - 1)
				CMP		ECX, 0
				JE		R10W

			LOOP2W:
				MOV		EAX, [ESI]
				AND		[EDI], EAX
				ADD		EDI, 4
				ADD		ESI, 4

				DEC		ECX
				JNZ		LOOP2W

			R10W:
				MOV		ECX, EDX
				SHR		ECX, 30
				CMP		ECX, 0
				JE		R11W

			LOOP3W:
				MOV		AL, [ESI]
				AND		[EDI], AL
				INC		ESI
				INC		EDI
				DEC		ECX
				JNZ		LOOP3W

			R11W:
				ADD		EDI, DestAddPitch
				ADD		ESI, SrcAddPitch
				DEC		EBX
				JNZ		LOOP1W
			}
#endif
			break ;
		}
	}

	// �X�V
	if( GSYS.Setting.ValidHardware )
	{
		Mask_UpdateMaskImageTexture_PF( &MovRect ) ;
	}

	// �I��
	return 0 ;
}

// �w��̃}�X�N����ʂ����ς��ɓW�J����
extern int NS_DrawFillMask( int x1, int y1, int x2, int y2, int MaskHandle )
{
	MASKDATA * Mask ;
	RECT Rect, MovRect ;

	if( MASKD.InitializeFlag == FALSE )
		return -1 ;


	if( MASKHCHK( MaskHandle, Mask ) )
		return -1 ;

	if( MASKD.MaskBuffer == NULL )
		return 0 ;

	if( x1 == x2 || y1 == y2 ) return 0 ;

	CheckActiveState() ;

	// �N���b�s���O����
	SETRECT( Rect, x1, y1, x2, y2 ) ;
	RectClipping_Inline( &Rect, &GSYS.DrawSetting.DrawArea ) ;
	MovRect = Rect ;
	if( Rect.left == Rect.right || Rect.top == Rect.bottom ) return 0 ;
	Rect.left -= x1 ;
	Rect.right -= x1 ;
	Rect.top -= y1 ;
	Rect.bottom -= y1 ;
	if( Rect.left > 0 ){ x1 += Rect.left ; }
	if( Rect.top  > 0 ){ y1 += Rect.top  ; }

	{
		BYTE *Dest, *Src, *Src2, *Src3 ;
		int DestWidth, DestHeight, SrcWidth, SrcHeight ;
		DWORD SrcAddPitch, DestAddPitch ;
		DWORD FirstX, FirstY ;
		DWORD CounterY ;

		SrcWidth = Mask->MaskWidth ;
		SrcHeight = Mask->MaskHeight ;

		DestWidth = Rect.right - Rect.left ;
		DestHeight = Rect.bottom - Rect.top ;

		FirstX = ( DWORD )( Mask->MaskWidth  - Rect.left % Mask->MaskWidth  ) ;
		FirstY = ( DWORD )( Mask->MaskHeight - Rect.top  % Mask->MaskHeight ) ;

		Dest = MASKD.MaskBuffer + x1 + y1 * MASKD.MaskBufferPitch ;
		Src = Mask->SrcData + ( Rect.left % Mask->MaskWidth ) + ( Rect.top % Mask->MaskHeight ) * Mask->SrcDataPitch ;
		Src2 = Mask->SrcData ;
		Src3 = Mask->SrcData + ( Rect.left % Mask->MaskWidth ) ;

		SrcAddPitch  = ( DWORD )( Mask->SrcDataPitch - ( ( DestWidth + Rect.left % Mask->MaskWidth ) % Mask->MaskWidth == 0 ? 0 : ( DestWidth + Rect.left % Mask->MaskWidth ) % Mask->MaskWidth ) + Rect.left % Mask->MaskWidth ) ;
		DestAddPitch = ( DWORD )( MASKD.MaskBufferPitch - DestWidth ) ;

		CounterY = FirstY ;

#ifdef DX_NON_INLINE_ASM
		int i, TempWidth, TempHeight ;

		TempHeight = ( int )FirstY ;
		do
		{
			TempWidth = ( int )FirstX ;
			i = DestWidth ;
			do
			{
				*Dest = *Src ;
				Dest ++ ;
				Src ++ ;
				if( -- TempWidth == 0 )
				{
					TempWidth = SrcWidth ;
					Src -= SrcWidth ;
				}
			}while( -- i != 0 ) ;

			Dest += DestAddPitch ;
			Src += SrcAddPitch ;

			if( -- TempHeight == 0 )
			{
				Src = Src3 ;
				TempHeight = SrcHeight ;
			}

		}while( -- DestHeight != 0 ) ;
#else
		__asm{
			CLD
			MOV		EDI, Dest
			MOV		ESI, Src
			MOV		EAX, SrcWidth
			MOV		EBX, FirstY
		LOOP1:
			MOV		ECX, FirstX
			MOV		EDX, DestWidth
		LOOP2:
			MOVSB
			DEC		ECX
			JZ		R2
		R1:
			DEC		EDX
			JZ		RE
			JMP		LOOP2

		R2:
			MOV		ECX, EAX
			SUB		ESI, ECX
			JMP		R1

		RE:
			ADD		ESI, SrcAddPitch
			ADD		EDI, DestAddPitch
			DEC		DestHeight
			JZ		RE2
			DEC		EBX
			JZ		R3
			JMP		LOOP1

		R3:
			MOV		ESI, Src3
			MOV		ECX, SrcHeight
			MOV		EBX, ECX
			JMP		LOOP1

		RE2:
		}
#endif
	}

	if( GSYS.Setting.ValidHardware )
	{
		Mask_UpdateMaskImageTexture_PF( &MovRect ) ;
	}

	// �I��
	return 0 ;
}

// �}�X�N�̃Z�b�g
extern int NS_DrawMaskToDirectData( int x, int y, int Width, int Height, const void *MaskData, int TransMode )
{
	if( MASKD.InitializeFlag == FALSE )
		return -1 ;

	// �}�X�N�X�N���[�����Ȃ��ꍇ�͂����ŏ������I��
	if( MASKD.MaskBuffer == NULL ) return 0 ;

	// �}�X�N�f�[�^���Ȃ������ꍇ�͏������I��
	if( MaskData == NULL ) return 0 ;

	CheckActiveState() ;

	// �}�X�N�X�N���[���Ƀ}�X�N��W�J
	DrawMaskToDirectData_Base( x, y, MASKD.MaskBuffer, MASKD.MaskBufferPitch,
							Width, Height, MaskData, TransMode ) ;

	RECT Rect ;

	if( GSYS.Setting.ValidHardware )
	{
		// �X�V
		Rect.left   = x ;
		Rect.top    = y ;
		Rect.right  = x + Width ;
		Rect.bottom = y + Height ;
		Mask_UpdateMaskImageTexture_PF( &Rect ) ;
	}

	// �I��
	return 0 ;
}

// �}�X�N�X�N���[�����w��̐F�œh��Ԃ�
extern int NS_FillMaskScreen( int Flag )
{
	if( MASKD.InitializeFlag == FALSE )
		return -1 ;

	// �}�X�N�X�N���[�����Ȃ��ꍇ�͂����ŏ������I��
	if( MASKD.MaskBuffer == NULL ) return -1 ;

	CheckActiveState() ;

	// �}�X�N�X�N���[�����N���A����
	_MEMSET( MASKD.MaskBuffer, ( unsigned char )( Flag ? 0xff : 0 ), ( size_t )( MASKD.MaskBufferPitch * GSYS.DrawSetting.DrawSizeY ) ) ;

	if( GSYS.Setting.ValidHardware )
	{
		Mask_FillMaskScreen_PF( Flag ) ;
	}

	// �I��
	return 0 ;
}

// �}�X�N�X�N���[���Ƃ��Ďg�p����O���t�B�b�N�̃n���h����ݒ肷��A-1��n���Ɖ���( �����œn���O���t�B�b�N�n���h���� MakeScreen �ō쐬�����u�A���t�@�`�����l���t���̕`��Ώۂɂł���O���t�B�b�N�n���h���v�ł���K�v������܂�( �A���t�@�`�����l�����}�X�N�Ɏg�p����܂� ) )
extern int NS_SetMaskScreenGraph( int GraphHandle )
{
	IMAGEDATA *Image ;

	if( MASKD.InitializeFlag == FALSE )
	{
		return -1 ;
	}

	// -1 �̏ꍇ�� 0 �ɂ���
	if( GraphHandle == -1 )
	{
		GraphHandle = 0 ;
	}
	else
	{
		// �����ȃO���t�B�b�N�n���h�����A�`��Ώۂɂł��Ȃ��O���t�B�b�N�n���h���ł���ꍇ�̓G���[
		Image = Graphics_Image_GetData( GraphHandle ) ;
		if( Image == NULL ||
			Image->Orig->FormatDesc.TextureFlag == FALSE ||
			Image->Orig->FormatDesc.DrawValidFlag == FALSE )
		{
			return -1 ;
		}
	}

	// ���܂łƒl�������ł���ꍇ�͉��������I��
	if( GraphHandle == MASKD.MaskScreenGraphHandle )
	{
		return 0 ;
	}

	// �t���X�N���[���}�X�N�������s���Ă���ꍇ�̓}�X�N�`��̌��ʂ𔽉f������
	MASK_FULLSCREEN_MASKUPDATE

	// �O���t�B�b�N�n���h����ۑ�
	MASKD.MaskScreenGraphHandle = GraphHandle ;

	// ����I��
	return 0 ;
}

// �}�X�N�X�N���[���Ƃ��Ďg�p����O���t�B�b�N�́A�ǂ̃`�����l�����}�X�N�Ƃ��Ďg�p���邩��ݒ肷��( �f�t�H���g�� DX_MASKGRAPH_CH_A�A ���ADX_MASKGRAPH_CH_A�ȊO���g�p����ꍇ�̓O���t�B�b�N�X�f�o�C�X���V�F�[�_�[���f��2.0�ȍ~�ɑΉ����Ă���K�v������܂� )
extern int NS_SetMaskScreenGraphUseChannel(	int UseChannel )
{
	if( UseChannel < 0 || UseChannel > DX_MASKGRAPH_CH_B )
	{
		return -1 ;
	}

	MASKD.MaskScreenGraphHandleUseChannel = UseChannel ;

	// ����I��
	return 0 ;
}











#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

#endif


