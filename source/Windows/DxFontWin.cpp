//-----------------------------------------------------------------------------
// 
// 		�c�w���C�u����		Windows�p�t�H���g�֌W�v���O����
// 
//  	Ver 3.16d
// 
//-----------------------------------------------------------------------------

// �c�w���C�u�����쐬���p��`
#define __DX_MAKE

// �C���N���[�h ---------------------------------------------------------------
#include "DxFontWin.h"
#include "DxWindow.h"
#include "DxBaseFuncWin.h"
#include "../DxFont.h"
#include "../DxBaseFunc.h"
#include "../DxSystem.h"
#include "../DxArchive_.h"
#include "../DxLog.h"
#include "DxWinAPI.h"
#include <windows.h>

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// �}�N����` -----------------------------------------------------------------

// �\���̐錾 -----------------------------------------------------------------

// �f�[�^��` -----------------------------------------------------------------

// �L�����Z�b�g�e�[�u��
DWORD CharSetTable[ DX_CHARSET_NUM ] =
{
	DEFAULT_CHARSET,		// DX_CHARSET_DEFAULT
	SHIFTJIS_CHARSET,		// DX_CHARSET_SHFTJIS
	HANGEUL_CHARSET,		// DX_CHARSET_HANGEUL
	CHINESEBIG5_CHARSET,	// DX_CHARSET_BIG5
	GB2312_CHARSET,			// DX_CHARSET_GB2312
	ANSI_CHARSET,			// DX_CHARSET_WINDOWS_1252
	ANSI_CHARSET,			// DX_CHARSET_ISO_IEC_8859_15
} ;

const unsigned char Japanese1[ 7 ] = { 0x93, 0xfa, 0x96, 0x7b, 0x8c, 0xea, 0x00 } ;		// ���{��
const unsigned char Japanese2[ 5 ] = { 0xa4, 0xe9, 0xa4, 0xe5, 0x00 } ;					// ����

FONTSYSTEM_WIN FontSystem_Win ;

// �֐��錾 -------------------------------------------------------------------

// �v���O���� -----------------------------------------------------------------

// CreateFontToHandle �̊��ˑ��������s���֐�
extern int CreateFontToHandle_PF( FONTMANAGE *ManageData, int DefaultCharSet )
{
	int CreateFontSize ;
	int SampleScale ;
	int EnableAddHeight	= FALSE ;
	int	AddHeight		= 0 ;
	int	OrigHeight		= 0 ;

	if( WinData.DestroyMessageCatchFlag == TRUE )
	{
		return -1 ;
	}

	// �t�H���g�f�[�^�t�@�C�����g�p����ꍇ�͉��������ɏI��
	if( ManageData->UseFontDataFile )
	{
		return 0 ;
	}

	switch( ManageData->FontType )
	{
	case DX_FONTTYPE_NORMAL:
	case DX_FONTTYPE_EDGE:
	case DX_FONTTYPE_ANTIALIASING:
	case DX_FONTTYPE_ANTIALIASING_EDGE:
		SampleScale = 1 ;
		break ;

	case DX_FONTTYPE_ANTIALIASING_4X4:
	case DX_FONTTYPE_ANTIALIASING_EDGE_4X4:
		SampleScale = 4 ;
		break ;

	case DX_FONTTYPE_ANTIALIASING_8X8:
	case DX_FONTTYPE_ANTIALIASING_EDGE_8X8:
		SampleScale = 8 ;
		break ;
	}
	CreateFontSize = ManageData->BaseInfo.FontSize * SampleScale ;

CREATEFONTLABEL:

	// ���Ƀt�H���g���쐬����Ă�����폜
	if( ManageData->PF->FontObj != NULL )
	{
		DeleteObject( ManageData->PF->FontObj ) ;
	}

	if( ManageData->FontName[0] != L'\0' )
	{
		// ���ɕ����Z�b�g�̎w�肪�����ꍇ�ŁA���w��̃t�H���g���̎w��̕����Z�b�g�������ꍇ�͕����Z�b�g�� DEFAULT_CHARSET �ɂ���
		if( DefaultCharSet == TRUE )
		{
			wchar_t	TempNameBuffer[ 16 ][ 64 ] ;
			wchar_t	*TempNameBufferP ;
			int		TempNameNum ;
			int		TempNameBufferAlloc ;
			int		i ;

			TempNameNum = EnumFontNameEx2_WCHAR_T( TempNameBuffer[ 0 ], 16, ManageData->FontName, ManageData->BaseInfo.CharSet ) ;
			if( TempNameNum >= 16 )
			{
				TempNameNum			= EnumFontNameEx2_WCHAR_T( NULL, 0, ManageData->FontName, ManageData->BaseInfo.CharSet ) ;
				TempNameBufferP		= ( wchar_t * )DXALLOC( TempNameNum * 64 * sizeof( wchar_t ) ) ;
				TempNameNum			= EnumFontNameEx2_WCHAR_T( TempNameBufferP, TempNameNum, ManageData->FontName, ManageData->BaseInfo.CharSet ) ;
				TempNameBufferAlloc = TRUE ;
			}
			else
			{
				TempNameBufferAlloc = FALSE ;
				TempNameBufferP		= TempNameBuffer[ 0 ] ;
			}

			for( i = 0 ; i < TempNameNum && _WCSCMP( TempNameBufferP + i * 64, ManageData->FontName ) != 0 ; i ++ ){}
			if( i == TempNameNum )
			{
				ManageData->BaseInfo.CharSet = DX_CHARSET_DEFAULT ;
			}

			if( TempNameBufferAlloc )
			{
				DXFREE( TempNameBufferP ) ;
				TempNameBufferP = NULL ;
			}
		}

		ManageData->PF->FontObj = CreateFontW(
			CreateFontSize + AddHeight, 0, 0, 0,
			ManageData->BaseInfo.FontThickness * 100,
			ManageData->BaseInfo.Italic, FALSE, FALSE,
			CharSetTable[ ManageData->BaseInfo.CharSet ],
			OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			NONANTIALIASED_QUALITY, FIXED_PITCH,
			ManageData->FontName
		) ;

		if( ManageData->PF->FontObj == NULL )
		{
			ManageData->PF->FontObj = CreateFontW(
				CreateFontSize + AddHeight, 0, 0, 0,
				ManageData->BaseInfo.FontThickness * 100,
				ManageData->BaseInfo.Italic, FALSE, FALSE,
				DEFAULT_CHARSET,
				OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
				NONANTIALIASED_QUALITY, FIXED_PITCH,
				ManageData->FontName
			) ;

			if( ManageData->PF->FontObj == NULL )
			{
				ManageData->PF->FontObj = CreateFontW(
					CreateFontSize + AddHeight, 0, 0, 0,
					ManageData->BaseInfo.FontThickness * 100,
					ManageData->BaseInfo.Italic, FALSE, FALSE,
					SHIFTJIS_CHARSET,
					OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
					NONANTIALIASED_QUALITY, FIXED_PITCH,
					ManageData->FontName
				) ;

				if( ManageData->PF->FontObj == NULL )
				{
					DXST_ERRORLOG_ADDUTF16LE( "\x07\x63\x9a\x5b\x6e\x30\xd5\x30\xa9\x30\xf3\x30\xc8\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�w��̃t�H���g�̍쐬�Ɏ��s���܂���\n" @*/ ) ;
					goto ERR ;
				}
			}
		}
	}
	else
	{
		ManageData->PF->FontObj = CreateFontW(
			CreateFontSize + AddHeight, 0, 0, 0,
			ManageData->BaseInfo.FontThickness * 100,
			ManageData->BaseInfo.Italic, FALSE, FALSE,
			CharSetTable[ ManageData->BaseInfo.CharSet ],
			OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			NONANTIALIASED_QUALITY, FIXED_PITCH,
			( wchar_t * )"\x2d\xff\x33\xff\x20\x00\xb4\x30\xb7\x30\xc3\x30\xaf\x30\x00"/*@ L"�l�r �S�V�b�N" @*/
		) ;
		ManageData->FontName[ 0 ] = L'\0' ;
	}

	if( ManageData->PF->FontObj == NULL )
	{
		ManageData->PF->FontObj = CreateFontW(
			CreateFontSize + AddHeight, 0, 0, 0,
			ManageData->BaseInfo.FontThickness * 100,
			FALSE, FALSE, FALSE,
			DEFAULT_CHARSET,
			OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			NONANTIALIASED_QUALITY, FIXED_PITCH,
			( wchar_t * )"\x2d\xff\x33\xff\x20\x00\xb4\x30\xb7\x30\xc3\x30\xaf\x30\x00"/*@ L"�l�r �S�V�b�N" @*/
		) ;

		if( ManageData->PF->FontObj == NULL )
		{
			ManageData->PF->FontObj = CreateFontW(
				CreateFontSize + AddHeight, 0, 0, 0,
				ManageData->BaseInfo.FontThickness * 100,
				FALSE, FALSE, FALSE,
				DEFAULT_CHARSET,
				OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
				NONANTIALIASED_QUALITY, FIXED_PITCH,
				NULL
			) ;
		}
		ManageData->FontName[ 0 ] = L'\0' ;

		if( ManageData->PF->FontObj == NULL )
		{
			DXST_ERRORLOG_ADDUTF16LE( "\xd5\x30\xa9\x30\xf3\x30\xc8\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�t�H���g�̍쐬�Ɏ��s���܂���\n" @*/ ) ;
			goto ERR ;
		}
	}

	// �����̃T�C�Y���擾����
	{
		HDC			DC ;
		HFONT		OldFont ;
		TEXTMETRIC	TextInfo ;

		// �f�o�C�X�R���e�L�X�g���擾
		DC = CreateCompatibleDC( NULL ) ;
		if( DC == NULL )
		{
			DXST_ERRORLOG_ADDUTF16LE( "\xc7\x30\xd0\x30\xa4\x30\xb9\x30\xb3\x30\xf3\x30\xc6\x30\xad\x30\xb9\x30\xc8\x30\x6e\x30\xd6\x53\x97\x5f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�f�o�C�X�R���e�L�X�g�̎擾�Ɏ��s���܂���\n" @*/ ) ;
			goto ERR ;
		}

		// �t�H���g�̃Z�b�g
		OldFont = ( HFONT )SelectObject( DC, ManageData->PF->FontObj ) ;

		// �t�H���g�̃X�e�[�^�X���擾
		GetTextMetrics( DC, &TextInfo ) ;

		// ���� TextInfo.tmInternalLeading + TextInfo.tmExternalLeading �� 0 �ł͂Ȃ������炻�̍�����ǉ����ăt�H���g���쐬���Ȃ���
		if( EnableAddHeight == FALSE )
		{
			if( TextInfo.tmInternalLeading + TextInfo.tmExternalLeading > 0 )
			{
				OrigHeight		= TextInfo.tmHeight ;
				AddHeight		= _FTOL( CreateFontSize / ( 1.0f - ( float )( TextInfo.tmInternalLeading + TextInfo.tmExternalLeading ) / TextInfo.tmHeight ) ) - CreateFontSize ;
				EnableAddHeight	= TRUE ;

				SelectObject( DC, OldFont ) ;
				DeleteDC( DC ) ;

				DeleteObject( ManageData->PF->FontObj ) ;
				ManageData->PF->FontObj = NULL ;

				goto CREATEFONTLABEL ;
			}
			
			ManageData->BaseInfo.FontAddHeight = 0 ;
		}
		else
		{
			ManageData->BaseInfo.FontAddHeight = ( WORD )( ( TextInfo.tmHeight - OrigHeight ) / SampleScale ) ;
		}

		// �t�H���g�̍ő�T�C�Y���擾
		if( ManageData->BaseInfo.Italic )
		{
			// �C�^���b�N�̂̏ꍇ�͍ő啝�� 1.35�{�ɂȂ�
			ManageData->BaseInfo.MaxWidth = ( WORD )( ( TextInfo.tmMaxCharWidth * 135 / SampleScale + 4 * 135 ) / 100 ) ;
		}
		else
		{
			ManageData->BaseInfo.MaxWidth = ( WORD )( TextInfo.tmMaxCharWidth / SampleScale + 4 ) ;
		}

		// �t�H���g�̍�����ۑ�
		ManageData->BaseInfo.FontHeight = ( WORD )( TextInfo.tmHeight / SampleScale + 1 ) ;

		// �x�[�X���C�������ԏ�܂ł̍�����ۑ�
		ManageData->BaseInfo.Ascent = ( WORD )( TextInfo.tmAscent / SampleScale ) ;
		if( TextInfo.tmAscent % SampleScale >= SampleScale / 2 )
		{
			ManageData->BaseInfo.Ascent ++ ;
		}

		// GetGlyphOutline ���g�p�ł��邩�ǂ����𒲂ׂ�
		{
			GLYPHMETRICS	gm ;
			MAT2			mt = { { 0, 1 }, { 0, 0 }, { 0, 0 }, { 0, 1 } } ;
			DWORD			Code ;
			DWORD			DataSize ;

			_MEMSET( &gm, 0, sizeof( GLYPHMETRICS ) ) ;
			Code		= L' ' ;
			DataSize	= GetGlyphOutlineW( DC, Code, GGO_BITMAP, &gm, 0, NULL, &mt ) ;

			// ���s�����ꍇ�� TextOut �������g�p����
			if( DataSize == GDI_ERROR )
			{
				ManageData->PF->UseTextOut = TRUE ;

				// �g�p����e�N�X�`���L���b�V���� 16bit �Œ�
				ManageData->TextureCacheColorBitDepth = 16 ;
			}
		}

		// TextOut �������g�p����ꍇ�� DIB ���쐬���Ă���
		ManageData->PF->CacheBitmap			= NULL ;
		ManageData->PF->CacheBitmapMem		= NULL ;
		ManageData->PF->CacheBitmapMemPitch	= 0 ;
		if( ManageData->PF->UseTextOut )
		{
			BITMAPINFO	*BmpInfoPlus ;
			BITMAP		BmpData ;

			// �t�H���g�^�C�v�� DX_FONTTYPE_NORMAL �� DX_FONTTYPE_EDGE �Ɍ�����
			switch( ManageData->FontType )
			{
			case DX_FONTTYPE_ANTIALIASING :
			case DX_FONTTYPE_ANTIALIASING_4X4 :
			case DX_FONTTYPE_ANTIALIASING_8X8 :
				ManageData->FontType = DX_FONTTYPE_NORMAL ;
				break ;

			case DX_FONTTYPE_ANTIALIASING_EDGE :
			case DX_FONTTYPE_ANTIALIASING_EDGE_4X4 :
			case DX_FONTTYPE_ANTIALIASING_EDGE_8X8 :
				ManageData->FontType = DX_FONTTYPE_EDGE ;
				break ;
			}

			// �L���b�V���̈�̃X�e�[�^�X�̏�����
			BmpInfoPlus = ( BITMAPINFO * )DXCALLOC( sizeof( BITMAPINFO ) + sizeof( RGBQUAD ) * 256 ) ;
			if( BmpInfoPlus == NULL )
			{
				return DxLib_ErrorUTF16LE( "\xe1\x30\xe2\x30\xea\x30\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x20\x00\x69\x00\x6e\x00\x20\x00\x43\x00\x46\x00\x6f\x00\x6e\x00\x74\x00\x00"/*@ L"�������̊m�ۂɎ��s���܂��� in CFont" @*/ ) ;
			}
			BmpInfoPlus->bmiHeader.biSize			= sizeof( BITMAPINFOHEADER ) ;
			BmpInfoPlus->bmiHeader.biWidth			=  ManageData->BaseInfo.MaxWidth ;
			BmpInfoPlus->bmiHeader.biHeight			= -ManageData->BaseInfo.MaxWidth ;
			BmpInfoPlus->bmiHeader.biPlanes			= 1 ;
			BmpInfoPlus->bmiHeader.biBitCount		= 8 ;
			BmpInfoPlus->bmiHeader.biCompression	= BI_RGB ;
			BmpInfoPlus->bmiHeader.biSizeImage		= ( DWORD )( ManageData->BaseInfo.MaxWidth * ManageData->BaseInfo.MaxWidth ) ;

			// �J���[�p���b�g�̃Z�b�g
			{
				RGBQUAD *Color ;
				int		i ;

				Color = &BmpInfoPlus->bmiColors[0] ;
				for( i = 0 ; i < 256 ; i ++ )
				{
					Color->rgbBlue     = ( BYTE )i ;
					Color->rgbRed      = ( BYTE )i ;
					Color->rgbBlue     = ( BYTE )i ;
					Color->rgbReserved = 0 ;

					Color ++ ;
				}
			}

			// �c�h�a�f�[�^���쐬����
			ManageData->PF->CacheBitmapMem	= NULL ; 
			ManageData->PF->CacheBitmap		= CreateDIBSection( DC, BmpInfoPlus, DIB_PAL_COLORS, ( void ** )&ManageData->PF->CacheBitmapMem, NULL, 0 ) ;

			// �s�b�`�𓾂�
			GetObject( ManageData->PF->CacheBitmap, sizeof( BITMAP ), &BmpData ) ;
			ManageData->PF->CacheBitmapMemPitch = BmpData.bmWidthBytes ;

			// �������̉��
			DXFREE( BmpInfoPlus ) ;
		}

		// �t�H���g�����ɖ߂�
		SelectObject( DC, OldFont ) ;

		// �f�o�C�X�R���e�L�X�g���폜����
		DeleteDC( DC ) ;
	}

	// ����I��
	return 0 ;

	// �G���[����
ERR :

	return -1 ;
}

// CreateFontToHandle �̊��ˑ��G���[�������s���֐�
extern int CreateFontToHandle_Error_PF( FONTMANAGE * ManageData )
{
	if( ManageData->PF->FontObj != NULL )
	{
		DeleteObject( ManageData->PF->FontObj ) ;
	}

	// �I��
	return 0 ;
}

// TerminateFontHandle �̊��ˑ��G���[�������s���֐�
extern int TerminateFontHandle_PF( FONTMANAGE *ManageData )
{
	// �t�H���g�C���[�W�擾�p�Ɏg�p�����������̉��
	if( ManageData->PF->GetGlyphOutlineBuffer != NULL )
	{
		DXFREE( ManageData->PF->GetGlyphOutlineBuffer ) ;
		ManageData->PF->GetGlyphOutlineBuffer = NULL ;
		ManageData->PF->GetGlyphOutlineBufferSize = 0 ;
	}

	// �t�H���g�I�u�W�F�N�g���폜
	if( ManageData->PF->FontObj != NULL )
	{
		DeleteObject( ManageData->PF->FontObj ) ;
		ManageData->PF->FontObj = NULL ;
	}

	// TextOut ���g�p����t�H���g�Ŏg�p����r�b�g�}�b�v�����
	if( ManageData->PF->CacheBitmap != NULL )
	{
		DeleteObject( ManageData->PF->CacheBitmap ) ;
		ManageData->PF->CacheBitmap = NULL ;
		ManageData->PF->CacheBitmapMem = NULL ;
	}

	// �I��
	return 0 ;
}

// FontCacheCharaAddToHandle�̊��ˑ��������s���֐�( ���s�ӏ���� 0 )
extern int FontCacheCharAddToHandle_Timing0_PF( FONTMANAGE *ManageData )
{
	// �f�o�C�X�R���e�L�X�g�̍쐬
	FontSystem_Win.Devicecontext = CreateCompatibleDC( NULL ) ;
	if( FontSystem_Win.Devicecontext == NULL )
	{
		DxLib_ErrorUTF16LE( "\xc6\x30\xad\x30\xb9\x30\xc8\x30\xad\x30\xe3\x30\xc3\x30\xb7\x30\xe5\x30\xb5\x30\xfc\x30\xd5\x30\xa7\x30\xb9\x30\x6e\x30\xc7\x30\xd0\x30\xa4\x30\xb9\x30\xb3\x30\xf3\x30\xc6\x30\xad\x30\xb9\x30\xc8\x30\x6e\x30\xd6\x53\x97\x5f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x00"/*@ L"�e�L�X�g�L���b�V���T�[�t�F�X�̃f�o�C�X�R���e�L�X�g�̎擾�Ɏ��s���܂���" @*/ ) ;
		return -1 ;
	}

	// �t�H���g���Z�b�g
	FontSystem_Win.OldFont = ( HFONT )SelectObject( FontSystem_Win.Devicecontext, ManageData->PF->FontObj ) ;
	if( FontSystem_Win.OldFont == NULL )
	{
		DeleteDC( FontSystem_Win.Devicecontext ) ;
		DxLib_ErrorUTF16LE( "\xc6\x30\xad\x30\xb9\x30\xc8\x30\xad\x30\xe3\x30\xc3\x30\xb7\x30\xe5\x30\xb5\x30\xfc\x30\xd5\x30\xa7\x30\xb9\x30\x6e\x30\xc7\x30\xd0\x30\xa4\x30\xb9\x30\xb3\x30\xf3\x30\xc6\x30\xad\x30\xb9\x30\xc8\x30\x6e\x30\xd6\x53\x97\x5f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x00"/*@ L"�e�L�X�g�L���b�V���T�[�t�F�X�̃f�o�C�X�R���e�L�X�g�̎擾�Ɏ��s���܂���" @*/ ) ;
		return -1 ;
	}

	// �t�H���g�̏����擾
	GetTextMetrics( FontSystem_Win.Devicecontext, &FontSystem_Win.TextMetric ) ;

	// TextOut ���g�p���邩�ǂ����ŏ����𕪊�
	if( ManageData->PF->UseTextOut )
	{
		// �`���r�b�g�}�b�v���Z�b�g
		FontSystem_Win.OldBitmap = ( HBITMAP )SelectObject( FontSystem_Win.Devicecontext , ManageData->PF->CacheBitmap ) ;

		// �����̕`��ݒ���s��
		{
			SetTextColor( FontSystem_Win.Devicecontext , RGB( 255 , 255 , 255 ) ) ; 		// �F���Z�b�g	

			// �w�i�F���Z�b�g
			SetBkColor( FontSystem_Win.Devicecontext , 0 ) ;
			SetBkMode( FontSystem_Win.Devicecontext , OPAQUE ) ;							// �w�i��h��Ԃ��w��
		}
	}

	// ����I��
	return 0 ;
}

// FontCacheCharaAddToHandle�̊��ˑ��������s���֐�( ���s�ӏ���� 1 )
extern int FontCacheCharAddToHandle_Timing1_PF( FONTMANAGE *ManageData, FONTCHARDATA *CharData, DWORD CharCode, int TextureCacheUpdate )
{
	int				ImageType = 0 ;
	int				gettype = 0 ;
	int				Space ;

	// �X�y�[�X���ǂ������擾���Ă���
	Space = CharCode == L' ' ? 1 : ( CharCode == FSYS.DoubleByteSpaceCharCode ? 2 : 0 ) ;

	// TextOut ���g�p���邩�ǂ����ŏ����𕪊�
	if( ManageData->PF->UseTextOut )
	{
		SIZE    TempSize ;
		wchar_t AddStr[ 8 ] ;
		int     CharNum ;

		CharNum = PutCharCode( CharCode, WIN32_WCHAR_CHARCODEFORMAT, ( char * )AddStr, sizeof( AddStr ) ) / sizeof( wchar_t ) ;

		// �ǉ����镶���̑傫�����擾
		GetTextExtentPoint32W( FontSystem_Win.Devicecontext, AddStr, CharNum, &TempSize );

		// �����C���[�W���o��
		TextOutW( FontSystem_Win.Devicecontext, 0, 0, AddStr, CharNum ) ;

		// �C���[�W��]��
		FontCacheCharImageBltToHandle(
			ManageData,
			CharData,
			CharCode, 
			FALSE,
			DX_FONT_SRCIMAGETYPE_8BIT_ON_OFF,
			ManageData->PF->CacheBitmapMem,
			TempSize.cx,
			TempSize.cy,
			ManageData->PF->CacheBitmapMemPitch,
			0,
			0,
			TempSize.cx,
			TextureCacheUpdate
		) ;
	}
	else
	{
		DWORD			DataSize ;
		GLYPHMETRICS	gm ;
		GCP_RESULTSW	gcp ;
		wchar_t			gcpBuffer[ 2 ] ;
		MAT2			mt = { { 0, 1 }, { 0, 0 }, { 0, 0 }, { 0, 1 } } ;
		DWORD			SrcPitch ; 

		// �擾����C���[�W�`�������肷��
		switch( ManageData->FontType )
		{
		case DX_FONTTYPE_NORMAL :
		case DX_FONTTYPE_EDGE :
		case DX_FONTTYPE_ANTIALIASING_4X4 :
		case DX_FONTTYPE_ANTIALIASING_8X8 :
		case DX_FONTTYPE_ANTIALIASING_EDGE_4X4 :
		case DX_FONTTYPE_ANTIALIASING_EDGE_8X8 :
			gettype		= GGO_BITMAP ;

			switch( ManageData->FontType )
			{
			case DX_FONTTYPE_NORMAL :
			case DX_FONTTYPE_EDGE :
				ImageType	= DX_FONT_SRCIMAGETYPE_1BIT ;
				break ;

			case DX_FONTTYPE_ANTIALIASING_4X4 :
			case DX_FONTTYPE_ANTIALIASING_EDGE_4X4 :
				ImageType	= DX_FONT_SRCIMAGETYPE_1BIT_SCALE4 ;
				break ;

			case DX_FONTTYPE_ANTIALIASING_8X8 :
			case DX_FONTTYPE_ANTIALIASING_EDGE_8X8 :
				ImageType	= DX_FONT_SRCIMAGETYPE_1BIT_SCALE8 ;
				break ;
			}
			break ;
			
		case DX_FONTTYPE_ANTIALIASING :
		case DX_FONTTYPE_ANTIALIASING_EDGE :
			if( ManageData->TextureCacheFlag == FALSE || ManageData->TextureCacheColorBitDepth == 16 )
			{
				gettype		= GGO_GRAY4_BITMAP ;
				ImageType	= DX_FONT_SRCIMAGETYPE_8BIT_MAX16 ;
			}
			else
			{
				gettype		= GGO_GRAY8_BITMAP ;
				ImageType	= DX_FONT_SRCIMAGETYPE_8BIT_MAX64 ;
			}
			break ;
		}

		// �T���Q�[�g�y�A�̏ꍇ�͕����̃O���t�C���f�b�N�X���擾����
		if( CharCode > 0xffff )
		{
			wchar_t CodeWString[ 8 ] ;
			int Bytes ;
			DWORD Result ;

			_MEMSET( &gcp, 0, sizeof( gcp ) ) ;
			gcp.lStructSize = sizeof( gcp ) ;
			gcp.lpGlyphs    = gcpBuffer ;
			gcp.nGlyphs     = 2 ;
			Bytes = PutCharCode( CharCode, DX_CHARCODEFORMAT_UTF16LE, ( char * )CodeWString, sizeof( CodeWString ) ) ;
			Result = GetCharacterPlacementW( FontSystem_Win.Devicecontext, CodeWString, Bytes / 2, 0, &gcp, GCP_GLYPHSHAPE ) ;
			if( Result == 0 )
			{
				DXST_ERRORLOG_ADDUTF16LE( "\x57\x00\x69\x00\x6e\x00\x33\x00\x32\x00\x41\x00\x50\x00\x49\x00\x20\x00\x47\x00\x65\x00\x74\x00\x43\x00\x68\x00\x61\x00\x72\x00\x61\x00\x63\x00\x74\x00\x65\x00\x72\x00\x50\x00\x6c\x00\x61\x00\x63\x00\x65\x00\x6d\x00\x65\x00\x6e\x00\x74\x00\x20\x00\x4c\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Win32API GetCharacterPlacement �����s���܂���\n" @*/ ) ;
				return -1 ;
			}
		}

		// �������̎擾
		_MEMSET( &gm, 0, sizeof( GLYPHMETRICS ) ) ;
		if( CharCode > 0xffff )
		{
			DataSize = GetGlyphOutlineW( FontSystem_Win.Devicecontext, ( UINT )gcp.lpGlyphs[ 0 ], ( UINT )gettype | GGO_GLYPH_INDEX, &gm, 0, NULL, &mt ) ;
		}
		else
		{
			DataSize = GetGlyphOutlineW( FontSystem_Win.Devicecontext, ( UINT )CharCode, ( UINT )gettype, &gm, 0, NULL, &mt ) ;
		}
		if( DataSize == GDI_ERROR )
		{
			DXST_ERRORLOG_ADDUTF16LE( "\x57\x00\x69\x00\x6e\x00\x33\x00\x32\x00\x41\x00\x50\x00\x49\x00\x20\x00\x47\x00\x65\x00\x74\x00\x47\x00\x6c\x00\x79\x00\x70\x00\x68\x00\x4f\x00\x75\x00\x74\x00\x6c\x00\x69\x00\x6e\x00\x65\x00\x20\x00\x4c\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Win32API GetGlyphOutline �����s���܂���\n" @*/ ) ;
			return -1 ;
		}

		// �X�y�[�X�����������ꍇ
		if( Space != 0 )
		{
			FontCacheCharImageBltToHandle(
				ManageData,
				CharData,
				CharCode, 
				TRUE,
				ImageType,
				NULL,
				0,
				0,
				0,
				0,
				0,
				gm.gmCellIncX,
				FALSE
			) ;
		}
		else
		// �X�y�[�X�ȊO�Ńf�[�^�T�C�Y���O�̏ꍇ
		if( DataSize == 0 )
		{
			FontCacheCharImageBltToHandle(
				ManageData,
				CharData,
				CharCode, 
				FALSE,
				DX_FONT_SRCIMAGETYPE_1BIT,
				NULL,
				0,
				0,
				0,
				0,
				0,
				0,
				FALSE
			) ;
		}
		else
		{
			BYTE *DataBuffer ;
			DWORD BufferSize ;

			SrcPitch   = ( DataSize / gm.gmBlackBoxY ) / 4 * 4 ;
			BufferSize = DataSize + SrcPitch * ( 2 + gm.gmBlackBoxY ) ;

			if( ManageData->PF->GetGlyphOutlineBufferSize < BufferSize )
			{
				ManageData->PF->GetGlyphOutlineBufferSize = BufferSize ;

				if( ManageData->PF->GetGlyphOutlineBuffer != NULL )
				{
					DXFREE( ManageData->PF->GetGlyphOutlineBuffer ) ;
					ManageData->PF->GetGlyphOutlineBuffer = NULL ;
				}

				ManageData->PF->GetGlyphOutlineBuffer = DXALLOC( ManageData->PF->GetGlyphOutlineBufferSize ) ;
				if( ManageData->PF->GetGlyphOutlineBuffer == NULL )
				{
					DxLib_ErrorUTF16LE( "\xa2\x30\xf3\x30\xc1\x30\xa8\x30\xa4\x30\xea\x30\xa2\x30\xb9\x30\x87\x65\x57\x5b\xd6\x53\x97\x5f\x28\x75\xd0\x30\xc3\x30\xd5\x30\xa1\x30\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x00"/*@ L"�A���`�G�C���A�X�����擾�p�o�b�t�@�̊m�ۂɎ��s���܂���" @*/ ) ;
					return -1 ;
				}
			}

			_MEMSET( ManageData->PF->GetGlyphOutlineBuffer, 0, BufferSize ) ;

			DataBuffer	= ( BYTE * )ManageData->PF->GetGlyphOutlineBuffer + SrcPitch * ( 1 + gm.gmBlackBoxY ) ;
			if( CharCode > 0xffff )
			{
				DataSize	= GetGlyphOutlineW( FontSystem_Win.Devicecontext, ( UINT )gcp.lpGlyphs[ 0 ], ( UINT )gettype | GGO_GLYPH_INDEX, &gm, DataSize, ( LPVOID )DataBuffer, &mt ) ;
			}
			else
			{
				DataSize	= GetGlyphOutlineW( FontSystem_Win.Devicecontext, ( UINT )CharCode, ( UINT )gettype, &gm, DataSize, ( LPVOID )DataBuffer, &mt ) ;
			}
			if( DataSize == GDI_ERROR )
			{
				DXST_ERRORLOG_ADDUTF16LE( "\x57\x00\x69\x00\x6e\x00\x33\x00\x32\x00\x41\x00\x50\x00\x49\x00\x20\x00\x47\x00\x65\x00\x74\x00\x47\x00\x6c\x00\x79\x00\x70\x00\x68\x00\x4f\x00\x75\x00\x74\x00\x6c\x00\x69\x00\x6e\x00\x65\x00\x20\x00\x4c\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Win32API GetGlyphOutline �����s���܂���\n" @*/ ) ;
				return -1 ;
			}

			// �C���[�W��]��
			FontCacheCharImageBltToHandle(
				ManageData,
				CharData,
				CharCode, 
				FALSE,
				ImageType,
				DataBuffer,
				gm.gmBlackBoxX,
				gm.gmBlackBoxY,
				( DataSize / gm.gmBlackBoxY ) / 4 * 4,
				gm.gmptGlyphOrigin.x ,
				FontSystem_Win.TextMetric.tmAscent - gm.gmptGlyphOrigin.y,
				gm.gmCellIncX,
				TextureCacheUpdate
			) ;
		}
	}

	// ����I��
	return 0 ;
}

// FontCacheCharaAddToHandle�̊��ˑ��������s���֐�( ���s�ӏ���� 2 )
extern int FontCacheCharAddToHandle_Timing2_PF( FONTMANAGE *ManageData )
{
	// �t�H���g�����ɖ߂�
	SelectObject( FontSystem_Win.Devicecontext, FontSystem_Win.OldFont ) ;

	if( ManageData->PF->UseTextOut )
	{
		// �r�b�g�}�b�v�����ɖ߂�
		SelectObject( FontSystem_Win.Devicecontext, FontSystem_Win.OldBitmap ) ;
		FontSystem_Win.OldBitmap = NULL ;
	}

	// �f�o�C�X�R���e�L�X�g�̍폜
	DeleteDC( FontSystem_Win.Devicecontext ) ;

	// �I��
	return 0 ;
}










// �w��̃t�H���g�t�@�C�����V�X�e���ɒǉ�����( �߂�l  NULL:���s  NULL�ȊO:�t�H���g�n���h��( WindowsOS �̂��̂Ȃ̂ŁA�c�w���C�u�����̃t�H���g�n���h���Ƃ͕ʕ��ł� ) )
extern HANDLE NS_AddFontFile( const TCHAR *FontFilePath )
{
#ifdef UNICODE
	return AddFontFile_WCHAR_T(
		FontFilePath
	) ;
#else
	HANDLE Result ;

	TCHAR_TO_WCHAR_T_STRING_ONE_BEGIN( FontFilePath, return NULL )

	Result = AddFontFile_WCHAR_T(
		UseFontFilePathBuffer
	) ;

	TCHAR_TO_WCHAR_T_STRING_END( FontFilePath )

	return Result ;
#endif
}

// �w��̃t�H���g�t�@�C�����V�X�e���ɒǉ�����( �߂�l  NULL:���s  NULL�ȊO:�t�H���g�n���h��( WindowsOS �̂��̂Ȃ̂ŁA�c�w���C�u�����̃t�H���g�n���h���Ƃ͕ʕ��ł� ) )
extern HANDLE AddFontFile_WCHAR_T( const wchar_t *FontFilePath )
{
	HANDLE FontHandle = NULL ;
	void *DataBuffer = NULL ;
	size_t DataSize ;
	DWORD_PTR FileHandle = 0 ;
	wchar_t FullPath[ 1024 ] ;

	ConvertFullPathW_( FontFilePath, FullPath, sizeof( FullPath ) ) ;

	// �t�@�C�����ۂ��Ɠǂݍ���
	FileHandle = DX_FOPEN( FontFilePath ) ;
	if( FileHandle == 0 )
	{
		DXST_ERRORLOGFMT_ADDW(( L"Font File Open Error : %s", FullPath )) ;
		goto ERRORLABEL ;
	}

	// �t�@�C���̃T�C�Y���擾����
	DX_FSEEK( FileHandle, 0, SEEK_END ) ;
	DataSize = ( size_t )DX_FTELL( FileHandle ) ;
	DX_FSEEK( FileHandle, 0, SEEK_SET ) ;

	// �t�@�C�����i�[���郁�������m��
	DataBuffer = DXALLOC( DataSize ) ;
	if( DataBuffer == NULL )
	{
		DXST_ERRORLOGFMT_ADDW(( L"Font File Memory Alloc Error : %s", FullPath )) ;
		goto ERRORLABEL ;
	}

	// �f�[�^��ǂݍ���
	DX_FREAD( DataBuffer, DataSize, 1, FileHandle ) ;

	// �t�@�C�������
	DX_FCLOSE( FileHandle ) ;
	FileHandle = 0 ;

	// �t�H���g�t�@�C�����V�X�e���ɒǉ�����
	FontHandle = NS_AddFontFileFromMem( DataBuffer, ( int )DataSize ) ;

	// �t�H���g�t�@�C���i�[�p�̃��������������
	DXFREE( DataBuffer ) ;
	DataBuffer = NULL ;

	// �߂�l��Ԃ�
	return FontHandle ;

ERRORLABEL :
	// �t�@�C������ǂݍ��񂾃f�[�^���������
	if( DataBuffer )
	{
		DXFREE( DataBuffer ) ;
		DataBuffer = NULL ;
	}

	// �t�@�C�������
	if( FileHandle )
	{
		DX_FCLOSE( FileHandle ) ;
		FileHandle = 0 ;
	}

	// �l��Ԃ�
	return NULL ;
}

// �w��̃������A�h���X�ɓW�J�����t�H���g�t�@�C���C���[�W���V�X�e���ɒǉ�����( �߂�l  NULL:���s  NULL�ȊO:�t�H���g�n���h��( WindowsOS �̂��̂Ȃ̂ŁA�c�w���C�u�����̃t�H���g�n���h���Ƃ͕ʕ��ł� ) )
extern HANDLE NS_AddFontFileFromMem( const void *FontFileImage, int FontFileImageSize )
{
	DWORD Fonts = 0 ;

	if( WinAPIData.Win32Func.AddFontMemResourceExFunc == NULL )
	{
		return NULL ;
	}

	return WinAPIData.Win32Func.AddFontMemResourceExFunc( ( void * )FontFileImage, ( DWORD )FontFileImageSize, NULL, &Fonts ) ;
}

// �w��̃t�H���g�n���h�����V�X�e������폜����( ������ AddFontFile �� AddFontFileFromMem �̖߂�l )
extern int NS_RemoveFontFile( HANDLE FontHandle )
{
	if( WinAPIData.Win32Func.RemoveFontMemResourceExFunc == NULL )
	{
		return -1 ;
	}

	return WinAPIData.Win32Func.RemoveFontMemResourceExFunc( FontHandle ) == 0 ? -1 : 0 ;
}


#ifndef DX_NON_SAVEFUNCTION

// �t�H���g�f�[�^�t�@�C�����쐬����
extern int NS_CreateFontDataFile( const TCHAR *SaveFilePath, const TCHAR *FontName, int Size, int BitDepth /* DX_FONTIMAGE_BIT_1�� */ , int Thick, int Italic, int CharSet, const TCHAR *SaveCharaList )
{
	FONTMANAGE				*ManageData ;
	FONTDATAFILEHEADER		*FontFileHeader ;
	FONTDATAFILECHARADATA	*FontCharaData ;
	BYTE					*FileHeaderBuffer = NULL ;
	DWORD					 FileHeaderBufferSize ;
	BYTE					*FilePressHeaderBuffer = NULL ;
	DWORD					 FilePressHeaderSize ;
	BYTE					*FileImageBuffer = NULL ;
	DWORD					 FileImageBufferSize ;
	DWORD					 FileImageBufferAddress ;
	BYTE					*FontTempImageBuffer = NULL ;
	DWORD					 FontTempImageBufferSize = 0 ;
	DWORD					 FontTempImageSize ;
	DWORD					 FontImagePressSize ;
	FONTCHARDATA			*CharData ;
	int						 FontHandle = -1 ;
	int						 FontType = 0 ;
	int						 FontCacheToTextureFlag ;
	int						 FontCacheTextureColorBitDepth ;
	int						 FontCacheCharaNum ;
	int						 FontCacheUsePremulAlphaFlag ;
	int						 i ;
	DWORD					*CharaList = NULL ;
	DWORD					 CharaIndex ;
	DWORD					 CharaNum ;
	const int				 CacheCharNum = 16 ;
	int						 CharCodeFormat ;

	// �w��̃p�����[�^�ɍ������t�H���g�n���h�����쐬����
	FontCacheToTextureFlag        = NS_GetFontCacheToTextureFlag() ;
	FontCacheTextureColorBitDepth = NS_GetFontCacheTextureColorBitDepth() ;
	FontCacheCharaNum             = NS_GetFontCacheCharNum() ;
	FontCacheUsePremulAlphaFlag   = NS_GetFontCacheUsePremulAlphaFlag() ;
	NS_SetFontCacheToTextureFlag( TRUE ) ;
	NS_SetFontCacheCharNum( CacheCharNum ) ;
	NS_SetFontCacheUsePremulAlphaFlag( FALSE ) ;
	switch( BitDepth )
	{
	case DX_FONTIMAGE_BIT_1 :
		FontType = DX_FONTTYPE_NORMAL ;
		NS_SetFontCacheTextureColorBitDepth( 16 ) ;
		break ;

	case DX_FONTIMAGE_BIT_4 :
	case DX_FONTIMAGE_BIT_8 :
		FontType = DX_FONTTYPE_ANTIALIASING_8X8 ;
		NS_SetFontCacheTextureColorBitDepth( 32 ) ;
		break ;
	}
	FontHandle = NS_CreateFontToHandle( FontName, Size, Thick, FontType, CharSet, 0, Italic ) ;
	NS_SetFontCacheToTextureFlag(        FontCacheToTextureFlag ) ;
	NS_SetFontCacheTextureColorBitDepth( FontCacheTextureColorBitDepth ) ;
	NS_SetFontCacheCharNum(              FontCacheCharaNum ) ;
	NS_SetFontCacheUsePremulAlphaFlag(   FontCacheUsePremulAlphaFlag ) ;
	if( FontHandle < 0 )
	{
		return -1 ;
	}

	ManageData = GetFontManageDataToHandle( FontHandle ) ;

	// �����R�[�h�`���̎擾
	CharCodeFormat = GetFontHandleCharCodeFormat( FontHandle ) ;

	// SaveCharaList �� NULL �̏ꍇ�́A�S�Ă̕�����ϊ��̑Ώۂɂ���
	CharaNum = 0 ;
	if( SaveCharaList == NULL )
	{
		CharaList = ( DWORD * )DXALLOC( sizeof( DWORD ) * 0x10000 ) ;
		_MEMSET( CharaList, 0, sizeof( DWORD ) * 0x10000 ) ;

		for( i = 1 ; i < 0x10000 ; i ++ )
		{
			CharaList[ CharaNum ] = i ;
			CharaNum ++ ;
		}
	}
	else
	{
		DWORD CharCode ;
		int   CharBytes ;
		int   SrcAddr ;
		int   StringCharNum ;
		DWORD j ;

		StringCharNum = GetStringCharNum( ( const char * )SaveCharaList, CharCodeFormat ) ;
		CharaList = ( DWORD * )DXALLOC( sizeof( DWORD ) * StringCharNum ) ;

		SrcAddr = 0 ;
		for( i = 0 ; i < StringCharNum ; i ++ )
		{
			CharCode = GetCharCode( ( const char * )&( ( BYTE * )SaveCharaList )[ SrcAddr ], CharCodeFormat, &CharBytes ) ;
			CharCode = ConvCharCode( CharCode, CharCodeFormat, DX_CHARCODEFORMAT_UTF32LE ) ;

			for( j = 0 ; j < CharaNum ; j ++ )
			{
				if( CharaList[ j ] == CharCode )
				{
					break ;
				}
			}
			if( j == CharaNum )
			{
				CharaList[ CharaNum ] = CharCode ;
				CharaNum ++ ;
			}
		}
	}

	// �e�N�X�`���L���b�V�����g�p�ł��Ȃ��ꍇ�̓G���[
	if( ManageData->TextureCacheFlag == FALSE )
	{
		goto ERR ;
	}

	// TextOut ���g�p����ꍇ�͋����I�� DX_FONTIMAGE_BIT_1 �ɂȂ�
	if( ManageData->PF->UseTextOut )
	{
		FontType = DX_FONTTYPE_NORMAL ;
		BitDepth = DX_FONTIMAGE_BIT_1 ;
	}

	// �w�b�_�o�b�t�@���m��
	FileHeaderBufferSize = sizeof( FONTDATAFILEHEADER ) + CharaNum * sizeof( FONTDATAFILECHARADATA ) ; 
	FileHeaderBuffer = ( BYTE * )DXALLOC( FileHeaderBufferSize * 3 ) ;
	if( FileHeaderBuffer == NULL )
	{
		DXST_ERRORLOG_ADDUTF16LE( "\xd5\x30\xa9\x30\xf3\x30\xc8\x30\xc7\x30\xfc\x30\xbf\x30\xd5\x30\xa1\x30\xa4\x30\xeb\x30\xd8\x30\xc3\x30\xc0\x30\xc5\x60\x31\x58\x00\x4e\x42\x66\x18\x8a\xb6\x61\x28\x75\xe1\x30\xe2\x30\xea\x30\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�t�H���g�f�[�^�t�@�C���w�b�_���ꎞ�L���p�������̊m�ۂɎ��s���܂���\n" @*/ ) ;
		goto ERR ;
	}
	_MEMSET( FileHeaderBuffer, 0, FileHeaderBufferSize * 3 ) ;
	FilePressHeaderBuffer = FileHeaderBuffer + FileHeaderBufferSize ;

	// �C���[�W�o�b�t�@���m��
	FileImageBufferSize = 1024 * 1024 * 32 ; 
	FileImageBuffer = ( BYTE * )DXALLOC( FileImageBufferSize ) ;
	if( FileImageBuffer == NULL )
	{
		DXST_ERRORLOG_ADDUTF16LE( "\xd5\x30\xa9\x30\xf3\x30\xc8\x30\xc7\x30\xfc\x30\xbf\x30\xd5\x30\xa1\x30\xa4\x30\xeb\x30\xd8\x30\xc3\x30\xc0\x30\xc5\x60\x31\x58\x00\x4e\x42\x66\x18\x8a\xb6\x61\x28\x75\xe1\x30\xe2\x30\xea\x30\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�t�H���g�f�[�^�t�@�C���w�b�_���ꎞ�L���p�������̊m�ۂɎ��s���܂���\n" @*/ ) ;
		goto ERR ;
	}
	_MEMSET( FileImageBuffer, 0, FileImageBufferSize ) ;

	FontFileHeader = ( FONTDATAFILEHEADER * )FileHeaderBuffer ;
	FontFileHeader->Magic[ 0 ]              = 'F' ;
	FontFileHeader->Magic[ 1 ]              = 'N' ;
	FontFileHeader->Magic[ 2 ]              = 'T' ;
	FontFileHeader->Magic[ 3 ]              = 'F' ;
	FontFileHeader->Version                 = 0 ;
	FontFileHeader->Press.BaseInfo          = ManageData->BaseInfo ;
	FontFileHeader->Press.BaseInfo.CharCodeFormat = ( WORD )CharCodeFormat ;
	FontFileHeader->Press.ImageBitDepth     = ( BYTE )BitDepth ;

	ConvString( ( const char * )ManageData->FontName, WCHAR_T_CHARCODEFORMAT, ( char * )FontFileHeader->Press.FontName, sizeof( FontFileHeader->Press.FontName ), DX_CHARCODEFORMAT_UTF16LE ) ;

	FontCharaData = ( FONTDATAFILECHARADATA * )( FileHeaderBuffer + 1 ) ;
	FileImageBufferAddress = 0 ;

	for( CharaIndex = 0 ; CharaIndex < CharaNum ; CharaIndex ++ )
	{
		// �L���b�V���ɕ�����ǉ�
		if( CharaIndex % CacheCharNum == 0 )
		{
			int AddNum ;

			AddNum = CharaNum - CharaIndex ;
			if( AddNum > CacheCharNum )
			{
				AddNum = CacheCharNum ;
			}

			FontCacheCharAddToHandle( AddNum, &CharaList[ CharaIndex ], ManageData, FALSE ) ;
		}

		// �L���b�V���ɑ��݂��Ȃ��ꍇ�͒ǉ�����
		CharData = GetFontCacheChar( ManageData, CharaList[ CharaIndex ], FALSE ) ;
		if( CharData == NULL )
		{
			continue ;
		}

		// �摜�T�C�Y���O�̏ꍇ�͒ǉ����Ȃ�
		if( CharData->SizeX == 0 &&
			CharData->SizeY == 0 &&
			CharData->AddX  == 0 )
		{
			continue ;
		}

		// �t�H���g�̏����Z�b�g
		FontCharaData->CodeUnicode   = CharaList[ CharaIndex ] ;
		FontCharaData->DrawX         = CharData->DrawX ;
		FontCharaData->DrawY         = CharData->DrawY ;
		FontCharaData->AddX          = CharData->AddX ;
		FontCharaData->SizeX         = CharData->SizeX ;
		FontCharaData->SizeY         = CharData->SizeY ;
		FontCharaData->ImageAddress  = FileImageBufferAddress ;

		// �C���[�W�f�[�^�̍\�z
		{
			DWORD		 DestPitch ;
			DWORD		 i ;
			DWORD		 j ;
			BYTE		*Dest ;
			BYTE		*Src ;
			DWORD		 SrcAlphaMask ;
			DWORD		 SrcAlphaLoc ;
			DWORD		 SrcAddPitch ;
			BASEIMAGE	*SrcBaseImage ;
			BYTE		 DestData ;

			SrcBaseImage = &ManageData->TextureCacheBaseImage ;
			SrcAlphaMask = SrcBaseImage->ColorData.AlphaMask ;
			SrcAlphaLoc  = SrcBaseImage->ColorData.AlphaLoc ;
			SrcAddPitch  = ( DWORD )( SrcBaseImage->Pitch - SrcBaseImage->ColorData.PixelByte * CharData->SizeX ) ;

			switch( BitDepth )
			{
			case DX_FONTIMAGE_BIT_1 :
				DestPitch = ( DWORD )( ( CharData->SizeX + 7 ) / 8 ) ;
				break ;

			case DX_FONTIMAGE_BIT_4 :
				DestPitch = ( DWORD )( ( CharData->SizeX + 1 ) / 2 ) ;
				break ;

			case DX_FONTIMAGE_BIT_8 :
				DestPitch = CharData->SizeX ;
				break ;
			}

			FontCharaData->ImagePitch = DestPitch ;

			FontTempImageSize = DestPitch * CharData->SizeY ;
			if( FontTempImageBufferSize < FontTempImageSize )
			{
				FontTempImageBufferSize = FontTempImageSize + 1024 * 32 ;
				FontTempImageBuffer = ( BYTE * )DXREALLOC( FontTempImageBuffer, FontTempImageBufferSize ) ;
				if( FontTempImageBuffer == NULL )
				{
					DXST_ERRORLOG_ADDUTF16LE( "\xd5\x30\xa9\x30\xf3\x30\xc8\x30\xc7\x30\xfc\x30\xbf\x30\x3b\x75\xcf\x50\x00\x4e\x42\x66\x18\x8a\xb6\x61\x28\x75\xe1\x30\xe2\x30\xea\x30\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�t�H���g�f�[�^�摜�ꎞ�L���p�������̊m�ۂɎ��s���܂���\n" @*/ ) ;
					goto ERR ;
				}
			}

			Dest = FontTempImageBuffer ;
			Src  = ( BYTE * )SrcBaseImage->GraphData + SrcBaseImage->ColorData.PixelByte * 1 + SrcBaseImage->Pitch * 1 ;
			switch( BitDepth )
			{
			case DX_FONTIMAGE_BIT_1 :
				for( i = 0 ; i < CharData->SizeY ; i ++, Src += SrcAddPitch )
				{
					for( j = 0 ; j < CharData->SizeX ; j += 8 )
					{
						DestData = 0 ;

						if( *( ( DWORD * )Src ) & SrcAlphaMask ) DestData |= 0x80 ;
						Src += SrcBaseImage->ColorData.PixelByte ;

						if( CharData->SizeX == j + 1 )
						{
							*Dest = DestData ;
							Dest ++ ;
							continue ;
						}
						if( *( ( DWORD * )Src ) & SrcAlphaMask ) DestData |= 0x40 ;
						Src += SrcBaseImage->ColorData.PixelByte ;

						if( CharData->SizeX == j + 2 )
						{
							*Dest = DestData ;
							Dest ++ ;
							continue ;
						}
						if( *( ( DWORD * )Src ) & SrcAlphaMask ) DestData |= 0x20 ;
						Src += SrcBaseImage->ColorData.PixelByte ;

						if( CharData->SizeX == j + 3 )
						{
							*Dest = DestData ;
							Dest ++ ;
							continue ;
						}
						if( *( ( DWORD * )Src ) & SrcAlphaMask ) DestData |= 0x10 ;
						Src += SrcBaseImage->ColorData.PixelByte ;

						if( CharData->SizeX == j + 4 )
						{
							*Dest = DestData ;
							Dest ++ ;
							continue ;
						}
						if( *( ( DWORD * )Src ) & SrcAlphaMask ) DestData |= 0x08 ;
						Src += SrcBaseImage->ColorData.PixelByte ;

						if( CharData->SizeX == j + 5 )
						{
							*Dest = DestData ;
							Dest ++ ;
							continue ;
						}
						if( *( ( DWORD * )Src ) & SrcAlphaMask ) DestData |= 0x04 ;
						Src += SrcBaseImage->ColorData.PixelByte ;

						if( CharData->SizeX == j + 6 )
						{
							*Dest = DestData ;
							Dest ++ ;
							continue ;
						}
						if( *( ( DWORD * )Src ) & SrcAlphaMask ) DestData |= 0x02 ;
						Src += SrcBaseImage->ColorData.PixelByte ;

						if( CharData->SizeX == j + 7 )
						{
							*Dest = DestData ;
							Dest ++ ;
							continue ;
						}
						if( *( ( DWORD * )Src ) & SrcAlphaMask ) DestData |= 0x01 ;
						Src += SrcBaseImage->ColorData.PixelByte ;

						*Dest = DestData ;
						Dest ++ ;
					}
				}
				break ;

			case DX_FONTIMAGE_BIT_4 :
				for( i = 0 ; i < CharData->SizeY ; i ++, Src += SrcAddPitch )
				{
					for( j = 0 ; j < CharData->SizeX ; j += 2 )
					{
						DestData = ( BYTE )( ( ( *( ( DWORD * )Src ) & SrcAlphaMask ) >> ( SrcAlphaLoc + 4 ) ) << 4 ) ;
						Src += SrcBaseImage->ColorData.PixelByte ;
						if( CharData->SizeX == j + 1 )
						{
							*Dest = DestData ;
							Dest ++ ;
							continue ;
						}

						DestData |= ( BYTE )( ( *( ( DWORD * )Src ) & SrcAlphaMask ) >> ( SrcAlphaLoc + 4 ) ) ;
						Src += SrcBaseImage->ColorData.PixelByte ;

						*Dest = DestData ;
						Dest ++ ;
					}
				}
				break ;

			case DX_FONTIMAGE_BIT_8 :
				for( i = 0 ; i < CharData->SizeY ; i ++, Src += SrcAddPitch )
				{
					for( j = 0 ; j < CharData->SizeX ; j ++, Dest ++, Src += SrcBaseImage->ColorData.PixelByte )
					{
						*Dest = ( BYTE )( ( *( ( DWORD * )Src ) & SrcAlphaMask ) >> SrcAlphaLoc ) ;
					}
				}
				break ;
			}

			// �C���[�W�����k����
			if( FileImageBufferSize < FileImageBufferAddress + FontTempImageSize * 2 )
			{
				FileImageBufferSize += FontTempImageSize * 2 + 1024 * 1024 * 32 ;
				FileImageBuffer = ( BYTE * )DXREALLOC( FileImageBuffer, FileImageBufferSize ) ;
				if( FileImageBuffer == NULL )
				{
					DXST_ERRORLOG_ADDUTF16LE( "\xd5\x30\xa9\x30\xf3\x30\xc8\x30\xc7\x30\xfc\x30\xbf\x30\xd5\x30\xa1\x30\xa4\x30\xeb\x30\xd8\x30\xc3\x30\xc0\x30\xc5\x60\x31\x58\x00\x4e\x42\x66\x18\x8a\xb6\x61\x28\x75\xe1\x30\xe2\x30\xea\x30\x6e\x30\xe1\x62\x35\x5f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�t�H���g�f�[�^�t�@�C���w�b�_���ꎞ�L���p�������̊g���Ɏ��s���܂���\n" @*/ ) ;
					goto ERR ;
				}
			}
			FontImagePressSize = ( DWORD )DXA_Encode( FontTempImageBuffer, FontTempImageSize, FileImageBuffer + FileImageBufferAddress ) ;

			if( FontImagePressSize >= FontTempImageSize )
			{
				FontCharaData->Press = 0 ;

				_MEMCPY( FileImageBuffer + FileImageBufferAddress, FontTempImageBuffer, FontTempImageSize ) ;
			}
			else
			{
				FontCharaData->Press = 1 ;
			}

			// ���ɓo�^���Ă��镶���Ƃ܂����������摜�����邩���ׂ�
			{
				FONTDATAFILECHARADATA	*FontCharaDataSub ;

				FontCharaDataSub = ( FONTDATAFILECHARADATA * )( FontFileHeader + 1 ) ;
				for( i = 0 ; i < FontFileHeader->CharaNum ; i ++, FontCharaDataSub ++ )
				{
					if( FontCharaDataSub->Press != FontCharaData->Press ||
						FontCharaDataSub->DrawX != FontCharaData->DrawX ||
						FontCharaDataSub->DrawY != FontCharaData->DrawY ||
						FontCharaDataSub->AddX  != FontCharaData->AddX ||
						FontCharaDataSub->SizeX != FontCharaData->SizeX ||
						FontCharaDataSub->SizeY != FontCharaData->SizeY )
					{
						continue ;
					}

					if( FontCharaData->Press )
					{
						int DecodeSize ;

						DecodeSize = DXA_Decode( FileImageBuffer + FontCharaDataSub->ImageAddress, NULL ) ;
						if( DecodeSize != FontImagePressSize )
						{
							continue ;
						}
						if( _MEMCMP( FileImageBuffer + FontCharaDataSub->ImageAddress, FileImageBuffer + FontCharaData->ImageAddress, FontImagePressSize ) == 0 )
						{
							break ;
						}
					}
					else
					{
						if( _MEMCMP( FileImageBuffer + FontCharaDataSub->ImageAddress, FileImageBuffer + FontCharaData->ImageAddress, FontTempImageSize ) == 0 )
						{
							break ;
						}
					}
				}

				// �S�������摜���������ꍇ�͉摜�����L����
				if( i != FontFileHeader->CharaNum )
				{
					FontCharaData->ImageAddress = FontCharaDataSub->ImageAddress ;
				}
				else
				{
					// ����ȊO�̏ꍇ�͉摜�̃A�h���X��i�߂�
					if( FontCharaData->Press )
					{
						FileImageBufferAddress += FontImagePressSize ;
					}
					else
					{
						FileImageBufferAddress += FontTempImageSize ;
					}
				}
			}
		}

		FontCharaData ++ ;
		FontFileHeader->CharaNum ++ ;

		if( CharaList[ CharaIndex ] >= 0x10000 )
		{
			FontFileHeader->CharaExNum ++ ;
		}
	}

	// �t�@�C���w�b�_�����̈��k
	{
		int NotPressSize ;

		NotPressSize = sizeof( FONTDATAFILEHEADER ) - sizeof( FONTDATAFILEPRESSHEADER ) ;

		FilePressHeaderSize = DXA_Encode(
			FileHeaderBuffer + NotPressSize,
			( DWORD )( ( BYTE * )FontCharaData - FileHeaderBuffer ) - NotPressSize,
			FilePressHeaderBuffer + NotPressSize ) + NotPressSize ;

		// �摜�f�[�^�J�n�A�h���X���Z�b�g
		FontFileHeader->ImageAddress = FilePressHeaderSize ;

		// ���k���Ȃ��������R�s�[
		_MEMCPY( FilePressHeaderBuffer, FileHeaderBuffer, NotPressSize ) ;
	}

	// �t�@�C���ɕۑ�
	{
		HANDLE FileHandle ;
		DWORD WriteSize ;

		// ���Ƀt�@�C�����������ꍇ�p�ɁA�t�@�C���폜����
		DeleteFile( SaveFilePath ) ;

		// �t�@�C�����J��
		FileHandle = CreateFile( SaveFilePath, GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL ) ;
		if( FileHandle == NULL ) 
		{
			DXST_ERRORLOG_ADDUTF16LE( "\xd5\x30\xa9\x30\xf3\x30\xc8\x30\xc7\x30\xfc\x30\xbf\x30\xd5\x30\xa1\x30\xa4\x30\xeb\x30\x6e\x30\xaa\x30\xfc\x30\xd7\x30\xf3\x30\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�t�H���g�f�[�^�t�@�C���̃I�[�v���Ɏ��s���܂���\n" @*/ ) ;
			goto ERR ;
		}

		// �t�@�C���w�b�_�̏����o��
		WriteFile( FileHandle, FilePressHeaderBuffer, FilePressHeaderSize, &WriteSize, NULL ) ;
		
		// �C���[�W�̏����o��
		WriteFile( FileHandle, FileImageBuffer, FileImageBufferAddress, &WriteSize, NULL ) ;

		// �t�@�C�������
		CloseHandle( FileHandle ) ;
	}

	// �m�ۂ��Ă��������������
	if( FontTempImageBuffer != NULL )
	{
		DXFREE( FontTempImageBuffer ) ;
		FontTempImageBuffer = NULL ;
	}

	if( FileHeaderBuffer != NULL )
	{
		DXFREE( FileHeaderBuffer ) ;
		FileHeaderBuffer = NULL ;
	}

	if( FileImageBuffer != NULL )
	{
		DXFREE( FileImageBuffer ) ;
		FileImageBuffer = NULL ;
	}

	if( CharaList != NULL )
	{
		DXFREE( CharaList ) ;
		CharaList = NULL ;
	}

	// �t�H���g�f�[�^�쐬�����p�ɍ쐬�����t�H���g�n���h�����폜
	NS_DeleteFontToHandle( FontHandle ) ;

	// ����I��
	return 0 ;

	// �G���[����
ERR :
	if( FontTempImageBuffer != NULL )
	{
		DXFREE( FontTempImageBuffer ) ;
		FontTempImageBuffer = NULL ;
	}

	if( FileHeaderBuffer != NULL )
	{
		DXFREE( FileHeaderBuffer ) ;
		FileHeaderBuffer = NULL ;
	}

	if( FileImageBuffer != NULL )
	{
		DXFREE( FileImageBuffer ) ;
		FileImageBuffer = NULL ;
	}

	if( CharaList != NULL )
	{
		DXFREE( CharaList ) ;
		CharaList = NULL ;
	}

	if( FontHandle >= 0 )
	{
		NS_DeleteFontToHandle( FontHandle ) ;
	}

	return -1 ;
}

#endif // DX_NON_SAVEFUNCTION

// �t�H���g�񋓗p�R�[���o�b�N�֐�
int CALLBACK EnumFontFamExProc( ENUMLOGFONTEXW *lpelf, NEWTEXTMETRICEXW * /*lpntm*/, int nFontType, LPARAM lParam )
{
	ENUMFONTDATA *EnumFontData = ( ENUMFONTDATA * )lParam ;

	// JapanOnly�t���O�������Ă����� TrueType �A���{��t�H���g������񋓂���
	// �������t�H���g(@�t)�͂�������͂���
	if( ( EnumFontData->JapanOnlyFlag == TRUE && ( nFontType & TRUETYPE_FONTTYPE ) &&
		( _WCSCMP( ( wchar_t * )"\xe5\x65\x2c\x67\x9e\x8a\x00"/*@ L"���{��" @*/, &lpelf->elfScript[0] ) == 0 ||
		  _WCSCMP( ( wchar_t * )"\xe5\x65\x87\x65\x00"/*@ L"����" @*/,   &lpelf->elfScript[0] ) == 0 )
		  && lpelf->elfFullName[0] != L'@' ) || 
		( EnumFontData->JapanOnlyFlag == FALSE && lpelf->elfFullName[0] != L'@' ) )
	{
		// �����t�H���g�����ȑO�ɂ��������ꍇ�͒e��
		{
			int i ;

			for( i = 0 ; i < EnumFontData->FontNum ; i ++ )
			{
				if( _WCSCMP( &lpelf->elfFullName[0], &EnumFontData->FontBuffer[64 * i] ) == 0 )
				{
					return TRUE ;
				}
			}
		}

		// �l�[����ۑ�����
		_WCSCPY_S( &EnumFontData->FontBuffer[ 64 * EnumFontData->FontNum ], sizeof( wchar_t ) * 64, &lpelf->elfFullName[0] ) ;

		// �t�H���g�̐��𑝂₷
		EnumFontData->FontNum ++ ;

		// �����o�b�t�@�̐������E�ɗ��Ă�����񋓏I��
		if( EnumFontData->BufferNum != 0 && EnumFontData->BufferNum == EnumFontData->FontNum )
		{
			return FALSE ;
		}
	}

	// �I��
	return TRUE ;
}

// �t�H���g�񋓗p�R�[���o�b�N�֐�
int CALLBACK EnumFontFamExProcEx( ENUMLOGFONTEXW *lpelf, NEWTEXTMETRICEXW * /*lpntm*/, int /*nFontType*/, LPARAM lParam )
{
	ENUMFONTDATA *EnumFontData = ( ENUMFONTDATA * )lParam ;

	// �t�H���g�o�b�t�@�������ȏꍇ�̓t�H���g���̂݃`�F�b�N����
	if( EnumFontData->FontBuffer == NULL )
	{
		if( EnumFontData->EnumFontName != NULL )
		{
			if( _WCSCMP( &lpelf->elfFullName[ 0 ], EnumFontData->EnumFontName ) == 0 )
			{
				EnumFontData->Valid = TRUE ;
			}
		}
	}
	else
	{
		// �������t�H���g(@�t)�͂�������͂���
		if( lpelf->elfFullName[0] != L'@' )
		{
			int i ;

			// �����t�H���g�����ȑO�ɂ��������ꍇ�͒e��
			for( i = 0 ; i < EnumFontData->FontNum ; i ++ )
			{
				if( _WCSCMP( &lpelf->elfFullName[0], &EnumFontData->FontBuffer[64 * i] ) == 0 )
				{
					return TRUE ;
				}
			}

			// �l�[����ۑ�����
			_WCSCPY_S( &EnumFontData->FontBuffer[ 64 * EnumFontData->FontNum ], sizeof( wchar_t ) * 64, &lpelf->elfFullName[0] ) ;

			// �t�H���g�̐��𑝂₷
			EnumFontData->FontNum ++ ;

			// �����o�b�t�@�̐������E�ɗ��Ă�����񋓏I��
			if( EnumFontData->BufferNum != 0 && EnumFontData->BufferNum == EnumFontData->FontNum )
			{
				return FALSE ;
			}
		}
	}

	// �I��
	return TRUE ;
}

// EnumFontName �̊��ˑ��������s���֐�
extern int EnumFontName_PF( ENUMFONTDATA *EnumFontData, int IsEx, int CharSet )
{
	HDC			hdc ;
	LOGFONTW	LogFont ;

	// �f�o�C�X�R���e�L�X�g���擾
	hdc = GetDC( NULL );

	// �񋓊J�n
	_MEMSET( &LogFont, 0, sizeof( LOGFONT ) ) ;
	if( IsEx )
	{
		LogFont.lfCharSet		= ( BYTE )( CharSet < 0 ? DEFAULT_CHARSET : CharSetTable[ CharSet ] ) ;
	}
	else
	{
		LogFont.lfCharSet		= DEFAULT_CHARSET ;
	}
	if( EnumFontData->EnumFontName != NULL )
	{
		_WCSNCPY_S( LogFont.lfFaceName, sizeof( LogFont.lfFaceName ), EnumFontData->EnumFontName, 31 ) ;
	}
	else
	{
		LogFont.lfFaceName[0]	= L'\0' ;
	}
	LogFont.lfPitchAndFamily	= 0 ;
	EnumFontFamiliesExW( hdc, &LogFont, ( FONTENUMPROCW )( IsEx ? EnumFontFamExProcEx : EnumFontFamExProc ), ( LPARAM )EnumFontData, 0  ) ;

	// �f�o�C�X�R���e�L�X�g�̉��
	ReleaseDC( NULL, hdc ) ;

	// ����I��
	return 0 ;
}



#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

