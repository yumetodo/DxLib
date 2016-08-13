// -------------------------------------------------------------------------------
// 
// 		�c�w���C�u����		�E�C���h�E�֌W����v���O����
// 
// 				Ver 3.16d
// 
// -------------------------------------------------------------------------------

// �c�w���C�u�����쐬���p��`
#define __DX_MAKE

// �C���N���[�h ------------------------------------------------------------------
#include "DxWindow.h"
#include "DxGraphicsD3D9.h"
#include "DxGraphicsWin.h"
//#include <winsock2.h>
//#include <ws2tcpip.h>
#include <windows.h>
#include <stdio.h>
#include <Winuser.h>
#include <commctrl.h>
#include <float.h>
#ifndef DX_NON_KEYEX
//#include <imm.h>
#endif
#include "../DxChar.h"
#include "../DxLog.h"
#include "../DxArchive_.h"
#include "../DxUseCLib.h"
#include "../DxGraphics.h"
#include "../DxModel.h"
#include "../DxFile.h"
#include "../DxSound.h"
#include "../DxInput.h"
#include "../DxMovie.h"
#include "../DxMask.h"
#include "../DxFont.h"
#include "../DxSystem.h"
#include "../DxInputString.h"
#include "../DxNetwork.h"
#include "../DxBaseImage.h"
#include "../DxASyncLoad.h"
#include "DxWindow.h"
#include "DxWinAPI.h"
#include "DxGuid.h"

#ifdef DX_NON_INLINE_ASM
  #include <math.h>
#endif

#ifdef DX_NON_INLINE_ASM
#ifdef _WIN64
  #include <intrin.h>
#endif
#endif


/*
#ifdef DX_USE_VISUAL_C_MEMDUMP
	#include <crtdbg.h>
#endif
*/
#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// �}�N����` -------------------------------------
#ifdef __BCC
	#ifdef sprintf
	#undef sprintf
	#endif
	
	#ifdef vsprintf
	#undef vsprintf
	#endif
#endif

//���C���E�C���h�E�̃N���X��
#define DXCLASSNAME		L"D123987X"

//���C���E�C���h�E�̃f�t�H���g�^�C�g��
#ifndef DX_NON_LITERAL_STRING
	#define WIN_DEFAULT_TITLE	L"DxLib"
#else
	#define WIN_DEFAULT_TITLE	L"Soft"
#endif

#ifdef UNICODE
	#define CLIPBOARD_TEXT CF_UNICODETEXT
#else
	#define CLIPBOARD_TEXT CF_TEXT
#endif





#ifndef DX_NON_INLINE_ASM

// RDTSC �j�[���j�b�N�� cpuid �j�[�j�b�N�̒�`
#ifdef __BCC
	#define RDTSC __emit__(0fh) ;	__emit__(031h) ;
	#define cpuid __emit__(0xf) ; __emit__(0xa2) ;
#else
	#define cpuid __asm __emit 0fh __asm __emit 0a2h
	#define RDTSC __asm __emit 0fh __asm __emit 031h
#endif

#endif // DX_NON_INLINE_ASM

#ifndef WH_KEYBOARD_LL
#define WH_KEYBOARD_LL    			(13)
#endif
#ifndef LLKHF_ALTDOWN
#define LLKHF_ALTDOWN    			(0x00000020)
#endif
#ifndef LLKHF_UP
#define LLKHF_UP           			(0x00000080)
#endif
#ifndef WM_MOUSEWHEEL
#define WM_MOUSEWHEEL 				(0x20A)				// �}�E�X�̃z�C�[�����상�b�Z�[�W�̒�`
#endif
#ifndef WM_MOUSEHWHEEL
#define WM_MOUSEHWHEEL				(0x20E)
#endif
#ifndef WM_TOUCH
#define WM_TOUCH					(0x240)
#endif
#ifndef WM_IME_REQUEST
#define WM_IME_REQUEST				(0x0288)
#endif
#ifndef WHEEL_DELTA
#define WHEEL_DELTA					(120)
#endif
#ifndef TOUCHEVENTF_MOVE
#define TOUCHEVENTF_MOVE			(0x0001)
#define TOUCHEVENTF_DOWN			(0x0002)
#define TOUCHEVENTF_UP				(0x0004)
#endif


//#define CHECKMULTIBYTECHAR(CP)		(( (unsigned char)*(CP) >= 0x81 && (unsigned char)*(CP) <= 0x9F ) || ( (unsigned char)*(CP) >= 0xE0 && (unsigned char)*(CP) <= 0xFC ))	// TRUE:�Q�o�C�g����  FALSE:�P�o�C�g����

// �c�[���o�[�p��`
#define TOOLBAR_COMMANDID_BASE		(0x500)


// �E�C���h�E�X�^�C����`
//#define WSTYLE_WINDOWMODE			(WS_MINIMIZEBOX | WS_POPUP | WS_SYSMENU | WS_CAPTION | WS_VISIBLE)	// �ʏ�
//#define WSTYLE_WINDOWMODE			(WS_POPUP | WS_SYSMENU | WS_VISIBLE)								// �^�X�N�o�[�Ȃ�
//#define WEXSTYLE_WINDOWMODE		(WS_EX_CLIENTEDGE | WS_EX_WINDOWEDGE)
//#define WSTYLE_FULLSCREENMODE		(WS_POPUP)
//#define WEXSTYLE_FULLSCREENMODE	(WS_EX_TOPMOST)

// �E�C���h�E�X�^�C���̒�`
#define WSTYLE_NUM					(12)		// �E�C���h�E�X�^�C���̐�

#ifndef DX_GCC_COMPILE
// ���C���[�h�E�C���h�E�p�̒�`
#define WS_EX_LAYERED				0x00080000
#endif

#ifndef LWA_COLORKEY
#define LWA_COLORKEY				0x01
#endif
#ifndef LWA_ALPHA
#define LWA_ALPHA					0x02
#endif

#ifndef ULW_COLORKEY
#define ULW_COLORKEY				0x01
#endif
#ifndef ULW_ALPHA
#define ULW_ALPHA					0x02
#endif
#ifndef ULW_OPAQUE
#define ULW_OPAQUE					0x04
#endif

#define AC_SRC_OVER                 0x00
#define AC_SRC_ALPHA                0x01


#ifndef VER_MINORVERSION
#define VER_MINORVERSION			0x0000001
#endif

#ifndef VER_MAJORVERSION
#define VER_MAJORVERSION			0x0000002
#endif

#ifndef VER_GREATER
#define VER_GREATER					2
#endif

#ifndef VER_GREATER_EQUAL
#define VER_GREATER_EQUAL			3
#endif


struct __MEMORYSTATUSEX
{
    DWORD dwLength;
    DWORD dwMemoryLoad;
    DWORDLONG ullTotalPhys;
    DWORDLONG ullAvailPhys;
    DWORDLONG ullTotalPageFile;
    DWORDLONG ullAvailPageFile;
    DWORDLONG ullTotalVirtual;
    DWORDLONG ullAvailVirtual;
    DWORDLONG ullAvailExtendedVirtual;
} ;

typedef WINBASEAPI BOOL ( WINAPI * GLOBALMEMORYSTATUSEX_FUNC )( __MEMORYSTATUSEX * ) ;


//  0 :�f�t�H���g
//  1 :�^�X�N�o�[�Ȃ�
//  2 :�^�X�N�o�[�Ȃ��g�Ȃ�
//  3 :�g�Ȃ�
//  4 :�����Ȃ�
//  5 :�ŏ�������
//  6 :�c�[���o�[�ɓK�����E�C���h�E
//  7 :�ő剻�{�^�����E�C���h�E�A������Ԃ��ʏ�T�C�Y
//  8 :�ő剻�{�^�����E�C���h�E�A������Ԃ��ő剻���
//  9 :�f�t�H���g�ɉ��̗��̉�����
// 10 :�ő剻�{�^�����E�C���h�E�A���̗��̉�����
// 11 :���閳���A�ŏ�������
const DWORD WStyle_WindowModeTable[WSTYLE_NUM] = 
{
	WS_MINIMIZEBOX | WS_POPUP | WS_SYSMENU | WS_CAPTION,
	WS_POPUP | WS_SYSMENU,
	WS_POPUP | WS_SYSMENU,
	WS_MINIMIZEBOX | WS_POPUP | WS_SYSMENU | WS_CAPTION,
	WS_POPUP | WS_SYSMENU,
	WS_POPUP | WS_SYSMENU | WS_CAPTION,
	WS_MINIMIZEBOX | WS_POPUP | WS_SYSMENU | WS_CAPTION,
	WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_POPUP | WS_SYSMENU | WS_CAPTION,
	WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_POPUP | WS_SYSMENU | WS_CAPTION | WS_MAXIMIZE,
	WS_MINIMIZEBOX | WS_POPUP | WS_SYSMENU | WS_CAPTION,
	WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_POPUP | WS_SYSMENU | WS_CAPTION,
	WS_POPUP | WS_CAPTION,
} ;

const DWORD WExStyle_WindowModeTable[WSTYLE_NUM] = 
{
	WS_EX_CLIENTEDGE | WS_EX_WINDOWEDGE,
	WS_EX_CLIENTEDGE | WS_EX_WINDOWEDGE,
	0,
	WS_EX_TOOLWINDOW,
	WS_EX_TOOLWINDOW,
	WS_EX_WINDOWEDGE,
	WS_EX_CLIENTEDGE | WS_EX_WINDOWEDGE,
	WS_EX_CLIENTEDGE | WS_EX_WINDOWEDGE,
	WS_EX_CLIENTEDGE | WS_EX_WINDOWEDGE,
	WS_EX_WINDOWEDGE,
	WS_EX_WINDOWEDGE,
	WS_EX_CLIENTEDGE | WS_EX_WINDOWEDGE,
} ;

const DWORD WStyle_FullScreenModeTable[WSTYLE_NUM] = 
{
	WS_POPUP,
	WS_POPUP,
	WS_POPUP,
	WS_POPUP,
	WS_POPUP,
	WS_POPUP,
	WS_POPUP,
	WS_POPUP,
	WS_POPUP,
	WS_POPUP,
	WS_POPUP,
	WS_POPUP,
} ;

const DWORD WExStyle_FullScreenModeTable[WSTYLE_NUM] = 
{
	WS_EX_TOPMOST,
	WS_EX_TOPMOST,
	WS_EX_TOPMOST,
	WS_EX_TOPMOST,
	WS_EX_TOPMOST,
	WS_EX_TOPMOST,
	WS_EX_TOPMOST,
	WS_EX_TOPMOST,
	WS_EX_TOPMOST,
	WS_EX_TOPMOST,
	WS_EX_TOPMOST,
	WS_EX_TOPMOST,
} ;

// �E�C���h�E�̉��s���ʒu�ݒ�^�C�v�ƑΉ����� Win32API �̒l
static HWND WindowZType_Win32Param[] =
{
	HWND_TOP,			// DX_WIN_ZTYPE_NORMAL
	HWND_BOTTOM,		// DX_WIN_ZTYPE_BOTTOM
	HWND_TOP,			// DX_WIN_ZTYPE_TOP
	HWND_TOPMOST,		// DX_WIN_ZTYPE_TOPMOST
};

//typedef LRESULT ( CALLBACK *MSGFUNC)(int, WPARAM, LPARAM) ;		// ���b�Z�[�W�t�b�N�̃R�[���o�b�N�֐�
#define F10MES				( WM_USER + 111 )
#define F12MES				( WM_USER + 112 )

// �\���̒�` --------------------------------------------------------------------

// �������ϐ��錾 --------------------------------------------------------------

// ���k�L�[�{�[�h�t�b�N�c�k�k�o�C�i���f�[�^
extern int  DxKeyHookBinaryConvert ;
extern BYTE DxKeyHookBinary[] ;
extern int  DxKeyHookBinary_x64_Convert ;
extern BYTE DxKeyHookBinary_x64[] ;

WINDATA WinData ;												// �E�C���h�E�̃f�[�^

// �֐��v���g�^�C�v�錾-----------------------------------------------------------

#ifndef DX_NON_INPUT

// �}�E�X�̓��͏���ۑ�����
static	void		StockMouseInputInfo( int Button ) ;

#endif // DX_NON_INPUT

// �E�C���h�E�֘A
static void			GetMainWindowSize( int *SizeX, int *SizeY ) ;								// ���C���E�C���h�E�̃T�C�Y���擾����
static int			ConvScreenPositionToDxScreenPosition( int ScreenX, int ScreenY, int *DxScreenX, int *DxScreenY ) ;		// �X�N���[�����W���c�w���C�u������ʍ��W�ɕϊ�����

// ���b�Z�[�W�����֐�
static	LRESULT		CALLBACK DxLib_WinProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam ) ;	// ���C���E�C���h�E�̃��b�Z�[�W�R�[���o�b�N�֐�
//LRESULT CALLBACK	LowLevelKeyboardProc (INT nCode, WPARAM wParam, LPARAM lParam)	;			// �t�b�N���ꂽ���̃R�[���o�b�N�֐�
#ifdef DX_THREAD_SAFE
DWORD	WINAPI		ProcessMessageThreadFunction( LPVOID ) ;									// ProcessMessage ���Ђ�����ĂтÂ���X���b�h
#endif

// �c�[���o�[�֌W
static	int			SearchToolBarButton( int ID ) ;												// �w��̂h�c�̃{�^���̃C���f�b�N�X�𓾂�

// ���j���[�֌W
static	int			SearchMenuItem( const wchar_t *ItemName, int ItemID, HMENU SearchMenu, HMENU *Menu, int *Index ) ;					// ���j���[�\���̒�����A�I�����ڂ̈ʒu���𓾂�( -1:�G���[  0:������Ȃ�����  1:�������� )

static	int			GetDisplayMenuState( void ) ;												// ���j���[���\������ׂ����ǂ������擾����
static	int			_GetMenuItemInfo( HMENU Menu, int Index, MENUITEMINFOW *Buffer ) ;			// ���j���[�A�C�e���̏����擾����
static	HMENU		MenuItemSubMenuSetup( const wchar_t *ItemName, int ItemID ) ;				// �w��̑I�����ڂɃT�u���j���[��t������悤�ɏ���������

static	int			ListupMenuItemInfo( HMENU Menu ) ;											// ���j���[�̑I�����ڂ̏��̈ꗗ���쐬���鎞�Ɏg�p����֐�
static	int			AddMenuItemInfo( HMENU Menu, int Index, int ID, const wchar_t *Name ) ;		// ���j���[�̑I�����ڂ̏���ǉ�����
static	int			DeleteMenuItemInfo( const wchar_t *Name, int ID ) ;							// ���j���[�̑I�����ڂ̏����폜����
static	WINMENUITEMINFO *SearchMenuItemInfo( const wchar_t *Name, int ID ) ;					// ���j���[�̑I�����ڂ̏����擾����
static	int			GetNewMenuItemID( void ) ;													// �V�����I�����ڂ̂h�c���擾����

// ���b�Z�[�W�����֐�
		int			WM_PAINTProcess( HDC Dc, RECT *FillRect, int AlwaysFillFlag = FALSE ) ;
		int			WM_SIZEProcess( void ) ;
		int			WM_SIZINGProcess( WPARAM wParam, LPARAM lParam ) ;
		int			WM_MOVEProcess( LPARAM lParam ) ;




// �v���O���� --------------------------------------------------------------------

// �������I���n�֐�

#pragma optimize("", off)

// �E�C���h�E�֌W�̏������֐�
extern int InitializeWindow( void )
{
	WNDCLASSEXW wc ;
	int WindowSizeX , WindowSizeY ;
//	HDC hdc ;
	TIMECAPS tc ;
	wchar_t CurrentDirectory[ FILEPATH_MAX ] ;
	wchar_t WindowText[ FILEPATH_MAX ] ;
	wchar_t ClassName[ 256 ] ;

	LONGLONG		OneSecCount										= WinData.OneSecCount ;
	int				UseRDTSCFlag									= WinData.UseRDTSCFlag ;
	int 			WindowModeFlag 									= WinData.WindowModeFlag ;
	int				NonActiveRunFlag								= WinData.NonActiveRunFlag  ;
	int				UseChangeWindowModeFlag							= WinData.UseChangeWindowModeFlag ;
	void 			(*ChangeWindowModeCallBackFunction)(void *) 	= WinData.ChangeWindowModeCallBackFunction ;
	void 			*ChangeWindowModeCallBackFunctionData 			= WinData.ChangeWindowModeCallBackFunctionData ;
	HACCEL			Accel											= WinData.Accel ;
	HMENU 			Menu 											= WinData.Menu ;
	int				MenuUseFlag 									= WinData.MenuUseFlag ;
	int 			(*MenuProc)( WORD ID )							= WinData.MenuProc ;
	void			(*MenuCallBackFunction)( const TCHAR *ItemName, int ItemID ) = WinData.MenuCallBackFunction ;
	int				NotMenuDisplayFlag								= WinData.NotMenuDisplayFlag ;
	int				NotMenuAutoDisplayFlag							= WinData.NotMenuAutoDisplayFlag ;
//	int 			MenuShredRunFlag								= WinData.MenuShredRunFlag ;
//	int 			MenuStartKey 									= WinData.MenuStartKey ;
	int 			WindowStyle 									= WinData.WindowStyle ;
	int				WindowZType										= WinData.WindowZType ;
	HRGN 			WindowRgn 										= WinData.WindowRgn ;
	int 			IconID 											= WinData.IconID ;
	HICON			IconHandle										= WinData.IconHandle ;
	int				ProcessorNum									= WinData.ProcessorNum ;
	int 			DirectXVersion 									= WinData.DirectXVersion ;
	int 			WindowsVersion 									= WinData.WindowsVersion ;
	int 			DoubleStartValidFlag							= WinData.DoubleStartValidFlag ;
	WNDPROC 		UserWindowProc 									= WinData.UserWindowProc ;
	int				DragFileValidFlag								= WinData.DragFileValidFlag ;
	int				EnableWindowText								= WinData.EnableWindowText  ;
	int				WindowX											= WinData.WindowX;
	int				WindowY											= WinData.WindowY;
	int				WindowPosValid									= WinData.WindowPosValid;
	int				WindowMaxWidth									= WinData.WindowMaxWidth ;
	int				WindowMaxHeight									= WinData.WindowMaxHeight ;
	int				WindowMaxSizeValid								= WinData.WindowMaxSizeValid ;
	int				WindowMinWidth									= WinData.WindowMinWidth ;
	int				WindowMinHeight									= WinData.WindowMinHeight ;
	int				WindowMinSizeValid								= WinData.WindowMinSizeValid ;
	int				WindowWidth										= WinData.WindowWidth;
	int				WindowHeight									= WinData.WindowHeight;
	int				WindowSizeValid									= WinData.WindowSizeValid;
	int				WindowSizeChangeEnable							= WinData.WindowSizeChangeEnable ;
	int				ScreenNotFitWindowSize							= WinData.ScreenNotFitWindowSize ;
	double			WindowSizeExRateX								= WinData.WindowSizeExRateX ;
	double			WindowSizeExRateY								= WinData.WindowSizeExRateY ;
	int				NotWindowVisibleFlag							= WinData.NotWindowVisibleFlag ;
	int				WindowMinimizeFlag								= WinData.WindowMinimizeFlag ;
	int				UseFPUPreserve									= WinData.UseFPUPreserve ;
	int				NonUserCloseEnableFlag							= WinData.NonUserCloseEnableFlag ;
	int				NonDxLibEndPostQuitMessageFlag					= WinData.NonDxLibEndPostQuitMessageFlag ;
	int				NotUseDxLibWM_PAINTProcess						= WinData.NotUseDxLibWM_PAINTProcess ;
	HWND			MainWindow										= WinData.MainWindow ;
	HWND			UserChildWindow									= WinData.UserChildWindow ;
	int				UserWindowFlag									= WinData.UserWindowFlag ;
	int				NotUserWindowMessageProcessDXLibFlag			= WinData.NotUserWindowMessageProcessDXLibFlag ;
	int				NotMoveMousePointerOutClientAreaFlag			= WinData.NotMoveMousePointerOutClientAreaFlag ;
	int				BackBufferTransColorFlag						= WinData.BackBufferTransColorFlag ;
	int				UseUpdateLayerdWindowFlag						= WinData.UseUpdateLayerdWindowFlag ;
//	int				DxConflictCheckFlag								= WinData.DxConflictCheckFlag ;
#if defined( DX_THREAD_SAFE ) || defined( DX_THREAD_SAFE_NETWORK_ONLY )
	int				DxConflictCheckCounter							= WinData.DxConflictCheckCounter ;
	int				DxUseThreadFlag									= WinData.DxUseThreadFlag ;
	DWORD			DxUseThreadID									= WinData.DxUseThreadID ;
	DWORD_PTR		DxConflictWaitThreadID[MAX_THREADWAIT_NUM][2] ;
	int				DxConflictWaitThreadIDInitializeFlag			= WinData.DxConflictWaitThreadIDInitializeFlag ;
	int				DxConflictWaitThreadNum							= WinData.DxConflictWaitThreadNum ;
	DX_CRITICAL_SECTION DxConflictCheckCriticalSection					= WinData.DxConflictCheckCriticalSection ;
	_MEMCPY( DxConflictWaitThreadID, WinData.DxConflictWaitThreadID, sizeof( DxConflictWaitThreadID ) ) ;
#endif // defined( DX_THREAD_SAFE ) || defined( DX_THREAD_SAFE_NETWORK_ONLY )
	PCINFO			PcInfo											= WinData.PcInfo ;
	int				UseMMXFlag										= WinData.UseMMXFlag ;
	int				UseSSEFlag										= WinData.UseSSEFlag ;
	int				UseSSE2Flag										= WinData.UseSSE2Flag ;
	HMODULE			LoadResourModule								= WinData.LoadResourModule ;
	double			ExtendRateX, ExtendRateY ;

	_WCSCPY_S( CurrentDirectory, sizeof( CurrentDirectory ), WinData.CurrentDirectory ) ;
	_WCSCPY_S( WindowText, sizeof( WindowText ), WinData.WindowText ) ;
	_WCSCPY_S( ClassName, sizeof( ClassName ), WinData.ClassName ) ;

	// ������
	_MEMSET( &WinData, 0, sizeof( WINDATA ) ) ;

	_WCSCPY_S( WinData.CurrentDirectory, sizeof( WinData.CurrentDirectory ), CurrentDirectory ) ;
	_WCSCPY_S( WinData.WindowText, sizeof( WinData.WindowText ), WindowText ) ;
	_WCSCPY_S( WinData.ClassName, sizeof( WinData.ClassName ), ClassName ) ;

	WinData.OneSecCount								= OneSecCount ;
	WinData.UseRDTSCFlag							= UseRDTSCFlag ;
	WinData.UseMMXFlag								= UseMMXFlag ;
	WinData.UseSSEFlag								= UseSSEFlag ;
	WinData.UseSSE2Flag								= UseSSE2Flag ;
	WinData.WindowModeFlag							= WindowModeFlag ;
	WinData.UseChangeWindowModeFlag					= UseChangeWindowModeFlag ;
	WinData.ChangeWindowModeCallBackFunction 		= ChangeWindowModeCallBackFunction ;
	WinData.ChangeWindowModeCallBackFunctionData 	= ChangeWindowModeCallBackFunctionData ;
	WinData.Accel									= Accel ;
	WinData.Menu 									= Menu ;
	WinData.MenuUseFlag 							= MenuUseFlag ; 
	WinData.MenuProc 								= MenuProc ;
//	WinData.MenuShredRunFlag						= MenuShredRunFlag ;
//	WinData.MenuStartKey 							= MenuStartKey ;
	WinData.WindowStyle 							= WindowStyle ;
	WinData.WindowZType								= WindowZType ;
	WinData.WindowRgn 								= WindowRgn ;
	WinData.IconID 									= IconID ;
	WinData.IconHandle								= IconHandle ;
	WinData.ProcessorNum							= ProcessorNum ;
	WinData.DirectXVersion 							= DirectXVersion ;
	WinData.WindowsVersion 							= WindowsVersion ;
	WinData.NonActiveRunFlag 						= NonActiveRunFlag ;
	WinData.DoubleStartValidFlag 					= DoubleStartValidFlag ;
	WinData.UserWindowProc 							= UserWindowProc ;
	WinData.DragFileValidFlag						= DragFileValidFlag ;
	WinData.EnableWindowText						= EnableWindowText ;
	WinData.WindowX									= WindowX;
	WinData.WindowY									= WindowY;
	WinData.WindowPosValid							= WindowPosValid;
	WinData.WindowMaxWidth							= WindowMaxWidth ;
	WinData.WindowMaxHeight							= WindowMaxHeight ;
	WinData.WindowMaxSizeValid						= WindowMaxSizeValid ;
	WinData.WindowMinWidth							= WindowMinWidth ;
	WinData.WindowMinHeight							= WindowMinHeight ;
	WinData.WindowMinSizeValid						= WindowMinSizeValid ;
	WinData.WindowWidth								= WindowWidth;
	WinData.WindowHeight							= WindowHeight;
	WinData.WindowSizeValid							= WindowSizeValid;
	WinData.MenuCallBackFunction					= MenuCallBackFunction ;
	WinData.NotMenuDisplayFlag						= NotMenuDisplayFlag ;
	WinData.NotMenuAutoDisplayFlag					= NotMenuAutoDisplayFlag ;
	WinData.WindowSizeChangeEnable					= WindowSizeChangeEnable ;
	WinData.ScreenNotFitWindowSize					= ScreenNotFitWindowSize ;
	WinData.WindowSizeExRateX						= WindowSizeExRateX ;
	WinData.WindowSizeExRateY						= WindowSizeExRateY ;
	WinData.NotWindowVisibleFlag					= NotWindowVisibleFlag ;
	WinData.WindowMinimizeFlag						= WindowMinimizeFlag ;
	WinData.UseFPUPreserve							= UseFPUPreserve ;
	WinData.NonUserCloseEnableFlag					= NonUserCloseEnableFlag ;
	WinData.NonDxLibEndPostQuitMessageFlag			= NonDxLibEndPostQuitMessageFlag ;
	WinData.NotUseDxLibWM_PAINTProcess				= NotUseDxLibWM_PAINTProcess ;
	WinData.MainWindow								= MainWindow ;
	WinData.UserChildWindow							= UserChildWindow ;
	WinData.UserWindowFlag							= UserWindowFlag ;
	WinData.NotUserWindowMessageProcessDXLibFlag	= NotUserWindowMessageProcessDXLibFlag ;
	WinData.NotMoveMousePointerOutClientAreaFlag	= NotMoveMousePointerOutClientAreaFlag ;
	WinData.BackBufferTransColorFlag				= BackBufferTransColorFlag ;
	WinData.UseUpdateLayerdWindowFlag				= UseUpdateLayerdWindowFlag ;
//	WinData.DxConflictCheckFlag						= DxConflictCheckFlag ;
#if defined( DX_THREAD_SAFE ) || defined( DX_THREAD_SAFE_NETWORK_ONLY )
	WinData.DxConflictCheckCounter					= DxConflictCheckCounter ;
	WinData.DxUseThreadFlag							= DxUseThreadFlag ;
	WinData.DxUseThreadID							= DxUseThreadID ;
	WinData.DxConflictWaitThreadIDInitializeFlag	= DxConflictWaitThreadIDInitializeFlag ;
	WinData.DxConflictWaitThreadNum					= DxConflictWaitThreadNum ;
	WinData.DxConflictCheckCriticalSection			= DxConflictCheckCriticalSection ;
	WinData.WaitTime								= -1 ;
	_MEMCPY( WinData.DxConflictWaitThreadID, DxConflictWaitThreadID, sizeof( DxConflictWaitThreadID ) ) ;
#endif // defined( DX_THREAD_SAFE ) || defined( DX_THREAD_SAFE_NETWORK_ONLY )
	WinData.PcInfo									= PcInfo ;
	WinData.LoadResourModule						= LoadResourModule ;

#ifdef DX_NON_GRAPHICS
	// �O���t�B�b�N�X�����������ȏꍇ�͕K���E�C���h�E���[�h
	WinData.WindowModeFlag = TRUE ;
#endif // DX_NON_GRAPHICS

	// �C���X�^���X�n���h���̎擾
	WinData.Instance = WinAPIData.Win32Func.GetModuleHandleWFunc( NULL ) ;

	// �X���b�h�h�c�̎擾
	WinData.MainThreadID = GetCurrentThreadId() ;


#ifndef DX_NON_ASYNCLOAD
	// �񓯊��ǂݍ��ݏ����̏�����
	InitializeASyncLoad( WinData.MainThreadID ) ;
#endif // DX_NON_ASYNCLOAD

	// �t�@�C���A�N�Z�X�����̏�����
	InitializeFile() ;

	// �N���X���������ݒ肳��Ă��Ȃ��ꍇ�̓f�t�H���g�̖��O��ݒ肷��
	if( WinData.ClassName[0] == L'\0' )
	{
		// �E�C���h�E�^�C�g�����ݒ肳��Ă���ꍇ�͂����ݒ肷��
		if( WinData.WindowText[0] != L'\0' )
		{
			_WCSCPY_S( WinData.ClassName, sizeof( WinData.ClassName ), WinData.WindowText );
		}
		else
		{
			_WCSCPY_S( WinData.ClassName, sizeof( WinData.ClassName ), DXCLASSNAME );
		}
	}

	// �󂫃������e�ʂ̏����o��
	{
		wchar_t str[256];

		if( WinData.WindowsVersion >= DX_WINDOWSVERSION_2000 )
		{
			GLOBALMEMORYSTATUSEX_FUNC GlobalMemoryStatusExFunc ;
			__MEMORYSTATUSEX MemEx ;
			HRESULT mhr ;
			HMODULE Kernel32DLL ;

			Kernel32DLL = LoadLibraryW( L"Kernel32.dll" ) ;
			if( Kernel32DLL )
			{
				GlobalMemoryStatusExFunc = ( GLOBALMEMORYSTATUSEX_FUNC )GetProcAddress( Kernel32DLL, "GlobalMemoryStatusEx" ) ;
				if( GlobalMemoryStatusExFunc )
				{
					_MEMSET( &MemEx, 0, sizeof( MemEx ) ) ;
					MemEx.dwLength = sizeof( MemEx ) ;
					mhr = GlobalMemoryStatusExFunc( &MemEx ) ;
					DXST_ERRORLOGFMT_ADDUTF16LE(( "\xe1\x30\xe2\x30\xea\x30\xcf\x7d\xcf\x91\x3a\x00\x25\x00\x2e\x00\x32\x00\x66\x00\x4d\x00\x42\x00\x20\x00\x20\x00\x7a\x7a\x4d\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x3a\x00\x25\x00\x2e\x00\x32\x00\x66\x00\x4d\x00\x42\x00\x20\x00\x00"/*@ L"����������:%.2fMB  �󂫃������̈�:%.2fMB " @*/,
								( double )( LONGLONG )MemEx.ullTotalPhys / 0x100000 , ( double )( LONGLONG )MemEx.ullAvailPhys / 0x100000 )) ;  

					WinData.PcInfo.FreeMemorySize  = ( LONGLONG )MemEx.ullAvailPhys ;
					WinData.PcInfo.TotalMemorySize = ( LONGLONG )MemEx.ullTotalPhys ;
				}

				FreeLibrary( Kernel32DLL ) ;
			}
		}

		if( WinData.PcInfo.TotalMemorySize == 0 )
		{
			MEMORYSTATUS Mem ;

			GlobalMemoryStatus( &Mem ) ;
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\xe1\x30\xe2\x30\xea\x30\xcf\x7d\xcf\x91\x3a\x00\x25\x00\x2e\x00\x32\x00\x66\x00\x4d\x00\x42\x00\x20\x00\x20\x00\x7a\x7a\x4d\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x3a\x00\x25\x00\x2e\x00\x32\x00\x66\x00\x4d\x00\x42\x00\x20\x00\x00"/*@ L"����������:%.2fMB  �󂫃������̈�:%.2fMB " @*/,
						( double )Mem.dwTotalPhys / 0x100000 , ( double )Mem.dwAvailPhys / 0x100000 )) ;  
			_SWNPRINTF( str, sizeof( str ) / 2, L"%.2fMB\n", ( double )Mem.dwAvailPhys / 0x100000 );
			OutputDebugStringW( str ) ;

			WinData.PcInfo.FreeMemorySize  = ( LONGLONG )Mem.dwAvailPhys ;
			WinData.PcInfo.TotalMemorySize = ( LONGLONG )Mem.dwTotalPhys ;
		}
	}

	// �f�X�N�`�b�v�̃T�C�Y�ƃJ���[�r�b�g�[�x��ۑ�
//	WinData.DefaultScreenSize.cx = GetSystemMetrics( SM_CXSCREEN ) ;
//	WinData.DefaultScreenSize.cy = GetSystemMetrics( SM_CYSCREEN ) ;
//	hdc = GetDC( NULL ) ;
//	WinData.DefaultColorBitCount = GetDeviceCaps( hdc , PLANES ) * GetDeviceCaps( hdc , BITSPIXEL ) ;
//	WinData.DefaultRefreshRate = GetDeviceCaps( hdc, VREFRESH ) ;
//	ReleaseDC( NULL , hdc ) ;

	// �`��̈�̃T�C�Y���擾
	GetMainWindowSize( &WindowSizeX, &WindowSizeY ) ;

	// �p�t�H�[�}���X�J�E���^�[���̏�����
	{
		// �p�t�H�[�}���X�J�E���^�[�̎��g�����擾����
		QueryPerformanceFrequency( ( LARGE_INTEGER * )&WinData.PerformanceClock ) ;

		// �g���邩�ǂ�����ۑ����Ă���
		WinData.PerformanceTimerFlag = WinData.PerformanceClock != 0 ;
	}

	// �^�C�}�̐��x��ݒ肷��
	{
		WinAPIData.Win32Func.timeGetDevCapsFunc( &tc , sizeof(TIMECAPS) );

		// �}���`���f�B�A�^�C�}�[�̃T�[�r�X���x���ő�� 
		WinAPIData.Win32Func.timeBeginPeriodFunc( tc.wPeriodMin ) ;
	}

	// �^�C�}�[�̐��x����������
	if( WinData.PerformanceTimerFlag )
	{
		int Cnt1 , Cnt2 ;
		DWORD Time1 , NowTime1, StartTime ;
		LONGLONG Time2 , NowTime2 ;

		DXST_ERRORLOG_ADDUTF16LE( "\xbf\x30\xa4\x30\xde\x30\xfc\x30\x6e\x30\xbe\x7c\xa6\x5e\x92\x30\x1c\x69\xfb\x67\x57\x30\x7e\x30\x59\x30\x0a\x00\x00"/*@ L"�^�C�}�[�̐��x���������܂�\n" @*/ ) ;

		Cnt1 = 0 ;
		Cnt2 = 0 ;
		Time1 = WinAPIData.Win32Func.timeGetTimeFunc() ;
		QueryPerformanceCounter( ( LARGE_INTEGER * )&Time2 ) ; 
		StartTime = WinAPIData.Win32Func.timeGetTimeFunc() ;
		while( /*!NS_ProcessMessage() &&*/ Cnt1 != 60 && Cnt2 != 60 && WinAPIData.Win32Func.timeGetTimeFunc() - StartTime < 1000 )
		{
			QueryPerformanceCounter( ( LARGE_INTEGER * )&NowTime2 ) ;
			if( NowTime2 != Time2 )
			{
				Cnt2 ++ ;
				Time2 = NowTime2 ;
			}

			if( ( NowTime1 = WinAPIData.Win32Func.timeGetTimeFunc() ) != Time1 )
			{
				Cnt1 ++ ;
				Time1 = NowTime1 ;
			}
		}

		{
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\xbe\x7c\xa6\x5e\x50\x7d\x9c\x67\x20\x00\xf4\x66\xb0\x65\xde\x56\x70\x65\x20\x00\xde\x30\xeb\x30\xc1\x30\xe1\x30\xc7\x30\xa3\x30\xa2\x30\xbf\x30\xa4\x30\xde\x30\xfc\x30\x1a\xff\x25\x00\x64\x00\x20\x00\x20\x00\xd1\x30\xd5\x30\xa9\x30\xfc\x30\xde\x30\xf3\x30\xb9\x30\xab\x30\xa6\x30\xf3\x30\xbf\x30\xfc\x30\x1a\xff\x25\x00\x64\x00\x00"/*@ L"���x���� �X�V�� �}���`���f�B�A�^�C�}�[�F%d  �p�t�H�[�}���X�J�E���^�[�F%d" @*/, Cnt1 , Cnt2 )) ;

#ifndef DX_NON_LOG
			NS_ErrorLogAdd( _T( " " ) ); // ��̃o�O���p
#endif
			if( Cnt1 > Cnt2 )
			{
				DXST_ERRORLOGFMT_ADDUTF16LE(( "\x4d\x00\x75\x00\x6c\x00\x74\x00\x69\x00\x20\x00\x4d\x00\x65\x00\x64\x00\x69\x00\x61\x00\x20\x00\x54\x00\x69\x00\x6d\x00\x65\x00\x72\x00\x20\x00\x92\x30\x7f\x4f\x28\x75\x57\x30\x7e\x30\x59\x30\x20\x00\x54\x00\x69\x00\x6d\x00\x65\x00\x72\x00\x20\x00\xbe\x7c\xa6\x5e\x20\x00\x3a\x00\x20\x00\x25\x00\x64\x00\x2e\x00\x30\x00\x30\x00\x20\x00\x6d\x00\x73\x00\x20\x00\x00"/*@ L"Multi Media Timer ���g�p���܂� Timer ���x : %d.00 ms " @*/, tc.wPeriodMin )) ;
				WinData.PerformanceTimerFlag = FALSE ;
			}
			else
			{
				DXST_ERRORLOGFMT_ADDUTF16LE(( "\xd1\x30\xd5\x30\xa9\x30\xfc\x30\xde\x30\xf3\x30\xb9\x30\xab\x30\xa6\x30\xf3\x30\xbf\x30\xfc\x30\x92\x30\x7f\x4f\x28\x75\x57\x30\x7e\x30\x59\x30\x20\x00\xbf\x30\xa4\x30\xde\x30\xfc\x30\xbe\x7c\xa6\x5e\x20\x00\x3a\x00\x20\x00\x25\x00\x6c\x00\x66\x00\x20\x00\x4b\x00\x48\x00\x7a\x00\x20\x00\x00"/*@ L"�p�t�H�[�}���X�J�E���^�[���g�p���܂� �^�C�}�[���x : %lf KHz " @*/, WinData.PerformanceClock / 1000.0 )) ;
			}
#ifndef DX_NON_LOG
			NS_ErrorLogAdd( _T( " " ) ); // ��̃o�O���p
#endif
		}
	}

	// �����_���W����������
#ifndef DX_NON_MERSENNE_TWISTER
	srandMT( ( unsigned int )NS_GetNowCount() ) ;
#else
	srand( NS_GetNowCount() ) ;
#endif

	// �X�N���[���Z�[�o�[����
	SystemParametersInfoW( SPI_SETSCREENSAVEACTIVE, FALSE, NULL, SPIF_SENDWININICHANGE ) ;

	// �E�C���h�E�Ɋ֘A���鏈���� NotWinFlag �������Ă���ꍇ�͎��s���Ȃ�
	if( DxSysData.NotWinFlag == FALSE )
	{
		// ��d�N���h�~����
		DXST_ERRORLOG_ADDUTF16LE( "\xbd\x30\xd5\x30\xc8\x30\x6e\x30\x8c\x4e\xcd\x91\x77\x8d\xd5\x52\x1c\x69\xfb\x67\x2e\x00\x2e\x00\x2e\x00\x20\x00\x00"/*@ L"�\�t�g�̓�d�N������... " @*/ ) ;
		{
			if( FindWindowW( WinData.ClassName , NULL ) != NULL )
			{
				if( WinData.DoubleStartValidFlag == FALSE )
				{
					DXST_ERRORLOG_ADDUTF16LE( "\x8c\x4e\xcd\x91\x77\x8d\xd5\x52\x55\x30\x8c\x30\x66\x30\x44\x30\x7e\x30\x59\x30\x01\x30\xbd\x30\xd5\x30\xc8\x30\x92\x30\x42\x7d\x86\x4e\x57\x30\x7e\x30\x59\x30\x0a\x00\x00"/*@ L"��d�N������Ă��܂��A�\�t�g���I�����܂�\n" @*/ ) ;
					WinData.QuitMessageFlag = TRUE;
					DxLib_EndRequest() ;
					return -1 ;
				}
				else
				{
					DXST_ERRORLOG_ADDUTF16LE( "\x8c\x4e\xcd\x91\x77\x8d\xd5\x52\x55\x30\x8c\x30\x66\x30\x44\x30\x7e\x30\x59\x30\x4c\x30\x9a\x7d\x4c\x88\x57\x30\x7e\x30\x59\x30\x0a\x00\x00"/*@ L"��d�N������Ă��܂������s���܂�\n" @*/ ) ;
				}
			}
			else
			{
				DXST_ERRORLOG_ADDUTF16LE( "\x8c\x4e\xcd\x91\x77\x8d\xd5\x52\x6f\x30\x55\x30\x8c\x30\x66\x30\x44\x30\x7e\x30\x5b\x30\x93\x30\x67\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"��d�N���͂���Ă��܂���ł���\n" @*/ ) ;
			}
		}

		if( WinData.MenuUseFlag && DxSysData.NotWinFlag == FALSE )
		{
			WinData.MenuItemInfoNum = 0 ;
			ListupMenuItemInfo( WinData.Menu ) ;
		}

		// ���j���[��ǂݍ���
	/*	if( WinData.MenuUseFlag )
		{
			// ���j���[�����[�h
			WinData.Menu = LoadMenuW( WinData.Instance, MAKEINTRESOURCE( WinData.MenuResourceID ) ) ;
	//		WinData.MenuResourceID = FALSE ;

			// ���s���Ă�����I��
			if( WinData.Menu == NULL ) WinData.MenuUseFlag = FALSE ;
		}
		else
		{
			WinData.Menu = NULL ;
		}
	*/

		// �\�t�g�v���Z�X�̎��s�D�揇�ʂ��ō����x���ɃZ�b�g
	//	DXST_ERRORLOG_ADDW( L"�\�t�g�̎��s�D�惌�x�����グ�܂���\n" ) ;
	//	SetPriorityClass( GetCurrentProcess() , HIGH_PRIORITY_CLASS );

		// �E�C���h�E�����[�U�[����񋟂���Ă��Ȃ��ꍇ�͂c�w���C�u�������쐬����
		// ���[�U�[����񋟂���Ă���ꍇ�̓v���V�[�W�����Ă΂��悤�ɂ���
		if( WinData.UserWindowFlag == TRUE )
		{
#ifdef _WIN64
			WinData.DefaultUserWindowProc = (WNDPROC)GetWindowLongPtrW( WinData.MainWindow, GWLP_WNDPROC ) ;
#else
			WinData.DefaultUserWindowProc = (WNDPROC)GetWindowLongW( WinData.MainWindow, GWL_WNDPROC ) ;
#endif
		}
		else
		{
			// �E�C���h�E�쐬���t���O�𗧂Ă�
			WinData.WindowCreateFlag = TRUE ;

			// ���C���E�C���h�E�̃E�C���h�E�N���X��o�^
			_MEMSET( &wc , 0, sizeof( wc ) ) ;
			{
				wc.style			= CS_HREDRAW | CS_VREDRAW ;
				wc.lpfnWndProc		= DxLib_WinProc ;
				wc.cbClsExtra		= 0 ;
				wc.cbWndExtra		= 0 ;
				wc.hInstance		= WinData.Instance ;
				wc.hIcon			= WinData.IconHandle != NULL ? WinData.IconHandle : ( LoadIconW( WinData.Instance , ( WinData.IconID == 0 ) ? ( LPWSTR )IDI_APPLICATION : MAKEINTRESOURCEW( WinData.IconID ) ) ) ;
				wc.hCursor			= LoadCursorW( NULL , ( LPCWSTR )IDC_ARROW ) ;
		//		wc.hbrBackground	= WinData.WindowModeFlag ? (HBRUSH)( COLOR_WINDOW + 1 ) : (HBRUSH)GetStockObject(BLACK_BRUSH); ;
		//		wc.hbrBackground	= ( WinData.WindowRgn == NULL && WinData.WindowModeFlag == TRUE ) ? (HBRUSH)GetStockObject(BLACK_BRUSH) : (HBRUSH)GetStockObject(NULL_BRUSH);
				wc.hbrBackground	= (HBRUSH)GetStockObject(NULL_BRUSH);
				wc.lpszMenuName		= NULL ;
				wc.lpszClassName	= WinData.ClassName ;
				wc.cbSize			= sizeof( WNDCLASSEX );
				wc.hIconSm			= NULL ;

				DXST_ERRORLOG_ADDUTF16LE( "\xa6\x30\xa4\x30\xf3\x30\xc9\x30\xa6\x30\xaf\x30\xe9\x30\xb9\x30\x92\x30\x7b\x76\x32\x93\x57\x30\x7e\x30\x59\x30\x2e\x00\x2e\x00\x2e\x00\x20\x00\x00"/*@ L"�E�C���h�E�N���X��o�^���܂�... " @*/ ) ;
				if( !RegisterClassExW( &wc ) )
				{
					WinData.QuitMessageFlag = TRUE;
					DxLib_EndRequest() ;
					//return 	DXST_ERRORLOG_ADDW( L"�E�C���h�E�N���X�̓o�^�Ɏ��s���܂���\n" ) ;
				}
				else
				{
					DXST_ERRORLOG_ADDUTF16LE( "\x7b\x76\x32\x93\x6b\x30\x10\x62\x9f\x52\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�o�^�ɐ������܂���\n" @*/ ) ;
				}
			}

			// �E�C���h�E�l�[�����Z�b�g
			if( WinData.EnableWindowText == FALSE )
			{
				_WCSCPY_S( WinData.WindowText, sizeof( WinData.WindowText ), WIN_DEFAULT_TITLE ) ;
			}
			
			// �E�C���h�E�𐶐�
#ifndef DX_NON_GRAPHICS
			if( WinData.WindowModeFlag )
#endif // DX_NON_GRAPHICS
			{
				RECT Rect ;
				LONG AddStyle, AddExStyle ;
				int DesktopW, DesktopH, DesktopX, DesktopY ;

				// �E�C���h�E���[�h��
				DXST_ERRORLOG_ADDUTF16LE( "\xa6\x30\xa4\x30\xf3\x30\xc9\x30\xa6\x30\xe2\x30\xfc\x30\xc9\x30\x77\x8d\xd5\x52\x28\x75\x6e\x30\xa6\x30\xa4\x30\xf3\x30\xc9\x30\xa6\x30\x92\x30\x5c\x4f\x10\x62\x57\x30\x7e\x30\x59\x30\x0a\x00\x00"/*@ L"�E�C���h�E���[�h�N���p�̃E�C���h�E���쐬���܂�\n" @*/ ) ;

				AddExStyle = 0 ;
//				if( WinData.NotWindowVisibleFlag == TRUE ) AddExStyle |= WS_EX_TRANSPARENT ;

				// �o�b�N�o�b�t�@�̓��ߐF�̕����𓧉߂�����t���O�������Ă��邩�A
				// UpdateLayerdWindow ���g�p����t���O���o���Ă���ꍇ�� WS_EX_LAYERED ��ǉ�����
				if( WinData.BackBufferTransColorFlag == TRUE ||
					WinData.UseUpdateLayerdWindowFlag == TRUE ) AddExStyle |= WS_EX_LAYERED ;

				AddStyle = 0 ;
				// �񓯊��E�C���h�E���[�h�ύX�@�\���L���ɂȂ��Ă���Ƃ��� WS_MAXIMIZEBOX ��ǉ�����
				if( WinData.UseChangeWindowModeFlag == TRUE ) AddStyle |= WS_MAXIMIZEBOX ;

				// �\���t���O�������Ă���ꍇ�� WS_VISIBLE ��ǉ�����
				if( WinData.VisibleFlag             == TRUE ) AddStyle |= WS_VISIBLE ;

				// �E�C���h�E�T�C�Y���ύX�ł���悤�ɂȂ��Ă���ꍇ�� WS_THICKFRAME ��ǉ�����
				if( WinData.WindowSizeChangeEnable  == TRUE ) AddStyle |= WS_THICKFRAME ;

				// �}�E�X�\���t���O�𗧂Ă�
				WinData.MouseDispFlag = TRUE ;

				// �N���C�A���g�̈�̎Z�o
				if( WinData.WindowSizeValid == TRUE )
				{
					if( WinData.ScreenNotFitWindowSize )
					{
						// �t�B�b�g�w�肪�����ꍇ�͂��̂܂܂̃E�C���h�E�T�C�Y���Z�b�g
						WindowSizeX = WinData.WindowWidth ;
						WindowSizeY = WinData.WindowHeight ;
					}
					else
					{
						// �t�B�b�g�w�肪����ꍇ�͊g�嗦���w�肷��
						ExtendRateX = ( double )WinData.WindowWidth / WindowSizeX ;
						ExtendRateY = ( double )WinData.WindowHeight / WindowSizeY ;
						NS_SetWindowSizeExtendRate( ExtendRateX, ExtendRateY ) ;

						NS_GetWindowSizeExtendRate( &ExtendRateX, &ExtendRateY ) ;
						WindowSizeX = _DTOL( WindowSizeX * ExtendRateX ) ;
						WindowSizeY = _DTOL( WindowSizeY * ExtendRateY ) ;
					}
				}
				else
				{
					NS_GetWindowSizeExtendRate( &ExtendRateX, &ExtendRateY ) ;
					WindowSizeX = _DTOL( WindowSizeX * ExtendRateX ) ;
					WindowSizeY = _DTOL( WindowSizeY * ExtendRateY ) ;
				}

				// �V�X�e���o�[���܂߂��E�C���h�E�̃T�C�Y���擾
				SETRECT( Rect, 0, 0, WindowSizeX, WindowSizeY ) ;
				AdjustWindowRectEx( &Rect ,
									WStyle_WindowModeTable[WinData.WindowStyle]   + AddStyle, FALSE,
									WExStyle_WindowModeTable[WinData.WindowStyle] + AddExStyle );
				WinData.SystembarHeight = -Rect.top ;

				// �ݒ�̈�����傢�ƍ׍H
		//		Rect.left   += - 3;
		//		Rect.top    += - 3;
		//		Rect.right  += + 3;
		//		Rect.bottom += + 3;

				// ���S�Ɏ����Ă���
				NS_GetDefaultState( &DesktopW, &DesktopH, NULL, NULL, &DesktopX, &DesktopY ) ;
				WindowSizeX = Rect.right  - Rect.left ;
				WindowSizeY = Rect.bottom - Rect.top  ;
				Rect.left   += ( DesktopW - WindowSizeX ) / 2 + DesktopX ;
				Rect.top    += ( DesktopH - WindowSizeY ) / 2 + DesktopY ;
				Rect.right  += ( DesktopW - WindowSizeX ) / 2 + DesktopX ;
				Rect.bottom += ( DesktopH - WindowSizeY ) / 2 + DesktopY ;

				// �E�C���h�E�̍쐬
				WinData.MainWindow = 
					WinAPIData.Win32Func.CreateWindowExWFunc(
						WExStyle_WindowModeTable[WinData.WindowStyle] + AddExStyle,
						WinData.ClassName ,
						WinData.WindowText,
						WStyle_WindowModeTable[WinData.WindowStyle] + AddStyle,
						WinData.WindowPosValid == TRUE ? WinData.WindowX : Rect.left,
						WinData.WindowPosValid == TRUE ? WinData.WindowY : Rect.top,
						WindowSizeX,
						WindowSizeY,
						NULL,WinData.Menu,
						WinData.Instance,
						NULL ) ;
			}
#ifndef DX_NON_GRAPHICS
			else
			{
				GRAPHICSSYS_DISPLAYINFO *DisplayInfo ;

				DXST_ERRORLOG_ADDUTF16LE( "\xd5\x30\xeb\x30\xb9\x30\xaf\x30\xea\x30\xfc\x30\xf3\x30\xe2\x30\xfc\x30\xc9\x30\x28\x75\x6e\x30\xa6\x30\xa4\x30\xf3\x30\xc9\x30\xa6\x30\x92\x30\x5c\x4f\x10\x62\x57\x30\x7e\x30\x59\x30\x0a\x00\x00"/*@ L"�t���X�N���[�����[�h�p�̃E�C���h�E���쐬���܂�\n" @*/ ) ;
				// �t���X�N���[�����[�h��

				DisplayInfo = &GSYS.Screen.DisplayInfo[ GSYS.Screen.ValidUseDisplayIndex ? GSYS.Screen.UseDisplayIndex : 0 ] ;

				// �}�E�X�\���t���O��|��
				WinData.MouseDispFlag = FALSE ;

				WinData.MainWindow = 
					WinAPIData.Win32Func.CreateWindowExWFunc( 
						WExStyle_FullScreenModeTable[WinData.WindowStyle] ,
						WinData.ClassName ,
						WinData.WindowText ,
						WStyle_FullScreenModeTable[WinData.WindowStyle] ,
						DisplayInfo->DesktopRect.left,	             DisplayInfo->DesktopRect.top,
						DisplayInfo->DesktopRect.left + WindowSizeX, DisplayInfo->DesktopRect.top + WindowSizeY ,
						NULL ,
						NULL , 
						WinData.Instance ,
						NULL ) ;
			}
#endif // DX_NON_GRAPHICS
			if( WinData.MainWindow == NULL )
			{
				WinData.QuitMessageFlag = TRUE;
				DxLib_EndRequest() ;
				return DXST_ERRORLOG_ADDUTF16LE( "\xa6\x30\xa4\x30\xf3\x30\xc9\x30\xa6\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�E�C���h�E�̍쐬�Ɏ��s���܂���\n" @*/ ) ;
			}
			DXST_ERRORLOG_ADDUTF16LE( "\xa6\x30\xa4\x30\xf3\x30\xc9\x30\xa6\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x10\x62\x9f\x52\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�E�C���h�E�̍쐬�ɐ������܂���\n" @*/ ) ;

			// �E�C���h�E�̕\���A�X�V
			if( WinData.NotWindowVisibleFlag )
			{
				ShowWindow( WinData.MainWindow , SW_HIDE ) ;
			}
			else
			if( WinData.WindowMinimizeFlag )
			{
				ShowWindow( WinData.MainWindow , SW_MINIMIZE ) ;
				UpdateWindow( WinData.MainWindow ) ;
			}
			else
			{
				DXST_ERRORLOG_ADDUTF16LE( "\xa6\x30\xa4\x30\xf3\x30\xc9\x30\xa6\x30\x92\x30\x68\x88\x3a\x79\x57\x30\x7e\x30\x59\x30\x0a\x00\x00"/*@ L"�E�C���h�E��\�����܂�\n" @*/ ) ;
				ShowWindow( WinData.MainWindow , SW_SHOW ) ;
				UpdateWindow( WinData.MainWindow ) ;
			}

			// �E�C���h�E���^�b�`�Ή��ɓo�^����
			if( WinAPIData.Win32Func.RegisterTouchWindowFunc != NULL )
			{
				WinAPIData.Win32Func.RegisterTouchWindowFunc( WinData.MainWindow,  0 /* TWF_WANTPALM */ ) ;
			}
		}

		// �e��f�[�^���Z�b�g����
		{
			WinData.CloseMessagePostFlag = FALSE ;							// WM_CLOSE���b�Z�[�W��������Ƃs�q�t�d�ɂȂ�t���O�ϐ����e�`�k�r�d�ɂ��Ēu��
			WinData.DestroyMessageCatchFlag = FALSE ;						// WM_DESTROY���b�Z�[�W���󂯎�������ɂs�q�t�d�ɂȂ�t���O�ϐ����e�`�k�r�d�ɂ��Ă���
			WinData.ActiveFlag = TRUE ;										// �A�N�e�B�u�t���O�����Ă�
			if( WinData.WindowModeFlag == FALSE )
			{
				DXST_ERRORLOG_ADDUTF16LE( "\xab\x30\xfc\x30\xbd\x30\xeb\x30\x92\x30\x0d\x4e\xef\x53\x96\x89\x6b\x30\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�J�[�\����s���ɂ��܂���\n" @*/ ) ;
				SetCursor( NULL ) ;										// �t���X�N���[�����[�h�������ꍇ�J�[�\��������
			}
			DXST_ERRORLOG_ADDUTF16LE( "\x29\xff\x2d\xff\x25\xff\x92\x30\x21\x71\xb9\x52\x6b\x30\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�h�l�d�𖳌��ɂ��܂���\n" @*/ ) ;

			if( WinData.WindowMinimizeFlag == FALSE 
#ifndef DX_NON_INPUTSTRING
				&& CharBuf.IMEUseFlag == FALSE
#endif // DX_NON_INPUTSTRING
			)
			{
				if( WinAPIData.Win32Func.WINNLSEnableIME_Func )
					WinAPIData.Win32Func.WINNLSEnableIME_Func( WinData.MainWindow , FALSE ) ;						// MS_IME����
			}
			else
			{
#ifndef DX_NON_INPUTSTRING
				if( CharBuf.IMEUseFlag )
				{
					if( WinAPIData.Win32Func.WINNLSEnableIME_Func )
						WinAPIData.Win32Func.WINNLSEnableIME_Func( WinData.MainWindow , TRUE ) ;						// MS_IME����
				}
#endif // DX_NON_INPUTSTRING
			}

			// �E�C���h�E�̃N���C�A���g�̈��ۑ�����
			if( WinData.WindowModeFlag )
			{
				GetClientRect( GetDisplayWindowHandle() , &WinData.WindowRect )  ;
				ClientToScreen( GetDisplayWindowHandle() , ( LPPOINT )&WinData.WindowRect ) ;
				ClientToScreen( GetDisplayWindowHandle() , ( LPPOINT )&WinData.WindowRect + 1 ) ;
			}
		}

		// �}�E�X�̕\���ݒ�𔽉f������
		NS_SetMouseDispFlag( WinData.MouseDispFlag ) ;

		// �����I�����C�t���O��|��
		WinData.AltF4_EndFlag = FALSE ;

		if( WinData.UserWindowFlag == FALSE )
		{
			// ���C���E�C���h�E���A�N�e�B�u�ɂ���
			if( WinData.NotWindowVisibleFlag == FALSE &&
				WinData.WindowMinimizeFlag == FALSE )
				BringWindowToTop( WinData.MainWindow ) ;

			// ���������҂�
/*
			{
				int Time = NS_GetNowCount() ;
				while( NS_GetNowCount() - Time < 200 )
					NS_ProcessMessage() ;
			}
*/
			// �T�C�Y�␳����
			if( WinData.NotWindowVisibleFlag == FALSE &&
				WinData.WindowMinimizeFlag == FALSE )
			{
				WM_SIZEProcess() ;
			}

			// �E�C���h�E�쐬���t���O��|��
			WinData.WindowCreateFlag = FALSE ;
		}

		// �����T�C�Y�w��͊������Ă���̂Ńt���O��|��
		WinData.WindowPosValid = FALSE ;
	}

#ifdef DX_THREAD_SAFE
	// ProcessMessage ���Ђ�����ĂтÂ���X���b�h�𗧂Ă�
	WinData.ProcessMessageThreadHandle = CreateThread( NULL, 0, ProcessMessageThreadFunction, NULL, 0, &WinData.ProcessMessageThreadID ) ;
//	WinData.ProcessMessageThreadHandle = NULL ;
	if( WinData.ProcessMessageThreadHandle == NULL )
	{
		DXST_ERRORLOG_ADDUTF16LE( "\x50\x00\x72\x00\x6f\x00\x63\x00\x65\x00\x73\x00\x73\x00\x4d\x00\x65\x00\x73\x00\x73\x00\x61\x00\x67\x00\x65\x00\x20\x00\x92\x30\x72\x30\x5f\x30\x59\x30\x89\x30\x7c\x54\x73\x30\x64\x30\x65\x30\x51\x30\x8b\x30\xb9\x30\xec\x30\xc3\x30\xc9\x30\x6e\x30\xcb\x7a\x61\x30\x0a\x4e\x52\x30\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"ProcessMessage ���Ђ�����ĂтÂ���X���b�h�̗����グ�Ɏ��s���܂���\n" @*/ ) ;
	}
#endif

#ifndef DX_NON_ASYNCLOAD
	// �񓯊��ǂݍ��ݏ������s���X���b�h�𗧂Ă�
	if( SetupASyncLoadThread( WinData.ProcessorNum ) < 0 )
	{
		DXST_ERRORLOG_ADDUTF16LE( "\x5e\x97\x0c\x54\x1f\x67\xad\x8a\x7f\x30\xbc\x8f\x7f\x30\xe6\x51\x06\x74\x92\x30\x4c\x88\x46\x30\xb9\x30\xec\x30\xc3\x30\xc9\x30\x6e\x30\xcb\x7a\x61\x30\x0a\x4e\x52\x30\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�񓯊��ǂݍ��ݏ������s���X���b�h�̗����グ�Ɏ��s���܂���\n" @*/ ) ;
		WinData.QuitMessageFlag = TRUE;
		DxLib_EndRequest() ;
		return -1 ;
	}
#endif // DX_NON_ASYNCLOAD

	// �I��
	return 0 ;
}

#pragma optimize("", on)

// �E�C���h�E�֌W�̏����I���֐�
extern int TerminateWindow( void )
{
	// �������ɃE�C���h�E�������ꍇ�͉��������I��
	if( WinData.MainWindow == NULL ) return 0 ;

	// �X�N���[���Z�[�o�[�L��
	SystemParametersInfoW( SPI_SETSCREENSAVEACTIVE, TRUE, NULL, SPIF_SENDWININICHANGE ) ;

	// �E�C���h�E�Ɋ֘A���鏈���� DxSysData.NotWinFlag �������Ă��Ȃ��ꍇ�̂ݎ��s����
	if( DxSysData.NotWinFlag == FALSE )
	{
		// MS_IME�L��
		if( WinAPIData.Win32Func.WINNLSEnableIME_Func )
			WinAPIData.Win32Func.WINNLSEnableIME_Func( WinData.MainWindow, TRUE ) ;

		// �}�E�X�J�[�\����\������
		NS_SetMouseDispFlag( TRUE ) ;

		// �^�X�N�X�C�b�`�̗L���A�������Z�b�g����
		{
	/*		if( WinData.WindowsVersion < DX_WINDOWSVERSION_NT31 )
			{
				// Win95 �J�[�l���̏ꍇ�̏���
				UINT nPreviousState;
				if( WinData.SysCommandOffFlag == TRUE )
					SystemParametersInfoW( SPI_SETSCREENSAVERRUNNING, FALSE, &nPreviousState, 0 ) ;
			}
			else
	*/		{
				// WinNT �J�[�l���̏ꍇ�̏���

				// �L�[�{�[�h�t�b�N�𖳌���
				if( WinData.TaskHookHandle != NULL )
				{
					UnhookWindowsHookEx( WinData.TaskHookHandle ) ;
					WinData.TaskHookHandle = NULL ;
				}

				// ���b�Z�[�W�t�b�N�𖳌���
//				if( WinData.GetMessageHookHandle != NULL )
				{
					if( WinData.MessageHookDLL != NULL )
					{
						WinData.MessageHookCallBadk( WinData.MainWindow, &WinData.KeyboardHookHandle, FALSE ) ;
//						UnhookWindowsHookEx( WinData.GetMessageHookHandle ) ;
//						UnhookWindowsHookEx( WinData.KeyboardHookHandle ) ;
						FreeLibrary( WinData.MessageHookDLL ) ;
						WinData.MessageHookCallBadk = NULL ;
						WinData.MessageHookDLL = NULL ;
//						WinData.GetMessageHookHandle = NULL ;
						WinData.KeyboardHookHandle = NULL ;
					}
				}

				// �L�[�{�[�h�t�b�N�c�k�k�Ƃ��ăe���|�����t�@�C�����g�p���Ă����ꍇ�̓t�@�C�����폜����
				if( WinData.NotUseUserHookDllFlag )
				{
					DeleteFileW( WinData.HookDLLFilePath ) ;
					WinData.NotUseUserHookDllFlag = FALSE ;
				}
			}
		}

		// ���[�W�������폜����
		NS_SetWindowRgnGraph( NULL ) ;

		// ���j���[�𖳌��ɂ���
		NS_SetUseMenuFlag( FALSE ) ;

		// �c�[���o�[�𖳌��ɂ���
		NS_SetupToolBar( NULL, 0 ) ;

		// ���[�U�[�̃E�C���h�E���g�p���Ă��邩�ǂ����ŏ����𕪊�
		if( WinData.UserWindowFlag == TRUE )
		{
			// �E�C���h�E�v���[�W�������ɖ߂�
#ifdef _WIN64
			SetWindowLongPtrW( WinData.MainWindow, GWLP_WNDPROC, ( LONG_PTR )WinData.DefaultUserWindowProc ) ;
#else
			SetWindowLongW( WinData.MainWindow, GWL_WNDPROC, (LONG)WinData.DefaultUserWindowProc ) ;
#endif

			// �I����Ԃɂ���
			WinData.QuitMessageFlag = TRUE ;
			DxLib_EndRequest() ;
		}
		else
		{
			// �N���[�Y�t���O���|��Ă�����WM_CLOSE���b�Z�[�W�𑗂�
			if( WinData.CloseMessagePostFlag == FALSE )
			{
				WinData.CloseMessagePostFlag = TRUE ;
				PostMessageW( WinData.MainWindow , WM_CLOSE, 0, 0 );
			}

			// �G���h�������I��܂Ń��[�v
			while( NS_ProcessMessage() == 0 && WinData.DestroyMessageCatchFlag == FALSE ){}

			// ���W�X�g���̍폜
			UnregisterClassW( WinData.ClassName, WinData.Instance ) ;
		}

	#ifndef DX_NON_KEYEX
		// �L�[���̓n���h���̏�����
		NS_InitKeyInput() ;
	#endif

	/*
		// �w�i�p�a�l�o�I�u�W�F�N�g�̍폜
		if( WinData.PauseGraph )
		{
			DeleteObject( ( HGDIOBJ )WinData.PauseGraph ) ;
			WinData.PauseGraph = NULL ;
		}o
	*/
		// �w�i�p�a�l�o�I�u�W�F�N�g�̍폜
		if( WinData.PauseGraph.GraphData != NULL )
		{
			NS_ReleaseGraphImage( &WinData.PauseGraph ) ;
#ifndef DX_NON_GRAPHICS
			NS_DeleteGraph( WinData.PauseGraphHandle ) ;
#endif // DX_NON_GRAPHICS
			WinData.PauseGraphHandle = 0 ;
		}

		// �h���b�O���h���b�v���ꂽ�t�@�C�����̃o�b�t�@�����
		NS_DragFileInfoClear() ;
	}
	else
	{
		WinData.QuitMessageFlag = TRUE;
		DxLib_EndRequest() ;
	}

#ifdef DX_THREAD_SAFE

	// ProcessMessage ���Ђ�����ĂтÂ���X���b�h�����
	if( WinData.ProcessMessageThreadHandle != NULL )
	{
		// �X���b�h���I������܂ő҂�
		while( WinData.ProcessMessageThreadExitFlag == 0 )
		{
			Sleep( 1 ) ;
		}

		// �X���b�h�̃n���h�������
		CloseHandle( WinData.ProcessMessageThreadHandle ) ;
	}

#endif

#ifndef DX_NON_ASYNCLOAD
	// �񓯊��ǂݍ��ݏ����p�̃X���b�h�����
	CloseASyncLoadThread() ;
#endif // DX_NON_ASYNCLOAD

	// �t�@�C���A�N�Z�X�����̌�n��
	TerminateFile() ;

#ifndef DX_NON_ASYNCLOAD
	// �񓯊��ǂݍ��ݏ����̌�n��
	TerminateASyncLoad() ;
#endif // DX_NON_ASYNCLOAD
	
	// �^�C�}�̐��x�����ɖ߂�
	{
		TIMECAPS tc ;

		WinAPIData.Win32Func.timeGetDevCapsFunc( &tc , sizeof(TIMECAPS) );

		// �}���`���f�B�A�^�C�}�[�̃T�[�r�X���x���ő��
		WinAPIData.Win32Func.timeEndPeriodFunc( tc.wPeriodMin ) ;
	}

	// �o�b�N�o�b�t�@�̓��ߐF�̕����𓧉߂����邽�߂̃r�b�g�}�b�v������ꍇ�͍폜����
	if( WinData.BackBufferTransBitmap )
	{
		DeleteObject( WinData.BackBufferTransBitmap ) ;
		WinData.BackBufferTransBitmap = NULL ;
	}

	// ���C���E�C���h�E�n���h���Ƀk�����Z�b�g����
	//WinData.MainWindow = NULL ;

	// ���������_�̌v�Z���x�����ɖ߂�
//	_control87( 0x00000000, 0x00030000 );

	// �I��
	return 0 ;
}







// �b�n�l�������A�I���֌W�֐�

// �b�n�l�C���^�[�t�F�[�X������������
extern	int	InitializeCom( void )
{
	if( WinData.ComInitializeFlag ) return -1 ;

	DXST_ERRORLOG_ADDUTF16LE( "\x23\xff\x2f\xff\x2d\xff\x6e\x30\x1d\x52\x1f\x67\x16\x53\x2e\x00\x2e\x00\x2e\x00\x20\x00\x00"/*@ L"�b�n�l�̏�����... " @*/ ) ;

	// �b�n�l�̏�����
	if( FAILED( WinAPIData.Win32Func.CoInitializeExFunc( NULL, COINIT_APARTMENTTHREADED ) ) )
		return DXST_ERRORLOG_ADDUTF16LE( "\x23\xff\x2f\xff\x2d\xff\x6e\x30\x1d\x52\x1f\x67\x16\x53\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�b�n�l�̏������Ɏ��s���܂���\n" @*/ ) ;

	DXST_ERRORLOG_ADDUTF16LE( "\x10\x62\x9f\x52\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�������܂���\n" @*/ ) ;

	WinData.ComInitializeFlag = TRUE ;

	// �I��
	return 0 ;
}

// �b�n�l�C���^�[�t�F�[�X���I������
extern	int	TerminateCom( void )
{
	if( WinData.ComInitializeFlag == FALSE ) return -1 ;
	
	// �b�n�l�̏I��
	WinAPIData.Win32Func.CoUninitializeFunc () ;

	DXST_ERRORLOG_ADDUTF16LE( "\x23\xff\x2f\xff\x2d\xff\x92\x30\x42\x7d\x86\x4e\x2e\x00\x2e\x00\x2e\x00\x20\x00\x8c\x5b\x86\x4e\x0a\x00\x00"/*@ L"�b�n�l���I��... ����\n" @*/ ) ;

	WinData.ComInitializeFlag = FALSE ;

	// �I��
	return 0 ;
}































































// �֗��֐�

// �w��̃��\�[�X���擾����( -1:���s  0:���� )
extern int NS_GetResourceInfo( const TCHAR *ResourceName, const TCHAR *ResourceType, void **DataPointerP, int *DataSizeP )
{
#ifdef UNICODE
	return GetResourceInfo_WCHAR_T( ResourceName, ResourceType, DataPointerP, DataSizeP ) ;
#else
	int Result = -1 ;

	int IntResourceName ;
	int IntResourceType ;
	TCHAR_TO_WCHAR_T_STRING_BEGIN( ResourceName )
	TCHAR_TO_WCHAR_T_STRING_BEGIN( ResourceType )

	IntResourceName = ( ( ( DWORD_PTR )ResourceName ) >> 16 ) == 0 ? TRUE : FALSE ;
	IntResourceType = ( ( ( DWORD_PTR )ResourceType ) >> 16 ) == 0 ? TRUE : FALSE ;

	if( IntResourceName )
	{
		UseResourceNameBuffer = ( wchar_t * )ResourceName ;
	}
	else
	{
		TCHAR_TO_WCHAR_T_STRING_SETUP( ResourceName, goto ERR )
	}

	if( IntResourceType )
	{
		UseResourceTypeBuffer = ( wchar_t * )ResourceType ;
	}
	else
	{
		TCHAR_TO_WCHAR_T_STRING_SETUP( ResourceType, goto ERR )
	}

	Result = GetResourceInfo_WCHAR_T( UseResourceNameBuffer, UseResourceTypeBuffer, DataPointerP, DataSizeP ) ;

ERR :

	if( IntResourceName == FALSE )
	{
		TCHAR_TO_WCHAR_T_STRING_END( ResourceName )
	}

	if( IntResourceType == FALSE )
	{
		TCHAR_TO_WCHAR_T_STRING_END( ResourceType )
	}

	return Result ;
#endif
}

// �w��̃��\�[�X���擾����( -1:���s  0:���� )
extern int GetResourceInfo_WCHAR_T( const wchar_t *ResourceName, const wchar_t *ResourceType, void **DataPointerP, int *DataSizeP )
{
	HRSRC   ResourceHandle = NULL ;
	HGLOBAL ResourceMem    = NULL ;
	LPVOID  ResourceData   = NULL ;
	DWORD   ResourceSize   = 0 ;
	HMODULE ResourceModule ;

	// ���\�[�X�n���h�����擾����
	ResourceModule = WinData.LoadResourModule == NULL ? WinAPIData.Win32Func.GetModuleHandleWFunc( NULL ) : WinData.LoadResourModule ;
	ResourceHandle = FindResourceW( ResourceModule, ResourceName, ResourceType ) ;
	if( ResourceHandle == NULL )
	{
		return -1 ;
	}

	// ���\�[�X�̃T�C�Y���擾����
	ResourceSize = SizeofResource( ResourceModule, ResourceHandle ) ;
	if( ResourceSize == 0 )
	{
		return -1 ;
	}

	// ���\�[�X���������ɓǂݍ���
	ResourceMem = LoadResource( ResourceModule, ResourceHandle ) ;
	if( ResourceMem == NULL )
	{
		return -1 ;
	}

	// ���\�[�X�f�[�^�����݂��郁�������擾����
	ResourceData = LockResource( ResourceMem ) ;
	if( ResourceData == NULL )
	{
		return -1 ;
	}

	// �擾����������������
	if( DataPointerP != NULL )
	{
		*DataPointerP = ResourceData ;
	}

	if( DataSizeP != NULL )
	{
		*DataSizeP = ( int )ResourceSize ;
	}

	// �I��
	return 0 ;
}

// ���\�[�X�h�c���烊�\�[�X�h�c������𓾂� 
extern const TCHAR *NS_GetResourceIDString( int ResourceID )
{
	return MAKEINTRESOURCE( ResourceID ) ;
}

#if defined( DX_THREAD_SAFE ) || defined( DX_THREAD_SAFE_NETWORK_ONLY )

// �����ɂc�w���C�u�����̊֐����Ă΂�Ă��Ȃ����`�F�b�N���āA�����ɌĂ΂�Ă�����҂֐�
extern void CheckConflictAndWaitDxFunction( void )
{
//	int Time ;
//	int num ;
	DWORD ThreadID ;
	HANDLE EventHandle ;

	// DxConflictWaitThreadID ������������Ă��Ȃ������珉��������
	if( WinData.DxConflictWaitThreadIDInitializeFlag == FALSE )
	{
		int i ;

		// �������t���O�𗧂Ă�
		WinData.DxConflictWaitThreadIDInitializeFlag = TRUE ;

		// �Փˎ��Ɏg�p����C�x���g�̍쐬
		for( i = 0 ; i < MAX_THREADWAIT_NUM ; i ++ )
		{
			WinData.DxConflictWaitThreadID[ i ][ 1 ] = ( DWORD_PTR )CreateEvent( NULL, TRUE, FALSE, NULL ) ;
		}

		// �N���e�B�J���Z�N�V����������������
		CriticalSection_Initialize( &WinData.DxConflictCheckCriticalSection ) ;
	}

	// ���̊֐����Ăяo�����X���b�h�̂h�c�𓾂�
	ThreadID = GetCurrentThreadId() ;

START :

	// ���̊֐����g�p���̏ꍇ�͑҂�
	CRITICALSECTION_LOCK( &WinData.DxConflictCheckCriticalSection ) ;
//	CheckConflict( &WinData.DxConflictCheckFlag ) ;

	// �ǂ̃X���b�h���c�w���C�u�����̊֐����g�p���Ă��Ȃ����A
	// �������͎g�p���Ă��Ă����ꂪ�����̃X���b�h��������ʉ�
	if( WinData.DxUseThreadFlag == FALSE || WinData.DxUseThreadID == ThreadID )
	{
		// �҂��Ă���X���b�h�����āA���ꂪ�����̃X���b�h�ł͂Ȃ��ꍇ�͑҂�
		if( WinData.DxUseThreadFlag == FALSE && WinData.DxConflictWaitThreadNum != 0 && WinData.DxUseThreadID != ThreadID )
		{
			if( WinData.DxConflictWaitThreadID[ 0 ][ 0 ] != ThreadID )
			{
				goto WAIT ;
			}
			else
			{
				// �����҂��Ă����̂������������烊�X�g���X���C�h������
				if( WinData.DxConflictWaitThreadNum != 1 )
				{
					EventHandle = ( HANDLE )WinData.DxConflictWaitThreadID[ 0 ][ 1 ] ;
					_MEMCPY( &WinData.DxConflictWaitThreadID[ 0 ][ 0 ], &WinData.DxConflictWaitThreadID[ 1 ][ 0 ], sizeof( DWORD_PTR ) * 2 * ( WinData.DxConflictWaitThreadNum - 1 ) ) ;
					WinData.DxConflictWaitThreadID[ WinData.DxConflictWaitThreadNum - 1 ][ 1 ] = ( DWORD_PTR )EventHandle ;
					WinData.DxConflictWaitThreadID[ WinData.DxConflictWaitThreadNum - 1 ][ 0 ] = ( DWORD_PTR )0 ;
				}

				// �҂��Ă��鐔�����炷
				WinData.DxConflictWaitThreadNum -- ;
			}
		}

		// �����Z�b�g
		WinData.DxUseThreadFlag = TRUE ;
		WinData.DxUseThreadID = ThreadID ;

		// �g�p���J�E���^���C���N�������g����
//		if( WinData.DxConflictCheckCounter != 0 )
//		{
//			DXST_ERRORLOG_ADDW( L"�G���[:�Փ˔��� No.0 \n" ) ;
//		}
		WinData.DxConflictCheckCounter ++ ;

		goto END ;
	}

	// ��������Ȃ��ꍇ�͑҂�
WAIT:

	// �����҂��Ă���X���b�h�����E���z���Ă�����P���ȑ҂��������s��
	if( WinData.DxConflictWaitThreadNum == MAX_THREADWAIT_NUM )
	{
		// �Ƃ肠�������̊֐����g�p���A�t���O��|��
		CriticalSection_Unlock( &WinData.DxConflictCheckCriticalSection ) ;
//		WinData.DxConflictCheckFlag -- ;

//		DXST_ERRORLOG_ADDW( L"�G���[:�Փ˔��� No.4 \n" ) ;

		// �����Q��
		Sleep( 1 ) ;

		// �ŏ��ɖ߂�
		goto START ;
	}

	// �҂��Ă�X���b�h�����܂���Ƃ�������ǉ�����
	WinData.DxConflictWaitThreadID[WinData.DxConflictWaitThreadNum][0] = ThreadID ;
	EventHandle = (HANDLE)WinData.DxConflictWaitThreadID[WinData.DxConflictWaitThreadNum][1] ;
	WinData.DxConflictWaitThreadNum ++ ;

	// �Ƃ肠�������̊֐����g�p���A�t���O��|��
	CriticalSection_Unlock( &WinData.DxConflictCheckCriticalSection ) ;
//	WinData.DxConflictCheckFlag -- ;

	// �����̔Ԃ�����܂ő҂�
	WaitForSingleObject( EventHandle, INFINITE ) ;
	ResetEvent( EventHandle ) ;
/*
	Time = WinAPIData.Win32Func.timeGetTimeFunc() ;
//	while( WinData.DxUseThreadFlag == TRUE )
	while( WinData.DxUseThreadFlag == TRUE || WinData.DxConflictWaitThreadID[0] != ThreadID )
	{
		Sleep( 0 ) ;
		if( WinAPIData.Win32Func.timeGetTimeFunc() - Time > 3000 )
		{
			HANDLE fp ;
			DWORD WriteSize ;
//			const char *ErrorStr = "�����Ԓ�~�����ςȂ��ł�\n" ;
			char ErrorStr[128] ;
			char String[256] ;
			Time = WinAPIData.Win32Func.timeGetTimeFunc() ;
			
			_SNPRINTF( ErrorStr, sizeof( ErrorStr ), "�����Ԓ�~�����ςȂ��ł� con:%d consub:%d\n", WinData.DxConflictCheckCounter, WinData.DxConflictCheckCounterSub ) ;
			
			// ���O�o�͗}���t���O�������Ă����ꍇ�͏o�͂��s��Ȃ�
			if( DxSysData.NotLogOutFlag == FALSE && WinData.LogOutDirectory[0] != '\0' )
			{
				char MotoPath[FILEPATH_MAX] ;

				// �G���[���O�t�@�C�����J��
				DX_FGETDIR( MotoPath, sizeof( MotoPath ) ) ;
				DX_FSETDIR( WinData.LogOutDirectory ) ;
				fp = CreateFile( WinData.LogFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL ) ;
				if( fp != NULL )
				{
					SetFilePointer( fp, 0, NULL, FILE_END ) ;

					// �K�蕪�����^�u��r�o
					if( DxSysData.ErTabStop == FALSE )
					{
						int i ;	

						// �^�C���X�^���v���o��
						if( WinData.NonUseTimeStampFlag == 0 )
						{
							_SNPRINTF( String, sizeof( String ), "%d:", WinAPIData.Win32Func.timeGetTimeFunc() - DxSysData.LogStartTime ) ;
							WriteFile( fp, String, lstrlenA( String ), &WriteSize, NULL ) ;
						}

						for( i = 0 ; i < DxSysData.ErTabNum ; i ++ )
							String[i] = '\t' ;
						String[i] = '\0' ;
						WriteFile( fp, String, DxSysData.ErTabNum, &WriteSize, NULL ) ;
						OutputDebugString( String ) ;
					}

					// �G���[���O�t�@�C���ɏ����o��
					WriteFile( fp, ErrorStr, lstrlenA( String ), &WriteSize, NULL ) ;
//					fputs( ErrorStr , fp ) ;

					// �G���[���O���A�E�g�v�b�g�ɏ����o��
					OutputDebugString( ErrorStr ) ;

					// �Ō�̕��������s�ӊO�������ꍇ�̓^�u�X�g�b�v�t���O�𗧂Ă�
					DxSysData.ErTabStop = ErrorStr[ lstrlenA( ErrorStr ) - 1 ] != '\n' ;

//					fclose( fp ) ;
					CloseHandle( fp ) ;
				}
				DX_FSETDIR( MotoPath ) ;
			}
		}
	}
*/
	
	// �҂��Ă�X���b�h�������J�E���^���f�N�������g����
//	WinData.DxConflictWaitThreadCounter -- ;
//	if( WinData.DxConflictWaitThreadCounter < 0 )
//	{
//		WinData.DxConflictWaitThreadCounter = 0 ;
//	}

	// �ŏ��ɖ߂�
	goto START ;

END :

	// �g�p���t���O��|��
	CriticalSection_Unlock( &WinData.DxConflictCheckCriticalSection ) ;
//	WinData.DxConflictCheckFlag -- ;
}
#endif // defined( DX_THREAD_SAFE ) || defined( DX_THREAD_SAFE_NETWORK_ONLY )


#if defined( DX_THREAD_SAFE ) || defined( DX_THREAD_SAFE_NETWORK_ONLY )

// CheckConfictAndWaitDxFunction ���g�p�����c�w���C�u�����̊֐��� return ����O�ɌĂԂׂ��֐�
extern void PostConflictProcessDxFunction( void )
{
	int WaitFlag = 0 ;
	
	// ���̊֐����g�p���̏ꍇ�͑҂�
	CRITICALSECTION_LOCK( &WinData.DxConflictCheckCriticalSection ) ;
//	CheckConflict( &WinData.DxConflictCheckFlag ) ;

	// �J�E���^���P�̏ꍇ�́A����łc�w���C�u�����g�p����Ԃ�
	// ��������邱�ƂɂȂ�̂ŁA�����𕪊�
	if( WinData.DxConflictCheckCounter == 1 )
	{
		// �������Z�b�g����
		WinData.DxUseThreadFlag = FALSE ;
		WinData.DxUseThreadID = 0xffffffff ;

		// �����҂��Ă���X���b�h������ꍇ�́A�X���[�v����t���O�𗧂Ă�
		if( WinData.DxConflictWaitThreadNum > 0 )
			WaitFlag = 1 ;
	}
//	else
//	{
//		DXST_ERRORLOG_ADDW( L"�G���[:�Փ˔��� No.1\n" ) ;
//	}

	// �J�E���^���f�N�������g���ďI��
	WinData.DxConflictCheckCounter -- ;

	// ���̊֐����g�p���A�t���O��|��
	CriticalSection_Unlock( &WinData.DxConflictCheckCriticalSection ) ;
//	WinData.DxConflictCheckFlag -- ;

	// �X���[�v����t���O�������Ă�����X���[�v����
	if( WaitFlag == 1 )
	{
//		DWORD ThreadID = GetCurrentThreadId() ;

		// ���Ɏ��s���ׂ��X���b�h�̃C�x���g���V�O�i����Ԃɂ���
		SetEvent( (HANDLE)WinData.DxConflictWaitThreadID[0][1] ) ;

		// �Q��
		Sleep( 0 ) ;

/*		// �҂��Ă�X���b�h�������J�E���^���O�ɂȂ�̂�҂�
		while( WinData.DxConflictWaitThreadNum > 0 )
		{
			Sleep( 0 ) ;
		}
		Sleep( 0 ) ;
*/
	}
}
#endif // defined( DX_THREAD_SAFE ) || defined( DX_THREAD_SAFE_NETWORK_ONLY )




















// ���b�Z�[�W�����֐�

// WM_PAINT�p�֐�
int DrawBackGraph( HDC /*DestDC*/ )
{
#ifndef DX_NON_GRAPHICS
	int DrawScreenWidth, DrawScreenHeight/*, DrawScreen, DrawMode, WaitVSync*/ ;
	int Width, Height, NonActiveRunFlag, BackUpScreen ;
	SCREENDRAWSETTINGINFO ScreenDrawSettingInfo ;

	if( WinData.AltF4_EndFlag == 1 ) return 0 ;

	if( WinData.PauseGraph.GraphData == NULL ) return -1 ;

	NonActiveRunFlag = WinData.NonActiveRunFlag ;
	WinData.NonActiveRunFlag = TRUE ;

	// �O���t�B�b�N�n���h���������ɂȂ��Ă������蒼��
//	if( Graphics_Image_GetData( WinData.PauseGraphHandle ) < 0 )
	if( Graphics_Image_IsValidHandle( WinData.PauseGraphHandle ) == FALSE )
	{
		WinData.PauseGraphHandle = Graphics_Image_CreateGraphFromGraphImageBase( &WinData.PauseGraph, NULL, TRUE, FALSE ) ;
		if( WinData.PauseGraphHandle < 0 )
		{
			WinData.NonActiveRunFlag = NonActiveRunFlag ;
			return -1 ;
		}
	}

	// DrawBackGraph �����s���t���O�𗧂Ă�
	WinData.DrawBackGraphFlag = TRUE ;

	// �e��`��ݒ�����擾
	Graphics_DrawSetting_GetScreenDrawSettingInfo( &ScreenDrawSettingInfo ) ;

	// �ݒ���ꎞ�I�ɕύX����
/*	DrawScreen = NS_GetActiveGraph() ;
	DrawMode = NS_GetDrawMode() ;
	WaitVSync = NS_GetWaitVSyncFlag() ;
*/	NS_SetDrawScreen( DX_SCREEN_BACK ) ;
	NS_SetDrawMode( DX_DRAWMODE_BILINEAR ) ;
	NS_SetWaitVSyncFlag( FALSE ) ;

	// ���݂̃E�C���h�E�̃N���C�A���g�̈�̃T�C�Y�𓾂�
	NS_GetDrawScreenSize( &DrawScreenWidth, &DrawScreenHeight ) ;

	// ��ʈꎟ�ۑ��p�摜�̍쐬
	{
		SETUP_GRAPHHANDLE_GPARAM GParam ;

		Graphics_Image_InitSetupGraphHandleGParam_Normal_NonDrawValid( &GParam, 32, FALSE, FALSE ) ;
		BackUpScreen = Graphics_Image_MakeGraph_UseGParam( &GParam, DrawScreenWidth, DrawScreenHeight, FALSE, FALSE, 0, FALSE ) ;
	}

	// ��ʉ摜�̎擾
	NS_GetDrawScreenGraph( 0, 0, DrawScreenWidth, DrawScreenHeight, BackUpScreen ) ;

	// �|�[�Y�摜�̃T�C�Y���擾����
	NS_GetGraphSize( WinData.PauseGraphHandle, &Width, &Height ) ;

	// ��ʈ�t�ɕ`�悷��
	if( Width == DrawScreenWidth && Height == DrawScreenHeight )
	{
		NS_DrawGraph( 0, 0, WinData.PauseGraphHandle, FALSE ) ;
	}
	else
	{
		NS_DrawExtendGraph( 0, 0, DrawScreenWidth, DrawScreenHeight, WinData.PauseGraphHandle, FALSE ) ;
	}

	// �\��ʂɃR�s�[
	NS_ScreenCopy() ;

	// ����ʂɂ��Ƃ̉摜��`�悷��
	NS_DrawGraph( 0, 0, BackUpScreen, FALSE ) ;

	// �ꎟ�ۑ��p�摜�̍폜
	NS_DeleteGraph( BackUpScreen ) ;

	// �e��`��ݒ�������ɖ߂�
	Graphics_DrawSetting_SetScreenDrawSettingInfo( &ScreenDrawSettingInfo ) ;

	// �`��ݒ�����ɖ߂�
/*	NS_SetDrawScreen( DrawScreen ) ;
	NS_SetDrawMode( DrawMode ) ;
	NS_SetWaitVSyncFlag( WaitVSync ) ;
*/	WinData.NonActiveRunFlag = NonActiveRunFlag ;

	// DrawBackGraph �����s���t���O��|��
	WinData.DrawBackGraphFlag = FALSE ;
#endif // DX_NON_GRAPHICS

	// �I��
	return 0 ;
}


// �E�C���h�E�֘A

// ���C���E�C���h�E�̃T�C�Y���擾����
static void	GetMainWindowSize( int *SizeX, int *SizeY )
{
#ifndef DX_NON_GRAPHICS
	int WindowSizeX ;
	int WindowSizeY ;

	if( WinData.WindowModeFlag )
	{
		if( GSYS.Screen.Emulation320x240Flag )
		{
			WindowSizeX = 640 ;
			WindowSizeY = 480 ;
		}
		else
		{
			NS_GetDrawScreenSize( &WindowSizeX, &WindowSizeY ) ;
		}
	}
	else
	{
		Graphics_Screen_SetupFullScreenModeInfo() ;
		WindowSizeX = GSYS.Screen.FullScreenUseDispModeData.Width ;
		WindowSizeY = GSYS.Screen.FullScreenUseDispModeData.Height ;
	}

	if( SizeX != NULL )
	{
		*SizeX = WindowSizeX ;
	}

	if( SizeY != NULL )
	{
		*SizeY = WindowSizeY ;
	}
#endif // DX_NON_GRAPHICS
}


// ���C���E�C���h�E�̃��b�Z�[�W�R�[���o�b�N�֐�
extern LRESULT CALLBACK DxLib_WinProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	int PrevWindowMinSizeFlag ;
	LRESULT Ret = -1000, UserProcRet = 0 ;

	// ���b�Z�[�W�o��
#if 0
	{
		const char *MessageName = NULL ;
		const char *SubName = NULL ;

		switch( message )
		{
		case WM_SETCURSOR :				MessageName = "WM_SETCURSOR" ;				break ;
		case WM_DROPFILES :				MessageName = "WM_DROPFILES" ;				break ;
		case WM_CREATE :				MessageName = "WM_CREATE" ;					break ;
		case WM_COMMAND :				MessageName = "WM_COMMAND" ;				break ;
		case WM_IME_SETCONTEXT :		MessageName = "WM_IME_SETCONTEXT" ;			break ;
		case WM_IME_STARTCOMPOSITION :	MessageName = "WM_IME_STARTCOMPOSITION" ;	break ;
		case WM_IME_ENDCOMPOSITION :	MessageName = "WM_IME_ENDCOMPOSITION" ;		break ;
		case WM_IME_COMPOSITION :		MessageName = "WM_IME_COMPOSITION" ;		break ;
		case WM_IME_NOTIFY :			MessageName = "WM_IME_NOTIFY" ;				break ;
		case WM_IME_REQUEST :			MessageName = "WM_IME_REQUEST" ;			break ;
		case WM_SIZING :				MessageName = "WM_SIZING" ;					break ;
		case WM_SIZE :					MessageName = "WM_SIZE" ;					break ;
		case WM_DISPLAYCHANGE :			MessageName = "WM_DISPLAYCHANGE" ;			break ;
		case WM_NCHITTEST :				goto MESNAMEEND ; MessageName = "WM_NCHITTEST" ;				break ;
		case WM_LBUTTONDOWN :			MessageName = "WM_LBUTTONDOWN" ;			break ;
		case WM_RBUTTONDOWN :			MessageName = "WM_RBUTTONDOWN" ;			break ;
		case WM_MBUTTONDOWN :			MessageName = "WM_MBUTTONDOWN" ;			break ;
		case WM_MOUSEMOVE :				goto MESNAMEEND ; MessageName = "WM_MOUSEMOVE" ;				break ;
		case WM_LBUTTONUP :				MessageName = "WM_LBUTTONUP" ;				break ;
		case WM_RBUTTONUP :				MessageName = "WM_RBUTTONUP" ;				break ;
		case WM_MBUTTONUP :				MessageName = "WM_MBUTTONUP" ;				break ;
		case WM_PAINT :					MessageName = "WM_PAINT" ;					break ;
		case WM_SYSCHAR :				MessageName = "WM_SYSCHAR" ;				break ;
		case WM_CHAR :					MessageName = "WM_CHAR" ;					break ;
		case F10MES :					MessageName = "F10MES" ;					break ;
		case F12MES :					MessageName = "F12MES" ;					break ;
		case WM_KEYDOWN :				MessageName = "WM_KEYDOWN" ;				break ;
		case WM_KEYUP :					MessageName = "WM_KEYUP" ;					break ;
		case WM_MOUSEWHEEL :			MessageName = "WM_MOUSEWHEEL" ;				break ;
		case WM_MOUSEHWHEEL :			MessageName = "WM_MOUSEHWHEEL" ;			break ;
		case WM_MOVE :					goto MESNAMEEND ; MessageName = "WM_MOVE" ;					break ;
		case WM_MOVING :				goto MESNAMEEND ; MessageName = "WM_MOVING" ;					break ;
		case WM_ACTIVATEAPP :			MessageName = "WM_ACTIVATEAPP" ;			break ;
		case WM_ACTIVATE :				MessageName = "WM_ACTIVATE" ;				break ;
		case WM_CLOSE :					MessageName = "WM_CLOSE" ;					break ;
		case WM_DESTROY :				MessageName = "WM_DESTROY" ;				break ;
		case WM_QUIT :					MessageName = "WM_QUIT" ;					break ;
		case MM_MCINOTIFY :				MessageName = "MM_MCINOTIFY" ;				break ;
		case WSA_WINSOCKMESSAGE :		MessageName = "WSA_WINSOCKMESSAGE" ;		break ;
		case WM_ENTERSIZEMOVE :			MessageName = "WM_ENTERSIZEMOVE" ;			break ;
		case WM_EXITSIZEMOVE :			MessageName = "WM_EXITSIZEMOVE" ;			break ;
		case WM_ENTERMENULOOP :			MessageName = "WM_ENTERMENULOOP" ;			break ;
		case WM_EXITMENULOOP :			MessageName = "WM_EXITMENULOOP" ;			break ;
		case WM_NCRBUTTONDOWN :			MessageName = "WM_NCRBUTTONDOWN" ;			break ;

		case WM_SYSCOMMAND :
			MessageName = "WM_SYSCOMMAND" ;

			switch( wParam & 0xfff0 )
			{
			case SC_SIZE			: SubName = "SC_SIZE" ;				break ;
			case SC_MOVE			: SubName = "SC_MOVE" ;				break ;
			case SC_MINIMIZE		: SubName = "SC_MINIMIZE" ;			break ;
			case SC_MAXIMIZE		: SubName = "SC_MAXIMIZE" ;			break ;
			case SC_NEXTWINDOW		: SubName = "SC_NEXTWINDOW" ;		break ;
			case SC_PREVWINDOW		: SubName = "SC_PREVWINDOW" ;		break ;
			case SC_CLOSE			: SubName = "SC_CLOSE" ;			break ;
			case SC_VSCROLL			: SubName = "SC_VSCROLL" ;			break ;
			case SC_HSCROLL			: SubName = "SC_HSCROLL" ;			break ;
			case SC_MOUSEMENU		: SubName = "SC_MOUSEMENU" ;		break ;
			case SC_KEYMENU			: SubName = "SC_KEYMENU" ;			break ;
			case SC_ARRANGE			: SubName = "SC_ARRANGE" ;			break ;
			case SC_RESTORE			: SubName = "SC_RESTORE" ;			break ;
			case SC_TASKLIST		: SubName = "SC_TASKLIST" ;			break ;
			case SC_SCREENSAVE		: SubName = "SC_SCREENSAVE" ;		break ;
			case SC_HOTKEY			: SubName = "SC_HOTKEY" ;			break ;
			case SC_DEFAULT			: SubName = "SC_DEFAULT" ;			break ;
			case SC_MONITORPOWER	: SubName = "SC_MONITORPOWER" ;		break ;
			case SC_CONTEXTHELP		: SubName = "SC_CONTEXTHELP" ;		break ;
			case SC_SEPARATOR		: SubName = "SC_SEPARATOR" ;		break ;
			}
			DXST_ERRORLOGFMT_ADDA(( "HWND:%08x	Message:%s	WParam:%08d:%08xh	LParam:%08d:%08xh", hWnd, MessageName, wParam, wParam, lParam, lParam ));
			DXST_ERRORLOGFMT_ADDA(( "%s uCmdType:%s		xPos:%-5d yPos:%-5d", MessageName, SubName, LOWORD( lParam ), HIWORD( lParam ) ));
			goto MESNAMEEND ;

		case WM_NCMOUSEMOVE :			goto MESNAMEEND ; MessageName = "@ WM_NCMOUSEMOVE" ;			break ;
		case WM_GETMINMAXINFO :			MessageName = "@ WM_GETMINMAXINFO" ;		break ;
		case WM_NCLBUTTONDOWN :			MessageName = "@ WM_NCLBUTTONDOWN" ;		break ;
		case WM_NCACTIVATE :			MessageName = "@ WM_NCACTIVATE" ;			break ;
		case WM_KILLFOCUS :				MessageName = "@ WM_KILLFOCUS" ;			break ;
		case WM_WINDOWPOSCHANGING :		goto MESNAMEEND ; MessageName = "@ WM_WINDOWPOSCHANGING" ;	break ;
		case WM_WINDOWPOSCHANGED :		goto MESNAMEEND ; MessageName = "@ WM_WINDOWPOSCHANGED" ;		break ;
		case WM_SETFOCUS :				MessageName = "@ WM_SETFOCUS" ;				break ;
		case WM_NCMOUSELEAVE :			MessageName = "@ WM_NCMOUSELEAVE" ;			break ;
		case WM_SYSKEYDOWN :			MessageName = "@ WM_SYSKEYDOWN" ;			break ;
		case WM_SYSKEYUP :				MessageName = "@ WM_SYSKEYUP" ;				break ;
		case WM_INITMENU :				MessageName = "@ WM_INITMENU" ;				break ;
		case WM_MENUSELECT :			MessageName = "@ WM_MENUSELECT" ;			break ;
		case WM_ENTERIDLE :				goto MESNAMEEND ; MessageName = "@ WM_ENTERIDLE" ;			break ;
		case WM_CAPTURECHANGED :		MessageName = "@ WM_CAPTURECHANGED" ;		break ;
		case WM_STYLECHANGING :			MessageName = "@ WM_STYLECHANGING" ;		break ;
		case WM_STYLECHANGED :			MessageName = "@ WM_STYLECHANGED" ;			break ;
		case WM_NCCALCSIZE :			MessageName = "@ WM_NCCALCSIZE" ;			break ;
		case WM_NCPAINT :				MessageName = "@ WM_NCPAINT" ;				break ;
		case WM_ERASEBKGND :			MessageName = "@ WM_ERASEBKGND" ;			break ;
		case WM_NCDESTROY :				MessageName = "@ WM_NCDESTROY" ;			break ;
		case WM_NCCREATE :				MessageName = "@ WM_NCCREATE" ;				break ;

		default :
			MessageName = NULL ;
			break ;
		}

		if( MessageName )
		{
			DXST_ERRORLOGFMT_ADDA(( "HWND:%08x	Message:%s	WParam:%08d:%08xh	LParam:%08d:%08xh", hWnd, MessageName, wParam, wParam, lParam, lParam ));
		}
		else
		{
			DXST_ERRORLOGFMT_ADDA(( "HWND:%08x	Message:%08d:%08xh	WParam:%08d:%08xh	LParam:%08d:%08xh", hWnd, message, message, wParam, wParam, lParam, lParam ));
		}
	}
MESNAMEEND:
#endif

	// ���[�U�[����񋟂��ꂽ�E�C���h�E���g�p���Ă���ꍇ�͂��̃E�C���h�E�̃v���V�[�W�����Ă�
	if( WinData.UserWindowFlag == TRUE )
	{
		if( WinData.DefaultUserWindowProc != NULL )
		{
			UserProcRet = WinData.DefaultUserWindowProc( hWnd, message, wParam, lParam ) ;
		}
	}

	// ���[�U�[��`�̃v���[�W���֐����������炻����Ă�
	if( WinData.UserWindowProc != NULL ) 
	{
		LRESULT RetValue ;

		WinData.UseUserWindowProcReturnValue = FALSE ;

		RetValue = WinData.UserWindowProc( hWnd, message, wParam, lParam ) ;

		// �t���O�������Ă����炱���ŏI��
		if( WinData.UseUserWindowProcReturnValue )
		{
			return RetValue ;
		}
	}
	
	switch( message )
	{
	case WM_DROPFILES :
		// �t�@�C�����h���b�O���h���b�v���ꂽ�ꍇ�̏���
		{
			DWORD FileNum ;
			DWORD i ;
			DWORD size ;
			HDROP hDrop = ( HDROP )wParam ;
			
			// �t�@�C���̐����擾
			FileNum = DragQueryFile( hDrop, 0xffffffff, NULL, 0 ) ;

			// �t�@�C���̐������J��Ԃ�
			for( i = 0 ; i < FileNum && WinData.DragFileNum < MAX_DRAGFILE_NUM ; i ++, WinData.DragFileNum ++ )
			{
				// �K�v�ȃo�b�t�@�̃T�C�Y���擾����
				size = DragQueryFile( hDrop, ( DWORD )i, NULL, 0 ) ;
				
				// �o�b�t�@�̊m��
				WinData.DragFileName[ WinData.DragFileNum ] = ( wchar_t * )DXALLOC( ( size + 1 ) * sizeof( wchar_t ) ) ;
				if( WinData.DragFileName[WinData.DragFileNum] == NULL ) break ;
				
				// �t�@�C�����̎擾
				DragQueryFileW( hDrop, i, WinData.DragFileName[WinData.DragFileNum], size + 1 ) ;
			}
			
			// �擾�̏I��
			DragFinish( hDrop ) ;
		}
		break ;
	
	case WM_CREATE :
		// �E�C���h�E���쐬���ꂽ����̎��̏���
		if( WinData.WindowRgn != NULL )
		{
			SetWindowRgn( hWnd, WinData.WindowRgn, FALSE ) ;
		}
		break ;

	case WM_COMMAND :
		// �c�[���o�[�̃R�}���h�����ׂ�
		if( WinData.ToolBarUseFlag == TRUE && LOWORD( wParam ) >= TOOLBAR_COMMANDID_BASE )
		{
			int Index ;
			WINTOOLBARITEMINFO *but ;

			Index = SearchToolBarButton( LOWORD( wParam ) - TOOLBAR_COMMANDID_BASE ) ;
			if( Index != -1 )
			{
				but = &WinData.ToolBarItem[ Index ] ;

				// �^�C�v�����ʂ̃{�^���̏ꍇ�̂� Clik �� TRUE �ɂ���
				if( but->Type == TOOLBUTTON_TYPE_NORMAL )
					but->Click = TRUE ;

				// �I��
				break ;
			}
		}

		// ���j���[�̃R�}���h�����ׂ�
		if( WinData.MenuUseFlag == TRUE /*&& HIWORD( wParam ) == 0*/ )
		{
			WORD ItemID ;

			ItemID = LOWORD( wParam ) ;
			
			// �R�[���o�b�N�֐����o�^����Ă���ꍇ�̓R�[���o�b�N�֐����Ă�
			if( WinData.MenuProc != NULL )
			{
				WinData.MenuProc( ItemID ) ;
			}

			if( WinData.MenuCallBackFunction != NULL )
			{
				TCHAR NameBuffer[128] ;
				
				// �I�����ڂ̖��O���擾����
				NS_GetMenuItemName( (int)ItemID, NameBuffer ) ;
				
				// �R�[���o�b�N�֐����Ă�
				WinData.MenuCallBackFunction( NameBuffer, (int)ItemID ) ;
			}

			// �I�����ꂽ���ڂ̏��ɒǉ�
			if( WinData.SelectMenuItemNum < MAX_MENUITEMSELECT_NUM )
			{
				int i ;
			
				// ���ɂ�������ǉ����Ȃ�
				for( i = 0 ; i < WinData.SelectMenuItemNum ; i ++ )
					if( WinData.SelectMenuItem[i] == ItemID ) break ;

				// ���������ꍇ�̂ݒǉ�
				if( i == WinData.SelectMenuItemNum )
				{
					WinData.SelectMenuItem[WinData.SelectMenuItemNum] = ItemID ;
					WinData.SelectMenuItemNum ++ ;
				}
			}
		}
		break ;

#ifndef DX_NON_KEYEX
	// �h�l�d���b�Z�[�W�������ꍇ�͓Ǝ��֐��ɓn��
	case WM_IME_SETCONTEXT :
	case WM_IME_STARTCOMPOSITION :
	case WM_IME_ENDCOMPOSITION :
	case WM_IME_COMPOSITION :
	case WM_IME_NOTIFY :
	case WM_IME_REQUEST :
		return IMEProc( hWnd , message , wParam , lParam ) ;
#endif

	// �E�C���h�E�̃T�C�Y���ύX����Ă��鎞�̏���
	case WM_SIZING :
		GetWindowRect( hWnd, &WinData.WindowEdgeRect ) ;

		// �E�C���h�E���[�h�̏ꍇ�̂ݏ���
		if( WinData.WindowModeFlag == TRUE )
		{
			// ���[�U�[�̃E�C���h�E���g�p���Ă���ꍇ�͕��̒����͂��Ȃ�
#ifndef DX_NON_GRAPHICS
			if( WinData.UserWindowFlag == FALSE )
			{
				WPARAM Side = wParam ;
				RECT *NextRect = (RECT *)lParam ;
				int Width, Height ;
				int NextWidth, NextHeight ;
				int AddWidth, AddHeight ;
				int DesktopW, DesktopH ;
				RECT WinRect, CliRect ;
				int WidthBigFlag ;

				// �f�X�N�g�b�v�̃T�C�Y���擾����
				NS_GetDefaultState( &DesktopW, &DesktopH, NULL, NULL ) ; 

				// �`���ʂ̃T�C�Y���擾����
				NS_GetDrawScreenSize( &Width, &Height ) ;

				// ���݂̃E�C���h�E�̗̈�ƃN���C�A���g�̈���擾����
				GetWindowRect( hWnd, &WinRect ) ;
				GetClientRect( hWnd, &CliRect ) ;

				// �N���C�A���g�̈�ȊO�̕����̕��ƍ������Z�o
				AddWidth  = ( WinRect.right  - WinRect.left ) - ( CliRect.right  - CliRect.left ) ;
				AddHeight = ( WinRect.bottom - WinRect.top  ) - ( CliRect.bottom - CliRect.top  ) ;

				// �N���C�A���g�̈�̃T�C�Y�𓾂�
				NextWidth  = ( NextRect->right  - NextRect->left ) - AddWidth ;
				NextHeight = ( NextRect->bottom - NextRect->top  ) - AddHeight ;
				if( NextWidth  + AddWidth  > DesktopW ) NextWidth  = DesktopW - AddWidth ;
				if( NextHeight + AddHeight > DesktopH ) NextHeight = DesktopH - AddHeight ;

				// �N���C�A���g�̈�ɉ�ʂ��t�B�b�g�����邩�ǂ����ŏ����𕪊�
				if( WinData.ScreenNotFitWindowSize == FALSE && WinData.WindowSizeValid == FALSE )
				{
					// �t�B�b�g������ꍇ

					// ���[�U�[�w��̍ŏ��T�C�Y���L���ȏꍇ�͓K�p����
					if( WinData.WindowMinSizeValid )
					{
						if( WinData.WindowMinWidth > NextWidth )
						{
							NextWidth = WinData.WindowMinWidth ;
						}

						if( WinData.WindowMinHeight > NextHeight )
						{
							NextHeight = WinData.WindowMinHeight ;
						}
					}

					// ���[�U�[�w��̍ő�T�C�Y���L���ȏꍇ�͓K�p����
					if( WinData.WindowMaxSizeValid )
					{
						if( WinData.WindowMaxWidth < NextWidth )
						{
							NextWidth = WinData.WindowMaxWidth ;
						}

						if( WinData.WindowMaxHeight < NextHeight )
						{
							NextHeight = WinData.WindowMaxHeight ;
						}
					}

					// ���ƍ����A�䗦�I�ɂǂ��炪���傫�������Z�o
					WidthBigFlag = ( (double)NextWidth / Width > (double)NextHeight / Height ) ? TRUE : FALSE ;

					// �V�����\���䗦��ݒ肷��
					switch( Side )
					{
					case WMSZ_LEFT :
					case WMSZ_RIGHT :
			WIDTH_SIZE_BASE :
						if( NextWidth + AddWidth > DesktopW ) NextWidth = DesktopW - AddWidth ;
						WinData.WindowSizeExRateY =
						WinData.WindowSizeExRateX = (double)NextWidth / Width ;
						NextHeight = NextWidth * Height / Width ;

						// ���[�U�[�w��̍ŏ��T�C�Y���L���ȏꍇ�͓K�p����
						if( WinData.WindowMinSizeValid && WinData.WindowMinHeight > NextHeight )
						{
							NextHeight = WinData.WindowMinHeight ;
							goto HEIGHT_SIZE_BASE ;
						}

						// ���[�U�[�w��̍ő�T�C�Y���L���ȏꍇ�͓K�p����
						if( WinData.WindowMaxSizeValid && WinData.WindowMaxHeight < NextHeight )
						{
							NextHeight = WinData.WindowMaxHeight ;
							goto HEIGHT_SIZE_BASE ;
						}
						break ;

					case 9 :
					case WMSZ_TOP :
					case WMSZ_BOTTOM :
			HEIGHT_SIZE_BASE :
						if( NextHeight + AddHeight > DesktopH ) NextHeight = DesktopH - AddHeight ;
						WinData.WindowSizeExRateY =
						WinData.WindowSizeExRateX = (double)NextHeight / Height ;
						NextWidth = NextHeight * Width / Height ;

						// ���[�U�[�w��̍ŏ��T�C�Y���L���ȏꍇ�͓K�p����
						if( WinData.WindowMinSizeValid && WinData.WindowMinWidth > NextWidth )
						{
							NextWidth = WinData.WindowMinWidth ;
							goto WIDTH_SIZE_BASE ;
						}

						// ���[�U�[�w��̍ő�T�C�Y���L���ȏꍇ�͓K�p����
						if( WinData.WindowMaxSizeValid && WinData.WindowMaxWidth < NextWidth )
						{
							NextWidth = WinData.WindowMaxWidth ;
							goto WIDTH_SIZE_BASE ;
						}
						break ;

					case WMSZ_TOPLEFT :
					case WMSZ_TOPRIGHT :
					case WMSZ_BOTTOMLEFT :
					case WMSZ_BOTTOMRIGHT :
						if( WidthBigFlag )
						{
							goto WIDTH_SIZE_BASE ;
						}
						else
						{
							goto HEIGHT_SIZE_BASE ;
						}
						break ;
					}

					// �T�C�Y�� 128 �ȉ��ɂ͂Ȃ�Ȃ��悤�ɂ���
					if( NextWidth < 128 || NextHeight < 128 )
					{
						if( Width > Height )
						{
							NextHeight = 128 ;
							NextWidth = NextHeight * Width / Height ;
						}
						else
						{
							NextWidth = 128 ;
							NextHeight = NextWidth * Height / Width ;
						}

						WinData.WindowSizeExRateY =
						WinData.WindowSizeExRateX = (double)NextWidth / Width ;
					}

					// �E�C���h�E�̌`��␳����
					switch( Side )
					{
					case WMSZ_TOPLEFT :
						NextRect->left   = NextRect->right  - ( NextWidth  + AddWidth  ) ;
						NextRect->top    = NextRect->bottom - ( NextHeight + AddHeight ) ;
						break ;

					case WMSZ_TOPRIGHT :
						NextRect->right  = NextRect->left   + ( NextWidth  + AddWidth  ) ;
						NextRect->top    = NextRect->bottom - ( NextHeight + AddHeight ) ;
						break ;

					case WMSZ_LEFT :
						NextRect->left   = NextRect->right  - ( NextWidth  + AddWidth  ) ;
						NextRect->bottom = NextRect->top    + ( NextHeight + AddHeight ) ;
						break ;

					case WMSZ_RIGHT :
						NextRect->right  = NextRect->left   + ( NextWidth  + AddWidth  ) ;
						NextRect->bottom = NextRect->top    + ( NextHeight + AddHeight ) ;
						break ;

					case 9 :
					case WMSZ_TOP :
						NextRect->right  = NextRect->left   + ( NextWidth  + AddWidth  ) ;
						NextRect->top    = NextRect->bottom - ( NextHeight + AddHeight ) ;
						break ;

					case WMSZ_BOTTOM :
						NextRect->right  = NextRect->left   + ( NextWidth  + AddWidth  ) ;
						NextRect->bottom = NextRect->top    + ( NextHeight + AddHeight ) ;
						break ;

					case WMSZ_BOTTOMLEFT :
						NextRect->left   = NextRect->right  - ( NextWidth  + AddWidth  ) ;
						NextRect->bottom = NextRect->top    + ( NextHeight + AddHeight ) ;
						break ;

					case WMSZ_BOTTOMRIGHT :
						NextRect->right  = NextRect->left   + ( NextWidth  + AddWidth  ) ;
						NextRect->bottom = NextRect->top    + ( NextHeight + AddHeight ) ;
						break ;
					}
				}
				else
				{
					int MinSizeW, MinSizeH ;
					int MainScreenW, MainScreenH ;

					// �t�B�b�g�����Ȃ��ꍇ

					// ���A��������ʂ̃T�C�Y�ȏ�ɂȂ��Ă����琧������
					NS_GetDrawScreenSize( &MainScreenW, &MainScreenH ) ;
					MainScreenW = _DTOL( MainScreenW * WinData.WindowSizeExRateX ) ;
					MainScreenH = _DTOL( MainScreenH * WinData.WindowSizeExRateY ) ;

					// ���[�U�[�w��̍ŏ��T�C�Y���`���ʂ̃T�C�Y���傫�����( ���`����ʂ�菬������� )��������g�p����
					if( WinData.WindowMinSizeValid )
					{
						MinSizeW = WinData.WindowMinWidth ;
						MinSizeH = WinData.WindowMinHeight ;

						// 128�s�N�Z���ȉ��ɂ͂ł��Ȃ�
						if( MinSizeW < 128 )
						{
							MinSizeW = 128 ;
						}
						if( MinSizeH < 128 )
						{
							MinSizeH = 128 ;
						}
					}
					else
					{
						MinSizeW = 128 ;
						MinSizeH = 128 ;
					}

					// ���[�U�[�w��̍ő�T�C�Y���`���ʂ̃T�C�Y��菬������΂�������g�p����
					if( WinData.WindowMaxSizeValid )
					{
						if( WinData.WindowMaxWidth < MainScreenW )
						{
							MainScreenW = WinData.WindowMaxWidth ;
						}

						if( WinData.WindowMaxHeight < MainScreenH )
						{
							MainScreenH = WinData.WindowMaxHeight ;
						}
					}

					// �E�C���h�E�̌`��␳����
					if( NextWidth > MainScreenW )
					{
						if( Side == WMSZ_RIGHT ||
							Side == WMSZ_TOPRIGHT ||
							Side == WMSZ_BOTTOMRIGHT )
						{
							NextRect->right = NextRect->left + ( MainScreenW + AddWidth ) ;
						}
						else
						{
							NextRect->left  = NextRect->right - ( MainScreenW + AddWidth ) ;
						}
					}
					else
					if( NextWidth < MinSizeW )
					{
						if( Side == WMSZ_RIGHT ||
							Side == WMSZ_TOPRIGHT ||
							Side == WMSZ_BOTTOMRIGHT )
						{
							NextRect->right = NextRect->left + ( MinSizeW + AddWidth ) ;
						}
						else
						{
							NextRect->left  = NextRect->right - ( MinSizeW + AddWidth ) ;
						}
					}

					if( NextHeight > MainScreenH )
					{
						if( Side == WMSZ_BOTTOM ||
							Side == WMSZ_BOTTOMLEFT ||
							Side == WMSZ_BOTTOMRIGHT )
						{
							NextRect->bottom = NextRect->top + ( MainScreenH + AddHeight ) ;
						}
						else
						{
							NextRect->top = NextRect->bottom - ( MainScreenH + AddHeight ) ;
						}
					}
					else
					if( NextHeight < MinSizeH )
					{
						if( Side == WMSZ_BOTTOM ||
							Side == WMSZ_BOTTOMLEFT ||
							Side == WMSZ_BOTTOMRIGHT )
						{
							NextRect->bottom = NextRect->top + ( MinSizeH + AddHeight ) ;
						}
						else
						{
							NextRect->top = NextRect->bottom - ( MinSizeH + AddHeight ) ;
						}
					}
/*
					DXST_ERRORLOGFMT_ADDW(( L"left:%d top:%d right:%d bottom:%d",
						NextRect->left,
						NextRect->top,
						NextRect->right,
						NextRect->bottom )) ;
*/
				}
			}
#endif // DX_NON_GRAPHICS
		}
		break ;


	// �E�C���h�E�̃T�C�Y�����ɕۂ���
	case WM_SIZE :
		GetWindowRect( hWnd, &WinData.WindowEdgeRect ) ;

		// �ő剻�ł͂Ȃ�������ő剻��ԃt���O��|��
		if( wParam != SIZE_MAXIMIZED )
		{
			WinData.WindowMaximizeFlag = FALSE ;
		}

		PrevWindowMinSizeFlag = WinData.WindowMinSizeFlag ;
		WinData.WindowMinSizeFlag = wParam == SIZE_MINIMIZED ? TRUE : FALSE ;

		// �����ő剻���ꂽ�ꍇ�Ŋ��񓯊��E�C���h�E���[�h�ύX�@�\���L���ɂȂ��Ă���ꍇ�̓E�C���h�E���[�h�ύX�������s��
		if( wParam == SIZE_MAXIMIZED && WinData.UseChangeWindowModeFlag )
		{
			// �����E�C���h�E���[�h�ύX����E�C���h�E�쐬����A���ꂩ�E�C���h�E���[�h�ł͂Ȃ������ꍇ�͉��������ɏI��
			if( WinData.ChangeWindodwFlag == FALSE &&
				WinData.WindowCreateFlag == FALSE &&
				WinData.WindowModeFlag == TRUE )
			{
				// �E�C���h�E���[�h����t���X�N���[�����[�h�Ɉڍs����
				WinData.ChangeWindowModeFlag = TRUE ;
			}
		}
		else
		{
			// �ő剻�������ꍇ�ł܂��ő剻�������s���Ă��炸�A���N���C�A���g�̈�ɉ�ʂ��t�B�b�g������ꍇ�̓E�C���h�E�̃X�P�[����ύX����
			if( wParam == SIZE_MAXIMIZED && 
				WinData.WindowMaximizeFlag == FALSE /* &&
				WinData.ScreenNotFitWindowSize == FALSE &&
				WinData.WindowSizeValid == FALSE */ )
			{
//				RECT CliRect ;
//				int Width, Height, NextWidth, NextHeight ;

				// �`���ʂ̃T�C�Y���擾����
//				NS_GetDrawScreenSize( &Width, &Height ) ;

				// ���܂ōŏ�������Ă����ꍇ�͒l���X�V���Ȃ�
				if( PrevWindowMinSizeFlag == FALSE )
				{
					// �E�C���h�E�̃N���C�A���g�̈���擾����
					RECT PrevWindowMaximizeClientRect = WinData.WindowMaximizedClientRect ;
					GetClientRect( hWnd, &WinData.WindowMaximizedClientRect ) ;

					// �E�C���h�E�̗̈���擾����
					RECT PrevWindowMaximizeRect = WinData.WindowMaximizedRect ;
					GetWindowRect( hWnd, &WinData.WindowMaximizedRect ) ;

					WinData.WindowMaximizedRect.left   -= WinData.WindowMaximizedAdjustRect.left - PrevWindowMaximizeRect.left ;
					WinData.WindowMaximizedRect.right  -= WinData.WindowMaximizedAdjustRect.left - PrevWindowMaximizeRect.left ;
					WinData.WindowMaximizedRect.top    -= WinData.WindowMaximizedAdjustRect.top  - PrevWindowMaximizeRect.top ;
					WinData.WindowMaximizedRect.bottom -= WinData.WindowMaximizedAdjustRect.top  - PrevWindowMaximizeRect.top ;

					// �S���̓�������l���قȂ�ꍇ�́A���̗v�f���ЂƂO�̒l���g�p����
					{
						int EqualCount = 0 ;

						if( WinData.WindowMaximizedRect.left   == PrevWindowMaximizeRect.left   ) EqualCount ++ ;
						if( WinData.WindowMaximizedRect.top    == PrevWindowMaximizeRect.top    ) EqualCount ++ ;
						if( WinData.WindowMaximizedRect.right  == PrevWindowMaximizeRect.right  ) EqualCount ++ ;
						if( WinData.WindowMaximizedRect.bottom == PrevWindowMaximizeRect.bottom ) EqualCount ++ ;
						
						if( EqualCount >= 3 )
						{
							WinData.WindowMaximizedRect       = PrevWindowMaximizeRect ;
							WinData.WindowMaximizedClientRect = PrevWindowMaximizeClientRect ;
						}
					}

					// ��ԍŏ��̍ő剻�̋�`���L���ŁA���ʂ̃f�B�X�v���C�ōő剻���ꂽ�Ǝv�����ꍇ��
					// ��ԍŏ��̍ő剻�̋�`�𖳌��ɂ���
					if( WinData.ValidFirstWindowMaximizedRect )
					{
						if( WinData.FirstWindowMaximizedRect.right  - 32 <= WinData.WindowMaximizedRect.left      ||
							WinData.FirstWindowMaximizedRect.bottom - 32 <= WinData.WindowMaximizedRect.top       ||
							WinData.WindowMaximizedRect.right       - 32 <= WinData.FirstWindowMaximizedRect.left ||
							WinData.WindowMaximizedRect.bottom      - 32 <= WinData.FirstWindowMaximizedRect.top  )
						{
							WinData.ValidFirstWindowMaximizedRect = FALSE ;
						}
					}

					// ��ԍŏ��̍ő剻�̏ꍇ�� FirstWindowMaximizeRect �ɂ��ۑ�����
					if( WinData.ValidFirstWindowMaximizedRect == FALSE )
					{
						WinData.ValidFirstWindowMaximizedRect = TRUE ;
						WinData.FirstWindowMaximizedRect = WinData.WindowMaximizedRect ;
					}
				}

				// WM_PAINT �̏������s��
//					{
//						HDC Dc = GetDC( hWnd ) ;
//
//						WM_PAINTProcess( Dc, &WinData.WindowMaximizedClientRect, TRUE ) ;
//
//						ReleaseDC( hWnd, Dc ) ;
//					}

				// �ő剻��ԃt���O�𗧂Ă�
				WinData.WindowMaximizeFlag = TRUE ;
			}
			WM_SIZEProcess() ;
		}

		// �c�[���o�[���L���ȏꍇ�̓c�[���o�[�ɂ�����
		if( WinData.ToolBarUseFlag == TRUE )
		{
			SendMessageW( WinData.ToolBarHandle, WM_SIZE, wParam, lParam ) ;
		}
		break ;




	// ��ʉ𑜓x�ύX���̏���
	case WM_DISPLAYCHANGE :
#ifndef DX_NON_GRAPHICS
//		// �t���X�N���[�����[�h�ŁA���Ƀ��C���E�C���h�E�ȊO���A�N�e�B�u�ɂȂ��Ă���
//		// �ꍇ�́A�X�ɕʂ̃E�C���h�E���A�N�e�B�u�ɂȂ�f�X�N�g�b�v��ʂɖ߂����Ɣ��f
//		if( WinData.WindowModeFlag == FALSE && WinData.StopFlag == TRUE )
//		{
//			int Cx = LOWORD( lParam ), Cy = HIWORD( lParam ), Ccb = wParam ;
//			int Sx, Sy, Scb ;
//
//			// ���X�̃f�X�N�g�b�v��ʂ̃T�C�Y�𓾂�
//			NS_GetDefaultState( &Sx , &Sy , &Scb ) ;
//
//			// �����f�X�N�g�b�v��ʂƓ�����ʃ��[�h�ł���Ίm��
//			if( Scb == Ccb && Sx == Cx && Sy == Cy )
//			{
//				// DirectX �֘A�I�u�W�F�N�g�̈ꎞ�I����������s��
////			DestroyGraphSystem() ;
//			}
//		}
//		else
		// �E�C���h�E���[�h���̃G���[�I��
		if( WinData.WindowModeFlag == TRUE && WinData.ChangeWindodwFlag == FALSE )
		{
			WPARAM ColorBit = wParam ;
			int Cx = LOWORD( lParam ) , Cy = HIWORD( lParam ) ;
			int Sx , Sy , Cb;

			// �]�݂̉�ʃ��[�h���擾
			NS_GetScreenState( &Sx, &Sy, &Cb ) ;

			// ���������X���b�h���o�^����Ă��Ȃ��ꍇ�̓\�t�g�I���A�Q�S�r�b�g�J���[���[�h�̏ꍇ���I��
			if( ColorBit != ( UINT_PTR )Cb || Sx != Cx || Sy != Cy )
			{
				if( !NS_GetValidRestoreShredPoint() || ColorBit == 24 || 
					( ColorBit != ( UINT_PTR )Cb && ColorBit == 8 ) || Sx > Cx || Sy > Cy )
				{
					DXST_ERRORLOG_ADDUTF16LE( "\xa9\x5f\x43\x51\xa2\x95\x70\x65\x4c\x30\x7b\x76\x32\x93\x55\x30\x8c\x30\x66\x30\x44\x30\x6a\x30\x44\x30\x5f\x30\x81\x30\x4b\x30\x3b\x75\x62\x97\x4c\x30\x12\xff\x14\xff\xd3\x30\xc3\x30\xc8\x30\xab\x30\xe9\x30\xfc\x30\x6b\x30\x09\x59\xf4\x66\x55\x30\x8c\x30\x5f\x30\x5f\x30\x81\x30\x42\x7d\x86\x4e\x57\x30\x7e\x30\x59\x30\x0a\x00\x00"/*@ L"�����֐����o�^����Ă��Ȃ����߂���ʂ��Q�S�r�b�g�J���[�ɕύX���ꂽ���ߏI�����܂�\n" @*/ ) ;
					DXST_ERRORLOG_ADDUTF16LE( "\x7e\x30\x5f\x30\x6f\x30\xb9\x30\xaf\x30\xea\x30\xfc\x30\xf3\x30\x4c\x30\x0f\x5c\x55\x30\x59\x30\x4e\x30\x8b\x30\x5f\x30\x81\x30\x4b\x30\x72\x82\xd3\x30\xc3\x30\xc8\x30\x70\x65\x6e\x30\x55\x90\x44\x30\x4c\x30\x27\x59\x4d\x30\x44\x30\x5f\x30\x81\x30\x4b\x30\x42\x7d\x86\x4e\x57\x30\x7e\x30\x59\x30\x0a\x00\x00"/*@ L"�܂��̓X�N���[�������������邽�߂��F�r�b�g���̈Ⴂ���傫�����߂��I�����܂�\n" @*/ ) ;
				
					// �N���[�Y�t���O���|��Ă�����WM_CLOSE���b�Z�[�W�𑗂�
					if( !WinData.CloseMessagePostFlag )
					{
						WinData.CloseMessagePostFlag = TRUE ;
						PostMessageW( WinData.MainWindow , WM_CLOSE, 0, 0 );
					}

					break ;
				}

#ifndef DX_NON_MOVIE
				// �Đ����̃��[�r�[�I�u�W�F�N�g�����ׂĎ~�߂�
				DisableMovieAll() ;

				// ���[�r�[�O���t�B�b�N�̍Đ�
				PlayMovieAll() ;
#endif

//				MessageBox( hWnd , "����ۏ؊O�̓��삪�����������ߏI�����܂�" , "�G���[" , MB_OK ) ;
//				return DxLib_Error( DXSTRING( "�E�C���h�E���[�h���Ƀf�X�N�g�b�v�̉𑜓x���ύX����܂����A�I�����܂�" ) ;
			}
		}
#endif // DX_NON_GRAPHICS

		WinData.DisplayChangeMessageFlag = TRUE ;
		break ;

	// ���j���[�ɏd�Ȃ��Ă��邩�e�X�g
	case WM_NCHITTEST :
#ifndef DX_NON_GRAPHICS
		if( GSYS.Setting.ValidHardware && WinData.WindowModeFlag == FALSE )
		{
			if( HIWORD( lParam ) < 8 )
				return HTMENU ;
		}
#endif // DX_NON_GRAPHICS
		break ;

	// �E�C���h�E�̃T�C�Y��ʒu�̕ύX���J�n����邩�A�V�X�e�����j���[�̑I�����J�n���ꂽ���A�N�e�B�u�Ɠ��������ɂ���
	case WM_ENTERSIZEMOVE :
	case WM_ENTERMENULOOP :
		if( WinData.WindowModeFlag == FALSE )
			break ;

		if( WinData.NotActive_WindowMoveOrSystemMenu == FALSE )
		{
			WinData.NotActive_WindowMoveOrSystemMenu = TRUE ;
			WM_ACTIVATEProcess( 0, 0, 0, TRUE ) ;
		}
		break ;

	// �^�C�g���o�[��ŉE�N���b�N�����ꂽ�ꍇ�͔�A�N�e�B�u�����ɂ���
	case WM_NCRBUTTONDOWN :
		if( WinData.WindowModeFlag == FALSE )
			break ;

		if( WinData.NotActive_WindowMoveOrSystemMenu == FALSE )
		{
			WinData.NotActive_WindowMoveOrSystemMenu = TRUE ;
			WM_ACTIVATEProcess( 0, 0, 0, TRUE ) ;
		}
		break ;

	// �E�C���h�E�̃T�C�Y��ʒu�̕ύX���J�n����邩�A�V�X�e�����j���[�̑I�����I��
	case WM_EXITSIZEMOVE :
	case WM_EXITMENULOOP :
		if( WinData.NotActive_WindowMoveOrSystemMenu )
		{
			WinData.NotActive_WindowMoveOrSystemMenu = FALSE ;
			WM_ACTIVATEProcessUseStock( 1, 0, 0, TRUE ) ;
		}
		break ;

	// �J�[�\���X�V��
	case WM_SETCURSOR :
		if( WinData.NotActive_WindowMoveOrSystemMenu )
		{
			WinData.NotActive_WindowMoveOrSystemMenu = FALSE ;
			WM_ACTIVATEProcessUseStock( 1, 0, 0, TRUE ) ;
		}

#ifndef DX_NON_GRAPHICS
		if( GSYS.Setting.ValidHardware && WinData.WindowModeFlag == FALSE && WinData.MenuUseFlag == TRUE )
		{
			if( HTMENU == LOWORD( lParam ) || HTCAPTION == LOWORD( lParam ) )
			{
				if( WinData.MousePosInMenuBarFlag < 5 )
				{
					Graphics_Win_SetDialogBoxMode( TRUE ) ;
					DrawMenuBar( hWnd ) ;
					WinData.MousePosInMenuBarFlag ++ ;
					NS_SetMouseDispFlag( -1 ) ;
				}
			}
			else
			{
				if( WinData.MousePosInMenuBarFlag > 0 && HTNOWHERE != LOWORD( lParam ) )
				{
					Graphics_Win_SetDialogBoxMode( FALSE ) ;
					WinData.MousePosInMenuBarFlag = FALSE ;
				}
			}
		}
#endif // DX_NON_GRAPHICS

	case WM_LBUTTONDOWN :
	case WM_RBUTTONDOWN :
	case WM_MBUTTONDOWN :
	case WM_MOUSEMOVE :
	case WM_LBUTTONUP :
	case WM_RBUTTONUP :
	case WM_MBUTTONUP :

#ifndef DX_NON_INPUT
		// �{�^�����̓R�[�h���Z�b�g����
		switch( message )
		{
		case WM_LBUTTONDOWN : StockMouseInputInfo( MOUSE_INPUT_LEFT   ) ; break ;
		case WM_RBUTTONDOWN : StockMouseInputInfo( MOUSE_INPUT_RIGHT  ) ; break ;
		case WM_MBUTTONDOWN : StockMouseInputInfo( MOUSE_INPUT_MIDDLE ) ; break ;
		}
#endif // DX_NON_INPUT

		// �}�E�X���g�p���Ȃ��ݒ�ɂȂ��Ă����ꍇ�}�E�X�J�[�\��������
		if( /*!WinData.WindowModeFlag &&*/ /*!WinData.MouseDispFlag*/ WinData.MouseDispState == FALSE )
		{
			SetCursor( NULL ) ;
			while( ShowCursor( FALSE ) > -1 ){} ;
		}
		break ;

	// �ĕ`�惁�b�Z�[�W
	case WM_PAINT :
#ifndef DX_NON_GRAPHICS
		{
			PAINTSTRUCT PaintStr ;

			// WM_PAINT ���b�Z�[�W���������Ƃ��L�^���Ă���
			WinData.RecvWM_PAINTFlag = TRUE;

			if( BeginPaint( hWnd , &PaintStr ) == NULL ) break ;

			// �E�C���h�E���[�h�̏ꍇ�̂ݏ������s��
			if( WinData.WindowModeFlag == TRUE )
			{
				WinData.WM_PAINTFlag = TRUE ;

				// WM_PAINT �̏������s��
				WM_PAINTProcess( PaintStr.hdc, &PaintStr.rcPaint ) ;

				WinData.WM_PAINTFlag = FALSE ;
			}
			EndPaint( hWnd , &PaintStr ) ;
		}
#endif // DX_NON_GRAPHICS
		break ;

	// �V�X�e�������R�[�h���b�Z�[�W
	case WM_SYSCHAR :
		{
			// ���͂��ꂽ�V�X�e�������R�[�h��ۑ�
			WinData.InputSysChara = (wchar_t)wParam ;

			// ����ALT+ENTER��������A�񓯊��E�C���h�E���[�h�ύX�@�\���L���ɂȂ��Ă�����
			if( (wchar_t)wParam == CTRL_CODE_CR )
			{
				// �E�C���h�E���[�h�ύX�t���O�𗧂Ă�
				if( WinData.UseChangeWindowModeFlag )
				{
					WinData.ChangeWindowModeFlag = TRUE ;
					return 0 ;
				}
			}
			return 0 ;
		}
		break ;

#ifndef DX_NON_INPUTSTRING
	// �����R�[�h���b�Z�[�W
	case WM_CHAR :

		// �������o�b�t�@�ɃR�s�[
		if( wParam == 10 )
		{
			StockInputChar_WCHAR_T( ( wchar_t )CTRL_CODE_CR ) ;
		}
		else
		{
			if( wParam >= 0x20            || 
				wParam == CTRL_CODE_BS    || 
				wParam == CTRL_CODE_TAB   || 
				wParam == CTRL_CODE_CR    || 
				wParam == CTRL_CODE_DEL   || 
				wParam == CTRL_CODE_ESC   ||
				wParam == CTRL_CODE_COPY  ||
				wParam == CTRL_CODE_PASTE ||
				wParam == CTRL_CODE_CUT   ||
				wParam == CTRL_CODE_ALL   )
				StockInputChar_WCHAR_T( ( wchar_t )wParam ) ;
		}

		break ;
#endif // DX_NON_INPUTSTRING

	case WM_SYSCOMMAND :
		if( ( wParam & 0xfff0 ) == SC_MOVE )
		{
			if( WinData.NotActive_WindowMoveOrSystemMenu == FALSE )
			{
				WinData.NotActive_WindowMoveOrSystemMenu = TRUE ;
				WM_ACTIVATEProcess( 0, 0, 0, TRUE ) ;
			}
		}

		if( ( wParam & 0xfff0 ) == SC_KEYMENU && WinData.SysCommandOffFlag == TRUE )
			return 0 ;

		if( ( wParam & 0xfff0 ) == SC_MONITORPOWER )
			return 1 ;		// �ȓd�̓��[�h��h�~����

		if( ( wParam & 0xfff0 ) == SC_SCREENSAVE )
			return 1 ;		// �X�N���[���Z�[�o�[�̋N����h�~����
//		if( wParam == SC_CLOSE && WinData.SysCommandOffFlag == TRUE )
//			return 0 ;
		break ;

/*	case WM_SYSKEYDOWN :
		{
			int VKey = ( int )wParam ;

			if( VKey == VK_F10 )
			{
				SetF10Input() ;
			}
		}
		break ;

	case WM_SYSKEYUP :
		{
			int VKey = ( int )wParam ;

			if( VKey == VK_F10 )
			{
				ResetF10Input() ;
			}
		}
		break ;
*/

#ifndef DX_NON_INPUT
	// �e�P�O���b�Z�[�W
	case F10MES :
		{
			if( wParam == 1 )
			{
				SetF10Input() ;
			}
			else
			{
				ResetF10Input() ;
			}
		}
		break ;

	// �e�P�Q���b�Z�[�W
	case F12MES :
		{
			if( wParam == 1 )
			{
				SetF12Input() ;
			}
			else
			{
				ResetF12Input() ;
			}
		}
		break ;
#endif // DX_NON_INPUT

#ifndef DX_NON_INPUTSTRING
	// �L�[�������b�Z�[�W
	case WM_KEYDOWN:
		{
			// �o�b�t�@����t�̏ꍇ�͂Ȃɂ����Ȃ�
			if( ( CharBuf.EdPoint + 1 == CharBuf.StPoint ) ||
				( CharBuf.StPoint == 0 && CharBuf.EdPoint == CHARBUFFER_SIZE ) ) break ;

			// �R���g���[�������R�[�h�ɑΉ�����L�[��
			// ������Ă�����o�b�t�@�Ɋi�[����
			{
				int   VKey  = ( int )wParam ;
				char *CCode = CtrlCode[ 0 ] ;

				for( CCode = ( char * )CtrlCode ; CCode[ 0 ] != 0 && CCode[ 0 ] != VKey ; CCode += 2 ){}
				if( CCode[ 0 ] != 0 )
				{
					// �o�b�t�@�ɕ����R�[�h����
					StockInputChar_WCHAR_T( ( wchar_t )*( CCode + 1 ) ) ;
				}
			}
		}
		break ;
#endif // DX_NON_INPUTSTRING

	// �z�C�[���̈ړ��ʎ擾
	case WM_MOUSEWHEEL :
		{
			int MoveParamZ = ( SHORT )HIWORD( wParam ) ;

			// ���܂ł̈ړ��ƕ������قȂ�ꍇ�͂O�Ɏd�؂�Ȃ���
			if( ( MoveParamZ < 0 && WinData.MouseMoveZ > 0 ) ||
				( MoveParamZ > 0 && WinData.MouseMoveZ < 0 ) )
			{
				WinData.MouseMoveZ = MoveParamZ ;
			}
			else
			{
				WinData.MouseMoveZ += MoveParamZ ;
			}
		}
		break ;

	// �z�C�[���̉��ړ��ʎ擾
	case WM_MOUSEHWHEEL :
		{
			int MoveParamHZ = ( SHORT )HIWORD( wParam ) ;

			// ���܂ł̈ړ��ƕ������قȂ�ꍇ�͂O�Ɏd�؂�Ȃ���
			if( ( MoveParamHZ < 0 && WinData.MouseMoveHZ > 0 ) ||
				( MoveParamHZ > 0 && WinData.MouseMoveHZ < 0 ) )
			{
				WinData.MouseMoveHZ = MoveParamHZ ;
			}
			else
			{
				WinData.MouseMoveHZ += MoveParamHZ ;
			}
		}
		break ;

	// �^�b�`
	case WM_TOUCH :
		{
			UINT TouchInputNum ;
			D_TOUCHINPUT TouchInputDefault[ TOUCHINPUTPOINT_MAX ] ;
			D_PTOUCHINPUT TouchInput ;

			if( WinAPIData.Win32Func.GetTouchInputInfoFunc != NULL )
			{
				TouchInputNum = LOWORD( wParam ) ;
				if( TouchInputNum > TOUCHINPUTPOINT_MAX )
				{
					TouchInput = ( D_TOUCHINPUT * )DXALLOC( sizeof( D_TOUCHINPUT ) * TouchInputNum ) ;
				}
				else
				{
					TouchInput = TouchInputDefault ;
				}

				if( TouchInput != NULL )
				{
					if( WinAPIData.Win32Func.GetTouchInputInfoFunc(	( D_HTOUCHINPUT )lParam, TouchInputNum, TouchInput, sizeof( D_TOUCHINPUT ) ) )
					{
						DWORD i ;
						D_TOUCHINPUT *TInp ;
						TOUCHINPUTDATA TouchInputData ;
						TOUCHINPUTPOINT *TouchPoint ;

						TouchInputData.Time = ( LONGLONG )TouchInput[ 0 ].dwTime * 1000 ;
						TouchInputData.PointNum = 0 ;

						TouchPoint = TouchInputData.Point ;
						TInp = TouchInput ;
						for( i = 0; i < TouchInputNum; i++, TInp++ )
						{
							if( TInp->dwFlags & TOUCHEVENTF_UP )
							{
								continue ;
							}

							TouchPoint->Device = 0 ;
							TouchPoint->ID = TInp->dwID ;
							ConvScreenPositionToDxScreenPosition( TInp->x / 100, TInp->y / 100, &TouchPoint->PositionX, &TouchPoint->PositionY ) ;

							TouchPoint ++ ;
							TouchInputData.PointNum ++ ;
						}

						AddTouchInputData( &TouchInputData ) ;
					}

					WinAPIData.Win32Func.CloseTouchInputHandleFunc( ( D_HTOUCHINPUT )lParam ) ;

					if( TouchInputNum > TOUCHINPUTPOINT_MAX )
					{
						DXFREE( TouchInput ) ;
					}
				}
			}
		}
		break ;

	// �E�C���h�E�ړ�������
	case WM_MOVE :
		GetWindowRect( hWnd, &WinData.WindowEdgeRect ) ;
		if( WinData.WindowModeFlag && !WinData.ChangeWindodwFlag )
		{
			WM_MOVEProcess( lParam ) ;
		}
		break ;

	// �E�C���h�E�ړ��̗}��
	case WM_MOVING :
		GetWindowRect( hWnd, &WinData.WindowEdgeRect ) ;
		if( WinData.WindowModeFlag == FALSE )
		{
			RECT rect ;
			GetWindowRect( WinData.MainWindow , &rect ) ;
			*( RECT *)lParam = rect ;
		}
		break ;

	// �A�N�e�B�u��ԕω���
	case WM_ACTIVATEAPP :
		// ��A�N�e�B�u�ɂȂ����ꍇ�̂� WM_ACTIVATE �Ƃ��ď�������
		if( wParam == 0 ) goto ACTIVATELABEL ;
		break ;

	case WM_ACTIVATE :
ACTIVATELABEL:
#ifndef DX_NON_ASYNCLOAD
		/*
		// �񓯊��ǂݍ��݂̓r���̏ꍇ�́A�񓯊��ǂݍ��݂��I������܂ő҂�
		if( WinData.QuitMessageFlag == FALSE && WinData.CloseMessagePostFlag == FALSE )
		{
			while( NS_GetASyncLoadNum() > 0 )
			{
				ProcessASyncLoadRequestMainThread() ;
				if( WinData.ProcessorNum <= 1 )
				{
					Sleep( 2 ) ;
				}
				else
				{
					Sleep( 0 ) ;
				}
			}
		}
		*/
#endif // DX_NON_ASYNCLOAD

		WM_ACTIVATEProcessUseStock( wParam, lParam, message == WM_ACTIVATEAPP ? TRUE : FALSE, FALSE ) ;
		break;

	// �E�C���h�E�N���[�Y��
	case WM_CLOSE :
		// ���[�U�[�񋟂̃E�C���h�E�������牽�����Ȃ�
		if( WinData.UserWindowFlag == FALSE )
		{
			DXST_ERRORLOG_ADDUTF16LE( "\xa6\x30\xa4\x30\xf3\x30\xc9\x30\xa6\x30\x92\x30\x89\x95\x58\x30\x88\x30\x46\x30\x68\x30\x57\x30\x66\x30\x44\x30\x7e\x30\x59\x30\x0a\x00\x00"/*@ L"�E�C���h�E����悤�Ƃ��Ă��܂�\n" @*/ ) ;

			// WM_DESTROY���b�Z�[�W�𐶐�
			if( WinData.NonUserCloseEnableFlag == FALSE || WinData.AltF4_EndFlag == 1 )
				DestroyWindow( WinData.MainWindow ) ;

			// �����I��������
			if( WinData.AltF4_EndFlag == 0 ) WinData.AltF4_EndFlag = 4 ;

			return 0 ;
		}
		break ;

	// �E�C���h�E�j����
	case WM_DESTROY :
		// ���[�U�[�񋟂̃E�C���h�E�������牽�����Ȃ�
		if( WinData.UserWindowFlag == FALSE )
		{
			DXST_ERRORLOG_ADDUTF16LE( "\xa6\x30\xa4\x30\xf3\x30\xc9\x30\xa6\x30\x4c\x30\x34\x78\xc4\x68\x55\x30\x8c\x30\x88\x30\x46\x30\x68\x30\x57\x30\x66\x30\x44\x30\x7e\x30\x59\x30\x0a\x00\x00"/*@ L"�E�C���h�E���j������悤�Ƃ��Ă��܂�\n" @*/ ) ;

			// WM_DESTROY���b�Z�[�W���󂯂��؋����c��
			WinData.DestroyMessageCatchFlag = TRUE ;

			// �\�t�g�̏I�����b�Z�[�W�𑗂�
			if( WinData.NonDxLibEndPostQuitMessageFlag == FALSE )
				PostQuitMessage( 0 ) ;
			WinData.QuitMessageFlag = TRUE ;
			DxLib_EndRequest() ;

			DXST_ERRORLOG_ADDUTF16LE( "\xbd\x30\xd5\x30\xc8\x30\x92\x30\x42\x7d\x86\x4e\x59\x30\x8b\x30\x96\x6e\x99\x50\x4c\x30\x74\x65\x44\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�\�t�g���I�����鏀���������܂���\n" @*/ ) ;
		}
		break ;

	case WM_QUIT :
		break ;

#ifndef DX_NON_SOUND
	// �l�h�c�h���t�I����
	case MM_MCINOTIFY:	
		MidiCallBackProcess() ;					// �l�h�c�h���t�I�����̏���
		break;
#endif // DX_NON_SOUND

#ifndef DX_NON_NETWORK
	// �v�����r���������b�Z�[�W
	case WSA_WINSOCKMESSAGE :
		return WinSockProc( hWnd , message , wParam , lParam ) ;
#endif

	}

	if( WinData.UserWindowFlag == TRUE )
	{
		if( Ret == -1000 )	return UserProcRet ;
		else				return DefWindowProcW( hWnd , message , wParam , lParam ) ;
	}
	else
	{
		return DefWindowProcW( hWnd , message , wParam , lParam ) ;
	}
}

// ��A�N�e�B�u���ǂ������`�F�b�N����
extern int CheckActiveWait( void )
{
	if(
#ifndef DX_NON_GRAPHICS
		GSYS.Screen.ScreenFlipFlag  == FALSE &&
#endif // DX_NON_GRAPHICS
		WinData.WM_PAINTFlag         == FALSE &&
		( WinData.ActiveFlag         == FALSE || WinData.WindowMinSizeFlag == TRUE ) &&
		WinData.UserWindowFlag       == FALSE &&
		DxSysData.NotWinFlag         == FALSE &&
		(
		  (
			WinData.WindowModeFlag   == TRUE  &&
		    WinData.NonActiveRunFlag == FALSE
		  ) ||	
		  (
			WinData.WindowModeFlag   == FALSE &&
		    WinData.NonActiveRunFlag == FALSE
		  )
		)
	  )
	{
		return TRUE ;
	}
	else
	{
		return FALSE ;
	}
}

// �A�N�e�B�u�ɂȂ�܂ŉ������Ȃ�
extern void DxActiveWait( void )
{
	if( CheckActiveWait() == TRUE && WinData.QuitMessageFlag == FALSE && WinData.MainWindow != NULL )
	{
		WinData.WaitTime = NS_GetNowCount();
		WinData.WaitTimeValidFlag = TRUE ;
		do
		{
			if( NS_ProcessMessage() != 0 )
			{
				break ;
			}
		}while( WinData.ActiveFlag == FALSE || WinData.WindowMinSizeFlag == TRUE );
	}
}

// WM_QUIT �����s����Ă��邩�ǂ������擾����
extern int GetQuitMessageFlag( void )
{
	return WinData.QuitMessageFlag;
}




// �E�C���h�E�֌W���擾�֐�

// �E�C���h�E�̃N���C�A���g�̈���擾����
extern int NS_GetWindowCRect( RECT *RectBuf )
{
	int H;

	H = GetToolBarHeight();
	*RectBuf = WinData.WindowRect ;
	RectBuf->top    += H ;
	RectBuf->bottom += H ;
	
	return 0 ;
}

// �E�C���h�E�̃A�N�e�B�u�t���O���擾
extern int NS_GetWindowActiveFlag( void )
{
	return WinData.ActiveFlag ;
}

// ���C���E�C���h�E���ŏ�������Ă��邩�ǂ������擾����( �߂�l  TRUE:�ŏ�������Ă���  FALSE:�ŏ�������Ă��Ȃ� )
extern int NS_GetWindowMinSizeFlag( void )
{
	return WinData.WindowMinSizeFlag ;
}

// ���C���E�C���h�E���ő剻����Ă��邩�ǂ������擾����( �߂�l  TRUE:�ő剻����Ă���  FALSE:�ő剻����Ă��Ȃ� )
extern int NS_GetWindowMaxSizeFlag( void )
{
	return WinData.WindowMaximizeFlag ;
}

// ���C���E�C���h�E�̃n���h�����擾����
extern HWND NS_GetMainWindowHandle( void )
{
	return WinData.MainWindow ;
}

// �E�C���h�E���[�h�ŋN�����Ă��邩�A�̃t���O���擾����
extern int NS_GetWindowModeFlag( void )
{
	return WinData.WindowModeFlag ;
}

// �N�����̃f�X�N�g�b�v�̉�ʃ��[�h���擾����
extern int NS_GetDefaultState( int *SizeX, int *SizeY, int *ColorBitDepth, int *RefreshRate , int *LeftTopX, int *LeftTopY )
{
#ifdef DX_NON_GRAPHICS
	if( SizeX ) *SizeX = GetSystemMetrics( SM_CXSCREEN ) ;
	if( SizeY ) *SizeY = GetSystemMetrics( SM_CYSCREEN ) ;
	if( ColorBitDepth || RefreshRate )
	{
		HDC hdc ;

		hdc = GetDC( NULL ) ;
		if( ColorBitDepth ) *ColorBitDepth = GetDeviceCaps( hdc , PLANES ) * GetDeviceCaps( hdc , BITSPIXEL ) ;
		if( RefreshRate )   *RefreshRate   = GetDeviceCaps( hdc , VREFRESH ) ;
		ReleaseDC( NULL , hdc ) ;
	}

#else // DX_NON_GRAPHICS

	GRAPHICSSYS_DISPLAYINFO *DisplayInfo ;

	if( GSYS.Screen.DisplayInfo == NULL )
	{
		Graphics_SetupDisplayInfo_PF() ;
	}

	DisplayInfo = &GSYS.Screen.DisplayInfo[ GSYS.Screen.ValidUseDisplayIndex ? GSYS.Screen.UseDisplayIndex : 0 ] ;

	if( SizeX )
	{
		*SizeX         = DisplayInfo->DesktopSizeX ;
	}

	if( SizeY )
	{
		*SizeY         = DisplayInfo->DesktopSizeY ;
	}

	if( ColorBitDepth )
	{
		*ColorBitDepth = DisplayInfo->DesktopColorBitDepth ;
	}

	if( RefreshRate )
	{
		*RefreshRate   = DisplayInfo->DesktopRefreshRate ;
	}

	if( LeftTopX )
	{
		*LeftTopX      = DisplayInfo->DesktopRect.left ;
	}

	if( LeftTopY )
	{
		*LeftTopY      = DisplayInfo->DesktopRect.top ;
	}
#endif // DX_NON_GRAPHICS

	// �I��
	return 0 ;
}

// �\�t�g���A�N�e�B�u���ǂ������擾����
extern int NS_GetActiveFlag( void )
{
	return WinData.ActiveFlag ;
}

// ��A�N�e�B�u�ɂȂ�A�������ꎞ��~���Ă������ǂ������擾����(���� ResetFlag=TRUE:��Ԃ����Z�b�g FALSE:��Ԃ����Z�b�g���Ȃ�    �߂�l: 0=�ꎞ��~�͂��Ă��Ȃ�  1=�ꎞ��~���Ă��� )
extern int NS_GetNoActiveState( int ResetFlag )
{
	int Return;

	Return = WinData.WaitTimeValidFlag ;
	if( ResetFlag == TRUE )
	{
		WinData.WaitTimeValidFlag = FALSE ;
	}

	return Return ;
}

// �}�E�X��\�����邩�ǂ����̃t���O���擾����
extern int NS_GetMouseDispFlag( void )
{
	return WinData.MouseDispFlag ;
}

// �E�C���h�E���A�N�e�B�u�ł͂Ȃ���Ԃł������𑱍s���邩�A�t���O���擾����
extern int NS_GetAlwaysRunFlag( void )
{
	return WinData.NonActiveRunFlag ;
}

// �c�w���C�u������ DirectX �̃o�[�W������ Windows �̃o�[�W�����𓾂�
extern int NS__GetSystemInfo( int *DxLibVer, int *DirectXVer, int *WindowsVer )
{
	if( DxLibVer ) *DxLibVer = DXLIB_VERSION ;
	if( DirectXVer ) *DirectXVer = WinData.DirectXVersion ;
	if( WindowsVer ) *WindowsVer = WinData.WindowsVersion ;

	// �I��
	return 0 ;
}

// �o�b�̏��𓾂�
extern int NS_GetPcInfo(
	TCHAR	*OSString,
	TCHAR	*DirectXString,
	TCHAR	*CPUString,
	int		*CPUSpeed/*�P��MHz*/,
	double	*FreeMemorySize/*�P��MByte*/,
	double	*TotalMemorySize,
	TCHAR	*VideoDriverFileName,
	TCHAR	*VideoDriverString,
	double	*FreeVideoMemorySize/*�P��MByte*/,
	double	*TotalVideoMemorySize
)
{
#ifdef UNICODE
	return GetPcInfo_WCHAR_T(
		OSString,
		DirectXString,
		CPUString,
		CPUSpeed/*�P��MHz*/,
		FreeMemorySize/*�P��MByte*/,
		TotalMemorySize,
		VideoDriverFileName,
		VideoDriverString,
		FreeVideoMemorySize/*�P��MByte*/,
		TotalVideoMemorySize
	) ;
#else
	int Result ;
	wchar_t OSStringBuffer[ 256 ] ;
	wchar_t DirectXStringBuffer[ 256 ] ;
	wchar_t CPUStringBuffer[ 256 ] ;
	wchar_t VideoDriverFileNameBuffer[ 256 ] ;
	wchar_t VideoDriverStringBuffer[ 256 ] ;

	Result = GetPcInfo_WCHAR_T(
		OSStringBuffer,
		DirectXStringBuffer,
		CPUStringBuffer,
		CPUSpeed/*�P��MHz*/,
		FreeMemorySize/*�P��MByte*/,
		TotalMemorySize,
		VideoDriverFileNameBuffer,
		VideoDriverStringBuffer,
		FreeVideoMemorySize/*�P��MByte*/,
		TotalVideoMemorySize
	) ;

	if( OSString            != NULL ) ConvString( ( const char * )OSStringBuffer,            WCHAR_T_CHARCODEFORMAT, ( char * )OSString,            BUFFERBYTES_CANCEL, _TCHARCODEFORMAT ) ;
	if( DirectXString       != NULL ) ConvString( ( const char * )DirectXStringBuffer,       WCHAR_T_CHARCODEFORMAT, ( char * )DirectXString,       BUFFERBYTES_CANCEL, _TCHARCODEFORMAT ) ;
	if( CPUString           != NULL ) ConvString( ( const char * )CPUStringBuffer,           WCHAR_T_CHARCODEFORMAT, ( char * )CPUString,           BUFFERBYTES_CANCEL, _TCHARCODEFORMAT ) ;
	if( VideoDriverFileName != NULL ) ConvString( ( const char * )VideoDriverFileNameBuffer, WCHAR_T_CHARCODEFORMAT, ( char * )VideoDriverFileName, BUFFERBYTES_CANCEL, _TCHARCODEFORMAT ) ;
	if( VideoDriverString   != NULL ) ConvString( ( const char * )VideoDriverStringBuffer,   WCHAR_T_CHARCODEFORMAT, ( char * )VideoDriverString,   BUFFERBYTES_CANCEL, _TCHARCODEFORMAT ) ;

	return Result ;
#endif
}

// �o�b�̏��𓾂�
extern int GetPcInfo_WCHAR_T(
	wchar_t	*OSString,
	wchar_t	*DirectXString,
	wchar_t	*CPUString,
	int		*CPUSpeed/*�P��MHz*/,
	double	*FreeMemorySize/*�P��MByte*/,
	double	*TotalMemorySize,
	wchar_t	*VideoDriverFileName,
	wchar_t	*VideoDriverString,
	double	*FreeVideoMemorySize/*�P��MByte*/,
	double	*TotalVideoMemorySize
)
{
	if( OSString				!= NULL )	_WCSCPY( OSString,				WinData.PcInfo.OSString ) ;
	if( DirectXString			!= NULL )	_WCSCPY( DirectXString,			WinData.PcInfo.DirectXString ) ;
	if( CPUString				!= NULL )	_WCSCPY( CPUString,				WinData.PcInfo.CPUString ) ;
	if( CPUSpeed				!= NULL )	*CPUSpeed				= WinData.PcInfo.CPUSpeed ;
	if( FreeMemorySize			!= NULL )	*FreeMemorySize			= ( double )WinData.PcInfo.FreeMemorySize  / 0x100000 ;
	if( TotalMemorySize			!= NULL )	*TotalMemorySize		= ( double )WinData.PcInfo.TotalMemorySize / 0x100000 ;
	if( VideoDriverFileName		!= NULL )	_WCSCPY( VideoDriverFileName,	WinData.PcInfo.VideoDriverFileName ) ;
	if( VideoDriverString		!= NULL )	_WCSCPY( VideoDriverString,		WinData.PcInfo.VideoDriverString ) ;
	if( FreeVideoMemorySize		!= NULL )	*FreeVideoMemorySize	= ( double )WinData.PcInfo.VideoFreeMemorySize  / 0x100000 ;
	if( TotalVideoMemorySize	!= NULL )	*TotalVideoMemorySize	= ( double )WinData.PcInfo.VideoTotalMemorySize / 0x100000 ;

	return 0 ;
}

// �l�l�w���g���邩�ǂ����̏��𓾂�
extern int NS_GetUseMMXFlag( void ) 
{
	return WinData.UseMMXFlag ;
}

// �r�r�d���g���邩�ǂ����̏��𓾂�
extern int NS_GetUseSSEFlag( void )
{
	return WinData.UseSSEFlag ;
}

// �r�r�d�Q���g���邩�ǂ����̏��𓾂�
extern int NS_GetUseSSE2Flag( void )
{
	return WinData.UseSSE2Flag ;
}

// �E�C���h�E����悤�Ƃ��Ă��邩�̏��𓾂�
extern int NS_GetWindowCloseFlag( void )
{
	return WinData.CloseMessagePostFlag ;
}

// �\�t�g�̃C���X�^���X���擾����
extern HINSTANCE NS_GetTaskInstance( void )
{
	return WinData.Instance ;
}

// ���[�W�������g���Ă��邩�ǂ������擾����
extern int NS_GetUseWindowRgnFlag( void )
{
	return WinData.WindowRgn != NULL ;
}

// �E�C���h�E�̃T�C�Y��ύX�ł��邩�ǂ����̃t���O���擾����
extern int NS_GetWindowSizeChangeEnableFlag( int *FitScreen )
{
	if( FitScreen ) *FitScreen = WinData.ScreenNotFitWindowSize == TRUE ? FALSE : TRUE ;
	return WinData.WindowSizeChangeEnable ;
}

// �`���ʂ̃T�C�Y�ɑ΂���E�C���h�E�T�C�Y�̔䗦���擾����
extern double NS_GetWindowSizeExtendRate( double *ExRateX, double *ExRateY )
{
	if( WinData.WindowSizeExRateX <= 0.0 )
		WinData.WindowSizeExRateX = 1.0 ;

	if( WinData.WindowSizeExRateY <= 0.0 )
		WinData.WindowSizeExRateY = 1.0 ;

	// �E�C���h�E���[�h���ǂ����ŏ����𕪊�
	if( WinData.WindowModeFlag )
	{
#ifndef DX_NON_GRAPHICS
		// �ő剻��Ԃ̏ꍇ���ǂ����ŏ����𕪊�
		if( WinData.WindowMaximizeFlag &&
			WinData.ScreenNotFitWindowSize == FALSE &&
			WinData.WindowSizeValid == FALSE )
		{
			int Width ;
			int Height ;
			int MaxCWidth ;
			int MaxCHeight ;
			double MaxCExRateX ;
			double MaxCExRateY ;

			// ��ʂ̃T�C�Y���擾
			NS_GetDrawScreenSize( &Width, &Height ) ;

			// �ő剻��Ԃ̃N���C�A���g�̈�̃T�C�Y���擾����
			MaxCWidth  = WinData.WindowMaximizedClientRect.right  - WinData.WindowMaximizedClientRect.left ;
			MaxCHeight = WinData.WindowMaximizedClientRect.bottom - WinData.WindowMaximizedClientRect.top ;

			// �v�Z�덷�Ή��̂��߂P�h�b�g�����������T�C�Y�ɂ���
			MaxCWidth  -= 1 ;
			MaxCHeight -= 1 ;

			// �{�����Z�o����
			MaxCExRateX = ( double )MaxCWidth  / Width ;
			MaxCExRateY = ( double )MaxCHeight / Height ;

			// �{���̏������ق��ɍ��킹��
			if( MaxCExRateX < MaxCExRateY )
			{
				MaxCExRateY = MaxCExRateX ;
			}
			else
			{
				MaxCExRateX = MaxCExRateY ;
			}

			if( ExRateX ) *ExRateX = MaxCExRateX ;
			if( ExRateY ) *ExRateY = MaxCExRateY ;
			return MaxCExRateX ;
		}
		else
#endif // DX_NON_GRAPHICS
		{
			if( ExRateX ) *ExRateX = WinData.WindowSizeExRateX ;
			if( ExRateY ) *ExRateY = WinData.WindowSizeExRateY ;
			return WinData.WindowSizeExRateX ;
		}
	}
	else
	{
		if( ExRateX ) *ExRateX = 1.0 ;
		if( ExRateY ) *ExRateY = 1.0 ;
		return 1.0 ;
	}
}

// �E�C���h�E���[�h�̃E�C���h�E�̃N���C�A���g�̈�̃T�C�Y���擾����
extern int NS_GetWindowSize( int *Width, int *Height )
{
	RECT Rect ;

	GetClientRect( GetDisplayWindowHandle(), &Rect ) ;
	if( Width  ) *Width  = Rect.right  - Rect.left ;
	if( Height ) *Height = Rect.bottom - Rect.top  ;

	// �I��
	return 0 ;
}

// �E�C���h�E���[�h�̃E�C���h�E�̈ʒu���擾����( �g���܂߂�������W )
extern int NS_GetWindowPosition( int *x, int *y )
{
	RECT Rect ;

	GetWindowRect( WinData.MainWindow, &Rect ) ;
	if( x ) *x = Rect.left ;
	if( y ) *y = Rect.top ;

	// �I��
	return 0 ;
}


// �E�C���h�E�̕���{�^���������ꂽ���ǂ������擾����
extern int NS_GetWindowUserCloseFlag( int StateResetFlag )
{
	int Result ;

	Result = WinData.AltF4_EndFlag == 4 ? TRUE : FALSE ;

	// ��Ԃ����Z�b�g����t���O�������Ă�����t���O�����Z�b�g����
	if( StateResetFlag == TRUE && WinData.AltF4_EndFlag == 4 && WinData.NonUserCloseEnableFlag == TRUE )
	{
		WinData.AltF4_EndFlag = FALSE ;
	}

	return Result ;
}

// WM_PAINT ���b�Z�[�W���������ǂ������擾����
// (�߂�l   TRUE:WM_PAINT���b�Z�[�W������(��x�擾����ƈȌ�A�Ă� WM_PAINT���b�Z�[�W������܂� FALSE ���Ԃ��Ă���悤�ɂȂ�)
//          FALSE:WM_PAINT ���b�Z�[�W�͗��Ă��Ȃ�)
extern int NS_GetPaintMessageFlag( void )
{
	int Result;

	Result = WinData.RecvWM_PAINTFlag;
	WinData.RecvWM_PAINTFlag = FALSE;

	return Result;
}

// �p�t�H�[�}���X�J�E���^���L�����ǂ������擾����(�߂�l  TRUE:�L��  FALSE:����)
extern int NS_GetValidHiPerformanceCounter( void )
{
	return WinData.PerformanceTimerFlag;
}

// ���͂��ꂽ�V�X�e���������擾����
extern TCHAR NS_GetInputSystemChar( int DeleteFlag )
{
#ifdef UNICODE
	return GetInputSystemChar_WCHAR_T( DeleteFlag ) ;
#else
	wchar_t Result ;

	Result = GetInputSystemChar_WCHAR_T( DeleteFlag ) ;

	return ( TCHAR )ConvCharCode( ( DWORD )Result, WCHAR_T_CHARCODEFORMAT, _TCHARCODEFORMAT ) ;
#endif
}

// ���͂��ꂽ�V�X�e���������擾����
extern wchar_t GetInputSystemChar_WCHAR_T( int DeleteFlag )
{
	wchar_t Result ;

	Result = WinData.InputSysChara ;

	if( DeleteFlag != FALSE )
	{
		WinData.InputSysChara = L'\0' ;
	}

	return Result ;
}












// Aero �̗L���A�����ݒ�
extern int SetEnableAero( int Flag )
{
	// �t���O�������������牽�����Ȃ�
	if( WinData.AeroDisableFlag == !Flag )
		return 0 ;

	if( WinAPIData.DF_DwmEnableComposition )
	{
		WinAPIData.DF_DwmEnableComposition( ( UINT )Flag ) ;
	}

	// �t���O��ۑ�
	WinData.AeroDisableFlag = !Flag ;

	// �I��
	return 0 ;
}







// �ݒ�֌W�֐�

// �E�C���h�E���[�h�ŋN�����邩���Z�b�g
extern int	SetWindowModeFlag( int Flag )
{
	// �t���O��ۑ�
	WinData.WindowModeFlag = Flag ;

	if( Flag ) 
	{
		DXST_ERRORLOG_ADDUTF16LE( "\xa6\x30\xa4\x30\xf3\x30\xc9\x30\xa6\x30\xe2\x30\xfc\x30\xc9\x30\xd5\x30\xe9\x30\xb0\x30\x4c\x30\xcb\x7a\x66\x30\x89\x30\x8c\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�E�C���h�E���[�h�t���O�����Ă��܂���\n" @*/ ) ;

		// ���j���[�����݂��鎞�̓��j���[���Z�b�g����
		if( WinData.MenuUseFlag == TRUE ) 
		{
			SetMenu( WinData.MainWindow, WinData.Menu ) ;
		}
	}
	else
	{
		DXST_ERRORLOG_ADDUTF16LE( "\xa6\x30\xa4\x30\xf3\x30\xc9\x30\xa6\x30\xe2\x30\xfc\x30\xc9\x30\xd5\x30\xe9\x30\xb0\x30\x4c\x30\x12\x50\x55\x30\x8c\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�E�C���h�E���[�h�t���O���|����܂���\n" @*/ ) ;

		// ���j���[�����݂��鎞�̓��j���[���O��
		if( WinData.MenuUseFlag == TRUE ) 
		{
			SetMenu( WinData.MainWindow, NULL ) ;
		}
	}

	// �I��
	return 0 ;
}

// �E�C���h�E�̃X�^�C�����Z�b�g����
extern int SetWindowStyle( int CenterPosition )
{
	RECT Rect ;
	RECT PrevWindowRect ;
	int WindowSizeX, WindowSizeY ;
	int ClientRectWidth, ClientRectHeight ;

	if( WinData.MainWindow == NULL || WinData.UserWindowFlag == TRUE ) return 0 ;

	// ���݂̃E�C���h�E�̈���擾
	GetWindowRect( WinData.MainWindow, &PrevWindowRect ) ;

	// �N���C�A���g�̈�̃T�C�Y��ۑ�
	GetClientRect( WinData.MainWindow, &Rect ) ;
	ClientRectWidth  = Rect.right  - Rect.left ;
	ClientRectHeight = Rect.bottom - Rect.top ;

	// ���[�U�[�񋟂̃E�C���h�E��������p�����[�^�擾�ȊO�͉������Ȃ�
	if( WinData.UserWindowFlag == TRUE )
	{
		// �E�C���h�E�̃N���C�A���g�̈��ۑ�����
		GetClientRect( WinData.MainWindow, &WinData.WindowRect )  ;
		ClientToScreen( WinData.MainWindow, ( LPPOINT )&WinData.WindowRect ) ;
		ClientToScreen( WinData.MainWindow, ( LPPOINT )&WinData.WindowRect + 1 ) ;

		// �E�C���h�E�X�^�C����ύX
		if( WinData.WindowModeFlag == TRUE )	NS_SetMouseDispFlag( TRUE ) ;
		else									NS_SetMouseDispFlag( FALSE ) ;
	}
	else
	{
		// �`��̈�̃T�C�Y���擾
		GetMainWindowSize( &WindowSizeX, &WindowSizeY ) ;

		// �E�C���h�E�X�^�C����ύX
		if( WinData.WindowModeFlag == TRUE )
		{
			LONG AddStyle, AddExStyle ;
			double ExtendRateX, ExtendRateY ;

			// �E�C���h�E���[�h�̏ꍇ

			DXST_ERRORLOG_ADDUTF16LE( "\xa6\x30\xa4\x30\xf3\x30\xc9\x30\xa6\x30\xb9\x30\xbf\x30\xa4\x30\xeb\x30\x92\x30\xa6\x30\xa4\x30\xf3\x30\xc9\x30\xa6\x30\xe2\x30\xfc\x30\xc9\x30\x28\x75\x6b\x30\x09\x59\xf4\x66\x57\x30\x7e\x30\x59\x30\x2e\x00\x2e\x00\x2e\x00\x20\x00\x00"/*@ L"�E�C���h�E�X�^�C�����E�C���h�E���[�h�p�ɕύX���܂�... " @*/ ) ;

			AddExStyle = 0 ;
			//if( WinData.NotWindowVisibleFlag == TRUE ) AddExStyle |= WS_EX_TRANSPARENT ;

			// �o�b�N�o�b�t�@�̓��ߐF�̕����𓧉߂�����t���O�������Ă��邩�A
			// UpdateLayerdWindow ���g�p����t���O���o���Ă���ꍇ�� WS_EX_LAYERED ��ǉ�����
			if( WinData.BackBufferTransColorFlag == TRUE ||
				WinData.UseUpdateLayerdWindowFlag == TRUE ) AddExStyle |= WS_EX_LAYERED ;

			AddStyle = 0 ;
			// �񓯊��E�C���h�E���[�h�ύX�@�\���L���ɂȂ��Ă���Ƃ��� WS_MAXIMIZEBOX ��ǉ�����
			if( WinData.UseChangeWindowModeFlag == TRUE ) AddStyle |= WS_MAXIMIZEBOX ;

			// �\���t���O�������Ă���ꍇ�� WS_VISIBLE ��ǉ�����
			if( WinData.VisibleFlag == TRUE ) AddStyle |= WS_VISIBLE ;

			// �E�C���h�E�T�C�Y���ύX�ł���悤�ɂȂ��Ă���ꍇ�� WS_THICKFRAME ��ǉ�����
			if( WinData.WindowSizeChangeEnable == TRUE ) AddStyle |= WS_THICKFRAME ;

			// �X�^�C���̕ύX
			SetWindowLongW( WinData.MainWindow , GWL_EXSTYLE , ( LONG )( WExStyle_WindowModeTable[ WinData.WindowStyle ] + AddExStyle ) ) ;
			SetWindowLongW( WinData.MainWindow , GWL_STYLE   , ( LONG )( WStyle_WindowModeTable[ WinData.WindowStyle ]   + AddStyle   ) ) ;

			// �N���C�A���g�̈�̎Z�o
			if( WinData.WindowSizeValid == TRUE )
			{
				WindowSizeX = WinData.WindowWidth ;
				WindowSizeY = WinData.WindowHeight ;
			}
			else
			if( WinData.ScreenNotFitWindowSize == TRUE )
			{
				WindowSizeX = ClientRectWidth ;
				WindowSizeY = ClientRectHeight ;
			}
			else
			{
				NS_GetWindowSizeExtendRate( &ExtendRateX, &ExtendRateY ) ;
				WindowSizeX = _DTOL( WindowSizeX * ExtendRateX ) ;
				WindowSizeY = _DTOL( WindowSizeY * ExtendRateY ) + GetToolBarHeight();
			}
			GetToolBarHeight();

			// �ʒu��ύX
			SETRECT( Rect, 0, 0, WindowSizeX, WindowSizeY ) ;
			AdjustWindowRectEx( &Rect,
								WStyle_WindowModeTable[WinData.WindowStyle] + AddStyle, FALSE,
								WExStyle_WindowModeTable[WinData.WindowStyle] + AddExStyle );

			// �ݒ�̈�����傢�ƍ׍H
	//		Rect.left   += - 3;
	//		Rect.top    += - 3;
	//		Rect.right  += + 3;
	//		Rect.bottom += + 3;

			WindowSizeX = Rect.right  - Rect.left ;
			WindowSizeY = Rect.bottom - Rect.top  ;
			if( CenterPosition )
			{
				int DesktopW, DesktopH, DesktopX, DesktopY ;

				NS_GetDefaultState( &DesktopW, &DesktopH, NULL, NULL, &DesktopX, &DesktopY ) ;

				// ���S�Ɏ����Ă���
				Rect.left   += ( DesktopW - WindowSizeX ) / 2 + DesktopX ;
				Rect.top    += ( DesktopH - WindowSizeY ) / 2 + DesktopY ;
				Rect.right  += ( DesktopW - WindowSizeX ) / 2 + DesktopX ;
				Rect.bottom += ( DesktopH - WindowSizeY ) / 2 + DesktopY ;
			}
			else
			{
				// ���̈ʒu�ɃZ�b�g����
				Rect.left   = PrevWindowRect.left ;
				Rect.top    = PrevWindowRect.top ;
				Rect.right  = Rect.left + WindowSizeX ;
				Rect.bottom = Rect.top  + WindowSizeY ;
			}

			// �E�C���h�E�̈ʒu�ƃT�C�Y��ύX
			if( WinData.WindowZType == DX_WIN_ZTYPE_NORMAL )
			{
				SetWindowPos( WinData.MainWindow,
								HWND_NOTOPMOST,
								WinData.WindowPosValid == TRUE ? WinData.WindowX : Rect.left,
								WinData.WindowPosValid == TRUE ? WinData.WindowY : Rect.top,
								WindowSizeX,
								WindowSizeY,
								0/*SWP_NOZORDER*/ );
				SetWindowPos( WinData.MainWindow,
								HWND_TOP,
								WinData.WindowPosValid == TRUE ? WinData.WindowX : Rect.left,
								WinData.WindowPosValid == TRUE ? WinData.WindowY : Rect.top,
								WindowSizeX,
								WindowSizeY,
								0/*SWP_NOZORDER*/ );
			}
			else
			{
				SetWindowPos( WinData.MainWindow,
								WindowZType_Win32Param[ WinData.WindowZType ],
								WinData.WindowPosValid == TRUE ? WinData.WindowX : Rect.left,
								WinData.WindowPosValid == TRUE ? WinData.WindowY : Rect.top,
								WindowSizeX,
								WindowSizeY,
								0/*SWP_NOZORDER*/ );
			}

			// �E�C���h�E�̃N���C�A���g�̈��ۑ�����
			GetClientRect( WinData.MainWindow , &WinData.WindowRect )  ;
			ClientToScreen( WinData.MainWindow , ( LPPOINT )&WinData.WindowRect ) ;
			ClientToScreen( WinData.MainWindow , ( LPPOINT )&WinData.WindowRect + 1 ) ;

			// �␳
			if( WinData.WindowRect.left < 0 || WinData.WindowRect.top < 0 )
			{
				int left, top ;

				if( WinData.WindowRect.left < 0 )
				{
					left = -WinData.WindowRect.left ;
					Rect.left   += left ;
					Rect.right  += left ;
					WinData.WindowRect.left   += left ;
					WinData.WindowRect.right  += left ;
				}

				if( WinData.WindowRect.top < 0 )
				{
					top  = -WinData.WindowRect.top  ;
					Rect.top    += top  ;
					Rect.bottom += top  ;
					WinData.WindowRect.top    += top  ;
					WinData.WindowRect.bottom += top  ;
				}

				SetWindowPos( WinData.MainWindow,
								WindowZType_Win32Param[ WinData.WindowZType ],
								WinData.WindowPosValid == TRUE ? WinData.WindowX : Rect.left,
								WinData.WindowPosValid == TRUE ? WinData.WindowY : Rect.top,
								WindowSizeX,
								WindowSizeY,
								0/*SWP_NOZORDER*/ );
			}
	
			NS_SetMouseDispFlag( TRUE ) ;
			DXST_ERRORLOG_ADDUTF16LE( "\x8c\x5b\x86\x4e\x0a\x00\x00"/*@ L"����\n" @*/ ) ;
		}
		else
		{
			DXST_ERRORLOG_ADDUTF16LE( "\xa6\x30\xa4\x30\xf3\x30\xc9\x30\xa6\x30\xb9\x30\xbf\x30\xa4\x30\xeb\x30\x92\x30\xd5\x30\xeb\x30\xb9\x30\xaf\x30\xea\x30\xfc\x30\xf3\x30\xe2\x30\xfc\x30\xc9\x30\x28\x75\x6b\x30\x09\x59\xf4\x66\x57\x30\x7e\x30\x59\x30\x2e\x00\x2e\x00\x2e\x00\x20\x00\x00"/*@ L"�E�C���h�E�X�^�C�����t���X�N���[�����[�h�p�ɕύX���܂�... " @*/ ) ;

			// �t���X�N���[�����[�h�̏ꍇ
			SetWindowLongW( WinData.MainWindow, GWL_EXSTYLE, ( LONG )( WExStyle_FullScreenModeTable[WinData.WindowStyle] ) ) ;
			SetWindowLongW( WinData.MainWindow, GWL_STYLE  , ( LONG )( WStyle_FullScreenModeTable[WinData.WindowStyle]   ) ) ;

			// �E�C���h�E��`��ύX
			SETRECT( WinData.WindowRect, 0, 0, WindowSizeX, WindowSizeY ) ;

			// �E�C���h�E��\��
			ShowWindow( WinData.MainWindow , SW_SHOW ) ;
			UpdateWindow( WinData.MainWindow ) ;

			// �y�I�[�_�[�̈ێ�
			SetWindowPos( WinData.MainWindow, HWND_TOPMOST, 0, 0, WindowSizeX, WindowSizeY, /*SWP_NOSIZE | SWP_NOMOVE |*/ SWP_NOREDRAW ) ; 

			NS_SetMouseDispFlag( FALSE ) ;
			DXST_ERRORLOG_ADDUTF16LE( "\x8c\x5b\x86\x4e\x0a\x00\x00"/*@ L"����\n" @*/ ) ;
		}

		SetActiveWindow( WinData.MainWindow ) ;
	}

	// �}�E�X�̃Z�b�g�M�����o��
	PostMessageW( WinData.MainWindow , WM_SETCURSOR , ( WPARAM )WinData.MainWindow , 0 ) ;

	// �I��
	return 0 ;
}

// ���擾�x����

// �o�b���\���̂̃A�h���X�𓾂�
extern PCINFO *GetPcInfoStructP( void ) 
{
	return &WinData.PcInfo ;
}


// �E�C���h�E���[�h��ύX����
extern int NS_ChangeWindowMode( int Flag )
{
#ifndef DX_NON_GRAPHICS
	int Ret ;

	DXST_ERRORLOG_ADDUTF16LE( "\x43\x00\x68\x00\x61\x00\x6e\x00\x67\x00\x65\x00\x57\x00\x69\x00\x6e\x00\x64\x00\x6f\x00\x77\x00\x4d\x00\x6f\x00\x64\x00\x65\x00\x9f\x5b\x4c\x88\x20\x00\x0a\x00\x00"/*@ L"ChangeWindowMode���s \n" @*/ ) ;

	// ���܂łƓ������[�h�������ꍇ�͂Ȃɂ������I��
	if( Flag == WinData.WindowModeFlag ) return 0 ;

	WinData.VisibleFlag = TRUE ;

	// WM_DISPLAYCHANGE ���b�Z�[�W���������t���O��|��
	WinData.DisplayChangeMessageFlag = FALSE ;

	// �E�C���h�E���[�h�t���O��ύX����
	if( SetWindowModeFlag( Flag ) == -1 ) return -1 ;

	// �܂��E�C���h�E�����쐬����Ă��Ȃ����A���[�U�[���쐬�����E�C���h�E���g�p���Ă���ꍇ�͂����ŏI��
	if( WinData.MainWindow == NULL || WinData.UserWindowFlag == TRUE ) return 0 ;

	// �E�C���h�E���[�h�ύX���t���O�𗧂Ă�
	WinData.ChangeWindodwFlag = TRUE ;

	// �E�C���h�E�X�^�C����ύX����
	SetWindowStyle() ;

	// ��ʃ��[�h��ύX����
	Ret = Graphics_Screen_ChangeMode( -1, -1, -1, TRUE, -1 ) ;

	// �E�C���h�E���[�h�ύX���t���O��|��
	WinData.ChangeWindodwFlag = FALSE ;

	// �t���X�N���[���ɂȂ�������̓��j���[��\�����Ă��Ȃ���Ԃɂ���
	if( Flag == FALSE )
	{
		WinData.MousePosInMenuBarFlag = FALSE ;
	}

	return Ret ;
#else // DX_NON_GRAPHICS
	return -1 ;
#endif // DX_NON_GRAPHICS
}

// �c�w���C�u�����̕����񏈗��őO��Ƃ��镶����Z�b�g��ݒ肷��
extern int NS_SetUseCharSet( int CharSet /* = DX_CHARSET_SHFTJIS �� */ )
{
	switch( CharSet )
	{
	default :
	case DX_CHARSET_DEFAULT :
		_SET_CHARSET( DX_CHARSET_DEFAULT ) ;
		_SET_CHAR_CHARCODEFORMAT( DX_CHARCODEFORMAT_SHIFTJIS ) ;
		break ;

	case DX_CHARSET_SHFTJIS :
		_SET_CHARSET( DX_CHARSET_SHFTJIS ) ;
		_SET_CHAR_CHARCODEFORMAT( DX_CHARCODEFORMAT_SHIFTJIS ) ;
		break ;

	case DX_CHARSET_HANGEUL :
		_SET_CHARSET( DX_CHARSET_HANGEUL ) ;
		_SET_CHAR_CHARCODEFORMAT( DX_CHARCODEFORMAT_UHC ) ;
		break ;

	case DX_CHARSET_BIG5 :
		_SET_CHARSET( DX_CHARSET_BIG5 ) ;
		_SET_CHAR_CHARCODEFORMAT( DX_CHARCODEFORMAT_BIG5 ) ;
		break ;

	case DX_CHARSET_GB2312 :
		_SET_CHARSET( DX_CHARSET_GB2312 ) ;
		_SET_CHAR_CHARCODEFORMAT( DX_CHARCODEFORMAT_GB2312 ) ;
		break ;

	case DX_CHARSET_WINDOWS_1252 :
		_SET_CHARSET( DX_CHARSET_WINDOWS_1252 ) ;
		_SET_CHAR_CHARCODEFORMAT( DX_CHARCODEFORMAT_WINDOWS_1252 ) ;
		break ;

	case DX_CHARSET_ISO_IEC_8859_15 :
		_SET_CHARSET( DX_CHARSET_ISO_IEC_8859_15 ) ;
		_SET_CHAR_CHARCODEFORMAT( DX_CHARCODEFORMAT_ISO_IEC_8859_15 ) ;
		break ;
	}

	// �I��
	return 0 ;
}

// �A�N�e�B�u�E�C���h�E�����̃\�t�g�Ɉڂ��Ă���ۂɕ\������摜�̃��[�h(NULL �ŉ���)
static int LoadPauseGraphToBase( const wchar_t *FileName, const void *MemImage, int MemImageSize )
{
#ifndef DX_NON_GRAPHICS
	BASEIMAGE RgbBaseImage ;

	// �摜�f�[�^�̏�񂪂Ȃ������牽�������I��
	if( FileName == NULL && MemImage == NULL ) return 0 ;

	// �摜�̃��[�h
	if( FileName != NULL )
	{
		if( CreateGraphImage_plus_Alpha_WCHAR_T( FileName, NULL, 0, LOADIMAGE_TYPE_FILE,
													  NULL, 0, LOADIMAGE_TYPE_FILE,
											&RgbBaseImage, NULL, FALSE ) < 0 )
		{
			return -1 ;
		}
	}
	else
	{
		if( CreateGraphImage_plus_Alpha_WCHAR_T( NULL, MemImage, MemImageSize, LOADIMAGE_TYPE_MEM, 
												  NULL, 	0,            LOADIMAGE_TYPE_MEM,
											&RgbBaseImage, NULL, FALSE ) < 0 )
		{
			return -1 ;
		}
	}

	// ���łɃO���t�B�b�N������ꍇ�͔j��
	if( WinData.PauseGraph.GraphData != NULL )
	{
		NS_ReleaseGraphImage( &WinData.PauseGraph ) ;
		NS_DeleteGraph( WinData.PauseGraphHandle ) ;
	}

	// �V�����摜�f�[�^�̃Z�b�g
	WinData.PauseGraph = RgbBaseImage ;

	// �O���t�B�b�N�n���h�����쐬����
	WinData.PauseGraphHandle = Graphics_Image_CreateGraphFromGraphImageBase( &RgbBaseImage, NULL, TRUE, FALSE ) ;
/*
	// ���łɃO���t�B�b�N������ꍇ�͔j��
	if( WinData.PauseGraph )
	{
		DeleteObject( ( HGDIOBJ )WinData.PauseGraph ) ;
		WinData.PauseGraph = 0 ;
	}


	if( FileName != NULL )
	{
		WinData.PauseGraph = NS_CreateDIBGraphVer2( FileName, NULL, 0, LOADIMAGE_TYPE_FILE, FALSE, NULL ) ;
	}
	else
	{
		WinData.PauseGraph = NS_CreateDIBGraphVer2( NULL, MemImage, MemImageSize, LOADIMAGE_TYPE_MEM, FALSE, NULL ) ;
	}
*/

	// �I��
	return 0 ;
#else // DX_NON_GRAPHICS
	return -1 ;
#endif // DX_NON_GRAPHICS
}

// �A�N�e�B�u�E�C���h�E�����̃\�t�g�Ɉڂ��Ă���ۂɕ\������摜�̃��[�h(NULL �ŉ���)
extern int NS_LoadPauseGraph( const TCHAR *FileName )
{
#ifdef UNICODE
	return LoadPauseGraph_WCHAR_T( FileName ) ;
#else
	int Result ;

	TCHAR_TO_WCHAR_T_STRING_ONE_BEGIN( FileName, return -1 )

	Result = LoadPauseGraph_WCHAR_T( UseFileNameBuffer ) ;

	TCHAR_TO_WCHAR_T_STRING_END( FileName )

	return Result ;
#endif
}

// �A�N�e�B�u�E�C���h�E�����̃\�t�g�Ɉڂ��Ă���ۂɕ\������摜�̃��[�h(NULL �ŉ���)
extern int LoadPauseGraph_WCHAR_T( const wchar_t *FileName )
{
	return LoadPauseGraphToBase( FileName, NULL, 0 ) ;
}

// �A�N�e�B�u�E�C���h�E�����̃\�t�g�Ɉڂ��Ă���ۂɕ\������摜�̃��[�h(NULL �ŉ���)
extern int NS_LoadPauseGraphFromMem( const void *MemImage, int MemImageSize )
{
	return LoadPauseGraphToBase( NULL, MemImage, MemImageSize ) ;
}

// �E�C���h�E�̃A�N�e�B�u��Ԃɕω����������Ƃ��ɌĂ΂��R�[���o�b�N�֐����Z�b�g����( NULL ���Z�b�g����ƌĂ΂�Ȃ��Ȃ� )
extern int NS_SetActiveStateChangeCallBackFunction( int (*CallBackFunction)( int ActiveState, void *UserData ), void *UserData )
{
	// �|�C���^��ۑ�
	WinData.ActiveStateChangeCallBackFunction = CallBackFunction ;
	WinData.ActiveStateChangeCallBackFunctionData = UserData ;

	// �I��
	return 0 ;
}

// ���C���E�C���h�E�̃E�C���h�E�e�L�X�g��ύX����
extern int NS_SetWindowText( const TCHAR *WindowText )
{
	return NS_SetMainWindowText( WindowText ) ;
}

// ���C���E�C���h�E�̃E�C���h�E�e�L�X�g��ύX����
extern int NS_SetMainWindowText( const TCHAR *WindowText )
{
#ifdef UNICODE
	return SetMainWindowText_WCHAR_T( WindowText ) ;
#else
	int Result ;

	TCHAR_TO_WCHAR_T_STRING_ONE_BEGIN( WindowText, return -1 )

	Result = SetMainWindowText_WCHAR_T( UseWindowTextBuffer ) ;

	TCHAR_TO_WCHAR_T_STRING_END( WindowText )

	return Result ;
#endif
}

// ���C���E�C���h�E�̃E�C���h�E�e�L�X�g��ύX����
extern int SetMainWindowText_WCHAR_T( const wchar_t *WindowText )
{
	// �e�L�X�g�̕ۑ�
	_WCSCPY_S( WinData.WindowText, sizeof( WinData.WindowText ), WindowText ) ;

	// WindowText �͗L���A�̃t���O�𗧂Ă�
	WinData.EnableWindowText = TRUE ;

	// ���C���E�C���h�E������Ă��Ȃ��ꍇ�͕ۑ��̂ݍs��
	if( WinData.MainWindow )
	{
		// ���C���E�C���h�E�e�L�X�g�̕ύX
		::SetWindowTextW( WinData.MainWindow , WinData.WindowText ) ;
	}

	// �I��
	return 0 ;
}

// ���C���E�C���h�E�̃N���X����ݒ肷��
extern int NS_SetMainWindowClassName( const TCHAR *ClassName )
{
#ifdef UNICODE
	return SetMainWindowClassName_WCHAR_T( ClassName ) ;
#else
	int Result ;

	TCHAR_TO_WCHAR_T_STRING_ONE_BEGIN( ClassName, return -1 )

	Result = SetMainWindowClassName_WCHAR_T( UseClassNameBuffer ) ;

	TCHAR_TO_WCHAR_T_STRING_END( ClassName )

	return Result ;
#endif
}

// ���C���E�C���h�E�̃N���X����ݒ肷��
extern int SetMainWindowClassName_WCHAR_T( const wchar_t *ClassName )
{
	// ���C���E�C���h�E�����ɍ쐬����Ă���ꍇ�͐ݒ�s��
	if( WinData.MainWindow != NULL )
	{
		return -1 ;
	}

	// �e�L�X�g�̕ۑ�
	_WCSCPY_S( WinData.ClassName, sizeof( WinData.ClassName ), ClassName ) ;

	// �I��
	return 0 ;
}

// �E�C���h�E���A�N�e�B�u�ł͂Ȃ���Ԃł������𑱍s���邩�A�t���O���Z�b�g����
extern int NS_SetAlwaysRunFlag( int Flag )
{
	// �t���O���Z�b�g
	WinData.NonActiveRunFlag = Flag ;
	
	// �I��
	return 0 ;
}

// �g�p����A�C�R���̂h�c���Z�b�g����
extern int NS_SetWindowIconID( int ID )
{
	WinData.IconID = ID ;

	// �������ɃE�C���h�E���쐬����Ă�����A�A�C�R����ύX����
	if( WinData.MainWindow != NULL )
	{
#ifdef _WIN64
		SetClassLongPtrW( WinData.MainWindow, GCLP_HICON, ( LONG_PTR )LoadIconW( WinData.Instance , ( WinData.IconID == 0 ) ? ( LPWSTR )IDI_APPLICATION : MAKEINTRESOURCEW( WinData.IconID ) ) ) ;
#else
		SetClassLongW(    WinData.MainWindow,  GCL_HICON, ( LONG_PTR )LoadIconW( WinData.Instance , ( WinData.IconID == 0 ) ? ( LPWSTR )IDI_APPLICATION : MAKEINTRESOURCEW( WinData.IconID ) ) ) ;
#endif
    }
	
	// �I��
	return 0 ;
}

// �g�p����A�C�R���̃n���h�����Z�b�g����
extern int NS_SetWindowIconHandle( HICON Icon )
{
	WinData.IconHandle = Icon ;

	// �������ɃE�C���h�E���쐬����Ă�����A�A�C�R����ύX����
	if( WinData.MainWindow != NULL )
	{
#ifdef _WIN64
		SetClassLongPtrW( WinData.MainWindow, GCLP_HICON, ( LONG_PTR )Icon ) ;
#else
		SetClassLongW( WinData.MainWindow, GCL_HICON, ( LONG_PTR )Icon ) ;
#endif
    }
	
	// �I��
	return 0 ;
}

// �ő剻�{�^����ALT+ENTER�L�[�ɂ��񓯊��ȃE�C���h�E���[�h�̕ύX�̋@�\�̐ݒ���s��
extern int NS_SetUseASyncChangeWindowModeFunction( int Flag, void (*CallBackFunction)(void*), void *Data )
{
	// �t���O�̃Z�b�g
	WinData.UseChangeWindowModeFlag = Flag ;

	// �t���O�ɏ]�����E�C���h�E�̃X�^�C����ݒ肷��
	if( WinData.WindowModeFlag )
		SetWindowStyle() ;

	// �R�[���o�b�N�֐��̕ۑ�
	WinData.ChangeWindowModeCallBackFunction = CallBackFunction ;

	// �R�[���o�b�N�֐��ɓn���f�[�^�̕ۑ�
	WinData.ChangeWindowModeCallBackFunctionData = Data ;

	// �I��
	return 0 ;
}

// �E�C���h�E�̃X�^�C����ύX����
extern int NS_SetWindowStyleMode( int Mode )
{
	if( Mode < 0 || Mode >= WSTYLE_NUM ) return -1 ; 

	// �X�^�C���l�̕ύX
	WinData.WindowStyle = Mode ;

	// �E�C���h�E�̑�����ύX
	if( WinData.WindowModeFlag ) SetWindowStyle() ;

	// �E�C���h�E�̍ĕ`��
	UpdateWindow( WinData.MainWindow ) ;

	// �I��
	return 0 ;
}

// ���C���E�C���h�E�̉��s���ʒu��ύX����
extern int NS_SetWindowZOrder( int ZType )
{
	if( ZType < 0 || ZType > DX_WIN_ZTYPE_TOPMOST ) return -1 ; 

	// ���s���ʒu�^�C�v�̕ۑ�
	WinData.WindowZType = ZType ;

	// �E�C���h�E�̑������Đݒ�
	if( WinData.WindowModeFlag )
	{
		SetWindowStyle( FALSE ) ;
	}

	// �E�C���h�E�̍ĕ`��
	UpdateWindow( WinData.MainWindow ) ;

	// �I��
	return 0 ;
}

// �E�C���h�E�̃T�C�Y��ύX�ł��邩�ǂ����̃t���O���Z�b�g����
// NotFitScreen:�E�C���h�E�̃N���C�A���g�̈�ɉ�ʂ��t�B�b�g������(�g�傳����)���ǂ���  TRUE:�t�B�b�g������  FALSE:�t�B�b�g�����Ȃ�
extern int NS_SetWindowSizeChangeEnableFlag( int Flag, int FitScreen )
{
	int NotFitWindowSize ;

	NotFitWindowSize = FitScreen == TRUE ? FALSE : TRUE ;

	// �t���O�������ꍇ�͉������Ȃ�
	if( WinData.WindowSizeChangeEnable == Flag &&
		WinData.ScreenNotFitWindowSize == NotFitWindowSize ) return 0 ;

	// �t���O��ۑ�
	WinData.WindowSizeChangeEnable = Flag ;
	WinData.ScreenNotFitWindowSize = NotFitWindowSize ;

	// �E�C���h�E�Ƀt�B�b�g������ꍇ�� SetWindowSize �ł̓E�C���h�E�̊g�嗦���ω�����悤�ɂ���
	/*
	// �E�C���h�E�Ƀt�B�b�g������ꍇ�� SetWindowSize �̐ݒ�͖����ɂ���
	if( FitScreen == TRUE )
	{
		WinData.WindowSizeValid = FALSE ;
	}
	*/

	// �E�C���h�E���[�h�̏ꍇ�̂݃E�C���h�E�X�^�C�����X�V
	if( WinData.WindowModeFlag == TRUE )
		SetWindowStyle() ;

	// �I��
	return 0 ;
}

// �`���ʂ̃T�C�Y�ɑ΂���E�C���h�E�T�C�Y�̔䗦��ݒ肷��
extern int NS_SetWindowSizeExtendRate( double ExRateX, double ExRateY )
{
	// ExRateY ���}�C�i�X�̒l�̏ꍇ�� ExRateX �̒l�� ExRateY �ł��g�p����
	if( ExRateY <= 0.0 ) ExRateY = ExRateX ;

	// ���܂łƓ����ꍇ�͉������Ȃ�
	if( WinData.WindowSizeExRateX == ExRateX &&
		WinData.WindowSizeExRateY == ExRateY ) return 0 ;

	WinData.WindowSizeExRateX = ExRateX ;
	WinData.WindowSizeExRateY = ExRateY ;
	if( WinData.WindowSizeExRateX <= 0.0 )
	{
		WinData.WindowSizeExRateX = 1.0 ;
	}
	if( WinData.WindowSizeExRateY <= 0.0 )
	{
		WinData.WindowSizeExRateY = 1.0 ;
	}

	// SetWindowSize �̐ݒ�͖����ɂ���
	WinData.WindowSizeValid = FALSE ;

	// �E�C���h�E���[�h�̏ꍇ�͐V�����䗦�𔽉f����
	if( WinData.WindowModeFlag == TRUE )
	{
		SetWindowStyle() ;
	}

	// �I��
	return 0 ;
}

// �E�C���h�E���[�h���̃E�C���h�E�̃N���C�A���g�̈�̃T�C�Y��ݒ肷��
extern int NS_SetWindowSize( int Width, int Height )
{
#ifndef DX_NON_GRAPHICS
	// �E�C���h�E���쐬�ς݂ŃE�C���h�E�Ƀt�B�b�g������ݒ�̏ꍇ�͊g�嗦��ύX����
	if( WinData.MainWindow != NULL && WinData.ScreenNotFitWindowSize == FALSE )
	{
		double ExtendRateX ;
		double ExtendRateY ;
		int ScreenSizeX ;
		int ScreenSizeY ;

		// �`��̈�̃T�C�Y���擾
		NS_GetDrawScreenSize( &ScreenSizeX , &ScreenSizeY ) ;

		// �g�嗦���w�肷��
		ExtendRateX = ( double )Width  / ScreenSizeX ;
		ExtendRateY = ( double )Height / ScreenSizeY ;
		NS_SetWindowSizeExtendRate( ExtendRateX, ExtendRateY ) ;
	}
	else
#endif // DX_NON_GRAPHICS
	{
		// ����ȊO�̏ꍇ�̓E�C���h�E�T�C�Y��ύX����
		WinData.WindowWidth = Width ;
		WinData.WindowHeight = Height ;
		WinData.WindowSizeValid = TRUE ;

		// ���f������
		WM_SIZEProcess() ;
	}

	// �I��
	return 0 ;
}

// ���C���E�C���h�E�̃N���C�A���g�̈�̍ő�T�C�Y��ݒ肷��( SetWindowSizeChangeEnableFlag �̑������� TRUE �ŌĂяo���āA�E�C���h�E�̃T�C�Y���ύX�ł����Ԃł̂ݎg�p�����p�����[�^�ł� )
extern int NS_SetWindowMaxSize( int MaxWidth, int MaxHeight )
{
	WinData.WindowMaxWidth = MaxWidth ;
	WinData.WindowMaxHeight = MaxHeight ;
	WinData.WindowMaxSizeValid = TRUE ;

	// �I��
	return 0 ;
}

// ���C���E�C���h�E�̃N���C�A���g�̈�̍ŏ��T�C�Y��ݒ肷��( SetWindowSizeChangeEnableFlag �̑������� TRUE �ŌĂяo���āA�E�C���h�E�̃T�C�Y���ύX�ł����Ԃł̂ݎg�p�����p�����[�^�ł� )
extern int NS_SetWindowMinSize( int MinWidth, int MinHeight )
{
	WinData.WindowMinWidth = MinWidth ;
	WinData.WindowMinHeight = MinHeight ;
	WinData.WindowMinSizeValid = TRUE ;

	// �I��
	return 0 ;
}

// �E�C���h�E���[�h�̃E�C���h�E�̈ʒu��ݒ肷��( �g���܂߂�������W )
extern int NS_SetWindowPosition( int x, int y )
{
	WinData.WindowX = x ;
	WinData.WindowY = y ;
	WinData.WindowPosValid = TRUE ;

	// ���f������
	WM_SIZEProcess() ;

	// �I��
	return 0 ;
}
/*
// _KBDLLHOOKSTRUCT �\���̂̒�`
typedef struct tag_KBDLLHOOKSTRUCT
{
    DWORD   vkCode;
    DWORD   scanCode;
    DWORD   flags;
    DWORD   time;
    DWORD   dwExtraInfo;
} _KBDLLHOOKSTRUCT, FAR *LP_KBDLLHOOKSTRUCT, *P_KBDLLHOOKSTRUCT;

// �t�b�N���ꂽ���̃R�[���o�b�N�֐�
LRESULT CALLBACK LowLevelKeyboardProc (INT nCode, WPARAM wParam, LPARAM lParam)
{
    // �t�b�N �v���V�[�W�������[���̒l��Ԃ����Ƃɂ��A
    // ���b�Z�[�W���^�[�Q�b�g �E�B���h�E�ɓn����Ȃ��Ȃ�܂�
    _KBDLLHOOKSTRUCT *pkbhs = (_KBDLLHOOKSTRUCT *) lParam;
    BOOL bControlKeyDown = 0;

	if( WinData.ActiveFlag == TRUE && WinData.SysCommandOffFlag == TRUE )
	{
		switch (nCode)
		{
			case HC_ACTION:
			{
				// Ctrl �L�[�������ꂽ���ǂ������`�F�b�N
				bControlKeyDown = GetAsyncKeyState (VK_CONTROL) >> ((sizeof(SHORT) * 8) - 1);

				// Ctrl + Esc �𖳌��ɂ��܂�
				if (pkbhs->vkCode == VK_ESCAPE && bControlKeyDown)
					return 1;

				// Alt + Tab �𖳌��ɂ��܂�
				if (pkbhs->vkCode == VK_TAB && pkbhs->flags & LLKHF_ALTDOWN)
					return 1;

				// Alt + Esc �𖳌��ɂ��܂�
				if (pkbhs->vkCode == VK_ESCAPE && pkbhs->flags & LLKHF_ALTDOWN)
					return 1;

				// Alt + F4 �𖳌��ɂ��܂�
				if (pkbhs->vkCode == VK_F4 && pkbhs->flags & LLKHF_ALTDOWN )
					return 1 ;

				// Alt up �𖳌��ɂ��܂�
				if (pkbhs->flags & LLKHF_UP )
					return 1 ;

				break;
			}

			default:
				break;
		}
	}
    return CallNextHookEx( WinData.TaskHookHandle, nCode, wParam, lParam);
}
*/
#ifdef DX_THREAD_SAFE

// ProcessMessage ���Ђ�����ĂтÂ���X���b�h
DWORD WINAPI ProcessMessageThreadFunction( LPVOID )
{
	int Result ;
	DWORD ThreadID ;
	HANDLE EventHandle ;
	int WaitFlag = 0 ;

	// �X���b�h�̂h�c�𓾂Ă���
	ThreadID = GetCurrentThreadId() ;

	// �Ђ����� ProcessMessage ���ĂтÂ���
	for(;;)
	{
		// �\�t�g�̏I���t���O���������烋�[�v����O���
		if( WinData.QuitMessageFlag == TRUE ) break ;

		// CheckConflictAndWaitDxFunction ���Ă΂�Ă�����A�Ă΂�I���܂ő҂�
		CRITICALSECTION_LOCK( &WinData.DxConflictCheckCriticalSection ) ;
//		CheckConflict( &WinData.DxConflictCheckFlag ) ;

		// �c�w���C�u�������g�p����Ă��邩�ǂ����ŏ����𕪊�
		if( WinData.DxUseThreadFlag == FALSE || WinData.DxUseThreadID == ThreadID )
		{
//RUN:
			// �҂��Ă���X���b�h�����āA���ꂪ�����̃X���b�h�ł͂Ȃ��ꍇ�͑҂�
			if( WinData.DxUseThreadFlag == FALSE && WinData.DxConflictWaitThreadNum != 0 && WinData.DxUseThreadID != ThreadID )
			{
				if( WinData.DxConflictWaitThreadID[0][0] != ThreadID )
				{
					goto WAIT ;
				}
				else
				{
					// �����҂��Ă����̂������������烊�X�g���X���C�h������
					if( WinData.DxConflictWaitThreadNum != 1 )
					{
						EventHandle = (HANDLE)WinData.DxConflictWaitThreadID[0][1] ;
						_MEMCPY( &WinData.DxConflictWaitThreadID[0][0], &WinData.DxConflictWaitThreadID[1][0], sizeof( DWORD_PTR ) * 2 * ( WinData.DxConflictWaitThreadNum - 1 ) ) ;
						WinData.DxConflictWaitThreadID[WinData.DxConflictWaitThreadNum - 1][1] = (DWORD_PTR)EventHandle ;
						WinData.DxConflictWaitThreadID[WinData.DxConflictWaitThreadNum - 1][0] = (DWORD_PTR)0 ;
					}

					// �҂��Ă��鐔�����炷
					WinData.DxConflictWaitThreadNum -- ;
				}
			}

			// �����Z�b�g
			WinData.DxUseThreadFlag = TRUE ;
			WinData.DxUseThreadID = ThreadID ;

			// �g�p���J�E���^���C���N�������g����
//			if( WinData.DxConflictCheckCounter != 0 )
//			{
//				DXST_ERRORLOG_ADDW( L"�G���[:�Փ˔��� No.2\n" ) ;
//			}
			WinData.DxConflictCheckCounter ++ ;

			// ���b�N�t���O��|��
			CriticalSection_Unlock( &WinData.DxConflictCheckCriticalSection ) ;
//			WinData.DxConflictCheckFlag -- ;

			// ProcessMessage ���Ă�
			Result = NS_ProcessMessage() ;

			// CheckConflictAndWaitDxFunction ���Ă΂�Ă�����A�Ă΂�I���܂ő҂�
			CRITICALSECTION_LOCK( &WinData.DxConflictCheckCriticalSection ) ;
//			CheckConflict( &WinData.DxConflictCheckFlag ) ;

			// �J�E���^���O�ɂȂ��Ă���g�p����Ԃ�����
			if( WinData.DxConflictCheckCounter == 1 )
			{
				WinData.DxUseThreadFlag = FALSE ;
				WinData.DxUseThreadID = 0xffffffff ;

				// �����҂��Ă���X���b�h������ꍇ�́A�X���[�v����t���O�𗧂Ă�
				if( WinData.DxConflictWaitThreadNum > 0 )
					WaitFlag = 1 ;
			}
//			else
//			{
//				DXST_ERRORLOG_ADDW( L"�G���[:�Փ˔��� No.3 \n" ) ;
//			}

			// �J�E���^���f�N�������g
			WinData.DxConflictCheckCounter -- ;

			// ���b�N�t���O��|��
			CriticalSection_Unlock( &WinData.DxConflictCheckCriticalSection ) ;
//			WinData.DxConflictCheckFlag -- ;

			// �X���[�v����t���O�������Ă�����X���[�v����
			if( WaitFlag == 1 )
			{
				// ���Ɏ��s���ׂ��X���b�h�̃C�x���g���V�O�i����Ԃɂ���
				SetEvent( (HANDLE)WinData.DxConflictWaitThreadID[0][1] ) ;
				WaitFlag = 0 ;
			}

			// PostMessageW �̖߂�l�� -1 �������烋�[�v�𔲂���
			if( Result < 0 ) break ;

			// �b���Q��
			Sleep( 17 ) ;
		}
		else
		{
WAIT:
			// �����҂��Ă���X���b�h�����E���z���Ă�����P���ȑ҂��������s��
			if( WinData.DxConflictWaitThreadNum == MAX_THREADWAIT_NUM )
			{
				// �Ƃ肠�������̊֐����g�p���A�t���O��|��
				CriticalSection_Unlock( &WinData.DxConflictCheckCriticalSection ) ;
//				WinData.DxConflictCheckFlag -- ;

//				DXST_ERRORLOG_ADDW( L"�G���[:�Փ˔��� No.5 \n" ) ;

				// �����Q��
				Sleep( 1 ) ;

				// �ŏ��ɖ߂�
				continue ;
			}

			// �҂��Ă�X���b�h�����܂���Ƃ�������ǉ�����
			WinData.DxConflictWaitThreadID[WinData.DxConflictWaitThreadNum][0] = ThreadID ;
			EventHandle = (HANDLE)WinData.DxConflictWaitThreadID[WinData.DxConflictWaitThreadNum][1] ;
			WinData.DxConflictWaitThreadNum ++ ;

			// ���̊֐����g�p���A�t���O��|��
			CriticalSection_Unlock( &WinData.DxConflictCheckCriticalSection ) ;
//			WinData.DxConflictCheckFlag -- ;

//			// �҂��Ă�X���b�h�����܂���J�E���^�[���C���N�������g����
//			WinData.DxConflictWaitThreadCounter ++ ;
//			if( WinData.DxConflictWaitThreadCounter <= 0 )
//				WinData.DxConflictWaitThreadCounter = 1 ;

			// �g�p���t���O���|��邩�AQuitMessageFlag �����܂ő҂�
//			while( WinData.DxUseThreadFlag == TRUE && WinData.QuitMessageFlag != TRUE )

			while( WaitForSingleObject( EventHandle, 0 ) == WAIT_TIMEOUT && WinData.QuitMessageFlag != TRUE )
			{
				WaitForSingleObject( EventHandle, 1000 ) ;
//				Sleep( 0 ) ;
			}
//			WaitForSingleObject( EventHandle, INFINITE ) ;
			ResetEvent( EventHandle ) ;
			// �҂��Ă�X���b�h�������J�E���^���f�N�������g����
//			WinData.DxConflictWaitThreadCounter -- ;
//			if( WinData.DxConflictWaitThreadCounter < 0 )
//				WinData.DxConflictWaitThreadCounter = 0 ;

			// CheckConflictAndWaitDxFunction ���Ă΂�Ă�����A�Ă΂�I���܂ő҂�
//			CRITICALSECTION_LOCK( &WinData.DxConflictCheckCriticalSection ) ;
//			CheckConflict( &WinData.DxConflictCheckFlag ) ;

//			goto RUN ;
		}
	}

	// �����X���b�h�̑҂����̒��Ɏ�����������l�߂�
	{
		int i ;

		// CheckConflictAndWaitDxFunction ���Ă΂�Ă�����A�Ă΂�I���܂ő҂�
		CRITICALSECTION_LOCK( &WinData.DxConflictCheckCriticalSection ) ;
//		CheckConflict( &WinData.DxConflictCheckFlag ) ;

		for( i = 0 ; i < WinData.DxConflictWaitThreadNum ; )
		{
			if( WinData.DxConflictWaitThreadID[i][0] != ThreadID )
			{
				i ++ ;
				continue ;
			}

			EventHandle = (HANDLE)WinData.DxConflictWaitThreadID[i][1] ;
			if( WinData.DxConflictWaitThreadNum - 1 != i )
			{
				_MEMCPY( &WinData.DxConflictWaitThreadID[i][0], &WinData.DxConflictWaitThreadID[i+1][0], sizeof( DWORD_PTR ) * 2 * ( WinData.DxConflictWaitThreadNum - i - 1 ) ) ;
				WinData.DxConflictWaitThreadID[WinData.DxConflictWaitThreadNum - 1][1] = (DWORD_PTR)EventHandle ;
				WinData.DxConflictWaitThreadID[WinData.DxConflictWaitThreadNum - 1][0] = 0 ;
			}
			WinData.DxConflictWaitThreadNum -- ;

			break ;
		}

		// �t���O��|��
		CriticalSection_Unlock( &WinData.DxConflictCheckCriticalSection ) ;
//		WinData.DxConflictCheckFlag -- ;

		// �������X�����̔Ԃ������玟�̃X���b�h�̃C�x���g���V�O�i����Ԃɂ���
		if( i == 0 && WinData.DxConflictWaitThreadNum > 0 )
		{
			SetEvent( (HANDLE)WinData.DxConflictWaitThreadID[0][1] ) ;
		}
	}

	// �X���b�h���I���������Ƃ������t���O�𗧂Ă�
	WinData.ProcessMessageThreadExitFlag = TRUE ;

	// �X���b�h�I��
	ExitThread( 0 ) ;

	// �I��
	return 0 ;
}

#endif

//// �t�b�N���ꂽ���̃R�[���o�b�N�֐�
//LRESULT CALLBACK MsgHook(int /*nCnode*/, WPARAM /* wParam */, LPARAM lParam)
//{
//	MSG *pmsg;
//
//	if( WinData.ActiveFlag == TRUE && WinData.SysCommandOffFlag == TRUE )
//	{
//		pmsg = (MSG *)lParam;
//		if(pmsg->message == WM_USER + 260) pmsg->message = WM_NULL;
//	}
//
//	return 0;
//}

// �^�X�N�X�C�b�`��L���ɂ��邩�ǂ�����ݒ肷��
extern int NS_SetSysCommandOffFlag( int Flag, const TCHAR *HookDllPath )
{
#ifdef UNICODE
	return SetSysCommandOffFlag_WCHAR_T( Flag, HookDllPath ) ;
#else
	int Result ;

	TCHAR_TO_WCHAR_T_STRING_ONE_BEGIN( HookDllPath, -1 )

	Result = SetSysCommandOffFlag_WCHAR_T( Flag, UseHookDllPathBuffer ) ;

	TCHAR_TO_WCHAR_T_STRING_END( HookDllPath )

	return Result ;
#endif
}

// �^�X�N�X�C�b�`��L���ɂ��邩�ǂ�����ݒ肷��
extern int SetSysCommandOffFlag_WCHAR_T( int Flag, const wchar_t *HookDllPath )
{
#ifndef DX_NON_STOPTASKSWITCH
	if( WinData.SysCommandOffFlag == Flag ) return 0 ;

	// �L���ɂ���w��̏ꍇ�̓t�@�C���p�X��ۑ�����
	if( Flag == TRUE )
	{
		// �t�@�C���p�X��ۑ�
		if( HookDllPath == NULL )
		{
			int Length, FileSize ;
			HANDLE FileHandle ;
			void *DestBuffer ;
			DWORD WriteSize ;
			int *pDxKeyHookBinaryConvert ;
			void *pDxKeyHookBinary ;

#ifdef _WIN64
			pDxKeyHookBinaryConvert = &DxKeyHookBinary_x64_Convert ;
			pDxKeyHookBinary        = DxKeyHookBinary_x64 ;
#else // _WIN64
			pDxKeyHookBinaryConvert = &DxKeyHookBinaryConvert ;
			pDxKeyHookBinary        = DxKeyHookBinary ;
#endif // _WIN64

			// �p�X�������Ɏw�肳��Ȃ������ꍇ�͓����̂c�k�k��
			// �e���|�����t�@�C���ɏo�͂��Ďg�p����

			// �L�[�{�[�h�t�b�N�c�k�k�t�@�C���̃T�C�Y���擾��
			if( *pDxKeyHookBinaryConvert == 0 )
			{
				*pDxKeyHookBinaryConvert = 1 ;
				Char128ToBin( pDxKeyHookBinary, pDxKeyHookBinary ) ;
			}
			FileSize = DXA_Decode( pDxKeyHookBinary, NULL ) ;

			// �������̊m��
			DestBuffer = DXALLOC( ( size_t )FileSize ) ;
			if( DestBuffer == NULL )
				return -1 ;

			// ��
			DXA_Decode( pDxKeyHookBinary, DestBuffer ) ;

			// �e���|�����t�@�C���̃f�B���N�g���p�X���擾����
			if( GetTempPathW( FILEPATH_MAX, WinData.HookDLLFilePath ) == 0 )
			{
				DXFREE( DestBuffer ) ;
				return -1 ;
			}

			// ������̍Ō�Ɂ��}�[�N������
			Length = ( int )_WCSLEN( WinData.HookDLLFilePath ) ;
			if( WinData.HookDLLFilePath[Length-1] != L'\\' ) 
			{
				WinData.HookDLLFilePath[Length]   = L'\\' ;
				WinData.HookDLLFilePath[Length+1] = L'\0' ;
			}

			// �N���g�������ɖ����t�@�C������ǉ�����
			_WCSCAT_S( WinData.HookDLLFilePath, sizeof( WinData.HookDLLFilePath ), L"ddxx_MesHoooooook.dll" );

			// �e���|�����t�@�C�����J��
			DeleteFileW( WinData.HookDLLFilePath ) ;
			FileHandle = CreateFileW( WinData.HookDLLFilePath, GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL ) ;
			if( FileHandle == NULL )
			{
				DXFREE( DestBuffer ) ;
				return -1 ;
			}

			// �e���|�����t�@�C���Ƀf�[�^�������o��
			WriteFile( FileHandle, DestBuffer, ( DWORD )FileSize, &WriteSize, NULL ) ;

			// ����
			CloseHandle( FileHandle ) ;

			// �������̉��
			DXFREE( DestBuffer ) ;

			// ���[�U�[�w��̃t�b�N�c�k�k���g���Ă��Ȃ��t���O�𗧂Ă�
			WinData.NotUseUserHookDllFlag = TRUE ;
		}
		else
		{
			_WCSCPY_S( WinData.HookDLLFilePath, sizeof( WinData.HookDLLFilePath ), HookDllPath ) ; 

			// ���[�U�[�w��̃t�b�N�c�k�k���g���Ă��Ȃ��t���O��|��
			WinData.NotUseUserHookDllFlag = FALSE ;
		}
	}

	// �E�C���h�E�Y�̃o�[�W�����ɂ���ď����𕪊�
	if( WinData.WindowsVersion < DX_WINDOWSVERSION_NT31 )
	{
		// Win95 �J�[�l���̏ꍇ�̏���
		UINT nPreviousState;
//		SystemParametersInfoW( SPI_SETSCREENSAVERRUNNING, Flag, &nPreviousState, 0 ) ;
//		SystemParametersInfoW( SPI_SCREENSAVERRUNNING, TRUE, &nPreviousState, 0 ) ;
		SystemParametersInfoW( SPI_SETSCREENSAVERRUNNING/*SPI_SCREENSAVERRUNNING*/, ( UINT )( WinData.ActiveFlag && Flag ? 1 : 0 ), &nPreviousState, 0 ) ;
	}
	else
	{
		// WinNT �J�[�l���̏ꍇ�̏���
		if( Flag == TRUE )
		{
			// �L�[�{�[�h�t�b�N�̃Z�b�g
//			if( WinData.TaskHookHandle == NULL )
//			{
//				WinData.TaskHookHandle = SetWindowsHookExW( WH_KEYBOARD_LL, LowLevelKeyboardProc, WinData.Instance, 0 ) ;
//			}

			// ���b�Z�[�W�t�b�N�̃Z�b�g
//			if( WinData.GetMessageHookHandle == NULL )
			{
//				WinData.MessageHookThredID = GetWindowThreadProcessId( WinData.MainWindow, NULL ) ;
//				WinData.MessageHookThredID = GetWindowThreadProcessId( GetDesktopWindow(), NULL ) ;
				WinData.MessageHookDLL = LoadLibraryW( WinData.HookDLLFilePath ) ;

				// DLL ������������i�܂Ȃ�
				if( WinData.MessageHookDLL != NULL )
				{
					WinData.MessageHookCallBadk = ( MSGFUNC )GetProcAddress( WinData.MessageHookDLL, "SetMSGHookDll" ) ;
					if( WinData.MessageHookCallBadk != NULL )
					{
						WinData.MessageHookCallBadk( WinData.MainWindow, &WinData.KeyboardHookHandle, TRUE ) ;
//						WinData.GetMessageHookHandle = SetWindowsHookExW( WH_GETMESSAGE, WinData.MessageHookCallBadk, WinData.MessageHookDLL, WinData.MessageHookThredID ) ;
//						WinData.GetMessageHookHandle = SetWindowsHookExW( WH_GETMESSAGE, MsgHook, WinData.Instance, 0 ) ;
					}
				}
			}
		}
		else
		{
			// ���b�Z�[�W�t�b�N�𖳌���
//			if( WinData.GetMessageHookHandle != NULL )
			{
				if( WinData.MessageHookDLL != NULL )
				{
					WinData.MessageHookCallBadk( WinData.MainWindow, &WinData.KeyboardHookHandle, FALSE ) ;
//					UnhookWindowsHookEx( WinData.GetMessageHookHandle ) ;
//					UnhookWindowsHookEx( WinData.KeyboardHookHandle ) ;
					FreeLibrary( WinData.MessageHookDLL ) ;
					WinData.MessageHookCallBadk = NULL ;
					WinData.MessageHookDLL = NULL ;
//					WinData.GetMessageHookHandle = NULL ;
					WinData.KeyboardHookHandle = NULL ;
				}
			}
		}
	}

	// �S�o�[�W��������
/*	if( Flag == TRUE )
	{
		// ����ɑ��̃E�C���h�E���A�N�e�B�u�ɂȂ�Ȃ��悤�ɂ���
		while( WinData.ActiveFlag != TRUE )
		{
			if( NS_ProcessMessage() == -1 ) break ;
		}
		LockSetForegroundWindow( LSFW_LOCK ) ;
	}
*/
	// �t���O��ۑ�
	WinData.SysCommandOffFlag = Flag ;

	// �I��
	return 0 ;
#else
	return -1;
#endif
}

// ���b�Z�[�W���t�b�N����E�C���h�E�v���[�W����o�^����
extern int NS_SetHookWinProc( WNDPROC WinProc )
{
	// �o�^����
	WinData.UserWindowProc = WinProc ;

	// �I��
	return 0 ;
}

// SetHookWinProc �Őݒ肵���E�C���h�E�v���[�W���̖߂�l���g�p���邩�ǂ�����ݒ肷��ASetHookWinProc �Őݒ肵���E�C���h�E�v���[�W���̒��ł̂ݎg�p�\( UseFlag TRUE:�߂�l���g�p���āA�c�w���C�u�����̃E�C���h�E�v���[�W���̏����͍s��Ȃ�  FALSE:�߂�l�͎g�p�����A�E�C���h�E�v���[�W������o����A�c�w���C�u�����̃E�C���h�E�v���[�W���̏������s�� )
extern int NS_SetUseHookWinProcReturnValue( int UseFlag )
{
	// �t���O��ۑ�����
	WinData.UseUserWindowProcReturnValue = UseFlag ;

	// �I��
	return 0 ;
}

// �Q�d�N�����������ǂ����̃t���O���Z�b�g����
extern int NS_SetDoubleStartValidFlag( int Flag )
{
	WinData.DoubleStartValidFlag = Flag ;

	// �I��
	return 0 ;
}

// ���b�Z�[�W�������c�w���C�u�����Ɍ����肵�Ă��炤�E�C���h�E��ǉ�����
extern int NS_AddMessageTakeOverWindow( HWND Window )
{
	if( WinData.MesTakeOverWindowNum == MAX_MESTAKEOVERWIN_NUM ) return -1 ;

	// �ǉ�����
	WinData.MesTakeOverWindow[WinData.MesTakeOverWindowNum] = Window ;
	WinData.MesTakeOverWindowNum ++ ;

	// �I��
	return 0 ;
}

// ���b�Z�[�W�������c�w���C�u�����Ɍ����肵�Ă��炤�E�C���h�E�����炷
extern	int	NS_SubMessageTakeOverWindow( HWND Window )
{
	int i ;

	// �T��
	for( i = 0 ; i < MAX_MESTAKEOVERWIN_NUM && WinData.MesTakeOverWindow[i] != Window ; i ++ ){}
	if( i == MAX_MESTAKEOVERWIN_NUM ) return -1 ;

	// ���炷
	WinData.MesTakeOverWindowNum -- ;

	// �Ԃ�
	_MEMMOVE( &WinData.MesTakeOverWindow[i], &WinData.MesTakeOverWindow[i+1], sizeof( HWND ) * ( WinData.MesTakeOverWindowNum - i ) ) ;

	// �I��
	return 0 ;
}

// �E�C���h�E�̏����ʒu��ݒ肷��
extern int NS_SetWindowInitPosition( int x, int y )
{
	WinData.WindowX = x ;
	WinData.WindowY = y ;
	WinData.WindowPosValid = TRUE ;

	return 0 ;
}

// �c�w���C�u�����Ń��b�Z�[�W�������s���_�C�A���O�{�b�N�X��o�^����
extern int NS_SetDialogBoxHandle( HWND WindowHandle )
{
	WinData.DialogBoxHandle = WindowHandle ;
	
	return 0 ;
}

// ���C���E�C���h�E��\�����邩�ǂ����̃t���O���Z�b�g����
extern int NS_SetWindowVisibleFlag( int Flag )
{
	if( WinData.NotWindowVisibleFlag == !Flag ) return 0 ;

	WinData.NotWindowVisibleFlag = !Flag ;

	if( WinData.MainWindow == NULL )
	{
//		NS_SetNotDrawFlag( WinData.NotWindowVisibleFlag ) ;
		
		return 0 ;
	}

	if( WinData.NotWindowVisibleFlag == FALSE )
	{
		ShowWindow( WinData.MainWindow , SW_SHOW ) ;
		UpdateWindow( WinData.MainWindow ) ;
	}
	else
	{
		ShowWindow( WinData.MainWindow , SW_HIDE ) ;
		UpdateWindow( WinData.MainWindow ) ;
	}

	// �I��
	return 0 ;
}

// ���C���E�C���h�E���ŏ������邩�ǂ����̃t���O���Z�b�g����
extern int NS_SetWindowMinimizeFlag( int Flag )
{
	if( WinData.WindowMinimizeFlag == Flag ) return 0 ;

	WinData.WindowMinimizeFlag = Flag ;

	if( WinData.MainWindow == NULL )
	{
//		NS_SetNotDrawFlag( WinData.NotWindowVisibleFlag ) ;
		
		return 0 ;
	}

	if( WinData.WindowMinimizeFlag == FALSE )
	{
		ShowWindow( WinData.MainWindow , SW_RESTORE ) ;
		UpdateWindow( WinData.MainWindow ) ;
	}
	else
	{
		ShowWindow( WinData.MainWindow , SW_MINIMIZE ) ;
		UpdateWindow( WinData.MainWindow ) ;
	}

	// �I��
	return 0 ;
}

// ���C���E�C���h�E�́~�{�^�������������Ƀ��C�u�����������I�ɃE�C���h�E����邩�ǂ����̃t���O���Z�b�g����
extern int NS_SetWindowUserCloseEnableFlag( int Flag )
{
	WinData.NonUserCloseEnableFlag = !Flag ;
	
	// �I��
	return 0 ;
}

// �c�w���C�u�����I������ PostQuitMessage ���ĂԂ��ǂ����̃t���O���Z�b�g����
extern int NS_SetDxLibEndPostQuitMessageFlag( int Flag )
{
	WinData.NonDxLibEndPostQuitMessageFlag = !Flag ;

	// �I��
	return 0 ;
}

// �\���Ɏg�p����E�C���h�E�̃n���h�����擾����
extern HWND GetDisplayWindowHandle( void )
{
	if( WinData.UserWindowFlag )
	{
		return WinData.UserChildWindow ? WinData.UserChildWindow : WinData.MainWindow ;
	}

	return WinData.MainWindow ;
}

// ���[�U�[�E�C���h�E�̏����X�V
static void UpdateUserWindowInfo( void )
{
	// ��ʃ��[�h�̓E�C���h�E���[�h
	NS_ChangeWindowMode( TRUE ) ;

#ifndef DX_NON_GRAPHICS
	// ��ʃT�C�Y�̓N���C�A���g�̈�̃T�C�Y
	{
		RECT ClientRect ;

		GetClientRect( GetDisplayWindowHandle(), &ClientRect ) ;
		NS_SetGraphMode( ClientRect.right - ClientRect.left,
							ClientRect.bottom - ClientRect.top, 32 ) ;
	}
#endif // DX_NON_GRAPHICS
}

// �c�w���C�u�����ŗ��p����E�C���h�E�̃n���h�����Z�b�g����
extern int NS_SetUserWindow( HWND WindowHandle )
{
	if( DxSysData.DxLib_InitializeFlag == TRUE ) return -1 ;

	WinData.MainWindow = WindowHandle ;
	WinData.UserWindowFlag = WindowHandle == NULL ? FALSE : TRUE ;

	// �����X�V
	UpdateUserWindowInfo() ;

	// �I��
	return 0 ;
}

// �c�w���C�u�����Ŏg�p����\���p�̎q�E�C���h�E�̃n���h�����Z�b�g����(DxLib_Init �����s����ȑO�ł̂ݗL��)
extern int NS_SetUserChildWindow( HWND WindowHandle )
{
	if( DxSysData.DxLib_InitializeFlag == TRUE ) return -1 ;

	WinData.UserChildWindow = WindowHandle ;

	// �����X�V
	UpdateUserWindowInfo() ;

	// �I��
	return 0 ;
}

// SetUseWindow �Őݒ肵���E�C���h�E�̃��b�Z�[�W���[�v�������c�w���C�u�����ōs�����ǂ����A�t���O���Z�b�g����
extern int NS_SetUserWindowMessageProcessDXLibFlag( int Flag )
{
	WinData.NotUserWindowMessageProcessDXLibFlag = !Flag ;

	// �I��
	return 0 ;
}

// FPU�̐��x�𗎂Ƃ��Ȃ��ݒ���g�p���邩�ǂ�����ݒ肷��ADxLib_Init ���Ăяo���O�̂ݗL��( TRUE:�g�p����(���x�������Ȃ�)  FALSE:�g�p���Ȃ�(���x�𗎂Ƃ�(�f�t�H���g) )
extern int NS_SetUseFPUPreserveFlag( int Flag )
{
	// �������O�̂ݗL��
	if( DxSysData.DxLib_InitializeFlag ) return -1 ;

	// �t���O��ۑ�
	WinData.UseFPUPreserve = Flag ;

	// �I��
	return 0 ;
}

// �}�E�X�|�C���^���E�C���h�E�̃N���C�A���g�G���A�̊O�ɂ����邩�ǂ�����ݒ肷��( TRUE:������( �f�t�H���g�ݒ� )  FALSE:�����Ȃ� )
extern int NS_SetValidMousePointerWindowOutClientAreaMoveFlag( int Flag )
{
	Flag = Flag ? FALSE : TRUE ;

	// ���܂łƃt���O�������ꍇ�͉������Ȃ�
	if( WinData.NotMoveMousePointerOutClientAreaFlag == Flag )
		return 0 ;

	// �t���O��ۑ�
	WinData.NotMoveMousePointerOutClientAreaFlag = Flag ;

	if( DxSysData.DxLib_InitializeFlag == TRUE )
	{
		if( WinData.NotMoveMousePointerOutClientAreaFlag && WinData.ActiveFlag && WinData.WindowModeFlag )
		{
			WinData.SetClipCursorFlag = FALSE ;
		}
		else
		{
			ClipCursor( NULL ) ;
		}
	}

	// �I��
	return 0 ;
}

// �o�b�N�o�b�t�@�̓��ߐF�̕����𓧉߂����邩�ǂ�����ݒ肷��( TRUE:���߂�����  FALSE:���߂����Ȃ� )
extern int NS_SetUseBackBufferTransColorFlag( int Flag )
{
	// ���܂łƃt���O�������ꍇ�͉������Ȃ�
	if( WinData.BackBufferTransColorFlag == Flag )
		return 0 ;

	// �t���O��ۑ�
	WinData.BackBufferTransColorFlag = Flag ;

	// �E�C���h�E���[�h��ύX����
	NS_SetWindowStyleMode( WinData.WindowStyle ) ;

	// �I��
	return 0 ;
}

// UpdateLayerdWindowForBaseImage �� UpdateLayerdWindowForSoftImage ���g�p���邩�ǂ�����ݒ肷��( TRUE:�g�p����  FALSE:�g�p���Ȃ� )
extern int NS_SetUseUpdateLayerdWindowFlag( int Flag )
{
	// ���܂łƃt���O�������ꍇ�͉������Ȃ�
	if( WinData.UseUpdateLayerdWindowFlag == Flag )
		return 0 ;

	// �t���O��ۑ�
	WinData.UseUpdateLayerdWindowFlag = Flag ;

	// �E�C���h�E���[�h��ύX����
	NS_SetWindowStyleMode( WinData.WindowStyle ) ;

	// �I��
	return 0 ;
}

// ���\�[�X��ǂݍ��ލۂɎg�p���郂�W���[����ݒ肷��( NULL ���w�肷��Ə�����Ԃɖ߂�܂��A�f�t�H���g�ł� NULL )
extern int NS_SetResourceModule( HMODULE ResourceModule )
{
	WinData.LoadResourModule = ResourceModule ;

	// �I��
	return 0 ;
}

// WM_PAINT ���b�Z�[�W�������ۂɁw�c�w���C�u������ WM_PAINT���b�Z�[�W�������ۂ̏����x���s�����ǂ�����ݒ肷��( �ʃX���b�h�ŕ`�揈�����s���ꍇ�ȂǂŎg�p )
extern int NS_SetUseDxLibWM_PAINTProcess( int Flag )
{
	WinData.NotUseDxLibWM_PAINTProcess = Flag ? FALSE : TRUE ;

	// �I��
	return 0 ;
}












// �N���b�v�{�[�h�֌W

// �N���b�v�{�[�h�Ɋi�[����Ă���e�L�X�g�f�[�^��ǂݏo���A-1 �̏ꍇ�̓N���b�v�{�[�h�Ƀe�L�X�g�f�[�^�͖����Ƃ�������( DestBuffer �� NULL ��n���Ɗi�[�ɕK�v�ȃf�[�^�T�C�Y���Ԃ��Ă��� )
extern int NS_GetClipboardText( TCHAR *DestBuffer )
{
	HGLOBAL Mem ;
	void *SrcBuffer ;

	// �N���b�v�{�[�h���I�[�v��
	if( OpenClipboard( WinData.MainWindow ) == 0 )
		return -1 ;

	// �N���b�v�{�[�h�Ɋi�[����Ă���f�[�^���e�L�X�g�f�[�^���ǂ������擾����
	if( IsClipboardFormatAvailable( CLIPBOARD_TEXT ) == 0 )
	{
		CloseClipboard() ;
		return -1 ;
	}

	// �N���b�v�{�[�h�Ɋi�[����Ă���e�L�X�g�f�[�^�̃������n���h�����擾����
	Mem = GetClipboardData( CLIPBOARD_TEXT ) ;

	// �o�̓o�b�t�@�� NULL �̏ꍇ�̓e�L�X�g�f�[�^�̃T�C�Y��Ԃ�
	if( DestBuffer == NULL )
	{
		SIZE_T Size ;

		Size = GlobalSize( Mem ) + 1 ;
		CloseClipboard() ;
		return ( int )Size ;
	}

	// �o�̓o�b�t�@�Ƀe�L�X�g�f�[�^���R�s�[����
	SrcBuffer = GlobalLock( Mem ) ;
	_TSTRCPY( DestBuffer, ( TCHAR * )SrcBuffer ) ;
	GlobalUnlock( Mem ) ;
	CloseClipboard() ;

	// �I��
	return 0 ;
}

// �N���b�v�{�[�h�Ɋi�[����Ă���e�L�X�g�f�[�^��ǂݏo���A-1 �̏ꍇ�̓N���b�v�{�[�h�Ƀe�L�X�g�f�[�^�͖����Ƃ�������( DestBuffer �� NULL ��n���Ɗi�[�ɕK�v�ȃf�[�^�T�C�Y���Ԃ��Ă��� )
extern int GetClipboardText_WCHAR_T( wchar_t *DestBuffer )
{
	HGLOBAL Mem ;
	void *SrcBuffer ;

	// �N���b�v�{�[�h���I�[�v��
	if( OpenClipboard( WinData.MainWindow ) == 0 )
		return -1 ;

	// �N���b�v�{�[�h�Ɋi�[����Ă���f�[�^���e�L�X�g�f�[�^���ǂ������擾����
	if( IsClipboardFormatAvailable( CF_UNICODETEXT ) == 0 )
	{
		CloseClipboard() ;
		return -1 ;
	}

	// �N���b�v�{�[�h�Ɋi�[����Ă���e�L�X�g�f�[�^�̃������n���h�����擾����
	Mem = GetClipboardData( CF_UNICODETEXT ) ;

	// �o�̓o�b�t�@�� NULL �̏ꍇ�̓e�L�X�g�f�[�^�̃T�C�Y��Ԃ�
	if( DestBuffer == NULL )
	{
		SIZE_T Size ;

		Size = GlobalSize( Mem ) + 1 ;
		CloseClipboard() ;
		return ( int )Size ;
	}

	// �o�̓o�b�t�@�Ƀe�L�X�g�f�[�^���R�s�[����
	SrcBuffer = GlobalLock( Mem ) ;
	_WCSCPY( DestBuffer, ( wchar_t * )SrcBuffer ) ;
	GlobalUnlock( Mem ) ;
	CloseClipboard() ;

	// �I��
	return 0 ;
}

// �N���b�v�{�[�h�Ƀe�L�X�g�f�[�^���i�[����
extern int NS_SetClipboardText( const TCHAR *Text )
{
#ifdef UNICODE
	return SetClipboardText_WCHAR_T( Text ) ;
#else
	int Result ;

	TCHAR_TO_WCHAR_T_STRING_ONE_BEGIN( Text, return -1 )

	Result = SetClipboardText_WCHAR_T( UseTextBuffer ) ;

	TCHAR_TO_WCHAR_T_STRING_END( Text )

	return Result ;
#endif
}

// �N���b�v�{�[�h�Ƀe�L�X�g�f�[�^���i�[����
extern int SetClipboardText_WCHAR_T( const wchar_t *Text )
{
	HGLOBAL Mem ;
	void *Buffer ;
	int Len ;

	// ������̒������擾
	Len = ( int )_WCSLEN( Text ) ;

	// ��������i�[���郁�����̈�̊m��
	Mem = GlobalAlloc( GMEM_FIXED, ( Len + 1 ) * sizeof( wchar_t ) ) ;

	// ��������m�ۂ����������̈�Ɋi�[
	Buffer = GlobalLock( Mem ) ;
	_WCSCPY( ( wchar_t * )Buffer, Text ) ;
	GlobalUnlock( Mem ) ;

	// �N���b�v�{�[�h���I�[�v��
	if( OpenClipboard( WinData.MainWindow ) )
	{
		// �N���b�v�{�[�h�ɕ�������i�[����
		EmptyClipboard() ;
		SetClipboardData( CF_UNICODETEXT, Mem ) ;
		CloseClipboard() ;
	}
	else
	{
		// �������̉��
		GlobalFree( Mem ) ;
	}

	// �I��
	return 0 ;
}










// �h���b�O���h���b�v���ꂽ�t�@�C���֌W

// �t�@�C���̃h���b�O���h���b�v�@�\��L���ɂ��邩�ǂ����̐ݒ���Đݒ肷��
extern int RefreshDragFileValidFlag()
{
	// �L����Ԃ�ύX
	DragAcceptFiles( WinData.MainWindow, WinData.DragFileValidFlag ) ;

	// �I��
	return 0 ;
}

// �t�@�C���̃h���b�O���h���b�v�@�\��L���ɂ��邩�ǂ����̃t���O���Z�b�g����
extern int NS_SetDragFileValidFlag( int Flag )
{
	// �w��Ɗ��ɓ����ꍇ�͉��������I��
	if( WinData.DragFileValidFlag == Flag ) return 0 ;
	
	// �L����Ԃ�ύX
	DragAcceptFiles( WinData.MainWindow, Flag ) ;
	
	// �t���O��ۑ�
	WinData.DragFileValidFlag = Flag ;

	// �I��
	return 0 ;
}

// �h���b�O���h���b�v���ꂽ�t�@�C���̏�������������
extern int NS_DragFileInfoClear( void )
{
	int i ;
	
	// �S�Ẵt�@�C�����p�Ɋm�ۂ����������̈���������
	for( i = 0 ; i < WinData.DragFileNum ; i ++ )
	{
		DXFREE( WinData.DragFileName[i] ) ;
		WinData.DragFileName[i] = NULL ;
	}
	
	// �t�@�C���̐����O�ɂ���
	WinData.DragFileNum = 0 ;
	
	// �I��
	return 0 ;
}

// �h���b�O���h���b�v���ꂽ�t�@�C�������擾����( -1:�擾�ł��Ȃ�����  0:�擾�ł��� )
extern int NS_GetDragFilePath( TCHAR *FilePathBuffer )
{
#ifdef UNICODE
	return GetDragFilePath_WCHAR_T( FilePathBuffer ) ;
#else
	int Size ;

	// �t�@�C������������������� -1 ��Ԃ�
	if( WinData.DragFileNum == 0 )
	{
		return -1 ;
	}

	// ��������R�s�[����
	Size = ConvString( ( const char * )WinData.DragFileName[ WinData.DragFileNum - 1 ], WCHAR_T_CHARCODEFORMAT, ( char * )FilePathBuffer, BUFFERBYTES_CANCEL, _TCHARCODEFORMAT ) ;

	// NULL ��n���ꂽ�當����i�[�ɕK�v�ȃT�C�Y��Ԃ�
	if( FilePathBuffer == NULL ) 
	{
		return Size ;
	}

	// �n���I�����������͉������
	DXFREE( WinData.DragFileName[ WinData.DragFileNum - 1 ] ) ;
	WinData.DragFileName[ WinData.DragFileNum - 1 ] = NULL ;

	// �h���b�O���h���b�v���ꂽ�t�@�C���̐������炷
	WinData.DragFileNum -- ;

	// �I��
	return 0 ;
#endif
}

// �h���b�O���h���b�v���ꂽ�t�@�C�������擾����( -1:�擾�ł��Ȃ�����  0:�擾�ł��� )
extern int GetDragFilePath_WCHAR_T( wchar_t *FilePathBuffer )
{
	// �t�@�C������������������� -1 ��Ԃ�
	if( WinData.DragFileNum == 0 )
	{
		return -1 ;
	}

	// NULL ��n���ꂽ�當����i�[�ɕK�v�ȃT�C�Y��Ԃ�
	if( FilePathBuffer == NULL ) 
	{
		return ( int )( ( _WCSLEN( WinData.DragFileName[ WinData.DragFileNum - 1 ] ) + 1 ) * sizeof( wchar_t ) ) ;
	}

	// ��������R�s�[����
	_WCSCPY( FilePathBuffer, WinData.DragFileName[ WinData.DragFileNum - 1 ] ) ;

	// �n���I�����������͉������
	DXFREE( WinData.DragFileName[ WinData.DragFileNum - 1 ] ) ;
	WinData.DragFileName[ WinData.DragFileNum - 1 ] = NULL ;

	// �h���b�O���h���b�v���ꂽ�t�@�C���̐������炷
	WinData.DragFileNum -- ;

	// �I��
	return 0 ;
}

// �h���b�O���h���b�v���ꂽ�t�@�C���̐����擾����
extern int NS_GetDragFileNum( void )
{
	return WinData.DragFileNum ;
}




















// �E�C���h�E�`��̈�ݒ�n�֐�

// �C�ӂ̃O���t�B�b�N����RGN�n���h�����쐬����
extern HRGN NS_CreateRgnFromGraph( int Width, int Height, const void *MaskData, int Pitch, int Byte )
{
#ifndef DX_NON_GRAPHICS
	HRGN RgnTmp, Rgn ;
	int i, j, x, f = 0, AddPitch ;
	BYTE *Data ;
	int SizeX, SizeY ;

	// �匳��RGN�n���h�����쐬����
	NS_GetDrawScreenSize( &SizeX, &SizeY ) ;
	Rgn = CreateRectRgn( 0, 0, SizeX, SizeY ) ;
	RgnTmp = CreateRectRgn( 0, 0, SizeX, SizeY ) ;
	CombineRgn( Rgn, Rgn, RgnTmp, RGN_XOR ) ;
	DeleteObject( RgnTmp ) ;

	// ����RGN�̑���
	AddPitch = Pitch - Width * Byte ;
	Data = ( BYTE * )MaskData ;
	for( i = 0 ; i < Height ; i ++, Data += AddPitch )
	{
		x = -1 ;
		for( j = 0 ; j < Width ; j ++, Data += Byte )
		{
			switch( Byte )
			{
			case 1 : f = *Data == 0 ; break ;
			case 2 : f = *( ( WORD * )Data ) == 0 ; break ;
			case 3 : f = *( ( WORD * )Data ) == 0 || *( ( BYTE * )( Data + 2 ) ) == 0 ; break ;
			case 4 : f = *( ( DWORD * )Data ) == 0 ; break ;
			}

			if( !f && x == -1 )
			{
				x = j ;
			}
			else
			if( f && x != -1 )
			{
				RgnTmp = CreateRectRgn( x, i, j, i + 1 ) ;
				CombineRgn( Rgn, Rgn, RgnTmp, RGN_OR ) ; 
				DeleteObject( RgnTmp ) ;
				x = -1 ;
			}
		}

		if( x != -1 )
		{
			RgnTmp = CreateRectRgn( x, i, j, i + 1 ) ;
			CombineRgn( Rgn, Rgn, RgnTmp, RGN_OR ) ; 
			DeleteObject( RgnTmp ) ;
			x = -1 ;
		}
	}

	// �I��
	return Rgn ;
#else // DX_NON_GRAPHICS
	return NULL ;
#endif // DX_NON_GRAPHICS
}

// �C�ӂ̊�{�C���[�W�f�[�^�Ɠ��ߐF����RGN�n���h�����쐬����
extern HRGN NS_CreateRgnFromBaseImage( BASEIMAGE *BaseImage, int TransColorR, int TransColorG, int TransColorB )
{
	HRGN RgnTmp, Rgn ;
	int i, j, x, f, AddPitch, Byte ;
	BYTE *Data ;
	DWORD TransColor ;
	int Width, Height ;
	BASEIMAGE TempBaseImage, *UseBaseImage ;

	// ��{�C���[�W�f�[�^�̃t�H�[�}�b�g��XRGB8�ȊO�̏ꍇ��XRGB8�`���ɂ���
	if( BaseImage->ColorData.AlphaMask != 0x00000000 ||
		BaseImage->ColorData.RedMask   != 0x00ff0000 ||
		BaseImage->ColorData.GreenMask != 0x0000ff00 ||
		BaseImage->ColorData.BlueMask  != 0x000000ff )
	{
		NS_CreateXRGB8ColorBaseImage( BaseImage->Width, BaseImage->Height, &TempBaseImage ) ;
		NS_BltBaseImage( 0, 0, BaseImage, &TempBaseImage ) ;
		UseBaseImage = &TempBaseImage ;
	}
	else
	{
		UseBaseImage = BaseImage ;
	}

	// �匳��RGN�n���h�����쐬����
	Width = UseBaseImage->Width ;
	Height = UseBaseImage->Height ;
	Rgn = CreateRectRgn( 0, 0, Width, Height ) ;
	RgnTmp = CreateRectRgn( 0, 0, Width, Height ) ;
	CombineRgn( Rgn, Rgn, RgnTmp, RGN_XOR ) ;
	DeleteObject( RgnTmp ) ;

	// ����RGN�̑���
	Byte = UseBaseImage->ColorData.PixelByte ;
	AddPitch = UseBaseImage->Pitch - UseBaseImage->Width * Byte ;
	Data = ( BYTE * )UseBaseImage->GraphData ;
	TransColor = NS_GetColor3( &UseBaseImage->ColorData, TransColorR, TransColorG, TransColorB, 255 ) & 0x00ffffff ;
	for( i = 0 ; i < Height ; i ++, Data += AddPitch )
	{
		x = -1 ;
		for( j = 0 ; j < Width ; j ++, Data += Byte )
		{
			f = ( *( ( DWORD * )Data ) & 0x00ffffff ) == TransColor ;

			if( !f && x == -1 )
			{
				x = j ;
			}
			else
			if( f && x != -1 )
			{
				RgnTmp = CreateRectRgn( x, i, j, i + 1 ) ;
				CombineRgn( Rgn, Rgn, RgnTmp, RGN_OR ) ; 
				DeleteObject( RgnTmp ) ;
				x = -1 ;
			}
		}

		if( x != -1 )
		{
			RgnTmp = CreateRectRgn( x, i, j, i + 1 ) ;
			CombineRgn( Rgn, Rgn, RgnTmp, RGN_OR ) ; 
			DeleteObject( RgnTmp ) ;
			x = -1 ;
		}
	}

	if( UseBaseImage == &TempBaseImage )
	{
		ReleaseBaseImage( &TempBaseImage ) ;
	}

	// �I��
	return Rgn ;
}


// �C�ӂ̃O���t�B�b�N����q�f�m���Z�b�g����
extern int NS_SetWindowRgnGraph( const TCHAR *FileName )
{
#ifdef UNICODE
	return SetWindowRgnGraph_WCHAR_T( FileName ) ;
#else
	int Result ;

	TCHAR_TO_WCHAR_T_STRING_ONE_BEGIN( FileName, return -1 )

	Result = SetWindowRgnGraph_WCHAR_T( UseFileNameBuffer ) ;

	TCHAR_TO_WCHAR_T_STRING_END( FileName )

	return Result ;
#endif
}

// �C�ӂ̃O���t�B�b�N����q�f�m���Z�b�g����
extern int SetWindowRgnGraph_WCHAR_T( const wchar_t *FileName )
{
	HBITMAP bmp ;
	BITMAP bm ;

	if( FileName == NULL )
	{
		if( WinData.WindowRgn == NULL )
		{
			return 0 ;
		}
		else
		{
			DeleteObject( WinData.WindowRgn ) ;
			WinData.WindowRgn = NULL ; 
		}
	}
	else
	{
		// �t�@�C����ǂݍ���
		bmp = CreateDIBGraph_WCHAR_T( FileName, FALSE, NULL ) ;
		if( bmp == NULL )
		{
			return -1 ;
		}

		// �O���t�B�b�N�̏����擾����
		GetObject( bmp, sizeof( BITMAP ), &bm ) ;

		// �s�b�`�␳
		bm.bmWidthBytes += bm.bmWidthBytes % 4 ? 4 - bm.bmWidthBytes % 4 : 0 ;

		// ���[�W�����쐬
		if( WinData.WindowRgn != NULL )
		{
			DeleteObject( WinData.WindowRgn ) ;
		}
		WinData.WindowRgn = NS_CreateRgnFromGraph( bm.bmWidth, bm.bmHeight, bm.bmBits, bm.bmWidthBytes, bm.bmBitsPixel / 8 ) ;

		// ���[�W���������蓖�Ă�
		if( WinData.MainWindow != NULL )
		{
			SetWindowRgn( WinData.MainWindow, WinData.WindowRgn, TRUE ) ;
		}
	}

	// �I��
	return 0 ;
}

// �`���̉�ʂ̓��ߐF�̕����𓧉߂�����q�f�m���Z�b�g����
extern int NS_UpdateTransColorWindowRgn( void )
{
#ifndef DX_NON_GRAPHICS
	BASEIMAGE ScreenImage ;
	int Width, Height ;

	NS_GetDrawScreenSize( &Width, &Height ) ;
	NS_CreateXRGB8ColorBaseImage( Width, Height, &ScreenImage ) ;
	NS_GetDrawScreenBaseImage( 0, 0, Width, Height, &ScreenImage ) ;

	// ���[�W�����쐬
	if( WinData.WindowRgn != NULL ) DeleteObject( WinData.WindowRgn ) ;
	WinData.WindowRgn = NS_CreateRgnFromBaseImage( &ScreenImage,
		( int )( ( GSYS.CreateImage.TransColor >> 16 ) & 0xff ),
		( int )( ( GSYS.CreateImage.TransColor >>  8 ) & 0xff ),
		( int )(   GSYS.CreateImage.TransColor         & 0xff ) ) ;

	// ���[�W���������蓖�Ă�
	if( WinData.MainWindow != NULL ) SetWindowRgn( WinData.MainWindow, WinData.WindowRgn, TRUE ) ;

	NS_ReleaseBaseImage( &ScreenImage ) ;

	// �I��
	return 0 ;
#else // DX_NON_GRAPHICS
	return -1 ;
#endif // DX_NON_GRAPHICS
}















// �c�[���o�[�֌W

// �w��̂h�c�̃{�^���̃C���f�b�N�X�𓾂�
static int SearchToolBarButton( int ID )
{
	int i ;
	WINTOOLBARITEMINFO *but ;

	// �c�[���o�[�̃Z�b�g�A�b�v���������Ă��Ȃ��ꍇ�͉��������ɏI��
	if( WinData.ToolBarUseFlag == FALSE ) return -1 ;

	// �w��̂h�c�̃{�^����T��
	but = WinData.ToolBarItem ;
	for( i = 0 ; i < WinData.ToolBarItemNum ; i ++, but ++ )
	{
		if( but->Type == TOOLBUTTON_TYPE_SEP ) continue ;	// ���Ԃ͏��O
		if( but->ID == ID ) break ;							// �h�c����v�����ꍇ�͔�����
	}

	// ����������G���[
	if( i == WinData.ToolBarItemNum ) return -1;

	// ��������C���f�b�N�X��Ԃ�
	return i ;
}

// �c�[���o�[�̍����𓾂�
extern int GetToolBarHeight( void )
{
	RECT rect ;

	if( WinData.ToolBarUseFlag == FALSE ) return 0 ;

	GetWindowRect( WinData.ToolBarHandle, &rect ) ;
	return rect.bottom - rect.top ;
}

// �c�[���o�[�̏���( NULL ���w�肷��ƃc�[���o�[������ )
extern int NS_SetupToolBar( const TCHAR *BitmapName, int DivNum, int ResourceID )
{
#ifdef UNICODE
	return SetupToolBar_WCHAR_T( BitmapName, DivNum, ResourceID ) ;
#else
	int Result ;

	TCHAR_TO_WCHAR_T_STRING_ONE_BEGIN( BitmapName, return -1 )

	Result = SetupToolBar_WCHAR_T( UseBitmapNameBuffer, DivNum, ResourceID ) ;

	TCHAR_TO_WCHAR_T_STRING_END( BitmapName )

	return Result ;
#endif
}

// �c�[���o�[�̏���( NULL ���w�肷��ƃc�[���o�[������ )
extern int SetupToolBar_WCHAR_T( const wchar_t *BitmapName, int DivNum, int ResourceID )
{
	HBITMAP NewBitmap = NULL ;
	BITMAP bm ;

	if( WinAPIData.Win32Func.WinMMDLL == NULL )
	{
		return -1 ;
	}

	// BitmapName �� NULL �ŁA���c�[���o�[���g���Ă��Ȃ��ꍇ�͉��������ɏI��
	if( BitmapName == NULL && ResourceID < 0 && WinData.ToolBarUseFlag == FALSE )
	{
		return 0 ;
	}

	// �t���X�N���[���ŋN�����Ă���ꍇ�����������ɏI��
	if( WinData.WindowModeFlag == FALSE )
	{
		return 0 ;
	}

	// �Ƃ肠�����S�Ẵ{�^�����폜
	NS_DeleteAllToolBarButton() ;

	// BitmapName �� NULL �ŁAResouceID �� -1 �̏ꍇ�̓c�[���o�[���폜����
	if( BitmapName == NULL && ResourceID < 0 )
	{
		// �c�[���o�[�E�C���h�E�� WM_CLOSE ���b�Z�[�W�𑗂�
		SendMessageW( WinData.ToolBarHandle, WM_CLOSE, 0, 0 ) ;
		WinData.ToolBarHandle = NULL ;

		// �r�b�g�}�b�v�t�@�C�����폜����
		DeleteObject( WinData.ToolBarButtonImage ) ;
		WinData.ToolBarButtonImage = NULL ;

		// �c�[���o�[���g�p���Ă���t���O��|��
		WinData.ToolBarUseFlag = FALSE ;
	}
	else
	{
		// �r�b�g�}�b�v�t�@�C����ǂݍ���
		if( BitmapName != NULL )
		{
			NewBitmap = CreateDIBGraph_WCHAR_T( BitmapName, 0, NULL ) ;
		}

		if( NewBitmap == NULL && ResourceID != -1 )
		{
			HRSRC RSrc ;
			BYTE *DataP ;
			BITMAPINFO *BmpInfo ;
			HGLOBAL Global ;

			// ���\�[�X���擾
			RSrc = FindResource( WinData.LoadResourModule == NULL ? WinAPIData.Win32Func.GetModuleHandleWFunc( NULL ) : WinData.LoadResourModule, MAKEINTRESOURCE( ResourceID ), RT_BITMAP ) ;
			if( RSrc )
			{
				// ���\�[�X���i�[����Ă��郁�����̈���擾
				Global = LoadResource( WinData.LoadResourModule == NULL ? WinAPIData.Win32Func.GetModuleHandleWFunc( NULL ) : WinData.LoadResourModule, RSrc ) ;
				if( Global )
				{
					DataP = ( BYTE * )LockResource( Global ) ;
					if( DataP )
					{
						// �C���[�W�f�[�^�̈ʒu���擾
						BmpInfo = ( BITMAPINFO * )DataP ;
						DataP += sizeof( BITMAPINFOHEADER ) ;

						// �J���[�r�b�g�����W�ȉ��̎��̓p���b�g������
						if( BmpInfo->bmiHeader.biBitCount <= 8 )
						{
							DataP += ( size_t )( 1 << BmpInfo->bmiHeader.biBitCount ) * sizeof( RGBQUAD ) ;
						}
						else
						// �J���[�r�b�g�����R�Q���P�U�ŃJ���[�}�X�N������
						if( BmpInfo->bmiHeader.biBitCount == 16 || BmpInfo->bmiHeader.biBitCount == 32 || BmpInfo->bmiHeader.biCompression == BI_BITFIELDS )
						{
							DataP += sizeof( RGBQUAD ) * 3 ;
						}

						// HBITMAP �̍쐬
						NewBitmap = NS_CreateDIBGraphToMem( BmpInfo, DataP, FALSE, NULL ) ;
					}

					// ���\�[�X���
					UnlockResource( Global ) ;
				}
			}
//			NewBitmap = LoadBitmap( WinData.Instance, MAKEINTRESOURCE( ResourceID ) ) ;
		}

		if( NewBitmap == NULL )
		{
			DXST_ERRORLOG_ADDUTF16LE( "\xc4\x30\xfc\x30\xeb\x30\xd0\x30\xfc\x30\x6e\x30\xdc\x30\xbf\x30\xf3\x30\x28\x75\x6e\x30\xd3\x30\xc3\x30\xc8\x30\xde\x30\xc3\x30\xd7\x30\xd5\x30\xa1\x30\xa4\x30\xeb\x30\x6e\x30\xad\x8a\x7f\x30\xbc\x8f\x7f\x30\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�c�[���o�[�̃{�^���p�̃r�b�g�}�b�v�t�@�C���̓ǂݍ��݂Ɏ��s���܂���\n" @*/ ) ;
			return -1 ;
		}

		// �r�b�g�}�b�v�̃T�C�Y�𓾂�
		GetObject( NewBitmap, sizeof( bm ), &bm ) ;

		// �c�[���o�[�����Ɏg���Ă��邩�ǂ����ŏ����𕪊�
		if( WinData.ToolBarUseFlag == FALSE )
		{
			TBADDBITMAP AddBitmap ;

			// ���܂Ŏg���Ă��Ȃ������ꍇ�̓c�[���o�[�E�C���h�E���쐬����
			WinAPIData.Win32Func.InitCommonControlsFunc();
			WinData.ToolBarHandle = CreateWindowExW(
				0,
				TOOLBARCLASSNAMEW,
				NULL,
				WS_CHILD | WS_VISIBLE,
				0, 0,
				0, 0,
				WinData.MainWindow,
				NULL,
				WinData.Instance,
				NULL ) ;
			if( WinData.ToolBarHandle == NULL )
			{
				DXST_ERRORLOG_ADDUTF16LE( "\xc4\x30\xfc\x30\xeb\x30\xd0\x30\xfc\x30\xa6\x30\xa4\x30\xf3\x30\xc9\x30\xa6\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�c�[���o�[�E�C���h�E�̍쐬�Ɏ��s���܂���\n" @*/ ) ;
				return -1 ;
			}

			// TBBUTTON �\���̂̃T�C�Y�𑗂��Ă���
			SendMessageW( WinData.ToolBarHandle, TB_BUTTONSTRUCTSIZE,
						 (WPARAM)sizeof( TBBUTTON ), 0 ) ;

			// �c�[���o�[���g�p���Ă����Ԃɂ���
			WinData.ToolBarUseFlag = TRUE ;

			// �r�b�g�}�b�v�t�@�C����ݒ肷��
			AddBitmap.hInst = NULL ;
			AddBitmap.nID   = (UINT_PTR)NewBitmap ;
			SendMessageW( WinData.ToolBarHandle, TB_ADDBITMAP, ( WPARAM )DivNum, ( LPARAM )&AddBitmap ) ;
			WinData.ToolBarButtonImage = NewBitmap ;

			// ��������������
			WinData.ToolBarItemNum = 0 ;
		}
		else
		{
			// ���ɍ���Ă���ꍇ�̓r�b�g�}�b�v�t�@�C������������
			TBREPLACEBITMAP RepBitmap ;

			RepBitmap.hInstOld = NULL ;
			RepBitmap.nIDOld   = (UINT_PTR)WinData.ToolBarButtonImage ;
			RepBitmap.hInstNew = NULL ;
			RepBitmap.nIDNew   = (UINT_PTR)NewBitmap ;
			RepBitmap.nButtons = DivNum ;
			SendMessageW( WinData.ToolBarHandle, TB_REPLACEBITMAP, 0, (LPARAM)&RepBitmap ) ;

			// ���܂ł̃r�b�g�}�b�v��j������
			DeleteObject( WinData.ToolBarButtonImage ) ;
			WinData.ToolBarButtonImage = NewBitmap ;
		}

		// �r�b�g�}�b�v�̃T�C�Y��ݒ肷��
		SendMessageW( WinData.ToolBarHandle, TB_SETBITMAPSIZE, 0, ( bm.bmWidth / DivNum ) | ( bm.bmHeight << 16 ) ) ;

		// �X�^�C����ύX����
		NS_SetWindowStyleMode( 6 ) ;
	}

	WM_SIZEProcess() ;

	// �I��
	return 0 ;
}

// �c�[���o�[�Ƀ{�^����ǉ�
// int Type   : TOOLBUTTON_TYPE_NORMAL ��
// int State  : TOOLBUTTON_STATE_ENABLE ��
extern int NS_AddToolBarButton( int Type, int State, int ImageIndex, int ID )
{
	WINTOOLBARITEMINFO *but ;
	TBBUTTON tbbut ;
	static const BYTE StateTable[2][TOOLBUTTON_STATE_NUM] =
	{
		{ TBSTATE_ENABLED,                   TBSTATE_ENABLED, TBSTATE_INDETERMINATE, TBSTATE_PRESSED },
		{ TBSTATE_ENABLED, TBSTATE_CHECKED | TBSTATE_ENABLED, TBSTATE_INDETERMINATE, TBSTATE_CHECKED },
	};

	static const BYTE TypeTable[] =
	{ TBSTYLE_BUTTON, TBSTYLE_CHECK, TBSTYLE_CHECKGROUP, TBSTYLE_SEP } ;

	// �c�[���o�[�̃Z�b�g�A�b�v���������Ă��Ȃ��ꍇ�͉��������ɏI��
	if( WinData.ToolBarUseFlag == FALSE ) return -1 ;

	// �c�[���o�[�̃{�^���̐����ő吔�ɒB���Ă����牽�������ɏI��
	if( WinData.ToolBarItemNum == MAX_TOOLBARITEM_NUM )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\xc4\x30\xfc\x30\xeb\x30\xd0\x30\xfc\x30\x6e\x30\xdc\x30\xbf\x30\xf3\x30\x6e\x30\x70\x65\x4c\x30\x00\x67\x27\x59\x70\x65\x6e\x30\x20\x00\x25\x00\x64\x00\x20\x00\x6b\x30\x54\x90\x57\x30\x66\x30\x44\x30\x8b\x30\xba\x70\xdc\x30\xbf\x30\xf3\x30\x92\x30\xfd\x8f\xa0\x52\x67\x30\x4d\x30\x7e\x30\x5b\x30\x93\x30\x67\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�c�[���o�[�̃{�^���̐����ő吔�� %d �ɒB���Ă���׃{�^����ǉ��ł��܂���ł���\n" @*/, MAX_TOOLBARITEM_NUM )) ;
		return -1 ;
	}

	// �p�����[�^�̒l���ُ�ȏꍇ�̓G���[
	if( Type >= TOOLBUTTON_TYPE_NUM )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\xc4\x30\xfc\x30\xeb\x30\xd0\x30\xfc\x30\x6e\x30\xdc\x30\xbf\x30\xf3\x30\xfd\x8f\xa0\x52\xa2\x95\x70\x65\x6b\x30\x4a\x30\x44\x30\x66\x30\x20\x00\x53\x00\x74\x00\x61\x00\x74\x00\x65\x00\x20\x00\x6e\x30\x24\x50\x4c\x30\x0d\x4e\x63\x6b\x6a\x30\x24\x50\x20\x00\x25\x00\x64\x00\x20\x00\x68\x30\x6a\x30\x63\x30\x66\x30\x44\x30\x7e\x30\x59\x30\x0a\x00\x00"/*@ L"�c�[���o�[�̃{�^���ǉ��֐��ɂ����� State �̒l���s���Ȓl %d �ƂȂ��Ă��܂�\n" @*/, State )) ;
		return -1 ;
	}
	if( State >= TOOLBUTTON_STATE_NUM )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\xc4\x30\xfc\x30\xeb\x30\xd0\x30\xfc\x30\x6e\x30\xdc\x30\xbf\x30\xf3\x30\xfd\x8f\xa0\x52\xa2\x95\x70\x65\x6b\x30\x4a\x30\x44\x30\x66\x30\x20\x00\x54\x00\x79\x00\x70\x00\x65\x00\x20\x00\x6e\x30\x24\x50\x4c\x30\x0d\x4e\x63\x6b\x6a\x30\x24\x50\x20\x00\x25\x00\x64\x00\x20\x00\x68\x30\x6a\x30\x63\x30\x66\x30\x44\x30\x7e\x30\x59\x30\x0a\x00\x00"/*@ L"�c�[���o�[�̃{�^���ǉ��֐��ɂ����� Type �̒l���s���Ȓl %d �ƂȂ��Ă��܂�\n" @*/, Type )) ;
		return -1 ;
	}

	// �V�����{�^����ǉ�
	_MEMSET( &tbbut, 0, sizeof( tbbut ) ) ;
	tbbut.iBitmap = ImageIndex ;
	tbbut.idCommand = TOOLBAR_COMMANDID_BASE + ID ;
	tbbut.fsState = StateTable[ Type == TOOLBUTTON_TYPE_CHECK || Type == TOOLBUTTON_TYPE_GROUP ? 1 : 0 ][ State ] ;
	tbbut.fsStyle = TypeTable[ Type ] ;
	SendMessageW( WinData.ToolBarHandle, TB_ADDBUTTONS, 1, (LPARAM)&tbbut ) ;

	// �V�����{�^���̏����Z�b�g
	but = &WinData.ToolBarItem[ WinData.ToolBarItemNum ] ;
	but->ID         = ID ;
	but->ImageIndex = ImageIndex ;
	but->Type       = Type ;
	but->State      = State ;
	but->Click      = FALSE ;

	// �{�^���̐��𑝂₷
	WinData.ToolBarItemNum ++ ;

	// �I��
	return 0 ;
}

// �c�[���o�[�Ɍ��Ԃ�ǉ�
extern int NS_AddToolBarSep( void )
{
	WINTOOLBARITEMINFO *but ;
	TBBUTTON tbbut ;

	// �c�[���o�[�̃Z�b�g�A�b�v���������Ă��Ȃ��ꍇ�͉��������ɏI��
	if( WinData.ToolBarUseFlag == FALSE ) return -1 ;

	// �c�[���o�[�̃{�^���̐����ő吔�ɒB���Ă����牽�������ɏI��
	if( WinData.ToolBarItemNum == MAX_TOOLBARITEM_NUM )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\xc4\x30\xfc\x30\xeb\x30\xd0\x30\xfc\x30\x6e\x30\xa2\x30\xa4\x30\xc6\x30\xe0\x30\x6e\x30\x70\x65\x4c\x30\x00\x67\x27\x59\x70\x65\x6e\x30\x20\x00\x25\x00\x64\x00\x20\x00\x6b\x30\x54\x90\x57\x30\x66\x30\x44\x30\x8b\x30\xba\x70\x99\x96\x93\x95\x92\x30\xfd\x8f\xa0\x52\x67\x30\x4d\x30\x7e\x30\x5b\x30\x93\x30\x67\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�c�[���o�[�̃A�C�e���̐����ő吔�� %d �ɒB���Ă���׌��Ԃ�ǉ��ł��܂���ł���\n" @*/, MAX_TOOLBARITEM_NUM )) ;
		return -1 ;
	}

	// �V�������Ԃ�ǉ�
	_MEMSET( &tbbut, 0, sizeof( tbbut ) ) ;
	tbbut.iBitmap   = 0 ;
	tbbut.idCommand = 0 ;
	tbbut.fsState   = TBSTATE_ENABLED ;
	tbbut.fsStyle   = TBSTYLE_SEP ;
	SendMessageW( WinData.ToolBarHandle, TB_ADDBUTTONS, 1, (LPARAM)&tbbut ) ;

	// �V�����{�^���̏����Z�b�g
	but = &WinData.ToolBarItem[ WinData.ToolBarItemNum ] ;
	but->ID         = 0 ;
	but->ImageIndex = 0 ;
	but->Type       = TOOLBUTTON_TYPE_SEP ;
	but->State      = TOOLBUTTON_STATE_ENABLE ;
	but->Click      = FALSE ;

	// �{�^���̐��𑝂₷
	WinData.ToolBarItemNum ++ ;

	// �I��
	return 0 ;
}

// �c�[���o�[�̃{�^���̏�Ԃ��擾
extern int NS_GetToolBarButtonState( int ID )
{
	int i ;
	WINTOOLBARITEMINFO *but ;
	int State = 0 ;
	LRESULT Result ;

	// �c�[���o�[�̃Z�b�g�A�b�v���������Ă��Ȃ��ꍇ�͉��������ɏI��
	if( WinData.ToolBarUseFlag == FALSE ) return -1 ;

	// �w��̂h�c�̃{�^����T��
	i = SearchToolBarButton( ID ) ;
	if( i == -1 )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\x07\x63\x9a\x5b\x6e\x30\x29\xff\x24\xff\x20\x00\x25\x00\x64\x00\x20\x00\x92\x30\x01\x63\x63\x30\x5f\x30\xc4\x30\xfc\x30\xeb\x30\xd0\x30\xfc\x30\x6e\x30\xdc\x30\xbf\x30\xf3\x30\x4c\x30\x42\x30\x8a\x30\x7e\x30\x5b\x30\x93\x30\x67\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�w��̂h�c %d ���������c�[���o�[�̃{�^��������܂���ł���\n" @*/, ID )) ;
		return -1;
	}
	but = &WinData.ToolBarItem[i] ;

	// �{�^���̃^�C�v�ɂ���ď����𕪊�
	switch( but->Type )
	{
	case TOOLBUTTON_TYPE_NORMAL :	// ���ʂ̃{�^���̏ꍇ
		if( but->State == TOOLBUTTON_STATE_ENABLE )
		{
			// ��x�ł������ꂽ�� TRUE ��ԂɂȂ�ϐ���Ԃ�
			State = but->Click ;
			but->Click = FALSE ;
		}
		else
		{
			return but->State == TOOLBUTTON_STATE_PRESSED ? TRUE : FALSE ;
		}
		break ;

	case TOOLBUTTON_TYPE_CHECK :	// �������Ƃɂn�m�^�n�e�e���؂�ւ��{�^��
	case TOOLBUTTON_TYPE_GROUP :	// �ʂ� TOOLBUTTON_TYPE_GROUP �^�C�v�̃{�^�����������Ƃn�e�e�ɂȂ�^�C�v�̃{�^��(�O���[�v�̋�؂�͌��Ԃ�)
		// �{�^���̏�Ԃ��擾����
		Result = SendMessageW( WinData.ToolBarHandle, TB_GETSTATE, ( WPARAM )( TOOLBAR_COMMANDID_BASE + but->ID ), 0 ) ;
		if( Result & ( TBSTATE_CHECKED | TBSTATE_PRESSED ) ) State = TRUE  ;
		else                                                 State = FALSE ;
		break ;
	}

	// ��Ԃ�Ԃ�
	return State ;
}

// �c�[���o�[�̃{�^���̏�Ԃ�ݒ�
extern int NS_SetToolBarButtonState( int ID, int State )
{
	int i ;
	int SetState = 0 ;
	WINTOOLBARITEMINFO *but ;

	// �c�[���o�[�̃Z�b�g�A�b�v���������Ă��Ȃ��ꍇ�͉��������ɏI��
	if( WinData.ToolBarUseFlag == FALSE ) return -1 ;

	// �w��̂h�c�̃{�^����T��
	i = SearchToolBarButton( ID ) ;
	if( i == -1 )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\x07\x63\x9a\x5b\x6e\x30\x29\xff\x24\xff\x20\x00\x25\x00\x64\x00\x20\x00\x92\x30\x01\x63\x63\x30\x5f\x30\xc4\x30\xfc\x30\xeb\x30\xd0\x30\xfc\x30\x6e\x30\xdc\x30\xbf\x30\xf3\x30\x4c\x30\x42\x30\x8a\x30\x7e\x30\x5b\x30\x93\x30\x67\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�w��̂h�c %d ���������c�[���o�[�̃{�^��������܂���ł���\n" @*/, ID )) ;
		return -1;
	}
	but = &WinData.ToolBarItem[i] ;

	// �{�^���̏�Ԃ�ۑ�
	but->State = State ;
	but->Click = FALSE ;

	// �{�^���̏�Ԃ�ݒ肷��
	switch( State )
	{
	case TOOLBUTTON_STATE_ENABLE  :
		SetState = TBSTATE_ENABLED ;
		break ;

	case TOOLBUTTON_STATE_PRESSED :
		if( but->Type == TOOLBUTTON_TYPE_NORMAL ) SetState = TBSTATE_PRESSED | TBSTATE_ENABLED ;
		else                                      SetState = TBSTATE_CHECKED | TBSTATE_ENABLED ;
		break ;

	case TOOLBUTTON_STATE_DISABLE :
		SetState = TBSTATE_INDETERMINATE ;
		break ;

	case TOOLBUTTON_STATE_PRESSED_DISABLE :
		SetState = TBSTATE_PRESSED | TBSTATE_INDETERMINATE ;
		break ;
	}
	SendMessageW( WinData.ToolBarHandle, TB_SETSTATE, ( WPARAM )( TOOLBAR_COMMANDID_BASE + but->ID ), SetState ) ;

	// �I��
	return 0 ;
}

// �c�[���o�[�̃{�^����S�č폜
extern int NS_DeleteAllToolBarButton( void )
{
	int i ;

	// �c�[���o�[�̃Z�b�g�A�b�v���������Ă��Ȃ��ꍇ�͉��������ɏI��
	if( WinData.ToolBarUseFlag == FALSE ) return -1 ;

	// �{�^���̐������폜���b�Z�[�W�𑗂�
	for( i = 0 ; i < WinData.ToolBarItemNum ; i ++ )
		SendMessageW( WinData.ToolBarHandle, TB_DELETEBUTTON, 0, 0 ) ;

	// �{�^���̐����O�ɂ���
	WinData.ToolBarItemNum = 0 ;

	// �I��
	return 0 ;
}

















// ���j���[�֌W

// ���j���[��L���ɂ��邩�ǂ�����ݒ肷��
extern int NS_SetUseMenuFlag( int Flag )
{
	// �������܂łƓ����ꍇ�͉��������I��
	if( Flag == WinData.MenuUseFlag ) return 0 ;

	// �t���O�ɂ���ď����𕪊�
	if( Flag == FALSE )
	{
		// ���j���[���폜����
		if( WinData.Menu != NULL )
		{
			// ���j���[���E�C���h�E����O��
			NS_SetDisplayMenuFlag( FALSE ) ;

			// ���j���[���폜
			DestroyMenu( WinData.Menu ) ;
			WinData.Menu = NULL ;
		}
		
		// ���j���[�𖳌��ɂ���
		WinData.MenuUseFlag = FALSE ;
	}
	else
	{
		// ���j���[�������ꍇ�͋�̃��j���[���쐬����
		if( WinData.Menu == NULL )
		{
			WinData.Menu = CreateMenu() ;
			if( WinData.Menu == NULL ) return -1 ;
		}
		
		// ���j���[��L���ɂ���
		WinData.MenuUseFlag = TRUE ;
		
		// ������Ԃŕ\��
		WinData.MenuDisplayState = FALSE ;
		NS_SetDisplayMenuFlag( TRUE ) ;
	}

	// �I�����ꂽ���j���[���ڂ̐���������
	WinData.SelectMenuItemNum = 0 ;

	// ���j���[���ڂ̐�������������
	WinData.MenuItemInfoNum = 0 ;

	// �I��
	return 0 ;
}

// �L�[�{�[�h�A�N�Z�����[�^�[���g�p���邩�ǂ�����ݒ肷��
extern int NS_SetUseKeyAccelFlag( int Flag )
{
	if( WinData.AltF4_EndFlag == TRUE ) Flag = FALSE ;

	WinData.UseAccelFlag = Flag ;
	if( Flag == FALSE ) NS_ClearKeyAccel() ;

	// �I��
	return 0 ;
}

// �V���[�g�J�b�g�L�[��ǉ�����
extern int NS_AddKeyAccel( const TCHAR *ItemName, int ItemID,
							int KeyCode, int CtrlFlag, int AltFlag, int ShiftFlag )
{
#ifdef UNICODE
	return AddKeyAccel_WCHAR_T( ItemName, ItemID, KeyCode, CtrlFlag, AltFlag, ShiftFlag ) ;
#else
	int Result ;

	TCHAR_TO_WCHAR_T_STRING_ONE_BEGIN( ItemName, return -1 )

	Result = AddKeyAccel_WCHAR_T( UseItemNameBuffer, ItemID, KeyCode, CtrlFlag, AltFlag, ShiftFlag ) ;

	TCHAR_TO_WCHAR_T_STRING_END( ItemName )

	return Result ;
#endif
}

// �V���[�g�J�b�g�L�[��ǉ�����
extern int AddKeyAccel_WCHAR_T( const wchar_t *ItemName, int ItemID,
							int KeyCode, int CtrlFlag, int AltFlag, int ShiftFlag )
{
#ifdef DX_NON_INPUT

	return -1 ;

#else // DX_NON_INPUT

	HACCEL NewAccel ;
	ACCEL *Accel, *ac ;
	int AccelNum ;
	WINMENUITEMINFO *WinItemInfo ;

	if( WinData.MenuUseFlag == FALSE )
	{
		goto ERR ;
	}

	// �w��̑I�����ڂ�������Ȃ������牽�����Ȃ�
	WinItemInfo = SearchMenuItemInfo( ItemName, ItemID ) ;
	if( WinItemInfo == NULL )
	{
		goto ERR ;
	}

	if( WinData.UseAccelFlag == FALSE )
	{
		NS_SetUseKeyAccelFlag( TRUE ) ;
	}

	if( WinData.UseAccelFlag == FALSE )
	{
		goto ERR ;
	}

	// ���ɃA�N�Z�����[�^�[���ݒ肳��Ă��邩�ǂ����ŏ����𕪊�
	if( WinData.Accel != NULL )
	{
		// ���ɐݒ肳��Ă���ꍇ�͌��݂̃A�N�Z�����[�^�[�̏����擾����
		AccelNum = CopyAcceleratorTableW( WinData.Accel, NULL, 0 ) ;

		// �f�[�^���������̂Ő�������₷
		AccelNum ++ ;

		// �f�[�^���i�[���邽�߂̃������̈���m��
		Accel = (ACCEL *)DXALLOC( sizeof( ACCEL ) * AccelNum ) ;
		if( Accel == NULL ) goto ERR ;

		// ���݂̃f�[�^���擾����
		CopyAcceleratorTableW( WinData.Accel, Accel, AccelNum ) ;
	}
	else
	{
		// �f�[�^���ЂƂ������郁�����̈���m��
		AccelNum = 1 ;
		Accel = (ACCEL *)DXALLOC( sizeof( ACCEL ) * AccelNum ) ;
		if( Accel == NULL ) goto ERR ;
	}

	// �A�N�Z�����[�^�̏����쐬
	ac = &Accel[AccelNum-1] ;
	ac->fVirt = FNOINVERT | FVIRTKEY ;
	if( CtrlFlag != FALSE ) ac->fVirt |= FCONTROL ;
	if( AltFlag != FALSE ) ac->fVirt |= FALT ;
	if( ShiftFlag != FALSE ) ac->fVirt |= FSHIFT ;
	ac->key = (WORD)NS_ConvertKeyCodeToVirtualKey( KeyCode ) ;
	ac->cmd = (WORD)WinItemInfo->ID ;

	// �A�N�Z�����[�^���쐬
	NewAccel = CreateAcceleratorTableW( Accel, AccelNum ) ;
	if( NewAccel == NULL ) goto ERR ;

	// �������̉��
	_MEMSET( Accel, 0, sizeof( ACCEL ) * AccelNum ) ;
	DXFREE( Accel ) ;

	// �쐬�ɐ��������猻�݂̃A�N�Z�����[�^���폜����
	if( WinData.Accel != NULL )
		DestroyAcceleratorTable( WinData.Accel ) ;

	// �V�����A�N�Z�����[�^���Z�b�g����
	WinData.Accel = NewAccel ;

	// �I��
	return 0 ;
	
ERR:
	return -1 ;
#endif // DX_NON_INPUT
}

// �V���[�g�J�b�g�L�[��ǉ�����
extern int NS_AddKeyAccel_Name( const TCHAR *ItemName, int KeyCode, int CtrlFlag, int AltFlag, int ShiftFlag )
{
	return NS_AddKeyAccel( ItemName, 0, KeyCode, CtrlFlag, AltFlag, ShiftFlag ) ;
}

// �V���[�g�J�b�g�L�[��ǉ�����
extern int AddKeyAccel_Name_WCHAR_T( const wchar_t *ItemName , int KeyCode , int CtrlFlag , int AltFlag , int ShiftFlag )
{
	return AddKeyAccel_WCHAR_T( ItemName, 0, KeyCode, CtrlFlag, AltFlag, ShiftFlag ) ;
}

// �V���[�g�J�b�g�L�[��ǉ�����
extern int NS_AddKeyAccel_ID( int ItemID, int KeyCode, int CtrlFlag, int AltFlag, int ShiftFlag )
{
	return NS_AddKeyAccel( NULL, ItemID, KeyCode, CtrlFlag, AltFlag, ShiftFlag ) ;
}

// �V���[�g�J�b�g�L�[�̏�������������
extern int NS_ClearKeyAccel( void )
{
	// �A�N�Z�����[�^���L���ɂȂ��Ă����ꍇ�͍폜����
	if( WinData.Accel != NULL )
	{
		DestroyAcceleratorTable( WinData.Accel ) ;
		WinData.Accel = NULL ;
	}

	// �I��
	return 0 ;
}

// ���j���[�ɍ��ڂ�ǉ�����
extern int NS_AddMenuItem( int AddType, const TCHAR *ItemName, int ItemID,
						int SeparatorFlag, const TCHAR *NewItemName, int NewItemID )
{
#ifdef UNICODE
	return AddMenuItem_WCHAR_T( AddType, ItemName,ItemID,
								SeparatorFlag, NewItemName, NewItemID ) ;
#else
	int Result = -1 ;

	TCHAR_TO_WCHAR_T_STRING_BEGIN( ItemName )
	TCHAR_TO_WCHAR_T_STRING_BEGIN( NewItemName )

	TCHAR_TO_WCHAR_T_STRING_SETUP( ItemName,    goto ERR )
	TCHAR_TO_WCHAR_T_STRING_SETUP( NewItemName, goto ERR )

	Result = AddMenuItem_WCHAR_T( AddType, UseItemNameBuffer, ItemID,
								SeparatorFlag, UseNewItemNameBuffer, NewItemID ) ;

ERR:

	TCHAR_TO_WCHAR_T_STRING_END( ItemName )
	TCHAR_TO_WCHAR_T_STRING_END( NewItemName )

	return Result ;
#endif
}

// ���j���[�ɍ��ڂ�ǉ�����
extern int AddMenuItem_WCHAR_T( int AddType, const wchar_t *ItemName, int ItemID,
						int SeparatorFlag, const wchar_t *NewItemName, int NewItemID )
{
	HMENU Menu = NULL ;
	int AddIndex = 0 ;
	MENUITEMINFOW ItemInfo ;
	WINMENUITEMINFO *WItemInfo ;

	if( WinData.AltF4_EndFlag != 0 ) return -1 ;

	// ��񂪈�t��������G���[
	if( WinData.MenuItemInfoNum == MAX_MENUITEM_NUM ) return -1 ;

	// ���j���[��������������L���ɂ���
	if( WinData.MenuUseFlag == FALSE ) NS_SetUseMenuFlag( TRUE ) ;

	// �ǉ��̃^�C�v�ɂ���ď����𕪊�
	switch( AddType )
	{
	case MENUITEM_ADD_CHILD :	// �w��̍��ڂ̎q�Ƃ��Ēǉ�����ꍇ
		{
			// �e�̎w�肪���邩�ǂ����ŏ����𕪊�
			if( ItemID == MENUITEM_IDTOP )
			{
				Menu = WinData.Menu ;
			}
			else
			{
				// �w��̑I�����ڂ̃T�u���j���[���擾����(����������쐬����)
				Menu = MenuItemSubMenuSetup( ItemName, ItemID ) ;

				// �w��̑I�����ڂ������������ԏ�ɒǉ�����
				if( Menu == NULL )
				{
					Menu = WinData.Menu ;
				}
			}

			// �ǉ�����T�u���j���[���̖����ɂ���
			AddIndex = GetMenuItemCount( Menu ) ;
		}
		break ;
		
	case MENUITEM_ADD_INSERT :	// �w��̍��ڂƎw��̍��ڂ����̍��ڂ̊Ԃɒǉ�����ꍇ
		{
			WINMENUITEMINFO *WinItemInfo ;

			// �w��̍��ڂ̏����擾����
			WinItemInfo = SearchMenuItemInfo( ItemName, ItemID ) ;
			
			// ����������I�����ڂ̈�ԏ�ɒǉ�����
			if( WinItemInfo == NULL )
			{
				Menu = WinData.Menu ;
				AddIndex = GetMenuItemCount( Menu ) ;
			}
			else
			{
				// �݂����炻�̍��ڂƂ��̍��ڂ����̍��ڂ̊Ԃɒǉ�����
				Menu = WinItemInfo->Menu ;
				AddIndex = WinItemInfo->Index ;
			}
		}
		break ;
	}

	// ��؂�����ǂ����ŏ����𕪊�
	if( SeparatorFlag == TRUE )
	{
		// ��؂���̏ꍇ

		// ��؂���̏����Z�b�g����
		_MEMSET( &ItemInfo, 0, sizeof( ItemInfo ) ) ;
		ItemInfo.cbSize = sizeof( MENUITEMINFOW ) ;	// �\���̂̃T�C�Y
		ItemInfo.fMask = MIIM_TYPE ;				// �擾�܂��͐ݒ肷�郁���o
		ItemInfo.fType = MFT_SEPARATOR ; 			// �A�C�e���̃^�C�v

		// ��؂�����ڂ̒ǉ�
		if( InsertMenuItemW( Menu, ( UINT )AddIndex, TRUE, &ItemInfo ) == 0 )
		{
			return -1 ;
		}
	}
	else
	{
		// �I�����ڂ̏ꍇ

		// �V�����A�C�e���̂h�c���|�P�������ꍇ�g���Ă��Ȃ��h�c��t����
		if( NewItemID == -1 )
			NewItemID = GetNewMenuItemID() ;
		WItemInfo = &WinData.MenuItemInfo[ WinData.MenuItemInfoNum ] ;

		// �I�����ڂ�ǉ�����
		{
			// �V�������ڂ̏����Z�b�g����
			_MEMSET( &ItemInfo, 0, sizeof( ItemInfo ) ) ;
			ItemInfo.cbSize     = sizeof( MENUITEMINFOW ) ;				// �\���̂̃T�C�Y
			ItemInfo.fMask      = MIIM_STATE | MIIM_TYPE | MIIM_ID ;	// �擾�܂��͐ݒ肷�郁���o
			ItemInfo.fType      = MFT_STRING ;							// �A�C�e���̃^�C�v
			ItemInfo.fState     = MFS_ENABLED ;							// �A�C�e���̏��
			ItemInfo.wID        = ( UINT )NewItemID ;					// �A�C�e��ID
			_WCSCPY_S( WItemInfo->Name, sizeof( WItemInfo->Name ), NewItemName ) ;
			ItemInfo.dwTypeData = WItemInfo->Name ;						// �A�C�e���̓��e�Z�b�g
			ItemInfo.cch        = ( UINT )_WCSLEN( WItemInfo->Name ) ;		// �A�C�e���̕�����̒���

			// ���ڂ̒ǉ�
			if( InsertMenuItemW( Menu, ( UINT )AddIndex, TRUE, &ItemInfo ) == 0 )
			{
				return -1 ;
			}
		}

		// ���j���[���ڂ̏���ǉ�����
		AddMenuItemInfo( Menu, AddIndex, NewItemID, NewItemName ) ;
	}

	// ��ԍŏ��̃��j���[�o�[�ւ̍��ڒǉ��̏ꍇ�̓E�C���h�E�T�C�Y�𒲐�����
	if( WinData.WindowModeFlag == TRUE && Menu == WinData.Menu && GetMenuItemCount( WinData.Menu ) == 1 )
	{
		SetWindowStyle( FALSE ) ;
	}

	// ���j���[���ĕ`�悷��
	if( GetDisplayMenuState() == TRUE )
	{
#ifndef DX_NON_GRAPHICS
		if( GSYS.Setting.ValidHardware )
		{
			Graphics_Win_SetDialogBoxMode( TRUE )  ;
		}
#endif // DX_NON_GRAPHICS

		DrawMenuBar( WinData.MainWindow ) ;
	}

	// �}���̏ꍇ�̓��X�g���č\�z����
	if( AddType == MENUITEM_ADD_INSERT )
	{
		WinData.MenuItemInfoNum = 0 ;
		ListupMenuItemInfo( WinData.Menu ) ;
	}

	// �I��
	return 0 ;
}

// ���j���[���獀�ڂ��폜����
extern int NS_DeleteMenuItem( const TCHAR *ItemName, int ItemID )
{
#ifdef UNICODE
	return DeleteMenuItem_WCHAR_T( ItemName, ItemID ) ;
#else
	int Result ;

	TCHAR_TO_WCHAR_T_STRING_ONE_BEGIN( ItemName, return -1 )

	Result = DeleteMenuItem_WCHAR_T( UseItemNameBuffer, ItemID ) ;

	TCHAR_TO_WCHAR_T_STRING_END( ItemName )

	return Result ;
#endif
}

// ���j���[���獀�ڂ��폜����
extern int DeleteMenuItem_WCHAR_T( const wchar_t *ItemName, int ItemID )
{
	HMENU Menu ;
	int Index ;

	if( WinData.MenuUseFlag == FALSE )
	{
		return -1 ;
	}

	// �w��̍��ڂ�T��
	if( SearchMenuItem( ItemName, ItemID, WinData.Menu, &Menu, &Index ) != 1 )
	{
		return -1 ;
	}

	// �I�����ڂ��폜����
	DeleteMenu( Menu, ( UINT )Index, MF_BYPOSITION ) ;

	// ���X�g���č\�z����
	WinData.MenuItemInfoNum = 0 ;
	ListupMenuItemInfo( WinData.Menu ) ;

	// ���j���[���ĕ`�悷��
	if( GetDisplayMenuState() == TRUE )
	{
#ifndef DX_NON_GRAPHICS
		if( GSYS.Setting.ValidHardware )
		{
			Graphics_Win_SetDialogBoxMode( TRUE )  ;
		}
#endif // DX_NON_GRAPHICS

		DrawMenuBar( WinData.MainWindow ) ;
	}

	// �I��
	return 0 ;
}

// ���j���[���I�����ꂽ���ǂ������擾����( 0:�I������Ă��Ȃ�  1:�I�����ꂽ )
extern int NS_CheckMenuItemSelect( const TCHAR *ItemName, int ItemID )
{
#ifdef UNICODE
	return CheckMenuItemSelect_WCHAR_T( ItemName, ItemID ) ;
#else
	int Result ;

	TCHAR_TO_WCHAR_T_STRING_ONE_BEGIN( ItemName, return -1 )

	Result = CheckMenuItemSelect_WCHAR_T( UseItemNameBuffer, ItemID ) ;

	TCHAR_TO_WCHAR_T_STRING_END( ItemName )

	return Result ;
#endif
}

// ���j���[���I�����ꂽ���ǂ������擾����( 0:�I������Ă��Ȃ�  1:�I�����ꂽ )
extern int CheckMenuItemSelect_WCHAR_T( const wchar_t *ItemName, int ItemID )
{
	int i, InfoNum ;

	if( WinData.MenuUseFlag == FALSE ) return -1 ;

	// ���O���w�肳��Ă����ꍇ�͂h�c���擾����
	if( ItemName != NULL )
	{
		ItemID = GetMenuItemID_WCHAR_T( ItemName ) ;
	}

	// �w��̃A�C�e�������݂��邩���ׂ�
	InfoNum = WinData.SelectMenuItemNum ;
	for( i = 0 ; i < InfoNum ; i ++ )
		if( WinData.SelectMenuItem[i] == ItemID ) break ;

	// ����������O��Ԃ�
	if( i == InfoNum ) return 0 ;
	
	// �݂�����A��񂩂�O��
	if( i != InfoNum - 1 )
		_MEMMOVE( &WinData.SelectMenuItem[i], &WinData.SelectMenuItem[i+1], sizeof( int ) * ( InfoNum - i - 1 ) ) ;
	
	// ���̐������炷
	WinData.SelectMenuItemNum -- ;

	// �������̂łP��Ԃ�
	return 1 ;
}

// ���j���[�̑S�Ă̑I�����ڂ��폜����
extern int NS_DeleteMenuItemAll( void )
{
	HMENU NewMenu ;
	int DispFlag ;

	if( WinData.MenuUseFlag == FALSE ) return -1 ;
	
	// �V�������j���[���쐬
	NewMenu = CreateMenu() ;
	if( NewMenu == NULL ) return -1 ;

	DispFlag = NS_GetDisplayMenuFlag() ;
	
	// ��{�̃��j���[���폜���č�蒼��
	if( WinData.Menu != NULL )
	{
		// �Ƃ肠�����E�C���h�E����O��
		NS_SetDisplayMenuFlag( FALSE ) ;
		
		// �t���X�N���[���̏ꍇ
		if( WinData.WindowModeFlag == FALSE )
		{
			// ���j���[���O��
			if( WinData.MenuSetupFlag == TRUE )
			{
				SetMenu( WinData.MainWindow, NULL ) ;
				WinData.MenuSetupFlag = FALSE ;
			}
		}

		// ���j���[���폜
		DestroyMenu( WinData.Menu ) ;
	}

	// �V�������j���[���Z�b�g
	WinData.Menu = NewMenu ;

	// �\����Ԃ����ɖ߂�
	NS_SetDisplayMenuFlag( DispFlag ) ;

	// �t���X�N���[���̏ꍇ
	if( WinData.WindowModeFlag == FALSE )
	{
		// ���j���[�̃Z�b�g
		if( WinData.MenuSetupFlag == FALSE && WinData.MenuDisplayState != FALSE )
		{
			SetMenu( WinData.MainWindow, WinData.Menu ) ;
			WinData.MenuSetupFlag = TRUE ;
		}
	}

	// �I�����ꂽ���j���[���ڂ̐���������
	WinData.SelectMenuItemNum = 0 ;

	// ���X�g��������
	WinData.MenuItemInfoNum = 0 ;

	// �I��
	return 0 ;
}

// ���j���[���I�����ꂽ���ǂ����̏���������
extern int NS_ClearMenuItemSelect( void )
{
	WinData.SelectMenuItemNum = 0 ;

	// �I��
	return 0 ;
}

// ���j���[�̍��ڂ�I���o���邩�ǂ�����ݒ肷��
extern int NS_SetMenuItemEnable( const TCHAR *ItemName, int ItemID, int EnableFlag )
{
#ifdef UNICODE
	return SetMenuItemEnable_WCHAR_T( ItemName, ItemID, EnableFlag ) ;
#else
	int Result ;

	TCHAR_TO_WCHAR_T_STRING_ONE_BEGIN( ItemName, return -1 )

	Result = SetMenuItemEnable_WCHAR_T( UseItemNameBuffer, ItemID, EnableFlag ) ;

	TCHAR_TO_WCHAR_T_STRING_END( ItemName )

	return Result ;
#endif
}

// ���j���[�̍��ڂ�I���o���邩�ǂ�����ݒ肷��
extern int SetMenuItemEnable_WCHAR_T( const wchar_t *ItemName, int ItemID, int EnableFlag )
{
	WINMENUITEMINFO *WinItemInfo ;
	
	if( WinData.MenuUseFlag == FALSE ) return -1 ;

	// �I�����ڂ̏����擾
	WinItemInfo = SearchMenuItemInfo( ItemName, ItemID ) ;
	if( WinItemInfo == NULL ) return -1 ;

	// �I���o���邩�ǂ�����ݒ肷��
	EnableMenuItem( WinItemInfo->Menu, ( UINT )WinItemInfo->Index, ( UINT )( MF_BYPOSITION | ( EnableFlag == TRUE ? MF_ENABLED : MF_GRAYED ) ) ) ;

	// �\�����X�V����
	if( GetDisplayMenuState() == TRUE )
	{
#ifndef DX_NON_GRAPHICS
		if( GSYS.Setting.ValidHardware )
		{
			Graphics_Win_SetDialogBoxMode( TRUE )  ;
		}
#endif // DX_NON_GRAPHICS

		DrawMenuBar( WinData.MainWindow ) ;
	}

	// �I��
	return 0 ;
}

// ���j���[�̍��ڂɃ`�F�b�N�}�[�N�⃉�W�I�{�^����\�����邩�ǂ�����ݒ肷��
extern int NS_SetMenuItemMark( const TCHAR *ItemName, int ItemID, int Mark )
{
#ifdef UNICODE
	return SetMenuItemMark_WCHAR_T( ItemName, ItemID, Mark ) ;
#else
	int Result ;

	TCHAR_TO_WCHAR_T_STRING_ONE_BEGIN( ItemName, return -1 )

	Result = SetMenuItemMark_WCHAR_T( UseItemNameBuffer, ItemID, Mark ) ;

	TCHAR_TO_WCHAR_T_STRING_END( ItemName )

	return Result ;
#endif
}

// ���j���[�̍��ڂɃ`�F�b�N�}�[�N�⃉�W�I�{�^����\�����邩�ǂ�����ݒ肷��
extern int SetMenuItemMark_WCHAR_T( const wchar_t *ItemName, int ItemID, int Mark )
{
	MENUITEMINFOW ItemInfo ;
	WINMENUITEMINFO *WinItemInfo ;

	if( WinData.MenuUseFlag == FALSE ) return -1 ;
	
	// �I�����ڂ̏����擾
	WinItemInfo = SearchMenuItemInfo( ItemName, ItemID ) ;
	if( WinItemInfo == NULL ) return -1 ;

	// ���݂̏��𓾂�
	_GetMenuItemInfo( WinItemInfo->Menu, WinItemInfo->Index, &ItemInfo ) ;
	
	// ��������������
	switch( Mark )
	{
	case MENUITEM_MARK_NONE :	// �����t���Ȃ�
		ItemInfo.fMask |= MIIM_TYPE ;
		ItemInfo.fState &= ~MFS_CHECKED ;
		ItemInfo.fType &= ~MFT_RADIOCHECK ;
		break ;
		
	case MENUITEM_MARK_CHECK :	// ���ʂ̃`�F�b�N
		ItemInfo.fMask |= MIIM_TYPE ;
		ItemInfo.fType &= ~MFT_RADIOCHECK ;
		ItemInfo.fState |= MFS_CHECKED ;
		ItemInfo.hbmpChecked = NULL ;
		break ;

	case MENUITEM_MARK_RADIO :	// ���W�I�{�^��
		ItemInfo.fMask |= MIIM_TYPE ;
		ItemInfo.fType = MFT_RADIOCHECK ;
		ItemInfo.fState |= MFS_CHECKED ;
		ItemInfo.hbmpChecked = NULL ;
		break ;
	}
	SetMenuItemInfoW( WinItemInfo->Menu, ( UINT )WinItemInfo->Index, TRUE, &ItemInfo ) ;
	
	// �\�����X�V����
	if( GetDisplayMenuState() == TRUE )
	{
#ifndef DX_NON_GRAPHICS
		if( GSYS.Setting.ValidHardware )
		{
			Graphics_Win_SetDialogBoxMode( TRUE )  ;
		}
#endif // DX_NON_GRAPHICS

		DrawMenuBar( WinData.MainWindow ) ;
	}

	// �I��
	return 0 ;
}

// ���j���[�̍��ڂ��ǂꂩ�I�����ꂽ���ǂ������擾����( �߂�l  TRUE:�ǂꂩ�I�����ꂽ  FALSE:�I������Ă��Ȃ� )
extern int NS_CheckMenuItemSelectAll( void )
{
	return WinData.SelectMenuItemNum != 0 ? TRUE : FALSE ;
}

// ���j���[�ɍ��ڂ�ǉ�����
extern int NS_AddMenuItem_Name( const TCHAR *ParentItemName, const TCHAR *NewItemName )
{
	if( ParentItemName == NULL )
	{
		return NS_AddMenuItem( MENUITEM_ADD_CHILD, NULL, MENUITEM_IDTOP,
								 FALSE, NewItemName, -1 ) ;
	}
	else
	{
		return NS_AddMenuItem( MENUITEM_ADD_CHILD, ParentItemName, -1,
								 FALSE, NewItemName, -1 ) ;
	}
}

// ���j���[�ɍ��ڂ�ǉ�����
extern int AddMenuItem_Name_WCHAR_T( const wchar_t *ParentItemName, const wchar_t *NewItemName )
{
	if( ParentItemName == NULL )
	{
		return AddMenuItem_WCHAR_T( MENUITEM_ADD_CHILD, NULL, MENUITEM_IDTOP,
								 FALSE, NewItemName, -1 ) ;
	}
	else
	{
		return AddMenuItem_WCHAR_T( MENUITEM_ADD_CHILD, ParentItemName, -1,
								 FALSE, NewItemName, -1 ) ;
	}
}

// �w��̍��ڂƁA�w��̍��ڂ̈��̍��ڂƂ̊ԂɐV�������ڂ�ǉ�����
extern int NS_InsertMenuItem_Name( const TCHAR *ItemName, const TCHAR *NewItemName )
{
	if( ItemName == NULL )
	{
		return NS_AddMenuItem( MENUITEM_ADD_INSERT, NULL, MENUITEM_IDTOP,
								FALSE, NewItemName, -1 ) ;
	}
	else
	{
		return NS_AddMenuItem( MENUITEM_ADD_INSERT, ItemName, -1,
								FALSE, NewItemName, -1 ) ;
	}
}

// �w��̍��ڂƁA�w��̍��ڂ̈��̍��ڂƂ̊ԂɐV�������ڂ�ǉ�����
extern int InsertMenuItem_Name_WCHAR_T( const wchar_t *ItemName, const wchar_t *NewItemName )
{
	if( ItemName == NULL )
	{
		return AddMenuItem_WCHAR_T( MENUITEM_ADD_INSERT, NULL, MENUITEM_IDTOP,
								FALSE, NewItemName, -1 ) ;
	}
	else
	{
		return AddMenuItem_WCHAR_T( MENUITEM_ADD_INSERT, ItemName, -1,
								FALSE, NewItemName, -1 ) ;
	}
}

// ���j���[�̃��X�g�ɋ�؂����ǉ�����
extern int NS_AddMenuLine_Name( const TCHAR *ParentItemName )
{
	if( ParentItemName == NULL )
	{
		return NS_AddMenuItem( MENUITEM_ADD_CHILD, NULL, MENUITEM_IDTOP,
								TRUE, NULL, -1 ) ;
	}
	else
	{
		return NS_AddMenuItem( MENUITEM_ADD_CHILD, ParentItemName, -1,
								TRUE, NULL, -1 ) ;
	}
}

// ���j���[�̃��X�g�ɋ�؂����ǉ�����
extern int AddMenuLine_Name_WCHAR_T( const wchar_t *ParentItemName )
{
	if( ParentItemName == NULL )
	{
		return AddMenuItem_WCHAR_T( MENUITEM_ADD_CHILD, NULL, MENUITEM_IDTOP,
								TRUE, NULL, -1 ) ;
	}
	else
	{
		return AddMenuItem_WCHAR_T( MENUITEM_ADD_CHILD, ParentItemName, -1,
								TRUE, NULL, -1 ) ;
	}
}

// �w��̍��ڂƁA�w��̍��ڂ̈��̍��ڂƂ̊Ԃɋ�؂����ǉ�����
extern int NS_InsertMenuLine_Name( const TCHAR *ItemName )
{
	if( ItemName == NULL )
	{
		return NS_AddMenuItem( MENUITEM_ADD_INSERT, NULL, MENUITEM_IDTOP,
							TRUE, NULL, -1 ) ;
	}
	else
	{
		return NS_AddMenuItem( MENUITEM_ADD_INSERT, ItemName, -1,
								TRUE, NULL, -1 ) ;
	}
}

// �w��̍��ڂƁA�w��̍��ڂ̈��̍��ڂƂ̊Ԃɋ�؂����ǉ�����
extern int InsertMenuLine_Name_WCHAR_T( const wchar_t *ItemName )
{
	if( ItemName == NULL )
	{
		return AddMenuItem_WCHAR_T( MENUITEM_ADD_INSERT, NULL, MENUITEM_IDTOP,
							TRUE, NULL, -1 ) ;
	}
	else
	{
		return AddMenuItem_WCHAR_T( MENUITEM_ADD_INSERT, ItemName, -1,
								TRUE, NULL, -1 ) ;
	}
}

// ���j���[���獀�ڂ��폜����
extern int NS_DeleteMenuItem_Name( const TCHAR *ItemName )
{
	return NS_DeleteMenuItem( ItemName, -1 ) ;
}

// ���j���[���獀�ڂ��폜����
extern int DeleteMenuItem_Name_WCHAR_T( const wchar_t *ItemName )
{
	return DeleteMenuItem_WCHAR_T( ItemName, -1 ) ;
}

// ���j���[���I�����ꂽ���ǂ������擾����( 0:�I������Ă��Ȃ�  1:�I�����ꂽ )
extern int NS_CheckMenuItemSelect_Name( const TCHAR *ItemName )
{
	return NS_CheckMenuItemSelect( ItemName, -1 ) ;
}

// ���j���[���I�����ꂽ���ǂ������擾����( 0:�I������Ă��Ȃ�  1:�I�����ꂽ )
extern int CheckMenuItemSelect_Name_WCHAR_T( const wchar_t *ItemName )
{
	return CheckMenuItemSelect_WCHAR_T( ItemName, -1 ) ;
}

// ���j���[�̍��ڂ�I���o���邩�ǂ�����ݒ肷��
extern int NS_SetMenuItemEnable_Name( const TCHAR *ItemName, int EnableFlag )
{
	return NS_SetMenuItemEnable( ItemName, -1, EnableFlag ) ;
}

// ���j���[�̍��ڂ�I���o���邩�ǂ�����ݒ肷��
extern int SetMenuItemEnable_Name_WCHAR_T( const wchar_t *ItemName, int EnableFlag )
{
	return SetMenuItemEnable_WCHAR_T( ItemName, -1, EnableFlag ) ;
}

// ���j���[�̍��ڂɃ`�F�b�N�}�[�N�⃉�W�I�{�^����\�����邩�ǂ�����ݒ肷��
extern int NS_SetMenuItemMark_Name( const TCHAR *ItemName, int Mark )
{
	return NS_SetMenuItemMark( ItemName, -1, Mark ) ;
}

// ���j���[�̍��ڂɃ`�F�b�N�}�[�N�⃉�W�I�{�^����\�����邩�ǂ�����ݒ肷��
extern int SetMenuItemMark_Name_WCHAR_T( const wchar_t *ItemName, int Mark )
{
	return SetMenuItemMark_WCHAR_T( ItemName, -1, Mark ) ;
}


// ���j���[�ɍ��ڂ�ǉ�����
extern int NS_AddMenuItem_ID( int ParentItemID, const TCHAR *NewItemName, int NewItemID )
{
	return NS_AddMenuItem( MENUITEM_ADD_CHILD, NULL, ParentItemID,
							FALSE, NewItemName, NewItemID ) ;
}

// ���j���[�ɍ��ڂ�ǉ�����
extern int AddMenuItem_ID_WCHAR_T( int ParentItemID, const wchar_t *NewItemName, int NewItemID )
{
	return AddMenuItem_WCHAR_T( MENUITEM_ADD_CHILD, NULL, ParentItemID,
							FALSE, NewItemName, NewItemID ) ;
}

// �w��̍��ڂƁA�w��̍��ڂ̈��̍��ڂƂ̊ԂɐV�������ڂ�ǉ�����
extern int NS_InsertMenuItem_ID( int ItemID, int NewItemID )
{
	return NS_AddMenuItem( MENUITEM_ADD_INSERT, NULL, ItemID,
							FALSE, NULL, NewItemID ) ;
}

// ���j���[�̃��X�g�ɋ�؂����ǉ�����
extern int NS_AddMenuLine_ID( int ParentItemID )
{
	return NS_AddMenuItem( MENUITEM_ADD_CHILD, NULL, ParentItemID,
							TRUE, NULL, -1 ) ;
}

// �w��̍��ڂƁA�w��̍��ڂ̈��̍��ڂƂ̊Ԃɋ�؂����ǉ�����
extern int NS_InsertMenuLine_ID( int ItemID, int NewItemID )
{
	return NS_AddMenuItem( MENUITEM_ADD_INSERT, NULL, ItemID,
							TRUE, NULL, NewItemID ) ;
}

// ���j���[���獀�ڂ��폜����
extern int NS_DeleteMenuItem_ID( int ItemID )
{
	return NS_DeleteMenuItem( NULL, ItemID ) ;
}

// ���j���[���I�����ꂽ���ǂ������擾����( 0:�I������Ă��Ȃ�  1:�I�����ꂽ )
extern int NS_CheckMenuItemSelect_ID( int ItemID )
{
	return NS_CheckMenuItemSelect( NULL, ItemID ) ;
}

// ���j���[�̍��ڂ�I���o���邩�ǂ�����ݒ肷��
extern int NS_SetMenuItemEnable_ID( int ItemID, int EnableFlag )
{
	return NS_SetMenuItemEnable( NULL, ItemID, EnableFlag ) ;
}

// ���j���[�̍��ڂɃ`�F�b�N�}�[�N�⃉�W�I�{�^����\�����邩�ǂ�����ݒ肷��
extern int NS_SetMenuItemMark_ID( int ItemID, int Mark )
{
	return NS_SetMenuItemMark( NULL, ItemID, Mark ) ;
}


// ���j���[�̍��ږ�����h�c���擾����
extern int NS_GetMenuItemID( const TCHAR *ItemName )
{
#ifdef UNICODE
	return GetMenuItemID_WCHAR_T( ItemName ) ;
#else
	int Result ;

	TCHAR_TO_WCHAR_T_STRING_ONE_BEGIN( ItemName, return -1 )

	Result = GetMenuItemID_WCHAR_T( UseItemNameBuffer ) ;

	TCHAR_TO_WCHAR_T_STRING_END( ItemName )

	return Result ;
#endif
}

// ���j���[�̍��ږ�����h�c���擾����
extern int GetMenuItemID_WCHAR_T( const wchar_t *ItemName )
{
	WINMENUITEMINFO *WinItemInfo ;
	int Result ;

	if( WinData.MenuUseFlag == FALSE ) return -1 ;
	
	// �I�����ڂ̏����擾
	WinItemInfo = SearchMenuItemInfo( ItemName, -1 ) ;
	if( WinItemInfo == NULL ) return -1 ;
	Result = (int)WinItemInfo->ID ;
	
	// �h�c��Ԃ�
	return Result ;
}

// ���j���[�̂h�c���獀�ږ����擾����
extern int NS_GetMenuItemName( int ItemID, TCHAR *NameBuffer )
{
#ifdef UNICODE
	return GetMenuItemName_WCHAR_T( ItemID, NameBuffer ) ;
#else
	int Result ;
	MENUITEMINFOW ItemInfo ;
	WINMENUITEMINFO *WinItemInfo ;

	if( WinData.MenuUseFlag == FALSE ) return -1 ;
	
	// �e�̂h�c�����I�����ڂ̏����擾		
	WinItemInfo = SearchMenuItemInfo( NULL, ItemID ) ;
	if( WinItemInfo == NULL ) return -1 ;

	// ���݂̏��𓾂�
	Result = _GetMenuItemInfo( WinItemInfo->Menu, WinItemInfo->Index, &ItemInfo ) ;
	if( Result != 0 ) return -1 ;

	// ���ږ����R�s�[
	ConvString( ( const char * )ItemInfo.dwTypeData, WCHAR_T_CHARCODEFORMAT, ( char * )NameBuffer, BUFFERBYTES_CANCEL, _TCHARCODEFORMAT ) ;

	// �I��
	return 0 ;
#endif
}

// ���j���[�̂h�c���獀�ږ����擾����
extern int GetMenuItemName_WCHAR_T( int ItemID, wchar_t *NameBuffer )
{
	int Result ;
	MENUITEMINFOW ItemInfo ;
	WINMENUITEMINFO *WinItemInfo ;

	if( WinData.MenuUseFlag == FALSE ) return -1 ;
	
	// �e�̂h�c�����I�����ڂ̏����擾		
	WinItemInfo = SearchMenuItemInfo( NULL, ItemID ) ;
	if( WinItemInfo == NULL ) return -1 ;

	// ���݂̏��𓾂�
	Result = _GetMenuItemInfo( WinItemInfo->Menu, WinItemInfo->Index, &ItemInfo ) ;
	if( Result != 0 ) return -1 ;

	// ���ږ����R�s�[
	_WCSCPY( NameBuffer, ItemInfo.dwTypeData ) ;

	// �I��
	return 0 ;
}

// ���j���[�����\�[�X����ǂݍ���
extern int NS_LoadMenuResource( int MenuResourceID )
{
	HMENU Menu ;

	// �w��̃��\�[�X��ǂݍ���
	Menu = LoadMenuW( 
		WinData.LoadResourModule == NULL ? WinAPIData.Win32Func.GetModuleHandleWFunc( NULL ) : WinData.LoadResourModule,
		MAKEINTRESOURCEW( MenuResourceID )
	) ;
	if( Menu == NULL ) return -1 ;
	
	// ���j���[��L���ɂ���
	NS_SetUseMenuFlag( TRUE ) ;
	
	// ���ɍ쐬����Ă��郁�j���[���폜����
	if( WinData.Menu != NULL )
	{
		// ���j���[���E�C���h�E����O��
		NS_SetDisplayMenuFlag( FALSE ) ;

		// ���j���[���폜
		DestroyMenu( WinData.Menu ) ;
		WinData.Menu = NULL ;
	}

	// �V�������j���[���Z�b�g
	WinData.Menu = Menu ;
	
	// ���j���[��\����ԂɃZ�b�g
	WinData.MenuDisplayState = FALSE ;
	NS_SetDisplayMenuFlag( TRUE ) ;

	// �I�����ڂ̏����X�V����
	WinData.MenuItemInfoNum = 0 ;
	ListupMenuItemInfo( WinData.Menu ) ;

	// �I��
	return 0 ;
}

// ���j���[�̍��ڂ��I�����ꂽ�Ƃ��ɌĂ΂��R�[���o�b�N�֐���ݒ肷��
extern int NS_SetMenuItemSelectCallBackFunction( void (*CallBackFunction)( const TCHAR *ItemName, int ItemID ) )
{
	WinData.MenuCallBackFunction = CallBackFunction ;

	// �I��
	return 0 ;
}

// ���j���[�\���̒�����A�I�����ڂ̂h�c�����Ɉʒu���𓾂�( -1:�G���[  0:������Ȃ�����  1:�������� )
static int SearchMenuItem( const wchar_t *ItemName, int ItemID, HMENU SearchMenu, HMENU *Menu, int *Index )
{
	int ItemNum, i ;
	int Result ;
	HMENU SubMenu ;
	MENUITEMINFOW ItemInfo ;

	// ���j���[���L���ł͂Ȃ������牽�����Ȃ�
	if( WinData.MenuUseFlag == FALSE ) return -1 ;

	// ���j���[���̍��ڐ��𓾂�	
	ItemNum = GetMenuItemCount( SearchMenu ) ;
	if( ItemNum == -1 ) return -1 ;

	// ���j���[���Ɏw��̂h�c�̑I�����ڂ����������ׂ�
	for( i = 0 ; i < ItemNum ; i ++ )
	{
		// ���ڂ̏��𓾂�
		Result = _GetMenuItemInfo( SearchMenu, i, &ItemInfo ) ;
		
		// ��؂����������X�L�b�v
		if( Result == 1 ) continue ;
		
		// �ړI�̍��ڂ������炱���ŏI��
		if( ItemName )
		{
			if( _WCSCMP( ItemInfo.dwTypeData, ItemName ) == 0 ) break ;
		}
		else
		{
			if( ItemInfo.wID == (UINT)ItemID ) break ;
		}

		// �T�u���j���[�������Ă����炻�������������
		SubMenu = ItemInfo.hSubMenu ;
		if( SubMenu != NULL )
		{
			Result = SearchMenuItem( ItemName, ItemID, SubMenu, Menu, Index ) ;
			
			// �G���[���������邩�A�ړI�̂h�c�����������炱���ŏI��
			if( Result == -1 || Result == 1 ) return Result ;
		}
	}
	
	// �݂���������i�[���Ċ֐����o��
	if( i != ItemNum )
	{
		*Menu = SearchMenu ;
		*Index = i ;
		
		// �����I���͂P
		return 1 ;
	}
	
	// ����������O��Ԃ�
	return 0 ;
}



// (�Â��֐�)�E�C���h�E�Ƀ��j���[��ݒ肷��
extern int NS_SetWindowMenu( int MenuID, int (*MenuProc)( WORD ID ) )
{
	int DispFlag ;

	DispFlag = NS_GetDisplayMenuFlag() ;

	// �������Ƀ��j���[�����[�h����Ă����烁�j���[���폜����
	if( WinData.Menu != NULL )
	{
		// ���j���[������
		NS_SetDisplayMenuFlag( FALSE ) ;

		// ���j���[���폜
		DestroyMenu( WinData.Menu ) ;
		WinData.Menu = NULL ;
	}

	// �������j���[�h�c�� -1 �������烁�j���[���폜
	if( MenuID == -1 )
	{
		// ���j���[�𖳌��ɂ���
		WinData.MenuUseFlag = FALSE ;
	}
	else
	{
		// ���j���[�����[�h
		WinData.Menu = LoadMenuW( WinAPIData.Win32Func.GetModuleHandleWFunc( NULL ), MAKEINTRESOURCEW( MenuID ) ) ;
		if( WinData.Menu == NULL ) return -1 ;

		// ���j���[��L���ɂ���
		WinData.MenuUseFlag = TRUE ;

		// ���j���[��\����Ԃ��X�V����
		NS_SetDisplayMenuFlag( DispFlag ) ;

		// �R�[���o�b�N�֐��̃|�C���^��ۑ�
		WinData.MenuProc = MenuProc ;
	}
	
	// �I�����ڂ̏����X�V����
	WinData.MenuItemInfoNum = 0 ;
	ListupMenuItemInfo( WinData.Menu ) ;

	// �I��
	return 0 ;
}

// �E�C���h�E���j���[�\���J�n�Ɏg�p����L�[��ݒ肷��
/*extern int SetWindowMenuStartKey( int KeyID )
{
	// �L�[��ݒ肷��
	WinData.MenuStartKey = KeyID ;

	// �I��
	return 0 ;
}
*/

// ���j���[��\�����邩�ǂ������Z�b�g����
extern int NS_SetDisplayMenuFlag( int Flag )
{
	int DispState ;

	// ���j���[���L���ł͂Ȃ��ꍇ�͉������Ȃ�
	if( WinData.MenuUseFlag == FALSE ) return 0 ;

	// �t���O��ۑ�
	WinData.NotMenuDisplayFlag = !Flag ;
	
	// �E�C���h�E���쐬����Ă��Ȃ��ꍇ�͉������Ȃ�
	if( WinData.MainWindow == NULL ) return 0 ;

	// �\����Ԃ������ꍇ�͉��������I��
	DispState = GetDisplayMenuState() ;
	if( DispState == WinData.MenuDisplayState ) return 0 ;

	// ���j���[��\�����ׂ����ǂ����ɂ���ď����𕪊�
	if( DispState == TRUE )
	{
		// �\������ꍇ�̏���

		// ���j���[�̃Z�b�g
		if( WinData.MenuSetupFlag == FALSE )
		{
			SetMenu( WinData.MainWindow, WinData.Menu ) ;
			WinData.MenuSetupFlag = TRUE ;
		}

		// �E�C���h�E�̑�����ύX
		if( WinData.WindowModeFlag )
		{
			SetWindowStyle( FALSE ) ;
		}
	}
	else
	{
		// ��\���ɂ���ꍇ�̏���

		// ���j���[���O��
		if( WinData.MenuSetupFlag == TRUE )
		{
			SetMenu( WinData.MainWindow, NULL ) ;
			WinData.MenuSetupFlag = FALSE ;
		}

		// �E�C���h�E�̑�����ύX
		if( WinData.WindowModeFlag )
		{
			SetWindowStyle( FALSE ) ;
		}
	}

	// ���j���[�̍ĕ`��
	UpdateWindow( WinData.MainWindow ) ;
//	DrawMenuBar( WinData.MainWindow ) ;

	// �t���O�̕ۑ�
	WinData.MenuDisplayState = DispState ;

	// �I��
	return 0 ;
}

// ���j���[��\�����Ă��邩�ǂ������擾����
extern int NS_GetDisplayMenuFlag( void )
{
	return WinData.NotMenuDisplayFlag == FALSE ;
}

// ���j���[���\������ׂ����ǂ������擾����( FALSE:�\�����ׂ��łȂ�  TRUE:�\�����ׂ� )
static int GetDisplayMenuState( void )
{
	if( WinData.MainWindow == NULL ) return FALSE ;
	if( WinData.MenuUseFlag == FALSE ) return FALSE ;
	if( WinData.NotMenuDisplayFlag == TRUE ) return FALSE ;
	if( WinData.NotMenuAutoDisplayFlag == TRUE ) return TRUE ;
	if( WinData.WindowModeFlag == FALSE && WinData.MousePosInMenuBarFlag == FALSE ) return FALSE ;
	return TRUE ;
}

// ���j���[�A�C�e���̏����擾����( 0:����I��  -1:�G���[  1:��؂�� )
static int _GetMenuItemInfo( HMENU Menu, int Index, MENUITEMINFOW *Buffer )
{
	static wchar_t NameBuffer[128] ;

	// ��؂�����ǂ������ׂ�
	_MEMSET( Buffer, 0, sizeof( MENUITEMINFOW ) ) ;
	Buffer->cbSize = sizeof( MENUITEMINFOW ) ;
	Buffer->fMask = MIIM_TYPE ;
	if( GetMenuItemInfoW( Menu, ( UINT )Index, TRUE, Buffer ) == 0 ) return -1 ;

	// ��؂����������P��Ԃ�
	if( Buffer->fType & MFT_SEPARATOR ) return 1 ;

	// ��؂���ł͂Ȃ������炻��ȊO�̃X�e�[�^�X���擾����
	_MEMSET( Buffer, 0, sizeof( MENUITEMINFOW ) ) ;
	Buffer->cbSize = sizeof( MENUITEMINFOW ) ;
	Buffer->fMask = MIIM_STATE | MIIM_ID | MIIM_SUBMENU | MIIM_TYPE ;
	Buffer->fType = MFT_STRING ;
	Buffer->dwTypeData = NameBuffer ;
	Buffer->cch = 128 ;
	if( GetMenuItemInfoW( Menu, ( UINT )Index, TRUE, Buffer ) == 0 ) return -1 ;
	
	return 0 ;
}


// �w��̑I�����ڂɃT�u���j���[��t������悤�ɏ���������
static HMENU MenuItemSubMenuSetup( const wchar_t *ItemName, int ItemID )
{
	HMENU SubMenu ;
	MENUITEMINFOW ItemInfo ;
	WINMENUITEMINFO *WinItemInfo ;
	
	// �w��̍��ڂ̏��𓾂�
	WinItemInfo = SearchMenuItemInfo( ItemName, ItemID ) ;
	
	// ������Ȃ�������G���[
	if( WinItemInfo == NULL ) return NULL ;

	// �T�u���j���[�����ɂ��邩�ǂ������ׂ�
	SubMenu = GetSubMenu( WinItemInfo->Menu, WinItemInfo->Index ) ;

	// ���ɍ݂����炻���Ԃ�
	if( SubMenu != NULL ) return SubMenu ;

	// �Ȃ�������T�u���j���[��ǉ�����
	SubMenu = CreateMenu() ;

	// �I�����ڂ̏��ɃT�u���j���[��������
	{
		// ���݂̏����擾
		_GetMenuItemInfo( WinItemInfo->Menu, WinItemInfo->Index, &ItemInfo ) ;
		
		// �T�u���j���[��ǉ�
		ItemInfo.hSubMenu = SubMenu ;

		// �V���������Z�b�g
		SetMenuItemInfoW( WinItemInfo->Menu, ( UINT )WinItemInfo->Index, TRUE, &ItemInfo ) ;
	}

	// �쐬�����T�u���j���[��Ԃ�
	return SubMenu ;
}

// ���j���[���g�p���Ă��邩�ǂ����𓾂�
extern int NS_GetUseMenuFlag( void )
{
	return WinData.MenuUseFlag ;
}

// �t���X�N���[�����Ƀ��j���[�������ŕ\���������\���ɂ�����
// ���邩�ǂ����̃t���O���Z�b�g����
extern int NS_SetAutoMenuDisplayFlag( int Flag )
{
	WinData.NotMenuAutoDisplayFlag = !Flag ;

	return 0 ;
}

// ���j���[�̑I�����ڂ̏��̈ꗗ���쐬���鎞�Ɏg�p����֐�
static int ListupMenuItemInfo( HMENU Menu )
{
	int i, Num, Result ;
	MENUITEMINFOW ItemInfo ;
	WINMENUITEMINFO *WinItemInfo ;
	
	// �S�Ă̍��ڂ̂h�c�����X�g�ɒǉ�����
	Num = GetMenuItemCount( Menu ) ;
	if( Num == -1 ) return -1 ;
	
	for( i = 0 ; i < Num ; i ++ )
	{
		// ���ڂ̏����擾
		Result = _GetMenuItemInfo( Menu, i, &ItemInfo ) ;
		
		// �G���[������������I��
		if( Result == -1 ) return -1 ;
		
		// ��؂���ȊO�����������ǉ�
		if( Result != 1 )
		{
			WinItemInfo = &WinData.MenuItemInfo[ WinData.MenuItemInfoNum ] ;
			WinItemInfo->Menu  = Menu ;
			WinItemInfo->Index = ( short )i ;
			WinItemInfo->ID    = ( unsigned short )ItemInfo.wID ;
			_WCSCPY_S( WinItemInfo->Name, sizeof( WinItemInfo->Name ), ItemInfo.dwTypeData ) ;

			WinData.MenuItemInfoNum ++ ;

			// �T�u���j���[���������炻�������
			if( ItemInfo.hSubMenu != NULL )
				ListupMenuItemInfo( ItemInfo.hSubMenu ) ;
		}
	}

	// �I��
	return 0 ;
}

// ���j���[�̑I�����ڂ̏���ǉ�����
static int AddMenuItemInfo( HMENU Menu, int Index, int ID, const wchar_t *Name )
{
	WINMENUITEMINFO *ItemInfo ;

	// ��񂪈�t��������G���[
	if( WinData.MenuItemInfoNum == MAX_MENUITEM_NUM ) return -1 ;

	// ���̒ǉ�
	ItemInfo = &WinData.MenuItemInfo[WinData.MenuItemInfoNum] ;
	ItemInfo->Menu  = Menu ;
	ItemInfo->Index = ( short )Index ;
	ItemInfo->ID    = ( unsigned short )ID ;
	_WCSCPY_S( ItemInfo->Name, sizeof( ItemInfo->Name ), Name ) ;

	// ���𑝂₷
	WinData.MenuItemInfoNum ++ ;
	
	// �I��
	return 0 ;
}

// ���j���[�̑I�����ڂ̏����폜����
static int DeleteMenuItemInfo( const wchar_t *Name, int ID )
{
	LONG_PTR Index ;
	WINMENUITEMINFO *WinItemInfo ;
	
	// �폜���鍀�ڂ̏��𓾂�
	WinItemInfo = SearchMenuItemInfo( Name, ID ) ;
	if( WinItemInfo == NULL ) return -1 ;
	
	// �C���f�b�N�X���Z�o
	Index = WinItemInfo - WinData.MenuItemInfo ;

	// �����l�߂�
	if( Index != ( LONG_PTR )( WinData.MenuItemInfoNum - 1 ) )
	{
		_MEMMOVE( &WinData.MenuItemInfo[Index],
				 &WinData.MenuItemInfo[Index+1],
				 sizeof( WINMENUITEMINFO ) * ( WinData.MenuItemInfoNum - Index - 1 ) ) ;
	}
	
	// �������炷
	WinData.MenuItemInfoNum -- ;
	
	// �I��
	return 0 ;
}

// ���j���[�̑I�����ڂ̏����擾����
static WINMENUITEMINFO *SearchMenuItemInfo( const wchar_t *Name, int ID )
{
	int i, ItemInfoNum ;
	WINMENUITEMINFO *WinItemInfo ;

	ItemInfoNum = WinData.MenuItemInfoNum ;
	WinItemInfo = WinData.MenuItemInfo ;

	// ���O���L���ȏꍇ�͖��O�Œ��ׂ�
	if( Name != NULL )
	{
		for( i = 0 ; i < ItemInfoNum ; i ++, WinItemInfo ++ )
		{
			if( _WCSCMP( Name, WinItemInfo->Name ) == 0 ) break ;
		}
	}
	else
	{
		for( i = 0 ; i < ItemInfoNum ; i ++, WinItemInfo ++ )
		{
			if( (unsigned short)ID == WinItemInfo->ID ) break ;
		}
	}

	// ������Ȃ������ꍇ�͂m�t�k�k��Ԃ�
	if( i == ItemInfoNum ) return NULL ;

	// ����I��
	return WinItemInfo ;
}

// �V�����I�����ڂ̂h�c���擾����
static int GetNewMenuItemID( void )
{
	int NewItemID ;

	NewItemID = WinData.MenuItemInfoNum ;
	for(;;)
	{
		// �����h�c�����������烋�[�v�𔲂���
		if( SearchMenuItemInfo( NULL, NewItemID ) == NULL ) break ;

		// �݂����玟�̂h�c������
		NewItemID ++ ;
		
		// �h�c�̗L���͈͂𒴂�����O�ɂ���
		if( NewItemID == 0x10000 ) NewItemID = 0 ;
	}

	// �V�����h�c��Ԃ�
	return NewItemID ;
}

// ���j���[�������I�ɕ\���������\���ɂ����肷�鏈�����s��
extern int MenuAutoDisplayProcess( void )
{
//	MENUBARINFO MBInfo ;
	int MenuBarHeight ;
	int MouseX, MouseY ;

	// �E�C���h�E���[�h�̏ꍇ�͉������Ȃ�
	if( WinData.WindowModeFlag == TRUE ) return 0 ;

	// ���j���[�o�[�̍����𓾂�
/*	_MEMSET( &MBInfo, 0, sizeof( MBInfo ) ) ;
	MBInfo.cbSize = sizeof( MBInfo ) ;
	GetMenuBarInfo( WinData.MainWindow, OBJID_MENU, 0, &MBInfo ) ;
	MenuBarHeight = MBInfo.rcBar.bottom - MBInfo.rcBar.top ;
*/
	MenuBarHeight = 60 ;

	// �}�E�X�|�C���^�̍��W�𓾂�
	NS_GetMousePoint( &MouseX, &MouseY ) ;

#ifndef DX_NON_GRAPHICS
	// Direct3D9 ���g�p���Ă���ꍇ�̓��j���[�ʒu��艺�ɗ�����}�E�X�|�C���^�̕\����Ԃ��X�V����
	if( GSYS.Setting.ValidHardware )
	{
		// Direct3D9 ���g�p���Ă���ꍇ�̓��j���[�����݂���ꍇ��
		// �t���X�N���[�����[�h�ł���ɃZ�b�g���Ă���
		if( WinData.Menu && WinData.MenuSetupFlag == FALSE )
		{
			SetMenu( WinData.MainWindow, WinData.Menu ) ;
			WinData.MenuSetupFlag = TRUE ;
		}

		// �x���W���K��ȏ�ɂȂ������\���ɂ���
		if( MouseY > MenuBarHeight )
		{
			NS_SetMouseDispFlag( NS_GetMouseDispFlag() ) ;
		}
	}
	else
#endif // DX_NON_GRAPHICS
	{
		// �}�E�X�|�C���^�����j���[�̈ʒu�ɂ���ꍇ�̓��j���[�o�[��\������
		WinData.MousePosInMenuBarFlag = TRUE ;
		if( MouseY < MenuBarHeight )
			WinData.MousePosInMenuBarFlag = TRUE ;
		else
			WinData.MousePosInMenuBarFlag = FALSE ;

#ifndef DX_NON_GRAPHICS
		// �f�c�h���g�p���邩�ǂ������Z�b�g����
		NS_SetUseGDIFlag( NS_GetUseGDIFlag() ) ;
#endif // DX_NON_GRAPHICS

		// ���j���[�o�[�ƃ}�E�X�̕\����Ԃ��X�V����
		NS_SetMouseDispFlag( NS_GetMouseDispFlag() ) ;
		NS_SetDisplayMenuFlag( NS_GetDisplayMenuFlag() ) ;
	}

	// �I��
	return 0 ;	
}










// �}�E�X�֌W�֐�

// �}�E�X�̕\���t���O�̃Z�b�g
extern int NS_SetMouseDispFlag( int DispFlag )
{
	int DispState ;

	// ���܂łƃt���O�������ŁA�����j���[����\���������ꍇ�Ȃɂ������I��
//	if( DispFlag == WinData.MouseDispFlag &&
//		GetDisplayMenuState() == 0 )
//	{
//		
//		
//		return 0 ;
//	}

	// �}�E�X�̕\���t���O���Z�b�g���ă��b�Z�[�W�𐶐�����
	if( DispFlag != -1 )
	{
		WinData.MouseDispFlag = DispFlag ;
	}

	// �}�E�X��\�����邩�ǂ������擾
	DispState = WinData.MouseDispFlag == TRUE || GetDisplayMenuState() == TRUE ;

	// �}�E�X�̕\����Ԃ����܂łƓ����ꍇ�͉������Ȃ�
	if( DispFlag != -1 && DispState == WinData.MouseDispState ) return 0 ;

	// �}�E�X�̕\����Ԃ��Z�b�g
//	if( 1/*!WinData.WindowModeFlag && !WinData.MouseDispFlag*/ )
	{
		if( DispState == FALSE )
		{
			while( ShowCursor( FALSE ) > -1 ){} ;
		}
		else
		{
			while( ShowCursor( TRUE ) < 0 ){} ;
		}
	}

	// �}�E�X�̃Z�b�g�M�����o��
	PostMessageW( WinData.MainWindow, WM_SETCURSOR, ( WPARAM )WinData.MainWindow, 0 ) ;

	// �}�E�X�̕\����Ԃ�ۑ�����
	WinData.MouseDispState = DispState ;

	// �I��
	return 0 ;
}

// �X�N���[�����W���c�w���C�u������ʍ��W�ɕϊ�����
static int ConvScreenPositionToDxScreenPosition( int ScreenX, int ScreenY, int *DxScreenX, int *DxScreenY )
{
	double ExRateX ;
	double ExRateY ;
#ifndef DX_NON_GRAPHICS
	int SubBackBufferX ;
	int SubBackBufferY ;
#endif // DX_NON_GRAPHICS

	// �E�C���h�E���[�h�̏ꍇ�N���C�A���g�̈���ɍ��W��ϊ�
	if( WinData.WindowModeFlag )
	{
		ScreenX -= WinData.WindowRect.left ;
		ScreenY -= WinData.WindowRect.top + GetToolBarHeight() ;

		// ��ʂ��g�傳��Ă���ꍇ�͂��̉e�����l������
		NS_GetWindowSizeExtendRate( &ExRateX, &ExRateY ) ;
		ScreenX = _DTOL( ScreenX / ExRateX ) ;
		ScreenY = _DTOL( ScreenY / ExRateY ) ;

		if( DxScreenX )
		{
			*DxScreenX = ScreenX ;
		}

		if( DxScreenY )
		{
			*DxScreenY = ScreenY ;
		}
	}
#ifndef DX_NON_GRAPHICS
	else
	{
		if( Graphics_Screen_ScreenPosConvSubBackbufferPos( ScreenX, ScreenY, &SubBackBufferX, &SubBackBufferY ) < 0 )
		{
			if( DxScreenX )
			{
				*DxScreenX = ScreenX ;
			}

			if( DxScreenY )
			{
				*DxScreenY = ScreenY ;
			}
		}
		else
		{
			if( DxScreenX )
			{
				*DxScreenX = SubBackBufferX ;
			}

			if( DxScreenY )
			{
				*DxScreenY = SubBackBufferY ;
			}
		}
	}
#endif // DX_NON_GRAPHICS

	// �I��
	return 0 ;
}

// �}�E�X�̈ʒu���擾����
extern int GetMousePoint_PF( int *XBuf, int *YBuf )
{
	POINT MousePos ;

	// �X�N���[����ł̈ʒu���擾
	GetCursorPos( &MousePos ) ; 

	// �X�N���[�����W���N���C�A���g�̈���̍��W�ɕϊ�
	ConvScreenPositionToDxScreenPosition( MousePos.x, MousePos.y, XBuf, YBuf ) ;

	// �I��
	return 0 ;
}

// �}�E�X�̈ʒu���Z�b�g����
extern int SetMousePoint_PF( int PointX , int PointY )
{
	// �E�C���h�E���[�h���ǂ����ŏ����𕪊�
	if( WinData.WindowModeFlag )
	{
#ifndef DX_NON_GRAPHICS
		int ScreenSizeX , ScreenSizeY ;

		NS_GetDrawScreenSize( &ScreenSizeX , &ScreenSizeY ) ;

		// �E�C���h�E�̃X�P�[�����O�ɉ����č��W��␳
		PointX = _DTOL( PointX * WinData.WindowSizeExRateX ) ;
		PointY = _DTOL( PointY * WinData.WindowSizeExRateY ) ;
		ScreenSizeX = _DTOL( ScreenSizeX * WinData.WindowSizeExRateX ) ;
		ScreenSizeY = _DTOL( ScreenSizeY * WinData.WindowSizeExRateY ) ;

		// ��ʊO�ɃJ�[�\�����o�Ă����ꍇ�̕␳
		if( PointX < 0 ) 			PointX = 0 ;
		else
		if( PointX > ScreenSizeX )	PointX = ScreenSizeX ;

		if( PointY < 0 )			PointY = 0 ;
		else
		if( PointY > ScreenSizeY )	PointY = ScreenSizeY ;
#endif // DX_NON_GRAPHICS

		// �E�C���h�E���[�h�̏ꍇ�̈ʒu�␳
		if( WinData.WindowModeFlag ) 
		{
			PointX += WinData.WindowRect.left ;
			PointY += WinData.WindowRect.top ;
		}
	}
#ifndef DX_NON_GRAPHICS
	else
	{
		int ScreenPosX ;
		int ScreenPosY ;

		if( Graphics_Screen_SubBackbufferPosConvScreenPos( PointX, PointY, &ScreenPosX, &ScreenPosY ) >= 0 )
		{
			PointX = ScreenPosX ;
			PointY = ScreenPosY ;
		}
	}
#endif // DX_NON_GRAPHICS

	// �ʒu���Z�b�g����
	SetCursorPos( PointX , PointY ) ;

	// �I��
	return 0 ;
}

#ifndef DX_NON_INPUT

// �}�E�X�z�C�[���̉�]�ʂ𓾂�
extern int NS_GetMouseWheelRotVol( int CounterReset )
{
	int Vol ;

	// DirectInput ���g�p���Ă��邩�ǂ����ŏ����𕪊�
	if( CheckUseDirectInputMouse() == FALSE )
	{
		Vol = WinData.MouseMoveZ / WHEEL_DELTA ;
		if( CounterReset )
		{
			WinData.MouseMoveZ -= Vol * WHEEL_DELTA ;
		}
	}
	else
	{
		Vol = GetDirectInputMouseMoveZ( CounterReset ) ;
	}

	return Vol ;
}

// �����}�E�X�z�C�[���̉�]�ʂ𓾂�
extern int NS_GetMouseHWheelRotVol( int CounterReset )
{
	int Vol ;

	Vol = WinData.MouseMoveHZ / WHEEL_DELTA ;
	if( CounterReset )
	{
		WinData.MouseMoveHZ -= Vol * WHEEL_DELTA ;
	}

	return Vol ;
}

// �}�E�X�z�C�[���̉�]�ʂ𓾂�( �߂�l�� float �^ )
extern float NS_GetMouseWheelRotVolF( int CounterReset )
{
	float Vol ;

	// DirectInput ���g�p���Ă��邩�ǂ����ŏ����𕪊�
	if( CheckUseDirectInputMouse() == FALSE )
	{
		Vol = ( float )WinData.MouseMoveZ / WHEEL_DELTA ;
		if( CounterReset )
		{
			WinData.MouseMoveZ = 0 ;
		}
	}
	else
	{
		Vol = GetDirectInputMouseMoveZF( CounterReset ) ;
	}

	return Vol ;
}

// �����}�E�X�z�C�[���̉�]�ʂ𓾂�( �߂�l�� float �^ )
extern float NS_GetMouseHWheelRotVolF( int CounterReset )
{
	float Vol ;

	Vol = ( float )WinData.MouseMoveHZ / WHEEL_DELTA ;
	if( CounterReset )
	{
		WinData.MouseMoveHZ = 0 ;
	}

	return Vol ;
}

// �}�E�X�̓��͏���ۑ�����
static	void		StockMouseInputInfo( int Button )
{
	WINMOUSEINPUT *MInput ;

	// ���O�̐����ő吔�ɒB���Ă����牽�������I��
	if( WinData.MouseInputNum >= WIN_MOUSEINPUT_LOG_NUM ) return ;

	// ����ۑ�
	MInput = &WinData.MouseInput[ WinData.MouseInputNum ] ;
	NS_GetMousePoint( &MInput->ClickX, &MInput->ClickY ) ;
	MInput->Button = Button ;

	// ���̐��𑝂₷
	WinData.MouseInputNum ++ ;
}

// �}�E�X����

// �}�E�X�̃N���b�N�����擾����( �߂�l  -1:���O���������� )
extern int NS_GetMouseInputLog( int *Button, int *ClickX, int *ClickY, int LogDelete )
{
	WINMOUSEINPUT *MInput ;

	// ���O������������ -1 ��Ԃ�
	if( WinData.MouseInputNum == 0 ) return -1 ;

	// ����Ԃ�
	MInput = &WinData.MouseInput[ 0 ] ;
	if( Button ) *Button = MInput->Button ;
	if( ClickX ) *ClickX = MInput->ClickX ;
	if( ClickY ) *ClickY = MInput->ClickY ;

	// ���O���폜����w�肪����ꍇ�͍폜����
	if( LogDelete )
	{
		WinData.MouseInputNum -- ;
		if( WinData.MouseInputNum )
			_MEMMOVE( WinData.MouseInput, &WinData.MouseInput[ 1 ], sizeof( WINMOUSEINPUT ) * WinData.MouseInputNum ) ;
	}

	// �I��
	return 0 ;
}

#endif // DX_NON_INPUT





































// �⏕�֐�
extern void _FileTimeToLocalDateData( FILETIME *FileTime, DATEDATA *DateData )
{
	SYSTEMTIME SysTime;
	FILETIME LocalTime;

	FileTimeToLocalFileTime( FileTime, &LocalTime );
	FileTimeToSystemTime( &LocalTime, &SysTime );
	DateData->Year = SysTime.wYear ;
	DateData->Mon  = SysTime.wMonth ;
	DateData->Day  = SysTime.wDay ;
	DateData->Hour = SysTime.wHour ;
	DateData->Min  = SysTime.wMinute ;
	DateData->Sec  = SysTime.wSecond ;
}

// �w��̊�{�C���[�W���g�p���� UpdateLayeredWindow ���s��
extern int UpdateBackBufferTransColorWindow( const BASEIMAGE *SrcImage, const RECT *SrcImageRect, HDC Direct3DDC, int NotColorKey, int PreMultipliedAlphaImage )
{
	HDC dc, ddc, memdc ;
	SIZE wsize ;
	POINT wpos, pos ;
	BYTE *Dst ;
	BYTE *Src ;
//	RECT wrect, wcrect ;
//	RECT wrect, wcrect ;
	BLENDFUNCTION blend ;
	HGDIOBJ old ;
	COLORREF trans ;
	int w, h ;
	int Systembar ;

	// �V�X�e���o�[�����E�C���h�E�X�^�C�����ǂ������Z�o
	Systembar = ( WStyle_WindowModeTable[ WinData.WindowStyle ] & WS_CAPTION ) != 0;

	// ���ʏ���
	ddc = GetDC( NULL ) ;
	dc = GetDC( GetDisplayWindowHandle() ) ;

	pos.x = 0 ;
	pos.y = 0 ;
//	GetWindowRect( GetDisplayWindowHandle(), &wrect ) ;
//	GetClientRect( GetDisplayWindowHandle(), &wcrect ) ;
//	wpos.x = wrect.left + wcrect.left ;
//	wpos.y = wrect.top  + wcrect.top  ;
//	wsize.cx = wcrect.right - wcrect.left ;
//	wsize.cy = wcrect.bottom - wcrect.top ;
	wpos.x = WinData.WindowEdgeRect.left ;
	wpos.y = WinData.WindowEdgeRect.top ;
#ifndef DX_NON_GRAPHICS
	NS_GetDrawScreenSize( &w, &h ) ;
#else // DX_NON_GRAPHICS
	w = WinData.WindowRect.right  - WinData.WindowRect.left ;
	h = WinData.WindowRect.bottom - WinData.WindowRect.top ;
#endif // DX_NON_GRAPHICS
	wsize.cx = w ;
	wsize.cy = h ;

	DWORD BaseTransColor = 0 ;

#ifndef DX_NON_GRAPHICS
	BaseTransColor = GSYS.CreateImage.TransColor ;
#endif // DX_NON_GRAPHICS

	trans = ( ( BaseTransColor & 0xff0000 ) >> 16 ) | 
			( ( BaseTransColor & 0x0000ff ) << 16 ) |
			  ( BaseTransColor & 0x00ff00 ) ;

#ifndef DX_NON_GRAPHICS
	// Direct3DDC ���L�����ǂ����ŏ����𕪊�
	if( Direct3DDC )
	{
		WinAPIData.Win32Func.UpdateLayeredWindow( GetDisplayWindowHandle(), ddc, &wpos, &wsize, Direct3DDC, &pos, trans, &blend, ULW_COLORKEY ) ;
	}
	else
#endif // DX_NON_GRAPHICS
	{
		RECT SrcRect ;
		BASEIMAGE SrcImageTemp ;

		// SrcImage �� NULL �ȊO�ŁA���� SrcImageRect �� NULL �ȊO�̏ꍇ�͉��� BASEIMAGE ��p�ӂ���
		if( SrcImage != NULL && SrcImageRect != NULL )
		{
			if( NS_DerivationBaseImage( SrcImage, SrcImageRect->left, SrcImageRect->top, SrcImageRect->right, SrcImageRect->bottom, &SrcImageTemp ) < 0 )
			{
				return -1 ;
			}

			SrcImage = &SrcImageTemp ;
		}

		// �r�b�g�}�b�v�̃T�C�Y���������r�b�g�}�b�v�̍�蒼��
		if( WinData.BackBufferTransBitmapSize.cx != SrcImage->Width ||
			WinData.BackBufferTransBitmapSize.cy != SrcImage->Height )
		{
			BITMAPINFO BHead ;
			HDC dc ;

			if( WinData.BackBufferTransBitmap )
			{
				DeleteObject( WinData.BackBufferTransBitmap ) ;
				WinData.BackBufferTransBitmap = NULL ;
			}

			_MEMSET( &BHead, 0, sizeof( BHead ) ) ;
			BHead.bmiHeader.biSize = sizeof( BHead ) ;
			BHead.bmiHeader.biWidth = SrcImage->Width ;
			BHead.bmiHeader.biHeight = -SrcImage->Height;
			if( Systembar )
			{
				BHead.bmiHeader.biHeight -= WinData.SystembarHeight;
			}
			BHead.bmiHeader.biBitCount = 32 ;
			BHead.bmiHeader.biPlanes = 1 ;
			dc = GetDC( GetDisplayWindowHandle() ) ;
			WinData.BackBufferTransBitmap = CreateDIBSection( dc, &BHead, DIB_RGB_COLORS, &WinData.BackBufferTransBitmapImage, NULL, 0 ) ;
			ReleaseDC( GetDisplayWindowHandle(), dc ) ;
			NS_CreateARGB8ColorData( &WinData.BackBufferTransBitmapColorData ) ;
			_MEMSET( WinData.BackBufferTransBitmapImage, 0, ( size_t )( -BHead.bmiHeader.biHeight * SrcImage->Width * 4 ) ) ;

			WinData.BackBufferTransBitmapSize.cx = SrcImage->Width ;
			WinData.BackBufferTransBitmapSize.cy = SrcImage->Height ;
		}

		// �C���[�W�̃A�h���X���Z�b�g
		Dst = ( BYTE * )WinData.BackBufferTransBitmapImage ;
		if( Systembar )
		{
			Dst += WinData.SystembarHeight * SrcImage->Width * 4 ;
		}
		Src = ( BYTE * )SrcImage->GraphData ;

		// �r�b�g�}�b�v�Ƀf�[�^��]��
		SrcRect.left = 0 ;
		SrcRect.top = 0 ;
		SrcRect.right = SrcImage->Width ;
		SrcRect.bottom = SrcImage->Height ;
		if( NotColorKey == FALSE )
		{
			// VISTA�ȍ~�̏ꍇ�Ƃ���ȊO�ŏ����𕪊�
			if( WinData.WindowsVersion >= DX_WINDOWSVERSION_VISTA )
			{
				DWORD i, j, Size, PackNum, NokoriNum ;
				DWORD TransColor, SrcAddPitch ;

				PackNum   = ( DWORD )( SrcImage->Width / 4 ) ;
				NokoriNum = SrcImage->Width - PackNum * 4 ;

				if( SrcImage->ColorData.AlphaMask == 0x00000000 &&
					SrcImage->ColorData.RedMask   == 0x00ff0000 &&
					SrcImage->ColorData.GreenMask == 0x0000ff00 &&
					SrcImage->ColorData.BlueMask  == 0x000000ff )
				{
					SrcAddPitch = ( DWORD )( SrcImage->Pitch - SrcImage->Width * 4 ) ;
					TransColor  = BaseTransColor & 0x00ffffff ;
					for( i = ( DWORD )SrcImage->Height ; i ; i --, Src += SrcAddPitch )
					{
						for( j = ( DWORD )SrcImage->Width ; j ; j --, Src += 4, Dst += 4 )
						{
							if( ( *( ( DWORD * )Src ) & 0x00ffffff ) == TransColor )
							{
								*( ( DWORD * )Dst ) = 0 ;
							}
							else
							{
								*( ( DWORD * )Dst ) = *( ( DWORD * )Src ) | 0xff000000 ;
							}
						}
					}
				}
				else
				if( SrcImage->ColorData.AlphaMask == 0x00000000 &&
					SrcImage->ColorData.RedMask   == 0x0000f800 &&
					SrcImage->ColorData.GreenMask == 0x000007e0 &&
					SrcImage->ColorData.BlueMask  == 0x0000001f )
				{
					SrcAddPitch = ( DWORD )( SrcImage->Pitch - SrcImage->Width * 2 ) ;
					TransColor  =	( ( ( BaseTransColor & 0x00ff0000 ) >> ( 16 + 3 ) ) << 11 ) |
									( ( ( BaseTransColor & 0x0000ff00 ) >> (  8 + 2 ) ) <<  5 ) |
									( ( ( BaseTransColor & 0x000000ff ) >> (  0 + 3 ) ) <<  0 ) ;
					for( i = ( DWORD )SrcImage->Height ; i ; i --, Src += SrcAddPitch )
					{
						for( j = PackNum ; j ; j --, Src += 2 * 4, Dst += 4 * 4 )
						{
							if( ( ( WORD * )Src )[ 0 ] == TransColor )
							{
								( ( DWORD * )Dst )[ 0 ] = 0 ;
							}
							else
							{
								( ( DWORD * )Dst )[ 0 ] =
									( ( ( ( WORD * )Src )[ 0 ] & 0xf800 ) << ( 16 + 3 - 11 ) ) |
									( ( ( ( WORD * )Src )[ 0 ] & 0x07e0 ) << (  8 + 2 -  5 ) ) |
									( ( ( ( WORD * )Src )[ 0 ] & 0x001f ) << (  0 + 3 -  0 ) ) | 0xff000000 ;
							}

							if( ( ( WORD * )Src )[ 1 ] == TransColor )
							{
								( ( DWORD * )Dst )[ 1 ] = 0 ;
							}
							else
							{
								( ( DWORD * )Dst )[ 1 ] =
									( ( ( ( WORD * )Src )[ 1 ] & 0xf800 ) << ( 16 + 3 - 11 ) ) |
									( ( ( ( WORD * )Src )[ 1 ] & 0x07e0 ) << (  8 + 2 -  5 ) ) |
									( ( ( ( WORD * )Src )[ 1 ] & 0x001f ) << (  0 + 3 -  0 ) ) | 0xff000000 ;
							}

							if( ( ( WORD * )Src )[ 2 ] == TransColor )
							{
								( ( DWORD * )Dst )[ 2 ] = 0 ;
							}
							else
							{
								( ( DWORD * )Dst )[ 2 ] =
									( ( ( ( WORD * )Src )[ 2 ] & 0xf800 ) << ( 16 + 3 - 11 ) ) |
									( ( ( ( WORD * )Src )[ 2 ] & 0x07e0 ) << (  8 + 2 -  5 ) ) |
									( ( ( ( WORD * )Src )[ 2 ] & 0x001f ) << (  0 + 3 -  0 ) ) | 0xff000000 ;
							}

							if( ( ( WORD * )Src )[ 3 ] == TransColor )
							{
								( ( DWORD * )Dst )[ 3 ] = 0 ;
							}
							else
							{
								( ( DWORD * )Dst )[ 3 ] =
									( ( ( ( WORD * )Src )[ 3 ] & 0xf800 ) << ( 16 + 3 - 11 ) ) |
									( ( ( ( WORD * )Src )[ 3 ] & 0x07e0 ) << (  8 + 2 -  5 ) ) |
									( ( ( ( WORD * )Src )[ 3 ] & 0x001f ) << (  0 + 3 -  0 ) ) | 0xff000000 ;
							}
						}

						for( j = NokoriNum ; j ; j --, Src += 2, Dst += 4 )
						{
							if( *( ( WORD * )Src ) == TransColor )
							{
								*( ( DWORD * )Dst ) = 0 ;
							}
							else
							{
								*( ( DWORD * )Dst ) =
									( ( *( ( WORD * )Src ) & 0xf800 ) << ( 16 + 3 - 11 ) ) |
									( ( *( ( WORD * )Src ) & 0x07e0 ) << (  8 + 2 -  5 ) ) |
									( ( *( ( WORD * )Src ) & 0x001f ) << (  0 + 3 -  0 ) ) | 0xff000000 ;
							}
						}
					}
				}
				else
				{
					NS_GraphColorMatchBltVer2(
						Dst, SrcImage->Width * 4, &WinData.BackBufferTransBitmapColorData,
						Src, SrcImage->Pitch, &SrcImage->ColorData,
						NULL, 0, NULL,
						pos, &SrcRect, FALSE, FALSE, 0, 0, 0, 0, 0, 0 ) ;

					Size       = ( DWORD )( WinData.BackBufferTransBitmapSize.cx * WinData.BackBufferTransBitmapSize.cy ) ;
					TransColor = BaseTransColor & 0x00ffffff ;
					for( i = Size ; i ; i --, Dst += 4 )
					{
						if( ( *( ( DWORD * )Dst ) & 0x00ffffff ) == TransColor )
						{
							*( ( DWORD * )Dst ) = 0 ;
						}
						else
						{
							Dst[ 3 ] = 255 ;
						}
					}
				}
			}
			else
			{
				COLORDATA XRGB8ColorData ;

				if( Systembar )
				{
					_MEMSETD( WinData.BackBufferTransBitmapImage, BaseTransColor & 0x00ffffff, WinData.SystembarHeight * SrcImage->Width ) ;
				}
				NS_CreateXRGB8ColorData( &XRGB8ColorData ) ;
				NS_GraphColorMatchBltVer2(
					Dst, SrcImage->Width * 4, &XRGB8ColorData,
					Src, SrcImage->Pitch, &SrcImage->ColorData,
					NULL, 0, NULL,
					pos, &SrcRect, FALSE, FALSE, 0, 0, 0, 0, 0, 0 ) ;
			}
		}
		else
		{
			DWORD i, j, Size ;
			DWORD TransColor, SrcAddPitch ;
			int UseTransColor ;

			UseTransColor = WinData.UseUpdateLayerdWindowFlag == FALSE ? TRUE : FALSE ;

			if( SrcImage->ColorData.AlphaMask == 0xff000000 &&
				SrcImage->ColorData.RedMask   == 0x00ff0000 &&
				SrcImage->ColorData.GreenMask == 0x0000ff00 &&
				SrcImage->ColorData.BlueMask  == 0x000000ff )
			{
				SrcAddPitch = ( DWORD )( SrcImage->Pitch - SrcImage->Width * 4 ) ;
				TransColor  = BaseTransColor & 0x00ffffff ;
				if( PreMultipliedAlphaImage )
				{
					if( UseTransColor )
					{
						for( i = ( DWORD )SrcImage->Height ; i ; i --, Src += SrcAddPitch )
						{
							for( j = ( DWORD )SrcImage->Width ; j ; j --, Src += 4, Dst += 4 )
							{
								if( ( *( ( DWORD * )Src ) & 0x00ffffff ) == TransColor )
								{
									*( ( DWORD * )Dst ) = 0 ;
								}
								else
								{
									*( ( DWORD * )Dst ) = *( ( DWORD * )Src ) ;
								}
							}
						}
					}
					else
					{
						for( i = ( DWORD )SrcImage->Height ; i ; i --, Src += SrcAddPitch )
						{
							for( j = ( DWORD )SrcImage->Width ; j ; j --, Src += 4, Dst += 4 )
							{
								*( ( DWORD * )Dst ) = *( ( DWORD * )Src ) ;
							}
						}
					}
				}
				else
				{
					if( UseTransColor )
					{
						for( i = ( DWORD )SrcImage->Height ; i ; i --, Src += SrcAddPitch )
						{
							for( j = ( DWORD )SrcImage->Width ; j ; j --, Src += 4, Dst += 4 )
							{
								if( ( *( ( DWORD * )Src ) & 0x00ffffff ) == TransColor || Src[ 3 ] == 0 )
								{
									*( ( DWORD * )Dst ) = 0 ;
								}
								else
								if( Src[ 3 ] == 255 )
								{
									*( ( DWORD * )Dst ) = *( ( DWORD * )Src ) ;
								}
								else
								{
									Dst[ 0 ] = ( BYTE )( ( Src[ 0 ] * Src[ 3 ] ) >> 8 ) ;
									Dst[ 1 ] = ( BYTE )( ( Src[ 1 ] * Src[ 3 ] ) >> 8 ) ;
									Dst[ 2 ] = ( BYTE )( ( Src[ 2 ] * Src[ 3 ] ) >> 8 ) ;
									Dst[ 3 ] = Src[ 3 ] ;
								}
							}
						}
					}
					else
					{
						for( i = ( DWORD )SrcImage->Height ; i ; i --, Src += SrcAddPitch )
						{
							for( j = ( DWORD )SrcImage->Width ; j ; j --, Src += 4, Dst += 4 )
							{
								if( Src[ 3 ] == 0 )
								{
									*( ( DWORD * )Dst ) = 0 ;
								}
								else
								if( Src[ 3 ] == 255 )
								{
									*( ( DWORD * )Dst ) = *( ( DWORD * )Src ) ;
								}
								else
								{
									Dst[ 0 ] = ( BYTE )( ( Src[ 0 ] * Src[ 3 ] ) >> 8 ) ;
									Dst[ 1 ] = ( BYTE )( ( Src[ 1 ] * Src[ 3 ] ) >> 8 ) ;
									Dst[ 2 ] = ( BYTE )( ( Src[ 2 ] * Src[ 3 ] ) >> 8 ) ;
									Dst[ 3 ] = Src[ 3 ] ;
								}
							}
						}
					}
				}
			}
			else
			{
				NS_GraphColorMatchBltVer2(
					Dst, SrcImage->Width * 4, &WinData.BackBufferTransBitmapColorData,
					Src, SrcImage->Pitch, &SrcImage->ColorData,
					NULL, 0, NULL,
					pos, &SrcRect, FALSE, UseTransColor, BaseTransColor, 0, 0, 0, 0, 0 ) ;

				Size = ( DWORD )( WinData.BackBufferTransBitmapSize.cx * WinData.BackBufferTransBitmapSize.cy ) ;
				if( PreMultipliedAlphaImage == FALSE )
				{
					for( i = Size ; i ; i --, Dst += 4 )
					{
						if( Dst[ 3 ] == 0 )
						{
							*( ( DWORD * )Dst ) = 0 ;
						}
						else
						if( Dst[ 3 ] != 255 ) 
						{
							Dst[ 0 ] = ( BYTE )( ( Dst[ 0 ] * Dst[ 3 ] ) >> 8 ) ;
							Dst[ 1 ] = ( BYTE )( ( Dst[ 1 ] * Dst[ 3 ] ) >> 8 ) ;
							Dst[ 2 ] = ( BYTE )( ( Dst[ 2 ] * Dst[ 3 ] ) >> 8 ) ;
						}
					}
				}
			}
		}

		// �쐬�����r�b�g�}�b�v������ UpdateLeyeredWindow ���s��
		memdc = CreateCompatibleDC( dc ) ;
		old = SelectObject( memdc, WinData.BackBufferTransBitmap ) ;

		// Vista �ȍ~�̏ꍇ�͕K���A���t�@�u�����h����
		if( Systembar )
		{
			wsize.cy += WinData.SystembarHeight ;
		}
		if( NotColorKey == FALSE && WinData.WindowsVersion < DX_WINDOWSVERSION_VISTA )
		{
			WinAPIData.Win32Func.UpdateLayeredWindow( GetDisplayWindowHandle(), ddc, &wpos, &wsize, memdc, &pos, trans, NULL, ULW_COLORKEY ) ;
		}
		else
		{
			blend.BlendOp = AC_SRC_OVER ;
			blend.BlendFlags = 0 ;
			blend.SourceConstantAlpha = 255 ;
			blend.AlphaFormat = AC_SRC_ALPHA ;
			WinAPIData.Win32Func.UpdateLayeredWindow( GetDisplayWindowHandle(), ddc, &wpos, &wsize, memdc, &pos, 0, &blend, ULW_ALPHA ) ;
		}

		SelectObject( memdc, old ) ;
		DeleteDC( memdc ) ;
	}

	ReleaseDC( GetDisplayWindowHandle(), dc ) ;
	ReleaseDC( NULL, ddc ) ;

	return 0 ;
}


























// ���o�͌n

// �b�o�t�h�c���ߒ�`
#define CPUID	__asm _emit 0fH  __asm _emit 0A2H


// �n�r��c�����������w�̃o�[�W�������o�͂���
extern int OutSystemInfo( void )
{
	wchar_t Str[ 256 ] ;

	DXST_ERRORLOG_ADDUTF16LE( "\xb7\x30\xb9\x30\xc6\x30\xe0\x30\x6e\x30\xc5\x60\x31\x58\x92\x30\xfa\x51\x9b\x52\x57\x30\x7e\x30\x59\x30\x0a\x00\x00"/*@ L"�V�X�e���̏����o�͂��܂�\n" @*/ ) ;
	DXST_ERRORLOG_TABADD ;

#ifndef DX_NON_LITERAL_STRING
	// �c�w���C�u�����̃o�[�W�������o�͂���
	{
		char UTF16LE_Buffer[ 128 ] ;
		char DestBuffer[ 128 ] ;
		ConvString( ( const char * )DXLIB_VERSION_STR_W, WCHAR_T_CHARCODEFORMAT, UTF16LE_Buffer, sizeof( UTF16LE_Buffer ), DX_CHARCODEFORMAT_UTF16LE ) ;
		CL_snprintf( DX_CHARCODEFORMAT_UTF16LE, TRUE, DX_CHARCODEFORMAT_SHIFTJIS, DX_CHARCODEFORMAT_UTF16LE, DestBuffer, sizeof( Str ) / 2, "\x24\xff\x38\xff\xe9\x30\xa4\x30\xd6\x30\xe9\x30\xea\x30\x20\x00\x56\x00\x65\x00\x72\x00\x25\x00\x73\x00\x0a\x00\x00"/*@ L"�c�w���C�u���� Ver%s\n" @*/, UTF16LE_Buffer ) ;

		DXST_ERRORLOG_ADDUTF16LE( DestBuffer ) ;
	}
#endif

	// �b�o�t�̃R�A�����擾����
	{
		SYSTEM_INFO SystemInfo ;

		GetSystemInfo( &SystemInfo ) ;
		WinData.ProcessorNum = ( int )SystemInfo.dwNumberOfProcessors ;

		DXST_ERRORLOGFMT_ADDUTF16LE(( "\xd6\x8a\x06\x74\xd7\x30\xed\x30\xbb\x30\xc3\x30\xb5\x30\x6e\x30\x70\x65\x20\x00\x3a\x00\x20\x00\x25\x00\x64\x00\x00"/*@ L"�_���v���Z�b�T�̐� : %d" @*/, WinData.ProcessorNum )) ;
	}

	// �n�r�̃o�[�W�������o�͂���
	{
		OSVERSIONINFOW OsVersionInfo ;

		DXST_ERRORLOG_ADDUTF16LE( "\x2f\xff\x33\xff\x20\x00\x20\x00\x00"/*@ L"�n�r  " @*/ ) ;
		_WCSCPY( Str, L"Windows" ) ;

		_MEMSET( &OsVersionInfo, 0, sizeof( OsVersionInfo ) ) ;
		OsVersionInfo.dwOSVersionInfoSize = sizeof( OsVersionInfo ) ;

		GetVersionExW( &OsVersionInfo ) ;

		// �v���b�g�t�H�[���ɂ���ď����𕪂���
		switch( OsVersionInfo.dwPlatformId )
		{
		// Windows3.1�n(�܂������ɂ��邱�Ƃ͂Ȃ��Ǝv�����c)
		case VER_PLATFORM_WIN32s :
			_WCSCAT_S( Str, sizeof( Str ), L"3.1" ) ;
			WinData.WindowsVersion = DX_WINDOWSVERSION_31 ;
			break ;

		// Windows95, 98, Me
		case VER_PLATFORM_WIN32_WINDOWS :

			// �}�C�i�[�o�[�W�����ɂ���ĕ���
			switch( OsVersionInfo.dwMinorVersion )
			{
			case 0 :	// Windows95 
				_WCSCAT_S( Str, sizeof( Str ), L"95" ) ;
				WinData.WindowsVersion = DX_WINDOWSVERSION_95 ;
				if( OsVersionInfo.szCSDVersion[0] == L'C' )
				{
					_WCSCAT_S( Str, sizeof( Str ), L"OSR2" ) ;
				}
				break ;

			case 10 :	// Windows98
				_WCSCAT_S( Str, sizeof( Str ), L"98" ) ;
				WinData.WindowsVersion = DX_WINDOWSVERSION_98 ;
				if( OsVersionInfo.szCSDVersion[0] == L'A' )
				{
					_WCSCAT_S( Str, sizeof( Str ), L"SE" ) ;
				}
				break ;

			case 90 :
				_WCSCAT_S( Str, sizeof( Str ), L"Me" ) ;
				WinData.WindowsVersion = DX_WINDOWSVERSION_ME ;
				break ;
			}
			break ;

		// NT �n
		case VER_PLATFORM_WIN32_NT :

			// �o�[�W������ Windows8 �ȍ~�̏ꍇ�͕���
			if( OsVersionInfo.dwMajorVersion >= 6 && OsVersionInfo.dwMinorVersion >= 2 &&
				( ( WinAPIData.Win32Func.VerifyVersionInfoWFunc != NULL && WinAPIData.Win32Func.VerSetConditionMaskFunc != NULL ) ||
				    WinAPIData.Win32Func.RtlGetVersionFunc      != NULL ) )
			{
				OSVERSIONINFOEXW OsVersionInfoW ;
				ULONGLONG condition = 0 ;

				_MEMSET( &OsVersionInfoW, 0, sizeof( OsVersionInfoW ) ) ;
				OsVersionInfoW.dwOSVersionInfoSize = sizeof( OsVersionInfoW ) ;

				// RtlGetVersion ���g�p�ł���ꍇ�� RtlGetVersion ���g�p����
				if( WinAPIData.Win32Func.RtlGetVersionFunc != NULL )
				{
					WinAPIData.Win32Func.RtlGetVersionFunc( &OsVersionInfoW ) ;

					_WCSCPY( OsVersionInfo.szCSDVersion, OsVersionInfoW.szCSDVersion ) ;
					OsVersionInfo.dwBuildNumber = OsVersionInfoW.dwBuildNumber ;

					// ���W���[�o�[�W�����ɂ���ď����𕪊�
					switch( OsVersionInfoW.dwMajorVersion )
					{
					case 6 :
						switch( OsVersionInfoW.dwMinorVersion )
						{
						case 0 :
							_WCSCAT_S( Str, sizeof( Str ), L"Vista" ) ;
							WinData.WindowsVersion = DX_WINDOWSVERSION_VISTA ;
							break ;

						case 1 :
							_WCSCAT_S( Str, sizeof( Str ), L"7" ) ;
							WinData.WindowsVersion = DX_WINDOWSVERSION_7 ;
							break ;

						case 2 :
							_WCSCAT_S( Str, sizeof( Str ), L"8" ) ;
							WinData.WindowsVersion = DX_WINDOWSVERSION_8 ;
							break ;

						case 3 :
							_WCSCAT_S( Str, sizeof( Str ), L"8.1" ) ;
							WinData.WindowsVersion = DX_WINDOWSVERSION_8_1 ;
							break ;

						case 4 :
							_WCSCAT_S( Str, sizeof( Str ), L"10" ) ;
							WinData.WindowsVersion = DX_WINDOWSVERSION_10 ;
							break ;
						}
						break ;

					case 10 :
						switch( OsVersionInfoW.dwMinorVersion )
						{
						case 0 :
							_WCSCAT_S( Str, sizeof( Str ), L"10" ) ;
							WinData.WindowsVersion = DX_WINDOWSVERSION_10 ;
							break ;
						}
						break ;
					}
				}
				else
				{
					// �g�p�ł��Ȃ��ꍇ�� VerifyVersionInfo ���g�p���ăo�[�W�������`�F�b�N����

					OsVersionInfo.dwBuildNumber = 0 ;
					OsVersionInfo.szCSDVersion[ 0 ] = '\0' ;

					for(;;)
					{
						// Windows10�ȍ~�̃`�F�b�N
						condition = 0 ;
						condition = WinAPIData.Win32Func.VerSetConditionMaskFunc( condition, VER_MAJORVERSION, VER_GREATER ) ;
						condition = WinAPIData.Win32Func.VerSetConditionMaskFunc( condition, VER_MINORVERSION, VER_GREATER ) ;
						OsVersionInfoW.dwMajorVersion = 10 ;
						OsVersionInfoW.dwMinorVersion = 0 ;
						if( WinAPIData.Win32Func.VerifyVersionInfoWFunc( &OsVersionInfoW, VER_MAJORVERSION | VER_MINORVERSION, condition ) )
						{
							_WCSCAT_S( Str, sizeof( Str ), L"10 After" ) ;
							WinData.WindowsVersion = DX_WINDOWSVERSION_10 ;
							break ;
						}

						// Windows10�̃`�F�b�N
						condition = 0 ;
						condition = WinAPIData.Win32Func.VerSetConditionMaskFunc( condition, VER_MAJORVERSION, VER_GREATER_EQUAL ) ;
						condition = WinAPIData.Win32Func.VerSetConditionMaskFunc( condition, VER_MINORVERSION, VER_GREATER_EQUAL ) ;
						OsVersionInfoW.dwMajorVersion = 10 ;
						OsVersionInfoW.dwMinorVersion = 0 ;
						if( WinAPIData.Win32Func.VerifyVersionInfoWFunc( &OsVersionInfoW, VER_MAJORVERSION | VER_MINORVERSION, condition ) )
						{
							_WCSCAT_S( Str, sizeof( Str ), L"10" ) ;
							WinData.WindowsVersion = DX_WINDOWSVERSION_10 ;
							break ;
						}
						OsVersionInfoW.dwMajorVersion = 6 ;
						OsVersionInfoW.dwMinorVersion = 4 ;
						if( WinAPIData.Win32Func.VerifyVersionInfoWFunc( &OsVersionInfoW, VER_MAJORVERSION | VER_MINORVERSION, condition ) )
						{
							_WCSCAT_S( Str, sizeof( Str ), L"10" ) ;
							WinData.WindowsVersion = DX_WINDOWSVERSION_10 ;
							break ;
						}

						// Windows8.1�̃`�F�b�N
						OsVersionInfoW.dwMajorVersion = 6 ;
						OsVersionInfoW.dwMinorVersion = 3 ;
						if( WinAPIData.Win32Func.VerifyVersionInfoWFunc( &OsVersionInfoW, VER_MAJORVERSION | VER_MINORVERSION, condition ) )
						{
							_WCSCAT_S( Str, sizeof( Str ), L"8.1" ) ;
							WinData.WindowsVersion = DX_WINDOWSVERSION_8_1 ;
							break ;
						}

						// �����ɂ����� Windows8
						_WCSCAT_S( Str, sizeof( Str ), L"8" ) ;
						WinData.WindowsVersion = DX_WINDOWSVERSION_8 ;
						break ;
					}
				}
			}
			else
			{
				// ���W���[�o�[�W�����ɂ���ď����𕪊�
				switch( OsVersionInfo.dwMajorVersion )
				{
				case 3 :
					_WCSCAT_S( Str, sizeof( Str ), L"NT 3.51" ) ;
					WinData.WindowsVersion = DX_WINDOWSVERSION_NT31 ;
					break ;

				case 4 :
					_WCSCAT_S( Str, sizeof( Str ), L"4.0" ) ;
					WinData.WindowsVersion = DX_WINDOWSVERSION_NT40 ;
					break ;

				case 5 :	
					switch( OsVersionInfo.dwMinorVersion )
					{
					case 0 :
						_WCSCAT_S( Str, sizeof( Str ), L"2000" ) ;
						WinData.WindowsVersion = DX_WINDOWSVERSION_2000 ;
						break ;

					case 1 :
						_WCSCAT_S( Str, sizeof( Str ), L"XP" ) ;
						WinData.WindowsVersion = DX_WINDOWSVERSION_XP ;
						break ;
					}
					break ;

				case 6 :
					switch( OsVersionInfo.dwMinorVersion )
					{
					case 0 :
						_WCSCAT_S( Str, sizeof( Str ), L"Vista" ) ;
						WinData.WindowsVersion = DX_WINDOWSVERSION_VISTA ;
						break ;

					case 1 :
						_WCSCAT_S( Str, sizeof( Str ), L"7" ) ;
						WinData.WindowsVersion = DX_WINDOWSVERSION_7 ;
						break ;

					case 2 :
						_WCSCAT_S( Str, sizeof( Str ), L"8" ) ;
						WinData.WindowsVersion = DX_WINDOWSVERSION_8 ;
						break ;

					case 3 :
						_WCSCAT_S( Str, sizeof( Str ), L"8.1" ) ;
						WinData.WindowsVersion = DX_WINDOWSVERSION_8_1 ;
						break ;

					case 4 :
						_WCSCAT_S( Str, sizeof( Str ), L"10" ) ;
						WinData.WindowsVersion = DX_WINDOWSVERSION_10 ;
						break ;
					}
					break ;

				case 10 :
					switch( OsVersionInfo.dwMinorVersion )
					{
					case 0 :
						_WCSCAT_S( Str, sizeof( Str ), L"10" ) ;
						WinData.WindowsVersion = DX_WINDOWSVERSION_10 ;
						break ;
					}
					break ;
				}
			}
		}
		if( OsVersionInfo.dwBuildNumber != 0 )
		{
			_SWNPRINTF( WinData.PcInfo.OSString, sizeof( WinData.PcInfo.OSString ) / 2, L"%s ( Build %d %s )", Str, OsVersionInfo.dwBuildNumber, OsVersionInfo.szCSDVersion ) ;
		}
		else
		{
			_SWNPRINTF( WinData.PcInfo.OSString, sizeof( WinData.PcInfo.OSString ) / 2, L"%s", Str ) ;
		}
		DXST_ERRORLOGFMT_ADDW(( L"%s", WinData.PcInfo.OSString )) ;
	}

	// �c�����������w�̃o�[�W�������o��

	// ���� DirectX 7.0 ���C���X�g�[������Ă��Ȃ����͂܂������̂ŁA�N�����Ԃ�Z������ׂɃ`�F�b�N�͂��Ȃ�
	WinData.DirectXVersion = DX_DIRECTXVERSION_7 ;

	// �^�C�}�̐��x��ݒ肷��
	{
		TIMECAPS tc ;
		WinAPIData.Win32Func.timeGetDevCapsFunc( &tc , sizeof(TIMECAPS) );

		// �}���`���f�B�A�^�C�}�[�̃T�[�r�X���x���ő�� 
		WinAPIData.Win32Func.timeBeginPeriodFunc( tc.wPeriodMin ) ;
	}

	// CPU �̃`�F�b�N
	{
		int CPUCODE ;
		int ENDMODE = 0, RDTSCUse = 0 ;
		int Str1 = 0,Str2 = 0,Str3 = 0 ;
		char String[5], CpuName[4*4*3+1] ;

		CpuName[0] = '\0' ;
		CpuName[48] = '\0' ;
		WinData.UseMMXFlag = FALSE ;
#ifdef DX_NON_INLINE_ASM
	#ifdef _WIN64
		{
			int CPUInfo[ 4 ] ;

			ENDMODE = 1 ;

			// �b�o�t�x���_�����擾
			__cpuid( CPUInfo, 0 ) ;
			CPUCODE = CPUInfo[ 0 ] ;
			Str1 = CPUInfo[ 1 ] ;
			Str2 = CPUInfo[ 3 ] ;
			Str3 = CPUInfo[ 2 ] ;

			// �p�t�H�[�}���X�J�E���^�������邩���`�F�b�N
			__cpuid( CPUInfo, 1 ) ;
			RDTSCUse = ( CPUInfo[ 3 ] & 0x10 ) != 0 ? TRUE : FALSE ;

			// ���x�͊g���b�o�t�h�c���g���邩����
			__cpuid( CPUInfo, 0x80000000 ) ;
			if( ( unsigned int )CPUInfo[ 0 ] >= 0x80000004 )
			{

				// �g����̂łb�o�t�����擾
				__cpuid( CPUInfo, 0x80000002 ) ;
				( ( int * )CpuName )[  0 ] = CPUInfo[ 0 ] ;
				( ( int * )CpuName )[  1 ] = CPUInfo[ 1 ] ;
				( ( int * )CpuName )[  2 ] = CPUInfo[ 2 ] ;
				( ( int * )CpuName )[  3 ] = CPUInfo[ 3 ] ;

				__cpuid( CPUInfo, 0x80000003 ) ;
				( ( int * )CpuName )[  4 ] = CPUInfo[ 0 ] ;
				( ( int * )CpuName )[  5 ] = CPUInfo[ 1 ] ;
				( ( int * )CpuName )[  6 ] = CPUInfo[ 2 ] ;
				( ( int * )CpuName )[  7 ] = CPUInfo[ 3 ] ;

				__cpuid( CPUInfo, 0x80000004 ) ;
				( ( int * )CpuName )[  8 ] = CPUInfo[ 0 ] ;
				( ( int * )CpuName )[  9 ] = CPUInfo[ 1 ] ;
				( ( int * )CpuName )[ 10 ] = CPUInfo[ 2 ] ;
				( ( int * )CpuName )[ 11 ] = CPUInfo[ 3 ] ;
			}

			// ���x�͂l�l�w���g���邩����
			__cpuid( CPUInfo, 1 ) ;
			if( ( CPUInfo[ 3 ] & 0x00800000 ) != 0 )
			{
				ENDMODE = 3 ;

				//SSE���g���邩����
				if( ( CPUInfo[ 3 ] & 0x02000000 ) != 0 )
				{
					ENDMODE	++ ;

					//SSE2���g���邩����
					if( ( CPUInfo[ 3 ] & 0x04000000 ) != 0 )
					{
						ENDMODE ++ ;
					}
				}
			}
		}
	#endif // _WIN64
#else // DX_NON_INLINE_ASM
		__asm{
			// CPUID ���g���邩����
			PUSHFD
			PUSHFD
			POP		EAX
			XOR		EAX , 00200000H
			PUSH	EAX
			POPFD
			PUSHFD
			POP		EBX
			MOV		ENDMODE , 0 
			CMP		EAX , EBX
			JNZ		MMXEND
			MOV		EAX , 0
		}
//			db 		0fh
//			db		0a2h
        cpuid
		__asm{
			CMP		EAX , 0

			// �g����̂łb�o�t�x���_���ł����[�h
			MOV		CPUCODE , EAX
			MOV		Str1, EBX
			MOV		Str2, EDX
			MOV		Str3, ECX
			MOV		ENDMODE , 1

			// ���łɃp�t�H�[�}���X�J�E���^�������邩�`�F�b�N
			MOV		EAX , 1
		}
//			db 0fh
//			db 0a2h
        cpuid
		__asm{
			AND		EDX , 16
			SHR		EDX , 4
			MOV		RDTSCUse , EDX

			// ���x�͊g���b�o�t�h�c���g���邩����
			MOV		EAX , 80000000H
//			db 0fh
//			db 0a2h
		}
        cpuid
		__asm{
			CMP		EAX , 80000004H
			JB		CHECKMMX

			// �g����̂łb�o�t�����擾
			MOV		EAX , 80000002H
		}
//			db 0fh
//			db 0a2h
        cpuid
		__asm{
			MOV		DWORD PTR [CpuName + 0] , EAX
			MOV		DWORD PTR [CpuName + 4] , EBX
			MOV		DWORD PTR [CpuName + 8] , ECX
			MOV		DWORD PTR [CpuName + 12] , EDX
			MOV		EAX , 80000003H
		}
//			db 0fh
//			db 0a2h
        cpuid
		__asm{
			MOV		DWORD PTR [CpuName + 16] , EAX
			MOV		DWORD PTR [CpuName + 20] , EBX
			MOV		DWORD PTR [CpuName + 24] , ECX
			MOV		DWORD PTR [CpuName + 28] , EDX
			MOV		EAX , 80000004H
		}
//			db 0fh
//			db 0a2h
        cpuid
		__asm{
			MOV		DWORD PTR [CpuName + 32] , EAX
			MOV		DWORD PTR [CpuName + 36] , EBX
			MOV		DWORD PTR [CpuName + 40] , ECX
			MOV		DWORD PTR [CpuName + 44] , EDX

CHECKMMX:
			// ���x�͂l�l�w���g���邩����
			MOV		EAX , 1
		}
//			db 0fh
//			db 0a2h
        cpuid
		__asm{
			AND		EDX , 00800000H
			JZ		MMXEND
			MOV		ENDMODE , 3
			//SSE���g���邩����
			MOV		EAX , 1
		}
		cpuid
		__asm{
			AND		EDX	, 02000000H
			JZ		MMXEND
			ADD		ENDMODE	, 1
			//SSE2���g���邩����
			MOV		EAX	, 1
		}
		cpuid
		__asm{
			AND		EDX	, 04000000H
			JZ		MMXEND
			ADD		ENDMODE	, 1
MMXEND:
			POPFD
		}
#endif  // DX_NON_INLINE_ASM
			
		// �p�t�H�[�}���X�J�E���^�t���O��ۑ�
		WinData.UseRDTSCFlag = RDTSCUse ;

#ifdef DX_NON_INLINE_ASM
	#ifdef _WIN64
		// �p�t�H�[�}���X�J�E���^��������ꍇ�̓N���b�N�����ȒP�Ɍv��
		if( RDTSCUse )
		{
			ULONGLONG Clock1, Clock2 ;
			int Time ;
			DWORD Clock ;

			Clock1 = __rdtsc() ;

			Time = NS_GetNowCount( FALSE ) ;
			while( NS_GetNowCount( FALSE ) - Time < 100 ){}

			Clock2 = __rdtsc() ;

			Clock = ( DWORD )_DTOL( ( double )( Clock2 - Clock1 ) / 100000 ) ; 
			WinData.OneSecCount = ( Clock2 - Clock1 ) * 10 ;

			DXST_ERRORLOGFMT_ADDUTF16LE(( "\xfe\x73\x42\x66\xb9\x70\x6e\x30\x23\xff\x30\xff\x35\xff\xd5\x52\x5c\x4f\x1f\x90\xa6\x5e\x1a\xff\x27\x59\x53\x4f\x25\x00\x2e\x00\x32\x00\x66\x00\x47\x00\x48\x00\x7a\x00\x00"/*@ L"�����_�̂b�o�t���쑬�x�F���%.2fGHz" @*/, (float)Clock / 1000.0f )) ;
			WinData.PcInfo.CPUSpeed = ( int )Clock ;
		}

	#endif // _WIN64
#else
		// �p�t�H�[�}���X�J�E���^��������ꍇ�̓N���b�N�����ȒP�Ɍv��
		if( RDTSCUse )
		{
			LARGE_INTEGER Clock1, Clock2 ;
			int Time ;
			DWORD Clock ;
#ifdef __BCC
			unsigned int low2, high2 ;
			unsigned int low1, high1 ;

			__asm
			{
				db 0fh
				db 031h
				MOV		low1, EAX
				MOV		high1, EDX
			}

			Time = NS_GetNowCount(FALSE) ;
			while( NS_GetNowCount(FALSE) - Time < 100 ){}

			__asm
			{
				db 0fh
				db 031h
				MOV		low2, EAX
				MOV		high2, EDX
			}

			Clock1.LowPart = low1 ; Clock1.HighPart = high1 ;
			Clock2.LowPart = low2 ; Clock2.HighPart = high2 ;
#else // __BCC
			__asm
			{
				RDTSC
				MOV		Clock1.LowPart, EAX
				MOV		Clock1.HighPart, EDX
			}

			Time = NS_GetNowCount(FALSE) ;
			while( NS_GetNowCount(FALSE) - Time < 100 ){}

			__asm
			{
				RDTSC
				MOV		Clock2.LowPart, EAX
				MOV		Clock2.HighPart, EDX
			}
#endif // __BCC

			Clock = ( DWORD )_DTOL( (double)( Clock2.QuadPart - Clock1.QuadPart ) / 100000 ) ; 
			WinData.OneSecCount = ( Clock2.QuadPart - Clock1.QuadPart ) * 10 ;

			DXST_ERRORLOGFMT_ADDUTF16LE(( "\xfe\x73\x42\x66\xb9\x70\x6e\x30\x23\xff\x30\xff\x35\xff\xd5\x52\x5c\x4f\x1f\x90\xa6\x5e\x1a\xff\x27\x59\x53\x4f\x25\x00\x2e\x00\x32\x00\x66\x00\x47\x00\x48\x00\x7a\x00\x00"/*@ L"�����_�̂b�o�t���쑬�x�F���%.2fGHz" @*/, (float)Clock / 1000.0f )) ;
			WinData.PcInfo.CPUSpeed = ( int )Clock ;
		}
#endif // DX_NON_INLINE_ASM

		switch( ENDMODE )
		{
		case 0 :
#ifndef DX_NON_INLINE_ASM
			DXST_ERRORLOG_ADDUTF16LE( "\x23\xff\x30\xff\x35\xff\x29\xff\x24\xff\x7d\x54\xe4\x4e\x6f\x30\x7f\x4f\x48\x30\x7e\x30\x5b\x30\x93\x30\x0a\x00\x00"/*@ L"�b�o�t�h�c���߂͎g���܂���\n" @*/ ) ;
#endif // DX_NON_INLINE_ASM
			break ;

		case 1 :
			DXST_ERRORLOG_ADDUTF16LE( "\x2d\xff\x2d\xff\x38\xff\x6f\x30\x7f\x4f\x48\x30\x7e\x30\x5b\x30\x93\x30\x0a\x00\x00"/*@ L"�l�l�w�͎g���܂���\n" @*/ ) ;
			break ;

		case 3 :
			DXST_ERRORLOG_ADDUTF16LE( "\x2d\xff\x2d\xff\x38\xff\x7d\x54\xe4\x4e\x92\x30\x7f\x4f\x28\x75\x57\x30\x7e\x30\x59\x30\x0a\x00\x00"/*@ L"�l�l�w���߂��g�p���܂�\n" @*/ ) ;
			WinData.UseMMXFlag = TRUE ;
			break ;

		case 4 :
			DXST_ERRORLOG_ADDUTF16LE( "\x2d\xff\x2d\xff\x38\xff\x7d\x54\xe4\x4e\x92\x30\x7f\x4f\x28\x75\x57\x30\x7e\x30\x59\x30\x0a\x00\x00"/*@ L"�l�l�w���߂��g�p���܂�\n" @*/ ) ;
			DXST_ERRORLOG_ADDUTF16LE( "\x33\xff\x33\xff\x25\xff\x7d\x54\xe4\x4e\x4c\x30\x7f\x4f\x28\x75\xef\x53\xfd\x80\x67\x30\x59\x30\x0a\x00\x00"/*@ L"�r�r�d���߂��g�p�\�ł�\n" @*/ ) ;
			WinData.UseMMXFlag = TRUE ;
			WinData.UseSSEFlag = TRUE ;
			break ;

		case 5 :
			DXST_ERRORLOG_ADDUTF16LE( "\x2d\xff\x2d\xff\x38\xff\x7d\x54\xe4\x4e\x92\x30\x7f\x4f\x28\x75\x57\x30\x7e\x30\x59\x30\x0a\x00\x00"/*@ L"�l�l�w���߂��g�p���܂�\n" @*/ ) ;
			DXST_ERRORLOG_ADDUTF16LE( "\x33\xff\x33\xff\x25\xff\x7d\x54\xe4\x4e\x4c\x30\x7f\x4f\x28\x75\xef\x53\xfd\x80\x67\x30\x59\x30\x0a\x00\x00"/*@ L"�r�r�d���߂��g�p�\�ł�\n" @*/ ) ;
			DXST_ERRORLOG_ADDUTF16LE( "\x33\xff\x33\xff\x25\xff\x12\xff\x7d\x54\xe4\x4e\x4c\x30\x7f\x4f\x28\x75\xef\x53\xfd\x80\x67\x30\x59\x30\x0a\x00\x00"/*@ L"�r�r�d�Q���߂��g�p�\�ł�\n" @*/ ) ;
			WinData.UseMMXFlag = TRUE ;
			WinData.UseSSEFlag = TRUE ;
			WinData.UseSSE2Flag = TRUE ;
			break ;
		}

		if( ENDMODE > 0 )
		{
			DXST_ERRORLOG_ADDUTF16LE( "\x23\xff\x30\xff\x35\xff\xd9\x30\xf3\x30\xc0\x30\x1a\xff\x00"/*@ L"�b�o�t�x���_�F" @*/ ) ;

			String[4] = 0 ;
			*( ( int * )&String[0] ) = Str1 ;
			DXST_ERRORLOG_ADDA( String ) ;

			*( ( int * )&String[0] ) = Str2 ;
			DXST_ERRORLOG_ADDA( String ) ;

			*( ( int * )&String[0] ) = Str3 ;
			DXST_ERRORLOG_ADDA( String ) ;

			DXST_ERRORLOG_ADDW( L"\n" ) ;

			if( CpuName[0] != '\0' )
			{
				DXST_ERRORLOGFMT_ADDA(( "\x82\x62\x82\x6f\x82\x74\x96\xbc\x81\x46%s"/*@ "�b�o�t���F%s" @*/, CpuName )) ;

				ConvString( ( const char * )CpuName, DX_CHARCODEFORMAT_ASCII, ( char * )WinData.PcInfo.CPUString, sizeof( WinData.PcInfo.CPUString ), WCHAR_T_CHARCODEFORMAT ) ;
			}
		}
	}

	DXST_ERRORLOG_TABSUB ;

	// ���� DirectX �̃o�[�W�������V�ȉ��������ꍇ�͂����Ń\�t�g���I��
	if( WinData.DirectXVersion < DX_DIRECTXVERSION_7 )
	{
#ifndef DX_NON_LITERAL_STRING
		if( WinData.DirectXVersion == DX_DIRECTXVERSION_NON )
		{
			MessageBoxW( NULL, ( wchar_t * )"\x44\x00\x69\x00\x72\x00\x65\x00\x63\x00\x74\x00\x58\x00\x20\x00\x4c\x30\xa4\x30\xf3\x30\xb9\x30\xc8\x30\xfc\x30\xeb\x30\x55\x30\x8c\x30\x66\x30\x44\x30\x6a\x30\x44\x30\x6e\x30\x67\x30\x53\x30\x53\x30\x67\x30\x42\x7d\x86\x4e\x57\x30\x7e\x30\x59\x30\x00"/*@ L"DirectX ���C���X�g�[������Ă��Ȃ��̂ł����ŏI�����܂�" @*/, ( wchar_t * )"\xa8\x30\xe9\x30\xfc\x30\x00"/*@ L"�G���[" @*/, MB_OK ) ;
		}
		else
		{
			MessageBoxW( NULL, ( wchar_t * )"\x44\x00\x69\x00\x72\x00\x65\x00\x63\x00\x74\x00\x58\x00\x20\x00\xd0\x30\xfc\x30\xb8\x30\xe7\x30\xf3\x30\x20\x00\x37\x00\x20\x00\xe5\x4e\x4d\x52\x6a\x30\x6e\x30\x67\x30\xbd\x30\xd5\x30\xc8\x30\x92\x30\x77\x8d\xd5\x52\x59\x30\x8b\x30\x53\x30\x68\x30\x4c\x30\xfa\x51\x65\x67\x7e\x30\x5b\x30\x93\x30\x00"/*@ L"DirectX �o�[�W���� 7 �ȑO�Ȃ̂Ń\�t�g���N�����邱�Ƃ��o���܂���" @*/, ( wchar_t * )"\xa8\x30\xe9\x30\xfc\x30\x00"/*@ L"�G���[" @*/, MB_OK ) ;
		}
#endif
		ExitProcess( (DWORD)-1 ) ;
	}

	// �I��
	return 0 ;
}






















// ���b�Z�[�W�����֐�

int WM_PAINTProcess( HDC Dc, RECT *fillRect, int AlwaysFillFlag )
{
	// WM_PAINT ���b�Z�[�W�������t���O�𗧂Ă�
	WinData.WM_PAINTMessageFlag = TRUE ;

	// �c�w���C�u������ WM_PAINT �̏��������s���Ȃ��ꍇ�͂����ŏI��
	if( WinData.NotUseDxLibWM_PAINTProcess )
	{
		return 0 ;
	}

	// ��A�N�e�B�u�ŁA����A�N�e�B�u���ɕ\�������p�̉摜������ꍇ�͂����`�悷��
	if( ( WinData.ActiveFlag == FALSE && WinData.PauseGraph.GraphData != NULL && WinData.NonActiveRunFlag == FALSE ) /*|| 
		( WinData.MenuUseFlag == TRUE && WinData.MenuShredRunFlag == TRUE )*/ )
	{
		// �o�b�N�O���E���h�`��
		if( WinData.WindowMinSizeFlag == FALSE ) 
		{
			DrawBackGraph( Dc ) ;
		}
	}
	else
	{
		// ����ȊO�̏ꍇ

		// �c�w���C�u���������������ŁA�����[�W�������ݒ肳���
		// ���Ȃ��ꍇ�͉�ʂ���������������
		if( ( DxSysData.DxLib_InitializeFlag == FALSE && WinData.WindowRgn == NULL ) || AlwaysFillFlag )
		{
			// �o�b�N�O���E���h�J���[���L���ȏꍇ�͂��̐F���g�p����
			if( GSYS.Screen.EnableBackgroundColor == TRUE )
			{
				HBRUSH Brush;

				Brush = CreateSolidBrush( ( ( DWORD )GSYS.Screen.BackgroundBlue << 16 ) | ( ( DWORD )GSYS.Screen.BackgroundGreen << 8 ) | ( DWORD )GSYS.Screen.BackgroundRed );
				if( Brush != NULL )
				{
					FillRect( Dc, fillRect, Brush ) ;
					DeleteObject( Brush );
				}
				else
				{
					FillRect( Dc, fillRect, (HBRUSH)GetStockObject( BLACK_BRUSH ) ) ;
				}
			}
			else
			{
				FillRect( Dc, fillRect, (HBRUSH)GetStockObject( BLACK_BRUSH ) ) ;
			}
		}

		if( DxSysData.DxLib_InitializeFlag || WinData.WindowRgn != NULL )
		{
			// �Ƃ肠��������ʂ��R�s�[���Ă���
			if( NS_GetActiveGraph() == DX_SCREEN_BACK )
			{
				RECT ClientRect ;
				int waitVsync = NS_GetWaitVSyncFlag();
				NS_SetWaitVSyncFlag( FALSE );
				GetClientRect( GetDisplayWindowHandle(), &ClientRect ) ;

				if( fillRect->left   == ClientRect.left && 
					fillRect->right  == ClientRect.right &&
					fillRect->top    == ClientRect.top &&
					fillRect->bottom == ClientRect.bottom )
				{
					NS_ScreenCopy() ;
				}
				else
				{
					if( WinData.ScreenCopyRequestFlag == FALSE )
					{
						WinData.ScreenCopyRequestFlag      = TRUE ;
						WinData.ScreenCopyRequestStartTime = NS_GetNowCount() ;
					}
					else
					{
						if( NS_GetNowCount() - WinData.ScreenCopyRequestStartTime > 1000 / 60 * 4 )
						{
							NS_ScreenCopy() ;
							WinData.ScreenCopyRequestStartTime = NS_GetNowCount() ;
						}
					}
				}
				NS_SetWaitVSyncFlag( waitVsync );
			}
		}
	}

	return 0 ;
}


int WM_SIZEProcess( void )
{
	RECT   rect ;
	RECT   ClientRect ;
	int    Width ;
	int    Height ;
	int    CWidth ;
	int    CHeight ;
	double ExRateX ;
	double ExRateY ;

	// ���[�U�[�̃E�C���h�E���g�p���Ă��Ȃ��ꍇ�̂݃T�C�Y�̕␳���s��
	if( WinData.UserWindowFlag == TRUE ) return 0 ;

	GetMainWindowSize( &Width, &Height ) ;

	// �E�C���h�E�ւ̏o�̓T�C�Y�𔽉f������
	NS_GetWindowSizeExtendRate( &ExRateX, &ExRateY ) ;
	Width  = _DTOL( Width  * ExRateX ) ;
	Height = _DTOL( Height * ExRateY ) ;

	GetWindowRect( WinData.MainWindow, &rect ) ;
	if( GetClientRect( WinData.MainWindow, &ClientRect ) == 0 ) return 0 ;
	WinData.WindowRect.right  = WinData.WindowRect.left + ClientRect.right ;
	WinData.WindowRect.bottom = WinData.WindowRect.top  + ClientRect.bottom ;

	// �N���C�A���g�̈�̃T�C�Y�𓾂�
	CWidth  = ClientRect.right  - ClientRect.left ;
	CHeight = ClientRect.bottom - ClientRect.top ;

	// �N���C�A���g�̈�O�̕����̕��E�����𓾂�
	rect.right  -= CWidth ;
	rect.bottom -= CHeight ;

	// �E�C���h�E�ɉ�ʂ��t�B�b�g�����邩�ǂ����ŏ����𕪊�
	if( WinData.ScreenNotFitWindowSize == TRUE || WinData.WindowSizeValid == TRUE )
	{
		// �E�C���h�E�T�C�Y�ݒ肪�w�肳��Ă����炻����g�p����
		if( WinData.WindowSizeValid == TRUE )
		{
			CWidth  = WinData.WindowWidth ;
			CHeight = WinData.WindowHeight ;

			if( WinData.WindowSizeChangeEnable == TRUE || WinData.ScreenNotFitWindowSize == TRUE )
			{
				if( DxSysData.DxLib_RunInitializeFlag == FALSE )
				{
					WinData.WindowSizeValid = FALSE ;
				}
				else
				{
					WinData.WindowSizeValidResetRequest = TRUE ;
				}
			}
		}

		// �N���C�A���g�̈悪��ʗ̈���傫���Ȃ��Ă���ꍇ�͕␳����
		if( CWidth  > Width  ) CWidth  = Width  ;
		if( CHeight > Height ) CHeight = Height ;
	}
	else
	{
		// �N���C�A���g�̈悪��ʗ̈�ƈقȂ��Ă���ꍇ�͕␳����
		if( CWidth  != Width  ) CWidth  = Width  ;
		if( CHeight != Height ) CHeight = Height ;
	}
	rect.right  += CWidth ;
	rect.bottom += CHeight + GetToolBarHeight() ;

	// �E�C���h�E�̈ʒu�w�肪����ꍇ�͔��f
	if( WinData.WindowPosValid == TRUE )
	{
		int SX, SY;

		SX = WinData.WindowX - rect.left ;
		SY = WinData.WindowY - rect.top ;
		rect.left   += SX ;
		rect.top    += SY ;
		rect.right  += SX ;
		rect.bottom += SY ;

		if( DxSysData.DxLib_RunInitializeFlag == FALSE
#ifndef DX_NON_GRAPHICS
			&& GSYS.Screen.Graphics_Screen_ChangeModeFlag == FALSE
#endif // DX_NON_GRAPHICS
			)
		{
			WinData.WindowPosValid = FALSE ;
		}
#ifndef DX_NON_GRAPHICS
		if( WinData.BackBufferTransColorFlag == TRUE || WinData.UseUpdateLayerdWindowFlag == TRUE )
		{
			int SW, SH ;

			NS_GetDrawScreenSize( &SW, &SH ) ;
			rect.right  = rect.left + SW ;
			rect.bottom = rect.top  + SH ;
			if( WStyle_WindowModeTable[ WinData.WindowStyle ] & WS_CAPTION )
			{
				rect.bottom += WinData.SystembarHeight ;
			}
		}
#endif // DX_NON_GRAPHICS
	}
	else
	// �ő剻��Ԃ̏ꍇ�̓E�C���h�E���f�X�N�g�b�v�̒��S�ɔz�u����
	if( WinData.WindowMaximizeFlag )
	{
		int WindowX ;
		int WindowY ;
		int WindowCenterX ;
		int WindowCenterY ;
		int WindowSizeX ;
		int WindowSizeY ;

		if( WinData.ValidFirstWindowMaximizedRect )
		{
			WindowCenterX = ( WinData.FirstWindowMaximizedRect.right  + WinData.FirstWindowMaximizedRect.left ) / 2 ;
			WindowCenterY = ( WinData.FirstWindowMaximizedRect.bottom + WinData.FirstWindowMaximizedRect.top  ) / 2 ;
		}
		else
		{
			WindowCenterX = ( WinData.WindowMaximizedRect.right  + WinData.WindowMaximizedRect.left ) / 2 ;
			WindowCenterY = ( WinData.WindowMaximizedRect.bottom + WinData.WindowMaximizedRect.top  ) / 2 ;
		}

		WindowX     = WindowCenterX - ( rect.right  - rect.left ) / 2 ;
		WindowY     = WindowCenterY - ( rect.bottom - rect.top  ) / 2 ;
		WindowSizeX = rect.right  - rect.left ;
		WindowSizeY = rect.bottom - rect.top ;

		// �ʒu��T�C�Y�����̋�`�ƂP�����قȂ�ꍇ�͕␳����
		if( _ABS( WindowX - WinData.WindowMaximizedRect.left ) == 1 )
		{
			WindowX = WinData.WindowMaximizedRect.left ;
		}
		if( _ABS( WindowY - WinData.WindowMaximizedRect.top ) == 1 )
		{
			WindowY = WinData.WindowMaximizedRect.top ;
		}
		if( _ABS( WindowSizeX - ( WinData.WindowMaximizedRect.right - WinData.WindowMaximizedRect.left ) ) == 1 )
		{
			WindowSizeX = WinData.WindowMaximizedRect.right - WinData.WindowMaximizedRect.left ;
		}
		if( _ABS( WindowSizeY - ( WinData.WindowMaximizedRect.bottom - WinData.WindowMaximizedRect.top ) ) == 1 )
		{
			WindowSizeY = WinData.WindowMaximizedRect.bottom - WinData.WindowMaximizedRect.top ;
		}

		rect.left   = WindowX ;
		rect.top    = WindowY ;
		rect.right  = rect.left + WindowSizeX ;
		rect.bottom = rect.top  + WindowSizeY ;

		// ( �ő剻��̃E�C���h�E�̈ʒu��T�C�Y��ύX����ƁA�Q��ڈȍ~�̍ő剻�̃E�C���h�E�̃T�C�Y��ʒu���ςɂȂ�̂ŁA��U�␳�𖳌��� )
		rect = WinData.WindowMaximizedRect ;
		WinData.WindowMaximizedAdjustRect = rect ;
	}

	MoveWindow( WinData.MainWindow, rect.left, rect.top, rect.right-rect.left, rect.bottom-rect.top, TRUE ) ;
	if( WinData.WindowModeFlag == TRUE && WinData.VisibleFlag == FALSE )
	{
		WinData.VisibleFlag = TRUE ;
		SetWindowStyle() ;
	}

	// �I��
	return 0 ;
}

int WM_MOVEProcess( LPARAM lParam )
{
	RECT rect ;
	SIZE WindowSize ;

	// ���[�U�[�̃E�C���h�E���g�p���Ă���ꍇ�݈̂ړ����s��
	if( WinData.UserWindowFlag == TRUE ) return 0 ;

	// �E�C���h�E�̈ʒu��ۑ�
	if( GetClientRect( WinData.MainWindow , &rect ) == 0 ) return 0 ;

	WindowSize.cx = rect.right - rect.left ;
	WindowSize.cy = rect.bottom - rect.top ;

	WinData.WindowRect.left = ( LONG )( short )( lParam & 0xffff ) ;
	WinData.WindowRect.top  = ( LONG )( short )( ( lParam >> 16 ) & 0xffff ) ;

	WinData.WindowRect.right = WinData.WindowRect.left + WindowSize.cx ;
	WinData.WindowRect.bottom = WinData.WindowRect.top + WindowSize.cy ;

#ifndef DX_NON_GRAPHICS
	if( NS_GetActiveGraph() == DX_SCREEN_FRONT )
	{
#ifndef DX_NON_MASK
		int Flag ;

		// �}�X�N���g�p�I������葱�������
		Flag = NS_GetUseMaskScreenFlag() ;
		NS_SetUseMaskScreenFlag( FALSE ) ;

		// �}�X�N���g�p����葱�������
		NS_SetUseMaskScreenFlag( Flag ) ;
#endif
	}
#endif // DX_NON_GRAPHICS

	// �I��
	return 0 ;
}

int WM_ACTIVATEProcessUseStock( WPARAM wParam, LPARAM lParam, int APPMes, int Dummy )
{
	// ProcessMessage ���炱���ɗ����ꍇ( ���܂����N�G�X�g������Ă��Ȃ��A�E�C���h�E�쐬����ł��Ȃ��ꍇ )��
	// ProcessMessage �̍Ō�� WM_ACTIVATE ���s���悤�ɂ���
	if( WinData.ProcessMessageFlag == TRUE &&
		WinData.WindowCreateFlag == FALSE &&
		WinData.WM_ACTIVATE_StockNum < 512 )
	{
		WinData.WM_ACTIVATE_wParam[ WinData.WM_ACTIVATE_EndIndex ] = wParam ;
		WinData.WM_ACTIVATE_lParam[ WinData.WM_ACTIVATE_EndIndex ] = lParam ;
		WinData.WM_ACTIVATE_APPMes[ WinData.WM_ACTIVATE_EndIndex ] = APPMes ;
		WinData.WM_ACTIVATE_Dummy[  WinData.WM_ACTIVATE_EndIndex ] = Dummy ;
		WinData.WM_ACTIVATE_EndIndex = ( WinData.WM_ACTIVATE_EndIndex + 1 ) % 512 ;
		WinData.WM_ACTIVATE_StockNum ++ ;
	}
	else
	{
		WM_ACTIVATEProcess( wParam, lParam, APPMes, Dummy ) ;
	}

	return 0 ;
}

int WM_ACTIVATEProcess( WPARAM wParam, LPARAM /*lParam*/, int APPMes, int Dummy )
{
	int ActiveFlag ;

	// �A�N�e�B�u���ǂ������擾
	ActiveFlag = LOWORD( wParam ) != 0 ;

	// �E�C���h�E�쐬���ゾ�����ꍇ�͉��������I��
	if( WinData.WindowCreateFlag == TRUE )
	{
		// �A�N�e�B�u���ǂ�����ۑ�
		WinData.ActiveFlag = ActiveFlag ;
		return 0 ;
	}

	// �ŏ������ꂽ��Ԃ̃A�N�e�B�u���(wParam �� HIWORD �� 0 �ȊO)�͔�A�N�e�B�u�Ƃ��ď���������
	if( HIWORD( wParam ) != 0 )
	{
		ActiveFlag = FALSE ;
	}

	// �A�N�e�B�u��񂪍��܂łƓ����ꍇ�͉������Ȃ�
	if( WinData.ActiveFlag == ActiveFlag )
		return 0 ;

	// �A�N�e�B�u���ǂ�����ۑ�
	WinData.ActiveFlag = ActiveFlag ;

	if( ActiveFlag == TRUE )
	{
//		DXST_ERRORLOG_ADDW( L"�A�N�e�B�u�ɂȂ�܂���\n" ) ;

#ifndef DX_NON_INPUTSTRING
		// �h�l�d�ŕ��������͒��������ꍇ�͕�������m�肵�Ă��܂�
		if( CharBuf.IMEUseFlag_OSSet == TRUE && CharBuf.IMESwitch == TRUE )
		{
			CharBuf.IMERefreshStep = 1;
		}
#endif // DX_NON_INPUTSTRING

#ifndef DX_NON_GRAPHICS
		if( WinData.WindowModeFlag == TRUE )
		{
			Graphics_Win_WM_ACTIVATE_ActiveProcess() ;
		}
#endif // DX_NON_GRAPHICS

		if( WinData.WindowModeFlag == FALSE && /*WinData.StopFlag == TRUE &&*/ DxSysData.DxLib_InitializeFlag == TRUE )
		{
#ifndef DX_NON_GRAPHICS
			// �O���t�B�b�N�V�X�e���̕��A����
			NS_RestoreGraphSystem() ;
#endif // DX_NON_GRAPHICS

			WinData.StopFlag = FALSE ;

#ifndef DX_NON_MOVIE
			// ���[�r�[�O���t�B�b�N�̍Đ���Ԃ�߂�
			PlayMovieAll() ;
#endif

#ifndef DX_NON_SOUND
			// �T�E���h�̍Đ����ĊJ����
			PauseSoundMemAll( FALSE ) ;
			PauseSoftSoundAll( FALSE ) ;
#endif // DX_NON_SOUND
		}
		else
		{
			if( WinData.NonActiveRunFlag == FALSE )
			{
#ifndef DX_NON_MOVIE
				// ���[�r�[�O���t�B�b�N�̍Đ���Ԃ�߂�
				PlayMovieAll() ;
#endif

#ifndef DX_NON_SOUND
				// �T�E���h�̍Đ����ĊJ����
				PauseSoundMemAll( FALSE ) ;
				PauseSoftSoundAll( FALSE ) ;
#endif // DX_NON_SOUND
			}
		}

		// DirectInput�f�o�C�X�I�u�W�F�N�g�̍Ď擾
#ifndef DX_NON_INPUT
		RefreshInputDeviceAcquireState() ;
#endif // DX_NON_INPUT
	}
	else
	{
//		DXST_ERRORLOG_ADDW( L"��A�N�e�B�u�ɂȂ�܂���\n" ) ;

		if( WinData.WindowModeFlag == FALSE  )
		{
			if( DxSysData.NotDrawFlag == FALSE )
			{
#ifndef DX_NON_GRAPHICS
				// DirectX �̃I�u�W�F�N�g���������
				Graphics_ReleaseDirectXObject() ;
#endif // DX_NON_GRAPHICS
			}

#ifndef DX_NON_GRAPHICS
			if( WinData.WindowsVersion >= DX_WINDOWSVERSION_VISTA || GSYS.Setting.ValidHardware == FALSE )
			{
				ChangeDisplaySettings( NULL, 0 ) ;
				ShowWindow( WinData.MainWindow, SW_MINIMIZE );
			}
#endif // DX_NON_GRAPHICS
		}

		// �t���X�N���[�����[�h�̏ꍇ�̏���
#ifndef DX_NON_GRAPHICS
		if( WinData.WindowModeFlag == FALSE && NS_GetChangeDisplayFlag() && NS_GetUseDDrawObj() != NULL )
		{
			// �t���X�N���[���̏ꍇ�͕K�� DWM ��L���ɂ���
			SetEnableAero( TRUE ) ;

			// ���������X���b�h���o�^����Ă��Ȃ��ꍇ�̓\�t�g�I��
			if( NS_GetValidRestoreShredPoint() == FALSE ) 
			{
				// ���[�U�[�񋟂̃E�C���h�E�������牽�����Ȃ�
				if( WinData.UserWindowFlag == FALSE )
				{
					DXST_ERRORLOG_ADDUTF16LE( "\xa9\x5f\x43\x51\xa2\x95\x70\x65\x4c\x30\x7b\x76\x32\x93\x55\x30\x8c\x30\x66\x30\x44\x30\x6a\x30\x44\x30\x5f\x30\x81\x30\x42\x7d\x86\x4e\x57\x30\x7e\x30\x59\x30\x0a\x00\x00"/*@ L"�����֐����o�^����Ă��Ȃ����ߏI�����܂�\n" @*/ ) ;
				
					// �N���[�Y�t���O���|��Ă�����WM_CLOSE���b�Z�[�W�𑗂�
					if( WinData.CloseMessagePostFlag == FALSE )
					{
						WinData.CloseMessagePostFlag = TRUE ;
						PostMessageW( WinData.MainWindow, WM_CLOSE, 0, 0 );
					}

					return -1 ;
				}
			}

#ifndef DX_NON_MOVIE
			// ���[�r�[�O���t�B�b�N�̍Đ���Ԃ��Ƃ߂�
			PauseMovieAll() ;
#endif

#ifndef DX_NON_SOUND
			// �T�E���h�̍Đ����~�߂�
			PauseSoundMemAll( TRUE ) ;
			PauseSoftSoundAll( TRUE ) ;
#endif // DX_NON_SOUND

			WinData.StopFlag = TRUE ;
		}
		else
#endif // DX_NON_GRAPHICS
		{
			if( WinData.NonActiveRunFlag == FALSE )
			{
#ifndef DX_NON_MOVIE
				// ���[�r�[�O���t�B�b�N�̍Đ���Ԃ��Ƃ߂�
				PauseMovieAll() ;
#endif

#ifndef DX_NON_SOUND
				// �T�E���h�̍Đ����~�߂�
				PauseSoundMemAll( TRUE ) ;
				PauseSoftSoundAll( TRUE ) ;
#endif // DX_NON_SOUND
			}

			// DirectInput�f�o�C�X�I�u�W�F�N�g�̍Ď擾
#ifndef DX_NON_INPUT
			RefreshInputDeviceAcquireState() ;
#endif // DX_NON_INPUT
		}

		if( WinData.NotMoveMousePointerOutClientAreaFlag )
		{
			ClipCursor( NULL ) ;
			WinData.SetClipCursorFlag = FALSE ;
		}

		// �o�b�N�O���E���h�`��
		if( Dummy == FALSE )
		{
			HDC hdc ;

			hdc = GetDC( GetDisplayWindowHandle() ) ;
			if( hdc != NULL )
			{
				DrawBackGraph( hdc ) ;
				ReleaseDC( GetDisplayWindowHandle() , hdc ) ;
			}
		}
	}

	// �p�b�h�̃o�C�u���[�V������Ԃ��X�V
#ifndef DX_NON_INPUT
	RefreshEffectPlayState() ;
#endif // DX_NON_INPUT

	// �A�N�e�B�u��Ԃ��ω��������ɌĂԃR�[���o�b�N�֐����ݒ肳��Ă�����Ă�
	if( WinData.ActiveStateChangeCallBackFunction != NULL && APPMes == FALSE )
	{
		WinData.ActiveStateChangeCallBackFunction( ActiveFlag, WinData.ActiveStateChangeCallBackFunctionData ) ;
	}

	// �\�t�g�v���Z�X�̎��s�D�揇�ʂ��Z�b�g����
//	SetPriorityClass( GetCurrentProcess() , ActiveFlag ? HIGH_PRIORITY_CLASS : IDLE_PRIORITY_CLASS );

	// �^�X�N�X�C�b�`�̗L���������Z�b�g
	if( WinData.SysCommandOffFlag == TRUE )
	{
		if( WinData.WindowsVersion < DX_WINDOWSVERSION_NT31 )
		{
			// Win95 �J�[�l���̏ꍇ�̏���
			UINT nPreviousState;
//			SystemParametersInfoW( SPI_SETSCREENSAVERRUNNING, ActiveFlag, &nPreviousState, 0 ) ;
			SystemParametersInfoW( SPI_SETSCREENSAVERRUNNING/*SPI_SCREENSAVERRUNNING*/, ( UINT )ActiveFlag, &nPreviousState, 0 ) ;
		}
		else
		{
			// WinNT �J�[�l���̏ꍇ�̏���

			if( /*WinData.GetMessageHookHandle == NULL &&*/ ActiveFlag == TRUE )
			{
//				WinData.MessageHookThredID = GetWindowThreadProcessId( WinData.MainWindow, NULL ) ;
//				WinData.MessageHookThredID = GetWindowThreadProcessId( GetDesktopWindow(), NULL ) ;
				WinData.MessageHookDLL = LoadLibraryW( WinData.HookDLLFilePath ) ;

				// DLL ������������i�܂Ȃ�
				if( WinData.MessageHookDLL != NULL )
				{
					WinData.MessageHookCallBadk = ( MSGFUNC )GetProcAddress( WinData.MessageHookDLL, "SetMSGHookDll" ) ;
					if( WinData.MessageHookCallBadk != NULL )
					{
						WinData.MessageHookCallBadk( WinData.MainWindow, &WinData.KeyboardHookHandle, TRUE ) ;
//						WinData.GetMessageHookHandle = SetWindowsHookExW( WH_GETMESSAGE, WinData.MessageHookCallBadk, WinData.MessageHookDLL, WinData.MessageHookThredID ) ;
//						WinData.GetMessageHookHandle = SetWindowsHookExW( WH_GETMESSAGE, MsgHook, WinData.Instance, 0 ) ;
					}
				}
			}
			else
			if( /*WinData.GetMessageHookHandle != NULL &&*/ ActiveFlag == FALSE )
			{
				// ���b�Z�[�W�t�b�N�𖳌���
				if( WinData.MessageHookDLL != NULL )
				{
					WinData.MessageHookCallBadk( WinData.MainWindow, &WinData.KeyboardHookHandle, FALSE ) ;
//					UnhookWindowsHookEx( WinData.GetMessageHookHandle ) ;
//					UnhookWindowsHookEx( WinData.KeyboardHookHandle ) ;
					FreeLibrary( WinData.MessageHookDLL ) ;
					WinData.MessageHookCallBadk = NULL ;
					WinData.MessageHookDLL = NULL ;
//					WinData.GetMessageHookHandle = NULL ;
					WinData.KeyboardHookHandle = NULL ;
				}
			}
		}

		// ����
		WinData.LockInitializeFlag = TRUE ;
	}

	// ����I��
	return 0 ;
}



// SPI_GETFOREGROUNDLOCKTIMEOUT �̒�`
#if !defined(SPI_GETFOREGROUNDLOCKTIMEOUT) 
#define SPI_GETFOREGROUNDLOCKTIMEOUT 0x2000 
#define SPI_SETFOREGROUNDLOCKTIMEOUT 0x2001 
#endif 

// �\�t�g�̃E�C���h�E�Ƀt�H�[�J�X���ڂ�
extern void SetAbsoluteForegroundWindow( HWND hWnd, int Flag )
{
    DWORD nTargetID, nForegroundID;
    DWORD sp_time;

    // �t�H�A�O���E���h�E�B���h�E���쐬�����X���b�h��ID���擾
    nForegroundID = GetWindowThreadProcessId(GetForegroundWindow(), NULL);

    // �ړI�̃E�B���h�E���쐬�����X���b�h��ID���擾
    nTargetID = GetWindowThreadProcessId(hWnd, NULL );


    // �X���b�h�̃C���v�b�g��Ԃ����ѕt����
    AttachThreadInput(nTargetID, nForegroundID, TRUE );  // TRUE �Ō��ѕt��


    // ���݂̐ݒ�� sp_time �ɕۑ�
    SystemParametersInfoW( SPI_GETFOREGROUNDLOCKTIMEOUT,0,&sp_time,0);

    // �E�B���h�E�̐؂�ւ����Ԃ� 0ms �ɂ���
    SystemParametersInfoW( SPI_SETFOREGROUNDLOCKTIMEOUT,0,(LPVOID)0,0);


    // �E�B���h�E���t�H�A�O���E���h�Ɏ����Ă���
    SetForegroundWindow(hWnd);


    // �ݒ�����ɖ߂�
    SystemParametersInfoW( SPI_SETFOREGROUNDLOCKTIMEOUT,0,&sp_time,0);


    // �X���b�h�̃C���v�b�g��Ԃ�؂藣��
    AttachThreadInput(nTargetID, nForegroundID, FALSE );  // FALSE �Ő؂藣��

	// �E�C���h�E���őO�ʂɎ����Ă���
	if( Flag == TRUE ) SetWindowPos( WinData.MainWindow , HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOREDRAW ) ;
}



// �a�l�o���\�[�X���� BITMAPINFO �Ɖ摜�C���[�W���\�z����
extern int GetBmpImageToResource( int ResourceID, BITMAPINFO **BmpInfoP, void **GraphDataP )
{
	BITMAPINFO *BmpInfo = NULL ;
	void *GraphData = NULL ;
	HRSRC RSrc ;
	void *DataBuf ;
	BYTE *DataP ;
	HGLOBAL Global ;
	int Er = FALSE ;

	// ���\�[�X���擾
	RSrc = FindResource( WinData.LoadResourModule == NULL ? WinAPIData.Win32Func.GetModuleHandleWFunc( NULL ) : WinData.LoadResourModule, MAKEINTRESOURCE( ResourceID ), RT_BITMAP ) ;
	if( RSrc == NULL ) return -1 ;

	// ���\�[�X���i�[����Ă��郁�����̈���擾
	Global = LoadResource( WinData.LoadResourModule == NULL ? WinAPIData.Win32Func.GetModuleHandleWFunc( NULL ) : WinData.LoadResourModule, RSrc ) ;
	DataP = ( BYTE * )( DataBuf = LockResource( Global ) ) ;
	if( DataBuf == NULL ) return -1 ;

	// �f�[�^�����
	{
		BITMAPINFO			BmpInfoT ;
		int					ImageSize ;

		// BITMAPINFO��ǂ݂���
		memcpy( &BmpInfoT, DataP, sizeof( BITMAPINFOHEADER ) ) ;
		DataP += sizeof( BITMAPINFOHEADER ) ;

		// �J���[�r�b�g�����W�ȉ��̎��̓p���b�g��ǂ�
		if( BmpInfoT.bmiHeader.biBitCount <= 8 )
		{
			// �a�l�o�h�m�e�n�\���̂̊i�[�p���������m��
			if( ( BmpInfo = ( BITMAPINFO * )DXCALLOC( ( size_t )( sizeof( BITMAPINFOHEADER ) + ( ( size_t )1 << BmpInfoT.bmiHeader.biBitCount ) * sizeof( RGBQUAD ) ) ) ) == NULL )
			{
				Er = TRUE ; goto END1 ;
			}

			// �a�l�o�h�m�e�n�\���̂̓��e���R�s�[
			memcpy( BmpInfo, &BmpInfoT, sizeof( BITMAPINFOHEADER ) ) ;

			// �c��̃J���[�p���b�g�̓ǂ݂���
			memcpy( ( ( BYTE * )BmpInfo ) + sizeof( BITMAPINFOHEADER ), DataP, ( ( size_t )1 << BmpInfoT.bmiHeader.biBitCount ) * sizeof( RGBQUAD ) ) ;
			DataP += ( ( size_t )1 << BmpInfoT.bmiHeader.biBitCount ) * sizeof( RGBQUAD ) ;
		}
		else
		// �J���[�r�b�g�����R�Q���P�U�ŃJ���[�}�X�N�g�p���̏���
		if( BmpInfoT.bmiHeader.biBitCount == 16 || BmpInfoT.bmiHeader.biBitCount == 32 || BmpInfoT.bmiHeader.biCompression == BI_BITFIELDS )
		{
			// �a�l�o�h�m�e�n�\���̂̊i�[�p���������m��
			if( ( BmpInfo = ( BITMAPINFO * )DXCALLOC( sizeof( BITMAPINFOHEADER ) + sizeof( RGBQUAD ) * 3 ) ) == NULL )
			{
				Er = TRUE ; goto END1 ;
			}

			// �a�l�o�h�m�e�n�g�d�`�c�d�q�\���̂̓��e���R�s�[
			memcpy( BmpInfo, &BmpInfoT, sizeof( BITMAPINFOHEADER ) ) ;

			// �J���[�}�X�N�̓ǂ݂���
			memcpy( ( ( BYTE * )BmpInfo ) + sizeof( BITMAPINFOHEADER ), DataP, sizeof( RGBQUAD ) * 3 ) ;
			DataP += sizeof( RGBQUAD ) * 3 ;
		}
		else
		// ����ȊO�̏ꍇ�̏���
		{
			// �a�l�o�h�m�e�n�\���̂̊i�[�p���������m��
			if( ( BmpInfo = ( BITMAPINFO * )DXCALLOC( sizeof( BITMAPINFO ) ) ) == NULL )
			{
				Er = TRUE ; goto END1 ;
			}

			// �a�l�o�h�m�e�n�g�d�`�c�d�q�\���̂̓��e���R�s�[
			memcpy( BmpInfo, &BmpInfoT, sizeof( BITMAPINFOHEADER ) ) ;
		}

		// �C���[�W�T�C�Y���v�Z����
		{
			int Byte ;

			Byte = BmpInfoT.bmiHeader.biWidth * BmpInfoT.bmiHeader.biBitCount / 8 ;
			Byte += Byte % 4 != 0 ? ( 4 - Byte % 4 ) : 0 ;

			ImageSize = Byte * _ABS( BmpInfoT.bmiHeader.biHeight ) ;
		}

		// �O���t�B�b�N�f�[�^�̈���m��
		if( ( GraphData = DXALLOC( ( size_t )ImageSize ) ) == NULL )
		{
			DXFREE( BmpInfo ) ; BmpInfo = NULL ;
			goto END1 ;
		}

		// �O���t�B�b�N�f�[�^�̓ǂ݂���
		memcpy( GraphData, DataP, ( size_t )ImageSize ) ;
		DataP += ImageSize ;
	}

END1 :
	// ���\�[�X���
	UnlockResource( Global ) ;

	if( Er == TRUE ) return -1 ;

	*BmpInfoP = BmpInfo ;
	*GraphDataP = GraphData ;

	// ����
	return 0 ;
}





#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE










