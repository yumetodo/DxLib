//-----------------------------------------------------------------------------
// 
// 		ＤＸライブラリ		Windows用ログプログラム
// 
//  	Ver 3.16d
// 
//-----------------------------------------------------------------------------

// ＤＸライブラリ作成時用定義
#define __DX_MAKE

#include "../DxCompileConfig.h"

// インクルード ---------------------------------------------------------------
#include "DxLogWin.h"

#ifndef DX_NON_LOG

#include "../DxStatic.h"
#include "../DxLib.h"
#include "../DxLog.h"
#include "../DxBaseFunc.h"
#include "../DxMemory.h"
#include "../DxGraphics.h"
#include "DxWindow.h"
#include "DxWinAPI.h"
#include "DxGraphicsWin.h"

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// マクロ定義 -----------------------------------------------------------------

// 構造体宣言 -----------------------------------------------------------------

// データ定義 -----------------------------------------------------------------

// 関数宣言 -------------------------------------------------------------------

// プログラム -----------------------------------------------------------------

// エラー書き出しファイルを初期化する処理の環境依存部分
extern int ErrorLogInitialize_PF( const wchar_t *LogFilePath )
{
	HANDLE fp ;

	// エラーログファイルを再作成する
	DeleteFileW( LogFilePath ) ;
	fp = CreateFileW( LogFilePath, GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL ) ;

	BYTE BOM[ 2 ] = { 0xff, 0xfe } ;
	DWORD WriteSize ;
	WriteFile( fp, &BOM, 2, &WriteSize, NULL ) ;

	if( fp != NULL )
	{
		CloseHandle( fp ) ;
	}

	// 終了
	return 0 ;
}

// エラー文書を書き出す処理の環境依存部分
extern int ErrorLogAdd_WCHAR_T_PF( const wchar_t *LogFilePath, const wchar_t *ErrorStr )
{
	HANDLE	fp ;
	DWORD	WriteSize ;

	if( WinAPIData.Win32Func.WinMMDLL == NULL )
	{
		LoadWinAPI() ;
		if( WinAPIData.Win32Func.WinMMDLL == NULL )
		{
			return -1 ;
		}
	}

	// エラーログファイルを開く
	fp = CreateFileW( LogFilePath, GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL ) ;
	if( fp != NULL )
	{
		// ファイルアクセス位置をファイルの末端に移動
		SetFilePointer( fp, 0, NULL, FILE_END ) ;

		// エラーログファイルに書き出す
		WriteFile( fp, ErrorStr, ( DWORD )( _WCSLEN( ErrorStr ) * sizeof( wchar_t ) ), &WriteSize, NULL ) ;

		// エラーログをアウトプットに書き出す
		OutputDebugStringW( ErrorStr ) ;

		// ファイルを閉じる
		CloseHandle( fp ) ;
	}
	
	// 終了
	return 0 ;
}

// ログ機能の初期化を行うかどうかを取得する
extern int IsInitializeLog( void )
{
	if( WinData.MainWindow == NULL )
	{
		return FALSE ;
	}

#ifndef DX_NON_GRAPHICS
	// ハードウエア機能を使っているかどうかで処理を分岐
	if( GSYS.Setting.ValidHardware )
	{
		switch( GRAWIN.Setting.UseGraphicsAPI )
		{
#ifndef DX_NON_DIRECT3D11
		case GRAPHICS_API_DIRECT3D11_WIN32 :
			return D3D11Device_IsValid() ? TRUE : FALSE ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
		case GRAPHICS_API_DIRECT3D9_WIN32 :
			return NS_GetUseDDrawObj() == NULL ? FALSE : TRUE ;
#endif // DX_NON_DIRECT3D9

		default :
			return FALSE ;
		}
	}
	else
	{
		// ＤＸライブラリが終了状態に入っていたら初期化しない
		return NS_GetUseDDrawObj() == NULL || WinData.MainWindow == NULL ? FALSE : TRUE ;
	}
#else // DX_NON_GRAPHICS
	// ＤＸライブラリが終了状態に入っていたら初期化しない
	return WinData.MainWindow == NULL ? FALSE : TRUE ;
#endif // DX_NON_GRAPHICS

	return TRUE ;
}


#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

#endif // DX_NON_LOG

