//-----------------------------------------------------------------------------
// 
// 		�c�w���C�u����		WindowsOS�p�`�揈���v���O����
// 
//  	Ver 3.16d
// 
//-----------------------------------------------------------------------------

// �c�w���C�u�����쐬���p��`
#define __DX_MAKE

#include "DxGraphicsWin.h"

#ifndef DX_NON_GRAPHICS

// �C���N���[�h ---------------------------------------------------------------
#include "DxWindow.h"
#include "DxWinAPI.h"
#include "../DxSystem.h"
#include "../DxLog.h"
#include "../DxModel.h"
#include "../DxMath.h"
#include "../DxBaseFunc.h"
#include "../DxGraphics.h"
#include "../DxASyncLoad.h"
#include "../DxMask.h"

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// �}�N����` -----------------------------------------------------------------

// �\���̐錾 -----------------------------------------------------------------

// �f�[�^��` -----------------------------------------------------------------

static int UseGDIFlag ;							// GDI ���g�p���邩�ǂ���

GRAPHICSMANAGE_WIN GraphicsManage_Win ; 

// �֐��錾 -------------------------------------------------------------------

static int UseChangeDisplaySettings( void ) ;									// ChangeDisplaySettings ���g�p���ĉ�ʃ��[�h��ύX����

// �v���O���� -----------------------------------------------------------------

// DirectX �̃I�u�W�F�N�g���������
extern void Graphics_ReleaseDirectXObject( void )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		Graphics_D3D11_ReleaseDirectXObject_PF() ;
		break ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		Graphics_D3D9_ReleaseDirectXObject_PF() ;
		break ;
#endif // DX_NON_DIRECT3D9

	default :
		break ;
	}
}

// ���b�Z�[�W���[�v���ɍs���ׂ����������s����
extern void Graphics_Win_MessageLoop_Process( void )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		break ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		// �Ǘ��e�N�X�`���ւ̓]���p�̃V�X�e���������e�N�X�`���̒���������s��
		Graphics_D3D9_SysMemTextureProcess() ;

		// �Ǘ��e�N�X�`���ւ̓]���p�̃V�X�e���������T�[�t�F�X�̒���������s��
		Graphics_D3D9_SysMemSurfaceProcess() ;
		break ;
#endif // DX_NON_DIRECT3D9

	default :
		break ;
	}
}

// WM_ACTIVATE ���b�Z�[�W�̏����ŁA�E�C���h�E���[�h���ɃA�N�e�B�u�ɂȂ����ۂɌĂ΂��֐�
extern void Graphics_Win_WM_ACTIVATE_ActiveProcess( void )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		Graphics_D3D11_WM_ACTIVATE_ActiveProcess_PF() ;
		break ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		Graphics_D3D9_WM_ACTIVATE_ActiveProcess_PF() ;
		break ;
#endif // DX_NON_DIRECT3D9

	default :
		break ;
	}
}





// �ݒ�֌W�֐�

// �_�C�A���O�{�b�N�X���[�h��ύX����
extern	int		Graphics_Win_SetDialogBoxMode( int Flag )
{
#ifndef DX_NON_DIRECT3D9
	return Graphics_D3D9_SetDialogBoxMode_PF( Flag ) ;
#else // DX_NON_DIRECT3D9
	return 0 ;
#endif // DX_NON_DIRECT3D9
}








// ����ʂ̓��e���w��̃E�C���h�E�ɓ]������
extern int NS_BltBackScreenToWindow( HWND Window, int ClientX, int ClientY )
{
	RECT SrcRect, DestRect, WinRect ;

	GetClientRect( Window, &WinRect ) ;

	SrcRect.left   = 0 ;
	SrcRect.top    = 0 ;
	SrcRect.right  = GSYS.Screen.MainScreenSizeX ;
	SrcRect.bottom = GSYS.Screen.MainScreenSizeY ;

	DestRect.left   = WinRect.left + ClientX ;
	DestRect.top    = WinRect.top  + ClientY ;
	DestRect.right  = WinRect.left + ClientX + GSYS.Screen.MainScreenSizeX ;
	DestRect.bottom = WinRect.top  + ClientY + GSYS.Screen.MainScreenSizeY ;

	if( DestRect.left < WinRect.left )
	{
		SrcRect.left += WinRect.left - DestRect.left ;
		DestRect.left = WinRect.left ;
	}

	if( DestRect.top < WinRect.top )
	{
		SrcRect.top += WinRect.top - DestRect.top ;
		DestRect.top = WinRect.top ;
	}

	if( DestRect.right > WinRect.right )
	{
		SrcRect.right -= DestRect.right - WinRect.right ;
		DestRect.right = WinRect.right ;
	}

	if( DestRect.bottom > WinRect.bottom )
	{
		SrcRect.bottom -= DestRect.bottom - WinRect.bottom ;
		DestRect.bottom = WinRect.bottom ;
	}

	return NS_BltRectBackScreenToWindow( Window, SrcRect, DestRect ) ;
}

// ����ʂ̎w��̗̈���E�C���h�E�̃N���C�A���g�̈�̎w��̗̈�ɓ]������
extern int NS_BltRectBackScreenToWindow( HWND Window, RECT BackScreenRect, RECT WindowClientRect )
{
	// ����������
	if( DxSysData.DxLib_InitializeFlag == FALSE || DxLib_GetEndRequest() ) return -1 ;

	// �t���X�N���[�����[�h�ł͎g�p�ł��Ȃ�
	if( NS_GetWindowModeFlag() == FALSE ) return -1 ;

	// ���s
	return Graphics_BltRectBackScreenToWindow_PF( Window, BackScreenRect, WindowClientRect ) ;
}

// ScreenFlip �ŉ摜��]�������̃E�C���h�E��ݒ肷��( NULL ���w�肷��Ɛݒ���� )
extern int NS_SetScreenFlipTargetWindow( HWND TargetWindow )
{
	return Graphics_SetScreenFlipTargetWindow_PF( TargetWindow ) ;
}

// �f�X�N�g�b�v��ʂ���w��̈�̉摜�����O���t�B�b�N�n���h���ɓ]������
extern int NS_GetDesktopScreenGraph( int x1, int y1, int x2, int y2, int GrHandle, int DestX, int DestY )
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
	int GraphW, GraphH ;

	// �O���t�B�b�N�n���h���̃T�C�Y���擾
	if( NS_GetGraphSize( GrHandle, &GraphW, &GraphH ) < 0 )
	{
		return -1 ;
	}

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
	if( DestX + CaptureW > GraphW )
	{
		CaptureW = GraphW - DestX ;
		x2       = x1 + CaptureW ;
	}
	if( DestY + CaptureH > GraphH )
	{
		CaptureH = GraphH - DestY ;
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

	// �f�X�N�g�b�v�C���[�W���O���t�B�b�N�n���h���ɓ]��
	{
		RECT SrcRect ;

		_MEMSET( &CaptureImage, 0, sizeof( CaptureImage ) ) ;
		NS_CreateXRGB8ColorData( &CaptureImage.ColorData ) ;
		CaptureImage.Width		= CaptureW ;
		CaptureImage.Height		= CaptureH ;
		CaptureImage.Pitch		= CaptureW * 4 ;
		CaptureImage.GraphData	= ( void * )ImageBuffer ;
		SrcRect.left			= 0 ;
		SrcRect.top				= 0 ;
		SrcRect.right			= CaptureW ;
		SrcRect.bottom			= CaptureH ;
		NS_BltBmpOrGraphImageToGraph2( NULL, NULL, NULL, FALSE, &CaptureImage, NULL, &SrcRect, DestX, DestY, GrHandle ) ;
	}

	// DIB�̍폜
	DeleteDC( HMemDC ) ;
	DeleteObject( HBitmap ) ;

	// �I��
	return 0 ;
}

// �g�p���� DirectDraw �f�o�C�X�̃C���f�b�N�X��ݒ肷��
extern int NS_SetUseDirectDrawDeviceIndex( int Index )
{
	return NS_SetUseDisplayIndex( Index ) ;
}

// DirectDraw �� Direct3D �̋������x�����}���`�X���b�h�Ή��ɂ��邩�ǂ������Z�b�g����
extern	int		NS_SetMultiThreadFlag( int Flag )
{
	// �������O�̂ݎg�p�\ 
	if( DxSysData.DxLib_InitializeFlag == TRUE ) return -1 ;

	GRAWIN.Setting.UseMultiThread = Flag ;

	// �I��
	return 0 ;
}

// Vista�ȍ~�� Windows Aero �𖳌��ɂ��邩�ǂ������Z�b�g����ATRUE:�����ɂ���  FALSE:�L���ɂ���( DxLib_Init �̑O�ɌĂԕK�v������܂� )
extern int NS_SetAeroDisableFlag( int Flag )
{
	return Graphics_SetAeroDisableFlag_PF( Flag ) ;
}

// Vista�ȍ~�̊��� Direct3D9Ex ���g�p���邩�ǂ�����ݒ肷��( TRUE:�g�p����( �f�t�H���g )  FALSE:�g�p���Ȃ� )
extern int NS_SetUseDirect3D9Ex( int Flag )
{
#ifndef DX_NON_DIRECT3D9
	return Graphics_D3D9_SetUseDirect3D9Ex_PF( Flag ) ;
#else // DX_NON_DIRECT3D9
	return 0 ;
#endif // DX_NON_DIRECT3D9
}

// Direct3D11 ���g�p���邩�ǂ�����ݒ肷��( TRUE:�g�p����  FALSE:�g�p���Ȃ� )
extern int NS_SetUseDirect3D11( int Flag )
{
	// �������O�̂ݎg�p�\ 
	if( DxSysData.DxLib_InitializeFlag == TRUE ) return -1 ;

	// �t���O��ۑ�
	GRAWIN.Setting.NotUseDirect3D11 = Flag ? FALSE : TRUE ;

	// �I��
	return 0 ;
}

// Direct3D11 �Ŏg�p����Œ�@�\���x�����w�肷��֐��ł��A���ADX_DIRECT3D_11_FEATURE_LEVEL_11_0 ���Ⴂ�@�\���x���ł̐���ȓ���͕ۏ؂��܂���( �f�t�H���g�� DX_DIRECT3D_11_FEATURE_LEVEL_11_0 )
extern int NS_SetUseDirect3D11MinFeatureLevel( int Level /* DX_DIRECT3D_11_FEATURE_LEVEL_10_0 �Ȃ� */ )
{
#ifndef DX_NON_DIRECT3D11
	return Graphics_D3D11_SetUseDirect3D11MinFeatureLevel_PF( Level ) ;
#else // DX_NON_DIRECT3D11
	return 0 ;
#endif // DX_NON_DIRECT3D11
}

// �g�p���� Direct3D �̃o�[�W������ݒ肷��
extern int NS_SetUseDirect3DVersion( int Version )
{
	// �������O�̂ݗL��
	if( DxSysData.DxLib_InitializeFlag )
	{
		return -1 ;
	}

	// �o�[�W�����ɂ���ď�����ύX
	switch( Version )
	{
	case DX_DIRECT3D_NONE :
		NS_SetUseDirectDrawFlag( FALSE ) ;
		NS_SetUseDirect3D11( FALSE ) ;
		break ;

	case DX_DIRECT3D_9 :
		NS_SetUseDirect3D9Ex( FALSE ) ;
		break ;

	case DX_DIRECT3D_9EX :
		NS_SetUseDirect3D9Ex( TRUE ) ;
		break ;

	case DX_DIRECT3D_11 :
		NS_SetUseDirectDrawFlag( TRUE ) ;
		NS_SetUseDirect3D11( TRUE ) ;
		break ;

	default :
		return -1 ;
	}

	// �I��
	return 0 ;
}

// �g�p���Ă��� Direct3D �̃o�[�W�������擾����( DX_DIRECT3D_9 �Ȃ� )
extern int NS_GetUseDirect3DVersion( void )
{
#ifndef DX_NON_DIRECT3D9
	if( Direct3D9_IsValid() )
	{
		if( Direct3D9_IsExObject() )
		{
			return DX_DIRECT3D_9EX ;
		}
		else
		{
			return DX_DIRECT3D_9 ;
		}
	}
#endif // DX_NON_DIRECT3D9

#ifndef DX_NON_DIRECT3D11
	if( D3D11Device_IsValid() )
	{
		return DX_DIRECT3D_11 ;
	}
#endif // DX_NON_DIRECT3D11

	return DX_DIRECT3D_NONE ;
}

// �g�p���Ă��� Direct3D11 �� FeatureLevel ( DX_DIRECT3D_11_FEATURE_LEVEL_9_1 �� )���擾����( �߂�l�@-1�F�G���[�@-1�ȊO�FFeature Level )
extern int NS_GetUseDirect3D11FeatureLevel(	void )
{
#ifndef DX_NON_DIRECT3D11
	if( D3D11Device_IsValid() == FALSE )
	{
		return -1 ;
	}

	return GD3D11.Setting.FeatureLevel ;
#else  // DX_NON_DIRECT3D11
	return -1 ;
#endif // DX_NON_DIRECT3D11
}

// �c�����������c���������g�p���邩�ǂ������Z�b�g����
extern int NS_SetUseDirectDrawFlag( int Flag )
{
	// �������O�̂ݎg�p�\ 
	if( DxSysData.DxLib_InitializeFlag == TRUE ) return -1 ;

	GSYS.Setting.NotUseHardware = !Flag ;

	// �I��
	return 0 ;
}

// �f�c�h�`���K�v�Ƃ��邩�A��ύX����
extern	int		NS_SetUseGDIFlag( int Flag )
{
	// �t���O��ۑ�
	UseGDIFlag = Flag ;

	// �n�[�h�E�G�A���g�p���Ă��邩�ǂ����ŏ����𕪊�
	if( GSYS.Setting.ValidHardware )
	{
		switch( GRAWIN.Setting.UseGraphicsAPI )
		{
#ifndef DX_NON_DIRECT3D11
		case GRAPHICS_API_DIRECT3D11_WIN32 :
			break  ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
		case GRAPHICS_API_DIRECT3D9_WIN32 :
			// �n�[�h�E�G�A�����_�����O���[�h�̏ꍇ
			if( Direct3DDevice9_IsValid() == 0 ) return 0 ;

			// �f�o�C�X�̐ݒ�
			if( Flag == FALSE )
			{
				Direct3DDevice9_SetDialogBoxMode( FALSE ) ;
			}
			else
			{
				Direct3DDevice9_SetDialogBoxMode( FALSE ) ;
				Direct3DDevice9_SetDialogBoxMode( TRUE ) ;
			}
			break ;
#endif // DX_NON_DIRECT3D9

		default :
			break ;
	}
	}
	else
	{
		// �\�t�g�E�G�A�����_�����O���[�h�ł͓��ɂ��邱�Ɩ���
	}

	// �I��
	return 0 ;
}

// �f�c�h�`���K�v�Ƃ��邩�̏�Ԃ��擾����
extern int NS_GetUseGDIFlag( void )
{
	return UseGDIFlag ;
}

// �c�����������c���������g�p����f�t�h�c��ݒ肷��
extern	int		NS_SetDDrawUseGuid( const GUID FAR *Guid )
{
#ifndef DX_NON_DIRECT3D9
	return Graphics_D3D9_SetDDrawUseGuid_PF( Guid ) ;
#else // DX_NON_DIRECT3D9
	return 0 ;
#endif // DX_NON_DIRECT3D9
}

/*�߂�l�� IDirectDraw7 * �ɃL���X�g���ĉ�����*/			// ���ݎg�p���Ă���c�����������c�������I�u�W�F�N�g�̃A�h���X���擾����
extern	const void *NS_GetUseDDrawObj( void )
{
	// �I��
	return DirectDraw7_GetObject() ;
}

// �L���� DirectDraw �f�o�C�X�� GUID ���擾����
extern	const GUID *				NS_GetDirectDrawDeviceGUID( int Number )
{
#ifndef DX_NON_DIRECT3D9
	return Graphics_D3D9_GetDirectDrawDeviceGUID_PF( Number ) ;
#else // DX_NON_DIRECT3D9
	return 0 ;
#endif // DX_NON_DIRECT3D9
}

// �L���� DirectDraw �f�o�C�X�̖��O�𓾂�
extern	int						NS_GetDirectDrawDeviceDescription( int Number, char *StringBuffer )
{
#ifndef DX_NON_DIRECT3D9
	return Graphics_D3D9_GetDirectDrawDeviceDescription_PF( Number, StringBuffer ) ;
#else // DX_NON_DIRECT3D9
	return 0 ;
#endif // DX_NON_DIRECT3D9
}

// �L���� DirectDraw �f�o�C�X�̐����擾����
extern	int						NS_GetDirectDrawDeviceNum( void )
{
#ifndef DX_NON_DIRECT3D9
	return Graphics_D3D9_GetDirectDrawDeviceNum_PF() ;
#else // DX_NON_DIRECT3D9
	return 0 ;
#endif // DX_NON_DIRECT3D9
}

// �g�p���̂c�����������R�c�c�����������X�I�u�W�F�N�g�𓾂�
extern	const void * NS_GetUseDirect3DDevice9( void )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return NULL ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_Hardware_D3D9_GetUseDirect3DDevice9_PF(  ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return NULL ;
	}
}

// �g�p���̃o�b�N�o�b�t�@��Direct3DSurface9�I�u�W�F�N�g���擾����
extern const void * NS_GetUseDirect3D9BackBufferSurface( void )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return NULL ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_Hardware_D3D9_GetUseDirect3D9BackBufferSurface_PF(  ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return NULL ;
	}
}

// �g�p����ID3D11Device�I�u�W�F�N�g���擾����( �߂�l�� ID3D11Device * �ɃL���X�g���ĉ����� )
extern	const void*	NS_GetUseDirect3D11Device( void )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_Hardware_D3D11_GetUseDirect3D11Device_PF(  ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return NULL ;
#endif // DX_NON_DIRECT3D9

	default :
		return NULL ;
	}
}

// �g�p����ID3D11DeviceContext�I�u�W�F�N�g���擾����( �߂�l�� ID3D11DeviceContext * �ɃL���X�g���ĉ����� )
extern	const void*	NS_GetUseDirect3D11DeviceContext( void )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_Hardware_D3D11_GetUseDirect3D11DeviceContext_PF(  ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return NULL ;
#endif // DX_NON_DIRECT3D9

	default :
		return NULL ;
	}
}

// �g�p���̃o�b�N�o�b�t�@��ID3D11Texture2D�I�u�W�F�N�g���擾����( �߂�l�� ID3D11Texture2D * �ɃL���X�g���Ă������� )
extern	const void*	NS_GetUseDirect3D11BackBufferTexture2D( void )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_Hardware_D3D11_GetUseDirect3D11BackBufferTexture2D_PF(  ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return NULL ;
#endif // DX_NON_DIRECT3D9

	default :
		return NULL ;
	}
}

// �g�p���̐[�x�X�e���V���o�b�t�@��ID3D11Texture2D�I�u�W�F�N�g���擾����( �߂�l�� ID3D11Texture2D * �ɃL���X�g���Ă������� )
extern	const void*	NS_GetUseDirect3D11DepthStencilTexture2D( void )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_Hardware_D3D11_GetUseDirect3D11DepthStencilTexture2D_PF(  ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return NULL ;
#endif // DX_NON_DIRECT3D9

	default :
		return NULL ;
	}
}

// �c�w���C�u�����̂c�����������R�c�ݒ�����Ȃ���
extern int NS_RefreshDxLibDirect3DSetting( void )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_Hardware_D3D11_RefreshDxLibDirect3DSetting_PF(  ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_Hardware_D3D9_RefreshDxLibDirect3DSetting_PF(  ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}























// ���ˑ��������֌W

// ChangeDisplaySettings ���g�p���ĉ�ʃ��[�h��ύX����
static int UseChangeDisplaySettings( void )
{
//	HRESULT hr ;
//	int     DesktopWidth ;
//	int     DesktopHeight ;
//	int     DesktopColorBitDepth ;
//	int     ScreenSizeX ;
//	int     ScreenSizeY ;
	DEVMODE DevMode ;

	// ���̉�ʂ̑傫���𓾂�
//	NS_GetDefaultState( &DesktopWidth, &DesktopHeight, &DesktopColorBitDepth ) ;

	DXST_ERRORLOGFMT_ADDUTF16LE(( "\x3b\x75\x62\x97\xe2\x30\xfc\x30\xc9\x30\x6e\x30\x09\x59\xf4\x66\xe6\x51\x06\x74\x92\x30\x8b\x95\xcb\x59\x57\x30\x7e\x30\x59\x30\x20\x00\x25\x00\x75\x00\x20\x00\x78\x00\x20\x00\x25\x00\x75\x00\x20\x00\x20\x00\x25\x00\x75\x00\x20\x00\x62\x00\x69\x00\x74\x00\x20\x00\x00"/*@ L"��ʃ��[�h�̕ύX�������J�n���܂� %u x %u  %u bit " @*/, GSYS.Screen.MainScreenSizeX, GSYS.Screen.MainScreenSizeY, GSYS.Screen.MainScreenColorBitDepth )) ;
	DXST_ERRORLOG_TABADD ;

	// �t���X�N���[�����[�h�̏����Z�b�g�A�b�v
	Graphics_Screen_SetupFullScreenModeInfo() ;

	// �Ή����Ă���J���[�r�b�g�[�x�� 16 �� 32 �̂�
//	if( GSYS.Screen.MainScreenColorBitDepth != 16 &&
//		GSYS.Screen.MainScreenColorBitDepth != 32 )
	if( GSYS.Screen.FullScreenUseDispModeData.ColorBitDepth != 16 &&
		GSYS.Screen.FullScreenUseDispModeData.ColorBitDepth != 32 )
	{
		return -1 ;
	}

	// �f�B�X�v���C���[�h�̕ύX
	DXST_ERRORLOG_ADDUTF16LE( "\x3b\x75\x62\x97\xe3\x89\xcf\x50\xa6\x5e\x92\x30\x09\x59\xf4\x66\x57\x30\x7e\x30\x59\x30\x2e\x00\x2e\x00\x2e\x00\x20\x00\x00"/*@ L"��ʉ𑜓x��ύX���܂�... " @*/ ) ;

	// Win32 API ���g���ĉ�ʃ��[�h��ύX����
	{
		// �p�����[�^�Z�b�g
		_MEMSET( &DevMode, 0, sizeof( DevMode ) ) ;
		DevMode.dmSize             = sizeof( DevMode ) ;
		DevMode.dmBitsPerPel       = ( DWORD )GSYS.Screen.MainScreenColorBitDepth ;
		DevMode.dmFields           = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT ;
		if( GSYS.Screen.MainScreenRefreshRate != 0 )
		{
			DevMode.dmDisplayFrequency  = ( DWORD )GSYS.Screen.MainScreenRefreshRate ;
			DevMode.dmFields           |= DM_DISPLAYFREQUENCY ;
		}
		DevMode.dmPelsWidth        = ( DWORD )GSYS.Screen.FullScreenUseDispModeData.Width ;
		DevMode.dmPelsHeight       = ( DWORD )GSYS.Screen.FullScreenUseDispModeData.Height ;

		// ��ʃ��[�h�`�F���W
		if( ChangeDisplaySettings( &DevMode, CDS_FULLSCREEN ) != DISP_CHANGE_SUCCESSFUL )
		{
			// ���s�����烊�t���b�V�����[�g�w�薳���ł�����x����
			GSYS.Screen.MainScreenRefreshRate = 0 ;
			DevMode.dmDisplayFrequency        = 0 ;
			DevMode.dmFields                 &= ~DM_DISPLAYFREQUENCY ;
			if( ChangeDisplaySettings( &DevMode, 0/*CDS_FULLSCREEN*/ ) != DISP_CHANGE_SUCCESSFUL )
			{
#if 0
				// ����ł��ʖڂŁA����ʃ��[�h�� 320x240 �������ꍇ��
				// �[�� 320x240 ���[�h�Ŏ���
				if( GSYS.Screen.MainScreenSizeX == 320 && GSYS.Screen.MainScreenSizeY == 240 )
				{
					Graphics_Screen_SetMainScreenSize( 640, 480 ) ;
					NS_SetEmulation320x240( TRUE ) ;
					DevMode.dmPelsWidth        = GSYS.Screen.MainScreenSizeX ;
					DevMode.dmPelsHeight       = GSYS.Screen.MainScreenSizeY ;
					if( ChangeDisplaySettings( &DevMode, 0/*CDS_FULLSCREEN*/ ) != DISP_CHANGE_SUCCESSFUL )
					{
						goto ERRORLABEL ;
					}
					SetWindowStyle() ;
				}
				else
#endif
				{
//ERRORLABEL:
					DXST_ERRORLOG_ADDUTF16LE( "\x3b\x75\x62\x97\xe2\x30\xfc\x30\xc9\x30\x6e\x30\x09\x59\xf4\x66\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"��ʃ��[�h�̕ύX�Ɏ��s���܂���\n" @*/ ) ;
					DXST_ERRORLOG_TABSUB ;
					return -1 ;
				}
			}
		}
	}

	GRAWIN.UseChangeDisplaySettings = TRUE ;
	DXST_ERRORLOG_ADDUTF16LE( "\x10\x62\x9f\x52\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�������܂���\n" @*/ ) ;

	// 10���ʑS�̂����œh��Ԃ��w����c��
	GRAWIN.FullScreenBlackFillCounter = 10 ;

	return 0 ;
}

// �`�揈���̊��ˑ������̏��������s���֐�( ���s�ӏ���ʂO )
extern int Graphics_Initialize_Timing0_PF( void )
{
	int Result ;

	if( GSYS.Setting.NotUseHardware )
	{
		Result = -1 ;
	}
	else
	{
		Result                        = -1 ;
		GRAWIN.Setting.UseGraphicsAPI = GRAPHICS_API_NONE ;

#ifndef DX_NON_DIRECT3D11
		if( GRAWIN.Setting.NotUseDirect3D11 == FALSE )
		{
			if( Result < 0 )
			{
				GRAWIN.Setting.UseGraphicsAPI = GRAPHICS_API_DIRECT3D11_WIN32 ;
				Result = Graphics_D3D11_Initialize_Timing0_PF() ;
				if( Result < 0 )
				{
					GRAWIN.Setting.UseGraphicsAPI = GRAPHICS_API_NONE ;
				}
			}
		}
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
		if( Result < 0 )
		{
			GRAWIN.Setting.UseGraphicsAPI = GRAPHICS_API_DIRECT3D9_WIN32 ;
			Result = Graphics_D3D9_Initialize_Timing0_PF() ;
			if( Result < 0 )
			{
				GRAWIN.Setting.UseGraphicsAPI = GRAPHICS_API_NONE ;
			}
		}
#endif // DX_NON_DIRECT3D9
	}

	// �n�[�h�E�F�A�@�\���g�p�ł��Ȃ�( ���Ȃ� )�ꍇ�̓\�t�g�E�G�A�����_�����O���[�h�̏������s��
	if( Result < 0 )
	{
		// DirectDraw7 ���쐬����
		if( DirectDraw7_Create() < 0 )
		{
			return -1 ;
		}

		// �t���X�N���[�����[�h�̏ꍇ�͂����ŉ�ʃ��[�h��ύX����
		if( NS_GetWindowModeFlag() == FALSE )
		{
			if( UseChangeDisplaySettings() < 0 )
			{
				DirectDraw7_Release() ;
				return -1 ;
			}
		}

		// �u�r�x�m�b�҂��̎��Ԃ��擾����
		DirectDraw7_WaitVSyncInitialize() ;
	}

	// ����I��
	return 0 ;
}

// �`�揈���̊��ˑ������̏��������s���֐�( ���s�ӏ���ʂP )
extern int Graphics_Initialize_Timing1_PF( void )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		if( Graphics_D3D11_Initialize_Timing1_PF() < 0 )
		{
			return -1 ;
		}
		break ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		if( Graphics_D3D9_Initialize_Timing1_PF() < 0 )
		{
			return -1 ;
		}
#endif // DX_NON_DIRECT3D9
		break ;

	default :
		break ;
	}

	// ����I��
	return 0 ;
}

// �n�[�h�E�G�A�A�N�Z�����[�^���g�p����ꍇ�̊��ˑ��̏������������s��
extern	int		Graphics_Hardware_Initialize_PF( void )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_D3D11_Hardware_Initialize_PF() ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_D3D9_Hardware_Initialize_PF() ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// �`�揈���̊��ˑ������̌�n�����s���֐�
extern	int		Graphics_Terminate_PF( void )
{
	int Result ;

	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		Result = Graphics_D3D11_Terminate_PF() ;
		break ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		Result = Graphics_D3D9_Terminate_PF() ;
#endif // DX_NON_DIRECT3D9

	default :
		Result = 0 ;
		break ;
	}

	// �f�B�X�v���C���p�Ɋm�ۂ����������̉��
	if( GSYS.Screen.DisplayInfo )
	{
		HeapFree( GetProcessHeap(), 0, GSYS.Screen.DisplayInfo ) ;
		GSYS.Screen.DisplayInfo = NULL ;
	}

	return Result ;
}

// �O���t�B�b�N�X�V�X�e���̕��A�A���͕ύX�t���̍ăZ�b�g�A�b�v���s��
extern int Graphics_RestoreOrChangeSetupGraphSystem_PF(
	int Change,
	int ScreenSizeX,
	int ScreenSizeY,
	int ColorBitDepth,
	int RefreshRate
)
{
#ifndef DX_NON_ASYNCLOAD
	// �񓯊��ǂݍ��݂̓r���̏ꍇ�́A�񓯊��ǂݍ��݂��I������܂ő҂�
	while( NS_GetASyncLoadNum() > 0 )
	{
		ProcessASyncLoadRequestMainThread() ;
		if( WinData.ProcessorNum <= 1 )
		{
			Thread_Sleep( 2 ) ;
		}
		else
		{
			Thread_Sleep( 0 ) ;
		}
	}
#endif // DX_NON_ASYNCLOAD

	// �n�[�h�E�G�A�̋@�\���g�p���邩�ǂ����ŏ����𕪊�
	if( GSYS.Setting.ValidHardware == TRUE )
	{
		switch( GRAWIN.Setting.UseGraphicsAPI )
		{
#ifndef DX_NON_DIRECT3D11
		case GRAPHICS_API_DIRECT3D11_WIN32 :
			return Graphics_D3D11_RestoreOrChangeSetupGraphSystem_PF( Change, ScreenSizeX, ScreenSizeY, ColorBitDepth, RefreshRate ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
		case GRAPHICS_API_DIRECT3D9_WIN32 :
			return Graphics_D3D9_RestoreOrChangeSetupGraphSystem_PF( Change, ScreenSizeX, ScreenSizeY, ColorBitDepth, RefreshRate ) ;
#endif // DX_NON_DIRECT3D9

		default :
			return 0 ;
		}
	}
	else
	{
		// �\�t�g�E�G�A�����_�����O���[�h

		if( DxSysData.NotDrawFlag == FALSE )
		{
			// DirectDraw7 �̉��
			DirectDraw7_Release() ;
		}

		if( Change == TRUE )
		{
			// ��ʃ��[�h�̃Z�b�g
			Graphics_Screen_SetMainScreenSize( ScreenSizeX, ScreenSizeY ) ;
			GSYS.Screen.MainScreenColorBitDepth = ColorBitDepth ;
			GSYS.Screen.MainScreenRefreshRate   = RefreshRate ;
	//		SetMemImgDefaultColorType( ColorBitDepth == 32 ? 1 : 0 ) ;
		}

		if( DxSysData.NotDrawFlag == FALSE )
		{
			GSYS.Screen.FullScreenResolutionModeAct = GSYS.Screen.FullScreenResolutionMode ;

			// DirectDraw7 ���쐬����
			if( DirectDraw7_Create() < 0 )
			{
				return -1 ;
			}

			// �t���X�N���[�����[�h�̏ꍇ�͂����ŉ�ʃ��[�h��ύX����
			if( NS_GetWindowModeFlag() == FALSE )
			{
				if( UseChangeDisplaySettings() < 0 )
				{
					DirectDraw7_Release() ;
					return -1 ;
				}
			}

			// �u�r�x�m�b�҂��̎��Ԃ��擾����
			DirectDraw7_WaitVSyncInitialize() ;
		}
	}

	// ����I��
	return 0 ;
}

// �`��p�f�o�C�X���L�����ǂ������擾����
extern int Graphics_Hardware_CheckValid_PF( void )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_D3D11_Hardware_CheckValid_PF() ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_D3D9_Hardware_CheckValid_PF() ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}



























// ���ˑ��`��ݒ�֌W

// ���C���E�C���h�E�̔w�i�F��ݒ肷��( Red,Green,Blue:���ꂼ�� �O�`�Q�T�T )
extern	int		Graphics_Hardware_SetBackgroundColor_PF( int Red, int Green, int Blue )
{
	return 0 ;
}

// �V�F�[�_�[�`��ł̕`����ݒ肷��
extern	int		Graphics_Hardware_SetRenderTargetToShader_PF( int TargetIndex, int DrawScreen, int SurfaceIndex )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_Hardware_D3D11_SetRenderTargetToShader_PF(  TargetIndex,  DrawScreen,  SurfaceIndex ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_Hardware_D3D9_SetRenderTargetToShader_PF(  TargetIndex,  DrawScreen,  SurfaceIndex ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// SetDrawBright �̈��������
extern	int		Graphics_Hardware_SetDrawBrightToOneParam_PF( DWORD Bright )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_Hardware_D3D11_SetDrawBrightToOneParam_PF( Bright ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_Hardware_D3D9_SetDrawBrightToOneParam_PF( Bright ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// �`��u�����h���[�h���Z�b�g����
extern	int		Graphics_Hardware_SetDrawBlendMode_PF( int BlendMode, int BlendParam )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_Hardware_D3D11_SetDrawBlendMode_PF(  BlendMode,  BlendParam ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_Hardware_D3D9_SetDrawBlendMode_PF(  BlendMode,  BlendParam ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// �`�掞�̃A���t�@�e�X�g�̐ݒ���s��( TestMode:DX_CMP_GREATER��( -1:�f�t�H���g����ɖ߂� )  TestParam:�`��A���t�@�l�Ƃ̔�r�Ɏg�p����l )
extern	int		Graphics_Hardware_SetDrawAlphaTest_PF( int TestMode, int TestParam )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_Hardware_D3D11_SetDrawAlphaTest_PF(  TestMode,  TestParam ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_Hardware_D3D9_SetDrawAlphaTest_PF(  TestMode,  TestParam ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// �`�惂�[�h���Z�b�g����
extern	int		Graphics_Hardware_SetDrawMode_PF( int DrawMode )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_Hardware_D3D11_SetDrawMode_PF(  DrawMode ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_Hardware_D3D9_SetDrawMode_PF(  DrawMode ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// �`��P�x���Z�b�g
extern	int		Graphics_Hardware_SetDrawBright_PF( int RedBright, int GreenBright, int BlueBright )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_Hardware_D3D11_SetDrawBright_PF(  RedBright,  GreenBright,  BlueBright ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_Hardware_D3D9_SetDrawBright_PF(  RedBright,  GreenBright,  BlueBright ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// SetBlendGraphParam �̉ϒ������p�����[�^�t��
extern	int		Graphics_Hardware_SetBlendGraphParamBase_PF( IMAGEDATA *BlendImage, int BlendType, int *Param )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_Hardware_D3D11_SetBlendGraphParamBase_PF(  BlendImage,  BlendType, Param ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_Hardware_D3D9_SetBlendGraphParamBase_PF(  BlendImage,  BlendType, Param ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// �ő�ٕ����̒l���Z�b�g����
extern	int		Graphics_Hardware_SetMaxAnisotropy_PF( int MaxAnisotropy )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_Hardware_D3D11_SetMaxAnisotropy_PF(  MaxAnisotropy ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_Hardware_D3D9_SetMaxAnisotropy_PF(  MaxAnisotropy ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// ���[���h�ϊ��p�s����Z�b�g����
extern	int		Graphics_Hardware_SetTransformToWorld_PF( const MATRIX *Matrix )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_Hardware_D3D11_SetTransformToWorld_PF( Matrix ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_Hardware_D3D9_SetTransformToWorld_PF( Matrix ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// �r���[�ϊ��p�s����Z�b�g����
extern	int		Graphics_Hardware_SetTransformToView_PF( const MATRIX *Matrix )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_Hardware_D3D11_SetTransformToView_PF( Matrix ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_Hardware_D3D9_SetTransformToView_PF( Matrix ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// ���e�ϊ��p�s����Z�b�g����
extern	int		Graphics_Hardware_SetTransformToProjection_PF( const MATRIX *Matrix )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_Hardware_D3D11_SetTransformToProjection_PF( Matrix ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_Hardware_D3D9_SetTransformToProjection_PF( Matrix ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// �r���[�|�[�g�s����Z�b�g����
extern	int		Graphics_Hardware_SetTransformToViewport_PF( const MATRIX *Matrix )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_Hardware_D3D11_SetTransformToViewport_PF( Matrix ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_Hardware_D3D9_SetTransformToViewport_PF( Matrix ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// �e�N�X�`���A�h���X���[�h��ݒ肷��
extern	int		Graphics_Hardware_SetTextureAddressMode_PF( int Mode /* DX_TEXADDRESS_WRAP �� */, int Stage )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_Hardware_D3D11_SetTextureAddressMode_PF( Mode /* DX_TEXADDRESS_WRAP �� */,  Stage ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_Hardware_D3D9_SetTextureAddressMode_PF( Mode /* DX_TEXADDRESS_WRAP �� */,  Stage ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// �e�N�X�`���A�h���X���[�h��ݒ肷��
extern	int		Graphics_Hardware_SetTextureAddressModeUV_PF( int ModeU, int ModeV, int Stage )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_Hardware_D3D11_SetTextureAddressModeUV_PF(  ModeU,  ModeV,  Stage ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_Hardware_D3D9_SetTextureAddressModeUV_PF(  ModeU,  ModeV,  Stage ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// �e�N�X�`�����W�ϊ��s����Z�b�g����
extern	int		Graphics_Hardware_SetTextureAddressTransformMatrix_PF( int UseFlag, MATRIX *Matrix, int Sampler )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_Hardware_D3D11_SetTextureAddressTransformMatrix_PF( UseFlag, Matrix, Sampler ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_Hardware_D3D9_SetTextureAddressTransformMatrix_PF( UseFlag, Matrix, Sampler ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// �t�H�O��L���ɂ��邩�ǂ�����ݒ肷��( TRUE:�L��  FALSE:���� )
extern	int		Graphics_Hardware_SetFogEnable_PF( int Flag )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_Hardware_D3D11_SetFogEnable_PF(  Flag ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_Hardware_D3D9_SetFogEnable_PF(  Flag ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// �t�H�O���[�h��ݒ肷��
extern	int		Graphics_Hardware_SetFogMode_PF( int Mode /* DX_FOGMODE_NONE �� */ )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_Hardware_D3D11_SetFogMode_PF(  Mode /* DX_FOGMODE_NONE �� */ ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_Hardware_D3D9_SetFogMode_PF(  Mode /* DX_FOGMODE_NONE �� */ ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// �t�H�O�J���[��ύX����
extern	int		Graphics_Hardware_SetFogColor_PF( DWORD FogColor )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_Hardware_D3D11_SetFogColor_PF( FogColor ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_Hardware_D3D9_SetFogColor_PF( FogColor ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// �t�H�O���n�܂鋗���ƏI�����鋗����ݒ肷��( 0.0f �` 1.0f )
extern	int		Graphics_Hardware_SetFogStartEnd_PF( float start, float end )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_Hardware_D3D11_SetFogStartEnd_PF(  start,  end ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_Hardware_D3D9_SetFogStartEnd_PF(  start,  end ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// �t�H�O�̖��x��ݒ肷��( 0.0f �` 1.0f )
extern	int		Graphics_Hardware_SetFogDensity_PF( float density )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_Hardware_D3D11_SetFogDensity_PF(  density ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_Hardware_D3D9_SetFogDensity_PF(  density ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// ���[���h�ϊ��p�s����Z�b�g����
extern	int		Graphics_Hardware_DeviceDirect_SetWorldMatrix_PF( const MATRIX *Matrix )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_D3D11_DeviceState_SetWorldMatrix( Matrix ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_D3D9_DeviceState_SetWorldMatrix( Matrix ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// �r���[�ϊ��p�s����Z�b�g����
extern	int		Graphics_Hardware_DeviceDirect_SetViewMatrix_PF( const MATRIX *Matrix )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_D3D11_DeviceState_SetViewMatrix( Matrix ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_D3D9_DeviceState_SetViewMatrix( Matrix ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// ���e�ϊ��p�s����Z�b�g����
extern	int		Graphics_Hardware_DeviceDirect_SetProjectionMatrix_PF( const MATRIX *Matrix )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_D3D11_DeviceState_SetProjectionMatrix( Matrix ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_D3D9_DeviceState_SetProjectionMatrix( Matrix ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// ��{�f�[�^�ɐݒ肳��Ă���t�H�O�����n�[�h�E�F�A�ɔ��f����
extern	int		Graphics_Hardware_ApplyLigFogToHardware_PF( void )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_Hardware_D3D11_ApplyLigFogToHardware_PF( ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_Hardware_D3D9_ApplyLigFogToHardware_PF( ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// �ȑO�� DrawModiGraph �֐��̃R�[�h���g�p���邩�ǂ����̃t���O���Z�b�g����
extern	int		Graphics_Hardware_SetUseOldDrawModiGraphCodeFlag_PF( int Flag )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_Hardware_D3D11_SetUseOldDrawModiGraphCodeFlag_PF( Flag ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_Hardware_D3D9_SetUseOldDrawModiGraphCodeFlag_PF( Flag ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// �`���ɐ��������l���������ނ��ǂ����̃t���O���X�V����
extern	int		Graphics_Hardware_RefreshAlphaChDrawMode_PF( void )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_Hardware_D3D11_RefreshAlphaChDrawMode_PF() ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_Hardware_D3D9_RefreshAlphaChDrawMode_PF() ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}























// ���ˑ��ݒ�֌W

// �n�[�h�E�G�A�̒��_���Z�����@�\���g�p���邩�ǂ�����ݒ肷��
extern	int		Graphics_Hardware_SetUseHardwareVertexProcessing_PF( int Flag )
{
#ifndef DX_NON_DIRECT3D9
	return Graphics_Hardware_D3D9_SetUseHardwareVertexProcessing_PF( Flag ) ;
#else // DX_NON_DIRECT3D9
	return 0 ;
#endif // DX_NON_DIRECT3D9
}

// �s�N�Z���P�ʂŃ��C�e�B���O���s�����ǂ�����ݒ肷��A�v ShaderModel 3.0( TRUE:�s�N�Z���P�ʂ̃��C�e�B���O���s��  FALSE:���_�P�ʂ̃��C�e�B���O���s��( �f�t�H���g ) )
extern	int		Graphics_Hardware_SetUsePixelLighting_PF( int Flag )
{
#ifndef DX_NON_DIRECT3D11
	Graphics_Hardware_D3D11_SetUsePixelLighting_PF( Flag ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	Graphics_Hardware_D3D9_SetUsePixelLighting_PF( Flag ) ;
#endif // DX_NON_DIRECT3D9

	return 0 ;
}

// �O���t�B�b�N�X�f�o�C�X�����X�g���畜�A�����ۂɌĂ΂��R�[���o�b�N�֐���ݒ肷��
extern	int		Graphics_Hardware_SetGraphicsDeviceRestoreCallbackFunction_PF( void (* Callback )( void *Data ), void *CallbackData )
{
#ifndef DX_NON_DIRECT3D11
	Graphics_Hardware_D3D11_SetGraphicsDeviceRestoreCallbackFunction_PF( Callback, CallbackData ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	Graphics_Hardware_D3D9_SetGraphicsDeviceRestoreCallbackFunction_PF( Callback, CallbackData ) ;
#endif // DX_NON_DIRECT3D9

	return 0 ;
}

// �O���t�B�b�N�X�f�o�C�X�����X�g���畜�A����O�ɌĂ΂��R�[���o�b�N�֐���ݒ肷��
extern	int		Graphics_Hardware_SetGraphicsDeviceLostCallbackFunction_PF( void (* Callback )( void *Data ), void *CallbackData )
{
#ifndef DX_NON_DIRECT3D11
	Graphics_Hardware_D3D11_SetGraphicsDeviceLostCallbackFunction_PF( Callback, CallbackData ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	Graphics_Hardware_D3D9_SetGraphicsDeviceLostCallbackFunction_PF( Callback, CallbackData ) ;
#endif // DX_NON_DIRECT3D9

	return 0 ;
}

// �ʏ�`��Ƀv���O���}�u���V�F�[�_�[���g�p���邩�ǂ�����ݒ肷��( TRUE:�g�p����( �f�t�H���g )  FALSE:�g�p���Ȃ� )
extern	int		Graphics_Hardware_SetUseNormalDrawShader_PF( int Flag )
{
#ifndef DX_NON_DIRECT3D9
	return Graphics_Hardware_D3D9_SetUseNormalDrawShader_PF( Flag ) ;
#else // DX_NON_DIRECT3D9
	return 0 ;
#endif // DX_NON_DIRECT3D9
}

// �r�f�I�������̗e�ʂ𓾂�
extern	int		Graphics_Hardware_GetVideoMemorySize_PF( int *AllSize, int *FreeSize )
{
#ifndef DX_NON_DIRECT3D9
	return Graphics_Hardware_D3D9_GetVideoMemorySize_PF( AllSize, FreeSize ) ;
#else // DX_NON_DIRECT3D9
	return 0 ;
#endif // DX_NON_DIRECT3D9
}

// Vista�ȍ~�� Windows Aero �𖳌��ɂ��邩�ǂ������Z�b�g����ATRUE:�����ɂ���  FALSE:�L���ɂ���( DxLib_Init �̑O�ɌĂԕK�v������܂� )
extern	int		Graphics_SetAeroDisableFlag_PF( int Flag )
{
	// �������O�̂ݗL��
	if( DxSysData.DxLib_InitializeFlag == TRUE ) return -1 ;

	if( Flag )
	{
		GRAWIN.Setting.DisableAeroFlag = 2 ;
	}
	else
	{
		GRAWIN.Setting.DisableAeroFlag = 1 ;
	}

	// �I��
	return 0 ;
}























// ���ˑ���ʊ֌W

// �ݒ�Ɋ�Â��Ďg�p����y�o�b�t�@���Z�b�g����
extern	int		Graphics_Hardware_SetupUseZBuffer_PF( void )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_Hardware_D3D11_SetupUseZBuffer_PF() ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_Hardware_D3D9_SetupUseZBuffer_PF() ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// ��ʂ̂y�o�b�t�@�̏�Ԃ�����������
extern	int		Graphics_Hardware_ClearDrawScreenZBuffer_PF( const RECT *ClearRect )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_Hardware_D3D11_ClearDrawScreenZBuffer_PF( ClearRect ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_Hardware_D3D9_ClearDrawScreenZBuffer_PF( ClearRect ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// ��ʂ̏�Ԃ�����������
extern	int		Graphics_Hardware_ClearDrawScreen_PF( const RECT *ClearRect )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_Hardware_D3D11_ClearDrawScreen_PF( ClearRect ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_Hardware_D3D9_ClearDrawScreen_PF( ClearRect ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// �`����ʂ̃Z�b�g
extern	int		Graphics_Hardware_SetDrawScreen_PF( int DrawScreen, int OldScreenSurface, IMAGEDATA *NewTargetImage, IMAGEDATA *OldTargetImage, SHADOWMAPDATA *NewTargetShadowMap, SHADOWMAPDATA *OldTargetShadowMap )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_Hardware_D3D11_SetDrawScreen_PF(  DrawScreen,  OldScreenSurface, NewTargetImage, OldTargetImage, NewTargetShadowMap, OldTargetShadowMap ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_Hardware_D3D9_SetDrawScreen_PF(  DrawScreen,  OldScreenSurface, NewTargetImage, OldTargetImage, NewTargetShadowMap, OldTargetShadowMap ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// ����I�� ScreenCopy ���s���R�[���o�b�N�֐�
#define GRAPHICS_SOFTWARE_SCREENFLIPTIMER_ID		(32767)
#if _MSC_VER > 1200 || defined( DX_GCC_COMPILE_4_9_2 )
static VOID CALLBACK Graphics_Software_ScreenFlipTimerProc( HWND /*hwnd*/, UINT /*uMsg*/, UINT_PTR /*idEvent*/, DWORD /*dwTime*/ )
#else
static VOID CALLBACK Graphics_Software_ScreenFlipTimerProc( HWND /*hwnd*/, UINT /*uMsg*/, UINT     /*idEvent*/, DWORD /*dwTime*/ )
#endif
{
	KillTimer( NS_GetMainWindowHandle(), GRAPHICS_SOFTWARE_SCREENFLIPTIMER_ID ) ;

	if( GSYS.DrawSetting.TargetScreen[ 0 ]        == DX_SCREEN_FRONT &&
		GSYS.DrawSetting.TargetScreenSurface[ 0 ] == 0               &&
		WinData.BackBufferTransColorFlag          == FALSE           &&
		WinData.UseUpdateLayerdWindowFlag         == FALSE )
	{
		if( NS_GetActiveFlag() == TRUE )
		{
			NS_ScreenCopy() ;
		}
		SetTimer( NS_GetMainWindowHandle(), GRAPHICS_SOFTWARE_SCREENFLIPTIMER_ID, 32, Graphics_Software_ScreenFlipTimerProc ) ;
	}
}

// SetDrawScreen �̍Ō�ŌĂ΂��֐�
extern	int		Graphics_Hardware_SetDrawScreen_Post_PF( int DrawScreen )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_Hardware_D3D11_SetDrawScreen_Post_PF(  DrawScreen ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_Hardware_D3D9_SetDrawScreen_Post_PF(  DrawScreen ) ;
#endif // DX_NON_DIRECT3D9

	case GRAPHICS_API_NONE :
		if( DrawScreen == DX_SCREEN_FRONT )
		{
			SetTimer( NS_GetMainWindowHandle(), GRAPHICS_SOFTWARE_SCREENFLIPTIMER_ID, 32, Graphics_Software_ScreenFlipTimerProc ) ;
		}
		return 0 ;

	default :
		return 0 ;
	}
}

// �`��\�̈�̃Z�b�g
extern	int		Graphics_Hardware_SetDrawArea_PF( int x1, int y1, int x2, int y2 )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_Hardware_D3D11_SetDrawArea_PF(  x1,  y1,  x2,  y2 ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_Hardware_D3D9_SetDrawArea_PF(  x1,  y1,  x2,  y2 ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// �`���o�b�t�@�����b�N����
extern	int		Graphics_Hardware_LockDrawScreenBuffer_PF( RECT *LockRect, BASEIMAGE *BaseImage, int TargetScreen, IMAGEDATA *TargetImage, int TargetScreenSurface, int ReadOnly, int TargetScreenTextureNo )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_Hardware_D3D11_LockDrawScreenBuffer_PF( LockRect, BaseImage,  TargetScreen, TargetImage, TargetScreenSurface,  ReadOnly,  TargetScreenTextureNo ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_Hardware_D3D9_LockDrawScreenBuffer_PF( LockRect, BaseImage,  TargetScreen, TargetImage, TargetScreenSurface,  ReadOnly,  TargetScreenTextureNo ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// �`���o�b�t�@���A�����b�N����
extern	int		Graphics_Hardware_UnlockDrawScreenBuffer_PF( void )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_Hardware_D3D11_UnlockDrawScreenBuffer_PF() ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_Hardware_D3D9_UnlockDrawScreenBuffer_PF() ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// ����ʂ̓��e��\��ʂɕ`�悷��
extern	int		Graphics_Hardware_ScreenCopy_PF( int DrawTargetFrontScreenMode_Copy )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_Hardware_D3D11_ScreenCopy_PF() ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_Hardware_D3D9_ScreenCopy_PF() ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// �f�B�X�v���C�񋓗p�̃R�[���o�b�N�֐�
BOOL CALLBACK Graphics_MonitorEnumProc(
	HMONITOR hMonitor,    // �f�B�X�v���C���j�^�̃n���h��
	HDC      hdcMonitor,  // ���j�^�ɓK�����f�o�C�X�R���e�L�X�g�̃n���h��
	LPRECT   lprcMonitor, // ���j�^��̌���������\�������`�̈�ւ̃|�C���^
	LPARAM   dwData       // EnumDisplayMonitors ����n���ꂽ�f�[�^
)
{
	MONITORINFOEXW          MonitorInfoEx ;
	GRAPHICSSYS_DISPLAYINFO *DisplayInfo ;
	HDC                     hdc ;

	if( GSYS.Screen.DisplayInfo != NULL )
	{
		DisplayInfo = &GSYS.Screen.DisplayInfo[ GSYS.Screen.DisplayNum ] ;

		_MEMSET( &MonitorInfoEx, 0, sizeof( MonitorInfoEx ) ) ;
		MonitorInfoEx.cbSize = sizeof( MonitorInfoEx ) ;
		GetMonitorInfoW( hMonitor, &MonitorInfoEx ) ;

		DisplayInfo->DesktopRect = MonitorInfoEx.rcMonitor ;
		_WCSCPY_S( DisplayInfo->Name, sizeof( DisplayInfo->Name ), MonitorInfoEx.szDevice ) ;

		hdc = CreateDCW( DisplayInfo->Name, NULL, NULL, NULL ) ;
		DisplayInfo->DesktopSizeX         = GetDeviceCaps( hdc, HORZRES ) ;
		DisplayInfo->DesktopSizeY         = GetDeviceCaps( hdc, VERTRES ) ;
		DisplayInfo->DesktopColorBitDepth = GetDeviceCaps( hdc , PLANES ) * GetDeviceCaps( hdc , BITSPIXEL ) ;
		DisplayInfo->DesktopRefreshRate   = GetDeviceCaps( hdc , VREFRESH ) ;
		DeleteDC( hdc ) ;

		DXST_ERRORLOGFMT_ADDUTF16LE(( "\x4e\x00\x6f\x00\x2e\x00\x25\x00\x64\x00\x20\x00\x20\x00\xe2\x30\xcb\x30\xbf\x30\xfc\x30\x0d\x54\x1a\xff\x25\x00\x73\x00\x00\x30\x25\x00\x64\x00\x78\x00\x25\x00\x64\x00\x00\x30\x25\x00\x64\x00\x62\x00\x69\x00\x74\x00\x00\x30\x25\x00\x64\x00\x48\x00\x7a\x00\x00"/*@ L"No.%d  ���j�^�[���F%s�@%dx%d�@%dbit�@%dHz" @*/, GSYS.Screen.DisplayNum, DisplayInfo->Name, DisplayInfo->DesktopSizeX, DisplayInfo->DesktopSizeY, DisplayInfo->DesktopColorBitDepth, DisplayInfo->DesktopRefreshRate )) ;
	}

	GSYS.Screen.DisplayNum ++ ;

	return TRUE ;
}

// �f�B�X�v���C�̏����Z�b�g�A�b�v����
extern int Graphics_SetupDisplayInfo_PF( void )
{
	DWORD                    i ;
	DWORD                    j ;
	DISPLAY_DEVICEW          DisplayDevice ;
	GRAPHICSSYS_DISPLAYINFO *DisplayInfo ;
	wchar_t *                DeviceName ;
	DWORD                    DisplayDeviceNum ;

	OSVERSIONINFO OsVersionInfo ;

	DXST_ERRORLOGFMT_ADDUTF16LE(( "\xc7\x30\xa3\x30\xb9\x30\xd7\x30\xec\x30\xa4\x30\xc5\x60\x31\x58\x6e\x30\xbb\x30\xc3\x30\xc8\x30\xa2\x30\xc3\x30\xd7\x30\x8b\x95\xcb\x59\x00"/*@ L"�f�B�X�v���C���̃Z�b�g�A�b�v�J�n" @*/ )) ;
	DXST_ERRORLOG_TABADD ;

	_MEMSET( &OsVersionInfo, 0, sizeof( OsVersionInfo ) ) ;
	OsVersionInfo.dwOSVersionInfoSize = sizeof( OsVersionInfo ) ;

	GetVersionEx( &OsVersionInfo ) ;

	if( OsVersionInfo.dwPlatformId >= VER_PLATFORM_WIN32_NT &&
		OsVersionInfo.dwMajorVersion >= 5 )
	{
		_MEMSET( &DisplayDevice, 0, sizeof( DisplayDevice ) ) ;
		DisplayDevice.cb = sizeof( DisplayDevice ) ;

		GSYS.Screen.DisplayNum = 0 ;
		EnumDisplayMonitors( NULL, NULL, Graphics_MonitorEnumProc, NULL ) ;

		// API ��ǂݍ���
		if( WinAPIData.Win32Func.EnumDisplayDevicesWFunc == NULL )
		{
			LoadWinAPI() ;
		}

		DisplayDeviceNum = 0 ;
		while( WinAPIData.Win32Func.EnumDisplayDevicesWFunc( NULL, DisplayDeviceNum, &DisplayDevice, 0 ) != 0 )
		{
			DisplayDeviceNum ++ ;
		}
	}
	else
	{
		GSYS.Screen.DisplayNum = 1 ;
		DisplayDeviceNum       = 1 ;
	}

	DXST_ERRORLOGFMT_ADDUTF16LE(( "\xe2\x30\xcb\x30\xbf\x30\xfc\x30\x6e\x30\x70\x65\x1a\xff\x25\x00\x64\x00\x20\x00\x20\x00\xc7\x30\xa3\x30\xb9\x30\xd7\x30\xec\x30\xa4\x30\xc7\x30\xd0\x30\xa4\x30\xb9\x30\x6e\x30\x70\x65\x1a\xff\x25\x00\x64\x00\x00"/*@ L"���j�^�[�̐��F%d  �f�B�X�v���C�f�o�C�X�̐��F%d" @*/, GSYS.Screen.DisplayNum, DisplayDeviceNum )) ;

	GSYS.Screen.DisplayInfo = ( GRAPHICSSYS_DISPLAYINFO * )HeapAlloc( GetProcessHeap(), 0, sizeof( GRAPHICSSYS_DISPLAYINFO ) * GSYS.Screen.DisplayNum ) ;
	if( GSYS.Screen.DisplayInfo == NULL )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\x5b\x00\xa8\x30\xe9\x30\xfc\x30\x5d\x00\xc7\x30\xa3\x30\xb9\x30\xd7\x30\xec\x30\xa4\x30\xc5\x60\x31\x58\xdd\x4f\x58\x5b\x28\x75\x6e\x30\xe1\x30\xe2\x30\xea\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x00"/*@ L"[�G���[]�f�B�X�v���C���ۑ��p�̃������m�ۂɎ��s���܂���" @*/ )) ;
		DXST_ERRORLOG_TABSUB ;
		return -1 ;
	}
	_MEMSET( GSYS.Screen.DisplayInfo, 0, sizeof( GRAPHICSSYS_DISPLAYINFO ) * GSYS.Screen.DisplayNum ) ;

	if( OsVersionInfo.dwPlatformId >= VER_PLATFORM_WIN32_NT &&
		OsVersionInfo.dwMajorVersion >= 5 )
	{
		GSYS.Screen.DisplayNum = 0 ;
		EnumDisplayMonitors( NULL, NULL, Graphics_MonitorEnumProc, NULL ) ;
	}
	else
	{
		HDC hdc ;

		DisplayInfo = GSYS.Screen.DisplayInfo ;

		hdc = GetDC( NULL ) ;

		DisplayInfo->DesktopSizeX         = GetDeviceCaps( hdc, HORZRES ) ;
		DisplayInfo->DesktopSizeY         = GetDeviceCaps( hdc, VERTRES ) ;
		DisplayInfo->DesktopColorBitDepth = GetDeviceCaps( hdc , PLANES ) * GetDeviceCaps( hdc , BITSPIXEL ) ;
		DisplayInfo->DesktopRefreshRate   = GetDeviceCaps( hdc , VREFRESH ) ;

		ReleaseDC( NULL, hdc ) ;

		DisplayInfo->DesktopRect.left   = 0 ;
		DisplayInfo->DesktopRect.top    = 0 ;
		DisplayInfo->DesktopRect.right  = DisplayInfo->DesktopSizeX ;
		DisplayInfo->DesktopRect.bottom = DisplayInfo->DesktopSizeY ;
		_WCSCPY( DisplayInfo->Name, L"DISPLAY" ) ;

		DXST_ERRORLOGFMT_ADDUTF16LE(( "\x4e\x00\x6f\x00\x2e\x00\x25\x00\x64\x00\x20\x00\x20\x00\xe2\x30\xcb\x30\xbf\x30\xfc\x30\x0d\x54\x1a\xff\x25\x00\x73\x00\x00\x30\x25\x00\x64\x00\x78\x00\x25\x00\x64\x00\x00\x30\x25\x00\x64\x00\x62\x00\x69\x00\x74\x00\x00\x30\x25\x00\x64\x00\x48\x00\x7a\x00\x00"/*@ L"No.%d  ���j�^�[���F%s�@%dx%d�@%dbit�@%dHz" @*/, 0, DisplayInfo->Name, DisplayInfo->DesktopSizeX, DisplayInfo->DesktopSizeY, DisplayInfo->DesktopColorBitDepth, DisplayInfo->DesktopRefreshRate )) ;
	}

	for( j = 0 ; j < DisplayDeviceNum ; j ++ )
	{
		DWORD           ModeNum ;
		DEVMODEW        DevMode ;
		DISPLAYMODEDATA *ModeData ;

		if( OsVersionInfo.dwPlatformId >= VER_PLATFORM_WIN32_NT &&
			OsVersionInfo.dwMajorVersion >= 5 )
		{
			WinAPIData.Win32Func.EnumDisplayDevicesWFunc( NULL, j, &DisplayDevice, 0 ) ;
			DeviceName = DisplayDevice.DeviceName ;

			DisplayInfo = GSYS.Screen.DisplayInfo ;
			for( i = 0 ; i < ( DWORD )GSYS.Screen.DisplayNum ; i ++, DisplayInfo ++ )
			{
				if( _WCSCMP( DisplayInfo->Name, DeviceName ) == 0 )
				{
					break ;
				}
			}
			if( i == ( DWORD )GSYS.Screen.DisplayNum )
			{
				continue ;
			}
		}
		else
		{
			DisplayInfo = GSYS.Screen.DisplayInfo ;
			DeviceName = NULL ;
		}

//		DXST_ERRORLOGFMT_ADDUTF16LE(( "\x4e\x00\x6f\x00\x2e\x00\x25\x00\x64\x00\x20\x00\x20\x00\xc7\x30\xa3\x30\xb9\x30\xd7\x30\xec\x30\xa4\x30\xc7\x30\xd0\x30\xa4\x30\xb9\x30\x0d\x54\x1a\xff\x25\x00\x73\x00\x00"/*@ L"No.%d  �f�B�X�v���C�f�o�C�X���F%s" @*/, j, DeviceName == NULL ? L"None" : DeviceName )) ;

		// ���𐔂���
		DisplayInfo->ModeNum = 0 ;
		ModeNum              = 0 ;
		for( ModeNum ;  ; ModeNum ++ )
		{
			_MEMSET( &DevMode, 0, sizeof( DevMode ) ) ;
			if( EnumDisplaySettingsW( DeviceName, ModeNum, &DevMode ) == 0 )
			{
				break ;
			}

			// 16bit �ȉ��̃��[�h�͏��O����
			if( DevMode.dmBitsPerPel >= 16 )
			{
				DisplayInfo->ModeNum ++ ;
			}
		}

		// �����i�[���郁�����̈�̊m��
		DisplayInfo->ModeData = ( DISPLAYMODEDATA * )HeapAlloc( GetProcessHeap(), 0, sizeof( DISPLAYMODEDATA ) * DisplayInfo->ModeNum ) ;
		if( DisplayInfo->ModeData == NULL )
		{
			DisplayInfo->ModeNum = 0 ;
			return -1 ;
		}

		// �����擾����
		ModeData = DisplayInfo->ModeData ;
		for( i = 0 ; i < ModeNum ; i ++ )
		{
			_MEMSET( &DevMode, 0, sizeof( DevMode ) ) ;
			EnumDisplaySettingsW( DeviceName, i, &DevMode ) ;
			if( DevMode.dmBitsPerPel >= 16 )
			{
				ModeData->Width         = ( int )DevMode.dmPelsWidth ;
				ModeData->Height        = ( int )DevMode.dmPelsHeight ;
				ModeData->ColorBitDepth = ( int )DevMode.dmBitsPerPel ;
				ModeData->RefreshRate   = ( int )DevMode.dmDisplayFrequency ;
//				DXST_ERRORLOGFMT_ADDUTF16LE(( "\x3b\x75\x62\x97\xe2\x30\xfc\x30\xc9\x30\x4e\x00\x6f\x00\x2e\x00\x25\x00\x64\x00\x20\x00\x20\x00\x25\x00\x64\x00\x78\x00\x25\x00\x64\x00\x20\x00\x25\x00\x64\x00\x62\x00\x69\x00\x74\x00\x20\x00\x25\x00\x64\x00\x48\x00\x7a\x00\x00"/*@ L"��ʃ��[�hNo.%d  %dx%d %dbit %dHz" @*/, i, ModeData->Width, ModeData->Height, ModeData->ColorBitDepth, ModeData->RefreshRate )) ;
				ModeData ++ ;
			}
		}
	}

	DXST_ERRORLOG_TABSUB ;
	DXST_ERRORLOGFMT_ADDUTF16LE(( "\xc7\x30\xa3\x30\xb9\x30\xd7\x30\xec\x30\xa4\x30\xc5\x60\x31\x58\x6e\x30\xbb\x30\xc3\x30\xc8\x30\xa2\x30\xc3\x30\xd7\x30\x8c\x5b\x86\x4e\x00"/*@ L"�f�B�X�v���C���̃Z�b�g�A�b�v����" @*/ )) ;

	return 0 ;
}

// ���������M����҂�
extern	int		Graphics_Hardware_WaitVSync_PF( int SyncNum )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_Hardware_D3D11_WaitVSync_PF( SyncNum ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_Hardware_D3D9_WaitVSync_PF( SyncNum ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// ����ʂƕ\��ʂ���������
extern	int		Graphics_ScreenFlipBase_PF( void )
{
	// �n�[�h�E�G�A�@�\���g���Ă��邩�ǂ����ŏ����𕪊�
	if( GSYS.Setting.ValidHardware )
	{
		switch( GRAWIN.Setting.UseGraphicsAPI )
		{
#ifndef DX_NON_DIRECT3D11
		case GRAPHICS_API_DIRECT3D11_WIN32 :
			return Graphics_Hardware_D3D11_ScreenFlipBase_PF() ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
		case GRAPHICS_API_DIRECT3D9_WIN32 :
			return Graphics_Hardware_D3D9_ScreenFlipBase_PF() ;
#endif // DX_NON_DIRECT3D9

		default :
			return 0 ;
		}
	}
	else
	{
		// �\�t�g�E�G�A�����_�����O�̏ꍇ
		RECT        WindRect ;
		char        Buffer[ sizeof( BITMAPINFOHEADER ) + sizeof( DWORD ) * 3 + 4 ] ;
		BITMAPINFO *pBmpInfo ;
		HDC         Dc ;
		int         DrawScreenWidth ;
		int         DrawScreenHeight ;

		// �E�C���h�E���[�h�̏ꍇ�͓]����E�C���h�E�̃f�X�N�g�b�v��̍��W������o��
		if( NS_GetWindowModeFlag() == TRUE )
		{
			int    ClientHeight ;
			double ExRateX ;
			double ExRateY ;

			WindRect.left   = 0 ;
			WindRect.top    = 0 ;

			// ���݂̃E�C���h�E�̃N���C�A���g�̈�̃T�C�Y�𓾂�
			NS_GetWindowSizeExtendRate( &ExRateX, &ExRateY ) ;
			NS_GetDrawScreenSize( &DrawScreenWidth, &DrawScreenHeight ) ;
			WindRect.right  = _DTOL( DrawScreenWidth  * ExRateX ) ;
			WindRect.bottom = _DTOL( DrawScreenHeight * ExRateY ) ;

			if( WinData.ToolBarUseFlag )
			{
				ClientHeight    = WinData.WindowRect.bottom - WinData.WindowRect.top ;
				WindRect.top    = ClientHeight - DrawScreenHeight ;
				WindRect.bottom = ClientHeight ;
			}
		}
		else
		{
			WindRect.left   = 0 ;
			WindRect.top    = 0 ;
			WindRect.right  = GSYS.Screen.MainScreenSizeX ;
			WindRect.bottom = GSYS.Screen.MainScreenSizeY ;
		}

		if( CheckValidMemImg( &GSYS.SoftRender.MainBufferMemImg ) == FALSE ||
			CheckValidMemImg( &GSYS.SoftRender.SubBufferMemImg  ) == FALSE )
		{
			return 0 ;
		}

		// �u�r�x�m�b��҂�	
		if( GSYS.Screen.NotWaitVSyncFlag == FALSE )
		{
			DirectDraw7_LocalWaitVSync() ;
		}

		// �o�b�N�o�b�t�@�̓��ߐF�̕����𓧉߂���t���O���AUpdateLayerdWindow ���g�p����t���O�������Ă���ꍇ�͏����𕪊�
		if( WinData.BackBufferTransColorFlag && WinData.UseUpdateLayerdWindowFlag )
		{
			BASEIMAGE BackBufferImage ;
			RECT      LockRect ;
			int       OldTargetScreen ;
			int       OldTargetScreenSurface ;

			// �u�r�x�m�b��҂�	
			if( GSYS.Screen.NotWaitVSyncFlag == FALSE ) DirectDraw7_LocalWaitVSync() ;

			// Graphics_Screen_LockDrawScreen ���g�����@
			OldTargetScreen                           = GSYS.DrawSetting.TargetScreen[ 0 ] ;
			OldTargetScreenSurface                    = GSYS.DrawSetting.TargetScreenSurface[ 0 ] ;
			GSYS.DrawSetting.TargetScreen[ 0 ]        = DX_SCREEN_BACK ;
			GSYS.DrawSetting.TargetScreenSurface[ 0 ] = 0 ;
			LockRect.left                             = 0 ;
			LockRect.top                              = 0 ;
			LockRect.right                            = GSYS.Screen.MainScreenSizeX ;
			LockRect.bottom                           = GSYS.Screen.MainScreenSizeY ;
			if( Graphics_Screen_LockDrawScreen( &LockRect, &BackBufferImage, -1, -1, TRUE, 0 ) < 0 )
			{
				return -1 ;
			}
			UpdateBackBufferTransColorWindow( &BackBufferImage ) ;
			Graphics_Screen_UnlockDrawScreen() ;
			GSYS.DrawSetting.TargetScreen[ 0 ]        = OldTargetScreen ;
			GSYS.DrawSetting.TargetScreenSurface[ 0 ] = OldTargetScreenSurface ;
		}
		else
		{
			MEMIMG *UseScreenMemImg ;

			// ��ʂɓ]������̈�����肷��ꍇ�͍ŏ��ɃT�u�o�b�t�@�ɓ]������
			if( GSYS.Screen.ValidGraphDisplayArea )
			{
				RECT   SrcRect ;
				RECT   ClipRect ;
				MEMIMG BltSrcMemImg ;

				// �T�u�o�b�t�@�ɓ]������̈��ݒ肷��
				ClipRect.left   = 0 ;
				ClipRect.top    = 0 ;
				ClipRect.right  = GSYS.Screen.MainScreenSizeX ;
				ClipRect.bottom = GSYS.Screen.MainScreenSizeY ;

				SrcRect = GSYS.Screen.GraphDisplayArea ;
				RectClipping_Inline( &SrcRect, &ClipRect ) ;
				if( SrcRect.right  - SrcRect.left <= 0 ||
					SrcRect.bottom - SrcRect.top  <= 0 )
				{
					return -1 ;
				}

				// �����o�������݂̂� MEMIMG ���쐬
				DerivationMemImg(
					&BltSrcMemImg,
					&GSYS.SoftRender.MainBufferMemImg,
					SrcRect.left,
					SrcRect.top,
					SrcRect.right  - SrcRect.left,
					SrcRect.bottom - SrcRect.top
				) ;

				// �T�u�o�b�t�@�S�̂ɓ]��
				DrawEnlargeMemImg(
					&GSYS.SoftRender.SubBufferMemImg,
					&BltSrcMemImg,
					&ClipRect,
					FALSE,
					NULL
				) ;

				UseScreenMemImg = &GSYS.SoftRender.SubBufferMemImg ;
			}
			else
			{
				UseScreenMemImg = &GSYS.SoftRender.MainBufferMemImg ;
			}

			// ���C���E�C���h�E�̂c�b���擾���ē]������
			pBmpInfo = ( BITMAPINFO * )Buffer ;
			_MEMSET( Buffer, 0, sizeof( Buffer ) ) ;
			pBmpInfo->bmiHeader.biSize               = sizeof( pBmpInfo->bmiHeader ) ;
			pBmpInfo->bmiHeader.biWidth              =  GSYS.Screen.MainScreenSizeX ;
			pBmpInfo->bmiHeader.biHeight             = -GSYS.Screen.MainScreenSizeY ;
			pBmpInfo->bmiHeader.biPlanes             = 1 ;
			pBmpInfo->bmiHeader.biBitCount           = UseScreenMemImg->Base->ColorDataP->ColorBitDepth ;
			pBmpInfo->bmiHeader.biCompression        = BI_BITFIELDS ;
			( ( DWORD * )&pBmpInfo->bmiColors )[ 0 ] = UseScreenMemImg->Base->ColorDataP->RedMask ;
			( ( DWORD * )&pBmpInfo->bmiColors )[ 1 ] = UseScreenMemImg->Base->ColorDataP->GreenMask ;
			( ( DWORD * )&pBmpInfo->bmiColors )[ 2 ] = UseScreenMemImg->Base->ColorDataP->BlueMask ;

			// �s�b�`���P�U�o�C�g�̔{���Ȃ̂ŕ␳
			if( UseScreenMemImg->Base->ColorDataP->ColorBitDepth == 16 )
			{
				// 16bit
				pBmpInfo->bmiHeader.biWidth = ( pBmpInfo->bmiHeader.biWidth + 16 / 2 - 1 ) / ( 16 / 2 ) * ( 16 / 2 ) ;
			}
			else
			{
				// 32bit
				pBmpInfo->bmiHeader.biWidth = ( pBmpInfo->bmiHeader.biWidth + 16 / 4 - 1 ) / ( 16 / 4 ) * ( 16 / 4 ) ;
			}

			Dc = GetDC( GRAWIN.Setting.ScreenFlipTargetWindow ? GRAWIN.Setting.ScreenFlipTargetWindow : GetDisplayWindowHandle() ) ;
			if( Dc )
			{
				// �E�C���h�E���[�h�łR�Q�O�~�Q�S�O�G�~�����[�V�������[�h�̏ꍇ�͓]�����Ɠ]����̃p�����[�^�͌Œ�
				if( NS_GetWindowModeFlag() && GSYS.Screen.Emulation320x240Flag )
				{
					StretchDIBits(
						Dc,
						0,
						0,
						640,
						480, 
						0,
						0,
						320,
						240,
						UseScreenMemImg->UseImage, pBmpInfo, DIB_RGB_COLORS, SRCCOPY );
				}
				else
				{
					if( NS_GetWindowModeFlag() == FALSE )
					{
						// �t���X�N���[�����[�h�̏ꍇ

						// ��ʑS�̂������h��Ԃ��w�肪����������s����
						if( GRAWIN.FullScreenBlackFillCounter > 0 )
						{
							GRAWIN.FullScreenBlackFillCounter -- ;

							// �E�C���h�E�̍��W������[�Ɉړ�����
							NS_SetWindowPosition( 0, 0 ) ;

							// ��ʑS�̂����œh��Ԃ�
							{
								HBRUSH OldBrush;

								OldBrush = ( HBRUSH )SelectObject( Dc, GetStockObject( BLACK_BRUSH ) ) ;
								Rectangle(
									Dc,
									0,
									0,
									GSYS.Screen.FullScreenUseDispModeData.Width,
									GSYS.Screen.FullScreenUseDispModeData.Height
								) ;
								SelectObject( Dc, OldBrush) ;
							}
						}

						switch( GSYS.Screen.FullScreenResolutionModeAct )
						{
						case DX_FSRESOLUTIONMODE_NATIVE :
							SetDIBitsToDevice(
								Dc,
								0, 0,
								( DWORD )GSYS.Screen.MainScreenSizeX, ( DWORD )GSYS.Screen.MainScreenSizeY,
								0, 0,
								0, ( UINT )GSYS.Screen.MainScreenSizeY, 
								UseScreenMemImg->UseImage, pBmpInfo, DIB_RGB_COLORS );
							break ;

						case DX_FSRESOLUTIONMODE_MAXIMUM :
						case DX_FSRESOLUTIONMODE_DESKTOP :
							// �]�����`�̃Z�b�g�A�b�v
							Graphics_Screen_SetupFullScreenScalingDestRect() ;

//							DXST_ERRORLOGFMT_ADDUTF16LE(( L"GSYS.Screen.FullScreenScalingDestRect %d %d %d %d",
//								GSYS.Screen.FullScreenScalingDestRect.left,
//								GSYS.Screen.FullScreenScalingDestRect.top,
//								GSYS.Screen.FullScreenScalingDestRect.right,
//								GSYS.Screen.FullScreenScalingDestRect.bottom
//							)) ;

							StretchDIBits(
								Dc,
								GSYS.Screen.FullScreenScalingDestRect.left,
								GSYS.Screen.FullScreenScalingDestRect.top,
								GSYS.Screen.FullScreenScalingDestRect.right  - GSYS.Screen.FullScreenScalingDestRect.left,
								GSYS.Screen.FullScreenScalingDestRect.bottom - GSYS.Screen.FullScreenScalingDestRect.top,
								0,
								0,
								GSYS.Screen.MainScreenSizeX,
								GSYS.Screen.MainScreenSizeY,
								UseScreenMemImg->UseImage, pBmpInfo, DIB_RGB_COLORS, SRCCOPY );
							break ;
						}
					}
					else
					{
						int WindowSizeX ;
						int WindowSizeY ;

						WindowSizeX = WindRect.right  - WindRect.left ;
						WindowSizeY = WindRect.bottom - WindRect.top ;

						// �E�C���h�E���[�h�̏ꍇ
						if( GSYS.Screen.MainScreenSizeX == WindowSizeX && GSYS.Screen.MainScreenSizeY == WindowSizeY )
						{
							SetDIBitsToDevice(
								Dc,
								0, 0,
								( DWORD )WindowSizeX, ( DWORD )WindowSizeY,
								0, 0,
								0, ( UINT )WindowSizeY, 
								UseScreenMemImg->UseImage, pBmpInfo, DIB_RGB_COLORS );
						}
						else
						{
							StretchDIBits(
								Dc,
								0, 0,
								WindowSizeX, WindowSizeY, 
								0, 0,
								GSYS.Screen.MainScreenSizeX, GSYS.Screen.MainScreenSizeY,
								UseScreenMemImg->UseImage, pBmpInfo, DIB_RGB_COLORS, SRCCOPY );
						}
					}
				}

				ReleaseDC( GetDisplayWindowHandle(), Dc );
			}
		}
	}

	return 0 ;
}

// ����ʂ̎w��̗̈���E�C���h�E�̃N���C�A���g�̈�̎w��̗̈�ɓ]������
extern	int		Graphics_BltRectBackScreenToWindow_PF( HWND Window, RECT BackScreenRect, RECT WindowClientRect )
{
	// ��`�`�F�b�N
	if( BackScreenRect.left   == BackScreenRect.right    ||
		BackScreenRect.top    == BackScreenRect.bottom   ||
		WindowClientRect.left == WindowClientRect.right  ||
		WindowClientRect.top  == WindowClientRect.bottom )
	{
		return -1 ;
	}

	// �n�[�h�E�G�A�@�\���g���Ă��邩�ǂ����ŏ����𕪊�
	if( GSYS.Setting.ValidHardware )
	{
		// �g���Ă���ꍇ

		switch( GRAWIN.Setting.UseGraphicsAPI )
		{
#ifndef DX_NON_DIRECT3D11
		case GRAPHICS_API_DIRECT3D11_WIN32 :
			return Graphics_Hardware_D3D11_BltRectBackScreenToWindow_PF(  Window, BackScreenRect, WindowClientRect ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
		case GRAPHICS_API_DIRECT3D9_WIN32 :
			return Graphics_Hardware_D3D9_BltRectBackScreenToWindow_PF(  Window, BackScreenRect, WindowClientRect ) ;
#endif // DX_NON_DIRECT3D9

		default :
			return 0 ;
		}
	}
	else
	{
		// �\�t�g�E�G�A�����_�����O�̏ꍇ
		char Buffer[ sizeof( BITMAPINFOHEADER ) + sizeof( DWORD ) * 3 + 4 ];
		BITMAPINFO *pBmpInfo;
		HDC Dc;

		if( CheckValidMemImg( &GSYS.SoftRender.MainBufferMemImg ) == FALSE ) return 0 ;

		// �u�r�x�m�b��҂�	
		if( GSYS.Screen.NotWaitVSyncFlag == FALSE )
		{
			DirectDraw7_LocalWaitVSync() ;
		}

		// �E�C���h�E�̂c�b���擾���ē]������
		pBmpInfo = (BITMAPINFO *)Buffer ;
		_MEMSET( Buffer, 0, sizeof( Buffer ) ) ;
		pBmpInfo->bmiHeader.biSize        = sizeof( pBmpInfo->bmiHeader ) ;
		pBmpInfo->bmiHeader.biWidth       =  GSYS.Screen.MainScreenSizeX ;
		pBmpInfo->bmiHeader.biHeight      = -GSYS.Screen.MainScreenSizeY ;
		pBmpInfo->bmiHeader.biPlanes      = 1;
		pBmpInfo->bmiHeader.biBitCount    = GSYS.SoftRender.MainBufferMemImg.Base->ColorDataP->ColorBitDepth ;
		pBmpInfo->bmiHeader.biCompression = BI_BITFIELDS ;
		( ( DWORD * )&pBmpInfo->bmiColors )[ 0 ] = GSYS.SoftRender.MainBufferMemImg.Base->ColorDataP->RedMask ;
		( ( DWORD * )&pBmpInfo->bmiColors )[ 1 ] = GSYS.SoftRender.MainBufferMemImg.Base->ColorDataP->GreenMask ;
		( ( DWORD * )&pBmpInfo->bmiColors )[ 2 ] = GSYS.SoftRender.MainBufferMemImg.Base->ColorDataP->BlueMask ;
		Dc = GetDC( Window );
		if( Dc )
		{
			StretchDIBits(
				Dc,
				WindowClientRect.left, WindowClientRect.top,
				WindowClientRect.right - WindowClientRect.left,
				WindowClientRect.bottom - WindowClientRect.top,
				BackScreenRect.left, GSYS.Screen.MainScreenSizeY - BackScreenRect.bottom,
				BackScreenRect.right  - BackScreenRect.left,
				BackScreenRect.bottom - BackScreenRect.top,
				( BYTE * )GSYS.SoftRender.MainBufferMemImg.UseImage, pBmpInfo, DIB_RGB_COLORS, SRCCOPY );
			ReleaseDC( Window, Dc );
		}
	}

	// �I��
	return 0 ;
}

// ScreenFlip �ŉ摜��]�������̃E�C���h�E��ݒ肷��( NULL ���w�肷��Ɛݒ���� )
extern	int		Graphics_SetScreenFlipTargetWindow_PF( HWND TargetWindow )
{
	GRAWIN.Setting.ScreenFlipTargetWindow = TargetWindow ;

	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_Hardware_D3D11_SetScreenFlipTargetWindow_PF( TargetWindow ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_Hardware_D3D9_SetScreenFlipTargetWindow_PF( TargetWindow ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// ���C����ʂ̂y�o�b�t�@�̐ݒ��ύX����
extern	int		Graphics_Hardware_SetZBufferMode_PF( int ZBufferSizeX, int ZBufferSizeY, int ZBufferBitDepth )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_Hardware_D3D11_SetZBufferMode_PF(  ZBufferSizeX,  ZBufferSizeY,  ZBufferBitDepth ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_Hardware_D3D9_SetZBufferMode_PF(  ZBufferSizeX,  ZBufferSizeY,  ZBufferBitDepth ) ;
#endif // DX_NON_DIRECT3D9

	default :
#ifndef DX_NON_DIRECT3D11
		Graphics_Hardware_D3D11_SetZBufferMode_PF(  ZBufferSizeX,  ZBufferSizeY,  ZBufferBitDepth ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
		Graphics_Hardware_D3D9_SetZBufferMode_PF(  ZBufferSizeX,  ZBufferSizeY,  ZBufferBitDepth ) ;
#endif // DX_NON_DIRECT3D9
		return 0 ;
	}
}

// �`���y�o�b�t�@�̃Z�b�g
extern	int		Graphics_Hardware_SetDrawZBuffer_PF( int DrawScreen, IMAGEDATA *Image )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_Hardware_D3D11_SetDrawZBuffer_PF( DrawScreen, Image ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_Hardware_D3D9_SetDrawZBuffer_PF( DrawScreen, Image ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// ���݂̉�ʂ̃��t���b�V�����[�g���擾����
extern	int		Graphics_GetRefreshRate_PF( void )
{
	int RefreshRate = -1 ;

	// ���t���b�V�����[�g�̎擾
//	if( WinData.WindowsVersion >= DX_WINDOWSVERSION_NT31 )
	{
		HDC hdc ;
		
		hdc = GetDC( NS_GetMainWindowHandle() ) ;
		if( hdc )
		{
			RefreshRate = GetDeviceCaps( hdc, VREFRESH ) ;
			ReleaseDC( NS_GetMainWindowHandle(), hdc ) ;

			if( RefreshRate < 20 ) RefreshRate = -1 ;
		}
	}

	return RefreshRate ;
}






















// ���ˑ����擾�֌W

// GetColor �� GetColor2 �Ŏg�p����J���[�f�[�^���擾����
extern	const COLORDATA *	Graphics_Hardware_GetMainColorData_PF( void )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_Hardware_D3D11_GetMainColorData_PF(  ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_Hardware_D3D9_GetMainColorData_PF(  ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// �f�B�X�v���[�̃J���[�f�[�^�|�C���^�𓾂�
extern	const COLORDATA *	Graphics_Hardware_GetDispColorData_PF( void )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_Hardware_D3D11_GetDispColorData_PF(  ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_Hardware_D3D9_GetDispColorData_PF(  ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// �w����W�̐F���擾����
extern	DWORD				Graphics_Hardware_GetPixel_PF( int x, int y )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_Hardware_D3D11_GetPixel_PF(  x,  y ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_Hardware_D3D9_GetPixel_PF(  x,  y ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}


























// ���ˑ��摜�֌W

// YUV�T�[�t�F�X���g���� Theora ����̓��e���O���t�B�b�N�X�n���h���̃e�N�X�`���ɓ]������
extern	int		Graphics_Hardware_UpdateGraphMovie_TheoraYUV_PF( MOVIEGRAPH *Movie, IMAGEDATA *Image )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_Hardware_D3D11_UpdateGraphMovie_TheoraYUV_PF( Movie, Image ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_Hardware_D3D9_UpdateGraphMovie_TheoraYUV_PF( Movie, Image ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// �O���t�B�b�N�������̈�̃��b�N
extern	int		Graphics_Hardware_GraphLock_PF( IMAGEDATA *Image, COLORDATA **ColorDataP, int WriteOnly )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_Hardware_D3D11_GraphLock_PF( Image, ColorDataP, WriteOnly ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_Hardware_D3D9_GraphLock_PF( Image, ColorDataP, WriteOnly ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// �O���t�B�b�N�������̈�̃��b�N����
extern	int		Graphics_Hardware_GraphUnlock_PF( IMAGEDATA *Image )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_Hardware_D3D11_GraphUnlock_PF( Image ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_Hardware_D3D9_GraphUnlock_PF( Image ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// �O���t�B�b�N�̂y�o�b�t�@�̏�Ԃ�ʂ̃O���t�B�b�N�̂y�o�b�t�@�ɃR�s�[����( DestGrHandle �� SrcGrHandle ���y�o�b�t�@�������Ă���`��\�摜�ŁA���A���`�G�C���A�X�摜�ł͂Ȃ����Ƃ����� )
extern	int		Graphics_Hardware_CopyGraphZBufferImage_PF( IMAGEDATA *DestImage, IMAGEDATA *SrcImage )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_Hardware_D3D11_CopyGraphZBufferImage_PF( DestImage, SrcImage ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_Hardware_D3D9_CopyGraphZBufferImage_PF( DestImage, SrcImage ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// �摜�f�[�^�̏�����
extern	int		Graphics_Hardware_InitGraph_PF( void )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_Hardware_D3D11_InitGraph_PF( ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_Hardware_D3D9_InitGraph_PF( ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// �O���t�B�b�N�����̐F�œh��Ԃ�
extern	int		Graphics_Hardware_FillGraph_PF( IMAGEDATA *Image, int Red, int Green, int Blue, int Alpha, int ASyncThread )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_Hardware_D3D11_FillGraph_PF( Image,  Red,  Green,  Blue,  Alpha,  ASyncThread ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_Hardware_D3D9_FillGraph_PF( Image,  Red,  Green,  Blue,  Alpha,  ASyncThread ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// �`��\�摜��o�b�N�o�b�t�@����w��̈�̃O���t�B�b�N���擾����
extern	int		Graphics_Hardware_GetDrawScreenGraphBase_PF( IMAGEDATA *Image, IMAGEDATA *TargetImage, int TargetScreen, int TargetScreenSurface, int TargetScreenWidth, int TargetScreenHeight, int x1, int y1, int x2, int y2, int destX, int destY )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_Hardware_D3D11_GetDrawScreenGraphBase_PF( Image, TargetImage,  TargetScreen,  TargetScreenSurface,  TargetScreenWidth,  TargetScreenHeight,  x1,  y1,  x2,  y2,  destX,  destY ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_Hardware_D3D9_GetDrawScreenGraphBase_PF( Image, TargetImage,  TargetScreen,  TargetScreenSurface,  TargetScreenWidth,  TargetScreenHeight,  x1,  y1,  x2,  y2,  destX,  destY ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// Graphics_Image_BltBmpOrBaseImageToGraph3 �̋@��ˑ������p�֐�
extern int Graphics_Hardware_BltBmpOrBaseImageToGraph3_PF(
	const RECT		*SrcRect,
	      int		DestX,
	      int		DestY,
	      int		GrHandle,
	const BASEIMAGE	*RgbBaseImage,
	const BASEIMAGE	*AlphaBaseImage,
	      int		RedIsAlphaFlag,
	      int		UseTransColorConvAlpha,
	      int		TargetOrig,
	      int		ASyncThread
)
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_Hardware_D3D11_BltBmpOrBaseImageToGraph3_PF(
			SrcRect,
			DestX,
			DestY,
			GrHandle,
			RgbBaseImage,
			AlphaBaseImage,
			RedIsAlphaFlag,
			UseTransColorConvAlpha,
			TargetOrig,
			ASyncThread
		);
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_Hardware_D3D9_BltBmpOrBaseImageToGraph3_PF(
			SrcRect,
			DestX,
			DestY,
			GrHandle,
			RgbBaseImage,
			AlphaBaseImage,
			RedIsAlphaFlag,
			UseTransColorConvAlpha,
			TargetOrig,
			ASyncThread
		);
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// ��{�C���[�W�̃t�H�[�}�b�g�� DX_BASEIMAGE_FORMAT_NORMAL �ɕϊ�����K�v�����邩�ǂ������`�F�b�N����
// ( RequiredRgbBaseImageConvFlag �� RequiredAlphaBaseImageConvFlag �ɓ���l  TRUE:�ϊ�����K�v������  FALSE:�ϊ�����K�v�͖��� )
extern	int		Graphics_CheckRequiredNormalImageConv_BaseImageFormat_PF(
	IMAGEDATA_ORIG *Orig,
	int             RgbBaseImageFormat,
	int            *RequiredRgbBaseImageConvFlag,
	int             AlphaBaseImageFormat,
	int            *RequiredAlphaBaseImageConvFlag
)
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_D3D11_CheckRequiredNormalImageConv_BaseImageFormat_PF(
			Orig,
			RgbBaseImageFormat,
			RequiredRgbBaseImageConvFlag,
			AlphaBaseImageFormat,
			RequiredAlphaBaseImageConvFlag
		) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_D3D9_CheckRequiredNormalImageConv_BaseImageFormat_PF(
			Orig,
			RgbBaseImageFormat,
			RequiredRgbBaseImageConvFlag,
			AlphaBaseImageFormat,
			RequiredAlphaBaseImageConvFlag
		) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// �I���W�i���摜��񒆂̃e�N�X�`�����쐬����( 0:����  -1:���s )
extern int Graphics_Hardware_CreateOrigTexture_PF( IMAGEDATA_ORIG *Orig, int ASyncThread )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_Hardware_D3D11_CreateOrigTexture_PF( Orig, ASyncThread ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_Hardware_D3D9_CreateOrigTexture_PF( Orig, ASyncThread ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// �I���W�i���摜��񒆂̃e�N�X�`�����������
extern int Graphics_Hardware_ReleaseOrigTexture_PF( IMAGEDATA_ORIG *Orig )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_Hardware_D3D11_ReleaseOrigTexture_PF( Orig ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_Hardware_D3D9_ReleaseOrigTexture_PF( Orig ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// �w��̃}���`�T���v�����Ŏg�p�ł���ő�N�I���e�B�l���擾����
extern int Graphics_Hardware_GetMultiSampleQuality_PF( int Samples )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_Hardware_D3D11_GetMultiSampleQuality_PF( Samples ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_Hardware_D3D9_GetMultiSampleQuality_PF( Samples ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// �쐬����O���t�B�b�N�n���h���Ŏg�p������ˑ��̃e�N�X�`���t�H�[�}�b�g���w�肷��
extern int Graphics_Hardware_SetUsePlatformTextureFormat_PF( int PlatformTextureFormat )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_Hardware_D3D11_SetUsePlatformTextureFormat_PF( PlatformTextureFormat ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_Hardware_D3D9_SetUsePlatformTextureFormat_PF( PlatformTextureFormat ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}























// ���ˑ����_�o�b�t�@�E�C���f�b�N�X�o�b�t�@�֌W

// ���_�o�b�t�@�n���h���̒��_�o�b�t�@���쐬����
extern	int		Graphics_Hardware_VertexBuffer_Create_PF( VERTEXBUFFERHANDLEDATA *VertexBuffer )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_Hardware_D3D11_VertexBuffer_Create_PF( VertexBuffer ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_Hardware_D3D9_VertexBuffer_Create_PF( VertexBuffer ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// ���_�o�b�t�@�n���h���̌�n��
extern	int		Graphics_Hardware_VertexBuffer_Terminate_PF( VERTEXBUFFERHANDLEDATA *VertexBuffer )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_Hardware_D3D11_VertexBuffer_Terminate_PF( VertexBuffer ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_Hardware_D3D9_VertexBuffer_Terminate_PF( VertexBuffer ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// ���_�o�b�t�@�ɒ��_�f�[�^��]������
extern	int		Graphics_Hardware_VertexBuffer_SetData_PF( VERTEXBUFFERHANDLEDATA *VertexBuffer, int SetIndex, const void *VertexData, int VertexNum )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_Hardware_D3D11_VertexBuffer_SetData_PF( VertexBuffer, SetIndex, VertexData, VertexNum ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_Hardware_D3D9_VertexBuffer_SetData_PF( VertexBuffer, SetIndex, VertexData, VertexNum ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// �C���f�b�N�X�o�b�t�@�n���h���̃Z�b�g�A�b�v���s��
extern	int		Graphics_Hardware_IndexBuffer_Create_PF( INDEXBUFFERHANDLEDATA *IndexBuffer )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_Hardware_D3D11_IndexBuffer_Create_PF( IndexBuffer ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_Hardware_D3D9_IndexBuffer_Create_PF( IndexBuffer ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// �C���f�b�N�X�o�b�t�@�n���h���̌�n��
extern	int		Graphics_Hardware_IndexBuffer_Terminate_PF( INDEXBUFFERHANDLEDATA *IndexBuffer )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_Hardware_D3D11_IndexBuffer_Terminate_PF( IndexBuffer ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_Hardware_D3D9_IndexBuffer_Terminate_PF( IndexBuffer ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// �C���f�b�N�X�o�b�t�@�ɃC���f�b�N�X�f�[�^��]������
extern	int		Graphics_Hardware_IndexBuffer_SetData_PF( INDEXBUFFERHANDLEDATA *IndexBuffer, int SetIndex, const void *IndexData, int IndexNum )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_Hardware_D3D11_IndexBuffer_SetData_PF( IndexBuffer,  SetIndex, IndexData,  IndexNum ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_Hardware_D3D9_IndexBuffer_SetData_PF( IndexBuffer,  SetIndex, IndexData,  IndexNum ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}





















// ���ˑ����C�g�֌W

// ���C�e�B���O���s�����ǂ�����ݒ肷��
extern	int		Graphics_Hardware_Light_SetUse_PF( int Flag )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_Hardware_D3D11_Light_SetUse_PF( Flag ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_Hardware_D3D9_Light_SetUse_PF( Flag ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// �O���[�o���A���r�G���g���C�g�J���[��ݒ肷��
extern	int		Graphics_Hardware_Light_GlobalAmbient_PF( COLOR_F *Color )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_Hardware_D3D11_Light_GlobalAmbient_PF( Color ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_Hardware_D3D9_Light_GlobalAmbient_PF( Color ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// ���C�g�p�����[�^���Z�b�g
extern	int		Graphics_Hardware_Light_SetState_PF( int LightNumber, LIGHTPARAM *LightParam )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_Hardware_D3D11_Light_SetState_PF(  LightNumber, LightParam ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_Hardware_D3D9_Light_SetState_PF(  LightNumber, LightParam ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// ���C�g�̗L���A������ύX
extern	int		Graphics_Hardware_Light_SetEnable_PF( int LightNumber, int EnableState )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_Hardware_D3D11_Light_SetEnable_PF(  LightNumber,  EnableState ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_Hardware_D3D9_Light_SetEnable_PF(  LightNumber,  EnableState ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

























// ���ˑ��V���h�E�}�b�v�֌W

// �V���h�E�}�b�v�f�[�^�ɕK�v�ȃe�N�X�`�����쐬����
extern	int		Graphics_Hardware_ShadowMap_CreateTexture_PF( SHADOWMAPDATA *ShadowMap, int ASyncThread )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_Hardware_D3D11_ShadowMap_CreateTexture_PF( ShadowMap, ASyncThread ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_Hardware_D3D9_ShadowMap_CreateTexture_PF( ShadowMap, ASyncThread ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// �V���h�E�}�b�v�f�[�^�ɕK�v�ȃe�N�X�`�����������
extern	int		Graphics_Hardware_ShadowMap_ReleaseTexture_PF( SHADOWMAPDATA *ShadowMap )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_Hardware_D3D11_ShadowMap_ReleaseTexture_PF( ShadowMap ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_Hardware_D3D9_ShadowMap_ReleaseTexture_PF( ShadowMap ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// ���_�V�F�[�_�[�ɐݒ肷��V���h�E�}�b�v�̏����X�V����
extern	void	Graphics_Hardware_ShadowMap_RefreshVSParam_PF( void )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		Graphics_Hardware_D3D11_ShadowMap_RefreshVSParam_PF(  ) ;
		break ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		Graphics_Hardware_D3D9_ShadowMap_RefreshVSParam_PF(  ) ;
		break ;
#endif // DX_NON_DIRECT3D9

	default :
		break ;
	}
}

// �s�N�Z���V�F�[�_�[�ɐݒ肷��V���h�E�}�b�v�̏����X�V����
extern	void	Graphics_Hardware_ShadowMap_RefreshPSParam_PF( void )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		Graphics_Hardware_D3D11_ShadowMap_RefreshPSParam_PF(  ) ;
		break ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		Graphics_Hardware_D3D9_ShadowMap_RefreshPSParam_PF(  ) ;
		break ;
#endif // DX_NON_DIRECT3D9

	default :
		break ;
	}
}

// �V���h�E�}�b�v�ւ̕`��̏������s��
extern	int		Graphics_Hardware_ShadowMap_DrawSetup_PF( SHADOWMAPDATA *ShadowMap )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_Hardware_D3D11_ShadowMap_DrawSetup_PF( ShadowMap ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_Hardware_D3D9_ShadowMap_DrawSetup_PF( ShadowMap ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// �V���h�E�}�b�v�ւ̕`����I������
extern	int		Graphics_Hardware_ShadowMap_DrawEnd_PF( SHADOWMAPDATA *ShadowMap )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_Hardware_D3D11_ShadowMap_DrawEnd_PF( ShadowMap ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_Hardware_D3D9_ShadowMap_DrawEnd_PF( ShadowMap ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// �`��Ŏg�p����V���h�E�}�b�v���w�肷��( �X���b�g�͂O���P�����w��\ )
extern int Graphics_Hardware_ShadowMap_SetUse_PF( int SlotIndex, SHADOWMAPDATA *ShadowMap )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_Hardware_D3D11_ShadowMap_SetUse_PF( SlotIndex, ShadowMap ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_Hardware_D3D9_ShadowMap_SetUse_PF( SlotIndex, ShadowMap ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}
























// ���ˑ��V�F�[�_�[�֌W

// �V�F�[�_�[�n���h��������������
extern	int		Graphics_Hardware_Shader_Create_PF( int ShaderHandle, int IsVertexShader, void *Image, int ImageSize, int ImageAfterFree, int ASyncThread )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_Hardware_D3D11_Shader_Create_PF(  ShaderHandle,  IsVertexShader, Image,  ImageSize,  ImageAfterFree,  ASyncThread ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_Hardware_D3D9_Shader_Create_PF(  ShaderHandle,  IsVertexShader, Image,  ImageSize,  ImageAfterFree,  ASyncThread ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// �V�F�[�_�[�n���h���̌�n��
extern	int		Graphics_Hardware_Shader_TerminateHandle_PF( SHADERHANDLEDATA *Shader )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_Hardware_D3D11_Shader_TerminateHandle_PF( Shader ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_Hardware_D3D9_Shader_TerminateHandle_PF( Shader ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// �g�p�ł���V�F�[�_�[�̃o�[�W�������擾����( 0=�g���Ȃ�  200=�V�F�[�_�[���f���Q�D�O���g�p�\  300=�V�F�[�_�[���f���R�D�O���g�p�\ )
extern	int		Graphics_Hardware_Shader_GetValidShaderVersion_PF( void )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_Hardware_D3D11_Shader_GetValidShaderVersion_PF() ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_Hardware_D3D9_Shader_GetValidShaderVersion_PF() ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// �w��̖��O�����萔���g�p����V�F�[�_�[�萔�̔ԍ����擾����
extern	int		Graphics_Hardware_Shader_GetConstIndex_PF( const wchar_t *ConstantName, SHADERHANDLEDATA *Shader )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_Hardware_D3D11_Shader_GetConstIndex_PF( ConstantName, Shader ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_Hardware_D3D9_Shader_GetConstIndex_PF( ConstantName, Shader ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// �w��̖��O�����萔���g�p����V�F�[�_�[�萔�̐����擾����
extern	int		Graphics_Hardware_Shader_GetConstCount_PF( const wchar_t *ConstantName, SHADERHANDLEDATA *Shader )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_Hardware_D3D11_Shader_GetConstCount_PF( ConstantName, Shader ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_Hardware_D3D9_Shader_GetConstCount_PF( ConstantName, Shader ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// �w��̖��O�������������_�萔�̃f�t�H���g�p�����[�^���i�[����Ă��郁�����A�h���X���擾����
extern	const FLOAT4 *Graphics_Hardware_Shader_GetConstDefaultParamF_PF( const wchar_t *ConstantName, SHADERHANDLEDATA *Shader )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_Hardware_D3D11_Shader_GetConstDefaultParamF_PF( ConstantName, Shader ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_Hardware_D3D9_Shader_GetConstDefaultParamF_PF( ConstantName, Shader ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return NULL ;
	}
}

// �V�F�[�_�[�萔����ݒ肷��
extern	int		Graphics_Hardware_Shader_SetConst_PF( int TypeIndex, int SetIndex, int ConstantIndex, const void *Param, int ParamNum, int UpdateUseArea )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_Hardware_D3D11_Shader_SetConst_PF( TypeIndex,  SetIndex,  ConstantIndex, Param,  ParamNum,  UpdateUseArea ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_Hardware_D3D9_Shader_SetConst_PF( TypeIndex,  SetIndex,  ConstantIndex, Param,  ParamNum,  UpdateUseArea ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// �w��̈�̃V�F�[�_�[�萔�������Z�b�g����
extern	int		Graphics_Hardware_Shader_ResetConst_PF( int TypeIndex, int SetIndex, int ConstantIndex, int ParamNum )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_Hardware_D3D11_Shader_ResetConst_PF(  TypeIndex,  SetIndex,  ConstantIndex,  ParamNum ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_Hardware_D3D9_Shader_ResetConst_PF(  TypeIndex,  SetIndex,  ConstantIndex,  ParamNum ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

#ifndef DX_NON_MODEL
// �R�c���f���p�̃V�F�[�_�[�R�[�h�̏��������s��
extern	int		Graphics_Hardware_Shader_ModelCode_Init_PF( void )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_Hardware_D3D11_Shader_ModelCode_Init_PF() ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_Hardware_D3D9_Shader_ModelCode_Init_PF() ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}
#endif // DX_NON_MODEL























// ���ˑ��V�F�[�_�[�p�萔�o�b�t�@�֌W

// �V�F�[�_�[�p�萔�o�b�t�@�n���h��������������
extern int Graphics_Hardware_ShaderConstantBuffer_Create_PF( int ShaderConstantBufferHandle, int BufferSize, int ASyncThread )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_Hardware_D3D11_ShaderConstantBuffer_Create_PF( ShaderConstantBufferHandle, BufferSize, ASyncThread ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_Hardware_D3D9_ShaderConstantBuffer_Create_PF( ShaderConstantBufferHandle, BufferSize, ASyncThread ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// �V�F�[�_�[�p�萔�o�b�t�@�n���h���̌�n��
extern int Graphics_Hardware_ShaderConstantBuffer_TerminateHandle_PF( SHADERCONSTANTBUFFERHANDLEDATA *ShaderConstantBuffer )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_Hardware_D3D11_ShaderConstantBuffer_TerminateHandle_PF( ShaderConstantBuffer ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_Hardware_D3D9_ShaderConstantBuffer_TerminateHandle_PF( ShaderConstantBuffer ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// �V�F�[�_�[�p�萔�o�b�t�@�n���h���̒萔�o�b�t�@�̃A�h���X���擾����
extern void *Graphics_Hardware_ShaderConstantBuffer_GetBuffer_PF( SHADERCONSTANTBUFFERHANDLEDATA *ShaderConstantBuffer )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_Hardware_D3D11_ShaderConstantBuffer_GetBuffer_PF( ShaderConstantBuffer ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_Hardware_D3D9_ShaderConstantBuffer_GetBuffer_PF( ShaderConstantBuffer ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return NULL ;
	}
}

// �V�F�[�_�[�p�萔�o�b�t�@�n���h���̒萔�o�b�t�@�ւ̕ύX��K�p����
extern int Graphics_Hardware_ShaderConstantBuffer_Update_PF( SHADERCONSTANTBUFFERHANDLEDATA *ShaderConstantBuffer )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_Hardware_D3D11_ShaderConstantBuffer_Update_PF( ShaderConstantBuffer ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_Hardware_D3D9_ShaderConstantBuffer_Update_PF( ShaderConstantBuffer ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// �V�F�[�_�[�p�萔�o�b�t�@�n���h���̒萔�o�b�t�@���w��̃V�F�[�_�[�̎w��̃X���b�g�ɃZ�b�g����
extern int Graphics_Hardware_ShaderConstantBuffer_Set_PF( SHADERCONSTANTBUFFERHANDLEDATA *ShaderConstantBuffer, int TargetShader /* DX_SHADERTYPE_VERTEX �Ȃ� */, int Slot )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_Hardware_D3D11_ShaderConstantBuffer_Set_PF( ShaderConstantBuffer, TargetShader, Slot ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_Hardware_D3D9_ShaderConstantBuffer_Set_PF( ShaderConstantBuffer, TargetShader, Slot ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}


























// ���ˑ��`��֌W

// ���_�o�b�t�@�ɗ��܂������_�f�[�^�������_�����O����
extern	int		Graphics_Hardware_RenderVertex( int ASyncThread )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_D3D11_RenderVertex( -1, ASyncThread ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_D3D9_RenderVertex( ASyncThread ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawBillboard3D
extern	int		Graphics_Hardware_DrawBillboard3D_PF( VECTOR Pos, float cx, float cy, float Size, float Angle, IMAGEDATA *Image, IMAGEDATA *BlendImage, int TransFlag, int TurnFlag, int DrawFlag, RECT *DrawArea )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_D3D11_DrawBillboard3D( Pos,  cx,  cy,  Size,  Angle, Image, BlendImage,  TransFlag,  TurnFlag,  DrawFlag, DrawArea ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_D3D9_DrawBillboard3D( Pos,  cx,  cy,  Size,  Angle, Image, BlendImage,  TransFlag,  TurnFlag,  DrawFlag, DrawArea ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawModiBillboard3D
extern	int		Graphics_Hardware_DrawModiBillboard3D_PF( VECTOR Pos, float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, IMAGEDATA *Image, IMAGEDATA *BlendImage, int TransFlag, int DrawFlag, RECT *DrawArea )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_D3D11_DrawModiBillboard3D(  Pos,  x1,  y1,  x2,  y2,  x3,  y3,  x4,  y4, Image, BlendImage,  TransFlag,  DrawFlag, DrawArea ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_D3D9_DrawModiBillboard3D(  Pos,  x1,  y1,  x2,  y2,  x3,  y3,  x4,  y4, Image, BlendImage,  TransFlag,  DrawFlag, DrawArea ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawGraph
extern	int		Graphics_Hardware_DrawGraph_PF( int x, int y, float xf, float yf, IMAGEDATA *Image, IMAGEDATA *BlendImage, int TransFlag, int IntFlag )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_D3D11_DrawGraph( x, y, xf, yf, Image, BlendImage, TransFlag, IntFlag ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_D3D9_DrawGraph( x, y, xf, yf, Image, BlendImage, TransFlag, IntFlag ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawExtendGraph
extern	int		Graphics_Hardware_DrawExtendGraph_PF( int x1, int y1, int x2, int y2, float x1f, float y1f, float x2f, float y2f, IMAGEDATA *Image, IMAGEDATA *BlendImage, int TransFlag, int IntFlag )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_D3D11_DrawExtendGraph(  x1,  y1,  x2,  y2,  x1f,  y1f,  x2f,  y2f, Image, BlendImage,  TransFlag,  IntFlag ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_D3D9_DrawExtendGraph(  x1,  y1,  x2,  y2,  x1f,  y1f,  x2f,  y2f, Image, BlendImage,  TransFlag,  IntFlag ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawRotaGraph
extern	int		Graphics_Hardware_DrawRotaGraph_PF( int x, int y, float xf, float yf, double ExRate, double Angle, IMAGEDATA *Image, IMAGEDATA *BlendImage, int TransFlag, int TurnFlag, int IntFlag )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_D3D11_DrawRotaGraph(  x,  y,  xf,  yf,  ExRate,  Angle, Image, BlendImage,  TransFlag,  TurnFlag,  IntFlag ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_D3D9_DrawRotaGraph(  x,  y,  xf,  yf,  ExRate,  Angle, Image, BlendImage,  TransFlag,  TurnFlag,  IntFlag ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawRotaGraphFast
extern	int		Graphics_Hardware_DrawRotaGraphFast_PF( int x, int y, float xf, float yf, float ExRate, float Angle, IMAGEDATA *Image, IMAGEDATA *BlendImage, int TransFlag, int TurnFlag, int IntFlag )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_D3D11_DrawRotaGraphFast(  x,  y,  xf,  yf,  ExRate,  Angle, Image, BlendImage,  TransFlag,  TurnFlag,  IntFlag ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_D3D9_DrawRotaGraphFast(  x,  y,  xf,  yf,  ExRate,  Angle, Image, BlendImage,  TransFlag,  TurnFlag,  IntFlag ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawModiGraph
extern	int		Graphics_Hardware_DrawModiGraph_PF( int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, IMAGEDATA *Image, IMAGEDATA *BlendImage, int TransFlag, bool SimpleDrawFlag )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_D3D11_DrawModiGraph(  x1,  y1,  x2,  y2,  x3,  y3,  x4,  y4, Image, BlendImage,  TransFlag, SimpleDrawFlag ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_D3D9_DrawModiGraph(  x1,  y1,  x2,  y2,  x3,  y3,  x4,  y4, Image, BlendImage,  TransFlag, SimpleDrawFlag ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawModiGraphF
extern	int		Graphics_Hardware_DrawModiGraphF_PF( float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, IMAGEDATA *Image, IMAGEDATA *BlendImage, int TransFlag, bool SimpleDrawFlag )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_D3D11_DrawModiGraphF(  x1,  y1,  x2,  y2,  x3,  y3,  x4,  y4, Image, BlendImage,  TransFlag,  SimpleDrawFlag ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_D3D9_DrawModiGraphF(  x1,  y1,  x2,  y2,  x3,  y3,  x4,  y4, Image, BlendImage,  TransFlag,  SimpleDrawFlag ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawSimpleQuadrangleGraphF
extern	int		Graphics_Hardware_DrawSimpleQuadrangleGraphF_PF( const GRAPHICS_DRAW_DRAWSIMPLEQUADRANGLEGRAPHF_PARAM *Param, IMAGEDATA *Image, IMAGEDATA *BlendImage )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_D3D11_DrawSimpleQuadrangleGraphF( Param, Image, BlendImage ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_D3D9_DrawSimpleQuadrangleGraphF( Param, Image, BlendImage ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawSimpleTriangleGraphF
extern	int		Graphics_Hardware_DrawSimpleTriangleGraphF_PF( const GRAPHICS_DRAW_DRAWSIMPLETRIANGLEGRAPHF_PARAM *Param, IMAGEDATA *Image, IMAGEDATA *BlendImage )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_D3D11_DrawSimpleTriangleGraphF( Param, Image, BlendImage ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_D3D9_DrawSimpleTriangleGraphF( Param, Image, BlendImage ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawFillBox
extern	int		Graphics_Hardware_DrawFillBox_PF( int x1, int y1, int x2, int y2, unsigned int Color )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_D3D11_DrawFillBox(  x1,  y1,  x2,  y2,  Color ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_D3D9_DrawFillBox(  x1,  y1,  x2,  y2,  Color ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawLineBox
extern	int		Graphics_Hardware_DrawLineBox_PF( int x1, int y1, int x2, int y2, unsigned int Color )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_D3D11_DrawLineBox(  x1,  y1,  x2,  y2,  Color ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_D3D9_DrawLineBox(  x1,  y1,  x2,  y2,  Color ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}


// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawLine
extern	int		Graphics_Hardware_DrawLine_PF( int x1, int y1, int x2, int y2, unsigned int Color )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_D3D11_DrawLine(  x1,  y1,  x2,  y2,  Color ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_D3D9_DrawLine(  x1,  y1,  x2,  y2,  Color ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawLine3D
extern	int		Graphics_Hardware_DrawLine3D_PF( VECTOR Pos1, VECTOR Pos2, unsigned int Color, int DrawFlag, RECT *DrawArea )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_D3D11_DrawLine3D(  Pos1,  Pos2,  Color,  DrawFlag, DrawArea ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_D3D9_DrawLine3D(  Pos1,  Pos2,  Color,  DrawFlag, DrawArea ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawCircle( �����w�肠�� )
extern	int		Graphics_Hardware_DrawCircle_Thickness_PF( int x, int y, int r, unsigned int Color, int Thickness )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_D3D11_DrawCircle_Thickness(  x,  y,  r,  Color,  Thickness ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_D3D9_DrawCircle_Thickness(  x,  y,  r,  Color,  Thickness ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawOval( �����w�肠�� )
extern	int		Graphics_Hardware_DrawOval_Thickness_PF( int x, int y, int rx, int ry, unsigned int Color, int Thickness )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_D3D11_DrawOval_Thickness(  x,  y,  rx,  ry,  Color,  Thickness ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_D3D9_DrawOval_Thickness(  x,  y,  rx,  ry,  Color,  Thickness ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawCircle
extern	int		Graphics_Hardware_DrawCircle_PF( int x, int y, int r, unsigned int Color, int FillFlag )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_D3D11_DrawCircle(  x,  y,  r,  Color,  FillFlag ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_D3D9_DrawCircle(  x,  y,  r,  Color,  FillFlag ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawOval
extern	int		Graphics_Hardware_DrawOval_PF( int x, int y, int rx, int ry, unsigned int Color, int FillFlag )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_D3D11_DrawOval(  x,  y,  rx,  ry,  Color,  FillFlag ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_D3D9_DrawOval(  x,  y,  rx,  ry,  Color,  FillFlag ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawTriangle
extern	int		Graphics_Hardware_DrawTriangle_PF( int x1, int y1, int x2, int y2, int x3, int y3, unsigned int Color, int FillFlag )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_D3D11_DrawTriangle(  x1,  y1,  x2,  y2,  x3,  y3,  Color,  FillFlag ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_D3D9_DrawTriangle(  x1,  y1,  x2,  y2,  x3,  y3,  Color,  FillFlag ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawTriangle3D
extern	int		Graphics_Hardware_DrawTriangle3D_PF( VECTOR Pos1, VECTOR Pos2, VECTOR Pos3, unsigned int Color, int FillFlag, int DrawFlag, RECT *DrawArea )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_D3D11_DrawTriangle3D(  Pos1,  Pos2,  Pos3,  Color,  FillFlag,  DrawFlag, DrawArea ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_D3D9_DrawTriangle3D(  Pos1,  Pos2,  Pos3,  Color,  FillFlag,  DrawFlag, DrawArea ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawQuadrangle
extern	int		Graphics_Hardware_DrawQuadrangle_PF( int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, unsigned int Color, int FillFlag )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_D3D11_DrawQuadrangle(  x1,  y1,  x2,  y2,  x3,  y3,  x4,  y4,  Color,  FillFlag ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_D3D9_DrawQuadrangle(  x1,  y1,  x2,  y2,  x3,  y3,  x4,  y4,  Color,  FillFlag ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawPixel
extern	int		Graphics_Hardware_DrawPixel_PF( int x, int y, unsigned int Color )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_D3D11_DrawPixel(  x,  y,  Color ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_D3D9_DrawPixel(  x,  y,  Color ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawPixel3D
extern	int		Graphics_Hardware_DrawPixel3D_PF( VECTOR Pos, unsigned int Color, int DrawFlag, RECT *DrawArea )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_D3D11_DrawPixel3D(  Pos,  Color,  DrawFlag, DrawArea ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_D3D9_DrawPixel3D(  Pos,  Color,  DrawFlag, DrawArea ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawPixelSet
extern	int		Graphics_Hardware_DrawPixelSet_PF( const POINTDATA *PointData, int Num )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_D3D11_DrawPixelSet( PointData,  Num ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_D3D9_DrawPixelSet( PointData,  Num ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawLineSet
extern	int		Graphics_Hardware_DrawLineSet_PF( const LINEDATA *LineData, int Num )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_D3D11_DrawLineSet( LineData,  Num ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_D3D9_DrawLineSet( LineData,  Num ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}


extern	int		Graphics_Hardware_DrawPrimitive_PF(                             const VERTEX_3D *Vertex, int VertexNum,                                    int PrimitiveType, IMAGEDATA *Image, int TransFlag )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_D3D11_DrawPrimitive( Vertex, VertexNum, PrimitiveType, Image,  TransFlag ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_D3D9_DrawPrimitive( Vertex, VertexNum, PrimitiveType, Image,  TransFlag ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

extern	int		Graphics_Hardware_DrawIndexedPrimitive_PF(                      const VERTEX_3D *Vertex, int VertexNum, const WORD *Indices, int IndexNum, int PrimitiveType, IMAGEDATA *Image, int TransFlag )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_D3D11_DrawIndexedPrimitive( Vertex,  VertexNum, Indices, IndexNum,  PrimitiveType, Image,  TransFlag ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_D3D9_DrawIndexedPrimitive( Vertex,  VertexNum, Indices, IndexNum,  PrimitiveType, Image,  TransFlag ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

extern	int		Graphics_Hardware_DrawPrimitiveLight_PF(                        const VERTEX3D  *Vertex, int VertexNum,                                    int PrimitiveType, IMAGEDATA *Image, int TransFlag )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_D3D11_DrawPrimitiveLight(                        Vertex,  VertexNum,                                     PrimitiveType, Image,  TransFlag ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_D3D9_DrawPrimitiveLight(                        Vertex,  VertexNum,                                     PrimitiveType, Image,  TransFlag ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

extern	int		Graphics_Hardware_DrawIndexedPrimitiveLight_PF(                 const VERTEX3D  *Vertex, int VertexNum, const WORD *Indices, int IndexNum, int PrimitiveType, IMAGEDATA *Image, int TransFlag )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_D3D11_DrawIndexedPrimitiveLight(                 Vertex, VertexNum, Indices,  IndexNum,  PrimitiveType, Image,  TransFlag ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_D3D9_DrawIndexedPrimitiveLight(                 Vertex, VertexNum, Indices,  IndexNum,  PrimitiveType, Image,  TransFlag ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

extern	int		Graphics_Hardware_DrawPrimitiveLight_UseVertexBuffer_PF(        VERTEXBUFFERHANDLEDATA *VertexBuffer,                                      int PrimitiveType,                 int StartVertex, int UseVertexNum, IMAGEDATA *Image, int TransFlag )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_D3D11_DrawPrimitiveLight_UseVertexBuffer(        VertexBuffer,                                       PrimitiveType,                  StartVertex,  UseVertexNum, Image,  TransFlag ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_D3D9_DrawPrimitiveLight_UseVertexBuffer(        VertexBuffer,                                       PrimitiveType,                  StartVertex,  UseVertexNum, Image,  TransFlag ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

extern	int		Graphics_Hardware_DrawIndexedPrimitiveLight_UseVertexBuffer_PF( VERTEXBUFFERHANDLEDATA *VertexBuffer, INDEXBUFFERHANDLEDATA *IndexBuffer,  int PrimitiveType, int BaseVertex, int StartVertex, int UseVertexNum, int StartIndex, int UseIndexNum, IMAGEDATA *Image, int TransFlag )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_D3D11_DrawIndexedPrimitiveLight_UseVertexBuffer( VertexBuffer, IndexBuffer,   PrimitiveType,  BaseVertex,  StartVertex,  UseVertexNum,  StartIndex,  UseIndexNum, Image,  TransFlag ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_D3D9_DrawIndexedPrimitiveLight_UseVertexBuffer( VertexBuffer, IndexBuffer,   PrimitiveType,  BaseVertex,  StartVertex,  UseVertexNum,  StartIndex,  UseIndexNum, Image,  TransFlag ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

extern	int		Graphics_Hardware_DrawPrimitive2D_PF(                                 VERTEX_2D *Vertex, int VertexNum,                                    int PrimitiveType, IMAGEDATA *Image, int TransFlag, int BillboardFlag, int Is3D, int TurnFlag, int TextureNo, int IsShadowMap )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_D3D11_DrawPrimitive2D(                                 Vertex,  VertexNum,                                     PrimitiveType, Image,  TransFlag,  BillboardFlag,  Is3D,  TurnFlag,  TextureNo, IsShadowMap ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_D3D9_DrawPrimitive2D(                                 Vertex,  VertexNum,                                     PrimitiveType, Image,  TransFlag,  BillboardFlag,  Is3D,  TurnFlag,  TextureNo, IsShadowMap ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

extern	int		Graphics_Hardware_DrawPrimitive2DUser_PF(                       const VERTEX2D  *Vertex, int VertexNum,                                    int PrimitiveType, IMAGEDATA *Image, int TransFlag, int Is3D, int TurnFlag, int TextureNo )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_D3D11_DrawPrimitive2DUser(                       Vertex,  VertexNum,                                     PrimitiveType, Image,  TransFlag,  Is3D,  TurnFlag,  TextureNo ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_D3D9_DrawPrimitive2DUser(                       Vertex,  VertexNum,                                     PrimitiveType, Image,  TransFlag,  Is3D,  TurnFlag,  TextureNo ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

extern	int		Graphics_Hardware_DrawIndexedPrimitive2DUser_PF(                const VERTEX2D  *Vertex, int VertexNum, const WORD *Indices, int IndexNum, int PrimitiveType, IMAGEDATA *Image, int TransFlag )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_D3D11_DrawIndexedPrimitive2DUser(                Vertex,  VertexNum, Indices,  IndexNum,  PrimitiveType, Image,  TransFlag ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_D3D9_DrawIndexedPrimitive2DUser(                Vertex,  VertexNum, Indices,  IndexNum,  PrimitiveType, Image,  TransFlag ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// �V�F�[�_�[���g���ĂQ�c�v���~�e�B�u��`�悷��
extern	int		Graphics_Hardware_DrawPrimitive2DToShader_PF(        const VERTEX2DSHADER *Vertex, int VertexNum,                                              int PrimitiveType /* DX_PRIMTYPE_TRIANGLELIST �� */ )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_D3D11_DrawPrimitive2DToShader(        Vertex,  VertexNum,                                               PrimitiveType /* DX_PRIMTYPE_TRIANGLELIST �� */ ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_D3D9_DrawPrimitive2DToShader(        Vertex,  VertexNum,                                               PrimitiveType /* DX_PRIMTYPE_TRIANGLELIST �� */ ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// �V�F�[�_�[���g���ĂR�c�v���~�e�B�u��`�悷��
extern	int		Graphics_Hardware_DrawPrimitive3DToShader_PF(        const VERTEX3DSHADER *Vertex, int VertexNum,                                              int PrimitiveType /* DX_PRIMTYPE_TRIANGLELIST �� */ )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_D3D11_DrawPrimitive3DToShader(        Vertex,  VertexNum,                                               PrimitiveType /* DX_PRIMTYPE_TRIANGLELIST �� */ ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_D3D9_DrawPrimitive3DToShader(        Vertex,  VertexNum,                                               PrimitiveType /* DX_PRIMTYPE_TRIANGLELIST �� */ ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// �V�F�[�_�[���g���ĂQ�c�v���~�e�B�u��`�悷��( ���_�C���f�b�N�X���g�p���� )
extern	int		Graphics_Hardware_DrawPrimitiveIndexed2DToShader_PF( const VERTEX2DSHADER *Vertex, int VertexNum, const unsigned short *Indices, int IndexNum, int PrimitiveType /* DX_PRIMTYPE_TRIANGLELIST �� */ )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_D3D11_DrawPrimitiveIndexed2DToShader( Vertex,  VertexNum, Indices,  IndexNum,  PrimitiveType /* DX_PRIMTYPE_TRIANGLELIST �� */ ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_D3D9_DrawPrimitiveIndexed2DToShader( Vertex,  VertexNum, Indices,  IndexNum,  PrimitiveType /* DX_PRIMTYPE_TRIANGLELIST �� */ ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// �V�F�[�_�[���g���ĂR�c�v���~�e�B�u��`�悷��( ���_�C���f�b�N�X���g�p���� )
extern	int		Graphics_Hardware_DrawPrimitiveIndexed3DToShader_PF( const VERTEX3DSHADER *Vertex, int VertexNum, const unsigned short *Indices, int IndexNum, int PrimitiveType /* DX_PRIMTYPE_TRIANGLELIST �� */ )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_D3D11_DrawPrimitiveIndexed3DToShader( Vertex,  VertexNum, Indices,  IndexNum,  PrimitiveType /* DX_PRIMTYPE_TRIANGLELIST �� */ ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_D3D9_DrawPrimitiveIndexed3DToShader( Vertex,  VertexNum, Indices,  IndexNum,  PrimitiveType /* DX_PRIMTYPE_TRIANGLELIST �� */ ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// �V�F�[�_�[���g���ĂR�c�v���~�e�B�u��`�悷��( ���_�o�b�t�@�g�p�� )
extern	int		Graphics_Hardware_DrawPrimitive3DToShader_UseVertexBuffer2_PF(        int VertexBufHandle,                     int PrimitiveType /* DX_PRIMTYPE_TRIANGLELIST �� */, int StartVertex, int UseVertexNum )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_D3D11_DrawPrimitive3DToShader_UseVertexBuffer2(         VertexBufHandle,                      PrimitiveType /* DX_PRIMTYPE_TRIANGLELIST �� */,  StartVertex,  UseVertexNum ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_D3D9_DrawPrimitive3DToShader_UseVertexBuffer2(         VertexBufHandle,                      PrimitiveType /* DX_PRIMTYPE_TRIANGLELIST �� */,  StartVertex,  UseVertexNum ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// �V�F�[�_�[���g���ĂR�c�v���~�e�B�u��`�悷��( ���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@�g�p�� )
extern	int		Graphics_Hardware_DrawPrimitiveIndexed3DToShader_UseVertexBuffer2_PF( int VertexBufHandle, int IndexBufHandle, int PrimitiveType /* DX_PRIMTYPE_TRIANGLELIST �� */, int BaseVertex, int StartVertex, int UseVertexNum, int StartIndex, int UseIndexNum )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_D3D11_DrawPrimitiveIndexed3DToShader_UseVertexBuffer2(  VertexBufHandle,  IndexBufHandle,  PrimitiveType /* DX_PRIMTYPE_TRIANGLELIST �� */,  BaseVertex,  StartVertex,  UseVertexNum,  StartIndex,  UseIndexNum ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_D3D9_DrawPrimitiveIndexed3DToShader_UseVertexBuffer2(  VertexBufHandle,  IndexBufHandle,  PrimitiveType /* DX_PRIMTYPE_TRIANGLELIST �� */,  BaseVertex,  StartVertex,  UseVertexNum,  StartIndex,  UseIndexNum ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// �w��_���狫�E�F������Ƃ���܂œh��Ԃ�
extern	int		Graphics_Hardware_Paint_PF( int x, int y, unsigned int FillColor, ULONGLONG BoundaryColor )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Graphics_Hardware_D3D11_Paint_PF(  x,  y,  FillColor,  BoundaryColor ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Graphics_Hardware_D3D9_Paint_PF(  x,  y,  FillColor,  BoundaryColor ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}






























#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE


#endif // DX_NON_GRAPHICS
