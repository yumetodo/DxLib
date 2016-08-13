// ----------------------------------------------------------------------------
// 
// 		�c�w���C�u����		�a�������h���������v���O����
// 
// 				Ver 3.16d
// 
// ----------------------------------------------------------------------------

// �c�w���C�u�����쐬���p��`
#define __DX_MAKE

// �C���N���[�h----------------------------------------------------------------
#include "DxBaseImage.h"
#include "DxMemory.h"
#include "DxBaseFunc.h"
#include "DxGraphics.h"
#include "DxFile.h"
#include "DxLog.h"
#include "DxChar.h"
#include "DxUseCLib.h"
#include "DxASyncLoad.h"
#include "DxSystem.h"
#include "DxMath.h"

#ifdef __WINDOWS__
#include "Windows/DxBaseImageWin.h"
#endif // __WINDOWS__

#ifdef __psp2__
#include "PSVita/DxBaseImagePSVita.h"
#endif // __psp2__

#ifdef __ORBIS__
#include "PS4/DxBaseImagePS4.h"
#endif // __ORBIS__

#ifdef __ANDROID__
#include "Android/DxBaseImageAndroid.h"
#endif // __ANDROID__


#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// �}�N����`------------------------------------------------------------------

// �\���̐錾------------------------------------------------------------------

// �֐��錾--------------------------------------------------------------------

// �摜�ǂݍ��݊֐�
static	int LoadBmpImage( STREAMDATA *Stream, BASEIMAGE *BaseImage, int GetFormatOnly ) ;					// �a�l�o�摜�̓ǂ݂���
static	int LoadArgbImage( STREAMDATA *Src, BASEIMAGE *BaseImage, int GetFormatOnly ) ;						// �`�q�f�a�摜�̓ǂ݂���

#ifndef DX_NON_TGA
static	int LoadTargaImage( STREAMDATA *Src, BASEIMAGE *BaseImage, int GetFormatOnly ) ;					// �s�f�`�摜�̓ǂ݂���
#endif

static	int LoadDDSImage( STREAMDATA *Src, BASEIMAGE *BaseImage, int GetFormatOnly ) ;						// �c�c�r�摜�̓ǂ݂���

// �A���t�@�}�X�N�p�̃t�@�C���p�X���쐬����
static int CreateAlphaMaskFilePath(  const wchar_t *Path, wchar_t *Dest, size_t BufferBytes ) ;
static int CreateAlphaMaskFilePathT( const TCHAR   *Path, TCHAR   *Dest, size_t BufferBytes ) ;

// �f�[�^�錾------------------------------------------------------------------

// �摜�ǂݍ��݊֐��z��
int ( *DefaultImageLoadFunc[] )( STREAMDATA *Src, BASEIMAGE *BaseImage, int GetFormatOnly ) =
{
	LoadBmpImage ,
#ifndef DX_NON_JPEGREAD
	LoadJpegImage ,
#endif
#ifndef DX_NON_PNGREAD
	LoadPngImage ,
#endif
#ifndef DX_NON_TGA
	LoadTargaImage ,
#endif
	LoadDDSImage ,
#ifndef DX_NON_TIFFREAD
	LoadTiffImage ,
#endif
	LoadArgbImage ,
	NULL 
} ;

BASEIMAGEMANAGE BaseImageManage ;

// �v���O�����R�[�h------------------------------------------------------------

// �����x���������_��P���x���������_�ɕϊ�����
__inline float Float16ToFloat32( WORD Float16 )
{
	unsigned int s ;
	int e ;
	unsigned int f ;
	unsigned int dest ;

	if( Float16 == 0 )
	{
		return	0.0f ;
	}

	s =     Float16 & 0x8000 ;
	e = ( ( Float16 & 0x7c00 ) >> 10 ) - 15 + 127 ;
	f =     Float16 & 0x03ff ;
	dest = ( s << 16 ) | ( ( e << 23 ) & 0x7f800000 ) | ( f << 13 ) ;
	return	*( ( float * )&dest ) ;
}

// �P���x���������_�𔼐��x���������_�ɕϊ�����
__inline WORD Float32ToFloat16( float Float32 )
{
	unsigned int src = *( ( DWORD * )&Float32 ) ;
	int e ;
	unsigned int s ;
	unsigned int f ;

	if( src == 0 )
	{
		return 0 ;
	}

	e = ( ( src & 0x7f800000 ) >> 23 ) - 127 + 15 ;
	if( e < 0 )
	{
		return 0 ;
	}
	else
	if( e > 31 )
	{
		e = 31 ;
	}

	s = src & 0x80000000 ;
	f = src & 0x007fffff ;
	return ( ( s >> 16 ) & 0x8000 ) | ( ( e << 10 ) & 0x7c00 ) | ( ( f >> 13 ) & 0x03ff ) ;
}

// CREATEBASEIMAGETYPE2_GPARAM �̃f�[�^���Z�b�g����
extern void InitCreateBaseImageType2GParam( CREATEBASEIMAGETYPE2_GPARAM *GParam, int GetFormatOnly )
{
	int i ;

	for( i = 0 ; i < BASEIM.UserImageLoadFuncNum4 ; i ++ )
	{
		GParam->UserImageLoadFunc4[ i ] = BASEIM.UserImageLoadFunc4[ i ] ;
	}

	GParam->UserImageLoadFuncNum4 = BASEIM.UserImageLoadFuncNum4 ;
	GParam->GetFormatOnly         = GetFormatOnly ;
}

// LOADBASEIMAGE_GPARAM �̃f�[�^���Z�b�g����
extern void InitLoadBaseImageGParam( LOADBASEIMAGE_GPARAM *GParam, int GetFormatOnly )
{
	InitCreateBaseImageType2GParam( &GParam->CreateGraphImageType2GParam, GetFormatOnly ) ;

	GParam->StreamDataShred2			= *StreamGetStruct() ;
	GParam->FileStreamDataShred			= *GetFileStreamDataShredStruct() ;
	GParam->MemStreamDataShred			= *GetMemStreamDataShredStruct() ;
	GParam->ConvertPremultipliedAlpha	= BASEIM.ConvertPremultipliedAlpha ;
	GParam->AlphaTestImageCreateFlag	= BASEIM.AlphaTestImageCreateFlag ;
	GParam->TransColor					= BASEIM.TransColor ;
}

// ��{�C���[�W�f�[�^�̃��[�h�{�c�h�a�֌W

#ifndef DX_NON_SAVEFUNCTION

// ��{�摜�f�[�^���a�l�o�摜�Ƃ��ĕۑ�����
extern int NS_SaveBaseImageToBmp( const TCHAR *FilePath, const BASEIMAGE *BaseImage )
{
#ifdef UNICODE
	return SaveBaseImageToBmp_WCHAR_T(
		FilePath, BaseImage
	) ;
#else
	int Result ;

	TCHAR_TO_WCHAR_T_STRING_ONE_BEGIN( FilePath, return -1 )

	Result = SaveBaseImageToBmp_WCHAR_T(
		UseFilePathBuffer, BaseImage
	) ;

	TCHAR_TO_WCHAR_T_STRING_END( FilePath )

	return Result ;
#endif
}

// ��{�摜�f�[�^���a�l�o�摜�Ƃ��ĕۑ�����
extern int SaveBaseImageToBmp_WCHAR_T( const wchar_t *FilePath, const BASEIMAGE *BaseImage )
{
	BITMAPFILEHEADER BmpHead ;
	BITMAPINFO *BmpInfo = 0 ;
	BYTE *DIBBuf = 0 ;
	BYTE *Dest, *Src ;
	int Ret = -1 ;
	int x, y, i ;
	int WidthByte ;
	DWORD_PTR fp ;

	// �t�@�C���w�b�_�Ɋ�{�f�[�^���Z�b�g
	_MEMSET( &BmpHead, 0, sizeof( BmpHead ) ) ;
	( ( BYTE * )&BmpHead.bfType )[ 0 ] = 'B' ;
	( ( BYTE * )&BmpHead.bfType )[ 1 ] = 'M' ;
	BmpHead.bfOffBits	= sizeof( BmpHead ) + sizeof( BITMAPINFOHEADER ) ;
	BmpHead.bfSize		= BmpHead.bfOffBits ;

	// �F�����Ƃɏ�����ύX����
	switch( BaseImage->ColorData.ColorBitDepth )
	{
	case 8:
		// �p���b�g�ۑ��p���������܂߂��T�C�Y�̃��������m�ۂ���
		BmpInfo = ( BITMAPINFO * )DXCALLOC( sizeof( BITMAPINFO ) + sizeof( RGBQUAD ) * 256 ) ;
		if( BmpInfo == NULL )
		{
			DXST_ERRORLOG_ADDUTF16LE( "\x42\x00\x49\x00\x54\x00\x4d\x00\x41\x00\x50\x00\x49\x00\x4e\x00\x46\x00\x4f\x00\xcb\x69\x20\x90\x53\x4f\x28\x75\xe1\x30\xe2\x30\xea\x30\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"BITMAPINFO�\���̗p�������̊m�ۂɎ��s���܂���\n" @*/ ) ; goto END ;
		}

		// �P���C��������̃f�[�^�ʂ��Z�o
		WidthByte = ( BaseImage->Width + 3 ) / 4 * 4 ; 

		// �O���t�B�b�N�ۑ��������̊m��
		DIBBuf = ( BYTE * )DXCALLOC( ( size_t )( WidthByte * BaseImage->Height ) ) ;
		if( DIBBuf == NULL )
		{
			DXST_ERRORLOG_ADDUTF16LE( "\xb0\x30\xe9\x30\xd5\x30\xa3\x30\xc3\x30\xaf\x30\xdd\x4f\x58\x5b\x28\x75\xe1\x30\xe2\x30\xea\x30\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�O���t�B�b�N�ۑ��p�������̊m�ۂɎ��s���܂���\n" @*/ ) ; goto END ;
		}

		// �p���b�g�̕ۑ�
		{
			RGBQUAD *Bgb = BmpInfo->bmiColors ;
			const COLORPALETTEDATA *Pal = BaseImage->ColorData.Palette ;

			for( i = 0 ; i < 256 ; i ++, Bgb ++, Pal ++ )
			{
				Bgb->rgbBlue     = Pal->Blue ;
				Bgb->rgbGreen    = Pal->Green ;
				Bgb->rgbRed      = Pal->Red ;
				Bgb->rgbReserved = 0 ;
			}
		}

		// �C���[�W�f�[�^�̐擪�ʒu��ύX
		BmpHead.bfOffBits += sizeof( RGBQUAD ) * 256 ;

		// �t�@�C���̑��T�C�Y���Z�b�g
		BmpHead.bfSize += sizeof( RGBQUAD ) * 256 + WidthByte * BaseImage->Height ;

		// BITMAPINFO�Ƀf�[�^����������
		BmpInfo->bmiHeader.biBitCount = 8 ;
		BmpInfo->bmiHeader.biSize = sizeof( BmpInfo->bmiHeader ) ;
		BmpInfo->bmiHeader.biWidth = BaseImage->Width ;
		BmpInfo->bmiHeader.biHeight = BaseImage->Height ;
		BmpInfo->bmiHeader.biPlanes = 1 ;
		BmpInfo->bmiHeader.biCompression = 0L ;
		
		// �C���[�W���o�b�t�@�ɓ]��
		for( i = 0, y = BaseImage->Height - 1 ; y > -1 ; y --, i ++ )
		{
			_MEMCPY( DIBBuf + WidthByte * i, ( BYTE * )BaseImage->GraphData + BaseImage->Pitch * y, ( size_t )BaseImage->Width ) ;
		}

		// �t�@�C���ɏ����o��
		{
			WriteOnlyFileAccessDelete( FilePath ) ;
			fp = WriteOnlyFileAccessOpen( FilePath ) ;
			if( fp == 0 ) 
			{
				DXST_ERRORLOG_ADDUTF16LE( "\x22\xff\x2d\xff\x30\xff\xbb\x30\xfc\x30\xd6\x30\x28\x75\xd5\x30\xa1\x30\xa4\x30\xeb\x30\x6e\x30\xaa\x30\xfc\x30\xd7\x30\xf3\x30\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�a�l�o�Z�[�u�p�t�@�C���̃I�[�v���Ɏ��s���܂���\n" @*/ ) ; goto END ;
			}

			// �t�@�C���w�b�_�̏����o��
			WriteOnlyFileAccessWrite( fp, &BmpHead, sizeof( BmpHead ) ) ;
			
			// �C���t�H�w�b�_�̏����o��
			WriteOnlyFileAccessWrite( fp, BmpInfo, sizeof( BmpInfo->bmiHeader ) + sizeof( RGBQUAD ) * 256 ) ;

			// �C���[�W�̏����o��
			WriteOnlyFileAccessWrite( fp, DIBBuf, ( DWORD )( WidthByte * BaseImage->Height ) ) ;

			// �I��
			WriteOnlyFileAccessClose( fp ) ;
		}
		break ;

	case 32 :
	case 24 :
	case 16 :
		{
			// �p���b�g�ۑ��p���������܂߂��T�C�Y�̃��������m�ۂ���
			BmpInfo = ( BITMAPINFO * )DXCALLOC( sizeof( BITMAPINFO ) ) ;
			if( BmpInfo == NULL )
			{
				DXST_ERRORLOG_ADDUTF16LE( "\x42\x00\x49\x00\x54\x00\x4d\x00\x41\x00\x50\x00\x49\x00\x4e\x00\x46\x00\x4f\x00\xcb\x69\x20\x90\x53\x4f\x28\x75\xe1\x30\xe2\x30\xea\x30\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"BITMAPINFO�\���̗p�������̊m�ۂɎ��s���܂���\n" @*/ ) ; goto END ;
			}

			// �P���C��������̃f�[�^�ʂ��Z�o
			WidthByte = ( BaseImage->Width * 3 + 3 ) / 4 * 4 ; 

			// �O���t�B�b�N�ۑ��������̊m��
			DIBBuf = ( BYTE * )DXCALLOC( ( size_t )( WidthByte * BaseImage->Height ) ) ;
			if( DIBBuf == NULL ) 
			{
				DXST_ERRORLOG_ADDUTF16LE( "\xb0\x30\xe9\x30\xd5\x30\xa3\x30\xc3\x30\xaf\x30\xdd\x4f\x58\x5b\x28\x75\xe1\x30\xe2\x30\xea\x30\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�O���t�B�b�N�ۑ��p�������̊m�ۂɎ��s���܂���\n" @*/ ) ; goto END ;
			}

			// �t�@�C���̑��T�C�Y���Z�b�g
			BmpHead.bfSize += WidthByte * BaseImage->Height ;

			// BITMAPINFO�Ƀf�[�^����������
			BmpInfo->bmiHeader.biBitCount    = 24 ;
			BmpInfo->bmiHeader.biSize        = sizeof( BmpInfo->bmiHeader ) ;
			BmpInfo->bmiHeader.biWidth       = BaseImage->Width ;
			BmpInfo->bmiHeader.biHeight      = BaseImage->Height ;
			BmpInfo->bmiHeader.biPlanes      = 1 ;
			BmpInfo->bmiHeader.biCompression = 0L ;
			
			// �C���[�W���o�b�t�@�ɓ]��
			if( BaseImage->ColorData.BlueMask  == 0x00ff0000 &&
				BaseImage->ColorData.GreenMask == 0x0000ff00 &&
				BaseImage->ColorData.RedMask   == 0x000000ff &&
				BaseImage->ColorData.PixelByte == 4 )
			{
				for( i = 0, y = BaseImage->Height - 1 ; y > -1 ; y --, i ++ )
				{
					Dest = DIBBuf + WidthByte * i ;
					Src = ( BYTE * )BaseImage->GraphData + BaseImage->Pitch * y ;

					for( x = 0 ; x < BaseImage->Width ; x ++ )
					{
						Dest[ 0 ] = Src[ 2 ] ;
						Dest[ 1 ] = Src[ 1 ] ;
						Dest[ 2 ] = Src[ 0 ] ;
						Dest += 3 ;
						Src  += 4 ;
					}
				}
			}
			else
			if( BaseImage->ColorData.BlueMask  == 0x000000ff &&
				BaseImage->ColorData.GreenMask == 0x0000ff00 &&
				BaseImage->ColorData.RedMask   == 0x00ff0000 &&
				BaseImage->ColorData.PixelByte == 4 )
			{
				for( i = 0, y = BaseImage->Height - 1 ; y > -1 ; y --, i ++ )
				{
					Dest = DIBBuf + WidthByte * i ;
					Src = ( BYTE * )BaseImage->GraphData + BaseImage->Pitch * y ;

					for( x = 0 ; x < BaseImage->Width ; x ++ )
					{
						*Dest = *Src ; Src ++ ;   Dest ++ ; 
						*Dest = *Src ; Src ++ ;   Dest ++ ; 
						*Dest = *Src ; Src += 2 ; Dest ++ ;
					}
				}
			}
			else
			if( BaseImage->ColorData.BlueMask  == 0x000000ff &&
				BaseImage->ColorData.GreenMask == 0x0000ff00 &&
				BaseImage->ColorData.RedMask   == 0x00ff0000 &&
				BaseImage->ColorData.PixelByte == 3 )
			{
				for( i = 0, y = BaseImage->Height - 1 ; y > -1 ; y --, i ++ )
				{
					Dest = DIBBuf + WidthByte * i ;
					Src = ( BYTE * )BaseImage->GraphData + BaseImage->Pitch * y ;

					for( x = 0 ; x < BaseImage->Width ; x ++ )
					{
						*Dest = *Src ; Src ++ ; Dest ++ ; 
						*Dest = *Src ; Src ++ ; Dest ++ ; 
						*Dest = *Src ; Src ++ ; Dest ++ ;
					}
				}
			}
			else
			if( BaseImage->ColorData.PixelByte == 2 )
			{
				DWORD RedMask,  GreenMask, BlueMask ;
				DWORD RedLoc,   GreenLoc,  BlueLoc ;
				DWORD RedLoc2,  GreenLoc2, BlueLoc2 ;
				DWORD DestData, SrcData ;

				RedMask   = BaseImage->ColorData.RedMask ;
				GreenMask = BaseImage->ColorData.GreenMask ;
				BlueMask  = BaseImage->ColorData.BlueMask ;

				RedLoc    = BaseImage->ColorData.RedLoc ; 
				GreenLoc  = BaseImage->ColorData.GreenLoc ; 
				BlueLoc   = BaseImage->ColorData.BlueLoc ; 

				RedLoc2   = ( DWORD )( 8  - BaseImage->ColorData.RedWidth   ) ; 
				GreenLoc2 = ( DWORD )( 16 - BaseImage->ColorData.GreenWidth ) ; 
				BlueLoc2  = ( DWORD )( 24 - BaseImage->ColorData.BlueWidth  ) ;

				for( i = 0, y = BaseImage->Height - 1 ; y > -1 ; y --, i ++ )
				{
					Dest = DIBBuf + WidthByte * i ;
					Src = ( BYTE * )BaseImage->GraphData + BaseImage->Pitch * y ;

					for( x = 0 ; x < BaseImage->Width ; x ++ )
					{
						SrcData = ( DWORD )*( ( WORD * )Src ) ;
						DestData =
									( ( ( SrcData & RedMask   ) >> RedLoc   ) << RedLoc2   ) |
									( ( ( SrcData & GreenMask ) >> GreenLoc ) << GreenLoc2 ) |
									( ( ( SrcData & BlueMask  ) >> BlueLoc  ) << BlueLoc2  ) ;

						*Dest = ( BYTE )( DestData >> 16 ) ; Dest ++ ;
						*Dest = ( BYTE )( DestData >> 8  ) ; Dest ++ ;
						*Dest = ( BYTE )( DestData       ) ; Dest ++ ;

						Src += 2 ;
					}
				}
			}
			else
			{
				int r, g, b, a ;

				for( i = 0, y = BaseImage->Height - 1 ; y > -1 ; y --, i ++ )
				{
					Dest = DIBBuf + WidthByte * i ;

					for( x = 0 ; x < BaseImage->Width ; x ++ )
					{
						NS_GetPixelBaseImage( BaseImage, x, y, &r, &g, &b, &a ) ;

						Dest[ 0 ] = ( BYTE )b ;
						Dest[ 1 ] = ( BYTE )g ;
						Dest[ 2 ] = ( BYTE )r ;
						Dest += 3 ;
					}
				}
			}
		}

		// �t�@�C���ɏ����o��
		{
			WriteOnlyFileAccessDelete( FilePath ) ;
			fp = WriteOnlyFileAccessOpen( FilePath ) ;
			if( fp == 0 ) 
			{
				DXST_ERRORLOG_ADDUTF16LE( "\x22\xff\x2d\xff\x30\xff\xbb\x30\xfc\x30\xd6\x30\x28\x75\xd5\x30\xa1\x30\xa4\x30\xeb\x30\x6e\x30\xaa\x30\xfc\x30\xd7\x30\xf3\x30\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�a�l�o�Z�[�u�p�t�@�C���̃I�[�v���Ɏ��s���܂���\n" @*/ ) ; goto END ;
			}

			// �t�@�C���w�b�_�̏����o��
			WriteOnlyFileAccessWrite( fp, &BmpHead, sizeof( BmpHead ) ) ;
			
			// �C���t�H�w�b�_�̏����o��
			WriteOnlyFileAccessWrite( fp, BmpInfo, sizeof( BmpInfo->bmiHeader ) ) ;

			// �C���[�W�̏����o��
			WriteOnlyFileAccessWrite( fp, DIBBuf, ( DWORD )( WidthByte * BaseImage->Height ) ) ;

			// �I��
			WriteOnlyFileAccessClose( fp ) ;
		}
		break ;
	}

	Ret = 0 ;
END:

	// �������̉��
	if( BmpInfo ) DXFREE( BmpInfo ) ;
	if( DIBBuf  ) DXFREE( DIBBuf ) ;

	// ����I��
	return Ret ;
}

#define BI_DDPF_ALPHAPIXELS						(0x00000001l)
#define BI_DDPF_ALPHA							(0x00000002l)
#define BI_DDPF_FOURCC							(0x00000004l)
#define BI_DDPF_RGB								(0x00000040l)
#define BI_DDPF_ZBUFFER							(0x00000400l)
#define BI_DDPF_LUMINANCE						(0x00020000l)
#define BI_DDPF_BUMPLUMINANCE					(0x00040000l)
#define BI_DDPF_BUMPDUDV						(0x00080000l)
#define BI_DDPF_RGBTOYUV						(0x000001001)
#define BI_DDPF_YUV								(0x000002001)

#define BI_DDSD_CAPS							(0x00000001l)
#define BI_DDSD_HEIGHT							(0x00000002l)
#define BI_DDSD_WIDTH							(0x00000004l)
#define BI_DDSD_PITCH							(0x00000008l)
#define BI_DDSD_BACKBUFFERCOUNT					(0x00000020l)
#define BI_DDSD_ZBUFFERBITDEPTH					(0x00000040l)
#define BI_DDSD_ALPHABITDEPTH					(0x00000080l)
#define BI_DDSD_LPSURFACE						(0x00000800l)
#define BI_DDSD_PIXELFORMAT						(0x00001000l)
#define BI_DDSD_CKDESTOVERLAY					(0x00002000l)
#define BI_DDSD_CKDESTBLT						(0x00004000l)
#define BI_DDSD_CKSRCOVERLAY					(0x00008000l)
#define BI_DDSD_CKSRCBLT						(0x00010000l)
#define BI_DDSD_MIPMAPCOUNT						(0x00020000l)
#define BI_DDSD_REFRESHRATE						(0x00040000l)
#define BI_DDSD_LINEARSIZE						(0x00080000l)
#define BI_DDSD_TEXTURESTAGE					(0x00100000l)
#define BI_DDSD_FVF								(0x00200000l)
#define BI_DDSD_SRCVBHANDLE						(0x00400000l)
#define BI_DDSD_DEPTH							(0x00800000l)

#define BI_DDSCAPS_RESERVED1					(0x00000001l)
#define BI_DDSCAPS_ALPHA						(0x00000002l)
#define BI_DDSCAPS_BACKBUFFER					(0x00000004l)
#define BI_DDSCAPS_COMPLEX						(0x00000008l)
#define BI_DDSCAPS_FLIP							(0x00000010l)
#define BI_DDSCAPS_OFFSCREENPLAIN				(0x00000040l)
#define BI_DDSCAPS_OVERLAY						(0x00000080l)
#define BI_DDSCAPS_PRIMARYSURFACE				(0x00000200l)
#define BI_DDSCAPS_SYSTEMMEMORY					(0x00000800l)
#define BI_DDSCAPS_TEXTURE						(0x00001000l)
#define BI_DDSCAPS_3DDEVICE						(0x00002000l)
#define BI_DDSCAPS_VIDEOMEMORY					(0x00004000l)
#define BI_DDSCAPS_ZBUFFER						(0x00020000l)
#define BI_DDSCAPS_MIPMAP						(0x00400000l)
#define BI_DDSCAPS_LOCALVIDMEM					(0x10000000l)

#define BI_DDSCAPS2_RESERVED4					(0x00000002L)
#define BI_DDSCAPS2_HARDWAREDEINTERLACE			(0x00000000L)
#define BI_DDSCAPS2_HINTDYNAMIC					(0x00000004L)
#define BI_DDSCAPS2_HINTSTATIC					(0x00000008L)
#define BI_DDSCAPS2_TEXTUREMANAGE				(0x00000010L)
#define BI_DDSCAPS2_RESERVED1					(0x00000020L)
#define BI_DDSCAPS2_RESERVED2					(0x00000040L)
#define BI_DDSCAPS2_OPAQUE						(0x00000080L)
#define BI_DDSCAPS2_HINTANTIALIASING			(0x00000100L)
#define BI_DDSCAPS2_CUBEMAP						(0x00000200L)
#define BI_DDSCAPS2_CUBEMAP_POSITIVEX			(0x00000400L)
#define BI_DDSCAPS2_CUBEMAP_NEGATIVEX			(0x00000800L)
#define BI_DDSCAPS2_CUBEMAP_POSITIVEY			(0x00001000L)
#define BI_DDSCAPS2_CUBEMAP_NEGATIVEY			(0x00002000L)
#define BI_DDSCAPS2_CUBEMAP_POSITIVEZ			(0x00004000L)
#define BI_DDSCAPS2_CUBEMAP_NEGATIVEZ			(0x00008000L)
#define BI_DDSCAPS2_CUBEMAP_ALLFACES			(	BI_DDSCAPS2_CUBEMAP_POSITIVEX |\
													BI_DDSCAPS2_CUBEMAP_NEGATIVEX |\
													BI_DDSCAPS2_CUBEMAP_POSITIVEY |\
													BI_DDSCAPS2_CUBEMAP_NEGATIVEY |\
													BI_DDSCAPS2_CUBEMAP_POSITIVEZ |\
													BI_DDSCAPS2_CUBEMAP_NEGATIVEZ )
#define BI_DDSCAPS2_MIPMAPSUBLEVEL				(0x00010000L)
#define BI_DDSCAPS2_D3DTEXTUREMANAGE			(0x00020000L)
#define BI_DDSCAPS2_DONOTPERSIST				(0x00040000L)
#define BI_DDSCAPS2_STEREOSURFACELEFT			(0x00080000L)
#define BI_DDSCAPS2_VOLUME						(0x00200000L)
#define BI_DDSCAPS2_NOTUSERLOCKABLE				(0x00400000L)
#define BI_DDSCAPS2_POINTS						(0x00800000L)
#define BI_DDSCAPS2_RTPATCHES					(0x01000000L)
#define BI_DDSCAPS2_NPATCHES					(0x02000000L)
#define BI_DDSCAPS2_RESERVED3					(0x04000000L)
#define BI_DDSCAPS2_DISCARDBACKBUFFER			(0x10000000L)
#define BI_DDSCAPS2_ENABLEALPHACHANNEL			(0x20000000L)
#define BI_DDSCAPS2_EXTENDEDFORMATPRIMARY		(0x40000000L)
#define BI_DDSCAPS2_ADDITIONALPRIMARY			(0x80000000L)

// DDS�t�@�C���w�b�_
struct DDSFILEHEADER
{
	DWORD	dwMagic ;					// "DDS"
	DWORD	dwSize ;					// �������琔����w�b�_�̃T�C�Y( 124 )
	DWORD	dwFlags ;					// D_DDSURFACEDESC2.dwFlags �Ɠ�������
	DWORD	dwHeight ;					// �摜�̍���
	DWORD	dwWidth ;					// �摜�̕�
	DWORD	dwPitchOrLinearSize ;		// ��1 line �� byte �� (pitch)
										// �܂��� 1�ʕ��� byte �� (linearsize)
	DWORD	dwDepth ;					// �摜�̉��s�� z size (Volume Texture �p)
	DWORD	dwMipMapCount ;				// �܂܂�Ă��� mipmap ���x����
	DWORD	dwReserved1[ 11 ] ;			// �\��P
	DWORD	dwPfSize ;					// �s�N�Z���t�H�[�}�b�g�T�C�Y( 32 )
	DWORD	dwPfFlags ;					// D_DDPIXELFORMAT.dwFlags �Ɠ�������
	DWORD	dwFourCC ;					// FourCC
	DWORD	dwRGBBitCount ;				// �J���[�r�b�g�[�x
	DWORD	dwRBitMask ;				// �ԃ}�X�N
	DWORD	dwGBitMask ;				// �΃}�X�N
	DWORD	dwBBitMask ;				// �}�X�N
	DWORD	dwRGBAlphaBitMask ;			// ���}�X�N
	DWORD	dwCaps ;					// BI_DDSCAPS2.dwCaps �Ɠ�������
	DWORD	dwCaps2 ;					// BI_DDSCAPS2.dwCaps2 �Ɠ�������
	DWORD	dwReservedCaps[ 2 ] ;		// �\��Q
	DWORD	dwReserved2 ;				// �\��R
} ;

// �J���[�t�H�[�}�b�g�e�[�u���\����
struct DDSCOLORTABLE
{
	DWORD Type ;
	BYTE IsFloat ;
	BYTE ChannelNum ;
	BYTE BitDepth ;
	DWORD RedMask, GreenMask, BlueMask, AlphaMask ;
} ;

// ��{�摜�f�[�^���c�c�r�摜�Ƃ��ĕۑ�����
extern int NS_SaveBaseImageToDds( const TCHAR *FilePath, const BASEIMAGE *BaseImage )
{
#ifdef UNICODE
	return SaveBaseImageToDds_WCHAR_T(
		FilePath, BaseImage
	) ;
#else
	int Result ;

	TCHAR_TO_WCHAR_T_STRING_ONE_BEGIN( FilePath, return -1 )

	Result = SaveBaseImageToDds_WCHAR_T(
		UseFilePathBuffer, BaseImage
	) ;

	TCHAR_TO_WCHAR_T_STRING_END( FilePath )

	return Result ;
#endif
}

// ��{�摜�f�[�^���c�c�r�摜�Ƃ��ĕۑ�����
extern int SaveBaseImageToDds_WCHAR_T( const wchar_t *FilePath, const BASEIMAGE *BaseImage )
{
	int Ret = -1 ;
	DWORD_PTR fp ;
	DDSFILEHEADER DDSHead ;

	_MEMSET( &DDSHead, 0, sizeof( DDSHead ) ) ;

	// �}�W�b�N�i���o�[�wDDS �x�̃Z�b�g
	( ( BYTE * )&DDSHead.dwMagic )[ 0 ] = 'D' ;
	( ( BYTE * )&DDSHead.dwMagic )[ 1 ] = 'D' ;
	( ( BYTE * )&DDSHead.dwMagic )[ 2 ] = 'S' ;
	( ( BYTE * )&DDSHead.dwMagic )[ 3 ] = ' ' ;

	// �w�b�_�̃T�C�Y���Z�b�g
	DDSHead.dwSize = 124 ;

	// �t���O���Z�b�g
	DDSHead.dwFlags  = BI_DDSD_CAPS | BI_DDSD_HEIGHT | BI_DDSD_WIDTH | BI_DDSD_PITCH | BI_DDSD_PIXELFORMAT ;
	DDSHead.dwCaps = BI_DDSCAPS_TEXTURE ;

	// ���ƍ������Z�b�g
	DDSHead.dwWidth  = ( DWORD )BaseImage->Width ;
	DDSHead.dwHeight = ( DWORD )BaseImage->Height ;

	// �s�b�`���Z�b�g
	DDSHead.dwPitchOrLinearSize = ( DWORD )( BaseImage->Width * BaseImage->ColorData.PixelByte ) ;

	// �s�N�Z���t�H�[�}�b�g�̃T�C�Y���Z�b�g
	DDSHead.dwPfSize = 32 ;

	// Float�^�C�v���ǂ����ŏ����𕪊�
	if( BaseImage->ColorData.FloatTypeFlag )
	{
		// Float�^�C�v�̏ꍇ

		// �t���O���Z�b�g
		DDSHead.dwPfFlags = BI_DDPF_FOURCC ;

		// �^�C�v�ɂ����� FourCC ���Z�b�g����
		switch( BaseImage->ColorData.ChannelBitDepth )
		{
		case 16 :
			switch( BaseImage->ColorData.ChannelNum )
			{
			case 1 :
				DDSHead.dwFourCC = 111 ;
				break ;

			case 2 :
				DDSHead.dwFourCC = 112 ;
				break ;

			case 4 :
				DDSHead.dwFourCC = 113 ;
				break ;

			default :
				DXST_ERRORLOG_ADDUTF16LE( "\x24\xff\x24\xff\x33\xff\xbb\x30\xfc\x30\xd6\x30\xa8\x30\xe9\x30\xfc\x30\x1a\xff\x5e\x97\xfe\x5b\xdc\x5f\x6e\x30\x66\x00\x6c\x00\x6f\x00\x61\x00\x74\x00\x8b\x57\xc1\x30\xe3\x30\xf3\x30\xcd\x30\xeb\x30\x70\x65\x67\x30\x59\x30\x0a\x00\x00"/*@ L"�c�c�r�Z�[�u�G���[�F��Ή���float�^�`�����l�����ł�\n" @*/ ) ; goto END ;
				break ;
			}
			break ;

		case 32 :
			switch( BaseImage->ColorData.ChannelNum )
			{
			case 1 :
				DDSHead.dwFourCC = 114 ;
				break ;

			case 2 :
				DDSHead.dwFourCC = 115 ;
				break ;

			case 4 :
				DDSHead.dwFourCC = 116 ;
				break ;

			default :
				DXST_ERRORLOG_ADDUTF16LE( "\x24\xff\x24\xff\x33\xff\xbb\x30\xfc\x30\xd6\x30\xa8\x30\xe9\x30\xfc\x30\x1a\xff\x5e\x97\xfe\x5b\xdc\x5f\x6e\x30\x66\x00\x6c\x00\x6f\x00\x61\x00\x74\x00\x8b\x57\xc1\x30\xe3\x30\xf3\x30\xcd\x30\xeb\x30\x70\x65\x67\x30\x59\x30\x0a\x00\x00"/*@ L"�c�c�r�Z�[�u�G���[�F��Ή���float�^�`�����l�����ł�\n" @*/ ) ; goto END ;
				break ;
			}
			break ;

		default :
			DXST_ERRORLOG_ADDUTF16LE( "\x24\xff\x24\xff\x33\xff\xbb\x30\xfc\x30\xd6\x30\xa8\x30\xe9\x30\xfc\x30\x1a\xff\x5e\x97\xfe\x5b\xdc\x5f\x6e\x30\x66\x00\x6c\x00\x6f\x00\x61\x00\x74\x00\x8b\x57\xd3\x30\xc3\x30\xc8\x30\x70\x65\x67\x30\x59\x30\x0a\x00\x00"/*@ L"�c�c�r�Z�[�u�G���[�F��Ή���float�^�r�b�g���ł�\n" @*/ ) ; goto END ;
			break ;
		}
	}
	else
	// �����^��1�`�����l���̃r�b�g����16�̏ꍇ�͏����𕪊�
	if( BaseImage->ColorData.ChannelBitDepth == 16 )
	{
		// �t���O���Z�b�g
		DDSHead.dwPfFlags = BI_DDPF_FOURCC ;

		// �^�C�v�ɂ����� FourCC ���Z�b�g����
		switch( BaseImage->ColorData.ChannelNum )
		{
		case 1 :
			DDSHead.dwFourCC = 81 ;
			break ;

		case 2 :
			DDSHead.dwFourCC = 34 ;
			break ;

		case 4 :
			DDSHead.dwFourCC = 36 ;
			break ;

		default :
			DXST_ERRORLOG_ADDUTF16LE( "\x24\xff\x24\xff\x33\xff\xbb\x30\xfc\x30\xd6\x30\xa8\x30\xe9\x30\xfc\x30\x1a\xff\x5e\x97\xfe\x5b\xdc\x5f\x6e\x30\x74\x65\x70\x65\x8b\x57\x31\x00\xc1\x30\xe3\x30\xf3\x30\xcd\x30\xeb\x30\x31\x00\x36\x00\xd3\x30\xc3\x30\xc8\x30\x6e\x30\x8b\x57\xc1\x30\xe3\x30\xf3\x30\xcd\x30\xeb\x30\x70\x65\x67\x30\x59\x30\x0a\x00\x00"/*@ L"�c�c�r�Z�[�u�G���[�F��Ή��̐����^1�`�����l��16�r�b�g�̌^�`�����l�����ł�\n" @*/ ) ; goto END ;
			break ;
		}
	}
	else
	{
		// �ʏ�^( �����^�^�C�v�̏ꍇ )

		// �t���O���Z�b�g
		DDSHead.dwPfFlags = BI_DDPF_RGB ;

		// �A���t�@�r�b�g�����݂���ꍇ�� BI_DDPF_ALPHAPIXELS �� BI_DDSCAPS_ALPHA ���ǉ�
		if( BaseImage->ColorData.AlphaWidth != 0 )
		{
			DDSHead.dwCaps    |= BI_DDSCAPS_ALPHA ;
			DDSHead.dwPfFlags |= BI_DDPF_ALPHAPIXELS ;
		}

		// �r�b�g�����Z�b�g
		DDSHead.dwRGBBitCount = BaseImage->ColorData.ColorBitDepth ;

		// RGBA�̃}�X�N���Z�b�g
		DDSHead.dwRBitMask        = BaseImage->ColorData.RedMask ;
		DDSHead.dwGBitMask        = BaseImage->ColorData.GreenMask ;
		DDSHead.dwBBitMask        = BaseImage->ColorData.BlueMask ;
		DDSHead.dwRGBAlphaBitMask = BaseImage->ColorData.AlphaMask ;
	}

	// �t�@�C�����J��
	WriteOnlyFileAccessDelete( FilePath ) ;
	fp = WriteOnlyFileAccessOpen( FilePath ) ;
	if( fp == 0 ) 
	{
		DXST_ERRORLOG_ADDUTF16LE( "\x24\xff\x24\xff\x33\xff\xbb\x30\xfc\x30\xd6\x30\x28\x75\xd5\x30\xa1\x30\xa4\x30\xeb\x30\x6e\x30\xaa\x30\xfc\x30\xd7\x30\xf3\x30\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�c�c�r�Z�[�u�p�t�@�C���̃I�[�v���Ɏ��s���܂���\n" @*/ ) ; goto END ;
	}

	// �w�b�_�̏����o��
	WriteOnlyFileAccessWrite( fp, &DDSHead, 128 ) ;

	// �C���[�W�̏����o��
	{
		int i ;
		BYTE *Image ;
		DWORD LineSize ;

		Image = ( BYTE * )BaseImage->GraphData ;
		LineSize = ( DWORD )( BaseImage->Width * BaseImage->ColorData.PixelByte ) ;
		for( i = 0 ; i < BaseImage->Height ; i ++, Image += BaseImage->Pitch )
		{
			WriteOnlyFileAccessWrite( fp, Image, LineSize ) ;
		}
	}

	// �I��
	WriteOnlyFileAccessClose( fp ) ;

	Ret = 0 ;
END:

	// ����I��
	return Ret ;
}

extern int NS_SaveBaseImageToJpeg( const TCHAR *pFilePath, BASEIMAGE *BaseImage, int Quality, int Sample2x1 )
{
#ifndef DX_NON_JPEGREAD
#ifdef UNICODE
	return SaveBaseImageToJpegBase( ( const char * )pFilePath, NULL, BaseImage, Quality, Sample2x1 ) ;
#else
	return SaveBaseImageToJpegBase( NULL, pFilePath, BaseImage, Quality, Sample2x1 ) ;
#endif
#else
	return -1 ;
#endif
}

extern int SaveBaseImageToJpeg_WCHAR_T( const wchar_t *pFilePath, BASEIMAGE *BaseImage, int Quality, int Sample2x1 )
{
#ifndef DX_NON_JPEGREAD
	return SaveBaseImageToJpegBase( ( const char * )pFilePath, NULL, BaseImage, Quality, Sample2x1 ) ;
#else
	return -1 ;
#endif
}

extern int NS_SaveBaseImageToPng( const TCHAR *pFilePath, BASEIMAGE *BaseImage, int CompressionLevel )
{
#ifndef DX_NON_PNGREAD
#ifdef UNICODE
	return SaveBaseImageToPngBase( ( const char * )pFilePath, NULL, BaseImage, CompressionLevel ) ;
#else
	return SaveBaseImageToPngBase( NULL, pFilePath, BaseImage, CompressionLevel ) ;
#endif
#else
	return -1 ;
#endif
}

extern int SaveBaseImageToPng_WCHAR_T( const wchar_t *pFilePath, BASEIMAGE *BaseImage, int CompressionLevel )
{
#ifndef DX_NON_PNGREAD
	return SaveBaseImageToPngBase( ( const char * )pFilePath, NULL, BaseImage, CompressionLevel ) ;
#else
	return -1 ;
#endif
}

#endif // DX_NON_SAVEFUNCTION

// �o�^����Ă���e��O���t�B�b�N���[�_�֐�����A�a�l�o�f�[�^�������� GraphImage�f�[�^���\�z����
extern int CreateGraphImageOrDIBGraph_UseGParam(
	LOADBASEIMAGE_GPARAM *GParam,
	const wchar_t *FileName,
	const void *DataImage,
	int DataImageSize,
	int DataImageType,
	int BmpFlag,
	int ReverseFlag,
	BASEIMAGE *BaseImage,
	BITMAPINFO **BmpInfo,
	void **GraphData
)
{
	DWORD_PTR fp = 0 ;
	int hr ;
	BASEIMAGE GraphI ;
	STREAMDATA Src ;

	// �o�[�W�����S�̓ǂݍ��݊֐��Ɋ|����
	{
		if( DataImageType == LOADIMAGE_TYPE_FILE )
		{
			fp = GParam->StreamDataShred2.Open( FileName, FALSE, TRUE, FALSE ) ;
			if( fp == 0 )
			{
				return -1 ;
			}

			Src.DataPoint = ( void * )fp ;
			Src.ReadShred = GParam->FileStreamDataShred ;
		}
		else
		{
			Src.DataPoint = MemStreamOpen( DataImage, DataImageSize ) ;
			if( Src.DataPoint == NULL )
			{
				return -1 ;
			}
			Src.ReadShred = GParam->MemStreamDataShred ;
		}

		hr = CreateGraphImageType2_UseGParam( &GParam->CreateGraphImageType2GParam, &Src, &GraphI ) ;
		if( hr == 0 )
		{
			if( DataImageType != LOADIMAGE_TYPE_FILE )
				STCLOSE( &Src ) ;
			goto END ;
		}
	}
	// �����ɂ�����ǂݍ��߂Ȃ������Ƃ������ƂȂ̂ŃG���[
	goto ERR ;

END :
	// ���]�t���O�������Ă�����O���t�B�b�N�𔽓]������
	if( ReverseFlag == TRUE )
	{
		// �摜�t�H�[�}�b�g�� DX_BASEIMAGE_FORMAT_NORMAL �ȊO�������ꍇ�� DX_BASEIMAGE_FORMAT_NORMAL �ɕϊ�����
		NS_ConvertNormalFormatBaseImage( &GraphI ) ;

		// ���]
		NS_ReverseGraphImage( &GraphI ) ;
	}

	// ������Z�ς݃A���t�@�摜�ւ̕ϊ��̃t���O�������Ă�����ϊ�����
	if( GParam->ConvertPremultipliedAlpha )
	{
		// ���`�����l���������A���A���t�@�e�X�g���L���ȏꍇ�͓��ߐF������^�����ɂ���
		if( GraphI.ColorData.AlphaWidth == 0 && GParam->AlphaTestImageCreateFlag == TRUE )
		{
			DWORD i ;
			DWORD j ;
			DWORD *p ;
			DWORD TransColor ;

			// �t�H�[�}�b�g���W���t�H�[�}�b�g�ł͂Ȃ�������W���t�H�[�}�b�g�ɕϊ�����
			if( GraphI.ColorData.Format != DX_BASEIMAGE_FORMAT_NORMAL )
			{
				if( NS_ConvertNormalFormatBaseImage( &GraphI ) < 0 )
					return -1 ;
			}

			// ARGB8 �ȊO�̏ꍇ�� ARGB8 �ɕϊ�����
			if( GraphI.ColorData.ColorBitDepth != 32 ||
				GraphI.ColorData.PixelByte != 4 ||
				GraphI.ColorData.FloatTypeFlag != FALSE ||
				GraphI.ColorData.NoneMask  != 0x00000000 ||
				GraphI.ColorData.AlphaMask != 0xff000000 ||
				GraphI.ColorData.RedMask   != 0x00ff0000 ||
				GraphI.ColorData.GreenMask != 0x0000ff00 ||
				GraphI.ColorData.BlueMask  != 0x000000ff )
			{
				BASEIMAGE TempBaseImage ;
				TempBaseImage = GraphI ;
				NS_CreateARGB8ColorBaseImage( GraphI.Width, GraphI.Height, &GraphI ) ;
				NS_BltBaseImage( 0, 0, &TempBaseImage, &GraphI ) ;
				NS_ReleaseBaseImage( &TempBaseImage ) ;
			}

			TransColor = GParam->TransColor | 0xff000000 ;
			for( i = 0 ; i < ( DWORD )GraphI.Height ; i ++ )
			{
				p = ( DWORD * )( ( BYTE * )GraphI.GraphData + i * GraphI.Pitch ) ;
				for( j = 0 ; j < ( DWORD )GraphI.Width ; j ++, p ++ )
				{
					if( *p == TransColor )
					{
						*p = 0 ;
					}
				}
			}
		}
		else
		{
			// ���`�����l�������ʂɍ݂�ꍇ�͕��ʂɃR���o�[�g
			NS_ConvertPremulAlphaBaseImage( &GraphI ) ; 
		}
	}

	// ���[�h�����f�[�^�̃^�C�v���w��ƈ���Ă�����ϊ����|����
	if( BmpFlag != FALSE )
	{
#ifndef __WINDOWS__
		goto ERR ;
#else
		// �a�l�o�f�[�^ ��~���������ꍇ�� GraphImage �f�[�^�� �a�l�o �f�[�^�ɕϊ�����

		// �摜�t�H�[�}�b�g�� DX_BASEIMAGE_FORMAT_NORMAL �ȊO�������ꍇ�� DX_BASEIMAGE_FORMAT_NORMAL �ɕϊ�����
		NS_ConvertNormalFormatBaseImage( &GraphI ) ;

		// BITMAPINFO �\���̂̏����i�[���郁�����̈�̊m��
		if( ( *BmpInfo = ( BITMAPINFO * )DXALLOC( sizeof( BITMAPINFO ) + sizeof( RGBQUAD ) * 256 ) ) == NULL )
		{
			DXFREE( GraphI.GraphData ) ;
			goto ERR ;
		}

		// �ϊ�
		if( ( hr = NS_ConvGraphImageToBitmap( &GraphI, *BmpInfo, GraphData, FALSE, FALSE ) ) == -1 )
		{
			DXFREE( GraphI.GraphData ) ;
			DXFREE( *BmpInfo ) ;
		}

		// �R�s�[���s��ꂽ�ꍇ�͂��Ƃ̃O���t�B�b�N�f�[�^���������
		if( hr == 1 )
		{
			DXFREE( GraphI.GraphData ) ;
		}
#endif // __WINDOWS__
	}
	else
	{
		// �w��ʂ�̃f�[�^��ǂݍ��߂Ă����ꍇ�̓f�[�^���R�s�[���ďI��
		*BaseImage = GraphI ;
	}

	// �����t�@�C���C���[�W�̏ꍇ�̓t�@�C�������
	if( DataImageType == LOADIMAGE_TYPE_FILE ) GParam->StreamDataShred2.Close( fp ) ;

	// �I��
	return 0 ;

ERR:
	// �����t�@�C���C���[�W�̏ꍇ�̓t�@�C�������
	if( DataImageType == LOADIMAGE_TYPE_FILE )
	{
		GParam->StreamDataShred2.Close( fp ) ;
	}
	else
	{
		STCLOSE( &Src ) ;
	}

	return -1 ;
}

// �o�^����Ă���e��O���t�B�b�N���[�_�֐�����A�a�l�o�f�[�^�������� GraphImage�f�[�^���\�z����
extern int NS_CreateGraphImageOrDIBGraph(
	const TCHAR *FileName,
	const void *DataImage, int DataImageSize, int DataImageType,
	int BmpFlag, int ReverseFlag,
	 BASEIMAGE *BaseImage, BITMAPINFO **BmpInfo, void **GraphData
)
{
#ifdef UNICODE
	return CreateGraphImageOrDIBGraph_WCHAR_T(
		FileName,
		DataImage, DataImageSize, DataImageType,
		BmpFlag, ReverseFlag,
		BaseImage, BmpInfo, GraphData
	) ;
#else
	int Result ;

	TCHAR_TO_WCHAR_T_STRING_ONE_BEGIN( FileName, return -1 )

	Result = CreateGraphImageOrDIBGraph_WCHAR_T(
		UseFileNameBuffer,
		DataImage, DataImageSize, DataImageType,
		BmpFlag, ReverseFlag,
		BaseImage, BmpInfo, GraphData
	) ;

	TCHAR_TO_WCHAR_T_STRING_END( FileName )

	return Result ;
#endif
}

// �o�^����Ă���e��O���t�B�b�N���[�_�֐�����A�a�l�o�f�[�^�������� GraphImage�f�[�^���\�z����
extern int CreateGraphImageOrDIBGraph_WCHAR_T(
	const wchar_t *FileName,
	const void *DataImage, int DataImageSize, int DataImageType,
	int BmpFlag, int ReverseFlag,
	 BASEIMAGE *BaseImage, BITMAPINFO **BmpInfo, void **GraphData
)
{
	LOADBASEIMAGE_GPARAM GParam ;

	InitLoadBaseImageGParam( &GParam, FALSE ) ;
	return CreateGraphImageOrDIBGraph_UseGParam(
		&GParam,
		FileName,
		DataImage, DataImageSize, DataImageType,
		BmpFlag, ReverseFlag,
		BaseImage, BmpInfo, GraphData
	) ;
}

// �摜�t�@�C���̉��s�N�Z�����Əc�s�N�Z�������擾����
extern int NS_GetImageSize_File( const TCHAR *FileName, int *SizeX, int *SizeY )
{
#ifdef UNICODE
	return GetImageSize_File_WCHAR_T( FileName, SizeX, SizeY ) ;
#else
	int Result ;

	TCHAR_TO_WCHAR_T_STRING_ONE_BEGIN( FileName, return -1 )

	Result = GetImageSize_File_WCHAR_T( UseFileNameBuffer, SizeX, SizeY ) ;

	TCHAR_TO_WCHAR_T_STRING_END( FileName )

	return Result ;
#endif
}

// �摜�t�@�C���̉��s�N�Z�����Əc�s�N�Z�������擾����
extern int GetImageSize_File_WCHAR_T( const wchar_t *FileName, int *SizeX, int *SizeY )
{
	LOADBASEIMAGE_GPARAM GParam ;
	BASEIMAGE BaseImage ;
	int Result ;

	InitLoadBaseImageGParam( &GParam, TRUE ) ;
	Result = CreateGraphImageOrDIBGraph_UseGParam(
		&GParam,
		FileName,
		NULL, 0, LOADIMAGE_TYPE_FILE,
		FALSE, FALSE,
		&BaseImage, NULL, NULL
	) ;
	if( Result < 0 )
	{
		return -1 ;
	}

	if( SizeX )
	{
		*SizeX = BaseImage.Width ;
	}

	if( SizeY )
	{
		*SizeY = BaseImage.Height ;
	}

	return 0 ;
}

// ��������ɓW�J���ꂽ�摜�t�@�C���̉��s�N�Z�����Əc�s�N�Z�������擾����
extern int NS_GetImageSize_Mem( const void *FileImage, int FileImageSize, int *SizeX, int *SizeY )
{
	LOADBASEIMAGE_GPARAM GParam ;
	BASEIMAGE BaseImage ;
	int Result ;

	InitLoadBaseImageGParam( &GParam, TRUE ) ;
	Result = CreateGraphImageOrDIBGraph_UseGParam(
		&GParam,
		NULL,
		FileImage, FileImageSize, LOADIMAGE_TYPE_MEM,
		FALSE, FALSE,
		&BaseImage, NULL, NULL
	) ;
	if( Result < 0 )
	{
		return -1 ;
	}

	if( SizeX )
	{
		*SizeX = BaseImage.Width ;
	}

	if( SizeY )
	{
		*SizeY = BaseImage.Height ;
	}

	return 0 ;
}

// CreateGraphImageType2 �̃O���[�o���ϐ��ɃA�N�Z�X���Ȃ��o�[�W����
extern int CreateGraphImageType2_UseGParam( CREATEBASEIMAGETYPE2_GPARAM *GParam, STREAMDATA *Src, BASEIMAGE *Dest )
{
	int i ;

	// �o�͐��돉����
	_MEMSET( Dest, 0, sizeof( BASEIMAGE ) ) ;

	// �悸���[�U�[��`�֐��Ɋ��܂���
	for( i = 0 ; i < GParam->UserImageLoadFuncNum4 ; i ++ )
	{
		STSEEK( Src, 0, STREAM_SEEKTYPE_SET ) ;
		_MEMSET( Dest, 0, sizeof( BASEIMAGE ) ) ;
		if( GParam->UserImageLoadFunc4[i]( Src, Dest ) == 0 ) break ;
	}
	// �ǂݍ��߂Ă�����I��
	if( i != GParam->UserImageLoadFuncNum4 )
	{
		return 0 ;
	}

	// ���ˑ��̃f�t�H���g�֐������܂���
	for( i = 0 ; DefaultImageLoadFunc_PF[ i ] != NULL ; i ++ )
	{
		STSEEK( Src, 0, STREAM_SEEKTYPE_SET ) ;
		_MEMSET( Dest, 0, sizeof( BASEIMAGE ) ) ;
		if( DefaultImageLoadFunc_PF[ i ]( Src, Dest, GParam->GetFormatOnly ) == 0 ) break ;
	}
	// �ǂݍ��߂Ă�����I��
	if( DefaultImageLoadFunc_PF[ i ] != NULL )
	{
		return 0 ;
	}

	// ���Ƀf�t�H���g�֐������܂���
	for( i = 0 ; DefaultImageLoadFunc[i] != NULL ; i ++ )
	{
		STSEEK( Src, 0, STREAM_SEEKTYPE_SET ) ;
		_MEMSET( Dest, 0, sizeof( BASEIMAGE ) ) ;
		if( DefaultImageLoadFunc[i]( Src, Dest, GParam->GetFormatOnly ) == 0 ) break ;
	}

	// �I��
	return DefaultImageLoadFunc[i] == NULL ? -1 : 0 ;
}

// �ėp�ǂݍ��ݏ����ɂ��O���t�B�b�N�C���[�W�\�z�֐�( 0:����  -1:���s )
extern int NS_CreateGraphImageType2( STREAMDATA *Src, BASEIMAGE *Dest )
{
	CREATEBASEIMAGETYPE2_GPARAM GParam ;

	InitCreateBaseImageType2GParam( &GParam, FALSE ) ;

	return CreateGraphImageType2_UseGParam( &GParam, Src, Dest ) ;
}

#ifdef __WINDOWS__

// �c�h�a�O���t�B�b�N���쐬����
extern HBITMAP NS_CreateDIBGraph( const TCHAR *FileName, int ReverseFlag, COLORDATA *SrcColor )
{
	// �o�[�W�����Q�ɓn��
	return NS_CreateDIBGraphVer2( FileName, NULL, 0, LOADIMAGE_TYPE_FILE, ReverseFlag, SrcColor ) ;
}

// �c�h�a�O���t�B�b�N���쐬����
extern HBITMAP CreateDIBGraph_WCHAR_T( const wchar_t *FileName, int ReverseFlag, COLORDATA *SrcColor )
{
	// �o�[�W�����Q�ɓn��
	return CreateDIBGraphVer2_WCHAR_T( FileName, NULL, 0, LOADIMAGE_TYPE_FILE, ReverseFlag, SrcColor ) ;
}

// �c�h�a�O���t�B�b�N���쐬����(�o�[�W�����Q)
extern HBITMAP NS_CreateDIBGraphVer2( const TCHAR *FileName, const void *MemImage, int MemImageSize, int ImageType, int ReverseFlag, COLORDATA *SrcColor )
{
#ifdef UNICODE
	return CreateDIBGraphVer2_WCHAR_T(
		FileName, MemImage, MemImageSize, ImageType, ReverseFlag, SrcColor
	) ;
#else
	HBITMAP Result ;

	TCHAR_TO_WCHAR_T_STRING_ONE_BEGIN( FileName, return NULL )

	Result = CreateDIBGraphVer2_WCHAR_T(
		UseFileNameBuffer, MemImage, MemImageSize, ImageType, ReverseFlag, SrcColor
	) ;

	TCHAR_TO_WCHAR_T_STRING_END( FileName )

	return Result ;
#endif
}

// �c�h�a�O���t�B�b�N���쐬����(�o�[�W�����Q)
extern HBITMAP CreateDIBGraphVer2_WCHAR_T( const wchar_t *FileName, const void *MemImage, int MemImageSize, int ImageType, int ReverseFlag, COLORDATA *SrcColor )
{
	void *GraphData = NULL ;
	BITMAPINFO *BmpInfo = NULL ;
	HBITMAP Bmp ;

	// �t�@�C���̓ǂݍ���
	if( CreateGraphImageOrDIBGraph_WCHAR_T( FileName, MemImage, MemImageSize, ImageType, TRUE, FALSE, NULL, &BmpInfo, &GraphData ) == -1 )
	{
		return NULL ;
	}

	// HBITMAP �̍쐬
	Bmp = NS_CreateDIBGraphToMem( BmpInfo, GraphData, ReverseFlag, SrcColor ) ;

	// �f�[�^�̉��
	DXFREE( GraphData ) ;
	DXFREE( BmpInfo ) ;

	// �n���h����Ԃ�
	return Bmp ;
}

#endif // __WINDOWS__

// �t���J���[�`����BITMAPINFO�\���̂��쐬����
extern int NS_CreateBmpInfo( BITMAPINFO *BmpInfo, int Width, int Height, int Pitch, const void *SrcGrData, void **DestGrData )
{
	BYTE *DestData, *SrcData ;
	int i ;
	int DPitch, SPitch ;

	// �s�b�`�쐬
	SPitch = ( Pitch == 0 ) ? Width * 3 : Pitch ;
	DPitch = Width * 3 ;
	DPitch = ( ( DPitch + 3 ) / 4 ) * 4 ;

	// �L���X�g
	SrcData = ( BYTE * )SrcGrData ;

	// �O���t�B�b�N�f�[�^�𕡐�
	if( ( *DestGrData = ( BYTE *)DXCALLOC( ( size_t )( Height * DPitch ) ) ) == NULL ) return -1 ;
	DestData = ( BYTE * )*DestGrData ;
	if( DPitch == SPitch )
	{
		_MEMCPY( DestData, SrcData, ( size_t )( Height * DPitch ) ) ;
	}
	else
	{
		for( i = 0 ; i < Height ; i ++, SrcData += SPitch, DestData += DPitch )
			_MEMCPY( DestData, SrcData, ( size_t )SPitch ) ;
	}

	// BITMAPINFO�\���̂̃Z�b�g
	_MEMSET( BmpInfo, 0, sizeof( *BmpInfo ) ) ;
	BmpInfo->bmiHeader.biSize	= sizeof( BITMAPINFOHEADER ) ;
	BmpInfo->bmiHeader.biWidth	= Width ;
	BmpInfo->bmiHeader.biHeight	= -Height ;
	BmpInfo->bmiHeader.biPlanes	= 1 ;
	BmpInfo->bmiHeader.biBitCount= 24 ;

	// �I��
	return 0 ;
}

// BASEIMAGE �\���̂̉摜��񂩂�w��̍��W�̃t���J���[�R�[�h���擾����
extern unsigned int NS_GetGraphImageFullColorCode( const BASEIMAGE *GraphImage, int x, int y )
{
	BYTE *gp ;
	const COLORDATA * cr ;
	unsigned int code, src = 0 ;

	if( GraphImage->GraphData == NULL ) return 0 ;
	
	cr = &GraphImage->ColorData ;
	gp = (BYTE *)GraphImage->GraphData + GraphImage->Pitch * y + cr->PixelByte * x ;

	switch( cr->PixelByte )
	{
	case 2 : src = *((WORD *)gp) ; break ;
	case 3 : src = *((DWORD *)gp) & 0xffffff ; break ;
	case 4 : src = *((DWORD *)gp) ; break ;
	}

	// �t���J���[�̐F�R�[�h���쐬	
	code  = ( ( src & cr->RedMask   ) >> cr->RedLoc   ) << ( 16 + ( 8 - cr->RedWidth   ) ) ;
	code |= ( ( src & cr->GreenMask ) >> cr->GreenLoc ) << (  8 + ( 8 - cr->GreenWidth ) ) ;
	code |= ( ( src & cr->BlueMask  ) >> cr->BlueLoc  ) << (  0 + ( 8 - cr->BlueWidth  ) ) ;
	if( cr->AlphaWidth != 0 ) code |= ( ( src & cr->AlphaMask ) >> cr->AlphaLoc ) << ( 24 + ( 8 - cr->AlphaWidth ) ) ;

	// �F�R�[�h��Ԃ�
	return code ;
}

// �A���t�@�}�X�N�p�̃t�@�C���p�X���쐬����
static int CreateAlphaMaskFilePath( const wchar_t *Path, wchar_t *Dest, size_t BufferBytes )
{
	int   i, j ;
	int   LastPoint ;
	DWORD CharCode ;
	int   CharBytes ;

	// ��Ԍ��� . �̈ʒu�𒲂ׂ�
	LastPoint = -1 ;
	i = 0 ;
	for(;;)
	{
		CharCode = GetCharCode( ( const char * )&( ( BYTE * )Path )[ i ], WCHAR_T_CHARCODEFORMAT, &CharBytes ) ;
		if( CharCode == '\0' )
		{
			break ;
		}

		if( CharCode == '.' )
		{
			LastPoint = i ;
		}

		i += CharBytes ;
	}

	// �A���t�@�}�X�N�p�t�@�C���l�[���̍쐬
	i = 0 ;
	for(;;)
	{
		CharCode = GetCharCode( ( const char * )&( ( BYTE * )Path )[ i ], WCHAR_T_CHARCODEFORMAT, &CharBytes ) ;
		if( CharCode == '\0' || i == LastPoint )
		{
			break ;
		}

		PutCharCode( CharCode, WCHAR_T_CHARCODEFORMAT, ( char * )&( ( BYTE * )Dest )[ i ], BufferBytes - i ) ;
		i += CharBytes ;
	}

	j = i ;
	j += PutCharCode( '_', WCHAR_T_CHARCODEFORMAT, ( char * )&( ( BYTE * )Dest )[ j ], BufferBytes - j ) ;
	j += PutCharCode( 'a', WCHAR_T_CHARCODEFORMAT, ( char * )&( ( BYTE * )Dest )[ j ], BufferBytes - j ) ;
	_WCSCPY_S( ( wchar_t * )&( ( BYTE * )Dest )[ j ], BufferBytes - j, ( const wchar_t * )&( ( BYTE * )Path )[ i ] ) ;

	return 0 ;
}

static int CreateAlphaMaskFilePathT( const TCHAR *Path, TCHAR *Dest, size_t BufferBytes )
{
	int   i, j ;
	int   LastPoint ;
	DWORD CharCode ;
	int   CharBytes ;

	// ��Ԍ��� . �̈ʒu�𒲂ׂ�
	LastPoint = -1 ;
	i = 0 ;
	for(;;)
	{
		CharCode = GetCharCode( ( const char * )&( ( BYTE * )Path )[ i ], _TCHARCODEFORMAT, &CharBytes ) ;
		if( CharCode == '\0' )
		{
			break ;
		}

		if( CharCode == '.' )
		{
			LastPoint = i ;
		}

		i += CharBytes ;
	}

	// �A���t�@�}�X�N�p�t�@�C���l�[���̍쐬
	i = 0 ;
	for(;;)
	{
		CharCode = GetCharCode( ( const char * )&( ( BYTE * )Path )[ i ], _TCHARCODEFORMAT, &CharBytes ) ;
		if( CharCode == '\0' || i == LastPoint )
		{
			break ;
		}

		PutCharCode( CharCode, _TCHARCODEFORMAT, ( char * )&( ( BYTE * )Dest )[ i ], BufferBytes - i ) ;
		i += CharBytes ;
	}

	j = i ;
	j += PutCharCode( '_', _TCHARCODEFORMAT, ( char * )&( ( BYTE * )Dest )[ j ], BufferBytes - j ) ;
	j += PutCharCode( 'a', _TCHARCODEFORMAT, ( char * )&( ( BYTE * )Dest )[ j ], BufferBytes - j ) ;
	_TSTRCPY_S( ( TCHAR * )&( ( BYTE * )Dest )[ j ], BufferBytes - j, ( const TCHAR * )&( ( BYTE * )Path )[ i ] ) ;

	return 0 ;
}

// CreateGraphImage_plus_Alpha �̃O���[�o���ϐ��ɃA�N�Z�X���Ȃ��o�[�W����
extern int CreateGraphImage_plus_Alpha_UseGParam(
	LOADBASEIMAGE_GPARAM *GParam,
	const wchar_t *FileName,
	const void *RgbImage,
	int RgbImageSize,
	int RgbImageType,
	const void *AlphaImage,
	int AlphaImageSize,
	int AlphaImageType,
	BASEIMAGE *RgbGraphImage,
	BASEIMAGE *AlphaGraphImage,
	int ReverseFlag
)
{
	int Result ;

	// �q�f�a�C���[�W�̓ǂݍ���
	if( CreateGraphImageOrDIBGraph_UseGParam( GParam, FileName, RgbImage, RgbImageSize, RgbImageType, FALSE, ReverseFlag, RgbGraphImage, NULL, NULL ) == -1 )
		return -1 ;

	if( AlphaGraphImage != NULL )
	{
		switch( AlphaImageType )
		{
		case LOADIMAGE_TYPE_MEM :
			// ��������̃A���t�@�}�b�v�C���[�W�̓ǂݍ���

			// �ǂݍ��݁B���s������ RGB �����擾�ł����Ƃ������ƁA���������� Alpha ���擾�ł����Ƃ�������
			Result = CreateGraphImageOrDIBGraph_UseGParam( GParam, NULL, AlphaImage, AlphaImageSize, AlphaImageType, FALSE, ReverseFlag, AlphaGraphImage, NULL, NULL ) == -1 ? 1 : 0 ;
			return Result ;

		case LOADIMAGE_TYPE_FILE :
			// �t�@�C����̃A���t�@�}�b�v�C���[�W�̓ǂݍ���
			wchar_t AlphaName[FILEPATH_MAX] ;
			CreateAlphaMaskFilePath( FileName, AlphaName, sizeof( AlphaName ) ) ;

			// �ǂݍ��݁B���s������ RGB �����擾�ł����Ƃ������ƁA���������� Alpha ���擾�ł����Ƃ�������
			Result = CreateGraphImageOrDIBGraph_UseGParam( GParam, AlphaName, NULL, 0, AlphaImageType, FALSE, ReverseFlag, AlphaGraphImage, NULL, NULL ) == -1 ? 1 : 0 ;
			return Result ;
		}
	}

	// �����֗����Ƃ������Ƃ� RGB �����擾�ł����Ƃ�������
	return 1 ;
}

// �e��O���t�B�b�N�f�[�^����O���t�B�b�N�C���[�W�f�[�^�ƃA���t�@�}�b�v�p�C���[�W�f�[�^���\�z����
// Ret:  0:RGB��Alpha�ǂ�����擾  1:RGB�̂ݎ擾  -1:�ǂ�����擾�ł��Ȃ�����
extern int NS_CreateGraphImage_plus_Alpha(
	const TCHAR *FileName,
	const void *RgbImage,
	int RgbImageSize,
	int RgbImageType,
	const void *AlphaImage,
	int AlphaImageSize,
	int AlphaImageType,
	BASEIMAGE *RgbGraphImage,
	BASEIMAGE *AlphaGraphImage,
	int ReverseFlag
)
{
#ifdef UNICODE
	return CreateGraphImage_plus_Alpha_WCHAR_T(
		FileName,
		RgbImage,
		RgbImageSize,
		RgbImageType,
		AlphaImage,
		AlphaImageSize,
		AlphaImageType,
		RgbGraphImage,
		AlphaGraphImage,
		ReverseFlag
	) ;
#else
	int Result ;

	TCHAR_TO_WCHAR_T_STRING_ONE_BEGIN( FileName, return -1 )

	Result = CreateGraphImage_plus_Alpha_WCHAR_T(
		UseFileNameBuffer,
		RgbImage,
		RgbImageSize,
		RgbImageType,
		AlphaImage,
		AlphaImageSize,
		AlphaImageType,
		RgbGraphImage,
		AlphaGraphImage,
		ReverseFlag
	) ;

	TCHAR_TO_WCHAR_T_STRING_END( FileName )

	return Result ;
#endif
}

// �e��O���t�B�b�N�f�[�^����O���t�B�b�N�C���[�W�f�[�^�ƃA���t�@�}�b�v�p�C���[�W�f�[�^���\�z����
// Ret:  0:RGB��Alpha�ǂ�����擾  1:RGB�̂ݎ擾  -1:�ǂ�����擾�ł��Ȃ�����
extern int CreateGraphImage_plus_Alpha_WCHAR_T(
	const wchar_t *FileName,
	const void *RgbImage,
	int RgbImageSize,
	int RgbImageType,
	const void *AlphaImage,
	int AlphaImageSize,
	int AlphaImageType,
	BASEIMAGE *RgbGraphImage,
	BASEIMAGE *AlphaGraphImage,
	int ReverseFlag
)
{
	LOADBASEIMAGE_GPARAM GParam ;

	InitLoadBaseImageGParam( &GParam, FALSE ) ;
	return CreateGraphImage_plus_Alpha_UseGParam(
		&GParam,
		FileName,
		RgbImage, RgbImageSize, RgbImageType,
		AlphaImage, AlphaImageSize, AlphaImageType,
		RgbGraphImage, AlphaGraphImage, ReverseFlag
	) ;
}








// �w��� GraphImage �����E���]����
extern int NS_ReverseGraphImage( BASEIMAGE *GraphImage )
{
	int height, width, pitch, i ;
	DWORD tmp ;
	
	if( GraphImage->Width == 0 || GraphImage->Height == 0 ) return -1 ;
	
	switch( GraphImage->ColorData.PixelByte )
	{
	case 1 :
		{
			BYTE *bufL, *bufR ;
			
			bufL = (BYTE *)GraphImage->GraphData ;
			bufR = (BYTE *)GraphImage->GraphData + GraphImage->Width - 1 ;
			height = GraphImage->Height ;
			width = GraphImage->Width / 2 ;
			pitch = GraphImage->Pitch ;
			do
			{
				for( i = 0 ; i < width ; i ++ )
				{
					tmp = bufL[i] ;
					bufL[i] = bufR[-i] ;
					bufR[-i] = (BYTE)tmp ;
				}
				bufL += pitch ;
				bufR += pitch ;
			}while( -- height ) ;
		}
		break ;
		
	case 2 :
		{
			WORD *bufL, *bufR ;
			
			bufL = (WORD *)GraphImage->GraphData ;
			bufR = (WORD *)GraphImage->GraphData + GraphImage->Width - 1 ;
			height = GraphImage->Height ;
			width = GraphImage->Width / 2 ;
			pitch = GraphImage->Pitch ;
			do
			{
				for( i = 0 ; i < width ; i ++ )
				{
					tmp = bufL[i] ;
					bufL[i] = bufR[-i] ;
					bufR[-i] = (WORD)tmp ;
				}
				bufL = (WORD *)( (BYTE *)bufL + pitch ) ;
				bufR = (WORD *)( (BYTE *)bufR + pitch ) ;
			}while( -- height ) ;
		}
		break ;

	case 3 :
		{
			BYTE *bufL, *bufR ;
			
			bufL = (BYTE *)GraphImage->GraphData ;
			bufR = (BYTE *)GraphImage->GraphData + ( GraphImage->Width - 1 ) * 3 ;
			height = GraphImage->Height ;
			width = ( GraphImage->Width / 2 ) * 3 ;
			pitch = GraphImage->Pitch ;
			do
			{
				for( i = 0 ; i < width ; i += 3 )
				{
					tmp = *((WORD *)(&bufL[i])) ;
					*((WORD *)(&bufL[i])) = *((WORD *)(&bufR[-i])) ;
					*((WORD *)(&bufR[-i])) = (WORD)tmp ;
					tmp = *((BYTE *)(&bufL[i+2])) ;
					*((BYTE *)(&bufL[i+2])) = *((BYTE *)(&bufR[-i+2])) ;
					*((BYTE *)(&bufR[-i+2])) = (BYTE)tmp ;
				}
				bufL += pitch ;
				bufR += pitch ;
			}while( -- height ) ;
		}
		break ;

	case 4 :
		{
			DWORD *bufL, *bufR ;
			
			bufL = (DWORD *)GraphImage->GraphData ;
			bufR = (DWORD *)GraphImage->GraphData + GraphImage->Width - 1 ;
			height = GraphImage->Height ;
			width = GraphImage->Width / 2 ;
			pitch = GraphImage->Pitch ;
			do
			{
				for( i = 0 ; i < width ; i ++ )
				{
					tmp = bufL[i] ;
					bufL[i] = bufR[-i] ;
					bufR[-i] = tmp ;
				}
				bufL = (DWORD *)( (BYTE *)bufL + pitch ) ;
				bufR = (DWORD *)( (BYTE *)bufR + pitch ) ;
			}while( -- height ) ;
		}
		break ;
	}

	// �I��
	return 0 ;	
}





#ifdef __WINDOWS__

// �c�h�a�O���t�B�b�N���쐬����
extern int NS_CreateDIBGraphVer2_plus_Alpha( const TCHAR *FileName, const void *MemImage, int MemImageSize, const void *AlphaImage, int AlphaImageSize, int ImageType, HBITMAP *RGBBmp, HBITMAP *AlphaBmp, int ReverseFlag, COLORDATA *SrcColor )
{
#ifdef UNICODE
	return CreateDIBGraphVer2_plus_Alpha_WCHAR_T(
		FileName, MemImage, MemImageSize, AlphaImage, AlphaImageSize, ImageType, RGBBmp, AlphaBmp, ReverseFlag, SrcColor
	) ;
#else
	int Result ;

	TCHAR_TO_WCHAR_T_STRING_ONE_BEGIN( FileName, return -1 )

	Result = CreateDIBGraphVer2_plus_Alpha_WCHAR_T(
		UseFileNameBuffer, MemImage, MemImageSize, AlphaImage, AlphaImageSize, ImageType, RGBBmp, AlphaBmp, ReverseFlag, SrcColor
	) ;

	TCHAR_TO_WCHAR_T_STRING_END( FileName )

	return Result ;
#endif
}

// �c�h�a�O���t�B�b�N���쐬����
extern int CreateDIBGraphVer2_plus_Alpha_WCHAR_T( const wchar_t *FileName, const void *MemImage, int MemImageSize, const void *AlphaImage, int AlphaImageSize, int ImageType, HBITMAP *RGBBmp, HBITMAP *AlphaBmp, int ReverseFlag, COLORDATA *SrcColor )
{
	int Result = -1 ;

	// �C���[�W�^�C�v�ɂ���ď����𕪊�
	switch( ImageType )
	{
	case LOADIMAGE_TYPE_FILE :		// �t�@�C���C���[�W�̏ꍇ
		Result = CreateDIBGraph_plus_Alpha_WCHAR_T( FileName, RGBBmp, AlphaBmp, ReverseFlag, SrcColor ) ;		// �o�[�W�����P�ɓ�����
		break ;

	case LOADIMAGE_TYPE_MEM :		// �������C���[�W�̏ꍇ
		*RGBBmp = CreateDIBGraphVer2_WCHAR_T( NULL, MemImage, MemImageSize, LOADIMAGE_TYPE_MEM, ReverseFlag, SrcColor ) ;
		if( *RGBBmp == NULL )
		{
			return -1 ;
		}
		if( AlphaBmp != NULL && AlphaImage != NULL && AlphaImageSize != 0 )
		{
			*AlphaBmp = CreateDIBGraphVer2_WCHAR_T( NULL, AlphaImage, AlphaImageSize, LOADIMAGE_TYPE_MEM, ReverseFlag, SrcColor ) ;
		}
		Result = 0 ;
		break ;
	}

	// �I��
	return Result ;
}

// �c�h�a�O���t�B�b�N���������C���[�W����쐬����
extern HBITMAP NS_CreateDIBGraphToMem( const BITMAPINFO *BmpInfo, const void *GraphData, int ReverseFlag, COLORDATA *SrcColor )
{
	HDC hdc ;
	HBITMAP Bmp ; 
	BITMAP bm ;
	void *Data ;
	int SrcWidthByte ;
//	HRESULT hr ;
//	void *GData ;

	// �s�b�`�̎Z�o
	{
		int Byte ; 
		
		Byte = BmpInfo->bmiHeader.biWidth * BmpInfo->bmiHeader.biBitCount / 8 ;

		SrcWidthByte = Byte + ( Byte % 4 != 0 ? ( 4 - Byte % 4 ) : 0 ) ;
	}

	// �c�h�a�O���t�B�b�N�̍쐬
	{
		hdc = CreateCompatibleDC( NULL ) ;

		// �c�h�a�f�[�^���쐬����
		{
			BITMAPINFO *BmpInfo2 ;
			int PlusByte = 0 ;
//			PlusByte = BmpInfo->bmiHeader.biBitCount <= 8 ? sizeof( RGBQUAD ) * ( 1 << BmpInfo->bmiHeader.biBitCount ) : 
//						( BmpInfo->bmiHeader.biBitCount == 16 ? 4 * 3 : 0 ) ;
			if( SrcColor != NULL &&
				( SrcColor->ColorBitDepth == 16 || ( SrcColor->ColorBitDepth > 24 && BmpInfo->bmiHeader.biBitCount != 24 ) ) )
			{
				PlusByte = 3 * 4 ;
			}
			else
			if( BmpInfo->bmiHeader.biBitCount <= 8 )
			{
				PlusByte = ( 1 << BmpInfo->bmiHeader.biBitCount ) * ( int )sizeof( RGBQUAD ) ;
			}
			else
			{
				PlusByte = 0 ;
			}

			if( ( BmpInfo2 = ( BITMAPINFO * )DXALLOC( sizeof( BITMAPINFOHEADER ) + PlusByte ) ) == NULL )
			{
				DXST_ERRORLOG_ADDUTF16LE( "\xe1\x30\xe2\x30\xea\x30\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x20\x00\x69\x00\x6e\x00\x20\x00\x43\x00\x72\x00\x65\x00\x61\x00\x74\x00\x65\x00\x44\x00\x49\x00\x42\x00\x47\x00\x72\x00\x61\x00\x70\x00\x68\x00\x20\x00\x0a\x00\x00"/*@ L"�������̊m�ۂɎ��s���܂��� in CreateDIBGraph \n" @*/ ) ;	

				// �������̉��
				DeleteDC( hdc ) ;
				return NULL ;
			}

			// BITMAPINFOHEADER ��ݒ�
			_MEMSET( BmpInfo2, 0, sizeof( BITMAPINFOHEADER ) ) ;
			BmpInfo2->bmiHeader.biSize	= sizeof( BITMAPINFOHEADER ) ;
			BmpInfo2->bmiHeader.biWidth  = BmpInfo->bmiHeader.biWidth ;
			BmpInfo2->bmiHeader.biHeight = -_ABS(BmpInfo->bmiHeader.biHeight) ;
			BmpInfo2->bmiHeader.biPlanes = 1 ;

			if( SrcColor == NULL || ( SrcColor->ColorBitDepth == 16 && ( BASEIM.LowLoadFlag == TRUE || NS_GetGraphDataShavedMode() != DX_SHAVEDMODE_NONE ) ) )
			{
				if( BmpInfo->bmiHeader.biBitCount == 8 )
				{
					BmpInfo2->bmiHeader.biBitCount    = BmpInfo->bmiHeader.biBitCount ;
					BmpInfo2->bmiHeader.biCompression = BmpInfo->bmiHeader.biCompression ;
					_MEMCPY( BmpInfo2->bmiColors, BmpInfo->bmiColors, sizeof( RGBQUAD ) * 256 ) ;
				}
				else
				{
					BmpInfo2->bmiHeader.biBitCount = 24 ;
					BmpInfo2->bmiHeader.biCompression = BI_RGB ;
					if( SrcColor != NULL ) NS_CreateFullColorData( SrcColor ) ;
				}
			}
			else
			{
				if( SrcColor != NULL )
				{
					DWORD *Mask ;

	/*				if( SrcColor->ColorBitDepth != 24 )
					{
						Mask = ( DWORD * )( ( BYTE * )BmpInfo2 + sizeof( BITMAPINFOHEADER ) ) ;
						Mask[0] = SrcColor->RedMask ;
						Mask[1] = SrcColor->GreenMask ;
						Mask[2] = SrcColor->BlueMask ;
						BmpInfo2->bmiHeader.biCompression = BI_BITFIELDS ;
					}
					else
					{
						BmpInfo2->bmiHeader.biCompression = BI_RGB ;
					}
	*/
					if( SrcColor->ColorBitDepth == 16 ||
						( SrcColor->ColorBitDepth > 24 && BmpInfo->bmiHeader.biBitCount != 24 ) )
					{
						Mask = ( DWORD * )( ( BYTE * )BmpInfo2 + sizeof( BITMAPINFOHEADER ) ) ;
						Mask[0] = SrcColor->RedMask ;
						Mask[1] = SrcColor->GreenMask ;
						Mask[2] = SrcColor->BlueMask ;
						BmpInfo2->bmiHeader.biCompression = BI_BITFIELDS ;
					}
					else
					{
						if( SrcColor->ColorBitDepth == 32 )
						{
							BmpInfo2->bmiHeader.biBitCount = 24 ;
							NS_CreateFullColorData( SrcColor ) ;
						}

						BmpInfo2->bmiHeader.biCompression = BI_RGB ;
					}

					BmpInfo2->bmiHeader.biBitCount = SrcColor->ColorBitDepth ;
				}
				else
				{
					BmpInfo2->bmiHeader.biBitCount = 24 ;
					BmpInfo2->bmiHeader.biCompression = BI_RGB ;
				}
			}

//			_MEMCPY( BmpInfo2, BmpInfo, sizeof( BITMAPINFOHEADER ) + PlusByte ) ;
			if( ( Bmp = CreateDIBSection( hdc, BmpInfo2, DIB_RGB_COLORS, &Data, NULL, 0 ) ) == NULL )
			{
				DXST_ERRORLOG_ADDUTF16LE( "\x24\xff\x29\xff\x22\xff\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x33\x00\x0a\x00\x00"/*@ L"�c�h�a�̍쐬�Ɏ��s���܂���3\n" @*/ ) ;	

				// �������̉��
				DXFREE( BmpInfo2 ) ;
				DeleteDC( hdc ) ;
				return NULL ;
			}

			DXFREE( BmpInfo2 ) ;
		}

		// �r�b�g�}�b�v�̏����擾
		GetObject( Bmp, sizeof( bm ), &bm ) ;
		bm.bmWidthBytes = ( ( bm.bmWidthBytes + 3 ) / 4 ) * 4 ;

		// �O���t�B�b�N�f�[�^���R�s�[����
		{
			if( Data == NULL )
			{
				DXST_ERRORLOG_ADDUTF16LE( "\x24\xff\x29\xff\x22\xff\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x32\x00\x0a\x00\x00"/*@ L"�c�h�a�̍쐬�Ɏ��s���܂���2\n" @*/ ) ;
				return NULL ;
			}
			
			{
				HBITMAP OldBmp ;

				OldBmp = ( HBITMAP )SelectObject( hdc, ( HGDIOBJ )Bmp ) ;

				if( ReverseFlag )
				{
					StretchDIBits( hdc,
									                             0,                                  0,
									    BmpInfo->bmiHeader.biWidth,        BmpInfo->bmiHeader.biHeight, 
									BmpInfo->bmiHeader.biWidth - 1,                                  0,
									   -BmpInfo->bmiHeader.biWidth, _ABS(BmpInfo->bmiHeader.biHeight) ,
									GraphData, BmpInfo, DIB_RGB_COLORS, SRCCOPY ) ;
				}
				else
				{
					SetDIBitsToDevice( hdc, 0, 0, ( DWORD )BmpInfo->bmiHeader.biWidth, ( DWORD )_ABS(BmpInfo->bmiHeader.biHeight) ,
											0 , 0 ,
											0, ( UINT )_ABS(BmpInfo->bmiHeader.biHeight), GraphData, BmpInfo, DIB_RGB_COLORS ) ;
				}

				SelectObject( hdc, ( HGDIOBJ )OldBmp ) ;

			}
		}

		DeleteDC( hdc ) ;
	}

	// �a�l�o�n���h����Ԃ�
	return Bmp ;
}

// �t�@�C������c�h�a�O���t�B�b�N�ƃ}�X�N�O���t�B�b�N���쐬����
extern int NS_CreateDIBGraph_plus_Alpha( const TCHAR *FileName, HBITMAP *RGBBmp, HBITMAP *AlphaBmp, int ReverseFlag, COLORDATA *SrcColor )
{
#ifdef UNICODE
	return CreateDIBGraph_plus_Alpha_WCHAR_T(
		FileName, RGBBmp, AlphaBmp, ReverseFlag, SrcColor
	) ;
#else
	int Result ;

	TCHAR_TO_WCHAR_T_STRING_ONE_BEGIN( FileName, return -1 )

	Result = CreateDIBGraph_plus_Alpha_WCHAR_T(
		UseFileNameBuffer, RGBBmp, AlphaBmp, ReverseFlag, SrcColor
	) ;

	TCHAR_TO_WCHAR_T_STRING_END( FileName )

	return Result ;
#endif
}

// �t�@�C������c�h�a�O���t�B�b�N�ƃ}�X�N�O���t�B�b�N���쐬����
extern int CreateDIBGraph_plus_Alpha_WCHAR_T( const wchar_t *FileName, HBITMAP *RGBBmp, HBITMAP *AlphaBmp, int ReverseFlag, COLORDATA *SrcColor )
{
	wchar_t AlphaFileName[ 256 ] ;

	// �O���t�B�b�N�̓ǂ݂���
	*RGBBmp = CreateDIBGraph_WCHAR_T( FileName, ReverseFlag, SrcColor ) ;
	if( *RGBBmp == NULL )
	{
		return -1 ;
	}

	// �A���t�@�}�X�N�̓ǂ݂���
	if( AlphaBmp )
	{
		CreateAlphaMaskFilePath( FileName, AlphaFileName, sizeof( AlphaFileName ) ) ;
		*AlphaBmp = CreateDIBGraph_WCHAR_T( AlphaFileName, ReverseFlag, SrcColor ) ;
	}

	// �I��
	return 0 ;
}

// �a�l�o �� GraphImage �ɕϊ�����( Ret 0:����I��  1:�R�s�[���s����  -1:�G���[ )
extern int NS_ConvBitmapToGraphImage( const BITMAPINFO *BmpInfo, void *GraphData, BASEIMAGE *GraphImage, int CopyFlag )
{
	const BITMAPINFOHEADER *BInfo = &BmpInfo->bmiHeader ;

	// �o�͐��돉����
	_MEMSET( GraphImage, 0, sizeof( BASEIMAGE ) ) ;

	// �w�b�_�����Z�b�g����
	GraphImage->Width = BInfo->biWidth ;
	GraphImage->Height = _ABS( BInfo->biHeight ) ;

	// �p���b�g�J���[��������A���k����Ă����蔽�]�r�b�g�}�b�v�������肷��ꍇ�͂Q�S�r�b�g�J���[�ւ̕ϊ��������s��
	if( BmpInfo->bmiHeader.biHeight >= 0 || 
		BInfo->biBitCount <= 8 || ( BInfo->biBitCount != 16 && BInfo->biBitCount != 24 && BInfo->biBitCount != 32 ) ||
		( BInfo->biCompression != BI_RGB && BInfo->biCompression != BI_BITFIELDS ) )
	{
		HDC hdc ;
		HBITMAP Bmp ;
		BITMAP bm ;
		int Height ;
		void *Data ;

		// �ϊ������Ɏg�p����c�b�̍쐬
		hdc = CreateCompatibleDC( NULL ) ;

		// �Q�S�r�b�g�J���[�c�h�a�f�[�^���쐬����
		{
			BITMAPINFO BmpInfo2 ;

			// �r�b�g�}�b�v�C���t�H�w�b�_��ݒ�
			_MEMSET( &BmpInfo2, 0, sizeof( BITMAPINFOHEADER ) ) ;
			BmpInfo2.bmiHeader.biSize	= sizeof( BITMAPINFOHEADER ) ;
			BmpInfo2.bmiHeader.biWidth  = BmpInfo->bmiHeader.biWidth ;
			BmpInfo2.bmiHeader.biHeight = -_ABS(BmpInfo->bmiHeader.biHeight) ;
			BmpInfo2.bmiHeader.biPlanes = 1 ;
			BmpInfo2.bmiHeader.biBitCount = 24 ;
			BmpInfo2.bmiHeader.biCompression = BI_RGB ;

			Bmp = CreateDIBSection( hdc, &BmpInfo2, DIB_RGB_COLORS, &Data, NULL, 0 ) ;
			if( Bmp == NULL || Data == NULL )
			{
				DXST_ERRORLOG_ADDUTF16LE( "\x24\xff\x29\xff\x22\xff\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x33\x00\x0a\x00\x00"/*@ L"�c�h�a�̍쐬�Ɏ��s���܂���3\n" @*/ ) ;	
				DeleteDC( hdc ) ;
				return -1 ;
			}
		}

		// �r�b�g�}�b�v�̏����擾
		GetObject( Bmp, sizeof( bm ), &bm ) ;
		bm.bmWidthBytes += bm.bmWidthBytes % 4 ? 4 - bm.bmWidthBytes % 4 : 0 ;

		// �O���t�B�b�N�f�[�^�Q�S�r�b�g�ɕϊ����R�s�[����
		Height = _ABS(bm.bmHeight) ;
		{
			HBITMAP OldBmp ;

			OldBmp = ( HBITMAP )SelectObject( hdc, ( HGDIOBJ )Bmp ) ;
			SetDIBitsToDevice( hdc, 0, 0, ( DWORD )BmpInfo->bmiHeader.biWidth, ( DWORD )_ABS(BmpInfo->bmiHeader.biHeight) ,
									0 , 0 ,
									0, ( UINT )_ABS(BmpInfo->bmiHeader.biHeight), GraphData, BmpInfo, DIB_RGB_COLORS ) ;
			SelectObject( hdc, ( HGDIOBJ )OldBmp ) ;
		}

		// �s�v�ɂȂ����c�b�̍폜
		DeleteDC( hdc ) ;

		// �Q�S�r�b�g�J���[�������O���t�B�b�N���i�[���郁�����̈�̊m��
		GraphImage->GraphData = DXALLOC( ( size_t )( Height * bm.bmWidthBytes ) ) ;
		if( GraphImage->GraphData == NULL )
		{
			DXST_ERRORLOG_ADDUTF16LE( "\x12\xff\x14\xff\xd3\x30\xc3\x30\xc8\x30\xab\x30\xe9\x30\xfc\x30\x16\x53\x57\x30\x5f\x30\xb0\x30\xe9\x30\xd5\x30\xa3\x30\xc3\x30\xaf\x30\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�Q�S�r�b�g�J���[�������O���t�B�b�N���i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ ) ;	
			return -1 ;
		}
		
		// �Q�S�r�b�g�J���[�������O���t�B�b�N�̃f�[�^���p�̗̈�ɓ]��
		_MEMCPY( GraphImage->GraphData, bm.bmBits, ( size_t )( Height * bm.bmWidthBytes ) ) ;

		// �ϊ��p�ɍ쐬�����c�h�a�̍폜
		DeleteObject( Bmp ) ;
		
		// �t���J���[�p�̃J���[�����Z�b�g����
		NS_CreateFullColorData( &GraphImage->ColorData ) ;
		
		// �s�b�`���Z�b�g
		GraphImage->Pitch = bm.bmWidthBytes ;
		
		// �R�s�[�t���O�𗧂Ă�
		CopyFlag = TRUE ;
	}
	else
	// ���k�^�C�v���r�b�g�t�B�[���h�������ꍇ�͐�p�̏������s��
	if( BInfo->biCompression == BI_BITFIELDS ) 
	{
		unsigned int *Mask ;
		COLORDATA * Color ;
		
		Mask = (unsigned int *)BmpInfo->bmiColors ;
		Color = &GraphImage->ColorData ;

		// �J���[�r�b�g�[�x���Z�b�g
		Color->ColorBitDepth = ( unsigned char )BInfo->biBitCount ;
		Color->PixelByte     = ( unsigned char )( Color->ColorBitDepth / 8 ) ;

		// �s�b�`���Z�o
		GraphImage->Pitch = GraphImage->Width * Color->PixelByte ;
		GraphImage->Pitch = GraphImage->Pitch + ( GraphImage->Pitch % 4 != 0 ? ( 4 - GraphImage->Pitch % 4 ) : 0 ) ;
		
		// �r�b�g�t�B�[���h�𒲂ׂāA�q�f�a�e�F�̃r�b�g�}�X�N���擾����
		{
			int i, j, w ;
		
			// �q�����𒲂ׂ�
			j = 1 ;
			for( i = 0 ; i < 32 && ( *Mask & j ) == 0 ; i ++, j <<= 1 ){}
			Color->RedLoc = ( unsigned char )i ;
			for( w = 0 ; i < 32 && ( *Mask & j ) != 0 ; i ++, w ++, j <<= 1 ){}
			Color->RedWidth = ( unsigned char )w ;
			Color->RedMask = *Mask ;
			
			Mask ++ ;
			
			// �f�����𒲂ׂ�
			j = 1 ;
			for( i = 0 ; i < 32 && ( *Mask & j ) == 0 ; i ++, j <<= 1 ){}
			Color->GreenLoc = ( unsigned char )i ;
			for( w = 0 ; i < 32 && ( *Mask & j ) != 0 ; i ++, w ++, j <<= 1 ){}
			Color->GreenWidth = ( unsigned char )w ;
			Color->GreenMask = *Mask ;
			
			Mask ++ ;
			
			// �a�����𒲂ׂ�
			j = 1 ;
			for( i = 0 ; i < 32 && ( *Mask & j ) == 0 ; i ++, j <<= 1 ){}
			Color->BlueLoc = ( unsigned char )i ;
			for( w = 0 ; i < 32 && ( *Mask & j ) != 0 ; i ++, w ++, j <<= 1 ){}
			Color->BlueWidth = ( unsigned char )w ;
			Color->BlueMask = *Mask ;
			
			// �A���t�@�����͂Ȃ�
			Color->AlphaWidth = 0 ;
			Color->AlphaLoc = 0 ;
			Color->AlphaMask = 0 ;

			// NoneMask ���Z�b�g����
			NS_SetColorDataNoneMask( Color ) ;
		}
		
		// �R�s�[�t���O�������Ă�����f�[�^���R�s�[����
		if( CopyFlag == TRUE )
		{
			// �R�s�[�����f�[�^���i�[���郁�����̈�̊m��
			GraphImage->GraphData = DXALLOC( ( size_t )( GraphImage->Pitch * GraphImage->Height ) ) ;
			if( GraphImage->GraphData == NULL ) return -1 ;
			
			// �f�[�^�̃R�s�[
			_MEMCPY( GraphImage->GraphData, GraphData, ( size_t )( GraphImage->Pitch * GraphImage->Height ) ) ;
		}
		else
		{
			GraphImage->GraphData = GraphData ;
		}
	}
	else
	// ���k�^�C�v�������k�������ꍇ�͒ʏ�̏������s��
	if( BInfo->biCompression == BI_RGB && ( BInfo->biBitCount == 16 || BInfo->biBitCount == 24 ) ) 
	{
		COLORDATA * Color ;

		Color = &GraphImage->ColorData ;

		// �J���[�r�b�g�[�x���Z�b�g
		Color->ColorBitDepth = ( unsigned char )BInfo->biBitCount ;
		Color->PixelByte     = ( unsigned char )( Color->ColorBitDepth / 8 ) ;

		// �s�b�`���Z�o
		GraphImage->Pitch = GraphImage->Width * Color->PixelByte ;
		GraphImage->Pitch = GraphImage->Pitch + ( GraphImage->Pitch % 4 != 0 ? ( 4 - GraphImage->Pitch % 4 ) : 0 ) ;
		
		// �J���[�r�b�g�[�x���ƂɃJ���[�}�X�N�𖄂߂�
		switch( BInfo->biBitCount )
		{
		case 16 :		// �P�U�r�b�g�J���[�������ꍇ�͂q�T�f�T�a�T
			Color->RedWidth = 5 ; Color->RedLoc = 10 ; Color->RedMask = 0x7c00 ;
			Color->GreenWidth = 5 ; Color->GreenLoc = 5 ; Color->GreenMask = 0x3e0 ;
			Color->BlueWidth = 5 ; Color->BlueLoc = 0 ; Color->BlueMask = 0x1f ;
			break ;
			
		case 24 :		// �Q�S�r�b�g�J���[�������ꍇ�͂q�W�f�W�a�W
			Color->RedWidth = 8 ; Color->RedLoc = 16 ; Color->RedMask = 0xff0000 ;
			Color->GreenWidth = 8 ; Color->GreenLoc = 8 ; Color->GreenMask = 0xff00 ;
			Color->BlueWidth = 8 ; Color->BlueLoc = 0 ; Color->BlueMask = 0xff ;
			break ;
		}

		// �A���t�@�͖���		
		Color->AlphaWidth = 0 ; Color->AlphaLoc = 0 ; Color->AlphaMask = 0 ;

		// NoneMask ���Z�b�g����
		NS_SetColorDataNoneMask( Color ) ;
		
		// �R�s�[�t���O�������Ă�����f�[�^���R�s�[����
		if( CopyFlag == TRUE )
		{
			// �R�s�[�����f�[�^���i�[���郁�����̈�̊m��
			GraphImage->GraphData = DXALLOC( ( size_t )( GraphImage->Pitch * GraphImage->Height ) ) ;
			if( GraphImage->GraphData == NULL ) return -1 ;
			
			// �f�[�^�̃R�s�[
			_MEMCPY( GraphImage->GraphData, GraphData, ( size_t )( GraphImage->Pitch * GraphImage->Height ) ) ;
		}
		else
		{
			GraphImage->GraphData = GraphData ;
		}
	}
	else
	// ���܂ł̏����ɊY�����Ȃ�������G���[
	return -1 ;	
		
	// �R�s�[�������ǂ�����Ԃ�
	return CopyFlag ;
}

// GraphImage �� �a�l�o �ɕϊ�����(�A���t�@�f�[�^�͂����Ă����������)( Ret 0:����I��  1:�R�s�[���s����  -1:�G���[ )
extern int NS_ConvGraphImageToBitmap( const BASEIMAGE *GraphImage, BITMAPINFO *BmpInfo, void **GraphData, int CopyFlag, int FullColorConv )
{
	const COLORDATA *SrcColor ;
	int Pitch, i ;

	// �W�r�b�g�J���[�̏ꍇ
	if( GraphImage->ColorData.ColorBitDepth == 8 && FullColorConv == FALSE )
	{
		// �W�r�b�g�J���[�� BITMAPINFOHEADER �̏����l�߂�
		_MEMSET( BmpInfo, 0, sizeof( *BmpInfo ) ) ;
		BmpInfo->bmiHeader.biSize	= sizeof( BITMAPINFOHEADER ) ;
		BmpInfo->bmiHeader.biWidth	= GraphImage->Width ;
		BmpInfo->bmiHeader.biHeight	= -GraphImage->Height ;
		BmpInfo->bmiHeader.biPlanes	= 1 ;
		BmpInfo->bmiHeader.biBitCount= 8 ;
		BmpInfo->bmiHeader.biCompression = BI_RGB ;

		// �p���b�g�̃R�s�[
		_MEMCPY( BmpInfo->bmiColors, GraphImage->ColorData.Palette, sizeof( DWORD ) * 256 ) ;

		// �s�b�`�̎Z�o
		Pitch = ( ( GraphImage->Width + 3 ) / 4 ) * 4 ;

		// �R�s�[�t���O�������Ă��邩�A�s�b�`���S�̔{���ł͂Ȃ��ꍇ�͓��e���R�s�[����
		if( CopyFlag == TRUE || Pitch != GraphImage->Pitch )
		{
			BYTE *Src, *Dest ;

			// �ϊ������O���t�B�b�N�f�[�^���i�[����o�b�t�@�̊m��
			*GraphData = DXALLOC( ( size_t )( GraphImage->Height * Pitch ) ) ;
			if( *GraphData == NULL ) return -1 ;

			// �f�[�^�̃R�s�[
			Src  = (BYTE *)GraphImage->GraphData ;
			Dest = (BYTE *)*GraphData ; 
			for( i = 0 ; i < GraphImage->Height ; i ++, Src += GraphImage->Pitch, Dest += Pitch )
			{
				_MEMCPY( Dest, Src, ( size_t )GraphImage->Width ) ;
			}
		 							
			// �R�s�[�t���O�𗧂Ă�
			CopyFlag = TRUE ;
		}
		else
		// ����ȊO�̏ꍇ�̓A�h���X�����̂܂܃Z�b�g����
		{
			// �R�s�[�����Ȃ��ꍇ�́A�O���t�B�b�N�f�[�^�̃|�C���^�� GraphData �ɑ�����ďI���
			*GraphData = GraphImage->GraphData ;
		}
	}
	else
	// ����ȊO�̏ꍇ
	{
		// �Q�S�r�b�g�J���[�� BITMAPINFOHEADER �̏����l�߂�
		_MEMSET( BmpInfo, 0, sizeof( *BmpInfo ) ) ;
		BmpInfo->bmiHeader.biSize	= sizeof( BITMAPINFOHEADER ) ;
		BmpInfo->bmiHeader.biWidth	= GraphImage->Width ;
		BmpInfo->bmiHeader.biHeight	= -GraphImage->Height ;
		BmpInfo->bmiHeader.biPlanes	= 1 ;
		BmpInfo->bmiHeader.biBitCount= 24 ;

		// �ϊ���̃s�b�`�̎Z�o
		SrcColor = &GraphImage->ColorData ;
		Pitch = GraphImage->Width * 3 ;
		Pitch = ( ( Pitch + 3 ) / 4 ) * 4 ;

		// �J���[�t�H�[�}�b�g���Q�S�r�b�g�J���[�ȊO�������ꍇ���A�R�s�[�t���O�������Ă���ꍇ�͂Q�S�r�b�g�J���[�ɕϊ�����
		SrcColor = &GraphImage->ColorData ;
		if( SrcColor->ColorBitDepth != 24 || CopyFlag == TRUE || 
			SrcColor->RedMask != 0xff0000 || SrcColor->GreenMask != 0xff00 || SrcColor->BlueMask != 0xff )
		{
			COLORDATA SrcColor ;
			POINT DestPoint ;
			RECT SrcRect ;
		
			// �ϊ������O���t�B�b�N�f�[�^���i�[����o�b�t�@�̊m��
			*GraphData = DXALLOC( ( size_t )( GraphImage->Height * Pitch ) ) ;
			if( *GraphData == NULL ) return -1 ;
			
			// �ϊ���̃t�H�[�}�b�g���Z�b�g
			NS_CreateFullColorData( &SrcColor ) ;
			
			// �ϊ��������s��
			DestPoint.y = DestPoint.x = 0 ;
			SrcRect.left = SrcRect.top = 0 ;
			SrcRect.right = GraphImage->Width ; SrcRect.bottom = GraphImage->Height ;
			NS_GraphColorMatchBltVer2( *GraphData, Pitch, &SrcColor, 
										GraphImage->GraphData, GraphImage->Pitch, &GraphImage->ColorData,
		 								NULL, 0, NULL,
		 								DestPoint, &SrcRect, FALSE,
		 								FALSE, 0,
		 								DX_SHAVEDMODE_NONE ) ;
		 							
			// �R�s�[�t���O�𗧂Ă�
			CopyFlag = TRUE ;
		}
		else
		// �R�s�[�����Ȃ��ꍇ�́A�s�b�`�������Ă��Ȃ��ꍇ�̂݊ȈՓ]���������s��
		if( Pitch != GraphImage->Pitch )
		{
			// �R�s�[�t���O�𗧂Ă�
			CopyFlag = TRUE ;
			
			// �ϊ������O���t�B�b�N�f�[�^���i�[����o�b�t�@�̊m��
			*GraphData = DXALLOC( ( size_t )( GraphImage->Height * Pitch ) ) ;
			if( *GraphData == NULL ) return -1 ;
			
			// �]������
			{
				int i ;
				BYTE *Src, *Dest ;
				
				Dest = ( BYTE * )*GraphData ;
				Src = ( BYTE * )GraphImage->GraphData ;
				for( i = 0 ; i < GraphImage->Height ; i ++, Dest += Pitch, Src += GraphImage->Pitch )
				{
					_MEMCPY( Dest, Src, ( size_t )GraphImage->Pitch ) ;
				}
			}
		}
		else
		// �s�b�`�������Ă���ꍇ�͕ϊ��������s���K�v�͖���
		{
			// �R�s�[�����Ȃ��ꍇ�́A�O���t�B�b�N�f�[�^�̃|�C���^�� GraphData �ɑ�����ďI���
			*GraphData = GraphImage->GraphData ;
		}
	}

	// �I��
	return CopyFlag ;
}

#endif // __WINDOWS__










// ���[�U�[��`�̃O���t�B�b�N���[�h�֐�Ver4��o�^����
extern int NS_AddUserGraphLoadFunction4( int ( *UserLoadFunc )( STREAMDATA *Src, BASEIMAGE *BaseImage ) )
{
	int i ;

	// �����֐����o�^����Ă��Ȃ������ׂ�
	for( i = 0 ; i < MAX_USERIMAGEREAD_FUNCNUM ; i ++ )
	{
		if( BASEIM.UserImageLoadFunc4[ i ] == UserLoadFunc ) break ; 
	}
	if( i != MAX_USERIMAGEREAD_FUNCNUM ) return -1 ;

	// �g���Ă��Ȃ��|�C���^�ϐ���T��
	for( i = 0 ; i < MAX_USERIMAGEREAD_FUNCNUM ; i ++ )
	{
		if( BASEIM.UserImageLoadFunc4[ i ] == NULL ) break ; 
	}
	if( i == MAX_USERIMAGEREAD_FUNCNUM ) return -1 ;

	// �o�^
	BASEIM.UserImageLoadFunc4[ i ] = UserLoadFunc ;

	// ���[�_�[�̐��𑝂₷
	BASEIM.UserImageLoadFuncNum4 ++ ;

	// �I��
	return 0 ;
}

// ���[�U�[��`�̃O���t�B�b�N���[�h�֐�Ver4��o�^���疕������
extern int NS_SubUserGraphLoadFunction4( int ( *UserLoadFunc )( STREAMDATA *Src, BASEIMAGE *BaseImage ) )
{
	int i ;

	// �w��̃O���t�B�b�N���[�h�֐���T��
	for( i = 0 ; i < MAX_USERIMAGEREAD_FUNCNUM ; i ++ )
	{
		if( BASEIM.UserImageLoadFunc4[ i ] == UserLoadFunc ) break ;
	}
	if( i == MAX_USERIMAGEREAD_FUNCNUM ) return -1 ;

	// �f�[�^�𖕏�
	BASEIM.UserImageLoadFunc4[ i ] = NULL ;
	
	// �f�[�^�̐������炷
	BASEIM.UserImageLoadFuncNum4 -- ;

	// �I��
	return 0 ;
}

// 32bit bmp �摜�̂`���������ׂĂO��������`�����𖳎����邩�ǂ����̃t���O���Z�b�g����
extern int SetBmp32AllZeroAlphaToXRGB8( int Flag )
{
	// �t���O��ۑ�����
	BASEIM.Bmp32AllZeroAlphaToXRGB8 = Flag ;

	// �I��
	return 0 ;
}










// �W���摜�ǂݍ��݊֐�

// �a�l�o�摜�̓ǂ݂���
static int LoadBmpImage( STREAMDATA *Stream, BASEIMAGE *BaseImage, int GetFormatOnly )
{
	void				*sp ;
	BYTE				*GData = NULL ;
	BYTE				*Buf = NULL, *DecodeBuf = NULL ;
	STREAMDATASHRED		*sstr ;
	int					Pitch = 0, DataSize ;
	BITMAPFILEHEADER	BmpFileHeader ;
	BITMAPINFO			BmpInfoT, *BmpInfo2 = NULL ;
#ifdef __WINDOWS__
	HDC					hdc = NULL ;
	HBITMAP				Bmp = NULL ;
#endif // __WINDOWS__
//	bool				Palette ;
	BYTE 				*Dest, *DestT, *Src ;
	WORD				Num ;
	int					DestAddPitch, i, Height ;
	int					Loop, Loop2 ;
	size_t				FileSize, ImageSize ;

	sstr = &Stream->ReadShred ;
	sp   = Stream->DataPoint ;

	// �t�@�C���T�C�Y�𓾂�
	sstr->Seek( sp, 0, STREAM_SEEKTYPE_END ) ;
	FileSize = ( size_t )sstr->Tell( sp ) ;
	sstr->Seek( sp, 0, STREAM_SEEKTYPE_SET ) ;
	
	if( sstr->Read( &BmpFileHeader, sizeof( BmpFileHeader ), 1, sp ) <= 0 ) goto ERR ;		// �t�@�C���w�b�_�[������ǂ݂���
	if( _MEMCMP( &BmpFileHeader.bfType, "BM", 2 ) ) goto ERR ;								// �h�c����
	if( sstr->Read( &BmpInfoT, sizeof( BITMAPINFO ), 1, sp ) <= 0 ) goto ERR ;				// BITMAPINFO��ǂ݂���

	// �J���[�r�b�g�����W�ȉ��̎��̓p���b�g��ǂ�
	if( BmpInfoT.bmiHeader.biBitCount <= 8 )
	{
		// �a�l�o�h�m�e�n�\���̂̊i�[�p���������m��
		if( ( BmpInfo2 = ( BITMAPINFO * )DXCALLOC( sizeof( BITMAPINFO ) + 256 * sizeof( RGBQUAD ) ) ) == NULL ) goto ERR ;

		// �a�l�o�h�m�e�n�\���̂̓��e���R�s�[
		_MEMCPY( BmpInfo2, &BmpInfoT, sizeof( BITMAPINFO ) ) ;

		// �c��̃J���[�p���b�g�̓ǂ݂���
		if( ( sstr->Read( ( ( BYTE * )BmpInfo2 ) + sizeof( BITMAPINFO ), 1, ( ( 1 << BmpInfoT.bmiHeader.biBitCount ) - 1 ) * sizeof( RGBQUAD ), sp ) ) <= 0 ) goto ERR ;
	}
	else
	// �J���[�r�b�g�����R�Q���P�U�ŃJ���[�}�X�N�g�p���̏���
	if( BmpInfoT.bmiHeader.biBitCount == 16 || BmpInfoT.bmiHeader.biBitCount == 32 || BmpInfoT.bmiHeader.biCompression == BI_BITFIELDS )
	{
		// �a�l�o�h�m�e�n�\���̂̊i�[�p���������m��
		if( ( BmpInfo2 = ( BITMAPINFO * )DXCALLOC( sizeof( BITMAPINFOHEADER ) + sizeof( RGBQUAD ) * 3 ) ) == NULL ) goto ERR ;

		// �a�l�o�h�m�e�n�g�d�`�c�d�q�\���̂̓��e���R�s�[
		_MEMCPY( BmpInfo2, &BmpInfoT, sizeof( BITMAPINFO ) ) ;

		// �J���[�}�X�N�̓ǂ݂���
		if( BmpInfoT.bmiHeader.biCompression == BI_BITFIELDS )
		{
			if( ( sstr->Read( ( ( BYTE * )BmpInfo2 ) + sizeof( BITMAPINFO ), sizeof( RGBQUAD ) * 2, 1, sp ) ) <= 0 ) goto ERR ;
		}
	}
	else
	// ����ȊO�̏ꍇ�̏���
	{
		// �a�l�o�h�m�e�n�\���̂̊i�[�p���������m��
		if( ( BmpInfo2 = ( BITMAPINFO * )DXCALLOC( sizeof( BITMAPINFO ) ) ) == NULL ) goto ERR ;

		// �a�l�o�h�m�e�n�g�d�`�c�d�q�\���̂̓��e���R�s�[
		_MEMCPY( BmpInfo2, &BmpInfoT, sizeof( BITMAPINFO ) ) ;
	}

	// �t�@�C���|�C���^�𑀍�
	sstr->Seek( sp, BmpFileHeader.bfOffBits, STREAM_SEEKTYPE_SET ) ;

	// �C���[�W�T�C�Y���v�Z����
	{
		Pitch = BmpInfoT.bmiHeader.biWidth * BmpInfoT.bmiHeader.biBitCount / 8 ;
		if( ( BmpInfoT.bmiHeader.biWidth * BmpInfoT.bmiHeader.biBitCount ) % 8 != 0 ) Pitch ++ ;
		Pitch = ( Pitch + 3 ) / 4 * 4 ;
		DataSize = Pitch * _ABS( BmpInfoT.bmiHeader.biHeight ) ;
	}

	// �t�@�C���̎c��̕������������ɓǂݍ���
	if( GetFormatOnly == FALSE )
	{
		ImageSize = ( size_t )( FileSize - sstr->Tell( sp ) ) ;
		
		// �c��̕������i�[���郁�����̈�̊m��
		GData = (BYTE *)DXALLOC( ImageSize ) ;
		if( GData == NULL ) goto ERR ;

		// �ǂݍ���
		if( sstr->Read( GData, 1, ImageSize, sp ) <= 0 ) goto ERR ;
	}

	// �`���ɂ���ď����𕪊�
	if( BmpInfo2->bmiHeader.biCompression == BI_RLE8 )
	{
		// �W�r�b�g�p���b�g���k�`��

		if( GetFormatOnly == FALSE )
		{
			// �f�R�[�h��̃f�[�^���i�[���郁�����̈�̊m��
			DecodeBuf = (BYTE *)DXALLOC( ( size_t )DataSize ) ;	
			if( DecodeBuf == NULL ) goto ERR ;
			_MEMSET( DecodeBuf, 0, ( size_t )DataSize ) ;
			
			// �𓀏���
			if( BmpInfo2->bmiHeader.biHeight < 0 )
			{
				Dest         = DecodeBuf ;
				DestAddPitch = Pitch ;
			}
			else
			{
				Dest         = DecodeBuf + Pitch * ( BmpInfo2->bmiHeader.biHeight - 1 ) ;
				DestAddPitch = -Pitch ;
			}
			Src = GData ;
			
			// �𓀊J�n
			DestT = Dest ;
			for(;;)
			{
				// �P�o�C�g�ڂ̃R�[�h�ɂ���ď����𕪊�
				if( Src[0] != 0 )
				{
					// �A���W�J���[�h

					// �Q�o�C�g�ڂ̒l���P�o�C�g�ڂ̐��l�������W�J�����
					_MEMSET( DestT, Src[1], Src[0] ) ;

					// ���W�����炷
					DestT += Src[0] ;
				}
				else
				{
					// �R�[�h�����[�h

					// �G�X�P�[�v�R�[�h�ɂ���ď����𕪊�
					switch( Src[1] )
					{
					case 0 :	// �O��������s�̏I�[�A�A�h���X��ύX����
						Dest += DestAddPitch ;
						DestT = Dest ;
						break ;

					case 1 :	// �P��������C���[�W�f�[�^�̏I�[
						goto RLE8END ;

					case 2 :	// �Q�������猻�݂̃A�h���X��ύX����
						Dest  += *((char *)&Src[3]) * DestAddPitch ;
						DestT  = Dest + *((char *)&Src[2]) ;
						Src   += 2 ;
						break ;

					default :
						// ����ȊO�̏ꍇ�͔񈳏k�W�J
						_MEMCPY( DestT, Src + 2, Src[1] ) ;

						// �A�h���X��ύX����
						DestT += Src[1] ;
						Src   += Src[1] ;
						Src    = (BYTE *)( ( (DWORD_PTR)Src + 1 ) / 2 * 2 ) ;	// Src �̃A�h���X�͕K���Q�̔{��
						break ;
					}
				}
				
				Src += 2 ;
			}
	RLE8END:;

			DXFREE( GData ) ;
			GData = NULL ;
		}
		
		// BASEIMAGE �̃w�b�_���\�z
		{
			BaseImage->GraphData = GetFormatOnly == FALSE ? DecodeBuf : NULL ;
			BaseImage->Width     = BmpInfo2->bmiHeader.biWidth ;
			BaseImage->Height    = _ABS( BmpInfo2->bmiHeader.biHeight ) ;
			BaseImage->Pitch     = Pitch ;

			// �J���[���̃Z�b�g
			NS_CreatePaletteColorData( &BaseImage->ColorData ) ;
			_MEMCPY( BaseImage->ColorData.Palette, BmpInfo2->bmiColors, 256 * sizeof( RGBQUAD ) ) ;
			for( i = 0 ; i < 256 ; i ++ )
				BaseImage->ColorData.Palette[i].Alpha = 0 ;
		}
	}
	else
	if( BmpInfo2->bmiHeader.biCompression == BI_RLE4 )
	{
		// �S�r�b�g�p���b�g���k�`��
		// �W�r�b�g�p���b�g�`���ɕϊ����Ȃ����

		Pitch    = ( BmpInfo2->bmiHeader.biWidth + 3 ) / 4 * 4 ;
		DataSize = Pitch * _ABS( BmpInfo2->bmiHeader.biHeight ) ;

		if( GetFormatOnly == FALSE )
		{
			// �f�R�[�h��̃f�[�^���i�[���郁�����̈�̊m��
			DecodeBuf = (BYTE *)DXALLOC( ( size_t )DataSize ) ;	
			if( DecodeBuf == NULL ) goto ERR ;
			_MEMSET( DecodeBuf, 0, ( size_t )DataSize ) ;

			// �𓀏���
			if( BmpInfo2->bmiHeader.biHeight < 0 )
			{
				Dest         = DecodeBuf ;
				DestAddPitch = Pitch ;
			}
			else
			{
				Dest         = DecodeBuf + Pitch * ( BmpInfo2->bmiHeader.biHeight - 1 ) ;
				DestAddPitch = -Pitch ;
			}
			Src = GData ;
			
			// �𓀊J�n
			DestT = Dest ;
			for(;;)
			{
				// �P�o�C�g�ڂ̃R�[�h�ɂ���ď����𕪊�
				if( Src[0] != 0 )
				{
					// �A���W�J���[�h

					// �Q�o�C�g�ڂ̒l(�Q�s�N�Z����(���4bit���P�s�N�Z����))��
					// �P�o�C�g�ڂ̐��������W�J�����(�s�N�Z���P��)
					// �܂� 07 13 �Ƃ������� 01 03 01 03 01 03 01 �ƓW�J����� 
					Loop = Src[0] ;
					Num  = ( WORD )( ( ( Src[1] & 0xf0 ) >> 4 ) | ( ( Src[1] & 0x0f ) << 8 ) ) ;
					
					Loop2 = Loop / 2 ;
					if( Loop2 != 0 )
					{
						do{
							*((WORD *)DestT) = Num ;
							DestT += 2 ;
						}while( -- Loop2 ) ;
					}
					
					if( Loop % 2 != 0 )
					{
						DestT[0]   = (BYTE)( Num & 0xff ) ;
						DestT ++ ;
					}
				}
				else
				{
					// �R�[�h�����[�h

					// �G�X�P�[�v�R�[�h�ɂ���ď����𕪊�
					switch( Src[1] )
					{
					case 0 :
						// �O��������s�̏I�[�A�A�h���X��ύX����
						Dest  += DestAddPitch ;
						DestT  = Dest ;
						break ;

					case 1 :
						// �P��������C���[�W�f�[�^�̏I�[
						goto RLE4END ;

					case 2 :
						// �Q�������猻�݂̃A�h���X��ύX����
						Dest  += *((char *)&Src[3]) * DestAddPitch ;
						DestT  = Dest + *((char *)&Src[2]) ;
						Src   += 2 ;
						break ;

					default :
						// ����ȊO�̏ꍇ�͔񈳏k�W�J
						Loop = Src[1] ;
						Src += 2 ;
						Loop2 = Loop / 2 ;
						for( i = 0 ; i < Loop2 ; i ++, DestT += 2, Src ++ )
						{
							DestT[0] = ( BYTE )( ( *Src & 0xf0 ) >> 4 ) ;
							DestT[1] = ( BYTE )( *Src & 0x0f ) ;
						}
						
						if( Loop % 2 != 0 )
						{
							*DestT = ( BYTE )( ( *Src & 0xf0 ) >> 4 ) ;
							Src ++ ;
							DestT ++ ;
						}

						Src = (BYTE *)( ( (DWORD_PTR)Src + 1 ) / 2 * 2 ) ;
						Src -= 2 ;
						break ;
					}
				}
				Src += 2 ;
			}
	RLE4END :;

			DXFREE( GData ) ;
			GData = NULL ;
		}

		// BASEIMAGE �̃w�b�_���\�z
		{
			BaseImage->GraphData = DecodeBuf ;
			BaseImage->Width     = BmpInfo2->bmiHeader.biWidth ;
			BaseImage->Height    = _ABS( BmpInfo2->bmiHeader.biHeight ) ;
			BaseImage->Pitch     = Pitch ;

			// �J���[���̃Z�b�g
			NS_CreatePaletteColorData( &BaseImage->ColorData ) ;
			_MEMCPY( BaseImage->ColorData.Palette, BmpInfo2->bmiColors, 16 * sizeof( RGBQUAD ) ) ;
			for( i = 0 ; i < 16 ; i ++ )
				BaseImage->ColorData.Palette[i].Alpha = 0 ;
		}
	}
	else
	if( BmpInfo2->bmiHeader.biCompression == BI_RGB && BmpInfo2->bmiHeader.biBitCount == 1 )
	{
		// �Q�F�J���[�̏ꍇ�͂Q�T�U�F�J���[�ɕϊ�����
		int nt, qt, NokoriNum, MultiNum, Width, Height, DestPitch ;
		int SrcAddPitch, DestAddPitch ;

		Width     = BmpInfo2->bmiHeader.biWidth ;
		Height    = _ABS( BmpInfo2->bmiHeader.biHeight ) ;
		DestPitch = ( Width + 3 ) / 4 * 4 ;

		if( GetFormatOnly == FALSE )
		{
			Buf = (BYTE *)DXALLOC( ( size_t )( DestPitch * Height ) ) ;
			if( Buf == NULL ) goto ERR ;

			MultiNum  = Width / 8 ;
			NokoriNum = Width - MultiNum * 8 ;

			// �㉺���]���K�v�ȏꍇ�͂��̏����������ł���
			if( BmpInfo2->bmiHeader.biHeight > 0 )
			{
				// �㉺���]����ꍇ
				Src         = (BYTE *)GData + Pitch * ( Height - 1 ) ;
				SrcAddPitch = ( Pitch - Width / 8 ) - Pitch * 2 ;
			}
			else
			{
				// �㉺���]���Ȃ��ꍇ
				SrcAddPitch = Pitch - Width / 8 ;
				Src         = (BYTE *)GData ;
			}
			DestAddPitch = DestPitch - Width ;
			Dest         = Buf ;

			do
			{
				if( MultiNum )
				{
					qt = MultiNum ;
					do
					{
						Dest[ 0 ] = ( BYTE )( ( *Src >> 7 ) & 0x01 ) ;
						Dest[ 1 ] = ( BYTE )( ( *Src >> 6 ) & 0x01 ) ;
						Dest[ 2 ] = ( BYTE )( ( *Src >> 5 ) & 0x01 ) ;
						Dest[ 3 ] = ( BYTE )( ( *Src >> 4 ) & 0x01 ) ;
						Dest[ 4 ] = ( BYTE )( ( *Src >> 3 ) & 0x01 ) ;
						Dest[ 5 ] = ( BYTE )( ( *Src >> 2 ) & 0x01 ) ;
						Dest[ 6 ] = ( BYTE )( ( *Src >> 1 ) & 0x01 ) ;
						Dest[ 7 ] = ( BYTE )( ( *Src      ) & 0x01 ) ;

						Dest += 8 ;
						Src  ++ ;
					}while( --qt ) ;
				}

				if( NokoriNum )
				{
					nt = NokoriNum ;
					for(;;)
					{
						*Dest = ( BYTE )( ( *Src >> 7 ) & 0x01 ) ;
						Dest ++ ;
						if( --nt == 0 ) break ;

						*Dest = ( BYTE )( ( *Src >> 6 ) & 0x01 ) ;
						Dest ++ ;
						if( --nt == 0 ) break ;

						*Dest = ( BYTE )( ( *Src >> 5 ) & 0x01 ) ;
						Dest ++ ;
						if( --nt == 0 ) break ;

						*Dest = ( BYTE )( ( *Src >> 4 ) & 0x01 ) ;
						Dest ++ ;
						if( --nt == 0 ) break ;

						*Dest = ( BYTE )( ( *Src >> 3 ) & 0x01 ) ;
						Dest ++ ;
						if( --nt == 0 ) break ;

						*Dest = ( BYTE )( ( *Src >> 2 ) & 0x01 ) ;
						Dest ++ ;
						if( --nt == 0 ) break ;

						*Dest = ( BYTE )( ( *Src >> 1 ) & 0x01 ) ;
						Dest ++ ;
						if( --nt == 0 ) break ;

						*Dest = ( BYTE )(   *Src        & 0x01 ) ;
						Dest ++ ;
						Src  ++ ;
						if( --nt == 0 ) break ;
					}
				}

				Dest += DestAddPitch ;
				Src  += SrcAddPitch ;
			}while( --Height ) ;

			DXFREE( GData ) ;
			GData = Buf ;
			Buf   = NULL ;
		}

		// BASEIMAGE �̃w�b�_���\�z
		{
			BaseImage->GraphData = GData ;
			BaseImage->Width     = BmpInfo2->bmiHeader.biWidth ;
			BaseImage->Height    = _ABS( BmpInfo2->bmiHeader.biHeight ) ;
			BaseImage->Pitch     = DestPitch ;

			// �J���[���̃Z�b�g
			NS_CreatePaletteColorData( &BaseImage->ColorData ) ;
			BaseImage->ColorData.MaxPaletteNo = 15 ;
			_MEMCPY( BaseImage->ColorData.Palette, BmpInfo2->bmiColors, 2 * sizeof( RGBQUAD ) ) ;
			BaseImage->ColorData.Palette[ 0 ].Alpha = 0 ;
			BaseImage->ColorData.Palette[ 1 ].Alpha = 0 ;
		}
	}
	else
	if( BmpInfo2->bmiHeader.biCompression == BI_RGB && BmpInfo2->bmiHeader.biBitCount == 4 )
	{
		// �P�U�F�J���[�̏ꍇ�͂Q�T�U�F�J���[�ɕϊ�����
	
		int nt, qt, NokoriNum, QwordNum, Width, Height, DestPitch ;
		int SrcAddPitch, DestAddPitch ;
		unsigned char Code[4] ;

		Width     = BmpInfo2->bmiHeader.biWidth ;
		Height    = _ABS( BmpInfo2->bmiHeader.biHeight ) ;
		DestPitch = ( Width + 3 ) / 4 * 4 ;

		if( GetFormatOnly == FALSE )
		{
			Buf = (BYTE *)DXALLOC( ( size_t )( DestPitch * Height ) ) ;
			if( Buf == NULL ) goto ERR ;

			QwordNum  = Width / 8 ;
			NokoriNum = Width - QwordNum * 8 ;

			// �㉺���]���K�v�ȏꍇ�͂��̏����������ł���
			if( BmpInfo2->bmiHeader.biHeight > 0 )
			{
				// �㉺���]����ꍇ
				Src         = (BYTE *)GData + Pitch * ( Height - 1 ) ;
				SrcAddPitch = ( Pitch - Width / 2 ) - Pitch * 2 ;
			}
			else
			{
				// �㉺���]���Ȃ��ꍇ
				SrcAddPitch = Pitch - Width / 2 ;
				Src         = (BYTE *)GData ;
			}
			DestAddPitch = DestPitch - Width ;
			Dest         = Buf ;

			do
			{
				if( QwordNum )
				{
					qt = QwordNum ;
					do
					{
						*((DWORD *)&Code) = ( *((DWORD *)Src) & 0xf0f0f0f0 ) >> 4 ;
						Dest[0] = Code[0] ;
						Dest[2] = Code[1] ;
						Dest[4] = Code[2] ;
						Dest[6] = Code[3] ;

						*((DWORD *)&Code) = *((DWORD *)Src) & 0x0f0f0f0f ;
						Dest[1] = Code[0] ;
						Dest[3] = Code[1] ;
						Dest[5] = Code[2] ;
						Dest[7] = Code[3] ;

						Dest += 8 ;
						Src  += 4 ;
					}while( --qt ) ;
				}

				if( NokoriNum )
				{
					nt = NokoriNum ;
					for(;;)
					{
						*Dest = ( BYTE )( ( *Src & 0xf0 ) >> 4 ) ;
						Dest ++ ;
						if( --nt == 0 ) break ;

						*Dest = ( BYTE )( *Src & 0xf ) ;
						Dest ++ ;
						Src  ++ ;
						if( --nt == 0 ) break ;
					}
				}

				Dest += DestAddPitch ;
				Src += SrcAddPitch ;
			}while( --Height ) ;

			DXFREE( GData ) ;
			GData = Buf ;
			Buf   = NULL ;
		}

		// BASEIMAGE �̃w�b�_���\�z
		{
			BaseImage->GraphData = GData ;
			BaseImage->Width     = BmpInfo2->bmiHeader.biWidth ;
			BaseImage->Height    = _ABS( BmpInfo2->bmiHeader.biHeight ) ;
			BaseImage->Pitch     = DestPitch ;

			// �J���[���̃Z�b�g
			NS_CreatePaletteColorData( &BaseImage->ColorData ) ;
			BaseImage->ColorData.MaxPaletteNo = 15 ;
			_MEMCPY( BaseImage->ColorData.Palette, BmpInfo2->bmiColors, 16 * sizeof( RGBQUAD ) ) ;
			for( i = 0 ; i < 16 ; i ++ )
				BaseImage->ColorData.Palette[i].Alpha = 0 ;
		}
	}
	else
	if( BmpInfo2->bmiHeader.biCompression == BI_RGB &&
		( BmpInfo2->bmiHeader.biBitCount == 32 ||  BmpInfo2->bmiHeader.biBitCount == 24 || BmpInfo2->bmiHeader.biBitCount == 8 ) )
	{
		// �R�Q�r�b�g�J���[�a�l�o���Q�S�r�b�g�t���J���[�a�l�o���W�r�b�g�p���b�g�J���[�̏ꍇ

		if( GetFormatOnly == FALSE )
		{
			// �㉺���]���Ă���ꍇ�͏㉺���]���s��
			if( BmpInfo2->bmiHeader.biHeight > 0 )
			{
				Height = BmpInfo2->bmiHeader.biHeight ;
				Buf    = (BYTE *)DXALLOC( ( size_t )( Pitch * Height ) ) ;
				if( Buf == NULL ) goto ERR ;

				Dest = Buf ;
				Src  = (BYTE *)GData + Pitch * ( Height - 1 ) ;
				for( i = 0 ; i < Height ; i ++, Src -= Pitch, Dest += Pitch )
				{
					_MEMCPY( Dest, Src, ( size_t )Pitch ) ;
				}

				DXFREE( GData ) ;
				GData = Buf ;
				Buf   = NULL ;
			}
			else
			{
				Height = -BmpInfo2->bmiHeader.biHeight ;
			}
		}
		
		// BASEIMAGE �̃w�b�_���\�z
		{
			BaseImage->GraphData = GData ;
			BaseImage->Width     = BmpInfo2->bmiHeader.biWidth ;
			BaseImage->Height    = _ABS(BmpInfo2->bmiHeader.biHeight) ;
			BaseImage->Pitch     = Pitch ;
			
			if( BmpInfo2->bmiHeader.biBitCount == 8 )
			{
				// �W�r�b�g�J���[�̏ꍇ
				
				// �J���[���̃Z�b�g
				NS_CreatePaletteColorData( &BaseImage->ColorData ) ;
				_MEMCPY( BaseImage->ColorData.Palette, BmpInfo2->bmiColors, ( BmpInfo2->bmiHeader.biBitCount == 8 ? 256 : 16 ) * sizeof( RGBQUAD ) ) ;
				for( i = 0 ; i < 256 ; i ++ )
					BaseImage->ColorData.Palette[i].Alpha = 0 ;
			}
			else
			if( BmpInfo2->bmiHeader.biBitCount == 24 )
			{
				// �Q�S�r�b�g�J���[�̏ꍇ
				
				// �t���J���[�p�̃J���[�����Z�b�g����
				NS_CreateFullColorData( &BaseImage->ColorData ) ;
			}
			else
			{
				// �R�Q�r�b�g�J���[�̏ꍇ
				
				// �R�Q�r�b�g�J���[�p�̃J���[�����Z�b�g����
				if( BASEIM.Bmp32AllZeroAlphaToXRGB8 )
				{
					// ���ׂẴA���t�@�l���O��������A���t�@�Ȃ��Ɣ��f����
					DWORD PixelNum ;
					PixelNum = ( DWORD )( BmpInfo2->bmiHeader.biWidth * Height ) ;
					for( i = 0 ; ( DWORD )i < PixelNum ; i ++ )
						if( GData[ i * 4 + 3 ] != 0 ) break ;
					if( ( DWORD )i == PixelNum )
					{
						NS_CreateXRGB8ColorData( &BaseImage->ColorData ) ;
					}
					else
					{
						NS_CreateARGB8ColorData( &BaseImage->ColorData ) ;
					}
				}
				else
				{
					NS_CreateARGB8ColorData( &BaseImage->ColorData ) ;
				}
			}
		}
	}
	else
	{
#ifndef __WINDOWS__
		goto ERR ;

#else // __WINDOWS__

		// ����ȊO�̃t�H�[�}�b�g�̏ꍇ�͂Q�S�r�b�g�J���[�a�l�o�ɕϊ�����

		BITMAP bm ;
		HBITMAP OldBmp ;
		void *Data ;

		if( GetFormatOnly == FALSE )
		{
			// �ϊ������Ɏg�p����c�b�̍쐬
			hdc = CreateCompatibleDC( NULL ) ;

			// �c�h�a�f�[�^���쐬����
			{
				BITMAPINFO BmpInfo3 ;

				// �ϊ���̃r�b�g�}�b�v�C���t�H�w�b�_��ݒ�
				_MEMSET( &BmpInfo3, 0, sizeof( BITMAPINFOHEADER ) ) ;
				BmpInfo3.bmiHeader.biSize        = sizeof( BITMAPINFOHEADER ) ;
				BmpInfo3.bmiHeader.biWidth       = BmpInfo2->bmiHeader.biWidth ;
				BmpInfo3.bmiHeader.biHeight      = -_ABS(BmpInfo2->bmiHeader.biHeight) ;
				BmpInfo3.bmiHeader.biPlanes      = 1 ;
				BmpInfo3.bmiHeader.biBitCount    = 24 ;
				BmpInfo3.bmiHeader.biCompression = BI_RGB ;

				Bmp = CreateDIBSection( hdc, &BmpInfo3, DIB_RGB_COLORS, &Data, NULL, 0 ) ;
				if( Bmp == NULL || Data == NULL )
				{
					DXST_ERRORLOG_ADDUTF16LE( "\x24\xff\x29\xff\x22\xff\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x33\x00\x0a\x00\x00"/*@ L"�c�h�a�̍쐬�Ɏ��s���܂���3\n" @*/ ) ;
					goto ERR ;
				}
			}

			// �r�b�g�}�b�v�̏����擾
			GetObject( Bmp, sizeof( bm ), &bm ) ;
			bm.bmWidthBytes = ( bm.bmWidthBytes + 3 ) / 4 * 4 ;

			// �O���t�B�b�N�f�[�^�Q�S�r�b�g�ɕϊ����R�s�[����
			Height = _ABS( bm.bmHeight ) ;
			OldBmp = ( HBITMAP )SelectObject( hdc, ( HGDIOBJ )Bmp ) ;
			SetDIBitsToDevice(
				hdc,
				0, 0,  ( DWORD )BmpInfo2->bmiHeader.biWidth, ( DWORD )_ABS(BmpInfo2->bmiHeader.biHeight),
				0, 0,                                     0, ( UINT ) _ABS(BmpInfo2->bmiHeader.biHeight),
				GData, BmpInfo2, DIB_RGB_COLORS ) ;
			SelectObject( hdc, ( HGDIOBJ )OldBmp ) ;

			// �s�v�ɂȂ����c�b�̍폜
			DeleteDC( hdc ) ;
			hdc = NULL ;
			
			// �ϊ��O�̃f�[�^�����
			DXFREE( GData ) ;
			GData = NULL ;
			
			// �Q�S�r�b�g�J���[�������O���t�B�b�N���i�[���郁�����̈�̊m��
			GData = (BYTE *)DXALLOC( ( size_t )( Height * bm.bmWidthBytes ) ) ;
			if( GData == NULL )
			{
				DXST_ERRORLOG_ADDUTF16LE( "\x12\xff\x14\xff\xd3\x30\xc3\x30\xc8\x30\xab\x30\xe9\x30\xfc\x30\x16\x53\x57\x30\x5f\x30\xb0\x30\xe9\x30\xd5\x30\xa3\x30\xc3\x30\xaf\x30\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�Q�S�r�b�g�J���[�������O���t�B�b�N���i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ ) ;	
				goto ERR ;
			}
			
			// �Q�S�r�b�g�J���[�������O���t�B�b�N�̃f�[�^���p�̗̈�ɓ]��
			_MEMCPY( GData, bm.bmBits, ( size_t )( Height * bm.bmWidthBytes ) ) ;
			
			// �ϊ��p�ɍ쐬�����c�h�a�̍폜
			DeleteObject( Bmp ) ;
			Bmp = NULL ;

			BaseImage->Pitch     = bm.bmWidthBytes ;
		}
		else
		{
			BaseImage->Pitch     = 0 ;
		}
	
		// BASEIMAGE �̃w�b�_���\�z
		{
			// �w�b�_�����Z�b�g����
			BaseImage->GraphData = GData ;
			BaseImage->Width     = BmpInfo2->bmiHeader.biWidth ;
			BaseImage->Height    = _ABS(BmpInfo2->bmiHeader.biHeight) ;

			// �t���J���[�p�̃J���[�����Z�b�g����
			NS_CreateFullColorData( &BaseImage->ColorData ) ;
		}
#endif // __WINDOWS__
	}

	// BITMAPINFOHEADER �����i�[���Ă��������������
	DXFREE( BmpInfo2 ) ;
	BmpInfo2 = NULL ;

	// �I��
	return 0 ;

ERR :
	if( BmpInfo2 != NULL ) DXFREE( BmpInfo2 ) ;
	if( GData != NULL ) DXFREE( GData ) ;
	if( Buf != NULL ) DXFREE( Buf ) ;
#ifdef __WINDOWS__
	if( Bmp != NULL ) DeleteObject( Bmp ) ;
	if( hdc != NULL ) DeleteDC( hdc ) ;
#endif // __WINDOWS__

	// �G���[�I��
	return -1 ; 
}



// �s�N�Z���̕��ѕ��������咣
typedef struct
{
    union
	{
        BYTE abyIdentifier[4] ;		// "BGRA"
        DWORD dwIdentifier ;		// 0x41524742
    };

	// 0x08080808(32bitColor) or 0x10101010(64bitColor)
    DWORD dwPixelFormat ;
    DWORD dwWidth ;
    DWORD dwHeight ;
} ArgbHeader ;


// �`�q�f�a�摜�̓ǂ݂���
static int LoadArgbImage( STREAMDATA *Src, BASEIMAGE *BaseImage, int GetFormatOnly )
{
	ArgbHeader Head ;
	void *sp ;
	STREAMDATASHRED *sstr ;
	int P64Flag, PixelByte ;
	int Width, Height, Pitch ;
	void *GraphPoint = NULL ;
	BYTE *TempBuf = NULL, *SrcP ;

	sstr = &Src->ReadShred ;
	sp = Src->DataPoint ;

	if( sstr->Read( &Head, sizeof( Head ), 1, sp ) <= 0 ) return -1	;						// �w�b�_�̓ǂݍ���
	if( _MEMCMP( Head.abyIdentifier, "BGRA", 4 ) != 0 ) return -1 ;							// �h�c����
	if( Head.dwPixelFormat != 0x10101010 && Head.dwPixelFormat != 0x08080808 ) return -1 ;	// �s�N�Z���t�H�[�}�b�g�̌���
	P64Flag = Head.dwPixelFormat == 0x10101010 ;

	// �T�C�Y�̎擾
	Width		= ( int )Head.dwWidth ;
	Height		= ( int )Head.dwHeight ;
	PixelByte	= P64Flag ? 8 : 4 ;
	Pitch		= Width * PixelByte ;

	if( GetFormatOnly == FALSE )
	{
		// �O���t�B�b�N���i�[���郁�����̈�̊m��
		GraphPoint = DXALLOC( ( size_t )( Width * Height * 4 ) ) ;
		if( GraphPoint == NULL )
		{
			goto ERR ;
		}
		
		// �O���t�B�b�N�̓ǂݍ���
		if( P64Flag == TRUE )
		{
			unsigned int *GraphP ;
			int i, j ;
		
			// �R�Q�r�b�g�J���[�ɕϊ����Ȃ���ǂݍ���
			TempBuf = ( BYTE * )DXALLOC( ( size_t )Pitch ) ;
			if( TempBuf == NULL ) goto ERR ;
			
			GraphP = ( unsigned int * )GraphPoint ;
			for( i = 0 ; i < Height ; i ++ )
			{
				if( sstr->Read( TempBuf, ( size_t )Pitch, 1, sp ) <= 0 ) goto ERR ;

				SrcP = TempBuf ;
				for( j = 0 ; j < Width ; j ++, GraphP ++, SrcP += 8 )
				{
					*GraphP = ( unsigned int )( SrcP[1] | ( SrcP[3] << 8 ) | ( SrcP[5] << 16 ) | ( SrcP[7] << 24 ) ) ;
	//				*GraphP = ( unsigned int )(
	//							( ( *SrcP & 0xff00000000000000 ) >> 40 ) |
	//							( ( *SrcP & 0xff0000000000 ) >> 24 ) |
	//							( ( *SrcP & 0xff000000 ) >> 16 ) |
	//							( ( *SrcP & 0xff00 ) >> 8 ) ) ;  
				}
			}
			
			// �e���|�����o�b�t�@�����
			DXFREE( TempBuf ) ;
			TempBuf = NULL ;
		}
		else
		{
			// ���̂܂܃������̈�ɓǂݍ���
			if( sstr->Read( GraphPoint, ( size_t )( Width * Height * 4 ), 1, sp ) <= 0 ) goto ERR ;
		}
	}

	// BASEIMAGE �\���̂̏��𖄂߂�
	{
		BaseImage->GraphData = GraphPoint ;
		BaseImage->Width = Width ;
		BaseImage->Height = Height ;
		BaseImage->Pitch = Width * 4 ;

		// �J���[�����Z�b�g����
		NS_CreateFullColorData( &BaseImage->ColorData ) ;
		BaseImage->ColorData.ColorBitDepth = 32 ;
		BaseImage->ColorData.PixelByte = 4 ;
		BaseImage->ColorData.AlphaLoc = 24 ;
		BaseImage->ColorData.AlphaMask = 0xff000000 ;
		BaseImage->ColorData.AlphaWidth = 8 ;
	}

	// �I��
	return 0 ;

ERR :
	if( GraphPoint != NULL ) DXFREE( GraphPoint ) ;
	if( TempBuf != NULL ) DXFREE( TempBuf ) ;

	return -1 ;
}








#ifndef DX_GCC_COMPILE
#pragma pack(push)
#pragma pack(1)
#else
#pragma pack(push,1)
#endif

// �s�f�`�摜�̃w�b�_
typedef struct _tagTGAHEADER
{
	BYTE ImageIDLength ;	// 0:�C���[�W�h�c�̒���(�o�C�g)
	BYTE UseColorMap ;		// 1:�J���[�}�b�v�̗L���@0:����  1:����
	BYTE ImageType ;		// 2:�摜�`�� 
							// 	  0:�C���[�W����
							//    1:�C���f�b�N�X�J���[( 256�F ) �񈳏k
							//	  2:�q�f�a�J���[ �񈳏k
							//	  3:�Q�K���J���[ �񈳏k
							//	  9:�C���f�b�N�X�J���[ RLE���k
							//	  10:�q�f�a�J���[ RLE���k
							//	  11:�Q�K���J���[ RLE���k
	WORD PaletteOrigin ;	// 3:�J���[�p���b�g�̊J�n�ԍ�
	WORD PaletteLength ;	// 5:�J���[�p���b�g�̌�
	BYTE PaletteBitDepth ;	// 7:�J���[�p���b�g��̃r�b�g��( 16:16�r�b�g  24:24�r�b�g  32:32�r�b�g )
	WORD XOrigin ;			// 8:�摜�̔z�u����w���W( �ق�0 ? )
	WORD YOrigin ;			// 10:�摜�̔z�u����x���W( �ق�0 ? )
	WORD Width ;			// 12:�摜�̕�
	WORD Height ;			// 14:�摜�̍���
	BYTE ColorBitDepth ;	// 16:�F�[�x( 16:16�r�b�g  24:24�r�b�g  32:32�r�b�g )
	BYTE Descriptor ;		// 17:���̑��̏��
							//     bit 0�`3 ����( 16bit�摜�Ȃ� 0 ���� 1, 24bit�摜�Ȃ� 0, 32bit�摜�Ȃ� 8 )
							//     bit 4 �i�[����( 0:������E  1:�E���獶 )
							//     bit 5 �i�[����( 0:�������  1:�ォ�牺 )
							//     bit 6, 7  ���g�p
} _TGAHEADER ;

// �s�f�`�摜�̃t�b�^
typedef struct _tagTGAFOOTER
{
	DWORD ExtensionOffset ;		// �g�����̃A�h���X( 0:���� )
	DWORD DeveloperOffset ;		// �J���җ̈�(?)�̃A�h���X( 0:���� )
	char Signature[18] ;		// �t�b�^���ǂ����𒲂ׂ邽�߂̂h�c( "TRUEVISION-XFILE" )
} _TGAFOOTER ;

// �s�f�`�摜�̃^�O
typedef struct _tagTGATAG
{
	WORD  TagNumber ;	// �^�O�ԍ�
	DWORD DataOffset ;	// �s�f�`�̎��f�[�^�̂���A�h���X
	DWORD DataSize ;	// �s�f�`�̎��f�[�^�̃T�C�Y
} _TGATAG ;

// �s�f�`�摜�̊g�����
typedef struct _tagTGAEXTENSION
{
	WORD 		Size ;				// �t�b�^( 495�Œ� )
	BYTE 		Creator[41] ;		// ��Җ�( ���g�p���� 0 �Ŗ��߂� )
	BYTE 		Comment[4][81] ;	// ��Җ��⎞���A���̂ق��̏��
	WORD		CreationDateMonth ;	// �쐬��  �� 1�`12
	WORD		CreationDateDay ;	// �쐬��  �� 1�`31
	WORD		CreationDateYear ;	// �쐬��  �N 4��( 2004�� )
	WORD		CreationDateHour ;	// �쐬��  �� 0�`23
	WORD		CreationDateMinute ;// �쐬��  �� 0�`59
	WORD		CreationDateSecond ;// �쐬��  �b 0�`59
	BYTE 		JobName[41] ;		// �d����( ���g�p���� 0 �Ŗ��߂� )
	WORD		CreationTimeHours ;	// �쐬����  �� 0�`65535
	WORD		CreationTimeMinute ;// �쐬����  �� 0�`59
	WORD		CreationTimeSecond ;// �쐬����  �b 0�`59
	BYTE		SoftID[41] ;		// �\�t�g�E�G�A�̂h�c( ���g�p���� 0 �Ŗ��߂� )
	WORD		VersionNumber ;		// �o�[�W�����i���o�[( 1.01 �������� 101 )
	BYTE		VersionRot ;		// ���b�g( �A�X�L�[�R�[�h )
	DWORD		ColorKey ;			// �J���[�L�[
	WORD		AcpectX ;			// �h�b�g�̃A�X�y�N�g�� X (0:����)
	WORD		AcpectY ;			// �h�b�g�̃A�X�y�N�g�� Y (0:����)
	WORD		GammaN ;			// �K���}�l 0.0 �` 10.0
	WORD		GammaD ;			// �K���}�l ( 0:�K���}�w�薳�� )
	DWORD		ColorTableOffset ;	// �J���[�R���N�V�����e�[�u���̈ʒu (0:�ȗ�)
	DWORD		StampTableOffset ;	// �X�^���v�e�[�u���̈ʒu (0:�ȗ�)
	DWORD		ScanTableOffset ;	// �X�L�����e�[�u���̈ʒu (0:�ȗ�)
	BYTE		AttributeType ;		// �A�g���r���[�g�^�C�v
									//	0:���f�[�^����
									//	1,2:���f�[�^����
									//	3:���L��
									//	4:pre-multiplied �ȃ��f�[�^
} _TGAEXTENSION ;

#ifndef DX_GCC_COMPILE
#pragma pack(pop)
#pragma pack()
#else
#pragma pack(pop)
#endif

#ifndef DX_NON_TGA

// �s�f�`�摜�̓ǂ݂���
static int LoadTargaImage( STREAMDATA *Src, BASEIMAGE *BaseImage, int GetFormatOnly )
{
	_TGAHEADER head ;
//	_TGAEXTENSION ext ;
	_TGAFOOTER foot ;
//	_TGATAG tag ;
	void *sp ;
	BYTE *data, *buf ;
	BYTE *temp, *palette ;
	STREAMDATASHRED *sstr ;
	int datasize, pixelbyte, pitch, usealpha, validfooter ;
	int rightleft, bottomtop, rle, pal, colordepth ;
	
	usealpha = 1 ;
	data = NULL ;
	buf = NULL ;
	temp = NULL ;
	palette = NULL ;

	sstr = &Src->ReadShred ;
	sp = Src->DataPoint ;
	
	// �t�b�^�𓾂�
	sstr->Seek( sp, -(int)sizeof( foot ), SEEK_END ) ;
	if( sstr->Read( &foot, sizeof( foot ), 1, sp ) <= 0 ||
		_MEMCMP( "TRUEVISION-XFILE", foot.Signature, ( int )_STRLEN( "TRUEVISION-XFILE" ) ) != 0 ||
		foot.ExtensionOffset != 0 )
	{
		// �t�b�^�[�Ȃ�		
		validfooter = 0 ;
		
		// �t�@�C���̐擪����ǂݍ���
		sstr->Seek( sp, 0, SEEK_SET ) ;
/*	
		// ���̗L����������
		sstr->Seek( sp, foot.ExtensionOffset, SEEK_SET ) ;
		if( sstr->Read( &ext, sizeof( ext ), 1, sp ) <= 0 ) return -1 ;
		usealpha = ext.AttributeType == 3 ? 3 : 0 ;
	
		// �f�[�^�̂���ʒu���擾
		sstr->Seek( sp, foot.DeveloperOffset, SEEK_SET ) ;
		if( sstr->Read( &tag, sizeof( tag ), 1, sp ) <= 0 ) return -1 ;
		
		// ���f�[�^�̂���ʒu�ֈړ�
		sstr->Seek( sp, tag.DataOffset, SEEK_SET ) ;
*/
	}
	else
	{
		// �t�b�^�[����
		validfooter = 1 ;
		
		// �t�@�C���̐擪����ǂݍ���
		sstr->Seek( sp, 0, SEEK_SET ) ;
	}

	// �w�b�_��ǂݍ���
	if( sstr->Read( &head, sizeof( head ), 1, sp ) <= 0 ) return -1 ;

	// ID�̃T�C�Y�������ǂݍ��݈ʒu��ύX����
	sstr->Seek( sp, head.ImageIDLength, SEEK_CUR ) ;
	
/*
	DXST_ERRORLOGFMT_ADDW(( L"ImageIDLength:%d", head.ImageIDLength )) ;
	DXST_ERRORLOGFMT_ADDW(( L"ImageType:%d", head.ImageType )) ;
	DXST_ERRORLOGFMT_ADDW(( L"PaletteOrigin:%d", head.PaletteOrigin )) ;
	DXST_ERRORLOGFMT_ADDW(( L"PaletteLength:%d", head.PaletteLength )) ;
	DXST_ERRORLOGFMT_ADDW(( L"PaletteBitDepth:%d", head.PaletteBitDepth )) ;
	DXST_ERRORLOGFMT_ADDW(( L"XOrigin:%d", head.XOrigin )) ;
	DXST_ERRORLOGFMT_ADDW(( L"YOrigin:%d", head.YOrigin )) ;
	DXST_ERRORLOGFMT_ADDW(( L"Width:%d", head.Width )) ;
	DXST_ERRORLOGFMT_ADDW(( L"Height:%d", head.Height )) ;
	DXST_ERRORLOGFMT_ADDW(( L"ColorBitDepth:%d", head.ColorBitDepth )) ;
	DXST_ERRORLOGFMT_ADDW(( L"Descriptor:%x", head.Descriptor )) ;
	DXST_ERRORLOGFMT_ADDW(( L"ValidFooter:%d", validfooter )) ;
*/

	// �J���[�p���b�g�O���t�B�b�N���g�p���Ă��邩�ǂ������ׂ�
	pal = head.ImageType == 1 || head.ImageType == 9 ? 1 : 0 ;

	// �F�[�x�𓾂�
	colordepth = pal == 1 ? head.PaletteBitDepth : head.ColorBitDepth ;
	
	// �f�[�^���s�f�`�����ׂ�
	if( /*head.ImageIDLength != 0 ||*/
		( head.ImageType >= 4 && head.ImageType <= 8 ) ||
		head.ImageType > 11 ||
		head.XOrigin != 0 ||
		head.YOrigin != 0 ||
		head.Width > 8192 ||
		head.Height > 8192 ||
		( colordepth != 16 && colordepth != 24 && colordepth != 32 ) )
		return -1 ;

	// �Ή����Ă��Ȃ��`���͒e��( �Q�K���`���Ɩ��m�̌`�� )
	if(	head.ImageType == 0 ||
		head.ImageType == 3 ||
		head.ImageType == 11 ||
		head.ImageType > 11 )
		return -1 ;

	// ���������O�X���k�f�[�^���ǂ������ׂ�
	rle = 0 ;
	switch( head.ImageType )
	{
	case 0 : rle = 0 ; break ;
	case 1 : rle = 0 ; break ;
	case 2 : rle = 0 ; break ;
	case 3 : rle = 0 ; break ;
	case 9 : rle = 1 ; break ;
	case 10 : rle = 1 ; break ;
	case 11 : rle = 1 ; break ;
	}
	
	// �C���[�W�̊i�[�������Z�b�g����
	rightleft = ( head.Descriptor >> 4 ) & 1 ? 1 : 0 ;
	bottomtop = ( head.Descriptor >> 5 ) & 1 ? 0 : 1 ;

	// �C���[�W�̃f�[�^�T�C�Y�𓾂�
	pixelbyte = colordepth / 8 ;
	pitch     = pixelbyte * head.Width ;
	datasize  = pitch * head.Height ;

	// ���t���� 32 �r�b�g�ȊO�������炨��グ
	if( ( ( usealpha & 1 ) == 1 && colordepth != 32 ) && 
		( ( usealpha & 2 ) == 2 && colordepth != 16 ) ) goto ERR ;

	// BASEIMAGE �\���̂̏��𖄂߂�
	{
		COLORDATA *cr ;
	
		BaseImage->Width = head.Width ;
		BaseImage->Height = head.Height ;
		BaseImage->Pitch = pitch ;
		
		cr                = &BaseImage->ColorData ;
		cr->ColorBitDepth = ( unsigned char )colordepth ;
		cr->PixelByte     = ( unsigned char )pixelbyte ;
		
		switch( colordepth )
		{
		case 16 :
			cr->RedWidth 	= 5 ;	cr->RedLoc   = 10 ;	cr->RedMask   = 0x7c00 ;
			cr->GreenWidth 	= 5 ;	cr->GreenLoc = 5  ;	cr->GreenMask = 0x03e0 ;
			cr->BlueWidth 	= 5 ;	cr->BlueLoc  = 0  ;	cr->BlueMask  = 0x001f ;

			if( ( usealpha & 2 ) == 2 )
			{
				cr->AlphaWidth 	= 1 ;	cr->AlphaLoc = 15 ;	cr->AlphaMask = 0x8000 ;
				cr->NoneMask	= 0 ;
			}
			else
			{
				cr->AlphaWidth 	= 0 ;	cr->AlphaLoc = 0 ;	cr->AlphaMask = 0 ;
				cr->NoneMask	= 0x8000 ;
			}
			break ;
			
		case 24 :
			cr->RedWidth 	= 8 ;	cr->RedLoc   = 16 ;	cr->RedMask   = 0xff0000 ;
			cr->GreenWidth 	= 8 ;	cr->GreenLoc = 8  ;	cr->GreenMask = 0x00ff00 ;
			cr->BlueWidth 	= 8 ;	cr->BlueLoc  = 0  ;	cr->BlueMask  = 0x0000ff ;
			cr->AlphaWidth 	= 0 ;	cr->AlphaLoc = 0  ;	cr->AlphaMask = 0x000000 ;
			cr->NoneMask	= 0 ;
			break ;
			
		case 32 :
			cr->RedWidth 	= 8 ;	cr->RedLoc   = 16 ;	cr->RedMask   = 0x00ff0000 ;
			cr->GreenWidth 	= 8 ;	cr->GreenLoc = 8  ;	cr->GreenMask = 0x0000ff00 ;
			cr->BlueWidth 	= 8 ;	cr->BlueLoc  = 0  ;	cr->BlueMask  = 0x000000ff ;

			if( ( usealpha & 1 ) == 1 )
			{
				cr->AlphaWidth 	= 8 ;	cr->AlphaLoc = 24 ;	cr->AlphaMask = 0xff000000 ;
				cr->NoneMask	= 0 ;
			}
			else
			{
				cr->AlphaWidth 	= 0 ;	cr->AlphaLoc = 0 ;	cr->AlphaMask = 0 ;
				cr->NoneMask	= 0xff000000 ;
			}
			break ;
		}
	}
	
	if( GetFormatOnly == FALSE )
	{
		// �摜�f�[�^���i�[���郁�����̈���m��
		data = ( BYTE * )DXALLOC( ( size_t )datasize ) ;
		if( data == NULL ) goto ERR ;
		
		// �p���b�g�O���t�B�b�N���ǂ����ŏ����𕪊�
		if( pal == 1 )
		{
			// �p���b�g�J���[�ł���ꍇ
			
			// �p���b�g��ǂݍ���
			palette = ( BYTE * )DXALLOC( ( size_t )( 256 * pixelbyte ) ) ;
			if( palette == NULL ) goto ERR ;
			_MEMSET( palette, 0, ( size_t )( 256 * pixelbyte ) ) ;
			if( sstr->Read( palette + head.PaletteOrigin * pixelbyte, ( size_t )( head.PaletteLength * pixelbyte ), 1, sp ) <= 0 ) goto ERR ;
			
			// �摜�����k����Ă��邩�ǂ����ŏ����𕪊�
			if( rle == 0 )
			{
				// �񈳏k
				
				unsigned int destpixel ;
				BYTE *src ;

				// �S�s�N�Z�����𓾂�
				destpixel = ( unsigned int )( head.Width * head.Height ) ;
				
				// �f�[�^��ǂݍ��ރ��������m�ۂ���
				temp = ( BYTE * )DXALLOC( destpixel ) ;
				if( temp == NULL ) goto ERR ;
				
				// �f�[�^��ǂݍ���
				if( sstr->Read( temp, destpixel, 1, sp ) <= 0 ) goto ERR ;
				
				// �p���b�g�f�[�^�����Ƀf�[�^��W�J����
				src = temp ;
				switch( colordepth )
				{
				case 16 :
					{
						WORD *pal, *dest ;

						pal = ( WORD * )palette ;	
						dest = ( WORD * )data ;
						do
						{
							*dest++ = pal[*src++] ;
						}while( --destpixel ) ;
					}
					break ;

				case 24 :
					{
						BYTE *dest ;

						dest = data ;
						do
						{
							*((WORD *)dest) = *((WORD *)( palette + *src * 3 ) ) ;
							dest[2] = palette[( *src * 3 ) + 2] ;
							dest += 3 ;
							src ++ ;
						}while( --destpixel ) ;
					}
					break ;

				case 32 :
					{
						DWORD *pal, *dest ;

						pal = ( DWORD * )palette ;	
						dest = ( DWORD * )data ;
						do
						{
							*dest++ = pal[*src++] ;
						}while( --destpixel ) ;
					}
					break ;
				}
			}
			else
			{
				// ���k
				
				DWORD pos, size, destpixel ;
				int loopnum ;
				BYTE *src ;
				
				// �S�s�N�Z�����𓾂�
				destpixel = ( DWORD )( head.Width * head.Height ) ;
				
				// ���k�f�[�^���܂ރf�[�^��ߑ������ǂݍ���
				pos = ( DWORD )sstr->Tell( sp ) ;
				sstr->Seek( sp, -(int)sizeof( foot ), SEEK_END ) ;
				size = ( DWORD )( sstr->Tell( sp ) - pos ) ;
				sstr->Seek( sp, pos, SEEK_SET ) ;
				temp = ( BYTE * )DXALLOC( size ) ;
				if( temp == NULL ) goto ERR ;
				if( sstr->Read( temp, size, 1, sp ) <= 0 ) goto ERR ;
				
				// �p���b�g�ƈ��k�f�[�^�����ɓW�J����
				src = temp ;
				switch( colordepth )
				{
				case 16 :
					{
						WORD *pal, *dest ;
						
						pal = (WORD *)palette ;
						dest = (WORD *)data ;
						do
						{
							loopnum = ( *src & 0x7f ) + 1 ;	// ���[�v�񐔂𓾂�( ���� 7bit �����[�v�� - 1 )
							destpixel -= loopnum ;	// ��������s�N�Z���������Z
							if( *src++ & 0x80 ) 	// �����F�������Ă��邩�ǂ����ŏ����𕪊�( 8bit  1:���R�[�h�W�J  0:�ʏ�W�J )
							{
								// �F�R�[�h���擾�E�W�J
								WORD cr = pal[*src++] ;	
								do	*dest++ = cr ;	while( --loopnum ) ;
							}
							else
							{
								// �F�R�[�h���擾�E�W�J
								do	*dest++ = pal[*src++] ;	while( --loopnum ) ;
							}
						}while( destpixel ) ;
					}
					break ;


				case 24 :
					{
						BYTE *dest ;
						
						dest = data ;
						do
						{
							loopnum = ( *src & 0x7f ) + 1 ;	// ���[�v�񐔂𓾂�( ���� 7bit �����[�v�� - 1 )
							destpixel -= loopnum ;	// ��������s�N�Z���������Z
							if( *src++ & 0x80 ) 	// �����F�������Ă��邩�ǂ����ŏ����𕪊�( 8bit  1:���R�[�h�W�J  0:�ʏ�W�J )
							{
								// �F�R�[�h���擾�E�W�J
								WORD cr1 ;
								BYTE cr2 ;
								cr1 = *((WORD *)( palette + *src * 3 )) ;
								cr2 = palette[( *src * 3 ) + 2] ;
								do
								{
									*((WORD *)dest) = cr1 ;
									dest[2] = cr2 ;
									dest += 3 ;
								}while( --loopnum ) ;
								src ++ ;
							}
							else
							{
								// �F�R�[�h���擾�E�W�J
								do
								{
									*((WORD *)dest) = *((WORD *)( palette + *src * 3 )) ;
									dest[2] = palette[( *src * 3 ) + 2] ;
									dest += 3 ;
									src ++ ;
								}while( --loopnum ) ;
							}
						}while( destpixel ) ;
					}
					break ;


				case 32 :
					{
						DWORD *pal, *dest ;
						
						pal = (DWORD *)palette ;
						dest = (DWORD *)data ;
						do
						{
							loopnum = ( *src & 0x7f ) + 1 ;	// ���[�v�񐔂𓾂�( ���� 7bit �����[�v�� - 1 )
							destpixel -= loopnum ;	// ��������s�N�Z���������Z
							if( *src++ & 0x80 ) 	// �����F�������Ă��邩�ǂ����ŏ����𕪊�( 8bit  1:���R�[�h�W�J  0:�ʏ�W�J )
							{
								// �F�R�[�h���擾�E�W�J
								DWORD cr = pal[*src++] ;	
								do	*dest++ = cr ;	while( --loopnum ) ;
							}
							else
							{
								// �F�R�[�h���擾�E�W�J
								do	*dest++ = pal[*src++] ;	while( --loopnum ) ;
							}
						}while( destpixel ) ;
					}
					break ;
				}
			}
			
			// �p���b�g�摜�ƃp���b�g�����������
			DXFREE( temp ) ;
			DXFREE( palette ) ;
			temp = NULL ;
			palette = NULL ;
		}
		else
		{
			// �p���b�g�J���[�ł͂Ȃ��ꍇ
			
			// ���k�f�[�^���ǂ����ɂ���ď����𕪊�
			if( rle == 0 )
			{
				// �񈳏k

				// ���ʂɃf�[�^��ǂݍ���
				if( sstr->Read( data, ( size_t )datasize, 1, sp ) <= 0 ) goto ERR ;
			}
			else
			{
				// ���k
			
				BYTE *src, *dest ;
				int size, pos, destpixel, loopnum ;
				
				// ���k�f�[�^���܂ރf�[�^��ߑ������ǂݍ���
				pos = ( int )sstr->Tell( sp ) ;
				sstr->Seek( sp, validfooter ? -(int)sizeof( foot ) : 0, SEEK_END ) ;
				size = ( int )( sstr->Tell( sp ) - pos ) ;
				sstr->Seek( sp, pos, SEEK_SET ) ;
				temp = ( BYTE * )DXALLOC( ( size_t )size ) ;
				if( temp == NULL ) goto ERR ;
				if( sstr->Read( temp, ( size_t )size, 1, sp ) <= 0 ) goto ERR ;
				
				// �W�J����
				dest      = data ;
				src       = temp ;
				destpixel = head.Width * head.Height ;
				
				// �r�b�g�[�x�ɂ���ď����𕪊�
				switch( head.ColorBitDepth )
				{
				case 16 :
					do
					{
						loopnum    = ( *src & 0x7f ) + 1 ;	// ���[�v�񐔂𓾂�( ���� 7bit �����[�v�� - 1 )
						destpixel -= loopnum ;	// �����s�N�Z���������炷
						if( *src++ & 0x80 )		// �����F�������Ă��邩�ǂ����ŏ����𕪊�( 8bit  1:���R�[�h�W�J  0:�ʏ�W�J )
						{
							do	// �F�R�[�h���擾�E�W�J
							{
								*((WORD *)dest) = *((WORD *)src) ;
								dest += 2 ;
							}while( --loopnum ) ;
							src += 2 ;
						}
						else
						{
							do	// �F�R�[�h���擾�E�W�J
							{
								*((WORD *)dest) = *((WORD *)src) ;
								dest += 2 ;
								src += 2 ;
							}while( --loopnum ) ;
						}
					}while( destpixel ) ;
					break ;
					
				case 24 :
					do
					{
						loopnum = ( *src & 0x7f ) + 1 ;	// ���[�v�񐔂𓾂�( ���� 7bit �����[�v�� - 1 )
						destpixel -= loopnum ;	// �����s�N�Z���������炷
						if( *src++ & 0x80 )		// �����F�������Ă��邩�ǂ����ŏ����𕪊�( 8bit  1:���R�[�h�W�J  0:�ʏ�W�J )
						{
							do	// �F�R�[�h���擾�E�W�J
							{
								*((WORD *)dest) = *((WORD *)src) ;
								dest[2] = src[2] ;
								dest += 3 ;
							}while( --loopnum ) ;
							src += 3 ;
						}
						else
						{
							do	// �F�R�[�h���擾�E�W�J
							{
								*((WORD *)dest) = *((WORD *)src) ;
								dest[2] = src[2] ;
								dest += 3 ;
								src += 3 ;
							}while( --loopnum ) ;
						}
					}while( destpixel ) ;
					break ;
					
				case 32 :
					do
					{
						loopnum = ( *src & 0x7f ) + 1 ;	// ���[�v�񐔂𓾂�( ���� 7bit �����[�v�� - 1 )
						destpixel -= loopnum ;	// �����s�N�Z���������炷
						if( *src++ & 0x80 )		// �����F�������Ă��邩�ǂ����ŏ����𕪊�( 8bit  1:���R�[�h�W�J  0:�ʏ�W�J )
						{
							do	// �F�R�[�h���擾�E�W�J
							{
								*((DWORD *)dest) = *((DWORD *)src) ;
								dest += 4 ;
							}while( --loopnum ) ;
							src += 4 ;
						}
						else
						{
							do	// �F�R�[�h���擾�E�W�J
							{
								*((DWORD *)dest) = *((DWORD *)src) ;
								dest += 4 ;
								src += 4 ;
							}while( --loopnum ) ;
						}
					}while( destpixel ) ;
					break ;
				}
				
				// ���k�f�[�^�����
				DXFREE( temp ) ;
				temp = NULL ;
			}
		}

		// �f�[�^�̊i�[�������c�w���C�u�������]�ނ��̂Ɠ����ꍇ�͍����ŏI��
		if( rightleft == 0 && bottomtop == 0 )
		{
			BaseImage->GraphData = data ;
		}
		else
		{
			// �f�[�^�̊i�[�������c�w���C�u�������]�ނ��̂ƈႤ�ꍇ�͒u�������������s��
			
			BYTE *dest, *src ;
			int i, j, addx, addy ;

			buf = ( BYTE * )DXALLOC( ( size_t )datasize ) ;
			if( buf == NULL ) goto ERR ;
			
			dest = buf ;
			if( rightleft )
			{
				dest += ( head.Width - 1 ) * pixelbyte ;
				addx = -pixelbyte ;
			}
			else
			{
				addx = pixelbyte ;
			}
			
			if( bottomtop )
			{
				dest += ( head.Height - 1 ) * pitch ;
				addy = rightleft ? 0 : -pitch * 2 ;
			}
			else
			{
				addy = rightleft ? 0 : pitch * 2 ;
			}
			
			src = data ;
			switch( colordepth )
			{
			case 16 :
				for( i = 0 ; i < head.Height ; i ++, dest += addy )
					for( j = 0 ; j < head.Width ; j ++, src += pixelbyte, dest += addx )
					{
						*((WORD *)dest) = *((WORD *)src) ;
					}
				break ;

			case 24 :
				for( i = 0 ; i < head.Height ; i ++, dest += addy )
					for( j = 0 ; j < head.Width ; j ++, src += pixelbyte, dest += addx )
					{
						*((WORD *)dest) = *((WORD *)src) ;
						*((BYTE *)(dest+2)) = *((BYTE *)(src+2)) ;
					}
				break ;
				
			case 32 :
				for( i = 0 ; i < head.Height ; i ++, dest += addy )
					for( j = 0 ; j < head.Width ; j ++, src += pixelbyte, dest += addx )
					{
						*((DWORD *)dest) = *((DWORD *)src) ;
					}
				break ;
			}

			// �C���[�W�̃|�C���^��Ԃ��ďI��
			BaseImage->GraphData = buf ;
			DXFREE( data ) ;
			data = NULL ;
		}
	}
	else
	{
		BaseImage->GraphData = NULL ;
	}
	
	return 0 ;
	
ERR :
	if( data ) DXFREE( data ) ;
	if( buf ) DXFREE( buf ) ;
	if( palette ) DXFREE( palette ) ;
	if( temp ) DXFREE( temp ) ;

	return -1 ;
}

#endif






// �J���[�t�H�[�}�b�g�e�[�u��
static DDSCOLORTABLE _DDSColorTable[] =
{
	{
		20,		// D3DFMT_R8G8B8
		FALSE,
		0,
		24,
		0x00ff0000, 0x0000ff00, 0x000000ff, 0x00000000
	} ,
	{
		21,		// D3DFMT_A8R8G8B8
		FALSE,
		0,
		32,
		0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000
	} ,
	{
		22,		// D3DFMT_X8R8G8B8
		FALSE,
		0,
		32,
		0x00ff0000, 0x0000ff00, 0x000000ff, 0x00000000
	} ,
	{
		23,		// D3DFMT_R5G6B5
		FALSE,
		0,
		16,
		0xf800, 0x07c0, 0x003f, 0x0000
	} ,
	{
		24,		// D3DFMT_X1R5G5B5
		FALSE,
		0,
		16,
		0x7c00, 0x03e0, 0x001f, 0x0000
	} ,
	{
		25,		// D3DFMT_A1R5G5B5
		FALSE,
		0,
		16,
		0x7c00, 0x03e0, 0x001f, 0x8000
	} ,
	{
		26,		// D3DFMT_A4R4G4B4
		FALSE,
		0,
		16,
		0x0f00, 0x00f0, 0x000f, 0xf000
	} ,
	{
		27,		// D3DFMT_R3G3B2
		FALSE,
		0,
		8,
		0xe0, 0x1c, 0x03, 0x00
	} ,
	{
		29,		// D3DFMT_A8R3G3B2
		FALSE,
		0,
		16,
		0x00e0, 0x001c, 0x0003, 0xff00
	} ,
	{
		30,		// D3DFMT_X4R4G4B4
		FALSE,
		0,
		16,
		0x0f00, 0x00f0, 0x000f, 0x0000
	} ,
	{
		31,		// D3DFMT_A2B10G10R10
		FALSE,
		0,
		32,
		0x000003ff, 0x000ffc00, 0x3ff00000, 0xc0000000
	} ,
	{
		32,		// D3DFMT_A8B8G8R8
		FALSE,
		0,
		32,
		0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000
	} ,
	{
		33,		// D3DFMT_X8B8G8R8
		FALSE,
		0,
		32,
		0x000000ff, 0x0000ff00, 0x00ff0000, 0x00000000
	} ,
	{
		34,		// D3DFMT_G16R16
		FALSE,
		2,
		16,
		0x00000000, 0x00000000, 0x00000000, 0x00000000
	} ,
	{
		35,		// D3DFMT_A2R10G10B10
		FALSE,
		0,
		32,
		0x3ff00000, 0x000ffc00, 0x000003ff, 0xc0000000
	} ,
	{
		36,		// D3DFMT_A16B16G16R16
		FALSE,
		4,
		16,
		0x00000000, 0x00000000, 0x00000000, 0x00000000
	} ,
	{
		81,		// D3DFMT_L16
		FALSE,
		1,
		16,
		0x00000000, 0x00000000, 0x00000000, 0x00000000
	} ,

	{
		111,	// DXGI_FORMAT_R16_FLOAT
		TRUE,
		1,
		16,
		0x00000000, 0x00000000, 0x00000000, 0x00000000
	} ,
	{
		112,	// DXGI_FORMAT_R16G16_FLOAT
		TRUE,
		2,
		16,
		0x00000000, 0x00000000, 0x00000000, 0x00000000
	} ,
	{
		113,	// DXGI_FORMAT_R16G16B16A16_FLOAT
		TRUE,
		4,
		16,
		0x00000000, 0x00000000, 0x00000000, 0x00000000
	} ,

	{
		114,	// DXGI_FORMAT_R32_FLOAT
		TRUE,
		1,
		32,
		0x00000000, 0x00000000, 0x00000000, 0x00000000
	} ,
	{
		115,	// DXGI_FORMAT_R32G32_FLOAT
		TRUE,
		2,
		32,
		0x00000000, 0x00000000, 0x00000000, 0x00000000
	} ,
	{
		116,	// DXGI_FORMAT_R32G32B32A32_FLOAT
		TRUE,
		4,
		32,
		0x00000000, 0x00000000, 0x00000000, 0x00000000
	} ,

	{
		0xffffffff,		// �I�[
		0xff,
		0xff,
		0xff,
		0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff
	} ,
} ;

// �c�c�r�摜�̓ǂ݂���
static int LoadDDSImage( STREAMDATA *Src, BASEIMAGE *BaseImage, int GetFormatOnly )
{
	void            *sp ;
	STREAMDATASHRED *sstr ;
	DDSFILEHEADER   head ;
	void            *data = NULL ;
	int             size ;
	int             pitch = 0 ;
	int             addsize ;
	int             i ;
	DWORD           headsize ;
	char            magic[ 4 ] ;
	int             dxtformat ;
	int             dxtsize ;
	int             dxtpacketsize ;
	int             mipcount ;
	int             imagenum ;

	sstr = &Src->ReadShred ;
	sp = Src->DataPoint ;
	data = NULL ;
	dxtformat = 0 ;

	// �c�c�r���ʕ�����̃`�F�b�N
	if( sstr->Read( magic, 4, 1, sp ) <= 0 ) return -1 ;
	if( _MEMCMP( magic, "DDS", ( int )_STRLEN( "DDS" ) ) != 0 ) return -1 ;

	// �w�b�_�T�C�Y���擾
	if( sstr->Read( &headsize, 4, 1, sp ) <= 0 ) return -1 ;

	// �t�H�[�}�b�g�̎擾
	if( sstr->Read( &head.dwFlags, headsize - 4, 1, sp ) <= 0 ) return -1 ;

	// �t�H�[�}�b�g�̎擾
	{
		int formatget ;
		
		formatget = 0 ;
	
		// DirectX8 �ȍ~�̃t�H�[�}�b�g��`�𒲂ׂ�
		if( head.dwPfFlags == BI_DDPF_FOURCC )
		{
			DDSCOLORTABLE *table ;
			
			for( 	table = _DDSColorTable ;
					table->Type != 0xffffffff && table->Type != head.dwFourCC ;
					table ++ ){}
			// ��������t�H�[�}�b�g���Z�b�g����
			if( table->Type != 0xffffffff )
			{
				// Float �^�C�v���ǂ����ŏ����𕪊�
				if( table->IsFloat )
				{
					// Float�^�C�v
					_MEMSET( &BaseImage->ColorData, 0, sizeof( BaseImage->ColorData ) ) ;
					BaseImage->ColorData.Format				= DX_BASEIMAGE_FORMAT_NORMAL ;
					BaseImage->ColorData.ChannelBitDepth	= ( unsigned char )table->BitDepth ;
					BaseImage->ColorData.ChannelNum			= ( unsigned char )table->ChannelNum ;
					BaseImage->ColorData.FloatTypeFlag		= ( unsigned char )TRUE ;
					BaseImage->ColorData.PixelByte			= ( unsigned char )( BaseImage->ColorData.ChannelBitDepth * BaseImage->ColorData.ChannelNum / 8 ) ;
				}
				else
				if( table->BitDepth == 16 && table->ChannelNum > 0 )
				{
					// �����^��1�`�����l��16�r�b�g�^�C�v

					_MEMSET( &BaseImage->ColorData, 0, sizeof( BaseImage->ColorData ) ) ;
					BaseImage->ColorData.Format				= DX_BASEIMAGE_FORMAT_NORMAL ;
					BaseImage->ColorData.ChannelBitDepth	= ( unsigned char )table->BitDepth ;
					BaseImage->ColorData.ChannelNum			= ( unsigned char )table->ChannelNum ;
					BaseImage->ColorData.FloatTypeFlag		= ( unsigned char )FALSE ;
					BaseImage->ColorData.PixelByte			= ( unsigned char )( BaseImage->ColorData.ChannelBitDepth * BaseImage->ColorData.ChannelNum / 8 ) ;
				}
				else
				{
					// �����l�^�C�v
					NS_CreateColorData( &BaseImage->ColorData, table->BitDepth,
										table->RedMask, table->GreenMask, table->BlueMask, table->AlphaMask ) ;
				}
				formatget = 1 ;
			}
		}
	
		// DXT�t�H�[�}�b�g���ǂ����𒲂ׂ�
		dxtpacketsize = 0 ;
		if( formatget == 0 && ( head.dwPfFlags & BI_DDPF_FOURCC ) != 0 )
		{
			_MEMSET( &BaseImage->ColorData, 0, sizeof( COLORDATA ) ) ;
			if( head.dwFourCC == MAKEFOURCC( 'D', 'X', 'T', '1' ) )
			{
				dxtformat = DX_BASEIMAGE_FORMAT_DXT1 ;
				dxtpacketsize = 8 ;
			}

			if( head.dwFourCC == MAKEFOURCC( 'D', 'X', 'T', '2' ) )
			{
				dxtformat = DX_BASEIMAGE_FORMAT_DXT2 ;
				dxtpacketsize = 16 ;
			}

			if( head.dwFourCC == MAKEFOURCC( 'D', 'X', 'T', '3' ) )
			{
				dxtformat = DX_BASEIMAGE_FORMAT_DXT3 ;
				dxtpacketsize = 16 ;
			}

			if( head.dwFourCC == MAKEFOURCC( 'D', 'X', 'T', '4' ) )
			{
				dxtformat = DX_BASEIMAGE_FORMAT_DXT4 ;
				dxtpacketsize = 16 ;
			}

			if( head.dwFourCC == MAKEFOURCC( 'D', 'X', 'T', '5' ) )
			{
				dxtformat = DX_BASEIMAGE_FORMAT_DXT5 ;
				dxtpacketsize = 16 ;
			}

			if( dxtformat != 0 )
			{
				formatget = 1 ;
				NS_CreateARGB8ColorData( &BaseImage->ColorData ) ; 
			}
		}

		if( formatget == 0 )
		{
			// �q�f�a�ȊO�̃t�H�[�}�b�g�A�q�f�a�R�����ĂȂ��t�H�[�}�b�g�A
			// �y�уp���b�g�摜�͎󂯕t���Ȃ�
			if( ( ( head.dwPfFlags & ( BI_DDPF_RGB | BI_DDPF_ALPHAPIXELS ) ) == ( BI_DDPF_RGB | BI_DDPF_ALPHAPIXELS ) ) || 
				( ( head.dwPfFlags & BI_DDPF_RGB ) == BI_DDPF_RGB ) )
			{
				// �J���[�t�H�[�}�b�g�̃Z�b�g
				NS_CreateColorData( &BaseImage->ColorData, ( int )head.dwRGBBitCount,
									head.dwRBitMask, head.dwGBitMask, head.dwBBitMask, head.dwRGBAlphaBitMask ) ;
									
				formatget = 1 ;
			}
		}
		
		// �t�H�[�}�b�g�������Ă��Ȃ������炱���ŏI��
		if( formatget == 0 ) return -1 ;
	}

	// �~�b�v�}�b�v�̏����擾����
	if( ( head.dwFlags & BI_DDSD_MIPMAPCOUNT ) && ( head.dwCaps & ( BI_DDSCAPS_MIPMAP | BI_DDSCAPS_COMPLEX ) ) == ( BI_DDSCAPS_MIPMAP | BI_DDSCAPS_COMPLEX ) )
	{
		mipcount = ( int )head.dwMipMapCount ;
	}
	else
	{
		mipcount = 0 ;
	}

	// �L���[�u�}�b�v�̏����擾����
	if( ( head.dwCaps2 & BI_DDSCAPS2_CUBEMAP ) != 0 && ( head.dwCaps2 & BI_DDSCAPS2_CUBEMAP_ALLFACES ) == BI_DDSCAPS2_CUBEMAP_ALLFACES )
	{
		imagenum = CUBEMAP_SURFACE_NUM ;
	}
	else
	{
		imagenum = 1 ;
	}

	// �c�w�s���ǂ����ŏ����𕪊�
	if( dxtformat != 0 )
	{
		// ���ʂ̃J���[�t�H�[�}�b�g���Z�b�g
		NS_CreateColorData( &BaseImage->ColorData, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000 ) ;

		// �f�[�^�̓ǂݍ���
		if( GetFormatOnly == FALSE )
		{
			DWORD TempWidth, TempHeight ;

			dxtsize = ( int )( ( head.dwWidth / 4 ) * ( head.dwHeight / 4 ) * dxtpacketsize ) ;
			if( mipcount > 1 )
			{
				TempWidth = head.dwWidth ;
				TempHeight = head.dwHeight ;
				dxtsize = 0 ;
				for( i = 0 ; i < mipcount ; i ++ )
				{
					addsize = ( int )( ( ( TempWidth + 3 ) / 4 ) * ( ( TempHeight + 3 ) / 4 ) * dxtpacketsize ) ;
					dxtsize += addsize ;

					TempWidth  /= 2 ;
					if( TempWidth == 0 )
					{
						TempWidth = 1 ;
					}
					TempHeight /= 2 ;
					if( TempHeight == 0 )
					{
						TempHeight = 1 ;
					}
				}
			}

			data = ( BYTE * )DXALLOC( ( size_t )( dxtsize * imagenum ) ) ;
			if( data == NULL ) return -1 ;
			if( sstr->Read( data, ( size_t )( dxtsize * imagenum ), 1, sp ) <= 0 )
			{
				DXFREE( data ) ;
				return -1 ;
			}
		}

		// �F�r�b�g�[�x�͂P�s�N�Z���ӂ�ɕK�v�ȃr�b�g��
		BaseImage->ColorData.ColorBitDepth   = ( unsigned char )( dxtformat == DX_BASEIMAGE_FORMAT_DXT1 ? 4 : 8 ) ;
		BaseImage->ColorData.Format          = ( unsigned char )dxtformat ;
		BaseImage->ColorData.ChannelNum      = 0 ;
		BaseImage->ColorData.ChannelBitDepth = 0 ;
		BaseImage->ColorData.FloatTypeFlag   = FALSE ;
		pitch = 0 ;
	}
	else
	{
		// �ʏ�̉摜�̏ꍇ

		if( GetFormatOnly == FALSE )
		{
			// �s�b�`�̎Z�o
			if( head.dwFlags & BI_DDSD_PITCH )
			{
				pitch = ( int )head.dwPitchOrLinearSize ;
			}
			else
			{
		//		DXST_ERRORLOGFMT_ADDW(( L"bitcount:%d", pform->dwRGBBitCount )) ;
				pitch = ( int )( head.dwWidth * BaseImage->ColorData.PixelByte ) ;
			}
			
			// �f�[�^�̓ǂݍ���
			size = ( int )( head.dwHeight * pitch ) ;
			if( mipcount > 1 )
			{
				addsize = size ;
				size = 0 ;
				for( i = 0 ; i < mipcount ; i ++ )
				{
					size += addsize ;
					addsize /= 4 ;
				}
			}

			data = DXALLOC( ( size_t )( size * imagenum ) ) ;
			if( data == NULL )
			{
				return -1 ;
			}

			if( mipcount > 1 )
			{
				BYTE *datap ;
				DWORD Width, Height ;

				datap = ( BYTE * )data ;
				Width  = head.dwWidth ;
				Height = head.dwHeight ;
				for( i = 0 ; i < mipcount ; i ++ )
				{
					addsize = Width * Height * BaseImage->ColorData.PixelByte ;
					if( sstr->Read( datap, ( size_t )( addsize * imagenum ), 1, sp ) <= 0 )
					{
						if( i == 0 )
						{
							DXFREE( data ) ;
							return -1 ;
						}
						break ;
					}
					datap += addsize * imagenum ;

					Width  /= 2 ;
					if( Width == 0 )
					{
						Width = 1 ;
					}
					Height /= 2 ;
					if( Height == 0 )
					{
						Height = 1 ;
					}
				}
				mipcount = i ;
			}
			else
			{
				if( sstr->Read( data, ( size_t )( size * imagenum ), 1, sp ) <= 0 )
				{
					DXFREE( data ) ;
					return -1 ;
				}
			}
		}
	}

	// BASEIMAGE �\���̂̏��𖄂߂�
	{
		BaseImage->GraphData      = data ;
		BaseImage->Width          = ( int )head.dwWidth ;
		BaseImage->Height         = ( int )head.dwHeight ;
		BaseImage->Pitch          = pitch ;
		BaseImage->MipMapCount    = mipcount ;
		BaseImage->GraphDataCount = imagenum == 1 ? 0 : imagenum ; 
	}

	// �I��
	return 0 ;
}






















// ��{�C���[�W�f�[�^�@�\���J�p�֐�

// �������A��n��

// ��{�C���[�W�Ǘ����̏�����
extern int InitializeBaseImageManage( void )
{
	// ���ɏ���������Ă����牽�����Ȃ�
	if( BASEIM.InitializeFlag == TRUE )
	{
		return -1 ;
	}

	// ���ˑ��̏������������s��
	if( InitializeBaseImageManage_PF() != 0 )
	{
		return -1 ;
	}

	// ���̂Ƃ��뉽�����邱�ƂȂ�

	// �������t���O�𗧂Ă�
	BASEIM.InitializeFlag = TRUE ;

	// �I��
	return 0 ;
}

// ��{�C���[�W�Ǘ����̌�n��
extern int TerminateBaseImageManage( void )
{
	// ���Ɍ�n������Ă����牽�����Ȃ�
	if( BASEIM.InitializeFlag == FALSE )
	{
		return -1 ;
	}

	// ���ˑ��̌�n���������s��
	if( TerminateBaseImageManage_PF() != 0 )
	{
		return -1 ;
	}

	// ���̂Ƃ��뉽�����邱�ƂȂ�

	// �������t���O��|��
	BASEIM.InitializeFlag = FALSE ;

	// �I��
	return 0 ;
}

















//��{�C���[�W�f�[�^�\���̊֌W

// �e��O���t�B�b�N�f�[�^�����{�C���[�W�f�[�^���\�z����
extern int NS_CreateBaseImage(  const TCHAR *FileName, const void *FileImage, int FileImageSize, int DataType, BASEIMAGE *BaseImage, int ReverseFlag )
{
	return NS_CreateGraphImageOrDIBGraph( FileName, FileImage, FileImageSize, DataType, FALSE, ReverseFlag, BaseImage, NULL, NULL ) ;
}

// �e��O���t�B�b�N�f�[�^�����{�C���[�W�f�[�^���\�z����
extern int CreateBaseImage_WCHAR_T(  const wchar_t *FileName, const void *FileImage, int FileImageSize, int DataType, BASEIMAGE *BaseImage, int ReverseFlag )
{
	return CreateGraphImageOrDIBGraph_WCHAR_T( FileName, FileImage, FileImageSize, DataType, FALSE, ReverseFlag, BaseImage, NULL, NULL ) ;
}

// CreateBaseImage �̋�����
extern int NS_CreateGraphImage( const TCHAR *FileName, const void *DataImage, int DataImageSize, int DataImageType, BASEIMAGE *GraphImage, int ReverseFlag )
{
	return NS_CreateGraphImageOrDIBGraph( FileName, DataImage, DataImageSize, DataImageType, FALSE, ReverseFlag, GraphImage, NULL, NULL ) ;
}

// �摜�t�@�C�������{�C���[�W�f�[�^���\�z����
extern int NS_CreateBaseImageToFile( const TCHAR *FileName,               BASEIMAGE *BaseImage, int ReverseFlag )
{
	return NS_CreateBaseImage( FileName, NULL, 0, LOADIMAGE_TYPE_FILE, BaseImage, ReverseFlag ) ;
}

// �摜�t�@�C�������{�C���[�W�f�[�^���\�z����
extern int CreateBaseImageToFile_WCHAR_T( const wchar_t *FileName,               BASEIMAGE *BaseImage, int ReverseFlag )
{
	return CreateBaseImage_WCHAR_T( FileName, NULL, 0, LOADIMAGE_TYPE_FILE, BaseImage, ReverseFlag ) ;
}

// ��������ɓW�J���ꂽ�摜�t�@�C�������{�C���[�W�f�[�^���\�z����
extern int NS_CreateBaseImageToMem(  const void *FileImage, int FileImageSize, BASEIMAGE *BaseImage, int ReverseFlag )
{
	return NS_CreateBaseImage( NULL, FileImage, FileImageSize, LOADIMAGE_TYPE_MEM, BaseImage, ReverseFlag ) ;
}

// �`�q�f�a�e�`�����l�� 32bit ���������_�^ �J���[�̊�{�C���[�W�f�[�^���쐬����
extern int NS_CreateARGBF32ColorBaseImage( int SizeX, int SizeY, BASEIMAGE *BaseImage )
{
	NS_CreateARGBF32ColorData( &BaseImage->ColorData );
	BaseImage->Width          = SizeX ;
	BaseImage->Height         = SizeY ;
	BaseImage->Pitch          = SizeX * BaseImage->ColorData.PixelByte ;
	BaseImage->Pitch          = ( BaseImage->Pitch + 3 ) / 4 * 4 ;
	BaseImage->GraphData      = DXALLOC( ( size_t )( BaseImage->Pitch * BaseImage->Height ) );
	if( BaseImage->GraphData == NULL ) return -1;
	BaseImage->MipMapCount    = 0 ;
	BaseImage->GraphDataCount = 0 ;

	// �I��
	return 0;
}

// �`�q�f�a�e�`�����l�� 16bit ���������_�^ �J���[�̊�{�C���[�W�f�[�^���쐬����
extern int NS_CreateARGBF16ColorBaseImage( int SizeX, int SizeY, BASEIMAGE *BaseImage )
{
	NS_CreateARGBF16ColorData( &BaseImage->ColorData );
	BaseImage->Width          = SizeX ;
	BaseImage->Height         = SizeY ;
	BaseImage->Pitch          = SizeX * BaseImage->ColorData.PixelByte ;
	BaseImage->Pitch          = ( BaseImage->Pitch + 3 ) / 4 * 4 ;
	BaseImage->GraphData      = DXALLOC( ( size_t )( BaseImage->Pitch * BaseImage->Height ) );
	if( BaseImage->GraphData == NULL ) return -1;
	BaseImage->MipMapCount    = 0 ;
	BaseImage->GraphDataCount = 0 ;

	// �I��
	return 0;
}

// �`�q�f�a�W�J���[�̋�̊�{�C���[�W�f�[�^���쐬����
extern int NS_CreateARGB8ColorBaseImage( int SizeX, int SizeY, BASEIMAGE *BaseImage )
{
	NS_CreateARGB8ColorData( &BaseImage->ColorData );
	BaseImage->Width          = SizeX ;
	BaseImage->Height         = SizeY ;
	BaseImage->Pitch          = SizeX * BaseImage->ColorData.PixelByte ;
	BaseImage->Pitch          = ( BaseImage->Pitch + 3 ) / 4 * 4 ;
	BaseImage->GraphData      = DXALLOC( ( size_t )( BaseImage->Pitch * BaseImage->Height ) );
	if( BaseImage->GraphData == NULL ) return -1;
	BaseImage->MipMapCount    = 0 ;
	BaseImage->GraphDataCount = 0 ;

	// �I��
	return 0;
}

// �w�q�f�a�W�J���[�̋�̊�{�C���[�W�f�[�^���쐬����
extern int NS_CreateXRGB8ColorBaseImage( int SizeX, int SizeY, BASEIMAGE *BaseImage )
{
	NS_CreateXRGB8ColorData( &BaseImage->ColorData );
	BaseImage->Width  = SizeX;
	BaseImage->Height = SizeY;
	BaseImage->Pitch  = SizeX * BaseImage->ColorData.PixelByte;
	BaseImage->Pitch  = ( BaseImage->Pitch + 3 ) / 4 * 4;
	BaseImage->GraphData = DXALLOC( ( size_t )( BaseImage->Pitch * BaseImage->Height ) );
	if( BaseImage->GraphData == NULL ) return -1;
	BaseImage->MipMapCount = 0 ;
	BaseImage->GraphDataCount = 0 ;

	// �I��
	return 0;
}

// �`�q�f�a�S�J���[�̋�̊�{�C���[�W�f�[�^���쐬����
extern int NS_CreateARGB4ColorBaseImage( int SizeX, int SizeY, BASEIMAGE *BaseImage )
{
	NS_CreateARGB4ColorData( &BaseImage->ColorData );
	BaseImage->Width  = SizeX;
	BaseImage->Height = SizeY;
	BaseImage->Pitch  = SizeX * BaseImage->ColorData.PixelByte;
	BaseImage->Pitch  = ( BaseImage->Pitch + 3 ) / 4 * 4;
	BaseImage->GraphData = DXALLOC( ( size_t )( BaseImage->Pitch * BaseImage->Height ) );
	if( BaseImage->GraphData == NULL ) return -1;
	BaseImage->MipMapCount = 0 ;
	BaseImage->GraphDataCount = 0 ;

	// �I��
	return 0;
}

// �q�f�a�W�J���[�̋�̊�{�C���[�W�f�[�^���쐬����
extern int NS_CreateRGB8ColorBaseImage( int SizeX, int SizeY, BASEIMAGE *BaseImage )
{
	NS_CreateFullColorData( &BaseImage->ColorData );
	BaseImage->Width  = SizeX;
	BaseImage->Height = SizeY;
	BaseImage->Pitch  = SizeX * BaseImage->ColorData.PixelByte;
	BaseImage->Pitch  = ( BaseImage->Pitch + 3 ) / 4 * 4;
	BaseImage->GraphData = DXALLOC( ( size_t )( BaseImage->Pitch * BaseImage->Height ) );
	if( BaseImage->GraphData == NULL ) return -1;
	BaseImage->MipMapCount = 0 ;
	BaseImage->GraphDataCount = 0 ;

	// �I��
	return 0;
}

// �p���b�g�W�r�b�g�J���[�̋�̊�{�C���[�W�f�[�^���쐬����
extern int NS_CreatePAL8ColorBaseImage( int SizeX, int SizeY, BASEIMAGE *BaseImage, int UseAlpha )
{
	NS_CreatePal8ColorData( &BaseImage->ColorData, UseAlpha );
	BaseImage->Width  = SizeX;
	BaseImage->Height = SizeY;
	BaseImage->Pitch  = SizeX * BaseImage->ColorData.PixelByte;
	BaseImage->Pitch  = ( BaseImage->Pitch + 3 ) / 4 * 4;
	BaseImage->GraphData = DXALLOC( ( size_t )( BaseImage->Pitch * BaseImage->Height ) );
	if( BaseImage->GraphData == NULL ) return -1;
	BaseImage->MipMapCount = 0 ;
	BaseImage->GraphDataCount = 0 ;

	// �I��
	return 0;
}

// �w��̃J���[�t�H�[�}�b�g�̊k�̊�{�C���[�W�f�[�^���쐬����
extern int NS_CreateColorDataBaseImage( int SizeX, int SizeY, const COLORDATA *ColorData, BASEIMAGE *BaseImage )
{
	BaseImage->ColorData = *ColorData ;
	BaseImage->Width  = SizeX;
	BaseImage->Height = SizeY;
	BaseImage->Pitch  = SizeX * BaseImage->ColorData.PixelByte;
	BaseImage->Pitch  = ( BaseImage->Pitch + 3 ) / 4 * 4;
	BaseImage->GraphData = DXALLOC( ( size_t )( BaseImage->Pitch * BaseImage->Height ) );
	if( BaseImage->GraphData == NULL ) return -1;
	BaseImage->MipMapCount = 0 ;
	BaseImage->GraphDataCount = 0 ;

	// �I��
	return 0;
}

// ��{�C���[�W�f�[�^�̃C���[�W�T�C�Y���擾����( byte�P�� )
extern int NS_GetBaseImageGraphDataSize( const BASEIMAGE *BaseImage )
{
	int BaseImageSize ;
	int TotalSize ;
	int PacketSize ;
	int i ;

	if( BaseImage->ColorData.Format == DX_BASEIMAGE_FORMAT_NORMAL )
	{
		BaseImageSize = BaseImage->Pitch * BaseImage->Height ;
	}
	else
	{
		switch( BaseImage->ColorData.Format )
		{
		case DX_BASEIMAGE_FORMAT_DXT1 :
			PacketSize = 8 ;
			break ;

		case DX_BASEIMAGE_FORMAT_DXT2 :
			PacketSize = 16 ;
			break ;

		case DX_BASEIMAGE_FORMAT_DXT3 :
			PacketSize = 16 ;
			break ;

		case DX_BASEIMAGE_FORMAT_DXT4 :
			PacketSize = 16 ;
			break ;

		case DX_BASEIMAGE_FORMAT_DXT5 :
			PacketSize = 16 ;
			break ;

		default :
			return -1 ;
		}

		BaseImageSize = ( BaseImage->Width / 4 ) * ( BaseImage->Height / 4 ) * PacketSize ;
	}

	if( BaseImage->MipMapCount > 1 )
	{
		TotalSize = 0 ;
		for( i = 0 ; i < BaseImage->MipMapCount ; i ++ )
		{
			TotalSize += BaseImageSize ;
			BaseImageSize /= 4 ;
			if( BaseImageSize < 8 )
			{
				BaseImageSize = 8 ;
			}
		}
	}
	else
	{
		TotalSize = BaseImageSize ;
	}

	return TotalSize ;
}

// ��{�C���[�W�f�[�^�̎w��̕����������g����{�C���[�W�f�[�^�̏����쐬����
extern int NS_DerivationBaseImage( const BASEIMAGE *BaseImage, int x1, int y1, int x2, int y2, BASEIMAGE *NewBaseImage )
{
	// ���W�G���[�`�F�b�N
	if( x1 < 0 || y1 < 0 || x1 >= x2 || y1 >= y2 ||
		x2 > BaseImage->Width || y2 > BaseImage->Height )
	{
		return -1 ;
	}

	// �W���t�H�[�}�b�g�{MipMap��g�p�̂ݑΉ�
	if( BaseImage->ColorData.Format != DX_BASEIMAGE_FORMAT_NORMAL ||
		BaseImage->MipMapCount != 1 )
	{
		return -1 ;
	}

	// �����Z�b�g
	*NewBaseImage = *BaseImage ;
	NewBaseImage->Width  = x2 - x1 ;
	NewBaseImage->Height = y2 - y1 ;
	NewBaseImage->GraphData = ( BYTE * )BaseImage->GraphData + x1 * BaseImage->ColorData.PixelByte + y1 * BaseImage->Pitch ;

	// �I��
	return 0 ;
}

// ��{�C���[�W�f�[�^�̌�n�����s��
extern int NS_ReleaseBaseImage(  BASEIMAGE *GraphImage )
{
	if( GraphImage->GraphData != NULL )
	{
		DXFREE( GraphImage->GraphData ) ;
		GraphImage->GraphData = NULL ;
	}
	_MEMSET( GraphImage, 0, sizeof( BASEIMAGE ) ) ;

	// �I��
	return 0 ;
}

// ReleaseBaseImage �̋�����
extern int NS_ReleaseGraphImage( BASEIMAGE *GraphImage )
{
	return NS_ReleaseBaseImage( GraphImage ) ;
}

// DX_BASEIMAGE_FORMAT_NORMAL �ȊO�̌`���̃C���[�W�f�[�^�� DX_BASEIMAGE_FORMAT_NORMAL �`���̃C���[�W�ɕϊ�����
extern int NS_ConvertNormalFormatBaseImage( BASEIMAGE *BaseImage, int ReleaseOrigGraphData )
{
	void *NewBuffer ;
	DWORD NewPixelByte, NewPitch, NewImageSize ;
	DWORD code, *dstd, pw, ph, i, j, k ;
	BYTE *dstb, *srcd, *bdstb ;
	BYTE C[ 4 ][ 4 ], A[ 8 ] ;
	DWORD GraphNum ;

	// �ŏ�����`���� DX_BASEIMAGE_FORMAT_NORMAL �������牽�������ɏI��
	if( BaseImage->ColorData.Format == DX_BASEIMAGE_FORMAT_NORMAL )
		return 0 ;

	// �f�[�^�̐����Z�b�g
	GraphNum = ( DWORD )( BaseImage->GraphDataCount == 0 ? 1 : 6 ) ;

	// �V�����f�[�^���i�[���郁�����̈�̊m��
	NewPixelByte = 4 ;
	NewPitch = BaseImage->Width * NewPixelByte ;
	NewImageSize = NewPitch * BaseImage->Height ;
	NewBuffer = DXALLOC( NewImageSize * GraphNum ) ;
	if( NewBuffer == NULL )
	{
		// ���������m�ۂł��Ȃ�������G���[
		return -1 ;
	}

	// �t�H�[�}�b�g�ɂ���ď����𕪊�
	pw = ( DWORD )( BaseImage->Width  / 4 ) ;
	ph = ( DWORD )( BaseImage->Height / 4 ) ;
	for( k = 0 ; k < GraphNum ; k ++ )
	{
		bdstb = ( BYTE * )NewBuffer + NewImageSize * k ;
		srcd = ( BYTE * )BaseImage->GraphData ;
		switch( BaseImage->ColorData.Format )
		{
		case DX_BASEIMAGE_FORMAT_DXT1 :
			for( i = 0 ; i < ph ; i ++ )
			{
				for( j = 0 ; j < pw ; j ++, srcd += 8 )
				{
					dstd = ( DWORD * )( bdstb + ( j * 4 ) * NewPixelByte + ( i * 4 ) * NewPitch ) ;

					C[ 0 ][ 0 ] = ( BYTE )( ( ( ( ( WORD * )srcd )[ 0 ] >>  0 ) & 31 ) * 255 / 31 ) ;
					C[ 0 ][ 1 ] = ( BYTE )( ( ( ( ( WORD * )srcd )[ 0 ] >>  5 ) & 63 ) * 255 / 63 ) ;
					C[ 0 ][ 2 ] = ( BYTE )( ( ( ( ( WORD * )srcd )[ 0 ] >> 11 ) & 31 ) * 255 / 31 ) ;
					C[ 0 ][ 3 ] = 255 ;

					C[ 1 ][ 0 ] = ( BYTE )( ( ( ( ( WORD * )srcd )[ 1 ] >>  0 ) & 31 ) * 255 / 31 ) ;
					C[ 1 ][ 1 ] = ( BYTE )( ( ( ( ( WORD * )srcd )[ 1 ] >>  5 ) & 63 ) * 255 / 63 ) ;
					C[ 1 ][ 2 ] = ( BYTE )( ( ( ( ( WORD * )srcd )[ 1 ] >> 11 ) & 31 ) * 255 / 31 ) ;
					C[ 1 ][ 3 ] = 255 ;

					if( ( ( WORD * )srcd )[ 0 ] > ( ( WORD * )srcd )[ 1 ] )
					{
						// ���ߐF�Ȃ�
						C[ 2 ][ 0 ] = ( BYTE )( ( C[ 0 ][ 0 ] * 2 + C[ 1 ][ 0 ] ) / 3 ) ;
						C[ 2 ][ 1 ] = ( BYTE )( ( C[ 0 ][ 1 ] * 2 + C[ 1 ][ 1 ] ) / 3 ) ;
						C[ 2 ][ 2 ] = ( BYTE )( ( C[ 0 ][ 2 ] * 2 + C[ 1 ][ 2 ] ) / 3 ) ;
						C[ 2 ][ 3 ] = 255 ;

						C[ 3 ][ 0 ] = ( BYTE )( ( C[ 0 ][ 0 ] + C[ 1 ][ 0 ] * 2 ) / 3 ) ;
						C[ 3 ][ 1 ] = ( BYTE )( ( C[ 0 ][ 1 ] + C[ 1 ][ 1 ] * 2 ) / 3 ) ;
						C[ 3 ][ 2 ] = ( BYTE )( ( C[ 0 ][ 2 ] + C[ 1 ][ 2 ] * 2 ) / 3 ) ;
						C[ 3 ][ 3 ] = 255 ;
					}
					else
					{
						// ���ߐF����
						C[ 2 ][ 0 ] = ( BYTE )( ( C[ 0 ][ 0 ] + C[ 1 ][ 0 ] ) / 2 ) ;
						C[ 2 ][ 1 ] = ( BYTE )( ( C[ 0 ][ 1 ] + C[ 1 ][ 1 ] ) / 2 ) ;
						C[ 2 ][ 2 ] = ( BYTE )( ( C[ 0 ][ 2 ] + C[ 1 ][ 2 ] ) / 2 ) ;
						C[ 2 ][ 3 ] = 255 ;

						C[ 3 ][ 0 ] = ( BYTE )( ( C[ 0 ][ 0 ] + C[ 1 ][ 0 ] ) / 2 ) ;
						C[ 3 ][ 1 ] = ( BYTE )( ( C[ 0 ][ 1 ] + C[ 1 ][ 1 ] ) / 2 ) ;
						C[ 3 ][ 2 ] = ( BYTE )( ( C[ 0 ][ 2 ] + C[ 1 ][ 2 ] ) / 2 ) ;
						C[ 3 ][ 3 ] = 0 ;
					}

					code = *( ( DWORD * )( srcd + 4 ) ) ;
					dstd[ 0 ] = *( ( DWORD * )C[ ( code >>  0 ) & 3 ] ) ;
					dstd[ 1 ] = *( ( DWORD * )C[ ( code >>  2 ) & 3 ] ) ;
					dstd[ 2 ] = *( ( DWORD * )C[ ( code >>  4 ) & 3 ] ) ;
					dstd[ 3 ] = *( ( DWORD * )C[ ( code >>  6 ) & 3 ] ) ;
					dstd = ( DWORD * )( ( BYTE * )dstd + NewPitch ) ;

					dstd[ 0 ] = *( ( DWORD * )C[ ( code >>  8 ) & 3 ] ) ;
					dstd[ 1 ] = *( ( DWORD * )C[ ( code >> 10 ) & 3 ] ) ;
					dstd[ 2 ] = *( ( DWORD * )C[ ( code >> 12 ) & 3 ] ) ;
					dstd[ 3 ] = *( ( DWORD * )C[ ( code >> 14 ) & 3 ] ) ;
					dstd = ( DWORD * )( ( BYTE * )dstd + NewPitch ) ;

					dstd[ 0 ] = *( ( DWORD * )C[ ( code >> 16 ) & 3 ] ) ;
					dstd[ 1 ] = *( ( DWORD * )C[ ( code >> 18 ) & 3 ] ) ;
					dstd[ 2 ] = *( ( DWORD * )C[ ( code >> 20 ) & 3 ] ) ;
					dstd[ 3 ] = *( ( DWORD * )C[ ( code >> 22 ) & 3 ] ) ;
					dstd = ( DWORD * )( ( BYTE * )dstd + NewPitch ) ;

					dstd[ 0 ] = *( ( DWORD * )C[ ( code >> 24 ) & 3 ] ) ;
					dstd[ 1 ] = *( ( DWORD * )C[ ( code >> 26 ) & 3 ] ) ;
					dstd[ 2 ] = *( ( DWORD * )C[ ( code >> 28 ) & 3 ] ) ;
					dstd[ 3 ] = *( ( DWORD * )C[ ( code >> 30 ) & 3 ] ) ;
				}
			}
			break ;

		case DX_BASEIMAGE_FORMAT_DXT2 :
			for( i = 0 ; i < ph ; i ++ )
			{
				for( j = 0 ; j < pw ; j ++ )
				{
					// �J���[�̏���
					{
						dstb = bdstb + ( j * 4 ) * NewPixelByte + ( i * 4 ) * NewPitch ;
						dstd = ( DWORD * )dstb ;
						C[ 0 ][ 0 ] = ( BYTE )( ( ( ( ( WORD * )srcd )[ 0 ] >>  0 ) & 31 ) * 255 / 31 ) ; 
						C[ 0 ][ 1 ] = ( BYTE )( ( ( ( ( WORD * )srcd )[ 0 ] >>  5 ) & 63 ) * 255 / 63 ) ; 
						C[ 0 ][ 2 ] = ( BYTE )( ( ( ( ( WORD * )srcd )[ 0 ] >> 11 ) & 31 ) * 255 / 31 ) ;
						C[ 0 ][ 3 ] = 255 ;

						C[ 1 ][ 0 ] = ( BYTE )( ( ( ( ( WORD * )srcd )[ 1 ] >>  0 ) & 31 ) * 255 / 31 ) ;
						C[ 1 ][ 1 ] = ( BYTE )( ( ( ( ( WORD * )srcd )[ 1 ] >>  5 ) & 63 ) * 255 / 63 ) ;
						C[ 1 ][ 2 ] = ( BYTE )( ( ( ( ( WORD * )srcd )[ 1 ] >> 11 ) & 31 ) * 255 / 31 ) ;
						C[ 1 ][ 3 ] = 255 ;

						C[ 2 ][ 0 ] = ( BYTE )( ( C[ 0 ][ 0 ] * 2 + C[ 1 ][ 0 ] ) / 3 ) ;
						C[ 2 ][ 1 ] = ( BYTE )( ( C[ 0 ][ 1 ] * 2 + C[ 1 ][ 1 ] ) / 3 ) ;
						C[ 2 ][ 2 ] = ( BYTE )( ( C[ 0 ][ 2 ] * 2 + C[ 1 ][ 2 ] ) / 3 ) ;
						C[ 2 ][ 3 ] = 255 ;

						C[ 3 ][ 0 ] = ( BYTE )( ( C[ 0 ][ 0 ] + C[ 1 ][ 0 ] * 2 ) / 3 ) ;
						C[ 3 ][ 1 ] = ( BYTE )( ( C[ 0 ][ 1 ] + C[ 1 ][ 1 ] * 2 ) / 3 ) ;
						C[ 3 ][ 2 ] = ( BYTE )( ( C[ 0 ][ 2 ] + C[ 1 ][ 2 ] * 2 ) / 3 ) ;
						C[ 3 ][ 3 ] = 255 ;

						code = *( ( DWORD * )( srcd + 4 ) ) ;
						dstd[ 0 ] = *( ( DWORD * )C[ ( code >>  0 ) & 3 ] ) ;
						dstd[ 1 ] = *( ( DWORD * )C[ ( code >>  2 ) & 3 ] ) ;
						dstd[ 2 ] = *( ( DWORD * )C[ ( code >>  4 ) & 3 ] ) ;
						dstd[ 3 ] = *( ( DWORD * )C[ ( code >>  6 ) & 3 ] ) ;
						dstd = ( DWORD * )( ( BYTE * )dstd + NewPitch ) ;

						dstd[ 0 ] = *( ( DWORD * )C[ ( code >>  8 ) & 3 ] ) ;
						dstd[ 1 ] = *( ( DWORD * )C[ ( code >> 10 ) & 3 ] ) ;
						dstd[ 2 ] = *( ( DWORD * )C[ ( code >> 12 ) & 3 ] ) ;
						dstd[ 3 ] = *( ( DWORD * )C[ ( code >> 14 ) & 3 ] ) ;
						dstd = ( DWORD * )( ( BYTE * )dstd + NewPitch ) ;

						dstd[ 0 ] = *( ( DWORD * )C[ ( code >> 16 ) & 3 ] ) ;
						dstd[ 1 ] = *( ( DWORD * )C[ ( code >> 18 ) & 3 ] ) ;
						dstd[ 2 ] = *( ( DWORD * )C[ ( code >> 20 ) & 3 ] ) ;
						dstd[ 3 ] = *( ( DWORD * )C[ ( code >> 22 ) & 3 ] ) ;
						dstd = ( DWORD * )( ( BYTE * )dstd + NewPitch ) ;

						dstd[ 0 ] = *( ( DWORD * )C[ ( code >> 24 ) & 3 ] ) ;
						dstd[ 1 ] = *( ( DWORD * )C[ ( code >> 26 ) & 3 ] ) ;
						dstd[ 2 ] = *( ( DWORD * )C[ ( code >> 28 ) & 3 ] ) ;
						dstd[ 3 ] = *( ( DWORD * )C[ ( code >> 30 ) & 3 ] ) ;
					}
					srcd += 8 ;

					// �A���t�@�`�����l���̃Z�b�g
					{
						code = *( ( DWORD * )srcd ) ;
						dstb[  3 ] = ( BYTE )( ( ( code >>  0 ) & 15 ) * 255 / 15 ) ;
						dstb[  7 ] = ( BYTE )( ( ( code >>  4 ) & 15 ) * 255 / 15 ) ;
						dstb[ 11 ] = ( BYTE )( ( ( code >>  8 ) & 15 ) * 255 / 15 ) ;
						dstb[ 15 ] = ( BYTE )( ( ( code >> 12 ) & 15 ) * 255 / 15 ) ;
						dstb = dstb + NewPitch ;

						dstb[  3 ] = ( BYTE )( ( ( code >> 16 ) & 15 ) * 255 / 15 ) ;
						dstb[  7 ] = ( BYTE )( ( ( code >> 20 ) & 15 ) * 255 / 15 ) ;
						dstb[ 11 ] = ( BYTE )( ( ( code >> 24 ) & 15 ) * 255 / 15 ) ;
						dstb[ 15 ] = ( BYTE )( ( ( code >> 28 ) & 15 ) * 255 / 15 ) ;
						dstb = dstb + NewPitch ;

						code = *( ( DWORD * )( srcd + 4 ) ) ;
						dstb[  3 ] = ( BYTE )( ( ( code >>  0 ) & 15 ) * 255 / 15 ) ;
						dstb[  7 ] = ( BYTE )( ( ( code >>  4 ) & 15 ) * 255 / 15 ) ;
						dstb[ 11 ] = ( BYTE )( ( ( code >>  8 ) & 15 ) * 255 / 15 ) ;
						dstb[ 15 ] = ( BYTE )( ( ( code >> 12 ) & 15 ) * 255 / 15 ) ;
						dstb = dstb + NewPitch ;

						dstb[  3 ] = ( BYTE )( ( ( code >> 16 ) & 15 ) * 255 / 15 ) ;
						dstb[  7 ] = ( BYTE )( ( ( code >> 20 ) & 15 ) * 255 / 15 ) ;
						dstb[ 11 ] = ( BYTE )( ( ( code >> 24 ) & 15 ) * 255 / 15 ) ;
						dstb[ 15 ] = ( BYTE )( ( ( code >> 28 ) & 15 ) * 255 / 15 ) ;
					}
					srcd += 8 ;

					// �A���t�@�`�����l���̔��K��
					{
						dstb -= NewPitch * 3 ;
						dstb[  0 ] = ( BYTE )( dstb[  0 ] * 255 / dstb[  3 ] ) ;
						dstb[  1 ] = ( BYTE )( dstb[  1 ] * 255 / dstb[  3 ] ) ;
						dstb[  2 ] = ( BYTE )( dstb[  2 ] * 255 / dstb[  3 ] ) ;

						dstb[  4 ] = ( BYTE )( dstb[  4 ] * 255 / dstb[  7 ] ) ;
						dstb[  5 ] = ( BYTE )( dstb[  5 ] * 255 / dstb[  7 ] ) ;
						dstb[  6 ] = ( BYTE )( dstb[  6 ] * 255 / dstb[  7 ] ) ;

						dstb[  8 ] = ( BYTE )( dstb[  8 ] * 255 / dstb[ 11 ] ) ;
						dstb[  9 ] = ( BYTE )( dstb[  9 ] * 255 / dstb[ 11 ] ) ;
						dstb[ 10 ] = ( BYTE )( dstb[ 10 ] * 255 / dstb[ 11 ] ) ;

						dstb[ 12 ] = ( BYTE )( dstb[ 12 ] * 255 / dstb[ 15 ] ) ;
						dstb[ 13 ] = ( BYTE )( dstb[ 13 ] * 255 / dstb[ 15 ] ) ;
						dstb[ 14 ] = ( BYTE )( dstb[ 14 ] * 255 / dstb[ 15 ] ) ;
						dstb = dstb + NewPitch ;

						dstb[  0 ] = ( BYTE )( dstb[  0 ] * 255 / dstb[  3 ] ) ;
						dstb[  1 ] = ( BYTE )( dstb[  1 ] * 255 / dstb[  3 ] ) ;
						dstb[  2 ] = ( BYTE )( dstb[  2 ] * 255 / dstb[  3 ] ) ;

						dstb[  4 ] = ( BYTE )( dstb[  4 ] * 255 / dstb[  7 ] ) ;
						dstb[  5 ] = ( BYTE )( dstb[  5 ] * 255 / dstb[  7 ] ) ;
						dstb[  6 ] = ( BYTE )( dstb[  6 ] * 255 / dstb[  7 ] ) ;

						dstb[  8 ] = ( BYTE )( dstb[  8 ] * 255 / dstb[ 11 ] ) ;
						dstb[  9 ] = ( BYTE )( dstb[  9 ] * 255 / dstb[ 11 ] ) ;
						dstb[ 10 ] = ( BYTE )( dstb[ 10 ] * 255 / dstb[ 11 ] ) ;

						dstb[ 12 ] = ( BYTE )( dstb[ 12 ] * 255 / dstb[ 15 ] ) ;
						dstb[ 13 ] = ( BYTE )( dstb[ 13 ] * 255 / dstb[ 15 ] ) ;
						dstb[ 14 ] = ( BYTE )( dstb[ 14 ] * 255 / dstb[ 15 ] ) ;
						dstb = dstb + NewPitch ;

						dstb[  0 ] = ( BYTE )( dstb[  0 ] * 255 / dstb[  3 ] ) ;
						dstb[  1 ] = ( BYTE )( dstb[  1 ] * 255 / dstb[  3 ] ) ;
						dstb[  2 ] = ( BYTE )( dstb[  2 ] * 255 / dstb[  3 ] ) ;

						dstb[  4 ] = ( BYTE )( dstb[  4 ] * 255 / dstb[  7 ] ) ;
						dstb[  5 ] = ( BYTE )( dstb[  5 ] * 255 / dstb[  7 ] ) ;
						dstb[  6 ] = ( BYTE )( dstb[  6 ] * 255 / dstb[  7 ] ) ;

						dstb[  8 ] = ( BYTE )( dstb[  8 ] * 255 / dstb[ 11 ] ) ;
						dstb[  9 ] = ( BYTE )( dstb[  9 ] * 255 / dstb[ 11 ] ) ;
						dstb[ 10 ] = ( BYTE )( dstb[ 10 ] * 255 / dstb[ 11 ] ) ;

						dstb[ 12 ] = ( BYTE )( dstb[ 12 ] * 255 / dstb[ 15 ] ) ;
						dstb[ 13 ] = ( BYTE )( dstb[ 13 ] * 255 / dstb[ 15 ] ) ;
						dstb[ 14 ] = ( BYTE )( dstb[ 14 ] * 255 / dstb[ 15 ] ) ;
						dstb = dstb + NewPitch ;

						dstb[  0 ] = ( BYTE )( dstb[  0 ] * 255 / dstb[  3 ] ) ;
						dstb[  1 ] = ( BYTE )( dstb[  1 ] * 255 / dstb[  3 ] ) ;
						dstb[  2 ] = ( BYTE )( dstb[  2 ] * 255 / dstb[  3 ] ) ;

						dstb[  4 ] = ( BYTE )( dstb[  4 ] * 255 / dstb[  7 ] ) ;
						dstb[  5 ] = ( BYTE )( dstb[  5 ] * 255 / dstb[  7 ] ) ;
						dstb[  6 ] = ( BYTE )( dstb[  6 ] * 255 / dstb[  7 ] ) ;

						dstb[  8 ] = ( BYTE )( dstb[  8 ] * 255 / dstb[ 11 ] ) ;
						dstb[  9 ] = ( BYTE )( dstb[  9 ] * 255 / dstb[ 11 ] ) ;
						dstb[ 10 ] = ( BYTE )( dstb[ 10 ] * 255 / dstb[ 11 ] ) ;

						dstb[ 12 ] = ( BYTE )( dstb[ 12 ] * 255 / dstb[ 15 ] ) ;
						dstb[ 13 ] = ( BYTE )( dstb[ 13 ] * 255 / dstb[ 15 ] ) ;
						dstb[ 14 ] = ( BYTE )( dstb[ 14 ] * 255 / dstb[ 15 ] ) ;
					}
				}
			}
			break ;

		case DX_BASEIMAGE_FORMAT_DXT3 :
			for( i = 0 ; i < ph ; i ++ )
			{
				for( j = 0 ; j < pw ; j ++ )
				{
					// �J���[�̏���
					{
						dstb = bdstb + ( j * 4 ) * NewPixelByte + ( i * 4 ) * NewPitch ;
						dstd = ( DWORD * )dstb ;
						C[ 0 ][ 0 ] = ( BYTE )( ( ( ( ( WORD * )srcd )[ 0 ] >>  0 ) & 31 ) * 255 / 31 ) ; 
						C[ 0 ][ 1 ] = ( BYTE )( ( ( ( ( WORD * )srcd )[ 0 ] >>  5 ) & 63 ) * 255 / 63 ) ; 
						C[ 0 ][ 2 ] = ( BYTE )( ( ( ( ( WORD * )srcd )[ 0 ] >> 11 ) & 31 ) * 255 / 31 ) ; 
						C[ 0 ][ 3 ] = 255 ;

						C[ 1 ][ 0 ] = ( BYTE )( ( ( ( ( WORD * )srcd )[ 1 ] >>  0 ) & 31 ) * 255 / 31 ) ; 
						C[ 1 ][ 1 ] = ( BYTE )( ( ( ( ( WORD * )srcd )[ 1 ] >>  5 ) & 63 ) * 255 / 63 ) ; 
						C[ 1 ][ 2 ] = ( BYTE )( ( ( ( ( WORD * )srcd )[ 1 ] >> 11 ) & 31 ) * 255 / 31 ) ; 
						C[ 1 ][ 3 ] = 255 ;

						C[ 2 ][ 0 ] = ( BYTE )( ( C[ 0 ][ 0 ] * 2 + C[ 1 ][ 0 ] ) / 3 ) ;
						C[ 2 ][ 1 ] = ( BYTE )( ( C[ 0 ][ 1 ] * 2 + C[ 1 ][ 1 ] ) / 3 ) ;
						C[ 2 ][ 2 ] = ( BYTE )( ( C[ 0 ][ 2 ] * 2 + C[ 1 ][ 2 ] ) / 3 ) ;
						C[ 2 ][ 3 ] = 255 ;

						C[ 3 ][ 0 ] = ( BYTE )( ( C[ 0 ][ 0 ] + C[ 1 ][ 0 ] * 2 ) / 3 ) ;
						C[ 3 ][ 1 ] = ( BYTE )( ( C[ 0 ][ 1 ] + C[ 1 ][ 1 ] * 2 ) / 3 ) ;
						C[ 3 ][ 2 ] = ( BYTE )( ( C[ 0 ][ 2 ] + C[ 1 ][ 2 ] * 2 ) / 3 ) ;
						C[ 3 ][ 3 ] = 255 ;

						code = *( ( DWORD * )( srcd + 4 ) ) ;
						dstd[ 0 ] = *( ( DWORD * )C[ ( code >>  0 ) & 3 ] ) ;
						dstd[ 1 ] = *( ( DWORD * )C[ ( code >>  2 ) & 3 ] ) ;
						dstd[ 2 ] = *( ( DWORD * )C[ ( code >>  4 ) & 3 ] ) ;
						dstd[ 3 ] = *( ( DWORD * )C[ ( code >>  6 ) & 3 ] ) ;
						dstd = ( DWORD * )( ( BYTE * )dstd + NewPitch ) ;

						dstd[ 0 ] = *( ( DWORD * )C[ ( code >>  8 ) & 3 ] ) ;
						dstd[ 1 ] = *( ( DWORD * )C[ ( code >> 10 ) & 3 ] ) ;
						dstd[ 2 ] = *( ( DWORD * )C[ ( code >> 12 ) & 3 ] ) ;
						dstd[ 3 ] = *( ( DWORD * )C[ ( code >> 14 ) & 3 ] ) ;
						dstd = ( DWORD * )( ( BYTE * )dstd + NewPitch ) ;

						dstd[ 0 ] = *( ( DWORD * )C[ ( code >> 16 ) & 3 ] ) ;
						dstd[ 1 ] = *( ( DWORD * )C[ ( code >> 18 ) & 3 ] ) ;
						dstd[ 2 ] = *( ( DWORD * )C[ ( code >> 20 ) & 3 ] ) ;
						dstd[ 3 ] = *( ( DWORD * )C[ ( code >> 22 ) & 3 ] ) ;
						dstd = ( DWORD * )( ( BYTE * )dstd + NewPitch ) ;

						dstd[ 0 ] = *( ( DWORD * )C[ ( code >> 24 ) & 3 ] ) ;
						dstd[ 1 ] = *( ( DWORD * )C[ ( code >> 26 ) & 3 ] ) ;
						dstd[ 2 ] = *( ( DWORD * )C[ ( code >> 28 ) & 3 ] ) ;
						dstd[ 3 ] = *( ( DWORD * )C[ ( code >> 30 ) & 3 ] ) ;
					}
					srcd += 8 ;

					// �A���t�@�`�����l���̃Z�b�g
					{
						code = *( ( DWORD * )srcd ) ;
						dstb[  3 ] = ( BYTE )( ( ( code >>  0 ) & 15 ) * 255 / 15 ) ;
						dstb[  7 ] = ( BYTE )( ( ( code >>  4 ) & 15 ) * 255 / 15 ) ;
						dstb[ 11 ] = ( BYTE )( ( ( code >>  8 ) & 15 ) * 255 / 15 ) ;
						dstb[ 15 ] = ( BYTE )( ( ( code >> 12 ) & 15 ) * 255 / 15 ) ;
						dstb = dstb + NewPitch ;

						dstb[  3 ] = ( BYTE )( ( ( code >> 16 ) & 15 ) * 255 / 15 ) ;
						dstb[  7 ] = ( BYTE )( ( ( code >> 20 ) & 15 ) * 255 / 15 ) ;
						dstb[ 11 ] = ( BYTE )( ( ( code >> 24 ) & 15 ) * 255 / 15 ) ;
						dstb[ 15 ] = ( BYTE )( ( ( code >> 28 ) & 15 ) * 255 / 15 ) ;
						dstb = dstb + NewPitch ;

						code = *( ( DWORD * )( srcd + 4 ) ) ;
						dstb[  3 ] = ( BYTE )( ( ( code >>  0 ) & 15 ) * 255 / 15 ) ;
						dstb[  7 ] = ( BYTE )( ( ( code >>  4 ) & 15 ) * 255 / 15 ) ;
						dstb[ 11 ] = ( BYTE )( ( ( code >>  8 ) & 15 ) * 255 / 15 ) ;
						dstb[ 15 ] = ( BYTE )( ( ( code >> 12 ) & 15 ) * 255 / 15 ) ;
						dstb = dstb + NewPitch ;

						dstb[  3 ] = ( BYTE )( ( ( code >> 16 ) & 15 ) * 255 / 15 ) ;
						dstb[  7 ] = ( BYTE )( ( ( code >> 20 ) & 15 ) * 255 / 15 ) ;
						dstb[ 11 ] = ( BYTE )( ( ( code >> 24 ) & 15 ) * 255 / 15 ) ;
						dstb[ 15 ] = ( BYTE )( ( ( code >> 28 ) & 15 ) * 255 / 15 ) ;
					}
					srcd += 8 ;
				}
			}
			break ;

		case DX_BASEIMAGE_FORMAT_DXT4 :
			for( i = 0 ; i < ph ; i ++ )
			{
				for( j = 0 ; j < pw ; j ++ )
				{
					// �J���[�̏���
					{
						dstb = bdstb + ( j * 4 ) * NewPixelByte + ( i * 4 ) * NewPitch ;
						dstd = ( DWORD * )dstb ;
						C[ 0 ][ 0 ] = ( BYTE )( ( ( ( ( WORD * )srcd )[ 0 ] >>  0 ) & 31 ) * 255 / 31 ) ;
						C[ 0 ][ 1 ] = ( BYTE )( ( ( ( ( WORD * )srcd )[ 0 ] >>  5 ) & 63 ) * 255 / 63 ) ;
						C[ 0 ][ 2 ] = ( BYTE )( ( ( ( ( WORD * )srcd )[ 0 ] >> 11 ) & 31 ) * 255 / 31 ) ;
						C[ 0 ][ 3 ] = 255 ;

						C[ 1 ][ 0 ] = ( BYTE )( ( ( ( ( WORD * )srcd )[ 1 ] >>  0 ) & 31 ) * 255 / 31 ) ;
						C[ 1 ][ 1 ] = ( BYTE )( ( ( ( ( WORD * )srcd )[ 1 ] >>  5 ) & 63 ) * 255 / 63 ) ;
						C[ 1 ][ 2 ] = ( BYTE )( ( ( ( ( WORD * )srcd )[ 1 ] >> 11 ) & 31 ) * 255 / 31 ) ;
						C[ 1 ][ 3 ] = 255 ;

						C[ 2 ][ 0 ] = ( BYTE )( ( C[ 0 ][ 0 ] * 2 + C[ 1 ][ 0 ] ) / 3 ) ;
						C[ 2 ][ 1 ] = ( BYTE )( ( C[ 0 ][ 1 ] * 2 + C[ 1 ][ 1 ] ) / 3 ) ;
						C[ 2 ][ 2 ] = ( BYTE )( ( C[ 0 ][ 2 ] * 2 + C[ 1 ][ 2 ] ) / 3 ) ;
						C[ 2 ][ 3 ] = 255 ;

						C[ 3 ][ 0 ] = ( BYTE )( ( C[ 0 ][ 0 ] + C[ 1 ][ 0 ] * 2 ) / 3 ) ;
						C[ 3 ][ 1 ] = ( BYTE )( ( C[ 0 ][ 1 ] + C[ 1 ][ 1 ] * 2 ) / 3 ) ;
						C[ 3 ][ 2 ] = ( BYTE )( ( C[ 0 ][ 2 ] + C[ 1 ][ 2 ] * 2 ) / 3 ) ;
						C[ 3 ][ 3 ] = 255 ;

						code = *( ( DWORD * )( srcd + 4 ) ) ;
						dstd[ 0 ] = *( ( DWORD * )C[ ( code >>  0 ) & 3 ] ) ;
						dstd[ 1 ] = *( ( DWORD * )C[ ( code >>  2 ) & 3 ] ) ;
						dstd[ 2 ] = *( ( DWORD * )C[ ( code >>  4 ) & 3 ] ) ;
						dstd[ 3 ] = *( ( DWORD * )C[ ( code >>  6 ) & 3 ] ) ;
						dstd = ( DWORD * )( ( BYTE * )dstd + NewPitch ) ;

						dstd[ 0 ] = *( ( DWORD * )C[ ( code >>  8 ) & 3 ] ) ;
						dstd[ 1 ] = *( ( DWORD * )C[ ( code >> 10 ) & 3 ] ) ;
						dstd[ 2 ] = *( ( DWORD * )C[ ( code >> 12 ) & 3 ] ) ;
						dstd[ 3 ] = *( ( DWORD * )C[ ( code >> 14 ) & 3 ] ) ;
						dstd = ( DWORD * )( ( BYTE * )dstd + NewPitch ) ;

						dstd[ 0 ] = *( ( DWORD * )C[ ( code >> 16 ) & 3 ] ) ;
						dstd[ 1 ] = *( ( DWORD * )C[ ( code >> 18 ) & 3 ] ) ;
						dstd[ 2 ] = *( ( DWORD * )C[ ( code >> 20 ) & 3 ] ) ;
						dstd[ 3 ] = *( ( DWORD * )C[ ( code >> 22 ) & 3 ] ) ;
						dstd = ( DWORD * )( ( BYTE * )dstd + NewPitch ) ;

						dstd[ 0 ] = *( ( DWORD * )C[ ( code >> 24 ) & 3 ] ) ;
						dstd[ 1 ] = *( ( DWORD * )C[ ( code >> 26 ) & 3 ] ) ;
						dstd[ 2 ] = *( ( DWORD * )C[ ( code >> 28 ) & 3 ] ) ;
						dstd[ 3 ] = *( ( DWORD * )C[ ( code >> 30 ) & 3 ] ) ;
					}
					srcd += 8 ;

					// �A���t�@�`�����l���̃Z�b�g
					{
						A[ 0 ] = srcd[ 0 ] ;
						A[ 1 ] = srcd[ 1 ] ;
						if( A[ 0 ] > A[ 1 ] )
						{
							A[ 2 ] = ( BYTE )( ( A[ 0 ] * 6 + A[ 1 ] * 1 ) / 7 ) ;
							A[ 3 ] = ( BYTE )( ( A[ 0 ] * 5 + A[ 1 ] * 2 ) / 7 ) ;
							A[ 4 ] = ( BYTE )( ( A[ 0 ] * 4 + A[ 1 ] * 3 ) / 7 ) ;
							A[ 5 ] = ( BYTE )( ( A[ 0 ] * 3 + A[ 1 ] * 4 ) / 7 ) ;
							A[ 6 ] = ( BYTE )( ( A[ 0 ] * 2 + A[ 1 ] * 5 ) / 7 ) ;
							A[ 7 ] = ( BYTE )( ( A[ 0 ] * 1 + A[ 1 ] * 6 ) / 7 ) ;
						}
						else
						{
							A[ 2 ] = ( BYTE )( ( A[ 0 ] * 4 + A[ 1 ] * 1 ) / 5 ) ;
							A[ 3 ] = ( BYTE )( ( A[ 0 ] * 3 + A[ 1 ] * 2 ) / 5 ) ;
							A[ 4 ] = ( BYTE )( ( A[ 0 ] * 2 + A[ 1 ] * 3 ) / 5 ) ;
							A[ 5 ] = ( BYTE )( ( A[ 0 ] * 1 + A[ 1 ] * 4 ) / 5 ) ;
							A[ 6 ] = 0 ;
							A[ 7 ] = 255 ;
						}

						code = ( DWORD )( srcd[ 2 ] | ( srcd[ 3 ] << 8 ) | ( srcd[ 4 ] << 16 ) ) ;
						dstb[  3 ] = A[ ( code >>  0 ) & 7 ] ;
						dstb[  7 ] = A[ ( code >>  3 ) & 7 ] ;
						dstb[ 11 ] = A[ ( code >>  6 ) & 7 ] ;
						dstb[ 15 ] = A[ ( code >>  9 ) & 7 ] ;
						dstb = dstb + NewPitch ;

						dstb[  3 ] = A[ ( code >> 12 ) & 7 ] ;
						dstb[  7 ] = A[ ( code >> 15 ) & 7 ] ;
						dstb[ 11 ] = A[ ( code >> 18 ) & 7 ] ;
						dstb[ 15 ] = A[ ( code >> 21 ) & 7 ] ;
						dstb = dstb + NewPitch ;

						code = ( DWORD )( srcd[ 5 ] | ( srcd[ 6 ] << 8 ) | ( srcd[ 7 ] << 16 ) ) ;
						dstb[  3 ] = A[ ( code >>  0 ) & 7 ] ;
						dstb[  7 ] = A[ ( code >>  3 ) & 7 ] ;
						dstb[ 11 ] = A[ ( code >>  6 ) & 7 ] ;
						dstb[ 15 ] = A[ ( code >>  9 ) & 7 ] ;
						dstb = dstb + NewPitch ;

						dstb[  3 ] = A[ ( code >> 12 ) & 7 ] ;
						dstb[  7 ] = A[ ( code >> 15 ) & 7 ] ;
						dstb[ 11 ] = A[ ( code >> 18 ) & 7 ] ;
						dstb[ 15 ] = A[ ( code >> 21 ) & 7 ] ;
					}
					srcd += 8 ;

					// �A���t�@�`�����l���̔��K��
					{
						dstb -= NewPitch * 3 ;
						dstb[  0 ] = ( BYTE )( dstb[  0 ] * 255 / dstb[  3 ] ) ;
						dstb[  1 ] = ( BYTE )( dstb[  1 ] * 255 / dstb[  3 ] ) ;
						dstb[  2 ] = ( BYTE )( dstb[  2 ] * 255 / dstb[  3 ] ) ;

						dstb[  4 ] = ( BYTE )( dstb[  4 ] * 255 / dstb[  7 ] ) ;
						dstb[  5 ] = ( BYTE )( dstb[  5 ] * 255 / dstb[  7 ] ) ;
						dstb[  6 ] = ( BYTE )( dstb[  6 ] * 255 / dstb[  7 ] ) ;

						dstb[  8 ] = ( BYTE )( dstb[  8 ] * 255 / dstb[ 11 ] ) ;
						dstb[  9 ] = ( BYTE )( dstb[  9 ] * 255 / dstb[ 11 ] ) ;
						dstb[ 10 ] = ( BYTE )( dstb[ 10 ] * 255 / dstb[ 11 ] ) ;

						dstb[ 12 ] = ( BYTE )( dstb[ 12 ] * 255 / dstb[ 15 ] ) ;
						dstb[ 13 ] = ( BYTE )( dstb[ 13 ] * 255 / dstb[ 15 ] ) ;
						dstb[ 14 ] = ( BYTE )( dstb[ 14 ] * 255 / dstb[ 15 ] ) ;
						dstb = dstb + NewPitch ;

						dstb[  0 ] = ( BYTE )( dstb[  0 ] * 255 / dstb[  3 ] ) ;
						dstb[  1 ] = ( BYTE )( dstb[  1 ] * 255 / dstb[  3 ] ) ;
						dstb[  2 ] = ( BYTE )( dstb[  2 ] * 255 / dstb[  3 ] ) ;

						dstb[  4 ] = ( BYTE )( dstb[  4 ] * 255 / dstb[  7 ] ) ;
						dstb[  5 ] = ( BYTE )( dstb[  5 ] * 255 / dstb[  7 ] ) ;
						dstb[  6 ] = ( BYTE )( dstb[  6 ] * 255 / dstb[  7 ] ) ;

						dstb[  8 ] = ( BYTE )( dstb[  8 ] * 255 / dstb[ 11 ] ) ;
						dstb[  9 ] = ( BYTE )( dstb[  9 ] * 255 / dstb[ 11 ] ) ;
						dstb[ 10 ] = ( BYTE )( dstb[ 10 ] * 255 / dstb[ 11 ] ) ;

						dstb[ 12 ] = ( BYTE )( dstb[ 12 ] * 255 / dstb[ 15 ] ) ;
						dstb[ 13 ] = ( BYTE )( dstb[ 13 ] * 255 / dstb[ 15 ] ) ;
						dstb[ 14 ] = ( BYTE )( dstb[ 14 ] * 255 / dstb[ 15 ] ) ;
						dstb = dstb + NewPitch ;

						dstb[  0 ] = ( BYTE )( dstb[  0 ] * 255 / dstb[  3 ] ) ;
						dstb[  1 ] = ( BYTE )( dstb[  1 ] * 255 / dstb[  3 ] ) ;
						dstb[  2 ] = ( BYTE )( dstb[  2 ] * 255 / dstb[  3 ] ) ;

						dstb[  4 ] = ( BYTE )( dstb[  4 ] * 255 / dstb[  7 ] ) ;
						dstb[  5 ] = ( BYTE )( dstb[  5 ] * 255 / dstb[  7 ] ) ;
						dstb[  6 ] = ( BYTE )( dstb[  6 ] * 255 / dstb[  7 ] ) ;

						dstb[  8 ] = ( BYTE )( dstb[  8 ] * 255 / dstb[ 11 ] ) ;
						dstb[  9 ] = ( BYTE )( dstb[  9 ] * 255 / dstb[ 11 ] ) ;
						dstb[ 10 ] = ( BYTE )( dstb[ 10 ] * 255 / dstb[ 11 ] ) ;

						dstb[ 12 ] = ( BYTE )( dstb[ 12 ] * 255 / dstb[ 15 ] ) ;
						dstb[ 13 ] = ( BYTE )( dstb[ 13 ] * 255 / dstb[ 15 ] ) ;
						dstb[ 14 ] = ( BYTE )( dstb[ 14 ] * 255 / dstb[ 15 ] ) ;
						dstb = dstb + NewPitch ;

						dstb[  0 ] = ( BYTE )( dstb[  0 ] * 255 / dstb[  3 ] ) ;
						dstb[  1 ] = ( BYTE )( dstb[  1 ] * 255 / dstb[  3 ] ) ;
						dstb[  2 ] = ( BYTE )( dstb[  2 ] * 255 / dstb[  3 ] ) ;

						dstb[  4 ] = ( BYTE )( dstb[  4 ] * 255 / dstb[  7 ] ) ;
						dstb[  5 ] = ( BYTE )( dstb[  5 ] * 255 / dstb[  7 ] ) ;
						dstb[  6 ] = ( BYTE )( dstb[  6 ] * 255 / dstb[  7 ] ) ;

						dstb[  8 ] = ( BYTE )( dstb[  8 ] * 255 / dstb[ 11 ] ) ;
						dstb[  9 ] = ( BYTE )( dstb[  9 ] * 255 / dstb[ 11 ] ) ;
						dstb[ 10 ] = ( BYTE )( dstb[ 10 ] * 255 / dstb[ 11 ] ) ;

						dstb[ 12 ] = ( BYTE )( dstb[ 12 ] * 255 / dstb[ 15 ] ) ;
						dstb[ 13 ] = ( BYTE )( dstb[ 13 ] * 255 / dstb[ 15 ] ) ;
						dstb[ 14 ] = ( BYTE )( dstb[ 14 ] * 255 / dstb[ 15 ] ) ;
					}
				}
			}
			break ;

		case DX_BASEIMAGE_FORMAT_DXT5 :
			for( i = 0 ; i < ph ; i ++ )
			{
				for( j = 0 ; j < pw ; j ++ )
				{
					// �J���[�̏���
					{
						dstb = bdstb + ( j * 4 ) * NewPixelByte + ( i * 4 ) * NewPitch ;
						dstd = ( DWORD * )dstb ;
						C[ 0 ][ 0 ] = ( BYTE )( ( ( ( ( WORD * )srcd )[ 0 ] >>  0 ) & 31 ) * 255 / 31 ) ;
						C[ 0 ][ 1 ] = ( BYTE )( ( ( ( ( WORD * )srcd )[ 0 ] >>  5 ) & 63 ) * 255 / 63 ) ;
						C[ 0 ][ 2 ] = ( BYTE )( ( ( ( ( WORD * )srcd )[ 0 ] >> 11 ) & 31 ) * 255 / 31 ) ;
						C[ 0 ][ 3 ] = 255 ;

						C[ 1 ][ 0 ] = ( BYTE )( ( ( ( ( WORD * )srcd )[ 1 ] >>  0 ) & 31 ) * 255 / 31 ) ;
						C[ 1 ][ 1 ] = ( BYTE )( ( ( ( ( WORD * )srcd )[ 1 ] >>  5 ) & 63 ) * 255 / 63 ) ;
						C[ 1 ][ 2 ] = ( BYTE )( ( ( ( ( WORD * )srcd )[ 1 ] >> 11 ) & 31 ) * 255 / 31 ) ;
						C[ 1 ][ 3 ] = 255 ;

						C[ 2 ][ 0 ] = ( BYTE )( ( C[ 0 ][ 0 ] * 2 + C[ 1 ][ 0 ] ) / 3 ) ;
						C[ 2 ][ 1 ] = ( BYTE )( ( C[ 0 ][ 1 ] * 2 + C[ 1 ][ 1 ] ) / 3 ) ;
						C[ 2 ][ 2 ] = ( BYTE )( ( C[ 0 ][ 2 ] * 2 + C[ 1 ][ 2 ] ) / 3 ) ;
						C[ 2 ][ 3 ] = 255 ;

						C[ 3 ][ 0 ] = ( BYTE )( ( C[ 0 ][ 0 ] + C[ 1 ][ 0 ] * 2 ) / 3 ) ;
						C[ 3 ][ 1 ] = ( BYTE )( ( C[ 0 ][ 1 ] + C[ 1 ][ 1 ] * 2 ) / 3 ) ;
						C[ 3 ][ 2 ] = ( BYTE )( ( C[ 0 ][ 2 ] + C[ 1 ][ 2 ] * 2 ) / 3 ) ;
						C[ 3 ][ 3 ] = 255 ;

						code = *( ( DWORD * )( srcd + 4 ) ) ;
						dstd[ 0 ] = *( ( DWORD * )C[ ( code >>  0 ) & 3 ] ) ;
						dstd[ 1 ] = *( ( DWORD * )C[ ( code >>  2 ) & 3 ] ) ;
						dstd[ 2 ] = *( ( DWORD * )C[ ( code >>  4 ) & 3 ] ) ;
						dstd[ 3 ] = *( ( DWORD * )C[ ( code >>  6 ) & 3 ] ) ;
						dstd = ( DWORD * )( ( BYTE * )dstd + NewPitch ) ;

						dstd[ 0 ] = *( ( DWORD * )C[ ( code >>  8 ) & 3 ] ) ;
						dstd[ 1 ] = *( ( DWORD * )C[ ( code >> 10 ) & 3 ] ) ;
						dstd[ 2 ] = *( ( DWORD * )C[ ( code >> 12 ) & 3 ] ) ;
						dstd[ 3 ] = *( ( DWORD * )C[ ( code >> 14 ) & 3 ] ) ;
						dstd = ( DWORD * )( ( BYTE * )dstd + NewPitch ) ;

						dstd[ 0 ] = *( ( DWORD * )C[ ( code >> 16 ) & 3 ] ) ;
						dstd[ 1 ] = *( ( DWORD * )C[ ( code >> 18 ) & 3 ] ) ;
						dstd[ 2 ] = *( ( DWORD * )C[ ( code >> 20 ) & 3 ] ) ;
						dstd[ 3 ] = *( ( DWORD * )C[ ( code >> 22 ) & 3 ] ) ;
						dstd = ( DWORD * )( ( BYTE * )dstd + NewPitch ) ;

						dstd[ 0 ] = *( ( DWORD * )C[ ( code >> 24 ) & 3 ] ) ;
						dstd[ 1 ] = *( ( DWORD * )C[ ( code >> 26 ) & 3 ] ) ;
						dstd[ 2 ] = *( ( DWORD * )C[ ( code >> 28 ) & 3 ] ) ;
						dstd[ 3 ] = *( ( DWORD * )C[ ( code >> 30 ) & 3 ] ) ;
					}
					srcd += 8 ;

					// �A���t�@�`�����l���̃Z�b�g
					{
						A[ 0 ] = srcd[ 0 ] ;
						A[ 1 ] = srcd[ 1 ] ;
						if( A[ 0 ] > A[ 1 ] )
						{
							A[ 2 ] = ( BYTE )( ( A[ 0 ] * 6 + A[ 1 ] * 1 ) / 7 ) ;
							A[ 3 ] = ( BYTE )( ( A[ 0 ] * 5 + A[ 1 ] * 2 ) / 7 ) ;
							A[ 4 ] = ( BYTE )( ( A[ 0 ] * 4 + A[ 1 ] * 3 ) / 7 ) ;
							A[ 5 ] = ( BYTE )( ( A[ 0 ] * 3 + A[ 1 ] * 4 ) / 7 ) ;
							A[ 6 ] = ( BYTE )( ( A[ 0 ] * 2 + A[ 1 ] * 5 ) / 7 ) ;
							A[ 7 ] = ( BYTE )( ( A[ 0 ] * 1 + A[ 1 ] * 6 ) / 7 ) ;
						}
						else
						{
							A[ 2 ] = ( BYTE )( ( A[ 0 ] * 4 + A[ 1 ] * 1 ) / 5 ) ;
							A[ 3 ] = ( BYTE )( ( A[ 0 ] * 3 + A[ 1 ] * 2 ) / 5 ) ;
							A[ 4 ] = ( BYTE )( ( A[ 0 ] * 2 + A[ 1 ] * 3 ) / 5 ) ;
							A[ 5 ] = ( BYTE )( ( A[ 0 ] * 1 + A[ 1 ] * 4 ) / 5 ) ;
							A[ 6 ] = 0 ;
							A[ 7 ] = 255 ;
						}

						code = ( DWORD )( srcd[ 2 ] | ( srcd[ 3 ] << 8 ) | ( srcd[ 4 ] << 16 ) ) ;
						dstb[  3 ] = A[ ( code >>  0 ) & 7 ] ;
						dstb[  7 ] = A[ ( code >>  3 ) & 7 ] ;
						dstb[ 11 ] = A[ ( code >>  6 ) & 7 ] ;
						dstb[ 15 ] = A[ ( code >>  9 ) & 7 ] ;
						dstb = dstb + NewPitch ;

						dstb[  3 ] = A[ ( code >> 12 ) & 7 ] ;
						dstb[  7 ] = A[ ( code >> 15 ) & 7 ] ;
						dstb[ 11 ] = A[ ( code >> 18 ) & 7 ] ;
						dstb[ 15 ] = A[ ( code >> 21 ) & 7 ] ;
						dstb = dstb + NewPitch ;

						code = ( DWORD )( srcd[ 5 ] | ( srcd[ 6 ] << 8 ) | ( srcd[ 7 ] << 16 ) ) ;
						dstb[  3 ] = A[ ( code >>  0 ) & 7 ] ;
						dstb[  7 ] = A[ ( code >>  3 ) & 7 ] ;
						dstb[ 11 ] = A[ ( code >>  6 ) & 7 ] ;
						dstb[ 15 ] = A[ ( code >>  9 ) & 7 ] ;
						dstb = dstb + NewPitch ;

						dstb[  3 ] = A[ ( code >> 12 ) & 7 ] ;
						dstb[  7 ] = A[ ( code >> 15 ) & 7 ] ;
						dstb[ 11 ] = A[ ( code >> 18 ) & 7 ] ;
						dstb[ 15 ] = A[ ( code >> 21 ) & 7 ] ;
					}
					srcd += 8 ;
				}
			}
			break ;
		}
	}

	// ���܂ł̃f�[�^��������ĐV�����f�[�^�̃|�C���^���Z�b�g
	if( ReleaseOrigGraphData )
	{
		DXFREE( BaseImage->GraphData ) ;
	}
	BaseImage->GraphData = NewBuffer ;

	// �J���[�f�[�^�� ARGB8 �ɂ���
	NS_CreateARGB8ColorData( &BaseImage->ColorData ) ;

	// �s�b�`���V�����摜�ł̃s�b�`���Z�b�g����
	BaseImage->Pitch = ( int )NewPitch ;

	// ����I��
	return 0 ;
}

// �ʏ�̃��`�����l���t���摜����Z�ς݃��`�����l���t���摜�ɕϊ�����( �s�N�Z���t�H�[�}�b�g�� ARGB8 �ȊO�̏ꍇ�� ARGB8 �ɕϊ�����܂� )
extern int NS_ConvertPremulAlphaBaseImage( BASEIMAGE *BaseImage )
{
	DWORD i ;
	DWORD j ;
	BYTE *p ;

	// ���`�����l���̖����摜�t�H�[�}�b�g�̏ꍇ�͉������Ȃ�
	if( BaseImage->ColorData.AlphaWidth == 0 )
		return 0 ;

	// �t�H�[�}�b�g���W���t�H�[�}�b�g�ł͂Ȃ�������W���t�H�[�}�b�g�ɕϊ�����
	if( BaseImage->ColorData.Format != DX_BASEIMAGE_FORMAT_NORMAL )
	{
		if( NS_ConvertNormalFormatBaseImage( BaseImage ) < 0 )
			return -1 ;
	}

	// ARGB8 �ȊO�̏ꍇ�� ARGB8 �ɕϊ�����
	if( BaseImage->ColorData.ColorBitDepth != 32 ||
		BaseImage->ColorData.PixelByte != 4 ||
		BaseImage->ColorData.FloatTypeFlag != FALSE ||
		BaseImage->ColorData.NoneMask  != 0x00000000 ||
		BaseImage->ColorData.AlphaMask != 0xff000000 ||
		BaseImage->ColorData.RedMask   != 0x00ff0000 ||
		BaseImage->ColorData.GreenMask != 0x0000ff00 ||
		BaseImage->ColorData.BlueMask  != 0x000000ff )
	{
		BASEIMAGE TempBaseImage ;
		TempBaseImage = *BaseImage ;
		NS_CreateARGB8ColorBaseImage( BaseImage->Width, BaseImage->Height, BaseImage ) ;
		NS_BltBaseImage( 0, 0, &TempBaseImage, BaseImage ) ;
		NS_ReleaseBaseImage( &TempBaseImage ) ;
	}

	for( i = 0 ; i < ( DWORD )BaseImage->Height ; i ++ )
	{
		p = ( BYTE * )BaseImage->GraphData + i * BaseImage->Pitch ;
		for( j = 0 ; j < ( DWORD )BaseImage->Width ; j ++, p += 4 )
		{
			p[ 0 ] = ( BYTE )( p[ 0 ] * p[ 3 ] / 255 ) ;
			p[ 1 ] = ( BYTE )( p[ 1 ] * p[ 3 ] / 255 ) ;
			p[ 2 ] = ( BYTE )( p[ 2 ] * p[ 3 ] / 255 ) ;
		}
	}

	// ����I��
	return 0 ;
}

// ��Z�ς݃��`�����l���t���摜��ʏ�̃��`�����l���t���摜�ɕϊ�����( �s�N�Z���t�H�[�}�b�g�� ARGB8 �ȊO�̏ꍇ�� ARGB8 �ɕϊ�����܂� )
extern int NS_ConvertInterpAlphaBaseImage( BASEIMAGE *BaseImage )
{
	DWORD i ;
	DWORD j ;
	DWORD tmp ;
	BYTE *p ;

	// ���`�����l���̖����摜�t�H�[�}�b�g�̏ꍇ�͉������Ȃ�
	if( BaseImage->ColorData.AlphaWidth == 0 )
		return 0 ;

	// �t�H�[�}�b�g���W���t�H�[�}�b�g�ł͂Ȃ�������W���t�H�[�}�b�g�ɕϊ�����
	if( BaseImage->ColorData.Format != DX_BASEIMAGE_FORMAT_NORMAL )
	{
		if( NS_ConvertNormalFormatBaseImage( BaseImage ) < 0 )
			return -1 ;
	}
	else
	{
		// ARGB8 �ȊO�̏ꍇ�� ARGB8 �ɕϊ�����
		if( BaseImage->ColorData.ColorBitDepth != 32 ||
			BaseImage->ColorData.PixelByte != 4 ||
			BaseImage->ColorData.FloatTypeFlag != FALSE ||
			BaseImage->ColorData.NoneMask  != 0x00000000 ||
			BaseImage->ColorData.AlphaMask != 0xff000000 ||
			BaseImage->ColorData.RedMask   != 0x00ff0000 ||
			BaseImage->ColorData.GreenMask != 0x0000ff00 ||
			BaseImage->ColorData.BlueMask  != 0x000000ff )
		{
			BASEIMAGE TempBaseImage ;
			TempBaseImage = *BaseImage ;
			NS_CreateARGB8ColorBaseImage( BaseImage->Width, BaseImage->Height, BaseImage ) ;
			NS_BltBaseImage( 0, 0, &TempBaseImage, BaseImage ) ;
			NS_ReleaseBaseImage( &TempBaseImage ) ;
		}
	}

	for( i = 0 ; i < ( DWORD )BaseImage->Height ; i ++ )
	{
		p = ( BYTE * )BaseImage->GraphData + i * BaseImage->Pitch ;
		for( j = 0 ; j < ( DWORD )BaseImage->Width ; j ++, p += 4 )
		{
			if( p[ 3 ] == 0 )
			{
				p[ 0 ] = 0 ;
				p[ 1 ] = 0 ;
				p[ 2 ] = 0 ;
			}
			else
			{
				tmp = ( DWORD )( p[ 0 ] * 255 / p[ 3 ] ) ; p[ 0 ] = ( BYTE )( tmp > 255 ? 255 : tmp ) ;
				tmp = ( DWORD )( p[ 1 ] * 255 / p[ 3 ] ) ; p[ 1 ] = ( BYTE )( tmp > 255 ? 255 : tmp ) ;
				tmp = ( DWORD )( p[ 2 ] * 255 / p[ 3 ] ) ; p[ 2 ] = ( BYTE )( tmp > 255 ? 255 : tmp ) ;
			}
		}
	}

	// ����I��
	return 0 ;
}

#ifdef __WINDOWS__

// ��{�C���[�W���g�p���� UpdateLayerdWindow ���s��
extern int NS_UpdateLayerdWindowForBaseImage( const BASEIMAGE *BaseImage )
{
	return UpdateBackBufferTransColorWindow( BaseImage, NULL, NULL, TRUE, FALSE ) ;
}

// ��{�C���[�W�f�[�^�̎w��͈̔͂��g�p���� UpdateLayerdWindow ���s��
extern int NS_UpdateLayerdWindowForBaseImageRect( const BASEIMAGE *BaseImage, int x1, int y1, int x2, int y2 )
{
	RECT SrcRect ;

	SrcRect.left   = x1 ;
	SrcRect.top    = y1 ;
	SrcRect.right  = x2 ;
	SrcRect.bottom = y2 ;

	return UpdateBackBufferTransColorWindow( BaseImage, &SrcRect, NULL, TRUE, FALSE ) ;
}

// ��Z�ς݃A���t�@�̊�{�C���[�W���g�p���� UpdateLayerdWindow ���s��
extern int NS_UpdateLayerdWindowForPremultipliedAlphaBaseImage( const BASEIMAGE *BaseImage )
{
	return UpdateBackBufferTransColorWindow( BaseImage, NULL, NULL, TRUE, TRUE ) ;
}

// �f�X�N�g�b�v�L���v�`��

// �f�X�N�g�b�v�̎w��̗̈����{�C���[�W�f�[�^�ɓ]������
extern int NS_GetDesktopScreenBaseImage( int x1, int y1, int x2, int y2, BASEIMAGE *BaseImage, int DestX, int DestY )
{
	HWND DesktopHWND ;
	RECT DesktopRect ;
	int DesktopW, DesktopH ;
	int CaptureW, CaptureH ;
	HDC hdc ;
	BITMAPINFO BitmapInfo ;
	void *ImageBuffer = NULL ;
	HBITMAP HBitmap ;
	HDC HMemDC ;
	BASEIMAGE CaptureImage ;

	// �����̃`�F�b�N
	if( x1 >= x2 || y1 >= y2 )
	{
		return -1 ;
	}

	// �f�X�N�g�b�v�E�C���h�E�n���h�����擾
	DesktopHWND = GetDesktopWindow() ;

	// �f�X�N�g�b�v�̋�`���擾
	GetWindowRect( DesktopHWND, &DesktopRect ) ;

	// �f�X�N�g�b�v�̃T�C�Y���擾
	DesktopW = DesktopRect.right  - DesktopRect.left ;
	DesktopH = DesktopRect.bottom - DesktopRect.top ;

	// �w��̋�`�̕␳
	if( x1 < DesktopRect.left )
	{
		DestX += DesktopRect.left - x1 ;
		x1     = DesktopRect.left ;
	}
	if( y1 < DesktopRect.top )
	{
		DestY += DesktopRect.top - y1 ;
		y1     = DesktopRect.top ;
	}
	if( x2 > DesktopRect.right )
	{
		x2 = DesktopRect.right ;
	}
	if( y2 > DesktopRect.bottom )
	{
		y2 = DesktopRect.bottom ;
	}

	// ��荞�݃T�C�Y�̎Z�o
	CaptureW = x2 - x1 ;
	CaptureH = y2 - y1 ;

	// ��荞�݃T�C�Y�`�F�b�N
	if( CaptureW <= 0 || CaptureH <= 0 )
	{
		return -1 ;
	}

	// ��荞�݃T�C�Y�␳
	if( DestX + CaptureW > BaseImage->Width )
	{
		CaptureW = BaseImage->Width - DestX ;
		x2       = x1 + CaptureW ;
	}
	if( DestY + CaptureH > BaseImage->Height )
	{
		CaptureH = BaseImage->Height - DestY ;
		y2       = y1 + CaptureH ;
	}

	// �ēx��荞�݃T�C�Y�`�F�b�N
	if( CaptureW <= 0 || CaptureH <= 0 )
	{
		return -1 ;
	}

	// DIB�̏���ݒ肷��
	_MEMSET( &BitmapInfo, 0, sizeof( BitmapInfo ) ) ;
	BitmapInfo.bmiHeader.biSize			= sizeof( BITMAPINFOHEADER ) ;
	BitmapInfo.bmiHeader.biWidth		=  CaptureW ;
	BitmapInfo.bmiHeader.biHeight		= -CaptureH ;
	BitmapInfo.bmiHeader.biPlanes		= 1 ;
	BitmapInfo.bmiHeader.biBitCount		= 32 ;
	BitmapInfo.bmiHeader.biCompression	= BI_RGB ;

	// DIB�̍쐬
	hdc			= GetDC( GetMainWindowHandle() ) ;
	HBitmap		= CreateDIBSection( hdc, &BitmapInfo, DIB_RGB_COLORS, ( void ** )&ImageBuffer, NULL, 0 ) ;
	HMemDC		= CreateCompatibleDC( hdc ) ;
	SelectObject( HMemDC, HBitmap ) ;
	ReleaseDC( GetMainWindowHandle(), hdc ) ;

	// �f�X�N�g�b�v�C���[�W����DIB�ɃR�s�[
	hdc = GetDC( DesktopHWND ) ;
	BitBlt( HMemDC, 0, 0, CaptureW, CaptureH, hdc, x1, y1, SRCCOPY ) ;
	ReleaseDC( DesktopHWND, hdc ) ;

	// �f�X�N�g�b�v�C���[�W����{�C���[�W�f�[�^�ɓ]��
	_MEMSET( &CaptureImage, 0, sizeof( CaptureImage ) ) ;
	NS_CreateXRGB8ColorData( &CaptureImage.ColorData ) ;
	CaptureImage.Width		= CaptureW ;
	CaptureImage.Height		= CaptureH ;
	CaptureImage.Pitch		= CaptureW * 4 ;
	CaptureImage.GraphData	= ( void * )ImageBuffer ;
	NS_BltBaseImage( DestX, DestY, &CaptureImage, BaseImage ) ;

	// DIB�̍폜
	DeleteDC( HMemDC ) ;
	DeleteObject( HBitmap ) ;

	// �I��
	return 0 ;
}

// ��Z�ς݃A���t�@�̊�{�C���[�W�f�[�^�̎w��͈̔͂��g�p���� UpdateLayerdWindow ���s��
extern int NS_UpdateLayerdWindowForPremultipliedAlphaBaseImageRect( const BASEIMAGE *BaseImage, int x1, int y1, int x2, int y2 )
{
	RECT SrcRect ;

	SrcRect.left   = x1 ;
	SrcRect.top    = y1 ;
	SrcRect.right  = x2 ;
	SrcRect.bottom = y2 ;

	return UpdateBackBufferTransColorWindow( BaseImage, &SrcRect, NULL, TRUE, TRUE ) ;
}

#endif // __WINDOWS__

// ��{�C���[�W�f�[�^���w��̐F�œh��Ԃ�
extern int NS_FillBaseImage( BASEIMAGE *BaseImage, int r, int g, int b, int a )
{
	unsigned int Color;
	int i, j, w, h, addpitch;
	BYTE *p;

	Color = NS_GetColor3( &BaseImage->ColorData, r, g, b, a );
	w = BaseImage->Width;
	h = BaseImage->Height;
	p = (BYTE *)BaseImage->GraphData;
	addpitch = BaseImage->Pitch - w * BaseImage->ColorData.PixelByte;
	switch( BaseImage->ColorData.PixelByte )
	{
	case 1 :
		for( i = 0; i < h; i ++, p += addpitch )
			for( j = 0; j < w; j ++, p ++ )
				*((BYTE *)p) = (BYTE)r;
		break ;

	case 2:
		for( i = 0; i < h; i ++, p += addpitch )
			for( j = 0; j < w; j ++, p += 2 )
				*((WORD *)p) = (WORD)Color;
		break;

	case 3:
		for( i = 0; i < h; i ++, p += addpitch )
			for( j = 0; j < w; j ++, p += 3 )
			{
				((WORD *)p)[0] = ((WORD *)&Color)[0];
				((BYTE *)p)[2] = ((BYTE *)&Color)[2];
			}
		break;

	case 4:
		for( i = 0; i < h; i ++, p += addpitch )
			for( j = 0; j < w; j ++, p += 4 )
				*((DWORD *)p) = (DWORD)Color;
		break;
	}

	// �I��
	return 0;
}

// ��{�C���[�W�f�[�^�̎w��̗̈���w��̐F�œh��Ԃ�
extern int NS_FillRectBaseImage( BASEIMAGE *BaseImage, int x, int y, int w, int h, int r, int g, int b, int a )
{
	DWORD	Color;
	int		i ;
	int		j ;
	int		addpitch;
	BYTE *	p;

	if( x >= BaseImage->Width  || w <= 0 ||
		y >= BaseImage->Height || h <= 0 )
	{
		return -1 ;
	}

	if( x < 0 )
	{
		w += x ;
		x = 0 ;
	}
	else
	if( x + w > BaseImage->Width )
	{
		w = BaseImage->Width - x ;
	}

	if( y < 0 )
	{
		h += y ;
		y = 0 ;
	}
	else
	if( y + h > BaseImage->Height )
	{
		h = BaseImage->Height - y ;
	}

	Color    = BaseImage->ColorData.PixelByte == 1 ? 0 : NS_GetColor3( &BaseImage->ColorData, r, g, b, a ) ;
	p        = ( BYTE * )BaseImage->GraphData + x * BaseImage->ColorData.PixelByte + y * BaseImage->Pitch ;
	addpitch = BaseImage->Pitch - w * BaseImage->ColorData.PixelByte ;
	switch( BaseImage->ColorData.PixelByte )
	{
	case 1:
		for( i = 0; i < h; i ++, p += addpitch )
		{
			for( j = 0; j < w; j ++, p ++ )
			{
				*p = ( BYTE )r ;
			}
		}
		break;

	case 2:
		for( i = 0; i < h; i ++, p += addpitch )
		{
			for( j = 0; j < w; j ++, p += 2 )
			{
				*( ( WORD * )p ) = ( WORD )Color ;
			}
		}
		break;

	case 3:
		for( i = 0; i < h; i ++, p += addpitch )
		{
			for( j = 0; j < w; j ++, p += 3 )
			{
				( ( WORD * )p )[ 0 ] = ( ( WORD * )&Color )[ 0 ] ;
				( ( BYTE * )p )[ 2 ] = ( ( BYTE * )&Color )[ 2 ] ;
			}
		}
		break;

	case 4:
		for( i = 0; i < h; i ++, p += addpitch )
		{
			for( j = 0; j < w; j ++, p += 4 )
			{
				*( ( DWORD * )p ) = ( DWORD )Color ;
			}
		}
		break;
	}

	// �I��
	return 0;
}

// ��{�C���[�W�f�[�^�̎w��̗̈���O�N���A����
extern int NS_ClearRectBaseImage( BASEIMAGE *BaseImage, int x, int y, int w, int h )
{
	int i, FillByte, Pitch ;
	BYTE *Dest ;

	if( x >= BaseImage->Width  || w <= 0 ||
		y >= BaseImage->Height || h <= 0 )
	{
		return -1 ;
	}

	if( x < 0 )
	{
		w += x ;
		x = 0 ;
	}
	else
	if( x + w > BaseImage->Width )
	{
		w = BaseImage->Width - x ;
	}

	if( y < 0 )
	{
		h += y ;
		y = 0 ;
	}
	else
	if( y + h > BaseImage->Height )
	{
		h = BaseImage->Height - y ;
	}

	Pitch = BaseImage->Pitch ;
	FillByte = w * BaseImage->ColorData.PixelByte ;
	Dest = ( BYTE * )BaseImage->GraphData + x * BaseImage->ColorData.PixelByte + y * Pitch ;
	for( i = h ; i > 0 ; i --, Dest += Pitch )
	{
		_MEMSET( Dest, 0, ( size_t )FillByte ) ;
	}

	// ����I��
	return 0 ;
}

// ��{�C���[�W�f�[�^�̃p���b�g���擾����
extern int NS_GetPaletteBaseImage( const BASEIMAGE *BaseImage, int PaletteNo, int *r, int *g, int *b, int *a )
{
	const COLORPALETTEDATA *Palette ;
	
	if( PaletteNo < 0 || PaletteNo > 255 ) return -1 ;

	Palette = &BaseImage->ColorData.Palette[ PaletteNo ] ;
	if( r ) *r = Palette->Red ;
	if( g ) *g = Palette->Green ;
	if( b ) *b = Palette->Blue ;
	if( a ) *a = Palette->Alpha ;

	// �I��
	return 0 ;
}

// ��{�C���[�W�f�[�^�̃p���b�g���Z�b�g����
extern int NS_SetPaletteBaseImage( BASEIMAGE *BaseImage, int PaletteNo, int  r, int  g, int  b, int  a )
{
	COLORPALETTEDATA *Palette ;
	
	if( PaletteNo < 0 || PaletteNo > 255 ) return -1 ;

	Palette = &BaseImage->ColorData.Palette[ PaletteNo ] ;
	Palette->Red   = ( unsigned char )r ;
	Palette->Green = ( unsigned char )g ;
	Palette->Blue  = ( unsigned char )b ;
	Palette->Alpha = ( unsigned char )a ;

	// �I��
	return 0 ;
}

// ��{�C���[�W�f�[�^�̎w��̍��W�̐F�R�[�h��ύX����(�p���b�g�摜�p)
extern int NS_SetPixelPalCodeBaseImage( BASEIMAGE *BaseImage, int x, int y, int palNo )
{
	unsigned int Color;
	BYTE *p;

	// �t�H�[�}�b�g���W���t�H�[�}�b�g�ł͂Ȃ����A�p���b�g�`���ł͂Ȃ�������G���[
	if( BaseImage->ColorData.Format != DX_BASEIMAGE_FORMAT_NORMAL ||
		BaseImage->ColorData.PixelByte != 1 )
		return -1 ;

	// �͂ݏo������
	if( ( DWORD )BaseImage->Width <= ( DWORD )x || ( DWORD )BaseImage->Height <= ( DWORD )y )
		return -1 ;

	Color = ( DWORD )palNo ;
	switch( BaseImage->ColorData.PixelByte )
	{
	case 1:
		*( ( BYTE * )BaseImage->GraphData + BaseImage->Pitch * y + x ) = ( BYTE )Color ;
		break;

	case 2:
		*((WORD *)((BYTE *)BaseImage->GraphData + BaseImage->Pitch * y + x * 2)) =  (WORD)Color;
		break;

	case 3:
		p = (BYTE *)BaseImage->GraphData + BaseImage->Pitch * y + x * 3;
		((WORD *)p)[0] = ((WORD *)&Color)[0];
		((BYTE *)p)[2] = ((BYTE *)&Color)[2];
		break;

	case 4:
		*((DWORD *)((BYTE *)BaseImage->GraphData + BaseImage->Pitch * y + x * 4)) = (DWORD)Color;
		break;
	}

	// �I��
	return 0;
}

// ��{�C���[�W�f�[�^�̎w��̍��W�̐F�R�[�h���擾����(�p���b�g�摜�p)
extern int NS_GetPixelPalCodeBaseImage( const BASEIMAGE *BaseImage, int x, int y )
{
	BYTE *p;

	// �t�H�[�}�b�g���W���t�H�[�}�b�g�ł͂Ȃ����A�p���b�g�`���ł͂Ȃ�������G���[
	if( BaseImage->ColorData.Format != DX_BASEIMAGE_FORMAT_NORMAL ||
		BaseImage->ColorData.PixelByte != 1 )
		return 0xffffffff ;

	// �͂ݏo������
	if( ( DWORD )BaseImage->Width <= ( DWORD )x || ( DWORD )BaseImage->Height <= ( DWORD )y )
		return 0xffffffff ;

	p = (BYTE *)BaseImage->GraphData + BaseImage->Pitch * y;
	switch( BaseImage->ColorData.PixelByte )
	{
	case 1:
		return *( p + x ) ;

	case 2:
		return *( ( WORD * )p + x ) ;

	case 3:
		p += x * 3;
		return *( ( WORD * )p ) | ( p[ 2 ] << 16 ) ;

	case 4:
		return ( int )( *( ( DWORD * )p + x ) ) ;
	}

	// �I��
	return 0xffffffff;
}

// ��{�C���[�W�f�[�^�̎w��̍��W�̐F��ύX����(�e�F�v�f�͂O�`�Q�T�T)
extern int NS_SetPixelBaseImage( BASEIMAGE *BaseImage, int x, int y, int  r, int  g, int  b, int  a )
{
	unsigned int Color;
	BYTE *p;

	// �t�H�[�}�b�g���W���t�H�[�}�b�g�ł͂Ȃ�������W���t�H�[�}�b�g�ɕϊ�����
	if( BaseImage->ColorData.Format != DX_BASEIMAGE_FORMAT_NORMAL )
	{
		if( NS_ConvertNormalFormatBaseImage( BaseImage ) < 0 )
			return -1 ;
	}

	// �͂ݏo������
	if( ( DWORD )BaseImage->Width <= ( DWORD )x || ( DWORD )BaseImage->Height <= ( DWORD )y )
		return -1 ;

	if( BaseImage->ColorData.FloatTypeFlag )
	{
		switch( BaseImage->ColorData.ChannelBitDepth )
		{
		case 16 :
			p = ( BYTE * )BaseImage->GraphData + BaseImage->Pitch * y + x * 8 ;
			if( BaseImage->ColorData.ChannelNum > 0 )
			{
				( ( WORD * )p )[ 0 ] = Float32ToFloat16( r / 255.0f ) ;
			}
			if( BaseImage->ColorData.ChannelNum > 1 )
			{
				( ( WORD * )p )[ 1 ] = Float32ToFloat16( g / 255.0f ) ;
			}
			if( BaseImage->ColorData.ChannelNum > 2 )
			{
				( ( WORD * )p )[ 2 ] = Float32ToFloat16( b / 255.0f ) ;
			}
			if( BaseImage->ColorData.ChannelNum > 3 )
			{
				( ( WORD * )p )[ 3 ] = Float32ToFloat16( a / 255.0f ) ;
			}
			break ;

		case 32 :
			p = ( BYTE * )BaseImage->GraphData + BaseImage->Pitch * y + x * 16 ;
			if( BaseImage->ColorData.ChannelNum > 0 )
			{
				( ( float * )p )[ 0 ] = r / 255.0f ;
			}
			if( BaseImage->ColorData.ChannelNum > 1 )
			{
				( ( float * )p )[ 1 ] = g / 255.0f ;
			}
			if( BaseImage->ColorData.ChannelNum > 2 )
			{
				( ( float * )p )[ 2 ] = b / 255.0f ;
			}
			if( BaseImage->ColorData.ChannelNum > 3 )
			{
				( ( float * )p )[ 3 ] = a / 255.0f ;
			}
			break ;

		default :
			return -1 ;
		}
	}
	else
	{
		Color = NS_GetColor3( &BaseImage->ColorData, r, g, b, a );
		switch( BaseImage->ColorData.PixelByte )
		{
		case 1:
			*( ( BYTE * )BaseImage->GraphData + BaseImage->Pitch * y + x ) = ( BYTE )Color ;
			break;

		case 2:
			*((WORD *)((BYTE *)BaseImage->GraphData + BaseImage->Pitch * y + x * 2)) =  (WORD)Color;
			break;

		case 3:
			p = (BYTE *)BaseImage->GraphData + BaseImage->Pitch * y + x * 3;
			((WORD *)p)[0] = ((WORD *)&Color)[0];
			((BYTE *)p)[2] = ((BYTE *)&Color)[2];
			break;

		case 4: *((DWORD *)((BYTE *)BaseImage->GraphData + BaseImage->Pitch * y + x * 4)) = (DWORD)Color; break;
		}
	}

	// �I��
	return 0;
}

// ��{�C���[�W�f�[�^�̎w��̍��W�̐F��ύX����(�e�F�v�f�͕��������_��)
extern int NS_SetPixelBaseImageF( BASEIMAGE *BaseImage, int x, int y, float  r, float  g, float  b, float  a )
{
	unsigned int Color ;
	BYTE *p ;

	// �t�H�[�}�b�g���W���t�H�[�}�b�g�ł͂Ȃ�������W���t�H�[�}�b�g�ɕϊ�����
	if( BaseImage->ColorData.Format != DX_BASEIMAGE_FORMAT_NORMAL )
	{
		if( NS_ConvertNormalFormatBaseImage( BaseImage ) < 0 )
			return -1 ;
	}

	// �͂ݏo������
	if( ( DWORD )BaseImage->Width <= ( DWORD )x || ( DWORD )BaseImage->Height <= ( DWORD )y )
		return -1 ;

	if( BaseImage->ColorData.FloatTypeFlag )
	{
		switch( BaseImage->ColorData.ChannelBitDepth )
		{
		case 16 :
			p = ( BYTE * )BaseImage->GraphData + BaseImage->Pitch * y + x * 8 ;
			if( BaseImage->ColorData.ChannelNum > 0 )
			{
				( ( WORD * )p )[ 0 ] = Float32ToFloat16( r ) ;
			}
			if( BaseImage->ColorData.ChannelNum > 1 )
			{
				( ( WORD * )p )[ 1 ] = Float32ToFloat16( g ) ;
			}
			if( BaseImage->ColorData.ChannelNum > 2 )
			{
				( ( WORD * )p )[ 2 ] = Float32ToFloat16( b ) ;
			}
			if( BaseImage->ColorData.ChannelNum > 3 )
			{
				( ( WORD * )p )[ 3 ] = Float32ToFloat16( a ) ;
			}
			break ;

		case 32 :
			p = ( BYTE * )BaseImage->GraphData + BaseImage->Pitch * y + x * 16 ;
			if( BaseImage->ColorData.ChannelNum > 0 )
			{
				( ( float * )p )[ 0 ] = r ;
			}
			if( BaseImage->ColorData.ChannelNum > 1 )
			{
				( ( float * )p )[ 1 ] = g ;
			}
			if( BaseImage->ColorData.ChannelNum > 2 )
			{
				( ( float * )p )[ 2 ] = b ;
			}
			if( BaseImage->ColorData.ChannelNum > 3 )
			{
				( ( float * )p )[ 3 ] = a ;
			}
			break ;

		default :
			return -1 ;
		}
	}
	else
	{
		Color = NS_GetColor3( &BaseImage->ColorData,
			r < 0.0f ? 0 : ( r > 1.0f ? 255 : _FTOL( r * 255.0f ) ),
			g < 0.0f ? 0 : ( g > 1.0f ? 255 : _FTOL( g * 255.0f ) ),
			b < 0.0f ? 0 : ( b > 1.0f ? 255 : _FTOL( b * 255.0f ) ),
			a < 0.0f ? 0 : ( a > 1.0f ? 255 : _FTOL( a * 255.0f ) )
		) ;
		switch( BaseImage->ColorData.PixelByte )
		{
		case 1:
			*( ( BYTE * )BaseImage->GraphData + BaseImage->Pitch * y + x ) = ( BYTE )Color ;
			break;

		case 2:
			*((WORD *)((BYTE *)BaseImage->GraphData + BaseImage->Pitch * y + x * 2)) =  (WORD)Color;
			break;

		case 3:
			p = (BYTE *)BaseImage->GraphData + BaseImage->Pitch * y + x * 3;
			((WORD *)p)[0] = ((WORD *)&Color)[0];
			((BYTE *)p)[2] = ((BYTE *)&Color)[2];
			break;

		case 4: *((DWORD *)((BYTE *)BaseImage->GraphData + BaseImage->Pitch * y + x * 4)) = (DWORD)Color; break;
		}
	}

	// �I��
	return 0;
}

// ��{�C���[�W�f�[�^�̎w��̍��W�̐F���擾����(�e�F�v�f�͂O�`�Q�T�T)
extern int NS_GetPixelBaseImage( const BASEIMAGE *BaseImage, int x, int y, int *r, int *g, int *b, int *a )
{
	unsigned int Color;
	const BYTE *p;
	const COLORPALETTEDATA *Pal ;

	// �t�H�[�}�b�g���W���t�H�[�}�b�g�ł͂Ȃ�������W���t�H�[�}�b�g�ɕϊ�����
	if( BaseImage->ColorData.Format != DX_BASEIMAGE_FORMAT_NORMAL )
	{
		if( NS_ConvertNormalFormatBaseImage( ( BASEIMAGE * )BaseImage ) < 0 )
			return -1 ;
	}

	// �͂ݏo������
	if( ( DWORD )BaseImage->Width <= ( DWORD )x || ( DWORD )BaseImage->Height <= ( DWORD )y )
		return -1 ;

	p = (BYTE *)BaseImage->GraphData + BaseImage->Pitch * y;
	if( BaseImage->ColorData.FloatTypeFlag )
	{
		switch( BaseImage->ColorData.ChannelBitDepth )
		{
		case 16 :
			p += x * 8 ;
			if( BaseImage->ColorData.ChannelNum > 0 )
			{
				if( r ) *r = _FTOL( Float16ToFloat32( ( ( WORD * )p )[ 0 ] ) * 255.0f ) ;
			}
			if( BaseImage->ColorData.ChannelNum > 1 )
			{
				if( g ) *g = _FTOL( Float16ToFloat32( ( ( WORD * )p )[ 1 ] ) * 255.0f ) ;
			}
			if( BaseImage->ColorData.ChannelNum > 2 )
			{
				if( b ) *b = _FTOL( Float16ToFloat32( ( ( WORD * )p )[ 2 ] ) * 255.0f ) ;
			}
			if( BaseImage->ColorData.ChannelNum > 3 )
			{
				if( a ) *a = _FTOL( Float16ToFloat32( ( ( WORD * )p )[ 3 ] ) * 255.0f ) ;
			}
			break ;

		case 32 :
			p += x * 16 ;
			if( BaseImage->ColorData.ChannelNum > 0 )
			{
				if( r ) *r = _FTOL( ( ( float * )p )[ 0 ] * 255.0f ) ;
			}
			if( BaseImage->ColorData.ChannelNum > 1 )
			{
				if( g ) *g = _FTOL( ( ( float * )p )[ 1 ] * 255.0f ) ;
			}
			if( BaseImage->ColorData.ChannelNum > 2 )
			{
				if( b ) *b = _FTOL( ( ( float * )p )[ 2 ] * 255.0f ) ;
			}
			if( BaseImage->ColorData.ChannelNum > 3 )
			{
				if( a ) *a = _FTOL( ( ( float * )p )[ 3 ] * 255.0f ) ;
			}
			break ;

		default :
			return -1 ;
		}
	}
	else
	{
		switch( BaseImage->ColorData.PixelByte )
		{
		case 1:
				p += x;
				Pal = &BaseImage->ColorData.Palette[*p] ;
				if( r ) *r = Pal->Red ;
				if( g ) *g = Pal->Green ;
				if( b ) *b = Pal->Blue ;
				if( a ) *a = Pal->Alpha ;

				// �p���b�g�J���[�͂����ŏI��
				return 0 ;

		case 2: Color = *((WORD *)(p + x * 2)); break;
		case 3:
				p += x * 3;
				Color = 0;
				((WORD *)&Color)[0] = ((WORD *)p)[0];
				((BYTE *)&Color)[2] = ((BYTE *)p)[2];
				break;
		case 4: Color = *((DWORD *)(p + x * 4)); break;

		default :
			return -1 ;
		}

		NS_GetColor5( &BaseImage->ColorData, Color, r, g, b, a );
	}

	// �I��
	return 0;
}

// ��{�C���[�W�f�[�^�̎w��̍��W�̐F���擾����(�e�F�v�f�͕��������_��)
extern int NS_GetPixelBaseImageF( const BASEIMAGE *BaseImage, int x, int y, float *r, float *g, float *b, float *a )
{
	unsigned int Color;
	const BYTE *p;
	const COLORPALETTEDATA *Pal ;
	int ir, ig, ib, ia ;

	// �t�H�[�}�b�g���W���t�H�[�}�b�g�ł͂Ȃ�������W���t�H�[�}�b�g�ɕϊ�����
	if( BaseImage->ColorData.Format != DX_BASEIMAGE_FORMAT_NORMAL )
	{
		if( NS_ConvertNormalFormatBaseImage( ( BASEIMAGE * )BaseImage ) < 0 )
			return -1 ;
	}

	// �͂ݏo������
	if( ( DWORD )BaseImage->Width <= ( DWORD )x || ( DWORD )BaseImage->Height <= ( DWORD )y )
		return -1 ;

	p = (BYTE *)BaseImage->GraphData + BaseImage->Pitch * y;
	if( BaseImage->ColorData.FloatTypeFlag )
	{
		switch( BaseImage->ColorData.ChannelBitDepth )
		{
		case 16 :
			p += x * 8 ;
			if( BaseImage->ColorData.ChannelNum > 0 )
			{
				if( r ) *r = Float16ToFloat32( ( ( WORD * )p )[ 0 ] ) ;
			}
			if( BaseImage->ColorData.ChannelNum > 1 )
			{
				if( g ) *g = Float16ToFloat32( ( ( WORD * )p )[ 1 ] ) ;
			}
			if( BaseImage->ColorData.ChannelNum > 2 )
			{
				if( b ) *b = Float16ToFloat32( ( ( WORD * )p )[ 2 ] ) ;
			}
			if( BaseImage->ColorData.ChannelNum > 3 )
			{
				if( a ) *a = Float16ToFloat32( ( ( WORD * )p )[ 3 ] ) ;
			}
			break ;

		case 32 :
			p += x * 16 ;
			if( BaseImage->ColorData.ChannelNum > 0 )
			{
				if( r ) *r = ( ( float * )p )[ 0 ] ;
			}
			if( BaseImage->ColorData.ChannelNum > 1 )
			{
				if( g ) *g = ( ( float * )p )[ 1 ] ;
			}
			if( BaseImage->ColorData.ChannelNum > 2 )
			{
				if( b ) *b = ( ( float * )p )[ 2 ] ;
			}
			if( BaseImage->ColorData.ChannelNum > 3 )
			{
				if( a ) *a = ( ( float * )p )[ 3 ] ;
			}
			break ;

		default :
			return -1 ;
		}
	}
	else
	{
		switch( BaseImage->ColorData.PixelByte )
		{
		case 1:
				p += x;
				Pal = &BaseImage->ColorData.Palette[*p] ;
				if( r ) *r = Pal->Red   / 255.0f ;
				if( g ) *g = Pal->Green / 255.0f ;
				if( b ) *b = Pal->Blue  / 255.0f ;
				if( a ) *a = Pal->Alpha / 255.0f ;

				// �p���b�g�J���[�͂����ŏI��
				return 0 ;

		case 2: Color = *((WORD *)(p + x * 2)); break;
		case 3:
				p += x * 3;
				Color = 0;
				((WORD *)&Color)[0] = ((WORD *)p)[0];
				((BYTE *)&Color)[2] = ((BYTE *)p)[2];
				break;
		case 4: Color = *((DWORD *)(p + x * 4)); break;

		default :
			return -1 ;
		}

		NS_GetColor5( &BaseImage->ColorData, Color, &ir, &ig, &ib, &ia );
		if( r ) *r = ir / 255.0f ;
		if( g ) *g = ig / 255.0f ;
		if( b ) *b = ib / 255.0f ;
		if( a ) *a = ia / 255.0f ;
	}

	// �I��
	return 0;
}

// ��{�C���[�W�f�[�^�̎w��̍��W�ɐ���`�悷��(�e�F�v�f�͂O�`�Q�T�T)
extern int NS_DrawLineBaseImage( BASEIMAGE *BaseImage, int x1, int y1, int x2, int y2, int r, int g, int b, int a )
{
	unsigned int Color;
	BYTE *p;
	int c, w, h, adx, ady, x, y ;

	// �t�H�[�}�b�g���W���t�H�[�}�b�g�ł͂Ȃ�������W���t�H�[�}�b�g�ɕϊ�����
	if( BaseImage->ColorData.Format != DX_BASEIMAGE_FORMAT_NORMAL )
	{
		if( NS_ConvertNormalFormatBaseImage( BaseImage ) < 0 )
			return -1 ;
	}

	if( x1 == x2 && y1 == y2 ) return 0 ;

	w = x2 - x1 ;
	h = y2 - y1 ;
	if( w < 0 )
	{
		adx = -1 ;
		w = -w ;
	}
	else
	{
		adx = 1 ;
	}

	if( h < 0 )
	{
		ady = -1 ;
		h = -h ;
	}
	else
	{
		ady = 1 ;
	}

	Color = NS_GetColor3( &BaseImage->ColorData, r, g, b, a );
	x = x1 ;
	y = y1 ;
	switch( BaseImage->ColorData.PixelByte )
	{
	case 1:
		if( w == 0 )
		{
			if( ( DWORD )BaseImage->Width > ( DWORD )x )
			{
				while( y2 != y )
				{
					if( ( DWORD )BaseImage->Height > ( DWORD )y )
						*( ( BYTE * )BaseImage->GraphData + BaseImage->Pitch * y + x ) = ( BYTE )Color ;
					y += ady ;
				}
			}
		}
		else
		if( h == 0 )
		{
			if( ( DWORD )BaseImage->Height > ( DWORD )y )
			{
				while( x2 != x )
				{
					if( ( DWORD )BaseImage->Width > ( DWORD )x )
						*( ( BYTE * )BaseImage->GraphData + BaseImage->Pitch * y + x ) = ( BYTE )Color ;
					x += adx ;
				}
			}
		}
		if( h > w )
		{
			c = 0 ;
			while( x2 != x || y2 != y )
			{
				if( ( DWORD )BaseImage->Width > ( DWORD )x && ( DWORD )BaseImage->Height > ( DWORD )y )
					*( ( BYTE * )BaseImage->GraphData + BaseImage->Pitch * y + x ) = ( BYTE )Color ;
				c += w ;
				if( c >= h )
				{
					c -= h ;
					x += adx ;
				}
				y += ady ;
			}
		}
		else
		{
			c = 0 ;
			while( x2 != x || y2 != y )
			{
				if( ( DWORD )BaseImage->Width > ( DWORD )x && ( DWORD )BaseImage->Height > ( DWORD )y )
					*( ( BYTE * )BaseImage->GraphData + BaseImage->Pitch * y + x ) = ( BYTE )Color ;
				c += h ;
				if( c >= w )
				{
					c -= w ;
					y += ady ;
				}
				x += adx ;
			}
		}
		break;

	case 2:
		if( w == 0 )
		{
			if( ( DWORD )BaseImage->Width > ( DWORD )x )
			{
				while( y2 != y )
				{
					if( ( DWORD )BaseImage->Height > ( DWORD )y )
						*((WORD *)((BYTE *)BaseImage->GraphData + BaseImage->Pitch * y + x * 2)) =  (WORD)Color;
					y += ady ;
				}
			}
		}
		else
		if( h == 0 )
		{
			if( ( DWORD )BaseImage->Height > ( DWORD )y )
			{
				while( x2 != x )
				{
					if( ( DWORD )BaseImage->Width > ( DWORD )x )
						*((WORD *)((BYTE *)BaseImage->GraphData + BaseImage->Pitch * y + x * 2)) =  (WORD)Color;
					x += adx ;
				}
			}
		}
		if( h > w )
		{
			c = 0 ;
			while( x2 != x || y2 != y )
			{
				if( ( DWORD )BaseImage->Width > ( DWORD )x && ( DWORD )BaseImage->Height > ( DWORD )y )
					*((WORD *)((BYTE *)BaseImage->GraphData + BaseImage->Pitch * y + x * 2)) =  (WORD)Color;
				c += w ;
				if( c >= h )
				{
					c -= h ;
					x += adx ;
				}
				y += ady ;
			}
		}
		else
		{
			c = 0 ;
			while( x2 != x || y2 != y )
			{
				if( ( DWORD )BaseImage->Width > ( DWORD )x && ( DWORD )BaseImage->Height > ( DWORD )y )
					*((WORD *)((BYTE *)BaseImage->GraphData + BaseImage->Pitch * y + x * 2)) =  (WORD)Color;
				c += h ;
				if( c >= w )
				{
					c -= w ;
					y += ady ;
				}
				x += adx ;
			}
		}
		break;

	case 3:
		if( w == 0 )
		{
			if( ( DWORD )BaseImage->Width > ( DWORD )x )
			{
				while( y2 != y )
				{
					if( ( DWORD )BaseImage->Height > ( DWORD )y )
					{
						p = (BYTE *)BaseImage->GraphData + BaseImage->Pitch * y + x * 3;
						((WORD *)p)[0] = ((WORD *)&Color)[0];
						((BYTE *)p)[2] = ((BYTE *)&Color)[2];
					}
					y += ady ;
				}
			}
		}
		else
		if( h == 0 )
		{
			if( ( DWORD )BaseImage->Height > ( DWORD )y )
			{
				while( x2 != x )
				{
					if( ( DWORD )BaseImage->Width > ( DWORD )x )
					{
						p = (BYTE *)BaseImage->GraphData + BaseImage->Pitch * y + x * 3;
						((WORD *)p)[0] = ((WORD *)&Color)[0];
						((BYTE *)p)[2] = ((BYTE *)&Color)[2];
					}
					x += adx ;
				}
			}
		}
		if( h > w )
		{
			c = 0 ;
			while( x2 != x || y2 != y )
			{
				if( ( DWORD )BaseImage->Width > ( DWORD )x && ( DWORD )BaseImage->Height > ( DWORD )y )
				{
					p = (BYTE *)BaseImage->GraphData + BaseImage->Pitch * y + x * 3;
					((WORD *)p)[0] = ((WORD *)&Color)[0];
					((BYTE *)p)[2] = ((BYTE *)&Color)[2];
				}
				c += w ;
				if( c >= h )
				{
					c -= h ;
					x += adx ;
				}
				y += ady ;
			}
		}
		else
		{
			c = 0 ;
			while( x2 != x || y2 != y )
			{
				if( ( DWORD )BaseImage->Width > ( DWORD )x && ( DWORD )BaseImage->Height > ( DWORD )y )
				{
					p = (BYTE *)BaseImage->GraphData + BaseImage->Pitch * y + x * 3;
					((WORD *)p)[0] = ((WORD *)&Color)[0];
					((BYTE *)p)[2] = ((BYTE *)&Color)[2];
				}
				c += h ;
				if( c >= w )
				{
					c -= w ;
					y += ady ;
				}
				x += adx ;
			}
		}
		break;

	case 4:
		if( w == 0 )
		{
			if( ( DWORD )BaseImage->Width > ( DWORD )x )
			{
				while( y2 != y )
				{
					if( ( DWORD )BaseImage->Height > ( DWORD )y )
						*((DWORD *)((BYTE *)BaseImage->GraphData + BaseImage->Pitch * y + x * 4)) = (DWORD)Color;
					y += ady ;
				}
			}
		}
		else
		if( h == 0 )
		{
			if( ( DWORD )BaseImage->Height > ( DWORD )y )
			{
				while( x2 != x )
				{
					if( ( DWORD )BaseImage->Width > ( DWORD )x )
						*((DWORD *)((BYTE *)BaseImage->GraphData + BaseImage->Pitch * y + x * 4)) = (DWORD)Color;
					x += adx ;
				}
			}
		}
		if( h > w )
		{
			c = 0 ;
			while( x2 != x || y2 != y )
			{
				if( ( DWORD )BaseImage->Width > ( DWORD )x && ( DWORD )BaseImage->Height > ( DWORD )y )
					*((DWORD *)((BYTE *)BaseImage->GraphData + BaseImage->Pitch * y + x * 4)) = (DWORD)Color;
				c += w ;
				if( c >= h )
				{
					c -= h ;
					x += adx ;
				}
				y += ady ;
			}
		}
		else
		{
			c = 0 ;
			while( x2 != x || y2 != y )
			{
				if( ( DWORD )BaseImage->Width > ( DWORD )x && ( DWORD )BaseImage->Height > ( DWORD )y )
					*((DWORD *)((BYTE *)BaseImage->GraphData + BaseImage->Pitch * y + x * 4)) = (DWORD)Color;
				c += h ;
				if( c >= w )
				{
					c -= w ;
					y += ady ;
				}
				x += adx ;
			}
		}
		break;
	}

	// �I��
	return 0;
}










#define DRAWCIRCLEMEMIMG_FILL_SHRED_ND( DRAW, DESTADDNUM )	\
	if( !( ( x2 < DrawArea.left ) | ( x1 > DrawArea.right  ) |\
		   ( y1 < DrawArea.top  ) | ( y1 > DrawArea.bottom ) | LineDrawBuf[ y1 ] ) )\
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
	x2 = Dy + x ; x1 = -Dy + x ; y1 = Dx + y ;\
	DRAWCIRCLEMEMIMG_FILL_SHRED_ND( DRAW, DESTADDNUM )\
\
	x2 = Dy + x ; x1 = -Dy + x ; y1 = -Dx + y;\
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

#define DRAWCIRCLEMEMIMG_DRAW_BYTE		*( ( BYTE  * )DrawBP ) = ( BYTE  )Color ;
#define DRAWCIRCLEMEMIMG_DRAW_WORD		*( ( WORD  * )DrawBP ) = ( WORD  )Color ;
#define DRAWCIRCLEMEMIMG_DRAW_3BYTE		*( ( WORD  * )DrawBP ) = ( WORD  )Color ; ( ( BYTE * )DrawBP )[ 2 ] = ( BYTE )( Color >> 16 ) ;
#define DRAWCIRCLEMEMIMG_DRAW_DWORD		*( ( DWORD * )DrawBP ) = ( DWORD )Color ;


// ��{�C���[�W�f�[�^�̎w��̍��W�ɉ~��`�悷��(�e�F�v�f�͂O�`�Q�T�T)
extern int NS_DrawCircleBaseImage( BASEIMAGE *BaseImage, int x, int y, int radius, int r, int g, int b, int a, int FillFlag )
{
	unsigned int Color ;
	BYTE *LineDrawBuf ;
	RECT DrawArea ;
	DWORD DestPitch ;
	int Dx, Dy, F ;
	int x1, x2, y1, C ;
	BYTE  *DrawBP ;
	BYTE  *DestBP ;

	// �`��̈�̃Z�b�g
	DrawArea.left   = 0 ;
	DrawArea.top    = 0 ;
	DrawArea.right  = BaseImage->Width  - 1 ;
	DrawArea.bottom = BaseImage->Height - 1 ;

	// �]����̃A�h���X���Z�b�g
	DestBP    = ( BYTE *)BaseImage->GraphData ;
	DestPitch = BaseImage->Pitch ;

	// �����l�Z�b�g
	Dx = radius ;
	Dy = 0 ;
	F = -2 * radius + 3 ;

	Color = NS_GetColor3( &BaseImage->ColorData, r, g, b, a ) ;

	// �h��Ԃ����ǂ����ŏ����𕪊�
	if( FillFlag )
	{
		LineDrawBuf = (BYTE *)DXCALLOC( ( size_t )( DrawArea.bottom + 1 ) ) ;
		if( LineDrawBuf == NULL )
			return -1 ;

		switch( BaseImage->ColorData.PixelByte )
		{
		case 1 :
			DRAWCIRCLEMEMIMG_FILL_ND( DRAWCIRCLEMEMIMG_DRAW_BYTE, 1 )
			break ;

		case 2 :
			DRAWCIRCLEMEMIMG_FILL_ND( DRAWCIRCLEMEMIMG_DRAW_WORD, 2 )
			break ;

		case 3 :
			DRAWCIRCLEMEMIMG_FILL_ND( DRAWCIRCLEMEMIMG_DRAW_WORD, 3 )
			break ;

		case 4 :
			DRAWCIRCLEMEMIMG_FILL_ND( DRAWCIRCLEMEMIMG_DRAW_DWORD, 4 )
			break ;
		}

		DXFREE( LineDrawBuf ) ;
	}
	else
	{
		switch( BaseImage->ColorData.PixelByte )
		{
		case 1 :
			DRAWCIRCLEMEMIMG_PSET_ND( DRAWCIRCLEMEMIMG_DRAW_BYTE, 1 )
			break ;

		case 2 :
			DRAWCIRCLEMEMIMG_PSET_ND( DRAWCIRCLEMEMIMG_DRAW_WORD, 2 )
			break ;

		case 3 :
			DRAWCIRCLEMEMIMG_PSET_ND( DRAWCIRCLEMEMIMG_DRAW_3BYTE, 3 )
			break ;

		case 4 :
			DRAWCIRCLEMEMIMG_PSET_ND( DRAWCIRCLEMEMIMG_DRAW_DWORD, 4 )
			break ;
		}
	}

	// �I��
	return 0 ;
}

// ��{�C���[�W�f�[�^��]������
extern int NS_BltBaseImage( int DestX, int DestY, BASEIMAGE *SrcBaseImage, BASEIMAGE *DestBaseImage )
{
	return NS_BltBaseImage( 0, 0, SrcBaseImage->Width, SrcBaseImage->Height, DestX, DestY, SrcBaseImage, DestBaseImage ) ;
}

// ��{�C���[�W�f�[�^���g��]������
extern int ScalingBltBaseImage(
				int SrcX1,  int SrcY1,  int SrcX2,  int SrcY2,  BASEIMAGE *SrcBaseImage,
				int DestX1, int DestY1, int DestX2, int DestY2, BASEIMAGE *DestBaseImage, int Bilinear )
{
	DWORD DestWidth, DestHeight ;
	DWORD SrcWidth, SrcHeight ;
	DWORD i, j ;
	DWORD SrcX, SrcY ;
	DWORD AddSrcX, AddSrcY ;
	DWORD NowX, NowY ;
	DWORD RateX, RateY, RateXR, RateYR ;
	DWORD Rate11, Rate12, Rate21, Rate22 ;
	DWORD r11, r12, r21, r22 ;
	DWORD g11, g12, g21, g22 ;
	DWORD b11, b12, b21, b22 ;
	DWORD a11, a12, a21, a22 ;
	DWORD dr, dg, db, da ;

	// �]�������W�Ɠ]������W�̌���
	if( SrcX2 <= SrcX1 || SrcY2 <= SrcY1 ||
		SrcX1 < 0 || SrcX1 >= SrcBaseImage->Width  ||
		SrcY1 < 0 || SrcY1 >= SrcBaseImage->Height ||
		SrcX2 <= 0 || SrcX2 > SrcBaseImage->Width  ||
		SrcY2 <= 0 || SrcY2 > SrcBaseImage->Height ||
		DestX2 <= DestX1 || DestY2 <= DestY1 ||
		DestX1 < 0 || DestX1 >= DestBaseImage->Width  ||
		DestY1 < 0 || DestY1 >= DestBaseImage->Height ||
		DestX2 <= 0 || DestX2 > DestBaseImage->Width  ||
		DestY2 <= 0 || DestY2 > DestBaseImage->Height )
		return -1 ;

	// �]���T�C�Y���Z�o
	SrcWidth   = ( DWORD )( SrcX2  - SrcX1  ) ;
	SrcHeight  = ( DWORD )( SrcY2  - SrcY1  ) ;
	DestWidth  = ( DWORD )( DestX2 - DestX1 ) ;
	DestHeight = ( DWORD )( DestY2 - DestY1 ) ;

	// �]�����Ɠ]����̃T�C�Y���������ꍇ�͒ʏ�̓]�����s��
	if( SrcWidth == DestWidth && SrcHeight == DestHeight )
	{
		return NS_BltBaseImage( SrcX1, SrcY1, ( int )SrcWidth, ( int )SrcHeight, DestX1, DestY1, SrcBaseImage, DestBaseImage ) ;
	}

	// �P�h�b�g�ӂ�̕ω��l�̊���o��
	AddSrcX = ( SrcWidth  << 16 ) / DestWidth  ;
	AddSrcY = ( SrcHeight << 16 ) / DestHeight ;

	// �o�C���j�A��Ԃ��ǂ����ŏ����𕪊�
	if( Bilinear == TRUE )
	{
		// �o�C���j�A��Ԃ̏ꍇ
		SrcY = ( DWORD )( SrcY1 << 16 ) ;
		for( i = 0 ; i < DestHeight ; i ++, SrcY += AddSrcY )
		{
			SrcX = ( DWORD )( SrcX1 << 16 ) ;
			for( j = 0 ; j < DestWidth ; j ++, SrcX += AddSrcX )
			{
				NowX = SrcX >> 16 ;
				NowY = SrcY >> 16 ;
				NS_GetPixelBaseImage( SrcBaseImage, ( int )( NowX     ), ( int )( NowY     ), ( int * )&r11, ( int * )&g11, ( int * )&b11, ( int * )&a11 ) ; 
				NS_GetPixelBaseImage( SrcBaseImage, ( int )( NowX + 1 ), ( int )( NowY     ), ( int * )&r12, ( int * )&g12, ( int * )&b12, ( int * )&a12 ) ; 
				NS_GetPixelBaseImage( SrcBaseImage, ( int )( NowX     ), ( int )( NowY + 1 ), ( int * )&r21, ( int * )&g21, ( int * )&b21, ( int * )&a21 ) ; 
				NS_GetPixelBaseImage( SrcBaseImage, ( int )( NowX + 1 ), ( int )( NowY + 1 ), ( int * )&r22, ( int * )&g22, ( int * )&b22, ( int * )&a22 ) ; 

				RateX = SrcX & ( ( 1 << 16 ) - 1 ) ;
				RateY = SrcY & ( ( 1 << 16 ) - 1 ) ;
				RateXR = ( ( 1 << 16 ) - 1 ) - RateX ;
				RateYR = ( ( 1 << 16 ) - 1 ) - RateY ;
				Rate11 = ( RateXR * RateYR ) >> 16 ;
				Rate12 = ( RateX  * RateYR ) >> 16 ;
				Rate21 = ( RateXR * RateY  ) >> 16 ;
				Rate22 = ( RateX  * RateY  ) >> 16 ;

				dr = ( r11 * Rate11 + r12 * Rate12 + r21 * Rate21 + r22 * Rate22 ) >> 16 ; 
				dg = ( g11 * Rate11 + g12 * Rate12 + g21 * Rate21 + g22 * Rate22 ) >> 16 ; 
				db = ( b11 * Rate11 + b12 * Rate12 + b21 * Rate21 + b22 * Rate22 ) >> 16 ; 
				da = ( a11 * Rate11 + a12 * Rate12 + a21 * Rate21 + a22 * Rate22 ) >> 16 ; 

				NS_SetPixelBaseImage( DestBaseImage, ( int )( DestX1 + j ), ( int )( DestY1 + i ), ( int )dr, ( int )dg, ( int )db, ( int )da ) ;
			}
		}
	}
	else
	{
		// �o�C���j�A��Ԃ���Ȃ��ꍇ
		SrcY = ( DWORD )( SrcY1 << 16 ) ;
		for( i = 0 ; i < DestHeight ; i ++, SrcY += AddSrcY )
		{
			SrcX = ( DWORD )( SrcX1 << 16 ) ;
			for( j = 0 ; j < DestWidth ; j ++, SrcX += AddSrcX )
			{
				NowX = SrcX >> 16 ;
				NowY = SrcY >> 16 ;
				NS_GetPixelBaseImage( SrcBaseImage,  ( int )NowX,           ( int )NowY,            ( int * )&dr, ( int * )&dg, ( int * )&db, ( int * )&da ) ; 
				NS_SetPixelBaseImage( DestBaseImage, ( int )( DestX1 + j ), ( int )( DestY1 + i ),  ( int )dr,    ( int )dg,    ( int )db,    ( int )da    ) ;
			}
		}
	}

	// ����I��
	return 0 ;
}

// ��{�C���[�W�f�[�^��]������
extern int NS_BltBaseImage( int SrcX, int SrcY, int SrcSizeX, int SrcSizeY, int DestX, int DestY, BASEIMAGE *SrcBaseImage, BASEIMAGE *DestBaseImage )
{
	POINT DestPoint;
	RECT SrcRect;

	// �t�H�[�}�b�g���W���t�H�[�}�b�g�ł͂Ȃ�������W���t�H�[�}�b�g�ɕϊ�����
	if( SrcBaseImage->ColorData.Format != DX_BASEIMAGE_FORMAT_NORMAL )
	{
		if( NS_ConvertNormalFormatBaseImage( SrcBaseImage ) < 0 )
			return -1 ;
	}

	// �t�H�[�}�b�g���W���t�H�[�}�b�g�ł͂Ȃ�������W���t�H�[�}�b�g�ɕϊ�����
	if( DestBaseImage->ColorData.Format != DX_BASEIMAGE_FORMAT_NORMAL )
	{
		if( NS_ConvertNormalFormatBaseImage( DestBaseImage ) < 0 )
			return -1 ;
	}

	// �T�C�Y�␳
	if( SrcX  < 0 ){ SrcSizeX += SrcX  ; DestX -= SrcX;  SrcX  = 0 ; }
	if( SrcY  < 0 ){ SrcSizeY += SrcY  ; DestY -= SrcY;  SrcY  = 0 ; }
	if( DestX < 0 ){ SrcSizeX += DestX ; SrcX  -= DestX; DestX = 0 ; }
	if( DestY < 0 ){ SrcSizeY += DestY ; SrcY  -= DestY; DestY = 0 ; }
	if( DestBaseImage->Width  < DestX + SrcSizeX ) SrcSizeX = DestBaseImage->Width  - DestX;
	if( DestBaseImage->Height < DestY + SrcSizeY ) SrcSizeY = DestBaseImage->Height - DestY;
	if( SrcBaseImage->Width   < SrcX  + SrcSizeX ) SrcSizeX = SrcBaseImage->Width   - SrcX;
	if( SrcBaseImage->Height  < SrcY  + SrcSizeY ) SrcSizeY = SrcBaseImage->Height  - SrcY;
	if( SrcSizeX <= 0 || SrcSizeY <= 0 ) return 0;

	// �]��
	DestPoint.x    = DestX;
	DestPoint.y    = DestY;
	SrcRect.left   = SrcX;
	SrcRect.top    = SrcY;
	SrcRect.right  = SrcX + SrcSizeX;
	SrcRect.bottom = SrcY + SrcSizeY;
	NS_GraphColorMatchBltVer2(
		DestBaseImage->GraphData, DestBaseImage->Pitch, &DestBaseImage->ColorData, 
		 SrcBaseImage->GraphData,  SrcBaseImage->Pitch,  &SrcBaseImage->ColorData, 
		                    NULL,                    0,                      NULL,
		DestPoint, &SrcRect, FALSE,
		FALSE, 0,
		DX_SHAVEDMODE_NONE, FALSE, FALSE, FALSE, TRUE ) ;

	// �I��
	return 0;
}

// ��{�C���[�W�f�[�^�𓧉ߐF�����t���œ]������
extern	int		NS_BltBaseImageWithTransColor( int SrcX, int SrcY, int SrcSizeX, int SrcSizeY, int DestX, int DestY, BASEIMAGE *SrcBaseImage, BASEIMAGE *DestBaseImage, int Tr, int Tg, int Tb, int Ta )
{
	POINT DestPoint;
	RECT SrcRect;

	// �t�H�[�}�b�g���W���t�H�[�}�b�g�ł͂Ȃ�������W���t�H�[�}�b�g�ɕϊ�����
	if( SrcBaseImage->ColorData.Format != DX_BASEIMAGE_FORMAT_NORMAL )
	{
		if( NS_ConvertNormalFormatBaseImage( SrcBaseImage ) < 0 )
			return -1 ;
	}

	// �t�H�[�}�b�g���W���t�H�[�}�b�g�ł͂Ȃ�������W���t�H�[�}�b�g�ɕϊ�����
	if( DestBaseImage->ColorData.Format != DX_BASEIMAGE_FORMAT_NORMAL )
	{
		if( NS_ConvertNormalFormatBaseImage( DestBaseImage ) < 0 )
			return -1 ;
	}

	// �T�C�Y�␳
	if( SrcX  < 0 ){ SrcSizeX += SrcX  ; DestX -= SrcX;  SrcX  = 0 ; }
	if( SrcY  < 0 ){ SrcSizeY += SrcY  ; DestY -= SrcY;  SrcY  = 0 ; }
	if( DestX < 0 ){ SrcSizeX += DestX ; SrcX  -= DestX; DestX = 0 ; }
	if( DestY < 0 ){ SrcSizeY += DestY ; SrcY  -= DestY; DestY = 0 ; }
	if( DestBaseImage->Width  < DestX + SrcSizeX ) SrcSizeX = DestBaseImage->Width  - DestX;
	if( DestBaseImage->Height < DestY + SrcSizeY ) SrcSizeY = DestBaseImage->Height - DestY;
	if( SrcBaseImage->Width   < SrcX  + SrcSizeX ) SrcSizeX = SrcBaseImage->Width   - SrcX;
	if( SrcBaseImage->Height  < SrcY  + SrcSizeY ) SrcSizeY = SrcBaseImage->Height  - SrcY;
	if( SrcSizeX <= 0 || SrcSizeY <= 0 ) return 0;

	// ���ߐF�l�␳
	if( Tr < 0 ) Tr = 0 ; else if( Tr > 255 ) Tr = 255 ;
	if( Tg < 0 ) Tg = 0 ; else if( Tg > 255 ) Tg = 255 ;
	if( Tb < 0 ) Tb = 0 ; else if( Tb > 255 ) Tb = 255 ;
	if( Ta < 0 ) Ta = 0 ; else if( Ta > 255 ) Ta = 255 ;

	// �]��
	DestPoint.x    = DestX;
	DestPoint.y    = DestY;
	SrcRect.left   = SrcX;
	SrcRect.top    = SrcY;
	SrcRect.right  = SrcX + SrcSizeX;
	SrcRect.bottom = SrcY + SrcSizeY;
	NS_GraphColorMatchBltVer2(
		DestBaseImage->GraphData, DestBaseImage->Pitch, &DestBaseImage->ColorData, 
		 SrcBaseImage->GraphData,  SrcBaseImage->Pitch,  &SrcBaseImage->ColorData, 
		                    NULL,                    0,                      NULL,
		DestPoint, &SrcRect, FALSE,
		FALSE, ( ( DWORD )Ta << 24 ) | ( ( DWORD )Tr << 16 ) | ( ( DWORD )Tg << 8 ) | ( ( DWORD )Tb ),
		DX_SHAVEDMODE_NONE, FALSE, FALSE, TRUE, TRUE ) ;

	// �I��
	return 0;
}

// ��{�C���[�W�f�[�^���A���t�@�l�̃u�����h���l��������œ]������( �o�͐悪 ARGB8 �`���ȊO�̏ꍇ�̓G���[�ɂȂ�܂� )
extern int NS_BltBaseImageWithAlphaBlend( int SrcX, int SrcY, int SrcSizeX, int SrcSizeY, int DestX, int DestY, BASEIMAGE *SrcBaseImage, BASEIMAGE *DestBaseImage, int Opacity )
{
	DWORD Width, Height ;
	DWORD i, j, OpSrcAlpha, SrcAlpha, ResultAlpha, SrcAddPitch, DestAddPitch ;
	BYTE *Dest, *Src ;
	BASEIMAGE TempBaseImage, *UseSrcBaseImage ;

	// �t�H�[�}�b�g���W���t�H�[�}�b�g�ł͂Ȃ�������W���t�H�[�}�b�g�ɕϊ�����
	if( SrcBaseImage->ColorData.Format != DX_BASEIMAGE_FORMAT_NORMAL )
	{
		if( NS_ConvertNormalFormatBaseImage( SrcBaseImage ) < 0 )
			return -1 ;
	}

	// �t�H�[�}�b�g���W���t�H�[�}�b�g�ł͂Ȃ�������W���t�H�[�}�b�g�ɕϊ�����
	if( DestBaseImage->ColorData.Format != DX_BASEIMAGE_FORMAT_NORMAL )
	{
		if( NS_ConvertNormalFormatBaseImage( DestBaseImage ) < 0 )
			return -1 ;
	}

	// �o�͐悪 ARGB8 �ȊO�̏ꍇ�̓G���[
	if( DestBaseImage->ColorData.AlphaMask != 0xff000000 ||
		DestBaseImage->ColorData.RedMask   != 0x00ff0000 ||
		DestBaseImage->ColorData.GreenMask != 0x0000ff00 ||
		DestBaseImage->ColorData.BlueMask  != 0x000000ff )
	{
		return -1 ;
	}

	// �����x���ő�̂Ƃ��͉��������ɏI��
	if( Opacity == 0 )
		return 0 ;

	// �]�������R�Q�r�b�g�t�H�[�}�b�g�ȊO�̏ꍇ�͂R�Q�r�b�g�t�H�[�}�b�g�ɕύX����
	if( SrcBaseImage->ColorData.PixelByte != 4 )
	{
		if( SrcBaseImage->ColorData.AlphaWidth != 0 )
		{
			NS_CreateARGB8ColorBaseImage( SrcBaseImage->Width, SrcBaseImage->Height, &TempBaseImage ) ;
		}
		else
		{
			NS_CreateXRGB8ColorBaseImage( SrcBaseImage->Width, SrcBaseImage->Height, &TempBaseImage ) ;
		}
		NS_BltBaseImage( 0, 0, SrcBaseImage, &TempBaseImage ) ; 
		UseSrcBaseImage = &TempBaseImage ;
	}
	else
	{
		UseSrcBaseImage = SrcBaseImage ;
	}

	// �T�C�Y�␳
	if( SrcX  < 0 ){ SrcSizeX += SrcX  ; DestX -= SrcX;  SrcX  = 0 ; }
	if( SrcY  < 0 ){ SrcSizeY += SrcY  ; DestY -= SrcY;  SrcY  = 0 ; }
	if( DestX < 0 ){ SrcSizeX += DestX ; SrcX  -= DestX; DestX = 0 ; }
	if( DestY < 0 ){ SrcSizeY += DestY ; SrcY  -= DestY; DestY = 0 ; }
	if( DestBaseImage->Width  < DestX + SrcSizeX ) SrcSizeX = DestBaseImage->Width  - DestX;
	if( DestBaseImage->Height < DestY + SrcSizeY ) SrcSizeY = DestBaseImage->Height - DestY;
	if( SrcBaseImage->Width   < SrcX  + SrcSizeX ) SrcSizeX = SrcBaseImage->Width   - SrcX;
	if( SrcBaseImage->Height  < SrcY  + SrcSizeY ) SrcSizeY = SrcBaseImage->Height  - SrcY;
	if( SrcSizeX <= 0 || SrcSizeY <= 0 ) return 0;

	Width  = ( DWORD )SrcSizeX ;
	Height = ( DWORD )SrcSizeY ;

	SrcAddPitch  = UseSrcBaseImage->Pitch  - Width * UseSrcBaseImage->ColorData.PixelByte ;
	DestAddPitch = DestBaseImage->Pitch - Width * DestBaseImage->ColorData.PixelByte ;
	Src  = ( BYTE * )UseSrcBaseImage->GraphData + SrcX * UseSrcBaseImage->ColorData.PixelByte + SrcY * UseSrcBaseImage->Pitch ;
	Dest = ( BYTE * )DestBaseImage->GraphData + DestX * DestBaseImage->ColorData.PixelByte + DestY * DestBaseImage->Pitch ;

	// �]�����̃t�H�[�}�b�g�ɂ���ď����𕪊�
	if( SrcBaseImage->ColorData.AlphaMask == 0xff000000 &&
		SrcBaseImage->ColorData.RedMask   == 0x00ff0000 &&
		SrcBaseImage->ColorData.GreenMask == 0x0000ff00 &&
		SrcBaseImage->ColorData.BlueMask  == 0x000000ff )
	{
		if( Opacity == 255 )
		{
			for( i = Height ; i ; i --, Src += SrcAddPitch, Dest += DestAddPitch )
			{
				for( j = Width ; j ; j --, Src += 4, Dest += 4 )
				{
					if( Src[ 3 ] == 0 ) continue ;
					if( Src[ 3 ] == 255 )
					{
						*( ( DWORD * )Dest ) = *( ( DWORD * )Src ) ;
					}
					else
					{
						ResultAlpha = ( DWORD )( ( 256 - Dest[ 3 ] ) * Src[ 3 ] / 256 + Dest[ 3 ] ) ;
						SrcAlpha = Src[ 3 ] * 256 / ResultAlpha ;
						Dest[ 0 ] = ( BYTE )( SrcAlpha * ( Src[ 0 ] - Dest[ 0 ] ) / 256 + Dest[ 0 ] ) ;
						Dest[ 1 ] = ( BYTE )( SrcAlpha * ( Src[ 1 ] - Dest[ 1 ] ) / 256 + Dest[ 1 ] ) ;
						Dest[ 2 ] = ( BYTE )( SrcAlpha * ( Src[ 2 ] - Dest[ 2 ] ) / 256 + Dest[ 2 ] ) ;
						Dest[ 3 ] = ( BYTE )( ResultAlpha ) ;
					}
				}
			}
		}
		else
		{
			for( i = Height ; i ; i --, Src += SrcAddPitch, Dest += DestAddPitch )
			{
				for( j = Width ; j ; j --, Src += 4, Dest += 4 )
				{
					OpSrcAlpha = ( DWORD )( Src[ 3 ] * Opacity / 256 ) ;
					if( OpSrcAlpha == 0 ) continue ;
					ResultAlpha = ( 256 - Dest[ 3 ] ) * OpSrcAlpha / 256 + Dest[ 3 ] ;
					SrcAlpha = OpSrcAlpha * 256 / ResultAlpha ;
					Dest[ 0 ] = ( BYTE )( SrcAlpha * ( Src[ 0 ] - Dest[ 0 ] ) / 256 + Dest[ 0 ] ) ;
					Dest[ 1 ] = ( BYTE )( SrcAlpha * ( Src[ 1 ] - Dest[ 1 ] ) / 256 + Dest[ 1 ] ) ;
					Dest[ 2 ] = ( BYTE )( SrcAlpha * ( Src[ 2 ] - Dest[ 2 ] ) / 256 + Dest[ 2 ] ) ;
					Dest[ 3 ] = ( BYTE )( ResultAlpha ) ;
				}
			}
		}
	}
	else
	if( SrcBaseImage->ColorData.AlphaMask == 0x00000000 &&
		SrcBaseImage->ColorData.RedMask   == 0x00ff0000 &&
		SrcBaseImage->ColorData.GreenMask == 0x0000ff00 &&
		SrcBaseImage->ColorData.BlueMask  == 0x000000ff )
	{
		if( Opacity == 255 )
		{
			for( i = Height ; i ; i --, Src += SrcAddPitch, Dest += DestAddPitch )
			{
				for( j = Width ; j ; j --, Src += 4, Dest += 4 )
				{
					Dest[ 0 ] = Src[ 0 ] ;
					Dest[ 1 ] = Src[ 1 ] ;
					Dest[ 2 ] = Src[ 2 ] ;
					Dest[ 3 ] = 255 ;
				}
			}
		}
		else
		{
			for( i = Height ; i ; i --, Src += SrcAddPitch, Dest += DestAddPitch )
			{
				for( j = Width ; j ; j --, Src += 4, Dest += 4 )
				{
					ResultAlpha = ( DWORD )( ( 256 - Dest[ 3 ] ) * Opacity / 256 + Dest[ 3 ] ) ;
					SrcAlpha = Opacity * 256 / ResultAlpha ;
					Dest[ 0 ] = ( BYTE )( SrcAlpha * ( Src[ 0 ] - Dest[ 0 ] ) / 256 + Dest[ 0 ] ) ;
					Dest[ 1 ] = ( BYTE )( SrcAlpha * ( Src[ 1 ] - Dest[ 1 ] ) / 256 + Dest[ 1 ] ) ;
					Dest[ 2 ] = ( BYTE )( SrcAlpha * ( Src[ 2 ] - Dest[ 2 ] ) / 256 + Dest[ 2 ] ) ;
					Dest[ 3 ] = ( BYTE )( ResultAlpha ) ;
				}
			}
		}
	}

	if( UseSrcBaseImage == &TempBaseImage )
	{
		NS_ReleaseBaseImage( &TempBaseImage ) ;
	}

	// �I��
	return 0 ;
}


// ��{�C���[�W�f�[�^�̍��E�𔽓]����
extern int NS_ReverseBaseImageH( BASEIMAGE *BaseImage )
{
	BYTE TempB ;
	WORD TempW ;
	DWORD TempD ;
	DWORD i, j, loopnum ;
	DWORD w ,h ;
	BYTE *Src, *Dest ;

	// ���k�t�H�[�}�b�g�͔��]�ł��Ȃ�
	if( BaseImage->ColorData.Format != DX_BASEIMAGE_FORMAT_NORMAL )
		return -1 ;

	// �����]�����[�v�̐����Z�o
	loopnum = ( DWORD )( BaseImage->Width / 2 ) ;

	// �������[�J���ϐ��ɑ��
	w = ( DWORD )BaseImage->Width ;
	h = ( DWORD )BaseImage->Height ;

	// �o�C�g���ɂ���ď����𕪊�
	switch( BaseImage->ColorData.PixelByte )
	{
	case 1 :
		for( i = 0 ; i < h ; i ++ )
		{
			Src  = ( BYTE * )BaseImage->GraphData + i * BaseImage->Pitch ;
			Dest = Src + ( w - 1 ) * 1 ;
			for( j = 0 ; j < loopnum ; j ++, Src += 1, Dest -= 1 )
			{
				TempB = *( ( BYTE * )Src ) ;
				*( ( BYTE * )Src  ) = *( ( BYTE * )Dest ) ;
				*( ( BYTE * )Dest ) = TempB ;
			}
		}
		break ;

	case 2 :
		for( i = 0 ; i < h ; i ++ )
		{
			Src  = ( BYTE * )BaseImage->GraphData + i * BaseImage->Pitch ;
			Dest = Src + ( w - 1 ) * 2 ;
			for( j = 0 ; j < loopnum ; j ++, Src += 2, Dest -= 2 )
			{
				TempW = *( ( WORD * )Src ) ;
				*( ( WORD * )Src  ) = *( ( WORD * )Dest ) ;
				*( ( WORD * )Dest ) = TempW ;
			}
		}
		break ;

	case 3 :
		for( i = 0 ; i < h ; i ++ )
		{
			Src  = ( BYTE * )BaseImage->GraphData + i * BaseImage->Pitch ;
			Dest = Src + ( w - 1 ) * 3 ;
			for( j = 0 ; j < loopnum ; j ++, Src += 3, Dest -= 3 )
			{
				TempW = *( ( WORD * )Src ) ;
				TempB = Src[ 2 ] ;
				*( ( WORD * )Src  ) = *( ( WORD * )Dest ) ;
				Src[ 2 ] = Dest[ 2 ] ;
				*( ( WORD * )Dest ) = TempW ;
				Dest[ 2 ] = TempB ;
			}
		}
		break ;

	case 4 :
		for( i = 0 ; i < h ; i ++ )
		{
			Src  = ( BYTE * )BaseImage->GraphData + i * BaseImage->Pitch ;
			Dest = Src + ( w - 1 ) * 4 ;
			for( j = 0 ; j < loopnum ; j ++, Src += 4, Dest -= 4 )
			{
				TempD = *( ( DWORD * )Src ) ;
				*( ( DWORD * )Src  ) = *( ( DWORD * )Dest ) ;
				*( ( DWORD * )Dest ) = TempD ;
			}
		}
		break ;
	}

	// �I��
	return 0 ;
}

// ��{�C���[�W�f�[�^�̏㉺�𔽓]����
extern int NS_ReverseBaseImageV( BASEIMAGE *BaseImage )
{
	BYTE TempB ;
	WORD TempW ;
	DWORD TempD ;
	DWORD i, j, loopnum ;
	DWORD w ,h ;
	BYTE *Src, *Dest ;

	// ���k�t�H�[�}�b�g�͔��]�ł��Ȃ�
	if( BaseImage->ColorData.Format != DX_BASEIMAGE_FORMAT_NORMAL )
		return -1 ;

	// �����]�����[�v�̐����Z�o
	loopnum = ( DWORD )( BaseImage->Height / 2 ) ;

	// �������[�J���ϐ��ɑ��
	w = ( DWORD )BaseImage->Width ;
	h = ( DWORD )BaseImage->Height ;

	// �o�C�g���ɂ���ď����𕪊�
	switch( BaseImage->ColorData.PixelByte )
	{
	case 1 :
		for( i = 0 ; i < loopnum ; i ++ )
		{
			Src  = ( BYTE * )BaseImage->GraphData + i * BaseImage->Pitch ;
			Dest = ( BYTE * )BaseImage->GraphData + ( h - i - 1 ) * BaseImage->Pitch ;
			for( j = 0 ; j < w ; j ++, Src += 1, Dest += 1 )
			{
				TempB = *( ( BYTE * )Src ) ;
				*( ( BYTE * )Src  ) = *( ( BYTE * )Dest ) ;
				*( ( BYTE * )Dest ) = TempB ;
			}
		}
		break ;

	case 2 :
		for( i = 0 ; i < loopnum ; i ++ )
		{
			Src  = ( BYTE * )BaseImage->GraphData + i * BaseImage->Pitch ;
			Dest = ( BYTE * )BaseImage->GraphData + ( h - i - 1 ) * BaseImage->Pitch ;
			for( j = 0 ; j < w ; j ++, Src += 2, Dest += 2 )
			{
				TempW = *( ( WORD * )Src ) ;
				*( ( WORD * )Src  ) = *( ( WORD * )Dest ) ;
				*( ( WORD * )Dest ) = TempW ;
			}
		}
		break ;

	case 3 :
		for( i = 0 ; i < loopnum ; i ++ )
		{
			Src  = ( BYTE * )BaseImage->GraphData + i * BaseImage->Pitch ;
			Dest = ( BYTE * )BaseImage->GraphData + ( h - i - 1 ) * BaseImage->Pitch ;
			for( j = 0 ; j < w ; j ++, Src += 3, Dest += 3 )
			{
				TempW = *( ( WORD * )Src ) ;
				TempB = Src[ 2 ] ;
				*( ( WORD * )Src  ) = *( ( WORD * )Dest ) ;
				Src[ 2 ] = Dest[ 2 ] ;
				*( ( WORD * )Dest ) = TempW ;
				Dest[ 2 ] = TempB ;
			}
		}
		break ;

	case 4 :
		for( i = 0 ; i < loopnum ; i ++ )
		{
			Src  = ( BYTE * )BaseImage->GraphData + i * BaseImage->Pitch ;
			Dest = ( BYTE * )BaseImage->GraphData + ( h - i - 1 ) * BaseImage->Pitch ;
			for( j = 0 ; j < w ; j ++, Src += 4, Dest += 4 )
			{
				TempD = *( ( DWORD * )Src ) ;
				*( ( DWORD * )Src  ) = *( ( DWORD * )Dest ) ;
				*( ( DWORD * )Dest ) = TempD ;
			}
		}
		break ;
	}

	// �I��
	return 0 ;
}

// ��{�C���[�W�f�[�^�̏㉺���E�𔽓]����
extern int NS_ReverseBaseImage( BASEIMAGE *BaseImage )
{
	BYTE TempB ;
	WORD TempW ;
	DWORD TempD ;
	DWORD i, j, loopnum ;
	DWORD w ,h ;
	BYTE *Src, *Dest ;

	// ���k�t�H�[�}�b�g�͔��]�ł��Ȃ�
	if( BaseImage->ColorData.Format != DX_BASEIMAGE_FORMAT_NORMAL )
		return -1 ;

	// �����]�����[�v�̐����Z�o
	loopnum = ( DWORD )( BaseImage->Height / 2 ) ;

	// �������[�J���ϐ��ɑ��
	w = ( DWORD )BaseImage->Width ;
	h = ( DWORD )BaseImage->Height ;

	// �o�C�g���ɂ���ď����𕪊�
	switch( BaseImage->ColorData.PixelByte )
	{
	case 1 :
		for( i = 0 ; i < loopnum ; i ++ )
		{
			Src  = ( BYTE * )BaseImage->GraphData + i * BaseImage->Pitch ;
			Dest = ( BYTE * )BaseImage->GraphData + ( w - 1 ) * 1 + ( h - i - 1 ) * BaseImage->Pitch ;
			for( j = 0 ; j < w ; j ++, Src += 1, Dest -= 1 )
			{
				TempB = *( ( BYTE * )Src ) ;
				*( ( BYTE * )Src  ) = *( ( BYTE * )Dest ) ;
				*( ( BYTE * )Dest ) = TempB ;
			}
		}
		break ;

	case 2 :
		for( i = 0 ; i < loopnum ; i ++ )
		{
			Src  = ( BYTE * )BaseImage->GraphData + i * BaseImage->Pitch ;
			Dest = ( BYTE * )BaseImage->GraphData + ( w - 1 ) * 2 + ( h - i - 1 ) * BaseImage->Pitch ;
			for( j = 0 ; j < w ; j ++, Src += 2, Dest -= 2 )
			{
				TempW = *( ( WORD * )Src ) ;
				*( ( WORD * )Src  ) = *( ( WORD * )Dest ) ;
				*( ( WORD * )Dest ) = TempW ;
			}
		}
		break ;

	case 3 :
		for( i = 0 ; i < loopnum ; i ++ )
		{
			Src  = ( BYTE * )BaseImage->GraphData + i * BaseImage->Pitch ;
			Dest = ( BYTE * )BaseImage->GraphData + ( w - 1 ) * 3 + ( h - i - 1 ) * BaseImage->Pitch ;
			for( j = 0 ; j < w ; j ++, Src += 3, Dest -= 3 )
			{
				TempW = *( ( WORD * )Src ) ;
				TempB = Src[ 2 ] ;
				*( ( WORD * )Src  ) = *( ( WORD * )Dest ) ;
				Src[ 2 ] = Dest[ 2 ] ;
				*( ( WORD * )Dest ) = TempW ;
				Dest[ 2 ] = TempB ;
			}
		}
		break ;

	case 4 :
		for( i = 0 ; i < loopnum ; i ++ )
		{
			Src  = ( BYTE * )BaseImage->GraphData + i * BaseImage->Pitch ;
			Dest = ( BYTE * )BaseImage->GraphData + ( w - 1 ) * 4 + ( h - i - 1 ) * BaseImage->Pitch ;
			for( j = 0 ; j < w ; j ++, Src += 4, Dest -= 4 )
			{
				TempD = *( ( DWORD * )Src ) ;
				*( ( DWORD * )Src  ) = *( ( DWORD * )Dest ) ;
				*( ( DWORD * )Dest ) = TempD ;
			}
		}
		break ;
	}

	// �I��
	return 0 ;
}

// ��{�C���[�W�f�[�^�Ɋ܂܂��s�N�Z���̃A���t�@�l���`�F�b�N����
// �߂�l
// -1:�G���[
// 0:�摜�ɃA���t�@����������
// 1:�摜�ɃA���t�@����������A���ׂčő�(255)�l
// 2:�摜�ɃA���t�@����������A���݂���A���t�@�l�͍ŏ�(0)�ƍő�(255)�������͍ŏ�(0)�̂�
// 3:�摜�ɃA���t�@����������A�ŏ��ƍő�ȊO�̒��Ԃ̒l������
extern int NS_CheckPixelAlphaBaseImage( const BASEIMAGE *BaseImage )
{
	int i, j ;
	int Width ;
	int Height ;
	DWORD AlphaMask, AlphaLoc, AlphaMax ;
	DWORD Pitch ;
	BYTE *p, *lp ;
	const COLORPALETTEDATA *Pal ;
	int Result ;
	DWORD A ;

	// �t�H�[�}�b�g���W���t�H�[�}�b�g�ł͂Ȃ�������G���[
	if( BaseImage->ColorData.Format != DX_BASEIMAGE_FORMAT_NORMAL )
		return -1 ;

	// �摜�ɃA���t�@�������Ȃ���΂����ŏI��
	if( BaseImage->ColorData.AlphaWidth == 0 )
		return 0 ;

	AlphaMask = BaseImage->ColorData.AlphaMask ;
	AlphaLoc = BaseImage->ColorData.AlphaLoc ;
	AlphaMax = AlphaMask >> AlphaLoc ;
	Width = BaseImage->Width ;
	Height = BaseImage->Height ;
	Pitch = ( DWORD )BaseImage->Pitch ;
	lp = ( BYTE * )BaseImage->GraphData ;
	Result = 1 ;
	switch( BaseImage->ColorData.PixelByte )
	{
	case 1:
		Pal = BaseImage->ColorData.Palette ;
		for( i = 0 ; i < Height ; i ++, lp += Pitch )
		{
			p = lp ;
			for( j = 0 ; j < Width ; j ++, p ++ )
			{
				A = Pal[ *p ].Alpha ;
				if( A == 255 ) continue ;

				if( A == 0 )
				{
					if( Result == 1 ) Result = 2 ;
				}
				else return 3 ;
			}
		}
		break ;

	case 2:
		for( i = 0 ; i < Height ; i ++, lp += Pitch )
		{
			p = lp ;
			for( j = 0 ; j < Width ; j ++, p += 2 )
			{
				A = ( *(( WORD * )p ) & AlphaMask ) >> AlphaLoc ;
				if( A == AlphaMax ) continue ;

				if( A == 0 )
				{
					if( Result == 1 ) Result = 2 ;
				}
				else return 3 ;
			}
		}
		break ;

	case 3:
		for( i = 0 ; i < Height ; i ++, lp += Pitch )
		{
			p = lp ;
			for( j = 0 ; j < Width ; j ++, p += 3 )
			{
				A = ( ( p[0] | ( p [1] << 8 ) | ( p[2] << 16 ) ) & AlphaMask ) >> AlphaLoc ;
				if( A == AlphaMax ) continue ;

				if( A == 0 )
				{
					if( Result == 1 ) Result = 2 ;
				}
				else return 3 ;
			}
		}
		break ;

	case 4:
		for( i = 0 ; i < Height ; i ++, lp += Pitch )
		{
			p = lp ;
			for( j = 0 ; j < Width ; j ++, p += 4 )
			{
				A = ( *(( DWORD * )p ) & AlphaMask ) >> AlphaLoc ;
				if( A == AlphaMax ) continue ;

				if( A == 0 )
				{
					if( Result == 1 ) Result = 2 ;
				}
				else return 3 ;
			}
		}
		break ;
	}

	// �I��
	return Result ;
}

// ��{�C���[�W�f�[�^�Ŏg�p����Ă���p���b�g�ԍ��̍ő�l���擾����( �p���b�g�摜�ł͖����ꍇ�� -1 ���Ԃ� )
extern int NS_GetBaseImageUseMaxPaletteNo( const BASEIMAGE *BaseImage )
{
	int i, j ;
	int Width ;
	int Height ;
	DWORD Pitch ;
	BYTE *p, *lp ;
	BYTE MaxPaletteNo ;

	// �t�H�[�}�b�g���W���t�H�[�}�b�g�ł͂Ȃ�������G���[
	if( BaseImage->ColorData.Format != DX_BASEIMAGE_FORMAT_NORMAL )
		return -1 ;

	// �p���b�g�J���[�Ŗ����ꍇ���G���[
	if( BaseImage->ColorData.PixelByte != 1 )
		return -1 ;

	Width = BaseImage->Width ;
	Height = BaseImage->Height ;
	Pitch = ( DWORD )BaseImage->Pitch ;
	lp = ( BYTE * )BaseImage->GraphData ;
	MaxPaletteNo = 0 ;
	for( i = 0 ; i < Height ; i ++, lp += Pitch )
	{
		p = lp ;
		for( j = 0 ; j < Width ; j ++, p ++ )
		{
			// �ő�l���������炻���ŏI��
			if( *p == 255 )
			{
				return 255 ;
			}
			
			if( MaxPaletteNo < *p )
			{
				MaxPaletteNo = *p ;
			}
		}
	}

	// �I��
	return ( int )MaxPaletteNo ;
}

// �Q���̂P�X�P�[�����O���Ȃ���O���t�B�b�N�f�[�^�ԓ]�����s���A���̂��ߊ�{���̓]����`�͎w��ł��Ȃ�
extern int GraphHalfScaleBlt( const COLORDATA *ColorData,
								       void *DestGraphData, int DestPitch,
		 						 const void *SrcGraphData, int SrcPitch,
								 int DestX, int DestY, int SrcX, int SrcY, int SrcWidth, int SrcHeight )
{
	DWORD i, j ;
	DWORD pbyte ;
	DWORD code1, code2, code3, code4, dcode ;
	DWORD rm, gm, bm, am, rl, gl, bl, al, bitdepth, bltw, blth ;
	BYTE *src, *dst ;

	// �T�C�Y���P�ȊO�̊��������G���[
	if( ( SrcWidth  != 1 && ( SrcWidth  & 1 ) ) ||
		( SrcHeight != 1 && ( SrcHeight & 1 ) ) )
		return -1 ;

	bltw = ( DWORD )( SrcWidth  / 2 ) ;
	blth = ( DWORD )( SrcHeight / 2 ) ;

	rm = ColorData->RedMask ;
	rl = ColorData->RedLoc ;
	gm = ColorData->GreenMask ;
	gl = ColorData->GreenLoc ;
	bm = ColorData->BlueMask ;
	bl = ColorData->BlueLoc ;
	am = ColorData->AlphaMask ;
	al = ColorData->AlphaLoc ;
	bitdepth = ColorData->ColorBitDepth ;
	pbyte = ColorData->PixelByte ;

	// �c���ǂ��炩�̃T�C�Y�� 1 ���ǂ����ŏ����𕪊�
	if( SrcWidth == 1 && SrcHeight == 1 )
	{
		// �c�������P�̏ꍇ
		src = ( BYTE * )SrcGraphData + SrcX * pbyte + SrcY * SrcPitch ;
		dst = ( BYTE * )DestGraphData + DestX * pbyte + DestY * DestPitch ;
		switch( bitdepth )
		{
		case 16 :
			*( ( WORD * )dst ) = ( ( WORD * )src )[ 0 ] ;
			break ;

		case 32 :
			*( ( DWORD * )dst ) = ( ( DWORD * )src )[ 0 ] ;
			break ;
		}
	}
	else
	if( SrcWidth == 1 )
	{
		// �����P�̏ꍇ
		for( i = 0 ; i < blth ; i ++ )
		{
			src = ( BYTE * )SrcGraphData + SrcX * pbyte + ( SrcY + ( i << 1 ) ) * SrcPitch ;
			switch( bitdepth )
			{
			case 16 :
				code1 = ( ( WORD * )src )[ 0 ] ;
				code3 = ( ( WORD * )( src + SrcPitch ) )[ 0 ] ;
				break ;

			case 32 :
				code1 = ( ( DWORD * )src )[ 0 ] ;
				code3 = ( ( DWORD * )( src + SrcPitch ) )[ 0 ] ;
				break ;

			default :
				return -1 ;
			}

			dcode = 
				( ( ( ( ( code1 & rm ) >> rl ) + ( ( code3 & rm ) >> rl ) ) >> 1 ) << rl ) |
				( ( ( ( ( code1 & gm ) >> gl ) + ( ( code3 & gm ) >> gl ) ) >> 1 ) << gl ) |
				( ( ( ( ( code1 & bm ) >> bl ) + ( ( code3 & bm ) >> bl ) ) >> 1 ) << bl ) |
				( ( ( ( ( code1 & am ) >> al ) + ( ( code3 & am ) >> al ) ) >> 1 ) << al ) ;

			dst = ( BYTE * )DestGraphData + DestX * pbyte + ( SrcY + i ) * DestPitch ;
			switch( bitdepth )
			{
			case 16 :
				*( ( WORD * )dst ) = ( WORD )dcode ;
				break ;

			case 32 :
				*( ( DWORD * )dst ) = dcode ;
				break ;
			}
		}
	}
	else
	if( SrcHeight == 1 )
	{
		// �c���P�̏ꍇ
		for( j = 0 ; j < bltw ; j ++ )
		{
			src = ( BYTE * )SrcGraphData + ( SrcX + ( j << 1 ) ) * pbyte + SrcY * SrcPitch ;
			switch( bitdepth )
			{
			case 16 :
				code1 = ( ( WORD * )src )[ 0 ] ;
				code2 = ( ( WORD * )src )[ 1 ] ;
				break ;

			case 32 :
				code1 = ( ( DWORD * )src )[ 0 ] ;
				code2 = ( ( DWORD * )src )[ 1 ] ;
				break ;

			default :
				return -1 ;
			}

			dcode = 
				( ( ( ( ( code1 & rm ) >> rl ) + ( ( code2 & rm ) >> rl ) ) >> 1 ) << rl ) |
				( ( ( ( ( code1 & gm ) >> gl ) + ( ( code2 & gm ) >> gl ) ) >> 1 ) << gl ) |
				( ( ( ( ( code1 & bm ) >> bl ) + ( ( code2 & bm ) >> bl ) ) >> 1 ) << bl ) |
				( ( ( ( ( code1 & am ) >> al ) + ( ( code2 & am ) >> al ) ) >> 1 ) << al ) ;

			dst = ( BYTE * )DestGraphData + ( DestX + j ) * pbyte + SrcY * DestPitch ;
			switch( bitdepth )
			{
			case 16 :
				*( ( WORD * )dst ) = ( WORD )dcode ;
				break ;

			case 32 :
				*( ( DWORD * )dst ) = dcode ;
				break ;
			}
		}
	}
	else
	{
		// �c���ǂ�����P�ł͂Ȃ��ꍇ
		for( i = 0 ; i < blth ; i ++ )
		{
			for( j = 0 ; j < bltw ; j ++ )
			{
				src = ( BYTE * )SrcGraphData + ( SrcX + ( j << 1 ) ) * pbyte + ( SrcY + ( i << 1 ) ) * SrcPitch ;
				switch( bitdepth )
				{
				case 16 :
					code1 = ( ( WORD * )src )[ 0 ] ;
					code2 = ( ( WORD * )src )[ 1 ] ;
					code3 = ( ( WORD * )( src + SrcPitch ) )[ 0 ] ;
					code4 = ( ( WORD * )( src + SrcPitch ) )[ 1 ] ;
					break ;

				case 32 :
					code1 = ( ( DWORD * )src )[ 0 ] ;
					code2 = ( ( DWORD * )src )[ 1 ] ;
					code3 = ( ( DWORD * )( src + SrcPitch ) )[ 0 ] ;
					code4 = ( ( DWORD * )( src + SrcPitch ) )[ 1 ] ;
					break ;

				default :
					return -1 ;
				}

				dcode = 
					( ( ( ( ( code1 & rm ) >> rl ) + ( ( code2 & rm ) >> rl ) + ( ( code3 & rm ) >> rl ) + ( ( code4 & rm ) >> rl ) ) >> 2 ) << rl ) |
					( ( ( ( ( code1 & gm ) >> gl ) + ( ( code2 & gm ) >> gl ) + ( ( code3 & gm ) >> gl ) + ( ( code4 & gm ) >> gl ) ) >> 2 ) << gl ) |
					( ( ( ( ( code1 & bm ) >> bl ) + ( ( code2 & bm ) >> bl ) + ( ( code3 & bm ) >> bl ) + ( ( code4 & bm ) >> bl ) ) >> 2 ) << bl ) |
					( ( ( ( ( code1 & am ) >> al ) + ( ( code2 & am ) >> al ) + ( ( code3 & am ) >> al ) + ( ( code4 & am ) >> al ) ) >> 2 ) << al ) ;

				dst = ( BYTE * )DestGraphData + ( DestX + j ) * pbyte + ( SrcY + i ) * DestPitch ;
				switch( bitdepth )
				{
				case 16 :
					*( ( WORD * )dst ) = ( WORD )dcode ;
					break ;

				case 32 :
					*( ( DWORD * )dst ) = dcode ;
					break ;
				}
			}
		}
	}

	// �I��
	return 0 ;
}

// �g���Ă��Ȃ��r�b�g���w��̒l�Ŗ��߂�
extern int NoneMaskFill( RECT *Rect, void *ImageData, int Pitch, COLORDATA *ColorData, unsigned int Fill )
{
	DWORD Width, Height;
	DWORD AddPitch;
	BYTE *DestP;
	DWORD PixelByte, FillColor;

	// �g���Ă��Ȃ��r�b�g���Ȃ��ꍇ�͉������Ȃ�
	if( ColorData->NoneMask == 0 ) return 0;

	// ������`�̃T�C�Y�𓾂�
	RectAdjust( Rect );
	GetRectSize( Rect, ( int * )&Width, ( int * )&Height );

	// �]�������̏���
	PixelByte = ColorData->PixelByte;
	DestP     = ( BYTE * )ImageData + Rect->left * PixelByte + Rect->top * Pitch;
	AddPitch  = Pitch - Width * PixelByte;
	FillColor = Fill << ColorData->NoneLoc;

	// �]��
#ifdef DX_NON_INLINE_ASM
	int i ;
	switch( PixelByte )
	{
	case 4 :
		if( ColorData->NoneMask == 0xff000000 )
		{
			do
			{
				i = ( int )Width ;
				do
				{
					DestP[ 3 ] = ( BYTE )Fill ;
					DestP += 4 ;
				}while( -- i != 0 ) ;
				DestP += AddPitch ;
			}while( -- Height != 0 ) ;
		}
		else
		{
			do
			{
				i = ( int )Width ;
				do
				{
					*( ( DWORD * )DestP ) |= FillColor ;
					DestP += 4 ;
				}while( -- i != 0 ) ;
				DestP += AddPitch ;
			}while( -- Height != 0 ) ;
		}
		break ;

	case 2 :
		do
		{
			i = ( int )Width ;
			do
			{
				*( ( WORD * )DestP ) |= ( WORD )FillColor ;
				DestP += 2 ;
			}while( -- i != 0 ) ;
			DestP += AddPitch ;
		}while( -- Height != 0 ) ;
		break ;
	}
#else
	switch( PixelByte )
	{
	case 4:
		if( ColorData->NoneMask == 0xff000000 )
		{
			__asm
			{
				MOV EDI, DestP
				MOV EDX, Height
				MOV EBX, AddPitch
				MOV EAX, Fill
		LOOP1_32_1:
				MOV ECX, Width
		LOOP2_32_1:
				OR  [EDI+3], AL
				ADD EDI, 4
				DEC ECX
				JNZ LOOP2_32_1

				ADD EDI, EBX
				DEC EDX
				JNZ LOOP1_32_1
			}
		}
		else
		{
			__asm
			{
				MOV EDI, DestP
				MOV EDX, Height
				MOV EBX, AddPitch
				MOV EAX, FillColor
		LOOP1_32_2:
				MOV ECX, Width
		LOOP2_32_2:
				OR  [EDI], EAX
				ADD EDI, 4
				DEC ECX
				JNZ LOOP2_32_2

				ADD EDI, EBX
				DEC EDX
				JNZ LOOP1_32_2
			}
		}
		break;

	case 2:
		__asm
		{
			MOV EDI, DestP
			MOV EDX, Height
			MOV EBX, AddPitch
			MOV EAX, FillColor
	LOOP1_16:
			MOV ECX, Width
	LOOP2_16:
			OR  [EDI], AX
			ADD EDI, 2
			DEC ECX
			JNZ LOOP2_16

			ADD EDI, EBX
			DEC EDX
			JNZ LOOP1_16
		}
		break;
	}
#endif

	return 0;
}

// YV12 �t�H�[�}�b�g�̃C���[�W�� XRGB32 �̃r�b�g�}�b�v�C���[�W�ɕϊ�����
extern int ConvertYV12ToXRGB32( void *YV12Image, int Width, int Height, BASEIMAGE *DestBaseImage )
{
	int i, j ;
	BYTE *SrcY ;
	BYTE *SrcU ;
	BYTE *SrcV ;
	BYTE *SrcYY ;
	BYTE *SrcUY ;
	BYTE *SrcVY ;
	BYTE *Dest ;
	BYTE *DestY ;
	DWORD DestPitch2 ;
	DWORD DestPitch ;
	DWORD SrcYPitch2 ;
	DWORD SrcYPitch ;
	DWORD SrcVUPitch ;
	int U, V, Y, C ;
	int UB, VUG, VR ;

	SrcYY = ( BYTE * )YV12Image ;
	SrcVY = ( BYTE * )SrcYY + Width * Height ;
	SrcUY = ( BYTE * )SrcVY + Width * Height / 4 ;

	SrcYPitch  = ( DWORD )Width ;
	SrcYPitch2 = ( DWORD )( SrcYPitch * 2 ) ;

	DestPitch  = ( DWORD )DestBaseImage->Pitch ;
	DestPitch2 = DestPitch * 2 ;
	SrcVUPitch = SrcYPitch / 2 ;
	DestY = ( BYTE * )DestBaseImage->GraphData ;

	Width /= 2 ;
	Height /= 2 ;
	for( i = 0 ; i < Height ; i ++, DestY += DestPitch2, SrcYY += SrcYPitch2, SrcVY += SrcVUPitch, SrcUY += SrcVUPitch )
	{
		SrcY = SrcYY ;
		SrcV = SrcVY ;
		SrcU = SrcUY ;
		Dest = DestY ;
		for( j = 0 ; j < Width ; j ++, Dest += 8 )
		{
			U = ( int )SrcU[ 0 ] - 128 ;
			V = ( int )SrcV[ 0 ] - 128 ;

			UB  = U *  58064 ;				//  1.772
			VUG = U * -11272 + V * -23396 ;	// -0.344 -0.714
			VR  = V *  45940 ;				//  1.402

			Y = SrcY[ 0 ] << 15 ;
			C = Y + UB  ; Dest[             0 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // B
			C = Y + VUG ; Dest[             1 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // G
			C = Y + VR  ; Dest[             2 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // R
			Dest[             3 ] = 255 ;

			Y = SrcY[ 1 ] << 15 ;
			C = Y + UB  ; Dest[             4 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // B
			C = Y + VUG ; Dest[             5 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // G
			C = Y + VR  ; Dest[             6 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // R
			Dest[             7 ] = 255 ;

			Y = SrcY[ SrcYPitch + 0 ] << 15 ;
			C = Y + UB  ; Dest[ DestPitch + 0 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // B
			C = Y + VUG ; Dest[ DestPitch + 1 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // G
			C = Y + VR  ; Dest[ DestPitch + 2 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // R
			Dest[ DestPitch + 3 ] = 255 ;

			Y = SrcY[ SrcYPitch + 1 ] << 15 ;
			C = Y + UB  ; Dest[ DestPitch + 4 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // B
			C = Y + VUG ; Dest[ DestPitch + 5 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // G
			C = Y + VR  ; Dest[ DestPitch + 6 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // R
			Dest[ DestPitch + 7 ] = 255 ;

			SrcY += 2 ;
			SrcV ++ ;
			SrcU ++ ;
		}
	}

	// �I��
	return 0 ;
}

// NV11 �t�H�[�}�b�g�̃C���[�W�� XRGB32 �̃r�b�g�}�b�v�C���[�W�ɕϊ�����
extern int ConvertNV11ToXRGB32( void *NV11Image, int Width, int Height, BASEIMAGE *DestBaseImage )
{
	int i, j ;
	BYTE *SrcY ;
	BYTE *SrcUV ;
	BYTE *SrcYY ;
	BYTE *SrcUVY ;
	BYTE *Dest ;
	BYTE *DestY ;
	DWORD DestPitch ;
	DWORD SrcYPitch ;
	DWORD SrcVUPitch ;
	int U, V, Y ;
	int UB, VUG, VR, C ;

	SrcYY  = ( BYTE * )NV11Image ;
	SrcUVY = ( BYTE * )SrcYY + Width * Height ;

	SrcYPitch  = ( DWORD )Width ;

	DestPitch  = ( DWORD )DestBaseImage->Pitch ;
	SrcVUPitch = SrcYPitch / 2 ;
	DestY = ( BYTE * )DestBaseImage->GraphData ;

	Width /= 4 ;
	for( i = 0 ; i < Height ; i ++, DestY += DestPitch, SrcYY += SrcYPitch, SrcUVY += SrcVUPitch )
	{
		SrcY = SrcYY ;
		SrcUV = SrcUVY ;
		Dest = DestY ;
		for( j = 0 ; j < Width ; j ++, Dest += 16 )
		{
			U = ( int )SrcUV[ 0 ] - 128 ;
			V = ( int )SrcUV[ 1 ] - 128 ;

			UB  = U *  58064 ;				//  1.772
			VUG = U * -11272 + V * -23396 ;	// -0.344 -0.714
			VR  = V *  45940 ;				//  1.402

			Y = SrcY[ 0 ] << 15 ;
			C = Y + UB  ; Dest[  0 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // B
			C = Y + VUG ; Dest[  1 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // G
			C = Y + VR  ; Dest[  2 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // R
			Dest[ 3 ] = 255 ;

			Y = SrcY[ 1 ] << 15 ;
			C = Y + UB  ; Dest[  4 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // B
			C = Y + VUG ; Dest[  5 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // G
			C = Y + VR  ; Dest[  6 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // R
			Dest[ 7 ] = 255 ;

			Y = SrcY[ 2 ] << 15 ;
			C = Y + UB  ; Dest[  8 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // B
			C = Y + VUG ; Dest[  9 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // G
			C = Y + VR  ; Dest[ 10 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // R
			Dest[ 11 ] = 255 ;

			Y = SrcY[ 3 ] << 15 ;
			C = Y + UB  ; Dest[ 12 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // B
			C = Y + VUG ; Dest[ 13 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // G
			C = Y + VR  ; Dest[ 14 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // R
			Dest[ 15 ] = 255 ;

			SrcY += 4 ;
			SrcUV += 2 ;
		}
	}

	// �I��
	return 0 ;
}

// NV12 �t�H�[�}�b�g�̃C���[�W�� XRGB32 �̃r�b�g�}�b�v�C���[�W�ɕϊ�����
extern int ConvertNV12ToXRGB32( void *NV12Image, int Width, int Height, BASEIMAGE *DestBaseImage )
{
	int i, j ;
	BYTE *SrcY ;
	BYTE *SrcUV ;
	BYTE *SrcYY ;
	BYTE *SrcUVY ;
	BYTE *Dest ;
	BYTE *DestY ;
	DWORD DestPitch2 ;
	DWORD DestPitch ;
	DWORD SrcYPitch2 ;
	DWORD SrcYPitch ;
	DWORD SrcVUPitch ;
	int U, V, Y ;
	int UB, VUG, VR, C ;

	SrcYY = ( BYTE * )NV12Image ;
	SrcUVY = ( BYTE * )SrcYY + Width * Height ;

	SrcYPitch  = ( DWORD )Width ;
	SrcYPitch2 = SrcYPitch * 2 ;

	DestPitch  = ( DWORD )DestBaseImage->Pitch ;
	DestPitch2 = DestPitch * 2 ;
	SrcVUPitch = SrcYPitch ;
	DestY = ( BYTE * )DestBaseImage->GraphData ;

	Width /= 2 ;
	Height /= 2 ;
	for( i = 0 ; i < Height ; i ++, DestY += DestPitch2, SrcYY += SrcYPitch2, SrcUVY += SrcVUPitch )
	{
		SrcY = SrcYY ;
		SrcUV = SrcUVY ;
		Dest = DestY ;
		for( j = 0 ; j < Width ; j ++, Dest += 8 )
		{
			U = ( int )SrcUV[ 0 ] - 128 ;
			V = ( int )SrcUV[ 1 ] - 128 ;

			UB  = U *  58064 ;				//  1.772
			VUG = U * -11272 + V * -23396 ;	// -0.344 -0.714
			VR  = V *  45940 ;				//  1.402

			Y = SrcY[ 0 ] << 15 ;
			C = Y + UB  ; Dest[             0 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // B
			C = Y + VUG ; Dest[             1 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // G
			C = Y + VR  ; Dest[             2 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // R
			Dest[             3 ] = 255 ;

			Y = SrcY[ 1 ] << 15 ;
			C = Y + UB  ; Dest[             4 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // B
			C = Y + VUG ; Dest[             5 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // G
			C = Y + VR  ; Dest[             6 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // R
			Dest[             7 ] = 255 ;

			Y = SrcY[ SrcYPitch + 0 ] << 15 ;
			C = Y + UB  ; Dest[ DestPitch + 0 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // B
			C = Y + VUG ; Dest[ DestPitch + 1 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // G
			C = Y + VR  ; Dest[ DestPitch + 2 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // R
			Dest[ DestPitch + 3 ] = 255 ;

			Y = SrcY[ SrcYPitch + 1 ] << 15 ;
			C = Y + UB  ; Dest[ DestPitch + 4 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // B
			C = Y + VUG ; Dest[ DestPitch + 5 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // G
			C = Y + VR  ; Dest[ DestPitch + 6 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // R
			Dest[ DestPitch + 7 ] = 255 ;

			SrcY += 2 ;
			SrcUV += 2 ;
		}
	}

	// �I��
	return 0 ;
}

// YUY2 �t�H�[�}�b�g�̃C���[�W�� XRGB32 �̃r�b�g�}�b�v�C���[�W�ɕϊ�����
extern int ConvertYUY2ToXRGB32( void *YUY2Image, int Width, int Height, BASEIMAGE *DestBaseImage )
{
	int i, j ;
	BYTE *Src ;
	BYTE *SrcY ;
	BYTE *Dest ;
	BYTE *DestY ;
	DWORD DestPitch ;
	DWORD SrcPitch ;
	int U, V, Y ;
	int UB, VUG, VR, C ;

	SrcY = ( BYTE * )YUY2Image ;

	SrcPitch  = ( DWORD )( Width * 2 ) ;
	DestPitch = ( DWORD )DestBaseImage->Pitch ;
	DestY     = ( BYTE * )DestBaseImage->GraphData ;

	Width /= 2 ;
	for( i = 0 ; i < Height ; i ++, DestY += DestPitch, SrcY += SrcPitch )
	{
		Src = SrcY ;
		Dest = DestY ;
		for( j = 0 ; j < Width ; j ++, Dest += 8, Src += 4 )
		{
			U = ( int )Src[ 1 ] - 128 ;
			V = ( int )Src[ 3 ] - 128 ;

			UB  = U *  58064 ;				//  1.772
			VUG = U * -11272 + V * -23396 ;	// -0.344 -0.714
			VR  = V *  45940 ;				//  1.402

			Y = Src[ 0 ] << 15 ;
			C = Y + UB  ; Dest[ 0 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // B
			C = Y + VUG ; Dest[ 1 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // G
			C = Y + VR  ; Dest[ 2 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // R
			Dest[ 3 ] = 255 ;

			Y = Src[ 2 ] << 15 ;
			C = Y + UB  ; Dest[ 4 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // B
			C = Y + VUG ; Dest[ 5 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // G
			C = Y + VR  ; Dest[ 6 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // R
			Dest[ 7 ] = 255 ;
		}
	}

	// �I��
	return 0 ;
}

// UYVY �t�H�[�}�b�g�̃C���[�W�� XRGB32 �̃r�b�g�}�b�v�C���[�W�ɕϊ�����
extern int ConvertUYVYToXRGB32( void *UYVYImage, int Width, int Height, BASEIMAGE *DestBaseImage )
{
	int i, j ;
	BYTE *Src ;
	BYTE *SrcY ;
	BYTE *Dest ;
	BYTE *DestY ;
	DWORD DestPitch ;
	DWORD SrcPitch ;
	int U, V, Y ;
	int UB, VUG, VR, C ;

	SrcY = ( BYTE * )UYVYImage ;

	SrcPitch  = ( DWORD )( Width * 2 ) ;
	DestPitch = ( DWORD )DestBaseImage->Pitch ;
	DestY     = ( BYTE * )DestBaseImage->GraphData ;

	Width /= 2 ;
	for( i = 0 ; i < Height ; i ++, DestY += DestPitch, SrcY += SrcPitch )
	{
		Src = SrcY ;
		Dest = DestY ;
		for( j = 0 ; j < Width ; j ++, Dest += 8, Src += 4 )
		{
			U = ( int )Src[ 0 ] - 128 ;
			V = ( int )Src[ 2 ] - 128 ;

			UB  = U *  58064 ;				//  1.772
			VUG = U * -11272 + V * -23396 ;	// -0.344 -0.714
			VR  = V *  45940 ;				//  1.402

			Y = Src[ 1 ] << 15 ;
			C = Y + UB  ; Dest[ 0 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // B
			C = Y + VUG ; Dest[ 1 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // G
			C = Y + VR  ; Dest[ 2 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // R
			Dest[ 3 ] = 255 ;

			Y = Src[ 3 ] << 15 ;
			C = Y + UB  ; Dest[ 4 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // B
			C = Y + VUG ; Dest[ 5 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // G
			C = Y + VR  ; Dest[ 6 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // R
			Dest[ 7 ] = 255 ;
		}
	}

	// �I��
	return 0 ;
}

// YVYU �t�H�[�}�b�g�̃C���[�W�� XRGB32 �̃r�b�g�}�b�v�C���[�W�ɕϊ�����
extern int ConvertYVYUToXRGB32( void *YVYUImage, int Width, int Height, BASEIMAGE *DestBaseImage )
{
	int i, j ;
	BYTE *Src ;
	BYTE *SrcY ;
	BYTE *Dest ;
	BYTE *DestY ;
	DWORD DestPitch ;
	DWORD SrcPitch ;
	int U, V, Y ;
	int UB, VUG, VR, C ;

	SrcY = ( BYTE * )YVYUImage ;

	SrcPitch  = ( DWORD )( Width * 2 ) ;
	DestPitch = ( DWORD )DestBaseImage->Pitch ;
	DestY     = ( BYTE * )DestBaseImage->GraphData ;

	Width /= 2 ;
	for( i = 0 ; i < Height ; i ++, DestY += DestPitch, SrcY += SrcPitch )
	{
		Src = SrcY ;
		Dest = DestY ;
		for( j = 0 ; j < Width ; j ++, Dest += 8, Src += 4 )
		{
			U = ( int )Src[ 3 ] - 128 ;
			V = ( int )Src[ 1 ] - 128 ;

			UB  = U *  58064 ;				//  1.772
			VUG = U * -11272 + V * -23396 ;	// -0.344 -0.714
			VR  = V *  45940 ;				//  1.402

			Y = Src[ 0 ] << 15 ;
			C = Y + UB  ; Dest[ 0 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // B
			C = Y + VUG ; Dest[ 1 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // G
			C = Y + VR  ; Dest[ 2 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // R
			Dest[ 3 ] = 255 ;

			Y = Src[ 2 ] << 15 ;
			C = Y + UB  ; Dest[ 4 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // B
			C = Y + VUG ; Dest[ 5 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // G
			C = Y + VR  ; Dest[ 6 ] = ( BYTE )( C < 0 ? 0 : ( C > 0x7F8000 ? 255 : ( C >> 15 ) ) ) ; // R
			Dest[ 7 ] = 255 ;
		}
	}

	// �I��
	return 0 ;
}

// YV12 �t�H�[�}�b�g�̃C���[�W�� Y������UV�����̃C���[�W�ɕ�������
extern int ConvertYV12ToYPlane_UVPlane( void *YV12Image, int Width, int Height, void *YBuffer, void *UVBuffer )
{
	int i, j ;
	BYTE *SrcY ;
	BYTE *SrcU ;
	BYTE *SrcV ;
	BYTE *DestY ;
	BYTE *DestUV ;
	DWORD DestYPitch ;
	DWORD DestYPitch2 ;
	DWORD DestUVPitch ;
	DWORD SrcYPitch2 ;
	DWORD SrcYPitch ;
	DWORD SrcVUPitch ;

	SrcY = ( BYTE * )YV12Image ;
	SrcV = ( BYTE * )SrcY + Width * Height ;
	SrcU = ( BYTE * )SrcV + Width * Height / 4 ;

	SrcYPitch  = ( DWORD )Width ;
	SrcYPitch2 = ( DWORD )( SrcYPitch * 2 ) ;

	DestYPitch  = Width ;
	DestYPitch2 = DestYPitch * 2 ;
	DestUVPitch = Width ;
	SrcVUPitch = SrcYPitch / 2 ;
	DestY  = ( BYTE * )YBuffer ;
	DestUV = ( BYTE * )UVBuffer ;

	Width /= 2 ;
	Height /= 2 ;
	for( i = 0 ; i < Height ; i ++, DestY += DestYPitch2, DestUV += DestUVPitch, SrcY += SrcYPitch2, SrcV += SrcVUPitch, SrcU += SrcVUPitch )
	{
		BYTE *SrcYTemp ;
		BYTE *SrcVTemp ;
		BYTE *SrcUTemp ;
		BYTE *DestYTemp ;
		BYTE *DestUVTemp ;

		SrcYTemp = SrcY ;
		SrcVTemp = SrcV ;
		SrcUTemp = SrcU ;
		DestYTemp = DestY ;
		DestUVTemp = DestUV ;
		for( j = 0 ; j < Width ; j ++, DestYTemp += 2, DestUVTemp += 2 )
		{
			DestYTemp[              0 ] = SrcYTemp[             0 ] ;
			DestYTemp[              1 ] = SrcYTemp[             1 ] ;
			DestYTemp[ DestYPitch + 0 ] = SrcYTemp[ SrcYPitch + 0 ] ;
			DestYTemp[ DestYPitch + 1 ] = SrcYTemp[ SrcYPitch + 1 ] ;

			DestUVTemp[ 0 ] = SrcUTemp[ 0 ] ;
			DestUVTemp[ 1 ] = SrcVTemp[ 0 ] ;

			SrcYTemp += 2 ;
			SrcVTemp ++ ;
			SrcUTemp ++ ;
		}
	}

	// �I��
	return 0 ;
}

// NV11 �t�H�[�}�b�g�̃C���[�W�� Y������UV�����̃C���[�W�ɕ�������
extern int ConvertNV11ToYPlane_UVPlane( void *NV11Image, int Width, int Height, void *YBuffer, void *UVBuffer )
{
	int i, j ;
	BYTE *SrcY ;
	BYTE *SrcUV ;
	BYTE *DestY ;
	BYTE *DestUV ;
	DWORD DestYPitch ;
	DWORD DestUVPitch ;
	DWORD SrcYPitch ;
	DWORD SrcVUPitch ;

	SrcY  = ( BYTE * )NV11Image ;
	SrcUV = ( BYTE * )SrcY + Width * Height ;

	SrcYPitch  = ( DWORD )Width ;
	SrcVUPitch = SrcYPitch / 2 ;

	DestYPitch  = ( DWORD )Width ;
	DestUVPitch = ( DWORD )Width / 2 ; 

	DestY  = ( BYTE * )YBuffer ;
	DestUV = ( BYTE * )UVBuffer ;

	Width /= 4 ;
	for( i = 0 ; i < Height ; i ++, DestY += DestYPitch, DestUV += DestUVPitch, SrcY += SrcYPitch, SrcUV += SrcVUPitch )
	{
		BYTE *SrcYTemp ;
		BYTE *SrcUVTemp ;
		BYTE *DestYTemp ;
		BYTE *DestUVTemp ;

		SrcYTemp   = SrcY ;
		SrcUVTemp  = SrcUV ;
		DestYTemp  = DestY ;
		DestUVTemp = DestUV ;
		for( j = 0 ; j < Width ; j ++, DestYTemp += 4, DestUVTemp += 2 )
		{
			DestYTemp[ 0 ] = SrcYTemp[ 0 ] ;
			DestYTemp[ 1 ] = SrcYTemp[ 1 ] ;
			DestYTemp[ 2 ] = SrcYTemp[ 2 ] ;
			DestYTemp[ 3 ] = SrcYTemp[ 3 ] ;

			DestUVTemp[ 0 ] = SrcUVTemp[ 0 ] ;
			DestUVTemp[ 1 ] = SrcUVTemp[ 1 ] ;

			SrcYTemp  += 4 ;
			SrcUVTemp += 2 ;
		}
	}

	// �I��
	return 0 ;
}

// NV12 �t�H�[�}�b�g�̃C���[�W�� Y������UV�����̃C���[�W�ɕ�������
extern int ConvertNV12ToYPlane_UVPlane( void *NV12Image, int Width, int Height, void *YBuffer, void *UVBuffer )
{
	int i, j ;
	BYTE *SrcY ;
	BYTE *SrcUV ;
	BYTE *DestY ;
	BYTE *DestUV ;
	DWORD DestYPitch2 ;
	DWORD DestYPitch ;
	DWORD DestUVPitch ;
	DWORD SrcYPitch2 ;
	DWORD SrcYPitch ;
	DWORD SrcUVPitch ;

	SrcY  = ( BYTE * )NV12Image ;
	SrcUV = ( BYTE * )SrcY + Width * Height ;

	SrcYPitch  = ( DWORD )Width ;
	SrcYPitch2 = SrcYPitch * 2 ;
	SrcUVPitch = SrcYPitch ;

	DestYPitch  = Width ;
	DestYPitch2 = DestYPitch * 2 ;
	DestUVPitch = Width ;

	DestY  = ( BYTE * )YBuffer ;
	DestUV = ( BYTE * )UVBuffer ;

	Width  /= 2 ;
	Height /= 2 ;
	for( i = 0 ; i < Height ; i ++, DestY += DestYPitch2, DestUV += DestUVPitch, SrcY += SrcYPitch2, SrcUV += SrcUVPitch )
	{
		BYTE *SrcYTemp ;
		BYTE *SrcUVTemp ;
		BYTE *DestYTemp ;
		BYTE *DestUVTemp ;

		SrcYTemp   = SrcY ;
		SrcUVTemp  = SrcUV ;
		DestYTemp  = DestY ;
		DestUVTemp = DestUV ;
		for( j = 0 ; j < Width ; j ++, DestYTemp += 2, DestUVTemp += 2 )
		{
			DestYTemp[              0 ] = SrcYTemp[             0 ] ;
			DestYTemp[              1 ] = SrcYTemp[             1 ] ;
			DestYTemp[ DestYPitch + 0 ] = SrcYTemp[ SrcYPitch + 0 ] ;
			DestYTemp[ DestYPitch + 1 ] = SrcYTemp[ SrcYPitch + 1 ] ;

			DestUVTemp[ 0 ] = SrcUVTemp[ 0 ] ;
			DestUVTemp[ 1 ] = SrcUVTemp[ 1 ] ;

			SrcYTemp  += 2 ;
			SrcUVTemp += 2 ;
		}
	}

	// �I��
	return 0 ;
}

// YUY2 �t�H�[�}�b�g�̃C���[�W�� Y������UV�����̃C���[�W�ɕ�������
extern int ConvertYUY2ToYPlane_UVPlane( void *YUY2Image, int Width, int Height, void *YBuffer, void *UVBuffer )
{
	int i, j ;
	BYTE *Src ;
	BYTE *DestY ;
	BYTE *DestUV ;
	DWORD DestYPitch ;
	DWORD DestUVPitch ;
	DWORD SrcPitch ;

	Src = ( BYTE * )YUY2Image ;

	SrcPitch  = ( DWORD )( Width * 2 ) ;

	DestYPitch  = ( DWORD )Width ;
	DestUVPitch = DestYPitch ;

	DestY  = ( BYTE * )YBuffer ;
	DestUV = ( BYTE * )UVBuffer ;

	Width /= 2 ;
	for( i = 0 ; i < Height ; i ++, DestY += DestYPitch, DestUV += DestUVPitch, Src += SrcPitch )
	{
		BYTE *SrcTemp ;
		BYTE *DestYTemp ;
		BYTE *DestUVTemp ;

		SrcTemp = Src ;
		DestYTemp = DestY ;
		DestUVTemp = DestUV ;
		for( j = 0 ; j < Width ; j ++, DestYTemp += 2, DestUVTemp += 2 )
		{
			DestYTemp[ 0 ] = SrcTemp[ 0 ] ;
			DestYTemp[ 1 ] = SrcTemp[ 2 ] ;

			DestUVTemp[ 0 ] = SrcTemp[ 1 ] ;
			DestUVTemp[ 1 ] = SrcTemp[ 3 ] ;

			SrcTemp += 4 ;
		}
	}

	// �I��
	return 0 ;
}

// UYVY �t�H�[�}�b�g�̃C���[�W�� Y������UV�����̃C���[�W�ɕ�������
extern int ConvertUYVYToYPlane_UVPlane( void *UYVYImage, int Width, int Height, void *YBuffer, void *UVBuffer )
{
	int i, j ;
	BYTE *Src ;
	BYTE *DestY ;
	BYTE *DestUV ;
	DWORD DestYPitch ;
	DWORD DestUVPitch ;
	DWORD SrcPitch ;

	Src = ( BYTE * )UYVYImage ;

	SrcPitch    = ( DWORD )( Width * 2 ) ;

	DestYPitch  = ( DWORD )Width ;
	DestUVPitch = ( DWORD )Width ;

	DestY       = ( BYTE * )YBuffer ;
	DestUV      = ( BYTE * )UVBuffer ;

	Width /= 2 ;
	for( i = 0 ; i < Height ; i ++, DestY += DestYPitch, DestUV += DestUVPitch, Src += SrcPitch )
	{
		BYTE *SrcTemp ;
		BYTE *DestYTemp ;
		BYTE *DestUVTemp ;

		SrcTemp    = Src ;
		DestYTemp  = DestY ;
		DestUVTemp = DestUV ;
		for( j = 0 ; j < Width ; j ++, DestYTemp += 2, DestUVTemp += 2 )
		{
			DestYTemp[ 0 ] = SrcTemp[ 1 ] ;
			DestYTemp[ 1 ] = SrcTemp[ 3 ] ;

			DestUVTemp[ 0 ] = SrcTemp[ 0 ] ;
			DestUVTemp[ 1 ] = SrcTemp[ 2 ] ;

			SrcTemp += 4 ;
		}
	}

	// �I��
	return 0 ;
}

// YVYU �t�H�[�}�b�g�̃C���[�W�� Y������UV�����̃C���[�W�ɕ�������
extern int ConvertYVYUToYPlane_UVPlane( void *YVYUImage, int Width, int Height, void *YBuffer, void *UVBuffer )
{
	int i, j ;
	BYTE *Src ;
	BYTE *DestY ;
	BYTE *DestUV ;
	DWORD DestYPitch ;
	DWORD DestUVPitch ;
	DWORD SrcPitch ;

	Src         = ( BYTE * )YVYUImage ;

	SrcPitch    = ( DWORD )( Width * 2 ) ;

	DestYPitch  = ( DWORD )Width ;
	DestUVPitch = ( DWORD )Width ;

	DestY       = ( BYTE * )YBuffer ;
	DestUV      = ( BYTE * )UVBuffer ;

	Width /= 2 ;
	for( i = 0 ; i < Height ; i ++, DestY += DestYPitch, DestUV += DestUVPitch, Src += SrcPitch )
	{
		BYTE *SrcTemp ;
		BYTE *DestYTemp ;
		BYTE *DestUVTemp ;

		SrcTemp = Src ;
		DestYTemp = DestY ;
		DestUVTemp = DestUV ;
		for( j = 0 ; j < Width ; j ++, DestYTemp += 2, DestUVTemp += 2 )
		{
			DestYTemp[ 0 ] = SrcTemp[ 0 ] ;
			DestYTemp[ 1 ] = SrcTemp[ 2 ] ;

			DestUVTemp[ 0 ] = SrcTemp[ 3 ] ;
			DestUVTemp[ 1 ] = SrcTemp[ 1 ] ;

			SrcTemp += 4 ;
		}
	}

	// �I��
	return 0 ;
}

// 16�K���f�B�U�s��̃e�[�u��
static int DitherTable[16] = {
	 0,  8,  2, 10,
	12,  4, 14,  6,
     3, 11,  1,  9,
    15,  7, 13,  5
};
/*	0,   8, 2, 10,
	12,  4,  14,  6,
    3, 11,  1,  9,
    15,   7, 13, 5
*/

/*	1,   3, 15, 13,
	9,  11,  6,  8,
    16, 14,  2,  4,
    5,   7, 10, 12
*/

// �J���[�}�b�`���O���Ȃ���O���t�B�b�N�f�[�^�ԓ]�����s�� Ver2
extern int NS_GraphColorMatchBltVer2( void *DestGraphData,       int DestPitch,  const COLORDATA *DestColorData,
									  const void *SrcGraphData,  int SrcPitch,   const COLORDATA *SrcColorData,
									  const void *AlphaMask,     int AlphaPitch, const COLORDATA *AlphaColorData,
								 	  POINT DestPoint, const RECT *SrcRect, int ReverseFlag,
									  int TransColorAlphaTestFlag, unsigned int TransColor,
									  int ImageShavedMode, int AlphaOnlyFlag,
									  int RedIsAlphaFlag,  int TransColorNoMoveFlag, int Pal8ColorMatch )
{
	int BltWidth, BltHeight ;
	int AlphaFlag, AlphaMaskFlag ;
	COLORDATA SColor, AColor, DColor ;
	int FloatTypeFlag ;

	SColor = *SrcColorData ;
	DColor = *DestColorData ;
	if( AlphaColorData != NULL ) AColor = *AlphaColorData ; else _MEMSET( &AColor, 0, sizeof( AColor ) ) ;

	// �]�����Ɠ]����̂ǂ��炩�����������_�^���ǂ������Z�b�g
	FloatTypeFlag = SColor.FloatTypeFlag != 0 || DColor.FloatTypeFlag != 0 ? TRUE : FALSE ;

	// �A���t�@�`�����l�����������邩�ǂ����̃t���O���Z�b�g����
	AlphaFlag = DColor.AlphaWidth != 0 && ( AlphaMask != NULL || SColor.AlphaWidth != 0 ) ;
	
	// �A���t�@�`�����l���f�[�^�̓A���t�@�}�X�N�����邩�\�[�X�O���t�B�b�N�ォ���邩�̃t���O���Z�b�g
	AlphaMaskFlag = DColor.AlphaWidth != 0 && AlphaMask != NULL && SColor.AlphaWidth == 0 ;

	// ���ߐF�A���t�@�e�X�g�����́A�A���t�@�}�X�N�A�܂��̓A���t�@�`�����l��������ꍇ�A
	// �������͓]����O���t�B�b�N�̃t�H�[�}�b�g���A���t�@�Ȃ��������ꍇ�͋����I�ɂn�e�e�ɂȂ�
	// ���A�Ԑ������������ɕϊ�����w�肪����ꍇ���n�e�e�ɂȂ�
	if( AlphaFlag == TRUE || DColor.AlphaWidth == 0 || RedIsAlphaFlag == TRUE  ) TransColorAlphaTestFlag = FALSE ;

	// ���]�t���O������ꍇ�͓��ߐF�����͖���
	if( ReverseFlag ) TransColorNoMoveFlag = FALSE ;

	// �]���T�C�Y���Z�b�g
	BltWidth = SrcRect->right - SrcRect->left ;
	BltHeight = SrcRect->bottom - SrcRect->top ;
	if( BltWidth == 0 || BltHeight == 0 ) return -1 ;

	// �����^��1�`�����l���ӂ�16�r�b�g�̃t�H�[�}�b�g�̏ꍇ�͕���
	if( ( SColor.FloatTypeFlag == FALSE && SColor.ChannelBitDepth == 16 ) ||
		( DColor.FloatTypeFlag == FALSE && DColor.ChannelBitDepth == 16 ) )
	{
		// �]�����Ɠ]����̃t�H�[�}�b�g���S�������œ���ȏ����������ꍇ�͒P���]�����s��
		if( TransColorAlphaTestFlag == FALSE &&
			AlphaMaskFlag           == FALSE &&
			AlphaOnlyFlag           == FALSE &&
			RedIsAlphaFlag          == FALSE &&
			DColor.FloatTypeFlag    == SColor.FloatTypeFlag &&
			DColor.ChannelBitDepth  == SColor.ChannelBitDepth &&
			DColor.ChannelNum       == SColor.ChannelNum &&
			DColor.PixelByte        == SColor.PixelByte )
		{
			BYTE *DestP, *SrcP ;
			DWORD SrcAddPitch, DestAddPitch ;
			DWORD DwMoveSetNum ;
			DWORD MoveLineByte ;

			// �P���C��������̓]���T�C�Y���Z�b�g
			MoveLineByte = ( DWORD )( BltWidth * DColor.PixelByte ) ;

			// �]�����A��A�h���X�Z�b�g
			DestP = ( BYTE * )DestGraphData + DestPoint.y  * DestPitch + DestPoint.x   * DColor.PixelByte ;
			SrcP  = ( BYTE * )SrcGraphData  + SrcRect->top * SrcPitch  + SrcRect->left * SColor.PixelByte ;

			// �P���C���]����A���̓]�����A�h���X�܂ł̃o�C�g���v�Z
			DestAddPitch = ( DWORD )( DestPitch - MoveLineByte ) ;
			SrcAddPitch  = ( DWORD )( SrcPitch  - MoveLineByte ) ;

			// �S�o�C�g�]�����񕪂��邩�Z�o
			DwMoveSetNum = MoveLineByte / 4 ;

			DWORD i ;
			do
			{
				i = DwMoveSetNum ;
				do
				{
					*( ( DWORD * )DestP ) = *( ( DWORD * )SrcP ) ;
					DestP += 4 ;
					SrcP  += 4 ;
				}while( -- i != 0 ) ;

				DestP += DestAddPitch ;
				SrcP  += SrcAddPitch ;
			}while( -- BltHeight != 0 ) ;
		}

		// �ᑬ�]�����s��
		goto NORMALMOVE ;
	}

	// ���������_�^���ǂ����ŏ����𕪊�
	if( FloatTypeFlag )
	{
		// �]�����Ɠ]����̃t�H�[�}�b�g���S�������œ���ȏ����������ꍇ�͒P���]�����s��
		if( TransColorAlphaTestFlag == FALSE &&
			AlphaMaskFlag           == FALSE &&
			AlphaOnlyFlag           == FALSE &&
			RedIsAlphaFlag          == FALSE &&
			DColor.FloatTypeFlag    == SColor.FloatTypeFlag &&
			DColor.ChannelBitDepth  == SColor.ChannelBitDepth &&
			DColor.ChannelNum       == SColor.ChannelNum &&
			DColor.PixelByte        == SColor.PixelByte )
		{
			BYTE *DestP, *SrcP ;
			DWORD SrcAddPitch, DestAddPitch ;
			DWORD DwMoveSetNum ;
			DWORD MoveLineByte ;

			// �P���C��������̓]���T�C�Y���Z�b�g
			MoveLineByte = ( DWORD )( BltWidth * DColor.PixelByte ) ;

			// �]�����A��A�h���X�Z�b�g
			DestP = ( BYTE * )DestGraphData + DestPoint.y  * DestPitch + DestPoint.x   * DColor.PixelByte ;
			SrcP  = ( BYTE * )SrcGraphData  + SrcRect->top * SrcPitch  + SrcRect->left * SColor.PixelByte ;

			// �P���C���]����A���̓]�����A�h���X�܂ł̃o�C�g���v�Z
			DestAddPitch = ( DWORD )( DestPitch - MoveLineByte ) ;
			SrcAddPitch  = ( DWORD )( SrcPitch  - MoveLineByte ) ;

			// �S�o�C�g�]�����񕪂��邩�Z�o
			DwMoveSetNum = MoveLineByte / 4 ;

			DWORD i ;
			do
			{
				i = DwMoveSetNum ;
				do
				{
					*( ( DWORD * )DestP ) = *( ( DWORD * )SrcP ) ;
					DestP += 4 ;
					SrcP  += 4 ;
				}while( -- i != 0 ) ;

				DestP += DestAddPitch ;
				SrcP  += SrcAddPitch ;
			}while( -- BltHeight != 0 ) ;
		}

		// �ᑬ�]�����s��
		goto NORMALMOVE ;
	}

	// �]���悪 4bit �̏ꍇ
	if( DestColorData->ColorBitDepth == 4 )
	{
		// �]������ 8bit �ɂ����Ή����Ȃ�
		if( SrcColorData->ColorBitDepth != 8 )
		{
			return -1 ;
		}

		// x�������̓]���ʒu���Q�̔{���ł͂Ȃ��ꍇ�̓G���[
		if( DestPoint.x % 2 != 0 )
		{
			return -1 ;
		}

		BYTE *DestP ;
		BYTE *SrcP ;
		int SrcAddPitch ;
		int DestAddPitch ;
		int SingleBltNum ;
		int DoubleBltNum ;
	
		DestP = (BYTE *)DestGraphData + DestPitch * DestPoint.y  + DestPoint.x / 2 ;
		SrcP  = (BYTE *)SrcGraphData  + SrcPitch  * SrcRect->top + SrcRect->left ;
		
		DestAddPitch = DestPitch - BltWidth / 2 ;
		SrcAddPitch  = SrcPitch  - BltWidth ;

		DoubleBltNum = BltWidth / 2 ;
		SingleBltNum = BltWidth - DoubleBltNum * 2 ;

		int DoubleBltNumTemp ;
		do
		{
			DoubleBltNumTemp = DoubleBltNum ;
			do
			{
				*DestP = SrcP[ 0 ] | ( SrcP[ 1 ] << 4 ) ;
				DestP ++ ;
				SrcP  += 2 ;
			}while( -- DoubleBltNumTemp != 0 ) ;

			if( SingleBltNum != 0 )
			{
				*DestP = SrcP[ 0 ] ;
				SrcP ++ ;
			}

			DestP += DestAddPitch ;
			SrcP  += SrcAddPitch ;
		}while( -- BltHeight != 0 ) ;

		return 0 ;
	}

	// �W�r�b�g�J���[���m�̃J���[�}�b�`�w��̏ꍇ�̓p���b�g����v���Ă��Ȃ��ꍇ�͒ᑬ�]���������s��
	if( Pal8ColorMatch && DestColorData->ColorBitDepth == 8 && SrcColorData->ColorBitDepth == 8 && ReverseFlag == FALSE )
	{
		if( _MEMCMP( DestColorData->Palette, SrcColorData->Palette, sizeof( SrcColorData->Palette ) ) != 0 )
		{
			int i, j ;
			BYTE *src, *dest ;
			const COLORPALETTEDATA *srcp ;

			for( i = 0 ; i < BltHeight ; i ++ )
			{
				src  = ( BYTE * )SrcGraphData  + SrcPitch  * ( SrcRect->top + i ) + SrcRect->left ;
				dest = ( BYTE * )DestGraphData + DestPitch * ( DestPoint.y + i )  + DestPoint.x ;
				for( j = 0 ; j < BltWidth ; j ++, src ++, dest ++ )
				{
					srcp = &SrcColorData->Palette[ *src ] ;
					*dest = ( BYTE )NS_GetColor3( DestColorData, srcp->Red, srcp->Green, srcp->Blue, 0 ) ;
				}
			}
			return 0 ;
		}
	}

	// �]���悪�W�r�b�g�ŁA�]�������W�r�b�g�ȊO�������ꍇ�͒ᑬ�]��
	if( DestColorData->ColorBitDepth == 8 && SrcColorData->ColorBitDepth != 8 && RedIsAlphaFlag == FALSE && ReverseFlag == FALSE )
	{
		int i, j ;
		BYTE *src, *dest ;
		unsigned int Color ;

		switch( SrcColorData->ColorBitDepth )
		{
		case 16 :
			for( i = 0 ; i < BltHeight ; i ++ )
			{
				src  = ( BYTE * )SrcGraphData  + SrcPitch  * ( SrcRect->top + i ) + SrcRect->left * 2 ;
				dest = ( BYTE * )DestGraphData + DestPitch * ( DestPoint.y + i )  + DestPoint.x ;
				for( j = 0 ; j < BltWidth ; j ++, src += 2, dest ++ )
				{
					*dest = ( BYTE )NS_GetColor3( DestColorData,
						( int )( ( *( ( WORD * )src ) & SrcColorData->RedMask   ) >> SrcColorData->RedLoc   ),
						( int )( ( *( ( WORD * )src ) & SrcColorData->GreenMask ) >> SrcColorData->GreenLoc ),
						( int )( ( *( ( WORD * )src ) & SrcColorData->BlueMask  ) >> SrcColorData->BlueLoc  ),
						( int )( ( *( ( WORD * )src ) & SrcColorData->AlphaMask ) >> SrcColorData->AlphaLoc ) ) ;
				}
			}
			break ;

		case 24 :
			for( i = 0 ; i < BltHeight ; i ++ )
			{
				src  = ( BYTE * )SrcGraphData  + SrcPitch  * ( SrcRect->top + i ) + SrcRect->left * 2 ;
				dest = ( BYTE * )DestGraphData + DestPitch * ( DestPoint.y + i )  + DestPoint.x ;
				for( j = 0 ; j < BltWidth ; j ++, src += 3, dest ++ )
				{
					Color = ( DWORD )( *( ( WORD * )src ) | ( src[ 2 ] << 16 ) ) ;
					*dest = ( BYTE )NS_GetColor3( DestColorData,
						( int )( ( Color & SrcColorData->RedMask   ) >> SrcColorData->RedLoc   ),
						( int )( ( Color & SrcColorData->GreenMask ) >> SrcColorData->GreenLoc ),
						( int )( ( Color & SrcColorData->BlueMask  ) >> SrcColorData->BlueLoc  ),
						( int )( ( Color & SrcColorData->AlphaMask ) >> SrcColorData->AlphaLoc ) ) ;
				}
			}
			break ;

		case 32 :
			for( i = 0 ; i < BltHeight ; i ++ )
			{
				src  = ( BYTE * )SrcGraphData  + SrcPitch  * ( SrcRect->top + i ) + SrcRect->left * 4 ;
				dest = ( BYTE * )DestGraphData + DestPitch * ( DestPoint.y + i )  + DestPoint.x ;
				for( j = 0 ; j < BltWidth ; j ++, src += 4, dest ++ )
				{
					*dest = ( BYTE )NS_GetColor3( DestColorData,
						( int )( ( *( ( DWORD * )src ) & SrcColorData->RedMask   ) >> SrcColorData->RedLoc   ),
						( int )( ( *( ( DWORD * )src ) & SrcColorData->GreenMask ) >> SrcColorData->GreenLoc ),
						( int )( ( *( ( DWORD * )src ) & SrcColorData->BlueMask  ) >> SrcColorData->BlueLoc  ),
						( int )( ( *( ( DWORD * )src ) & SrcColorData->AlphaMask ) >> SrcColorData->AlphaLoc ) ) ;
				}
			}
			break ;
		}
		return 0 ;
	}

	// �]���悪�W�r�b�g�������珈���𕪊�
	if( DestColorData->ColorBitDepth == 8 && AlphaOnlyFlag == FALSE && TransColorNoMoveFlag == FALSE && RedIsAlphaFlag == FALSE && ReverseFlag == FALSE )
	{
		BYTE *DestP, *SrcP ;
		int SrcAddPitch, DestAddPitch ;
		DWORD MaxColorNumber ;

		// �]�������W�r�b�g�ȊO��������G���[
		if( SrcColorData->ColorBitDepth != 8 ) return -1 ;
	
		// �J���[�}�b�`���O�͍s�킸�A�����]�����邾��
		DestP = (BYTE *)DestGraphData + DestPitch * DestPoint.y + DestPoint.x ;
		SrcP = (BYTE *)SrcGraphData + SrcPitch * SrcRect->top + SrcRect->left ;
		
		SrcAddPitch = SrcPitch - BltWidth ;
		DestAddPitch = DestPitch - BltWidth ;
#ifdef DX_NON_INLINE_ASM
		int BltWidthTemp ;
		MaxColorNumber = 0 ;
		do
		{
			BltWidthTemp = BltWidth ;
			do
			{
				if( MaxColorNumber < *SrcP )
					MaxColorNumber = *SrcP ;
				*DestP ++ = *SrcP ++ ;
			}while( -- BltWidthTemp != 0 ) ;
			DestP += DestAddPitch ;
			SrcP += SrcAddPitch ;
		}while( -- BltHeight != 0 ) ;
#else
		_asm
		{
			PUSHF
			CLD
			MOV EDI, DestP
			MOV ESI, SrcP
			MOV EBX, BltHeight
			MOV EAX, 0
		LABEL_PAL_IMAGE_MOVE_1:
			MOV ECX, BltWidth
		LABEL_PAL_IMAGE_MOVE_2:
			MOVZX EDX, [ESI]
			MOV [EDI], DL
			CMP EDX, EAX
			JNA LABEL_PAL_IMAGE_MOVE_3
			MOV EAX, EDX
		LABEL_PAL_IMAGE_MOVE_3:
			INC ESI
			INC EDI
			DEC ECX
			JNZ LABEL_PAL_IMAGE_MOVE_2

			ADD EDI, DestAddPitch
			ADD ESI, SrcAddPitch
			DEC EBX
			JNZ LABEL_PAL_IMAGE_MOVE_1
			MOV MaxColorNumber, EAX
			POPF
		}
#endif
		// �g�p���Ă���F����Ԃ�
		return ( int )MaxColorNumber ;
	}
	

	// ���ߐF�A���t�@�e�X�g�t���O���|��Ă��ăs�N�Z���t�H�[�}�b�g������
	// �������ꍇ�͍����]���v���O�����𑖂点��
	if( TransColorAlphaTestFlag == FALSE &&
		AlphaMaskFlag           == FALSE &&
		AlphaOnlyFlag           == FALSE &&
		RedIsAlphaFlag          == FALSE &&
		DColor.ChannelNum       == 0                    &&
		SColor.ChannelNum       == 0                    &&
		DColor.ColorBitDepth    == SColor.ColorBitDepth &&
		DColor.RedMask          == SColor.RedMask       &&
		DColor.GreenMask        == SColor.GreenMask     &&
		DColor.BlueMask         == SColor.BlueMask      &&
		DColor.AlphaMask        == SColor.AlphaMask )
	{
		// �����]�������v���O����

		BYTE *DestP, *SrcP ;
		int SrcAddPitch, DestAddPitch ;
		int DwMoveSetNum, NokoriMoveSetNum ;
		int MoveLineByte, ColorBitDepth ;

		// �P���C��������̓]���T�C�Y���Z�b�g
		MoveLineByte = BltWidth * DColor.PixelByte ;
		
		// ���]�]�����ǂ����ŏ����𕪊�
		if( ReverseFlag == FALSE )
		{
			// �]�����A��A�h���X�Z�b�g
			DestP = ( BYTE * )DestGraphData + DestPoint.y  * DestPitch + DestPoint.x   * DColor.PixelByte ;
			SrcP  = ( BYTE * )SrcGraphData  + SrcRect->top * SrcPitch  + SrcRect->left * SColor.PixelByte ;

			// �P���C���]����A���̓]�����A�h���X�܂ł̃o�C�g���v�Z
			DestAddPitch = DestPitch - MoveLineByte ;
			SrcAddPitch  = SrcPitch  - MoveLineByte ;

			// ���ߐF�������s�����ǂ����ŏ����𕪊�
			if( TransColorNoMoveFlag == TRUE )
			{
				// ���ߐF�������s���ꍇ

				// �F�r�b�g�����Z�b�g
				ColorBitDepth = DColor.ColorBitDepth ;

#ifdef DX_NON_INLINE_ASM
				int i ;
				switch( ColorBitDepth )
				{
				case 32 :
					TransColor &= 0xffff ;
					do
					{
						i = BltWidth ;
						do
						{
							if( *( ( DWORD * )SrcP ) != TransColor )
							{
								*( ( DWORD * )DestP ) = *( ( DWORD * )SrcP ) ;
							}
							DestP += 4 ;
							SrcP += 4 ;
						}while( -- i != 0 ) ;
						DestP += DestAddPitch ;
						SrcP += SrcAddPitch ;
					}while( -- BltHeight != 0 ) ;
					break ;

				case 24 :
					DWORD SrcC ;
					TransColor &= 0xffff ;
					do
					{
						i = BltWidth ;
						do
						{
							SrcC = ( DWORD )( *( ( WORD * )SrcP ) | ( SrcP[ 2 ] << 16 ) ) ;
							if( SrcC != TransColor )
							{
								*( ( WORD * )DestP ) = *( ( WORD * )SrcP ) ;
								DestP[ 2 ] = SrcP[ 2 ] ;
							}
							DestP += 3 ;
							SrcP += 3 ;
						}while( -- i != 0 ) ;
						DestP += DestAddPitch ;
						SrcP += SrcAddPitch ;
					}while( -- BltHeight != 0 ) ;
					break ;

				case 16 :
					TransColor &= 0xffff ;
					do
					{
						i = BltWidth ;
						do
						{
							if( *( ( WORD * )SrcP ) != TransColor )
							{
								*( ( WORD * )DestP ) = *( ( WORD * )SrcP ) ;
							}
							DestP += 2 ;
							SrcP += 2 ;
						}while( -- i != 0 ) ;
						DestP += DestAddPitch ;
						SrcP += SrcAddPitch ;
					}while( -- BltHeight != 0 ) ;
					break ;

				case 8 :
					TransColor &= 0xff ;
					do
					{
						i = BltWidth ;
						do
						{
							if( *SrcP != TransColor )
							{
								*DestP = *SrcP ;
							}
							DestP ++ ;
							SrcP ++ ;
						}while( -- i != 0 ) ;
						DestP += DestAddPitch ;
						SrcP += SrcAddPitch ;
					}while( -- BltHeight != 0 ) ;
					break ;
				}
#else
				// �]������
				_asm{
					PUSHA
					PUSHF
					CLD
					MOV		EDI, DestP
					MOV		ESI, SrcP
					MOV		EBX, TransColor
					MOV		EDX, BltHeight
					CMP		ColorBitDepth, 8
					JZ		CR8_a_TR
					CMP		ColorBitDepth, 16
					JZ		CR16_a_TR
					CMP		ColorBitDepth, 24
					JZ		CR24_a_TR


				CR32_a_TR:
					MOV		ECX, BltWidth
				CR32_b_TR:
					MOV		EAX, [ESI]
					CMP		EAX, EBX
					JE		CR32_c_TR
					MOV		[EDI], EAX
				CR32_c_TR:
					ADD		ESI, 4
					ADD		EDI, 4
					DEC		ECX
					JNZ		CR32_b_TR
					ADD		EDI, DestAddPitch
					ADD		ESI, SrcAddPitch
					DEC		EDX
					JNZ		CR32_a_TR
					JMP		END_a_TR
					

				CR24_a_TR:
					MOV		ECX, BltWidth
					AND		EBX, 0xffffff
				CR24_b_TR:
					XOR		EAX, EAX
					MOV		AL, [ESI+2]
					SHL		EAX, 16
					MOV		AX, [ESI]
					CMP		EAX, EBX
					JE		CR24_c_TR
					MOV		[EDI], AX
					SHR		EAX, 16
					MOV		[EDI+2], AL
				CR24_c_TR:
					ADD		ESI, 3
					ADD		EDI, 3
					DEC		ECX
					JNZ		CR24_b_TR
					ADD		EDI, DestAddPitch
					ADD		ESI, SrcAddPitch
					DEC		EDX
					JNZ		CR24_a_TR
					JMP		END_a_TR

					

				CR16_a_TR:
					MOV		ECX, BltWidth
				CR16_b_TR:
					MOV		AX, [ESI]
					CMP		AX, BX
					JE		CR16_c_TR
					MOV		[EDI], AX
				CR16_c_TR:
					ADD		ESI, 2
					ADD		EDI, 2
					DEC		ECX
					JNZ		CR16_b_TR
					ADD		EDI, DestAddPitch
					ADD		ESI, SrcAddPitch
					DEC		EDX
					JNZ		CR16_a_TR
					JMP		END_a_TR


				CR8_a_TR:
					MOV		ECX, BltWidth
				CR8_b_TR:
					MOV		AL, [ESI]
					CMP		AL, BL
					JE		CR8_c_TR
					MOV		[EDI], AL
				CR8_c_TR:
					INC		ESI
					INC		EDI
					DEC		ECX
					JNZ		CR8_b_TR
					ADD		EDI, DestAddPitch
					ADD		ESI, SrcAddPitch
					DEC		EDX
					JNZ		CR8_a_TR
					JMP		END_a_TR

				END_a_TR:
					POPF
					POPA
				}
#endif
			}
			else
			{
				// �S�o�C�g�]������A���̌゠�܂�]���������s�N�Z�������邩�Z�o
				DwMoveSetNum = MoveLineByte / 4 ;
				NokoriMoveSetNum = ( MoveLineByte - DwMoveSetNum * 4 ) / DColor.PixelByte ;

				// �F�r�b�g�����Z�b�g
				ColorBitDepth = DColor.ColorBitDepth ;

#ifdef DX_NON_INLINE_ASM
				int i ;
				switch( ColorBitDepth )
				{
				case 32 :
					{
						DWORD Dw8MoveSetNum ;

						Dw8MoveSetNum  = DwMoveSetNum  / 16 ;
						DwMoveSetNum  -= Dw8MoveSetNum * 16 ;
						do
						{
							if( Dw8MoveSetNum > 0 )
							{
								i = Dw8MoveSetNum ;
								do
								{
									( ( ULONGLONG * )DestP )[ 0 ] = ( ( ULONGLONG * )SrcP )[ 0 ] ;
									( ( ULONGLONG * )DestP )[ 1 ] = ( ( ULONGLONG * )SrcP )[ 1 ] ;
									( ( ULONGLONG * )DestP )[ 2 ] = ( ( ULONGLONG * )SrcP )[ 2 ] ;
									( ( ULONGLONG * )DestP )[ 3 ] = ( ( ULONGLONG * )SrcP )[ 3 ] ;
									( ( ULONGLONG * )DestP )[ 4 ] = ( ( ULONGLONG * )SrcP )[ 4 ] ;
									( ( ULONGLONG * )DestP )[ 5 ] = ( ( ULONGLONG * )SrcP )[ 5 ] ;
									( ( ULONGLONG * )DestP )[ 6 ] = ( ( ULONGLONG * )SrcP )[ 6 ] ;
									( ( ULONGLONG * )DestP )[ 7 ] = ( ( ULONGLONG * )SrcP )[ 7 ] ;
									DestP += 64 ;
									SrcP  += 64 ;
								}while( -- i != 0 ) ;
							}

							if( DwMoveSetNum > 0 )
							{
								i = DwMoveSetNum ;
								do
								{
									*( ( DWORD * )DestP ) = *( ( DWORD * )SrcP ) ;
									DestP += 4 ;
									SrcP += 4 ;
								}while( -- i != 0 ) ;
							}

							DestP += DestAddPitch ;
							SrcP += SrcAddPitch ;
						}while( -- BltHeight != 0 ) ;
					}
					break ;

				case 24 :
					do
					{
						if( DwMoveSetNum != 0 )
						{
							i = DwMoveSetNum ;
							do
							{
								*( ( DWORD * )DestP ) = *( ( DWORD * )SrcP ) ;
								DestP += 4 ;
								SrcP += 4 ;
							}while( -- i != 0 ) ;
						}

						if( NokoriMoveSetNum != 0 )
						{
							i = NokoriMoveSetNum ;
							do
							{
								*( ( WORD * )DestP ) = *( ( WORD * )SrcP ) ;
								DestP[ 2 ] = SrcP[ 2 ] ;
								DestP += 3 ;
								SrcP += 3 ;
							}while( -- i != 0 ) ;
						}
						DestP += DestAddPitch ;
						SrcP += SrcAddPitch ;
					}while( -- BltHeight != 0 ) ;
					break ;

				case 16 :
					do
					{
						if( DwMoveSetNum != 0 )
						{
							i = DwMoveSetNum ;
							do
							{
								*( ( DWORD * )DestP ) = *( ( DWORD * )SrcP ) ;
								DestP += 4 ;
								SrcP += 4 ;
							}while( -- i != 0 ) ;
						}

						if( NokoriMoveSetNum != 0 )
						{
							i = NokoriMoveSetNum ;
							do
							{
								*( ( WORD * )DestP ) = *( ( WORD * )SrcP ) ;
								DestP += 2 ;
								SrcP += 2 ;
							}while( -- i != 0 ) ;
						}
						DestP += DestAddPitch ;
						SrcP += SrcAddPitch ;
					}while( -- BltHeight != 0 ) ;
					break ;

				case 8 :
					do
					{
						if( DwMoveSetNum != 0 )
						{
							i = DwMoveSetNum ;
							do
							{
								*( ( DWORD * )DestP ) = *( ( DWORD * )SrcP ) ;
								DestP += 4 ;
								SrcP += 4 ;
							}while( -- i != 0 ) ;
						}

						if( NokoriMoveSetNum != 0 )
						{
							i = NokoriMoveSetNum ;
							do
							{
								*DestP = *SrcP ;
								DestP ++ ;
								SrcP ++ ;
							}while( -- i != 0 ) ;
						}
						DestP += DestAddPitch ;
						SrcP += SrcAddPitch ;
					}while( -- BltHeight != 0 ) ;
					break ;
				}
#else
				// �]������
				_asm{
					PUSHA
					PUSHF
					CLD
					MOV		EDI, DestP
					MOV		ESI, SrcP
					MOV		EAX, DestAddPitch
					MOV		EBX, SrcAddPitch
					MOV		EDX, BltHeight
					CMP		ColorBitDepth, 8
					JZ		CR8_a
					CMP		ColorBitDepth, 16
					JZ		CR16_a
					CMP		ColorBitDepth, 24
					JZ		CR24_a


				CR32_a:
					MOV		ECX, DwMoveSetNum
					REP		MOVSD
					ADD		EDI, EAX
					ADD		ESI, EBX
					DEC		EDX
					JNZ		CR32_a
					JMP		END_a
					
					
				CR24_a:
					MOV		ECX, DwMoveSetNum
					TEST	ECX, 0xffffffff
					JZ		R241_a
					REP		MOVSD
				R241_a:
					MOV		ECX, NokoriMoveSetNum
					TEST	ECX, 0xffffffff
					JZ		R242_a
				R243_a:
					MOVSW
					MOVSB
					LOOP 	R243_a
				R242_a:
					ADD		EDI, EAX
					ADD		ESI, EBX
					DEC		EDX
					JNZ		CR24_a
					JMP		END_a

					

				CR16_a:
					MOV		ECX, DwMoveSetNum
					TEST	ECX, 0xffffffff
					JZ		R161_a
					REP		MOVSD
				R161_a:
					MOV		ECX, NokoriMoveSetNum
					TEST	ECX, 0xffffffff
					JZ		R162_a
					REP		MOVSW
				R162_a:
					ADD		EDI, EAX
					ADD		ESI, EBX
					DEC		EDX
					JNZ		CR16_a
					JMP		END_a


				CR8_a:
					MOV		ECX, DwMoveSetNum
					TEST	ECX, 0xffffffff
					JZ		R81_a
					REP		MOVSD
				R81_a:
					MOV		ECX, NokoriMoveSetNum
					TEST	ECX, 0xffffffff
					JZ		R82_a
					REP		MOVSB
				R82_a:
					ADD		EDI, EAX
					ADD		ESI, EBX
					DEC		EDX
					JNZ		CR8_a
					JMP		END_a

				END_a:
					POPF
					POPA
				}
#endif
			}
		}
		else
		{
			// ���]�]��

			// �]�����A��A�h���X�Z�b�g
			DestP = ( BYTE * )DestGraphData + DestPoint.y  * DestPitch + ( ( BltWidth - 1 ) + DestPoint.x ) * DColor.PixelByte ;
			SrcP  = ( BYTE * )SrcGraphData  + SrcRect->top * SrcPitch  + SrcRect->left                      * SColor.PixelByte ;

			// �P���C���]����A���̓]�����A�h���X�܂ł̃o�C�g���v�Z
			DestAddPitch = DestPitch + MoveLineByte ;
			SrcAddPitch = SrcPitch - MoveLineByte ;

			// �F�r�b�g�����Z�b�g
			ColorBitDepth = DColor.ColorBitDepth ;

#ifdef DX_NON_INLINE_ASM
			int i ;
			switch( ColorBitDepth )
			{
			case 32 :
				do
				{
					i = BltWidth ;
					do
					{
						*( ( DWORD * )DestP ) = *( ( DWORD * )SrcP ) ;
						DestP -= 4 ;
						SrcP += 4 ;
					}while( -- i != 0 ) ;
					DestP += DestAddPitch ;
					SrcP += SrcAddPitch ;
				}while( -- BltHeight != 0 ) ;
				break ;

			case 24 :
				do
				{
					i = BltWidth ;
					do
					{
						*( ( WORD * )DestP ) = *( ( WORD * )SrcP ) ;
						DestP[ 2 ] = SrcP[ 2 ] ;
						DestP -= 3 ;
						SrcP += 3 ;
					}while( -- i != 0 ) ;
					DestP += DestAddPitch ;
					SrcP += SrcAddPitch ;
				}while( -- BltHeight != 0 ) ;
				break ;

			case 16 :
				do
				{
					i = BltWidth ;
					do
					{
						*( ( WORD * )DestP ) = *( ( WORD * )SrcP ) ;
						DestP -= 2 ;
						SrcP += 2 ;
					}while( -- i != 0 ) ;
					DestP += DestAddPitch ;
					SrcP += SrcAddPitch ;
				}while( -- BltHeight != 0 ) ;
				break ;

			case 8 :
				do
				{
					i = BltWidth ;
					do
					{
						*DestP = *SrcP ;
						DestP -- ;
						SrcP ++ ;
					}while( -- i != 0 ) ;
					DestP += DestAddPitch ;
					SrcP += SrcAddPitch ;
				}while( -- BltHeight != 0 ) ;
				break ;
			}
#else
			// �]������
			_asm{
				PUSHA
				MOV		EDI, DestP
				MOV		ESI, SrcP
				MOV		EAX, DestAddPitch
				MOV		EBX, SrcAddPitch
				MOV		EDX, BltHeight
				CMP		ColorBitDepth, 8
				JZ		CR8R
				CMP		ColorBitDepth, 16
				JZ		CR16R
				CMP		ColorBitDepth, 24
				JZ		CR24R

			CR32R:
				MOV		ECX, BltWidth
			R321R:
				MOVSD
				SUB		EDI, 8
				LOOP	R321R
				ADD		EDI, EAX
				ADD		ESI, EBX
				DEC		EDX
				JNZ		CR32R
				JMP		ENDR
				
				
			CR24R:
				MOV		ECX, BltWidth
			R241R:
				MOVSW
				MOVSB
				SUB		EDI, 6
				LOOP 	R241R
				ADD		EDI, EAX
				ADD		ESI, EBX
				DEC		EDX
				JNZ		CR24R
				JMP		ENDR
				

			CR16R:
				MOV		ECX, BltWidth
			R161R:
				MOVSW
				SUB		EDI, 4
				LOOP	R161R
				ADD		EDI, EAX
				ADD		ESI, EBX
				DEC		EDX
				JNZ		CR16R
				JMP		ENDR


			CR8R:
				MOV		ECX, BltWidth
			R81R:
				MOVSB
				SUB		EDI, 2
				LOOP	R81R
				ADD		EDI, EAX
				ADD		ESI, EBX
				DEC		EDX
				JNZ		CR8R
				JMP		ENDR;

			ENDR:
				POPA
			}
#endif
		}
		// �I��
		return 0 ;
	}
	else
	// ���ߐF�������s���ꍇ
	if( TransColorNoMoveFlag == TRUE )
	{
		// Src�� XRGB32 Dest �� ARGB32 �̏ꍇ�͂�����ƍ����ȓ]��
		if( AlphaOnlyFlag == FALSE &&
			AlphaMaskFlag == FALSE &&
			AlphaFlag == FALSE &&
			RedIsAlphaFlag == FALSE &&
			TransColorAlphaTestFlag == FALSE &&
			ReverseFlag == FALSE &&
			SColor.ColorBitDepth == 32 &&
			DColor.ColorBitDepth == 32 &&
			SColor.RedMask   == 0x00ff0000 &&
			SColor.GreenMask == 0x0000ff00 &&
			SColor.BlueMask  == 0x000000ff &&
			SColor.AlphaMask == 0x00000000 &&
			DColor.RedMask   == 0x00ff0000 &&
			DColor.GreenMask == 0x0000ff00 &&
			DColor.BlueMask  == 0x000000ff &&
			DColor.AlphaMask == 0xff000000 )
		{
			DWORD TColor ;
			BYTE *DBuf, *SBuf ;
			DWORD DestPitch2, SrcPitch2 ;
			DWORD i, j ;

			TColor = TransColor & 0x00ffffff;

			// �e�o�b�t�@�ւ̃A�h���X���Z�b�g
			DBuf = ( BYTE * )DestGraphData + DestPoint.y  * DestPitch + DestPoint.x   * DColor.PixelByte ;
			SBuf = ( BYTE * )SrcGraphData  + SrcRect->top * SrcPitch  + SrcRect->left * SColor.PixelByte ;

			// �P���C���]������x�ɉ��Z����l�Z�b�g
			DestPitch2 = ( DWORD )( DestPitch - BltWidth * DColor.PixelByte ) ;
			SrcPitch2  = ( DWORD )( SrcPitch  - BltWidth * SColor.PixelByte ) ;

			// �]��
			if( BltHeight != 0 && BltWidth != 0 )
			{
				i = ( DWORD )BltHeight ;
				do
				{
					j = ( DWORD )BltWidth ;
					do
					{
						if( ( *( ( DWORD * )SBuf ) & 0x00ffffff ) != TColor )
						{
							*( ( DWORD * )DBuf ) = *( ( DWORD * )SBuf ) | 0xff000000 ;
						}
						DBuf += 4 ;
						SBuf += 4 ;
					}while( -- j ) ;

					DBuf += DestPitch2 ;
					SBuf += SrcPitch2 ;
				}while( -- i ) ;
			}

			// �I��
			return 0 ;
		}

		goto NORMALMOVE ;
	}
	else
	// �A���t�@�`�����l���𓧉ߐF�e�X�g�A���t�@������A����ɂ���ȊO�̃t�H�[�}�b�g�������̏ꍇ��
	// ���������ȓ]���������s��
	if( ( DColor.AlphaWidth != 0 && TransColorAlphaTestFlag == TRUE ) && 
		AlphaOnlyFlag == FALSE && RedIsAlphaFlag == FALSE &&
		DColor.RedMask       == SColor.RedMask &&
		DColor.GreenMask     == SColor.GreenMask &&
		DColor.BlueMask      == SColor.BlueMask &&
		DColor.ColorBitDepth == SColor.ColorBitDepth )
	{
		DWORD TColor, NRGBMask ;
		BYTE *DBuf, *SBuf ;
		DWORD DestPitch2, SrcPitch2 ;
		int DestAdd ;

		// ���ߐF�R�[�h�𓾂�
		TColor = NS_GetColor3( &DColor,
									( int )( ( TransColor & 0xff0000 ) >> 16 ),
									( int )( ( TransColor & 0x00ff00 ) >>  8 ),
									( int )( ( TransColor & 0x0000ff )       ) ) ;

		// �q�f�a�̔��]�}�X�N���쐬����
		NRGBMask = ~( DColor.RedMask | DColor.GreenMask | DColor.BlueMask ) ;

		// ���]�]�����ǂ����ŏ����𕪊�
		if( ReverseFlag == TRUE )
		{
			DestPitch2 = ( DWORD )( DestPitch + BltWidth * DColor.PixelByte ) ;
			DestAdd    = -DColor.PixelByte ;
			DBuf       = ( BYTE * )DestGraphData + DestPoint.y * DestPitch + DestPoint.x * DColor.PixelByte + ( BltWidth - 1 ) * DColor.PixelByte ;
		}
		else
		{
			DestPitch2 = ( DWORD )( DestPitch - BltWidth * DColor.PixelByte ) ;
			DestAdd    = DColor.PixelByte ;
			DBuf       = ( BYTE * )DestGraphData + DestPoint.y * DestPitch + DestPoint.x * DColor.PixelByte ;
		}

		// �e�o�b�t�@�ւ̃A�h���X���Z�b�g
		SBuf = ( BYTE * )SrcGraphData + SrcRect->top * SrcPitch + SrcRect->left * SColor.PixelByte ;

		// �P���C���]������x�ɉ��Z����l�Z�b�g
		SrcPitch2 = ( DWORD )( SrcPitch - BltWidth * SColor.PixelByte ) ;
		
		// �J���[�r�b�g���ɂ���ď����𕪊�
		switch( DColor.ColorBitDepth )
		{
		case 8 :		// �W�r�b�g�J���[
			// �]������
			{
				BYTE Color, Trans ;
				int i, j ;

				Trans = ( BYTE )TColor ;

				for( i = 0 ; i < BltHeight ; i ++, DBuf += DestAdd, SBuf += SrcPitch2 )
				{
					for( j = 0 ; j < BltWidth ; j ++, DBuf += DestAdd, SBuf += SColor.PixelByte )
					{
						Color = *( ( BYTE * )SBuf ) ;
						if( Trans == Color )	Color &= ~DColor.AlphaMask ;
						else					Color |= DColor.AlphaMask ;

						*( ( BYTE * )DBuf ) = Color ; 
					}
				}
			}
			break ;

		case 16 :		// �P�U�r�b�g�J���[
			// �]������
			{
				WORD Color, Trans ;
				int i, j ;

				Trans = ( WORD )TColor ;

				for( i = 0 ; i < BltHeight ; i ++, DBuf += DestPitch2, SBuf += SrcPitch2 )
				{
					for( j = 0 ; j < BltWidth ; j ++, DBuf += DestAdd, SBuf += SColor.PixelByte )
					{
						Color = *( ( WORD * )SBuf ) ;
						if( Trans == Color )	Color &= ~DColor.AlphaMask ;
						else					Color |= DColor.AlphaMask ;

						*( ( WORD * )DBuf ) = Color ; 
					}
				}
			}
			break ;
			
		case 24 :		// �Q�S�r�b�g�J���[
			// �]������
			{
				unsigned int Color, Trans ;
				int i, j ;

				Trans = TColor ;

				for( i = 0 ; i < BltHeight ; i ++, DBuf += DestPitch2, SBuf += SrcPitch2 )
				{
					for( j = 0 ; j < BltWidth ; j ++, DBuf += DestAdd, SBuf += SColor.PixelByte )
					{
						Color = ( DWORD )( ( *( ( WORD * )SBuf ) + ( *( ( BYTE * )( SBuf + 2 ) ) << 16 ) ) ) ;
						if( Trans == Color )	Color &= ~DColor.AlphaMask ;
						else					Color |= DColor.AlphaMask ;

						*( ( WORD * )DBuf ) 		= ( WORD )( Color & 0xffff ) ;
						*( ( BYTE * )( DBuf + 2 ) )	= ( BYTE )( ( Color >> 16 ) & 0xff ) ; 
					}
				}
			}
			break ;

		case 32 :		// �R�Q�r�b�g�J���[
			// �]������
			{
				unsigned int Color, Trans, NRgbMask ;
				int i, j ;

				Trans = TColor ;
				NRgbMask = NRGBMask ;

				for( i = 0 ; i < BltHeight ; i ++, DBuf += DestPitch2, SBuf += SrcPitch2 )
				{
					for( j = 0 ; j < BltWidth ; j ++, DBuf += DestAdd, SBuf += SColor.PixelByte )
					{
						Color = *( ( DWORD * )SBuf ) | NRgbMask ;
//						Color = *( ( DWORD * )SBuf ) ;
						if( Trans == Color )	Color &= ~DColor.AlphaMask ;
						else					Color |= DColor.AlphaMask ;

						*( ( DWORD * )DBuf ) = Color ; 
					}
				}
			}
			break ;
		}

		// �I��
		return 0 ;
	}

	// �ǂ���������ł̂ݏ��������ȓ]���������s��
	if( 
		ImageShavedMode == DX_SHAVEDMODE_NONE &&
		AlphaOnlyFlag   == FALSE &&
		RedIsAlphaFlag  == FALSE &&
		ReverseFlag     == FALSE &&
		AlphaMask       == NULL &&
		ReverseFlag     == FALSE )
	{
		if( TransColorAlphaTestFlag == FALSE &&
			SColor.ChannelNum      == 1 &&
			SColor.ChannelBitDepth == 8 )
		{
			BYTE *Dest ;
			BYTE *Src ;
			DWORD BlockNum ;
			DWORD OneNum ;
			DWORD w ;
			int DestAddPitch, SrcAddPitch ;

			DestAddPitch = DestPitch - BltWidth * DColor.PixelByte ;
			SrcAddPitch  = SrcPitch  - BltWidth * SColor.PixelByte ;

			Dest = ( BYTE * )DestGraphData + DestPoint.y  * DestPitch + DestPoint.x   * DColor.PixelByte ;
			Src  = ( BYTE * )SrcGraphData  + SrcRect->top * SrcPitch  + SrcRect->left * SColor.PixelByte ;

			if( DColor.ChannelNum      == 1 &&
				DColor.ChannelBitDepth == 8 )
			{
				BlockNum = BltWidth / 8 ;
				OneNum   = BltWidth - BlockNum * 8 ;

				do
				{
					if( BlockNum > 0 )
					{
						w = BlockNum ;
						do
						{
							( ( DWORD * )Dest )[ 0 ] = ( ( DWORD * )Src )[ 0 ] ;
							( ( DWORD * )Dest )[ 1 ] = ( ( DWORD * )Src )[ 1 ] ;
							Dest += 8 ;
							Src  += 8 ;
						}while( -- w ) ;
					}
					if( OneNum > 0 )
					{
						w = OneNum ;
						do
						{
							*Dest = *Src ;
							Dest ++ ;
							Src  ++ ;
						}while( -- w ) ;
					}
					Dest += DestAddPitch ;
					Src  += SrcAddPitch ;
				}while( -- BltHeight ) ;

				return 0 ;
			}
			else
			if( DColor.ChannelNum      == 2 &&
				DColor.ChannelBitDepth == 8 )
			{
				BlockNum = BltWidth / 8 ;
				OneNum   = BltWidth - BlockNum * 8 ;

				do
				{
					if( BlockNum > 0 )
					{
						w = BlockNum ;
						do
						{
							Dest[  0 ] = Src[ 0 ] ; Dest[  1 ] = Src[ 0 ] ;
							Dest[  2 ] = Src[ 1 ] ; Dest[  3 ] = Src[ 1 ] ;
							Dest[  4 ] = Src[ 2 ] ; Dest[  5 ] = Src[ 2 ] ;
							Dest[  6 ] = Src[ 3 ] ; Dest[  7 ] = Src[ 3 ] ;
							Dest[  8 ] = Src[ 4 ] ; Dest[  9 ] = Src[ 4 ] ;
							Dest[ 10 ] = Src[ 5 ] ; Dest[ 11 ] = Src[ 5 ] ;
							Dest[ 12 ] = Src[ 6 ] ; Dest[ 13 ] = Src[ 6 ] ;
							Dest[ 14 ] = Src[ 7 ] ; Dest[ 15 ] = Src[ 7 ] ;
							Dest += 16 ;
							Src  += 8 ;
						}while( -- w ) ;
					}
					if( OneNum > 0 )
					{
						w = OneNum ;
						do
						{
							Dest[ 0 ] = Src[ 0 ] ;
							Dest[ 1 ] = 0 ;
							Dest += 2 ;
							Src  ++ ;
						}while( -- w ) ;
					}
					Dest += DestAddPitch ;
					Src  += SrcAddPitch ;
				}while( -- BltHeight ) ;

				return 0 ;
			}
			else
			if( ( DColor.ColorBitDepth == 32 &&
				  // DColor.AlphaMask	== 0xff000000 &&
				  DColor.RedMask 	== 0x00ff0000 &&
				  DColor.GreenMask 	== 0x0000ff00 &&
				  DColor.BlueMask 	== 0x000000ff ) ||
				( DColor.ColorBitDepth == 32 &&
				  // DColor.AlphaMask	== 0xff000000 &&
				  DColor.RedMask 	== 0x000000ff &&
				  DColor.GreenMask 	== 0x0000ff00 &&
				  DColor.BlueMask 	== 0x00ff0000 ) )
			{
				BlockNum = BltWidth / 8 ;
				OneNum   = BltWidth - BlockNum * 8 ;

				do
				{
					if( BlockNum > 0 )
					{
						w = BlockNum ;
						do
						{
							Dest[  0 ] = Src[ 0 ] ; Dest[  1 ] = Src[ 0 ] ; Dest[  2 ] = Src[ 0 ] ; Dest[  3 ] = Src[ 0 ] ;
							Dest[  4 ] = Src[ 1 ] ; Dest[  5 ] = Src[ 1 ] ; Dest[  6 ] = Src[ 1 ] ; Dest[  7 ] = Src[ 1 ] ;
							Dest[  8 ] = Src[ 2 ] ; Dest[  9 ] = Src[ 2 ] ; Dest[ 10 ] = Src[ 2 ] ; Dest[ 11 ] = Src[ 2 ] ;
							Dest[ 12 ] = Src[ 3 ] ; Dest[ 13 ] = Src[ 3 ] ; Dest[ 14 ] = Src[ 3 ] ; Dest[ 15 ] = Src[ 3 ] ;
							Dest[ 16 ] = Src[ 4 ] ; Dest[ 17 ] = Src[ 4 ] ; Dest[ 18 ] = Src[ 4 ] ; Dest[ 19 ] = Src[ 4 ] ;
							Dest[ 20 ] = Src[ 5 ] ; Dest[ 21 ] = Src[ 5 ] ; Dest[ 22 ] = Src[ 5 ] ; Dest[ 23 ] = Src[ 5 ] ;
							Dest[ 24 ] = Src[ 6 ] ; Dest[ 25 ] = Src[ 6 ] ; Dest[ 26 ] = Src[ 6 ] ; Dest[ 27 ] = Src[ 6 ] ;
							Dest[ 28 ] = Src[ 7 ] ; Dest[ 29 ] = Src[ 7 ] ; Dest[ 30 ] = Src[ 7 ] ; Dest[ 31 ] = Src[ 7 ] ;
							Dest += 32 ;
							Src  += 8 ;
						}while( -- w ) ;
					}
					if( OneNum > 0 )
					{
						w = OneNum ;
						do
						{
							Dest[  0 ] = Src[ 0 ] ;
							Dest[  1 ] = Src[ 0 ] ;
							Dest[  2 ] = Src[ 0 ] ;
							Dest[  3 ] = Src[ 0 ] ;
							Dest += 4 ;
							Src  ++ ;
						}while( -- w ) ;
					}
					Dest += DestAddPitch ;
					Src  += SrcAddPitch ;
				}while( -- BltHeight ) ;

				return 0 ;
			}
		}
		else
		if( TransColorAlphaTestFlag == FALSE &&
			SColor.ChannelNum      == 2 &&
			SColor.ChannelBitDepth == 8 )
		{
			BYTE *Dest ;
			BYTE *Src ;
			DWORD BlockNum ;
			DWORD OneNum ;
			DWORD w ;
			int DestAddPitch, SrcAddPitch ;

			DestAddPitch = DestPitch - BltWidth * DColor.PixelByte ;
			SrcAddPitch  = SrcPitch  - BltWidth * SColor.PixelByte ;

			Dest = ( BYTE * )DestGraphData + DestPoint.y  * DestPitch + DestPoint.x   * DColor.PixelByte ;
			Src  = ( BYTE * )SrcGraphData  + SrcRect->top * SrcPitch  + SrcRect->left * SColor.PixelByte ;

			if( DColor.ChannelNum      == 2 &&
				DColor.ChannelBitDepth == 8 )
			{
				BlockNum = BltWidth / 8 ;
				OneNum   = BltWidth - BlockNum * 8 ;

				do
				{
					if( BlockNum > 0 )
					{
						w = BlockNum ;
						do
						{
							( ( DWORD * )Dest )[ 0 ] = ( ( DWORD * )Src )[ 0 ] ;
							( ( DWORD * )Dest )[ 1 ] = ( ( DWORD * )Src )[ 1 ] ;
							( ( DWORD * )Dest )[ 2 ] = ( ( DWORD * )Src )[ 2 ] ;
							( ( DWORD * )Dest )[ 3 ] = ( ( DWORD * )Src )[ 3 ] ;
							Dest += 16 ;
							Src  += 16 ;
						}while( -- w ) ;
					}

					if( OneNum > 0 )
					{
						w = OneNum ;
						do
						{
							*( ( WORD * )Dest ) = *( ( WORD * )Src ) ;
							Dest += 2 ;
							Src  += 2 ;
						}while( -- w ) ;
					}

					Dest += DestAddPitch ;
					Src  += SrcAddPitch ;
				}while( -- BltHeight ) ;

				return 0 ;
			}
			else
			if( ( DColor.ColorBitDepth == 32 &&
				  // DColor.AlphaMask	== 0xff000000 &&
				  DColor.RedMask 	== 0x00ff0000 &&
				  DColor.GreenMask 	== 0x0000ff00 &&
				  DColor.BlueMask 	== 0x000000ff ) ||
				( DColor.ColorBitDepth == 32 &&
				  // DColor.AlphaMask	== 0xff000000 &&
				  DColor.RedMask 	== 0x000000ff &&
				  DColor.GreenMask 	== 0x0000ff00 &&
				  DColor.BlueMask 	== 0x00ff0000 ) )
			{
				BlockNum = BltWidth / 8 ;
				OneNum   = BltWidth - BlockNum * 8 ;

				do
				{
					if( BlockNum > 0 )
					{
						w = BlockNum ;
						do
						{
							Dest[  0 ] = Src[  0 ] ; Dest[  1 ] = Src[  1 ] ; Dest[  2 ] = Src[  0 ] ; Dest[  3 ] = Src[  1 ] ;
							Dest[  4 ] = Src[  2 ] ; Dest[  5 ] = Src[  3 ] ; Dest[  6 ] = Src[  2 ] ; Dest[  7 ] = Src[  3 ] ;
							Dest[  8 ] = Src[  4 ] ; Dest[  9 ] = Src[  5 ] ; Dest[ 10 ] = Src[  4 ] ; Dest[ 11 ] = Src[  5 ] ;
							Dest[ 12 ] = Src[  6 ] ; Dest[ 13 ] = Src[  7 ] ; Dest[ 14 ] = Src[  6 ] ; Dest[ 15 ] = Src[  7 ] ;
							Dest[ 16 ] = Src[  8 ] ; Dest[ 17 ] = Src[  9 ] ; Dest[ 18 ] = Src[  8 ] ; Dest[ 19 ] = Src[  9 ] ;
							Dest[ 20 ] = Src[ 10 ] ; Dest[ 21 ] = Src[ 11 ] ; Dest[ 22 ] = Src[ 10 ] ; Dest[ 23 ] = Src[ 11 ] ;
							Dest[ 24 ] = Src[ 12 ] ; Dest[ 25 ] = Src[ 13 ] ; Dest[ 26 ] = Src[ 12 ] ; Dest[ 27 ] = Src[ 13 ] ;
							Dest[ 28 ] = Src[ 14 ] ; Dest[ 29 ] = Src[ 15 ] ; Dest[ 30 ] = Src[ 14 ] ; Dest[ 31 ] = Src[ 15 ] ;
							Dest += 32 ;
							Src  += 16 ;
						}while( -- w ) ;
					}
					if( OneNum > 0 )
					{
						w = OneNum ;
						do
						{
							Dest[  0 ] = Src[ 0 ] ;
							Dest[  1 ] = Src[ 1 ] ;
							Dest[  2 ] = Src[ 0 ] ;
							Dest[  3 ] = Src[ 1 ] ;
							Dest += 4 ;
							Src  += 2 ;
						}while( -- w ) ;
					}
					Dest += DestAddPitch ;
					Src  += SrcAddPitch ;
				}while( -- BltHeight ) ;

				return 0 ;
			}
		}
		else
		if( TransColorAlphaTestFlag == FALSE &&
			SColor.ColorBitDepth == 8 )
		{
			BYTE *Dest ;
			BYTE *Src ;
#ifdef DX_NON_INLINE_ASM
			DWORD *Palette ;
			BYTE Code[4] ;
			int w ;
#endif
			int DestAddPitch, SrcAddPitch ;

			DestAddPitch = DestPitch - BltWidth * DColor.PixelByte ;
			SrcAddPitch = SrcPitch - BltWidth * SColor.PixelByte ;

#ifdef DX_NON_INLINE_ASM
			Palette = (DWORD *)SColor.Palette ;
#endif
			Dest = ( BYTE * )DestGraphData + DestPoint.y  * DestPitch + DestPoint.x   * DColor.PixelByte ;
			Src  = ( BYTE * )SrcGraphData  + SrcRect->top * SrcPitch  + SrcRect->left * SColor.PixelByte ;

			if( DColor.ColorBitDepth == 16 &&
				DColor.AlphaMask	== 0x00000000 &&
				DColor.RedMask 		== 0x0000f800 &&
				DColor.GreenMask 	== 0x000007e0 &&
				DColor.BlueMask 	== 0x0000001f )
			{
#ifdef DX_NON_INLINE_ASM
				do
				{
					w = BltWidth ;
					do
					{
						*((DWORD *)Code) = Palette[*Src] ;
						*((WORD  *)Dest) = ( WORD )( 
											((Code[2] >> 3) << 11) |
											((Code[1] >> 2) <<  5) |
											((Code[0] >> 3)      ) ) ;
						Dest += 2 ;
						Src ++ ;
					}while( -- w ) ;
					Dest += DestAddPitch ;
					Src += SrcAddPitch ;
				}while( -- BltHeight ) ;
#else
				__asm
				{
					MOV EDI, Dest
					MOV ESI, Src
	LOCK6_LOOP1_8:
					MOV	ECX, BltWidth
	LOCK6_LOOP2_8:
					MOVZX EAX, BYTE PTR [ESI]
					MOV	EAX, DWORD PTR SColor[EAX*4+40]
					MOV	EDX, EAX
					SHR	EDX, 5
					AND	EDX, 0x07e0
					MOV	EBX, EAX
					SHR	EBX, 3
					AND	EBX, 0x001f
					OR	EDX, EBX
					SHR	EAX, 8
					AND	EAX, 0xf800
					OR	EDX, EAX

					MOV	[EDI], DX
					ADD	EDI, 2
					INC	ESI
					DEC	ECX
					JNE	LOCK6_LOOP2_8
					ADD	EDI, DestAddPitch
					ADD	ESI, SrcAddPitch
					DEC	BltHeight
					JNE	LOCK6_LOOP1_8
				} ;
#endif
				return 0 ;
			}
			else
			if( DColor.ColorBitDepth == 16 &&
				DColor.AlphaMask	== 0x00000000 &&
				DColor.RedMask 		== 0x00007c00 &&
				DColor.GreenMask 	== 0x000003e0 &&
				DColor.BlueMask 	== 0x0000001f )
			{
#ifdef DX_NON_INLINE_ASM
				do
				{
					w = BltWidth ;
					do
					{
						*((DWORD *)Code) = Palette[*Src] ;
						*((WORD  *)Dest) = ( WORD )( 
											((Code[2] >> 3) << 10) |
											((Code[1] >> 3) <<  5) |
											((Code[0] >> 3)      ) ) ;
						Dest += 2 ;
						Src ++ ;
					}while( -- w ) ;
					Dest += DestAddPitch ;
					Src += SrcAddPitch ;
				}while( -- BltHeight ) ;
#else
				__asm
				{
					MOV EDI, Dest
					MOV ESI, Src
	LOCK7_LOOP1_8:
					MOV	ECX, BltWidth
	LOCK7_LOOP2_8:
					MOVZX EAX, BYTE PTR [ESI]
					MOV	EAX, DWORD PTR SColor[EAX*4+40]
					MOV	EDX, EAX
					SHR	EDX, 6
					AND	EDX, 0x03e0
					MOV	EBX, EAX
					SHR	EBX, 3
					AND	EBX, 0x001f
					OR	EDX, EBX
					SHR	EAX, 9
					AND	EAX, 0x7c00
					OR	EDX, EAX
					OR  EDX, 0x8000

					MOV	[EDI], DX
					ADD	EDI, 2
					INC	ESI
					DEC	ECX
					JNE	LOCK7_LOOP2_8
					ADD	EDI, DestAddPitch
					ADD	ESI, SrcAddPitch
					DEC	BltHeight
					JNE	LOCK7_LOOP1_8
				} ;
#endif
				return 0 ;
			}
			else
			if( DColor.ColorBitDepth == 32 &&
				DColor.AlphaMask	== 0x00000000 &&
				DColor.RedMask 		== 0x00ff0000 &&
				DColor.GreenMask 	== 0x0000ff00 &&
				DColor.BlueMask 	== 0x000000ff )
			{
#ifdef DX_NON_INLINE_ASM
				do
				{
					w = BltWidth ;
					do
					{
						*((DWORD *)Dest) = Palette[*Src] | 0xff000000 ;
						Dest += 4 ;
						Src ++ ;
					}while( -- w ) ;
					Dest += DestAddPitch ;
					Src += SrcAddPitch ;
				}while( -- BltHeight ) ;
#else
				__asm
				{
					MOV EDI, Dest
					MOV ESI, Src
					MOV EDX, TransColor
					MOV EBX, BltHeight
	LOCK8_LOOP1_8:
					MOV ECX, BltWidth
	LOCK8_LOOP2_8:
					XOR EAX, EAX
					MOV AL, [ESI]
					MOV EAX, DWORD PTR SColor[EAX*4+40]
					OR EAX, 0xFF000000
					MOV [EDI], EAX
					INC ESI
					ADD EDI, 4
					DEC ECX
					JNZ LOCK8_LOOP2_8
					
					ADD EDI, DestAddPitch
					ADD ESI, SrcAddPitch
					DEC EBX
					JNZ LOCK8_LOOP1_8
				} ;
#endif
				return 0 ;
			}
		}
		else
		if( TransColorAlphaTestFlag == TRUE &&
			SColor.ColorBitDepth == 8 )
		{
			BYTE *Dest ;
			BYTE *Src ;
			int DestAddPitch, SrcAddPitch ;
			int w ;
			DWORD *Palette ;
			BYTE Code[4] ;

			Palette = (DWORD *)SColor.Palette ;
			DestAddPitch = DestPitch - BltWidth * DColor.PixelByte ;
			SrcAddPitch = SrcPitch - BltWidth * SColor.PixelByte ;

			Dest = ( BYTE * )DestGraphData + DestPoint.y  * DestPitch + DestPoint.x   * DColor.PixelByte ;
			Src  = ( BYTE * )SrcGraphData  + SrcRect->top * SrcPitch  + SrcRect->left * SColor.PixelByte ;

			if( DColor.ColorBitDepth == 16 &&
				DColor.AlphaMask	== 0x00008000 &&
				DColor.RedMask 		== 0x00007c00 &&
				DColor.GreenMask 	== 0x000003e0 &&
				DColor.BlueMask 	== 0x0000001f )
			{
#ifdef DX_NON_INLINE_ASM
				WORD Alpha ;
				do
				{
					w = BltWidth ;
					do
					{
						*((DWORD *)Code) = Palette[*Src] ;
						if( *((DWORD *)Code) == TransColor )	Alpha = 0 ;
						else									Alpha = 0x8000 ;
						*((WORD *)Dest) =	(WORD)(
											( ( *((DWORD *)Code) & 0xf80000 ) >> 9 ) |
											( ( *((DWORD *)Code) & 0xf800   ) >> 6 ) |
											( ( *((DWORD *)Code) & 0xf8     ) >> 3 ) | Alpha ) ;
						Dest += 2 ;
						Src ++ ;
					}while( -- w ) ;
					Dest += DestAddPitch ;
					Src += SrcAddPitch ;
				}while( -- BltHeight ) ;
#else
				__asm
				{
					MOV EDI, Dest
					MOV ESI, Src
	LOCK5_LOOP1_8:
					MOV	ECX, BltWidth
	LOCK5_LOOP2_8:
					MOVZX EAX, BYTE PTR [ESI]
					MOV	EAX, DWORD PTR SColor[EAX*4+40]
					CMP EAX, TransColor
					JE LOCK5_RABEL1_8
					OR EAX, 0x1000000
	LOCK5_RABEL1_8:
					MOV	EDX, EAX
					SHR	EDX, 6
					AND	EDX, 0x03e0
					MOV	EBX, EAX
					SHR	EBX, 3
					AND	EBX, 0x001f
					OR	EDX, EBX
					SHR	EAX, 9
					AND	EAX, 0xfc00
					OR	EDX, EAX

					MOV	[EDI], DX
					ADD	EDI, 2
					INC	ESI
					DEC	ECX
					JNE	LOCK5_LOOP2_8
					ADD	EDI, DestAddPitch
					ADD	ESI, SrcAddPitch
					DEC	BltHeight
					JNE	LOCK5_LOOP1_8
				} ;
#endif
				return 0 ;
			}
			else
			if( DColor.ColorBitDepth == 32 &&
				DColor.AlphaMask	== 0xff000000 &&
				DColor.RedMask 		== 0x00ff0000 &&
				DColor.GreenMask 	== 0x0000ff00 &&
				DColor.BlueMask 	== 0x000000ff )
			{
#ifdef DX_NON_INLINE_ASM
				do
				{
					w = BltWidth ;
					do
					{
						*((DWORD *)Code) = Palette[*Src] ;
						if( *((DWORD *)Code) == TransColor )	Code[3] = 0 ;
						else									Code[3] = 0xff ;
						*((DWORD *)Dest) = *((DWORD *)Code) ;
						Dest += 4 ;
						Src ++ ;
					}while( -- w ) ;
					Dest += DestAddPitch ;
					Src += SrcAddPitch ;
				}while( -- BltHeight ) ;
#else
				__asm
				{
					MOV EDI, Dest
					MOV ESI, Src
					MOV EDX, TransColor
					MOV EBX, BltHeight
	LOCK4_LOOP1_8:
					MOV ECX, BltWidth
	LOCK4_LOOP2_8:
					MOVZX EAX, BYTE PTR [ESI]
					MOV EAX, DWORD PTR SColor[EAX*4+40]
					CMP EAX, EDX
					JE LOCK4_RABEL1_8
					OR EAX, 0xFF000000
	LOCK4_RABEL1_8:
					MOV [EDI], EAX
					INC ESI
					ADD EDI, 4
					DEC ECX
					JNZ LOCK4_LOOP2_8
					
					ADD EDI, DestAddPitch
					ADD ESI, SrcAddPitch
					DEC EBX
					JNZ LOCK4_LOOP1_8
				} ;
#endif
				return 0 ;
			}
			else
			if( DColor.ColorBitDepth == 32 &&
				DColor.AlphaMask	== 0xff000000 &&
				DColor.RedMask 		== 0x000000ff &&
				DColor.GreenMask 	== 0x0000ff00 &&
				DColor.BlueMask 	== 0x00ff0000 )
			{
				DWORD TransColorRev ;

				TransColorRev = ( ( TransColor & 0x00ff0000 ) >> 16 ) |
								( ( TransColor & 0x000000ff ) << 16 ) |
								( ( TransColor & 0xff00ff00 )       ) ;

				do
				{
					w = BltWidth ;
					do
					{
						Code[ 0 ] = ( ( BYTE * )&Palette[ *Src ] )[ 2 ] ;
						Code[ 1 ] = ( ( BYTE * )&Palette[ *Src ] )[ 1 ] ;
						Code[ 2 ] = ( ( BYTE * )&Palette[ *Src ] )[ 0 ] ;
						Code[ 3 ] = ( ( BYTE * )&Palette[ *Src ] )[ 3 ] ;
						if( *((DWORD *)Code) == TransColorRev )	Code[3] = 0 ;
						else									Code[3] = 0xff ;
						*((DWORD *)Dest) = *((DWORD *)Code) ;
						Dest += 4 ;
						Src ++ ;
					}while( -- w ) ;
					Dest += DestAddPitch ;
					Src += SrcAddPitch ;
				}while( -- BltHeight ) ;
				return 0 ;
			}
		}
		else
		if( TransColorAlphaTestFlag == FALSE &&
			SColor.ColorBitDepth	== 32 &&
			SColor.AlphaMask		== 0xff000000 &&
			SColor.RedMask 			== 0x00ff0000 &&
			SColor.GreenMask 		== 0x0000ff00 &&
			SColor.BlueMask 		== 0x000000ff )
		{
			BYTE *Dest ;
			BYTE *Src ;
			int DestAddPitch, SrcAddPitch ;
			int w ;

			DestAddPitch = DestPitch - BltWidth * DColor.PixelByte ;
			SrcAddPitch = SrcPitch - BltWidth * SColor.PixelByte ;
			
			Dest = ( BYTE * )DestGraphData + DestPoint.y  * DestPitch + DestPoint.x   * DColor.PixelByte ;
			Src  = ( BYTE * )SrcGraphData  + SrcRect->top * SrcPitch  + SrcRect->left * SColor.PixelByte ;

			if( DColor.ColorBitDepth	== 32 &&
				DColor.AlphaMask		== 0xff000000 &&
				DColor.RedMask			== 0x000000ff &&
				DColor.GreenMask	 	== 0x0000ff00 &&
				DColor.BlueMask			== 0x00ff0000 )
			{
				do
				{
					w = BltWidth ;
					do
					{
						*( (DWORD *) Dest ) = ( DWORD )( 
												( Src[ 0 ] << 16 ) |
												( Src[ 1 ] <<  8 ) |
												( Src[ 2 ]       ) |
												( Src[ 3 ] << 24 ) ) ;
						Dest += 4 ;
						Src  += 4 ;
					}while( -- w ) ;
					Dest += DestAddPitch ;
					Src  += SrcAddPitch ;
				}while( -- BltHeight ) ;

				return 0 ;
			}
			else
			if( DColor.ColorBitDepth	== 32 &&
				DColor.AlphaMask		== 0x00000000 &&
				DColor.RedMask			== 0x00ff0000 &&
				DColor.GreenMask	 	== 0x0000ff00 &&
				DColor.BlueMask			== 0x000000ff )
			{
				do
				{
					w = BltWidth ;
					do
					{
						*( (DWORD *) Dest ) = *( ( DWORD *) Src ) ;
						Dest += 4 ;
						Src  += 4 ;
					}while( -- w ) ;
					Dest += DestAddPitch ;
					Src  += SrcAddPitch ;
				}while( -- BltHeight ) ;

				return 0 ;
			}
		}
		else
		if( TransColorAlphaTestFlag == FALSE &&
			SColor.ColorBitDepth == 24 &&
			SColor.RedMask 		== 0xff0000 &&
			SColor.GreenMask 	== 0x00ff00 &&
			SColor.BlueMask 	== 0x0000ff )
		{
			BYTE *Dest ;
			BYTE *Src ;
			int DestAddPitch, SrcAddPitch ;
			int w ;

			DestAddPitch = DestPitch - BltWidth * DColor.PixelByte ;
			SrcAddPitch = SrcPitch - BltWidth * SColor.PixelByte ;
			
			Dest = ( BYTE * )DestGraphData + DestPoint.y  * DestPitch + DestPoint.x   * DColor.PixelByte ;
			Src  = ( BYTE * )SrcGraphData  + SrcRect->top * SrcPitch  + SrcRect->left * SColor.PixelByte ;

			if( DColor.ColorBitDepth == 16 &&
				DColor.AlphaMask	== 0x00000000 &&
				DColor.RedMask 		== 0x0000f800 &&
				DColor.GreenMask 	== 0x000007e0 &&
				DColor.BlueMask 	== 0x0000001f )
			{
#ifdef DX_NON_INLINE_ASM
				do
				{
					w = BltWidth ;
					do
					{
						*((WORD *)Dest) = ( WORD )(
											((Src[2] >> 3) << 11) |
											((Src[1] >> 2) <<  5) |
											((Src[0] >> 3)      ) ) ;
						Dest += 2 ;
						Src += 3 ;
					}while( -- w ) ;
					Dest += DestAddPitch ;
					Src += SrcAddPitch ;
				}while( -- BltHeight ) ;
#else
				__asm
				{
					MOV EDI, Dest
					MOV ESI, Src
					MOV EBX, BltWidth
					MOV EDX, DestAddPitch
	LOCK3_LOOP1:
					MOV ECX, EBX // BltWidth
	LOCK3_LOOP2:
					XOR EAX, EAX
					MOV AL, [ESI+2]
					SHR EAX, 3
					SHL EAX, 11
					MOV EDX, EAX
					XOR EAX, EAX
					MOV AL, [ESI+1]
					SHR EAX, 2
					SHL EAX, 5
					OR EDX, EAX
					XOR EAX, EAX
					MOV AL, [ESI]
					SHR EAX, 3
					OR EDX, EAX
					MOV [EDI], DX
					ADD EDI, 2
					ADD ESI, 3
					DEC ECX
					JNZ LOCK3_LOOP2
					
					ADD EDI, DestAddPitch
					ADD ESI, SrcAddPitch
					DEC BltHeight
					JNZ LOCK3_LOOP1
				} ;
#endif
				return 0 ;
			}
			else
			if( DColor.ColorBitDepth == 16 &&
				DColor.AlphaMask	== 0x00000000 &&
				DColor.RedMask 		== 0x00007c00 &&
				DColor.GreenMask 	== 0x000003e0 &&
				DColor.BlueMask 	== 0x0000001f )
			{
#ifdef DX_NON_INLINE_ASM
				do
				{
					w = BltWidth ;
					do
					{
						*((WORD *)Dest) = ( WORD )(
											((Src[2] >> 3) << 10) |
											((Src[1] >> 3) <<  5) |
											((Src[0] >> 3)      ) ) ;
						Dest += 2 ;
						Src += 3 ;
					}while( -- w ) ;
					Dest += DestAddPitch ;
					Src += SrcAddPitch ;
				}while( -- BltHeight ) ;
#else
				__asm
				{
					MOV EDI, Dest
					MOV ESI, Src
					MOV EBX, BltWidth
					MOV EDX, DestAddPitch
	LOCK2_LOOP1:
					MOV ECX, EBX // BltWidth
	LOCK2_LOOP2:
					XOR EAX, EAX
					MOV AL, [ESI+2]
					SHR EAX, 3
					SHL EAX, 10
					MOV EDX, EAX
					XOR EAX, EAX
					MOV AL, [ESI+1]
					SHR EAX, 3
					SHL EAX, 5
					OR EDX, EAX
					XOR EAX, EAX
					MOV AL, [ESI]
					SHR EAX, 3
					OR EAX, 0x8000
					OR EDX, EAX
					MOV [EDI], DX
					ADD EDI, 2
					ADD ESI, 3
					DEC ECX
					JNZ LOCK2_LOOP2
					
					ADD EDI, DestAddPitch
					ADD ESI, SrcAddPitch
					DEC BltHeight
					JNZ LOCK2_LOOP1
				} ;
#endif
				return 0 ;
			}
			else
			if( DColor.ColorBitDepth == 32 &&
//				DColor.AlphaMask	== 0x00000000 &&
				DColor.RedMask 		== 0x00ff0000 &&
				DColor.GreenMask 	== 0x0000ff00 &&
				DColor.BlueMask 	== 0x000000ff )
			{
#ifdef DX_NON_INLINE_ASM
				do
				{
					w = BltWidth ;
					do
					{
						*((DWORD *)Dest) = *((WORD *)Src) | ( Src[2] << 16 ) | 0xff000000 ;
						Dest += 4 ;
						Src += 3 ;
					}while( -- w ) ;
					Dest += DestAddPitch ;
					Src += SrcAddPitch ;
				}while( -- BltHeight ) ;
#else
				__asm
				{
					PUSH EBP
					MOV EDI, Dest
					MOV ESI, Src
					MOV EAX, SrcAddPitch
					PUSH EAX
					MOV EBX, BltWidth
					MOV EAX, BltHeight
					MOV EDX, DestAddPitch
					POP EBP // SrcAddPitch
	LOCK1_LOOP1:
					MOV ECX, EBX // BltWidth
	LOCK1_LOOP2:
					MOVSW
					MOVSB
					MOV byte ptr [EDI], 0xFF
					INC EDI
					DEC ECX
					JNZ LOCK1_LOOP2
					
					ADD EDI, EDX // DestAddPitch
					ADD ESI, EBP // SrcAddPitch
					DEC EAX // BltHeight
					JNZ LOCK1_LOOP1
					POP EBP
				} ;
#endif
				return 0 ;
			}
			else
			if( DColor.ColorBitDepth == 32 &&
//				DColor.AlphaMask	== 0x00000000 &&
				DColor.RedMask 		== 0x000000ff &&
				DColor.GreenMask 	== 0x0000ff00 &&
				DColor.BlueMask 	== 0x00ff0000 )
			{
				do
				{
					w = BltWidth ;
					do
					{
						*((DWORD *)Dest) = ( Src[ 0 ] << 16 ) | ( Src[ 1 ] << 8 ) | Src[ 2 ] | 0xff000000 ;
						Dest += 4 ;
						Src += 3 ;
					}while( -- w ) ;
					Dest += DestAddPitch ;
					Src += SrcAddPitch ;
				}while( -- BltHeight ) ;
				return 0 ;
			}
		}
		else
		if( TransColorAlphaTestFlag == TRUE &&
			SColor.ColorBitDepth == 24 &&
			SColor.RedMask 		== 0xff0000 &&
			SColor.GreenMask 	== 0x00ff00 &&
			SColor.BlueMask 	== 0x0000ff )
		{
			BYTE *Dest ;
			BYTE *Src ;
			int DestAddPitch, SrcAddPitch ;
			int w, Alpha ;
			union
			{
				unsigned int dword ;
				unsigned char byte[4] ;
			} SrcColor ;

			DestAddPitch = DestPitch - BltWidth * DColor.PixelByte ;
			SrcAddPitch = SrcPitch - BltWidth * SColor.PixelByte ;
			
			Dest = ( BYTE * )DestGraphData + DestPoint.y  * DestPitch + DestPoint.x   * DColor.PixelByte ;
			Src  = ( BYTE * )SrcGraphData  + SrcRect->top * SrcPitch  + SrcRect->left * SColor.PixelByte ;

			if( DColor.ColorBitDepth == 16 &&
				DColor.AlphaMask	== 0x00008000 &&
				DColor.RedMask 		== 0x00007c00 &&
				DColor.GreenMask 	== 0x000003e0 &&
				DColor.BlueMask 	== 0x0000001f )
			{
#ifdef DX_NON_INLINE_ASM
				do
				{
					w = BltWidth ;
					do
					{
						SrcColor.dword = ( unsigned int )( *((WORD *)Src) + (Src[2] << 16) ) ;
						Alpha          = 0x8000 ;
						if( SrcColor.dword == TransColor ) Alpha = 0 ;
						*((WORD *)Dest) = ( WORD )(
											((SrcColor.byte[2] >> 3) << 10) |
											((SrcColor.byte[1] >> 3) <<  5) |
											((SrcColor.byte[0] >> 3)      ) |
											Alpha ) ;
						Dest += 2 ;
						Src += 3 ;
					}while( -- w ) ;
					Dest += DestAddPitch ;
					Src += SrcAddPitch ;
				}while( -- BltHeight ) ;
#else
//				DWORD TransCode ;
//				TransCode = (   ((BYTE *)&TransColor)[0] >> 3) |
//							( ( ((BYTE *)&TransColor)[1] >> 3) << 5) |
//							( ( ((BYTE *)&TransColor)[2] >> 3) << 10) ;
				__asm
				{
					MOV EDI, Dest
					MOV ESI, Src
					MOV EBX, TransColor
	LOCK5_LOOP1:
					MOV ECX, BltWidth
	LOCK5_LOOP2:
					MOV EAX, [ESI]
					AND EAX, 0x00ffffff
					CMP EAX, EBX
					JE LOCK5_RABEL1
					XOR EDX, EDX
					AND EAX, 0xF8F8F8
					SHR EAX, 3
					MOV DL, AL
					SHR	AX, 8
					SHL AX, 5
					OR DX, AX
					SHR EAX, 16
					SHL EAX, 10
					OR EDX, EAX
					OR EDX, 0x8000
					MOV [EDI], DX
					ADD EDI, 2
					ADD ESI, 3
					DEC ECX
					JNZ LOCK5_LOOP2
					JMP LOCK5_RABEL2
	LOCK5_RABEL1:
					XOR EDX, EDX
					AND EAX, 0xF8F8F8
					SHR EAX, 3
					MOV DL, AL
					SHR	AX, 8
					SHL AX, 5
					OR DX, AX
					SHR EAX, 16
					SHL EAX, 10
					OR EDX, EAX
					MOV [EDI], DX
					ADD EDI, 2
					ADD ESI, 3
					DEC ECX
					JNZ LOCK5_LOOP2
	LOCK5_RABEL2:
					
					ADD EDI, DestAddPitch
					ADD ESI, SrcAddPitch
					DEC BltHeight
					JNZ LOCK5_LOOP1
				} ;
#endif
				return 0 ;
			}
			else
			if( DColor.ColorBitDepth == 32 &&
				DColor.AlphaMask	== 0xff000000 &&
				DColor.RedMask 		== 0x00ff0000 &&
				DColor.GreenMask 	== 0x0000ff00 &&
				DColor.BlueMask 	== 0x000000ff )
			{
#ifdef DX_NON_INLINE_ASM
				do
				{
					w = BltWidth ;
					do
					{
						SrcColor.dword = ( unsigned int )( *((WORD *)Src) + (Src[2] << 16) ) ;
						Alpha = 0xff000000 ;
						if( SrcColor.dword == TransColor ) Alpha = 0 ;
						*((DWORD *)Dest) = SrcColor.dword | Alpha ;
						Dest += 4 ;
						Src += 3 ;
					}while( -- w ) ;
					Dest += DestAddPitch ;
					Src += SrcAddPitch ;
				}while( -- BltHeight ) ;
#else
				__asm
				{
					MOV EDI, Dest
					MOV ESI, Src
					MOV EDX, TransColor
					MOV EBX, BltHeight
	LOCK4_LOOP1:
					MOV ECX, BltWidth
	LOCK4_LOOP2:
					XOR EAX, EAX
					MOV AL, [ESI+2]
					SHL EAX, 16
					MOV AX, [ESI]
					CMP EAX, EDX
					JE LOCK4_RABEL1
					OR EAX, 0xFF000000
	LOCK4_RABEL1:
					MOV [EDI], EAX
					ADD ESI, 3
					ADD EDI, 4
					DEC ECX
					JNZ LOCK4_LOOP2
					
					ADD EDI, DestAddPitch
					ADD ESI, SrcAddPitch
					DEC EBX
					JNZ LOCK4_LOOP1
				} ;
#endif
				return 0 ;
			}
			else
			if( DColor.ColorBitDepth == 32 &&
				DColor.AlphaMask	== 0xff000000 &&
				DColor.RedMask 		== 0x000000ff &&
				DColor.GreenMask 	== 0x0000ff00 &&
				DColor.BlueMask 	== 0x00ff0000 )
			{
				do
				{
					w = BltWidth ;
					do
					{
						SrcColor.dword = ( unsigned int )( *((WORD *)Src) + (Src[2] << 16) ) ;
						Alpha = 0xff000000 ;
						if( SrcColor.dword == TransColor ) Alpha = 0 ;
						*((DWORD *)Dest) = ( DWORD )( ( Src[ 0 ] << 16 ) | ( Src[ 1 ] << 8 ) | Src[ 2 ] | Alpha ) ;
						Dest += 4 ;
						Src  += 3 ;
					}while( -- w ) ;
					Dest += DestAddPitch ;
					Src  += SrcAddPitch ;
				}while( -- BltHeight ) ;

				return 0 ;
			}
		}
		else
		if( TransColorAlphaTestFlag == FALSE &&
			AlphaFlag               == FALSE &&
			SColor.ColorBitDepth    == 32 &&
			SColor.AlphaMask	    == 0x00000000 &&
			SColor.RedMask 	   	    == 0x00ff0000 &&
			SColor.GreenMask 	    == 0x0000ff00 &&
			SColor.BlueMask 	    == 0x000000ff )
		{
			BYTE *Dest ;
			BYTE *Src ;
			int DestAddPitch, SrcAddPitch ;

			DestAddPitch = DestPitch - BltWidth * DColor.PixelByte ;
			SrcAddPitch  = SrcPitch  - BltWidth * SColor.PixelByte ;

			Dest = ( BYTE * )DestGraphData + DestPoint.y  * DestPitch + DestPoint.x   * DColor.PixelByte ;
			Src  = ( BYTE * )SrcGraphData  + SrcRect->top * SrcPitch  + SrcRect->left * SColor.PixelByte ;

			if( DColor.ColorBitDepth == 16 &&
				DColor.AlphaMask	 == 0x00000000 &&
				DColor.RedMask 		 == 0x0000f800 &&
				DColor.GreenMask 	 == 0x000007e0 &&
				DColor.BlueMask 	 == 0x0000001f )
			{
#ifdef DX_NON_INLINE_ASM
				int i ;
				DWORD SrcC ;
				do
				{
					i = BltWidth ;
					do
					{
						SrcC = *( ( DWORD * )Src ) ;
						*( ( WORD * )Dest ) = ( WORD )
							( ( ( SrcC & 0x00f80000 ) >> 8 ) |
							  ( ( SrcC & 0x0000fc00 ) >> 5 ) |
							  ( ( SrcC & 0x000000f8 ) >> 3 ) ) ;
						Src += 4 ;
						Dest += 2 ;
					}while( -- i != 0 ) ;

					Dest += DestAddPitch ;
					Src += SrcAddPitch ;

				}while( -- BltHeight != 0 ) ;
#else
				__asm
				{
					MOV EDI, Dest
					MOV ESI, Src
	LOCK6_LOOP1:
					MOV ECX, BltWidth
	LOCK6_LOOP2:
					MOV EAX, [ESI]
					MOV EBX, EAX
					MOV EDX, EAX
					AND EBX, 0x00f80000
					AND EDX, 0x0000fc00
					AND EAX, 0x000000f8
					SHR EBX, 8
					SHR EDX, 5
					SHR EAX, 3
					OR EDX, EBX
					OR EAX, EDX
					MOV [EDI], AX
					ADD EDI, 2
					ADD ESI, 4
					DEC ECX
					JNZ LOCK6_LOOP2
					
					ADD EDI, DestAddPitch
					ADD ESI, SrcAddPitch
					DEC BltHeight
					JNZ LOCK6_LOOP1
				} ;
#endif
				return 0 ;
			}
		}
		else
		if( TransColorAlphaTestFlag == FALSE &&
			SColor.ColorBitDepth    == 16 &&
			SColor.AlphaMask	    == 0x00008000 &&
			SColor.RedMask 			== 0x00007c00 &&
			SColor.GreenMask 		== 0x000003e0 &&
			SColor.BlueMask 		== 0x0000001f )
		{
			BYTE *Dest ;
			BYTE *Src ;
			int DestAddPitch, SrcAddPitch ;

			DestAddPitch = DestPitch - BltWidth * DColor.PixelByte ;
			SrcAddPitch  = SrcPitch  - BltWidth * SColor.PixelByte ;

			Dest = ( BYTE * )DestGraphData + DestPoint.y  * DestPitch + DestPoint.x   * DColor.PixelByte ;
			Src  = ( BYTE * )SrcGraphData  + SrcRect->top * SrcPitch  + SrcRect->left * SColor.PixelByte ;

			if( DColor.ColorBitDepth == 32 &&
				DColor.AlphaMask	 == 0xff000000 &&
				DColor.RedMask 		 == 0x000000ff &&
				DColor.GreenMask 	 == 0x0000ff00 &&
				DColor.BlueMask 	 == 0x00ff0000 )
			{
				int i ;
				DWORD SrcC ;
				do
				{
					i = BltWidth ;
					do
					{
						SrcC  = *( ( WORD * )Src ) ;
						if( SrcC & 0x8000 )
						{
							*( ( DWORD * )Dest ) =
								( ( SrcC & 0x7c00 ) >>  7 ) |
								( ( SrcC & 0x03e0 ) <<  6 ) |
								( ( SrcC & 0x001f ) << 19 ) | 0xff000000 ;
						}
						else
						{
							*( ( DWORD * )Dest ) =
								( ( SrcC & 0x7c00 ) >>  7 ) |
								( ( SrcC & 0x03e0 ) <<  6 ) |
								( ( SrcC & 0x001f ) << 19 ) ;
						}

						Src  += 2 ;
						Dest += 4 ;
					}while( -- i != 0 ) ;

					Dest += DestAddPitch ;
					Src += SrcAddPitch ;

				}while( -- BltHeight != 0 ) ;

				return 0 ;
			}
		}
		else
		if( TransColorAlphaTestFlag == TRUE &&
			SColor.ColorBitDepth    == 16 &&
			SColor.AlphaMask	    == 0x00000000 &&
			SColor.RedMask 	   	    == 0x0000f800 &&
			SColor.GreenMask 	    == 0x000007e0 &&
			SColor.BlueMask 	    == 0x0000001f )
		{
			BYTE *Dest ;
			BYTE *Src ;
			int DestAddPitch, SrcAddPitch ;
			DWORD TransCode ;

			DestAddPitch = DestPitch - BltWidth * DColor.PixelByte ;
			SrcAddPitch  = SrcPitch  - BltWidth * SColor.PixelByte ;

			Dest = ( BYTE * )DestGraphData + DestPoint.y  * DestPitch + DestPoint.x   * DColor.PixelByte ;
			Src  = ( BYTE * )SrcGraphData  + SrcRect->top * SrcPitch  + SrcRect->left * SColor.PixelByte ;

			TransCode = ( DWORD )( 
						(   ((BYTE *)&TransColor)[0] >> 3)        |
						( ( ((BYTE *)&TransColor)[1] >> 3) <<  5) |
						( ( ((BYTE *)&TransColor)[2] >> 3) << 10) ) ;

			if( DColor.ColorBitDepth == 16 &&
				DColor.AlphaMask	 == 0x00008000 &&
				DColor.RedMask 		 == 0x00007c00 &&
				DColor.GreenMask 	 == 0x000003e0 &&
				DColor.BlueMask 	 == 0x0000001f )
			{
#ifdef DX_NON_INLINE_ASM
				int i ;
				DWORD SrcC ;
				do
				{
					i = BltWidth ;
					do
					{
						SrcC = ( DWORD )( ( *( ( WORD * )Src ) & 0x1f ) | ( ( *( ( WORD * )Src ) & 0xffc0 ) >> 1 ) ) ;
						if( SrcC != TransCode )
							*( ( WORD * )Dest ) = ( WORD )( SrcC | 0x8000 ) ;
						Src += 2 ;
						Dest += 2 ;
					}while( -- i != 0 ) ;

					Dest += DestAddPitch ;
					Src += SrcAddPitch ;

				}while( -- BltHeight != 0 ) ;
#else
				__asm
				{
					MOV EDI, Dest
					MOV ESI, Src
					MOV EBX, TransCode
	LOCK11_LOOP1:
					MOV ECX, BltWidth
	LOCK11_LOOP2:
					XOR EAX, EAX
					MOV AX, [ESI]
					MOV EDX, EAX
					AND EDX, 0x1f
					AND EAX, 0xffc0
					SHR EAX, 1
					OR  EAX, EDX
					CMP EAX, EBX
					JE LOCK11_RABEL1
					OR EAX, 0x8000
	LOCK11_RABEL1:
					MOV [EDI], AX
					ADD EDI, 2
					ADD ESI, 2
					DEC ECX
					JNZ LOCK11_LOOP2
					
					ADD EDI, DestAddPitch
					ADD ESI, SrcAddPitch
					DEC BltHeight
					JNZ LOCK11_LOOP1
				} ;
#endif
				return 0 ;
			}
			else
			if( DColor.ColorBitDepth == 32 &&
				DColor.AlphaMask	 == 0xff000000 &&
				DColor.RedMask 		 == 0x00ff0000 &&
				DColor.GreenMask 	 == 0x0000ff00 &&
				DColor.BlueMask 	 == 0x000000ff )
			{
#ifdef DX_NON_INLINE_ASM
				int i ;
				DWORD SrcC ;
				do
				{
					i = BltWidth ;
					do
					{
						SrcC = *( ( WORD * )Src ) ;
						if( SrcC != TransCode )
						{
							*( ( DWORD * )Dest ) = 
								( ( SrcC & 0x001f ) << 3 ) |
								( ( SrcC & 0x07e0 ) << 5 ) |
								( ( SrcC & 0xf800 ) << 8 ) |
								0xff000000 ;
						}
						Src += 2 ;
						Dest += 4 ;
					}while( -- i != 0 ) ;

					Dest += DestAddPitch ;
					Src += SrcAddPitch ;

				}while( -- BltHeight != 0 ) ;
#else
				__asm
				{
					MOV EDI, Dest
					MOV ESI, Src
	LOCK15_LOOP1:
					MOV ECX, BltWidth
	LOCK15_LOOP2:
					XOR EAX, EAX
					MOV AX, [ESI]
					MOV EDX, EAX
					MOV EBX, EAX
					AND EDX, 0x001f
					AND EAX, 0x07e0
					AND EBX, 0xf800
					SHL EDX, 3
					SHL EAX, 5
					SHL EBX, 8
					OR  EAX, EDX
					OR  EAX, EBX
					XOR EDX, EDX
					MOV DX, [ESI]
					CMP EDX, TransCode
					JE  LOCK15_RABEL1
					OR  EAX, 0xff000000
	LOCK15_RABEL1:
					MOV [EDI], EAX
					ADD EDI, 4
					ADD ESI, 2
					DEC ECX
					JNZ LOCK15_LOOP2
					
					ADD EDI, DestAddPitch
					ADD ESI, SrcAddPitch
					DEC BltHeight
					JNZ LOCK15_LOOP1
				} ;
#endif
				return 0 ;
			}
		}
		else
		if( TransColorAlphaTestFlag == FALSE &&
			AlphaFlag               == FALSE &&
			SColor.ColorBitDepth    == 16 &&
			SColor.AlphaMask	    == 0x00000000 &&
			SColor.RedMask 	   	    == 0x0000f800 &&
			SColor.GreenMask 	    == 0x000007e0 &&
			SColor.BlueMask 	    == 0x0000001f )
		{
			BYTE *Dest ;
			BYTE *Src ;
			int DestAddPitch, SrcAddPitch ;

			DestAddPitch = DestPitch - BltWidth * DColor.PixelByte ;
			SrcAddPitch  = SrcPitch  - BltWidth * SColor.PixelByte ;

			Dest = ( BYTE * )DestGraphData + DestPoint.y  * DestPitch + DestPoint.x   * DColor.PixelByte ;
			Src  = ( BYTE * )SrcGraphData  + SrcRect->top * SrcPitch  + SrcRect->left * SColor.PixelByte ;

			if( DColor.ColorBitDepth == 24 &&
				DColor.AlphaMask	 == 0x000000 &&
				DColor.RedMask 		 == 0xff0000 &&
				DColor.GreenMask 	 == 0x00ff00 &&
				DColor.BlueMask 	 == 0x0000ff )
			{
#ifdef DX_NON_INLINE_ASM
				int i ;
				DWORD SrcC ;
				do
				{
					i = BltWidth ;
					do
					{
						SrcC = ( WORD )
							( ( ( *( ( WORD * )Src ) & 0x001f ) << 3 ) |
							  ( ( *( ( WORD * )Src ) & 0x07e0 ) << 5 ) |
							  ( ( *( ( WORD * )Src ) & 0xf800 ) << 8 ) ) ;
						*( ( WORD * )Dest ) = ( WORD )SrcC ;
						Dest[ 2 ] = ( BYTE )( SrcC >> 16 ) ;
						Src += 2 ;
						Dest += 3 ;
					}while( -- i != 0 ) ;

					Dest += DestAddPitch ;
					Src += SrcAddPitch ;

				}while( -- BltHeight != 0 ) ;
#else
				__asm
				{
					MOV EDI, Dest
					MOV ESI, Src
	LOCK14_LOOP1:
					MOV ECX, BltWidth
	LOCK14_LOOP2:
					MOV AX, [ESI]
					MOV EBX, EAX
					MOV EDX, EAX
					AND EBX, 0x0000f800
					AND EDX, 0x000007e0
					AND EAX, 0x0000001f
					SHL EBX, 8
					SHL EDX, 5
					SHL EAX, 3
					OR EDX, EBX
					OR EAX, EDX
					MOV [EDI], AX
					SHR EAX, 16
					MOV [EDI+2], AL
					ADD EDI, 3
					ADD ESI, 2
					DEC ECX
					JNZ LOCK14_LOOP2
					
					ADD EDI, DestAddPitch
					ADD ESI, SrcAddPitch
					DEC BltHeight
					JNZ LOCK14_LOOP1
				} ;
#endif
				return 0 ;
			}
			else
			if( DColor.ColorBitDepth == 32 &&
				DColor.AlphaMask	 == 0x00000000 &&
				DColor.RedMask 		 == 0x00ff0000 &&
				DColor.GreenMask 	 == 0x0000ff00 &&
				DColor.BlueMask 	 == 0x000000ff )
			{
#ifdef DX_NON_INLINE_ASM
				int i ;
				do
				{
					i = BltWidth ;
					do
					{
						*( ( DWORD * )Dest ) = ( DWORD )( 
							( ( *( ( WORD * )Src ) & 0x001f ) << 3 ) |
							( ( *( ( WORD * )Src ) & 0x07e0 ) << 5 ) |
							( ( *( ( WORD * )Src ) & 0xf800 ) << 8 ) ) ;
						Src += 2 ;
						Dest += 4 ;
					}while( -- i != 0 ) ;

					Dest += DestAddPitch ;
					Src += SrcAddPitch ;

				}while( -- BltHeight != 0 ) ;
#else
				__asm
				{
					MOV EDI, Dest
					MOV ESI, Src
	LOCK8_LOOP1:
					MOV ECX, BltWidth
	LOCK8_LOOP2:
					MOV AX, [ESI]
					MOV EBX, EAX
					MOV EDX, EAX
					AND EBX, 0x0000f800
					AND EDX, 0x000007e0
					AND EAX, 0x0000001f
					SHL EBX, 8
					SHL EDX, 5
					SHL EAX, 3
					OR EDX, EBX
					OR EAX, EDX
					MOV [EDI], EAX
					ADD EDI, 4
					ADD ESI, 2
					DEC ECX
					JNZ LOCK8_LOOP2
					
					ADD EDI, DestAddPitch
					ADD ESI, SrcAddPitch
					DEC BltHeight
					JNZ LOCK8_LOOP1
				} ;
#endif
				return 0 ;
			}
			else
			if( DColor.ColorBitDepth == 16 &&
				DColor.AlphaMask	 == 0x00000000 &&
				DColor.RedMask 		 == 0x00007c00 &&
				DColor.GreenMask 	 == 0x000003e0 &&
				DColor.BlueMask 	 == 0x0000001f )
			{
#ifdef DX_NON_INLINE_ASM
				int i ;
				do
				{
					i = BltWidth >> 1 ;
					if( i != 0 )
					{
						do
						{
							*( ( DWORD * )Dest ) =  
								( ( *( ( DWORD * )Src ) & 0xffc0ffc0 ) >> 1 ) |
								( ( *( ( DWORD * )Src ) & 0x001f001f ) ) ;
							Src += 4 ;
							Dest += 4 ;
						}while( -- i != 0 ) ;
					}

					if( ( BltWidth & 1 ) != 0 )
					{
						*( ( DWORD * )Dest ) =  
							( ( *( ( DWORD * )Src ) & 0xffc0 ) >> 1 ) |
							( ( *( ( DWORD * )Src ) & 0x001f ) ) ;
						Src += 2 ;
						Dest += 2 ;
					}

					Dest += DestAddPitch ;
					Src += SrcAddPitch ;

				}while( -- BltHeight != 0 ) ;
#else
				if( NS_GetUseMMXFlag() == TRUE )
				{
					__asm
					{
						MOV   EAX, 0xffc0ffc0
						MOV   EBX, 0x001f001f
						MOVD  MM0, EAX
						MOVD  MM1, EBX
						MOVQ  MM2, MM0
						MOVQ  MM3, MM1
						PSLLQ MM2, 32
						PSLLQ MM3, 32
						POR   MM0, MM2
						POR   MM1, MM3
						MOV EDI, Dest
						MOV ESI, Src
		LOCK9_MMX_LOOP1:
						MOV ECX, BltWidth
						SHR ECX, 2
						JZ LOCK9_MMX_LABEL1
		LOCK9_MMX_LOOP2:
						MOVQ  MM2, [ESI]
						MOVQ  MM3, MM2
						PAND  MM2, MM0
						PAND  MM3, MM1
						PSRLQ MM2, 1
						POR   MM2, MM3
						MOVQ  [EDI], MM2
						ADD EDI, 8
						ADD ESI, 8
						DEC ECX
						JNZ LOCK9_MMX_LOOP2

		LOCK9_MMX_LABEL1:
						MOV ECX, BltWidth
						AND ECX, 3
						JZ LOCK9_MMX_LABEL2
		LOCK9_MMX_LOOP3:
						MOV AX, [ESI]
						MOV EBX, EAX
						AND EBX, 0xffc0ffc0
						AND EAX, 0x001f001f
						SHR EBX, 1
						OR EAX, EBX
						MOV [EDI], AX
						ADD EDI, 2
						ADD ESI, 2
						DEC ECX
						JNZ LOCK9_MMX_LOOP3
						
		LOCK9_MMX_LABEL2:
						ADD EDI, DestAddPitch
						ADD ESI, SrcAddPitch
						DEC BltHeight
						JNZ LOCK9_MMX_LOOP1
						
						EMMS
					}
				}
				else
				{
					__asm
					{
						MOV EDI, Dest
						MOV ESI, Src
		LOCK9_LOOP1:
						MOV ECX, BltWidth
						SHR ECX, 1
						JZ LOCK9_LABEL1
		LOCK9_LOOP2:
						MOV EAX, [ESI]
						MOV EBX, EAX
						AND EBX, 0xffc0ffc0
						AND EAX, 0x001f001f
						SHR EBX, 1
						OR EAX, EBX
						MOV [EDI], EAX
						ADD EDI, 4
						ADD ESI, 4
						DEC ECX
						JNZ LOCK9_LOOP2

		LOCK9_LABEL1:
						TEST BltWidth, 1
						JZ LOCK9_LABEL2
						MOV AX, [ESI]
						MOV EBX, EAX
						AND EBX, 0xffc0ffc0
						AND EAX, 0x001f001f
						SHR EBX, 1
						OR EAX, EBX
						MOV [EDI], AX
						ADD EDI, 2
						ADD ESI, 2
						
		LOCK9_LABEL2:
						ADD EDI, DestAddPitch
						ADD ESI, SrcAddPitch
						DEC BltHeight
						JNZ LOCK9_LOOP1
					}
				}
#endif
				return 0 ;
			}
		}
		else
		if( TransColorAlphaTestFlag == FALSE &&
			AlphaFlag               == FALSE &&
			SColor.ColorBitDepth    == 16 &&
			SColor.AlphaMask	    == 0x00000000 &&
			SColor.RedMask 	   	    == 0x00007c00 &&
			SColor.GreenMask 	    == 0x000003e0 &&
			SColor.BlueMask 	    == 0x0000001f )
		{
			BYTE *Dest ;
			BYTE *Src ;
			int DestAddPitch, SrcAddPitch ;

			DestAddPitch = DestPitch - BltWidth * DColor.PixelByte ;
			SrcAddPitch  = SrcPitch  - BltWidth * SColor.PixelByte ;

			Dest = ( BYTE * )DestGraphData + DestPoint.y  * DestPitch + DestPoint.x   * DColor.PixelByte ;
			Src  = ( BYTE * )SrcGraphData  + SrcRect->top * SrcPitch  + SrcRect->left * SColor.PixelByte ;

			if( DColor.ColorBitDepth == 16 &&
				DColor.AlphaMask	 == 0x00000000 &&
				DColor.RedMask 		 == 0x0000f800 &&
				DColor.GreenMask 	 == 0x000007e0 &&
				DColor.BlueMask 	 == 0x0000001f )
			{
#ifdef DX_NON_INLINE_ASM
				int i ;
				do
				{
					i = BltWidth >> 1 ;
					if( i != 0 )
					{
						do
						{
							*( ( DWORD * )Dest ) =  
								( ( *( ( DWORD * )Src ) & 0x7fe07fe0 ) << 1 ) |
								( ( *( ( DWORD * )Src ) & 0x003f003f ) ) ;
							Src += 4 ;
							Dest += 4 ;
						}while( -- i != 0 ) ;
					}

					if( ( BltWidth & 1 ) != 0 )
					{
						*( ( DWORD * )Dest ) =  
							( ( *( ( DWORD * )Src ) & 0x7fe0 ) << 1 ) |
							( ( *( ( DWORD * )Src ) & 0x003f ) ) ;
						Src += 2 ;
						Dest += 2 ;
					}

					Dest += DestAddPitch ;
					Src += SrcAddPitch ;

				}while( -- BltHeight != 0 ) ;
#else
				if( NS_GetUseMMXFlag() == TRUE )
				{
					__asm
					{
						MOV   EAX, 0x7fe07fe0
						MOV   EBX, 0x003f003f
						MOVD  MM0, EAX
						MOVD  MM1, EBX
						MOVQ  MM2, MM0
						MOVQ  MM3, MM1
						PSLLQ MM2, 32
						PSLLQ MM3, 32
						POR   MM0, MM2
						POR   MM1, MM3
						MOV EDI, Dest
						MOV ESI, Src
		LOCK10_MMX_LOOP1:
						MOV ECX, BltWidth
						SHR ECX, 2
						JZ LOCK10_MMX_LABEL1
		LOCK10_MMX_LOOP2:
						MOVQ  MM2, [ESI]
						MOVQ  MM3, MM2
						PAND  MM2, MM0
						PAND  MM3, MM1
						PSLLQ MM2, 1
						POR   MM2, MM3
						MOVQ  [EDI], MM2
						ADD EDI, 8
						ADD ESI, 8
						DEC ECX
						JNZ LOCK10_MMX_LOOP2

		LOCK10_MMX_LABEL1:
						MOV ECX, BltWidth
						AND ECX, 3
						JZ LOCK10_MMX_LABEL2
		LOCK10_MMX_LOOP3:
						MOV AX, [ESI]
						MOV EBX, EAX
						AND EBX, 0x7fe07fe0
						AND EAX, 0x003f003f
						SHL EBX, 1
						OR EAX, EBX
						MOV [EDI], AX
						ADD EDI, 2
						ADD ESI, 2
						DEC ECX
						JNZ LOCK10_MMX_LOOP3
						
		LOCK10_MMX_LABEL2:
						ADD EDI, DestAddPitch
						ADD ESI, SrcAddPitch
						DEC BltHeight
						JNZ LOCK10_MMX_LOOP1
						
						EMMS
					}
				}
				else
				{
					__asm
					{
						MOV EDI, Dest
						MOV ESI, Src
		LOCK10_LOOP1:
						MOV ECX, BltWidth
						SHR ECX, 1
						JZ LOCK10_LABEL1
		LOCK10_LOOP2:
						MOV EAX, [ESI]
						MOV EBX, EAX
						AND EBX, 0x7fe07fe0
						AND EAX, 0x003f003f
						SHL EBX, 1
						OR EAX, EBX
						MOV [EDI], EAX
						ADD EDI, 4
						ADD ESI, 4
						DEC ECX
						JNZ LOCK10_LOOP2

		LOCK10_LABEL1:
						TEST BltWidth, 1
						JZ LOCK10_LABEL2

						MOV AX, [ESI]
						MOV EBX, EAX
						AND EBX, 0x7fe07fe0
						AND EAX, 0x003f003f
						SHL EBX, 1
						OR EAX, EBX
						MOV [EDI], AX
						ADD EDI, 2
						ADD ESI, 2
						
		LOCK10_LABEL2:
						ADD EDI, DestAddPitch
						ADD ESI, SrcAddPitch
						DEC BltHeight
						JNZ LOCK10_LOOP1
					}
				}
#endif
				return 0 ;
			}
		}
	}

NORMALMOVE:

	// ������ɂ��Ή����Ă��Ȃ��ꍇ�͕��ʂ̓]���������s��
	{
		DWORD DitherTableRed[16], DitherTableBlue[16], DitherTableGreen[16], DitherTableAlpha[16] ;
		DWORD DiffMaskRed = 0, DiffMaskGreen = 0, DiffMaskBlue = 0, DiffMaskAlpha = 0 ;

		DWORD SrcPitch2, DestPitch2, AlphaPitch2 ;
		DWORD ARGBMask = 0 ;
		DWORD GRed = 0, GGreen = 0 ,GBlue = 0, GAlpha = 0 ;
		DWORD TRed = 0, TGreen = 0, TBlue = 0 ;
		BYTE GraphShavedFlag, DestColorBitDepth, SrcColorBitDepth, AlphaColorBitDepth ;
		BYTE  GWRed    = 0, GWGreen    = 0, GWBlue    = 0, GWAlpha    = 0 ;
		DWORD GRedMask = 0, GGreenMask = 0, GBlueMask = 0, GAlphaMask = 0 ;
		DWORD RedMask  = 0, GreenMask  = 0, BlueMask  = 0, AMask      = 0 ;
		DWORD *SrcPalette, *AlphaPalette ;
		DWORD SrcValidMask ;

		// ���������_�t�H�[�}�b�g�̏ꍇ�� 32bit ARGB�Ƃ���
		// 1�`�����l����16�r�b�g�̐����^�������ꍇ������ 32bit ARGB �Ƃ���
		if( SColor.FloatTypeFlag != 0 ||
			( SColor.FloatTypeFlag == 0 && SColor.ChannelBitDepth == 16 ) )
		{
			SColor.ColorBitDepth = 32 ;

			SColor.RedMask   = 0x00ff0000 ; SColor.RedLoc   = 16 ; SColor.RedWidth   = 8 ;
			SColor.GreenMask = 0x0000ff00 ; SColor.GreenLoc =  8 ; SColor.GreenWidth = 8 ;
			SColor.BlueMask  = 0x000000ff ; SColor.BlueLoc  =  0 ; SColor.BlueWidth  = 8 ;
			SColor.AlphaMask = 0xff000000 ; SColor.AlphaLoc = 24 ; SColor.AlphaWidth = 8 ;
		}
		if( DColor.FloatTypeFlag != 0 ||
			( DColor.FloatTypeFlag == 0 && DColor.ChannelBitDepth == 16 ) )
		{
			DColor.ColorBitDepth = 32 ;

			DColor.RedMask   = 0x00ff0000 ; DColor.RedLoc   = 16 ; DColor.RedWidth   = 8 ;
			DColor.GreenMask = 0x0000ff00 ; DColor.GreenLoc =  8 ; DColor.GreenWidth = 8 ;
			DColor.BlueMask  = 0x000000ff ; DColor.BlueLoc  =  0 ; DColor.BlueWidth  = 8 ;
			DColor.AlphaMask = 0xff000000 ; DColor.AlphaLoc = 24 ; DColor.AlphaWidth = 8 ;
		}

		// �O���t�B�b�N�򉻊ɘa�������[�h�����[�J���ϐ��ɗ��Ƃ�
		GraphShavedFlag = ( BYTE )(
							( SColor.RedWidth   != DColor.RedWidth   ) || 
							( SColor.GreenWidth != DColor.GreenWidth ) || 
							( SColor.RedWidth   != DColor.RedWidth   ) ) ; 
		if( AlphaMaskFlag && AColor.RedWidth != DColor.AlphaWidth ) GraphShavedFlag = TRUE;

		// �]���悪���������_�^�̏ꍇ��1�`�����l����16�r�b�g�̐����^�̏ꍇ�̓O���t�B�b�N�򉻊ɘa�����͎g�p�ł��Ȃ�
		if( DColor.FloatTypeFlag != 0 || 
			( DColor.FloatTypeFlag == 0 && DColor.ChannelBitDepth == 16 ) )
		{
			GraphShavedFlag = FALSE ;
		}

		// ���ߐF��A���t�@�e�X�g��A���t�@�}�X�N�͕��������_�摜��1�`�����l����16�r�b�g�̐����^�̏ꍇ�͎g�p�ł��Ȃ�
		if( SColor.FloatTypeFlag != 0 || DColor.FloatTypeFlag != 0 ||
			( SColor.FloatTypeFlag == 0 && SColor.ChannelBitDepth == 16 ) ||
			( DColor.FloatTypeFlag == 0 && DColor.ChannelBitDepth == 16 ) )
		{
			TransColorNoMoveFlag = FALSE ;
			TransColorAlphaTestFlag = FALSE ;
			AlphaMaskFlag = FALSE ;
		}

		// �p���b�g�Z�b�g
		SrcPalette = (DWORD *)SColor.Palette ;
		AlphaPalette = (DWORD *)AColor.Palette ;

		// �F�����Z�b�g
		SrcColorBitDepth = 0 ;
		if( SColor.FloatTypeFlag != 0 )
		{
			switch( SColor.ChannelBitDepth )
			{
			case 32 : SrcColorBitDepth = 4 ; break ;
			case 16 : SrcColorBitDepth = 5 ; break ;
			}
		}
		else
		if( SColor.ChannelBitDepth == 16 )
		{
			switch( SColor.ChannelBitDepth )
			{
			case 16 : SrcColorBitDepth = 6 ; break ;
			}
		}
		else
		{
			switch( SColor.ColorBitDepth )
			{
			case 16 : SrcColorBitDepth = 0 ; break ;
			case 24 : SrcColorBitDepth = 1 ; break ;
			case 32 : SrcColorBitDepth = 2 ; break ;
			case 8  : SrcColorBitDepth = 3 ; break ;
			}
		}

		// �]�����̗L���r�b�g�̃Z�b�g
		SrcValidMask = SColor.RedMask | SColor.GreenMask | SColor.BlueMask | SColor.AlphaMask ;
		if( TransColorAlphaTestFlag == FALSE )
		{
			switch( SColor.ColorBitDepth )
			{
			case 16 : TransColor &= 0xffff     ; break ;
			case 24 : TransColor &= 0xffffff   ; break ;
			case 32 : TransColor &= 0xffffffff ; break ;
			case 8  : TransColor &= 0xff       ; break ;
			}

			// ���ߐF�̃Z�b�e�B���O
			if( SColor.ColorBitDepth != 8 )
			{
				TransColor &= SrcValidMask ;
			}
		}

		DestColorBitDepth = 0 ;
		if( DColor.FloatTypeFlag != 0 )
		{
			switch( DColor.ChannelBitDepth )
			{
			case 32 : DestColorBitDepth = 4 ; break ;
			case 16 : DestColorBitDepth = 5 ; break ;
			}
		}
		else
		if( DColor.ChannelBitDepth == 16 )
		{
			switch( DColor.ChannelBitDepth )
			{
			case 16 : DestColorBitDepth = 6 ; break ;
			}
		}
		else
		{
			switch( DColor.ColorBitDepth )
			{
			case 16 : DestColorBitDepth = 0 ; break ;
			case 24 : DestColorBitDepth = 1 ; break ;
			case 32 : DestColorBitDepth = 2 ; break ;
			case  8 : DestColorBitDepth = 3 ; break ;
			}
		}
		
		AlphaColorBitDepth = 0 ;
		switch( AColor.ColorBitDepth )
		{
		case 16 : AlphaColorBitDepth = 0 ; break ;
		case 24 : AlphaColorBitDepth = 1 ; break ;
		case 32 : AlphaColorBitDepth = 2 ; break ;
		case  8 : AlphaColorBitDepth = 3 ; break ;
		}

		// �F����������ꍇ�͌��F�ɘa�����_�����O�������s��
		if( GraphShavedFlag == TRUE )
		{
			DWORD DiffRed, DiffGreen, DiffBlue, DiffAlpha ;
			int i ;

			GWRed	= ( BYTE )( SColor.RedWidth   - DColor.RedWidth   ) ;
			GWGreen	= ( BYTE )( SColor.GreenWidth - DColor.GreenWidth ) ;
			GWBlue	= ( BYTE )( SColor.BlueWidth  - DColor.BlueWidth  ) ;
			GWAlpha	= ( BYTE )( ( AlphaMaskFlag ? AColor.RedWidth : SColor.AlphaWidth	) - DColor.AlphaWidth ) ;

			DiffRed   = ( DWORD )( 1 << GWRed   ) ;
			DiffGreen = ( DWORD )( 1 << GWGreen ) ;
			DiffBlue  = ( DWORD )( 1 << GWBlue  ) ;
			DiffAlpha = ( DWORD )( 1 << GWAlpha ) ;

			// ���F�ɘa���[�h���f�B�U�����O�������ꍇ�̓e�[�u���𖄂߂�
			if( ImageShavedMode == DX_SHAVEDMODE_DITHER )
			{
				for( i = 0 ; i < 16 ; i ++ )
				{
					DitherTableRed  [ i ] = ( DiffRed   * DitherTable[ i ] / 16 ) << SColor.RedLoc ;
					DitherTableGreen[ i ] = ( DiffGreen * DitherTable[ i ] / 16 ) << SColor.GreenLoc ;
					DitherTableBlue [ i ] = ( DiffBlue  * DitherTable[ i ] / 16 ) << SColor.BlueLoc ;
					DitherTableAlpha[ i ] = ( DiffAlpha * DitherTable[ i ] / 16 ) << ( AlphaMaskFlag ? AColor.RedLoc : SColor.AlphaLoc ) ;
				}
			}

			DiffMaskRed   = ( DiffRed   - 1 ) << SColor.RedLoc   ;
			DiffMaskGreen = ( DiffGreen - 1 ) << SColor.GreenLoc ;
			DiffMaskBlue  = ( DiffBlue  - 1 ) << SColor.BlueLoc  ;
			DiffMaskAlpha = ( DiffAlpha - 1 ) << ( AlphaMaskFlag ? AColor.RedLoc : SColor.AlphaLoc ) ;

			ARGBMask  = ~( DColor.RedMask | DColor.GreenMask | DColor.BlueMask | DColor.AlphaMask ) ;
			
			RedMask   = SColor.RedMask   >> SColor.RedLoc   ;
			GreenMask = SColor.GreenMask >> SColor.GreenLoc ;
			BlueMask  = SColor.BlueMask  >> SColor.BlueLoc  ;
			AMask     = ( AlphaMaskFlag ? ( AColor.RedMask >> AColor.RedLoc ) : ( SColor.AlphaMask >> SColor.AlphaLoc ) ) ;
			
			GRedMask   = ( DWORD )( ( 1 << ( DColor.RedWidth   - SColor.RedWidth   ) ) - 1 ) ;
			GGreenMask = ( DWORD )( ( 1 << ( DColor.GreenWidth - SColor.GreenWidth ) ) - 1 ) ;
			GBlueMask  = ( DWORD )( ( 1 << ( DColor.BlueWidth  - SColor.BlueWidth  ) ) - 1 ) ;
			GAlphaMask = ( DWORD )( ( 1 << ( DColor.AlphaWidth - ( AlphaMaskFlag ? AColor.RedWidth : SColor.AlphaWidth	) ) )  - 1 ) ;

			GAlpha = GRed = GBlue = GGreen = 0 ;
		}
		
//		DXST_ERRORLOGFMT_ADDW(( L"GWAlpha:%d   GAlphaMask:%x   AlphaMask:%x", GWAlpha, GAlphaMask, AlphaMask )) ;

		// �J���[�L�[���擾
		if( TransColorAlphaTestFlag )
		{
			unsigned int Color ;

			TRed   = ( ( TransColor >> 16 ) & 0xff )  ;
			TGreen = ( ( TransColor >>  8 ) & 0xff )  ;
			TBlue  = ( ( TransColor       ) & 0xff )  ;
			Color  = NS_GetColor3( &SColor, ( int )TRed, ( int )TGreen, ( int )TBlue ) ;
			if( SColor.ColorBitDepth == 8 )
			{
				TRed   = SColor.Palette[ Color ].Red ;
				TGreen = SColor.Palette[ Color ].Green ;
				TBlue  = SColor.Palette[ Color ].Blue ;
			}
			else
			{
				TRed   = ( Color & SColor.RedMask   ) >> SColor.RedLoc ;
				TGreen = ( Color & SColor.GreenMask ) >> SColor.GreenLoc ;
				TBlue  = ( Color & SColor.BlueMask  ) >> SColor.BlueLoc ;
			}
		}

		// �P���C���]������x�ɉ��Z����l�Z�b�g
		if( ReverseFlag == FALSE )
		{
			DestPitch2  = ( DWORD )( DestPitch  - BltWidth * DColor.PixelByte ) ;
			SrcPitch2   = ( DWORD )( SrcPitch   - BltWidth * SColor.PixelByte ) ;
			AlphaPitch2 = ( DWORD )( AlphaPitch - BltWidth * AColor.PixelByte ) ;
		}
		else
		{
			DestPitch2  = ( DWORD )( DestPitch  + BltWidth * DColor.PixelByte ) ;
			SrcPitch2   = ( DWORD )( SrcPitch   - BltWidth * SColor.PixelByte ) ;
			AlphaPitch2 = ( DWORD )( AlphaPitch - BltWidth * AColor.PixelByte ) ;
		}

		// �]������
		{
			BYTE *DBuf, *SBuf, *ABuf ;
			DWORD DestColor = 0, SrcColor  = 0, AlphaColor = 0, NoneMask = 0 ;
			WORD SrcColorR_I = 0, SrcColorG_I = 0, SrcColorB_I = 0, SrcColorA_I = 0 ;
			float SrcColorR_F = 0, SrcColorG_F = 0, SrcColorB_F = 0, SrcColorA_F = 0 ;
			int i, j, DestAdd ;

			// �����r�b�g���Z�b�g
			NoneMask = DColor.NoneMask ;
			
			// ���]�]�����ǂ����ŁA�]����̏����A�h���X�Ȃǂ�ύX
			if( ReverseFlag == TRUE )
			{
				DBuf = ( BYTE * )DestGraphData + DestPoint.y * DestPitch + DestPoint.x * DColor.PixelByte + ( BltWidth - 1 ) * DColor.PixelByte ;
				DestAdd = -DColor.PixelByte ;
			}
			else
			{
				DBuf = ( BYTE * )DestGraphData + DestPoint.y * DestPitch + DestPoint.x * DColor.PixelByte ;
				DestAdd = DColor.PixelByte ;
			}
			
			// �]����ȊO�̃f�[�^�͒ʏ�ʂ�
			SBuf = ( BYTE * )SrcGraphData + SrcRect->top * SrcPitch   + SrcRect->left * SColor.PixelByte ;
			ABuf = ( BYTE * )AlphaMask    + SrcRect->top * AlphaPitch + SrcRect->left * AColor.PixelByte ;

			for( i = 0 ; i < BltHeight ; i ++, DBuf += DestPitch2, SBuf += SrcPitch2, ABuf += AlphaPitch2 )
			{
				for( j = 0 ; j < BltWidth ; j ++, DBuf += DestAdd, SBuf += SColor.PixelByte, ABuf += AColor.PixelByte )
				{
					// �J���[�𓾂�
					switch( SrcColorBitDepth )
					{
					case 0 :	SrcColor = ( DWORD )( *( ( WORD  * )SBuf ) ) ; break ; 
					case 1 :	SrcColor = ( DWORD )( *( ( WORD  * )SBuf ) ) | ( ( DWORD )( SBuf[ 2 ] ) << 16 ) ; break ; 
					case 2 :	SrcColor = ( DWORD )( *( ( DWORD * )SBuf ) ) ; break ;
					case 3 :	SrcColor = ( DWORD )( SrcPalette[*( ( BYTE * )SBuf )] ) ; break ;

					case 4 :
						SrcColorR_F =                         ( ( float * )SBuf )[ 0 ] ;
						SrcColorG_F = SColor.ChannelNum > 1 ? ( ( float * )SBuf )[ 1 ] : 1.0f ;
						SrcColorB_F = SColor.ChannelNum > 2 ? ( ( float * )SBuf )[ 2 ] : 1.0f ;
						SrcColorA_F = SColor.ChannelNum > 3 ? ( ( float * )SBuf )[ 3 ] : 1.0f ;
						break ;

					case 5 :
						SrcColorR_F =                         Float16ToFloat32( ( ( WORD * )SBuf )[ 0 ] ) ;
						SrcColorG_F = SColor.ChannelNum > 1 ? Float16ToFloat32( ( ( WORD * )SBuf )[ 1 ] ) : 1.0f ;
						SrcColorB_F = SColor.ChannelNum > 2 ? Float16ToFloat32( ( ( WORD * )SBuf )[ 2 ] ) : 1.0f ;
						SrcColorA_F = SColor.ChannelNum > 3 ? Float16ToFloat32( ( ( WORD * )SBuf )[ 3 ] ) : 1.0f ;
						break ;

					case 6 :
						SrcColorR_I =                         ( ( WORD * )SBuf )[ 0 ] ;
						SrcColorG_I = SColor.ChannelNum > 1 ? ( ( WORD * )SBuf )[ 1 ] : 0xffff ;
						SrcColorB_I = SColor.ChannelNum > 2 ? ( ( WORD * )SBuf )[ 2 ] : 0xffff ;
						SrcColorA_I = SColor.ChannelNum > 3 ? ( ( WORD * )SBuf )[ 3 ] : 0xffff ;
						break ;
					}

					if( SColor.FloatTypeFlag != 0 )
					{
						if( DColor.FloatTypeFlag == 0 )
						{
							if( DColor.ChannelBitDepth == 16 )
							{
								SrcColorR_I = 0 ;
								SrcColorG_I = 0 ;
								SrcColorB_I = 0 ;
								SrcColorA_I = 0 ;

									 if( SrcColorR_F > 1.0f ) SrcColorR_I = 0xffff ;
								else if( SrcColorR_F > 0.0f ) SrcColorR_I = ( WORD )_FTOL( SrcColorR_F * 0xffff ) ;

									 if( SrcColorG_F > 1.0f ) SrcColorG_I = 0xffff ;
								else if( SrcColorG_F > 0.0f ) SrcColorG_I = ( WORD )_FTOL( SrcColorG_F * 0xffff ) ;

									 if( SrcColorB_F > 1.0f ) SrcColorB_I = 0xffff ;
								else if( SrcColorB_F > 0.0f ) SrcColorB_I = ( WORD )_FTOL( SrcColorB_F * 0xffff ) ;

									 if( SrcColorA_F > 1.0f ) SrcColorA_I = 0xffff ;
								else if( SrcColorA_F > 0.0f ) SrcColorA_I = ( WORD )_FTOL( SrcColorA_F * 0xffff ) ;
							}
							else
							{
								SrcColor = 0 ;

									 if( SrcColorR_F > 1.0f ) SrcColor |= ( DWORD )0xff << SColor.RedLoc ;
								else if( SrcColorR_F > 0.0f ) SrcColor |= ( DWORD )( _FTOL( SrcColorR_F * 255.0f ) << SColor.RedLoc ) ;

									 if( SrcColorG_F > 1.0f ) SrcColor |= ( DWORD )0xff << SColor.GreenLoc ;
								else if( SrcColorG_F > 0.0f ) SrcColor |= ( DWORD )( _FTOL( SrcColorG_F * 255.0f ) << SColor.GreenLoc ) ;

									 if( SrcColorB_F > 1.0f ) SrcColor |= ( DWORD )0xff << SColor.BlueLoc ;
								else if( SrcColorB_F > 0.0f ) SrcColor |= ( DWORD )( _FTOL( SrcColorB_F * 255.0f ) << SColor.BlueLoc ) ;

									 if( SrcColorA_F > 1.0f ) SrcColor |= ( DWORD )0xff << SColor.AlphaLoc ;
								else if( SrcColorA_F > 0.0f ) SrcColor |= ( DWORD )( _FTOL( SrcColorA_F * 255.0f ) << SColor.AlphaLoc ) ;
							}
						}
					}
					else
					if( SColor.ChannelBitDepth == 16 )
					{
						if( DColor.FloatTypeFlag != 0 )
						{
							SrcColorR_F = SrcColorR_I / 65535.0f ;
							SrcColorG_F = SrcColorG_I / 65535.0f ;
							SrcColorB_F = SrcColorB_I / 65535.0f ;
							SrcColorA_F = SrcColorA_I / 65535.0f ;
						}
						else
						if( DColor.ChannelBitDepth != 16 )
						{
							SrcColor = 0 ;

							SrcColor |= ( DWORD )( ( SrcColorR_I >> 8 ) << SColor.RedLoc ) ;
							SrcColor |= ( DWORD )( ( SrcColorG_I >> 8 ) << SColor.GreenLoc ) ;
							SrcColor |= ( DWORD )( ( SrcColorB_I >> 8 ) << SColor.BlueLoc ) ;
							SrcColor |= ( DWORD )( ( SrcColorA_I >> 8 ) << SColor.AlphaLoc ) ;
						}
					}
					else
					{
						if( DColor.FloatTypeFlag != 0 )
						{
							SrcColorR_F = SColor.RedWidth   == 0 ? 1.0f : ( float )( ( SrcColor & SColor.RedMask   ) >> SColor.RedLoc   ) / ( SColor.RedMask   >> SColor.RedLoc   ) ;
							SrcColorG_F = SColor.GreenWidth == 0 ? 1.0f : ( float )( ( SrcColor & SColor.GreenMask ) >> SColor.GreenLoc ) / ( SColor.GreenMask >> SColor.GreenLoc ) ;
							SrcColorB_F = SColor.BlueWidth  == 0 ? 1.0f : ( float )( ( SrcColor & SColor.BlueMask  ) >> SColor.BlueLoc  ) / ( SColor.BlueMask  >> SColor.BlueLoc  ) ;
							SrcColorA_F = SColor.AlphaWidth == 0 ? 1.0f : ( float )( ( SrcColor & SColor.AlphaMask ) >> SColor.AlphaLoc ) / ( SColor.AlphaMask >> SColor.AlphaLoc ) ;
						}
						else
						if( DColor.ChannelBitDepth > 8 )
						{
							SrcColorR_I = SColor.RedWidth   == 0 ? 0xffff : ( WORD )( ( ( SrcColor & SColor.RedMask   ) >> SColor.RedLoc   ) * 0xffff / ( SColor.RedMask   >> SColor.RedLoc   ) ) ;
							SrcColorG_I = SColor.GreenWidth == 0 ? 0xffff : ( WORD )( ( ( SrcColor & SColor.GreenMask ) >> SColor.GreenLoc ) * 0xffff / ( SColor.GreenMask >> SColor.GreenLoc ) ) ;
							SrcColorB_I = SColor.BlueWidth  == 0 ? 0xffff : ( WORD )( ( ( SrcColor & SColor.BlueMask  ) >> SColor.BlueLoc  ) * 0xffff / ( SColor.BlueMask  >> SColor.BlueLoc  ) ) ;
							SrcColorA_I = SColor.AlphaWidth == 0 ? 0xffff : ( WORD )( ( ( SrcColor & SColor.AlphaMask ) >> SColor.AlphaLoc ) * 0xffff / ( SColor.AlphaMask >> SColor.AlphaLoc ) ) ;
						}
					}

					// ���ߐF����
					if( TransColorNoMoveFlag )
					{
						if( SrcColorBitDepth != 3 )
						{
							if( ( SrcColor & SrcValidMask ) == TransColor )
								continue ;
						}
						else
						if( SrcColor == TransColor ) continue ;
					}

					// �A���t�@�}�X�N����A���t�@�l�����ꍇ�̓A���t�@�l��
					if( AlphaMaskFlag == TRUE )
					{
						switch( AlphaColorBitDepth )
						{
						case 0 :	AlphaColor = ( DWORD )( *( ( WORD * )ABuf ) ) ; break ; 
						case 1 :	AlphaColor = ( DWORD )( *( ( WORD * )ABuf ) ) | ( ( DWORD )( ABuf[ 2 ] ) << 16 ) ; break ; 
						case 2 :	AlphaColor = ( DWORD )( *( ( DWORD * )ABuf ) ) ; break ;
						case 3 :	AlphaColor = ( DWORD )( AlphaPalette[*( ( BYTE * )ABuf )] ) ; break ;
						}
					}

					// ���F����
					if( DColor.FloatTypeFlag == FALSE )
					{
						DWORD Red, Blue, Green, Alpha ;

						Red   = ( SrcColor & SColor.RedMask   ) >> SColor.RedLoc ;
						Green = ( SrcColor & SColor.GreenMask ) >> SColor.GreenLoc ;
						Blue  = ( SrcColor & SColor.BlueMask  ) >> SColor.BlueLoc ;

						if( DColor.AlphaWidth != 0 && TransColorAlphaTestFlag == TRUE )
						{
							if( Red == TRed && Green == TGreen && Blue == TBlue )
								Alpha = 0 ;
							else
								Alpha = DColor.AlphaMask >> DColor.AlphaLoc ;
						}
						else
						{
							if( AlphaMaskFlag == TRUE )
							{
								Alpha = ( AlphaColor & AColor.RedMask ) >> AColor.RedLoc ;
								if( GWAlpha ){ if( GWAlpha & 0x80 ) Alpha = ( Alpha << ( ~GWAlpha + 1 ) ) + ( Alpha * GAlphaMask / AMask ) ; else Alpha >>= GWAlpha ; }
							}
							else
							{
								if( SColor.AlphaWidth == 0 )
								{
									Alpha = DColor.AlphaMask >> DColor.AlphaLoc ;
								}
								else
								if( DColor.AlphaMask == 0 )
								{
									Alpha = 0 ;
								}
								else
								{
									Alpha = ( SrcColor & SColor.AlphaMask ) >> SColor.AlphaLoc ;
									if( GWAlpha ){ if( GWAlpha & 0x80 ) Alpha = ( Alpha << ( ~GWAlpha + 1 ) ) + ( Alpha * GAlphaMask / AMask ) ; else Alpha >>= GWAlpha ; }
								}
							}

							if( RedIsAlphaFlag ) Alpha = Red ;
						}

						if( GWRed   && RedMask   != 0 ){ if( GWRed   & 0x80 ) Red   = ( Red   << ( ~GWRed   + 1 ) ) + ( Red   * GRedMask   / RedMask   ) ; else Red   >>= GWRed   ; }
						if( GWGreen && GreenMask != 0 ){ if( GWGreen & 0x80 ) Green = ( Green << ( ~GWGreen + 1 ) ) + ( Green * GGreenMask / GreenMask ) ; else Green >>= GWGreen ; }
						if( GWBlue  && BlueMask  != 0 ){ if( GWBlue  & 0x80 ) Blue  = ( Blue  << ( ~GWBlue  + 1 ) ) + ( Blue  * GBlueMask  / BlueMask  ) ; else Blue  >>= GWBlue  ; }

						if( AlphaOnlyFlag ) Green = Blue = Red = 0 ;
						DestColor = ( Red   << DColor.RedLoc   ) |
									( Blue  << DColor.BlueLoc  ) |
									( Green << DColor.GreenLoc ) |
									( Alpha << DColor.AlphaLoc ) ;
					}

					// �摜�򉻊ɘa����
					if( GraphShavedFlag )
					{
						DWORD Red, Green, Blue, Alpha ;

						Red   = DestColor & DColor.RedMask ;
						Green = DestColor & DColor.GreenMask ;
						Blue  = DestColor & DColor.BlueMask ;
						Alpha = DestColor & DColor.AlphaMask ;

						switch( ImageShavedMode )
						{
						case DX_SHAVEDMODE_DITHER :		// �f�B�U�����O�ɂ��ɘa����
							Red   += ( ( SrcColor & DiffMaskRed   ) > DitherTableRed  [ ( ( i & 3 ) << 2 ) + ( j & 3 ) ] ) << DColor.RedLoc ;
							Green += ( ( SrcColor & DiffMaskGreen ) > DitherTableGreen[ ( ( i & 3 ) << 2 ) + ( j & 3 ) ] ) << DColor.GreenLoc ;
							Blue  += ( ( SrcColor & DiffMaskBlue  ) > DitherTableBlue [ ( ( i & 3 ) << 2 ) + ( j & 3 ) ] ) << DColor.BlueLoc ;
							Alpha += ( ( SrcColor & DiffMaskAlpha ) > DitherTableAlpha[ ( ( i & 3 ) << 2 ) + ( j & 3 ) ] ) << DColor.AlphaLoc ;
							break ;

						case DX_SHAVEDMODE_DIFFUS :		// �덷�g�U�ɂ��ɘa����
							GRed   += SrcColor & DiffMaskRed ;
							GGreen += SrcColor & DiffMaskGreen ;
							GBlue  += SrcColor & DiffMaskBlue ;
							GAlpha += SrcColor & DiffMaskAlpha ;

							Red   += ( GRed   > DiffMaskRed   ) << DColor.RedLoc   ;
							Green += ( GGreen > DiffMaskGreen ) << DColor.GreenLoc ;
							Blue  += ( GBlue  > DiffMaskBlue  ) << DColor.BlueLoc  ;
							Alpha += ( GAlpha > DiffMaskAlpha ) << DColor.AlphaLoc ;

							GRed   &= DiffMaskRed ;
							GGreen &= DiffMaskGreen ;
							GBlue  &= DiffMaskBlue ;
							GAlpha &= DiffMaskAlpha ;
							break ;
						}

						if( Red   > ( DWORD )DColor.RedMask   ) Red   = ( DWORD )DColor.RedMask ;
						if( Green > ( DWORD )DColor.GreenMask ) Green = ( DWORD )DColor.GreenMask ;
						if( Blue  > ( DWORD )DColor.BlueMask  ) Blue  = ( DWORD )DColor.BlueMask ;
						if( Alpha > ( DWORD )DColor.AlphaMask ) Alpha = ( DWORD )DColor.AlphaMask ;

						DestColor = ( DestColor & ARGBMask ) | Red | Blue | Green | Alpha ;
					}

					// �]��
					switch( DestColorBitDepth )
					{
					case 0 :	*( ( WORD  * )DBuf ) = ( WORD )( DestColor | NoneMask ) ; break ; 
					case 1 :	*( ( WORD  * )DBuf ) = ( WORD )DestColor  ; *( ( BYTE * )( DBuf + 2 ) ) = ( BYTE )( DestColor >> 16 ) ; break ; 
					case 2 :	*( ( DWORD * )DBuf ) = ( DWORD )( DestColor | NoneMask ) ; break ;
					case 3 :	*( ( BYTE  * )DBuf ) = ( BYTE )DestColor ; break ;

					case 4 :
						                            ( ( float * )DBuf )[ 0 ] = SrcColorR_F ;
						if( DColor.ChannelNum > 1 ) ( ( float * )DBuf )[ 1 ] = SrcColorG_F ;
						if( DColor.ChannelNum > 2 ) ( ( float * )DBuf )[ 2 ] = SrcColorB_F ;
						if( DColor.ChannelNum > 3 ) ( ( float * )DBuf )[ 3 ] = SrcColorA_F ;
						break ;

					case 5 :
						                            ( ( WORD * )DBuf )[ 0 ] = Float32ToFloat16( SrcColorR_F ) ;
						if( DColor.ChannelNum > 1 ) ( ( WORD * )DBuf )[ 1 ] = Float32ToFloat16( SrcColorG_F ) ;
						if( DColor.ChannelNum > 2 ) ( ( WORD * )DBuf )[ 2 ] = Float32ToFloat16( SrcColorB_F ) ;
						if( DColor.ChannelNum > 3 ) ( ( WORD * )DBuf )[ 3 ] = Float32ToFloat16( SrcColorA_F ) ;
						break ;

					case 6 :
						                            ( ( WORD * )DBuf )[ 0 ] = SrcColorR_I ;
						if( DColor.ChannelNum > 1 ) ( ( WORD * )DBuf )[ 1 ] = SrcColorG_I ;
						if( DColor.ChannelNum > 2 ) ( ( WORD * )DBuf )[ 2 ] = SrcColorB_I ;
						if( DColor.ChannelNum > 3 ) ( ( WORD * )DBuf )[ 3 ] = SrcColorA_I ;
						break ;
					}
				}
			}
		}
	}

	// �I��
	return 0 ;
}











// �F���擾�֌W
extern COLOR_F NS_GetColorF( float Red, float Green, float Blue, float Alpha )
{
	COLOR_F Ret = { Red,  Green,  Blue,  Alpha } ;
	return Ret ;
}

extern COLOR_U8 NS_GetColorU8( int Red, int Green, int Blue, int Alpha )
{
	COLOR_U8 Ret = { ( BYTE )Blue, ( BYTE )Green, ( BYTE )Red, ( BYTE )Alpha } ;
	return Ret ;
}

// DrawPixel ���̕`��֐��Ŏg�p����J���[�l���擾����
extern	unsigned int	NS_GetColor( int Red, int Green, int Blue )
{
#ifndef DX_NON_GRAPHICS
	const COLORDATA *ColorData ;

	// �F����Ԃ�
	if( GSYS.Setting.ValidHardware == TRUE )
	{
//		ColorData = Graphics_Hardware_GetMainColorData_PF() ;
		return 0xff000000 | ( ( ( unsigned int )Red ) << 16 ) | ( ( ( unsigned int )Green ) << 8 ) | ( ( unsigned int )Blue ) ;
	}
	else
	{
		ColorData = GetMemImgColorData( GSYS.Screen.MainScreenColorBitDepth == 16 ? 0 : 1, FALSE, FALSE ) ;
	}
	
	return ColorData->NoneMask +
				( ( ( BYTE )Red   >> ( 8 - ColorData->RedWidth   ) ) << ColorData->RedLoc   ) +
				( ( ( BYTE )Green >> ( 8 - ColorData->GreenWidth ) ) << ColorData->GreenLoc ) +
				( ( ( BYTE )Blue  >> ( 8 - ColorData->BlueWidth  ) ) << ColorData->BlueLoc  ) ;
#else // DX_NON_GRAPHICS
	return 0 ;
#endif // DX_NON_GRAPHICS
}

// �J���[�l����ԁA�΁A�A�A���t�@�̒l���擾����
extern	int		NS_GetColor2( unsigned int Color, int *Red, int *Green, int *Blue )
{
#ifndef DX_NON_GRAPHICS
	const COLORDATA *ColorData ;
	int MaxRed ;
	int MaxGreen ;
	int MaxBlue ;

	// �F����Ԃ�
	if( GSYS.Setting.ValidHardware == TRUE )
	{
//		ColorData = Graphics_Hardware_GetMainColorData_PF() ;
		*Red   = ( int )( ( Color >> 16 ) & 0xff ) ;
		*Green = ( int )( ( Color >>  8 ) & 0xff ) ;
		*Blue  = ( int )( ( Color       ) & 0xff ) ;

		return 0 ;
	}
	else
	{
		ColorData = GetMemImgColorData( GSYS.Screen.MainScreenColorBitDepth == 16 ? 0 : 1, FALSE, FALSE ) ;
	}

	MaxRed		= ( 1 << ColorData->RedWidth   ) - 1 ; 
	MaxGreen	= ( 1 << ColorData->GreenWidth ) - 1 ;
	MaxBlue		= ( 1 << ColorData->BlueWidth  ) - 1 ; 

	// �F�����i�[����
	*Red	= ( int )( ( ( Color & ColorData->RedMask   ) >> ColorData->RedLoc   ) * 255 / MaxRed   ) ;
	*Green	= ( int )( ( ( Color & ColorData->GreenMask ) >> ColorData->GreenLoc ) * 255 / MaxGreen ) ;
	*Blue	= ( int )( ( ( Color & ColorData->BlueMask  ) >> ColorData->BlueLoc  ) * 255 / MaxBlue  ) ;

	// �I��
	return 0 ;
#else // DX_NON_GRAPHICS
	return -1 ;
#endif // DX_NON_GRAPHICS
}

// �w��̃s�N�Z���t�H�[�}�b�g�ɑΉ������J���[�l�𓾂�
extern unsigned int NS_GetColor3( const COLORDATA * ColorData, int Red, int Green, int Blue, int Alpha )
{
	// 8�r�b�g�J���[�̏ꍇ�̓p���b�g�����ԋ߂��F��T��
	if( ColorData->PixelByte == 1 )
	{
		int RedData, BlueData, GreenData ;
		unsigned int ColorCode ;
		unsigned int MinColor = 0xffffff;
		int MinIndex = 0 ;
		int i ;
		const COLORPALETTEDATA *PalData ;

		// ��ԋ߂��F��Ԃ�
		PalData = ColorData->Palette ;
		for( i = 0 ; i < 256 ; i ++, PalData ++ )
		{
			RedData   = ( int )PalData->Red   - Red ;
			GreenData = ( int )PalData->Green - Green ;
			BlueData  = ( int )PalData->Blue  - Blue ;

			ColorCode = ( unsigned int )( RedData * RedData + GreenData * GreenData + BlueData * BlueData ) ;
			if( ColorCode < MinColor ) 
			{
				MinIndex = i ;
				MinColor = ColorCode ;
			}
		}
		
		return ( unsigned int )MinIndex ;
	}
	else
	{
		// �F����Ԃ�
		return ColorData->NoneMask +
				( ( ( BYTE )Red   >> ( 8 - ColorData->RedWidth   ) ) << ColorData->RedLoc   ) +
				( ( ( BYTE )Green >> ( 8 - ColorData->GreenWidth ) ) << ColorData->GreenLoc ) +
				( ( ( BYTE )Blue  >> ( 8 - ColorData->BlueWidth  ) ) << ColorData->BlueLoc  ) +
				( ColorData->AlphaWidth != 0 ? ( ( ( BYTE )Alpha >> ( 8 - ColorData->AlphaWidth ) ) << ColorData->AlphaLoc ) : 0 ) ;
	}
}

// �w��̃J���[�t�H�[�}�b�g�̃J���[�l��ʂ̃J���[�t�H�[�}�b�g�̃J���[�l�ɕϊ�����
extern unsigned int NS_GetColor4( const COLORDATA * DestColorData, const COLORDATA * SrcColorData, unsigned int SrcColor )
{
	unsigned int Red, Blue, Green, Alpha ;
	unsigned int i ;

	// �t�H�[�}�b�g�������ꍇ�͉��������ɕԂ�
	if( SrcColorData->ColorBitDepth == DestColorData->ColorBitDepth &&
		SrcColorData->RedMask       == DestColorData->RedMask       &&
		SrcColorData->GreenMask     == DestColorData->GreenMask     &&
		SrcColorData->BlueMask      == DestColorData->BlueMask      &&
		SrcColorData->AlphaMask     == DestColorData->AlphaMask )
	{
		return DestColorData->NoneMask | SrcColor ;
	}
	else
	{
		Red = ( SrcColor & SrcColorData->RedMask ) >> SrcColorData->RedLoc ;
		i = ( DWORD )( SrcColorData->RedWidth - DestColorData->RedWidth ) ;
		if( i != 0 )
		{
			if( i & 0x80000000 )
			{
				Red <<= ~i + 1 ;
			}
			else
			{
				Red >>= i ;
			}
		}

		Green = ( SrcColor & SrcColorData->GreenMask ) >> SrcColorData->GreenLoc ;
		i = ( DWORD )( SrcColorData->GreenWidth - DestColorData->GreenWidth ) ;
		if( i != 0 )
		{
			if( i & 0x80000000 )
			{
				Green <<= ~i + 1 ;
			}
			else
			{
				Green >>= i ;
			}
		}

		Blue = ( SrcColor & SrcColorData->BlueMask ) >> SrcColorData->BlueLoc ;
		i = ( DWORD )( SrcColorData->BlueWidth - DestColorData->BlueWidth ) ;
		if( i != 0 )
		{
			if( i & 0x80000000 )
			{
				Blue <<= ~i + 1 ;
			}
			else
			{
				Blue >>= i ;
			}
		}

		Alpha = ( SrcColor & SrcColorData->AlphaMask ) >> SrcColorData->AlphaLoc ;
		i = ( DWORD )( SrcColorData->AlphaWidth - DestColorData->AlphaWidth ) ;
		if( i != 0 )
		{
			if( i & 0x80000000 )
			{
				Alpha <<= ~i + 1 ;
			}
			else
			{
				Alpha >>= i ;
			}
		}

		if( SrcColorData->AlphaWidth == 0 )
		{
			Alpha = DestColorData->AlphaMask >> DestColorData->AlphaLoc ;
		}

		// �l��Ԃ�
		return DestColorData->NoneMask |
					( Red   << DestColorData->RedLoc ) |
					( Blue  << DestColorData->BlueLoc ) |
					( Green << DestColorData->GreenLoc ) |
					( Alpha << DestColorData->AlphaLoc ) ;
	}
}

// �w��̃J���[�t�H�[�}�b�g�̃J���[�l��ԁA�΁A�A�A���t�@�̒l���擾����
extern int NS_GetColor5( const COLORDATA * ColorData, unsigned int Color, int *Red, int *Green, int *Blue, int *Alpha )
{
	DWORD MaxRed, MaxGreen, MaxBlue, MaxAlpha ;

	// �e�r�b�g���W�r�b�g��������ȗ�����
	if( ColorData->RedWidth   == 8 &&
		ColorData->GreenWidth == 8 &&
		ColorData->BlueWidth  == 8 )
	{
		// �F�����i�[����
		if( Red   != NULL )	*Red	= ( int )( ( Color & ColorData->RedMask   ) >> ColorData->RedLoc   ) ;
		if( Green != NULL )	*Green	= ( int )( ( Color & ColorData->GreenMask ) >> ColorData->GreenLoc ) ;
		if( Blue  != NULL )	*Blue	= ( int )( ( Color & ColorData->BlueMask  ) >> ColorData->BlueLoc  ) ;

		if( ColorData->AlphaWidth != 0 )
		{
			if( ColorData->AlphaWidth == 8 )
			{
				if( Alpha != NULL )	*Alpha = ( int )( ( Color & ColorData->AlphaMask ) >> ColorData->AlphaLoc ) ;
			}
			else
			{
				MaxAlpha = ( DWORD )( ( 1 << ColorData->AlphaWidth	) - 1 ) ;
				if( Alpha != NULL ) *Alpha = ( int )( ColorData->AlphaWidth != 0 ? ( ( Color & ColorData->AlphaMask	) >> ColorData->AlphaLoc ) * 255 / MaxAlpha : 0 ) ;
			}
		}
	}
	else
	{
		// �W�r�b�g�ȏゾ�����ꍇ�̏���
		MaxRed		= ( DWORD )( ( 1 << ColorData->RedWidth		) - 1 ) ;
		MaxGreen	= ( DWORD )( ( 1 << ColorData->GreenWidth	) - 1 ) ;
		MaxBlue		= ( DWORD )( ( 1 << ColorData->BlueWidth	) - 1 ) ;
		MaxAlpha	= ( DWORD )( ( 1 << ColorData->AlphaWidth	) - 1 ) ;

		// �F�����i�[����
		if( Red   != NULL )	*Red	= ( int )( ( ( Color & ColorData->RedMask	) >> ColorData->RedLoc		) * 255 / MaxRed   ) ;
		if( Green != NULL )	*Green	= ( int )( ( ( Color & ColorData->GreenMask	) >> ColorData->GreenLoc	) * 255 / MaxGreen ) ;
		if( Blue  != NULL )	*Blue	= ( int )( ( ( Color & ColorData->BlueMask	) >> ColorData->BlueLoc		) * 255 / MaxBlue  ) ;
		if( Alpha != NULL ) *Alpha	= ( int )( ColorData->AlphaWidth != 0 ? ( ( Color & ColorData->AlphaMask	) >> ColorData->AlphaLoc ) * 255 / MaxAlpha : 0 ) ;
	}

	// �I��
	return 0 ;
}

// �p���b�g�J���[�̃J���[�����\�z����
extern int NS_CreatePaletteColorData( COLORDATA * ColorDataBuf )
{
	ColorDataBuf->Format = DX_BASEIMAGE_FORMAT_NORMAL ;
	ColorDataBuf->ChannelNum = 0 ;
	ColorDataBuf->ChannelBitDepth = 0 ;
	ColorDataBuf->FloatTypeFlag = FALSE ;
	ColorDataBuf->ColorBitDepth = 8 ;
	ColorDataBuf->PixelByte = 1 ;
	ColorDataBuf->MaxPaletteNo = 255 ;

	ColorDataBuf->NoneMask = 0xff000000 ;

	ColorDataBuf->AlphaLoc = 0 ;
	ColorDataBuf->AlphaMask = 0 ;
	ColorDataBuf->AlphaWidth = 0 ;

	ColorDataBuf->RedLoc = 16 ;
	ColorDataBuf->RedMask = 0x00ff0000 ;
	ColorDataBuf->RedWidth = 8 ;

	ColorDataBuf->GreenLoc = 8 ;
	ColorDataBuf->GreenMask = 0x0000ff00 ;
	ColorDataBuf->GreenWidth = 8 ;

	ColorDataBuf->BlueLoc = 0  ;
	ColorDataBuf->BlueMask = 0x000000ff ;
	ColorDataBuf->BlueWidth = 8 ;

	// �I��
	return 0 ;
}

// �`�q�f�a�e�`�����l�� 32bit ���������_�^�J���[�̃J���[�t�H�[�}�b�g���\�z����
extern int NS_CreateARGBF32ColorData( COLORDATA *ColorDataBuf )
{
	_MEMSET( ColorDataBuf, 0, sizeof( COLORDATA ) ) ;

	// �f�[�^�̃Z�b�g
	ColorDataBuf->Format = DX_BASEIMAGE_FORMAT_NORMAL ;

	ColorDataBuf->ChannelBitDepth = ( unsigned char )32 ;
	ColorDataBuf->ChannelNum      = ( unsigned char )4 ;
	ColorDataBuf->FloatTypeFlag   = ( unsigned char )TRUE ;
	ColorDataBuf->PixelByte       = ( unsigned char )( ColorDataBuf->ChannelBitDepth * ColorDataBuf->ChannelNum / 8 ) ;

	// �I��
	return 0 ;
}

// �`�q�f�a�e�`�����l�� 16bit ���������_�^�J���[�̃J���[�t�H�[�}�b�g���\�z����
extern int NS_CreateARGBF16ColorData( COLORDATA *ColorDataBuf )
{
	_MEMSET( ColorDataBuf, 0, sizeof( COLORDATA ) ) ;

	// �f�[�^�̃Z�b�g
	ColorDataBuf->Format = DX_BASEIMAGE_FORMAT_NORMAL ;

	ColorDataBuf->ChannelBitDepth = ( unsigned char )16 ;
	ColorDataBuf->ChannelNum      = ( unsigned char )4 ;
	ColorDataBuf->FloatTypeFlag   = ( unsigned char )TRUE ;
	ColorDataBuf->PixelByte       = ( unsigned char )( ColorDataBuf->ChannelBitDepth * ColorDataBuf->ChannelNum / 8 ) ;

	// �I��
	return 0 ;
}

// �w�q�f�a�W�J���[�̃J���[�����\�z����
extern	int NS_CreateXRGB8ColorData( COLORDATA * ColorDataBuf )
{
	ColorDataBuf->Format = DX_BASEIMAGE_FORMAT_NORMAL ;
	ColorDataBuf->ChannelNum = 0 ;
	ColorDataBuf->ChannelBitDepth = 0 ;
	ColorDataBuf->FloatTypeFlag = FALSE ;
	ColorDataBuf->ColorBitDepth = 32 ;
	ColorDataBuf->PixelByte = 4 ;
	ColorDataBuf->MaxPaletteNo = 0 ;

	ColorDataBuf->NoneLoc = 24 ;
	ColorDataBuf->NoneMask = 0xff000000 ;
	ColorDataBuf->NoneWidth = 8 ;

	ColorDataBuf->AlphaLoc = 0 ;
	ColorDataBuf->AlphaMask = 0 ;
	ColorDataBuf->AlphaWidth = 0 ;

	ColorDataBuf->RedLoc = 16 ;
	ColorDataBuf->RedMask = 0x00ff0000 ;
	ColorDataBuf->RedWidth = 8 ;

	ColorDataBuf->GreenLoc = 8 ;
	ColorDataBuf->GreenMask = 0x0000ff00 ;
	ColorDataBuf->GreenWidth = 8 ;

	ColorDataBuf->BlueLoc = 0  ;
	ColorDataBuf->BlueMask = 0x000000ff ;
	ColorDataBuf->BlueWidth = 8 ;

	// �I��
	return 0 ;
}

// �`�q�f�a�W�J���[�̃J���[�����\�z����
extern	int NS_CreateARGB8ColorData( COLORDATA * ColorDataBuf )
{
	ColorDataBuf->Format = DX_BASEIMAGE_FORMAT_NORMAL ;
	ColorDataBuf->ChannelNum = 0 ;
	ColorDataBuf->ChannelBitDepth = 0 ;
	ColorDataBuf->FloatTypeFlag = FALSE ;
	ColorDataBuf->ColorBitDepth = 32 ;
	ColorDataBuf->PixelByte = 4 ;
	ColorDataBuf->MaxPaletteNo = 0 ;

	ColorDataBuf->NoneMask = 0x00000000 ;

	ColorDataBuf->AlphaLoc = 24 ;
	ColorDataBuf->AlphaMask = 0xff000000 ;
	ColorDataBuf->AlphaWidth = 8 ;

	ColorDataBuf->RedLoc = 16 ;
	ColorDataBuf->RedMask = 0x00ff0000 ;
	ColorDataBuf->RedWidth = 8 ;

	ColorDataBuf->GreenLoc = 8 ;
	ColorDataBuf->GreenMask = 0x0000ff00 ;
	ColorDataBuf->GreenWidth = 8 ;

	ColorDataBuf->BlueLoc = 0  ;
	ColorDataBuf->BlueMask = 0x000000ff ;
	ColorDataBuf->BlueWidth = 8 ;

	// �I��
	return 0 ;
}

// �`�q�f�a�S�J���[�̃J���[�����\�z����
extern	int NS_CreateARGB4ColorData( COLORDATA * ColorDataBuf )
{
	ColorDataBuf->Format = DX_BASEIMAGE_FORMAT_NORMAL ;
	ColorDataBuf->ChannelNum = 0 ;
	ColorDataBuf->ChannelBitDepth = 0 ;
	ColorDataBuf->FloatTypeFlag = FALSE ;
	ColorDataBuf->ColorBitDepth = 16 ;
	ColorDataBuf->PixelByte = 2 ;
	ColorDataBuf->MaxPaletteNo = 0 ;

	ColorDataBuf->NoneMask = 0x00000000 ;

	ColorDataBuf->AlphaLoc = 12 ;
	ColorDataBuf->AlphaMask = 0x0000f000 ;
	ColorDataBuf->AlphaWidth = 4 ;

	ColorDataBuf->RedLoc = 8 ;
	ColorDataBuf->RedMask = 0x00000f00 ;
	ColorDataBuf->RedWidth = 4 ;

	ColorDataBuf->GreenLoc = 4 ;
	ColorDataBuf->GreenMask = 0x000000f0 ;
	ColorDataBuf->GreenWidth = 4 ;

	ColorDataBuf->BlueLoc = 0  ;
	ColorDataBuf->BlueMask = 0x0000000f ;
	ColorDataBuf->BlueWidth = 4 ;

	// �I��
	return 0 ;
}


// �t���J���[�c�h�a�̃J���[�����\�z����
extern	int	NS_CreateFullColorData( COLORDATA * ColorDataBuf )
{
	ColorDataBuf->Format = DX_BASEIMAGE_FORMAT_NORMAL ;
	ColorDataBuf->ChannelNum = 0 ;
	ColorDataBuf->ChannelBitDepth = 0 ;
	ColorDataBuf->FloatTypeFlag = FALSE ;
	ColorDataBuf->ColorBitDepth = 24 ;
	ColorDataBuf->PixelByte = 3 ;
	ColorDataBuf->MaxPaletteNo = 0 ;

	ColorDataBuf->NoneMask = 0x00000000 ;

	ColorDataBuf->AlphaLoc = 0 ;
	ColorDataBuf->AlphaMask = 0 ;
	ColorDataBuf->AlphaWidth = 0 ;

	ColorDataBuf->RedLoc = 16 ;
	ColorDataBuf->RedMask = 0xff0000 ;
	ColorDataBuf->RedWidth = 8 ;

	ColorDataBuf->GreenLoc = 8 ;
	ColorDataBuf->GreenMask = 0x00ff00 ;
	ColorDataBuf->GreenWidth = 8 ;

	ColorDataBuf->BlueLoc = 0  ;
	ColorDataBuf->BlueMask = 0x0000ff ;
	ColorDataBuf->BlueWidth = 8 ;

	// �I��
	return 0 ;
}

// �O���[�X�P�[���̃J���[�����\�z����
extern int NS_CreateGrayColorData( COLORDATA * ColorDataBuf )
{
	int i ;

	ColorDataBuf->Format = DX_BASEIMAGE_FORMAT_NORMAL ;
	ColorDataBuf->ChannelNum = 0 ;
	ColorDataBuf->ChannelBitDepth = 0 ;
	ColorDataBuf->FloatTypeFlag = FALSE ;
	ColorDataBuf->ColorBitDepth = 8 ;
	ColorDataBuf->PixelByte = 1 ;
	ColorDataBuf->MaxPaletteNo = 255 ;

	ColorDataBuf->NoneMask = 0xff000000 ;

	ColorDataBuf->AlphaLoc = 0 ;
	ColorDataBuf->AlphaMask = 0 ;
	ColorDataBuf->AlphaWidth = 0 ;

	ColorDataBuf->RedLoc = 16 ;
	ColorDataBuf->RedMask = 0x00ff0000 ;
	ColorDataBuf->RedWidth = 8 ;

	ColorDataBuf->GreenLoc = 8 ;
	ColorDataBuf->GreenMask = 0x0000ff00 ;
	ColorDataBuf->GreenWidth = 8 ;

	ColorDataBuf->BlueLoc = 0  ;
	ColorDataBuf->BlueMask = 0x000000ff ;
	ColorDataBuf->BlueWidth = 8 ;

	for( i = 0 ; i < 256 ; i ++ )
	{
		ColorDataBuf->Palette[i].Red = ( unsigned char )i ;
		ColorDataBuf->Palette[i].Green = ( unsigned char )i ;
		ColorDataBuf->Palette[i].Blue = ( unsigned char )i ;
		ColorDataBuf->Palette[i].Alpha = 0 ;
	}

	// �I��
	return 0 ;
}

// �p���b�g�Q�T�U�F�̃J���[�����\�z����
extern int NS_CreatePal8ColorData( COLORDATA * ColorDataBuf, int UseAlpha )
{
	int i ;

	ColorDataBuf->Format = DX_BASEIMAGE_FORMAT_NORMAL ;
	ColorDataBuf->ChannelNum = 0 ;
	ColorDataBuf->ChannelBitDepth = 0 ;
	ColorDataBuf->FloatTypeFlag = FALSE ;
	ColorDataBuf->ColorBitDepth = 8 ;
	ColorDataBuf->PixelByte = 1 ;
	ColorDataBuf->MaxPaletteNo = 255 ;

	if( UseAlpha == FALSE )
	{
		ColorDataBuf->NoneMask = 0xff000000 ;

		ColorDataBuf->AlphaLoc = 0 ;
		ColorDataBuf->AlphaMask = 0 ;
		ColorDataBuf->AlphaWidth = 0 ;
	}
	else
	{
		ColorDataBuf->NoneMask = 0x00000000 ;

		ColorDataBuf->AlphaLoc = 24 ;
		ColorDataBuf->AlphaMask = 0xff000000 ;
		ColorDataBuf->AlphaWidth = 8 ;
	}

	ColorDataBuf->RedLoc = 16 ;
	ColorDataBuf->RedMask = 0x00ff0000 ;
	ColorDataBuf->RedWidth = 8 ;

	ColorDataBuf->GreenLoc = 8 ;
	ColorDataBuf->GreenMask = 0x0000ff00 ;
	ColorDataBuf->GreenWidth = 8 ;

	ColorDataBuf->BlueLoc = 0  ;
	ColorDataBuf->BlueMask = 0x000000ff ;
	ColorDataBuf->BlueWidth = 8 ;

	for( i = 0 ; i < 256 ; i ++ )
	{
		ColorDataBuf->Palette[i].Red   = ( BYTE )( ( ( i >> 0 ) & 7 ) * 255 / 7 ) ;
		ColorDataBuf->Palette[i].Green = ( BYTE )( ( ( i >> 3 ) & 7 ) * 255 / 7 ) ;
		ColorDataBuf->Palette[i].Blue  = ( BYTE )( ( ( i >> 5 ) & 3 ) * 255 / 3 ) ;
		ColorDataBuf->Palette[i].Alpha = 0 ;
	}

	// �I��
	return 0 ;
}

// �J���[�f�[�^���쐬����	
extern int NS_CreateColorData( COLORDATA * ColorDataBuf, int ColorBitDepth,
							 DWORD RedMask, DWORD GreenMask, DWORD BlueMask, DWORD AlphaMask,
							 int ChannelNum, int ChannelBitDepth, int FloatTypeFlag )
{
	int i, j ;
	DWORD NoneMask;

	_MEMSET( ColorDataBuf, 0, sizeof( COLORDATA ) ) ;

	// �f�[�^�̃Z�b�g
	ColorDataBuf->Format = DX_BASEIMAGE_FORMAT_NORMAL ;

	if( ChannelNum != 0 && ChannelBitDepth != 0 )
	{
		ColorDataBuf->ChannelBitDepth = ( unsigned char )ChannelBitDepth ;
		ColorDataBuf->ChannelNum      = ( unsigned char )ChannelNum ;
		ColorDataBuf->FloatTypeFlag   = ( unsigned char )FloatTypeFlag ;
		ColorDataBuf->PixelByte       = ( unsigned char )( ChannelBitDepth * ChannelNum / 8 ) ;
	}
	else
	{
		ColorDataBuf->ChannelBitDepth = 0 ;
		ColorDataBuf->ChannelNum      = 0 ;
		ColorDataBuf->FloatTypeFlag   = FALSE ;
		ColorDataBuf->ColorBitDepth   = ( unsigned char )ColorBitDepth ;
		ColorDataBuf->PixelByte       = ( unsigned char )( ColorBitDepth / 8 ) ;
		ColorDataBuf->RedMask         = RedMask ;
		ColorDataBuf->GreenMask       = GreenMask ;
		ColorDataBuf->BlueMask        = BlueMask ;
		ColorDataBuf->AlphaMask       = AlphaMask ;
		if( ColorBitDepth == 32 ) ColorDataBuf->NoneMask = 0xffffffff;
		else                      ColorDataBuf->NoneMask = ( DWORD )( ( 1 << ColorBitDepth ) - 1 ) ;
		ColorDataBuf->NoneMask     &= ~( RedMask | GreenMask | BlueMask | AlphaMask );
		NoneMask = ColorDataBuf->NoneMask;
		
		// �ԐF�}�X�N�̉��
		if( RedMask == 0 )
		{
			ColorDataBuf->RedLoc = ColorDataBuf->RedWidth = 0 ;
			ColorDataBuf->RedMask = 0 ;
		}
		else
		{
			j = 1 ;
			for( i = 0 ; ( RedMask & j ) == 0 ; j <<= 1, i ++ ){} ColorDataBuf->RedLoc   = ( unsigned char )i ;
			for( i = 0 ; ( RedMask & j ) != 0 ; j <<= 1, i ++ ){} ColorDataBuf->RedWidth = ( unsigned char )i ;
		}

		// �ΐF�}�X�N�̉��
		if( GreenMask == 0 )
		{
			ColorDataBuf->GreenLoc = ColorDataBuf->GreenWidth = 0 ;
			ColorDataBuf->GreenMask = 0 ;
		}
		else
		{
			j = 1 ;
			for( i = 0 ; ( GreenMask & j ) == 0 ; j <<= 1, i ++ ){} ColorDataBuf->GreenLoc   = ( unsigned char )i ;
			for( i = 0 ; ( GreenMask & j ) != 0 ; j <<= 1, i ++ ){} ColorDataBuf->GreenWidth = ( unsigned char )i ;
		}

		// �F�}�X�N�̉��
		if( BlueMask == 0 )
		{
			ColorDataBuf->BlueLoc = ColorDataBuf->BlueWidth = 0 ;
			ColorDataBuf->BlueMask = 0 ;
		}
		else
		{
			j = 1 ;
			for( i = 0 ; ( BlueMask & j ) == 0 ; j <<= 1, i ++ ){} ColorDataBuf->BlueLoc   = ( unsigned char )i ;
			for( i = 0 ; ( BlueMask & j ) != 0 ; j <<= 1, i ++ ){} ColorDataBuf->BlueWidth = ( unsigned char )i ;
		}

		// �A���t�@�}�X�N�̉��
		if( AlphaMask == 0 )
		{
			ColorDataBuf->AlphaLoc = ColorDataBuf->AlphaWidth = 0 ;
			ColorDataBuf->AlphaMask = 0 ;
		}
		else
		{
			j = 1 ;
			for( i = 0 ; ( AlphaMask & j ) == 0 ; j <<= 1, i ++ ){} ColorDataBuf->AlphaLoc   = ( unsigned char )i ;
			for( i = 0 ; ( AlphaMask & j ) != 0 ; j <<= 1, i ++ ){} ColorDataBuf->AlphaWidth = ( unsigned char )i ;
		}

		// �g���Ă��Ȃ��}�X�N�̉��
		if( NoneMask == 0 )
		{
			ColorDataBuf->NoneLoc = ColorDataBuf->NoneWidth = 0 ;
			ColorDataBuf->NoneMask = 0 ;
		}
		else
		{
			j = 1 ;
			for( i = 0 ; ( NoneMask & j ) == 0 ; j <<= 1, i ++ ){} ColorDataBuf->NoneLoc   = ( unsigned char )i ;
			for( i = 0 ; ( NoneMask & j ) != 0 ; j <<= 1, i ++ ){} ColorDataBuf->NoneWidth = ( unsigned char )i ;
		}

		// NoneMask �̃Z�b�g
//		NS_SetColorDataNoneMask( ColorDataBuf ) ;
	}
	
	// �I��
	return 0 ;
}

// NoneMask �ȊO�̗v�f�𖄂߂� COLORDATA �\���̂̏������� NoneMask ���Z�b�g����
extern void NS_SetColorDataNoneMask( COLORDATA * ColorData )
{
	if( ColorData->AlphaWidth + ColorData->RedWidth + ColorData->GreenWidth + ColorData->BlueWidth != ColorData->ColorBitDepth )
	{
		int i ;

		ColorData->NoneMask = 0 ;
		for( i = 0 ; i < ColorData->ColorBitDepth ; i ++ )
		{
			ColorData->NoneMask <<= 1 ;
			ColorData->NoneMask |= 1 ;
		}
		ColorData->NoneMask &= ~ColorData->RedMask & ~ColorData->GreenMask & ~ColorData->BlueMask & ~ColorData->AlphaMask ;
	}
	else
	{
		ColorData->NoneMask = 0 ;
	}
}

// ��̃J���[�f�[�^�����������ǂ������ׂ�( TRUE:������  FALSE:�������Ȃ� )
extern int NS_CmpColorData( const COLORDATA * ColorData1, const COLORDATA * ColorData2 )
{
	if( ColorData1->Format != ColorData2->Format ) return FALSE ;

	if( ColorData1->ChannelNum != 0 || ColorData2->ChannelNum != 0 )
	{
		return	ColorData1->ChannelNum      == ColorData2->ChannelNum &&
				ColorData1->ChannelBitDepth == ColorData2->ChannelBitDepth &&
				ColorData1->FloatTypeFlag   == ColorData2->FloatTypeFlag ? TRUE : FALSE ;
	}

	return 	ColorData1->ColorBitDepth 	== ColorData2->ColorBitDepth 	&&
			ColorData1->RedMask 		== ColorData2->RedMask 			&&
			ColorData1->GreenMask 		== ColorData2->GreenMask 		&&
			ColorData1->BlueMask 		== ColorData2->BlueMask 		&&
			ColorData1->AlphaMask 		== ColorData2->AlphaMask ? TRUE : FALSE ;
}

// �����ǂݍ��݃��[�`�����g�����ۂ��̃t���O���Z�b�g����
extern int NS_SetUseFastLoadFlag( int Flag )
{
	// �t���O���Z�b�g����
	BASEIM.LowLoadFlag = Flag == TRUE ? FALSE : TRUE ;

	// �I��
	return 0 ;
}

// �O���t�B�b�N���F���̉摜�򉻊ɘa�������[�h�̎擾
extern int NS_GetGraphDataShavedMode( void )
{
	return BASEIM.ImageShavedMode ;
}

// �O���t�B�b�N���F���̉摜�򉻊ɘa�������[�h�̕ύX
extern int NS_SetGraphDataShavedMode( int ShavedMode )
{
	BASEIM.ImageShavedMode = ShavedMode ;

	// �I��
	return 0 ;
}

// �摜�t�@�C���ǂݍ��ݎ��ɏ�Z�ς݃A���t�@�摜�ɕϊ����邩�ǂ�����ݒ肷��( TRUE:�ϊ��������s��  FALSE:�ϊ��������s��Ȃ�( �f�t�H���g ) )
extern int NS_SetUsePremulAlphaConvertLoad( int UseFlag )
{
	BASEIM.ConvertPremultipliedAlpha = UseFlag ;

	// �I��
	return 0 ;
}

#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE
