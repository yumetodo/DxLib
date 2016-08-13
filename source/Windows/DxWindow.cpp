// -------------------------------------------------------------------------------
// 
// 		ＤＸライブラリ		ウインドウ関係制御プログラム
// 
// 				Ver 3.16d
// 
// -------------------------------------------------------------------------------

// ＤＸライブラリ作成時用定義
#define __DX_MAKE

// インクルード ------------------------------------------------------------------
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

// マクロ定義 -------------------------------------
#ifdef __BCC
	#ifdef sprintf
	#undef sprintf
	#endif
	
	#ifdef vsprintf
	#undef vsprintf
	#endif
#endif

//メインウインドウのクラス名
#define DXCLASSNAME		L"D123987X"

//メインウインドウのデフォルトタイトル
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

// RDTSC ニーモニックと cpuid ニーニックの定義
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
#define WM_MOUSEWHEEL 				(0x20A)				// マウスのホイール動作メッセージの定義
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


//#define CHECKMULTIBYTECHAR(CP)		(( (unsigned char)*(CP) >= 0x81 && (unsigned char)*(CP) <= 0x9F ) || ( (unsigned char)*(CP) >= 0xE0 && (unsigned char)*(CP) <= 0xFC ))	// TRUE:２バイト文字  FALSE:１バイト文字

// ツールバー用定義
#define TOOLBAR_COMMANDID_BASE		(0x500)


// ウインドウスタイル定義
//#define WSTYLE_WINDOWMODE			(WS_MINIMIZEBOX | WS_POPUP | WS_SYSMENU | WS_CAPTION | WS_VISIBLE)	// 通常
//#define WSTYLE_WINDOWMODE			(WS_POPUP | WS_SYSMENU | WS_VISIBLE)								// タスクバーなし
//#define WEXSTYLE_WINDOWMODE		(WS_EX_CLIENTEDGE | WS_EX_WINDOWEDGE)
//#define WSTYLE_FULLSCREENMODE		(WS_POPUP)
//#define WEXSTYLE_FULLSCREENMODE	(WS_EX_TOPMOST)

// ウインドウスタイルの定義
#define WSTYLE_NUM					(12)		// ウインドウスタイルの数

#ifndef DX_GCC_COMPILE
// レイヤードウインドウ用の定義
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


//  0 :デフォルト
//  1 :タスクバーなし
//  2 :タスクバーなし枠なし
//  3 :枠なし
//  4 :何もなし
//  5 :最小化無し
//  6 :ツールバーに適したウインドウ
//  7 :最大化ボタンつきウインドウ、初期状態が通常サイズ
//  8 :最大化ボタンつきウインドウ、初期状態が最大化状態
//  9 :デフォルトに縁の立体化無し
// 10 :最大化ボタンつきウインドウ、縁の立体化無し
// 11 :閉じる無し、最小化無し
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

// ウインドウの奥行き位置設定タイプと対応する Win32API の値
static HWND WindowZType_Win32Param[] =
{
	HWND_TOP,			// DX_WIN_ZTYPE_NORMAL
	HWND_BOTTOM,		// DX_WIN_ZTYPE_BOTTOM
	HWND_TOP,			// DX_WIN_ZTYPE_TOP
	HWND_TOPMOST,		// DX_WIN_ZTYPE_TOPMOST
};

//typedef LRESULT ( CALLBACK *MSGFUNC)(int, WPARAM, LPARAM) ;		// メッセージフックのコールバック関数
#define F10MES				( WM_USER + 111 )
#define F12MES				( WM_USER + 112 )

// 構造体定義 --------------------------------------------------------------------

// 内部大域変数宣言 --------------------------------------------------------------

// 圧縮キーボードフックＤＬＬバイナリデータ
extern int  DxKeyHookBinaryConvert ;
extern BYTE DxKeyHookBinary[] ;
extern int  DxKeyHookBinary_x64_Convert ;
extern BYTE DxKeyHookBinary_x64[] ;

WINDATA WinData ;												// ウインドウのデータ

// 関数プロトタイプ宣言-----------------------------------------------------------

#ifndef DX_NON_INPUT

// マウスの入力情報を保存する
static	void		StockMouseInputInfo( int Button ) ;

#endif // DX_NON_INPUT

// ウインドウ関連
static void			GetMainWindowSize( int *SizeX, int *SizeY ) ;								// メインウインドウのサイズを取得する
static int			ConvScreenPositionToDxScreenPosition( int ScreenX, int ScreenY, int *DxScreenX, int *DxScreenY ) ;		// スクリーン座標をＤＸライブラリ画面座標に変換する

// メッセージ処理関数
static	LRESULT		CALLBACK DxLib_WinProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam ) ;	// メインウインドウのメッセージコールバック関数
//LRESULT CALLBACK	LowLevelKeyboardProc (INT nCode, WPARAM wParam, LPARAM lParam)	;			// フックされた時のコールバック関数
#ifdef DX_THREAD_SAFE
DWORD	WINAPI		ProcessMessageThreadFunction( LPVOID ) ;									// ProcessMessage をひたすら呼びつづけるスレッド
#endif

// ツールバー関係
static	int			SearchToolBarButton( int ID ) ;												// 指定のＩＤのボタンのインデックスを得る

// メニュー関係
static	int			SearchMenuItem( const wchar_t *ItemName, int ItemID, HMENU SearchMenu, HMENU *Menu, int *Index ) ;					// メニュー構造の中から、選択項目の位置情報を得る( -1:エラー  0:見つからなかった  1:見つかった )

static	int			GetDisplayMenuState( void ) ;												// メニューが表示するべきかどうかを取得する
static	int			_GetMenuItemInfo( HMENU Menu, int Index, MENUITEMINFOW *Buffer ) ;			// メニューアイテムの情報を取得する
static	HMENU		MenuItemSubMenuSetup( const wchar_t *ItemName, int ItemID ) ;				// 指定の選択項目にサブメニューを付けられるように準備をする

static	int			ListupMenuItemInfo( HMENU Menu ) ;											// メニューの選択項目の情報の一覧を作成する時に使用する関数
static	int			AddMenuItemInfo( HMENU Menu, int Index, int ID, const wchar_t *Name ) ;		// メニューの選択項目の情報を追加する
static	int			DeleteMenuItemInfo( const wchar_t *Name, int ID ) ;							// メニューの選択項目の情報を削除する
static	WINMENUITEMINFO *SearchMenuItemInfo( const wchar_t *Name, int ID ) ;					// メニューの選択項目の情報を取得する
static	int			GetNewMenuItemID( void ) ;													// 新しい選択項目のＩＤを取得する

// メッセージ処理関数
		int			WM_PAINTProcess( HDC Dc, RECT *FillRect, int AlwaysFillFlag = FALSE ) ;
		int			WM_SIZEProcess( void ) ;
		int			WM_SIZINGProcess( WPARAM wParam, LPARAM lParam ) ;
		int			WM_MOVEProcess( LPARAM lParam ) ;




// プログラム --------------------------------------------------------------------

// 初期化終了系関数

#pragma optimize("", off)

// ウインドウ関係の初期化関数
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

	// 初期化
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
	// グラフィックス処理が無効な場合は必ずウインドウモード
	WinData.WindowModeFlag = TRUE ;
#endif // DX_NON_GRAPHICS

	// インスタンスハンドルの取得
	WinData.Instance = WinAPIData.Win32Func.GetModuleHandleWFunc( NULL ) ;

	// スレッドＩＤの取得
	WinData.MainThreadID = GetCurrentThreadId() ;


#ifndef DX_NON_ASYNCLOAD
	// 非同期読み込み処理の初期化
	InitializeASyncLoad( WinData.MainThreadID ) ;
#endif // DX_NON_ASYNCLOAD

	// ファイルアクセス処理の初期化
	InitializeFile() ;

	// クラス名が何も設定されていない場合はデフォルトの名前を設定する
	if( WinData.ClassName[0] == L'\0' )
	{
		// ウインドウタイトルが設定されている場合はそれを設定する
		if( WinData.WindowText[0] != L'\0' )
		{
			_WCSCPY_S( WinData.ClassName, sizeof( WinData.ClassName ), WinData.WindowText );
		}
		else
		{
			_WCSCPY_S( WinData.ClassName, sizeof( WinData.ClassName ), DXCLASSNAME );
		}
	}

	// 空きメモリ容量の書き出し
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
					DXST_ERRORLOGFMT_ADDUTF16LE(( "\xe1\x30\xe2\x30\xea\x30\xcf\x7d\xcf\x91\x3a\x00\x25\x00\x2e\x00\x32\x00\x66\x00\x4d\x00\x42\x00\x20\x00\x20\x00\x7a\x7a\x4d\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x3a\x00\x25\x00\x2e\x00\x32\x00\x66\x00\x4d\x00\x42\x00\x20\x00\x00"/*@ L"メモリ総量:%.2fMB  空きメモリ領域:%.2fMB " @*/,
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
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\xe1\x30\xe2\x30\xea\x30\xcf\x7d\xcf\x91\x3a\x00\x25\x00\x2e\x00\x32\x00\x66\x00\x4d\x00\x42\x00\x20\x00\x20\x00\x7a\x7a\x4d\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x3a\x00\x25\x00\x2e\x00\x32\x00\x66\x00\x4d\x00\x42\x00\x20\x00\x00"/*@ L"メモリ総量:%.2fMB  空きメモリ領域:%.2fMB " @*/,
						( double )Mem.dwTotalPhys / 0x100000 , ( double )Mem.dwAvailPhys / 0x100000 )) ;  
			_SWNPRINTF( str, sizeof( str ) / 2, L"%.2fMB\n", ( double )Mem.dwAvailPhys / 0x100000 );
			OutputDebugStringW( str ) ;

			WinData.PcInfo.FreeMemorySize  = ( LONGLONG )Mem.dwAvailPhys ;
			WinData.PcInfo.TotalMemorySize = ( LONGLONG )Mem.dwTotalPhys ;
		}
	}

	// デスクチップのサイズとカラービット深度を保存
//	WinData.DefaultScreenSize.cx = GetSystemMetrics( SM_CXSCREEN ) ;
//	WinData.DefaultScreenSize.cy = GetSystemMetrics( SM_CYSCREEN ) ;
//	hdc = GetDC( NULL ) ;
//	WinData.DefaultColorBitCount = GetDeviceCaps( hdc , PLANES ) * GetDeviceCaps( hdc , BITSPIXEL ) ;
//	WinData.DefaultRefreshRate = GetDeviceCaps( hdc, VREFRESH ) ;
//	ReleaseDC( NULL , hdc ) ;

	// 描画領域のサイズを取得
	GetMainWindowSize( &WindowSizeX, &WindowSizeY ) ;

	// パフォーマンスカウンター情報の初期化
	{
		// パフォーマンスカウンターの周波数を取得する
		QueryPerformanceFrequency( ( LARGE_INTEGER * )&WinData.PerformanceClock ) ;

		// 使えるかどうかを保存しておく
		WinData.PerformanceTimerFlag = WinData.PerformanceClock != 0 ;
	}

	// タイマの精度を設定する
	{
		WinAPIData.Win32Func.timeGetDevCapsFunc( &tc , sizeof(TIMECAPS) );

		// マルチメディアタイマーのサービス精度を最大に 
		WinAPIData.Win32Func.timeBeginPeriodFunc( tc.wPeriodMin ) ;
	}

	// タイマーの精度を検査する
	if( WinData.PerformanceTimerFlag )
	{
		int Cnt1 , Cnt2 ;
		DWORD Time1 , NowTime1, StartTime ;
		LONGLONG Time2 , NowTime2 ;

		DXST_ERRORLOG_ADDUTF16LE( "\xbf\x30\xa4\x30\xde\x30\xfc\x30\x6e\x30\xbe\x7c\xa6\x5e\x92\x30\x1c\x69\xfb\x67\x57\x30\x7e\x30\x59\x30\x0a\x00\x00"/*@ L"タイマーの精度を検査します\n" @*/ ) ;

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
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\xbe\x7c\xa6\x5e\x50\x7d\x9c\x67\x20\x00\xf4\x66\xb0\x65\xde\x56\x70\x65\x20\x00\xde\x30\xeb\x30\xc1\x30\xe1\x30\xc7\x30\xa3\x30\xa2\x30\xbf\x30\xa4\x30\xde\x30\xfc\x30\x1a\xff\x25\x00\x64\x00\x20\x00\x20\x00\xd1\x30\xd5\x30\xa9\x30\xfc\x30\xde\x30\xf3\x30\xb9\x30\xab\x30\xa6\x30\xf3\x30\xbf\x30\xfc\x30\x1a\xff\x25\x00\x64\x00\x00"/*@ L"精度結果 更新回数 マルチメディアタイマー：%d  パフォーマンスカウンター：%d" @*/, Cnt1 , Cnt2 )) ;

#ifndef DX_NON_LOG
			NS_ErrorLogAdd( _T( " " ) ); // 謎のバグ回避用
#endif
			if( Cnt1 > Cnt2 )
			{
				DXST_ERRORLOGFMT_ADDUTF16LE(( "\x4d\x00\x75\x00\x6c\x00\x74\x00\x69\x00\x20\x00\x4d\x00\x65\x00\x64\x00\x69\x00\x61\x00\x20\x00\x54\x00\x69\x00\x6d\x00\x65\x00\x72\x00\x20\x00\x92\x30\x7f\x4f\x28\x75\x57\x30\x7e\x30\x59\x30\x20\x00\x54\x00\x69\x00\x6d\x00\x65\x00\x72\x00\x20\x00\xbe\x7c\xa6\x5e\x20\x00\x3a\x00\x20\x00\x25\x00\x64\x00\x2e\x00\x30\x00\x30\x00\x20\x00\x6d\x00\x73\x00\x20\x00\x00"/*@ L"Multi Media Timer を使用します Timer 精度 : %d.00 ms " @*/, tc.wPeriodMin )) ;
				WinData.PerformanceTimerFlag = FALSE ;
			}
			else
			{
				DXST_ERRORLOGFMT_ADDUTF16LE(( "\xd1\x30\xd5\x30\xa9\x30\xfc\x30\xde\x30\xf3\x30\xb9\x30\xab\x30\xa6\x30\xf3\x30\xbf\x30\xfc\x30\x92\x30\x7f\x4f\x28\x75\x57\x30\x7e\x30\x59\x30\x20\x00\xbf\x30\xa4\x30\xde\x30\xfc\x30\xbe\x7c\xa6\x5e\x20\x00\x3a\x00\x20\x00\x25\x00\x6c\x00\x66\x00\x20\x00\x4b\x00\x48\x00\x7a\x00\x20\x00\x00"/*@ L"パフォーマンスカウンターを使用します タイマー精度 : %lf KHz " @*/, WinData.PerformanceClock / 1000.0 )) ;
			}
#ifndef DX_NON_LOG
			NS_ErrorLogAdd( _T( " " ) ); // 謎のバグ回避用
#endif
		}
	}

	// ランダム係数を初期化
#ifndef DX_NON_MERSENNE_TWISTER
	srandMT( ( unsigned int )NS_GetNowCount() ) ;
#else
	srand( NS_GetNowCount() ) ;
#endif

	// スクリーンセーバー無効
	SystemParametersInfoW( SPI_SETSCREENSAVEACTIVE, FALSE, NULL, SPIF_SENDWININICHANGE ) ;

	// ウインドウに関連する処理は NotWinFlag が立っている場合は実行しない
	if( DxSysData.NotWinFlag == FALSE )
	{
		// 二重起動防止処理
		DXST_ERRORLOG_ADDUTF16LE( "\xbd\x30\xd5\x30\xc8\x30\x6e\x30\x8c\x4e\xcd\x91\x77\x8d\xd5\x52\x1c\x69\xfb\x67\x2e\x00\x2e\x00\x2e\x00\x20\x00\x00"/*@ L"ソフトの二重起動検査... " @*/ ) ;
		{
			if( FindWindowW( WinData.ClassName , NULL ) != NULL )
			{
				if( WinData.DoubleStartValidFlag == FALSE )
				{
					DXST_ERRORLOG_ADDUTF16LE( "\x8c\x4e\xcd\x91\x77\x8d\xd5\x52\x55\x30\x8c\x30\x66\x30\x44\x30\x7e\x30\x59\x30\x01\x30\xbd\x30\xd5\x30\xc8\x30\x92\x30\x42\x7d\x86\x4e\x57\x30\x7e\x30\x59\x30\x0a\x00\x00"/*@ L"二重起動されています、ソフトを終了します\n" @*/ ) ;
					WinData.QuitMessageFlag = TRUE;
					DxLib_EndRequest() ;
					return -1 ;
				}
				else
				{
					DXST_ERRORLOG_ADDUTF16LE( "\x8c\x4e\xcd\x91\x77\x8d\xd5\x52\x55\x30\x8c\x30\x66\x30\x44\x30\x7e\x30\x59\x30\x4c\x30\x9a\x7d\x4c\x88\x57\x30\x7e\x30\x59\x30\x0a\x00\x00"/*@ L"二重起動されていますが続行します\n" @*/ ) ;
				}
			}
			else
			{
				DXST_ERRORLOG_ADDUTF16LE( "\x8c\x4e\xcd\x91\x77\x8d\xd5\x52\x6f\x30\x55\x30\x8c\x30\x66\x30\x44\x30\x7e\x30\x5b\x30\x93\x30\x67\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"二重起動はされていませんでした\n" @*/ ) ;
			}
		}

		if( WinData.MenuUseFlag && DxSysData.NotWinFlag == FALSE )
		{
			WinData.MenuItemInfoNum = 0 ;
			ListupMenuItemInfo( WinData.Menu ) ;
		}

		// メニューを読み込む
	/*	if( WinData.MenuUseFlag )
		{
			// メニューをロード
			WinData.Menu = LoadMenuW( WinData.Instance, MAKEINTRESOURCE( WinData.MenuResourceID ) ) ;
	//		WinData.MenuResourceID = FALSE ;

			// 失敗していたら終了
			if( WinData.Menu == NULL ) WinData.MenuUseFlag = FALSE ;
		}
		else
		{
			WinData.Menu = NULL ;
		}
	*/

		// ソフトプロセスの実行優先順位を最高レベルにセット
	//	DXST_ERRORLOG_ADDW( L"ソフトの実行優先レベルを上げました\n" ) ;
	//	SetPriorityClass( GetCurrentProcess() , HIGH_PRIORITY_CLASS );

		// ウインドウがユーザーから提供されていない場合はＤＸライブラリが作成する
		// ユーザーから提供されている場合はプロシージャが呼ばれるようにする
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
			// ウインドウ作成中フラグを立てる
			WinData.WindowCreateFlag = TRUE ;

			// メインウインドウのウインドウクラスを登録
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

				DXST_ERRORLOG_ADDUTF16LE( "\xa6\x30\xa4\x30\xf3\x30\xc9\x30\xa6\x30\xaf\x30\xe9\x30\xb9\x30\x92\x30\x7b\x76\x32\x93\x57\x30\x7e\x30\x59\x30\x2e\x00\x2e\x00\x2e\x00\x20\x00\x00"/*@ L"ウインドウクラスを登録します... " @*/ ) ;
				if( !RegisterClassExW( &wc ) )
				{
					WinData.QuitMessageFlag = TRUE;
					DxLib_EndRequest() ;
					//return 	DXST_ERRORLOG_ADDW( L"ウインドウクラスの登録に失敗しました\n" ) ;
				}
				else
				{
					DXST_ERRORLOG_ADDUTF16LE( "\x7b\x76\x32\x93\x6b\x30\x10\x62\x9f\x52\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"登録に成功しました\n" @*/ ) ;
				}
			}

			// ウインドウネームをセット
			if( WinData.EnableWindowText == FALSE )
			{
				_WCSCPY_S( WinData.WindowText, sizeof( WinData.WindowText ), WIN_DEFAULT_TITLE ) ;
			}
			
			// ウインドウを生成
#ifndef DX_NON_GRAPHICS
			if( WinData.WindowModeFlag )
#endif // DX_NON_GRAPHICS
			{
				RECT Rect ;
				LONG AddStyle, AddExStyle ;
				int DesktopW, DesktopH, DesktopX, DesktopY ;

				// ウインドウモード時
				DXST_ERRORLOG_ADDUTF16LE( "\xa6\x30\xa4\x30\xf3\x30\xc9\x30\xa6\x30\xe2\x30\xfc\x30\xc9\x30\x77\x8d\xd5\x52\x28\x75\x6e\x30\xa6\x30\xa4\x30\xf3\x30\xc9\x30\xa6\x30\x92\x30\x5c\x4f\x10\x62\x57\x30\x7e\x30\x59\x30\x0a\x00\x00"/*@ L"ウインドウモード起動用のウインドウを作成します\n" @*/ ) ;

				AddExStyle = 0 ;
//				if( WinData.NotWindowVisibleFlag == TRUE ) AddExStyle |= WS_EX_TRANSPARENT ;

				// バックバッファの透過色の部分を透過させるフラグが立っているか、
				// UpdateLayerdWindow を使用するフラグが経っている場合は WS_EX_LAYERED を追加する
				if( WinData.BackBufferTransColorFlag == TRUE ||
					WinData.UseUpdateLayerdWindowFlag == TRUE ) AddExStyle |= WS_EX_LAYERED ;

				AddStyle = 0 ;
				// 非同期ウインドウモード変更機能が有効になっているときは WS_MAXIMIZEBOX を追加する
				if( WinData.UseChangeWindowModeFlag == TRUE ) AddStyle |= WS_MAXIMIZEBOX ;

				// 表示フラグが立っている場合は WS_VISIBLE を追加する
				if( WinData.VisibleFlag             == TRUE ) AddStyle |= WS_VISIBLE ;

				// ウインドウサイズが変更できるようになっている場合は WS_THICKFRAME を追加する
				if( WinData.WindowSizeChangeEnable  == TRUE ) AddStyle |= WS_THICKFRAME ;

				// マウス表示フラグを立てる
				WinData.MouseDispFlag = TRUE ;

				// クライアント領域の算出
				if( WinData.WindowSizeValid == TRUE )
				{
					if( WinData.ScreenNotFitWindowSize )
					{
						// フィット指定が無い場合はそのままのウインドウサイズをセット
						WindowSizeX = WinData.WindowWidth ;
						WindowSizeY = WinData.WindowHeight ;
					}
					else
					{
						// フィット指定がある場合は拡大率を指定する
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

				// システムバーを含めたウインドウのサイズを取得
				SETRECT( Rect, 0, 0, WindowSizeX, WindowSizeY ) ;
				AdjustWindowRectEx( &Rect ,
									WStyle_WindowModeTable[WinData.WindowStyle]   + AddStyle, FALSE,
									WExStyle_WindowModeTable[WinData.WindowStyle] + AddExStyle );
				WinData.SystembarHeight = -Rect.top ;

				// 設定領域をちょいと細工
		//		Rect.left   += - 3;
		//		Rect.top    += - 3;
		//		Rect.right  += + 3;
		//		Rect.bottom += + 3;

				// 中心に持ってくる
				NS_GetDefaultState( &DesktopW, &DesktopH, NULL, NULL, &DesktopX, &DesktopY ) ;
				WindowSizeX = Rect.right  - Rect.left ;
				WindowSizeY = Rect.bottom - Rect.top  ;
				Rect.left   += ( DesktopW - WindowSizeX ) / 2 + DesktopX ;
				Rect.top    += ( DesktopH - WindowSizeY ) / 2 + DesktopY ;
				Rect.right  += ( DesktopW - WindowSizeX ) / 2 + DesktopX ;
				Rect.bottom += ( DesktopH - WindowSizeY ) / 2 + DesktopY ;

				// ウインドウの作成
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

				DXST_ERRORLOG_ADDUTF16LE( "\xd5\x30\xeb\x30\xb9\x30\xaf\x30\xea\x30\xfc\x30\xf3\x30\xe2\x30\xfc\x30\xc9\x30\x28\x75\x6e\x30\xa6\x30\xa4\x30\xf3\x30\xc9\x30\xa6\x30\x92\x30\x5c\x4f\x10\x62\x57\x30\x7e\x30\x59\x30\x0a\x00\x00"/*@ L"フルスクリーンモード用のウインドウを作成します\n" @*/ ) ;
				// フルスクリーンモード時

				DisplayInfo = &GSYS.Screen.DisplayInfo[ GSYS.Screen.ValidUseDisplayIndex ? GSYS.Screen.UseDisplayIndex : 0 ] ;

				// マウス表示フラグを倒す
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
				return DXST_ERRORLOG_ADDUTF16LE( "\xa6\x30\xa4\x30\xf3\x30\xc9\x30\xa6\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"ウインドウの作成に失敗しました\n" @*/ ) ;
			}
			DXST_ERRORLOG_ADDUTF16LE( "\xa6\x30\xa4\x30\xf3\x30\xc9\x30\xa6\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x10\x62\x9f\x52\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"ウインドウの作成に成功しました\n" @*/ ) ;

			// ウインドウの表示、更新
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
				DXST_ERRORLOG_ADDUTF16LE( "\xa6\x30\xa4\x30\xf3\x30\xc9\x30\xa6\x30\x92\x30\x68\x88\x3a\x79\x57\x30\x7e\x30\x59\x30\x0a\x00\x00"/*@ L"ウインドウを表示します\n" @*/ ) ;
				ShowWindow( WinData.MainWindow , SW_SHOW ) ;
				UpdateWindow( WinData.MainWindow ) ;
			}

			// ウインドウをタッチ対応に登録する
			if( WinAPIData.Win32Func.RegisterTouchWindowFunc != NULL )
			{
				WinAPIData.Win32Func.RegisterTouchWindowFunc( WinData.MainWindow,  0 /* TWF_WANTPALM */ ) ;
			}
		}

		// 各種データをセットする
		{
			WinData.CloseMessagePostFlag = FALSE ;							// WM_CLOSEメッセージが送られるとＴＲＵＥになるフラグ変数をＦＡＬＳＥにして置く
			WinData.DestroyMessageCatchFlag = FALSE ;						// WM_DESTROYメッセージを受け取った時にＴＲＵＥになるフラグ変数をＦＡＬＳＥにしておく
			WinData.ActiveFlag = TRUE ;										// アクティブフラグをたてる
			if( WinData.WindowModeFlag == FALSE )
			{
				DXST_ERRORLOG_ADDUTF16LE( "\xab\x30\xfc\x30\xbd\x30\xeb\x30\x92\x30\x0d\x4e\xef\x53\x96\x89\x6b\x30\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"カーソルを不可視にしました\n" @*/ ) ;
				SetCursor( NULL ) ;										// フルスクリーンモードだった場合カーソルを消去
			}
			DXST_ERRORLOG_ADDUTF16LE( "\x29\xff\x2d\xff\x25\xff\x92\x30\x21\x71\xb9\x52\x6b\x30\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"ＩＭＥを無効にしました\n" @*/ ) ;

			if( WinData.WindowMinimizeFlag == FALSE 
#ifndef DX_NON_INPUTSTRING
				&& CharBuf.IMEUseFlag == FALSE
#endif // DX_NON_INPUTSTRING
			)
			{
				if( WinAPIData.Win32Func.WINNLSEnableIME_Func )
					WinAPIData.Win32Func.WINNLSEnableIME_Func( WinData.MainWindow , FALSE ) ;						// MS_IME無効
			}
			else
			{
#ifndef DX_NON_INPUTSTRING
				if( CharBuf.IMEUseFlag )
				{
					if( WinAPIData.Win32Func.WINNLSEnableIME_Func )
						WinAPIData.Win32Func.WINNLSEnableIME_Func( WinData.MainWindow , TRUE ) ;						// MS_IME無効
				}
#endif // DX_NON_INPUTSTRING
			}

			// ウインドウのクライアント領域を保存する
			if( WinData.WindowModeFlag )
			{
				GetClientRect( GetDisplayWindowHandle() , &WinData.WindowRect )  ;
				ClientToScreen( GetDisplayWindowHandle() , ( LPPOINT )&WinData.WindowRect ) ;
				ClientToScreen( GetDisplayWindowHandle() , ( LPPOINT )&WinData.WindowRect + 1 ) ;
			}
		}

		// マウスの表示設定を反映させる
		NS_SetMouseDispFlag( WinData.MouseDispFlag ) ;

		// 強制終了か，フラグを倒す
		WinData.AltF4_EndFlag = FALSE ;

		if( WinData.UserWindowFlag == FALSE )
		{
			// メインウインドウをアクティブにする
			if( WinData.NotWindowVisibleFlag == FALSE &&
				WinData.WindowMinimizeFlag == FALSE )
				BringWindowToTop( WinData.MainWindow ) ;

			// 少しだけ待つ
/*
			{
				int Time = NS_GetNowCount() ;
				while( NS_GetNowCount() - Time < 200 )
					NS_ProcessMessage() ;
			}
*/
			// サイズ補正処理
			if( WinData.NotWindowVisibleFlag == FALSE &&
				WinData.WindowMinimizeFlag == FALSE )
			{
				WM_SIZEProcess() ;
			}

			// ウインドウ作成中フラグを倒す
			WinData.WindowCreateFlag = FALSE ;
		}

		// 初期サイズ指定は完了しているのでフラグを倒す
		WinData.WindowPosValid = FALSE ;
	}

#ifdef DX_THREAD_SAFE
	// ProcessMessage をひたすら呼びつづけるスレッドを立てる
	WinData.ProcessMessageThreadHandle = CreateThread( NULL, 0, ProcessMessageThreadFunction, NULL, 0, &WinData.ProcessMessageThreadID ) ;
//	WinData.ProcessMessageThreadHandle = NULL ;
	if( WinData.ProcessMessageThreadHandle == NULL )
	{
		DXST_ERRORLOG_ADDUTF16LE( "\x50\x00\x72\x00\x6f\x00\x63\x00\x65\x00\x73\x00\x73\x00\x4d\x00\x65\x00\x73\x00\x73\x00\x61\x00\x67\x00\x65\x00\x20\x00\x92\x30\x72\x30\x5f\x30\x59\x30\x89\x30\x7c\x54\x73\x30\x64\x30\x65\x30\x51\x30\x8b\x30\xb9\x30\xec\x30\xc3\x30\xc9\x30\x6e\x30\xcb\x7a\x61\x30\x0a\x4e\x52\x30\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"ProcessMessage をひたすら呼びつづけるスレッドの立ち上げに失敗しました\n" @*/ ) ;
	}
#endif

#ifndef DX_NON_ASYNCLOAD
	// 非同期読み込み処理を行うスレッドを立てる
	if( SetupASyncLoadThread( WinData.ProcessorNum ) < 0 )
	{
		DXST_ERRORLOG_ADDUTF16LE( "\x5e\x97\x0c\x54\x1f\x67\xad\x8a\x7f\x30\xbc\x8f\x7f\x30\xe6\x51\x06\x74\x92\x30\x4c\x88\x46\x30\xb9\x30\xec\x30\xc3\x30\xc9\x30\x6e\x30\xcb\x7a\x61\x30\x0a\x4e\x52\x30\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"非同期読み込み処理を行うスレッドの立ち上げに失敗しました\n" @*/ ) ;
		WinData.QuitMessageFlag = TRUE;
		DxLib_EndRequest() ;
		return -1 ;
	}
#endif // DX_NON_ASYNCLOAD

	// 終了
	return 0 ;
}

#pragma optimize("", on)

// ウインドウ関係の処理終了関数
extern int TerminateWindow( void )
{
	// もし既にウインドウが無い場合は何もせず終了
	if( WinData.MainWindow == NULL ) return 0 ;

	// スクリーンセーバー有効
	SystemParametersInfoW( SPI_SETSCREENSAVEACTIVE, TRUE, NULL, SPIF_SENDWININICHANGE ) ;

	// ウインドウに関連する処理は DxSysData.NotWinFlag が立っていない場合のみ実行する
	if( DxSysData.NotWinFlag == FALSE )
	{
		// MS_IME有効
		if( WinAPIData.Win32Func.WINNLSEnableIME_Func )
			WinAPIData.Win32Func.WINNLSEnableIME_Func( WinData.MainWindow, TRUE ) ;

		// マウスカーソルを表示する
		NS_SetMouseDispFlag( TRUE ) ;

		// タスクスイッチの有効、無効をセットする
		{
	/*		if( WinData.WindowsVersion < DX_WINDOWSVERSION_NT31 )
			{
				// Win95 カーネルの場合の処理
				UINT nPreviousState;
				if( WinData.SysCommandOffFlag == TRUE )
					SystemParametersInfoW( SPI_SETSCREENSAVERRUNNING, FALSE, &nPreviousState, 0 ) ;
			}
			else
	*/		{
				// WinNT カーネルの場合の処理

				// キーボードフックを無効に
				if( WinData.TaskHookHandle != NULL )
				{
					UnhookWindowsHookEx( WinData.TaskHookHandle ) ;
					WinData.TaskHookHandle = NULL ;
				}

				// メッセージフックを無効に
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

				// キーボードフックＤＬＬとしてテンポラリファイルを使用していた場合はファイルを削除する
				if( WinData.NotUseUserHookDllFlag )
				{
					DeleteFileW( WinData.HookDLLFilePath ) ;
					WinData.NotUseUserHookDllFlag = FALSE ;
				}
			}
		}

		// リージョンを削除する
		NS_SetWindowRgnGraph( NULL ) ;

		// メニューを無効にする
		NS_SetUseMenuFlag( FALSE ) ;

		// ツールバーを無効にする
		NS_SetupToolBar( NULL, 0 ) ;

		// ユーザーのウインドウを使用しているかどうかで処理を分岐
		if( WinData.UserWindowFlag == TRUE )
		{
			// ウインドウプロージャを元に戻す
#ifdef _WIN64
			SetWindowLongPtrW( WinData.MainWindow, GWLP_WNDPROC, ( LONG_PTR )WinData.DefaultUserWindowProc ) ;
#else
			SetWindowLongW( WinData.MainWindow, GWL_WNDPROC, (LONG)WinData.DefaultUserWindowProc ) ;
#endif

			// 終了状態にする
			WinData.QuitMessageFlag = TRUE ;
			DxLib_EndRequest() ;
		}
		else
		{
			// クローズフラグが倒れていたらWM_CLOSEメッセージを送る
			if( WinData.CloseMessagePostFlag == FALSE )
			{
				WinData.CloseMessagePostFlag = TRUE ;
				PostMessageW( WinData.MainWindow , WM_CLOSE, 0, 0 );
			}

			// エンド処理が終るまでループ
			while( NS_ProcessMessage() == 0 && WinData.DestroyMessageCatchFlag == FALSE ){}

			// レジストリの削除
			UnregisterClassW( WinData.ClassName, WinData.Instance ) ;
		}

	#ifndef DX_NON_KEYEX
		// キー入力ハンドルの初期化
		NS_InitKeyInput() ;
	#endif

	/*
		// 背景用ＢＭＰオブジェクトの削除
		if( WinData.PauseGraph )
		{
			DeleteObject( ( HGDIOBJ )WinData.PauseGraph ) ;
			WinData.PauseGraph = NULL ;
		}o
	*/
		// 背景用ＢＭＰオブジェクトの削除
		if( WinData.PauseGraph.GraphData != NULL )
		{
			NS_ReleaseGraphImage( &WinData.PauseGraph ) ;
#ifndef DX_NON_GRAPHICS
			NS_DeleteGraph( WinData.PauseGraphHandle ) ;
#endif // DX_NON_GRAPHICS
			WinData.PauseGraphHandle = 0 ;
		}

		// ドラッグ＆ドロップされたファイル名のバッファを解放
		NS_DragFileInfoClear() ;
	}
	else
	{
		WinData.QuitMessageFlag = TRUE;
		DxLib_EndRequest() ;
	}

#ifdef DX_THREAD_SAFE

	// ProcessMessage をひたすら呼びつづけるスレッドを閉じる
	if( WinData.ProcessMessageThreadHandle != NULL )
	{
		// スレッドが終了するまで待つ
		while( WinData.ProcessMessageThreadExitFlag == 0 )
		{
			Sleep( 1 ) ;
		}

		// スレッドのハンドルを閉じる
		CloseHandle( WinData.ProcessMessageThreadHandle ) ;
	}

#endif

#ifndef DX_NON_ASYNCLOAD
	// 非同期読み込み処理用のスレッドを閉じる
	CloseASyncLoadThread() ;
#endif // DX_NON_ASYNCLOAD

	// ファイルアクセス処理の後始末
	TerminateFile() ;

#ifndef DX_NON_ASYNCLOAD
	// 非同期読み込み処理の後始末
	TerminateASyncLoad() ;
#endif // DX_NON_ASYNCLOAD
	
	// タイマの精度を元に戻す
	{
		TIMECAPS tc ;

		WinAPIData.Win32Func.timeGetDevCapsFunc( &tc , sizeof(TIMECAPS) );

		// マルチメディアタイマーのサービス精度を最大に
		WinAPIData.Win32Func.timeEndPeriodFunc( tc.wPeriodMin ) ;
	}

	// バックバッファの透過色の部分を透過させるためのビットマップがある場合は削除する
	if( WinData.BackBufferTransBitmap )
	{
		DeleteObject( WinData.BackBufferTransBitmap ) ;
		WinData.BackBufferTransBitmap = NULL ;
	}

	// メインウインドウハンドルにヌルをセットする
	//WinData.MainWindow = NULL ;

	// 浮動小数点の計算精度を元に戻す
//	_control87( 0x00000000, 0x00030000 );

	// 終了
	return 0 ;
}







// ＣＯＭ初期化、終了関係関数

// ＣＯＭインターフェースを初期化する
extern	int	InitializeCom( void )
{
	if( WinData.ComInitializeFlag ) return -1 ;

	DXST_ERRORLOG_ADDUTF16LE( "\x23\xff\x2f\xff\x2d\xff\x6e\x30\x1d\x52\x1f\x67\x16\x53\x2e\x00\x2e\x00\x2e\x00\x20\x00\x00"/*@ L"ＣＯＭの初期化... " @*/ ) ;

	// ＣＯＭの初期化
	if( FAILED( WinAPIData.Win32Func.CoInitializeExFunc( NULL, COINIT_APARTMENTTHREADED ) ) )
		return DXST_ERRORLOG_ADDUTF16LE( "\x23\xff\x2f\xff\x2d\xff\x6e\x30\x1d\x52\x1f\x67\x16\x53\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"ＣＯＭの初期化に失敗しました\n" @*/ ) ;

	DXST_ERRORLOG_ADDUTF16LE( "\x10\x62\x9f\x52\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"成功しました\n" @*/ ) ;

	WinData.ComInitializeFlag = TRUE ;

	// 終了
	return 0 ;
}

// ＣＯＭインターフェースを終了する
extern	int	TerminateCom( void )
{
	if( WinData.ComInitializeFlag == FALSE ) return -1 ;
	
	// ＣＯＭの終了
	WinAPIData.Win32Func.CoUninitializeFunc () ;

	DXST_ERRORLOG_ADDUTF16LE( "\x23\xff\x2f\xff\x2d\xff\x92\x30\x42\x7d\x86\x4e\x2e\x00\x2e\x00\x2e\x00\x20\x00\x8c\x5b\x86\x4e\x0a\x00\x00"/*@ L"ＣＯＭを終了... 完了\n" @*/ ) ;

	WinData.ComInitializeFlag = FALSE ;

	// 終了
	return 0 ;
}































































// 便利関数

// 指定のリソースを取得する( -1:失敗  0:成功 )
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

// 指定のリソースを取得する( -1:失敗  0:成功 )
extern int GetResourceInfo_WCHAR_T( const wchar_t *ResourceName, const wchar_t *ResourceType, void **DataPointerP, int *DataSizeP )
{
	HRSRC   ResourceHandle = NULL ;
	HGLOBAL ResourceMem    = NULL ;
	LPVOID  ResourceData   = NULL ;
	DWORD   ResourceSize   = 0 ;
	HMODULE ResourceModule ;

	// リソースハンドルを取得する
	ResourceModule = WinData.LoadResourModule == NULL ? WinAPIData.Win32Func.GetModuleHandleWFunc( NULL ) : WinData.LoadResourModule ;
	ResourceHandle = FindResourceW( ResourceModule, ResourceName, ResourceType ) ;
	if( ResourceHandle == NULL )
	{
		return -1 ;
	}

	// リソースのサイズを取得する
	ResourceSize = SizeofResource( ResourceModule, ResourceHandle ) ;
	if( ResourceSize == 0 )
	{
		return -1 ;
	}

	// リソースをメモリに読み込む
	ResourceMem = LoadResource( ResourceModule, ResourceHandle ) ;
	if( ResourceMem == NULL )
	{
		return -1 ;
	}

	// リソースデータが存在するメモリを取得する
	ResourceData = LockResource( ResourceMem ) ;
	if( ResourceData == NULL )
	{
		return -1 ;
	}

	// 取得した情報を書き込む
	if( DataPointerP != NULL )
	{
		*DataPointerP = ResourceData ;
	}

	if( DataSizeP != NULL )
	{
		*DataSizeP = ( int )ResourceSize ;
	}

	// 終了
	return 0 ;
}

// リソースＩＤからリソースＩＤ文字列を得る 
extern const TCHAR *NS_GetResourceIDString( int ResourceID )
{
	return MAKEINTRESOURCE( ResourceID ) ;
}

#if defined( DX_THREAD_SAFE ) || defined( DX_THREAD_SAFE_NETWORK_ONLY )

// 同時にＤＸライブラリの関数が呼ばれていないかチェックして、同時に呼ばれていたら待つ関数
extern void CheckConflictAndWaitDxFunction( void )
{
//	int Time ;
//	int num ;
	DWORD ThreadID ;
	HANDLE EventHandle ;

	// DxConflictWaitThreadID が初期化されていなかったら初期化する
	if( WinData.DxConflictWaitThreadIDInitializeFlag == FALSE )
	{
		int i ;

		// 初期化フラグを立てる
		WinData.DxConflictWaitThreadIDInitializeFlag = TRUE ;

		// 衝突時に使用するイベントの作成
		for( i = 0 ; i < MAX_THREADWAIT_NUM ; i ++ )
		{
			WinData.DxConflictWaitThreadID[ i ][ 1 ] = ( DWORD_PTR )CreateEvent( NULL, TRUE, FALSE, NULL ) ;
		}

		// クリティカルセクションも初期化する
		CriticalSection_Initialize( &WinData.DxConflictCheckCriticalSection ) ;
	}

	// この関数を呼び出したスレッドのＩＤを得る
	ThreadID = GetCurrentThreadId() ;

START :

	// この関数が使用中の場合は待つ
	CRITICALSECTION_LOCK( &WinData.DxConflictCheckCriticalSection ) ;
//	CheckConflict( &WinData.DxConflictCheckFlag ) ;

	// どのスレッドもＤＸライブラリの関数を使用していないか、
	// もしくは使用していてもそれが自分のスレッドだったら通過
	if( WinData.DxUseThreadFlag == FALSE || WinData.DxUseThreadID == ThreadID )
	{
		// 待っているスレッドが居て、それが自分のスレッドではない場合は待ち
		if( WinData.DxUseThreadFlag == FALSE && WinData.DxConflictWaitThreadNum != 0 && WinData.DxUseThreadID != ThreadID )
		{
			if( WinData.DxConflictWaitThreadID[ 0 ][ 0 ] != ThreadID )
			{
				goto WAIT ;
			}
			else
			{
				// もし待っていたのが自分だったらリストをスライドさせる
				if( WinData.DxConflictWaitThreadNum != 1 )
				{
					EventHandle = ( HANDLE )WinData.DxConflictWaitThreadID[ 0 ][ 1 ] ;
					_MEMCPY( &WinData.DxConflictWaitThreadID[ 0 ][ 0 ], &WinData.DxConflictWaitThreadID[ 1 ][ 0 ], sizeof( DWORD_PTR ) * 2 * ( WinData.DxConflictWaitThreadNum - 1 ) ) ;
					WinData.DxConflictWaitThreadID[ WinData.DxConflictWaitThreadNum - 1 ][ 1 ] = ( DWORD_PTR )EventHandle ;
					WinData.DxConflictWaitThreadID[ WinData.DxConflictWaitThreadNum - 1 ][ 0 ] = ( DWORD_PTR )0 ;
				}

				// 待っている数を減らす
				WinData.DxConflictWaitThreadNum -- ;
			}
		}

		// 情報をセット
		WinData.DxUseThreadFlag = TRUE ;
		WinData.DxUseThreadID = ThreadID ;

		// 使用中カウンタをインクリメントする
//		if( WinData.DxConflictCheckCounter != 0 )
//		{
//			DXST_ERRORLOG_ADDW( L"エラー:衝突発生 No.0 \n" ) ;
//		}
		WinData.DxConflictCheckCounter ++ ;

		goto END ;
	}

	// そうじゃない場合は待ち
WAIT:

	// もし待っているスレッドが限界を越えていたら単純な待ち処理を行う
	if( WinData.DxConflictWaitThreadNum == MAX_THREADWAIT_NUM )
	{
		// とりあえずこの関数を使用中、フラグを倒す
		CriticalSection_Unlock( &WinData.DxConflictCheckCriticalSection ) ;
//		WinData.DxConflictCheckFlag -- ;

//		DXST_ERRORLOG_ADDW( L"エラー:衝突発生 No.4 \n" ) ;

		// 少し寝る
		Sleep( 1 ) ;

		// 最初に戻る
		goto START ;
	}

	// 待ってるスレッドがいますよという情報を追加する
	WinData.DxConflictWaitThreadID[WinData.DxConflictWaitThreadNum][0] = ThreadID ;
	EventHandle = (HANDLE)WinData.DxConflictWaitThreadID[WinData.DxConflictWaitThreadNum][1] ;
	WinData.DxConflictWaitThreadNum ++ ;

	// とりあえずこの関数を使用中、フラグを倒す
	CriticalSection_Unlock( &WinData.DxConflictCheckCriticalSection ) ;
//	WinData.DxConflictCheckFlag -- ;

	// 自分の番が来るまで待つ
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
//			const char *ErrorStr = "長時間停止しっぱなしです\n" ;
			char ErrorStr[128] ;
			char String[256] ;
			Time = WinAPIData.Win32Func.timeGetTimeFunc() ;
			
			_SNPRINTF( ErrorStr, sizeof( ErrorStr ), "長時間停止しっぱなしです con:%d consub:%d\n", WinData.DxConflictCheckCounter, WinData.DxConflictCheckCounterSub ) ;
			
			// ログ出力抑制フラグが立っていた場合は出力を行わない
			if( DxSysData.NotLogOutFlag == FALSE && WinData.LogOutDirectory[0] != '\0' )
			{
				char MotoPath[FILEPATH_MAX] ;

				// エラーログファイルを開く
				DX_FGETDIR( MotoPath, sizeof( MotoPath ) ) ;
				DX_FSETDIR( WinData.LogOutDirectory ) ;
				fp = CreateFile( WinData.LogFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL ) ;
				if( fp != NULL )
				{
					SetFilePointer( fp, 0, NULL, FILE_END ) ;

					// 規定分だけタブを排出
					if( DxSysData.ErTabStop == FALSE )
					{
						int i ;	

						// タイムスタンプを出力
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

					// エラーログファイルに書き出す
					WriteFile( fp, ErrorStr, lstrlenA( String ), &WriteSize, NULL ) ;
//					fputs( ErrorStr , fp ) ;

					// エラーログをアウトプットに書き出す
					OutputDebugString( ErrorStr ) ;

					// 最後の文字が改行意外だった場合はタブストップフラグを立てる
					DxSysData.ErTabStop = ErrorStr[ lstrlenA( ErrorStr ) - 1 ] != '\n' ;

//					fclose( fp ) ;
					CloseHandle( fp ) ;
				}
				DX_FSETDIR( MotoPath ) ;
			}
		}
	}
*/
	
	// 待ってるスレッドがあるよカウンタをデクリメントする
//	WinData.DxConflictWaitThreadCounter -- ;
//	if( WinData.DxConflictWaitThreadCounter < 0 )
//	{
//		WinData.DxConflictWaitThreadCounter = 0 ;
//	}

	// 最初に戻る
	goto START ;

END :

	// 使用中フラグを倒す
	CriticalSection_Unlock( &WinData.DxConflictCheckCriticalSection ) ;
//	WinData.DxConflictCheckFlag -- ;
}
#endif // defined( DX_THREAD_SAFE ) || defined( DX_THREAD_SAFE_NETWORK_ONLY )


#if defined( DX_THREAD_SAFE ) || defined( DX_THREAD_SAFE_NETWORK_ONLY )

// CheckConfictAndWaitDxFunction を使用したＤＸライブラリの関数が return する前に呼ぶべき関数
extern void PostConflictProcessDxFunction( void )
{
	int WaitFlag = 0 ;
	
	// この関数が使用中の場合は待つ
	CRITICALSECTION_LOCK( &WinData.DxConflictCheckCriticalSection ) ;
//	CheckConflict( &WinData.DxConflictCheckFlag ) ;

	// カウンタが１の場合は、これでＤＸライブラリ使用中状態が
	// 解除されることになるので、処理を分岐
	if( WinData.DxConflictCheckCounter == 1 )
	{
		// 情報をリセットする
		WinData.DxUseThreadFlag = FALSE ;
		WinData.DxUseThreadID = 0xffffffff ;

		// もし待っているスレッドがある場合は、スリープするフラグを立てる
		if( WinData.DxConflictWaitThreadNum > 0 )
			WaitFlag = 1 ;
	}
//	else
//	{
//		DXST_ERRORLOG_ADDW( L"エラー:衝突発生 No.1\n" ) ;
//	}

	// カウンタをデクリメントして終了
	WinData.DxConflictCheckCounter -- ;

	// この関数を使用中、フラグを倒す
	CriticalSection_Unlock( &WinData.DxConflictCheckCriticalSection ) ;
//	WinData.DxConflictCheckFlag -- ;

	// スリープするフラグが立っていたらスリープする
	if( WaitFlag == 1 )
	{
//		DWORD ThreadID = GetCurrentThreadId() ;

		// 次に実行すべきスレッドのイベントをシグナル状態にする
		SetEvent( (HANDLE)WinData.DxConflictWaitThreadID[0][1] ) ;

		// 寝る
		Sleep( 0 ) ;

/*		// 待ってるスレッドがあるよカウンタが０になるのを待つ
		while( WinData.DxConflictWaitThreadNum > 0 )
		{
			Sleep( 0 ) ;
		}
		Sleep( 0 ) ;
*/
	}
}
#endif // defined( DX_THREAD_SAFE ) || defined( DX_THREAD_SAFE_NETWORK_ONLY )




















// メッセージ処理関数

// WM_PAINT用関数
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

	// グラフィックハンドルが無効になっていたら作り直す
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

	// DrawBackGraph を実行中フラグを立てる
	WinData.DrawBackGraphFlag = TRUE ;

	// 各種描画設定情報を取得
	Graphics_DrawSetting_GetScreenDrawSettingInfo( &ScreenDrawSettingInfo ) ;

	// 設定を一時的に変更する
/*	DrawScreen = NS_GetActiveGraph() ;
	DrawMode = NS_GetDrawMode() ;
	WaitVSync = NS_GetWaitVSyncFlag() ;
*/	NS_SetDrawScreen( DX_SCREEN_BACK ) ;
	NS_SetDrawMode( DX_DRAWMODE_BILINEAR ) ;
	NS_SetWaitVSyncFlag( FALSE ) ;

	// 現在のウインドウのクライアント領域のサイズを得る
	NS_GetDrawScreenSize( &DrawScreenWidth, &DrawScreenHeight ) ;

	// 画面一次保存用画像の作成
	{
		SETUP_GRAPHHANDLE_GPARAM GParam ;

		Graphics_Image_InitSetupGraphHandleGParam_Normal_NonDrawValid( &GParam, 32, FALSE, FALSE ) ;
		BackUpScreen = Graphics_Image_MakeGraph_UseGParam( &GParam, DrawScreenWidth, DrawScreenHeight, FALSE, FALSE, 0, FALSE ) ;
	}

	// 画面画像の取得
	NS_GetDrawScreenGraph( 0, 0, DrawScreenWidth, DrawScreenHeight, BackUpScreen ) ;

	// ポーズ画像のサイズを取得する
	NS_GetGraphSize( WinData.PauseGraphHandle, &Width, &Height ) ;

	// 画面一杯に描画する
	if( Width == DrawScreenWidth && Height == DrawScreenHeight )
	{
		NS_DrawGraph( 0, 0, WinData.PauseGraphHandle, FALSE ) ;
	}
	else
	{
		NS_DrawExtendGraph( 0, 0, DrawScreenWidth, DrawScreenHeight, WinData.PauseGraphHandle, FALSE ) ;
	}

	// 表画面にコピー
	NS_ScreenCopy() ;

	// 裏画面にもとの画像を描画する
	NS_DrawGraph( 0, 0, BackUpScreen, FALSE ) ;

	// 一次保存用画像の削除
	NS_DeleteGraph( BackUpScreen ) ;

	// 各種描画設定情報を元に戻す
	Graphics_DrawSetting_SetScreenDrawSettingInfo( &ScreenDrawSettingInfo ) ;

	// 描画設定を元に戻す
/*	NS_SetDrawScreen( DrawScreen ) ;
	NS_SetDrawMode( DrawMode ) ;
	NS_SetWaitVSyncFlag( WaitVSync ) ;
*/	WinData.NonActiveRunFlag = NonActiveRunFlag ;

	// DrawBackGraph を実行中フラグを倒す
	WinData.DrawBackGraphFlag = FALSE ;
#endif // DX_NON_GRAPHICS

	// 終了
	return 0 ;
}


// ウインドウ関連

// メインウインドウのサイズを取得する
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


// メインウインドウのメッセージコールバック関数
extern LRESULT CALLBACK DxLib_WinProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	int PrevWindowMinSizeFlag ;
	LRESULT Ret = -1000, UserProcRet = 0 ;

	// メッセージ出力
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

	// ユーザーから提供されたウインドウを使用している場合はそのウインドウのプロシージャを呼ぶ
	if( WinData.UserWindowFlag == TRUE )
	{
		if( WinData.DefaultUserWindowProc != NULL )
		{
			UserProcRet = WinData.DefaultUserWindowProc( hWnd, message, wParam, lParam ) ;
		}
	}

	// ユーザー定義のプロージャ関数があったらそれを呼ぶ
	if( WinData.UserWindowProc != NULL ) 
	{
		LRESULT RetValue ;

		WinData.UseUserWindowProcReturnValue = FALSE ;

		RetValue = WinData.UserWindowProc( hWnd, message, wParam, lParam ) ;

		// フラグが立っていたらここで終了
		if( WinData.UseUserWindowProcReturnValue )
		{
			return RetValue ;
		}
	}
	
	switch( message )
	{
	case WM_DROPFILES :
		// ファイルがドラッグ＆ドロップされた場合の処理
		{
			DWORD FileNum ;
			DWORD i ;
			DWORD size ;
			HDROP hDrop = ( HDROP )wParam ;
			
			// ファイルの数を取得
			FileNum = DragQueryFile( hDrop, 0xffffffff, NULL, 0 ) ;

			// ファイルの数だけ繰り返し
			for( i = 0 ; i < FileNum && WinData.DragFileNum < MAX_DRAGFILE_NUM ; i ++, WinData.DragFileNum ++ )
			{
				// 必要なバッファのサイズを取得する
				size = DragQueryFile( hDrop, ( DWORD )i, NULL, 0 ) ;
				
				// バッファの確保
				WinData.DragFileName[ WinData.DragFileNum ] = ( wchar_t * )DXALLOC( ( size + 1 ) * sizeof( wchar_t ) ) ;
				if( WinData.DragFileName[WinData.DragFileNum] == NULL ) break ;
				
				// ファイル名の取得
				DragQueryFileW( hDrop, i, WinData.DragFileName[WinData.DragFileNum], size + 1 ) ;
			}
			
			// 取得の終了
			DragFinish( hDrop ) ;
		}
		break ;
	
	case WM_CREATE :
		// ウインドウが作成された直後の時の処理
		if( WinData.WindowRgn != NULL )
		{
			SetWindowRgn( hWnd, WinData.WindowRgn, FALSE ) ;
		}
		break ;

	case WM_COMMAND :
		// ツールバーのコマンドか調べる
		if( WinData.ToolBarUseFlag == TRUE && LOWORD( wParam ) >= TOOLBAR_COMMANDID_BASE )
		{
			int Index ;
			WINTOOLBARITEMINFO *but ;

			Index = SearchToolBarButton( LOWORD( wParam ) - TOOLBAR_COMMANDID_BASE ) ;
			if( Index != -1 )
			{
				but = &WinData.ToolBarItem[ Index ] ;

				// タイプが普通のボタンの場合のみ Clik を TRUE にする
				if( but->Type == TOOLBUTTON_TYPE_NORMAL )
					but->Click = TRUE ;

				// 終了
				break ;
			}
		}

		// メニューのコマンドか調べる
		if( WinData.MenuUseFlag == TRUE /*&& HIWORD( wParam ) == 0*/ )
		{
			WORD ItemID ;

			ItemID = LOWORD( wParam ) ;
			
			// コールバック関数が登録されている場合はコールバック関数を呼ぶ
			if( WinData.MenuProc != NULL )
			{
				WinData.MenuProc( ItemID ) ;
			}

			if( WinData.MenuCallBackFunction != NULL )
			{
				TCHAR NameBuffer[128] ;
				
				// 選択項目の名前を取得する
				NS_GetMenuItemName( (int)ItemID, NameBuffer ) ;
				
				// コールバック関数を呼ぶ
				WinData.MenuCallBackFunction( NameBuffer, (int)ItemID ) ;
			}

			// 選択された項目の情報に追加
			if( WinData.SelectMenuItemNum < MAX_MENUITEMSELECT_NUM )
			{
				int i ;
			
				// 既にあったら追加しない
				for( i = 0 ; i < WinData.SelectMenuItemNum ; i ++ )
					if( WinData.SelectMenuItem[i] == ItemID ) break ;

				// 無かった場合のみ追加
				if( i == WinData.SelectMenuItemNum )
				{
					WinData.SelectMenuItem[WinData.SelectMenuItemNum] = ItemID ;
					WinData.SelectMenuItemNum ++ ;
				}
			}
		}
		break ;

#ifndef DX_NON_KEYEX
	// ＩＭＥメッセージが来た場合は独自関数に渡す
	case WM_IME_SETCONTEXT :
	case WM_IME_STARTCOMPOSITION :
	case WM_IME_ENDCOMPOSITION :
	case WM_IME_COMPOSITION :
	case WM_IME_NOTIFY :
	case WM_IME_REQUEST :
		return IMEProc( hWnd , message , wParam , lParam ) ;
#endif

	// ウインドウのサイズが変更されている時の処理
	case WM_SIZING :
		GetWindowRect( hWnd, &WinData.WindowEdgeRect ) ;

		// ウインドウモードの場合のみ処理
		if( WinData.WindowModeFlag == TRUE )
		{
			// ユーザーのウインドウを使用している場合は幅の調整はしない
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

				// デスクトップのサイズを取得する
				NS_GetDefaultState( &DesktopW, &DesktopH, NULL, NULL ) ; 

				// 描画画面のサイズを取得する
				NS_GetDrawScreenSize( &Width, &Height ) ;

				// 現在のウインドウの領域とクライアント領域を取得する
				GetWindowRect( hWnd, &WinRect ) ;
				GetClientRect( hWnd, &CliRect ) ;

				// クライアント領域以外の部分の幅と高さを算出
				AddWidth  = ( WinRect.right  - WinRect.left ) - ( CliRect.right  - CliRect.left ) ;
				AddHeight = ( WinRect.bottom - WinRect.top  ) - ( CliRect.bottom - CliRect.top  ) ;

				// クライアント領域のサイズを得る
				NextWidth  = ( NextRect->right  - NextRect->left ) - AddWidth ;
				NextHeight = ( NextRect->bottom - NextRect->top  ) - AddHeight ;
				if( NextWidth  + AddWidth  > DesktopW ) NextWidth  = DesktopW - AddWidth ;
				if( NextHeight + AddHeight > DesktopH ) NextHeight = DesktopH - AddHeight ;

				// クライアント領域に画面をフィットさせるかどうかで処理を分岐
				if( WinData.ScreenNotFitWindowSize == FALSE && WinData.WindowSizeValid == FALSE )
				{
					// フィットさせる場合

					// ユーザー指定の最小サイズが有効な場合は適用する
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

					// ユーザー指定の最大サイズが有効な場合は適用する
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

					// 幅と高さ、比率的にどちらがより大きいかを算出
					WidthBigFlag = ( (double)NextWidth / Width > (double)NextHeight / Height ) ? TRUE : FALSE ;

					// 新しい表示比率を設定する
					switch( Side )
					{
					case WMSZ_LEFT :
					case WMSZ_RIGHT :
			WIDTH_SIZE_BASE :
						if( NextWidth + AddWidth > DesktopW ) NextWidth = DesktopW - AddWidth ;
						WinData.WindowSizeExRateY =
						WinData.WindowSizeExRateX = (double)NextWidth / Width ;
						NextHeight = NextWidth * Height / Width ;

						// ユーザー指定の最小サイズが有効な場合は適用する
						if( WinData.WindowMinSizeValid && WinData.WindowMinHeight > NextHeight )
						{
							NextHeight = WinData.WindowMinHeight ;
							goto HEIGHT_SIZE_BASE ;
						}

						// ユーザー指定の最大サイズが有効な場合は適用する
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

						// ユーザー指定の最小サイズが有効な場合は適用する
						if( WinData.WindowMinSizeValid && WinData.WindowMinWidth > NextWidth )
						{
							NextWidth = WinData.WindowMinWidth ;
							goto WIDTH_SIZE_BASE ;
						}

						// ユーザー指定の最大サイズが有効な場合は適用する
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

					// サイズが 128 以下にはならないようにする
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

					// ウインドウの形を補正する
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

					// フィットさせない場合

					// 幅、高さが画面のサイズ以上になっていたら制限する
					NS_GetDrawScreenSize( &MainScreenW, &MainScreenH ) ;
					MainScreenW = _DTOL( MainScreenW * WinData.WindowSizeExRateX ) ;
					MainScreenH = _DTOL( MainScreenH * WinData.WindowSizeExRateY ) ;

					// ユーザー指定の最小サイズが描画画面のサイズより大きければ( 且つ描画先画面より小さければ )そちらを使用する
					if( WinData.WindowMinSizeValid )
					{
						MinSizeW = WinData.WindowMinWidth ;
						MinSizeH = WinData.WindowMinHeight ;

						// 128ピクセル以下にはできない
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

					// ユーザー指定の最大サイズが描画画面のサイズより小さければそちらを使用する
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

					// ウインドウの形を補正する
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


	// ウインドウのサイズを一定に保つ処理
	case WM_SIZE :
		GetWindowRect( hWnd, &WinData.WindowEdgeRect ) ;

		// 最大化ではなかったら最大化状態フラグを倒す
		if( wParam != SIZE_MAXIMIZED )
		{
			WinData.WindowMaximizeFlag = FALSE ;
		}

		PrevWindowMinSizeFlag = WinData.WindowMinSizeFlag ;
		WinData.WindowMinSizeFlag = wParam == SIZE_MINIMIZED ? TRUE : FALSE ;

		// もし最大化された場合で且つ非同期ウインドウモード変更機能が有効になっている場合はウインドウモード変更処理を行う
		if( wParam == SIZE_MAXIMIZED && WinData.UseChangeWindowModeFlag )
		{
			// もしウインドウモード変更中やウインドウ作成直後、それかウインドウモードではなかった場合は何もせずに終了
			if( WinData.ChangeWindodwFlag == FALSE &&
				WinData.WindowCreateFlag == FALSE &&
				WinData.WindowModeFlag == TRUE )
			{
				// ウインドウモードからフルスクリーンモードに移行する
				WinData.ChangeWindowModeFlag = TRUE ;
			}
		}
		else
		{
			// 最大化だった場合でまだ最大化処理を行っておらず、且つクライアント領域に画面をフィットさせる場合はウインドウのスケールを変更する
			if( wParam == SIZE_MAXIMIZED && 
				WinData.WindowMaximizeFlag == FALSE /* &&
				WinData.ScreenNotFitWindowSize == FALSE &&
				WinData.WindowSizeValid == FALSE */ )
			{
//				RECT CliRect ;
//				int Width, Height, NextWidth, NextHeight ;

				// 描画画面のサイズを取得する
//				NS_GetDrawScreenSize( &Width, &Height ) ;

				// 今まで最小化されていた場合は値を更新しない
				if( PrevWindowMinSizeFlag == FALSE )
				{
					// ウインドウのクライアント領域を取得する
					RECT PrevWindowMaximizeClientRect = WinData.WindowMaximizedClientRect ;
					GetClientRect( hWnd, &WinData.WindowMaximizedClientRect ) ;

					// ウインドウの領域を取得する
					RECT PrevWindowMaximizeRect = WinData.WindowMaximizedRect ;
					GetWindowRect( hWnd, &WinData.WindowMaximizedRect ) ;

					WinData.WindowMaximizedRect.left   -= WinData.WindowMaximizedAdjustRect.left - PrevWindowMaximizeRect.left ;
					WinData.WindowMaximizedRect.right  -= WinData.WindowMaximizedAdjustRect.left - PrevWindowMaximizeRect.left ;
					WinData.WindowMaximizedRect.top    -= WinData.WindowMaximizedAdjustRect.top  - PrevWindowMaximizeRect.top ;
					WinData.WindowMaximizedRect.bottom -= WinData.WindowMaximizedAdjustRect.top  - PrevWindowMaximizeRect.top ;

					// ４隅の内一つだけ値が異なる場合は、その要素もひとつ前の値を使用する
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

					// 一番最初の最大化の矩形が有効で、且つ別のディスプレイで最大化されたと思しき場合は
					// 一番最初の最大化の矩形を無効にする
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

					// 一番最初の最大化の場合は FirstWindowMaximizeRect にも保存する
					if( WinData.ValidFirstWindowMaximizedRect == FALSE )
					{
						WinData.ValidFirstWindowMaximizedRect = TRUE ;
						WinData.FirstWindowMaximizedRect = WinData.WindowMaximizedRect ;
					}
				}

				// WM_PAINT の処理を行う
//					{
//						HDC Dc = GetDC( hWnd ) ;
//
//						WM_PAINTProcess( Dc, &WinData.WindowMaximizedClientRect, TRUE ) ;
//
//						ReleaseDC( hWnd, Dc ) ;
//					}

				// 最大化状態フラグを立てる
				WinData.WindowMaximizeFlag = TRUE ;
			}
			WM_SIZEProcess() ;
		}

		// ツールバーが有効な場合はツールバーにも送る
		if( WinData.ToolBarUseFlag == TRUE )
		{
			SendMessageW( WinData.ToolBarHandle, WM_SIZE, wParam, lParam ) ;
		}
		break ;




	// 画面解像度変更時の処理
	case WM_DISPLAYCHANGE :
#ifndef DX_NON_GRAPHICS
//		// フルスクリーンモードで、既にメインウインドウ以外がアクティブになっていた
//		// 場合は、更に別のウインドウがアクティブになりデスクトップ画面に戻ったと判断
//		if( WinData.WindowModeFlag == FALSE && WinData.StopFlag == TRUE )
//		{
//			int Cx = LOWORD( lParam ), Cy = HIWORD( lParam ), Ccb = wParam ;
//			int Sx, Sy, Scb ;
//
//			// 元々のデスクトップ画面のサイズを得る
//			NS_GetDefaultState( &Sx , &Sy , &Scb ) ;
//
//			// もしデスクトップ画面と同じ画面モードであれば確実
//			if( Scb == Ccb && Sx == Cx && Sy == Cy )
//			{
//				// DirectX 関連オブジェクトの一時的解放処理を行う
////			DestroyGraphSystem() ;
//			}
//		}
//		else
		// ウインドウモード時のエラー終了
		if( WinData.WindowModeFlag == TRUE && WinData.ChangeWindodwFlag == FALSE )
		{
			WPARAM ColorBit = wParam ;
			int Cx = LOWORD( lParam ) , Cy = HIWORD( lParam ) ;
			int Sx , Sy , Cb;

			// 望みの画面モードを取得
			NS_GetScreenState( &Sx, &Sy, &Cb ) ;

			// もし復元スレッドが登録されていない場合はソフト終了、２４ビットカラーモードの場合も終了
			if( ColorBit != ( UINT_PTR )Cb || Sx != Cx || Sy != Cy )
			{
				if( !NS_GetValidRestoreShredPoint() || ColorBit == 24 || 
					( ColorBit != ( UINT_PTR )Cb && ColorBit == 8 ) || Sx > Cx || Sy > Cy )
				{
					DXST_ERRORLOG_ADDUTF16LE( "\xa9\x5f\x43\x51\xa2\x95\x70\x65\x4c\x30\x7b\x76\x32\x93\x55\x30\x8c\x30\x66\x30\x44\x30\x6a\x30\x44\x30\x5f\x30\x81\x30\x4b\x30\x3b\x75\x62\x97\x4c\x30\x12\xff\x14\xff\xd3\x30\xc3\x30\xc8\x30\xab\x30\xe9\x30\xfc\x30\x6b\x30\x09\x59\xf4\x66\x55\x30\x8c\x30\x5f\x30\x5f\x30\x81\x30\x42\x7d\x86\x4e\x57\x30\x7e\x30\x59\x30\x0a\x00\x00"/*@ L"復元関数が登録されていないためか画面が２４ビットカラーに変更されたため終了します\n" @*/ ) ;
					DXST_ERRORLOG_ADDUTF16LE( "\x7e\x30\x5f\x30\x6f\x30\xb9\x30\xaf\x30\xea\x30\xfc\x30\xf3\x30\x4c\x30\x0f\x5c\x55\x30\x59\x30\x4e\x30\x8b\x30\x5f\x30\x81\x30\x4b\x30\x72\x82\xd3\x30\xc3\x30\xc8\x30\x70\x65\x6e\x30\x55\x90\x44\x30\x4c\x30\x27\x59\x4d\x30\x44\x30\x5f\x30\x81\x30\x4b\x30\x42\x7d\x86\x4e\x57\x30\x7e\x30\x59\x30\x0a\x00\x00"/*@ L"またはスクリーンが小さすぎるためか色ビット数の違いが大きいためか終了します\n" @*/ ) ;
				
					// クローズフラグが倒れていたらWM_CLOSEメッセージを送る
					if( !WinData.CloseMessagePostFlag )
					{
						WinData.CloseMessagePostFlag = TRUE ;
						PostMessageW( WinData.MainWindow , WM_CLOSE, 0, 0 );
					}

					break ;
				}

#ifndef DX_NON_MOVIE
				// 再生中のムービーオブジェクトをすべて止める
				DisableMovieAll() ;

				// ムービーグラフィックの再生
				PlayMovieAll() ;
#endif

//				MessageBox( hWnd , "動作保証外の動作が発生したため終了します" , "エラー" , MB_OK ) ;
//				return DxLib_Error( DXSTRING( "ウインドウモード時にデスクトップの解像度が変更されました、終了します" ) ;
			}
		}
#endif // DX_NON_GRAPHICS

		WinData.DisplayChangeMessageFlag = TRUE ;
		break ;

	// メニューに重なっているかテスト
	case WM_NCHITTEST :
#ifndef DX_NON_GRAPHICS
		if( GSYS.Setting.ValidHardware && WinData.WindowModeFlag == FALSE )
		{
			if( HIWORD( lParam ) < 8 )
				return HTMENU ;
		}
#endif // DX_NON_GRAPHICS
		break ;

	// ウインドウのサイズや位置の変更が開始されるか、システムメニューの選択が開始されたら非アクティブと同じ扱いにする
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

	// タイトルバー上で右クリックがされた場合は非アクティブ扱いにする
	case WM_NCRBUTTONDOWN :
		if( WinData.WindowModeFlag == FALSE )
			break ;

		if( WinData.NotActive_WindowMoveOrSystemMenu == FALSE )
		{
			WinData.NotActive_WindowMoveOrSystemMenu = TRUE ;
			WM_ACTIVATEProcess( 0, 0, 0, TRUE ) ;
		}
		break ;

	// ウインドウのサイズや位置の変更が開始されるか、システムメニューの選択が終了
	case WM_EXITSIZEMOVE :
	case WM_EXITMENULOOP :
		if( WinData.NotActive_WindowMoveOrSystemMenu )
		{
			WinData.NotActive_WindowMoveOrSystemMenu = FALSE ;
			WM_ACTIVATEProcessUseStock( 1, 0, 0, TRUE ) ;
		}
		break ;

	// カーソル更新時
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
		// ボタン入力コードをセットする
		switch( message )
		{
		case WM_LBUTTONDOWN : StockMouseInputInfo( MOUSE_INPUT_LEFT   ) ; break ;
		case WM_RBUTTONDOWN : StockMouseInputInfo( MOUSE_INPUT_RIGHT  ) ; break ;
		case WM_MBUTTONDOWN : StockMouseInputInfo( MOUSE_INPUT_MIDDLE ) ; break ;
		}
#endif // DX_NON_INPUT

		// マウスを使用しない設定になっていた場合マウスカーソルを消す
		if( /*!WinData.WindowModeFlag &&*/ /*!WinData.MouseDispFlag*/ WinData.MouseDispState == FALSE )
		{
			SetCursor( NULL ) ;
			while( ShowCursor( FALSE ) > -1 ){} ;
		}
		break ;

	// 再描画メッセージ
	case WM_PAINT :
#ifndef DX_NON_GRAPHICS
		{
			PAINTSTRUCT PaintStr ;

			// WM_PAINT メッセージが来たことを記録しておく
			WinData.RecvWM_PAINTFlag = TRUE;

			if( BeginPaint( hWnd , &PaintStr ) == NULL ) break ;

			// ウインドウモードの場合のみ処理を行う
			if( WinData.WindowModeFlag == TRUE )
			{
				WinData.WM_PAINTFlag = TRUE ;

				// WM_PAINT の処理を行う
				WM_PAINTProcess( PaintStr.hdc, &PaintStr.rcPaint ) ;

				WinData.WM_PAINTFlag = FALSE ;
			}
			EndPaint( hWnd , &PaintStr ) ;
		}
#endif // DX_NON_GRAPHICS
		break ;

	// システム文字コードメッセージ
	case WM_SYSCHAR :
		{
			// 入力されたシステム文字コードを保存
			WinData.InputSysChara = (wchar_t)wParam ;

			// もしALT+ENTERがおされ、非同期ウインドウモード変更機能が有効になっていたら
			if( (wchar_t)wParam == CTRL_CODE_CR )
			{
				// ウインドウモード変更フラグを立てる
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
	// 文字コードメッセージ
	case WM_CHAR :

		// 文字をバッファにコピー
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
			return 1 ;		// 省電力モードを防止する

		if( ( wParam & 0xfff0 ) == SC_SCREENSAVE )
			return 1 ;		// スクリーンセーバーの起動を防止する
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
	// Ｆ１０メッセージ
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

	// Ｆ１２メッセージ
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
	// キー押下メッセージ
	case WM_KEYDOWN:
		{
			// バッファが一杯の場合はなにもしない
			if( ( CharBuf.EdPoint + 1 == CharBuf.StPoint ) ||
				( CharBuf.StPoint == 0 && CharBuf.EdPoint == CHARBUFFER_SIZE ) ) break ;

			// コントロール文字コードに対応するキーが
			// 押されていたらバッファに格納する
			{
				int   VKey  = ( int )wParam ;
				char *CCode = CtrlCode[ 0 ] ;

				for( CCode = ( char * )CtrlCode ; CCode[ 0 ] != 0 && CCode[ 0 ] != VKey ; CCode += 2 ){}
				if( CCode[ 0 ] != 0 )
				{
					// バッファに文字コードを代入
					StockInputChar_WCHAR_T( ( wchar_t )*( CCode + 1 ) ) ;
				}
			}
		}
		break ;
#endif // DX_NON_INPUTSTRING

	// ホイールの移動量取得
	case WM_MOUSEWHEEL :
		{
			int MoveParamZ = ( SHORT )HIWORD( wParam ) ;

			// 今までの移動と方向が異なる場合は０に仕切りなおす
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

	// ホイールの横移動量取得
	case WM_MOUSEHWHEEL :
		{
			int MoveParamHZ = ( SHORT )HIWORD( wParam ) ;

			// 今までの移動と方向が異なる場合は０に仕切りなおす
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

	// タッチ
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

	// ウインドウ移動時処理
	case WM_MOVE :
		GetWindowRect( hWnd, &WinData.WindowEdgeRect ) ;
		if( WinData.WindowModeFlag && !WinData.ChangeWindodwFlag )
		{
			WM_MOVEProcess( lParam ) ;
		}
		break ;

	// ウインドウ移動の抑制
	case WM_MOVING :
		GetWindowRect( hWnd, &WinData.WindowEdgeRect ) ;
		if( WinData.WindowModeFlag == FALSE )
		{
			RECT rect ;
			GetWindowRect( WinData.MainWindow , &rect ) ;
			*( RECT *)lParam = rect ;
		}
		break ;

	// アクティブ状態変化時
	case WM_ACTIVATEAPP :
		// 非アクティブになった場合のみ WM_ACTIVATE として処理する
		if( wParam == 0 ) goto ACTIVATELABEL ;
		break ;

	case WM_ACTIVATE :
ACTIVATELABEL:
#ifndef DX_NON_ASYNCLOAD
		/*
		// 非同期読み込みの途中の場合は、非同期読み込みが終了するまで待つ
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

	// ウインドウクローズ時
	case WM_CLOSE :
		// ユーザー提供のウインドウだったら何もしない
		if( WinData.UserWindowFlag == FALSE )
		{
			DXST_ERRORLOG_ADDUTF16LE( "\xa6\x30\xa4\x30\xf3\x30\xc9\x30\xa6\x30\x92\x30\x89\x95\x58\x30\x88\x30\x46\x30\x68\x30\x57\x30\x66\x30\x44\x30\x7e\x30\x59\x30\x0a\x00\x00"/*@ L"ウインドウを閉じようとしています\n" @*/ ) ;

			// WM_DESTROYメッセージを生成
			if( WinData.NonUserCloseEnableFlag == FALSE || WinData.AltF4_EndFlag == 1 )
				DestroyWindow( WinData.MainWindow ) ;

			// 強制終了か判定
			if( WinData.AltF4_EndFlag == 0 ) WinData.AltF4_EndFlag = 4 ;

			return 0 ;
		}
		break ;

	// ウインドウ破棄時
	case WM_DESTROY :
		// ユーザー提供のウインドウだったら何もしない
		if( WinData.UserWindowFlag == FALSE )
		{
			DXST_ERRORLOG_ADDUTF16LE( "\xa6\x30\xa4\x30\xf3\x30\xc9\x30\xa6\x30\x4c\x30\x34\x78\xc4\x68\x55\x30\x8c\x30\x88\x30\x46\x30\x68\x30\x57\x30\x66\x30\x44\x30\x7e\x30\x59\x30\x0a\x00\x00"/*@ L"ウインドウが破棄されようとしています\n" @*/ ) ;

			// WM_DESTROYメッセージを受けた証拠を残す
			WinData.DestroyMessageCatchFlag = TRUE ;

			// ソフトの終了メッセージを送る
			if( WinData.NonDxLibEndPostQuitMessageFlag == FALSE )
				PostQuitMessage( 0 ) ;
			WinData.QuitMessageFlag = TRUE ;
			DxLib_EndRequest() ;

			DXST_ERRORLOG_ADDUTF16LE( "\xbd\x30\xd5\x30\xc8\x30\x92\x30\x42\x7d\x86\x4e\x59\x30\x8b\x30\x96\x6e\x99\x50\x4c\x30\x74\x65\x44\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"ソフトを終了する準備が整いました\n" @*/ ) ;
		}
		break ;

	case WM_QUIT :
		break ;

#ifndef DX_NON_SOUND
	// ＭＩＤＩ演奏終了時
	case MM_MCINOTIFY:	
		MidiCallBackProcess() ;					// ＭＩＤＩ演奏終了時の処理
		break;
#endif // DX_NON_SOUND

#ifndef DX_NON_NETWORK
	// ＷｉｎＳｏｃｋメッセージ
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

// 非アクティブかどうかをチェックする
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

// アクティブになるまで何もしない
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

// WM_QUIT が発行されているかどうかを取得する
extern int GetQuitMessageFlag( void )
{
	return WinData.QuitMessageFlag;
}




// ウインドウ関係情報取得関数

// ウインドウのクライアント領域を取得する
extern int NS_GetWindowCRect( RECT *RectBuf )
{
	int H;

	H = GetToolBarHeight();
	*RectBuf = WinData.WindowRect ;
	RectBuf->top    += H ;
	RectBuf->bottom += H ;
	
	return 0 ;
}

// ウインドウのアクティブフラグを取得
extern int NS_GetWindowActiveFlag( void )
{
	return WinData.ActiveFlag ;
}

// メインウインドウが最小化されているかどうかを取得する( 戻り値  TRUE:最小化されている  FALSE:最小化されていない )
extern int NS_GetWindowMinSizeFlag( void )
{
	return WinData.WindowMinSizeFlag ;
}

// メインウインドウが最大化されているかどうかを取得する( 戻り値  TRUE:最大化されている  FALSE:最大化されていない )
extern int NS_GetWindowMaxSizeFlag( void )
{
	return WinData.WindowMaximizeFlag ;
}

// メインウインドウのハンドルを取得する
extern HWND NS_GetMainWindowHandle( void )
{
	return WinData.MainWindow ;
}

// ウインドウモードで起動しているか、のフラグを取得する
extern int NS_GetWindowModeFlag( void )
{
	return WinData.WindowModeFlag ;
}

// 起動時のデスクトップの画面モードを取得する
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

	// 終了
	return 0 ;
}

// ソフトがアクティブかどうかを取得する
extern int NS_GetActiveFlag( void )
{
	return WinData.ActiveFlag ;
}

// 非アクティブになり、処理が一時停止していたかどうかを取得する(引数 ResetFlag=TRUE:状態をリセット FALSE:状態をリセットしない    戻り値: 0=一時停止はしていない  1=一時停止していた )
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

// マウスを表示するかどうかのフラグを取得する
extern int NS_GetMouseDispFlag( void )
{
	return WinData.MouseDispFlag ;
}

// ウインドウがアクティブではない状態でも処理を続行するか、フラグを取得する
extern int NS_GetAlwaysRunFlag( void )
{
	return WinData.NonActiveRunFlag ;
}

// ＤＸライブラリと DirectX のバージョンと Windows のバージョンを得る
extern int NS__GetSystemInfo( int *DxLibVer, int *DirectXVer, int *WindowsVer )
{
	if( DxLibVer ) *DxLibVer = DXLIB_VERSION ;
	if( DirectXVer ) *DirectXVer = WinData.DirectXVersion ;
	if( WindowsVer ) *WindowsVer = WinData.WindowsVersion ;

	// 終了
	return 0 ;
}

// ＰＣの情報を得る
extern int NS_GetPcInfo(
	TCHAR	*OSString,
	TCHAR	*DirectXString,
	TCHAR	*CPUString,
	int		*CPUSpeed/*単位MHz*/,
	double	*FreeMemorySize/*単位MByte*/,
	double	*TotalMemorySize,
	TCHAR	*VideoDriverFileName,
	TCHAR	*VideoDriverString,
	double	*FreeVideoMemorySize/*単位MByte*/,
	double	*TotalVideoMemorySize
)
{
#ifdef UNICODE
	return GetPcInfo_WCHAR_T(
		OSString,
		DirectXString,
		CPUString,
		CPUSpeed/*単位MHz*/,
		FreeMemorySize/*単位MByte*/,
		TotalMemorySize,
		VideoDriverFileName,
		VideoDriverString,
		FreeVideoMemorySize/*単位MByte*/,
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
		CPUSpeed/*単位MHz*/,
		FreeMemorySize/*単位MByte*/,
		TotalMemorySize,
		VideoDriverFileNameBuffer,
		VideoDriverStringBuffer,
		FreeVideoMemorySize/*単位MByte*/,
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

// ＰＣの情報を得る
extern int GetPcInfo_WCHAR_T(
	wchar_t	*OSString,
	wchar_t	*DirectXString,
	wchar_t	*CPUString,
	int		*CPUSpeed/*単位MHz*/,
	double	*FreeMemorySize/*単位MByte*/,
	double	*TotalMemorySize,
	wchar_t	*VideoDriverFileName,
	wchar_t	*VideoDriverString,
	double	*FreeVideoMemorySize/*単位MByte*/,
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

// ＭＭＸが使えるかどうかの情報を得る
extern int NS_GetUseMMXFlag( void ) 
{
	return WinData.UseMMXFlag ;
}

// ＳＳＥが使えるかどうかの情報を得る
extern int NS_GetUseSSEFlag( void )
{
	return WinData.UseSSEFlag ;
}

// ＳＳＥ２が使えるかどうかの情報を得る
extern int NS_GetUseSSE2Flag( void )
{
	return WinData.UseSSE2Flag ;
}

// ウインドウを閉じようとしているかの情報を得る
extern int NS_GetWindowCloseFlag( void )
{
	return WinData.CloseMessagePostFlag ;
}

// ソフトのインスタンスを取得する
extern HINSTANCE NS_GetTaskInstance( void )
{
	return WinData.Instance ;
}

// リージョンを使っているかどうかを取得する
extern int NS_GetUseWindowRgnFlag( void )
{
	return WinData.WindowRgn != NULL ;
}

// ウインドウのサイズを変更できるかどうかのフラグを取得する
extern int NS_GetWindowSizeChangeEnableFlag( int *FitScreen )
{
	if( FitScreen ) *FitScreen = WinData.ScreenNotFitWindowSize == TRUE ? FALSE : TRUE ;
	return WinData.WindowSizeChangeEnable ;
}

// 描画画面のサイズに対するウインドウサイズの比率を取得する
extern double NS_GetWindowSizeExtendRate( double *ExRateX, double *ExRateY )
{
	if( WinData.WindowSizeExRateX <= 0.0 )
		WinData.WindowSizeExRateX = 1.0 ;

	if( WinData.WindowSizeExRateY <= 0.0 )
		WinData.WindowSizeExRateY = 1.0 ;

	// ウインドウモードかどうかで処理を分岐
	if( WinData.WindowModeFlag )
	{
#ifndef DX_NON_GRAPHICS
		// 最大化状態の場合かどうかで処理を分岐
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

			// 画面のサイズを取得
			NS_GetDrawScreenSize( &Width, &Height ) ;

			// 最大化状態のクライアント領域のサイズを取得する
			MaxCWidth  = WinData.WindowMaximizedClientRect.right  - WinData.WindowMaximizedClientRect.left ;
			MaxCHeight = WinData.WindowMaximizedClientRect.bottom - WinData.WindowMaximizedClientRect.top ;

			// 計算誤差対応のため１ドットだけ小さいサイズにする
			MaxCWidth  -= 1 ;
			MaxCHeight -= 1 ;

			// 倍率を算出する
			MaxCExRateX = ( double )MaxCWidth  / Width ;
			MaxCExRateY = ( double )MaxCHeight / Height ;

			// 倍率の小さいほうに合わせる
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

// ウインドウモードのウインドウのクライアント領域のサイズを取得する
extern int NS_GetWindowSize( int *Width, int *Height )
{
	RECT Rect ;

	GetClientRect( GetDisplayWindowHandle(), &Rect ) ;
	if( Width  ) *Width  = Rect.right  - Rect.left ;
	if( Height ) *Height = Rect.bottom - Rect.top  ;

	// 終了
	return 0 ;
}

// ウインドウモードのウインドウの位置を取得する( 枠も含めた左上座標 )
extern int NS_GetWindowPosition( int *x, int *y )
{
	RECT Rect ;

	GetWindowRect( WinData.MainWindow, &Rect ) ;
	if( x ) *x = Rect.left ;
	if( y ) *y = Rect.top ;

	// 終了
	return 0 ;
}


// ウインドウの閉じるボタンが押されたかどうかを取得する
extern int NS_GetWindowUserCloseFlag( int StateResetFlag )
{
	int Result ;

	Result = WinData.AltF4_EndFlag == 4 ? TRUE : FALSE ;

	// 状態をリセットしろフラグが立っていたらフラグをリセットする
	if( StateResetFlag == TRUE && WinData.AltF4_EndFlag == 4 && WinData.NonUserCloseEnableFlag == TRUE )
	{
		WinData.AltF4_EndFlag = FALSE ;
	}

	return Result ;
}

// WM_PAINT メッセージが来たかどうかを取得する
// (戻り値   TRUE:WM_PAINTメッセージが来た(一度取得すると以後、再び WM_PAINTメッセージが来るまで FALSE が返ってくるようになる)
//          FALSE:WM_PAINT メッセージは来ていない)
extern int NS_GetPaintMessageFlag( void )
{
	int Result;

	Result = WinData.RecvWM_PAINTFlag;
	WinData.RecvWM_PAINTFlag = FALSE;

	return Result;
}

// パフォーマンスカウンタが有効かどうかを取得する(戻り値  TRUE:有効  FALSE:無効)
extern int NS_GetValidHiPerformanceCounter( void )
{
	return WinData.PerformanceTimerFlag;
}

// 入力されたシステム文字を取得する
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

// 入力されたシステム文字を取得する
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












// Aero の有効、無効設定
extern int SetEnableAero( int Flag )
{
	// フラグが同じだったら何もしない
	if( WinData.AeroDisableFlag == !Flag )
		return 0 ;

	if( WinAPIData.DF_DwmEnableComposition )
	{
		WinAPIData.DF_DwmEnableComposition( ( UINT )Flag ) ;
	}

	// フラグを保存
	WinData.AeroDisableFlag = !Flag ;

	// 終了
	return 0 ;
}







// 設定関係関数

// ウインドウモードで起動するかをセット
extern int	SetWindowModeFlag( int Flag )
{
	// フラグを保存
	WinData.WindowModeFlag = Flag ;

	if( Flag ) 
	{
		DXST_ERRORLOG_ADDUTF16LE( "\xa6\x30\xa4\x30\xf3\x30\xc9\x30\xa6\x30\xe2\x30\xfc\x30\xc9\x30\xd5\x30\xe9\x30\xb0\x30\x4c\x30\xcb\x7a\x66\x30\x89\x30\x8c\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"ウインドウモードフラグが立てられました\n" @*/ ) ;

		// メニューが存在する時はメニューをセットする
		if( WinData.MenuUseFlag == TRUE ) 
		{
			SetMenu( WinData.MainWindow, WinData.Menu ) ;
		}
	}
	else
	{
		DXST_ERRORLOG_ADDUTF16LE( "\xa6\x30\xa4\x30\xf3\x30\xc9\x30\xa6\x30\xe2\x30\xfc\x30\xc9\x30\xd5\x30\xe9\x30\xb0\x30\x4c\x30\x12\x50\x55\x30\x8c\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"ウインドウモードフラグが倒されました\n" @*/ ) ;

		// メニューが存在する時はメニューを外す
		if( WinData.MenuUseFlag == TRUE ) 
		{
			SetMenu( WinData.MainWindow, NULL ) ;
		}
	}

	// 終了
	return 0 ;
}

// ウインドウのスタイルをセットする
extern int SetWindowStyle( int CenterPosition )
{
	RECT Rect ;
	RECT PrevWindowRect ;
	int WindowSizeX, WindowSizeY ;
	int ClientRectWidth, ClientRectHeight ;

	if( WinData.MainWindow == NULL || WinData.UserWindowFlag == TRUE ) return 0 ;

	// 現在のウインドウ領域を取得
	GetWindowRect( WinData.MainWindow, &PrevWindowRect ) ;

	// クライアント領域のサイズを保存
	GetClientRect( WinData.MainWindow, &Rect ) ;
	ClientRectWidth  = Rect.right  - Rect.left ;
	ClientRectHeight = Rect.bottom - Rect.top ;

	// ユーザー提供のウインドウだったらパラメータ取得以外は何もしない
	if( WinData.UserWindowFlag == TRUE )
	{
		// ウインドウのクライアント領域を保存する
		GetClientRect( WinData.MainWindow, &WinData.WindowRect )  ;
		ClientToScreen( WinData.MainWindow, ( LPPOINT )&WinData.WindowRect ) ;
		ClientToScreen( WinData.MainWindow, ( LPPOINT )&WinData.WindowRect + 1 ) ;

		// ウインドウスタイルを変更
		if( WinData.WindowModeFlag == TRUE )	NS_SetMouseDispFlag( TRUE ) ;
		else									NS_SetMouseDispFlag( FALSE ) ;
	}
	else
	{
		// 描画領域のサイズを取得
		GetMainWindowSize( &WindowSizeX, &WindowSizeY ) ;

		// ウインドウスタイルを変更
		if( WinData.WindowModeFlag == TRUE )
		{
			LONG AddStyle, AddExStyle ;
			double ExtendRateX, ExtendRateY ;

			// ウインドウモードの場合

			DXST_ERRORLOG_ADDUTF16LE( "\xa6\x30\xa4\x30\xf3\x30\xc9\x30\xa6\x30\xb9\x30\xbf\x30\xa4\x30\xeb\x30\x92\x30\xa6\x30\xa4\x30\xf3\x30\xc9\x30\xa6\x30\xe2\x30\xfc\x30\xc9\x30\x28\x75\x6b\x30\x09\x59\xf4\x66\x57\x30\x7e\x30\x59\x30\x2e\x00\x2e\x00\x2e\x00\x20\x00\x00"/*@ L"ウインドウスタイルをウインドウモード用に変更します... " @*/ ) ;

			AddExStyle = 0 ;
			//if( WinData.NotWindowVisibleFlag == TRUE ) AddExStyle |= WS_EX_TRANSPARENT ;

			// バックバッファの透過色の部分を透過させるフラグが立っているか、
			// UpdateLayerdWindow を使用するフラグが経っている場合は WS_EX_LAYERED を追加する
			if( WinData.BackBufferTransColorFlag == TRUE ||
				WinData.UseUpdateLayerdWindowFlag == TRUE ) AddExStyle |= WS_EX_LAYERED ;

			AddStyle = 0 ;
			// 非同期ウインドウモード変更機能が有効になっているときは WS_MAXIMIZEBOX を追加する
			if( WinData.UseChangeWindowModeFlag == TRUE ) AddStyle |= WS_MAXIMIZEBOX ;

			// 表示フラグが立っている場合は WS_VISIBLE を追加する
			if( WinData.VisibleFlag == TRUE ) AddStyle |= WS_VISIBLE ;

			// ウインドウサイズが変更できるようになっている場合は WS_THICKFRAME を追加する
			if( WinData.WindowSizeChangeEnable == TRUE ) AddStyle |= WS_THICKFRAME ;

			// スタイルの変更
			SetWindowLongW( WinData.MainWindow , GWL_EXSTYLE , ( LONG )( WExStyle_WindowModeTable[ WinData.WindowStyle ] + AddExStyle ) ) ;
			SetWindowLongW( WinData.MainWindow , GWL_STYLE   , ( LONG )( WStyle_WindowModeTable[ WinData.WindowStyle ]   + AddStyle   ) ) ;

			// クライアント領域の算出
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

			// 位置を変更
			SETRECT( Rect, 0, 0, WindowSizeX, WindowSizeY ) ;
			AdjustWindowRectEx( &Rect,
								WStyle_WindowModeTable[WinData.WindowStyle] + AddStyle, FALSE,
								WExStyle_WindowModeTable[WinData.WindowStyle] + AddExStyle );

			// 設定領域をちょいと細工
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

				// 中心に持ってくる
				Rect.left   += ( DesktopW - WindowSizeX ) / 2 + DesktopX ;
				Rect.top    += ( DesktopH - WindowSizeY ) / 2 + DesktopY ;
				Rect.right  += ( DesktopW - WindowSizeX ) / 2 + DesktopX ;
				Rect.bottom += ( DesktopH - WindowSizeY ) / 2 + DesktopY ;
			}
			else
			{
				// 元の位置にセットする
				Rect.left   = PrevWindowRect.left ;
				Rect.top    = PrevWindowRect.top ;
				Rect.right  = Rect.left + WindowSizeX ;
				Rect.bottom = Rect.top  + WindowSizeY ;
			}

			// ウインドウの位置とサイズを変更
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

			// ウインドウのクライアント領域を保存する
			GetClientRect( WinData.MainWindow , &WinData.WindowRect )  ;
			ClientToScreen( WinData.MainWindow , ( LPPOINT )&WinData.WindowRect ) ;
			ClientToScreen( WinData.MainWindow , ( LPPOINT )&WinData.WindowRect + 1 ) ;

			// 補正
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
			DXST_ERRORLOG_ADDUTF16LE( "\x8c\x5b\x86\x4e\x0a\x00\x00"/*@ L"完了\n" @*/ ) ;
		}
		else
		{
			DXST_ERRORLOG_ADDUTF16LE( "\xa6\x30\xa4\x30\xf3\x30\xc9\x30\xa6\x30\xb9\x30\xbf\x30\xa4\x30\xeb\x30\x92\x30\xd5\x30\xeb\x30\xb9\x30\xaf\x30\xea\x30\xfc\x30\xf3\x30\xe2\x30\xfc\x30\xc9\x30\x28\x75\x6b\x30\x09\x59\xf4\x66\x57\x30\x7e\x30\x59\x30\x2e\x00\x2e\x00\x2e\x00\x20\x00\x00"/*@ L"ウインドウスタイルをフルスクリーンモード用に変更します... " @*/ ) ;

			// フルスクリーンモードの場合
			SetWindowLongW( WinData.MainWindow, GWL_EXSTYLE, ( LONG )( WExStyle_FullScreenModeTable[WinData.WindowStyle] ) ) ;
			SetWindowLongW( WinData.MainWindow, GWL_STYLE  , ( LONG )( WStyle_FullScreenModeTable[WinData.WindowStyle]   ) ) ;

			// ウインドウ矩形を変更
			SETRECT( WinData.WindowRect, 0, 0, WindowSizeX, WindowSizeY ) ;

			// ウインドウを表示
			ShowWindow( WinData.MainWindow , SW_SHOW ) ;
			UpdateWindow( WinData.MainWindow ) ;

			// Ｚオーダーの維持
			SetWindowPos( WinData.MainWindow, HWND_TOPMOST, 0, 0, WindowSizeX, WindowSizeY, /*SWP_NOSIZE | SWP_NOMOVE |*/ SWP_NOREDRAW ) ; 

			NS_SetMouseDispFlag( FALSE ) ;
			DXST_ERRORLOG_ADDUTF16LE( "\x8c\x5b\x86\x4e\x0a\x00\x00"/*@ L"完了\n" @*/ ) ;
		}

		SetActiveWindow( WinData.MainWindow ) ;
	}

	// マウスのセット信号を出す
	PostMessageW( WinData.MainWindow , WM_SETCURSOR , ( WPARAM )WinData.MainWindow , 0 ) ;

	// 終了
	return 0 ;
}

// 情報取得警官数

// ＰＣ情報構造体のアドレスを得る
extern PCINFO *GetPcInfoStructP( void ) 
{
	return &WinData.PcInfo ;
}


// ウインドウモードを変更する
extern int NS_ChangeWindowMode( int Flag )
{
#ifndef DX_NON_GRAPHICS
	int Ret ;

	DXST_ERRORLOG_ADDUTF16LE( "\x43\x00\x68\x00\x61\x00\x6e\x00\x67\x00\x65\x00\x57\x00\x69\x00\x6e\x00\x64\x00\x6f\x00\x77\x00\x4d\x00\x6f\x00\x64\x00\x65\x00\x9f\x5b\x4c\x88\x20\x00\x0a\x00\x00"/*@ L"ChangeWindowMode実行 \n" @*/ ) ;

	// 今までと同じモードだった場合はなにもせず終了
	if( Flag == WinData.WindowModeFlag ) return 0 ;

	WinData.VisibleFlag = TRUE ;

	// WM_DISPLAYCHANGE メッセージが来たかフラグを倒す
	WinData.DisplayChangeMessageFlag = FALSE ;

	// ウインドウモードフラグを変更する
	if( SetWindowModeFlag( Flag ) == -1 ) return -1 ;

	// まだウインドウさえ作成されていないか、ユーザーが作成したウインドウを使用している場合はここで終了
	if( WinData.MainWindow == NULL || WinData.UserWindowFlag == TRUE ) return 0 ;

	// ウインドウモード変更中フラグを立てる
	WinData.ChangeWindodwFlag = TRUE ;

	// ウインドウスタイルを変更する
	SetWindowStyle() ;

	// 画面モードを変更する
	Ret = Graphics_Screen_ChangeMode( -1, -1, -1, TRUE, -1 ) ;

	// ウインドウモード変更中フラグを倒す
	WinData.ChangeWindodwFlag = FALSE ;

	// フルスクリーンになった直後はメニューを表示していない状態にする
	if( Flag == FALSE )
	{
		WinData.MousePosInMenuBarFlag = FALSE ;
	}

	return Ret ;
#else // DX_NON_GRAPHICS
	return -1 ;
#endif // DX_NON_GRAPHICS
}

// ＤＸライブラリの文字列処理で前提とする文字列セットを設定する
extern int NS_SetUseCharSet( int CharSet /* = DX_CHARSET_SHFTJIS 等 */ )
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

	// 終了
	return 0 ;
}

// アクティブウインドウが他のソフトに移っている際に表示する画像のロード(NULL で解除)
static int LoadPauseGraphToBase( const wchar_t *FileName, const void *MemImage, int MemImageSize )
{
#ifndef DX_NON_GRAPHICS
	BASEIMAGE RgbBaseImage ;

	// 画像データの情報がなかったら何もせず終了
	if( FileName == NULL && MemImage == NULL ) return 0 ;

	// 画像のロード
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

	// すでにグラフィックがある場合は破棄
	if( WinData.PauseGraph.GraphData != NULL )
	{
		NS_ReleaseGraphImage( &WinData.PauseGraph ) ;
		NS_DeleteGraph( WinData.PauseGraphHandle ) ;
	}

	// 新しい画像データのセット
	WinData.PauseGraph = RgbBaseImage ;

	// グラフィックハンドルを作成する
	WinData.PauseGraphHandle = Graphics_Image_CreateGraphFromGraphImageBase( &RgbBaseImage, NULL, TRUE, FALSE ) ;
/*
	// すでにグラフィックがある場合は破棄
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

	// 終了
	return 0 ;
#else // DX_NON_GRAPHICS
	return -1 ;
#endif // DX_NON_GRAPHICS
}

// アクティブウインドウが他のソフトに移っている際に表示する画像のロード(NULL で解除)
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

// アクティブウインドウが他のソフトに移っている際に表示する画像のロード(NULL で解除)
extern int LoadPauseGraph_WCHAR_T( const wchar_t *FileName )
{
	return LoadPauseGraphToBase( FileName, NULL, 0 ) ;
}

// アクティブウインドウが他のソフトに移っている際に表示する画像のロード(NULL で解除)
extern int NS_LoadPauseGraphFromMem( const void *MemImage, int MemImageSize )
{
	return LoadPauseGraphToBase( NULL, MemImage, MemImageSize ) ;
}

// ウインドウのアクティブ状態に変化があったときに呼ばれるコールバック関数をセットする( NULL をセットすると呼ばれなくなる )
extern int NS_SetActiveStateChangeCallBackFunction( int (*CallBackFunction)( int ActiveState, void *UserData ), void *UserData )
{
	// ポインタを保存
	WinData.ActiveStateChangeCallBackFunction = CallBackFunction ;
	WinData.ActiveStateChangeCallBackFunctionData = UserData ;

	// 終了
	return 0 ;
}

// メインウインドウのウインドウテキストを変更する
extern int NS_SetWindowText( const TCHAR *WindowText )
{
	return NS_SetMainWindowText( WindowText ) ;
}

// メインウインドウのウインドウテキストを変更する
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

// メインウインドウのウインドウテキストを変更する
extern int SetMainWindowText_WCHAR_T( const wchar_t *WindowText )
{
	// テキストの保存
	_WCSCPY_S( WinData.WindowText, sizeof( WinData.WindowText ), WindowText ) ;

	// WindowText は有効、のフラグを立てる
	WinData.EnableWindowText = TRUE ;

	// メインウインドウが作られていない場合は保存のみ行う
	if( WinData.MainWindow )
	{
		// メインウインドウテキストの変更
		::SetWindowTextW( WinData.MainWindow , WinData.WindowText ) ;
	}

	// 終了
	return 0 ;
}

// メインウインドウのクラス名を設定する
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

// メインウインドウのクラス名を設定する
extern int SetMainWindowClassName_WCHAR_T( const wchar_t *ClassName )
{
	// メインウインドウが既に作成されている場合は設定不可
	if( WinData.MainWindow != NULL )
	{
		return -1 ;
	}

	// テキストの保存
	_WCSCPY_S( WinData.ClassName, sizeof( WinData.ClassName ), ClassName ) ;

	// 終了
	return 0 ;
}

// ウインドウがアクティブではない状態でも処理を続行するか、フラグをセットする
extern int NS_SetAlwaysRunFlag( int Flag )
{
	// フラグをセット
	WinData.NonActiveRunFlag = Flag ;
	
	// 終了
	return 0 ;
}

// 使用するアイコンのＩＤをセットする
extern int NS_SetWindowIconID( int ID )
{
	WinData.IconID = ID ;

	// もし既にウインドウが作成されていたら、アイコンを変更する
	if( WinData.MainWindow != NULL )
	{
#ifdef _WIN64
		SetClassLongPtrW( WinData.MainWindow, GCLP_HICON, ( LONG_PTR )LoadIconW( WinData.Instance , ( WinData.IconID == 0 ) ? ( LPWSTR )IDI_APPLICATION : MAKEINTRESOURCEW( WinData.IconID ) ) ) ;
#else
		SetClassLongW(    WinData.MainWindow,  GCL_HICON, ( LONG_PTR )LoadIconW( WinData.Instance , ( WinData.IconID == 0 ) ? ( LPWSTR )IDI_APPLICATION : MAKEINTRESOURCEW( WinData.IconID ) ) ) ;
#endif
    }
	
	// 終了
	return 0 ;
}

// 使用するアイコンのハンドルをセットする
extern int NS_SetWindowIconHandle( HICON Icon )
{
	WinData.IconHandle = Icon ;

	// もし既にウインドウが作成されていたら、アイコンを変更する
	if( WinData.MainWindow != NULL )
	{
#ifdef _WIN64
		SetClassLongPtrW( WinData.MainWindow, GCLP_HICON, ( LONG_PTR )Icon ) ;
#else
		SetClassLongW( WinData.MainWindow, GCL_HICON, ( LONG_PTR )Icon ) ;
#endif
    }
	
	// 終了
	return 0 ;
}

// 最大化ボタンやALT+ENTERキーによる非同期なウインドウモードの変更の機能の設定を行う
extern int NS_SetUseASyncChangeWindowModeFunction( int Flag, void (*CallBackFunction)(void*), void *Data )
{
	// フラグのセット
	WinData.UseChangeWindowModeFlag = Flag ;

	// フラグに従ったウインドウのスタイルを設定する
	if( WinData.WindowModeFlag )
		SetWindowStyle() ;

	// コールバック関数の保存
	WinData.ChangeWindowModeCallBackFunction = CallBackFunction ;

	// コールバック関数に渡すデータの保存
	WinData.ChangeWindowModeCallBackFunctionData = Data ;

	// 終了
	return 0 ;
}

// ウインドウのスタイルを変更する
extern int NS_SetWindowStyleMode( int Mode )
{
	if( Mode < 0 || Mode >= WSTYLE_NUM ) return -1 ; 

	// スタイル値の変更
	WinData.WindowStyle = Mode ;

	// ウインドウの属性を変更
	if( WinData.WindowModeFlag ) SetWindowStyle() ;

	// ウインドウの再描画
	UpdateWindow( WinData.MainWindow ) ;

	// 終了
	return 0 ;
}

// メインウインドウの奥行き位置を変更する
extern int NS_SetWindowZOrder( int ZType )
{
	if( ZType < 0 || ZType > DX_WIN_ZTYPE_TOPMOST ) return -1 ; 

	// 奥行き位置タイプの保存
	WinData.WindowZType = ZType ;

	// ウインドウの属性を再設定
	if( WinData.WindowModeFlag )
	{
		SetWindowStyle( FALSE ) ;
	}

	// ウインドウの再描画
	UpdateWindow( WinData.MainWindow ) ;

	// 終了
	return 0 ;
}

// ウインドウのサイズを変更できるかどうかのフラグをセットする
// NotFitScreen:ウインドウのクライアント領域に画面をフィットさせる(拡大させる)かどうか  TRUE:フィットさせる  FALSE:フィットさせない
extern int NS_SetWindowSizeChangeEnableFlag( int Flag, int FitScreen )
{
	int NotFitWindowSize ;

	NotFitWindowSize = FitScreen == TRUE ? FALSE : TRUE ;

	// フラグが同じ場合は何もしない
	if( WinData.WindowSizeChangeEnable == Flag &&
		WinData.ScreenNotFitWindowSize == NotFitWindowSize ) return 0 ;

	// フラグを保存
	WinData.WindowSizeChangeEnable = Flag ;
	WinData.ScreenNotFitWindowSize = NotFitWindowSize ;

	// ウインドウにフィットさせる場合は SetWindowSize ではウインドウの拡大率が変化するようにした
	/*
	// ウインドウにフィットさせる場合は SetWindowSize の設定は無効にする
	if( FitScreen == TRUE )
	{
		WinData.WindowSizeValid = FALSE ;
	}
	*/

	// ウインドウモードの場合のみウインドウスタイルを更新
	if( WinData.WindowModeFlag == TRUE )
		SetWindowStyle() ;

	// 終了
	return 0 ;
}

// 描画画面のサイズに対するウインドウサイズの比率を設定する
extern int NS_SetWindowSizeExtendRate( double ExRateX, double ExRateY )
{
	// ExRateY がマイナスの値の場合は ExRateX の値を ExRateY でも使用する
	if( ExRateY <= 0.0 ) ExRateY = ExRateX ;

	// 今までと同じ場合は何もしない
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

	// SetWindowSize の設定は無効にする
	WinData.WindowSizeValid = FALSE ;

	// ウインドウモードの場合は新しい比率を反映する
	if( WinData.WindowModeFlag == TRUE )
	{
		SetWindowStyle() ;
	}

	// 終了
	return 0 ;
}

// ウインドウモード時のウインドウのクライアント領域のサイズを設定する
extern int NS_SetWindowSize( int Width, int Height )
{
#ifndef DX_NON_GRAPHICS
	// ウインドウが作成済みでウインドウにフィットさせる設定の場合は拡大率を変更する
	if( WinData.MainWindow != NULL && WinData.ScreenNotFitWindowSize == FALSE )
	{
		double ExtendRateX ;
		double ExtendRateY ;
		int ScreenSizeX ;
		int ScreenSizeY ;

		// 描画領域のサイズを取得
		NS_GetDrawScreenSize( &ScreenSizeX , &ScreenSizeY ) ;

		// 拡大率を指定する
		ExtendRateX = ( double )Width  / ScreenSizeX ;
		ExtendRateY = ( double )Height / ScreenSizeY ;
		NS_SetWindowSizeExtendRate( ExtendRateX, ExtendRateY ) ;
	}
	else
#endif // DX_NON_GRAPHICS
	{
		// それ以外の場合はウインドウサイズを変更する
		WinData.WindowWidth = Width ;
		WinData.WindowHeight = Height ;
		WinData.WindowSizeValid = TRUE ;

		// 反映させる
		WM_SIZEProcess() ;
	}

	// 終了
	return 0 ;
}

// メインウインドウのクライアント領域の最大サイズを設定する( SetWindowSizeChangeEnableFlag の第一引数を TRUE で呼び出して、ウインドウのサイズが変更できる状態でのみ使用されるパラメータです )
extern int NS_SetWindowMaxSize( int MaxWidth, int MaxHeight )
{
	WinData.WindowMaxWidth = MaxWidth ;
	WinData.WindowMaxHeight = MaxHeight ;
	WinData.WindowMaxSizeValid = TRUE ;

	// 終了
	return 0 ;
}

// メインウインドウのクライアント領域の最小サイズを設定する( SetWindowSizeChangeEnableFlag の第一引数を TRUE で呼び出して、ウインドウのサイズが変更できる状態でのみ使用されるパラメータです )
extern int NS_SetWindowMinSize( int MinWidth, int MinHeight )
{
	WinData.WindowMinWidth = MinWidth ;
	WinData.WindowMinHeight = MinHeight ;
	WinData.WindowMinSizeValid = TRUE ;

	// 終了
	return 0 ;
}

// ウインドウモードのウインドウの位置を設定する( 枠も含めた左上座標 )
extern int NS_SetWindowPosition( int x, int y )
{
	WinData.WindowX = x ;
	WinData.WindowY = y ;
	WinData.WindowPosValid = TRUE ;

	// 反映させる
	WM_SIZEProcess() ;

	// 終了
	return 0 ;
}
/*
// _KBDLLHOOKSTRUCT 構造体の定義
typedef struct tag_KBDLLHOOKSTRUCT
{
    DWORD   vkCode;
    DWORD   scanCode;
    DWORD   flags;
    DWORD   time;
    DWORD   dwExtraInfo;
} _KBDLLHOOKSTRUCT, FAR *LP_KBDLLHOOKSTRUCT, *P_KBDLLHOOKSTRUCT;

// フックされた時のコールバック関数
LRESULT CALLBACK LowLevelKeyboardProc (INT nCode, WPARAM wParam, LPARAM lParam)
{
    // フック プロシージャから非ゼロの値を返すことにより、
    // メッセージがターゲット ウィンドウに渡されなくなります
    _KBDLLHOOKSTRUCT *pkbhs = (_KBDLLHOOKSTRUCT *) lParam;
    BOOL bControlKeyDown = 0;

	if( WinData.ActiveFlag == TRUE && WinData.SysCommandOffFlag == TRUE )
	{
		switch (nCode)
		{
			case HC_ACTION:
			{
				// Ctrl キーが押されたかどうかをチェック
				bControlKeyDown = GetAsyncKeyState (VK_CONTROL) >> ((sizeof(SHORT) * 8) - 1);

				// Ctrl + Esc を無効にします
				if (pkbhs->vkCode == VK_ESCAPE && bControlKeyDown)
					return 1;

				// Alt + Tab を無効にします
				if (pkbhs->vkCode == VK_TAB && pkbhs->flags & LLKHF_ALTDOWN)
					return 1;

				// Alt + Esc を無効にします
				if (pkbhs->vkCode == VK_ESCAPE && pkbhs->flags & LLKHF_ALTDOWN)
					return 1;

				// Alt + F4 を無効にします
				if (pkbhs->vkCode == VK_F4 && pkbhs->flags & LLKHF_ALTDOWN )
					return 1 ;

				// Alt up を無効にします
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

// ProcessMessage をひたすら呼びつづけるスレッド
DWORD WINAPI ProcessMessageThreadFunction( LPVOID )
{
	int Result ;
	DWORD ThreadID ;
	HANDLE EventHandle ;
	int WaitFlag = 0 ;

	// スレッドのＩＤを得ておく
	ThreadID = GetCurrentThreadId() ;

	// ひたすら ProcessMessage を呼びつづける
	for(;;)
	{
		// ソフトの終了フラグが立ったらループから外れる
		if( WinData.QuitMessageFlag == TRUE ) break ;

		// CheckConflictAndWaitDxFunction が呼ばれていたら、呼ばれ終わるまで待つ
		CRITICALSECTION_LOCK( &WinData.DxConflictCheckCriticalSection ) ;
//		CheckConflict( &WinData.DxConflictCheckFlag ) ;

		// ＤＸライブラリが使用されているかどうかで処理を分岐
		if( WinData.DxUseThreadFlag == FALSE || WinData.DxUseThreadID == ThreadID )
		{
//RUN:
			// 待っているスレッドが居て、それが自分のスレッドではない場合は待ち
			if( WinData.DxUseThreadFlag == FALSE && WinData.DxConflictWaitThreadNum != 0 && WinData.DxUseThreadID != ThreadID )
			{
				if( WinData.DxConflictWaitThreadID[0][0] != ThreadID )
				{
					goto WAIT ;
				}
				else
				{
					// もし待っていたのが自分だったらリストをスライドさせる
					if( WinData.DxConflictWaitThreadNum != 1 )
					{
						EventHandle = (HANDLE)WinData.DxConflictWaitThreadID[0][1] ;
						_MEMCPY( &WinData.DxConflictWaitThreadID[0][0], &WinData.DxConflictWaitThreadID[1][0], sizeof( DWORD_PTR ) * 2 * ( WinData.DxConflictWaitThreadNum - 1 ) ) ;
						WinData.DxConflictWaitThreadID[WinData.DxConflictWaitThreadNum - 1][1] = (DWORD_PTR)EventHandle ;
						WinData.DxConflictWaitThreadID[WinData.DxConflictWaitThreadNum - 1][0] = (DWORD_PTR)0 ;
					}

					// 待っている数を減らす
					WinData.DxConflictWaitThreadNum -- ;
				}
			}

			// 情報をセット
			WinData.DxUseThreadFlag = TRUE ;
			WinData.DxUseThreadID = ThreadID ;

			// 使用中カウンタをインクリメントする
//			if( WinData.DxConflictCheckCounter != 0 )
//			{
//				DXST_ERRORLOG_ADDW( L"エラー:衝突発生 No.2\n" ) ;
//			}
			WinData.DxConflictCheckCounter ++ ;

			// ロックフラグを倒す
			CriticalSection_Unlock( &WinData.DxConflictCheckCriticalSection ) ;
//			WinData.DxConflictCheckFlag -- ;

			// ProcessMessage を呼ぶ
			Result = NS_ProcessMessage() ;

			// CheckConflictAndWaitDxFunction が呼ばれていたら、呼ばれ終わるまで待つ
			CRITICALSECTION_LOCK( &WinData.DxConflictCheckCriticalSection ) ;
//			CheckConflict( &WinData.DxConflictCheckFlag ) ;

			// カウンタが０になってたら使用中状態を解除
			if( WinData.DxConflictCheckCounter == 1 )
			{
				WinData.DxUseThreadFlag = FALSE ;
				WinData.DxUseThreadID = 0xffffffff ;

				// もし待っているスレッドがある場合は、スリープするフラグを立てる
				if( WinData.DxConflictWaitThreadNum > 0 )
					WaitFlag = 1 ;
			}
//			else
//			{
//				DXST_ERRORLOG_ADDW( L"エラー:衝突発生 No.3 \n" ) ;
//			}

			// カウンタをデクリメント
			WinData.DxConflictCheckCounter -- ;

			// ロックフラグを倒す
			CriticalSection_Unlock( &WinData.DxConflictCheckCriticalSection ) ;
//			WinData.DxConflictCheckFlag -- ;

			// スリープするフラグが立っていたらスリープする
			if( WaitFlag == 1 )
			{
				// 次に実行すべきスレッドのイベントをシグナル状態にする
				SetEvent( (HANDLE)WinData.DxConflictWaitThreadID[0][1] ) ;
				WaitFlag = 0 ;
			}

			// PostMessageW の戻り値が -1 だったらループを抜ける
			if( Result < 0 ) break ;

			// 暫く寝る
			Sleep( 17 ) ;
		}
		else
		{
WAIT:
			// もし待っているスレッドが限界を越えていたら単純な待ち処理を行う
			if( WinData.DxConflictWaitThreadNum == MAX_THREADWAIT_NUM )
			{
				// とりあえずこの関数を使用中、フラグを倒す
				CriticalSection_Unlock( &WinData.DxConflictCheckCriticalSection ) ;
//				WinData.DxConflictCheckFlag -- ;

//				DXST_ERRORLOG_ADDW( L"エラー:衝突発生 No.5 \n" ) ;

				// 少し寝る
				Sleep( 1 ) ;

				// 最初に戻る
				continue ;
			}

			// 待ってるスレッドがいますよという情報を追加する
			WinData.DxConflictWaitThreadID[WinData.DxConflictWaitThreadNum][0] = ThreadID ;
			EventHandle = (HANDLE)WinData.DxConflictWaitThreadID[WinData.DxConflictWaitThreadNum][1] ;
			WinData.DxConflictWaitThreadNum ++ ;

			// この関数を使用中、フラグを倒す
			CriticalSection_Unlock( &WinData.DxConflictCheckCriticalSection ) ;
//			WinData.DxConflictCheckFlag -- ;

//			// 待ってるスレッドがいますよカウンターをインクリメントする
//			WinData.DxConflictWaitThreadCounter ++ ;
//			if( WinData.DxConflictWaitThreadCounter <= 0 )
//				WinData.DxConflictWaitThreadCounter = 1 ;

			// 使用中フラグが倒れるか、QuitMessageFlag が立つまで待つ
//			while( WinData.DxUseThreadFlag == TRUE && WinData.QuitMessageFlag != TRUE )

			while( WaitForSingleObject( EventHandle, 0 ) == WAIT_TIMEOUT && WinData.QuitMessageFlag != TRUE )
			{
				WaitForSingleObject( EventHandle, 1000 ) ;
//				Sleep( 0 ) ;
			}
//			WaitForSingleObject( EventHandle, INFINITE ) ;
			ResetEvent( EventHandle ) ;
			// 待ってるスレッドがあるよカウンタをデクリメントする
//			WinData.DxConflictWaitThreadCounter -- ;
//			if( WinData.DxConflictWaitThreadCounter < 0 )
//				WinData.DxConflictWaitThreadCounter = 0 ;

			// CheckConflictAndWaitDxFunction が呼ばれていたら、呼ばれ終わるまで待つ
//			CRITICALSECTION_LOCK( &WinData.DxConflictCheckCriticalSection ) ;
//			CheckConflict( &WinData.DxConflictCheckFlag ) ;

//			goto RUN ;
		}
	}

	// もしスレッドの待ち順の中に自分がいたら詰める
	{
		int i ;

		// CheckConflictAndWaitDxFunction が呼ばれていたら、呼ばれ終わるまで待つ
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

		// フラグを倒す
		CriticalSection_Unlock( &WinData.DxConflictCheckCriticalSection ) ;
//		WinData.DxConflictCheckFlag -- ;

		// もし元々自分の番だったら次のスレッドのイベントをシグナル状態にする
		if( i == 0 && WinData.DxConflictWaitThreadNum > 0 )
		{
			SetEvent( (HANDLE)WinData.DxConflictWaitThreadID[0][1] ) ;
		}
	}

	// スレッドが終了したことを示すフラグを立てる
	WinData.ProcessMessageThreadExitFlag = TRUE ;

	// スレッド終了
	ExitThread( 0 ) ;

	// 終了
	return 0 ;
}

#endif

//// フックされた時のコールバック関数
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

// タスクスイッチを有効にするかどうかを設定する
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

// タスクスイッチを有効にするかどうかを設定する
extern int SetSysCommandOffFlag_WCHAR_T( int Flag, const wchar_t *HookDllPath )
{
#ifndef DX_NON_STOPTASKSWITCH
	if( WinData.SysCommandOffFlag == Flag ) return 0 ;

	// 有効にする指定の場合はファイルパスを保存する
	if( Flag == TRUE )
	{
		// ファイルパスを保存
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

			// パス名が特に指定されなかった場合は内蔵のＤＬＬを
			// テンポラリファイルに出力して使用する

			// キーボードフックＤＬＬファイルのサイズを取得す
			if( *pDxKeyHookBinaryConvert == 0 )
			{
				*pDxKeyHookBinaryConvert = 1 ;
				Char128ToBin( pDxKeyHookBinary, pDxKeyHookBinary ) ;
			}
			FileSize = DXA_Decode( pDxKeyHookBinary, NULL ) ;

			// メモリの確保
			DestBuffer = DXALLOC( ( size_t )FileSize ) ;
			if( DestBuffer == NULL )
				return -1 ;

			// 解凍
			DXA_Decode( pDxKeyHookBinary, DestBuffer ) ;

			// テンポラリファイルのディレクトリパスを取得する
			if( GetTempPathW( FILEPATH_MAX, WinData.HookDLLFilePath ) == 0 )
			{
				DXFREE( DestBuffer ) ;
				return -1 ;
			}

			// 文字列の最後に￥マークをつける
			Length = ( int )_WCSLEN( WinData.HookDLLFilePath ) ;
			if( WinData.HookDLLFilePath[Length-1] != L'\\' ) 
			{
				WinData.HookDLLFilePath[Length]   = L'\\' ;
				WinData.HookDLLFilePath[Length+1] = L'\0' ;
			}

			// 誰も使いそうに無いファイル名を追加する
			_WCSCAT_S( WinData.HookDLLFilePath, sizeof( WinData.HookDLLFilePath ), L"ddxx_MesHoooooook.dll" );

			// テンポラリファイルを開く
			DeleteFileW( WinData.HookDLLFilePath ) ;
			FileHandle = CreateFileW( WinData.HookDLLFilePath, GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL ) ;
			if( FileHandle == NULL )
			{
				DXFREE( DestBuffer ) ;
				return -1 ;
			}

			// テンポラリファイルにデータを書き出す
			WriteFile( FileHandle, DestBuffer, ( DWORD )FileSize, &WriteSize, NULL ) ;

			// 閉じる
			CloseHandle( FileHandle ) ;

			// メモリの解放
			DXFREE( DestBuffer ) ;

			// ユーザー指定のフックＤＬＬを使っていないフラグを立てる
			WinData.NotUseUserHookDllFlag = TRUE ;
		}
		else
		{
			_WCSCPY_S( WinData.HookDLLFilePath, sizeof( WinData.HookDLLFilePath ), HookDllPath ) ; 

			// ユーザー指定のフックＤＬＬを使っていないフラグを倒す
			WinData.NotUseUserHookDllFlag = FALSE ;
		}
	}

	// ウインドウズのバージョンによって処理を分岐
	if( WinData.WindowsVersion < DX_WINDOWSVERSION_NT31 )
	{
		// Win95 カーネルの場合の処理
		UINT nPreviousState;
//		SystemParametersInfoW( SPI_SETSCREENSAVERRUNNING, Flag, &nPreviousState, 0 ) ;
//		SystemParametersInfoW( SPI_SCREENSAVERRUNNING, TRUE, &nPreviousState, 0 ) ;
		SystemParametersInfoW( SPI_SETSCREENSAVERRUNNING/*SPI_SCREENSAVERRUNNING*/, ( UINT )( WinData.ActiveFlag && Flag ? 1 : 0 ), &nPreviousState, 0 ) ;
	}
	else
	{
		// WinNT カーネルの場合の処理
		if( Flag == TRUE )
		{
			// キーボードフックのセット
//			if( WinData.TaskHookHandle == NULL )
//			{
//				WinData.TaskHookHandle = SetWindowsHookExW( WH_KEYBOARD_LL, LowLevelKeyboardProc, WinData.Instance, 0 ) ;
//			}

			// メッセージフックのセット
//			if( WinData.GetMessageHookHandle == NULL )
			{
//				WinData.MessageHookThredID = GetWindowThreadProcessId( WinData.MainWindow, NULL ) ;
//				WinData.MessageHookThredID = GetWindowThreadProcessId( GetDesktopWindow(), NULL ) ;
				WinData.MessageHookDLL = LoadLibraryW( WinData.HookDLLFilePath ) ;

				// DLL が無かったら進まない
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
			// メッセージフックを無効に
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

	// 全バージョン共通
/*	if( Flag == TRUE )
	{
		// 勝手に他のウインドウがアクティブにならないようにする
		while( WinData.ActiveFlag != TRUE )
		{
			if( NS_ProcessMessage() == -1 ) break ;
		}
		LockSetForegroundWindow( LSFW_LOCK ) ;
	}
*/
	// フラグを保存
	WinData.SysCommandOffFlag = Flag ;

	// 終了
	return 0 ;
#else
	return -1;
#endif
}

// メッセージをフックするウインドウプロージャを登録する
extern int NS_SetHookWinProc( WNDPROC WinProc )
{
	// 登録する
	WinData.UserWindowProc = WinProc ;

	// 終了
	return 0 ;
}

// SetHookWinProc で設定したウインドウプロージャの戻り値を使用するかどうかを設定する、SetHookWinProc で設定したウインドウプロージャの中でのみ使用可能( UseFlag TRUE:戻り値を使用して、ＤＸライブラリのウインドウプロージャの処理は行わない  FALSE:戻り値は使用せず、ウインドウプロージャから出た後、ＤＸライブラリのウインドウプロージャの処理を行う )
extern int NS_SetUseHookWinProcReturnValue( int UseFlag )
{
	// フラグを保存する
	WinData.UseUserWindowProcReturnValue = UseFlag ;

	// 終了
	return 0 ;
}

// ２重起動を許すかどうかのフラグをセットする
extern int NS_SetDoubleStartValidFlag( int Flag )
{
	WinData.DoubleStartValidFlag = Flag ;

	// 終了
	return 0 ;
}

// メッセージ処理をＤＸライブラリに肩代わりしてもらうウインドウを追加する
extern int NS_AddMessageTakeOverWindow( HWND Window )
{
	if( WinData.MesTakeOverWindowNum == MAX_MESTAKEOVERWIN_NUM ) return -1 ;

	// 追加する
	WinData.MesTakeOverWindow[WinData.MesTakeOverWindowNum] = Window ;
	WinData.MesTakeOverWindowNum ++ ;

	// 終了
	return 0 ;
}

// メッセージ処理をＤＸライブラリに肩代わりしてもらうウインドウを減らす
extern	int	NS_SubMessageTakeOverWindow( HWND Window )
{
	int i ;

	// 探す
	for( i = 0 ; i < MAX_MESTAKEOVERWIN_NUM && WinData.MesTakeOverWindow[i] != Window ; i ++ ){}
	if( i == MAX_MESTAKEOVERWIN_NUM ) return -1 ;

	// 減らす
	WinData.MesTakeOverWindowNum -- ;

	// つぶす
	_MEMMOVE( &WinData.MesTakeOverWindow[i], &WinData.MesTakeOverWindow[i+1], sizeof( HWND ) * ( WinData.MesTakeOverWindowNum - i ) ) ;

	// 終了
	return 0 ;
}

// ウインドウの初期位置を設定する
extern int NS_SetWindowInitPosition( int x, int y )
{
	WinData.WindowX = x ;
	WinData.WindowY = y ;
	WinData.WindowPosValid = TRUE ;

	return 0 ;
}

// ＤＸライブラリでメッセージ処理を行うダイアログボックスを登録する
extern int NS_SetDialogBoxHandle( HWND WindowHandle )
{
	WinData.DialogBoxHandle = WindowHandle ;
	
	return 0 ;
}

// メインウインドウを表示するかどうかのフラグをセットする
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

	// 終了
	return 0 ;
}

// メインウインドウを最小化するかどうかのフラグをセットする
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

	// 終了
	return 0 ;
}

// メインウインドウの×ボタンを押した時にライブラリが自動的にウインドウを閉じるかどうかのフラグをセットする
extern int NS_SetWindowUserCloseEnableFlag( int Flag )
{
	WinData.NonUserCloseEnableFlag = !Flag ;
	
	// 終了
	return 0 ;
}

// ＤＸライブラリ終了時に PostQuitMessage を呼ぶかどうかのフラグをセットする
extern int NS_SetDxLibEndPostQuitMessageFlag( int Flag )
{
	WinData.NonDxLibEndPostQuitMessageFlag = !Flag ;

	// 終了
	return 0 ;
}

// 表示に使用するウインドウのハンドルを取得する
extern HWND GetDisplayWindowHandle( void )
{
	if( WinData.UserWindowFlag )
	{
		return WinData.UserChildWindow ? WinData.UserChildWindow : WinData.MainWindow ;
	}

	return WinData.MainWindow ;
}

// ユーザーウインドウの情報を更新
static void UpdateUserWindowInfo( void )
{
	// 画面モードはウインドウモード
	NS_ChangeWindowMode( TRUE ) ;

#ifndef DX_NON_GRAPHICS
	// 画面サイズはクライアント領域のサイズ
	{
		RECT ClientRect ;

		GetClientRect( GetDisplayWindowHandle(), &ClientRect ) ;
		NS_SetGraphMode( ClientRect.right - ClientRect.left,
							ClientRect.bottom - ClientRect.top, 32 ) ;
	}
#endif // DX_NON_GRAPHICS
}

// ＤＸライブラリで利用するウインドウのハンドルをセットする
extern int NS_SetUserWindow( HWND WindowHandle )
{
	if( DxSysData.DxLib_InitializeFlag == TRUE ) return -1 ;

	WinData.MainWindow = WindowHandle ;
	WinData.UserWindowFlag = WindowHandle == NULL ? FALSE : TRUE ;

	// 情報を更新
	UpdateUserWindowInfo() ;

	// 終了
	return 0 ;
}

// ＤＸライブラリで使用する表示用の子ウインドウのハンドルをセットする(DxLib_Init を実行する以前でのみ有効)
extern int NS_SetUserChildWindow( HWND WindowHandle )
{
	if( DxSysData.DxLib_InitializeFlag == TRUE ) return -1 ;

	WinData.UserChildWindow = WindowHandle ;

	// 情報を更新
	UpdateUserWindowInfo() ;

	// 終了
	return 0 ;
}

// SetUseWindow で設定したウインドウのメッセージループ処理をＤＸライブラリで行うかどうか、フラグをセットする
extern int NS_SetUserWindowMessageProcessDXLibFlag( int Flag )
{
	WinData.NotUserWindowMessageProcessDXLibFlag = !Flag ;

	// 終了
	return 0 ;
}

// FPUの精度を落とさない設定を使用するかどうかを設定する、DxLib_Init を呼び出す前のみ有効( TRUE:使用する(精度が落ちない)  FALSE:使用しない(精度を落とす(デフォルト) )
extern int NS_SetUseFPUPreserveFlag( int Flag )
{
	// 初期化前のみ有効
	if( DxSysData.DxLib_InitializeFlag ) return -1 ;

	// フラグを保存
	WinData.UseFPUPreserve = Flag ;

	// 終了
	return 0 ;
}

// マウスポインタがウインドウのクライアントエリアの外にいけるかどうかを設定する( TRUE:いける( デフォルト設定 )  FALSE:いけない )
extern int NS_SetValidMousePointerWindowOutClientAreaMoveFlag( int Flag )
{
	Flag = Flag ? FALSE : TRUE ;

	// 今までとフラグが同じ場合は何もしない
	if( WinData.NotMoveMousePointerOutClientAreaFlag == Flag )
		return 0 ;

	// フラグを保存
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

	// 終了
	return 0 ;
}

// バックバッファの透過色の部分を透過させるかどうかを設定する( TRUE:透過させる  FALSE:透過させない )
extern int NS_SetUseBackBufferTransColorFlag( int Flag )
{
	// 今までとフラグが同じ場合は何もしない
	if( WinData.BackBufferTransColorFlag == Flag )
		return 0 ;

	// フラグを保存
	WinData.BackBufferTransColorFlag = Flag ;

	// ウインドウモードを変更する
	NS_SetWindowStyleMode( WinData.WindowStyle ) ;

	// 終了
	return 0 ;
}

// UpdateLayerdWindowForBaseImage や UpdateLayerdWindowForSoftImage を使用するかどうかを設定する( TRUE:使用する  FALSE:使用しない )
extern int NS_SetUseUpdateLayerdWindowFlag( int Flag )
{
	// 今までとフラグが同じ場合は何もしない
	if( WinData.UseUpdateLayerdWindowFlag == Flag )
		return 0 ;

	// フラグを保存
	WinData.UseUpdateLayerdWindowFlag = Flag ;

	// ウインドウモードを変更する
	NS_SetWindowStyleMode( WinData.WindowStyle ) ;

	// 終了
	return 0 ;
}

// リソースを読み込む際に使用するモジュールを設定する( NULL を指定すると初期状態に戻ります、デフォルトでは NULL )
extern int NS_SetResourceModule( HMODULE ResourceModule )
{
	WinData.LoadResourModule = ResourceModule ;

	// 終了
	return 0 ;
}

// WM_PAINT メッセージが来た際に『ＤＸライブラリの WM_PAINTメッセージが来た際の処理』を行うかどうかを設定する( 別スレッドで描画処理を行う場合などで使用 )
extern int NS_SetUseDxLibWM_PAINTProcess( int Flag )
{
	WinData.NotUseDxLibWM_PAINTProcess = Flag ? FALSE : TRUE ;

	// 終了
	return 0 ;
}












// クリップボード関係

// クリップボードに格納されているテキストデータを読み出す、-1 の場合はクリップボードにテキストデータは無いということ( DestBuffer に NULL を渡すと格納に必要なデータサイズが返ってくる )
extern int NS_GetClipboardText( TCHAR *DestBuffer )
{
	HGLOBAL Mem ;
	void *SrcBuffer ;

	// クリップボードをオープン
	if( OpenClipboard( WinData.MainWindow ) == 0 )
		return -1 ;

	// クリップボードに格納されているデータがテキストデータかどうかを取得する
	if( IsClipboardFormatAvailable( CLIPBOARD_TEXT ) == 0 )
	{
		CloseClipboard() ;
		return -1 ;
	}

	// クリップボードに格納されているテキストデータのメモリハンドルを取得する
	Mem = GetClipboardData( CLIPBOARD_TEXT ) ;

	// 出力バッファが NULL の場合はテキストデータのサイズを返す
	if( DestBuffer == NULL )
	{
		SIZE_T Size ;

		Size = GlobalSize( Mem ) + 1 ;
		CloseClipboard() ;
		return ( int )Size ;
	}

	// 出力バッファにテキストデータをコピーする
	SrcBuffer = GlobalLock( Mem ) ;
	_TSTRCPY( DestBuffer, ( TCHAR * )SrcBuffer ) ;
	GlobalUnlock( Mem ) ;
	CloseClipboard() ;

	// 終了
	return 0 ;
}

// クリップボードに格納されているテキストデータを読み出す、-1 の場合はクリップボードにテキストデータは無いということ( DestBuffer に NULL を渡すと格納に必要なデータサイズが返ってくる )
extern int GetClipboardText_WCHAR_T( wchar_t *DestBuffer )
{
	HGLOBAL Mem ;
	void *SrcBuffer ;

	// クリップボードをオープン
	if( OpenClipboard( WinData.MainWindow ) == 0 )
		return -1 ;

	// クリップボードに格納されているデータがテキストデータかどうかを取得する
	if( IsClipboardFormatAvailable( CF_UNICODETEXT ) == 0 )
	{
		CloseClipboard() ;
		return -1 ;
	}

	// クリップボードに格納されているテキストデータのメモリハンドルを取得する
	Mem = GetClipboardData( CF_UNICODETEXT ) ;

	// 出力バッファが NULL の場合はテキストデータのサイズを返す
	if( DestBuffer == NULL )
	{
		SIZE_T Size ;

		Size = GlobalSize( Mem ) + 1 ;
		CloseClipboard() ;
		return ( int )Size ;
	}

	// 出力バッファにテキストデータをコピーする
	SrcBuffer = GlobalLock( Mem ) ;
	_WCSCPY( DestBuffer, ( wchar_t * )SrcBuffer ) ;
	GlobalUnlock( Mem ) ;
	CloseClipboard() ;

	// 終了
	return 0 ;
}

// クリップボードにテキストデータを格納する
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

// クリップボードにテキストデータを格納する
extern int SetClipboardText_WCHAR_T( const wchar_t *Text )
{
	HGLOBAL Mem ;
	void *Buffer ;
	int Len ;

	// 文字列の長さを取得
	Len = ( int )_WCSLEN( Text ) ;

	// 文字列を格納するメモリ領域の確保
	Mem = GlobalAlloc( GMEM_FIXED, ( Len + 1 ) * sizeof( wchar_t ) ) ;

	// 文字列を確保したメモリ領域に格納
	Buffer = GlobalLock( Mem ) ;
	_WCSCPY( ( wchar_t * )Buffer, Text ) ;
	GlobalUnlock( Mem ) ;

	// クリップボードをオープン
	if( OpenClipboard( WinData.MainWindow ) )
	{
		// クリップボードに文字列を格納する
		EmptyClipboard() ;
		SetClipboardData( CF_UNICODETEXT, Mem ) ;
		CloseClipboard() ;
	}
	else
	{
		// メモリの解放
		GlobalFree( Mem ) ;
	}

	// 終了
	return 0 ;
}










// ドラッグ＆ドロップされたファイル関係

// ファイルのドラッグ＆ドロップ機能を有効にするかどうかの設定を再設定する
extern int RefreshDragFileValidFlag()
{
	// 有効状態を変更
	DragAcceptFiles( WinData.MainWindow, WinData.DragFileValidFlag ) ;

	// 終了
	return 0 ;
}

// ファイルのドラッグ＆ドロップ機能を有効にするかどうかのフラグをセットする
extern int NS_SetDragFileValidFlag( int Flag )
{
	// 指定と既に同じ場合は何もせず終了
	if( WinData.DragFileValidFlag == Flag ) return 0 ;
	
	// 有効状態を変更
	DragAcceptFiles( WinData.MainWindow, Flag ) ;
	
	// フラグを保存
	WinData.DragFileValidFlag = Flag ;

	// 終了
	return 0 ;
}

// ドラッグ＆ドロップされたファイルの情報を初期化する
extern int NS_DragFileInfoClear( void )
{
	int i ;
	
	// 全てのファイル名用に確保したメモリ領域を解放する
	for( i = 0 ; i < WinData.DragFileNum ; i ++ )
	{
		DXFREE( WinData.DragFileName[i] ) ;
		WinData.DragFileName[i] = NULL ;
	}
	
	// ファイルの数を０にする
	WinData.DragFileNum = 0 ;
	
	// 終了
	return 0 ;
}

// ドラッグ＆ドロップされたファイル名を取得する( -1:取得できなかった  0:取得できた )
extern int NS_GetDragFilePath( TCHAR *FilePathBuffer )
{
#ifdef UNICODE
	return GetDragFilePath_WCHAR_T( FilePathBuffer ) ;
#else
	int Size ;

	// ファイル名が一つも無かったら -1 を返す
	if( WinData.DragFileNum == 0 )
	{
		return -1 ;
	}

	// 文字列をコピーする
	Size = ConvString( ( const char * )WinData.DragFileName[ WinData.DragFileNum - 1 ], WCHAR_T_CHARCODEFORMAT, ( char * )FilePathBuffer, BUFFERBYTES_CANCEL, _TCHARCODEFORMAT ) ;

	// NULL を渡されたら文字列格納に必要なサイズを返す
	if( FilePathBuffer == NULL ) 
	{
		return Size ;
	}

	// 渡し終わった文字列は解放する
	DXFREE( WinData.DragFileName[ WinData.DragFileNum - 1 ] ) ;
	WinData.DragFileName[ WinData.DragFileNum - 1 ] = NULL ;

	// ドラッグ＆ドロップされたファイルの数を減らす
	WinData.DragFileNum -- ;

	// 終了
	return 0 ;
#endif
}

// ドラッグ＆ドロップされたファイル名を取得する( -1:取得できなかった  0:取得できた )
extern int GetDragFilePath_WCHAR_T( wchar_t *FilePathBuffer )
{
	// ファイル名が一つも無かったら -1 を返す
	if( WinData.DragFileNum == 0 )
	{
		return -1 ;
	}

	// NULL を渡されたら文字列格納に必要なサイズを返す
	if( FilePathBuffer == NULL ) 
	{
		return ( int )( ( _WCSLEN( WinData.DragFileName[ WinData.DragFileNum - 1 ] ) + 1 ) * sizeof( wchar_t ) ) ;
	}

	// 文字列をコピーする
	_WCSCPY( FilePathBuffer, WinData.DragFileName[ WinData.DragFileNum - 1 ] ) ;

	// 渡し終わった文字列は解放する
	DXFREE( WinData.DragFileName[ WinData.DragFileNum - 1 ] ) ;
	WinData.DragFileName[ WinData.DragFileNum - 1 ] = NULL ;

	// ドラッグ＆ドロップされたファイルの数を減らす
	WinData.DragFileNum -- ;

	// 終了
	return 0 ;
}

// ドラッグ＆ドロップされたファイルの数を取得する
extern int NS_GetDragFileNum( void )
{
	return WinData.DragFileNum ;
}




















// ウインドウ描画領域設定系関数

// 任意のグラフィックからRGNハンドルを作成する
extern HRGN NS_CreateRgnFromGraph( int Width, int Height, const void *MaskData, int Pitch, int Byte )
{
#ifndef DX_NON_GRAPHICS
	HRGN RgnTmp, Rgn ;
	int i, j, x, f = 0, AddPitch ;
	BYTE *Data ;
	int SizeX, SizeY ;

	// 大元のRGNハンドルを作成する
	NS_GetDrawScreenSize( &SizeX, &SizeY ) ;
	Rgn = CreateRectRgn( 0, 0, SizeX, SizeY ) ;
	RgnTmp = CreateRectRgn( 0, 0, SizeX, SizeY ) ;
	CombineRgn( Rgn, Rgn, RgnTmp, RGN_XOR ) ;
	DeleteObject( RgnTmp ) ;

	// 透過RGNの走査
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

	// 終了
	return Rgn ;
#else // DX_NON_GRAPHICS
	return NULL ;
#endif // DX_NON_GRAPHICS
}

// 任意の基本イメージデータと透過色からRGNハンドルを作成する
extern HRGN NS_CreateRgnFromBaseImage( BASEIMAGE *BaseImage, int TransColorR, int TransColorG, int TransColorB )
{
	HRGN RgnTmp, Rgn ;
	int i, j, x, f, AddPitch, Byte ;
	BYTE *Data ;
	DWORD TransColor ;
	int Width, Height ;
	BASEIMAGE TempBaseImage, *UseBaseImage ;

	// 基本イメージデータのフォーマットがXRGB8以外の場合はXRGB8形式にする
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

	// 大元のRGNハンドルを作成する
	Width = UseBaseImage->Width ;
	Height = UseBaseImage->Height ;
	Rgn = CreateRectRgn( 0, 0, Width, Height ) ;
	RgnTmp = CreateRectRgn( 0, 0, Width, Height ) ;
	CombineRgn( Rgn, Rgn, RgnTmp, RGN_XOR ) ;
	DeleteObject( RgnTmp ) ;

	// 透過RGNの走査
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

	// 終了
	return Rgn ;
}


// 任意のグラフィックからＲＧＮをセットする
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

// 任意のグラフィックからＲＧＮをセットする
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
		// ファイルを読み込む
		bmp = CreateDIBGraph_WCHAR_T( FileName, FALSE, NULL ) ;
		if( bmp == NULL )
		{
			return -1 ;
		}

		// グラフィックの情報を取得する
		GetObject( bmp, sizeof( BITMAP ), &bm ) ;

		// ピッチ補正
		bm.bmWidthBytes += bm.bmWidthBytes % 4 ? 4 - bm.bmWidthBytes % 4 : 0 ;

		// リージョン作成
		if( WinData.WindowRgn != NULL )
		{
			DeleteObject( WinData.WindowRgn ) ;
		}
		WinData.WindowRgn = NS_CreateRgnFromGraph( bm.bmWidth, bm.bmHeight, bm.bmBits, bm.bmWidthBytes, bm.bmBitsPixel / 8 ) ;

		// リージョンを割り当てる
		if( WinData.MainWindow != NULL )
		{
			SetWindowRgn( WinData.MainWindow, WinData.WindowRgn, TRUE ) ;
		}
	}

	// 終了
	return 0 ;
}

// 描画先の画面の透過色の部分を透過させるＲＧＮをセットする
extern int NS_UpdateTransColorWindowRgn( void )
{
#ifndef DX_NON_GRAPHICS
	BASEIMAGE ScreenImage ;
	int Width, Height ;

	NS_GetDrawScreenSize( &Width, &Height ) ;
	NS_CreateXRGB8ColorBaseImage( Width, Height, &ScreenImage ) ;
	NS_GetDrawScreenBaseImage( 0, 0, Width, Height, &ScreenImage ) ;

	// リージョン作成
	if( WinData.WindowRgn != NULL ) DeleteObject( WinData.WindowRgn ) ;
	WinData.WindowRgn = NS_CreateRgnFromBaseImage( &ScreenImage,
		( int )( ( GSYS.CreateImage.TransColor >> 16 ) & 0xff ),
		( int )( ( GSYS.CreateImage.TransColor >>  8 ) & 0xff ),
		( int )(   GSYS.CreateImage.TransColor         & 0xff ) ) ;

	// リージョンを割り当てる
	if( WinData.MainWindow != NULL ) SetWindowRgn( WinData.MainWindow, WinData.WindowRgn, TRUE ) ;

	NS_ReleaseBaseImage( &ScreenImage ) ;

	// 終了
	return 0 ;
#else // DX_NON_GRAPHICS
	return -1 ;
#endif // DX_NON_GRAPHICS
}















// ツールバー関係

// 指定のＩＤのボタンのインデックスを得る
static int SearchToolBarButton( int ID )
{
	int i ;
	WINTOOLBARITEMINFO *but ;

	// ツールバーのセットアップが完了していない場合は何もせずに終了
	if( WinData.ToolBarUseFlag == FALSE ) return -1 ;

	// 指定のＩＤのボタンを探す
	but = WinData.ToolBarItem ;
	for( i = 0 ; i < WinData.ToolBarItemNum ; i ++, but ++ )
	{
		if( but->Type == TOOLBUTTON_TYPE_SEP ) continue ;	// 隙間は除外
		if( but->ID == ID ) break ;							// ＩＤが一致した場合は抜ける
	}

	// 無かったらエラー
	if( i == WinData.ToolBarItemNum ) return -1;

	// あったらインデックスを返す
	return i ;
}

// ツールバーの高さを得る
extern int GetToolBarHeight( void )
{
	RECT rect ;

	if( WinData.ToolBarUseFlag == FALSE ) return 0 ;

	GetWindowRect( WinData.ToolBarHandle, &rect ) ;
	return rect.bottom - rect.top ;
}

// ツールバーの準備( NULL を指定するとツールバーを解除 )
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

// ツールバーの準備( NULL を指定するとツールバーを解除 )
extern int SetupToolBar_WCHAR_T( const wchar_t *BitmapName, int DivNum, int ResourceID )
{
	HBITMAP NewBitmap = NULL ;
	BITMAP bm ;

	if( WinAPIData.Win32Func.WinMMDLL == NULL )
	{
		return -1 ;
	}

	// BitmapName が NULL で、且つツールバーを使っていない場合は何もせずに終了
	if( BitmapName == NULL && ResourceID < 0 && WinData.ToolBarUseFlag == FALSE )
	{
		return 0 ;
	}

	// フルスクリーンで起動している場合も何もせずに終了
	if( WinData.WindowModeFlag == FALSE )
	{
		return 0 ;
	}

	// とりあえず全てのボタンを削除
	NS_DeleteAllToolBarButton() ;

	// BitmapName が NULL で、ResouceID も -1 の場合はツールバーを削除する
	if( BitmapName == NULL && ResourceID < 0 )
	{
		// ツールバーウインドウに WM_CLOSE メッセージを送る
		SendMessageW( WinData.ToolBarHandle, WM_CLOSE, 0, 0 ) ;
		WinData.ToolBarHandle = NULL ;

		// ビットマップファイルを削除する
		DeleteObject( WinData.ToolBarButtonImage ) ;
		WinData.ToolBarButtonImage = NULL ;

		// ツールバーを使用しているフラグを倒す
		WinData.ToolBarUseFlag = FALSE ;
	}
	else
	{
		// ビットマップファイルを読み込む
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

			// リソースを取得
			RSrc = FindResource( WinData.LoadResourModule == NULL ? WinAPIData.Win32Func.GetModuleHandleWFunc( NULL ) : WinData.LoadResourModule, MAKEINTRESOURCE( ResourceID ), RT_BITMAP ) ;
			if( RSrc )
			{
				// リソースが格納されているメモリ領域を取得
				Global = LoadResource( WinData.LoadResourModule == NULL ? WinAPIData.Win32Func.GetModuleHandleWFunc( NULL ) : WinData.LoadResourModule, RSrc ) ;
				if( Global )
				{
					DataP = ( BYTE * )LockResource( Global ) ;
					if( DataP )
					{
						// イメージデータの位置を取得
						BmpInfo = ( BITMAPINFO * )DataP ;
						DataP += sizeof( BITMAPINFOHEADER ) ;

						// カラービット数が８以下の時はパレットがある
						if( BmpInfo->bmiHeader.biBitCount <= 8 )
						{
							DataP += ( size_t )( 1 << BmpInfo->bmiHeader.biBitCount ) * sizeof( RGBQUAD ) ;
						}
						else
						// カラービット数が３２か１６でカラーマスクがある
						if( BmpInfo->bmiHeader.biBitCount == 16 || BmpInfo->bmiHeader.biBitCount == 32 || BmpInfo->bmiHeader.biCompression == BI_BITFIELDS )
						{
							DataP += sizeof( RGBQUAD ) * 3 ;
						}

						// HBITMAP の作成
						NewBitmap = NS_CreateDIBGraphToMem( BmpInfo, DataP, FALSE, NULL ) ;
					}

					// リソース解放
					UnlockResource( Global ) ;
				}
			}
//			NewBitmap = LoadBitmap( WinData.Instance, MAKEINTRESOURCE( ResourceID ) ) ;
		}

		if( NewBitmap == NULL )
		{
			DXST_ERRORLOG_ADDUTF16LE( "\xc4\x30\xfc\x30\xeb\x30\xd0\x30\xfc\x30\x6e\x30\xdc\x30\xbf\x30\xf3\x30\x28\x75\x6e\x30\xd3\x30\xc3\x30\xc8\x30\xde\x30\xc3\x30\xd7\x30\xd5\x30\xa1\x30\xa4\x30\xeb\x30\x6e\x30\xad\x8a\x7f\x30\xbc\x8f\x7f\x30\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"ツールバーのボタン用のビットマップファイルの読み込みに失敗しました\n" @*/ ) ;
			return -1 ;
		}

		// ビットマップのサイズを得る
		GetObject( NewBitmap, sizeof( bm ), &bm ) ;

		// ツールバーを既に使っているかどうかで処理を分岐
		if( WinData.ToolBarUseFlag == FALSE )
		{
			TBADDBITMAP AddBitmap ;

			// 今まで使っていなかった場合はツールバーウインドウを作成する
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
				DXST_ERRORLOG_ADDUTF16LE( "\xc4\x30\xfc\x30\xeb\x30\xd0\x30\xfc\x30\xa6\x30\xa4\x30\xf3\x30\xc9\x30\xa6\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"ツールバーウインドウの作成に失敗しました\n" @*/ ) ;
				return -1 ;
			}

			// TBBUTTON 構造体のサイズを送っておく
			SendMessageW( WinData.ToolBarHandle, TB_BUTTONSTRUCTSIZE,
						 (WPARAM)sizeof( TBBUTTON ), 0 ) ;

			// ツールバーを使用している状態にする
			WinData.ToolBarUseFlag = TRUE ;

			// ビットマップファイルを設定する
			AddBitmap.hInst = NULL ;
			AddBitmap.nID   = (UINT_PTR)NewBitmap ;
			SendMessageW( WinData.ToolBarHandle, TB_ADDBITMAP, ( WPARAM )DivNum, ( LPARAM )&AddBitmap ) ;
			WinData.ToolBarButtonImage = NewBitmap ;

			// 情報を初期化する
			WinData.ToolBarItemNum = 0 ;
		}
		else
		{
			// 既に作られている場合はビットマップファイルを交換する
			TBREPLACEBITMAP RepBitmap ;

			RepBitmap.hInstOld = NULL ;
			RepBitmap.nIDOld   = (UINT_PTR)WinData.ToolBarButtonImage ;
			RepBitmap.hInstNew = NULL ;
			RepBitmap.nIDNew   = (UINT_PTR)NewBitmap ;
			RepBitmap.nButtons = DivNum ;
			SendMessageW( WinData.ToolBarHandle, TB_REPLACEBITMAP, 0, (LPARAM)&RepBitmap ) ;

			// 今までのビットマップを破棄する
			DeleteObject( WinData.ToolBarButtonImage ) ;
			WinData.ToolBarButtonImage = NewBitmap ;
		}

		// ビットマップのサイズを設定する
		SendMessageW( WinData.ToolBarHandle, TB_SETBITMAPSIZE, 0, ( bm.bmWidth / DivNum ) | ( bm.bmHeight << 16 ) ) ;

		// スタイルを変更する
		NS_SetWindowStyleMode( 6 ) ;
	}

	WM_SIZEProcess() ;

	// 終了
	return 0 ;
}

// ツールバーにボタンを追加
// int Type   : TOOLBUTTON_TYPE_NORMAL 等
// int State  : TOOLBUTTON_STATE_ENABLE 等
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

	// ツールバーのセットアップが完了していない場合は何もせずに終了
	if( WinData.ToolBarUseFlag == FALSE ) return -1 ;

	// ツールバーのボタンの数が最大数に達していたら何もせずに終了
	if( WinData.ToolBarItemNum == MAX_TOOLBARITEM_NUM )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\xc4\x30\xfc\x30\xeb\x30\xd0\x30\xfc\x30\x6e\x30\xdc\x30\xbf\x30\xf3\x30\x6e\x30\x70\x65\x4c\x30\x00\x67\x27\x59\x70\x65\x6e\x30\x20\x00\x25\x00\x64\x00\x20\x00\x6b\x30\x54\x90\x57\x30\x66\x30\x44\x30\x8b\x30\xba\x70\xdc\x30\xbf\x30\xf3\x30\x92\x30\xfd\x8f\xa0\x52\x67\x30\x4d\x30\x7e\x30\x5b\x30\x93\x30\x67\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"ツールバーのボタンの数が最大数の %d に達している為ボタンを追加できませんでした\n" @*/, MAX_TOOLBARITEM_NUM )) ;
		return -1 ;
	}

	// パラメータの値が異常な場合はエラー
	if( Type >= TOOLBUTTON_TYPE_NUM )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\xc4\x30\xfc\x30\xeb\x30\xd0\x30\xfc\x30\x6e\x30\xdc\x30\xbf\x30\xf3\x30\xfd\x8f\xa0\x52\xa2\x95\x70\x65\x6b\x30\x4a\x30\x44\x30\x66\x30\x20\x00\x53\x00\x74\x00\x61\x00\x74\x00\x65\x00\x20\x00\x6e\x30\x24\x50\x4c\x30\x0d\x4e\x63\x6b\x6a\x30\x24\x50\x20\x00\x25\x00\x64\x00\x20\x00\x68\x30\x6a\x30\x63\x30\x66\x30\x44\x30\x7e\x30\x59\x30\x0a\x00\x00"/*@ L"ツールバーのボタン追加関数において State の値が不正な値 %d となっています\n" @*/, State )) ;
		return -1 ;
	}
	if( State >= TOOLBUTTON_STATE_NUM )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\xc4\x30\xfc\x30\xeb\x30\xd0\x30\xfc\x30\x6e\x30\xdc\x30\xbf\x30\xf3\x30\xfd\x8f\xa0\x52\xa2\x95\x70\x65\x6b\x30\x4a\x30\x44\x30\x66\x30\x20\x00\x54\x00\x79\x00\x70\x00\x65\x00\x20\x00\x6e\x30\x24\x50\x4c\x30\x0d\x4e\x63\x6b\x6a\x30\x24\x50\x20\x00\x25\x00\x64\x00\x20\x00\x68\x30\x6a\x30\x63\x30\x66\x30\x44\x30\x7e\x30\x59\x30\x0a\x00\x00"/*@ L"ツールバーのボタン追加関数において Type の値が不正な値 %d となっています\n" @*/, Type )) ;
		return -1 ;
	}

	// 新しいボタンを追加
	_MEMSET( &tbbut, 0, sizeof( tbbut ) ) ;
	tbbut.iBitmap = ImageIndex ;
	tbbut.idCommand = TOOLBAR_COMMANDID_BASE + ID ;
	tbbut.fsState = StateTable[ Type == TOOLBUTTON_TYPE_CHECK || Type == TOOLBUTTON_TYPE_GROUP ? 1 : 0 ][ State ] ;
	tbbut.fsStyle = TypeTable[ Type ] ;
	SendMessageW( WinData.ToolBarHandle, TB_ADDBUTTONS, 1, (LPARAM)&tbbut ) ;

	// 新しいボタンの情報をセット
	but = &WinData.ToolBarItem[ WinData.ToolBarItemNum ] ;
	but->ID         = ID ;
	but->ImageIndex = ImageIndex ;
	but->Type       = Type ;
	but->State      = State ;
	but->Click      = FALSE ;

	// ボタンの数を増やす
	WinData.ToolBarItemNum ++ ;

	// 終了
	return 0 ;
}

// ツールバーに隙間を追加
extern int NS_AddToolBarSep( void )
{
	WINTOOLBARITEMINFO *but ;
	TBBUTTON tbbut ;

	// ツールバーのセットアップが完了していない場合は何もせずに終了
	if( WinData.ToolBarUseFlag == FALSE ) return -1 ;

	// ツールバーのボタンの数が最大数に達していたら何もせずに終了
	if( WinData.ToolBarItemNum == MAX_TOOLBARITEM_NUM )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\xc4\x30\xfc\x30\xeb\x30\xd0\x30\xfc\x30\x6e\x30\xa2\x30\xa4\x30\xc6\x30\xe0\x30\x6e\x30\x70\x65\x4c\x30\x00\x67\x27\x59\x70\x65\x6e\x30\x20\x00\x25\x00\x64\x00\x20\x00\x6b\x30\x54\x90\x57\x30\x66\x30\x44\x30\x8b\x30\xba\x70\x99\x96\x93\x95\x92\x30\xfd\x8f\xa0\x52\x67\x30\x4d\x30\x7e\x30\x5b\x30\x93\x30\x67\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"ツールバーのアイテムの数が最大数の %d に達している為隙間を追加できませんでした\n" @*/, MAX_TOOLBARITEM_NUM )) ;
		return -1 ;
	}

	// 新しく隙間を追加
	_MEMSET( &tbbut, 0, sizeof( tbbut ) ) ;
	tbbut.iBitmap   = 0 ;
	tbbut.idCommand = 0 ;
	tbbut.fsState   = TBSTATE_ENABLED ;
	tbbut.fsStyle   = TBSTYLE_SEP ;
	SendMessageW( WinData.ToolBarHandle, TB_ADDBUTTONS, 1, (LPARAM)&tbbut ) ;

	// 新しいボタンの情報をセット
	but = &WinData.ToolBarItem[ WinData.ToolBarItemNum ] ;
	but->ID         = 0 ;
	but->ImageIndex = 0 ;
	but->Type       = TOOLBUTTON_TYPE_SEP ;
	but->State      = TOOLBUTTON_STATE_ENABLE ;
	but->Click      = FALSE ;

	// ボタンの数を増やす
	WinData.ToolBarItemNum ++ ;

	// 終了
	return 0 ;
}

// ツールバーのボタンの状態を取得
extern int NS_GetToolBarButtonState( int ID )
{
	int i ;
	WINTOOLBARITEMINFO *but ;
	int State = 0 ;
	LRESULT Result ;

	// ツールバーのセットアップが完了していない場合は何もせずに終了
	if( WinData.ToolBarUseFlag == FALSE ) return -1 ;

	// 指定のＩＤのボタンを探す
	i = SearchToolBarButton( ID ) ;
	if( i == -1 )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\x07\x63\x9a\x5b\x6e\x30\x29\xff\x24\xff\x20\x00\x25\x00\x64\x00\x20\x00\x92\x30\x01\x63\x63\x30\x5f\x30\xc4\x30\xfc\x30\xeb\x30\xd0\x30\xfc\x30\x6e\x30\xdc\x30\xbf\x30\xf3\x30\x4c\x30\x42\x30\x8a\x30\x7e\x30\x5b\x30\x93\x30\x67\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"指定のＩＤ %d を持ったツールバーのボタンがありませんでした\n" @*/, ID )) ;
		return -1;
	}
	but = &WinData.ToolBarItem[i] ;

	// ボタンのタイプによって処理を分岐
	switch( but->Type )
	{
	case TOOLBUTTON_TYPE_NORMAL :	// 普通のボタンの場合
		if( but->State == TOOLBUTTON_STATE_ENABLE )
		{
			// 一度でも押されたら TRUE 状態になる変数を返す
			State = but->Click ;
			but->Click = FALSE ;
		}
		else
		{
			return but->State == TOOLBUTTON_STATE_PRESSED ? TRUE : FALSE ;
		}
		break ;

	case TOOLBUTTON_TYPE_CHECK :	// 押すごとにＯＮ／ＯＦＦが切り替わるボタン
	case TOOLBUTTON_TYPE_GROUP :	// 別の TOOLBUTTON_TYPE_GROUP タイプのボタンが押されるとＯＦＦになるタイプのボタン(グループの区切りは隙間で)
		// ボタンの状態を取得する
		Result = SendMessageW( WinData.ToolBarHandle, TB_GETSTATE, ( WPARAM )( TOOLBAR_COMMANDID_BASE + but->ID ), 0 ) ;
		if( Result & ( TBSTATE_CHECKED | TBSTATE_PRESSED ) ) State = TRUE  ;
		else                                                 State = FALSE ;
		break ;
	}

	// 状態を返す
	return State ;
}

// ツールバーのボタンの状態を設定
extern int NS_SetToolBarButtonState( int ID, int State )
{
	int i ;
	int SetState = 0 ;
	WINTOOLBARITEMINFO *but ;

	// ツールバーのセットアップが完了していない場合は何もせずに終了
	if( WinData.ToolBarUseFlag == FALSE ) return -1 ;

	// 指定のＩＤのボタンを探す
	i = SearchToolBarButton( ID ) ;
	if( i == -1 )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\x07\x63\x9a\x5b\x6e\x30\x29\xff\x24\xff\x20\x00\x25\x00\x64\x00\x20\x00\x92\x30\x01\x63\x63\x30\x5f\x30\xc4\x30\xfc\x30\xeb\x30\xd0\x30\xfc\x30\x6e\x30\xdc\x30\xbf\x30\xf3\x30\x4c\x30\x42\x30\x8a\x30\x7e\x30\x5b\x30\x93\x30\x67\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"指定のＩＤ %d を持ったツールバーのボタンがありませんでした\n" @*/, ID )) ;
		return -1;
	}
	but = &WinData.ToolBarItem[i] ;

	// ボタンの状態を保存
	but->State = State ;
	but->Click = FALSE ;

	// ボタンの状態を設定する
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

	// 終了
	return 0 ;
}

// ツールバーのボタンを全て削除
extern int NS_DeleteAllToolBarButton( void )
{
	int i ;

	// ツールバーのセットアップが完了していない場合は何もせずに終了
	if( WinData.ToolBarUseFlag == FALSE ) return -1 ;

	// ボタンの数だけ削除メッセージを送る
	for( i = 0 ; i < WinData.ToolBarItemNum ; i ++ )
		SendMessageW( WinData.ToolBarHandle, TB_DELETEBUTTON, 0, 0 ) ;

	// ボタンの数を０にする
	WinData.ToolBarItemNum = 0 ;

	// 終了
	return 0 ;
}

















// メニュー関係

// メニューを有効にするかどうかを設定する
extern int NS_SetUseMenuFlag( int Flag )
{
	// もし今までと同じ場合は何もせず終了
	if( Flag == WinData.MenuUseFlag ) return 0 ;

	// フラグによって処理を分岐
	if( Flag == FALSE )
	{
		// メニューを削除する
		if( WinData.Menu != NULL )
		{
			// メニューをウインドウから外す
			NS_SetDisplayMenuFlag( FALSE ) ;

			// メニューを削除
			DestroyMenu( WinData.Menu ) ;
			WinData.Menu = NULL ;
		}
		
		// メニューを無効にする
		WinData.MenuUseFlag = FALSE ;
	}
	else
	{
		// メニューが無い場合は空のメニューを作成する
		if( WinData.Menu == NULL )
		{
			WinData.Menu = CreateMenu() ;
			if( WinData.Menu == NULL ) return -1 ;
		}
		
		// メニューを有効にする
		WinData.MenuUseFlag = TRUE ;
		
		// 初期状態で表示
		WinData.MenuDisplayState = FALSE ;
		NS_SetDisplayMenuFlag( TRUE ) ;
	}

	// 選択されたメニュー項目の数を初期化
	WinData.SelectMenuItemNum = 0 ;

	// メニュー項目の数を初期化する
	WinData.MenuItemInfoNum = 0 ;

	// 終了
	return 0 ;
}

// キーボードアクセラレーターを使用するかどうかを設定する
extern int NS_SetUseKeyAccelFlag( int Flag )
{
	if( WinData.AltF4_EndFlag == TRUE ) Flag = FALSE ;

	WinData.UseAccelFlag = Flag ;
	if( Flag == FALSE ) NS_ClearKeyAccel() ;

	// 終了
	return 0 ;
}

// ショートカットキーを追加する
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

// ショートカットキーを追加する
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

	// 指定の選択項目が見つからなかったら何もしない
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

	// 既にアクセラレーターが設定されているかどうかで処理を分岐
	if( WinData.Accel != NULL )
	{
		// 既に設定されている場合は現在のアクセラレーターの情報を取得する
		AccelNum = CopyAcceleratorTableW( WinData.Accel, NULL, 0 ) ;

		// データが一つ増えるので数を一つ増やす
		AccelNum ++ ;

		// データを格納するためのメモリ領域を確保
		Accel = (ACCEL *)DXALLOC( sizeof( ACCEL ) * AccelNum ) ;
		if( Accel == NULL ) goto ERR ;

		// 現在のデータを取得する
		CopyAcceleratorTableW( WinData.Accel, Accel, AccelNum ) ;
	}
	else
	{
		// データがひとつだけ入るメモリ領域を確保
		AccelNum = 1 ;
		Accel = (ACCEL *)DXALLOC( sizeof( ACCEL ) * AccelNum ) ;
		if( Accel == NULL ) goto ERR ;
	}

	// アクセラレータの情報を作成
	ac = &Accel[AccelNum-1] ;
	ac->fVirt = FNOINVERT | FVIRTKEY ;
	if( CtrlFlag != FALSE ) ac->fVirt |= FCONTROL ;
	if( AltFlag != FALSE ) ac->fVirt |= FALT ;
	if( ShiftFlag != FALSE ) ac->fVirt |= FSHIFT ;
	ac->key = (WORD)NS_ConvertKeyCodeToVirtualKey( KeyCode ) ;
	ac->cmd = (WORD)WinItemInfo->ID ;

	// アクセラレータを作成
	NewAccel = CreateAcceleratorTableW( Accel, AccelNum ) ;
	if( NewAccel == NULL ) goto ERR ;

	// メモリの解放
	_MEMSET( Accel, 0, sizeof( ACCEL ) * AccelNum ) ;
	DXFREE( Accel ) ;

	// 作成に成功したら現在のアクセラレータを削除する
	if( WinData.Accel != NULL )
		DestroyAcceleratorTable( WinData.Accel ) ;

	// 新しいアクセラレータをセットする
	WinData.Accel = NewAccel ;

	// 終了
	return 0 ;
	
ERR:
	return -1 ;
#endif // DX_NON_INPUT
}

// ショートカットキーを追加する
extern int NS_AddKeyAccel_Name( const TCHAR *ItemName, int KeyCode, int CtrlFlag, int AltFlag, int ShiftFlag )
{
	return NS_AddKeyAccel( ItemName, 0, KeyCode, CtrlFlag, AltFlag, ShiftFlag ) ;
}

// ショートカットキーを追加する
extern int AddKeyAccel_Name_WCHAR_T( const wchar_t *ItemName , int KeyCode , int CtrlFlag , int AltFlag , int ShiftFlag )
{
	return AddKeyAccel_WCHAR_T( ItemName, 0, KeyCode, CtrlFlag, AltFlag, ShiftFlag ) ;
}

// ショートカットキーを追加する
extern int NS_AddKeyAccel_ID( int ItemID, int KeyCode, int CtrlFlag, int AltFlag, int ShiftFlag )
{
	return NS_AddKeyAccel( NULL, ItemID, KeyCode, CtrlFlag, AltFlag, ShiftFlag ) ;
}

// ショートカットキーの情報を初期化する
extern int NS_ClearKeyAccel( void )
{
	// アクセラレータが有効になっていた場合は削除する
	if( WinData.Accel != NULL )
	{
		DestroyAcceleratorTable( WinData.Accel ) ;
		WinData.Accel = NULL ;
	}

	// 終了
	return 0 ;
}

// メニューに項目を追加する
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

// メニューに項目を追加する
extern int AddMenuItem_WCHAR_T( int AddType, const wchar_t *ItemName, int ItemID,
						int SeparatorFlag, const wchar_t *NewItemName, int NewItemID )
{
	HMENU Menu = NULL ;
	int AddIndex = 0 ;
	MENUITEMINFOW ItemInfo ;
	WINMENUITEMINFO *WItemInfo ;

	if( WinData.AltF4_EndFlag != 0 ) return -1 ;

	// 情報が一杯だったらエラー
	if( WinData.MenuItemInfoNum == MAX_MENUITEM_NUM ) return -1 ;

	// メニューが無効だったら有効にする
	if( WinData.MenuUseFlag == FALSE ) NS_SetUseMenuFlag( TRUE ) ;

	// 追加のタイプによって処理を分岐
	switch( AddType )
	{
	case MENUITEM_ADD_CHILD :	// 指定の項目の子として追加する場合
		{
			// 親の指定があるかどうかで処理を分岐
			if( ItemID == MENUITEM_IDTOP )
			{
				Menu = WinData.Menu ;
			}
			else
			{
				// 指定の選択項目のサブメニューを取得する(無かったら作成する)
				Menu = MenuItemSubMenuSetup( ItemName, ItemID ) ;

				// 指定の選択項目が無かったら一番上に追加する
				if( Menu == NULL )
				{
					Menu = WinData.Menu ;
				}
			}

			// 追加先をサブメニュー内の末尾にする
			AddIndex = GetMenuItemCount( Menu ) ;
		}
		break ;
		
	case MENUITEM_ADD_INSERT :	// 指定の項目と指定の項目より一つ上の項目の間に追加する場合
		{
			WINMENUITEMINFO *WinItemInfo ;

			// 指定の項目の情報を取得する
			WinItemInfo = SearchMenuItemInfo( ItemName, ItemID ) ;
			
			// 無かったら選択項目の一番上に追加する
			if( WinItemInfo == NULL )
			{
				Menu = WinData.Menu ;
				AddIndex = GetMenuItemCount( Menu ) ;
			}
			else
			{
				// 在ったらその項目とその項目より一つ上の項目の間に追加する
				Menu = WinItemInfo->Menu ;
				AddIndex = WinItemInfo->Index ;
			}
		}
		break ;
	}

	// 区切り線かどうかで処理を分岐
	if( SeparatorFlag == TRUE )
	{
		// 区切り線の場合

		// 区切り線の情報をセットする
		_MEMSET( &ItemInfo, 0, sizeof( ItemInfo ) ) ;
		ItemInfo.cbSize = sizeof( MENUITEMINFOW ) ;	// 構造体のサイズ
		ItemInfo.fMask = MIIM_TYPE ;				// 取得または設定するメンバ
		ItemInfo.fType = MFT_SEPARATOR ; 			// アイテムのタイプ

		// 区切り線項目の追加
		if( InsertMenuItemW( Menu, ( UINT )AddIndex, TRUE, &ItemInfo ) == 0 )
		{
			return -1 ;
		}
	}
	else
	{
		// 選択項目の場合

		// 新しいアイテムのＩＤが－１だった場合使われていないＩＤを付ける
		if( NewItemID == -1 )
			NewItemID = GetNewMenuItemID() ;
		WItemInfo = &WinData.MenuItemInfo[ WinData.MenuItemInfoNum ] ;

		// 選択項目を追加する
		{
			// 新しい項目の情報をセットする
			_MEMSET( &ItemInfo, 0, sizeof( ItemInfo ) ) ;
			ItemInfo.cbSize     = sizeof( MENUITEMINFOW ) ;				// 構造体のサイズ
			ItemInfo.fMask      = MIIM_STATE | MIIM_TYPE | MIIM_ID ;	// 取得または設定するメンバ
			ItemInfo.fType      = MFT_STRING ;							// アイテムのタイプ
			ItemInfo.fState     = MFS_ENABLED ;							// アイテムの状態
			ItemInfo.wID        = ( UINT )NewItemID ;					// アイテムID
			_WCSCPY_S( WItemInfo->Name, sizeof( WItemInfo->Name ), NewItemName ) ;
			ItemInfo.dwTypeData = WItemInfo->Name ;						// アイテムの内容セット
			ItemInfo.cch        = ( UINT )_WCSLEN( WItemInfo->Name ) ;		// アイテムの文字列の長さ

			// 項目の追加
			if( InsertMenuItemW( Menu, ( UINT )AddIndex, TRUE, &ItemInfo ) == 0 )
			{
				return -1 ;
			}
		}

		// メニュー項目の情報を追加する
		AddMenuItemInfo( Menu, AddIndex, NewItemID, NewItemName ) ;
	}

	// 一番最初のメニューバーへの項目追加の場合はウインドウサイズを調整する
	if( WinData.WindowModeFlag == TRUE && Menu == WinData.Menu && GetMenuItemCount( WinData.Menu ) == 1 )
	{
		SetWindowStyle( FALSE ) ;
	}

	// メニューを再描画する
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

	// 挿入の場合はリストを再構築する
	if( AddType == MENUITEM_ADD_INSERT )
	{
		WinData.MenuItemInfoNum = 0 ;
		ListupMenuItemInfo( WinData.Menu ) ;
	}

	// 終了
	return 0 ;
}

// メニューから項目を削除する
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

// メニューから項目を削除する
extern int DeleteMenuItem_WCHAR_T( const wchar_t *ItemName, int ItemID )
{
	HMENU Menu ;
	int Index ;

	if( WinData.MenuUseFlag == FALSE )
	{
		return -1 ;
	}

	// 指定の項目を探す
	if( SearchMenuItem( ItemName, ItemID, WinData.Menu, &Menu, &Index ) != 1 )
	{
		return -1 ;
	}

	// 選択項目を削除する
	DeleteMenu( Menu, ( UINT )Index, MF_BYPOSITION ) ;

	// リストを再構築する
	WinData.MenuItemInfoNum = 0 ;
	ListupMenuItemInfo( WinData.Menu ) ;

	// メニューを再描画する
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

	// 終了
	return 0 ;
}

// メニューが選択されたかどうかを取得する( 0:選択されていない  1:選択された )
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

// メニューが選択されたかどうかを取得する( 0:選択されていない  1:選択された )
extern int CheckMenuItemSelect_WCHAR_T( const wchar_t *ItemName, int ItemID )
{
	int i, InfoNum ;

	if( WinData.MenuUseFlag == FALSE ) return -1 ;

	// 名前が指定されていた場合はＩＤを取得する
	if( ItemName != NULL )
	{
		ItemID = GetMenuItemID_WCHAR_T( ItemName ) ;
	}

	// 指定のアイテムが存在するか調べる
	InfoNum = WinData.SelectMenuItemNum ;
	for( i = 0 ; i < InfoNum ; i ++ )
		if( WinData.SelectMenuItem[i] == ItemID ) break ;

	// 無かったら０を返す
	if( i == InfoNum ) return 0 ;
	
	// 在ったら、情報から外す
	if( i != InfoNum - 1 )
		_MEMMOVE( &WinData.SelectMenuItem[i], &WinData.SelectMenuItem[i+1], sizeof( int ) * ( InfoNum - i - 1 ) ) ;
	
	// 情報の数を減らす
	WinData.SelectMenuItemNum -- ;

	// あったので１を返す
	return 1 ;
}

// メニューの全ての選択項目を削除する
extern int NS_DeleteMenuItemAll( void )
{
	HMENU NewMenu ;
	int DispFlag ;

	if( WinData.MenuUseFlag == FALSE ) return -1 ;
	
	// 新しいメニューを作成
	NewMenu = CreateMenu() ;
	if( NewMenu == NULL ) return -1 ;

	DispFlag = NS_GetDisplayMenuFlag() ;
	
	// 大本のメニューを削除して作り直す
	if( WinData.Menu != NULL )
	{
		// とりあえずウインドウから外す
		NS_SetDisplayMenuFlag( FALSE ) ;
		
		// フルスクリーンの場合
		if( WinData.WindowModeFlag == FALSE )
		{
			// メニューを外す
			if( WinData.MenuSetupFlag == TRUE )
			{
				SetMenu( WinData.MainWindow, NULL ) ;
				WinData.MenuSetupFlag = FALSE ;
			}
		}

		// メニューを削除
		DestroyMenu( WinData.Menu ) ;
	}

	// 新しいメニューをセット
	WinData.Menu = NewMenu ;

	// 表示状態を元に戻す
	NS_SetDisplayMenuFlag( DispFlag ) ;

	// フルスクリーンの場合
	if( WinData.WindowModeFlag == FALSE )
	{
		// メニューのセット
		if( WinData.MenuSetupFlag == FALSE && WinData.MenuDisplayState != FALSE )
		{
			SetMenu( WinData.MainWindow, WinData.Menu ) ;
			WinData.MenuSetupFlag = TRUE ;
		}
	}

	// 選択されたメニュー項目の数を初期化
	WinData.SelectMenuItemNum = 0 ;

	// リストも初期化
	WinData.MenuItemInfoNum = 0 ;

	// 終了
	return 0 ;
}

// メニューが選択されたかどうかの情報を初期化
extern int NS_ClearMenuItemSelect( void )
{
	WinData.SelectMenuItemNum = 0 ;

	// 終了
	return 0 ;
}

// メニューの項目を選択出来るかどうかを設定する
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

// メニューの項目を選択出来るかどうかを設定する
extern int SetMenuItemEnable_WCHAR_T( const wchar_t *ItemName, int ItemID, int EnableFlag )
{
	WINMENUITEMINFO *WinItemInfo ;
	
	if( WinData.MenuUseFlag == FALSE ) return -1 ;

	// 選択項目の情報を取得
	WinItemInfo = SearchMenuItemInfo( ItemName, ItemID ) ;
	if( WinItemInfo == NULL ) return -1 ;

	// 選択出来るかどうかを設定する
	EnableMenuItem( WinItemInfo->Menu, ( UINT )WinItemInfo->Index, ( UINT )( MF_BYPOSITION | ( EnableFlag == TRUE ? MF_ENABLED : MF_GRAYED ) ) ) ;

	// 表示を更新する
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

	// 終了
	return 0 ;
}

// メニューの項目にチェックマークやラジオボタンを表示するかどうかを設定する
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

// メニューの項目にチェックマークやラジオボタンを表示するかどうかを設定する
extern int SetMenuItemMark_WCHAR_T( const wchar_t *ItemName, int ItemID, int Mark )
{
	MENUITEMINFOW ItemInfo ;
	WINMENUITEMINFO *WinItemInfo ;

	if( WinData.MenuUseFlag == FALSE ) return -1 ;
	
	// 選択項目の情報を取得
	WinItemInfo = SearchMenuItemInfo( ItemName, ItemID ) ;
	if( WinItemInfo == NULL ) return -1 ;

	// 現在の情報を得る
	_GetMenuItemInfo( WinItemInfo->Menu, WinItemInfo->Index, &ItemInfo ) ;
	
	// 情報を書き換える
	switch( Mark )
	{
	case MENUITEM_MARK_NONE :	// 何も付けない
		ItemInfo.fMask |= MIIM_TYPE ;
		ItemInfo.fState &= ~MFS_CHECKED ;
		ItemInfo.fType &= ~MFT_RADIOCHECK ;
		break ;
		
	case MENUITEM_MARK_CHECK :	// 普通のチェック
		ItemInfo.fMask |= MIIM_TYPE ;
		ItemInfo.fType &= ~MFT_RADIOCHECK ;
		ItemInfo.fState |= MFS_CHECKED ;
		ItemInfo.hbmpChecked = NULL ;
		break ;

	case MENUITEM_MARK_RADIO :	// ラジオボタン
		ItemInfo.fMask |= MIIM_TYPE ;
		ItemInfo.fType = MFT_RADIOCHECK ;
		ItemInfo.fState |= MFS_CHECKED ;
		ItemInfo.hbmpChecked = NULL ;
		break ;
	}
	SetMenuItemInfoW( WinItemInfo->Menu, ( UINT )WinItemInfo->Index, TRUE, &ItemInfo ) ;
	
	// 表示を更新する
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

	// 終了
	return 0 ;
}

// メニューの項目がどれか選択されたかどうかを取得する( 戻り値  TRUE:どれか選択された  FALSE:選択されていない )
extern int NS_CheckMenuItemSelectAll( void )
{
	return WinData.SelectMenuItemNum != 0 ? TRUE : FALSE ;
}

// メニューに項目を追加する
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

// メニューに項目を追加する
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

// 指定の項目と、指定の項目の一つ上の項目との間に新しい項目を追加する
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

// 指定の項目と、指定の項目の一つ上の項目との間に新しい項目を追加する
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

// メニューのリストに区切り線を追加する
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

// メニューのリストに区切り線を追加する
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

// 指定の項目と、指定の項目の一つ上の項目との間に区切り線を追加する
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

// 指定の項目と、指定の項目の一つ上の項目との間に区切り線を追加する
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

// メニューから項目を削除する
extern int NS_DeleteMenuItem_Name( const TCHAR *ItemName )
{
	return NS_DeleteMenuItem( ItemName, -1 ) ;
}

// メニューから項目を削除する
extern int DeleteMenuItem_Name_WCHAR_T( const wchar_t *ItemName )
{
	return DeleteMenuItem_WCHAR_T( ItemName, -1 ) ;
}

// メニューが選択されたかどうかを取得する( 0:選択されていない  1:選択された )
extern int NS_CheckMenuItemSelect_Name( const TCHAR *ItemName )
{
	return NS_CheckMenuItemSelect( ItemName, -1 ) ;
}

// メニューが選択されたかどうかを取得する( 0:選択されていない  1:選択された )
extern int CheckMenuItemSelect_Name_WCHAR_T( const wchar_t *ItemName )
{
	return CheckMenuItemSelect_WCHAR_T( ItemName, -1 ) ;
}

// メニューの項目を選択出来るかどうかを設定する
extern int NS_SetMenuItemEnable_Name( const TCHAR *ItemName, int EnableFlag )
{
	return NS_SetMenuItemEnable( ItemName, -1, EnableFlag ) ;
}

// メニューの項目を選択出来るかどうかを設定する
extern int SetMenuItemEnable_Name_WCHAR_T( const wchar_t *ItemName, int EnableFlag )
{
	return SetMenuItemEnable_WCHAR_T( ItemName, -1, EnableFlag ) ;
}

// メニューの項目にチェックマークやラジオボタンを表示するかどうかを設定する
extern int NS_SetMenuItemMark_Name( const TCHAR *ItemName, int Mark )
{
	return NS_SetMenuItemMark( ItemName, -1, Mark ) ;
}

// メニューの項目にチェックマークやラジオボタンを表示するかどうかを設定する
extern int SetMenuItemMark_Name_WCHAR_T( const wchar_t *ItemName, int Mark )
{
	return SetMenuItemMark_WCHAR_T( ItemName, -1, Mark ) ;
}


// メニューに項目を追加する
extern int NS_AddMenuItem_ID( int ParentItemID, const TCHAR *NewItemName, int NewItemID )
{
	return NS_AddMenuItem( MENUITEM_ADD_CHILD, NULL, ParentItemID,
							FALSE, NewItemName, NewItemID ) ;
}

// メニューに項目を追加する
extern int AddMenuItem_ID_WCHAR_T( int ParentItemID, const wchar_t *NewItemName, int NewItemID )
{
	return AddMenuItem_WCHAR_T( MENUITEM_ADD_CHILD, NULL, ParentItemID,
							FALSE, NewItemName, NewItemID ) ;
}

// 指定の項目と、指定の項目の一つ上の項目との間に新しい項目を追加する
extern int NS_InsertMenuItem_ID( int ItemID, int NewItemID )
{
	return NS_AddMenuItem( MENUITEM_ADD_INSERT, NULL, ItemID,
							FALSE, NULL, NewItemID ) ;
}

// メニューのリストに区切り線を追加する
extern int NS_AddMenuLine_ID( int ParentItemID )
{
	return NS_AddMenuItem( MENUITEM_ADD_CHILD, NULL, ParentItemID,
							TRUE, NULL, -1 ) ;
}

// 指定の項目と、指定の項目の一つ上の項目との間に区切り線を追加する
extern int NS_InsertMenuLine_ID( int ItemID, int NewItemID )
{
	return NS_AddMenuItem( MENUITEM_ADD_INSERT, NULL, ItemID,
							TRUE, NULL, NewItemID ) ;
}

// メニューから項目を削除する
extern int NS_DeleteMenuItem_ID( int ItemID )
{
	return NS_DeleteMenuItem( NULL, ItemID ) ;
}

// メニューが選択されたかどうかを取得する( 0:選択されていない  1:選択された )
extern int NS_CheckMenuItemSelect_ID( int ItemID )
{
	return NS_CheckMenuItemSelect( NULL, ItemID ) ;
}

// メニューの項目を選択出来るかどうかを設定する
extern int NS_SetMenuItemEnable_ID( int ItemID, int EnableFlag )
{
	return NS_SetMenuItemEnable( NULL, ItemID, EnableFlag ) ;
}

// メニューの項目にチェックマークやラジオボタンを表示するかどうかを設定する
extern int NS_SetMenuItemMark_ID( int ItemID, int Mark )
{
	return NS_SetMenuItemMark( NULL, ItemID, Mark ) ;
}


// メニューの項目名からＩＤを取得する
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

// メニューの項目名からＩＤを取得する
extern int GetMenuItemID_WCHAR_T( const wchar_t *ItemName )
{
	WINMENUITEMINFO *WinItemInfo ;
	int Result ;

	if( WinData.MenuUseFlag == FALSE ) return -1 ;
	
	// 選択項目の情報を取得
	WinItemInfo = SearchMenuItemInfo( ItemName, -1 ) ;
	if( WinItemInfo == NULL ) return -1 ;
	Result = (int)WinItemInfo->ID ;
	
	// ＩＤを返す
	return Result ;
}

// メニューのＩＤから項目名を取得する
extern int NS_GetMenuItemName( int ItemID, TCHAR *NameBuffer )
{
#ifdef UNICODE
	return GetMenuItemName_WCHAR_T( ItemID, NameBuffer ) ;
#else
	int Result ;
	MENUITEMINFOW ItemInfo ;
	WINMENUITEMINFO *WinItemInfo ;

	if( WinData.MenuUseFlag == FALSE ) return -1 ;
	
	// 親のＩＤを持つ選択項目の情報を取得		
	WinItemInfo = SearchMenuItemInfo( NULL, ItemID ) ;
	if( WinItemInfo == NULL ) return -1 ;

	// 現在の情報を得る
	Result = _GetMenuItemInfo( WinItemInfo->Menu, WinItemInfo->Index, &ItemInfo ) ;
	if( Result != 0 ) return -1 ;

	// 項目名をコピー
	ConvString( ( const char * )ItemInfo.dwTypeData, WCHAR_T_CHARCODEFORMAT, ( char * )NameBuffer, BUFFERBYTES_CANCEL, _TCHARCODEFORMAT ) ;

	// 終了
	return 0 ;
#endif
}

// メニューのＩＤから項目名を取得する
extern int GetMenuItemName_WCHAR_T( int ItemID, wchar_t *NameBuffer )
{
	int Result ;
	MENUITEMINFOW ItemInfo ;
	WINMENUITEMINFO *WinItemInfo ;

	if( WinData.MenuUseFlag == FALSE ) return -1 ;
	
	// 親のＩＤを持つ選択項目の情報を取得		
	WinItemInfo = SearchMenuItemInfo( NULL, ItemID ) ;
	if( WinItemInfo == NULL ) return -1 ;

	// 現在の情報を得る
	Result = _GetMenuItemInfo( WinItemInfo->Menu, WinItemInfo->Index, &ItemInfo ) ;
	if( Result != 0 ) return -1 ;

	// 項目名をコピー
	_WCSCPY( NameBuffer, ItemInfo.dwTypeData ) ;

	// 終了
	return 0 ;
}

// メニューをリソースから読み込む
extern int NS_LoadMenuResource( int MenuResourceID )
{
	HMENU Menu ;

	// 指定のリソースを読み込む
	Menu = LoadMenuW( 
		WinData.LoadResourModule == NULL ? WinAPIData.Win32Func.GetModuleHandleWFunc( NULL ) : WinData.LoadResourModule,
		MAKEINTRESOURCEW( MenuResourceID )
	) ;
	if( Menu == NULL ) return -1 ;
	
	// メニューを有効にする
	NS_SetUseMenuFlag( TRUE ) ;
	
	// 既に作成されているメニューを削除する
	if( WinData.Menu != NULL )
	{
		// メニューをウインドウから外す
		NS_SetDisplayMenuFlag( FALSE ) ;

		// メニューを削除
		DestroyMenu( WinData.Menu ) ;
		WinData.Menu = NULL ;
	}

	// 新しいメニューをセット
	WinData.Menu = Menu ;
	
	// メニューを表示状態にセット
	WinData.MenuDisplayState = FALSE ;
	NS_SetDisplayMenuFlag( TRUE ) ;

	// 選択項目の情報を更新する
	WinData.MenuItemInfoNum = 0 ;
	ListupMenuItemInfo( WinData.Menu ) ;

	// 終了
	return 0 ;
}

// メニューの項目が選択されたときに呼ばれるコールバック関数を設定する
extern int NS_SetMenuItemSelectCallBackFunction( void (*CallBackFunction)( const TCHAR *ItemName, int ItemID ) )
{
	WinData.MenuCallBackFunction = CallBackFunction ;

	// 終了
	return 0 ;
}

// メニュー構造の中から、選択項目のＩＤを元に位置情報を得る( -1:エラー  0:見つからなかった  1:見つかった )
static int SearchMenuItem( const wchar_t *ItemName, int ItemID, HMENU SearchMenu, HMENU *Menu, int *Index )
{
	int ItemNum, i ;
	int Result ;
	HMENU SubMenu ;
	MENUITEMINFOW ItemInfo ;

	// メニューが有効ではなかったら何もしない
	if( WinData.MenuUseFlag == FALSE ) return -1 ;

	// メニュー内の項目数を得る	
	ItemNum = GetMenuItemCount( SearchMenu ) ;
	if( ItemNum == -1 ) return -1 ;

	// メニュー内に指定のＩＤの選択項目が無いか調べる
	for( i = 0 ; i < ItemNum ; i ++ )
	{
		// 項目の情報を得る
		Result = _GetMenuItemInfo( SearchMenu, i, &ItemInfo ) ;
		
		// 区切り線だったらスキップ
		if( Result == 1 ) continue ;
		
		// 目的の項目だったらここで終了
		if( ItemName )
		{
			if( _WCSCMP( ItemInfo.dwTypeData, ItemName ) == 0 ) break ;
		}
		else
		{
			if( ItemInfo.wID == (UINT)ItemID ) break ;
		}

		// サブメニューを持っていたらそちらも検索する
		SubMenu = ItemInfo.hSubMenu ;
		if( SubMenu != NULL )
		{
			Result = SearchMenuItem( ItemName, ItemID, SubMenu, Menu, Index ) ;
			
			// エラーが発生するか、目的のＩＤが見つかったらここで終了
			if( Result == -1 || Result == 1 ) return Result ;
		}
	}
	
	// 在ったら情報を格納して関数を出る
	if( i != ItemNum )
	{
		*Menu = SearchMenu ;
		*Index = i ;
		
		// 成功終了は１
		return 1 ;
	}
	
	// 無かったら０を返す
	return 0 ;
}



// (古い関数)ウインドウにメニューを設定する
extern int NS_SetWindowMenu( int MenuID, int (*MenuProc)( WORD ID ) )
{
	int DispFlag ;

	DispFlag = NS_GetDisplayMenuFlag() ;

	// もし既にメニューがロードされていたらメニューを削除する
	if( WinData.Menu != NULL )
	{
		// メニューを除去
		NS_SetDisplayMenuFlag( FALSE ) ;

		// メニューを削除
		DestroyMenu( WinData.Menu ) ;
		WinData.Menu = NULL ;
	}

	// もしメニューＩＤが -1 だったらメニューを削除
	if( MenuID == -1 )
	{
		// メニューを無効にする
		WinData.MenuUseFlag = FALSE ;
	}
	else
	{
		// メニューをロード
		WinData.Menu = LoadMenuW( WinAPIData.Win32Func.GetModuleHandleWFunc( NULL ), MAKEINTRESOURCEW( MenuID ) ) ;
		if( WinData.Menu == NULL ) return -1 ;

		// メニューを有効にする
		WinData.MenuUseFlag = TRUE ;

		// メニューを表示状態を更新する
		NS_SetDisplayMenuFlag( DispFlag ) ;

		// コールバック関数のポインタを保存
		WinData.MenuProc = MenuProc ;
	}
	
	// 選択項目の情報を更新する
	WinData.MenuItemInfoNum = 0 ;
	ListupMenuItemInfo( WinData.Menu ) ;

	// 終了
	return 0 ;
}

// ウインドウメニュー表示開始に使用するキーを設定する
/*extern int SetWindowMenuStartKey( int KeyID )
{
	// キーを設定する
	WinData.MenuStartKey = KeyID ;

	// 終了
	return 0 ;
}
*/

// メニューを表示するかどうかをセットする
extern int NS_SetDisplayMenuFlag( int Flag )
{
	int DispState ;

	// メニューが有効ではない場合は何もしない
	if( WinData.MenuUseFlag == FALSE ) return 0 ;

	// フラグを保存
	WinData.NotMenuDisplayFlag = !Flag ;
	
	// ウインドウが作成されていない場合は何もしない
	if( WinData.MainWindow == NULL ) return 0 ;

	// 表示状態が同じ場合は何もせず終了
	DispState = GetDisplayMenuState() ;
	if( DispState == WinData.MenuDisplayState ) return 0 ;

	// メニューを表示すべきかどうかによって処理を分岐
	if( DispState == TRUE )
	{
		// 表示する場合の処理

		// メニューのセット
		if( WinData.MenuSetupFlag == FALSE )
		{
			SetMenu( WinData.MainWindow, WinData.Menu ) ;
			WinData.MenuSetupFlag = TRUE ;
		}

		// ウインドウの属性を変更
		if( WinData.WindowModeFlag )
		{
			SetWindowStyle( FALSE ) ;
		}
	}
	else
	{
		// 非表示にする場合の処理

		// メニューを外す
		if( WinData.MenuSetupFlag == TRUE )
		{
			SetMenu( WinData.MainWindow, NULL ) ;
			WinData.MenuSetupFlag = FALSE ;
		}

		// ウインドウの属性を変更
		if( WinData.WindowModeFlag )
		{
			SetWindowStyle( FALSE ) ;
		}
	}

	// メニューの再描画
	UpdateWindow( WinData.MainWindow ) ;
//	DrawMenuBar( WinData.MainWindow ) ;

	// フラグの保存
	WinData.MenuDisplayState = DispState ;

	// 終了
	return 0 ;
}

// メニューを表示しているかどうかを取得する
extern int NS_GetDisplayMenuFlag( void )
{
	return WinData.NotMenuDisplayFlag == FALSE ;
}

// メニューが表示するべきかどうかを取得する( FALSE:表示すべきでない  TRUE:表示すべき )
static int GetDisplayMenuState( void )
{
	if( WinData.MainWindow == NULL ) return FALSE ;
	if( WinData.MenuUseFlag == FALSE ) return FALSE ;
	if( WinData.NotMenuDisplayFlag == TRUE ) return FALSE ;
	if( WinData.NotMenuAutoDisplayFlag == TRUE ) return TRUE ;
	if( WinData.WindowModeFlag == FALSE && WinData.MousePosInMenuBarFlag == FALSE ) return FALSE ;
	return TRUE ;
}

// メニューアイテムの情報を取得する( 0:正常終了  -1:エラー  1:区切り線 )
static int _GetMenuItemInfo( HMENU Menu, int Index, MENUITEMINFOW *Buffer )
{
	static wchar_t NameBuffer[128] ;

	// 区切り線かどうか調べる
	_MEMSET( Buffer, 0, sizeof( MENUITEMINFOW ) ) ;
	Buffer->cbSize = sizeof( MENUITEMINFOW ) ;
	Buffer->fMask = MIIM_TYPE ;
	if( GetMenuItemInfoW( Menu, ( UINT )Index, TRUE, Buffer ) == 0 ) return -1 ;

	// 区切り線だったら１を返す
	if( Buffer->fType & MFT_SEPARATOR ) return 1 ;

	// 区切り線ではなかったらそれ以外のステータスを取得する
	_MEMSET( Buffer, 0, sizeof( MENUITEMINFOW ) ) ;
	Buffer->cbSize = sizeof( MENUITEMINFOW ) ;
	Buffer->fMask = MIIM_STATE | MIIM_ID | MIIM_SUBMENU | MIIM_TYPE ;
	Buffer->fType = MFT_STRING ;
	Buffer->dwTypeData = NameBuffer ;
	Buffer->cch = 128 ;
	if( GetMenuItemInfoW( Menu, ( UINT )Index, TRUE, Buffer ) == 0 ) return -1 ;
	
	return 0 ;
}


// 指定の選択項目にサブメニューを付けられるように準備をする
static HMENU MenuItemSubMenuSetup( const wchar_t *ItemName, int ItemID )
{
	HMENU SubMenu ;
	MENUITEMINFOW ItemInfo ;
	WINMENUITEMINFO *WinItemInfo ;
	
	// 指定の項目の情報を得る
	WinItemInfo = SearchMenuItemInfo( ItemName, ItemID ) ;
	
	// 見つからなかったらエラー
	if( WinItemInfo == NULL ) return NULL ;

	// サブメニューが既にあるかどうか調べる
	SubMenu = GetSubMenu( WinItemInfo->Menu, WinItemInfo->Index ) ;

	// 既に在ったらそれを返す
	if( SubMenu != NULL ) return SubMenu ;

	// なかったらサブメニューを追加する
	SubMenu = CreateMenu() ;

	// 選択項目の情報にサブメニューを加える
	{
		// 現在の情報を取得
		_GetMenuItemInfo( WinItemInfo->Menu, WinItemInfo->Index, &ItemInfo ) ;
		
		// サブメニューを追加
		ItemInfo.hSubMenu = SubMenu ;

		// 新しい情報をセット
		SetMenuItemInfoW( WinItemInfo->Menu, ( UINT )WinItemInfo->Index, TRUE, &ItemInfo ) ;
	}

	// 作成したサブメニューを返す
	return SubMenu ;
}

// メニューを使用しているかどうかを得る
extern int NS_GetUseMenuFlag( void )
{
	return WinData.MenuUseFlag ;
}

// フルスクリーン時にメニューを自動で表示したり非表示にしたり
// するかどうかのフラグをセットする
extern int NS_SetAutoMenuDisplayFlag( int Flag )
{
	WinData.NotMenuAutoDisplayFlag = !Flag ;

	return 0 ;
}

// メニューの選択項目の情報の一覧を作成する時に使用する関数
static int ListupMenuItemInfo( HMENU Menu )
{
	int i, Num, Result ;
	MENUITEMINFOW ItemInfo ;
	WINMENUITEMINFO *WinItemInfo ;
	
	// 全ての項目のＩＤをリストに追加する
	Num = GetMenuItemCount( Menu ) ;
	if( Num == -1 ) return -1 ;
	
	for( i = 0 ; i < Num ; i ++ )
	{
		// 項目の情報を取得
		Result = _GetMenuItemInfo( Menu, i, &ItemInfo ) ;
		
		// エラーが発生したら終了
		if( Result == -1 ) return -1 ;
		
		// 区切り線以外だったら情報を追加
		if( Result != 1 )
		{
			WinItemInfo = &WinData.MenuItemInfo[ WinData.MenuItemInfoNum ] ;
			WinItemInfo->Menu  = Menu ;
			WinItemInfo->Index = ( short )i ;
			WinItemInfo->ID    = ( unsigned short )ItemInfo.wID ;
			_WCSCPY_S( WinItemInfo->Name, sizeof( WinItemInfo->Name ), ItemInfo.dwTypeData ) ;

			WinData.MenuItemInfoNum ++ ;

			// サブメニューがあったらそれも処理
			if( ItemInfo.hSubMenu != NULL )
				ListupMenuItemInfo( ItemInfo.hSubMenu ) ;
		}
	}

	// 終了
	return 0 ;
}

// メニューの選択項目の情報を追加する
static int AddMenuItemInfo( HMENU Menu, int Index, int ID, const wchar_t *Name )
{
	WINMENUITEMINFO *ItemInfo ;

	// 情報が一杯だったらエラー
	if( WinData.MenuItemInfoNum == MAX_MENUITEM_NUM ) return -1 ;

	// 情報の追加
	ItemInfo = &WinData.MenuItemInfo[WinData.MenuItemInfoNum] ;
	ItemInfo->Menu  = Menu ;
	ItemInfo->Index = ( short )Index ;
	ItemInfo->ID    = ( unsigned short )ID ;
	_WCSCPY_S( ItemInfo->Name, sizeof( ItemInfo->Name ), Name ) ;

	// 数を増やす
	WinData.MenuItemInfoNum ++ ;
	
	// 終了
	return 0 ;
}

// メニューの選択項目の情報を削除する
static int DeleteMenuItemInfo( const wchar_t *Name, int ID )
{
	LONG_PTR Index ;
	WINMENUITEMINFO *WinItemInfo ;
	
	// 削除する項目の情報を得る
	WinItemInfo = SearchMenuItemInfo( Name, ID ) ;
	if( WinItemInfo == NULL ) return -1 ;
	
	// インデックスを算出
	Index = WinItemInfo - WinData.MenuItemInfo ;

	// 情報を詰める
	if( Index != ( LONG_PTR )( WinData.MenuItemInfoNum - 1 ) )
	{
		_MEMMOVE( &WinData.MenuItemInfo[Index],
				 &WinData.MenuItemInfo[Index+1],
				 sizeof( WINMENUITEMINFO ) * ( WinData.MenuItemInfoNum - Index - 1 ) ) ;
	}
	
	// 数を減らす
	WinData.MenuItemInfoNum -- ;
	
	// 終了
	return 0 ;
}

// メニューの選択項目の情報を取得する
static WINMENUITEMINFO *SearchMenuItemInfo( const wchar_t *Name, int ID )
{
	int i, ItemInfoNum ;
	WINMENUITEMINFO *WinItemInfo ;

	ItemInfoNum = WinData.MenuItemInfoNum ;
	WinItemInfo = WinData.MenuItemInfo ;

	// 名前が有効な場合は名前で調べる
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

	// 見つからなかった場合はＮＵＬＬを返す
	if( i == ItemInfoNum ) return NULL ;

	// 正常終了
	return WinItemInfo ;
}

// 新しい選択項目のＩＤを取得する
static int GetNewMenuItemID( void )
{
	int NewItemID ;

	NewItemID = WinData.MenuItemInfoNum ;
	for(;;)
	{
		// 同じＩＤが無かったらループを抜ける
		if( SearchMenuItemInfo( NULL, NewItemID ) == NULL ) break ;

		// 在ったら次のＩＤを試す
		NewItemID ++ ;
		
		// ＩＤの有効範囲を超えたら０にする
		if( NewItemID == 0x10000 ) NewItemID = 0 ;
	}

	// 新しいＩＤを返す
	return NewItemID ;
}

// メニューを自動的に表示したり非表示にしたりする処理を行う
extern int MenuAutoDisplayProcess( void )
{
//	MENUBARINFO MBInfo ;
	int MenuBarHeight ;
	int MouseX, MouseY ;

	// ウインドウモードの場合は何もしない
	if( WinData.WindowModeFlag == TRUE ) return 0 ;

	// メニューバーの高さを得る
/*	_MEMSET( &MBInfo, 0, sizeof( MBInfo ) ) ;
	MBInfo.cbSize = sizeof( MBInfo ) ;
	GetMenuBarInfo( WinData.MainWindow, OBJID_MENU, 0, &MBInfo ) ;
	MenuBarHeight = MBInfo.rcBar.bottom - MBInfo.rcBar.top ;
*/
	MenuBarHeight = 60 ;

	// マウスポインタの座標を得る
	NS_GetMousePoint( &MouseX, &MouseY ) ;

#ifndef DX_NON_GRAPHICS
	// Direct3D9 を使用している場合はメニュー位置より下に来たらマウスポインタの表示状態を更新する
	if( GSYS.Setting.ValidHardware )
	{
		// Direct3D9 を使用している場合はメニューが存在する場合は
		// フルスクリーンモードでも常にセットしておく
		if( WinData.Menu && WinData.MenuSetupFlag == FALSE )
		{
			SetMenu( WinData.MainWindow, WinData.Menu ) ;
			WinData.MenuSetupFlag = TRUE ;
		}

		// Ｙ座標が規定以上になったら非表示にする
		if( MouseY > MenuBarHeight )
		{
			NS_SetMouseDispFlag( NS_GetMouseDispFlag() ) ;
		}
	}
	else
#endif // DX_NON_GRAPHICS
	{
		// マウスポインタがメニューの位置にある場合はメニューバーを表示する
		WinData.MousePosInMenuBarFlag = TRUE ;
		if( MouseY < MenuBarHeight )
			WinData.MousePosInMenuBarFlag = TRUE ;
		else
			WinData.MousePosInMenuBarFlag = FALSE ;

#ifndef DX_NON_GRAPHICS
		// ＧＤＩを使用するかどうかをセットする
		NS_SetUseGDIFlag( NS_GetUseGDIFlag() ) ;
#endif // DX_NON_GRAPHICS

		// メニューバーとマウスの表示状態を更新する
		NS_SetMouseDispFlag( NS_GetMouseDispFlag() ) ;
		NS_SetDisplayMenuFlag( NS_GetDisplayMenuFlag() ) ;
	}

	// 終了
	return 0 ;	
}










// マウス関係関数

// マウスの表示フラグのセット
extern int NS_SetMouseDispFlag( int DispFlag )
{
	int DispState ;

	// 今までとフラグが同じで、且つメニューが非表示だった場合なにもせず終了
//	if( DispFlag == WinData.MouseDispFlag &&
//		GetDisplayMenuState() == 0 )
//	{
//		
//		
//		return 0 ;
//	}

	// マウスの表示フラグをセットしてメッセージを生成する
	if( DispFlag != -1 )
	{
		WinData.MouseDispFlag = DispFlag ;
	}

	// マウスを表示するかどうかを取得
	DispState = WinData.MouseDispFlag == TRUE || GetDisplayMenuState() == TRUE ;

	// マウスの表示状態が今までと同じ場合は何もしない
	if( DispFlag != -1 && DispState == WinData.MouseDispState ) return 0 ;

	// マウスの表示状態をセット
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

	// マウスのセット信号を出す
	PostMessageW( WinData.MainWindow, WM_SETCURSOR, ( WPARAM )WinData.MainWindow, 0 ) ;

	// マウスの表示状態を保存する
	WinData.MouseDispState = DispState ;

	// 終了
	return 0 ;
}

// スクリーン座標をＤＸライブラリ画面座標に変換する
static int ConvScreenPositionToDxScreenPosition( int ScreenX, int ScreenY, int *DxScreenX, int *DxScreenY )
{
	double ExRateX ;
	double ExRateY ;
#ifndef DX_NON_GRAPHICS
	int SubBackBufferX ;
	int SubBackBufferY ;
#endif // DX_NON_GRAPHICS

	// ウインドウモードの場合クライアント領域内に座標を変換
	if( WinData.WindowModeFlag )
	{
		ScreenX -= WinData.WindowRect.left ;
		ScreenY -= WinData.WindowRect.top + GetToolBarHeight() ;

		// 画面が拡大されている場合はその影響を考慮する
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

	// 終了
	return 0 ;
}

// マウスの位置を取得する
extern int GetMousePoint_PF( int *XBuf, int *YBuf )
{
	POINT MousePos ;

	// スクリーン上での位置を取得
	GetCursorPos( &MousePos ) ; 

	// スクリーン座標をクライアント領域内の座標に変換
	ConvScreenPositionToDxScreenPosition( MousePos.x, MousePos.y, XBuf, YBuf ) ;

	// 終了
	return 0 ;
}

// マウスの位置をセットする
extern int SetMousePoint_PF( int PointX , int PointY )
{
	// ウインドウモードかどうかで処理を分岐
	if( WinData.WindowModeFlag )
	{
#ifndef DX_NON_GRAPHICS
		int ScreenSizeX , ScreenSizeY ;

		NS_GetDrawScreenSize( &ScreenSizeX , &ScreenSizeY ) ;

		// ウインドウのスケーリングに応じて座標を補正
		PointX = _DTOL( PointX * WinData.WindowSizeExRateX ) ;
		PointY = _DTOL( PointY * WinData.WindowSizeExRateY ) ;
		ScreenSizeX = _DTOL( ScreenSizeX * WinData.WindowSizeExRateX ) ;
		ScreenSizeY = _DTOL( ScreenSizeY * WinData.WindowSizeExRateY ) ;

		// 画面外にカーソルが出ていた場合の補正
		if( PointX < 0 ) 			PointX = 0 ;
		else
		if( PointX > ScreenSizeX )	PointX = ScreenSizeX ;

		if( PointY < 0 )			PointY = 0 ;
		else
		if( PointY > ScreenSizeY )	PointY = ScreenSizeY ;
#endif // DX_NON_GRAPHICS

		// ウインドウモードの場合の位置補正
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

	// 位置をセットする
	SetCursorPos( PointX , PointY ) ;

	// 終了
	return 0 ;
}

#ifndef DX_NON_INPUT

// マウスホイールの回転量を得る
extern int NS_GetMouseWheelRotVol( int CounterReset )
{
	int Vol ;

	// DirectInput を使用しているかどうかで処理を分岐
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

// 水平マウスホイールの回転量を得る
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

// マウスホイールの回転量を得る( 戻り値が float 型 )
extern float NS_GetMouseWheelRotVolF( int CounterReset )
{
	float Vol ;

	// DirectInput を使用しているかどうかで処理を分岐
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

// 水平マウスホイールの回転量を得る( 戻り値が float 型 )
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

// マウスの入力情報を保存する
static	void		StockMouseInputInfo( int Button )
{
	WINMOUSEINPUT *MInput ;

	// ログの数が最大数に達していたら何もせず終了
	if( WinData.MouseInputNum >= WIN_MOUSEINPUT_LOG_NUM ) return ;

	// 情報を保存
	MInput = &WinData.MouseInput[ WinData.MouseInputNum ] ;
	NS_GetMousePoint( &MInput->ClickX, &MInput->ClickY ) ;
	MInput->Button = Button ;

	// 情報の数を増やす
	WinData.MouseInputNum ++ ;
}

// マウス入力

// マウスのクリック情報を取得する( 戻り値  -1:ログが無かった )
extern int NS_GetMouseInputLog( int *Button, int *ClickX, int *ClickY, int LogDelete )
{
	WINMOUSEINPUT *MInput ;

	// ログが無かったら -1 を返す
	if( WinData.MouseInputNum == 0 ) return -1 ;

	// 情報を返す
	MInput = &WinData.MouseInput[ 0 ] ;
	if( Button ) *Button = MInput->Button ;
	if( ClickX ) *ClickX = MInput->ClickX ;
	if( ClickY ) *ClickY = MInput->ClickY ;

	// ログを削除する指定がある場合は削除する
	if( LogDelete )
	{
		WinData.MouseInputNum -- ;
		if( WinData.MouseInputNum )
			_MEMMOVE( WinData.MouseInput, &WinData.MouseInput[ 1 ], sizeof( WINMOUSEINPUT ) * WinData.MouseInputNum ) ;
	}

	// 終了
	return 0 ;
}

#endif // DX_NON_INPUT





































// 補助関数
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

// 指定の基本イメージを使用して UpdateLayeredWindow を行う
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

	// システムバーを持つウインドウスタイルかどうかを算出
	Systembar = ( WStyle_WindowModeTable[ WinData.WindowStyle ] & WS_CAPTION ) != 0;

	// 共通処理
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
	// Direct3DDC が有効かどうかで処理を分岐
	if( Direct3DDC )
	{
		WinAPIData.Win32Func.UpdateLayeredWindow( GetDisplayWindowHandle(), ddc, &wpos, &wsize, Direct3DDC, &pos, trans, &blend, ULW_COLORKEY ) ;
	}
	else
#endif // DX_NON_GRAPHICS
	{
		RECT SrcRect ;
		BASEIMAGE SrcImageTemp ;

		// SrcImage が NULL 以外で、且つ SrcImageRect が NULL 以外の場合は仮の BASEIMAGE を用意する
		if( SrcImage != NULL && SrcImageRect != NULL )
		{
			if( NS_DerivationBaseImage( SrcImage, SrcImageRect->left, SrcImageRect->top, SrcImageRect->right, SrcImageRect->bottom, &SrcImageTemp ) < 0 )
			{
				return -1 ;
			}

			SrcImage = &SrcImageTemp ;
		}

		// ビットマップのサイズが違ったらビットマップの作り直し
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

		// イメージのアドレスをセット
		Dst = ( BYTE * )WinData.BackBufferTransBitmapImage ;
		if( Systembar )
		{
			Dst += WinData.SystembarHeight * SrcImage->Width * 4 ;
		}
		Src = ( BYTE * )SrcImage->GraphData ;

		// ビットマップにデータを転送
		SrcRect.left = 0 ;
		SrcRect.top = 0 ;
		SrcRect.right = SrcImage->Width ;
		SrcRect.bottom = SrcImage->Height ;
		if( NotColorKey == FALSE )
		{
			// VISTA以降の場合とそれ以外で処理を分岐
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

		// 作成したビットマップを元に UpdateLeyeredWindow を行う
		memdc = CreateCompatibleDC( dc ) ;
		old = SelectObject( memdc, WinData.BackBufferTransBitmap ) ;

		// Vista 以降の場合は必ずアルファブレンドする
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


























// 情報出力系

// ＣＰＵＩＤ命令定義
#define CPUID	__asm _emit 0fH  __asm _emit 0A2H


// ＯＳやＤｉｒｅｃｔＸのバージョンを出力する
extern int OutSystemInfo( void )
{
	wchar_t Str[ 256 ] ;

	DXST_ERRORLOG_ADDUTF16LE( "\xb7\x30\xb9\x30\xc6\x30\xe0\x30\x6e\x30\xc5\x60\x31\x58\x92\x30\xfa\x51\x9b\x52\x57\x30\x7e\x30\x59\x30\x0a\x00\x00"/*@ L"システムの情報を出力します\n" @*/ ) ;
	DXST_ERRORLOG_TABADD ;

#ifndef DX_NON_LITERAL_STRING
	// ＤＸライブラリのバージョンを出力する
	{
		char UTF16LE_Buffer[ 128 ] ;
		char DestBuffer[ 128 ] ;
		ConvString( ( const char * )DXLIB_VERSION_STR_W, WCHAR_T_CHARCODEFORMAT, UTF16LE_Buffer, sizeof( UTF16LE_Buffer ), DX_CHARCODEFORMAT_UTF16LE ) ;
		CL_snprintf( DX_CHARCODEFORMAT_UTF16LE, TRUE, DX_CHARCODEFORMAT_SHIFTJIS, DX_CHARCODEFORMAT_UTF16LE, DestBuffer, sizeof( Str ) / 2, "\x24\xff\x38\xff\xe9\x30\xa4\x30\xd6\x30\xe9\x30\xea\x30\x20\x00\x56\x00\x65\x00\x72\x00\x25\x00\x73\x00\x0a\x00\x00"/*@ L"ＤＸライブラリ Ver%s\n" @*/, UTF16LE_Buffer ) ;

		DXST_ERRORLOG_ADDUTF16LE( DestBuffer ) ;
	}
#endif

	// ＣＰＵのコア数を取得する
	{
		SYSTEM_INFO SystemInfo ;

		GetSystemInfo( &SystemInfo ) ;
		WinData.ProcessorNum = ( int )SystemInfo.dwNumberOfProcessors ;

		DXST_ERRORLOGFMT_ADDUTF16LE(( "\xd6\x8a\x06\x74\xd7\x30\xed\x30\xbb\x30\xc3\x30\xb5\x30\x6e\x30\x70\x65\x20\x00\x3a\x00\x20\x00\x25\x00\x64\x00\x00"/*@ L"論理プロセッサの数 : %d" @*/, WinData.ProcessorNum )) ;
	}

	// ＯＳのバージョンを出力する
	{
		OSVERSIONINFOW OsVersionInfo ;

		DXST_ERRORLOG_ADDUTF16LE( "\x2f\xff\x33\xff\x20\x00\x20\x00\x00"/*@ L"ＯＳ  " @*/ ) ;
		_WCSCPY( Str, L"Windows" ) ;

		_MEMSET( &OsVersionInfo, 0, sizeof( OsVersionInfo ) ) ;
		OsVersionInfo.dwOSVersionInfoSize = sizeof( OsVersionInfo ) ;

		GetVersionExW( &OsVersionInfo ) ;

		// プラットフォームによって処理を分ける
		switch( OsVersionInfo.dwPlatformId )
		{
		// Windows3.1系(まずここにくることはないと思うが…)
		case VER_PLATFORM_WIN32s :
			_WCSCAT_S( Str, sizeof( Str ), L"3.1" ) ;
			WinData.WindowsVersion = DX_WINDOWSVERSION_31 ;
			break ;

		// Windows95, 98, Me
		case VER_PLATFORM_WIN32_WINDOWS :

			// マイナーバージョンによって分岐
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

		// NT 系
		case VER_PLATFORM_WIN32_NT :

			// バージョンが Windows8 以降の場合は分岐
			if( OsVersionInfo.dwMajorVersion >= 6 && OsVersionInfo.dwMinorVersion >= 2 &&
				( ( WinAPIData.Win32Func.VerifyVersionInfoWFunc != NULL && WinAPIData.Win32Func.VerSetConditionMaskFunc != NULL ) ||
				    WinAPIData.Win32Func.RtlGetVersionFunc      != NULL ) )
			{
				OSVERSIONINFOEXW OsVersionInfoW ;
				ULONGLONG condition = 0 ;

				_MEMSET( &OsVersionInfoW, 0, sizeof( OsVersionInfoW ) ) ;
				OsVersionInfoW.dwOSVersionInfoSize = sizeof( OsVersionInfoW ) ;

				// RtlGetVersion が使用できる場合は RtlGetVersion を使用する
				if( WinAPIData.Win32Func.RtlGetVersionFunc != NULL )
				{
					WinAPIData.Win32Func.RtlGetVersionFunc( &OsVersionInfoW ) ;

					_WCSCPY( OsVersionInfo.szCSDVersion, OsVersionInfoW.szCSDVersion ) ;
					OsVersionInfo.dwBuildNumber = OsVersionInfoW.dwBuildNumber ;

					// メジャーバージョンによって処理を分岐
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
					// 使用できない場合は VerifyVersionInfo を使用してバージョンをチェックする

					OsVersionInfo.dwBuildNumber = 0 ;
					OsVersionInfo.szCSDVersion[ 0 ] = '\0' ;

					for(;;)
					{
						// Windows10以降のチェック
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

						// Windows10のチェック
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

						// Windows8.1のチェック
						OsVersionInfoW.dwMajorVersion = 6 ;
						OsVersionInfoW.dwMinorVersion = 3 ;
						if( WinAPIData.Win32Func.VerifyVersionInfoWFunc( &OsVersionInfoW, VER_MAJORVERSION | VER_MINORVERSION, condition ) )
						{
							_WCSCAT_S( Str, sizeof( Str ), L"8.1" ) ;
							WinData.WindowsVersion = DX_WINDOWSVERSION_8_1 ;
							break ;
						}

						// ここにきたら Windows8
						_WCSCAT_S( Str, sizeof( Str ), L"8" ) ;
						WinData.WindowsVersion = DX_WINDOWSVERSION_8 ;
						break ;
					}
				}
			}
			else
			{
				// メジャーバージョンによって処理を分岐
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

	// ＤｉｒｅｃｔＸのバージョンを出力

	// 今時 DirectX 7.0 がインストールされていない環境はまず無いので、起動時間を短くする為にチェックはしない
	WinData.DirectXVersion = DX_DIRECTXVERSION_7 ;

	// タイマの精度を設定する
	{
		TIMECAPS tc ;
		WinAPIData.Win32Func.timeGetDevCapsFunc( &tc , sizeof(TIMECAPS) );

		// マルチメディアタイマーのサービス精度を最大に 
		WinAPIData.Win32Func.timeBeginPeriodFunc( tc.wPeriodMin ) ;
	}

	// CPU のチェック
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

			// ＣＰＵベンダ名を取得
			__cpuid( CPUInfo, 0 ) ;
			CPUCODE = CPUInfo[ 0 ] ;
			Str1 = CPUInfo[ 1 ] ;
			Str2 = CPUInfo[ 3 ] ;
			Str3 = CPUInfo[ 2 ] ;

			// パフォーマンスカウンタがつかえるかをチェック
			__cpuid( CPUInfo, 1 ) ;
			RDTSCUse = ( CPUInfo[ 3 ] & 0x10 ) != 0 ? TRUE : FALSE ;

			// 今度は拡張ＣＰＵＩＤが使えるか検査
			__cpuid( CPUInfo, 0x80000000 ) ;
			if( ( unsigned int )CPUInfo[ 0 ] >= 0x80000004 )
			{

				// 使えるのでＣＰＵ名を取得
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

			// 今度はＭＭＸが使えるか検査
			__cpuid( CPUInfo, 1 ) ;
			if( ( CPUInfo[ 3 ] & 0x00800000 ) != 0 )
			{
				ENDMODE = 3 ;

				//SSEが使えるか検査
				if( ( CPUInfo[ 3 ] & 0x02000000 ) != 0 )
				{
					ENDMODE	++ ;

					//SSE2が使えるか検査
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
			// CPUID が使えるか検査
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

			// 使えるのでＣＰＵベンダ名でもロード
			MOV		CPUCODE , EAX
			MOV		Str1, EBX
			MOV		Str2, EDX
			MOV		Str3, ECX
			MOV		ENDMODE , 1

			// ついでにパフォーマンスカウンタがつかえるかチェック
			MOV		EAX , 1
		}
//			db 0fh
//			db 0a2h
        cpuid
		__asm{
			AND		EDX , 16
			SHR		EDX , 4
			MOV		RDTSCUse , EDX

			// 今度は拡張ＣＰＵＩＤが使えるか検査
			MOV		EAX , 80000000H
//			db 0fh
//			db 0a2h
		}
        cpuid
		__asm{
			CMP		EAX , 80000004H
			JB		CHECKMMX

			// 使えるのでＣＰＵ名を取得
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
			// 今度はＭＭＸが使えるか検査
			MOV		EAX , 1
		}
//			db 0fh
//			db 0a2h
        cpuid
		__asm{
			AND		EDX , 00800000H
			JZ		MMXEND
			MOV		ENDMODE , 3
			//SSEが使えるか検査
			MOV		EAX , 1
		}
		cpuid
		__asm{
			AND		EDX	, 02000000H
			JZ		MMXEND
			ADD		ENDMODE	, 1
			//SSE2が使えるか検査
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
			
		// パフォーマンスカウンタフラグを保存
		WinData.UseRDTSCFlag = RDTSCUse ;

#ifdef DX_NON_INLINE_ASM
	#ifdef _WIN64
		// パフォーマンスカウンタがつかえる場合はクロック数を簡単に計測
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

			DXST_ERRORLOGFMT_ADDUTF16LE(( "\xfe\x73\x42\x66\xb9\x70\x6e\x30\x23\xff\x30\xff\x35\xff\xd5\x52\x5c\x4f\x1f\x90\xa6\x5e\x1a\xff\x27\x59\x53\x4f\x25\x00\x2e\x00\x32\x00\x66\x00\x47\x00\x48\x00\x7a\x00\x00"/*@ L"現時点のＣＰＵ動作速度：大体%.2fGHz" @*/, (float)Clock / 1000.0f )) ;
			WinData.PcInfo.CPUSpeed = ( int )Clock ;
		}

	#endif // _WIN64
#else
		// パフォーマンスカウンタがつかえる場合はクロック数を簡単に計測
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

			DXST_ERRORLOGFMT_ADDUTF16LE(( "\xfe\x73\x42\x66\xb9\x70\x6e\x30\x23\xff\x30\xff\x35\xff\xd5\x52\x5c\x4f\x1f\x90\xa6\x5e\x1a\xff\x27\x59\x53\x4f\x25\x00\x2e\x00\x32\x00\x66\x00\x47\x00\x48\x00\x7a\x00\x00"/*@ L"現時点のＣＰＵ動作速度：大体%.2fGHz" @*/, (float)Clock / 1000.0f )) ;
			WinData.PcInfo.CPUSpeed = ( int )Clock ;
		}
#endif // DX_NON_INLINE_ASM

		switch( ENDMODE )
		{
		case 0 :
#ifndef DX_NON_INLINE_ASM
			DXST_ERRORLOG_ADDUTF16LE( "\x23\xff\x30\xff\x35\xff\x29\xff\x24\xff\x7d\x54\xe4\x4e\x6f\x30\x7f\x4f\x48\x30\x7e\x30\x5b\x30\x93\x30\x0a\x00\x00"/*@ L"ＣＰＵＩＤ命令は使えません\n" @*/ ) ;
#endif // DX_NON_INLINE_ASM
			break ;

		case 1 :
			DXST_ERRORLOG_ADDUTF16LE( "\x2d\xff\x2d\xff\x38\xff\x6f\x30\x7f\x4f\x48\x30\x7e\x30\x5b\x30\x93\x30\x0a\x00\x00"/*@ L"ＭＭＸは使えません\n" @*/ ) ;
			break ;

		case 3 :
			DXST_ERRORLOG_ADDUTF16LE( "\x2d\xff\x2d\xff\x38\xff\x7d\x54\xe4\x4e\x92\x30\x7f\x4f\x28\x75\x57\x30\x7e\x30\x59\x30\x0a\x00\x00"/*@ L"ＭＭＸ命令を使用します\n" @*/ ) ;
			WinData.UseMMXFlag = TRUE ;
			break ;

		case 4 :
			DXST_ERRORLOG_ADDUTF16LE( "\x2d\xff\x2d\xff\x38\xff\x7d\x54\xe4\x4e\x92\x30\x7f\x4f\x28\x75\x57\x30\x7e\x30\x59\x30\x0a\x00\x00"/*@ L"ＭＭＸ命令を使用します\n" @*/ ) ;
			DXST_ERRORLOG_ADDUTF16LE( "\x33\xff\x33\xff\x25\xff\x7d\x54\xe4\x4e\x4c\x30\x7f\x4f\x28\x75\xef\x53\xfd\x80\x67\x30\x59\x30\x0a\x00\x00"/*@ L"ＳＳＥ命令が使用可能です\n" @*/ ) ;
			WinData.UseMMXFlag = TRUE ;
			WinData.UseSSEFlag = TRUE ;
			break ;

		case 5 :
			DXST_ERRORLOG_ADDUTF16LE( "\x2d\xff\x2d\xff\x38\xff\x7d\x54\xe4\x4e\x92\x30\x7f\x4f\x28\x75\x57\x30\x7e\x30\x59\x30\x0a\x00\x00"/*@ L"ＭＭＸ命令を使用します\n" @*/ ) ;
			DXST_ERRORLOG_ADDUTF16LE( "\x33\xff\x33\xff\x25\xff\x7d\x54\xe4\x4e\x4c\x30\x7f\x4f\x28\x75\xef\x53\xfd\x80\x67\x30\x59\x30\x0a\x00\x00"/*@ L"ＳＳＥ命令が使用可能です\n" @*/ ) ;
			DXST_ERRORLOG_ADDUTF16LE( "\x33\xff\x33\xff\x25\xff\x12\xff\x7d\x54\xe4\x4e\x4c\x30\x7f\x4f\x28\x75\xef\x53\xfd\x80\x67\x30\x59\x30\x0a\x00\x00"/*@ L"ＳＳＥ２命令が使用可能です\n" @*/ ) ;
			WinData.UseMMXFlag = TRUE ;
			WinData.UseSSEFlag = TRUE ;
			WinData.UseSSE2Flag = TRUE ;
			break ;
		}

		if( ENDMODE > 0 )
		{
			DXST_ERRORLOG_ADDUTF16LE( "\x23\xff\x30\xff\x35\xff\xd9\x30\xf3\x30\xc0\x30\x1a\xff\x00"/*@ L"ＣＰＵベンダ：" @*/ ) ;

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
				DXST_ERRORLOGFMT_ADDA(( "\x82\x62\x82\x6f\x82\x74\x96\xbc\x81\x46%s"/*@ "ＣＰＵ名：%s" @*/, CpuName )) ;

				ConvString( ( const char * )CpuName, DX_CHARCODEFORMAT_ASCII, ( char * )WinData.PcInfo.CPUString, sizeof( WinData.PcInfo.CPUString ), WCHAR_T_CHARCODEFORMAT ) ;
			}
		}
	}

	DXST_ERRORLOG_TABSUB ;

	// もし DirectX のバージョンが７以下だった場合はここでソフトを終了
	if( WinData.DirectXVersion < DX_DIRECTXVERSION_7 )
	{
#ifndef DX_NON_LITERAL_STRING
		if( WinData.DirectXVersion == DX_DIRECTXVERSION_NON )
		{
			MessageBoxW( NULL, ( wchar_t * )"\x44\x00\x69\x00\x72\x00\x65\x00\x63\x00\x74\x00\x58\x00\x20\x00\x4c\x30\xa4\x30\xf3\x30\xb9\x30\xc8\x30\xfc\x30\xeb\x30\x55\x30\x8c\x30\x66\x30\x44\x30\x6a\x30\x44\x30\x6e\x30\x67\x30\x53\x30\x53\x30\x67\x30\x42\x7d\x86\x4e\x57\x30\x7e\x30\x59\x30\x00"/*@ L"DirectX がインストールされていないのでここで終了します" @*/, ( wchar_t * )"\xa8\x30\xe9\x30\xfc\x30\x00"/*@ L"エラー" @*/, MB_OK ) ;
		}
		else
		{
			MessageBoxW( NULL, ( wchar_t * )"\x44\x00\x69\x00\x72\x00\x65\x00\x63\x00\x74\x00\x58\x00\x20\x00\xd0\x30\xfc\x30\xb8\x30\xe7\x30\xf3\x30\x20\x00\x37\x00\x20\x00\xe5\x4e\x4d\x52\x6a\x30\x6e\x30\x67\x30\xbd\x30\xd5\x30\xc8\x30\x92\x30\x77\x8d\xd5\x52\x59\x30\x8b\x30\x53\x30\x68\x30\x4c\x30\xfa\x51\x65\x67\x7e\x30\x5b\x30\x93\x30\x00"/*@ L"DirectX バージョン 7 以前なのでソフトを起動することが出来ません" @*/, ( wchar_t * )"\xa8\x30\xe9\x30\xfc\x30\x00"/*@ L"エラー" @*/, MB_OK ) ;
		}
#endif
		ExitProcess( (DWORD)-1 ) ;
	}

	// 終了
	return 0 ;
}






















// メッセージ処理関数

int WM_PAINTProcess( HDC Dc, RECT *fillRect, int AlwaysFillFlag )
{
	// WM_PAINT メッセージが来たフラグを立てる
	WinData.WM_PAINTMessageFlag = TRUE ;

	// ＤＸライブラリの WM_PAINT の処理を実行しない場合はここで終了
	if( WinData.NotUseDxLibWM_PAINTProcess )
	{
		return 0 ;
	}

	// 非アクティブで、且つ非アクティブ時に表示する専用の画像がある場合はそれを描画する
	if( ( WinData.ActiveFlag == FALSE && WinData.PauseGraph.GraphData != NULL && WinData.NonActiveRunFlag == FALSE ) /*|| 
		( WinData.MenuUseFlag == TRUE && WinData.MenuShredRunFlag == TRUE )*/ )
	{
		// バックグラウンド描画
		if( WinData.WindowMinSizeFlag == FALSE ) 
		{
			DrawBackGraph( Dc ) ;
		}
	}
	else
	{
		// それ以外の場合

		// ＤＸライブラリを初期化中で、且つリージョンが設定されて
		// いない場合は画面を黒く初期化する
		if( ( DxSysData.DxLib_InitializeFlag == FALSE && WinData.WindowRgn == NULL ) || AlwaysFillFlag )
		{
			// バックグラウンドカラーが有効な場合はその色を使用する
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
			// とりあえず裏画面をコピーしておく
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

	// ユーザーのウインドウを使用していない場合のみサイズの補正を行う
	if( WinData.UserWindowFlag == TRUE ) return 0 ;

	GetMainWindowSize( &Width, &Height ) ;

	// ウインドウへの出力サイズを反映させる
	NS_GetWindowSizeExtendRate( &ExRateX, &ExRateY ) ;
	Width  = _DTOL( Width  * ExRateX ) ;
	Height = _DTOL( Height * ExRateY ) ;

	GetWindowRect( WinData.MainWindow, &rect ) ;
	if( GetClientRect( WinData.MainWindow, &ClientRect ) == 0 ) return 0 ;
	WinData.WindowRect.right  = WinData.WindowRect.left + ClientRect.right ;
	WinData.WindowRect.bottom = WinData.WindowRect.top  + ClientRect.bottom ;

	// クライアント領域のサイズを得る
	CWidth  = ClientRect.right  - ClientRect.left ;
	CHeight = ClientRect.bottom - ClientRect.top ;

	// クライアント領域外の部分の幅・高さを得る
	rect.right  -= CWidth ;
	rect.bottom -= CHeight ;

	// ウインドウに画面をフィットさせるかどうかで処理を分岐
	if( WinData.ScreenNotFitWindowSize == TRUE || WinData.WindowSizeValid == TRUE )
	{
		// ウインドウサイズ設定が指定されていたらそれを使用する
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

		// クライアント領域が画面領域より大きくなっている場合は補正する
		if( CWidth  > Width  ) CWidth  = Width  ;
		if( CHeight > Height ) CHeight = Height ;
	}
	else
	{
		// クライアント領域が画面領域と異なっている場合は補正する
		if( CWidth  != Width  ) CWidth  = Width  ;
		if( CHeight != Height ) CHeight = Height ;
	}
	rect.right  += CWidth ;
	rect.bottom += CHeight + GetToolBarHeight() ;

	// ウインドウの位置指定がある場合は反映
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
	// 最大化状態の場合はウインドウをデスクトップの中心に配置する
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

		// 位置やサイズが元の矩形と１だけ異なる場合は補正する
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

		// ( 最大化後のウインドウの位置やサイズを変更すると、２回目以降の最大化のウインドウのサイズや位置が変になるので、一旦補正を無効化 )
		rect = WinData.WindowMaximizedRect ;
		WinData.WindowMaximizedAdjustRect = rect ;
	}

	MoveWindow( WinData.MainWindow, rect.left, rect.top, rect.right-rect.left, rect.bottom-rect.top, TRUE ) ;
	if( WinData.WindowModeFlag == TRUE && WinData.VisibleFlag == FALSE )
	{
		WinData.VisibleFlag = TRUE ;
		SetWindowStyle() ;
	}

	// 終了
	return 0 ;
}

int WM_MOVEProcess( LPARAM lParam )
{
	RECT rect ;
	SIZE WindowSize ;

	// ユーザーのウインドウを使用している場合のみ移動を行う
	if( WinData.UserWindowFlag == TRUE ) return 0 ;

	// ウインドウの位置を保存
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

		// マスクを使用終了する手続きを取る
		Flag = NS_GetUseMaskScreenFlag() ;
		NS_SetUseMaskScreenFlag( FALSE ) ;

		// マスクを使用する手続きを取る
		NS_SetUseMaskScreenFlag( Flag ) ;
#endif
	}
#endif // DX_NON_GRAPHICS

	// 終了
	return 0 ;
}

int WM_ACTIVATEProcessUseStock( WPARAM wParam, LPARAM lParam, int APPMes, int Dummy )
{
	// ProcessMessage からここに来た場合( 且つまだリクエストがされていなく、ウインドウ作成直後でもない場合 )は
	// ProcessMessage の最後に WM_ACTIVATE を行うようにする
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

	// アクティブかどうかを取得
	ActiveFlag = LOWORD( wParam ) != 0 ;

	// ウインドウ作成直後だった場合は何もせず終了
	if( WinData.WindowCreateFlag == TRUE )
	{
		// アクティブかどうかを保存
		WinData.ActiveFlag = ActiveFlag ;
		return 0 ;
	}

	// 最小化された状態のアクティブ情報(wParam の HIWORD が 0 以外)は非アクティブとして処理をする
	if( HIWORD( wParam ) != 0 )
	{
		ActiveFlag = FALSE ;
	}

	// アクティブ情報が今までと同じ場合は何もしない
	if( WinData.ActiveFlag == ActiveFlag )
		return 0 ;

	// アクティブかどうかを保存
	WinData.ActiveFlag = ActiveFlag ;

	if( ActiveFlag == TRUE )
	{
//		DXST_ERRORLOG_ADDW( L"アクティブになりました\n" ) ;

#ifndef DX_NON_INPUTSTRING
		// ＩＭＥで文字列を入力中だった場合は文字列を確定してしまう
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
			// グラフィックシステムの復帰処理
			NS_RestoreGraphSystem() ;
#endif // DX_NON_GRAPHICS

			WinData.StopFlag = FALSE ;

#ifndef DX_NON_MOVIE
			// ムービーグラフィックの再生状態を戻す
			PlayMovieAll() ;
#endif

#ifndef DX_NON_SOUND
			// サウンドの再生を再開する
			PauseSoundMemAll( FALSE ) ;
			PauseSoftSoundAll( FALSE ) ;
#endif // DX_NON_SOUND
		}
		else
		{
			if( WinData.NonActiveRunFlag == FALSE )
			{
#ifndef DX_NON_MOVIE
				// ムービーグラフィックの再生状態を戻す
				PlayMovieAll() ;
#endif

#ifndef DX_NON_SOUND
				// サウンドの再生を再開する
				PauseSoundMemAll( FALSE ) ;
				PauseSoftSoundAll( FALSE ) ;
#endif // DX_NON_SOUND
			}
		}

		// DirectInputデバイスオブジェクトの再取得
#ifndef DX_NON_INPUT
		RefreshInputDeviceAcquireState() ;
#endif // DX_NON_INPUT
	}
	else
	{
//		DXST_ERRORLOG_ADDW( L"非アクティブになりました\n" ) ;

		if( WinData.WindowModeFlag == FALSE  )
		{
			if( DxSysData.NotDrawFlag == FALSE )
			{
#ifndef DX_NON_GRAPHICS
				// DirectX のオブジェクトを解放する
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

		// フルスクリーンモードの場合の処理
#ifndef DX_NON_GRAPHICS
		if( WinData.WindowModeFlag == FALSE && NS_GetChangeDisplayFlag() && NS_GetUseDDrawObj() != NULL )
		{
			// フルスクリーンの場合は必ず DWM を有効にする
			SetEnableAero( TRUE ) ;

			// もし復元スレッドが登録されていない場合はソフト終了
			if( NS_GetValidRestoreShredPoint() == FALSE ) 
			{
				// ユーザー提供のウインドウだったら何もしない
				if( WinData.UserWindowFlag == FALSE )
				{
					DXST_ERRORLOG_ADDUTF16LE( "\xa9\x5f\x43\x51\xa2\x95\x70\x65\x4c\x30\x7b\x76\x32\x93\x55\x30\x8c\x30\x66\x30\x44\x30\x6a\x30\x44\x30\x5f\x30\x81\x30\x42\x7d\x86\x4e\x57\x30\x7e\x30\x59\x30\x0a\x00\x00"/*@ L"復元関数が登録されていないため終了します\n" @*/ ) ;
				
					// クローズフラグが倒れていたらWM_CLOSEメッセージを送る
					if( WinData.CloseMessagePostFlag == FALSE )
					{
						WinData.CloseMessagePostFlag = TRUE ;
						PostMessageW( WinData.MainWindow, WM_CLOSE, 0, 0 );
					}

					return -1 ;
				}
			}

#ifndef DX_NON_MOVIE
			// ムービーグラフィックの再生状態をとめる
			PauseMovieAll() ;
#endif

#ifndef DX_NON_SOUND
			// サウンドの再生を止める
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
				// ムービーグラフィックの再生状態をとめる
				PauseMovieAll() ;
#endif

#ifndef DX_NON_SOUND
				// サウンドの再生を止める
				PauseSoundMemAll( TRUE ) ;
				PauseSoftSoundAll( TRUE ) ;
#endif // DX_NON_SOUND
			}

			// DirectInputデバイスオブジェクトの再取得
#ifndef DX_NON_INPUT
			RefreshInputDeviceAcquireState() ;
#endif // DX_NON_INPUT
		}

		if( WinData.NotMoveMousePointerOutClientAreaFlag )
		{
			ClipCursor( NULL ) ;
			WinData.SetClipCursorFlag = FALSE ;
		}

		// バックグラウンド描画
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

	// パッドのバイブレーション状態を更新
#ifndef DX_NON_INPUT
	RefreshEffectPlayState() ;
#endif // DX_NON_INPUT

	// アクティブ状態が変化した時に呼ぶコールバック関数が設定されていたら呼ぶ
	if( WinData.ActiveStateChangeCallBackFunction != NULL && APPMes == FALSE )
	{
		WinData.ActiveStateChangeCallBackFunction( ActiveFlag, WinData.ActiveStateChangeCallBackFunctionData ) ;
	}

	// ソフトプロセスの実行優先順位をセットする
//	SetPriorityClass( GetCurrentProcess() , ActiveFlag ? HIGH_PRIORITY_CLASS : IDLE_PRIORITY_CLASS );

	// タスクスイッチの有効無効をセット
	if( WinData.SysCommandOffFlag == TRUE )
	{
		if( WinData.WindowsVersion < DX_WINDOWSVERSION_NT31 )
		{
			// Win95 カーネルの場合の処理
			UINT nPreviousState;
//			SystemParametersInfoW( SPI_SETSCREENSAVERRUNNING, ActiveFlag, &nPreviousState, 0 ) ;
			SystemParametersInfoW( SPI_SETSCREENSAVERRUNNING/*SPI_SCREENSAVERRUNNING*/, ( UINT )ActiveFlag, &nPreviousState, 0 ) ;
		}
		else
		{
			// WinNT カーネルの場合の処理

			if( /*WinData.GetMessageHookHandle == NULL &&*/ ActiveFlag == TRUE )
			{
//				WinData.MessageHookThredID = GetWindowThreadProcessId( WinData.MainWindow, NULL ) ;
//				WinData.MessageHookThredID = GetWindowThreadProcessId( GetDesktopWindow(), NULL ) ;
				WinData.MessageHookDLL = LoadLibraryW( WinData.HookDLLFilePath ) ;

				// DLL が無かったら進まない
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
				// メッセージフックを無効に
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

		// 共通
		WinData.LockInitializeFlag = TRUE ;
	}

	// 正常終了
	return 0 ;
}



// SPI_GETFOREGROUNDLOCKTIMEOUT の定義
#if !defined(SPI_GETFOREGROUNDLOCKTIMEOUT) 
#define SPI_GETFOREGROUNDLOCKTIMEOUT 0x2000 
#define SPI_SETFOREGROUNDLOCKTIMEOUT 0x2001 
#endif 

// ソフトのウインドウにフォーカスを移す
extern void SetAbsoluteForegroundWindow( HWND hWnd, int Flag )
{
    DWORD nTargetID, nForegroundID;
    DWORD sp_time;

    // フォアグラウンドウィンドウを作成したスレッドのIDを取得
    nForegroundID = GetWindowThreadProcessId(GetForegroundWindow(), NULL);

    // 目的のウィンドウを作成したスレッドのIDを取得
    nTargetID = GetWindowThreadProcessId(hWnd, NULL );


    // スレッドのインプット状態を結び付ける
    AttachThreadInput(nTargetID, nForegroundID, TRUE );  // TRUE で結び付け


    // 現在の設定を sp_time に保存
    SystemParametersInfoW( SPI_GETFOREGROUNDLOCKTIMEOUT,0,&sp_time,0);

    // ウィンドウの切り替え時間を 0ms にする
    SystemParametersInfoW( SPI_SETFOREGROUNDLOCKTIMEOUT,0,(LPVOID)0,0);


    // ウィンドウをフォアグラウンドに持ってくる
    SetForegroundWindow(hWnd);


    // 設定を元に戻す
    SystemParametersInfoW( SPI_SETFOREGROUNDLOCKTIMEOUT,0,&sp_time,0);


    // スレッドのインプット状態を切り離す
    AttachThreadInput(nTargetID, nForegroundID, FALSE );  // FALSE で切り離し

	// ウインドウを最前面に持ってくる
	if( Flag == TRUE ) SetWindowPos( WinData.MainWindow , HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOREDRAW ) ;
}



// ＢＭＰリソースから BITMAPINFO と画像イメージを構築する
extern int GetBmpImageToResource( int ResourceID, BITMAPINFO **BmpInfoP, void **GraphDataP )
{
	BITMAPINFO *BmpInfo = NULL ;
	void *GraphData = NULL ;
	HRSRC RSrc ;
	void *DataBuf ;
	BYTE *DataP ;
	HGLOBAL Global ;
	int Er = FALSE ;

	// リソースを取得
	RSrc = FindResource( WinData.LoadResourModule == NULL ? WinAPIData.Win32Func.GetModuleHandleWFunc( NULL ) : WinData.LoadResourModule, MAKEINTRESOURCE( ResourceID ), RT_BITMAP ) ;
	if( RSrc == NULL ) return -1 ;

	// リソースが格納されているメモリ領域を取得
	Global = LoadResource( WinData.LoadResourModule == NULL ? WinAPIData.Win32Func.GetModuleHandleWFunc( NULL ) : WinData.LoadResourModule, RSrc ) ;
	DataP = ( BYTE * )( DataBuf = LockResource( Global ) ) ;
	if( DataBuf == NULL ) return -1 ;

	// データを解析
	{
		BITMAPINFO			BmpInfoT ;
		int					ImageSize ;

		// BITMAPINFOを読みこむ
		memcpy( &BmpInfoT, DataP, sizeof( BITMAPINFOHEADER ) ) ;
		DataP += sizeof( BITMAPINFOHEADER ) ;

		// カラービット数が８以下の時はパレットを読む
		if( BmpInfoT.bmiHeader.biBitCount <= 8 )
		{
			// ＢＭＰＩＮＦＯ構造体の格納用メモリを確保
			if( ( BmpInfo = ( BITMAPINFO * )DXCALLOC( ( size_t )( sizeof( BITMAPINFOHEADER ) + ( ( size_t )1 << BmpInfoT.bmiHeader.biBitCount ) * sizeof( RGBQUAD ) ) ) ) == NULL )
			{
				Er = TRUE ; goto END1 ;
			}

			// ＢＭＰＩＮＦＯ構造体の内容をコピー
			memcpy( BmpInfo, &BmpInfoT, sizeof( BITMAPINFOHEADER ) ) ;

			// 残りのカラーパレットの読みこみ
			memcpy( ( ( BYTE * )BmpInfo ) + sizeof( BITMAPINFOHEADER ), DataP, ( ( size_t )1 << BmpInfoT.bmiHeader.biBitCount ) * sizeof( RGBQUAD ) ) ;
			DataP += ( ( size_t )1 << BmpInfoT.bmiHeader.biBitCount ) * sizeof( RGBQUAD ) ;
		}
		else
		// カラービット数が３２か１６でカラーマスク使用時の処理
		if( BmpInfoT.bmiHeader.biBitCount == 16 || BmpInfoT.bmiHeader.biBitCount == 32 || BmpInfoT.bmiHeader.biCompression == BI_BITFIELDS )
		{
			// ＢＭＰＩＮＦＯ構造体の格納用メモリを確保
			if( ( BmpInfo = ( BITMAPINFO * )DXCALLOC( sizeof( BITMAPINFOHEADER ) + sizeof( RGBQUAD ) * 3 ) ) == NULL )
			{
				Er = TRUE ; goto END1 ;
			}

			// ＢＭＰＩＮＦＯＨＥＡＤＥＲ構造体の内容をコピー
			memcpy( BmpInfo, &BmpInfoT, sizeof( BITMAPINFOHEADER ) ) ;

			// カラーマスクの読みこみ
			memcpy( ( ( BYTE * )BmpInfo ) + sizeof( BITMAPINFOHEADER ), DataP, sizeof( RGBQUAD ) * 3 ) ;
			DataP += sizeof( RGBQUAD ) * 3 ;
		}
		else
		// それ以外の場合の処理
		{
			// ＢＭＰＩＮＦＯ構造体の格納用メモリを確保
			if( ( BmpInfo = ( BITMAPINFO * )DXCALLOC( sizeof( BITMAPINFO ) ) ) == NULL )
			{
				Er = TRUE ; goto END1 ;
			}

			// ＢＭＰＩＮＦＯＨＥＡＤＥＲ構造体の内容をコピー
			memcpy( BmpInfo, &BmpInfoT, sizeof( BITMAPINFOHEADER ) ) ;
		}

		// イメージサイズを計算する
		{
			int Byte ;

			Byte = BmpInfoT.bmiHeader.biWidth * BmpInfoT.bmiHeader.biBitCount / 8 ;
			Byte += Byte % 4 != 0 ? ( 4 - Byte % 4 ) : 0 ;

			ImageSize = Byte * _ABS( BmpInfoT.bmiHeader.biHeight ) ;
		}

		// グラフィックデータ領域を確保
		if( ( GraphData = DXALLOC( ( size_t )ImageSize ) ) == NULL )
		{
			DXFREE( BmpInfo ) ; BmpInfo = NULL ;
			goto END1 ;
		}

		// グラフィックデータの読みこみ
		memcpy( GraphData, DataP, ( size_t )ImageSize ) ;
		DataP += ImageSize ;
	}

END1 :
	// リソース解放
	UnlockResource( Global ) ;

	if( Er == TRUE ) return -1 ;

	*BmpInfoP = BmpInfo ;
	*GraphDataP = GraphData ;

	// 成功
	return 0 ;
}





#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE










