//-----------------------------------------------------------------------------
// 
// 		ＤＸライブラリ		Windows用GraphFilter系プログラム
// 
//  	Ver 3.16d
// 
//-----------------------------------------------------------------------------

// ＤＸライブラリ作成時用定義
#define __DX_MAKE

#include "DxGraphicsFilterWin.h"

#ifndef DX_NON_GRAPHICS

// インクルード ---------------------------------------------------------------
#include "DxGraphicsWin.h"
#include "DxGraphicsFilterD3D9.h"
#include "DxGraphicsFilterD3D11.h"

#ifndef DX_NON_FILTER

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// マクロ定義 -----------------------------------------------------------------

// 構造体宣言 -----------------------------------------------------------------

// データ定義 -----------------------------------------------------------------

// 関数宣言 -------------------------------------------------------------------

// プログラム -----------------------------------------------------------------

// 環境依存関係

extern int	GraphFilter_Mono_PF(       GRAPHFILTER_INFO *Info, float Cb, float Cr )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return GraphFilter_D3D11_Mono_PF(       Info,  Cb,  Cr ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return GraphFilter_D3D9_Mono_PF(       Info,  Cb,  Cr ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

extern int	GraphFilter_Gauss_PF(      GRAPHFILTER_INFO *Info, int PixelWidth, float Param, float *Table )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return GraphFilter_D3D11_Gauss_PF(      Info,  PixelWidth,  Param, Table ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return GraphFilter_D3D9_Gauss_PF(      Info,  PixelWidth,  Param, Table ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

extern int	GraphFilter_Down_Scale_PF(  GRAPHFILTER_INFO *Info, int DivNum )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return GraphFilter_D3D11_Down_Scale_PF(  Info,  DivNum ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return GraphFilter_D3D9_Down_Scale_PF(  Info,  DivNum ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

extern int	GraphFilter_Bright_Clip_PF( GRAPHFILTER_INFO *Info, int CmpType, float CmpParam, int ClipFillFlag, COLOR_F *ClipFillColor )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return GraphFilter_D3D11_Bright_Clip_PF( Info,  CmpType,  CmpParam,  ClipFillFlag, ClipFillColor ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return GraphFilter_D3D9_Bright_Clip_PF( Info,  CmpType,  CmpParam,  ClipFillFlag, ClipFillColor ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

extern int	GraphFilter_Bright_Scale_PF( GRAPHFILTER_INFO *Info, int BrightMin, int BrightMax )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return GraphFilter_D3D11_Bright_Scale_PF( Info, BrightMin, BrightMax ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return GraphFilter_D3D9_Bright_Scale_PF( Info, BrightMin, BrightMax ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

extern int	GraphFilter_HSB_PF(         GRAPHFILTER_INFO *Info, int HueType, float Hue, float Saturation, float Bright )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return GraphFilter_D3D11_HSB_PF(         Info,  HueType,  Hue,  Saturation,  Bright ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return GraphFilter_D3D9_HSB_PF(         Info,  HueType,  Hue,  Saturation,  Bright ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

extern int	GraphFilter_Invert_PF(      GRAPHFILTER_INFO *Info )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return GraphFilter_D3D11_Invert_PF(      Info ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return GraphFilter_D3D9_Invert_PF(      Info ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

extern int	GraphFilter_Level_PF(       GRAPHFILTER_INFO *Info, float Min, float Max, float Gamma, float AfterMin, float AfterMax )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return GraphFilter_D3D11_Level_PF(       Info,  Min,  Max,  Gamma,  AfterMin,  AfterMax ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return GraphFilter_D3D9_Level_PF(       Info,  Min,  Max,  Gamma,  AfterMin,  AfterMax ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

extern int	GraphFilter_TwoColor_PF(    GRAPHFILTER_INFO *Info, float Threshold, COLOR_F *LowColor, COLOR_F *HighColor )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return GraphFilter_D3D11_TwoColor_PF(    Info,  Threshold, LowColor, HighColor ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return GraphFilter_D3D9_TwoColor_PF(    Info,  Threshold, LowColor, HighColor ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

extern int	GraphFilter_GradientMap_PF( GRAPHFILTER_INFO *Info, int MapGrHandle, int Reverse )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return GraphFilter_D3D11_GradientMap_PF( Info,  MapGrHandle,  Reverse ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return GraphFilter_D3D9_GradientMap_PF( Info,  MapGrHandle,  Reverse ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

extern int	GraphFilter_PremulAlpha_PF( GRAPHFILTER_INFO *Info )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return GraphFilter_D3D11_PremulAlpha_PF( Info ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return GraphFilter_D3D9_PremulAlpha_PF( Info ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

extern int	GraphFilter_InterpAlpha_PF( GRAPHFILTER_INFO *Info )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return GraphFilter_D3D11_InterpAlpha_PF( Info ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return GraphFilter_D3D9_InterpAlpha_PF( Info ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

extern int	GraphFilter_YUVtoRGB_PF(    GRAPHFILTER_INFO *Info, int UVGrHandle )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return GraphFilter_D3D11_YUVtoRGB_PF( Info, UVGrHandle ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return GraphFilter_D3D9_YUVtoRGB_PF( Info, UVGrHandle ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

extern int	GraphFilter_BicubicScale_PF(  GRAPHFILTER_INFO *Info, int DestSizeX, int DestSizeY )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return GraphFilter_D3D11_BicubicScale_PF( Info, DestSizeX, DestSizeY ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return GraphFilter_D3D9_BicubicScale_PF( Info, DestSizeX, DestSizeY ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

extern int	GraphFilter_Lanczos3Scale_PF(  GRAPHFILTER_INFO *Info, int DestSizeX, int DestSizeY )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return GraphFilter_D3D11_Lanczos3Scale_PF( Info, DestSizeX, DestSizeY ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return GraphFilter_D3D9_Lanczos3Scale_PF( Info, DestSizeX, DestSizeY ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

extern int GraphFilter_RectBltBase_Timing0_PF( GRAPHFILTER_INFO *Info, GRAPHFILTER_PARAM *Param )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return GraphFilter_D3D11_RectBltBase_Timing0_PF( Info, Param ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return GraphFilter_D3D9_RectBltBase_Timing0_PF( Info, Param ) ;
#endif // DX_NON_DIRECT3D9

	default :
		Info->PassNum = -1 ;
		if( Info->IsBlend )
		{
			// パスの数を取得
			Info->PassNum = 1 ;

			// 作業用テクスチャを使用するかどうかを取得
			Info->UseWorkScreen = FALSE ;
		}
		else
		{
			// ガウスフィルタのみ２パス
			if( Info->FilterOrBlendType == DX_GRAPH_FILTER_GAUSS )
			{
				Info->UseWorkScreen = TRUE ;
				Info->PassNum = 2 ;
			}
			else
			{
				Info->UseWorkScreen = FALSE ;
				Info->PassNum = 1 ;
			}
		}

		if( Info->SrcX1 != Info->DestX ||
			Info->SrcY1 != Info->DestY  )
		{
			Info->UseWorkScreen = 1 ;
		}
		return 0 ;
	}
}

extern int GraphFilter_RectBltBase_Timing1_PF( void )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return GraphFilter_D3D11_RectBltBase_Timing1_PF() ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return GraphFilter_D3D9_RectBltBase_Timing1_PF() ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

extern int GraphFilter_DestGraphSetup_PF( GRAPHFILTER_INFO *Info, int *UseSrcGrHandle, int *UseDestGrHandle )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return GraphFilter_D3D11_DestGraphSetup_PF( Info, UseSrcGrHandle, UseDestGrHandle ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return GraphFilter_D3D9_DestGraphSetup_PF( Info, UseSrcGrHandle, UseDestGrHandle ) ;
#endif // DX_NON_DIRECT3D9

	default :
		*UseDestGrHandle = Info->DestGrHandle ;
		return 0 ;
	}
}

extern int GraphFilter_DestGraphUpdate_PF( GRAPHFILTER_INFO *Info, int UseDestGrHandle, int FilterResult )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return GraphFilter_D3D11_DestGraphUpdate_PF( Info, UseDestGrHandle, FilterResult ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return GraphFilter_D3D9_DestGraphUpdate_PF( Info, UseDestGrHandle, FilterResult ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}



extern int	GraphBlend_Basic_PF(           GRAPHFILTER_INFO *Info )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return GraphBlend_D3D11_Basic_PF(           Info ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return GraphBlend_D3D9_Basic_PF(           Info ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

extern int	GraphBlend_RGBA_Select_Mix_PF( GRAPHFILTER_INFO *Info, int SelectR, int SelectG, int SelectB, int SelectA )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return GraphBlend_D3D11_RGBA_Select_Mix_PF( Info,  SelectR,  SelectG,  SelectB,  SelectA ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return GraphBlend_D3D9_RGBA_Select_Mix_PF( Info,  SelectR,  SelectG,  SelectB,  SelectA ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}



#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

#endif // DX_NON_FILTER


#endif // DX_NON_GRAPHICS
