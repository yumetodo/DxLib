// -------------------------------------------------------------------------------
// 
// 		ＤＸライブラリ		WindowsOS用動画プログラムヘッダファイル
// 
// 				Ver 3.16d
// 
// -------------------------------------------------------------------------------

#ifndef __DXMOVIEWIN_H__
#define __DXMOVIEWIN_H__

#include "../DxCompileConfig.h"

#ifndef DX_NON_MOVIE

// インクルード ------------------------------------------------------------------
#include "DxUseCStrmBaseFilter.h"
#include "DxGuid.h"
#include "../DxLib.h"
#include "../DxFile.h"

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// マクロ定義 --------------------------------------------------------------------

// 構造体定義 --------------------------------------------------------------------

// ムービーグラフィック環境依存データ型
struct MOVIEGRAPH_PF
{
	int						Dummy ;

#ifndef DX_NON_DSHOW_MOVIE
	D_IGraphBuilder			*pGraph ;						// フィルタグラフマネージャ
	D_IMediaControl			*pMediaControl ;				// メディアコントローラ
	D_IMediaSeeking			*pMediaSeeking ;				// メディアシーキング
	D_IBasicAudio			*pBasicAudio ;					// BasicAudio インターフェイス

//	D_ISampleGrabber		*SampleGrabber ;				// サンプルグラッバオブジェクト

	D_CMovieRender			*pMovieImage ;					// 動画のイメージ

	D_STREAM_TIME			FrameTime ;						// １フレーム当たりの時間
	D_STREAM_TIME			OldTime ;						// 前回描画時の時間
	D_STREAM_TIME			StartTime ;						// 再生開始時刻

	D_STREAM_TIME			BackUpTime ;					// 再生を止めた時間
	int						UseTemporaryFile ;				// テンポラリファイルを使用しているかどうか、フラグ
	wchar_t					FileName[ FILEPATH_MAX ] ;		// ファイルネーム
#endif // DX_NON_DSHOW_MOVIE
} ;

// 内部大域変数宣言 --------------------------------------------------------------

// 関数プロトタイプ宣言-----------------------------------------------------------

#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

#endif // DX_NON_MOVIE

#endif // __DXMOVIEWIN_H__
