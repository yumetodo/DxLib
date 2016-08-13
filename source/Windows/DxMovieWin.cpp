//-----------------------------------------------------------------------------
// 
// 		ＤＸライブラリ		WindowsOS用動画プログラム
// 
//  	Ver 3.16d
// 
//-----------------------------------------------------------------------------

// ＤＸライブラリ作成時用定義
#define __DX_MAKE

#include "../DxCompileConfig.h"

#ifndef DX_NON_MOVIE

// インクルード----------------------------------------------------------------
#include "DxMovieWin.h"
#include "DxWinAPI.h"
#include "DxFileWin.h"
#include "../DxGraphics.h"
#include "../DxLog.h"
#include "../DxMovie.h"
#include "../DxSystem.h"

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// マクロ定義------------------------------------------------------------------

// 型定義----------------------------------------------------------------------

// データ宣言------------------------------------------------------------------

// 関数プロトタイプ宣言 -------------------------------------------------------

#ifndef DX_NON_DSHOW_MOVIE
// DirectShow を使用した動画ファイルのオープンを試みる
extern int DirectShowOpenMovieFile(
	MOVIEGRAPH * Movie,
	const wchar_t *FileName,
	int *Width,
	int *Height,
	int SurfaceMode,
	int ASyncThread
) ;
#endif

// プログラム------------------------------------------------------------------

// ムービー関連の管理処理の初期化の環境依存処理
extern int InitializeMovieManage_PF( void )
{
	return 0 ;
}

// ムービー関連の管理処理の後始末の環境依存処理
extern int TerminateMovieManage_PF( void )
{
	return 0 ;
}

#ifndef DX_NON_DSHOW_MOVIE

// DirectShow を使用した動画ファイルのオープンを試みる
extern int DirectShowOpenMovieFile(
	MOVIEGRAPH * Movie,
	const wchar_t *FileName,
	int *Width,
	int *Height,
	int SurfaceMode,
	int ASyncThread
)
{
	wchar_t ErStr[256] ;
	D_IAMMultiMediaStream *pAMStream = NULL;
	HRESULT hr ;

	Movie->NowImage.GraphData = NULL;
	Movie->YUVFlag = FALSE ;
	Movie->OverlayDestX = 0 ;
	Movie->OverlayDestY = 0 ;
	Movie->OverlayDestExRate = 0 ;
	Movie->OverlayDispFlag = 0 ;
	Movie->FirstUpdateFlag = FALSE ;
	_MEMSET( &Movie->OverlaySrcRect, 0, sizeof( RECT ) ) ;
	_MEMSET( &Movie->OverlayDestRect, 0, sizeof( RECT ) ) ;

	// もしオーバーレイが使えない場合はフルカラーにする
	if( SurfaceMode == DX_MOVIESURFACE_OVERLAY  )
	{
		SurfaceMode = DX_MOVIESURFACE_FULLCOLOR ;
	}

	// もし画面が３２ビットカラーモードでフルカラーモードを指定してきた場合はノーマルにする
	if( SurfaceMode == DX_MOVIESURFACE_FULLCOLOR && NS_GetColorBitDepth() == 32 ) SurfaceMode = DX_MOVIESURFACE_NORMAL ;

	SurfaceMode = DX_MOVIESURFACE_FULLCOLOR ;

	// グラフィックビルダーオブジェクトの作成
	if( ( FAILED( WinAPI_CoCreateInstance_ASync(CLSID_FILTERGRAPH, NULL, CLSCTX_INPROC, IID_IGRAPHBUILDER, (void **)&Movie->PF.pGraph, ASyncThread ) ) ) )
	{
		_WCSCPY_S( ErStr, sizeof( ErStr ), L"CoCreateInstance Error : CLSID_FilterGraph\n" ) ;
		goto ERROR_R ;
	}

	// Create the Texture Renderer object
	Movie->PF.pMovieImage = New_D_CMovieRender( NULL, &hr ) ;
    
    // Get a pointer to the IBaseFilter on the TextureRenderer, add it to graph
	hr = Movie->PF.pGraph->AddFilter( Movie->PF.pMovieImage, L"MovieRenderer" ) ;
    if( FAILED( hr ) )
    {
        _WCSCPY_S( ErStr, sizeof( ErStr ), L"Could not add renderer filter to graph!\n" ) ;
        return hr;
    }

	// BasicAudio インターフェイスを得る
	if( FAILED( Movie->PF.pGraph->QueryInterface( IID_IBASICAUDIO, ( void ** )&Movie->PF.pBasicAudio ) ) )
	{
		_WCSCPY_S( ErStr, sizeof( ErStr ), L"QueryInterface Error : IID_IBasicAudio\n" ) ;
		goto ERROR_R ;
	}

	// メディアコントローラオブジェクトを取得する
	if( FAILED( Movie->PF.pGraph->QueryInterface( IID_IMEDIACONTROL, ( void ** )&Movie->PF.pMediaControl ) ) )
	{
		_WCSCPY_S( ErStr, sizeof( ErStr ), L"QueryInterface Error : IID_IMediaControl\n" ) ;
		goto ERROR_R ;
	}

	// メディアシーキングオブジェクトを取得する
	if( FAILED( Movie->PF.pGraph->QueryInterface( IID_IMEDIASEEKING, ( void ** )&Movie->PF.pMediaSeeking ) ) )
	{
		_WCSCPY_S( ErStr, sizeof( ErStr ), L"QueryInterface Error : IID_IMediaSeeking\n" ) ;
		goto ERROR_R ;
	}

   	// ファイル名保存
	_WCSCPY_S( Movie->PF.FileName, sizeof( Movie->PF.FileName ), FileName ) ;

	// ディレクトリの区切りが / になっている箇所を \ に置き換える
	{
		wchar_t *wp ;

		for( wp = Movie->PF.FileName; *wp; wp++ )
		{
			if( *wp == L'/' )
			{
				*wp = '\\' ;
			}
		}
	}

	hr = Movie->PF.pGraph->RenderFile( Movie->PF.FileName, NULL ) ;
	if( FAILED( hr ) )
	{
		_WCSCPY_S( ErStr, sizeof( ErStr ), L"RenderFile faired!\n" ) ;

		DWORD_PTR fp ;
		HANDLE FileHandle ;
		void *TempBuffer ;
		size_t CompSize, MoveSize ;
		DWORD WriteSize ;
		size_t FileSize ;
		const DWORD BufferSize = 0x100000 ;

		_WCSCPY_S( ErStr, sizeof( ErStr ), L"Movie File Open Error : " ) ;
		_WCSCAT_S( ErStr, sizeof( ErStr ), FileName ) ;
		_WCSCAT_S( ErStr, sizeof( ErStr ), L"\n" ) ;

		// 既にテンポラリファイルを作成している場合は、
		// 更にテンポラリファイルを作成することはしない
		if( Movie->PF.UseTemporaryFile == TRUE )
			goto ERROR_R ;

		// ファイルが開けなかったらアーカイブされている可能性がある
#ifdef UNICODE
		fp = DX_FOPEN( Movie->PF.FileName ) ;
#else
		fp = DX_FOPEN( FileName ) ;
#endif
		if( fp == 0 ) goto ERROR_R ;
		
		// 開けた場合はテンポラリファイルに書き出す
		{
			// ファイルサイズの取得
			DX_FSEEK( fp, 0L, SEEK_END ) ;
			FileSize = ( size_t )DX_FTELL( fp ) ;
			DX_FSEEK( fp, 0L, SEEK_SET ) ;

			// 一時的にデータを格納するバッファを確保
			TempBuffer = DXALLOC( BufferSize ) ;
			if( TempBuffer == NULL )
			{
				DX_FCLOSE( fp ) ;
				goto ERROR_R ;
			}

			// テンポラリファイルの作成
			FileHandle = CreateTemporaryFile( Movie->PF.FileName, sizeof( Movie->PF.FileName ) ) ;

			if( FileHandle == NULL )
			{
				DX_FCLOSE( fp ) ;
				DXFREE( TempBuffer ) ;
				goto ERROR_R ;
			}
			Movie->PF.UseTemporaryFile = TRUE ;

			// テンポラリファイルにデータを書き出す
			CompSize = 0 ;
			while( CompSize < FileSize )
			{
				MoveSize = CompSize - FileSize ;
				if( MoveSize > BufferSize ) MoveSize = BufferSize ;

				DX_FREAD( TempBuffer, MoveSize, 1, fp ) ;
				WriteFile( FileHandle, TempBuffer, ( DWORD )MoveSize, &WriteSize, NULL ) ;

				if( MoveSize != WriteSize ) break ;
				CompSize += MoveSize ;
			}

			// ファイルを閉じ、メモリを解放する
			DX_FCLOSE( fp ) ;
			CloseHandle( FileHandle ) ;
			DXFREE( TempBuffer ) ;
		}

		// 改めてファイルを開く
		hr = Movie->PF.pGraph->RenderFile( Movie->PF.FileName, NULL ) ;
		if( FAILED( hr ) )
		{
			// テンポラリファイルを削除
			DeleteFileW( Movie->PF.FileName ) ;
			goto ERROR_R ;
		}
	}

	// １フレームあたりの時間を得る
	Movie->PF.pMediaSeeking->GetDuration( &Movie->PF.FrameTime ) ;
	if( Movie->PF.FrameTime == 0 )
	{
		Movie->PF.FrameTime = 10000000 / 60 ;
	}

	// 終了時間を取得する
	Movie->PF.pMediaSeeking->GetStopPosition( &Movie->StopTime ) ;

	// 画像イメージの情報をセットする
	Movie->NowImage.Width        = ( int )Movie->PF.pMovieImage->Width ;
	Movie->NowImage.Height       = ( int )Movie->PF.pMovieImage->Height ;
	Movie->NowImage.Pitch        = ( int )Movie->PF.pMovieImage->Pitch ;
	Movie->NowImage.GraphData    = Movie->PF.pMovieImage->ImageBuffer ;
	Movie->NowImageGraphOutAlloc = TRUE ;
	if( Movie->PF.pMovieImage->ImageType == 0 )
	{
		NS_CreateFullColorData( &Movie->NowImage.ColorData ) ;
	}
	else
	if( Movie->PF.pMovieImage->ImageType == 1 && Movie->A8R8G8B8Flag )
	{
		NS_CreateARGB8ColorData( &Movie->NowImage.ColorData ) ;
	}
	else
	{
		NS_CreateXRGB8ColorData( &Movie->NowImage.ColorData ) ;
	}
	Movie->UseNowImage = &Movie->NowImage ;

	if( Width  ) *Width  = ( int )Movie->PF.pMovieImage->Width ;
	if( Height ) *Height = ( int )Movie->PF.pMovieImage->Height ;

	// 終了
	return 0 ;


ERROR_R:

	// 各種ＣＯＭオブジェクトを終了する
	if( pAMStream					){ pAMStream->Release()					; pAMStream = NULL ; }

	if( Movie->PF.pGraph			){ Movie->PF.pGraph->Release()			; Movie->PF.pGraph = NULL ; }
	if( Movie->PF.pMediaControl		){ Movie->PF.pMediaControl->Release()	; Movie->PF.pMediaControl = NULL ; }
	if( Movie->PF.pMediaSeeking		){ Movie->PF.pMediaSeeking->Release()	; Movie->PF.pMediaSeeking = NULL ; }
	if( Movie->PF.pBasicAudio		){ Movie->PF.pBasicAudio->Release()		; Movie->PF.pBasicAudio = NULL ; }

	return DXST_ERRORLOG_ADDW( ErStr ) ;
}

#endif // DX_NON_DSHOW_MOVIE


// ムービーハンドルの後始末を行う
extern int TerminateMovieHandle_PF( HANDLEINFO *HandleInfo )
{
	MOVIEGRAPH *Movie = ( MOVIEGRAPH * )HandleInfo ;

#ifndef DX_NON_DSHOW_MOVIE
	// もしテンポラリファイルを使用していた場合はテンポラリファイルを削除する
	if( Movie->PF.UseTemporaryFile == TRUE )
	{
		DeleteFileW( Movie->PF.FileName ) ;
		Movie->PF.UseTemporaryFile = FALSE ;
	}

	if( Movie->PF.pBasicAudio )		{ Movie->PF.pBasicAudio->Release() ; 			Movie->PF.pBasicAudio = NULL ; }
	if( Movie->PF.pMediaSeeking )		{ Movie->PF.pMediaSeeking->Release() ; 		Movie->PF.pMediaSeeking = NULL ; }
	if( Movie->PF.pMediaControl )		{ Movie->PF.pMediaControl->Release(); 			Movie->PF.pMediaControl = NULL ; }
	if( Movie->PF.pGraph )				{ Movie->PF.pGraph->Release(); 				Movie->PF.pGraph = NULL ; }
#endif // DX_NON_DSHOW_MOVIE

	// 正常終了
	return 0 ;
}


// OpenMovie のグローバル変数にアクセスしないバージョンの環境依存処理
extern int OpenMovie_UseGParam_PF( MOVIEGRAPH * Movie, OPENMOVIE_GPARAM *GParam, const wchar_t *FileName, int *Width, int *Height, int SurfaceMode, int ASyncThread )
{
#ifndef DX_NON_DSHOW_MOVIE
	// DirectShow によるファイルのオープンを試みる
	Movie->PF.UseTemporaryFile = FALSE ;
	if( DirectShowOpenMovieFile( Movie, FileName, Width, Height,SurfaceMode, ASyncThread ) == -1 )
	{
		return -1 ;
	}
#else  // DX_NON_DSHOW_MOVIE

	// DirectShow が無効の場合は無条件で失敗
	return -1 ;

#endif // DX_NON_DSHOW_MOVIE

	// 成功
	return 0 ;
}

// ムービーの再生を開始する処理の環境依存処理
extern int PlayMovie__PF( MOVIEGRAPH * Movie, int PlayType, int SysPlay )
{
#ifndef DX_NON_DSHOW_MOVIE
	// DirectShow による再生を開始
	Movie->PF.pMediaControl->Run() ;
#endif // DX_NON_DSHOW_MOVIE

	// 正常終了
	return 0 ;
}

// ムービーの再生をストップする処理の環境依存処理
extern 	int PauseMovie_PF( MOVIEGRAPH * Movie, int SysPause )
{
#ifndef DX_NON_DSHOW_MOVIE
	if( Movie->PF.pMediaControl == NULL )
	{
		return -1 ; 
	}

	// 停止
	Movie->PF.pMediaControl->Pause() ;
#endif // DX_NON_DSHOW_MOVIE

	// 正常終了
	return 0 ;
}

// ムービーの再生位置を設定する(ミリ秒単位)処理の環境依存処理
extern int SeekMovie_PF( MOVIEGRAPH * Movie, int Time )
{
#ifndef DX_NON_DSHOW_MOVIE
	LONGLONG Now, Stop ;

	if( Movie->PF.pMediaSeeking == NULL )
	{
		return -1 ;
	}

	Now = ( LONGLONG )Time * 10000 ;
	Stop = 0 ;
	Movie->PF.pMediaSeeking->SetPositions( &Now, D_AM_SEEKING_AbsolutePositioning, &Stop, D_AM_SEEKING_NoPositioning ) ;
#endif // DX_NON_DSHOW_MOVIE

	// 正常終了
	return 0 ;
}

// ムービーの再生速度を設定する( 1.0 = 等倍速  2.0 = ２倍速 )処理の環境依存処理
extern int SetPlaySpeedRateMovie_PF( MOVIEGRAPH * Movie, double SpeedRate )
{
#ifndef DX_NON_DSHOW_MOVIE
	if( Movie->PF.pMediaSeeking == NULL )
	{
		return -1 ;
	}

	Movie->PF.pMediaSeeking->SetRate( SpeedRate ) ;
#endif // DX_NON_DSHOW_MOVIE

	// 正常終了
	return 0 ;
}

// ムービーの再生状態を得る処理の環境依存処理
extern int GetMovieState_PF( MOVIEGRAPH * Movie )
{
#ifndef DX_NON_DSHOW_MOVIE
	D_OAFilterState state ;
	LONGLONG Current ;

	if( Movie->PF.pMediaSeeking == NULL )
	{
		return Movie->PlayFlag ; 
	}

	if( Movie->PF.pMediaControl->GetState( 1000, &state ) != S_OK )
	{
		return Movie->PlayFlag ;
	}

	Movie->PF.pMediaSeeking->GetCurrentPosition( &Current ) ;
	if( ( Movie->PlayType & DX_PLAYTYPE_LOOPBIT ) == 0 && Current >= Movie->StopTime && state == D_State_Stopped )
	{
		Movie->PlayFlag = FALSE ;

		// 内部一時停止フラグを立てる
		Movie->SysPauseFlag = 1 ;
	}
#endif // DX_NON_DSHOW_MOVIE

	return Movie->PlayFlag ;
}

// ムービーのボリュームをセットする(0～10000)処理の環境依存処理
extern int SetMovieVolume_PF( MOVIEGRAPH * Movie, int Volume )
{
#ifndef DX_NON_DSHOW_MOVIE
	if( Movie->PF.pBasicAudio == NULL )
	{
		return -1 ;
	}

	// 音量セット
	if( Volume > 10000 )
	{
		Volume = 10000 ;
	}

	if( Volume < 0     )
	{
		Volume = 0 ;
	}

	Movie->PF.pBasicAudio->put_Volume( -10000 + Volume ) ;
#endif // DX_NON_DSHOW_MOVIE

	// 正常終了
	return 0 ;
}

// ムービーの基本イメージデータを取得する処理の環境依存処理
extern	BASEIMAGE *GetMovieBaseImage_PF( MOVIEGRAPH * Movie, int *ImageUpdateFlag, int ImageUpdateFlagSetOnly )
{
#ifndef DX_NON_DSHOW_MOVIE
	// ムービーのフレームを更新
	if( GetMovieState( Movie->HandleInfo.Handle ) == FALSE )
	{
		int Time ;

		Time = TellMovie( Movie->HandleInfo.Handle ) ;
		PlayMovie_(  Movie->HandleInfo.Handle ) ;
		UpdateMovie( Movie->HandleInfo.Handle, TRUE ) ;
		PauseMovie(  Movie->HandleInfo.Handle ) ;
		SeekMovie(   Movie->HandleInfo.Handle, Time ) ;
	}
	else
	{
#ifndef DX_NON_DSHOW_MOVIE
		if( ImageUpdateFlagSetOnly == FALSE && Movie->PF.pMovieImage->AlwaysBaseImage == 0 )
		{
			Movie->PF.pMovieImage->AlwaysBaseImage = 1 ;
		}
#endif // DX_NON_DSHOW_MOVIE
		UpdateMovie( Movie->HandleInfo.Handle ) ;
	}

	// NowImage の内容が更新されたかどうかのフラグを代入する
	if( ImageUpdateFlag != NULL )
	{
		*ImageUpdateFlag = Movie->NowImageUpdateFlag ;
	}
	Movie->NowImageUpdateFlag = FALSE ;

	return &Movie->NowImage ;

#else  // DX_NON_DSHOW_MOVIE

	return NULL ;

#endif // DX_NON_DSHOW_MOVIE
}

// ムービーの再生位置を取得する(ミリ秒単位)処理の環境依存処理
extern int TellMovie_PF( MOVIEGRAPH * Movie )
{
#ifndef DX_NON_DSHOW_MOVIE
	D_STREAM_TIME NowTime ;

	if( Movie->PF.pMediaSeeking == NULL )
	{
		return 0 ;
	}

	// 時間取得
	if( Movie->PF.pMediaSeeking->GetCurrentPosition( &NowTime ) != S_OK )
	{
		return -1 ;
	}

	// 時間を返す
	return _DTOL( (double)NowTime / 10000 ) ;
#else // DX_NON_DSHOW_MOVIE

	return -1 ;

#endif // DX_NON_DSHOW_MOVIE
}

// ムービーの再生位置を取得する(フレーム単位)処理の環境依存処理
extern int TellMovieToFrame_PF( MOVIEGRAPH * Movie )
{
#ifndef DX_NON_DSHOW_MOVIE
	D_STREAM_TIME NowTime ;

	if( Movie->PF.pMediaSeeking == NULL )
	{
		return 0 ;
	}

	// 時間取得
	if( Movie->PF.pMediaSeeking->GetCurrentPosition( &NowTime ) != S_OK )
	{
		return -1 ;
	}

	// 時間を返す
	return _DTOL( (double)NowTime / Movie->PF.FrameTime ) ;
#else // DX_NON_DSHOW_MOVIE

	return -1 ;

#endif // DX_NON_DSHOW_MOVIE
}

// ムービーの再生位置を設定する(フレーム単位)処理の環境依存処理
extern int SeekMovieToFrame_PF( MOVIEGRAPH * Movie, int Frame )
{
#ifndef DX_NON_DSHOW_MOVIE
	LONGLONG Now, Stop ;

	if( Movie->PF.pMediaSeeking == NULL ) return 0 ;

	Now = ( D_STREAM_TIME )_DTOL64( (double)Frame * Movie->PF.FrameTime ) ;
	Stop = 0 ;
	Movie->PF.pMediaSeeking->SetPositions( &Now, D_AM_SEEKING_AbsolutePositioning, &Stop, D_AM_SEEKING_NoPositioning ) ;
#endif // DX_NON_DSHOW_MOVIE

	// 正常終了
	return 0 ;
}

// ムービーの１フレームあたりの時間を得る処理の環境依存処理
extern LONGLONG GetOneFrameTimeMovie_PF( MOVIEGRAPH * Movie )
{
#ifndef DX_NON_DSHOW_MOVIE

	return Movie->PF.FrameTime ;

#else // DX_NON_DSHOW_MOVIE

	return -1 ;

#endif // DX_NON_DSHOW_MOVIE
}





// ムービーの更新を行う処理の環境依存処理
extern int UpdateMovie_PF( MOVIEGRAPH * Movie, int AlwaysFlag )
{
#ifndef DX_NON_DSHOW_MOVIE
	LONGLONG Now, Stop ;

	if( Movie->PF.pMovieImage->NewImageSet )
	{
		Movie->NowImageUpdateFlag = TRUE ;

		Movie->PF.pMovieImage->NewImageSet = 0 ;
		if( Movie->UpdateFunction )
		{
			if( Movie->PF.pMovieImage->ImageBuffer &&
				( Movie->PF.pMovieImage->YImageBuffer == NULL || Movie->PF.pMovieImage->AlwaysBaseImage ) )
			{
				Movie->NowImage.Width        = ( int )Movie->PF.pMovieImage->Width ;
				Movie->NowImage.Height       = ( int )Movie->PF.pMovieImage->Height ;
				Movie->NowImage.Pitch        = ( int )Movie->PF.pMovieImage->Pitch ;
				Movie->NowImage.GraphData    = Movie->PF.pMovieImage->ImageBuffer ;
				Movie->NowImageGraphOutAlloc = TRUE ;
				if( Movie->PF.pMovieImage->ImageType == 0 )
				{
					NS_CreateFullColorData( &Movie->NowImage.ColorData ) ;
				}
				else
				if( Movie->PF.pMovieImage->ImageType == 1 && Movie->A8R8G8B8Flag )
				{
					NS_CreateARGB8ColorData( &Movie->NowImage.ColorData ) ;
				}
				else
				{
					NS_CreateXRGB8ColorData( &Movie->NowImage.ColorData ) ;
				}
				Movie->UseNowImage = &Movie->NowImage ;
				Movie->UpdateFunction( Movie, Movie->UpdateFunctionData ) ;
			}

			if( Movie->PF.pMovieImage->YImageBuffer )
			{
				SETUP_GRAPHHANDLE_GPARAM GParam ;
				BASEIMAGE BaseImage ;
				RECT SrcRect ;

				// グラフィックハンドルがまだ作成されていなかったら作成する
				if( Movie->YGrHandle == -1 )
				{
					Graphics_Image_InitSetupGraphHandleGParam( &GParam ) ;

					Graphics_Image_InitSetupGraphHandleGParam_Normal_NonDrawValid( &GParam, 32, FALSE ) ;
					GParam.CreateImageChannelNum      = 1 ;
					GParam.CreateImageChannelBitDepth = 8 ;
					Movie->YGrHandle = Graphics_Image_MakeGraph_UseGParam( &GParam, Movie->PF.pMovieImage->YWidth, Movie->PF.pMovieImage->YHeight, FALSE, FALSE, 0, FALSE, FALSE ) ;
					NS_SetDeleteHandleFlag( Movie->YGrHandle, ( int * )&Movie->YGrHandle ) ;
				}
				if( Movie->UVGrHandle == -1 )
				{
					Graphics_Image_InitSetupGraphHandleGParam( &GParam ) ;

					Graphics_Image_InitSetupGraphHandleGParam_Normal_NonDrawValid( &GParam, 32, FALSE ) ;
					GParam.CreateImageChannelNum      = 2 ;
					GParam.CreateImageChannelBitDepth = 8 ;
					Movie->UVGrHandle     = Graphics_Image_MakeGraph_UseGParam( &GParam, Movie->PF.pMovieImage->UVWidth, Movie->PF.pMovieImage->UVHeight, FALSE, FALSE, 0, FALSE, FALSE ) ;
					NS_SetDeleteHandleFlag( Movie->UVGrHandle, ( int * )&Movie->UVGrHandle ) ;
				}

				_MEMSET( &BaseImage, 0, sizeof( BaseImage ) ) ;
				BaseImage.Width                     = Movie->PF.pMovieImage->YWidth ;
				BaseImage.Height                    = Movie->PF.pMovieImage->YHeight ;
				BaseImage.Pitch                     = Movie->PF.pMovieImage->YPitch ;
				BaseImage.GraphData                 = Movie->PF.pMovieImage->YImageBuffer ;
				BaseImage.ColorData.Format          = DX_BASEIMAGE_FORMAT_NORMAL ;
				BaseImage.ColorData.PixelByte       = 1 ;
				BaseImage.ColorData.ChannelNum      = 1 ;
				BaseImage.ColorData.ChannelBitDepth = 8 ;
				SETRECT( SrcRect, 0, 0, BaseImage.Width, BaseImage.Height ) ;
				Graphics_Image_BltBmpOrBaseImageToGraph3(
					&SrcRect,
					0,
					0,
					Movie->YGrHandle,
					&BaseImage,
					NULL,
					FALSE,
					FALSE,
					FALSE,
					FALSE
				) ;

				BaseImage.Width                     = Movie->PF.pMovieImage->UVWidth ;
				BaseImage.Height                    = Movie->PF.pMovieImage->UVHeight ;
				BaseImage.Pitch                     = Movie->PF.pMovieImage->UVPitch ;
				BaseImage.GraphData                 = Movie->PF.pMovieImage->UVImageBuffer ;
				BaseImage.ColorData.Format          = DX_BASEIMAGE_FORMAT_NORMAL ;
				BaseImage.ColorData.PixelByte       = 2 ;
				BaseImage.ColorData.ChannelNum      = 2 ;
				BaseImage.ColorData.ChannelBitDepth = 8 ;
				SETRECT( SrcRect, 0, 0, BaseImage.Width, BaseImage.Height ) ;
				Graphics_Image_BltBmpOrBaseImageToGraph3(
					&SrcRect,
					0,
					0,
					Movie->UVGrHandle,
					&BaseImage,
					NULL,
					FALSE,
					FALSE,
					FALSE,
					FALSE
				) ;

				Movie->UpdateFunction( Movie, Movie->UpdateFunctionData ) ;
			}
		}
	}

	if( Movie->PF.pMediaSeeking && Movie->PF.pMediaControl )
	{
		Movie->PF.pMediaSeeking->GetCurrentPosition( &Now ) ;
		if( Now >= Movie->StopTime )
		{
			if( Movie->PlayType & DX_PLAYTYPE_LOOPBIT )
			{
				Now = 0 ;
				Stop = 0 ;
				Movie->PF.pMediaSeeking->SetPositions( &Now, D_AM_SEEKING_AbsolutePositioning, &Stop, D_AM_SEEKING_NoPositioning ) ;
				Movie->PF.pMediaControl->Run() ;
			}
			else
			{
				// 停止
				Movie->PF.pMediaControl->Pause() ;

				Movie->PlayFlag = FALSE ;

				// 内部一時停止フラグを立てる
				Movie->SysPauseFlag = 1 ;
			}
		}
	}
#endif // DX_NON_DSHOW_MOVIE

	// 正常終了
	return 0 ;
}





#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

#endif // DX_NON_MOVIE

