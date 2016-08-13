//-----------------------------------------------------------------------------
// 
// 		�c�w���C�u����		GraphFilter�n�v���O����( Direct3D11 )
// 
//  	Ver 3.16d
// 
//-----------------------------------------------------------------------------

// �c�w���C�u�����쐬���p��`
#define __DX_MAKE

#include "../DxCompileConfig.h"

#ifndef DX_NON_GRAPHICS

#ifndef DX_NON_DIRECT3D11

// �C���N���[�h ---------------------------------------------------------------
#include "DxGraphicsFilterD3D11.h"
#include "DxGraphicsWin.h"
#include "DxWindow.h"
#include "DxWinAPI.h"
#include "DxShader_DataType_D3D11.h"
#include "DxShader_PS_D3D11.h"
#include "../DxSystem.h"
#include "../DxLog.h"
#include "../DxModel.h"
#include "../DxBaseFunc.h"
#include "../DxGraphics.h"
#include "../DxASyncLoad.h"

#ifndef DX_NON_FILTER

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// �}�N����` -----------------------------------------------------------------

// �\���̐錾 -----------------------------------------------------------------

// �f�[�^��` -----------------------------------------------------------------

GRAPHFILTER_SYSTEMIFNO_DIRET3D11 GraphFilterSystemInfoD3D11 ;

// �֐��錾 -------------------------------------------------------------------

static int	Direct3D11_GraphFilter_GetWorkTexture( int IsFloatType, DWORD TexSizeW, DWORD TexSizeH, DWORD HandleIndex ) ;	// �t�B���^�[��Ɨp�̃e�N�X�`���n���h�����擾����
static int  Direct3D11_MemLoadShaderCode( const char *ShaderName, bool IsVertexShader ) ;									// �w�薼�̎��O�p�ӃV�F�[�_�[��ǂݍ���
static int  Direct3D11_FilterStretchBlt( int UseShaderHandle, GRAPHFILTER_INFO *Info, int IsLinearFilter = TRUE, int ScaleDivNum = 1, int SubImageHandle = -1, VERTEX_TEX8_2D *Texcoord8Vertex = NULL ) ;

// �v���O���� -----------------------------------------------------------------

// �t�B���^�[��Ɨp�̃e�N�X�`���n���h�����擾����
static int	Direct3D11_GraphFilter_GetWorkTexture( int IsFloatType, DWORD TexSizeW, DWORD TexSizeH, DWORD HandleIndex )
{
	IMAGEDATA *BaseImage ;
	DWORD       TexSizeBig ;
	DWORD       NPow ;
	int         TexSize ;

	TexSizeBig = TexSizeW > TexSizeH ? TexSizeW : TexSizeH ;
	for( NPow = 0 ; ( DWORD )( 1 << NPow ) < TexSizeBig ; NPow ++ ){}
	TexSize = ( int )( 1 << NPow ) ;

	if( GRAPHCHK( GraphFilterSystemInfoD3D11.WorkDrawValidGrHandle[ IsFloatType ][ NPow ][ HandleIndex ], BaseImage ) )
	{
		SETUP_GRAPHHANDLE_GPARAM GParam ;

		Graphics_Image_InitSetupGraphHandleGParam_Normal_DrawValid_NoneZBuffer( &GParam, 32, TRUE ) ;
		GParam.DrawValidFloatTypeGraphCreateFlag = IsFloatType ;

		GraphFilterSystemInfoD3D11.WorkDrawValidGrHandle[ IsFloatType ][ NPow ][ HandleIndex ] = Graphics_Image_MakeGraph_UseGParam( &GParam, TexSize, TexSize, FALSE, FALSE, 0, FALSE ) ;
		if( GraphFilterSystemInfoD3D11.WorkDrawValidGrHandle[ IsFloatType ][ NPow ][ HandleIndex ] < 0 )
		{
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\x5c\x4f\x6d\x69\x28\x75\xcf\x63\x3b\x75\xef\x53\xfd\x80\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x20\x00\x53\x00\x69\x00\x7a\x00\x65\x00\x3a\x00\x25\x00\x64\x00\x78\x00\x25\x00\x64\x00\x00"/*@ L"��Ɨp�`��\�e�N�X�`���̍쐬�Ɏ��s���܂��� Size:%dx%d" @*/, TexSize, TexSize )) ;
			return -1 ;
		}
		NS_SetDeleteHandleFlag( GraphFilterSystemInfoD3D11.WorkDrawValidGrHandle[ IsFloatType ][ NPow ][ HandleIndex ], &GraphFilterSystemInfoD3D11.WorkDrawValidGrHandle[ IsFloatType ][ NPow ][ HandleIndex ] ) ;
	}

	return GraphFilterSystemInfoD3D11.WorkDrawValidGrHandle[ IsFloatType ][ NPow ][ HandleIndex ] ;
}

// �w�薼�̎��O�p�ӃV�F�[�_�[��ǂݍ���
static int Direct3D11_MemLoadShaderCode( const char *ShaderName, bool IsVertexShader )
{
	int Addr, Size ;
	BYTE *FileImage ;
	int Ret ;

	if( GD3D11.ShaderCode.Base.BaseShaderInitializeFlag == FALSE )
	{
		return -1 ;
	}

	if( DXA_GetFileInfo( &GD3D11.ShaderCode.Base.FilterShaderBinDxa, DX_CHARCODEFORMAT_ASCII, ShaderName, &Addr, &Size ) < 0 )
	{
		return -1 ;
	}

	FileImage = ( BYTE * )DXA_GetFileImage( &GD3D11.ShaderCode.Base.FilterShaderBinDxa ) + Addr ;

	Ret = Graphics_Shader_CreateHandle_UseGParam( IsVertexShader, FileImage, Size, FALSE, FALSE ) ;

	return Ret ;
}

static int Direct3D11_FilterStretchBlt( int UseShaderHandle, GRAPHFILTER_INFO *Info, int IsLinearFilter, int ScaleDivNum, int SubImageHandle, VERTEX_TEX8_2D *Texcoord8Vertex )
{
	IMAGEDATA     *SrcImage      = NULL ;
	IMAGEDATA     *BlendImage    = NULL ;
	IMAGEDATA     *DestImage     = NULL ;
	SHADOWMAPDATA *SrcShadowMap  = NULL ;
	SHADOWMAPDATA *DestShadowMap = NULL ;
	D_ID3D11Texture2D          *SrcTexture      = NULL ;
	D_ID3D11ShaderResourceView *SrcTextureSRV   = NULL ;
	D_ID3D11Texture2D          *DestTexture     = NULL ;
	D_ID3D11RenderTargetView   *DestTextureRTV  = NULL ;
	D_ID3D11Texture2D          *BlendTexture    = NULL ;
	D_ID3D11ShaderResourceView *BlendTextureSRV = NULL ;
	SHADERHANDLEDATA           *ShaderData ;
	int  SrcWidth ;
	int  SrcHeight ;
	RECT SrcRect ;
	RECT DestRect ;
	RECT BlendRect ;

	if( GRAPHCHK(     Info->SrcGrHandle, SrcImage    ) &&
		SHADOWMAPCHK( Info->SrcGrHandle, SrcShadowMap ) )
	{
		return -1 ;
	}

	if( GRAPHCHK(     Info->DestGrHandle, DestImage    ) &&
		SHADOWMAPCHK( Info->DestGrHandle, DestShadowMap ) )
	{
		return -1 ;
	}

	ShaderData = Graphics_Shader_GetData( UseShaderHandle ) ;
	if( ShaderData == NULL )
	{
		return -1 ;
	}

	if( SrcShadowMap != NULL )
	{
		SrcTexture    = SrcShadowMap->PF->D3D11.DepthTexture ;
		SrcTextureSRV = SrcShadowMap->PF->D3D11.DepthTextureSRV ;
	}
	else
	{
		SrcTexture    = SrcImage->Hard.Draw[ 0 ].Tex->PF->D3D11.Texture ;
		SrcTextureSRV = SrcImage->Hard.Draw[ 0 ].Tex->PF->D3D11.TextureSRV ;
	}

	if( DestShadowMap != NULL )
	{
		DestTexture    = DestShadowMap->PF->D3D11.DepthTexture ;
		DestTextureRTV = DestShadowMap->PF->D3D11.DepthTextureRTV ;
	}
	else
	{
		DestTexture    = DestImage->Hard.Draw[ 0 ].Tex->PF->D3D11.Texture ;
		DestTextureRTV = DestImage->Hard.Draw[ 0 ].Tex->PF->D3D11.TextureRTV[ 0 ] ;
	}

	SrcRect.left   = Info->SrcX1 ;
	SrcRect.top    = Info->SrcY1 ;
	SrcRect.right  = Info->SrcX2 ;
	SrcRect.bottom = Info->SrcY2 ;

	SrcWidth  = Info->SrcX2 - Info->SrcX1 ;
	SrcHeight = Info->SrcY2 - Info->SrcY1 ;

	if( ScaleDivNum != 1 )
	{
		DestRect.left   = Info->DestX                           ;
		DestRect.right  = Info->DestX + SrcWidth  / ScaleDivNum ;
		DestRect.top    = Info->DestY                           ;
		DestRect.bottom = Info->DestY + SrcHeight / ScaleDivNum ;
	}
	else
	{
		DestRect.left   = Info->DestX             ;
		DestRect.right  = Info->DestX + SrcWidth  ;
		DestRect.top    = Info->DestY             ;
		DestRect.bottom = Info->DestY + SrcHeight ;
	}

	if( Info->IsBlend || SubImageHandle >= 0 )
	{
		if( GRAPHCHK( Info->IsBlend ? Info->BlendGrHandle : SubImageHandle, BlendImage ) )
			return -1 ;

		BlendTexture    = BlendImage->Hard.Draw[ 0 ].Tex->PF->D3D11.Texture ;
		BlendTextureSRV = BlendImage->Hard.Draw[ 0 ].Tex->PF->D3D11.TextureSRV ;

		if( Info->BlendPosEnable )
		{
			BlendRect.left   = Info->BlendX             ;
			BlendRect.right  = Info->BlendX + SrcWidth  ;
			BlendRect.top    = Info->BlendY             ;
			BlendRect.bottom = Info->BlendY + SrcHeight ;
		}
		else
		{
			BlendRect.left   = BlendImage->Hard.Draw[ 0 ].UsePosXI ;
			BlendRect.right  = BlendImage->Hard.Draw[ 0 ].UsePosXI + BlendImage->Hard.Draw[ 0 ].WidthI ;
			BlendRect.top    = BlendImage->Hard.Draw[ 0 ].UsePosYI ;
			BlendRect.bottom = BlendImage->Hard.Draw[ 0 ].UsePosYI + BlendImage->Hard.Draw[ 0 ].HeightI ;
		}
	}

	// �]��
	Graphics_D3D11_StretchRect(
		SrcTexture,   SrcTextureSRV,   &SrcRect,
		DestTexture,  DestTextureRTV,  &DestRect,
		IsLinearFilter ? D_D3D11_FILTER_TYPE_LINEAR : D_D3D11_FILTER_TYPE_POINT,
		FALSE,
		NULL, ShaderData->PF->D3D11.PixelShader,
		BlendTexture, BlendTextureSRV, &BlendRect,
		Texcoord8Vertex
	) ;

	// �I��
	return 0 ;
}





extern int	GraphFilter_D3D11_Mono_PF(        GRAPHFILTER_INFO *Info, float Cb, float Cr )
{
	static const char *PsoFileName[ 1 ] =
	{
		"Mono.pso",
	};
	DX_D3D11_SHADER_FLOAT4 *ParamF4 ;

	// �g�p����V�F�[�_�[�̃Z�b�g�A�b�v
	if( GraphFilterShaderHandle.MonoPS < 0 )
	{
		GraphFilterShaderHandle.MonoPS = Direct3D11_MemLoadShaderCode( PsoFileName[ 0 ], false ) ;
		if( GraphFilterShaderHandle.MonoPS < 0 )
		{
			char PathUTF16LE[ 128 ] ;

			ConvString( PsoFileName[ 0 ], DX_CHARCODEFORMAT_ASCII, ( char * )PathUTF16LE, sizeof( PathUTF16LE ), DX_CHARCODEFORMAT_UTF16LE ) ;
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\xd5\x30\xa3\x30\xeb\x30\xbf\x30\xfc\x30\x28\x75\xb7\x30\xa7\x30\xfc\x30\xc0\x30\xfc\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x20\x00\x25\x00\x73\x00\x00"/*@ L"�t�B���^�[�p�V�F�[�_�[�̍쐬�Ɏ��s���܂��� %s" @*/, PathUTF16LE )) ;
			return -1 ;
		}
		NS_SetDeleteHandleFlag( GraphFilterShaderHandle.MonoPS, &GraphFilterShaderHandle.MonoPS ) ;
	}

	ParamF4 = ( DX_D3D11_SHADER_FLOAT4 * )GD3D11.Device.Shader.Constant.ConstBuffer_PS_Filter->SysmemBuffer ;

	ParamF4[ 0 ][ 0 ] = 0.29900f ;
	ParamF4[ 0 ][ 1 ] = 0.58700f ;
	ParamF4[ 0 ][ 2 ] = 0.11400f ;
	ParamF4[ 0 ][ 3 ] = 0.0f ;
	ParamF4[ 1 ][ 0 ] = 1.40200f * Cr ;
	ParamF4[ 1 ][ 1 ] = 0.34414f * Cb - 0.71414f * Cr ;
	ParamF4[ 1 ][ 2 ] = 1.77200f * Cb ;
	ParamF4[ 1 ][ 3 ] = 0.0f ;

	GD3D11.Device.Shader.Constant.ConstBuffer_PS_Filter->ChangeFlag = TRUE ;
	Graphics_D3D11_ConstantBuffer_Update( GD3D11.Device.Shader.Constant.ConstBuffer_PS_Filter ) ;

	Direct3D11_FilterStretchBlt( GraphFilterShaderHandle.MonoPS, Info, FALSE ) ;

	// ����I��
	return 0 ;
}

extern int	GraphFilter_D3D11_Gauss_PF( GRAPHFILTER_INFO *Info, int PixelWidth, float Param, float *Table )
{
	static const char *PsoFileName[] =
	{
		"Gauss_08Pixel_PS.pso",
		"Gauss_16Pixel_PS.pso",
		"Gauss_32Pixel_PS.pso"
	} ;
	IMAGEDATA               *SrcImage      = NULL ;
	SHADOWMAPDATA           *SrcShadowMap  = NULL ;
	IMAGEDATA               *DestImage     = NULL ;
	SHADOWMAPDATA           *DestShadowMap = NULL ;
	int                      UseShader = 0 ;
	int                      TexWidth ;
	int                      TexHeight ;
	float                    TexWidthF ;
	float                    TexHeightF ;
//	RECT                     BltRect ;

	if( GRAPHCHK(     Info->SrcGrHandle, SrcImage    ) &&
		SHADOWMAPCHK( Info->SrcGrHandle, SrcShadowMap ) )
	{
		return -1 ;
	}

	// PixelWidth �ɂ���ăV�F�[�_�[��ύX
	switch( PixelWidth )
	{
	case 8  : UseShader = 0 ; break ;
	case 16 : UseShader = 1 ; break ;
	case 32 : UseShader = 2 ; break ;
	}

	NS_GetGraphTextureSize( Info->SrcGrHandle, &TexWidth, &TexHeight ) ;
	TexWidthF  = ( float )TexWidth ;
	TexHeightF = ( float )TexHeight ;
/*	BltRect.left   = 0 ;
	BltRect.top    = 0 ;
	BltRect.right  = TexWidth ;
	BltRect.bottom = TexHeight ;
*/
/*	if( Param < 0.0000001f )
	{
		IMAGEDATA *DestImage = NULL ;
		SHADOWMAPDATA *DestShadowMap = NULL ;

		if( GRAPHCHK(     Info->DestGrHandle, DestImage    ) &&
			SHADOWMAPCHK( Info->DestGrHandle, DestShadowMap ) )
			return -1 ;

		if( DestShadowMap )
		{
			Graphics_D3D11_StretchRect(
				SrcImage->Hard.Draw[ 0 ].Tex->PF->D3D11.Texture, SrcImage->Hard.Draw[ 0 ].Tex->PF->D3D11.TextureSRV, &BltRect,
				DestShadowMap->PF->D3D11.DepthTexture,           DestShadowMap->PF->D3D11.DepthTextureRTV,           &BltRect ) ;
		}
		else
		{
			Graphics_D3D11_StretchRect(
				SrcImage->Hard.Draw[ 0 ].Tex->PF->D3D11.Texture,  SrcImage->Hard.Draw[ 0 ].Tex->PF->D3D11.TextureSRV,  &BltRect,
				DestImage->Hard.Draw[ 0 ].Tex->PF->D3D11.Texture, DestImage->Hard.Draw[ 0 ].Tex->PF->D3D11.TextureRTV, &BltRect ) ;
		}
	}
	else */
	{
		DX_D3D11_SHADER_FLOAT4 *ParamF4 ;

		// �g�p����V�F�[�_�[�̃Z�b�g�A�b�v
		if( GraphFilterShaderHandle.Gauss_PS[ UseShader ] < 0 )
		{
			GraphFilterShaderHandle.Gauss_PS[ UseShader ] = Direct3D11_MemLoadShaderCode( PsoFileName[ UseShader ], false ) ;
			if( GraphFilterShaderHandle.Gauss_PS[ UseShader ] < 0 )
			{
				char PathUTF16LE[ 128 ] ;

				ConvString( PsoFileName[ UseShader ], DX_CHARCODEFORMAT_ASCII, ( char * )PathUTF16LE, sizeof( PathUTF16LE ), DX_CHARCODEFORMAT_UTF16LE ) ;
				DXST_ERRORLOGFMT_ADDUTF16LE(( "\xd5\x30\xa3\x30\xeb\x30\xbf\x30\xfc\x30\x28\x75\xb7\x30\xa7\x30\xfc\x30\xc0\x30\xfc\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x20\x00\x25\x00\x73\x00\x00"/*@ L"�t�B���^�[�p�V�F�[�_�[�̍쐬�Ɏ��s���܂��� %s" @*/, PathUTF16LE )) ;
				return -1 ;
			}
			NS_SetDeleteHandleFlag( GraphFilterShaderHandle.Gauss_PS[ UseShader ], &GraphFilterShaderHandle.Gauss_PS[ UseShader ] ) ;
		}

		ParamF4 = ( DX_D3D11_SHADER_FLOAT4 * )GD3D11.Device.Shader.Constant.ConstBuffer_PS_Filter->SysmemBuffer ;

		ParamF4[ 0 ][ 0 ] = Table[ 0 ] ;
		ParamF4[ 0 ][ 1 ] = Table[ 1 ] ;
		ParamF4[ 0 ][ 2 ] = Table[ 2 ] ;
		ParamF4[ 0 ][ 3 ] = Table[ 3 ] ;
		ParamF4[ 1 ][ 0 ] = Table[ 4 ] ;
		ParamF4[ 1 ][ 1 ] = Table[ 5 ] ;
		ParamF4[ 1 ][ 2 ] = Table[ 6 ] ;
		ParamF4[ 1 ][ 3 ] = Table[ 7 ] ;
		ParamF4[ 2 ][ 0 ] = Table[ 8 ] ;
		ParamF4[ 2 ][ 1 ] = Table[ 9 ] ;
		ParamF4[ 2 ][ 2 ] = Table[ 10 ] ;
		ParamF4[ 2 ][ 3 ] = Table[ 11 ] ;
		ParamF4[ 3 ][ 0 ] = Table[ 12 ] ;
		ParamF4[ 3 ][ 1 ] = Table[ 13 ] ;
		ParamF4[ 3 ][ 2 ] = Table[ 14 ] ;
		ParamF4[ 3 ][ 3 ] = Table[ 15 ] ;
		ParamF4 += 4 ;

		// �p�X�ɂ���ď����𕪊�
		switch( Info->Pass )
		{
		case 0 :
			ParamF4[ 0 ][ 0 ] =  0.0f / TexWidthF ; ParamF4[ 0 ][ 1 ] = 0.0f ;
			ParamF4[ 0 ][ 2 ] =  1.5f / TexWidthF ; ParamF4[ 0 ][ 3 ] = 0.0f ;
			ParamF4[ 1 ][ 0 ] =  3.5f / TexWidthF ; ParamF4[ 1 ][ 1 ] = 0.0f ;
			ParamF4[ 1 ][ 2 ] =  5.5f / TexWidthF ; ParamF4[ 1 ][ 3 ] = 0.0f ;
			ParamF4[ 2 ][ 0 ] =  7.5f / TexWidthF ; ParamF4[ 2 ][ 1 ] = 0.0f ;
			ParamF4[ 2 ][ 2 ] =  9.5f / TexWidthF ; ParamF4[ 2 ][ 3 ] = 0.0f ;
			ParamF4[ 3 ][ 0 ] = 11.5f / TexWidthF ; ParamF4[ 3 ][ 1 ] = 0.0f ;
			ParamF4[ 3 ][ 2 ] = 13.5f / TexWidthF ; ParamF4[ 3 ][ 3 ] = 0.0f ;
			ParamF4[ 4 ][ 0 ] = 15.5f / TexWidthF ; ParamF4[ 4 ][ 1 ] = 0.0f ;
			ParamF4[ 4 ][ 2 ] = 17.5f / TexWidthF ; ParamF4[ 4 ][ 3 ] = 0.0f ;
			ParamF4[ 5 ][ 0 ] = 19.5f / TexWidthF ; ParamF4[ 5 ][ 1 ] = 0.0f ;
			ParamF4[ 5 ][ 2 ] = 21.5f / TexWidthF ; ParamF4[ 5 ][ 3 ] = 0.0f ;
			ParamF4[ 6 ][ 0 ] = 23.5f / TexWidthF ; ParamF4[ 6 ][ 1 ] = 0.0f ;
			ParamF4[ 6 ][ 2 ] = 25.5f / TexWidthF ; ParamF4[ 6 ][ 3 ] = 0.0f ;
			ParamF4[ 7 ][ 0 ] = 27.5f / TexWidthF ; ParamF4[ 7 ][ 1 ] = 0.0f ;
			ParamF4[ 7 ][ 2 ] = 29.5f / TexWidthF ; ParamF4[ 7 ][ 3 ] = 0.0f ;
			break ;

		case 1 :
			ParamF4[ 0 ][ 0 ] = 0.0f ; ParamF4[ 0 ][ 1 ] =  0.0f / TexHeightF ;
			ParamF4[ 0 ][ 2 ] = 0.0f ; ParamF4[ 0 ][ 3 ] =  1.5f / TexHeightF ;
			ParamF4[ 1 ][ 0 ] = 0.0f ; ParamF4[ 1 ][ 1 ] =  3.5f / TexHeightF ;
			ParamF4[ 1 ][ 2 ] = 0.0f ; ParamF4[ 1 ][ 3 ] =  5.5f / TexHeightF ;
			ParamF4[ 2 ][ 0 ] = 0.0f ; ParamF4[ 2 ][ 1 ] =  7.5f / TexHeightF ;
			ParamF4[ 2 ][ 2 ] = 0.0f ; ParamF4[ 2 ][ 3 ] =  9.5f / TexHeightF ;
			ParamF4[ 3 ][ 0 ] = 0.0f ; ParamF4[ 3 ][ 1 ] = 11.5f / TexHeightF ;
			ParamF4[ 3 ][ 2 ] = 0.0f ; ParamF4[ 3 ][ 3 ] = 13.5f / TexHeightF ;
			ParamF4[ 4 ][ 0 ] = 0.0f ; ParamF4[ 4 ][ 1 ] = 15.5f / TexHeightF ;
			ParamF4[ 4 ][ 2 ] = 0.0f ; ParamF4[ 4 ][ 3 ] = 17.5f / TexHeightF ;
			ParamF4[ 5 ][ 0 ] = 0.0f ; ParamF4[ 5 ][ 1 ] = 19.5f / TexHeightF ;
			ParamF4[ 5 ][ 2 ] = 0.0f ; ParamF4[ 5 ][ 3 ] = 21.5f / TexHeightF ;
			ParamF4[ 6 ][ 0 ] = 0.0f ; ParamF4[ 6 ][ 1 ] = 23.5f / TexHeightF ;
			ParamF4[ 6 ][ 2 ] = 0.0f ; ParamF4[ 6 ][ 3 ] = 25.5f / TexHeightF ;
			ParamF4[ 7 ][ 0 ] = 0.0f ; ParamF4[ 7 ][ 1 ] = 27.5f / TexHeightF ;
			ParamF4[ 7 ][ 2 ] = 0.0f ; ParamF4[ 7 ][ 3 ] = 29.5f / TexHeightF ;
			break ;
		}

		GD3D11.Device.Shader.Constant.ConstBuffer_PS_Filter->ChangeFlag = TRUE ;
		Graphics_D3D11_ConstantBuffer_Update( GD3D11.Device.Shader.Constant.ConstBuffer_PS_Filter ) ;

		Direct3D11_FilterStretchBlt( GraphFilterShaderHandle.Gauss_PS[ UseShader ], Info ) ;

		if( Info->Pass == 0 )
		{
			D_ID3D11Texture2D          *SrcTexture      = NULL ;
			D_ID3D11ShaderResourceView *SrcTextureSRV   = NULL ;
			D_ID3D11Texture2D          *DestTexture     = NULL ;
			D_ID3D11RenderTargetView   *DestTextureRTV  = NULL ;
			RECT SrcRect ;
			RECT DestRect ;
			int Width ;
			int Height ;
			int TexWidth ;
			int TexHeight ;

			if( GRAPHCHK(     Info->SrcGrHandle, SrcImage    ) &&
				SHADOWMAPCHK( Info->SrcGrHandle, SrcShadowMap ) )
			{
				return -1 ;
			}

			if( GRAPHCHK(     Info->DestGrHandle, DestImage    ) &&
				SHADOWMAPCHK( Info->DestGrHandle, DestShadowMap ) )
			{
				return -1 ;
			}

			if( SrcShadowMap != NULL )
			{
				SrcTexture    = SrcShadowMap->PF->D3D11.DepthTexture ;
				SrcTextureSRV = SrcShadowMap->PF->D3D11.DepthTextureSRV ;
			}
			else
			{
				SrcTexture    = SrcImage->Hard.Draw[ 0 ].Tex->PF->D3D11.Texture ;
				SrcTextureSRV = SrcImage->Hard.Draw[ 0 ].Tex->PF->D3D11.TextureSRV ;
			}

			if( DestShadowMap != NULL )
			{
				DestTexture    = DestShadowMap->PF->D3D11.DepthTexture ;
				DestTextureRTV = DestShadowMap->PF->D3D11.DepthTextureRTV ;
			}
			else
			{
				DestTexture    = DestImage->Hard.Draw[ 0 ].Tex->PF->D3D11.Texture ;
				DestTextureRTV = DestImage->Hard.Draw[ 0 ].Tex->PF->D3D11.TextureRTV[ 0 ] ;
			}

			GetGraphTextureSize( Info->DestGrHandle, &TexWidth, &TexHeight ) ;
			Width  = Info->SrcX2 - Info->SrcX1 ;
			Height = Info->SrcY2 - Info->SrcY1 ;

			SrcRect.left    = Info->SrcX1 ;
			SrcRect.right   = Info->SrcX2 ;
			SrcRect.top     = Info->SrcY2 - 1 ;
			SrcRect.bottom  = Info->SrcY2 ;

			DestRect.left   = Info->DestX ;
			DestRect.right  = Info->DestX + Width ;
			DestRect.top    = Info->DestY + Height ;
			DestRect.bottom = TexHeight ;

			if( DestRect.top != DestRect.bottom )
			{
				Graphics_D3D11_StretchRect(
					SrcTexture,  SrcTextureSRV,  &SrcRect,
					DestTexture, DestTextureRTV, &DestRect
				) ;
			}
		}
	}

	// ����I��
	return 0 ;
}

extern int	GraphFilter_D3D11_Down_Scale_PF(  GRAPHFILTER_INFO *Info, int DivNum )
{
	static const char *PsoFileName[ 3 ] =
	{
		"DownScaleX2.pso",
		"DownScaleX4.pso",
		"DownScaleX8.pso",
	} ;
	IMAGEDATA              *SrcImage ;
	int                    UseShader = 0 ;
	int                    UseConstNum = 0 ;
	DX_D3D11_SHADER_FLOAT4 *ParamF4 ;
	int                    TextureW ;
	int                    TextureH ;
	float                  TextureWF ;
	float                  TextureHF ;
	int                    i ;

	if( GRAPHCHK( Info->SrcGrHandle, SrcImage ) )
	{
		return -1 ;
	}

	// �g�p����V�F�[�_�[�̃Z�b�g�A�b�v
	switch( DivNum )
	{
	case 2 : UseShader = 0 ; break ;
	case 4 : UseShader = 1 ; break ;
	case 8 : UseShader = 2 ; break ;
	}
	if( GraphFilterShaderHandle.DownScalePS[ UseShader ] < 0 )
	{
		GraphFilterShaderHandle.DownScalePS[ UseShader ] = Direct3D11_MemLoadShaderCode( PsoFileName[ UseShader ], false ) ;
		if( GraphFilterShaderHandle.DownScalePS[ UseShader ] < 0 )
		{
			char PathUTF16LE[ 128 ] ;

			ConvString( PsoFileName[ UseShader ], DX_CHARCODEFORMAT_ASCII, ( char * )PathUTF16LE, sizeof( PathUTF16LE ), DX_CHARCODEFORMAT_UTF16LE ) ;
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\xd5\x30\xa3\x30\xeb\x30\xbf\x30\xfc\x30\x28\x75\xb7\x30\xa7\x30\xfc\x30\xc0\x30\xfc\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x20\x00\x25\x00\x73\x00\x00"/*@ L"�t�B���^�[�p�V�F�[�_�[�̍쐬�Ɏ��s���܂��� %s" @*/, PathUTF16LE )) ;
			return -1 ;
		}
		NS_SetDeleteHandleFlag( GraphFilterShaderHandle.DownScalePS[ UseShader ], &GraphFilterShaderHandle.DownScalePS[ UseShader ] ) ;
	}

	ParamF4 = ( DX_D3D11_SHADER_FLOAT4 * )GD3D11.Device.Shader.Constant.ConstBuffer_PS_Filter->SysmemBuffer ;

	switch( DivNum )
	{
	case 2 :
		ParamF4[ 0 ][ 0 ] = 0.0f ; ParamF4[ 0 ][ 1 ] = 0.0f ;
		UseConstNum = 1 ;
		break ;

	case 4 :
		ParamF4[ 0 ][ 0 ] = -1.0f ; ParamF4[ 0 ][ 1 ] = -1.0f ;
		ParamF4[ 0 ][ 2 ] =  1.0f ; ParamF4[ 0 ][ 3 ] = -1.0f ;
		ParamF4[ 1 ][ 0 ] = -1.0f ; ParamF4[ 1 ][ 1 ] =  1.0f ;
		ParamF4[ 1 ][ 2 ] =  1.0f ; ParamF4[ 1 ][ 3 ] =  1.0f ;
		UseConstNum = 4 ;
		break ;

	case 8 :
		ParamF4[ 0 ][ 0 ] = -3.0f ; ParamF4[ 0 ][ 1 ] = -3.0f ;
		ParamF4[ 0 ][ 2 ] = -1.0f ; ParamF4[ 0 ][ 3 ] = -3.0f ;
		ParamF4[ 1 ][ 0 ] =  1.0f ; ParamF4[ 1 ][ 1 ] = -3.0f ;
		ParamF4[ 1 ][ 2 ] =  3.0f ; ParamF4[ 1 ][ 3 ] = -3.0f ;
		ParamF4[ 2 ][ 0 ] = -3.0f ; ParamF4[ 2 ][ 1 ] = -1.0f ;
		ParamF4[ 2 ][ 2 ] = -1.0f ; ParamF4[ 2 ][ 3 ] = -1.0f ;
		ParamF4[ 3 ][ 0 ] =  1.0f ; ParamF4[ 3 ][ 1 ] = -1.0f ;
		ParamF4[ 3 ][ 2 ] =  3.0f ; ParamF4[ 3 ][ 3 ] = -1.0f ;
		ParamF4[ 4 ][ 0 ] = -3.0f ; ParamF4[ 4 ][ 1 ] =  1.0f ;
		ParamF4[ 4 ][ 2 ] = -1.0f ; ParamF4[ 4 ][ 3 ] =  1.0f ;
		ParamF4[ 5 ][ 0 ] =  1.0f ; ParamF4[ 5 ][ 1 ] =  1.0f ;
		ParamF4[ 5 ][ 2 ] =  3.0f ; ParamF4[ 5 ][ 3 ] =  1.0f ;
		ParamF4[ 6 ][ 0 ] = -3.0f ; ParamF4[ 6 ][ 1 ] =  3.0f ;
		ParamF4[ 6 ][ 2 ] = -1.0f ; ParamF4[ 6 ][ 3 ] =  3.0f ;
		ParamF4[ 7 ][ 0 ] =  1.0f ; ParamF4[ 7 ][ 1 ] =  3.0f ;
		ParamF4[ 7 ][ 2 ] =  3.0f ; ParamF4[ 7 ][ 3 ] =  3.0f ;
		UseConstNum = 16 ;
		break ;
	}
	NS_GetGraphTextureSize( Info->SrcGrHandle, &TextureW, &TextureH ) ;
	TextureWF = ( float )TextureW ;
	TextureHF = ( float )TextureH ;
	for( i = 0 ; i < UseConstNum ; i ++ )
	{
		if( i % 2 == 0 )
		{
			ParamF4[ i / 2 ][ 0 ] /= TextureWF ;
			ParamF4[ i / 2 ][ 1 ] /= TextureHF ;
		}
		else
		{
			ParamF4[ i / 2 ][ 2 ] /= TextureWF ;
			ParamF4[ i / 2 ][ 3 ] /= TextureHF ;
		}
	}

	GD3D11.Device.Shader.Constant.ConstBuffer_PS_Filter->ChangeFlag = TRUE ;
	Graphics_D3D11_ConstantBuffer_Update( GD3D11.Device.Shader.Constant.ConstBuffer_PS_Filter ) ;

	Direct3D11_FilterStretchBlt( GraphFilterShaderHandle.DownScalePS[ UseShader ], Info, TRUE, DivNum ) ;

	// ����I��
	return 0 ;
}

extern int	GraphFilter_D3D11_Bright_Clip_PF( GRAPHFILTER_INFO *Info, int CmpType, float CmpParam, int ClipFillFlag, COLOR_F *ClipFillColor )
{
	static const char *PsoFileName[ 2 ][ 2 ] =
	{
		"BrightClipLess.pso",
		"BrightClipFillLess.pso",
		"BrightClipGreater.pso",
		"BrightClipFillGreater.pso",
	} ;
	int UseShader = 0 ;
	int UseFill ;
	DX_D3D11_SHADER_FLOAT4 *ParamF4 ;

	// �g�p����V�F�[�_�[�̃Z�b�g�A�b�v
	UseFill = ClipFillFlag == TRUE ? 1 : 0 ;
	switch( CmpType )
	{
	case DX_CMP_GREATER :
		UseShader = 1 ;
		break ;

	case DX_CMP_LESS :
		UseShader = 0 ;
		break ;
	}
	if( GraphFilterShaderHandle.BrightClipPS[ UseShader ][ UseFill ] < 0 )
	{
		GraphFilterShaderHandle.BrightClipPS[ UseShader ][ UseFill ] = Direct3D11_MemLoadShaderCode( PsoFileName[ UseShader ][ UseFill ], false ) ;
		if( GraphFilterShaderHandle.BrightClipPS[ UseShader ][ UseFill ] < 0 )
		{
			char PathUTF16LE[ 128 ] ;

			ConvString( PsoFileName[ UseShader ][ UseFill ], DX_CHARCODEFORMAT_ASCII, ( char * )PathUTF16LE, sizeof( PathUTF16LE ), DX_CHARCODEFORMAT_UTF16LE ) ;
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\xd5\x30\xa3\x30\xeb\x30\xbf\x30\xfc\x30\x28\x75\xb7\x30\xa7\x30\xfc\x30\xc0\x30\xfc\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x20\x00\x25\x00\x73\x00\x00"/*@ L"�t�B���^�[�p�V�F�[�_�[�̍쐬�Ɏ��s���܂��� %s" @*/, PathUTF16LE )) ;
			return -1 ;
		}
		NS_SetDeleteHandleFlag( GraphFilterShaderHandle.BrightClipPS[ UseShader ][ UseFill ], &GraphFilterShaderHandle.BrightClipPS[ UseShader ][ UseFill ] ) ;
	}

	ParamF4 = ( DX_D3D11_SHADER_FLOAT4 * )GD3D11.Device.Shader.Constant.ConstBuffer_PS_Filter->SysmemBuffer ;

	ParamF4[  0 ][ 0 ] = CmpParam ;
	ParamF4[  0 ][ 1 ] = CmpParam ;
	ParamF4[  0 ][ 2 ] = CmpParam ;
	ParamF4[  0 ][ 3 ] = CmpParam ;
	ParamF4[  1 ][ 0 ] = 0.299f ;
	ParamF4[  1 ][ 1 ] = 0.587f ;
	ParamF4[  1 ][ 2 ] = 0.114f ;
	ParamF4[  1 ][ 3 ] = 0.0f ;
	if( ClipFillColor != NULL )
	{
		ParamF4[  2 ][ 0 ] = ClipFillColor->r ;
		ParamF4[  2 ][ 1 ] = ClipFillColor->g ;
		ParamF4[  2 ][ 2 ] = ClipFillColor->b ;
		ParamF4[  2 ][ 3 ] = ClipFillColor->a ;
	}

	GD3D11.Device.Shader.Constant.ConstBuffer_PS_Filter->ChangeFlag = TRUE ;
	Graphics_D3D11_ConstantBuffer_Update( GD3D11.Device.Shader.Constant.ConstBuffer_PS_Filter ) ;

	Direct3D11_FilterStretchBlt( GraphFilterShaderHandle.BrightClipPS[ UseShader ][ UseFill ], Info, FALSE ) ;

	// ����I��
	return 0 ;
}

extern int	GraphFilter_D3D11_Bright_Scale_PF( GRAPHFILTER_INFO *Info, int BrightMin, int BrightMax )
{
	static const char *PsoFileName[ 1 ] =
	{
		"BrightnessScale.pso",
	} ;
	DX_D3D11_SHADER_FLOAT4 *ParamF4 ;

	// �g�p����V�F�[�_�[�̃Z�b�g�A�b�v
	if( GraphFilterShaderHandle.BrightScalePS < 0 )
	{
		GraphFilterShaderHandle.BrightScalePS = Direct3D11_MemLoadShaderCode( PsoFileName[ 0 ], false ) ;
		if( GraphFilterShaderHandle.BrightScalePS < 0 )
		{
			char PathUTF16LE[ 128 ] ;

			ConvString( PsoFileName[ 0 ], DX_CHARCODEFORMAT_ASCII, ( char * )PathUTF16LE, sizeof( PathUTF16LE ), DX_CHARCODEFORMAT_UTF16LE ) ;
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\xd5\x30\xa3\x30\xeb\x30\xbf\x30\xfc\x30\x28\x75\xb7\x30\xa7\x30\xfc\x30\xc0\x30\xfc\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x20\x00\x25\x00\x73\x00\x00"/*@ L"�t�B���^�[�p�V�F�[�_�[�̍쐬�Ɏ��s���܂��� %s" @*/, PathUTF16LE )) ;
			return -1 ;
		}
		NS_SetDeleteHandleFlag( GraphFilterShaderHandle.BrightScalePS, &GraphFilterShaderHandle.BrightScalePS ) ;
	}

	ParamF4 = ( DX_D3D11_SHADER_FLOAT4 * )GD3D11.Device.Shader.Constant.ConstBuffer_PS_Filter->SysmemBuffer ;

	ParamF4[  0 ][ 0 ] = BrightMin / 255.0f ;
	ParamF4[  0 ][ 1 ] = 255.0f / ( float )( BrightMax - BrightMin ) ;
	ParamF4[  0 ][ 2 ] = 0.0f ;
	ParamF4[  0 ][ 3 ] = 0.0f ;

	GD3D11.Device.Shader.Constant.ConstBuffer_PS_Filter->ChangeFlag = TRUE ;
	Graphics_D3D11_ConstantBuffer_Update( GD3D11.Device.Shader.Constant.ConstBuffer_PS_Filter ) ;

	Direct3D11_FilterStretchBlt( GraphFilterShaderHandle.BrightScalePS, Info, FALSE ) ;

	// ����I��
	return 0 ;
}

extern int	GraphFilter_D3D11_HSB_PF(         GRAPHFILTER_INFO *Info, int HueType, float Hue, float Saturation, float Bright )
{
	static const char *PsoFileName[ 3 ] =
	{
		"Hsb_HSB_BASE.pso",
		"Hsb_HSB_HLOCK.pso",
	} ;
	int                    UseShader ;
	DX_D3D11_SHADER_FLOAT4 *ParamF4 ;

	// �g�p����V�F�[�_�[�̃Z�b�g�A�b�v
	UseShader = HueType ;
	if( GraphFilterShaderHandle.HsbPS[ UseShader ] < 0 )
	{
		GraphFilterShaderHandle.HsbPS[ UseShader ] = Direct3D11_MemLoadShaderCode( PsoFileName[ UseShader ], false ) ;
		if( GraphFilterShaderHandle.HsbPS[ UseShader ] < 0 )
		{
			char PathUTF16LE[ 128 ] ;

			ConvString( PsoFileName[ UseShader ], DX_CHARCODEFORMAT_ASCII, ( char * )PathUTF16LE, sizeof( PathUTF16LE ), DX_CHARCODEFORMAT_UTF16LE ) ;
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\xd5\x30\xa3\x30\xeb\x30\xbf\x30\xfc\x30\x28\x75\xb7\x30\xa7\x30\xfc\x30\xc0\x30\xfc\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x20\x00\x25\x00\x73\x00\x00"/*@ L"�t�B���^�[�p�V�F�[�_�[�̍쐬�Ɏ��s���܂��� %s" @*/, PathUTF16LE )) ;
			return -1 ;
		}
		NS_SetDeleteHandleFlag( GraphFilterShaderHandle.HsbPS[ UseShader ], &GraphFilterShaderHandle.HsbPS[ UseShader ] ) ;
	}

	ParamF4 = ( DX_D3D11_SHADER_FLOAT4 * )GD3D11.Device.Shader.Constant.ConstBuffer_PS_Filter->SysmemBuffer ;

	ParamF4[ 0 ][ 0 ] = Hue ;
	ParamF4[ 0 ][ 1 ] = Saturation > 0.0f ? Saturation * 5.0f : Saturation ;
	ParamF4[ 0 ][ 2 ] = Bright ;
	ParamF4[ 0 ][ 3 ] = 0.0f ;
	ParamF4[ 1 ][ 0 ] = 0.5f ;
	ParamF4[ 1 ][ 1 ] = 0.5f ;
	ParamF4[ 1 ][ 2 ] = 0.5f ;
	ParamF4[ 1 ][ 3 ] = 0.5f ;
	ParamF4[ 2 ][ 0 ] = 1.0f ;
	ParamF4[ 2 ][ 1 ] = 1.0f ;
	ParamF4[ 2 ][ 2 ] = 1.0f ;
	ParamF4[ 2 ][ 3 ] = 1.0f ;

	GD3D11.Device.Shader.Constant.ConstBuffer_PS_Filter->ChangeFlag = TRUE ;
	Graphics_D3D11_ConstantBuffer_Update( GD3D11.Device.Shader.Constant.ConstBuffer_PS_Filter ) ;

	Direct3D11_FilterStretchBlt( GraphFilterShaderHandle.HsbPS[ UseShader ], Info, FALSE ) ;

	// ����I��
	return 0 ;
}

extern int	GraphFilter_D3D11_Invert_PF(      GRAPHFILTER_INFO *Info )
{
	static const char *PsoFileName[ 1 ] =
	{
		"Invert.pso",
	} ;

	// �g�p����V�F�[�_�[�̃Z�b�g�A�b�v
	if( GraphFilterShaderHandle.InvertPS < 0 )
	{
		GraphFilterShaderHandle.InvertPS = Direct3D11_MemLoadShaderCode( PsoFileName[ 0 ], false ) ;
		if( GraphFilterShaderHandle.InvertPS < 0 )
		{
			char PathUTF16LE[ 128 ] ;

			ConvString( PsoFileName[ 0 ], DX_CHARCODEFORMAT_ASCII, ( char * )PathUTF16LE, sizeof( PathUTF16LE ), DX_CHARCODEFORMAT_UTF16LE ) ;
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\xd5\x30\xa3\x30\xeb\x30\xbf\x30\xfc\x30\x28\x75\xb7\x30\xa7\x30\xfc\x30\xc0\x30\xfc\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x20\x00\x25\x00\x73\x00\x00"/*@ L"�t�B���^�[�p�V�F�[�_�[�̍쐬�Ɏ��s���܂��� %s" @*/, PathUTF16LE )) ;
			return -1 ;
		}
		NS_SetDeleteHandleFlag( GraphFilterShaderHandle.InvertPS, &GraphFilterShaderHandle.InvertPS ) ;
	}

	Direct3D11_FilterStretchBlt( GraphFilterShaderHandle.InvertPS, Info, FALSE ) ;

	// ����I��
	return 0 ;
}

extern int	GraphFilter_D3D11_Level_PF(       GRAPHFILTER_INFO *Info, float Min, float Max, float Gamma, float AfterMin, float AfterMax )
{
	static const char *PsoFileName[ 1 ] =
	{
		"Level.pso",
	} ;
	DX_D3D11_SHADER_FLOAT4 *ParamF4 ;

	// �g�p����V�F�[�_�[�̃Z�b�g�A�b�v
	if( GraphFilterShaderHandle.LevelPS < 0 )
	{
		GraphFilterShaderHandle.LevelPS = Direct3D11_MemLoadShaderCode( PsoFileName[ 0 ], false ) ;
		if( GraphFilterShaderHandle.LevelPS < 0 )
		{
			char PathUTF16LE[ 128 ] ;

			ConvString( PsoFileName[ 0 ], DX_CHARCODEFORMAT_ASCII, ( char * )PathUTF16LE, sizeof( PathUTF16LE ), DX_CHARCODEFORMAT_UTF16LE ) ;
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\xd5\x30\xa3\x30\xeb\x30\xbf\x30\xfc\x30\x28\x75\xb7\x30\xa7\x30\xfc\x30\xc0\x30\xfc\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x20\x00\x25\x00\x73\x00\x00"/*@ L"�t�B���^�[�p�V�F�[�_�[�̍쐬�Ɏ��s���܂��� %s" @*/, PathUTF16LE )) ;
			return -1 ;
		}
		NS_SetDeleteHandleFlag( GraphFilterShaderHandle.LevelPS, &GraphFilterShaderHandle.LevelPS ) ;
	}

	ParamF4 = ( DX_D3D11_SHADER_FLOAT4 * )GD3D11.Device.Shader.Constant.ConstBuffer_PS_Filter->SysmemBuffer ;

	ParamF4[ 0 ][ 0 ] = Min ;
	ParamF4[ 0 ][ 1 ] = 1.0f / ( Max - Min ) ;
	ParamF4[ 0 ][ 2 ] = AfterMin ;
	ParamF4[ 0 ][ 3 ] = AfterMax ;

	GD3D11.Device.Shader.Constant.ConstBuffer_PS_Filter->ChangeFlag = TRUE ;
	Graphics_D3D11_ConstantBuffer_Update( GD3D11.Device.Shader.Constant.ConstBuffer_PS_Filter ) ;

	Direct3D11_FilterStretchBlt( GraphFilterShaderHandle.LevelPS, Info, TRUE, 1, GraphFilterShaderHandle.GammaTex ) ;

	// ����I��
	return 0 ;
}

extern int	GraphFilter_D3D11_TwoColor_PF(    GRAPHFILTER_INFO *Info, float Threshold, COLOR_F *LowColor, COLOR_F *HighColor )
{
	Threshold /= 255.0f ;

	static const char *PsoFileName[ 1 ] =
	{
		"TwoColor.pso",
	} ;
	DX_D3D11_SHADER_FLOAT4 *ParamF4 ;

	// �g�p����V�F�[�_�[�̃Z�b�g�A�b�v
	if( GraphFilterShaderHandle.TwoColorPS < 0 )
	{
		GraphFilterShaderHandle.TwoColorPS = Direct3D11_MemLoadShaderCode( PsoFileName[ 0 ], false ) ;
		if( GraphFilterShaderHandle.TwoColorPS < 0 )
		{
			char PathUTF16LE[ 128 ] ;

			ConvString( PsoFileName[ 0 ], DX_CHARCODEFORMAT_ASCII, ( char * )PathUTF16LE, sizeof( PathUTF16LE ), DX_CHARCODEFORMAT_UTF16LE ) ;
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\xd5\x30\xa3\x30\xeb\x30\xbf\x30\xfc\x30\x28\x75\xb7\x30\xa7\x30\xfc\x30\xc0\x30\xfc\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x20\x00\x25\x00\x73\x00\x00"/*@ L"�t�B���^�[�p�V�F�[�_�[�̍쐬�Ɏ��s���܂��� %s" @*/, PathUTF16LE )) ;
			return -1 ;
		}
		NS_SetDeleteHandleFlag( GraphFilterShaderHandle.TwoColorPS, &GraphFilterShaderHandle.TwoColorPS ) ;
	}

	ParamF4 = ( DX_D3D11_SHADER_FLOAT4 * )GD3D11.Device.Shader.Constant.ConstBuffer_PS_Filter->SysmemBuffer ;

	ParamF4[ 0 ][ 0 ] = 0.29900f ;
	ParamF4[ 0 ][ 1 ] = 0.58700f ;
	ParamF4[ 0 ][ 2 ] = 0.11400f ;
	ParamF4[ 0 ][ 3 ] = Threshold ;
	ParamF4[ 1 ][ 0 ] = LowColor->r ;
	ParamF4[ 1 ][ 1 ] = LowColor->g ;
	ParamF4[ 1 ][ 2 ] = LowColor->b ;
	ParamF4[ 1 ][ 3 ] = LowColor->a ;
	ParamF4[ 2 ][ 0 ] = HighColor->r ;
	ParamF4[ 2 ][ 1 ] = HighColor->g ;
	ParamF4[ 2 ][ 2 ] = HighColor->b ;
	ParamF4[ 2 ][ 3 ] = HighColor->a ;

	GD3D11.Device.Shader.Constant.ConstBuffer_PS_Filter->ChangeFlag = TRUE ;
	Graphics_D3D11_ConstantBuffer_Update( GD3D11.Device.Shader.Constant.ConstBuffer_PS_Filter ) ;

	Direct3D11_FilterStretchBlt( GraphFilterShaderHandle.TwoColorPS, Info, FALSE ) ;

	// ����I��
	return 0 ;
}

extern int	GraphFilter_D3D11_GradientMap_PF( GRAPHFILTER_INFO *Info, int MapGrHandle, int Reverse )
{
	static const char *PsoFileName[ 2 ] =
	{
		"GradientMap.pso",
		"GradientMapRev.pso",
	} ;
	int                    UseShader ;
	DX_D3D11_SHADER_FLOAT4 *ParamF4 ;

	// �g�p����V�F�[�_�[�̃Z�b�g�A�b�v
	UseShader = Reverse ? 1 : 0 ;
	if( GraphFilterShaderHandle.GradientMapPS[ UseShader ] < 0 )
	{
		GraphFilterShaderHandle.GradientMapPS[ UseShader ] = Direct3D11_MemLoadShaderCode( PsoFileName[ UseShader ], false ) ;
		if( GraphFilterShaderHandle.GradientMapPS[ UseShader ] < 0 )
		{
			char PathUTF16LE[ 128 ] ;

			ConvString( PsoFileName[ UseShader ], DX_CHARCODEFORMAT_ASCII, ( char * )PathUTF16LE, sizeof( PathUTF16LE ), DX_CHARCODEFORMAT_UTF16LE ) ;
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\xd5\x30\xa3\x30\xeb\x30\xbf\x30\xfc\x30\x28\x75\xb7\x30\xa7\x30\xfc\x30\xc0\x30\xfc\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x20\x00\x25\x00\x73\x00\x00"/*@ L"�t�B���^�[�p�V�F�[�_�[�̍쐬�Ɏ��s���܂��� %s" @*/, PathUTF16LE )) ;
			return -1 ;
		}
		NS_SetDeleteHandleFlag( GraphFilterShaderHandle.GradientMapPS[ UseShader ], &GraphFilterShaderHandle.GradientMapPS[ UseShader ] ) ;
	}

	ParamF4 = ( DX_D3D11_SHADER_FLOAT4 * )GD3D11.Device.Shader.Constant.ConstBuffer_PS_Filter->SysmemBuffer ;

	ParamF4[ 0 ][ 0 ] = 0.29900f ;
	ParamF4[ 0 ][ 1 ] = 0.58700f ;
	ParamF4[ 0 ][ 2 ] = 0.11400f ;
	ParamF4[ 0 ][ 3 ] = 0.0f ;

	GD3D11.Device.Shader.Constant.ConstBuffer_PS_Filter->ChangeFlag = TRUE ;
	Graphics_D3D11_ConstantBuffer_Update( GD3D11.Device.Shader.Constant.ConstBuffer_PS_Filter ) ;

	Direct3D11_FilterStretchBlt( GraphFilterShaderHandle.GradientMapPS[ UseShader ], Info, FALSE, 1, MapGrHandle ) ;

	// ����I��
	return 0 ;
}

extern int	GraphFilter_D3D11_PremulAlpha_PF( GRAPHFILTER_INFO *Info )
{
	static const char *PsoFileName[ 1 ] =
	{
		"PremultipliedAlpha.pso",
	} ;
	int UseShader ;

	// �g�p����V�F�[�_�[�̃Z�b�g�A�b�v
	UseShader = 0 ;
	if( GraphFilterShaderHandle.PreMulAlphaPS < 0 )
	{
		GraphFilterShaderHandle.PreMulAlphaPS = Direct3D11_MemLoadShaderCode( PsoFileName[ UseShader ], false ) ;
		if( GraphFilterShaderHandle.PreMulAlphaPS < 0 )
		{
			char PathUTF16LE[ 128 ] ;

			ConvString( PsoFileName[ UseShader ], DX_CHARCODEFORMAT_ASCII, ( char * )PathUTF16LE, sizeof( PathUTF16LE ), DX_CHARCODEFORMAT_UTF16LE ) ;
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\xd5\x30\xa3\x30\xeb\x30\xbf\x30\xfc\x30\x28\x75\xb7\x30\xa7\x30\xfc\x30\xc0\x30\xfc\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x20\x00\x25\x00\x73\x00\x00"/*@ L"�t�B���^�[�p�V�F�[�_�[�̍쐬�Ɏ��s���܂��� %s" @*/, PathUTF16LE )) ;
			return -1 ;
		}
		NS_SetDeleteHandleFlag( GraphFilterShaderHandle.PreMulAlphaPS, &GraphFilterShaderHandle.PreMulAlphaPS ) ;
	}

	Direct3D11_FilterStretchBlt( GraphFilterShaderHandle.PreMulAlphaPS, Info, FALSE ) ;

	// ����I��
	return 0 ;
}

extern int	GraphFilter_D3D11_InterpAlpha_PF( GRAPHFILTER_INFO *Info )
{
	static const char *PsoFileName[ 1 ] =
	{
		"InterpolatedAlpha.pso",
	} ;
	int UseShader ;

	// �g�p����V�F�[�_�[�̃Z�b�g�A�b�v
	UseShader = 0 ;
	if( GraphFilterShaderHandle.InterpAlphaPS < 0 )
	{
		GraphFilterShaderHandle.InterpAlphaPS = Direct3D11_MemLoadShaderCode( PsoFileName[ UseShader ], false ) ;
		if( GraphFilterShaderHandle.InterpAlphaPS < 0 )
		{
			char PathUTF16LE[ 128 ] ;

			ConvString( PsoFileName[ UseShader ], DX_CHARCODEFORMAT_ASCII, ( char * )PathUTF16LE, sizeof( PathUTF16LE ), DX_CHARCODEFORMAT_UTF16LE ) ;
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\xd5\x30\xa3\x30\xeb\x30\xbf\x30\xfc\x30\x28\x75\xb7\x30\xa7\x30\xfc\x30\xc0\x30\xfc\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x20\x00\x25\x00\x73\x00\x00"/*@ L"�t�B���^�[�p�V�F�[�_�[�̍쐬�Ɏ��s���܂��� %s" @*/, PathUTF16LE )) ;
			return -1 ;
		}
		NS_SetDeleteHandleFlag( GraphFilterShaderHandle.InterpAlphaPS, &GraphFilterShaderHandle.InterpAlphaPS ) ;
	}

	Direct3D11_FilterStretchBlt( GraphFilterShaderHandle.InterpAlphaPS, Info, FALSE ) ;

	// ����I��
	return 0 ;
}

extern int	GraphFilter_D3D11_YUVtoRGB_PF( GRAPHFILTER_INFO *Info, int UVGrHandle )
{
	static const char *PsoFileName[ 4 ] =
	{
		"Y2uv2ToRgb.pso",
		"Y2uv1ToRgb.pso",

		"Y2uv2ToRgb_RRA.pso",
		"Y2uv1ToRgb_RRA.pso",
	} ;
	int                    UseShader ;
	DX_D3D11_SHADER_FLOAT4 *ParamF4 ;
	int RRAFlag = ( Info->FilterOrBlendType == DX_GRAPH_FILTER_YUV_TO_RGB_RRA || Info->FilterOrBlendType == DX_GRAPH_FILTER_Y2UV1_TO_RGB_RRA ) ? TRUE : FALSE ;

	// �g�p����V�F�[�_�[�̃Z�b�g�A�b�v
	UseShader = ( UVGrHandle < 0 ? 0 : 1 ) + ( RRAFlag ? 2 : 0 ) ;
	if( GraphFilterShaderHandle.YUVtoRGBPS[ UseShader ] < 0 )
	{
		GraphFilterShaderHandle.YUVtoRGBPS[ UseShader ] = Direct3D11_MemLoadShaderCode( PsoFileName[ UseShader ], false ) ;
		if( GraphFilterShaderHandle.YUVtoRGBPS[ UseShader ] < 0 )
		{
			char PathUTF16LE[ 128 ] ;

			ConvString( PsoFileName[ UseShader ], DX_CHARCODEFORMAT_ASCII, ( char * )PathUTF16LE, sizeof( PathUTF16LE ), DX_CHARCODEFORMAT_UTF16LE ) ;
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\xd5\x30\xa3\x30\xeb\x30\xbf\x30\xfc\x30\x28\x75\xb7\x30\xa7\x30\xfc\x30\xc0\x30\xfc\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x20\x00\x25\x00\x73\x00\x00"/*@ L"�t�B���^�[�p�V�F�[�_�[�̍쐬�Ɏ��s���܂��� %s" @*/, PathUTF16LE )) ;
			return -1 ;
		}
		NS_SetDeleteHandleFlag( GraphFilterShaderHandle.YUVtoRGBPS[ UseShader ], &GraphFilterShaderHandle.YUVtoRGBPS[ UseShader ] ) ;
	}

	if( UVGrHandle >= 0 || RRAFlag )
	{
		int YImgWidth, YImgHeight ;
		int YTexWidth, YTexHeight ;
		float YU, YV ;

		NS_GetGraphSize(        Info->SrcGrHandle, &YImgWidth,  &YImgHeight ) ;
		NS_GetGraphTextureSize( Info->SrcGrHandle, &YTexWidth,  &YTexHeight ) ;
		YU  = ( float )YImgWidth  / YTexWidth ;
		YV  = ( float )YImgHeight / YTexHeight ;

		ParamF4 = ( DX_D3D11_SHADER_FLOAT4 * )GD3D11.Device.Shader.Constant.ConstBuffer_PS_Filter->SysmemBuffer ;

		if( UVGrHandle >= 0 )
		{
			int UVImgWidth, UVImgHeight ;
			int UVTexWidth, UVTexHeight ;
			float UVU, UVV ;

			NS_GetGraphSize(        UVGrHandle, &UVImgWidth, &UVImgHeight ) ;
			NS_GetGraphTextureSize( UVGrHandle, &UVTexWidth, &UVTexHeight ) ;
			UVU = ( float )UVImgWidth  / UVTexWidth ;
			UVV = ( float )UVImgHeight / UVTexHeight ;

			ParamF4[ 0 ][ 0 ] = UVU / YU ;
			ParamF4[ 0 ][ 1 ] = UVV / YV ;
			ParamF4[ 0 ][ 2 ] = YU  / 2.0f ;
			ParamF4[ 0 ][ 3 ] = UVU / 2.0f ;
		}
		else
		{
			ParamF4[ 0 ][ 0 ] = 0.0f ;
			ParamF4[ 0 ][ 1 ] = 0.0f ;
			ParamF4[ 0 ][ 2 ] = YU  / 2.0f ;
			ParamF4[ 0 ][ 3 ] = 0.0f ;
		}

		GD3D11.Device.Shader.Constant.ConstBuffer_PS_Filter->ChangeFlag = TRUE ;
		Graphics_D3D11_ConstantBuffer_Update( GD3D11.Device.Shader.Constant.ConstBuffer_PS_Filter ) ;
	}

	Direct3D11_FilterStretchBlt( GraphFilterShaderHandle.YUVtoRGBPS[ UseShader ], Info, FALSE, 1, UVGrHandle ) ;

	// ����I��
	return 0 ;
}

extern int	GraphFilter_D3D11_BicubicScale_PF( GRAPHFILTER_INFO *Info, int DestSizeX, int DestSizeY )
{
	static const char *PsoFileName[ 1 ] =
	{
		"Bicubic.pso",
	} ;
	int                    UseShader ;
	DX_D3D11_SHADER_FLOAT4 *ParamF4 ;
	VERTEX_TEX8_2D VertexTex8[ 4 ] ;

	// �g�p����V�F�[�_�[�̃Z�b�g�A�b�v
	UseShader = 0 ;
	if( GraphFilterShaderHandle.BicubicPS < 0 )
	{
		GraphFilterShaderHandle.BicubicPS = Direct3D11_MemLoadShaderCode( PsoFileName[ UseShader ], false ) ;
		if( GraphFilterShaderHandle.BicubicPS < 0 )
		{
			char PathUTF16LE[ 128 ] ;

			ConvString( PsoFileName[ UseShader ], DX_CHARCODEFORMAT_ASCII, ( char * )PathUTF16LE, sizeof( PathUTF16LE ), DX_CHARCODEFORMAT_UTF16LE ) ;
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\xd5\x30\xa3\x30\xeb\x30\xbf\x30\xfc\x30\x28\x75\xb7\x30\xa7\x30\xfc\x30\xc0\x30\xfc\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x20\x00\x25\x00\x73\x00\x00"/*@ L"�t�B���^�[�p�V�F�[�_�[�̍쐬�Ɏ��s���܂��� %s" @*/, PathUTF16LE )) ;
			return -1 ;
		}
		NS_SetDeleteHandleFlag( GraphFilterShaderHandle.BicubicPS, &GraphFilterShaderHandle.BicubicPS ) ;
	}

	ParamF4 = ( DX_D3D11_SHADER_FLOAT4 * )GD3D11.Device.Shader.Constant.ConstBuffer_PS_Filter->SysmemBuffer ;

	int TexW, TexH ;
	NS_GetGraphTextureSize( Info->SrcGrHandle, &TexW, &TexH ) ;

	ParamF4[ 0 ][ 0 ] = ( float )TexW ;
	ParamF4[ 0 ][ 1 ] = ( float )TexH ;
	ParamF4[ 0 ][ 2 ] = 2.0f / ( float )TexW ;
	ParamF4[ 0 ][ 3 ] = 0.0f ;

	GD3D11.Device.Shader.Constant.ConstBuffer_PS_Filter->ChangeFlag = TRUE ;
	Graphics_D3D11_ConstantBuffer_Update( GD3D11.Device.Shader.Constant.ConstBuffer_PS_Filter ) ;

	float du = 1.0f / TexW ;
	float dv = 1.0f / TexH ;

	VertexTex8[ 0 ].pos.x = 0.0f ;
	VertexTex8[ 0 ].pos.y = 0.0f ;
	VertexTex8[ 0 ].pos.z = 0.2f ;
	VertexTex8[ 0 ].rhw = 1.0f ;
	VertexTex8[ 0 ].u0 = 0.0f - 1.5f * du ;	VertexTex8[ 0 ].v0 = 0.0f - 1.5f * dv ;
	VertexTex8[ 0 ].u1 = 0.0f - 1.5f * du ;	VertexTex8[ 0 ].v1 = 0.0f - 0.5f * dv ;
	VertexTex8[ 0 ].u2 = 0.0f - 1.5f * du ;	VertexTex8[ 0 ].v2 = 0.0f + 0.5f * dv ;
	VertexTex8[ 0 ].u3 = 0.0f - 1.5f * du ;	VertexTex8[ 0 ].v3 = 0.0f + 1.5f * dv ;
	VertexTex8[ 0 ].u4 = 0.0f - 0.5f * du ;	VertexTex8[ 0 ].v4 = 0.0f - 1.5f * dv ;
	VertexTex8[ 0 ].u5 = 0.0f - 0.5f * du ;	VertexTex8[ 0 ].v5 = 0.0f - 0.5f * dv ;
	VertexTex8[ 0 ].u6 = 0.0f - 0.5f * du ;	VertexTex8[ 0 ].v6 = 0.0f + 0.5f * dv ;
	VertexTex8[ 0 ].u7 = 0.0f - 0.5f * du ;	VertexTex8[ 0 ].v7 = 0.0f + 1.5f * dv ;

	VertexTex8[ 1 ].pos.x = ( float )DestSizeX ;
	VertexTex8[ 1 ].pos.y = 0.0f ;
	VertexTex8[ 1 ].pos.z = 0.2f ;
	VertexTex8[ 1 ].rhw = 1.0f ;
	VertexTex8[ 1 ].u0 = 1.0f - 1.5f * du ;	VertexTex8[ 1 ].v0 = 0.0f - 1.5f * dv ;
	VertexTex8[ 1 ].u1 = 1.0f - 1.5f * du ;	VertexTex8[ 1 ].v1 = 0.0f - 0.5f * dv ;
	VertexTex8[ 1 ].u2 = 1.0f - 1.5f * du ;	VertexTex8[ 1 ].v2 = 0.0f + 0.5f * dv ;
	VertexTex8[ 1 ].u3 = 1.0f - 1.5f * du ;	VertexTex8[ 1 ].v3 = 0.0f + 1.5f * dv ;
	VertexTex8[ 1 ].u4 = 1.0f - 0.5f * du ;	VertexTex8[ 1 ].v4 = 0.0f - 1.5f * dv ;
	VertexTex8[ 1 ].u5 = 1.0f - 0.5f * du ;	VertexTex8[ 1 ].v5 = 0.0f - 0.5f * dv ;
	VertexTex8[ 1 ].u6 = 1.0f - 0.5f * du ;	VertexTex8[ 1 ].v6 = 0.0f + 0.5f * dv ;
	VertexTex8[ 1 ].u7 = 1.0f - 0.5f * du ;	VertexTex8[ 1 ].v7 = 0.0f + 1.5f * dv ;

	VertexTex8[ 2 ].pos.x = 0.0f ;
	VertexTex8[ 2 ].pos.y = ( float )DestSizeY ;
	VertexTex8[ 2 ].pos.z = 0.2f ;
	VertexTex8[ 2 ].rhw = 1.0f ;
	VertexTex8[ 2 ].u0 = 0.0f - 1.5f * du ;	VertexTex8[ 2 ].v0 = 1.0f - 1.5f * dv ;
	VertexTex8[ 2 ].u1 = 0.0f - 1.5f * du ;	VertexTex8[ 2 ].v1 = 1.0f - 0.5f * dv ;
	VertexTex8[ 2 ].u2 = 0.0f - 1.5f * du ;	VertexTex8[ 2 ].v2 = 1.0f + 0.5f * dv ;
	VertexTex8[ 2 ].u3 = 0.0f - 1.5f * du ;	VertexTex8[ 2 ].v3 = 1.0f + 1.5f * dv ;
	VertexTex8[ 2 ].u4 = 0.0f - 0.5f * du ;	VertexTex8[ 2 ].v4 = 1.0f - 1.5f * dv ;
	VertexTex8[ 2 ].u5 = 0.0f - 0.5f * du ;	VertexTex8[ 2 ].v5 = 1.0f - 0.5f * dv ;
	VertexTex8[ 2 ].u6 = 0.0f - 0.5f * du ;	VertexTex8[ 2 ].v6 = 1.0f + 0.5f * dv ;
	VertexTex8[ 2 ].u7 = 0.0f - 0.5f * du ;	VertexTex8[ 2 ].v7 = 1.0f + 1.5f * dv ;

	VertexTex8[ 3 ].pos.x = ( float )DestSizeX ;
	VertexTex8[ 3 ].pos.y = ( float )DestSizeY ;
	VertexTex8[ 3 ].pos.z = 0.2f ;
	VertexTex8[ 3 ].rhw = 1.0f ;
	VertexTex8[ 3 ].u0 = 1.0f - 1.5f * du ;	VertexTex8[ 3 ].v0 = 1.0f - 1.5f * dv ;
	VertexTex8[ 3 ].u1 = 1.0f - 1.5f * du ;	VertexTex8[ 3 ].v1 = 1.0f - 0.5f * dv ;
	VertexTex8[ 3 ].u2 = 1.0f - 1.5f * du ;	VertexTex8[ 3 ].v2 = 1.0f + 0.5f * dv ;
	VertexTex8[ 3 ].u3 = 1.0f - 1.5f * du ;	VertexTex8[ 3 ].v3 = 1.0f + 1.5f * dv ;
	VertexTex8[ 3 ].u4 = 1.0f - 0.5f * du ;	VertexTex8[ 3 ].v4 = 1.0f - 1.5f * dv ;
	VertexTex8[ 3 ].u5 = 1.0f - 0.5f * du ;	VertexTex8[ 3 ].v5 = 1.0f - 0.5f * dv ;
	VertexTex8[ 3 ].u6 = 1.0f - 0.5f * du ;	VertexTex8[ 3 ].v6 = 1.0f + 0.5f * dv ;
	VertexTex8[ 3 ].u7 = 1.0f - 0.5f * du ;	VertexTex8[ 3 ].v7 = 1.0f + 1.5f * dv ;

	Direct3D11_FilterStretchBlt( GraphFilterShaderHandle.BicubicPS, Info, FALSE, 1, -1, VertexTex8 ) ;

	// ����I��
	return 0 ;
}

extern int	GraphFilter_D3D11_Lanczos3Scale_PF( GRAPHFILTER_INFO *Info, int DestSizeX, int DestSizeY )
{
	static const char *PsoFileName[ 1 ] =
	{
		"Lanczos3.pso",
	} ;
	int                    UseShader ;
	DX_D3D11_SHADER_FLOAT4 *ParamF4 ;
	VERTEX_TEX8_2D VertexTex8[ 4 ] ;

	// �g�p����V�F�[�_�[�̃Z�b�g�A�b�v
	UseShader = 0 ;
	if( GraphFilterShaderHandle.Lanczos3PS < 0 )
	{
		GraphFilterShaderHandle.Lanczos3PS = Direct3D11_MemLoadShaderCode( PsoFileName[ UseShader ], false ) ;
		if( GraphFilterShaderHandle.Lanczos3PS < 0 )
		{
			char PathUTF16LE[ 128 ] ;

			ConvString( PsoFileName[ UseShader ], DX_CHARCODEFORMAT_ASCII, ( char * )PathUTF16LE, sizeof( PathUTF16LE ), DX_CHARCODEFORMAT_UTF16LE ) ;
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\xd5\x30\xa3\x30\xeb\x30\xbf\x30\xfc\x30\x28\x75\xb7\x30\xa7\x30\xfc\x30\xc0\x30\xfc\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x20\x00\x25\x00\x73\x00\x00"/*@ L"�t�B���^�[�p�V�F�[�_�[�̍쐬�Ɏ��s���܂��� %s" @*/, PathUTF16LE )) ;
			return -1 ;
		}
		NS_SetDeleteHandleFlag( GraphFilterShaderHandle.Lanczos3PS, &GraphFilterShaderHandle.Lanczos3PS ) ;
	}

	ParamF4 = ( DX_D3D11_SHADER_FLOAT4 * )GD3D11.Device.Shader.Constant.ConstBuffer_PS_Filter->SysmemBuffer ;

	int TexW, TexH ;
	NS_GetGraphTextureSize( Info->SrcGrHandle, &TexW, &TexH ) ;

	ParamF4[ 0 ][ 0 ] = ( float )TexW ;
	ParamF4[ 0 ][ 1 ] = ( float )TexH ;
	ParamF4[ 0 ][ 2 ] = 1.0f / ( float )TexW ;
	ParamF4[ 0 ][ 3 ] = 0.0f ;

	GD3D11.Device.Shader.Constant.ConstBuffer_PS_Filter->ChangeFlag = TRUE ;
	Graphics_D3D11_ConstantBuffer_Update( GD3D11.Device.Shader.Constant.ConstBuffer_PS_Filter ) ;

	float du = 1.0f / TexW ;
	float dv = 1.0f / TexH ;

	VertexTex8[ 0 ].pos.x = 0.0f ;
	VertexTex8[ 0 ].pos.y = 0.0f ;
	VertexTex8[ 0 ].pos.z = 0.0f ;
	VertexTex8[ 0 ].rhw = 1.0f ;
	VertexTex8[ 0 ].u0 = 0.0f - 0.5f * du ;	VertexTex8[ 0 ].v0 = 0.0f - 2.5f * dv ;
	VertexTex8[ 0 ].u1 = 0.0f - 0.5f * du ;	VertexTex8[ 0 ].v1 = 0.0f - 1.5f * dv ;
	VertexTex8[ 0 ].u2 = 0.0f - 0.5f * du ;	VertexTex8[ 0 ].v2 = 0.0f - 0.5f * dv ;
	VertexTex8[ 0 ].u3 = 0.0f - 0.5f * du ;	VertexTex8[ 0 ].v3 = 0.0f + 0.5f * dv ;
	VertexTex8[ 0 ].u4 = 0.0f - 0.5f * du ;	VertexTex8[ 0 ].v4 = 0.0f + 1.5f * dv ;
	VertexTex8[ 0 ].u5 = 0.0f - 0.5f * du ;	VertexTex8[ 0 ].v5 = 0.0f + 2.5f * dv ;

	VertexTex8[ 1 ].pos.x = ( float )DestSizeX ;
	VertexTex8[ 1 ].pos.y = 0.0f ;
	VertexTex8[ 1 ].pos.z = 0.0f ;
	VertexTex8[ 1 ].rhw = 1.0f ;
	VertexTex8[ 1 ].u0 = 1.0f - 0.5f * du ;	VertexTex8[ 1 ].v0 = 0.0f - 2.5f * dv ;
	VertexTex8[ 1 ].u1 = 1.0f - 0.5f * du ;	VertexTex8[ 1 ].v1 = 0.0f - 1.5f * dv ;
	VertexTex8[ 1 ].u2 = 1.0f - 0.5f * du ;	VertexTex8[ 1 ].v2 = 0.0f - 0.5f * dv ;
	VertexTex8[ 1 ].u3 = 1.0f - 0.5f * du ;	VertexTex8[ 1 ].v3 = 0.0f + 0.5f * dv ;
	VertexTex8[ 1 ].u4 = 1.0f - 0.5f * du ;	VertexTex8[ 1 ].v4 = 0.0f + 1.5f * dv ;
	VertexTex8[ 1 ].u5 = 1.0f - 0.5f * du ;	VertexTex8[ 1 ].v5 = 0.0f + 2.5f * dv ;

	VertexTex8[ 2 ].pos.x = 0.0f ;
	VertexTex8[ 2 ].pos.y = ( float )DestSizeY ;
	VertexTex8[ 2 ].pos.z = 0.0f ;
	VertexTex8[ 2 ].rhw = 1.0f ;
	VertexTex8[ 2 ].u0 = 0.0f - 0.5f * du ;	VertexTex8[ 2 ].v0 = 1.0f - 2.5f * dv ;
	VertexTex8[ 2 ].u1 = 0.0f - 0.5f * du ;	VertexTex8[ 2 ].v1 = 1.0f - 1.5f * dv ;
	VertexTex8[ 2 ].u2 = 0.0f - 0.5f * du ;	VertexTex8[ 2 ].v2 = 1.0f - 0.5f * dv ;
	VertexTex8[ 2 ].u3 = 0.0f - 0.5f * du ;	VertexTex8[ 2 ].v3 = 1.0f + 0.5f * dv ;
	VertexTex8[ 2 ].u4 = 0.0f - 0.5f * du ;	VertexTex8[ 2 ].v4 = 1.0f + 1.5f * dv ;
	VertexTex8[ 2 ].u5 = 0.0f - 0.5f * du ;	VertexTex8[ 2 ].v5 = 1.0f + 2.5f * dv ;

	VertexTex8[ 3 ].pos.x = ( float )DestSizeX ;
	VertexTex8[ 3 ].pos.y = ( float )DestSizeY ;
	VertexTex8[ 3 ].pos.z = 0.0f ;
	VertexTex8[ 3 ].rhw = 1.0f ;
	VertexTex8[ 3 ].u0 = 1.0f - 0.5f * du ;	VertexTex8[ 3 ].v0 = 1.0f - 2.5f * dv ;
	VertexTex8[ 3 ].u1 = 1.0f - 0.5f * du ;	VertexTex8[ 3 ].v1 = 1.0f - 1.5f * dv ;
	VertexTex8[ 3 ].u2 = 1.0f - 0.5f * du ;	VertexTex8[ 3 ].v2 = 1.0f - 0.5f * dv ;
	VertexTex8[ 3 ].u3 = 1.0f - 0.5f * du ;	VertexTex8[ 3 ].v3 = 1.0f + 0.5f * dv ;
	VertexTex8[ 3 ].u4 = 1.0f - 0.5f * du ;	VertexTex8[ 3 ].v4 = 1.0f + 1.5f * dv ;
	VertexTex8[ 3 ].u5 = 1.0f - 0.5f * du ;	VertexTex8[ 3 ].v5 = 1.0f + 2.5f * dv ;

	Direct3D11_FilterStretchBlt( GraphFilterShaderHandle.Lanczos3PS, Info, FALSE, 1, -1, VertexTex8 ) ;

	// ����I��
	return 0 ;
}

extern int	GraphBlend_D3D11_Basic_PF(           GRAPHFILTER_INFO *Info )
{
	static const char *PsoFileName[ 18 ] =
	{
		"BasBF_Normal.pso",				// DX_GRAPH_BLEND_NORMAL
		NULL,							// DX_GRAPH_BLEND_RGBA_SELECT_MIX
		"BasBF_Multiple.pso",			// DX_GRAPH_BLEND_MULTIPLE
		"BasBF_Difference.pso",			// DX_GRAPH_BLEND_DIFFERENCE
		"BasBF_Add.pso",				// DX_GRAPH_BLEND_ADD
		"BasBF_Screen.pso",				// DX_GRAPH_BLEND_SCREEN
		"BasBF_Overlay.pso",			// DX_GRAPH_BLEND_OVERLAY
		"BasBF_Dodge.pso",				// DX_GRAPH_BLEND_DODGE
		"BasBF_Burn.pso",				// DX_GRAPH_BLEND_BURN
		"BasBF_Darken.pso",				// DX_GRAPH_BLEND_DARKEN
		"BasBF_Lighten.pso",			// DX_GRAPH_BLEND_LIGHTEN
		"BasBF_SoftLight.pso",			// DX_GRAPH_BLEND_SOFTLIGHT
		"BasBF_HardLight.pso",			// DX_GRAPH_BLEND_HARDLIGHT
		"BasBF_Exclusion.pso",			// DX_GRAPH_BLEND_EXCLUSION
		"BasBF_Normal_AlphaCh.pso",		// DX_GRAPH_BLEND_NORMAL_ALPHACH
		"BasBF_Add_AlphaCh.pso",		// DX_GRAPH_BLEND_ADD_ALPHACH
		"BasBF_Multiple_AOnly.pso",		// DX_GRAPH_BLEND_MULTIPLE_A_ONLY
		"BasBF_PMA_Multiple_AOnly.pso",	// DX_GRAPH_BLEND_PMA_MULTIPLE_A_ONLY
	} ;
	int                    UseShader ;
	DX_D3D11_SHADER_FLOAT4 *ParamF4 ;

	// �g�p����V�F�[�_�[�̃Z�b�g�A�b�v
	UseShader = Info->FilterOrBlendType ;
	if( GraphFilterShaderHandle.BasicBlendPS[ UseShader ] < 0 )
	{
		GraphFilterShaderHandle.BasicBlendPS[ UseShader ] = Direct3D11_MemLoadShaderCode( PsoFileName[ UseShader ], false ) ;
		if( GraphFilterShaderHandle.BasicBlendPS[ UseShader ] < 0 )
		{
			char PathUTF16LE[ 128 ] ;

			ConvString( PsoFileName[ UseShader ], DX_CHARCODEFORMAT_ASCII, ( char * )PathUTF16LE, sizeof( PathUTF16LE ), DX_CHARCODEFORMAT_UTF16LE ) ;
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\xd5\x30\xa3\x30\xeb\x30\xbf\x30\xfc\x30\x28\x75\xb7\x30\xa7\x30\xfc\x30\xc0\x30\xfc\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x20\x00\x25\x00\x73\x00\x00"/*@ L"�t�B���^�[�p�V�F�[�_�[�̍쐬�Ɏ��s���܂��� %s" @*/, PathUTF16LE )) ;
			return -1 ;
		}
		NS_SetDeleteHandleFlag( GraphFilterShaderHandle.BasicBlendPS[ UseShader ], &GraphFilterShaderHandle.BasicBlendPS[ UseShader ] ) ;
	}

	ParamF4 = ( DX_D3D11_SHADER_FLOAT4 * )GD3D11.Device.Shader.Constant.ConstBuffer_PS_Filter->SysmemBuffer ;
	
	ParamF4[ 0 ][ 0 ] = Info->BlendRatio ;
	ParamF4[ 0 ][ 1 ] = Info->BlendRatio ;
	ParamF4[ 0 ][ 2 ] = Info->BlendRatio ;
	ParamF4[ 0 ][ 3 ] = Info->BlendRatio ;

	GD3D11.Device.Shader.Constant.ConstBuffer_PS_Filter->ChangeFlag = TRUE ;
	Graphics_D3D11_ConstantBuffer_Update( GD3D11.Device.Shader.Constant.ConstBuffer_PS_Filter ) ;

	Direct3D11_FilterStretchBlt( GraphFilterShaderHandle.BasicBlendPS[ UseShader ], Info ) ;

	// ����I��
	return 0 ;
}

extern int	GraphBlend_D3D11_RGBA_Select_Mix_PF( GRAPHFILTER_INFO *Info, int SelectR, int SelectG, int SelectB, int SelectA )
{
	static const char *PsoFileName[ 1 ] =
	{
		"RGBAMix_Base.pso",
	} ;
	static const char *CharTable[ 4 ] =
	{
		"R", "G", "B", "A"
	};
	int                    *PixelShaderHandle ;
	DX_D3D11_SHADER_FLOAT4 *ParamF4 ;
	char                   FileName[ 64 ] ;
	int                    SrcBlendReverse = FALSE ;
	int                    HandleTemp ;
	int                    SrcOnlyFlag = FALSE ;
	char                   PathUTF16LE[ 64 ] ;

	// �g�p����V�F�[�_�[�̃Z�b�g�A�b�v
	if( SelectR >= DX_RGBA_SELECT_SRC_R && SelectR <= DX_RGBA_SELECT_SRC_A &&
		SelectG >= DX_RGBA_SELECT_SRC_R && SelectG <= DX_RGBA_SELECT_SRC_A &&
		SelectB >= DX_RGBA_SELECT_SRC_R && SelectB <= DX_RGBA_SELECT_SRC_A &&
		SelectA >= DX_RGBA_SELECT_SRC_R && SelectA <= DX_RGBA_SELECT_SRC_A )
	{
		SrcOnlyFlag = TRUE ;
	}
	else
	if( SelectR >= DX_RGBA_SELECT_BLEND_R && SelectR <= DX_RGBA_SELECT_BLEND_A &&
		SelectG >= DX_RGBA_SELECT_BLEND_R && SelectG <= DX_RGBA_SELECT_BLEND_A &&
		SelectB >= DX_RGBA_SELECT_BLEND_R && SelectB <= DX_RGBA_SELECT_BLEND_A &&
		SelectA >= DX_RGBA_SELECT_BLEND_R && SelectA <= DX_RGBA_SELECT_BLEND_A )
	{
		SelectR -= DX_RGBA_SELECT_BLEND_R ;
		SelectG -= DX_RGBA_SELECT_BLEND_R ;
		SelectB -= DX_RGBA_SELECT_BLEND_R ;
		SelectA -= DX_RGBA_SELECT_BLEND_R ;

		SrcOnlyFlag     = TRUE ;
		SrcBlendReverse = TRUE ;

		HandleTemp          = Info->BlendGrHandle ;
		Info->BlendGrHandle = Info->SrcGrHandle ;
		Info->SrcGrHandle   = HandleTemp ;
	}

	if( SrcOnlyFlag )
	{
		PixelShaderHandle = &GraphFilterShaderHandle.RgbaMixS[ SelectR ][ SelectG ][ SelectB ][ SelectA ] ;
		if( *PixelShaderHandle < 0 )
		{
			if( GD3D11.ShaderCode.Base.RGBAMixS_ShaderPackImage != NULL )
			{
				*PixelShaderHandle = NS_LoadPixelShaderFromMem( GD3D11.ShaderCode.Base.RGBAMixS_PS_Code[ SelectR ][ SelectG ][ SelectB ][ SelectA ].Binary, GD3D11.ShaderCode.Base.RGBAMixS_PS_Code[ SelectR ][ SelectG ][ SelectB ][ SelectA ].Size ) ;
				if( *PixelShaderHandle < 0 )
				{
					DXST_ERRORLOGFMT_ADDUTF16LE(( "\x52\x00\x47\x00\x42\x00\x41\x00\x4d\x00\x69\x00\x78\x00\x53\x00\x20\x00\xd5\x30\xa3\x30\xeb\x30\xbf\x30\xfc\x30\x28\x75\xb7\x30\xa7\x30\xfc\x30\xc0\x30\xfc\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x00"/*@ L"RGBAMixS �t�B���^�[�p�V�F�[�_�[�̍쐬�Ɏ��s���܂���" @*/ )) ;
					return -1 ;
				}
				NS_SetDeleteHandleFlag( *PixelShaderHandle, PixelShaderHandle ) ;
			}
		}
	}
	else
	if( SelectR == SelectG && SelectR == SelectB && SelectR != DX_RGBA_SELECT_BLEND_A && SelectR != DX_RGBA_SELECT_SRC_A )
	{
		if( SelectR >= DX_RGBA_SELECT_BLEND_R && SelectR <= DX_RGBA_SELECT_BLEND_A )
		{
			SelectR -= DX_RGBA_SELECT_BLEND_R ;
			SelectG -= DX_RGBA_SELECT_BLEND_R ;
			SelectB -= DX_RGBA_SELECT_BLEND_R ;
			SelectA += DX_RGBA_SELECT_BLEND_R ;

			SrcBlendReverse = TRUE ;

			HandleTemp          = Info->BlendGrHandle ;
			Info->BlendGrHandle = Info->SrcGrHandle ;
			Info->SrcGrHandle   = HandleTemp ;
		}

		PixelShaderHandle = &GraphFilterShaderHandle.RgbaMixSRRRB[ SelectR ][ SelectA - DX_RGBA_SELECT_BLEND_R ] ;
		if( *PixelShaderHandle < 0 )
		{
			_SNPRINTF( FileName, sizeof( FileName ), "RGBAMix_S%sS%sS%sB%s.pso", CharTable[ SelectR ], CharTable[ SelectG ], CharTable[ SelectB ], CharTable[ SelectA - DX_RGBA_SELECT_BLEND_R ] ) ;
			*PixelShaderHandle = Direct3D11_MemLoadShaderCode( FileName, false ) ;
			if( *PixelShaderHandle < 0 )
			{
				ConvString( FileName, DX_CHARCODEFORMAT_ASCII, ( char * )PathUTF16LE, sizeof( PathUTF16LE ), DX_CHARCODEFORMAT_UTF16LE ) ;
				DXST_ERRORLOGFMT_ADDUTF16LE(( "\x52\x00\x47\x00\x42\x00\x41\x00\x4d\x00\x69\x00\x78\x00\x53\x00\x20\x00\xd5\x30\xa3\x30\xeb\x30\xbf\x30\xfc\x30\x28\x75\xb7\x30\xa7\x30\xfc\x30\xc0\x30\xfc\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x20\x00\x25\x00\x73\x00\x00"/*@ L"RGBAMixS �t�B���^�[�p�V�F�[�_�[�̍쐬�Ɏ��s���܂��� %s" @*/, PathUTF16LE )) ;
				return -1 ;
			}
			NS_SetDeleteHandleFlag( *PixelShaderHandle, PixelShaderHandle ) ;
		}
	}
	else
	if( ( SelectR == DX_RGBA_SELECT_SRC_R   && SelectG == DX_RGBA_SELECT_SRC_G   && SelectB == DX_RGBA_SELECT_SRC_B   ) ||
		( SelectR == DX_RGBA_SELECT_BLEND_R && SelectG == DX_RGBA_SELECT_BLEND_G && SelectB == DX_RGBA_SELECT_BLEND_B ) )
	{
		if( SelectR == DX_RGBA_SELECT_BLEND_R )
		{
			SelectR -= DX_RGBA_SELECT_BLEND_R ;
			SelectG -= DX_RGBA_SELECT_BLEND_R ;
			SelectB -= DX_RGBA_SELECT_BLEND_R ;
			SelectA += DX_RGBA_SELECT_BLEND_R ;

			SrcBlendReverse = TRUE ;

			HandleTemp          = Info->BlendGrHandle ;
			Info->BlendGrHandle = Info->SrcGrHandle ;
			Info->SrcGrHandle   = HandleTemp ;
		}

		PixelShaderHandle = &GraphFilterShaderHandle.RgbaMixSRGBB[ SelectA - DX_RGBA_SELECT_BLEND_R ] ;
		if( *PixelShaderHandle < 0 )
		{
			_SNPRINTF( FileName, sizeof( FileName ), "RGBAMix_S%sS%sS%sB%s.pso", CharTable[ SelectR ], CharTable[ SelectG ], CharTable[ SelectB ], CharTable[ SelectA - DX_RGBA_SELECT_BLEND_R ] ) ;
			*PixelShaderHandle = Direct3D11_MemLoadShaderCode( FileName, false ) ;
			if( *PixelShaderHandle < 0 )
			{
				ConvString( FileName, DX_CHARCODEFORMAT_ASCII, ( char * )PathUTF16LE, sizeof( PathUTF16LE ), DX_CHARCODEFORMAT_UTF16LE ) ;
				DXST_ERRORLOGFMT_ADDUTF16LE(( "\x52\x00\x47\x00\x42\x00\x41\x00\x4d\x00\x69\x00\x78\x00\x53\x00\x20\x00\xd5\x30\xa3\x30\xeb\x30\xbf\x30\xfc\x30\x28\x75\xb7\x30\xa7\x30\xfc\x30\xc0\x30\xfc\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x20\x00\x25\x00\x73\x00\x00"/*@ L"RGBAMixS �t�B���^�[�p�V�F�[�_�[�̍쐬�Ɏ��s���܂��� %s" @*/, PathUTF16LE )) ;
				return -1 ;
			}
			NS_SetDeleteHandleFlag( *PixelShaderHandle, PixelShaderHandle ) ;
		}
	}
	else
	{
		PixelShaderHandle = &GraphFilterShaderHandle.RgbaMixBasePS ;
		if( *PixelShaderHandle < 0 )
		{
			*PixelShaderHandle = Direct3D11_MemLoadShaderCode( PsoFileName[ 0 ], false ) ;
			if( *PixelShaderHandle < 0 )
			{
				ConvString( PsoFileName[ 0 ], DX_CHARCODEFORMAT_ASCII, ( char * )PathUTF16LE, sizeof( PathUTF16LE ), DX_CHARCODEFORMAT_UTF16LE ) ;
				DXST_ERRORLOGFMT_ADDUTF16LE(( "\xd5\x30\xa3\x30\xeb\x30\xbf\x30\xfc\x30\x28\x75\xb7\x30\xa7\x30\xfc\x30\xc0\x30\xfc\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x20\x00\x25\x00\x73\x00\x00"/*@ L"�t�B���^�[�p�V�F�[�_�[�̍쐬�Ɏ��s���܂��� %s" @*/, PathUTF16LE )) ;
				return -1 ;
			}
			NS_SetDeleteHandleFlag( *PixelShaderHandle, PixelShaderHandle ) ;
		}
	}

	ParamF4 = ( DX_D3D11_SHADER_FLOAT4 * )GD3D11.Device.Shader.Constant.ConstBuffer_PS_Filter->SysmemBuffer ;

	ParamF4[ 0 ][ 0 ] = ( float )SelectR + 0.5f ;
	ParamF4[ 0 ][ 1 ] = ( float )SelectG + 0.5f ;
	ParamF4[ 0 ][ 2 ] = ( float )SelectB + 0.5f ;
	ParamF4[ 0 ][ 3 ] = ( float )SelectA + 0.5f ;

	GD3D11.Device.Shader.Constant.ConstBuffer_PS_Filter->ChangeFlag = TRUE ;
	Graphics_D3D11_ConstantBuffer_Update( GD3D11.Device.Shader.Constant.ConstBuffer_PS_Filter ) ;

	Direct3D11_FilterStretchBlt( *PixelShaderHandle, Info, FALSE ) ;

	if( SrcBlendReverse )
	{
		HandleTemp          = Info->BlendGrHandle ;
		Info->BlendGrHandle = Info->SrcGrHandle ;
		Info->SrcGrHandle   = HandleTemp ;
	}

	// ����I��
	return 0 ;
}



extern int	GraphFilter_D3D11_RectBltBase_Timing0_PF( GRAPHFILTER_INFO *Info, GRAPHFILTER_PARAM *Param )
{
	Info->PassNum = -1 ;
	if( Info->IsBlend )
	{
		// �p�X�̐����擾
		Info->PassNum = 1 ;

		// ��Ɨp�e�N�X�`�����g�p���邩�ǂ������擾
		Info->UseWorkScreen = FALSE ;
	}
	else
	{
		// �K�E�X�t�B���^�݂̂Q�p�X
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

extern int	GraphFilter_D3D11_RectBltBase_Timing1_PF( void )
{
	// ����I��
	return 0 ;
}

extern int	GraphFilter_D3D11_DestGraphSetup_PF(      GRAPHFILTER_INFO *Info, int *UseSrcGrHandle, int *UseDestGrHandle )
{
	SHADOWMAPDATA *SrcShadowMap  = NULL ;
	SHADOWMAPDATA *DestShadowMap = NULL ;
	IMAGEDATA     *SrcImage      = NULL ;
	IMAGEDATA     *DestImage     = NULL ;
	IMAGEDATA     *TempSrcImage  = NULL ;
	int TexSizeW ;
	int TexSizeH ;
	int SrcTexFloatType ;
	int DestTexFloatType ;
	int SrcEqDestOrDestNonDrawValidHandle ;
	int AlwaysUseDestWorkHandle ;
	int UseDestWorkHandleIndex ;
	int UseSrcWorkHandleIndex ;
	int SrcGraphDivFlag ;

	if( !GRAPHCHK( Info->SrcGrHandle, SrcImage ) )
	{
		SrcTexFloatType = SrcImage->Orig->FormatDesc.FloatTypeFlag ;
	}
	else
	if( !SHADOWMAPCHK( Info->SrcGrHandle, SrcShadowMap ) )
	{
		SrcTexFloatType = SrcShadowMap->TexFormat_Float ;
	}
	else
	{
		return -1 ;
	}

	if( !GRAPHCHK( Info->DestGrHandle, DestImage ) )
	{
		DestTexFloatType = DestImage->Orig->FormatDesc.FloatTypeFlag ;
	}
	else
	if( !SHADOWMAPCHK( Info->DestGrHandle, DestShadowMap ) )
	{
		DestTexFloatType = DestShadowMap->TexFormat_Float ;
	}
	else
	{
		return -1 ;
	}

	UseSrcWorkHandleIndex = 0 ;

	if( SrcShadowMap == NULL &&
		( ( /*Info->UseWorkScreen == 1 &&*/ Info->SrcGrHandle == Info->DestGrHandle ) ||
		  DestImage->Orig->FormatDesc.DrawValidFlag == FALSE ) )
	{
		SrcEqDestOrDestNonDrawValidHandle = TRUE ;
	}
	else
	{
		SrcEqDestOrDestNonDrawValidHandle = FALSE ;
	}

	if( Info->UseWorkScreen == 1 &&
		( ( Info->Pass % 2 == 0 && Info->PassNum % 2 == 0 ) ||
		  ( Info->Pass % 2 == 1 && Info->PassNum % 2 == 1 ) ) )
	{
		AlwaysUseDestWorkHandle = TRUE ;
	}
	else
	{
		AlwaysUseDestWorkHandle = FALSE ;
	}

	if(
		SrcShadowMap == NULL &&
		Info->Pass == 0 &&
		(
		  SrcImage->Hard.DrawNum != 1 ||
		  SrcImage->Hard.Draw[ 0 ].UsePosXI != 0 ||
		  SrcImage->Hard.Draw[ 0 ].UsePosYI != 0
		)
	  )
	{
		SrcGraphDivFlag = TRUE ;
	}
	else
	{
		SrcGraphDivFlag = FALSE ;
	}

	if( SrcEqDestOrDestNonDrawValidHandle || AlwaysUseDestWorkHandle )
	{
		if( AlwaysUseDestWorkHandle == FALSE && SrcEqDestOrDestNonDrawValidHandle == TRUE )
		{
			UseDestWorkHandleIndex = 1 ;
			UseSrcWorkHandleIndex  = 0 ;
		}
		else
		{
			UseDestWorkHandleIndex = 0 ;
			UseSrcWorkHandleIndex  = 1 ;
		}

		if( SrcGraphDivFlag )
		{
			TexSizeW = SrcImage->WidthI ;
			TexSizeH = SrcImage->HeightI ;
		}
		else
		{
			NS_GetGraphTextureSize( Info->SrcGrHandle, &TexSizeW, &TexSizeH ) ;
		}

		*UseDestGrHandle = Direct3D11_GraphFilter_GetWorkTexture( DestTexFloatType, ( DWORD )TexSizeW, ( DWORD )TexSizeH, ( DWORD )UseDestWorkHandleIndex ) ;
		if( *UseDestGrHandle < 0 )
		{
			return -1 ;
		}
	}
	else
	{
		*UseDestGrHandle = Info->DestGrHandle ;
	}

	if( SrcGraphDivFlag )
	{
		RECT DestRect ;
		RECT SrcRect ;
		int i ;

		TexSizeW = SrcImage->WidthI ;
		TexSizeH = SrcImage->HeightI ;

		*UseSrcGrHandle = Direct3D11_GraphFilter_GetWorkTexture( SrcTexFloatType, ( DWORD )TexSizeW, ( DWORD )TexSizeH, ( DWORD )UseSrcWorkHandleIndex ) ;
		if( *UseSrcGrHandle < 0 )
		{
			return -1 ;
		}

		GRAPHCHK( *UseSrcGrHandle, TempSrcImage ) ;

		for( i = 0 ; i < SrcImage->Hard.DrawNum ; i ++ )
		{
			SrcRect.left    = SrcImage->Hard.Draw[ i ].UsePosXI ;
			SrcRect.top     = SrcImage->Hard.Draw[ i ].UsePosYI ;
			SrcRect.right   = SrcImage->Hard.Draw[ i ].UsePosXI  + SrcImage->Hard.Draw[ i ].WidthI ;
			SrcRect.bottom  = SrcImage->Hard.Draw[ i ].UsePosYI  + SrcImage->Hard.Draw[ i ].HeightI ;

			DestRect.left   = SrcImage->Hard.Draw[ i ].DrawPosXI ;
			DestRect.top    = SrcImage->Hard.Draw[ i ].DrawPosYI ;
			DestRect.right  = SrcImage->Hard.Draw[ i ].DrawPosXI + SrcImage->Hard.Draw[ i ].WidthI ;
			DestRect.bottom = SrcImage->Hard.Draw[ i ].DrawPosYI + SrcImage->Hard.Draw[ i ].HeightI ;

			Graphics_D3D11_StretchRect(
				SrcImage->Hard.Draw[ i ].Tex->PF->D3D11.Texture,     SrcImage->Hard.Draw[ i ].Tex->PF->D3D11.TextureSRV,          &SrcRect,
				TempSrcImage->Hard.Draw[ 0 ].Tex->PF->D3D11.Texture, TempSrcImage->Hard.Draw[ 0 ].Tex->PF->D3D11.TextureRTV[ 0 ], &DestRect, D_D3D11_FILTER_TYPE_POINT ) ;
		}
	}

	return 0 ;
}

extern int GraphFilter_D3D11_DestGraphUpdate_PF( GRAPHFILTER_INFO *Info, int UseDestGrHandle, int FilterResult )
{
	if( FilterResult >= 0 )
	{
		if( ( Info->UseWorkScreen == 1 && Info->SrcGrHandle == Info->DestGrHandle ) ||
			UseDestGrHandle != Info->DestGrHandle )
		{
			Graphics_Image_GetDrawScreenGraphBase(
				UseDestGrHandle,
				0,
				Info->DestX,
				Info->DestY,
				Info->DestX + ( Info->SrcX2 - Info->SrcX1 ) / Info->SrcDivNum,
				Info->DestY + ( Info->SrcY2 - Info->SrcY1 ) / Info->SrcDivNum,
				Info->DestX,
				Info->DestY,
				Info->DestGrHandle
			) ;
		}
	}

	// ����I��
	return 0 ;
}





#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

#endif // DX_NON_FILTER

#endif // DX_NON_DIRECT3D11

#endif // DX_NON_GRAPHICS
