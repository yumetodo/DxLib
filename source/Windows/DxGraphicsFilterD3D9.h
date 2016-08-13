// -------------------------------------------------------------------------------
// 
// 		�c�w���C�u����		GraphFilter�n�v���O����( Direct3D9 )�w�b�_�t�@�C��
// 
// 				Ver 3.16d
// 
// -------------------------------------------------------------------------------

#ifndef __DXGRAPHICSFILTERD3D9_H__
#define __DXGRAPHICSFILTERD3D9_H__

#include "../DxCompileConfig.h"

#ifndef DX_NON_GRAPHICS

#ifndef DX_NON_DIRECT3D9

// �C���N���[�h ------------------------------------------------------------------
#include "../DxLib.h"
#include "../DxGraphicsFilter.h"
#include "../DxArchive_.h"
#include "DxGraphicsD3D9.h"

#ifndef DX_NON_FILTER

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// �}�N����` --------------------------------------------------------------------


// �\���̒�` --------------------------------------------------------------------

// �t�B���^�[�֐��p�̃��C�u�����̐ݒ�����ꎞ�I�ɑޔ����邽�߂Ɏg���\����
struct GRAPHFILTER_DRAWPARAMTEMP_DIRECT3D9
{
	D_IDirect3DSurface9 *	TargetSurface[ DX_RENDERTARGET_COUNT ] ;
	D_IDirect3DSurface9 *	DepthStencilSurfaece ;
	int						BlendMode ;
	int						BlendParam ;
	int						AlphaTestMode ;
	int						AlphaTestParam ;
	int						UseZBufferFlag3D ;
	int						DrawMode ;
	int						FogEnable ;
	int						LightEnable ;
} ;

// �t�B���^�[�����p�̏��\����
struct GRAPHFILTER_SYSTEMIFNO_DIRET3D9
{
	int									WorkDrawValidGrHandle[ 2 ][ 16 ][ 2 ] ;	// �t�B���^�[������Ɨp�`��\�e�N�X�`��[ 0:�����e�N�X�`�� 1:���������_�e�N�X�`�� ][ �Q�̂��� ][ ��Ɨp�Q�� ]
	GRAPHFILTER_DRAWPARAMTEMP_DIRECT3D9	DrawParamTemp ;
} ;


// �������ϐ��錾 --------------------------------------------------------------

extern GRAPHFILTER_SYSTEMIFNO_DIRET3D9 GraphFilterSystemInfoD3D9 ;

// �֐��v���g�^�C�v�錾-----------------------------------------------------------

// ���ˑ��֌W
extern int	GraphFilter_D3D9_Mono_PF(          GRAPHFILTER_INFO *Info, float Cb, float Cr ) ;
extern int	GraphFilter_D3D9_Gauss_PF(         GRAPHFILTER_INFO *Info, int PixelWidth, float Param, float *Table ) ;
extern int	GraphFilter_D3D9_Down_Scale_PF(    GRAPHFILTER_INFO *Info, int DivNum ) ;
extern int	GraphFilter_D3D9_Bright_Clip_PF(   GRAPHFILTER_INFO *Info, int CmpType, float CmpParam, int ClipFillFlag, COLOR_F *ClipFillColor ) ;
extern int	GraphFilter_D3D9_Bright_Scale_PF(  GRAPHFILTER_INFO *Info, int BrightMin, int BrightMax ) ;
extern int	GraphFilter_D3D9_HSB_PF(           GRAPHFILTER_INFO *Info, int HueType, float Hue, float Saturation, float Bright ) ;
extern int	GraphFilter_D3D9_Invert_PF(        GRAPHFILTER_INFO *Info ) ;
extern int	GraphFilter_D3D9_Level_PF(         GRAPHFILTER_INFO *Info, float Min, float Max, float Gamma, float AfterMin, float AfterMax ) ;
extern int	GraphFilter_D3D9_TwoColor_PF(      GRAPHFILTER_INFO *Info, float Threshold, COLOR_F *LowColor, COLOR_F *HighColor ) ;
extern int	GraphFilter_D3D9_GradientMap_PF(   GRAPHFILTER_INFO *Info, int MapGrHandle, int Reverse ) ;
extern int	GraphFilter_D3D9_PremulAlpha_PF(   GRAPHFILTER_INFO *Info ) ;
extern int	GraphFilter_D3D9_InterpAlpha_PF(   GRAPHFILTER_INFO *Info ) ;
extern int	GraphFilter_D3D9_YUVtoRGB_PF(      GRAPHFILTER_INFO *Info, int UVGrHandle ) ;
extern int	GraphFilter_D3D9_BicubicScale_PF(  GRAPHFILTER_INFO *Info, int DestSizeX, int DestSizeY ) ;
extern int	GraphFilter_D3D9_Lanczos3Scale_PF( GRAPHFILTER_INFO *Info, int DestSizeX, int DestSizeY ) ;

//extern int	GraphFilter_D3D9_RectBltBase_PF( int IsBlend, int SrcGrHandle, int BlendGrHandle, int DestGrHandle, int BlendRatio, int FilterOrBlendType, int SrcX1, int SrcY1, int SrcX2, int SrcY2, int BlendX, int BlendY, int BlendPosEnable, int DestX, int DestY, va_list ParamList ) ;			// �摜�̃t�B���^�[�t���]�����s��( �ψ������t�� )
//extern int	GraphFilter_D3D9_RectBltBase_Timing0_PF( int SrcGrHandle, int DestGrHandle, int BlendGrHandle, int IsBlend, IMAGEDATA *TargetScreenImage ) ;
//extern int	GraphFilter_D3D9_RectBltBase_PF(     GRAPHFILTER_INFO *Info, GRAPHFILTER_PARAM *Param ) ;			// �摜�̃t�B���^�[�t���]�����s��( �ψ������t�� )

extern int	GraphFilter_D3D9_RectBltBase_Timing0_PF( GRAPHFILTER_INFO *Info, GRAPHFILTER_PARAM *Param ) ;
extern int	GraphFilter_D3D9_RectBltBase_Timing1_PF( void ) ;
extern int	GraphFilter_D3D9_DestGraphSetup_PF(      GRAPHFILTER_INFO *Info, int *UseSrcGrHandle, int *UseDestGrHandle ) ;
extern int	GraphFilter_D3D9_DestGraphUpdate_PF(     GRAPHFILTER_INFO *Info, int UseDestGrHandle, int FilterResult ) ;

extern int	GraphBlend_D3D9_Basic_PF(           GRAPHFILTER_INFO *Info ) ;
extern int	GraphBlend_D3D9_RGBA_Select_Mix_PF( GRAPHFILTER_INFO *Info, int SelectR, int SelectG, int SelectB, int SelectA ) ;

#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

#endif // DX_NON_FILTER

#endif // DX_NON_DIRECT3D9

#endif // DX_NON_GRAPHICS

#endif // __DXGRAPHICSFILTERD3D9_H__

