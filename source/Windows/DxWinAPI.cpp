// -------------------------------------------------------------------------------
// 
// 		ＤＸライブラリ		WinAPIプログラム
// 
// 				Ver 3.16d
// 
// -------------------------------------------------------------------------------

// ＤＸライブラリ作成時用定義
#define __DX_MAKE

// インクルード ------------------------------------------------------------------
#include "DxWinAPI.h"
#include <tchar.h>
#include "../DxASyncLoad.h"

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// マクロ定義 --------------------------------------------------------------------

// 構造体定義 --------------------------------------------------------------------

// 内部大域変数宣言 --------------------------------------------------------------

WINAPIDATA WinAPIData ;

// 関数プロトタイプ宣言-----------------------------------------------------------

// プログラム --------------------------------------------------------------------

// WindowsOS の DLL を読み込む
extern int LoadWinAPI( void )
{
#ifndef DX_NON_NETWORK
	// ws2_32.dll をロードする
	if( WinAPIData.WinSockFunc.WinSockDLL == NULL )
	{
		WinAPIData.WinSockFunc.WinSockDLL = LoadLibraryW( L"ws2_32.dll" ) ;
		WinAPIData.WinSockFunc.WSAGetLastErrorFunc = ( int ( WINAPI * )( void ) )GetProcAddress( WinAPIData.WinSockFunc.WinSockDLL, "WSAGetLastError" ) ;
		WinAPIData.WinSockFunc.WSAStartupFunc = ( int ( WINAPI * )( WORD , LPWSADATA  ) )GetProcAddress( WinAPIData.WinSockFunc.WinSockDLL, "WSAStartup" ) ;
		WinAPIData.WinSockFunc.WSACleanupFunc = ( int ( WINAPI * )( void ) )GetProcAddress( WinAPIData.WinSockFunc.WinSockDLL, "WSACleanup" ) ;
		WinAPIData.WinSockFunc.WSAAsyncSelectFunc = ( int ( WINAPI * )( SOCKET, HWND, unsigned int, long ) )GetProcAddress( WinAPIData.WinSockFunc.WinSockDLL, "WSAAsyncSelect" ) ;
		WinAPIData.WinSockFunc.getaddrinfoFunc = ( int ( WINAPI * )( const char *, const char *, const _addrinfo *, _addrinfo ** ) )GetProcAddress( WinAPIData.WinSockFunc.WinSockDLL, "getaddrinfo" ) ;
		WinAPIData.WinSockFunc.gethostbyaddrFunc = ( struct hostent* ( WINAPI * )( const char *, int, int ) )GetProcAddress( WinAPIData.WinSockFunc.WinSockDLL, "gethostbyaddr" ) ;
		WinAPIData.WinSockFunc.gethostbynameFunc = ( struct hostent* ( WINAPI * )( const char * ) )GetProcAddress( WinAPIData.WinSockFunc.WinSockDLL, "gethostbyname" ) ;
		WinAPIData.WinSockFunc.gethostnameFunc = ( int ( WINAPI * )( char *name, int namelen ) )GetProcAddress( WinAPIData.WinSockFunc.WinSockDLL, "gethostname" ) ;
		WinAPIData.WinSockFunc.ntohsFunc = ( u_short ( WINAPI * )( u_short ) )GetProcAddress( WinAPIData.WinSockFunc.WinSockDLL, "ntohs" ) ;
		WinAPIData.WinSockFunc.htonsFunc = ( u_short ( WINAPI * )( u_short ) )GetProcAddress( WinAPIData.WinSockFunc.WinSockDLL, "htons" ) ;
		WinAPIData.WinSockFunc.connectFunc = ( int ( WINAPI * )( SOCKET s, const struct sockaddr *, int ) )GetProcAddress( WinAPIData.WinSockFunc.WinSockDLL, "connect" ) ;
		WinAPIData.WinSockFunc.socketFunc = ( SOCKET ( WINAPI * )( int, int, int ) )GetProcAddress( WinAPIData.WinSockFunc.WinSockDLL, "socket" ) ;
		WinAPIData.WinSockFunc.sendFunc = ( int ( WINAPI * )( SOCKET s, const char *, int, int ) )GetProcAddress( WinAPIData.WinSockFunc.WinSockDLL, "send" ) ;
		WinAPIData.WinSockFunc.sendtoFunc = ( int ( WINAPI * )( SOCKET s, const char *, int, int, const struct sockaddr *, int ) )GetProcAddress( WinAPIData.WinSockFunc.WinSockDLL, "sendto" ) ;
		WinAPIData.WinSockFunc.recvfromFunc = ( int ( WINAPI * )( SOCKET s, char *, int , int , struct sockaddr *, int * ) )GetProcAddress( WinAPIData.WinSockFunc.WinSockDLL, "recvfrom" ) ;
		WinAPIData.WinSockFunc.acceptFunc = ( SOCKET ( WINAPI * )( SOCKET s, struct sockaddr *, int * ) )GetProcAddress( WinAPIData.WinSockFunc.WinSockDLL, "accept" ) ;
		WinAPIData.WinSockFunc.closesocketFunc = ( int ( WINAPI * )( SOCKET ) )GetProcAddress( WinAPIData.WinSockFunc.WinSockDLL, "closesocket" ) ;
		WinAPIData.WinSockFunc.shutdownFunc = ( int ( WINAPI * )( SOCKET , int  ) )GetProcAddress( WinAPIData.WinSockFunc.WinSockDLL, "shutdown" ) ;
		WinAPIData.WinSockFunc.listenFunc = ( int ( WINAPI * )( SOCKET , int  ) )GetProcAddress( WinAPIData.WinSockFunc.WinSockDLL, "listen" ) ;
		WinAPIData.WinSockFunc.bindFunc = ( int ( WINAPI * )( SOCKET , const struct sockaddr *, int  ) )GetProcAddress( WinAPIData.WinSockFunc.WinSockDLL, "bind" ) ;
		WinAPIData.WinSockFunc.inet_addrFunc = ( unsigned long ( WINAPI * )( const char * ) )GetProcAddress( WinAPIData.WinSockFunc.WinSockDLL, "inet_addr" ) ;
		WinAPIData.WinSockFunc.recvFunc = ( int ( WINAPI * )( SOCKET, char *, int, int ) )GetProcAddress( WinAPIData.WinSockFunc.WinSockDLL, "recv" ) ;
	}
#endif

#ifndef DX_NON_KEYEX
	// Imm32DLL をロードする
	if( WinAPIData.ImmFunc.Imm32DLL == NULL )
	{
		WinAPIData.ImmFunc.Imm32DLL = LoadLibraryW( L"Imm32.dll" ) ;
		WinAPIData.ImmFunc.ImmGetContextFunc = ( HIMC ( WINAPI * )( HWND  ) )GetProcAddress( WinAPIData.ImmFunc.Imm32DLL, "ImmGetContext" ) ;
		WinAPIData.ImmFunc.ImmReleaseContextFunc = ( BOOL ( WINAPI * )( HWND , HIMC  ) )GetProcAddress( WinAPIData.ImmFunc.Imm32DLL, "ImmReleaseContext" ) ;
		WinAPIData.ImmFunc.ImmGetOpenStatusFunc = ( BOOL ( WINAPI * )( HIMC  ) )GetProcAddress( WinAPIData.ImmFunc.Imm32DLL, "ImmGetOpenStatus" ) ;
		WinAPIData.ImmFunc.ImmGetConversionStatusFunc = ( BOOL ( WINAPI * )( HIMC , LPDWORD , LPDWORD  ) )GetProcAddress( WinAPIData.ImmFunc.Imm32DLL, "ImmGetConversionStatus" ) ;
		WinAPIData.ImmFunc.ImmNotifyIMEFunc = ( BOOL ( WINAPI * )( HIMC , DWORD , DWORD , DWORD  ) )GetProcAddress( WinAPIData.ImmFunc.Imm32DLL, "ImmNotifyIME" ) ;
		WinAPIData.ImmFunc.ImmSetOpenStatusFunc = ( BOOL ( WINAPI * )( HIMC , BOOL  ) )GetProcAddress( WinAPIData.ImmFunc.Imm32DLL, "ImmSetOpenStatus" ) ;

//#ifdef UNICODE
		WinAPIData.ImmFunc.ImmGetCandidateListFunc = ( DWORD ( WINAPI * )( HIMC , DWORD , LPCANDIDATELIST , DWORD  ) )GetProcAddress( WinAPIData.ImmFunc.Imm32DLL, "ImmGetCandidateListW" ) ;
		WinAPIData.ImmFunc.ImmGetCandidateListCountFunc = ( DWORD ( WINAPI * )( HIMC , LPDWORD  ) )GetProcAddress( WinAPIData.ImmFunc.Imm32DLL, "ImmGetCandidateListCountW" ) ;
		WinAPIData.ImmFunc.ImmGetCompositionStringFunc = ( LONG ( WINAPI * )( HIMC , DWORD , LPVOID , DWORD  ) )GetProcAddress( WinAPIData.ImmFunc.Imm32DLL, "ImmGetCompositionStringW" ) ;
		WinAPIData.ImmFunc.ImmSetCompositionStringFunc = ( BOOL ( WINAPI * )( HIMC , DWORD , LPCVOID , DWORD , LPCVOID , DWORD  ) )GetProcAddress( WinAPIData.ImmFunc.Imm32DLL, "ImmSetCompositionStringW" ) ;
//#else
//		WinAPIData.ImmFunc.ImmGetCandidateListFunc = ( DWORD ( WINAPI * )( HIMC , DWORD , LPCANDIDATELIST , DWORD  ) )GetProcAddress( WinAPIData.ImmFunc.Imm32DLL, "ImmGetCandidateListA" ) ;
//		WinAPIData.ImmFunc.ImmGetCandidateListCountFunc = ( DWORD ( WINAPI * )( HIMC , LPDWORD  ) )GetProcAddress( WinAPIData.ImmFunc.Imm32DLL, "ImmGetCandidateListCountA" ) ;
//		WinAPIData.ImmFunc.ImmGetCompositionStringFunc = ( LONG ( WINAPI * )( HIMC , DWORD , LPVOID , DWORD  ) )GetProcAddress( WinAPIData.ImmFunc.Imm32DLL, "ImmGetCompositionStringA" ) ;
//		WinAPIData.ImmFunc.ImmSetCompositionStringFunc = ( BOOL ( WINAPI * )( HIMC , DWORD , LPCVOID , DWORD , LPCVOID , DWORD  ) )GetProcAddress( WinAPIData.ImmFunc.Imm32DLL, "ImmSetCompositionStringA" ) ;
//#endif
	}
#endif

	// WinMM DLL をロードする
	if( WinAPIData.Win32Func.WinMMDLL == NULL )
	{
		WinAPIData.Win32Func.WinMMDLL = LoadLibraryW( L"winmm.dll" ) ;
		WinAPIData.Win32Func.timeSetEventFunc = ( MMRESULT	( WINAPI * )( UINT , UINT , LPTIMECALLBACK , DWORD_PTR , UINT  ) )GetProcAddress( WinAPIData.Win32Func.WinMMDLL, "timeSetEvent" ) ;
		WinAPIData.Win32Func.timeKillEventFunc = ( MMRESULT ( WINAPI * )( UINT  ) )GetProcAddress( WinAPIData.Win32Func.WinMMDLL, "timeKillEvent" ) ;
		WinAPIData.Win32Func.timeBeginPeriodFunc = ( MMRESULT ( WINAPI * )( UINT  ) )GetProcAddress( WinAPIData.Win32Func.WinMMDLL, "timeBeginPeriod" ) ;
		WinAPIData.Win32Func.timeEndPeriodFunc = ( MMRESULT ( WINAPI * )( UINT  ) )GetProcAddress( WinAPIData.Win32Func.WinMMDLL, "timeEndPeriod" ) ;
		WinAPIData.Win32Func.timeGetTimeFunc = ( DWORD ( WINAPI * )( VOID ) )GetProcAddress( WinAPIData.Win32Func.WinMMDLL, "timeGetTime" ) ;
		WinAPIData.Win32Func.timeGetDevCapsFunc = ( MMRESULT ( WINAPI * )( LPTIMECAPS , UINT  ) )GetProcAddress( WinAPIData.Win32Func.WinMMDLL, "timeGetDevCaps" ) ;
		WinAPIData.Win32Func.joyGetPosExFunc = ( MMRESULT ( WINAPI * )( UINT , LPJOYINFOEX  ) )GetProcAddress( WinAPIData.Win32Func.WinMMDLL, "joyGetPosEx" ) ;

//#ifdef UNICODE
		WinAPIData.Win32Func.joyGetDevCapsFunc = ( MMRESULT ( WINAPI * )( UINT , LPJOYCAPSW , UINT  ) )GetProcAddress( WinAPIData.Win32Func.WinMMDLL, "joyGetDevCapsW" ) ;
		WinAPIData.Win32Func.mciSendCommandFunc = ( MCIERROR ( WINAPI * )( MCIDEVICEID , UINT , DWORD_PTR , DWORD_PTR  ) )GetProcAddress( WinAPIData.Win32Func.WinMMDLL, "mciSendCommandW" ) ;
//#else
//		WinAPIData.Win32Func.joyGetDevCapsFunc = ( MMRESULT ( WINAPI * )( UINT , LPJOYCAPSA , UINT  ) )GetProcAddress( WinAPIData.Win32Func.WinMMDLL, "joyGetDevCapsA" ) ;
//		WinAPIData.Win32Func.mciSendCommandFunc = ( MCIERROR ( WINAPI * )( MCIDEVICEID , UINT , DWORD_PTR , DWORD_PTR  ) )GetProcAddress( WinAPIData.Win32Func.WinMMDLL, "mciSendCommandA" ) ;
//#endif
	}

	// Kernel32 DLL をロードする
	if( WinAPIData.Win32Func.Kernel32DLL == NULL )
	{
		WinAPIData.Win32Func.Kernel32DLL = LoadLibraryW( L"kernel32.dll" ) ;
		WinAPIData.Win32Func.GetModuleHandleWFunc = ( HMODULE ( WINAPI * )( LPCWSTR ) )GetProcAddress( WinAPIData.Win32Func.Kernel32DLL, "GetModuleHandleW" ) ;
		WinAPIData.Win32Func.VerifyVersionInfoWFunc = ( BOOL ( WINAPI * )( LPOSVERSIONINFOEXW, DWORD, DWORDLONG ) )GetProcAddress( WinAPIData.Win32Func.Kernel32DLL, "VerifyVersionInfoW" ) ;
		WinAPIData.Win32Func.VerSetConditionMaskFunc = ( ULONGLONG ( WINAPI * )( ULONGLONG, DWORD, BYTE ) )GetProcAddress( WinAPIData.Win32Func.Kernel32DLL, "VerSetConditionMask" ) ;
	}

	// Old32DLL DLL をロードする
	if( WinAPIData.Win32Func.Old32DLL == NULL )
	{
		WinAPIData.Win32Func.Old32DLL = LoadLibraryW( L"ole32.dll" ) ;
		WinAPIData.Win32Func.CoCreateInstanceFunc = ( HRESULT ( WINAPI * )( REFCLSID , IUnknown * , DWORD , REFIID , LPVOID * ) )GetProcAddress( WinAPIData.Win32Func.Old32DLL, "CoCreateInstance" ) ;
		WinAPIData.Win32Func.CoTaskMemAllocFunc = ( LPVOID ( WINAPI * )( SIZE_T  ) )GetProcAddress( WinAPIData.Win32Func.Old32DLL, "CoTaskMemAlloc" ) ;
		WinAPIData.Win32Func.CoTaskMemFreeFunc = ( void ( WINAPI * )( LPVOID  ) )GetProcAddress( WinAPIData.Win32Func.Old32DLL, "CoTaskMemFree" ) ;
		WinAPIData.Win32Func.CoInitializeExFunc = ( HRESULT ( WINAPI * )( LPVOID, DWORD ) )GetProcAddress( WinAPIData.Win32Func.Old32DLL, "CoInitializeEx" ) ;
		WinAPIData.Win32Func.CoFreeUnusedLibrariesFunc = ( void ( WINAPI * )( void ) )GetProcAddress( WinAPIData.Win32Func.Old32DLL, "CoFreeUnusedLibraries" ) ;
		WinAPIData.Win32Func.CoUninitializeFunc = ( void ( WINAPI * )( void ) )GetProcAddress( WinAPIData.Win32Func.Old32DLL, "CoUninitialize" ) ;
	}

	// Comctl32DLL DLL をロードする
	if( WinAPIData.Win32Func.Comctl32DLL == NULL )
	{
		WinAPIData.Win32Func.Comctl32DLL = LoadLibraryW( L"comctl32.dll" ) ;
		if( WinAPIData.Win32Func.Comctl32DLL != NULL )
		{
			WinAPIData.Win32Func.InitCommonControlsFunc = ( void ( WINAPI * )( VOID ) )GetProcAddress( WinAPIData.Win32Func.Comctl32DLL, "InitCommonControls" ) ;
		}
	}

	// User32DLL をロードする
	if( WinAPIData.Win32Func.User32DLL == NULL )
	{
		WinAPIData.Win32Func.User32DLL = LoadLibraryW( L"User32.dll" ) ;
		WinAPIData.Win32Func.WINNLSEnableIME_Func = ( BOOL ( WINAPI * )( HWND, BOOL ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "WINNLSEnableIME" ) ;
		WinAPIData.Win32Func.UpdateLayeredWindow = ( BOOL ( WINAPI * )( HWND, HDC, POINT*, SIZE*, HDC, POINT*, COLORREF, BLENDFUNCTION*, DWORD ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "UpdateLayeredWindow" ) ;
		WinAPIData.Win32Func.CreateWindowExWFunc  = ( HWND ( WINAPI * )( DWORD, LPCWSTR, LPCWSTR, DWORD, int, int, int, int, HWND, HMENU, HINSTANCE, LPVOID ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "CreateWindowExW" ) ;
		WinAPIData.Win32Func.EnumDisplayDevicesWFunc = ( BOOL ( WINAPI * )( LPCWSTR, DWORD, PDISPLAY_DEVICEW, DWORD ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "EnumDisplayDevicesW" ) ;

		WinAPIData.Win32Func.CloseTouchInputHandleFunc = ( BOOL ( WINAPI * )( D_HTOUCHINPUT ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "CloseTouchInputHandle" ) ;
		WinAPIData.Win32Func.GetTouchInputInfoFunc     = ( BOOL ( WINAPI * )( D_HTOUCHINPUT, UINT, D_PTOUCHINPUT, int ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "GetTouchInputInfo" ) ;
		WinAPIData.Win32Func.IsTouchWindowFunc         = ( BOOL ( WINAPI * )( HWND, ULONG *) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "IsTouchWindow" ) ;
		WinAPIData.Win32Func.RegisterTouchWindowFunc   = ( BOOL ( WINAPI * )( HWND, ULONG ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "RegisterTouchWindow" ) ;
		WinAPIData.Win32Func.UnregisterTouchWindowFunc = ( BOOL ( WINAPI * )( HWND) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "UnregisterTouchWindow" ) ;
	}

	// Gdi32DLL をロードする
	if( WinAPIData.Win32Func.GDI32DLL == NULL )
	{
		WinAPIData.Win32Func.GDI32DLL = LoadLibraryW( L"gdi32.dll" ) ;
		if( WinAPIData.Win32Func.GDI32DLL != NULL )
		{
			WinAPIData.Win32Func.AddFontMemResourceExFunc    = ( HANDLE	( WINAPI * )( LPVOID, DWORD, void /* DESIGNVECTOR */ *, DWORD * ) )GetProcAddress( WinAPIData.Win32Func.GDI32DLL, "AddFontMemResourceEx" ) ;
			WinAPIData.Win32Func.RemoveFontMemResourceExFunc = ( int    ( WINAPI * )( HANDLE ) )GetProcAddress( WinAPIData.Win32Func.GDI32DLL, "RemoveFontMemResourceEx" ) ;
		}
	}

	// NTDLL をロードする
	if( WinAPIData.Win32Func.NTDLL == NULL )
	{
		WinAPIData.Win32Func.NTDLL = LoadLibraryW( L"ntdll.dll" ) ;
		if( WinAPIData.Win32Func.NTDLL != NULL )
		{
			WinAPIData.Win32Func.RtlGetVersionFunc = ( void ( WINAPI * )( LPOSVERSIONINFOEXW ) )GetProcAddress( WinAPIData.Win32Func.NTDLL, "RtlGetVersion" ) ;
		}
	}

	// DwmApiDLL をロードする
	if( WinAPIData.DwmApiDLL == NULL )
	{
		WinAPIData.DwmApiDLL = LoadLibraryW( L"dwmapi.dll" ) ;
		if( WinAPIData.DwmApiDLL != NULL )
		{
			WinAPIData.DF_DwmEnableComposition = ( HRESULT	( WINAPI * )( UINT ) )GetProcAddress( WinAPIData.DwmApiDLL, "DwmEnableComposition" ) ;
		}
	}

	// 終了
	return 0 ;
}

// WindowsOS の DLL を解放する
extern int ReleaseWinAPI( void )
{
	// DWMAPI.DLL の解放
	if( WinAPIData.DwmApiDLL )
	{
		FreeLibrary( WinAPIData.DwmApiDLL ) ;
		WinAPIData.DwmApiDLL = NULL ;
		WinAPIData.DF_DwmEnableComposition = NULL ;
	}

	// User32.DLL の解放
	if( WinAPIData.Win32Func.User32DLL )
	{
		FreeLibrary( WinAPIData.Win32Func.User32DLL ) ;
		WinAPIData.Win32Func.User32DLL = NULL ;
		WinAPIData.Win32Func.WINNLSEnableIME_Func = NULL ;
		WinAPIData.Win32Func.UpdateLayeredWindow = NULL ;
		WinAPIData.Win32Func.CreateWindowExWFunc = NULL ;
		WinAPIData.Win32Func.EnumDisplayDevicesWFunc = NULL ;
	}

#ifndef DX_NON_NETWORK
	// ws2_32.dll の解放
	if( WinAPIData.WinSockFunc.WinSockDLL )
	{
		FreeLibrary( WinAPIData.WinSockFunc.WinSockDLL ) ;
		WinAPIData.WinSockFunc.WinSockDLL = NULL ;
		WinAPIData.WinSockFunc.WSAGetLastErrorFunc = NULL ;
		WinAPIData.WinSockFunc.WSAStartupFunc = NULL ;
		WinAPIData.WinSockFunc.WSACleanupFunc = NULL ;
		WinAPIData.WinSockFunc.WSAAsyncSelectFunc = NULL ;
		WinAPIData.WinSockFunc.getaddrinfoFunc = NULL ;
		WinAPIData.WinSockFunc.gethostbyaddrFunc = NULL ;
		WinAPIData.WinSockFunc.gethostbynameFunc = NULL ;
		WinAPIData.WinSockFunc.gethostnameFunc = NULL ;
		WinAPIData.WinSockFunc.ntohsFunc = NULL ;
		WinAPIData.WinSockFunc.htonsFunc = NULL ;
		WinAPIData.WinSockFunc.connectFunc = NULL ;
		WinAPIData.WinSockFunc.socketFunc = NULL ;
		WinAPIData.WinSockFunc.sendFunc = NULL ;
		WinAPIData.WinSockFunc.sendtoFunc = NULL ;
		WinAPIData.WinSockFunc.recvfromFunc = NULL ;
		WinAPIData.WinSockFunc.acceptFunc = NULL ;
		WinAPIData.WinSockFunc.closesocketFunc = NULL ;
		WinAPIData.WinSockFunc.shutdownFunc = NULL ;
		WinAPIData.WinSockFunc.listenFunc = NULL ;
		WinAPIData.WinSockFunc.bindFunc = NULL ;
		WinAPIData.WinSockFunc.inet_addrFunc = NULL ;
		WinAPIData.WinSockFunc.recvFunc = NULL ;
	}
#endif

#ifndef DX_NON_KEYEX
	// Imm32DLL を解放する
	if( WinAPIData.ImmFunc.Imm32DLL )
	{
		FreeLibrary( WinAPIData.ImmFunc.Imm32DLL ) ;
		WinAPIData.ImmFunc.Imm32DLL = NULL ;
		WinAPIData.ImmFunc.ImmGetContextFunc = NULL ;
		WinAPIData.ImmFunc.ImmReleaseContextFunc = NULL ;
		WinAPIData.ImmFunc.ImmGetOpenStatusFunc = NULL ;
		WinAPIData.ImmFunc.ImmGetConversionStatusFunc = NULL ;
		WinAPIData.ImmFunc.ImmNotifyIMEFunc = NULL ;
		WinAPIData.ImmFunc.ImmSetOpenStatusFunc = NULL ;

		WinAPIData.ImmFunc.ImmGetCandidateListFunc = NULL ;
		WinAPIData.ImmFunc.ImmGetCompositionStringFunc = NULL ;
		WinAPIData.ImmFunc.ImmSetCompositionStringFunc = NULL ;
	}
#endif

	// WinMM DLL を解放する
	if( WinAPIData.Win32Func.WinMMDLL )
	{
		FreeLibrary( WinAPIData.Win32Func.WinMMDLL ) ;
		WinAPIData.Win32Func.WinMMDLL = NULL ;
		WinAPIData.Win32Func.timeSetEventFunc = NULL ;
		WinAPIData.Win32Func.timeKillEventFunc = NULL ;
		WinAPIData.Win32Func.timeBeginPeriodFunc = NULL ;
		WinAPIData.Win32Func.timeEndPeriodFunc = NULL ;
		WinAPIData.Win32Func.timeGetTimeFunc = NULL ;
		WinAPIData.Win32Func.timeGetDevCapsFunc = NULL ;
		WinAPIData.Win32Func.joyGetPosExFunc = NULL ;

		WinAPIData.Win32Func.joyGetDevCapsFunc = NULL ;
		WinAPIData.Win32Func.mciSendCommandFunc = NULL ;
	}

	// Kernel32 DLL を解放する
	if( WinAPIData.Win32Func.Kernel32DLL )
	{
		FreeLibrary( WinAPIData.Win32Func.Kernel32DLL ) ;
		WinAPIData.Win32Func.Kernel32DLL = NULL ;
		WinAPIData.Win32Func.GetModuleHandleWFunc = NULL ;
	}

	// Ole32 DLL を解放する
	if( WinAPIData.Win32Func.Old32DLL )
	{
		FreeLibrary( WinAPIData.Win32Func.Old32DLL ) ;
		WinAPIData.Win32Func.Old32DLL = NULL ;
		WinAPIData.Win32Func.CoCreateInstanceFunc = NULL ;
		WinAPIData.Win32Func.CoTaskMemAllocFunc = NULL ;
		WinAPIData.Win32Func.CoTaskMemFreeFunc = NULL ;
		WinAPIData.Win32Func.CoInitializeExFunc = NULL ;
		WinAPIData.Win32Func.CoFreeUnusedLibrariesFunc = NULL ;
		WinAPIData.Win32Func.CoUninitializeFunc = NULL ;
	}

	// Comctl32 DLL を解放する
	if( WinAPIData.Win32Func.Comctl32DLL )
	{
		FreeLibrary( WinAPIData.Win32Func.Comctl32DLL ) ;
		WinAPIData.Win32Func.Comctl32DLL = NULL ;
		WinAPIData.Win32Func.InitCommonControlsFunc = NULL ;
	}

	// 終了
	return 0 ;
}













#ifndef DX_NON_ASYNCLOAD
extern int WinAPI_CoCreateInstance_ASyncCallback( ASYNCLOAD_MAINTHREAD_REQUESTINFO *Info )
{
	GUID rclsid ;
	GUID riid ;

	rclsid.Data1      = ( unsigned long  )    Info->Data[ 0 ] ;
	rclsid.Data2      = ( unsigned short )(   Info->Data[ 1 ] & 0xffff ) ;
	rclsid.Data3      = ( unsigned short )( ( Info->Data[ 1 ] >> 16 ) & 0xffff ) ;
	rclsid.Data4[ 0 ] = ( unsigned char  )(   Info->Data[ 2 ]         & 0xff ) ;
	rclsid.Data4[ 1 ] = ( unsigned char  )( ( Info->Data[ 2 ] >>  8 ) & 0xff ) ;
	rclsid.Data4[ 2 ] = ( unsigned char  )( ( Info->Data[ 2 ] >> 16 ) & 0xff ) ;
	rclsid.Data4[ 3 ] = ( unsigned char  )( ( Info->Data[ 2 ] >> 24 ) & 0xff ) ;
	rclsid.Data4[ 4 ] = ( unsigned char  )(   Info->Data[ 3 ]         & 0xff ) ;
	rclsid.Data4[ 5 ] = ( unsigned char  )( ( Info->Data[ 3 ] >>  8 ) & 0xff ) ;
	rclsid.Data4[ 6 ] = ( unsigned char  )( ( Info->Data[ 3 ] >> 16 ) & 0xff ) ;
	rclsid.Data4[ 7 ] = ( unsigned char  )( ( Info->Data[ 3 ] >> 24 ) & 0xff ) ;

	riid.Data1        = ( unsigned long  )    Info->Data[ 6 ] ;
	riid.Data2        = ( unsigned short )(   Info->Data[ 7 ] & 0xffff ) ;
	riid.Data3        = ( unsigned short )( ( Info->Data[ 7 ] >> 16 ) & 0xffff ) ;
	riid.Data4[ 0 ]   = ( unsigned char  )(   Info->Data[ 8 ]         & 0xff ) ;
	riid.Data4[ 1 ]   = ( unsigned char  )( ( Info->Data[ 8 ] >>  8 ) & 0xff ) ;
	riid.Data4[ 2 ]   = ( unsigned char  )( ( Info->Data[ 8 ] >> 16 ) & 0xff ) ;
	riid.Data4[ 3 ]   = ( unsigned char  )( ( Info->Data[ 8 ] >> 24 ) & 0xff ) ;
	riid.Data4[ 4 ]   = ( unsigned char  )(   Info->Data[ 9 ]         & 0xff ) ;
	riid.Data4[ 5 ]   = ( unsigned char  )( ( Info->Data[ 9 ] >>  8 ) & 0xff ) ;
	riid.Data4[ 6 ]   = ( unsigned char  )( ( Info->Data[ 9 ] >> 16 ) & 0xff ) ;
	riid.Data4[ 7 ]   = ( unsigned char  )( ( Info->Data[ 9 ] >> 24 ) & 0xff ) ;

	return WinAPI_CoCreateInstance_ASync(
		rclsid,
		( IUnknown * )Info->Data[ 4 ],
		( DWORD )Info->Data[ 5 ],
		riid,
		( LPVOID * )Info->Data[ 10 ],
		FALSE ) ;
}
#endif // DX_NON_ASYNCLOAD

extern HRESULT WinAPI_CoCreateInstance_ASync( REFCLSID rclsid, IUnknown * pUnkOuter, DWORD dwClsContext, REFIID riid, LPVOID *ppv, int ASyncThread )
{
#ifndef DX_NON_ASYNCLOAD
	if( ASyncThread )
	{
		ASYNCLOAD_MAINTHREAD_REQUESTINFO AInfo ;

		AInfo.Function = WinAPI_CoCreateInstance_ASyncCallback ;
		AInfo.Data[  0 ] = ( DWORD_PTR )rclsid.Data1 ;
		AInfo.Data[  1 ] = ( DWORD_PTR )( rclsid.Data2 | ( ( ( DWORD )rclsid.Data3 ) ) << 16 );
		AInfo.Data[  2 ] = ( DWORD_PTR )( rclsid.Data4[ 0 ] | ( ( ( DWORD )rclsid.Data4[ 1 ] ) << 8 ) | ( ( ( DWORD )rclsid.Data4[ 2 ] ) << 16 ) | ( ( ( DWORD )rclsid.Data4[ 3 ] ) << 24 ) ) ;
		AInfo.Data[  3 ] = ( DWORD_PTR )( rclsid.Data4[ 4 ] | ( ( ( DWORD )rclsid.Data4[ 5 ] ) << 8 ) | ( ( ( DWORD )rclsid.Data4[ 6 ] ) << 16 ) | ( ( ( DWORD )rclsid.Data4[ 7 ] ) << 24 ) ) ;
		AInfo.Data[  4 ] = ( DWORD_PTR )pUnkOuter ;
		AInfo.Data[  5 ] = ( DWORD_PTR )dwClsContext ;
		AInfo.Data[  6 ] = ( DWORD_PTR )riid.Data1 ;
		AInfo.Data[  7 ] = ( DWORD_PTR )( riid.Data2 | ( ( ( DWORD )riid.Data3 ) ) << 16 );
		AInfo.Data[  8 ] = ( DWORD_PTR )( riid.Data4[ 0 ] | ( ( ( DWORD )riid.Data4[ 1 ] ) << 8 ) | ( ( ( DWORD )riid.Data4[ 2 ] ) << 16 ) | ( ( ( DWORD )riid.Data4[ 3 ] ) << 24 ) ) ;
		AInfo.Data[  9 ] = ( DWORD_PTR )( riid.Data4[ 4 ] | ( ( ( DWORD )riid.Data4[ 5 ] ) << 8 ) | ( ( ( DWORD )riid.Data4[ 6 ] ) << 16 ) | ( ( ( DWORD )riid.Data4[ 7 ] ) << 24 ) ) ;
		AInfo.Data[ 10 ] = ( DWORD_PTR )ppv ;
		return AddASyncLoadRequestMainThreadInfo( &AInfo ) ;
	}
#endif // DX_NON_ASYNCLOAD

	return WinAPIData.Win32Func.CoCreateInstanceFunc( rclsid, pUnkOuter, dwClsContext, riid, ppv );
}


#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

