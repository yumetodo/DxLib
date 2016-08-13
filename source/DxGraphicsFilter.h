// -------------------------------------------------------------------------------
// 
// 		ＤＸライブラリ		GraphFilter系プログラムヘッダファイル
// 
// 				Ver 3.16d
// 
// -------------------------------------------------------------------------------

#ifndef __DXGRAPHICSFILTER_H__
#define __DXGRAPHICSFILTER_H__

#include "DxCompileConfig.h"

#ifndef DX_NON_FILTER

// インクルード ------------------------------------------------------------------
#include "DxLib.h"
#include "DxGraphics.h"

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// マクロ定義 --------------------------------------------------------------------


// 構造体定義 --------------------------------------------------------------------

// フィルター情報構造体
struct GRAPHFILTER_INFO
{
	int						IsBlend ;
	int						FilterOrBlendType ;
	int						SrcGrHandle ;
	int						BlendGrHandle ;
	int						DestGrHandle ;
	int						SrcEqualDestClearFlag ;
	float					BlendRatio ;
	int						Pass ;
	int						SrcX1 ;
	int						SrcY1 ;
	int						SrcX2 ;
	int						SrcY2 ;
	int						SrcDivNum ;
	int						BlendX ;
	int						BlendY ;
	int						BlendPosEnable ;
	int						DestX ;
	int						DestY ;

	int						PassNum ;
	int						UseWorkScreen ;

	int						DestWidth ;
	int						DestHeight ;
	BASEIMAGE				SrcBaseImage ;
	BASEIMAGE				BlendBaseImage ;
	BASEIMAGE				DestBaseImage ;

	IMAGEDATA				*SrcImage ;
	IMAGEDATA				*DestImage ;
	IMAGEDATA				*BlendImage ;
	IMAGEDATA				*TargetScreenImage ;
} ;

// フィルター毎情報
struct GRAPHFILTER_PARAM
{
	int						Gauss_PixelWidth ;
	float					Gauss_Param ;
	int						Bright_Clip_CmpType ;
	float					Bright_Clip_CmpParam ;
	int						Bright_Clip_ClipFillFlag ;
	COLOR_F					Bright_Clip_ClipFillColor ;
	int						Bright_Scale_Min ;
	int						Bright_Scale_Max ;
	int						Hsb_HueType ;
	float					Hsb_Hue ;
	float					Hsb_Saturation ;
	float					Hsb_Bright ;
	float					Mono_Cb ;
	float					Mono_Cr ;
	float					Lv_Min ;
	float					Lv_Max ;
	float					Lv_Gamma ;
	float					Lv_AfterMin ;
	float					Lv_AfterMax ;
	float					TC_Threshold ;
	COLOR_F					TC_LowColor ;
	COLOR_F					TC_HighColor ;
	int						GM_MapGrHandle ;
	int						GM_Reverse ;
	int						UVGrHandle ;
	int						DestSizeX ;
	int						DestSizeY ;

	int						RGBA_R ;
	int						RGBA_G ;
	int						RGBA_B ;
	int						RGBA_A ;
} ;

// フィルターのシェーダーハンドル
struct GRAPHFILTER_SHADER_HANDLE
{
	int						Gauss_PS[ 3 ] ;							// ガウスフィルタのピクセルシェーダー
	int						BrightClipPS[ 2 ][ 2 ] ;				// 明るさでクリップするフィルタのピクセルシェーダー( [ 1:一定以上クリップ  0:一定以下クリップ ][ 1:クリップ塗りつぶしあり 0:塗りつぶしなし ] )
	int						BrightScalePS ;							// 指定の明るさの領域を拡大するフィルタのピクセルシェーダー
	int						DownScalePS[ 3 ] ;						// 縮小フィルター( 0:X2 1:X4 2:X8 )
	int						HsbPS[ 3 ] ;							// ＨＳＢフィルター( 0:RGB to HSI  1:HSI to RGB  2:HSB の HLock )
	int						MonoPS ;								// モノトーンフィルター
	int						InvertPS ;								// 階調反転フィルター
	int						LevelPS ;								// レベル補正フィルター
	int						GammaTex ;								// ガンマ補正に使用する画像
	float					PrevGamma ;								// 前回のガンマフィルタの際のガンマ値
	int						TwoColorPS ;							// ２値化フィルター
	int						GradientMapPS[ 2 ] ;					// グラデーションマップフィルター
	int						PreMulAlphaPS ;							// 通常画像から乗算済みアルファ画像を作成する為のフィルター
	int						InterpAlphaPS ;							// 乗算済みアルファ画像から通常画像を作成する為のフィルター
	int						YUVtoRGBPS[ 4 ] ;						// YUVカラーをRGBカラーに変換するフィルター
	int						BicubicPS ;								// バイキュービック補間フィルター
	int						Lanczos3PS ;							// Lanczos-3補間フィルター

	int						BasicBlendPS[ 14 ] ;					// 基本的なブレンドフィルター
	int						RgbaMixBasePS ;							// RGBAMixブレンド基本
	int						RgbaMixSRGBB[ 4 ] ;						// RGBAMixブレンドの A だけ BRGBA の４つ
	int						RgbaMixSRRRB[ 4 ][ 4 ] ;				// RGBAMixブレンドの SYYY BX の１６こ [ Y ][ X ]
	int						RgbaMixS[ 4 ][ 4 ][ 4 ][ 4 ] ;			// RGBAMixブレンドの S だけの組み合わせ256個[ R ][ G ][ B ][ A ]
} ;

// 内部大域変数宣言 --------------------------------------------------------------

extern GRAPHFILTER_SHADER_HANDLE GraphFilterShaderHandle ;

// 関数プロトタイプ宣言-----------------------------------------------------------

// フィルターのシェーダーハンドルを初期化する
extern int GraphFilter_Initialize( void ) ;

extern int	GraphFilter_Mono(          GRAPHFILTER_INFO *Info, float Cb, float Cr ) ;
extern int	GraphFilter_Gauss(         GRAPHFILTER_INFO *Info, int PixelWidth, float Param ) ;
extern int	GraphFilter_Down_Scale(    GRAPHFILTER_INFO *Info, int DivNum ) ;
extern int	GraphFilter_Bright_Clip(   GRAPHFILTER_INFO *Info, int CmpType, float CmpParam, int ClipFillFlag, COLOR_F *ClipFillColor ) ;
extern int	GraphFilter_Bright_Scale(  GRAPHFILTER_INFO *Info, int BrightMin, int BrightMax ) ;
extern int	GraphFilter_HSB(           GRAPHFILTER_INFO *Info, int HueType, float Hue, float Saturation, float Bright ) ;
extern int	GraphFilter_Invert(        GRAPHFILTER_INFO *Info ) ;
extern int	GraphFilter_Level(         GRAPHFILTER_INFO *Info, float Min, float Max, float Gamma, float AfterMin, float AfterMax ) ;
extern int	GraphFilter_TwoColor(      GRAPHFILTER_INFO *Info, float Threshold, COLOR_F *LowColor, COLOR_F *HighColor ) ;
extern int	GraphFilter_GradientMap(   GRAPHFILTER_INFO *Info, int MapGrHandle, int Reverse ) ;
extern int	GraphFilter_PremulAlpha(   GRAPHFILTER_INFO *Info ) ;
extern int	GraphFilter_InterpAlpha(   GRAPHFILTER_INFO *Info ) ;
extern int	GraphFilter_YUVtoRGB(      GRAPHFILTER_INFO *Info, int UVGrHandle ) ;
extern int	GraphFilter_BicubicScale(  GRAPHFILTER_INFO *Info, int DestSizeX, int DestSizeY ) ;
extern int	GraphFilter_Lanczos3Scale( GRAPHFILTER_INFO *Info, int DestSizeX, int DestSizeY ) ;

extern int	GraphFilter_RectBltBase( int IsBlend, int SrcGrHandle, int BlendGrHandle, int DestGrHandle, int BlendRatio, int FilterOrBlendType, int SrcX1, int SrcY1, int SrcX2, int SrcY2, int BlendX, int BlendY, int BlendPosEnable, int DestX, int DestY, va_list ParamList ) ;

extern int	GraphBlend_Basic(           GRAPHFILTER_INFO *Info ) ;
extern int	GraphBlend_RGBA_Select_Mix( GRAPHFILTER_INFO *Info, int SelectR, int SelectG, int SelectB, int SelectA ) ;




// 環境依存関係
extern int	GraphFilter_Mono_PF(          GRAPHFILTER_INFO *Info, float Cb, float Cr ) ;
extern int	GraphFilter_Gauss_PF(         GRAPHFILTER_INFO *Info, int PixelWidth, float Param, float *Table ) ;
extern int	GraphFilter_Down_Scale_PF(    GRAPHFILTER_INFO *Info, int DivNum ) ;
extern int	GraphFilter_Bright_Clip_PF(   GRAPHFILTER_INFO *Info, int CmpType, float CmpParam, int ClipFillFlag, COLOR_F *ClipFillColor ) ;
extern int	GraphFilter_Bright_Scale_PF(  GRAPHFILTER_INFO *Info, int BrightMin, int BrightMax ) ;
extern int	GraphFilter_HSB_PF(           GRAPHFILTER_INFO *Info, int HueType, float Hue, float Saturation, float Bright ) ;
extern int	GraphFilter_Invert_PF(        GRAPHFILTER_INFO *Info ) ;
extern int	GraphFilter_Level_PF(         GRAPHFILTER_INFO *Info, float Min, float Max, float Gamma, float AfterMin, float AfterMax ) ;
extern int	GraphFilter_TwoColor_PF(      GRAPHFILTER_INFO *Info, float Threshold, COLOR_F *LowColor, COLOR_F *HighColor ) ;
extern int	GraphFilter_GradientMap_PF(   GRAPHFILTER_INFO *Info, int MapGrHandle, int Reverse ) ;
extern int	GraphFilter_PremulAlpha_PF(   GRAPHFILTER_INFO *Info ) ;
extern int	GraphFilter_InterpAlpha_PF(   GRAPHFILTER_INFO *Info ) ;
extern int	GraphFilter_YUVtoRGB_PF(      GRAPHFILTER_INFO *Info, int UVGrHandle ) ;
extern int	GraphFilter_BicubicScale_PF(  GRAPHFILTER_INFO *Info, int DestSizeX, int DestSizeY ) ;
extern int	GraphFilter_Lanczos3Scale_PF( GRAPHFILTER_INFO *Info, int DestSizeX, int DestSizeY ) ;
extern int	GraphFilter_RectBltBase_Timing0_PF( GRAPHFILTER_INFO *Info, GRAPHFILTER_PARAM *Param ) ;
extern int	GraphFilter_RectBltBase_Timing1_PF( void ) ;
extern int	GraphFilter_DestGraphSetup_PF(      GRAPHFILTER_INFO *Info, int *UseSrcGrHandle, int *UseDestGrHandle ) ;
extern int	GraphFilter_DestGraphUpdate_PF(     GRAPHFILTER_INFO *Info, int UseDestGrHandle, int FilterResult ) ;

extern int	GraphBlend_Basic_PF(           GRAPHFILTER_INFO *Info ) ;
extern int	GraphBlend_RGBA_Select_Mix_PF( GRAPHFILTER_INFO *Info, int SelectR, int SelectG, int SelectB, int SelectA ) ;

#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

#endif // DX_NON_FILTER

#endif // __DXGRAPHICSFILTER_H__

