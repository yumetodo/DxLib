// -------------------------------------------------------------------------------
// 
// 		ＤＸライブラリ		システムプログラムヘッダファイル
// 
// 				Ver 3.16d
// 
// -------------------------------------------------------------------------------

#ifndef __DXSYSTEM_H__
#define __DXSYSTEM_H__

// インクルード ------------------------------------------------------------------
#include "DxCompileConfig.h"
#include "DxLib.h"
#include "DxStatic.h"
#include "DxBaseFunc.h"

#ifdef __WINDOWS__
#include "Windows/DxSystemWin.h"
#endif

#ifdef __psp2__
#include "PSVita/DxSystemPSVita.h"
#endif

#ifdef __ORBIS__
#include "PS4/DxSystemPS4.h"
#endif

#ifdef __ANDROID__
#include "Android/DxSystemAndroid.h"
#endif

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// マクロ定義 --------------------------------------------------------------------


// 構造体定義 --------------------------------------------------------------------

// システムデータ構造体
struct DXSYSTEMDATA
{
	int						DxLib_InitializeFlag ;				// ＤＸライブラリの初期化が完了しているかフラグ
	int						DxLib_RunInitializeFlag ;			// ＤＸライブラリの初期化中か、フラグ

	int						NotWinFlag ;						// ＤＸライブラリのウインドウ関連機能を使用しないフラグ
	int						NotDrawFlag ;						// 描画機能を使用しないフラグ
	int						NotSoundFlag ;						// サウンド機能を使用しないフラグ
	int						NotInputFlag ;						// 入力状態取得機能を使用しないフラグ

	int						EndRequestFlag ;					// 終了リクエストフラグ
	int						ASyncLoadFlag ;						// 非同期読み込みフラグ
} ;

// テーブル-----------------------------------------------------------------------

// 内部大域変数宣言 --------------------------------------------------------------

extern DXSYSTEMDATA DxSysData ;

// 関数プロトタイプ宣言-----------------------------------------------------------

// 初期化・終了関係
extern	int			DxLib_SysInit( void ) ;													// DxSysData 関係の初期化を行う
extern	int			DxLib_EndRequest( void ) ;												// 終了リクエストを行う
extern	int			DxLib_GetEndRequest( void ) ;											// 終了リクエストの状態を取得する

// エラー処理関数
extern	int			DxLib_Error(        const wchar_t *ErrorStr ) ;							// ライブラリのエラー処理を行う
extern	int			DxLib_ErrorUTF16LE( const char    *ErrorStr ) ;							// ライブラリのエラー処理を行う( UTF16LE版 )
extern	int			DxLib_FmtError(        const wchar_t *FormatString, ... ) ;				// 書式付きライブラリのエラー処理を行う
extern	int			DxLib_FmtErrorUTF16LE( const char    *FormatString, ... ) ;				// 書式付きライブラリのエラー処理を行う( UTF16LE版 )

// 非同期読み込み関係
extern	int			GetASyncLoadFlag( void ) ;												// 非同期読み込みを行うかどうかを取得する

#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

#endif // __DXSYSTEM_H__

