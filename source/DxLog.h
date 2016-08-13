// -------------------------------------------------------------------------------
// 
// 		ＤＸライブラリ		ログプログラムヘッダファイル
// 
// 				Ver 3.16d
// 
// -------------------------------------------------------------------------------

#ifndef __DXLOG_H__
#define __DXLOG_H__

// インクルード ------------------------------------------------------------------

#include "DxCompileConfig.h"
#include "DxFile.h"

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// マクロ定義 --------------------------------------------------------------------

//ＤＸライブラリ内のリテラル文字列を極力減らすかどうかの定義を反映する為のマクロ
#if !defined( DX_NON_LITERAL_STRING ) && !defined( DX_NON_LOG )
	#define DXSTRING( x )					x
	#define DXST_ERRORLOG_ADD(x)			NS_ErrorLogAdd(x)
	#define DXST_ERRORLOG_ADDA(x)			ErrorLogAddA(x)
	#define DXST_ERRORLOG_ADDW(x)			ErrorLogAddW(x)
	#define DXST_ERRORLOG_ADDUTF16LE(x)		ErrorLogAddUTF16LE(x)
	#define DXST_ERRORLOGFMT_ADD(x)			NS_ErrorLogFmtAdd x
	#define DXST_ERRORLOGFMT_ADDA(x)		ErrorLogFmtAddA x
	#define DXST_ERRORLOGFMT_ADDW(x)		ErrorLogFmtAddW x
	#define DXST_ERRORLOGFMT_ADDUTF16LE(x)	ErrorLogFmtAddUTF16LE x
	#define DXST_ERRORLOG_TABADD			NS_ErrorLogTabAdd()
	#define DXST_ERRORLOG_TABSUB			NS_ErrorLogTabSub()
	#define DXERRORNETLOG_ADD(x)			ErrorNetLogAdd(x)
	#define DXERRORNETWORK(x)				ErrorNetWork x
#else
	#define DXSTRING( x )					" "
	#define DXST_ERRORLOG_ADD(x)			(-1)
	#define DXST_ERRORLOG_ADDA(x)			(-1)
	#define DXST_ERRORLOG_ADDW(x)			(-1)
	#define DXST_ERRORLOG_ADDUTF16LE(x)		(-1)
	#define DXST_ERRORLOGFMT_ADD(x)			(-1)
	#define DXST_ERRORLOGFMT_ADDA(x)		(-1)
	#define DXST_ERRORLOGFMT_ADDW(x)		(-1)
	#define DXST_ERRORLOGFMT_ADDUTF16LE(x)	(-1)
	#define DXST_ERRORLOG_TABADD			(0)
	#define DXST_ERRORLOG_TABSUB			(0)
	#define DXERRORNETLOG_ADD(x)			(-1)
	#define DXERRORNETWORK(x)				(-1)
#endif


#ifndef DX_NON_LOG

#define LOG_MAXHEIGHT				(80)				// １画面に入るログの最大行
#define LOG_MAXLENGTH				(255)				// １行のログに使える文字列の長さ

// ログ処理用定義
#define LOG_FONTSIZE				(16)				// ログに使うフォントのサイズ
#define LOG_FONTTICK				(4)					// ログ表示に使うフォントの太さ
#define LOG_TABWIDTH				(4)					// タブ一つ分の幅

// 構造体定義 --------------------------------------------------------------------

// ログ処理データ構造体
struct LOGDATA
{
	int						InitializeFlag ;					// 初期化完了フラグ

	int						NonUseTimeStampFlag ;				// タイムスタンプを出力しないかどうかのフラグ
	int						NotLogOutFlag ;						// ログ出力を行うかフラグ(TRUEで行わない)
	int						LogStartTime ;						// 起動時のタイムスタンプ
	wchar_t					LogOutDirectory[ FILEPATH_MAX ] ;	// ログ出力を行うファイルを保存するディレクトリ
	int						UseLogDateName ;					// ログ出力を行うファイルに日付をつけるかどうかのフラグ
	wchar_t					LogFileName[256] ;					// ログ出力を行うファイルの名前
	int						ErTabNum ;							// エラーログ排出時のタブの数
	int						ErTabStop ;							// タブ排出ストップフラグ

#ifndef DX_NON_PRINTF_DX
	int						LogInitializeFlag ;					// ログデータが初期化されているかフラグ
	int						LogUserFontHandle ;					// ユーザー指定のログ描画用フォントハンドル
	int						LogFontHandle ;						// ログ描画用フォントハンドル
	int						LogFontHandleLostFlag ;				// フォントハンドルが削除された際に立つフラグ
	int						LogX ,LogY ;						// 現在のログのカーソル位置
	wchar_t					LogString[LOG_MAXHEIGHT][LOG_MAXLENGTH] ; // ログ文字列
	int						LogDrawFlag ;						// ログを画面上に表示するか、フラグ
	int						LogFontSize ;						// ログフォントのサイズ
	int						LogTabWidth ;						// ログのタブの幅
	int						LogDrawWidth ;						// 現在カーソルがいる行の描画幅
	int						LogFileOutFlag ;					// ログをファイル出力するか、フラグ
	RECT					LogDrawArea ;						// ログを描画する領域
#endif // DX_NON_PRINTF_DX

} ;

// 内部大域変数宣言 --------------------------------------------------------------

extern LOGDATA LogData ;

// 関数プロトタイプ宣言-----------------------------------------------------------

// ログ出力機能関数
extern	int			ErrorLogInitialize(		void ) ;										// エラー書き出しファイルを初期化する
extern	int			ErrorLogTerminate(		void ) ;										// エラー書き出しファイルの後始末
extern	int			ErrorLogAddA(			const char    *ErrorStr ) ;						// エラー文書を書き出す( char版 )
extern	int			ErrorLogAddW(			const wchar_t *ErrorStr ) ;						// エラー文書を書き出す( wchar_t版 )
extern	int			ErrorLogAddUTF16LE(		const char    *ErrorStr ) ;						// エラー文書を書き出す( UTF16LE版 )
extern 	int			ErrorLogFmtAddA(		const char	  *FormatString , ... ) ;			// 書式付きエラー文書を書き出す( char版 )
extern 	int			ErrorLogFmtAddW(		const wchar_t *FormatString , ... ) ;			// 書式付きエラー文書を書き出す( wchar_t版 )
extern 	int			ErrorLogFmtAddUTF16LE(	const char    *FormatString , ... ) ;			// 書式付きエラー文書を書き出す( UTF16LE版 )

// wchar_t版関数
extern	int			ErrorLogAdd_WCHAR_T(	const wchar_t *ErrorStr ) ;						// ログファイル( Log.txt ) に文字列を出力する
extern 	int			ErrorLogFmtAdd_WCHAR_T(	const wchar_t *FormatString , ... ) ;			// 書式付きで ログファイル( Log.txt ) に文字列を出力する( 書式は printf と同じ )
extern 	int			AppLogAdd_WCHAR_T(		const wchar_t *String , ... ) ;					// ErrorLogFmtAdd と同じ機能の関数

extern	int			SetApplicationLogSaveDirectory_WCHAR_T(	const wchar_t *DirectoryPath );	// ログファイル( Log.txt ) を保存するディレクトリパスを設定する


// 環境依存関数
extern	int			ErrorLogInitialize_PF( const wchar_t *LogFilePath ) ;					// エラー書き出しファイルを初期化する処理の環境依存部分
extern	int			ErrorLogAdd_WCHAR_T_PF( const wchar_t *LogFilePath, const wchar_t *ErrorStr ) ;	// エラー文書を書き出す処理の環境依存部分



#ifndef DX_NON_PRINTF_DX

extern	int			IsInitializeLog( void ) ;												// ログ機能の初期化を行うかどうかを取得する
extern	int			InitializeLog( void ) ;													// ログ機能の初期化
extern	int			TerminateLog( void ) ;													// ログ処理の後始末を行う
extern	int			DrawLog( void ) ;														// ログを描画する
extern 	int			printfDxBase( const TCHAR *String ) ;									// printfDx の書式文字列ではない文字列を引数とするバージョン

// wchar_t版関数
extern 	int			printfDx_WCHAR_T(		 const wchar_t *FormatString , ... ) ;				// printf と同じ引数で画面に文字列を表示するための関数
extern 	int			printfDxBase_WCHAR_T(	 const wchar_t *String ) ;							// printfDx_WCHAR_T の書式文字列ではない文字列を引数とするバージョン
extern 	int			putsDx_WCHAR_T(			 const wchar_t *String , int NewLine ) ;			// puts と同じ引数で画面に文字列を表示するための関数

#endif // DX_NON_PRINTF_DX

#endif // DX_NON_LOG

#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

#endif // __DXLOG_H__

