//-----------------------------------------------------------------------------
// 
// 		�c�w���C�u����		�`�揈���v���O����( Direct3D9 )
// 
//  	Ver 3.16d
// 
//-----------------------------------------------------------------------------

// �c�w���C�u�����쐬���p��`
#define __DX_MAKE

#include "../DxCompileConfig.h"

#ifndef DX_NON_GRAPHICS

#ifndef DX_NON_DIRECT3D9

// �C���N���[�h ---------------------------------------------------------------
#include "DxGraphicsD3D9.h"
#include "DxGraphicsWin.h"
#include "DxMaskD3D9.h"
#include "DxWindow.h"
#include "DxWinAPI.h"
#include "DxShader_PS_D3D9.h"
#include "DxShader_VS_D3D9.h"
#include "../DxSystem.h"
#include "../DxLog.h"
#include "../DxModel.h"
#include "../DxMovie.h"
#include "../DxMask.h"
#include "../DxMath.h"
#include "../DxBaseFunc.h"
#include "../DxGraphics.h"
#include "../DxGraphicsFilter.h"
#include "../DxUseCLib.h"
#include "../DxInput.h"
#include "../DxThread.h"
#include "../DxASyncLoad.h"

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// �}�N����` -----------------------------------------------------------------

// ��Ǘ��e�N�X�`���ւ̃f�[�^�]���p�V�X�e���������z�u�e�N�X�`���̓����t�H�[�}�b�g�̍ő吔
#define DIRECT3D9_SYSMEMTEXTURE_ONE_FORMAT_NUM	(16)

// ��Ǘ��e�N�X�`���ւ̃f�[�^�]���p�V�X�e���������z�u�T�[�t�F�X�̓����t�H�[�}�b�g�̍ő吔
#define DIRECT3D9_SYSMEMSURFACE_ONE_FORMAT_NUM	(16)




#define DX_D3D9_DRAWPREP_NOTEX( FLAG )													\
	if( GD3D9.Device.DrawSetting.RenderTexture != NULL )								\
		Graphics_D3D9_DrawSetting_SetTexture( NULL ) ;									\
	if( GSYS.ChangeSettingFlag ||														\
		GD3D9.Device.DrawSetting.DrawPrepAlwaysFlag != FALSE ||							\
		GD3D9.Device.DrawSetting.DrawPrepParamFlag  != FLAG )							\
		Graphics_D3D9_DrawPreparation( FLAG ) ;											\
	/*if( GRH.UseAlphaChDrawShader )*/													\
	/*{*/																				\
	/*	AlphaChDraw_Pre( &GD3D9.Device.DrawInfo.DrawRect ) ;*/							\
	/*}*/


// Graphics_D3D9_DrawPreparation ���ĂԒ�^��
#define DX_D3D9_DRAWPREP_TEX( ORIG, TEX, FLAG )											\
	FLAG |= DX_D3D9_DRAWPREP_TEXTURE ;													\
	if( (ORIG)->FormatDesc.AlphaChFlag   ) FLAG |= DX_D3D9_DRAWPREP_TEXALPHACH ;		\
	if( (ORIG)->FormatDesc.AlphaTestFlag ) FLAG |= DX_D3D9_DRAWPREP_TEXALPHATEST ;		\
	if( GD3D9.Device.DrawSetting.RenderTexture != (TEX) )								\
		Graphics_D3D9_DrawSetting_SetTexture( (TEX) ) ;									\
	if( GSYS.ChangeSettingFlag ||														\
		GD3D9.Device.DrawSetting.DrawPrepAlwaysFlag != FALSE ||							\
		GD3D9.Device.DrawSetting.DrawPrepParamFlag  != FLAG )							\
		Graphics_D3D9_DrawPreparation( FLAG ) ;											\
	/*if( GRH.UseAlphaChDrawShader )*/													\
	/*{*/																				\
	/*	AlphaChDraw_Pre( &GD3D9.Device.DrawInfo.DrawRect ) ;*/							\
	/*}*/



#define RENDERVERTEX( NextUse3DVertex )	\
	{\
		if( GD3D9.Device.DrawInfo.VertexNum != 0 )\
		{\
			/*SETFVF(                     D3DDev_VertexType[ GD3D9.Device.DrawInfo.Use3DVertex ][ GD3D9.Device.DrawInfo.VertexType ] )*/\
			/*Graphics_D3D9_DeviceState_SetFVF( D3DDev_VertexType[ GD3D9.Device.DrawInfo.Use3DVertex ][ GD3D9.Device.DrawInfo.VertexType ] ) ;*/\
			Graphics_Hardware_D3D9_SetFVF_UseTable( D3DDev_VertexDeclaration[ GD3D9.Device.DrawInfo.Use3DVertex ][ GD3D9.Device.DrawInfo.VertexType ] ) ;\
			/*if( GD3D9.Device.DrawInfo.Use3DVertex )*/\
			/*{*/\
				/*Graphics_D3D9_DeviceState_SetWorldMatrix( &GlobalIdentMatrix ) ;*/\
			/*}*/\
			Direct3DDevice9_DrawPrimitiveUP(\
				GD3D9.Device.DrawInfo.PrimitiveType,\
				( UINT )GETPRIMNUM( GD3D9.Device.DrawInfo.PrimitiveType, GD3D9.Device.DrawInfo.VertexNum ),\
				GD3D9.Device.DrawInfo.VertexBufferAddr[ GD3D9.Device.DrawInfo.Use3DVertex ][ GD3D9.Device.DrawInfo.VertexType ],\
				D3DDev_VertexSize[ GD3D9.Device.DrawInfo.Use3DVertex ][ GD3D9.Device.DrawInfo.VertexType ] ) ;\
			/*if( GD3D9.Device.DrawInfo.Use3DVertex )*/\
			/*{*/\
				/*Graphics_D3D9_DeviceState_SetWorldMatrix( &GSYS.DrawSetting.WorldMatrixF ) ;*/\
			/*}*/\
			/*RenderVertexDrawPrimitive_PF() ;*/\
		}\
		GD3D9.Device.DrawInfo.VertexNum = 0 ;\
		GD3D9.Device.DrawInfo.VertexBufferNextAddr = GD3D9.Device.DrawInfo.VertexBufferAddr[ NextUse3DVertex ][ GD3D9.Device.DrawInfo.VertexType ] ;\
	}

#define GETVERTEX_QUAD( p )		\
	MV1DRAWPACKDRAWMODEL		\
	if(\
		  GD3D9.Device.DrawInfo.Use3DVertex != 0 ||\
		  GD3D9.Device.DrawInfo.PrimitiveType != D_D3DPT_TRIANGLELIST ||\
		  GD3D9.Device.DrawInfo.VertexNum + 6 >= D3DDev_VertexBuffer_MaxVertexNum[ GD3D9.Device.DrawInfo.Use3DVertex ][ GD3D9.Device.DrawInfo.VertexType ] /* ���{���� > �����ǁAGeForce 9800 GT �̕s��Ή��̂��� >= �ɂ��� */\
	  )\
	{\
		RENDERVERTEX( 0 )\
		GD3D9.Device.DrawInfo.Use3DVertex = 0 ;\
		GD3D9.Device.DrawInfo.PrimitiveType = D_D3DPT_TRIANGLELIST ;\
	}\
	*((DWORD_PTR *)&p) = (DWORD_PTR)GD3D9.Device.DrawInfo.VertexBufferNextAddr ;

#define GETVERTEX_BILLBOARD( p )		\
	MV1DRAWPACKDRAWMODEL		\
	if(\
		  GD3D9.Device.DrawInfo.Use3DVertex != 1 ||\
		  GD3D9.Device.DrawInfo.PrimitiveType != D_D3DPT_TRIANGLELIST ||\
		  GD3D9.Device.DrawInfo.VertexNum + 6 >= D3DDev_VertexBuffer_MaxVertexNum[ GD3D9.Device.DrawInfo.Use3DVertex ][ GD3D9.Device.DrawInfo.VertexType ] /* ���{���� > �����ǁAGeForce 9800 GT �̕s��Ή��̂��� >= �ɂ��� */\
	  )\
	{\
		RENDERVERTEX( 1 )\
		GD3D9.Device.DrawInfo.Use3DVertex = 1 ;\
		GD3D9.Device.DrawInfo.PrimitiveType = D_D3DPT_TRIANGLELIST ;\
	}\
	*((DWORD_PTR *)&p) = (DWORD_PTR)GD3D9.Device.DrawInfo.VertexBufferNextAddr ;

#define GETVERTEX_TRIANGLE( p )		\
	MV1DRAWPACKDRAWMODEL		\
	if(\
		  GD3D9.Device.DrawInfo.Use3DVertex != 0 ||\
		  GD3D9.Device.DrawInfo.PrimitiveType != D_D3DPT_TRIANGLELIST ||\
		  GD3D9.Device.DrawInfo.VertexNum + 3 >= D3DDev_VertexBuffer_MaxVertexNum[ GD3D9.Device.DrawInfo.Use3DVertex ][ GD3D9.Device.DrawInfo.VertexType ] /* ���{���� > �����ǁAGeForce 9800 GT �̕s��Ή��̂��� >= �ɂ��� */\
	  )\
	{\
		RENDERVERTEX( 0 )\
		GD3D9.Device.DrawInfo.Use3DVertex = 0 ;\
		GD3D9.Device.DrawInfo.PrimitiveType = D_D3DPT_TRIANGLELIST ;\
	}\
	*((DWORD_PTR *)&p) = (DWORD_PTR)GD3D9.Device.DrawInfo.VertexBufferNextAddr ;

#define GETVERTEX_TRIANGLE3D( p )		\
	MV1DRAWPACKDRAWMODEL		\
	if(\
		  GD3D9.Device.DrawInfo.Use3DVertex != 1 ||\
		  GD3D9.Device.DrawInfo.PrimitiveType != D_D3DPT_TRIANGLELIST ||\
		  GD3D9.Device.DrawInfo.VertexNum + 3 >= D3DDev_VertexBuffer_MaxVertexNum[ GD3D9.Device.DrawInfo.Use3DVertex ][ GD3D9.Device.DrawInfo.VertexType ] /* ���{���� > �����ǁAGeForce 9800 GT �̕s��Ή��̂��� >= �ɂ��� */\
	  )\
	{\
		RENDERVERTEX( 1 )\
		GD3D9.Device.DrawInfo.Use3DVertex = 1 ;\
		GD3D9.Device.DrawInfo.PrimitiveType = D_D3DPT_TRIANGLELIST ;\
	}\
	*((DWORD_PTR *)&p) = (DWORD_PTR)GD3D9.Device.DrawInfo.VertexBufferNextAddr ;

#define GETVERTEX_LINEBOX( p )		\
	MV1DRAWPACKDRAWMODEL		\
	if(\
		  GD3D9.Device.DrawInfo.Use3DVertex != 0 ||\
		  GD3D9.Device.DrawInfo.PrimitiveType != D_D3DPT_TRIANGLELIST ||\
		  GD3D9.Device.DrawInfo.VertexNum + 24 >= D3DDev_VertexBuffer_MaxVertexNum[ GD3D9.Device.DrawInfo.Use3DVertex ][ GD3D9.Device.DrawInfo.VertexType ] /* ���{���� > �����ǁAGeForce 9800 GT �̕s��Ή��̂��� >= �ɂ��� */\
	  )\
	{\
		RENDERVERTEX( 0 )\
		GD3D9.Device.DrawInfo.Use3DVertex = 0 ;\
		GD3D9.Device.DrawInfo.PrimitiveType = D_D3DPT_TRIANGLELIST ;\
	}\
	*((DWORD_PTR *)&p) = (DWORD_PTR)GD3D9.Device.DrawInfo.VertexBufferNextAddr ;

#define GETVERTEX_LINETRIANGLE( p )		\
	MV1DRAWPACKDRAWMODEL		\
	if(\
		  GD3D9.Device.DrawInfo.Use3DVertex != 0 ||\
		  GD3D9.Device.DrawInfo.PrimitiveType != D_D3DPT_LINELIST ||\
		  GD3D9.Device.DrawInfo.VertexNum + 6 >= D3DDev_VertexBuffer_MaxVertexNum[ GD3D9.Device.DrawInfo.Use3DVertex ][ GD3D9.Device.DrawInfo.VertexType ] /* ���{���� > �����ǁAGeForce 9800 GT �̕s��Ή��̂��� >= �ɂ��� */\
	  )\
	{\
		RENDERVERTEX( 0 )\
		GD3D9.Device.DrawInfo.Use3DVertex = 0 ;\
		GD3D9.Device.DrawInfo.PrimitiveType = D_D3DPT_LINELIST ;\
	}\
	*((DWORD_PTR *)&p) = (DWORD_PTR)GD3D9.Device.DrawInfo.VertexBufferNextAddr ;

#define GETVERTEX_LINETRIANGLE3D( p )		\
	MV1DRAWPACKDRAWMODEL		\
	if(\
		  GD3D9.Device.DrawInfo.Use3DVertex != 1 ||\
		  GD3D9.Device.DrawInfo.PrimitiveType != D_D3DPT_LINELIST ||\
		  GD3D9.Device.DrawInfo.VertexNum + 6 >= D3DDev_VertexBuffer_MaxVertexNum[ GD3D9.Device.DrawInfo.Use3DVertex ][ GD3D9.Device.DrawInfo.VertexType ] /* ���{���� > �����ǁAGeForce 9800 GT �̕s��Ή��̂��� >= �ɂ��� */\
	  )\
	{\
		RENDERVERTEX( 1 )\
		GD3D9.Device.DrawInfo.Use3DVertex = 1 ;\
		GD3D9.Device.DrawInfo.PrimitiveType = D_D3DPT_LINELIST ;\
	}\
	*((DWORD_PTR *)&p) = (DWORD_PTR)GD3D9.Device.DrawInfo.VertexBufferNextAddr ;

#define GETVERTEX_LINE( p )		\
	MV1DRAWPACKDRAWMODEL		\
	if(\
		  GD3D9.Device.DrawInfo.Use3DVertex == 1 ||\
		  GD3D9.Device.DrawInfo.PrimitiveType != D_D3DPT_LINELIST ||\
		  GD3D9.Device.DrawInfo.VertexNum + 2 >= D3DDev_VertexBuffer_MaxVertexNum[ GD3D9.Device.DrawInfo.Use3DVertex ][ GD3D9.Device.DrawInfo.VertexType ] /* ���{���� > �����ǁAGeForce 9800 GT �̕s��Ή��̂��� >= �ɂ��� */\
	  )\
	{\
		RENDERVERTEX( 0 )\
		GD3D9.Device.DrawInfo.Use3DVertex = 0 ;\
		GD3D9.Device.DrawInfo.PrimitiveType = D_D3DPT_LINELIST ;\
	}\
	*((DWORD_PTR *)&p) = (DWORD_PTR)GD3D9.Device.DrawInfo.VertexBufferNextAddr ;


#define GETVERTEX_LINE3D( p )		\
	MV1DRAWPACKDRAWMODEL		\
	if(\
		  GD3D9.Device.DrawInfo.Use3DVertex != 1 ||\
		  GD3D9.Device.DrawInfo.PrimitiveType != D_D3DPT_LINELIST ||\
		  GD3D9.Device.DrawInfo.VertexNum + 2 >= D3DDev_VertexBuffer_MaxVertexNum[ GD3D9.Device.DrawInfo.Use3DVertex ][ GD3D9.Device.DrawInfo.VertexType ] /* ���{���� > �����ǁAGeForce 9800 GT �̕s��Ή��̂��� >= �ɂ��� */\
	  )\
	{\
		RENDERVERTEX( 1 )\
		GD3D9.Device.DrawInfo.Use3DVertex = 1 ;\
		GD3D9.Device.DrawInfo.PrimitiveType = D_D3DPT_LINELIST ;\
	}\
	*((DWORD_PTR *)&p) = (DWORD_PTR)GD3D9.Device.DrawInfo.VertexBufferNextAddr ;


#define GETVERTEX_POINT( p )		\
	MV1DRAWPACKDRAWMODEL		\
	if(\
		  GD3D9.Device.DrawInfo.Use3DVertex != 0 ||\
		  GD3D9.Device.DrawInfo.PrimitiveType != D_D3DPT_POINTLIST ||\
		  GD3D9.Device.DrawInfo.VertexNum + 1 >= D3DDev_VertexBuffer_MaxVertexNum[ GD3D9.Device.DrawInfo.Use3DVertex ][ GD3D9.Device.DrawInfo.VertexType ] /* ���{���� > �����ǁAGeForce 9800 GT �̕s��Ή��̂��� >= �ɂ��� */\
	  )\
	{\
		RENDERVERTEX( 0 )\
		GD3D9.Device.DrawInfo.Use3DVertex = 0 ;\
		GD3D9.Device.DrawInfo.PrimitiveType = D_D3DPT_POINTLIST ;\
	}\
	*((DWORD_PTR *)&p) = (DWORD_PTR)GD3D9.Device.DrawInfo.VertexBufferNextAddr ;

#define GETVERTEX_POINT3D( p )		\
	MV1DRAWPACKDRAWMODEL		\
	if(\
		  GD3D9.Device.DrawInfo.Use3DVertex != 1 ||\
		  GD3D9.Device.DrawInfo.PrimitiveType != D_D3DPT_POINTLIST ||\
		  GD3D9.Device.DrawInfo.VertexNum + 1 >= D3DDev_VertexBuffer_MaxVertexNum[ GD3D9.Device.DrawInfo.Use3DVertex ][ GD3D9.Device.DrawInfo.VertexType ] /* ���{���� > �����ǁAGeForce 9800 GT �̕s��Ή��̂��� >= �ɂ��� */\
	  )\
	{\
		RENDERVERTEX( 1 )\
		GD3D9.Device.DrawInfo.Use3DVertex = 1 ;\
		GD3D9.Device.DrawInfo.PrimitiveType = D_D3DPT_POINTLIST ;\
	}\
	*((DWORD_PTR *)&p) = (DWORD_PTR)GD3D9.Device.DrawInfo.VertexBufferNextAddr ;

#define ADD4VERTEX_POINT		\
		GD3D9.Device.DrawInfo.VertexBufferNextAddr += sizeof( VERTEX_NOTEX_2D ) ;	\
		GD3D9.Device.DrawInfo.VertexNum ++ ;

#define ADD4VERTEX_POINT3D		\
		GD3D9.Device.DrawInfo.VertexBufferNextAddr += sizeof( VERTEX_NOTEX_3D ) ;	\
		GD3D9.Device.DrawInfo.VertexNum ++ ;
/*
#define ADD4VERTEX_LINEBOX			\
		GD3D9.Device.DrawInfo.VertexBufferNextAddr += sizeof( VERTEX_NOTEX_2D ) * 8 ;	\
		GD3D9.Device.DrawInfo.VertexNum += 8 ;
*/
#define ADD4VERTEX_LINEBOX			\
		GD3D9.Device.DrawInfo.VertexBufferNextAddr += sizeof( VERTEX_NOTEX_2D ) * 24 ;	\
		GD3D9.Device.DrawInfo.VertexNum += 24 ;

#define ADD4VERTEX_LINETRIANGLE		\
		GD3D9.Device.DrawInfo.VertexBufferNextAddr += sizeof( VERTEX_NOTEX_2D ) * 6 ;	\
		GD3D9.Device.DrawInfo.VertexNum += 6 ;

#define ADD4VERTEX_LINETRIANGLE3D		\
		GD3D9.Device.DrawInfo.VertexBufferNextAddr += sizeof( VERTEX_NOTEX_3D ) * 6 ;	\
		GD3D9.Device.DrawInfo.VertexNum += 6 ;

#define ADD4VERTEX_LINE			\
		GD3D9.Device.DrawInfo.VertexBufferNextAddr += sizeof( VERTEX_NOTEX_2D ) * 2 ;	\
		GD3D9.Device.DrawInfo.VertexNum += 2 ;

#define ADD4VERTEX_LINE3D		\
		GD3D9.Device.DrawInfo.VertexBufferNextAddr += sizeof( VERTEX_NOTEX_3D ) * 2 ;	\
		GD3D9.Device.DrawInfo.VertexNum += 2 ;

#define ADD4VERTEX_NOTEX		\
		GD3D9.Device.DrawInfo.VertexBufferNextAddr += sizeof( VERTEX_NOTEX_2D ) * 6 ;	\
		GD3D9.Device.DrawInfo.VertexNum += 6 ;

#define ADD4VERTEX_TEX			\
		GD3D9.Device.DrawInfo.VertexBufferNextAddr += sizeof( VERTEX_2D ) * 6 ;	\
		GD3D9.Device.DrawInfo.VertexNum += 6 ;

#define ADD4VERTEX_BLENDTEX		\
		GD3D9.Device.DrawInfo.VertexBufferNextAddr += sizeof( VERTEX_BLENDTEX_2D ) * 6 ;	\
		GD3D9.Device.DrawInfo.VertexNum += 6 ;

#define ADD3VERTEX_NOTEX		\
		GD3D9.Device.DrawInfo.VertexBufferNextAddr += sizeof( VERTEX_NOTEX_2D ) * 3 ;	\
		GD3D9.Device.DrawInfo.VertexNum += 3 ;

#define ADD3VERTEX_TEX			\
		GD3D9.Device.DrawInfo.VertexBufferNextAddr += sizeof( VERTEX_2D ) * 3 ;	\
		GD3D9.Device.DrawInfo.VertexNum += 3 ;

#define ADD3VERTEX_BLENDTEX		\
		GD3D9.Device.DrawInfo.VertexBufferNextAddr += sizeof( VERTEX_BLENDTEX_2D ) * 3 ;	\
		GD3D9.Device.DrawInfo.VertexNum += 3 ;

#define ADD3VERTEX_NOTEX3D		\
		GD3D9.Device.DrawInfo.VertexBufferNextAddr += sizeof( VERTEX_NOTEX_3D ) * 3 ;	\
		GD3D9.Device.DrawInfo.VertexNum += 3 ;

#define ADD4VERTEX_BILLBOARD		\
		GD3D9.Device.DrawInfo.VertexBufferNextAddr += sizeof( VERTEX_3D ) * 6 ;	\
		GD3D9.Device.DrawInfo.VertexNum += 6 ;

#define SETUPCOLOR( DEST )		\
	DEST  = ( ( ( ( ( 	GD3D9.Device.DrawInfo.DiffuseColor & 0x00ff0000 ) >> 16 ) * Red   ) / 255 ) << 16 ) |\
			( ( ( ( ( 	GD3D9.Device.DrawInfo.DiffuseColor & 0x0000ff00 ) >> 8  ) * Green ) / 255 ) << 8  ) |\
			( ( ( ( (	GD3D9.Device.DrawInfo.DiffuseColor & 0x000000ff )       ) * Blue  ) / 255 )       ) |\
			( 			GD3D9.Device.DrawInfo.DiffuseColor & 0xff000000 ) ;


// �f�o�C�X����̍ő�E�ŏ��̃e�N�X�`���T�C�Y���擾�ł��Ȃ��������ɐݒ肷��T�C�Y
#define DEFAULT_MAXTEXTURE_SIZE		(256)
#define DEFAULT_MINTEXTURE_SIZE		(8)

// RGB�l���P�x�ő��RGB�l�ɕϊ����邽�߂̃{�����[���e�N�X�`���̃T�C�Y
#define RGBTOVMAXRGB_TEX_SIZE		(128)

// �\���̐錾 -----------------------------------------------------------------

// �f�[�^��` -----------------------------------------------------------------

// ���ˑ��e�N�X�`���t�H�[�}�b�g�� D3DFORMAT �ɕϊ����邽�߂̃e�[�u��
D_D3DFORMAT g_DXD3D9FMT_to_D3DFORMAT[] =
{
	D_D3DFMT_UNKNOWN,
	D_D3DFMT_R8G8B8,			// DX_TEXTUREFORMAT_DIRECT3D9_R8G8B8				(1)
	D_D3DFMT_A8R8G8B8,			// DX_TEXTUREFORMAT_DIRECT3D9_A8R8G8B8				(2)
	D_D3DFMT_X8R8G8B8,			// DX_TEXTUREFORMAT_DIRECT3D9_X8R8G8B8				(3)
	D_D3DFMT_R5G6B5,			// DX_TEXTUREFORMAT_DIRECT3D9_R5G6B5				(4)
	D_D3DFMT_X1R5G5B5,			// DX_TEXTUREFORMAT_DIRECT3D9_X1R5G5B5				(5)
	D_D3DFMT_A1R5G5B5,			// DX_TEXTUREFORMAT_DIRECT3D9_A1R5G5B5				(6)
	D_D3DFMT_A4R4G4B4,			// DX_TEXTUREFORMAT_DIRECT3D9_A4R4G4B4				(7)
	D_D3DFMT_R3G3B2,			// DX_TEXTUREFORMAT_DIRECT3D9_R3G3B2				(8)
	D_D3DFMT_A8R3G3B2,			// DX_TEXTUREFORMAT_DIRECT3D9_A8R3G3B2				(9)
	D_D3DFMT_X4R4G4B4,			// DX_TEXTUREFORMAT_DIRECT3D9_X4R4G4B4				(10)
	D_D3DFMT_A2B10G10R10,		// DX_TEXTUREFORMAT_DIRECT3D9_A2B10G10R10			(11)
	D_D3DFMT_G16R16,			// DX_TEXTUREFORMAT_DIRECT3D9_G16R16				(12)
	D_D3DFMT_A8B8G8R8,			// DX_TEXTUREFORMAT_DIRECT3D9_A8B8G8R8				(13)
	D_D3DFMT_X8B8G8R8,			// DX_TEXTUREFORMAT_DIRECT3D9_X8B8G8R8				(14)
	D_D3DFMT_A2R10G10B10,		// DX_TEXTUREFORMAT_DIRECT3D9_A2R10G10B10			(15)
	D_D3DFMT_A16B16G16R16,		// DX_TEXTUREFORMAT_DIRECT3D9_A16B16G16R16			(16)
	D_D3DFMT_R16F,				// DX_TEXTUREFORMAT_DIRECT3D9_R16F					(17)
	D_D3DFMT_G16R16F,			// DX_TEXTUREFORMAT_DIRECT3D9_G16R16F				(18)
	D_D3DFMT_A16B16G16R16F,		// DX_TEXTUREFORMAT_DIRECT3D9_A16B16G16R16F			(19)
	D_D3DFMT_R32F,				// DX_TEXTUREFORMAT_DIRECT3D9_R32F					(20)
	D_D3DFMT_G32R32F,			// DX_TEXTUREFORMAT_DIRECT3D9_G32R32F				(21)
	D_D3DFMT_A32B32G32R32F,		// DX_TEXTUREFORMAT_DIRECT3D9_A32B32G32R32F			(22)
} ;

// ���_�o�b�t�@�Ɋi�[�ł��钸�_�̍ő吔�̃e�[�u��[ 0:�Q�c���_  1:�R�c���_ ]
static const int D3DDev_VertexBuffer_MaxVertexNum[ 2 ][ VERTEXTYPE_NUM ] =
{
	D3DDEV_NOTEX_VERTMAXNUM,
	D3DDEV_TEX_VERTMAXNUM, 
	D3DDEV_BLENDTEX_VERTMAXNUM,

	D3DDEV_NOTEX_3D_VERTMAXNUM,
	D3DDEV_TEX_3D_VERTMAXNUM, 
} ;

// �P�ʍs��
static MATRIX GlobalIdentMatrix =
{
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f
} ;

// �e���_�^�C�v��FVF�e�[�u��
static const DWORD D3DDev_VertexFVF[ VERTEXFVF_DECL_NUM ] =
{
	VERTEXFVF_2D_USER,
	VERTEXFVF_NOTEX_2D,
	VERTEXFVF_2D,
	VERTEXFVF_BLENDTEX_2D,
	VERTEXFVF_SHADER_2D,
	VERTEXFVF_NOTEX_3D,
	VERTEXFVF_3D,
	VERTEXFVF_3D_LIGHT,
	VERTEXFVF_SHADER_3D,
	VERTEXFVF_TEX8_2D,
} ;

// �e���_�錾���R�c���ǂ����̃e�[�u��
static const int D3DDev_VertexDeclaration_Is3D[ VERTEXFVF_DECL_NUM ] =
{
	0,		// VERTEXFVF_DECL_2D_USER
	0,		// VERTEXFVF_DECL_NOTEX_2D
	0,		// VERTEXFVF_DECL_2D
	0,		// VERTEXFVF_DECL_BLENDTEX_2D
	0,		// VERTEXFVF_DECL_SHADER_2D
	1,		// VERTEXFVF_DECL_NOTEX_3D
	1,		// VERTEXFVF_DECL_3D
	1,		// VERTEXFVF_DECL_3D_LIGHT
	1,		// VERTEXFVF_DECL_SHADER_3D
	0,		// VERTEXFVF_DECL_TEX8_2D
} ;

// ���_�\���̒�`�̃e�[�u��[ 0:�Q�c���_  1:�R�c���_ ]
static const DWORD D3DDev_VertexType[ 2 ][ VERTEXTYPE_NUM ] =
{
	VERTEXFVF_NOTEX_2D,
	VERTEXFVF_2D,
	VERTEXFVF_BLENDTEX_2D,

	VERTEXFVF_NOTEX_3D,
	VERTEXFVF_3D
} ;

// ���_�錾�̒�`�̃e�[�u��[ 0:�Q�c���_  1:�R�c���_ ]
static const int D3DDev_VertexDeclaration[ 2 ][ VERTEXTYPE_NUM ] =
{
	VERTEXFVF_DECL_NOTEX_2D,
	VERTEXFVF_DECL_2D,
	VERTEXFVF_DECL_BLENDTEX_2D,

	VERTEXFVF_DECL_NOTEX_3D,
	VERTEXFVF_DECL_3D
} ;

// ���_�̃T�C�Y�̃e�[�u��[ 0:�Q�c���_  1:�R�c���_ ]
static const UINT D3DDev_VertexSize[ 2 ][ VERTEXTYPE_NUM ] =
{
	sizeof( VERTEX_NOTEX_2D ),
	sizeof( VERTEX_2D ),
	sizeof( VERTEX_BLENDTEX_2D ),

	sizeof( VERTEX_NOTEX_3D ),
	sizeof( VERTEX_3D ),
} ;

// �V�F�[�_�[�o�C�i���z��
extern int  DxShaderCodeBin_ModelConvert ;
extern BYTE DxShaderCodeBin_Model[] ;

extern int  DxShaderCodeBin_FilterConvert ;
extern BYTE DxShaderCodeBin_Filter[] ;

extern int  DxShaderCodeBin_BaseConvert ;
extern BYTE DxShaderCodeBin_Base[] ;

extern int  DxShaderCodeBin_RgbaMixConvert ;
extern BYTE DxShaderCodeBin_RgbaMix[] ;

// Direct3D9 ���g�����O���t�B�b�N�X�������̍\����
GRAPHICS_HARDDATA_DIRECT3D9 GraphicsHardDataDirect3D9 ;

// �֐��錾 -------------------------------------------------------------------

static int EnumDirect3DAdapter() ;												// Direct3D �̃A�_�v�^����񋓂���

static  D_D3DVERTEXELEMENT9	MakeVertexElement9( BYTE Stream, BYTE Offset, BYTE Type, BYTE Method, BYTE Usage, BYTE UsageIndex ) ;

static	int		SetPlaneVertexHardware( VERTEX_2D *GraphVert, int xnum, int ynum ) ;		// �Q�����z��I�ɔz�u���ꂽ���_�f�[�^�𒸓_�o�b�t�@�ɒǉ�����

// Direct3D9 �ɐݒ肷��ˉe�s����X�V����
static void Graphics_Hardware_D3D9_RefreshProjectionMatrix( int Update3DProjection, int Update2DProjection, int AlwaysUpdate ) ;

// �Œ�@�\�p�C�v���C���݊������p�̒��_�錾�ƒ��_�V�F�[�_�[�̐ݒ���s��
static void Graphics_Hardware_D3D9_SetFVF_UseTable( int FVFDecl /* VERTEXFVF_DECL_2D_USER�� */ ) ;

#ifndef DX_NON_MODEL
static	int		CreateRGBtoVMaxRGBVolumeTexture() ;								// RGB�J���[���P�x���ő�ɂ���RGB�l�ɕϊ����邽�߂̃{�����[���e�N�X�`�����쐬����( 0:����  -1:���s )
#endif // DX_NON_MODEL

static int Graphics_D3D9_BltBmpOrBaseImageToGraph3_MipMapBlt(
	      IMAGEDATA_ORIG			*Orig,
	const RECT						*SrcRect,
	const RECT						*DestRect,
	      int						UseSysMemSurface,
	      D_IDirect3DBaseTexture9	*UseTex,
	      int						TexWidth,
	      int						TexHeight,
	const BASEIMAGE					*RgbBaseImage,
	const BASEIMAGE					*AlphaBaseImage,
	const COLORDATA					*DestColor,
	      int						RedIsAlphaFlag,
	      int						UseTransColorConvAlpha,
	      int						ASyncThread = FALSE
) ;

static int Graphics_D3D9_BltBmpOrBaseImageToGraph3_NoMipMapBlt(
	      IMAGEDATA_ORIG			*Orig,
	const RECT						*SrcRect,
	const RECT						*DestRect,
	      int						UseSysMemSurface,
	      D_IDirect3DBaseTexture9	*UseTex,
	      int						TexWidth,
	      int						TexHeight,
	      int						TexUseWidth,
	      int						TexUseHeight,
	const BASEIMAGE					*RgbBaseImage,
	const BASEIMAGE					*AlphaBaseImage,
	const COLORDATA					*DestColor,
	      int						RedIsAlphaFlag,
	      int						UseTransColorConvAlpha,
	      int						ASyncThread = FALSE
) ;

__inline	static DWORD GetDiffuseColor( void )															// ���݂̃f�B�t���[�Y�J���[�𓾂�
			{
				return  ( ( DWORD )GSYS.DrawSetting.DrawBright.Red   << 16 ) |
						( ( DWORD )GSYS.DrawSetting.DrawBright.Green << 8  ) |
						( ( DWORD )GSYS.DrawSetting.DrawBright.Blue        ) |
						( ( ( GSYS.DrawSetting.BlendMode != DX_BLENDMODE_NOBLEND || GSYS.DrawSetting.UseNoBlendModeParam ) ? ( DWORD )GSYS.DrawSetting.BlendParam : 255 ) << 24 ) ;
			}

// �v���O���� -----------------------------------------------------------------

// Direct3D9 �̏������֌W

#ifndef DX_NON_MODEL

__inline void RGBtoVMaxRGBI( int R, int G, int B, BYTE &RD, BYTE &GD, BYTE &BD )
{
	int h, s ;

	if( R + G + B == 0 )
	{
		RD = 255 ;
		GD = 255 ;
		BD = 255 ;
		return ;
	}

	if( R > 255 ) R = 255 ;
	if( G > 255 ) G = 255 ;
	if( B > 255 ) B = 255 ;

	if( R > G )
	{
		if( R > B )
		{
			if( G > B )
			{
				// R > G > B
				if( R == B )
				{
					RD = 255 ;
					GD = 255 ;
					BD = 255 ;
				}
				else
				{
					h = ( G - B ) * 255 / ( R - B ) ;
					s = ( R - B ) * 255 / R ;

					RD = 255 ;
					GD = ( BYTE )( 255 - ( 255 - h ) * s / 255 ) ;
					BD = ( BYTE )( 255 - s ) ;
				}
			}
			else
			{
				// R > B > G
				if( R == G )
				{
					RD = 255 ;
					GD = 255 ;
					BD = 255 ;
				}
				else
				{
					h = ( B - G ) * 255 / ( R - G ) ;
					s = ( R - G ) * 255 / R ;

					RD = 255 ;
					GD = ( BYTE )( 255 - s ) ;
					BD = ( BYTE )( 255 - ( 255 - h ) * s / 255 ) ;
				}
			}
		}
		else
		{
			// B > R > G
			if( B == G )
			{
				RD = 255 ;
				GD = 255 ;
				BD = 255 ;
			}
			else
			{
				h = ( R - G ) * 255 / ( B - G ) ;
				s = ( B - G ) * 255 / B ;

				RD = ( BYTE )( 255 - ( 255 - h ) * s / 255 ) ;
				GD = ( BYTE )( 255 - s ) ;
				BD = 255 ;
			}
		}
	}
	else
	{
		if( G > B )
		{
			if( R > B )
			{
				// G > R > B
				if( G == B )
				{
					RD = 255 ;
					GD = 255 ;
					BD = 255 ;
				}
				else
				{
					h = ( R - B ) * 255 / ( G - B ) ;
					s = ( G - B ) * 255 / G ;

					RD = ( BYTE )( 255 - ( 255 - h ) * s / 255 ) ;
					GD = 255 ;
					BD = ( BYTE )( 255 - s ) ;
				}
			}
			else
			{
				// G > B > R
				if( G == R )
				{
					RD = 255 ;
					GD = 255 ;
					BD = 255 ;
				}
				else
				{
					h = ( B - R ) * 255 / ( G - R ) ;
					s = ( G - R ) * 255 / G ;

					RD = ( BYTE )( 255 - s ) ;
					GD = 255 ;
					BD = ( BYTE )( 255 - ( 255 - h ) * s / 255 ) ;
				}
			}
		}
		else
		{
			// B > G > R
			if( B == R )
			{
				RD = 255 ;
				GD = 255 ;
				BD = 255 ;
			}
			else
			{
				h = ( G - R ) * 255 / ( B - R ) ;
				s = ( B - R ) * 255 / B ;

				RD = ( BYTE )( 255 - s ) ;
				GD = ( BYTE )( 255 - ( 255 - h ) * s / 255 ) ;
				BD = 255 ;
			}
		}
	}
}

// RGB�J���[���P�x���ő�ɂ���RGB�l�ɕϊ����邽�߂̃{�����[���e�N�X�`�����쐬����( 0:����  -1:���s )
static int CreateRGBtoVMaxRGBVolumeTexture()
{
	D_IDirect3DVolumeTexture9 *TempMemTexture9 ;
	D_D3DLOCKED_BOX LockInfo ;
	D_D3DBOX LockBox ;
	int r, g, b ;
	BYTE *pDest ;
	HRESULT hr ;

	if( GAPIWin.Direct3DDevice9Object == NULL ) return -1 ;

	GD3D9.RGBtoVMaxRGBVolumeTexture = NULL ;
	TempMemTexture9 = NULL ;

	// �{�����[���e�N�X�`���̍쐬
	hr = Direct3DDevice9_CreateVolumeTexture(
		RGBTOVMAXRGB_TEX_SIZE,
		RGBTOVMAXRGB_TEX_SIZE,
		RGBTOVMAXRGB_TEX_SIZE,
		1,
		0,
		D_D3DFMT_X8R8G8B8,
		D_D3DPOOL_DEFAULT,
		&GD3D9.RGBtoVMaxRGBVolumeTexture,
		NULL ) ;
	if( hr != D_D3D_OK )
	{
		DXST_ERRORLOG_ADDUTF16LE( "\x52\x00\x47\x00\x42\x00\xab\x30\xe9\x30\xfc\x30\x92\x30\x1d\x8f\xa6\x5e\x92\x30\x00\x67\x27\x59\x6b\x30\x57\x30\x5f\x30\x52\x00\x47\x00\x42\x00\x24\x50\x6b\x30\x09\x59\xdb\x63\x59\x30\x8b\x30\x5f\x30\x81\x30\x6e\x30\xdc\x30\xea\x30\xe5\x30\xfc\x30\xe0\x30\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"RGB�J���[���P�x���ő�ɂ���RGB�l�ɕϊ����邽�߂̃{�����[���e�N�X�`���̍쐬�Ɏ��s���܂���\n" @*/ ) ;
		goto ERR ;
	}

	// �]���p�V�X�e���������{�����[���e�N�X�`���̍쐬
	if( Direct3DDevice9_CreateVolumeTexture(
		RGBTOVMAXRGB_TEX_SIZE,
		RGBTOVMAXRGB_TEX_SIZE,
		RGBTOVMAXRGB_TEX_SIZE,
		1,
		0,
		D_D3DFMT_X8R8G8B8,
		D_D3DPOOL_SYSTEMMEM,
		&TempMemTexture9,
		NULL ) != D_D3D_OK )
	{
		DXST_ERRORLOG_ADDUTF16LE( "\x52\x00\x47\x00\x42\x00\xab\x30\xe9\x30\xfc\x30\x92\x30\x1d\x8f\xa6\x5e\x92\x30\x00\x67\x27\x59\x6b\x30\x57\x30\x5f\x30\x52\x00\x47\x00\x42\x00\x24\x50\x6b\x30\x09\x59\xdb\x63\x59\x30\x8b\x30\x5f\x30\x81\x30\x6e\x30\xe2\x8e\x01\x90\x43\x51\x28\x75\xdc\x30\xea\x30\xe5\x30\xfc\x30\xe0\x30\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"RGB�J���[���P�x���ő�ɂ���RGB�l�ɕϊ����邽�߂̓]�����p�{�����[���e�N�X�`���̍쐬�Ɏ��s���܂���\n" @*/ ) ;
		goto ERR ;
	}

	// �V�X�e���������{�����[���e�N�X�`���̃��b�N
	_MEMSET( &LockInfo, 0, sizeof( LockInfo ) ) ;
	LockBox.Left   = 0 ;
	LockBox.Top    = 0 ;
	LockBox.Front  = 0 ;
	LockBox.Right  = RGBTOVMAXRGB_TEX_SIZE ;
	LockBox.Bottom = RGBTOVMAXRGB_TEX_SIZE ;
	LockBox.Back   = RGBTOVMAXRGB_TEX_SIZE ;
	hr = TempMemTexture9->LockBox( 0, &LockInfo, &LockBox, 0 ) ;
	if( hr == D_D3D_OK )
	{
		// �V�X�e���������{�����[���e�N�X�`���ɕϊ�������������
		for( b = 0 ; b < RGBTOVMAXRGB_TEX_SIZE ; b ++ )
		{
			for( g = 0 ; g < RGBTOVMAXRGB_TEX_SIZE ; g ++ )
			{
				pDest = ( BYTE * )LockInfo.pBits + LockInfo.SlicePitch * b + LockInfo.RowPitch * g ;
				for( r = 0 ; r < RGBTOVMAXRGB_TEX_SIZE ; r ++, pDest += 4 )
				{
					RGBtoVMaxRGBI( r * 2, g * 2, b * 2, pDest[ 2 ], pDest[ 1 ], pDest[ 0 ] ) ;
					if( r < 8 && g < 8 && b < 8 )
					{
						pDest[ 0 ] = 255 ;
						pDest[ 1 ] = 255 ;
						pDest[ 2 ] = 255 ;
					}
					pDest[ 3 ] = 255 ;
				}
			}
		}
	}
	else
	{
		DXST_ERRORLOG_ADDUTF16LE( "\x52\x00\x47\x00\x42\x00\xab\x30\xe9\x30\xfc\x30\x92\x30\x1d\x8f\xa6\x5e\x92\x30\x00\x67\x27\x59\x6b\x30\x57\x30\x5f\x30\x52\x00\x47\x00\x42\x00\x24\x50\x6b\x30\x09\x59\xdb\x63\x59\x30\x8b\x30\x5f\x30\x81\x30\x6e\x30\xe2\x8e\x01\x90\x43\x51\x28\x75\xdc\x30\xea\x30\xe5\x30\xfc\x30\xe0\x30\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\x6e\x30\xed\x30\xc3\x30\xaf\x30\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"RGB�J���[���P�x���ő�ɂ���RGB�l�ɕϊ����邽�߂̓]�����p�{�����[���e�N�X�`���̃��b�N�Ɏ��s���܂���\n" @*/ ) ;
		goto ERR ;
	}

	// ���b�N�̉���
	TempMemTexture9->UnlockBox( 0 ) ;

	// �e�N�X�`���̓]��
	hr = Direct3DDevice9_UpdateTexture( TempMemTexture9, GD3D9.RGBtoVMaxRGBVolumeTexture ) ;

	// �V�X�e���������{�����[���e�N�X�`���̉��
	Direct3D9_ObjectRelease( TempMemTexture9 ) ;
	TempMemTexture9 = NULL ;

	// �I��
	return 0 ;

ERR :
	if( GD3D9.RGBtoVMaxRGBVolumeTexture )
	{
		Direct3D9_ObjectRelease( GD3D9.RGBtoVMaxRGBVolumeTexture ) ;
		GD3D9.RGBtoVMaxRGBVolumeTexture = NULL ;
	}

	if( TempMemTexture9 )
	{
		Direct3D9_ObjectRelease( TempMemTexture9 ) ;
		TempMemTexture9 = NULL ;
	}
	
	return -1 ;
}

#endif // DX_NON_MODEL

// Direct3D9 ���g�p�����O���t�B�b�N�X�����̏��������s��( 0:���� -1:���s )
extern	int		Graphics_D3D9_Initialize( void )
{
	int Ret ;

	GSYS.Setting.ValidHardware = FALSE ;

	// Direct3D9.DLL �̓ǂݍ���
	if( Direct3D9_LoadDLL() < 0 )
	{
		goto ERR ;
	}

	// DirectDraw7 ���쐬����
	if( DirectDraw7_Create() < 0 )
	{
		goto ERR ;
	}

CREATEDIRECT3D9OBJECT :
	// Direct3DDevice9 ���쐬����
	if( Direct3D9_Create() != 0 )
	{
		goto ERR ;
	}

	// Direct3DDevice9 �̎擾�����݂�
	Ret = Graphics_D3D9_Device_Create() ;
	if( Ret == -2 )
	{
		goto CREATEDIRECT3D9OBJECT ;
	}
	else
	if( Ret != 0 )
	{
		goto ERR ;
	}

	// Z�o�b�t�@ �̍쐬�����݂�
	if( Graphics_D3D9_CreateZBuffer() != 0 )
	{
		goto ERR ;
	}

	// �V�F�[�_�[�̍쐬
	Graphics_D3D9_Shader_Initialize() ;

	// �V�F�[�_�[���g�p�ł���ꍇ�Ƃł��Ȃ��ꍇ�ŏ����𕪊�
	if( GSYS.HardInfo.UseShader )
	{
		// ���_�錾�̍쐬
		Graphics_D3D9_VertexDeclaration_Create() ;

#ifndef DX_NON_MODEL
		// �{�����[���e�N�X�`���̍쐬
		CreateRGBtoVMaxRGBVolumeTexture() ;
#endif // DX_NON_MODEL

		// �t���X�N���[�����[�h�̏ꍇ�̓T�u�o�b�N�o�b�t�@�̃Z�b�g�A�b�v���s��
		if( GetWindowModeFlag() == FALSE )
		{
			Graphics_D3D9_SetupSubBackBuffer() ;
		}
	}

	GSYS.Setting.ValidHardware = TRUE ;

	// �I��
	return 0 ;

ERR :
	// Z�o�b�t�@ �̉��
	if( GD3D9.Device.Screen.ZBufferSurface )
	{
		Direct3D9_ObjectRelease( GD3D9.Device.Screen.ZBufferSurface ) ;
		GD3D9.Device.Screen.ZBufferSurface = NULL ;
	}

	// Direct3DDevice9 �̉��
	Direct3DDevice9_Release() ;
	GD3D9.Device.DrawInfo.BeginSceneFlag = FALSE ;

	// Direct3D9 �̉��
	Direct3D9_Release() ;

	// Direct3D9.DLL �̉��
	Direct3D9_FreeDLL() ;

	// DirectDraw7 �̉��
	DirectDraw7_Release() ;

	return -1 ;
}

// Direct3D9 ���g�p�����O���t�B�b�N�X�����̌�n�����s��
extern	int		Graphics_D3D9_Terminate( void )
{
#ifndef DX_NON_MODEL
	// ���f���̒��_�o�b�t�@�����
	MV1TerminateVertexBufferAll() ;
#endif

	// ���_�錾�̉��
	Graphics_D3D9_VertexDeclaration_Terminate() ;

	// �V�F�[�_�[�̉��
	Graphics_D3D9_Shader_Terminate() ;

//	// �n�[�h�E�G�A�A�N�Z�����[�^���g�p���Ȃ��ꍇ�͂����ŉ�ʃ��[�h�����ɖ߂�
//	if( /*NS_GetWindowModeFlag() == FALSE &&*/
//		( GSYS.Setting.ValidHardware == FALSE ||
//		 ( WinData.WindowsVersion >= DX_WINDOWSVERSION_VISTA && ( GSYS.Screen.ValidGraphDisplayArea || GSYS.Screen.Emulation320x240Flag || GSYS.Screen.FullScreenEmulation320x240 ) ) ) )
	// ChangeDisplaySettings ���g�p���ĉ�ʃ��[�h��ύX�����ꍇ�͂����ŉ�ʃ��[�h�����ɖ߂�
	if( GRAWIN.UseChangeDisplaySettings )
	{
		GRAWIN.UseChangeDisplaySettings = FALSE ;
		ChangeDisplaySettings( NULL, 0 ) ;
	}

	// Z�o�b�t�@ �̉��
	if( GD3D9.Device.Screen.ZBufferSurface )
	{
		Direct3D9_ObjectRelease( GD3D9.Device.Screen.ZBufferSurface ) ;
		GD3D9.Device.Screen.ZBufferSurface = NULL ;
	}

	// �T�u�o�b�N�o�b�t�@�̉��
	if( GD3D9.Device.Screen.SubBackBufferSurface )
	{
		Direct3D9_ObjectRelease( GD3D9.Device.Screen.SubBackBufferSurface ) ;
		GD3D9.Device.Screen.SubBackBufferSurface = NULL ;
	}
	if( GD3D9.Device.Screen.SubBackBufferTexture )
	{
		Direct3D9_ObjectRelease( GD3D9.Device.Screen.SubBackBufferTexture ) ;
		GD3D9.Device.Screen.SubBackBufferTexture = NULL ;
	}

	// �o�b�N�o�b�t�@�̉��
	if( GD3D9.Device.Screen.BackBufferSurface )
	{
		Direct3D9_ObjectRelease( GD3D9.Device.Screen.BackBufferSurface ) ;
		GD3D9.Device.Screen.BackBufferSurface = NULL ;
	}

#ifndef DX_NON_MODEL
	// �{�����[���e�N�X�`���̉��
	if( GD3D9.RGBtoVMaxRGBVolumeTexture )
	{
		Direct3D9_ObjectRelease( GD3D9.RGBtoVMaxRGBVolumeTexture ) ;
		GD3D9.RGBtoVMaxRGBVolumeTexture = NULL ;
	}
#endif // DX_NON_MODEL

	// Direct3DDevice9 �̉��
	Direct3DDevice9_Release() ;
	GD3D9.Device.DrawInfo.BeginSceneFlag = FALSE ;

	// Direct3D9 �̉��
	Direct3D9_Release() ;

	// DirectDraw �̉��
	DirectDraw7_Release() ;

	// d3d9.dll �̉��
	Direct3D9_FreeDLL() ;

	// �f�B�X�v���C���p�Ɋm�ۂ����������̉��
	if( GSYS.Screen.DisplayInfo )
	{
		HeapFree( GetProcessHeap(), 0, GSYS.Screen.DisplayInfo ) ;
		GSYS.Screen.DisplayInfo = NULL ;
	}

	// �I��
	return 0 ;
}


// ���ׂĂ� Direct3D9 �n�I�u�W�F�N�g���������
extern	int		Graphics_D3D9_ReleaseObjectAll( void )
{
	int i ;
	SHADERHANDLEDATA                          *Shader ;
	VERTEXBUFFERHANDLEDATA                    *VertexBuffer ;
	INDEXBUFFERHANDLEDATA                     *IndexBuffer ;
	GRAPHICS_HARDDATA_DIRECT3D9_SYSMEMTEXTURE *MemTex ;
	GRAPHICS_HARDDATA_DIRECT3D9_SYSMEMSURFACE *MemSurface ;
	IMAGEDATA                                 *Image ;
	SHADOWMAPDATA                             *ShadowMap ;

	if( HandleManageArray[ DX_HANDLETYPE_GRAPH ].InitializeFlag )
	{
		for( i = HandleManageArray[ DX_HANDLETYPE_GRAPH ].AreaMin ; i <= HandleManageArray[ DX_HANDLETYPE_GRAPH ].AreaMax ; i ++ )
		{
			Image = ( IMAGEDATA * )HandleManageArray[ DX_HANDLETYPE_GRAPH ].Handle[ i ] ;
			if( Image == NULL ) continue ;

#ifndef DX_NON_ASYNCLOAD
			WaitASyncLoad( Image->HandleInfo.Handle ) ;
#endif

			if( Image->Orig == NULL ) continue ;
			Graphics_Hardware_ReleaseOrigTexture_PF( Image->Orig ) ;

#ifndef DX_NON_MOVIE
			if( Image->MovieHandle != -1 )
			{
				ReleaseMovieSurface( Image->MovieHandle ) ;
			}
#endif
		}
	}

	if( HandleManageArray[ DX_HANDLETYPE_SHADOWMAP ].InitializeFlag )
	{
		for( i = HandleManageArray[ DX_HANDLETYPE_SHADOWMAP ].AreaMin ; i <= HandleManageArray[ DX_HANDLETYPE_SHADOWMAP ].AreaMax ; i ++ )
		{
			ShadowMap = ( SHADOWMAPDATA * )HandleManageArray[ DX_HANDLETYPE_SHADOWMAP ].Handle[ i ] ;
			if( ShadowMap == NULL ) continue ;

#ifndef DX_NON_ASYNCLOAD
			WaitASyncLoad( ShadowMap->HandleInfo.Handle ) ;
#endif

			Graphics_Hardware_D3D9_ShadowMap_ReleaseTexture_PF( ShadowMap  ) ;
		}
	}

	if( HandleManageArray[ DX_HANDLETYPE_VERTEX_BUFFER ].InitializeFlag )
	{
		for( i = HandleManageArray[ DX_HANDLETYPE_VERTEX_BUFFER ].AreaMin ; i <= HandleManageArray[ DX_HANDLETYPE_VERTEX_BUFFER ].AreaMax ; i ++ )
		{
			VertexBuffer = ( VERTEXBUFFERHANDLEDATA * )HandleManageArray[ DX_HANDLETYPE_VERTEX_BUFFER ].Handle[ i ] ;
			if( VertexBuffer == NULL ) continue ;

#ifndef DX_NON_ASYNCLOAD
			WaitASyncLoad( VertexBuffer->HandleInfo.Handle ) ;
#endif

			Graphics_D3D9_VertexBuffer_ReleaseObject( VertexBuffer ) ;
		}
	}

	if( HandleManageArray[ DX_HANDLETYPE_INDEX_BUFFER ].InitializeFlag )
	{
		for( i = HandleManageArray[ DX_HANDLETYPE_INDEX_BUFFER ].AreaMin ; i <= HandleManageArray[ DX_HANDLETYPE_INDEX_BUFFER ].AreaMax ; i ++ )
		{
			IndexBuffer = ( INDEXBUFFERHANDLEDATA * )HandleManageArray[ DX_HANDLETYPE_INDEX_BUFFER ].Handle[ i ] ;
			if( IndexBuffer == NULL ) continue ;

#ifndef DX_NON_ASYNCLOAD
			WaitASyncLoad( IndexBuffer->HandleInfo.Handle ) ;
#endif

			Graphics_D3D9_IndexBuffer_ReleaseObject( IndexBuffer ) ;
		}
	}

	if( HandleManageArray[ DX_HANDLETYPE_SHADER ].InitializeFlag )
	{
		for( i = HandleManageArray[ DX_HANDLETYPE_SHADER ].AreaMin ; i <= HandleManageArray[ DX_HANDLETYPE_SHADER ].AreaMax ; i ++ )
		{
			Shader = ( SHADERHANDLEDATA * )HandleManageArray[ DX_HANDLETYPE_SHADER ].Handle[ i ] ;
			if( Shader == NULL ) continue ;

#ifndef DX_NON_ASYNCLOAD
			WaitASyncLoad( Shader->HandleInfo.Handle ) ;
#endif

			if( Shader->IsVertexShader )
			{
				if( Shader->PF->D3D9.VertexShader )
				{
					Direct3D9_ObjectRelease( Shader->PF->D3D9.VertexShader ) ;
					Shader->PF->D3D9.VertexShader = NULL ;
				}
			}
			else
			{
				if( Shader->PF->D3D9.PixelShader )
				{
					Direct3D9_ObjectRelease( Shader->PF->D3D9.PixelShader ) ;
					Shader->PF->D3D9.PixelShader = NULL ;
				}
			}
		}
	}

	MemTex = GD3D9.SysMemTexSurf.Texture ;
	for( i = 0 ; i < GD3D9.SysMemTexSurf.TextureInitNum ; MemTex ++ )
	{
		if( MemTex->InitializeFlag == FALSE )
			continue ;

		i ++ ;

		if( MemTex->MemTexture != NULL )
		{
			Direct3D9_ObjectRelease( MemTex->MemTexture ) ;
			MemTex->MemTexture = NULL ;
		}
	}

	MemSurface = GD3D9.SysMemTexSurf.Surface ;
	for( i = 0 ; i < GD3D9.SysMemTexSurf.SurfaceInitNum ; MemSurface ++ )
	{
		if( MemSurface->InitializeFlag == FALSE )
			continue ;

		i ++ ;

		if( MemSurface->MemSurface != NULL )
		{
			Direct3D9_ObjectRelease( MemSurface->MemSurface ) ;
			MemSurface->MemSurface = NULL ;
		}
	}

	// �I��
	return 0 ;
}


// ���ׂĂ� Direct3D9 �n�I�u�W�F�N�g���쐬����
extern	int		Graphics_D3D9_CreateObjectAll( void )
{
	int                                       i ;
	SHADERHANDLEDATA                          *Shader ;
	VERTEXBUFFERHANDLEDATA                    *VertexBuffer ;
	INDEXBUFFERHANDLEDATA                     *IndexBuffer ;
	GRAPHICS_HARDDATA_DIRECT3D9_SYSMEMTEXTURE *MemTex ;
	GRAPHICS_HARDDATA_DIRECT3D9_SYSMEMSURFACE *MemSurface ;
	IMAGEDATA                                 *Image ;
	SHADOWMAPDATA                             *ShadowMap ;

	MemTex = GD3D9.SysMemTexSurf.Texture ;
	for( i = 0 ; i < GD3D9.SysMemTexSurf.TextureInitNum ; MemTex ++ )
	{
		if( MemTex->InitializeFlag == FALSE )
			continue ;

		i ++ ;

		Graphics_D3D9_CreateSysMemTextureDirect3D9Texture( MemTex ) ;
	}

	MemSurface = GD3D9.SysMemTexSurf.Surface ;
	for( i = 0 ; i < GD3D9.SysMemTexSurf.SurfaceInitNum ; MemSurface ++ )
	{
		if( MemSurface->InitializeFlag == FALSE )
			continue ;

		i ++ ;

		Graphics_D3D9_CreateSysMemSurfaceDirect3D9Surface( MemSurface ) ;
	}

	if( HandleManageArray[ DX_HANDLETYPE_GRAPH ].InitializeFlag )
	{
		for( i = HandleManageArray[ DX_HANDLETYPE_GRAPH ].AreaMin ; i <= HandleManageArray[ DX_HANDLETYPE_GRAPH ].AreaMax ; i ++ )
		{
			Image = ( IMAGEDATA * )HandleManageArray[ DX_HANDLETYPE_GRAPH ].Handle[ i ] ;

			if( Image == NULL ) continue ;
			if( Image->Orig == NULL ) continue ;
			Graphics_Hardware_CreateOrigTexture_PF( Image->Orig ) ;
		}
	}

	if( HandleManageArray[ DX_HANDLETYPE_SHADOWMAP ].InitializeFlag )
	{
		for( i = HandleManageArray[ DX_HANDLETYPE_SHADOWMAP ].AreaMin ; i <= HandleManageArray[ DX_HANDLETYPE_SHADOWMAP ].AreaMax ; i ++ )
		{
			ShadowMap = ( SHADOWMAPDATA * )HandleManageArray[ DX_HANDLETYPE_SHADOWMAP ].Handle[ i ] ;

			if( ShadowMap == NULL ) continue ;
			Graphics_Hardware_D3D9_ShadowMap_CreateTexture_PF( ShadowMap ) ;
		}
	}

	if( HandleManageArray[ DX_HANDLETYPE_VERTEX_BUFFER ].InitializeFlag )
	{
		for( i = HandleManageArray[ DX_HANDLETYPE_VERTEX_BUFFER ].AreaMin ; i <= HandleManageArray[ DX_HANDLETYPE_VERTEX_BUFFER ].AreaMax ; i ++ )
		{
			VertexBuffer = ( VERTEXBUFFERHANDLEDATA * )HandleManageArray[ DX_HANDLETYPE_VERTEX_BUFFER ].Handle[ i ] ;
			if( VertexBuffer == NULL ) continue ;
			Graphics_D3D9_VertexBuffer_CreateObject( VertexBuffer, TRUE ) ;
		}
	}

	if( HandleManageArray[ DX_HANDLETYPE_INDEX_BUFFER ].InitializeFlag )
	{
		for( i = HandleManageArray[ DX_HANDLETYPE_INDEX_BUFFER ].AreaMin ; i <= HandleManageArray[ DX_HANDLETYPE_INDEX_BUFFER ].AreaMax ; i ++ )
		{
			IndexBuffer = ( INDEXBUFFERHANDLEDATA * )HandleManageArray[ DX_HANDLETYPE_INDEX_BUFFER ].Handle[ i ] ;
			if( IndexBuffer == NULL ) continue ;
			Graphics_D3D9_IndexBuffer_CreateObject( IndexBuffer, TRUE ) ;
		}
	}

	if( HandleManageArray[ DX_HANDLETYPE_SHADER ].InitializeFlag )
	{
		for( i = HandleManageArray[ DX_HANDLETYPE_SHADER ].AreaMin ; i <= HandleManageArray[ DX_HANDLETYPE_SHADER ].AreaMax ; i ++ )
		{
			Shader = ( SHADERHANDLEDATA * )HandleManageArray[ DX_HANDLETYPE_SHADER ].Handle[ i ] ;
			if( Shader == NULL ) continue ;
			if( Shader->IsVertexShader )
			{
				Direct3DDevice9_CreateVertexShader( ( DWORD * )Shader->FunctionCode, &Shader->PF->D3D9.VertexShader ) ;
			}
			else
			{
				Direct3DDevice9_CreatePixelShader( ( DWORD * )Shader->FunctionCode, &Shader->PF->D3D9.PixelShader ) ;
			}
		}
	}

	// �I��
	return 0 ;
}

// �s�N�Z���V�F�[�_�[���g�p����ꍇ�͏o�͐���e�N�X�`���Ƃ��Ďg�p�ł��邩�ǂ����̃`�F�b�N���s��
extern	int		Graphics_D3D9_SetupRenderTargetInputTextureFlag( void )
{
#ifndef DX_NON_FILTER
	// �s�N�Z���V�F�[�_�[���g�p����ꍇ�͏o�͐���e�N�X�`���Ƃ��Ďg�p�ł��邩�ǂ����̃`�F�b�N���s��
	GD3D9.Device.Caps.ValidRenderTargetInputTexture = FALSE ;
/*	if( GSYS.Setting.ValidHardware == TRUE && GSYS.HardInfo.UseShader == TRUE )
	{
		int TempScreen ;
		BASEIMAGE TempInImage ;
		BASEIMAGE TempOutImage ;
		BASEIMAGE TempScreenImage ;
		RECT LockRect ;
		int i, j, s, count ;
		int r, g, b, a ;
		const int rgb[ 4 * 3 ] =
		{
			204, 168,  80,
			128, 100, 245,
			 66, 222, 143,
			 34, 255,  78,
		} ;
		SETUP_GRAPHHANDLE_GPARAM GParam ;

		Graphics_Image_InitSetupGraphHandleGParam( &GParam ) ;
		GParam.DrawValidImageCreateFlag      = TRUE ;
		GParam.DrawValidAlphaImageCreateFlag = FALSE ;
		GParam.CubeMapTextureCreateFlag      = FALSE ;
		TempScreen = Graphics_Image_MakeGraph_UseGParam( &GParam, 8, 8, FALSE, FALSE ) ;
		NS_CreateXRGB8ColorBaseImage( 8, 8, &TempInImage ) ;
		NS_CreateXRGB8ColorBaseImage( 8, 8, &TempOutImage ) ;

		j = 0 ;
		for( i = 0 ; i < 4 ; i ++, j += 3 )
		{
			NS_SetPixelBaseImage( &TempInImage, i, i, rgb[ j + 0 ], rgb[ j + 1 ], rgb[ j + 2 ], 255 ) ;
		}

		NS_BltBmpOrGraphImageToGraph( NULL, NULL, NULL, FALSE, &TempInImage, NULL, 0, 0, TempScreen ) ;
		GD3D9.Device.Caps.ValidRenderTargetInputTexture = TRUE ;
		NS_GraphFilter( TempScreen, DX_GRAPH_FILTER_INVERT ) ;
		GD3D9.Device.Caps.ValidRenderTargetInputTexture = FALSE ;
		LockRect.left   = 0 ;
		LockRect.top    = 0 ;
		LockRect.right  = 8 ;
		LockRect.bottom = 8 ;
		if( Graphics_Screen_LockDrawScreen( &LockRect, &TempScreenImage, TempScreen, 0, TRUE, 0 ) != -1 )
		{
			NS_BltBaseImage( 0, 0, &TempScreenImage, &TempOutImage ) ;
			Graphics_Screen_UnlockDrawScreen() ;

			count = 0 ;
			j = 0 ;
			for( i = 0 ; i < 4 ; i ++, j += 3 )
			{
				NS_GetPixelBaseImage( &TempOutImage, i, i, &r, &g, &b, &a ) ;
				s = r - ( 255 - rgb[ j + 0 ] ) ; if( s >= -1 && s <= 1 ) count ++ ;
				s = g - ( 255 - rgb[ j + 1 ] ) ; if( s >= -1 && s <= 1 ) count ++ ;
				s = b - ( 255 - rgb[ j + 2 ] ) ; if( s >= -1 && s <= 1 ) count ++ ;
			}
			if( count == 4 * 3 )
			{
				GD3D9.Device.Caps.ValidRenderTargetInputTexture = TRUE ;
			}
		}
		NS_DeleteGraph( TempScreen ) ;
		NS_ReleaseBaseImage( &TempInImage ) ;
		NS_ReleaseBaseImage( &TempOutImage ) ;
	}*/
#endif // DX_NON_FILTER

	// �I��
	return 0 ;
}


























// �V�F�[�_�[�R�[�h�֌W

// Direct3D9 �̂Q�c�`��p�̃V�F�[�_�[�R�[�h�̏��������s��
extern int Graphics_D3D9_ShaderCode_2D_Initialize( void )
{
	GRAPHICS_HARDWARE_DIRECT3D9_SHADERCODE_2D *SC2D = &GraphicsHardDataDirect3D9.ShaderCode._2D ;
	int   Size = 0 ;
	int   Addr = 0 ;
	int   i    = 0 ;
	void  **AddrBuf   = NULL ;
	BYTE  *ShaderAddr = NULL ;
	WORD  *SizeBuf    = NULL ;
	short *SizeDest   = NULL ;

	// ���łɏ���������Ă����牽�����Ȃ�
	if( SC2D->_2DShaderInitializeFlag == TRUE )
	{
		return TRUE ;
	}

#ifndef DX_NON_FILTER
	SC2D->FilterShaderBinDxaImage = NULL ;
	SC2D->RGBAMixS_ShaderPackImage = NULL ;
#endif // DX_NON_FILTER

#ifndef DX_NON_NORMAL_DRAW_SHADER
	SC2D->BaseShaderBinDxaImage = NULL ;

	// ��{�V�F�[�_�[�I�u�W�F�N�g�t�@�C���c�w�`�����k�����f�[�^���𓀂���
	{
		if( DxShaderCodeBin_BaseConvert == 0 )
		{
			DxShaderCodeBin_BaseConvert = 1 ;
			Char128ToBin( DxShaderCodeBin_Base, DxShaderCodeBin_Base ) ;
		}
		Size = DXA_Decode( DxShaderCodeBin_Base, NULL ) ;
		SC2D->BaseShaderBinDxaImage = DXALLOC( ( size_t )Size ) ;
		if( SC2D->BaseShaderBinDxaImage == NULL )
		{
			goto ERR ;
		}

		DXA_Decode( DxShaderCodeBin_Base, SC2D->BaseShaderBinDxaImage ) ;

		// �c�w�`�t�@�C�����I�[�v������
		DXA_Initialize( &SC2D->BaseShaderBinDxa ) ;
		if( DXA_OpenArchiveFromMem( &SC2D->BaseShaderBinDxa, SC2D->BaseShaderBinDxaImage, Size, FALSE, FALSE ) != 0 )
		{
			goto ERR ;
		}
	}
#else // DX_NON_NORMAL_DRAW_SHADER
	Graphics_Hardware_D3D9_SetUseNormalDrawShader_PF( FALSE ) ;
#endif // DX_NON_NORMAL_DRAW_SHADER

#ifndef DX_NON_FILTER

	// RGBAMix�V�F�[�_�[�p�b�N���k�t�@�C����W�J����
	{
		if( DxShaderCodeBin_RgbaMixConvert == 0 )
		{
			DxShaderCodeBin_RgbaMixConvert = 1 ;
			Char128ToBin( DxShaderCodeBin_RgbaMix, DxShaderCodeBin_RgbaMix ) ;
		}
		Size = DXA_Decode( DxShaderCodeBin_RgbaMix, NULL ) ;
		SC2D->RGBAMixS_ShaderPackImage = DXALLOC( ( size_t )Size ) ;
		if( SC2D->RGBAMixS_ShaderPackImage == NULL )
		{
			goto ERR ;
		}

		DXA_Decode( DxShaderCodeBin_RgbaMix, SC2D->RGBAMixS_ShaderPackImage ) ;

		// �A�h���X���X�g�̃Z�b�g
		Addr       = 0 ;
		SizeBuf    = ( WORD  * )SC2D->RGBAMixS_ShaderPackImage ;
		ShaderAddr = ( BYTE  * )SC2D->RGBAMixS_ShaderPackImage + sizeof( WORD ) * 256 ;
		AddrBuf    = ( void ** )SC2D->RGBAMixS_ShaderAddress ;
		SizeDest   = ( short * )SC2D->RGBAMixS_ShaderSize ;
		for( i = 0 ; i < 256 ; i ++, SizeBuf ++, AddrBuf ++, SizeDest ++ )
		{
			if( *SizeBuf != 0 )
			{
				*AddrBuf    = ( void * )ShaderAddr ;
				ShaderAddr += *SizeBuf ;
			}
			*SizeDest = ( short )*SizeBuf ;
		}
	}

	// �t�B���^�[�V�F�[�_�[�I�u�W�F�N�g�t�@�C���c�w�`�����k�����f�[�^���𓀂���
	{
		if( DxShaderCodeBin_FilterConvert == 0 )
		{
			DxShaderCodeBin_FilterConvert = 1 ;
			Char128ToBin( DxShaderCodeBin_Filter, DxShaderCodeBin_Filter ) ;
		}
		Size = DXA_Decode( DxShaderCodeBin_Filter, NULL ) ;
		SC2D->FilterShaderBinDxaImage = DXALLOC( ( size_t )Size ) ;
		if( SC2D->FilterShaderBinDxaImage == NULL )
		{
			goto ERR ;
		}

		DXA_Decode( DxShaderCodeBin_Filter, SC2D->FilterShaderBinDxaImage ) ;

		// �c�w�`�t�@�C�����I�[�v������
		DXA_Initialize( &SC2D->FilterShaderBinDxa ) ;
		if( DXA_OpenArchiveFromMem( &SC2D->FilterShaderBinDxa, SC2D->FilterShaderBinDxaImage, Size, FALSE, FALSE ) != 0 )
		{
			goto ERR ;
		}
	}

#endif // DX_NON_FILTER

	SC2D->_2DShaderInitializeFlag = TRUE ;

	// ����I��
	return TRUE ;

ERR :
#ifndef DX_NON_NORMAL_DRAW_SHADER
	if( SC2D->BaseShaderBinDxaImage != NULL )
	{
		DXFREE( SC2D->BaseShaderBinDxaImage ) ;
		SC2D->BaseShaderBinDxaImage = NULL ;
	}
#endif // DX_NON_NORMAL_DRAW_SHADER

#ifndef DX_NON_FILTER
	if( SC2D->FilterShaderBinDxaImage != NULL )
	{
		DXFREE( SC2D->FilterShaderBinDxaImage ) ;
		SC2D->FilterShaderBinDxaImage = NULL ;
	}

	if( SC2D->RGBAMixS_ShaderPackImage != NULL )
	{
		DXFREE( SC2D->RGBAMixS_ShaderPackImage ) ;
		SC2D->RGBAMixS_ShaderPackImage = NULL ;
	}
#endif // DX_NON_FILTER

	return FALSE ;
}



// Direct3D9 �̂Q�c�`��p�̃V�F�[�_�[�R�[�h�̌�n�����s��
extern int Graphics_D3D9_ShaderCode_2D_Terminate( void )
{
	GRAPHICS_HARDWARE_DIRECT3D9_SHADERCODE_2D *SC2D = &GraphicsHardDataDirect3D9.ShaderCode._2D ;

	// ���łɌ�n������Ă����牽�����Ȃ�
	if( SC2D->_2DShaderInitializeFlag == FALSE )
	{
		return TRUE ;
	}

#ifndef DX_NON_NORMAL_DRAW_SHADER
	// ��{�V�F�[�_�[�p�c�w�`�̌�n��
	DXA_Terminate( &SC2D->BaseShaderBinDxa ) ;


	// �𓀂����V�F�[�_�[���i�[���Ă����������̉��
	if( SC2D->BaseShaderBinDxaImage != NULL )
	{
		DXFREE( SC2D->BaseShaderBinDxaImage ) ;
		SC2D->BaseShaderBinDxaImage = NULL ;
	}
#endif // DX_NON_NORMAL_DRAW_SHADER

#ifndef DX_NON_FILTER

	// �t�B���^�[�V�F�[�_�[�p�c�w�`�̌�n��
	DXA_Terminate( &SC2D->FilterShaderBinDxa ) ;

	if( SC2D->RGBAMixS_ShaderPackImage )
	{
		DXFREE( SC2D->RGBAMixS_ShaderPackImage ) ;
		SC2D->RGBAMixS_ShaderPackImage = NULL ;
	}
	if( SC2D->FilterShaderBinDxaImage )
	{
		DXFREE( SC2D->FilterShaderBinDxaImage ) ;
		SC2D->FilterShaderBinDxaImage = NULL ;
	}

#endif // DX_NON_FILTER

	// �������t���O��|��
	SC2D->_2DShaderInitializeFlag = FALSE ;

	// ����I��
	return TRUE ;
}








#ifndef DX_NON_MODEL

// Direct3D9 �̃��f���`��p�̃V�F�[�_�[�R�[�h�̏��������s��
extern int Graphics_D3D9_ShaderCode_Model_Initialize( void )
{
	GRAPHICS_HARDWARE_DIRECT3D9_SHADERCODE_MODEL *SCMD = &GraphicsHardDataDirect3D9.ShaderCode.Model ;
	HANDLEMANAGE *HandleManage = &HandleManageArray[ DX_HANDLETYPE_MODEL_BASE ] ;
	int  Size = 0 ;
	int  Addr = 0 ;
	int  i    = 0 ;
	void **AddrBuf   = NULL ;
	BYTE *ShaderAddr = NULL ;
	WORD *SizeBuf    = NULL ;

	// ���łɏ���������Ă����牽�����Ȃ�
	if( SCMD->ModelShaderInitializeFlag == TRUE )
	{
		return TRUE ;
	}

	// �N���e�B�J���Z�N�V�����̎擾
	CRITICALSECTION_LOCK( &HandleManage->CriticalSection ) ;

	// �N���e�B�J���Z�N�V�������擾������ŉ��߂ď������m�F
	if( SCMD->ModelShaderInitializeFlag == TRUE )
	{
		// �N���e�B�J���Z�N�V�����̉��
		CriticalSection_Unlock( &HandleManage->CriticalSection ) ;

		return TRUE ;
	}

	SCMD->ModelShaderPackImage = NULL ;

	// ���C�g�C���f�b�N�X���X�g�̍\�z
	{
		int l0, l1, l2, l3, l4, l5, ind ;

		ind = 0 ;
		for( l0 = 0 ; l0 < 4 ; l0 ++ )
		{
			for( l1 = 0 ; l1 <= l0 ; l1 ++ )
			{
				for( l2 = 0 ; l2 <= l1 ; l2 ++ )
				{
					for( l3 = 0 ; l3 <= l2 ; l3 ++ )
					{
						for( l4 = 0 ; l4 <= l3 ; l4 ++ )
						{
							for( l5 = 0 ; l5 <= l4 ; l5 ++ )
							{
								SCMD->LightIndexList84[ l0 ][ l1 ][ l2 ][ l3 ][ l4 ][ l5 ] = ( short )ind ;
								ind ++ ;
							}
						}
					}
				}
			}
		}

		ind = 0 ;
		for( l0 = 0 ; l0 < 4 ; l0 ++ )
		{
			for( l1 = 0 ; l1 <= l0 ; l1 ++ )
			{
				for( l2 = 0 ; l2 <= l1 ; l2 ++ )
				{
					SCMD->LightIndexList20[ l0 ][ l1 ][ l2 ] = ( short )ind ;
					ind ++ ;
				}
			}
		}

		ind = 0 ;
		for( l0 = 0 ; l0 < 3 ; l0 ++ )
		{
			for( l1 = 0 ; l1 <= l0 ; l1 ++ )
			{
				for( l2 = 0 ; l2 <= l1 ; l2 ++ )
				{
					SCMD->LightIndexList10[ l0 ][ l1 ][ l2 ] = ( short )ind ;
					ind ++ ;
				}
			}
		}
	}

	if( DxShaderCodeBin_ModelConvert == 0 )
	{
		DxShaderCodeBin_ModelConvert = 1 ;
		Char128ToBin( DxShaderCodeBin_Model, DxShaderCodeBin_Model ) ;
	}
	Size = DXA_Decode( DxShaderCodeBin_Model, NULL ) ;
	SCMD->ModelShaderPackImage = DXCALLOC( ( size_t )Size ) ;
	if( SCMD->ModelShaderPackImage == NULL )
	{
		goto ERR ;
	}

	DXA_Decode( DxShaderCodeBin_Model, SCMD->ModelShaderPackImage ) ;

	// �A�h���X���X�g�̃Z�b�g
	Addr = 0 ;
	SizeBuf = ( WORD * )SCMD->ModelShaderPackImage ;
	ShaderAddr = ( BYTE * )SCMD->ModelShaderPackImage +
		sizeof( WORD ) * (
			sizeof( SCMD->MV1_PixelLighting_VertexShaderAddress          ) / sizeof( void * ) +
			sizeof( SCMD->MV1_PixelLighting_ToonPixelShaderAddress       ) / sizeof( void * ) +
			sizeof( SCMD->MV1_PixelLighting_PixelShaderAddress           ) / sizeof( void * ) +

			sizeof( SCMD->MV1_ToonOutLineVertexShaderAddress   ) / sizeof( void * ) +
			sizeof( SCMD->MV1_ShadowMapVertexShaderAddress     ) / sizeof( void * ) +
			sizeof( SCMD->MV1_NoLightingVertexShaderAddress    ) / sizeof( void * ) +
			sizeof( SCMD->MV1_LightingVertexShaderAddress      ) / sizeof( void * ) +
			sizeof( SCMD->MV1_ToonOutLinePixelShaderAddress    ) / sizeof( void * ) +
			sizeof( SCMD->MV1_ShadowMapToonPixelShaderAddress  ) / sizeof( void * ) +
			sizeof( SCMD->MV1_ShadowMapPixelShaderAddress      ) / sizeof( void * ) +
			sizeof( SCMD->MV1_ToonNoLightingPixelShaderAddress ) / sizeof( void * ) +
			sizeof( SCMD->MV1_NoLightingPixelShaderAddress     ) / sizeof( void * ) +
			sizeof( SCMD->MV1_ToonLightingPixelShaderAddress   ) / sizeof( void * ) +
			sizeof( SCMD->MV1_LightingPixelShaderAddress       ) / sizeof( void * ) ) ;

	AddrBuf = ( void ** )SCMD->MV1_PixelLighting_VertexShaderAddress ;
	for( i = 0 ; i < sizeof( SCMD->MV1_PixelLighting_VertexShaderAddress ) / sizeof( void * ) ; i ++, SizeBuf ++, AddrBuf ++ )
	{
		if( *SizeBuf != 0 )
		{
			*AddrBuf = ( void * )ShaderAddr ;
			ShaderAddr += *SizeBuf ;
		}
	}

	AddrBuf = ( void ** )SCMD->MV1_PixelLighting_ToonPixelShaderAddress ;
	for( i = 0 ; i < sizeof( SCMD->MV1_PixelLighting_ToonPixelShaderAddress ) / sizeof( void * ) ; i ++, SizeBuf ++, AddrBuf ++ )
	{
		if( *SizeBuf != 0 )
		{
			*AddrBuf = ( void * )ShaderAddr ;
			ShaderAddr += *SizeBuf ;
		}
	}

	AddrBuf = ( void ** )SCMD->MV1_PixelLighting_PixelShaderAddress ;
	for( i = 0 ; i < sizeof( SCMD->MV1_PixelLighting_PixelShaderAddress ) / sizeof( void * ) ; i ++, SizeBuf ++, AddrBuf ++ )
	{
		if( *SizeBuf != 0 )
		{
			*AddrBuf = ( void * )ShaderAddr ;
			ShaderAddr += *SizeBuf ;
		}
	}







	AddrBuf = ( void ** )SCMD->MV1_ToonOutLineVertexShaderAddress ;
	for( i = 0 ; i < sizeof( SCMD->MV1_ToonOutLineVertexShaderAddress ) / sizeof( void * ) ; i ++, SizeBuf ++, AddrBuf ++ )
	{
		if( *SizeBuf != 0 )
		{
			*AddrBuf = ( void * )ShaderAddr ;
			ShaderAddr += *SizeBuf ;
		}
	}

	AddrBuf = ( void ** )SCMD->MV1_ShadowMapVertexShaderAddress ;
	for( i = 0 ; i < sizeof( SCMD->MV1_ShadowMapVertexShaderAddress ) / sizeof( void * ) ; i ++, SizeBuf ++, AddrBuf ++ )
	{
		if( *SizeBuf != 0 )
		{
			*AddrBuf = ( void * )ShaderAddr ;
			ShaderAddr += *SizeBuf ;
		}
	}

	AddrBuf = ( void ** )SCMD->MV1_NoLightingVertexShaderAddress ;
	for( i = 0 ; i < sizeof( SCMD->MV1_NoLightingVertexShaderAddress ) / sizeof( void * ) ; i ++, SizeBuf ++, AddrBuf ++ )
	{
		if( *SizeBuf != 0 )
		{
			*AddrBuf = ( void * )ShaderAddr ;
			ShaderAddr += *SizeBuf ;
		}
	}

	AddrBuf = ( void ** )SCMD->MV1_LightingVertexShaderAddress ;
	for( i = 0 ; i < sizeof( SCMD->MV1_LightingVertexShaderAddress ) / sizeof( void * ) ; i ++, SizeBuf ++, AddrBuf ++ )
	{
		if( *SizeBuf != 0 )
		{
			*AddrBuf = ( void * )ShaderAddr ;
			ShaderAddr += *SizeBuf ;
		}
	}




	AddrBuf = ( void ** )&SCMD->MV1_ToonOutLinePixelShaderAddress ;
	for( i = 0 ; i < sizeof( SCMD->MV1_ToonOutLinePixelShaderAddress ) / sizeof( void * ) ; i ++, SizeBuf ++, AddrBuf ++ )
	{
		if( *SizeBuf != 0 )
		{
			*AddrBuf = ( void * )ShaderAddr ;
			ShaderAddr += *SizeBuf ;
		}
	}

	AddrBuf = ( void ** )SCMD->MV1_ShadowMapToonPixelShaderAddress ;
	for( i = 0 ; i < sizeof( SCMD->MV1_ShadowMapToonPixelShaderAddress ) / sizeof( void * ) ; i ++, SizeBuf ++, AddrBuf ++ )
	{
		if( *SizeBuf != 0 )
		{
			*AddrBuf = ( void * )ShaderAddr ;
			ShaderAddr += *SizeBuf ;
		}
	}

	AddrBuf = ( void ** )&SCMD->MV1_ShadowMapPixelShaderAddress ;
	for( i = 0 ; i < sizeof( SCMD->MV1_ShadowMapPixelShaderAddress ) / sizeof( void * ) ; i ++, SizeBuf ++, AddrBuf ++ )
	{
		if( *SizeBuf != 0 )
		{
			*AddrBuf = ( void * )ShaderAddr ;
			ShaderAddr += *SizeBuf ;
		}
	}

	AddrBuf = ( void ** )SCMD->MV1_ToonNoLightingPixelShaderAddress ;
	for( i = 0 ; i < sizeof( SCMD->MV1_ToonNoLightingPixelShaderAddress ) / sizeof( void * ) ; i ++, SizeBuf ++, AddrBuf ++ )
	{
		if( *SizeBuf != 0 )
		{
			*AddrBuf = ( void * )ShaderAddr ;
			ShaderAddr += *SizeBuf ;
		}
	}

	AddrBuf = ( void ** )SCMD->MV1_NoLightingPixelShaderAddress ;
	for( i = 0 ; i < sizeof( SCMD->MV1_NoLightingPixelShaderAddress ) / sizeof( void * ) ; i ++, SizeBuf ++, AddrBuf ++ )
	{
		if( *SizeBuf != 0 )
		{
			*AddrBuf = ( void * )ShaderAddr ;
			ShaderAddr += *SizeBuf ;
		}
	}

	AddrBuf = ( void ** )SCMD->MV1_ToonLightingPixelShaderAddress ;
	for( i = 0 ; i < sizeof( SCMD->MV1_ToonLightingPixelShaderAddress ) / sizeof( void * ) ; i ++, SizeBuf ++, AddrBuf ++ )
	{
		if( *SizeBuf != 0 )
		{
			*AddrBuf = ( void * )ShaderAddr ;
			ShaderAddr += *SizeBuf ;
		}
	}

	AddrBuf = ( void ** )SCMD->MV1_LightingPixelShaderAddress ;
	for( i = 0 ; i < sizeof( SCMD->MV1_LightingPixelShaderAddress ) / sizeof( void * ) ; i ++, SizeBuf ++, AddrBuf ++ )
	{
		if( *SizeBuf != 0 )
		{
			*AddrBuf = ( void * )ShaderAddr ;
			ShaderAddr += *SizeBuf ;
		}
	}

	SCMD->ModelShaderInitializeFlag = TRUE ;

	// �N���e�B�J���Z�N�V�����̉��
	CriticalSection_Unlock( &HandleManage->CriticalSection ) ;

	// ����I��
	return TRUE ;

ERR :
	if( SCMD->ModelShaderPackImage != NULL )
	{
		DXFREE( SCMD->ModelShaderPackImage ) ;
		SCMD->ModelShaderPackImage = NULL ;
	}

	// �N���e�B�J���Z�N�V�����̉��
	CriticalSection_Unlock( &HandleManage->CriticalSection ) ;

	return FALSE ;
}

// Direct3D9 �̃��f���`��p�̃V�F�[�_�[�R�[�h�̌�n�����s��
extern int Graphics_D3D9_ShaderCode_Model_Terminate( void )
{
	GRAPHICS_HARDWARE_DIRECT3D9_SHADERCODE_MODEL *SCMD = &GraphicsHardDataDirect3D9.ShaderCode.Model ;

	// ���łɌ�n������Ă����牽�����Ȃ�
	if( SCMD->ModelShaderInitializeFlag == FALSE )
	{
		return TRUE ;
	}

	if( SCMD->ModelShaderPackImage )
	{
		DXFREE( SCMD->ModelShaderPackImage ) ;
		SCMD->ModelShaderPackImage = NULL ;
	}

	// �������t���O��|��
	SCMD->ModelShaderInitializeFlag = FALSE ;

	// ����I��
	return TRUE ;
}

#endif // DX_NON_MODEL








// Direct3D9 �̃V�F�[�_�[�̏��������s��
extern int Graphics_D3D9_Shader_Initialize( void )
{
	GRAPHICS_HARDWARE_DIRECT3D9_SHADERCODE *ShaderCode = &GraphicsHardDataDirect3D9.ShaderCode ;
	GRAPHICS_HARDWARE_DIRECT3D9_SHADER     *Shader     = &GraphicsHardDataDirect3D9.Device.Shader ;

	// ���ɍ쐬����Ă����Ƃ��̂��߂ɍ폜�������s��
	//TerminateDirect3D9Shader() ;
	Graphics_D3D9_Shader_Terminate() ;

//	return 0 ;

	// �V�F�[�_�[���g�p�ł��邩���ׂ�
	GSYS.HardInfo.UseShader = FALSE ;
	if( ( Shader->ValidVertexShader_SM3 && Shader->ValidPixelShader_SM3 ) ||
		( Shader->ValidVertexShader && Shader->ValidPixelShader ) )
	{
		GSYS.HardInfo.UseShader = TRUE ;

		// �V�F�[�_�[�R�[�h���W�J����Ă��Ȃ��ꍇ�͓W�J
		if( Graphics_D3D9_ShaderCode_2D_Initialize() == FALSE )
		{
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\xb7\x30\xa7\x30\xfc\x30\xc0\x30\xfc\x30\xb3\x30\xfc\x30\xc9\x30\x92\x30\x55\x5c\x8b\x95\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�V�F�[�_�[�R�[�h��W�J���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
			GSYS.HardInfo.UseShader = FALSE ;
			goto ENDLABEL ;
		}

#ifndef DX_NON_NORMAL_DRAW_SHADER
		// ��{�I�ȕ`�揈���Ɏg�p����s�N�Z���V�F�[�_�[���쐬����
		{
			int  i ;
			int  j ;
			int  k ;
			int  l ;
			int  m ;
			char FileName[ 64 ] ;
			int  Addr ;
			int  Size ;
			void *DataImage ;

			DataImage = DXA_GetFileImage( &ShaderCode->_2D.BaseShaderBinDxa ) ;

			// �Q�c�����p���_�V�F�[�_�[�̍쐬
//			if( DXA_GetFileInfo( &ShaderCode->_2D.BaseShaderBinDxa, "Base2DVertexShader.vso", &Addr, &Size ) == 0 )
//			{
//				if( Direct3DDevice9_CreateVertexShader( 
//					( DWORD * )( ( BYTE * )DataImage + Addr ),
//					&Shader->_2D.Base2DVertexShader ) != D_D3D_OK )
//				{
//					DXST_ERRORLOGFMT_ADDUTF16LE(( L"�Q�c�`��p��{���_�V�F�[�_�[�I�u�W�F�N�g�̍쐬�Ɏ��s���܂���\n" )) ;
//					GSYS.HardInfo.UseShader = FALSE ;
//					goto ENDLABEL ;
//				}
//			}

			// �R�c�����p���_�V�F�[�_�[�̍쐬
//			if( DXA_GetFileInfo( &ShaderCode->_2D.BaseShaderBinDxa, "Base3DVertexShader.vso", &Addr, &Size ) == 0 )
//			{
//				if( Direct3DDevice9_CreateVertexShader( 
//					( DWORD * )( ( BYTE * )DataImage + Addr ),
//					&Shader->_2D.Base3DVertexShader ) != D_D3D_OK )
//				{
//					DXST_ERRORLOGFMT_ADDUTF16LE(( L"�R�c�`��p��{���_�V�F�[�_�[�I�u�W�F�N�g�̍쐬�Ɏ��s���܂���\n" )) ;
//					GSYS.HardInfo.UseShader = FALSE ;
//					goto ENDLABEL ;
//				}
//			}

			// �e�N�X�`���Ȃ��V�F�[�_�[�̍쐬
			_STRCPY_S( FileName, sizeof( FileName ), "ps_nonetex_type0_ach0.pso" ) ;
			for( i = 0 ; i < 7 ; i ++ )
			{
				FileName[ 15 ] = ( char )( '0' + i ) ;
				for( j = 0 ; j < 2 ; j ++ )
				{
					FileName[ 20 ] = ( char )( '0' + j ) ;

					if( DXA_GetFileInfo( &ShaderCode->_2D.BaseShaderBinDxa, DX_CHARCODEFORMAT_ASCII, FileName, &Addr, &Size ) == 0 )
					{
						if( Direct3DDevice9_CreatePixelShader( 
							( DWORD * )( ( BYTE * )DataImage + Addr ),
							&Shader->_2D.BaseNoneTexPixelShader[ i ][ j ] ) != D_D3D_OK )
						{
							DXST_ERRORLOGFMT_ADDUTF16LE(( "\xfa\x57\x2c\x67\xb7\x30\xa7\x30\xfc\x30\xc0\x30\xfc\x30\xaa\x30\xd6\x30\xb8\x30\xa7\x30\xaf\x30\xc8\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"��{�V�F�[�_�[�I�u�W�F�N�g�̍쐬�Ɏ��s���܂���\n" @*/ )) ;
							GSYS.HardInfo.UseShader = FALSE ;
							goto ENDLABEL ;
						}
					}
				}
			}

			// �e�N�X�`������V�F�[�_�[�̍쐬
			_STRCPY_S( FileName, sizeof( FileName ), "ps_blend0_type0_igcolor0_igtalpha0_ach0.pso" ) ;
			for( i = 0 ; i < 4 ; i ++ )
			{
				FileName[ 8 ] = ( char )( '0' + i ) ;
				for( j = 0 ; j < 7 ; j ++ )
				{
					FileName[ 14 ] = ( char )( '0' + j ) ;
					for( k = 0 ; k < 2 ; k ++ )
					{
						FileName[ 23 ] =( char )(  '0' + k ) ;
						for( l = 0 ; l < 2 ; l ++ )
						{
							FileName[ 33 ] = ( char )( '0' + l ) ;
							for( m = 0 ; m < 2 ; m ++ )
							{
								FileName[ 38 ] = ( char )( '0' + m ) ;

								if( DXA_GetFileInfo( &ShaderCode->_2D.BaseShaderBinDxa, DX_CHARCODEFORMAT_ASCII, FileName, &Addr, &Size ) == 0 )
								{
									if( Direct3DDevice9_CreatePixelShader(
										( DWORD * )( ( BYTE * )DataImage + Addr ),
										&Shader->_2D.BaseUseTexPixelShader[ i ][ j ][ k ][ l ][ m ] ) != D_D3D_OK )
									{
										DXST_ERRORLOGFMT_ADDUTF16LE(( "\xfa\x57\x2c\x67\xb7\x30\xa7\x30\xfc\x30\xc0\x30\xfc\x30\xaa\x30\xd6\x30\xb8\x30\xa7\x30\xaf\x30\xc8\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"��{�V�F�[�_�[�I�u�W�F�N�g�̍쐬�Ɏ��s���܂���\n" @*/ )) ;
										GSYS.HardInfo.UseShader = FALSE ;
										goto ENDLABEL ;
									}
								}
							}
						}
					}
				}
			}

			// �}�X�N�����p�V�F�[�_�[�̍쐬
			DXA_GetFileInfo( &ShaderCode->_2D.BaseShaderBinDxa, DX_CHARCODEFORMAT_ASCII, "ps_mask_blend.pso", &Addr, &Size ) ;
			if( Direct3DDevice9_CreatePixelShader(
				( DWORD * )( ( BYTE * )DataImage + Addr ),
				&Shader->_2D.MaskEffectPixelShader ) != D_D3D_OK )
			{
				DXST_ERRORLOGFMT_ADDUTF16LE(( "\xde\x30\xb9\x30\xaf\x30\xe6\x51\x06\x74\x28\x75\xb7\x30\xa7\x30\xfc\x30\xc0\x30\xfc\x30\xaa\x30\xd6\x30\xb8\x30\xa7\x30\xaf\x30\xc8\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�}�X�N�����p�V�F�[�_�[�I�u�W�F�N�g�̍쐬�Ɏ��s���܂���\n" @*/ )) ;
				GSYS.HardInfo.UseShader = FALSE ;
				goto ENDLABEL ;
			}
			{
				static char *mask_use_grhandle_filename[ 4 ] =
				{
					"ps_mask_blend_use_grhandle_cha.pso",
					"ps_mask_blend_use_grhandle_chr.pso",
					"ps_mask_blend_use_grhandle_chg.pso",
					"ps_mask_blend_use_grhandle_chb.pso",
				};
				static char *mask_use_grhandle_reverse_filename[ 4 ] =
				{
					"ps_mask_blend_use_grhandle_cha_reverse.pso",
					"ps_mask_blend_use_grhandle_chr_reverse.pso",
					"ps_mask_blend_use_grhandle_chg_reverse.pso",
					"ps_mask_blend_use_grhandle_chb_reverse.pso",
				};

				for( i = 0 ; i < 4 ; i ++ )
				{
					DXA_GetFileInfo( &ShaderCode->_2D.BaseShaderBinDxa, DX_CHARCODEFORMAT_ASCII, mask_use_grhandle_filename[ i ], &Addr, &Size ) ;
					if( Direct3DDevice9_CreatePixelShader(
						( DWORD * )( ( BYTE * )DataImage + Addr ),
						&Shader->_2D.MaskEffect_UseGraphHandle_PixelShader[ i ] ) != D_D3D_OK )
					{
						DXST_ERRORLOGFMT_ADDUTF16LE(( "\xde\x30\xb9\x30\xaf\x30\xe6\x51\x06\x74\x28\x75\xb7\x30\xa7\x30\xfc\x30\xc0\x30\xfc\x30\xaa\x30\xd6\x30\xb8\x30\xa7\x30\xaf\x30\xc8\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�}�X�N�����p�V�F�[�_�[�I�u�W�F�N�g�̍쐬�Ɏ��s���܂���\n" @*/ )) ;
						GSYS.HardInfo.UseShader = FALSE ;
						goto ENDLABEL ;
					}

					DXA_GetFileInfo( &ShaderCode->_2D.BaseShaderBinDxa, DX_CHARCODEFORMAT_ASCII, mask_use_grhandle_reverse_filename[ i ], &Addr, &Size ) ;
					if( Direct3DDevice9_CreatePixelShader(
						( DWORD * )( ( BYTE * )DataImage + Addr ),
						&Shader->_2D.MaskEffect_UseGraphHandle_ReverseEffect_PixelShader[ i ] ) != D_D3D_OK )
					{
						DXST_ERRORLOGFMT_ADDUTF16LE(( "\xde\x30\xb9\x30\xaf\x30\xe6\x51\x06\x74\x28\x75\xb7\x30\xa7\x30\xfc\x30\xc0\x30\xfc\x30\xaa\x30\xd6\x30\xb8\x30\xa7\x30\xaf\x30\xc8\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�}�X�N�����p�V�F�[�_�[�I�u�W�F�N�g�̍쐬�Ɏ��s���܂���\n" @*/ )) ;
						GSYS.HardInfo.UseShader = FALSE ;
						goto ENDLABEL ;
					}
				}
			}
		}
#endif // DX_NON_NORMAL_DRAW_SHADER
	}

ENDLABEL:

	if( GSYS.HardInfo.UseShader == FALSE )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\xd7\x30\xed\x30\xb0\x30\xe9\x30\xde\x30\xd6\x30\xeb\x30\xb7\x30\xa7\x30\xfc\x30\xc0\x30\xfc\x30\x92\x30\x7f\x4f\x28\x75\x57\x30\x7e\x30\x5b\x30\x93\x30\x0a\x00\x00"/*@ L"�v���O���}�u���V�F�[�_�[���g�p���܂���\n" @*/ )) ;
	}
	else
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\xd7\x30\xed\x30\xb0\x30\xe9\x30\xde\x30\xd6\x30\xeb\x30\xb7\x30\xa7\x30\xfc\x30\xc0\x30\xfc\x30\x92\x30\x7f\x4f\x28\x75\x57\x30\x7e\x30\x59\x30\x0a\x00\x00"/*@ L"�v���O���}�u���V�F�[�_�[���g�p���܂�\n" @*/ )) ;
	}

	// �V�F�[�_�[�̒萔���Z�b�g
	static float ZeroOne[ 4 ]        = { 0.0f, 1.0f, 0.0f, 0.0f } ;
	static float ZeroHalfOneTwo[ 4 ] = { 0.0f, 0.5f, 1.0f, 2.0f } ;
	Graphics_D3D9_ShaderConstant_InfoSet_SetParam( &Shader->ShaderConstantInfo, DX_SHADERCONSTANTTYPE_VS_FLOAT, DX_SHADERCONSTANTSET_LIB, DX_VS_CONSTF_ZERO_ONE,          ZeroOne,        1, TRUE ) ;
	Graphics_D3D9_ShaderConstant_InfoSet_SetParam( &Shader->ShaderConstantInfo, DX_SHADERCONSTANTTYPE_PS_FLOAT, DX_SHADERCONSTANTSET_LIB, DX_PS_CONSTF_ZERO_HALF_ONE_TWO, ZeroHalfOneTwo, 1, TRUE ) ;

	// ����I��
	return TRUE ;
}


// Direct3D9 �̃V�F�[�_�[�̌�n��������
extern int Graphics_D3D9_Shader_Terminate( void )
{
	GRAPHICS_HARDDATA_DIRECT3D9_DEVICE *Device = &GraphicsHardDataDirect3D9.Device ;
	GRAPHICS_HARDWARE_DIRECT3D9_SHADER *Shader = &Device->Shader ;
	D_IDirect3DVertexShader9           **VS = NULL ;
	D_IDirect3DPixelShader9            **PS = NULL ;
	int                                i = 0 ;

	if( GAPIWin.Direct3DDevice9Object == NULL )
	{
		return 0 ;
	}

	Graphics_D3D9_DeviceState_ResetVertexShader() ;
	Graphics_D3D9_DeviceState_ResetPixelShader() ;

#ifndef DX_NON_NORMAL_DRAW_SHADER

//	if( Shader->_2D.Base2DVertexShader )
//	{
//		Direct3D9_ObjectRelease( Shader->_2D.Base2DVertexShader ) ;
//		Shader->_2D.Base2DVertexShader = NULL ;
//	}

//	if( Shader->_2D.Base3DVertexShader )
//	{
//		Direct3D9_ObjectRelease( Shader->_2D.Base3DVertexShader ) ;
//		Shader->_2D.Base3DVertexShader = NULL ;
//	}

	PS = ( D_IDirect3DPixelShader9 ** )Shader->_2D.BaseNoneTexPixelShader ;
	for( i = 0 ; i < sizeof( Shader->_2D.BaseNoneTexPixelShader ) / sizeof( D_IDirect3DPixelShader9 * ) ; i ++ )
	{
		if( PS[ i ] )
		{
			Direct3D9_ObjectRelease( PS[ i ] ) ;
			PS[ i ] = NULL ;
		}
	}

	PS = ( D_IDirect3DPixelShader9 ** )Shader->_2D.BaseUseTexPixelShader ;
	for( i = 0 ; i < sizeof( Shader->_2D.BaseUseTexPixelShader ) / sizeof( D_IDirect3DPixelShader9 * ) ; i ++ )
	{
		if( PS[ i ] )
		{
			Direct3D9_ObjectRelease( PS[ i ] ) ;
			PS[ i ] = NULL ;
		}
	}

	if( Shader->_2D.MaskEffectPixelShader )
	{
		Direct3D9_ObjectRelease( Shader->_2D.MaskEffectPixelShader ) ;
		Shader->_2D.MaskEffectPixelShader = NULL ;
	}

	for( i = 0 ; i < 4 ; i ++ )
	{
		if( Shader->_2D.MaskEffect_UseGraphHandle_PixelShader[ i ] )
		{
			Direct3D9_ObjectRelease( Shader->_2D.MaskEffect_UseGraphHandle_PixelShader[ i ] ) ;
			Shader->_2D.MaskEffect_UseGraphHandle_PixelShader[ i ] = NULL ;
		}

		if( Shader->_2D.MaskEffect_UseGraphHandle_ReverseEffect_PixelShader[ i ] )
		{
			Direct3D9_ObjectRelease( Shader->_2D.MaskEffect_UseGraphHandle_ReverseEffect_PixelShader[ i ] ) ;
			Shader->_2D.MaskEffect_UseGraphHandle_ReverseEffect_PixelShader[ i ] = NULL ;
		}
	}

#endif // DX_NON_NORMAL_DRAW_SHADER

#ifndef DX_NON_MODEL

	VS = ( D_IDirect3DVertexShader9 ** )Shader->Model.MV1_PixelLighting_VertexShader ;
	for( i = 0 ; i < sizeof( Shader->Model.MV1_PixelLighting_VertexShader ) / sizeof( D_IDirect3DVertexShader9 * ) ; i ++ )
	{
		if( VS[ i ] )
		{
			Direct3D9_ObjectRelease( VS[ i ] ) ;
			VS[ i ] = NULL ;
		}
	}

	PS = ( D_IDirect3DPixelShader9 ** )Shader->Model.MV1_PixelLighting_ToonPixelShader ;
	for( i = 0 ; i < sizeof( Shader->Model.MV1_PixelLighting_ToonPixelShader ) / sizeof( D_IDirect3DPixelShader9 * ) ; i ++ )
	{
		if( PS[ i ] )
		{
			Direct3D9_ObjectRelease( PS[ i ] ) ;
			PS[ i ] = NULL ;
		}
	}

	PS = ( D_IDirect3DPixelShader9 ** )Shader->Model.MV1_PixelLighting_PixelShader ;
	for( i = 0 ; i < sizeof( Shader->Model.MV1_PixelLighting_PixelShader ) / sizeof( D_IDirect3DPixelShader9 * ) ; i ++ )
	{
		if( PS[ i ] )
		{
			Direct3D9_ObjectRelease( PS[ i ] ) ;
			PS[ i ] = NULL ;
		}
	}








	VS = ( D_IDirect3DVertexShader9 ** )Shader->Model.MV1_ToonOutLineVertexShader ;
	for( i = 0 ; i < sizeof( Shader->Model.MV1_ToonOutLineVertexShader ) / sizeof( D_IDirect3DVertexShader9 * ) ; i ++ )
	{
		if( VS[ i ] )
		{
			Direct3D9_ObjectRelease( VS[ i ] ) ;
			VS[ i ] = NULL ;
		}
	}

	VS = ( D_IDirect3DVertexShader9 ** )Shader->Model.MV1_ShadowMapVertexShader ;
	for( i = 0 ; i < sizeof( Shader->Model.MV1_ShadowMapVertexShader ) / sizeof( D_IDirect3DVertexShader9 * ) ; i ++ )
	{
		if( VS[ i ] )
		{
			Direct3D9_ObjectRelease( VS[ i ] ) ;
			VS[ i ] = NULL ;
		}
	}

	VS = ( D_IDirect3DVertexShader9 ** )Shader->Model.MV1_NoLightingVertexShader ;
	for( i = 0 ; i < sizeof( Shader->Model.MV1_NoLightingVertexShader ) / sizeof( D_IDirect3DVertexShader9 * ) ; i ++ )
	{
		if( VS[ i ] )
		{
			Direct3D9_ObjectRelease( VS[ i ] ) ;
			VS[ i ] = NULL ;
		}
	}

	VS = ( D_IDirect3DVertexShader9 ** )Shader->Model.MV1_LightingVertexShader ;
	for( i = 0 ; i < sizeof( Shader->Model.MV1_LightingVertexShader ) / sizeof( D_IDirect3DVertexShader9 * ) ; i ++ )
	{
		if( VS[ i ] )
		{
			Direct3D9_ObjectRelease( VS[ i ] ) ;
			VS[ i ] = NULL ;
		}
	}

	PS = ( D_IDirect3DPixelShader9 ** )&Shader->Model.MV1_ToonOutLinePixelShader ;
	for( i = 0 ; i < sizeof( Shader->Model.MV1_ToonOutLinePixelShader ) / sizeof( D_IDirect3DPixelShader9 * ) ; i ++ )
	{
		if( PS[ i ] )
		{
			Direct3D9_ObjectRelease( PS[ i ] ) ;
			PS[ i ] = NULL ;
		}
	}

	PS = ( D_IDirect3DPixelShader9 ** )Shader->Model.MV1_ShadowMapToonPixelShader ;
	for( i = 0 ; i < sizeof( Shader->Model.MV1_ShadowMapToonPixelShader ) / sizeof( D_IDirect3DPixelShader9 * ) ; i ++ )
	{
		if( PS[ i ] )
		{
			Direct3D9_ObjectRelease( PS[ i ] ) ;
			PS[ i ] = NULL ;
		}
	}

	PS = ( D_IDirect3DPixelShader9 ** )&Shader->Model.MV1_ShadowMapPixelShader ;
	for( i = 0 ; i < sizeof( Shader->Model.MV1_ShadowMapPixelShader ) / sizeof( D_IDirect3DPixelShader9 * ) ; i ++ )
	{
		if( PS[ i ] )
		{
			Direct3D9_ObjectRelease( PS[ i ] ) ;
			PS[ i ] = NULL ;
		}
	}

	PS = ( D_IDirect3DPixelShader9 ** )Shader->Model.MV1_ToonNoLightingPixelShader ;
	for( i = 0 ; i < sizeof( Shader->Model.MV1_ToonNoLightingPixelShader ) / sizeof( D_IDirect3DPixelShader9 * ) ; i ++ )
	{
		if( PS[ i ] )
		{
			Direct3D9_ObjectRelease( PS[ i ] ) ;
			PS[ i ] = NULL ;
		}
	}

	PS = ( D_IDirect3DPixelShader9 ** )Shader->Model.MV1_NoLightingPixelShader ;
	for( i = 0 ; i < sizeof( Shader->Model.MV1_NoLightingPixelShader ) / sizeof( D_IDirect3DPixelShader9 * ) ; i ++ )
	{
		if( PS[ i ] )
		{
			Direct3D9_ObjectRelease( PS[ i ] ) ;
			PS[ i ] = NULL ;
		}
	}

	PS = ( D_IDirect3DPixelShader9 ** )Shader->Model.MV1_ToonLightingPixelShader ;
	for( i = 0 ; i < sizeof( Shader->Model.MV1_ToonLightingPixelShader ) / sizeof( D_IDirect3DPixelShader9 * ) ; i ++ )
	{
		if( PS[ i ] )
		{
			Direct3D9_ObjectRelease( PS[ i ] ) ;
			PS[ i ] = NULL ;
		}
	}

	PS = ( D_IDirect3DPixelShader9 ** )Shader->Model.MV1_LightingPixelShader ;
	for( i = 0 ; i < sizeof( Shader->Model.MV1_LightingPixelShader ) / sizeof( D_IDirect3DPixelShader9 * ) ; i ++ )
	{
		if( PS[ i ] )
		{
			Direct3D9_ObjectRelease( PS[ i ] ) ;
			PS[ i ] = NULL ;
		}
	}

#endif // DX_NON_MODEL

	// ����I��
	return 0 ;
}








#ifndef DX_NON_MODEL

// �w��̃��f���`��p�V�F�[�_�[���Z�b�g�A�b�v����( TRUE:����  FALSE:���s )
extern int Graphics_D3D9_Shader_Model_Setup(
	int VertexShaderIndex,
	int VertexShaderIndex_PL,
	int PixelShaderIndex,
	int PixelShaderIndex_PL )
{
	GRAPHICS_HARDDATA_DIRECT3D9_DEVICE           *Device = &GraphicsHardDataDirect3D9.Device ;
	GRAPHICS_HARDWARE_DIRECT3D9_SHADER           *Shader = &Device->Shader ;
	GRAPHICS_HARDWARE_DIRECT3D9_SHADER_MODEL     *SM     = &Shader->Model ;
	GRAPHICS_HARDWARE_DIRECT3D9_SHADERCODE_MODEL *SCM    = &GraphicsHardDataDirect3D9.ShaderCode.Model ;
	int                        ValidPL ;
	D_IDirect3DVertexShader9 **VS_PL        = NULL ;
	void                     **VSAddress_PL = NULL ;
	D_IDirect3DPixelShader9  **PS_PL        = NULL ;
	void                     **PSAddress_PL = NULL ;
	D_IDirect3DVertexShader9 **VS           = NULL ;
	void                     **VSAddress    = NULL ;
	D_IDirect3DPixelShader9  **PS           = NULL ;
	void                     **PSAddress    = NULL ;
	int                        PS_Type_PL = 0 ;
	int                        PS_ShadowMap_PL = 0 ;
	int                        PS_MultiTexBlendMode_PL = 0 ;
	int                        PS_ToonType_PL = 0 ;
	int                        PS_ToonSphereOP_PL = 0 ;
	int                        PS_ToonDiffuseBlendOP_PL = 0 ;
	int                        PS_ToonSpecularBlendOP_PL = 0 ;
	int                        PS_SpecularMap_PL = 0 ;
	int                        PS_BumpMap_PL = 0 ;
	int                        PS_LightIndex_PL = 0 ;
	int                        PS_Specular_PL = 0 ;
	int                        VS_Type = 0 ;
	int                        VS_ShaderModel = 0 ;
	int                        VS_ShadowMap = 0 ;
	int                        VS_MeshType = 0 ;
	int                        VS_BumpMap = 0 ;
	int                        VS_FogMode = 0 ;
	int                        VS_LightIndex = 0 ;
	int                        VS_Specular = 0 ;
	int                        PS_Type = 0 ;
	int                        PS_ShaderModel = 0 ;
	int                        PS_ShadowMap = 0 ;
	int                        PS_MultiTexBlendMode = 0 ;
	int                        PS_Toon = 0 ;
	int                        PS_ToonType = 0 ;
	int                        PS_ToonSphereOP = 0 ;
	int                        PS_ToonDiffuseBlendOP = 0 ;
	int                        PS_ToonSpecularBlendOP = 0 ;
	int                        PS_SpecularMap = 0 ;
	int                        PS_BumpMap = 0 ;
	int                        PS_LightIndex = 0 ;
	int                        PS_Specular = 0 ;
//	int                        Time = NS_GetNowCount() ;

	if( 0 )
//	if( Time % 1000 < 500 )
	{
		GSYS.HardInfo.UseShader = FALSE ;
		GD3D9.Device.Caps.ValidTexTempRegFlag = FALSE ;
		return FALSE ;
	}
/*	else
	{
		GSYS.HardInfo.UseShader = TRUE ;
	}
*/

	// �V�F�[�_���g���Ȃ��ꍇ�͎��s
	if( GSYS.HardInfo.UseShader == FALSE )
	{
		return FALSE ;
	}

	if( VertexShaderIndex >= 0 )
	{
		VS_PL        = &( ( D_IDirect3DVertexShader9 ** )SM->MV1_PixelLighting_VertexShader         )[ VertexShaderIndex_PL ] ;
		VSAddress_PL = &( ( void **                     )SCM->MV1_PixelLighting_VertexShaderAddress )[ VertexShaderIndex_PL ] ;
	}

	if( PixelShaderIndex >= 0 )
	{
		PS_Type_PL        = D3D9_PIXELLIGHTING_PIXELSHADER_GET_TYPE(        PixelShaderIndex_PL ) ;
		PS_ShadowMap_PL   = D3D9_PIXELLIGHTING_PIXELSHADER_GET_SHADOWMAP(   PixelShaderIndex_PL ) ;
		PS_SpecularMap_PL = D3D9_PIXELLIGHTING_PIXELSHADER_GET_SPECULARMAP( PixelShaderIndex_PL ) ;
		PS_BumpMap_PL     = D3D9_PIXELLIGHTING_PIXELSHADER_GET_BUMPMAP(     PixelShaderIndex_PL ) ;
		PS_LightIndex_PL  = D3D9_PIXELLIGHTING_PIXELSHADER_GET_LIGHTINDEX(  PixelShaderIndex_PL ) ;
		PS_Specular_PL    = D3D9_PIXELLIGHTING_PIXELSHADER_GET_SPECULAR(    PixelShaderIndex_PL ) ;
		switch( PS_Type_PL )
		{
		case D3D9_PIXELLIGHTING_PIXELSHADER_TYPE_TOON :
			PS_ToonType_PL            = D3D9_PIXELLIGHTING_PIXELSHADER_GET_TOONTYPE(     PixelShaderIndex_PL ) ;
			PS_ToonSphereOP_PL        = D3D9_PIXELLIGHTING_PIXELSHADER_GET_TOONSPHEREOP( PixelShaderIndex_PL ) ;
			PS_ToonDiffuseBlendOP_PL  = D3D9_PIXELLIGHTING_PIXELSHADER_GET_TOONDIFBLDOP( PixelShaderIndex_PL ) ;
			PS_ToonSpecularBlendOP_PL = D3D9_PIXELLIGHTING_PIXELSHADER_GET_TOONSPCBLDOP( PixelShaderIndex_PL ) ;

			PS_PL        =  &SM->MV1_PixelLighting_ToonPixelShader       [ PS_ShadowMap_PL ][ PS_ToonType_PL ][ PS_ToonSphereOP_PL ][ PS_ToonDiffuseBlendOP_PL ][ PS_ToonSpecularBlendOP_PL ][ PS_SpecularMap_PL ][ PS_BumpMap_PL ][ PS_LightIndex_PL ][ PS_Specular_PL ] ;
			PSAddress_PL = &SCM->MV1_PixelLighting_ToonPixelShaderAddress[ PS_ShadowMap_PL ][ PS_ToonType_PL ][ PS_ToonSphereOP_PL ][ PS_ToonDiffuseBlendOP_PL ][ PS_ToonSpecularBlendOP_PL ][ PS_SpecularMap_PL ][ PS_BumpMap_PL ][ PS_LightIndex_PL ][ PS_Specular_PL ] ;
			break ;

		case D3D9_PIXELLIGHTING_PIXELSHADER_TYPE_NORMAL :
			PS_MultiTexBlendMode_PL = D3D9_PIXELLIGHTING_PIXELSHADER_GET_MULTITEX( PixelShaderIndex_PL ) ;

			PS_PL        = & SM->MV1_PixelLighting_PixelShader       [ PS_ShadowMap_PL ][ PS_MultiTexBlendMode_PL ][ PS_SpecularMap_PL ][ PS_BumpMap_PL ][ PS_LightIndex_PL ][ PS_Specular_PL ] ;
			PSAddress_PL = &SCM->MV1_PixelLighting_PixelShaderAddress[ PS_ShadowMap_PL ][ PS_MultiTexBlendMode_PL ][ PS_SpecularMap_PL ][ PS_BumpMap_PL ][ PS_LightIndex_PL ][ PS_Specular_PL ] ;
			break ;
		}
	}

	if( VertexShaderIndex >= 0 )
	{
		VS_Type     = D3D9_VERTEXSHADER_GET_TYPE(        VertexShaderIndex ) ;
		VS_MeshType = D3D9_VERTEXSHADER_GET_MESHTYPE(    VertexShaderIndex ) ;
		VS_FogMode  = D3D9_VERTEXSHADER_GET_FOGMODE(     VertexShaderIndex ) ;
		switch( VS_Type )
		{
		case D3D9_VERTEXSHADER_TYPE_NORMAL :
			VS_LightIndex  = D3D9_VERTEXSHADER_GET_LIGHTINDEX(  VertexShaderIndex ) ;
			if( VS_LightIndex == 0 )
			{
				VS        =  &SM->MV1_NoLightingVertexShader       [ VS_MeshType ][ VS_FogMode ] ;
				VSAddress = &SCM->MV1_NoLightingVertexShaderAddress[ VS_MeshType ][ VS_FogMode ] ;
			}
			else
			{
				VS_ShaderModel = D3D9_VERTEXSHADER_GET_SHADERMODEL( VertexShaderIndex ) ;
				VS_ShadowMap   = D3D9_VERTEXSHADER_GET_SHADOWMAP(   VertexShaderIndex ) ;
				VS_BumpMap     = D3D9_VERTEXSHADER_GET_BUMPMAP(     VertexShaderIndex ) ;
				VS_Specular    = D3D9_VERTEXSHADER_GET_SPECULAR(    VertexShaderIndex ) ;

				if( Shader->ValidVertexShader_SM3 )
				{
					VS_ShaderModel = 1 ;
				}

				VS        = & SM->MV1_LightingVertexShader       [ VS_ShaderModel ][ VS_ShadowMap ][ VS_MeshType ][ VS_BumpMap ][ VS_FogMode ][ VS_LightIndex ][ VS_Specular ] ;
				VSAddress = &SCM->MV1_LightingVertexShaderAddress[ VS_ShaderModel ][ VS_ShadowMap ][ VS_MeshType ][ VS_BumpMap ][ VS_FogMode ][ VS_LightIndex ][ VS_Specular ] ;
			}
			break ;

		case D3D9_VERTEXSHADER_TYPE_DRAW_SHADOWMAP :
			VS        =  &SM->MV1_ShadowMapVertexShader       [ VS_MeshType ] ;
			VSAddress = &SCM->MV1_ShadowMapVertexShaderAddress[ VS_MeshType ] ;
			break ;

		case D3D9_VERTEXSHADER_TYPE_TOON_OUTLINE :
			VS        =  &SM->MV1_ToonOutLineVertexShader       [ VS_MeshType ][ VS_FogMode ] ;
			VSAddress = &SCM->MV1_ToonOutLineVertexShaderAddress[ VS_MeshType ][ VS_FogMode ] ;
			break ;
		}
	}

	if( PixelShaderIndex >= 0 )
	{
		PS_Type        = D3D9_PIXELSHADER_GET_TYPE(        PixelShaderIndex ) ;
		PS_ShaderModel = D3D9_PIXELSHADER_GET_SHADERMODEL( PixelShaderIndex ) ;
		PS_ShadowMap   = D3D9_PIXELSHADER_GET_SHADOWMAP(   PixelShaderIndex ) ;
		PS_SpecularMap = D3D9_PIXELSHADER_GET_SPECULARMAP( PixelShaderIndex ) ;
		PS_BumpMap     = D3D9_PIXELSHADER_GET_BUMPMAP(     PixelShaderIndex ) ;
		PS_LightIndex  = D3D9_PIXELSHADER_GET_LIGHTINDEX(  PixelShaderIndex ) ;
		PS_Specular    = D3D9_PIXELSHADER_GET_SPECULAR(    PixelShaderIndex ) ;

		if( Shader->ValidPixelShader_SM3 )
		{
			PS_ShaderModel = 1 ;
		}

		switch( PS_Type )
		{
		case D3D9_PIXELSHADER_TYPE_NORMAL :
			PS_Toon = D3D9_PIXELSHADER_GET_TOON( PixelShaderIndex ) ;
			if( PS_Toon )
			{
				PS_ToonType            = D3D9_PIXELSHADER_GET_TOONTYPE(     PixelShaderIndex ) ;
				PS_ToonDiffuseBlendOP  = D3D9_PIXELSHADER_GET_TOONDIFBLDOP( PixelShaderIndex ) ;
				if( PS_LightIndex == 0 )
				{
					PS        =  &SM->MV1_ToonNoLightingPixelShader       [ PS_ToonType ][ PS_ToonDiffuseBlendOP ] ;
					PSAddress = &SCM->MV1_ToonNoLightingPixelShaderAddress[ PS_ToonType ][ PS_ToonDiffuseBlendOP ] ;
				}
				else
				{
					PS_ToonSphereOP        = D3D9_PIXELSHADER_GET_TOONSPHEREOP( PixelShaderIndex ) ;
					PS_ToonSpecularBlendOP = D3D9_PIXELSHADER_GET_TOONSPCBLDOP( PixelShaderIndex ) ;

					PS        =  &SM->MV1_ToonLightingPixelShader       [ PS_ShaderModel ][ PS_ShadowMap ][ PS_ToonType ][ PS_ToonSphereOP ][ PS_ToonDiffuseBlendOP ][ PS_ToonSpecularBlendOP ][ PS_SpecularMap ][ PS_BumpMap ][ PS_LightIndex ][ PS_Specular ] ;
					PSAddress = &SCM->MV1_ToonLightingPixelShaderAddress[ PS_ShaderModel ][ PS_ShadowMap ][ PS_ToonType ][ PS_ToonSphereOP ][ PS_ToonDiffuseBlendOP ][ PS_ToonSpecularBlendOP ][ PS_SpecularMap ][ PS_BumpMap ][ PS_LightIndex ][ PS_Specular ] ;
				}
			}
			else
			{
				PS_MultiTexBlendMode = D3D9_PIXELSHADER_GET_MULTITEX( PixelShaderIndex ) ;
				if( PS_LightIndex == 0 )
				{
					PS        =  &SM->MV1_NoLightingPixelShader       [ PS_MultiTexBlendMode ] ;
					PSAddress = &SCM->MV1_NoLightingPixelShaderAddress[ PS_MultiTexBlendMode ] ;
				}
				else
				{
					PS        =  &SM->MV1_LightingPixelShader       [ PS_ShaderModel ][ PS_ShadowMap ][ PS_MultiTexBlendMode ][ PS_SpecularMap ][ PS_BumpMap ][ PS_LightIndex ][ PS_Specular ] ;
					PSAddress = &SCM->MV1_LightingPixelShaderAddress[ PS_ShaderModel ][ PS_ShadowMap ][ PS_MultiTexBlendMode ][ PS_SpecularMap ][ PS_BumpMap ][ PS_LightIndex ][ PS_Specular ] ;
				}
			}
			break ;

		case D3D9_PIXELSHADER_TYPE_DRAW_SHADOWMAP :
			PS_Toon = D3D9_PIXELSHADER_GET_TOON( PixelShaderIndex ) ;
			if( PS_Toon )
			{
				PS_ToonType            = D3D9_PIXELSHADER_GET_TOONTYPE(     PixelShaderIndex ) ;
				PS_ToonSphereOP        = D3D9_PIXELSHADER_GET_TOONSPHEREOP( PixelShaderIndex ) ;
				PS_ToonDiffuseBlendOP  = D3D9_PIXELSHADER_GET_TOONDIFBLDOP( PixelShaderIndex ) ;

				PS        =  &SM->MV1_ShadowMapToonPixelShader       [ PS_ToonType ][ PS_ToonSphereOP ][ PS_ToonDiffuseBlendOP ] ;
				PSAddress = &SCM->MV1_ShadowMapToonPixelShaderAddress[ PS_ToonType ][ PS_ToonSphereOP ][ PS_ToonDiffuseBlendOP ] ;
			}
			else
			{
				PS        =  &SM->MV1_ShadowMapPixelShader ;
				PSAddress = &SCM->MV1_ShadowMapPixelShaderAddress ;
			}
			break ;

		case D3D9_PIXELSHADER_TYPE_TOON_OUTLINE :
			PS        =  &SM->MV1_ToonOutLinePixelShader ;
			PSAddress = &SCM->MV1_ToonOutLinePixelShaderAddress ;
			break ;
		}
	}

	ValidPL = 
		VertexShaderIndex >= 0 &&
		Shader->ValidVertexShader_SM3 &&
		VS_Type == D3D9_VERTEXSHADER_TYPE_NORMAL &&
		VS_LightIndex != 0 &&
		*VSAddress_PL != NULL &&

		PixelShaderIndex >= 0 &&
		Shader->ValidPixelShader_SM3 &&
		PS_Type == D3D9_PIXELSHADER_TYPE_NORMAL &&
		PS_LightIndex != 0 &&
		*PSAddress_PL != NULL
	;

//	void *Addr1 = *VSAddress_PL ;
//	void *Addr2 = *PSAddress_PL ;

	if( ( Shader->UseOnlyPixelLightingTypeCode || GD3D9.UsePixelLightingShader ) && ValidPL )
	{
SHADER_MODEL_3:
		if( VertexShaderIndex >= 0 )
		{
			// �V�F�[�_�[�����邩�ǂ����𒲂ׂ�
			if( *VS_PL == NULL )
			{
				// �V�F�[�_�[�̍쐬�����݂�
				if( Direct3DDevice9_CreateVertexShader( ( DWORD * )*VSAddress_PL, VS_PL ) != D_D3D_OK )
					return FALSE ;
			}

			// �o�[�e�b�N�X�V�F�[�_�[�̃Z�b�g�A�b�v
			Graphics_D3D9_DeviceState_SetVertexShader( *VS_PL ) ;
		}

		if( PixelShaderIndex >= 0 )
		{
			// �V�F�[�_�[�����邩�ǂ����𒲂ׂ�
			if( *PS_PL == NULL )
			{
				// �V�F�[�_�[�̍쐬�����݂�
				if( Direct3DDevice9_CreatePixelShader( ( DWORD * )*PSAddress_PL, PS_PL ) != D_D3D_OK )
					return FALSE ;
			}

			// �s�N�Z���V�F�[�_�[�̃Z�b�g�A�b�v
			Graphics_D3D9_DeviceState_SetPixelShader( *PS_PL ) ;
		}
	}
	else
	{
		if( VertexShaderIndex >= 0 )
		{
			// �V�F�[�_�[�����邩�ǂ����𒲂ׂ�
			if( *VS == NULL )
			{
				// �V�F�[�_�[�̍쐬�����݂�
				if( *VSAddress == NULL ||
					Direct3DDevice9_CreateVertexShader( ( DWORD * )*VSAddress, VS ) != D_D3D_OK )
				{
					if( ValidPL )
					{
						goto SHADER_MODEL_3 ;
					}
					return FALSE ;
				}
			}

			// �o�[�e�b�N�X�V�F�[�_�[�̃Z�b�g�A�b�v
		#ifdef DXLIB_DEBUG
			Graphics_D3D9_DeviceState_SetVertexShader( MV1Man.VS_Test ) ;
			Graphics_D3D9_DeviceState_SetVertexDeclaration( MV1Man.VertexDeclaration[ 1 ][ 0 ][ 1 ] ) ;
		#else
			Graphics_D3D9_DeviceState_SetVertexShader( *VS ) ;
		#endif
		}

		if( PixelShaderIndex >= 0 )
		{
			// �V�F�[�_�[�����邩�ǂ����𒲂ׂ�
			if( *PS == NULL )
			{
				// �V�F�[�_�[�̍쐬�����݂�
				if( *PSAddress == NULL ||
					Direct3DDevice9_CreatePixelShader( ( DWORD * )*PSAddress, PS ) != D_D3D_OK )
				{
					if( ValidPL )
					{
						goto SHADER_MODEL_3 ;
					}

					return FALSE ;
				}
			}

			// �s�N�Z���V�F�[�_�[�̃Z�b�g�A�b�v
		#ifdef DXLIB_DEBUG
			Graphics_D3D9_DeviceState_SetPixelShader( MV1Man.PS_Test ) ;
		#else
			Graphics_D3D9_DeviceState_SetPixelShader( *PS ) ;
		#endif
		}
	}


	// �I��
	return TRUE ;
}

#endif DX_NON_MODEL











// �g�p����V�F�[�_�[�̃C���f�b�N�X�̒l���X�V����
extern void Graphics_D3D9_Shader_RefreshUseIndex( void )
{
#ifndef DX_NON_MODEL
	GRAPHICS_HARDWARE_DIRECT3D9_SHADER *Shader = &GraphicsHardDataDirect3D9.Device.Shader ;
	int LightMode[ DX_PS_CONSTB_LIGHT2_NUM ] ;
	int PhongValid ;
	int i ;
	int UseShadowMap ;

	if( GSYS.HardInfo.UseShader == FALSE )
	{
		return ;
	}

	PhongValid     = 0 ;
	LightMode[ 0 ] = 0 ;
	LightMode[ 1 ] = 0 ;
	LightMode[ 2 ] = 0 ;
	LightMode[ 3 ] = 0 ;
	LightMode[ 4 ] = 0 ;
	LightMode[ 5 ] = 0 ;
	Shader->UseOnlyPixelLightingTypeCode = FALSE ;
	if( GD3D9.Device.State.Lighting )
	{
		for( i = 0 ; i < DX_PS_CONSTB_LIGHT2_NUM ; i ++ )
		{
			if( GD3D9.Device.State.LightEnableFlag[ i ] )
			{
				LightMode[ i ] = GD3D9.Device.State.LightParam[ i ].Type ;
				PhongValid = 1 ;

				// ���_�P�ʃ��C�e�B���O�ł̓��C�g�͂R�܂ł����Ή����Ă��Ȃ�
				if( i >= 3 )
				{
					Shader->UseOnlyPixelLightingTypeCode = TRUE ;
				}
			}
		}
	}

	UseShadowMap = FALSE ;
	for( i = 0 ; i < MAX_USE_SHADOWMAP_NUM ; i ++ )
	{
		if( GSYS.DrawSetting.ShadowMap[ i ] > 0 )
		{
			UseShadowMap = TRUE ;
			break ;
		}
	}

	Shader->UseBaseVertexShaderIndex =
		D3D9_VERTEXSHADER_TYPE( GSYS.DrawSetting.ShadowMapDraw ? D3D9_VERTEXSHADER_TYPE_DRAW_SHADOWMAP : D3D9_VERTEXSHADER_TYPE_NORMAL ) +
		D3D9_VERTEXSHADER_SHADOWMAP( UseShadowMap ? 1 : 0 ) +
		D3D9_VERTEXSHADER_FOGMODE( GD3D9.Device.State.FogEnable ? GD3D9.Device.State.FogMode : DX_FOGMODE_NONE ) +
		D3D9_VERTEXSHADER_LIGHTINDEX( GD3D9.ShaderCode.Model.LightIndexList20[ LightMode[ 0 ] ][ LightMode[ 1 ] ][ LightMode[ 2 ] ] )
	;

	Shader->UseBasePixelShaderIndex =
		D3D9_PIXELSHADER_TYPE( GSYS.DrawSetting.ShadowMapDraw ? D3D9_PIXELSHADER_TYPE_DRAW_SHADOWMAP : D3D9_PIXELSHADER_TYPE_NORMAL ) +
		D3D9_PIXELSHADER_SHADOWMAP( UseShadowMap ? 1 : 0 ) +
		D3D9_PIXELSHADER_LIGHTINDEX( 
			GD3D9.ShaderCode.Model.LightIndexList10
				[ LightMode[ 0 ] == DX_LIGHTTYPE_D3DLIGHT_DIRECTIONAL ? 2 : ( LightMode[ 0 ] ? 1 : 0 ) ]
				[ LightMode[ 1 ] == DX_LIGHTTYPE_D3DLIGHT_DIRECTIONAL ? 2 : ( LightMode[ 1 ] ? 1 : 0 ) ]
				[ LightMode[ 2 ] == DX_LIGHTTYPE_D3DLIGHT_DIRECTIONAL ? 2 : ( LightMode[ 2 ] ? 1 : 0 ) ] 
		)
	;

	Shader->UseBaseVertexShaderIndex_PL =
		D3D9_PIXELLIGHTING_VERTEXSHADER_FOGMODE( GD3D9.Device.State.FogEnable ? GD3D9.Device.State.FogMode : DX_FOGMODE_NONE ) +
		D3D9_PIXELLIGHTING_VERTEXSHADER_SHADOWMAP( UseShadowMap ? 1 : 0 )
	;

	Shader->UseBasePixelShaderIndex_PL =
		D3D9_PIXELLIGHTING_PIXELSHADER_LIGHTINDEX( GD3D9.ShaderCode.Model.LightIndexList84[ LightMode[ 0 ] ][ LightMode[ 1 ] ][ LightMode[ 2 ] ][ LightMode[ 3 ] ][ LightMode[ 4 ] ][ LightMode[ 5 ] ] ) +
		D3D9_PIXELLIGHTING_PIXELSHADER_SHADOWMAP( UseShadowMap ? 1 : 0 )
	;
#endif // DX_NON_MODEL
}

// �V�F�[�_�[�̒萔���𓾂�
extern	D_D3DXSHADER_CONSTANTINFO *Graphics_D3D9_GetShaderConstInfo( SHADERHANDLEDATA *Shader, const wchar_t *ConstantName )
{
	D_D3DXSHADER_CONSTANTINFO *Info ;
	int i ;

	// �w��̖��O�����萔��{��
	Info = Shader->PF->D3D9.ConstantInfo ;

	char ConstantNameA[ 512 ] ;

	ConvString( ( const char * )ConstantName, WCHAR_T_CHARCODEFORMAT, ConstantNameA, sizeof( ConstantNameA ), DX_CHARCODEFORMAT_ASCII ) ;
	for( i = 0 ; i < Shader->PF->D3D9.ConstantNum ; i ++, Info ++ )
	{
		if( _STRCMP( ConstantNameA, ( char * )Shader->FunctionCode + Info->Name + 12 ) == 0 )
			break ;
	}

	if( i == Shader->PF->D3D9.ConstantNum )
		return NULL ;

	// ����Ԃ�
	return Info ;
}





















// ���_�o�b�t�@�E�C���f�b�N�X�o�b�t�@�֌W

// ���_�o�b�t�@�n���h���p�� Direct3DVertexBuffer9 �I�u�W�F�N�g���쐬����
extern	int		Graphics_D3D9_VertexBuffer_CreateObject( VERTEXBUFFERHANDLEDATA *VertexBuffer, int Restore )
{
	DWORD FVF ;
	int Result ;

	// ���_�������n�[�h�E�G�A�ŏ�������ꍇ�̂� Direct3D9VertexBuffer ���쐬����
	if( GD3D9.Device.Caps.VertexHardwareProcess == FALSE && GD3D9.Device.Shader.ValidVertexShader == FALSE )
	{
		return 0 ;
	}

	// �Ή����Ă��Ȃ��^�C�v�̏ꍇ�̓G���[
	FVF = VERTEXFVF_3D_LIGHT ;
	switch( VertexBuffer->Type )
	{
	case DX_VERTEX_TYPE_NORMAL_3D :
		FVF = VERTEXFVF_3D_LIGHT ;
		break ;

	case DX_VERTEX_TYPE_SHADER_3D :
		FVF = VERTEXFVF_3D_LIGHT ;
		break ;

	default :
		return -1 ;
	}

	// ���_�o�b�t�@���쐬����
	Result = Direct3DDevice9_CreateVertexBuffer_ASync( 
				( DWORD )( VertexBuffer->UnitSize * VertexBuffer->Num ),
				D_D3DUSAGE_WRITEONLY,
				FVF,
				D_D3DPOOL_DEFAULT,
				&VertexBuffer->PF->D3D9.VertexBuffer ) ;
	if( Result < 0 )
	{
		DXST_ERRORLOG_ADDUTF16LE( "\x02\x98\xb9\x70\xd0\x30\xc3\x30\xd5\x30\xa1\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"���_�o�b�t�@�̍쐬�Ɏ��s���܂���\n" @*/ ) ;
		return -1 ;
	}

	// �f�[�^�𕜋A����
	if( Restore )
	{
		NS_SetVertexBufferData( 0, VertexBuffer->Buffer, VertexBuffer->Num, VertexBuffer->HandleInfo.Handle ) ; 
	}

	// �I��
	return 0 ;
}

// ���_�o�b�t�@�n���h���p�� Direct3DVertexBuffer9 �I�u�W�F�N�g���������
extern	int		Graphics_D3D9_VertexBuffer_ReleaseObject( VERTEXBUFFERHANDLEDATA *VertexBuffer )
{
	if( VertexBuffer->PF->D3D9.VertexBuffer != NULL )
	{
		Direct3D9_ObjectRelease( VertexBuffer->PF->D3D9.VertexBuffer ) ;
		VertexBuffer->PF->D3D9.VertexBuffer = NULL ;
	}

	// �I��
	return 0 ;
}

// �C���f�b�N�X�o�b�t�@�n���h���p�� Direct3DIndexBuffer9 �I�u�W�F�N�g���쐬����
extern	int		Graphics_D3D9_IndexBuffer_CreateObject( INDEXBUFFERHANDLEDATA *IndexBuffer, int Restore )
{
	D_D3DFORMAT Format ;

	// ���_�������n�[�h�E�G�A�ŏ�������ꍇ�̂� Direct3D9IndexBuffer ���쐬����
	if( GD3D9.Device.Caps.VertexHardwareProcess == FALSE && GD3D9.Device.Shader.ValidVertexShader == FALSE )
	{
		return 0 ;
	}

	// �Ή����Ă��Ȃ��^�C�v�̏ꍇ�̓G���[
	Format = D_D3DFMT_INDEX16 ;
	switch( IndexBuffer->Type )
	{
	case DX_INDEX_TYPE_16BIT :
		Format = D_D3DFMT_INDEX16 ;
		break ;

	case DX_INDEX_TYPE_32BIT :
		Format = D_D3DFMT_INDEX32 ;
		break ;
	}

	// �C���f�b�N�X�o�b�t�@���쐬����
	if( Direct3DDevice9_CreateIndexBuffer_ASync( 
			( DWORD )( IndexBuffer->UnitSize * IndexBuffer->Num ),
			D_D3DUSAGE_WRITEONLY,
			Format,
			D_D3DPOOL_DEFAULT,
			&IndexBuffer->PF->D3D9.IndexBuffer
		) < 0 )
	{
		DXST_ERRORLOG_ADDUTF16LE( "\xa4\x30\xf3\x30\xc7\x30\xc3\x30\xaf\x30\xb9\x30\xd0\x30\xc3\x30\xd5\x30\xa1\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�C���f�b�N�X�o�b�t�@�̍쐬�Ɏ��s���܂���\n" @*/ ) ;
		return -1 ;
	}

	// �f�[�^�𕜋A����
	if( Restore )
	{
		NS_SetIndexBufferData( 0, IndexBuffer->Buffer, IndexBuffer->Num, IndexBuffer->HandleInfo.Handle ) ; 
	}

	// �I��
	return 0 ;
}

// �C���f�b�N�X�o�b�t�@�n���h���p�� Direct3DIndexBuffer9 �I�u�W�F�N�g���������
extern	int		Graphics_D3D9_IndexBuffer_ReleaseObject( INDEXBUFFERHANDLEDATA *IndexBuffer )
{
	if( IndexBuffer->PF->D3D9.IndexBuffer != NULL )
	{
		Direct3D9_ObjectRelease( IndexBuffer->PF->D3D9.IndexBuffer ) ;
		IndexBuffer->PF->D3D9.IndexBuffer = NULL ;
	}

	// �I��
	return 0 ;
}



























// Direct3D �̉�ʊ֌W

// ScreenCopy �� GetDrawScreen ���������邽�߂Ɏg�p����e���|�����o�b�t�@�̍쐬( 0:����  -1:���s )
extern	int		Graphics_D3D9_SetupSubBackBuffer( void )
{
	HRESULT hr ;

	// ���ɃT�u�o�b�N�o�b�t�@���쐬����Ă���ꍇ�͉������Ȃ�
	if( GD3D9.Device.Screen.SubBackBufferSurface != NULL ) return 0 ;

	// �f�o�C�X�������̏ꍇ�͉��������I��
	if( GAPIWin.Direct3DDevice9Object == NULL ) return -1 ;

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �`����I��点��
	Graphics_D3D9_EndScene() ;

	// �t���V�[���A���`�G�C���A�X���g�p���Ȃ��ꍇ�̓����_�����O�^�[�Q�b�g�ɂł���e�N�X�`�����쐬����
	GD3D9.Device.Screen.SubBackBufferTexture = NULL ;
	if( GSYS.Setting.FSAAMultiSampleCount == 0 && GSYS.Setting.FSAAMultiSampleQuality == 0 )
	{
		// �����_�����O�^�[�Q�b�g�ɂł���e�N�X�`���̃T�C�Y������
		if( GSYS.HardInfo.TextureSizePow2 )
		{
			for( GD3D9.Device.Screen.SubBackBufferTextureSizeX = 1 ;
				GD3D9.Device.Screen.SubBackBufferTextureSizeX < GSYS.Screen.MainScreenSizeX ;
				GD3D9.Device.Screen.SubBackBufferTextureSizeX <<= 1 ){}

			for( GD3D9.Device.Screen.SubBackBufferTextureSizeY = 1 ;
				GD3D9.Device.Screen.SubBackBufferTextureSizeY < GSYS.Screen.MainScreenSizeY ;
				GD3D9.Device.Screen.SubBackBufferTextureSizeY <<= 1 ){}
		}
		else
		{
			GD3D9.Device.Screen.SubBackBufferTextureSizeX = GSYS.Screen.MainScreenSizeX ;
			GD3D9.Device.Screen.SubBackBufferTextureSizeY = GSYS.Screen.MainScreenSizeY ;
		}

		// �����_�����O�^�[�Q�b�g�ɂł���e�N�X�`���̍쐬
		if( Direct3DDevice9_CreateTexture_ASync(
			( UINT )GD3D9.Device.Screen.SubBackBufferTextureSizeX,	// ��
			( UINT )GD3D9.Device.Screen.SubBackBufferTextureSizeY,	// ����
			1,														// MipMap ���x��
			D_D3DUSAGE_RENDERTARGET,								// �t���O
			GD3D9.Device.Caps.ScreenFormat,							// �t�H�[�}�b�g
			D_D3DPOOL_DEFAULT,										// �r�f�I�������ɔz�u����
			&GD3D9.Device.Screen.SubBackBufferTexture,				// D_IDirect3DTexture9 �̃|�C���^���󂯎��A�h���X
			NULL,																																// ��� NULL
			FALSE ) == D_D3D_OK )
		{
			// �����_�����O�^�[�Q�b�g�ɂł���e�N�X�`���̃T�[�t�F�X���擾����
			if( Direct3DTexture9_GetSurfaceLevel_ASync(
				GD3D9.Device.Screen.SubBackBufferTexture,
				0,
				&GD3D9.Device.Screen.SubBackBufferSurface,
				FALSE ) != D_D3D_OK )
			{
				DXST_ERRORLOG_ADDUTF16LE( "\xb5\x30\xd6\x30\xd0\x30\xc3\x30\xaf\x30\xd0\x30\xc3\x30\xd5\x30\xa1\x30\x28\x75\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\x6e\x30\xb5\x30\xfc\x30\xd5\x30\xa7\x30\xb9\x30\xa2\x30\xc9\x30\xec\x30\xb9\x30\x6e\x30\xd6\x53\x97\x5f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�T�u�o�b�N�o�b�t�@�p�e�N�X�`���̃T�[�t�F�X�A�h���X�̎擾�Ɏ��s���܂���\n" @*/ ) ;
				return -1 ;
			}
		}
		else
		{
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\xb5\x30\xd6\x30\xd0\x30\xc3\x30\xaf\x30\xd0\x30\xc3\x30\xd5\x30\xa1\x30\x28\x75\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x20\x00\x20\x00\x46\x00\x6f\x00\x72\x00\x6d\x00\x61\x00\x74\x00\x3a\x00\x25\x00\x64\x00\x20\x00\x20\x00\x53\x00\x69\x00\x7a\x00\x65\x00\x58\x00\x3a\x00\x25\x00\x64\x00\x20\x00\x20\x00\x53\x00\x69\x00\x7a\x00\x65\x00\x59\x00\x3a\x00\x25\x00\x64\x00\x20\x00\x0a\x00\x00"/*@ L"�T�u�o�b�N�o�b�t�@�p�e�N�X�`���̍쐬�Ɏ��s���܂���  Format:%d  SizeX:%d  SizeY:%d \n" @*/,
				GD3D9.Device.Caps.ScreenFormat, GD3D9.Device.Screen.SubBackBufferTextureSizeX, GD3D9.Device.Screen.SubBackBufferTextureSizeY )) ;
		}
	}

	// �����ɗ������_�Ń����_�����O�^�[�Q�b�g���쐬����Ă��Ȃ������烌���_�����O�^�[�Q�b�g���쐬����
	if( GD3D9.Device.Screen.SubBackBufferSurface == NULL )
	{
		// �����_�����O�^�[�Q�b�g�̍쐬
		hr = Direct3DDevice9_CreateRenderTarget(
			( UINT )GSYS.Screen.MainScreenSizeX,
			( UINT )GSYS.Screen.MainScreenSizeY,
			GD3D9.Device.Caps.ScreenFormat,
			( D_D3DMULTISAMPLE_TYPE )GSYS.Setting.FSAAMultiSampleCount,
			( DWORD )GSYS.Setting.FSAAMultiSampleQuality,
			FALSE,
			&GD3D9.Device.Screen.SubBackBufferSurface,
			NULL
		) ;

		if( FAILED( hr ) )
		{
			DXST_ERRORLOG_ADDUTF16LE( "\xb5\x30\xd6\x30\xd0\x30\xc3\x30\xaf\x30\xd0\x30\xc3\x30\xd5\x30\xa1\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�T�u�o�b�N�o�b�t�@�̍쐬�Ɏ��s���܂���\n" @*/ ) ;
			return -1 ;
		}
	}

	// �{�o�b�N�o�b�t�@�̓��e��]��
	Direct3DDevice9_StretchRect( GD3D9.Device.Screen.BackBufferSurface, NULL, GD3D9.Device.Screen.SubBackBufferSurface, NULL, D_D3DTEXF_NONE ) ;

	// �{�o�b�N�o�b�t�@�̓��e��������
	Direct3DDevice9_ColorFill( GD3D9.Device.Screen.BackBufferSurface, NULL, 0 ) ;

	// �����_�����O�^�[�Q�b�g�̕ύX
	GD3D9.Device.DrawSetting.CancelSettingEqualCheck = TRUE ;
	Graphics_D3D9_DeviceState_SetRenderTarget( GD3D9.Device.Screen.SubBackBufferSurface ) ;
	GD3D9.Device.DrawSetting.CancelSettingEqualCheck = FALSE ;

	// �I��
	return 0 ;
}

// �y�o�b�t�@�I�u�W�F�N�g�̍쐬( 0:����  -1:���s )
extern	int		Graphics_D3D9_CreateZBuffer( void )
{
	HRESULT hr ;
	DWORD w, h ;
	int depth ;
	int index ;

	if( GAPIWin.Direct3DDevice9Object == NULL ) return -1 ;

	// ���ɍ쐬�ς݂̏ꍇ�͈�x�폜����
	if( GD3D9.Device.Screen.ZBufferSurface )
	{
		Direct3D9_ObjectRelease( GD3D9.Device.Screen.ZBufferSurface ) ;
		GD3D9.Device.Screen.ZBufferSurface = NULL ;
	}

	DXST_ERRORLOG_ADDUTF16LE( "\x3a\xff\xd0\x30\xc3\x30\xd5\x30\xa1\x30\x92\x30\x5c\x4f\x10\x62\x57\x30\x7e\x30\x59\x30\x2e\x00\x2e\x00\x2e\x00\x2e\x00\x20\x00\x00"/*@ L"�y�o�b�t�@���쐬���܂�.... " @*/ ) ;

	// �y�o�b�t�@�̃T�C�Y������
	if( GD3D9.Setting.UserZBufferSizeSet == FALSE ||
		GD3D9.Device.Screen.ZBufferSizeX == 0 ||
		GD3D9.Device.Screen.ZBufferSizeY == 0 )
	{
		for( w = 1 ; w < ( DWORD )GSYS.Screen.MainScreenSizeX ; w <<= 1 ){}
		for( h = 1 ; h < ( DWORD )GSYS.Screen.MainScreenSizeY ; h <<= 1 ){}
		GD3D9.Device.Screen.ZBufferSizeX = ( int )w ;
		GD3D9.Device.Screen.ZBufferSizeY = ( int )h ;
	}
	else
	{
		w = ( DWORD )GD3D9.Device.Screen.ZBufferSizeX ;
		h = ( DWORD )GD3D9.Device.Screen.ZBufferSizeY ;
	}

	// �y�o�b�t�@�̃r�b�g�[�x������
	if( GD3D9.Setting.UserZBufferBitDepthSet == FALSE ||
		GD3D9.Device.Screen.ZBufferBitDepth == 0  )
	{
		depth = DEFAULT_ZBUFFER_BITDEPTH ;
		GD3D9.Device.Screen.ZBufferBitDepth = DEFAULT_ZBUFFER_BITDEPTH ;
	}
	else
	{
		depth = GD3D9.Device.Screen.ZBufferBitDepth ;
	}
	switch( depth )
	{
	default :
	case 16 : index = ZBUFFER_FORMAT_16BIT ; break ;
	case 24 : index = ZBUFFER_FORMAT_24BIT ; break ;
	case 32 : index = ZBUFFER_FORMAT_32BIT ; break ;
	}

	// �쐬
	hr = Direct3DDevice9_CreateDepthStencilSurface(
		w,
		h,
		GD3D9.Device.Caps.ZBufferFormat[ index ],
		( D_D3DMULTISAMPLE_TYPE )GSYS.Setting.FSAAMultiSampleCount,
		( DWORD )GSYS.Setting.FSAAMultiSampleQuality,
		TRUE,
		&GD3D9.Device.Screen.ZBufferSurface,
		NULL
	) ;
	if( FAILED( hr ) )
	{
		DXST_ERRORLOG_ADDUTF16LE( "\x3a\xff\xd0\x30\xc3\x30\xd5\x30\xa1\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�y�o�b�t�@�̍쐬�Ɏ��s���܂���\n" @*/ ) ;
		return -1 ;
	}

	// �f�o�C�X�ɃZ�b�g
	Direct3DDevice9_SetDepthStencilSurface( GD3D9.Device.Screen.ZBufferSurface ) ;

	// �N���A
	Direct3DDevice9_Clear( 0, NULL, D_D3DCLEAR_ZBUFFER, 0, 1.0f, 0 ) ;

	DXST_ERRORLOG_ADDUTF16LE( "\x10\x62\x9f\x52\x0a\x00\x00"/*@ L"����\n" @*/ ) ;

	// �I��
	return 0 ;
}



















// Direct3D9 �̉摜�֌W


// D3DFORMAT �̃t�H�[�}�b�g�ɍ��킹���J���[�t�H�[�}�b�g�����擾����
extern COLORDATA *Graphics_D3D9_GetD3DFormatColorData( D_D3DFORMAT Format )
{
	static int InitializeFlag = FALSE ;
	static COLORDATA R8G8B8_ColorData,   A8R8G8B8_ColorData,    X8R8G8B8_ColorData ;
	static COLORDATA R5G6B5_ColorData,   X1R5G5B5_ColorData,    A1R5G5B5_ColorData ;
	static COLORDATA A4R4G4B4_ColorData, R3G3B2_ColorData,      A8R3G3B2_ColorData ;
	static COLORDATA X4R4G4B4_ColorData, A2B10G10R10_ColorData, A8B8G8R8_ColorData ;
	static COLORDATA X8B8G8R8_ColorData, A2R10G10B10_ColorData ;
	static COLORDATA A16B16G16R16I_ColorData ;
	static COLORDATA A16B16G16R16F_ColorData ;
	static COLORDATA A32B32G32R32F_ColorData ;
	static COLORDATA ONE_I8_ColorData ;
	static COLORDATA ONE_I16_ColorData ;
	static COLORDATA ONE_F16_ColorData ;
	static COLORDATA ONE_F32_ColorData ;
	static COLORDATA TWO_I8_ColorData ;
	static COLORDATA TWO_I16_ColorData ;
	static COLORDATA TWO_F16_ColorData ;
	static COLORDATA TWO_F32_ColorData ;

	if( InitializeFlag == FALSE )
	{
		InitializeFlag = TRUE ;
		NS_CreateColorData( &R8G8B8_ColorData,         24, 0x00ff0000, 0x0000ff00, 0x000000ff, 0x00000000 ) ;
		NS_CreateColorData( &A8R8G8B8_ColorData,       32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000 ) ;
		NS_CreateColorData( &X8R8G8B8_ColorData,       32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0x00000000 ) ;
		NS_CreateColorData( &R5G6B5_ColorData,         16, 0x0000f800, 0x000007e0, 0x0000001f, 0x00000000 ) ;
		NS_CreateColorData( &X1R5G5B5_ColorData,       16, 0x00007c00, 0x000003e0, 0x0000001f, 0x00000000 ) ;
		NS_CreateColorData( &A1R5G5B5_ColorData,       16, 0x00007c00, 0x000003e0, 0x0000001f, 0x00008000 ) ;
		NS_CreateColorData( &A4R4G4B4_ColorData,       16, 0x00000f00, 0x000000f0, 0x0000000f, 0x0000f000 ) ;
		NS_CreateColorData( &R3G3B2_ColorData,          8, 0x000000e0, 0x0000001c, 0x00000003, 0x00000000 ) ;
		NS_CreateColorData( &A8R3G3B2_ColorData,       16, 0x000000e0, 0x0000001c, 0x00000003, 0x0000ff00 ) ;
		NS_CreateColorData( &X4R4G4B4_ColorData,       16, 0x00000f00, 0x000000f0, 0x0000000f, 0x00000000 ) ;
		NS_CreateColorData( &A2B10G10R10_ColorData,    32, 0x000003ff, 0x000ffc00, 0x3ff00000, 0xc0000000 ) ;
		NS_CreateColorData( &A8B8G8R8_ColorData,       32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000 ) ;
		NS_CreateColorData( &X8B8G8R8_ColorData,       32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0x00000000 ) ;
		NS_CreateColorData( &A2R10G10B10_ColorData,    32, 0x3ff00000, 0x000ffc00, 0x000003ff, 0xc0000000 ) ;
		NS_CreateColorData( &A16B16G16R16I_ColorData,   0, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 4, 16, FALSE ) ;
		NS_CreateColorData( &A16B16G16R16F_ColorData,   0, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 4, 16,  TRUE ) ;
		NS_CreateColorData( &A32B32G32R32F_ColorData,   0, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 4, 32,  TRUE ) ;
		NS_CreateColorData( &ONE_I8_ColorData,	        0, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 1,  8, FALSE ) ;
		NS_CreateColorData( &ONE_I16_ColorData,	        0, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 1, 16, FALSE ) ;
		NS_CreateColorData( &ONE_F16_ColorData,	        0, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 1, 16,  TRUE ) ;
		NS_CreateColorData( &ONE_F32_ColorData,	        0, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 1, 32,  TRUE ) ;
		NS_CreateColorData( &TWO_I8_ColorData,	        0, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 2,  8, FALSE ) ;
		NS_CreateColorData( &TWO_I16_ColorData,	        0, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 2, 16, FALSE ) ;
		NS_CreateColorData( &TWO_F16_ColorData,	        0, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 2, 16,  TRUE ) ;
		NS_CreateColorData( &TWO_F32_ColorData,	        0, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 2, 32,  TRUE ) ;
	}

	switch( Format )
	{
	case D_D3DFMT_R8G8B8 :        return &R8G8B8_ColorData ;
	case D_D3DFMT_A8R8G8B8 :      return &A8R8G8B8_ColorData ;
	case D_D3DFMT_X8R8G8B8 :      return &X8R8G8B8_ColorData ;
	case D_D3DFMT_R5G6B5 :        return &R5G6B5_ColorData ;
	case D_D3DFMT_X1R5G5B5 :      return &X1R5G5B5_ColorData ;
	case D_D3DFMT_A1R5G5B5 :      return &A1R5G5B5_ColorData ;
	case D_D3DFMT_A4R4G4B4 :      return &A4R4G4B4_ColorData ;
	case D_D3DFMT_R3G3B2 :        return &R3G3B2_ColorData ;
	case D_D3DFMT_A8R3G3B2 :      return &A8R3G3B2_ColorData ;
	case D_D3DFMT_X4R4G4B4 :      return &X4R4G4B4_ColorData ;
	case D_D3DFMT_A2B10G10R10 :   return &A2B10G10R10_ColorData ;
	case D_D3DFMT_A8B8G8R8 :      return &A8B8G8R8_ColorData ;
	case D_D3DFMT_X8B8G8R8 :      return &X8B8G8R8_ColorData ;
	case D_D3DFMT_A2R10G10B10 :   return &A2R10G10B10_ColorData ;
	case D_D3DFMT_A16B16G16R16 :  return &A16B16G16R16I_ColorData ;
	case D_D3DFMT_A16B16G16R16F : return &A16B16G16R16F_ColorData ;
	case D_D3DFMT_A32B32G32R32F : return &A32B32G32R32F_ColorData ;
	case D_D3DFMT_L8 :            return &ONE_I8_ColorData ;
	case D_D3DFMT_L16 :           return &ONE_I16_ColorData ;
	case D_D3DFMT_R16F :          return &ONE_F16_ColorData ;
	case D_D3DFMT_R32F :          return &ONE_F32_ColorData ;
	case D_D3DFMT_A8L8 :          return &TWO_I8_ColorData ;
	case D_D3DFMT_V8U8 :          return &TWO_I8_ColorData ;
	case D_D3DFMT_G16R16 :        return &TWO_I16_ColorData ;
	case D_D3DFMT_G16R16F :       return &TWO_F16_ColorData ;
	case D_D3DFMT_G32R32F :       return &TWO_F32_ColorData ;
	}
	return NULL ;
}


// �`���p�e�N�X�`���ƕ`��p�e�N�X�`����������Ă���ꍇ�ɁA�`��p�e�N�X�`���ɕ`���p�e�N�X�`���̓��e�𔽉f����
extern	int		Graphics_D3D9_UpdateDrawTexture( IMAGEDATA_ORIG_HARD_TEX *OrigTex, int TargetSurface )
{
	Direct3DDevice9_StretchRect(
		OrigTex->PF->D3D9.RenderTargetSurface,
		NULL,
		OrigTex->PF->D3D9.Surface[ TargetSurface ],
		NULL,
		D_D3DTEXF_NONE ) ;

	// �I��
	return 0 ;
}

// ��{�C���[�W�̃t�H�[�}�b�g�� DX_BASEIMAGE_FORMAT_NORMAL �ɕϊ�����K�v�����邩�ǂ������`�F�b�N����
// ( TRUE:�ϊ�����K�v������  FALSE:�ϊ�����K�v�͖��� )
extern	int		Graphics_D3D9_CheckRequiredNormalImageConv_BaseImageFormat_PF(
	IMAGEDATA_ORIG *Orig,
	int             RgbBaseImageFormat,
	int            *RequiredRgbBaseImageConvFlag,
	int             AlphaBaseImageFormat,
	int            *RequiredAlphaBaseImageConvFlag
)
{
	int RgbConvFlag   = FALSE ;
	int AlphaConvFlag = FALSE ;

	if( Orig->FormatDesc.TextureFlag )
	{
		if( ( RgbBaseImageFormat == DX_BASEIMAGE_FORMAT_DXT1 && GD3D9.Device.Caps.TextureFormat[ Orig->ColorFormat ] != D_D3DFMT_DXT1 ) ||
			( RgbBaseImageFormat == DX_BASEIMAGE_FORMAT_DXT2 && GD3D9.Device.Caps.TextureFormat[ Orig->ColorFormat ] != D_D3DFMT_DXT2 ) ||
			( RgbBaseImageFormat == DX_BASEIMAGE_FORMAT_DXT3 && GD3D9.Device.Caps.TextureFormat[ Orig->ColorFormat ] != D_D3DFMT_DXT3 ) ||
			( RgbBaseImageFormat == DX_BASEIMAGE_FORMAT_DXT4 && GD3D9.Device.Caps.TextureFormat[ Orig->ColorFormat ] != D_D3DFMT_DXT4 ) ||
			( RgbBaseImageFormat == DX_BASEIMAGE_FORMAT_DXT5 && GD3D9.Device.Caps.TextureFormat[ Orig->ColorFormat ] != D_D3DFMT_DXT5 ) )
		{
			RgbConvFlag = TRUE ;
		}

		if( AlphaBaseImageFormat >= 0 )
		{
			if( ( AlphaBaseImageFormat == DX_BASEIMAGE_FORMAT_DXT1 && GD3D9.Device.Caps.TextureFormat[ Orig->ColorFormat ] != D_D3DFMT_DXT1 ) ||
				( AlphaBaseImageFormat == DX_BASEIMAGE_FORMAT_DXT2 && GD3D9.Device.Caps.TextureFormat[ Orig->ColorFormat ] != D_D3DFMT_DXT2 ) ||
				( AlphaBaseImageFormat == DX_BASEIMAGE_FORMAT_DXT3 && GD3D9.Device.Caps.TextureFormat[ Orig->ColorFormat ] != D_D3DFMT_DXT3 ) ||
				( AlphaBaseImageFormat == DX_BASEIMAGE_FORMAT_DXT4 && GD3D9.Device.Caps.TextureFormat[ Orig->ColorFormat ] != D_D3DFMT_DXT4 ) ||
				( AlphaBaseImageFormat == DX_BASEIMAGE_FORMAT_DXT5 && GD3D9.Device.Caps.TextureFormat[ Orig->ColorFormat ] != D_D3DFMT_DXT5 ) )
			{
				AlphaConvFlag = TRUE ;
			}
		}
	}
	else
	{
		if( RgbBaseImageFormat == DX_BASEIMAGE_FORMAT_DXT1 ||
			RgbBaseImageFormat == DX_BASEIMAGE_FORMAT_DXT2 ||
			RgbBaseImageFormat == DX_BASEIMAGE_FORMAT_DXT3 ||
			RgbBaseImageFormat == DX_BASEIMAGE_FORMAT_DXT4 ||
			RgbBaseImageFormat == DX_BASEIMAGE_FORMAT_DXT5 )
		{
			RgbConvFlag = TRUE ;
		}

		if( AlphaBaseImageFormat >= 0 )
		{
			if( AlphaBaseImageFormat == DX_BASEIMAGE_FORMAT_DXT1 ||
				AlphaBaseImageFormat == DX_BASEIMAGE_FORMAT_DXT2 ||
				AlphaBaseImageFormat == DX_BASEIMAGE_FORMAT_DXT3 ||
				AlphaBaseImageFormat == DX_BASEIMAGE_FORMAT_DXT4 ||
				AlphaBaseImageFormat == DX_BASEIMAGE_FORMAT_DXT5 )
			{
				AlphaConvFlag = TRUE ;
			}
		}
	}

	if( RequiredRgbBaseImageConvFlag != NULL )
	{
		*RequiredRgbBaseImageConvFlag = RgbConvFlag ;
	}

	if( RequiredAlphaBaseImageConvFlag != NULL )
	{
		*RequiredAlphaBaseImageConvFlag = AlphaConvFlag ;
	}

	// ����I��
	return 0 ;
}



























// Direct3DDevice �̏������֌W

// �w��̐[�x�X�e���V���t�H�[�}�b�g���g�p�\���ǂ����𒲂ׂ�
// �߂�l   -1:�g�p�ł��Ȃ�    0:�g�p�ł���
static int CheckDeatpStencilMatchRapper( DWORD Adapter, D_D3DDEVTYPE DeviceType, D_D3DFORMAT AdapterFormat, D_D3DFORMAT RenderTargetFormat, D_D3DFORMAT DepthStencilFormat )
{
	if( Direct3D9_CheckDeviceFormat( Adapter, DeviceType, AdapterFormat, D_D3DUSAGE_DEPTHSTENCIL, D_D3DRTYPE_SURFACE, DepthStencilFormat ) != D_D3D_OK )
	{
		return -1 ;
	}

	if( Direct3D9_CheckDepthStencilMatch( Adapter, DeviceType, AdapterFormat, RenderTargetFormat, DepthStencilFormat ) != D_D3D_OK )
	{
		return -1 ;
	}

	return 0 ;
}

// Direct3DDevice9 �I�u�W�F�N�g�̍쐬
extern int Graphics_D3D9_Device_Create()
{
	HRESULT hr ;
	const char *String = NULL ;
	int ErrorRet = -1 ;

	if( Direct3D9_IsValid() == 0 ) return -1 ;

	// ���ɍ쐬�ς݂̏ꍇ�͈�x�폜����
	Direct3DDevice9_Release() ;
	GD3D9.Device.DrawInfo.BeginSceneFlag = FALSE ;

	// �V�F�[�_�[�Q�D�O���g���Ȃ����ł̓t���X�N���[���A���`�G�C���A�X�𐳏�Ɏg����\�����Ⴂ�̂�
	// �V�F�[�_�[�Q�D�O���g���Ȃ����ł̓t���X�N���[���A���`�G�C���A�X�𖳌��ɂ���
	if( NS_GetValidShaderVersion() < 200 )
	{
		GSYS.Setting.FSAAMultiSampleCount = D_D3DMULTISAMPLE_NONE ;
		GSYS.Setting.FSAAMultiSampleQuality = 0 ;
	}

	// �f�o�C�X�̍쐬
	if( Direct3D9_CreateDevice() < 0 )
	{
		// ���s�������ʃ��[�h��ύX���Ď���
		GSYS.Screen.MainScreenColorBitDepth = 32 ;
		SetMemImgDefaultColorType( GSYS.Screen.MainScreenColorBitDepth == 32 ? 1 : 0 ) ;
		if( Direct3D9_CreateDevice() < 0 )
		{
			Graphics_Screen_SetMainScreenSize( 800, 600 ) ;
			if( Direct3D9_CreateDevice() < 0 )
			{
				Graphics_Screen_SetMainScreenSize( 1024, 768 ) ;
				if( Direct3D9_CreateDevice() < 0 )
				{
					goto ERR ;
				}
			}
		}
	}

	// VSync�̑O��Sleep���鎞�Ԃ��擾����
	DirectDraw7_WaitVSyncInitialize() ;

	// �Œ�@�\�p�C�v���C���ɖ߂�
	GD3D9.Device.State.SetVertexShader       = NULL ;
	GD3D9.Device.State.SetVertexDeclaration  = NULL ;
	GD3D9.Device.State.SetPixelShader        = NULL ;
	GD3D9.Device.State.SetFVF                = 0 ;
	GD3D9.Device.State.SetIndexBuffer        = NULL ;
	GD3D9.Device.State.SetVertexBuffer       = NULL ;
	GD3D9.Device.State.SetVertexBufferStride = 0 ;

	// �o�b�N�o�b�t�@�̃A�h���X���擾���Ă���
	hr = Direct3DDevice9_GetBackBuffer( 0, 0, D_D3DBACKBUFFER_TYPE_MONO, &GD3D9.Device.Screen.BackBufferSurface ) ;

	DXST_ERRORLOG_ADDUTF16LE( "\x10\x62\x9f\x52\x0a\x00\x00"/*@ L"����\n" @*/ ) ;

	// �t���X�N���[�����[�h�̏ꍇ�̓f�X�N�g�b�v��̈ʒu���擾���Ă���
	if( NS_GetWindowModeFlag() == FALSE )
	{
		HMONITOR      hMonitor ;
		MONITORINFOEX MonitorInfoEx ;

		hMonitor = ( HMONITOR )Direct3D9_GetAdapterMonitor( GSYS.Screen.ValidUseDisplayIndex ? ( DWORD )GSYS.Screen.UseDisplayIndex : D_D3DADAPTER_DEFAULT ) ;

		_MEMSET( &MonitorInfoEx, 0, sizeof( MonitorInfoEx ) ) ;
		MonitorInfoEx.cbSize = sizeof( MonitorInfoEx ) ;
		GetMonitorInfo( hMonitor, &MonitorInfoEx ) ;

		GSYS.Screen.FullScreenDesktopRect = MonitorInfoEx.rcMonitor ;
	}

	// ���������ꍇ�̓f�o�C�X�����擾����
	Direct3DDevice9_GetDeviceCaps( &GD3D9.Device.Caps.DeviceCaps ) ;

//	GD3D9.Device.Caps.DeviceCaps.VertexShaderVersion = 0x200 ;
//	GD3D9.Device.Caps.DeviceCaps.PixelShaderVersion  = 0x200 ;
//	GD3D9.Device.Caps.DeviceCaps.VertexShaderVersion = 0x000 ;
//	GD3D9.Device.Caps.DeviceCaps.PixelShaderVersion  = 0x000 ;

	// �g�p�ł���e�N�X�`���X�e�[�W�̐���ۑ�
	GD3D9.Device.Caps.MaxTextureBlendStages = ( int )GD3D9.Device.Caps.DeviceCaps.MaxTextureBlendStages ;

	// ���_�J���[��BGRA�J���[�^�C�v���g�p����
	GSYS.HardInfo.UseVertexColorBGRAFormat = TRUE ;

	// ���_���W�^�C�v�� DX_DRAWFLOATCOORDTYPE_DIRECT3D9
	GSYS.HardInfo.DrawFloatCoordType = DX_DRAWFLOATCOORDTYPE_DIRECT3D9 ;

	// OggTheora�Đ��p�̉摜�ɕ`��\�摜���g�p����
	GSYS.HardInfo.OggTheoraUseDrawValidGraph = TRUE ;

	// �����Ƀ����_�����O�ł���^�[�Q�b�g�̐���ۑ�
	GSYS.HardInfo.RenderTargetNum = ( int )GD3D9.Device.Caps.DeviceCaps.NumSimultaneousRTs ;
	if( GSYS.HardInfo.RenderTargetNum > DX_RENDERTARGET_COUNT )
	{
		GSYS.HardInfo.RenderTargetNum = DX_RENDERTARGET_COUNT ;
	}

	// �e�N�X�`���X�e�[�W�̃e���|�������W�X�^���g�p�ł��邩�ǂ�����ۑ�
	GD3D9.Device.Caps.ValidTexTempRegFlag = ( GD3D9.Device.Caps.DeviceCaps.PrimitiveMiscCaps & D_D3DPMISCCAPS_TSSARGTEMP ) != 0 ? TRUE : FALSE ;

	// �V�F�[�_�[���g��Ȃ��ݒ�̏ꍇ�͕K������
	if( GD3D9.Setting.NonUseVertexHardwareProcess )
	{
		GD3D9.Device.Shader.ValidVertexShader = FALSE ;
		GD3D9.Device.Shader.ValidPixelShader = FALSE ;
		GD3D9.Device.Shader.ValidVertexShader_SM3 = FALSE ;
		GD3D9.Device.Shader.ValidPixelShader_SM3 = FALSE ;
	}
	else
	{
		// ���_�V�F�[�_�[���g�p�\���ǂ�����ۑ�
		GD3D9.Device.Shader.ValidVertexShader = /* GD3D9.Device.Caps.VertexHardwareProcess && */ ( GD3D9.Device.Caps.DeviceCaps.VertexShaderVersion & 0xffff ) >= 0x0200 ;

		// �s�N�Z���V�F�[�_�[���g�p�\���ǂ�����ۑ�
		GD3D9.Device.Shader.ValidPixelShader = /* GD3D9.Device.Caps.VertexHardwareProcess && */ ( GD3D9.Device.Caps.DeviceCaps.PixelShaderVersion & 0xffff ) >= 0x0200 ;

		// SM3.0�̒��_�V�F�[�_�[���g�p�\���ǂ�����ۑ�
		GD3D9.Device.Shader.ValidVertexShader_SM3 = /* GD3D9.Device.Caps.VertexHardwareProcess && */ ( GD3D9.Device.Caps.DeviceCaps.VertexShaderVersion & 0xffff ) >= 0x0300 ;

		// SM3.0�̃s�N�Z���V�F�[�_�[���g�p�\���ǂ�����ۑ�
		GD3D9.Device.Shader.ValidPixelShader_SM3 = /* GD3D9.Device.Caps.VertexHardwareProcess && */ ( GD3D9.Device.Caps.DeviceCaps.PixelShaderVersion & 0xffff ) >= 0x0300 ;

		// SM3.0 �̓s�N�Z���V�F�[�_�[�����_�V�F�[�_�[���Ή����Ă��Ȃ��ꍇ�͎g�p�ł��Ȃ�
		if( GD3D9.Device.Shader.ValidVertexShader_SM3 == FALSE ||
			GD3D9.Device.Shader.ValidPixelShader_SM3 == FALSE )
		{
			GD3D9.Device.Shader.ValidVertexShader_SM3 = FALSE ;
			GD3D9.Device.Shader.ValidPixelShader_SM3  = FALSE ;
		}
	}

	// D3DBLENDOP_ADD �ȊO���g�p�ł��邩�ǂ������擾����
	GD3D9.Device.Caps.ValidDestBlendOp = ( GD3D9.Device.Caps.DeviceCaps.PrimitiveMiscCaps & D_D3DPMISCCAPS_BLENDOP ) == 0 ? FALSE : TRUE ;

	// ���Z�u�����h���\���ǂ������Z�b�g����
	GSYS.HardInfo.ValidSubBlend = GD3D9.Device.Caps.ValidDestBlendOp ;

	// �e�N�X�`���̃T�C�Y���Q�̂���ł���K�v�����邩�ǂ������擾����
	GSYS.HardInfo.TextureSizePow2 = ( GD3D9.Device.Caps.DeviceCaps.TextureCaps & D_D3DPTEXTURECAPS_POW2 ) != 0 ? TRUE : FALSE ;

	// �e�N�X�`���������`�ł���K�v�����邩�ǂ������擾����
	GSYS.HardInfo.TextureSquareOnly = ( GD3D9.Device.Caps.DeviceCaps.TextureCaps & D_D3DPTEXTURECAPS_SQUAREONLY ) != 0 ? TRUE : FALSE ;

	// �����t�Ńe�N�X�`���̃T�C�Y���Q�̂���łȂ��Ă����v���ǂ������擾����
	GSYS.HardInfo.TextureSizeNonPow2Conditional = ( GD3D9.Device.Caps.DeviceCaps.TextureCaps & D_D3DPTEXTURECAPS_NONPOW2CONDITIONAL ) != 0 ? TRUE : FALSE ;

	// �e�N�X�`���T�C�Y���ݒ肳��Ă��Ȃ��ꍇ�͐ݒ肷��
	if( GD3D9.Device.Caps.DeviceCaps.MaxTextureWidth  == 0 ) GD3D9.Device.Caps.DeviceCaps.MaxTextureWidth  = DEFAULT_MAXTEXTURE_SIZE ;
	if( GD3D9.Device.Caps.DeviceCaps.MaxTextureHeight == 0 ) GD3D9.Device.Caps.DeviceCaps.MaxTextureHeight = DEFAULT_MAXTEXTURE_SIZE ;

	// �e�N�X�`���̍ő啝�ƍő卂����ۑ�����
	GSYS.HardInfo.MaxTextureWidth  = ( int )GD3D9.Device.Caps.DeviceCaps.MaxTextureWidth ;
	GSYS.HardInfo.MaxTextureHeight = ( int )GD3D9.Device.Caps.DeviceCaps.MaxTextureHeight ;

	// �T�C�Y�̏������ق����ő�T�C�Y�Ƃ���
	GSYS.HardInfo.MaxTextureSize = ( int )( GD3D9.Device.Caps.DeviceCaps.MaxTextureWidth < GD3D9.Device.Caps.DeviceCaps.MaxTextureHeight ? GD3D9.Device.Caps.DeviceCaps.MaxTextureWidth : GD3D9.Device.Caps.DeviceCaps.MaxTextureHeight ) ;

	// ��x�ɕ`��ł���ő�v���~�e�B�u�����擾����
	GSYS.HardInfo.MaxPrimitiveCount = ( int )GD3D9.Device.Caps.DeviceCaps.MaxPrimitiveCount ;

	// ��x�Ɏg�p���邱�Ƃ̏o����ő咸�_�����擾����
	GSYS.HardInfo.MaxVertexIndex = ( int )GD3D9.Device.Caps.DeviceCaps.MaxVertexIndex ;

	// ���_�J���[���g�p���邩�ǂ����̒l��������
	GD3D9.Device.State.MaterialUseVertexDiffuseColor = 0xff ;
	GD3D9.Device.State.MaterialUseVertexSpecularColor = 0xff ;

	// Graphics_D3D9_DrawPreparation ���s���ׂ��t���O�𗧂Ă�
	GD3D9.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

	{
		D_D3DADAPTER_IDENTIFIER9 Iden ;

		Direct3D9_GetAdapterIdentifier( D_D3DADAPTER_DEFAULT, 0, &Iden ) ;
		DXST_ERRORLOGFMT_ADDA(( "Driver:%s  Description:%s", Iden.Driver, Iden.Description )) ;
	}

	// �e��t�H�[�}�b�g�𒲂ׂ�
	{
		D_D3DSURFACE_DESC SurfaceDesc ;
		DWORD UseAdapterNumber ;

		// �g�p����f�B�X�v���C�A�_�v�^�[������
		UseAdapterNumber = GSYS.Screen.ValidUseDisplayIndex ? ( DWORD )GSYS.Screen.UseDisplayIndex : D_D3DADAPTER_DEFAULT ;

		// ��ʂ̃t�H�[�}�b�g�̎擾
		Direct3DSurface9_GetDesc( GD3D9.Device.Screen.BackBufferSurface, &SurfaceDesc ) ;
		GD3D9.Device.Caps.ScreenFormat = SurfaceDesc.Format ;
		switch( GD3D9.Device.Caps.ScreenFormat )
		{
		case D_D3DFMT_X8R8G8B8 : String = "D3DFMT_X8R8G8B8" ; break ;
		case D_D3DFMT_R5G6B5 :   String = "D3DFMT_R5G6B5" ;   break ;
		case D_D3DFMT_X1R5G5B5 : String = "D3DFMT_X1R5G5B5" ; break ;
		default:
			DXST_ERRORLOGFMT_ADDA(( "\x83\x6f\x83\x62\x83\x4e\x83\x6f\x83\x62\x83\x74\x83\x40\x82\xaa\x91\xce\x89\x9e\x82\xb5\x82\xc4\x82\xa2\x82\xc8\x82\xa2\x83\x74\x83\x48\x81\x5b\x83\x7d\x83\x62\x83\x67 %d \x82\xc5\x82\xb7"/*@ "�o�b�N�o�b�t�@���Ή����Ă��Ȃ��t�H�[�}�b�g %d �ł�" @*/, GD3D9.Device.Caps.ScreenFormat )) ;
			break ;
		}
		DXST_ERRORLOGFMT_ADDA(( "\x89\xe6\x96\xca\x82\xcc\x83\x74\x83\x48\x81\x5b\x83\x7d\x83\x62\x83\x67\x82\xcd %s \x82\xc5\x82\xb7"/*@ "��ʂ̃t�H�[�}�b�g�� %s �ł�" @*/, String )) ;

		// 16�r�b�g�y�o�b�t�@�t�H�[�}�b�g�̐ݒ�
		GD3D9.Device.Caps.ZBufferFormat[ ZBUFFER_FORMAT_16BIT ] = D_D3DFMT_D16 ;
		String = "D3DFMT_D16" ;
		DXST_ERRORLOGFMT_ADDA(( "16bit Z\x83\x6f\x83\x62\x83\x74\x83\x40\x83\x74\x83\x48\x81\x5b\x83\x7d\x83\x62\x83\x67\x82\xcd %s \x82\xc5\x82\xb7"/*@ "16bit Z�o�b�t�@�t�H�[�}�b�g�� %s �ł�" @*/, String )) ;

		// 24�r�b�g�y�o�b�t�@�t�H�[�}�b�g�̑I��
		if( CheckDeatpStencilMatchRapper( UseAdapterNumber, D_D3DDEVTYPE_HAL, GD3D9.Device.Caps.ScreenFormat, GD3D9.Device.Caps.ScreenFormat, D_D3DFMT_D24X8 ) == 0 )
		{
			GD3D9.Device.Caps.ZBufferFormat[ ZBUFFER_FORMAT_24BIT ] = D_D3DFMT_D24X8 ;
			String = "D3DFMT_D24X8" ;
		}
		else
		if( CheckDeatpStencilMatchRapper( UseAdapterNumber, D_D3DDEVTYPE_HAL, GD3D9.Device.Caps.ScreenFormat, GD3D9.Device.Caps.ScreenFormat, D_D3DFMT_D32 ) == 0 )
		{
			GD3D9.Device.Caps.ZBufferFormat[ ZBUFFER_FORMAT_24BIT ] = D_D3DFMT_D32 ;
			String = "D3DFMT_D32" ;
		}
		else
		{
			GD3D9.Device.Caps.ZBufferFormat[ ZBUFFER_FORMAT_24BIT ] = D_D3DFMT_D16 ;
			String = "D3DFMT_D16" ;
		}
		DXST_ERRORLOGFMT_ADDA(( "24bit Z\x83\x6f\x83\x62\x83\x74\x83\x40\x83\x74\x83\x48\x81\x5b\x83\x7d\x83\x62\x83\x67\x82\xcd %s \x82\xc5\x82\xb7"/*@ "24bit Z�o�b�t�@�t�H�[�}�b�g�� %s �ł�" @*/, String )) ;

		// 32�r�b�g�y�o�b�t�@�t�H�[�}�b�g�̑I��
		if( CheckDeatpStencilMatchRapper( UseAdapterNumber, D_D3DDEVTYPE_HAL, GD3D9.Device.Caps.ScreenFormat, GD3D9.Device.Caps.ScreenFormat, D_D3DFMT_D32 ) == 0 )
		{
			GD3D9.Device.Caps.ZBufferFormat[ ZBUFFER_FORMAT_32BIT ] = D_D3DFMT_D32 ;
			String = "D3DFMT_D32" ;
		}
		else
		if( CheckDeatpStencilMatchRapper( UseAdapterNumber, D_D3DDEVTYPE_HAL, GD3D9.Device.Caps.ScreenFormat, GD3D9.Device.Caps.ScreenFormat, D_D3DFMT_D24X8 ) == 0 )
		{
			GD3D9.Device.Caps.ZBufferFormat[ ZBUFFER_FORMAT_32BIT ] = D_D3DFMT_D24X8 ;
			String = "D3DFMT_D24X8" ;
		}
		else
		if( CheckDeatpStencilMatchRapper( UseAdapterNumber, D_D3DDEVTYPE_HAL, GD3D9.Device.Caps.ScreenFormat, GD3D9.Device.Caps.ScreenFormat, D_D3DFMT_D16 ) == 0 )
		{
			GD3D9.Device.Caps.ZBufferFormat[ ZBUFFER_FORMAT_32BIT ] = D_D3DFMT_D16 ;
			String = "D3DFMT_D16" ;
		}
		DXST_ERRORLOGFMT_ADDA(( "32bit Z\x83\x6f\x83\x62\x83\x74\x83\x40\x83\x74\x83\x48\x81\x5b\x83\x7d\x83\x62\x83\x67\x82\xcd %s \x82\xc5\x82\xb7"/*@ "32bit Z�o�b�t�@�t�H�[�}�b�g�� %s �ł�" @*/, String )) ;

		// 16�r�b�g�J���[�t�H�[�}�b�g�̑I��
		if( Direct3D9_CheckDeviceFormat( UseAdapterNumber, D_D3DDEVTYPE_HAL, GD3D9.Device.Caps.ScreenFormat, 0, D_D3DRTYPE_TEXTURE, D_D3DFMT_R5G6B5 ) == D_D3D_OK )
		{
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_RGB16 ] = D_D3DFMT_R5G6B5 ;
			String = "D3DFMT_R5G6B5" ;
		}
		else
		if( Direct3D9_CheckDeviceFormat( UseAdapterNumber, D_D3DDEVTYPE_HAL, GD3D9.Device.Caps.ScreenFormat, 0, D_D3DRTYPE_TEXTURE, D_D3DFMT_X1R5G5B5 ) == D_D3D_OK )
		{
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_RGB16 ] = D_D3DFMT_X1R5G5B5 ;
			String = "D3DFMT_X1R5G5B5" ;
		}
		else
		{
			DXST_ERRORLOG_ADDUTF16LE( "\x7f\x4f\x28\x75\x67\x30\x4d\x30\x8b\x30\x20\x00\x31\x00\x36\x00\x62\x00\x69\x00\x74\x00\x20\x00\xab\x30\xe9\x30\xfc\x30\xd5\x30\xa9\x30\xfc\x30\xde\x30\xc3\x30\xc8\x30\x4c\x30\x8b\x89\x64\x30\x4b\x30\x8a\x30\x7e\x30\x5b\x30\x93\x30\x67\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�g�p�ł��� 16bit �J���[�t�H�[�}�b�g��������܂���ł���\n" @*/ ) ;
			goto ERR ;
		}
		DXST_ERRORLOGFMT_ADDA(( "16bit \x83\x4a\x83\x89\x81\x5b\x83\x74\x83\x48\x81\x5b\x83\x7d\x83\x62\x83\x67\x82\xcd %s \x82\xc5\x82\xb7"/*@ "16bit �J���[�t�H�[�}�b�g�� %s �ł�" @*/, String )) ;

		// 32�r�b�g�J���[�t�H�[�}�b�g�̑I��
		if( Direct3D9_CheckDeviceFormat( UseAdapterNumber, D_D3DDEVTYPE_HAL, GD3D9.Device.Caps.ScreenFormat, 0, D_D3DRTYPE_TEXTURE, D_D3DFMT_X8R8G8B8 ) == D_D3D_OK )
		{
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_RGB32 ] = D_D3DFMT_X8R8G8B8 ;
			String = "D3DFMT_X8R8G8B8" ;
		}
		else
		{
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_RGB32 ] = GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_RGB16 ] ;
		}
		DXST_ERRORLOGFMT_ADDA(( "32bit \x83\x4a\x83\x89\x81\x5b\x83\x74\x83\x48\x81\x5b\x83\x7d\x83\x62\x83\x67\x82\xcd %s \x82\xc5\x82\xb7"/*@ "32bit �J���[�t�H�[�}�b�g�� %s �ł�" @*/, String )) ;

		// �A���t�@�t�� 16�r�b�g�J���[�t�H�[�}�b�g�̑I��
		if( Direct3D9_CheckDeviceFormat( UseAdapterNumber, D_D3DDEVTYPE_HAL, GD3D9.Device.Caps.ScreenFormat, 0, D_D3DRTYPE_TEXTURE, D_D3DFMT_A4R4G4B4 ) == D_D3D_OK )
		{
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ALPHA_RGB16 ] = D_D3DFMT_A4R4G4B4 ;
			String = "D3DFMT_A4R4G4B4" ;
		}
		else
		if( Direct3D9_CheckDeviceFormat( UseAdapterNumber, D_D3DDEVTYPE_HAL, GD3D9.Device.Caps.ScreenFormat, 0, D_D3DRTYPE_TEXTURE, D_D3DFMT_A8R8G8B8 ) == D_D3D_OK )
		{
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ALPHA_RGB16 ] = D_D3DFMT_A8R8G8B8 ;
			String = "D3DFMT_A8R8G8B8" ;
		}
		else
		{
			DXST_ERRORLOG_ADDUTF16LE( "\x7f\x4f\x28\x75\x67\x30\x4d\x30\x8b\x30\xa2\x30\xeb\x30\xd5\x30\xa1\x30\xd8\x4e\x4d\x30\x20\x00\x31\x00\x36\x00\x62\x00\x69\x00\x74\x00\x20\x00\xab\x30\xe9\x30\xfc\x30\xd5\x30\xa9\x30\xfc\x30\xde\x30\xc3\x30\xc8\x30\x4c\x30\x8b\x89\x64\x30\x4b\x30\x8a\x30\x7e\x30\x5b\x30\x93\x30\x67\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�g�p�ł���A���t�@�t�� 16bit �J���[�t�H�[�}�b�g��������܂���ł���\n" @*/ ) ;
			goto ERR ;
		}
		DXST_ERRORLOGFMT_ADDA(( "\x83\x41\x83\x8b\x83\x74\x83\x40\x95\x74\x82\xab 16bit \x83\x4a\x83\x89\x81\x5b\x83\x74\x83\x48\x81\x5b\x83\x7d\x83\x62\x83\x67\x82\xcd %s \x82\xc5\x82\xb7"/*@ "�A���t�@�t�� 16bit �J���[�t�H�[�}�b�g�� %s �ł�" @*/, String )) ;

		// �A���t�@�t�� 32�r�b�g�J���[�t�H�[�}�b�g�̑I��
		if( Direct3D9_CheckDeviceFormat( UseAdapterNumber, D_D3DDEVTYPE_HAL, GD3D9.Device.Caps.ScreenFormat, 0, D_D3DRTYPE_TEXTURE, D_D3DFMT_A8R8G8B8 ) == D_D3D_OK )
		{
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ALPHA_RGB32 ] = D_D3DFMT_A8R8G8B8 ;
			String = "D3DFMT_A8R8G8B8" ;
		}
		else
		if( Direct3D9_CheckDeviceFormat( UseAdapterNumber, D_D3DDEVTYPE_HAL, GD3D9.Device.Caps.ScreenFormat, 0, D_D3DRTYPE_TEXTURE, D_D3DFMT_A4R4G4B4 ) == D_D3D_OK )
		{
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ALPHA_RGB32 ] = D_D3DFMT_A4R4G4B4 ;
			String = "D3DFMT_A4R4G4B4" ;
		}
		else
		{
			DXST_ERRORLOG_ADDUTF16LE( "\x7f\x4f\x28\x75\x67\x30\x4d\x30\x8b\x30\xa2\x30\xeb\x30\xd5\x30\xa1\x30\xd8\x4e\x4d\x30\x20\x00\x33\x00\x32\x00\x62\x00\x69\x00\x74\x00\x20\x00\xab\x30\xe9\x30\xfc\x30\xd5\x30\xa9\x30\xfc\x30\xde\x30\xc3\x30\xc8\x30\x4c\x30\x8b\x89\x64\x30\x4b\x30\x8a\x30\x7e\x30\x5b\x30\x93\x30\x67\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�g�p�ł���A���t�@�t�� 32bit �J���[�t�H�[�}�b�g��������܂���ł���\n" @*/ ) ;
			goto ERR ;
		}
		DXST_ERRORLOGFMT_ADDA(( "\x83\x41\x83\x8b\x83\x74\x83\x40\x95\x74\x82\xab 32bit \x83\x4a\x83\x89\x81\x5b\x83\x74\x83\x48\x81\x5b\x83\x7d\x83\x62\x83\x67\x82\xcd %s \x82\xc5\x82\xb7"/*@ "�A���t�@�t�� 32bit �J���[�t�H�[�}�b�g�� %s �ł�" @*/, String )) ;

		// �A���t�@�e�X�g�p 16�r�b�g�J���[�t�H�[�}�b�g�̑I��
		if( Direct3D9_CheckDeviceFormat( UseAdapterNumber, D_D3DDEVTYPE_HAL, GD3D9.Device.Caps.ScreenFormat, 0, D_D3DRTYPE_TEXTURE, D_D3DFMT_A1R5G5B5 ) == D_D3D_OK )
		{
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ALPHATEST_RGB16 ] = D_D3DFMT_A1R5G5B5 ;
			String = "D3DFMT_A1R5G5B5" ;
		}
		else
		if( Direct3D9_CheckDeviceFormat( UseAdapterNumber, D_D3DDEVTYPE_HAL, GD3D9.Device.Caps.ScreenFormat, 0, D_D3DRTYPE_TEXTURE, D_D3DFMT_A8R8G8B8 ) == D_D3D_OK )
		{
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ALPHATEST_RGB16 ] = D_D3DFMT_A8R8G8B8 ;
			String = "D3DFMT_A8R8G8B8" ;
		}
		else
		if( Direct3D9_CheckDeviceFormat( UseAdapterNumber, D_D3DDEVTYPE_HAL, GD3D9.Device.Caps.ScreenFormat, 0, D_D3DRTYPE_TEXTURE, D_D3DFMT_A4R4G4B4 ) == D_D3D_OK )
		{
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ALPHATEST_RGB16 ] = D_D3DFMT_A4R4G4B4 ;
			String = "D3DFMT_A4R4G4B4" ;
		}
		else
		{
			DXST_ERRORLOG_ADDUTF16LE( "\x7f\x4f\x28\x75\x67\x30\x4d\x30\x8b\x30\xa2\x30\xeb\x30\xd5\x30\xa1\x30\xc6\x30\xb9\x30\xc8\x30\x28\x75\x20\x00\x31\x00\x36\x00\x62\x00\x69\x00\x74\x00\x20\x00\xab\x30\xe9\x30\xfc\x30\xd5\x30\xa9\x30\xfc\x30\xde\x30\xc3\x30\xc8\x30\x4c\x30\x8b\x89\x64\x30\x4b\x30\x8a\x30\x7e\x30\x5b\x30\x93\x30\x67\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�g�p�ł���A���t�@�e�X�g�p 16bit �J���[�t�H�[�}�b�g��������܂���ł���\n" @*/ ) ;
			goto ERR ;
		}
		DXST_ERRORLOGFMT_ADDA(( "\x83\x41\x83\x8b\x83\x74\x83\x40\x83\x65\x83\x58\x83\x67\x97\x70 16bit \x83\x4a\x83\x89\x81\x5b\x83\x74\x83\x48\x81\x5b\x83\x7d\x83\x62\x83\x67\x82\xcd %s \x82\xc5\x82\xb7"/*@ "�A���t�@�e�X�g�p 16bit �J���[�t�H�[�}�b�g�� %s �ł�" @*/, String )) ;

		// �A���t�@�e�X�g�p 32�r�b�g�J���[�t�H�[�}�b�g�̑I��
		if( Direct3D9_CheckDeviceFormat( UseAdapterNumber, D_D3DDEVTYPE_HAL, GD3D9.Device.Caps.ScreenFormat, 0, D_D3DRTYPE_TEXTURE, D_D3DFMT_A8R8G8B8 ) == D_D3D_OK )
		{
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ALPHATEST_RGB32 ] = D_D3DFMT_A8R8G8B8 ;
			String = "D3DFMT_A8R8G8B8" ;
		}
		else
		if( Direct3D9_CheckDeviceFormat( UseAdapterNumber, D_D3DDEVTYPE_HAL, GD3D9.Device.Caps.ScreenFormat, 0, D_D3DRTYPE_TEXTURE, D_D3DFMT_A1R5G5B5 ) == D_D3D_OK )
		{
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ALPHATEST_RGB32 ] = D_D3DFMT_A1R5G5B5 ;
			String = "D3DFMT_A1R5G5B5" ;
		}
		else
		if( Direct3D9_CheckDeviceFormat( UseAdapterNumber, D_D3DDEVTYPE_HAL, GD3D9.Device.Caps.ScreenFormat, 0, D_D3DRTYPE_TEXTURE, D_D3DFMT_A4R4G4B4 ) == D_D3D_OK )
		{
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ALPHATEST_RGB32 ] = D_D3DFMT_A4R4G4B4 ;
			String = "D3DFMT_A4R4G4B4" ;
		}
		else
		{
			DXST_ERRORLOG_ADDUTF16LE( "\x7f\x4f\x28\x75\x67\x30\x4d\x30\x8b\x30\xa2\x30\xeb\x30\xd5\x30\xa1\x30\xc6\x30\xb9\x30\xc8\x30\x28\x75\x20\x00\x33\x00\x32\x00\x62\x00\x69\x00\x74\x00\x20\x00\xab\x30\xe9\x30\xfc\x30\xd5\x30\xa9\x30\xfc\x30\xde\x30\xc3\x30\xc8\x30\x4c\x30\x8b\x89\x64\x30\x4b\x30\x8a\x30\x7e\x30\x5b\x30\x93\x30\x67\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�g�p�ł���A���t�@�e�X�g�p 32bit �J���[�t�H�[�}�b�g��������܂���ł���\n" @*/ ) ;
			goto ERR ;
		}
		DXST_ERRORLOGFMT_ADDA(( "\x83\x41\x83\x8b\x83\x74\x83\x40\x83\x65\x83\x58\x83\x67\x97\x70 32bit \x83\x4a\x83\x89\x81\x5b\x83\x74\x83\x48\x81\x5b\x83\x7d\x83\x62\x83\x67\x82\xcd %s \x82\xc5\x82\xb7"/*@ "�A���t�@�e�X�g�p 32bit �J���[�t�H�[�}�b�g�� %s �ł�" @*/, String )) ;

		// �p���b�g�J���[�t�H�[�}�b�g�̃Z�b�g
		if( GSYS.Screen.MainScreenColorBitDepth == 16 )
		{
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_PAL4           ] = GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_RGB16 ] ;
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_PAL8           ] = GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_RGB16 ] ;
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ALPHA_PAL4     ] = GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ALPHA_RGB16 ] ;
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ALPHA_PAL8     ] = GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ALPHA_RGB16 ] ;
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ALPHATEST_PAL4 ] = GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ALPHATEST_RGB16 ] ;
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ALPHATEST_PAL8 ] = GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ALPHATEST_RGB16 ] ;
		}
		else
		{
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_PAL4           ] = GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_RGB32 ] ;
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_PAL8           ] = GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_RGB32 ] ;
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ALPHA_PAL4     ] = GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ALPHA_RGB32 ] ;
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ALPHA_PAL8     ] = GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ALPHA_RGB32 ] ;
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ALPHATEST_PAL4 ] = GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ALPHATEST_RGB32 ] ;
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ALPHATEST_PAL8 ] = GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ALPHATEST_RGB32 ] ;
		}

		// DXT1�t�H�[�}�b�g�̃`�F�b�N
		if( Direct3D9_CheckDeviceFormat( UseAdapterNumber, D_D3DDEVTYPE_HAL, GD3D9.Device.Caps.ScreenFormat, 0, D_D3DRTYPE_TEXTURE, D_D3DFMT_DXT1 ) == D_D3D_OK )
		{
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DXT1 ] = D_D3DFMT_DXT1 ;
			String = "D3DFMT_DXT1 \x82\xc5\x82\xb7"/*@ "D3DFMT_DXT1 �ł�" @*/ ;
		}
		else
		{
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DXT1 ] = GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ALPHATEST_RGB16 ] ;
			String = "\x8e\x67\x82\xa6\x82\xdc\x82\xb9\x82\xf1"/*@ "�g���܂���" @*/ ;
		}
		DXST_ERRORLOGFMT_ADDA(( "\x82\x63\x82\x77\x82\x73\x82\x50\x83\x65\x83\x4e\x83\x58\x83\x60\x83\x83\x83\x74\x83\x48\x81\x5b\x83\x7d\x83\x62\x83\x67\x82\xcd%s"/*@ "�c�w�s�P�e�N�X�`���t�H�[�}�b�g��%s" @*/, String )) ;

		// DXT2�t�H�[�}�b�g�̃`�F�b�N
		if( Direct3D9_CheckDeviceFormat( UseAdapterNumber, D_D3DDEVTYPE_HAL, GD3D9.Device.Caps.ScreenFormat, 0, D_D3DRTYPE_TEXTURE, D_D3DFMT_DXT2 ) == D_D3D_OK )
		{
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DXT2 ] = D_D3DFMT_DXT2 ;
			String = "D3DFMT_DXT2 \x82\xc5\x82\xb7"/*@ "D3DFMT_DXT2 �ł�" @*/ ;
		}
		else
		{
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DXT2 ] = GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ALPHA_RGB32 ] ;
			String = "\x8e\x67\x82\xa6\x82\xdc\x82\xb9\x82\xf1"/*@ "�g���܂���" @*/ ;
		}
		DXST_ERRORLOGFMT_ADDA(( "\x82\x63\x82\x77\x82\x73\x82\x51\x83\x65\x83\x4e\x83\x58\x83\x60\x83\x83\x83\x74\x83\x48\x81\x5b\x83\x7d\x83\x62\x83\x67\x82\xcd%s"/*@ "�c�w�s�Q�e�N�X�`���t�H�[�}�b�g��%s" @*/, String )) ;

		// DXT3�t�H�[�}�b�g�̃`�F�b�N
		if( Direct3D9_CheckDeviceFormat( UseAdapterNumber, D_D3DDEVTYPE_HAL, GD3D9.Device.Caps.ScreenFormat, 0, D_D3DRTYPE_TEXTURE, D_D3DFMT_DXT3 ) == D_D3D_OK )
		{
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DXT3 ] = D_D3DFMT_DXT3 ;
			String = "D3DFMT_DXT3 \x82\xc5\x82\xb7"/*@ "D3DFMT_DXT3 �ł�" @*/ ;
		}
		else
		{
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DXT3 ] = GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ALPHA_RGB32 ] ;
			String = "\x8e\x67\x82\xa6\x82\xdc\x82\xb9\x82\xf1"/*@ "�g���܂���" @*/ ;
		}
		DXST_ERRORLOGFMT_ADDA(( "\x82\x63\x82\x77\x82\x73\x82\x52\x83\x65\x83\x4e\x83\x58\x83\x60\x83\x83\x83\x74\x83\x48\x81\x5b\x83\x7d\x83\x62\x83\x67\x82\xcd%s"/*@ "�c�w�s�R�e�N�X�`���t�H�[�}�b�g��%s" @*/, String )) ;

		// DXT4�t�H�[�}�b�g�̃`�F�b�N
		if( Direct3D9_CheckDeviceFormat( UseAdapterNumber, D_D3DDEVTYPE_HAL, GD3D9.Device.Caps.ScreenFormat, 0, D_D3DRTYPE_TEXTURE, D_D3DFMT_DXT4 ) == D_D3D_OK )
		{
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DXT4 ] = D_D3DFMT_DXT4 ;
			String = "D3DFMT_DXT4 \x82\xc5\x82\xb7"/*@ "D3DFMT_DXT4 �ł�" @*/ ;
		}
		else
		{
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DXT4 ] = GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ALPHA_RGB32 ] ;
			String = "\x8e\x67\x82\xa6\x82\xdc\x82\xb9\x82\xf1"/*@ "�g���܂���" @*/ ;
		}
		DXST_ERRORLOGFMT_ADDA(( "\x82\x63\x82\x77\x82\x73\x82\x53\x83\x65\x83\x4e\x83\x58\x83\x60\x83\x83\x83\x74\x83\x48\x81\x5b\x83\x7d\x83\x62\x83\x67\x82\xcd%s"/*@ "�c�w�s�S�e�N�X�`���t�H�[�}�b�g��%s" @*/, String )) ;

		// DXT5�t�H�[�}�b�g�̃`�F�b�N
		if( Direct3D9_CheckDeviceFormat( UseAdapterNumber, D_D3DDEVTYPE_HAL, GD3D9.Device.Caps.ScreenFormat, 0, D_D3DRTYPE_TEXTURE, D_D3DFMT_DXT5 ) == D_D3D_OK )
		{
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DXT5 ] = D_D3DFMT_DXT5 ;
			String = "D3DFMT_DXT5 \x82\xc5\x82\xb7"/*@ "D3DFMT_DXT5 �ł�" @*/ ;
		}
		else
		{
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DXT5 ] = GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ALPHA_RGB32 ] ;
			String = "\x8e\x67\x82\xa6\x82\xdc\x82\xb9\x82\xf1"/*@ "�g���܂���" @*/ ;
		}
		DXST_ERRORLOGFMT_ADDA(( "\x82\x63\x82\x77\x82\x73\x82\x54\x83\x65\x83\x4e\x83\x58\x83\x60\x83\x83\x83\x74\x83\x48\x81\x5b\x83\x7d\x83\x62\x83\x67\x82\xcd%s"/*@ "�c�w�s�T�e�N�X�`���t�H�[�}�b�g��%s" @*/, String )) ;







		// ARGB����16�r�b�g�^�J���[�t�H�[�}�b�g�̑I��
		if( Direct3D9_CheckDeviceFormat( UseAdapterNumber, D_D3DDEVTYPE_HAL, GD3D9.Device.Caps.ScreenFormat, 0, D_D3DRTYPE_TEXTURE, D_D3DFMT_A16B16G16R16 ) == D_D3D_OK )
		{
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ABGR_I16 ] = D_D3DFMT_A16B16G16R16 ;
			String = "D3DFMT_A16B16G16R16" ;
		}
		else
		{
			DXST_ERRORLOG_ADDUTF16LE( "\x7f\x4f\x28\x75\x67\x30\x4d\x30\x8b\x30\x41\x00\x42\x00\x47\x00\x52\x00\x74\x65\x70\x65\x31\x00\x36\x00\xd3\x30\xc3\x30\xc8\x30\x8b\x57\xab\x30\xe9\x30\xfc\x30\xd5\x30\xa9\x30\xfc\x30\xde\x30\xc3\x30\xc8\x30\x4c\x30\x42\x30\x8a\x30\x7e\x30\x5b\x30\x93\x30\x67\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�g�p�ł���ABGR����16�r�b�g�^�J���[�t�H�[�}�b�g������܂���ł���\n" @*/ ) ;
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ABGR_I16 ] = D_D3DFMT_UNKNOWN ;
		}

		if( GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ABGR_I16 ] != D_D3DFMT_UNKNOWN )
		{
			DXST_ERRORLOGFMT_ADDA(( "\x41\x42\x47\x52\x20\x90\xae\x90\x94\x20\x31\x36\x20\x83\x72\x83\x62\x83\x67\x8c\x5e\x83\x4a\x83\x89\x81\x5b\x83\x74\x83\x48\x81\x5b\x83\x7d\x83\x62\x83\x67\x82\xcd\x20\x25\x73\x20\x82\xc5\x82\xb7"/*@ "ABGR ���� 16 �r�b�g�^�J���[�t�H�[�}�b�g�� %s �ł�" @*/, String )) ;
		}

		// ARGB���������_16�r�b�g�^�J���[�t�H�[�}�b�g�̑I��
		if( Direct3D9_CheckDeviceFormat( UseAdapterNumber, D_D3DDEVTYPE_HAL, GD3D9.Device.Caps.ScreenFormat, 0, D_D3DRTYPE_TEXTURE, D_D3DFMT_A16B16G16R16F ) == D_D3D_OK )
		{
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ABGR_F16 ] = D_D3DFMT_A16B16G16R16F ;
			String = "D3DFMT_A16B16G16R16F" ;
		}
		else
		{
			DXST_ERRORLOG_ADDUTF16LE( "\x7f\x4f\x28\x75\x67\x30\x4d\x30\x8b\x30\x41\x00\x42\x00\x47\x00\x52\x00\x6e\x6d\xd5\x52\x0f\x5c\x70\x65\xb9\x70\x31\x00\x36\x00\xd3\x30\xc3\x30\xc8\x30\x8b\x57\xab\x30\xe9\x30\xfc\x30\xd5\x30\xa9\x30\xfc\x30\xde\x30\xc3\x30\xc8\x30\x4c\x30\x42\x30\x8a\x30\x7e\x30\x5b\x30\x93\x30\x67\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�g�p�ł���ABGR���������_16�r�b�g�^�J���[�t�H�[�}�b�g������܂���ł���\n" @*/ ) ;
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ABGR_F16 ] = D_D3DFMT_UNKNOWN ;
		}

		if( GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ABGR_F16 ] != D_D3DFMT_UNKNOWN )
		{
			DXST_ERRORLOGFMT_ADDA(( "\x41\x42\x47\x52\x20\x95\x82\x93\xae\x8f\xac\x90\x94\x93\x5f\x20\x31\x36\x20\x83\x72\x83\x62\x83\x67\x8c\x5e\x83\x4a\x83\x89\x81\x5b\x83\x74\x83\x48\x81\x5b\x83\x7d\x83\x62\x83\x67\x82\xcd\x20\x25\x73\x20\x82\xc5\x82\xb7"/*@ "ABGR ���������_ 16 �r�b�g�^�J���[�t�H�[�}�b�g�� %s �ł�" @*/, String )) ;
		}

		// ARGB���������_32�r�b�g�^�J���[�t�H�[�}�b�g�̑I��
		if( Direct3D9_CheckDeviceFormat( UseAdapterNumber, D_D3DDEVTYPE_HAL, GD3D9.Device.Caps.ScreenFormat, 0, D_D3DRTYPE_TEXTURE, D_D3DFMT_A32B32G32R32F ) == D_D3D_OK )
		{
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ABGR_F32 ] = D_D3DFMT_A32B32G32R32F ;
			String = "D3DFMT_A32B32G32R32F" ;
		}
		else
		if( Direct3D9_CheckDeviceFormat( UseAdapterNumber, D_D3DDEVTYPE_HAL, GD3D9.Device.Caps.ScreenFormat, 0, D_D3DRTYPE_TEXTURE, D_D3DFMT_A16B16G16R16F ) == D_D3D_OK )
		{
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ABGR_F32 ] = D_D3DFMT_A16B16G16R16F ;
			String = "D3DFMT_A16B16G16R16F" ;
		}
		else
		{
			DXST_ERRORLOG_ADDUTF16LE( "\x7f\x4f\x28\x75\x67\x30\x4d\x30\x8b\x30\x41\x00\x42\x00\x47\x00\x52\x00\x6e\x6d\xd5\x52\x0f\x5c\x70\x65\xb9\x70\x33\x00\x32\x00\xd3\x30\xc3\x30\xc8\x30\x8b\x57\xab\x30\xe9\x30\xfc\x30\xd5\x30\xa9\x30\xfc\x30\xde\x30\xc3\x30\xc8\x30\x4c\x30\x42\x30\x8a\x30\x7e\x30\x5b\x30\x93\x30\x67\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�g�p�ł���ABGR���������_32�r�b�g�^�J���[�t�H�[�}�b�g������܂���ł���\n" @*/ ) ;
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ABGR_F32 ] = D_D3DFMT_UNKNOWN ;
		}

		if( GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ABGR_F32 ] != D_D3DFMT_UNKNOWN )
		{
			DXST_ERRORLOGFMT_ADDA(( "\x41\x42\x47\x52\x20\x95\x82\x93\xae\x8f\xac\x90\x94\x93\x5f\x20\x33\x32\x20\x83\x72\x83\x62\x83\x67\x8c\x5e\x83\x4a\x83\x89\x81\x5b\x83\x74\x83\x48\x81\x5b\x83\x7d\x83\x62\x83\x67\x82\xcd\x20\x25\x73\x20\x82\xc5\x82\xb7"/*@ "ABGR ���������_ 32 �r�b�g�^�J���[�t�H�[�}�b�g�� %s �ł�" @*/, String )) ;
		}

		// �P�`�����l������8�r�b�g�^�J���[�t�H�[�}�b�g�̑I��
/*		if( Direct3D9_CheckDeviceFormat( UseAdapterNumber, D_D3DDEVTYPE_HAL, GD3D9.Device.Caps.ScreenFormat, 0, D_D3DRTYPE_TEXTURE, D_D3DFMT_L8 ) == D_D3D_OK )
		{
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ONE_I8 ] = D_D3DFMT_L8 ;
			String = "D3DFMT_L8" ;
		}
		else
		if( Direct3D9_CheckDeviceFormat( UseAdapterNumber, D_D3DDEVTYPE_HAL, GD3D9.Device.Caps.ScreenFormat, 0, D_D3DRTYPE_TEXTURE, D_D3DFMT_A8L8 ) == D_D3D_OK )
		{
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ONE_I8 ] = D_D3DFMT_A8L8 ;
			String = "D3DFMT_A8L8" ;
		}
		else*/
		if( Direct3D9_CheckDeviceFormat( UseAdapterNumber, D_D3DDEVTYPE_HAL, GD3D9.Device.Caps.ScreenFormat, 0, D_D3DRTYPE_TEXTURE, D_D3DFMT_A8R8G8B8 ) == D_D3D_OK )
		{
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ONE_I8 ] = D_D3DFMT_A8R8G8B8 ;
			String = "D3DFMT_A8R8G8B8" ;
		}
		else
		{
			DXST_ERRORLOG_ADDUTF16LE( "\x7f\x4f\x28\x75\x67\x30\x4d\x30\x8b\x30\x11\xff\xc1\x30\xe3\x30\xf3\x30\xcd\x30\xeb\x30\x74\x65\x70\x65\x38\x00\xd3\x30\xc3\x30\xc8\x30\x8b\x57\xab\x30\xe9\x30\xfc\x30\xd5\x30\xa9\x30\xfc\x30\xde\x30\xc3\x30\xc8\x30\x4c\x30\x42\x30\x8a\x30\x7e\x30\x5b\x30\x93\x30\x67\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�g�p�ł���P�`�����l������8�r�b�g�^�J���[�t�H�[�}�b�g������܂���ł���\n" @*/ ) ;
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ONE_I8 ] = D_D3DFMT_UNKNOWN ;
		}

		if( GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ONE_I8 ] != D_D3DFMT_UNKNOWN )
		{
			DXST_ERRORLOGFMT_ADDA(( "\x82\x50\x83\x60\x83\x83\x83\x93\x83\x6c\x83\x8b\x90\xae\x90\x94\x20\x38\x20\x83\x72\x83\x62\x83\x67\x8c\x5e\x83\x4a\x83\x89\x81\x5b\x83\x74\x83\x48\x81\x5b\x83\x7d\x83\x62\x83\x67\x82\xcd\x20\x25\x73\x20\x82\xc5\x82\xb7"/*@ "�P�`�����l������ 8 �r�b�g�^�J���[�t�H�[�}�b�g�� %s �ł�" @*/, String )) ;
		}

		// �P�`�����l������16�r�b�g�^�J���[�t�H�[�}�b�g�̑I��
/*		if( Direct3D9_CheckDeviceFormat( UseAdapterNumber, D_D3DDEVTYPE_HAL, GD3D9.Device.Caps.ScreenFormat, 0, D_D3DRTYPE_TEXTURE, D_D3DFMT_L16 ) == D_D3D_OK )
		{
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ONE_I16 ] = D_D3DFMT_L16 ;
			String = "D3DFMT_L16" ;
		}
		else*/
		if( Direct3D9_CheckDeviceFormat( UseAdapterNumber, D_D3DDEVTYPE_HAL, GD3D9.Device.Caps.ScreenFormat, 0, D_D3DRTYPE_TEXTURE, D_D3DFMT_G16R16 ) == D_D3D_OK )
		{
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ONE_I16 ] = D_D3DFMT_G16R16 ;
			String = "D3DFMT_G16R16" ;
		}
		else
		if( Direct3D9_CheckDeviceFormat( UseAdapterNumber, D_D3DDEVTYPE_HAL, GD3D9.Device.Caps.ScreenFormat, 0, D_D3DRTYPE_TEXTURE, D_D3DFMT_A16B16G16R16 ) == D_D3D_OK )
		{
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ONE_I16 ] = D_D3DFMT_A16B16G16R16 ;
			String = "D3DFMT_A16B16G16R16" ;
		}
		else
		{
			DXST_ERRORLOG_ADDUTF16LE( "\x7f\x4f\x28\x75\x67\x30\x4d\x30\x8b\x30\x11\xff\xc1\x30\xe3\x30\xf3\x30\xcd\x30\xeb\x30\x74\x65\x70\x65\x31\x00\x36\x00\xd3\x30\xc3\x30\xc8\x30\x8b\x57\xab\x30\xe9\x30\xfc\x30\xd5\x30\xa9\x30\xfc\x30\xde\x30\xc3\x30\xc8\x30\x4c\x30\x42\x30\x8a\x30\x7e\x30\x5b\x30\x93\x30\x67\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�g�p�ł���P�`�����l������16�r�b�g�^�J���[�t�H�[�}�b�g������܂���ł���\n" @*/ ) ;
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ONE_I16 ] = D_D3DFMT_UNKNOWN ;
		}

		if( GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ONE_I16 ] != D_D3DFMT_UNKNOWN )
		{
			DXST_ERRORLOGFMT_ADDA(( "\x82\x50\x83\x60\x83\x83\x83\x93\x83\x6c\x83\x8b\x90\xae\x90\x94\x20\x31\x36\x20\x83\x72\x83\x62\x83\x67\x8c\x5e\x83\x4a\x83\x89\x81\x5b\x83\x74\x83\x48\x81\x5b\x83\x7d\x83\x62\x83\x67\x82\xcd\x20\x25\x73\x20\x82\xc5\x82\xb7"/*@ "�P�`�����l������ 16 �r�b�g�^�J���[�t�H�[�}�b�g�� %s �ł�" @*/, String )) ;
		}

		// �P�`�����l�����������_16�r�b�g�^�J���[�t�H�[�}�b�g�̑I��
		if( Direct3D9_CheckDeviceFormat( UseAdapterNumber, D_D3DDEVTYPE_HAL, GD3D9.Device.Caps.ScreenFormat, 0, D_D3DRTYPE_TEXTURE, D_D3DFMT_R16F ) == D_D3D_OK )
		{
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ONE_F16 ] = D_D3DFMT_R16F ;
			String = "D3DFMT_R16F" ;
		}
		else
		if( Direct3D9_CheckDeviceFormat( UseAdapterNumber, D_D3DDEVTYPE_HAL, GD3D9.Device.Caps.ScreenFormat, 0, D_D3DRTYPE_TEXTURE, D_D3DFMT_G16R16F ) == D_D3D_OK )
		{
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ONE_F16 ] = D_D3DFMT_G16R16F ;
			String = "D3DFMT_G16R16F" ;
		}
		else
		if( Direct3D9_CheckDeviceFormat( UseAdapterNumber, D_D3DDEVTYPE_HAL, GD3D9.Device.Caps.ScreenFormat, 0, D_D3DRTYPE_TEXTURE, D_D3DFMT_A16B16G16R16F ) == D_D3D_OK )
		{
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ONE_F16 ] = D_D3DFMT_A16B16G16R16F ;
			String = "D3DFMT_A16B16G16R16F" ;
		}
		else
		if( Direct3D9_CheckDeviceFormat( UseAdapterNumber, D_D3DDEVTYPE_HAL, GD3D9.Device.Caps.ScreenFormat, 0, D_D3DRTYPE_TEXTURE, D_D3DFMT_A32B32G32R32F ) == D_D3D_OK )
		{
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ONE_F16 ] = D_D3DFMT_A32B32G32R32F ;
			String = "D3DFMT_A32B32G32R32F" ;
		}
		else
		{
			DXST_ERRORLOG_ADDUTF16LE( "\x7f\x4f\x28\x75\x67\x30\x4d\x30\x8b\x30\x11\xff\xc1\x30\xe3\x30\xf3\x30\xcd\x30\xeb\x30\x6e\x6d\xd5\x52\x0f\x5c\x70\x65\xb9\x70\x31\x00\x36\x00\xd3\x30\xc3\x30\xc8\x30\x8b\x57\xab\x30\xe9\x30\xfc\x30\xd5\x30\xa9\x30\xfc\x30\xde\x30\xc3\x30\xc8\x30\x4c\x30\x42\x30\x8a\x30\x7e\x30\x5b\x30\x93\x30\x67\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�g�p�ł���P�`�����l�����������_16�r�b�g�^�J���[�t�H�[�}�b�g������܂���ł���\n" @*/ ) ;
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ONE_F16 ] = D_D3DFMT_UNKNOWN ;
		}

		if( GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ONE_F16 ] != D_D3DFMT_UNKNOWN )
		{
			DXST_ERRORLOGFMT_ADDA(( "\x82\x50\x83\x60\x83\x83\x83\x93\x83\x6c\x83\x8b\x95\x82\x93\xae\x8f\xac\x90\x94\x93\x5f\x20\x31\x36\x20\x83\x72\x83\x62\x83\x67\x8c\x5e\x83\x4a\x83\x89\x81\x5b\x83\x74\x83\x48\x81\x5b\x83\x7d\x83\x62\x83\x67\x82\xcd\x20\x25\x73\x20\x82\xc5\x82\xb7"/*@ "�P�`�����l�����������_ 16 �r�b�g�^�J���[�t�H�[�}�b�g�� %s �ł�" @*/, String )) ;
		}

		// �P�`�����l�����������_32�r�b�g�^�J���[�t�H�[�}�b�g�̑I��
		if( Direct3D9_CheckDeviceFormat( UseAdapterNumber, D_D3DDEVTYPE_HAL, GD3D9.Device.Caps.ScreenFormat, 0, D_D3DRTYPE_TEXTURE, D_D3DFMT_R32F ) == D_D3D_OK )
		{
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ONE_F32 ] = D_D3DFMT_R32F ;
			String = "D3DFMT_R32F" ;
		}
		else
		if( Direct3D9_CheckDeviceFormat( UseAdapterNumber, D_D3DDEVTYPE_HAL, GD3D9.Device.Caps.ScreenFormat, 0, D_D3DRTYPE_TEXTURE, D_D3DFMT_G32R32F ) == D_D3D_OK )
		{
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ONE_F32 ] = D_D3DFMT_G32R32F ;
			String = "D3DFMT_G32R32F" ;
		}
		else
		if( Direct3D9_CheckDeviceFormat( UseAdapterNumber, D_D3DDEVTYPE_HAL, GD3D9.Device.Caps.ScreenFormat, 0, D_D3DRTYPE_TEXTURE, D_D3DFMT_A32B32G32R32F ) == D_D3D_OK )
		{
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ONE_F32 ] = D_D3DFMT_A32B32G32R32F ;
			String = "D3DFMT_A32B32G32R32F" ;
		}
		else
		if( Direct3D9_CheckDeviceFormat( UseAdapterNumber, D_D3DDEVTYPE_HAL, GD3D9.Device.Caps.ScreenFormat, 0, D_D3DRTYPE_TEXTURE, D_D3DFMT_A16B16G16R16F ) == D_D3D_OK )
		{
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ONE_F32 ] = D_D3DFMT_A16B16G16R16F ;
			String = "D3DFMT_A16B16G16R16F" ;
		}
		else
		{
			DXST_ERRORLOG_ADDUTF16LE( "\x7f\x4f\x28\x75\x67\x30\x4d\x30\x8b\x30\x11\xff\xc1\x30\xe3\x30\xf3\x30\xcd\x30\xeb\x30\x6e\x6d\xd5\x52\x0f\x5c\x70\x65\xb9\x70\x33\x00\x32\x00\xd3\x30\xc3\x30\xc8\x30\x8b\x57\xab\x30\xe9\x30\xfc\x30\xd5\x30\xa9\x30\xfc\x30\xde\x30\xc3\x30\xc8\x30\x4c\x30\x42\x30\x8a\x30\x7e\x30\x5b\x30\x93\x30\x67\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�g�p�ł���P�`�����l�����������_32�r�b�g�^�J���[�t�H�[�}�b�g������܂���ł���\n" @*/ ) ;
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ONE_F32 ] = D_D3DFMT_UNKNOWN ;
		}

		if( GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ONE_F32 ] != D_D3DFMT_UNKNOWN )
		{
			DXST_ERRORLOGFMT_ADDA(( "\x82\x50\x83\x60\x83\x83\x83\x93\x83\x6c\x83\x8b\x95\x82\x93\xae\x8f\xac\x90\x94\x93\x5f\x20\x33\x32\x20\x83\x72\x83\x62\x83\x67\x8c\x5e\x83\x4a\x83\x89\x81\x5b\x83\x74\x83\x48\x81\x5b\x83\x7d\x83\x62\x83\x67\x82\xcd\x20\x25\x73\x20\x82\xc5\x82\xb7"/*@ "�P�`�����l�����������_ 32 �r�b�g�^�J���[�t�H�[�}�b�g�� %s �ł�" @*/, String )) ;
		}

		// �Q�`�����l������8�r�b�g�^�J���[�t�H�[�}�b�g�̑I��
/*		if( Direct3D9_CheckDeviceFormat( UseAdapterNumber, D_D3DDEVTYPE_HAL, GD3D9.Device.Caps.ScreenFormat, 0, D_D3DRTYPE_TEXTURE, D_D3DFMT_A8L8 ) == D_D3D_OK )
		{
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_TWO_I8 ] = D_D3DFMT_A8L8 ;
			String = "D3DFMT_A8L8" ;
		}
		else*/
		if( Direct3D9_CheckDeviceFormat( UseAdapterNumber, D_D3DDEVTYPE_HAL, GD3D9.Device.Caps.ScreenFormat, 0, D_D3DRTYPE_TEXTURE, D_D3DFMT_A8R8G8B8 ) == D_D3D_OK )
		{
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_TWO_I8 ] = D_D3DFMT_A8R8G8B8 ;
			String = "D3DFMT_A8B8G8R8" ;
		}
		else
		{
			DXST_ERRORLOG_ADDUTF16LE( "\x7f\x4f\x28\x75\x67\x30\x4d\x30\x8b\x30\x12\xff\xc1\x30\xe3\x30\xf3\x30\xcd\x30\xeb\x30\x74\x65\x70\x65\x38\x00\xd3\x30\xc3\x30\xc8\x30\x8b\x57\xab\x30\xe9\x30\xfc\x30\xd5\x30\xa9\x30\xfc\x30\xde\x30\xc3\x30\xc8\x30\x4c\x30\x42\x30\x8a\x30\x7e\x30\x5b\x30\x93\x30\x67\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�g�p�ł���Q�`�����l������8�r�b�g�^�J���[�t�H�[�}�b�g������܂���ł���\n" @*/ ) ;
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_TWO_I8 ] = D_D3DFMT_UNKNOWN ;
		}

		if( GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_TWO_I8 ] != D_D3DFMT_UNKNOWN )
		{
			DXST_ERRORLOGFMT_ADDA(( "\x82\x51\x83\x60\x83\x83\x83\x93\x83\x6c\x83\x8b\x90\xae\x90\x94\x20\x38\x20\x83\x72\x83\x62\x83\x67\x8c\x5e\x83\x4a\x83\x89\x81\x5b\x83\x74\x83\x48\x81\x5b\x83\x7d\x83\x62\x83\x67\x82\xcd\x20\x25\x73\x20\x82\xc5\x82\xb7"/*@ "�Q�`�����l������ 8 �r�b�g�^�J���[�t�H�[�}�b�g�� %s �ł�" @*/, String )) ;
		}

		// �Q�`�����l������16�r�b�g�^�J���[�t�H�[�}�b�g�̑I��
		if( Direct3D9_CheckDeviceFormat( UseAdapterNumber, D_D3DDEVTYPE_HAL, GD3D9.Device.Caps.ScreenFormat, 0, D_D3DRTYPE_TEXTURE, D_D3DFMT_G16R16 ) == D_D3D_OK )
		{
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_TWO_I16 ] = D_D3DFMT_G16R16 ;
			String = "D3DFMT_G16R16" ;
		}
		else
		if( Direct3D9_CheckDeviceFormat( UseAdapterNumber, D_D3DDEVTYPE_HAL, GD3D9.Device.Caps.ScreenFormat, 0, D_D3DRTYPE_TEXTURE, D_D3DFMT_A16B16G16R16 ) == D_D3D_OK )
		{
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_TWO_I16 ] = D_D3DFMT_A16B16G16R16 ;
			String = "D3DFMT_A16B16G16R16" ;
		}
		else
		{
			DXST_ERRORLOG_ADDUTF16LE( "\x7f\x4f\x28\x75\x67\x30\x4d\x30\x8b\x30\x12\xff\xc1\x30\xe3\x30\xf3\x30\xcd\x30\xeb\x30\x74\x65\x70\x65\x31\x00\x36\x00\xd3\x30\xc3\x30\xc8\x30\x8b\x57\xab\x30\xe9\x30\xfc\x30\xd5\x30\xa9\x30\xfc\x30\xde\x30\xc3\x30\xc8\x30\x4c\x30\x42\x30\x8a\x30\x7e\x30\x5b\x30\x93\x30\x67\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�g�p�ł���Q�`�����l������16�r�b�g�^�J���[�t�H�[�}�b�g������܂���ł���\n" @*/ ) ;
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_TWO_I16 ] = D_D3DFMT_UNKNOWN ;
		}

		if( GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_TWO_I16 ] != D_D3DFMT_UNKNOWN )
		{
			DXST_ERRORLOGFMT_ADDA(( "\x82\x51\x83\x60\x83\x83\x83\x93\x83\x6c\x83\x8b\x90\xae\x90\x94\x20\x31\x36\x20\x83\x72\x83\x62\x83\x67\x8c\x5e\x83\x4a\x83\x89\x81\x5b\x83\x74\x83\x48\x81\x5b\x83\x7d\x83\x62\x83\x67\x82\xcd\x20\x25\x73\x20\x82\xc5\x82\xb7"/*@ "�Q�`�����l������ 16 �r�b�g�^�J���[�t�H�[�}�b�g�� %s �ł�" @*/, String )) ;
		}

		// �Q�`�����l�����������_16�r�b�g�^�J���[�t�H�[�}�b�g�̑I��
		if( Direct3D9_CheckDeviceFormat( UseAdapterNumber, D_D3DDEVTYPE_HAL, GD3D9.Device.Caps.ScreenFormat, 0, D_D3DRTYPE_TEXTURE, D_D3DFMT_G16R16F ) == D_D3D_OK )
		{
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_TWO_F16 ] = D_D3DFMT_G16R16F ;
			String = "D3DFMT_G16R16F" ;
		}
		else
		if( Direct3D9_CheckDeviceFormat( UseAdapterNumber, D_D3DDEVTYPE_HAL, GD3D9.Device.Caps.ScreenFormat, 0, D_D3DRTYPE_TEXTURE, D_D3DFMT_A16B16G16R16F ) == D_D3D_OK )
		{
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_TWO_F16 ] = D_D3DFMT_A16B16G16R16F ;
			String = "D3DFMT_A16B16G16R16F" ;
		}
		else
		if( Direct3D9_CheckDeviceFormat( UseAdapterNumber, D_D3DDEVTYPE_HAL, GD3D9.Device.Caps.ScreenFormat, 0, D_D3DRTYPE_TEXTURE, D_D3DFMT_A32B32G32R32F ) == D_D3D_OK )
		{
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_TWO_F16 ] = D_D3DFMT_A32B32G32R32F ;
			String = "D3DFMT_A32B32G32R32F" ;
		}
		else
		{
			DXST_ERRORLOG_ADDUTF16LE( "\x7f\x4f\x28\x75\x67\x30\x4d\x30\x8b\x30\x12\xff\xc1\x30\xe3\x30\xf3\x30\xcd\x30\xeb\x30\x6e\x6d\xd5\x52\x0f\x5c\x70\x65\xb9\x70\x31\x00\x36\x00\xd3\x30\xc3\x30\xc8\x30\x8b\x57\xab\x30\xe9\x30\xfc\x30\xd5\x30\xa9\x30\xfc\x30\xde\x30\xc3\x30\xc8\x30\x4c\x30\x42\x30\x8a\x30\x7e\x30\x5b\x30\x93\x30\x67\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�g�p�ł���Q�`�����l�����������_16�r�b�g�^�J���[�t�H�[�}�b�g������܂���ł���\n" @*/ ) ;
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_TWO_F16 ] = D_D3DFMT_UNKNOWN ;
		}

		if( GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_TWO_F16 ] != D_D3DFMT_UNKNOWN )
		{
			DXST_ERRORLOGFMT_ADDA(( "\x82\x51\x83\x60\x83\x83\x83\x93\x83\x6c\x83\x8b\x95\x82\x93\xae\x8f\xac\x90\x94\x93\x5f\x20\x31\x36\x20\x83\x72\x83\x62\x83\x67\x8c\x5e\x83\x4a\x83\x89\x81\x5b\x83\x74\x83\x48\x81\x5b\x83\x7d\x83\x62\x83\x67\x82\xcd\x20\x25\x73\x20\x82\xc5\x82\xb7"/*@ "�Q�`�����l�����������_ 16 �r�b�g�^�J���[�t�H�[�}�b�g�� %s �ł�" @*/, String )) ;
		}

		// �Q�`�����l�����������_32�r�b�g�^�J���[�t�H�[�}�b�g�̑I��
		if( Direct3D9_CheckDeviceFormat( UseAdapterNumber, D_D3DDEVTYPE_HAL, GD3D9.Device.Caps.ScreenFormat, 0, D_D3DRTYPE_TEXTURE, D_D3DFMT_G32R32F ) == D_D3D_OK )
		{
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_TWO_F32 ] = D_D3DFMT_G32R32F ;
			String = "D3DFMT_G32R32F" ;
		}
		else
		if( Direct3D9_CheckDeviceFormat( UseAdapterNumber, D_D3DDEVTYPE_HAL, GD3D9.Device.Caps.ScreenFormat, 0, D_D3DRTYPE_TEXTURE, D_D3DFMT_A32B32G32R32F ) == D_D3D_OK )
		{
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_TWO_F32 ] = D_D3DFMT_A32B32G32R32F ;
			String = "D3DFMT_A32B32G32R32F" ;
		}
		else
		if( Direct3D9_CheckDeviceFormat( UseAdapterNumber, D_D3DDEVTYPE_HAL, GD3D9.Device.Caps.ScreenFormat, 0, D_D3DRTYPE_TEXTURE, D_D3DFMT_A16B16G16R16F ) == D_D3D_OK )
		{
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_TWO_F32 ] = D_D3DFMT_A16B16G16R16F ;
			String = "D3DFMT_A16B16G16R16F" ;
		}
		else
		{
			DXST_ERRORLOG_ADDUTF16LE( "\x7f\x4f\x28\x75\x67\x30\x4d\x30\x8b\x30\x12\xff\xc1\x30\xe3\x30\xf3\x30\xcd\x30\xeb\x30\x6e\x6d\xd5\x52\x0f\x5c\x70\x65\xb9\x70\x33\x00\x32\x00\xd3\x30\xc3\x30\xc8\x30\x8b\x57\xab\x30\xe9\x30\xfc\x30\xd5\x30\xa9\x30\xfc\x30\xde\x30\xc3\x30\xc8\x30\x4c\x30\x42\x30\x8a\x30\x7e\x30\x5b\x30\x93\x30\x67\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�g�p�ł���Q�`�����l�����������_32�r�b�g�^�J���[�t�H�[�}�b�g������܂���ł���\n" @*/ ) ;
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_TWO_F32 ] = D_D3DFMT_UNKNOWN ;
		}

		if( GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_TWO_F32 ] != D_D3DFMT_UNKNOWN )
		{
			DXST_ERRORLOGFMT_ADDA(( "\x82\x51\x83\x60\x83\x83\x83\x93\x83\x6c\x83\x8b\x95\x82\x93\xae\x8f\xac\x90\x94\x93\x5f\x20\x33\x32\x20\x83\x72\x83\x62\x83\x67\x8c\x5e\x83\x4a\x83\x89\x81\x5b\x83\x74\x83\x48\x81\x5b\x83\x7d\x83\x62\x83\x67\x82\xcd\x20\x25\x73\x20\x82\xc5\x82\xb7"/*@ "�Q�`�����l�����������_ 32 �r�b�g�^�J���[�t�H�[�}�b�g�� %s �ł�" @*/, String )) ;
		}







		// �`��\ 16�r�b�g�J���[�t�H�[�}�b�g�̑I��
		if( Direct3D9_CheckDeviceFormat( UseAdapterNumber, D_D3DDEVTYPE_HAL, GD3D9.Device.Caps.ScreenFormat, D_D3DUSAGE_RENDERTARGET, D_D3DRTYPE_TEXTURE, GD3D9.Device.Caps.ScreenFormat == D_D3DFMT_R5G6B5 ? D_D3DFMT_R5G6B5 : D_D3DFMT_X1R5G5B5 ) == D_D3D_OK )
		{
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_RGB16 ] = GD3D9.Device.Caps.ScreenFormat == D_D3DFMT_R5G6B5 ? D_D3DFMT_R5G6B5 : D_D3DFMT_X1R5G5B5 ;
			String = GD3D9.Device.Caps.ScreenFormat == D_D3DFMT_R5G6B5 ? "D3DFMT_R5G6B5" : "D3DFMT_R5G5B5" ;
		}
		else
		if( Direct3D9_CheckDeviceFormat( UseAdapterNumber, D_D3DDEVTYPE_HAL, GD3D9.Device.Caps.ScreenFormat, D_D3DUSAGE_RENDERTARGET, D_D3DRTYPE_TEXTURE, GD3D9.Device.Caps.ScreenFormat == D_D3DFMT_R5G6B5 ? D_D3DFMT_X1R5G5B5 : D_D3DFMT_R5G6B5 ) == D_D3D_OK )
		{
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_RGB16 ] = GD3D9.Device.Caps.ScreenFormat == D_D3DFMT_R5G6B5 ? D_D3DFMT_X1R5G5B5 : D_D3DFMT_R5G6B5 ;
			String = GD3D9.Device.Caps.ScreenFormat == D_D3DFMT_R5G6B5 ? "D3DFMT_R5G5B5" : "D3DFMT_R5G6B5" ;
		}
		else
		if( Direct3D9_CheckDeviceFormat( UseAdapterNumber, D_D3DDEVTYPE_HAL, GD3D9.Device.Caps.ScreenFormat, D_D3DUSAGE_RENDERTARGET, D_D3DRTYPE_TEXTURE, D_D3DFMT_X8R8G8B8 ) == D_D3D_OK )
		{
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_RGB16 ] = D_D3DFMT_X8R8G8B8 ;
			String = "D3DFMT_X8R8G8B8" ;
		}
		else
		{
			DXST_ERRORLOG_ADDUTF16LE( "\x7f\x4f\x28\x75\x67\x30\x4d\x30\x8b\x30\xcf\x63\x3b\x75\x28\x75\x20\x00\x31\x00\x36\x00\x62\x00\x69\x00\x74\x00\x20\x00\xab\x30\xe9\x30\xfc\x30\xd5\x30\xa9\x30\xfc\x30\xde\x30\xc3\x30\xc8\x30\x4c\x30\x42\x30\x8a\x30\x7e\x30\x5b\x30\x93\x30\x67\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�g�p�ł���`��p 16bit �J���[�t�H�[�}�b�g������܂���ł���\n" @*/ ) ;
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_RGB16 ] = D_D3DFMT_UNKNOWN ;
		}

		if( GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_RGB16 ] != D_D3DFMT_UNKNOWN )
		{
			DXST_ERRORLOGFMT_ADDA(( "\x95\x60\x89\xe6\x97\x70 16bit \x83\x4a\x83\x89\x81\x5b\x83\x74\x83\x48\x81\x5b\x83\x7d\x83\x62\x83\x67\x82\xcd %s \x82\xc5\x82\xb7"/*@ "�`��p 16bit �J���[�t�H�[�}�b�g�� %s �ł�" @*/, String )) ;
		}

		// �`��\ 32�r�b�g�J���[�t�H�[�}�b�g�̑I��
		if( Direct3D9_CheckDeviceFormat( UseAdapterNumber, D_D3DDEVTYPE_HAL, GD3D9.Device.Caps.ScreenFormat, D_D3DUSAGE_RENDERTARGET, D_D3DRTYPE_TEXTURE, D_D3DFMT_X8R8G8B8 ) == D_D3D_OK )
		{
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_RGB32 ] = D_D3DFMT_X8R8G8B8 ;
			String = "D3DFMT_X8R8G8B8" ;
		}
		else
		if( Direct3D9_CheckDeviceFormat( UseAdapterNumber, D_D3DDEVTYPE_HAL, GD3D9.Device.Caps.ScreenFormat, D_D3DUSAGE_RENDERTARGET, D_D3DRTYPE_TEXTURE, D_D3DFMT_R5G6B5 ) == D_D3D_OK )
		{
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_RGB32 ] = D_D3DFMT_R5G6B5 ;
			String = "D3DFMT_R5G6B5" ;
		}
		else
		if( Direct3D9_CheckDeviceFormat( UseAdapterNumber, D_D3DDEVTYPE_HAL, GD3D9.Device.Caps.ScreenFormat, D_D3DUSAGE_RENDERTARGET, D_D3DRTYPE_TEXTURE, D_D3DFMT_X1R5G5B5 ) == D_D3D_OK )
		{
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_RGB32 ] = D_D3DFMT_X1R5G5B5 ;
			String = "D3DFMT_R5G5B5" ;
		}
		else
		{
			DXST_ERRORLOG_ADDUTF16LE( "\x7f\x4f\x28\x75\x67\x30\x4d\x30\x8b\x30\xcf\x63\x3b\x75\x28\x75\x20\x00\x33\x00\x32\x00\x62\x00\x69\x00\x74\x00\x20\x00\xab\x30\xe9\x30\xfc\x30\xd5\x30\xa9\x30\xfc\x30\xde\x30\xc3\x30\xc8\x30\x4c\x30\x42\x30\x8a\x30\x7e\x30\x5b\x30\x93\x30\x67\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�g�p�ł���`��p 32bit �J���[�t�H�[�}�b�g������܂���ł���\n" @*/ ) ;
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_RGB32 ] = D_D3DFMT_UNKNOWN ;
		}

		if( GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_RGB32 ] != D_D3DFMT_UNKNOWN )
		{
			DXST_ERRORLOGFMT_ADDA(( "\x95\x60\x89\xe6\x97\x70 32bit \x83\x4a\x83\x89\x81\x5b\x83\x74\x83\x48\x81\x5b\x83\x7d\x83\x62\x83\x67\x82\xcd %s \x82\xc5\x82\xb7"/*@ "�`��p 32bit �J���[�t�H�[�}�b�g�� %s �ł�" @*/, String )) ;
		}

		// �`��\ �A���t�@�`�����l���t�� 32 �r�b�g�J���[�t�H�[�}�b�g�̑I��
		if( Direct3D9_CheckDeviceFormat( UseAdapterNumber, D_D3DDEVTYPE_HAL, GD3D9.Device.Caps.ScreenFormat, D_D3DUSAGE_RENDERTARGET, D_D3DRTYPE_TEXTURE, D_D3DFMT_A8R8G8B8 ) == D_D3D_OK )
		{
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ALPHA_RGB32 ] = D_D3DFMT_A8R8G8B8 ;
			String = "D3DFMT_A8R8G8B8" ;
		}
		else
		if( Direct3D9_CheckDeviceFormat( UseAdapterNumber, D_D3DDEVTYPE_HAL, GD3D9.Device.Caps.ScreenFormat, D_D3DUSAGE_RENDERTARGET, D_D3DRTYPE_TEXTURE, D_D3DFMT_A4R4G4B4 ) == D_D3D_OK )
		{
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ALPHA_RGB32 ] = D_D3DFMT_A4R4G4B4 ;
			String = "D3DFMT_A4R4G4B4" ;
		}
		else
		{
			DXST_ERRORLOG_ADDUTF16LE( "\x7f\x4f\x28\x75\x67\x30\x4d\x30\x8b\x30\xcf\x63\x3b\x75\x28\x75\xa2\x30\xeb\x30\xd5\x30\xa1\x30\xd8\x4e\x4d\x30\x20\x00\x33\x00\x32\x00\x62\x00\x69\x00\x74\x00\x20\x00\xab\x30\xe9\x30\xfc\x30\xd5\x30\xa9\x30\xfc\x30\xde\x30\xc3\x30\xc8\x30\x4c\x30\x42\x30\x8a\x30\x7e\x30\x5b\x30\x93\x30\x67\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�g�p�ł���`��p�A���t�@�t�� 32bit �J���[�t�H�[�}�b�g������܂���ł���\n" @*/ ) ;
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ALPHA_RGB32 ] = D_D3DFMT_UNKNOWN ;
		}

		if( GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ALPHA_RGB32 ] != D_D3DFMT_UNKNOWN )
		{
			DXST_ERRORLOGFMT_ADDA(( "\x95\x60\x89\xe6\x97\x70\x83\x41\x83\x8b\x83\x74\x83\x40\x95\x74\x82\xab 32bit \x83\x4a\x83\x89\x81\x5b\x83\x74\x83\x48\x81\x5b\x83\x7d\x83\x62\x83\x67\x82\xcd %s \x82\xc5\x82\xb7"/*@ "�`��p�A���t�@�t�� 32bit �J���[�t�H�[�}�b�g�� %s �ł�" @*/, String )) ;
		}

		// �`��\ ARGB����16�r�b�g�^�J���[�t�H�[�}�b�g�̑I��
		if( Direct3D9_CheckDeviceFormat( UseAdapterNumber, D_D3DDEVTYPE_HAL, GD3D9.Device.Caps.ScreenFormat, D_D3DUSAGE_RENDERTARGET, D_D3DRTYPE_TEXTURE, D_D3DFMT_A16B16G16R16 ) == D_D3D_OK )
		{
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ABGR_I16 ] = D_D3DFMT_A16B16G16R16 ;
			String = "D3DFMT_A16B16G16R16" ;
		}
		else
		{
			DXST_ERRORLOG_ADDUTF16LE( "\x7f\x4f\x28\x75\x67\x30\x4d\x30\x8b\x30\xcf\x63\x3b\x75\x28\x75\x41\x00\x42\x00\x47\x00\x52\x00\x74\x65\x70\x65\x31\x00\x36\x00\xd3\x30\xc3\x30\xc8\x30\x8b\x57\xab\x30\xe9\x30\xfc\x30\xd5\x30\xa9\x30\xfc\x30\xde\x30\xc3\x30\xc8\x30\x4c\x30\x42\x30\x8a\x30\x7e\x30\x5b\x30\x93\x30\x67\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�g�p�ł���`��pABGR����16�r�b�g�^�J���[�t�H�[�}�b�g������܂���ł���\n" @*/ ) ;
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ABGR_I16 ] = D_D3DFMT_UNKNOWN ;
		}

		if( GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ABGR_I16 ] != D_D3DFMT_UNKNOWN )
		{
			DXST_ERRORLOGFMT_ADDA(( "\x95\x60\x89\xe6\x97\x70 ABGR \x90\xae\x90\x94 16 \x83\x72\x83\x62\x83\x67\x8c\x5e\x83\x4a\x83\x89\x81\x5b\x83\x74\x83\x48\x81\x5b\x83\x7d\x83\x62\x83\x67\x82\xcd %s \x82\xc5\x82\xb7"/*@ "�`��p ABGR ���� 16 �r�b�g�^�J���[�t�H�[�}�b�g�� %s �ł�" @*/, String )) ;
		}

		// �`��\ ARGB���������_16�r�b�g�^�J���[�t�H�[�}�b�g�̑I��
		if( Direct3D9_CheckDeviceFormat( UseAdapterNumber, D_D3DDEVTYPE_HAL, GD3D9.Device.Caps.ScreenFormat, D_D3DUSAGE_RENDERTARGET, D_D3DRTYPE_TEXTURE, D_D3DFMT_A16B16G16R16F ) == D_D3D_OK )
		{
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ABGR_F16 ] = D_D3DFMT_A16B16G16R16F ;
			String = "D3DFMT_A16B16G16R16F" ;
		}
		else
		{
			DXST_ERRORLOG_ADDUTF16LE( "\x7f\x4f\x28\x75\x67\x30\x4d\x30\x8b\x30\xcf\x63\x3b\x75\x28\x75\x41\x00\x42\x00\x47\x00\x52\x00\x6e\x6d\xd5\x52\x0f\x5c\x70\x65\xb9\x70\x31\x00\x36\x00\xd3\x30\xc3\x30\xc8\x30\x8b\x57\xab\x30\xe9\x30\xfc\x30\xd5\x30\xa9\x30\xfc\x30\xde\x30\xc3\x30\xc8\x30\x4c\x30\x42\x30\x8a\x30\x7e\x30\x5b\x30\x93\x30\x67\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�g�p�ł���`��pABGR���������_16�r�b�g�^�J���[�t�H�[�}�b�g������܂���ł���\n" @*/ ) ;
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ABGR_F16 ] = D_D3DFMT_UNKNOWN ;
		}

		if( GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ABGR_F16 ] != D_D3DFMT_UNKNOWN )
		{
			DXST_ERRORLOGFMT_ADDA(( "\x95\x60\x89\xe6\x97\x70 ABGR \x95\x82\x93\xae\x8f\xac\x90\x94\x93\x5f 16 \x83\x72\x83\x62\x83\x67\x8c\x5e\x83\x4a\x83\x89\x81\x5b\x83\x74\x83\x48\x81\x5b\x83\x7d\x83\x62\x83\x67\x82\xcd %s \x82\xc5\x82\xb7"/*@ "�`��p ABGR ���������_ 16 �r�b�g�^�J���[�t�H�[�}�b�g�� %s �ł�" @*/, String )) ;
		}

		// �`��\ ARGB���������_32�r�b�g�^�J���[�t�H�[�}�b�g�̑I��
		if( Direct3D9_CheckDeviceFormat( UseAdapterNumber, D_D3DDEVTYPE_HAL, GD3D9.Device.Caps.ScreenFormat, D_D3DUSAGE_RENDERTARGET, D_D3DRTYPE_TEXTURE, D_D3DFMT_A32B32G32R32F ) == D_D3D_OK )
		{
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ABGR_F32 ] = D_D3DFMT_A32B32G32R32F ;
			String = "D3DFMT_A32B32G32R32F" ;
		}
		else
		if( Direct3D9_CheckDeviceFormat( UseAdapterNumber, D_D3DDEVTYPE_HAL, GD3D9.Device.Caps.ScreenFormat, D_D3DUSAGE_RENDERTARGET, D_D3DRTYPE_TEXTURE, D_D3DFMT_A16B16G16R16F ) == D_D3D_OK )
		{
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ABGR_F32 ] = D_D3DFMT_A16B16G16R16F ;
			String = "D3DFMT_A16B16G16R16F" ;
		}
		else
		{
			DXST_ERRORLOG_ADDUTF16LE( "\x7f\x4f\x28\x75\x67\x30\x4d\x30\x8b\x30\xcf\x63\x3b\x75\x28\x75\x41\x00\x42\x00\x47\x00\x52\x00\x6e\x6d\xd5\x52\x0f\x5c\x70\x65\xb9\x70\x33\x00\x32\x00\xd3\x30\xc3\x30\xc8\x30\x8b\x57\xab\x30\xe9\x30\xfc\x30\xd5\x30\xa9\x30\xfc\x30\xde\x30\xc3\x30\xc8\x30\x4c\x30\x42\x30\x8a\x30\x7e\x30\x5b\x30\x93\x30\x67\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�g�p�ł���`��pABGR���������_32�r�b�g�^�J���[�t�H�[�}�b�g������܂���ł���\n" @*/ ) ;
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ABGR_F32 ] = D_D3DFMT_UNKNOWN ;
		}

		if( GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ABGR_F32 ] != D_D3DFMT_UNKNOWN )
		{
			DXST_ERRORLOGFMT_ADDA(( "\x95\x60\x89\xe6\x97\x70 ABGR \x95\x82\x93\xae\x8f\xac\x90\x94\x93\x5f 32 \x83\x72\x83\x62\x83\x67\x8c\x5e\x83\x4a\x83\x89\x81\x5b\x83\x74\x83\x48\x81\x5b\x83\x7d\x83\x62\x83\x67\x82\xcd %s \x82\xc5\x82\xb7"/*@ "�`��p ABGR ���������_ 32 �r�b�g�^�J���[�t�H�[�}�b�g�� %s �ł�" @*/, String )) ;
		}

		// �`��\ �P�`�����l������8�r�b�g�^�J���[�t�H�[�}�b�g�̑I��
		if( Direct3D9_CheckDeviceFormat( UseAdapterNumber, D_D3DDEVTYPE_HAL, GD3D9.Device.Caps.ScreenFormat, D_D3DUSAGE_RENDERTARGET, D_D3DRTYPE_TEXTURE, D_D3DFMT_L8 ) == D_D3D_OK )
		{
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ONE_I8 ] = D_D3DFMT_L8 ;
			String = "D3DFMT_L8" ;
		}
		else
		if( Direct3D9_CheckDeviceFormat( UseAdapterNumber, D_D3DDEVTYPE_HAL, GD3D9.Device.Caps.ScreenFormat, D_D3DUSAGE_RENDERTARGET, D_D3DRTYPE_TEXTURE, D_D3DFMT_A8L8 ) == D_D3D_OK )
		{
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ONE_I8 ] = D_D3DFMT_A8L8 ;
			String = "D3DFMT_A8L8" ;
		}
		else
		if( Direct3D9_CheckDeviceFormat( UseAdapterNumber, D_D3DDEVTYPE_HAL, GD3D9.Device.Caps.ScreenFormat, D_D3DUSAGE_RENDERTARGET, D_D3DRTYPE_TEXTURE, D_D3DFMT_A8R8G8B8 ) == D_D3D_OK )
		{
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ONE_I8 ] = D_D3DFMT_A8R8G8B8 ;
			String = "D3DFMT_A8R8G8B8" ;
		}
		else
		{
			DXST_ERRORLOG_ADDUTF16LE( "\x7f\x4f\x28\x75\x67\x30\x4d\x30\x8b\x30\xcf\x63\x3b\x75\x28\x75\x11\xff\xc1\x30\xe3\x30\xf3\x30\xcd\x30\xeb\x30\x74\x65\x70\x65\x38\x00\xd3\x30\xc3\x30\xc8\x30\x8b\x57\xab\x30\xe9\x30\xfc\x30\xd5\x30\xa9\x30\xfc\x30\xde\x30\xc3\x30\xc8\x30\x4c\x30\x42\x30\x8a\x30\x7e\x30\x5b\x30\x93\x30\x67\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�g�p�ł���`��p�P�`�����l������8�r�b�g�^�J���[�t�H�[�}�b�g������܂���ł���\n" @*/ ) ;
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ONE_I8 ] = D_D3DFMT_UNKNOWN ;
		}

		if( GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ONE_I8 ] != D_D3DFMT_UNKNOWN )
		{
			DXST_ERRORLOGFMT_ADDA(( "\x95\x60\x89\xe6\x97\x70\x82\x50\x83\x60\x83\x83\x83\x93\x83\x6c\x83\x8b\x90\xae\x90\x94 8 \x83\x72\x83\x62\x83\x67\x8c\x5e\x83\x4a\x83\x89\x81\x5b\x83\x74\x83\x48\x81\x5b\x83\x7d\x83\x62\x83\x67\x82\xcd %s \x82\xc5\x82\xb7"/*@ "�`��p�P�`�����l������ 8 �r�b�g�^�J���[�t�H�[�}�b�g�� %s �ł�" @*/, String )) ;
		}

		// �`��\ �P�`�����l������16�r�b�g�^�J���[�t�H�[�}�b�g�̑I��
		if( Direct3D9_CheckDeviceFormat( UseAdapterNumber, D_D3DDEVTYPE_HAL, GD3D9.Device.Caps.ScreenFormat, D_D3DUSAGE_RENDERTARGET, D_D3DRTYPE_TEXTURE, D_D3DFMT_L16 ) == D_D3D_OK )
		{
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ONE_I16 ] = D_D3DFMT_L16 ;
			String = "D3DFMT_L16" ;
		}
		else
		if( Direct3D9_CheckDeviceFormat( UseAdapterNumber, D_D3DDEVTYPE_HAL, GD3D9.Device.Caps.ScreenFormat, D_D3DUSAGE_RENDERTARGET, D_D3DRTYPE_TEXTURE, D_D3DFMT_G16R16 ) == D_D3D_OK )
		{
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ONE_I16 ] = D_D3DFMT_G16R16 ;
			String = "D3DFMT_G16R16" ;
		}
		else
		if( Direct3D9_CheckDeviceFormat( UseAdapterNumber, D_D3DDEVTYPE_HAL, GD3D9.Device.Caps.ScreenFormat, D_D3DUSAGE_RENDERTARGET, D_D3DRTYPE_TEXTURE, D_D3DFMT_A16B16G16R16 ) == D_D3D_OK )
		{
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ONE_I16 ] = D_D3DFMT_A16B16G16R16 ;
			String = "D3DFMT_A16B16G16R16" ;
		}
		else
		{
			DXST_ERRORLOG_ADDUTF16LE( "\x7f\x4f\x28\x75\x67\x30\x4d\x30\x8b\x30\xcf\x63\x3b\x75\x28\x75\x11\xff\xc1\x30\xe3\x30\xf3\x30\xcd\x30\xeb\x30\x74\x65\x70\x65\x31\x00\x36\x00\xd3\x30\xc3\x30\xc8\x30\x8b\x57\xab\x30\xe9\x30\xfc\x30\xd5\x30\xa9\x30\xfc\x30\xde\x30\xc3\x30\xc8\x30\x4c\x30\x42\x30\x8a\x30\x7e\x30\x5b\x30\x93\x30\x67\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�g�p�ł���`��p�P�`�����l������16�r�b�g�^�J���[�t�H�[�}�b�g������܂���ł���\n" @*/ ) ;
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ONE_I16 ] = D_D3DFMT_UNKNOWN ;
		}

		if( GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ONE_I16 ] != D_D3DFMT_UNKNOWN )
		{
			DXST_ERRORLOGFMT_ADDA(( "\x95\x60\x89\xe6\x97\x70\x82\x50\x83\x60\x83\x83\x83\x93\x83\x6c\x83\x8b\x90\xae\x90\x94 16 \x83\x72\x83\x62\x83\x67\x8c\x5e\x83\x4a\x83\x89\x81\x5b\x83\x74\x83\x48\x81\x5b\x83\x7d\x83\x62\x83\x67\x82\xcd %s \x82\xc5\x82\xb7"/*@ "�`��p�P�`�����l������ 16 �r�b�g�^�J���[�t�H�[�}�b�g�� %s �ł�" @*/, String )) ;
		}

		// �`��\ �P�`�����l�����������_16�r�b�g�^�J���[�t�H�[�}�b�g�̑I��
		if( Direct3D9_CheckDeviceFormat( UseAdapterNumber, D_D3DDEVTYPE_HAL, GD3D9.Device.Caps.ScreenFormat, D_D3DUSAGE_RENDERTARGET, D_D3DRTYPE_TEXTURE, D_D3DFMT_R16F ) == D_D3D_OK )
		{
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ONE_F16 ] = D_D3DFMT_R16F ;
			String = "D3DFMT_R16F" ;
		}
		else
		if( Direct3D9_CheckDeviceFormat( UseAdapterNumber, D_D3DDEVTYPE_HAL, GD3D9.Device.Caps.ScreenFormat, D_D3DUSAGE_RENDERTARGET, D_D3DRTYPE_TEXTURE, D_D3DFMT_G16R16F ) == D_D3D_OK )
		{
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ONE_F16 ] = D_D3DFMT_G16R16F ;
			String = "D3DFMT_G16R16F" ;
		}
		else
		if( Direct3D9_CheckDeviceFormat( UseAdapterNumber, D_D3DDEVTYPE_HAL, GD3D9.Device.Caps.ScreenFormat, D_D3DUSAGE_RENDERTARGET, D_D3DRTYPE_TEXTURE, D_D3DFMT_A16B16G16R16F ) == D_D3D_OK )
		{
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ONE_F16 ] = D_D3DFMT_A16B16G16R16F ;
			String = "D3DFMT_A16B16G16R16F" ;
		}
		else
		if( Direct3D9_CheckDeviceFormat( UseAdapterNumber, D_D3DDEVTYPE_HAL, GD3D9.Device.Caps.ScreenFormat, D_D3DUSAGE_RENDERTARGET, D_D3DRTYPE_TEXTURE, D_D3DFMT_A32B32G32R32F ) == D_D3D_OK )
		{
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ONE_F16 ] = D_D3DFMT_A32B32G32R32F ;
			String = "D3DFMT_A32B32G32R32F" ;
		}
		else
		{
			DXST_ERRORLOG_ADDUTF16LE( "\x7f\x4f\x28\x75\x67\x30\x4d\x30\x8b\x30\xcf\x63\x3b\x75\x28\x75\x11\xff\xc1\x30\xe3\x30\xf3\x30\xcd\x30\xeb\x30\x6e\x6d\xd5\x52\x0f\x5c\x70\x65\xb9\x70\x31\x00\x36\x00\xd3\x30\xc3\x30\xc8\x30\x8b\x57\xab\x30\xe9\x30\xfc\x30\xd5\x30\xa9\x30\xfc\x30\xde\x30\xc3\x30\xc8\x30\x4c\x30\x42\x30\x8a\x30\x7e\x30\x5b\x30\x93\x30\x67\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�g�p�ł���`��p�P�`�����l�����������_16�r�b�g�^�J���[�t�H�[�}�b�g������܂���ł���\n" @*/ ) ;
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ONE_F16 ] = D_D3DFMT_UNKNOWN ;
		}

		if( GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ONE_F16 ] != D_D3DFMT_UNKNOWN )
		{
			DXST_ERRORLOGFMT_ADDA(( "\x95\x60\x89\xe6\x97\x70\x82\x50\x83\x60\x83\x83\x83\x93\x83\x6c\x83\x8b\x95\x82\x93\xae\x8f\xac\x90\x94\x93\x5f 16 \x83\x72\x83\x62\x83\x67\x8c\x5e\x83\x4a\x83\x89\x81\x5b\x83\x74\x83\x48\x81\x5b\x83\x7d\x83\x62\x83\x67\x82\xcd %s \x82\xc5\x82\xb7"/*@ "�`��p�P�`�����l�����������_ 16 �r�b�g�^�J���[�t�H�[�}�b�g�� %s �ł�" @*/, String )) ;
		}

		// �`��\ �P�`�����l�����������_32�r�b�g�^�J���[�t�H�[�}�b�g�̑I��
		if( Direct3D9_CheckDeviceFormat( UseAdapterNumber, D_D3DDEVTYPE_HAL, GD3D9.Device.Caps.ScreenFormat, D_D3DUSAGE_RENDERTARGET, D_D3DRTYPE_TEXTURE, D_D3DFMT_R32F ) == D_D3D_OK )
		{
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ONE_F32 ] = D_D3DFMT_R32F ;
			String = "D3DFMT_R32F" ;
		}
		else
		if( Direct3D9_CheckDeviceFormat( UseAdapterNumber, D_D3DDEVTYPE_HAL, GD3D9.Device.Caps.ScreenFormat, D_D3DUSAGE_RENDERTARGET, D_D3DRTYPE_TEXTURE, D_D3DFMT_G32R32F ) == D_D3D_OK )
		{
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ONE_F32 ] = D_D3DFMT_G32R32F ;
			String = "D3DFMT_G32R32F" ;
		}
		else
		if( Direct3D9_CheckDeviceFormat( UseAdapterNumber, D_D3DDEVTYPE_HAL, GD3D9.Device.Caps.ScreenFormat, D_D3DUSAGE_RENDERTARGET, D_D3DRTYPE_TEXTURE, D_D3DFMT_A32B32G32R32F ) == D_D3D_OK )
		{
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ONE_F32 ] = D_D3DFMT_A32B32G32R32F ;
			String = "D3DFMT_A32B32G32R32F" ;
		}
		else
		if( Direct3D9_CheckDeviceFormat( UseAdapterNumber, D_D3DDEVTYPE_HAL, GD3D9.Device.Caps.ScreenFormat, D_D3DUSAGE_RENDERTARGET, D_D3DRTYPE_TEXTURE, D_D3DFMT_A16B16G16R16F ) == D_D3D_OK )
		{
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ONE_F32 ] = D_D3DFMT_A16B16G16R16F ;
			String = "D3DFMT_A16B16G16R16F" ;
		}
		else
		{
			DXST_ERRORLOG_ADDUTF16LE( "\x7f\x4f\x28\x75\x67\x30\x4d\x30\x8b\x30\xcf\x63\x3b\x75\x28\x75\x11\xff\xc1\x30\xe3\x30\xf3\x30\xcd\x30\xeb\x30\x6e\x6d\xd5\x52\x0f\x5c\x70\x65\xb9\x70\x33\x00\x32\x00\xd3\x30\xc3\x30\xc8\x30\x8b\x57\xab\x30\xe9\x30\xfc\x30\xd5\x30\xa9\x30\xfc\x30\xde\x30\xc3\x30\xc8\x30\x4c\x30\x42\x30\x8a\x30\x7e\x30\x5b\x30\x93\x30\x67\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�g�p�ł���`��p�P�`�����l�����������_32�r�b�g�^�J���[�t�H�[�}�b�g������܂���ł���\n" @*/ ) ;
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ONE_F32 ] = D_D3DFMT_UNKNOWN ;
		}

		if( GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ONE_F32 ] != D_D3DFMT_UNKNOWN )
		{
			DXST_ERRORLOGFMT_ADDA(( "\x95\x60\x89\xe6\x97\x70\x82\x50\x83\x60\x83\x83\x83\x93\x83\x6c\x83\x8b\x95\x82\x93\xae\x8f\xac\x90\x94\x93\x5f 32 \x83\x72\x83\x62\x83\x67\x8c\x5e\x83\x4a\x83\x89\x81\x5b\x83\x74\x83\x48\x81\x5b\x83\x7d\x83\x62\x83\x67\x82\xcd %s \x82\xc5\x82\xb7"/*@ "�`��p�P�`�����l�����������_ 32 �r�b�g�^�J���[�t�H�[�}�b�g�� %s �ł�" @*/, String )) ;
		}

		// �`��\ �Q�`�����l������8�r�b�g�^�J���[�t�H�[�}�b�g�̑I��
		if( Direct3D9_CheckDeviceFormat( UseAdapterNumber, D_D3DDEVTYPE_HAL, GD3D9.Device.Caps.ScreenFormat, D_D3DUSAGE_RENDERTARGET, D_D3DRTYPE_TEXTURE, D_D3DFMT_A8L8 ) == D_D3D_OK )
		{
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_TWO_I8 ] = D_D3DFMT_A8L8 ;
			String = "D3DFMT_A8L8" ;
		}
		else
		if( Direct3D9_CheckDeviceFormat( UseAdapterNumber, D_D3DDEVTYPE_HAL, GD3D9.Device.Caps.ScreenFormat, D_D3DUSAGE_RENDERTARGET, D_D3DRTYPE_TEXTURE, D_D3DFMT_A8R8G8B8 ) == D_D3D_OK )
		{
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_TWO_I8 ] = D_D3DFMT_A8R8G8B8 ;
			String = "D3DFMT_A8B8G8R8" ;
		}
		else
		{
			DXST_ERRORLOG_ADDUTF16LE( "\x7f\x4f\x28\x75\x67\x30\x4d\x30\x8b\x30\xcf\x63\x3b\x75\x28\x75\x12\xff\xc1\x30\xe3\x30\xf3\x30\xcd\x30\xeb\x30\x74\x65\x70\x65\x38\x00\xd3\x30\xc3\x30\xc8\x30\x8b\x57\xab\x30\xe9\x30\xfc\x30\xd5\x30\xa9\x30\xfc\x30\xde\x30\xc3\x30\xc8\x30\x4c\x30\x42\x30\x8a\x30\x7e\x30\x5b\x30\x93\x30\x67\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�g�p�ł���`��p�Q�`�����l������8�r�b�g�^�J���[�t�H�[�}�b�g������܂���ł���\n" @*/ ) ;
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_TWO_I8 ] = D_D3DFMT_UNKNOWN ;
		}

		if( GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_TWO_I8 ] != D_D3DFMT_UNKNOWN )
		{
			DXST_ERRORLOGFMT_ADDA(( "\x95\x60\x89\xe6\x97\x70\x82\x51\x83\x60\x83\x83\x83\x93\x83\x6c\x83\x8b\x90\xae\x90\x94 8 \x83\x72\x83\x62\x83\x67\x8c\x5e\x83\x4a\x83\x89\x81\x5b\x83\x74\x83\x48\x81\x5b\x83\x7d\x83\x62\x83\x67\x82\xcd %s \x82\xc5\x82\xb7"/*@ "�`��p�Q�`�����l������ 8 �r�b�g�^�J���[�t�H�[�}�b�g�� %s �ł�" @*/, String )) ;
		}

		// �`��\ �Q�`�����l������16�r�b�g�^�J���[�t�H�[�}�b�g�̑I��
		if( Direct3D9_CheckDeviceFormat( UseAdapterNumber, D_D3DDEVTYPE_HAL, GD3D9.Device.Caps.ScreenFormat, D_D3DUSAGE_RENDERTARGET, D_D3DRTYPE_TEXTURE, D_D3DFMT_G16R16 ) == D_D3D_OK )
		{
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_TWO_I16 ] = D_D3DFMT_G16R16 ;
			String = "D3DFMT_G16R16" ;
		}
		else
		if( Direct3D9_CheckDeviceFormat( UseAdapterNumber, D_D3DDEVTYPE_HAL, GD3D9.Device.Caps.ScreenFormat, D_D3DUSAGE_RENDERTARGET, D_D3DRTYPE_TEXTURE, D_D3DFMT_A16B16G16R16 ) == D_D3D_OK )
		{
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_TWO_I16 ] = D_D3DFMT_A16B16G16R16 ;
			String = "D3DFMT_A16B16G16R16" ;
		}
		else
		{
			DXST_ERRORLOG_ADDUTF16LE( "\x7f\x4f\x28\x75\x67\x30\x4d\x30\x8b\x30\xcf\x63\x3b\x75\x28\x75\x12\xff\xc1\x30\xe3\x30\xf3\x30\xcd\x30\xeb\x30\x74\x65\x70\x65\x31\x00\x36\x00\xd3\x30\xc3\x30\xc8\x30\x8b\x57\xab\x30\xe9\x30\xfc\x30\xd5\x30\xa9\x30\xfc\x30\xde\x30\xc3\x30\xc8\x30\x4c\x30\x42\x30\x8a\x30\x7e\x30\x5b\x30\x93\x30\x67\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�g�p�ł���`��p�Q�`�����l������16�r�b�g�^�J���[�t�H�[�}�b�g������܂���ł���\n" @*/ ) ;
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_TWO_I16 ] = D_D3DFMT_UNKNOWN ;
		}

		if( GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_TWO_I16 ] != D_D3DFMT_UNKNOWN )
		{
			DXST_ERRORLOGFMT_ADDA(( "\x95\x60\x89\xe6\x97\x70\x82\x51\x83\x60\x83\x83\x83\x93\x83\x6c\x83\x8b\x90\xae\x90\x94 16 \x83\x72\x83\x62\x83\x67\x8c\x5e\x83\x4a\x83\x89\x81\x5b\x83\x74\x83\x48\x81\x5b\x83\x7d\x83\x62\x83\x67\x82\xcd %s \x82\xc5\x82\xb7"/*@ "�`��p�Q�`�����l������ 16 �r�b�g�^�J���[�t�H�[�}�b�g�� %s �ł�" @*/, String )) ;
		}

		// �`��\ �Q�`�����l�����������_16�r�b�g�^�J���[�t�H�[�}�b�g�̑I��
		if( Direct3D9_CheckDeviceFormat( UseAdapterNumber, D_D3DDEVTYPE_HAL, GD3D9.Device.Caps.ScreenFormat, D_D3DUSAGE_RENDERTARGET, D_D3DRTYPE_TEXTURE, D_D3DFMT_G16R16F ) == D_D3D_OK )
		{
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_TWO_F16 ] = D_D3DFMT_G16R16F ;
			String = "D3DFMT_G16R16F" ;
		}
		else
		if( Direct3D9_CheckDeviceFormat( UseAdapterNumber, D_D3DDEVTYPE_HAL, GD3D9.Device.Caps.ScreenFormat, D_D3DUSAGE_RENDERTARGET, D_D3DRTYPE_TEXTURE, D_D3DFMT_A16B16G16R16F ) == D_D3D_OK )
		{
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_TWO_F16 ] = D_D3DFMT_A16B16G16R16F ;
			String = "D3DFMT_A16B16G16R16F" ;
		}
		else
		if( Direct3D9_CheckDeviceFormat( UseAdapterNumber, D_D3DDEVTYPE_HAL, GD3D9.Device.Caps.ScreenFormat, D_D3DUSAGE_RENDERTARGET, D_D3DRTYPE_TEXTURE, D_D3DFMT_A32B32G32R32F ) == D_D3D_OK )
		{
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_TWO_F16 ] = D_D3DFMT_A32B32G32R32F ;
			String = "D3DFMT_A32B32G32R32F" ;
		}
		else
		{
			DXST_ERRORLOG_ADDUTF16LE( "\x7f\x4f\x28\x75\x67\x30\x4d\x30\x8b\x30\xcf\x63\x3b\x75\x28\x75\x12\xff\xc1\x30\xe3\x30\xf3\x30\xcd\x30\xeb\x30\x6e\x6d\xd5\x52\x0f\x5c\x70\x65\xb9\x70\x31\x00\x36\x00\xd3\x30\xc3\x30\xc8\x30\x8b\x57\xab\x30\xe9\x30\xfc\x30\xd5\x30\xa9\x30\xfc\x30\xde\x30\xc3\x30\xc8\x30\x4c\x30\x42\x30\x8a\x30\x7e\x30\x5b\x30\x93\x30\x67\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�g�p�ł���`��p�Q�`�����l�����������_16�r�b�g�^�J���[�t�H�[�}�b�g������܂���ł���\n" @*/ ) ;
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_TWO_F16 ] = D_D3DFMT_UNKNOWN ;
		}

		if( GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_TWO_F16 ] != D_D3DFMT_UNKNOWN )
		{
			DXST_ERRORLOGFMT_ADDA(( "\x95\x60\x89\xe6\x97\x70\x82\x51\x83\x60\x83\x83\x83\x93\x83\x6c\x83\x8b\x95\x82\x93\xae\x8f\xac\x90\x94\x93\x5f 16 \x83\x72\x83\x62\x83\x67\x8c\x5e\x83\x4a\x83\x89\x81\x5b\x83\x74\x83\x48\x81\x5b\x83\x7d\x83\x62\x83\x67\x82\xcd %s \x82\xc5\x82\xb7"/*@ "�`��p�Q�`�����l�����������_ 16 �r�b�g�^�J���[�t�H�[�}�b�g�� %s �ł�" @*/, String )) ;
		}

		// �`��\ �Q�`�����l�����������_32�r�b�g�^�J���[�t�H�[�}�b�g�̑I��
		if( Direct3D9_CheckDeviceFormat( UseAdapterNumber, D_D3DDEVTYPE_HAL, GD3D9.Device.Caps.ScreenFormat, D_D3DUSAGE_RENDERTARGET, D_D3DRTYPE_TEXTURE, D_D3DFMT_G32R32F ) == D_D3D_OK )
		{
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_TWO_F32 ] = D_D3DFMT_G32R32F ;
			String = "D3DFMT_G32R32F" ;
		}
		else
		if( Direct3D9_CheckDeviceFormat( UseAdapterNumber, D_D3DDEVTYPE_HAL, GD3D9.Device.Caps.ScreenFormat, D_D3DUSAGE_RENDERTARGET, D_D3DRTYPE_TEXTURE, D_D3DFMT_A32B32G32R32F ) == D_D3D_OK )
		{
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_TWO_F32 ] = D_D3DFMT_A32B32G32R32F ;
			String = "D3DFMT_A32B32G32R32F" ;
		}
		else
		if( Direct3D9_CheckDeviceFormat( UseAdapterNumber, D_D3DDEVTYPE_HAL, GD3D9.Device.Caps.ScreenFormat, D_D3DUSAGE_RENDERTARGET, D_D3DRTYPE_TEXTURE, D_D3DFMT_A16B16G16R16F ) == D_D3D_OK )
		{
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_TWO_F32 ] = D_D3DFMT_A16B16G16R16F ;
			String = "D3DFMT_A16B16G16R16F" ;
		}
		else
		{
			DXST_ERRORLOG_ADDUTF16LE( "\x7f\x4f\x28\x75\x67\x30\x4d\x30\x8b\x30\xcf\x63\x3b\x75\x28\x75\x12\xff\xc1\x30\xe3\x30\xf3\x30\xcd\x30\xeb\x30\x6e\x6d\xd5\x52\x0f\x5c\x70\x65\xb9\x70\x33\x00\x32\x00\xd3\x30\xc3\x30\xc8\x30\x8b\x57\xab\x30\xe9\x30\xfc\x30\xd5\x30\xa9\x30\xfc\x30\xde\x30\xc3\x30\xc8\x30\x4c\x30\x42\x30\x8a\x30\x7e\x30\x5b\x30\x93\x30\x67\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�g�p�ł���`��p�Q�`�����l�����������_32�r�b�g�^�J���[�t�H�[�}�b�g������܂���ł���\n" @*/ ) ;
			GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_TWO_F32 ] = D_D3DFMT_UNKNOWN ;
		}

		if( GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_TWO_F32 ] != D_D3DFMT_UNKNOWN )
		{
			DXST_ERRORLOGFMT_ADDA(( "\x95\x60\x89\xe6\x97\x70\x82\x51\x83\x60\x83\x83\x83\x93\x83\x6c\x83\x8b\x95\x82\x93\xae\x8f\xac\x90\x94\x93\x5f 32 \x83\x72\x83\x62\x83\x67\x8c\x5e\x83\x4a\x83\x89\x81\x5b\x83\x74\x83\x48\x81\x5b\x83\x7d\x83\x62\x83\x67\x82\xcd %s \x82\xc5\x82\xb7"/*@ "�`��p�Q�`�����l�����������_ 32 �r�b�g�^�J���[�t�H�[�}�b�g�� %s �ł�" @*/, String )) ;
		}

		// �}�X�N�J���[�o�b�t�@�p�t�H�[�}�b�g�̑I��
		GD3D9.Device.Caps.MaskColorFormat = GSYS.Screen.MainScreenColorBitDepth == 32 ? GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_RGB32 ] : GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_RGB16 ] ;

		// �}�X�N�A���t�@�t�H�[�}�b�g�̑I��
		GD3D9.Device.Caps.MaskAlphaFormat = GSYS.Screen.MainScreenColorBitDepth == 32 ? GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ALPHATEST_RGB32 ] : GD3D9.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ALPHATEST_RGB16 ] ;
	}

	// ���o��
	{
		DXST_ERRORLOGFMT_ADDA(( "\x88\xea\x93\x78\x82\xc9\x95\x60\x89\xe6\x82\xc5\x82\xab\x82\xe9\x83\x76\x83\x8a\x83\x7e\x83\x65\x83\x42\x83\x75\x82\xcc\x8d\xc5\x91\xe5\x90\x94:%d"/*@ "��x�ɕ`��ł���v���~�e�B�u�̍ő吔:%d" @*/, GSYS.HardInfo.MaxPrimitiveCount )) ;
		DXST_ERRORLOGFMT_ADDA(( "\x91\xce\x89\x9e\x82\xb5\x82\xc4\x82\xa2\x82\xe9\x8d\xc5\x91\xe5\x92\xb8\x93\x5f\x83\x43\x83\x93\x83\x66\x83\x62\x83\x4e\x83\x58:%d"/*@ "�Ή����Ă���ő咸�_�C���f�b�N�X:%d" @*/,     GSYS.HardInfo.MaxVertexIndex )) ;
		DXST_ERRORLOGFMT_ADDA(( "\x93\xaf\x8e\x9e\x82\xc9\x83\x8c\x83\x93\x83\x5f\x83\x8a\x83\x93\x83\x4f\x82\xc5\x82\xab\x82\xe9\x83\x6f\x83\x62\x83\x74\x83\x40\x82\xcc\x90\x94:%d"/*@ "�����Ƀ����_�����O�ł���o�b�t�@�̐�:%d" @*/, GSYS.HardInfo.RenderTargetNum )) ;
		DXST_ERRORLOGFMT_ADDA(( "\x8d\xc5\x91\xe5\x83\x65\x83\x4e\x83\x58\x83\x60\x83\x83\x83\x54\x83\x43\x83\x59\x81\x40\x95\x9d:%d \x8d\x82\x82\xb3:%d"/*@ "�ő�e�N�X�`���T�C�Y�@��:%d ����:%d" @*/, GD3D9.Device.Caps.DeviceCaps.MaxTextureWidth, GD3D9.Device.Caps.DeviceCaps.MaxTextureHeight )) ;
		DXST_ERRORLOGFMT_ADDA(( "\x83\x65\x83\x4e\x83\x58\x83\x60\x83\x83\x83\x58\x83\x65\x81\x5b\x83\x57\x83\x65\x83\x93\x83\x7c\x83\x89\x83\x8a\x83\x8c\x83\x57\x83\x58\x83\x5e\x81\x46%s"/*@ "�e�N�X�`���X�e�[�W�e���|�������W�X�^�F%s" @*/, GD3D9.Device.Caps.ValidTexTempRegFlag ? "\x8e\x67\x97\x70\x89\xc2"/*@ "�g�p��" @*/ : "\x8e\x67\x97\x70\x95\x73\x89\xc2"/*@ "�g�p�s��" @*/ )) ;
		DXST_ERRORLOGFMT_ADDA(( "\x8c\xb8\x8e\x5a\x8d\x87\x90\xac\x82\xcc\x83\x6e\x81\x5b\x83\x68\x83\x45\x83\x46\x83\x41\x91\xce\x89\x9e\x81\x46%s"/*@ "���Z�����̃n�[�h�E�F�A�Ή��F%s" @*/, GD3D9.Device.Caps.ValidDestBlendOp ? "\x83\x6c\x83\x43\x83\x65\x83\x42\x83\x75"/*@ "�l�C�e�B�u" @*/ : "\x83\x47\x83\x7e\x83\x85\x83\x8c\x81\x5b\x83\x67"/*@ "�G�~�����[�g" @*/ )) ;
		if( GD3D9.Device.Caps.VertexHardwareProcess )
		{
			DXST_ERRORLOGFMT_ADDA(( "\x83\x6e\x81\x5b\x83\x68\x83\x45\x83\x46\x83\x41\x92\xb8\x93\x5f\x83\x56\x83\x46\x81\x5b\x83\x5f\x81\x5b\x83\x6f\x81\x5b\x83\x57\x83\x87\x83\x93\x83\x52\x81\x5b\x83\x68\x81\x46%x"/*@ "�n�[�h�E�F�A���_�V�F�[�_�[�o�[�W�����R�[�h�F%x" @*/, GD3D9.Device.Caps.DeviceCaps.VertexShaderVersion & 0xffff )) ;
			DXST_ERRORLOGFMT_ADDA(( "\x83\x47\x83\x7e\x83\x85\x83\x8c\x81\x5b\x83\x56\x83\x87\x83\x93\x92\xb8\x93\x5f\x83\x56\x83\x46\x81\x5b\x83\x5f\x81\x5b\x83\x6f\x81\x5b\x83\x57\x83\x87\x83\x93\x83\x52\x81\x5b\x83\x68\x81\x46\x83\x47\x83\x7e\x83\x85\x83\x8c\x81\x5b\x83\x56\x83\x87\x83\x93\x96\xb3\x82\xb5"/*@ "�G�~�����[�V�������_�V�F�[�_�[�o�[�W�����R�[�h�F�G�~�����[�V��������" @*/ )) ;
		}
		else
		{
			DXST_ERRORLOGFMT_ADDA(( "\x83\x6e\x81\x5b\x83\x68\x83\x45\x83\x46\x83\x41\x92\xb8\x93\x5f\x83\x56\x83\x46\x81\x5b\x83\x5f\x81\x5b\x83\x6f\x81\x5b\x83\x57\x83\x87\x83\x93\x83\x52\x81\x5b\x83\x68\x81\x46%x"/*@ "�n�[�h�E�F�A���_�V�F�[�_�[�o�[�W�����R�[�h�F%x" @*/, GD3D9.Device.Shader.NativeVertexShaderVersion & 0xffff )) ;
			DXST_ERRORLOGFMT_ADDA(( "\x83\x47\x83\x7e\x83\x85\x83\x8c\x81\x5b\x83\x56\x83\x87\x83\x93\x92\xb8\x93\x5f\x83\x56\x83\x46\x81\x5b\x83\x5f\x81\x5b\x83\x6f\x81\x5b\x83\x57\x83\x87\x83\x93\x83\x52\x81\x5b\x83\x68\x81\x46%x"/*@ "�G�~�����[�V�������_�V�F�[�_�[�o�[�W�����R�[�h�F%x" @*/, GD3D9.Device.Caps.DeviceCaps.VertexShaderVersion & 0xffff )) ;
		}
		DXST_ERRORLOGFMT_ADDA(( "\x83\x73\x83\x4e\x83\x5a\x83\x8b\x83\x56\x83\x46\x81\x5b\x83\x5f\x81\x5b\x83\x6f\x81\x5b\x83\x57\x83\x87\x83\x93\x83\x52\x81\x5b\x83\x68\x81\x46%x"/*@ "�s�N�Z���V�F�[�_�[�o�[�W�����R�[�h�F%x" @*/, GD3D9.Device.Caps.DeviceCaps.PixelShaderVersion & 0xffff )) ;
	}

	// �`���̏���`�������b�N���ēǂݎ��ꍇ�ƈꎞ�I�ȕ`��T�[�t�F�X����ēǂݎ��ꍇ�̂ǂ��炪�������𒲂ׂ�
	{
		D_IDirect3DSurface9 *RTSurface = NULL ;
		D_IDirect3DSurface9 *SMSurface = NULL ;
		D_IDirect3DSurface9 *DestSurface = NULL ;
		D_D3DLOCKED_RECT SrcLockedRect ;
		D_D3DLOCKED_RECT DestLockedRect ;
		RECT LockRect ;
		LONGLONG Time1 = 0, Time2 = 0;
		UINT Size, i, j ;
		COLORDATA *ColorData ;

		// ������Ԃł̓��b�N���g�p���Ȃ��A�ɂ��Ă���
		GD3D9.Device.Caps.UseRenderTargetLock = FALSE ;

		// �t���V�[���A���`�G�C���A�X���g�p����ꍇ�̓o�b�N�o�b�t�@�̃��b�N�͂ł��Ȃ�
		if( ( D_D3DMULTISAMPLE_TYPE )GSYS.Setting.FSAAMultiSampleCount != D_D3DMULTISAMPLE_NONE )
		{
			DXST_ERRORLOG_ADDUTF16LE( "\xd5\x30\xeb\x30\xb7\x30\xfc\x30\xf3\x30\xa2\x30\xf3\x30\xc1\x30\xa8\x30\xa4\x30\xea\x30\xa2\x30\xb9\x30\x92\x30\x7f\x4f\x28\x75\x59\x30\x8b\x30\x34\x58\x08\x54\x6f\x30\xd0\x30\xc3\x30\xaf\x30\xd0\x30\xc3\x30\xd5\x30\xa1\x30\x6f\x30\xed\x30\xc3\x30\xaf\x30\x67\x30\x4d\x30\x7e\x30\x5b\x30\x93\x30\x0a\x00\x00"/*@ L"�t���V�[���A���`�G�C���A�X���g�p����ꍇ�̓o�b�N�o�b�t�@�̓��b�N�ł��܂���\n" @*/ ) ;
		}
		else
		{
			// �L���b�V���Ȃǂ̊֌W�Ő�s��������s���ɂȂ�\�������邽�߁A�R�񓯂����Ƃ��J��Ԃ�
			for( j = 0 ; j < 3 ; j ++ )
			{
				// �`�F�b�N�J�n
				Time1 = -1 ;
				Time2 = -1 ;
				for(;;)
				{
					// �e�X�g�Ɏg�p����T�[�t�F�X�̃T�C�Y������
					Size = 256 ;
					while( Size > ( UINT )GSYS.Screen.MainScreenSizeX || Size > ( UINT )GSYS.Screen.MainScreenSizeY ) Size >>= 1 ;

					// ���̏o�͐�̃T�[�t�F�X���쐬����
					DestSurface = NULL ;
					if( Direct3DDevice9_CreateOffscreenPlainSurface(
							Size, Size, GD3D9.Device.Caps.ScreenFormat, D_D3DPOOL_SYSTEMMEM, &DestSurface, NULL ) != D_D3D_OK )
						break ;

					// ���b�N�����`�̍쐬
					LockRect.left   = 0 ;
					LockRect.top    = 0 ;
					LockRect.right  = ( LONG )Size ;
					LockRect.bottom = ( LONG )Size ;

					// �J���[�f�[�^�̎擾
					ColorData = Graphics_D3D9_GetD3DFormatColorData( GD3D9.Device.Caps.ScreenFormat ) ;

					// ���̏o�͐�̃T�[�t�F�X�����b�N
					if( Direct3DSurface9_LockRect( DestSurface, &DestLockedRect, &LockRect, D_D3DLOCK_READONLY ) != D_D3D_OK )
						break ;

					// �ŏ��ɒ��ڃ��b�N����ꍇ�����݂�
					{
						Time1 = NS_GetNowHiPerformanceCount() ;

						// ���ڃ��b�N
						if( Direct3DSurface9_LockRect( GD3D9.Device.Screen.BackBufferSurface, &SrcLockedRect, &LockRect, D_D3DLOCK_READONLY ) != D_D3D_OK )
							break ;

						// �f�[�^��]��
						for( i = 0 ; i < Size ; i ++ )
							_MEMCPY(
								( BYTE * )DestLockedRect.pBits + DestLockedRect.Pitch * i,
								( BYTE * )SrcLockedRect.pBits + SrcLockedRect.Pitch * i,
								Size * ColorData->PixelByte ) ;

						// ���b�N����
						Direct3DSurface9_UnlockRect( GD3D9.Device.Screen.BackBufferSurface ) ;

						Time1 = NS_GetNowHiPerformanceCount() - Time1 ;

					}

					// �ꎞ�I�ȕ`���T�[�t�F�X���쐬���ēǂݎ��ꍇ�����݂�
					{
						Time2 = GetNowHiPerformanceCount() ;

						// ���b�N�T�C�Y�Ɠ����傫���̕`��\�T�[�t�F�X�̍쐬
						if( Direct3DDevice9_CreateRenderTarget(
								Size, Size, GD3D9.Device.Caps.ScreenFormat, D_D3DMULTISAMPLE_NONE, 0, FALSE, &RTSurface, NULL ) != D_D3D_OK )
							break ;

						// ���b�N�T�C�Y�Ɠ����傫���̃V�X�e����������̃I�t�X�N���[���T�[�t�F�X�̍쐬
						if( Direct3DDevice9_CreateOffscreenPlainSurface(
								Size, Size, GD3D9.Device.Caps.ScreenFormat, D_D3DPOOL_SYSTEMMEM, &SMSurface, NULL ) != D_D3D_OK )
							break ;

						// �܂��`��\�T�[�t�F�X�ɓ��e��]������
						Direct3DDevice9_StretchRect(
							GD3D9.Device.Screen.BackBufferSurface, &LockRect,
							RTSurface,             &LockRect, D_D3DTEXF_NONE ) ;

						// ���̌�V�X�e���������T�[�t�F�X�ɓ��e��]������
						Direct3DDevice9_GetRenderTargetData(
							RTSurface, SMSurface ) ;

						// �V�X�e���������T�[�t�F�X�����b�N
						if( Direct3DSurface9_LockRect( SMSurface, &SrcLockedRect, NULL, D_D3DLOCK_READONLY ) != D_D3D_OK )
							break ;

						// �f�[�^��]��
						for( i = 0 ; i < Size ; i ++ )
							_MEMCPY(
								( BYTE * )DestLockedRect.pBits + DestLockedRect.Pitch * i,
								( BYTE * )SrcLockedRect.pBits + SrcLockedRect.Pitch * i,
								Size * ColorData->PixelByte ) ;

						// ���b�N����
						Direct3DSurface9_UnlockRect( SMSurface ) ;

						// �ꎞ�I�ȃT�[�t�F�X�����
						Direct3D9_ObjectRelease( RTSurface ) ;
						Direct3D9_ObjectRelease( SMSurface ) ;
						RTSurface = NULL ;
						SMSurface = NULL ;

						Time2 = GetNowHiPerformanceCount() - Time2 ;
					}

					break ;
				}
				if( SMSurface != NULL )
				{
					Direct3D9_ObjectRelease( SMSurface ) ;
					SMSurface = NULL ;
				}
				if( RTSurface != NULL )
				{
					Direct3D9_ObjectRelease( RTSurface ) ;
					RTSurface = NULL ;
				}
				if( DestSurface != NULL )
				{
					Direct3D9_ObjectRelease( DestSurface ) ;
					DestSurface = NULL ;
				}
			}

			if( Time2 == -1 || Time1 == -1 )
			{
				DXST_ERRORLOG_ADDUTF16LE( "\xd0\x30\xc3\x30\xaf\x30\xd0\x30\xc3\x30\xd5\x30\xa1\x30\x6f\x30\xed\x30\xc3\x30\xaf\x30\x67\x30\x4d\x30\x7e\x30\x5b\x30\x93\x30\x0a\x00\x00"/*@ L"�o�b�N�o�b�t�@�̓��b�N�ł��܂���\n" @*/ ) ;
			}
			else
			{
				// �����ȕ����g�p����
				DXST_ERRORLOGFMT_ADDUTF16LE(( "\xd0\x30\xc3\x30\xaf\x30\xd0\x30\xc3\x30\xd5\x30\xa1\x30\xed\x30\xc3\x30\xaf\x30\xe2\x8e\x01\x90\x6e\x30\x42\x66\x93\x95\x3a\x00\x25\x00\x64\x00\x6e\x00\x73\x00\x65\x00\x63\x00\x20\x00\x20\x00\x00\x4e\x42\x66\x84\x76\x6a\x30\xcf\x63\x3b\x75\xef\x53\xfd\x80\xd0\x30\xc3\x30\xd5\x30\xa1\x30\x92\x30\x7f\x4f\x28\x75\x57\x30\x5f\x30\xe2\x8e\x01\x90\x6e\x30\x42\x66\x93\x95\x3a\x00\x25\x00\x64\x00\x6e\x00\x73\x00\x65\x00\x63\x00\x00"/*@ L"�o�b�N�o�b�t�@���b�N�]���̎���:%dnsec  �ꎞ�I�ȕ`��\�o�b�t�@���g�p�����]���̎���:%dnsec" @*/, ( int )Time1, ( int )Time2 ) ) ;
				if( Time1 < Time2 )
				{
					GD3D9.Device.Caps.UseRenderTargetLock = TRUE ;
					DXST_ERRORLOG_ADDUTF16LE( "\xd0\x30\xc3\x30\xaf\x30\xd0\x30\xc3\x30\xd5\x30\xa1\x30\xed\x30\xc3\x30\xaf\x30\x92\x30\x7f\x4f\x28\x75\x57\x30\x7e\x30\x59\x30\x0a\x00\x00"/*@ L"�o�b�N�o�b�t�@���b�N���g�p���܂�\n" @*/ ) ;
				}
				else
				{
					DXST_ERRORLOG_ADDUTF16LE( "\xd0\x30\xc3\x30\xaf\x30\xd0\x30\xc3\x30\xd5\x30\xa1\x30\xed\x30\xc3\x30\xaf\x30\x92\x30\x7f\x4f\x28\x75\x57\x30\x7e\x30\x5b\x30\x93\x30\x0a\x00\x00"/*@ L"�o�b�N�o�b�t�@���b�N���g�p���܂���\n" @*/ ) ;
				}
			}
		}
	}

	// �I��
	return 0 ;

	// �G���[����
ERR:
	return ErrorRet ;
}

// Direct3DDevice9 �֌W�̏�����
extern	int		Graphics_D3D9_Device_Initialize( void )
{
	DWORD i ;

#ifndef DX_NON_FILTER
	GraphFilter_Initialize() ;
#endif // DX_NON_FILTER

	// ���_�o�b�t�@���̌Œ�l���Z�b�g����
	{
		VERTEX_NOTEX_2D    *vertNoTex ;
		VERTEX_2D          *vertTex ;
		VERTEX_BLENDTEX_2D *vertBlend ;

		vertNoTex = (VERTEX_NOTEX_2D    *)GD3D9.Device.DrawInfo.VertexBuffer_NoTex ;
		vertTex   = (VERTEX_2D          *)GD3D9.Device.DrawInfo.VertexBuffer_Tex ;
		vertBlend = (VERTEX_BLENDTEX_2D *)GD3D9.Device.DrawInfo.VertexBuffer_BlendTex ;
		
		for( i = 0 ; i < D3DDEV_NOTEX_VERTMAXNUM ; i ++, vertNoTex ++ )
		{
			vertNoTex->pos.z = 0.0F ;
			vertNoTex->rhw   = 1.0F ;
		}

		for( i = 0 ; i < D3DDEV_TEX_VERTMAXNUM ; i ++, vertTex ++ )
		{
			vertTex->pos.z = 0.0F ;
			vertTex->rhw   = 1.0F ;
		}

		for( i = 0 ; i < D3DDEV_BLENDTEX_VERTMAXNUM ; i ++, vertBlend ++ )
		{
			vertBlend->pos.z    = 0.0F ;
			vertBlend->rhw      = 1.0F ;
			vertBlend->specular = 0xffffffff ;
		}
		
		GD3D9.Device.DrawInfo.VertexBufferAddr[ 0 ][ 0 ] = ( BYTE * )GD3D9.Device.DrawInfo.VertexBuffer_NoTex ;
		GD3D9.Device.DrawInfo.VertexBufferAddr[ 0 ][ 1 ] = ( BYTE * )GD3D9.Device.DrawInfo.VertexBuffer_Tex ;
		GD3D9.Device.DrawInfo.VertexBufferAddr[ 0 ][ 2 ] = ( BYTE * )GD3D9.Device.DrawInfo.VertexBuffer_BlendTex ;
		GD3D9.Device.DrawInfo.VertexBufferAddr[ 1 ][ 0 ] = ( BYTE * )GD3D9.Device.DrawInfo.VertexBuffer_3D ;
		GD3D9.Device.DrawInfo.VertexBufferAddr[ 1 ][ 1 ] = ( BYTE * )GD3D9.Device.DrawInfo.VertexBuffer_3D ;
	}

	// �`��X�e�[�^�X�̃Z�b�g
	{
		float FloatParam ;

		// �e�N�X�`���`��p�ݒ�
		for( i = 0 ; i < USE_TEXTURESTAGE_NUM ; i ++ )
		{
			if( GSYS.DrawSetting.TexAddressModeU[ i ] == 0 )
				GSYS.DrawSetting.TexAddressModeU[ i ] = D_D3DTADDRESS_CLAMP ;
			if( GSYS.DrawSetting.TexAddressModeV[ i ] == 0 )
				GSYS.DrawSetting.TexAddressModeV[ i ] = D_D3DTADDRESS_CLAMP ;
			if( GSYS.DrawSetting.TexAddressModeW[ i ] == 0 )
				GSYS.DrawSetting.TexAddressModeW[ i ] = D_D3DTADDRESS_CLAMP ;
		}
		for( i = 0 ; i < USE_TEXTURESTAGE_NUM ; i ++ )
		{
			// �e�N�X�`���A�h���X���[�h�ݒ�
			Direct3DDevice9_SetSamplerState( i, D_D3DSAMP_ADDRESSU, ( DWORD )GSYS.DrawSetting.TexAddressModeU[ i ] ) ;
			Direct3DDevice9_SetSamplerState( i, D_D3DSAMP_ADDRESSV, ( DWORD )GSYS.DrawSetting.TexAddressModeV[ i ] ) ;
			Direct3DDevice9_SetSamplerState( i, D_D3DSAMP_ADDRESSW, ( DWORD )GSYS.DrawSetting.TexAddressModeW[ i ] ) ;

			// �t�B���^�����O���[�h��ݒ�
			switch( GD3D9.Device.State.DrawMode )
			{
			case DX_DRAWMODE_NEAREST :
				GD3D9.Device.State.MagFilter[ i ] = D_D3DTEXF_POINT ;
				GD3D9.Device.State.MinFilter[ i ] = D_D3DTEXF_POINT ;
				GD3D9.Device.State.MipFilter[ i ] = D_D3DTEXF_POINT ;
				break ;

			case DX_DRAWMODE_BILINEAR :
				GD3D9.Device.State.MagFilter[ i ] = D_D3DTEXF_LINEAR ;
				GD3D9.Device.State.MinFilter[ i ] = D_D3DTEXF_LINEAR ;
				GD3D9.Device.State.MipFilter[ i ] = D_D3DTEXF_LINEAR ;
				break ;

			case DX_DRAWMODE_ANISOTROPIC :
				GD3D9.Device.State.MagFilter[ i ] = D_D3DTEXF_ANISOTROPIC ;
				GD3D9.Device.State.MinFilter[ i ] = D_D3DTEXF_ANISOTROPIC ;
				GD3D9.Device.State.MipFilter[ i ] = D_D3DTEXF_LINEAR ;
				break ;
			}
			Direct3DDevice9_SetSamplerState( i, D_D3DSAMP_MAGFILTER, GD3D9.Device.State.MagFilter[ i ] ) ;
			Direct3DDevice9_SetSamplerState( i, D_D3DSAMP_MINFILTER, GD3D9.Device.State.MinFilter[ i ] ) ;
			Direct3DDevice9_SetSamplerState( i, D_D3DSAMP_MIPFILTER, GD3D9.Device.State.MipFilter[ i ] ) ;

			// �����ő�ٕ������Z�b�g
			GD3D9.Device.State.MaxAnisotropyDim[ i ] = 1 ;
		}

		// �Ō�̃h�b�g�`�悵�Ȃ�
//		Direct3DDevice9_SetRenderState( D_D3DRS_LASTPIXEL, TRUE ) ;

		// �J�����O�I�t
		Direct3DDevice9_SetRenderState( D_D3DRS_CULLMODE, D_D3DCULL_NONE ) ;

		// ���C�e�B���O�n�m
		Direct3DDevice9_SetRenderState( D_D3DRS_LIGHTING, TRUE ) ;

		// �t���b�g�V�F�[�f�B���O
		Direct3DDevice9_SetRenderState( D_D3DRS_SHADEMODE, D_D3DSHADE_FLAT ) ;

		// �`��u�����h�Ȃ�
//		Direct3DDevice9_SetRenderState( D_D3DRS_ALPHABLENDENABLE, FALSE ) ;

		// �f�B�U�����O�n�e�e
		Direct3DDevice9_SetRenderState( D_D3DRS_DITHERENABLE, FALSE ) ;

		// �X�y�L�������C�g�n�e�e
		Direct3DDevice9_SetRenderState( D_D3DRS_SPECULARENABLE, TRUE ) ;

		// �A���r�G���g���C�g�\�[�X�̓}�e���A��
		Direct3DDevice9_SetRenderState( D_D3DRS_AMBIENTMATERIALSOURCE, D_D3DMCS_MATERIAL  ) ;

		// �@���̐��K�����s��
		Direct3DDevice9_SetRenderState( D_D3DRS_NORMALIZENORMALS, TRUE ) ;

		// �t�H�O�͖���
		Direct3DDevice9_SetRenderState( D_D3DRS_FOGENABLE, FALSE ) ;
		Direct3DDevice9_SetRenderState( D_D3DRS_FOGVERTEXMODE, D_D3DFOG_NONE ) ;
		Direct3DDevice9_SetRenderState( D_D3DRS_FOGCOLOR, 0 ) ;
		FloatParam = 0.0f ;
		Direct3DDevice9_SetRenderState( D_D3DRS_FOGSTART, *( ( DWORD * )&FloatParam ) ) ;
		FloatParam = 1.0f ;
		Direct3DDevice9_SetRenderState( D_D3DRS_FOGEND, *( ( DWORD * )&FloatParam ) ) ;

		// �}�e���A���̃p�����[�^���擾
		{
			D_D3DMATERIAL9 Material ;

			Direct3DDevice9_GetMaterial( &Material ) ;
			GD3D9.Device.State.Material.Diffuse.r = Material.Diffuse.r ;
			GD3D9.Device.State.Material.Diffuse.g = Material.Diffuse.g ;
			GD3D9.Device.State.Material.Diffuse.b = Material.Diffuse.b ;
			GD3D9.Device.State.Material.Diffuse.a = Material.Diffuse.a ;
			GD3D9.Device.State.Material.Specular.r = Material.Specular.r ;
			GD3D9.Device.State.Material.Specular.g = Material.Specular.g ;
			GD3D9.Device.State.Material.Specular.b = Material.Specular.b ;
			GD3D9.Device.State.Material.Specular.a = Material.Specular.a ;
			GD3D9.Device.State.Material.Emissive.r = Material.Emissive.r ;
			GD3D9.Device.State.Material.Emissive.g = Material.Emissive.g ;
			GD3D9.Device.State.Material.Emissive.b = Material.Emissive.b ;
			GD3D9.Device.State.Material.Emissive.a = Material.Emissive.a ;
			GD3D9.Device.State.Material.Ambient.r = Material.Ambient.r ;
			GD3D9.Device.State.Material.Ambient.g = Material.Ambient.g ;
			GD3D9.Device.State.Material.Ambient.b = Material.Ambient.b ;
			GD3D9.Device.State.Material.Ambient.a = Material.Ambient.a ;
			GD3D9.Device.State.Material.Power = Material.Power ;
		}
	}

	// �u�����f�B���O�p�����[�^������������
	_MEMSET( &GD3D9.Device.State.BlendInfo, 0, sizeof( GRAPHICS_HARDDATA_DIRECT3D9_BLENDINFO ) ) ;
	GD3D9.Device.State.BlendInfo.UseTextureStageNum = 0 ;
	for( i = 0 ; i < USE_TEXTURESTAGE_NUM ; i ++ )
	{
		GD3D9.Device.State.BlendInfo.TextureStageInfo[i].Texture       = NULL ;
		GD3D9.Device.State.BlendInfo.TextureStageInfo[i].ResultTempARG = FALSE ;
		GD3D9.Device.State.BlendInfo.TextureStageInfo[i].AlphaARG1     = D_D3DTA_TEXTURE ;
		GD3D9.Device.State.BlendInfo.TextureStageInfo[i].AlphaARG2     = D_D3DTA_DIFFUSE ;
		GD3D9.Device.State.BlendInfo.TextureStageInfo[i].AlphaOP       = D_D3DTOP_DISABLE ;
		GD3D9.Device.State.BlendInfo.TextureStageInfo[i].ColorARG1     = D_D3DTA_TEXTURE ;
		GD3D9.Device.State.BlendInfo.TextureStageInfo[i].ColorARG2     = D_D3DTA_DIFFUSE ;
		GD3D9.Device.State.BlendInfo.TextureStageInfo[i].ColorOP       = D_D3DTOP_DISABLE ;
	}

	GD3D9.Device.State.Lighting				= TRUE ;
	GD3D9.Device.State.MaxAnisotropy		= 1 ;
	GD3D9.Device.State.ShadeMode			= DX_SHADEMODE_FLAT ;
	GD3D9.Device.State.ZEnable				= TRUE ;
	GD3D9.Device.State.ZWriteEnable			= FALSE ; 
	GD3D9.Device.State.ZFunc				= DX_CMP_LESSEQUAL ;
	GD3D9.Device.State.DepthBias			= 0 ;
	GD3D9.Device.State.FillMode				= D_D3DFILL_SOLID ;
	GD3D9.Device.State.CullMode				= FALSE ;
	GD3D9.Device.State.UseSpecular			= TRUE ;
	GD3D9.Device.State.FogEnable			= FALSE ;
	GD3D9.Device.State.FogColor				= 0 ;
	GD3D9.Device.State.FogMode				= DX_FOGMODE_NONE ;
	GD3D9.Device.State.FogStart				= 0.0f ;
	GD3D9.Device.State.FogEnd				= 1.0f ;
	for( i = 0 ; i < USE_TEXTURESTAGE_NUM ; i ++ )
	{
		GD3D9.Device.State.TexAddressModeU[ i ] = GSYS.DrawSetting.TexAddressModeU[ i ] ;
		GD3D9.Device.State.TexAddressModeV[ i ] = GSYS.DrawSetting.TexAddressModeV[ i ] ;
		GD3D9.Device.State.TexAddressModeW[ i ] = GSYS.DrawSetting.TexAddressModeW[ i ] ;
	}

	GD3D9.Device.State.Viewport.X					= 0 ;
	GD3D9.Device.State.Viewport.Y					= 0 ;
	GD3D9.Device.State.Viewport.Width				= ( DWORD )GSYS.Screen.MainScreenSizeX ;
	GD3D9.Device.State.Viewport.Height				= ( DWORD )GSYS.Screen.MainScreenSizeY ;
	GD3D9.Device.State.Viewport.MinZ				= 0.0f ;
	GD3D9.Device.State.Viewport.MaxZ				= 1.0f ;
	GD3D9.Device.State.DrawMode						= DX_DRAWMODE_OTHER ;
	GD3D9.Device.State.TargetSurface[ 0 ]			= GD3D9.Device.Screen.BackBufferSurface ;

	GD3D9.Device.DrawSetting.DrawPrepAlwaysFlag		= TRUE ;
	GD3D9.Device.DrawSetting.UseDiffuseRGBColor		= FALSE ;
	GD3D9.Device.DrawSetting.UseDiffuseAlphaColor	= FALSE ;
	GD3D9.Device.DrawSetting.BlendMode				= DX_BLENDMODE_NOBLEND ;
	GD3D9.Device.DrawSetting.BlendGraphBorderParam	= -1 ;
	GD3D9.Device.DrawSetting.AlphaTestMode			= -1 ;
	GD3D9.Device.DrawSetting.AlphaTestParam			= 0 ;
	GD3D9.Device.DrawSetting.AlphaChannelValidFlag	= FALSE ;
	GD3D9.Device.DrawSetting.AlphaTestValidFlag		= FALSE ;
	GD3D9.Device.DrawSetting.ChangeBlendParamFlag	= TRUE ;
	GD3D9.Device.DrawSetting.ChangeTextureFlag		= FALSE ;
	GD3D9.Device.DrawSetting.BlendTexture			= NULL ;
	GD3D9.Device.DrawSetting.BlendGraphType			= 0 ;
	GD3D9.Device.DrawSetting.RenderTexture			= NULL ;

	GD3D9.Device.DrawInfo.DiffuseColor				= 0xffffffff ;
	GD3D9.Device.DrawInfo.BlendMaxNotDrawFlag		= FALSE ;
	GD3D9.Device.DrawInfo.BeginSceneFlag			= FALSE ;
	GD3D9.Device.DrawInfo.VertexNum					= 0 ;
	GD3D9.Device.DrawInfo.VertexBufferNextAddr		= GD3D9.Device.DrawInfo.VertexBuffer_Tex ;
	GD3D9.Device.DrawInfo.VertexType				= VERTEXTYPE_TEX ;

//	GRH.PerspectiveFlag			= FALSE ;
//	GRH.AlphaValidFlag			= FALSE ;
//	GRH.AlphaBlendEnable		= FALSE ;
//	GRH.BlendParam				= 0 ;
//	GRH.VertexBufferUseFlag		= FALSE ;
//	GRH.ColorKeyFlag			= FALSE ;

	// �ݒ�����t���b�V��
	Graphics_D3D9_DeviceState_RefreshRenderState() ;

	// ���܂��Ȃ�
	{
		VERTEX_NOTEX_2D *vec ;

		Graphics_D3D9_BeginScene() ;
		Graphics_D3D9_DrawPreparation( 0 ) ;

//		GETVERTEX_POINT( vec ) ;
		GD3D9.Device.DrawInfo.VertexNum            = 0 ;
		GD3D9.Device.DrawInfo.VertexBufferNextAddr = GD3D9.Device.DrawInfo.VertexBufferAddr[ 0 ][ GD3D9.Device.DrawInfo.VertexType ] ;
		GD3D9.Device.DrawInfo.Use3DVertex          = 0 ;
		GD3D9.Device.DrawInfo.PrimitiveType        = D_D3DPT_POINTLIST ;
		*( (DWORD_PTR *)&vec) = (DWORD_PTR)GD3D9.Device.DrawInfo.VertexBufferNextAddr ;


		vec->pos.x = 0.0f ;
		vec->pos.y = 0.0f ;
		vec->pos.z = 0.0f ;


//		ADD4VERTEX_POINT
		GD3D9.Device.DrawInfo.VertexBufferNextAddr += sizeof( VERTEX_NOTEX_2D ) ;
		GD3D9.Device.DrawInfo.VertexNum ++ ;
	}

	// �I��
	return 0 ;
}

// �ꎞ�I�� Direct3DDevice9 ��j�������ۂɁA�j���O�̏�Ԃ����o�����߂̊֐�
extern	int		Graphics_D3D9_Device_ReInitialize( void )
{
	int i ;

	if( GAPIWin.Direct3DDevice9Object == NULL ) return -1 ;

	// �`��X�e�[�^�X�̃Z�b�g
	{
		// �e�N�X�`���`��p�ݒ�
//		Direct3DDevice9_SetTextureStageState( 0, D_D3DTSS_ADDRESS, D_D3DTADDRESS_CLAMP );

		// �f�B�U�����O�n�e�e
		Direct3DDevice9_SetRenderState( D_D3DRS_DITHERENABLE, FALSE ) ;
	}

	// �������t���O�𗧂Ă�
	GD3D9.Device.DrawSetting.CancelSettingEqualCheck = TRUE ;

	// �K�� Graphics_D3D9_DrawPreparation �����s����t���O�𗧂Ă�
	GD3D9.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

	// �폜�O�Ɠ����������o��
	{
		// �`���̐ݒ�
//		if( GD3D9.Device.State.TargetSurface != NULL )
//			SetRenderTargetHardware( GD3D9.Device.State.TargetSurface ) ;

		// �e�N�X�`���̃��Z�b�g
		GD3D9.Device.DrawSetting.RenderTexture = NULL ;

		// �`�撸�_�����Z�b�g
		GD3D9.Device.DrawInfo.VertexNum = 0 ;

		// �`��u�����h���[�h�̐ݒ�
		Graphics_D3D9_DrawSetting_SetDrawBlendMode( GD3D9.Device.DrawSetting.BlendMode, GD3D9.Device.DrawSetting.AlphaTestValidFlag, GD3D9.Device.DrawSetting.AlphaChannelValidFlag ) ;
//		Graphics_D3D9_DeviceState_RefreshBlendState( DevHandle ) ;

		// �`��摜�̐F�𖳎����邩�ǂ������Z�b�g����
		Graphics_D3D9_DrawSetting_SetIgnoreDrawGraphColor( GD3D9.Device.DrawSetting.IgnoreGraphColorFlag ) ;

		// �J�����O���[�h�̃Z�b�g
		Graphics_D3D9_DeviceState_SetCullMode( GD3D9.Device.State.CullMode ) ;

		// �`�惂�[�h�̐ݒ�
		Graphics_D3D9_DeviceState_SetDrawMode( GD3D9.Device.State.DrawMode ) ;

		// �V�F�[�f�B���O���[�h�̃Z�b�g
		Graphics_D3D9_DeviceState_SetShadeMode( GD3D9.Device.State.ShadeMode ) ;

		// �ٕ����t�B���^�����O�̐ݒ���Z�b�g
		Graphics_D3D9_DeviceState_SetMaxAnisotropy( GD3D9.Device.State.MaxAnisotropy ) ;

		// �J�����O���[�h�̐ݒ���Z�b�g
		Graphics_D3D9_DeviceState_SetCullMode( GD3D9.Device.State.CullMode ) ;

		// UV�A�h���b�V���O���[�h�̐ݒ���Z�b�g
		for( i = 0 ; i < 8 ; i ++ )
		{
			Graphics_D3D9_DeviceState_SetTextureAddressU( GD3D9.Device.State.TexAddressModeU[ i ], i ) ;
			Graphics_D3D9_DeviceState_SetTextureAddressV( GD3D9.Device.State.TexAddressModeV[ i ], i ) ;
			Graphics_D3D9_DeviceState_SetTextureAddressW( GD3D9.Device.State.TexAddressModeW[ i ], i ) ;
		}

		// �e�N�X�`�����W�ϊ��s����g�p���邩�ǂ����̐ݒ���Z�b�g
		for( i = 0 ; i < 8 ; i ++ )
		{
			Graphics_D3D9_DeviceState_SetTextureAddressTransformMatrix( GD3D9.Device.State.TextureTransformUse[ i ], &GD3D9.Device.State.TextureTransformMatrix[ i ], i ) ;
		}

		// �t�H�O�̐ݒ���Z�b�g
		Graphics_D3D9_DeviceState_SetFogEnable(     GD3D9.Device.State.FogEnable ) ;
		Graphics_D3D9_DeviceState_SetFogVertexMode( GD3D9.Device.State.FogMode ) ;
		Graphics_D3D9_DeviceState_SetFogColor(      GD3D9.Device.State.FogColor ) ;
		Graphics_D3D9_DeviceState_SetFogStartEnd(   GD3D9.Device.State.FogStart, GD3D9.Device.State.FogEnd ) ;
		Graphics_D3D9_DeviceState_SetFogDensity(    GD3D9.Device.State.FogDensity ) ;

		// �y�o�b�t�@�̐ݒ���Z�b�g
		Graphics_D3D9_DeviceState_SetZEnable(       GD3D9.Device.State.ZEnable ) ;
		Graphics_D3D9_DeviceState_SetZWriteEnable(  GD3D9.Device.State.ZWriteEnable ) ;
		Graphics_D3D9_DeviceState_SetZFunc(         GD3D9.Device.State.ZFunc ) ;
		Graphics_D3D9_DeviceState_SetDepthBias(     GD3D9.Device.State.DepthBias ) ;
		Graphics_D3D9_DeviceState_SetFillMode(      GD3D9.Device.State.FillMode ) ;

		// �s��̍Đݒ�
		NS_SetTransformToWorldD(                              &GSYS.DrawSetting.WorldMatrix ) ;
		NS_SetTransformToViewD(                               &GSYS.DrawSetting.ViewMatrix ) ;
		Graphics_DrawSetting_SetTransformToProjection_Direct( &GSYS.DrawSetting.ProjectionMatrix ) ;
		NS_SetTransformToViewportD(                           &GSYS.DrawSetting.ViewportMatrix ) ;

		// �}�e���A���̍Đݒ�
		Graphics_D3D9_DeviceState_SetMaterial(               &GD3D9.Device.State.Material ) ;
		Graphics_D3D9_DeviceState_SetSpecularEnable(          GD3D9.Device.State.UseSpecular ) ;
		Graphics_D3D9_DeviceState_SetDiffuseMaterialSource(   GD3D9.Device.State.MaterialUseVertexDiffuseColor ) ;
		Graphics_D3D9_DeviceState_SetSpecularMaterialSource(  GD3D9.Device.State.MaterialUseVertexSpecularColor ) ;

		// ���C�g�̍Đݒ�
		Graphics_D3D9_DeviceState_SetAmbient(
			( ( ( DWORD )( _FTOL( GD3D9.Device.State.GlobalAmbientLightColor.a ) * 255 ) ) << 24 ) | 
			( ( ( DWORD )( _FTOL( GD3D9.Device.State.GlobalAmbientLightColor.r ) * 255 ) ) << 16 ) | 
			( ( ( DWORD )( _FTOL( GD3D9.Device.State.GlobalAmbientLightColor.g ) * 255 ) ) << 8  ) | 
			( ( ( DWORD )( _FTOL( GD3D9.Device.State.GlobalAmbientLightColor.b ) * 255 ) ) << 0  ) ) ;
		Graphics_D3D9_DeviceState_SetLighting( GD3D9.Device.State.Lighting ) ;
		for( i = 0 ; i < 256 ; i ++ )
		{
			if( GD3D9.Device.State.LightParam[ i ].Type == 0 ) continue ;
			Graphics_D3D9_DeviceState_SetLightState( i, &GD3D9.Device.State.LightParam[ i ] ) ;
			Graphics_D3D9_DeviceState_SetLightEnable( i, GD3D9.Device.State.LightEnableFlag[ i ] ) ;
		}

		// �`��̈�̐ݒ�
		{
			D_D3DVIEWPORT9 Viewport ;

			// �r���[�|�[�g�̃Z�b�e�B���O
			_MEMSET( &Viewport, 0, sizeof( Viewport ) ) ;
			Viewport.X		= ( DWORD )GSYS.DrawSetting.DrawArea.left ;
			Viewport.Y		= ( DWORD )GSYS.DrawSetting.DrawArea.top ;
			Viewport.Width	= ( DWORD )( GSYS.DrawSetting.DrawArea.right  - GSYS.DrawSetting.DrawArea.left ) ;
			Viewport.Height	= ( DWORD )( GSYS.DrawSetting.DrawArea.bottom - GSYS.DrawSetting.DrawArea.top  ) ;
			Viewport.MinZ	= 0.0f ;
			Viewport.MaxZ	= 1.0f ;

			// �Z�b�g
			Graphics_D3D9_DeviceState_SetViewport( &Viewport ) ;
			Graphics_D3D9_DeviceState_SetScissorRect( &GSYS.DrawSetting.DrawArea ) ;
		}

		// �e�X�e�[�W�Ŏg�p����e�N�X�`�����W�̃��Z�b�g
		Graphics_D3D9_DeviceState_ResetTextureCoord() ;
	
		// �ݒ�𔽉f
		Graphics_D3D9_DeviceState_RefreshBlendState( NULL, 0, 1, FALSE ) ;
		Graphics_D3D9_DeviceState_RefreshBlendState( NULL, 0, 1, TRUE ) ;
		Graphics_D3D9_DeviceState_RefreshRenderState() ;
	}

	// �������t���O��|��
	GD3D9.Device.DrawSetting.CancelSettingEqualCheck = FALSE ;

	// �I��
	return 0 ;
}
























// Direct3DDevice9 �X�e�[�^�X�֌W

// �`��ݒ�����t���b�V��
extern	void	Graphics_D3D9_DeviceState_RefreshRenderState( void )
{
	if( GAPIWin.Direct3DDevice9Object == NULL ) return ;

	// �������t���O�𗧂Ă�
	GD3D9.Device.DrawSetting.CancelSettingEqualCheck = TRUE ;

	// Graphics_D3D9_DrawPreparation ���s���ׂ��t���O�𗧂Ă�
	GD3D9.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

	// �ݒ�̃��t���b�V��
	Graphics_D3D9_DeviceState_SetFogEnable             ( GD3D9.Device.State.FogEnable           ) ;
	Graphics_D3D9_DeviceState_SetViewport              ( &GD3D9.Device.State.Viewport           ) ;
	Graphics_D3D9_DeviceState_SetScissorRect           ( &GD3D9.Device.State.ScissorRect        ) ;
	Graphics_D3D9_DrawSetting_SetTexture               ( GD3D9.Device.DrawSetting.RenderTexture ) ;
	Graphics_D3D9_DeviceState_SetBaseState             (                                        ) ;
	Graphics_D3D9_DeviceState_SetDrawMode              ( GD3D9.Device.State.DrawMode            ) ;
//	Graphics_D3D9_DrawSetting_SetDrawBlendMode         ( GD3D9.Device.DrawSetting.BlendMode, GD3D9.Device.DrawSetting.AlphaTestValidFlag, GD3D9.Device.DrawSetting.AlphaChannelValidFlag  ) ;
	Graphics_D3D9_DeviceState_RefreshBlendState        ( NULL, 0, 1, FALSE                      ) ;
	Graphics_D3D9_DeviceState_RefreshBlendState        ( NULL, 0, 1, TRUE                       ) ;
	Graphics_D3D9_DeviceState_SetZEnable               ( GD3D9.Device.State.ZEnable             ) ;
	Graphics_D3D9_DeviceState_SetZWriteEnable          ( GD3D9.Device.State.ZWriteEnable        ) ;
	Graphics_D3D9_DeviceState_SetZFunc                 ( GD3D9.Device.State.ZFunc               ) ;
	Graphics_D3D9_DeviceState_SetDepthBias             ( GD3D9.Device.State.DepthBias           ) ;
	Graphics_D3D9_DeviceState_SetFillMode              ( GD3D9.Device.State.FillMode            ) ;
	Graphics_D3D9_DeviceState_SetCullMode              ( GD3D9.Device.State.CullMode            ) ;
	Graphics_D3D9_DeviceState_SetShadeMode             ( GD3D9.Device.State.ShadeMode           ) ;
	Graphics_D3D9_DeviceState_SetMaterial              ( &GD3D9.Device.State.Material           ) ;
	Graphics_D3D9_DeviceState_SetSpecularEnable        ( GD3D9.Device.State.UseSpecular         ) ;
	Graphics_D3D9_DeviceState_SetDiffuseMaterialSource ( GD3D9.Device.State.MaterialUseVertexDiffuseColor ) ;
	Graphics_D3D9_DeviceState_SetSpecularMaterialSource( GD3D9.Device.State.MaterialUseVertexSpecularColor ) ;

	if( GAPIWin.Direct3DDevice9Object != NULL )
	{
		Direct3DDevice9_SetVertexShaderConstantF( 0, ( float * )GD3D9.Device.Shader.ShaderConstantInfo.FixInfo[ DX_SHADERCONSTANTTYPE_VS_FLOAT ].Float4, 256 ) ;
		Direct3DDevice9_SetVertexShaderConstantI( 0, ( int   * )GD3D9.Device.Shader.ShaderConstantInfo.FixInfo[ DX_SHADERCONSTANTTYPE_VS_INT   ].Int4,   16  ) ;
		Direct3DDevice9_SetVertexShaderConstantB( 0, ( BOOL  * )GD3D9.Device.Shader.ShaderConstantInfo.FixInfo[ DX_SHADERCONSTANTTYPE_VS_BOOL  ].Bool,   16  ) ;

		Direct3DDevice9_SetPixelShaderConstantF( 0, ( float * )GD3D9.Device.Shader.ShaderConstantInfo.FixInfo[ DX_SHADERCONSTANTTYPE_PS_FLOAT ].Float4, 224 ) ;
		Direct3DDevice9_SetPixelShaderConstantI( 0, ( int   * )GD3D9.Device.Shader.ShaderConstantInfo.FixInfo[ DX_SHADERCONSTANTTYPE_PS_INT   ].Int4,   16  ) ;
		Direct3DDevice9_SetPixelShaderConstantB( 0, ( BOOL  * )GD3D9.Device.Shader.ShaderConstantInfo.FixInfo[ DX_SHADERCONSTANTTYPE_PS_BOOL  ].Bool,   16  ) ;
	}

	// �������t���O��|��
	GD3D9.Device.DrawSetting.CancelSettingEqualCheck = FALSE ;
}

// Direct3DDevice9 �̊�{�ݒ���s��
extern	int		Graphics_D3D9_DeviceState_SetBaseState( void )
{
	if( GAPIWin.Direct3DDevice9Object == NULL ) return -1 ;

	if( GD3D9.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
	{
		// �`��ҋ@���Ă���`�敨��`��
		DRAWSTOCKINFO
	}

	// �A���t�@�u�����f�B���O�J���[�L�[����
//	Direct3DDevice9_SetRenderState( D_D3DRS_COLORKEYBLENDENABLE, FALSE ) ;

	// �Ō�̃h�b�g�`�悵�Ȃ�
//	Direct3DDevice9_SetRenderState( D_D3DRS_LASTPIXEL, TRUE ) ;

	// �e�N�X�`���`��p�ݒ�
	Direct3DDevice9_SetSamplerState( 0, D_D3DSAMP_ADDRESSU, ( DWORD )GD3D9.Device.State.TexAddressModeU[ 0 ] ) ;
	Direct3DDevice9_SetSamplerState( 0, D_D3DSAMP_ADDRESSV, ( DWORD )GD3D9.Device.State.TexAddressModeV[ 0 ] ) ;
	Direct3DDevice9_SetSamplerState( 1, D_D3DSAMP_ADDRESSU, ( DWORD )GD3D9.Device.State.TexAddressModeU[ 1 ] ) ;
	Direct3DDevice9_SetSamplerState( 1, D_D3DSAMP_ADDRESSV, ( DWORD )GD3D9.Device.State.TexAddressModeV[ 1 ] ) ;
	Direct3DDevice9_SetSamplerState( 2, D_D3DSAMP_ADDRESSU, ( DWORD )GD3D9.Device.State.TexAddressModeU[ 2 ] ) ;
	Direct3DDevice9_SetSamplerState( 2, D_D3DSAMP_ADDRESSV, ( DWORD )GD3D9.Device.State.TexAddressModeV[ 2 ] ) ;
	Direct3DDevice9_SetSamplerState( 3, D_D3DSAMP_ADDRESSU, ( DWORD )GD3D9.Device.State.TexAddressModeU[ 3 ] ) ;
	Direct3DDevice9_SetSamplerState( 3, D_D3DSAMP_ADDRESSV, ( DWORD )GD3D9.Device.State.TexAddressModeV[ 3 ] ) ;
	Direct3DDevice9_SetTextureStageState( 0, D_D3DTSS_TEXCOORDINDEX, 0 ) ;
	Direct3DDevice9_SetTextureStageState( 1, D_D3DTSS_TEXCOORDINDEX, 1 ) ;
	Direct3DDevice9_SetTextureStageState( 2, D_D3DTSS_TEXCOORDINDEX, 1 ) ;
	Direct3DDevice9_SetTextureStageState( 3, D_D3DTSS_TEXCOORDINDEX, 0 ) ;
	GD3D9.Device.State.BlendInfo.TextureStageInfo[ 0 ].TextureCoordIndex = 0 ;
	GD3D9.Device.State.BlendInfo.TextureStageInfo[ 1 ].TextureCoordIndex = 1 ;
	GD3D9.Device.State.BlendInfo.TextureStageInfo[ 2 ].TextureCoordIndex = 1 ;
	GD3D9.Device.State.BlendInfo.TextureStageInfo[ 3 ].TextureCoordIndex = 0 ;
	if( GD3D9.Device.Caps.ValidTexTempRegFlag )
	{
		Direct3DDevice9_SetTextureStageState( 0, D_D3DTSS_RESULTARG, D_D3DTA_CURRENT ) ;
		Direct3DDevice9_SetTextureStageState( 1, D_D3DTSS_RESULTARG, D_D3DTA_CURRENT ) ;
		Direct3DDevice9_SetTextureStageState( 2, D_D3DTSS_RESULTARG, D_D3DTA_CURRENT ) ;
		Direct3DDevice9_SetTextureStageState( 3, D_D3DTSS_RESULTARG, D_D3DTA_CURRENT ) ;
		GD3D9.Device.State.BlendInfo.TextureStageInfo[ 0 ].ResultTempARG = FALSE ;
		GD3D9.Device.State.BlendInfo.TextureStageInfo[ 1 ].ResultTempARG = FALSE ;
		GD3D9.Device.State.BlendInfo.TextureStageInfo[ 2 ].ResultTempARG = FALSE ;
		GD3D9.Device.State.BlendInfo.TextureStageInfo[ 3 ].ResultTempARG = FALSE ;
	}

	// �p�[�X�y�N�e�B�u�R���N�g�ݒ�
//	Direct3DDevice9_SetRenderState( D_D3DRS_TEXTUREPERSPECTIVE, GRH.PerspectiveFlag ) ;

	// �V�F�[�f�B���O���[�h�ݒ�
	Direct3DDevice9_SetRenderState( D_D3DRS_SHADEMODE, ( DWORD )GD3D9.Device.State.ShadeMode ) ;

	// ���C�e�B���O�ݒ�
	Direct3DDevice9_SetRenderState( D_D3DRS_LIGHTING, ( DWORD )GD3D9.Device.State.Lighting ) ;

	// �J�����O�ݒ�
	Direct3DDevice9_SetRenderState( D_D3DRS_CULLMODE, D_D3DCULL_NONE ) ;

	// �f�B�U�����O�n�e�e
	Direct3DDevice9_SetRenderState( D_D3DRS_DITHERENABLE, FALSE ) ;
	
	// �V�F�[�_�[�̒萔���Z�b�g
	static float ZeroOne[ 4 ]        = { 0.0f, 1.0f, 0.0f, 0.0f } ;
	static float ZeroHalfOneTwo[ 4 ] = { 0.0f, 0.5f, 1.0f, 2.0f } ;
	Graphics_D3D9_ShaderConstant_InfoSet_SetParam( &GD3D9.Device.Shader.ShaderConstantInfo, DX_SHADERCONSTANTTYPE_VS_FLOAT, DX_SHADERCONSTANTSET_LIB, DX_VS_CONSTF_ZERO_ONE,          ZeroOne,        1, TRUE ) ;
	Graphics_D3D9_ShaderConstant_InfoSet_SetParam( &GD3D9.Device.Shader.ShaderConstantInfo, DX_SHADERCONSTANTTYPE_PS_FLOAT, DX_SHADERCONSTANTSET_LIB, DX_PS_CONSTF_ZERO_HALF_ONE_TWO, ZeroHalfOneTwo, 1, TRUE ) ;

	// �Œ�@�\�p�C�v���C���ɖ߂�
	Graphics_D3D9_DeviceState_ResetVertexShader() ;
	Graphics_D3D9_DeviceState_ResetPixelShader() ;
	Graphics_D3D9_DeviceState_SetVertexBuffer( NULL ) ;
	Graphics_D3D9_DeviceState_SetIndexBuffer( NULL ) ;

	// �I��
	return 0 ;
}

// �T���v���[�̃e�N�X�`���t�B���^�����O���[�h��ݒ肷��
extern int	Graphics_D3D9_DeviceState_SetSampleFilterMode( int Sampler, int SetTarget, D_D3DTEXTUREFILTERTYPE FilterType )
{
	GRAPHICS_HARDDATA_DIRECT3D9_DEVICE_STATE *State = &GraphicsHardDataDirect3D9.Device.State ;

	if( GAPIWin.Direct3DDevice9Object == NULL )
	{
		return 0 ;
	}

	if( Sampler < 0 || Sampler >= USE_TEXTURESTAGE_NUM )
	{
		return 0 ;
	}

	GD3D9.Device.State.DrawMode           = DX_DRAWMODE_OTHER ;
	GD3D9.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

	switch( SetTarget )
	{
	case D_D3DTSS_MAGFILTER :
		if( State->MagFilter[ Sampler ] != FilterType )
		{
			Direct3DDevice9_SetSamplerState( ( DWORD )Sampler, D_D3DSAMP_MAGFILTER, FilterType ) ;
			State->MagFilter[ Sampler ] = FilterType ;
		}
		break ;

	case D_D3DTSS_MINFILTER :
		if( State->MinFilter[ Sampler ] != FilterType )
		{
			Direct3DDevice9_SetSamplerState( ( DWORD )Sampler, D_D3DSAMP_MINFILTER, FilterType ) ;
			State->MinFilter[ Sampler ] = FilterType ;
		}
		break ;

	case D_D3DTSS_MIPFILTER :
		if( State->MipFilter[ Sampler ] != FilterType )
		{
			Direct3DDevice9_SetSamplerState( ( DWORD )Sampler, D_D3DSAMP_MIPFILTER, FilterType ) ;
			State->MipFilter[ Sampler ] = FilterType ;
		}
		break ;
	}

	// �I��
	return 0 ;
}


// �y�o�b�t�@�̗L�������̃Z�b�g
extern int  Graphics_D3D9_DeviceState_SetZEnable( int Flag )
{
	if( GAPIWin.Direct3DDevice9Object == NULL ) return -1 ;

	// �t���O�������������牽�������I��
	if( GD3D9.Device.DrawSetting.CancelSettingEqualCheck == FALSE && GD3D9.Device.State.ZEnable == Flag ) return 0 ;

	if( GD3D9.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
	{
		// �`��ҋ@���Ă���`�敨��`��
		DRAWSTOCKINFO
	}

	// �y�o�b�t�@�̗L����Ԃ�ύX
	Direct3DDevice9_SetRenderState( D_D3DRS_ZENABLE, Flag == TRUE ? D_D3DZB_TRUE : D_D3DZB_FALSE ) ;
	GD3D9.Device.State.ZEnable = Flag ;
	GD3D9.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

	// �I��
	return 0 ;
}

// �y�o�b�t�@�̏������݂̗L�����Z�b�g
extern int  Graphics_D3D9_DeviceState_SetZWriteEnable( int Flag )
{
	if( GAPIWin.Direct3DDevice9Object == NULL ) return -1 ;

	// �t���O�������������牽�������I��
	if( GD3D9.Device.DrawSetting.CancelSettingEqualCheck == FALSE && GD3D9.Device.State.ZWriteEnable == Flag ) return 0 ;

	if( GD3D9.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
	{
		// �`��ҋ@���Ă���`�敨��`��
		DRAWSTOCKINFO
	}

	// �y�o�b�t�@�̏������ݏ�Ԃ�ύX
	Direct3DDevice9_SetRenderState( D_D3DRS_ZWRITEENABLE, ( DWORD )Flag ) ;
	GD3D9.Device.State.ZWriteEnable = Flag ;
	GD3D9.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

	// �I��
	return 0 ;
}

// �[�x�l�̔�r�^�C�v���Z�b�g
extern int  Graphics_D3D9_DeviceState_SetZFunc( int CmpType )
{
	if( GAPIWin.Direct3DDevice9Object == NULL ) return -1 ;

	// �^�C�v�������������牽�������I��
	if( GD3D9.Device.DrawSetting.CancelSettingEqualCheck == FALSE && GD3D9.Device.State.ZFunc == CmpType ) return 0 ;

	if( GD3D9.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
	{
		// �`��ҋ@���Ă���`�敨��`��
		DRAWSTOCKINFO
	}

	// �y�l�̔�r�^�C�v��ύX
	Direct3DDevice9_SetRenderState( D_D3DRS_ZFUNC, ( DWORD )CmpType ) ;
	GD3D9.Device.State.ZFunc = CmpType ;
	GD3D9.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

	// �I��
	return 0 ;
}

// �[�x�l�̃o�C�A�X���Z�b�g
extern int  Graphics_D3D9_DeviceState_SetDepthBias( int Bias )
{
	if( GAPIWin.Direct3DDevice9Object == NULL ) return -1 ;

	// �o�C�A�X�l�������������牽�������I��
	if( GD3D9.Device.DrawSetting.CancelSettingEqualCheck == FALSE && GD3D9.Device.State.DepthBias == Bias ) return 0 ;

	if( GD3D9.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
	{
		// �`��ҋ@���Ă���`�敨��`��
		DRAWSTOCKINFO
	}

	// �y�o�C�A�X��ύX
	Direct3DDevice9_SetRenderState( D_D3DRS_DEPTHBIAS, ( DWORD )Bias ) ;
	GD3D9.Device.State.DepthBias = Bias ;
	GD3D9.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

	// �I��
	return 0 ;
}

// �t�B�����[�h���Z�b�g
extern int  Graphics_D3D9_DeviceState_SetFillMode( int FillMode )
{
	if( GAPIWin.Direct3DDevice9Object == NULL ) return -1 ;

	// �t�B�����[�h�������������牽�������I��
	if( GD3D9.Device.DrawSetting.CancelSettingEqualCheck == FALSE && GD3D9.Device.State.FillMode == FillMode ) return 0 ;

	if( GD3D9.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
	{
		// �`��ҋ@���Ă���`�敨��`��
		DRAWSTOCKINFO
	}

	// �t�B�����[�h��ύX
	Direct3DDevice9_SetRenderState( D_D3DRS_FILLMODE, ( DWORD )FillMode ) ;
	GD3D9.Device.State.FillMode = FillMode ;
	GD3D9.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

	// �I��
	return 0 ;
}


// ���[���h�ϊ��p�s����Z�b�g����
extern int Graphics_D3D9_DeviceState_SetWorldMatrix( const MATRIX *Matrix )
{
	if( GAPIWin.Direct3DDevice9Object == NULL ) return 0 ;

	Direct3DDevice9_SetTransform( D_D3DTS_WORLDMATRIX( 0 ), ( D_D3DMATRIX * )Matrix ) ;

//	if( GSYS.HardInfo.UseShader )
	{
		// �]�u�s��𒸓_�V�F�[�_�[�ɃZ�b�g����
		MATRIX TransMat ;
		CreateTransposeMatrix( &TransMat, Matrix ) ;
		Graphics_D3D9_ShaderConstant_InfoSet_SetParam(
			&GD3D9.Device.Shader.ShaderConstantInfo,
			DX_SHADERCONSTANTTYPE_VS_FLOAT,
			DX_SHADERCONSTANTSET_LIB,
			DX_VS_CONSTF_WORLD_MAT_START,
			&TransMat,
			4,
			TRUE
		) ;
	}

	// �I��
	return 0 ;
}

// �r���[�ϊ��p�s����Z�b�g����
extern int Graphics_D3D9_DeviceState_SetViewMatrix( const MATRIX *Matrix )
{
	if( GAPIWin.Direct3DDevice9Object == NULL ) return 0 ;

	Direct3DDevice9_SetTransform( D_D3DTS_VIEW, ( D_D3DMATRIX * )Matrix ) ;

//	if( GSYS.HardInfo.UseShader )
	{
		int i ;
		VECTOR Dir, Pos ;
		float Data[ 2 ][ 4 ] ;
		MATRIX RotateTransMat ;

		// �]�u�s��𒸓_�V�F�[�_�[�ɃZ�b�g����
		MATRIX TransMat ;
		CreateTransposeMatrix( &TransMat, Matrix ) ;
		Graphics_D3D9_ShaderConstant_InfoSet_SetParam(
			&GD3D9.Device.Shader.ShaderConstantInfo,
			DX_SHADERCONSTANTTYPE_VS_FLOAT,
			DX_SHADERCONSTANTSET_LIB,
			DX_VS_CONSTF_VIEW_MAT,
			&TransMat,
			4,
			TRUE
		) ;

		// ���C�g�̈ʒu�E�����̍Čv�Z
		RotateTransMat = *Matrix ;
		RotateTransMat.m[ 3 ][ 0 ] = 0.0f ;
		RotateTransMat.m[ 3 ][ 1 ] = 0.0f ;
		RotateTransMat.m[ 3 ][ 2 ] = 0.0f ;
		for( i = 0 ; i < DX_PS_CONSTB_LIGHT2_NUM ; i ++ )
		{
			VectorTransform( &Dir, ( VECTOR * )&GD3D9.Device.State.LightParam[ i ].Direction, &RotateTransMat ) ;
			VectorTransform( &Pos, ( VECTOR * )&GD3D9.Device.State.LightParam[ i ].Position,  &GSYS.DrawSetting.ViewMatrixF ) ;
			Data[ 0 ][ 0 ] = Pos.x ;
			Data[ 0 ][ 1 ] = Pos.y ;
			Data[ 0 ][ 2 ] = Pos.z ;
			Data[ 0 ][ 3 ] = 1.0f ;
			Data[ 1 ][ 0 ] = Dir.x ;
			Data[ 1 ][ 1 ] = Dir.y ;
			Data[ 1 ][ 2 ] = Dir.z ;
			Data[ 1 ][ 3 ] = 0.0f ;
			if( i < DX_VS_CONSTF_LIGHT_NUM )
			{
				Graphics_D3D9_ShaderConstant_InfoSet_SetParam(
					&GD3D9.Device.Shader.ShaderConstantInfo,
					DX_SHADERCONSTANTTYPE_VS_FLOAT,
					DX_SHADERCONSTANTSET_LIB,
					DX_VS_CONSTF_LIGHT_START + i * DX_VS_CONSTF_LIGHT_UNITSIZE,
					Data[ 0 ],
					2,
					TRUE
				) ;
			}
			Graphics_D3D9_ShaderConstant_InfoSet_SetParam(
				&GD3D9.Device.Shader.ShaderConstantInfo,
				DX_SHADERCONSTANTTYPE_PS_FLOAT,
				DX_SHADERCONSTANTSET_LIB,
				DX_PS_CONSTF_LIGHT2_START + i * DX_PS_CONSTF_LIGHT2_UNITSIZE,
				Data[ 0 ],
				2,
				TRUE
			) ;
		}
	}

	// �I��
	return 0 ;
}

// ���e�ϊ��p�s����Z�b�g����
extern int Graphics_D3D9_DeviceState_SetProjectionMatrix( const MATRIX *Matrix )
{
	MATRIX TransMat ;

	if( GAPIWin.Direct3DDevice9Object == NULL ) return 0 ;

	Direct3DDevice9_SetTransform( D_D3DTS_PROJECTION, ( D_D3DMATRIX * )Matrix ) ;

	// �]�u�s��𒸓_�V�F�[�_�[�ɃZ�b�g����
//	if( GSYS.HardInfo.UseShader )
	{
		CreateTransposeMatrix( &TransMat, Matrix ) ;
		Graphics_D3D9_ShaderConstant_InfoSet_SetParam(
			&GD3D9.Device.Shader.ShaderConstantInfo,
			DX_SHADERCONSTANTTYPE_VS_FLOAT,
			DX_SHADERCONSTANTSET_LIB,
			DX_VS_CONSTF_PROJECTION_MAT,
			&TransMat,
			4,
			TRUE
		) ;
	}

	// �I��
	return 0 ;
}

// ���C�g�̃Z�b�g
extern int Graphics_D3D9_DeviceState_SetLightState( int LightNumber, D_D3DLIGHT9 *Light )
{
	if( GAPIWin.Direct3DDevice9Object == NULL ) return 0 ;

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �p�����[�^�̕ۑ�
	if( LightNumber < 256 && LightNumber >= 0 )
	{
		GD3D9.Device.State.LightParam[ LightNumber ] = *Light ;
	}

	// �o�[�e�b�N�X�V�F�[�_�[�ƃs�N�Z���V�F�[�_�[�ɂ����f����
//	if( GSYS.HardInfo.UseShader )
	{
		if( LightNumber < DX_PS_CONSTB_LIGHT2_NUM )
		{
			float Data[ 7 ][ 4 ] ;
			float ThetaSin, ThetaCos ;
			float PhiSin, PhiCos ;
			VECTOR Dir, Pos ;

			Dir = VTransformSR( *( ( VECTOR * )&Light->Direction ), GSYS.DrawSetting.ViewMatrixF ) ;
			Pos = VTransform(   *( ( VECTOR * )&Light->Position ),  GSYS.DrawSetting.ViewMatrixF ) ;
			Data[ 0 ][ 0 ] = Pos.x ;
			Data[ 0 ][ 1 ] = Pos.y ;
			Data[ 0 ][ 2 ] = Pos.z ;
			Data[ 0 ][ 3 ] = 1.0f ;
			Data[ 1 ][ 0 ] = Dir.x ;
			Data[ 1 ][ 1 ] = Dir.y ;
			Data[ 1 ][ 2 ] = Dir.z ;
			Data[ 1 ][ 3 ] = 0.0f ;
			Data[ 2 ][ 0 ] = Light->Diffuse.r ;
			Data[ 2 ][ 1 ] = Light->Diffuse.g ;
			Data[ 2 ][ 2 ] = Light->Diffuse.b ;
			Data[ 2 ][ 3 ] = Light->Diffuse.a ;
			Data[ 3 ][ 0 ] = Light->Specular.r ;
			Data[ 3 ][ 1 ] = Light->Specular.g ;
			Data[ 3 ][ 2 ] = Light->Specular.b ;
			Data[ 3 ][ 3 ] = Light->Specular.a ;
			Data[ 4 ][ 0 ] = Light->Ambient.r ;
			Data[ 4 ][ 1 ] = Light->Ambient.g ;
			Data[ 4 ][ 2 ] = Light->Ambient.b ;
			Data[ 4 ][ 3 ] = Light->Ambient.a ;
			Data[ 5 ][ 0 ] = Light->Range * Light->Range ;
			Data[ 5 ][ 1 ] = Light->Falloff ;
			Data[ 5 ][ 2 ] = Light->Attenuation0 ;
			Data[ 5 ][ 3 ] = Light->Attenuation1 ;
			Data[ 6 ][ 0 ] = Light->Attenuation2 ;
			_SINCOS( Light->Theta / 2.0f, &ThetaSin, &ThetaCos ) ;
			_SINCOS( Light->Phi / 2.0f, &PhiSin, &PhiCos ) ;
			Data[ 6 ][ 1 ] = PhiCos ;
			Data[ 6 ][ 2 ] = 1.0f / ( ThetaCos - PhiCos ) ;
			Data[ 6 ][ 3 ] = 0.0f ;
			if( LightNumber < DX_VS_CONSTF_LIGHT_NUM )
			{
				Graphics_D3D9_ShaderConstant_InfoSet_SetParam(
					&GD3D9.Device.Shader.ShaderConstantInfo,
					DX_SHADERCONSTANTTYPE_VS_FLOAT,
					DX_SHADERCONSTANTSET_LIB,
					DX_VS_CONSTF_LIGHT_START + LightNumber * DX_VS_CONSTF_LIGHT_UNITSIZE,
					Data[ 0 ],
					DX_VS_CONSTF_LIGHT_UNITSIZE,
					TRUE
				) ;
				Graphics_D3D9_ShaderConstant_InfoSet_SetParam(
					&GD3D9.Device.Shader.ShaderConstantInfo,
					DX_SHADERCONSTANTTYPE_PS_FLOAT,
					DX_SHADERCONSTANTSET_LIB,
					DX_PS_CONSTF_LIGHT_START  + LightNumber * DX_PS_CONSTF_LIGHT_UNITSIZE,
					Data[ 2 ],
					DX_PS_CONSTF_LIGHT_UNITSIZE,
					TRUE
				) ;
			}
			Graphics_D3D9_ShaderConstant_InfoSet_SetParam(
				&GD3D9.Device.Shader.ShaderConstantInfo,
				DX_SHADERCONSTANTTYPE_PS_FLOAT,
				DX_SHADERCONSTANTSET_LIB,
				DX_PS_CONSTF_LIGHT2_START + LightNumber * DX_PS_CONSTF_LIGHT2_UNITSIZE,
				Data[ 0 ],
				DX_PS_CONSTF_LIGHT2_UNITSIZE,
				TRUE
			) ;

			// �A���r�G���g�J���[�ƃG�~�b�V�u�J���[���|�����킹�������X�V����
			Graphics_D3D9_DeviceState_RefreshAmbientAndEmissiveParam() ;
		}

		if( LightNumber < DX_VS_CONSTB_LIGHT_NUM )
		{
			BOOL BData[ 2 ] ;
			BData[ 0 ] = Light->Type == D_D3DLIGHT_SPOT || Light->Type == D_D3DLIGHT_POINT ;
			BData[ 1 ] = Light->Type == D_D3DLIGHT_SPOT ;
			Graphics_D3D9_ShaderConstant_InfoSet_SetParam(
				&GD3D9.Device.Shader.ShaderConstantInfo,
				DX_SHADERCONSTANTTYPE_VS_BOOL,
				DX_SHADERCONSTANTSET_LIB,
				DX_VS_CONSTB_LIGHT_START + LightNumber * DX_VS_CONSTB_LIGHT_UNITSIZE + DX_VS_CONSTB_LGT_POINT_OR_SPOT,
				BData,
				2,
				TRUE
			) ;
		}

		// �g�p����V�F�[�_�[�̃C���f�b�N�X���X�V����
		Graphics_D3D9_Shader_RefreshUseIndex() ;
	}

	return Direct3DDevice9_SetLight( ( DWORD )LightNumber, ( D_D3DLIGHT9 * )Light ) == D_D3D_OK ? 0 : -1 ;
}

// ���C�g�̗L���A������ύX
extern int Graphics_D3D9_DeviceState_SetLightEnable( int LightNumber, int EnableState )
{
	if( GAPIWin.Direct3DDevice9Object == NULL ) return 0 ;

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	if( LightNumber < 256 )
	{
		GD3D9.Device.State.LightEnableFlag[ LightNumber ] = EnableState ;

		// �L���ȃ��C�g�̍ő�C���f�b�N�X���X�V
		if( EnableState == TRUE )
		{
			if( GD3D9.Device.State.LightEnableMaxIndex < LightNumber )
				GD3D9.Device.State.LightEnableMaxIndex = LightNumber ;
		}
		else
		{
			if( GD3D9.Device.State.LightEnableMaxIndex == LightNumber )
			{
				int i ;

				for( i = LightNumber ; i > 0 && GD3D9.Device.State.LightEnableFlag[ i ] == FALSE ; i -- ){}
				GD3D9.Device.State.LightEnableMaxIndex = i ;
			}
		}

		// �A���r�G���g�J���[�ƃG�~�b�V�u�J���[���|�����킹�������X�V����
		Graphics_D3D9_DeviceState_RefreshAmbientAndEmissiveParam() ;
	}

	if( GSYS.HardInfo.UseShader == TRUE && LightNumber < DX_VS_CONSTB_LIGHT_NUM )
	{
		BOOL BData ;
		BData = EnableState ;
		Graphics_D3D9_ShaderConstant_InfoSet_SetParam(
			&GD3D9.Device.Shader.ShaderConstantInfo,
			DX_SHADERCONSTANTTYPE_VS_BOOL,
			DX_SHADERCONSTANTSET_LIB,
			DX_VS_CONSTB_LIGHT_START + LightNumber * DX_VS_CONSTB_LIGHT_UNITSIZE + DX_VS_CONSTB_LGT_USE,
			&BData,
			1,
			TRUE
		) ;
	}

	// �g�p����V�F�[�_�[�̃C���f�b�N�X���X�V����
	Graphics_D3D9_Shader_RefreshUseIndex() ;

	return Direct3DDevice9_LightEnable( ( DWORD )LightNumber, EnableState ) == D_D3D_OK ? 0 : -1 ;
}

// �}�e���A���̃Z�b�g
extern int Graphics_D3D9_DeviceState_SetMaterial( D_D3DMATERIAL9 *Material )
{
	if( GAPIWin.Direct3DDevice9Object == NULL ) return 0 ;

	// �����̃p�����[�^�Ɠ����ꍇ�͉������Ȃ�
	if( GD3D9.Device.DrawSetting.CancelSettingEqualCheck == FALSE &&
		*( ( DWORD * )&GD3D9.Device.State.Material.Ambient.a )  == *( ( DWORD * )&Material->Ambient.a  ) &&
		*( ( DWORD * )&GD3D9.Device.State.Material.Ambient.r )  == *( ( DWORD * )&Material->Ambient.r  ) &&
		*( ( DWORD * )&GD3D9.Device.State.Material.Ambient.g )  == *( ( DWORD * )&Material->Ambient.g  ) &&
		*( ( DWORD * )&GD3D9.Device.State.Material.Ambient.b )  == *( ( DWORD * )&Material->Ambient.b  ) &&
		*( ( DWORD * )&GD3D9.Device.State.Material.Diffuse.a )  == *( ( DWORD * )&Material->Diffuse.a  ) &&
		*( ( DWORD * )&GD3D9.Device.State.Material.Diffuse.r )  == *( ( DWORD * )&Material->Diffuse.r  ) &&
		*( ( DWORD * )&GD3D9.Device.State.Material.Diffuse.g )  == *( ( DWORD * )&Material->Diffuse.g  ) &&
		*( ( DWORD * )&GD3D9.Device.State.Material.Diffuse.b )  == *( ( DWORD * )&Material->Diffuse.b  ) &&
		*( ( DWORD * )&GD3D9.Device.State.Material.Emissive.a ) == *( ( DWORD * )&Material->Emissive.a ) &&
		*( ( DWORD * )&GD3D9.Device.State.Material.Emissive.r ) == *( ( DWORD * )&Material->Emissive.r ) &&
		*( ( DWORD * )&GD3D9.Device.State.Material.Emissive.g ) == *( ( DWORD * )&Material->Emissive.g ) &&
		*( ( DWORD * )&GD3D9.Device.State.Material.Emissive.b ) == *( ( DWORD * )&Material->Emissive.b ) &&
		*( ( DWORD * )&GD3D9.Device.State.Material.Specular.a ) == *( ( DWORD * )&Material->Specular.a ) &&
		*( ( DWORD * )&GD3D9.Device.State.Material.Specular.r ) == *( ( DWORD * )&Material->Specular.r ) &&
		*( ( DWORD * )&GD3D9.Device.State.Material.Specular.g ) == *( ( DWORD * )&Material->Specular.g ) &&
		*( ( DWORD * )&GD3D9.Device.State.Material.Specular.b ) == *( ( DWORD * )&Material->Specular.b ) &&
		*( ( DWORD * )&GD3D9.Device.State.Material.Power )      == *( ( DWORD * )&Material->Power      ) )
		return 0 ;

	if( GD3D9.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
	{
		// �`��ҋ@���Ă���`�敨��`��
		DRAWSTOCKINFO
	}

	// �p�����[�^��ۑ�
	GD3D9.Device.State.Material = *Material ;

	// �o�[�e�b�N�X�V�F�[�_�[�ƃs�N�Z���V�F�[�_�[�ɂ����f����
//	if( GSYS.HardInfo.UseShader )
	{
		float Data[ 3 ][ 4 ] ;

		Data[ 0 ][ 0 ] = Material->Diffuse.r ;
		Data[ 0 ][ 1 ] = Material->Diffuse.g ;
		Data[ 0 ][ 2 ] = Material->Diffuse.b ;
		Data[ 0 ][ 3 ] = Material->Diffuse.a ;
		Data[ 1 ][ 0 ] = Material->Specular.r ;
		Data[ 1 ][ 1 ] = Material->Specular.g ;
		Data[ 1 ][ 2 ] = Material->Specular.b ;
		Data[ 1 ][ 3 ] = Material->Specular.a ;
		Data[ 2 ][ 0 ] = Material->Power ;
		Data[ 2 ][ 1 ] = 0.0f ;
		Data[ 2 ][ 2 ] = 0.0f ;
		Data[ 2 ][ 3 ] = 0.0f ;
		Graphics_D3D9_ShaderConstant_InfoSet_SetParam(
			&GD3D9.Device.Shader.ShaderConstantInfo,
			DX_SHADERCONSTANTTYPE_VS_FLOAT,
			DX_SHADERCONSTANTSET_LIB,
			DX_VS_CONSTF_MATERIAL_START,
			Data[ 0 ],
			DX_VS_CONSTF_MATERIAL_SIZE,
			TRUE
		) ;
		Graphics_D3D9_ShaderConstant_InfoSet_SetParam(
			&GD3D9.Device.Shader.ShaderConstantInfo,
			DX_SHADERCONSTANTTYPE_PS_FLOAT,
			DX_SHADERCONSTANTSET_LIB,
			DX_PS_CONSTF_MATERIAL_START,
			Data[ 0 ],
			DX_PS_CONSTF_MATERIAL_SIZE,
			TRUE
		) ;

		// �A���r�G���g�J���[�ƃG�~�b�V�u�J���[���|�����킹�����̂��X�V
		Graphics_D3D9_DeviceState_RefreshAmbientAndEmissiveParam() ;
	}

	return Direct3DDevice9_SetMaterial( Material ) == D_D3D_OK ? 0 : -1 ;
}


// �X�y�L�������g�p���邩�ǂ������Z�b�g
extern int  Graphics_D3D9_DeviceState_SetSpecularEnable( int UseFlag )
{
	if( GAPIWin.Direct3DDevice9Object == NULL ) return 0 ;

	// �����̃p�����[�^�Ɠ����ꍇ�͉������Ȃ�
	if( GD3D9.Device.State.UseSpecular == UseFlag && GD3D9.Device.DrawSetting.CancelSettingEqualCheck == FALSE ) return 0 ;

	if( GD3D9.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
	{
		// �`��ҋ@���Ă���`�敨��`��
		DRAWSTOCKINFO
	}

	// �p�����[�^��ۑ�
	GD3D9.Device.State.UseSpecular = UseFlag ;
	GD3D9.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

	return Direct3DDevice9_SetRenderState( D_D3DRS_SPECULARENABLE, ( DWORD )UseFlag ) == D_D3D_OK ? 0 : -1 ;
}

// ���_�̃f�B�t���[�Y�J���[���}�e���A���̃f�B�t���[�Y�J���[�Ƃ��Ďg�p���邩�ǂ�����ݒ肷��
extern int  Graphics_D3D9_DeviceState_SetDiffuseMaterialSource( int UseFlag )
{
	if( GAPIWin.Direct3DDevice9Object == NULL ) return 0 ;

	// �����̃p�����[�^�Ɠ����ꍇ�͉������Ȃ�
	if( GD3D9.Device.State.MaterialUseVertexDiffuseColor == UseFlag && GD3D9.Device.DrawSetting.CancelSettingEqualCheck == FALSE ) return 0 ;

	if( GD3D9.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
	{
		// �`��ҋ@���Ă���`�敨��`��
		DRAWSTOCKINFO
	}

	// �p�����[�^��ۑ�
	GD3D9.Device.State.MaterialUseVertexDiffuseColor = UseFlag ;
	GD3D9.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

	// �o�[�e�b�N�X�V�F�[�_�[�ɔ��f����
//	if( GSYS.HardInfo.UseShader )
	{
		float Data[ 4 ] ;

		Data[ 0 ] = GD3D9.Device.State.MaterialUseVertexDiffuseColor  ? 1.0f : 0.0f ;
		Data[ 1 ] = GD3D9.Device.State.MaterialUseVertexSpecularColor ? 1.0f : 0.0f ;
		Data[ 2 ] = 0.0f ;
		Data[ 3 ] = 0.0f ;
		Graphics_D3D9_ShaderConstant_InfoSet_SetParam(
			&GD3D9.Device.Shader.ShaderConstantInfo,
			DX_SHADERCONSTANTTYPE_VS_FLOAT,
			DX_SHADERCONSTANTSET_LIB,
			DX_VS_CONSTF_DIF_SPE_SOURCE,
			Data,
			1,
			TRUE
		) ;
	}

	return Direct3DDevice9_SetRenderState( D_D3DRS_DIFFUSEMATERIALSOURCE, UseFlag ? D_D3DMCS_COLOR1 : D_D3DMCS_MATERIAL ) == D_D3D_OK ? 0 : -1 ;
}

// ���_�̃X�y�L�����J���[���}�e���A���̃X�y�L�����J���[�Ƃ��Ďg�p���邩�ǂ�����ݒ肷��
extern int  Graphics_D3D9_DeviceState_SetSpecularMaterialSource( int UseFlag )
{
	if( GAPIWin.Direct3DDevice9Object == NULL ) return 0 ;

	// �����̃p�����[�^�Ɠ����ꍇ�͉������Ȃ�
	if( GD3D9.Device.State.MaterialUseVertexSpecularColor == UseFlag && GD3D9.Device.DrawSetting.CancelSettingEqualCheck == FALSE ) return 0 ;

	if( GD3D9.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
	{
		// �`��ҋ@���Ă���`�敨��`��
		DRAWSTOCKINFO
	}

	// �p�����[�^��ۑ�
	GD3D9.Device.State.MaterialUseVertexSpecularColor = UseFlag ;
	GD3D9.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

	// �o�[�e�b�N�X�V�F�[�_�[�ɔ��f����
//	if( GSYS.HardInfo.UseShader )
	{
		float Data[ 4 ] ;

		Data[ 0 ] = GD3D9.Device.State.MaterialUseVertexDiffuseColor  ? 1.0f : 0.0f ;
		Data[ 1 ] = GD3D9.Device.State.MaterialUseVertexSpecularColor ? 1.0f : 0.0f ;
		Data[ 2 ] = 0.0f ;
		Data[ 3 ] = 0.0f ;
		Graphics_D3D9_ShaderConstant_InfoSet_SetParam(
			&GD3D9.Device.Shader.ShaderConstantInfo,
			DX_SHADERCONSTANTTYPE_VS_FLOAT,
			DX_SHADERCONSTANTSET_LIB,
			DX_VS_CONSTF_DIF_SPE_SOURCE,
			Data,
			1,
			TRUE
		) ;
	}

	return Direct3DDevice9_SetRenderState( D_D3DRS_SPECULARMATERIALSOURCE, UseFlag ? D_D3DMCS_COLOR2 : D_D3DMCS_MATERIAL ) == D_D3D_OK ? 0 : -1 ;
}

// �J�����O�̗L�����Z�b�g 
extern int  Graphics_D3D9_DeviceState_SetCullMode( int State )
{
	if( GAPIWin.Direct3DDevice9Object == NULL ) return 0 ;

	// ���[�h�������������牽�������I��
	if( GD3D9.Device.DrawSetting.CancelSettingEqualCheck == FALSE && GD3D9.Device.State.CullMode == State ) return 0 ;
	
	if( GD3D9.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
	{
		// �`��ҋ@���Ă���`�敨��`��
		DRAWSTOCKINFO
	}

	// �J�����O�̐ݒ��ύX
	Direct3DDevice9_SetRenderState( D_D3DRS_CULLMODE, State == 2 ? D_D3DCULL_CW : ( State ? D_D3DCULL_CCW : D_D3DCULL_NONE ) ) == D_D3D_OK ? 0 : -1 ;
	GD3D9.Device.State.CullMode = State;
	GD3D9.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

	// �I��
	return 0;
}

// �A���r�G���g�J���[�̃Z�b�g
extern int  Graphics_D3D9_DeviceState_SetAmbient( unsigned int Color )
{
	if( GAPIWin.Direct3DDevice9Object == NULL ) return 0 ;

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �A���r�G���g�J���[��ۑ�����
	GD3D9.Device.State.GlobalAmbientLightColor.r = ( float )( ( Color >> 16 ) & 0xff ) / 255.0f ;
	GD3D9.Device.State.GlobalAmbientLightColor.g = ( float )( ( Color >>  8 ) & 0xff ) / 255.0f ;
	GD3D9.Device.State.GlobalAmbientLightColor.b = ( float )( ( Color >>  0 ) & 0xff ) / 255.0f ;
	GD3D9.Device.State.GlobalAmbientLightColor.a = ( float )( ( Color >> 24 ) & 0xff ) / 255.0f ;

	// �A���r�G���g���C�g�ƃG�~�b�V�u�J���[���|�����킹���p�����[�^���X�V����
	Graphics_D3D9_DeviceState_RefreshAmbientAndEmissiveParam() ;

	return Direct3DDevice9_SetRenderState( D_D3DRS_AMBIENT, Color ) == D_D3D_OK ? 0 : -1 ;
}

// �V�F�[�f�B���O���[�h�̃Z�b�g
extern int  Graphics_D3D9_DeviceState_SetShadeMode( int ShadeMode )
{
	if( GAPIWin.Direct3DDevice9Object == NULL ) return 0 ;

	if( GD3D9.Device.State.ShadeMode == ShadeMode && GD3D9.Device.DrawSetting.CancelSettingEqualCheck == FALSE ) return 0 ;

	GD3D9.Device.State.ShadeMode = ShadeMode ;
	GD3D9.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

	if( GD3D9.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
	{
		// �`��ҋ@���Ă���`�敨��`��
		DRAWSTOCKINFO
	}

	return Direct3DDevice9_SetRenderState( D_D3DRS_SHADEMODE, ( DWORD )ShadeMode ) == D_D3D_OK ? 0 : -1 ;
}

// �e�N�X�`���[�̃A�h���b�V���O���[�h���Z�b�g����
extern int  Graphics_D3D9_DeviceState_SetTextureAddress( int AddressMode, int Sampler )
{
	DWORD i ;
	HRESULT hr ;

	if( GAPIWin.Direct3DDevice9Object == NULL ) return 0 ;

	if( Sampler == -1 )
	{
		for( i = 0 ; i < USE_TEXTURESTAGE_NUM ; i ++ )
		{
			if( AddressMode != GD3D9.Device.State.TexAddressModeU[ i ] ||
				AddressMode != GD3D9.Device.State.TexAddressModeV[ i ] ||
				AddressMode != GD3D9.Device.State.TexAddressModeW[ i ] )
			{
				break ;
			}
		}

		if( i == USE_TEXTURESTAGE_NUM &&
			GD3D9.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
		{
			return 0 ;
		}

		// �`��ҋ@���Ă���`�敨��`��
		DRAWSTOCKINFO

		for( i = 0 ; i < USE_TEXTURESTAGE_NUM ; i ++ )
		{
			GD3D9.Device.State.TexAddressModeU[ i ] = AddressMode ;
			GD3D9.Device.State.TexAddressModeV[ i ] = AddressMode ;
			GD3D9.Device.State.TexAddressModeW[ i ] = AddressMode ;
		}

		GD3D9.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

		hr = Direct3DDevice9_SetSamplerState( 0, D_D3DSAMP_ADDRESSU, ( DWORD )AddressMode ) ;
		Direct3DDevice9_SetSamplerState( 0, D_D3DSAMP_ADDRESSV, ( DWORD )AddressMode ) ;
		Direct3DDevice9_SetSamplerState( 0, D_D3DSAMP_ADDRESSW, ( DWORD )AddressMode ) ;
		for( i = 1 ; i < USE_TEXTURESTAGE_NUM ; i ++ )
		{
			Direct3DDevice9_SetSamplerState( i, D_D3DSAMP_ADDRESSU, ( DWORD )AddressMode ) ;
			Direct3DDevice9_SetSamplerState( i, D_D3DSAMP_ADDRESSV, ( DWORD )AddressMode ) ;
			Direct3DDevice9_SetSamplerState( i, D_D3DSAMP_ADDRESSW, ( DWORD )AddressMode ) ;
		}
	}
	else
	{
		if( Sampler < 0 || Sampler >= USE_TEXTURESTAGE_NUM )
		{
			return -1 ;
		}

		if( AddressMode == GD3D9.Device.State.TexAddressModeU[ Sampler ] &&
			AddressMode == GD3D9.Device.State.TexAddressModeV[ Sampler ] &&
			AddressMode == GD3D9.Device.State.TexAddressModeW[ Sampler ] &&
			GD3D9.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
		{
			return 0 ;
		}

		// �`��ҋ@���Ă���`�敨��`��
		DRAWSTOCKINFO

		GD3D9.Device.State.TexAddressModeU[ Sampler ] = AddressMode ;
		GD3D9.Device.State.TexAddressModeV[ Sampler ] = AddressMode ;
		GD3D9.Device.State.TexAddressModeW[ Sampler ] = AddressMode ;

		GD3D9.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

		hr = Direct3DDevice9_SetSamplerState( ( DWORD )Sampler, D_D3DSAMP_ADDRESSU, ( DWORD )AddressMode ) ;
		Direct3DDevice9_SetSamplerState( ( DWORD )Sampler, D_D3DSAMP_ADDRESSV, ( DWORD )AddressMode ) ;
		Direct3DDevice9_SetSamplerState( ( DWORD )Sampler, D_D3DSAMP_ADDRESSW, ( DWORD )AddressMode ) ;
	}

	return hr == D_D3D_OK ? 0 : -1 ;
}

// �e�N�X�`���[�̃A�h���b�V���O���[�h���Z�b�g����
extern int  Graphics_D3D9_DeviceState_SetTextureAddressU( int AddressMode, int Sampler )
{
	DWORD i ;
	HRESULT hr ;

	if( GAPIWin.Direct3DDevice9Object == NULL ) return 0 ;

	if( Sampler == -1 )
	{
		for( i = 0 ; i < USE_TEXTURESTAGE_NUM ; i ++ )
		{
			if( AddressMode != GD3D9.Device.State.TexAddressModeU[ i ] )
			{
				break ;
			}
		}

		if( i == USE_TEXTURESTAGE_NUM &&
			GD3D9.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
		{
			return 0 ;
		}

		// �`��ҋ@���Ă���`�敨��`��
		DRAWSTOCKINFO

		for( i = 0 ; i < USE_TEXTURESTAGE_NUM ; i ++ )
		{
			GD3D9.Device.State.TexAddressModeU[ i ] = AddressMode ;
		}

		GD3D9.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

		hr = Direct3DDevice9_SetSamplerState( 0, D_D3DSAMP_ADDRESSU, ( DWORD )AddressMode ) ;
		for( i = 1 ; i < USE_TEXTURESTAGE_NUM ; i ++ )
		{
			Direct3DDevice9_SetSamplerState( i, D_D3DSAMP_ADDRESSU, ( DWORD )AddressMode ) ;
		}
	}
	else
	{
		if( Sampler < 0 || Sampler >= USE_TEXTURESTAGE_NUM )
		{
			return -1 ;
		}

		if( AddressMode == GD3D9.Device.State.TexAddressModeU[ Sampler ] &&
			GD3D9.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
		{
			return 0 ;
		}

		// �`��ҋ@���Ă���`�敨��`��
		DRAWSTOCKINFO

		GD3D9.Device.State.TexAddressModeU[ Sampler ] = AddressMode ;

		GD3D9.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

		hr = Direct3DDevice9_SetSamplerState( ( DWORD )Sampler, D_D3DSAMP_ADDRESSU, ( DWORD )AddressMode ) ;
	}

	return hr == D_D3D_OK ? 0 : -1 ;
}

// �e�N�X�`���[�̃A�h���b�V���O���[�h���Z�b�g����
extern int  Graphics_D3D9_DeviceState_SetTextureAddressV( int AddressMode, int Sampler )
{
	DWORD i ;
	HRESULT hr ;

	if( GAPIWin.Direct3DDevice9Object == NULL ) return 0 ;

	if( Sampler == -1 )
	{
		for( i = 0 ; i < USE_TEXTURESTAGE_NUM ; i ++ )
		{
			if( AddressMode != GD3D9.Device.State.TexAddressModeV[ i ] )
			{
				break ;
			}
		}

		if( i == USE_TEXTURESTAGE_NUM &&
			GD3D9.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
		{
			return 0 ;
		}

		// �`��ҋ@���Ă���`�敨��`��
		DRAWSTOCKINFO

		for( i = 0 ; i < USE_TEXTURESTAGE_NUM ; i ++ )
		{
			GD3D9.Device.State.TexAddressModeV[ i ] = AddressMode ;
		}

		GD3D9.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

		hr = Direct3DDevice9_SetSamplerState( 0, D_D3DSAMP_ADDRESSV, ( DWORD )AddressMode ) ;
		for( i = 1 ; i < USE_TEXTURESTAGE_NUM ; i ++ )
		{
			Direct3DDevice9_SetSamplerState( i, D_D3DSAMP_ADDRESSV, ( DWORD )AddressMode ) ;
		}
	}
	else
	{
		if( Sampler < 0 || Sampler >= USE_TEXTURESTAGE_NUM )
		{
			return -1 ;
		}

		if( AddressMode == GD3D9.Device.State.TexAddressModeV[ Sampler ] &&
			GD3D9.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
		{
			return 0 ;
		}

		// �`��ҋ@���Ă���`�敨��`��
		DRAWSTOCKINFO

		GD3D9.Device.State.TexAddressModeV[ Sampler ] = AddressMode ;

		GD3D9.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

		hr = Direct3DDevice9_SetSamplerState( ( DWORD )Sampler, D_D3DSAMP_ADDRESSV, ( DWORD )AddressMode ) ;
	}

	return hr == D_D3D_OK ? 0 : -1 ;
}

// �e�N�X�`���[�̃A�h���b�V���O���[�h���Z�b�g����
extern int  Graphics_D3D9_DeviceState_SetTextureAddressW( int AddressMode, int Sampler )
{
	DWORD i ;
	HRESULT hr ;

	if( GAPIWin.Direct3DDevice9Object == NULL ) return 0 ;

	if( Sampler == -1 )
	{
		for( i = 0 ; i < USE_TEXTURESTAGE_NUM ; i ++ )
		{
			if( AddressMode != GD3D9.Device.State.TexAddressModeW[ i ] )
			{
				break ;
			}
		}

		if( i == USE_TEXTURESTAGE_NUM &&
			GD3D9.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
		{
			return 0 ;
		}

		// �`��ҋ@���Ă���`�敨��`��
		DRAWSTOCKINFO

		for( i = 0 ; i < USE_TEXTURESTAGE_NUM ; i ++ )
		{
			GD3D9.Device.State.TexAddressModeW[ i ] = AddressMode ;
		}

		GD3D9.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

		hr = Direct3DDevice9_SetSamplerState( 0, D_D3DSAMP_ADDRESSW, ( DWORD )AddressMode ) ;
		for( i = 1 ; i < USE_TEXTURESTAGE_NUM ; i ++ )
		{
			Direct3DDevice9_SetSamplerState( i, D_D3DSAMP_ADDRESSW, ( DWORD )AddressMode ) ;
		}
	}
	else
	{
		if( Sampler < 0 || Sampler >= USE_TEXTURESTAGE_NUM )
		{
			return -1 ;
		}

		if( AddressMode == GD3D9.Device.State.TexAddressModeW[ Sampler ] &&
			GD3D9.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
		{
			return 0 ;
		}

		// �`��ҋ@���Ă���`�敨��`��
		DRAWSTOCKINFO

		GD3D9.Device.State.TexAddressModeW[ Sampler ] = AddressMode ;

		GD3D9.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

		hr = Direct3DDevice9_SetSamplerState( ( DWORD )Sampler, D_D3DSAMP_ADDRESSW, ( DWORD )AddressMode ) ;
	}

	return hr == D_D3D_OK ? 0 : -1 ;
}

// �e�N�X�`���[�̃A�h���b�V���O���[�h���Z�b�g����
extern int Graphics_D3D9_DeviceState_SetTextureAddressUVW( int AddressModeU, int AddressModeV, int AddressModeW, int Sampler )
{
	Graphics_D3D9_DeviceState_SetTextureAddressU( AddressModeU, Sampler ) ;
	Graphics_D3D9_DeviceState_SetTextureAddressV( AddressModeV, Sampler ) ;
	return Graphics_D3D9_DeviceState_SetTextureAddressW( AddressModeW, Sampler ) ;
}

// �e�N�X�`�����W�ϊ��s����Z�b�g����
extern int  Graphics_D3D9_DeviceState_SetTextureAddressTransformMatrix( int Use, MATRIX *Matrix, int Samplar )
{
	if( Samplar == -1 )
	{
		// �`��ҋ@���Ă���`�敨��`��
		DRAWSTOCKINFO

		if( Use == TRUE )
		{
			Direct3DDevice9_SetTextureStageState( 0, D_D3DTSS_TEXTURETRANSFORMFLAGS, D_D3DTTFF_COUNT3 ) ;
			Direct3DDevice9_SetTextureStageState( 1, D_D3DTSS_TEXTURETRANSFORMFLAGS, D_D3DTTFF_COUNT3 ) ;
			Direct3DDevice9_SetTextureStageState( 2, D_D3DTSS_TEXTURETRANSFORMFLAGS, D_D3DTTFF_COUNT3 ) ;
			Direct3DDevice9_SetTextureStageState( 3, D_D3DTSS_TEXTURETRANSFORMFLAGS, D_D3DTTFF_COUNT3 ) ;
			Direct3DDevice9_SetTransform( D_D3DTS_TEXTURE0, ( D_D3DMATRIX * )Matrix ) ;
			Direct3DDevice9_SetTransform( D_D3DTS_TEXTURE1, ( D_D3DMATRIX * )Matrix ) ;
			Direct3DDevice9_SetTransform( D_D3DTS_TEXTURE2, ( D_D3DMATRIX * )Matrix ) ;
			Direct3DDevice9_SetTransform( D_D3DTS_TEXTURE3, ( D_D3DMATRIX * )Matrix ) ;
		}
		else
		{
			Direct3DDevice9_SetTextureStageState( 0, D_D3DTSS_TEXTURETRANSFORMFLAGS, D_D3DTTFF_DISABLE ) ;
			Direct3DDevice9_SetTextureStageState( 1, D_D3DTSS_TEXTURETRANSFORMFLAGS, D_D3DTTFF_DISABLE ) ;
			Direct3DDevice9_SetTextureStageState( 2, D_D3DTSS_TEXTURETRANSFORMFLAGS, D_D3DTTFF_DISABLE ) ;
			Direct3DDevice9_SetTextureStageState( 3, D_D3DTSS_TEXTURETRANSFORMFLAGS, D_D3DTTFF_DISABLE ) ;
		}
		GD3D9.Device.State.TextureTransformUse[ 0 ] = Use ;
		GD3D9.Device.State.TextureTransformUse[ 1 ] = Use ;
		GD3D9.Device.State.TextureTransformUse[ 2 ] = Use ;
		GD3D9.Device.State.TextureTransformUse[ 3 ] = Use ;
		GD3D9.Device.State.TextureTransformMatrix[ 0 ] = *Matrix ;
		GD3D9.Device.State.TextureTransformMatrix[ 1 ] = *Matrix ;
		GD3D9.Device.State.TextureTransformMatrix[ 2 ] = *Matrix ;
		GD3D9.Device.State.TextureTransformMatrix[ 3 ] = *Matrix ;
	}
	else
	{
		if( Samplar >= GD3D9.Device.Caps.MaxTextureBlendStages || GD3D9.Device.State.TextureTransformUse[ Samplar ] == FALSE && Use == FALSE )
			return 0 ;

		// �`��ҋ@���Ă���`�敨��`��
		DRAWSTOCKINFO

		if( Use == TRUE )
		{
			if( GD3D9.Device.State.TextureTransformUse[ Samplar ] == FALSE )
			{
				Direct3DDevice9_SetTextureStageState( ( DWORD )Samplar, D_D3DTSS_TEXTURETRANSFORMFLAGS, D_D3DTTFF_COUNT3 ) ;
			}
			Direct3DDevice9_SetTransform( ( D_D3DTRANSFORMSTATETYPE )( D_D3DTS_TEXTURE0 + Samplar ), ( D_D3DMATRIX * )Matrix ) ;
		}
		else
		{
			Direct3DDevice9_SetTextureStageState( ( DWORD )Samplar, D_D3DTSS_TEXTURETRANSFORMFLAGS, D_D3DTTFF_DISABLE ) ;
		}
		GD3D9.Device.State.TextureTransformUse[ Samplar ] = Use ;
		GD3D9.Device.State.TextureTransformMatrix[ Samplar ] = *Matrix ;
	}
	return 0 ;
}

// �t�H�O��L���ɂ��邩�ǂ�����ݒ肷��( TRUE:�L��  FALSE:���� )
extern int  Graphics_D3D9_DeviceState_SetFogEnable( int Flag )
{
	if( GAPIWin.Direct3DDevice9Object == NULL ) return 0 ;

	if( Flag == GD3D9.Device.State.FogEnable && GD3D9.Device.DrawSetting.CancelSettingEqualCheck == FALSE ) return 0 ;

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	GD3D9.Device.State.FogEnable = Flag ;
	GD3D9.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

	// �g�p����V�F�[�_�[�̃C���f�b�N�X���X�V����
	Graphics_D3D9_Shader_RefreshUseIndex() ;

	{
		BOOL BData ;
		BData = Flag == 0 ? 0 : 1 ;
		Graphics_D3D9_ShaderConstant_InfoSet_SetParam(
			&GD3D9.Device.Shader.ShaderConstantInfo,
			DX_SHADERCONSTANTTYPE_VS_BOOL,
			DX_SHADERCONSTANTSET_LIB,
			DX_VS_CONSTB_FOG_USE,
			&BData,
			1,
			TRUE
		) ;
	}

	return Direct3DDevice9_SetRenderState( D_D3DRS_FOGENABLE, ( DWORD )Flag ) == D_D3D_OK ? 0 : -1 ;
}

// �t�H�O���[�h��ݒ肷��
extern int  Graphics_D3D9_DeviceState_SetFogVertexMode( int Mode /* DX_FOGMODE_NONE �� */ )
{
	if( GAPIWin.Direct3DDevice9Object == NULL ) return 0 ;

	if( Mode == GD3D9.Device.State.FogMode && GD3D9.Device.DrawSetting.CancelSettingEqualCheck == FALSE ) return 0 ;

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �o�[�e�b�N�X�V�F�[�_�[�p�̃p�����[�^�Z�b�g
	if( GSYS.HardInfo.UseShader )
	{
		BOOL BData[ 4 ] ;

		BData[ 0 ] = Mode == DX_FOGMODE_LINEAR ;
		BData[ 1 ] = Mode == DX_FOGMODE_EXP ;
		BData[ 2 ] = Mode == DX_FOGMODE_EXP2 ;
		BData[ 3 ] = Mode != DX_FOGMODE_NONE ;
		Graphics_D3D9_ShaderConstant_InfoSet_SetParam(
			&GD3D9.Device.Shader.ShaderConstantInfo,
			DX_SHADERCONSTANTTYPE_VS_BOOL,
			DX_SHADERCONSTANTSET_LIB,
			DX_VS_CONSTB_FOG_LINEAR,
			BData,
			4,
			TRUE
		) ;
	}

	GD3D9.Device.State.FogMode = Mode ;

	// �g�p����V�F�[�_�[�̃C���f�b�N�X���X�V����
	Graphics_D3D9_Shader_RefreshUseIndex() ;

	return Direct3DDevice9_SetRenderState( D_D3DRS_FOGVERTEXMODE, ( DWORD )Mode ) == D_D3D_OK ? 0 : -1 ;
}

// �t�H�O�J���[��ύX����
extern int  Graphics_D3D9_DeviceState_SetFogColor( unsigned int Color )
{
	float Data[ 4 ] ;

	if( GAPIWin.Direct3DDevice9Object == NULL ) return 0 ;

	if( Color == GD3D9.Device.State.FogColor && GD3D9.Device.DrawSetting.CancelSettingEqualCheck == FALSE ) return 0 ;

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

//	if( GSYS.HardInfo.UseShader )
	{
		Data[ 0 ] = ( ( Color >> 16 ) & 0xff ) / 255.0f ;
		Data[ 1 ] = ( ( Color >>  8 ) & 0xff ) / 255.0f ;
		Data[ 2 ] = ( ( Color >>  0 ) & 0xff ) / 255.0f ;
		Data[ 3 ] = 1.0f ;
		Graphics_D3D9_ShaderConstant_InfoSet_SetParam(
			&GD3D9.Device.Shader.ShaderConstantInfo,
			DX_SHADERCONSTANTTYPE_PS_FLOAT,
			DX_SHADERCONSTANTSET_LIB,
			DX_PS_CONSTF_FOG_COLOR,
			Data,
			1,
			TRUE
		) ;
	}

	GD3D9.Device.State.FogColor = Color ;
	return Direct3DDevice9_SetRenderState( D_D3DRS_FOGCOLOR, Color ) == D_D3D_OK ? 0 : -1 ;
}

// �t�H�O���n�܂鋗���ƏI�����鋗����ݒ肷��( 0.0f �` 1.0f )
extern int  Graphics_D3D9_DeviceState_SetFogStartEnd( float Start, float End )
{
	float Data[ 4 ] ;

	if( GAPIWin.Direct3DDevice9Object == NULL ) return 0 ;

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	if( Start != GD3D9.Device.State.FogStart || GD3D9.Device.DrawSetting.CancelSettingEqualCheck == TRUE )
	{
		GD3D9.Device.State.FogStart = Start ;
		Direct3DDevice9_SetRenderState( D_D3DRS_FOGSTART, *(( DWORD *)&Start) ) ;

		// �o�[�e�b�N�X�V�F�[�_�[�p�̃p�����[�^�Z�b�g
//		if( GSYS.HardInfo.UseShader )
		{
			Data[ 0 ] = GD3D9.Device.State.FogEnd / ( GD3D9.Device.State.FogEnd - GD3D9.Device.State.FogStart ) ;
			Data[ 1 ] = -1.0f / ( GD3D9.Device.State.FogEnd - GD3D9.Device.State.FogStart ) ;
			Data[ 2 ] = GD3D9.Device.State.FogDensity ;
			Data[ 3 ] = 2.71828183f ;
			Graphics_D3D9_ShaderConstant_InfoSet_SetParam(
				&GD3D9.Device.Shader.ShaderConstantInfo,
				DX_SHADERCONSTANTTYPE_VS_FLOAT,
				DX_SHADERCONSTANTSET_LIB,
				DX_VS_CONSTF_FOG,
				Data,
				1,
				TRUE
			) ;
		}
	}

	if( End != GD3D9.Device.State.FogEnd || GD3D9.Device.DrawSetting.CancelSettingEqualCheck == TRUE )
	{
		GD3D9.Device.State.FogEnd = End ;
		Direct3DDevice9_SetRenderState( D_D3DRS_FOGEND, *(( DWORD *)&End) ) ;

		// �o�[�e�b�N�X�V�F�[�_�[�p�̃p�����[�^�Z�b�g
//		if( GSYS.HardInfo.UseShader )
		{
			Data[ 0 ] = GD3D9.Device.State.FogEnd / ( GD3D9.Device.State.FogEnd - GD3D9.Device.State.FogStart ) ;
			Data[ 1 ] = -1.0f / ( GD3D9.Device.State.FogEnd - GD3D9.Device.State.FogStart ) ;
			Data[ 2 ] = GD3D9.Device.State.FogDensity ;
			Data[ 3 ] = 2.71828183f ;
			Graphics_D3D9_ShaderConstant_InfoSet_SetParam(
				&GD3D9.Device.Shader.ShaderConstantInfo,
				DX_SHADERCONSTANTTYPE_VS_FLOAT,
				DX_SHADERCONSTANTSET_LIB,
				DX_VS_CONSTF_FOG,
				Data,
				1,
				TRUE
			) ;
		}
	}

	// �I��
	return 0 ;
}

// �t�H�O�̖��x��ݒ肷��( 0.0f �` 1.0f )
extern int  Graphics_D3D9_DeviceState_SetFogDensity( float Density )
{
	float Data[ 4 ] ;
	if( GAPIWin.Direct3DDevice9Object == NULL ) return 0 ;

	if( Density == GD3D9.Device.State.FogDensity && GD3D9.Device.DrawSetting.CancelSettingEqualCheck == FALSE ) return 0 ;

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	GD3D9.Device.State.FogDensity = Density ;

	// �o�[�e�b�N�X�V�F�[�_�[�p�̃p�����[�^�Z�b�g
//	if( GSYS.HardInfo.UseShader )
	{
		Data[ 0 ] = GD3D9.Device.State.FogEnd / ( GD3D9.Device.State.FogEnd - GD3D9.Device.State.FogStart ) ;
		Data[ 1 ] = -1.0f / ( GD3D9.Device.State.FogEnd - GD3D9.Device.State.FogStart ) ;
		Data[ 2 ] = GD3D9.Device.State.FogDensity ;
		Data[ 3 ] = 2.71828183f ;
		Graphics_D3D9_ShaderConstant_InfoSet_SetParam(
			&GD3D9.Device.Shader.ShaderConstantInfo,
			DX_SHADERCONSTANTTYPE_VS_FLOAT,
			DX_SHADERCONSTANTSET_LIB,
			DX_VS_CONSTF_FOG,
			Data,
			1,
			TRUE
		) ;
	}

	return Direct3DDevice9_SetRenderState( D_D3DRS_FOGDENSITY, *(( DWORD *)&Density) ) == D_D3D_OK ? 0 : -1 ;
}

// ���C�g�̗L���t���O���Z�b�g����
extern int  Graphics_D3D9_DeviceState_SetLighting( int Flag )
{
	if( GAPIWin.Direct3DDevice9Object == NULL ) return 0 ;

	if( Flag == GD3D9.Device.State.Lighting && GD3D9.Device.DrawSetting.CancelSettingEqualCheck == FALSE ) return 0 ;

	GD3D9.Device.State.Lighting = Flag ;
	GD3D9.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

	Graphics_D3D9_Shader_RefreshUseIndex() ;

	return Direct3DDevice9_SetRenderState( D_D3DRS_LIGHTING, ( DWORD )Flag ) == D_D3D_OK ? 0 : -1 ;
}

// �ő�ٕ������Z�b�g����
extern	int		Graphics_D3D9_DeviceState_SetMaxAnisotropy( int MaxAnisotropy, int Sampler )
{
	if( GAPIWin.Direct3DDevice9Object == NULL ) return 0 ;

	if( Sampler >= 0 )
	{
		if( Sampler < 0 || Sampler >= USE_TEXTURESTAGE_NUM ) return 0 ;

		if( GD3D9.Device.State.MaxAnisotropyDim[ Sampler ] == MaxAnisotropy && GD3D9.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
		{
			return 0 ;
		}

		Direct3DDevice9_SetSamplerState( ( DWORD )Sampler, D_D3DSAMP_MAXANISOTROPY, ( DWORD )MaxAnisotropy ) ;
		GD3D9.Device.State.MaxAnisotropyDim[ Sampler ] = MaxAnisotropy ;

		GD3D9.Device.State.MaxAnisotropy = -1 ;
	}
	else
	{
		int i, num ;

		if( GD3D9.Device.State.MaxAnisotropy == MaxAnisotropy && GD3D9.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
		{
			return 0 ;
		}

		// �l�̕␳
		if( MaxAnisotropy < 1 )
		{
			MaxAnisotropy = 1 ;
		}

		// �`��ҋ@���Ă���`�敨��`��
		DRAWSTOCKINFO

		// �����_�����O�ݒ��ύX����
		num = GD3D9.Device.State.BlendInfo.UseTextureStageNum ;
		if( num < 4 ) num = 4 ;
		for( i = 0 ; i < num ; i ++ )
		{
			if( GD3D9.Device.State.MaxAnisotropyDim[ i ] != MaxAnisotropy )
			{
				Direct3DDevice9_SetSamplerState( ( DWORD )i, D_D3DSAMP_MAXANISOTROPY, ( DWORD )MaxAnisotropy ) ;
				GD3D9.Device.State.MaxAnisotropyDim[ i ] = MaxAnisotropy ;
			}
		}

		GD3D9.Device.State.MaxAnisotropy = MaxAnisotropy ;
	}

	GD3D9.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

	// �I��
	return 0 ;

}

// �r���[�|�[�g���Z�b�g����
extern int Graphics_D3D9_DeviceState_SetViewport( D_D3DVIEWPORT9 *Viewport )
{
	HRESULT hr ;

	if( GAPIWin.Direct3DDevice9Object == NULL ) return -1 ;
	if( Viewport->X      == GD3D9.Device.State.Viewport.X      &&
		Viewport->Y      == GD3D9.Device.State.Viewport.Y      &&
		Viewport->Width  == GD3D9.Device.State.Viewport.Width  &&
		Viewport->Height == GD3D9.Device.State.Viewport.Height &&
		GD3D9.Device.DrawSetting.CancelSettingEqualCheck == FALSE ) return 0 ;

	// �����o��
	if( GD3D9.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
	{
		// �`��ҋ@���Ă���`�敨��`��
		DRAWSTOCKINFO
	}

	// �`����I�������Ă���
	Graphics_D3D9_EndScene() ;

	// �r���[�|�[�g�̃Z�b�e�B���O
	hr = Direct3DDevice9_SetViewport( Viewport ) ;
	if( hr != D_D3D_OK )
		return DXST_ERRORLOGFMT_ADDUTF16LE(( "\xd3\x30\xe5\x30\xfc\x30\xdd\x30\xfc\x30\xc8\x30\x6e\x30\xbb\x30\xc3\x30\xc8\x30\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x43\x00\x6f\x00\x64\x00\x65\x00\x20\x00\x25\x00\x78\x00\x0a\x00\x00"/*@ L"�r���[�|�[�g�̃Z�b�g�Ɏ��s���܂��� ErrorCode %x\n" @*/, hr ));

	// �r���[�|�[�g�̏��̕ۑ�
	GD3D9.Device.State.Viewport = *Viewport ;

	// �I��
	return 0 ;
}

// �r���[�|�[�g���Z�b�g����( �ȈՔ� )
extern int Graphics_D3D9_DeviceState_SetViewportEasy( int x1, int y1, int x2, int y2 )
{
	D_D3DVIEWPORT9 Viewport ;

	Viewport.X      = ( DWORD )x1 ;
	Viewport.Y      = ( DWORD )y1 ;
	Viewport.Width  = ( DWORD )( x2 - x1 ) ;
	Viewport.Height = ( DWORD )( y2 - y1 ) ;
	Viewport.MinZ   = 0.0f ;
	Viewport.MaxZ   = 1.0f ;

	return Graphics_D3D9_DeviceState_SetViewport( &Viewport ) ;
}

// �V�U�[��`��ݒ肷��
extern int Graphics_D3D9_DeviceState_SetScissorRect( RECT *Rect )
{
	if( GAPIWin.Direct3DDevice9Object == NULL ) return -1 ;
	if( Rect->left   == GD3D9.Device.State.ScissorRect.left   &&
		Rect->right  == GD3D9.Device.State.ScissorRect.right  &&
		Rect->top    == GD3D9.Device.State.ScissorRect.top    &&
		Rect->bottom == GD3D9.Device.State.ScissorRect.bottom &&
		GD3D9.Device.DrawSetting.CancelSettingEqualCheck == FALSE ) return 0 ;

	if( GD3D9.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
	{
		// �`��ҋ@���Ă���`�敨��`��
		DRAWSTOCKINFO
	}

	// �`����I�������Ă���
	Graphics_D3D9_EndScene() ;

	// �V�U�[��`�̃Z�b�e�B���O
	Direct3DDevice9_SetScissorRect( Rect ) ;

	// �V�U�[��`�̕ۑ�
	GD3D9.Device.State.ScissorRect = *Rect ;

	// �I��
	return 0 ;
}

// �`�惂�[�h�̃Z�b�g
extern int Graphics_D3D9_DeviceState_SetDrawMode( int DrawMode )
{
	DWORD i, num ;

	if( GAPIWin.Direct3DDevice9Object == NULL ) return -1 ;
	if( GD3D9.Device.State.DrawMode == DrawMode && GD3D9.Device.DrawSetting.CancelSettingEqualCheck == FALSE ) return 0 ;

	if( GD3D9.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
	{
		// �`��ҋ@���Ă���`�敨��`��
		DRAWSTOCKINFO
	}

	// �����_�����O�ݒ��ύX����
	num = ( DWORD )GD3D9.Device.State.BlendInfo.UseTextureStageNum ;
	if( num < USE_TEXTURESTAGE_NUM ) num = USE_TEXTURESTAGE_NUM ;
	switch( DrawMode )
	{
	case DX_DRAWMODE_BILINEAR :
		// �o�C���j�A�`��
		for( i = 0 ; i < num ; i ++ )
		{
			if( GD3D9.Device.State.MagFilter[ i ] != D_D3DTEXF_LINEAR || GD3D9.Device.DrawSetting.CancelSettingEqualCheck )
			{
				Direct3DDevice9_SetSamplerState( i, D_D3DSAMP_MAGFILTER, D_D3DTEXF_LINEAR ) ;
				GD3D9.Device.State.MagFilter[ i ] = D_D3DTEXF_LINEAR ;
			}
			if( GD3D9.Device.State.MinFilter[ i ] != D_D3DTEXF_LINEAR || GD3D9.Device.DrawSetting.CancelSettingEqualCheck )
			{
				Direct3DDevice9_SetSamplerState( i, D_D3DSAMP_MINFILTER, D_D3DTEXF_LINEAR ) ;
				GD3D9.Device.State.MinFilter[ i ] = D_D3DTEXF_LINEAR ;
			}
			if( GD3D9.Device.State.MipFilter[ i ] != D_D3DTEXF_LINEAR || GD3D9.Device.DrawSetting.CancelSettingEqualCheck )
			{
				Direct3DDevice9_SetSamplerState( i, D_D3DSAMP_MIPFILTER, D_D3DTEXF_NONE ) ;
				GD3D9.Device.State.MipFilter[ i ] = D_D3DTEXF_NONE ;
			}
		}
		break ;

	case DX_DRAWMODE_NEAREST :
		// ��A���X�g�l�C�o�[
		for( i = 0 ; i < num ; i ++ )
		{
			if( GD3D9.Device.State.MagFilter[ i ] != D_D3DTEXF_POINT || GD3D9.Device.DrawSetting.CancelSettingEqualCheck )
			{
				Direct3DDevice9_SetSamplerState( i, D_D3DSAMP_MAGFILTER, D_D3DTEXF_POINT ) ;
				GD3D9.Device.State.MagFilter[ i ] = D_D3DTEXF_POINT ;
			}
			if( GD3D9.Device.State.MinFilter[ i ] != D_D3DTEXF_POINT || GD3D9.Device.DrawSetting.CancelSettingEqualCheck )
			{
				Direct3DDevice9_SetSamplerState( i, D_D3DSAMP_MINFILTER, D_D3DTEXF_POINT ) ;
				GD3D9.Device.State.MinFilter[ i ] = D_D3DTEXF_POINT ;
			}
			if( GD3D9.Device.State.MipFilter[ i ] != D_D3DTEXF_POINT || GD3D9.Device.DrawSetting.CancelSettingEqualCheck )
			{
				Direct3DDevice9_SetSamplerState( i, D_D3DSAMP_MIPFILTER, D_D3DTEXF_NONE ) ;
				GD3D9.Device.State.MipFilter[ i ] = D_D3DTEXF_NONE ;
			}
		}
		break ;

	case DX_DRAWMODE_ANISOTROPIC :
		// �ٕ����t�B���^�����O
		for( i = 0 ; i < num ; i ++ )
		{
			if( GD3D9.Device.State.MagFilter[ i ] != D_D3DTEXF_LINEAR || GD3D9.Device.DrawSetting.CancelSettingEqualCheck )
			{
				Direct3DDevice9_SetSamplerState( i, D_D3DSAMP_MAGFILTER, D_D3DTEXF_LINEAR ) ;
				GD3D9.Device.State.MagFilter[ i ] = D_D3DTEXF_LINEAR ;
			}
			if( GD3D9.Device.State.MinFilter[ i ] != D_D3DTEXF_ANISOTROPIC || GD3D9.Device.DrawSetting.CancelSettingEqualCheck )
			{
				Direct3DDevice9_SetSamplerState( i, D_D3DSAMP_MINFILTER, D_D3DTEXF_ANISOTROPIC ) ;
				GD3D9.Device.State.MinFilter[ i ] = D_D3DTEXF_ANISOTROPIC ;
			}
			if( GD3D9.Device.State.MipFilter[ i ] != D_D3DTEXF_LINEAR || GD3D9.Device.DrawSetting.CancelSettingEqualCheck )
			{
				Direct3DDevice9_SetSamplerState( i, D_D3DSAMP_MIPFILTER, D_D3DTEXF_LINEAR ) ;
				GD3D9.Device.State.MipFilter[ i ] = D_D3DTEXF_LINEAR ;
			}
		}
		break ;

	default : return -1 ;
	}
	GD3D9.Device.State.DrawMode                 = DrawMode ;
	GD3D9.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

	// �I��
	return 0 ;
}

// �`��Ώۂ̕ύX
extern	int		Graphics_D3D9_DeviceState_SetRenderTarget( D_IDirect3DSurface9 *TargetSurface, int TargetIndex )
{
	D_D3DSURFACE_DESC SurfaceDesc ;
	HRESULT hr ;

	if( GAPIWin.Direct3DDevice9Object == NULL ) return -1 ;

	if( TargetIndex < 0 || TargetIndex >= DX_RENDERTARGET_COUNT ) return -1 ;

	if( GD3D9.Device.State.TargetSurface[ TargetIndex ] == TargetSurface && GD3D9.Device.DrawSetting.CancelSettingEqualCheck == FALSE ) return 0 ;

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �����_�����O�^�[�Q�b�g�̕ύX
	hr = Direct3DDevice9_SetRenderTarget( ( DWORD )TargetIndex, TargetSurface ) ;

	// �����_�����O�^�[�Q�b�g�̕ۑ�
	GD3D9.Device.State.TargetSurface[ TargetIndex ] = TargetSurface ;

	// �r���[�|�[�g�̐ݒ�l���X�V
	if( TargetIndex == 0 )
	{
		Direct3DSurface9_GetDesc( TargetSurface, &SurfaceDesc ) ; 
		Graphics_D3D9_DeviceState_SetViewportEasy( 0, 0, ( int )SurfaceDesc.Width, ( int )SurfaceDesc.Height ) ;
	}

	// �I��
	return  0 ;
}

// �Œ�@�\�p�C�v���C���݊������p�̒��_�錾�ƒ��_�V�F�[�_�[�̐ݒ���s��
static void Graphics_Hardware_D3D9_SetFVF_UseTable( int FVFDecl /* VERTEXFVF_DECL_2D_USER�� */ )
{
#ifndef DX_NON_NORMAL_DRAW_SHADER
//	if( GSYS.HardInfo.UseShader == FALSE || GD3D9.NormalDraw_UseVertexShader == FALSE )
//	{
		Graphics_D3D9_DeviceState_SetFVF( ( int )D3DDev_VertexFVF[ FVFDecl ] ) ;
//	}
//	else
//	{
//		if( D3DDev_VertexDeclaration_Is3D[ FVFDecl ] )
//		{
//			Graphics_D3D9_DeviceState_SetVertexShader( GD3D9.Device.Shader._2D.Base3DVertexShader, TRUE ) ;
//		}
//		else
//		{
//			Graphics_D3D9_DeviceState_SetVertexShader( GD3D9.Device.Shader._2D.Base2DVertexShader, TRUE ) ;
//		}
//
//		Graphics_D3D9_DeviceState_SetVertexDeclaration( GD3D9.Device.VertexDeclaration.Base2DDeclaration[ FVFDecl ] ) ;
//	}
#else  // DX_NON_NORMAL_DRAW_SHADER

	Graphics_D3D9_DeviceState_SetFVF( D3DDev_VertexFVF[ FVFDecl ] ) ;

#endif // DX_NON_NORMAL_DRAW_SHADER
}

// �g�p����FVF��ύX����
extern	int		Graphics_D3D9_DeviceState_SetFVF( int FVF )
{
	if( GAPIWin.Direct3DDevice9Object == NULL ) return -1 ;

	Graphics_D3D9_DeviceState_SetVertexDeclaration( NULL ) ;

	if( GD3D9.Device.State.SetFVF != ( DWORD )FVF || GD3D9.Device.DrawSetting.CancelSettingEqualCheck )
	{
		Direct3DDevice9_SetFVF( ( DWORD )FVF ) ;
		GD3D9.Device.State.SetFVF = ( DWORD )FVF ;
	}

	// �I��
	return 0 ;
}

// �g�p���钸�_�V�F�[�_�錾��ύX����
extern	int		Graphics_D3D9_DeviceState_SetVertexDeclaration( D_IDirect3DVertexDeclaration9 *VertexDeclaration )
{
	if( GAPIWin.Direct3DDevice9Object == NULL ) return -1 ;

	if( GD3D9.Device.State.SetVertexDeclaration != VertexDeclaration || GD3D9.Device.DrawSetting.CancelSettingEqualCheck )
	{
		Direct3DDevice9_SetVertexDeclaration( VertexDeclaration ) ;
		GD3D9.Device.State.SetVertexDeclaration = VertexDeclaration ;
	}

	GD3D9.Device.State.SetFVF = 0 ;
	GD3D9.Device.DrawSetting.DrawPrepAlwaysFlag    = TRUE ;

	// �I��
	return 0 ;
}

#ifndef DX_NON_MODEL

// �w��̃��f���p�̒��_�V�F�[�_�錾���Z�b�g����
extern	int		Graphics_D3D9_DeviceState_SetMV1VertexDeclaration( int BumpMap, int SkinMesh, int UVNum )
{
	return Graphics_D3D9_DeviceState_SetVertexDeclaration( GD3D9.Device.VertexDeclaration.MV1_VertexDeclaration[ BumpMap ][ SkinMesh ][ UVNum ] ) ;
}

#endif // DX_NON_MODEL

// �w��̒��_�V�F�[�_�[���f�o�C�X�ɃZ�b�g����
extern int Graphics_D3D9_DeviceState_SetVertexShaderToHandle( int ShaderHandle )
{
	SHADERHANDLEDATA *VertexShader ;

	// �n���h���`�F�b�N
	if( SHADERCHK( ShaderHandle, VertexShader ) )
		return -1 ;
	if( VertexShader->IsVertexShader == FALSE )
		return -1 ;

	// �V�F�[�_�[�̃Z�b�g
	Graphics_D3D9_DeviceState_SetVertexShader( VertexShader->PF->D3D9.VertexShader ) ;

	// ����I��
	return 0 ;
}

// �g�p���钸�_�V�F�[�_�[��ύX����
extern	int		Graphics_D3D9_DeviceState_SetVertexShader( D_IDirect3DVertexShader9 *VertexShader, int NormalVertexShader )
{
	if( GD3D9.Device.State.SetVertexShader == VertexShader && GD3D9.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
	{
		return 0 ;
	}

	if( GAPIWin.Direct3DDevice9Object == NULL ) return -1 ;

	Direct3DDevice9_SetVertexShader( VertexShader ) ;
	GD3D9.Device.State.SetVertexShader          = VertexShader ;
	GD3D9.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;
	GD3D9.Device.State.SetNormalVertexShader    = NormalVertexShader ;
	if( NormalVertexShader == FALSE )
	{
		GD3D9.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;
	}

	// �I��
	return 0 ;
}


// ���_�V�F�[�_�[�̎g�p���~�߂�( �Œ�@�\�p�C�v���C�����g�p�����Ԃɂ��� )
extern	int		Graphics_D3D9_DeviceState_ResetVertexShader( int SetNormalVertexShaderCancel )
{
	if( GD3D9.Device.State.SetNormalVertexShader &&
		SetNormalVertexShaderCancel &&
		GD3D9.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
	{
		return 0 ;
	}

	if( GD3D9.Device.DrawSetting.CancelSettingEqualCheck == FALSE &&
		GD3D9.Device.State.SetVertexShader == NULL &&
		GD3D9.Device.State.SetVertexDeclaration == NULL )
	{
		return 0 ;
	}

	if( GAPIWin.Direct3DDevice9Object == NULL ) return -1 ;

	if( GD3D9.Device.DrawSetting.CancelSettingEqualCheck || GD3D9.Device.State.SetVertexShader != NULL )
	{
		Direct3DDevice9_SetVertexShader( NULL ) ;
		GD3D9.Device.State.SetVertexShader = NULL ;
	}

	if( GD3D9.Device.DrawSetting.CancelSettingEqualCheck || GD3D9.Device.State.SetVertexDeclaration != NULL )
	{
		Direct3DDevice9_SetVertexDeclaration( NULL ) ;
		GD3D9.Device.State.SetVertexDeclaration = NULL ;
	}

	GD3D9.Device.State.SetNormalVertexShader    = FALSE ;
	GD3D9.Device.State.SetFVF                   = 0 ;
	GD3D9.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

	// �I��
	return 0 ;
}

// �w��̃s�N�Z���V�F�[�_�[���f�o�C�X�ɃZ�b�g����
extern int Graphics_D3D9_DeviceState_SetPixelShaderToHandle( int ShaderHandle )
{
	SHADERHANDLEDATA *PixelShader ;

	// �n���h���`�F�b�N
	if( SHADERCHK( ShaderHandle, PixelShader ) )
		return -1 ;
	if( PixelShader->IsVertexShader != FALSE )
		return -1 ;

	// �V�F�[�_�[�̃Z�b�g
	Graphics_D3D9_DeviceState_SetPixelShader( PixelShader->PF->D3D9.PixelShader ) ;

	// ����I��
	return 0 ;
}

// �g�p����s�N�Z���V�F�[�_�[��ύX����
extern	int		Graphics_D3D9_DeviceState_SetPixelShader( D_IDirect3DPixelShader9 *PixelShader, int NormalPixelShader )
{
	if( GD3D9.Device.State.SetPixelShader == PixelShader && GD3D9.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
	{
		return 0 ;
	}

	if( GAPIWin.Direct3DDevice9Object == NULL ) return -1 ;

	Direct3DDevice9_SetPixelShader( PixelShader ) ;
	GD3D9.Device.State.SetPixelShader       = PixelShader ;
	GD3D9.Device.State.SetNormalPixelShader = NormalPixelShader ;
	if( NormalPixelShader == FALSE )
	{
		GD3D9.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;
	}

	// �I��
	return 0 ;
}

// �s�N�Z���V�F�[�_�[�̎g�p���~�߂�( �Œ�@�\�p�C�v���C�����g�p�����Ԃɂ��� )
extern	int		Graphics_D3D9_DeviceState_ResetPixelShader( int SetNormalPixelShaderCancel )
{
	if( GD3D9.Device.State.SetNormalPixelShader &&
		SetNormalPixelShaderCancel &&
		GD3D9.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
	{
		return 0 ;
	}

	if( GD3D9.Device.DrawSetting.CancelSettingEqualCheck == FALSE &&
		GD3D9.Device.State.SetPixelShader == NULL )
	{
		return 0 ;
	}

	if( GAPIWin.Direct3DDevice9Object == NULL ) return -1 ;

	if( GD3D9.Device.DrawSetting.CancelSettingEqualCheck || GD3D9.Device.State.SetPixelShader != NULL )
	{
		Direct3DDevice9_SetPixelShader( NULL ) ;
		GD3D9.Device.State.SetPixelShader = NULL ;
	}

	GD3D9.Device.State.SetNormalPixelShader     = FALSE ;
	GD3D9.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

	// �I��
	return 0 ;
}

// �g�p���钸�_�o�b�t�@��ύX����
extern	int		Graphics_D3D9_DeviceState_SetVertexBuffer( D_IDirect3DVertexBuffer9 *VertexBuffer, int VertexStride )
{
	if( GD3D9.Device.State.SetVertexBuffer       == VertexBuffer &&
		GD3D9.Device.State.SetVertexBufferStride == VertexStride &&
		GD3D9.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
	{
		return 0 ;
	}

	if( GAPIWin.Direct3DDevice9Object == NULL ) return -1 ;

	if( VertexBuffer != NULL )
	{
		Direct3DDevice9_SetStreamSource( 0, VertexBuffer, 0, ( UINT )VertexStride ) ;
	}
	GD3D9.Device.State.SetVertexBuffer       = VertexBuffer ;
	GD3D9.Device.State.SetVertexBufferStride = VertexStride ;

	// �I��
	return 0 ;
}

// �g�p����C���f�b�N�X�o�b�t�@��ύX����
extern	int		Graphics_D3D9_DeviceState_SetIndexBuffer( D_IDirect3DIndexBuffer9 *IndexBuffer )
{
	if( GD3D9.Device.State.SetIndexBuffer == IndexBuffer && GD3D9.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
	{
		return 0 ;
	}

	if( GAPIWin.Direct3DDevice9Object == NULL ) return -1 ;

	if( IndexBuffer != NULL )
	{
		Direct3DDevice9_SetIndices( IndexBuffer ) ;
	}
	GD3D9.Device.State.SetIndexBuffer = IndexBuffer ;

	// �I��
	return 0 ;
}

// �e�X�e�[�W���g�p����e�N�X�`���A�h���X���X�e�[�W�ԍ��Ɠ����ɂ���
extern	int		Graphics_D3D9_DeviceState_ResetTextureCoord( void )
{
	DWORD i ;
	GRAPHICS_HARDDATA_DIRECT3D9_TEXTURESTAGEINFO *Stage ;

	Stage = GD3D9.Device.State.BlendInfo.TextureStageInfo ;
	for( i = 0 ; i < ( DWORD )GD3D9.Device.Caps.MaxTextureBlendStages ; i ++, Stage ++ )
	{
		if( ( DWORD )Stage->TextureCoordIndex != i || GD3D9.Device.DrawSetting.CancelSettingEqualCheck )
		{
			Direct3DDevice9_SetTextureStageState( i, D_D3DTSS_TEXCOORDINDEX, i ) ;
			Stage->TextureCoordIndex = ( int )i ;
		}
	}

	// �I��
	return 0 ;
}










// ���[�U�[�̃u�����h�C���t�H��K������
extern	int		Graphics_D3D9_DeviceState_SetUserBlendInfo( GRAPHICS_HARDDATA_DIRECT3D9_BLENDINFO *BlendInfo, int TextureStageIsTextureAndTextureCoordOnlyFlag, int TextureIsGraphHandleFlag, int UseShader )
{
	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	Graphics_D3D9_DeviceState_RefreshBlendState( BlendInfo, TextureStageIsTextureAndTextureCoordOnlyFlag, TextureIsGraphHandleFlag, UseShader ) ;
	GD3D9.Device.DrawSetting.ChangeBlendParamFlag = TRUE ;
	GD3D9.Device.DrawSetting.ChangeTextureFlag    = TRUE ;
	GD3D9.Device.DrawSetting.DrawPrepAlwaysFlag   = TRUE ;

	// �I��
	return 0 ;
}












// �u�����h�e�[�u��(�f�o�b�O�p)
static const char *__BufferBlendName[] =
{
	NULL,
	"D3DBLEND_ZERO",
	"D3DBLEND_ONE",
	"D3DBLEND_SRCCOLOR",
	"D3DBLEND_INVSRCCOLOR",
	"D3DBLEND_SRCALPHA",
	"D3DBLEND_INVSRCALPHA",
	"D3DBLEND_DESTALPHA",
	"D3DBLEND_INVDESTALPHA",
	"D3DBLEND_DESTCOLOR",
	"D3DBLEND_INVDESTCOLOR",
	"D3DBLEND_SRCALPHASAT",
	"D3DBLEND_BOTHSRCALPHA",
	"D3DBLEND_BOTHINVSRCALPHA",
} ;

// �v�f�e�[�u��(�f�o�b�O�p)
static const char *__TextureStage_ARGName[] =
{
	"D3DTA_DIFFUSE",
	"D3DTA_CURRENT",
	"D3DTA_TEXTURE",
	"D3DTA_TFACTOR",
} ;

// �u�����h�e�[�u��(�f�o�b�O�p)
static const char *__TextureStage_OPName[] =
{
	NULL, 
	"D3DTOP_DISABLE",
	"D3DTOP_SELECTARG1",
	"D3DTOP_SELECTARG2",

	"D3DTOP_MODULATE",
	"D3DTOP_MODULATE2X",
	"D3DTOP_MODULATE4X",

	"D3DTOP_ADD",
	"D3DTOP_ADDSIGNED",
	"D3DTOP_ADDSIGNED2X",
	"D3DTOP_SUBTRACT",
	"D3DTOP_ADDSMOOTH",

	"D3DTOP_BLENDDIFFUSEALPHA",
	"D3DTOP_BLENDTEXTUREALPHA",
	"D3DTOP_BLENDFACTORALPHA",

	"D3DTOP_BLENDTEXTUREALPHAPM",
	"D3DTOP_BLENDCURRENTALPHA",

	"D3DTOP_PREMODULATE",
	"D3DTOP_MODULATEALPHA_ADDCOLOR",
	"D3DTOP_MODULATECOLOR_ADDALPHA",
	"D3DTOP_MODULATEINVALPHA_ADDCOLOR",
	"D3DTOP_MODULATEINVCOLOR_ADDALPHA",

	"D3DTOP_BUMPENVMAP",
	"D3DTOP_BUMPENVMAPLUMINANCE",
	"D3DTOP_DOTPRODUCT3",
} ;


// �q�f�a�l�u�����h�X�e�[�g�e�[�u��
// [�u�����h���[�h�l]
// [
//   0:DESTBLEND
//   1:SRCBLEND
//   2:�g�p�e�N�X�`���u�����h�e�[�u��( TEXTURE_BLEND_TABLE_INDEX_NORMAL �� )
//   3:�g�pBLENDOP
//   4:���u�����h�@�\���g�p���邩�ǂ���( 1:�g�p���� 0:�g�p���Ȃ� )
//   5:���`�����l���̑��݂���`���̏ꍇ�Ƀs�N�Z���V�F�[�_�[���g�p�������u�����h���s�����ǂ���( 1:�s��  0:�s��Ȃ� )
//   6:�A���t�@�l���O�̂Ƃ��ɕ`����s��Ȃ����ǂ���( 1:�s��Ȃ�  0:�s�� )
// ]
#define RGB_BLENDSTATE_TABLE_DESTBLEND				(0)
#define RGB_BLENDSTATE_TABLE_SRCBLEND				(1)
#define RGB_BLENDSTATE_TABLE_TEXBLENDTABLEINDEX		(2)
#define RGB_BLENDSTATE_TABLE_BLENDOP				(3)
#define RGB_BLENDSTATE_TABLE_ALPHABLENDENABLE		(4)
#define RGB_BLENDSTATE_TABLE_SHADERALPHABLEND		(5)

#define TEXTURE_BLEND_TABLE_INDEX_NORMAL			(0)			// 0:�ʏ�`��
#define TEXTURE_BLEND_TABLE_INDEX_MUL				(1)			// 1:��Z�`��
#define TEXTURE_BLEND_TABLE_INDEX_INVERSE			(2)			// 2:RGB���]
#define TEXTURE_BLEND_TABLE_INDEX_X4				(3)			// 3:�`��P�x4�{
#define TEXTURE_BLEND_TABLE_INDEX_PMA_NORMAL		(4)			// 4:��Z�ς݃��u�����h���[�h�̒ʏ�`��
#define TEXTURE_BLEND_TABLE_INDEX_PMA_INVERSE		(5)			// 5:��Z�ς݃��u�����h���[�h��RGB���]
#define TEXTURE_BLEND_TABLE_INDEX_PMA_X4			(6)			// 6:��Z�ς݃��u�����h���[�h�̕`��P�x4�{
#define TEXTURE_BLEND_TABLE_INDEX_NUM				(7)
static const int __RGBBlendStateTable[ DX_BLENDMODE_NUM ][ 7 ] =
{
	{ -1,						-1,							TEXTURE_BLEND_TABLE_INDEX_NORMAL,		D_D3DBLENDOP_ADD,			0, 0, 1 },	// DX_BLENDMODE_NOBLEND			�m�[�u�����h
	{ D_D3DBLEND_INVSRCALPHA,	D_D3DBLEND_SRCALPHA,		TEXTURE_BLEND_TABLE_INDEX_NORMAL,		D_D3DBLENDOP_ADD,			1, 1, 1 },	// DX_BLENDMODE_ALPHA			���u�����h
	{ D_D3DBLEND_ONE,			D_D3DBLEND_SRCALPHA,		TEXTURE_BLEND_TABLE_INDEX_NORMAL,		D_D3DBLENDOP_ADD,			1, 0, 1 },	// DX_BLENDMODE_ADD				���Z�u�����h
	{ D_D3DBLEND_ZERO,			D_D3DBLEND_INVDESTCOLOR,	TEXTURE_BLEND_TABLE_INDEX_NORMAL,		D_D3DBLENDOP_ADD,			1, 0, 1 },	// DX_BLENDMODE_SUB				���Z�u�����h
	{ D_D3DBLEND_SRCCOLOR,		D_D3DBLEND_ZERO,			TEXTURE_BLEND_TABLE_INDEX_NORMAL,		D_D3DBLENDOP_ADD,			1, 0, 0 },	// DX_BLENDMODE_MUL				��Z�u�����h
	{ D_D3DBLEND_ONE,			D_D3DBLEND_SRCALPHA,		TEXTURE_BLEND_TABLE_INDEX_NORMAL,		D_D3DBLENDOP_ADD,			1, 0, 1 },	// DX_BLENDMODE_SUB2			���������p���Z�u�����h�P
	{ -1,						-1,							TEXTURE_BLEND_TABLE_INDEX_NORMAL,		D_D3DBLENDOP_ADD,			0, 0, 1 },	// DX_BLENDMODE_XOR				XOR�u�����h(��Ή�)
	{ -1,						-1,							TEXTURE_BLEND_TABLE_INDEX_NORMAL,		D_D3DBLENDOP_ADD,			0, 0, 1 },	// ����
	{ D_D3DBLEND_ONE,			D_D3DBLEND_ZERO,			TEXTURE_BLEND_TABLE_INDEX_NORMAL,		D_D3DBLENDOP_ADD,			1, 0, 0 },	// DX_BLENDMODE_DESTCOLOR		�J���[�͍X�V����Ȃ�
	{ D_D3DBLEND_ZERO,			D_D3DBLEND_INVDESTCOLOR,	TEXTURE_BLEND_TABLE_INDEX_NORMAL,		D_D3DBLENDOP_ADD,			1, 0, 0 },	// DX_BLENDMODE_INVDESTCOLOR	�`���̐F�̔��]�l���|����
	{ D_D3DBLEND_INVSRCALPHA,	D_D3DBLEND_SRCALPHA,		TEXTURE_BLEND_TABLE_INDEX_INVERSE,		D_D3DBLENDOP_ADD,			1, 1, 1 },	// DX_BLENDMODE_INVSRC			�`�挳�̐F�𔽓]����
	{ D_D3DBLEND_SRCCOLOR,		D_D3DBLEND_ZERO,			TEXTURE_BLEND_TABLE_INDEX_MUL,			D_D3DBLENDOP_ADD,			1, 0, 1 },	// DX_BLENDMODE_MULA			�A���t�@�`�����l���l���t����Z�u�����h
	{ D_D3DBLEND_INVSRCALPHA,	D_D3DBLEND_SRCALPHA,		TEXTURE_BLEND_TABLE_INDEX_X4,			D_D3DBLENDOP_ADD,			1, 1, 1 },	// DX_BLENDMODE_ALPHA_X4		���u�����h�̕`�摤�̋P�x���ő�S�{�ɂł��郂�[�h
	{ D_D3DBLEND_ONE,			D_D3DBLEND_SRCALPHA,		TEXTURE_BLEND_TABLE_INDEX_X4,			D_D3DBLENDOP_ADD,			1, 0, 1 },	// DX_BLENDMODE_ADD_X4			���Z�u�����h�̕`�摤�̋P�x���ő�S�{�ɂł��郂�[�h
	{ D_D3DBLEND_ZERO,			D_D3DBLEND_ONE,				TEXTURE_BLEND_TABLE_INDEX_NORMAL,		D_D3DBLENDOP_ADD,			1, 0, 0 },	// DX_BLENDMODE_SRCCOLOR		�`�挳�̃J���[�ł��̂܂ܕ`�悳���
	{ D_D3DBLEND_INVSRCALPHA,	D_D3DBLEND_ONE,				TEXTURE_BLEND_TABLE_INDEX_NORMAL,		D_D3DBLENDOP_ADD,			1, 0, 0 },	// DX_BLENDMODE_HALF_ADD		�����Z�u�����h
	{ D_D3DBLEND_ONE,			D_D3DBLEND_SRCALPHA,		TEXTURE_BLEND_TABLE_INDEX_NORMAL,		D_D3DBLENDOP_REVSUBTRACT,	1, 0, 1 },	// DX_BLENDMODE_SUB1			�o�̓u�����h���g�p�\�ȏꍇ�̌��Z�u�����h

	{ D_D3DBLEND_INVSRCALPHA,	D_D3DBLEND_ONE,				TEXTURE_BLEND_TABLE_INDEX_PMA_NORMAL,	D_D3DBLENDOP_ADD,			1, 0, 1 },	// DX_BLENDMODE_PMA_ALPHA		��Z�ς݃��u�����h���[�h�̃��u�����h
	{ D_D3DBLEND_ONE,			D_D3DBLEND_ONE,				TEXTURE_BLEND_TABLE_INDEX_PMA_NORMAL,	D_D3DBLENDOP_ADD,			1, 0, 1 },	// DX_BLENDMODE_PMA_ADD			��Z�ς݃��u�����h���[�h�̉��Z�u�����h
	{ D_D3DBLEND_ONE,			D_D3DBLEND_ONE,				TEXTURE_BLEND_TABLE_INDEX_PMA_NORMAL,	D_D3DBLENDOP_REVSUBTRACT,	1, 0, 1 },	// DX_BLENDMODE_PMA_SUB			��Z�ς݃��u�����h���[�h�̌��Z�u�����h
	{ D_D3DBLEND_INVSRCALPHA,	D_D3DBLEND_ONE,				TEXTURE_BLEND_TABLE_INDEX_PMA_INVERSE,	D_D3DBLENDOP_ADD,			1, 0, 1 },	// DX_BLENDMODE_PMA_INVSRC		��Z�ς݃��u�����h���[�h�̕`�挳�̐F�𔽓]����
	{ D_D3DBLEND_INVSRCALPHA,	D_D3DBLEND_ONE,				TEXTURE_BLEND_TABLE_INDEX_PMA_X4,		D_D3DBLENDOP_ADD,			1, 0, 1 },	// DX_BLENDMODE_PMA_ALPHA_X4	��Z�ς݃��u�����h���[�h�̃��u�����h�̕`�摤�̋P�x���ő�S�{�ɂł��郂�[�h
	{ D_D3DBLEND_ONE,			D_D3DBLEND_ONE,				TEXTURE_BLEND_TABLE_INDEX_PMA_X4,		D_D3DBLENDOP_ADD,			1, 0, 1 },	// DX_BLENDMODE_PMA_ADD_X4		��Z�ς݃��u�����h���[�h�̉��Z�u�����h�̕`�摤�̋P�x���ő�S�{�ɂł��郂�[�h
} ;

// �`�l�u�����h�X�e�[�g�e�[�u��[�u�����h���[�h�l][ 0:DESTBLEND  1:SRCBLEND  2:�V�F�[�_�[���u�����h���g�p���邩( 0:�g�p���Ȃ�  1:�g�p���� )  3:�g�pBLENDOP ]
#define ALPHA_BLENDSTATE_TABLE_DESTBLEND			(0)
#define ALPHA_BLENDSTATE_TABLE_SRCBLEND				(1)
#define ALPHA_BLENDSTATE_TABLE_USESHADERBLEND		(2)
#define ALPHA_BLENDSTATE_TABLE_BLENDOP				(3)
static const int __ABlendStateTable[ DX_BLENDMODE_NUM ][ 4 ] =
{
	{ -1,						-1,							0, D_D3DBLENDOP_ADD			},  // DX_BLENDMODE_NOBLEND			�m�[�u�����h
	{ -1,						-1,							1, D_D3DBLENDOP_ADD			},	// DX_BLENDMODE_ALPHA			���u�����h
	{ D_D3DBLEND_ONE,			D_D3DBLEND_ZERO,			0, D_D3DBLENDOP_ADD			},	// DX_BLENDMODE_ADD				���Z�u�����h
	{ D_D3DBLEND_ONE,			D_D3DBLEND_ZERO,			0, D_D3DBLENDOP_ADD			},	// DX_BLENDMODE_SUB				���Z�u�����h
	{ D_D3DBLEND_ONE,			D_D3DBLEND_ZERO,			0, D_D3DBLENDOP_ADD			},	// DX_BLENDMODE_MUL				��Z�u�����h
	{ D_D3DBLEND_ONE,			D_D3DBLEND_ZERO,			0, D_D3DBLENDOP_ADD			},	// DX_BLENDMODE_SUB2			���������p���Z�u�����h�P
	{ -1,						-1,							0, D_D3DBLENDOP_ADD			},	// DX_BLENDMODE_XOR				XOR�u�����h(��Ή�)
	{ -1,						-1,							0, D_D3DBLENDOP_ADD			},	// ����
	{ D_D3DBLEND_ONE,			D_D3DBLEND_ZERO,			0, D_D3DBLENDOP_ADD			},	// DX_BLENDMODE_DESTCOLOR		�J���[�͍X�V����Ȃ�
	{ D_D3DBLEND_ONE,			D_D3DBLEND_ZERO,			0, D_D3DBLENDOP_ADD			},	// DX_BLENDMODE_INVDESTCOLOR	�`���̐F�̔��]�l���|����
	{ -1,						-1,							1, D_D3DBLENDOP_ADD			},	// DX_BLENDMODE_INVSRC			�`�挳�̐F�𔽓]����
	{ D_D3DBLEND_ONE,			D_D3DBLEND_ZERO,			0, D_D3DBLENDOP_ADD			},	// DX_BLENDMODE_MULA			�A���t�@�`�����l���l���t����Z�u�����h
	{ -1,						-1,							1, D_D3DBLENDOP_ADD			},	// DX_BLENDMODE_ALPHA_X4		���u�����h�̕`�摤�̋P�x���ő�S�{�ɂł��郂�[�h
	{ D_D3DBLEND_ONE,			D_D3DBLEND_ZERO,			0, D_D3DBLENDOP_ADD			},	// DX_BLENDMODE_ADD_X4			���Z�u�����h�̕`�摤�̋P�x���ő�S�{�ɂł��郂�[�h
	{ D_D3DBLEND_ZERO,			D_D3DBLEND_ONE,				0, D_D3DBLENDOP_ADD			},	// DX_BLENDMODE_SRCCOLOR		�`�挳�̃J���[�ł��̂܂ܕ`�悳���
	{ D_D3DBLEND_INVSRCALPHA,	D_D3DBLEND_ONE,				0, D_D3DBLENDOP_ADD			},	// DX_BLENDMODE_HALF_ADD		�����Z�u�����h
	{ D_D3DBLEND_ONE,			D_D3DBLEND_ZERO,			0, D_D3DBLENDOP_ADD			},	// DX_BLENDMODE_SUB1			�o�̓u�����h���g�p�\�ȏꍇ�̌��Z�u�����h

	{ D_D3DBLEND_INVSRCALPHA,	D_D3DBLEND_ONE,				0, D_D3DBLENDOP_ADD			},	// DX_BLENDMODE_PMA_ALPHA		��Z�ς݃��u�����h���[�h�̃��u�����h
	{ D_D3DBLEND_ONE,			D_D3DBLEND_ONE,				0, D_D3DBLENDOP_ADD			},	// DX_BLENDMODE_PMA_ADD			��Z�ς݃��u�����h���[�h�̉��Z�u�����h
	{ D_D3DBLEND_ONE,			D_D3DBLEND_ONE,				0, D_D3DBLENDOP_REVSUBTRACT },	// DX_BLENDMODE_PMA_SUB			��Z�ς݃��u�����h���[�h�̌��Z�u�����h
	{ D_D3DBLEND_INVSRCALPHA,	D_D3DBLEND_ONE,				0, D_D3DBLENDOP_ADD			},	// DX_BLENDMODE_PMA_INVSRC		��Z�ς݃��u�����h���[�h�̕`�挳�̐F�𔽓]����
	{ D_D3DBLEND_INVSRCALPHA,	D_D3DBLEND_ONE,				0, D_D3DBLENDOP_ADD			},	// DX_BLENDMODE_PMA_ALPHA_X4	��Z�ς݃��u�����h���[�h�̃��u�����h�̕`�摤�̋P�x���ő�S�{�ɂł��郂�[�h
	{ D_D3DBLEND_ONE,			D_D3DBLEND_ONE,				0, D_D3DBLENDOP_ADD			},	// DX_BLENDMODE_PMA_ADD_X4		��Z�ς݃��u�����h���[�h�̉��Z�u�����h�̕`�摤�̋P�x���ő�S�{�ɂł��郂�[�h
} ;

#define __NONE		(-1)

#define __DIFF		D_D3DTA_DIFFUSE
#define __CURR		D_D3DTA_CURRENT
#define __TEX_		D_D3DTA_TEXTURE
#define __FACT		D_D3DTA_TFACTOR
#define __SPEC		D_D3DTA_SPECULAR
#define __COMP		D_D3DTA_COMPLEMENT
#define __ALRP		D_D3DTA_ALPHAREPLICATE
#define __TEMP		D_D3DTA_TEMP

#define __DISB		D_D3DTOP_DISABLE
#define __SEL1		D_D3DTOP_SELECTARG1
#define __SEL2		D_D3DTOP_SELECTARG2
#define __MUL1		D_D3DTOP_MODULATE
#define __MUL2		D_D3DTOP_MODULATE2X
#define __MUL4		D_D3DTOP_MODULATE4X
#define __ADD1		D_D3DTOP_ADD
#define __SUB1		D_D3DTOP_SUBTRACT
#define __BLDC		D_D3DTOP_BLENDCURRENTALPHA 
#define __BLDF		D_D3DTOP_BLENDFACTORALPHA 

#define __NUM(x)	((void *)(x))
#define DISB_ST		{NULL, -1, -1, __NONE,__NONE,__DISB,	__NONE,__NONE,__DISB},

// ���Z�u�����h�p�X�e�[�g
static const GRAPHICS_HARDDATA_DIRECT3D9_TEXTURESTAGEINFO __TexSubBlendStageStateTable[USE_TEXTURESTAGE_NUM] =
{
	/*Stage 0*/{__NUM(1),0,0,__NONE,__DIFF,__SEL2,	__DIFF,__NONE,__SEL1},
	/*Stage 1*/{    NULL,0,0,__NONE,__NONE,__DISB,	__NONE,__NONE,__DISB},
	/*Stage 2*/{    NULL,0,0,__NONE,__NONE,__DISB,	__NONE,__NONE,__DISB},
	/*Stage 3*/{    NULL,0,0,__NONE,__NONE,__DISB,	__NONE,__NONE,__DISB},
	/*Stage 4*/{    NULL,0,0,__NONE,__NONE,__DISB,	__NONE,__NONE,__DISB},
	/*Stage 5*/{    NULL,0,0,__NONE,__NONE,__DISB,	__NONE,__NONE,__DISB},
	/*Stage 6*/{    NULL,0,0,__NONE,__NONE,__DISB,	__NONE,__NONE,__DISB},
	/*Stage 7*/{    NULL,0,0,__NONE,__NONE,__DISB,	__NONE,__NONE,__DISB},
	/*Stage 8*/{    NULL,0,0,__NONE,__NONE,__DISB,	__NONE,__NONE,__DISB},
	/*Stage 9*/{    NULL,0,0,__NONE,__NONE,__DISB,	__NONE,__NONE,__DISB},
	/*Stage10*/{    NULL,0,0,__NONE,__NONE,__DISB,	__NONE,__NONE,__DISB},
	/*Stage11*/{    NULL,0,0,__NONE,__NONE,__DISB,	__NONE,__NONE,__DISB},
} ;

// �e�N�X�`���[�X�e�[�W�X�e�[�g
//0:[0:�ʏ�`�� 1:MUL 2:RGB���] 3:�`��P�x4�{ 4:��Z�ς݃��u�����h���[�h�̒ʏ�`�� 5:��Z�ς݃��u�����h���[�h��RGB���] 6:��Z�ς݃��u�����h���[�h�̕`��P�x4�{]
//1:[0:�u�����h�e�N�X�`������  1:DX_BLENDGRAPHTYPE_NORMAL  2:DX_BLENDGRAPHTYPE_WIPE  3:DX_BLENDGRAPHTYPE_ALPHA ]
//2:[0:�f�B�t���[�YRGB�J���[����  1:�f�B�t���[�YRGB�J���[����]
//3:[0:�f�B�t���[�YAlpha�J���[����  1:�f�B�t���[�YAlpha�J���[����]
//4:[0:���`�����l�������@1:���`�����l������  2:���e�X�g����]
//5:[ �e�N�X�`���[�X�e�[�W�i���o�[ ]
static const GRAPHICS_HARDDATA_DIRECT3D9_TEXTURESTAGEINFO __TexStageStateTable[ TEXTURE_BLEND_TABLE_INDEX_NUM ][4][2][2][3][USE_TEXTURESTAGE_NUM] =
{
	// �ʏ�`��
	{
		// �u�����h�e�N�X�`������
		{
			//�f�B�t���[�YRGB�J���[����
			{
				//�f�B�t���[�YAlpha�J���[����
				{
					//���`�����l������
					{
						/*Stage0*/{__NUM(1), 0, 0,__NONE,__DIFF,__SEL2,	__TEX_,__NONE,__SEL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						/*Stage0*/{__NUM(1), 0, 0,__TEX_,__NONE,__SEL1,	__TEX_,__NONE,__SEL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���e�X�g����
					{
						/*Stage0*/{__NUM(1), 0, 0,__TEX_,__NONE,__SEL1,	__TEX_,__NONE,__SEL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
				//�f�B�t���[�YAlpha�J���[����
				{
					//���`�����l������
					{
						/*Stage0*/{__NUM(1), 0, 0,__NONE,__DIFF,__SEL2,	__TEX_,__NONE,__SEL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						/*Stage0*/{__NUM(1), 0, 0,__TEX_,__DIFF,__MUL1,	__TEX_,__NONE,__SEL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
					
					//���e�X�g����
					{
						/*Stage0*/{__NUM(1), 0, 0,__TEX_,__DIFF,__MUL1,	__TEX_,__NONE,__SEL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
			},
			//�f�B�t���[�YRGB�J���[����
			{
				//�f�B�t���[�YAlpha�J���[�Ȃ�
				{
					//���`�����l������
					{
						/*Stage0*/{__NUM(1), 0, 0,__NONE,__DIFF,__SEL2,	__TEX_,__DIFF,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						/*Stage0*/{__NUM(1), 0, 0,__TEX_,__NONE,__SEL1,	__TEX_,__DIFF,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
					
					//���e�X�g����
					{
						/*Stage0*/{__NUM(1), 0, 0,__TEX_,__NONE,__SEL1,	__TEX_,__DIFF,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
				//�f�B�t���[�YAlpha�J���[����
				{
					//���`�����l������
					{
						/*Stage0*/{__NUM(1), 0, 0,__NONE,__DIFF,__SEL2,	__TEX_,__DIFF,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						/*Stage0*/{__NUM(1), 0, 0,__TEX_,__DIFF,__MUL1,	__TEX_,__DIFF,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
					
					//���e�X�g����
					{
						/*Stage0*/{__NUM(1), 0, 0,__TEX_,__DIFF,__MUL1,	__TEX_,__DIFF,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
			},
		},
		//�u�����h�e�N�X�`������ DX_BLENDGRAPHTYPE_NORMAL
		{
			//�f�B�t���[�YRGB�J���[����
			{
				//�f�B�t���[�YAlpha�J���[����
				{
					//���`�����l������
					{
						/*Stage0*/{__NUM(2), 0, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE,__SEL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__TEX_,__BLDF,	__CURR,__TEX_,__BLDF}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						/*Stage0*/{__NUM(2), 0, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE,__SEL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__TEX_,__BLDF,	__CURR,__TEX_,__BLDF}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
					
					//���e�X�g����
					{
						/*Stage0*/{__NUM(2), 0, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE,__SEL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__TEX_,__BLDF,	__CURR,__TEX_,__BLDF}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
				//�f�B�t���[�YAlpha�J���[����
				{
					//���`�����l������
					{
						/*Stage0*/{__NUM(3), 0, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE,__SEL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__TEX_,__BLDF,	__CURR,__TEX_,__BLDF},
						/*Stage2*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR,__DIFF,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						/*Stage0*/{__NUM(3), 0, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE,__SEL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__TEX_,__BLDF,	__CURR,__TEX_,__BLDF},
						/*Stage2*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR,__DIFF,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
					
					//���e�X�g����
					{
						/*Stage0*/{__NUM(3), 0, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE,__SEL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__TEX_,__BLDF,	__CURR,__TEX_,__BLDF},
						/*Stage2*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR,__DIFF,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
			},
			//�f�B�t���[�YRGB�J���[����
			{
				//�f�B�t���[�YAlpha�J���[����
				{
					//���`�����l������
					{
						/*Stage0*/{__NUM(3), 0, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE,__SEL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__TEX_,__BLDF,	__CURR,__TEX_,__BLDF},
						/*Stage2*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR,__DIFF,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						/*Stage0*/{__NUM(3), 0, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE,__SEL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__TEX_,__BLDF,	__CURR,__TEX_,__BLDF},
						/*Stage2*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR,__DIFF,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
					
					//���e�X�g����
					{
						/*Stage0*/{__NUM(3), 0, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE,__SEL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__TEX_,__BLDF,	__CURR,__TEX_,__BLDF},
						/*Stage2*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR,__DIFF,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
				//�f�B�t���[�YAlpha�J���[����
				{
					//���`�����l������
					{
						/*Stage0*/{__NUM(3), 0, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE,__SEL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__TEX_,__BLDF,	__CURR,__TEX_,__BLDF},
						/*Stage2*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR,__DIFF,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						/*Stage0*/{__NUM(3), 0, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE,__SEL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__TEX_,__BLDF,	__CURR,__TEX_,__BLDF},
						/*Stage2*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR,__DIFF,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
					
					//���e�X�g����
					{
						/*Stage0*/{__NUM(3), 0, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE,__SEL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__TEX_,__BLDF,	__CURR,__TEX_,__BLDF},
						/*Stage2*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR,__DIFF,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
			},
		},
		//�u�����h�e�N�X�`������ DX_BLENDGRAPHTYPE_WIPE
		{
			//�f�B�t���[�YRGB�J���[����
			{
				//�f�B�t���[�YAlpha�J���[����
				{
					//���`�����l������
					{
						/*Stage0*/{__NUM(2), 0, 0, __TEX_,__FACT,__MUL1,	__NONE,__DIFF,__SEL2},
						/*Stage1*/{    NULL, 1, 0, __CURR,__DIFF,__MUL1,	__TEX_,__NONE,__SEL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						/*Stage0*/{__NUM(3), 0, 0, __TEX_,__FACT,__ADD1,	__NONE,__DIFF,__SEL2},
						/*Stage1*/{    NULL, 1, 0, __SPEC,__CURR,__MUL1,	__CURR,__NONE,__SEL1},
						/*Stage2*/{    NULL, 1, 0, __TEX_,__CURR,__MUL1,	__TEX_,__NONE,__SEL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
					
					//���e�X�g����
					{
						/*Stage0*/{__NUM(2), 0, 0, __TEX_,__FACT,__ADD1,	__NONE,__DIFF,__SEL2},
						/*Stage1*/{    NULL, 1, 0, __TEX_,__CURR,__MUL1,	__TEX_,__NONE,__SEL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
				//�f�B�t���[�YAlpha�J���[����
				{
					//���`�����l������
					{
						/*Stage0*/{__NUM(2), 0, 0, __TEX_,__FACT,__ADD1,	__NONE,__DIFF,__SEL2},
						/*Stage1*/{    NULL, 1, 0, __DIFF,__CURR,__MUL1,	__TEX_,__NONE,__SEL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						/*Stage0*/{__NUM(4), 0, 0, __TEX_,__FACT,__ADD1,	__NONE,__DIFF,__SEL2},
						/*Stage1*/{    NULL, 1, 0, __SPEC,__CURR,__MUL1,	__CURR,__NONE,__SEL1},
						/*Stage2*/{    NULL, 1, 0, __TEX_,__CURR,__MUL1,	__TEX_,__NONE,__SEL1},
						/*Stage3*/{    NULL, 0, 0, __DIFF,__CURR,__MUL1,	__CURR,__NONE,__SEL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
					
					//���e�X�g����
					{
						/*Stage0*/{__NUM(3), 0, 0, __TEX_,__FACT,__ADD1,	__NONE,__DIFF,__SEL2},
						/*Stage1*/{    NULL, 1, 0, __TEX_,__CURR,__MUL1,	__TEX_,__NONE,__SEL1},
						/*Stage2*/{    NULL, 1, 0, __DIFF,__CURR,__MUL1,	__CURR,__NONE,__SEL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
			},
			//�f�B�t���[�YRGB�J���[����
			{
				//�f�B�t���[�YAlpha�J���[����
				{
					//���`�����l������
					{
						/*Stage0*/{__NUM(2), 0, 0, __TEX_,__FACT,__ADD1,	__NONE,__DIFF,__SEL2},
						/*Stage1*/{    NULL, 1, 0, __CURR,__DIFF,__MUL1,	__TEX_,__DIFF,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						/*Stage0*/{__NUM(3), 0, 0, __TEX_,__FACT,__ADD1,	__NONE,__DIFF,__SEL2},
						/*Stage1*/{    NULL, 1, 0, __SPEC,__CURR,__MUL1,	__CURR,__NONE,__SEL1},
						/*Stage2*/{    NULL, 1, 0, __TEX_,__CURR,__MUL1,	__TEX_,__DIFF,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
					
					//���e�X�g����
					{
						/*Stage0*/{__NUM(2), 0, 0, __TEX_,__FACT,__ADD1,	__NONE,__DIFF,__SEL2},
						/*Stage1*/{    NULL, 1, 0, __TEX_,__CURR,__MUL1,	__TEX_,__DIFF,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
				//�f�B�t���[�YAlpha�J���[����
				{
					//���`�����l������
					{
						/*Stage0*/{__NUM(2), 0, 0, __TEX_,__FACT,__ADD1,	__NONE,__DIFF,__SEL2},
						/*Stage1*/{    NULL, 1, 0, __DIFF,__CURR,__MUL1,	__TEX_,__DIFF,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						/*Stage0*/{__NUM(4), 0, 0, __TEX_,__FACT,__ADD1,	__NONE,__DIFF,__SEL2},
						/*Stage1*/{    NULL, 1, 0, __SPEC,__CURR,__MUL1,	__CURR,__NONE,__SEL1},
						/*Stage2*/{    NULL, 1, 0, __TEX_,__CURR,__MUL1,	__TEX_,__DIFF,__MUL1},
						/*Stage3*/{    NULL, 0, 0, __DIFF,__CURR,__MUL1,	__CURR,__NONE,__SEL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
					
					//���e�X�g����
					{
						/*Stage0*/{__NUM(3), 0, 0, __TEX_,__FACT,__ADD1,	__NONE,__DIFF,__SEL2},
						/*Stage1*/{    NULL, 1, 0, __TEX_,__CURR,__MUL1,	__TEX_,__DIFF,__MUL1},
						/*Stage2*/{    NULL, 1, 0, __DIFF,__CURR,__MUL1,	__CURR,__NONE,__SEL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
			},
		},
		//�u�����h�e�N�X�`������ DX_BLENDGRAPHTYPE_ALPHA
		{
			//�f�B�t���[�YRGB�J���[����
			{
				//�f�B�t���[�YAlpha�J���[����
				{
					//���`�����l������
					{
						/*Stage0*/{__NUM(2), 1, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE,__SEL1},
						/*Stage1*/{    NULL, 0, 0, __CURR,__TEX_,__MUL1,	__CURR,__NONE,__SEL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						/*Stage0*/{__NUM(2), 1, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE,__SEL1},
						/*Stage1*/{    NULL, 0, 0, __CURR,__TEX_,__MUL1,	__CURR,__NONE,__SEL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
					
					//���e�X�g����
					{
						/*Stage0*/{__NUM(2), 1, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE,__SEL1},
						/*Stage1*/{    NULL, 0, 0, __CURR,__TEX_,__MUL1,	__CURR,__NONE,__SEL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
				//�f�B�t���[�YAlpha�J���[����
				{
					//���`�����l������
					{
						/*Stage0*/{__NUM(3), 1, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE,__SEL1},
						/*Stage1*/{    NULL, 0, 0, __CURR,__TEX_,__MUL1,	__CURR,__NONE,__SEL1},
						/*Stage2*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR,__DIFF,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						/*Stage0*/{__NUM(3), 1, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE,__SEL1},
						/*Stage1*/{    NULL, 0, 0, __CURR,__TEX_,__MUL1,	__CURR,__NONE,__SEL1},
						/*Stage2*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR,__DIFF,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
					
					//���e�X�g����
					{
						/*Stage0*/{__NUM(3), 1, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE,__SEL1},
						/*Stage1*/{    NULL, 0, 0, __CURR,__TEX_,__MUL1,	__CURR,__NONE,__SEL1},
						/*Stage2*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR,__DIFF,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
			},
			//�f�B�t���[�YRGB�J���[����
			{
				//�f�B�t���[�YAlpha�J���[����
				{
					//���`�����l������
					{
						/*Stage0*/{__NUM(3), 1, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE,__SEL1},
						/*Stage1*/{    NULL, 0, 0, __CURR,__TEX_,__MUL1,	__CURR,__NONE,__SEL1},
						/*Stage2*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR,__DIFF,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						/*Stage0*/{__NUM(3), 1, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE,__SEL1},
						/*Stage1*/{    NULL, 0, 0, __CURR,__TEX_,__MUL1,	__CURR,__NONE,__SEL1},
						/*Stage2*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR,__DIFF,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
					
					//���e�X�g����
					{
						/*Stage0*/{__NUM(3), 1, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE,__SEL1},
						/*Stage1*/{    NULL, 0, 0, __CURR,__TEX_,__MUL1,	__CURR,__NONE,__SEL1},
						/*Stage2*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR,__DIFF,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
				//�f�B�t���[�YAlpha�J���[����
				{
					//���`�����l������
					{
						/*Stage0*/{__NUM(3), 1, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE,__SEL1},
						/*Stage1*/{    NULL, 0, 0, __CURR,__TEX_,__MUL1,	__CURR,__NONE,__SEL1},
						/*Stage2*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR,__DIFF,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						/*Stage0*/{__NUM(3), 1, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE,__SEL1},
						/*Stage1*/{    NULL, 0, 0, __CURR,__TEX_,__MUL1,	__CURR,__NONE,__SEL1},
						/*Stage2*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR,__DIFF,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
					
					//���e�X�g����
					{
						/*Stage0*/{__NUM(3), 1, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE,__SEL1},
						/*Stage1*/{    NULL, 0, 0, __CURR,__TEX_,__MUL1,	__CURR,__NONE,__SEL1},
						/*Stage2*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR,__DIFF,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
			},
		},
	},

	// MUL�`��
	{
		// �u�����h�e�N�X�`������
		{
			//�f�B�t���[�YRGB�J���[����
			{
				//�f�B�t���[�YAlpha�J���[����
				{
					//���`�����l������
					{
						/*Stage0*/{__NUM(2), 0, 0, __NONE,__DIFF,__SEL2,	__TEX_,__NONE,__SEL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__NONE,__SEL1,	__CURR,__FACT,__BLDC}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						/*Stage0*/{__NUM(2), 0, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE,__SEL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__NONE,__SEL1,	__CURR,__FACT,__BLDC}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���e�X�g����
					{
						/*Stage0*/{__NUM(2), 0, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE,__SEL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__NONE,__SEL1,	__CURR,__FACT,__BLDC}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
				//�f�B�t���[�YAlpha�J���[����
				{
					//���`�����l������
					{
						/*Stage0*/{__NUM(2), 0, 0, __NONE,__DIFF,__SEL2,	__TEX_,__NONE,__SEL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__NONE,__SEL1,	__CURR,__FACT,__BLDC}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						/*Stage0*/{__NUM(2), 0, 0, __TEX_,__DIFF,__MUL1,	__TEX_,__NONE,__SEL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__NONE,__SEL1,	__CURR,__FACT,__BLDC}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
					
					//���e�X�g����
					{
						/*Stage0*/{__NUM(2), 0, 0, __TEX_,__DIFF,__MUL1,	__TEX_,__NONE,__SEL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__NONE,__SEL1,	__CURR,__FACT,__BLDC}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
			},
			//�f�B�t���[�YRGB�J���[����
			{
				//�f�B�t���[�YAlpha�J���[�Ȃ�
				{
					//���`�����l������
					{
						/*Stage0*/{__NUM(2), 0, 0, __NONE,__DIFF,__SEL2,	__TEX_,__DIFF,__MUL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__NONE,__SEL1,	__CURR,__FACT,__BLDC}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						/*Stage0*/{__NUM(2), 0, 0, __TEX_,__NONE,__SEL1,	__TEX_,__DIFF,__MUL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__NONE,__SEL1,	__CURR,__FACT,__BLDC}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
					
					//���e�X�g����
					{
						/*Stage0*/{__NUM(2), 0, 0, __TEX_,__NONE,__SEL1,	__TEX_,__DIFF,__MUL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__NONE,__SEL1,	__CURR,__FACT,__BLDC}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
				//�f�B�t���[�YAlpha�J���[����
				{
					//���`�����l������
					{
						/*Stage0*/{__NUM(2), 0, 0, __NONE,__DIFF,__SEL2,	__TEX_,__DIFF,__MUL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__NONE,__SEL1,	__CURR,__FACT,__BLDC}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						/*Stage0*/{__NUM(2), 0, 0, __TEX_,__DIFF,__MUL1,	__TEX_,__DIFF,__MUL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__NONE,__SEL1,	__CURR,__FACT,__BLDC}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
					
					//���e�X�g����
					{
						/*Stage0*/{__NUM(2), 0, 0, __TEX_,__DIFF,__MUL1,	__TEX_,__DIFF,__MUL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__NONE,__SEL1,	__CURR,__FACT,__BLDC}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
			},
		},
		//�u�����h�e�N�X�`������ DX_BLENDGRAPHTYPE_NORMAL
		{
			//�f�B�t���[�YRGB�J���[����
			{
				//�f�B�t���[�YAlpha�J���[����
				{
					//���`�����l������
					{
						 DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						 DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
					
					//���e�X�g����
					{
						 DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
				//�f�B�t���[�YAlpha�J���[����
				{
					//���`�����l������
					{
						 DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						 DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
					
					//���e�X�g����
					{
						 DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
			},
			//�f�B�t���[�YRGB�J���[����
			{
				//�f�B�t���[�YAlpha�J���[����
				{
					//���`�����l������
					{
						 DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						 DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
					
					//���e�X�g����
					{
						 DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
				//�f�B�t���[�YAlpha�J���[����
				{
					//���`�����l������
					{
						 DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						 DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
					
					//���e�X�g����
					{
						 DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
			},
		},
		//�u�����h�e�N�X�`������ DX_BLENDGRAPHTYPE_WIPE
		{
			//�f�B�t���[�YRGB�J���[����
			{
				//�f�B�t���[�YAlpha�J���[����
				{
					//���`�����l������
					{
						 DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						 DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
					
					//���e�X�g����
					{
						 DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
				//�f�B�t���[�YAlpha�J���[����
				{
					//���`�����l������
					{
						 DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						 DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
					
					//���e�X�g����
					{
						 DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
			},
			//�f�B�t���[�YRGB�J���[����
			{
				//�f�B�t���[�YAlpha�J���[����
				{
					//���`�����l������
					{
						 DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						 DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
					
					//���e�X�g����
					{
						 DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
				//�f�B�t���[�YAlpha�J���[����
				{
					//���`�����l������
					{
						 DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						 DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
					
					//���e�X�g����
					{
						 DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
			},
		},
		//�u�����h�e�N�X�`������ DX_BLENDGRAPHTYPE_ALPHA
		{
			//�f�B�t���[�YRGB�J���[����
			{
				//�f�B�t���[�YAlpha�J���[����
				{
					//���`�����l������
					{
						 DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						 DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
					
					//���e�X�g����
					{
						 DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
				//�f�B�t���[�YAlpha�J���[����
				{
					//���`�����l������
					{
						 DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						 DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
					
					//���e�X�g����
					{
						 DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
			},
			//�f�B�t���[�YRGB�J���[����
			{
				//�f�B�t���[�YAlpha�J���[����
				{
					//���`�����l������
					{
						 DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						 DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
					
					//���e�X�g����
					{
						 DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
				//�f�B�t���[�YAlpha�J���[����
				{
					//���`�����l������
					{
						 DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						 DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
					
					//���e�X�g����
					{
						 DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
			},
		},
	},

	// RGB���]
	{
		// �u�����h�e�N�X�`������
		{
			//�f�B�t���[�YRGB�J���[����
			{
				//�f�B�t���[�YAlpha�J���[����
				{
					//���`�����l������
					{
						/*Stage0*/{__NUM(1), 0, 0,__NONE,__DIFF,__SEL2,	__TEX_ | __COMP,__NONE,__SEL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						/*Stage0*/{__NUM(1), 0, 0,__TEX_,__NONE,__SEL1,	__TEX_ | __COMP,__NONE,__SEL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���e�X�g����
					{
						/*Stage0*/{__NUM(1), 0, 0,__TEX_,__NONE,__SEL1,	__TEX_ | __COMP,__NONE,__SEL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
				//�f�B�t���[�YAlpha�J���[����
				{
					//���`�����l������
					{
						/*Stage0*/{__NUM(1), 0, 0,__NONE,__DIFF,__SEL2,	__TEX_ | __COMP,__NONE,__SEL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						/*Stage0*/{__NUM(1), 0, 0,__TEX_,__DIFF,__MUL1,	__TEX_ | __COMP,__NONE,__SEL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
					
					//���e�X�g����
					{
						/*Stage0*/{__NUM(1), 0, 0,__TEX_,__DIFF,__MUL1,	__TEX_ | __COMP,__NONE,__SEL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
			},
			//�f�B�t���[�YRGB�J���[����
			{
				//�f�B�t���[�YAlpha�J���[�Ȃ�
				{
					//���`�����l������
					{
						/*Stage0*/{__NUM(1), 0, 0, __NONE,__DIFF,__SEL2,	__TEX_ | __COMP,__DIFF,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						/*Stage0*/{__NUM(1), 0, 0, __TEX_,__NONE,__SEL1,	__TEX_ | __COMP,__DIFF,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
					
					//���e�X�g����
					{
						/*Stage0*/{__NUM(1), 0, 0, __TEX_,__NONE,__SEL1,	__TEX_ | __COMP,__DIFF,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
				//�f�B�t���[�YAlpha�J���[����
				{
					//���`�����l������
					{
						/*Stage0*/{__NUM(1), 0, 0, __NONE,__DIFF,__SEL2,	__TEX_ | __COMP,__DIFF,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						/*Stage0*/{__NUM(1), 0, 0, __TEX_,__DIFF,__MUL1,	__TEX_ | __COMP,__DIFF,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
					
					//���e�X�g����
					{
						/*Stage0*/{__NUM(1), 0, 0, __TEX_,__DIFF,__MUL1,	__TEX_ | __COMP,__DIFF,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
			},
		},
		//�u�����h�e�N�X�`������ DX_BLENDGRAPHTYPE_NORMAL
		{
			//�f�B�t���[�YRGB�J���[����
			{
				//�f�B�t���[�YAlpha�J���[����
				{
					//���`�����l������
					{
						/*Stage0*/{__NUM(3), 0, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE,__SEL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__TEX_,__BLDF,	__CURR,__TEX_,__BLDF},
						/*Stage2*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR | __COMP,__DIFF,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						/*Stage0*/{__NUM(3), 0, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE,__SEL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__TEX_,__BLDF,	__CURR,__TEX_,__BLDF},
						/*Stage2*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR | __COMP,__DIFF,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
					
					//���e�X�g����
					{
						/*Stage0*/{__NUM(3), 0, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE,__SEL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__TEX_,__BLDF,	__CURR,__TEX_,__BLDF},
						/*Stage2*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR | __COMP,__DIFF,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
				//�f�B�t���[�YAlpha�J���[����
				{
					//���`�����l������
					{
						/*Stage0*/{__NUM(3), 0, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE,__SEL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__TEX_,__BLDF,	__CURR,__TEX_,__BLDF},
						/*Stage2*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR | __COMP,__DIFF,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						/*Stage0*/{__NUM(3), 0, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE,__SEL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__TEX_,__BLDF,	__CURR,__TEX_,__BLDF},
						/*Stage2*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR | __COMP,__DIFF,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
					
					//���e�X�g����
					{
						/*Stage0*/{__NUM(3), 0, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE,__SEL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__TEX_,__BLDF,	__CURR,__TEX_,__BLDF},
						/*Stage2*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR | __COMP,__DIFF,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
			},
			//�f�B�t���[�YRGB�J���[����
			{
				//�f�B�t���[�YAlpha�J���[����
				{
					//���`�����l������
					{
						/*Stage0*/{__NUM(3), 0, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE,__SEL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__TEX_,__BLDF,	__CURR,__TEX_,__BLDF},
						/*Stage2*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR | __COMP,__DIFF,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						/*Stage0*/{__NUM(3), 0, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE,__SEL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__TEX_,__BLDF,	__CURR,__TEX_,__BLDF},
						/*Stage2*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR | __COMP,__DIFF,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
					
					//���e�X�g����
					{
						/*Stage0*/{__NUM(3), 0, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE,__SEL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__TEX_,__BLDF,	__CURR,__TEX_,__BLDF},
						/*Stage2*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR | __COMP,__DIFF,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
				//�f�B�t���[�YAlpha�J���[����
				{
					//���`�����l������
					{
						/*Stage0*/{__NUM(3), 0, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE,__SEL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__TEX_,__BLDF,	__CURR,__TEX_,__BLDF},
						/*Stage2*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR | __COMP,__DIFF,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						/*Stage0*/{__NUM(3), 0, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE,__SEL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__TEX_,__BLDF,	__CURR,__TEX_,__BLDF},
						/*Stage2*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR | __COMP,__DIFF,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
					
					//���e�X�g����
					{
						/*Stage0*/{__NUM(3), 0, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE,__SEL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__TEX_,__BLDF,	__CURR,__TEX_,__BLDF},
						/*Stage2*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR | __COMP,__DIFF,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
			},
		},
		//�u�����h�e�N�X�`������ DX_BLENDGRAPHTYPE_WIPE
		{
			//�f�B�t���[�YRGB�J���[����
			{
				//�f�B�t���[�YAlpha�J���[����
				{
					//���`�����l������
					{
						/*Stage0*/{__NUM(2), 0, 0, __TEX_,__FACT,__ADD1,	__NONE,__DIFF,__SEL2},
						/*Stage1*/{    NULL, 1, 0, __DIFF,__CURR,__MUL1,	__TEX_ | __COMP,__NONE,__SEL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						/*Stage0*/{__NUM(3), 0, 0, __TEX_,__FACT,__ADD1,	__NONE,__DIFF,__SEL2},
						/*Stage1*/{    NULL, 1, 0, __SPEC,__CURR,__MUL1,	__CURR,__NONE,__SEL1},
						/*Stage2*/{    NULL, 1, 0, __TEX_,__CURR,__MUL1,	__TEX_ | __COMP,__NONE,__SEL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
					
					//���e�X�g����
					{
						/*Stage0*/{__NUM(2), 0, 0, __TEX_,__FACT,__ADD1,	__NONE,__DIFF,__SEL2},
						/*Stage1*/{    NULL, 1, 0, __TEX_,__CURR,__MUL1,	__TEX_ | __COMP,__NONE,__SEL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
				//�f�B�t���[�YAlpha�J���[����
				{
					//���`�����l������
					{
						/*Stage0*/{__NUM(2), 0, 0, __TEX_,__FACT,__ADD1,	__NONE,__DIFF,__SEL2},
						/*Stage1*/{    NULL, 1, 0, __DIFF,__CURR,__MUL1,	__TEX_ | __COMP,__NONE,__SEL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						/*Stage0*/{__NUM(4), 0, 0, __TEX_,__FACT,__ADD1,	__NONE,__DIFF,__SEL2},
						/*Stage1*/{    NULL, 1, 0, __SPEC,__CURR,__MUL1,	__CURR,__NONE,__SEL1},
						/*Stage2*/{    NULL, 1, 0, __TEX_,__CURR,__MUL1,	__TEX_ | __COMP,__NONE,__SEL1},
						/*Stage3*/{    NULL, 0, 0, __DIFF,__CURR,__MUL1,	__CURR,__NONE,__SEL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
					
					//���e�X�g����
					{
						/*Stage0*/{__NUM(3), 0, 0, __TEX_,__FACT,__ADD1,	__NONE,__DIFF,__SEL2},
						/*Stage1*/{    NULL, 1, 0, __TEX_,__CURR,__MUL1,	__TEX_ | __COMP,__NONE,__SEL1},
						/*Stage2*/{    NULL, 1, 0, __DIFF,__CURR,__MUL1,	__CURR,__NONE,__SEL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
			},
			//�f�B�t���[�YRGB�J���[����
			{
				//�f�B�t���[�YAlpha�J���[����
				{
					//���`�����l������
					{
						/*Stage0*/{__NUM(2), 0, 0, __TEX_,__FACT,__ADD1,	__NONE,__DIFF,__SEL2},
						/*Stage1*/{    NULL, 1, 0, __CURR,__DIFF,__MUL1,	__TEX_ | __COMP,__DIFF,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						/*Stage0*/{__NUM(3), 0, 0, __TEX_,__FACT,__ADD1,	__NONE,__DIFF,__SEL2},
						/*Stage1*/{    NULL, 1, 0, __SPEC,__CURR,__MUL1,	__CURR,__NONE,__SEL1},
						/*Stage2*/{    NULL, 1, 0, __TEX_,__CURR,__MUL1,	__TEX_ | __COMP,__DIFF,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
					
					//���e�X�g����
					{
						/*Stage0*/{__NUM(2), 0, 0, __TEX_,__FACT,__ADD1,	__NONE,__DIFF,__SEL2},
						/*Stage1*/{    NULL, 1, 0, __TEX_,__CURR,__MUL1,	__TEX_ | __COMP,__DIFF,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
				//�f�B�t���[�YAlpha�J���[����
				{
					//���`�����l������
					{
						/*Stage0*/{__NUM(2), 0, 0, __TEX_,__FACT,__ADD1,	__NONE,__DIFF,__SEL2},
						/*Stage1*/{    NULL, 1, 0, __DIFF,__CURR,__MUL1,	__TEX_ | __COMP,__DIFF,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						/*Stage0*/{__NUM(4), 0, 0, __TEX_,__FACT,__ADD1,	__NONE,__DIFF,__SEL2},
						/*Stage1*/{    NULL, 1, 0, __SPEC,__CURR,__MUL1,	__CURR,__NONE,__SEL1},
						/*Stage2*/{    NULL, 1, 0, __TEX_,__CURR,__MUL1,	__TEX_ | __COMP,__DIFF,__MUL1},
						/*Stage3*/{    NULL, 0, 0, __DIFF,__CURR,__MUL1,	__CURR,__NONE,__SEL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
					
					//���e�X�g����
					{
						/*Stage0*/{__NUM(3), 0, 0, __TEX_,__FACT,__ADD1,	__NONE,__DIFF,__SEL2},
						/*Stage1*/{    NULL, 1, 0, __TEX_,__CURR,__MUL1,	__TEX_ | __COMP,__DIFF,__MUL1},
						/*Stage2*/{    NULL, 1, 0, __DIFF,__CURR,__MUL1,	__CURR,__NONE,__SEL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
			},
		},
		//�u�����h�e�N�X�`������ DX_BLENDGRAPHTYPE_ALPHA
		{
			//�f�B�t���[�YRGB�J���[����
			{
				//�f�B�t���[�YAlpha�J���[����
				{
					//���`�����l������
					{
						/*Stage0*/{__NUM(3), 0, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE,__SEL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__TEX_,__MUL1,	__CURR,__NONE,__SEL1},
						/*Stage2*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR | __COMP,__DIFF,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						/*Stage0*/{__NUM(3), 0, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE,__SEL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__TEX_,__MUL1,	__CURR,__NONE,__SEL1},
						/*Stage2*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR | __COMP,__DIFF,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
					
					//���e�X�g����
					{
						/*Stage0*/{__NUM(3), 0, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE,__SEL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__TEX_,__MUL1,	__CURR,__NONE,__SEL1},
						/*Stage2*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR | __COMP,__DIFF,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
				//�f�B�t���[�YAlpha�J���[����
				{
					//���`�����l������
					{
						/*Stage0*/{__NUM(3), 0, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE,__SEL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__TEX_,__MUL1,	__CURR,__NONE,__SEL1},
						/*Stage2*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR | __COMP,__DIFF,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						/*Stage0*/{__NUM(3), 0, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE,__SEL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__TEX_,__MUL1,	__CURR,__NONE,__SEL1},
						/*Stage2*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR | __COMP,__DIFF,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
					
					//���e�X�g����
					{
						/*Stage0*/{__NUM(3), 0, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE,__SEL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__TEX_,__MUL1,	__CURR,__NONE,__SEL1},
						/*Stage2*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR | __COMP,__DIFF,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
			},
			//�f�B�t���[�YRGB�J���[����
			{
				//�f�B�t���[�YAlpha�J���[����
				{
					//���`�����l������
					{
						/*Stage0*/{__NUM(3), 0, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE,__SEL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__TEX_,__MUL1,	__CURR,__NONE,__SEL1},
						/*Stage2*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR | __COMP,__DIFF,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						/*Stage0*/{__NUM(3), 0, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE,__SEL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__TEX_,__MUL1,	__CURR,__NONE,__SEL1},
						/*Stage2*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR | __COMP,__DIFF,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
					
					//���e�X�g����
					{
						/*Stage0*/{__NUM(3), 0, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE,__SEL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__TEX_,__MUL1,	__CURR,__NONE,__SEL1},
						/*Stage2*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR | __COMP,__DIFF,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
				//�f�B�t���[�YAlpha�J���[����
				{
					//���`�����l������
					{
						/*Stage0*/{__NUM(3), 0, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE,__SEL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__TEX_,__MUL1,	__CURR,__NONE,__SEL1},
						/*Stage2*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR | __COMP,__DIFF,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						/*Stage0*/{__NUM(3), 0, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE,__SEL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__TEX_,__MUL1,	__CURR,__NONE,__SEL1},
						/*Stage2*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR | __COMP,__DIFF,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
					
					//���e�X�g����
					{
						/*Stage0*/{__NUM(3), 0, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE,__SEL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__TEX_,__MUL1,	__CURR,__NONE,__SEL1},
						/*Stage2*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR | __COMP,__DIFF,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
			},
		},
	},

	// �`��P�x�S�{
	{
		// �u�����h�e�N�X�`������
		{
			//�f�B�t���[�YRGB�J���[����
			{
				//�f�B�t���[�YAlpha�J���[����
				{
					//���`�����l������
					{
						/*Stage0*/{__NUM(1), 0, 0,__NONE,__DIFF,__SEL2,	__TEX_,__DIFF,__MUL4}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						/*Stage0*/{__NUM(1), 0, 0,__TEX_,__NONE,__SEL1,	__TEX_,__DIFF,__MUL4}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���e�X�g����
					{
						/*Stage0*/{__NUM(1), 0, 0,__TEX_,__NONE,__SEL1,	__TEX_,__DIFF,__MUL4}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
				//�f�B�t���[�YAlpha�J���[����
				{
					//���`�����l������
					{
						/*Stage0*/{__NUM(1), 0, 0,__NONE,__DIFF,__SEL2,	__TEX_,__DIFF,__MUL4}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						/*Stage0*/{__NUM(1), 0, 0,__TEX_,__DIFF,__MUL1,	__TEX_,__DIFF,__MUL4}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
					
					//���e�X�g����
					{
						/*Stage0*/{__NUM(1), 0, 0,__TEX_,__DIFF,__MUL1,	__TEX_,__DIFF,__MUL4}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
			},
			//�f�B�t���[�YRGB�J���[����
			{
				//�f�B�t���[�YAlpha�J���[�Ȃ�
				{
					//���`�����l������
					{
						/*Stage0*/{__NUM(1), 0, 0,__NONE,__DIFF,__SEL2,	__TEX_,__DIFF,__MUL4}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						/*Stage0*/{__NUM(1), 0, 0,__TEX_,__NONE,__SEL1,	__TEX_,__DIFF,__MUL4}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
					
					//���e�X�g����
					{
						/*Stage0*/{__NUM(1), 0, 0,__TEX_,__NONE,__SEL1,	__TEX_,__DIFF,__MUL4}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
				//�f�B�t���[�YAlpha�J���[����
				{
					//���`�����l������
					{
						/*Stage0*/{__NUM(1), 0, 0,__NONE,__DIFF,__SEL2,	__TEX_,__DIFF,__MUL4}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						/*Stage0*/{__NUM(1), 0, 0,__TEX_,__DIFF,__MUL1,	__TEX_,__DIFF,__MUL4}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
					
					//���e�X�g����
					{
						/*Stage0*/{__NUM(1), 0, 0,__TEX_,__DIFF,__MUL1,	__TEX_,__DIFF,__MUL4}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
			},
		},
		//�u�����h�e�N�X�`������ DX_BLENDGRAPHTYPE_NORMAL
		{
			//�f�B�t���[�YRGB�J���[����
			{
				//�f�B�t���[�YAlpha�J���[����
				{
					//���`�����l������
					{
						/*Stage0*/{__NUM(5), 1, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE,__SEL1},
						/*Stage1*/{    NULL, 0, 0, __CURR,__TEX_,__SUB1,	__CURR,__TEX_,__SUB1},
						/*Stage2*/{    NULL, 0, 0, __CURR,__FACT,__MUL1,	__CURR,__FACT,__MUL1},
						/*Stage3*/{    NULL, 0, 0, __CURR,__TEX_,__ADD1,	__CURR,__TEX_,__ADD1},
						/*Stage4*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR,__DIFF,__MUL4}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						/*Stage0*/{__NUM(5), 1, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE,__SEL1},
						/*Stage1*/{    NULL, 0, 0, __CURR,__TEX_,__SUB1,	__CURR,__TEX_,__SUB1},
						/*Stage2*/{    NULL, 0, 0, __CURR,__FACT,__MUL1,	__CURR,__FACT,__MUL1},
						/*Stage3*/{    NULL, 0, 0, __CURR,__TEX_,__ADD1,	__CURR,__TEX_,__ADD1},
						/*Stage4*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR,__DIFF,__MUL4}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
					
					//���e�X�g����
					{
						/*Stage0*/{__NUM(5), 1, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE,__SEL1},
						/*Stage1*/{    NULL, 0, 0, __CURR,__TEX_,__SUB1,	__CURR,__TEX_,__SUB1},
						/*Stage2*/{    NULL, 0, 0, __CURR,__FACT,__MUL1,	__CURR,__FACT,__MUL1},
						/*Stage3*/{    NULL, 0, 0, __CURR,__TEX_,__ADD1,	__CURR,__TEX_,__ADD1},
						/*Stage4*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR,__DIFF,__MUL4}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
				//�f�B�t���[�YAlpha�J���[����
				{
					//���`�����l������
					{
						/*Stage0*/{__NUM(5), 1, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE,__SEL1},
						/*Stage1*/{    NULL, 0, 0, __CURR,__TEX_,__SUB1,	__CURR,__TEX_,__SUB1},
						/*Stage2*/{    NULL, 0, 0, __CURR,__FACT,__MUL1,	__CURR,__FACT,__MUL1},
						/*Stage3*/{    NULL, 0, 0, __CURR,__TEX_,__ADD1,	__CURR,__TEX_,__ADD1},
						/*Stage4*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR,__DIFF,__MUL4}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						/*Stage0*/{__NUM(5), 1, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE,__SEL1},
						/*Stage1*/{    NULL, 0, 0, __CURR,__TEX_,__SUB1,	__CURR,__TEX_,__SUB1},
						/*Stage2*/{    NULL, 0, 0, __CURR,__FACT,__MUL1,	__CURR,__FACT,__MUL1},
						/*Stage3*/{    NULL, 0, 0, __CURR,__TEX_,__ADD1,	__CURR,__TEX_,__ADD1},
						/*Stage4*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR,__DIFF,__MUL4}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
					
					//���e�X�g����
					{
						/*Stage0*/{__NUM(5), 1, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE,__SEL1},
						/*Stage1*/{    NULL, 0, 0, __CURR,__TEX_,__SUB1,	__CURR,__TEX_,__SUB1},
						/*Stage2*/{    NULL, 0, 0, __CURR,__FACT,__MUL1,	__CURR,__FACT,__MUL1},
						/*Stage3*/{    NULL, 0, 0, __CURR,__TEX_,__ADD1,	__CURR,__TEX_,__ADD1},
						/*Stage4*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR,__DIFF,__MUL4}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
			},
			//�f�B�t���[�YRGB�J���[����
			{
				//�f�B�t���[�YAlpha�J���[����
				{
					//���`�����l������
					{
						/*Stage0*/{__NUM(5), 1, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE,__SEL1},
						/*Stage1*/{    NULL, 0, 0, __CURR,__TEX_,__SUB1,	__CURR,__TEX_,__SUB1},
						/*Stage2*/{    NULL, 0, 0, __CURR,__FACT,__MUL1,	__CURR,__FACT,__MUL1},
						/*Stage3*/{    NULL, 0, 0, __CURR,__TEX_,__ADD1,	__CURR,__TEX_,__ADD1},
						/*Stage4*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR,__DIFF,__MUL4}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						/*Stage0*/{__NUM(5), 1, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE,__SEL1},
						/*Stage1*/{    NULL, 0, 0, __CURR,__TEX_,__SUB1,	__CURR,__TEX_,__SUB1},
						/*Stage2*/{    NULL, 0, 0, __CURR,__FACT,__MUL1,	__CURR,__FACT,__MUL1},
						/*Stage3*/{    NULL, 0, 0, __CURR,__TEX_,__ADD1,	__CURR,__TEX_,__ADD1},
						/*Stage4*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR,__DIFF,__MUL4}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
					
					//���e�X�g����
					{
						/*Stage0*/{__NUM(5), 1, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE,__SEL1},
						/*Stage1*/{    NULL, 0, 0, __CURR,__TEX_,__SUB1,	__CURR,__TEX_,__SUB1},
						/*Stage2*/{    NULL, 0, 0, __CURR,__FACT,__MUL1,	__CURR,__FACT,__MUL1},
						/*Stage3*/{    NULL, 0, 0, __CURR,__TEX_,__ADD1,	__CURR,__TEX_,__ADD1},
						/*Stage4*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR,__DIFF,__MUL4}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
				//�f�B�t���[�YAlpha�J���[����
				{
					//���`�����l������
					{
						/*Stage0*/{__NUM(5), 1, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE,__SEL1},
						/*Stage1*/{    NULL, 0, 0, __CURR,__TEX_,__SUB1,	__CURR,__TEX_,__SUB1},
						/*Stage2*/{    NULL, 0, 0, __CURR,__FACT,__MUL1,	__CURR,__FACT,__MUL1},
						/*Stage3*/{    NULL, 0, 0, __CURR,__TEX_,__ADD1,	__CURR,__TEX_,__ADD1},
						/*Stage4*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR,__DIFF,__MUL4}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						/*Stage0*/{__NUM(5), 1, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE,__SEL1},
						/*Stage1*/{    NULL, 0, 0, __CURR,__TEX_,__SUB1,	__CURR,__TEX_,__SUB1},
						/*Stage2*/{    NULL, 0, 0, __CURR,__FACT,__MUL1,	__CURR,__FACT,__MUL1},
						/*Stage3*/{    NULL, 0, 0, __CURR,__TEX_,__ADD1,	__CURR,__TEX_,__ADD1},
						/*Stage4*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR,__DIFF,__MUL4}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
					
					//���e�X�g����
					{
						/*Stage0*/{__NUM(5), 1, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE,__SEL1},
						/*Stage1*/{    NULL, 0, 0, __CURR,__TEX_,__SUB1,	__CURR,__TEX_,__SUB1},
						/*Stage2*/{    NULL, 0, 0, __CURR,__FACT,__MUL1,	__CURR,__FACT,__MUL1},
						/*Stage3*/{    NULL, 0, 0, __CURR,__TEX_,__ADD1,	__CURR,__TEX_,__ADD1},
						/*Stage4*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR,__DIFF,__MUL4}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
			},
		},
		//�u�����h�e�N�X�`������ DX_BLENDGRAPHTYPE_WIPE
		{
			//�f�B�t���[�YRGB�J���[����
			{
				//�f�B�t���[�YAlpha�J���[����
				{
					//���`�����l������
					{
						/*Stage0*/{__NUM(2), 0, 0, __TEX_,__FACT,__ADD1,	__NONE,__DIFF,__SEL2},
						/*Stage1*/{    NULL, 1, 0, __CURR,__DIFF,__MUL1,	__TEX_,__DIFF,__MUL4}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						/*Stage0*/{__NUM(3), 0, 0, __TEX_,__FACT,__ADD1,	__NONE,__DIFF,__SEL2},
						/*Stage1*/{    NULL, 1, 0, __SPEC,__CURR,__MUL1,	__CURR,__NONE,__SEL1},
						/*Stage2*/{    NULL, 1, 0, __TEX_,__CURR,__MUL1,	__TEX_,__DIFF,__MUL4}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
					
					//���e�X�g����
					{
						/*Stage0*/{__NUM(2), 0, 0, __TEX_,__FACT,__ADD1,	__NONE,__DIFF,__SEL2},
						/*Stage1*/{    NULL, 1, 0, __TEX_,__CURR,__MUL1,	__TEX_,__DIFF,__MUL4}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
				//�f�B�t���[�YAlpha�J���[����
				{
					//���`�����l������
					{
						/*Stage0*/{__NUM(2), 0, 0, __TEX_,__FACT,__ADD1,	__NONE,__DIFF,__SEL2},
						/*Stage1*/{    NULL, 1, 0, __DIFF,__CURR,__MUL1,	__TEX_,__DIFF,__MUL4}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						/*Stage0*/{__NUM(4), 0, 0, __TEX_,__FACT,__ADD1,	__NONE,__DIFF,__SEL2},
						/*Stage1*/{    NULL, 1, 0, __SPEC,__CURR,__MUL1,	__CURR,__NONE,__SEL1},
						/*Stage2*/{    NULL, 1, 0, __TEX_,__CURR,__MUL1,	__TEX_,__DIFF,__MUL4},
						/*Stage3*/{    NULL, 0, 0, __DIFF,__CURR,__MUL1,	__CURR,__NONE,__SEL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
					
					//���e�X�g����
					{
						/*Stage0*/{__NUM(3), 0, 0, __TEX_,__FACT,__ADD1,	__NONE,__DIFF,__SEL2},
						/*Stage1*/{    NULL, 1, 0, __TEX_,__CURR,__MUL1,	__TEX_,__DIFF,__MUL4},
						/*Stage2*/{    NULL, 1, 0, __DIFF,__CURR,__MUL1,	__CURR,__NONE,__SEL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
			},
			//�f�B�t���[�YRGB�J���[����
			{
				//�f�B�t���[�YAlpha�J���[����
				{
					//���`�����l������
					{
						/*Stage0*/{__NUM(2), 0, 0, __TEX_,__FACT,__ADD1,	__NONE,__DIFF,__SEL2},
						/*Stage1*/{    NULL, 1, 0, __CURR,__DIFF,__MUL1,	__TEX_,__DIFF,__MUL4}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						/*Stage0*/{__NUM(3), 0, 0, __TEX_,__FACT,__ADD1,	__NONE,__DIFF,__SEL2},
						/*Stage1*/{    NULL, 1, 0, __SPEC,__CURR,__MUL1,	__CURR,__NONE,__SEL1},
						/*Stage2*/{    NULL, 1, 0, __TEX_,__CURR,__MUL1,	__TEX_,__DIFF,__MUL4}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
					
					//���e�X�g����
					{
						/*Stage0*/{__NUM(2), 0, 0, __TEX_,__FACT,__ADD1,	__NONE,__DIFF,__SEL2},
						/*Stage1*/{    NULL, 1, 0, __TEX_,__CURR,__MUL1,	__TEX_,__DIFF,__MUL4}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
				//�f�B�t���[�YAlpha�J���[����
				{
					//���`�����l������
					{
						/*Stage0*/{__NUM(2), 0, 0, __TEX_,__FACT,__ADD1,	__NONE,__DIFF,__SEL2},
						/*Stage1*/{    NULL, 1, 0, __DIFF,__CURR,__MUL1,	__TEX_,__DIFF,__MUL4}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						/*Stage0*/{__NUM(4), 0, 0, __TEX_,__FACT,__ADD1,	__NONE,__DIFF,__SEL2},
						/*Stage1*/{    NULL, 1, 0, __SPEC,__CURR,__MUL1,	__CURR,__NONE,__SEL1},
						/*Stage2*/{    NULL, 1, 0, __TEX_,__CURR,__MUL1,	__TEX_,__DIFF,__MUL4},
						/*Stage3*/{    NULL, 0, 0, __DIFF,__CURR,__MUL1,	__CURR,__NONE,__SEL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
					
					//���e�X�g����
					{
						/*Stage0*/{__NUM(3), 0, 0, __TEX_,__FACT,__ADD1,	__NONE,__DIFF,__SEL2},
						/*Stage1*/{    NULL, 1, 0, __TEX_,__CURR,__MUL1,	__TEX_,__DIFF,__MUL4},
						/*Stage2*/{    NULL, 1, 0, __DIFF,__CURR,__MUL1,	__CURR,__NONE,__SEL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
			},
		},
		//�u�����h�e�N�X�`������ DX_BLENDGRAPHTYPE_ALPHA
		{
			//�f�B�t���[�YRGB�J���[����
			{
				//�f�B�t���[�YAlpha�J���[����
				{
					//���`�����l������
					{
						/*Stage0*/{__NUM(2), 0, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE,__SEL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__TEX_,__MUL1,	__CURR,__DIFF,__MUL4}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						/*Stage0*/{__NUM(2), 0, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE,__SEL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__TEX_,__MUL1,	__CURR,__DIFF,__MUL4}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
					
					//���e�X�g����
					{
						/*Stage0*/{__NUM(2), 0, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE,__SEL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__TEX_,__MUL1,	__CURR,__DIFF,__MUL4}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
				//�f�B�t���[�YAlpha�J���[����
				{
					//���`�����l������
					{
						/*Stage0*/{__NUM(3), 0, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE,__SEL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__TEX_,__MUL1,	__CURR,__NONE,__SEL1},
						/*Stage2*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR,__DIFF,__MUL4}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						/*Stage0*/{__NUM(3), 0, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE,__SEL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__TEX_,__MUL1,	__CURR,__NONE,__SEL1},
						/*Stage2*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR,__DIFF,__MUL4}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
					
					//���e�X�g����
					{
						/*Stage0*/{__NUM(3), 0, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE,__SEL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__TEX_,__MUL1,	__CURR,__NONE,__SEL1},
						/*Stage2*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR,__DIFF,__MUL4}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
			},
			//�f�B�t���[�YRGB�J���[����
			{
				//�f�B�t���[�YAlpha�J���[����
				{
					//���`�����l������
					{
						/*Stage0*/{__NUM(3), 0, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE,__SEL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__TEX_,__MUL1,	__CURR,__NONE,__SEL1},
						/*Stage2*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR,__DIFF,__MUL4}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						/*Stage0*/{__NUM(3), 0, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE,__SEL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__TEX_,__MUL1,	__CURR,__NONE,__SEL1},
						/*Stage2*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR,__DIFF,__MUL4}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
					
					//���e�X�g����
					{
						/*Stage0*/{__NUM(3), 0, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE,__SEL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__TEX_,__MUL1,	__CURR,__NONE,__SEL1},
						/*Stage2*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR,__DIFF,__MUL4}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
				//�f�B�t���[�YAlpha�J���[����
				{
					//���`�����l������
					{
						/*Stage0*/{__NUM(3), 0, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE,__SEL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__TEX_,__MUL1,	__CURR,__NONE,__SEL1},
						/*Stage2*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR,__DIFF,__MUL4}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						/*Stage0*/{__NUM(3), 0, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE,__SEL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__TEX_,__MUL1,	__CURR,__NONE,__SEL1},
						/*Stage2*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR,__DIFF,__MUL4}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
					
					//���e�X�g����
					{
						/*Stage0*/{__NUM(3), 0, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE,__SEL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__TEX_,__MUL1,	__CURR,__NONE,__SEL1},
						/*Stage2*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR,__DIFF,__MUL4}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
			},
		},
	},

	// ��Z�ς݃��u�����h���[�h�̒ʏ�`��
	{
		// �u�����h�e�N�X�`������
		{
			//�f�B�t���[�YRGB�J���[����
			{
				//�f�B�t���[�YAlpha�J���[����
				{
					//���`�����l������
					{
						/*Stage0*/{__NUM(1), 0, 0,__NONE,__DIFF,__SEL2,	__TEX_,__NONE,__SEL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						/*Stage0*/{__NUM(1), 0, 0,__TEX_,__NONE,__SEL1,	__TEX_,__NONE,__SEL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���e�X�g����
					{
						/*Stage0*/{__NUM(1), 0, 0,__TEX_,__NONE,__SEL1,	__TEX_,__NONE,__SEL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
				//�f�B�t���[�YAlpha�J���[����
				{
					//���`�����l������
					{
						/*Stage0*/{__NUM(1), 0, 0,__NONE,__DIFF,__SEL2,	__TEX_,__DIFF | __ALRP,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						/*Stage0*/{__NUM(1), 0, 0,__TEX_,__DIFF,__MUL1,	__TEX_,__DIFF | __ALRP,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
					
					//���e�X�g����
					{
						/*Stage0*/{__NUM(1), 0, 0,__TEX_,__DIFF,__MUL1,	__TEX_,__DIFF | __ALRP,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
			},
			//�f�B�t���[�YRGB�J���[����
			{
				//�f�B�t���[�YAlpha�J���[�Ȃ�
				{
					//���`�����l������
					{
						/*Stage0*/{__NUM(1), 0, 0,__NONE,__DIFF,__SEL2,	__TEX_,__DIFF,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						/*Stage0*/{__NUM(1), 0, 0,__TEX_,__NONE,__SEL1,	__TEX_,__DIFF,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
					
					//���e�X�g����
					{
						/*Stage0*/{__NUM(1), 0, 0,__TEX_,__NONE,__SEL1,	__TEX_,__DIFF,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
				//�f�B�t���[�YAlpha�J���[����
				{
					//���`�����l������
					{
						/*Stage0*/{__NUM(2), 0, 0,__NONE,__DIFF,__SEL2,	__TEX_,__DIFF         ,__MUL1},
						/*Stage1*/{       0, 0, 0,__CURR,__NONE,__SEL1,	__CURR,__DIFF | __ALRP,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						/*Stage0*/{__NUM(2), 0, 0,__TEX_,__DIFF,__MUL1,	__TEX_,__DIFF         ,__MUL1},
						/*Stage1*/{       0, 0, 0,__CURR,__NONE,__SEL1,	__CURR,__DIFF | __ALRP,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
					
					//���e�X�g����
					{
						/*Stage0*/{__NUM(2), 0, 0,__TEX_,__DIFF,__MUL1,	__TEX_,__DIFF         ,__MUL1},
						/*Stage1*/{       0, 0, 0,__CURR,__NONE,__SEL1,	__CURR,__DIFF | __ALRP,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
			},
		},
		//�u�����h�e�N�X�`������ DX_BLENDGRAPHTYPE_NORMAL
		{
			//�f�B�t���[�YRGB�J���[����
			{
				//�f�B�t���[�YAlpha�J���[����
				{
					//���`�����l������
					{
						/*Stage0*/{__NUM(2), 0, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE,__SEL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__TEX_,__BLDF,	__CURR,__TEX_,__BLDF}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						/*Stage0*/{__NUM(2), 0, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE,__SEL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__TEX_,__BLDF,	__CURR,__TEX_,__BLDF}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
					
					//���e�X�g����
					{
						/*Stage0*/{__NUM(2), 0, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE,__SEL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__TEX_,__BLDF,	__CURR,__TEX_,__BLDF}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
				//�f�B�t���[�YAlpha�J���[����
				{
					//���`�����l������
					{
						/*Stage0*/{__NUM(3), 0, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE         ,__SEL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__TEX_,__BLDF,	__CURR,__TEX_         ,__BLDF},
						/*Stage2*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR,__DIFF | __ALRP,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						/*Stage0*/{__NUM(3), 0, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE         ,__SEL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__TEX_,__BLDF,	__CURR,__TEX_         ,__BLDF},
						/*Stage2*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR,__DIFF | __ALRP,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
					
					//���e�X�g����
					{
						/*Stage0*/{__NUM(3), 0, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE         ,__SEL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__TEX_,__BLDF,	__CURR,__TEX_         ,__BLDF},
						/*Stage2*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR,__DIFF | __ALRP,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
			},
			//�f�B�t���[�YRGB�J���[����
			{
				//�f�B�t���[�YAlpha�J���[����
				{
					//���`�����l������
					{
						/*Stage0*/{__NUM(3), 0, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE,__SEL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__TEX_,__BLDF,	__CURR,__TEX_,__BLDF},
						/*Stage2*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR,__DIFF,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						/*Stage0*/{__NUM(3), 0, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE,__SEL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__TEX_,__BLDF,	__CURR,__TEX_,__BLDF},
						/*Stage2*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR,__DIFF,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
					
					//���e�X�g����
					{
						/*Stage0*/{__NUM(3), 0, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE,__SEL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__TEX_,__BLDF,	__CURR,__TEX_,__BLDF},
						/*Stage2*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR,__DIFF,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
				//�f�B�t���[�YAlpha�J���[����
				{
					//���`�����l������
					{
						/*Stage0*/{__NUM(4), 0, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE         ,__SEL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__TEX_,__BLDF,	__CURR,__TEX_         ,__BLDF},
						/*Stage2*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR,__DIFF         ,__MUL1}, 
						/*Stage3*/{    NULL, 0, 0, __CURR,__NONE,__SEL1,	__CURR,__DIFF | __ALRP,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						/*Stage0*/{__NUM(4), 0, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE         ,__SEL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__TEX_,__BLDF,	__CURR,__TEX_         ,__BLDF},
						/*Stage2*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR,__DIFF         ,__MUL1},
						/*Stage3*/{    NULL, 0, 0, __CURR,__NONE,__SEL1,	__CURR,__DIFF | __ALRP,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
					
					//���e�X�g����
					{
						/*Stage0*/{__NUM(4), 0, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE         ,__SEL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__TEX_,__BLDF,	__CURR,__TEX_         ,__BLDF},
						/*Stage2*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR,__DIFF         ,__MUL1},
						/*Stage3*/{    NULL, 0, 0, __CURR,__NONE,__SEL1,	__CURR,__DIFF | __ALRP,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
			},
		},
		//�u�����h�e�N�X�`������ DX_BLENDGRAPHTYPE_WIPE
		{
			//�f�B�t���[�YRGB�J���[����
			{
				//�f�B�t���[�YAlpha�J���[����
				{
					//���`�����l������
					{
						/*Stage0*/{__NUM(2), 0, 0, __TEX_,__FACT,__MUL1,	__NONE,__DIFF,__SEL2},
						/*Stage1*/{    NULL, 1, 0, __CURR,__DIFF,__MUL1,	__TEX_,__NONE,__SEL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						/*Stage0*/{__NUM(3), 0, 0, __TEX_,__FACT,__ADD1,	__NONE,__DIFF,__SEL2},
						/*Stage1*/{    NULL, 1, 0, __SPEC,__CURR,__MUL1,	__CURR,__NONE,__SEL1},
						/*Stage2*/{    NULL, 1, 0, __TEX_,__CURR,__MUL1,	__TEX_,__NONE,__SEL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
					
					//���e�X�g����
					{
						/*Stage0*/{__NUM(2), 0, 0, __TEX_,__FACT,__ADD1,	__NONE,__DIFF,__SEL2},
						/*Stage1*/{    NULL, 1, 0, __TEX_,__CURR,__MUL1,	__TEX_,__NONE,__SEL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
				//�f�B�t���[�YAlpha�J���[����
				{
					//���`�����l������
					{
						/*Stage0*/{__NUM(2), 0, 0, __TEX_,__FACT,__ADD1,	__NONE,__DIFF         ,__SEL2},
						/*Stage1*/{    NULL, 1, 0, __DIFF,__CURR,__MUL1,	__TEX_,__DIFF | __ALRP,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						/*Stage0*/{__NUM(4), 0, 0, __TEX_,__FACT,__ADD1,	__NONE,__DIFF         ,__SEL2},
						/*Stage1*/{    NULL, 1, 0, __SPEC,__CURR,__MUL1,	__CURR,__NONE         ,__SEL1},
						/*Stage2*/{    NULL, 1, 0, __TEX_,__CURR,__MUL1,	__TEX_,__DIFF | __ALRP,__MUL1},
						/*Stage3*/{    NULL, 0, 0, __DIFF,__CURR,__MUL1,	__CURR,__NONE         ,__SEL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
					
					//���e�X�g����
					{
						/*Stage0*/{__NUM(3), 0, 0, __TEX_,__FACT,__ADD1,	__NONE,__DIFF         ,__SEL2},
						/*Stage1*/{    NULL, 1, 0, __TEX_,__CURR,__MUL1,	__TEX_,__DIFF | __ALRP,__MUL1},
						/*Stage2*/{    NULL, 1, 0, __DIFF,__CURR,__MUL1,	__CURR,__NONE         ,__SEL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
			},
			//�f�B�t���[�YRGB�J���[����
			{
				//�f�B�t���[�YAlpha�J���[����
				{
					//���`�����l������
					{
						/*Stage0*/{__NUM(2), 0, 0, __TEX_,__FACT,__ADD1,	__NONE,__DIFF,__SEL2},
						/*Stage1*/{    NULL, 1, 0, __CURR,__DIFF,__MUL1,	__TEX_,__DIFF,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						/*Stage0*/{__NUM(3), 0, 0, __TEX_,__FACT,__ADD1,	__NONE,__DIFF,__SEL2},
						/*Stage1*/{    NULL, 1, 0, __SPEC,__CURR,__MUL1,	__CURR,__NONE,__SEL1},
						/*Stage2*/{    NULL, 1, 0, __TEX_,__CURR,__MUL1,	__TEX_,__DIFF,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
					
					//���e�X�g����
					{
						/*Stage0*/{__NUM(2), 0, 0, __TEX_,__FACT,__ADD1,	__NONE,__DIFF,__SEL2},
						/*Stage1*/{    NULL, 1, 0, __TEX_,__CURR,__MUL1,	__TEX_,__DIFF,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
				//�f�B�t���[�YAlpha�J���[����
				{
					//���`�����l������
					{
						/*Stage0*/{__NUM(3), 0, 0, __TEX_,__FACT,__ADD1,	__NONE,__DIFF         ,__SEL2},
						/*Stage1*/{    NULL, 1, 0, __DIFF,__CURR,__MUL1,	__TEX_,__DIFF         ,__MUL1},
						/*Stage2*/{    NULL, 0, 0, __CURR,__NONE,__SEL1,	__CURR,__DIFF | __ALRP,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						/*Stage0*/{__NUM(4), 0, 0, __TEX_,__FACT,__ADD1,	__NONE,__DIFF         ,__SEL2},
						/*Stage1*/{    NULL, 1, 0, __SPEC,__CURR,__MUL1,	__CURR,__NONE         ,__SEL1},
						/*Stage2*/{    NULL, 1, 0, __TEX_,__CURR,__MUL1,	__TEX_,__DIFF         ,__MUL1},
						/*Stage3*/{    NULL, 0, 0, __DIFF,__CURR,__MUL1,	__CURR,__DIFF | __ALRP,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
					
					//���e�X�g����
					{
						/*Stage0*/{__NUM(3), 0, 0, __TEX_,__FACT,__ADD1,	__NONE,__DIFF         ,__SEL2},
						/*Stage1*/{    NULL, 1, 0, __TEX_,__CURR,__MUL1,	__TEX_,__DIFF         ,__MUL1},
						/*Stage2*/{    NULL, 1, 0, __DIFF,__CURR,__MUL1,	__CURR,__DIFF | __ALRP,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
			},
		},
		//�u�����h�e�N�X�`������ DX_BLENDGRAPHTYPE_ALPHA
		{
			//�f�B�t���[�YRGB�J���[����
			{
				//�f�B�t���[�YAlpha�J���[����
				{
					//���`�����l������
					{
						/*Stage0*/{__NUM(2), 1, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE,__SEL1},
						/*Stage1*/{    NULL, 0, 0, __CURR,__TEX_,__MUL1,	__CURR,__NONE,__SEL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						/*Stage0*/{__NUM(2), 1, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE,__SEL1},
						/*Stage1*/{    NULL, 0, 0, __CURR,__TEX_,__MUL1,	__CURR,__NONE,__SEL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
					
					//���e�X�g����
					{
						/*Stage0*/{__NUM(2), 1, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE,__SEL1},
						/*Stage1*/{    NULL, 0, 0, __CURR,__TEX_,__MUL1,	__CURR,__NONE,__SEL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
				//�f�B�t���[�YAlpha�J���[����
				{
					//���`�����l������
					{
						/*Stage0*/{__NUM(3), 1, 0, __TEX_,__NONE,__SEL1,	__TEX_,__DIFF | __ALRP,__MUL1},
						/*Stage1*/{    NULL, 0, 0, __CURR,__TEX_,__MUL1,	__CURR,__NONE         ,__SEL1},
						/*Stage2*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR,__DIFF         ,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						/*Stage0*/{__NUM(3), 1, 0, __TEX_,__NONE,__SEL1,	__TEX_,__DIFF | __ALRP,__MUL1},
						/*Stage1*/{    NULL, 0, 0, __CURR,__TEX_,__MUL1,	__CURR,__NONE         ,__SEL1},
						/*Stage2*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR,__DIFF         ,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
					
					//���e�X�g����
					{
						/*Stage0*/{__NUM(3), 1, 0, __TEX_,__NONE,__SEL1,	__TEX_,__DIFF | __ALRP,__MUL1},
						/*Stage1*/{    NULL, 0, 0, __CURR,__TEX_,__MUL1,	__CURR,__NONE         ,__SEL1},
						/*Stage2*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR,__DIFF         ,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
			},
			//�f�B�t���[�YRGB�J���[����
			{
				//�f�B�t���[�YAlpha�J���[����
				{
					//���`�����l������
					{
						/*Stage0*/{__NUM(3), 1, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE,__SEL1},
						/*Stage1*/{    NULL, 0, 0, __CURR,__TEX_,__MUL1,	__CURR,__NONE,__SEL1},
						/*Stage2*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR,__DIFF,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						/*Stage0*/{__NUM(3), 1, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE,__SEL1},
						/*Stage1*/{    NULL, 0, 0, __CURR,__TEX_,__MUL1,	__CURR,__NONE,__SEL1},
						/*Stage2*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR,__DIFF,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
					
					//���e�X�g����
					{
						/*Stage0*/{__NUM(3), 1, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE,__SEL1},
						/*Stage1*/{    NULL, 0, 0, __CURR,__TEX_,__MUL1,	__CURR,__NONE,__SEL1},
						/*Stage2*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR,__DIFF,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
				//�f�B�t���[�YAlpha�J���[����
				{
					//���`�����l������
					{
						/*Stage0*/{__NUM(3), 1, 0, __TEX_,__NONE,__SEL1,	__TEX_,__DIFF | __ALRP,__MUL1},
						/*Stage1*/{    NULL, 0, 0, __CURR,__TEX_,__MUL1,	__CURR,__NONE         ,__SEL1},
						/*Stage2*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR,__DIFF         ,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						/*Stage0*/{__NUM(3), 1, 0, __TEX_,__NONE,__SEL1,	__TEX_,__DIFF | __ALRP,__MUL1},
						/*Stage1*/{    NULL, 0, 0, __CURR,__TEX_,__MUL1,	__CURR,__NONE         ,__SEL1},
						/*Stage2*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR,__DIFF         ,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
					
					//���e�X�g����
					{
						/*Stage0*/{__NUM(3), 1, 0, __TEX_,__NONE,__SEL1,	__TEX_,__DIFF | __ALRP,__MUL1},
						/*Stage1*/{    NULL, 0, 0, __CURR,__TEX_,__MUL1,	__CURR,__NONE         ,__SEL1},
						/*Stage2*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR,__DIFF         ,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
			},
		},
	},

	// ��Z�ς݃��u�����h���[�h��RGB���]
	{
		// �u�����h�e�N�X�`������
		{
			//�f�B�t���[�YRGB�J���[����
			{
				//�f�B�t���[�YAlpha�J���[����
				{
					//���`�����l������
					{
						/*Stage0*/{__NUM(1), 0, 0,__NONE,__DIFF,__SEL2,	__TEX_ | __COMP,__NONE,__SEL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						/*Stage0*/{__NUM(1), 0, 0,__TEX_,__NONE,__SEL1,	__TEX_ | __COMP,__NONE,__SEL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���e�X�g����
					{
						/*Stage0*/{__NUM(1), 0, 0,__TEX_,__NONE,__SEL1,	__TEX_ | __COMP,__NONE,__SEL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
				//�f�B�t���[�YAlpha�J���[����
				{
					//���`�����l������
					{
						/*Stage0*/{__NUM(1), 0, 0,__NONE,__DIFF,__SEL2,	__TEX_ | __COMP,__DIFF | __ALRP,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						/*Stage0*/{__NUM(1), 0, 0,__TEX_,__DIFF,__MUL1,	__TEX_ | __COMP,__DIFF | __ALRP,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
					
					//���e�X�g����
					{
						/*Stage0*/{__NUM(1), 0, 0,__TEX_,__DIFF,__MUL1,	__TEX_ | __COMP,__DIFF | __ALRP,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
			},
			//�f�B�t���[�YRGB�J���[����
			{
				//�f�B�t���[�YAlpha�J���[�Ȃ�
				{
					//���`�����l������
					{
						/*Stage0*/{__NUM(1), 0, 0, __NONE,__DIFF,__SEL2,	__TEX_ | __COMP,__DIFF,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						/*Stage0*/{__NUM(1), 0, 0, __TEX_,__NONE,__SEL1,	__TEX_ | __COMP,__DIFF,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
					
					//���e�X�g����
					{
						/*Stage0*/{__NUM(1), 0, 0, __TEX_,__NONE,__SEL1,	__TEX_ | __COMP,__DIFF,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
				//�f�B�t���[�YAlpha�J���[����
				{
					//���`�����l������
					{
						/*Stage0*/{__NUM(2), 0, 0, __NONE,__DIFF,__SEL2,	__TEX_ | __COMP,__DIFF         ,__MUL1}, 
						/*Stage1*/{       0, 0, 0, __CURR,__NONE,__SEL1,	__CURR         ,__DIFF | __ALRP,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						/*Stage0*/{__NUM(2), 0, 0, __TEX_,__DIFF,__MUL1,	__TEX_ | __COMP,__DIFF         ,__MUL1},
						/*Stage1*/{       0, 0, 0, __CURR,__NONE,__SEL1,	__CURR         ,__DIFF | __ALRP,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
					
					//���e�X�g����
					{
						/*Stage0*/{__NUM(2), 0, 0, __TEX_,__DIFF,__MUL1,	__TEX_ | __COMP,__DIFF         ,__MUL1},
						/*Stage1*/{       0, 0, 0, __CURR,__NONE,__SEL1,	__CURR         ,__DIFF | __ALRP,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
			},
		},
		//�u�����h�e�N�X�`������ DX_BLENDGRAPHTYPE_NORMAL
		{
			//�f�B�t���[�YRGB�J���[����
			{
				//�f�B�t���[�YAlpha�J���[����
				{
					//���`�����l������
					{
						/*Stage0*/{__NUM(3), 0, 0, __TEX_,__NONE,__SEL1,	__TEX_         ,__NONE,__SEL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__TEX_,__BLDF,	__CURR         ,__TEX_,__BLDF},
						/*Stage2*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR | __COMP,__DIFF,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						/*Stage0*/{__NUM(3), 0, 0, __TEX_,__NONE,__SEL1,	__TEX_         ,__NONE,__SEL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__TEX_,__BLDF,	__CURR         ,__TEX_,__BLDF},
						/*Stage2*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR | __COMP,__DIFF,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
					
					//���e�X�g����
					{
						/*Stage0*/{__NUM(3), 0, 0, __TEX_,__NONE,__SEL1,	__TEX_         ,__NONE,__SEL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__TEX_,__BLDF,	__CURR         ,__TEX_,__BLDF},
						/*Stage2*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR | __COMP,__DIFF,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
				//�f�B�t���[�YAlpha�J���[����
				{
					//���`�����l������
					{
						/*Stage0*/{__NUM(3), 0, 0, __TEX_,__NONE,__SEL1,	__TEX_         ,__NONE         ,__SEL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__TEX_,__BLDF,	__CURR         ,__TEX_         ,__BLDF},
						/*Stage2*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR | __COMP,__DIFF | __ALRP,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						/*Stage0*/{__NUM(3), 0, 0, __TEX_,__NONE,__SEL1,	__TEX_         ,__NONE         ,__SEL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__TEX_,__BLDF,	__CURR         ,__TEX_         ,__BLDF},
						/*Stage2*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR | __COMP,__DIFF | __ALRP,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
					
					//���e�X�g����
					{
						/*Stage0*/{__NUM(3), 0, 0, __TEX_,__NONE,__SEL1,	__TEX_         ,__NONE         ,__SEL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__TEX_,__BLDF,	__CURR         ,__TEX_         ,__BLDF},
						/*Stage2*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR | __COMP,__DIFF | __ALRP,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
			},
			//�f�B�t���[�YRGB�J���[����
			{
				//�f�B�t���[�YAlpha�J���[����
				{
					//���`�����l������
					{
						/*Stage0*/{__NUM(3), 0, 0, __TEX_,__NONE,__SEL1,	__TEX_         ,__NONE,__SEL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__TEX_,__BLDF,	__CURR         ,__TEX_,__BLDF},
						/*Stage2*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR | __COMP,__DIFF,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						/*Stage0*/{__NUM(3), 0, 0, __TEX_,__NONE,__SEL1,	__TEX_         ,__NONE,__SEL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__TEX_,__BLDF,	__CURR         ,__TEX_,__BLDF},
						/*Stage2*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR | __COMP,__DIFF,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
					
					//���e�X�g����
					{
						/*Stage0*/{__NUM(3), 0, 0, __TEX_,__NONE,__SEL1,	__TEX_         ,__NONE,__SEL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__TEX_,__BLDF,	__CURR         ,__TEX_,__BLDF},
						/*Stage2*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR | __COMP,__DIFF,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
				//�f�B�t���[�YAlpha�J���[����
				{
					//���`�����l������
					{
						/*Stage0*/{__NUM(4), 0, 0, __TEX_,__NONE,__SEL1,	__TEX_         ,__NONE         ,__SEL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__TEX_,__BLDF,	__CURR         ,__TEX_         ,__BLDF},
						/*Stage2*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR | __COMP,__DIFF         ,__MUL1},
						/*Stage3*/{    NULL, 0, 0, __CURR,__NONE,__SEL1,	__CURR         ,__DIFF | __ALRP,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						/*Stage0*/{__NUM(4), 0, 0, __TEX_,__NONE,__SEL1,	__TEX_         ,__NONE         ,__SEL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__TEX_,__BLDF,	__CURR         ,__TEX_         ,__BLDF},
						/*Stage2*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR | __COMP,__DIFF         ,__MUL1},
						/*Stage3*/{    NULL, 0, 0, __CURR,__NONE,__SEL1,	__CURR         ,__DIFF | __ALRP,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
					
					//���e�X�g����
					{
						/*Stage0*/{__NUM(4), 0, 0, __TEX_,__NONE,__SEL1,	__TEX_         ,__NONE         ,__SEL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__TEX_,__BLDF,	__CURR         ,__TEX_         ,__BLDF},
						/*Stage2*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR | __COMP,__DIFF         ,__MUL1},
						/*Stage3*/{    NULL, 0, 0, __CURR,__NONE,__SEL1,	__CURR         ,__DIFF | __ALRP,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
			},
		},
		//�u�����h�e�N�X�`������ DX_BLENDGRAPHTYPE_WIPE
		{
			//�f�B�t���[�YRGB�J���[����
			{
				//�f�B�t���[�YAlpha�J���[����
				{
					//���`�����l������
					{
						/*Stage0*/{__NUM(2), 0, 0, __TEX_,__FACT,__ADD1,	__NONE         ,__DIFF,__SEL2},
						/*Stage1*/{    NULL, 1, 0, __DIFF,__CURR,__MUL1,	__TEX_ | __COMP,__NONE,__SEL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						/*Stage0*/{__NUM(3), 0, 0, __TEX_,__FACT,__ADD1,	__NONE         ,__DIFF,__SEL2},
						/*Stage1*/{    NULL, 1, 0, __SPEC,__CURR,__MUL1,	__CURR         ,__NONE,__SEL1},
						/*Stage2*/{    NULL, 1, 0, __TEX_,__CURR,__MUL1,	__TEX_ | __COMP,__NONE,__SEL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
					
					//���e�X�g����
					{
						/*Stage0*/{__NUM(2), 0, 0, __TEX_,__FACT,__ADD1,	__NONE         ,__DIFF,__SEL2},
						/*Stage1*/{    NULL, 1, 0, __TEX_,__CURR,__MUL1,	__TEX_ | __COMP,__NONE,__SEL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
				//�f�B�t���[�YAlpha�J���[����
				{
					//���`�����l������
					{
						/*Stage0*/{__NUM(2), 0, 0, __TEX_,__FACT,__ADD1,	__NONE         ,__DIFF         ,__SEL2},
						/*Stage1*/{    NULL, 1, 0, __DIFF,__CURR,__MUL1,	__TEX_ | __COMP,__DIFF | __ALRP,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						/*Stage0*/{__NUM(4), 0, 0, __TEX_,__FACT,__ADD1,	__NONE         ,__DIFF         ,__SEL2},
						/*Stage1*/{    NULL, 1, 0, __SPEC,__CURR,__MUL1,	__CURR         ,__NONE         ,__SEL1},
						/*Stage2*/{    NULL, 1, 0, __TEX_,__CURR,__MUL1,	__TEX_ | __COMP,__DIFF | __ALRP,__MUL1},
						/*Stage3*/{    NULL, 0, 0, __DIFF,__CURR,__MUL1,	__CURR         ,__NONE         ,__SEL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
					
					//���e�X�g����
					{
						/*Stage0*/{__NUM(3), 0, 0, __TEX_,__FACT,__ADD1,	__NONE         ,__DIFF         ,__SEL2},
						/*Stage1*/{    NULL, 1, 0, __TEX_,__CURR,__MUL1,	__TEX_ | __COMP,__DIFF | __ALRP,__MUL1},
						/*Stage2*/{    NULL, 1, 0, __DIFF,__CURR,__MUL1,	__CURR         ,__NONE         ,__SEL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
			},
			//�f�B�t���[�YRGB�J���[����
			{
				//�f�B�t���[�YAlpha�J���[����
				{
					//���`�����l������
					{
						/*Stage0*/{__NUM(2), 0, 0, __TEX_,__FACT,__ADD1,	__NONE         ,__DIFF,__SEL2},
						/*Stage1*/{    NULL, 1, 0, __CURR,__DIFF,__MUL1,	__TEX_ | __COMP,__DIFF,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						/*Stage0*/{__NUM(3), 0, 0, __TEX_,__FACT,__ADD1,	__NONE         ,__DIFF,__SEL2},
						/*Stage1*/{    NULL, 1, 0, __SPEC,__CURR,__MUL1,	__CURR         ,__NONE,__SEL1},
						/*Stage2*/{    NULL, 1, 0, __TEX_,__CURR,__MUL1,	__TEX_ | __COMP,__DIFF,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
					
					//���e�X�g����
					{
						/*Stage0*/{__NUM(2), 0, 0, __TEX_,__FACT,__ADD1,	__NONE         ,__DIFF,__SEL2},
						/*Stage1*/{    NULL, 1, 0, __TEX_,__CURR,__MUL1,	__TEX_ | __COMP,__DIFF,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
				//�f�B�t���[�YAlpha�J���[����
				{
					//���`�����l������
					{
						/*Stage0*/{__NUM(3), 0, 0, __TEX_,__FACT,__ADD1,	__NONE         ,__DIFF         ,__SEL2},
						/*Stage1*/{    NULL, 1, 0, __DIFF,__CURR,__MUL1,	__TEX_ | __COMP,__DIFF         ,__MUL1},
						/*Stage2*/{    NULL, 0, 0, __CURR,__CURR,__SEL1,	__CURR         ,__DIFF | __ALRP,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						/*Stage0*/{__NUM(4), 0, 0, __TEX_,__FACT,__ADD1,	__NONE         ,__DIFF         ,__SEL2},
						/*Stage1*/{    NULL, 1, 0, __SPEC,__CURR,__MUL1,	__CURR         ,__NONE         ,__SEL1},
						/*Stage2*/{    NULL, 1, 0, __TEX_,__CURR,__MUL1,	__TEX_ | __COMP,__DIFF         ,__MUL1},
						/*Stage3*/{    NULL, 0, 0, __DIFF,__CURR,__MUL1,	__CURR         ,__DIFF | __ALRP,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
					
					//���e�X�g����
					{
						/*Stage0*/{__NUM(3), 0, 0, __TEX_,__FACT,__ADD1,	__NONE         ,__DIFF         ,__SEL2},
						/*Stage1*/{    NULL, 1, 0, __TEX_,__CURR,__MUL1,	__TEX_ | __COMP,__DIFF         ,__MUL1},
						/*Stage2*/{    NULL, 1, 0, __DIFF,__CURR,__MUL1,	__CURR         ,__DIFF | __ALRP,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
			},
		},
		//�u�����h�e�N�X�`������ DX_BLENDGRAPHTYPE_ALPHA
		{
			//�f�B�t���[�YRGB�J���[����
			{
				//�f�B�t���[�YAlpha�J���[����
				{
					//���`�����l������
					{
						/*Stage0*/{__NUM(3), 0, 0, __TEX_,__NONE,__SEL1,	__TEX_         ,__NONE,__SEL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__TEX_,__MUL1,	__CURR         ,__NONE,__SEL1},
						/*Stage2*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR | __COMP,__DIFF,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						/*Stage0*/{__NUM(3), 0, 0, __TEX_,__NONE,__SEL1,	__TEX_         ,__NONE,__SEL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__TEX_,__MUL1,	__CURR         ,__NONE,__SEL1},
						/*Stage2*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR | __COMP,__DIFF,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
					
					//���e�X�g����
					{
						/*Stage0*/{__NUM(3), 0, 0, __TEX_,__NONE,__SEL1,	__TEX_         ,__NONE,__SEL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__TEX_,__MUL1,	__CURR         ,__NONE,__SEL1},
						/*Stage2*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR | __COMP,__DIFF,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
				//�f�B�t���[�YAlpha�J���[����
				{
					//���`�����l������
					{
						/*Stage0*/{__NUM(4), 0, 0, __TEX_,__NONE,__SEL1,	__TEX_         ,__NONE         ,__SEL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__TEX_,__MUL1,	__CURR         ,__NONE         ,__SEL1},
						/*Stage2*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR | __COMP,__DIFF         ,__MUL1},
						/*Stage3*/{    NULL, 0, 0, __CURR,__NONE,__SEL1,	__CURR         ,__DIFF | __ALRP,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						/*Stage0*/{__NUM(4), 0, 0, __TEX_,__NONE,__SEL1,	__TEX_         ,__NONE         ,__SEL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__TEX_,__MUL1,	__CURR         ,__NONE         ,__SEL1},
						/*Stage2*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR | __COMP,__DIFF         ,__MUL1},
						/*Stage3*/{    NULL, 0, 0, __CURR,__NONE,__SEL1,	__CURR         ,__DIFF | __ALRP,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
					
					//���e�X�g����
					{
						/*Stage0*/{__NUM(4), 0, 0, __TEX_,__NONE,__SEL1,	__TEX_         ,__NONE         ,__SEL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__TEX_,__MUL1,	__CURR         ,__NONE         ,__SEL1},
						/*Stage2*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR | __COMP,__DIFF         ,__MUL1},
						/*Stage3*/{    NULL, 0, 0, __CURR,__NONE,__SEL1,	__CURR         ,__DIFF | __ALRP,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
			},
			//�f�B�t���[�YRGB�J���[����
			{
				//�f�B�t���[�YAlpha�J���[����
				{
					//���`�����l������
					{
						/*Stage0*/{__NUM(3), 0, 0, __TEX_,__NONE,__SEL1,	__TEX_         ,__NONE,__SEL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__TEX_,__MUL1,	__CURR         ,__NONE,__SEL1},
						/*Stage2*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR | __COMP,__DIFF,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						/*Stage0*/{__NUM(3), 0, 0, __TEX_,__NONE,__SEL1,	__TEX_         ,__NONE,__SEL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__TEX_,__MUL1,	__CURR         ,__NONE,__SEL1},
						/*Stage2*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR | __COMP,__DIFF,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
					
					//���e�X�g����
					{
						/*Stage0*/{__NUM(3), 0, 0, __TEX_,__NONE,__SEL1,	__TEX_         ,__NONE,__SEL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__TEX_,__MUL1,	__CURR         ,__NONE,__SEL1},
						/*Stage2*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR | __COMP,__DIFF,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
				//�f�B�t���[�YAlpha�J���[����
				{
					//���`�����l������
					{
						/*Stage0*/{__NUM(4), 0, 0, __TEX_,__NONE,__SEL1,	__TEX_         ,__NONE         ,__SEL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__TEX_,__MUL1,	__CURR         ,__NONE         ,__SEL1},
						/*Stage2*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR | __COMP,__DIFF         ,__MUL1},
						/*Stage3*/{    NULL, 0, 0, __CURR,__NONE,__SEL1,	__CURR         ,__DIFF | __ALRP,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						/*Stage0*/{__NUM(4), 0, 0, __TEX_,__NONE,__SEL1,	__TEX_         ,__NONE         ,__SEL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__TEX_,__MUL1,	__CURR         ,__NONE         ,__SEL1},
						/*Stage2*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR | __COMP,__DIFF         ,__MUL1},
						/*Stage3*/{    NULL, 0, 0, __CURR,__NONE,__SEL1,	__CURR         ,__DIFF | __ALRP,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
					
					//���e�X�g����
					{
						/*Stage0*/{__NUM(4), 0, 0, __TEX_,__NONE,__SEL1,	__TEX_         ,__NONE         ,__SEL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__TEX_,__MUL1,	__CURR         ,__NONE         ,__SEL1},
						/*Stage2*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR | __COMP,__DIFF         ,__MUL1},
						/*Stage3*/{    NULL, 0, 0, __CURR,__NONE,__SEL1,	__CURR         ,__DIFF | __ALRP,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
			},
		},
	},

	// ��Z�ς݃��u�����h���[�h�̕`��P�x�S�{
	{
		// �u�����h�e�N�X�`������
		{
			//�f�B�t���[�YRGB�J���[����
			{
				//�f�B�t���[�YAlpha�J���[����
				{
					//���`�����l������
					{
						/*Stage0*/{__NUM(1), 0, 0,__NONE,__DIFF,__SEL2,	__TEX_,__DIFF,__MUL4}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						/*Stage0*/{__NUM(1), 0, 0,__TEX_,__NONE,__SEL1,	__TEX_,__DIFF,__MUL4}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���e�X�g����
					{
						/*Stage0*/{__NUM(1), 0, 0,__TEX_,__NONE,__SEL1,	__TEX_,__DIFF,__MUL4}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
				//�f�B�t���[�YAlpha�J���[����
				{
					//���`�����l������
					{
						/*Stage0*/{__NUM(2), 0, 0,__NONE,__DIFF,__SEL2,	__TEX_,__DIFF         ,__MUL4},
						/*Stage1*/{    NULL, 0, 0,__CURR,__NONE,__SEL1,	__CURR,__DIFF | __ALRP,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						/*Stage0*/{__NUM(2), 0, 0,__TEX_,__DIFF,__MUL1,	__TEX_,__DIFF         ,__MUL4},
						/*Stage1*/{    NULL, 0, 0,__CURR,__NONE,__SEL1,	__CURR,__DIFF | __ALRP,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
					
					//���e�X�g����
					{
						/*Stage0*/{__NUM(2), 0, 0,__TEX_,__DIFF,__MUL1,	__TEX_,__DIFF         ,__MUL4},
						/*Stage1*/{    NULL, 0, 0,__CURR,__NONE,__SEL1,	__CURR,__DIFF | __ALRP,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
			},
			//�f�B�t���[�YRGB�J���[����
			{
				//�f�B�t���[�YAlpha�J���[�Ȃ�
				{
					//���`�����l������
					{
						/*Stage0*/{__NUM(1), 0, 0,__NONE,__DIFF,__SEL2,	__TEX_,__DIFF,__MUL4}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						/*Stage0*/{__NUM(1), 0, 0,__TEX_,__NONE,__SEL1,	__TEX_,__DIFF,__MUL4}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
					
					//���e�X�g����
					{
						/*Stage0*/{__NUM(1), 0, 0,__TEX_,__NONE,__SEL1,	__TEX_,__DIFF,__MUL4}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
				//�f�B�t���[�YAlpha�J���[����
				{
					//���`�����l������
					{
						/*Stage0*/{__NUM(2), 0, 0,__NONE,__DIFF,__SEL2,	__TEX_,__DIFF         ,__MUL4},
						/*Stage1*/{    NULL, 0, 0,__CURR,__NONE,__SEL1,	__CURR,__DIFF | __ALRP,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						/*Stage0*/{__NUM(2), 0, 0,__TEX_,__DIFF,__MUL1,	__TEX_,__DIFF         ,__MUL4},
						/*Stage1*/{    NULL, 0, 0,__CURR,__NONE,__SEL1,	__CURR,__DIFF | __ALRP,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
					
					//���e�X�g����
					{
						/*Stage0*/{__NUM(2), 0, 0,__TEX_,__DIFF,__MUL1,	__TEX_,__DIFF         ,__MUL4},
						/*Stage1*/{    NULL, 0, 0,__CURR,__NONE,__SEL1,	__CURR,__DIFF | __ALRP,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
			},
		},
		//�u�����h�e�N�X�`������ DX_BLENDGRAPHTYPE_NORMAL
		{
			//�f�B�t���[�YRGB�J���[����
			{
				//�f�B�t���[�YAlpha�J���[����
				{
					//���`�����l������
					{
						/*Stage0*/{__NUM(5), 1, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE,__SEL1},
						/*Stage1*/{    NULL, 0, 0, __CURR,__TEX_,__SUB1,	__CURR,__TEX_,__SUB1},
						/*Stage2*/{    NULL, 0, 0, __CURR,__FACT,__MUL1,	__CURR,__FACT,__MUL1},
						/*Stage3*/{    NULL, 0, 0, __CURR,__TEX_,__ADD1,	__CURR,__TEX_,__ADD1},
						/*Stage4*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR,__DIFF,__MUL4}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						/*Stage0*/{__NUM(5), 1, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE,__SEL1},
						/*Stage1*/{    NULL, 0, 0, __CURR,__TEX_,__SUB1,	__CURR,__TEX_,__SUB1},
						/*Stage2*/{    NULL, 0, 0, __CURR,__FACT,__MUL1,	__CURR,__FACT,__MUL1},
						/*Stage3*/{    NULL, 0, 0, __CURR,__TEX_,__ADD1,	__CURR,__TEX_,__ADD1},
						/*Stage4*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR,__DIFF,__MUL4}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
					
					//���e�X�g����
					{
						/*Stage0*/{__NUM(5), 1, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE,__SEL1},
						/*Stage1*/{    NULL, 0, 0, __CURR,__TEX_,__SUB1,	__CURR,__TEX_,__SUB1},
						/*Stage2*/{    NULL, 0, 0, __CURR,__FACT,__MUL1,	__CURR,__FACT,__MUL1},
						/*Stage3*/{    NULL, 0, 0, __CURR,__TEX_,__ADD1,	__CURR,__TEX_,__ADD1},
						/*Stage4*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR,__DIFF,__MUL4}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
				//�f�B�t���[�YAlpha�J���[����
				{
					//���`�����l������
					{
						/*Stage0*/{__NUM(6), 1, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE         ,__SEL1},
						/*Stage1*/{    NULL, 0, 0, __CURR,__TEX_,__SUB1,	__CURR,__TEX_         ,__SUB1},
						/*Stage2*/{    NULL, 0, 0, __CURR,__FACT,__MUL1,	__CURR,__FACT         ,__MUL1},
						/*Stage3*/{    NULL, 0, 0, __CURR,__TEX_,__ADD1,	__CURR,__TEX_         ,__ADD1},
						/*Stage4*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR,__DIFF         ,__MUL4},
						/*Stage5*/{    NULL, 0, 0, __CURR,__NONE,__SEL1,	__CURR,__DIFF | __ALRP,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						/*Stage0*/{__NUM(6), 1, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE         ,__SEL1},
						/*Stage1*/{    NULL, 0, 0, __CURR,__TEX_,__SUB1,	__CURR,__TEX_         ,__SUB1},
						/*Stage2*/{    NULL, 0, 0, __CURR,__FACT,__MUL1,	__CURR,__FACT         ,__MUL1},
						/*Stage3*/{    NULL, 0, 0, __CURR,__TEX_,__ADD1,	__CURR,__TEX_         ,__ADD1},
						/*Stage4*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR,__DIFF         ,__MUL4},
						/*Stage5*/{    NULL, 0, 0, __CURR,__NONE,__SEL1,	__CURR,__DIFF | __ALRP,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
					
					//���e�X�g����
					{
						/*Stage0*/{__NUM(6), 1, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE         ,__SEL1},
						/*Stage1*/{    NULL, 0, 0, __CURR,__TEX_,__SUB1,	__CURR,__TEX_         ,__SUB1},
						/*Stage2*/{    NULL, 0, 0, __CURR,__FACT,__MUL1,	__CURR,__FACT         ,__MUL1},
						/*Stage3*/{    NULL, 0, 0, __CURR,__TEX_,__ADD1,	__CURR,__TEX_         ,__ADD1},
						/*Stage4*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR,__DIFF         ,__MUL4},
						/*Stage5*/{    NULL, 0, 0, __CURR,__NONE,__SEL1,	__CURR,__DIFF | __ALRP,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
			},
			//�f�B�t���[�YRGB�J���[����
			{
				//�f�B�t���[�YAlpha�J���[����
				{
					//���`�����l������
					{
						/*Stage0*/{__NUM(5), 1, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE,__SEL1},
						/*Stage1*/{    NULL, 0, 0, __CURR,__TEX_,__SUB1,	__CURR,__TEX_,__SUB1},
						/*Stage2*/{    NULL, 0, 0, __CURR,__FACT,__MUL1,	__CURR,__FACT,__MUL1},
						/*Stage3*/{    NULL, 0, 0, __CURR,__TEX_,__ADD1,	__CURR,__TEX_,__ADD1},
						/*Stage4*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR,__DIFF,__MUL4}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						/*Stage0*/{__NUM(5), 1, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE,__SEL1},
						/*Stage1*/{    NULL, 0, 0, __CURR,__TEX_,__SUB1,	__CURR,__TEX_,__SUB1},
						/*Stage2*/{    NULL, 0, 0, __CURR,__FACT,__MUL1,	__CURR,__FACT,__MUL1},
						/*Stage3*/{    NULL, 0, 0, __CURR,__TEX_,__ADD1,	__CURR,__TEX_,__ADD1},
						/*Stage4*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR,__DIFF,__MUL4}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
					
					//���e�X�g����
					{
						/*Stage0*/{__NUM(5), 1, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE,__SEL1},
						/*Stage1*/{    NULL, 0, 0, __CURR,__TEX_,__SUB1,	__CURR,__TEX_,__SUB1},
						/*Stage2*/{    NULL, 0, 0, __CURR,__FACT,__MUL1,	__CURR,__FACT,__MUL1},
						/*Stage3*/{    NULL, 0, 0, __CURR,__TEX_,__ADD1,	__CURR,__TEX_,__ADD1},
						/*Stage4*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR,__DIFF,__MUL4}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
				//�f�B�t���[�YAlpha�J���[����
				{
					//���`�����l������
					{
						/*Stage0*/{__NUM(6), 1, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE         ,__SEL1},
						/*Stage1*/{    NULL, 0, 0, __CURR,__TEX_,__SUB1,	__CURR,__TEX_         ,__SUB1},
						/*Stage2*/{    NULL, 0, 0, __CURR,__FACT,__MUL1,	__CURR,__FACT         ,__MUL1},
						/*Stage3*/{    NULL, 0, 0, __CURR,__TEX_,__ADD1,	__CURR,__TEX_         ,__ADD1},
						/*Stage4*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR,__DIFF         ,__MUL4},
						/*Stage5*/{    NULL, 0, 0, __CURR,__NONE,__SEL1,	__CURR,__DIFF | __ALRP,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						/*Stage0*/{__NUM(6), 1, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE         ,__SEL1},
						/*Stage1*/{    NULL, 0, 0, __CURR,__TEX_,__SUB1,	__CURR,__TEX_         ,__SUB1},
						/*Stage2*/{    NULL, 0, 0, __CURR,__FACT,__MUL1,	__CURR,__FACT         ,__MUL1},
						/*Stage3*/{    NULL, 0, 0, __CURR,__TEX_,__ADD1,	__CURR,__TEX_         ,__ADD1},
						/*Stage4*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR,__DIFF         ,__MUL4},
						/*Stage5*/{    NULL, 0, 0, __CURR,__NONE,__SEL1,	__CURR,__DIFF | __ALRP,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
					
					//���e�X�g����
					{
						/*Stage0*/{__NUM(6), 1, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE         ,__SEL1},
						/*Stage1*/{    NULL, 0, 0, __CURR,__TEX_,__SUB1,	__CURR,__TEX_         ,__SUB1},
						/*Stage2*/{    NULL, 0, 0, __CURR,__FACT,__MUL1,	__CURR,__FACT         ,__MUL1},
						/*Stage3*/{    NULL, 0, 0, __CURR,__TEX_,__ADD1,	__CURR,__TEX_         ,__ADD1},
						/*Stage4*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR,__DIFF         ,__MUL4},
						/*Stage5*/{    NULL, 0, 0, __CURR,__NONE,__SEL1,	__CURR,__DIFF | __ALRP,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
			},
		},
		//�u�����h�e�N�X�`������ DX_BLENDGRAPHTYPE_WIPE
		{
			//�f�B�t���[�YRGB�J���[����
			{
				//�f�B�t���[�YAlpha�J���[����
				{
					//���`�����l������
					{
						/*Stage0*/{__NUM(2), 0, 0, __TEX_,__FACT,__ADD1,	__NONE,__DIFF,__SEL2},
						/*Stage1*/{    NULL, 1, 0, __CURR,__DIFF,__MUL1,	__TEX_,__DIFF,__MUL4}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						/*Stage0*/{__NUM(3), 0, 0, __TEX_,__FACT,__ADD1,	__NONE,__DIFF,__SEL2},
						/*Stage1*/{    NULL, 1, 0, __SPEC,__CURR,__MUL1,	__CURR,__NONE,__SEL1},
						/*Stage2*/{    NULL, 1, 0, __TEX_,__CURR,__MUL1,	__TEX_,__DIFF,__MUL4}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
					
					//���e�X�g����
					{
						/*Stage0*/{__NUM(2), 0, 0, __TEX_,__FACT,__ADD1,	__NONE,__DIFF,__SEL2},
						/*Stage1*/{    NULL, 1, 0, __TEX_,__CURR,__MUL1,	__TEX_,__DIFF,__MUL4}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
				//�f�B�t���[�YAlpha�J���[����
				{
					//���`�����l������
					{
						/*Stage0*/{__NUM(3), 0, 0, __TEX_,__FACT,__ADD1,	__NONE,__DIFF         ,__SEL2},
						/*Stage1*/{    NULL, 1, 0, __DIFF,__CURR,__MUL1,	__TEX_,__DIFF         ,__MUL4},
						/*Stage2*/{    NULL, 0, 0, __CURR,__NONE,__SEL1,	__CURR,__DIFF | __ALRP,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						/*Stage0*/{__NUM(4), 0, 0, __TEX_,__FACT,__ADD1,	__NONE,__DIFF         ,__SEL2},
						/*Stage1*/{    NULL, 1, 0, __SPEC,__CURR,__MUL1,	__CURR,__NONE         ,__SEL1},
						/*Stage2*/{    NULL, 1, 0, __TEX_,__CURR,__MUL1,	__TEX_,__DIFF         ,__MUL4},
						/*Stage3*/{    NULL, 0, 0, __DIFF,__CURR,__MUL1,	__CURR,__DIFF | __ALRP,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
					
					//���e�X�g����
					{
						/*Stage0*/{__NUM(3), 0, 0, __TEX_,__FACT,__ADD1,	__NONE,__DIFF         ,__SEL2},
						/*Stage1*/{    NULL, 1, 0, __TEX_,__CURR,__MUL1,	__TEX_,__DIFF         ,__MUL4},
						/*Stage2*/{    NULL, 1, 0, __DIFF,__CURR,__MUL1,	__CURR,__DIFF | __ALRP,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
			},
			//�f�B�t���[�YRGB�J���[����
			{
				//�f�B�t���[�YAlpha�J���[����
				{
					//���`�����l������
					{
						/*Stage0*/{__NUM(2), 0, 0, __TEX_,__FACT,__ADD1,	__NONE,__DIFF,__SEL2},
						/*Stage1*/{    NULL, 1, 0, __CURR,__DIFF,__MUL1,	__TEX_,__DIFF,__MUL4}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						/*Stage0*/{__NUM(3), 0, 0, __TEX_,__FACT,__ADD1,	__NONE,__DIFF,__SEL2},
						/*Stage1*/{    NULL, 1, 0, __SPEC,__CURR,__MUL1,	__CURR,__NONE,__SEL1},
						/*Stage2*/{    NULL, 1, 0, __TEX_,__CURR,__MUL1,	__TEX_,__DIFF,__MUL4}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
					
					//���e�X�g����
					{
						/*Stage0*/{__NUM(2), 0, 0, __TEX_,__FACT,__ADD1,	__NONE,__DIFF,__SEL2},
						/*Stage1*/{    NULL, 1, 0, __TEX_,__CURR,__MUL1,	__TEX_,__DIFF,__MUL4}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
				//�f�B�t���[�YAlpha�J���[����
				{
					//���`�����l������
					{
						/*Stage0*/{__NUM(3), 0, 0, __TEX_,__FACT,__ADD1,	__NONE,__DIFF         ,__SEL2},
						/*Stage1*/{    NULL, 1, 0, __DIFF,__CURR,__MUL1,	__TEX_,__DIFF         ,__MUL4},
						/*Stage2*/{    NULL, 0, 0, __CURR,__NONE,__SEL1,	__CURR,__DIFF | __ALRP,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						/*Stage0*/{__NUM(4), 0, 0, __TEX_,__FACT,__ADD1,	__NONE,__DIFF         ,__SEL2},
						/*Stage1*/{    NULL, 1, 0, __SPEC,__CURR,__MUL1,	__CURR,__NONE         ,__SEL1},
						/*Stage2*/{    NULL, 1, 0, __TEX_,__CURR,__MUL1,	__TEX_,__DIFF         ,__MUL4},
						/*Stage3*/{    NULL, 0, 0, __DIFF,__CURR,__MUL1,	__CURR,__DIFF | __ALRP,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
					
					//���e�X�g����
					{
						/*Stage0*/{__NUM(3), 0, 0, __TEX_,__FACT,__ADD1,	__NONE,__DIFF         ,__SEL2},
						/*Stage1*/{    NULL, 1, 0, __TEX_,__CURR,__MUL1,	__TEX_,__DIFF         ,__MUL4},
						/*Stage2*/{    NULL, 1, 0, __DIFF,__CURR,__MUL1,	__CURR,__DIFF | __ALRP,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
			},
		},
		//�u�����h�e�N�X�`������ DX_BLENDGRAPHTYPE_ALPHA
		{
			//�f�B�t���[�YRGB�J���[����
			{
				//�f�B�t���[�YAlpha�J���[����
				{
					//���`�����l������
					{
						/*Stage0*/{__NUM(2), 0, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE,__SEL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__TEX_,__MUL1,	__CURR,__DIFF,__MUL4}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						/*Stage0*/{__NUM(2), 0, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE,__SEL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__TEX_,__MUL1,	__CURR,__DIFF,__MUL4}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
					
					//���e�X�g����
					{
						/*Stage0*/{__NUM(2), 0, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE,__SEL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__TEX_,__MUL1,	__CURR,__DIFF,__MUL4}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
				//�f�B�t���[�YAlpha�J���[����
				{
					//���`�����l������
					{
						/*Stage0*/{__NUM(4), 0, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE         ,__SEL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__TEX_,__MUL1,	__CURR,__NONE         ,__SEL1},
						/*Stage2*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR,__DIFF         ,__MUL4},
						/*Stage3*/{    NULL, 0, 0, __CURR,__NONE,__SEL1,	__CURR,__DIFF | __ALRP,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						/*Stage0*/{__NUM(4), 0, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE         ,__SEL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__TEX_,__MUL1,	__CURR,__NONE         ,__SEL1},
						/*Stage2*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR,__DIFF         ,__MUL4},
						/*Stage3*/{    NULL, 0, 0, __CURR,__NONE,__SEL1,	__CURR,__DIFF | __ALRP,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
					
					//���e�X�g����
					{
						/*Stage0*/{__NUM(4), 0, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE         ,__SEL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__TEX_,__MUL1,	__CURR,__NONE         ,__SEL1},
						/*Stage2*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR,__DIFF         ,__MUL4},
						/*Stage3*/{    NULL, 0, 0, __CURR,__NONE,__SEL1,	__CURR,__DIFF | __ALRP,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
			},
			//�f�B�t���[�YRGB�J���[����
			{
				//�f�B�t���[�YAlpha�J���[����
				{
					//���`�����l������
					{
						/*Stage0*/{__NUM(3), 0, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE,__SEL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__TEX_,__MUL1,	__CURR,__NONE,__SEL1},
						/*Stage2*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR,__DIFF,__MUL4}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						/*Stage0*/{__NUM(3), 0, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE,__SEL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__TEX_,__MUL1,	__CURR,__NONE,__SEL1},
						/*Stage2*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR,__DIFF,__MUL4}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
					
					//���e�X�g����
					{
						/*Stage0*/{__NUM(3), 0, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE,__SEL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__TEX_,__MUL1,	__CURR,__NONE,__SEL1},
						/*Stage2*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR,__DIFF,__MUL4}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
				//�f�B�t���[�YAlpha�J���[����
				{
					//���`�����l������
					{
						/*Stage0*/{__NUM(4), 0, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE         ,__SEL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__TEX_,__MUL1,	__CURR,__NONE         ,__SEL1},
						/*Stage2*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR,__DIFF         ,__MUL4},
						/*Stage3*/{    NULL, 0, 0, __CURR,__NONE,__SEL1,	__CURR,__DIFF | __ALRP,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},

					//���`�����l������
					{
						/*Stage0*/{__NUM(4), 0, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE         ,__SEL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__TEX_,__MUL1,	__CURR,__NONE         ,__SEL1},
						/*Stage2*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR,__DIFF         ,__MUL4},
						/*Stage3*/{    NULL, 0, 0, __CURR,__NONE,__SEL1,	__CURR,__DIFF | __ALRP,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
					
					//���e�X�g����
					{
						/*Stage0*/{__NUM(4), 0, 0, __TEX_,__NONE,__SEL1,	__TEX_,__NONE         ,__SEL1},
						/*Stage1*/{    NULL, 1, 0, __CURR,__TEX_,__MUL1,	__CURR,__NONE         ,__SEL1},
						/*Stage2*/{    NULL, 0, 0, __CURR,__DIFF,__MUL1,	__CURR,__DIFF         ,__MUL4},
						/*Stage3*/{    NULL, 0, 0, __CURR,__NONE,__SEL1,	__CURR,__DIFF | __ALRP,__MUL1}, DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST DISB_ST
					},
				},
			},
		},
	},
} ;

#undef __DIFF
#undef __CURR
#undef __TEX_
#undef __FACT
#undef __SPEC
#undef __TEMP

#undef __DISB
#undef __SEL1
#undef __SEL2
#undef __MUL1
#undef __MUL2
#undef __MUL4
#undef __ADD1

#undef __NUM

// �u�����h�ݒ���X�V(Direct3D �ɔ��f)����
extern int  Graphics_D3D9_DeviceState_RefreshBlendState(
	GRAPHICS_HARDDATA_DIRECT3D9_BLENDINFO *pUserInfo,
	int TextureStageIsTextureAndTextureCoordOnlyFlag,
	int TextureIsGraphHandleFlag,
	int UseShader
)
{
	GRAPHICS_HARDDATA_DIRECT3D9_BLENDINFO BlendInfo ;
	int AlphaTableIndex ;
	int BlendTextureIndex ;
	int NextBlendMode ;
	DWORD i ;
	DWORD StageNum ;
	int TextureOnly ;
	GRAPHICS_HARDDATA_DIRECT3D9_TEXTURESTAGEINFO *Dest ;
	const GRAPHICS_HARDDATA_DIRECT3D9_TEXTURESTAGEINFO *Src, *TextureStageInfo = NULL ;
	D_IDirect3DTexture9 *StageTexture[USE_TEXTURESTAGE_NUM] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL } ;
	D_IDirect3DPixelShader9 *UsePixelShader = NULL ;
	IMAGEDATA *Image ;
	SHADOWMAPDATA *ShadowMap ;
	int UseFloatFactorColor = FALSE ;
	FLOAT4 FloatFactorColor ;
	GRAPHICS_HARDDATA_DIRECT3D9_TEXTURESTAGEINFO TempTextureStageInfo[ USE_TEXTURESTAGE_NUM ] ;
	int UseShaderFlag ;

	if( GAPIWin.Direct3DDevice9Object == NULL ) return -1 ;

	UseShaderFlag = GSYS.HardInfo.UseShader && ( GD3D9.NormalDraw_NotUsePixelShader == FALSE ? TRUE : FALSE ) && UseShader ;

	// �u�����h�e�N�X�`��������ꍇ�́A��{�I�ɃV�F�[�_�[���g�p���Ȃ��悤�ɏ�����ύX
	if( GD3D9.Device.DrawSetting.BlendTexture != NULL )
	{
		UseShaderFlag = FALSE ;
	}

	for( i = 0 ; i < USE_TEXTURESTAGE_NUM ; i ++ )
	{
		BlendInfo.TextureStageInfo[i].ResultTempARG = FALSE ;
	}

	BlendInfo.SeparateAlphaBlendEnable = FALSE ;
	BlendInfo.SrcBlendAlpha = -1 ;
	BlendInfo.DestBlendAlpha = -1 ;
	BlendInfo.BlendOpAlpha = -1 ;

	// ���[�U�[�w��̃u�����h��񂪂��邩�ǂ����ŏ����𕪊�
	TextureOnly = FALSE ;
	if( pUserInfo )
	{
		StageNum = ( DWORD )pUserInfo->UseTextureStageNum ;

		_MEMCPY( &BlendInfo, pUserInfo, sizeof( GRAPHICS_HARDDATA_DIRECT3D9_BLENDINFO ) - sizeof( pUserInfo->TextureStageInfo ) ) ;
		TextureStageInfo = pUserInfo->TextureStageInfo ;
		TextureOnly = TextureStageIsTextureAndTextureCoordOnlyFlag ;

		// �O���t�B�b�N�n���h������e�N�X�`�����擾����
		if( TextureIsGraphHandleFlag )
		{
			for( i = 0 ; i < StageNum ; i ++ )
			{
				if( !GRAPHCHKFULL( ( LONG_PTR )pUserInfo->TextureStageInfo[ i ].Texture, Image ) )
				{
					StageTexture[ i ] = Image->Hard.Draw[ 0 ].Tex->PF->D3D9.Texture ;
				}
				else
				if( !SHADOWMAPCHKFULL( ( LONG_PTR )pUserInfo->TextureStageInfo[ i ].Texture, ShadowMap ) )
				{
					StageTexture[ i ] = ShadowMap->PF->D3D9.Texture ;
				}
				else
				{
					StageTexture[ i ] = NULL ;
				}
			}
		}
		else
		{
			for( i = 0 ; i < StageNum ; i ++ )
			{
				StageTexture[ i ] = ( D_IDirect3DTexture9 * )pUserInfo->TextureStageInfo[ i ].Texture ;
			}
		}

//		GRH.UseAlphaChDrawShader = FALSE ;
	}
	else
	{
		// �������t���O�������Ă��炸�A���p�����[�^����ؕύX����Ė��������牽�����Ȃ�
		if( GD3D9.Device.DrawSetting.CancelSettingEqualCheck == FALSE && GD3D9.Device.DrawSetting.ChangeBlendParamFlag == FALSE && GD3D9.Device.DrawSetting.ChangeTextureFlag == FALSE ) return -1 ; 

		// �`��e�N�X�`���̂ݕύX���ꂽ�ꍇ�͕`��e�N�X�`���̕ύX�̂ݍs��
		if( GD3D9.Device.DrawSetting.CancelSettingEqualCheck == FALSE && GD3D9.Device.DrawSetting.ChangeBlendParamFlag == FALSE && GD3D9.Device.DrawSetting.ChangeTextureFlag == TRUE )
		{
			// �u�����h�e�N�X�`��������ꍇ�Ɩ����ꍇ�ŏ����𕪊�
			if( GD3D9.Device.DrawSetting.BlendTexture != NULL )
			{
				// ����ꍇ

				if( UseShaderFlag )
				{
					// �g�p����e�N�X�`���[�A�h���X���Z�b�g
					Direct3DDevice9_SetTexture( 0, GD3D9.Device.DrawSetting.RenderTexture ) ;
					GD3D9.Device.State.BlendInfo.TextureStageInfo[0].Texture = GD3D9.Device.DrawSetting.RenderTexture ;

					Direct3DDevice9_SetTexture( 1, GD3D9.Device.DrawSetting.BlendTexture ) ;
					GD3D9.Device.State.BlendInfo.TextureStageInfo[1].Texture = GD3D9.Device.DrawSetting.BlendTexture ;
				}
				else
				{
					// ���[�h�ɂ���ď�����ύX
					switch( GD3D9.Device.DrawSetting.BlendGraphType )
					{
					case DX_BLENDGRAPHTYPE_NORMAL :
						Direct3DDevice9_SetTexture( 0, GD3D9.Device.DrawSetting.BlendTexture ) ;
						GD3D9.Device.State.BlendInfo.TextureStageInfo[0].Texture = GD3D9.Device.DrawSetting.BlendTexture ;

						Direct3DDevice9_SetTexture( 1, GD3D9.Device.DrawSetting.RenderTexture ) ;
						GD3D9.Device.State.BlendInfo.TextureStageInfo[1].Texture = GD3D9.Device.DrawSetting.RenderTexture ;

						Direct3DDevice9_SetTexture( 3, GD3D9.Device.DrawSetting.RenderTexture ) ;
						GD3D9.Device.State.BlendInfo.TextureStageInfo[3].Texture = GD3D9.Device.DrawSetting.RenderTexture ;
						break;

					case DX_BLENDGRAPHTYPE_WIPE :
						Direct3DDevice9_SetTexture( 0, GD3D9.Device.DrawSetting.BlendTexture ) ;
						GD3D9.Device.State.BlendInfo.TextureStageInfo[0].Texture = GD3D9.Device.DrawSetting.BlendTexture ;

						// ���`�����l�����L���ȏꍇ�Ƃ����łȂ��ꍇ�ŏ����𕪊�
						if( GD3D9.Device.DrawSetting.AlphaChannelValidFlag == TRUE )
						{
							Direct3DDevice9_SetTexture( 2, GD3D9.Device.DrawSetting.RenderTexture ) ;
							GD3D9.Device.State.BlendInfo.TextureStageInfo[2].Texture = GD3D9.Device.DrawSetting.RenderTexture ;
	//						StageTexture[2] = GD3D9.Device.DrawSetting.RenderTexture ;
						}
						else
						{
							Direct3DDevice9_SetTexture( 1, GD3D9.Device.DrawSetting.RenderTexture ) ;
							GD3D9.Device.State.BlendInfo.TextureStageInfo[1].Texture = GD3D9.Device.DrawSetting.RenderTexture ;
	//						StageTexture[1] = GD3D9.Device.DrawSetting.RenderTexture ;
						}
						break ;

					case DX_BLENDGRAPHTYPE_ALPHA :
						Direct3DDevice9_SetTexture( 0, GD3D9.Device.DrawSetting.RenderTexture ) ;
						GD3D9.Device.State.BlendInfo.TextureStageInfo[0].Texture = GD3D9.Device.DrawSetting.RenderTexture ;

						Direct3DDevice9_SetTexture( 1, GD3D9.Device.DrawSetting.BlendTexture ) ;
						GD3D9.Device.State.BlendInfo.TextureStageInfo[1].Texture = GD3D9.Device.DrawSetting.BlendTexture ;
						break;
					}
				}
			}
			else
			{
				// �����ꍇ
				Direct3DDevice9_SetTexture( 0, GD3D9.Device.DrawSetting.RenderTexture ) ;
				GD3D9.Device.State.BlendInfo.TextureStageInfo[0].Texture = GD3D9.Device.DrawSetting.RenderTexture ;
	//			StageTexture[0] = GD3D9.Device.DrawSetting.RenderTexture ;
			}

			// �e�N�X�`�����ύX���ꂽ�A�t���O��|��
			GD3D9.Device.DrawSetting.ChangeTextureFlag = FALSE ;
			GD3D9.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

			// �I��
			return 0 ;
		}

		// �e�N�X�`�����ύX���ꂽ�A�t���O��|��
		GD3D9.Device.DrawSetting.ChangeTextureFlag = FALSE ;
		GD3D9.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

		// �p�����[�^���ύX���ꂽ�A�t���O��|��
		GD3D9.Device.DrawSetting.ChangeBlendParamFlag = FALSE ;

		// �`��ҋ@���Ă���`�敨��`��
//		DRAWSTOCKINFO

		// �u�����h���[�h�̌���
		{
			NextBlendMode = GD3D9.Device.DrawSetting.BlendMode ;
			switch( GD3D9.Device.DrawSetting.BlendMode )
			{
			case DX_BLENDMODE_SUB :
				// ���Z�u�����h�w��� D3DRS_BLENDOP ���g�p�\�ȏꍇ�� 14�Ԗڂ̃u�����h���[�h���g�p����
				if( GD3D9.Device.Caps.ValidDestBlendOp )
				{
					NextBlendMode = DX_BLENDMODE_SUB1 ;
				}
				break ;

			case DX_BLENDMODE_NOBLEND :
				// �`���Ƀ��l������ꍇ�� DX_BLENDMODE_NOBLEND �ł��u�����h���[�h�� DX_BLENDMODE_SRCCOLOR �ɂ���
				if( GSYS.DrawSetting.AlphaChDrawMode )
				{
					NextBlendMode = DX_BLENDMODE_SRCCOLOR ;
				}

				// �`�挳�摜�Ƀ��`�����l��������ꍇ��u�����h�摜�̗L���ȂǏ�������� DX_BLENDMODE_ALPHA ���g�p����
				if( GD3D9.Device.DrawSetting.RenderTexture != NULL )
				{
					if( GD3D9.Device.DrawSetting.BlendTexture != NULL )
					{
						switch( GD3D9.Device.DrawSetting.BlendGraphType )
						{
						case DX_BLENDGRAPHTYPE_NORMAL :
						case DX_BLENDGRAPHTYPE_ALPHA :
							if( GD3D9.Device.DrawSetting.AlphaTestValidFlag == TRUE || GD3D9.Device.DrawSetting.AlphaChannelValidFlag == TRUE )
							{
								NextBlendMode = DX_BLENDMODE_ALPHA ;
							}
							break ;

						case DX_BLENDGRAPHTYPE_WIPE :
							NextBlendMode = DX_BLENDMODE_ALPHA ;
							break ;
						}
					}
					else
					{
						if( GD3D9.Device.DrawSetting.AlphaChannelValidFlag == TRUE )
						{
							NextBlendMode = DX_BLENDMODE_ALPHA ;
						}
					}
				}
				break ;
			}
		}
		
		// �\�[�X�u�����h�ƃf�X�g�u�����h�̒l���Z�b�g
		BlendInfo.DestBlend = __RGBBlendStateTable[ NextBlendMode ][ RGB_BLENDSTATE_TABLE_DESTBLEND ] ;
		BlendInfo.SrcBlend  = __RGBBlendStateTable[ NextBlendMode ][ RGB_BLENDSTATE_TABLE_SRCBLEND ] ;
		BlendInfo.BlendOp   = __RGBBlendStateTable[ NextBlendMode ][ RGB_BLENDSTATE_TABLE_BLENDOP ] ;
		BlendInfo.FactorColor = 0xffffffff ;

		// Direct3D �̃��u�����h�@�\���g�p����u�����h���[�h���ǂ������擾����
		BlendInfo.AlphaBlendEnable = __RGBBlendStateTable[ NextBlendMode ][ RGB_BLENDSTATE_TABLE_ALPHABLENDENABLE ] ;
/*
		// �������݂̃��l�𐳂����v�Z����u�����h���[�h���g�p���邩�ǂ����ŏ����𕪊�
		GRH.UseAlphaChDrawShader = FALSE ;
		if( 0 ) // �d���̂łƂ肠����������
		{
			if( UseShaderFlag && GSYS.DrawSetting.AlphaChDrawMode )
			{
				// �u�����h�����ɃV�F�[�_�[���g�p����ꍇ�Ƃ����łȂ��ꍇ�ŕ���
				if( __RGBBlendStateTable[ NextBlendMode ][ RGB_BLENDSTATE_TABLE_SHADERALPHABLEND ] == 1 )
				{
					GRH.UseAlphaChDrawShader = TRUE ;

					BlendInfo.SeparateAlphaBlendEnable = FALSE ;
					BlendInfo.SrcBlend       = D_D3DBLEND_ONE ;
					BlendInfo.DestBlend      = D_D3DBLEND_ZERO ;
					BlendInfo.BlendOp        = D_D3DBLENDOP_ADD ;
					BlendInfo.SrcBlendAlpha  = -1 ;
					BlendInfo.DestBlendAlpha = -1 ;
					BlendInfo.BlendOpAlpha   = -1 ;
				}
			}
		}
*/
		// �J�����g�e�N�X�`���������ꍇ�Ƃ���ꍇ�ŕ���
		if( GD3D9.Device.DrawSetting.RenderTexture == NULL )
		{
			// �J�����g�e�N�X�`���������ꍇ�͐��┠�̕`��Ɣ��f�A�e�N�X�`���X�e�[�W�͖����ɂ���

			// �Ƃ肠�����`�揈�����s��
			GD3D9.Device.DrawInfo.BlendMaxNotDrawFlag = FALSE ;
			
			// �V�F�[�_�[���g�p���邩�ǂ����ŏ����𕪊�
#ifndef DX_NON_NORMAL_DRAW_SHADER
			if( UseShaderFlag )
			{
				UsePixelShader = GD3D9.Device.Shader._2D.BaseNoneTexPixelShader[ __RGBBlendStateTable[ NextBlendMode ][ RGB_BLENDSTATE_TABLE_TEXBLENDTABLEINDEX ] ][ 0 /*GRH.UseAlphaChDrawShader*/ ] ;
				StageNum = 0 ;
			}
			else
#endif // DX_NON_NORMAL_DRAW_SHADER
			{
				// ���Z�u�����h�̔��]�����̎��̂ݐ�p�̃e�[�u�����g�p����
				if( NextBlendMode == DX_BLENDMODE_SUB )
				{
					TextureStageInfo = __TexSubBlendStageStateTable ;
					StageNum         = ( DWORD )(DWORD_PTR)TextureStageInfo[0].Texture ;
				}
				else
				{
					IMAGEDATA *WhiteTex = NULL ;

					BlendInfo.TextureStageInfo[0].TextureCoordIndex = 0 ;
					BlendInfo.TextureStageInfo[1].TextureCoordIndex = 0 ;
					BlendInfo.TextureStageInfo[1].AlphaARG1 = D_D3DTA_DIFFUSE ;
					BlendInfo.TextureStageInfo[1].AlphaARG2 = D_D3DTA_DIFFUSE ;
					BlendInfo.TextureStageInfo[1].AlphaOP   = D_D3DTOP_DISABLE ;
					BlendInfo.TextureStageInfo[1].ColorARG1 = D_D3DTA_DIFFUSE ;
					BlendInfo.TextureStageInfo[1].ColorARG2 = D_D3DTA_DIFFUSE ;
					BlendInfo.TextureStageInfo[1].ColorOP   = D_D3DTOP_DISABLE ;
					StageNum = 1 ;
					switch( __RGBBlendStateTable[ NextBlendMode ][ RGB_BLENDSTATE_TABLE_TEXBLENDTABLEINDEX ] )
					{
					case TEXTURE_BLEND_TABLE_INDEX_NORMAL :	// �ʏ�`��
						BlendInfo.TextureStageInfo[0].AlphaARG1 = D_D3DTA_DIFFUSE ;
						BlendInfo.TextureStageInfo[0].AlphaARG2 = D_D3DTA_DIFFUSE ;
						BlendInfo.TextureStageInfo[0].AlphaOP   = D_D3DTOP_SELECTARG2 ;
						BlendInfo.TextureStageInfo[0].ColorARG1 = D_D3DTA_DIFFUSE ;
						BlendInfo.TextureStageInfo[0].ColorARG2 = D_D3DTA_DIFFUSE ;
						BlendInfo.TextureStageInfo[0].ColorOP   = D_D3DTOP_SELECTARG2 ;
						break ;

					case TEXTURE_BLEND_TABLE_INDEX_MUL :	// ��Z�`��
						BlendInfo.TextureStageInfo[0].AlphaARG1 = D_D3DTA_DIFFUSE ;
						BlendInfo.TextureStageInfo[0].AlphaARG2 = D_D3DTA_DIFFUSE ;
						BlendInfo.TextureStageInfo[0].AlphaOP   = D_D3DTOP_SELECTARG2 ;
						BlendInfo.TextureStageInfo[0].ColorARG1 = D_D3DTA_DIFFUSE ;
						BlendInfo.TextureStageInfo[0].ColorARG2 = D_D3DTA_TFACTOR ;
						BlendInfo.TextureStageInfo[0].ColorOP   = D_D3DTOP_BLENDDIFFUSEALPHA ;
						break ;

					case TEXTURE_BLEND_TABLE_INDEX_INVERSE :	// �q�f�a���]
						BlendInfo.TextureStageInfo[0].AlphaARG1 = D_D3DTA_DIFFUSE ;
						BlendInfo.TextureStageInfo[0].AlphaARG2 = D_D3DTA_DIFFUSE ;
						BlendInfo.TextureStageInfo[0].AlphaOP   = D_D3DTOP_SELECTARG2 ;
						BlendInfo.TextureStageInfo[0].ColorARG1 = D_D3DTA_DIFFUSE | D_D3DTA_COMPLEMENT;
						BlendInfo.TextureStageInfo[0].ColorARG2 = D_D3DTA_DIFFUSE ;
						BlendInfo.TextureStageInfo[0].ColorOP   = D_D3DTOP_SELECTARG1 ;
						break ;

					case TEXTURE_BLEND_TABLE_INDEX_X4 :	// �`��P�x4�{
						GRAPHCHK( Graphics_Image_GetWhiteTexHandle(), WhiteTex ) ;
						Graphics_D3D9_BeginScene() ;
						StageTexture[0] = WhiteTex->Orig->Hard.Tex[ 0 ].PF->D3D9.Texture ;
						BlendInfo.TextureStageInfo[0].AlphaARG1 = D_D3DTA_DIFFUSE ;
						BlendInfo.TextureStageInfo[0].AlphaARG2 = D_D3DTA_DIFFUSE ;
						BlendInfo.TextureStageInfo[0].AlphaOP   = D_D3DTOP_SELECTARG2 ;
						BlendInfo.TextureStageInfo[0].ColorARG1 = D_D3DTA_DIFFUSE ;
						BlendInfo.TextureStageInfo[0].ColorARG2 = D_D3DTA_TEXTURE ;
						BlendInfo.TextureStageInfo[0].ColorOP   = D_D3DTOP_MODULATE4X ;
						break ;

					case TEXTURE_BLEND_TABLE_INDEX_PMA_NORMAL :		// ��Z�ς݃��u�����h���[�h�̒ʏ�`��
						BlendInfo.TextureStageInfo[0].AlphaARG1 = D_D3DTA_DIFFUSE ;
						BlendInfo.TextureStageInfo[0].AlphaARG2 = D_D3DTA_DIFFUSE ;
						BlendInfo.TextureStageInfo[0].AlphaOP   = D_D3DTOP_SELECTARG2 ;
						BlendInfo.TextureStageInfo[0].ColorARG1 = D_D3DTA_DIFFUSE ;
						BlendInfo.TextureStageInfo[0].ColorARG2 = D_D3DTA_DIFFUSE | D_D3DTA_ALPHAREPLICATE ;
						BlendInfo.TextureStageInfo[0].ColorOP   = D_D3DTOP_MODULATE ;
						break ;

					case TEXTURE_BLEND_TABLE_INDEX_PMA_INVERSE :	// ��Z�ς݃��u�����h���[�h��RGB���]
						BlendInfo.TextureStageInfo[0].AlphaARG1 = D_D3DTA_DIFFUSE ;
						BlendInfo.TextureStageInfo[0].AlphaARG2 = D_D3DTA_DIFFUSE ;
						BlendInfo.TextureStageInfo[0].AlphaOP   = D_D3DTOP_SELECTARG2 ;
						BlendInfo.TextureStageInfo[0].ColorARG1 = D_D3DTA_DIFFUSE | D_D3DTA_COMPLEMENT;
						BlendInfo.TextureStageInfo[0].ColorARG2 = D_D3DTA_DIFFUSE | D_D3DTA_ALPHAREPLICATE ;
						BlendInfo.TextureStageInfo[0].ColorOP   = D_D3DTOP_MODULATE ;
						break ;

					case TEXTURE_BLEND_TABLE_INDEX_PMA_X4 :			// ��Z�ς݃��u�����h���[�h�̕`��P�x4�{
						GRAPHCHK( Graphics_Image_GetWhiteTexHandle(), WhiteTex ) ;
						Graphics_D3D9_BeginScene() ;
						StageTexture[0] = WhiteTex->Orig->Hard.Tex[ 0 ].PF->D3D9.Texture ;
						BlendInfo.TextureStageInfo[0].AlphaARG1 = D_D3DTA_DIFFUSE ;
						BlendInfo.TextureStageInfo[0].AlphaARG2 = D_D3DTA_DIFFUSE ;
						BlendInfo.TextureStageInfo[0].AlphaOP   = D_D3DTOP_SELECTARG2 ;
						BlendInfo.TextureStageInfo[0].ColorARG1 = D_D3DTA_DIFFUSE ;
						BlendInfo.TextureStageInfo[0].ColorARG2 = D_D3DTA_TEXTURE ;
						BlendInfo.TextureStageInfo[0].ColorOP   = D_D3DTOP_MODULATE4X ;

						BlendInfo.TextureStageInfo[1].AlphaARG1 = D_D3DTA_CURRENT ;
						BlendInfo.TextureStageInfo[1].AlphaARG2 = D_D3DTA_DIFFUSE ;
						BlendInfo.TextureStageInfo[1].AlphaOP   = D_D3DTOP_SELECTARG1 ;
						BlendInfo.TextureStageInfo[1].ColorARG1 = D_D3DTA_CURRENT ;
						BlendInfo.TextureStageInfo[1].ColorARG2 = D_D3DTA_DIFFUSE | D_D3DTA_ALPHAREPLICATE ;
						BlendInfo.TextureStageInfo[1].ColorOP   = D_D3DTOP_MODULATE ;

						BlendInfo.TextureStageInfo[2].AlphaARG1 = D_D3DTA_DIFFUSE ;
						BlendInfo.TextureStageInfo[2].AlphaARG2 = D_D3DTA_DIFFUSE ;
						BlendInfo.TextureStageInfo[2].AlphaOP   = D_D3DTOP_DISABLE ;
						BlendInfo.TextureStageInfo[2].ColorARG1 = D_D3DTA_DIFFUSE ;
						BlendInfo.TextureStageInfo[2].ColorARG2 = D_D3DTA_DIFFUSE ;
						BlendInfo.TextureStageInfo[2].ColorOP   = D_D3DTOP_DISABLE ;
						StageNum = 2 ;
						break ;
					}

					TextureStageInfo = BlendInfo.TextureStageInfo ;
				}
			}

			// ���e�X�g�͍s��Ȃ�
			BlendInfo.AlphaTestEnable = FALSE ;
			BlendInfo.AlphaRef        = GD3D9.Device.State.BlendInfo.AlphaRef ;
			BlendInfo.AlphaFunc       = GD3D9.Device.State.BlendInfo.AlphaFunc ;

			// ���_�^�C�v�̃Z�b�g
			GD3D9.Device.DrawInfo.VertexType = VERTEXTYPE_NOTEX ;
			GD3D9.Device.DrawInfo.VertexBufferNextAddr = GD3D9.Device.DrawInfo.VertexBufferAddr[ GD3D9.Device.DrawInfo.Use3DVertex ][ GD3D9.Device.DrawInfo.VertexType ] ;
		}
		else
		{
			// �J�����g�e�N�X�`��������ꍇ�̓e�N�X�`���X�e�[�W�u�����h�e�[�u������l���擾����

			// �u�����h�e�N�X�`���[�����邩�ǂ������擾����
			BlendTextureIndex = GD3D9.Device.DrawSetting.BlendTexture != NULL ? GD3D9.Device.DrawSetting.BlendGraphType + 1 : 0 ;

			// �g�p����e�N�X�`���u�����h�X�e�[�W�X�e�[�^�X�e�[�u���̃C���f�b�N�X���Z�b�g
			AlphaTableIndex = 0 ;
			if( GD3D9.Device.DrawSetting.AlphaChannelValidFlag || GD3D9.Device.DrawSetting.AlphaTestValidFlag || ( GD3D9.Device.State.ZEnable && BlendTextureIndex == 0 ) )
			{
				AlphaTableIndex = GD3D9.Device.DrawSetting.AlphaChannelValidFlag ? 1 : 2 ;
			}

			// �u�����h�e�N�X�`���[���g�p���邩�ǂ����ŕ���
			if( BlendTextureIndex != 0 )
			{
				int TableIndex ;

				// �u�����h�e�N�X�`���[������ꍇ�� AlphaTestValidFlag �͖���

				// �g�p����e�[�u���̃C���f�b�N�X������o��
				TableIndex = __RGBBlendStateTable[ NextBlendMode ][ RGB_BLENDSTATE_TABLE_TEXBLENDTABLEINDEX ] ;
/*
				if( NextBlendMode == DX_BLENDMODE_ADD_X4 || NextBlendMode == DX_BLENDMODE_ALPHA_X4 )
				{
					TableIndex = TEXTURE_BLEND_TABLE_INDEX_X4 ;
				}
				else
				if( NextBlendMode == DX_BLENDMODE_INVSRC )
				{
					TableIndex = TEXTURE_BLEND_TABLE_INDEX_INVERSE ;
				}
				else
				{
					TableIndex = TEXTURE_BLEND_TABLE_INDEX_NORMAL ;
				}
*/

				// �V�F�[�_�[���g�p���邩�ǂ����ŏ����𕪊�
				if( UseShaderFlag )
				{
					// �g�p����e�N�X�`���[�A�h���X���Z�b�g
					StageTexture[0] = GD3D9.Device.DrawSetting.RenderTexture ;
					StageTexture[1] = GD3D9.Device.DrawSetting.BlendTexture ;
					StageTexture[2] = NULL ;
					StageNum = 2 ;
				}
				else
				{
					// �e�N�X�`���[�X�e�[�W�X�e�[�g�̐ݒ�l���擾����
					TextureStageInfo = BlendInfo.TextureStageInfo ;
					memcpy( BlendInfo.TextureStageInfo,
						__TexStageStateTable[ TableIndex ][ BlendTextureIndex ][ GD3D9.Device.DrawSetting.UseDiffuseRGBColor ][ GD3D9.Device.DrawSetting.UseDiffuseAlphaColor ][ AlphaTableIndex ],
							 sizeof( GRAPHICS_HARDDATA_DIRECT3D9_TEXTURESTAGEINFO ) * USE_TEXTURESTAGE_NUM ) ;

/*
					int p[5] ;

					p[0] = TableIndex ;
					p[1] = BlendTextureIndex ;
					p[2] = GD3D9.Device.DrawSetting.UseDiffuseRGBColor ;
					p[3] = GD3D9.Device.DrawSetting.UseDiffuseAlphaColor ;
					p[4] = AlphaTableIndex ;
*/
					StageNum = ( DWORD )(DWORD_PTR)TextureStageInfo[0].Texture ;
				}

				// ���_�^�C�v�̃Z�b�g
				GD3D9.Device.DrawInfo.VertexType = VERTEXTYPE_BLENDTEX ;
				GD3D9.Device.DrawInfo.VertexBufferNextAddr = GD3D9.Device.DrawInfo.VertexBufferAddr[ GD3D9.Device.DrawInfo.Use3DVertex ][ GD3D9.Device.DrawInfo.VertexType ] ;

				// �u�����h�^�C�v�ɂ���ď����𕪊�
				switch( GD3D9.Device.DrawSetting.BlendGraphType )
				{
				case DX_BLENDGRAPHTYPE_NORMAL :
					// �Ƃ肠�����`�揈�����s��
					GD3D9.Device.DrawInfo.BlendMaxNotDrawFlag = FALSE ;

					// ���������L���ȏꍇ�̓��l���O�̏ꍇ�̂ݕ\������Ȃ��悤�ɂ���
					if( GD3D9.Device.DrawSetting.AlphaTestValidFlag == TRUE || BlendInfo.AlphaBlendEnable == TRUE || GD3D9.Device.DrawSetting.AlphaChannelValidFlag == TRUE )
					{
						BlendInfo.AlphaTestEnable = TRUE ;
						BlendInfo.AlphaRef        = 0 ;
						BlendInfo.AlphaFunc       = D_D3DCMP_GREATER ;
					}
					else
					{
						BlendInfo.AlphaTestEnable = FALSE ;
						BlendInfo.AlphaRef        = GD3D9.Device.State.BlendInfo.AlphaRef ;
						BlendInfo.AlphaFunc       = GD3D9.Device.State.BlendInfo.AlphaFunc ;
					}

					// �N���X�t�F�[�h����ݒ肷��
					BlendInfo.FactorColor = ( ( DWORD )GD3D9.Device.DrawSetting.BlendGraphFadeRatio << 24 ) | 0x00ffffff ;

					// �V�F�[�_�[���g�p���邩�ǂ����ŏ����𕪊�
					if( UseShaderFlag == FALSE )
					{
						// �g�p����e�N�X�`���[�A�h���X���Z�b�g�ƒ��_�^�C�v�A�e�N�X�`���X�e�[�W���g�p����e�N�X�`�����W�̃C���f�b�N�X���Z�b�g����
						StageTexture[0] = GD3D9.Device.DrawSetting.BlendTexture ;
						StageTexture[1] = GD3D9.Device.DrawSetting.RenderTexture ;
						StageTexture[2] = NULL ;

						// �e�N�X�`���J���[�����t���O�������Ă���ꍇ�̓e�N�X�`���v�f�𖳌��ɂ���
						if( GD3D9.Device.DrawSetting.IgnoreGraphColorFlag )
						{
							BlendInfo.TextureStageInfo[ 0 ].ColorARG1 = D_D3DTA_DIFFUSE ;
							BlendInfo.TextureStageInfo[ 0 ].ColorARG2 = -1 ;
							BlendInfo.TextureStageInfo[ 0 ].ColorOP = D_D3DTOP_SELECTARG1 ;
							BlendInfo.TextureStageInfo[ 1 ].ColorARG1 = D_D3DTA_CURRENT ;
							BlendInfo.TextureStageInfo[ 1 ].ColorARG2 = -1 ;
							BlendInfo.TextureStageInfo[ 1 ].ColorOP = D_D3DTOP_SELECTARG1 ;
							if( StageNum == 3 )
							{
								BlendInfo.TextureStageInfo[ 2 ].ColorARG1 = D_D3DTA_CURRENT ;
								BlendInfo.TextureStageInfo[ 2 ].ColorARG2 = -1 ;
								BlendInfo.TextureStageInfo[ 2 ].ColorOP = D_D3DTOP_SELECTARG1 ;
							}
						}
					}
#ifndef DX_NON_NORMAL_DRAW_SHADER
					else
					{
						// �g�p����V�F�[�_�[���Z�b�g
						UsePixelShader = GD3D9.Device.Shader._2D.BaseUseTexPixelShader[ 1 ][ TableIndex ][ GD3D9.Device.DrawSetting.IgnoreGraphColorFlag ? 1 : 0 ][ AlphaTableIndex == 0 ? 1 : 0 ][ 0/*GRH.UseAlphaChDrawShader*/ ] ;
					}
#endif // DX_NON_NORMAL_DRAW_SHADER
					break ;

				case DX_BLENDGRAPHTYPE_WIPE :
					// �u�����h���E�l���ő�̏ꍇ�͉����`�悵�Ȃ�
					GD3D9.Device.DrawInfo.BlendMaxNotDrawFlag = GD3D9.Device.DrawSetting.BlendGraphBorderParam == 255 ? TRUE : FALSE ;

					// �V�F�[�_�[���g�p���邩�ǂ����ŏ����𕪊�
					if( UseShaderFlag == FALSE )
					{
						// �g�p����e�N�X�`���[�A�h���X���Z�b�g�ƒ��_�^�C�v�A�e�N�X�`���X�e�[�W���g�p����e�N�X�`�����W�̃C���f�b�N�X���Z�b�g����
						StageTexture[0] = GD3D9.Device.DrawSetting.BlendTexture ;
						if( GD3D9.Device.DrawSetting.AlphaChannelValidFlag == TRUE )
						{
							StageTexture[1] = NULL ;
							StageTexture[2] = GD3D9.Device.DrawSetting.RenderTexture ;
						}
						else
						{
							StageTexture[1] = GD3D9.Device.DrawSetting.RenderTexture ;
						}
					}
#ifndef DX_NON_NORMAL_DRAW_SHADER
					else
					{
						// �g�p����V�F�[�_�[���Z�b�g
						UsePixelShader = GD3D9.Device.Shader._2D.BaseUseTexPixelShader[ 2 ][ TableIndex ][ GD3D9.Device.DrawSetting.IgnoreGraphColorFlag ? 1 : 0 ][ AlphaTableIndex == 0 ? 1 : 0 ][ 0/*GRH.UseAlphaChDrawShader*/ ] ;

						// float�^�� FactorColor ���g�p����
						UseFloatFactorColor = TRUE ;
						FloatFactorColor.x = 0.0f ;
						FloatFactorColor.y = 0.0f ;
						FloatFactorColor.z = 0.0f ;
						FloatFactorColor.w = 0.0f ;
					}
#endif // DX_NON_NORMAL_DRAW_SHADER

					// ���E�͈͂��P�ȉ����ǂ����ŏ����𕪊�
					if( GD3D9.Device.DrawSetting.BlendGraphBorderRange <= 1 )
					{
						// �����͂����w�Ǘ�O���� -------------------------------------------

						// ���e�X�g���g�p����
						BlendInfo.AlphaTestEnable = TRUE ;
						BlendInfo.AlphaRef = GD3D9.Device.DrawSetting.BlendGraphBorderParam ;
						BlendInfo.AlphaFunc = D_D3DCMP_GREATEREQUAL ;

						// ���u�����h�@�\�n�e�e
						BlendInfo.AlphaBlendEnable = FALSE ;

						// �������郿�l�͖���
						if( UseShaderFlag )
						{
							FloatFactorColor.w = 0.0f ;
							FloatFactorColor.z = 1.0f ;
						}
						else
						{
							BlendInfo.FactorColor = 0x00FFFFFF | ( 0 << 24 ) ;
						}

						// �����摜�����`�����l�����g�p���Ă��āA���� BlendGraphBorderParam �� 0 ��������
						// AlphaRef ���O�ɂ��Ȃ�
						if( ( GD3D9.Device.DrawSetting.AlphaChannelValidFlag || GD3D9.Device.DrawSetting.AlphaTestValidFlag ) && GD3D9.Device.DrawSetting.BlendGraphBorderParam == 0 )
						{
							BlendInfo.AlphaRef = 1 ;
						}

						// -------------------------------------------------------------------
					}
					else
					{
						int p ;
					
						// ���u�����h�ɂ��e�N�X�`���u�����h

						// ���e�X�g�͍s��Ȃ�
						BlendInfo.AlphaTestEnable = FALSE ;
						BlendInfo.AlphaRef        = GD3D9.Device.State.BlendInfo.AlphaRef ;
						BlendInfo.AlphaFunc       = GD3D9.Device.State.BlendInfo.AlphaFunc ;

						// ���u�����h�@�\�n�m
						BlendInfo.AlphaBlendEnable = TRUE ;

						// ���E�͈͂ɂ���ĂS�i�K
						if( GD3D9.Device.DrawSetting.BlendGraphBorderRange <= 64 )
						{
							// �~�S

							p = 1280 - GD3D9.Device.DrawSetting.BlendGraphBorderParam * 1280 / 255 ; 
							BlendInfo.TextureStageInfo[1].AlphaOP = D_D3DTOP_MODULATE4X ;

							// �u�����h�e�N�X�`���̃u�����h�l�ɂ���č����p�����[�^��ύX
							if( p < 1024 )
							{
								BlendInfo.FactorColor = ( DWORD )( 0x00FFFFFF | ( ( 255 - p * 255 / 1024 ) << 24 ) ) ;
								BlendInfo.TextureStageInfo[0].AlphaOP = D_D3DTOP_SUBTRACT ;

								if( UseShaderFlag )
								{
									FloatFactorColor.w = - ( float )( BlendInfo.FactorColor >> 24 ) / 255.0f ;
								}
							}
							else
							{
								BlendInfo.FactorColor = ( DWORD )( 0x00FFFFFF | ( ( 64 * ( p - 1024 ) / 256 ) << 24 ) ) ;
								BlendInfo.TextureStageInfo[0].AlphaOP = D_D3DTOP_ADD ;

								if( UseShaderFlag )
								{
									FloatFactorColor.w = ( float )( BlendInfo.FactorColor >> 24 ) / 255.0f ;
								}
							}

							if( UseShaderFlag )
							{
								FloatFactorColor.z = 4.0f ;
							}
						}
						else
						if( GD3D9.Device.DrawSetting.BlendGraphBorderRange <= 128 )
						{
							// �~�Q

							p = 768 - GD3D9.Device.DrawSetting.BlendGraphBorderParam * 768 / 255 ; 
							BlendInfo.TextureStageInfo[1].AlphaOP = D_D3DTOP_MODULATE2X ;

							// �u�����h�e�N�X�`���̃u�����h�l�ɂ���č����p�����[�^��ύX
							if( p < 512 )
							{
								BlendInfo.FactorColor = ( DWORD )( 0x00FFFFFF | ( ( 255 - p * 255 / 512 ) << 24 ) ) ;
								BlendInfo.TextureStageInfo[0].AlphaOP = D_D3DTOP_SUBTRACT ;

								if( UseShaderFlag )
								{
									FloatFactorColor.w = - ( float )( BlendInfo.FactorColor >> 24 ) / 255.0f ;
								}
							}
							else
							{
								BlendInfo.FactorColor = ( DWORD )( 0x00FFFFFF | ( ( 128 * ( p - 512 ) / 256 ) << 24 ) ) ;
								BlendInfo.TextureStageInfo[0].AlphaOP = D_D3DTOP_ADD ;

								if( UseShaderFlag )
								{
									FloatFactorColor.w = ( float )( BlendInfo.FactorColor >> 24 ) / 255.0f ;
								}
							}

							if( UseShaderFlag )
							{
								FloatFactorColor.z = 2.0f ;
							}
						}
						else
						{
							// �~�P

							p = 512 - GD3D9.Device.DrawSetting.BlendGraphBorderParam * 512 / 255 ; 
							BlendInfo.TextureStageInfo[1].AlphaOP = D_D3DTOP_MODULATE ;

							// �u�����h�e�N�X�`���̃u�����h�l�ɂ���č����p�����[�^��ύX
							if( p < 256 )
							{
								BlendInfo.FactorColor = ( DWORD )( 0x00FFFFFF | ( ( 255 - p * 255 / 256 ) << 24 ) ) ;
								BlendInfo.TextureStageInfo[0].AlphaOP = D_D3DTOP_SUBTRACT ;

								if( UseShaderFlag )
								{
									FloatFactorColor.w = - ( float )( BlendInfo.FactorColor >> 24 ) / 255.0f ;
								}
							}
							else
							{
								BlendInfo.FactorColor = ( DWORD )( 0x00FFFFFF | ( ( 255 * ( p - 256 ) / 256 ) << 24 ) ) ;
								BlendInfo.TextureStageInfo[0].AlphaOP = D_D3DTOP_ADD ;

								if( UseShaderFlag )
								{
									FloatFactorColor.w = ( float )( BlendInfo.FactorColor >> 24 ) / 255.0f ;
								}
							}

							if( UseShaderFlag )
							{
								FloatFactorColor.z = 1.0f ;
							}
						}
					}

					if( UseShaderFlag == FALSE )
					{
						// �e�N�X�`���J���[�����t���O�������Ă���ꍇ�̓e�N�X�`���v�f�𖳌��ɂ���
						if( GD3D9.Device.DrawSetting.IgnoreGraphColorFlag )
						{
							int TargetNumber ;

							if( GD3D9.Device.DrawSetting.AlphaChannelValidFlag == TRUE )
							{
								TargetNumber = 2 ;
							}
							else
							{
								TargetNumber = 1 ;
							}

							if( BlendInfo.TextureStageInfo[ TargetNumber ].ColorARG1 == D_D3DTA_TEXTURE &&
								BlendInfo.TextureStageInfo[ TargetNumber ].ColorOP == D_D3DTOP_SELECTARG1 )
							{
								BlendInfo.TextureStageInfo[ TargetNumber ].ColorARG1 = D_D3DTA_CURRENT ;
							}
							else
							if( BlendInfo.TextureStageInfo[ TargetNumber ].ColorARG1 == ( D_D3DTA_TEXTURE | D_D3DTA_COMPLEMENT ) &&
								BlendInfo.TextureStageInfo[ TargetNumber ].ColorOP == D_D3DTOP_SELECTARG1 )
							{
								BlendInfo.TextureStageInfo[ TargetNumber ].ColorARG1 = D_D3DTA_CURRENT | D_D3DTA_COMPLEMENT ;
							}
							else
							if( BlendInfo.TextureStageInfo[ TargetNumber ].ColorARG1 == D_D3DTA_TEXTURE &&
								( BlendInfo.TextureStageInfo[ TargetNumber ].ColorOP == D_D3DTOP_MODULATE ||
								  BlendInfo.TextureStageInfo[ TargetNumber ].ColorOP == D_D3DTOP_MODULATE2X ||
								  BlendInfo.TextureStageInfo[ TargetNumber ].ColorOP == D_D3DTOP_MODULATE4X ) )
							{
								BlendInfo.TextureStageInfo[ TargetNumber ].ColorOP = D_D3DTOP_SELECTARG2 ;
							}
						}
					}
					break ;

				case DX_BLENDGRAPHTYPE_ALPHA :
					// �Ƃ肠�����`�揈�����s��
					GD3D9.Device.DrawInfo.BlendMaxNotDrawFlag = FALSE ;

					// ���������L���ȏꍇ�̓��l���O�̏ꍇ�̂ݕ\������Ȃ��悤�ɂ���
					if( BlendInfo.AlphaBlendEnable == TRUE )
					{
						BlendInfo.AlphaTestEnable = TRUE ;
						BlendInfo.AlphaRef        = 0 ;
						BlendInfo.AlphaFunc       = D_D3DCMP_GREATER ;
					}
					else
					{
						BlendInfo.AlphaTestEnable = FALSE ;
						BlendInfo.AlphaRef        = GD3D9.Device.State.BlendInfo.AlphaRef ;
						BlendInfo.AlphaFunc       = GD3D9.Device.State.BlendInfo.AlphaFunc ;
					}

					// �V�F�[�_�[���g�p���邩�ǂ����ŏ����𕪊�
					if( UseShaderFlag == FALSE )
					{
						// �g�p����e�N�X�`���[�A�h���X���Z�b�g�ƒ��_�^�C�v�A�e�N�X�`���X�e�[�W���g�p����e�N�X�`�����W�̃C���f�b�N�X���Z�b�g����
						StageTexture[0] = GD3D9.Device.DrawSetting.RenderTexture ;
						StageTexture[1] = GD3D9.Device.DrawSetting.BlendTexture ;
						StageTexture[2] = NULL ;

						// �e�N�X�`���J���[�����t���O�������Ă���ꍇ�̓e�N�X�`���J���[�v�f�𖳌��ɂ���
						if( GD3D9.Device.DrawSetting.IgnoreGraphColorFlag )
						{
							BlendInfo.TextureStageInfo[ 0 ].ColorARG1 = D_D3DTA_DIFFUSE ;
						}
					}
#ifndef DX_NON_NORMAL_DRAW_SHADER
					else
					{
						// �g�p����V�F�[�_�[���Z�b�g
						UsePixelShader = GD3D9.Device.Shader._2D.BaseUseTexPixelShader[ 3 ][ TableIndex ][ GD3D9.Device.DrawSetting.IgnoreGraphColorFlag ? 1 : 0 ][ AlphaTableIndex == 0 ? 1 : 0 ][ 0/*GRH.UseAlphaChDrawShader*/ ] ;
					}
#endif // DX_NON_NORMAL_DRAW_SHADER
					break ;
				}
			}
			else
			{
				// �u�����h�e�N�X�`���[�������ꍇ

				// �Ƃ肠�����`�揈�����s��
				GD3D9.Device.DrawInfo.BlendMaxNotDrawFlag = FALSE ;

				// ���_�^�C�v�̃Z�b�g
				GD3D9.Device.DrawInfo.VertexType = VERTEXTYPE_TEX ;
				GD3D9.Device.DrawInfo.VertexBufferNextAddr = GD3D9.Device.DrawInfo.VertexBufferAddr[ GD3D9.Device.DrawInfo.Use3DVertex ][ GD3D9.Device.DrawInfo.VertexType ] ;

#ifndef DX_NON_NORMAL_DRAW_SHADER
				// �V�F�[�_�[���g�p���邩�ǂ����ŏ����𕪊�
				if( UseShaderFlag )
				{
					// �g�p����s�N�Z���V�F�[�_�[���Z�b�g
					UsePixelShader = GD3D9.Device.Shader._2D.BaseUseTexPixelShader[ 0 ][ __RGBBlendStateTable[NextBlendMode][RGB_BLENDSTATE_TABLE_TEXBLENDTABLEINDEX] ][ GD3D9.Device.DrawSetting.IgnoreGraphColorFlag ? 1 : 0 ][ AlphaTableIndex == 0 ? 1 : 0 ][ 0/*GRH.UseAlphaChDrawShader*/ ] ;

					// �g�p����e�N�X�`���̐��͈��
					StageNum = 1 ;
				}
				else
#endif // DX_NON_NORMAL_DRAW_SHADER
				{
					// �e�N�X�`���[�X�e�[�W�X�e�[�g�̐ݒ�l���擾����
					TextureStageInfo = __TexStageStateTable[ __RGBBlendStateTable[ NextBlendMode ][ RGB_BLENDSTATE_TABLE_TEXBLENDTABLEINDEX ] ][ FALSE ][ GD3D9.Device.DrawSetting.UseDiffuseRGBColor ][ GD3D9.Device.DrawSetting.UseDiffuseAlphaColor ][ AlphaTableIndex ] ;
					StageNum = ( DWORD )(DWORD_PTR)TextureStageInfo[0].Texture ;

					// �e�N�X�`���J���[�����t���O�������Ă���ꍇ�̓e�N�X�`���v�f�𖳌��ɂ���
					if( GD3D9.Device.DrawSetting.IgnoreGraphColorFlag )
					{
						_MEMCPY( TempTextureStageInfo, TextureStageInfo, sizeof( GRAPHICS_HARDDATA_DIRECT3D9_TEXTURESTAGEINFO ) * ( StageNum + 1 ) ) ;
						if( TempTextureStageInfo[ 0 ].ColorARG1 == D_D3DTA_TEXTURE )
						{
							TempTextureStageInfo[ 0 ].ColorARG1 = D_D3DTA_TFACTOR ;
						}
						else
						if( TempTextureStageInfo[ 0 ].ColorARG1 == ( D_D3DTA_TEXTURE | D_D3DTA_COMPLEMENT ) )
						{
							TempTextureStageInfo[ 0 ].ColorARG1 = D_D3DTA_TFACTOR | D_D3DTA_COMPLEMENT ;
						}
						TextureStageInfo = TempTextureStageInfo ;
					}
				}

				// ���e�X�g���g�p���邩�ǂ����ŏ����𕪊�
				if( GD3D9.Device.DrawSetting.AlphaTestValidFlag == TRUE )
				{
					// ���e�X�g���s��(���ߐF�����ȊO�Ƀ��e�X�g�̓u�����h�e�N�X�`���ł��g�p���Ă���)
					BlendInfo.AlphaTestEnable = TRUE ;
					BlendInfo.AlphaRef = 16 ;
					BlendInfo.AlphaFunc = D_D3DCMP_GREATER ;
				}
				else
				{
					// ���������L���ȏꍇ�̓u�����h���[�h�ɂ���Ă̓��l���O�̏ꍇ�̂ݕ\������Ȃ��悤�ɂ���
					if( BlendInfo.AlphaBlendEnable == TRUE &&
						__RGBBlendStateTable[ NextBlendMode ][ 6 ] == 1 )
					{
						BlendInfo.AlphaTestEnable = TRUE ;
						BlendInfo.AlphaRef        = 0 ;
						BlendInfo.AlphaFunc       = D_D3DCMP_GREATER ;
					}
					else
					{
						BlendInfo.AlphaTestEnable = FALSE ;
						BlendInfo.AlphaRef        = GD3D9.Device.State.BlendInfo.AlphaRef ;
						BlendInfo.AlphaFunc       = GD3D9.Device.State.BlendInfo.AlphaFunc ;
					}
				}

				// �g�p����e�N�X�`���[�A�h���X���Z�b�g
				StageTexture[0] = GD3D9.Device.DrawSetting.RenderTexture ;
			}
		}

		// ���`�����l���𐳂����v�Z����V�F�[�_�[���g�p����ꍇ�̓����_�����O�^�[�Q�b�g�̃e�N�X�`�����Z�b�g����
/*		if( GRH.UseAlphaChDrawShader )
		{
			StageTexture[ 2 ] = GRH.RenderTargetTexture ;
			if( StageNum < 3 )
			{
				for( i = StageNum ; i < 2 ; i ++ )
				{
					StageTexture[ i ] = NULL ;
				}
				StageNum = 3 ;
			}
		}*/
		
		// �������t���O�������Ă��邩�ǂ����ŏ����𕪊�	
		if( StageNum != ( DWORD )GD3D9.Device.State.BlendInfo.UseTextureStageNum )
		{
			int InitFlag ;
			
			GD3D9.Device.State.BlendInfo.UseTextureStageNum = ( int )StageNum ;
			
			InitFlag = GD3D9.Device.DrawSetting.CancelSettingEqualCheck ;

			GD3D9.Device.DrawSetting.CancelSettingEqualCheck = TRUE ;
			Graphics_D3D9_DeviceState_SetDrawMode( GD3D9.Device.State.DrawMode ) ;
			Graphics_D3D9_DeviceState_SetMaxAnisotropy( GD3D9.Device.State.MaxAnisotropy ) ;
			GD3D9.Device.DrawSetting.CancelSettingEqualCheck = InitFlag ;
		}
		if( StageNum < USE_TEXTURESTAGE_NUM ) StageNum ++ ;

		// �A���t�@�e�X�g�p�����[�^���L���ȏꍇ�͂����D�悷��
		if( GD3D9.Device.DrawSetting.AlphaTestMode != -1 )
		{
			BlendInfo.AlphaTestEnable = TRUE ;
			BlendInfo.AlphaFunc = GD3D9.Device.DrawSetting.AlphaTestMode ;
			BlendInfo.AlphaRef = GD3D9.Device.DrawSetting.AlphaTestParam ;
		}
	}
	if( GD3D9.Device.DrawSetting.CancelSettingEqualCheck == TRUE )
	{
		// �����Ă���ꍇ�͏�Ԃ�S�čX�V����

		Direct3DDevice9_SetRenderState( D_D3DRS_ALPHAFUNC, ( DWORD )BlendInfo.AlphaFunc ) ;
		GD3D9.Device.State.BlendInfo.AlphaFunc = BlendInfo.AlphaFunc ;

		Direct3DDevice9_SetRenderState( D_D3DRS_ALPHAREF, ( DWORD )BlendInfo.AlphaRef ) ;
		GD3D9.Device.State.BlendInfo.AlphaRef = BlendInfo.AlphaRef ;
		
		Direct3DDevice9_SetRenderState( D_D3DRS_ALPHATESTENABLE, ( DWORD )BlendInfo.AlphaTestEnable ) ;
		GD3D9.Device.State.BlendInfo.AlphaTestEnable = BlendInfo.AlphaTestEnable ;

		Direct3DDevice9_SetRenderState( D_D3DRS_ALPHABLENDENABLE, ( DWORD )BlendInfo.AlphaBlendEnable ) ;
		GD3D9.Device.State.BlendInfo.AlphaBlendEnable = BlendInfo.AlphaBlendEnable ;
			
		if( UseShaderFlag == FALSE )
		{
			Direct3DDevice9_SetRenderState( D_D3DRS_TEXTUREFACTOR, BlendInfo.FactorColor ) ;
		}
		GD3D9.Device.State.BlendInfo.FactorColor = BlendInfo.FactorColor ;

		{
			if( UseFloatFactorColor )
			{
				*( ( FLOAT4 * )GD3D9.Device.State.FactorColorPSConstantF ) = FloatFactorColor ;
			}
			else
			{
				GD3D9.Device.State.FactorColorPSConstantF[ 0 ] = (   BlendInfo.FactorColor         & 0xff ) / 255.0f ;
				GD3D9.Device.State.FactorColorPSConstantF[ 1 ] = ( ( BlendInfo.FactorColor >> 8  ) & 0xff ) / 255.0f ;
				GD3D9.Device.State.FactorColorPSConstantF[ 2 ] = ( ( BlendInfo.FactorColor >> 16 ) & 0xff ) / 255.0f ;
				GD3D9.Device.State.FactorColorPSConstantF[ 3 ] = ( ( BlendInfo.FactorColor >> 24 ) & 0xff ) / 255.0f ;
			}
			Graphics_D3D9_ShaderConstant_InfoSet_SetParam(
				&GD3D9.Device.Shader.ShaderConstantInfo,
				DX_SHADERCONSTANTTYPE_PS_FLOAT,
				DX_SHADERCONSTANTSET_LIB,
				DX_PS_CONSTF_FACTORCOLOR,
				GD3D9.Device.State.FactorColorPSConstantF,
				1,
				TRUE
			) ;
		}

		if( BlendInfo.SrcBlend != -1 )
		{
			Direct3DDevice9_SetRenderState( D_D3DRS_SRCBLEND, ( DWORD )BlendInfo.SrcBlend ) ;
			GD3D9.Device.State.BlendInfo.SrcBlend = BlendInfo.SrcBlend ;
		}
		else
		if( GD3D9.Device.State.BlendInfo.SrcBlend > 1 )
		{
			Direct3DDevice9_SetRenderState( D_D3DRS_SRCBLEND, ( DWORD )GD3D9.Device.State.BlendInfo.SrcBlend ) ;
		}

		if( BlendInfo.DestBlend != -1 )
		{
			Direct3DDevice9_SetRenderState( D_D3DRS_DESTBLEND, ( DWORD )BlendInfo.DestBlend ) ;
			GD3D9.Device.State.BlendInfo.DestBlend = BlendInfo.DestBlend ;
		}
		else
		if( GD3D9.Device.State.BlendInfo.DestBlend > 1 )
		{
			Direct3DDevice9_SetRenderState( D_D3DRS_DESTBLEND, ( DWORD )GD3D9.Device.State.BlendInfo.DestBlend ) ;
		}

		if( BlendInfo.BlendOp != -1 )
		{
			Direct3DDevice9_SetRenderState( D_D3DRS_BLENDOP, ( DWORD )BlendInfo.BlendOp ) ;
			GD3D9.Device.State.BlendInfo.BlendOp = BlendInfo.BlendOp ;
		}
		else
		if( GD3D9.Device.State.BlendInfo.BlendOp > 1 )
		{
			Direct3DDevice9_SetRenderState( D_D3DRS_BLENDOP, ( DWORD )GD3D9.Device.State.BlendInfo.BlendOp ) ;
		}

		Direct3DDevice9_SetRenderState( D_D3DRS_SEPARATEALPHABLENDENABLE, ( DWORD )BlendInfo.SeparateAlphaBlendEnable ) ;
		GD3D9.Device.State.BlendInfo.SeparateAlphaBlendEnable = BlendInfo.SeparateAlphaBlendEnable ;

		if( BlendInfo.SrcBlendAlpha != -1 )
		{
			Direct3DDevice9_SetRenderState( D_D3DRS_SRCBLENDALPHA, ( DWORD )BlendInfo.SrcBlendAlpha ) ;
			GD3D9.Device.State.BlendInfo.SrcBlendAlpha = BlendInfo.SrcBlendAlpha ;
		}
		else
		if( GD3D9.Device.State.BlendInfo.SrcBlendAlpha > 1 )
		{
			Direct3DDevice9_SetRenderState( D_D3DRS_SRCBLENDALPHA, ( DWORD )GD3D9.Device.State.BlendInfo.SrcBlendAlpha ) ;
		}

		if( BlendInfo.DestBlendAlpha != -1 )
		{
			Direct3DDevice9_SetRenderState( D_D3DRS_DESTBLENDALPHA, ( DWORD )BlendInfo.DestBlendAlpha ) ;
			GD3D9.Device.State.BlendInfo.DestBlendAlpha = BlendInfo.DestBlendAlpha ;
		}
		else
		if( GD3D9.Device.State.BlendInfo.DestBlendAlpha > 1 )
		{
			Direct3DDevice9_SetRenderState( D_D3DRS_DESTBLENDALPHA, ( DWORD )GD3D9.Device.State.BlendInfo.DestBlendAlpha ) ;
		}

		if( BlendInfo.BlendOpAlpha != -1 )
		{
			Direct3DDevice9_SetRenderState( D_D3DRS_BLENDOPALPHA, ( DWORD )BlendInfo.BlendOpAlpha ) ;
			GD3D9.Device.State.BlendInfo.BlendOpAlpha = BlendInfo.BlendOpAlpha ;
		}
		else
		if( GD3D9.Device.State.BlendInfo.BlendOpAlpha > 1 )
		{
			Direct3DDevice9_SetRenderState( D_D3DRS_BLENDOPALPHA, ( DWORD )GD3D9.Device.State.BlendInfo.BlendOpAlpha ) ;
		}

		if( UseShaderFlag )
		{
			Src = NULL ;
		}
		else
		{
			Src  = TextureStageInfo ;
		}
		Dest = GD3D9.Device.State.BlendInfo.TextureStageInfo ;
		for( i = 0 ; i < USE_TEXTURESTAGE_NUM ; i ++, Src ++, Dest ++ )
		{
//			if( i >= 2 && Src[-1].AlphaOP == D_D3DTOP_DISABLE && TextureOnly == FALSE ) break ;

			if( i < StageNum )
			{
				Direct3DDevice9_SetTexture( i, StageTexture[i] ) ;
				Dest->Texture = StageTexture[i] ;

				if( UseShaderFlag == FALSE )
				{
					if( Src->TextureCoordIndex != -1 && i < ( DWORD )GD3D9.Device.Caps.MaxTextureBlendStages )
					{
						Direct3DDevice9_SetTextureStageState( i, D_D3DTSS_TEXCOORDINDEX, ( DWORD )Src->TextureCoordIndex ) ;
						Dest->TextureCoordIndex = Src->TextureCoordIndex ;
					}
				}
			}
			else
			{
				Direct3DDevice9_SetTexture( i, NULL ) ;
				Dest->Texture = NULL ;

				if( UseShaderFlag == FALSE && i < ( DWORD )GD3D9.Device.Caps.MaxTextureBlendStages )
				{
					Direct3DDevice9_SetTextureStageState( i, D_D3DTSS_TEXCOORDINDEX, 0 ) ;
					Dest->TextureCoordIndex = 0 ;
				}
			}

			if( TextureOnly || ( UseShaderFlag && i != 0 ) ) continue ;

			if( UseShaderFlag )
			{
				Graphics_D3D9_DeviceState_SetPixelShader( UsePixelShader, TRUE ) ;
			}
			else
			{
				Graphics_D3D9_DeviceState_ResetPixelShader( FALSE ) ;

				if( i < StageNum && i < ( DWORD )GD3D9.Device.Caps.MaxTextureBlendStages )
				{
					if( GD3D9.Device.Caps.ValidTexTempRegFlag )
					{
						if( Src->ResultTempARG != -1 )
						{
							Direct3DDevice9_SetTextureStageState( i, D_D3DTSS_RESULTARG, ( DWORD )( Src->ResultTempARG ? D_D3DTA_TEMP : D_D3DTA_CURRENT ) ) ;
							Dest->ResultTempARG = Src->ResultTempARG ;
						}
					}

					if( Src->AlphaOP != -1 )
					{
						Direct3DDevice9_SetTextureStageState( i, D_D3DTSS_ALPHAOP, ( DWORD )Src->AlphaOP ) ;
						Dest->AlphaOP = Src->AlphaOP ;
					}

					if( Src->AlphaARG1 != -1 )
					{
						Direct3DDevice9_SetTextureStageState( i, D_D3DTSS_ALPHAARG1, ( DWORD )Src->AlphaARG1 ) ;
						Dest->AlphaARG1 = Src->AlphaARG1 ;
					}

					if( Src->AlphaARG2 != -1 )
					{
						Direct3DDevice9_SetTextureStageState( i, D_D3DTSS_ALPHAARG2, ( DWORD )Src->AlphaARG2 ) ;
						Dest->AlphaARG2 = Src->AlphaARG2 ;
					}

					if( Src->ColorOP != -1 )
					{
						Direct3DDevice9_SetTextureStageState( i, D_D3DTSS_COLOROP, ( DWORD )Src->ColorOP ) ;
						Dest->ColorOP = Src->ColorOP ;
					}

					if( Src->ColorARG1 != -1 )
					{
						Direct3DDevice9_SetTextureStageState( i, D_D3DTSS_COLORARG1, ( DWORD )Src->ColorARG1 ) ;
						Dest->ColorARG1 = Src->ColorARG1 ;
					}

					if( Src->ColorARG2 != -1 )
					{
						Direct3DDevice9_SetTextureStageState( i, D_D3DTSS_COLORARG2, ( DWORD )Src->ColorARG2 ) ;
						Dest->ColorARG2 = Src->ColorARG2 ;
					}
				}
				else
				{
					if( GD3D9.Device.Caps.ValidTexTempRegFlag )
					{
						Direct3DDevice9_SetTextureStageState( i, D_D3DTSS_RESULTARG, D_D3DTA_CURRENT ) ;
						Dest->ResultTempARG = FALSE ;
					}

					Direct3DDevice9_SetTextureStageState( i, D_D3DTSS_ALPHAOP, D_D3DTOP_DISABLE ) ;
					Dest->AlphaOP = D_D3DTOP_DISABLE ;

					Direct3DDevice9_SetTextureStageState( i, D_D3DTSS_ALPHAARG1, D_D3DTA_DIFFUSE ) ;
					Dest->AlphaARG1 = D_D3DTA_DIFFUSE ;

					Direct3DDevice9_SetTextureStageState( i, D_D3DTSS_ALPHAARG2, D_D3DTA_DIFFUSE ) ;
					Dest->AlphaARG2 = D_D3DTA_DIFFUSE ;

					Direct3DDevice9_SetTextureStageState( i, D_D3DTSS_COLOROP, D_D3DTOP_DISABLE ) ;
					Dest->ColorOP = D_D3DTOP_DISABLE ;

					Direct3DDevice9_SetTextureStageState( i, D_D3DTSS_COLORARG1, D_D3DTA_DIFFUSE ) ;
					Dest->ColorARG1 = D_D3DTA_DIFFUSE ;

					Direct3DDevice9_SetTextureStageState( i, D_D3DTSS_COLORARG2, D_D3DTA_DIFFUSE ) ;
					Dest->ColorARG2 = D_D3DTA_DIFFUSE ;
				}
			}
		}
	}
	else
	{
		// �����Ă��Ȃ��ꍇ�͕ύX�_�̂ݍĐݒ肷��

		if( BlendInfo.AlphaFunc != GD3D9.Device.State.BlendInfo.AlphaFunc )
		{
			Direct3DDevice9_SetRenderState( D_D3DRS_ALPHAFUNC, ( DWORD )BlendInfo.AlphaFunc ) ;
			GD3D9.Device.State.BlendInfo.AlphaFunc = BlendInfo.AlphaFunc ;
		}

		if( BlendInfo.AlphaRef != GD3D9.Device.State.BlendInfo.AlphaRef )
		{
			Direct3DDevice9_SetRenderState( D_D3DRS_ALPHAREF, ( DWORD )BlendInfo.AlphaRef ) ;
			GD3D9.Device.State.BlendInfo.AlphaRef = BlendInfo.AlphaRef ;
		}
		
		if( BlendInfo.AlphaTestEnable != GD3D9.Device.State.BlendInfo.AlphaTestEnable )
		{
			Direct3DDevice9_SetRenderState( D_D3DRS_ALPHATESTENABLE, ( DWORD )BlendInfo.AlphaTestEnable ) ;
			GD3D9.Device.State.BlendInfo.AlphaTestEnable = BlendInfo.AlphaTestEnable ;
		}
		
		if( BlendInfo.AlphaBlendEnable != GD3D9.Device.State.BlendInfo.AlphaBlendEnable )
		{
			Direct3DDevice9_SetRenderState( D_D3DRS_ALPHABLENDENABLE, ( DWORD )BlendInfo.AlphaBlendEnable ) ;
			GD3D9.Device.State.BlendInfo.AlphaBlendEnable = BlendInfo.AlphaBlendEnable ;
		}
			
		if( BlendInfo.FactorColor != GD3D9.Device.State.BlendInfo.FactorColor )
		{
			if( UseShaderFlag == FALSE )
			{
				Direct3DDevice9_SetRenderState( D_D3DRS_TEXTUREFACTOR, BlendInfo.FactorColor ) ;
			}
			GD3D9.Device.State.BlendInfo.FactorColor = BlendInfo.FactorColor ;
		}

		{
			float FactorData[ 4 ] ;

			if( UseFloatFactorColor )
			{
				*( ( FLOAT4 * )FactorData ) = FloatFactorColor ;
			}
			else
			{
				FactorData[ 0 ] = (   BlendInfo.FactorColor         & 0xff ) / 255.0f ;
				FactorData[ 1 ] = ( ( BlendInfo.FactorColor >> 8  ) & 0xff ) / 255.0f ;
				FactorData[ 2 ] = ( ( BlendInfo.FactorColor >> 16 ) & 0xff ) / 255.0f ;
				FactorData[ 3 ] = ( ( BlendInfo.FactorColor >> 24 ) & 0xff ) / 255.0f ;
			}
			if( _MEMCMP( GD3D9.Device.State.FactorColorPSConstantF, FactorData, sizeof( float ) * 4 ) != 0 )
			{
				_MEMCPY( GD3D9.Device.State.FactorColorPSConstantF, FactorData, sizeof( float ) * 4 ) ;
				Graphics_D3D9_ShaderConstant_InfoSet_SetParam(
					&GD3D9.Device.Shader.ShaderConstantInfo,
					DX_SHADERCONSTANTTYPE_PS_FLOAT,
					DX_SHADERCONSTANTSET_LIB,
					DX_PS_CONSTF_FACTORCOLOR,
					FactorData,
					1,
					TRUE
				) ;
			}
		}

		if( BlendInfo.SrcBlend != -1 && BlendInfo.SrcBlend != GD3D9.Device.State.BlendInfo.SrcBlend )
		{
			Direct3DDevice9_SetRenderState( D_D3DRS_SRCBLEND, ( DWORD )BlendInfo.SrcBlend ) ;
			GD3D9.Device.State.BlendInfo.SrcBlend = BlendInfo.SrcBlend ;
		}

		if( BlendInfo.DestBlend != -1 && BlendInfo.DestBlend != GD3D9.Device.State.BlendInfo.DestBlend )
		{
			Direct3DDevice9_SetRenderState( D_D3DRS_DESTBLEND, ( DWORD )BlendInfo.DestBlend ) ;
			GD3D9.Device.State.BlendInfo.DestBlend = BlendInfo.DestBlend ;
		}

		if( BlendInfo.BlendOp != -1 && BlendInfo.BlendOp != GD3D9.Device.State.BlendInfo.BlendOp )
		{
			Direct3DDevice9_SetRenderState( D_D3DRS_BLENDOP, ( DWORD )BlendInfo.BlendOp ) ;
			GD3D9.Device.State.BlendInfo.BlendOp = BlendInfo.BlendOp ;
		}

		if( BlendInfo.SeparateAlphaBlendEnable != GD3D9.Device.State.BlendInfo.SeparateAlphaBlendEnable )
		{
			Direct3DDevice9_SetRenderState( D_D3DRS_SEPARATEALPHABLENDENABLE, ( DWORD )BlendInfo.SeparateAlphaBlendEnable ) ;
			GD3D9.Device.State.BlendInfo.SeparateAlphaBlendEnable = BlendInfo.SeparateAlphaBlendEnable ;
		}

		if( BlendInfo.SrcBlendAlpha != -1 && BlendInfo.SrcBlendAlpha != GD3D9.Device.State.BlendInfo.SrcBlendAlpha )
		{
			Direct3DDevice9_SetRenderState( D_D3DRS_SRCBLENDALPHA, ( DWORD )BlendInfo.SrcBlendAlpha ) ;
			GD3D9.Device.State.BlendInfo.SrcBlendAlpha = BlendInfo.SrcBlendAlpha ;
		}

		if( BlendInfo.DestBlendAlpha != -1 && BlendInfo.DestBlendAlpha != GD3D9.Device.State.BlendInfo.DestBlendAlpha )
		{
			Direct3DDevice9_SetRenderState( D_D3DRS_DESTBLENDALPHA, ( DWORD )BlendInfo.DestBlendAlpha ) ;
			GD3D9.Device.State.BlendInfo.DestBlendAlpha = BlendInfo.DestBlendAlpha ;
		}

		if( BlendInfo.BlendOpAlpha != -1 && BlendInfo.BlendOpAlpha != GD3D9.Device.State.BlendInfo.BlendOpAlpha )
		{
			Direct3DDevice9_SetRenderState( D_D3DRS_BLENDOPALPHA, ( DWORD )BlendInfo.BlendOpAlpha ) ;
			GD3D9.Device.State.BlendInfo.BlendOpAlpha = BlendInfo.BlendOpAlpha ;
		}


		if( UseShaderFlag )
		{
			Src = NULL ;
		}
		else
		{
			Src  = TextureStageInfo ;
		}
		DWORD UseShaderStageNum = /* GRH.UseAlphaChDrawShader ? 3 : */ 2 ;
		Dest = GD3D9.Device.State.BlendInfo.TextureStageInfo ;
		for( i = 0 ; i < StageNum ; i ++, Src ++, Dest ++ )
		{
			if( i >= UseShaderStageNum && ( UseShaderFlag || Src[-1].AlphaOP == D_D3DTOP_DISABLE ) && TextureOnly == FALSE ) break ;

			if( StageTexture[i] != Dest->Texture )
			{
				HRESULT hr ;

				hr = Direct3DDevice9_SetTexture( i, StageTexture[i] ) ;
				Dest->Texture = StageTexture[i] ;
			}

			if( UseShaderFlag == FALSE )
			{
				if( Src->TextureCoordIndex != -1 &&
					Src->TextureCoordIndex != Dest->TextureCoordIndex &&
					i < ( DWORD )GD3D9.Device.Caps.MaxTextureBlendStages )
				{
					Direct3DDevice9_SetTextureStageState( i, D_D3DTSS_TEXCOORDINDEX, ( DWORD )Src->TextureCoordIndex ) ;
					Dest->TextureCoordIndex = Src->TextureCoordIndex ;
				}
			}

			if( TextureOnly || ( UseShaderFlag && i != 0 ) ) continue ;

			if( UseShaderFlag )
			{
				Graphics_D3D9_DeviceState_SetPixelShader( UsePixelShader, TRUE ) ;
			}
			else
			{
				Graphics_D3D9_DeviceState_ResetPixelShader( FALSE ) ;

				if( i < ( DWORD )GD3D9.Device.Caps.MaxTextureBlendStages )
				{
					if( GD3D9.Device.Caps.ValidTexTempRegFlag )
					{
						if( Src->ResultTempARG != -1 && Src->ResultTempARG != Dest->ResultTempARG )
						{
							Direct3DDevice9_SetTextureStageState( i, D_D3DTSS_RESULTARG, ( DWORD )( Src->ResultTempARG ? D_D3DTA_TEMP : D_D3DTA_CURRENT ) ) ;
							Dest->ResultTempARG = Src->ResultTempARG ;
						}
					}

					if( Src->AlphaOP != -1 && Src->AlphaOP != Dest->AlphaOP )
					{
						Direct3DDevice9_SetTextureStageState( i, D_D3DTSS_ALPHAOP, ( DWORD )Src->AlphaOP ) ;
						Dest->AlphaOP = Src->AlphaOP ;
					}

					if( Src->AlphaARG1 != -1 && Src->AlphaARG1 != Dest->AlphaARG1 )
					{
						Direct3DDevice9_SetTextureStageState( i, D_D3DTSS_ALPHAARG1, ( DWORD )Src->AlphaARG1 ) ;
						Dest->AlphaARG1 = Src->AlphaARG1 ;
					}

					if( Src->AlphaARG2 != -1 && Src->AlphaARG2 != Dest->AlphaARG2 )
					{
						Direct3DDevice9_SetTextureStageState( i, D_D3DTSS_ALPHAARG2, ( DWORD )Src->AlphaARG2 ) ;
						Dest->AlphaARG2 = Src->AlphaARG2 ;
					}

					if( Src->ColorOP != -1 && Src->ColorOP != Dest->ColorOP )
					{
						Direct3DDevice9_SetTextureStageState( i, D_D3DTSS_COLOROP, ( DWORD )Src->ColorOP ) ;
						Dest->ColorOP = Src->ColorOP ;
					}

					if( Src->ColorARG1 != -1 && Src->ColorARG1 != Dest->ColorARG1 )
					{
						Direct3DDevice9_SetTextureStageState( i, D_D3DTSS_COLORARG1, ( DWORD )Src->ColorARG1 ) ;
						Dest->ColorARG1 = Src->ColorARG1 ;
					}

					if( Src->ColorARG2 != -1 && Src->ColorARG2 != Dest->ColorARG2 )
					{
						Direct3DDevice9_SetTextureStageState( i, D_D3DTSS_COLORARG2, ( DWORD )Src->ColorARG2 ) ;
						Dest->ColorARG2 = Src->ColorARG2 ;
					}
				}
			}
		}
		for( ; i < USE_TEXTURESTAGE_NUM ; i ++, Dest ++ )
		{
			if( Dest->Texture != NULL )
			{
				Direct3DDevice9_SetTexture( i, NULL ) ;
				Dest->Texture = NULL ;
			}
		}
	}
	
	// �I��
	return 0 ;
}

// �A���r�G���g���C�g�ƃG�~�b�V�u�J���[���|�����킹���p�����[�^���X�V����
extern	void	Graphics_D3D9_DeviceState_RefreshAmbientAndEmissiveParam( void )
{
	int i ;
	COLOR_F Result ;

	// ���ʒl��������
	Result.r = 0.0f ;
	Result.g = 0.0f ;
	Result.b = 0.0f ;
	Result.a = 0.0f ;

	// �L���ȃ��C�g�̃A���r�G���g�J���[�ƃ}�e���A���̃A���r�G���g�J���[����Z�������̂��V�F�[�_�[�ɃZ�b�g����
	for( i = 0 ; i <= GD3D9.Device.State.LightEnableMaxIndex ; i ++ )
	{
		if( GD3D9.Device.State.LightEnableFlag[ i ] == FALSE ) continue ;
		Result.r = GD3D9.Device.State.LightParam[ i ].Ambient.r * GD3D9.Device.State.Material.Ambient.r ;
		Result.g = GD3D9.Device.State.LightParam[ i ].Ambient.g * GD3D9.Device.State.Material.Ambient.g ;
		Result.b = GD3D9.Device.State.LightParam[ i ].Ambient.b * GD3D9.Device.State.Material.Ambient.b ;
		Result.a = GD3D9.Device.State.LightParam[ i ].Ambient.a * GD3D9.Device.State.Material.Ambient.a ;

		// �o�[�e�b�N�X�V�F�[�_�[�ƃs�N�Z���V�F�[�_�[�ɔ��f����
		if( i < DX_VS_CONSTB_LIGHT_NUM )
		{
			Graphics_D3D9_ShaderConstant_InfoSet_SetParam(
				&GD3D9.Device.Shader.ShaderConstantInfo,
				DX_SHADERCONSTANTTYPE_VS_FLOAT,
				DX_SHADERCONSTANTSET_LIB,
				DX_VS_CONSTF_LIGHT_START + DX_VS_CONSTF_LIGHT_UNITSIZE * i + DX_VS_CONSTF_LGT_AMBIENT,
				&Result,
				1,
				TRUE
			) ;
			Graphics_D3D9_ShaderConstant_InfoSet_SetParam(
				&GD3D9.Device.Shader.ShaderConstantInfo,
				DX_SHADERCONSTANTTYPE_PS_FLOAT,
				DX_SHADERCONSTANTSET_LIB,
				DX_PS_CONSTF_LIGHT_START  + DX_PS_CONSTF_LIGHT_UNITSIZE  * i + DX_PS_CONSTF_LGT_AMBIENT,
				&Result,
				1,
				TRUE
			) ;
		}
		if( i < DX_PS_CONSTB_LIGHT2_NUM )
		{
			Graphics_D3D9_ShaderConstant_InfoSet_SetParam(
				&GD3D9.Device.Shader.ShaderConstantInfo,
				DX_SHADERCONSTANTTYPE_PS_FLOAT,
				DX_SHADERCONSTANTSET_LIB,
				DX_PS_CONSTF_LIGHT2_START  + DX_PS_CONSTF_LIGHT2_UNITSIZE  * i + DX_PS_CONSTF_LGT2_AMBIENT,
				&Result,
				1,
				TRUE
			) ;
		}
	}

	// �O���[�o���A���r�G���g���C�g�J���[�ƃ}�e���A���A���r�G���g�J���[����Z�������̂ƃG�~�b�V�u�J���[�����Z�������̂��쐬����
	Result.r = GD3D9.Device.State.GlobalAmbientLightColor.r * GD3D9.Device.State.Material.Ambient.r + GD3D9.Device.State.Material.Emissive.r ;
	Result.g = GD3D9.Device.State.GlobalAmbientLightColor.g * GD3D9.Device.State.Material.Ambient.g + GD3D9.Device.State.Material.Emissive.g ;
	Result.b = GD3D9.Device.State.GlobalAmbientLightColor.b * GD3D9.Device.State.Material.Ambient.b + GD3D9.Device.State.Material.Emissive.b ;
	Result.a = GD3D9.Device.State.GlobalAmbientLightColor.a * GD3D9.Device.State.Material.Ambient.a + GD3D9.Device.State.Material.Emissive.a ;

	// �o�[�e�b�N�X�V�F�[�_�[�ƃs�N�Z���V�F�[�_�[�ɔ��f����
	Graphics_D3D9_ShaderConstant_InfoSet_SetParam(
		&GD3D9.Device.Shader.ShaderConstantInfo,
		DX_SHADERCONSTANTTYPE_VS_FLOAT,
		DX_SHADERCONSTANTSET_LIB,
		DX_VS_CONSTF_AMBIENT_EMISSIVE,
		&Result,
		1,
		TRUE
	) ;
	Graphics_D3D9_ShaderConstant_InfoSet_SetParam(
		&GD3D9.Device.Shader.ShaderConstantInfo,
		DX_SHADERCONSTANTTYPE_PS_FLOAT,
		DX_SHADERCONSTANTSET_LIB,
		DX_PS_CONSTF_AMBIENT_EMISSIVE,
		&Result,
		1,
		TRUE
	) ;
}
























// �`��ݒ�֌W�֐�

// �`��u�����h���[�h�̐ݒ�
extern	int		Graphics_D3D9_DrawSetting_SetDrawBlendMode( int BlendMode, int AlphaTestValidFlag, int AlphaChannelValidFlag )
{
	if( GAPIWin.Direct3DDevice9Object == NULL ) return -1 ;

	if( GD3D9.Device.DrawSetting.CancelSettingEqualCheck == FALSE &&
		GD3D9.Device.DrawSetting.BlendMode == BlendMode &&
		GD3D9.Device.DrawSetting.AlphaTestValidFlag == AlphaTestValidFlag &&
		GD3D9.Device.DrawSetting.AlphaChannelValidFlag == AlphaChannelValidFlag ) return 0 ;

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	GD3D9.Device.DrawSetting.BlendMode             = BlendMode ;
	GD3D9.Device.DrawSetting.AlphaChannelValidFlag = AlphaChannelValidFlag ;
	GD3D9.Device.DrawSetting.AlphaTestValidFlag    = AlphaTestValidFlag ;
	GD3D9.Device.DrawSetting.DrawPrepAlwaysFlag    = TRUE ;

	// �p�����[�^���ύX���ꂽ�A�t���O�𗧂Ă�
	GD3D9.Device.DrawSetting.ChangeBlendParamFlag = TRUE ;

	return 0 ;
}

// �`�掞�̉摜�̐F�𖳎����邩�ǂ�����ݒ肷��
extern	int		Graphics_D3D9_DrawSetting_SetIgnoreDrawGraphColor( int EnableFlag )
{
	if( GAPIWin.Direct3DDevice9Object == NULL ) return -1 ;

	if( GD3D9.Device.DrawSetting.CancelSettingEqualCheck == FALSE &&
		GD3D9.Device.DrawSetting.IgnoreGraphColorFlag == EnableFlag )
		return 0 ;

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	GD3D9.Device.DrawSetting.IgnoreGraphColorFlag = EnableFlag ;
	GD3D9.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

	// �p�����[�^���ύX���ꂽ�A�t���O�𗧂Ă�
	GD3D9.Device.DrawSetting.ChangeBlendParamFlag = TRUE ;

	return 0 ;
}

// �A���t�@�e�X�g�̐ݒ�
extern	int		Graphics_D3D9_DrawSetting_SetDrawAlphaTest( int TestMode, int TestParam )
{
	if( GAPIWin.Direct3DDevice9Object == NULL ) return -1 ;

	if( GD3D9.Device.DrawSetting.CancelSettingEqualCheck == FALSE && GD3D9.Device.DrawSetting.AlphaTestMode == TestMode && GD3D9.Device.DrawSetting.AlphaTestParam == TestParam ) return 0 ;

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	GD3D9.Device.DrawSetting.AlphaTestMode  = TestMode ;
	GD3D9.Device.DrawSetting.AlphaTestParam = TestParam ;
	GD3D9.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

	// �p�����[�^���ύX���ꂽ�A�t���O�𗧂Ă�
	GD3D9.Device.DrawSetting.ChangeBlendParamFlag = TRUE ;

	return 0 ;
}

// �f�B�t���[�Y�J���[�̃��l���g�p���邩�ǂ����t���O���Z�b�g����
extern	int		Graphics_D3D9_DrawSetting_SetUseDiffuseAlphaColorFlag( int UseFlag )
{
	if( GAPIWin.Direct3DDevice9Object == NULL ) return -1 ;

	if( UseFlag == GD3D9.Device.DrawSetting.UseDiffuseAlphaColor && GD3D9.Device.DrawSetting.CancelSettingEqualCheck == FALSE ) return 0 ;

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �p�����[�^�̕ۑ�
	GD3D9.Device.DrawSetting.UseDiffuseAlphaColor = UseFlag ;
	GD3D9.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

	// �p�����[�^���ύX���ꂽ�A�t���O�𗧂Ă�
	GD3D9.Device.DrawSetting.ChangeBlendParamFlag = TRUE ;

	// �I��
	return 0 ;
}

// �f�B�t���[�Y�J���[�̂q�f�a�l���g�p���邩�ǂ����t���O���Z�b�g����
extern	int		Graphics_D3D9_DrawSetting_SetUseDiffuseRGBColorFlag( int UseFlag )
{
	if( GAPIWin.Direct3DDevice9Object == NULL ) return -1 ;

	if( UseFlag == GD3D9.Device.DrawSetting.UseDiffuseRGBColor && GD3D9.Device.DrawSetting.CancelSettingEqualCheck == FALSE ) return 0 ;

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �p�����[�^�̕ۑ�
	GD3D9.Device.DrawSetting.UseDiffuseRGBColor = UseFlag ;
	GD3D9.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

	// �p�����[�^���ύX���ꂽ�A�t���O�𗧂Ă�
	GD3D9.Device.DrawSetting.ChangeBlendParamFlag = TRUE ;

	// �I��
	return 0 ;
}

// �u�����h����e�N�X�`���̃p�����[�^���Z�b�g����
extern	int		Graphics_D3D9_DrawSetting_SetBlendTextureParam( int BlendType, int *Param )
{
	if( GAPIWin.Direct3DDevice9Object == NULL ) return -1 ;

	// �p�����[�^��ۑ�
	switch( BlendType )
	{
	case DX_BLENDGRAPHTYPE_NORMAL :
		if( GD3D9.Device.DrawSetting.CancelSettingEqualCheck == FALSE &&
			BlendType == GD3D9.Device.DrawSetting.BlendGraphType &&
			GD3D9.Device.DrawSetting.BlendGraphFadeRatio == Param[ 0 ] )
			return 0 ;

		if( GD3D9.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
		{
			// �`��ҋ@���Ă���`�敨��`��
			DRAWSTOCKINFO
		}

		GD3D9.Device.DrawSetting.BlendGraphFadeRatio = Param[ 0 ] ;
		break;

	case DX_BLENDGRAPHTYPE_WIPE:
		if( GD3D9.Device.DrawSetting.CancelSettingEqualCheck == FALSE &&
			BlendType == GD3D9.Device.DrawSetting.BlendGraphType &&
			GD3D9.Device.DrawSetting.BlendGraphBorderParam == Param[ 0 ] &&
			GD3D9.Device.DrawSetting.BlendGraphBorderRange == Param[ 1 ] )
			return 0 ;

		if( GD3D9.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
		{
			// �`��ҋ@���Ă���`�敨��`��
			DRAWSTOCKINFO
		}

		GD3D9.Device.DrawSetting.BlendGraphBorderParam = Param[ 0 ] ;
		GD3D9.Device.DrawSetting.BlendGraphBorderRange = Param[ 1 ] ;
		break;

	case DX_BLENDGRAPHTYPE_ALPHA :
		if( GD3D9.Device.DrawSetting.CancelSettingEqualCheck == FALSE &&
			BlendType == GD3D9.Device.DrawSetting.BlendGraphType )
			return 0 ;

		if( GD3D9.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
		{
			// �`��ҋ@���Ă���`�敨��`��
			DRAWSTOCKINFO
		}
		break;
	}

	// �u�����h�^�C�v��ۑ�����
	GD3D9.Device.DrawSetting.BlendGraphType = BlendType ;

	// �p�����[�^���ύX���ꂽ�A�t���O�𗧂Ă�
	GD3D9.Device.DrawSetting.ChangeBlendParamFlag = TRUE ;
	GD3D9.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

	// �I��
	return 0 ;
}

// �e�N�X�`�����Z�b�g���� 
extern	int		Graphics_D3D9_DrawSetting_SetTexture( D_IDirect3DTexture9 *RenderTexture )
{
	if( GAPIWin.Direct3DDevice9Object == NULL ) return -1 ;

	if( RenderTexture == GD3D9.Device.DrawSetting.RenderTexture && GD3D9.Device.DrawSetting.CancelSettingEqualCheck == FALSE ) return 0 ;

	if( GD3D9.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
	{
		// �`��ҋ@���Ă���`�敨��`��
		DRAWSTOCKINFO
	}

	// �e�N�X�`�����ύX���ꂽ�A�t���O�𗧂Ă�
	GD3D9.Device.DrawSetting.ChangeTextureFlag = TRUE ;

	// �e�N�X�`���̗L�薳�����ύX���ꂽ�ꍇ�̓u�����h�p�����[�^�̕ύX���s���t���O���Z�b�g����
	if( ( GD3D9.Device.DrawSetting.RenderTexture == NULL && RenderTexture != NULL ) ||
		( GD3D9.Device.DrawSetting.RenderTexture != NULL && RenderTexture == NULL ) )
	{
		GD3D9.Device.DrawSetting.ChangeBlendParamFlag = TRUE ;
	}

	// �e�N�X�`���Z�b�g
	GD3D9.Device.DrawSetting.RenderTexture = RenderTexture ;
	GD3D9.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

	// �I��
	return 0 ;
}

// �u�����h����e�N�X�`�����Z�b�g���� 
extern	int		Graphics_D3D9_DrawSetting_SetBlendTexture( D_IDirect3DTexture9 *BlendTexture, int TexWidth, int TexHeight )
{
	if( GAPIWin.Direct3DDevice9Object == NULL ) return -1 ;

	if( GD3D9.Device.DrawSetting.CancelSettingEqualCheck == FALSE && BlendTexture == GD3D9.Device.DrawSetting.BlendTexture ) return 0 ;

	if( GD3D9.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
	{
		// �`��ҋ@���Ă���`�敨��`��
		DRAWSTOCKINFO
	}

	// �e�N�X�`�����ύX���ꂽ�A�t���O�𗧂Ă�
	GD3D9.Device.DrawSetting.ChangeTextureFlag = TRUE ;
	GD3D9.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

	// �u�����h�e�N�X�`���̗L�薳�����ύX���ꂽ�ꍇ�̓u�����h�p�����[�^�̕ύX���s���t���O���Z�b�g����
	if( ( GD3D9.Device.DrawSetting.BlendTexture == NULL && BlendTexture != NULL ) ||
		( GD3D9.Device.DrawSetting.BlendTexture != NULL && BlendTexture == NULL ) )
		GD3D9.Device.DrawSetting.ChangeBlendParamFlag = TRUE ;

	// �e�N�X�`���Z�b�g
	GD3D9.Device.DrawSetting.BlendTexture = BlendTexture ;

	// �e�N�X�`���̕��ƍ�����ۑ�
	if( BlendTexture != NULL )
	{
		GD3D9.Device.DrawSetting.BlendTextureWidth  = ( float )TexWidth  ;
		GD3D9.Device.DrawSetting.BlendTextureHeight = ( float )TexHeight ;
		GD3D9.Device.DrawSetting.InvBlendTextureWidth  = 1.0F / GD3D9.Device.DrawSetting.BlendTextureWidth ;
		GD3D9.Device.DrawSetting.InvBlendTextureHeight = 1.0F / GD3D9.Device.DrawSetting.BlendTextureHeight ;
	}

	// �I��
	return 0 ;
}

























// �V�F�[�_�[�萔�Ǘ��\���̏����֐�

// �V�F�[�_�[�萔�g�p�̈��������������
extern int Graphics_D3D9_ShaderConstant_UseArea_Initialize( DIRECT3D9_SHADERCONSTANT_USEAREA *UseArea, int TotalSize )
{
	_MEMSET( UseArea, 0, sizeof( DIRECT3D9_SHADERCONSTANT_USEAREA ) ) ;

	// �Ǘ�����̈�̃T�C�Y��ݒ�
	UseArea->TotalSize = TotalSize ;

	// �I��
	return 0 ;
}

// �V�F�[�_�[�萔�g�p�̈����ύX����
extern int Graphics_D3D9_ShaderConstant_UseArea_Set( DIRECT3D9_SHADERCONSTANT_USEAREA *UseArea, int IsUse, int Index, int Num )
{
	DIRECT3D9_SHADERCONST_ONE_USEAREA *OneArea ;
	int i ;
	int j ;
	int StartIndex ;
	int EndPlusOneIndex ;
	int Bottom ;
	int NoHit ;

	// �ύX�����O�������牽�����Ȃ�
	if( Num == 0 )
		return 0 ;

	StartIndex = Index ;
	EndPlusOneIndex = Index + Num ;

	// �S�������������ĂȂ����𒲂ׂ�
	Bottom = TRUE ;
	NoHit = FALSE ;
	OneArea = UseArea->AreaInfo ;
	i = 0 ;
	while( i < UseArea->AreaInfoNum && OneArea->Start > EndPlusOneIndex )
	{
		i ++ ;
		OneArea ++ ;
	}
	if( i != UseArea->AreaInfoNum )
	{
		Bottom = FALSE ;

		// ����������Ƃ���܂ŃA�h���X��i�߂�
		OneArea = UseArea->AreaInfo ;
		i = 0 ;
		while( i < UseArea->AreaInfoNum && OneArea->EndPlusOne <= StartIndex )
		{
			i ++ ;
			OneArea ++ ;
		}

		// �����͈̔͂ɐG��Ă��邩���`�F�b�N
		if( OneArea->Start > EndPlusOneIndex )
		{
			NoHit = TRUE ;
		}
	}

	// �g�p�̈悪������̂��ǂ����ŏ����𕪊�
	if( IsUse == FALSE )
	{
		// ����ꍇ

		// �������������Ȃ�������I��
		if( i == UseArea->AreaInfoNum || NoHit )
			return 0 ;

		// �ŏ��Ɉ������������͈͓��Ɏ��܂�ꍇ�͔͈͏��̐���������
		if( OneArea->Start      < StartIndex &&
			OneArea->EndPlusOne > EndPlusOneIndex )
		{
			_MEMMOVE( OneArea + 1, OneArea, sizeof( DIRECT3D9_SHADERCONST_ONE_USEAREA ) * ( UseArea->AreaInfoNum - i ) ) ;
			OneArea[ 0 ].EndPlusOne = ( WORD )StartIndex ;
			OneArea[ 1 ].Start = ( WORD )EndPlusOneIndex ;
			UseArea->AreaInfoNum ++ ;
			return 0 ;
		}

		// �ŏ��Ɉ������������͈͓��̊J�n�ʒu�Ɠ������Ȃ��ꍇ�̓T�C�Y�𒲐�
		if( OneArea->Start < StartIndex )
		{
			OneArea->EndPlusOne = ( WORD )StartIndex ;
			OneArea ++ ;
			i ++ ;
		}

		// �ہX�܂܂�Ă��܂��͈͂̐��𒲂ׂ�
		j = 0 ;
		while( i < UseArea->AreaInfoNum && OneArea[ j ].EndPlusOne <= EndPlusOneIndex )
		{
			j ++ ;
			i ++ ;
		}

		// �ہX�܂܂�Ă��܂��͈͏����폜
		if( j > 0 && UseArea->AreaInfoNum - i > 0 )
		{
			_MEMMOVE( OneArea, OneArea + j, sizeof( DIRECT3D9_SHADERCONST_ONE_USEAREA ) * ( UseArea->AreaInfoNum - i ) ) ;
		}
		UseArea->AreaInfoNum -= j ;

		// �c�肪����ꍇ�Ŕ͈͂�����Ă���ꍇ�͔͈͂̊J�n�ʒu�����炷
		if( UseArea->AreaInfoNum > 0 && OneArea->Start < EndPlusOneIndex )
		{
			OneArea->Start = ( WORD )EndPlusOneIndex ;
		}
	}
	else
	{
		// ������ꍇ

		// �������������Ȃ�������͈͏���ǉ�
		if( i == UseArea->AreaInfoNum )
		{
			// ������O�͈̔͂Ɛڂ��Ă������O�͈̔͏����L����
			if( i > 0 && OneArea[ -1 ].EndPlusOne == StartIndex )
			{
				OneArea[ -1 ].EndPlusOne = ( WORD )EndPlusOneIndex ;
			}
			else
			{
				if( Bottom )
				{
					OneArea = UseArea->AreaInfo ;
					_MEMMOVE( OneArea + 1, OneArea, sizeof( DIRECT3D9_SHADERCONST_ONE_USEAREA ) * UseArea->AreaInfoNum ) ;
				}
				OneArea->Start = ( WORD )StartIndex ;
				OneArea->EndPlusOne = ( WORD )EndPlusOneIndex ;
				UseArea->AreaInfoNum ++ ;
			}

			return 0 ;
		}
		if( NoHit )
		{
			// ������O�͈̔͂Ɛڂ��Ă������O�͈̔͏����L����
			if( i > 0 && OneArea[ -1 ].EndPlusOne == StartIndex )
			{
				OneArea[ -1 ].EndPlusOne = ( WORD )EndPlusOneIndex ;
			}
			else
			{
				_MEMMOVE( OneArea + 1, OneArea, sizeof( DIRECT3D9_SHADERCONST_ONE_USEAREA ) * ( UseArea->AreaInfoNum - i ) ) ;
				OneArea->Start = ( WORD )StartIndex ;
				OneArea->EndPlusOne = ( WORD )EndPlusOneIndex ;
				UseArea->AreaInfoNum ++ ;
			}
			return 0 ;
		}

		// �ŏ��̈�O�͈̔͏��Ɛڂ��Ă����ꍇ�͑Ώۂ���O�͈̔͏��ɂ���
		if( i > 0 && OneArea[ -1 ].EndPlusOne == StartIndex )
		{
			OneArea -- ;
			i -- ;
		}

		// �ŏ��Ɉ������������͈͏��̊J�n�ʒu���J�n�ʒu���Ⴉ������L����
		if( OneArea->Start > StartIndex )
		{
			OneArea->Start = ( WORD )StartIndex ;
		}

		// �ŏ��Ɉ������������͈͏��Ɏ��܂�ꍇ�͂����ŏI��
		if( OneArea->EndPlusOne >= EndPlusOneIndex )
			return 0 ;

		// ���܂�Ȃ��ꍇ�͔͈͂��L����
		OneArea->EndPlusOne = ( WORD )EndPlusOneIndex ;
		StartIndex = OneArea->Start ;

		// �ȍ~�͈̔͏��ŊہX����Ă���͈͂̐��𒲂ׂ�
		OneArea ++ ;
		i ++ ;
		j = 0 ;
		while( i < UseArea->AreaInfoNum && OneArea[ j ].EndPlusOne <= EndPlusOneIndex )
		{
			j ++ ;
			i ++ ;
		}

		// �c�肪����ꍇ�Ŕ͈͂�����Ă�����͈͂�ǉ����č폜
		if( i < UseArea->AreaInfoNum && OneArea[ j ].Start <= EndPlusOneIndex )
		{
			OneArea[ -1 ].EndPlusOne = OneArea[ j ].EndPlusOne ;
			j ++ ;
			i ++ ;
		}

		// �͈͏����폜
		if( j > 0 && UseArea->AreaInfoNum - i > 0 )
		{
			_MEMMOVE( OneArea, OneArea + j, sizeof( DIRECT3D9_SHADERCONST_ONE_USEAREA ) * ( UseArea->AreaInfoNum - i ) ) ;
		}
		UseArea->AreaInfoNum -= j ;
	}

	// �I��
	return 0 ;
}

// �V�F�[�_�[�萔�g�p�̈��񂩂�g�p�}�b�v���쐬����
extern int Graphics_D3D9_ShaderConstant_UseArea_Create( DIRECT3D9_SHADERCONSTANT_USEAREA *UseArea, BYTE *Map, BYTE SetNumber )
{
	int i, j ;
	DIRECT3D9_SHADERCONST_ONE_USEAREA *AInfo ;

	AInfo = UseArea->AreaInfo ;
	for( i = 0 ; i < UseArea->AreaInfoNum ; i ++, AInfo ++ )
	{
		for( j = AInfo->Start ; j < AInfo->EndPlusOne ; j ++ )
		{
			Map[ j ] = SetNumber ;
		}
	}

	// �I��
	return 0 ;
}




// �V�F�[�_�[�萔���̏�����
extern int Graphics_D3D9_ShaderConstant_InfoSet_Initialize( DIRECT3D9_SHADERCONSTANTINFOSET *ConstInfoSet )
{
	int i ;

	// �Ƃ肠�����돉����
	_MEMSET( ConstInfoSet, 0, sizeof( *ConstInfoSet ) ) ;

	// SetMap �� 0xff �Ŗ��߂�
	_MEMSET( ConstInfoSet->SetMap, 0xff, sizeof( ConstInfoSet->SetMap ) ) ;

	// �V�F�[�_�[�萔�̎g�p�̈��������������
	for( i = 0 ; i < DX_SHADERCONSTANTSET_NUM ; i ++ )
	{
		Graphics_D3D9_ShaderConstant_UseArea_Initialize( &ConstInfoSet->Info[ DX_SHADERCONSTANTTYPE_VS_FLOAT ][ i ].UseArea, 256 ) ;
		ConstInfoSet->Info[ DX_SHADERCONSTANTTYPE_VS_FLOAT ][ i ].UnitSize = sizeof( FLOAT4 ) ;

		Graphics_D3D9_ShaderConstant_UseArea_Initialize( &ConstInfoSet->Info[ DX_SHADERCONSTANTTYPE_VS_INT   ][ i ].UseArea,  16 ) ;
		ConstInfoSet->Info[ DX_SHADERCONSTANTTYPE_VS_INT ][ i ].UnitSize = sizeof( INT4 ) ;

		Graphics_D3D9_ShaderConstant_UseArea_Initialize( &ConstInfoSet->Info[ DX_SHADERCONSTANTTYPE_VS_BOOL  ][ i ].UseArea,  16 ) ;
		ConstInfoSet->Info[ DX_SHADERCONSTANTTYPE_VS_BOOL ][ i ].UnitSize = sizeof( BOOL ) ;

		Graphics_D3D9_ShaderConstant_UseArea_Initialize( &ConstInfoSet->Info[ DX_SHADERCONSTANTTYPE_PS_FLOAT ][ i ].UseArea, 224 ) ;
		ConstInfoSet->Info[ DX_SHADERCONSTANTTYPE_PS_FLOAT ][ i ].UnitSize = sizeof( FLOAT4 ) ;

		Graphics_D3D9_ShaderConstant_UseArea_Initialize( &ConstInfoSet->Info[ DX_SHADERCONSTANTTYPE_PS_INT   ][ i ].UseArea,  16 ) ;
		ConstInfoSet->Info[ DX_SHADERCONSTANTTYPE_PS_INT ][ i ].UnitSize = sizeof( INT4 ) ;

		Graphics_D3D9_ShaderConstant_UseArea_Initialize( &ConstInfoSet->Info[ DX_SHADERCONSTANTTYPE_PS_BOOL  ][ i ].UseArea,  16 ) ;
		ConstInfoSet->Info[ DX_SHADERCONSTANTTYPE_PS_BOOL ][ i ].UnitSize = sizeof( BOOL ) ;
	}

	// �I��
	return 0 ;
}

// �w��̃V�F�[�_�[�萔�Z�b�g��K�p���邩�ǂ�����ݒ肷��
extern int Graphics_D3D9_ShaderConstant_InfoSet_SetUseState( DIRECT3D9_SHADERCONSTANTINFOSET *ConstInfoSet, int ApplyMask )
{
	BYTE *BeforeMap ;
	BYTE AfterMap[ DX_SHADERCONSTANTTYPE_NUM ][ 256 ] ;
	BYTE ChangeInfo[ DX_SHADERCONSTANTTYPE_NUM ][ 256 ][ 2 ] ;
	int ChangeInfoNum[ DX_SHADERCONSTANTTYPE_NUM ] ;
	DIRECT3D9_SHADERCONSTANTINFO *Info ;
	BYTE *Map ;
	FLOAT4 FloatConst[ 256 ] ;
	INT4 IntConst[ 16 ] ;
	BOOL BoolConst[ 16 ] ;
	int i, j, k, Size ;
	BYTE Start, End ;

	if( GAPIWin.Direct3DDevice9Object == NULL )
	{
		return -1 ;
	}

	// �w��̒萔�Z�b�g�̏�Ԃɕω��������ꍇ�͉������Ȃ�
	if( ConstInfoSet->ApplyMask == ApplyMask )
		return 0 ;

	// �}�X�N��ۑ�
	ConstInfoSet->ApplyMask = ApplyMask ;

	// �}�X�N����t���O�}�b�v���쐬
	for( i = 0 ; i < DX_SHADERCONSTANTSET_NUM ; i ++ )
	{
		ConstInfoSet->IsApply[ i ] = ( ApplyMask & ( 1 << i ) ) != 0 ? TRUE : FALSE ;
	}

	// �ω���̎g�p�}�b�v�̍쐬
	_MEMSET( AfterMap, 0xff, sizeof( AfterMap ) ) ;
	for( i = 0 ; i < DX_SHADERCONSTANTSET_NUM ; i ++ )
	{
		if( ConstInfoSet->IsApply[ i ] )
		{
			for( j = 0 ; j < DX_SHADERCONSTANTTYPE_NUM ; j ++ )
			{
				Graphics_D3D9_ShaderConstant_UseArea_Create( &ConstInfoSet->Info[ j ][ i ].UseArea, AfterMap[ j ], ( BYTE )i ) ;
			}
		}
	}

	// �ω�������ӏ��̏����\�z
	for( i = 0 ; i < DX_SHADERCONSTANTTYPE_NUM ; i ++ )
	{
		BeforeMap = ConstInfoSet->SetMap[ i ] ;
		ChangeInfoNum[ i ] = 0 ;
		Size = ConstInfoSet->Info[ i ][ 0 ].UseArea.TotalSize ;
		j = 0;
		for(;;)
		{
			for( ; j < Size && ( AfterMap[ i ][ j ] == 0xff || BeforeMap[ j ] == AfterMap[ i ][ j ] ) ; j ++ ){}
			if( j == Size ) break ;

			ChangeInfo[ i ][ ChangeInfoNum[ i ] ][ 0 ] = ( BYTE )j ;

			for( ; j < Size && AfterMap[ i ][ j ] != 0xff && BeforeMap[ j ] != AfterMap[ i ][ j ] ; j ++ ){}

			ChangeInfo[ i ][ ChangeInfoNum[ i ] ][ 1 ] = ( BYTE )j ;

			ChangeInfoNum[ i ] ++ ;
		}
	}

	// �ω���̎g�p���^�C�v�}�b�v��ۑ�
	_MEMCPY( ConstInfoSet->SetMap, AfterMap, sizeof( AfterMap ) ) ;


	// �ω��̂���ӏ��̒萔�� Direct3DDevice9 �ɓK�p

	for( j = 0 ; j < ChangeInfoNum[ DX_SHADERCONSTANTTYPE_VS_FLOAT ] ; j ++ )
	{
		Start = ChangeInfo[ DX_SHADERCONSTANTTYPE_VS_FLOAT ][ j ][ 0 ] ;
		End   = ChangeInfo[ DX_SHADERCONSTANTTYPE_VS_FLOAT ][ j ][ 1 ] ;
		Map = AfterMap[ DX_SHADERCONSTANTTYPE_VS_FLOAT ] ;
		Info = ConstInfoSet->Info[ DX_SHADERCONSTANTTYPE_VS_FLOAT ] ;
		for( k = Start ; k < End ; k ++ )
		{
			FloatConst[ k ] = Info[ Map[ k ] ].Float4[ k ] ;
		}
		Direct3DDevice9_SetVertexShaderConstantF( Start, ( float * )&FloatConst[ Start ], ( UINT )( End - Start ) ) ;
		_MEMCPY( &ConstInfoSet->FixInfo[ DX_SHADERCONSTANTTYPE_VS_FLOAT ].Float4[ Start ], ( float * )&FloatConst[ Start ], sizeof( FLOAT4 ) * ( End - Start ) ) ;
	}

	for( j = 0 ; j < ChangeInfoNum[ DX_SHADERCONSTANTTYPE_VS_INT ] ; j ++ )
	{
		Start = ChangeInfo[ DX_SHADERCONSTANTTYPE_VS_INT ][ j ][ 0 ] ;
		End   = ChangeInfo[ DX_SHADERCONSTANTTYPE_VS_INT ][ j ][ 1 ] ;
		Map = AfterMap[ DX_SHADERCONSTANTTYPE_VS_INT ] ;
		Info = ConstInfoSet->Info[ DX_SHADERCONSTANTTYPE_VS_INT ] ;
		for( k = Start ; k < End ; k ++ )
		{
			IntConst[ k ] = Info[ Map[ k ] ].Int4[ k ] ;
		}
		Direct3DDevice9_SetVertexShaderConstantI( Start, ( int * )&IntConst[ Start ], ( UINT )( End - Start ) ) ;
		_MEMCPY( &ConstInfoSet->FixInfo[ DX_SHADERCONSTANTTYPE_VS_INT ].Int4[ Start ], ( int * )&IntConst[ Start ], sizeof( INT4 ) * ( End - Start ) ) ;
	}

	for( j = 0 ; j < ChangeInfoNum[ DX_SHADERCONSTANTTYPE_VS_BOOL ] ; j ++ )
	{
		Start = ChangeInfo[ DX_SHADERCONSTANTTYPE_VS_BOOL ][ j ][ 0 ] ;
		End   = ChangeInfo[ DX_SHADERCONSTANTTYPE_VS_BOOL ][ j ][ 1 ] ;
		Map = AfterMap[ DX_SHADERCONSTANTTYPE_VS_BOOL ] ;
		Info = ConstInfoSet->Info[ DX_SHADERCONSTANTTYPE_VS_BOOL ] ;
		for( k = Start ; k < End ; k ++ )
		{
			BoolConst[ k ] = Info[ Map[ k ] ].Bool[ k ] ;
		}
		Direct3DDevice9_SetVertexShaderConstantB( Start, ( BOOL * )&BoolConst[ Start ], ( UINT )( End - Start ) ) ;
		_MEMCPY( &ConstInfoSet->FixInfo[ DX_SHADERCONSTANTTYPE_VS_BOOL ].Bool[ Start ], ( BOOL * )&BoolConst[ Start ], sizeof( BOOL ) * ( End - Start ) ) ;
	}



	for( j = 0 ; j < ChangeInfoNum[ DX_SHADERCONSTANTTYPE_PS_FLOAT ] ; j ++ )
	{
		Start = ChangeInfo[ DX_SHADERCONSTANTTYPE_PS_FLOAT ][ j ][ 0 ] ;
		End   = ChangeInfo[ DX_SHADERCONSTANTTYPE_PS_FLOAT ][ j ][ 1 ] ;
		Map = AfterMap[ DX_SHADERCONSTANTTYPE_PS_FLOAT ] ;
		Info = ConstInfoSet->Info[ DX_SHADERCONSTANTTYPE_PS_FLOAT ] ;
		for( k = Start ; k < End ; k ++ )
		{
			FloatConst[ k ] = Info[ Map[ k ] ].Float4[ k ] ;
		}
		Direct3DDevice9_SetPixelShaderConstantF( Start, ( float * )&FloatConst[ Start ], ( UINT )( End - Start ) ) ;
		_MEMCPY( &ConstInfoSet->FixInfo[ DX_SHADERCONSTANTTYPE_PS_FLOAT ].Float4[ Start ], ( float * )&FloatConst[ Start ], sizeof( FLOAT4 ) * ( End - Start ) ) ;
	}

	for( j = 0 ; j < ChangeInfoNum[ DX_SHADERCONSTANTTYPE_PS_INT ] ; j ++ )
	{
		Start = ChangeInfo[ DX_SHADERCONSTANTTYPE_PS_INT ][ j ][ 0 ] ;
		End   = ChangeInfo[ DX_SHADERCONSTANTTYPE_PS_INT ][ j ][ 1 ] ;
		Map = AfterMap[ DX_SHADERCONSTANTTYPE_PS_INT ] ;
		Info = ConstInfoSet->Info[ DX_SHADERCONSTANTTYPE_PS_INT ] ;
		for( k = Start ; k < End ; k ++ )
		{
			IntConst[ k ] = Info[ Map[ k ] ].Int4[ k ] ;
		}
		Direct3DDevice9_SetPixelShaderConstantI( Start, ( int * )&IntConst[ Start ], ( UINT )( End - Start ) ) ;
		_MEMCPY( &ConstInfoSet->FixInfo[ DX_SHADERCONSTANTTYPE_PS_INT ].Int4[ Start ], ( int * )&IntConst[ Start ], sizeof( INT4 ) * ( End - Start ) ) ;
	}

	for( j = 0 ; j < ChangeInfoNum[ DX_SHADERCONSTANTTYPE_PS_BOOL ] ; j ++ )
	{
		Start = ChangeInfo[ DX_SHADERCONSTANTTYPE_PS_BOOL ][ j ][ 0 ] ;
		End   = ChangeInfo[ DX_SHADERCONSTANTTYPE_PS_BOOL ][ j ][ 1 ] ;
		Map = AfterMap[ DX_SHADERCONSTANTTYPE_PS_BOOL ] ;
		Info = ConstInfoSet->Info[ DX_SHADERCONSTANTTYPE_PS_BOOL ] ;
		for( k = Start ; k < End ; k ++ )
		{
			BoolConst[ k ] = Info[ Map[ k ] ].Bool[ k ] ;
		}
		Direct3DDevice9_SetPixelShaderConstantB( Start, ( BOOL * )&BoolConst[ Start ], ( UINT )( End - Start ) ) ;
		_MEMCPY( &ConstInfoSet->FixInfo[ DX_SHADERCONSTANTTYPE_PS_BOOL ].Bool[ Start ], ( BOOL * )&BoolConst[ Start ], sizeof( BOOL ) * ( End - Start ) ) ;
	}

	// �I��
	return 0 ;
}

// �V�F�[�_�[�萔�����f�o�C�X�ɐݒ肷��
static int Graphics_D3D9_SetShaderConstantDeviceSet( DIRECT3D9_SHADERCONSTANTINFOSET *ConstInfoSet, int TypeIndex, int ConstantIndex, const void *Param, int ParamNum )
{
	union
	{
		FLOAT4 TempArrayF[ 256 ] ;
		INT4 TempArrayI[ 16 ] ;
		BOOL TempArrayB[ 16 ] ;
	} ;

	if( GAPIWin.Direct3DDevice9Object == NULL )
	{
		return -1 ;
	}

	// �f�[�^�������ꍇ�̓}�b�v����Q�Ƃ���
	if( Param == NULL )
	{
		DIRECT3D9_SHADERCONSTANTINFO *Info ;
		BYTE *Map ;
		int Start, End ;
		int i ;

		Start = ConstantIndex ;
		End = Start + ParamNum ;
		Map = ConstInfoSet->SetMap[ TypeIndex ] ;

		Info = ConstInfoSet->Info[ TypeIndex ] ;
		switch( TypeIndex )
		{
		case DX_SHADERCONSTANTTYPE_VS_FLOAT :
		case DX_SHADERCONSTANTTYPE_PS_FLOAT :
			_MEMSET( TempArrayF, 0, sizeof( TempArrayF ) ) ;
			for( i = Start ; i < End ; i ++ )
			{
				if( Map[ i ] != 0xff )
				{
					TempArrayF[ i ] = Info[ Map[ i ] ].Float4[ i ] ;
				}
			}
			Param = &TempArrayF[ Start ] ;
			break ;

		case DX_SHADERCONSTANTTYPE_VS_INT :
		case DX_SHADERCONSTANTTYPE_PS_INT :
			_MEMSET( TempArrayI, 0, sizeof( TempArrayI ) ) ;
			for( i = Start ; i < End ; i ++ )
			{
				if( Map[ i ] != 0xff )
				{
					TempArrayI[ i ] = Info[ Map[ i ] ].Int4[ i ] ;
				}
			}
			Param = &TempArrayI[ Start ] ;
			break ;

		case DX_SHADERCONSTANTTYPE_VS_BOOL :
		case DX_SHADERCONSTANTTYPE_PS_BOOL :
			_MEMSET( TempArrayB, 0, sizeof( TempArrayB ) ) ;
			for( i = Start ; i < End ; i ++ )
			{
				if( Map[ i ] != 0xff )
				{
					TempArrayB[ i ] = Info[ Map[ i ] ].Bool[ i ] ;
				}
			}
			Param = &TempArrayB[ Start ] ;
			break ;
		}
	}

	// �f�o�C�X�ɃZ�b�g�ƃp�����[�^��ۑ�
	switch( TypeIndex )
	{
	case DX_SHADERCONSTANTTYPE_VS_FLOAT :
		if( GSYS.HardInfo.UseShader == TRUE ) 
		{
			Direct3DDevice9_SetVertexShaderConstantF( ( UINT )ConstantIndex, ( float * )Param, ( UINT )ParamNum ) ;
		}
		_MEMCPY( &ConstInfoSet->FixInfo[ DX_SHADERCONSTANTTYPE_VS_FLOAT ].Float4[ ConstantIndex ], Param, sizeof( FLOAT4 ) * ParamNum ) ;
		break ;

	case DX_SHADERCONSTANTTYPE_VS_INT :
		if( GSYS.HardInfo.UseShader == TRUE ) 
		{
			Direct3DDevice9_SetVertexShaderConstantI( ( UINT )ConstantIndex, ( int * )Param, ( UINT )ParamNum ) ;
		}
		_MEMCPY( &ConstInfoSet->FixInfo[ DX_SHADERCONSTANTTYPE_VS_INT ].Int4[ ConstantIndex ], Param, sizeof( INT4 ) * ParamNum ) ;
		break ;

	case DX_SHADERCONSTANTTYPE_VS_BOOL :
		if( GSYS.HardInfo.UseShader == TRUE ) 
		{
			Direct3DDevice9_SetVertexShaderConstantB( ( UINT )ConstantIndex, ( BOOL * )Param, ( UINT )ParamNum ) ;
		}
		_MEMCPY( &ConstInfoSet->FixInfo[ DX_SHADERCONSTANTTYPE_VS_BOOL ].Bool[ ConstantIndex ], Param, sizeof( BOOL ) * ParamNum ) ;
		break ;

	case DX_SHADERCONSTANTTYPE_PS_FLOAT :
		if( GSYS.HardInfo.UseShader == TRUE ) 
		{
			Direct3DDevice9_SetPixelShaderConstantF( ( UINT )ConstantIndex, ( float * )Param, ( UINT )ParamNum ) ;
		}
		_MEMCPY( &ConstInfoSet->FixInfo[ DX_SHADERCONSTANTTYPE_PS_FLOAT ].Float4[ ConstantIndex ], Param, sizeof( FLOAT4 ) * ParamNum ) ;
		break ;

	case DX_SHADERCONSTANTTYPE_PS_INT :
		if( GSYS.HardInfo.UseShader == TRUE ) 
		{
			Direct3DDevice9_SetPixelShaderConstantI( ( UINT )ConstantIndex, ( int * )Param, ( UINT )ParamNum ) ;
		}
		_MEMCPY( &ConstInfoSet->FixInfo[ DX_SHADERCONSTANTTYPE_PS_INT ].Int4[ ConstantIndex ], Param, sizeof( INT4 ) * ParamNum ) ;
		break ;

	case DX_SHADERCONSTANTTYPE_PS_BOOL :
		if( GSYS.HardInfo.UseShader == TRUE ) 
		{
			Direct3DDevice9_SetPixelShaderConstantB( ( UINT )ConstantIndex, ( BOOL * )Param, ( UINT )ParamNum ) ;
		}
		_MEMCPY( &ConstInfoSet->FixInfo[ DX_SHADERCONSTANTTYPE_PS_BOOL ].Bool[ ConstantIndex ], Param, sizeof( BOOL ) * ParamNum ) ;
		break ;
	}

	// �I��
	return 0 ;
}

// �V�F�[�_�[�萔����ݒ肷��
extern int Graphics_D3D9_ShaderConstant_InfoSet_SetParam( DIRECT3D9_SHADERCONSTANTINFOSET *ConstInfoSet, int TypeIndex, int SetIndex, int ConstantIndex, const void *Param, int ParamNum, int UpdateUseArea )
{
	int Start, End ;
	int i ;
	bool bMaskHit ;
	BYTE *Map ;
	BYTE *UseMap ;
	DIRECT3D9_SHADERCONSTANTINFO *Info ;

	Info = &ConstInfoSet->Info[ TypeIndex ][ SetIndex ] ;

	// �P�̂̎g�p�͈͂̍X�V
	if( UpdateUseArea )
	{
		Graphics_D3D9_ShaderConstant_UseArea_Set( &Info->UseArea, TRUE, ConstantIndex, ParamNum ) ;
	}

	// �z��Ƀf�[�^���Z�b�g
	_MEMCPY( &Info->Data[ ConstantIndex * Info->UnitSize ], Param, ( size_t )( ParamNum * Info->UnitSize ) ) ;

	// �͈͂̃Z�b�g
	Start = ConstantIndex ;
	End = Start + ParamNum ;

	// �S�̂̎g�p�̈�̍X�V
	bMaskHit = false ;
	Map = ConstInfoSet->SetMap[ TypeIndex ] ;
	UseMap = ConstInfoSet->UseMap[ TypeIndex ][ SetIndex ] ;
	for( i = Start; i < End; i++ )
	{
		UseMap[ i ] = TRUE ;

		if( ConstInfoSet->IsApply[ SetIndex ] )
		{
			if( Map[ i ] == 0xff || Map[ i ] <= SetIndex )
			{
				Map[ i ] = ( BYTE )SetIndex ;
			}
			else
			{
				bMaskHit = true ;
			}
		}
	}

	// �f�o�C�X�ɃZ�b�g
	if( ConstInfoSet->IsApply[ SetIndex ] )
	{
		Graphics_D3D9_SetShaderConstantDeviceSet( ConstInfoSet, TypeIndex, ConstantIndex, bMaskHit == false ? Param : NULL, ParamNum ) ;
	}

	// �I��
	return 0 ;
}

// �w��̈�̃V�F�[�_�[�萔�������Z�b�g����
extern int Graphics_D3D9_ShaderConstant_InfoSet_ResetParam( DIRECT3D9_SHADERCONSTANTINFOSET *ConstInfoSet, int TypeIndex, int SetIndex, int ConstantIndex, int ParamNum )
{
	int Start, End ;
	int i, j ;
	bool bMaskHit ;
	BYTE *Map ;
	BYTE ( *UseMap )[ 256 ] ;
	DIRECT3D9_SHADERCONSTANTINFO *Info ;

	// �g�p�͈͂̍X�V
	Info = &ConstInfoSet->Info[ TypeIndex ][ SetIndex ] ;
	Graphics_D3D9_ShaderConstant_UseArea_Set( &Info->UseArea, FALSE, ConstantIndex, ParamNum ) ;

	// �͈͂̃Z�b�g
	Start = ConstantIndex ;
	End = Start + ParamNum ;

	// �g�p�̈�̍X�V
	Map = ConstInfoSet->SetMap[ TypeIndex ] ;
	UseMap = ConstInfoSet->UseMap[ TypeIndex ] ;
	bMaskHit = false ;
	for( i = Start; i < End; i++ )
	{
		UseMap[ SetIndex ][ i ] = FALSE ;

		if( Map[ i ] == SetIndex )
		{
			for( j = SetIndex; j >= 0 && UseMap[ j ][ i ] == FALSE; j-- ){}
			if( j == -1 )
			{
				Map[ i ] = 0xff ;
			}
			else
			{
				Map[ i ] = ( BYTE )j ;
				bMaskHit = true ;
			}
		}
	}

	// �g�p�̈悪�ω����邱�Ƃō��܂Ŗ����������萔���L���ɂȂ�ꍇ�͒萔���̍X�V
	if( bMaskHit )
	{
		Graphics_D3D9_SetShaderConstantDeviceSet( ConstInfoSet, TypeIndex, ConstantIndex, NULL, ParamNum ) ;
	}

	// �I��
	return 0 ;
}














// ���_�V�F�[�_�錾�֌W�֐�

static  D_D3DVERTEXELEMENT9	MakeVertexElement9( BYTE Stream, BYTE Offset, BYTE Type, BYTE Method, BYTE Usage, BYTE UsageIndex )
{
	D_D3DVERTEXELEMENT9 Result = { Stream, Offset, Type, Method, Usage, UsageIndex } ;
	return Result ;
}

// Direct3DVertexDeclaration9 �I�u�W�F�N�g�̍쐬( 0:����  -1:���s )
extern	int		Graphics_D3D9_VertexDeclaration_Create( void )
{
	// ���ɍ쐬����Ă����Ƃ��̂��߂ɍ폜�������s��
	Graphics_D3D9_VertexDeclaration_Terminate() ;

#ifndef DX_NON_MODEL
	{
		D_D3DVERTEXELEMENT9 *VE, VertElem[ 64 ] = 
		{
			{ 0, 0,   D_D3DDECLTYPE_FLOAT3,   D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_POSITION,     0 },
			{ 0, 12,  D_D3DDECLTYPE_FLOAT4,   D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_BLENDWEIGHT,  0 },
			{ 0, 28,  D_D3DDECLTYPE_UBYTE4,   D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_BLENDINDICES, 0 },
			{ 0, 32,  D_D3DDECLTYPE_FLOAT3,   D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_NORMAL,       0 },
			{ 0, 44,  D_D3DDECLTYPE_D3DCOLOR, D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_COLOR,        0 },
			{ 0, 48,  D_D3DDECLTYPE_D3DCOLOR, D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_COLOR,        1 },
			{ 0, 52,  D_D3DDECLTYPE_FLOAT4,   D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_TEXCOORD,     0 },
			D_D3DDECL_END()
		} ;
		int offset, index ;
		int i, j, k, l ;

		// �o���v�}�b�v�������ǂ����łQ���[�v
		for( i = 0 ; i < 2 ; i ++ )
		{
			// �X�L�j���O���b�V���^�C�v�ɂ���ĂR���[�v
			for( j = 0 ; j < 3 ; j ++ )
			{
				if( i == 0 )
				{
					switch( j )
					{
					case 0 :
						// �o���v�}�b�v��񖳂����̃��b�V��
						VertElem[ 0 ] = MakeVertexElement9( 0,  0,  D_D3DDECLTYPE_FLOAT3,   D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_POSITION,     0 ) ;
						VertElem[ 1 ] = MakeVertexElement9( 0, 12,  D_D3DDECLTYPE_FLOAT3,   D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_NORMAL,       0 ) ;
						VertElem[ 2 ] = MakeVertexElement9( 0, 24,  D_D3DDECLTYPE_D3DCOLOR, D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_COLOR,        0 ) ;
						VertElem[ 3 ] = MakeVertexElement9( 0, 28,  D_D3DDECLTYPE_D3DCOLOR, D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_COLOR,        1 ) ;
						offset = 32 ;
						index = 4 ;
						break ;

					case 1 :
						// �o���v�}�b�v��񖳂��S�{�[�����X�L�j���O���b�V��
						VertElem[ 0 ] = MakeVertexElement9( 0,  0,  D_D3DDECLTYPE_FLOAT3,   D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_POSITION,     0 ) ;
						VertElem[ 1 ] = MakeVertexElement9( 0, 12,  D_D3DDECLTYPE_UBYTE4,   D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_BLENDINDICES, 0 ) ;
						VertElem[ 2 ] = MakeVertexElement9( 0, 16,  D_D3DDECLTYPE_FLOAT4,   D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_BLENDWEIGHT,  0 ) ;
						VertElem[ 3 ] = MakeVertexElement9( 0, 32,  D_D3DDECLTYPE_FLOAT3,   D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_NORMAL,       0 ) ;
						VertElem[ 4 ] = MakeVertexElement9( 0, 44,  D_D3DDECLTYPE_D3DCOLOR, D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_COLOR,        0 ) ;
						VertElem[ 5 ] = MakeVertexElement9( 0, 48,  D_D3DDECLTYPE_D3DCOLOR, D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_COLOR,        1 ) ;
						offset = 52 ;
						index = 6 ;
						break ;

					case 2 :
						// �o���v�}�b�v��񖳂��W�{�[�����X�L�j���O���b�V��
						VertElem[ 0 ] = MakeVertexElement9( 0,  0,  D_D3DDECLTYPE_FLOAT3,   D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_POSITION,     0 ) ;
						VertElem[ 1 ] = MakeVertexElement9( 0, 12,  D_D3DDECLTYPE_UBYTE4,   D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_BLENDINDICES, 0 ) ;
						VertElem[ 2 ] = MakeVertexElement9( 0, 16,  D_D3DDECLTYPE_UBYTE4,   D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_BLENDINDICES, 1 ) ;
						VertElem[ 3 ] = MakeVertexElement9( 0, 20,  D_D3DDECLTYPE_FLOAT4,   D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_BLENDWEIGHT,  0 ) ;
						VertElem[ 4 ] = MakeVertexElement9( 0, 36,  D_D3DDECLTYPE_FLOAT4,   D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_BLENDWEIGHT,  1 ) ;
						VertElem[ 5 ] = MakeVertexElement9( 0, 52,  D_D3DDECLTYPE_FLOAT3,   D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_NORMAL,       0 ) ;
						VertElem[ 6 ] = MakeVertexElement9( 0, 64,  D_D3DDECLTYPE_D3DCOLOR, D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_COLOR,        0 ) ;
						VertElem[ 7 ] = MakeVertexElement9( 0, 68,  D_D3DDECLTYPE_D3DCOLOR, D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_COLOR,        1 ) ;
						offset = 72 ;
						index = 8 ;
						break ;
					}
				}
				else
				{
					switch( j )
					{
					case 0 :
						// �o���v�}�b�v���L�荄�̃��b�V��
						VertElem[ 0 ] = MakeVertexElement9( 0,  0,  D_D3DDECLTYPE_FLOAT3,   D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_POSITION,     0 ) ;
						VertElem[ 1 ] = MakeVertexElement9( 0, 12,  D_D3DDECLTYPE_FLOAT3,   D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_TANGENT,      0 ) ;
						VertElem[ 2 ] = MakeVertexElement9( 0, 24,  D_D3DDECLTYPE_FLOAT3,   D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_BINORMAL,     0 ) ;
						VertElem[ 3 ] = MakeVertexElement9( 0, 36,  D_D3DDECLTYPE_FLOAT3,   D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_NORMAL,       0 ) ;
						VertElem[ 4 ] = MakeVertexElement9( 0, 48,  D_D3DDECLTYPE_D3DCOLOR, D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_COLOR,        0 ) ;
						VertElem[ 5 ] = MakeVertexElement9( 0, 52,  D_D3DDECLTYPE_D3DCOLOR, D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_COLOR,        1 ) ;
						offset = 56 ;
						index = 6 ;
						break ;

					case 1 :
						// �o���v�}�b�v���L��S�{�[�����X�L�j���O���b�V��
						VertElem[ 0 ] = MakeVertexElement9( 0,  0,  D_D3DDECLTYPE_FLOAT3,   D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_POSITION,     0 ) ;
						VertElem[ 1 ] = MakeVertexElement9( 0, 12,  D_D3DDECLTYPE_UBYTE4,   D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_BLENDINDICES, 0 ) ;
						VertElem[ 2 ] = MakeVertexElement9( 0, 16,  D_D3DDECLTYPE_FLOAT4,   D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_BLENDWEIGHT,  0 ) ;
						VertElem[ 3 ] = MakeVertexElement9( 0, 32,  D_D3DDECLTYPE_FLOAT3,   D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_TANGENT,      0 ) ;
						VertElem[ 4 ] = MakeVertexElement9( 0, 44,  D_D3DDECLTYPE_FLOAT3,   D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_BINORMAL,     0 ) ;
						VertElem[ 5 ] = MakeVertexElement9( 0, 56,  D_D3DDECLTYPE_FLOAT3,   D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_NORMAL,       0 ) ;
						VertElem[ 6 ] = MakeVertexElement9( 0, 68,  D_D3DDECLTYPE_D3DCOLOR, D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_COLOR,        0 ) ;
						VertElem[ 7 ] = MakeVertexElement9( 0, 72,  D_D3DDECLTYPE_D3DCOLOR, D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_COLOR,        1 ) ;
						offset = 76 ;
						index = 8 ;
						break ;

					case 2 :
						// �o���v�}�b�v���L��W�{�[�����X�L�j���O���b�V��
						VertElem[ 0 ] = MakeVertexElement9( 0,  0,  D_D3DDECLTYPE_FLOAT3,   D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_POSITION,     0 ) ;
						VertElem[ 1 ] = MakeVertexElement9( 0, 12,  D_D3DDECLTYPE_UBYTE4,   D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_BLENDINDICES, 0 ) ;
						VertElem[ 2 ] = MakeVertexElement9( 0, 16,  D_D3DDECLTYPE_UBYTE4,   D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_BLENDINDICES, 1 ) ;
						VertElem[ 3 ] = MakeVertexElement9( 0, 20,  D_D3DDECLTYPE_FLOAT4,   D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_BLENDWEIGHT,  0 ) ;
						VertElem[ 4 ] = MakeVertexElement9( 0, 36,  D_D3DDECLTYPE_FLOAT4,   D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_BLENDWEIGHT,  1 ) ;
						VertElem[ 5 ] = MakeVertexElement9( 0, 52,  D_D3DDECLTYPE_FLOAT3,   D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_TANGENT,      0 ) ;
						VertElem[ 6 ] = MakeVertexElement9( 0, 64,  D_D3DDECLTYPE_FLOAT3,   D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_BINORMAL,     0 ) ;
						VertElem[ 7 ] = MakeVertexElement9( 0, 76,  D_D3DDECLTYPE_FLOAT3,   D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_NORMAL,       0 ) ;
						VertElem[ 8 ] = MakeVertexElement9( 0, 88,  D_D3DDECLTYPE_D3DCOLOR, D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_COLOR,        0 ) ;
						VertElem[ 9 ] = MakeVertexElement9( 0, 92,  D_D3DDECLTYPE_D3DCOLOR, D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_COLOR,        1 ) ;
						offset = 96 ;
						index = 10 ;
						break ;
					}
				}

				for( k = 0 ; k < 9 ; k ++ )
				{
					VE = &VertElem[ index ] ;
					for( l = 0 ; l < k ; l ++, VE ++ )
					{
						VE->Stream     = 0 ;
						VE->Offset     = ( WORD )( offset + sizeof( float ) * 4 * l ) ;
						VE->Type       = D_D3DDECLTYPE_FLOAT4 ;
						VE->Method     = D_D3DDECLMETHOD_DEFAULT ;
						VE->Usage      = D_D3DDECLUSAGE_TEXCOORD ;
						VE->UsageIndex = ( BYTE )l ;
					}
					VE->Stream     = 0xFF ;
					VE->Offset     = 0 ;
					VE->Type       = D_D3DDECLTYPE_UNUSED ;
					VE->Method     = 0 ;
					VE->Usage      = 0 ;
					VE->UsageIndex = 0 ;

					Direct3DDevice9_CreateVertexDeclaration( VertElem, &GD3D9.Device.VertexDeclaration.MV1_VertexDeclaration[ i ][ j ][ k ] ) ;
				}
			}
		}
	}
#endif // DX_NON_MODEL

	// �Œ�p�C�v���C���݊��V�F�[�_�[�Ŏg�p���钸�_�f�[�^�`�����쐬
	{
		D_D3DVERTEXELEMENT9 VertElem_[ 11 ] ;

		// VERTEXFVF_2D_USER ( VERTEX2D�\���̌`�� )
		VertElem_[ 0 ] = MakeVertexElement9( 0,     0,  D_D3DDECLTYPE_FLOAT4,   D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_POSITION,     0 ) ;
		VertElem_[ 1 ] = MakeVertexElement9( 0,    16,  D_D3DDECLTYPE_D3DCOLOR, D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_COLOR,        0 ) ;
		VertElem_[ 2 ] = MakeVertexElement9( 0,    20,  D_D3DDECLTYPE_FLOAT2,   D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_TEXCOORD,     0 ) ;
		VertElem_[ 3 ] = MakeVertexElement9( 0xff,  0,  D_D3DDECLTYPE_UNUSED,                         0,                       0,     0 ) ;
		Direct3DDevice9_CreateVertexDeclaration( VertElem_, &GD3D9.Device.VertexDeclaration.Base2DDeclaration[ VERTEXFVF_DECL_2D_USER ] ) ;

		// VERTEXFVF_NOTEX_2D ( VERTEX_NOTEX_2D�\���̌`�� )
		VertElem_[ 0 ] = MakeVertexElement9( 0,     0,  D_D3DDECLTYPE_FLOAT4,   D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_POSITION,     0 ) ;
		VertElem_[ 1 ] = MakeVertexElement9( 0,    16,  D_D3DDECLTYPE_D3DCOLOR, D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_COLOR,        0 ) ;
		VertElem_[ 2 ] = MakeVertexElement9( 0xff,  0,  D_D3DDECLTYPE_UNUSED,                         0,                       0,     0 ) ;
		Direct3DDevice9_CreateVertexDeclaration( VertElem_, &GD3D9.Device.VertexDeclaration.Base2DDeclaration[ VERTEXFVF_DECL_NOTEX_2D ] ) ;

		// VERTEXFVF_2D ( VERTEX_2D�\���̌`�� )
		VertElem_[ 0 ] = MakeVertexElement9( 0,     0,  D_D3DDECLTYPE_FLOAT4,   D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_POSITION,     0 ) ;
		VertElem_[ 1 ] = MakeVertexElement9( 0,    16,  D_D3DDECLTYPE_D3DCOLOR, D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_COLOR,        0 ) ;
		VertElem_[ 2 ] = MakeVertexElement9( 0,    20,  D_D3DDECLTYPE_FLOAT2,   D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_TEXCOORD,     0 ) ;
		VertElem_[ 3 ] = MakeVertexElement9( 0xff,  0,  D_D3DDECLTYPE_UNUSED,                         0,                       0,     0 ) ;
		Direct3DDevice9_CreateVertexDeclaration( VertElem_, &GD3D9.Device.VertexDeclaration.Base2DDeclaration[ VERTEXFVF_DECL_2D ] ) ;

		// VERTEXFVF_BLENDTEX_2D ( VERTEX_BLENDTEX_2D�\���̌`�� )
		VertElem_[ 0 ] = MakeVertexElement9( 0,     0,  D_D3DDECLTYPE_FLOAT4,   D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_POSITION,     0 ) ;
		VertElem_[ 1 ] = MakeVertexElement9( 0,    16,  D_D3DDECLTYPE_D3DCOLOR, D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_COLOR,        0 ) ;
		VertElem_[ 2 ] = MakeVertexElement9( 0,    20,  D_D3DDECLTYPE_D3DCOLOR, D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_COLOR,        1 ) ;
		VertElem_[ 3 ] = MakeVertexElement9( 0,    24,  D_D3DDECLTYPE_FLOAT2,   D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_TEXCOORD,     0 ) ;
		VertElem_[ 4 ] = MakeVertexElement9( 0,    32,  D_D3DDECLTYPE_FLOAT2,   D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_TEXCOORD,     1 ) ;
		VertElem_[ 5 ] = MakeVertexElement9( 0xff,  0,  D_D3DDECLTYPE_UNUSED,                         0,                       0,     0 ) ;
		Direct3DDevice9_CreateVertexDeclaration( VertElem_, &GD3D9.Device.VertexDeclaration.Base2DDeclaration[ VERTEXFVF_DECL_BLENDTEX_2D ] ) ;

		// VERTEXFVF_SHADER_2D ( VERTEX2DSHADER�\���̌`�� )
		VertElem_[ 0 ] = MakeVertexElement9( 0,     0,  D_D3DDECLTYPE_FLOAT4,   D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_POSITION,     0 ) ;
		VertElem_[ 1 ] = MakeVertexElement9( 0,    16,  D_D3DDECLTYPE_D3DCOLOR, D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_COLOR,        0 ) ;
		VertElem_[ 2 ] = MakeVertexElement9( 0,    20,  D_D3DDECLTYPE_D3DCOLOR, D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_COLOR,        1 ) ;
		VertElem_[ 3 ] = MakeVertexElement9( 0,    24,  D_D3DDECLTYPE_FLOAT2,   D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_TEXCOORD,     0 ) ;
		VertElem_[ 4 ] = MakeVertexElement9( 0,    32,  D_D3DDECLTYPE_FLOAT2,   D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_TEXCOORD,     1 ) ;
		VertElem_[ 5 ] = MakeVertexElement9( 0xff,  0,  D_D3DDECLTYPE_UNUSED,                         0,                       0,     0 ) ;
		Direct3DDevice9_CreateVertexDeclaration( VertElem_, &GD3D9.Device.VertexDeclaration.Base2DDeclaration[ VERTEXFVF_DECL_SHADER_2D ] ) ;

		// VERTEXFVF_NOTEX_3D ( VERTEX_NOTEX_3D�\���̌`�� )
		VertElem_[ 0 ] = MakeVertexElement9( 0,     0,  D_D3DDECLTYPE_FLOAT3,   D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_POSITION,     0 ) ;
		VertElem_[ 1 ] = MakeVertexElement9( 0,    12,  D_D3DDECLTYPE_D3DCOLOR, D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_COLOR,        0 ) ;
		VertElem_[ 2 ] = MakeVertexElement9( 0xff,  0,  D_D3DDECLTYPE_UNUSED,                         0,                       0,     0 ) ;
		Direct3DDevice9_CreateVertexDeclaration( VertElem_, &GD3D9.Device.VertexDeclaration.Base2DDeclaration[ VERTEXFVF_DECL_NOTEX_3D ] ) ;

		// VERTEXFVF_3D ( VERTEX_3D�\���̌`�� )
		VertElem_[ 0 ] = MakeVertexElement9( 0,     0,  D_D3DDECLTYPE_FLOAT3,   D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_POSITION,     0 ) ;
		VertElem_[ 1 ] = MakeVertexElement9( 0,    12,  D_D3DDECLTYPE_D3DCOLOR, D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_COLOR,        0 ) ;
		VertElem_[ 2 ] = MakeVertexElement9( 0,    16,  D_D3DDECLTYPE_FLOAT2,   D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_TEXCOORD,     0 ) ;
		VertElem_[ 3 ] = MakeVertexElement9( 0xff,  0,  D_D3DDECLTYPE_UNUSED,                         0,                       0,     0 ) ;
		Direct3DDevice9_CreateVertexDeclaration( VertElem_, &GD3D9.Device.VertexDeclaration.Base2DDeclaration[ VERTEXFVF_DECL_3D ] ) ;

		// VERTEXFVF_3D_LIGHT ( VERTEX3D�\���̌`�� )
		VertElem_[ 0 ] = MakeVertexElement9( 0,     0,  D_D3DDECLTYPE_FLOAT3,   D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_POSITION,     0 ) ;
		VertElem_[ 1 ] = MakeVertexElement9( 0,    12,  D_D3DDECLTYPE_FLOAT3,   D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_NORMAL,       0 ) ;
		VertElem_[ 2 ] = MakeVertexElement9( 0,    24,  D_D3DDECLTYPE_D3DCOLOR, D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_COLOR,        0 ) ;
		VertElem_[ 3 ] = MakeVertexElement9( 0,    28,  D_D3DDECLTYPE_D3DCOLOR, D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_COLOR,        1 ) ;
		VertElem_[ 4 ] = MakeVertexElement9( 0,    32,  D_D3DDECLTYPE_FLOAT2,   D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_TEXCOORD,     0 ) ;
		VertElem_[ 5 ] = MakeVertexElement9( 0,    40,  D_D3DDECLTYPE_FLOAT2,   D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_TEXCOORD,     1 ) ;
		VertElem_[ 6 ] = MakeVertexElement9( 0xff,  0,  D_D3DDECLTYPE_UNUSED,                         0,                       0,     0 ) ;
		Direct3DDevice9_CreateVertexDeclaration( VertElem_, &GD3D9.Device.VertexDeclaration.Base2DDeclaration[ VERTEXFVF_DECL_3D_LIGHT ] ) ;

		// VERTEXFVF_SHADER_3D ( VERTEX3DSHADER�\���̌`�� )
		VertElem_[ 0 ] = MakeVertexElement9( 0,     0,  D_D3DDECLTYPE_FLOAT3,   D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_POSITION,     0 ) ;
		VertElem_[ 1 ] = MakeVertexElement9( 0,    12,  D_D3DDECLTYPE_FLOAT4,   D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_POSITION,     1 ) ;
		VertElem_[ 2 ] = MakeVertexElement9( 0,    28,  D_D3DDECLTYPE_FLOAT3,   D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_NORMAL,       0 ) ;
		VertElem_[ 3 ] = MakeVertexElement9( 0,    40,  D_D3DDECLTYPE_FLOAT3,   D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_TANGENT,      0 ) ;
		VertElem_[ 4 ] = MakeVertexElement9( 0,    52,  D_D3DDECLTYPE_FLOAT3,   D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_BINORMAL,     0 ) ;
		VertElem_[ 5 ] = MakeVertexElement9( 0,    64,  D_D3DDECLTYPE_D3DCOLOR, D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_COLOR,        0 ) ;
		VertElem_[ 6 ] = MakeVertexElement9( 0,    68,  D_D3DDECLTYPE_D3DCOLOR, D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_COLOR,        1 ) ;
		VertElem_[ 7 ] = MakeVertexElement9( 0,    72,  D_D3DDECLTYPE_FLOAT2,   D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_TEXCOORD,     0 ) ;
		VertElem_[ 8 ] = MakeVertexElement9( 0,    80,  D_D3DDECLTYPE_FLOAT2,   D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_TEXCOORD,     1 ) ;
		VertElem_[ 9 ] = MakeVertexElement9( 0xff,  0,  D_D3DDECLTYPE_UNUSED,                         0,                       0,     0 ) ;
		Direct3DDevice9_CreateVertexDeclaration( VertElem_, &GD3D9.Device.VertexDeclaration.Base2DDeclaration[ VERTEXFVF_DECL_SHADER_3D ] ) ;

		// VERTEXFVF_DECL_TEX8_2D ( VERTEX_TEX8_2D�\���̌`�� )
		VertElem_[  0 ] = MakeVertexElement9( 0,     0,  D_D3DDECLTYPE_FLOAT4,   D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_POSITION,     0 ) ;
		VertElem_[  1 ] = MakeVertexElement9( 0,    16,  D_D3DDECLTYPE_D3DCOLOR, D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_COLOR,        0 ) ;
		VertElem_[  2 ] = MakeVertexElement9( 0,    20,  D_D3DDECLTYPE_FLOAT2,   D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_TEXCOORD,     0 ) ;
		VertElem_[  3 ] = MakeVertexElement9( 0,    28,  D_D3DDECLTYPE_FLOAT2,   D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_TEXCOORD,     1 ) ;
		VertElem_[  4 ] = MakeVertexElement9( 0,    36,  D_D3DDECLTYPE_FLOAT2,   D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_TEXCOORD,     2 ) ;
		VertElem_[  5 ] = MakeVertexElement9( 0,    44,  D_D3DDECLTYPE_FLOAT2,   D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_TEXCOORD,     3 ) ;
		VertElem_[  6 ] = MakeVertexElement9( 0,    52,  D_D3DDECLTYPE_FLOAT2,   D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_TEXCOORD,     4 ) ;
		VertElem_[  7 ] = MakeVertexElement9( 0,    60,  D_D3DDECLTYPE_FLOAT2,   D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_TEXCOORD,     5 ) ;
		VertElem_[  8 ] = MakeVertexElement9( 0,    68,  D_D3DDECLTYPE_FLOAT2,   D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_TEXCOORD,     6 ) ;
		VertElem_[  9 ] = MakeVertexElement9( 0,    76,  D_D3DDECLTYPE_FLOAT2,   D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_TEXCOORD,     7 ) ;
		VertElem_[ 10 ] = MakeVertexElement9( 0xff,  0,  D_D3DDECLTYPE_UNUSED,                         0,                       0,     0 ) ;
		Direct3DDevice9_CreateVertexDeclaration( VertElem_, &GD3D9.Device.VertexDeclaration.Base2DDeclaration[ VERTEXFVF_DECL_TEX8_2D ] ) ;
	}

	// ���[�U�[�V�F�[�_�[���[�h�Ŏg�p���钸�_�f�[�^�`�����쐬
	{
		D_D3DVERTEXELEMENT9 _VertElem[ 10 ] ;

		// DX_VERTEX_TYPE_NORMAL_3D ( VERTEX3D�\���̌`�� )
		_VertElem[ 0 ] = MakeVertexElement9( 0,     0,  D_D3DDECLTYPE_FLOAT3,   D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_POSITION,     0 ) ;
		_VertElem[ 1 ] = MakeVertexElement9( 0,    12,  D_D3DDECLTYPE_FLOAT3,   D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_NORMAL,       0 ) ;
		_VertElem[ 2 ] = MakeVertexElement9( 0,    24,  D_D3DDECLTYPE_D3DCOLOR, D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_COLOR,        0 ) ;
		_VertElem[ 3 ] = MakeVertexElement9( 0,    28,  D_D3DDECLTYPE_D3DCOLOR, D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_COLOR,        1 ) ;
		_VertElem[ 4 ] = MakeVertexElement9( 0,    32,  D_D3DDECLTYPE_FLOAT2,   D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_TEXCOORD,     0 ) ;
		_VertElem[ 5 ] = MakeVertexElement9( 0,    40,  D_D3DDECLTYPE_FLOAT2,   D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_TEXCOORD,     1 ) ;
		_VertElem[ 6 ] = MakeVertexElement9( 0xff,  0,  D_D3DDECLTYPE_UNUSED,                         0,                       0,     0 ) ;
		Direct3DDevice9_CreateVertexDeclaration( _VertElem, &GD3D9.Device.VertexDeclaration.UserShaderDeclaration[ DX_VERTEX_TYPE_NORMAL_3D ] ) ;

		// DX_VERTEX_TYPE_SHADER_3D ( VERTEX3DSHADER�\���̌`�� )
		_VertElem[ 0 ] = MakeVertexElement9( 0,     0,  D_D3DDECLTYPE_FLOAT3,   D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_POSITION,     0 ) ;
		_VertElem[ 1 ] = MakeVertexElement9( 0,    12,  D_D3DDECLTYPE_FLOAT4,   D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_POSITION,     1 ) ;
		_VertElem[ 2 ] = MakeVertexElement9( 0,    28,  D_D3DDECLTYPE_FLOAT3,   D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_NORMAL,       0 ) ;
		_VertElem[ 3 ] = MakeVertexElement9( 0,    40,  D_D3DDECLTYPE_FLOAT3,   D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_TANGENT,      0 ) ;
		_VertElem[ 4 ] = MakeVertexElement9( 0,    52,  D_D3DDECLTYPE_FLOAT3,   D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_BINORMAL,     0 ) ;
		_VertElem[ 5 ] = MakeVertexElement9( 0,    64,  D_D3DDECLTYPE_D3DCOLOR, D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_COLOR,        0 ) ;
		_VertElem[ 6 ] = MakeVertexElement9( 0,    68,  D_D3DDECLTYPE_D3DCOLOR, D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_COLOR,        1 ) ;
		_VertElem[ 7 ] = MakeVertexElement9( 0,    72,  D_D3DDECLTYPE_FLOAT2,   D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_TEXCOORD,     0 ) ;
		_VertElem[ 8 ] = MakeVertexElement9( 0,    80,  D_D3DDECLTYPE_FLOAT2,   D_D3DDECLMETHOD_DEFAULT, D_D3DDECLUSAGE_TEXCOORD,     1 ) ;
		_VertElem[ 9 ] = MakeVertexElement9( 0xff,  0,  D_D3DDECLTYPE_UNUSED,                         0,                       0,     0 ) ;
		Direct3DDevice9_CreateVertexDeclaration( _VertElem, &GD3D9.Device.VertexDeclaration.UserShaderDeclaration[ DX_VERTEX_TYPE_SHADER_3D ] ) ;
	}

	// ����I��
	return 0 ;
}

// Direct3DVertexDeclaration9 �I�u�W�F�N�g�̍폜( 0:����  -1:���s )
extern	int		Graphics_D3D9_VertexDeclaration_Terminate( void )
{
	int i ;

	if( GAPIWin.Direct3DDevice9Object == NULL )
		return 0 ;

	Graphics_D3D9_DeviceState_ResetVertexShader() ;
	Graphics_D3D9_DeviceState_ResetPixelShader() ;

#ifndef DX_NON_MODEL
	D_IDirect3DVertexDeclaration9 **VD ;

	VD = ( D_IDirect3DVertexDeclaration9 ** )GD3D9.Device.VertexDeclaration.MV1_VertexDeclaration ;
	for( i = 0 ; i < sizeof( GD3D9.Device.VertexDeclaration.MV1_VertexDeclaration ) / sizeof( D_IDirect3DVertexDeclaration9 * ) ; i ++ )
	{
		if( VD[ i ] )
		{
			Direct3D9_ObjectRelease( VD[ i ] ) ;
			VD[ i ] = NULL ;
		}
	}

#endif // DX_NON_MODEL

	// �Œ�p�C�v���C���݊��V�F�[�_�[�Ŏg�p���钸�_�錾�̉��
	for( i = 0 ; i < VERTEXFVF_DECL_NUM ; i ++ )
	{
		if( GD3D9.Device.VertexDeclaration.Base2DDeclaration[ i ] )
		{
			Direct3D9_ObjectRelease( GD3D9.Device.VertexDeclaration.Base2DDeclaration[ i ] ) ;
			GD3D9.Device.VertexDeclaration.Base2DDeclaration[ i ] = NULL ;
		}
	}

	// ���[�U�[�V�F�[�_�[���[�h�p�̒��_�錾�̉��
	for( i = 0 ; i < DX_VERTEX_TYPE_NUM ; i ++ )
	{
		if( GD3D9.Device.VertexDeclaration.UserShaderDeclaration[ i ] )
		{
			Direct3D9_ObjectRelease( GD3D9.Device.VertexDeclaration.UserShaderDeclaration[ i ] ) ;
			GD3D9.Device.VertexDeclaration.UserShaderDeclaration[ i ] = NULL ;
		}
	}

	// ����I��
	return 0 ;
}



















// �V�X�e���e�N�X�`���E�T�[�t�F�X�֌W�֐�

// IMAGEDATA_ORIG �� IMAGEDATA_ORIG_HARD_TEX �̏������� Graphics_D3D9_GetSysMemTexture ���g�p����
extern int Graphics_D3D9_GetSysMemTextureFromOrig( struct IMAGEDATA_ORIG *Orig, struct IMAGEDATA_ORIG_HARD_TEX *OrigTex, int ASyncThread )
{
	return Graphics_D3D9_GetSysMemTexture( Orig->FormatDesc.CubeMapTextureFlag, OrigTex->TexWidth, OrigTex->TexHeight, Orig->Hard.MipMapCount,  GD3D9.Device.Caps.TextureFormat[ Orig->ColorFormat ], ASyncThread ) ; 
}


#ifndef DX_NON_ASYNCLOAD

static int Graphics_D3D9_GetSysMemTextureASyncCallback( ASYNCLOAD_MAINTHREAD_REQUESTINFO *Info )
{
	return Graphics_D3D9_GetSysMemTexture(
				( int )Info->Data[ 0 ],
				( int )Info->Data[ 1 ],
				( int )Info->Data[ 2 ],
				( int )Info->Data[ 3 ],
				( D_D3DFORMAT )Info->Data[ 4 ],
				FALSE ) ;
}

#endif // DX_NON_ASYNCLOAD

// �Ǘ��e�N�X�`���ւ̓]���p�̃V�X�e���������e�N�X�`�����擾����
extern int Graphics_D3D9_GetSysMemTexture( int CubeMap, int Width, int Height, int MipMapCount, D_D3DFORMAT Format, int ASyncThread )
{
#ifndef DX_NON_ASYNCLOAD
	if( ASyncThread )
	{
		ASYNCLOAD_MAINTHREAD_REQUESTINFO AInfo ;

		AInfo.Function = Graphics_D3D9_GetSysMemTextureASyncCallback ;
		AInfo.Data[ 0 ] = ( DWORD_PTR )CubeMap ;
		AInfo.Data[ 1 ] = ( DWORD_PTR )Width ;
		AInfo.Data[ 2 ] = ( DWORD_PTR )Height ;
		AInfo.Data[ 3 ] = ( DWORD_PTR )MipMapCount ;
		AInfo.Data[ 4 ] = ( DWORD_PTR )Format ;
		return AddASyncLoadRequestMainThreadInfo( &AInfo ) ;
	}
#endif // DX_NON_ASYNCLOAD

	int i, j ;
	GRAPHICS_HARDDATA_DIRECT3D9_SYSMEMTEXTURE *MemTex ;
	GRAPHICS_HARDDATA_DIRECT3D9_SYSMEMTEXTURE *NewMemTex ;

	// ���ɓK������e�N�X�`���Ŗ��g�p�̂��̂����������ׂ�
	MemTex = GD3D9.SysMemTexSurf.Texture ;
	NewMemTex = NULL ;
	j = 0 ;
	for( i = 0 ; i < GD3D9.SysMemTexSurf.TextureInitNum ; MemTex ++ )
	{
		if( MemTex->InitializeFlag == FALSE )
		{
			if( NewMemTex == NULL )
			{
				NewMemTex = MemTex ;
			}
			continue ;
		}

		i ++ ;

		if( MemTex->CubeMap     != ( unsigned char )CubeMap ||
			MemTex->Width       != ( short )        Width ||
			MemTex->Height      != ( short )        Height ||
			MemTex->MipMapCount != ( unsigned char )MipMapCount ||
			MemTex->Format      !=                  Format )
			continue ;

		j ++ ;

		if( MemTex->UseFlag == TRUE )
		{
			// �����t�H�[�}�b�g�̃e�N�X�`�����K�萔�g�p����������G���[
			if( j == DIRECT3D9_SYSMEMTEXTURE_ONE_FORMAT_NUM )
				return -1 ;
			continue ;
		}

		MemTex->UseCount ++ ;
		MemTex->UseFlag = TRUE ;
		return ( int )( MemTex - GD3D9.SysMemTexSurf.Texture ) ; 
	}

	// �K������e�N�X�`���������A���ׂẴe�N�X�`�����������ς݂̏ꍇ�͖��g�p���Ŏg�p�p�x�̒Ⴂ�e�N�X�`�����������
	if( GD3D9.SysMemTexSurf.TextureInitNum == DIRECT3D9_SYSMEMTEXTURE_NUM )
	{
		unsigned int MinUseCount ;

		MinUseCount = 0xffffffff ;
		MemTex = GD3D9.SysMemTexSurf.Texture ;
		NewMemTex = NULL ;
		for( i = 0 ; i < DIRECT3D9_SYSMEMTEXTURE_NUM ; i ++, MemTex ++ )
		{
			if( MemTex->UseFlag == FALSE &&
				MemTex->UseCount <= MinUseCount )
			{
				MinUseCount = MemTex->UseCount ;
				NewMemTex = MemTex ;
			}
		}

		// ���ׂĂ��g�p���̏ꍇ�̓G���[
		if( NewMemTex == NULL )
		{
			return -1 ;
		}

		// �����̃f�[�^�����
		Direct3D9_ObjectRelease( NewMemTex->MemTexture ) ;
		NewMemTex->MemTexture = NULL ;

		NewMemTex->InitializeFlag = FALSE ;
		NewMemTex->UseCount = 0 ;
		NewMemTex->UseFlag = FALSE ;

		// �������f�[�^�����炷
		GD3D9.SysMemTexSurf.TextureInitNum -- ;
	}
	else
	{
		if( NewMemTex == NULL )
		{
			NewMemTex = MemTex ;
		}
	}

	// �f�[�^�̏�����
	NewMemTex->CubeMap = ( unsigned char )CubeMap ;
	NewMemTex->MipMapCount = ( unsigned char )MipMapCount ;
	NewMemTex->Width = ( short )Width ;
	NewMemTex->Height = ( short )Height ;
	NewMemTex->Format = Format ;
	NewMemTex->UseCount = 1 ;

	// �e�N�X�`���̍쐬
	if( Graphics_D3D9_CreateSysMemTextureDirect3D9Texture( NewMemTex ) < 0 )
	{
		// �g�p����Ă��Ȃ��V�X�e���������e�N�X�`����������āA�ēx�쐬�����݂�
		Graphics_D3D9_ReleaseSysMemTextureAll( TRUE ) ;
		if( Graphics_D3D9_CreateSysMemTextureDirect3D9Texture( NewMemTex ) < 0 )
		{
			return -1 ;
		}
	}

	// �������t���O�Ǝg�p���t���O�𗧂Ă�
	NewMemTex->InitializeFlag = TRUE ;
	NewMemTex->UseFlag = TRUE ;

	// �������f�[�^�̒ǉ�
	GD3D9.SysMemTexSurf.TextureInitNum ++ ;

	return ( int )( NewMemTex - GD3D9.SysMemTexSurf.Texture ) ;
}



#ifndef DX_NON_ASYNCLOAD

static int Graphics_D3D9_GetSysMemSurfaceASyncCallback( ASYNCLOAD_MAINTHREAD_REQUESTINFO *Info )
{
	return Graphics_D3D9_GetSysMemSurface(
				( int )Info->Data[ 0 ],
				( int )Info->Data[ 1 ],
				( D_D3DFORMAT )Info->Data[ 2 ],
				FALSE ) ;
}

#endif // DX_NON_ASYNCLOAD

// �Ǘ��e�N�X�`���ւ̓]���p�̃V�X�e���������e�N�X�`�����擾����
extern int Graphics_D3D9_GetSysMemSurface( int Width, int Height, D_D3DFORMAT Format, int ASyncThread )
{
#ifndef DX_NON_ASYNCLOAD
	if( ASyncThread )
	{
		ASYNCLOAD_MAINTHREAD_REQUESTINFO AInfo ;

		AInfo.Function = Graphics_D3D9_GetSysMemSurfaceASyncCallback ;
		AInfo.Data[ 0 ] = ( DWORD_PTR )Width ;
		AInfo.Data[ 1 ] = ( DWORD_PTR )Height ;
		AInfo.Data[ 2 ] = ( DWORD_PTR )Format ;
		return AddASyncLoadRequestMainThreadInfo( &AInfo ) ;
	}
#endif // DX_NON_ASYNCLOAD

	int i, j ;
	GRAPHICS_HARDDATA_DIRECT3D9_SYSMEMSURFACE *MemSurface ;
	GRAPHICS_HARDDATA_DIRECT3D9_SYSMEMSURFACE *NewMemSurface ;

	// ���ɓK������e�N�X�`���Ŗ��g�p�̂��̂����������ׂ�
	MemSurface = GD3D9.SysMemTexSurf.Surface ;
	NewMemSurface = NULL ;
	j = 0 ;
	for( i = 0 ; i < GD3D9.SysMemTexSurf.SurfaceInitNum ; MemSurface ++ )
	{
		if( MemSurface->InitializeFlag == FALSE )
		{
			if( NewMemSurface == NULL )
			{
				NewMemSurface = MemSurface ;
			}
			continue ;
		}

		i ++ ;

		if( MemSurface->Width       != ( short )Width ||
			MemSurface->Height      != ( short )Height ||
			MemSurface->Format      !=          Format )
			continue ;

		j ++ ;

		if( MemSurface->UseFlag == TRUE )
		{
			// �����t�H�[�}�b�g�̃e�N�X�`�����K�萔�g�p����������G���[
			if( j == DIRECT3D9_SYSMEMSURFACE_ONE_FORMAT_NUM )
				return -1 ;
			continue ;
		}

		MemSurface->UseCount ++ ;
		MemSurface->UseFlag = TRUE ;
		return ( int )( MemSurface - GD3D9.SysMemTexSurf.Surface ) ; 
	}

	// �K������e�N�X�`���������A���ׂẴe�N�X�`�����������ς݂̏ꍇ�͖��g�p���Ŏg�p�p�x�̒Ⴂ�e�N�X�`�����������
	if( GD3D9.SysMemTexSurf.SurfaceInitNum == DIRECT3D9_SYSMEMSURFACE_NUM )
	{
		unsigned int MinUseCount ;

		MinUseCount = 0xffffffff ;
		MemSurface = GD3D9.SysMemTexSurf.Surface ;
		NewMemSurface = NULL ;
		for( i = 0 ; i < DIRECT3D9_SYSMEMSURFACE_NUM ; i ++, MemSurface ++ )
		{
			if( MemSurface->UseFlag == FALSE &&
				MemSurface->UseCount <= MinUseCount )
			{
				MinUseCount = MemSurface->UseCount ;
				NewMemSurface = MemSurface ;
			}
		}

		// ���ׂĂ��g�p���̏ꍇ�̓G���[
		if( NewMemSurface == NULL )
		{
			return -1 ;
		}

		// �����̃f�[�^�����
		Direct3D9_ObjectRelease( NewMemSurface->MemSurface ) ;
		NewMemSurface->MemSurface = NULL ;

		NewMemSurface->InitializeFlag = FALSE ;
		NewMemSurface->UseCount = 0 ;
		NewMemSurface->UseFlag = FALSE ;

		// �������f�[�^�����炷
		GD3D9.SysMemTexSurf.SurfaceInitNum -- ;
	}
	else
	{
		if( NewMemSurface == NULL )
		{
			NewMemSurface = MemSurface ;
		}
	}

	// �f�[�^�̏�����
	NewMemSurface->Width = ( short )Width ;
	NewMemSurface->Height = ( short )Height ;
	NewMemSurface->Format = Format ;
	NewMemSurface->UseCount = 1 ;

	// �e�N�X�`���̍쐬
	if( Graphics_D3D9_CreateSysMemSurfaceDirect3D9Surface( NewMemSurface ) < 0 )
	{
		// �g�p����Ă��Ȃ��V�X�e���������e�N�X�`����������āA�ēx�쐬�����݂�
		Graphics_D3D9_ReleaseSysMemSurfaceAll( TRUE ) ;
		if( Graphics_D3D9_CreateSysMemSurfaceDirect3D9Surface( NewMemSurface ) < 0 )
		{
			return -1 ;
		}
	}

	// �������t���O�Ǝg�p���t���O�𗧂Ă�
	NewMemSurface->InitializeFlag = TRUE ;
	NewMemSurface->UseFlag = TRUE ;

	// �������f�[�^�̒ǉ�
	GD3D9.SysMemTexSurf.SurfaceInitNum ++ ;

	return ( int )( NewMemSurface - GD3D9.SysMemTexSurf.Surface ) ;
}


// �\���̂̃f�[�^�����ɓ]���p�̃V�X�e���������e�N�X�`�����쐬����
extern int Graphics_D3D9_CreateSysMemTextureDirect3D9Texture( GRAPHICS_HARDDATA_DIRECT3D9_SYSMEMTEXTURE *MemTex )
{
	long Result ;

	if( MemTex->CubeMap != 0 )
	{
		Result = Direct3DDevice9_CreateCubeTexture(
			( UINT )MemTex->Width,									// ��
			( UINT )MemTex->MipMapCount,							// MipMap ���x��
			D_D3DUSAGE_DYNAMIC,										// �t���O
			MemTex->Format,											// �t�H�[�}�b�g
			D_D3DPOOL_SYSTEMMEM,									// �e�N�X�`���� DirectX ���Ǘ����邩�ǂ���
			( D_IDirect3DCubeTexture9 ** )&MemTex->MemTexture,		// D_IDirect3DCubeTexture9 �̃|�C���^���󂯎��A�h���X
			NULL													// ��� NULL
		) ;
		if( Result != D_D3D_OK )
		{
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\xe1\x30\xe2\x30\xea\x30\xad\x30\xe5\x30\xfc\x30\xd6\x30\xde\x30\xc3\x30\xd7\x30\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x20\x00\x20\x00\xa8\x30\xe9\x30\xfc\x30\xb3\x30\xfc\x30\xc9\x30\x3a\x00\x30\x00\x78\x00\x25\x00\x30\x00\x38\x00\x78\x00\x20\x00\x53\x00\x69\x00\x7a\x00\x65\x00\x58\x00\x3a\x00\x25\x00\x64\x00\x20\x00\x53\x00\x69\x00\x7a\x00\x65\x00\x59\x00\x3a\x00\x25\x00\x64\x00\x20\x00\x46\x00\x6f\x00\x72\x00\x6d\x00\x61\x00\x74\x00\x3a\x00\x25\x00\x64\x00\x20\x00\xcf\x7d\xe1\x30\xe2\x30\xea\x30\xba\x78\xdd\x4f\x70\x65\x3a\x00\x25\x00\x64\x00\x20\x00\xcf\x7d\xba\x78\xdd\x4f\xe1\x30\xe2\x30\xea\x30\xb5\x30\xa4\x30\xba\x30\x3a\x00\x25\x00\x64\x00\x42\x00\x79\x00\x74\x00\x65\x00\x28\x00\x25\x00\x64\x00\x4b\x00\x42\x00\x79\x00\x74\x00\x65\x00\x29\x00\x00"/*@ L"�������L���[�u�}�b�v�e�N�X�`���̍쐬�Ɏ��s���܂���  �G���[�R�[�h:0x%08x SizeX:%d SizeY:%d Format:%d ���������m�ې�:%d ���m�ۃ������T�C�Y:%dByte(%dKByte)" @*/,
				Result, MemTex->Width, MemTex->Height, GD3D9.Device.Caps.TextureFormat[ MemTex->Format ], NS_DxGetAllocNum(), NS_DxGetAllocSize(), NS_DxGetAllocSize() )) ;
			return -1 ;
		}
	}
	else
	{
		Result = Direct3DDevice9_CreateTexture(
			( UINT )MemTex->Width,							// ��
			( UINT )MemTex->Height,							// ����
			MemTex->MipMapCount,							// MipMap ���x��
			D_D3DUSAGE_DYNAMIC,								// �t���O
			MemTex->Format,									// �t�H�[�}�b�g
			D_D3DPOOL_SYSTEMMEM,							// �e�N�X�`���� DirectX ���Ǘ����邩�ǂ���
			( D_IDirect3DTexture9 ** )&MemTex->MemTexture,	// D_IDirect3DTexture9 �̃|�C���^���󂯎��A�h���X
			NULL											// ��� NULL
		) ;
		if( Result != D_D3D_OK )
		{
			static bool OutPutLog = false ;

			DXST_ERRORLOGFMT_ADDUTF16LE(( "\xe1\x30\xe2\x30\xea\x30\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x20\x00\x20\x00\xa8\x30\xe9\x30\xfc\x30\xb3\x30\xfc\x30\xc9\x30\x3a\x00\x30\x00\x78\x00\x25\x00\x30\x00\x38\x00\x78\x00\x20\x00\x53\x00\x69\x00\x7a\x00\x65\x00\x58\x00\x3a\x00\x25\x00\x64\x00\x20\x00\x53\x00\x69\x00\x7a\x00\x65\x00\x59\x00\x3a\x00\x25\x00\x64\x00\x20\x00\x46\x00\x6f\x00\x72\x00\x6d\x00\x61\x00\x74\x00\x3a\x00\x25\x00\x64\x00\x20\x00\xcf\x7d\xe1\x30\xe2\x30\xea\x30\xba\x78\xdd\x4f\x70\x65\x3a\x00\x25\x00\x64\x00\x20\x00\xcf\x7d\xba\x78\xdd\x4f\xe1\x30\xe2\x30\xea\x30\xb5\x30\xa4\x30\xba\x30\x3a\x00\x25\x00\x64\x00\x42\x00\x79\x00\x74\x00\x65\x00\x28\x00\x25\x00\x64\x00\x4b\x00\x42\x00\x79\x00\x74\x00\x65\x00\x29\x00\x00"/*@ L"�������e�N�X�`���̍쐬�Ɏ��s���܂���  �G���[�R�[�h:0x%08x SizeX:%d SizeY:%d Format:%d ���������m�ې�:%d ���m�ۃ������T�C�Y:%dByte(%dKByte)" @*/,
				Result, MemTex->Width, MemTex->Height, GD3D9.Device.Caps.TextureFormat[ MemTex->Format ], NS_DxGetAllocNum(), NS_DxGetAllocSize(), NS_DxGetAllocSize() )) ;

			if( OutPutLog == false )
			{
				OutPutLog = true ;

				DXST_ERRORLOGFMT_ADDUTF16LE(( "\xe1\x30\xe2\x30\xea\x30\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x20\x00\x20\x00\xa8\x30\xe9\x30\xfc\x30\xb3\x30\xfc\x30\xc9\x30\x3a\x00\x30\x00\x78\x00\x25\x00\x30\x00\x38\x00\x78\x00\x20\x00\x53\x00\x69\x00\x7a\x00\x65\x00\x58\x00\x3a\x00\x25\x00\x64\x00\x20\x00\x53\x00\x69\x00\x7a\x00\x65\x00\x59\x00\x3a\x00\x25\x00\x64\x00\x20\x00\x46\x00\x6f\x00\x72\x00\x6d\x00\x61\x00\x74\x00\x3a\x00\x25\x00\x64\x00\x20\x00\xcf\x7d\xe1\x30\xe2\x30\xea\x30\xba\x78\xdd\x4f\x70\x65\x3a\x00\x25\x00\x64\x00\x20\x00\xcf\x7d\xba\x78\xdd\x4f\xe1\x30\xe2\x30\xea\x30\xb5\x30\xa4\x30\xba\x30\x3a\x00\x25\x00\x64\x00\x42\x00\x79\x00\x74\x00\x65\x00\x28\x00\x25\x00\x64\x00\x4b\x00\x42\x00\x79\x00\x74\x00\x65\x00\x29\x00\x00"/*@ L"�������e�N�X�`���̍쐬�Ɏ��s���܂���  �G���[�R�[�h:0x%08x SizeX:%d SizeY:%d Format:%d ���������m�ې�:%d ���m�ۃ������T�C�Y:%dByte(%dKByte)" @*/,
					Result, MemTex->Width, MemTex->Height, GD3D9.Device.Caps.TextureFormat[ MemTex->Format ], NS_DxGetAllocNum(), NS_DxGetAllocSize(), NS_DxGetAllocSize() )) ;
				DXST_ERRORLOGFMT_ADDUTF16LE(( "\x5e\x97\xa1\x7b\x06\x74\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\x78\x30\x6e\x30\xc7\x30\xfc\x30\xbf\x30\xe2\x8e\x01\x90\x28\x75\xb7\x30\xb9\x30\xc6\x30\xe0\x30\xe1\x30\xe2\x30\xea\x30\x4d\x91\x6e\x7f\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\x6e\x30\x70\x65\x1a\xff\x25\x00\x64\x00\x00"/*@ L"��Ǘ��e�N�X�`���ւ̃f�[�^�]���p�V�X�e���������z�u�e�N�X�`���̐��F%d" @*/, GD3D9.SysMemTexSurf.TextureInitNum )) ;

				int i, j ;
				MemTex = GD3D9.SysMemTexSurf.Texture ;
				j = 0 ;
				for( i = 0 ; i < GD3D9.SysMemTexSurf.TextureInitNum ; j ++ , MemTex ++ )
				{
					if( MemTex->InitializeFlag == FALSE )
					{
						continue ;
					}
					i++ ;
					DXST_ERRORLOGFMT_ADDW(( L"No%d. UseFlag:%d UseCount:%d CubeMap:%d SizeX:%d SizeY:%d MipMapCount:%d Format:%d",
						j, MemTex->UseFlag, MemTex->UseCount, MemTex->CubeMap, MemTex->Width, MemTex->Height, MemTex->MipMapCount, MemTex->Format )) ;
				}
			}
			return -1 ;
		}
	}

	// ����I��
	return 0 ;
}


// �\���̂̃f�[�^�����ɓ]���p�̃V�X�e���������T�[�t�F�X���쐬����
extern int Graphics_D3D9_CreateSysMemSurfaceDirect3D9Surface( GRAPHICS_HARDDATA_DIRECT3D9_SYSMEMSURFACE *MemSurface )
{
	long Result ;

	Result = Direct3DDevice9_CreateOffscreenPlainSurface(
		( UINT )MemSurface->Width,							// ��
		( UINT )MemSurface->Height,							// ����
		MemSurface->Format,									// �t�H�[�}�b�g
		D_D3DPOOL_SYSTEMMEM,								// �e�N�X�`���� DirectX ���Ǘ����邩�ǂ���
		( D_IDirect3DSurface9 ** )&MemSurface->MemSurface,	// D_IDirect3DSurface9 �̃|�C���^���󂯎��A�h���X
		NULL												// ��� NULL
	) ;
	if( Result != D_D3D_OK )
	{
		static bool OutPutLog = false ;

		DXST_ERRORLOGFMT_ADDUTF16LE(( "\xe1\x30\xe2\x30\xea\x30\xb5\x30\xfc\x30\xd5\x30\xa7\x30\xb9\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x20\x00\x50\x00\x6f\x00\x73\x00\x4e\x00\x6f\x00\x2e\x00\x31\x00\x20\x00\x20\x00\xa8\x30\xe9\x30\xfc\x30\xb3\x30\xfc\x30\xc9\x30\x3a\x00\x30\x00\x78\x00\x25\x00\x30\x00\x38\x00\x78\x00\x20\x00\x53\x00\x69\x00\x7a\x00\x65\x00\x58\x00\x3a\x00\x25\x00\x64\x00\x20\x00\x53\x00\x69\x00\x7a\x00\x65\x00\x59\x00\x3a\x00\x25\x00\x64\x00\x20\x00\x46\x00\x6f\x00\x72\x00\x6d\x00\x61\x00\x74\x00\x3a\x00\x25\x00\x64\x00\x20\x00\xcf\x7d\xe1\x30\xe2\x30\xea\x30\xba\x78\xdd\x4f\x70\x65\x3a\x00\x25\x00\x64\x00\x20\x00\xcf\x7d\xba\x78\xdd\x4f\xe1\x30\xe2\x30\xea\x30\xb5\x30\xa4\x30\xba\x30\x3a\x00\x25\x00\x64\x00\x42\x00\x79\x00\x74\x00\x65\x00\x28\x00\x25\x00\x64\x00\x4b\x00\x42\x00\x79\x00\x74\x00\x65\x00\x29\x00\x00"/*@ L"�������T�[�t�F�X�̍쐬�Ɏ��s���܂��� PosNo.1  �G���[�R�[�h:0x%08x SizeX:%d SizeY:%d Format:%d ���������m�ې�:%d ���m�ۃ������T�C�Y:%dByte(%dKByte)" @*/,
			Result, MemSurface->Width, MemSurface->Height, MemSurface->Format, NS_DxGetAllocNum(), NS_DxGetAllocSize(), NS_DxGetAllocSize() / 1024 )) ;

		if( OutPutLog == false )
		{
			OutPutLog = true ;

			DXST_ERRORLOGFMT_ADDUTF16LE(( "\xe1\x30\xe2\x30\xea\x30\xb5\x30\xfc\x30\xd5\x30\xa7\x30\xb9\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x20\x00\x50\x00\x6f\x00\x73\x00\x4e\x00\x6f\x00\x2e\x00\x32\x00\x20\x00\x20\x00\xa8\x30\xe9\x30\xfc\x30\xb3\x30\xfc\x30\xc9\x30\x3a\x00\x30\x00\x78\x00\x25\x00\x30\x00\x38\x00\x78\x00\x20\x00\x53\x00\x69\x00\x7a\x00\x65\x00\x58\x00\x3a\x00\x25\x00\x64\x00\x20\x00\x53\x00\x69\x00\x7a\x00\x65\x00\x59\x00\x3a\x00\x25\x00\x64\x00\x20\x00\x46\x00\x6f\x00\x72\x00\x6d\x00\x61\x00\x74\x00\x3a\x00\x25\x00\x64\x00\x20\x00\xcf\x7d\xe1\x30\xe2\x30\xea\x30\xba\x78\xdd\x4f\x70\x65\x3a\x00\x25\x00\x64\x00\x20\x00\xcf\x7d\xba\x78\xdd\x4f\xe1\x30\xe2\x30\xea\x30\xb5\x30\xa4\x30\xba\x30\x3a\x00\x25\x00\x64\x00\x42\x00\x79\x00\x74\x00\x65\x00\x28\x00\x25\x00\x64\x00\x4b\x00\x42\x00\x79\x00\x74\x00\x65\x00\x29\x00\x00"/*@ L"�������T�[�t�F�X�̍쐬�Ɏ��s���܂��� PosNo.2  �G���[�R�[�h:0x%08x SizeX:%d SizeY:%d Format:%d ���������m�ې�:%d ���m�ۃ������T�C�Y:%dByte(%dKByte)" @*/,
				Result, MemSurface->Width, MemSurface->Height, MemSurface->Format, NS_DxGetAllocNum(), NS_DxGetAllocSize(), NS_DxGetAllocSize() / 1024 )) ;
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\x5e\x97\xa1\x7b\x06\x74\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\x78\x30\x6e\x30\xc7\x30\xfc\x30\xbf\x30\xe2\x8e\x01\x90\x28\x75\xb7\x30\xb9\x30\xc6\x30\xe0\x30\xe1\x30\xe2\x30\xea\x30\x4d\x91\x6e\x7f\xb5\x30\xfc\x30\xd5\x30\xa7\x30\xb9\x30\x6e\x30\x70\x65\x1a\xff\x25\x00\x64\x00\x00"/*@ L"��Ǘ��e�N�X�`���ւ̃f�[�^�]���p�V�X�e���������z�u�T�[�t�F�X�̐��F%d" @*/, GD3D9.SysMemTexSurf.TextureInitNum )) ;

			int i, j ;
			MemSurface = GD3D9.SysMemTexSurf.Surface ;
			j = 0 ;
			for( i = 0 ; i < GD3D9.SysMemTexSurf.SurfaceInitNum ; j ++ , MemSurface ++ )
			{
				if( MemSurface->InitializeFlag == FALSE )
				{
					continue ;
				}
				i++ ;
				DXST_ERRORLOGFMT_ADDW(( L"No%d. UseFlag:%d UseCount:%d SizeX:%d SizeY:%d Format:%d",
					j, MemSurface->UseFlag, MemSurface->UseCount, MemSurface->Width, MemSurface->Height, MemSurface->Format )) ;
			}
		}
		return -1 ;
	}

	// ����I��
	return 0 ;
}


#ifndef DX_NON_ASYNCLOAD

static int Graphics_D3D9_ReleaseSysMemTextureASyncCallback( ASYNCLOAD_MAINTHREAD_REQUESTINFO *Info )
{
	return Graphics_D3D9_ReleaseSysMemTexture(
				( int )Info->Data[ 0 ],
				FALSE ) ;
}

#endif // DX_NON_ASYNCLOAD

// �Ǘ��e�N�X�`���ւ̓]���p�̃V�X�e���������e�N�X�`���̎擾���������
extern int Graphics_D3D9_ReleaseSysMemTexture( int Index, int ASyncThread )
{
#ifndef DX_NON_ASYNCLOAD
	if( ASyncThread )
	{
		ASYNCLOAD_MAINTHREAD_REQUESTINFO AInfo ;

		AInfo.Function = Graphics_D3D9_ReleaseSysMemTextureASyncCallback ;
		AInfo.Data[ 0 ] = ( DWORD_PTR )Index ;
		return AddASyncLoadRequestMainThreadInfo( &AInfo ) ;
	}
#endif // DX_NON_ASYNCLOAD

	GRAPHICS_HARDDATA_DIRECT3D9_SYSMEMTEXTURE *MemTex ;

	MemTex = &GD3D9.SysMemTexSurf.Texture[ Index ] ;

	if( MemTex->InitializeFlag == FALSE ||
		MemTex->UseFlag == FALSE )
		return 0 ;

	MemTex->UseFlag = FALSE ;
	MemTex->UseTime = NS_GetNowCount() ;

	// �I��
	return 0 ;
}


#ifndef DX_NON_ASYNCLOAD

static int Graphics_D3D9_ReleaseSysMemSurfaceASyncCallback( ASYNCLOAD_MAINTHREAD_REQUESTINFO *Info )
{
	return Graphics_D3D9_ReleaseSysMemSurface(
				( int )Info->Data[ 0 ],
				FALSE ) ;
}

#endif // DX_NON_ASYNCLOAD

// �Ǘ��e�N�X�`���ւ̓]���p�̃V�X�e���������T�[�t�F�X�̎擾���������
extern int Graphics_D3D9_ReleaseSysMemSurface( int Index, int ASyncThread )
{
#ifndef DX_NON_ASYNCLOAD
	if( ASyncThread )
	{
		ASYNCLOAD_MAINTHREAD_REQUESTINFO AInfo ;

		AInfo.Function = Graphics_D3D9_ReleaseSysMemSurfaceASyncCallback ;
		AInfo.Data[ 0 ] = ( DWORD_PTR )Index ;
		return AddASyncLoadRequestMainThreadInfo( &AInfo ) ;
	}
#endif // DX_NON_ASYNCLOAD

	GRAPHICS_HARDDATA_DIRECT3D9_SYSMEMSURFACE *MemSurface ;

	MemSurface = &GD3D9.SysMemTexSurf.Surface[ Index ] ;

	if( MemSurface->InitializeFlag == FALSE ||
		MemSurface->UseFlag == FALSE )
		return 0 ;

	MemSurface->UseFlag = FALSE ;
	MemSurface->UseTime = NS_GetNowCount() ;

	// �I��
	return 0 ;
}


// �Ǘ��e�N�X�`���ւ̓]���p�̃V�X�e���������e�N�X�`�������ׂĉ������
extern int Graphics_D3D9_ReleaseSysMemTextureAll( int IsNotUseOnly )
{
	int i ;
	int ReleaseCount ;
	GRAPHICS_HARDDATA_DIRECT3D9_SYSMEMTEXTURE *MemTex ;

	ReleaseCount = 0 ;
	MemTex = GD3D9.SysMemTexSurf.Texture ;
	for( i = 0 ; i < GD3D9.SysMemTexSurf.TextureInitNum ; MemTex ++ )
	{
		if( MemTex->InitializeFlag == FALSE )
		{
			continue ;
		}

		i ++ ;

		if( IsNotUseOnly && MemTex->UseFlag )
		{
			continue ;
		}

		if( MemTex->MemTexture != NULL )
		{
			Direct3D9_ObjectRelease( MemTex->MemTexture ) ;
			MemTex->MemTexture = NULL ;
		}

		MemTex->InitializeFlag = FALSE ;
		MemTex->UseCount = 0 ;
		MemTex->UseFlag = FALSE ;

		ReleaseCount ++ ;
	}
	GD3D9.SysMemTexSurf.TextureInitNum -= ReleaseCount ;

	// �I��
	return 0 ;
}

// �Ǘ��e�N�X�`���ւ̓]���p�̃V�X�e���������T�[�t�F�X�����ׂĉ������
extern int Graphics_D3D9_ReleaseSysMemSurfaceAll( int IsNotUseOnly )
{
	int i ;
	int ReleaseCount ;
	GRAPHICS_HARDDATA_DIRECT3D9_SYSMEMSURFACE *MemSurface ;

	ReleaseCount = 0 ;
	MemSurface = GD3D9.SysMemTexSurf.Surface ;
	for( i = 0 ; i < GD3D9.SysMemTexSurf.SurfaceInitNum ; MemSurface ++ )
	{
		if( MemSurface->InitializeFlag == FALSE )
		{
			continue ;
		}

		i ++ ;

		if( IsNotUseOnly && MemSurface->UseFlag )
		{
			continue ;
		}

		if( MemSurface->MemSurface != NULL )
		{
			Direct3D9_ObjectRelease( MemSurface->MemSurface ) ;
			MemSurface->MemSurface = NULL ;
		}

		MemSurface->InitializeFlag = FALSE ;
		MemSurface->UseCount = 0 ;
		MemSurface->UseFlag = FALSE ;

		ReleaseCount ++ ;
	}
	GD3D9.SysMemTexSurf.SurfaceInitNum -= ReleaseCount ;

	// �I��
	return 0 ;
}


// �Ǘ��e�N�X�`���ւ̓]���p�̃V�X�e���������e�N�X�`���̒���������s��
extern int Graphics_D3D9_SysMemTextureProcess( void )
{
	int i ;
	int ReleaseCount ;
	GRAPHICS_HARDDATA_DIRECT3D9_SYSMEMTEXTURE *MemTex ;
	int NowTime ;

#ifndef DX_NON_ASYNCLOAD
	// ���C���X���b�h�ȊO����Ă΂ꂽ�ꍇ�͉������Ȃ�
	if( CheckMainThread() == FALSE )
	{
		return -1 ;
	}
#endif // DX_NON_ASYNCLOAD

	NowTime = NS_GetNowCount() ;

	MemTex = GD3D9.SysMemTexSurf.Texture ;
	ReleaseCount = 0 ;
	for( i = 0 ; i < GD3D9.SysMemTexSurf.TextureInitNum ; MemTex ++ )
	{
		if( MemTex->InitializeFlag == FALSE )
		{
			continue ;
		}

		i ++ ;

		// �Ō�Ɏg�p���Ă���Q�b�ȏ�o�߂��Ă�����������
		if( MemTex->UseFlag || NowTime - MemTex->UseTime < 2000 )
		{
			continue ;
		}

		if( MemTex->MemTexture != NULL )
		{
			Direct3D9_ObjectRelease( MemTex->MemTexture ) ;
			MemTex->MemTexture = NULL ;
		}

		MemTex->InitializeFlag = FALSE ;
		MemTex->UseCount = 0 ;
		MemTex->UseFlag = FALSE ;

		ReleaseCount ++ ;
	}
	GD3D9.SysMemTexSurf.TextureInitNum -= ReleaseCount ;

	// �I��
	return 0 ;
}


// �Ǘ��e�N�X�`���ւ̓]���p�̃V�X�e���������T�[�t�F�X�̒���������s��
extern int Graphics_D3D9_SysMemSurfaceProcess( void )
{
	int i ;
	int ReleaseCount ;
	GRAPHICS_HARDDATA_DIRECT3D9_SYSMEMSURFACE *MemSurface ;
	int NowTime ;

#ifndef DX_NON_ASYNCLOAD
	// ���C���X���b�h�ȊO����Ă΂ꂽ�ꍇ�͉������Ȃ�
	if( CheckMainThread() == FALSE )
	{
		return -1 ;
	}
#endif // DX_NON_ASYNCLOAD

	NowTime = NS_GetNowCount() ;

	MemSurface = GD3D9.SysMemTexSurf.Surface ;
	ReleaseCount = 0 ;
	for( i = 0 ; i < GD3D9.SysMemTexSurf.SurfaceInitNum ; MemSurface ++ )
	{
		if( MemSurface->InitializeFlag == FALSE )
		{
			continue ;
		}

		i ++ ;

		// �Ō�Ɏg�p���Ă���Q�b�ȏ�o�߂��Ă�����������
		if( MemSurface->UseFlag || NowTime - MemSurface->UseTime < 2000 )
		{
			continue ;
		}

		if( MemSurface->MemSurface != NULL )
		{
			Direct3D9_ObjectRelease( MemSurface->MemSurface ) ;
			MemSurface->MemSurface = NULL ;
		}

		MemSurface->InitializeFlag = FALSE ;
		MemSurface->UseCount = 0 ;
		MemSurface->UseFlag = FALSE ;

		ReleaseCount ++ ;
	}
	GD3D9.SysMemTexSurf.SurfaceInitNum -= ReleaseCount ;

	// �I��
	return 0 ;
}






























// Direct3D9 �̕`�揈�������֌W

#ifndef DX_NON_ASYNCLOAD
static int Graphics_D3D9_RenderVertexASyncCallback( ASYNCLOAD_MAINTHREAD_REQUESTINFO * /*Info*/ )
{
	return Graphics_D3D9_RenderVertex( FALSE ) ;
}
#endif // DX_NON_ASYNCLOAD

// ���_�o�b�t�@�ɗ��܂������_�f�[�^�������_�����O����
extern	int		Graphics_D3D9_RenderVertex( int ASyncThread )
{
#ifndef DX_NON_ASYNCLOAD
	if( ASyncThread )
	{
		ASYNCLOAD_MAINTHREAD_REQUESTINFO AInfo ;

		AInfo.Function = Graphics_D3D9_RenderVertexASyncCallback ;
		return AddASyncLoadRequestMainThreadInfo( &AInfo ) ;
	}
#endif // DX_NON_ASYNCLOAD

	if( GAPIWin.Direct3DDevice9Object == NULL ) return -1 ;

	// ���_���������������`��͍s��Ȃ�
	if( GD3D9.Device.DrawInfo.VertexNum != 0 && DxLib_GetEndRequest() == FALSE )
	{
		// Graphics_D3D9_BeginScene �����Ă��Ȃ������炷��
		if( GD3D9.Device.DrawInfo.BeginSceneFlag == FALSE ) Graphics_D3D9_BeginScene() ;

		// ��`��t���O�������Ă��Ȃ���΃����_�����O����
		if( GD3D9.Device.DrawInfo.BlendMaxNotDrawFlag == FALSE )
		{
			UINT PrimNum ;

			PrimNum = 0 ;
			switch( GD3D9.Device.DrawInfo.PrimitiveType )
			{
			case D_D3DPT_POINTLIST     : PrimNum = ( UINT )( GD3D9.Device.DrawInfo.VertexNum     ) ; break ;
			case D_D3DPT_LINELIST      : PrimNum = ( UINT )( GD3D9.Device.DrawInfo.VertexNum / 2 ) ; break ;
			case D_D3DPT_LINESTRIP     : PrimNum = ( UINT )( GD3D9.Device.DrawInfo.VertexNum - 1 ) ; break ;
			case D_D3DPT_TRIANGLELIST  : PrimNum = ( UINT )( GD3D9.Device.DrawInfo.VertexNum / 3 ) ; break ;
			case D_D3DPT_TRIANGLESTRIP : PrimNum = ( UINT )( GD3D9.Device.DrawInfo.VertexNum - 2 ) ; break ;
			case D_D3DPT_TRIANGLEFAN   : PrimNum = ( UINT )( GD3D9.Device.DrawInfo.VertexNum - 2 ) ; break ;
			}

//			SETFVF( D3DDev_VertexType[ GD3D9.Device.DrawInfo.Use3DVertex ][ GD3D9.Device.DrawInfo.VertexType ] );
//			Graphics_D3D9_DeviceState_SetFVF( D3DDev_VertexType[ GD3D9.Device.DrawInfo.Use3DVertex ][ GD3D9.Device.DrawInfo.VertexType ] ) ;
			Graphics_Hardware_D3D9_SetFVF_UseTable( D3DDev_VertexDeclaration[ GD3D9.Device.DrawInfo.Use3DVertex ][ GD3D9.Device.DrawInfo.VertexType ] ) ;
//			if( GD3D9.Device.DrawInfo.Use3DVertex )
//			{
//				Graphics_D3D9_DeviceState_SetWorldMatrix( &GlobalIdentMatrix ) ;
//			}
			Direct3DDevice9_DrawPrimitiveUP(
				GD3D9.Device.DrawInfo.PrimitiveType,
				PrimNum,
				GD3D9.Device.DrawInfo.VertexBufferAddr[ GD3D9.Device.DrawInfo.Use3DVertex ][ GD3D9.Device.DrawInfo.VertexType ],
				D3DDev_VertexSize[ GD3D9.Device.DrawInfo.Use3DVertex ][ GD3D9.Device.DrawInfo.VertexType ] ) ;

//			if( GD3D9.Device.DrawInfo.Use3DVertex )
//			{
//				Graphics_D3D9_DeviceState_SetWorldMatrix( &GSYS.DrawSetting.WorldMatrixF ) ;
//			}
//			RenderVertexDrawPrimitive_PF() ;
		}
	}

	GD3D9.Device.DrawInfo.VertexNum = 0 ;
	GD3D9.Device.DrawInfo.VertexBufferNextAddr = GD3D9.Device.DrawInfo.VertexBufferAddr[ GD3D9.Device.DrawInfo.Use3DVertex ][ GD3D9.Device.DrawInfo.VertexType ] ;

	// �I��
	return 0 ;
}

// �`�揀�����s��
extern	void	FASTCALL Graphics_D3D9_DrawPreparation( int ParamFlag )
{
	int AlphaTest ;
	int AlphaChannel ;
	int UseDiffuseRGB ;
	int UseDiffuseAlpha ;
	int ShadeMode ;
	int Specular ;
	int UseShader ;
	int Is3D ;
//	int EdgeFont ;

	// ���t���t�H���g�̕`�悩�ǂ����̃t���O���擾
//	EdgeFont = ( ParamFlag & DX_D3D9_DRAWPREP_EDGEFONT ) != 0 ? TRUE : FALSE ;

	// �K�� Graphics_D3D9_DrawPreparation ���s���ׂ��Ƃ����t���O��|��
	GD3D9.Device.DrawSetting.DrawPrepAlwaysFlag = FALSE ;

	// �ݒ肪�ύX���ꂽ���ǂ����̃t���O��|��
	GSYS.ChangeSettingFlag = FALSE ;

	UseShader = GSYS.HardInfo.UseShader && GD3D9.NormalDraw_NotUsePixelShader == FALSE ;
	Is3D      = ( ParamFlag & DX_D3D9_DRAWPREP_3D ) != 0 ;

	// �V�F�[�_�[���Z�b�g����Ă�����O��
	if( ( ParamFlag & DX_D3D9_DRAWPREP_NOTSHADERRESET ) == 0 )
	{
		Graphics_D3D9_DeviceState_ResetVertexShader( TRUE ) ;
		Graphics_D3D9_DeviceState_ResetPixelShader( TRUE ) ;
	}

	Graphics_D3D9_DeviceState_SetIndexBuffer( NULL ) ;
	Graphics_D3D9_DeviceState_SetVertexBuffer( NULL ) ;

	// ���C�g���g�����ǂ����ŏ����𕪊�
	if( ( ParamFlag & DX_D3D9_DRAWPREP_LIGHTING ) != 0 && GSYS.Light.ProcessDisable == FALSE )
	{
		// ���C�g�̐ݒ肪�ύX����Ă�����ύX��K������
		if( GSYS.Light.HardwareChange )
		{
			Graphics_Light_RefreshState() ;
		}
		else
		{
			// ���C�g���L���Ȃ̂Ɉꎞ�I�ɖ����ɂȂ��Ă�����L���ɂ���
			if( GD3D9.Device.State.Lighting == 0 )
			{
				Graphics_D3D9_DeviceState_SetLighting( TRUE ) ;
			}
		}

		// �}�e���A���̐ݒ肪�O������ύX����Ă����猳�ɖ߂�
		if( GSYS.Light.ChangeMaterial )
		{
			GSYS.Light.ChangeMaterial = 0 ;
			Graphics_D3D9_DeviceState_SetMaterial( ( D_D3DMATERIAL9 * )&GSYS.Light.Material ) ;
		}

		// ���_�J���[���}�e���A���̃J���[�Ƃ��Ďg�p���邩�ǂ������Z�b�g����
		if( ( GSYS.Light.MaterialNotUseVertexDiffuseColor ? FALSE : TRUE ) != GD3D9.Device.State.MaterialUseVertexDiffuseColor )
		{
			Graphics_D3D9_DeviceState_SetDiffuseMaterialSource( GSYS.Light.MaterialNotUseVertexDiffuseColor ? FALSE : TRUE ) ;
		}

		if( ( GSYS.Light.MaterialNotUseVertexSpecularColor ? FALSE : TRUE ) != GD3D9.Device.State.MaterialUseVertexSpecularColor )
		{
			Graphics_D3D9_DeviceState_SetSpecularMaterialSource( GSYS.Light.MaterialNotUseVertexSpecularColor ? FALSE : TRUE ) ;
		}
	}
	else
	{
		// ���C�g���L���ɂȂ��Ă����疳���ɂ���
		if( GD3D9.Device.State.Lighting == 1 )
		{
			Graphics_D3D9_DeviceState_SetLighting( FALSE ) ;
		}
	}

	// �X�y�L�������g�p���邩�ǂ����̐ݒ���s��
	Specular = ( GSYS.DrawSetting.NotUseSpecular == FALSE && ( ParamFlag & DX_D3D9_DRAWPREP_SPECULAR ) != 0 ) ? TRUE : FALSE ;
	if( Specular != GD3D9.Device.State.UseSpecular )
	{
		Graphics_D3D9_DeviceState_SetSpecularEnable( Specular ) ;
	}

	// �ˉe�s��̐ݒ���s��
//	Graphics_DrawSetting_SetUse2DProjectionMatrix( ( ParamFlag & DX_D3D9_DRAWPREP_3D ) == 0 ? TRUE : FALSE ) ;

	// �y�o�b�t�@�̐ݒ���s��
	if( Is3D )
	{
		if( GSYS.DrawSetting.EnableZBufferFlag3D )
		{
			if( GD3D9.Device.State.ZEnable      != GSYS.DrawSetting.EnableZBufferFlag3D ) Graphics_D3D9_DeviceState_SetZEnable(      GSYS.DrawSetting.EnableZBufferFlag3D ) ;
			if( GD3D9.Device.State.ZWriteEnable != GSYS.DrawSetting.WriteZBufferFlag3D  ) Graphics_D3D9_DeviceState_SetZWriteEnable( GSYS.DrawSetting.WriteZBufferFlag3D  ) ;
			if( GD3D9.Device.State.ZFunc        != GSYS.DrawSetting.ZBufferCmpType3D    ) Graphics_D3D9_DeviceState_SetZFunc(        GSYS.DrawSetting.ZBufferCmpType3D    ) ;
			if( GD3D9.Device.State.DepthBias    != GSYS.DrawSetting.ZBias3D             ) Graphics_D3D9_DeviceState_SetDepthBias(    GSYS.DrawSetting.ZBias3D             ) ;
		}
		else
		{
			if( GD3D9.Device.State.ZEnable ) Graphics_D3D9_DeviceState_SetZEnable( FALSE ) ;
		}
	}
	else
	{
		if( GSYS.DrawSetting.EnableZBufferFlag2D )
		{
			if( GD3D9.Device.State.ZEnable      != GSYS.DrawSetting.EnableZBufferFlag2D ) Graphics_D3D9_DeviceState_SetZEnable(      GSYS.DrawSetting.EnableZBufferFlag2D ) ;
			if( GD3D9.Device.State.ZWriteEnable != GSYS.DrawSetting.WriteZBufferFlag2D  ) Graphics_D3D9_DeviceState_SetZWriteEnable( GSYS.DrawSetting.WriteZBufferFlag2D  ) ;
			if( GD3D9.Device.State.ZFunc        != GSYS.DrawSetting.ZBufferCmpType2D    ) Graphics_D3D9_DeviceState_SetZFunc(        GSYS.DrawSetting.ZBufferCmpType2D    ) ;
			if( GD3D9.Device.State.DepthBias    != GSYS.DrawSetting.ZBias2D             ) Graphics_D3D9_DeviceState_SetDepthBias(    GSYS.DrawSetting.ZBias2D             ) ;
		}
		else
		{
			if( GD3D9.Device.State.ZEnable      ) Graphics_D3D9_DeviceState_SetZEnable( FALSE ) ;
			if( GD3D9.Device.State.ZWriteEnable ) Graphics_D3D9_DeviceState_SetZWriteEnable( FALSE ) ;
		}
	}

	// �t�B�����[�h�̃Z�b�g
	if( GD3D9.Device.State.FillMode != GSYS.DrawSetting.FillMode ) Graphics_D3D9_DeviceState_SetFillMode( GSYS.DrawSetting.FillMode ) ;

	// �t�H�O�̐ݒ���s��
	if( ParamFlag & DX_D3D9_DRAWPREP_FOG )
	{
		if( GD3D9.Device.State.FogEnable != GSYS.DrawSetting.FogEnable ) Graphics_D3D9_DeviceState_SetFogEnable( GSYS.DrawSetting.FogEnable ) ;
	}
	else
	{
		if( GD3D9.Device.State.FogEnable ) Graphics_D3D9_DeviceState_SetFogEnable( FALSE ) ;
	}

	// �u�����f�B���O�֌W�̃Z�b�e�B���O���s���ꍇ�̂ݎ��s����
	if( ( ParamFlag & DX_D3D9_DRAWPREP_NOBLENDSETTING ) == 0 )
	{
		// �O��Ƃ܂����������ꍇ�͉��������ɏI��
//		if( Format    == GRH.DrawPrepFormat   &&
//			Texture   == GRH.DrawPrepTexture  &&
//			ParamFlag == GD3D9.Device.DrawSetting.DrawPrepParamFlag ) return ;

		// �t���O�̏�����
		AlphaTest    = FALSE ;
		AlphaChannel = FALSE ;

		// �e�N�X�`���[���g�p���邩�ǂ����ŏ����𕪊�
		// �g�p���Ȃ��ꍇ�͏����l�̂܂�
		if( ParamFlag & DX_D3D9_DRAWPREP_TEXTURE /* Texture */ )
		{
			// ���ߐF�������s��Ȃ��ꍇ�̓J���[�L�[�����e�X�g�����`�����l�����g�p���Ȃ��̂ŏ����l�̂܂�
			// ���ߐF�������s���ꍇ�̂ݏ���������
			if( ParamFlag & DX_D3D9_DRAWPREP_TRANS )
			{
				int TexAlphaTestFlag = ( ParamFlag & DX_D3D9_DRAWPREP_TEXALPHATEST ) != 0 ? 1 : 0 ;
				int TexAlphaChFlag   = ( ParamFlag & DX_D3D9_DRAWPREP_TEXALPHACH   ) != 0 ? 1 : 0 ;

				// �e�N�X�`���Ƀ��r�b�g�����邩�ǂ����ŏ����𕪊�
				if( TexAlphaTestFlag || TexAlphaChFlag )
				{
					// �e�N�X�`���Ƀ��r�b�g������ꍇ�J���[�L�[�͎g�p���Ȃ�(�̂ŏ����l�̂܂�)
/*
					// ���e�X�g�ł����_���W�̃f�[�^�^�����������_�^�ŁA���e�N�X�`���t�B���^�����O���[�h��
					// ���`��Ԃ������ꍇ�̓��`�����l���Ƃ��Ĉ���
					if( VectorIntFlag == FALSE && GSYS.DrawSetting.DrawMode == DX_DRAWMODE_BILINEAR )
					{
						AlphaChannel = TRUE ;
					}
					else
					{
						// ����ȊO�̏ꍇ�̓e�N�X�`���[�̃t���O�Ɉς˂�
						AlphaTest = Format->AlphaTestFlag ;
						AlphaChannel = Format->AlphaChFlag ;
					}
*/
					// ���e�X�g�ł����_���W�̃f�[�^�^�����������_�^�ŁA���e�N�X�`���t�B���^�����O���[�h��
					// ���`��Ԃł��邩�A�u�����h���[�h�� DX_BLENDMODE_NOBLEND �ȊO�������烿�`�����l���Ƃ��Ĉ���
					if( ( GSYS.DrawSetting.BlendMode != DX_BLENDMODE_NOBLEND && GSYS.DrawSetting.BlendMode != DX_BLENDMODE_DESTCOLOR ) ||
						( ( ParamFlag & DX_D3D9_DRAWPREP_VECTORINT ) == 0 && GSYS.DrawSetting.DrawMode == DX_DRAWMODE_BILINEAR ) )
//					if( VectorIntFlag == FALSE && GSYS.DrawSetting.DrawMode == DX_DRAWMODE_BILINEAR )
					{
						AlphaChannel = TRUE ;
					}
					else
					{
						// ����ȊO�̏ꍇ�̓e�N�X�`���[�̃t���O�Ɉς˂�
						AlphaTest    = TexAlphaTestFlag ;
						AlphaChannel = TexAlphaChFlag ;
					}
				}
			}
		}

		// �f�B�t�[�Y�J���[���g�p���邩�ǂ������Z�b�g
		UseDiffuseRGB   = ( ParamFlag & DX_D3D9_DRAWPREP_DIFFUSERGB )   || ( GSYS.DrawSetting.bDrawBright & 0xffffff ) != 0xffffff || GSYS.DrawSetting.AlwaysDiffuseColorFlag ;
		UseDiffuseAlpha = ( ParamFlag & DX_D3D9_DRAWPREP_DIFFUSEALPHA ) || ( GSYS.DrawSetting.BlendMode != DX_BLENDMODE_NOBLEND || GSYS.DrawSetting.UseNoBlendModeParam /*&& ( GSYS.DrawSetting.BlendMode != DX_BLENDMODE_ALPHA || GSYS.DrawSetting.BlendParam != 255 )*/ ) ;

		// �������ꂽ�p�����[�^���Z�b�g����
		if( UseDiffuseRGB             != GD3D9.Device.DrawSetting.UseDiffuseRGBColor     ) Graphics_D3D9_DrawSetting_SetUseDiffuseRGBColorFlag(   UseDiffuseRGB   ) ;
		if( UseDiffuseAlpha           != GD3D9.Device.DrawSetting.UseDiffuseAlphaColor   ) Graphics_D3D9_DrawSetting_SetUseDiffuseAlphaColorFlag( UseDiffuseAlpha ) ;
//		if( Texture                   != GD3D9.Device.DrawSetting.RenderTexture          ) Graphics_D3D9_DrawSetting_SetTexture(                  Texture         ) ;
		if( GD3D9.Device.DrawSetting.IgnoreGraphColorFlag  != GSYS.DrawSetting.IgnoreGraphColorFlag ) Graphics_D3D9_DrawSetting_SetIgnoreDrawGraphColor( GSYS.DrawSetting.IgnoreGraphColorFlag ) ;
		if( GD3D9.Device.DrawSetting.BlendMode             != GSYS.DrawSetting.BlendMode ||
			GD3D9.Device.DrawSetting.AlphaTestValidFlag    != AlphaTest ||
			GD3D9.Device.DrawSetting.AlphaChannelValidFlag != AlphaChannel               ) Graphics_D3D9_DrawSetting_SetDrawBlendMode( GSYS.DrawSetting.BlendMode, AlphaTest, AlphaChannel ) ;
//		if( GRH.EdgeFontDrawFlag      != EdgeFont                   ) SetEdgeFontDrawFlagHardware( EdgeFont ) ;
		if( GD3D9.Device.DrawSetting.AlphaTestMode         != GSYS.DrawSetting.AlphaTestMode ||
			GD3D9.Device.DrawSetting.AlphaTestParam        != GSYS.DrawSetting.AlphaTestParam       ) Graphics_D3D9_DrawSetting_SetDrawAlphaTest( GSYS.DrawSetting.AlphaTestMode, GSYS.DrawSetting.AlphaTestParam ) ;
		if( GD3D9.Device.DrawSetting.ChangeBlendParamFlag || GD3D9.Device.DrawSetting.ChangeTextureFlag ||
			( UseShader && GD3D9.Device.State.SetNormalPixelShader == FALSE && ( Is3D == FALSE || Specular == FALSE ) ) ||
			( UseShader && GD3D9.Device.State.SetNormalPixelShader == TRUE  && ( Is3D == TRUE  && Specular          ) ) )
		{
			Graphics_D3D9_DeviceState_RefreshBlendState( NULL, 0, 1, Is3D == FALSE || Specular == FALSE ) ;
		}
	}
	else
	{
		if( ( UseShader && GD3D9.Device.State.SetNormalPixelShader == FALSE && ( Is3D == FALSE || Specular == FALSE ) ) ||
			( UseShader && GD3D9.Device.State.SetNormalPixelShader == TRUE  && ( Is3D == TRUE  && Specular          ) ) )
		{
			Graphics_D3D9_DeviceState_RefreshBlendState( NULL, 0, 1, Is3D == FALSE || Specular == FALSE ) ;
		}
	}

	// �V�F�[�f�B���O���[�h�̃Z�b�g
	ShadeMode = ( ParamFlag & DX_D3D9_DRAWPREP_GOURAUDSHADE ) ? D_D3DSHADE_GOURAUD : D_D3DSHADE_FLAT ;

	if( GD3D9.Device.State.ShadeMode      != ShadeMode            ) Graphics_D3D9_DeviceState_SetShadeMode(      ShadeMode           ) ;
	if( GD3D9.Device.State.DrawMode       != GSYS.DrawSetting.DrawMode       ) Graphics_D3D9_DeviceState_SetDrawMode(       GSYS.DrawSetting.DrawMode      ) ;
	if( GD3D9.Device.State.MaxAnisotropy  != GSYS.DrawSetting.MaxAnisotropy  ) Graphics_D3D9_DeviceState_SetMaxAnisotropy(  GSYS.DrawSetting.MaxAnisotropy ) ;
	if( ParamFlag & DX_D3D9_DRAWPREP_CULLING )
	{
		if( GD3D9.Device.State.CullMode   != GSYS.DrawSetting.CullMode       ) Graphics_D3D9_DeviceState_SetCullMode(     GSYS.DrawSetting.CullMode      ) ;
	}
	else
	{
		if( GD3D9.Device.State.CullMode   != DX_CULLING_NONE      ) Graphics_D3D9_DeviceState_SetCullMode(     DX_CULLING_NONE     ) ;
	}
//	if( GRH.TextureTransformMatrixDirectChange     )
	if( GSYS.DrawSetting.MatchHardwareTextureAddressTransformMatrix == FALSE )
	{
		Graphics_D3D9_DeviceState_SetTextureAddressTransformMatrix( GSYS.DrawSetting.TextureAddressTransformUse, &GSYS.DrawSetting.TextureAddressTransformMatrix, -1 ) ;
		GSYS.DrawSetting.MatchHardwareTextureAddressTransformMatrix = TRUE ;
//		GRH.TextureTransformMatrixDirectChange = FALSE ;
		GD3D9.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;
	}
	if( ParamFlag & DX_D3D9_DRAWPREP_TEXADDRESS )
	{
		if( GD3D9.Device.State.TexAddressModeU[ 0 ] != GSYS.DrawSetting.TexAddressModeU[ 0 ] ) Graphics_D3D9_DeviceState_SetTextureAddressU( GSYS.DrawSetting.TexAddressModeU[ 0 ], 0 ) ;
		if( GD3D9.Device.State.TexAddressModeU[ 1 ] != GSYS.DrawSetting.TexAddressModeU[ 1 ] ) Graphics_D3D9_DeviceState_SetTextureAddressU( GSYS.DrawSetting.TexAddressModeU[ 1 ], 1 ) ;
		if( GD3D9.Device.State.TexAddressModeU[ 2 ] != GSYS.DrawSetting.TexAddressModeU[ 2 ] ) Graphics_D3D9_DeviceState_SetTextureAddressU( GSYS.DrawSetting.TexAddressModeU[ 2 ], 2 ) ;
		if( GD3D9.Device.State.TexAddressModeU[ 3 ] != GSYS.DrawSetting.TexAddressModeU[ 3 ] ) Graphics_D3D9_DeviceState_SetTextureAddressU( GSYS.DrawSetting.TexAddressModeU[ 3 ], 3 ) ;
		if( GD3D9.Device.State.TexAddressModeU[ 4 ] != GSYS.DrawSetting.TexAddressModeU[ 4 ] ) Graphics_D3D9_DeviceState_SetTextureAddressU( GSYS.DrawSetting.TexAddressModeU[ 4 ], 4 ) ;
		if( GD3D9.Device.State.TexAddressModeU[ 5 ] != GSYS.DrawSetting.TexAddressModeU[ 5 ] ) Graphics_D3D9_DeviceState_SetTextureAddressU( GSYS.DrawSetting.TexAddressModeU[ 5 ], 5 ) ;
		if( GD3D9.Device.State.TexAddressModeU[ 6 ] != GSYS.DrawSetting.TexAddressModeU[ 6 ] ) Graphics_D3D9_DeviceState_SetTextureAddressU( GSYS.DrawSetting.TexAddressModeU[ 6 ], 6 ) ;
		if( GD3D9.Device.State.TexAddressModeU[ 7 ] != GSYS.DrawSetting.TexAddressModeU[ 7 ] ) Graphics_D3D9_DeviceState_SetTextureAddressU( GSYS.DrawSetting.TexAddressModeU[ 7 ], 7 ) ;

		if( GD3D9.Device.State.TexAddressModeV[ 0 ] != GSYS.DrawSetting.TexAddressModeV[ 0 ] ) Graphics_D3D9_DeviceState_SetTextureAddressV( GSYS.DrawSetting.TexAddressModeV[ 0 ], 0 ) ;
		if( GD3D9.Device.State.TexAddressModeV[ 1 ] != GSYS.DrawSetting.TexAddressModeV[ 1 ] ) Graphics_D3D9_DeviceState_SetTextureAddressV( GSYS.DrawSetting.TexAddressModeV[ 1 ], 1 ) ;
		if( GD3D9.Device.State.TexAddressModeV[ 2 ] != GSYS.DrawSetting.TexAddressModeV[ 2 ] ) Graphics_D3D9_DeviceState_SetTextureAddressV( GSYS.DrawSetting.TexAddressModeV[ 2 ], 2 ) ;
		if( GD3D9.Device.State.TexAddressModeV[ 3 ] != GSYS.DrawSetting.TexAddressModeV[ 3 ] ) Graphics_D3D9_DeviceState_SetTextureAddressV( GSYS.DrawSetting.TexAddressModeV[ 3 ], 3 ) ;
		if( GD3D9.Device.State.TexAddressModeV[ 4 ] != GSYS.DrawSetting.TexAddressModeV[ 4 ] ) Graphics_D3D9_DeviceState_SetTextureAddressV( GSYS.DrawSetting.TexAddressModeV[ 4 ], 4 ) ;
		if( GD3D9.Device.State.TexAddressModeV[ 5 ] != GSYS.DrawSetting.TexAddressModeV[ 5 ] ) Graphics_D3D9_DeviceState_SetTextureAddressV( GSYS.DrawSetting.TexAddressModeV[ 5 ], 5 ) ;
		if( GD3D9.Device.State.TexAddressModeV[ 6 ] != GSYS.DrawSetting.TexAddressModeV[ 6 ] ) Graphics_D3D9_DeviceState_SetTextureAddressV( GSYS.DrawSetting.TexAddressModeV[ 6 ], 6 ) ;
		if( GD3D9.Device.State.TexAddressModeV[ 7 ] != GSYS.DrawSetting.TexAddressModeV[ 7 ] ) Graphics_D3D9_DeviceState_SetTextureAddressV( GSYS.DrawSetting.TexAddressModeV[ 7 ], 7 ) ;
	}
	else
	{
		if( GD3D9.Device.State.TexAddressModeU[ 0 ] != DX_TEXADDRESS_CLAMP || GD3D9.Device.State.TexAddressModeV[ 0 ] != DX_TEXADDRESS_CLAMP ) Graphics_D3D9_DeviceState_SetTextureAddress( DX_TEXADDRESS_CLAMP, 0 ) ;
		if( GD3D9.Device.State.TexAddressModeU[ 1 ] != DX_TEXADDRESS_CLAMP || GD3D9.Device.State.TexAddressModeV[ 1 ] != DX_TEXADDRESS_CLAMP ) Graphics_D3D9_DeviceState_SetTextureAddress( DX_TEXADDRESS_CLAMP, 1 ) ;
		if( GD3D9.Device.State.TexAddressModeU[ 2 ] != DX_TEXADDRESS_CLAMP || GD3D9.Device.State.TexAddressModeV[ 2 ] != DX_TEXADDRESS_CLAMP ) Graphics_D3D9_DeviceState_SetTextureAddress( DX_TEXADDRESS_CLAMP, 2 ) ;
		if( GD3D9.Device.State.TexAddressModeU[ 3 ] != DX_TEXADDRESS_CLAMP || GD3D9.Device.State.TexAddressModeV[ 3 ] != DX_TEXADDRESS_CLAMP ) Graphics_D3D9_DeviceState_SetTextureAddress( DX_TEXADDRESS_CLAMP, 3 ) ;
	}

/*
	Graphics_D3D9_DrawSetting_SetDrawBlendMode( GSYS.DrawSetting.BlendMode, GSYS.DrawSetting.BlendParam,  ) ;

	if( ( Format->AlphaTestFlag || Format->AlphaChFlag ) && ( TransFlag != GRH.AlphaValidFlag ) )
	{
		Graphics_D3D9_DrawSetting_SetDrawBlendMode( DX_BLENDMODE_BLINEALPHA ,
									Format->AlphaTestFlag ? ( TransFlag  ? TRUE : FALSE ) : ( Format->AlphaChFlag ? TRUE : FALSE ) ) ;
	}
*/
	// �p�����[�^��ۑ�
//	GRH.DrawPrepFormat    = Format ;
//	GRH.DrawPrepTexture   = Texture ;
	GD3D9.Device.DrawSetting.DrawPrepParamFlag = ParamFlag ;
}

// �r�M���V�[�����s��
extern void Graphics_D3D9_BeginScene( void )
{
	if( Direct3D9_IsValid() == 0 || GD3D9.Device.DrawInfo.BeginSceneFlag == TRUE ) return ;

	Direct3DDevice9_BeginScene() ;
	GD3D9.Device.DrawInfo.BeginSceneFlag = TRUE ;

//	GRH.DrawPrepFormat    = NULL ;
//	GRH.DrawPrepTexture   = NULL ;
	GD3D9.Device.DrawSetting.DrawPrepParamFlag = 0 ;
}

// �G���h�V�[�����s��
extern void Graphics_D3D9_EndScene( void )
{
	if( Direct3D9_IsValid() == 0 || GD3D9.Device.DrawInfo.BeginSceneFlag == FALSE ) return ;

	Direct3DDevice9_EndScene() ;
	GD3D9.Device.DrawInfo.BeginSceneFlag = FALSE ;
}





























// Direct3D9 ���g�����`��֌W

// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawBillboard3D
extern	int		Graphics_D3D9_DrawBillboard3D( VECTOR Pos, float cx, float cy, float Size, float Angle, IMAGEDATA *Image, IMAGEDATA *BlendImage, int TransFlag, int TurnFlag, int DrawFlag, RECT *DrawArea )
{
	VERTEX_2D *DrawVert ;
	VERTEX_2D TempVert[ 6 ] ;
	VERTEX_3D *DrawVert3D ;
	VERTEX_BLENDTEX_2D *DrawVertB ;
	VERTEX_BLENDTEX_2D TempVertB[ 6 ] ;
	IMAGEDATA_HARD_DRAW *DrawTex ;
	IMAGEDATA_HARD_DRAW *BlendDrawTex ;
	IMAGEDATA_ORIG *Orig ;
	IMAGEDATA_HARD_VERT *TexVert ;
	IMAGEDATA_HARD_VERT *BlendTexVert = NULL ;
	DWORD DiffuseColor ;
	int DrawTexNum ;
	int i ;
	int Flag ;
	int BlendGraphNoIncFlag ;
	float SizeX ;
	float SizeY ;
	float f ;
	VECTOR SrcVec[ 4 ] ;
	VECTOR SrcVec2[ 4 ] ;
	float Sin = 0.0f ;
	float Cos = 1.0f ;
	float ScaleX ;
	float ScaleY ;
	float dleft = 0.0f ;
	float dright = 0.0f ;
	float dtop = 0.0f ;
	float dbottom = 0.0f ;
	float z ;
	float rhw ;
	float u[ 2 ] ;
	float v[ 2 ] ;

	Orig = Image->Orig ;

	if( GAPIWin.Direct3DDevice9Object == NULL ) return -1 ;

	// �`�揀��
	if( DrawFlag )
	{
		if( GD3D9.Device.DrawInfo.BeginSceneFlag == FALSE ) Graphics_D3D9_BeginScene() ;
		Flag = TransFlag | DX_D3D9_DRAWPREP_3D | DX_D3D9_DRAWPREP_FOG | DX_D3D9_DRAWPREP_TEXADDRESS ;
		DX_D3D9_DRAWPREP_TEX( Orig, Image->Hard.Draw[ 0 ].Tex->PF->D3D9.Texture, Flag )
	}
	else
	{
		dleft   = -100000000.0f ;
		dright  =  100000000.0f ;
		dtop    = -100000000.0f ;
		dbottom =  100000000.0f ;
	}

	// ���_�f�[�^���擾
	DiffuseColor = GD3D9.Device.DrawInfo.DiffuseColor ;

	// �`����̐����Z�b�g
	DrawTexNum = Image->Hard.DrawNum ;

	// �`����̐����u�����h�摜�ƈقȂ��Ă�����O�Ԗڂ̃e�N�X�`���������g�p����
	BlendGraphNoIncFlag = FALSE ;
	if( BlendImage != NULL && BlendImage->Hard.DrawNum != Image->Hard.DrawNum )
	{
		BlendGraphNoIncFlag = TRUE ;
	}

	// �`����z��̃A�h���X���Z�b�g���Ă���
	DrawTex = Image->Hard.Draw ;
	BlendDrawTex = NULL ;
	if( BlendImage != NULL )
	{
		BlendDrawTex = BlendImage->Hard.Draw ;
	}

	// �T�C�Y�ƍ��W�֌W�̎��O�v�Z
	SizeX = Size ;
	SizeY = Size * ( float )Image->HeightF / ( float )Image->WidthF ;

	ScaleX = SizeX / Image->WidthF ;
	ScaleY = SizeY / Image->HeightF ;
	cx *= Image->WidthF ;
	cy *= Image->HeightF ;

	// ��]����ꍇ�͉�]�l�����߂Ă���
	if( Angle != 0.0 )
	{
		_SINCOS( (float)Angle, &Sin, &Cos ) ;
	}

	// �e�N�X�`���[�̐������J��Ԃ�
	for( i = 0 ; i < DrawTexNum ; i ++, DrawTex ++ )
	{
		// �`�悷��e�N�X�`���[�̃Z�b�g
		if( DrawFlag )
		{
			Graphics_D3D9_DrawSetting_SetTexture( DrawTex->Tex->PF->D3D9.Texture ) ;

			if( BlendDrawTex != NULL )
			{
				Graphics_D3D9_DrawSetting_SetBlendTexture( BlendDrawTex->Tex->PF->D3D9.Texture, BlendDrawTex->Tex->TexWidth, BlendDrawTex->Tex->TexHeight );
			}

			if( GD3D9.Device.DrawSetting.ChangeTextureFlag )
			{
				Graphics_D3D9_DeviceState_RefreshBlendState() ;
			}
		}

		TexVert = DrawTex->Vertex ;
		if( BlendDrawTex != NULL )
		{
			BlendTexVert = BlendDrawTex->Vertex ;
		}

		// �u�����h�O���t�B�b�N���g�p���Ă��Ȃ��āA���`�悷��ꍇ�͍����ȏ������g�p����
		if( GD3D9.Device.DrawInfo.VertexType != VERTEXTYPE_BLENDTEX && DrawFlag == TRUE )
		{
			GETVERTEX_BILLBOARD( DrawVert3D ) ;

			// ��]����ꍇ�Ƃ��Ȃ��ꍇ�ŏ����𕪊�
			if( Angle != 0.0 )
			{
				// ���[�J�����W����
				SrcVec[2].x = SrcVec[0].x = ( -cx + TexVert[0].x ) * ScaleX ;
				SrcVec[3].x = SrcVec[1].x = ( -cx + TexVert[1].x ) * ScaleX ;

				SrcVec[1].y = SrcVec[0].y = ( -cy + Image->HeightF - TexVert[0].y ) * ScaleY ;
				SrcVec[3].y = SrcVec[2].y = ( -cy + Image->HeightF - TexVert[2].y ) * ScaleY ;

				// ��]�v�Z
				f             = SrcVec[ 0 ].x * Cos - SrcVec[ 0 ].y * Sin ;	
				SrcVec[ 0 ].y = SrcVec[ 0 ].x * Sin + SrcVec[ 0 ].y * Cos ;
				SrcVec[ 0 ].x = f ;

				f             = SrcVec[ 1 ].x * Cos - SrcVec[ 1 ].y * Sin ;	
				SrcVec[ 1 ].y = SrcVec[ 1 ].x * Sin + SrcVec[ 1 ].y * Cos ;
				SrcVec[ 1 ].x = f ;

				f             = SrcVec[ 2 ].x * Cos - SrcVec[ 2 ].y * Sin ;	
				SrcVec[ 2 ].y = SrcVec[ 2 ].x * Sin + SrcVec[ 2 ].y * Cos ;
				SrcVec[ 2 ].x = f ;

				f             = SrcVec[ 3 ].x * Cos - SrcVec[ 3 ].y * Sin ;	
				SrcVec[ 3 ].y = SrcVec[ 3 ].x * Sin + SrcVec[ 3 ].y * Cos ;
				SrcVec[ 3 ].x = f ;

				// �r���{�[�h���W�����[���h���W�֕ϊ�
				DrawVert3D[ 0 ].pos.x = SrcVec[ 0 ].x * GSYS.DrawSetting.BillboardMatrixF.m[0][0] + SrcVec[ 0 ].y * GSYS.DrawSetting.BillboardMatrixF.m[1][0] + Pos.x ;
				DrawVert3D[ 0 ].pos.y = SrcVec[ 0 ].x * GSYS.DrawSetting.BillboardMatrixF.m[0][1] + SrcVec[ 0 ].y * GSYS.DrawSetting.BillboardMatrixF.m[1][1] + Pos.y ;
				DrawVert3D[ 0 ].pos.z = SrcVec[ 0 ].x * GSYS.DrawSetting.BillboardMatrixF.m[0][2] + SrcVec[ 0 ].y * GSYS.DrawSetting.BillboardMatrixF.m[1][2] + Pos.z ;

				DrawVert3D[ 1 ].pos.x = SrcVec[ 1 ].x * GSYS.DrawSetting.BillboardMatrixF.m[0][0] + SrcVec[ 1 ].y * GSYS.DrawSetting.BillboardMatrixF.m[1][0] + Pos.x ;
				DrawVert3D[ 1 ].pos.y = SrcVec[ 1 ].x * GSYS.DrawSetting.BillboardMatrixF.m[0][1] + SrcVec[ 1 ].y * GSYS.DrawSetting.BillboardMatrixF.m[1][1] + Pos.y ;
				DrawVert3D[ 1 ].pos.z = SrcVec[ 1 ].x * GSYS.DrawSetting.BillboardMatrixF.m[0][2] + SrcVec[ 1 ].y * GSYS.DrawSetting.BillboardMatrixF.m[1][2] + Pos.z ;

				DrawVert3D[ 2 ].pos.x = SrcVec[ 2 ].x * GSYS.DrawSetting.BillboardMatrixF.m[0][0] + SrcVec[ 2 ].y * GSYS.DrawSetting.BillboardMatrixF.m[1][0] + Pos.x ;
				DrawVert3D[ 2 ].pos.y = SrcVec[ 2 ].x * GSYS.DrawSetting.BillboardMatrixF.m[0][1] + SrcVec[ 2 ].y * GSYS.DrawSetting.BillboardMatrixF.m[1][1] + Pos.y ;
				DrawVert3D[ 2 ].pos.z = SrcVec[ 2 ].x * GSYS.DrawSetting.BillboardMatrixF.m[0][2] + SrcVec[ 2 ].y * GSYS.DrawSetting.BillboardMatrixF.m[1][2] + Pos.z ;

				DrawVert3D[ 3 ].pos.x = SrcVec[ 3 ].x * GSYS.DrawSetting.BillboardMatrixF.m[0][0] + SrcVec[ 3 ].y * GSYS.DrawSetting.BillboardMatrixF.m[1][0] + Pos.x ;
				DrawVert3D[ 3 ].pos.y = SrcVec[ 3 ].x * GSYS.DrawSetting.BillboardMatrixF.m[0][1] + SrcVec[ 3 ].y * GSYS.DrawSetting.BillboardMatrixF.m[1][1] + Pos.y ;
				DrawVert3D[ 3 ].pos.z = SrcVec[ 3 ].x * GSYS.DrawSetting.BillboardMatrixF.m[0][2] + SrcVec[ 3 ].y * GSYS.DrawSetting.BillboardMatrixF.m[1][2] + Pos.z ;
			}
			else
			{
				VECTOR TempVecX[ 2 ], TempVecY[ 2 ] ;

				// ���[�J�����W����
				SrcVec[0].x = ( -cx + TexVert[0].x ) * ScaleX ;
				SrcVec[1].x = ( -cx + TexVert[1].x ) * ScaleX ;

				SrcVec[0].y = ( -cy + Image->HeightF - TexVert[0].y ) * ScaleY ;
				SrcVec[2].y = ( -cy + Image->HeightF - TexVert[2].y ) * ScaleY ;

				// �r���{�[�h���W�����[���h���W�֕ϊ�
				TempVecX[ 0 ].x = SrcVec[ 0 ].x * GSYS.DrawSetting.BillboardMatrixF.m[0][0] + Pos.x ;
				TempVecX[ 0 ].y = SrcVec[ 0 ].x * GSYS.DrawSetting.BillboardMatrixF.m[0][1] + Pos.y ;
				TempVecX[ 0 ].z = SrcVec[ 0 ].x * GSYS.DrawSetting.BillboardMatrixF.m[0][2] + Pos.z ;

				TempVecX[ 1 ].x = SrcVec[ 1 ].x * GSYS.DrawSetting.BillboardMatrixF.m[0][0] + Pos.x ;
				TempVecX[ 1 ].y = SrcVec[ 1 ].x * GSYS.DrawSetting.BillboardMatrixF.m[0][1] + Pos.y ;
				TempVecX[ 1 ].z = SrcVec[ 1 ].x * GSYS.DrawSetting.BillboardMatrixF.m[0][2] + Pos.z ;

				TempVecY[ 0 ].x = SrcVec[ 0 ].y * GSYS.DrawSetting.BillboardMatrixF.m[1][0] ;
				TempVecY[ 0 ].y = SrcVec[ 0 ].y * GSYS.DrawSetting.BillboardMatrixF.m[1][1] ;
				TempVecY[ 0 ].z = SrcVec[ 0 ].y * GSYS.DrawSetting.BillboardMatrixF.m[1][2] ;

				TempVecY[ 1 ].x = SrcVec[ 2 ].y * GSYS.DrawSetting.BillboardMatrixF.m[1][0] ;
				TempVecY[ 1 ].y = SrcVec[ 2 ].y * GSYS.DrawSetting.BillboardMatrixF.m[1][1] ;
				TempVecY[ 1 ].z = SrcVec[ 2 ].y * GSYS.DrawSetting.BillboardMatrixF.m[1][2] ;

				DrawVert3D[ 0 ].pos.x = TempVecX[ 0 ].x + TempVecY[ 0 ].x ;
				DrawVert3D[ 0 ].pos.y = TempVecX[ 0 ].y + TempVecY[ 0 ].y ;
				DrawVert3D[ 0 ].pos.z = TempVecX[ 0 ].z + TempVecY[ 0 ].z ;

				DrawVert3D[ 1 ].pos.x = TempVecX[ 1 ].x + TempVecY[ 0 ].x ;
				DrawVert3D[ 1 ].pos.y = TempVecX[ 1 ].y + TempVecY[ 0 ].y ;
				DrawVert3D[ 1 ].pos.z = TempVecX[ 1 ].z + TempVecY[ 0 ].z ;

				DrawVert3D[ 2 ].pos.x = TempVecX[ 0 ].x + TempVecY[ 1 ].x ;
				DrawVert3D[ 2 ].pos.y = TempVecX[ 0 ].y + TempVecY[ 1 ].y ;
				DrawVert3D[ 2 ].pos.z = TempVecX[ 0 ].z + TempVecY[ 1 ].z ;

				DrawVert3D[ 3 ].pos.x = TempVecX[ 1 ].x + TempVecY[ 1 ].x ;
				DrawVert3D[ 3 ].pos.y = TempVecX[ 1 ].y + TempVecY[ 1 ].y ;
				DrawVert3D[ 3 ].pos.z = TempVecX[ 1 ].z + TempVecY[ 1 ].z ;
			}

			DrawVert3D[4].pos = DrawVert3D[2].pos ;
			DrawVert3D[5].pos = DrawVert3D[1].pos ;

			*( ( DWORD * )&DrawVert3D[0].b ) = 
			*( ( DWORD * )&DrawVert3D[3].b ) = DiffuseColor ;

			if( TurnFlag )
			{
				DrawVert3D[5].u = DrawVert3D[3].u = DrawVert3D[1].u = TexVert[0].u ;
				DrawVert3D[4].u = DrawVert3D[2].u = DrawVert3D[0].u = TexVert[1].u ;
			}
			else
			{
				DrawVert3D[4].u = DrawVert3D[2].u = DrawVert3D[0].u = TexVert[0].u ;
				DrawVert3D[5].u = DrawVert3D[3].u = DrawVert3D[1].u = TexVert[1].u ;
			}
			DrawVert3D[5].v = DrawVert3D[1].v = DrawVert3D[0].v = TexVert[0].v ;
			DrawVert3D[4].v = DrawVert3D[3].v = DrawVert3D[2].v = TexVert[2].v ;

			// �e�N�X�`���[��`�悷��
			ADD4VERTEX_BILLBOARD
		}
		else
		{
			VECTOR DrawPos[ 4 ] ;

			// �r���{�[�h�̂S���_�𓾂�
			{
				// ��]����ꍇ�Ƃ��Ȃ��ꍇ�ŏ����𕪊�
				if( Angle != 0.0 )
				{
					// ���[�J�����W����
					SrcVec[2].x = SrcVec[0].x = ( -cx + TexVert[0].x ) * ScaleX ;
					SrcVec[3].x = SrcVec[1].x = ( -cx + TexVert[1].x ) * ScaleX ;

					SrcVec[1].y = SrcVec[0].y = ( -cy + Image->HeightF - TexVert[0].y ) * ScaleY ;
					SrcVec[3].y = SrcVec[2].y = ( -cy + Image->HeightF - TexVert[2].y ) * ScaleY ;

					// ��]�v�Z
					f             = SrcVec[ 0 ].x * Cos - SrcVec[ 0 ].y * Sin ;	
					SrcVec[ 0 ].y = SrcVec[ 0 ].x * Sin + SrcVec[ 0 ].y * Cos ;
					SrcVec[ 0 ].x = f ;

					f             = SrcVec[ 1 ].x * Cos - SrcVec[ 1 ].y * Sin ;	
					SrcVec[ 1 ].y = SrcVec[ 1 ].x * Sin + SrcVec[ 1 ].y * Cos ;
					SrcVec[ 1 ].x = f ;

					f             = SrcVec[ 2 ].x * Cos - SrcVec[ 2 ].y * Sin ;	
					SrcVec[ 2 ].y = SrcVec[ 2 ].x * Sin + SrcVec[ 2 ].y * Cos ;
					SrcVec[ 2 ].x = f ;

					f             = SrcVec[ 3 ].x * Cos - SrcVec[ 3 ].y * Sin ;	
					SrcVec[ 3 ].y = SrcVec[ 3 ].x * Sin + SrcVec[ 3 ].y * Cos ;
					SrcVec[ 3 ].x = f ;

					// �r���{�[�h���W�����[���h���W�֕ϊ�
					SrcVec2[ 0 ].x = SrcVec[ 0 ].x * GSYS.DrawSetting.BillboardMatrixF.m[0][0] + SrcVec[ 0 ].y * GSYS.DrawSetting.BillboardMatrixF.m[1][0] + Pos.x ;
					SrcVec2[ 0 ].y = SrcVec[ 0 ].x * GSYS.DrawSetting.BillboardMatrixF.m[0][1] + SrcVec[ 0 ].y * GSYS.DrawSetting.BillboardMatrixF.m[1][1] + Pos.y ;
					SrcVec2[ 0 ].z = SrcVec[ 0 ].x * GSYS.DrawSetting.BillboardMatrixF.m[0][2] + SrcVec[ 0 ].y * GSYS.DrawSetting.BillboardMatrixF.m[1][2] + Pos.z ;

					SrcVec2[ 1 ].x = SrcVec[ 1 ].x * GSYS.DrawSetting.BillboardMatrixF.m[0][0] + SrcVec[ 1 ].y * GSYS.DrawSetting.BillboardMatrixF.m[1][0] + Pos.x ;
					SrcVec2[ 1 ].y = SrcVec[ 1 ].x * GSYS.DrawSetting.BillboardMatrixF.m[0][1] + SrcVec[ 1 ].y * GSYS.DrawSetting.BillboardMatrixF.m[1][1] + Pos.y ;
					SrcVec2[ 1 ].z = SrcVec[ 1 ].x * GSYS.DrawSetting.BillboardMatrixF.m[0][2] + SrcVec[ 1 ].y * GSYS.DrawSetting.BillboardMatrixF.m[1][2] + Pos.z ;

					SrcVec2[ 2 ].x = SrcVec[ 2 ].x * GSYS.DrawSetting.BillboardMatrixF.m[0][0] + SrcVec[ 2 ].y * GSYS.DrawSetting.BillboardMatrixF.m[1][0] + Pos.x ;
					SrcVec2[ 2 ].y = SrcVec[ 2 ].x * GSYS.DrawSetting.BillboardMatrixF.m[0][1] + SrcVec[ 2 ].y * GSYS.DrawSetting.BillboardMatrixF.m[1][1] + Pos.y ;
					SrcVec2[ 2 ].z = SrcVec[ 2 ].x * GSYS.DrawSetting.BillboardMatrixF.m[0][2] + SrcVec[ 2 ].y * GSYS.DrawSetting.BillboardMatrixF.m[1][2] + Pos.z ;

					SrcVec2[ 3 ].x = SrcVec[ 3 ].x * GSYS.DrawSetting.BillboardMatrixF.m[0][0] + SrcVec[ 3 ].y * GSYS.DrawSetting.BillboardMatrixF.m[1][0] + Pos.x ;
					SrcVec2[ 3 ].y = SrcVec[ 3 ].x * GSYS.DrawSetting.BillboardMatrixF.m[0][1] + SrcVec[ 3 ].y * GSYS.DrawSetting.BillboardMatrixF.m[1][1] + Pos.y ;
					SrcVec2[ 3 ].z = SrcVec[ 3 ].x * GSYS.DrawSetting.BillboardMatrixF.m[0][2] + SrcVec[ 3 ].y * GSYS.DrawSetting.BillboardMatrixF.m[1][2] + Pos.z ;
				}
				else
				{
					VECTOR TempVecX[ 2 ], TempVecY[ 2 ] ;

					// ���[�J�����W����
					SrcVec[0].x = ( -cx + TexVert[0].x ) * ScaleX ;
					SrcVec[1].x = ( -cx + TexVert[1].x ) * ScaleX ;

					SrcVec[0].y = ( -cy + Image->HeightF - TexVert[0].y ) * ScaleY ;
					SrcVec[2].y = ( -cy + Image->HeightF - TexVert[2].y ) * ScaleY ;

					// �r���{�[�h���W�����[���h���W�֕ϊ�
					TempVecX[ 0 ].x = SrcVec[ 0 ].x * GSYS.DrawSetting.BillboardMatrixF.m[0][0] + Pos.x ;
					TempVecX[ 0 ].y = SrcVec[ 0 ].x * GSYS.DrawSetting.BillboardMatrixF.m[0][1] + Pos.y ;
					TempVecX[ 0 ].z = SrcVec[ 0 ].x * GSYS.DrawSetting.BillboardMatrixF.m[0][2] + Pos.z ;

					TempVecX[ 1 ].x = SrcVec[ 1 ].x * GSYS.DrawSetting.BillboardMatrixF.m[0][0] + Pos.x ;
					TempVecX[ 1 ].y = SrcVec[ 1 ].x * GSYS.DrawSetting.BillboardMatrixF.m[0][1] + Pos.y ;
					TempVecX[ 1 ].z = SrcVec[ 1 ].x * GSYS.DrawSetting.BillboardMatrixF.m[0][2] + Pos.z ;

					TempVecY[ 0 ].x = SrcVec[ 0 ].y * GSYS.DrawSetting.BillboardMatrixF.m[1][0] ;
					TempVecY[ 0 ].y = SrcVec[ 0 ].y * GSYS.DrawSetting.BillboardMatrixF.m[1][1] ;
					TempVecY[ 0 ].z = SrcVec[ 0 ].y * GSYS.DrawSetting.BillboardMatrixF.m[1][2] ;

					TempVecY[ 1 ].x = SrcVec[ 2 ].y * GSYS.DrawSetting.BillboardMatrixF.m[1][0] ;
					TempVecY[ 1 ].y = SrcVec[ 2 ].y * GSYS.DrawSetting.BillboardMatrixF.m[1][1] ;
					TempVecY[ 1 ].z = SrcVec[ 2 ].y * GSYS.DrawSetting.BillboardMatrixF.m[1][2] ;

					SrcVec2[ 0 ].x = TempVecX[ 0 ].x + TempVecY[ 0 ].x ;
					SrcVec2[ 0 ].y = TempVecX[ 0 ].y + TempVecY[ 0 ].y ;
					SrcVec2[ 0 ].z = TempVecX[ 0 ].z + TempVecY[ 0 ].z ;

					SrcVec2[ 1 ].x = TempVecX[ 1 ].x + TempVecY[ 0 ].x ;
					SrcVec2[ 1 ].y = TempVecX[ 1 ].y + TempVecY[ 0 ].y ;
					SrcVec2[ 1 ].z = TempVecX[ 1 ].z + TempVecY[ 0 ].z ;

					SrcVec2[ 2 ].x = TempVecX[ 0 ].x + TempVecY[ 1 ].x ;
					SrcVec2[ 2 ].y = TempVecX[ 0 ].y + TempVecY[ 1 ].y ;
					SrcVec2[ 2 ].z = TempVecX[ 0 ].z + TempVecY[ 1 ].z ;

					SrcVec2[ 3 ].x = TempVecX[ 1 ].x + TempVecY[ 1 ].x ;
					SrcVec2[ 3 ].y = TempVecX[ 1 ].y + TempVecY[ 1 ].y ;
					SrcVec2[ 3 ].z = TempVecX[ 1 ].z + TempVecY[ 1 ].z ;
				}
			}

			// ���_�^�C�v�ɂ���ď����𕪊�
			switch( GD3D9.Device.DrawInfo.VertexType )
			{
			case VERTEXTYPE_BLENDTEX :
				// �u�����h�e�N�X�`�����g�p����

				// ���_�o�b�t�@�̎擾
				if( DrawFlag )
				{
					GETVERTEX_QUAD( DrawVertB )
				}
				else
				{
					DrawVertB = TempVertB ;
				}

				DrawVertB[0].color	= DiffuseColor ;
				DrawVertB[3].color	= DiffuseColor ;

				// �X�N���[�����W�ɕϊ�
				rhw = 1.0f / ( SrcVec2[0].x * GSYS.DrawSetting.BlendMatrixF.m[0][3] + SrcVec2[0].y * GSYS.DrawSetting.BlendMatrixF.m[1][3] + SrcVec2[0].z * GSYS.DrawSetting.BlendMatrixF.m[2][3] + GSYS.DrawSetting.BlendMatrixF.m[3][3] ) ;
				z   = rhw  * ( SrcVec2[0].x * GSYS.DrawSetting.BlendMatrixF.m[0][2] + SrcVec2[0].y * GSYS.DrawSetting.BlendMatrixF.m[1][2] + SrcVec2[0].z * GSYS.DrawSetting.BlendMatrixF.m[2][2] + GSYS.DrawSetting.BlendMatrixF.m[3][2] ) ;

				DrawPos[ 0 ].x = ( SrcVec2[0].x * GSYS.DrawSetting.BlendMatrixF.m[0][0] + SrcVec2[0].y * GSYS.DrawSetting.BlendMatrixF.m[1][0] + SrcVec2[0].z * GSYS.DrawSetting.BlendMatrixF.m[2][0] + GSYS.DrawSetting.BlendMatrixF.m[3][0] ) * rhw ;
				DrawPos[ 0 ].y = ( SrcVec2[0].x * GSYS.DrawSetting.BlendMatrixF.m[0][1] + SrcVec2[0].y * GSYS.DrawSetting.BlendMatrixF.m[1][1] + SrcVec2[0].z * GSYS.DrawSetting.BlendMatrixF.m[2][1] + GSYS.DrawSetting.BlendMatrixF.m[3][1] ) * rhw ;
				DrawPos[ 1 ].x = ( SrcVec2[1].x * GSYS.DrawSetting.BlendMatrixF.m[0][0] + SrcVec2[1].y * GSYS.DrawSetting.BlendMatrixF.m[1][0] + SrcVec2[1].z * GSYS.DrawSetting.BlendMatrixF.m[2][0] + GSYS.DrawSetting.BlendMatrixF.m[3][0] ) * rhw ;
				DrawPos[ 1 ].y = ( SrcVec2[1].x * GSYS.DrawSetting.BlendMatrixF.m[0][1] + SrcVec2[1].y * GSYS.DrawSetting.BlendMatrixF.m[1][1] + SrcVec2[1].z * GSYS.DrawSetting.BlendMatrixF.m[2][1] + GSYS.DrawSetting.BlendMatrixF.m[3][1] ) * rhw ;
				DrawPos[ 2 ].x = ( SrcVec2[2].x * GSYS.DrawSetting.BlendMatrixF.m[0][0] + SrcVec2[2].y * GSYS.DrawSetting.BlendMatrixF.m[1][0] + SrcVec2[2].z * GSYS.DrawSetting.BlendMatrixF.m[2][0] + GSYS.DrawSetting.BlendMatrixF.m[3][0] ) * rhw ;
				DrawPos[ 2 ].y = ( SrcVec2[2].x * GSYS.DrawSetting.BlendMatrixF.m[0][1] + SrcVec2[2].y * GSYS.DrawSetting.BlendMatrixF.m[1][1] + SrcVec2[2].z * GSYS.DrawSetting.BlendMatrixF.m[2][1] + GSYS.DrawSetting.BlendMatrixF.m[3][1] ) * rhw ;
				DrawPos[ 3 ].x = ( SrcVec2[3].x * GSYS.DrawSetting.BlendMatrixF.m[0][0] + SrcVec2[3].y * GSYS.DrawSetting.BlendMatrixF.m[1][0] + SrcVec2[3].z * GSYS.DrawSetting.BlendMatrixF.m[2][0] + GSYS.DrawSetting.BlendMatrixF.m[3][0] ) * rhw ;
				DrawPos[ 3 ].y = ( SrcVec2[3].x * GSYS.DrawSetting.BlendMatrixF.m[0][1] + SrcVec2[3].y * GSYS.DrawSetting.BlendMatrixF.m[1][1] + SrcVec2[3].z * GSYS.DrawSetting.BlendMatrixF.m[2][1] + GSYS.DrawSetting.BlendMatrixF.m[3][1] ) * rhw ;

				DrawVertB[0].pos.x = DrawPos[ 0 ].x ;
				DrawVertB[0].pos.y = DrawPos[ 0 ].y ;
				DrawVertB[1].pos.x = DrawPos[ 1 ].x ;
				DrawVertB[1].pos.y = DrawPos[ 1 ].y ;
				DrawVertB[2].pos.x = DrawPos[ 2 ].x ;
				DrawVertB[2].pos.y = DrawPos[ 2 ].y ;
				DrawVertB[3].pos.x = DrawPos[ 3 ].x ;
				DrawVertB[3].pos.y = DrawPos[ 3 ].y ;
				DrawVertB[4].pos.x = DrawPos[ 2 ].x ;
				DrawVertB[4].pos.y = DrawPos[ 2 ].y ;
				DrawVertB[5].pos.x = DrawPos[ 1 ].x ;
				DrawVertB[5].pos.y = DrawPos[ 1 ].y ;

				switch( GSYS.DrawSetting.BlendGraphPosMode )
				{
				default :
				case DX_BLENDGRAPH_POSMODE_DRAWGRAPH:
					u[ 0 ] = ( TexVert[0].x - BlendTexVert[0].x + GSYS.DrawSetting.BlendGraphX ) * GD3D9.Device.DrawSetting.InvBlendTextureWidth ;
					u[ 1 ] = ( TexVert[1].x - BlendTexVert[0].x + GSYS.DrawSetting.BlendGraphX ) * GD3D9.Device.DrawSetting.InvBlendTextureWidth ;
					v[ 0 ] = ( TexVert[0].y - BlendTexVert[0].y + GSYS.DrawSetting.BlendGraphY ) * GD3D9.Device.DrawSetting.InvBlendTextureHeight ;
					v[ 1 ] = ( TexVert[2].y - BlendTexVert[0].y + GSYS.DrawSetting.BlendGraphY ) * GD3D9.Device.DrawSetting.InvBlendTextureHeight ;
					DrawVertB[0].u1 = u[ 0 ] ;
					DrawVertB[0].v1 = v[ 0 ] ;
					DrawVertB[1].u1 = u[ 1 ] ;
					DrawVertB[1].v1 = v[ 0 ] ;
					DrawVertB[2].u1 = u[ 0 ] ;
					DrawVertB[2].v1 = v[ 1 ] ;
					DrawVertB[3].u1 = u[ 1 ] ;
					DrawVertB[3].v1 = v[ 1 ] ;
					DrawVertB[4].u1 = u[ 0 ] ;
					DrawVertB[4].v1 = v[ 1 ] ;
					DrawVertB[5].u1 = u[ 1 ] ;
					DrawVertB[5].v1 = v[ 0 ] ;
					break ;

				case DX_BLENDGRAPH_POSMODE_SCREEN :
					DrawVertB[0].u1 = ( DrawPos[ 0 ].x - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D9.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[0].v1 = ( DrawPos[ 0 ].y - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D9.Device.DrawSetting.InvBlendTextureHeight ;
					DrawVertB[1].u1 = ( DrawPos[ 1 ].x - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D9.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[1].v1 = ( DrawPos[ 1 ].y - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D9.Device.DrawSetting.InvBlendTextureHeight ;
					DrawVertB[2].u1 = ( DrawPos[ 2 ].x - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D9.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[2].v1 = ( DrawPos[ 2 ].y - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D9.Device.DrawSetting.InvBlendTextureHeight ;
					DrawVertB[3].u1 = ( DrawPos[ 3 ].x - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D9.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[3].v1 = ( DrawPos[ 3 ].y - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D9.Device.DrawSetting.InvBlendTextureHeight ;
					DrawVertB[4].u1 = ( DrawPos[ 2 ].x - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D9.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[4].v1 = ( DrawPos[ 2 ].y - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D9.Device.DrawSetting.InvBlendTextureHeight ;
					DrawVertB[5].u1 = ( DrawPos[ 1 ].x - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D9.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[5].v1 = ( DrawPos[ 1 ].y - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D9.Device.DrawSetting.InvBlendTextureHeight ;
					break ;
				}

				if( TurnFlag )
				{
					DrawVertB[0].u2 = TexVert[1].u ;
					DrawVertB[1].u2 = TexVert[0].u ;
					DrawVertB[2].u2 = TexVert[1].u ;
					DrawVertB[3].u2 = TexVert[0].u ;
					DrawVertB[4].u2 = TexVert[1].u ;
					DrawVertB[5].u2 = TexVert[0].u ;
				}
				else
				{
					DrawVertB[0].u2 = TexVert[0].u ;
					DrawVertB[1].u2 = TexVert[1].u ;
					DrawVertB[2].u2 = TexVert[0].u ;
					DrawVertB[3].u2 = TexVert[1].u ;
					DrawVertB[4].u2 = TexVert[0].u ;
					DrawVertB[5].u2 = TexVert[1].u ;
				}
				DrawVertB[0].v2 = TexVert[0].v ;
				DrawVertB[1].v2 = TexVert[0].v ;
				DrawVertB[2].v2 = TexVert[2].v ;
				DrawVertB[3].v2 = TexVert[2].v ;
				DrawVertB[4].v2 = TexVert[2].v ;
				DrawVertB[5].v2 = TexVert[0].v ;

				DrawVertB[0].pos.z = z ;
				DrawVertB[1].pos.z = z ;
				DrawVertB[2].pos.z = z ;
				DrawVertB[3].pos.z = z ;
				DrawVertB[4].pos.z = z ;
				DrawVertB[5].pos.z = z ;

				DrawVertB[0].rhw = rhw ;
				DrawVertB[1].rhw = rhw ;
				DrawVertB[2].rhw = rhw ;
				DrawVertB[3].rhw = rhw ;
				DrawVertB[4].rhw = rhw ;
				DrawVertB[5].rhw = rhw ;

				// �e�N�X�`���[��`�悷��
				if( DrawFlag )
				{
					ADD4VERTEX_BLENDTEX
				}
				else
				{
					if( dright  > DrawVertB[ 0 ].pos.x ) dright  = DrawVertB[ 0 ].pos.x ;
					if( dright  > DrawVertB[ 1 ].pos.x ) dright  = DrawVertB[ 1 ].pos.x ;
					if( dright  > DrawVertB[ 2 ].pos.x ) dright  = DrawVertB[ 2 ].pos.x ;
					if( dright  > DrawVertB[ 3 ].pos.x ) dright  = DrawVertB[ 3 ].pos.x ;

					if( dleft   < DrawVertB[ 0 ].pos.x ) dleft   = DrawVertB[ 0 ].pos.x ;
					if( dleft   < DrawVertB[ 1 ].pos.x ) dleft   = DrawVertB[ 1 ].pos.x ;
					if( dleft   < DrawVertB[ 2 ].pos.x ) dleft   = DrawVertB[ 2 ].pos.x ;
					if( dleft   < DrawVertB[ 3 ].pos.x ) dleft   = DrawVertB[ 3 ].pos.x ;

					if( dbottom > DrawVertB[ 0 ].pos.y ) dbottom = DrawVertB[ 0 ].pos.y ;
					if( dbottom > DrawVertB[ 1 ].pos.y ) dbottom = DrawVertB[ 1 ].pos.y ;
					if( dbottom > DrawVertB[ 2 ].pos.y ) dbottom = DrawVertB[ 2 ].pos.y ;
					if( dbottom > DrawVertB[ 3 ].pos.y ) dbottom = DrawVertB[ 3 ].pos.y ;

					if( dtop    < DrawVertB[ 0 ].pos.y ) dtop    = DrawVertB[ 0 ].pos.y ;
					if( dtop    < DrawVertB[ 1 ].pos.y ) dtop    = DrawVertB[ 1 ].pos.y ;
					if( dtop    < DrawVertB[ 2 ].pos.y ) dtop    = DrawVertB[ 2 ].pos.y ;
					if( dtop    < DrawVertB[ 3 ].pos.y ) dtop    = DrawVertB[ 3 ].pos.y ;
				}

				if( BlendGraphNoIncFlag == FALSE )
				{
					BlendDrawTex ++ ;
				}
				break ;

			case VERTEXTYPE_TEX :
				// �u�����h�e�N�X�`�����g�p���Ȃ�

				// ���_�o�b�t�@�̎擾
				if( DrawFlag )
				{
					GETVERTEX_QUAD( DrawVert )
				}
				else
				{
					DrawVert = TempVert;
				}

				// �X�N���[�����W�ɕϊ�
				rhw = 1.0f / ( SrcVec2[0].x * GSYS.DrawSetting.BlendMatrixF.m[0][3] + SrcVec2[0].y * GSYS.DrawSetting.BlendMatrixF.m[1][3] + SrcVec2[0].z * GSYS.DrawSetting.BlendMatrixF.m[2][3] + GSYS.DrawSetting.BlendMatrixF.m[3][3] ) ;
				z   = rhw  * ( SrcVec2[0].x * GSYS.DrawSetting.BlendMatrixF.m[0][2] + SrcVec2[0].y * GSYS.DrawSetting.BlendMatrixF.m[1][2] + SrcVec2[0].z * GSYS.DrawSetting.BlendMatrixF.m[2][2] + GSYS.DrawSetting.BlendMatrixF.m[3][2] ) ;

				DrawVert[0].pos.x = ( SrcVec2[0].x * GSYS.DrawSetting.BlendMatrixF.m[0][0] + SrcVec2[0].y * GSYS.DrawSetting.BlendMatrixF.m[1][0] + SrcVec2[0].z * GSYS.DrawSetting.BlendMatrixF.m[2][0] + GSYS.DrawSetting.BlendMatrixF.m[3][0] ) * rhw ;
				DrawVert[0].pos.y = ( SrcVec2[0].x * GSYS.DrawSetting.BlendMatrixF.m[0][1] + SrcVec2[0].y * GSYS.DrawSetting.BlendMatrixF.m[1][1] + SrcVec2[0].z * GSYS.DrawSetting.BlendMatrixF.m[2][1] + GSYS.DrawSetting.BlendMatrixF.m[3][1] ) * rhw ;
				DrawVert[1].pos.x = ( SrcVec2[1].x * GSYS.DrawSetting.BlendMatrixF.m[0][0] + SrcVec2[1].y * GSYS.DrawSetting.BlendMatrixF.m[1][0] + SrcVec2[1].z * GSYS.DrawSetting.BlendMatrixF.m[2][0] + GSYS.DrawSetting.BlendMatrixF.m[3][0] ) * rhw ;
				DrawVert[1].pos.y = ( SrcVec2[1].x * GSYS.DrawSetting.BlendMatrixF.m[0][1] + SrcVec2[1].y * GSYS.DrawSetting.BlendMatrixF.m[1][1] + SrcVec2[1].z * GSYS.DrawSetting.BlendMatrixF.m[2][1] + GSYS.DrawSetting.BlendMatrixF.m[3][1] ) * rhw ;
				DrawVert[2].pos.x = ( SrcVec2[2].x * GSYS.DrawSetting.BlendMatrixF.m[0][0] + SrcVec2[2].y * GSYS.DrawSetting.BlendMatrixF.m[1][0] + SrcVec2[2].z * GSYS.DrawSetting.BlendMatrixF.m[2][0] + GSYS.DrawSetting.BlendMatrixF.m[3][0] ) * rhw ;
				DrawVert[2].pos.y = ( SrcVec2[2].x * GSYS.DrawSetting.BlendMatrixF.m[0][1] + SrcVec2[2].y * GSYS.DrawSetting.BlendMatrixF.m[1][1] + SrcVec2[2].z * GSYS.DrawSetting.BlendMatrixF.m[2][1] + GSYS.DrawSetting.BlendMatrixF.m[3][1] ) * rhw ;
				DrawVert[3].pos.x = ( SrcVec2[3].x * GSYS.DrawSetting.BlendMatrixF.m[0][0] + SrcVec2[3].y * GSYS.DrawSetting.BlendMatrixF.m[1][0] + SrcVec2[3].z * GSYS.DrawSetting.BlendMatrixF.m[2][0] + GSYS.DrawSetting.BlendMatrixF.m[3][0] ) * rhw ;
				DrawVert[3].pos.y = ( SrcVec2[3].x * GSYS.DrawSetting.BlendMatrixF.m[0][1] + SrcVec2[3].y * GSYS.DrawSetting.BlendMatrixF.m[1][1] + SrcVec2[3].z * GSYS.DrawSetting.BlendMatrixF.m[2][1] + GSYS.DrawSetting.BlendMatrixF.m[3][1] ) * rhw ;
				DrawVert[4].pos.x = ( SrcVec2[2].x * GSYS.DrawSetting.BlendMatrixF.m[0][0] + SrcVec2[2].y * GSYS.DrawSetting.BlendMatrixF.m[1][0] + SrcVec2[2].z * GSYS.DrawSetting.BlendMatrixF.m[2][0] + GSYS.DrawSetting.BlendMatrixF.m[3][0] ) * rhw ;
				DrawVert[4].pos.y = ( SrcVec2[2].x * GSYS.DrawSetting.BlendMatrixF.m[0][1] + SrcVec2[2].y * GSYS.DrawSetting.BlendMatrixF.m[1][1] + SrcVec2[2].z * GSYS.DrawSetting.BlendMatrixF.m[2][1] + GSYS.DrawSetting.BlendMatrixF.m[3][1] ) * rhw ;
				DrawVert[5].pos.x = ( SrcVec2[1].x * GSYS.DrawSetting.BlendMatrixF.m[0][0] + SrcVec2[1].y * GSYS.DrawSetting.BlendMatrixF.m[1][0] + SrcVec2[1].z * GSYS.DrawSetting.BlendMatrixF.m[2][0] + GSYS.DrawSetting.BlendMatrixF.m[3][0] ) * rhw ;
				DrawVert[5].pos.y = ( SrcVec2[1].x * GSYS.DrawSetting.BlendMatrixF.m[0][1] + SrcVec2[1].y * GSYS.DrawSetting.BlendMatrixF.m[1][1] + SrcVec2[1].z * GSYS.DrawSetting.BlendMatrixF.m[2][1] + GSYS.DrawSetting.BlendMatrixF.m[3][1] ) * rhw ;

				DrawVert[0].pos.z = z ;
				DrawVert[1].pos.z = z ;
				DrawVert[2].pos.z = z ;
				DrawVert[3].pos.z = z ;
				DrawVert[4].pos.z = z ;
				DrawVert[5].pos.z = z ;

				DrawVert[0].rhw = rhw ;
				DrawVert[1].rhw = rhw ;
				DrawVert[2].rhw = rhw ;
				DrawVert[3].rhw = rhw ;
				DrawVert[4].rhw = rhw ;
				DrawVert[5].rhw = rhw ;

				DrawVert[0].color = DiffuseColor ;
				DrawVert[1].color = DiffuseColor ;
				DrawVert[2].color = DiffuseColor ;
				DrawVert[3].color = DiffuseColor ;
				DrawVert[4].color = DiffuseColor ;
				DrawVert[5].color = DiffuseColor ;

				if( TurnFlag )
				{
					DrawVert[0].u = TexVert[1].u ;
					DrawVert[1].u = TexVert[0].u ;
					DrawVert[2].u = TexVert[1].u ;
					DrawVert[3].u = TexVert[0].u ;
					DrawVert[4].u = TexVert[1].u ;
					DrawVert[5].u = TexVert[0].u ;
				}
				else
				{
					DrawVert[0].u = TexVert[0].u ;
					DrawVert[1].u = TexVert[1].u ;
					DrawVert[2].u = TexVert[0].u ;
					DrawVert[3].u = TexVert[1].u ;
					DrawVert[4].u = TexVert[0].u ;
					DrawVert[5].u = TexVert[1].u ;
				}
				DrawVert[0].v = TexVert[0].v ;
				DrawVert[1].v = TexVert[0].v ;
				DrawVert[2].v = TexVert[2].v ;
				DrawVert[3].v = TexVert[2].v ;
				DrawVert[4].v = TexVert[2].v ;
				DrawVert[5].v = TexVert[0].v ;

				// �e�N�X�`���[��`�悷��
				if( DrawFlag )
				{
					ADD4VERTEX_TEX
				}
				else
				{
					if( dright  > DrawVert[ 0 ].pos.x ) dright  = DrawVert[ 0 ].pos.x ;
					if( dright  > DrawVert[ 1 ].pos.x ) dright  = DrawVert[ 1 ].pos.x ;
					if( dright  > DrawVert[ 2 ].pos.x ) dright  = DrawVert[ 2 ].pos.x ;
					if( dright  > DrawVert[ 3 ].pos.x ) dright  = DrawVert[ 3 ].pos.x ;

					if( dleft   < DrawVert[ 0 ].pos.x ) dleft   = DrawVert[ 0 ].pos.x ;
					if( dleft   < DrawVert[ 1 ].pos.x ) dleft   = DrawVert[ 1 ].pos.x ;
					if( dleft   < DrawVert[ 2 ].pos.x ) dleft   = DrawVert[ 2 ].pos.x ;
					if( dleft   < DrawVert[ 3 ].pos.x ) dleft   = DrawVert[ 3 ].pos.x ;

					if( dbottom > DrawVert[ 0 ].pos.y ) dbottom = DrawVert[ 0 ].pos.y ;
					if( dbottom > DrawVert[ 1 ].pos.y ) dbottom = DrawVert[ 1 ].pos.y ;
					if( dbottom > DrawVert[ 2 ].pos.y ) dbottom = DrawVert[ 2 ].pos.y ;
					if( dbottom > DrawVert[ 3 ].pos.y ) dbottom = DrawVert[ 3 ].pos.y ;

					if( dtop    < DrawVert[ 0 ].pos.y ) dtop    = DrawVert[ 0 ].pos.y ;
					if( dtop    < DrawVert[ 1 ].pos.y ) dtop    = DrawVert[ 1 ].pos.y ;
					if( dtop    < DrawVert[ 2 ].pos.y ) dtop    = DrawVert[ 2 ].pos.y ;
					if( dtop    < DrawVert[ 3 ].pos.y ) dtop    = DrawVert[ 3 ].pos.y ;
				}
				break ;
			}
		}
	}

	if( DrawFlag == FALSE )
	{
		DrawArea->left   = _FTOL( dleft   ) - 1 ;
		DrawArea->right  = _FTOL( dright  ) + 1 ;
		DrawArea->top    = _FTOL( dtop    ) - 1 ;
		DrawArea->bottom = _FTOL( dbottom ) + 1 ;
	}

	// �I��
	return 0 ;
}

// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawModiBillboard3D
extern	int		Graphics_D3D9_DrawModiBillboard3D( VECTOR Pos, float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, IMAGEDATA *Image, IMAGEDATA *BlendImage, int TransFlag, int DrawFlag, RECT *DrawArea )
{
	VERTEX_2D *DrawVert ;
	VERTEX_3D *DrawVert3D ;
	VERTEX_BLENDTEX_2D *DrawVertB ;
	IMAGEDATA_HARD_DRAW *DrawTex ;
	IMAGEDATA_HARD_DRAW *BlendDrawTex ;
	IMAGEDATA_ORIG *Orig ;
	IMAGEDATA_HARD_VERT *TexVert ;
	IMAGEDATA_HARD_VERT *BlendTexVert = NULL ;
	DWORD DiffuseColor ;
	int DrawTexNum ;
	int i, Flag ;
	int BlendGraphNoIncFlag ;
	VECTOR SrcVec2[ 4 ] ;
	float dleft = 0.0f ;
	float dright = 0.0f ;
	float dtop = 0.0f ;
	float dbottom = 0.0f ;
	float z ;
	float rhw ;
	float u[ 2 ] ;
	float v[ 2 ] ;

	Orig = Image->Orig ;

	if( GAPIWin.Direct3DDevice9Object == NULL ) return -1 ;

	// �`�揀��
	if( DrawFlag )
	{
		if( GD3D9.Device.DrawInfo.BeginSceneFlag == FALSE ) Graphics_D3D9_BeginScene() ;
		Flag = TransFlag | DX_D3D9_DRAWPREP_3D | DX_D3D9_DRAWPREP_FOG | DX_D3D9_DRAWPREP_TEXADDRESS ;
		DX_D3D9_DRAWPREP_TEX( Orig, Image->Hard.Draw[ 0 ].Tex->PF->D3D9.Texture, Flag )
	}
	else
	{
		dleft   = -100000000.0f ;
		dright  =  100000000.0f ;
		dtop    = -100000000.0f ;
		dbottom =  100000000.0f ;
	}

	// ���_�f�[�^���擾
	DiffuseColor = GD3D9.Device.DrawInfo.DiffuseColor ;

	// �`����̐����Z�b�g
	DrawTexNum = Image->Hard.DrawNum ;

	// �`����̐����u�����h�摜�ƈقȂ��Ă�����O�Ԗڂ̃e�N�X�`���������g�p����
	BlendGraphNoIncFlag = FALSE ;
	if( BlendImage != NULL && BlendImage->Hard.DrawNum != Image->Hard.DrawNum )
	{
		BlendGraphNoIncFlag = TRUE ;
	}

	// �`����z��̃A�h���X���Z�b�g���Ă���
	DrawTex = Image->Hard.Draw ;
	BlendDrawTex = NULL ;
	if( BlendImage != NULL )
	{
		BlendDrawTex = BlendImage->Hard.Draw ;
	}

	// �e�N�X�`���[�̐������J��Ԃ�
	for( i = 0 ; i < DrawTexNum ; i ++, DrawTex ++ )
	{
		// �`�悷��e�N�X�`���[�̃Z�b�g
		if( DrawFlag )
		{
			Graphics_D3D9_DrawSetting_SetTexture( DrawTex->Tex->PF->D3D9.Texture ) ;

			if( BlendDrawTex != NULL )
			{
				Graphics_D3D9_DrawSetting_SetBlendTexture( BlendDrawTex->Tex->PF->D3D9.Texture, BlendDrawTex->Tex->TexWidth, BlendDrawTex->Tex->TexHeight );
			}

			if( GD3D9.Device.DrawSetting.ChangeTextureFlag )
			{
				Graphics_D3D9_DeviceState_RefreshBlendState() ;
			}
		}

		TexVert = DrawTex->Vertex ;
		if( BlendDrawTex != NULL )
		{
			BlendTexVert = BlendDrawTex->Vertex ;
		}

		// �u�����h�O���t�B�b�N���g�p���Ă��Ȃ��āA���`�悷��ꍇ�͍����ȏ������g�p����
		if( GD3D9.Device.DrawInfo.VertexType != VERTEXTYPE_BLENDTEX && DrawFlag == TRUE )
		{
			GETVERTEX_BILLBOARD( DrawVert3D ) ;

			// �r���{�[�h���W�����[���h���W�֕ϊ�
			DrawVert3D[ 0 ].pos.x = x1 * GSYS.DrawSetting.BillboardMatrixF.m[0][0] + y1 * GSYS.DrawSetting.BillboardMatrixF.m[1][0] + Pos.x ;
			DrawVert3D[ 0 ].pos.y = x1 * GSYS.DrawSetting.BillboardMatrixF.m[0][1] + y1 * GSYS.DrawSetting.BillboardMatrixF.m[1][1] + Pos.y ;
			DrawVert3D[ 0 ].pos.z = x1 * GSYS.DrawSetting.BillboardMatrixF.m[0][2] + y1 * GSYS.DrawSetting.BillboardMatrixF.m[1][2] + Pos.z ;

			DrawVert3D[ 1 ].pos.x = x2 * GSYS.DrawSetting.BillboardMatrixF.m[0][0] + y2 * GSYS.DrawSetting.BillboardMatrixF.m[1][0] + Pos.x ;
			DrawVert3D[ 1 ].pos.y = x2 * GSYS.DrawSetting.BillboardMatrixF.m[0][1] + y2 * GSYS.DrawSetting.BillboardMatrixF.m[1][1] + Pos.y ;
			DrawVert3D[ 1 ].pos.z = x2 * GSYS.DrawSetting.BillboardMatrixF.m[0][2] + y2 * GSYS.DrawSetting.BillboardMatrixF.m[1][2] + Pos.z ;

			DrawVert3D[ 2 ].pos.x = x4 * GSYS.DrawSetting.BillboardMatrixF.m[0][0] + y4 * GSYS.DrawSetting.BillboardMatrixF.m[1][0] + Pos.x ;
			DrawVert3D[ 2 ].pos.y = x4 * GSYS.DrawSetting.BillboardMatrixF.m[0][1] + y4 * GSYS.DrawSetting.BillboardMatrixF.m[1][1] + Pos.y ;
			DrawVert3D[ 2 ].pos.z = x4 * GSYS.DrawSetting.BillboardMatrixF.m[0][2] + y4 * GSYS.DrawSetting.BillboardMatrixF.m[1][2] + Pos.z ;

			DrawVert3D[ 3 ].pos.x = x3 * GSYS.DrawSetting.BillboardMatrixF.m[0][0] + y3 * GSYS.DrawSetting.BillboardMatrixF.m[1][0] + Pos.x ;
			DrawVert3D[ 3 ].pos.y = x3 * GSYS.DrawSetting.BillboardMatrixF.m[0][1] + y3 * GSYS.DrawSetting.BillboardMatrixF.m[1][1] + Pos.y ;
			DrawVert3D[ 3 ].pos.z = x3 * GSYS.DrawSetting.BillboardMatrixF.m[0][2] + y3 * GSYS.DrawSetting.BillboardMatrixF.m[1][2] + Pos.z ;

			DrawVert3D[4].pos = DrawVert3D[2].pos ;
			DrawVert3D[5].pos = DrawVert3D[1].pos ;

			*( ( DWORD * )&DrawVert3D[0].b ) = 
			*( ( DWORD * )&DrawVert3D[3].b )	= DiffuseColor ;

			DrawVert3D[4].u       = DrawVert3D[2].u       = DrawVert3D[0].u       = TexVert[0].u ;
			DrawVert3D[5].u       = DrawVert3D[3].u       = DrawVert3D[1].u       = TexVert[1].u ;
			DrawVert3D[5].v       = DrawVert3D[1].v       = DrawVert3D[0].v       = TexVert[0].v ;
			DrawVert3D[4].v       = DrawVert3D[3].v       = DrawVert3D[2].v       = TexVert[2].v ;

			// �e�N�X�`���[��`�悷��
			ADD4VERTEX_BILLBOARD
		}
		else
		{
			VECTOR DrawPos[ 4 ] ;

			// �r���{�[�h�̂S���_�𓾂�
			{
				// �r���{�[�h���W�����[���h���W�֕ϊ�
				SrcVec2[ 0 ].x = x1 * GSYS.DrawSetting.BillboardMatrixF.m[0][0] + y1 * GSYS.DrawSetting.BillboardMatrixF.m[1][0] + Pos.x ;
				SrcVec2[ 0 ].y = x1 * GSYS.DrawSetting.BillboardMatrixF.m[0][1] + y1 * GSYS.DrawSetting.BillboardMatrixF.m[1][1] + Pos.y ;
				SrcVec2[ 0 ].z = x1 * GSYS.DrawSetting.BillboardMatrixF.m[0][2] + y1 * GSYS.DrawSetting.BillboardMatrixF.m[1][2] + Pos.z ;

				SrcVec2[ 1 ].x = x2 * GSYS.DrawSetting.BillboardMatrixF.m[0][0] + y2 * GSYS.DrawSetting.BillboardMatrixF.m[1][0] + Pos.x ;
				SrcVec2[ 1 ].y = x2 * GSYS.DrawSetting.BillboardMatrixF.m[0][1] + y2 * GSYS.DrawSetting.BillboardMatrixF.m[1][1] + Pos.y ;
				SrcVec2[ 1 ].z = x2 * GSYS.DrawSetting.BillboardMatrixF.m[0][2] + y2 * GSYS.DrawSetting.BillboardMatrixF.m[1][2] + Pos.z ;

				SrcVec2[ 2 ].x = x4 * GSYS.DrawSetting.BillboardMatrixF.m[0][0] + y4 * GSYS.DrawSetting.BillboardMatrixF.m[1][0] + Pos.x ;
				SrcVec2[ 2 ].y = x4 * GSYS.DrawSetting.BillboardMatrixF.m[0][1] + y4 * GSYS.DrawSetting.BillboardMatrixF.m[1][1] + Pos.y ;
				SrcVec2[ 2 ].z = x4 * GSYS.DrawSetting.BillboardMatrixF.m[0][2] + y4 * GSYS.DrawSetting.BillboardMatrixF.m[1][2] + Pos.z ;

				SrcVec2[ 3 ].x = x3 * GSYS.DrawSetting.BillboardMatrixF.m[0][0] + y3 * GSYS.DrawSetting.BillboardMatrixF.m[1][0] + Pos.x ;
				SrcVec2[ 3 ].y = x3 * GSYS.DrawSetting.BillboardMatrixF.m[0][1] + y3 * GSYS.DrawSetting.BillboardMatrixF.m[1][1] + Pos.y ;
				SrcVec2[ 3 ].z = x3 * GSYS.DrawSetting.BillboardMatrixF.m[0][2] + y3 * GSYS.DrawSetting.BillboardMatrixF.m[1][2] + Pos.z ;
			}

			// ���_�^�C�v�ɂ���ď����𕪊�
			switch( GD3D9.Device.DrawInfo.VertexType )
			{
			case VERTEXTYPE_BLENDTEX :
				// �u�����h�e�N�X�`�����g�p����

				// ���_�o�b�t�@�̎擾
				GETVERTEX_QUAD( DrawVertB )

				DrawVertB[0].color = DiffuseColor ;
				DrawVertB[1].color = DiffuseColor ;
				DrawVertB[2].color = DiffuseColor ;
				DrawVertB[3].color = DiffuseColor ;
				DrawVertB[4].color = DiffuseColor ;
				DrawVertB[5].color = DiffuseColor ;

				// �X�N���[�����W�ɕϊ�
				rhw = 1.0f / ( SrcVec2[0].x * GSYS.DrawSetting.BlendMatrixF.m[0][3] + SrcVec2[0].y * GSYS.DrawSetting.BlendMatrixF.m[1][3] + SrcVec2[0].z * GSYS.DrawSetting.BlendMatrixF.m[2][3] + GSYS.DrawSetting.BlendMatrixF.m[3][3] ) ;
				z   = rhw  * ( SrcVec2[0].x * GSYS.DrawSetting.BlendMatrixF.m[0][2] + SrcVec2[0].y * GSYS.DrawSetting.BlendMatrixF.m[1][2] + SrcVec2[0].z * GSYS.DrawSetting.BlendMatrixF.m[2][2] + GSYS.DrawSetting.BlendMatrixF.m[3][2] ) ;

				DrawPos[ 0 ].x = ( SrcVec2[0].x * GSYS.DrawSetting.BlendMatrixF.m[0][0] + SrcVec2[0].y * GSYS.DrawSetting.BlendMatrixF.m[1][0] + SrcVec2[0].z * GSYS.DrawSetting.BlendMatrixF.m[2][0] + GSYS.DrawSetting.BlendMatrixF.m[3][0] ) * rhw ;
				DrawPos[ 0 ].y = ( SrcVec2[0].x * GSYS.DrawSetting.BlendMatrixF.m[0][1] + SrcVec2[0].y * GSYS.DrawSetting.BlendMatrixF.m[1][1] + SrcVec2[0].z * GSYS.DrawSetting.BlendMatrixF.m[2][1] + GSYS.DrawSetting.BlendMatrixF.m[3][1] ) * rhw ;
				DrawPos[ 1 ].x = ( SrcVec2[1].x * GSYS.DrawSetting.BlendMatrixF.m[0][0] + SrcVec2[1].y * GSYS.DrawSetting.BlendMatrixF.m[1][0] + SrcVec2[1].z * GSYS.DrawSetting.BlendMatrixF.m[2][0] + GSYS.DrawSetting.BlendMatrixF.m[3][0] ) * rhw ;
				DrawPos[ 1 ].y = ( SrcVec2[1].x * GSYS.DrawSetting.BlendMatrixF.m[0][1] + SrcVec2[1].y * GSYS.DrawSetting.BlendMatrixF.m[1][1] + SrcVec2[1].z * GSYS.DrawSetting.BlendMatrixF.m[2][1] + GSYS.DrawSetting.BlendMatrixF.m[3][1] ) * rhw ;
				DrawPos[ 2 ].x = ( SrcVec2[2].x * GSYS.DrawSetting.BlendMatrixF.m[0][0] + SrcVec2[2].y * GSYS.DrawSetting.BlendMatrixF.m[1][0] + SrcVec2[2].z * GSYS.DrawSetting.BlendMatrixF.m[2][0] + GSYS.DrawSetting.BlendMatrixF.m[3][0] ) * rhw ;
				DrawPos[ 2 ].y = ( SrcVec2[2].x * GSYS.DrawSetting.BlendMatrixF.m[0][1] + SrcVec2[2].y * GSYS.DrawSetting.BlendMatrixF.m[1][1] + SrcVec2[2].z * GSYS.DrawSetting.BlendMatrixF.m[2][1] + GSYS.DrawSetting.BlendMatrixF.m[3][1] ) * rhw ;
				DrawPos[ 3 ].x = ( SrcVec2[3].x * GSYS.DrawSetting.BlendMatrixF.m[0][0] + SrcVec2[3].y * GSYS.DrawSetting.BlendMatrixF.m[1][0] + SrcVec2[3].z * GSYS.DrawSetting.BlendMatrixF.m[2][0] + GSYS.DrawSetting.BlendMatrixF.m[3][0] ) * rhw ;
				DrawPos[ 3 ].y = ( SrcVec2[3].x * GSYS.DrawSetting.BlendMatrixF.m[0][1] + SrcVec2[3].y * GSYS.DrawSetting.BlendMatrixF.m[1][1] + SrcVec2[3].z * GSYS.DrawSetting.BlendMatrixF.m[2][1] + GSYS.DrawSetting.BlendMatrixF.m[3][1] ) * rhw ;

				DrawVertB[0].pos.x = DrawPos[ 0 ].x ;
				DrawVertB[0].pos.y = DrawPos[ 0 ].y ;
				DrawVertB[1].pos.x = DrawPos[ 1 ].x ;
				DrawVertB[1].pos.y = DrawPos[ 1 ].y ;
				DrawVertB[2].pos.x = DrawPos[ 2 ].x ;
				DrawVertB[2].pos.y = DrawPos[ 2 ].y ;
				DrawVertB[3].pos.x = DrawPos[ 3 ].x ;
				DrawVertB[3].pos.y = DrawPos[ 3 ].y ;
				DrawVertB[4].pos.x = DrawPos[ 2 ].x ;
				DrawVertB[4].pos.y = DrawPos[ 2 ].y ;
				DrawVertB[5].pos.x = DrawPos[ 1 ].x ;
				DrawVertB[5].pos.y = DrawPos[ 1 ].y ;

				switch( GSYS.DrawSetting.BlendGraphPosMode )
				{
				default :
				case DX_BLENDGRAPH_POSMODE_DRAWGRAPH:
					u[ 0 ] = ( TexVert[0].x - BlendTexVert[0].x + GSYS.DrawSetting.BlendGraphX ) * GD3D9.Device.DrawSetting.InvBlendTextureWidth ;
					u[ 1 ] = ( TexVert[1].x - BlendTexVert[0].x + GSYS.DrawSetting.BlendGraphX ) * GD3D9.Device.DrawSetting.InvBlendTextureWidth ;
					v[ 0 ] = ( TexVert[0].y - BlendTexVert[0].y + GSYS.DrawSetting.BlendGraphY ) * GD3D9.Device.DrawSetting.InvBlendTextureHeight ;
					v[ 1 ] = ( TexVert[2].y - BlendTexVert[0].y + GSYS.DrawSetting.BlendGraphY ) * GD3D9.Device.DrawSetting.InvBlendTextureHeight ;
					DrawVertB[0].u1 = u[ 0 ] ;
					DrawVertB[0].v1 = v[ 0 ] ;
					DrawVertB[1].u1 = u[ 1 ] ;
					DrawVertB[1].v1 = v[ 0 ] ;
					DrawVertB[2].u1 = u[ 0 ] ;
					DrawVertB[2].v1 = v[ 1 ] ;
					DrawVertB[3].u1 = u[ 1 ] ;
					DrawVertB[3].v1 = v[ 1 ] ;
					DrawVertB[4].u1 = u[ 0 ] ;
					DrawVertB[4].v1 = v[ 1 ] ;
					DrawVertB[5].u1 = u[ 1 ] ;
					DrawVertB[5].v1 = v[ 0 ] ;
					break ;

				case DX_BLENDGRAPH_POSMODE_SCREEN :
					DrawVertB[0].u1 = ( DrawPos[ 0 ].x - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D9.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[0].v1 = ( DrawPos[ 0 ].y - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D9.Device.DrawSetting.InvBlendTextureHeight ;
					DrawVertB[1].u1 = ( DrawPos[ 1 ].x - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D9.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[1].v1 = ( DrawPos[ 1 ].y - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D9.Device.DrawSetting.InvBlendTextureHeight ;
					DrawVertB[2].u1 = ( DrawPos[ 2 ].x - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D9.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[2].v1 = ( DrawPos[ 2 ].y - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D9.Device.DrawSetting.InvBlendTextureHeight ;
					DrawVertB[3].u1 = ( DrawPos[ 3 ].x - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D9.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[3].v1 = ( DrawPos[ 3 ].y - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D9.Device.DrawSetting.InvBlendTextureHeight ;
					DrawVertB[4].u1 = ( DrawPos[ 2 ].x - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D9.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[4].v1 = ( DrawPos[ 2 ].y - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D9.Device.DrawSetting.InvBlendTextureHeight ;
					DrawVertB[5].u1 = ( DrawPos[ 1 ].x - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D9.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[5].v1 = ( DrawPos[ 1 ].y - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D9.Device.DrawSetting.InvBlendTextureHeight ;
					break ;
				}

				DrawVertB[0].u2 = TexVert[0].u ;
				DrawVertB[0].v2 = TexVert[0].v ;
				DrawVertB[1].u2 = TexVert[1].u ;
				DrawVertB[1].v2 = TexVert[0].v ;
				DrawVertB[2].u2 = TexVert[0].u ;
				DrawVertB[2].v2 = TexVert[2].v ;
				DrawVertB[3].u2 = TexVert[1].u ;
				DrawVertB[3].v2 = TexVert[2].v ;
				DrawVertB[4].u2 = TexVert[0].u ;
				DrawVertB[4].v2 = TexVert[2].v ;
				DrawVertB[5].u2 = TexVert[1].u ;
				DrawVertB[5].v2 = TexVert[0].v ;

				DrawVertB[0].pos.z = z ;
				DrawVertB[1].pos.z = z ;
				DrawVertB[2].pos.z = z ;
				DrawVertB[3].pos.z = z ;
				DrawVertB[4].pos.z = z ;
				DrawVertB[5].pos.z = z ;

				DrawVertB[0].rhw = rhw ;
				DrawVertB[1].rhw = rhw ;
				DrawVertB[2].rhw = rhw ;
				DrawVertB[3].rhw = rhw ;
				DrawVertB[4].rhw = rhw ;
				DrawVertB[5].rhw = rhw ;

				// �e�N�X�`���[��`�悷��
				if( DrawFlag )
				{
					ADD4VERTEX_BLENDTEX
				}
				else
				{
					if( dright  > DrawVertB[ 0 ].pos.x ) dright  = DrawVertB[ 0 ].pos.x ;
					if( dright  > DrawVertB[ 1 ].pos.x ) dright  = DrawVertB[ 1 ].pos.x ;
					if( dright  > DrawVertB[ 2 ].pos.x ) dright  = DrawVertB[ 2 ].pos.x ;
					if( dright  > DrawVertB[ 3 ].pos.x ) dright  = DrawVertB[ 3 ].pos.x ;

					if( dleft   < DrawVertB[ 0 ].pos.x ) dleft   = DrawVertB[ 0 ].pos.x ;
					if( dleft   < DrawVertB[ 1 ].pos.x ) dleft   = DrawVertB[ 1 ].pos.x ;
					if( dleft   < DrawVertB[ 2 ].pos.x ) dleft   = DrawVertB[ 2 ].pos.x ;
					if( dleft   < DrawVertB[ 3 ].pos.x ) dleft   = DrawVertB[ 3 ].pos.x ;

					if( dbottom > DrawVertB[ 0 ].pos.y ) dbottom = DrawVertB[ 0 ].pos.y ;
					if( dbottom > DrawVertB[ 1 ].pos.y ) dbottom = DrawVertB[ 1 ].pos.y ;
					if( dbottom > DrawVertB[ 2 ].pos.y ) dbottom = DrawVertB[ 2 ].pos.y ;
					if( dbottom > DrawVertB[ 3 ].pos.y ) dbottom = DrawVertB[ 3 ].pos.y ;

					if( dtop    < DrawVertB[ 0 ].pos.y ) dtop    = DrawVertB[ 0 ].pos.y ;
					if( dtop    < DrawVertB[ 1 ].pos.y ) dtop    = DrawVertB[ 1 ].pos.y ;
					if( dtop    < DrawVertB[ 2 ].pos.y ) dtop    = DrawVertB[ 2 ].pos.y ;
					if( dtop    < DrawVertB[ 3 ].pos.y ) dtop    = DrawVertB[ 3 ].pos.y ;
				}

				if( BlendGraphNoIncFlag == FALSE )
				{
					BlendDrawTex ++ ;
				}
				break ;

			case VERTEXTYPE_TEX :
				// �u�����h�e�N�X�`�����g�p���Ȃ�

				// ���_�o�b�t�@�̎擾
				GETVERTEX_QUAD( DrawVert )

				// �X�N���[�����W�ɕϊ�
				rhw = 1.0f / ( SrcVec2[0].x * GSYS.DrawSetting.BlendMatrixF.m[0][3] + SrcVec2[0].y * GSYS.DrawSetting.BlendMatrixF.m[1][3] + SrcVec2[0].z * GSYS.DrawSetting.BlendMatrixF.m[2][3] + GSYS.DrawSetting.BlendMatrixF.m[3][3] ) ;
				z   = rhw  * ( SrcVec2[0].x * GSYS.DrawSetting.BlendMatrixF.m[0][2] + SrcVec2[0].y * GSYS.DrawSetting.BlendMatrixF.m[1][2] + SrcVec2[0].z * GSYS.DrawSetting.BlendMatrixF.m[2][2] + GSYS.DrawSetting.BlendMatrixF.m[3][2] ) ;

				DrawVert[0].pos.x = ( SrcVec2[0].x * GSYS.DrawSetting.BlendMatrixF.m[0][0] + SrcVec2[0].y * GSYS.DrawSetting.BlendMatrixF.m[1][0] + SrcVec2[0].z * GSYS.DrawSetting.BlendMatrixF.m[2][0] + GSYS.DrawSetting.BlendMatrixF.m[3][0] ) * rhw ;
				DrawVert[0].pos.y = ( SrcVec2[0].x * GSYS.DrawSetting.BlendMatrixF.m[0][1] + SrcVec2[0].y * GSYS.DrawSetting.BlendMatrixF.m[1][1] + SrcVec2[0].z * GSYS.DrawSetting.BlendMatrixF.m[2][1] + GSYS.DrawSetting.BlendMatrixF.m[3][1] ) * rhw ;
				DrawVert[1].pos.x = ( SrcVec2[1].x * GSYS.DrawSetting.BlendMatrixF.m[0][0] + SrcVec2[1].y * GSYS.DrawSetting.BlendMatrixF.m[1][0] + SrcVec2[1].z * GSYS.DrawSetting.BlendMatrixF.m[2][0] + GSYS.DrawSetting.BlendMatrixF.m[3][0] ) * rhw ;
				DrawVert[1].pos.y = ( SrcVec2[1].x * GSYS.DrawSetting.BlendMatrixF.m[0][1] + SrcVec2[1].y * GSYS.DrawSetting.BlendMatrixF.m[1][1] + SrcVec2[1].z * GSYS.DrawSetting.BlendMatrixF.m[2][1] + GSYS.DrawSetting.BlendMatrixF.m[3][1] ) * rhw ;
				DrawVert[2].pos.x = ( SrcVec2[2].x * GSYS.DrawSetting.BlendMatrixF.m[0][0] + SrcVec2[2].y * GSYS.DrawSetting.BlendMatrixF.m[1][0] + SrcVec2[2].z * GSYS.DrawSetting.BlendMatrixF.m[2][0] + GSYS.DrawSetting.BlendMatrixF.m[3][0] ) * rhw ;
				DrawVert[2].pos.y = ( SrcVec2[2].x * GSYS.DrawSetting.BlendMatrixF.m[0][1] + SrcVec2[2].y * GSYS.DrawSetting.BlendMatrixF.m[1][1] + SrcVec2[2].z * GSYS.DrawSetting.BlendMatrixF.m[2][1] + GSYS.DrawSetting.BlendMatrixF.m[3][1] ) * rhw ;
				DrawVert[3].pos.x = ( SrcVec2[3].x * GSYS.DrawSetting.BlendMatrixF.m[0][0] + SrcVec2[3].y * GSYS.DrawSetting.BlendMatrixF.m[1][0] + SrcVec2[3].z * GSYS.DrawSetting.BlendMatrixF.m[2][0] + GSYS.DrawSetting.BlendMatrixF.m[3][0] ) * rhw ;
				DrawVert[3].pos.y = ( SrcVec2[3].x * GSYS.DrawSetting.BlendMatrixF.m[0][1] + SrcVec2[3].y * GSYS.DrawSetting.BlendMatrixF.m[1][1] + SrcVec2[3].z * GSYS.DrawSetting.BlendMatrixF.m[2][1] + GSYS.DrawSetting.BlendMatrixF.m[3][1] ) * rhw ;
				DrawVert[4].pos.x = ( SrcVec2[2].x * GSYS.DrawSetting.BlendMatrixF.m[0][0] + SrcVec2[2].y * GSYS.DrawSetting.BlendMatrixF.m[1][0] + SrcVec2[2].z * GSYS.DrawSetting.BlendMatrixF.m[2][0] + GSYS.DrawSetting.BlendMatrixF.m[3][0] ) * rhw ;
				DrawVert[4].pos.y = ( SrcVec2[2].x * GSYS.DrawSetting.BlendMatrixF.m[0][1] + SrcVec2[2].y * GSYS.DrawSetting.BlendMatrixF.m[1][1] + SrcVec2[2].z * GSYS.DrawSetting.BlendMatrixF.m[2][1] + GSYS.DrawSetting.BlendMatrixF.m[3][1] ) * rhw ;
				DrawVert[5].pos.x = ( SrcVec2[1].x * GSYS.DrawSetting.BlendMatrixF.m[0][0] + SrcVec2[1].y * GSYS.DrawSetting.BlendMatrixF.m[1][0] + SrcVec2[1].z * GSYS.DrawSetting.BlendMatrixF.m[2][0] + GSYS.DrawSetting.BlendMatrixF.m[3][0] ) * rhw ;
				DrawVert[5].pos.y = ( SrcVec2[1].x * GSYS.DrawSetting.BlendMatrixF.m[0][1] + SrcVec2[1].y * GSYS.DrawSetting.BlendMatrixF.m[1][1] + SrcVec2[1].z * GSYS.DrawSetting.BlendMatrixF.m[2][1] + GSYS.DrawSetting.BlendMatrixF.m[3][1] ) * rhw ;

				DrawVert[0].pos.z = z ;
				DrawVert[1].pos.z = z ;
				DrawVert[2].pos.z = z ;
				DrawVert[3].pos.z = z ;
				DrawVert[4].pos.z = z ;
				DrawVert[5].pos.z = z ;

				DrawVert[0].rhw = rhw ;
				DrawVert[1].rhw = rhw ;
				DrawVert[2].rhw = rhw ;
				DrawVert[3].rhw = rhw ;
				DrawVert[4].rhw = rhw ;
				DrawVert[5].rhw = rhw ;

				DrawVert[0].color = DiffuseColor ;
				DrawVert[1].color = DiffuseColor ;
				DrawVert[2].color = DiffuseColor ;
				DrawVert[3].color = DiffuseColor ;
				DrawVert[4].color = DiffuseColor ;
				DrawVert[5].color = DiffuseColor ;

				DrawVert[0].u = TexVert[0].u ;
				DrawVert[1].u = TexVert[1].u ;
				DrawVert[2].u = TexVert[0].u ;
				DrawVert[3].u = TexVert[1].u ;
				DrawVert[4].u = TexVert[0].u ;
				DrawVert[5].u = TexVert[1].u ;

				DrawVert[0].v = TexVert[0].v ;
				DrawVert[1].v = TexVert[0].v ;
				DrawVert[2].v = TexVert[2].v ;
				DrawVert[3].v = TexVert[2].v ;
				DrawVert[4].v = TexVert[2].v ;
				DrawVert[5].v = TexVert[0].v ;

				// �e�N�X�`���[��`�悷��
				if( DrawFlag )
				{
					ADD4VERTEX_TEX
				}
				else
				{
					if( dright  > DrawVert[ 0 ].pos.x ) dright  = DrawVert[ 0 ].pos.x ;
					if( dright  > DrawVert[ 1 ].pos.x ) dright  = DrawVert[ 1 ].pos.x ;
					if( dright  > DrawVert[ 2 ].pos.x ) dright  = DrawVert[ 2 ].pos.x ;
					if( dright  > DrawVert[ 3 ].pos.x ) dright  = DrawVert[ 3 ].pos.x ;

					if( dleft   < DrawVert[ 0 ].pos.x ) dleft   = DrawVert[ 0 ].pos.x ;
					if( dleft   < DrawVert[ 1 ].pos.x ) dleft   = DrawVert[ 1 ].pos.x ;
					if( dleft   < DrawVert[ 2 ].pos.x ) dleft   = DrawVert[ 2 ].pos.x ;
					if( dleft   < DrawVert[ 3 ].pos.x ) dleft   = DrawVert[ 3 ].pos.x ;

					if( dbottom > DrawVert[ 0 ].pos.y ) dbottom = DrawVert[ 0 ].pos.y ;
					if( dbottom > DrawVert[ 1 ].pos.y ) dbottom = DrawVert[ 1 ].pos.y ;
					if( dbottom > DrawVert[ 2 ].pos.y ) dbottom = DrawVert[ 2 ].pos.y ;
					if( dbottom > DrawVert[ 3 ].pos.y ) dbottom = DrawVert[ 3 ].pos.y ;

					if( dtop    < DrawVert[ 0 ].pos.y ) dtop    = DrawVert[ 0 ].pos.y ;
					if( dtop    < DrawVert[ 1 ].pos.y ) dtop    = DrawVert[ 1 ].pos.y ;
					if( dtop    < DrawVert[ 2 ].pos.y ) dtop    = DrawVert[ 2 ].pos.y ;
					if( dtop    < DrawVert[ 3 ].pos.y ) dtop    = DrawVert[ 3 ].pos.y ;
				}
				break ;
			}
		}
	}

	if( DrawFlag == FALSE )
	{
		DrawArea->left   = _FTOL( dleft   ) - 1 ;
		DrawArea->right  = _FTOL( dright  ) + 1 ;
		DrawArea->top    = _FTOL( dtop    ) - 1 ;
		DrawArea->bottom = _FTOL( dbottom ) + 1 ;
	}

	// �I��
	return 0 ;
}

// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawGraph
extern	int		Graphics_D3D9_DrawGraph( int x, int y, float xf, float yf, IMAGEDATA *Image, IMAGEDATA *BlendImage, int TransFlag, int IntFlag )
{
	VERTEX_2D			*DrawVert ;
	VERTEX_BLENDTEX_2D	*DrawVertB ;
	IMAGEDATA_HARD_DRAW	*DrawTex ;
	IMAGEDATA_HARD_DRAW	*BlendDrawTex = NULL ;
	IMAGEDATA_ORIG		*Orig ;
	IMAGEDATA_HARD_VERT	*TexVert ;
	IMAGEDATA_HARD_VERT	*BlendTexVert ;
	DWORD				DiffuseColor ;
	int					DrawTexNum ;
	int					i ;
	int					j ;
	int					Flag ;
	int					BlendGraphNoIncFlag ;
	float				left ;
	float				right ;
	float				top ;
	float				bottom ;
	float				Vx ;
	float				Vy ;
	float				VBx ;
	float				VBy ;
	DWORD				drawz ;
	float				u[ 2 ] ;
	float				v[ 2 ] ;

	Orig = Image->Orig ;

	if( GAPIWin.Direct3DDevice9Object == NULL ) return -1 ;

	// float���W�␳
	xf -= 0.5F ;
	yf -= 0.5F ;

	// �`�揀��
	if( GD3D9.Device.DrawInfo.BeginSceneFlag == FALSE )
	{
		Graphics_D3D9_BeginScene() ;
	}
	Flag = TransFlag | ( IntFlag << 1 ) ;
	DX_D3D9_DRAWPREP_TEX( Orig, Image->Hard.Draw[ 0 ].Tex->PF->D3D9.Texture, Flag )

	// ���_�f�[�^���擾
	DiffuseColor = GD3D9.Device.DrawInfo.DiffuseColor ;

	// �`����̐����Z�b�g
	DrawTexNum = Image->Hard.DrawNum ;

	// �`����̐����u�����h�摜�ƈقȂ��Ă�����O�Ԗڂ̃e�N�X�`���������g�p����
	BlendGraphNoIncFlag = FALSE ;
	if( BlendImage != NULL && BlendImage->Hard.DrawNum != Image->Hard.DrawNum )
	{
		BlendGraphNoIncFlag = TRUE ;
	}

	// �`����z��̃A�h���X���Z�b�g���Ă���
	DrawTex = Image->Hard.Draw ;
	if( BlendImage != NULL )
	{
		BlendDrawTex = BlendImage->Hard.Draw ;
	}

	// �y�o�b�t�@�ɏ������ނy�l���Z�b�g����
	drawz = *( ( DWORD * )&GSYS.DrawSetting.DrawZ ) ;

	// �`��͈͂���͂ݏo�Ă��邩�ǂ����ŏ����𕪊�
	if( IN_DRAWAREA_CHECK( x, y, x + Image->WidthI, y + Image->HeightI ) )
//	if( x + Image->Width < GSYS.DrawSetting.DrawArea.right && x > GSYS.DrawSetting.DrawArea.left &&
//		y + Image->Height < GSYS.DrawSetting.DrawArea.bottom && y > GSYS.DrawSetting.DrawArea.right )
	{
		// �͂ݏo�Ă��Ȃ��ꍇ

		// ���_�^�C�v�ɂ���ď����𕪊�
		if( GD3D9.Device.DrawInfo.VertexType == VERTEXTYPE_BLENDTEX )
		{
			// �u�����h�e�N�X�`�����g�p����

			// �e�N�X�`���[�̐������J��Ԃ�
			for( i = 0 ; i < DrawTexNum ; i ++, DrawTex ++ )
			{
				// �`�悷��e�N�X�`���[�̃Z�b�g
				Graphics_D3D9_DrawSetting_SetTexture( DrawTex->Tex->PF->D3D9.Texture ) ;
				Graphics_D3D9_DrawSetting_SetBlendTexture( BlendDrawTex->Tex->PF->D3D9.Texture, BlendDrawTex->Tex->TexWidth, BlendDrawTex->Tex->TexHeight );

				if( GD3D9.Device.DrawSetting.ChangeTextureFlag )
				{
					Graphics_D3D9_DeviceState_RefreshBlendState() ;
				}

				// ���_�o�b�t�@�̎擾
				GETVERTEX_QUAD( DrawVertB )
				
				TexVert = DrawTex->Vertex ;
				BlendTexVert = BlendDrawTex->Vertex ;

				DrawVertB[0].color = ( DWORD )DiffuseColor ; 
				DrawVertB[3].color = ( DWORD )DiffuseColor ;

				DrawVertB[0].pos.x = TexVert[0].x + xf ;
				DrawVertB[0].pos.y = TexVert[0].y + yf ;
				DrawVertB[1].pos.x = TexVert[1].x + xf ;
				DrawVertB[1].pos.y = TexVert[0].y + yf ;
				DrawVertB[2].pos.x = TexVert[0].x + xf ;
				DrawVertB[2].pos.y = TexVert[2].y + yf ;
				DrawVertB[3].pos.x = TexVert[1].x + xf ;
				DrawVertB[3].pos.y = TexVert[2].y + yf ;
				DrawVertB[4].pos.x = TexVert[0].x + xf ;
				DrawVertB[4].pos.y = TexVert[2].y + yf ;
				DrawVertB[5].pos.x = TexVert[1].x + xf ;
				DrawVertB[5].pos.y = TexVert[0].y + yf ;

				switch( GSYS.DrawSetting.BlendGraphPosMode )
				{
				default :
				case DX_BLENDGRAPH_POSMODE_DRAWGRAPH:
					u[ 0 ] = TexVert[0].x - BlendTexVert[0].x + GSYS.DrawSetting.BlendGraphX ;
					u[ 1 ] = TexVert[1].x - BlendTexVert[0].x + GSYS.DrawSetting.BlendGraphX ;
					v[ 0 ] = TexVert[0].y - BlendTexVert[0].y + GSYS.DrawSetting.BlendGraphY ;
					v[ 1 ] = TexVert[2].y - BlendTexVert[0].y + GSYS.DrawSetting.BlendGraphY ;
					break ;

				case DX_BLENDGRAPH_POSMODE_SCREEN :
					u[ 0 ] = ( xf + TexVert[0].x ) - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ;
					u[ 1 ] = ( xf + TexVert[1].x ) - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ;
					v[ 0 ] = ( yf + TexVert[0].y ) - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ;
					v[ 1 ] = ( yf + TexVert[2].y ) - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ;
					break ;
				}

				u[ 0 ] *= GD3D9.Device.DrawSetting.InvBlendTextureWidth ;
				u[ 1 ] *= GD3D9.Device.DrawSetting.InvBlendTextureWidth ;
				v[ 0 ] *= GD3D9.Device.DrawSetting.InvBlendTextureHeight ;
				v[ 1 ] *= GD3D9.Device.DrawSetting.InvBlendTextureHeight ;

				DrawVertB[0].u1 = u[ 0 ] ;
				DrawVertB[0].v1 = v[ 0 ] ;
				DrawVertB[1].u1 = u[ 1 ] ;
				DrawVertB[1].v1 = v[ 0 ] ;
				DrawVertB[2].u1 = u[ 0 ] ;
				DrawVertB[2].v1 = v[ 1 ] ;
				DrawVertB[3].u1 = u[ 1 ] ;
				DrawVertB[3].v1 = v[ 1 ] ;
				DrawVertB[4].u1 = u[ 0 ] ;
				DrawVertB[4].v1 = v[ 1 ] ;
				DrawVertB[5].u1 = u[ 1 ] ;
				DrawVertB[5].v1 = v[ 0 ] ;

				DrawVertB[0].u2 = TexVert[0].u ;
				DrawVertB[0].v2 = TexVert[0].v ;
				DrawVertB[1].u2 = TexVert[1].u ;
				DrawVertB[1].v2 = TexVert[0].v ;
				DrawVertB[2].u2 = TexVert[0].u ;
				DrawVertB[2].v2 = TexVert[2].v ;
				DrawVertB[3].u2 = TexVert[1].u ;
				DrawVertB[3].v2 = TexVert[2].v ;
				DrawVertB[4].u2 = TexVert[0].u ;
				DrawVertB[4].v2 = TexVert[2].v ;
				DrawVertB[5].u2 = TexVert[1].u ;
				DrawVertB[5].v2 = TexVert[0].v ;

				*((DWORD *)&DrawVertB[0].pos.z) = drawz ;
				*((DWORD *)&DrawVertB[1].pos.z) = drawz ;
				*((DWORD *)&DrawVertB[2].pos.z) = drawz ;
				*((DWORD *)&DrawVertB[3].pos.z) = drawz ;
				*((DWORD *)&DrawVertB[4].pos.z) = drawz ;
				*((DWORD *)&DrawVertB[5].pos.z) = drawz ;

				// �e�N�X�`���[��`�悷��
				ADD4VERTEX_BLENDTEX

				if( BlendGraphNoIncFlag == FALSE )
				{
					BlendDrawTex ++ ;
				}
			}
		}
		else
		{
			// �u�����h�e�N�X�`�����g�p���Ȃ�

			// �e�N�X�`���[�̐������J��Ԃ�
			i = 0 ;
			for(;;)
			{
				// ���_�o�b�t�@�̎擾
				GETVERTEX_QUAD( DrawVert )

				// ���_�f�[�^����
				TexVert = DrawTex->Vertex ;
				DrawVert[0].color = DiffuseColor ;
				DrawVert[3].color = DiffuseColor ;

				DrawVert[0].pos.x = TexVert[0].x + xf ;
				DrawVert[0].pos.y = TexVert[0].y + yf ;
				DrawVert[1].pos.x = TexVert[1].x + xf ;
				DrawVert[1].pos.y = TexVert[0].y + yf ;
				DrawVert[2].pos.x = TexVert[0].x + xf ;
				DrawVert[2].pos.y = TexVert[2].y + yf ;
				DrawVert[3].pos.x = TexVert[1].x + xf ;
				DrawVert[3].pos.y = TexVert[2].y + yf ;
				DrawVert[4].pos.x = TexVert[0].x + xf ;
				DrawVert[4].pos.y = TexVert[2].y + yf ;
				DrawVert[5].pos.x = TexVert[1].x + xf ;
				DrawVert[5].pos.y = TexVert[0].y + yf ;

				DrawVert[0].u = TexVert[0].u ;
				DrawVert[0].v = TexVert[0].v ;
				DrawVert[1].u = TexVert[1].u ;
				DrawVert[1].v = TexVert[0].v ;
				DrawVert[2].u = TexVert[0].u ;
				DrawVert[2].v = TexVert[2].v ;
				DrawVert[3].u = TexVert[1].u ;
				DrawVert[3].v = TexVert[2].v ;
				DrawVert[4].u = TexVert[0].u ;
				DrawVert[4].v = TexVert[2].v ;
				DrawVert[5].u = TexVert[1].u ;
				DrawVert[5].v = TexVert[0].v ;

				*((DWORD *)&DrawVert[0].pos.z) = drawz ;
				*((DWORD *)&DrawVert[1].pos.z) = drawz ;
				*((DWORD *)&DrawVert[2].pos.z) = drawz ;
				*((DWORD *)&DrawVert[3].pos.z) = drawz ;
				*((DWORD *)&DrawVert[4].pos.z) = drawz ;
				*((DWORD *)&DrawVert[5].pos.z) = drawz ;

				// �e�N�X�`���[��`�悷��
				ADD4VERTEX_TEX

				i ++ ;
				if( i >= DrawTexNum ) break ;

				// �`�悷��e�N�X�`���[�̃Z�b�g
				DrawTex ++ ;
				Graphics_D3D9_DrawSetting_SetTexture( DrawTex->Tex->PF->D3D9.Texture ) ;
				Graphics_D3D9_DeviceState_RefreshBlendState() ;
			}
		}
	}
	else
	{
		// �͂ݏo�Ă���ꍇ

		// ���S�ɂ͂ݏo�Ă����牽�������I��
		if( OUT_DRAWAREA_CHECK( x, y, x + Image->WidthI, y + Image->HeightI ) ) return 0 ;
//		if( x > GSYS.DrawSetting.DrawArea.right || x + Image->Width < GSYS.DrawSetting.DrawArea.left ||
//			y > GSYS.DrawSetting.DrawArea.bottom || y + Image->Height < GSYS.DrawSetting.DrawArea.top ) return 0 ;

		// �`���`�̕ۑ�
		left	= GSYS.DrawSetting.DrawAreaF.left   ;
		top		= GSYS.DrawSetting.DrawAreaF.top    ;
		right	= GSYS.DrawSetting.DrawAreaF.right  ;
		bottom	= GSYS.DrawSetting.DrawAreaF.bottom ;

		// ���_�^�C�v�ɂ���ď����𕪊�
		switch( GD3D9.Device.DrawInfo.VertexType )
		{
		case VERTEXTYPE_BLENDTEX :
			// �u�����h�e�N�X�`�����g�p����

			// �e�N�X�`���[�̐������J��Ԃ�
			for( i = 0 ; i < DrawTexNum ; i ++, DrawTex ++ )
			{
				VECTOR DrawPos[ 4 ] ;
				float u1[ 4 ] ;
				float v1[ 4 ] ;
				float u2[ 4 ] ;
				float v2[ 4 ] ;

				// �`�悷��e�N�X�`���[�̃Z�b�g
				Graphics_D3D9_DrawSetting_SetTexture( DrawTex->Tex->PF->D3D9.Texture ) ;
				Graphics_D3D9_DrawSetting_SetBlendTexture( BlendDrawTex->Tex->PF->D3D9.Texture, BlendDrawTex->Tex->TexWidth, BlendDrawTex->Tex->TexHeight );

				if( GD3D9.Device.DrawSetting.ChangeTextureFlag )
				{
					Graphics_D3D9_DeviceState_RefreshBlendState() ;
				}

				// ���_�o�b�t�@�̎擾
				GETVERTEX_QUAD( DrawVertB )
				
				// ���_�f�[�^������������
				TexVert = DrawTex->Vertex ;
				BlendTexVert = BlendDrawTex->Vertex ;

				Vx  = 1.0F / ( float )DrawTex->Tex->TexWidth  ;
				Vy  = 1.0F / ( float )DrawTex->Tex->TexHeight ;
				VBx = GD3D9.Device.DrawSetting.InvBlendTextureWidth  ;
				VBy = GD3D9.Device.DrawSetting.InvBlendTextureHeight ;

				DrawVertB[0].color = ( DWORD )DiffuseColor ;
				DrawVertB[3].color = ( DWORD )DiffuseColor ;

				DrawPos[0].x = TexVert[0].x + xf ;
				DrawPos[0].y = TexVert[0].y + yf ;
				DrawPos[1].x = TexVert[1].x + xf ;
				DrawPos[1].y = TexVert[0].y + yf ;
				DrawPos[2].x = TexVert[0].x + xf ;
				DrawPos[2].y = TexVert[2].y + yf ;
				DrawPos[3].x = TexVert[1].x + xf ;
				DrawPos[3].y = TexVert[2].y + yf ;

				u2[2] = u2[0] = TexVert[0].u ;
				u2[3] = u2[1] = TexVert[1].u ;
				v2[1] = v2[0] = TexVert[0].v ;
				v2[3] = v2[2] = TexVert[2].v ;

				switch( GSYS.DrawSetting.BlendGraphPosMode )
				{
				default :
				case DX_BLENDGRAPH_POSMODE_DRAWGRAPH:
					u1[2] = u1[0] = ( TexVert[0].x - BlendTexVert[0].x + GSYS.DrawSetting.BlendGraphX ) * GD3D9.Device.DrawSetting.InvBlendTextureWidth ;
					u1[3] = u1[1] = ( TexVert[1].x - BlendTexVert[0].x + GSYS.DrawSetting.BlendGraphX ) * GD3D9.Device.DrawSetting.InvBlendTextureWidth ;
					v1[1] = v1[0] = ( TexVert[0].y - BlendTexVert[0].y + GSYS.DrawSetting.BlendGraphY ) * GD3D9.Device.DrawSetting.InvBlendTextureHeight ;
					v1[3] = v1[2] = ( TexVert[2].y - BlendTexVert[0].y + GSYS.DrawSetting.BlendGraphY ) * GD3D9.Device.DrawSetting.InvBlendTextureHeight ;
					break ;

				case DX_BLENDGRAPH_POSMODE_SCREEN :
					u1[2] = u1[0] = ( ( xf + TexVert[0].x ) - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D9.Device.DrawSetting.InvBlendTextureWidth ;
					u1[3] = u1[1] = ( ( xf + TexVert[1].x ) - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D9.Device.DrawSetting.InvBlendTextureWidth ;
					v1[1] = v1[0] = ( ( yf + TexVert[0].y ) - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D9.Device.DrawSetting.InvBlendTextureHeight ;
					v1[3] = v1[2] = ( ( yf + TexVert[2].y ) - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D9.Device.DrawSetting.InvBlendTextureHeight ;
					break ;
				}

				for( j = 0 ; j < 4 ; j ++ )
				{
					// �`���`����o�Ă����ꍇ�̕␳����
					if( DrawPos[ j ].x < left )
					{
						u2[ j ] += ( left - DrawPos[ j ].x ) * Vx ;
						u1[ j ] += ( left - DrawPos[ j ].x ) * VBx ;
						DrawPos[ j ].x = left ;
					}
					else
					if( DrawPos[ j ].x > right )
					{
						u2[ j ] -= ( DrawPos[ j ].x - right ) * Vx ;
						u1[ j ] -= ( DrawPos[ j ].x - right ) * VBx ;
						DrawPos[ j ].x = right ;
					}	

					if( DrawPos[ j ].y < top )
					{
						v2[ j ] += ( top - DrawPos[ j ].y ) * Vy ;
						v1[ j ] += ( top - DrawPos[ j ].y ) * VBy ;
						DrawPos[ j ].y = top ;
					}
					else
					if( DrawPos[ j ].y > bottom )
					{
						v2[ j ] -= ( DrawPos[ j ].y - bottom ) * Vy ;
						v1[ j ] -= ( DrawPos[ j ].y - bottom ) * VBy ;
						DrawPos[ j ].y = bottom ;
					}
				}

				DrawVertB[0].pos.x = DrawPos[0].x ;
				DrawVertB[0].pos.y = DrawPos[0].y ;
				DrawVertB[1].pos.x = DrawPos[1].x ;
				DrawVertB[1].pos.y = DrawPos[1].y ;
				DrawVertB[2].pos.x = DrawPos[2].x ;
				DrawVertB[2].pos.y = DrawPos[2].y ;
				DrawVertB[3].pos.x = DrawPos[3].x ;
				DrawVertB[3].pos.y = DrawPos[3].y ;
				DrawVertB[4].pos.x = DrawPos[2].x ;
				DrawVertB[4].pos.y = DrawPos[2].y ;
				DrawVertB[5].pos.x = DrawPos[1].x ;
				DrawVertB[5].pos.y = DrawPos[1].y ;

				DrawVertB[0].u1 = u1[0] ;
				DrawVertB[0].v1 = v1[0] ;
				DrawVertB[1].u1 = u1[1] ;
				DrawVertB[1].v1 = v1[1] ;
				DrawVertB[2].u1 = u1[2] ;
				DrawVertB[2].v1 = v1[2] ;
				DrawVertB[3].u1 = u1[3] ;
				DrawVertB[3].v1 = v1[3] ;
				DrawVertB[4].u1 = u1[2] ;
				DrawVertB[4].v1 = v1[2] ;
				DrawVertB[5].u1 = u1[1] ;
				DrawVertB[5].v1 = v1[1] ;

				DrawVertB[0].u2 = u2[0] ;
				DrawVertB[0].v2 = v2[0] ;
				DrawVertB[1].u2 = u2[1] ;
				DrawVertB[1].v2 = v2[1] ;
				DrawVertB[2].u2 = u2[2] ;
				DrawVertB[2].v2 = v2[2] ;
				DrawVertB[3].u2 = u2[3] ;
				DrawVertB[3].v2 = v2[3] ;
				DrawVertB[4].u2 = u2[2] ;
				DrawVertB[4].v2 = v2[2] ;
				DrawVertB[5].u2 = u2[1] ;
				DrawVertB[5].v2 = v2[1] ;

				*((DWORD *)&DrawVertB[0].pos.z) = drawz ;
				*((DWORD *)&DrawVertB[1].pos.z) = drawz ;
				*((DWORD *)&DrawVertB[2].pos.z) = drawz ;
				*((DWORD *)&DrawVertB[3].pos.z) = drawz ;
				*((DWORD *)&DrawVertB[4].pos.z) = drawz ;
				*((DWORD *)&DrawVertB[5].pos.z) = drawz ;

				// �e�N�X�`���[��`�悷��
				ADD4VERTEX_BLENDTEX

				if( BlendGraphNoIncFlag == FALSE )
				{
					BlendDrawTex ++ ;
				}
			}
			break ;
		
		case VERTEXTYPE_TEX :
			// �u�����h�e�N�X�`�����g�p���Ȃ�

			// �e�N�X�`���[�̐������J��Ԃ�
			i = 0 ;
			for(;;)
			{
				VECTOR DrawPos[ 4 ] ;
				float u[ 4 ] ;
				float v[ 4 ] ;

				// ���_�o�b�t�@�̎擾
				GETVERTEX_QUAD( DrawVert )
				
				// ���_�f�[�^����
				TexVert = DrawTex->Vertex ;
				DrawVert[0].color	= DiffuseColor ;
				DrawVert[3].color	= DiffuseColor ;

				DrawPos[0].x = TexVert[0].x + xf ;
				DrawPos[0].y = TexVert[0].y + yf ;
				DrawPos[1].x = TexVert[1].x + xf ;
				DrawPos[1].y = TexVert[0].y + yf ;
				DrawPos[2].x = TexVert[0].x + xf ;
				DrawPos[2].y = TexVert[2].y + yf ;
				DrawPos[3].x = TexVert[1].x + xf ;
				DrawPos[3].y = TexVert[2].y + yf ;

				u[2] = u[0] = TexVert[0].u ;
				u[3] = u[1] = TexVert[1].u ;
				v[1] = v[0] = TexVert[0].v ;
				v[3] = v[2] = TexVert[2].v ;
				
				// ���_�f�[�^������������
				Vx	= 1.0F / ( float )DrawTex->Tex->TexWidth ;
				Vy	= 1.0F / ( float )DrawTex->Tex->TexHeight ;
				for( j = 0 ; j < 4 ; j ++ )
				{
					// �`���`����o�Ă����ꍇ�̕␳����
					if( DrawPos[ j ].x < left )
					{
						u[ j ] += ( left - DrawPos[ j ].x ) * Vx ;
						DrawPos[ j ].x = left ;
					}
					else
					if( DrawPos[ j ].x > right )
					{
						u[ j ] -= ( DrawPos[ j ].x - right ) * Vx ;
						DrawPos[ j ].x = right ;
					}	

					if( DrawPos[ j ].y < top )
					{
						v[ j ] += ( top - DrawPos[ j ].y ) * Vy ;
						DrawPos[ j ].y = top ;
					}
					else
					if( DrawPos[ j ].y > bottom )
					{
						v[ j ] -= ( DrawPos[ j ].y - bottom ) * Vy ;
						DrawPos[ j ].y = bottom ;
					}
				}

				DrawVert[0].pos.x = DrawPos[0].x ;
				DrawVert[0].pos.y = DrawPos[0].y ;
				DrawVert[1].pos.x = DrawPos[1].x ;
				DrawVert[1].pos.y = DrawPos[1].y ;
				DrawVert[2].pos.x = DrawPos[2].x ;
				DrawVert[2].pos.y = DrawPos[2].y ;
				DrawVert[3].pos.x = DrawPos[3].x ;
				DrawVert[3].pos.y = DrawPos[3].y ;
				DrawVert[4].pos.x = DrawPos[2].x ;
				DrawVert[4].pos.y = DrawPos[2].y ;
				DrawVert[5].pos.x = DrawPos[1].x ;
				DrawVert[5].pos.y = DrawPos[1].y ;

				DrawVert[0].u = u[0] ;
				DrawVert[0].v = v[0] ;
				DrawVert[1].u = u[1] ;
				DrawVert[1].v = v[1] ;
				DrawVert[2].u = u[2] ;
				DrawVert[2].v = v[2] ;
				DrawVert[3].u = u[3] ;
				DrawVert[3].v = v[3] ;
				DrawVert[4].u = u[2] ;
				DrawVert[4].v = v[2] ;
				DrawVert[5].u = u[1] ;
				DrawVert[5].v = v[1] ;

				*((DWORD *)&DrawVert[0].pos.z) = drawz ;
				*((DWORD *)&DrawVert[1].pos.z) = drawz ;
				*((DWORD *)&DrawVert[2].pos.z) = drawz ;
				*((DWORD *)&DrawVert[3].pos.z) = drawz ;
				*((DWORD *)&DrawVert[4].pos.z) = drawz ;
				*((DWORD *)&DrawVert[5].pos.z) = drawz ;

				// �e�N�X�`���[��`�悷��
				ADD4VERTEX_TEX

				i ++ ;
				if( i >= DrawTexNum ) break ;

				// �`�悷��e�N�X�`���[�̃Z�b�g
				DrawTex ++ ;
				Graphics_D3D9_DrawSetting_SetTexture( DrawTex->Tex->PF->D3D9.Texture ) ;
				Graphics_D3D9_DeviceState_RefreshBlendState() ;
			}
			break ;
		}
	}

	// �I��
	return 0 ;
}

// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawExtendGraph
extern	int		Graphics_D3D9_DrawExtendGraph( int x1, int y1, int x2, int y2, float x1f, float y1f, float x2f, float y2f, IMAGEDATA *Image, IMAGEDATA *BlendImage, int TransFlag, int IntFlag )
{
	VERTEX_2D *DrawVert ;
	VERTEX_BLENDTEX_2D *DrawVertB ; 
	IMAGEDATA_HARD_DRAW *DrawTex ;
	IMAGEDATA_HARD_DRAW *BlendDrawTex = NULL ;
	IMAGEDATA_HARD_VERT *TexVert ;
	IMAGEDATA_HARD_VERT *BlendTexVert ;
	IMAGEDATA_ORIG *Orig ;
	int DrawTexNum ;
	int i, j ;
	int BlendGraphNoIncFlag ;
	float GraphExRateX, GraphExRateY ;
//	int ReverseX = FALSE, ReverseY = FALSE ;
	int Reverse ;
//	int AdjX = FALSE, AdjY = FALSE ;
	float left, right, top, bottom ;
	float Vx, Vy ;
	float VBx, VBy ;
	DWORD DiffuseColor ;
	DWORD drawz ;
	int Flag ;
	float XBuf[ 2 ] ;
	float YBuf[ 2 ] ;
	float u[ 2 ] ;
	float v[ 2 ] ;

	Orig = Image->Orig ;

	if( GAPIWin.Direct3DDevice9Object == NULL ) return -1 ;

	// �`�攻��
//	if( x2 - x1 == Image->Width && y2 - y1 == Image->Height ) return G_D3D_DrawGraph( x1, y1, GraphData, TransFlag ) ; 	

	// �����Ȃ������ꍇ�̓G���[
	if( x1 == x2 || y1 == y2 ) return -1 ;

	// �`��̏���
	if( GD3D9.Device.DrawInfo.BeginSceneFlag == FALSE ) Graphics_D3D9_BeginScene() ;
	Flag = TransFlag | ( IntFlag ? ( GSYS.DrawSetting.DrawMode == DX_DRAWMODE_BILINEAR ? 0 : DX_D3D9_DRAWPREP_VECTORINT ) : 0 ) ;
	DX_D3D9_DRAWPREP_TEX( Orig, Image->Hard.Draw[ 0 ].Tex->PF->D3D9.Texture, Flag )

	// �f�B�t���[�Y�J���[�̎擾
	DiffuseColor = GD3D9.Device.DrawInfo.DiffuseColor ;

	// �`����̐����Z�b�g
	DrawTexNum = Image->Hard.DrawNum ;

	// �`����̐����u�����h�摜�ƈقȂ��Ă�����O�Ԗڂ̃e�N�X�`���������g�p����
	BlendGraphNoIncFlag = FALSE ;
	if( BlendImage != NULL && BlendImage->Hard.DrawNum != Image->Hard.DrawNum )
	{
		BlendGraphNoIncFlag = TRUE ;
	}

	// �`����z��̃A�h���X���Z�b�g���Ă���
	DrawTex = Image->Hard.Draw ;
	if( BlendImage != NULL )
	{
		BlendDrawTex = BlendImage->Hard.Draw ;
	}

	// �y�o�b�t�@�ɏ������ނy�l���Z�b�g����
	drawz = *((DWORD *)&GSYS.DrawSetting.DrawZ);

	// �g�嗦���擾���Ă���
	GraphExRateX = ( x2f - x1f ) / Image->WidthF ;
	GraphExRateY = ( y2f - y1f ) / Image->HeightF ;
	Reverse = ( GraphExRateX < 0 ) != ( GraphExRateY < 0 ) ;

	x1f -= 0.5F ;
	y1f -= 0.5F ;

	if( x1 > x2 ){ i = x2; x2 = x1; x1 = i; }
	if( y1 > y2 ){ i = y2; y2 = y1; y1 = i; }

	// �`��͈͂���͂ݏo�Ă��邩�ǂ����ŏ����𕪊�
	if( IN_DRAWAREA_CHECK( x1, y1, x2, y2 ) )
	{
		// ���_�^�C�v�ɂ���ď����𕪊�
		switch( GD3D9.Device.DrawInfo.VertexType )
		{
		case VERTEXTYPE_BLENDTEX :	// �u�����h�e�N�X�`�����g�p����
			// �e�N�X�`���[�̐������J��Ԃ�
			for( i = 0 ; i < DrawTexNum ; i ++, DrawTex ++ )
			{
				// �`�悷��e�N�X�`���[�̃Z�b�g
				Graphics_D3D9_DrawSetting_SetTexture( DrawTex->Tex->PF->D3D9.Texture ) ;
				Graphics_D3D9_DrawSetting_SetBlendTexture( BlendDrawTex->Tex->PF->D3D9.Texture, BlendDrawTex->Tex->TexWidth, BlendDrawTex->Tex->TexHeight );

				if( GD3D9.Device.DrawSetting.ChangeTextureFlag )
					Graphics_D3D9_DeviceState_RefreshBlendState() ;

				// ���_�f�[�^������������
				GETVERTEX_QUAD( DrawVertB )
				
				TexVert = DrawTex->Vertex ;
				BlendTexVert = BlendDrawTex->Vertex ;

				DrawVertB[0].color = ( DWORD )DiffuseColor ;
				DrawVertB[3].color = ( DWORD )DiffuseColor ;

				XBuf[ 0 ] = TexVert[0].x * GraphExRateX + x1f ;
				XBuf[ 1 ] = TexVert[1].x * GraphExRateX + x1f ;
				YBuf[ 0 ] = TexVert[0].y * GraphExRateY + y1f ;
				YBuf[ 1 ] = TexVert[2].y * GraphExRateY + y1f ;
				DrawVertB[0].pos.x = XBuf[ 0 ] ;
				DrawVertB[0].pos.y = YBuf[ 0 ] ;
				DrawVertB[1].pos.x = XBuf[ 1 ] ;
				DrawVertB[1].pos.y = YBuf[ 0 ] ;
				DrawVertB[2].pos.x = XBuf[ 0 ] ;
				DrawVertB[2].pos.y = YBuf[ 1 ] ;
				DrawVertB[3].pos.x = XBuf[ 1 ] ;
				DrawVertB[3].pos.y = YBuf[ 1 ] ;
				DrawVertB[4].pos.x = XBuf[ 0 ] ;
				DrawVertB[4].pos.y = YBuf[ 1 ] ;
				DrawVertB[5].pos.x = XBuf[ 1 ] ;
				DrawVertB[5].pos.y = YBuf[ 0 ] ;

				switch( GSYS.DrawSetting.BlendGraphPosMode )
				{
				default :
				case DX_BLENDGRAPH_POSMODE_DRAWGRAPH:
					u[ 0 ] = TexVert[0].x - BlendTexVert[0].x + GSYS.DrawSetting.BlendGraphX ;
					u[ 1 ] = TexVert[1].x - BlendTexVert[0].x + GSYS.DrawSetting.BlendGraphX ;
					v[ 0 ] = TexVert[0].y - BlendTexVert[0].y + GSYS.DrawSetting.BlendGraphY ;
					v[ 1 ] = TexVert[2].y - BlendTexVert[0].y + GSYS.DrawSetting.BlendGraphY ;
					break ;

				case DX_BLENDGRAPH_POSMODE_SCREEN :
					u[ 0 ] = XBuf[ 0 ] - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ;
					u[ 1 ] = XBuf[ 1 ] - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ;
					v[ 0 ] = YBuf[ 0 ] - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ;
					v[ 1 ] = YBuf[ 1 ] - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ;
					break ;
				}

				u[ 0 ] *= GD3D9.Device.DrawSetting.InvBlendTextureWidth ;
				u[ 1 ] *= GD3D9.Device.DrawSetting.InvBlendTextureWidth ;
				v[ 0 ] *= GD3D9.Device.DrawSetting.InvBlendTextureHeight ;
				v[ 1 ] *= GD3D9.Device.DrawSetting.InvBlendTextureHeight ;

				DrawVertB[0].u1 = u[ 0 ] ;
				DrawVertB[0].v1 = v[ 0 ] ;
				DrawVertB[1].u1 = u[ 1 ] ;
				DrawVertB[1].v1 = v[ 0 ] ;
				DrawVertB[2].u1 = u[ 0 ] ;
				DrawVertB[2].v1 = v[ 1 ] ;
				DrawVertB[3].u1 = u[ 1 ] ;
				DrawVertB[3].v1 = v[ 1 ] ;
				DrawVertB[4].u1 = u[ 0 ] ;
				DrawVertB[4].v1 = v[ 1 ] ;
				DrawVertB[5].u1 = u[ 1 ] ;
				DrawVertB[5].v1 = v[ 0 ] ;

				DrawVertB[0].u2 = TexVert[0].u ;
				DrawVertB[0].v2 = TexVert[0].v ;
				DrawVertB[1].u2 = TexVert[1].u ;
				DrawVertB[1].v2 = TexVert[0].v ;
				DrawVertB[2].u2 = TexVert[0].u ;
				DrawVertB[2].v2 = TexVert[2].v ;
				DrawVertB[3].u2 = TexVert[1].u ;
				DrawVertB[3].v2 = TexVert[2].v ;
				DrawVertB[4].u2 = TexVert[0].u ;
				DrawVertB[4].v2 = TexVert[2].v ;
				DrawVertB[5].u2 = TexVert[1].u ;
				DrawVertB[5].v2 = TexVert[0].v ;

				*((DWORD *)&DrawVertB[0].pos.z) = drawz ;
				*((DWORD *)&DrawVertB[1].pos.z) = drawz ;
				*((DWORD *)&DrawVertB[2].pos.z) = drawz ;
				*((DWORD *)&DrawVertB[3].pos.z) = drawz ;
				*((DWORD *)&DrawVertB[4].pos.z) = drawz ;
				*((DWORD *)&DrawVertB[5].pos.z) = drawz ;

				// �e�N�X�`���[��`�悷��
				ADD4VERTEX_BLENDTEX

				if( BlendGraphNoIncFlag == FALSE )
				{
					BlendDrawTex ++ ;
				}
			}
			break ;

		case VERTEXTYPE_TEX :	// �u�����h�e�N�X�`�����g�p���Ȃ�
			// �e�N�X�`���[�̐������J��Ԃ�
			i = 0 ;
			for(;;)
			{
				// ���_�f�[�^������������
				GETVERTEX_QUAD( DrawVert )
				
				TexVert = DrawTex->Vertex ;
				
				DrawVert[0].color = ( DWORD )DiffuseColor ;
				DrawVert[3].color = ( DWORD )DiffuseColor ;

				XBuf[ 0 ] = TexVert[0].x * GraphExRateX + x1f ;
				XBuf[ 1 ] = TexVert[1].x * GraphExRateX + x1f ;
				YBuf[ 0 ] = TexVert[0].y * GraphExRateY + y1f ;
				YBuf[ 1 ] = TexVert[2].y * GraphExRateY + y1f ;
				DrawVert[0].pos.x = XBuf[ 0 ] ;
				DrawVert[0].pos.y = YBuf[ 0 ] ;
				DrawVert[1].pos.x = XBuf[ 1 ] ;
				DrawVert[1].pos.y = YBuf[ 0 ] ;
				DrawVert[2].pos.x = XBuf[ 0 ] ;
				DrawVert[2].pos.y = YBuf[ 1 ] ;
				DrawVert[3].pos.x = XBuf[ 1 ] ;
				DrawVert[3].pos.y = YBuf[ 1 ] ;
				DrawVert[4].pos.x = XBuf[ 0 ] ;
				DrawVert[4].pos.y = YBuf[ 1 ] ;
				DrawVert[5].pos.x = XBuf[ 1 ] ;
				DrawVert[5].pos.y = YBuf[ 0 ] ;

				DrawVert[0].u = TexVert[0].u ;
				DrawVert[0].v = TexVert[0].v ;
				DrawVert[1].u = TexVert[1].u ;
				DrawVert[1].v = TexVert[0].v ;
				DrawVert[2].u = TexVert[0].u ;
				DrawVert[2].v = TexVert[2].v ;
				DrawVert[3].u = TexVert[1].u ;
				DrawVert[3].v = TexVert[2].v ;
				DrawVert[4].u = TexVert[0].u ;
				DrawVert[4].v = TexVert[2].v ;
				DrawVert[5].u = TexVert[1].u ;
				DrawVert[5].v = TexVert[0].v ;

				*((DWORD *)&DrawVert[0].pos.z) = drawz ;
				*((DWORD *)&DrawVert[1].pos.z) = drawz ;
				*((DWORD *)&DrawVert[2].pos.z) = drawz ;
				*((DWORD *)&DrawVert[3].pos.z) = drawz ;
				*((DWORD *)&DrawVert[4].pos.z) = drawz ;
				*((DWORD *)&DrawVert[5].pos.z) = drawz ;

				// �e�N�X�`���[��`�悷��
				ADD4VERTEX_TEX

				i ++ ;
				if( i >= DrawTexNum ) break ;

				// �`�悷��e�N�X�`���[�̃Z�b�g
				DrawTex ++ ;
				Graphics_D3D9_DrawSetting_SetTexture( DrawTex->Tex->PF->D3D9.Texture ) ;
				Graphics_D3D9_DeviceState_RefreshBlendState() ;
			}
			break ;
		}
	}
	else
	{
		// ���S�ɂ͂ݏo�Ă����牽�������I��
		if( OUT_DRAWAREA_CHECK( x1, y1, x2, y2 ) ) return 0 ;

		// �`���`�̕ۑ�
		left	= GSYS.DrawSetting.DrawAreaF.left   - 0.5f /* + GSYS.WindowDrawRect.left */ ;
		top		= GSYS.DrawSetting.DrawAreaF.top    - 0.5f /* + GSYS.WindowDrawRect.top  */ ;
		right	= GSYS.DrawSetting.DrawAreaF.right  - 0.5f /* + GSYS.WindowDrawRect.left */ ;
		bottom	= GSYS.DrawSetting.DrawAreaF.bottom - 0.5f /* + GSYS.WindowDrawRect.top  */ ;

		// ���_�^�C�v�ɂ���ď����𕪊�
		switch( GD3D9.Device.DrawInfo.VertexType )
		{
		case VERTEXTYPE_BLENDTEX :	// �u�����h�e�N�X�`�����g�p����
			// �e�N�X�`���[�̐������J��Ԃ�
			for( i = 0 ; i < DrawTexNum ; i ++, DrawTex ++ )
			{
				VECTOR DrawPos[ 4 ] ;
				float u1[ 4 ] ;
				float v1[ 4 ] ;
				float u2[ 4 ] ;
				float v2[ 4 ] ;

				// �`�悷��e�N�X�`���[�̃Z�b�g
				Graphics_D3D9_DrawSetting_SetTexture( DrawTex->Tex->PF->D3D9.Texture ) ;
				Graphics_D3D9_DrawSetting_SetBlendTexture( BlendDrawTex->Tex->PF->D3D9.Texture, BlendDrawTex->Tex->TexWidth, BlendDrawTex->Tex->TexHeight );

				if( GD3D9.Device.DrawSetting.ChangeTextureFlag )
					Graphics_D3D9_DeviceState_RefreshBlendState() ;

				GETVERTEX_QUAD( DrawVertB )
				
				Vx = 1.0F / ( DrawTex->Tex->TexWidth * GraphExRateX ) ;
				Vy = 1.0F / ( DrawTex->Tex->TexHeight * GraphExRateY ) ;
				VBx = GD3D9.Device.DrawSetting.InvBlendTextureWidth ;
				VBy = GD3D9.Device.DrawSetting.InvBlendTextureHeight ;

				// ���_�f�[�^������������
				TexVert = DrawTex->Vertex ;
				BlendTexVert = BlendDrawTex->Vertex ;

				DrawVertB[0].color = ( DWORD )DiffuseColor ;
				DrawVertB[3].color = ( DWORD )DiffuseColor ;

				XBuf[ 0 ] = TexVert[0].x * GraphExRateX + x1f ;
				XBuf[ 1 ] = TexVert[1].x * GraphExRateX + x1f ;
				YBuf[ 0 ] = TexVert[0].y * GraphExRateY + y1f ;
				YBuf[ 1 ] = TexVert[2].y * GraphExRateY + y1f ;
				DrawPos[0].x = XBuf[ 0 ] ;
				DrawPos[0].y = YBuf[ 0 ] ;
				DrawPos[1].x = XBuf[ 1 ] ;
				DrawPos[1].y = YBuf[ 0 ] ;
				DrawPos[2].x = XBuf[ 0 ] ;
				DrawPos[2].y = YBuf[ 1 ] ;
				DrawPos[3].x = XBuf[ 1 ] ;
				DrawPos[3].y = YBuf[ 1 ] ;

				switch( GSYS.DrawSetting.BlendGraphPosMode )
				{
				default :
				case DX_BLENDGRAPH_POSMODE_DRAWGRAPH:
					u1[2] = u1[0] = ( TexVert[0].x - BlendTexVert[0].x + GSYS.DrawSetting.BlendGraphX ) * GD3D9.Device.DrawSetting.InvBlendTextureWidth ;
					u1[3] = u1[1] = ( TexVert[1].x - BlendTexVert[0].x + GSYS.DrawSetting.BlendGraphX ) * GD3D9.Device.DrawSetting.InvBlendTextureWidth ;
					v1[1] = v1[0] = ( TexVert[0].y - BlendTexVert[0].y + GSYS.DrawSetting.BlendGraphY ) * GD3D9.Device.DrawSetting.InvBlendTextureHeight ;
					v1[3] = v1[2] = ( TexVert[2].y - BlendTexVert[0].y + GSYS.DrawSetting.BlendGraphY ) * GD3D9.Device.DrawSetting.InvBlendTextureHeight ;
					break ;

				case DX_BLENDGRAPH_POSMODE_SCREEN :
					u1[2] = u1[0] = ( XBuf[ 0 ] - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D9.Device.DrawSetting.InvBlendTextureWidth ;
					u1[3] = u1[1] = ( XBuf[ 1 ] - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D9.Device.DrawSetting.InvBlendTextureWidth ;
					v1[1] = v1[0] = ( YBuf[ 0 ] - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D9.Device.DrawSetting.InvBlendTextureHeight ;
					v1[3] = v1[2] = ( YBuf[ 1 ] - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D9.Device.DrawSetting.InvBlendTextureHeight ;
					break ;
				}

				u2[2] = u2[0] = TexVert[0].u ;
				u2[3] = u2[1] = TexVert[1].u ;
				v2[1] = v2[0] = TexVert[0].v ;
				v2[3] = v2[2] = TexVert[2].v ;

				for( j = 0 ; j < 4 ; j ++ )
				{
					// �`���`����o�Ă����ꍇ�̕␳����
					if( DrawPos[ j ].x < left )
					{
						u2[ j ] += ( left - DrawPos[ j ].x ) * Vx ;
						u1[ j ] += ( left - DrawPos[ j ].x ) * VBx ;
						DrawPos[ j ].x = left ;
					}
					else
					if( DrawPos[ j ].x > right )
					{
						u2[ j ] -= ( DrawPos[ j ].x - right ) * Vx ;
						u1[ j ] -= ( DrawPos[ j ].x - right ) * VBx ;
						DrawPos[ j ].x = right ;
					}	

					if( DrawPos[ j ].y < top )
					{
						v2[ j ] += ( top - DrawPos[ j ].y ) * Vy ;
						v1[ j ] += ( top - DrawPos[ j ].y ) * VBy ;
						DrawPos[ j ].y = top ;
					}
					else
					if( DrawPos[ j ].y > bottom )
					{
						v2[ j ] -= ( DrawPos[ j ].y - bottom ) * Vy ;
						v1[ j ] -= ( DrawPos[ j ].y - bottom ) * VBy ;
						DrawPos[ j ].y = bottom ;
					}
				}

				DrawVertB[0].pos.x = DrawPos[0].x ;
				DrawVertB[0].pos.y = DrawPos[0].y ;
				DrawVertB[1].pos.x = DrawPos[1].x ;
				DrawVertB[1].pos.y = DrawPos[1].y ;
				DrawVertB[2].pos.x = DrawPos[2].x ;
				DrawVertB[2].pos.y = DrawPos[2].y ;
				DrawVertB[3].pos.x = DrawPos[3].x ;
				DrawVertB[3].pos.y = DrawPos[3].y ;
				DrawVertB[4].pos.x = DrawPos[2].x ;
				DrawVertB[4].pos.y = DrawPos[2].y ;
				DrawVertB[5].pos.x = DrawPos[1].x ;
				DrawVertB[5].pos.y = DrawPos[1].y ;

				DrawVertB[0].u1 = u1[0] ;
				DrawVertB[0].v1 = v1[0] ;
				DrawVertB[1].u1 = u1[1] ;
				DrawVertB[1].v1 = v1[1] ;
				DrawVertB[2].u1 = u1[2] ;
				DrawVertB[2].v1 = v1[2] ;
				DrawVertB[3].u1 = u1[3] ;
				DrawVertB[3].v1 = v1[3] ;
				DrawVertB[4].u1 = u1[2] ;
				DrawVertB[4].v1 = v1[2] ;
				DrawVertB[5].u1 = u1[1] ;
				DrawVertB[5].v1 = v1[1] ;

				DrawVertB[0].u2 = u2[0] ;
				DrawVertB[0].v2 = v2[0] ;
				DrawVertB[1].u2 = u2[1] ;
				DrawVertB[1].v2 = v2[1] ;
				DrawVertB[2].u2 = u2[2] ;
				DrawVertB[2].v2 = v2[2] ;
				DrawVertB[3].u2 = u2[3] ;
				DrawVertB[3].v2 = v2[3] ;
				DrawVertB[4].u2 = u2[2] ;
				DrawVertB[4].v2 = v2[2] ;
				DrawVertB[5].u2 = u2[1] ;
				DrawVertB[5].v2 = v2[1] ;

				*((DWORD *)&DrawVertB[0].pos.z) = drawz ;
				*((DWORD *)&DrawVertB[1].pos.z) = drawz ;
				*((DWORD *)&DrawVertB[2].pos.z) = drawz ;
				*((DWORD *)&DrawVertB[3].pos.z) = drawz ;
				*((DWORD *)&DrawVertB[4].pos.z) = drawz ;
				*((DWORD *)&DrawVertB[5].pos.z) = drawz ;

				// �e�N�X�`���[��`�悷��
				ADD4VERTEX_BLENDTEX

				if( BlendGraphNoIncFlag == FALSE )
				{
					BlendDrawTex ++ ;
				}
			}
			break ;

		case VERTEXTYPE_TEX :	// �u�����h�e�N�X�`�����g�p���Ȃ�
			// �e�N�X�`���[�̐������J��Ԃ�
			i = 0 ;
			for(;;)
			{
				VECTOR DrawPos[ 4 ] ;
				float u[ 4 ] ;
				float v[ 4 ] ;

				// ���_�f�[�^������������
				GETVERTEX_QUAD( DrawVert )
				
				TexVert = DrawTex->Vertex ;

				DrawVert[0].color = ( DWORD )DiffuseColor ;
				DrawVert[3].color = ( DWORD )DiffuseColor ;

				XBuf[ 0 ] = TexVert[0].x * GraphExRateX + x1f ;
				XBuf[ 1 ] = TexVert[1].x * GraphExRateX + x1f ;
				YBuf[ 0 ] = TexVert[0].y * GraphExRateY + y1f ;
				YBuf[ 1 ] = TexVert[2].y * GraphExRateY + y1f ;
				DrawPos[0].x = XBuf[ 0 ] ;
				DrawPos[0].y = YBuf[ 0 ] ;
				DrawPos[1].x = XBuf[ 1 ] ;
				DrawPos[1].y = YBuf[ 0 ] ;
				DrawPos[2].x = XBuf[ 0 ] ;
				DrawPos[2].y = YBuf[ 1 ] ;
				DrawPos[3].x = XBuf[ 1 ] ;
				DrawPos[3].y = YBuf[ 1 ] ;

				u[2] = u[0] = TexVert[0].u ;
				u[3] = u[1] = TexVert[1].u ;
				v[1] = v[0] = TexVert[0].v ;
				v[3] = v[2] = TexVert[2].v ;

				Vx = 1.0F / ( DrawTex->Tex->TexWidth * GraphExRateX ) ;
				Vy = 1.0F / ( DrawTex->Tex->TexHeight * GraphExRateY ) ;
				for( j = 0 ; j < 4 ; j ++ )
				{
					// �`���`����o�Ă����ꍇ�̕␳����
					if( DrawPos[ j ].x < left )
					{
						u[ j ] += ( left - DrawPos[ j ].x ) * Vx ;
						DrawPos[ j ].x = left ;
					}
					else
					if( DrawPos[ j ].x > right )
					{
						u[ j ] -= ( DrawPos[ j ].x - right ) * Vx ;
						DrawPos[ j ].x = right ;
					}	

					if( DrawPos[ j ].y < top )
					{
						v[ j ] += ( top - DrawPos[ j ].y ) * Vy ;
						DrawPos[ j ].y = top ;
					}
					else
					if( DrawPos[ j ].y > bottom )
					{
						v[ j ] -= ( DrawPos[ j ].y - bottom ) * Vy ;
						DrawPos[ j ].y = bottom ;
					}
				}

				DrawVert[0].pos.x = DrawPos[0].x ;
				DrawVert[0].pos.y = DrawPos[0].y ;
				DrawVert[1].pos.x = DrawPos[1].x ;
				DrawVert[1].pos.y = DrawPos[1].y ;
				DrawVert[2].pos.x = DrawPos[2].x ;
				DrawVert[2].pos.y = DrawPos[2].y ;
				DrawVert[3].pos.x = DrawPos[3].x ;
				DrawVert[3].pos.y = DrawPos[3].y ;
				DrawVert[4].pos.x = DrawPos[2].x ;
				DrawVert[4].pos.y = DrawPos[2].y ;
				DrawVert[5].pos.x = DrawPos[1].x ;
				DrawVert[5].pos.y = DrawPos[1].y ;

				DrawVert[0].u = u[0] ;
				DrawVert[0].v = v[0] ;
				DrawVert[1].u = u[1] ;
				DrawVert[1].v = v[1] ;
				DrawVert[2].u = u[2] ;
				DrawVert[2].v = v[2] ;
				DrawVert[3].u = u[3] ;
				DrawVert[3].v = v[3] ;
				DrawVert[4].u = u[2] ;
				DrawVert[4].v = v[2] ;
				DrawVert[5].u = u[1] ;
				DrawVert[5].v = v[1] ;

				*((DWORD *)&DrawVert[0].pos.z) = drawz ;
				*((DWORD *)&DrawVert[1].pos.z) = drawz ;
				*((DWORD *)&DrawVert[2].pos.z) = drawz ;
				*((DWORD *)&DrawVert[3].pos.z) = drawz ;
				*((DWORD *)&DrawVert[4].pos.z) = drawz ;
				*((DWORD *)&DrawVert[5].pos.z) = drawz ;

				// �e�N�X�`���[��`�悷��
				ADD4VERTEX_TEX

				i ++ ;
				if( i >= DrawTexNum ) break ;

				// �`�悷��e�N�X�`���[�̃Z�b�g
				DrawTex ++ ;
				Graphics_D3D9_DrawSetting_SetTexture( DrawTex->Tex->PF->D3D9.Texture ) ;
				Graphics_D3D9_DeviceState_RefreshBlendState() ;
			}
			break ;
		}
	}

	// �I��
	return 0 ;
}

// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawRotaGraph
extern	int		Graphics_D3D9_DrawRotaGraph( int x, int y, float xf, float yf, double ExRate, double Angle, IMAGEDATA *Image, IMAGEDATA *BlendImage, int TransFlag, int TurnFlag, int IntFlag )
{
	VERTEX_2D *DrawVert ;
	VERTEX_BLENDTEX_2D *DrawVertB ;
	VECTOR DrawPos[ 4 ] ;
	DWORD DiffuseColor ;
	IMAGEDATA_HARD_DRAW *DrawTex ;
	IMAGEDATA_HARD_DRAW *BlendDrawTex = NULL ;
	IMAGEDATA_HARD_VERT *TexVert ;
	IMAGEDATA_HARD_VERT *BlendTexVert ;
	int DrawTexNum ;
	int i ; 
	float Sin = 0.0f, Cos = 1.0f ;
	int BlendGraphNoIncFlag ;
	float XBuf[ 8 ], YBuf[ 8 ] ;
	float u[ 4 ], v[ 4 ] ;
	float ExtendRate = ( float )ExRate ; 
	float ExtendRateX ;
	DWORD drawz ;
//	BOOL Adj = FALSE ;
	float CenX, CenY ;
	float fx, fy ;
	int hx = 0, hy = 0 ;
	int FastCode, SinCosNone, AngleCheck, AlwaysCheck ;
	int Flag ;

	if( GAPIWin.Direct3DDevice9Object == NULL ) return -1 ;

	// �����Ȃ������ꍇ�̓G���[
	if( ExRate == 0.0 ) return -1 ;

	// �`��̏���
	if( GD3D9.Device.DrawInfo.BeginSceneFlag == FALSE ) Graphics_D3D9_BeginScene() ;
	Flag = TransFlag ;
	DX_D3D9_DRAWPREP_TEX( Image->Orig, Image->Hard.Draw[ 0 ].Tex->PF->D3D9.Texture, Flag )

	// ��]�v�f�����������ׂ�
	if( Angle == 0.0 )
	{
		SinCosNone = 1;
	}
	else
	{
		SinCosNone = 0;
		_SINCOS( (float)Angle, &Sin, &Cos ) ;
	}
//	if( _FABS( (float)Angle ) < 0.0001 && ExRate < 1.001 && ExRate > 0.999 ) Adj = TRUE ;

	// ���W�̕␳
	if( IntFlag || GSYS.DrawSetting.DrawMode == DX_DRAWMODE_NEAREST )
	{
		AlwaysCheck = 0 ;
		FastCode    = 0 ;
		hx          = Image->WidthI % 2 ;
		hy          = Image->HeightI % 2 ;
		fx = xf - ( float )x ;
		fy = yf - ( float )y ;
		if( fx < 0.0f ) fx = -fx;
		if( fy < 0.0f ) fy = -fy;
		if( fx < 0.00001f && fy < 0.00001f )
		{
			if( ExRate == 1.0 )
			{
				if( hx == hy )
				{
					if( hx == 0 )
					{
						CenX = ( float )(Image->WidthI >> 1) ;
						CenY = ( float )(Image->HeightI >> 1) ;
						xf   = ( float )x - 0.5F ;
						yf   = ( float )y - 0.5F ;
					}
					else
					{
						CenX = ( float )Image->WidthI * 0.5F ;
						CenY = ( float )Image->HeightI * 0.5F ;
						xf   = ( float )x ;
						yf   = ( float )y ;
						FastCode = 1;
					}
				}
				else
				{
					CenX = ( float )Image->WidthI * 0.5F ;
					CenY = ( float )Image->HeightI * 0.5F ;
					xf   = ( float )x ;
					yf   = ( float )y ;
						
					if( hx == 0 ) CenX -= 0.5F ;
					if( hy == 0 ) CenY -= 0.5F ;
				}
			}
			else
			{
				CenX = ( float )Image->WidthI * 0.5F ;
				CenY = ( float )Image->HeightI * 0.5F ;
				xf   = ( float )x - 0.5F ;
				yf   = ( float )y - 0.5F ;
				FastCode = 1 ;
			}
		}
		else
		{
			CenX = ( float )Image->WidthI * 0.5F ;
			CenY = ( float )Image->HeightI * 0.5F ;
			xf   = ( float )x ;
			yf   = ( float )y ;
			AlwaysCheck = 1 ;
		}
	}
	else
	{
		CenX = ( float )Image->WidthF * 0.5F ;
		CenY = ( float )Image->HeightF * 0.5F ;
		xf -= 0.5F ;
		yf -= 0.5F ;
		AngleCheck = 0 ;
		AlwaysCheck = 0 ;
		FastCode = 1 ;
	}

	// �����ȉ�]�l�̃`�F�b�N���s�����ǂ����𒲂ׂ�
	if( IntFlag == TRUE && AlwaysCheck == 0 && SinCosNone == 0 && ( hx == 1 || hy == 1 ) &&
		( ( Angle < DX_PI / 180.0 *         - 4.0   ) ||
		  ( Angle > DX_PI / 180.0 * ( 360.0 + 4.0 ) ) ||
		  ( Angle > DX_PI / 180.0 *         - 4.0   && Angle < DX_PI / 180.0 *           4.0   ) ||
		  ( Angle > DX_PI / 180.0 * (  90.0 - 4.0 ) && Angle < DX_PI / 180.0 * (  90.0 + 4.0 ) ) ||
		  ( Angle > DX_PI / 180.0 * ( 180.0 - 4.0 ) && Angle < DX_PI / 180.0 * ( 180.0 + 4.0 ) ) ||
		  ( Angle > DX_PI / 180.0 * ( 270.0 - 4.0 ) && Angle < DX_PI / 180.0 * ( 270.0 + 4.0 ) ) ||
		  ( Angle > DX_PI / 180.0 * ( 360.0 - 4.0 ) && Angle < DX_PI / 180.0 * ( 360.0 + 4.0 ) ) ) )
	{
		AngleCheck = 1 ;
	}
	else
	{
		AngleCheck = 0 ;
	}

	// ���_�f�[�^���擾�i���̂Ƃ���F���̃Z�b�g�̂݁j
	DiffuseColor = GD3D9.Device.DrawInfo.DiffuseColor ;

	// �`����̐����Z�b�g
	DrawTexNum = Image->Hard.DrawNum ;

	// �`����̐����u�����h�摜�ƈقȂ��Ă�����O�Ԗڂ̃e�N�X�`���������g�p����
	BlendGraphNoIncFlag = FALSE ;
	if( BlendImage != NULL && BlendImage->Hard.DrawNum != Image->Hard.DrawNum )
	{
		BlendGraphNoIncFlag = TRUE ;
	}

	// �`����z��̃A�h���X���Z�b�g���Ă���
	DrawTex = Image->Hard.Draw ;
	if( BlendImage != NULL )
	{
		BlendDrawTex = BlendImage->Hard.Draw ;
	}

	// �y�o�b�t�@�ɏ������ނy�l���Z�b�g����
	drawz = *((DWORD *)&GSYS.DrawSetting.DrawZ);

	// ���]���l�������������̊g�嗦���Z�b�g
	ExtendRateX = ( ( TurnFlag == TRUE ) ? -1.0f : 1.0f ) * ExtendRate ;

	// ���_�^�C�v�ɂ���ď����𕪊�
	switch( GD3D9.Device.DrawInfo.VertexType )
	{
	case VERTEXTYPE_BLENDTEX :	// �u�����h�e�N�X�`�����g�p����
		// �e�N�X�`���[�̐������J��Ԃ�
		if( DrawTexNum == 1 && FastCode == 1 )
		{
			// �`�悷��e�N�X�`���[�̃Z�b�g
			Graphics_D3D9_DrawSetting_SetTexture( DrawTex->Tex->PF->D3D9.Texture ) ;
			Graphics_D3D9_DrawSetting_SetBlendTexture( BlendDrawTex->Tex->PF->D3D9.Texture, BlendDrawTex->Tex->TexWidth, BlendDrawTex->Tex->TexHeight );

			if( GD3D9.Device.DrawSetting.ChangeTextureFlag )
				Graphics_D3D9_DeviceState_RefreshBlendState() ;

			// ���_�f�[�^������������
			GETVERTEX_QUAD( DrawVertB )

			TexVert = DrawTex->Vertex ;
			BlendTexVert = BlendDrawTex->Vertex ;

			DrawVertB[0].color = ( DWORD )DiffuseColor ;
			DrawVertB[3].color = ( DWORD )DiffuseColor ;

			XBuf[0] = ( TexVert[0].x - CenX ) * ExtendRateX ;
			YBuf[0] = ( TexVert[0].y - CenY ) * ExtendRate ;
			XBuf[1] = XBuf[0] * Cos - YBuf[0] * Sin ;
			YBuf[1] = XBuf[0] * Sin + YBuf[0] * Cos ;

			XBuf[2] = ( TexVert[1].x - CenX ) * ExtendRateX ;
			YBuf[2] = ( TexVert[1].y - CenY ) * ExtendRate ;
			XBuf[3] = XBuf[2] * Cos - YBuf[2] * Sin ;
			YBuf[3] = XBuf[2] * Sin + YBuf[2] * Cos ;

			DrawVertB[0].pos.x =  XBuf[1] + xf ;
			DrawVertB[0].pos.y =  YBuf[1] + yf ;
			DrawVertB[1].pos.x =  XBuf[3] + xf ;
			DrawVertB[1].pos.y =  YBuf[3] + yf ;
			DrawVertB[2].pos.x = -XBuf[3] + xf ;
			DrawVertB[2].pos.y = -YBuf[3] + yf ;
			DrawVertB[3].pos.x = -XBuf[1] + xf ;
			DrawVertB[3].pos.y = -YBuf[1] + yf ;
			DrawVertB[4].pos.x = -XBuf[3] + xf ;
			DrawVertB[4].pos.y = -YBuf[3] + yf ;
			DrawVertB[5].pos.x =  XBuf[3] + xf ;
			DrawVertB[5].pos.y =  YBuf[3] + yf ;

			switch( GSYS.DrawSetting.BlendGraphPosMode )
			{
			default :
			case DX_BLENDGRAPH_POSMODE_DRAWGRAPH:
				u[ 0 ] = TexVert[0].x - BlendTexVert[0].x + GSYS.DrawSetting.BlendGraphX * GD3D9.Device.DrawSetting.InvBlendTextureWidth ;
				u[ 1 ] = TexVert[1].x - BlendTexVert[0].x + GSYS.DrawSetting.BlendGraphX * GD3D9.Device.DrawSetting.InvBlendTextureWidth ;
				v[ 0 ] = TexVert[0].y - BlendTexVert[0].y + GSYS.DrawSetting.BlendGraphY * GD3D9.Device.DrawSetting.InvBlendTextureHeight ;
				v[ 1 ] = TexVert[2].y - BlendTexVert[0].y + GSYS.DrawSetting.BlendGraphY * GD3D9.Device.DrawSetting.InvBlendTextureHeight ;
				DrawVertB[0].u1 = u[ 0 ] ;
				DrawVertB[0].v1 = v[ 0 ] ;
				DrawVertB[1].u1 = u[ 1 ] ;
				DrawVertB[1].v1 = v[ 0 ] ;
				DrawVertB[2].u1 = u[ 0 ] ;
				DrawVertB[2].v1 = v[ 1 ] ;
				DrawVertB[3].u1 = u[ 1 ] ;
				DrawVertB[3].v1 = v[ 1 ] ;
				DrawVertB[4].u1 = u[ 0 ] ;
				DrawVertB[4].v1 = v[ 1 ] ;
				DrawVertB[5].u1 = u[ 1 ] ;
				DrawVertB[5].v1 = v[ 0 ] ;
				break ;

			case DX_BLENDGRAPH_POSMODE_SCREEN :
				DrawVertB[0].u1 = ( (  XBuf[1] + xf ) - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D9.Device.DrawSetting.InvBlendTextureWidth ;
				DrawVertB[0].v1 = ( (  YBuf[1] + yf ) - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D9.Device.DrawSetting.InvBlendTextureHeight ;
				DrawVertB[1].u1 = ( (  XBuf[3] + xf ) - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D9.Device.DrawSetting.InvBlendTextureWidth ;
				DrawVertB[1].v1 = ( (  YBuf[3] + yf ) - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D9.Device.DrawSetting.InvBlendTextureHeight ;
				DrawVertB[2].u1 = ( ( -XBuf[3] + xf ) - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D9.Device.DrawSetting.InvBlendTextureWidth ;
				DrawVertB[2].v1 = ( ( -YBuf[3] + yf ) - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D9.Device.DrawSetting.InvBlendTextureHeight ;
				DrawVertB[3].u1 = ( ( -XBuf[1] + xf ) - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D9.Device.DrawSetting.InvBlendTextureWidth ;
				DrawVertB[3].v1 = ( ( -YBuf[1] + yf ) - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D9.Device.DrawSetting.InvBlendTextureHeight ;
				DrawVertB[4].u1 = ( ( -XBuf[3] + xf ) - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D9.Device.DrawSetting.InvBlendTextureWidth ;
				DrawVertB[4].v1 = ( ( -YBuf[3] + yf ) - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D9.Device.DrawSetting.InvBlendTextureHeight ;
				DrawVertB[5].u1 = ( (  XBuf[3] + xf ) - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D9.Device.DrawSetting.InvBlendTextureWidth ;
				DrawVertB[5].v1 = ( (  YBuf[3] + yf ) - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D9.Device.DrawSetting.InvBlendTextureHeight ;
				break ;
			}

			DrawVertB[0].u2 = TexVert[0].u ;
			DrawVertB[0].v2 = TexVert[0].v ;
			DrawVertB[1].u2 = TexVert[1].u ;
			DrawVertB[1].v2 = TexVert[0].v ;
			DrawVertB[2].u2 = TexVert[0].u ;
			DrawVertB[2].v2 = TexVert[2].v ;
			DrawVertB[3].u2 = TexVert[1].u ;
			DrawVertB[3].v2 = TexVert[2].v ;
			DrawVertB[4].u2 = TexVert[0].u ;
			DrawVertB[4].v2 = TexVert[2].v ;
			DrawVertB[5].u2 = TexVert[1].u ;
			DrawVertB[5].v2 = TexVert[0].v ;

			*((DWORD *)&DrawVertB[0].pos.z) = drawz ;
			*((DWORD *)&DrawVertB[1].pos.z) = drawz ;
			*((DWORD *)&DrawVertB[2].pos.z) = drawz ;
			*((DWORD *)&DrawVertB[3].pos.z) = drawz ;
			*((DWORD *)&DrawVertB[4].pos.z) = drawz ;
			*((DWORD *)&DrawVertB[5].pos.z) = drawz ;

			// �e�N�X�`���[��`�悷��
			ADD4VERTEX_BLENDTEX

			if( BlendGraphNoIncFlag == FALSE )
			{
				BlendDrawTex ++ ;
			}
		}
		else
		{
			// �e�N�X�`���[�̐������J��Ԃ�
			for( i = 0 ; i < DrawTexNum ; i ++, DrawTex ++ )
			{
				// �`�悷��e�N�X�`���[�̃Z�b�g
				Graphics_D3D9_DrawSetting_SetTexture( DrawTex->Tex->PF->D3D9.Texture ) ;
				Graphics_D3D9_DrawSetting_SetBlendTexture( BlendDrawTex->Tex->PF->D3D9.Texture, BlendDrawTex->Tex->TexWidth, BlendDrawTex->Tex->TexHeight );

				if( GD3D9.Device.DrawSetting.ChangeTextureFlag )
					Graphics_D3D9_DeviceState_RefreshBlendState() ;

				// ���_�f�[�^������������
				GETVERTEX_QUAD( DrawVertB )

				TexVert = DrawTex->Vertex ;
				BlendTexVert = BlendDrawTex->Vertex ;

				DrawVertB[0].color = ( DWORD )DiffuseColor ;
				DrawVertB[3].color = ( DWORD )DiffuseColor ;

				XBuf[0] = ( TexVert[0].x - CenX ) * ExtendRateX ;
				YBuf[0] = ( TexVert[0].y - CenY ) * ExtendRate ;
				XBuf[1] = XBuf[0] * Cos - YBuf[0] * Sin ;
				YBuf[1] = XBuf[0] * Sin + YBuf[0] * Cos ;

				XBuf[2] = ( TexVert[1].x - CenX ) * ExtendRateX ;
				YBuf[2] = ( TexVert[1].y - CenY ) * ExtendRate ;
				XBuf[3] = XBuf[2] * Cos - YBuf[2] * Sin ;
				YBuf[3] = XBuf[2] * Sin + YBuf[2] * Cos ;

				XBuf[4] = ( TexVert[2].x - CenX ) * ExtendRateX ;
				YBuf[4] = ( TexVert[2].y - CenY ) * ExtendRate ;
				XBuf[5] = XBuf[4] * Cos - YBuf[4] * Sin ;
				YBuf[5] = XBuf[4] * Sin + YBuf[4] * Cos ;

				XBuf[6] = ( TexVert[3].x - CenX ) * ExtendRateX ;
				YBuf[6] = ( TexVert[3].y - CenY ) * ExtendRate ;
				XBuf[7] = XBuf[6] * Cos - YBuf[6] * Sin ;
				YBuf[7] = XBuf[6] * Sin + YBuf[6] * Cos ;

				DrawVertB[0].pos.x = XBuf[1] + xf ;
				DrawVertB[0].pos.y = YBuf[1] + yf ;
				DrawVertB[1].pos.x = XBuf[3] + xf ;
				DrawVertB[1].pos.y = YBuf[3] + yf ;
				DrawVertB[2].pos.x = XBuf[5] + xf ;
				DrawVertB[2].pos.y = YBuf[5] + yf ;
				DrawVertB[3].pos.x = XBuf[7] + xf ;
				DrawVertB[3].pos.y = YBuf[7] + yf ;
				DrawVertB[4].pos.x = XBuf[5] + xf ;
				DrawVertB[4].pos.y = YBuf[5] + yf ;
				DrawVertB[5].pos.x = XBuf[3] + xf ;
				DrawVertB[5].pos.y = YBuf[3] + yf ;

				switch( GSYS.DrawSetting.BlendGraphPosMode )
				{
				default :
				case DX_BLENDGRAPH_POSMODE_DRAWGRAPH:
					u[ 0 ] = ( TexVert[0].x - BlendTexVert[0].x + GSYS.DrawSetting.BlendGraphX ) * GD3D9.Device.DrawSetting.InvBlendTextureWidth ;
					u[ 1 ] = ( TexVert[1].x - BlendTexVert[0].x + GSYS.DrawSetting.BlendGraphX ) * GD3D9.Device.DrawSetting.InvBlendTextureWidth ;
					u[ 2 ] = ( TexVert[2].x - BlendTexVert[0].x + GSYS.DrawSetting.BlendGraphX ) * GD3D9.Device.DrawSetting.InvBlendTextureWidth ;
					u[ 3 ] = ( TexVert[3].x - BlendTexVert[0].x + GSYS.DrawSetting.BlendGraphX ) * GD3D9.Device.DrawSetting.InvBlendTextureWidth ;
					
					v[ 0 ] = ( TexVert[0].y - BlendTexVert[0].y + GSYS.DrawSetting.BlendGraphY ) * GD3D9.Device.DrawSetting.InvBlendTextureHeight ;
					v[ 1 ] = ( TexVert[1].y - BlendTexVert[0].y + GSYS.DrawSetting.BlendGraphY ) * GD3D9.Device.DrawSetting.InvBlendTextureHeight ;
					v[ 2 ] = ( TexVert[2].y - BlendTexVert[0].y + GSYS.DrawSetting.BlendGraphY ) * GD3D9.Device.DrawSetting.InvBlendTextureHeight ;
					v[ 3 ] = ( TexVert[3].y - BlendTexVert[0].y + GSYS.DrawSetting.BlendGraphY ) * GD3D9.Device.DrawSetting.InvBlendTextureHeight ;

					DrawVertB[0].u1 = u[ 0 ] ;
					DrawVertB[0].v1 = v[ 0 ] ;
					DrawVertB[1].u1 = u[ 1 ] ;
					DrawVertB[1].v1 = v[ 1 ] ;
					DrawVertB[2].u1 = u[ 2 ] ;
					DrawVertB[2].v1 = v[ 2 ] ;
					DrawVertB[3].u1 = u[ 3 ] ;
					DrawVertB[3].v1 = v[ 3 ] ;
					DrawVertB[4].u1 = u[ 2 ] ;
					DrawVertB[4].v1 = v[ 2 ] ;
					DrawVertB[5].u1 = u[ 1 ] ;
					DrawVertB[5].v1 = v[ 1 ] ;
					break ;

				case DX_BLENDGRAPH_POSMODE_SCREEN :
					DrawVertB[0].u1 = ( ( XBuf[1] + xf ) - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D9.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[0].v1 = ( ( YBuf[1] + yf ) - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D9.Device.DrawSetting.InvBlendTextureHeight ;
					DrawVertB[1].u1 = ( ( XBuf[3] + xf ) - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D9.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[1].v1 = ( ( YBuf[3] + yf ) - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D9.Device.DrawSetting.InvBlendTextureHeight ;
					DrawVertB[2].u1 = ( ( XBuf[5] + xf ) - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D9.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[2].v1 = ( ( YBuf[5] + yf ) - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D9.Device.DrawSetting.InvBlendTextureHeight ;
					DrawVertB[3].u1 = ( ( XBuf[7] + xf ) - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D9.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[3].v1 = ( ( YBuf[7] + yf ) - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D9.Device.DrawSetting.InvBlendTextureHeight ;
					DrawVertB[4].u1 = ( ( XBuf[5] + xf ) - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D9.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[4].v1 = ( ( YBuf[5] + yf ) - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D9.Device.DrawSetting.InvBlendTextureHeight ;
					DrawVertB[5].u1 = ( ( XBuf[3] + xf ) - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D9.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[5].v1 = ( ( YBuf[3] + yf ) - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D9.Device.DrawSetting.InvBlendTextureHeight ;
					break ;
				}

				DrawVertB[0].u2 = TexVert[0].u ;
				DrawVertB[0].v2 = TexVert[0].v ;
				DrawVertB[1].u2 = TexVert[1].u ;
				DrawVertB[1].v2 = TexVert[0].v ;
				DrawVertB[2].u2 = TexVert[0].u ;
				DrawVertB[2].v2 = TexVert[2].v ;
				DrawVertB[3].u2 = TexVert[1].u ;
				DrawVertB[3].v2 = TexVert[2].v ;
				DrawVertB[4].u2 = TexVert[0].u ;
				DrawVertB[4].v2 = TexVert[2].v ;
				DrawVertB[5].u2 = TexVert[1].u ;
				DrawVertB[5].v2 = TexVert[0].v ;

				*((DWORD *)&DrawVertB[0].pos.z) = drawz ;
				*((DWORD *)&DrawVertB[1].pos.z) = drawz ;
				*((DWORD *)&DrawVertB[2].pos.z) = drawz ;
				*((DWORD *)&DrawVertB[3].pos.z) = drawz ;
				*((DWORD *)&DrawVertB[4].pos.z) = drawz ;
				*((DWORD *)&DrawVertB[5].pos.z) = drawz ;

				// �e�N�X�`���[��`�悷��
				ADD4VERTEX_BLENDTEX

				if( BlendGraphNoIncFlag == FALSE )
				{
					BlendDrawTex ++ ;
				}
			}
		}
		break ;

	case VERTEXTYPE_TEX :	// �u�����h�e�N�X�`�����g�p���Ȃ�
		// �e�N�X�`���[�̐������J��Ԃ�
		if( DrawTexNum == 1 && FastCode == 1 )
		{
			if( SinCosNone == 0 )
			{
				// ���_�f�[�^������������
				GETVERTEX_QUAD( DrawVert )
				
				TexVert = DrawTex->Vertex ;

				DrawVert[0].color = ( DWORD )DiffuseColor ;
				DrawVert[3].color = ( DWORD )DiffuseColor ;

				DrawVert[0].u = TexVert[0].u ;
				DrawVert[0].v = TexVert[0].v ;
				DrawVert[1].u = TexVert[1].u ;
				DrawVert[1].v = TexVert[0].v ;
				DrawVert[2].v = TexVert[2].v ;
				DrawVert[2].u = TexVert[0].u ;
				DrawVert[3].u = TexVert[1].u ;
				DrawVert[3].v = TexVert[2].v ;
				DrawVert[4].u = TexVert[0].u ;
				DrawVert[4].v = TexVert[2].v ;
				DrawVert[5].u = TexVert[1].u ;
				DrawVert[5].v = TexVert[0].v ;

				XBuf[0] = ( TexVert[0].x - CenX ) * ExtendRateX ;
				YBuf[0] = ( TexVert[0].y - CenY ) * ExtendRate  ;
				XBuf[1] = XBuf[0] * Cos - YBuf[0] * Sin ;
				YBuf[1] = XBuf[0] * Sin + YBuf[0] * Cos ;
				
				XBuf[2] = ( TexVert[1].x - CenX ) * ExtendRateX ;
				YBuf[2] = ( TexVert[1].y - CenY ) * ExtendRate  ;
				XBuf[3] = XBuf[2] * Cos - YBuf[2] * Sin ;
				YBuf[3] = XBuf[2] * Sin + YBuf[2] * Cos ;

				if( AlwaysCheck )
				{
					DrawPos[0].x =  XBuf[1] + xf ;
					DrawPos[0].y =  YBuf[1] + yf ;
					DrawPos[1].x =  XBuf[3] + xf ;
					DrawPos[1].y =  YBuf[3] + yf ;
					DrawPos[2].x = -XBuf[3] + xf ;
					DrawPos[2].y = -YBuf[3] + yf ;
					DrawPos[3].x = -XBuf[1] + xf ;
					DrawPos[3].y = -YBuf[1] + yf ;

					if( ( ( ( *((DWORD *)&DrawPos[0].x) == *((DWORD *)&DrawPos[2].x) ) ||
							( ( DrawPos[0].x > DrawPos[2].x ) && ( DrawPos[0].x < DrawPos[2].x + 0.01f ) ) ||
							( ( DrawPos[2].x > DrawPos[0].x ) && ( DrawPos[2].x < DrawPos[0].x + 0.01f ) ) ) &&
							_FABS( DrawPos[0].x - _FTOL( DrawPos[0].x ) ) < 0.0001f ) ||
						( ( ( *((DWORD *)&DrawPos[0].x) == *((DWORD *)&DrawPos[1].x) ) ||
							( ( DrawPos[0].x > DrawPos[1].x ) && ( DrawPos[0].x < DrawPos[1].x + 0.01f ) ) ||
							( ( DrawPos[1].x > DrawPos[0].x ) && ( DrawPos[1].x < DrawPos[0].x + 0.01f ) ) ) &&
							_FABS( DrawPos[0].x - _FTOL( DrawPos[0].x ) ) < 0.0001f ) )
					{
						DrawPos[0].x -= 0.5f;
						DrawPos[1].x -= 0.5f;
						DrawPos[2].x -= 0.5f;
						DrawPos[3].x -= 0.5f;
					}

					if( ( ( ( *((DWORD *)&DrawPos[0].y) == *((DWORD *)&DrawPos[2].y) ) ||
							( ( DrawPos[0].y > DrawPos[2].y ) && ( DrawPos[0].y < DrawPos[2].y + 0.01f ) ) ||
							( ( DrawPos[2].y > DrawPos[0].y ) && ( DrawPos[2].y < DrawPos[0].y + 0.01f ) ) ) &&
							_FABS( DrawPos[0].y - _FTOL( DrawPos[0].y ) ) < 0.0001f ) ||
						( ( ( *((DWORD *)&DrawPos[0].y) == *((DWORD *)&DrawPos[1].y) ) ||
							( ( DrawPos[0].y > DrawPos[1].y ) && ( DrawPos[0].y < DrawPos[1].y + 0.01f ) ) ||
							( ( DrawPos[1].y > DrawPos[0].y ) && ( DrawPos[1].y < DrawPos[0].y + 0.01f ) ) ) &&
							_FABS( DrawPos[0].y - _FTOL( DrawPos[0].y ) ) < 0.0001f ) )
					{
						DrawPos[0].y -= 0.5f;
						DrawPos[1].y -= 0.5f;
						DrawPos[2].y -= 0.5f;
						DrawPos[3].y -= 0.5f;
					}

					DrawVert[0].pos.x = DrawPos[0].x ;
					DrawVert[0].pos.y = DrawPos[0].y ;
					DrawVert[1].pos.x = DrawPos[1].x ;
					DrawVert[1].pos.y = DrawPos[1].y ;
					DrawVert[2].pos.x = DrawPos[2].x ;
					DrawVert[2].pos.y = DrawPos[2].y ;
					DrawVert[3].pos.x = DrawPos[3].x ;
					DrawVert[3].pos.y = DrawPos[3].y ;
					DrawVert[4].pos.x = DrawPos[2].x ;
					DrawVert[4].pos.y = DrawPos[2].y ;
					DrawVert[5].pos.x = DrawPos[1].x ;
					DrawVert[5].pos.y = DrawPos[1].y ;
				}
				else
				if( AngleCheck )
				{
					DrawPos[0].x =  XBuf[1] + xf ;
					DrawPos[0].y =  YBuf[1] + yf ;
					DrawPos[3].x = -XBuf[1] + xf ;
					DrawPos[3].y = -YBuf[1] + yf ;

					DrawPos[1].x =  XBuf[3] + xf ;
					DrawPos[1].y =  YBuf[3] + yf ;
					DrawPos[2].x = -XBuf[3] + xf ;
					DrawPos[2].y = -YBuf[3] + yf ;

					if( hx &&
						( ( *((DWORD *)&DrawPos[0].x) == *((DWORD *)&DrawPos[2].x) ) ||
						  ( ( DrawPos[0].x > DrawPos[2].x - 0.01f ) && ( DrawPos[0].x < DrawPos[2].x + 0.01f ) ) ||
						  ( ( DrawPos[2].x > DrawPos[0].x - 0.01f ) && ( DrawPos[2].x < DrawPos[0].x + 0.01f ) ) ||
						  ( *((DWORD *)&DrawPos[0].x) == *((DWORD *)&DrawPos[1].x) ) ||
						  ( ( DrawPos[0].x > DrawPos[1].x - 0.01f ) && ( DrawPos[0].x < DrawPos[1].x + 0.01f ) ) ||
						  ( ( DrawPos[1].x > DrawPos[0].x - 0.01f ) && ( DrawPos[1].x < DrawPos[0].x + 0.01f ) ) ) )
					{
						DrawPos[0].x -= 0.5f;
						DrawPos[1].x -= 0.5f;
						DrawPos[2].x -= 0.5f;
						DrawPos[3].x -= 0.5f;
					}

					if( hy &&
						( ( *((DWORD *)&DrawPos[0].y) == *((DWORD *)&DrawPos[1].y) ) ||
						  ( ( DrawPos[0].y > DrawPos[1].y - 0.01f ) && ( DrawPos[0].y < DrawPos[1].y + 0.01f ) ) ||
						  ( ( DrawPos[1].y > DrawPos[0].y - 0.01f ) && ( DrawPos[1].y < DrawPos[0].y + 0.01f ) ) ||
						  ( *((DWORD *)&DrawPos[0].y) == *((DWORD *)&DrawPos[2].y) ) ||
						  ( ( DrawPos[0].y > DrawPos[2].y - 0.01f ) && ( DrawPos[0].y < DrawPos[2].y + 0.01f ) ) ||
						  ( ( DrawPos[2].y > DrawPos[0].y - 0.01f ) && ( DrawPos[2].y < DrawPos[0].y + 0.01f ) ) ) )
					{
						DrawPos[0].y -= 0.5f;
						DrawPos[1].y -= 0.5f;
						DrawPos[2].y -= 0.5f;
						DrawPos[3].y -= 0.5f;
					}

					DrawVert[0].pos.x = DrawPos[0].x ;
					DrawVert[0].pos.y = DrawPos[0].y ;
					DrawVert[1].pos.x = DrawPos[1].x ;
					DrawVert[1].pos.y = DrawPos[1].y ;
					DrawVert[2].pos.x = DrawPos[2].x ;
					DrawVert[2].pos.y = DrawPos[2].y ;
					DrawVert[3].pos.x = DrawPos[3].x ;
					DrawVert[3].pos.y = DrawPos[3].y ;
					DrawVert[4].pos.x = DrawPos[2].x ;
					DrawVert[4].pos.y = DrawPos[2].y ;
					DrawVert[5].pos.x = DrawPos[1].x ;
					DrawVert[5].pos.y = DrawPos[1].y ;
				}
				else
				{
					DrawVert[0].pos.x =  XBuf[1] + xf ;
					DrawVert[0].pos.y =  YBuf[1] + yf ;
					DrawVert[1].pos.x =  XBuf[3] + xf ;
					DrawVert[1].pos.y =  YBuf[3] + yf ;
					DrawVert[2].pos.x = -XBuf[3] + xf ;
					DrawVert[2].pos.y = -YBuf[3] + yf ;
					DrawVert[3].pos.x = -XBuf[1] + xf ;
					DrawVert[3].pos.y = -YBuf[1] + yf ;
					DrawVert[4].pos.x = -XBuf[3] + xf ;
					DrawVert[4].pos.y = -YBuf[3] + yf ;
					DrawVert[5].pos.x =  XBuf[3] + xf ;
					DrawVert[5].pos.y =  YBuf[3] + yf ;
				}

				*((DWORD *)&DrawVert[0].pos.z) = drawz ;
				*((DWORD *)&DrawVert[1].pos.z) = drawz ;
				*((DWORD *)&DrawVert[2].pos.z) = drawz ;
				*((DWORD *)&DrawVert[3].pos.z) = drawz ;
				*((DWORD *)&DrawVert[4].pos.z) = drawz ;
				*((DWORD *)&DrawVert[5].pos.z) = drawz ;

				// �e�N�X�`���[��`�悷��
				ADD4VERTEX_TEX
			}
			else
			{
				// ���_�f�[�^������������
				GETVERTEX_QUAD( DrawVert )
				
				TexVert = DrawTex->Vertex ;

				DrawVert[0].color = ( DWORD )DiffuseColor ;
				DrawVert[3].color = ( DWORD )DiffuseColor ;

				DrawVert[0].u = TexVert[0].u ;
				DrawVert[0].v = TexVert[0].v ;
				DrawVert[1].u = TexVert[1].u ;
				DrawVert[1].v = TexVert[0].v ;
				DrawVert[2].u = TexVert[0].u ;
				DrawVert[2].v = TexVert[2].v ;
				DrawVert[3].u = TexVert[1].u ;
				DrawVert[3].v = TexVert[2].v ;
				DrawVert[4].u = TexVert[0].u ;
				DrawVert[4].v = TexVert[2].v ;
				DrawVert[5].u = TexVert[1].u ;
				DrawVert[5].v = TexVert[0].v ;

				XBuf[0] = ( TexVert[0].x - CenX ) * ExtendRateX ;
				YBuf[0] = ( TexVert[0].y - CenY ) * ExtendRate ;

				XBuf[1] = ( TexVert[1].x - CenX ) * ExtendRateX ;
				YBuf[1] = ( TexVert[1].y - CenY ) * ExtendRate ;

				DrawVert[0].pos.x =  XBuf[0] + xf ;
				DrawVert[0].pos.y =  YBuf[0] + yf ;
				DrawVert[1].pos.x =  XBuf[1] + xf ;
				DrawVert[1].pos.y =  YBuf[1] + yf ;
				DrawVert[2].pos.x = -XBuf[1] + xf ;
				DrawVert[2].pos.y = -YBuf[1] + yf ;
				DrawVert[3].pos.x = -XBuf[0] + xf ;
				DrawVert[3].pos.y = -YBuf[0] + yf ;
				DrawVert[4].pos.x = -XBuf[1] + xf ;
				DrawVert[4].pos.y = -YBuf[1] + yf ;
				DrawVert[5].pos.x =  XBuf[1] + xf ;
				DrawVert[5].pos.y =  YBuf[1] + yf ;

				*((DWORD *)&DrawVert[0].pos.z) = drawz ;
				*((DWORD *)&DrawVert[1].pos.z) = drawz ;
				*((DWORD *)&DrawVert[2].pos.z) = drawz ;
				*((DWORD *)&DrawVert[3].pos.z) = drawz ;
				*((DWORD *)&DrawVert[4].pos.z) = drawz ;
				*((DWORD *)&DrawVert[5].pos.z) = drawz ;

				// �e�N�X�`���[��`�悷��
				ADD4VERTEX_TEX
			}
		}
		else
		{
			if( SinCosNone == 0 )
			{
				// �e�N�X�`���[�̐������J��Ԃ�
				i = 0 ;
				for(;;)
				{
					// ���_�f�[�^������������
					GETVERTEX_QUAD( DrawVert )
					
					TexVert = DrawTex->Vertex ;

					DrawVert[0].color = ( DWORD )DiffuseColor ;
					DrawVert[3].color = ( DWORD )DiffuseColor ;

					DrawVert[0].u = TexVert[0].u ;
					DrawVert[0].v = TexVert[0].v ;
					DrawVert[1].u = TexVert[1].u ;
					DrawVert[1].v = TexVert[0].v ;
					DrawVert[2].u = TexVert[0].u ;
					DrawVert[2].v = TexVert[2].v ;
					DrawVert[3].u = TexVert[1].u ;
					DrawVert[3].v = TexVert[2].v ;
					DrawVert[4].u = TexVert[0].u ;
					DrawVert[4].v = TexVert[2].v ;
					DrawVert[5].u = TexVert[1].u ;
					DrawVert[5].v = TexVert[0].v ;

					XBuf[0] = ( TexVert[0].x - CenX ) * ExtendRateX ;
					YBuf[0] = ( TexVert[0].y - CenY ) * ExtendRate  ;
					XBuf[1] = XBuf[0] * Cos - YBuf[0] * Sin ;
					YBuf[1] = XBuf[0] * Sin + YBuf[0] * Cos ;

					XBuf[2] = ( TexVert[1].x - CenX ) * ExtendRateX ;
					YBuf[2] = ( TexVert[1].y - CenY ) * ExtendRate ;
					XBuf[3] = XBuf[2] * Cos - YBuf[2] * Sin ;
					YBuf[3] = XBuf[2] * Sin + YBuf[2] * Cos ;

					XBuf[4] = ( TexVert[2].x - CenX ) * ExtendRateX ;
					YBuf[4] = ( TexVert[2].y - CenY ) * ExtendRate ;
					XBuf[5] = XBuf[4] * Cos - YBuf[4] * Sin ;
					YBuf[5] = XBuf[4] * Sin + YBuf[4] * Cos ;

					XBuf[6] = ( TexVert[3].x - CenX ) * ExtendRateX ;
					YBuf[6] = ( TexVert[3].y - CenY ) * ExtendRate ;
					XBuf[7] = XBuf[6] * Cos - YBuf[6] * Sin ;
					YBuf[7] = XBuf[6] * Sin + YBuf[6] * Cos ;

					if( AlwaysCheck )
					{
						float f;

						DrawPos[0].x = XBuf[1] + xf ;
						DrawPos[0].y = YBuf[1] + yf ;
						DrawPos[1].x = XBuf[3] + xf ;
						DrawPos[1].y = YBuf[3] + yf ;
						DrawPos[2].x = XBuf[5] + xf ;
						DrawPos[2].y = YBuf[5] + yf ;
						DrawPos[3].x = XBuf[7] + xf ;
						DrawPos[3].y = YBuf[7] + yf ;

						f = _FABS( DrawPos[0].x - _FTOL( DrawPos[0].x ) );
						if( ( ( ( *((DWORD *)&DrawPos[0].x) == *((DWORD *)&DrawPos[2].x) ) ||
							    ( ( DrawPos[0].x >= DrawPos[2].x - 0.01f ) && ( DrawPos[0].x < DrawPos[2].x + 0.01f ) ) ||
							    ( ( DrawPos[2].x >= DrawPos[0].x - 0.01f ) && ( DrawPos[2].x < DrawPos[0].x + 0.01f ) ) ) &&
							    ( f < 0.001f || f > 0.999f ) ) ||
							( ( ( *((DWORD *)&DrawPos[0].x) == *((DWORD *)&DrawPos[1].x) ) ||
							    ( ( DrawPos[0].x >= DrawPos[1].x - 0.01f ) && ( DrawPos[0].x < DrawPos[1].x + 0.01f ) ) ||
							    ( ( DrawPos[1].x >= DrawPos[0].x - 0.01f ) && ( DrawPos[1].x < DrawPos[0].x + 0.01f ) ) ) &&
							    ( f < 0.001f || f > 0.999f ) ) )
						{
							DrawPos[0].x -= 0.5f;
							DrawPos[1].x -= 0.5f;
							DrawPos[2].x -= 0.5f;
							DrawPos[3].x -= 0.5f;
						}

						f = _FABS( DrawPos[0].y - _FTOL( DrawPos[0].y ) );
						if( ( ( ( *((DWORD *)&DrawPos[0].y) == *((DWORD *)&DrawPos[2].y) ) ||
							    ( ( DrawPos[0].y >= DrawPos[2].y - 0.01f ) && ( DrawPos[0].y < DrawPos[2].y + 0.01f ) ) ||
							    ( ( DrawPos[2].y >= DrawPos[0].y - 0.01f ) && ( DrawPos[2].y < DrawPos[0].y + 0.01f ) ) ) &&
							    ( f < 0.001f || f > 0.999f ) ) ||
							( ( ( *((DWORD *)&DrawPos[0].y) == *((DWORD *)&DrawPos[1].y) ) ||
							    ( ( DrawPos[0].y >= DrawPos[1].y - 0.01f ) && ( DrawPos[0].y < DrawPos[1].y + 0.01f ) ) ||
							    ( ( DrawPos[1].y >= DrawPos[0].y - 0.01f ) && ( DrawPos[1].y < DrawPos[0].y + 0.01f ) ) ) &&
							    ( f < 0.001f || f > 0.999f ) ) )
						{
							DrawPos[0].y -= 0.5f;
							DrawPos[1].y -= 0.5f;
							DrawPos[2].y -= 0.5f;
							DrawPos[3].y -= 0.5f;
						}

						DrawVert[0].pos.x = DrawPos[0].x ;
						DrawVert[0].pos.y = DrawPos[0].y ;
						DrawVert[1].pos.x = DrawPos[1].x ;
						DrawVert[1].pos.y = DrawPos[1].y ;
						DrawVert[2].pos.x = DrawPos[2].x ;
						DrawVert[2].pos.y = DrawPos[2].y ;
						DrawVert[3].pos.x = DrawPos[3].x ;
						DrawVert[3].pos.y = DrawPos[3].y ;
						DrawVert[4].pos.x = DrawPos[2].x ;
						DrawVert[4].pos.y = DrawPos[2].y ;
						DrawVert[5].pos.x = DrawPos[1].x ;
						DrawVert[5].pos.y = DrawPos[1].y ;
					}
					else
					if( AngleCheck )
					{
						float f;

						DrawPos[0].x = XBuf[1] + xf ;
						DrawPos[0].y = YBuf[1] + yf ;
						DrawPos[1].x = XBuf[3] + xf ;
						DrawPos[1].y = YBuf[3] + yf ;
						DrawPos[2].x = XBuf[5] + xf ;
						DrawPos[2].y = YBuf[5] + yf ;
						DrawPos[3].x = XBuf[7] + xf ;
						DrawPos[3].y = YBuf[7] + yf ;

						if( hx )
						{
							f = _FABS( DrawPos[0].x - _FTOL( DrawPos[0].x ) );
							if( ( ( ( *((DWORD *)&DrawPos[0].x) == *((DWORD *)&DrawPos[2].x) ) ||
									( ( DrawPos[0].x >= DrawPos[2].x - 0.01f ) && ( DrawPos[0].x < DrawPos[2].x + 0.01f ) ) ||
									( ( DrawPos[2].x >= DrawPos[0].x - 0.01f ) && ( DrawPos[2].x < DrawPos[0].x + 0.01f ) ) ) &&
									( f < 0.001f || f > 0.999f ) ) ||
								( ( ( *((DWORD *)&DrawPos[0].x) == *((DWORD *)&DrawPos[1].x) ) ||
									( ( DrawPos[0].x >= DrawPos[1].x - 0.01f ) && ( DrawPos[0].x < DrawPos[1].x + 0.01f ) ) ||
									( ( DrawPos[1].x >= DrawPos[0].x - 0.01f ) && ( DrawPos[1].x < DrawPos[0].x + 0.01f ) ) ) &&
									( f < 0.001f || f > 0.999f ) ) )
							{
								DrawPos[0].x -= 0.5f;
								DrawPos[1].x -= 0.5f;
								DrawPos[2].x -= 0.5f;
								DrawPos[3].x -= 0.5f;
							}
						}

						if( hy )
						{
							f = _FABS( DrawPos[0].y - _FTOL( DrawPos[0].y ) );
							if( ( ( ( *((DWORD *)&DrawPos[0].y) == *((DWORD *)&DrawPos[2].y) ) ||
									( ( DrawPos[0].y >= DrawPos[2].y - 0.01f ) && ( DrawPos[0].y < DrawPos[2].y + 0.01f ) ) ||
									( ( DrawPos[2].y >= DrawPos[0].y - 0.01f ) && ( DrawPos[2].y < DrawPos[0].y + 0.01f ) ) ) &&
									( f < 0.001f || f > 0.999f ) ) ||
								( ( ( *((DWORD *)&DrawPos[0].y) == *((DWORD *)&DrawPos[1].y) ) ||
									( ( DrawPos[0].y >= DrawPos[1].y - 0.01f ) && ( DrawPos[0].y < DrawPos[1].y + 0.01f ) ) ||
									( ( DrawPos[1].y >= DrawPos[0].y - 0.01f ) && ( DrawPos[1].y < DrawPos[0].y + 0.01f ) ) ) &&
									( f < 0.001f || f > 0.999f ) ) )
							{
								DrawPos[0].y -= 0.5f;
								DrawPos[1].y -= 0.5f;
								DrawPos[2].y -= 0.5f;
								DrawPos[3].y -= 0.5f;
							}
						}

						DrawVert[0].pos.x = DrawPos[0].x ;
						DrawVert[0].pos.y = DrawPos[0].y ;
						DrawVert[1].pos.x = DrawPos[1].x ;
						DrawVert[1].pos.y = DrawPos[1].y ;
						DrawVert[2].pos.x = DrawPos[2].x ;
						DrawVert[2].pos.y = DrawPos[2].y ;
						DrawVert[3].pos.x = DrawPos[3].x ;
						DrawVert[3].pos.y = DrawPos[3].y ;
						DrawVert[4].pos.x = DrawPos[2].x ;
						DrawVert[4].pos.y = DrawPos[2].y ;
						DrawVert[5].pos.x = DrawPos[1].x ;
						DrawVert[5].pos.y = DrawPos[1].y ;
					}
					else
					{
						DrawVert[0].pos.x = XBuf[1] + xf ;
						DrawVert[0].pos.y = YBuf[1] + yf ;
						DrawVert[1].pos.x = XBuf[3] + xf ;
						DrawVert[1].pos.y = YBuf[3] + yf ;
						DrawVert[2].pos.x = XBuf[5] + xf ;
						DrawVert[2].pos.y = YBuf[5] + yf ;
						DrawVert[3].pos.x = XBuf[7] + xf ;
						DrawVert[3].pos.y = YBuf[7] + yf ;
						DrawVert[4].pos.x = XBuf[5] + xf ;
						DrawVert[4].pos.y = YBuf[5] + yf ;
						DrawVert[5].pos.x = XBuf[3] + xf ;
						DrawVert[5].pos.y = YBuf[3] + yf ;
					}

					*((DWORD *)&DrawVert[0].pos.z) = drawz ;
					*((DWORD *)&DrawVert[1].pos.z) = drawz ;
					*((DWORD *)&DrawVert[2].pos.z) = drawz ;
					*((DWORD *)&DrawVert[3].pos.z) = drawz ;
					*((DWORD *)&DrawVert[4].pos.z) = drawz ;
					*((DWORD *)&DrawVert[5].pos.z) = drawz ;

					// �e�N�X�`���[��`�悷��
					ADD4VERTEX_TEX

					i ++ ;
					if( i >= DrawTexNum ) break ;

					// �`�悷��e�N�X�`���[�̃Z�b�g
					DrawTex ++ ;
					Graphics_D3D9_DrawSetting_SetTexture( DrawTex->Tex->PF->D3D9.Texture ) ;
					Graphics_D3D9_DeviceState_RefreshBlendState() ;
				}
			}
			else
			{
				// �e�N�X�`���[�̐������J��Ԃ�
				i = 0 ;
				for(;;)
				{
					// ���_�f�[�^������������
					GETVERTEX_QUAD( DrawVert )
					
					TexVert = DrawTex->Vertex ;

					DrawVert[0].color = ( DWORD )DiffuseColor ;
					DrawVert[3].color = ( DWORD )DiffuseColor ;

					DrawVert[0].u = TexVert[0].u ;
					DrawVert[0].v = TexVert[0].v ;
					DrawVert[1].u = TexVert[1].u ;
					DrawVert[1].v = TexVert[0].v ;
					DrawVert[2].v = TexVert[2].v ;
					DrawVert[2].u = TexVert[0].u ;
					DrawVert[3].u = TexVert[1].u ;
					DrawVert[3].v = TexVert[2].v ;
					DrawVert[4].u = TexVert[0].u ;
					DrawVert[4].v = TexVert[2].v ;
					DrawVert[5].u = TexVert[1].u ;
					DrawVert[5].v = TexVert[0].v ;

					XBuf[0] = ( TexVert[0].x - CenX ) * ExtendRateX + xf ;
					YBuf[0] = ( TexVert[0].y - CenY ) * ExtendRate  + yf ;
					XBuf[1] = ( TexVert[1].x - CenX ) * ExtendRateX + xf ;
					YBuf[1] = ( TexVert[1].y - CenY ) * ExtendRate  + yf ;
					XBuf[2] = ( TexVert[2].x - CenX ) * ExtendRateX + xf ;
					YBuf[2] = ( TexVert[2].y - CenY ) * ExtendRate  + yf ;
					XBuf[3] = ( TexVert[3].x - CenX ) * ExtendRateX + xf ;
					YBuf[3] = ( TexVert[3].y - CenY ) * ExtendRate  + yf ;

					DrawVert[0].pos.x = XBuf[0] ;
					DrawVert[0].pos.y = YBuf[0] ;
					DrawVert[1].pos.x = XBuf[1] ;
					DrawVert[1].pos.y = YBuf[1] ;
					DrawVert[2].pos.x = XBuf[2] ;
					DrawVert[2].pos.y = YBuf[2] ;
					DrawVert[3].pos.x = XBuf[3] ;
					DrawVert[3].pos.y = YBuf[3] ;
					DrawVert[4].pos.x = XBuf[2] ;
					DrawVert[4].pos.y = YBuf[2] ;
					DrawVert[5].pos.x = XBuf[1] ;
					DrawVert[5].pos.y = YBuf[1] ;

					*((DWORD *)&DrawVert[0].pos.z) = drawz ;
					*((DWORD *)&DrawVert[1].pos.z) = drawz ;
					*((DWORD *)&DrawVert[2].pos.z) = drawz ;
					*((DWORD *)&DrawVert[3].pos.z) = drawz ;
					*((DWORD *)&DrawVert[4].pos.z) = drawz ;
					*((DWORD *)&DrawVert[5].pos.z) = drawz ;

					// �e�N�X�`���[��`�悷��
					ADD4VERTEX_TEX

					i ++ ;
					if( i >= DrawTexNum ) break ;

					// �`�悷��e�N�X�`���[�̃Z�b�g
					DrawTex ++ ;
					Graphics_D3D9_DrawSetting_SetTexture( DrawTex->Tex->PF->D3D9.Texture ) ;
					Graphics_D3D9_DeviceState_RefreshBlendState() ;
				}
			}
		}
		break ;
	}

	// �I��
	return 0 ;
}

// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawRotaGraph
extern	int		Graphics_D3D9_DrawRotaGraphFast( int x, int y, float xf, float yf, float ExtendRate, float Angle, IMAGEDATA *Image, IMAGEDATA *BlendImage, int TransFlag, int TurnFlag, int IntFlag )
{
	VERTEX_2D *DrawVert ;
	VERTEX_BLENDTEX_2D *DrawVertB ;
	DWORD DiffuseColor ;
	IMAGEDATA_HARD_DRAW *DrawTex ;
	IMAGEDATA_HARD_DRAW *BlendDrawTex = NULL ;
	IMAGEDATA_HARD_VERT *TexVert ;
	IMAGEDATA_HARD_VERT *BlendTexVert ;
	int DrawTexNum ;
	int i ; 
	float Sin = 0.0f, Cos = 1.0f ;
	int BlendGraphNoIncFlag ;
	float XBuf[ 8 ], YBuf[ 8 ] ;
	float u[ 4 ], v[ 4 ] ;
	float ExtendRateX ;
	DWORD drawz ;
//	BOOL Adj = FALSE ;
	float CenX, CenY ;
	int SinCosNone ;
	int Flag ;

	if( GAPIWin.Direct3DDevice9Object == NULL ) return -1 ;

	// �����Ȃ������ꍇ�̓G���[
	if( ExtendRate == 0.0f ) return -1 ;

	// �`��̏���
	if( GD3D9.Device.DrawInfo.BeginSceneFlag == FALSE ) Graphics_D3D9_BeginScene() ;
	Flag = TransFlag ;
	DX_D3D9_DRAWPREP_TEX( Image->Orig, Image->Hard.Draw[ 0 ].Tex->PF->D3D9.Texture, Flag )

	// ��]�v�f�����������ׂ�
	if( Angle == 0.0f )
	{
		SinCosNone = 1;
	}
	else
	{
		SinCosNone = 0;
		_TABLE_SINCOS( Angle, &Sin, &Cos ) ;
	}

	// ���S���W�̌v�Z
	CenX = ( float )Image->WidthF * 0.5F ;
	CenY = ( float )Image->HeightF * 0.5F ;
	xf -= 0.5F ;
	yf -= 0.5F ;

	// ���_�f�[�^���擾�i���̂Ƃ���F���̃Z�b�g�̂݁j
	DiffuseColor = GD3D9.Device.DrawInfo.DiffuseColor ;

	// �`����̐����Z�b�g
	DrawTexNum = Image->Hard.DrawNum ;

	// �`����̐����u�����h�摜�ƈقȂ��Ă�����O�Ԗڂ̃e�N�X�`���������g�p����
	BlendGraphNoIncFlag = FALSE ;
	if( BlendImage != NULL && BlendImage->Hard.DrawNum != Image->Hard.DrawNum )
	{
		BlendGraphNoIncFlag = TRUE ;
	}

	// �`����z��̃A�h���X���Z�b�g���Ă���
	DrawTex = Image->Hard.Draw ;
	if( BlendImage != NULL )
	{
		BlendDrawTex = BlendImage->Hard.Draw ;
	}

	// �y�o�b�t�@�ɏ������ނy�l���Z�b�g����
	drawz = *((DWORD *)&GSYS.DrawSetting.DrawZ);

	// ���]���l�������������̊g�嗦���Z�b�g
	ExtendRateX = ( ( TurnFlag == TRUE ) ? -1.0f : 1.0f ) * ExtendRate ;

	// ���_�^�C�v�ɂ���ď����𕪊�
	switch( GD3D9.Device.DrawInfo.VertexType )
	{
	case VERTEXTYPE_BLENDTEX :	// �u�����h�e�N�X�`�����g�p����
		// �e�N�X�`���[�̐������J��Ԃ�
		if( DrawTexNum == 1 )
		{
			// �`�悷��e�N�X�`���[�̃Z�b�g
			Graphics_D3D9_DrawSetting_SetTexture( DrawTex->Tex->PF->D3D9.Texture ) ;
			Graphics_D3D9_DrawSetting_SetBlendTexture( BlendDrawTex->Tex->PF->D3D9.Texture, BlendDrawTex->Tex->TexWidth, BlendDrawTex->Tex->TexHeight );

			if( GD3D9.Device.DrawSetting.ChangeTextureFlag )
				Graphics_D3D9_DeviceState_RefreshBlendState() ;

			// ���_�f�[�^������������
			GETVERTEX_QUAD( DrawVertB )

			TexVert = DrawTex->Vertex ;
			BlendTexVert = BlendDrawTex->Vertex ;

			DrawVertB[0].color = ( DWORD )DiffuseColor ;
			DrawVertB[3].color = ( DWORD )DiffuseColor ;

			XBuf[0] = ( TexVert[0].x - CenX ) * ExtendRateX ;
			YBuf[0] = ( TexVert[0].y - CenY ) * ExtendRate ;
			XBuf[1] = XBuf[0] * Cos - YBuf[0] * Sin ;
			YBuf[1] = XBuf[0] * Sin + YBuf[0] * Cos ;

			XBuf[2] = ( TexVert[1].x - CenX ) * ExtendRateX ;
			YBuf[2] = ( TexVert[1].y - CenY ) * ExtendRate ;
			XBuf[3] = XBuf[2] * Cos - YBuf[2] * Sin ;
			YBuf[3] = XBuf[2] * Sin + YBuf[2] * Cos ;

			DrawVertB[0].pos.x =  XBuf[1] + xf ;
			DrawVertB[0].pos.y =  YBuf[1] + yf ;
			DrawVertB[1].pos.x =  XBuf[3] + xf ;
			DrawVertB[1].pos.y =  YBuf[3] + yf ;
			DrawVertB[2].pos.x = -XBuf[3] + xf ;
			DrawVertB[2].pos.y = -YBuf[3] + yf ;
			DrawVertB[3].pos.x = -XBuf[1] + xf ;
			DrawVertB[3].pos.y = -YBuf[1] + yf ;
			DrawVertB[4].pos.x = -XBuf[3] + xf ;
			DrawVertB[4].pos.y = -YBuf[3] + yf ;
			DrawVertB[5].pos.x =  XBuf[3] + xf ;
			DrawVertB[5].pos.y =  YBuf[3] + yf ;

			switch( GSYS.DrawSetting.BlendGraphPosMode )
			{
			default :
			case DX_BLENDGRAPH_POSMODE_DRAWGRAPH:
				u[ 0 ] = TexVert[0].x - BlendTexVert[0].x + GSYS.DrawSetting.BlendGraphX * GD3D9.Device.DrawSetting.InvBlendTextureWidth ;
				u[ 1 ] = TexVert[1].x - BlendTexVert[0].x + GSYS.DrawSetting.BlendGraphX * GD3D9.Device.DrawSetting.InvBlendTextureWidth ;
				v[ 0 ] = TexVert[0].y - BlendTexVert[0].y + GSYS.DrawSetting.BlendGraphY * GD3D9.Device.DrawSetting.InvBlendTextureHeight ;
				v[ 1 ] = TexVert[2].y - BlendTexVert[0].y + GSYS.DrawSetting.BlendGraphY * GD3D9.Device.DrawSetting.InvBlendTextureHeight ;
				DrawVertB[0].u1 = u[ 0 ] ;
				DrawVertB[0].v1 = v[ 0 ] ;
				DrawVertB[1].u1 = u[ 1 ] ;
				DrawVertB[1].v1 = v[ 0 ] ;
				DrawVertB[2].u1 = u[ 0 ] ;
				DrawVertB[2].v1 = v[ 1 ] ;
				DrawVertB[3].u1 = u[ 1 ] ;
				DrawVertB[3].v1 = v[ 1 ] ;
				DrawVertB[4].u1 = u[ 0 ] ;
				DrawVertB[4].v1 = v[ 1 ] ;
				DrawVertB[5].u1 = u[ 1 ] ;
				DrawVertB[5].v1 = v[ 0 ] ;
				break ;

			case DX_BLENDGRAPH_POSMODE_SCREEN :
				DrawVertB[0].u1 = ( (  XBuf[1] + xf ) - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D9.Device.DrawSetting.InvBlendTextureWidth ;
				DrawVertB[0].v1 = ( (  YBuf[1] + yf ) - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D9.Device.DrawSetting.InvBlendTextureHeight ;
				DrawVertB[1].u1 = ( (  XBuf[3] + xf ) - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D9.Device.DrawSetting.InvBlendTextureWidth ;
				DrawVertB[1].v1 = ( (  YBuf[3] + yf ) - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D9.Device.DrawSetting.InvBlendTextureHeight ;
				DrawVertB[2].u1 = ( ( -XBuf[3] + xf ) - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D9.Device.DrawSetting.InvBlendTextureWidth ;
				DrawVertB[2].v1 = ( ( -YBuf[3] + yf ) - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D9.Device.DrawSetting.InvBlendTextureHeight ;
				DrawVertB[3].u1 = ( ( -XBuf[1] + xf ) - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D9.Device.DrawSetting.InvBlendTextureWidth ;
				DrawVertB[3].v1 = ( ( -YBuf[1] + yf ) - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D9.Device.DrawSetting.InvBlendTextureHeight ;
				DrawVertB[4].u1 = ( ( -XBuf[3] + xf ) - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D9.Device.DrawSetting.InvBlendTextureWidth ;
				DrawVertB[4].v1 = ( ( -YBuf[3] + yf ) - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D9.Device.DrawSetting.InvBlendTextureHeight ;
				DrawVertB[5].u1 = ( (  XBuf[3] + xf ) - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D9.Device.DrawSetting.InvBlendTextureWidth ;
				DrawVertB[5].v1 = ( (  YBuf[3] + yf ) - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D9.Device.DrawSetting.InvBlendTextureHeight ;
				break ;
			}

			DrawVertB[0].u2 = TexVert[0].u ;
			DrawVertB[0].v2 = TexVert[0].v ;
			DrawVertB[1].u2 = TexVert[1].u ;
			DrawVertB[1].v2 = TexVert[0].v ;
			DrawVertB[2].u2 = TexVert[0].u ;
			DrawVertB[2].v2 = TexVert[2].v ;
			DrawVertB[3].u2 = TexVert[1].u ;
			DrawVertB[3].v2 = TexVert[2].v ;
			DrawVertB[4].u2 = TexVert[0].u ;
			DrawVertB[4].v2 = TexVert[2].v ;
			DrawVertB[5].u2 = TexVert[1].u ;
			DrawVertB[5].v2 = TexVert[0].v ;

			*((DWORD *)&DrawVertB[0].pos.z) = drawz ;
			*((DWORD *)&DrawVertB[1].pos.z) = drawz ;
			*((DWORD *)&DrawVertB[2].pos.z) = drawz ;
			*((DWORD *)&DrawVertB[3].pos.z) = drawz ;
			*((DWORD *)&DrawVertB[4].pos.z) = drawz ;
			*((DWORD *)&DrawVertB[5].pos.z) = drawz ;

			// �e�N�X�`���[��`�悷��
			ADD4VERTEX_BLENDTEX

			if( BlendGraphNoIncFlag == FALSE )
			{
				BlendDrawTex ++ ;
			}
		}
		else
		{
			// �e�N�X�`���[�̐������J��Ԃ�
			for( i = 0 ; i < DrawTexNum ; i ++, DrawTex ++ )
			{
				// �`�悷��e�N�X�`���[�̃Z�b�g
				Graphics_D3D9_DrawSetting_SetTexture( DrawTex->Tex->PF->D3D9.Texture ) ;
				Graphics_D3D9_DrawSetting_SetBlendTexture( BlendDrawTex->Tex->PF->D3D9.Texture, BlendDrawTex->Tex->TexWidth, BlendDrawTex->Tex->TexHeight );

				if( GD3D9.Device.DrawSetting.ChangeTextureFlag )
					Graphics_D3D9_DeviceState_RefreshBlendState() ;

				// ���_�f�[�^������������
				GETVERTEX_QUAD( DrawVertB )

				TexVert = DrawTex->Vertex ;
				BlendTexVert = BlendDrawTex->Vertex ;

				DrawVertB[0].color = ( DWORD )DiffuseColor ;
				DrawVertB[3].color = ( DWORD )DiffuseColor ;

				XBuf[0] = ( TexVert[0].x - CenX ) * ExtendRateX ;
				YBuf[0] = ( TexVert[0].y - CenY ) * ExtendRate ;
				XBuf[1] = XBuf[0] * Cos - YBuf[0] * Sin ;
				YBuf[1] = XBuf[0] * Sin + YBuf[0] * Cos ;

				XBuf[2] = ( TexVert[1].x - CenX ) * ExtendRateX ;
				YBuf[2] = ( TexVert[1].y - CenY ) * ExtendRate ;
				XBuf[3] = XBuf[2] * Cos - YBuf[2] * Sin ;
				YBuf[3] = XBuf[2] * Sin + YBuf[2] * Cos ;

				XBuf[4] = ( TexVert[2].x - CenX ) * ExtendRateX ;
				YBuf[4] = ( TexVert[2].y - CenY ) * ExtendRate ;
				XBuf[5] = XBuf[4] * Cos - YBuf[4] * Sin ;
				YBuf[5] = XBuf[4] * Sin + YBuf[4] * Cos ;

				XBuf[6] = ( TexVert[3].x - CenX ) * ExtendRateX ;
				YBuf[6] = ( TexVert[3].y - CenY ) * ExtendRate ;
				XBuf[7] = XBuf[6] * Cos - YBuf[6] * Sin ;
				YBuf[7] = XBuf[6] * Sin + YBuf[6] * Cos ;

				DrawVertB[0].pos.x = XBuf[1] + xf ;
				DrawVertB[0].pos.y = YBuf[1] + yf ;
				DrawVertB[1].pos.x = XBuf[3] + xf ;
				DrawVertB[1].pos.y = YBuf[3] + yf ;
				DrawVertB[2].pos.x = XBuf[5] + xf ;
				DrawVertB[2].pos.y = YBuf[5] + yf ;
				DrawVertB[3].pos.x = XBuf[7] + xf ;
				DrawVertB[3].pos.y = YBuf[7] + yf ;
				DrawVertB[4].pos.x = XBuf[5] + xf ;
				DrawVertB[4].pos.y = YBuf[5] + yf ;
				DrawVertB[5].pos.x = XBuf[3] + xf ;
				DrawVertB[5].pos.y = YBuf[3] + yf ;

				switch( GSYS.DrawSetting.BlendGraphPosMode )
				{
				default :
				case DX_BLENDGRAPH_POSMODE_DRAWGRAPH:
					u[ 0 ] = ( TexVert[0].x - BlendTexVert[0].x + GSYS.DrawSetting.BlendGraphX ) * GD3D9.Device.DrawSetting.InvBlendTextureWidth ;
					u[ 1 ] = ( TexVert[1].x - BlendTexVert[0].x + GSYS.DrawSetting.BlendGraphX ) * GD3D9.Device.DrawSetting.InvBlendTextureWidth ;
					u[ 2 ] = ( TexVert[2].x - BlendTexVert[0].x + GSYS.DrawSetting.BlendGraphX ) * GD3D9.Device.DrawSetting.InvBlendTextureWidth ;
					u[ 3 ] = ( TexVert[3].x - BlendTexVert[0].x + GSYS.DrawSetting.BlendGraphX ) * GD3D9.Device.DrawSetting.InvBlendTextureWidth ;
					
					v[ 0 ] = ( TexVert[0].y - BlendTexVert[0].y + GSYS.DrawSetting.BlendGraphY ) * GD3D9.Device.DrawSetting.InvBlendTextureHeight ;
					v[ 1 ] = ( TexVert[1].y - BlendTexVert[0].y + GSYS.DrawSetting.BlendGraphY ) * GD3D9.Device.DrawSetting.InvBlendTextureHeight ;
					v[ 2 ] = ( TexVert[2].y - BlendTexVert[0].y + GSYS.DrawSetting.BlendGraphY ) * GD3D9.Device.DrawSetting.InvBlendTextureHeight ;
					v[ 3 ] = ( TexVert[3].y - BlendTexVert[0].y + GSYS.DrawSetting.BlendGraphY ) * GD3D9.Device.DrawSetting.InvBlendTextureHeight ;

					DrawVertB[0].u1 = u[ 0 ] ;
					DrawVertB[0].v1 = v[ 0 ] ;
					DrawVertB[1].u1 = u[ 1 ] ;
					DrawVertB[1].v1 = v[ 1 ] ;
					DrawVertB[2].u1 = u[ 2 ] ;
					DrawVertB[2].v1 = v[ 2 ] ;
					DrawVertB[3].u1 = u[ 3 ] ;
					DrawVertB[3].v1 = v[ 3 ] ;
					DrawVertB[4].u1 = u[ 2 ] ;
					DrawVertB[4].v1 = v[ 2 ] ;
					DrawVertB[5].u1 = u[ 1 ] ;
					DrawVertB[5].v1 = v[ 1 ] ;
					break ;

				case DX_BLENDGRAPH_POSMODE_SCREEN :
					DrawVertB[0].u1 = ( ( XBuf[1] + xf ) - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D9.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[0].v1 = ( ( YBuf[1] + yf ) - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D9.Device.DrawSetting.InvBlendTextureHeight ;
					DrawVertB[1].u1 = ( ( XBuf[3] + xf ) - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D9.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[1].v1 = ( ( YBuf[3] + yf ) - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D9.Device.DrawSetting.InvBlendTextureHeight ;
					DrawVertB[2].u1 = ( ( XBuf[5] + xf ) - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D9.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[2].v1 = ( ( YBuf[5] + yf ) - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D9.Device.DrawSetting.InvBlendTextureHeight ;
					DrawVertB[3].u1 = ( ( XBuf[7] + xf ) - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D9.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[3].v1 = ( ( YBuf[7] + yf ) - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D9.Device.DrawSetting.InvBlendTextureHeight ;
					DrawVertB[4].u1 = ( ( XBuf[5] + xf ) - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D9.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[4].v1 = ( ( YBuf[5] + yf ) - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D9.Device.DrawSetting.InvBlendTextureHeight ;
					DrawVertB[5].u1 = ( ( XBuf[3] + xf ) - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D9.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[5].v1 = ( ( YBuf[3] + yf ) - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D9.Device.DrawSetting.InvBlendTextureHeight ;
					break ;
				}

				DrawVertB[0].u2 = TexVert[0].u ;
				DrawVertB[0].v2 = TexVert[0].v ;
				DrawVertB[1].u2 = TexVert[1].u ;
				DrawVertB[1].v2 = TexVert[0].v ;
				DrawVertB[2].u2 = TexVert[0].u ;
				DrawVertB[2].v2 = TexVert[2].v ;
				DrawVertB[3].u2 = TexVert[1].u ;
				DrawVertB[3].v2 = TexVert[2].v ;
				DrawVertB[4].u2 = TexVert[0].u ;
				DrawVertB[4].v2 = TexVert[2].v ;
				DrawVertB[5].u2 = TexVert[1].u ;
				DrawVertB[5].v2 = TexVert[0].v ;

				*((DWORD *)&DrawVertB[0].pos.z) = drawz ;
				*((DWORD *)&DrawVertB[1].pos.z) = drawz ;
				*((DWORD *)&DrawVertB[2].pos.z) = drawz ;
				*((DWORD *)&DrawVertB[3].pos.z) = drawz ;
				*((DWORD *)&DrawVertB[4].pos.z) = drawz ;
				*((DWORD *)&DrawVertB[5].pos.z) = drawz ;

				// �e�N�X�`���[��`�悷��
				ADD4VERTEX_BLENDTEX

				if( BlendGraphNoIncFlag == FALSE )
				{
					BlendDrawTex ++ ;
				}
			}
		}
		break ;

	case VERTEXTYPE_TEX :	// �u�����h�e�N�X�`�����g�p���Ȃ�
		// �e�N�X�`���[�̐������J��Ԃ�
		if( DrawTexNum == 1 )
		{
			if( SinCosNone == 0 )
			{
				// ���_�f�[�^������������
				GETVERTEX_QUAD( DrawVert )
				
				TexVert = DrawTex->Vertex ;

				DrawVert[0].color = ( DWORD )DiffuseColor ;
				DrawVert[3].color = ( DWORD )DiffuseColor ;

				DrawVert[0].u = TexVert[0].u ;
				DrawVert[0].v = TexVert[0].v ;
				DrawVert[1].u = TexVert[1].u ;
				DrawVert[1].v = TexVert[0].v ;
				DrawVert[2].v = TexVert[2].v ;
				DrawVert[2].u = TexVert[0].u ;
				DrawVert[3].u = TexVert[1].u ;
				DrawVert[3].v = TexVert[2].v ;
				DrawVert[4].u = TexVert[0].u ;
				DrawVert[4].v = TexVert[2].v ;
				DrawVert[5].u = TexVert[1].u ;
				DrawVert[5].v = TexVert[0].v ;

				XBuf[0] = ( TexVert[0].x - CenX ) * ExtendRateX ;
				YBuf[0] = ( TexVert[0].y - CenY ) * ExtendRate  ;
				XBuf[1] = XBuf[0] * Cos - YBuf[0] * Sin ;
				YBuf[1] = XBuf[0] * Sin + YBuf[0] * Cos ;
				
				XBuf[2] = ( TexVert[1].x - CenX ) * ExtendRateX ;
				YBuf[2] = ( TexVert[1].y - CenY ) * ExtendRate  ;
				XBuf[3] = XBuf[2] * Cos - YBuf[2] * Sin ;
				YBuf[3] = XBuf[2] * Sin + YBuf[2] * Cos ;

				DrawVert[0].pos.x =  XBuf[1] + xf ;
				DrawVert[0].pos.y =  YBuf[1] + yf ;
				DrawVert[1].pos.x =  XBuf[3] + xf ;
				DrawVert[1].pos.y =  YBuf[3] + yf ;
				DrawVert[2].pos.x = -XBuf[3] + xf ;
				DrawVert[2].pos.y = -YBuf[3] + yf ;
				DrawVert[3].pos.x = -XBuf[1] + xf ;
				DrawVert[3].pos.y = -YBuf[1] + yf ;
				DrawVert[4].pos.x = -XBuf[3] + xf ;
				DrawVert[4].pos.y = -YBuf[3] + yf ;
				DrawVert[5].pos.x =  XBuf[3] + xf ;
				DrawVert[5].pos.y =  YBuf[3] + yf ;

				*((DWORD *)&DrawVert[0].pos.z) = drawz ;
				*((DWORD *)&DrawVert[1].pos.z) = drawz ;
				*((DWORD *)&DrawVert[2].pos.z) = drawz ;
				*((DWORD *)&DrawVert[3].pos.z) = drawz ;
				*((DWORD *)&DrawVert[4].pos.z) = drawz ;
				*((DWORD *)&DrawVert[5].pos.z) = drawz ;

				// �e�N�X�`���[��`�悷��
				ADD4VERTEX_TEX
			}
			else
			{
				// ���_�f�[�^������������
				GETVERTEX_QUAD( DrawVert )
				
				TexVert = DrawTex->Vertex ;

				DrawVert[0].color = ( DWORD )DiffuseColor ;
				DrawVert[3].color = ( DWORD )DiffuseColor ;

				DrawVert[0].u = TexVert[0].u ;
				DrawVert[0].v = TexVert[0].v ;
				DrawVert[1].u = TexVert[1].u ;
				DrawVert[1].v = TexVert[0].v ;
				DrawVert[2].u = TexVert[0].u ;
				DrawVert[2].v = TexVert[2].v ;
				DrawVert[3].u = TexVert[1].u ;
				DrawVert[3].v = TexVert[2].v ;
				DrawVert[4].u = TexVert[0].u ;
				DrawVert[4].v = TexVert[2].v ;
				DrawVert[5].u = TexVert[1].u ;
				DrawVert[5].v = TexVert[0].v ;

				XBuf[0] = ( TexVert[0].x - CenX ) * ExtendRateX ;
				YBuf[0] = ( TexVert[0].y - CenY ) * ExtendRate ;

				XBuf[1] = ( TexVert[1].x - CenX ) * ExtendRateX ;
				YBuf[1] = ( TexVert[1].y - CenY ) * ExtendRate ;

				DrawVert[0].pos.x =  XBuf[0] + xf ;
				DrawVert[0].pos.y =  YBuf[0] + yf ;
				DrawVert[1].pos.x =  XBuf[1] + xf ;
				DrawVert[1].pos.y =  YBuf[1] + yf ;
				DrawVert[2].pos.x = -XBuf[1] + xf ;
				DrawVert[2].pos.y = -YBuf[1] + yf ;
				DrawVert[3].pos.x = -XBuf[0] + xf ;
				DrawVert[3].pos.y = -YBuf[0] + yf ;
				DrawVert[4].pos.x = -XBuf[1] + xf ;
				DrawVert[4].pos.y = -YBuf[1] + yf ;
				DrawVert[5].pos.x =  XBuf[1] + xf ;
				DrawVert[5].pos.y =  YBuf[1] + yf ;

				*((DWORD *)&DrawVert[0].pos.z) = drawz ;
				*((DWORD *)&DrawVert[1].pos.z) = drawz ;
				*((DWORD *)&DrawVert[2].pos.z) = drawz ;
				*((DWORD *)&DrawVert[3].pos.z) = drawz ;
				*((DWORD *)&DrawVert[4].pos.z) = drawz ;
				*((DWORD *)&DrawVert[5].pos.z) = drawz ;

				// �e�N�X�`���[��`�悷��
				ADD4VERTEX_TEX
			}
		}
		else
		{
			if( SinCosNone == 0 )
			{
				// �e�N�X�`���[�̐������J��Ԃ�
				i = 0 ;
				for(;;)
				{
					// ���_�f�[�^������������
					GETVERTEX_QUAD( DrawVert )
					
					TexVert = DrawTex->Vertex ;

					DrawVert[0].color = ( DWORD )DiffuseColor ;
					DrawVert[3].color = ( DWORD )DiffuseColor ;

					DrawVert[0].u = TexVert[0].u ;
					DrawVert[0].v = TexVert[0].v ;
					DrawVert[1].u = TexVert[1].u ;
					DrawVert[1].v = TexVert[0].v ;
					DrawVert[2].u = TexVert[0].u ;
					DrawVert[2].v = TexVert[2].v ;
					DrawVert[3].u = TexVert[1].u ;
					DrawVert[3].v = TexVert[2].v ;
					DrawVert[4].u = TexVert[0].u ;
					DrawVert[4].v = TexVert[2].v ;
					DrawVert[5].u = TexVert[1].u ;
					DrawVert[5].v = TexVert[0].v ;

					XBuf[0] = ( TexVert[0].x - CenX ) * ExtendRateX ;
					YBuf[0] = ( TexVert[0].y - CenY ) * ExtendRate  ;
					XBuf[1] = XBuf[0] * Cos - YBuf[0] * Sin ;
					YBuf[1] = XBuf[0] * Sin + YBuf[0] * Cos ;

					XBuf[2] = ( TexVert[1].x - CenX ) * ExtendRateX ;
					YBuf[2] = ( TexVert[1].y - CenY ) * ExtendRate ;
					XBuf[3] = XBuf[2] * Cos - YBuf[2] * Sin ;
					YBuf[3] = XBuf[2] * Sin + YBuf[2] * Cos ;

					XBuf[4] = ( TexVert[2].x - CenX ) * ExtendRateX ;
					YBuf[4] = ( TexVert[2].y - CenY ) * ExtendRate ;
					XBuf[5] = XBuf[4] * Cos - YBuf[4] * Sin ;
					YBuf[5] = XBuf[4] * Sin + YBuf[4] * Cos ;

					XBuf[6] = ( TexVert[3].x - CenX ) * ExtendRateX ;
					YBuf[6] = ( TexVert[3].y - CenY ) * ExtendRate ;
					XBuf[7] = XBuf[6] * Cos - YBuf[6] * Sin ;
					YBuf[7] = XBuf[6] * Sin + YBuf[6] * Cos ;

					DrawVert[0].pos.x = XBuf[1] + xf ;
					DrawVert[0].pos.y = YBuf[1] + yf ;
					DrawVert[1].pos.x = XBuf[3] + xf ;
					DrawVert[1].pos.y = YBuf[3] + yf ;
					DrawVert[2].pos.x = XBuf[5] + xf ;
					DrawVert[2].pos.y = YBuf[5] + yf ;
					DrawVert[3].pos.x = XBuf[7] + xf ;
					DrawVert[3].pos.y = YBuf[7] + yf ;
					DrawVert[4].pos.x = XBuf[5] + xf ;
					DrawVert[4].pos.y = YBuf[5] + yf ;
					DrawVert[5].pos.x = XBuf[3] + xf ;
					DrawVert[5].pos.y = YBuf[3] + yf ;

					*((DWORD *)&DrawVert[0].pos.z) = drawz ;
					*((DWORD *)&DrawVert[1].pos.z) = drawz ;
					*((DWORD *)&DrawVert[2].pos.z) = drawz ;
					*((DWORD *)&DrawVert[3].pos.z) = drawz ;
					*((DWORD *)&DrawVert[4].pos.z) = drawz ;
					*((DWORD *)&DrawVert[5].pos.z) = drawz ;

					// �e�N�X�`���[��`�悷��
					ADD4VERTEX_TEX

					i ++ ;
					if( i >= DrawTexNum ) break ;

					// �`�悷��e�N�X�`���[�̃Z�b�g
					DrawTex ++ ;
					Graphics_D3D9_DrawSetting_SetTexture( DrawTex->Tex->PF->D3D9.Texture ) ;
					Graphics_D3D9_DeviceState_RefreshBlendState() ;
				}
			}
			else
			{
				// �e�N�X�`���[�̐������J��Ԃ�
				i = 0 ;
				for(;;)
				{
					// ���_�f�[�^������������
					GETVERTEX_QUAD( DrawVert )
					
					TexVert = DrawTex->Vertex ;

					DrawVert[0].color = ( DWORD )DiffuseColor ;
					DrawVert[3].color = ( DWORD )DiffuseColor ;

					DrawVert[0].u = TexVert[0].u ;
					DrawVert[0].v = TexVert[0].v ;
					DrawVert[1].u = TexVert[1].u ;
					DrawVert[1].v = TexVert[0].v ;
					DrawVert[2].v = TexVert[2].v ;
					DrawVert[2].u = TexVert[0].u ;
					DrawVert[3].u = TexVert[1].u ;
					DrawVert[3].v = TexVert[2].v ;
					DrawVert[4].u = TexVert[0].u ;
					DrawVert[4].v = TexVert[2].v ;
					DrawVert[5].u = TexVert[1].u ;
					DrawVert[5].v = TexVert[0].v ;

					XBuf[0] = ( TexVert[0].x - CenX ) * ExtendRateX + xf ;
					YBuf[0] = ( TexVert[0].y - CenY ) * ExtendRate  + yf ;
					XBuf[1] = ( TexVert[1].x - CenX ) * ExtendRateX + xf ;
					YBuf[1] = ( TexVert[1].y - CenY ) * ExtendRate  + yf ;
					XBuf[2] = ( TexVert[2].x - CenX ) * ExtendRateX + xf ;
					YBuf[2] = ( TexVert[2].y - CenY ) * ExtendRate  + yf ;
					XBuf[3] = ( TexVert[3].x - CenX ) * ExtendRateX + xf ;
					YBuf[3] = ( TexVert[3].y - CenY ) * ExtendRate  + yf ;

					DrawVert[0].pos.x = XBuf[0] ;
					DrawVert[0].pos.y = YBuf[0] ;
					DrawVert[1].pos.x = XBuf[1] ;
					DrawVert[1].pos.y = YBuf[1] ;
					DrawVert[2].pos.x = XBuf[2] ;
					DrawVert[2].pos.y = YBuf[2] ;
					DrawVert[3].pos.x = XBuf[3] ;
					DrawVert[3].pos.y = YBuf[3] ;
					DrawVert[4].pos.x = XBuf[2] ;
					DrawVert[4].pos.y = YBuf[2] ;
					DrawVert[5].pos.x = XBuf[1] ;
					DrawVert[5].pos.y = YBuf[1] ;

					*((DWORD *)&DrawVert[0].pos.z) = drawz ;
					*((DWORD *)&DrawVert[1].pos.z) = drawz ;
					*((DWORD *)&DrawVert[2].pos.z) = drawz ;
					*((DWORD *)&DrawVert[3].pos.z) = drawz ;
					*((DWORD *)&DrawVert[4].pos.z) = drawz ;
					*((DWORD *)&DrawVert[5].pos.z) = drawz ;

					// �e�N�X�`���[��`�悷��
					ADD4VERTEX_TEX

					i ++ ;
					if( i >= DrawTexNum ) break ;

					// �`�悷��e�N�X�`���[�̃Z�b�g
					DrawTex ++ ;
					Graphics_D3D9_DrawSetting_SetTexture( DrawTex->Tex->PF->D3D9.Texture ) ;
					Graphics_D3D9_DeviceState_RefreshBlendState() ;
				}
			}
		}
		break ;
	}

	// �I��
	return 0 ;
}

// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawModiGraph
extern	int		Graphics_D3D9_DrawModiGraph( int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, IMAGEDATA *Image, IMAGEDATA *BlendImage, int TransFlag, bool SimpleDrawFlag )
{
	if( x1 == x3 && x2 == x4 && y1 == y2 && y3 == y4 ) return Graphics_D3D9_DrawExtendGraph( x1, y1, x4, y4, ( float )x1, ( float )y1, ( float )x4, ( float )y4, Image, BlendImage, TransFlag, TRUE ) ; 

	// �����u�����h�e�N�X�`�����g�p�����A���^�֐����g�p����t���O�������Ă��Ȃ����
	// �����������s���`��֐����g�p����
	if( GD3D9.Setting.UseOldDrawModiGraphCodeFlag == FALSE && GSYS.DrawSetting.BlendGraph <= 0 )
		return Graphics_D3D9_DrawModiGraphF( (float)x1, (float)y1, (float)x2, (float)y2, (float)x3, (float)y3, (float)x4, (float)y4, Image, BlendImage, TransFlag, SimpleDrawFlag ) ;

	VERTEX_2D           *DrawVert ;
	VERTEX_BLENDTEX_2D	*DrawVertB ;
	IMAGEDATA_HARD_DRAW *DrawTex ;
	IMAGEDATA_HARD_DRAW *BlendDrawTex = NULL ;
	IMAGEDATA_HARD_VERT *TexVert ;
	IMAGEDATA_HARD_VERT *BlendTexVert ;
	float GraphSizeX, GraphSizeY ;
	float XBuf, YBuf ;
	int DrawTexNum ;
	unsigned char *VertType ;
	int i, j ; 
	int BlendGraphNoIncFlag ;
	float Reverse0/*, Reverse1 */ ;
	float ExRate1X1, ExRate1Y1, ExRate2X1, ExRate2Y1 ;
	float ExRate1X2, ExRate1Y2, ExRate2X2, ExRate2Y2 ;
	DWORD DiffuseColor ;
	VECTOR DrawPos[ 4 ] ;
	float u[ 4 ] ;
	float v[ 4 ] ;
	float  xf1, yf1, xf4, yf4;
	DWORD drawz ;
	int Flag ;

	if( GAPIWin.Direct3DDevice9Object == NULL ) return -1 ;

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �`��̏���
	if( GD3D9.Device.DrawInfo.BeginSceneFlag == FALSE ) Graphics_D3D9_BeginScene() ;
	Flag = TransFlag | DX_D3D9_DRAWPREP_VECTORINT ;
	DX_D3D9_DRAWPREP_TEX( Image->Orig, Image->Hard.Draw[ 0 ].Tex->PF->D3D9.Texture, Flag )

	GraphSizeX = ( float )Image->WidthF ;  GraphSizeY = ( float )Image->HeightF ;
	ExRate1X1 = ( x2 - x1 ) /  GraphSizeX ;  ExRate1X2 = ( x3 - x1 ) /  GraphSizeY ;
	ExRate1Y1 = ( y3 - y1 ) /  GraphSizeY ;  ExRate1Y2 = ( y2 - y1 ) /  GraphSizeX ;
	ExRate2X1 = ( x3 - x4 ) / -GraphSizeX ;  ExRate2X2 = ( x2 - x4 ) / -GraphSizeY ;
	ExRate2Y1 = ( y2 - y4 ) / -GraphSizeY ;  ExRate2Y2 = ( y3 - y4 ) / -GraphSizeX ;
	xf1 = (float)x1 - 0.5F ; yf1 = (float)y1 - 0.5F ;
	xf4 = (float)x4 - 0.5F ; yf4 = (float)y4 - 0.5F ; 

	// ���_�f�[�^���擾�i���̂Ƃ���F���̃Z�b�g�̂݁j
	DiffuseColor = GD3D9.Device.DrawInfo.DiffuseColor ;

	// �y�o�b�t�@�ɏ������ނy�l���Z�b�g����
	drawz = *((DWORD *)&GSYS.DrawSetting.DrawZ);

	// �`����̐����Z�b�g
	DrawTexNum = Image->Hard.DrawNum ;

	// �`����̐����u�����h�摜�ƈقȂ��Ă�����O�Ԗڂ̃e�N�X�`���������g�p����
	BlendGraphNoIncFlag = FALSE ;
	if( BlendImage != NULL && BlendImage->Hard.DrawNum != Image->Hard.DrawNum )
	{
		BlendGraphNoIncFlag = TRUE ;
	}

	// �`����z��̃A�h���X���Z�b�g���Ă���
	DrawTex = Image->Hard.Draw ;
	if( BlendImage != NULL )
	{
		BlendDrawTex = BlendImage->Hard.Draw ;
	}

	// ���_�^�C�v�ɂ���ď����𕪊�
	switch( GD3D9.Device.DrawInfo.VertexType )
	{
	case VERTEXTYPE_BLENDTEX :	// �u�����h�e�N�X�`�����g�p����
		// FVF�ݒ�
//		Graphics_D3D9_DeviceState_SetFVF( VERTEXFVF_BLENDTEX_2D ) ;
//		Graphics_Hardware_D3D9_SetFVF_UseTable( VERTEXFVF_DECL_BLENDTEX_2D ) ;

		// �e�N�X�`���[�̐������J��Ԃ�
		for( i = 0 ; i < DrawTexNum ; i ++, DrawTex ++ )
		{
			// �`�悷��e�N�X�`���[�̃Z�b�g
			Graphics_D3D9_DrawSetting_SetTexture( DrawTex->Tex->PF->D3D9.Texture ) ;
			Graphics_D3D9_DrawSetting_SetBlendTexture( BlendDrawTex->Tex->PF->D3D9.Texture, BlendDrawTex->Tex->TexWidth, BlendDrawTex->Tex->TexHeight );

			if( GD3D9.Device.DrawSetting.ChangeTextureFlag )
			{
				Graphics_D3D9_DeviceState_RefreshBlendState() ;
			}

			// ���_�o�b�t�@�̎擾
			GETVERTEX_QUAD( DrawVertB )

			// ���_�f�[�^���Z�b�g����
			DrawVertB[0].color = (DWORD)DiffuseColor ;
			DrawVertB[1].color = (DWORD)DiffuseColor ;
			DrawVertB[2].color = (DWORD)DiffuseColor ;
			DrawVertB[3].color = (DWORD)DiffuseColor ;
			DrawVertB[4].color = (DWORD)DiffuseColor ;
			DrawVertB[5].color = (DWORD)DiffuseColor ;

			VertType = DrawTex->VertType ;
			TexVert = DrawTex->Vertex ;
			BlendTexVert = BlendDrawTex->Vertex ;

			for( j = 0 ; j < 4 ; j ++, TexVert ++ )
			{
				// ���_���W�̎Z�o
				if( *VertType ++ )
				{
					// �O�p�̏㑤
					DrawPos[ j ].x = ( TexVert->x * ExRate1X1 ) + ( TexVert->y * ExRate1X2 ) + xf1 ;
					DrawPos[ j ].y = ( TexVert->y * ExRate1Y1 ) + ( TexVert->x * ExRate1Y2 ) + yf1 ;
				} 
				else
				{
					// �O�p�̉���
					XBuf = TexVert->x - GraphSizeX ;
					YBuf = TexVert->y - GraphSizeY ;
					DrawPos[ j ].x = ( XBuf * ExRate2X1 ) + ( YBuf * ExRate2X2 ) + xf4 ;
					DrawPos[ j ].y = ( YBuf * ExRate2Y1 ) + ( XBuf * ExRate2Y2 ) + yf4 ;
				}

				u[ j ] = ( TexVert->x - BlendTexVert->x + GSYS.DrawSetting.BlendGraphX ) * GD3D9.Device.DrawSetting.InvBlendTextureWidth ;
				v[ j ] = ( TexVert->y - BlendTexVert->y + GSYS.DrawSetting.BlendGraphY ) * GD3D9.Device.DrawSetting.InvBlendTextureHeight ;
			}

			// ���]���菈��
			Reverse0 =  ( DrawPos[ 1 ].x - DrawPos[ 0 ].x ) * ( DrawPos[ 2 ].y - DrawPos[ 0 ].y ) -
						( DrawPos[ 2 ].x - DrawPos[ 0 ].x ) * ( DrawPos[ 1 ].y - DrawPos[ 0 ].y ) ;
//			Reverse1 = -( DrawPos[ 1 ].x - DrawPos[ 3 ].x ) * ( DrawPos[ 2 ].y - DrawPos[ 3 ].y ) +
//						( DrawPos[ 1 ].y - DrawPos[ 3 ].y ) * ( DrawPos[ 2 ].x - DrawPos[ 3 ].x ) ;
			TexVert = DrawTex->Vertex ;
			if( Reverse0 <= 0 )
			{
				DrawVertB[ 0 ].pos.x = DrawPos[ 0 ].x ;
				DrawVertB[ 0 ].pos.y = DrawPos[ 0 ].y ;
				DrawVertB[ 1 ].pos.x = DrawPos[ 2 ].x ;
				DrawVertB[ 1 ].pos.y = DrawPos[ 2 ].y ;
				DrawVertB[ 2 ].pos.x = DrawPos[ 1 ].x ;
				DrawVertB[ 2 ].pos.y = DrawPos[ 1 ].y ;
				DrawVertB[ 3 ].pos.x = DrawPos[ 3 ].x ;
				DrawVertB[ 3 ].pos.y = DrawPos[ 3 ].y ;
				DrawVertB[ 4 ].pos.x = DrawPos[ 1 ].x ;
				DrawVertB[ 4 ].pos.y = DrawPos[ 1 ].y ;
				DrawVertB[ 5 ].pos.x = DrawPos[ 2 ].x ;
				DrawVertB[ 5 ].pos.y = DrawPos[ 2 ].y ;

				switch( GSYS.DrawSetting.BlendGraphPosMode )
				{
				default :
				case DX_BLENDGRAPH_POSMODE_DRAWGRAPH:
					DrawVertB[ 0 ].u1 = u[ 0 ] ;
					DrawVertB[ 0 ].v1 = v[ 0 ] ;
					DrawVertB[ 1 ].u1 = u[ 2 ] ;
					DrawVertB[ 1 ].v1 = v[ 2 ] ;
					DrawVertB[ 2 ].u1 = u[ 1 ] ;
					DrawVertB[ 2 ].v1 = v[ 1 ] ;
					DrawVertB[ 3 ].u1 = u[ 3 ] ;
					DrawVertB[ 3 ].v1 = v[ 3 ] ;
					DrawVertB[ 4 ].u1 = u[ 1 ] ;
					DrawVertB[ 4 ].v1 = v[ 1 ] ;
					DrawVertB[ 5 ].u1 = u[ 2 ] ;
					DrawVertB[ 5 ].v1 = v[ 2 ] ;
					break ;

				case DX_BLENDGRAPH_POSMODE_SCREEN :
					DrawVertB[ 0 ].u1 = ( DrawPos[ 0 ].x - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D9.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[ 0 ].v1 = ( DrawPos[ 0 ].y - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D9.Device.DrawSetting.InvBlendTextureHeight ;
					DrawVertB[ 1 ].u1 = ( DrawPos[ 2 ].x - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D9.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[ 1 ].v1 = ( DrawPos[ 2 ].y - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D9.Device.DrawSetting.InvBlendTextureHeight ;
					DrawVertB[ 2 ].u1 = ( DrawPos[ 1 ].x - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D9.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[ 2 ].v1 = ( DrawPos[ 1 ].y - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D9.Device.DrawSetting.InvBlendTextureHeight ;
					DrawVertB[ 3 ].u1 = ( DrawPos[ 3 ].x - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D9.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[ 3 ].v1 = ( DrawPos[ 3 ].y - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D9.Device.DrawSetting.InvBlendTextureHeight ;
					DrawVertB[ 4 ].u1 = ( DrawPos[ 1 ].x - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D9.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[ 4 ].v1 = ( DrawPos[ 1 ].y - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D9.Device.DrawSetting.InvBlendTextureHeight ;
					DrawVertB[ 5 ].u1 = ( DrawPos[ 2 ].x - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D9.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[ 5 ].v1 = ( DrawPos[ 2 ].y - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D9.Device.DrawSetting.InvBlendTextureHeight ;
					break ;
				}

				DrawVertB[ 0 ].u2 = TexVert[ 0 ].u ;
				DrawVertB[ 0 ].v2 = TexVert[ 0 ].v ;
				DrawVertB[ 1 ].u2 = TexVert[ 2 ].u ;
				DrawVertB[ 1 ].v2 = TexVert[ 2 ].v ;
				DrawVertB[ 2 ].u2 = TexVert[ 1 ].u ;
				DrawVertB[ 2 ].v2 = TexVert[ 1 ].v ;
				DrawVertB[ 3 ].u2 = TexVert[ 3 ].u ;
				DrawVertB[ 3 ].v2 = TexVert[ 3 ].v ;
				DrawVertB[ 4 ].u2 = TexVert[ 1 ].u ;
				DrawVertB[ 4 ].v2 = TexVert[ 1 ].v ;
				DrawVertB[ 5 ].u2 = TexVert[ 2 ].u ;
				DrawVertB[ 5 ].v2 = TexVert[ 2 ].v ;
			}
			else
			{
				DrawVertB[ 0 ].pos.x = DrawPos[ 0 ].x ;
				DrawVertB[ 0 ].pos.y = DrawPos[ 0 ].y ;
				DrawVertB[ 1 ].pos.x = DrawPos[ 1 ].x ;
				DrawVertB[ 1 ].pos.y = DrawPos[ 1 ].y ;
				DrawVertB[ 2 ].pos.x = DrawPos[ 2 ].x ;
				DrawVertB[ 2 ].pos.y = DrawPos[ 2 ].y ;
				DrawVertB[ 3 ].pos.x = DrawPos[ 3 ].x ;
				DrawVertB[ 3 ].pos.y = DrawPos[ 3 ].y ;
				DrawVertB[ 4 ].pos.x = DrawPos[ 2 ].x ;
				DrawVertB[ 4 ].pos.y = DrawPos[ 2 ].y ;
				DrawVertB[ 5 ].pos.x = DrawPos[ 1 ].x ;
				DrawVertB[ 5 ].pos.y = DrawPos[ 1 ].y ;

				switch( GSYS.DrawSetting.BlendGraphPosMode )
				{
				default :
				case DX_BLENDGRAPH_POSMODE_DRAWGRAPH:
					DrawVertB[ 0 ].u1 = u[ 0 ] ;
					DrawVertB[ 0 ].v1 = v[ 0 ] ;
					DrawVertB[ 1 ].u1 = u[ 1 ] ;
					DrawVertB[ 1 ].v1 = v[ 1 ] ;
					DrawVertB[ 2 ].u1 = u[ 2 ] ;
					DrawVertB[ 2 ].v1 = v[ 2 ] ;
					DrawVertB[ 3 ].u1 = u[ 3 ] ;
					DrawVertB[ 3 ].v1 = v[ 3 ] ;
					DrawVertB[ 4 ].u1 = u[ 2 ] ;
					DrawVertB[ 4 ].v1 = v[ 2 ] ;
					DrawVertB[ 5 ].u1 = u[ 1 ] ;
					DrawVertB[ 5 ].v1 = v[ 1 ] ;
					break ;

				case DX_BLENDGRAPH_POSMODE_SCREEN :
					DrawVertB[ 0 ].u1 = ( DrawPos[ 0 ].x - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D9.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[ 0 ].v1 = ( DrawPos[ 0 ].y - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D9.Device.DrawSetting.InvBlendTextureHeight ;
					DrawVertB[ 1 ].u1 = ( DrawPos[ 1 ].x - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D9.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[ 1 ].v1 = ( DrawPos[ 1 ].y - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D9.Device.DrawSetting.InvBlendTextureHeight ;
					DrawVertB[ 2 ].u1 = ( DrawPos[ 2 ].x - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D9.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[ 2 ].v1 = ( DrawPos[ 2 ].y - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D9.Device.DrawSetting.InvBlendTextureHeight ;
					DrawVertB[ 3 ].u1 = ( DrawPos[ 3 ].x - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D9.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[ 3 ].v1 = ( DrawPos[ 3 ].y - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D9.Device.DrawSetting.InvBlendTextureHeight ;
					DrawVertB[ 4 ].u1 = ( DrawPos[ 2 ].x - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D9.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[ 4 ].v1 = ( DrawPos[ 2 ].y - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D9.Device.DrawSetting.InvBlendTextureHeight ;
					DrawVertB[ 5 ].u1 = ( DrawPos[ 1 ].x - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D9.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[ 5 ].v1 = ( DrawPos[ 1 ].y - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D9.Device.DrawSetting.InvBlendTextureHeight ;
					break ;
				}

				DrawVertB[ 0 ].u2 = TexVert[ 0 ].u ;
				DrawVertB[ 0 ].v2 = TexVert[ 0 ].v ;
				DrawVertB[ 1 ].u2 = TexVert[ 1 ].u ;
				DrawVertB[ 1 ].v2 = TexVert[ 1 ].v ;
				DrawVertB[ 2 ].u2 = TexVert[ 2 ].u ;
				DrawVertB[ 2 ].v2 = TexVert[ 2 ].v ;
				DrawVertB[ 3 ].u2 = TexVert[ 3 ].u ;
				DrawVertB[ 3 ].v2 = TexVert[ 3 ].v ;
				DrawVertB[ 4 ].u2 = TexVert[ 2 ].u ;
				DrawVertB[ 4 ].v2 = TexVert[ 2 ].v ;
				DrawVertB[ 5 ].u2 = TexVert[ 1 ].u ;
				DrawVertB[ 5 ].v2 = TexVert[ 1 ].v ;
			}

			*((DWORD *)&DrawVertB[0].pos.z) = drawz ;
			*((DWORD *)&DrawVertB[1].pos.z) = drawz ;
			*((DWORD *)&DrawVertB[2].pos.z) = drawz ;
			*((DWORD *)&DrawVertB[3].pos.z) = drawz ;
			*((DWORD *)&DrawVertB[4].pos.z) = drawz ;
			*((DWORD *)&DrawVertB[5].pos.z) = drawz ;

			// �e�N�X�`���[��`�悷��
			ADD4VERTEX_BLENDTEX

			if( BlendGraphNoIncFlag == FALSE )
			{
				BlendDrawTex ++ ;
			}
		}
		break ;

	case VERTEXTYPE_TEX :	// �u�����h�e�N�X�`�����g�p���Ȃ�
		// FVF�ݒ�
//		Graphics_D3D9_DeviceState_SetFVF( VERTEXFVF_2D ) ;
//		Graphics_Hardware_D3D9_SetFVF_UseTable( VERTEXFVF_DECL_2D ) ;

		// �e�N�X�`���[�̐������J��Ԃ�
		i = 0 ;
		for(;;)
		{
			// ���_�o�b�t�@�̎擾
			GETVERTEX_QUAD( DrawVert )

			// ���_�f�[�^���Z�b�g����
			DrawVert[ 0 ].color = DiffuseColor ;
			DrawVert[ 1 ].color = DiffuseColor ;
			DrawVert[ 2 ].color = DiffuseColor ;
			DrawVert[ 3 ].color = DiffuseColor ;
			DrawVert[ 4 ].color = DiffuseColor ;
			DrawVert[ 5 ].color = DiffuseColor ;

			VertType = DrawTex->VertType ;
			TexVert  = DrawTex->Vertex ;

			for( j = 0 ; j < 4 ; j ++, TexVert ++ )
			{
				// ���_���W�̎Z�o
				if( *VertType ++ )
				{
					// �O�p�̏㑤
					DrawPos[ j ].x = ( TexVert->x * ExRate1X1 ) + ( TexVert->y * ExRate1X2 ) + xf1 ;
					DrawPos[ j ].y = ( TexVert->y * ExRate1Y1 ) + ( TexVert->x * ExRate1Y2 ) + yf1 ;
				} 
				else
				{
					// �O�p�̉���
					XBuf = TexVert->x - GraphSizeX ;
					YBuf = TexVert->y - GraphSizeY ;
					DrawPos[ j ].x = ( XBuf * ExRate2X1 ) + ( YBuf * ExRate2X2 ) + xf4 ;
					DrawPos[ j ].y = ( YBuf * ExRate2Y1 ) + ( XBuf * ExRate2Y2 ) + yf4 ;
				}
			}

			// ���]���菈��
			Reverse0 =  ( DrawPos[ 1 ].x - DrawPos[ 0 ].x ) * ( DrawPos[ 2 ].y - DrawPos[ 0 ].y ) -
						( DrawPos[ 2 ].x - DrawPos[ 0 ].x ) * ( DrawPos[ 1 ].y - DrawPos[ 0 ].y ) ;
//			Reverse1 = -( DrawPos[ 1 ].x - DrawPos[ 3 ].x ) * ( DrawPos[ 2 ].y - DrawPos[ 3 ].y ) +
//						( DrawPos[ 1 ].y - DrawPos[ 3 ].y ) * ( DrawPos[ 2 ].x - DrawPos[ 3 ].x ) ;
			TexVert = DrawTex->Vertex ;
			if( Reverse0 <= 0 )
			{
				DrawVert[ 0 ].pos.x = DrawPos[ 0 ].x ;
				DrawVert[ 0 ].pos.y = DrawPos[ 0 ].y ;
				DrawVert[ 1 ].pos.x = DrawPos[ 2 ].x ;
				DrawVert[ 1 ].pos.y = DrawPos[ 2 ].y ;
				DrawVert[ 2 ].pos.x = DrawPos[ 1 ].x ;
				DrawVert[ 2 ].pos.y = DrawPos[ 1 ].y ;
				DrawVert[ 3 ].pos.x = DrawPos[ 3 ].x ;
				DrawVert[ 3 ].pos.y = DrawPos[ 3 ].y ;
				DrawVert[ 4 ].pos.x = DrawPos[ 1 ].x ;
				DrawVert[ 4 ].pos.y = DrawPos[ 1 ].y ;
				DrawVert[ 5 ].pos.x = DrawPos[ 2 ].x ;
				DrawVert[ 5 ].pos.y = DrawPos[ 2 ].y ;

				DrawVert[ 0 ].u = TexVert[ 0 ].u ;
				DrawVert[ 0 ].v = TexVert[ 0 ].v ;
				DrawVert[ 1 ].u = TexVert[ 2 ].u ;
				DrawVert[ 1 ].v = TexVert[ 2 ].v ;
				DrawVert[ 2 ].u = TexVert[ 1 ].u ;
				DrawVert[ 2 ].v = TexVert[ 1 ].v ;
				DrawVert[ 3 ].u = TexVert[ 3 ].u ;
				DrawVert[ 3 ].v = TexVert[ 3 ].v ;
				DrawVert[ 4 ].u = TexVert[ 1 ].u ;
				DrawVert[ 4 ].v = TexVert[ 1 ].v ;
				DrawVert[ 5 ].u = TexVert[ 2 ].u ;
				DrawVert[ 5 ].v = TexVert[ 2 ].v ;
			}
			else
			{
				DrawVert[ 0 ].pos.x = DrawPos[ 0 ].x ;
				DrawVert[ 0 ].pos.y = DrawPos[ 0 ].y ;
				DrawVert[ 1 ].pos.x = DrawPos[ 1 ].x ;
				DrawVert[ 1 ].pos.y = DrawPos[ 1 ].y ;
				DrawVert[ 2 ].pos.x = DrawPos[ 2 ].x ;
				DrawVert[ 2 ].pos.y = DrawPos[ 2 ].y ;
				DrawVert[ 3 ].pos.x = DrawPos[ 3 ].x ;
				DrawVert[ 3 ].pos.y = DrawPos[ 3 ].y ;
				DrawVert[ 4 ].pos.x = DrawPos[ 2 ].x ;
				DrawVert[ 4 ].pos.y = DrawPos[ 2 ].y ;
				DrawVert[ 5 ].pos.x = DrawPos[ 1 ].x ;
				DrawVert[ 5 ].pos.y = DrawPos[ 1 ].y ;

				DrawVert[ 0 ].u = TexVert[ 0 ].u ;
				DrawVert[ 0 ].v = TexVert[ 0 ].v ;
				DrawVert[ 1 ].u = TexVert[ 1 ].u ;
				DrawVert[ 1 ].v = TexVert[ 1 ].v ;
				DrawVert[ 2 ].u = TexVert[ 2 ].u ;
				DrawVert[ 2 ].v = TexVert[ 2 ].v ;
				DrawVert[ 3 ].u = TexVert[ 3 ].u ;
				DrawVert[ 3 ].v = TexVert[ 3 ].v ;
				DrawVert[ 4 ].u = TexVert[ 2 ].u ;
				DrawVert[ 4 ].v = TexVert[ 2 ].v ;
				DrawVert[ 5 ].u = TexVert[ 1 ].u ;
				DrawVert[ 5 ].v = TexVert[ 1 ].v ;
			}

			*((DWORD *)&DrawVert[0].pos.z) = drawz ;
			*((DWORD *)&DrawVert[1].pos.z) = drawz ;
			*((DWORD *)&DrawVert[2].pos.z) = drawz ;
			*((DWORD *)&DrawVert[3].pos.z) = drawz ;
			*((DWORD *)&DrawVert[4].pos.z) = drawz ;
			*((DWORD *)&DrawVert[5].pos.z) = drawz ;

			// �`��I��
/*			if( Reverse0 * Reverse1 >= 0 )
			{
				// �v���~�e�B�u�̕`��
				Direct3DDevice9_DrawPrimitiveUP( D_D3DPT_TRIANGLESTRIP, 2, VertData, sizeof( VERTEX_2D ) ) ;
			}
			else
			{
				// �w�������]���̕␳
				if( Reverse0 < 0 ) 
				{
					VertBuf = VertData[ 1 ] ; VertData[ 1 ] = VertData[ 2 ] ; VertData[ 2 ] = VertBuf ;
				}

				// �v���~�e�B�u�̕`��
				Direct3DDevice9_DrawPrimitiveUP( D_D3DPT_TRIANGLESTRIP, 2, VertData, sizeof( VERTEX_2D ) ) ;
	//			GRH.Direct3DDeviceObject->DrawPrimitive( D_D3DPT_TRIANGLESTRIP, VERTEXFVF_2D, VertData,     3, 0 ) ;
	//			GRH.Direct3DDeviceObject->DrawPrimitive( D_D3DPT_TRIANGLESTRIP, VERTEXFVF_2D, &VertData[1], 3, 0 ) ;
			}
*/
			ADD4VERTEX_TEX

			i ++ ;
			if( i >= DrawTexNum ) break ;

			// �`�悷��e�N�X�`���[�̃Z�b�g
			DrawTex ++ ;
			Graphics_D3D9_DrawSetting_SetTexture( DrawTex->Tex->PF->D3D9.Texture ) ;
			Graphics_D3D9_DeviceState_RefreshBlendState() ;
		}
		break ;
	}

	// �I��
	return 0 ;
}

// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawModiGraphF
extern	int		Graphics_D3D9_DrawModiGraphF( float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, IMAGEDATA *Image, IMAGEDATA *BlendImage, int TransFlag, bool SimpleDrawFlag )
{
	int Flag ;

	if( GAPIWin.Direct3DDevice9Object == NULL ) return -1 ;

	// �����u�����h�e�N�X�`���t�̏ꍇ�͓����������s��Ȃ��`��֐����g�p����
	if( GSYS.DrawSetting.BlendGraph > 0 ) return Graphics_D3D9_DrawModiGraph( _FTOL( x1 ), _FTOL( y1 ), _FTOL( x2 ), _FTOL( y2 ), _FTOL( x3 ), _FTOL( y3 ), _FTOL( x4 ), _FTOL( y4 ), Image, BlendImage, TransFlag, SimpleDrawFlag ) ;

	// �`��̏���
	if( GD3D9.Device.DrawInfo.BeginSceneFlag == FALSE ) Graphics_D3D9_BeginScene() ;
	Flag = TransFlag ;
	DX_D3D9_DRAWPREP_TEX( Image->Orig, Image->Hard.Draw[ 0 ].Tex->PF->D3D9.Texture, Flag )

	// �e�N�X�`���[���P���݂̂̏ꍇ�͏����𕪊�
	if( Image->Hard.DrawNum == 1 )
	{
		// ���̂܂ܕ`��
		Graphics_D3D9_DrawModiTex( x1, y1, x2, y2, x3, y3, x4, y4, &Image->Hard.Draw[ 0 ], false ) ;
	}
	else
	{
		IMAGEDATA_HARD_DRAW *DrawTex ;
		IMAGEDATA_HARD_VERT *TexVert ;
		int DrawTexNum ;
		int i, j ;
		double xx1, yy1, xx2, yy2, xx3, yy3, xx4, yy4, xt1, yt1, xt2, yt2 ;
		double w, h, r ;
		float x[4], y[4], *xp, *yp ;

		xx1 = x2 - x1 ; xx2 = x4 - x3 ;
		yy1 = y2 - y1 ; yy2 = y4 - y3 ;

		xx3 = x3 - x1 ; xx4 = x4 - x2 ;
		yy3 = y3 - y1 ; yy4 = y4 - y2 ;
		
//		w = 1.0F / (float)Image->Width ;
//		h = 1.0F / (float)Image->Height ;
		w = 1.0F / (double)Image->WidthF ;
		h = 1.0F / (double)Image->HeightF ;

		// �`����̐����Z�b�g
		DrawTexNum = Image->Hard.DrawNum ;

		// �`����z��̃A�h���X���Z�b�g���Ă���
		DrawTex = Image->Hard.Draw ;

		// �e�N�X�`���[�̐������J��Ԃ�
		for( i = 0 ; i < DrawTexNum ; i ++, DrawTex ++ )
		{
			// �e�N�X�`���[�̃f�[�^�A�h���X���擾
			TexVert = DrawTex->Vertex ;

			// ���W���Z�o
			xp = x ;
			yp = y ;
			for( j = 0 ; j < 4 ; j ++, TexVert ++ )
			{
				r = TexVert->y * h ;
				xt1 = x1 + xx3 * r ;	yt1 = y1 + yy3 * r ;
				xt2 = x2 + xx4 * r ;	yt2 = y2 + yy4 * r ;

				r = TexVert->x * w ;
				*xp++ = (float)( ( xt2 - xt1 ) * r + xt1 ) ;
				*yp++ = (float)( ( yt2 - yt1 ) * r + yt1 ) ;
			}

			// �`��
			Graphics_D3D9_DrawModiTex( x[0], y[0], x[1], y[1], x[2], y[2], x[3], y[3], DrawTex, false ) ;
		}
	}

	// �I��
	return 0 ;
}

// �Q�����z��I�ɔz�u���ꂽ���_�f�[�^�𒸓_�o�b�t�@�ɒǉ�����
static int SetPlaneVertexHardware( VERTEX_2D *GraphVert, int xnum, int ynum )
{
	int i, j, k, l ;

	if( GAPIWin.Direct3DDevice9Object == NULL ) return 0 ;

	if( xnum < 2 || ynum < 2 ) return -1 ;

	// �g���C�A���O�����X�g���[�h�̏ꍇ�̓o�b�t�@�͎g�p���Ȃ�
	DRAWSTOCKINFO

	// �o�b�t�@���g�p���Ȃ��ݒ�ɂȂ��Ă����炱�̏�ŕ`��
	{
#define MAX_POSNUM		1000
		WORD list[MAX_POSNUM] ;

		// ���_�C���f�b�N�X���X�g���쐬����
		k = 0 ;
		l = 0 ;
		for( i = 0 ; i < ynum - 1 ; i ++ )
		{
			// ���_��������������������`��
			if( k + xnum * 2 + 2 > MAX_POSNUM )
			{
//				Graphics_D3D9_DeviceState_SetFVF( VERTEXFVF_2D ) ;
				Graphics_Hardware_D3D9_SetFVF_UseTable( VERTEXFVF_DECL_2D ) ;
				Direct3DDevice9_DrawIndexedPrimitiveUP(
					D_D3DPT_TRIANGLESTRIP,
					0,
					( UINT )( xnum * ynum ),
					( UINT )( k - 2 ),
					list,
					D_D3DFMT_INDEX16,
					GraphVert,
					sizeof( VERTEX_2D ) ) ;
				k = 0 ;
			}

			// �O�̍s����̑����������ꍇ�̓X�g���b�v�̘A���p�̒��_���o��		
			if( k != 0 )
			{
				list[k] = ( WORD )( l + xnum ) ;
				k ++ ;
			}
			
			for( j = 0 ; j < xnum ; j ++, l ++ )
			{
				list[k] = ( WORD )( l + xnum ) ;
				list[k+1] = ( WORD )l ;
				k += 2 ;
			}
			if( i != ynum - 2 )
			{
				list[k] = list[k-1] ;
				k ++ ;
			}
		}
		if( k != 0 )
		{
//			Graphics_D3D9_DeviceState_SetFVF( VERTEXFVF_2D ) ;
			Graphics_Hardware_D3D9_SetFVF_UseTable( VERTEXFVF_DECL_2D ) ;
			Direct3DDevice9_DrawIndexedPrimitiveUP(
				D_D3DPT_TRIANGLESTRIP,
				0,
				( UINT )( xnum * ynum ),
				( UINT )( k - 2 ),
				list,
				D_D3DFMT_INDEX16,
				GraphVert,
				sizeof( VERTEX_2D ) ) ;
		}
#undef	MAX_POSNUM
	}

	// �I��
	return 0 ;
}

// �e�N�X�`����ό`���ĕ`�悷��
extern	void	Graphics_D3D9_DrawModiTex( float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, IMAGEDATA_HARD_DRAW *DrawTex, bool SimpleDrawFlag )
{
	IMAGEDATA_HARD_VERT *TexVert ;
	VERTEX_2D *vec ; 
	VERTEX_2D VertBuf[2] ;
	float xx1, yy1, xx2, yy2, xx3, yy3, xx4, yy4/*, r1, r2, r3, r4*/ ;
//	float xtmp1, ytmp1, xtmp2, ytmp2 ;
	int c, f1, f2 ;
	DWORD DiffuseColor ;
	float Reverse0, Reverse1;
	DWORD drawz ;

	// �`�悷��e�N�X�`���[�̃Z�b�g
	Graphics_D3D9_DrawSetting_SetTexture( DrawTex->Tex->PF->D3D9.Texture ) ;
	Graphics_D3D9_DeviceState_RefreshBlendState() ;

	// ���_�f�[�^���擾�i���̂Ƃ���F���̃Z�b�g�̂݁j
	DiffuseColor = GD3D9.Device.DrawInfo.DiffuseColor ;

	// �y�o�b�t�@�ɏ������ނy�l���Z�b�g����
	drawz = *((DWORD *)&GSYS.DrawSetting.DrawZ);

	TexVert = DrawTex->Vertex ;

	// ���]����p����
	Reverse0 =  ( x2 - x1 ) * ( y3 - y1 ) - ( x3 - x1 ) * ( y2 - y1 ) ;
	Reverse1 = -( x2 - x4 ) * ( y3 - y4 ) + ( y2 - y4 ) * ( x3 - x4 ) ;

	if( SimpleDrawFlag ) goto R1 ;

	// ���s�l�ӌ`�ȏ�̕ό`���N���Ă��Ȃ������畁�ʂ̏��������s
	f1 = f2 = c = 0 ;
	xx1 = x2 - x1 ; xx2 = x4 - x3 ;
	yy1 = y2 - y1 ; yy2 = y4 - y3 ;

	xx3 = x3 - x1 ; xx4 = x4 - x2 ;
	yy3 = y3 - y1 ; yy4 = y4 - y2 ;

	if( _FABS( xx1 - xx2 ) < 1.0F && _FABS( yy1 - yy2 ) < 1.0F ) c ++, f1 = 1 ;
	if( _FABS( xx3 - xx4 ) < 1.0F && _FABS( yy3 - yy4 ) < 1.0F ) c ++, f2 = 1 ;
	if( c == 2 ) goto R1 ;

/*
	// ��`�����S�Ȏ��R�l�p�`���𔻒�
	f1 = f2 = c = 0 ;
	r1 = _SQRT( xx1 * xx1 + yy1 * yy1 ) ;	xtmp1 = xx1 / r1 ;	ytmp1 = yy1 / r1 ;
	r2 = _SQRT( xx2 * xx2 + yy2 * yy2 ) ;	xtmp2 = xx2 / r2 ;	ytmp2 = yy2 / r2 ;
	if( _FABS( xtmp1 - xtmp2 ) < 0.001F && _FABS( ytmp1 - ytmp2 ) < 0.001F ) c ++, f1 = 1 ;
	
	r3 = _SQRT( xx3 * xx3 + yy3 * yy3 ) ;	xtmp1 = xx3 / r3 ;	ytmp1 = yy3 / r3 ;
	r4 = _SQRT( xx4 * xx4 + yy4 * yy4 ) ;	xtmp2 = xx4 / r4 ;	ytmp2 = yy4 / r4 ;
	if( _FABS( xtmp1 - xtmp2 ) < 0.001F && _FABS( ytmp1 - ytmp2 ) < 0.001F ) c ++, f2 = 1 ;
*/

	{
#define TDIVNUM		5
#define TDN			(1 + ( 1 << TDIVNUM ))
#define ADJUSTUV	(0.001f)
		int DivNum, tdn ;
		static VERTEX_2D VertData2[TDN * TDN] ;
		VERTEX_2D *v ;
		float xxx1, yyy1, xxx2, yyy2, xxx3, yyy3 ;
		float adx1, ady1, adx2, ady2, adx3, ady3, adtu, adtv, tu, tv ; 
		int i, j ;
		float adjust_u_l, adjust_u_r, adjust_v_t, adjust_v_b ;

		// �����摜�̏ꍇ�ׂ̉摜�̃s�N�Z��������Ȃ����߂̕␳�t�u�l���쐬����
		{
			if( TexVert[ 0 ].u < 0.000001f )
			{
				adjust_u_l = 0.0f ;
			}
			else
			{
				adjust_u_l =  ( TexVert[ 3 ].u - TexVert[ 0 ].u ) / ( TexVert[ 3 ].x - TexVert[ 0 ].x ) * ADJUSTUV ;
			}

			if( TexVert[ 3 ].u > 0.99999f )
			{
				adjust_u_r = 0.0f ;
			}
			else
			{
				adjust_u_r = -( TexVert[ 3 ].u - TexVert[ 0 ].u ) / ( TexVert[ 3 ].x - TexVert[ 0 ].x ) * ADJUSTUV ;
			}

			if( TexVert[ 0 ].v < 0.000001f )
			{
				adjust_v_t = 0.0f ;
			}
			else
			{
				adjust_v_t =  ( TexVert[ 3 ].v - TexVert[ 0 ].v ) / ( TexVert[ 3 ].y - TexVert[ 0 ].y ) * ADJUSTUV ;
			}

			if( TexVert[ 3 ].v > 0.99999f )
			{
				adjust_v_b = 0.0f ;
			}
			else
			{
				adjust_v_b = -( TexVert[ 3 ].v - TexVert[ 0 ].v ) / ( TexVert[ 3 ].y - TexVert[ 0 ].y ) * ADJUSTUV ;
			}
		}

		// �������̎Z�o
		{
			float v1x, v1y, v2x, v2y ;
			float rate ;
			const float Table[] = { 2.0F, 1.0F, 0.6F, 0.1F, 0.05F } ;

			v1x = x4 - x1 ;		v1y = y4 - y1 ;
			v2x = x3 - x2 ;		v2y = y3 - y2 ;
			rate = (float)( 
				( _SQRT( v1x * v1x + v1y * v1y ) + _SQRT( v2x * v2x + v2y * v2y ) ) /
				( ( _SQRT( (float)( GSYS.DrawSetting.DrawSizeX * GSYS.DrawSetting.DrawSizeX + 
						GSYS.DrawSetting.DrawSizeY * GSYS.DrawSetting.DrawSizeY ) ) * 2 ) ) ) ;
			tdn = 8 ;
			if( tdn < 8 ) tdn = 8 ;
			else if( tdn > TDN ) tdn = TDN ;
			for( DivNum = TDIVNUM ; DivNum > 0 && rate < Table[TDIVNUM - DivNum] ; DivNum -- ){}
		}

		// ���R�l�p�`����
		adx1 = xx3 / ( tdn - 1 ) ;	ady1 = yy3 / ( tdn - 1 ) ;
		adx2 = xx4 / ( tdn - 1 ) ;	ady2 = yy4 / ( tdn - 1 ) ;
		xxx1 = x1 ;		yyy1 = y1 ;
		xxx2 = x2 ;		yyy2 = y2 ;
		adtu = ( ( TexVert[3].u + adjust_u_r ) - ( TexVert[0].u + adjust_u_l ) ) / ( tdn - 1 ) ;
		adtv = ( ( TexVert[3].v + adjust_v_b ) - ( TexVert[0].v + adjust_v_t ) ) / ( tdn - 1 ) ;
		tv = TexVert[0].v + adjust_v_t ;
		v = VertData2 ;
		for( i = 0 ; i < tdn ; i ++, tv += adtv,
									 xxx1 += adx1, yyy1 += ady1,
									 xxx2 += adx2, yyy2 += ady2 )
		{
			adx3 = ( xxx2 - xxx1 ) / ( tdn - 1 ) ;
			ady3 = ( yyy2 - yyy1 ) / ( tdn - 1 ) ;

//			xxx3 = xxx1 - 0.5F ;	yyy3 = yyy1 - 0.5F ;
			xxx3 = xxx1 ;
			yyy3 = yyy1 ;
			tu = TexVert[0].u + adjust_u_l ;
			for( j = 0 ; j < tdn ; j ++, tu += adtu,
										 xxx3 += adx3, yyy3 += ady3, v ++ )
			{
				v->pos.x = (float)xxx3 ;
				v->pos.y = (float)yyy3 ;
				*((DWORD *)&v->pos.z) = drawz ;
				v->rhw   = 1.0F ;
				v->u     = (float)tu ;
				v->v     = (float)tv ;
				v->color = DiffuseColor ;
			}
		}

		// ���_�ǉ�����
		SetPlaneVertexHardware( VertData2, tdn, tdn ) ;

#undef TDIVNUM
#undef TDN
	}

	return ;
		
R1 :
	// ���ʂɕ`��
	GETVERTEX_QUAD( vec )

	vec[0].color =
	vec[3].color = DiffuseColor ;

	vec[0].pos.x = x1 ;	
	vec[5].pos.x = vec[1].pos.x = x2 ;	
	vec[4].pos.x = vec[2].pos.x = x3 ;
	vec[3].pos.x = x4 ;
	
	vec[0].pos.y = y1 ;
	vec[5].pos.y = vec[1].pos.y = y2 ;
	vec[4].pos.y = vec[2].pos.y = y3 ;
	vec[3].pos.y = y4 ;

	vec[4].u = vec[2].u = vec[0].u = TexVert[0].u + 0.0625f / DrawTex->Tex->TexWidth ;
	vec[5].u = vec[3].u = vec[1].u = TexVert[1].u - 0.0625f / DrawTex->Tex->TexWidth ;
	vec[5].v = vec[1].v = vec[0].v = TexVert[0].v + 0.0625f / DrawTex->Tex->TexHeight ;
	vec[4].v = vec[3].v = vec[2].v = TexVert[2].v - 0.0625f / DrawTex->Tex->TexHeight ;

	*((DWORD *)&vec[0].pos.z) = drawz ;
	*((DWORD *)&vec[1].pos.z) = drawz ;
	*((DWORD *)&vec[2].pos.z) = drawz ;
	*((DWORD *)&vec[3].pos.z) = drawz ;
	*((DWORD *)&vec[4].pos.z) = drawz ;
	*((DWORD *)&vec[5].pos.z) = drawz ;

	if( Reverse0 <= 0  &&  Reverse1 <= 0 )
	{
		VertBuf[ 0 ] = vec[ 1 ] ;
		VertBuf[ 1 ] = vec[ 4 ] ;
		vec[ 1 ]     = vec[ 2 ] ;
		vec[ 4 ]     = vec[ 5 ] ;
		vec[ 2 ]     = VertBuf[ 0 ] ;
		vec[ 5 ]     = VertBuf[ 1 ] ;
	}

	ADD4VERTEX_TEX
}

// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawSimpleQuadrangleGraphF
extern	int		Graphics_D3D9_DrawSimpleQuadrangleGraphF( const GRAPHICS_DRAW_DRAWSIMPLEQUADRANGLEGRAPHF_PARAM *Param, IMAGEDATA *Image, IMAGEDATA *BlendImage )
{
	VERTEX_2D *DrawVert ;
	VERTEX_BLENDTEX_2D *DrawVertB ;
	DWORD DiffuseColor ;
	IMAGEDATA_HARD_DRAW *DrawTex ;
	IMAGEDATA_HARD_DRAW *BlendDrawTex = NULL ;
	IMAGEDATA_HARD_VERT *TexVert ;
	IMAGEDATA_HARD_VERT *BlendTexVert ;
	DWORD drawz ;
	int Flag ;
	float u[ 4 ] ;
	float v[ 4 ] ;
	int i ;
	const GRAPHICS_DRAW_DRAWSIMPLEANGLEGRAPHF_VERTEX *ParamV ;

	if( GAPIWin.Direct3DDevice9Object == NULL )
	{
		return -1 ;
	}

	// �e�N�X�`���̐����P�ȊO�̏ꍇ�̓G���[
	if( Image->Hard.DrawNum != 1 )
	{
		return -1 ;
	}

	// �`��̏���
	if( GD3D9.Device.DrawInfo.BeginSceneFlag == FALSE )
	{
		Graphics_D3D9_BeginScene() ;
	}
	Flag = Param->TransFlag ;
	DX_D3D9_DRAWPREP_TEX( Image->Orig, Image->Hard.Draw[ 0 ].Tex->PF->D3D9.Texture, Flag )

	// ���_�f�[�^���擾�i���̂Ƃ���F���̃Z�b�g�̂݁j
	DiffuseColor = GD3D9.Device.DrawInfo.DiffuseColor ;

	// �`����z��̃A�h���X���Z�b�g���Ă���
	DrawTex = Image->Hard.Draw ;
	if( BlendImage != NULL )
	{
		BlendDrawTex = BlendImage->Hard.Draw ;
	}

	// �y�o�b�t�@�ɏ������ނy�l���Z�b�g����
	drawz = *((DWORD *)&GSYS.DrawSetting.DrawZ);

	// ���_�^�C�v�ɂ���ď����𕪊�
	switch( GD3D9.Device.DrawInfo.VertexType )
	{
	case VERTEXTYPE_BLENDTEX :	// �u�����h�e�N�X�`�����g�p����
		// �`�悷��e�N�X�`���[�̃Z�b�g
		Graphics_D3D9_DrawSetting_SetTexture( DrawTex->Tex->PF->D3D9.Texture ) ;
		Graphics_D3D9_DrawSetting_SetBlendTexture( BlendDrawTex->Tex->PF->D3D9.Texture, BlendDrawTex->Tex->TexWidth, BlendDrawTex->Tex->TexHeight );

		if( GD3D9.Device.DrawSetting.ChangeTextureFlag )
		{
			Graphics_D3D9_DeviceState_RefreshBlendState() ;
		}

		TexVert      = DrawTex->Vertex ;
		BlendTexVert = BlendDrawTex->Vertex ;

		// ���_�f�[�^������������
		ParamV = Param->Vertex ;
		for( i = 0 ; i < Param->QuadrangleNum ; i ++, ParamV += 4 )
		{
			GETVERTEX_QUAD( DrawVertB )

			DrawVertB[0].color = ( DWORD )DiffuseColor ;
			DrawVertB[1].color = ( DWORD )DiffuseColor ;
			DrawVertB[2].color = ( DWORD )DiffuseColor ;
			DrawVertB[3].color = ( DWORD )DiffuseColor ;
			DrawVertB[4].color = ( DWORD )DiffuseColor ;
			DrawVertB[5].color = ( DWORD )DiffuseColor ;

			DrawVertB[0].pos.x = ParamV[ 0 ].x ;
			DrawVertB[0].pos.y = ParamV[ 0 ].y ;
			DrawVertB[1].pos.x = ParamV[ 1 ].x ;
			DrawVertB[1].pos.y = ParamV[ 1 ].y ;
			DrawVertB[2].pos.x = ParamV[ 2 ].x ;
			DrawVertB[2].pos.y = ParamV[ 2 ].y ;
			DrawVertB[3].pos.x = ParamV[ 3 ].x ;
			DrawVertB[3].pos.y = ParamV[ 3 ].y ;
			DrawVertB[4].pos.x = ParamV[ 2 ].x ;
			DrawVertB[4].pos.y = ParamV[ 2 ].y ;
			DrawVertB[5].pos.x = ParamV[ 1 ].x ;
			DrawVertB[5].pos.y = ParamV[ 1 ].y ;

			switch( GSYS.DrawSetting.BlendGraphPosMode )
			{
			default :
			case DX_BLENDGRAPH_POSMODE_DRAWGRAPH:
				u[ 0 ] = ( TexVert[0].x - BlendTexVert[0].x + GSYS.DrawSetting.BlendGraphX ) * GD3D9.Device.DrawSetting.InvBlendTextureWidth ;
				v[ 0 ] = ( TexVert[0].y - BlendTexVert[0].y + GSYS.DrawSetting.BlendGraphY ) * GD3D9.Device.DrawSetting.InvBlendTextureHeight ;
				u[ 1 ] = ( TexVert[1].x - BlendTexVert[1].x + GSYS.DrawSetting.BlendGraphX ) * GD3D9.Device.DrawSetting.InvBlendTextureWidth ;
				v[ 1 ] = ( TexVert[1].y - BlendTexVert[1].y + GSYS.DrawSetting.BlendGraphY ) * GD3D9.Device.DrawSetting.InvBlendTextureHeight ;
				u[ 2 ] = ( TexVert[2].x - BlendTexVert[2].x + GSYS.DrawSetting.BlendGraphX ) * GD3D9.Device.DrawSetting.InvBlendTextureWidth ;
				v[ 2 ] = ( TexVert[2].y - BlendTexVert[2].y + GSYS.DrawSetting.BlendGraphY ) * GD3D9.Device.DrawSetting.InvBlendTextureHeight ;
				u[ 3 ] = ( TexVert[3].x - BlendTexVert[3].x + GSYS.DrawSetting.BlendGraphX ) * GD3D9.Device.DrawSetting.InvBlendTextureWidth ;
				v[ 3 ] = ( TexVert[3].y - BlendTexVert[3].y + GSYS.DrawSetting.BlendGraphY ) * GD3D9.Device.DrawSetting.InvBlendTextureHeight ;
				break ;

			case DX_BLENDGRAPH_POSMODE_SCREEN :
				u[ 0 ] = ( ParamV[0].x - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D9.Device.DrawSetting.InvBlendTextureWidth ;
				v[ 0 ] = ( ParamV[0].y - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D9.Device.DrawSetting.InvBlendTextureHeight ;
				u[ 1 ] = ( ParamV[1].x - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D9.Device.DrawSetting.InvBlendTextureWidth ;
				v[ 1 ] = ( ParamV[1].y - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D9.Device.DrawSetting.InvBlendTextureHeight ;
				u[ 2 ] = ( ParamV[2].x - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D9.Device.DrawSetting.InvBlendTextureWidth ;
				v[ 2 ] = ( ParamV[2].y - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D9.Device.DrawSetting.InvBlendTextureHeight ;
				u[ 3 ] = ( ParamV[3].x - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D9.Device.DrawSetting.InvBlendTextureWidth ;
				v[ 3 ] = ( ParamV[3].y - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D9.Device.DrawSetting.InvBlendTextureHeight ;
				break ;
			}

			DrawVertB[0].u1 = u[ 0 ] ;
			DrawVertB[0].v1 = v[ 0 ] ;
			DrawVertB[1].u1 = u[ 1 ] ;
			DrawVertB[1].v1 = v[ 1 ] ;
			DrawVertB[2].u1 = u[ 2 ] ;
			DrawVertB[2].v1 = v[ 2 ] ;
			DrawVertB[3].u1 = u[ 3 ] ;
			DrawVertB[3].v1 = v[ 3 ] ;
			DrawVertB[4].u1 = u[ 2 ] ;
			DrawVertB[4].v1 = v[ 2 ] ;
			DrawVertB[5].u1 = u[ 1 ] ;
			DrawVertB[5].v1 = v[ 1 ] ;

			DrawVertB[0].u2 = ParamV[ 0 ].u ;
			DrawVertB[0].v2 = ParamV[ 0 ].v ;
			DrawVertB[1].u2 = ParamV[ 1 ].u ;
			DrawVertB[1].v2 = ParamV[ 1 ].v ;
			DrawVertB[2].u2 = ParamV[ 2 ].u ;
			DrawVertB[2].v2 = ParamV[ 2 ].v ;
			DrawVertB[3].u2 = ParamV[ 3 ].u ;
			DrawVertB[3].v2 = ParamV[ 3 ].v ;
			DrawVertB[4].u2 = ParamV[ 2 ].u ;
			DrawVertB[4].v2 = ParamV[ 2 ].v ;
			DrawVertB[5].u2 = ParamV[ 1 ].u ;
			DrawVertB[5].v2 = ParamV[ 1 ].v ;

			*((DWORD *)&DrawVertB[0].pos.z) = drawz ;
			*((DWORD *)&DrawVertB[1].pos.z) = drawz ;
			*((DWORD *)&DrawVertB[2].pos.z) = drawz ;
			*((DWORD *)&DrawVertB[3].pos.z) = drawz ;
			*((DWORD *)&DrawVertB[4].pos.z) = drawz ;
			*((DWORD *)&DrawVertB[5].pos.z) = drawz ;

			DrawVertB[0].rhw = 1.0f ;
			DrawVertB[1].rhw = 1.0f ;
			DrawVertB[2].rhw = 1.0f ;
			DrawVertB[3].rhw = 1.0f ;
			DrawVertB[4].rhw = 1.0f ;
			DrawVertB[5].rhw = 1.0f ;

			// �e�N�X�`���[��`�悷��
			ADD4VERTEX_BLENDTEX
		}
		break ;

	case VERTEXTYPE_TEX :	// �u�����h�e�N�X�`�����g�p���Ȃ�
		TexVert = DrawTex->Vertex ;

		// ���_�f�[�^������������
		ParamV = Param->Vertex ;
		for( i = 0 ; i < Param->QuadrangleNum ; i ++, ParamV += 4 )
		{
			GETVERTEX_QUAD( DrawVert )

			DrawVert[0].color = ( DWORD )DiffuseColor ;
			DrawVert[1].color = ( DWORD )DiffuseColor ;
			DrawVert[2].color = ( DWORD )DiffuseColor ;
			DrawVert[3].color = ( DWORD )DiffuseColor ;
			DrawVert[4].color = ( DWORD )DiffuseColor ;
			DrawVert[5].color = ( DWORD )DiffuseColor ;

			DrawVert[0].u = ParamV[ 0 ].u ;
			DrawVert[0].v = ParamV[ 0 ].v ;
			DrawVert[1].u = ParamV[ 1 ].u ;
			DrawVert[1].v = ParamV[ 1 ].v ;
			DrawVert[2].u = ParamV[ 2 ].u ;
			DrawVert[2].v = ParamV[ 2 ].v ;
			DrawVert[3].u = ParamV[ 3 ].u ;
			DrawVert[3].v = ParamV[ 3 ].v ;
			DrawVert[4].u = ParamV[ 2 ].u ;
			DrawVert[4].v = ParamV[ 2 ].v ;
			DrawVert[5].u = ParamV[ 1 ].u ;
			DrawVert[5].v = ParamV[ 1 ].v ;

			DrawVert[0].pos.x = ParamV[ 0 ].x ;
			DrawVert[0].pos.y = ParamV[ 0 ].y ;
			DrawVert[1].pos.x = ParamV[ 1 ].x ;
			DrawVert[1].pos.y = ParamV[ 1 ].y ;
			DrawVert[2].pos.x = ParamV[ 2 ].x ;
			DrawVert[2].pos.y = ParamV[ 2 ].y ;
			DrawVert[3].pos.x = ParamV[ 3 ].x ;
			DrawVert[3].pos.y = ParamV[ 3 ].y ;
			DrawVert[4].pos.x = ParamV[ 2 ].x ;
			DrawVert[4].pos.y = ParamV[ 2 ].y ;
			DrawVert[5].pos.x = ParamV[ 1 ].x ;
			DrawVert[5].pos.y = ParamV[ 1 ].y ;

			*((DWORD *)&DrawVert[0].pos.z) = drawz ;
			*((DWORD *)&DrawVert[1].pos.z) = drawz ;
			*((DWORD *)&DrawVert[2].pos.z) = drawz ;
			*((DWORD *)&DrawVert[3].pos.z) = drawz ;
			*((DWORD *)&DrawVert[4].pos.z) = drawz ;
			*((DWORD *)&DrawVert[5].pos.z) = drawz ;

			// �e�N�X�`���[��`�悷��
			ADD4VERTEX_TEX
		}
		break ;
	}

	// �I��
	return 0 ;
}

// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawSimpleTriangleGraphF
extern	int		Graphics_D3D9_DrawSimpleTriangleGraphF( const GRAPHICS_DRAW_DRAWSIMPLETRIANGLEGRAPHF_PARAM *Param, IMAGEDATA *Image, IMAGEDATA *BlendImage )
{
	VERTEX_2D *DrawVert ;
	VERTEX_BLENDTEX_2D *DrawVertB ;
	DWORD DiffuseColor ;
	IMAGEDATA_HARD_DRAW *DrawTex ;
	IMAGEDATA_HARD_DRAW *BlendDrawTex = NULL ;
	IMAGEDATA_HARD_VERT *TexVert ;
	IMAGEDATA_HARD_VERT *BlendTexVert ;
	DWORD drawz ;
	int Flag ;
	float u[ 3 ] ;
	float v[ 3 ] ;
	int i ;
	const GRAPHICS_DRAW_DRAWSIMPLEANGLEGRAPHF_VERTEX *ParamV ;

	if( GAPIWin.Direct3DDevice9Object == NULL )
	{
		return -1 ;
	}

	// �e�N�X�`���̐����P�ȊO�̏ꍇ�̓G���[
	if( Image->Hard.DrawNum != 1 )
	{
		return -1 ;
	}

	// �`��̏���
	if( GD3D9.Device.DrawInfo.BeginSceneFlag == FALSE )
	{
		Graphics_D3D9_BeginScene() ;
	}
	Flag = Param->TransFlag ;
	DX_D3D9_DRAWPREP_TEX( Image->Orig, Image->Hard.Draw[ 0 ].Tex->PF->D3D9.Texture, Flag )

	// ���_�f�[�^���擾�i���̂Ƃ���F���̃Z�b�g�̂݁j
	DiffuseColor = GD3D9.Device.DrawInfo.DiffuseColor ;

	// �`����z��̃A�h���X���Z�b�g���Ă���
	DrawTex = Image->Hard.Draw ;
	if( BlendImage != NULL )
	{
		BlendDrawTex = BlendImage->Hard.Draw ;
	}

	// �y�o�b�t�@�ɏ������ނy�l���Z�b�g����
	drawz = *((DWORD *)&GSYS.DrawSetting.DrawZ);

	// ���_�^�C�v�ɂ���ď����𕪊�
	switch( GD3D9.Device.DrawInfo.VertexType )
	{
	case VERTEXTYPE_BLENDTEX :	// �u�����h�e�N�X�`�����g�p����
		// �`�悷��e�N�X�`���[�̃Z�b�g
		Graphics_D3D9_DrawSetting_SetTexture( DrawTex->Tex->PF->D3D9.Texture ) ;
		Graphics_D3D9_DrawSetting_SetBlendTexture( BlendDrawTex->Tex->PF->D3D9.Texture, BlendDrawTex->Tex->TexWidth, BlendDrawTex->Tex->TexHeight );

		if( GD3D9.Device.DrawSetting.ChangeTextureFlag )
		{
			Graphics_D3D9_DeviceState_RefreshBlendState() ;
		}

		TexVert      = DrawTex->Vertex ;
		BlendTexVert = BlendDrawTex->Vertex ;

		// ���_�f�[�^������������
		ParamV = Param->Vertex ;
		for( i = 0 ; i < Param->TriangleNum ; i ++, ParamV += 3 )
		{
			GETVERTEX_TRIANGLE( DrawVertB )

			DrawVertB[0].color = ( DWORD )DiffuseColor ;
			DrawVertB[1].color = ( DWORD )DiffuseColor ;
			DrawVertB[2].color = ( DWORD )DiffuseColor ;

			DrawVertB[0].pos.x = ParamV[ 0 ].x ;
			DrawVertB[0].pos.y = ParamV[ 0 ].y ;
			DrawVertB[1].pos.x = ParamV[ 1 ].x ;
			DrawVertB[1].pos.y = ParamV[ 1 ].y ;
			DrawVertB[2].pos.x = ParamV[ 2 ].x ;
			DrawVertB[2].pos.y = ParamV[ 2 ].y ;

			switch( GSYS.DrawSetting.BlendGraphPosMode )
			{
			default :
			case DX_BLENDGRAPH_POSMODE_DRAWGRAPH:
				u[ 0 ] = ( TexVert[0].x - BlendTexVert[0].x + GSYS.DrawSetting.BlendGraphX ) * GD3D9.Device.DrawSetting.InvBlendTextureWidth ;
				v[ 0 ] = ( TexVert[0].y - BlendTexVert[0].y + GSYS.DrawSetting.BlendGraphY ) * GD3D9.Device.DrawSetting.InvBlendTextureHeight ;
				u[ 1 ] = ( TexVert[1].x - BlendTexVert[1].x + GSYS.DrawSetting.BlendGraphX ) * GD3D9.Device.DrawSetting.InvBlendTextureWidth ;
				v[ 1 ] = ( TexVert[1].y - BlendTexVert[1].y + GSYS.DrawSetting.BlendGraphY ) * GD3D9.Device.DrawSetting.InvBlendTextureHeight ;
				u[ 2 ] = ( TexVert[2].x - BlendTexVert[2].x + GSYS.DrawSetting.BlendGraphX ) * GD3D9.Device.DrawSetting.InvBlendTextureWidth ;
				v[ 2 ] = ( TexVert[2].y - BlendTexVert[2].y + GSYS.DrawSetting.BlendGraphY ) * GD3D9.Device.DrawSetting.InvBlendTextureHeight ;
				break ;

			case DX_BLENDGRAPH_POSMODE_SCREEN :
				u[ 0 ] = ( ParamV[0].x - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D9.Device.DrawSetting.InvBlendTextureWidth ;
				v[ 0 ] = ( ParamV[0].y - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D9.Device.DrawSetting.InvBlendTextureHeight ;
				u[ 1 ] = ( ParamV[1].x - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D9.Device.DrawSetting.InvBlendTextureWidth ;
				v[ 1 ] = ( ParamV[1].y - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D9.Device.DrawSetting.InvBlendTextureHeight ;
				u[ 2 ] = ( ParamV[2].x - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D9.Device.DrawSetting.InvBlendTextureWidth ;
				v[ 2 ] = ( ParamV[2].y - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D9.Device.DrawSetting.InvBlendTextureHeight ;
				break ;
			}

			DrawVertB[0].u1 = u[ 0 ] ;
			DrawVertB[0].v1 = v[ 0 ] ;
			DrawVertB[1].u1 = u[ 1 ] ;
			DrawVertB[1].v1 = v[ 1 ] ;
			DrawVertB[2].u1 = u[ 2 ] ;
			DrawVertB[2].v1 = v[ 2 ] ;

			DrawVertB[0].u2 = ParamV[ 0 ].u ;
			DrawVertB[0].v2 = ParamV[ 0 ].v ;
			DrawVertB[1].u2 = ParamV[ 1 ].u ;
			DrawVertB[1].v2 = ParamV[ 1 ].v ;
			DrawVertB[2].u2 = ParamV[ 2 ].u ;
			DrawVertB[2].v2 = ParamV[ 2 ].v ;

			*((DWORD *)&DrawVertB[0].pos.z) = drawz ;
			*((DWORD *)&DrawVertB[1].pos.z) = drawz ;
			*((DWORD *)&DrawVertB[2].pos.z) = drawz ;

			DrawVertB[0].rhw = 1.0f ;
			DrawVertB[1].rhw = 1.0f ;
			DrawVertB[2].rhw = 1.0f ;

			// �e�N�X�`���[��`�悷��
			ADD3VERTEX_BLENDTEX
		}
		break ;

	case VERTEXTYPE_TEX :	// �u�����h�e�N�X�`�����g�p���Ȃ�
		TexVert = DrawTex->Vertex ;

		// ���_�f�[�^������������
		ParamV = Param->Vertex ;
		for( i = 0 ; i < Param->TriangleNum ; i ++, ParamV += 3 )
		{
			GETVERTEX_TRIANGLE( DrawVert )

			DrawVert[0].color = ( DWORD )DiffuseColor ;
			DrawVert[1].color = ( DWORD )DiffuseColor ;
			DrawVert[2].color = ( DWORD )DiffuseColor ;

			DrawVert[0].u = ParamV[ 0 ].u ;
			DrawVert[0].v = ParamV[ 0 ].v ;
			DrawVert[1].u = ParamV[ 1 ].u ;
			DrawVert[1].v = ParamV[ 1 ].v ;
			DrawVert[2].u = ParamV[ 2 ].u ;
			DrawVert[2].v = ParamV[ 2 ].v ;

			DrawVert[0].pos.x = ParamV[ 0 ].x ;
			DrawVert[0].pos.y = ParamV[ 0 ].y ;
			DrawVert[1].pos.x = ParamV[ 1 ].x ;
			DrawVert[1].pos.y = ParamV[ 1 ].y ;
			DrawVert[2].pos.x = ParamV[ 2 ].x ;
			DrawVert[2].pos.y = ParamV[ 2 ].y ;

			*((DWORD *)&DrawVert[0].pos.z) = drawz ;
			*((DWORD *)&DrawVert[1].pos.z) = drawz ;
			*((DWORD *)&DrawVert[2].pos.z) = drawz ;

			// �e�N�X�`���[��`�悷��
			ADD3VERTEX_TEX
		}
		break ;
	}

	// �I��
	return 0 ;
}

// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawFillBox
extern	int		Graphics_D3D9_DrawFillBox( int x1, int y1, int x2, int y2, unsigned int Color )
{
	VERTEX_NOTEX_2D *vec ;
	RECT drect ;
	int Red, Green, Blue, Flag ;
	DWORD drawz ;

	if( GAPIWin.Direct3DDevice9Object == NULL ) return -1 ;

	// ���]����
	{
		int b ;

		if( x1 > x2 ){ b = x1 ; x1 = x2 ; x2 = b ; }
		if( y1 > y2 ){ b = y1 ; y1 = y2 ; y2 = b ; }
	}
	
	// �N���b�s���O����
	{
		drect.left   = x1 ;
		drect.right  = x2 ;
		drect.top    = y1 ;
		drect.bottom = y2 ;
		RectClipping_Inline( &drect, &GSYS.DrawSetting.DrawArea ) ;
		if( drect.left == drect.right || drect.top == drect.bottom ) return 0 ;
	}

	// �`��̏���
	Graphics_D3D9_BeginScene() ;
	Flag = DX_D3D9_DRAWPREP_DIFFUSERGB ;
	DX_D3D9_DRAWPREP_NOTEX( Flag ) ;

	// �F���̑��X�e�[�^�X�̃Z�b�g
	NS_GetColor2( Color, &Red, &Green, &Blue ) ;
	SETUPCOLOR( Color )

	// �y�o�b�t�@�ɏ������ނy�l���Z�b�g����
	drawz = *((DWORD *)&GSYS.DrawSetting.DrawZ);

	// ���_�f�[�^�̃Z�b�g
	GETVERTEX_QUAD( vec )
	
	vec[0].color =
	vec[3].color = Color ;
	
	vec[4].pos.x = vec[2].pos.x = vec[0].pos.x = (float)drect.left   ;
	vec[5].pos.x = vec[3].pos.x = vec[1].pos.x = (float)drect.right  ;
	vec[5].pos.y = vec[1].pos.y = vec[0].pos.y = (float)drect.top    ;
	vec[4].pos.y = vec[3].pos.y = vec[2].pos.y = (float)drect.bottom ;

	*((DWORD *)&vec[0].pos.z) = drawz ;
	*((DWORD *)&vec[1].pos.z) = drawz ;
	*((DWORD *)&vec[2].pos.z) = drawz ;
	*((DWORD *)&vec[3].pos.z) = drawz ;
	*((DWORD *)&vec[4].pos.z) = drawz ;
	*((DWORD *)&vec[5].pos.z) = drawz ;

	// ���_�f�[�^�̏o��
	ADD4VERTEX_NOTEX

	// �I��
	return 0 ;
}

// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawLineBox
extern	int		Graphics_D3D9_DrawLineBox( int x1, int y1, int x2, int y2, unsigned int Color )
{
	VERTEX_NOTEX_2D *VertData ;
	int Red, Green, Blue ;
	float fx1, fx2, fy1, fy2 ;
	int Flag;
	DWORD drawz ;

//	Graphics_D3D9_RenderVertex( D3D.Direct3DDeviceHandle ) ;

	if( GAPIWin.Direct3DDevice9Object == NULL ) return -1 ;

	// �`��̏���
	if( GD3D9.Device.DrawInfo.BeginSceneFlag == FALSE ) Graphics_D3D9_BeginScene() ;
	if( GD3D9.Device.DrawSetting.RenderTexture != NULL ) Graphics_D3D9_DrawSetting_SetTexture( NULL ) ;
	Flag = DX_D3D9_DRAWPREP_DIFFUSERGB ;
	DX_D3D9_DRAWPREP_NOTEX( Flag ) ;

	// �y�o�b�t�@�ɏ������ނy�l���Z�b�g����
	drawz = *((DWORD *)&GSYS.DrawSetting.DrawZ);

	// �F���̑��X�e�[�^�X�̃Z�b�g
	NS_GetColor2( Color, &Red, &Green, &Blue ) ;
	SETUPCOLOR( Color )

	// ���]����
	{
		int bx, by ;

		if( x1 > x2 || y1 > y2 )
		{
			bx = x1 ; by = y1 ;
			x1 = x2 ; y1 = y2 ;
			x2 = bx ; y2 = by ;
		}
	}

	fx1 = ( float )x1 ;
	fy1 = ( float )y1 ;
	fx2 = ( float )x2 ;
	fy2 = ( float )y2 ;

	GETVERTEX_LINEBOX( VertData ) ;

	VertData[ 0].color = Color ;
	VertData[ 1].color = Color ;
	VertData[ 2].color = Color ;
	VertData[ 3].color = Color ;
	VertData[ 4].color = Color ;
	VertData[ 5].color = Color ;
	VertData[ 6].color = Color ;
	VertData[ 7].color = Color ;
	VertData[ 8].color = Color ;
	VertData[ 9].color = Color ;
	VertData[10].color = Color ;
	VertData[11].color = Color ;
	VertData[12].color = Color ;
	VertData[13].color = Color ;
	VertData[14].color = Color ;
	VertData[15].color = Color ;
	VertData[16].color = Color ;
	VertData[17].color = Color ;
	VertData[18].color = Color ;
	VertData[19].color = Color ;
	VertData[20].color = Color ;
	VertData[21].color = Color ;
	VertData[22].color = Color ;
	VertData[23].color = Color ;

	*((DWORD *)&VertData[ 0].pos.z) = drawz ;
	*((DWORD *)&VertData[ 1].pos.z) = drawz ;
	*((DWORD *)&VertData[ 2].pos.z) = drawz ;
	*((DWORD *)&VertData[ 3].pos.z) = drawz ;
	*((DWORD *)&VertData[ 4].pos.z) = drawz ;
	*((DWORD *)&VertData[ 5].pos.z) = drawz ;
	*((DWORD *)&VertData[ 6].pos.z) = drawz ;
	*((DWORD *)&VertData[ 7].pos.z) = drawz ;
	*((DWORD *)&VertData[ 8].pos.z) = drawz ;
	*((DWORD *)&VertData[ 9].pos.z) = drawz ;
	*((DWORD *)&VertData[10].pos.z) = drawz ;
	*((DWORD *)&VertData[11].pos.z) = drawz ;
	*((DWORD *)&VertData[12].pos.z) = drawz ;
	*((DWORD *)&VertData[13].pos.z) = drawz ;
	*((DWORD *)&VertData[14].pos.z) = drawz ;
	*((DWORD *)&VertData[15].pos.z) = drawz ;
	*((DWORD *)&VertData[16].pos.z) = drawz ;
	*((DWORD *)&VertData[17].pos.z) = drawz ;
	*((DWORD *)&VertData[18].pos.z) = drawz ;
	*((DWORD *)&VertData[19].pos.z) = drawz ;
	*((DWORD *)&VertData[20].pos.z) = drawz ;
	*((DWORD *)&VertData[21].pos.z) = drawz ;
	*((DWORD *)&VertData[22].pos.z) = drawz ;
	*((DWORD *)&VertData[23].pos.z) = drawz ;

	VertData[0].pos.x = fx1			; VertData[0].pos.y = fy1		 ;
	VertData[1].pos.x = fx2			; VertData[1].pos.y = fy1		 ;
	VertData[2].pos.x = fx1			; VertData[2].pos.y = fy1 + 1.0f ;

	VertData[3].pos.x = fx2			; VertData[3].pos.y = fy1 + 1.0f ;
	VertData[4].pos.x = fx1			; VertData[4].pos.y = fy1 + 1.0f ;
	VertData[5].pos.x = fx2			; VertData[5].pos.y = fy1		 ;

	VertData += 6 ;


	VertData[0].pos.x = fx2 - 1.0f	; VertData[0].pos.y = fy1 + 1.0f ;
	VertData[1].pos.x = fx2			; VertData[1].pos.y = fy1 + 1.0f ;
	VertData[2].pos.x = fx2	- 1.0f	; VertData[2].pos.y = fy2		 ;

	VertData[3].pos.x = fx2			; VertData[3].pos.y = fy2		 ;
	VertData[4].pos.x = fx2 - 1.0f	; VertData[4].pos.y = fy2		 ;
	VertData[5].pos.x = fx2			; VertData[5].pos.y = fy1 + 1.0f ;

	VertData += 6 ;


	VertData[0].pos.x = fx1			; VertData[0].pos.y = fy1 + 1.0f ;
	VertData[1].pos.x = fx1 + 1.0f	; VertData[1].pos.y = fy1 + 1.0f ;
	VertData[2].pos.x = fx1			; VertData[2].pos.y = fy2		 ;

	VertData[3].pos.x = fx1 + 1.0f	; VertData[3].pos.y = fy2		 ;
	VertData[4].pos.x = fx1			; VertData[4].pos.y = fy2		 ;
	VertData[5].pos.x = fx1 + 1.0f	; VertData[5].pos.y = fy1 + 1.0f ;

	VertData += 6 ;


	VertData[0].pos.x = fx1 + 1.0f	; VertData[0].pos.y = fy2 - 1.0f ;
	VertData[1].pos.x = fx2 - 1.0f	; VertData[1].pos.y = fy2 - 1.0f ;
	VertData[2].pos.x = fx1 + 1.0f	; VertData[2].pos.y = fy2		 ;

	VertData[3].pos.x = fx2 - 1.0f	; VertData[3].pos.y = fy2		 ;
	VertData[4].pos.x = fx1 + 1.0f	; VertData[4].pos.y = fy2		 ;
	VertData[5].pos.x = fx2 - 1.0f	; VertData[5].pos.y = fy2 - 1.0f ;

	VertData += 6 ;

	// ���_�̒ǉ�
	ADD4VERTEX_LINEBOX

	// �I��
	return 0 ;
}



// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawLine
extern	int		Graphics_D3D9_DrawLine( int x1, int y1, int x2, int y2, unsigned int Color )
{
	VERTEX_NOTEX_2D *VertData ;
	int Red, Green, Blue, Flag ;
	DWORD drawz ;

	if( GAPIWin.Direct3DDevice9Object == NULL ) return -1 ; 

	if( x2 - x1 == 0 && y2 - y1 == 0 ) return 0 ; 

	// �`��̏���
	if( GD3D9.Device.DrawInfo.BeginSceneFlag == FALSE ) Graphics_D3D9_BeginScene() ;
	if( GD3D9.Device.DrawSetting.RenderTexture != NULL ) Graphics_D3D9_DrawSetting_SetTexture( NULL ) ;
	Flag = DX_D3D9_DRAWPREP_DIFFUSERGB ;
	DX_D3D9_DRAWPREP_NOTEX( Flag )

	// �y�o�b�t�@�ɏ������ނy�l���Z�b�g����
	drawz = *((DWORD *)&GSYS.DrawSetting.DrawZ);

	// �F���Z�b�g
	GETVERTEX_LINE( VertData ) ;
	NS_GetColor2( Color, &Red, &Green, &Blue ) ;
	SETUPCOLOR( Color )
	VertData[ 0 ].color = Color ;
	VertData[ 1 ].color = Color ;
	*((DWORD *)&VertData[ 0 ].pos.z) = drawz ;
	*((DWORD *)&VertData[ 1 ].pos.z) = drawz ;

	// ���W���Z�b�g
	VertData[ 0 ].pos.x = ( float )( x1 ) ;
	VertData[ 0 ].pos.y = ( float )( y1 ) ;

	VertData[ 1 ].pos.x = ( float )( x2 ) ;
	VertData[ 1 ].pos.y = ( float )( y2 ) ;

	// ���W�̕␳
	if( x1 == x2 ) VertData[ 1 ].pos.y += y2 > y1 ? -0.1F : 0.1F ;
	if( y1 == y2 ) VertData[ 1 ].pos.x += x2 > x1 ? -0.1F : 0.1F ;

	// ���_�̒ǉ�
	ADD4VERTEX_LINE

	// �I��
	return 0 ;
}

// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawLine3D
extern	int		Graphics_D3D9_DrawLine3D( VECTOR Pos1, VECTOR Pos2, unsigned int Color, int DrawFlag, RECT *DrawArea )
{
	VERTEX_NOTEX_3D *VertData ;
	int Red, Green, Blue, Flag ;

	if( DrawFlag )
	{
		if( GAPIWin.Direct3DDevice9Object == NULL ) return -1 ; 

		// �`��̏���
		if( GD3D9.Device.DrawInfo.BeginSceneFlag == FALSE ) Graphics_D3D9_BeginScene() ;
		if( GD3D9.Device.DrawSetting.RenderTexture != NULL ) Graphics_D3D9_DrawSetting_SetTexture( NULL ) ;

		Flag = DX_D3D9_DRAWPREP_DIFFUSERGB | DX_D3D9_DRAWPREP_3D | DX_D3D9_DRAWPREP_FOG ;
		DX_D3D9_DRAWPREP_NOTEX( Flag )

		GETVERTEX_LINE3D( VertData ) ;

		// �F���Z�b�g
		NS_GetColor2( Color, &Red, &Green, &Blue ) ;
		SETUPCOLOR( Color )
		*( ( DWORD * )&VertData[ 0 ].b ) = Color ;
		*( ( DWORD * )&VertData[ 1 ].b ) = Color ;

		// ���W���Z�b�g
		VertData[ 0 ].pos = Pos1 ;
		VertData[ 1 ].pos = Pos2 ;

		// ���_�̒ǉ�
		ADD4VERTEX_LINE3D
	}
	else
	{
		VERTEX_2D TempVert[ 2 ] ;
		float dleft, dright, dtop, dbottom ;

		dleft   = -100000000.0f ;
		dright  =  100000000.0f ;
		dtop    = -100000000.0f ;
		dbottom =  100000000.0f ;

		// �X�N���[�����W�ɕϊ�
		TempVert[0].pos.x = Pos1.x * GSYS.DrawSetting.BlendMatrixF.m[0][0] + Pos1.y * GSYS.DrawSetting.BlendMatrixF.m[1][0] + Pos1.z * GSYS.DrawSetting.BlendMatrixF.m[2][0] + GSYS.DrawSetting.BlendMatrixF.m[3][0]  ;
		TempVert[0].pos.y = Pos1.x * GSYS.DrawSetting.BlendMatrixF.m[0][1] + Pos1.y * GSYS.DrawSetting.BlendMatrixF.m[1][1] + Pos1.z * GSYS.DrawSetting.BlendMatrixF.m[2][1] + GSYS.DrawSetting.BlendMatrixF.m[3][1]  ;
		TempVert[0].pos.z = Pos1.x * GSYS.DrawSetting.BlendMatrixF.m[0][2] + Pos1.y * GSYS.DrawSetting.BlendMatrixF.m[1][2] + Pos1.z * GSYS.DrawSetting.BlendMatrixF.m[2][2] + GSYS.DrawSetting.BlendMatrixF.m[3][2]  ;
		TempVert[0].rhw   = Pos1.x * GSYS.DrawSetting.BlendMatrixF.m[0][3] + Pos1.y * GSYS.DrawSetting.BlendMatrixF.m[1][3] + Pos1.z * GSYS.DrawSetting.BlendMatrixF.m[2][3] + GSYS.DrawSetting.BlendMatrixF.m[3][3]  ;

		TempVert[1].pos.x = Pos2.x * GSYS.DrawSetting.BlendMatrixF.m[0][0] + Pos2.y * GSYS.DrawSetting.BlendMatrixF.m[1][0] + Pos2.z * GSYS.DrawSetting.BlendMatrixF.m[2][0] + GSYS.DrawSetting.BlendMatrixF.m[3][0]  ;
		TempVert[1].pos.y = Pos2.x * GSYS.DrawSetting.BlendMatrixF.m[0][1] + Pos2.y * GSYS.DrawSetting.BlendMatrixF.m[1][1] + Pos2.z * GSYS.DrawSetting.BlendMatrixF.m[2][1] + GSYS.DrawSetting.BlendMatrixF.m[3][1]  ;
		TempVert[1].pos.z = Pos2.x * GSYS.DrawSetting.BlendMatrixF.m[0][2] + Pos2.y * GSYS.DrawSetting.BlendMatrixF.m[1][2] + Pos2.z * GSYS.DrawSetting.BlendMatrixF.m[2][2] + GSYS.DrawSetting.BlendMatrixF.m[3][2]  ;
		TempVert[1].rhw   = Pos2.x * GSYS.DrawSetting.BlendMatrixF.m[0][3] + Pos2.y * GSYS.DrawSetting.BlendMatrixF.m[1][3] + Pos2.z * GSYS.DrawSetting.BlendMatrixF.m[2][3] + GSYS.DrawSetting.BlendMatrixF.m[3][3]  ;

		TempVert[0].rhw = 1.0F / TempVert[0].rhw ;
		TempVert[0].pos.x *= TempVert[0].rhw ;
		TempVert[0].pos.y *= TempVert[0].rhw ;
		TempVert[0].pos.z *= TempVert[0].rhw ;

		TempVert[1].rhw = 1.0F / TempVert[1].rhw ;
		TempVert[1].pos.x *= TempVert[1].rhw ;
		TempVert[1].pos.y *= TempVert[1].rhw ;
		TempVert[1].pos.z *= TempVert[1].rhw ;

		if( dright  > TempVert[ 0 ].pos.x ) dright  = TempVert[ 0 ].pos.x ;
		if( dright  > TempVert[ 1 ].pos.x ) dright  = TempVert[ 1 ].pos.x ;

		if( dleft   < TempVert[ 0 ].pos.x ) dleft   = TempVert[ 0 ].pos.x ;
		if( dleft   < TempVert[ 1 ].pos.x ) dleft   = TempVert[ 1 ].pos.x ;

		if( dbottom > TempVert[ 0 ].pos.y ) dbottom = TempVert[ 0 ].pos.y ;
		if( dbottom > TempVert[ 1 ].pos.y ) dbottom = TempVert[ 1 ].pos.y ;

		if( dtop    < TempVert[ 0 ].pos.y ) dtop    = TempVert[ 0 ].pos.y ;
		if( dtop    < TempVert[ 1 ].pos.y ) dtop    = TempVert[ 1 ].pos.y ;

		DrawArea->left   = _FTOL( dleft   ) - 1 ;
		DrawArea->right  = _FTOL( dright  ) + 1 ;
		DrawArea->top    = _FTOL( dtop    ) - 1 ;
		DrawArea->bottom = _FTOL( dbottom ) + 1 ;
	}

	// �I��
	return 0 ;
}



#define CIRCLE_VERTEX_NUM			(2000)

#define DX_3D_CIRCLE_THICKNESS												\
	if( x1 < DrawRect.left      ) x1 = DrawRect.left ;						\
	if( x2 > DrawRect.right - 1 ) x2 = DrawRect.right - 1 ;					\
																			\
	VertBuf[ VertNum ].pos.x  = ( float )x1 ;								\
	VertBuf[ VertNum ].pos.y  = y1f ;										\
	VertBuf[ VertNum ].color = Color ;										\
	VertBuf[ VertNum ].rhw   = 1.0f ;										\
	*((DWORD *)&VertBuf[ VertNum ].pos.z) = drawz ;							\
	VertNum ++ ;															\
																			\
	VertBuf[ VertNum ].pos.x =  ( float )( x2 + 1 ) ;						\
	VertBuf[ VertNum ].pos.y =  y1f ;										\
	VertBuf[ VertNum ].color = Color ;										\
	VertBuf[ VertNum ].rhw   = 1.0f ;										\
	*((DWORD *)&VertBuf[ VertNum ].pos.z) = drawz ;							\
	VertNum  ++ ;															\
																			\
	if( VertNum >= CIRCLE_VERTEX_NUM )										\
	{																		\
		Direct3DDevice9_DrawPrimitiveUP( D_D3DPT_LINELIST, ( UINT )( VertNum / 2 ), VertBuf, sizeof( VERTEX_NOTEX_2D ) ) ;\
		VertNum = 0 ;														\
	}																		

// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawCircle( �����w�肠�� )
extern	int		Graphics_D3D9_DrawCircle_Thickness( int x, int y, int r, unsigned int Color, int Thickness )
{
	VERTEX_NOTEX_2D *VertBuf ;
	DWORD VertNum ;
	short ( *CirclePos )[ 5 ] ;
	int Red, Green, Blue, Flag ;
	RECT DrawRect ;
	DWORD drawz ;
	int i ;
//	int j ;
	int x1, x2 ;
	float y1f ;

	// �������Q�ȉ��������牽�������I��
	if( Thickness < 2 )
	{
		return -1 ;
	}

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	if( GAPIWin.Direct3DDevice9Object == NULL ) return -1 ; 

	// �`��p�������̊m��
	if( Graphics_Other_AllocCommonBuffer( 0, sizeof( VERTEX_NOTEX_2D ) * CIRCLE_VERTEX_NUM ) < 0 )
	{
		return -1 ;
	}
	VertBuf = ( VERTEX_NOTEX_2D * )GSYS.Resource.CommonBuffer[ 0 ] ;

	// �`��̏���
	Graphics_D3D9_BeginScene() ;
	Flag = DX_D3D9_DRAWPREP_DIFFUSERGB ;
	DX_D3D9_DRAWPREP_NOTEX( Flag )

	// �y�o�b�t�@�ɏ������ނy�l���Z�b�g����
	drawz = *((DWORD *)&GSYS.DrawSetting.DrawZ);

	// �␳
	DrawRect = GSYS.DrawSetting.DrawArea ;

	NS_GetColor2( Color, &Red, &Green, &Blue ) ;
	SETUPCOLOR( Color )

	// FVF �̕ύX
//	Graphics_D3D9_DeviceState_SetFVF( VERTEXFVF_NOTEX_2D ) ;
	Graphics_Hardware_D3D9_SetFVF_UseTable( VERTEXFVF_DECL_NOTEX_2D ) ;

	// �~�̍��W��������o�b�t�@���擾
	if( Graphics_Other_AllocCommonBuffer( 1, sizeof( short ) * 5 * DrawRect.bottom ) < 0 )
	{
		return -1 ;
	}
	CirclePos = ( short (*)[ 5 ] )GSYS.Resource.CommonBuffer[ 1 ] ;
	_MEMSET( CirclePos, 0, sizeof( short ) * 5 * DrawRect.bottom ) ;

	// �~�̍��W���X�g�쐬
	Graphics_Draw_GetCircle_ThicknessDrawPosition( x, y, r, Thickness, CirclePos ) ;

	// �`�揈��
	VertNum = 0 ;
	for( i = DrawRect.top ; i < DrawRect.bottom ; i ++ )
	{
		if( CirclePos[ i ][ 4 ] == 0 )
		{
			continue ;
		}

		y1f = ( float )i ;

		if( ( CirclePos[ i ][ 4 ] & ( 4 | 8 ) ) == 0 || CirclePos[ i ][ 2 ] >= CirclePos[ i ][ 3 ] )
		{
			x1 = CirclePos[ i ][ 0 ] ;
			x2 = CirclePos[ i ][ 1 ] ;
			DX_3D_CIRCLE_THICKNESS
		}
		else
		{
			x1 = CirclePos[ i ][ 0 ] ;
			x2 = CirclePos[ i ][ 2 ] ;
			DX_3D_CIRCLE_THICKNESS

			x1 = CirclePos[ i ][ 3 ] ;
			x2 = CirclePos[ i ][ 1 ] ;
			DX_3D_CIRCLE_THICKNESS
		}
	}
	if( VertNum )
	{
		Direct3DDevice9_DrawPrimitiveUP( D_D3DPT_LINELIST, VertNum / 2, VertBuf, sizeof( VERTEX_NOTEX_2D ) ) ;
	}

	// �I��
	return 0 ;
}

// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawOval( �����w�肠�� )
extern	int		Graphics_D3D9_DrawOval_Thickness( int x, int y, int rx, int ry, unsigned int Color, int Thickness )
{
	VERTEX_NOTEX_2D *VertBuf ;
	int VertNum ;
	short ( *CirclePos )[ 5 ] ;
	int Red, Green, Blue, Flag ;
	RECT DrawRect ;
	DWORD drawz ;
	int i ;
//	int j ;
	int x1, x2 ;
	float y1f ;

	// �������Q�ȉ��������牽�������I��
	if( Thickness < 2 )
	{
		return -1 ;
	}

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	if( GAPIWin.Direct3DDevice9Object == NULL ) return -1 ; 

	// �`��p�������̊m��
	if( Graphics_Other_AllocCommonBuffer( 0, sizeof( VERTEX_NOTEX_2D ) * CIRCLE_VERTEX_NUM ) < 0 )
	{
		return -1 ;
	}
	VertBuf = ( VERTEX_NOTEX_2D * )GSYS.Resource.CommonBuffer[ 0 ] ;

	// �`��̏���
	Graphics_D3D9_BeginScene() ;
	Flag = DX_D3D9_DRAWPREP_DIFFUSERGB ;
	DX_D3D9_DRAWPREP_NOTEX( Flag )

	// �y�o�b�t�@�ɏ������ނy�l���Z�b�g����
	drawz = *((DWORD *)&GSYS.DrawSetting.DrawZ);

	// �␳
	DrawRect = GSYS.DrawSetting.DrawArea ;

	NS_GetColor2( Color, &Red, &Green, &Blue ) ;
	SETUPCOLOR( Color )

	// FVF �̕ύX
//	Graphics_D3D9_DeviceState_SetFVF( VERTEXFVF_NOTEX_2D ) ;
	Graphics_Hardware_D3D9_SetFVF_UseTable( VERTEXFVF_DECL_NOTEX_2D ) ;

	// �~�̍��W��������o�b�t�@���擾
	if( Graphics_Other_AllocCommonBuffer( 1, sizeof( short ) * 5 * DrawRect.bottom ) < 0 )
	{
		return -1 ;
	}
	CirclePos = ( short (*)[ 5 ] )GSYS.Resource.CommonBuffer[ 1 ] ;
	_MEMSET( CirclePos, 0, sizeof( short ) * 5 * DrawRect.bottom ) ;

	// �ȉ~�̍��W���X�g�쐬
	Graphics_Draw_GetOval_ThicknessDrawPosition( x, y, rx, ry, Thickness, CirclePos ) ;

	// �`�揈��
	VertNum = 0 ;
	for( i = DrawRect.top ; i < DrawRect.bottom ; i ++ )
	{
		if( CirclePos[ i ][ 4 ] == 0 )
		{
			continue ;
		}

		y1f = ( float )i ;

		if( ( CirclePos[ i ][ 4 ] & ( 4 | 8 ) ) == 0 || CirclePos[ i ][ 2 ] >= CirclePos[ i ][ 3 ] )
		{
			x1 = CirclePos[ i ][ 0 ] ;
			x2 = CirclePos[ i ][ 1 ] ;
			DX_3D_CIRCLE_THICKNESS
		}
		else
		{
			x1 = CirclePos[ i ][ 0 ] ;
			x2 = CirclePos[ i ][ 2 ] ;
			DX_3D_CIRCLE_THICKNESS

			x1 = CirclePos[ i ][ 3 ] ;
			x2 = CirclePos[ i ][ 1 ] ;
			DX_3D_CIRCLE_THICKNESS
		}
	}
	if( VertNum )
	{
		Direct3DDevice9_DrawPrimitiveUP( D_D3DPT_LINELIST, ( UINT )( VertNum / 2 ), VertBuf, sizeof( VERTEX_NOTEX_2D ) ) ;
	}

	// �I��
	return 0 ;
}




#define DX_3D_CIRCLE																						\
			if( !( ( x2 < DrawRect.left ) || ( x1 >= DrawRect.right  ) ||									\
			       ( y1 < DrawRect.top  ) || ( y1 >= DrawRect.bottom ) || LineDrawBuf[ y1 ] ) )				\
			{																								\
				if( x1 < DrawRect.left ) x1 = DrawRect.left ;												\
				if( x2 > DrawRect.right - 1 ) x2 = DrawRect.right - 1 ;										\
																											\
				VertBuf[ VertNum ].pos.x  = ( float )x1 + GD3D9.Device.Caps.DrawFillCircleLeftVertAddX ;	\
				VertBuf[ VertNum ].pos.y  = ( float )y1 ;													\
				VertBuf[ VertNum ].color = Color ;															\
				VertBuf[ VertNum ].rhw   = 1.0f ;															\
				*((DWORD *)&VertBuf[ VertNum ].pos.z) = drawz ;												\
				VertNum ++ ;																				\
				LineDrawBuf[y1] = TRUE ;																	\
																											\
				VertBuf[ VertNum ].pos.x =  ( float )( x2 + 1 ) + GD3D9.Device.Caps.DrawFillCircleRightVertAddX ;	\
				VertBuf[ VertNum ].pos.y =  ( float )y1 ;													\
				VertBuf[ VertNum ].color = Color ;															\
				VertBuf[ VertNum ].rhw   = 1.0f ;															\
				*((DWORD *)&VertBuf[ VertNum ].pos.z) = drawz ;												\
				VertNum  ++ ;																				\
																											\
				if( VertNum >= CIRCLE_VERTEX_NUM )															\
				{																							\
					Direct3DDevice9_DrawPrimitiveUP( D_D3DPT_LINELIST, ( UINT )( VertNum / 2 ), VertBuf, sizeof( VERTEX_NOTEX_2D ) ) ;\
					VertNum = 0 ;																			\
				}																							\
			}

#define DX_3D_CIRCLE_PSET																\
			if( !( ( x1 < DrawRect.left ) || ( x1 >= DrawRect.right  ) ||				\
				   ( y1 < DrawRect.top  ) || ( y1 >= DrawRect.bottom ) ) )				\
			{																			\
				VertBuf[ VertNum ].pos.x  = ( float )x1 ;								\
				VertBuf[ VertNum ].pos.y  = ( float )y1 ;								\
				VertBuf[ VertNum ].color = Color ;										\
				VertBuf[ VertNum ].rhw   = 1.0f ;										\
				*((DWORD *)&VertBuf[ VertNum ].pos.z) = drawz ;							\
				VertNum ++ ;															\
																						\
				if( VertNum >= CIRCLE_VERTEX_NUM )										\
				{																		\
					Direct3DDevice9_DrawPrimitiveUP( D_D3DPT_POINTLIST, VertNum, VertBuf, sizeof( VERTEX_NOTEX_2D ) ) ;\
					VertNum = 0 ;														\
				}																		\
			}

// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawCircle
extern	int		Graphics_D3D9_DrawCircle( int x, int y, int r, unsigned int Color, int FillFlag )
{
	VERTEX_NOTEX_2D *VertBuf ;
	DWORD VertNum ;
	BYTE *LineDrawBuf ;
	int Red, Green, Blue, Flag ;
	RECT DrawRect ;
	DWORD drawz ;

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	if( GAPIWin.Direct3DDevice9Object == NULL ) return -1 ; 

	// �`��p�������̊m��
	if( Graphics_Other_AllocCommonBuffer( 0, sizeof( VERTEX_NOTEX_2D ) * CIRCLE_VERTEX_NUM ) < 0 )
	{
		return -1 ;
	}
	VertBuf = ( VERTEX_NOTEX_2D * )GSYS.Resource.CommonBuffer[ 0 ] ;

	// �`��̏���
	Graphics_D3D9_BeginScene() ;
	Flag = DX_D3D9_DRAWPREP_DIFFUSERGB ;
	DX_D3D9_DRAWPREP_NOTEX( Flag )

	// �y�o�b�t�@�ɏ������ނy�l���Z�b�g����
	drawz = *((DWORD *)&GSYS.DrawSetting.DrawZ);

	// �␳
	DrawRect = GSYS.DrawSetting.DrawArea ;

	NS_GetColor2( Color, &Red, &Green, &Blue ) ;
	SETUPCOLOR( Color )

	// FVF �̕ύX
//	Graphics_D3D9_DeviceState_SetFVF( VERTEXFVF_NOTEX_2D ) ;
	Graphics_Hardware_D3D9_SetFVF_UseTable( VERTEXFVF_DECL_NOTEX_2D ) ;

	// �`�揈��
	{
		int Dx, Dy, F/*, i*/, j ;
		int x1, x2, y1 ;

		// �����l�Z�b�g
		Dx = r ; Dy = 0 ; F = -2 * r + 3 ;
		VertNum = 0 ;

		j = 0 ;
		// �`��J�n
		if( FillFlag )
		{
			if( Graphics_Other_AllocCommonBuffer( 1, ( DWORD )DrawRect.bottom ) < 0 )
			{
				return DxLib_ErrorUTF16LE( "\xe1\x30\xe2\x30\xea\x30\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x20\x00\x69\x00\x6e\x00\x20\x00\x47\x00\x5f\x00\x44\x00\x33\x00\x44\x00\x5f\x00\x44\x00\x72\x00\x61\x00\x77\x00\x43\x00\x69\x00\x72\x00\x63\x00\x6c\x00\x65\x00\x00"/*@ L"�������̊m�ۂɎ��s���܂��� in G_D3D_DrawCircle" @*/ ) ;
			}
			LineDrawBuf = ( BYTE * )GSYS.Resource.CommonBuffer[ 1 ] ;
			_MEMSET( LineDrawBuf, 0, ( size_t )DrawRect.bottom ) ;

			// �ŏ��̃��C����`��
			{
				x2 = Dx + x ; x1 = -Dx + x ; y1 = Dy + y ;
				DX_3D_CIRCLE ;

				// ���W�f�[�^��i�߂�
				if( F >= 0 )
				{
					x2 = Dy + x ; x1 = -Dy + x ; y1 = Dx + y ;
					DX_3D_CIRCLE ;
					x2 = Dy + x ; x1 = -Dy + x ; y1 = -Dx + y;
					DX_3D_CIRCLE ;

					Dx -- ;
					F -= 4 * Dx ;
				}	

				Dy ++ ;
				F += 4 * Dy + 2 ;
			}

			while( Dx >= Dy )
			{
				x2 = Dx + x ; x1 = -Dx + x ; y1 = Dy + y ;
				DX_3D_CIRCLE ;
				x2 = Dx + x ; x1 = -Dx + x ; y1 = -Dy + y;
				DX_3D_CIRCLE ;

				// ���W�f�[�^��i�߂�
				if( F >= 0 )
				{
					x2 = Dy + x ; x1 = -Dy + x ; y1 = Dx + y ;
					DX_3D_CIRCLE ;
					x2 = Dy + x ; x1 = -Dy + x ; y1 = -Dx + y;
					DX_3D_CIRCLE ;

					Dx -- ;
					F -= 4 * Dx ;
				}

				Dy ++ ;
				F += 4 * Dy + 2 ;
			}
			if( VertNum )
			{
				Direct3DDevice9_DrawPrimitiveUP( D_D3DPT_LINELIST, ( UINT )( VertNum / 2 ), VertBuf, sizeof( VERTEX_NOTEX_2D ) ) ;
			}
		}
		else
		{
			// �ŏ��̓_��`��
			{
				x1 = -Dx + x ; y1 = Dy + y ;
				DX_3D_CIRCLE_PSET ;
				x1 = Dx + x ;  
				DX_3D_CIRCLE_PSET ;

				x1 = Dy + x ; y1 = Dx + y ;
				DX_3D_CIRCLE_PSET ;

				x1 = Dy + x ; y1 = -Dx + y;
				DX_3D_CIRCLE_PSET ;

				// ���W�f�[�^��i�߂�
				if( F >= 0 )
				{
					Dx -- ;
					F -= 4 * Dx ;
				}	

				Dy ++ ;
				F += 4 * Dy + 2 ;
			}

			while( Dx >= Dy )
			{
				x1 = -Dx + x ; y1 = Dy + y ;
				DX_3D_CIRCLE_PSET ;
				x1 = Dx + x ;  
				DX_3D_CIRCLE_PSET ;

				x1 = Dy + x ; y1 = Dx + y ;
				DX_3D_CIRCLE_PSET ;
				x1 = -Dy + x ; 
				DX_3D_CIRCLE_PSET ;
			
				x1 = Dx + x ; y1 = -Dy + y;
				DX_3D_CIRCLE_PSET ;
				x1 = -Dx + x ; 
				DX_3D_CIRCLE_PSET ;

				x1 = Dy + x ; y1 = -Dx + y;
				DX_3D_CIRCLE_PSET ;
				x1 = -Dy + x ; 
				DX_3D_CIRCLE_PSET ;

				// ���W�f�[�^��i�߂�
				if( F >= 0 )
				{
					Dx -- ;
					F -= 4 * Dx ;
				}

				Dy ++ ;
				F += 4 * Dy + 2 ;
			}
			if( VertNum )
			{
				Direct3DDevice9_DrawPrimitiveUP( D_D3DPT_POINTLIST, VertNum, VertBuf, sizeof( VERTEX_NOTEX_2D ) ) ;
			}
		}
	}

	// �I��
	return 0 ;
}

// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawOval
extern	int		Graphics_D3D9_DrawOval( int x, int y, int rx, int ry, unsigned int Color, int FillFlag )
{
	VERTEX_NOTEX_2D *VertBuf ;
	DWORD VertNum ;
	BYTE *LineDrawBuf ;
//	VERTEX_NOTEX_2D *VertData ;
//	int i, j, r2 ;
//	float xd, x2, y2 ;
//	float d, yd ;
	int Red, Green, Blue, Flag ;
	RECT DrawRect ;
	DWORD drawz ;

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	if( GAPIWin.Direct3DDevice9Object == NULL ) return -1 ; 
	if( !rx || !ry ) return -1 ;

	// �`��p�������̊m��
	if( Graphics_Other_AllocCommonBuffer( 0, sizeof( VERTEX_NOTEX_2D ) * CIRCLE_VERTEX_NUM ) < 0 )
	{
		return -1 ;
	}
	VertBuf = ( VERTEX_NOTEX_2D * )GSYS.Resource.CommonBuffer[ 0 ] ;

	// �`��̏���
	Graphics_D3D9_BeginScene() ;
	Graphics_D3D9_DrawSetting_SetTexture( NULL ) ;
	Flag = DX_D3D9_DRAWPREP_DIFFUSERGB ;
	DX_D3D9_DRAWPREP_NOTEX( Flag )

	// �␳
	if( rx < 0 ) rx *= -1 ;
	if( ry < 0 ) ry *= -1 ;
	DrawRect = GSYS.DrawSetting.DrawArea ;

	NS_GetColor2( Color, &Red, &Green, &Blue ) ;
	SETUPCOLOR( Color )

	// �y�o�b�t�@�ɏ������ނy�l���Z�b�g����
	drawz = *((DWORD *)&GSYS.DrawSetting.DrawZ);

	// FVF �̕ύX
//	Graphics_D3D9_DeviceState_SetFVF( VERTEXFVF_NOTEX_2D ) ;
	Graphics_Hardware_D3D9_SetFVF_UseTable( VERTEXFVF_DECL_NOTEX_2D ) ;

	VertNum = 0 ;

	// �`�揈��
	{
		int Dx, Dy, F, H, j/*, i */ ;
		int x1, x2, y1, Df ;
		int yc, xc, rc, r ;

		j = 0 ;
		// �`��J�n
		if( FillFlag )
		{
			if( Graphics_Other_AllocCommonBuffer( 1, ( DWORD )DrawRect.bottom ) < 0 )
			{
				return DxLib_ErrorUTF16LE( "\xe1\x30\xe2\x30\xea\x30\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x20\x00\x69\x00\x6e\x00\x20\x00\x47\x00\x5f\x00\x44\x00\x33\x00\x44\x00\x5f\x00\x44\x00\x72\x00\x61\x00\x77\x00\x43\x00\x69\x00\x72\x00\x63\x00\x6c\x00\x65\x00\x00"/*@ L"�������̊m�ۂɎ��s���܂��� in G_D3D_DrawCircle" @*/ ) ;
			}
			LineDrawBuf = ( BYTE * )GSYS.Resource.CommonBuffer[ 1 ] ;
			_MEMSET( LineDrawBuf, 0, ( size_t )DrawRect.bottom ) ;

			if( rx >= ry )
			{
				// �����l���Z�b�g
				rc = ( rx * rx ) / ry ;

				xc = 0 ; yc = 0 ;
				r = rx ;
				Df = 1 ;
		
				Dx = r ;
				Dy = 0;
				F = -2 * r + 1 + 2 * 1;
				H = -4 * r + 2 + 1;

				// �ŏ��̐���`��
				{
					x1 = -Dx + x ; x2 = Dx + x ; y1 = Dy + y ;
					DX_3D_CIRCLE ;

					Df = 0 ;

					// ���W�f�[�^��i�߂�
					if( F < 0 )
					{
						yc += ry ;
						if( yc >= rc )
						{
							Dy ++ ;
							yc -= rc ;
							Df = 1 ;
						}
						F += 4 * Dy + 2 ;
						H += 4 * Dy;
					}
					else
					{
						Df = 1 ;

						if( H>=0 )
						{
							Dx--;
							F -= 4 * Dx;
							H -= 4 * Dx - 2 ;
						}
						else
						{
							Dx--;	
							yc += ry ;
							if( yc >= rc )
							{
								Dy ++ ;
								yc -= rc ;
								Df = 1 ;
							}
							F += 4 * Dy - 4 * Dx + 2 ;
							H += 4 * Dy - 4 * Dx + 2 ;
						}
					}
				}

				while( Dx >= 0 )
				{
					if( Df )
					{
						x1 = -Dx + x ; x2 = Dx + x ; y1 = Dy + y ;
						DX_3D_CIRCLE ;

						y1 = -Dy + y ;
						DX_3D_CIRCLE ;
					}

					Df = 0 ;

					// ���W�f�[�^��i�߂�
					if( F < 0 )
					{
						yc += ry ;
						if( yc >= rc )
						{
							Dy ++ ;
							yc -= rc ;
							Df = 1 ;
						}
						F += 4 * Dy + 2 ;
						H += 4 * Dy;
					}
					else
					{
						if( H>=0 )
						{
							Dx--;
							F -= 4 * Dx;
							H -= 4 * Dx - 2 ;
						}
						else
						{
							Dx--;	
							yc += ry ;
							if( yc >= rc )
							{
								Dy ++ ;
								yc -= rc ;
								Df = 1 ;
							}
							F += 4 * Dy - 4 * Dx + 2 ;
							H += 4 * Dy - 4 * Dx + 2 ;
						}
					}
				}

				// �Ō�̐���`��
				x1 = -Dx + x ; x2 = Dx + x ; y1 = -Dy + y;
				DX_3D_CIRCLE ;
			}
			else
			{
				// �����l���Z�b�g
				rc = ( ry * ry ) / rx ;

				xc = 0 ; yc = 0 ;
				r = rx ;
				Df = 2 ;
		
				Dx = r ;
				Dy = 0;
				F = -2 * r + 1 + 2 * 1;
				H = -4 * r + 2 + 1;

				// �ŏ��̐���`��
				{
					x1 = -Dx + x ; x2 = Dx + x ; y1 = Dy + y ;
					DX_3D_CIRCLE ;

					Df = 0 ;

					// ���W�f�[�^��i�߂�
					if( F < 0 )
					{
						Df = 1 ;

						Dy ++ ;
						F += 4 * Dy + 2 ;
						H += 4 * Dy;
					}
					else
					{
						if( H>=0 )
						{
							xc += rx ;
							if( xc >= rc )
							{
								Dx--;
								xc -= rc ;
							}
							F -= 4 * Dx;
							H -= 4 * Dx - 2 ;
						}
						else
						{
							Df = 1 ;

							xc += rx ;
							if( xc >= rc )
							{
								Dx--;
								xc -= rc ;
							}

							Dy ++ ;
							F += 4 * Dy - 4 * Dx + 2 ;
							H += 4 * Dy - 4 * Dx + 2 ;
						}
					}
				}

				while( Dx > 0 )
				{
					if( Df )
					{
						x1 = -Dx + x ; x2 = Dx + x ; y1 = Dy + y ;
						DX_3D_CIRCLE ;

						y1 = -Dy + y ;
						DX_3D_CIRCLE ;
					}

					Df = 0 ;

					// ���W�f�[�^��i�߂�
					if( F < 0 )
					{
						Df = 1 ;

						Dy ++ ;
						F += 4 * Dy + 2 ;
						H += 4 * Dy;
					}
					else
					{
						if( H>=0 )
						{
							xc += rx ;
							if( xc >= rc )
							{
								Dx--;
								xc -= rc ;
							}
							F -= 4 * Dx;
							H -= 4 * Dx - 2 ;
						}
						else
						{
							Df = 1 ;

							xc += rx ;
							if( xc >= rc )
							{
								Dx--;
								xc -= rc ;
							}

							Dy ++ ;
							F += 4 * Dy - 4 * Dx + 2 ;
							H += 4 * Dy - 4 * Dx + 2 ;
						}
					}
				}

				// �Ō�̐���`��
				x1 = -Dx + x ; x2 = Dx + x ; y1 = -Dy + y;
				DX_3D_CIRCLE ;
			}
			if( VertNum )
			{
				Direct3DDevice9_DrawPrimitiveUP( D_D3DPT_LINELIST, VertNum / 2, VertBuf, sizeof( VERTEX_NOTEX_2D ) ) ;
			}
		}
		else
		{
			if( rx >= ry )
			{
				// �����l���Z�b�g
				rc = ( rx * rx ) / ry ;

				xc = 0 ; yc = 0 ;
				r = rx ;
				Df = 2 ;
		
				Dx = r ;
				Dy = 0;
				F = -2 * r + 1 + 2 * 1;
				H = -4 * r + 2 + 1;

				// �ŏ��̓_��`��
				{
					x1 = -Dx + x ; y1 = Dy + y ;
					DX_3D_CIRCLE_PSET ;
					x1 = Dx + x ;  
					DX_3D_CIRCLE_PSET ;

					Df = 0 ;

					// ���W�f�[�^��i�߂�
					if( F < 0 )
					{
						yc += ry ;
						if( yc >= rc )
						{
							Dy ++ ;
							yc -= rc ;
							Df = 1 ;
						}
						F += 4 * Dy + 2 ;
						H += 4 * Dy;
					}
					else
					{
						Df = 1 ;

						if( H>=0 )
						{
							Dx--;
							F -= 4 * Dx;
							H -= 4 * Dx - 2 ;
						}
						else
						{
							Dx--;	
							yc += ry ;
							if( yc >= rc )
							{
								Dy ++ ;
								yc -= rc ;
								Df = 1 ;
							}
							F += 4 * Dy - 4 * Dx + 2 ;
							H += 4 * Dy - 4 * Dx + 2 ;
						}
					}
				}

				while( Dx > 0 )
				{
					if( Df )
					{
						x1 = -Dx + x ; y1 = Dy + y ;
						DX_3D_CIRCLE_PSET ;
						x1 = Dx + x ;  
						DX_3D_CIRCLE_PSET ;

						x1 = -Dx + x ; y1 = -Dy + y ;
						DX_3D_CIRCLE_PSET ;
						x1 = Dx + x ;  
						DX_3D_CIRCLE_PSET ;
					}

					Df = 0 ;

					// ���W�f�[�^��i�߂�
					if( F < 0 )
					{
						yc += ry ;
						if( yc >= rc )
						{
							Dy ++ ;
							yc -= rc ;
							Df = 1 ;
						}
						F += 4 * Dy + 2 ;
						H += 4 * Dy;
					}
					else
					{
						Df = 1 ;

						if( H>=0 )
						{
							Dx--;
							F -= 4 * Dx;
							H -= 4 * Dx - 2 ;
						}
						else
						{
							Dx--;	
							yc += ry ;
							if( yc >= rc )
							{
								Dy ++ ;
								yc -= rc ;
								Df = 1 ;
							}
							F += 4 * Dy - 4 * Dx + 2 ;
							H += 4 * Dy - 4 * Dx + 2 ;
						}
					}
				}

				// �Ō�̓_��`��
				x1 = Dx + x ; y1 = -Dy + y;
				DX_3D_CIRCLE_PSET ;

				x1 = -Dx + x ; y1 = Dy + y ;
				DX_3D_CIRCLE_PSET ;
			}
			else
			{
				// �����l���Z�b�g
				rc = ( ry * ry ) / rx ;

				xc = 0 ; yc = 0 ;
				r = rx ;
				Df = 2 ;
		
				Dx = r ;
				Dy = 0;
				F = -2 * r + 1 + 2 * 1;
				H = -4 * r + 2 + 1;

				// �ŏ��̓_��`��
				{
					x1 = -Dx + x ; y1 = Dy + y ;
					DX_3D_CIRCLE_PSET ;
					x1 = Dx + x ;  
					DX_3D_CIRCLE_PSET ;

					Df = 0 ;

					// ���W�f�[�^��i�߂�
					if( F < 0 )
					{
						Df = 1 ;

						Dy ++ ;
						F += 4 * Dy + 2 ;
						H += 4 * Dy;
					}
					else
					{
						if( H>=0 )
						{
							xc += rx ;
							if( xc >= rc )
							{
								Dx--;
								xc -= rc ;
								Df = 1 ;
							}
							F -= 4 * Dx;
							H -= 4 * Dx - 2 ;
						}
						else
						{
							Df = 1 ;

							xc += rx ;
							if( xc >= rc )
							{
								Dx--;
								xc -= rc ;
							}

							Dy ++ ;
							F += 4 * Dy - 4 * Dx + 2 ;
							H += 4 * Dy - 4 * Dx + 2 ;
						}
					}
				}

				while( Dx > 0 )
				{
					if( Df )
					{
						x1 = -Dx + x ; y1 = Dy + y ;
						DX_3D_CIRCLE_PSET ;
						x1 = Dx + x ;  
						DX_3D_CIRCLE_PSET ;

						x1 = -Dx + x ; y1 = -Dy + y ;
						DX_3D_CIRCLE_PSET ;
						x1 = Dx + x ;  
						DX_3D_CIRCLE_PSET ;
					}

					Df = 0 ;

					// ���W�f�[�^��i�߂�
					if( F < 0 )
					{
						Df = 1 ;

						Dy ++ ;
						F += 4 * Dy + 2 ;
						H += 4 * Dy;
					}
					else
					{
						if( H>=0 )
						{
							xc += rx ;
							if( xc >= rc )
							{
								Dx--;
								xc -= rc ;
								Df = 1 ;
							}
							F -= 4 * Dx;
							H -= 4 * Dx - 2 ;
						}
						else
						{
							Df = 1 ;

							xc += rx ;
							if( xc >= rc )
							{
								Dx--;
								xc -= rc ;
							}

							Dy ++ ;
							F += 4 * Dy - 4 * Dx + 2 ;
							H += 4 * Dy - 4 * Dx + 2 ;
						}
					}
				}

				// �Ō�̓_��`��
				x1 = Dx + x ; y1 = -Dy + y;
				DX_3D_CIRCLE_PSET ;

				x1 = -Dx + x ; y1 = Dy + y ;
				DX_3D_CIRCLE_PSET ;
			}
			if( VertNum )
			{
				Direct3DDevice9_DrawPrimitiveUP( D_D3DPT_POINTLIST, VertNum, VertBuf, sizeof( VERTEX_NOTEX_2D ) ) ;
			}
		}
	}

	// �I��
	return 0 ;
}

// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawTriangle
extern	int		Graphics_D3D9_DrawTriangle( int x1, int y1, int x2, int y2, int x3, int y3, unsigned int Color, int FillFlag )
{
	VERTEX_NOTEX_2D *vec ; 
	int Red, Green, Blue ;
	int Flag ;
	int sx1, sx2, sy1, sy2 ;
	DWORD drawz ;

	if( GAPIWin.Direct3DDevice9Object == NULL ) return -1 ;

	// �`��̏���
	Graphics_D3D9_BeginScene() ;
	Graphics_D3D9_DrawSetting_SetTexture( NULL ) ;
	Flag = DX_D3D9_DRAWPREP_DIFFUSERGB ;
	DX_D3D9_DRAWPREP_NOTEX( Flag )

	// �y�o�b�t�@�ɏ������ނy�l���Z�b�g����
	drawz = *((DWORD *)&GSYS.DrawSetting.DrawZ);

	// �F���̑��X�e�[�^�X�̃Z�b�g
	NS_GetColor2( Color, &Red, &Green, &Blue ) ;
	SETUPCOLOR( Color )

	// ���C�����ǂ����ŏ����𕪊�
	if( FillFlag )
	{
		GETVERTEX_TRIANGLE( vec ) ;

		vec[ 0 ].color = Color ;

		vec[ 0 ].pos.x = ( float )x1 ; vec[ 0 ].pos.y = ( float )y1 ;

		// �J�����O���L���ɂȂ��Ă���ꍇ�̓J�����O����Ȃ��悤�ɂ���
		switch( GSYS.DrawSetting.CullMode )
		{
		case 0 :	// �J�����O�Ȃ�
			vec[ 1 ].pos.x = ( float )x2 ; vec[ 1 ].pos.y = ( float )y2 ;
			vec[ 2 ].pos.x = ( float )x3 ; vec[ 2 ].pos.y = ( float )y3 ;
			break ;

		case 1 :	// �����J�����O
			sx1 = x2 - x1 ;		sy1 = y2 - y1 ;
			sx2 = x3 - x1 ;		sy2 = y3 - y1 ;
			if( sx1 * sy2 - sy1 * sx2 > 0 )
			{
				vec[ 1 ].pos.x = ( float )x2 ; vec[ 1 ].pos.y = ( float )y2 ;
				vec[ 2 ].pos.x = ( float )x3 ; vec[ 2 ].pos.y = ( float )y3 ;
			}
			else
			{
				vec[ 1 ].pos.x = ( float )x3 ; vec[ 1 ].pos.y = ( float )y3 ;
				vec[ 2 ].pos.x = ( float )x2 ; vec[ 2 ].pos.y = ( float )y2 ;
			}
			break ;

		case 2 :	// �E���J�����O
			sx1 = x2 - x1 ;		sy1 = y2 - y1 ;
			sx2 = x3 - x1 ;		sy2 = y3 - y1 ;
			if( sx1 * sy2 - sy1 * sx2 < 0 )
			{
				vec[ 1 ].pos.x = ( float )x2 ; vec[ 1 ].pos.y = ( float )y2 ;
				vec[ 2 ].pos.x = ( float )x3 ; vec[ 2 ].pos.y = ( float )y3 ;
			}
			else
			{
				vec[ 1 ].pos.x = ( float )x3 ; vec[ 1 ].pos.y = ( float )y3 ;
				vec[ 2 ].pos.x = ( float )x2 ; vec[ 2 ].pos.y = ( float )y2 ;
			}
			break ;
		}

		*( ( DWORD * )&vec[ 0 ].pos.z ) = drawz ;
		*( ( DWORD * )&vec[ 1 ].pos.z ) = drawz ;
		*( ( DWORD * )&vec[ 2 ].pos.z ) = drawz ;

		ADD3VERTEX_NOTEX
	}
	else
	{
		GETVERTEX_LINETRIANGLE( vec )

		vec[ 0 ].color =
		vec[ 1 ].color =
		vec[ 2 ].color =
		vec[ 3 ].color =
		vec[ 4 ].color =
		vec[ 5 ].color = Color ;

		vec[ 0 ].pos.x = ( float )x1 ; vec[ 0 ].pos.y = ( float )y1 ;
		vec[ 1 ].pos.x = ( float )x2 ; vec[ 1 ].pos.y = ( float )y2 ;
		vec[ 2 ].pos.x = ( float )x2 ; vec[ 2 ].pos.y = ( float )y2 ;
		vec[ 3 ].pos.x = ( float )x3 ; vec[ 3 ].pos.y = ( float )y3 ;
		vec[ 4 ].pos.x = ( float )x3 ; vec[ 4 ].pos.y = ( float )y3 ;
		vec[ 5 ].pos.x = ( float )x1 ; vec[ 5 ].pos.y = ( float )y1 ;
		*( ( DWORD * )&vec[ 0 ].pos.z ) = drawz ;
		*( ( DWORD * )&vec[ 1 ].pos.z ) = drawz ;
		*( ( DWORD * )&vec[ 2 ].pos.z ) = drawz ;
		*( ( DWORD * )&vec[ 3 ].pos.z ) = drawz ;
		*( ( DWORD * )&vec[ 4 ].pos.z ) = drawz ;
		*( ( DWORD * )&vec[ 5 ].pos.z ) = drawz ;

		ADD4VERTEX_LINETRIANGLE
	}

	// �I��
	return 0 ;
}

// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawTriangle3D
extern	int		Graphics_D3D9_DrawTriangle3D( VECTOR Pos1, VECTOR Pos2, VECTOR Pos3, unsigned int Color, int FillFlag, int DrawFlag, RECT *DrawArea )
{
	VERTEX_NOTEX_3D *vec ; 
	int Red, Green, Blue ;
	int Flag ;

	if( DrawFlag )
	{
		if( GAPIWin.Direct3DDevice9Object == NULL ) return -1 ;

		// �`��̏���
		if( GD3D9.Device.DrawInfo.BeginSceneFlag == FALSE ) Graphics_D3D9_BeginScene() ;
		if( GD3D9.Device.DrawSetting.RenderTexture != NULL ) Graphics_D3D9_DrawSetting_SetTexture( NULL ) ;

		Flag = DX_D3D9_DRAWPREP_DIFFUSERGB | DX_D3D9_DRAWPREP_3D | DX_D3D9_DRAWPREP_FOG | DX_D3D9_DRAWPREP_CULLING ;
		DX_D3D9_DRAWPREP_NOTEX( Flag )

		// �F���̑��X�e�[�^�X�̃Z�b�g
		NS_GetColor2( Color, &Red, &Green, &Blue ) ;
		SETUPCOLOR( Color )

		// ���C�����ǂ����ŏ����𕪊�
		if( FillFlag )
		{
			GETVERTEX_TRIANGLE3D( vec ) ;

			*( ( DWORD * )&vec[ 0 ].b ) =
			*( ( DWORD * )&vec[ 1 ].b ) =
			*( ( DWORD * )&vec[ 2 ].b ) = ( DWORD )Color ;
			vec[ 0 ].pos = Pos1 ;
			vec[ 1 ].pos = Pos2 ;
			vec[ 2 ].pos = Pos3 ;

			ADD3VERTEX_NOTEX3D
		}
		else
		{
			GETVERTEX_LINETRIANGLE3D( vec )

			*( ( DWORD * )&vec[ 0 ].b ) =
			*( ( DWORD * )&vec[ 1 ].b ) =
			*( ( DWORD * )&vec[ 2 ].b ) =
			*( ( DWORD * )&vec[ 3 ].b ) =
			*( ( DWORD * )&vec[ 4 ].b ) =
			*( ( DWORD * )&vec[ 5 ].b ) = ( DWORD )Color ;

			vec[ 0 ].pos = Pos1 ;
			vec[ 1 ].pos = Pos2 ;
			vec[ 2 ].pos = Pos2 ;
			vec[ 3 ].pos = Pos3 ;
			vec[ 4 ].pos = Pos3 ;
			vec[ 5 ].pos = Pos1 ;

			ADD4VERTEX_LINETRIANGLE3D
		}
	}
	else
	{
		VERTEX_2D TempVert[ 3 ] ;
		float dleft, dright, dtop, dbottom ;

		dleft   =  100000000.0f ;
		dright  = -100000000.0f ;
		dtop    =  100000000.0f ;
		dbottom = -100000000.0f ;

		// �X�N���[�����W�ɕϊ�
		TempVert[0].pos.x = Pos1.x * GSYS.DrawSetting.BlendMatrixF.m[0][0] + Pos1.y * GSYS.DrawSetting.BlendMatrixF.m[1][0] + Pos1.z * GSYS.DrawSetting.BlendMatrixF.m[2][0] + GSYS.DrawSetting.BlendMatrixF.m[3][0]  ;
		TempVert[0].pos.y = Pos1.x * GSYS.DrawSetting.BlendMatrixF.m[0][1] + Pos1.y * GSYS.DrawSetting.BlendMatrixF.m[1][1] + Pos1.z * GSYS.DrawSetting.BlendMatrixF.m[2][1] + GSYS.DrawSetting.BlendMatrixF.m[3][1]  ;
		TempVert[0].pos.z = Pos1.x * GSYS.DrawSetting.BlendMatrixF.m[0][2] + Pos1.y * GSYS.DrawSetting.BlendMatrixF.m[1][2] + Pos1.z * GSYS.DrawSetting.BlendMatrixF.m[2][2] + GSYS.DrawSetting.BlendMatrixF.m[3][2]  ;
		TempVert[0].rhw   = Pos1.x * GSYS.DrawSetting.BlendMatrixF.m[0][3] + Pos1.y * GSYS.DrawSetting.BlendMatrixF.m[1][3] + Pos1.z * GSYS.DrawSetting.BlendMatrixF.m[2][3] + GSYS.DrawSetting.BlendMatrixF.m[3][3]  ;

		TempVert[1].pos.x = Pos2.x * GSYS.DrawSetting.BlendMatrixF.m[0][0] + Pos2.y * GSYS.DrawSetting.BlendMatrixF.m[1][0] + Pos2.z * GSYS.DrawSetting.BlendMatrixF.m[2][0] + GSYS.DrawSetting.BlendMatrixF.m[3][0]  ;
		TempVert[1].pos.y = Pos2.x * GSYS.DrawSetting.BlendMatrixF.m[0][1] + Pos2.y * GSYS.DrawSetting.BlendMatrixF.m[1][1] + Pos2.z * GSYS.DrawSetting.BlendMatrixF.m[2][1] + GSYS.DrawSetting.BlendMatrixF.m[3][1]  ;
		TempVert[1].pos.z = Pos2.x * GSYS.DrawSetting.BlendMatrixF.m[0][2] + Pos2.y * GSYS.DrawSetting.BlendMatrixF.m[1][2] + Pos2.z * GSYS.DrawSetting.BlendMatrixF.m[2][2] + GSYS.DrawSetting.BlendMatrixF.m[3][2]  ;
		TempVert[1].rhw   = Pos2.x * GSYS.DrawSetting.BlendMatrixF.m[0][3] + Pos2.y * GSYS.DrawSetting.BlendMatrixF.m[1][3] + Pos2.z * GSYS.DrawSetting.BlendMatrixF.m[2][3] + GSYS.DrawSetting.BlendMatrixF.m[3][3]  ;

		TempVert[2].pos.x = Pos3.x * GSYS.DrawSetting.BlendMatrixF.m[0][0] + Pos3.y * GSYS.DrawSetting.BlendMatrixF.m[1][0] + Pos3.z * GSYS.DrawSetting.BlendMatrixF.m[2][0] + GSYS.DrawSetting.BlendMatrixF.m[3][0]  ;
		TempVert[2].pos.y = Pos3.x * GSYS.DrawSetting.BlendMatrixF.m[0][1] + Pos3.y * GSYS.DrawSetting.BlendMatrixF.m[1][1] + Pos3.z * GSYS.DrawSetting.BlendMatrixF.m[2][1] + GSYS.DrawSetting.BlendMatrixF.m[3][1]  ;
		TempVert[2].pos.z = Pos3.x * GSYS.DrawSetting.BlendMatrixF.m[0][2] + Pos3.y * GSYS.DrawSetting.BlendMatrixF.m[1][2] + Pos3.z * GSYS.DrawSetting.BlendMatrixF.m[2][2] + GSYS.DrawSetting.BlendMatrixF.m[3][2]  ;
		TempVert[2].rhw   = Pos3.x * GSYS.DrawSetting.BlendMatrixF.m[0][3] + Pos3.y * GSYS.DrawSetting.BlendMatrixF.m[1][3] + Pos3.z * GSYS.DrawSetting.BlendMatrixF.m[2][3] + GSYS.DrawSetting.BlendMatrixF.m[3][3]  ;

		TempVert[0].rhw = 1.0F / TempVert[0].rhw ;
		TempVert[0].pos.x *= TempVert[0].rhw ;
		TempVert[0].pos.y *= TempVert[0].rhw ;
		TempVert[0].pos.z *= TempVert[0].rhw ;

		TempVert[1].rhw = 1.0F / TempVert[1].rhw ;
		TempVert[1].pos.x *= TempVert[1].rhw ;
		TempVert[1].pos.y *= TempVert[1].rhw ;
		TempVert[1].pos.z *= TempVert[1].rhw ;

		TempVert[2].rhw = 1.0F / TempVert[2].rhw ;
		TempVert[2].pos.x *= TempVert[2].rhw ;
		TempVert[2].pos.y *= TempVert[2].rhw ;
		TempVert[2].pos.z *= TempVert[2].rhw ;

		if( dright  < TempVert[ 0 ].pos.x ) dright  = TempVert[ 0 ].pos.x ;
		if( dright  < TempVert[ 1 ].pos.x ) dright  = TempVert[ 1 ].pos.x ;
		if( dright  < TempVert[ 2 ].pos.x ) dright  = TempVert[ 2 ].pos.x ;

		if( dleft   > TempVert[ 0 ].pos.x ) dleft   = TempVert[ 0 ].pos.x ;
		if( dleft   > TempVert[ 1 ].pos.x ) dleft   = TempVert[ 1 ].pos.x ;
		if( dleft   > TempVert[ 2 ].pos.x ) dleft   = TempVert[ 2 ].pos.x ;

		if( dbottom < TempVert[ 0 ].pos.y ) dbottom = TempVert[ 0 ].pos.y ;
		if( dbottom < TempVert[ 1 ].pos.y ) dbottom = TempVert[ 1 ].pos.y ;
		if( dbottom < TempVert[ 2 ].pos.y ) dbottom = TempVert[ 2 ].pos.y ;

		if( dtop    > TempVert[ 0 ].pos.y ) dtop    = TempVert[ 0 ].pos.y ;
		if( dtop    > TempVert[ 1 ].pos.y ) dtop    = TempVert[ 1 ].pos.y ;
		if( dtop    > TempVert[ 2 ].pos.y ) dtop    = TempVert[ 2 ].pos.y ;

		DrawArea->left   = _FTOL( dleft   ) - 1 ;
		DrawArea->right  = _FTOL( dright  ) + 1 ;
		DrawArea->top    = _FTOL( dtop    ) - 1 ;
		DrawArea->bottom = _FTOL( dbottom ) + 1 ;
	}

	// �I��
	return 0 ;
}

// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawQuadrangle
extern	int		Graphics_D3D9_DrawQuadrangle( int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, unsigned int Color, int FillFlag )
{
	VERTEX_NOTEX_2D VertData[ 5 ], *Vert ; 
	int Red, Green, Blue ;
	int i, Flag ;
	DWORD drawz ;

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	if( GAPIWin.Direct3DDevice9Object == NULL ) return -1 ;

	// �`��̏���
	Graphics_D3D9_BeginScene() ;
	Flag = DX_D3D9_DRAWPREP_DIFFUSERGB ;
	DX_D3D9_DRAWPREP_NOTEX( Flag )

	// �y�o�b�t�@�ɏ������ނy�l���Z�b�g����
	drawz = *((DWORD *)&GSYS.DrawSetting.DrawZ);

	// �F���̑��X�e�[�^�X�̃Z�b�g
	NS_GetColor2( Color, &Red, &Green, &Blue ) ;
	SETUPCOLOR( Color )
	Vert = VertData ;
	for( i = 0 ; i < 5 ; i ++, Vert ++ )
	{
		Vert->color = Color ;
		Vert->rhw   = 1.0f ;
		*((DWORD *)&Vert->pos.z) = drawz ;
	}

	// FVF �̃Z�b�g
//	Graphics_D3D9_DeviceState_SetFVF( VERTEXFVF_NOTEX_2D ) ;
	Graphics_Hardware_D3D9_SetFVF_UseTable( VERTEXFVF_DECL_NOTEX_2D ) ;

	// �h��Ԃ����ǂ����ŏ����𕪊�
	if( FillFlag )
	{
		// �h��Ԃ��̏ꍇ
		VertData[ 0 ].pos.x = ( float )x1 ; VertData[ 0 ].pos.y = ( float )y1 ;
		VertData[ 1 ].pos.x = ( float )x2 ; VertData[ 1 ].pos.y = ( float )y2 ;
		VertData[ 2 ].pos.x = ( float )x4 ; VertData[ 2 ].pos.y = ( float )y4 ;
		VertData[ 3 ].pos.x = ( float )x3 ; VertData[ 3 ].pos.y = ( float )y3 ;

		// ���]��������
		{
			double Reverse0, Reverse1 ;
			VERTEX_NOTEX_2D VertTemp ;

			Reverse0 =  ( VertData[ 1 ].pos.x - VertData[ 0 ].pos.x ) * ( VertData[ 2 ].pos.y - VertData[ 0 ].pos.y ) -
					    ( VertData[ 2 ].pos.x - VertData[ 0 ].pos.x ) * ( VertData[ 1 ].pos.y - VertData[ 0 ].pos.y ) ;
			Reverse1 = -( VertData[ 1 ].pos.x - VertData[ 3 ].pos.x ) * ( VertData[ 2 ].pos.y - VertData[ 3 ].pos.y ) +
						( VertData[ 1 ].pos.y - VertData[ 3 ].pos.y ) * ( VertData[ 2 ].pos.x - VertData[ 3 ].pos.x ) ;
			if( Reverse0 <= 0 && Reverse1 <= 0 )
			{
				VertTemp = VertData[ 1 ] ; VertData[ 1 ] = VertData[ 2 ] ; VertData[ 2 ] = VertTemp ;
			}

			if( Reverse0 * Reverse1 >= 0 )
			{
				// �v���~�e�B�u�̕`��
				Direct3DDevice9_DrawPrimitiveUP( D_D3DPT_TRIANGLESTRIP, 2, VertData, sizeof( VERTEX_NOTEX_2D ) ) ;
			}
			else
			{
				// �w�������]���̕␳
				if( Reverse0 < 0 ) 
				{
					VertTemp = VertData[ 1 ] ; VertData[ 1 ] = VertData[ 2 ] ; VertData[ 2 ] = VertTemp ;
				}

				// �v���~�e�B�u�̕`��
				Direct3DDevice9_DrawPrimitiveUP( D_D3DPT_TRIANGLESTRIP, 2, VertData, sizeof( VERTEX_NOTEX_2D ) ) ;
			}
		}
	}
	else
	{
		// �h��Ԃ��ł͖����ꍇ
		VertData[ 0 ].pos.x = ( float )x1 ; VertData[ 0 ].pos.y = ( float )y1 ;
		VertData[ 1 ].pos.x = ( float )x2 ; VertData[ 1 ].pos.y = ( float )y2 ;
		VertData[ 2 ].pos.x = ( float )x3 ; VertData[ 2 ].pos.y = ( float )y3 ;
		VertData[ 3 ].pos.x = ( float )x4 ; VertData[ 3 ].pos.y = ( float )y4 ;
		VertData[ 4 ].pos.x = ( float )x1 ; VertData[ 4 ].pos.y = ( float )y1 ;

		// �v���~�e�B�u�̕`��
		Direct3DDevice9_DrawPrimitiveUP( D_D3DPT_LINESTRIP, 4, VertData, sizeof( VERTEX_NOTEX_2D ) ) ;
	}

	// �I��
	return 0 ;
}

// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawPixel
extern	int		Graphics_D3D9_DrawPixel( int x, int y, unsigned int Color )
{
	VERTEX_NOTEX_2D *VertData ;
	int Red, Green, Blue, Flag ;

	if( GAPIWin.Direct3DDevice9Object == NULL ) return -1 ; 

	// �`��̏���
	if( GD3D9.Device.DrawInfo.BeginSceneFlag == FALSE ) Graphics_D3D9_BeginScene() ;
	Flag = DX_D3D9_DRAWPREP_DIFFUSERGB ;
	DX_D3D9_DRAWPREP_NOTEX( Flag )

	// �F���̑��X�e�[�^�X�̃Z�b�g
	GETVERTEX_POINT( VertData ) ;

	if( GD3D9.Device.DrawInfo.DiffuseColor == 0xffffffff )
	{
		NS_GetColor2( Color, &Red, &Green, &Blue ) ;
		VertData->color = 0xff000000 | ( Red << 16 ) | ( Green << 8 ) | Blue ;
	}
	else
	{
		NS_GetColor2( Color, &Red, &Green, &Blue ) ;
		SETUPCOLOR( VertData->color )
	}

	// ���W�̃Z�b�g
	VertData->pos.x = ( float )x ;
	VertData->pos.y = ( float )y ;
	VertData->pos.z = GSYS.DrawSetting.DrawZ;

	// ���_�̒ǉ�
	ADD4VERTEX_POINT

	// �I��
	return 0 ;
}

// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawPixel3D
extern	int		Graphics_D3D9_DrawPixel3D( VECTOR Pos, unsigned int Color, int DrawFlag, RECT *DrawArea )
{
	VERTEX_NOTEX_3D *VertData ; 
	int Red, Green, Blue ;
	int Flag ;

	if( DrawFlag )
	{
		if( GAPIWin.Direct3DDevice9Object == NULL ) return -1 ;

		// �`��̏���
		if( GD3D9.Device.DrawInfo.BeginSceneFlag == FALSE ) Graphics_D3D9_BeginScene() ;
		if( GD3D9.Device.DrawSetting.RenderTexture != NULL ) Graphics_D3D9_DrawSetting_SetTexture( NULL ) ;

		Flag = DX_D3D9_DRAWPREP_DIFFUSERGB | DX_D3D9_DRAWPREP_3D | DX_D3D9_DRAWPREP_FOG ;
		DX_D3D9_DRAWPREP_NOTEX( Flag )

		// �F���̑��X�e�[�^�X�̃Z�b�g
		GETVERTEX_POINT3D( VertData ) ;

		NS_GetColor2( Color, &Red, &Green, &Blue ) ;
		if( GD3D9.Device.DrawInfo.DiffuseColor == 0xffffffff )
		{
			*( ( DWORD * )&VertData->b ) = ( DWORD )( 0xff000000 | ( Red << 16 ) | ( Green << 8 ) | Blue ) ;
		}
		else
		{
			SETUPCOLOR( *( ( DWORD * )&VertData->b ) )
		}

		// ���W�̃Z�b�g
		VertData->pos = Pos ;

		// ���_�̒ǉ�
		ADD4VERTEX_POINT3D
	}
	else
	{
		VERTEX_2D TempVert ;

		// �X�N���[�����W�ɕϊ�
		TempVert.pos.x = Pos.x * GSYS.DrawSetting.BlendMatrixF.m[0][0] + Pos.y * GSYS.DrawSetting.BlendMatrixF.m[1][0] + Pos.z * GSYS.DrawSetting.BlendMatrixF.m[2][0] + GSYS.DrawSetting.BlendMatrixF.m[3][0]  ;
		TempVert.pos.y = Pos.x * GSYS.DrawSetting.BlendMatrixF.m[0][1] + Pos.y * GSYS.DrawSetting.BlendMatrixF.m[1][1] + Pos.z * GSYS.DrawSetting.BlendMatrixF.m[2][1] + GSYS.DrawSetting.BlendMatrixF.m[3][1]  ;
		TempVert.pos.z = Pos.x * GSYS.DrawSetting.BlendMatrixF.m[0][2] + Pos.y * GSYS.DrawSetting.BlendMatrixF.m[1][2] + Pos.z * GSYS.DrawSetting.BlendMatrixF.m[2][2] + GSYS.DrawSetting.BlendMatrixF.m[3][2]  ;
		TempVert.rhw   = Pos.x * GSYS.DrawSetting.BlendMatrixF.m[0][3] + Pos.y * GSYS.DrawSetting.BlendMatrixF.m[1][3] + Pos.z * GSYS.DrawSetting.BlendMatrixF.m[2][3] + GSYS.DrawSetting.BlendMatrixF.m[3][3]  ;

		TempVert.rhw = 1.0F / TempVert.rhw ;
		TempVert.pos.x *= TempVert.rhw ;
		TempVert.pos.y *= TempVert.rhw ;
		TempVert.pos.z *= TempVert.rhw ;

		DrawArea->left   = _FTOL( TempVert.pos.x ) - 1 ;
		DrawArea->right  = _FTOL( TempVert.pos.x ) + 1 ;
		DrawArea->top    = _FTOL( TempVert.pos.y ) - 1 ;
		DrawArea->bottom = _FTOL( TempVert.pos.y ) + 1 ;
	}

	// �I��
	return 0 ;
}

// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawPixelSet
#define SetDrawNum (1000)
extern	int		Graphics_D3D9_DrawPixelSet( const POINTDATA *PointData, int Num )
{
	DWORD i, j/*, k*/ ;
	static VERTEX_NOTEX_2D VertData[ SetDrawNum ], *Vert ;
	COLORDATA ColorData ;
	int MaxRed, MaxGreen, MaxBlue ; 
	unsigned int Color ;
	int Flag ;
	DWORD drawz ;

	if( GAPIWin.Direct3DDevice9Object == NULL ) return -1 ;

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �`��̏���
	Graphics_D3D9_BeginScene() ;
	Flag = DX_D3D9_DRAWPREP_DIFFUSERGB ;
	DX_D3D9_DRAWPREP_NOTEX( Flag )

	// �y�o�b�t�@�ɏ������ނy�l���Z�b�g����
	drawz = *((DWORD *)&GSYS.DrawSetting.DrawZ);

	// �x�N�g����{�X�e�[�^�X���擾
	ColorData = *( ( COLORDATA * )NS_GetDispColorData() ) ;
	
	MaxRed		= ( 1 << ColorData.RedWidth	) - 1 ; 
	MaxGreen	= ( 1 << ColorData.GreenWidth ) - 1 ;
	MaxBlue		= ( 1 << ColorData.BlueWidth ) - 1 ; 

	j = 0 ;
	Vert = VertData ;

	// FVF �̃Z�b�g
//	Graphics_D3D9_DeviceState_SetFVF( VERTEXFVF_NOTEX_2D ) ;
	Graphics_Hardware_D3D9_SetFVF_UseTable( VERTEXFVF_DECL_NOTEX_2D ) ;

	for( i = 0 ; i < ( UINT )Num ; i ++, PointData ++ )
	{
		// �F���̑��X�e�[�^�X�̃Z�b�g
		Color = PointData->color ;
		Vert->color = ( PointData->pal << 24 ) |
						( ( ( ( ( ( Color & ColorData.RedMask	) >> ColorData.RedLoc	) << 8 ) - 1 ) / MaxRed		) << 16 ) |
						( ( ( ( ( ( Color & ColorData.GreenMask	) >> ColorData.GreenLoc	) << 8 ) - 1 ) / MaxGreen	) << 8 ) |
						( ( ( ( ( ( Color & ColorData.BlueMask	) >> ColorData.BlueLoc	) << 8 ) - 1 ) / MaxBlue	) ) ; 
		Vert->rhw	= 1.0f ;
		*((DWORD *)&Vert->pos.z) = drawz ;

		// ���W�̃Z�b�g���E�C���h�E���ڕ`�掞�␳
		Vert->pos.x = ( float )PointData->x ;
		Vert->pos.y = ( float )PointData->y ;

		j ++ ;
		Vert ++ ;
		if( j == SetDrawNum )
		{
			// �`��
			Direct3DDevice9_DrawPrimitiveUP( D_D3DPT_POINTLIST, j, VertData, sizeof( VERTEX_NOTEX_2D ) ) ;
			j = 0 ;
			Vert = VertData ;
		}
	}

	if( j != 0 )
	{
		// �`��
		Direct3DDevice9_DrawPrimitiveUP( D_D3DPT_POINTLIST, j, VertData, sizeof( VERTEX_NOTEX_2D ) ) ;
	}

	// �I��
	return 0;
}

// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawLineSet
extern	int		Graphics_D3D9_DrawLineSet( const LINEDATA *LineData, int Num )
{
	DWORD i, j/*, k, l */ ;
	static VERTEX_NOTEX_2D VertData[ SetDrawNum ], *Vert ;
	COLORDATA ColorData ;
	int MaxRed, MaxGreen, MaxBlue ; 
	unsigned int Color ;
	int Flag ;
	DWORD drawz ;

	if( GAPIWin.Direct3DDevice9Object == NULL ) return -1 ;

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �`��̏���
	Graphics_D3D9_BeginScene() ;
	Flag = DX_D3D9_DRAWPREP_DIFFUSERGB ;
	DX_D3D9_DRAWPREP_NOTEX( Flag )

	// �y�o�b�t�@�ɏ������ނy�l���Z�b�g����
	drawz = *((DWORD *)&GSYS.DrawSetting.DrawZ);

	// �x�N�g����{�X�e�[�^�X���擾
	ColorData = *( ( COLORDATA * )NS_GetDispColorData() ) ;
	
	MaxRed		= ( 1 << ColorData.RedWidth	) - 1 ; 
	MaxGreen	= ( 1 << ColorData.GreenWidth ) - 1 ;
	MaxBlue		= ( 1 << ColorData.BlueWidth ) - 1 ; 

	j = 0 ;
	Vert = VertData ;

	// FVF �̐ݒ�
//	Graphics_D3D9_DeviceState_SetFVF( VERTEXFVF_NOTEX_2D ) ;
	Graphics_Hardware_D3D9_SetFVF_UseTable( VERTEXFVF_DECL_NOTEX_2D ) ;

	for( i = 0 ; i < ( DWORD )Num ; i ++, LineData ++ )
	{
		// �F���̑��X�e�[�^�X�̃Z�b�g
		Color = LineData->color ;
		Vert[0].color = ( LineData->pal << 24 ) |
						( ( ( ( ( ( Color & ColorData.RedMask	) >> ColorData.RedLoc	) << 8 ) - 1 ) / MaxRed		) << 16 ) |
						( ( ( ( ( ( Color & ColorData.GreenMask	) >> ColorData.GreenLoc	) << 8 ) - 1 ) / MaxGreen	) << 8 ) |
						( ( ( ( ( ( Color & ColorData.BlueMask	) >> ColorData.BlueLoc	) << 8 ) - 1 ) / MaxBlue	) ) ; 
		Vert[1].color = Vert[0].color ;
		Vert[0].rhw	  =
		Vert[1].rhw   = 1.0f ;
		*((DWORD *)&Vert[0].pos.z) = drawz ;
		*((DWORD *)&Vert[1].pos.z) = drawz ;
 
		// ���W�̃Z�b�g���E�C���h�E���ڕ`�掞�␳
		Vert[0].pos.x = ( float )LineData->x1 ;
		Vert[0].pos.y = ( float )LineData->y1 ;
		Vert[1].pos.x = ( float )LineData->x2 ;
		Vert[1].pos.y = ( float )LineData->y2 ;

		j ++ ;
		Vert += 2 ;
		if( j * 2 == SetDrawNum )
		{
			// �`��
			Direct3DDevice9_DrawPrimitiveUP( D_D3DPT_LINELIST, j, VertData, sizeof( VERTEX_NOTEX_2D ) ) ;
			j = 0 ;
			Vert = VertData ;
		}
	}

	if( j != 0 )
	{
		// �`��
		Direct3DDevice9_DrawPrimitiveUP( D_D3DPT_LINELIST, j, VertData, sizeof( VERTEX_NOTEX_2D ) ) ;
	}

	// �I��
	return 0;
}
















extern	int		Graphics_D3D9_DrawPrimitive( const VERTEX_3D *Vertex, int VertexNum, int PrimitiveType, IMAGEDATA *Image, int TransFlag )
{
	int Flag ;

	if( GAPIWin.Direct3DDevice9Object == NULL ) return -1 ;

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �`��̏���
	Graphics_D3D9_BeginScene() ;
	Graphics_D3D9_DrawSetting_SetBlendTexture( NULL ) ;

	// �`��t���O�̃Z�b�g
	Flag = TransFlag | DX_D3D9_DRAWPREP_GOURAUDSHADE | DX_D3D9_DRAWPREP_PERSPECTIVE | DX_D3D9_DRAWPREP_DIFFUSERGB | DX_D3D9_DRAWPREP_DIFFUSEALPHA | DX_D3D9_DRAWPREP_FOG | DX_D3D9_DRAWPREP_3D | DX_D3D9_DRAWPREP_TEXADDRESS | DX_D3D9_DRAWPREP_CULLING ;

	// �`��̏���
	if( Image )
	{
		DX_D3D9_DRAWPREP_TEX( Image->Orig, Image->Hard.Draw[ 0 ].Tex->PF->D3D9.Texture, Flag )
	}
	else
	{
		DX_D3D9_DRAWPREP_NOTEX( Flag )
	}

	// �`��
//	Graphics_D3D9_DeviceState_SetFVF( VERTEXFVF_3D ) ;
	Graphics_Hardware_D3D9_SetFVF_UseTable( VERTEXFVF_DECL_3D ) ;
	Direct3DDevice9_DrawPrimitiveUP( ( D_D3DPRIMITIVETYPE )PrimitiveType, GETPRIMNUM( PrimitiveType, VertexNum ), Vertex, sizeof( VERTEX_3D ) ) ;

	// �I��
	return 0 ;
}

extern	int		Graphics_D3D9_DrawIndexedPrimitive( const VERTEX_3D *Vertex, int VertexNum, const WORD *Indices, int IndexNum, int PrimitiveType, IMAGEDATA *Image, int TransFlag )
{
	int Flag ;

	if( GAPIWin.Direct3DDevice9Object == NULL ) return -1 ;

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �`��̏���
	Graphics_D3D9_BeginScene() ;
	Graphics_D3D9_DrawSetting_SetBlendTexture( NULL ) ;

	Flag = TransFlag | DX_D3D9_DRAWPREP_GOURAUDSHADE | DX_D3D9_DRAWPREP_PERSPECTIVE | DX_D3D9_DRAWPREP_DIFFUSERGB | DX_D3D9_DRAWPREP_DIFFUSEALPHA | DX_D3D9_DRAWPREP_FOG | DX_D3D9_DRAWPREP_3D | DX_D3D9_DRAWPREP_TEXADDRESS | DX_D3D9_DRAWPREP_CULLING ;
	if( Image )
	{
		DX_D3D9_DRAWPREP_TEX( Image->Orig, Image->Hard.Draw[ 0 ].Tex->PF->D3D9.Texture, Flag )
	}
	else
	{
		DX_D3D9_DRAWPREP_NOTEX( Flag )
	}

	// �`��
//	Graphics_D3D9_DeviceState_SetFVF( VERTEXFVF_3D ) ;
	Graphics_Hardware_D3D9_SetFVF_UseTable( VERTEXFVF_DECL_3D ) ;
	Direct3DDevice9_DrawIndexedPrimitiveUP( ( D_D3DPRIMITIVETYPE )PrimitiveType, 0, ( UINT )VertexNum, GETPRIMNUM( PrimitiveType, IndexNum ), Indices, D_D3DFMT_INDEX16, Vertex, sizeof( VERTEX_3D ) ) ;

	// �I��
	return 0 ;
}

extern	int		Graphics_D3D9_DrawPrimitiveLight( const VERTEX3D *Vertex, int VertexNum, int PrimitiveType, IMAGEDATA *Image, int TransFlag )
{
	int Flag ;

	if( GAPIWin.Direct3DDevice9Object == NULL ) return -1 ;

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �`��̏���
	Graphics_D3D9_BeginScene() ;
	Graphics_D3D9_DrawSetting_SetBlendTexture( NULL ) ;

	Flag = TransFlag | DX_D3D9_DRAWPREP_GOURAUDSHADE | DX_D3D9_DRAWPREP_PERSPECTIVE | DX_D3D9_DRAWPREP_DIFFUSERGB | DX_D3D9_DRAWPREP_DIFFUSEALPHA | DX_D3D9_DRAWPREP_FOG | DX_D3D9_DRAWPREP_LIGHTING | DX_D3D9_DRAWPREP_SPECULAR | DX_D3D9_DRAWPREP_3D | DX_D3D9_DRAWPREP_TEXADDRESS | DX_D3D9_DRAWPREP_CULLING ;
	if( Image )
	{
		DX_D3D9_DRAWPREP_TEX( Image->Orig, Image->Hard.Draw[ 0 ].Tex->PF->D3D9.Texture, Flag )
	}
	else
	{
		DX_D3D9_DRAWPREP_NOTEX( Flag )
	}

	// �`��
//	Graphics_D3D9_DeviceState_SetFVF( VERTEXFVF_3D_LIGHT ) ;
	Graphics_Hardware_D3D9_SetFVF_UseTable( VERTEXFVF_DECL_3D_LIGHT ) ;
	Direct3DDevice9_DrawPrimitiveUP( ( D_D3DPRIMITIVETYPE )PrimitiveType, GETPRIMNUM( PrimitiveType, VertexNum ), Vertex, sizeof( VERTEX3D ) ) ;

	// �I��
	return 0 ;
}

extern	int		Graphics_D3D9_DrawIndexedPrimitiveLight( const VERTEX3D *Vertex, int VertexNum, const WORD *Indices, int IndexNum, int PrimitiveType, IMAGEDATA *Image, int TransFlag )
{
	int Flag ;

	if( GAPIWin.Direct3DDevice9Object == NULL ) return -1 ;

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �`��̏���
	Graphics_D3D9_BeginScene() ;
	Graphics_D3D9_DrawSetting_SetBlendTexture( NULL ) ;

	Flag = TransFlag | DX_D3D9_DRAWPREP_GOURAUDSHADE | DX_D3D9_DRAWPREP_PERSPECTIVE | DX_D3D9_DRAWPREP_DIFFUSERGB | DX_D3D9_DRAWPREP_DIFFUSEALPHA | DX_D3D9_DRAWPREP_FOG | DX_D3D9_DRAWPREP_LIGHTING | DX_D3D9_DRAWPREP_SPECULAR | DX_D3D9_DRAWPREP_3D | DX_D3D9_DRAWPREP_TEXADDRESS | DX_D3D9_DRAWPREP_CULLING ;
	if( Image )
	{
		DX_D3D9_DRAWPREP_TEX( Image->Orig, Image->Hard.Draw[ 0 ].Tex->PF->D3D9.Texture, Flag )
	}
	else
	{
		DX_D3D9_DRAWPREP_NOTEX( Flag )
	}

	// �`��
//	Graphics_D3D9_DeviceState_SetFVF( VERTEXFVF_3D_LIGHT ) ;
	Graphics_Hardware_D3D9_SetFVF_UseTable( VERTEXFVF_DECL_3D_LIGHT ) ;
	Direct3DDevice9_DrawIndexedPrimitiveUP( ( D_D3DPRIMITIVETYPE )PrimitiveType, 0, ( UINT )VertexNum, GETPRIMNUM( PrimitiveType, IndexNum ), Indices, D_D3DFMT_INDEX16, Vertex, sizeof( VERTEX3D ) ) ;

	// �I��
	return 0 ;
}

extern	int		Graphics_D3D9_DrawPrimitiveLight_UseVertexBuffer( 
	VERTEXBUFFERHANDLEDATA *VertexBuffer,
	int						PrimitiveType,
	int						StartVertex,
	int						UseVertexNum,
	IMAGEDATA *			Image,
	int						TransFlag 
)
{
	int Flag ;
	DWORD PrimitiveCount ;

	if( GAPIWin.Direct3DDevice9Object == NULL ) return -1 ;

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �`��̏���
	Graphics_D3D9_BeginScene() ;
	Graphics_D3D9_DrawSetting_SetBlendTexture( NULL ) ;

	Flag = TransFlag | DX_D3D9_DRAWPREP_GOURAUDSHADE | DX_D3D9_DRAWPREP_PERSPECTIVE | DX_D3D9_DRAWPREP_DIFFUSERGB | DX_D3D9_DRAWPREP_DIFFUSEALPHA | DX_D3D9_DRAWPREP_FOG | DX_D3D9_DRAWPREP_LIGHTING | DX_D3D9_DRAWPREP_SPECULAR | DX_D3D9_DRAWPREP_3D | DX_D3D9_DRAWPREP_TEXADDRESS | DX_D3D9_DRAWPREP_CULLING ;
	if( Image )
	{
		DX_D3D9_DRAWPREP_TEX( Image->Orig, Image->Hard.Draw[ 0 ].Tex->PF->D3D9.Texture, Flag )
	}
	else
	{
		DX_D3D9_DRAWPREP_NOTEX( Flag )
	}

	// FVF�̃Z�b�g�A�b�v
	switch( VertexBuffer->Type )
	{
	case DX_VERTEX_TYPE_NORMAL_3D:
//		Graphics_D3D9_DeviceState_SetFVF( VERTEXFVF_3D_LIGHT ) ;
		Graphics_Hardware_D3D9_SetFVF_UseTable( VERTEXFVF_DECL_3D_LIGHT ) ;
		break;
	}

	// �v���~�e�B�u�̐����擾
	PrimitiveCount = GETPRIMNUM( PrimitiveType, UseVertexNum ) ;

	// �`��
	if( VertexBuffer->PF->D3D9.VertexBuffer != NULL )
	{
		Graphics_D3D9_DeviceState_SetVertexBuffer( VertexBuffer->PF->D3D9.VertexBuffer, VertexBuffer->UnitSize ) ;
		Direct3DDevice9_DrawPrimitive(
			( D_D3DPRIMITIVETYPE )PrimitiveType,
			( UINT )StartVertex,
			PrimitiveCount ) ;
	}
	else
	{
		Direct3DDevice9_DrawPrimitiveUP(
			( D_D3DPRIMITIVETYPE )PrimitiveType,
			PrimitiveCount,
			( BYTE * )VertexBuffer->Buffer + StartVertex * VertexBuffer->UnitSize,
			( UINT )VertexBuffer->UnitSize ) ;
	}

	// �I��
	return 0 ;
}

extern	int		Graphics_D3D9_DrawIndexedPrimitiveLight_UseVertexBuffer(
	VERTEXBUFFERHANDLEDATA *	VertexBuffer,
	INDEXBUFFERHANDLEDATA *		IndexBuffer,
	int							PrimitiveType,
	int							BaseVertex,
	int							StartVertex,
	int							UseVertexNum,
	int							StartIndex,
	int							UseIndexNum,
	IMAGEDATA *				Image,
	int							TransFlag
)
{
	int Flag ;
	DWORD PrimitiveCount ;
	D_D3DFORMAT IndexFormat ;

	if( GAPIWin.Direct3DDevice9Object == NULL ) return -1 ;

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �`��̏���
	Graphics_D3D9_BeginScene() ;
	Graphics_D3D9_DrawSetting_SetBlendTexture( NULL ) ;

	Flag = TransFlag | DX_D3D9_DRAWPREP_GOURAUDSHADE | DX_D3D9_DRAWPREP_PERSPECTIVE | DX_D3D9_DRAWPREP_DIFFUSERGB | DX_D3D9_DRAWPREP_DIFFUSEALPHA | DX_D3D9_DRAWPREP_FOG | DX_D3D9_DRAWPREP_LIGHTING | DX_D3D9_DRAWPREP_SPECULAR | DX_D3D9_DRAWPREP_3D | DX_D3D9_DRAWPREP_TEXADDRESS | DX_D3D9_DRAWPREP_CULLING ;
	if( Image )
	{
		DX_D3D9_DRAWPREP_TEX( Image->Orig, Image->Hard.Draw[ 0 ].Tex->PF->D3D9.Texture, Flag )
	}
	else
	{
		DX_D3D9_DRAWPREP_NOTEX( Flag )
	}

	// FVF�̃Z�b�g�A�b�v
	switch( VertexBuffer->Type )
	{
	case DX_VERTEX_TYPE_NORMAL_3D:
//		Graphics_D3D9_DeviceState_SetFVF( VERTEXFVF_3D_LIGHT ) ;
		Graphics_Hardware_D3D9_SetFVF_UseTable( VERTEXFVF_DECL_3D_LIGHT ) ;
		break ;
	}

	// �C���f�b�N�X�^�C�v���Z�b�g
	IndexFormat = D_D3DFMT_INDEX16 ;
	switch( IndexBuffer->Type )
	{
	case DX_INDEX_TYPE_16BIT:
		IndexFormat = D_D3DFMT_INDEX16 ;
		break ;

	case DX_INDEX_TYPE_32BIT:
		IndexFormat = D_D3DFMT_INDEX32 ;
		break ;
	}

	// �v���~�e�B�u�̐����擾
	PrimitiveCount = GETPRIMNUM( PrimitiveType, UseIndexNum ) ;

	// �`��
	if( VertexBuffer->PF->D3D9.VertexBuffer != NULL &&
		IndexBuffer->PF->D3D9.IndexBuffer  != NULL )
	{
		Graphics_D3D9_DeviceState_SetVertexBuffer( VertexBuffer->PF->D3D9.VertexBuffer, VertexBuffer->UnitSize ) ;
		Graphics_D3D9_DeviceState_SetIndexBuffer( IndexBuffer->PF->D3D9.IndexBuffer ) ;

		Direct3DDevice9_DrawIndexedPrimitive(
			( D_D3DPRIMITIVETYPE )PrimitiveType,
			BaseVertex,
			( UINT )StartVertex,
			( UINT )UseVertexNum,
			( UINT )StartIndex,
			PrimitiveCount ) ;
	}
	else
	{
		Direct3DDevice9_DrawIndexedPrimitiveUP(
			( D_D3DPRIMITIVETYPE )PrimitiveType,
			( UINT )StartVertex,
			( UINT )UseVertexNum,
			PrimitiveCount,
			( BYTE * )IndexBuffer->Buffer + StartIndex * IndexBuffer->UnitSize,
			IndexFormat, 
			( BYTE * )VertexBuffer->Buffer + BaseVertex * VertexBuffer->UnitSize,
			( UINT )VertexBuffer->UnitSize ) ;
	}

	// �I��
	return 0 ;
}

extern	int		Graphics_D3D9_DrawPrimitive2D( VERTEX_2D *Vertex, int VertexNum, int PrimitiveType, IMAGEDATA *Image, int TransFlag, int BillboardFlag, int Is3D, int TurnFlag, int TextureNo, int IsShadowMap )
{
	int Flag ;
	int i ;
	VERTEX_2D *Vert ;
	int SmHandle ;
	SHADOWMAPDATA *ShadowMap ;

	if( GAPIWin.Direct3DDevice9Object == NULL ) return -1 ;

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �`��̏���
	Graphics_D3D9_BeginScene() ;
	Graphics_D3D9_DrawSetting_SetBlendTexture( NULL ) ;

	if( BillboardFlag == TRUE )
	{
		Flag = TransFlag | ( Is3D ? DX_D3D9_DRAWPREP_3D | DX_D3D9_DRAWPREP_FOG : 0 ) | DX_D3D9_DRAWPREP_TEXADDRESS ;
	}
	else
	{
		Flag = TransFlag | ( Is3D ? DX_D3D9_DRAWPREP_3D | DX_D3D9_DRAWPREP_FOG : 0 ) | DX_D3D9_DRAWPREP_GOURAUDSHADE | DX_D3D9_DRAWPREP_PERSPECTIVE | DX_D3D9_DRAWPREP_DIFFUSERGB | DX_D3D9_DRAWPREP_DIFFUSEALPHA | DX_D3D9_DRAWPREP_TEXADDRESS | DX_D3D9_DRAWPREP_CULLING ;
	}
	if( IsShadowMap )
	{
		SmHandle = ( int )( DWORD_PTR )Image ;
		if( !SHADOWMAPCHKFULL( SmHandle, ShadowMap ) )
		{
			Flag |= DX_D3D9_DRAWPREP_TEXTURE ;
			Graphics_D3D9_DrawSetting_SetTexture( ShadowMap->PF->D3D9.Texture ) ;
			if( GSYS.ChangeSettingFlag ||
				GD3D9.Device.DrawSetting.DrawPrepAlwaysFlag != FALSE ||
				GD3D9.Device.DrawSetting.DrawPrepParamFlag != Flag )
			{
				Graphics_D3D9_DrawPreparation( Flag ) ;
			}
		}
	}
	else
	if( Image )
	{
		DX_D3D9_DRAWPREP_TEX( Image->Orig, Image->Hard.Draw[ TextureNo ].Tex->PF->D3D9.Texture, Flag )
	}
	else
	{
		DX_D3D9_DRAWPREP_NOTEX( Flag )
	}

	// �r���{�[�h�̏ꍇ�͂t�u�l���T�[�t�F�X�f�[�^����擾����
	if( BillboardFlag == TRUE )
	{
		float tu1, tv1, tu2, tv2 ;
		int Num ;
		
		Vert = Vertex ;
		Num = VertexNum / 6 ;
		tu1 = Image->Hard.Draw[ 0 ].Vertex[ 0 ].u ;
		tv1 = Image->Hard.Draw[ 0 ].Vertex[ 0 ].v ;
		tu2 = Image->Hard.Draw[ 0 ].Vertex[ 3 ].u ;
		tv2 = Image->Hard.Draw[ 0 ].Vertex[ 3 ].v ;
		for( i = 0 ; i < Num ; i ++, Vert += 6 )
		{
			if( TurnFlag )
			{
				Vert[ 0 ].u = tu2 ;
				Vert[ 1 ].u = tu1 ;
				Vert[ 2 ].u = tu2 ;
				Vert[ 3 ].u = tu1 ;
			}
			else
			{
				Vert[ 0 ].u = tu1 ;
				Vert[ 1 ].u = tu2 ;
				Vert[ 2 ].u = tu1 ;
				Vert[ 3 ].u = tu2 ;
			}
			Vert[ 0 ].v = tv1 ;
			Vert[ 1 ].v = tv1 ;
			Vert[ 2 ].v = tv2 ;
			Vert[ 3 ].v = tv2 ;

			Vert[ 4 ].u = Vert[ 2 ].u ;
			Vert[ 4 ].v = Vert[ 2 ].v ;
			Vert[ 5 ].u = Vert[ 1 ].u ;
			Vert[ 5 ].v = Vert[ 1 ].v ;
		}
	}

	// �`��
//	Graphics_D3D9_DeviceState_SetFVF( VERTEXFVF_2D ) ;
	Graphics_Hardware_D3D9_SetFVF_UseTable( VERTEXFVF_DECL_2D ) ;
	Direct3DDevice9_DrawPrimitiveUP( ( D_D3DPRIMITIVETYPE )PrimitiveType, GETPRIMNUM( PrimitiveType, VertexNum ), Vertex, sizeof( VERTEX_2D ) ) ;

	// �I��
	return 0 ;
}

extern	int		Graphics_D3D9_DrawPrimitive2DUser( const VERTEX2D *Vertex, int VertexNum, int PrimitiveType, IMAGEDATA *Image, int TransFlag, int Is3D, int /* TurnFlag */, int TextureNo )
{
	int Flag ;

	if( GAPIWin.Direct3DDevice9Object == NULL ) return -1 ;

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �`��̏���
	Graphics_D3D9_BeginScene() ;
	Graphics_D3D9_DrawSetting_SetBlendTexture( NULL ) ;

	Flag = TransFlag | ( Is3D ? DX_D3D9_DRAWPREP_3D | DX_D3D9_DRAWPREP_FOG : 0 ) | DX_D3D9_DRAWPREP_GOURAUDSHADE | DX_D3D9_DRAWPREP_PERSPECTIVE | DX_D3D9_DRAWPREP_DIFFUSERGB | DX_D3D9_DRAWPREP_DIFFUSEALPHA | DX_D3D9_DRAWPREP_TEXADDRESS | DX_D3D9_DRAWPREP_CULLING ;
	if( Image )
	{
		DX_D3D9_DRAWPREP_TEX( Image->Orig, Image->Hard.Draw[ TextureNo ].Tex->PF->D3D9.Texture, Flag )
	}
	else
	{
		DX_D3D9_DRAWPREP_NOTEX( Flag )
	}

	// �`��
//	Graphics_D3D9_DeviceState_SetFVF( VERTEXFVF_2D_USER ) ;
	Graphics_Hardware_D3D9_SetFVF_UseTable( VERTEXFVF_DECL_2D_USER ) ;
	Direct3DDevice9_DrawPrimitiveUP( ( D_D3DPRIMITIVETYPE )PrimitiveType, GETPRIMNUM( PrimitiveType, VertexNum ), Vertex, sizeof( VERTEX2D ) ) ;

	// �I��
	return 0 ;
}

extern	int		Graphics_D3D9_DrawIndexedPrimitive2DUser( const VERTEX2D *Vertex, int VertexNum, const WORD *Indices, int IndexNum, int PrimitiveType, IMAGEDATA *Image, int TransFlag )
{
	int Flag ;

	if( GAPIWin.Direct3DDevice9Object == NULL ) return -1 ;

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �`��̏���
	Graphics_D3D9_BeginScene() ;
	Graphics_D3D9_DrawSetting_SetBlendTexture( NULL ) ;

	Flag = TransFlag | DX_D3D9_DRAWPREP_GOURAUDSHADE | DX_D3D9_DRAWPREP_PERSPECTIVE | DX_D3D9_DRAWPREP_DIFFUSERGB | DX_D3D9_DRAWPREP_DIFFUSEALPHA | DX_D3D9_DRAWPREP_TEXADDRESS | DX_D3D9_DRAWPREP_CULLING ;
	if( Image )
	{
		DX_D3D9_DRAWPREP_TEX( Image->Orig, Image->Hard.Draw[ 0 ].Tex->PF->D3D9.Texture, Flag )
	}
	else
	{
		DX_D3D9_DRAWPREP_NOTEX( Flag )
	}

	// �`��
//	Graphics_D3D9_DeviceState_SetFVF( VERTEXFVF_2D_USER ) ;
	Graphics_Hardware_D3D9_SetFVF_UseTable( VERTEXFVF_DECL_2D_USER ) ;
	Direct3DDevice9_DrawIndexedPrimitiveUP( ( D_D3DPRIMITIVETYPE )PrimitiveType, 0, ( UINT )VertexNum, GETPRIMNUM( PrimitiveType, IndexNum ), Indices, D_D3DFMT_INDEX16, Vertex, sizeof( VERTEX2D ) ) ;

	// �I��
	return 0 ;
}












// �V�F�[�_�[�`��p�`��O�Z�b�g�A�b�v�֐�
extern void Graphics_D3D9_DrawPreparationToShader( int ParamFlag, int UseVertexShaderNo, int Is2D, int UseLibSubShaderConst )
{
	GRAPHICS_HARDDATA_DIRECT3D9_BLENDINFO BlendInfo ;
	int i, Flag ;

	// ��{�I�ȃZ�b�g�A�b�v����
	Flag = ParamFlag | DX_D3D9_DRAWPREP_SPECULAR | DX_D3D9_DRAWPREP_NOTSHADERRESET | DX_D3D9_DRAWPREP_TEXADDRESS | DX_D3D9_DRAWPREP_NOBLENDSETTING | ( Is2D ? 0 : DX_D3D9_DRAWPREP_CULLING ) ;
	DX_D3D9_DRAWPREP_NOTEX( Flag )

	// ���[�U�[�ݒ�̃V�F�[�_�[�萔��L���ɂ���
	if( UseLibSubShaderConst )
	{
		Graphics_D3D9_ShaderConstant_InfoSet_SetUseState( &GD3D9.Device.Shader.ShaderConstantInfo, DX_SHADERCONSTANTSET_MASK_LIB | DX_SHADERCONSTANTSET_MASK_LIB_SUB ) ;
	}
	else
	{
		Graphics_D3D9_ShaderConstant_InfoSet_SetUseState( &GD3D9.Device.Shader.ShaderConstantInfo, DX_SHADERCONSTANTSET_MASK_LIB | DX_SHADERCONSTANTSET_MASK_USER ) ;
	}

	// ���_�t�H�[�}�b�g�̓��Z�b�g
	Graphics_D3D9_DeviceState_SetVertexDeclaration( NULL ) ;

	// �u�����h���[�h�͎��O�ŃZ�b�g�A�b�v
	{
		BlendInfo.SeparateAlphaBlendEnable = FALSE ;
		BlendInfo.SrcBlendAlpha            = -1 ;
		BlendInfo.DestBlendAlpha           = -1 ;
		BlendInfo.BlendOpAlpha             = -1 ;

		// �A���t�@�e�X�g�p�����[�^���L���ȏꍇ�͂����D�悷��
		if( GSYS.DrawSetting.AlphaTestMode != -1 )
		{
			BlendInfo.AlphaTestEnable = TRUE ;
			BlendInfo.AlphaFunc       = GSYS.DrawSetting.AlphaTestMode ;
			BlendInfo.AlphaRef        = GSYS.DrawSetting.AlphaTestParam ;
		}
		else
		{
			// �f�t�H���g�̃A���t�@�e�X�g�̐ݒ���Z�b�g
			BlendInfo.AlphaTestEnable = TRUE ;
			BlendInfo.AlphaRef        = 0 ;
			BlendInfo.AlphaFunc       = D_D3DCMP_GREATER ;
		}

		// �o�͐�Ƃ̃u�����h�������Z�b�g
		BlendInfo.AlphaBlendEnable = GSYS.DrawSetting.BlendMode != DX_BLENDMODE_NOBLEND && GSYS.DrawSetting.BlendMode != DX_BLENDMODE_XOR ;
		BlendInfo.DestBlend        = __RGBBlendStateTable[ GSYS.DrawSetting.BlendMode ][ RGB_BLENDSTATE_TABLE_DESTBLEND ] ;
		BlendInfo.SrcBlend         = __RGBBlendStateTable[ GSYS.DrawSetting.BlendMode ][ RGB_BLENDSTATE_TABLE_SRCBLEND ] ;
		BlendInfo.BlendOp          = __RGBBlendStateTable[ GSYS.DrawSetting.BlendMode ][ RGB_BLENDSTATE_TABLE_BLENDOP ] ;
		BlendInfo.FactorColor      = 0xffffffff ;

		// ���Z�u�����h�w��� D3DRS_BLENDOP ���g�p�\�ȏꍇ�� 14�Ԗڂ̃u�����h���[�h���g�p����
		if( GSYS.DrawSetting.BlendMode == DX_BLENDMODE_SUB && GD3D9.Device.Caps.ValidDestBlendOp )
		{
			BlendInfo.DestBlend   = __RGBBlendStateTable[ DX_BLENDMODE_SUB1 ][ RGB_BLENDSTATE_TABLE_DESTBLEND ] ;
			BlendInfo.SrcBlend    = __RGBBlendStateTable[ DX_BLENDMODE_SUB1 ][ RGB_BLENDSTATE_TABLE_SRCBLEND ] ;
			BlendInfo.BlendOp     = __RGBBlendStateTable[ DX_BLENDMODE_SUB1 ][ RGB_BLENDSTATE_TABLE_BLENDOP ] ;
			BlendInfo.FactorColor = 0xffffffff ;
		}

		// �g�p����e�N�X�`�����
		BlendInfo.UseTextureStageNum = 0 ;
		for( i = 0 ; i < USE_TEXTURESTAGE_NUM ; i ++ )
		{
			BlendInfo.TextureStageInfo[ i ].Texture = ( void * )( DWORD_PTR )GSYS.DrawSetting.UserShaderRenderInfo.SetTextureGraphHandle[ i ] ;
			BlendInfo.TextureStageInfo[ i ].TextureCoordIndex = i ;
			if( GSYS.DrawSetting.UserShaderRenderInfo.SetTextureGraphHandle[ i ] == 0 ) continue ;
			BlendInfo.UseTextureStageNum = i + 1 ;
		}

		// �ݒ�𔽉f
		Graphics_D3D9_DeviceState_SetUserBlendInfo( &BlendInfo, TRUE, TRUE ) ;

		// �g�p����e�N�X�`�����W�����Z�b�g
		Graphics_D3D9_DeviceState_ResetTextureCoord() ;
	}

	// ���_�V�F�[�_�[���g�p���邩�ǂ����ŏ����𕪊�
	if( UseVertexShaderNo >= 0 )
	{
		// �g�p���钸�_�f�[�^�t�H�[�}�b�g���X�V
		Graphics_D3D9_DeviceState_SetVertexDeclaration( GD3D9.Device.VertexDeclaration.UserShaderDeclaration[ UseVertexShaderNo ] ) ;

		// �g�p���钸�_�V�F�[�_�[���X�V
		Graphics_D3D9_DeviceState_SetVertexShaderToHandle( GSYS.DrawSetting.UserShaderRenderInfo.SetVertexShaderHandle ) ;
	}
	else
	{
		// �g�p���Ȃ��ꍇ�͒��_�V�F�[�_�[�����Z�b�g����
		Graphics_D3D9_DeviceState_ResetVertexShader( TRUE ) ;

		// ���_�f�[�^�`�����Z�b�g
		if( Is2D )
		{
//			Graphics_D3D9_DeviceState_SetFVF( VERTEXFVF_SHADER_2D ) ;
			Graphics_Hardware_D3D9_SetFVF_UseTable( VERTEXFVF_DECL_SHADER_2D ) ;
		}
		else
		{
//			Graphics_D3D9_DeviceState_SetFVF( VERTEXFVF_SHADER_3D ) ;
			Graphics_Hardware_D3D9_SetFVF_UseTable( VERTEXFVF_DECL_SHADER_3D ) ;
		}
	}

	// �g�p����s�N�Z���V�F�[�_�[���X�V
	Graphics_D3D9_DeviceState_SetPixelShaderToHandle( GSYS.DrawSetting.UserShaderRenderInfo.SetPixelShaderHandle ) ;
}

// �V�F�[�_�[���g���ĂQ�c�v���~�e�B�u��`�悷��
extern	int		Graphics_D3D9_DrawPrimitive2DToShader(        const VERTEX2DSHADER *Vertex, int VertexNum,                                              int PrimitiveType /* DX_PRIMTYPE_TRIANGLELIST �� */ )
{
	if( GAPIWin.Direct3DDevice9Object == NULL ) return -1 ;
	if( GSYS.HardInfo.UseShader == FALSE ) return -1 ;

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �`��̏���
	Graphics_D3D9_BeginScene() ;
	Graphics_D3D9_DrawPreparationToShader( DX_D3D9_DRAWPREP_GOURAUDSHADE | DX_D3D9_DRAWPREP_PERSPECTIVE, -1, TRUE, FALSE ) ;

	// �`��
	Direct3DDevice9_DrawPrimitiveUP( ( D_D3DPRIMITIVETYPE )PrimitiveType, GETPRIMNUM( PrimitiveType, VertexNum ), Vertex, sizeof( VERTEX2DSHADER ) ) ;

	// �I��
	return 0 ;
}

// �V�F�[�_�[���g���ĂR�c�v���~�e�B�u��`�悷��
extern	int		Graphics_D3D9_DrawPrimitive3DToShader(        const VERTEX3DSHADER *Vertex, int VertexNum,                                              int PrimitiveType /* DX_PRIMTYPE_TRIANGLELIST �� */ )
{
	if( GAPIWin.Direct3DDevice9Object == NULL ) return -1 ;
	if( GSYS.HardInfo.UseShader == FALSE ) return -1 ;

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �`��̏���
	Graphics_D3D9_BeginScene() ;
	Graphics_D3D9_DrawPreparationToShader( DX_D3D9_DRAWPREP_3D | DX_D3D9_DRAWPREP_GOURAUDSHADE | DX_D3D9_DRAWPREP_FOG | DX_D3D9_DRAWPREP_PERSPECTIVE | DX_D3D9_DRAWPREP_LIGHTING, DX_VERTEX_TYPE_SHADER_3D, FALSE, FALSE ) ;

	// �s����n�[�h�E�G�A�ɔ��f����
	Graphics_DrawSetting_ApplyLibMatrixToHardware() ;

	// �`��
	Direct3DDevice9_DrawPrimitiveUP( ( D_D3DPRIMITIVETYPE )PrimitiveType, GETPRIMNUM( PrimitiveType, VertexNum ), Vertex, sizeof( VERTEX3DSHADER ) ) ;

	// �I��
	return 0 ;
}

// �V�F�[�_�[���g���ĂQ�c�v���~�e�B�u��`�悷��( ���_�C���f�b�N�X���g�p���� )
extern	int		Graphics_D3D9_DrawPrimitiveIndexed2DToShader( const VERTEX2DSHADER *Vertex, int VertexNum, const unsigned short *Indices, int IndexNum, int PrimitiveType /* DX_PRIMTYPE_TRIANGLELIST �� */ )
{
	if( GAPIWin.Direct3DDevice9Object == NULL ) return -1 ;
	if( GSYS.HardInfo.UseShader == FALSE ) return -1 ;

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �`��̏���
	Graphics_D3D9_BeginScene() ;
	Graphics_D3D9_DrawPreparationToShader( DX_D3D9_DRAWPREP_GOURAUDSHADE | DX_D3D9_DRAWPREP_PERSPECTIVE, -1, TRUE, FALSE ) ;

	// �`��
	Direct3DDevice9_DrawIndexedPrimitiveUP( ( D_D3DPRIMITIVETYPE )PrimitiveType, 0, ( UINT )VertexNum, GETPRIMNUM( PrimitiveType, IndexNum ), Indices, D_D3DFMT_INDEX16, Vertex, sizeof( VERTEX2DSHADER ) ) ;

	// �I��
	return 0 ;
}

// �V�F�[�_�[���g���ĂR�c�v���~�e�B�u��`�悷��( ���_�C���f�b�N�X���g�p���� )
extern	int		Graphics_D3D9_DrawPrimitiveIndexed3DToShader( const VERTEX3DSHADER *Vertex, int VertexNum, const unsigned short *Indices, int IndexNum, int PrimitiveType /* DX_PRIMTYPE_TRIANGLELIST �� */ )
{
	if( GAPIWin.Direct3DDevice9Object == NULL ) return -1 ;
	if( GSYS.HardInfo.UseShader == FALSE ) return -1 ;

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �`��̏���
	Graphics_D3D9_BeginScene() ;
	Graphics_D3D9_DrawPreparationToShader( DX_D3D9_DRAWPREP_3D | DX_D3D9_DRAWPREP_GOURAUDSHADE | DX_D3D9_DRAWPREP_FOG | DX_D3D9_DRAWPREP_PERSPECTIVE | DX_D3D9_DRAWPREP_LIGHTING, DX_VERTEX_TYPE_SHADER_3D, FALSE, FALSE ) ;

	// �s����n�[�h�E�G�A�ɔ��f����
	Graphics_DrawSetting_ApplyLibMatrixToHardware() ;

	// �`��
	Direct3DDevice9_DrawIndexedPrimitiveUP( ( D_D3DPRIMITIVETYPE )PrimitiveType, 0, ( UINT )VertexNum, GETPRIMNUM( PrimitiveType, IndexNum ), Indices, D_D3DFMT_INDEX16, Vertex, sizeof( VERTEX3DSHADER ) ) ;

	// �I��
	return 0 ;
}

// �V�F�[�_�[���g���ĂR�c�v���~�e�B�u��`�悷��( ���_�o�b�t�@�g�p�� )
extern	int		Graphics_D3D9_DrawPrimitive3DToShader_UseVertexBuffer2(        int VertexBufHandle,                     int PrimitiveType /* DX_PRIMTYPE_TRIANGLELIST �� */, int StartVertex, int UseVertexNum )
{
	VERTEXBUFFERHANDLEDATA *VertexBuffer ;
	DWORD PrimitiveCount ;

	// �G���[����
	if( VERTEXBUFFERCHK( VertexBufHandle, VertexBuffer ) )
		return -1 ;

	// �s����n�[�h�E�G�A�ɔ��f����
	Graphics_DrawSetting_ApplyLibMatrixToHardware() ;

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �`��̏���
	Graphics_D3D9_BeginScene() ;
	Graphics_D3D9_DrawPreparationToShader( DX_D3D9_DRAWPREP_3D | DX_D3D9_DRAWPREP_GOURAUDSHADE | DX_D3D9_DRAWPREP_FOG | DX_D3D9_DRAWPREP_PERSPECTIVE | DX_D3D9_DRAWPREP_LIGHTING, VertexBuffer->Type, FALSE, FALSE ) ;

	// �v���~�e�B�u�̐����擾
	PrimitiveCount = GETPRIMNUM( PrimitiveType, UseVertexNum ) ;

	// �`��
	if( VertexBuffer->PF->D3D9.VertexBuffer != NULL )
	{
		Graphics_D3D9_DeviceState_SetVertexBuffer( VertexBuffer->PF->D3D9.VertexBuffer, VertexBuffer->UnitSize ) ;
		Direct3DDevice9_DrawPrimitive(
			( D_D3DPRIMITIVETYPE )PrimitiveType,
			( UINT )StartVertex,
			PrimitiveCount ) ;
	}
	else
	{
		Direct3DDevice9_DrawPrimitiveUP(
			( D_D3DPRIMITIVETYPE )PrimitiveType,
			PrimitiveCount,
			( BYTE * )VertexBuffer->Buffer + StartVertex * VertexBuffer->UnitSize,
			( UINT )VertexBuffer->UnitSize ) ;
	}

	// �I��
	return 0 ;
}

// �V�F�[�_�[���g���ĂR�c�v���~�e�B�u��`�悷��( ���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@�g�p�� )
extern	int		Graphics_D3D9_DrawPrimitiveIndexed3DToShader_UseVertexBuffer2( int VertexBufHandle, int IndexBufHandle, int PrimitiveType /* DX_PRIMTYPE_TRIANGLELIST �� */, int BaseVertex, int StartVertex, int UseVertexNum, int StartIndex, int UseIndexNum )
{
	VERTEXBUFFERHANDLEDATA *VertexBuffer ;
	INDEXBUFFERHANDLEDATA *IndexBuffer ;
	DWORD PrimitiveCount ;
	D_D3DFORMAT IndexFormat ;

	// �G���[����
	if( VERTEXBUFFERCHK( VertexBufHandle, VertexBuffer ) )
		return -1 ;
	if( INDEXBUFFERCHK( IndexBufHandle, IndexBuffer ) )
		return -1 ;

	// �s����n�[�h�E�G�A�ɔ��f����
	Graphics_DrawSetting_ApplyLibMatrixToHardware() ;

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �`��̏���
	Graphics_D3D9_BeginScene() ;
	Graphics_D3D9_DrawPreparationToShader( DX_D3D9_DRAWPREP_3D | DX_D3D9_DRAWPREP_GOURAUDSHADE | DX_D3D9_DRAWPREP_FOG | DX_D3D9_DRAWPREP_PERSPECTIVE | DX_D3D9_DRAWPREP_LIGHTING, VertexBuffer->Type, FALSE, FALSE ) ;

	// �C���f�b�N�X�^�C�v���Z�b�g
	IndexFormat = D_D3DFMT_INDEX16 ;
	switch( IndexBuffer->Type )
	{
	case DX_INDEX_TYPE_16BIT:
		IndexFormat = D_D3DFMT_INDEX16 ;
		break ;

	case DX_INDEX_TYPE_32BIT:
		IndexFormat = D_D3DFMT_INDEX32 ;
		break ;
	}

	// �v���~�e�B�u�̐����擾
	PrimitiveCount = GETPRIMNUM( PrimitiveType, UseIndexNum ) ;

	// �`��
	if( VertexBuffer->PF->D3D9.VertexBuffer != NULL &&
		IndexBuffer->PF->D3D9.IndexBuffer  != NULL )
	{
		Graphics_D3D9_DeviceState_SetVertexBuffer( VertexBuffer->PF->D3D9.VertexBuffer, VertexBuffer->UnitSize ) ;
		Graphics_D3D9_DeviceState_SetIndexBuffer( IndexBuffer->PF->D3D9.IndexBuffer ) ;

		Direct3DDevice9_DrawIndexedPrimitive(
			( D_D3DPRIMITIVETYPE )PrimitiveType,
			BaseVertex,
			( UINT )StartVertex,
			( UINT )UseVertexNum,
			( UINT )StartIndex,
			PrimitiveCount ) ;
	}
	else
	{
		Direct3DDevice9_DrawIndexedPrimitiveUP(
			( D_D3DPRIMITIVETYPE )PrimitiveType,
			( UINT )StartVertex,
			( UINT )UseVertexNum,
			PrimitiveCount,
			( BYTE * )IndexBuffer->Buffer + StartIndex * IndexBuffer->UnitSize,
			IndexFormat, 
			( BYTE * )VertexBuffer->Buffer + BaseVertex * VertexBuffer->UnitSize,
			( UINT )VertexBuffer->UnitSize ) ;
	}

	// �I��
	return 0 ;
}
















// Direct3D �̃A�_�v�^����񋓂���
static int EnumDirect3DAdapter()
{
	int i ;

	if( GD3D9.Adapter.ValidInfo ) return 0 ;

	// ����������
	if( DxSysData.DxLib_InitializeFlag == FALSE || DxLib_GetEndRequest() )
	{
		// Direct3D9.DLL �̓ǂݍ���
		if( Direct3D9_LoadDLL() < 0 )
			return -1 ;

		// Direct3D9 �̍쐬
		if( Direct3D9_Create() == 0 )
		{
			GD3D9.Adapter.InfoNum = ( int )Direct3D9_GetAdapterCount() ;
			for( i = 0 ; i < GD3D9.Adapter.InfoNum ; i ++ )
				Direct3D9_GetAdapterIdentifier( ( UINT )i, 0, &GD3D9.Adapter.Info[ i ] ) ;

			// Direct3D9 �̉��
			Direct3D9_Release() ;
		}

		// d3d9.dll �̉��
		Direct3D9_FreeDLL() ;
	}
	else
	{
		GD3D9.Adapter.InfoNum = ( int )Direct3D9_GetAdapterCount() ;
		for( i = 0 ; i < GD3D9.Adapter.InfoNum ; i ++ )
			Direct3D9_GetAdapterIdentifier( ( UINT )i, 0, &GD3D9.Adapter.Info[ i ] ) ;
	}

	GD3D9.Adapter.ValidInfo = TRUE ;

	// ����I��
	return 0 ;
}





















// ���ˑ��������֌W

// �`�揈���̊��ˑ������̏��������s���֐�( ���s�ӏ���ʂO )
extern	int		Graphics_D3D9_Initialize_Timing0_PF( void )
{
	// Direct3D9 ���g�p�����O���t�B�b�N�X�����̏�����
	if( Graphics_D3D9_Initialize() == -1 )
	{
		return -1 ;
	}

	// �V�F�[�_�[�萔����������
	Graphics_D3D9_ShaderConstant_InfoSet_Initialize( &GD3D9.Device.Shader.ShaderConstantInfo ) ;
	Graphics_D3D9_ShaderConstant_InfoSet_SetUseState( &GD3D9.Device.Shader.ShaderConstantInfo, DX_SHADERCONSTANTSET_MASK_LIB ) ;

	// ����I��
	return 0 ;
}

// �`�揈���̊��ˑ������̏��������s���֐�( ���s�ӏ���ʂQ )
extern	int		Graphics_D3D9_Initialize_Timing1_PF( void )
{
	// �f�o�C�X�̐ݒ�����t���b�V��
	Graphics_D3D9_DeviceState_RefreshRenderState() ;

	// �s�N�Z���V�F�[�_�[���g�p����ꍇ�͏o�͐���e�N�X�`���Ƃ��Ďg�p�ł��邩�ǂ����̃`�F�b�N���s��
	Graphics_D3D9_SetupRenderTargetInputTextureFlag() ;

	// ����h��Ԃ� DrawCircle �̍ۂ� X ���W�ɑ����ׂ��l�𒲂ׂ�
	{
		DWORD RetL ;
		DWORD RetR ;

		GD3D9.Device.Caps.DrawFillCircleLeftVertAddX  = -0.5f ;
		GD3D9.Device.Caps.DrawFillCircleRightVertAddX = -0.5f ;

		NS_DrawCircle( 16, 16, 10, GetColor( 255,255,255 ), TRUE ) ;
		RetL = NS_GetPixel( 5, 16 ) ;
		RetR = NS_GetPixel( 26, 16 ) ;
		NS_DrawBox( 5, 5, 28, 28, GetColor( GSYS.Screen.BackgroundRed, GSYS.Screen.BackgroundGreen, GSYS.Screen.BackgroundBlue ), TRUE ) ;

		if( RetL != 0 )
		{
			GD3D9.Device.Caps.DrawFillCircleLeftVertAddX  = -0.1f ;
		}

		if( RetR == 0 )
		{
			GD3D9.Device.Caps.DrawFillCircleLeftVertAddX  = -0.1f ;
		}

		if( RetL != 0 || RetR == 0 )
		{
			NS_DrawCircle( 16, 16, 10, GetColor( 255,255,255 ), TRUE ) ;
			RetL = NS_GetPixel( 5, 16 ) ;
			RetR = NS_GetPixel( 26, 16 ) ;
			NS_DrawBox( 5, 5, 28, 28, GetColor( GSYS.Screen.BackgroundRed, GSYS.Screen.BackgroundGreen, GSYS.Screen.BackgroundBlue ), TRUE ) ;

			if( RetL != 0 )
			{
				GD3D9.Device.Caps.DrawFillCircleLeftVertAddX  = 0.0f ;
			}

			if( RetR == 0 )
			{
				GD3D9.Device.Caps.DrawFillCircleLeftVertAddX  = 0.0f ;
			}
		}
	}

	// ����I��
	return 0 ;
}

// �n�[�h�E�G�A�A�N�Z�����[�^���g�p����ꍇ�̊��ˑ��̏������������s��
extern	int		Graphics_D3D9_Hardware_Initialize_PF( void )
{
	return Graphics_D3D9_Device_Initialize() ;
}

// �`�揈���̊��ˑ������̌�n�����s���֐�
extern	int		Graphics_D3D9_Terminate_PF( void )
{
	// Direct3D9 �̕`�揈���̌�n��
	Graphics_D3D9_Terminate() ;

	// �V�F�[�_�[�R�[�h�̌�n��
//	ShaderCodeTerminate() ;
	Graphics_D3D9_ShaderCode_2D_Terminate() ;

#ifndef DX_NON_MODEL
	// ShaderCodeTerminate_Model() ;
	Graphics_D3D9_ShaderCode_Model_Terminate() ;
#endif // DX_NON_MODEL

	// �I��
	return 0 ;
}

// �O���t�B�b�N�X�V�X�e���̕��A�A���͕ύX�t���̍ăZ�b�g�A�b�v���s��
extern	int		Graphics_D3D9_RestoreOrChangeSetupGraphSystem_PF( int Change, int ScreenSizeX, int ScreenSizeY, int ColorBitDepth, int RefreshRate )
{
	int i ;

	// �O���t�B�b�N�X�f�o�C�X�̃��X�g���畜�A����O�ɌĂԊ֐����o�^����Ă�������s����
	if( GD3D9.Device.Setting.DeviceLostCallbackFunction )
	{
		GD3D9.Device.Setting.DeviceLostCallbackFunction( GD3D9.Device.Setting.DeviceLostCallbackData ) ;
	}

	if( DxSysData.NotDrawFlag == FALSE )
	{
		// DirectX �̃I�u�W�F�N�g���������
		Graphics_ReleaseDirectXObject() ;
	}

	if( Change == TRUE )
	{
		// ��ʃ��[�h�̃Z�b�g
		Graphics_Screen_SetMainScreenSize( ScreenSizeX, ScreenSizeY ) ;
		GSYS.Screen.MainScreenColorBitDepth = ColorBitDepth ;
		GSYS.Screen.MainScreenRefreshRate   = RefreshRate ;
		SetMemImgDefaultColorType( ColorBitDepth == 32 ? 1 : 0 ) ;
	}
	else
	{
		// DirectInput �I�u�W�F�N�g�̍ăZ�b�g�A�b�v
#ifndef DX_NON_INPUT
		TerminateInputSystem() ;
		InitializeInputSystem() ;
#endif // DX_NON_INPUT
	}

//	DXST_ERRORLOGFMT_ADDUTF16LE(( L"�m�ۃ�������:%d  �m�ۃ��������T�C�Y:%dByte(%dKByte)", NS_DxGetAllocNum(), NS_DxGetAllocSize(), NS_DxGetAllocSize() / 1024 )) ;

	if( DxSysData.NotDrawFlag == FALSE )
	{
		MATRIX_D ViewMatrix, ProjectionMatrix, ViewportMatrix ;

		// Direct3D9 �ɂ��`�揈���̏�����
		GSYS.Screen.FullScreenResolutionModeAct = GSYS.Screen.FullScreenResolutionMode ;
		Graphics_D3D9_Initialize() ;

		// �O���t�B�b�N�n���h�������� Direct3D9 �I�u�W�F�N�g�̍č쐬
		Graphics_D3D9_CreateObjectAll() ;

#ifndef DX_NON_MASK
		// �}�X�N�T�[�t�F�X�̍č쐬
		Mask_ReCreateSurface() ;
#endif
		// �A�N�e�B�u�O���t�B�b�N�̃A�h���X���ēx�ݒ�
		if( Change == FALSE )
		{
			ViewMatrix       = GSYS.DrawSetting.ViewMatrix ;
			ProjectionMatrix = GSYS.DrawSetting.ProjectionMatrix ;
			ViewportMatrix   = GSYS.DrawSetting.ViewportMatrix ;
		}
		NS_SetRenderTargetToShader( 0, GSYS.DrawSetting.TargetScreen[ 0 ], GSYS.DrawSetting.TargetScreenSurface[ 0 ] ) ;

		// �O���t�B�b�N�֌W�̐ݒ��������
		Graphics_D3D9_Device_ReInitialize() ;

		// ��ʂ̏�����
//		NS_ClearDrawScreen() ;
//		NS_ScreenFlip() ;
//		NS_ClearDrawScreen() ;
//		NS_ScreenFlip() ;

		// �f�o�C�X���X�g�������ɍ폜����t���O�������Ă���O���t�B�b�N���폜����
		Graphics_Image_DeleteDeviceLostDelete() ;

		// �O���t�B�b�N�̕���
/*
		if( Change == TRUE )
		{
			// �O���t�B�b�N�̕���
			DefaultRestoreGraphFunction() ;

#ifndef DX_NON_MODEL
			// ���f���̃e�N�X�`�����ēǂݍ���
			MV1ReloadTexture() ;
#endif
			// �t�H���g�n���h�������L���b�V����������
			InitCacheFontToHandle() ;
		}
		else
*/		{
			NS_RunRestoreShred() ;
		}

		// �O���t�B�b�N�X�f�o�C�X�̃��X�g���畜�A�����Ƃ��ɌĂԊ֐����o�^����Ă�������s����
		if( GD3D9.Device.Setting.DeviceRestoreCallbackFunction )
		{
			GD3D9.Device.Setting.DeviceRestoreCallbackFunction( GD3D9.Device.Setting.DeviceRestoreCallbackData ) ;
		}

		// �n�[�h�E�G�A�̐ݒ�����t���b�V��
		Graphics_D3D9_DeviceState_RefreshRenderState() ;

#ifndef DX_NON_MODEL
		// ���_�o�b�t�@�̃Z�b�g�A�b�v
		MV1SetupVertexBufferAll() ;
#endif

		// ���C�g�̐ݒ�� Direct3D �ɍĔ��f����悤�Ƀt���O���Z�b�g
		GSYS.Light.HardwareRefresh = TRUE ;

		// ���̑��̍Đݒ�
		if( Change == FALSE )
		{
			NS_SetDrawArea( GSYS.DrawSetting.OriginalDrawRect.left, GSYS.DrawSetting.OriginalDrawRect.top, GSYS.DrawSetting.OriginalDrawRect.right, GSYS.DrawSetting.OriginalDrawRect.bottom ) ;
			NS_SetTransformToWorldD( &GSYS.DrawSetting.WorldMatrix );
			NS_SetTransformToViewD( &ViewMatrix );
			NS_SetTransformToProjectionD( &ProjectionMatrix );
			NS_SetTransformToViewportD( &ViewportMatrix );
		}

		// �O�ȊO�̃^�[�Q�b�g�̃Z�b�g�A�b�v
		for( i = 1 ; i < GSYS.HardInfo.RenderTargetNum ; i ++ )
		{
			NS_SetRenderTargetToShader( i, GSYS.DrawSetting.TargetScreen[ i ], GSYS.DrawSetting.TargetScreenSurface[ i ] ) ;
		}
	}

	// �I��
	return 0 ;
}

// �`��p�f�o�C�X���L�����ǂ������擾����( �߂�l  TRUE:�L��  FALSE:���� )
extern	int		Graphics_D3D9_Hardware_CheckValid_PF( void )
{
	return Direct3DDevice9_IsValid() ;
}

// DirectX �̃I�u�W�F�N�g���������
extern	void	Graphics_D3D9_ReleaseDirectXObject_PF( void )
{
#ifndef DX_NON_MASK
	// �}�X�N�T�[�t�F�X���ꎞ�폜
	Mask_ReleaseSurface() ;
#endif
	// �O���t�B�b�N�n���h�������� DirectX �I�u�W�F�N�g�̉��
	Graphics_D3D9_ReleaseObjectAll() ;

	// �V�X�e�������� Direct3D9 �I�u�W�F�N�g�̉��
	Graphics_D3D9_Terminate() ;
}























// ���ˑ��V�X�e���֌W

// WM_ACTIVATE ���b�Z�[�W�̏����ŁA�E�C���h�E���[�h���ɃA�N�e�B�u�ɂȂ����ۂɌĂ΂��֐�
extern	void	Graphics_D3D9_WM_ACTIVATE_ActiveProcess_PF( void )
{
//	if( NS_ScreenFlip() != 0 )
	if( GAPIWin.Direct3DDevice9Object == NULL )
	{
		return ;
	}

	Graphics_D3D9_EndScene() ;
	if( Direct3DDevice9_IsLost() != 0 )
	{
		NS_RestoreGraphSystem() ;
	}
}























// ���ˑ��`��ݒ�֌W

// �V�F�[�_�[�`��ł̕`����ݒ肷��
extern	int		Graphics_Hardware_D3D9_SetRenderTargetToShader_PF( int TargetIndex, int DrawScreen, int SurfaceIndex )
{
	IMAGEDATA *Image ;
	IMAGEDATA *OldImage ;

	// �^�[�Q�b�g���O�ȊO�̏ꍇ�̓V�F�[�_�[���g�p�ł��Ȃ��ꍇ�͉������Ȃ�
	if( GSYS.HardInfo.UseShader == FALSE )
		return -1 ;

	// �f�o�C�X�������ɂȂ��Ă����牽�������I��
	if( GAPIWin.Direct3DDevice9Object == NULL )
		return -1 ;

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �`�揈�����I�����Ă���
	Graphics_D3D9_EndScene() ;

	// �Z�b�g���Ă����e�N�X�`���[���O��
	Graphics_D3D9_DrawSetting_SetTexture( NULL ) ;

	// ���܂ł̕`���̉摜���̎擾
	if( GRAPHCHKFULL( GSYS.DrawSetting.TargetScreen[ TargetIndex ], OldImage ) )
	{
		OldImage = NULL ;
	}
	else
	{
		// ���܂ł̕`��悪�����_�����O�^�[�Q�b�g�T�[�t�F�X�������ꍇ�̓����_�����O�^�[�Q�b�g�e�N�X�`���ɓ]������
		if( OldImage->Hard.Draw[ 0 ].Tex->PF->D3D9.RenderTargetSurface )
		{
			Graphics_D3D9_UpdateDrawTexture( OldImage->Hard.Draw[ 0 ].Tex, GSYS.DrawSetting.TargetScreenSurface[ TargetIndex ] ) ;
		}
	}

	// �摜���ǂ����𔻒�
	if( GRAPHCHKFULL( DrawScreen, Image ) )
	{
		// �摜�ł͂Ȃ��ꍇ�͕`��Ώۂ𖳌��ɂ���
		GSYS.DrawSetting.TargetScreen[ TargetIndex ] = 0 ;
		GSYS.DrawSetting.TargetScreenSurface[ TargetIndex ] = 0 ;
		Graphics_D3D9_DeviceState_SetRenderTarget( NULL, TargetIndex ) ;
		return 0 ;
	}

	// �摜�������ꍇ�͕`��\�ł͖����ꍇ�̓G���[
	if( Image->Orig->FormatDesc.DrawValidFlag == FALSE )
	{
		return -1 ;
	}

	// �`�����Z�b�g
	if( Image->Hard.Draw[ 0 ].Tex->PF->D3D9.RenderTargetSurface )
	{
		Graphics_D3D9_DeviceState_SetRenderTarget( Image->Hard.Draw[ 0 ].Tex->PF->D3D9.RenderTargetSurface, TargetIndex ) ;
	}
	else
	{
		Graphics_D3D9_DeviceState_SetRenderTarget( Image->Hard.Draw[ 0 ].Tex->PF->D3D9.Surface[ SurfaceIndex ], TargetIndex ) ;
	}

	// ����I��
	return 0 ;
}

// SetDrawBright �̈��������
extern	int		Graphics_Hardware_D3D9_SetDrawBrightToOneParam_PF( DWORD /* Bright */ )
{
	// �f�B�t���[�Y�J���[�̍X�V
	GD3D9.Device.DrawInfo.DiffuseColor = GetDiffuseColor() ;

	// ����I��
	return 0 ;
}

// �`��u�����h���[�h���Z�b�g����
extern	int		Graphics_Hardware_D3D9_SetDrawBlendMode_PF( int /*  BlendMode */, int /* BlendParam */ )
{
	// �f�B�t�[�Y�J���[�̍X�V
	GD3D9.Device.DrawInfo.DiffuseColor = GetDiffuseColor() ;

	// ����I��
	return 0 ;
}

// �`�掞�̃A���t�@�e�X�g�̐ݒ���s��( TestMode:DX_CMP_GREATER��( -1:�f�t�H���g����ɖ߂� )  TestParam:�`��A���t�@�l�Ƃ̔�r�Ɏg�p����l )
extern	int		Graphics_Hardware_D3D9_SetDrawAlphaTest_PF( int /* TestMode */, int /* TestParam */ )
{
	// �f�B�t�[�Y�J���[�̍X�V
	GD3D9.Device.DrawInfo.DiffuseColor = GetDiffuseColor() ;

	// ����I��
	return 0 ;
}

// �`�惂�[�h���Z�b�g����
extern	int		Graphics_Hardware_D3D9_SetDrawMode_PF( int DrawMode )
{
	Graphics_D3D9_DeviceState_SetDrawMode( DrawMode ) ;

	// ����I��
	return 0 ;
}

// �`��P�x���Z�b�g
extern	int		Graphics_Hardware_D3D9_SetDrawBright_PF( int /* RedBright */, int /* GreenBright */, int /* BlueBright */ )
{
	// �f�B�t���[�Y�J���[�̍X�V
	GD3D9.Device.DrawInfo.DiffuseColor = GetDiffuseColor() ;

	// ����I��
	return 0 ;
}

// SetBlendGraphParam �̉ϒ������p�����[�^�t��
extern	int		Graphics_Hardware_D3D9_SetBlendGraphParamBase_PF( IMAGEDATA *BlendImage, int BlendType, int *Param )
{
	if( GAPIWin.Direct3DDevice9Object == NULL ) return -1 ;
	
	// �u�����h�摜�� NULL �������̓e�N�X�`���ł͂Ȃ�������u�����h�摜�����̉���
	if( BlendImage == NULL || BlendImage->Orig->FormatDesc.TextureFlag == FALSE )
	{
		GSYS.DrawSetting.BlendGraph = -1 ;
		Graphics_D3D9_DrawSetting_SetBlendTexture( NULL ) ;
	}
	else
	{
		// �u�����h�p�����[�^���Z�b�g����
		Graphics_D3D9_DrawSetting_SetBlendTexture(
			BlendImage->Hard.Draw[ 0 ].Tex[ 0 ].PF->D3D9.Texture,
			BlendImage->Hard.Draw[ 0 ].Tex[ 0 ].TexWidth,
			BlendImage->Hard.Draw[ 0 ].Tex[ 0 ].TexHeight
		) ;
		Graphics_D3D9_DrawSetting_SetBlendTextureParam( BlendType, Param ) ;
	}

	// ����I��
	return 0 ;
}

// �ő�ٕ����̒l���Z�b�g����
extern	int		Graphics_Hardware_D3D9_SetMaxAnisotropy_PF( int MaxAnisotropy )
{
	Graphics_D3D9_DeviceState_SetMaxAnisotropy( MaxAnisotropy ) ;

	// ����I��
	return 0 ;
}

// ���[���h�ϊ��p�s����Z�b�g����
extern	int		Graphics_Hardware_D3D9_SetTransformToWorld_PF( const MATRIX *Matrix )
{
	Graphics_D3D9_DeviceState_SetWorldMatrix( Matrix ) ;

	// ����I��
	return 0 ;
}

// �r���[�ϊ��p�s����Z�b�g����
extern	int		Graphics_Hardware_D3D9_SetTransformToView_PF( const MATRIX *Matrix )
{
	Graphics_D3D9_DeviceState_SetViewMatrix( Matrix ) ;

	// ����I��
	return 0 ;
}

// Direct3D9 �ɐݒ肷��ˉe�s����X�V����
static void Graphics_Hardware_D3D9_RefreshProjectionMatrix( int Update3DProjection, int Update2DProjection, int AlwaysUpdate )
{
	MATRIX *UseProjectionMatrix ;
	MATRIX *UseViewportMatrix ;
	MATRIX TempMatrix ;

//	if( GSYS.DrawSetting.UseProjectionMatrix2D )
//	{
//		if( AlwaysUpdate == FALSE && Update2DProjection == FALSE )
//		{
//			return ;
//		}
//
//		UseProjectionMatrix = &GSYS.DrawSetting.ProjectionMatrix2D ;
//		UseViewportMatrix   = &GSYS.DrawSetting.ViewportMatrix2D ;
//	}
//	else
	{
		if( AlwaysUpdate == FALSE && Update3DProjection == FALSE )
		{
			return ;
		}

		UseProjectionMatrix = &GSYS.DrawSetting.ProjectionMatrixF ;
		UseViewportMatrix   = &GSYS.DrawSetting.ViewportMatrixF ;
	}
	CreateMultiplyMatrix( &TempMatrix, UseProjectionMatrix, UseViewportMatrix                             ) ;
	CreateMultiplyMatrix( &TempMatrix, &TempMatrix,         &GSYS.DrawSetting.Direct3DViewportMatrixAntiF ) ;

	Graphics_D3D9_DeviceState_SetProjectionMatrix( &TempMatrix ) ;
}

// ���e�ϊ��p�s����Z�b�g����
extern	int		Graphics_Hardware_D3D9_SetTransformToProjection_PF( const MATRIX * /* Matrix */ )
{
	Graphics_Hardware_D3D9_RefreshProjectionMatrix( TRUE, FALSE, FALSE ) ;

//	MATRIX TempMatrix ;
//
//	CreateMultiplyMatrix( &TempMatrix, &GSYS.DrawSetting.ProjectionMatrix, &GSYS.DrawSetting.ViewportMatrix             ) ;
//	CreateMultiplyMatrix( &TempMatrix, &TempMatrix,                        &GSYS.DrawSetting.Direct3DViewportMatrixAnti ) ;
//	Graphics_D3D9_DeviceState_SetProjectionMatrix( &TempMatrix ) ;

	// ����I��
	return 0 ;
}

// �r���[�|�[�g�s����Z�b�g����
extern	int		Graphics_Hardware_D3D9_SetTransformToViewport_PF( const MATRIX * /* Matrix */ )
{
	Graphics_Hardware_D3D9_RefreshProjectionMatrix( TRUE, FALSE, FALSE ) ;

//	MATRIX TempMatrix ;
//
//	CreateMultiplyMatrix( &TempMatrix, &GSYS.DrawSetting.ProjectionMatrix, &GSYS.DrawSetting.ViewportMatrix ) ;
//	CreateMultiplyMatrix( &TempMatrix, &TempMatrix,                        &GSYS.DrawSetting.Direct3DViewportMatrixAnti ) ;
//	Graphics_D3D9_DeviceState_SetProjectionMatrix( &TempMatrix ) ;

	// ����I��
	return 0 ;
}

// �e�N�X�`���A�h���X���[�h��ݒ肷��
extern	int		Graphics_Hardware_D3D9_SetTextureAddressMode_PF( int Mode /* DX_TEXADDRESS_WRAP �� */, int Stage )
{
	Graphics_D3D9_DeviceState_SetTextureAddress( Mode, Stage ) ;

	// ����I��
	return 0 ;
}

// �e�N�X�`���A�h���X���[�h��ݒ肷��
extern	int		Graphics_Hardware_D3D9_SetTextureAddressModeUV_PF( int ModeU, int ModeV, int Stage )
{
	Graphics_D3D9_DeviceState_SetTextureAddressU( ModeU, Stage ) ;
	Graphics_D3D9_DeviceState_SetTextureAddressV( ModeV, Stage ) ;

	// ����I��
	return 0 ;
}

// �e�N�X�`�����W�ϊ��s����Z�b�g����
extern	int		Graphics_Hardware_D3D9_SetTextureAddressTransformMatrix_PF( int UseFlag, MATRIX *Matrix, int Sampler )
{
	Graphics_D3D9_DeviceState_SetTextureAddressTransformMatrix( UseFlag, Matrix, Sampler ) ;

	// ����I��
	return 0 ;
}

// �t�H�O��L���ɂ��邩�ǂ�����ݒ肷��( TRUE:�L��  FALSE:���� )
extern	int		Graphics_Hardware_D3D9_SetFogEnable_PF( int Flag )
{
	Graphics_D3D9_DeviceState_SetFogEnable( Flag ) ;

	// ����I��
	return 0 ;
}

// �t�H�O���[�h��ݒ肷��
extern	int		Graphics_Hardware_D3D9_SetFogMode_PF( int Mode /* DX_FOGMODE_NONE �� */ )
{
	Graphics_D3D9_DeviceState_SetFogVertexMode( Mode ) ;

	// ����I��
	return 0 ;
}

// �t�H�O�J���[��ύX����
extern	int		Graphics_Hardware_D3D9_SetFogColor_PF( DWORD FogColor )
{
	Graphics_D3D9_DeviceState_SetFogColor( FogColor ) ;

	// ����I��
	return 0 ;
}

// �t�H�O���n�܂鋗���ƏI�����鋗����ݒ肷��( 0.0f �` 1.0f )
extern	int		Graphics_Hardware_D3D9_SetFogStartEnd_PF( float start, float end )
{
	Graphics_D3D9_DeviceState_SetFogStartEnd( start, end ) ;

	// ����I��
	return 0 ;
}

// �t�H�O�̖��x��ݒ肷��( 0.0f �` 1.0f )
extern	int		Graphics_Hardware_D3D9_SetFogDensity_PF( float density )
{
	Graphics_D3D9_DeviceState_SetFogDensity( density ) ;

	// ����I��
	return 0 ;
}

// ��{�f�[�^�ɐݒ肳��Ă���t�H�O�����n�[�h�E�F�A�ɔ��f����
extern	int		Graphics_Hardware_D3D9_ApplyLigFogToHardware_PF( void )
{
	if( GD3D9.Device.State.FogEnable != GSYS.DrawSetting.FogEnable )
	{
		Graphics_D3D9_DeviceState_SetFogEnable( GSYS.DrawSetting.FogEnable ) ;
	}

	// ����I��
	return 0 ;
}

// �ȑO�� DrawModiGraph �֐��̃R�[�h���g�p���邩�ǂ����̃t���O���Z�b�g����
extern	int		Graphics_Hardware_D3D9_SetUseOldDrawModiGraphCodeFlag_PF( int Flag )
{
	GD3D9.Setting.UseOldDrawModiGraphCodeFlag = Flag ;
	
	// �I��
	return 0 ;
}

// �`���ɐ��������l���������ނ��ǂ����̃t���O���X�V����
extern	int		Graphics_Hardware_D3D9_RefreshAlphaChDrawMode_PF( void )
{
	IMAGEDATA *Image ;
	int i ;
	int NextFlag ;

	NextFlag = FALSE ;

	// �O�ԈȊO�ɂ��`��悪�ݒ肳��Ă����ꍇ�͒ʏ탂�[�h
	for( i = 1 ; i < DX_RENDERTARGET_COUNT ; i ++ )
	{
		if( GSYS.DrawSetting.TargetScreen[ i ] != 0 )
			goto END ;
	}

	// �`��悪�ʏ��ʂ̏ꍇ�͉������Ȃ�
	if( GSYS.DrawSetting.TargetScreen[ 0 ] == DX_SCREEN_BACK ||
		GSYS.DrawSetting.TargetScreen[ 0 ] == DX_SCREEN_FRONT ||
		GSYS.DrawSetting.TargetScreen[ 0 ] == DX_SCREEN_WORK ||
		GSYS.DrawSetting.TargetScreen[ 0 ] == DX_SCREEN_TEMPFRONT )
		goto END ;

	// �`���ƂȂ��Ă���摜�������̏ꍇ�͉������Ȃ�
	if( GRAPHCHKFULL( GSYS.DrawSetting.TargetScreen[ 0 ], Image ) )
		goto END ;

	// �`���̉摜�Ƀ��`�����l���������ꍇ���������Ȃ�
	if( Image->Orig->FormatDesc.AlphaChFlag == FALSE )
		goto END ;
	
	// �e�N�X�`���ł͂Ȃ��ꍇ�͉������Ȃ�
	if( Image->Orig->FormatDesc.TextureFlag == FALSE )
		goto END ;

	// �����ɗ����Ƃ������Ƃ͐��m�ȃ��`�����l���̌v�Z���s���Ƃ�������
	NextFlag = TRUE ;

END :
/*
	// �e�N�X�`���̕ۑ�
	if( NextFlag == TRUE )
	{
		IMAGEDATA *WorkImage ;
		int WorkTextureHandle ;

		WorkTextureHandle = GetWorkTexture( Image->Orig->FormatDesc.FloatTypeFlag, Image->Width, Image->Height, 0 ) ;
		if( GRAPHCHK( WorkTextureHandle, WorkImage ) )
			return -1 ;

		GRH.RenderTargetTexture          = WorkImage->Orig->Hard.Tex[ 0 ].PF->D3D9.Texture ;
		GRH.RenderTargetSurface          =     Image->Orig->Hard.Tex[ 0 ].PF->D3D9.Surface[ 0 ] ;
		GRH.RenderTargetCopySurface      = WorkImage->Orig->Hard.Tex[ 0 ].PF->D3D9.Surface[ 0 ] ;
		GRH.RenderTargetTextureInvWidth  = 1.0f / WorkImage->Orig->Hard.Tex[ 0 ].TexWidth ;
		GRH.RenderTargetTextureInvHeight = 1.0f / WorkImage->Orig->Hard.Tex[ 0 ].TexHeight ;
	}
	else
	{
		GRH.RenderTargetTexture = FALSE ;
	}
*/
	// �ȑO�ƃt���O�̏�Ԃ��قȂ�ꍇ�̂݃u�����h���[�h�̍X�V���s��
	if( NextFlag != GSYS.DrawSetting.AlphaChDrawMode )
	{
		// �t���O�̕ۑ�
		GSYS.DrawSetting.AlphaChDrawMode = NextFlag ;
		GD3D9.Device.DrawSetting.ChangeBlendParamFlag = TRUE ;
		GSYS.ChangeSettingFlag = TRUE ;
	}

	// �I��
	return 0 ;
}
























// ���ˑ��ݒ�֌W

// �n�[�h�E�G�A�̒��_���Z�����@�\���g�p���邩�ǂ�����ݒ肷��
extern	int		Graphics_Hardware_D3D9_SetUseHardwareVertexProcessing_PF( int Flag )
{
	// �������O�̂ݗL��
	if( DxSysData.DxLib_InitializeFlag == TRUE ) return -1 ;

	GD3D9.Setting.NonUseVertexHardwareProcess = !Flag ;

	// ����I��
	return 0 ;
}

// �s�N�Z���P�ʂŃ��C�e�B���O���s�����ǂ�����ݒ肷��A�v ShaderModel 3.0( TRUE:�s�N�Z���P�ʂ̃��C�e�B���O���s��  FALSE:���_�P�ʂ̃��C�e�B���O���s��( �f�t�H���g ) )
extern	int		Graphics_Hardware_D3D9_SetUsePixelLighting_PF( int Flag )
{
	GD3D9.UsePixelLightingShader = Flag ;

	// �I��
	return 0 ;
}

// �O���t�B�b�N�X�f�o�C�X�����X�g���畜�A�����ۂɌĂ΂��R�[���o�b�N�֐���ݒ肷��
extern	int		Graphics_Hardware_D3D9_SetGraphicsDeviceRestoreCallbackFunction_PF( void (* Callback )( void *Data ), void *CallbackData )
{
	GD3D9.Device.Setting.DeviceRestoreCallbackFunction = Callback ;
	GD3D9.Device.Setting.DeviceRestoreCallbackData = CallbackData ;

	// �I��
	return 0 ;
}

// �O���t�B�b�N�X�f�o�C�X�����X�g���畜�A����O�ɌĂ΂��R�[���o�b�N�֐���ݒ肷��
extern	int		Graphics_Hardware_D3D9_SetGraphicsDeviceLostCallbackFunction_PF( void (* Callback )( void *Data ), void *CallbackData )
{
	GD3D9.Device.Setting.DeviceLostCallbackFunction = Callback ;
	GD3D9.Device.Setting.DeviceLostCallbackData = CallbackData ;

	// �I��
	return 0 ;
}

// �ʏ�`��Ƀv���O���}�u���V�F�[�_�[���g�p���邩�ǂ�����ݒ肷��( TRUE:�g�p����( �f�t�H���g )  FALSE:�g�p���Ȃ� )
extern	int		Graphics_Hardware_D3D9_SetUseNormalDrawShader_PF( int Flag )
{
#ifdef DX_NON_NORMAL_DRAW_SHADER
	return -1 ;
#endif // DX_NON_NORMAL_DRAW_SHADER

	Flag = Flag ? FALSE : TRUE ;

	// �������O�̏ꍇ�́A�t���O�����ۑ�����
	if( DxSysData.DxLib_InitializeFlag == FALSE )
	{
		GD3D9.NormalDraw_NotUsePixelShader = Flag ;

		return 0 ;
	}

	// �t���O�������ꍇ�͉������Ȃ�
	if( Flag == GD3D9.NormalDraw_NotUsePixelShader )
	{
		return 0 ;
	}

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �`�����f���o���Ă���
	Graphics_D3D9_EndScene() ;

	// �t���O��ۑ�
	GD3D9.NormalDraw_NotUsePixelShader = Flag ;
	GSYS.ChangeSettingFlag = TRUE ;
	GD3D9.Device.DrawSetting.DrawPrepParamFlag = TRUE ;

	// �ݒ�����t���b�V��
	Graphics_D3D9_DeviceState_RefreshRenderState() ;

	// �I��
	return 0 ;
}

// �r�f�I�������̗e�ʂ𓾂�
extern	int		Graphics_Hardware_D3D9_GetVideoMemorySize_PF( int *AllSize, int *FreeSize )
{
	return DirectDraw7_GetVideoMemorySize( AllSize, FreeSize ) ;
}

// �g�p���̂c�����������R�c�c�����������X�I�u�W�F�N�g�𓾂�
extern	const D_IDirect3DDevice9 * Graphics_Hardware_D3D9_GetUseDirect3DDevice9_PF( void )
{
	return ( D_IDirect3DDevice9* )Direct3DDevice9_GetObject() ;
}

// �g�p���̃o�b�N�o�b�t�@��Direct3DSurface9�I�u�W�F�N�g���擾����
extern	const D_IDirect3DSurface9* Graphics_Hardware_D3D9_GetUseDirect3D9BackBufferSurface_PF( void )
{
	return GD3D9.Device.Screen.BackBufferSurface ;
}

// �c�w���C�u�����̂c�����������R�c�ݒ�����Ȃ���
extern	int		Graphics_Hardware_D3D9_RefreshDxLibDirect3DSetting_PF( void )
{
	if( GAPIWin.Direct3DDevice9Object == NULL ) return -1 ;

	Graphics_D3D9_DeviceState_RefreshRenderState() ;

	// �������t���O�𗧂Ă�
	GD3D9.Device.DrawSetting.CancelSettingEqualCheck = TRUE ;

	// �u�����h���[�h�̍Đݒ�
	Graphics_D3D9_DrawSetting_SetDrawBlendMode( GD3D9.Device.DrawSetting.BlendMode, GD3D9.Device.DrawSetting.AlphaTestValidFlag, GD3D9.Device.DrawSetting.AlphaChannelValidFlag ) ;
	Graphics_D3D9_DrawPreparation() ;

	// �������t���O��|��
	GD3D9.Device.DrawSetting.CancelSettingEqualCheck = FALSE ;

	// �I��
	return 0 ;
}

// �c�����������c���������g�p����f�t�h�c��ݒ肷��
extern	int		Graphics_D3D9_SetDDrawUseGuid_PF( const GUID FAR *Guid )
{
	int i ;

	if( EnumDirect3DAdapter() != 0 ) return -1 ;

	// �w��̂f�t�h�c�����邩�ǂ������ׂ�
	for( i = 0 ; i < GD3D9.Adapter.InfoNum && _MEMCMP( Guid, &GD3D9.Adapter.Info[ i ].DeviceIdentifier, sizeof( GUID ) ) != 0 ; i ++ ){}
	if( i == GD3D9.Adapter.InfoNum ) return -1 ;

	// �l��ۑ�����
	GSYS.Screen.ValidUseDisplayIndex = TRUE ;
	GSYS.Screen.UseDisplayIndex = i ;

	// �I��
	return 0 ;
}

// �L���� DirectDraw �f�o�C�X�� GUID ���擾����
extern	const GUID *Graphics_D3D9_GetDirectDrawDeviceGUID_PF( int Number )
{
	if( EnumDirect3DAdapter() != 0 || Number >= GD3D9.Adapter.InfoNum ) return NULL ;

	return &GD3D9.Adapter.Info[ Number ].DeviceIdentifier ;
}

// �L���� DirectDraw �f�o�C�X�̖��O�𓾂�
extern	int		Graphics_D3D9_GetDirectDrawDeviceDescription_PF( int Number, char *StringBuffer )
{
	if( EnumDirect3DAdapter() != 0 || Number >= GD3D9.Adapter.InfoNum ) return -1 ;

	_STRCPY( StringBuffer, GD3D9.Adapter.Info[ Number ].Description ) ;

	// �I��
	return 0 ;
}

// �L���� DirectDraw �f�o�C�X�̐����擾����
extern	int		Graphics_D3D9_GetDirectDrawDeviceNum_PF( void )
{
	if( EnumDirect3DAdapter() != 0 ) return -1 ;

	return GD3D9.Adapter.InfoNum ;
}

// �g�p���� DirectDraw �f�o�C�X�̃C���f�b�N�X��ݒ肷��
//extern	int		Graphics_D3D9_SetUseDirectDrawDeviceIndex_PF( int Index )
//{
//	// �l��ۑ�����
//	GD3D9.Adapter.ValidUseAdapterNumber = TRUE ;
//	GD3D9.Adapter.UseAdapterNumber = Index ;
//
//	// �I��
//	return 0 ;
//}

// Vista�ȍ~�̊��� Direct3D9Ex ���g�p���邩�ǂ�����ݒ肷��( TRUE:�g�p����( �f�t�H���g )  FALSE:�g�p���Ȃ� )
extern	int		Graphics_D3D9_SetUseDirect3D9Ex_PF( int Flag )
{
	// �������O�̂ݗL��
	if( DxSysData.DxLib_InitializeFlag == TRUE )
	{
		return -1 ;
	}

	NS_SetUseDirectDrawFlag( TRUE ) ;
	NS_SetUseDirect3D11( FALSE ) ;

	// �t���O��ۑ�
	if( Flag )
	{
		GD3D9.Setting.NotUseDirect3D9Ex = FALSE ;
	}
	else
	{
		GD3D9.Setting.NotUseDirect3D9Ex = TRUE ;
	}

	// �I��
	return 0 ;
}

// �_�C�A���O�{�b�N�X���[�h��ύX����
extern	int		Graphics_D3D9_SetDialogBoxMode_PF( int Flag )
{
	HRESULT hr ;

	if( Direct3DDevice9_IsValid() == FALSE )
	{
		return 0 ;
	}

	Graphics_D3D9_EndScene() ;
	hr = Direct3DDevice9_SetDialogBoxMode( Flag ) ;

	return 0 ;
}































// ���ˑ���ʊ֌W

// �ݒ�Ɋ�Â��Ďg�p����y�o�b�t�@���Z�b�g����
extern	int		Graphics_Hardware_D3D9_SetupUseZBuffer_PF( void )
{
	IMAGEDATA *TargetScreenImage = NULL ;
	IMAGEDATA *TargetZBufferImage = NULL ;
	SHADOWMAPDATA *TargetShadowMap = NULL ;

	// �V���h�E�}�b�v���ǂ������`�F�b�N
	if( !SHADOWMAPCHKFULL( GSYS.DrawSetting.TargetScreen[ 0 ], TargetShadowMap ) )
	{
		// �V���h�E�}�b�v�̏ꍇ�͐�p�̂y�o�b�t�@���g�p����
		Direct3DDevice9_SetDepthStencilSurface( TargetShadowMap->PF->D3D9.ZBufferSurface ) ;
	}
	else
	{
		// �`���̉摜���̎擾
		GRAPHCHKFULL( GSYS.DrawSetting.TargetScreen[ 0 ], TargetScreenImage ) ;

		// �g�p����y�o�b�t�@�̉摜���̎擾
		GRAPHCHKFULL( GSYS.DrawSetting.TargetZBuffer, TargetZBufferImage ) ;

		// �}�X�N�T�[�t�F�X�����݂��Ă��Ċ��L���ȏꍇ�̓}�X�N�T�[�t�F�X�̂y�o�b�t�@���g�p����
#ifndef DX_NON_MASK
		if( MASKD.MaskValidFlag && MASKD3D9.MaskScreenSurface )
		{
			Direct3DDevice9_SetDepthStencilSurface( GD3D9.Device.Screen.ZBufferSurface ) ;
		}
		else
#endif
		// �`��\�摜���`��悩�ǂ����ŏ����𕪊�
		if( TargetScreenImage )
		{
			// �`���y�o�b�t�@�������āA���`���ȏ�̃T�C�Y�����ꍇ�͕`���y�o�b�t�@���g�p����
			if( TargetZBufferImage != NULL &&
				TargetZBufferImage->WidthI  >= TargetScreenImage->WidthI &&
				TargetZBufferImage->HeightI >= TargetScreenImage->HeightI )
			{
				Direct3DDevice9_SetDepthStencilSurface( TargetZBufferImage->Hard.Draw[ 0 ].Tex->PF->D3D9.ZBuffer ) ;
			}
			else
			// �`��\�摜�̂y�o�b�t�@������ꍇ�͂��̂y�o�b�t�@���g�p����
			if( TargetScreenImage->Hard.Draw[ 0 ].Tex->PF->D3D9.ZBuffer )
			{
				Direct3DDevice9_SetDepthStencilSurface( TargetScreenImage->Hard.Draw[ 0 ].Tex->PF->D3D9.ZBuffer ) ;
			}
			else
			// �`��悪�f�t�H���g�̂y�o�b�t�@�����傫��������y�o�b�t�@���O��
			if( TargetScreenImage->WidthI  > GD3D9.Device.Screen.ZBufferSizeX || 
				TargetScreenImage->HeightI > GD3D9.Device.Screen.ZBufferSizeY )
			{
				Direct3DDevice9_SetDepthStencilSurface( NULL ) ;
			}
			else
			{
				// �f�t�H���g�̂y�o�b�t�@���g�p����
				Direct3DDevice9_SetDepthStencilSurface( GD3D9.Device.Screen.ZBufferSurface ) ;
			}
		}
		else
		{
			// �`���y�o�b�t�@�������āA���`���ȏ�̃T�C�Y�����ꍇ�͕`���y�o�b�t�@���g�p����
			if( TargetZBufferImage != NULL &&
				TargetZBufferImage->WidthI  >= GSYS.DrawSetting.DrawSizeX &&
				TargetZBufferImage->HeightI >= GSYS.DrawSetting.DrawSizeY )
			{
				Direct3DDevice9_SetDepthStencilSurface( TargetZBufferImage->Hard.Draw[ 0 ].Tex->PF->D3D9.ZBuffer ) ;
			}
			else
			{
				// ����ȊO�̏ꍇ�f�t�H���g�̂y�o�b�t�@���g�p����
				Direct3DDevice9_SetDepthStencilSurface( GD3D9.Device.Screen.ZBufferSurface ) ;
			}
		}
	}

	// �I��
	return 0 ;
}

// ��ʂ̂y�o�b�t�@�̏�Ԃ�����������
extern	int		Graphics_Hardware_D3D9_ClearDrawScreenZBuffer_PF( const RECT *ClearRect )
{
	if( GAPIWin.Direct3DDevice9Object == NULL ) return 0 ;

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �`��̏I��
	Graphics_D3D9_EndScene() ;

	// �y�o�b�t�@�����݂���ꍇ�͂y�o�b�t�@������������
	if( GD3D9.Device.Screen.ZBufferSurface )
	{
		D_D3DVIEWPORT9 Viewport ;
		D_D3DRECT D3DRect ;

		// �r���[�|�[�g���T�[�t�F�X�S�̂ɂ���
		_MEMSET( &Viewport, 0, sizeof( Viewport ) ) ;
		Viewport.X		= 0 ;
		Viewport.Y		= 0 ;
		Viewport.Width	= ( DWORD )GSYS.DrawSetting.DrawSizeX ;
		Viewport.Height	= ( DWORD )GSYS.DrawSetting.DrawSizeY ;
		Viewport.MinZ	= 0.0f ;
		Viewport.MaxZ	= 1.0f ;
		Graphics_D3D9_DeviceState_SetViewport( &Viewport ) ;

		// �N���A
		if( ClearRect )
		{
			D3DRect.x1 = ClearRect->left ;
			D3DRect.y1 = ClearRect->top ;
			D3DRect.x2 = ClearRect->right ;
			D3DRect.y2 = ClearRect->bottom ;
			Direct3DDevice9_Clear( 1, &D3DRect, D_D3DCLEAR_ZBUFFER, 0, 1.0f, 0 ) ;
		}
		else
		{
			Direct3DDevice9_Clear( 0, NULL, D_D3DCLEAR_ZBUFFER, 0, 1.0f, 0 ) ;
		}

		// �r���[�|�[�g�͈̔͂����ɖ߂�
		Viewport.X		= ( DWORD )GSYS.DrawSetting.DrawArea.left ;
		Viewport.Y		= ( DWORD )GSYS.DrawSetting.DrawArea.top ;
		Viewport.Width	= ( DWORD )( GSYS.DrawSetting.DrawArea.right  - GSYS.DrawSetting.DrawArea.left ) ;
		Viewport.Height	= ( DWORD )( GSYS.DrawSetting.DrawArea.bottom - GSYS.DrawSetting.DrawArea.top  ) ;
		Graphics_D3D9_DeviceState_SetViewport( &Viewport ) ;
	}

	// ����I��
	return 0 ;
}

// ��ʂ̏�Ԃ�����������
extern	int		Graphics_Hardware_D3D9_ClearDrawScreen_PF( const RECT *ClearRect )
{
	D_IDirect3DSurface9 *TargetSurface ;
	D_D3DFORMAT          TargetFormat ;
	D_D3DRECT            D3DRect ;
	D_D3DVIEWPORT9       Viewport ;
	DWORD                ClearColor ;
	IMAGEDATA           *Image = NULL ;
	SHADOWMAPDATA       *ShadowMap = NULL ;
//	HRESULT hr ;

	if( GAPIWin.Direct3DDevice9Object == NULL ) return 0 ;

	// �`���摜�̉摜�����擾
	GRAPHCHKFULL( GSYS.DrawSetting.TargetScreen[ 0 ], Image ) ;
	SHADOWMAPCHKFULL( GSYS.DrawSetting.TargetScreen[ 0 ], ShadowMap ) ;

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �`��̏I��
	Graphics_D3D9_EndScene() ;

	// �V���h�E�}�b�v���g�p����Ă���ꍇ�̓V���h�E�}�b�v��������
	if( ShadowMap )
	{
		TargetSurface = ShadowMap->PF->D3D9.Surface ;
		TargetFormat = GD3D9.Device.Caps.TextureFormat[ ShadowMap->ColorFormat ] ;
	}
	else
	// �`��\�摜���g�p����Ă���ꍇ�͕`��\�摜��������
	if( Image )
	{
		TargetSurface = Image->Orig->Hard.Tex[ 0 ].PF->D3D9.Surface[ GSYS.DrawSetting.TargetScreenSurface[ 0 ] ] ;
		TargetFormat = GD3D9.Device.Caps.TextureFormat[ Image->Orig->ColorFormat ] ;
	}
	else
	// �T�u�o�b�N�o�b�t�@���L���ɂȂ��Ă���ꍇ�̓T�u�o�b�N�o�b�t�@��������
	{
		TargetSurface = GD3D9.Device.Screen.SubBackBufferSurface ? GD3D9.Device.Screen.SubBackBufferSurface : GD3D9.Device.Screen.BackBufferSurface ;
		TargetFormat  = GD3D9.Device.Caps.ScreenFormat ;
	}

	// �`��ΏۃT�[�t�F�X��ύX����
#ifndef DX_NON_MASK
	if( MASKD.MaskValidFlag && MASKD3D9.MaskScreenSurface )
	{
		Graphics_D3D9_DeviceState_SetRenderTarget( TargetSurface ) ;
	}
#endif

	// �r���[�|�[�g���T�[�t�F�X�S�̂ɂ���
	_MEMSET( &Viewport, 0, sizeof( Viewport ) ) ;
	Viewport.X		= 0 ;
	Viewport.Y		= 0 ;
	Viewport.Width	= ( DWORD )GSYS.DrawSetting.DrawSizeX ;
	Viewport.Height	= ( DWORD )GSYS.DrawSetting.DrawSizeY ;
	Viewport.MinZ	= 0.0f ;
	Viewport.MaxZ	= 1.0f ;
	Graphics_D3D9_DeviceState_SetViewport( &Viewport ) ;

	// �p�����[�^���Z�b�g
	if( ClearRect )
	{
		D3DRect.x1 = ClearRect->left ;
		D3DRect.y1 = ClearRect->top ;
		D3DRect.x2 = ClearRect->right ;
		D3DRect.y2 = ClearRect->bottom ;
	}

//	Direct3DDevice9_ColorFill( TargetSurface, NULL, NS_GetColor3( Graphics_D3D9_GetD3DFormatColorData( TargetFormat ), GSYS.Screen.BackgroundRed, GSYS.Screen.BackgroundGreen, GSYS.Screen.BackgroundBlue, 0 ) ) ;
//	hr = Direct3DDevice9_ColorFill( TargetSurface, ClearRect, ( GSYS.Screen.BackgroundRed << 16 ) | ( GSYS.Screen.BackgroundGreen << 8 ) | GSYS.Screen.BackgroundBlue ) ;
	ClearColor = ( ( DWORD )GSYS.Screen.BackgroundRed << 16 ) | ( ( DWORD )GSYS.Screen.BackgroundGreen << 8 ) | ( DWORD )GSYS.Screen.BackgroundBlue ;
	if( ClearRect )
	{
		Direct3DDevice9_Clear( 1, &D3DRect, D_D3DCLEAR_TARGET, ClearColor, 1.0f, 0 ) ;
	}
	else
	{
		Direct3DDevice9_Clear( 0, NULL, D_D3DCLEAR_TARGET, ClearColor, 1.0f, 0 ) ;
	}

	// �y�o�b�t�@�����݂���ꍇ�͂y�o�b�t�@������������
	if( GD3D9.Device.Screen.ZBufferSurface )
	{
		// �N���A
		if( ClearRect )
		{
			Direct3DDevice9_Clear( 1, &D3DRect, D_D3DCLEAR_ZBUFFER, 0, 1.0f, 0 ) ;
		}
		else
		{
			Direct3DDevice9_Clear( 0, NULL, D_D3DCLEAR_ZBUFFER, 0, 1.0f, 0 ) ;
		}
	}

	// �`��ΏۃT�[�t�F�X�����ɖ߂�
#ifndef DX_NON_MASK
	if( MASKD.MaskValidFlag && MASKD3D9.MaskScreenSurface )
	{
		Graphics_D3D9_DeviceState_SetRenderTarget( MASKD3D9.MaskScreenSurface ) ;
	}
#endif

	// �r���[�|�[�g�͈̔͂����ɖ߂�
	Viewport.X		= ( DWORD )GSYS.DrawSetting.DrawArea.left ;
	Viewport.Y		= ( DWORD )GSYS.DrawSetting.DrawArea.top ;
	Viewport.Width	= ( DWORD )( GSYS.DrawSetting.DrawArea.right  - GSYS.DrawSetting.DrawArea.left ) ;
	Viewport.Height	= ( DWORD )( GSYS.DrawSetting.DrawArea.bottom - GSYS.DrawSetting.DrawArea.top  ) ;
	Graphics_D3D9_DeviceState_SetViewport( &Viewport ) ;

	// ����I��
	return 0 ;
}

// �`����ʂ̃Z�b�g
extern	int		Graphics_Hardware_D3D9_SetDrawScreen_PF( int /* DrawScreen */, int OldScreenSurface, IMAGEDATA *NewTargetImage, IMAGEDATA *OldTargetImage, SHADOWMAPDATA *NewTargetShadowMap, SHADOWMAPDATA * /* OldTargetShadowMap */ )
{
	if( GAPIWin.Direct3DDevice9Object == NULL )
	{
		return -1 ;
	}

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �`�揈�����I�����Ă���
	Graphics_D3D9_EndScene() ;

	// �Z�b�g���Ă����e�N�X�`���[���O��
	Graphics_D3D9_DrawSetting_SetTexture( NULL ) ;

	// ���܂ł̕`��悪�����_�����O�^�[�Q�b�g�T�[�t�F�X�������ꍇ�̓����_�����O�^�[�Q�b�g�e�N�X�`���ɓ]������
	if( OldTargetImage != NULL && OldTargetImage->Hard.Draw[ 0 ].Tex->PF->D3D9.RenderTargetSurface )
	{
		Graphics_D3D9_UpdateDrawTexture( OldTargetImage->Hard.Draw[ 0 ].Tex, OldScreenSurface ) ;
	}

	// �`���̕ύX

	// �}�X�N�T�[�t�F�X�����݂��Ă��Ċ��L���ȏꍇ�̓}�X�N�T�[�t�F�X��`��Ώ̂ɂ���
#ifndef DX_NON_MASK
	if( MASKD.MaskValidFlag && MASKD3D9.MaskScreenSurface )
	{
		Graphics_D3D9_DeviceState_SetRenderTarget( MASKD3D9.MaskScreenSurface ) ;
	}
	else
#endif
	// �V���h�E�}�b�v���L���ȏꍇ�̓V���h�E�}�b�v��`��Ώۂɂ���
	if( NewTargetShadowMap )
	{
		Graphics_D3D9_DeviceState_SetRenderTarget( NewTargetShadowMap->PF->D3D9.Surface ) ;
	}
	else
	// �`��\�摜���L���ȏꍇ�͕`��\�摜��`��Ώۂɂ���
	if( NewTargetImage )
	{
		if( NewTargetImage->Hard.Draw[ 0 ].Tex->PF->D3D9.RenderTargetSurface )
		{
			Graphics_D3D9_DeviceState_SetRenderTarget( NewTargetImage->Hard.Draw[ 0 ].Tex->PF->D3D9.RenderTargetSurface ) ;
		}
		else
		{
			Graphics_D3D9_DeviceState_SetRenderTarget( NewTargetImage->Hard.Draw[ 0 ].Tex->PF->D3D9.Surface[ GSYS.DrawSetting.TargetScreenSurface[ 0 ] ] ) ;
		}
	}
	else
	{
		// ����ȊO�̏ꍇ�̓T�u�o�b�N�o�b�t�@���L���ȏꍇ�̓T�u�o�b�N�o�b�t�@��`��Ώ̂ɂ���
		Graphics_D3D9_DeviceState_SetRenderTarget( GD3D9.Device.Screen.SubBackBufferSurface ? GD3D9.Device.Screen.SubBackBufferSurface : GD3D9.Device.Screen.BackBufferSurface ) ;
	}

	// �g�p����y�o�b�t�@�̃Z�b�g�A�b�v
	Graphics_Screen_SetupUseZBuffer() ;

	// ����I��
	return 0 ;
}

// SetDrawScreen �̍Ō�ŌĂ΂��֐�
extern	int		Graphics_Hardware_D3D9_SetDrawScreen_Post_PF( int DrawScreen )
{
	// �`��悪�\��ʂ����������I�ɉ�ʃR�s�[���s��
	Direct3DDevice9_SetupTimerPresent( DrawScreen == DX_SCREEN_FRONT ) ;

	// ����I��
	return 0 ;
}

// �`��\�̈�̃Z�b�g
extern	int		Graphics_Hardware_D3D9_SetDrawArea_PF( int /* x1 */, int /* y1 */, int /* x2 */, int /* y2 */ )
{
	D_D3DVIEWPORT9 Viewport ;
//	MATRIX TempMatrix ;

	if( GAPIWin.Direct3DDevice9Object == NULL )	return -1 ;

	if( GSYS.DrawSetting.DrawArea.right  - GSYS.DrawSetting.DrawArea.left == 0 ||
		GSYS.DrawSetting.DrawArea.bottom - GSYS.DrawSetting.DrawArea.top  == 0 )
		return -1 ;

	// �r���[�|�[�g�̃Z�b�e�B���O
	_MEMSET( &Viewport, 0, sizeof( Viewport ) ) ;
	Viewport.X		= ( DWORD )GSYS.DrawSetting.DrawArea.left ;
	Viewport.Y		= ( DWORD )GSYS.DrawSetting.DrawArea.top ;
	Viewport.Width	= ( DWORD )( GSYS.DrawSetting.DrawArea.right  - GSYS.DrawSetting.DrawArea.left ) ;
	Viewport.Height	= ( DWORD )( GSYS.DrawSetting.DrawArea.bottom - GSYS.DrawSetting.DrawArea.top  ) ;
	Viewport.MinZ	= 0.0f ;
	Viewport.MaxZ	= 1.0f ;

	// �Z�b�g
	Graphics_D3D9_DeviceState_SetViewport( &Viewport ) ;
	Graphics_D3D9_DeviceState_SetScissorRect( &GSYS.DrawSetting.DrawArea ) ;

	// �ˉe�s��ƃr���[�|�[�g�s��̋t�s�����Z�������̂��Z�b�g
//	CreateMultiplyMatrix( &TempMatrix, &GSYS.DrawSetting.ProjectionMatrix, &GSYS.DrawSetting.ViewportMatrix             ) ;
//	CreateMultiplyMatrix( &TempMatrix, &TempMatrix,                        &GSYS.DrawSetting.Direct3DViewportMatrixAnti ) ;
//	Graphics_D3D9_DeviceState_SetProjectionMatrix( &TempMatrix ) ;
	Graphics_Hardware_D3D9_RefreshProjectionMatrix( FALSE, FALSE, TRUE ) ;
	GSYS.DrawSetting.MatchHardwareProjectionMatrix = TRUE ;

	// ����I��
	return 0 ;
}

// �`���o�b�t�@�����b�N����
extern	int		Graphics_Hardware_D3D9_LockDrawScreenBuffer_PF( RECT *LockRect, BASEIMAGE *BaseImage, int /* TargetScreen */, IMAGEDATA *TargetImage, int TargetScreenSurface, int ReadOnly, int TargetScreenTextureNo )
{
	D_IDirect3DSurface9 *TargetSurface ;
	D_D3DFORMAT          TargetFormat ;
	COLORDATA           *TargetColorData ;
	int                  Width ;
	int                  Height ;
	int                  LockEnable ;
	int                  UseSysMemSurfaceIndex = -1 ;
	D_D3DLOCKED_RECT     SrcLockRect ;
	RECT                 DestRect ;
	HRESULT              hr ;

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �`�揈�����������Ă���
	Graphics_D3D9_EndScene() ;

	Width  = LockRect->right  - LockRect->left ;
	Height = LockRect->bottom - LockRect->top ;

	// ��荞�݌��ƂȂ�T�[�t�F�X�̌���

	// �`��\�摜���Ώۂ̏ꍇ
	if( TargetImage )
	{
		TargetSurface = TargetImage->Orig->Hard.Tex[ TargetScreenTextureNo ].PF->D3D9.RenderTargetSurface ?
						TargetImage->Orig->Hard.Tex[ TargetScreenTextureNo ].PF->D3D9.RenderTargetSurface :
						TargetImage->Orig->Hard.Tex[ TargetScreenTextureNo ].PF->D3D9.Surface[ TargetScreenSurface ] ;
		TargetFormat  = GD3D9.Device.Caps.TextureFormat[ TargetImage->Orig->ColorFormat ] ;
		LockEnable    = FALSE ;
	}
	else
	// ����ȊO�̏ꍇ�̓T�u�o�b�N�o�b�t�@���L���������炻�ꂪ�Ώ�
	{
		TargetSurface = GD3D9.Device.Screen.SubBackBufferSurface ? GD3D9.Device.Screen.SubBackBufferSurface : GD3D9.Device.Screen.BackBufferSurface ;
		TargetFormat  = GD3D9.Device.Caps.ScreenFormat ;
		LockEnable    = GD3D9.Device.Caps.UseRenderTargetLock ;
	}
	TargetColorData = Graphics_D3D9_GetD3DFormatColorData( TargetFormat ) ;

	// ���b�N�ł���ꍇ�͂����Ȃ胍�b�N
	if( LockEnable && 
		Direct3DSurface9_LockRect( TargetSurface, &SrcLockRect, LockRect, ( DWORD )( ReadOnly ? D_D3DLOCK_READONLY : 0 ) ) == D_D3D_OK )
	{
		// ���b�N�����T�[�t�F�X�̃|�C���^��ۑ�
		GD3D9.Device.Screen.DrawScreenBufferLockSurface = TargetSurface ;

		// �ꎞ�I�Ɏg�p���Ă���T�[�t�F�X�̃|�C���^�����Z�b�g
		GD3D9.Device.Screen.DrawScreenBufferLockSMSurface = NULL ;
	}
	else
	// �ʖڂ������ꍇ�͈�x�������Ɏ�荞��ł��烍�b�N
	{
		D_IDirect3DSurface9 *RTSurface ;
		D_IDirect3DSurface9 *SMSurface ;

		// ��荞�݃T�C�Y�Ɠ����傫���̕`��\�T�[�t�F�X�̍쐬
		RTSurface = NULL ;
		if( Direct3DDevice9_CreateRenderTarget(
				( UINT )Width, ( UINT )Height, TargetFormat, D_D3DMULTISAMPLE_NONE, 0, FALSE, &RTSurface, NULL ) != D_D3D_OK )
		{
			DXST_ERRORLOG_ADDUTF16LE( "\xd0\x30\xc3\x30\xaf\x30\xd0\x30\xc3\x30\xd5\x30\xa1\x30\xed\x30\xc3\x30\xaf\x30\x28\x75\xcf\x63\x3b\x75\xef\x53\xfd\x80\xb5\x30\xfc\x30\xd5\x30\xa7\x30\xb9\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�o�b�N�o�b�t�@���b�N�p�`��\�T�[�t�F�X�̍쐬�Ɏ��s���܂���\n" @*/ ) ;
			return -1 ;
		}

		// ��荞�݃T�C�Y�Ɠ����傫���̃V�X�e����������̃I�t�X�N���[���T�[�t�F�X�̍쐬
		SMSurface = NULL ;
/*
		if( Direct3DDevice9_CreateOffscreenPlainSurface(
				Width, Height, TargetFormat, D_D3DPOOL_SYSTEMMEM, &SMSurface, NULL ) != D_D3D_OK )
		{
			DXST_ERRORLOG_ADDUTF16LE( L"�o�b�N�o�b�t�@���b�N�p�V�X�e���������T�[�t�F�X�̍쐬�Ɏ��s���܂���\n" ) ;
			Direct3D9_ObjectRelease( RTSurface ) ;
			return -1 ;
		}
*/
		for(;;)
		{
			UseSysMemSurfaceIndex = Graphics_D3D9_GetSysMemSurface( Width, Height, TargetFormat, FALSE ) ;
			if( UseSysMemSurfaceIndex != -1 ) break ;
			if( NS_ProcessMessage() != 0 ) break ;
			Thread_Sleep( 1 ) ;
		}
		if( UseSysMemSurfaceIndex == -1 )
		{
			DXST_ERRORLOG_ADDUTF16LE( "\xd0\x30\xc3\x30\xaf\x30\xd0\x30\xc3\x30\xd5\x30\xa1\x30\xed\x30\xc3\x30\xaf\x30\x28\x75\xb7\x30\xb9\x30\xc6\x30\xe0\x30\xe1\x30\xe2\x30\xea\x30\xb5\x30\xfc\x30\xd5\x30\xa7\x30\xb9\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�o�b�N�o�b�t�@���b�N�p�V�X�e���������T�[�t�F�X�̍쐬�Ɏ��s���܂���\n" @*/ ) ;
			Direct3D9_ObjectRelease( RTSurface ) ;
			return -1 ;
		}
		SMSurface = GD3D9.SysMemTexSurf.Surface[ UseSysMemSurfaceIndex ].MemSurface ;

		// �܂��`��\�T�[�t�F�X�ɓ��e��]������
		DestRect.left   = 0 ;
		DestRect.top    = 0 ;
		DestRect.right  = Width ;
		DestRect.bottom = Height ;
		hr = Direct3DDevice9_StretchRect(
			TargetSurface, LockRect,
			RTSurface, &DestRect, D_D3DTEXF_NONE ) ;
		if( hr != D_D3D_OK )
		{
			// �ꎞ�I�ɍ쐬�����T�[�t�F�X�̉��
			Direct3D9_ObjectRelease( RTSurface ) ;

			// �ꎞ�I�ɍ쐬�����T�[�t�F�X�̉��
			Graphics_D3D9_ReleaseSysMemSurface( UseSysMemSurfaceIndex, FALSE ) ;

			// �G���[�I��
			return -1 ;
		}

		// ���̌�V�X�e���������T�[�t�F�X�ɓ��e��]������
		hr = Direct3DDevice9_GetRenderTargetData(
			RTSurface, SMSurface ) ;

		// �V�X�e���������T�[�t�F�X�����b�N
		hr = Direct3DSurface9_LockRect( SMSurface, &SrcLockRect, NULL, ( DWORD )( ReadOnly ? D_D3DLOCK_READONLY : 0 ) ) ;

		// �ꎞ�I�Ɏg�p���Ă���T�[�t�F�X�̃|�C���^��ۑ�
		GD3D9.Device.Screen.DrawScreenBufferLockSMSurface = SMSurface ;

		// �ꎞ�I�Ɏg�p���Ă���T�[�t�F�X�L���b�V���̃C���f�b�N�X��ۑ�
		GD3D9.Device.Screen.DrawScreenBufferLockSMSurfaceIndex = UseSysMemSurfaceIndex ;

		// �ꎞ�I�ɍ쐬�����T�[�t�F�X�̉��
		Direct3D9_ObjectRelease( RTSurface ) ;
	}

	// ��{�C���[�W�\���̂̃f�[�^�ɃZ�b�g
	BaseImage->ColorData     = *TargetColorData ;
	BaseImage->Width          = Width ;
	BaseImage->Height         = Height ;
	BaseImage->Pitch          = SrcLockRect.Pitch ;
	BaseImage->GraphData      = SrcLockRect.pBits ;
	BaseImage->MipMapCount    = 0 ;
	BaseImage->GraphDataCount = 0 ;

	// ����I��
	return 0 ;
}

// �`���o�b�t�@���A�����b�N����
extern	int		Graphics_Hardware_D3D9_UnlockDrawScreenBuffer_PF( void )
{
	// �ꎞ�I�Ɏg�p���Ă���T�[�t�F�X�����b�N�������ǂ����ŏ����𕪊�
	if( GD3D9.Device.Screen.DrawScreenBufferLockSMSurface != NULL )
	{
		// ���b�N�̉���
		Direct3DSurface9_UnlockRect( GD3D9.Device.Screen.DrawScreenBufferLockSMSurface ) ;

		// �ꎞ�I�ɍ쐬�����T�[�t�F�X�̉��
		// Direct3D9_ObjectRelease( GD3D9.Device.Screen.DrawScreenBufferLockSMSurface ) ;
		Graphics_D3D9_ReleaseSysMemSurface( GD3D9.Device.Screen.DrawScreenBufferLockSMSurfaceIndex, FALSE ) ;

		// �T�[�t�F�X�L���b�V���C���f�b�N�X�𖳌���
		GD3D9.Device.Screen.DrawScreenBufferLockSMSurfaceIndex = -1 ;

		// �|�C���^�𖳌���
		GD3D9.Device.Screen.DrawScreenBufferLockSMSurface = NULL ;
	}
	else
	{
		// �`��Ώۂ̃T�[�t�F�X�̃��b�N������
		Direct3DSurface9_UnlockRect( GD3D9.Device.Screen.DrawScreenBufferLockSurface ) ;
		GD3D9.Device.Screen.DrawScreenBufferLockSurface = NULL ;
	}

	// ����I��
	return 0 ;
}

// ����ʂ̓��e��\��ʂɕ`�悷��
extern	int		Graphics_Hardware_D3D9_ScreenCopy_PF( void )
{
	// �T�u�o�b�N�o�b�t�@�̃Z�b�g�A�b�v
	if( NS_GetWindowModeFlag() == FALSE )
	{
		Graphics_D3D9_SetupSubBackBuffer() ;
	}

	// ����I��
	return 0 ;
}

// ���������M����҂�
extern	int		Graphics_Hardware_D3D9_WaitVSync_PF( int SyncNum )
{
	return DirectDraw7_WaitVSync( SyncNum ) ;
}

// ����ʂƕ\��ʂ���������
extern	int		Graphics_Hardware_D3D9_ScreenFlipBase_PF( void )
{
	// ����ȏ����ł̓t���b�v���L�����Z������
	if( WinData.ActiveFlag == FALSE && 
		WinData.DrawBackGraphFlag == FALSE && 
		WinData.PauseGraph.GraphData != NULL )
	{
		return 0 ;
	}

	// �t���X�N���[�����[�h��
	// �t���X�N���[���𑜓x���[�h�� DX_FSRESOLUTIONMODE_NATIVE �ł͂Ȃ��ꍇ��
	// GSYS.Screen/ValidGraphDisplayArea �� TRUE �̏ꍇ�̓T�u�o�b�N�o�b�t�@�̃Z�b�g�A�b�v
	if( NS_GetWindowModeFlag() == FALSE &&
		( GSYS.Screen.FullScreenResolutionModeAct != DX_FSRESOLUTIONMODE_NATIVE ||
		  GSYS.Screen.ValidGraphDisplayArea != FALSE ) )
	{
		Graphics_D3D9_SetupSubBackBuffer() ;
	}

	// ����ʂ̓��e��\��ʂɔ��f
	return Direct3DDevice9_Present() ;
}

// ����ʂ̎w��̗̈���E�C���h�E�̃N���C�A���g�̈�̎w��̗̈�ɓ]������
extern	int		Graphics_Hardware_D3D9_BltRectBackScreenToWindow_PF( HWND Window, RECT BackScreenRect, RECT WindowClientRect )
{
	return Direct3DDevice9_BltRectBackScreenToWindow( Window, BackScreenRect, WindowClientRect ) ;
}

// ScreenFlip �ŉ摜��]�������̃E�C���h�E��ݒ肷��( NULL ���w�肷��Ɛݒ���� )
extern	int		Graphics_Hardware_D3D9_SetScreenFlipTargetWindow_PF( HWND TargetWindow )
{
	return 0 ;
}

// ���C����ʂ̂y�o�b�t�@�̐ݒ��ύX����
extern	int		Graphics_Hardware_D3D9_SetZBufferMode_PF( int ZBufferSizeX, int ZBufferSizeY, int ZBufferBitDepth )
{
	HRESULT hr ;
	D_IDirect3DSurface9 *SetZBuffer ;
	int UseScreenZBuffer = FALSE ;
	int BitIndex ;

	// �l�̃`�F�b�N�ƕۑ�
	if( ZBufferSizeX > 0 && ZBufferSizeY > 0 )
	{
		for( GD3D9.Device.Screen.ZBufferSizeX = 1 ; GD3D9.Device.Screen.ZBufferSizeX < ZBufferSizeX ; GD3D9.Device.Screen.ZBufferSizeX <<= 1 ){}
		for( GD3D9.Device.Screen.ZBufferSizeY = 1 ; GD3D9.Device.Screen.ZBufferSizeY < ZBufferSizeY ; GD3D9.Device.Screen.ZBufferSizeY <<= 1 ){}
		GD3D9.Setting.UserZBufferSizeSet = TRUE ;
	}
	if( ZBufferBitDepth > 0 )
	{
		if( ZBufferBitDepth != 16 && ZBufferBitDepth != 24 && ZBufferBitDepth != 32 )
		{
			return -1 ;
		}
		GD3D9.Device.Screen.ZBufferBitDepth = ZBufferBitDepth ;
		GD3D9.Setting.UserZBufferBitDepthSet = TRUE ;
	}

	// �������O�̏ꍇ�͒l�����ݒ肵�ďI��
	if( GAPIWin.Direct3DDevice9Object == NULL )
		return 0 ;

	// ���ݐݒ肳��Ă���y�o�b�t�@���擾
	Direct3DDevice9_GetDepthStencilSurface( &SetZBuffer ) ;

	// ������ʗp�y�o�b�t�@���g�p����Ă���ꍇ�͈�U�O��
	if( SetZBuffer == GD3D9.Device.Screen.ZBufferSurface )
	{
		Direct3DDevice9_SetDepthStencilSurface( NULL ) ;
		UseScreenZBuffer = TRUE ;
	}
	Direct3D9_ObjectRelease( SetZBuffer ) ;
	SetZBuffer = NULL ;

	// Z�o�b�t�@���J��
	if( GD3D9.Device.Screen.ZBufferSurface )
	{
		Direct3D9_ObjectRelease( GD3D9.Device.Screen.ZBufferSurface ) ;
		GD3D9.Device.Screen.ZBufferSurface = NULL ;
	}

	// �V���Ȃy�o�b�t�@�̍쐬
	switch( GD3D9.Device.Screen.ZBufferBitDepth )
	{
	default:
	case 16 : BitIndex = 0 ; break ;
	case 24 : BitIndex = 1 ; break ;
	case 32 : BitIndex = 2 ; break ;
	}
	hr = Direct3DDevice9_CreateDepthStencilSurface(
		( UINT )GD3D9.Device.Screen.ZBufferSizeX,
		( UINT )GD3D9.Device.Screen.ZBufferSizeY,
		GD3D9.Device.Caps.ZBufferFormat[ BitIndex ],
		( D_D3DMULTISAMPLE_TYPE )GSYS.Setting.FSAAMultiSampleCount,
		( DWORD )GSYS.Setting.FSAAMultiSampleQuality,
		TRUE,
		&GD3D9.Device.Screen.ZBufferSurface,
		NULL
	) ;
	if( FAILED( hr ) )
	{
		DXST_ERRORLOG_ADDUTF16LE( "\x3a\xff\xd0\x30\xc3\x30\xd5\x30\xa1\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�y�o�b�t�@�̍쐬�Ɏ��s���܂���\n" @*/ ) ;
		return -1 ;
	}

	// ������ʗp�y�o�b�t�@���g�p����Ă���ꍇ�͍ăZ�b�g
	if( UseScreenZBuffer )
	{
		Direct3DDevice9_SetDepthStencilSurface( GD3D9.Device.Screen.ZBufferSurface ) ;

		// �N���A
		Direct3DDevice9_Clear( 0, NULL, D_D3DCLEAR_ZBUFFER, 0, 1.0f, 0 ) ;
	}

	// ����I��
	return 0 ;
}

// �`���y�o�b�t�@�̃Z�b�g
extern	int		Graphics_Hardware_D3D9_SetDrawZBuffer_PF( int /* DrawScreen */, IMAGEDATA *Image )
{
	// �摜���ǂ����ŏ����𕪊�
	if( Image != NULL )
	{
		// �摜�������ꍇ�͕`��\�ł͖����ꍇ��y�o�b�t�@�������Ă��Ȃ��摜�̏ꍇ�̓G���[
		if( Image->Orig->FormatDesc.DrawValidFlag == FALSE ||
			Image->Hard.Draw[ 0 ].Tex->PF->D3D9.ZBuffer == NULL )
			return -1 ;
	}

	// ����I��
	return 0 ;
}

























// ���ˑ����擾�֌W

// GetColor �� GetColor2 �Ŏg�p����J���[�f�[�^���擾����
extern const COLORDATA *Graphics_Hardware_D3D9_GetMainColorData_PF( void )
{
	return Graphics_D3D9_GetD3DFormatColorData( GSYS.Screen.MainScreenColorBitDepth == 16 ? D_D3DFMT_R5G6B5 : D_D3DFMT_X8R8G8B8 ) ;
}

// �f�B�X�v���[�̃J���[�f�[�^�|�C���^�𓾂�
extern	const COLORDATA *Graphics_Hardware_D3D9_GetDispColorData_PF( void )
{
	return Graphics_D3D9_GetD3DFormatColorData( GD3D9.Device.Caps.ScreenFormat ) ;
}

// �w����W�̐F���擾����
extern	DWORD Graphics_Hardware_D3D9_GetPixel_PF( int x, int y )
{
	RECT      SrcRect ;
//	COLORDATA *DestColorData ;
	BASEIMAGE BufferImage ;
	DWORD     Ret = 0xffffffff ;
	int       Red, Green, Blue, Alpha ;

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �g���Ă���ꍇ
	Graphics_D3D9_EndScene();

	// �`���o�b�t�@�����b�N
	SrcRect.left   = x ;
	SrcRect.right  = x + 1 ;
	SrcRect.top    = y ;
	SrcRect.bottom = y + 1 ;
	if( Graphics_Screen_LockDrawScreen( &SrcRect, &BufferImage, -1, -1, TRUE, 0 ) < 0 )
		return Ret ;

	switch( BufferImage.ColorData.ColorBitDepth )
	{
	case 16 :
		NS_GetColor5( &BufferImage.ColorData, *( ( WORD   * )BufferImage.GraphData ), &Red, &Green, &Blue, &Alpha ) ;
		break ;

	case 32 :
		NS_GetColor5( &BufferImage.ColorData, *( ( DWORD  * )BufferImage.GraphData ), &Red, &Green, &Blue, &Alpha ) ;
		break ;
	}

	Ret = 0xff000000 | ( ( ( unsigned int )Red ) << 16 ) | ( ( ( unsigned int )Green ) << 8 ) | ( ( unsigned int )Blue ) ;

//	DestColorData = Graphics_D3D9_GetD3DFormatColorData( GD3D9.Device.Caps.ScreenFormat ) ;
//	switch( BufferImage.ColorData.ColorBitDepth )
//	{
//	case 16 : Ret = NS_GetColor4( DestColorData, &BufferImage.ColorData, *( ( WORD  * )BufferImage.GraphData ) ) & ~DestColorData->NoneMask ; break ;
//	case 32 : Ret = NS_GetColor4( DestColorData, &BufferImage.ColorData, *( ( DWORD * )BufferImage.GraphData ) ) & ~DestColorData->NoneMask ; break ;
//	}

	// �`���o�b�t�@���A�����b�N
	Graphics_Screen_UnlockDrawScreen() ;

	// �F��Ԃ�
	return Ret ;
}


























// ���ˑ��摜�֌W

// YUV�T�[�t�F�X���g���� Theora ����̓��e���O���t�B�b�N�X�n���h���̃e�N�X�`���ɓ]������
extern	int		Graphics_Hardware_D3D9_UpdateGraphMovie_TheoraYUV_PF( MOVIEGRAPH *Movie, IMAGEDATA *Image )
{
#ifndef DX_NON_OGGTHEORA
	RECT                 Rect ;
	D_IDirect3DSurface9 *Surface ;
	D_IDirect3DSurface9 *YUY2Image ;

	if( GAPIWin.Direct3DDevice9Object == NULL )
	{
		return -1 ;
	}

	YUY2Image = ( D_IDirect3DSurface9 * )TheoraDecode_GetYUVImage( Movie->TheoraHandle ) ;

	// �r�b�g�}�b�v�f�[�^�𕪊�����Ă���e�N�X�`���[�ɓ\��t����
	SETRECT( Rect, 0, 0, Image->Orig->Hard.Tex[ 0 ].UseWidth, Image->Orig->Hard.Tex[ 0 ].UseHeight ) ;

	// �O���t�B�b�N�̓]��
	if( Direct3DTexture9_GetSurfaceLevel( Image->Orig->Hard.Tex[ 0 ].PF->D3D9.Texture, 0, &Surface ) == D_D3D_OK )
	{
		HRESULT hr ;
		hr = Direct3DDevice9_StretchRect( YUY2Image, &Rect, Surface, &Rect, D_D3DTEXF_NONE ) ;
		Direct3D9_ObjectRelease( Surface ) ;
	}
#endif

	// ����I��
	return 0 ;
}

// �O���t�B�b�N�������̈�̃��b�N
extern	int		Graphics_Hardware_D3D9_GraphLock_PF( IMAGEDATA *Image, COLORDATA **ColorDataP, int WriteOnly )
{
	IMAGEDATA_HARD_DRAW *DrawTex ;
	COLORDATA           *ColorData ;
	BYTE                *Dest ;
	BYTE                *Src ;
	DWORD                WidthByte ;
	HRESULT              hr ;
	D_D3DLOCKED_RECT     LockRect ;
	D_IDirect3DSurface9 *SMSurface ;
	int                  i ;
	RECT                 Rect ;

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �`����I�����Ă���
	Graphics_D3D9_EndScene() ;

	// �e�N�X�`���̃t�H�[�}�b�g���擾
	ColorData = Graphics_D3D9_GetD3DFormatColorData( GD3D9.Device.Caps.TextureFormat[ Image->Orig->ColorFormat ] ) ;

	// ���b�N���̃e���|�����C���[�W���i�[���邽�߂̃��������m��
	Image->LockImagePitch = ( DWORD )( Image->WidthI * ColorData->PixelByte ) ;
	Image->LockImage = ( BYTE * )DXALLOC( Image->HeightI * Image->LockImagePitch ) ;
	if( Image->LockImage == NULL )
	{
		DXST_ERRORLOG_ADDUTF16LE( "\xed\x30\xc3\x30\xaf\x30\x28\x75\xc6\x30\xf3\x30\xdd\x30\xe9\x30\xea\x30\xa4\x30\xe1\x30\xfc\x30\xb8\x30\x3c\x68\x0d\x7d\x28\x75\xe1\x30\xe2\x30\xea\x30\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"���b�N�p�e���|�����C���[�W�i�[�p�������̊m�ۂɎ��s���܂���\n" @*/ ) ;
		return -1 ;
	}

	// �������ݐ�p�ł͂Ȃ��ꍇ�̓e���|�����o�b�t�@�Ƀf�[�^��]������
	if( WriteOnly == FALSE )
	{
		// �`����̐������J��Ԃ�
		DrawTex = Image->Hard.Draw ;
		SMSurface = NULL ;
		for( i = 0 ; i < Image->Hard.DrawNum ; i ++, DrawTex ++ )
		{
			// Direct3D9Ex ���g�p���Ă��邩�A�`��\�e�N�X�`���̏ꍇ�͈�x�V�X�e���������T�[�t�F�X�Ƀf�[�^��]�����Ă��烍�b�N����
			if( Direct3D9_IsExObject() || Image->Orig->FormatDesc.DrawValidFlag )
			{
				D_IDirect3DSurface9 *RTSurface ;
				RECT DestRect ;

				// ��荞�݃T�C�Y�Ɠ����傫���̕`��\�T�[�t�F�X�̍쐬
				RTSurface = NULL ;
				if( Direct3DDevice9_CreateRenderTarget(
						( UINT )DrawTex->WidthI,
						( UINT )DrawTex->HeightI,
						GD3D9.Device.Caps.TextureFormat[ Image->Orig->ColorFormat ],
						D_D3DMULTISAMPLE_NONE,
						0,
						FALSE,
						&RTSurface,
						NULL
					) != D_D3D_OK )
				{
					DXST_ERRORLOG_ADDUTF16LE( "\xed\x30\xc3\x30\xaf\x30\x28\x75\xcf\x63\x3b\x75\xef\x53\xfd\x80\xb5\x30\xfc\x30\xd5\x30\xa7\x30\xb9\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"���b�N�p�`��\�T�[�t�F�X�̍쐬�Ɏ��s���܂���\n" @*/ ) ;
					return -1 ;
				}

				// ��荞�݃T�C�Y�Ɠ����傫���̃V�X�e����������̃I�t�X�N���[���T�[�t�F�X�̍쐬
				if( Direct3DDevice9_CreateOffscreenPlainSurface(
						( UINT )DrawTex->WidthI,
						( UINT )DrawTex->HeightI,
						GD3D9.Device.Caps.TextureFormat[ Image->Orig->ColorFormat ],
						D_D3DPOOL_SYSTEMMEM,
						&SMSurface,
						NULL
					) != D_D3D_OK )
				{
					DXST_ERRORLOG_ADDUTF16LE( "\xed\x30\xc3\x30\xaf\x30\x28\x75\xb7\x30\xb9\x30\xc6\x30\xe0\x30\xe1\x30\xe2\x30\xea\x30\xb5\x30\xfc\x30\xd5\x30\xa7\x30\xb9\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"���b�N�p�V�X�e���������T�[�t�F�X�̍쐬�Ɏ��s���܂���\n" @*/ ) ;
					Direct3D9_ObjectRelease( RTSurface ) ;
					return -1 ;
				}

				// �܂��`��\�T�[�t�F�X�ɓ��e��]������
				Rect.left       = DrawTex->UsePosXI ;
				Rect.top        = DrawTex->UsePosYI ;
				Rect.right      = DrawTex->UsePosXI + DrawTex->WidthI ;
				Rect.bottom     = DrawTex->UsePosYI + DrawTex->HeightI ;
				DestRect.left   = 0 ;
				DestRect.top    = 0 ;
				DestRect.right  = DrawTex->WidthI ;
				DestRect.bottom = DrawTex->HeightI ;
				hr = Direct3DDevice9_StretchRect(
					DrawTex->Tex->PF->D3D9.Surface[ 0 ], &Rect,
					RTSurface, &DestRect, D_D3DTEXF_NONE ) ;

				// ���̌�V�X�e���������T�[�t�F�X�ɓ��e��]������
				hr = Direct3DDevice9_GetRenderTargetData( RTSurface, SMSurface ) ;

				// �ꎞ�I�ɍ쐬�����T�[�t�F�X�̉��
				Direct3D9_ObjectRelease( RTSurface ) ;

				// �V�X�e���������[�t�F�X�����b�N
				Rect.left   = 0 ;
				Rect.top    = 0 ;
				Rect.right  = DrawTex->WidthI ;
				Rect.bottom = DrawTex->HeightI ;
				hr = Direct3DSurface9_LockRect( SMSurface, &LockRect, &Rect, D_D3DLOCK_READONLY ) ;
			}
			else
			{
				// ����ȊO�̏ꍇ�͂����Ȃ胍�b�N
				Rect.left   = DrawTex->UsePosXI ;
				Rect.top    = DrawTex->UsePosYI ;
				Rect.right  = DrawTex->UsePosXI + DrawTex->WidthI ;
				Rect.bottom = DrawTex->UsePosYI + DrawTex->HeightI ;
				hr = Direct3DSurface9_LockRect( DrawTex->Tex->PF->D3D9.Surface[ 0 ], &LockRect, &Rect, D_D3DLOCK_READONLY ) ;
			}

			// �]��
			Src = ( BYTE * )LockRect.pBits ;
			Dest = Image->LockImage + DrawTex->DrawPosXI * ColorData->PixelByte + DrawTex->DrawPosYI * Image->LockImagePitch ;
			WidthByte = ( DWORD )( DrawTex->WidthI * ColorData->PixelByte ) ;
			for( i = 0 ; i < DrawTex->HeightI ; i ++, Dest += Image->LockImagePitch, Src += LockRect.Pitch )
			{
				_MEMCPY( Dest, Src, WidthByte ) ;
			}

			// ���b�N�̉���
			if( SMSurface )
			{
				Direct3DSurface9_UnlockRect( SMSurface ) ;
				Direct3D9_ObjectRelease( SMSurface ) ;
			}
			else
			{
				Direct3DSurface9_UnlockRect( DrawTex->Tex->PF->D3D9.Surface[ 0 ] ) ;
			}
		}
	}

	// �J���[�f�[�^�̃A�h���X��ۑ�
	*ColorDataP = ColorData ;

	// ����I��
	return 0 ;
}

// �O���t�B�b�N�������̈�̃��b�N����
extern	int		Graphics_Hardware_D3D9_GraphUnlock_PF( IMAGEDATA *Image )
{
	IMAGEDATA_HARD_DRAW *DrawTex ;
	int                  i ;
	int                  j ;
	RECT                 Rect ;
	RECT                 SrcRect ;
	COLORDATA           *ColorData ;
	BYTE                *Dest ;
	BYTE                *Src ;
	DWORD                WidthByte ;
	POINT                DestPoint ;
	HRESULT              hr ;
	D_D3DLOCKED_RECT     LockRect ;
	D_IDirect3DSurface9 *SMSurface ;

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �`����I�����Ă���
	Graphics_D3D9_EndScene() ;

	// �e�N�X�`���̃t�H�[�}�b�g���擾
	ColorData = Graphics_D3D9_GetD3DFormatColorData( GD3D9.Device.Caps.TextureFormat[ Image->Orig->ColorFormat ] ) ;

	// �`����̐������J��Ԃ�
	DrawTex = Image->Hard.Draw ;
	SMSurface = NULL ;
	for( i = 0 ; i < Image->Hard.DrawNum ; i ++, DrawTex ++ )
	{
		// Direct3D9Ex ���g�p���Ă��邩�A���͕`��\�e�N�X�`�����ǂ����ŏ����𕪊�
		if( Direct3D9_IsExObject() || Image->Orig->FormatDesc.DrawValidFlag )
		{
			// Direct3D9Ex ���g�p���Ă��邩�A���͕`��\�e�N�X�`���̏ꍇ�͈�x�V�X�e���������T�[�t�F�X�Ƀf�[�^��]�����Ă���X�V����
			if( Direct3DDevice9_CreateOffscreenPlainSurface( ( UINT )DrawTex->WidthI, ( UINT )DrawTex->HeightI, GD3D9.Device.Caps.TextureFormat[ Image->Orig->ColorFormat ], D_D3DPOOL_SYSTEMMEM, &SMSurface, NULL ) != D_D3D_OK )
			{
				DXST_ERRORLOG_ADDUTF16LE( "\xed\x30\xc3\x30\xaf\x30\x28\x75\xc6\x30\xf3\x30\xdd\x30\xe9\x30\xea\x30\x6e\x30\xb7\x30\xb9\x30\xc6\x30\xe0\x30\xe1\x30\xe2\x30\xea\x30\x4d\x91\x6e\x7f\xb5\x30\xfc\x30\xd5\x30\xa7\x30\xb9\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"���b�N�p�e���|�����̃V�X�e���������z�u�T�[�t�F�X�̍쐬�Ɏ��s���܂���\n" @*/ ) ;
				return -1 ;
			}

			// �쐬�����e�N�X�`�������b�N
			hr = Direct3DSurface9_LockRect( SMSurface, &LockRect, NULL, 0 ) ;

			// �f�[�^��]��
			Src       = Image->LockImage + ColorData->PixelByte * DrawTex->DrawPosXI + Image->LockImagePitch * DrawTex->DrawPosYI ;
			Dest      = ( BYTE * )LockRect.pBits ;
			WidthByte = ( DWORD )( Image->WidthI * ColorData->PixelByte ) ;
			for( j = 0 ; j < DrawTex->HeightI ; j ++, Src += Image->LockImagePitch, Dest += LockRect.Pitch )
			{
				_MEMCPY( Dest, Src, WidthByte ) ;
			}

			// ���b�N����
			Direct3DSurface9_UnlockRect( SMSurface ) ;

			// �V�X�e���������T�[�t�F�X����`��\�e�N�X�`���ɓ]��
			SrcRect.left = 0 ;
			SrcRect.top = 0 ;
			SrcRect.right = DrawTex->WidthI ;
			SrcRect.bottom = DrawTex->HeightI ;
			DestPoint.x = DrawTex->UsePosXI ;
			DestPoint.y = DrawTex->UsePosYI ;
			hr = Direct3DDevice9_UpdateSurface(
				SMSurface,                  &SrcRect,
				DrawTex->Tex->PF->D3D9.Surface[ 0 ], &DestPoint ) ;

			// �V�X�e���������T�[�t�F�X���
			if( SMSurface )
			{
				Direct3D9_ObjectRelease( SMSurface ) ;
				SMSurface = NULL ;
			}
		}
		else
		{
			// �ʏ�̃e�N�X�`���̏ꍇ�̓��b�N���ē]������

			// ���b�N
			Rect.left   = DrawTex->UsePosXI ;
			Rect.top    = DrawTex->UsePosYI ;
			Rect.right  = DrawTex->UsePosXI + DrawTex->WidthI ;
			Rect.bottom = DrawTex->UsePosYI + DrawTex->HeightI ;
			hr = Direct3DSurface9_LockRect( DrawTex->Tex->PF->D3D9.Surface[ 0 ], &LockRect, &Rect, 0 ) ;

			// �]��
			Src       = Image->LockImage + DrawTex->DrawPosXI * ColorData->PixelByte + DrawTex->DrawPosYI * Image->LockImagePitch ;
			Dest      = ( BYTE * )LockRect.pBits ;
			WidthByte = ( DWORD )( DrawTex->WidthI * ColorData->PixelByte ) ;
			for( i = 0 ; i < DrawTex->HeightI ; i ++, Src += Image->LockImagePitch, Dest += LockRect.Pitch )
			{
				_MEMCPY( Dest, Src, WidthByte ) ;
			}

			// ���b�N�̉���
			Direct3DSurface9_UnlockRect( DrawTex->Tex->PF->D3D9.Surface[ 0 ] ) ;
		}
	}

	// ���b�N�C���[�W�̉��
	DXFREE( Image->LockImage ) ;
	Image->LockImage = NULL ;

	// ����I��
	return 0 ;
}

// �O���t�B�b�N�̂y�o�b�t�@�̏�Ԃ�ʂ̃O���t�B�b�N�̂y�o�b�t�@�ɃR�s�[����( DestGrHandle �� SrcGrHandle ���y�o�b�t�@�������Ă���`��\�摜�ŁA���A���`�G�C���A�X�摜�ł͂Ȃ����Ƃ����� )
extern	int		Graphics_Hardware_D3D9_CopyGraphZBufferImage_PF( IMAGEDATA *DestImage, IMAGEDATA *SrcImage )
{
	// �`��\�ł͂Ȃ��ꍇ���������Ȃ�
	if( SrcImage->Orig->FormatDesc.DrawValidFlag == FALSE ||
		DestImage->Orig->FormatDesc.DrawValidFlag == FALSE )
		return -1 ;

	// �A���`�G�C���A�X�����_�����O�^�[�Q�b�g�̏ꍇ���������Ȃ�
	if( SrcImage->Orig->FormatDesc.MSSamples != D_D3DMULTISAMPLE_NONE ||
		DestImage->Orig->FormatDesc.MSSamples != D_D3DMULTISAMPLE_NONE )
		return -1 ;

	// �y�o�b�t�@�������ꍇ���������Ȃ�
	if( SrcImage->Orig->Hard.Tex[ 0 ].PF->D3D9.ZBuffer == NULL ||
		DestImage->Orig->Hard.Tex[ 0 ].PF->D3D9.ZBuffer == NULL )
		return -1 ;

	// �T�C�Y���Ⴄ�ꍇ���������Ȃ�
	if( SrcImage->Orig->Hard.Tex[ 0 ].TexWidth != DestImage->Orig->Hard.Tex[ 0 ].TexWidth ||
		SrcImage->Orig->Hard.Tex[ 0 ].TexHeight != DestImage->Orig->Hard.Tex[ 0 ].TexHeight )
		return -1 ;

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �`����I��点�Ă���
	Graphics_D3D9_EndScene() ;

	// �]��
	if( Direct3DDevice9_StretchRect( SrcImage->Orig->Hard.Tex[ 0 ].PF->D3D9.ZBuffer, NULL, DestImage->Orig->Hard.Tex[ 0 ].PF->D3D9.ZBuffer, NULL, D_D3DTEXF_NONE ) != D_D3D_OK )
	{
		DXST_ERRORLOG_ADDUTF16LE( "\x5a\x00\xd0\x30\xc3\x30\xd5\x30\xa1\x30\xfc\x30\xa4\x30\xe1\x30\xfc\x30\xb8\x30\x6e\x30\xe2\x8e\x01\x90\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Z�o�b�t�@�[�C���[�W�̓]���Ɏ��s���܂���\n" @*/ ) ;
		return -1 ;
	}

	// ����I��
	return 0 ;
}

// �摜�f�[�^�̏�����
extern	int		Graphics_Hardware_D3D9_InitGraph_PF( void )
{
	// �V�X�e���������e�N�X�`���̉��
	Graphics_D3D9_ReleaseSysMemTextureAll() ;

	// �V�X�e���������T�[�t�F�X�̉��
	Graphics_D3D9_ReleaseSysMemSurfaceAll() ;

	// ����I��
	return 0 ;
}

// �O���t�B�b�N�����̐F�œh��Ԃ�
extern	int		Graphics_Hardware_D3D9_FillGraph_PF( IMAGEDATA *Image, int Red, int Green, int Blue, int Alpha, int ASyncThread )
{
	IMAGEDATA_HARD_DRAW *DrawTex ;
	int                  i ;
	DWORD                Code ;
	RECT                 Rect ;

	// �f�o�C�X������������G���[
	if( GAPIWin.Direct3DDevice9Object == NULL ) return -1 ;

	// �J���[�R�[�h�̍쐬
	Code = ( ( DWORD )Alpha << 24 ) | ( ( DWORD )Red << 16 ) | ( ( DWORD )Green << 8 ) | ( ( DWORD )Blue << 0 ) ;

	// �`����̐������J��Ԃ�
	if( Image->Orig->FormatDesc.DrawValidFlag )
	{
		DrawTex = Image->Hard.Draw ;
		for( i = 0 ; i < Image->Hard.DrawNum ; i ++, DrawTex ++ )
		{
			// �g�p����w��F�œh��Ԃ�
			Rect.left   = DrawTex->UsePosXI ;
			Rect.top    = DrawTex->UsePosYI ;
			Rect.right  = DrawTex->UsePosXI + DrawTex->WidthI ;
			Rect.bottom = DrawTex->UsePosYI + DrawTex->HeightI ;
			Direct3DDevice9_ColorFill_ASync( DrawTex->Tex->PF->D3D9.Surface[ 0 ], &Rect, Code, ASyncThread ) ;
		}
	}
	else
	{
		return 2 ;
	}

	// ����I��
	return 0 ;
}

// �`��\�摜��o�b�N�o�b�t�@����w��̈�̃O���t�B�b�N���擾����
extern	int		Graphics_Hardware_D3D9_GetDrawScreenGraphBase_PF( IMAGEDATA *Image, IMAGEDATA *TargetImage, int TargetScreen, int TargetScreenSurface, int /* TargetScreenWidth */, int /* TargetScreenHeight */, int x1, int y1, int x2, int y2, int destX, int destY )
{
	RECT SrcRect ;
	RECT DestRect ;
	RECT Rect ;
	int Width, Height ;

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �`�揈�����������Ă���
	Graphics_D3D9_EndScene() ;

	// ���ƍ������v�Z
	Width  = x2 - x1 ;
	Height = y2 - y1 ;

	// �]����ƂȂ�T�[�t�F�X���`��\�e�N�X�`���������ꍇ�͒P���ɓ]��
	if( Image->Orig->FormatDesc.DrawValidFlag )
	{
		D_IDirect3DSurface9 *TargetSurface ;

		// ��荞�݌��ƂȂ�T�[�t�F�X�̌���

		// �`��\�摜���Ώۂ̏ꍇ
		if( TargetImage )
		{
			TargetSurface = TargetImage->Orig->Hard.Tex[ 0 ].PF->D3D9.RenderTargetSurface ?
							TargetImage->Orig->Hard.Tex[ 0 ].PF->D3D9.RenderTargetSurface :
							TargetImage->Orig->Hard.Tex[ 0 ].PF->D3D9.Surface[ TargetScreenSurface ] ;
		}
		else
		// ����ȊO�̏ꍇ�̓T�u�o�b�N�o�b�t�@���L���������炻�ꂪ�Ώ�
		{
			TargetSurface = GD3D9.Device.Screen.SubBackBufferSurface ? GD3D9.Device.Screen.SubBackBufferSurface : GD3D9.Device.Screen.BackBufferSurface ;
		}

		SrcRect.left   = x1 ;
		SrcRect.top    = y1 ;
		SrcRect.right  = x2 ;
		SrcRect.bottom = y2 ;

		DestRect.left   = destX ;
		DestRect.top    = destY ;
		DestRect.right  = destX + Width ;
		DestRect.bottom = destY + Height ;
		Direct3DDevice9_StretchRect(
			TargetSurface,                               &SrcRect,
			Image->Orig->Hard.Tex[ 0 ].PF->D3D9.Surface[ 0 ], &DestRect, D_D3DTEXF_NONE ) ;
	}
	else
	// �ʏ�̃e�N�X�`���������ꍇ�͍ŏ��Ƀ��b�N�������A�ʖڂ��������x�������Ɏ�荞��ł���]��
	{
		// �`���o�b�t�@�����b�N
		BASEIMAGE LockImage ;
		SrcRect.left   = x1 ;
		SrcRect.top    = y1 ;
		SrcRect.right  = x2 ;
		SrcRect.bottom = y2 ;
		if( Graphics_Screen_LockDrawScreen( &SrcRect, &LockImage, TargetScreen, TargetScreenSurface, TRUE, 0 ) < 0 )
		{
			return -1 ;
		}

		// �e�N�X�`���ɓ]��
		Rect.left   = 0 ;
		Rect.top    = 0 ;
		Rect.right  = LockImage.Width ;
		Rect.bottom = LockImage.Height ;
		Graphics_Image_BltBmpOrBaseImageToGraph3(
			&Rect,
			destX, destY,
			Image->HandleInfo.Handle,
			&LockImage, NULL,
			FALSE,
			FALSE,
			FALSE
		) ;

		// �`���o�b�t�@�̃A�����b�N
		Graphics_Screen_UnlockDrawScreen() ;
	}

	// ����I��
	return 0 ;
}

static int Graphics_D3D9_BltBmpOrBaseImageToGraph3_MipMapBlt(
	      IMAGEDATA_ORIG			*Orig,
	const RECT						*SrcRect,
	const RECT						*DestRect,
	      int						UseSysMemSurface,
	      D_IDirect3DBaseTexture9	*UseTex,
	      int						TexWidth,
	      int						TexHeight,
	const BASEIMAGE					*RgbBaseImage,
	const BASEIMAGE					*AlphaBaseImage,
	const COLORDATA					*DestColor,
	      int						RedIsAlphaFlag,
	      int						UseTransColorConvAlpha,
	      int						ASyncThread
)
{
	int					AlphaImageSize ;
	int					ImageSize ;
	int					ImageW ;
	int					ImageH ;
	int					ImagePitch ;
	void				*Image1 ;
	void				*Image2 ;
	void				*ImageD ;
	void				*ImageS ;
	void				*ImageBuffer ;
	int					ImageDW ;
	int					ImageDH ;
	int					ImageSW ;
	int					ImageSH ;
	int					ImageNum ;
	RECT				DestRectT ;
	RECT				SrcRectT ;
	D_D3DLOCKED_RECT	LockRect ;
	POINT				DestPoint ;
	int					UseSysMemSurfaceIndex = -1 ;
	D_IDirect3DSurface9	*DestSurface = NULL ;
	D_IDirect3DSurface9	*UseSurface = NULL ;
	int					TempTexWidth ;
	int					TempTexHeight ;
	int					i ;
	int					j ;
	int					ts ;
	int					IsDXTFormat ;
	HRESULT				hr ;

	DestPoint.x = 0 ;
	DestPoint.y = 0 ;

	IsDXTFormat =
		( RgbBaseImage->ColorData.Format == DX_BASEIMAGE_FORMAT_DXT1 ) ||
		( RgbBaseImage->ColorData.Format == DX_BASEIMAGE_FORMAT_DXT2 ) ||
		( RgbBaseImage->ColorData.Format == DX_BASEIMAGE_FORMAT_DXT3 ) ||
		( RgbBaseImage->ColorData.Format == DX_BASEIMAGE_FORMAT_DXT4 ) ||
		( RgbBaseImage->ColorData.Format == DX_BASEIMAGE_FORMAT_DXT5 ) ;

	ImageNum = Orig->FormatDesc.CubeMapTextureFlag ? CUBEMAP_SURFACE_NUM : 1 ;

	// �]���悪�e�N�X�`���S�̂ŁA����DXT�t�H�[�}�b�g�œ]�����DXT�t�H�[�}�b�g�̏ꍇ�͒P���ȃ������]�����s��
	if( DestRect->left == 0 && DestRect->right  == TexWidth  &&
		DestRect->top  == 0 && DestRect->bottom == TexHeight &&
		RgbBaseImage->MipMapCount >= Orig->Hard.MipMapCount &&
		( ( RgbBaseImage->ColorData.Format == DX_BASEIMAGE_FORMAT_DXT1 && GD3D9.Device.Caps.TextureFormat[ Orig->ColorFormat ] == D_D3DFMT_DXT1 ) ||
		  ( RgbBaseImage->ColorData.Format == DX_BASEIMAGE_FORMAT_DXT2 && GD3D9.Device.Caps.TextureFormat[ Orig->ColorFormat ] == D_D3DFMT_DXT2 ) ||
		  ( RgbBaseImage->ColorData.Format == DX_BASEIMAGE_FORMAT_DXT3 && GD3D9.Device.Caps.TextureFormat[ Orig->ColorFormat ] == D_D3DFMT_DXT3 ) ||
		  ( RgbBaseImage->ColorData.Format == DX_BASEIMAGE_FORMAT_DXT4 && GD3D9.Device.Caps.TextureFormat[ Orig->ColorFormat ] == D_D3DFMT_DXT4 ) ||
		  ( RgbBaseImage->ColorData.Format == DX_BASEIMAGE_FORMAT_DXT5 && GD3D9.Device.Caps.TextureFormat[ Orig->ColorFormat ] == D_D3DFMT_DXT5 ) ) )
	{
		if( Orig->FormatDesc.CubeMapTextureFlag )
		{
			ImageBuffer = RgbBaseImage->GraphData ;
			for( i = 0 ; i < CUBEMAP_SURFACE_NUM ; i ++ )
			{
				TempTexWidth  = TexWidth ;
				TempTexHeight = TexHeight ;
				for( j = 0 ; j < Orig->Hard.MipMapCount ; j ++ )
				{
					if( IsDXTFormat )
					{
						ImageSize = ( ( TempTexWidth + 3 ) / 4 ) * ( ( TempTexHeight + 3 ) / 4 ) * ( RgbBaseImage->ColorData.ColorBitDepth * 2 ) ;
					}
					else
					{
						ImageSize = TempTexWidth * TempTexHeight * RgbBaseImage->ColorData.PixelByte ;
					}

					// �e�N�X�`�������b�N
					if( UseSysMemSurface == FALSE )
					{
						hr = Direct3DCubeTexture9_LockRect_ASync(
							( D_IDirect3DCubeTexture9 * )UseTex,
							( D_D3DCUBEMAP_FACES )i,
							( UINT )j,
							&LockRect,
							NULL,
							0,
							ASyncThread
						) ;
					}
					else
					{
						for(;;)
						{
							UseSysMemSurfaceIndex = Graphics_D3D9_GetSysMemSurface(
								TempTexWidth,
								TempTexHeight,
								GD3D9.Device.Caps.TextureFormat[ Orig->ColorFormat ],
								ASyncThread
							) ;

							if( UseSysMemSurfaceIndex != -1 )
							{
								break ;
							}

							if( NS_ProcessMessage() != 0 )
							{
								break ;
							}

							Thread_Sleep( 1 ) ;
						}

						if( UseSysMemSurfaceIndex == -1 )
						{
							return -1 ;
						}

						UseSurface = GD3D9.SysMemTexSurf.Surface[ UseSysMemSurfaceIndex ].MemSurface ;

						hr = Direct3DSurface9_LockRect_ASync(
							UseSurface,
							&LockRect,
							NULL,
							D_D3DLOCK_DISCARD,
							ASyncThread
						) ;
					}
					if( hr != D_D3D_OK )
					{
						DXST_ERRORLOG_ADDUTF16LE( "\xad\x30\xe5\x30\xfc\x30\xd6\x30\xde\x30\xc3\x30\xd7\x30\xdf\x30\xc3\x30\xd7\x30\xde\x30\xc3\x30\xd7\x30\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\x6e\x30\xed\x30\xc3\x30\xaf\x30\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�L���[�u�}�b�v�~�b�v�}�b�v�e�N�X�`���̃��b�N�Ɏ��s���܂���\n" @*/ ) ;
						return -1 ;
					}

					// �e�N�X�`���ɓ]��
					_MEMCPY( LockRect.pBits, ImageBuffer, ( size_t )ImageSize ) ;

					// ���b�N����
					if( UseSysMemSurface == FALSE )
					{
						Direct3DCubeTexture9_UnlockRect_ASync(
							( D_IDirect3DCubeTexture9 * )UseTex,
							( D_D3DCUBEMAP_FACES )i,
							( UINT )j,
							ASyncThread
						) ;
					}
					else
					{
						Direct3DSurface9_UnlockRect_ASync( UseSurface, ASyncThread ) ;

						Direct3DCubeTexture9_GetCubeMapSurface_ASync(
							( D_IDirect3DCubeTexture9 * )UseTex,
							( D_D3DCUBEMAP_FACES )i,
							( UINT )j,
							&DestSurface,
							ASyncThread
						) ;

						Direct3DDevice9_UpdateSurface_ASync(
							UseSurface,
							NULL,
							DestSurface,
							NULL,
							ASyncThread
						) ; 

						Direct3D9_ObjectRelease_ASync( DestSurface, ASyncThread ) ;

						Graphics_D3D9_ReleaseSysMemSurface( UseSysMemSurfaceIndex, ASyncThread ) ;
						UseSysMemSurfaceIndex = -1 ;
					}

					// �]�����A�h���X��i�߂�
					ImageBuffer  = ( BYTE * )ImageBuffer + ImageSize ;
					ImageSize   /= 4 ;
					if( ImageSize < 8 )
					{
						ImageSize = 8 ;
					}

					TempTexWidth  >>= 1 ;
					TempTexHeight >>= 1 ;
					if( TempTexWidth < 1 )
					{
						TempTexWidth = 1 ;
					}
					if( TempTexHeight < 1 )
					{
						TempTexHeight = 1 ;
					}
				}
			}
		}
		else
		{
			TempTexWidth  = TexWidth ;
			TempTexHeight = TexHeight ;
			ImageBuffer   = RgbBaseImage->GraphData ;
			for( j = 0 ; j < Orig->Hard.MipMapCount ; j ++ )
			{
				if( IsDXTFormat )
				{
					ImageSize = ( ( TempTexWidth + 3 ) / 4 ) * ( ( TempTexHeight + 3 ) / 4 ) * ( RgbBaseImage->ColorData.ColorBitDepth * 2 ) ;
				}
				else
				{
					ImageSize = TempTexWidth * TempTexHeight * RgbBaseImage->ColorData.PixelByte ;
				}

				// �e�N�X�`�������b�N
				if( UseSysMemSurface == FALSE )
				{
					hr = Direct3DTexture9_LockRect_ASync( ( D_IDirect3DTexture9 * )UseTex, ( UINT )j, &LockRect, NULL, 0, ASyncThread ) ;
				}
				else
				{
					for(;;)
					{
						UseSysMemSurfaceIndex = Graphics_D3D9_GetSysMemSurface(
							TempTexWidth,
							TempTexHeight,
							GD3D9.Device.Caps.TextureFormat[ Orig->ColorFormat ],
							ASyncThread
						) ;

						if( UseSysMemSurfaceIndex != -1 )
						{
							break ;
						}

						if( NS_ProcessMessage() != 0 )
						{
							break ;
						}

						Thread_Sleep( 1 ) ;
					}

					if( UseSysMemSurfaceIndex == -1 )
					{
						return -1 ;
					}

					UseSurface = GD3D9.SysMemTexSurf.Surface[ UseSysMemSurfaceIndex ].MemSurface ;

					hr = Direct3DSurface9_LockRect_ASync(
						UseSurface,
						&LockRect,
						NULL,
						D_D3DLOCK_DISCARD,
						ASyncThread
					) ;
				}
				if( hr != D_D3D_OK )
				{
					DXST_ERRORLOG_ADDUTF16LE( "\xdf\x30\xc3\x30\xd7\x30\xde\x30\xc3\x30\xd7\x30\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\x6e\x30\xed\x30\xc3\x30\xaf\x30\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�~�b�v�}�b�v�e�N�X�`���̃��b�N�Ɏ��s���܂���\n" @*/ ) ;
					return -1 ;
				}

				// �e�N�X�`���ɓ]��
				_MEMCPY( LockRect.pBits, ImageBuffer, ( size_t )ImageSize ) ;

				// ���b�N����
				if( UseSysMemSurface == FALSE )
				{
					Direct3DTexture9_UnlockRect_ASync( ( D_IDirect3DTexture9 * )UseTex, ( UINT )j, ASyncThread ) ;
				}
				else
				{
					Direct3DSurface9_UnlockRect_ASync( UseSurface, ASyncThread ) ;

					Direct3DTexture9_GetSurfaceLevel_ASync(
						( D_IDirect3DTexture9 * )UseTex,
						( UINT )j,
						&DestSurface,
						ASyncThread
					) ;

					Direct3DDevice9_UpdateSurface_ASync(
						UseSurface,
						NULL,
						DestSurface,
						NULL,
						ASyncThread
					) ; 

					Direct3D9_ObjectRelease_ASync( DestSurface, ASyncThread ) ;

					Graphics_D3D9_ReleaseSysMemSurface( UseSysMemSurfaceIndex, ASyncThread ) ;
					UseSysMemSurfaceIndex = -1 ;
				}

				// �]�����A�h���X��i�߂�
				ImageBuffer  = ( BYTE * )ImageBuffer + ImageSize ;
				ImageSize   /= 4 ;
				if( ImageSize < 8 )
				{
					ImageSize = 8 ;
				}

				TempTexWidth  >>= 1 ;
				TempTexHeight >>= 1 ;
				if( TempTexWidth < 1 )
				{
					TempTexWidth = 1 ;
				}
				if( TempTexHeight < 1 )
				{
					TempTexHeight = 1 ;
				}
			}
		}
	}
	else
	{
		// �]���悪�W���t�H�[�}�b�g�ł͂Ȃ��ꍇ�͓]���ł��Ȃ�
		if( GD3D9.Device.Caps.TextureFormat[ Orig->ColorFormat ] != D_D3DFMT_DXT1 &&
			GD3D9.Device.Caps.TextureFormat[ Orig->ColorFormat ] != D_D3DFMT_DXT2 &&
			GD3D9.Device.Caps.TextureFormat[ Orig->ColorFormat ] != D_D3DFMT_DXT3 &&
			GD3D9.Device.Caps.TextureFormat[ Orig->ColorFormat ] != D_D3DFMT_DXT4 &&
			GD3D9.Device.Caps.TextureFormat[ Orig->ColorFormat ] != D_D3DFMT_DXT5 )
		{
			// �]�����Ƀ~�b�v�}�b�v��񂪂���ꍇ�Ɩ����ꍇ�ŏ����𕪊�
			if( RgbBaseImage->MipMapCount >= Orig->Hard.MipMapCount )
			{
				// ����ꍇ

				ImageBuffer = RgbBaseImage->GraphData ;
				if( Orig->FormatDesc.CubeMapTextureFlag )
				{
					for( i = 0 ; i < CUBEMAP_SURFACE_NUM ; i ++ )
					{
						TempTexWidth = TexWidth ;
						TempTexHeight = TexHeight ;
//						ImageSize = RgbBaseImage->Width * RgbBaseImage->Height * RgbBaseImage->ColorData.PixelByte ;
						for( j = 0 ; j < Orig->Hard.MipMapCount ; j ++ )
						{
							ImageSize = TempTexWidth * TempTexHeight * RgbBaseImage->ColorData.PixelByte ;

							// �e�N�X�`�������b�N
							if( UseSysMemSurface == FALSE )
							{
								hr = Direct3DCubeTexture9_LockRect_ASync(
									( D_IDirect3DCubeTexture9 * )UseTex,
									( D_D3DCUBEMAP_FACES )i,
									( UINT )j,
									&LockRect,
									NULL,
									0,
									ASyncThread
								) ;
							}
							else
							{
								for(;;)
								{
									UseSysMemSurfaceIndex = Graphics_D3D9_GetSysMemSurface(
										TempTexWidth,
										TempTexHeight,
										GD3D9.Device.Caps.TextureFormat[ Orig->ColorFormat ],
										ASyncThread
									) ;

									if( UseSysMemSurfaceIndex != -1 )
									{
										break ;
									}

									if( NS_ProcessMessage() != 0 )
									{
										break ;
									}

									Thread_Sleep( 1 ) ;
								}
								if( UseSysMemSurfaceIndex == -1 )
								{
									return -1 ;
								}
								UseSurface = GD3D9.SysMemTexSurf.Surface[ UseSysMemSurfaceIndex ].MemSurface ;

								hr = Direct3DSurface9_LockRect_ASync( UseSurface, &LockRect, NULL, D_D3DLOCK_DISCARD, ASyncThread ) ;
							}
							if( hr != D_D3D_OK )
							{
								DXST_ERRORLOG_ADDUTF16LE( "\xdf\x30\xc3\x30\xd7\x30\xde\x30\xc3\x30\xd7\x30\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\x6e\x30\xed\x30\xc3\x30\xaf\x30\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�~�b�v�}�b�v�e�N�X�`���̃��b�N�Ɏ��s���܂���\n" @*/ ) ;
								return -1 ;
							}

							// �e�N�X�`���ɓ]��
							_MEMCPY( LockRect.pBits, ImageBuffer, ( size_t )ImageSize ) ;

							// ���b�N����
							if( UseSysMemSurface == FALSE )
							{
								Direct3DCubeTexture9_UnlockRect_ASync( ( D_IDirect3DCubeTexture9 * )UseTex, ( D_D3DCUBEMAP_FACES )i, ( UINT )j, ASyncThread ) ;
							}
							else
							{
								Direct3DSurface9_UnlockRect_ASync( UseSurface, ASyncThread ) ;

								Direct3DCubeTexture9_GetCubeMapSurface_ASync( ( D_IDirect3DCubeTexture9 * )UseTex, ( D_D3DCUBEMAP_FACES )i, ( UINT )j, &DestSurface, ASyncThread ) ;

								Direct3DDevice9_UpdateSurface_ASync( UseSurface, NULL, DestSurface, NULL, ASyncThread ) ; 

								Direct3D9_ObjectRelease_ASync( DestSurface, ASyncThread ) ;

								Graphics_D3D9_ReleaseSysMemSurface( UseSysMemSurfaceIndex, ASyncThread ) ;
								UseSysMemSurfaceIndex = -1 ;
							}

							// �]�����A�h���X��i�߂�
							ImageBuffer = ( BYTE * )ImageBuffer + ImageSize ;
							ImageSize /= 4 ;

							TempTexWidth >>= 1 ;
							TempTexHeight >>= 1 ;
							if( TempTexWidth < 1 )
							{
								TempTexWidth = 1 ;
							}
							if( TempTexHeight < 1 )
							{
								TempTexHeight = 1 ;
							}
						}
					}
				}
				else
				{
					TempTexWidth = TexWidth ;
					TempTexHeight = TexHeight ;
//					ImageSize = RgbBaseImage->Width * RgbBaseImage->Height * RgbBaseImage->ColorData.PixelByte ;
					for( j = 0 ; j < Orig->Hard.MipMapCount ; j ++ )
					{
						ImageSize = TempTexWidth * TempTexHeight * RgbBaseImage->ColorData.PixelByte ;

						// �e�N�X�`�������b�N
						if( UseSysMemSurface == FALSE )
						{
							hr = Direct3DTexture9_LockRect_ASync( ( D_IDirect3DTexture9 * )UseTex, ( UINT )j, &LockRect, NULL, 0, ASyncThread ) ;
						}
						else
						{
							for(;;)
							{
								UseSysMemSurfaceIndex = Graphics_D3D9_GetSysMemSurface( TempTexWidth, TempTexHeight, GD3D9.Device.Caps.TextureFormat[ Orig->ColorFormat ], ASyncThread ) ;
								if( UseSysMemSurfaceIndex != -1 ) break ;
								if( NS_ProcessMessage() != 0 ) break ;
								Thread_Sleep( 1 ) ;
							}
							if( UseSysMemSurfaceIndex == -1 )
							{
								return -1 ;
							}
							UseSurface = GD3D9.SysMemTexSurf.Surface[ UseSysMemSurfaceIndex ].MemSurface ;

							hr = Direct3DSurface9_LockRect_ASync( UseSurface, &LockRect, NULL, D_D3DLOCK_DISCARD, ASyncThread ) ;
						}
						if( hr != D_D3D_OK )
						{
							DXST_ERRORLOG_ADDUTF16LE( "\xdf\x30\xc3\x30\xd7\x30\xde\x30\xc3\x30\xd7\x30\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\x6e\x30\xed\x30\xc3\x30\xaf\x30\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�~�b�v�}�b�v�e�N�X�`���̃��b�N�Ɏ��s���܂���\n" @*/ ) ;
							return -1 ;
						}

						// �e�N�X�`���ɓ]��
						_MEMCPY( LockRect.pBits, ImageBuffer, ( size_t )ImageSize ) ;

						// ���b�N����
						if( UseSysMemSurface == FALSE )
						{
							Direct3DTexture9_UnlockRect_ASync( ( D_IDirect3DTexture9 * )UseTex, ( UINT )j, ASyncThread ) ;
						}
						else
						{
							Direct3DSurface9_UnlockRect_ASync( UseSurface, ASyncThread ) ;

							Direct3DTexture9_GetSurfaceLevel_ASync( ( D_IDirect3DTexture9 * )UseTex, ( UINT )j, &DestSurface, ASyncThread ) ;

							Direct3DDevice9_UpdateSurface_ASync( UseSurface, NULL, DestSurface, NULL, ASyncThread ) ; 

							Direct3D9_ObjectRelease_ASync( DestSurface, ASyncThread ) ;

							Graphics_D3D9_ReleaseSysMemSurface( UseSysMemSurfaceIndex, ASyncThread ) ;
							UseSysMemSurfaceIndex = -1 ;
						}

						// �]�����A�h���X��i�߂�
						ImageBuffer = ( BYTE * )ImageBuffer + ImageSize ;
						ImageSize /= 4 ;

						TempTexWidth >>= 1 ;
						TempTexHeight >>= 1 ;
						if( TempTexWidth < 1 )
						{
							TempTexWidth = 1 ;
						}
						if( TempTexHeight < 1 )
						{
							TempTexHeight = 1 ;
						}
					}
				}
			}
			else
			{
				// �����ꍇ

				// �ꎞ�o�b�t�@�Ɋi�[
				ts = DestRect->right  - DestRect->left ;
				for( ImageW = 1 ; ImageW < ts ; ImageW <<= 1 ){}
				ts = DestRect->bottom - DestRect->top  ;
				for( ImageH = 1 ; ImageH < ts ; ImageH <<= 1 ){}
				ImagePitch  = DestColor->PixelByte * ImageW ;
				ImageSize   = ImagePitch * ImageH ;
				ImageBuffer = DXALLOC( ( size_t )( ImageSize * 2 ) ) ;
				Image1      = ImageBuffer ;
				Image2      = ( BYTE * )Image1 + ImageSize ;
				if( ImageBuffer == NULL )
				{
					DXST_ERRORLOG_ADDUTF16LE( "\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\x78\x30\xe2\x8e\x01\x90\x59\x30\x8b\x30\x3b\x75\xcf\x50\x92\x30\x00\x4e\x42\x66\x84\x76\x6b\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�e�N�X�`���֓]������摜���ꎞ�I�Ɋi�[���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ ) ;
					return -1 ;
				}

				ImageSize      = RgbBaseImage->Pitch   * RgbBaseImage->Height ;
				AlphaImageSize = AlphaBaseImage->Pitch * AlphaBaseImage->Height ;
				for( i = 0 ; i < ImageNum ; i ++ )
				{
					// �~�b�v�}�b�v�̐[�������J��Ԃ�
					ImageSW = ImageW ;
					ImageSH = ImageH ;
					ImageDW = ImageW ;
					ImageDH = ImageH ;
					TempTexWidth = TexWidth ;
					TempTexHeight = TexHeight ;
					for( j = 0 ; j < Orig->Hard.MipMapCount ; j ++ )
					{
						// �k���摜�̍쐬
						if( j == 0 )
						{
							// level0 �̎��͓��{�̏o�̓t�H�[�}�b�g�摜���쐬����
							NS_GraphColorMatchBltVer2(
											   Image1,        ImagePitch,              DestColor,
								( BYTE * )RgbBaseImage->GraphData   + ImageSize * i,      RgbBaseImage->Pitch,   &RgbBaseImage->ColorData,
								( BYTE * )AlphaBaseImage->GraphData + AlphaImageSize * i, AlphaBaseImage->Pitch, &AlphaBaseImage->ColorData,
								DestPoint, SrcRect, FALSE,
								UseTransColorConvAlpha && ( DestColor->AlphaWidth != 0 && AlphaBaseImage->GraphData == NULL ), Orig->TransCode,
								BASEIM.ImageShavedMode, FALSE,
								RedIsAlphaFlag, FALSE,
								FALSE ) ;
							ImageD = Image1 ;
						}
						else
						{
							if( j & 1 )
							{
								ImageS = Image1 ;
								ImageD = Image2 ;
							}
							else
							{
								ImageS = Image2 ;
								ImageD = Image1 ;
							}

							GraphHalfScaleBlt( DestColor, ImageD, ImagePitch, ImageS, ImagePitch, 0, 0, 0, 0, ImageSW, ImageSH ) ;
						}

						// �]����̃��b�N
						DestRectT.left   = DestRect->left   >> j ;
						DestRectT.right  = DestRect->right  >> j ;
						DestRectT.top    = DestRect->top    >> j ;
						DestRectT.bottom = DestRect->bottom >> j ;
						if( DestRectT.right  == DestRectT.left ) DestRectT.right  = DestRectT.left + 1 ;
						if( DestRectT.bottom == DestRectT.top  ) DestRectT.bottom = DestRectT.top  + 1 ;
						if( UseSysMemSurface == FALSE )
						{
							if( Orig->FormatDesc.CubeMapTextureFlag )
							{
								if( Direct3DCubeTexture9_LockRect_ASync( ( D_IDirect3DCubeTexture9 * )UseTex, ( D_D3DCUBEMAP_FACES )i, ( UINT )j, &LockRect, &DestRectT, 0, ASyncThread ) != D_D3D_OK )
								{
									DXFREE( ImageBuffer ) ;
									ImageBuffer = NULL ;
									DXST_ERRORLOG_ADDUTF16LE( "\xad\x30\xe5\x30\xfc\x30\xd6\x30\xde\x30\xc3\x30\xd7\x30\xdf\x30\xc3\x30\xd7\x30\xde\x30\xc3\x30\xd7\x30\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\x6e\x30\xed\x30\xc3\x30\xaf\x30\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�L���[�u�}�b�v�~�b�v�}�b�v�e�N�X�`���̃��b�N�Ɏ��s���܂���\n" @*/ ) ;
									return -1 ;
								}
							}
							else
							{
								if( Direct3DTexture9_LockRect_ASync( ( D_IDirect3DTexture9 * )UseTex, ( UINT )j, &LockRect, &DestRectT, 0, ASyncThread ) != D_D3D_OK )
								{
									DXFREE( ImageBuffer ) ;
									ImageBuffer = NULL ;
									DXST_ERRORLOG_ADDUTF16LE( "\xdf\x30\xc3\x30\xd7\x30\xde\x30\xc3\x30\xd7\x30\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\x6e\x30\xed\x30\xc3\x30\xaf\x30\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�~�b�v�}�b�v�e�N�X�`���̃��b�N�Ɏ��s���܂���\n" @*/ ) ;
									return -1 ;
								}
							}
						}
						else
						{
							for(;;)
							{
								UseSysMemSurfaceIndex = Graphics_D3D9_GetSysMemSurface( TempTexWidth, TempTexHeight, GD3D9.Device.Caps.TextureFormat[ Orig->ColorFormat ], ASyncThread ) ;
								if( UseSysMemSurfaceIndex != -1 ) break ;
								if( NS_ProcessMessage() != 0 ) break ;
								Thread_Sleep( 1 ) ;
							}
							if( UseSysMemSurfaceIndex == -1 )
							{
								DXFREE( ImageBuffer ) ;
								ImageBuffer = NULL ;
								DXST_ERRORLOG_ADDUTF16LE( "\xdf\x30\xc3\x30\xd7\x30\xde\x30\xc3\x30\xd7\x30\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\x28\x75\x6e\x30\xe1\x30\xe2\x30\xea\x30\xb5\x30\xfc\x30\xd5\x30\xa7\x30\xb9\x30\x6e\x30\xd6\x53\x97\x5f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�~�b�v�}�b�v�e�N�X�`���p�̃������T�[�t�F�X�̎擾�Ɏ��s���܂���\n" @*/ ) ;
								return -1 ;
							}
							UseSurface = GD3D9.SysMemTexSurf.Surface[ UseSysMemSurfaceIndex ].MemSurface ;

							if( Direct3DSurface9_LockRect_ASync( UseSurface, &LockRect, &DestRectT, D_D3DLOCK_DISCARD, ASyncThread ) != D_D3D_OK )
							{
								DXFREE( ImageBuffer ) ;
								ImageBuffer = NULL ;
								DXST_ERRORLOG_ADDUTF16LE( "\xdf\x30\xc3\x30\xd7\x30\xde\x30\xc3\x30\xd7\x30\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\x6e\x30\xed\x30\xc3\x30\xaf\x30\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�~�b�v�}�b�v�e�N�X�`���̃��b�N�Ɏ��s���܂���\n" @*/ ) ;
								return -1 ;
							}
						}

						// �e�N�X�`���ɓ]��
						SrcRectT.left   = 0 ;
						SrcRectT.top    = 0 ;
						SrcRectT.right  = ImageDW ;
						SrcRectT.bottom = ImageDH ;
						NS_GraphColorMatchBltVer2(
							LockRect.pBits, LockRect.Pitch,  DestColor,
									ImageD,     ImagePitch,  DestColor,
									  NULL,              0,       NULL,
							DestPoint, &SrcRectT, FALSE,
							FALSE, 0, 0, FALSE, FALSE, FALSE, FALSE ) ;

						// ���b�N����
						if( UseSysMemSurface == FALSE )
						{
							if( Orig->FormatDesc.CubeMapTextureFlag )
							{
								Direct3DCubeTexture9_UnlockRect_ASync( ( D_IDirect3DCubeTexture9 * )UseTex, ( D_D3DCUBEMAP_FACES )i, ( UINT )j, ASyncThread ) ;
							}
							else
							{
								Direct3DTexture9_UnlockRect_ASync( ( D_IDirect3DTexture9 * )UseTex, ( UINT )j, ASyncThread ) ;
							}
						}
						else
						{
							D_IDirect3DSurface9 *DestSurface = NULL ;
							POINT DestSurfacePoint ;

							Direct3DSurface9_UnlockRect_ASync( UseSurface, ASyncThread ) ;

							if( Orig->FormatDesc.CubeMapTextureFlag )
							{
								Direct3DCubeTexture9_GetCubeMapSurface_ASync( ( D_IDirect3DCubeTexture9 * )UseTex, ( D_D3DCUBEMAP_FACES )i, ( UINT )j, &DestSurface, ASyncThread ) ;
							}
							else
							{
								Direct3DTexture9_GetSurfaceLevel_ASync( ( D_IDirect3DTexture9 * )UseTex, ( UINT )j, &DestSurface, ASyncThread ) ;
							}

							DestSurfacePoint.x = DestRectT.left ;
							DestSurfacePoint.y = DestRectT.top ;
							Direct3DDevice9_UpdateSurface_ASync( UseSurface, &DestRectT, DestSurface, &DestSurfacePoint, ASyncThread ) ; 

							Direct3D9_ObjectRelease_ASync( DestSurface, ASyncThread ) ;

							Graphics_D3D9_ReleaseSysMemSurface( UseSysMemSurfaceIndex, ASyncThread ) ;
							UseSysMemSurfaceIndex = -1 ;
						}

						// ���̃C���[�W�̃T�C�Y���Z�b�g
						ImageSW = ImageDW ;
						ImageSH = ImageDH ;
						ImageDW >>= 1 ;
						ImageDH >>= 1 ;
						if( ImageDW == 0 ) ImageDW = 1 ;
						if( ImageDH == 0 ) ImageDH = 1 ;

						TempTexWidth >>= 1 ;
						TempTexHeight >>= 1 ;
						if( TempTexWidth < 1 )
						{
							TempTexWidth = 1 ;
						}
						if( TempTexHeight < 1 )
						{
							TempTexHeight = 1 ;
						}
					}
				}

				// �������̉��
				DXFREE( ImageBuffer ) ;
			}
		}
		else
		{
			DXST_ERRORLOG_ADDUTF16LE( "\x44\x00\x58\x00\x54\x00\xd5\x30\xa9\x30\xfc\x30\xde\x30\xc3\x30\xc8\x30\x6e\x30\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\x6b\x30\x19\x6a\x96\x6e\x62\x5f\x0f\x5f\x6e\x30\xa4\x30\xe1\x30\xfc\x30\xb8\x30\x92\x30\xe2\x8e\x01\x90\x59\x30\x8b\x30\x53\x30\x68\x30\x6f\x30\x67\x30\x4d\x30\x7e\x30\x5b\x30\x93\x30\x0a\x00\x00"/*@ L"DXT�t�H�[�}�b�g�̃e�N�X�`���ɕW���`���̃C���[�W��]�����邱�Ƃ͂ł��܂���\n" @*/ ) ;
		}
	}

	// ����I��
	return 0 ;
}

static int Graphics_D3D9_BltBmpOrBaseImageToGraph3_NoMipMapBlt(
	      IMAGEDATA_ORIG			*Orig,
	const RECT						*SrcRect,
	const RECT						*DestRect,
	      int						UseSysMemSurface,
	      D_IDirect3DBaseTexture9	*UseTex,
	      int						TexWidth,
	      int						TexHeight,
	      int						TexUseWidth,
	      int						TexUseHeight,
	const BASEIMAGE					*RgbBaseImage,
	const BASEIMAGE					*AlphaBaseImage,
	const COLORDATA					*DestColor,
	      int						RedIsAlphaFlag,
	      int						UseTransColorConvAlpha,
	      int						ASyncThread
)
{
	int					i ;
	int					k ;
	int					hr ;
	POINT				DestPoint ;
	RECT				DestRectT ;
	int					H ;
	BYTE				*DestAddr ;
	BYTE				*SrcRgbAddr ;
	BYTE				*SrcAlphaAddr ;
	D_D3DLOCKED_RECT	LockRect ;
	int					RightLineFillFlag ;
	int					BottomLineFillFlag ;
	int					ImageNum ;
	DWORD				ImageSize ;
	DWORD				AlphaImageSize ;
	int					UseSysMemSurfaceIndex ;
	D_IDirect3DSurface9	*UseSurface = NULL ;

	DestPoint.x = 0 ;
	DestPoint.y = 0 ;

	RightLineFillFlag  = FALSE ;
	BottomLineFillFlag = FALSE ;
	DestRectT          = *DestRect ;
	if( TexUseWidth  < TexWidth  && TexUseWidth  == DestRectT.right )
	{
		RightLineFillFlag = TRUE ;
		DestRectT.right ++ ;
	}
	if( TexUseHeight < TexHeight && TexUseHeight == DestRectT.bottom )
	{
		BottomLineFillFlag = TRUE ;
		DestRectT.bottom ++ ;
	}

	ImageNum = Orig->FormatDesc.CubeMapTextureFlag ? CUBEMAP_SURFACE_NUM : 1 ;

//	DXST_ERRORLOGFMT_ADDUTF16LE(( L"SrcRect l:%d t:%d r:%d b:%d  DestRect l:%d t:%d r:%d b:%d",
//	SrcRect->left, SrcRect->top, SrcRect->right, SrcRect->bottom,
//	DestRectT.left, DestRectT.top, DestRectT.right, DestRectT.bottom )) ;

	for( i = 0 ; i < ImageNum ; i ++ )
	{
		// �]����̃��b�N
		if( UseSysMemSurface == FALSE )
		{
			if( Orig->FormatDesc.CubeMapTextureFlag )
			{
				hr = Direct3DCubeTexture9_LockRect_ASync( ( D_IDirect3DCubeTexture9 * )UseTex, ( D_D3DCUBEMAP_FACES )i, 0, &LockRect, &DestRectT, 0, ASyncThread ) ;
			}
			else
			{
				hr = Direct3DTexture9_LockRect_ASync( ( D_IDirect3DTexture9 * )UseTex, 0, &LockRect, &DestRectT, 0, ASyncThread ) ;
			}
			UseSysMemSurfaceIndex = -1 ;
		}
		else
		{
			for(;;)
			{
				UseSysMemSurfaceIndex = Graphics_D3D9_GetSysMemSurface( TexWidth, TexHeight, GD3D9.Device.Caps.TextureFormat[ Orig->ColorFormat ], ASyncThread ) ;
				if( UseSysMemSurfaceIndex != -1 ) break ;
				if( NS_ProcessMessage() != 0 ) break ;
				Thread_Sleep( 1 ) ;
			}
			if( UseSysMemSurfaceIndex == -1 )
			{
				return -1 ;
			}
			UseSurface = GD3D9.SysMemTexSurf.Surface[ UseSysMemSurfaceIndex ].MemSurface ;

			hr = Direct3DSurface9_LockRect_ASync( UseSurface, &LockRect, &DestRectT, D_D3DLOCK_DISCARD, ASyncThread ) ;
		}
		if( hr != D_D3D_OK )
		{
			DXST_ERRORLOG_ADDUTF16LE( "\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\x6e\x30\xed\x30\xc3\x30\xaf\x30\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�e�N�X�`���̃��b�N�Ɏ��s���܂���\n" @*/ ) ;
			return -1  ;
		}

		// �]������

		// �]���悪�e�N�X�`���S�̂ŁA����DXT�t�H�[�}�b�g�œ]�����DXT�t�H�[�}�b�g�̏ꍇ�͒P���ȃ������]�����s��
		if( DestRect->left == 0 && DestRect->right  == TexWidth  &&
			DestRect->top  == 0 && DestRect->bottom == TexHeight &&
			( ( RgbBaseImage->ColorData.Format == DX_BASEIMAGE_FORMAT_DXT1 && GD3D9.Device.Caps.TextureFormat[ Orig->ColorFormat ] == D_D3DFMT_DXT1 ) ||
			  ( RgbBaseImage->ColorData.Format == DX_BASEIMAGE_FORMAT_DXT2 && GD3D9.Device.Caps.TextureFormat[ Orig->ColorFormat ] == D_D3DFMT_DXT2 ) ||
			  ( RgbBaseImage->ColorData.Format == DX_BASEIMAGE_FORMAT_DXT3 && GD3D9.Device.Caps.TextureFormat[ Orig->ColorFormat ] == D_D3DFMT_DXT3 ) ||
			  ( RgbBaseImage->ColorData.Format == DX_BASEIMAGE_FORMAT_DXT4 && GD3D9.Device.Caps.TextureFormat[ Orig->ColorFormat ] == D_D3DFMT_DXT4 ) ||
			  ( RgbBaseImage->ColorData.Format == DX_BASEIMAGE_FORMAT_DXT5 && GD3D9.Device.Caps.TextureFormat[ Orig->ColorFormat ] == D_D3DFMT_DXT5 ) ) )
		{
			ImageSize = ( DWORD )( RgbBaseImage->Width * RgbBaseImage->Height * RgbBaseImage->ColorData.ColorBitDepth / 8 ) ;
			_MEMCPY( LockRect.pBits, ( BYTE * )RgbBaseImage->GraphData + ImageSize * i, ImageSize ) ;
		}
		else
		{
			// �]���悪�W���t�H�[�}�b�g�ł͂Ȃ��ꍇ�͓]���ł��Ȃ�
			if( GD3D9.Device.Caps.TextureFormat[ Orig->ColorFormat ] != D_D3DFMT_DXT1 &&
				GD3D9.Device.Caps.TextureFormat[ Orig->ColorFormat ] != D_D3DFMT_DXT2 &&
				GD3D9.Device.Caps.TextureFormat[ Orig->ColorFormat ] != D_D3DFMT_DXT3 &&
				GD3D9.Device.Caps.TextureFormat[ Orig->ColorFormat ] != D_D3DFMT_DXT4 &&
				GD3D9.Device.Caps.TextureFormat[ Orig->ColorFormat ] != D_D3DFMT_DXT5 )
			{
				ImageSize      = ( DWORD )( RgbBaseImage->Pitch   * RgbBaseImage->Height   ) ;
				AlphaImageSize = ( DWORD )( AlphaBaseImage->Pitch * AlphaBaseImage->Height ) ;

				SrcRgbAddr   = ( BYTE * )RgbBaseImage->GraphData   + i * ImageSize ;
				SrcAlphaAddr = ( BYTE * )AlphaBaseImage->GraphData + i * AlphaImageSize ;
				NS_GraphColorMatchBltVer2(
					LockRect.pBits, LockRect.Pitch,    DestColor,
					SrcRgbAddr,     RgbBaseImage->Pitch,   &RgbBaseImage->ColorData,
					SrcAlphaAddr,   AlphaBaseImage->Pitch, &AlphaBaseImage->ColorData,
					DestPoint, SrcRect, FALSE,
					UseTransColorConvAlpha && ( DestColor->AlphaWidth != 0 && AlphaBaseImage->GraphData == NULL ), Orig->TransCode,
					BASEIM.ImageShavedMode, FALSE,
					RedIsAlphaFlag, FALSE,
					FALSE ) ;

				if( BottomLineFillFlag )
				{
					_MEMCPY( ( BYTE * )LockRect.pBits + ( TexUseHeight - DestRect->top     ) * LockRect.Pitch,
							 ( BYTE * )LockRect.pBits + ( TexUseHeight - DestRect->top - 1 ) * LockRect.Pitch,
							 ( size_t )( DestColor->PixelByte * ( SrcRect->right - SrcRect->left ) ) ) ;
				}
				if( RightLineFillFlag )
				{
					H = SrcRect->bottom - SrcRect->top ;
					DestAddr = ( BYTE * )LockRect.pBits + ( TexUseWidth - DestRect->left ) * DestColor->PixelByte ;
					switch( DestColor->PixelByte )
					{
					case 2 :
						for( k = 0 ; k < H ; k ++, DestAddr += LockRect.Pitch )
						{
							*( ( WORD * )DestAddr ) = *( ( WORD * )DestAddr - 1 ) ;
						}
						break ;

					case 4 :
						for( k = 0 ; k < H ; k ++, DestAddr += LockRect.Pitch )
						{
							*( ( DWORD * )DestAddr ) = *( ( DWORD * )DestAddr - 1 ) ;
						}
						break ;
					}
				}
				if( BottomLineFillFlag && RightLineFillFlag )
				{
					DestAddr = ( BYTE * )LockRect.pBits + ( TexUseHeight - DestRect->top ) * LockRect.Pitch + ( TexUseWidth - DestRect->left ) * DestColor->PixelByte ;
					switch( DestColor->PixelByte )
					{
					case 2 : *( ( WORD  * )DestAddr ) = *( ( WORD  * )( DestAddr - LockRect.Pitch ) - 1 ) ; break ;
					case 4 : *( ( DWORD * )DestAddr ) = *( ( DWORD * )( DestAddr - LockRect.Pitch ) - 1 ) ;	break ;
					}
				}
			}
			else
			{
				DXST_ERRORLOG_ADDUTF16LE( "\x44\x00\x58\x00\x54\x00\xd5\x30\xa9\x30\xfc\x30\xde\x30\xc3\x30\xc8\x30\x6e\x30\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\x6b\x30\x19\x6a\x96\x6e\x62\x5f\x0f\x5f\x6e\x30\xa4\x30\xe1\x30\xfc\x30\xb8\x30\x92\x30\xe2\x8e\x01\x90\x59\x30\x8b\x30\x53\x30\x68\x30\x6f\x30\x67\x30\x4d\x30\x7e\x30\x5b\x30\x93\x30\x0a\x00\x00"/*@ L"DXT�t�H�[�}�b�g�̃e�N�X�`���ɕW���`���̃C���[�W��]�����邱�Ƃ͂ł��܂���\n" @*/ ) ;
			}
		}

		// ���b�N����
		if( UseSysMemSurface == FALSE )
		{
			if( Orig->FormatDesc.CubeMapTextureFlag )
			{
				Direct3DCubeTexture9_UnlockRect_ASync( ( D_IDirect3DCubeTexture9 * )UseTex, ( D_D3DCUBEMAP_FACES )i, 0, ASyncThread ) ;
			}
			else
			{
				Direct3DTexture9_UnlockRect_ASync( ( D_IDirect3DTexture9 * )UseTex, 0, ASyncThread ) ;
			}
		}
		else
		{
			D_IDirect3DSurface9 *DestSurface = NULL ;
			POINT                DestSurfacePoint ;

			Direct3DSurface9_UnlockRect_ASync( UseSurface, ASyncThread ) ;

			if( Orig->FormatDesc.CubeMapTextureFlag )
			{
				Direct3DCubeTexture9_GetCubeMapSurface_ASync( ( D_IDirect3DCubeTexture9 * )UseTex, ( D_D3DCUBEMAP_FACES )i, 0, &DestSurface, ASyncThread ) ;
			}
			else
			{
				Direct3DTexture9_GetSurfaceLevel_ASync( ( D_IDirect3DTexture9 * )UseTex, 0, &DestSurface, ASyncThread ) ;
			}

			DestSurfacePoint.x = DestRectT.left ;
			DestSurfacePoint.y = DestRectT.top ;
			Direct3DDevice9_UpdateSurface_ASync( UseSurface, &DestRectT, DestSurface, &DestSurfacePoint, ASyncThread ) ; 

			Direct3D9_ObjectRelease_ASync( DestSurface, ASyncThread ) ;

			Graphics_D3D9_ReleaseSysMemSurface( UseSysMemSurfaceIndex, ASyncThread ) ;
			UseSysMemSurfaceIndex = -1 ;
		}
	}

	// ����I��
	return 0 ;
}

// Direct3D9 �̃O���t�B�b�N�n���h���ɉ摜�f�[�^��]�����邽�߂̊֐�
extern	int		Graphics_Hardware_D3D9_BltBmpOrBaseImageToGraph3_PF(
	const RECT		*SrcRect,
	      int		DestX,
	      int		DestY,
	      int		GrHandle,
	const BASEIMAGE	*RgbBaseImage,
	const BASEIMAGE	*AlphaBaseImage,
	      int		RedIsAlphaFlag,
	      int		UseTransColorConvAlpha,
	      int		TargetOrig,
	      int		ASyncThread
)
{
	IMAGEDATA					*Image ;
	IMAGEDATA_ORIG				*Orig ;
	IMAGEDATA_ORIG_HARD_TEX	*OrigTex ;
	IMAGEDATA_HARD_DRAW		*DrawTex ;
	int							ImageNum ;
	int							UseSysMemTex = -1 ;
	int							i ;
	int							j ;
	POINT						DestPoint ;
	RECT						DestRect ;
	RECT						MoveRect ;
	COLORDATA					*DestColor ;
	D_D3DLOCKED_RECT			LockRect ;
	D_IDirect3DBaseTexture9		*UseTex ;
	D_IDirect3DSurface9			*SMSurface[ CUBEMAP_SURFACE_NUM ] = { 0 } ;
	D_IDirect3DTexture9			*SMTexture = NULL ;
	int							SrcWidth ;
	int							SrcHeight ;
	HRESULT						hr ;

	if( Direct3DDevice9_IsValid() == 0 )
	{
		return -1 ;
	}

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO_ASYNC( ASyncThread ) ;

	// �]�������v�Z���Ă���
	SrcWidth  = SrcRect->right  - SrcRect->left ;
	SrcHeight = SrcRect->bottom - SrcRect->top ;
	if( SrcWidth <= 0 || SrcHeight <= 0 )
	{
		return 0 ;
	}

	// �G���[����
	if( ASyncThread )
	{
		if( GRAPHCHK_ASYNC( GrHandle, Image ) )
		{
			return -1 ;
		}
	}
	else
	{
		if( GRAPHCHK( GrHandle, Image ) )
		{
			return -1 ;
		}
	}
	Orig = Image->Orig ;

	// �C���[�W�̐����Z�b�g
	ImageNum = RgbBaseImage->GraphDataCount == 0 ? 1 : RgbBaseImage->GraphDataCount ;

	// �e�N�X�`���̃J���[�����擾����
	DestColor = Graphics_D3D9_GetD3DFormatColorData( GD3D9.Device.Caps.TextureFormat[ Orig->ColorFormat ] ) ;

	// �]���ʒu�̃Z�b�g
	DestPoint.x = 0 ;
	DestPoint.y = 0 ;

	// �`��\�摜�̏ꍇ�͓]������摜���Ɠ����傫���̃V�X�e���������T�[�t�F�X�ɓ��e��]������
	if( Orig->FormatDesc.DrawValidFlag )
	{
		for( i = 0 ; i < ImageNum ; i ++ )
		{
			if( Direct3DDevice9_CreateOffscreenPlainSurface_ASync(
					( UINT )SrcWidth,
					( UINT )SrcHeight,
					GD3D9.Device.Caps.TextureFormat[ Orig->ColorFormat ],
					D_D3DPOOL_SYSTEMMEM,
					&SMSurface[ i ],
					NULL,
					ASyncThread
				) != D_D3D_OK )
			{
				break ;
			}

			// �V�X�e���������T�[�t�F�X�����b�N
			Direct3DSurface9_LockRect_ASync( SMSurface[ i ], &LockRect, NULL, D_D3DLOCK_DISCARD, ASyncThread ) ;

			// ���e��]��
			MoveRect.left   = 0 ;
			MoveRect.top    = 0 ;
			MoveRect.right  = SrcWidth ;
			MoveRect.bottom = SrcHeight ;
			NS_GraphColorMatchBltVer2(
				LockRect.pBits,                                                               LockRect.Pitch,    DestColor,
				( BYTE * )RgbBaseImage->GraphData   + RgbBaseImage->Pitch   * RgbBaseImage->Height   * i, RgbBaseImage->Pitch,   &RgbBaseImage->ColorData,
				( BYTE * )AlphaBaseImage->GraphData + AlphaBaseImage->Pitch * AlphaBaseImage->Height * i, AlphaBaseImage->Pitch, &AlphaBaseImage->ColorData,
				DestPoint, &MoveRect, FALSE,
				UseTransColorConvAlpha && ( DestColor->AlphaWidth != 0 && AlphaBaseImage->GraphData == NULL ), Orig->TransCode,
				BASEIM.ImageShavedMode, FALSE,
				RedIsAlphaFlag, FALSE,
				FALSE ) ;

			// �V�X�e���������T�[�t�F�X�̃��b�N������
			Direct3DSurface9_UnlockRect_ASync( SMSurface[ i ], ASyncThread ) ;
		}

		// �I�t�X�N���[���T�[�t�F�X�̍쐬�Ɏ��s������e�N�X�`���̍쐬�����݂�
		if( ImageNum != i )
		{
			// �L���[�u�}�b�v�╡���e�N�X�`���ɂ͔�Ή�
			if( ImageNum > 1 || Orig->Hard.TexNum > 1 )
			{
				DXST_ERRORLOG_ADDUTF16LE( "\x3b\x75\xcf\x50\xe2\x8e\x01\x90\x28\x75\x6e\x30\xb7\x30\xb9\x30\xc6\x30\xe0\x30\xe1\x30\xe2\x30\xea\x30\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�摜�]���p�̃V�X�e���������e�N�X�`���̍쐬�Ɏ��s���܂���\n" @*/ ) ;
				goto ERR ;
			}
			else
			{
				DWORD TexWidth ;
				DWORD TexHeight ;
				D_IDirect3DTexture9 *TargetTexture ;

				if( TargetOrig == TRUE )
				{
					TexWidth  = ( DWORD )Orig->Hard.Tex[ 0 ].TexWidth ;
					TexHeight = ( DWORD )Orig->Hard.Tex[ 0 ].TexHeight ;
					TargetTexture = Orig->Hard.Tex[ 0 ].PF->D3D9.Texture ;
				}
				else
				{
					TexWidth  = ( DWORD )Image->Hard.Draw[ 0 ].Tex->TexWidth ;
					TexHeight = ( DWORD )Image->Hard.Draw[ 0 ].Tex->TexHeight ;
					TargetTexture = Image->Hard.Draw[ 0 ].Tex->PF->D3D9.Texture ;
				}

				if( Direct3DDevice9_CreateTexture_ASync(
						TexWidth,
						TexHeight,
						1,
						0,
						GD3D9.Device.Caps.TextureFormat[ Orig->ColorFormat ],
						D_D3DPOOL_SYSTEMMEM,
						&SMTexture,
						NULL,
						ASyncThread ) != D_D3D_OK )
				{
					DXST_ERRORLOG_ADDUTF16LE( "\x3b\x75\xcf\x50\xe2\x8e\x01\x90\x28\x75\x6e\x30\xb7\x30\xb9\x30\xc6\x30\xe0\x30\xe1\x30\xe2\x30\xea\x30\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�摜�]���p�̃V�X�e���������e�N�X�`���̍쐬�Ɏ��s���܂���\n" @*/ ) ;
					goto ERR ;
				}

				// �V�X�e���������e�N�X�`�������b�N
				hr = Direct3DTexture9_LockRect_ASync( SMTexture, 0, &LockRect, NULL, D_D3DLOCK_DISCARD, ASyncThread ) ;

				// ���e��]��
				MoveRect.left   = 0 ;
				MoveRect.top    = 0 ;
				MoveRect.right  = SrcWidth ;
				MoveRect.bottom = SrcHeight ;
				NS_GraphColorMatchBltVer2(
					LockRect.pBits,                                                                                  LockRect.Pitch,                  DestColor,
					( BYTE * )RgbBaseImage->GraphData   + RgbBaseImage->Pitch   * RgbBaseImage->Height   * i,   RgbBaseImage->Pitch,   &RgbBaseImage->ColorData,
					( BYTE * )AlphaBaseImage->GraphData + AlphaBaseImage->Pitch * AlphaBaseImage->Height * i, AlphaBaseImage->Pitch, &AlphaBaseImage->ColorData,
					DestPoint, &MoveRect, FALSE,
					UseTransColorConvAlpha && ( DestColor->AlphaWidth != 0 && AlphaBaseImage->GraphData == NULL ), Orig->TransCode,
					BASEIM.ImageShavedMode, FALSE,
					RedIsAlphaFlag, FALSE,
					FALSE ) ;

				// �V�X�e���������e�N�X�`���̃��b�N������
				hr = Direct3DTexture9_UnlockRect_ASync( SMTexture, 0, ASyncThread ) ;

				// �e�N�X�`���̍X�V
				hr = Direct3DDevice9_UpdateTexture_ASync( SMTexture, TargetTexture, ASyncThread ) ;
				goto END ;
			}
		}
	}

	// �]���Ώۂ��I���W�i���摜�ɑ΂��Ă��ǂ����ŏ����𕪊�
	if( TargetOrig == TRUE )
	{
		// �I���W�i���摜�ɑ΂��Ă̏ꍇ

		// �e�N�X�`���̐������J��Ԃ�
		OrigTex = Orig->Hard.Tex ;
		for( i = 0 ; i < Orig->Hard.TexNum ; i ++, OrigTex ++ )
		{
			// �]����`�̍쐬
			if( Graphics_Image_BltBmpOrBaseImageToGraph3_Make_OrigTex_MoveRect(
					OrigTex,
					SrcRect,
					SrcWidth,
					SrcHeight,
					DestX,
					DestY,
					&DestRect,
					&MoveRect ) < 0 )
			{
				continue ;
			}

			// �`��\�摜���ǂ����ŏ����𕪊�
			if( Orig->FormatDesc.DrawValidFlag == TRUE )
			{
				// �`��\�摜�̏ꍇ

				// ���e��]��

				DestPoint.x = DestRect.left ;
				DestPoint.y = DestRect.top ;
				for( j = 0 ; j < ImageNum ; j ++ )
				{
					Direct3DDevice9_UpdateSurface_ASync(
						SMSurface[ j ],				&MoveRect,
						OrigTex->PF->D3D9.Surface[ j ],	&DestPoint, ASyncThread ) ;
				}
				if( OrigTex->PF->D3D9.RenderTargetSurface )
				{
					Direct3DDevice9_UpdateSurface_ASync(
						SMSurface[ 0 ],						&MoveRect,
						OrigTex->PF->D3D9.RenderTargetSurface,	&DestPoint, ASyncThread ) ;
				}
			}
			else
			{
				int FullUpdate ;

				// �`��s�\�摜�̏ꍇ

				// �S�̓]�����ǂ����𒲂ׂ�
				if( DestRect.right  - DestRect.left == OrigTex->TexWidth &&
					DestRect.bottom - DestRect.top  == OrigTex->TexHeight )
				{
					FullUpdate = TRUE ;
				}
				else
				{
					FullUpdate = FALSE ;
				}

				// �g�p����e�N�X�`���̃Z�b�g
				//UseTex = Orig->FormatDesc.UseManagedTextureFlag == FALSE ? OrigTex->MemTexture : OrigTex->PF->D3D9.Texture ;
				if( FullUpdate && Orig->FormatDesc.UseManagedTextureFlag == FALSE )
				{
					for(;;)
					{
						UseSysMemTex = Graphics_D3D9_GetSysMemTextureFromOrig( Orig, OrigTex, ASyncThread ) ;
						if( UseSysMemTex != -1 ) break ;
						if( NS_ProcessMessage() != 0 ) break ;
						Thread_Sleep( 1 ) ;
					}
					if( UseSysMemTex == -1 )
					{
						goto ERR ;
					}
					UseTex = GD3D9.SysMemTexSurf.Texture[ UseSysMemTex ].MemTexture ;
				}
				else
				{
					UseSysMemTex = -1 ;
					UseTex = OrigTex->PF->D3D9.Texture ;
				}

				// �~�b�v�}�b�v���ǂ����ŏ����𕪊�
				if( Orig->Hard.MipMapCount > 1 )
				{
					if( Graphics_D3D9_BltBmpOrBaseImageToGraph3_MipMapBlt(
							Orig,
							&MoveRect,
							&DestRect,
							FullUpdate == FALSE && Orig->FormatDesc.UseManagedTextureFlag == FALSE,
							UseTex,
							OrigTex->TexWidth,
							OrigTex->TexHeight,
							RgbBaseImage,
							AlphaBaseImage,
							DestColor,
							RedIsAlphaFlag,
							UseTransColorConvAlpha,
							ASyncThread ) < 0 )
						goto ERR ;
				}
				else
				{
					if( Graphics_D3D9_BltBmpOrBaseImageToGraph3_NoMipMapBlt(
							Orig,
							&MoveRect,
							&DestRect,
							FullUpdate == FALSE && Orig->FormatDesc.UseManagedTextureFlag == FALSE,
							UseTex,
							OrigTex->TexWidth,
							OrigTex->TexHeight,
							OrigTex->UseWidth,
							OrigTex->UseHeight,
							RgbBaseImage,
							AlphaBaseImage,
							DestColor,
							RedIsAlphaFlag,
							UseTransColorConvAlpha,
							ASyncThread ) < 0 )
						goto ERR ;
				}

				// �Ǘ��e�N�X�`���ł͂Ȃ��ꍇ�͂����œ]���p�̃V�X�e���������e�N�X�`�����������
				if( FullUpdate && Orig->FormatDesc.UseManagedTextureFlag == FALSE )
				{
					if( Orig->FormatDesc.CubeMapTextureFlag )
					{
						Direct3DDevice9_UpdateTexture_ASync( UseTex, OrigTex->PF->D3D9.CubeTexture, ASyncThread ) ;
					}
					else
					{
						Direct3DDevice9_UpdateTexture_ASync( UseTex, OrigTex->PF->D3D9.Texture, ASyncThread ) ;
					}
					Graphics_D3D9_ReleaseSysMemTexture( UseSysMemTex, ASyncThread ) ;
					UseSysMemTex = -1 ;
				}
			}
		}
	}
	else
	{
		// �I���W�i���摜�ɑ΂��Ăł͂Ȃ��ꍇ

		// �`����̐������J��Ԃ�
		DrawTex = Image->Hard.Draw ;
		DestPoint.x = 0 ;
		DestPoint.y = 0 ;
		for( i = 0 ; i < Image->Hard.DrawNum ; i ++, DrawTex ++ )
		{
			// �]����`�̍쐬
			if( Graphics_Image_BltBmpOrBaseImageToGraph3_Make_DrawTex_MoveRect(
					DrawTex,
					SrcRect,
					SrcWidth,
					SrcHeight,
					DestX,
					DestY,
					&DestRect,
					&MoveRect ) < 0 )
			{
				continue ;
			}

			// �`��\�摜���ǂ����ŏ����𕪊�
			if( Orig->FormatDesc.DrawValidFlag == TRUE )
			{
				// �`��\�摜�̏ꍇ

				// ���e��]��
				DestPoint.x = DestRect.left ;
				DestPoint.y = DestRect.top ;
				for( j = 0 ; j < ImageNum ; j ++ )
				{
					if( Direct3DDevice9_UpdateSurface_ASync(
							SMSurface[ j ],             &MoveRect,
							DrawTex->Tex->PF->D3D9.Surface[ j ], &DestPoint, ASyncThread ) != D_D3D_OK )
					{
						DXST_ERRORLOG_ADDUTF16LE( "\xcf\x63\x3b\x75\xef\x53\xfd\x80\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\x78\x30\x6e\x30\x3b\x75\xcf\x50\x6e\x30\xe2\x8e\x01\x90\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�`��\�e�N�X�`���ւ̉摜�̓]���Ɏ��s���܂���\n" @*/ ) ;
						goto ERR ;
					}
				}
			}
			else
			{
				int FullUpdate ;

				// �`��s�\�摜�̏ꍇ

				// �S�̓]�����ǂ����𒲂ׂ�
				if( DestRect.right  - DestRect.left == DrawTex->Tex->TexWidth &&
					DestRect.bottom - DestRect.top  == DrawTex->Tex->TexHeight )
				{
					FullUpdate = TRUE ;
				}
				else
				{
					FullUpdate = FALSE ;
				}

				// �g�p����e�N�X�`���̃Z�b�g
				//UseTex = Orig->FormatDesc.UseManagedTextureFlag == FALSE ? DrawTex->Tex->MemTexture : DrawTex->Tex->PF->D3D9.Texture ;
				if( FullUpdate && Orig->FormatDesc.UseManagedTextureFlag == FALSE )
				{
					for(;;)
					{
						UseSysMemTex = Graphics_D3D9_GetSysMemTextureFromOrig( Orig, DrawTex->Tex, ASyncThread ) ;
						if( UseSysMemTex       != -1 ) break ;
						if( NS_ProcessMessage() != 0 ) break ;
						Thread_Sleep( 1 ) ;
					}
					if( UseSysMemTex == -1 )
					{
						goto ERR ;
					}
					UseTex       = GD3D9.SysMemTexSurf.Texture[ UseSysMemTex ].MemTexture ;
				}
				else
				{
					UseTex       = DrawTex->Tex->PF->D3D9.Texture ;
					UseSysMemTex = -1 ;
				}

				// �~�b�v�}�b�v���ǂ����ŏ����𕪊�
				if( Orig->Hard.MipMapCount > 1 )
				{
					if( Graphics_D3D9_BltBmpOrBaseImageToGraph3_MipMapBlt(
							Orig,
							&MoveRect,
							&DestRect,
							FullUpdate == FALSE && Orig->FormatDesc.UseManagedTextureFlag == FALSE,
							UseTex,
							DrawTex->Tex->TexWidth,
							DrawTex->Tex->TexHeight,
							RgbBaseImage,
							AlphaBaseImage,
							DestColor,
							RedIsAlphaFlag,
							UseTransColorConvAlpha,
							ASyncThread ) < 0 )
						goto ERR ;
				}
				else
				{
					if( Graphics_D3D9_BltBmpOrBaseImageToGraph3_NoMipMapBlt(
							Orig,
							&MoveRect,
							&DestRect,
							FullUpdate == FALSE && Orig->FormatDesc.UseManagedTextureFlag == FALSE,
							UseTex,
							DrawTex->Tex->TexWidth,
							DrawTex->Tex->TexHeight,
							DrawTex->Tex->UseWidth,
							DrawTex->Tex->UseHeight,
							RgbBaseImage,
							AlphaBaseImage,
							DestColor,
							RedIsAlphaFlag,
							UseTransColorConvAlpha,
							ASyncThread ) < 0 )
						goto ERR ;
				}

				// �Ǘ��e�N�X�`���ł͂Ȃ��ꍇ�͂����œ]���p�̃V�X�e���������e�N�X�`�����������
				if( FullUpdate && Orig->FormatDesc.UseManagedTextureFlag == FALSE )
				{
					if( Orig->FormatDesc.CubeMapTextureFlag )
					{
						Direct3DDevice9_UpdateTexture_ASync( UseTex, DrawTex->Tex->PF->D3D9.CubeTexture, ASyncThread ) ;
					}
					else
					{
						Direct3DDevice9_UpdateTexture_ASync( UseTex, DrawTex->Tex->PF->D3D9.Texture, ASyncThread ) ;
					}
					Graphics_D3D9_ReleaseSysMemTexture( UseSysMemTex, ASyncThread ) ;
					UseSysMemTex = -1 ;
				}
			}
		}
	}

END :

	for( i = 0 ; i < CUBEMAP_SURFACE_NUM ; i ++ )
	{
		if( SMSurface[ i ] != NULL )
		{
			Direct3D9_ObjectRelease_ASync( SMSurface[ i ], ASyncThread ) ;
			SMSurface[ i ] = NULL ;
		}
	}

	if( SMTexture != NULL )
	{
		Direct3D9_ObjectRelease_ASync( SMTexture, ASyncThread ) ;
		SMTexture = NULL ;
	}

	if( UseSysMemTex != -1 )
	{
		Graphics_D3D9_ReleaseSysMemTexture( UseSysMemTex, ASyncThread ) ;
		UseSysMemTex = -1 ;
	}

	// ����I��
	return 0 ;


	// �G���[�I��
ERR :

	for( i = 0 ; i < CUBEMAP_SURFACE_NUM ; i ++ )
	{
		if( SMSurface[ i ] != NULL )
		{
			Direct3D9_ObjectRelease_ASync( SMSurface[ i ], ASyncThread ) ;
			SMSurface[ i ] = NULL ;
		}
	}

	if( SMTexture != NULL )
	{
		Direct3D9_ObjectRelease_ASync( SMTexture, ASyncThread ) ;
		SMTexture = NULL ;
	}

	if( UseSysMemTex != -1 )
	{
		Graphics_D3D9_ReleaseSysMemTexture( UseSysMemTex, ASyncThread ) ;
		UseSysMemTex = -1 ;
	}

	return -1 ;
}

// �I���W�i���摜��񒆂̃e�N�X�`�����쐬����( 0:����  -1:���s )
extern	int		Graphics_Hardware_D3D9_CreateOrigTexture_PF(  IMAGEDATA_ORIG *Orig, int ASyncThread )
{
	int i, j ;
	IMAGEDATA_ORIG_HARD_TEX *OrigTex ;

	// Direct3DDevice ��������������G���[
	if( Direct3DDevice9_IsValid() == 0 )
		return -1 ;

	// �n�[�h�E�G�A���g�p����摜�ł͂Ȃ��ꍇ�͉��������ɏI��
	if( Orig->FormatDesc.TextureFlag == FALSE ) return 0 ;

	// �w��̃t�H�[�}�b�g���쐬�ł��Ȃ��ꍇ�͎��s
	if( GD3D9.Device.Caps.TextureFormat[ Orig->ColorFormat ] == D_D3DFMT_UNKNOWN )
	{
		DXST_ERRORLOG_ADDUTF16LE( "\x07\x63\x9a\x5b\x6e\x30\xd5\x30\xa9\x30\xfc\x30\xde\x30\xc3\x30\xc8\x30\x67\x30\x5c\x4f\x10\x62\x67\x30\x4d\x30\x8b\x30\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\xd5\x30\xa9\x30\xfc\x30\xde\x30\xc3\x30\xc8\x30\x4c\x30\x42\x30\x8a\x30\x7e\x30\x5b\x30\x93\x30\x67\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�w��̃t�H�[�}�b�g�ō쐬�ł���e�N�X�`���t�H�[�}�b�g������܂���ł���\n" @*/ ) ;
		return -1 ;
	}

	// �e�N�X�`���̏ꍇ�̓e�N�X�`���̐������e�N�X�`�����쐬
	OrigTex = Orig->Hard.Tex ;
	for( i = 0 ; i < Orig->Hard.TexNum ; i ++, OrigTex ++ )
	{
		// ���Ƀe�N�X�`�����쐬����Ă�����X�L�b�v
//		if( OrigTex->PF->D3D9.Texture != NULL ) continue ;

		// Direct3D9Ex ���g�p���Ă���ꍇ�͊Ǘ��������g�p���Ȃ�( �ł��Ȃ� )
		if( Direct3D9_IsExObject() != 0 )
			Orig->FormatDesc.UseManagedTextureFlag = FALSE ;

		// �����_�����O�^�[�Q�b�g�T�[�t�F�X�̍쐬
		if( Orig->FormatDesc.DrawValidFlag )
		{
			// �L���[�u�}�b�v�ł͕ʓr�����_�[�^�[�Q�b�g�͍쐬���Ȃ�
			if( Orig->FormatDesc.CubeMapTextureFlag == FALSE )
			{
				D_D3DMULTISAMPLE_TYPE Samples = D_D3DMULTISAMPLE_NONE ;
				DWORD Quality = 0 ;

				if( Orig->FormatDesc.MSSamples != 0 )
				{
					Samples = ( D_D3DMULTISAMPLE_TYPE )Orig->FormatDesc.MSSamples ;
					Quality = ( DWORD )Orig->FormatDesc.MSQuality ;
					Direct3D9_CheckMultiSampleParam_ASync( GD3D9.Device.Caps.TextureFormat[ Orig->ColorFormat ], &Samples, &Quality, FALSE, ASyncThread ) ;
				}

				if( Samples != D_D3DMULTISAMPLE_NONE && OrigTex->PF->D3D9.RenderTargetSurface == NULL )
				{
					// �����_�����O�^�[�Q�b�g�T�[�t�F�X�̍쐬
					if( Direct3DDevice9_CreateRenderTarget_ASync(
						( UINT )OrigTex->TexWidth,								// ��
						( UINT )OrigTex->TexHeight,								// ����
						Orig->FormatDesc.PlatformTextureFormat != 0 ? g_DXD3D9FMT_to_D3DFORMAT[ Orig->FormatDesc.PlatformTextureFormat ] : GD3D9.Device.Caps.TextureFormat[ Orig->ColorFormat ],	// �t�H�[�}�b�g
						Samples,												// �}���`�T���v�����O�^�C�v
						( UINT )Quality,										// �}���`�T���v�����O�N�I���e�B
						FALSE,													// ���b�N�͂ł��Ȃ�
						&OrigTex->PF->D3D9.RenderTargetSurface,					// D_IDirect3DSurface9 �̃|�C���^���󂯎��A�h���X
						NULL,													// ��� NULL
						ASyncThread
						) != D_D3D_OK )
					{
						DXST_ERRORLOG_ADDUTF16LE( "\xf8\x66\x4d\x30\xbc\x8f\x7f\x30\x02\x5c\x28\x75\xb5\x30\xfc\x30\xd5\x30\xa7\x30\xb9\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�������ݐ�p�T�[�t�F�X�̍쐬�Ɏ��s���܂���\n" @*/ ) ;
						return -1 ;
					}
				}

				if( ( ( D_D3DMULTISAMPLE_TYPE )GSYS.Setting.FSAAMultiSampleCount == D_D3DMULTISAMPLE_NONE && Samples != D_D3DMULTISAMPLE_NONE ) ||
					( ( D_D3DMULTISAMPLE_TYPE )GSYS.Setting.FSAAMultiSampleCount != D_D3DMULTISAMPLE_NONE && Samples == D_D3DMULTISAMPLE_NONE ) ||
					Orig->ZBufferFlag  )
				{
					if( OrigTex->PF->D3D9.ZBuffer != NULL )
					{
						Direct3D9_ObjectRelease( OrigTex->PF->D3D9.ZBuffer ) ;
						OrigTex->PF->D3D9.ZBuffer = NULL ;
					}

					// �����_�����O�^�[�Q�b�g�[�x�o�b�t�@�̍쐬
					if( Direct3DDevice9_CreateDepthStencilSurface_ASync(
						( UINT )OrigTex->TexWidth,										// ��
						( UINT )OrigTex->TexHeight,										// ����
						GD3D9.Device.Caps.ZBufferFormat[ Orig->ZBufferBitDepthIndex ],	// �t�H�[�}�b�g
						Samples,														// �}���`�T���v�����O�^�C�v
						( UINT )Quality,												// �}���`�T���v�����O�N�I���e�B
						FALSE,															// ���b�N�͂ł��Ȃ�
						&OrigTex->PF->D3D9.ZBuffer,										// D_IDirect3DSurface9 �̃|�C���^���󂯎��A�h���X
						NULL,															// ��� NULL
						ASyncThread
						) != D_D3D_OK )
					{
						DXST_ERRORLOG_ADDUTF16LE( "\xf8\x66\x4d\x30\xbc\x8f\x7f\x30\x02\x5c\x28\x75\xde\x30\xeb\x30\xc1\x30\xb5\x30\xf3\x30\xd7\x30\xeb\x30\xf1\x6d\xa6\x5e\xb5\x30\xfc\x30\xd5\x30\xa7\x30\xb9\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�������ݐ�p�}���`�T���v���[�x�T�[�t�F�X�̍쐬�Ɏ��s���܂���\n" @*/ ) ;
						return -1 ;
					}
				}
				else
				if( OrigTex->PF->D3D9.ZBuffer != NULL )
				{
					Direct3D9_ObjectRelease( OrigTex->PF->D3D9.ZBuffer ) ;
					OrigTex->PF->D3D9.ZBuffer = NULL ;
				}
			}
		}

		// �r�f�I�������e�N�X�`���̍쐬
		if( Orig->FormatDesc.CubeMapTextureFlag )
		{
			if( OrigTex->PF->D3D9.CubeTexture == NULL )
			{
				if( Direct3DDevice9_CreateCubeTexture_ASync(
					( UINT )OrigTex->TexWidth,																											// �G�b�W�T�C�Y
					( UINT )Orig->Hard.MipMapCount,																										// MipMap ���x��
					( DWORD )( Orig->FormatDesc.DrawValidFlag ? D_D3DUSAGE_RENDERTARGET : 0 ),															// �t���O
					Orig->FormatDesc.PlatformTextureFormat != 0 ? g_DXD3D9FMT_to_D3DFORMAT[ Orig->FormatDesc.PlatformTextureFormat ] : GD3D9.Device.Caps.TextureFormat[ Orig->ColorFormat ],		// �t�H�[�}�b�g
					Orig->FormatDesc.UseManagedTextureFlag == FALSE || Orig->FormatDesc.DrawValidFlag ? D_D3DPOOL_DEFAULT : D_D3DPOOL_MANAGED,			// �e�N�X�`���� DirectX ���Ǘ����邩�ǂ���
					&OrigTex->PF->D3D9.CubeTexture,																										// D_IDirect3DTexture9 �̃|�C���^���󂯎��A�h���X
					NULL,																																// ��� NULL
					ASyncThread
					) != D_D3D_OK )
				{
					DXST_ERRORLOG_ADDUTF16LE( "\xad\x30\xe5\x30\xfc\x30\xd6\x30\xde\x30\xc3\x30\xd7\x30\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�L���[�u�}�b�v�e�N�X�`���̍쐬�Ɏ��s���܂���\n" @*/ ) ;
					return -1 ;
				}
			}
		}
		else
		{
			if( OrigTex->PF->D3D9.Texture == NULL )
			{
				if( Direct3DDevice9_CreateTexture_ASync(
					( UINT )OrigTex->TexWidth,																											// ��
					( UINT )OrigTex->TexHeight,																											// ����
					( UINT )Orig->Hard.MipMapCount,																										// MipMap ���x��
					( DWORD )( Orig->FormatDesc.DrawValidFlag ? D_D3DUSAGE_RENDERTARGET : 0 ),															// �t���O
					Orig->FormatDesc.PlatformTextureFormat != 0 ? g_DXD3D9FMT_to_D3DFORMAT[ Orig->FormatDesc.PlatformTextureFormat ] : GD3D9.Device.Caps.TextureFormat[ Orig->ColorFormat ],	// �t�H�[�}�b�g
					Orig->FormatDesc.UseManagedTextureFlag == FALSE || Orig->FormatDesc.DrawValidFlag ? D_D3DPOOL_DEFAULT : D_D3DPOOL_MANAGED,			// �e�N�X�`���� DirectX ���Ǘ����邩�ǂ���
					&OrigTex->PF->D3D9.Texture,																											// D_IDirect3DTexture9 �̃|�C���^���󂯎��A�h���X
					NULL,																																// ��� NULL
					ASyncThread
					) != D_D3D_OK )
				{
					DXST_ERRORLOG_ADDUTF16LE( "\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�e�N�X�`���̍쐬�Ɏ��s���܂���\n" @*/ ) ;
					return -1 ;
				}
			}
		}

		// �`��\�e�N�X�`���ł͂Ȃ��A���Ǘ��e�N�X�`���ł͂Ȃ��ꍇ�̓V�X�e���������e�N�X�`�����쐬
		/*if( ( Orig->FormatDesc.DrawValidFlag == FALSE && Orig->FormatDesc.UseManagedTextureFlag == FALSE ) && OrigTex->MemTexture == NULL )
		{
			if( Direct3DDevice9_CreateTexture(
				OrigTex->TexWidth,							// ��
				OrigTex->TexHeight,							// ����
				Orig->Hard.MipMapCount,						// MipMap ���x��
				D_D3DUSAGE_DYNAMIC,							// �t���O
				GD3D9.Device.Caps.TextureFormat[ Orig->ColorFormat ],		// �t�H�[�}�b�g
				D_D3DPOOL_SYSTEMMEM,						// �e�N�X�`���� DirectX ���Ǘ����邩�ǂ���
				&OrigTex->MemTexture,						// D_IDirect3DTexture9 �̃|�C���^���󂯎��A�h���X
				NULL										// ��� NULL
				) != D_D3D_OK )
			{
				Direct3D9_ObjectRelease( OrigTex->PF->D3D9.Texture ) ;
				OrigTex->PF->D3D9.Texture = NULL ;

				DXST_ERRORLOG_ADDUTF16LE( L"�������e�N�X�`���̍쐬�Ɏ��s���܂���\n" ) ;
				return -1 ;
			}
		}*/

		// �T�[�t�F�X�̃A�h���X���擾����
		if( Orig->FormatDesc.CubeMapTextureFlag )
		{
			for( j = 0 ; j < CUBEMAP_SURFACE_NUM ; j ++ )
			{
				if( OrigTex->PF->D3D9.Surface[ j ] == NULL )
				{
					if( Direct3DCubeTexture9_GetCubeMapSurface_ASync( OrigTex->PF->D3D9.CubeTexture, ( D_D3DCUBEMAP_FACES )j, 0, &OrigTex->PF->D3D9.Surface[ j ], ASyncThread ) != D_D3D_OK )
					{
						DXST_ERRORLOG_ADDUTF16LE( "\xad\x30\xe5\x30\xfc\x30\xd6\x30\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\x6e\x30\xb5\x30\xfc\x30\xd5\x30\xa7\x30\xb9\x30\xa2\x30\xc9\x30\xec\x30\xb9\x30\x6e\x30\xd6\x53\x97\x5f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�L���[�u�e�N�X�`���̃T�[�t�F�X�A�h���X�̎擾�Ɏ��s���܂���\n" @*/ ) ;
						return -1 ;
					}
				}
			}
		}
		else
		{
			if( OrigTex->PF->D3D9.Surface[ 0 ] == NULL )
			{
				if( Direct3DTexture9_GetSurfaceLevel_ASync( OrigTex->PF->D3D9.Texture, 0, &OrigTex->PF->D3D9.Surface[ 0 ], ASyncThread ) != D_D3D_OK )
				{
					DXST_ERRORLOG_ADDUTF16LE( "\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\x6e\x30\xb5\x30\xfc\x30\xd5\x30\xa7\x30\xb9\x30\xa2\x30\xc9\x30\xec\x30\xb9\x30\x6e\x30\xd6\x53\x97\x5f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�e�N�X�`���̃T�[�t�F�X�A�h���X�̎擾�Ɏ��s���܂���\n" @*/ ) ;
					return -1 ;
				}
			}
		}

		// ���łɐ^�����ɓh��Ԃ�
		{
//			int j, MipCount, h ;
//			D_D3DLOCKED_RECT LockRect ;

			if( Orig->FormatDesc.DrawValidFlag )
			{
				for( j = 0 ; j < CUBEMAP_SURFACE_NUM ; j ++ )
				{
					if( OrigTex->PF->D3D9.Surface[ j ] != NULL )
					{
						Direct3DDevice9_ColorFill_ASync( OrigTex->PF->D3D9.Surface[ j ], NULL, 0x00000000, ASyncThread ) ;
					}
				}
			}
/*
			else
			// DXT�t�H�[�}�b�g�̏ꍇ�̓N���A���Ȃ�
			if( Orig->ColorFormat != DX_GRAPHICSIMAGE_FORMAT_3D_DXT1 &&
				Orig->ColorFormat != DX_GRAPHICSIMAGE_FORMAT_3D_DXT2 &&
				Orig->ColorFormat != DX_GRAPHICSIMAGE_FORMAT_3D_DXT3 &&
				Orig->ColorFormat != DX_GRAPHICSIMAGE_FORMAT_3D_DXT4 &&
				Orig->ColorFormat != DX_GRAPHICSIMAGE_FORMAT_3D_DXT5 )
			{
				D_IDirect3DTexture9 *UseTex ;

				UseTex = Orig->FormatDesc.UseManagedTextureFlag == FALSE ? OrigTex->MemTexture : OrigTex->PF->D3D9.Texture ;
				h = OrigTex->TexHeight ;
				if( Orig->Hard.MipMapCount > 1 )
				{
					MipCount = Orig->Hard.MipMapCount ;
					for( j = 0 ; j < MipCount ; j ++, h >>= 1 )
					{
						if( UseTex->LockRect( j, &LockRect, NULL, 0 ) == D_D3D_OK )
						{
							_MEMSET( LockRect.pBits, 0, LockRect.Pitch * h ) ;

							UseTex->UnlockRect( j ) ;
						}
					}
				}
				else
				{
					if( UseTex->LockRect( 0, &LockRect, NULL, 0 ) == D_D3D_OK )
					{
						_MEMSET( LockRect.pBits, 0, LockRect.Pitch * h ) ;

						UseTex->UnlockRect( 0 ) ;
					}
				}

				if( Orig->FormatDesc.UseManagedTextureFlag == FALSE )
				{
					Direct3DDevice9_UpdateTexture( OrigTex->MemTexture, OrigTex->PF->D3D9.Texture ) ;
				}
			}
*/
		}
	}

	// �I��
	return 0 ;
}

// �I���W�i���摜��񒆂̃e�N�X�`�����������
extern	int		Graphics_Hardware_D3D9_ReleaseOrigTexture_PF( IMAGEDATA_ORIG *Orig )
{
	int i ;
	int j ;
	IMAGEDATA_ORIG_HARD_TEX *OrigTex ;

	// �n�[�h�E�G�A���g�p����摜�ł͂Ȃ��ꍇ�͉��������ɏI��
	if( Orig->FormatDesc.TextureFlag == FALSE ) return 0 ;

	// �e�N�X�`���̏ꍇ�̓e�N�X�`���̐������e�N�X�`�����쐬
	OrigTex = Orig->Hard.Tex ;
	for( i = 0 ; i < Orig->Hard.TexNum ; i ++, OrigTex ++ )
	{
		// �T�[�t�F�X�̉��
		for( j = 0 ; j < CUBEMAP_SURFACE_NUM ; j ++ )
		{
			if( OrigTex->PF->D3D9.Surface[ j ] )
			{
				Direct3D9_ObjectRelease( OrigTex->PF->D3D9.Surface[ j ] ) ;
				OrigTex->PF->D3D9.Surface[ j ] = NULL ;
			}
		}
		if( OrigTex->PF->D3D9.RenderTargetSurface )
		{
			Direct3D9_ObjectRelease( OrigTex->PF->D3D9.RenderTargetSurface ) ;
			OrigTex->PF->D3D9.RenderTargetSurface = NULL ;
		}
		if( OrigTex->PF->D3D9.ZBuffer )
		{
			Direct3D9_ObjectRelease( OrigTex->PF->D3D9.ZBuffer ) ;
			OrigTex->PF->D3D9.ZBuffer = NULL ;
		}

		// �e�N�X�`���̉��
		if( OrigTex->PF->D3D9.Texture )
		{
			Direct3D9_ObjectRelease( OrigTex->PF->D3D9.Texture ) ;
			OrigTex->PF->D3D9.Texture = NULL ;
		}
		
		/*if( OrigTex->MemTexture )
		{
			Direct3D9_ObjectRelease( OrigTex->MemTexture ) ;
			OrigTex->MemTexture = NULL ;
		}*/
	}

	// �I��
	return 0 ;
}

// �w��̃}���`�T���v�����Ŏg�p�ł���ő�N�I���e�B�l���擾����
extern	int		Graphics_Hardware_D3D9_GetMultiSampleQuality_PF( int Samples )
{
	IMAGEFORMATDESC				Format ;
	SETUP_GRAPHHANDLE_GPARAM	GParam ;
	int							FormatIndex ;
	D_D3DFORMAT					D3DFormat ;
	DWORD						Quality ;

	if( Samples > 16 )
	{
		Samples = 16 ;
	}

	if( Direct3D9_IsValid() == 0 )
	{
		return -1 ;
	}

	// ���݂̐ݒ�̃t�H�[�}�b�g���Z�b�g�A�b�v����
	Graphics_Image_InitSetupGraphHandleGParam( &GParam ) ; 
	GParam.DrawValidImageCreateFlag = TRUE ;
	GParam.CubeMapTextureCreateFlag = FALSE ;
	Graphics_Image_SetupFormatDesc( &Format, &GParam, 256, 256, GParam.AlphaChannelImageCreateFlag, FALSE, 0, DX_BASEIMAGE_FORMAT_NORMAL, -1 ) ;

	// �t�H�[�}�b�g�C���f�b�N�X�𓾂�
	FormatIndex = NS_GetTexFormatIndex( &Format ) ;

	// Direct3D �ł̃t�H�[�}�b�g���擾
	D3DFormat = GD3D9.Device.Caps.TextureFormat[ FormatIndex ] ;
	if( D3DFormat == D_D3DFMT_UNKNOWN )
	{
		return -1 ;
	}

	// �ݒ�ł���N�I���e�B���擾
	Quality = 10000 ;
	if( Direct3D9_CheckMultiSampleParam_ASync( D3DFormat, ( D_D3DMULTISAMPLE_TYPE * )&Samples, &Quality, TRUE ) < 0 )
	{
		return -1 ;
	}

	// �N�I���e�B��Ԃ�
	return ( int )Quality ;
}

// �쐬����O���t�B�b�N�n���h���Ŏg�p������ˑ��̃e�N�X�`���t�H�[�}�b�g���w�肷��
extern int Graphics_Hardware_D3D9_SetUsePlatformTextureFormat_PF( int PlatformTextureFormat )
{
	return 0 ;
}




























// ���ˑ����_�o�b�t�@�E�C���f�b�N�X�o�b�t�@�֌W

// ���_�o�b�t�@�n���h���̒��_�o�b�t�@���쐬����
extern	int		Graphics_Hardware_D3D9_VertexBuffer_Create_PF( VERTEXBUFFERHANDLEDATA *VertexBuffer )
{
	return Graphics_D3D9_VertexBuffer_CreateObject( VertexBuffer, FALSE ) ;
}

// ���_�o�b�t�@�n���h���̌�n��
extern	int		Graphics_Hardware_D3D9_VertexBuffer_Terminate_PF( VERTEXBUFFERHANDLEDATA *VertexBuffer )
{
	return Graphics_D3D9_VertexBuffer_ReleaseObject( VertexBuffer ) ;
}

// ���_�o�b�t�@�ɒ��_�f�[�^��]������
extern	int		Graphics_Hardware_D3D9_VertexBuffer_SetData_PF( VERTEXBUFFERHANDLEDATA *VertexBuffer, int SetIndex, const void *VertexData, int VertexNum )
{
	void *pBuffer ;

	// Direct3DVertexBuffer9 ���Ȃ��ꍇ�͂����ŏI��
	if( VertexBuffer->PF->D3D9.VertexBuffer == NULL )
	{
		return 0 ;
	}

	// �o�b�t�@�̃��b�N
	if( Direct3DVertexBuffer9_Lock(
			VertexBuffer->PF->D3D9.VertexBuffer, 
			( UINT )( SetIndex  * VertexBuffer->UnitSize ),
			( UINT )( VertexNum * VertexBuffer->UnitSize ),
			&pBuffer,
			0 ) != D_D3D_OK )
	{
		DXST_ERRORLOG_ADDUTF16LE( "\x02\x98\xb9\x70\xd0\x30\xc3\x30\xd5\x30\xa1\x30\x6e\x30\xed\x30\xc3\x30\xaf\x30\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"���_�o�b�t�@�̃��b�N�Ɏ��s���܂���\n" @*/ ) ;
		return -1 ;
	}

	// �o�b�t�@�փf�[�^��]��
	_MEMCPY( pBuffer, VertexData, ( size_t )( VertexNum * VertexBuffer->UnitSize ) ) ;

	// �o�b�t�@�̃A�����b�N
	Direct3DVertexBuffer9_Unlock( VertexBuffer->PF->D3D9.VertexBuffer ) ;

	// ����I��
	return 0 ;
}

// �C���f�b�N�X�o�b�t�@�n���h���̃Z�b�g�A�b�v���s��
extern	int		Graphics_Hardware_D3D9_IndexBuffer_Create_PF( INDEXBUFFERHANDLEDATA *IndexBuffer )
{
	return Graphics_D3D9_IndexBuffer_CreateObject( IndexBuffer, FALSE ) ;
}

// �C���f�b�N�X�o�b�t�@�n���h���̌�n��
extern	int		Graphics_Hardware_D3D9_IndexBuffer_Terminate_PF( INDEXBUFFERHANDLEDATA *IndexBuffer )
{
	return Graphics_D3D9_IndexBuffer_ReleaseObject( IndexBuffer ) ;
}

// �C���f�b�N�X�o�b�t�@�ɃC���f�b�N�X�f�[�^��]������
extern	int		Graphics_Hardware_D3D9_IndexBuffer_SetData_PF( INDEXBUFFERHANDLEDATA *IndexBuffer, int SetIndex, const void *IndexData, int IndexNum )
{
	void *pBuffer ;

	// Direct3DIndexBuffer9 �������ꍇ�͂����ŏI��
	if( IndexBuffer->PF->D3D9.IndexBuffer == NULL )
	{
		return 0 ;
	}

	// �o�b�t�@�̃��b�N
	if( Direct3DIndexBuffer9_Lock(
			IndexBuffer->PF->D3D9.IndexBuffer, 
			( UINT )( SetIndex * IndexBuffer->UnitSize ),
			( UINT )( IndexNum * IndexBuffer->UnitSize ),
			&pBuffer,
			0 ) != D_D3D_OK )
	{
		DXST_ERRORLOG_ADDUTF16LE( "\xa4\x30\xf3\x30\xc7\x30\xc3\x30\xaf\x30\xb9\x30\xd0\x30\xc3\x30\xd5\x30\xa1\x30\x6e\x30\xed\x30\xc3\x30\xaf\x30\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�C���f�b�N�X�o�b�t�@�̃��b�N�Ɏ��s���܂���\n" @*/ ) ;
		return -1 ;
	}

	// �o�b�t�@�փf�[�^��]��
	_MEMCPY( pBuffer, IndexData, ( size_t )( IndexNum * IndexBuffer->UnitSize ) ) ;

	// �o�b�t�@�̃A�����b�N
	Direct3DIndexBuffer9_Unlock( IndexBuffer->PF->D3D9.IndexBuffer ) ;

	// ����I��
	return 0 ;
}























// ���ˑ����C�g�֌W

// ���C�e�B���O���s�����ǂ�����ݒ肷��
extern	int		Graphics_Hardware_D3D9_Light_SetUse_PF( int Flag )
{
	return Graphics_D3D9_DeviceState_SetLighting( Flag ) ;
}

// �O���[�o���A���r�G���g���C�g�J���[��ݒ肷��
extern	int		Graphics_Hardware_D3D9_Light_GlobalAmbient_PF( COLOR_F *Color )
{
	return Graphics_D3D9_DeviceState_SetAmbient( D_RGBA_MAKE( _DTOL( Color->r * 255.0f ), _DTOL( Color->g * 255.0f ), _DTOL( Color->b * 255.0f ), 0 ) ) ;
}

// ���C�g�p�����[�^���Z�b�g
extern	int		Graphics_Hardware_D3D9_Light_SetState_PF( int LightNumber, LIGHTPARAM *LightParam )
{
	return Graphics_D3D9_DeviceState_SetLightState( LightNumber, ( D_D3DLIGHT9 * )LightParam ) ;
}

// ���C�g�̗L���A������ύX
extern	int		Graphics_Hardware_D3D9_Light_SetEnable_PF( int LightNumber, int EnableState )
{
	return Graphics_D3D9_DeviceState_SetLightEnable( LightNumber, EnableState ) ;
}




























// ���ˑ��V���h�E�}�b�v�֌W

// �V���h�E�}�b�v�f�[�^�ɕK�v�ȃe�N�X�`�����쐬����
extern	int		Graphics_Hardware_D3D9_ShadowMap_CreateTexture_PF( SHADOWMAPDATA *ShadowMap, int ASyncThread )
{
	// �t�H�[�}�b�g�̌���
	if( ShadowMap->TexFormat_Float )
	{
		if( ShadowMap->TexFormat_BitDepth == 16 )
		{
			ShadowMap->ColorFormat = /*DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_TWO_F16*/DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ONE_F16 ;
			ShadowMap->ZBufferFormat = ZBUFFER_FORMAT_16BIT ;
		}
		else
		{
			ShadowMap->ColorFormat = /*DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ABGR_F32*/DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ONE_F32 ;
			ShadowMap->ZBufferFormat = ZBUFFER_FORMAT_32BIT ;
		}
	}
	else
	{
		if( ShadowMap->TexFormat_BitDepth == 16 )
		{
			ShadowMap->ColorFormat = /*DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_TWO_I16*/ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ONE_I16 ;
			ShadowMap->ZBufferFormat = ZBUFFER_FORMAT_16BIT ;
		}
		else
		{
			ShadowMap->ColorFormat = DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ONE_F32 ;
			ShadowMap->ZBufferFormat = ZBUFFER_FORMAT_32BIT ;
		}
	}

	// ���肵���t�H�[�}�b�g���쐬�ł��Ȃ��ꍇ�͎��s
	if( GD3D9.Device.Caps.TextureFormat[ ShadowMap->ColorFormat ] == D_D3DFMT_UNKNOWN )
	{
		DXST_ERRORLOG_ADDUTF16LE( "\xb7\x30\xe3\x30\xc9\x30\xa6\x30\xde\x30\xc3\x30\xd7\x30\xc7\x30\xfc\x30\xbf\x30\x28\x75\x6e\x30\xd5\x30\xa9\x30\xfc\x30\xde\x30\xc3\x30\xc8\x30\x67\x30\x5c\x4f\x10\x62\x67\x30\x4d\x30\x8b\x30\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\xd5\x30\xa9\x30\xfc\x30\xde\x30\xc3\x30\xc8\x30\x4c\x30\x42\x30\x8a\x30\x7e\x30\x5b\x30\x93\x30\x67\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�V���h�E�}�b�v�f�[�^�p�̃t�H�[�}�b�g�ō쐬�ł���e�N�X�`���t�H�[�}�b�g������܂���ł���\n" @*/ ) ;
		return -1 ;
	}

	// �����_�����O�^�[�Q�b�g�[�x�o�b�t�@�̍쐬
	if( ShadowMap->PF->D3D9.ZBufferSurface == NULL )
	{
		if( Direct3DDevice9_CreateDepthStencilSurface_ASync(
			( UINT )ShadowMap->BaseSizeX,					// ��
			( UINT )ShadowMap->BaseSizeY,					// ����
			GD3D9.Device.Caps.ZBufferFormat[ ShadowMap->ZBufferFormat ],		// �t�H�[�}�b�g
			D_D3DMULTISAMPLE_NONE,							// �}���`�T���v�����O�^�C�v
			0,												// �}���`�T���v�����O�N�I���e�B
			FALSE,											// ���b�N�͂ł��Ȃ�
			&ShadowMap->PF->D3D9.ZBufferSurface,			// D_IDirect3DSurface9 �̃|�C���^���󂯎��A�h���X
			NULL,											// ��� NULL
			ASyncThread
			) != D_D3D_OK )
		{
			DXST_ERRORLOG_ADDUTF16LE( "\xb7\x30\xe3\x30\xc9\x30\xa6\x30\xde\x30\xc3\x30\xd7\x30\x28\x75\xf1\x6d\xa6\x5e\xb5\x30\xfc\x30\xd5\x30\xa7\x30\xb9\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�V���h�E�}�b�v�p�[�x�T�[�t�F�X�̍쐬�Ɏ��s���܂���\n" @*/ ) ;
			return -1 ;
		}
	}

	// �����_�[�^�[�Q�b�g�ɂł���e�N�X�`���̍쐬
	if( ShadowMap->PF->D3D9.Texture == NULL )
	{
		if( Direct3DDevice9_CreateTexture_ASync(
			( UINT )ShadowMap->BaseSizeX,					// ��
			( UINT )ShadowMap->BaseSizeY,					// ����
			1,												// MipMap ���x��
			D_D3DUSAGE_RENDERTARGET,						// �t���O
			GD3D9.Device.Caps.TextureFormat[ ShadowMap->ColorFormat ],		// �t�H�[�}�b�g
			D_D3DPOOL_DEFAULT,								// �e�N�X�`���� DirectX ���Ǘ����邩�ǂ���
			&ShadowMap->PF->D3D9.Texture,					// D_IDirect3DTexture9 �̃|�C���^���󂯎��A�h���X
			NULL,											// ��� NULL
			ASyncThread
			) != D_D3D_OK )
		{
			DXST_ERRORLOG_ADDUTF16LE( "\xb7\x30\xe3\x30\xc9\x30\xa6\x30\xde\x30\xc3\x30\xd7\x30\x28\x75\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�V���h�E�}�b�v�p�e�N�X�`���̍쐬�Ɏ��s���܂���\n" @*/ ) ;
			return -1 ;
		}

		// �e�N�X�`���̃T�[�t�F�X���擾���Ă���
		if( Direct3DTexture9_GetSurfaceLevel_ASync( ShadowMap->PF->D3D9.Texture, 0, &ShadowMap->PF->D3D9.Surface, ASyncThread ) != D_D3D_OK )
		{
			DXST_ERRORLOG_ADDUTF16LE( "\xb7\x30\xe3\x30\xc9\x30\xa6\x30\xde\x30\xc3\x30\xd7\x30\x28\x75\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\x6e\x30\xb5\x30\xfc\x30\xd5\x30\xa7\x30\xb9\x30\xa2\x30\xc9\x30\xec\x30\xb9\x30\x6e\x30\xd6\x53\x97\x5f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�V���h�E�}�b�v�p�e�N�X�`���̃T�[�t�F�X�A�h���X�̎擾�Ɏ��s���܂���\n" @*/ ) ;
			return -1 ;
		}
	}

	// �I��
	return 0 ;
}

// �V���h�E�}�b�v�f�[�^�ɕK�v�ȃe�N�X�`�����������
extern	int		Graphics_Hardware_D3D9_ShadowMap_ReleaseTexture_PF( SHADOWMAPDATA *ShadowMap )
{
	if( ShadowMap->PF->D3D9.ZBufferSurface != NULL )
	{
		Direct3D9_ObjectRelease( ShadowMap->PF->D3D9.ZBufferSurface ) ;
		ShadowMap->PF->D3D9.ZBufferSurface = NULL ;
	}

	if( ShadowMap->PF->D3D9.Surface != NULL )
	{
		Direct3D9_ObjectRelease( ShadowMap->PF->D3D9.Surface ) ;
		ShadowMap->PF->D3D9.Surface = NULL ;
	}

	if( ShadowMap->PF->D3D9.Texture != NULL )
	{
		Direct3D9_ObjectRelease( ShadowMap->PF->D3D9.Texture ) ;
		ShadowMap->PF->D3D9.Texture = NULL ;
	}

	// �I��
	return 0 ;
}

// ���_�V�F�[�_�[�ɐݒ肷��V���h�E�}�b�v�̏����X�V����
extern	void	Graphics_Hardware_D3D9_ShadowMap_RefreshVSParam_PF( void )
{
	SHADOWMAPDATA *ShadowMap ;
	float TranspMat[ 4 ][ 4 ] ;
	static float TranspMatDef[ 4 ][ 4 ] =
	{
		{ 0.0f, 0.0f, 0.0f, -1.0f },
		{ 0.0f, 0.0f, 0.0f, -1.0f },
		{ 0.0f, 0.0f, 0.0f, -1.0f },
		{ 0.0f, 0.0f, 0.0f, -1.0f },
	} ;
	int i ;

	for( i = 0 ; i < MAX_USE_SHADOWMAP_NUM ; i ++ )
	{
		// �A�h���X�̎擾
		if( SHADOWMAPCHKFULL( GSYS.DrawSetting.ShadowMap[ i ], ShadowMap ) )
		{
			Graphics_D3D9_ShaderConstant_InfoSet_SetParam(
				&GD3D9.Device.Shader.ShaderConstantInfo,
				DX_SHADERCONSTANTTYPE_VS_FLOAT,
				DX_SHADERCONSTANTSET_LIB,
				DX_VS_CONSTF_SHADOWMAP1_LIGHT_VIEW_PROJECTION_MAT + i * 4,
				TranspMatDef,
				4,
				TRUE
			) ;

		}
		else
		{
			// �s��̃Z�b�g
			TranspMat[ 0 ][ 0 ] = ShadowMap->ShadowMapViewProjectionMatrix.m[ 0 ][ 0 ] ;
			TranspMat[ 0 ][ 1 ] = ShadowMap->ShadowMapViewProjectionMatrix.m[ 1 ][ 0 ] ;
			TranspMat[ 0 ][ 2 ] = ShadowMap->ShadowMapViewProjectionMatrix.m[ 2 ][ 0 ] ;
			TranspMat[ 0 ][ 3 ] = ShadowMap->ShadowMapViewProjectionMatrix.m[ 3 ][ 0 ] ;

			TranspMat[ 1 ][ 0 ] = ShadowMap->ShadowMapViewProjectionMatrix.m[ 0 ][ 1 ] ;
			TranspMat[ 1 ][ 1 ] = ShadowMap->ShadowMapViewProjectionMatrix.m[ 1 ][ 1 ] ;
			TranspMat[ 1 ][ 2 ] = ShadowMap->ShadowMapViewProjectionMatrix.m[ 2 ][ 1 ] ;
			TranspMat[ 1 ][ 3 ] = ShadowMap->ShadowMapViewProjectionMatrix.m[ 3 ][ 1 ] ;

			TranspMat[ 2 ][ 0 ] = ShadowMap->ShadowMapViewProjectionMatrix.m[ 0 ][ 2 ] ;
			TranspMat[ 2 ][ 1 ] = ShadowMap->ShadowMapViewProjectionMatrix.m[ 1 ][ 2 ] ;
			TranspMat[ 2 ][ 2 ] = ShadowMap->ShadowMapViewProjectionMatrix.m[ 2 ][ 2 ] ;
			TranspMat[ 2 ][ 3 ] = ShadowMap->ShadowMapViewProjectionMatrix.m[ 3 ][ 2 ] ;

			TranspMat[ 3 ][ 0 ] = ShadowMap->ShadowMapViewProjectionMatrix.m[ 0 ][ 3 ] ;
			TranspMat[ 3 ][ 1 ] = ShadowMap->ShadowMapViewProjectionMatrix.m[ 1 ][ 3 ] ;
			TranspMat[ 3 ][ 2 ] = ShadowMap->ShadowMapViewProjectionMatrix.m[ 2 ][ 3 ] ;
			TranspMat[ 3 ][ 3 ] = ShadowMap->ShadowMapViewProjectionMatrix.m[ 3 ][ 3 ] ;

			Graphics_D3D9_ShaderConstant_InfoSet_SetParam(
				&GD3D9.Device.Shader.ShaderConstantInfo,
				DX_SHADERCONSTANTTYPE_VS_FLOAT,
				DX_SHADERCONSTANTSET_LIB,
				DX_VS_CONSTF_SHADOWMAP1_LIGHT_VIEW_PROJECTION_MAT + i * 4,
				TranspMat,
				4,
				TRUE
			) ;
		}
	}
}

// �s�N�Z���V�F�[�_�[�ɐݒ肷��V���h�E�}�b�v�̏����X�V����
extern	void	Graphics_Hardware_D3D9_ShadowMap_RefreshPSParam_PF( void )
{
	SHADOWMAPDATA *ShadowMap ;
	float ShadowMapParam[ 4 ][ 4 ] ;
	LIGHT_HANDLE *pLH ;
	static int ConstAddressTable[ MAX_USE_SHADOWMAP_NUM ][ 5 ][ 2 ] =
	{
		// ShadowMap1
		{
			{ 0, 0 },
			{ 0, 1 },
			{ 0, 2 },
			{ 0, 3 },
			{ 1, 0 },
		},

		// ShadowMap2
		{
			{ 1, 1 },
			{ 1, 2 },
			{ 1, 3 },
			{ 2, 0 },
			{ 2, 1 },
		},

		// ShadowMap3
		{
			{ 2, 2 },
			{ 2, 3 },
			{ 3, 0 },
			{ 3, 1 },
			{ 3, 2 },
		},
	} ;
	int CheckNum ;
	int i ;
	int j ;

	CheckNum = GSYS.Light.EnableNum > 3 /* DX_PS_CONSTB_LIGHT2_NUM */ ? 3 /* DX_PS_CONSTB_LIGHT2_NUM */ : GSYS.Light.EnableNum ;

	for( i = 0 ; i < MAX_USE_SHADOWMAP_NUM ; i ++ )
	{
		for( j = 0; j < 3; j++ )
		{
			ShadowMapParam[ ConstAddressTable[ i ][ j + 2 ][ 0 ] ][ ConstAddressTable[ i ][ j + 2 ][ 1 ] ] = 1.0f ;
		}

		// �A�h���X�̎擾
		if( SHADOWMAPCHKFULL( GSYS.DrawSetting.ShadowMap[ i ], ShadowMap ) )
		{
			ShadowMapParam[ ConstAddressTable[ i ][ 0 ][ 0 ] ][ ConstAddressTable[ i ][ 0 ][ 1 ] ] = 1.0f ;
			ShadowMapParam[ ConstAddressTable[ i ][ 1 ][ 0 ] ][ ConstAddressTable[ i ][ 1 ][ 1 ] ] = 0.0f ;
		}
		else
		{
			ShadowMapParam[ ConstAddressTable[ i ][ 0 ][ 0 ] ][ ConstAddressTable[ i ][ 0 ][ 1 ] ] = ShadowMap->AdjustDepth ;
			ShadowMapParam[ ConstAddressTable[ i ][ 1 ][ 0 ] ][ ConstAddressTable[ i ][ 1 ][ 1 ] ] = ShadowMap->GradationParam ;

			for( j = 0 ; j < CheckNum ; j ++ )
			{
				pLH = GSYS.Light.Data[ GSYS.Light.EnableHardwareIndex[ j ] ] ;
				if( pLH->ShadowMapSlotDisableFlag[ i ] == FALSE )
				{
					ShadowMapParam[ ConstAddressTable[ i ][ j + 2 ][ 0 ] ][ ConstAddressTable[ i ][ j + 2 ][ 1 ] ] = 0.0f ;
				}
			}
		}
	}

	Graphics_D3D9_ShaderConstant_InfoSet_SetParam(
		&GD3D9.Device.Shader.ShaderConstantInfo,
		DX_SHADERCONSTANTTYPE_PS_FLOAT,
		DX_SHADERCONSTANTSET_LIB,
		DX_PS_CONSTF_SHADOW1_DADJ_GRAD_ENBL0_1,
		ShadowMapParam,
		4,
		TRUE
	) ;
}

// �V���h�E�}�b�v�ւ̕`��̏������s��
extern	int		Graphics_Hardware_D3D9_ShadowMap_DrawSetup_PF( SHADOWMAPDATA * /* ShadowMap */ )
{
	// �g�p����V�F�[�_�[�̃C���f�b�N�X���X�V����
	Graphics_D3D9_Shader_RefreshUseIndex() ;

	// ����I��
	return 0 ;
}

// �V���h�E�}�b�v�ւ̕`����I������
extern	int		Graphics_Hardware_D3D9_ShadowMap_DrawEnd_PF( SHADOWMAPDATA * /* ShadowMap */ )
{
	// �g�p����V�F�[�_�[�̃C���f�b�N�X���X�V����
	Graphics_D3D9_Shader_RefreshUseIndex() ;

	// ����I��
	return 0 ;
}

// �`��Ŏg�p����V���h�E�}�b�v���w�肷��( �X���b�g�͂O���P�����w��\ )
extern	int		Graphics_Hardware_D3D9_ShadowMap_SetUse_PF( int /* SlotIndex */, SHADOWMAPDATA * /* ShadowMap */ )
{
	// �g�p����V�F�[�_�[�̃C���f�b�N�X���X�V����
	Graphics_D3D9_Shader_RefreshUseIndex() ;

	// ����I��
	return 0 ;
}































// ���ˑ��V�F�[�_�[�֌W

// �V�F�[�_�[�n���h��������������
extern	int		Graphics_Hardware_D3D9_Shader_Create_PF( int ShaderHandle, int IsVertexShader, void *Image, int ImageSize, int ImageAfterFree, int ASyncThread )
{
	SHADERHANDLEDATA *pShader ;
	D_IDirect3DPixelShader9 *PixelShader = NULL ;
	D_IDirect3DVertexShader9 *VertexShader = NULL ;

	// �G���[����
	if( ASyncThread )
	{
		if( SHADERCHK_ASYNC( ShaderHandle, pShader ) )
			return -1 ;
	}
	else
	{
		if( SHADERCHK( ShaderHandle, pShader ) )
			return -1 ;
	}

	// �V�F�[�_�[���쐬����
	if( IsVertexShader )
	{
		if( Direct3DDevice9_CreateVertexShader_ASync( ( DWORD * )Image, &VertexShader, ASyncThread ) != D_D3D_OK )
		{
			DXST_ERRORLOG_ADDUTF16LE( "\x02\x98\xb9\x70\xb7\x30\xa7\x30\xfc\x30\xc0\x30\xfc\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"���_�V�F�[�_�[�̍쐬�Ɏ��s���܂���\n" @*/ ) ;
			goto ERR ;
		}
	}
	else
	{
		if( Direct3DDevice9_CreatePixelShader_ASync( ( DWORD * )Image, &PixelShader, ASyncThread ) != D_D3D_OK )
		{
			DXST_ERRORLOG_ADDUTF16LE( "\xd4\x30\xaf\x30\xbb\x30\xeb\x30\xb7\x30\xa7\x30\xfc\x30\xc0\x30\xfc\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�s�N�Z���V�F�[�_�[�̍쐬�Ɏ��s���܂���\n" @*/ ) ;
			goto ERR ;
		}
	}

	// �o�C�i���C���[�W��ۑ����郁�����̈�̊m��
	pShader->FunctionCode = DXCALLOC( ( size_t )ImageSize ) ;
	if( pShader->FunctionCode == NULL )
	{
		DXST_ERRORLOG_ADDUTF16LE( "\xe1\x30\xe2\x30\xea\x30\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x00"/*@ L"�������̊m�ۂɎ��s���܂���" @*/ ) ;
		goto ERR ;
	}

	// �o�C�i���C���[�W��ۑ�����
	_MEMCPY( pShader->FunctionCode, Image, ( size_t )ImageSize ) ;

	// ���_�V�F�[�_�[���ǂ�����ۑ�����
	pShader->IsVertexShader = IsVertexShader ? TRUE : FALSE ;

	// �⏕��񂪂��邩�ǂ����𒲂ׂ�
	if( ( ( BYTE * )pShader->FunctionCode )[ 4 ] == 0xfe && ( ( BYTE * )pShader->FunctionCode )[ 5 ] == 0xff )
	{
		// ����ꍇ

		// �萔���z��̃A�h���X��ۑ�����
		pShader->PF->D3D9.ConstantInfo = ( D_D3DXSHADER_CONSTANTINFO * )( ( BYTE * )pShader->FunctionCode + 0x28 ) ;

		// �萔�̐���ۑ�����
		pShader->PF->D3D9.ConstantNum = *( ( int * )( ( BYTE * )pShader->FunctionCode + 0x18 ) ) ;
	}
	else
	{
		// �����ꍇ

		// �萔���z��̃A�h���X������������
		pShader->PF->D3D9.ConstantInfo = NULL ;

		// ���Ƃ��ăA�N�Z�X�ł���萔�͂O��
		pShader->PF->D3D9.ConstantNum = 0 ;
	}

	// �V�F�[�_�[�̃A�h���X��ۑ�
	if( IsVertexShader )
	{
		pShader->PF->D3D9.VertexShader = VertexShader ;
	}
	else
	{
		pShader->PF->D3D9.PixelShader = PixelShader ;
	}

	// ������ɉ�����ׂ��̃t���O�������Ă�����������
	if( ImageAfterFree )
	{
		DXFREE( Image ) ;
	}

	// ����I��
	return 0 ;

ERR :
	if( VertexShader )
	{
		Direct3D9_ObjectRelease_ASync( VertexShader, ASyncThread ) ;
	}

	if( PixelShader )
	{
		Direct3D9_ObjectRelease_ASync( PixelShader, ASyncThread ) ;
	}

	// ������ɉ�����ׂ��̃t���O�������Ă�����������
	if( ImageAfterFree )
	{
		DXFREE( Image ) ;
	}

	return -1 ;
}

// �V�F�[�_�[�n���h���̌�n��
extern	int		Graphics_Hardware_D3D9_Shader_TerminateHandle_PF( SHADERHANDLEDATA *Shader )
{
	// �V�F�[�_�[�̉��
	if( Shader->IsVertexShader )
	{
		if( Shader->PF->D3D9.VertexShader )
		{
			Direct3D9_ObjectRelease( Shader->PF->D3D9.VertexShader ) ;
			Shader->PF->D3D9.VertexShader = NULL ;
		}
	}
	else
	{
		if( Shader->PF->D3D9.PixelShader )
		{
			Direct3D9_ObjectRelease( Shader->PF->D3D9.PixelShader ) ;
			Shader->PF->D3D9.PixelShader = NULL ;
		}
	}

	// ����I��
	return 0 ;
}

// �g�p�ł���V�F�[�_�[�̃o�[�W�������擾����( 0=�g���Ȃ�  200=�V�F�[�_�[���f���Q�D�O���g�p�\  300=�V�F�[�_�[���f���R�D�O���g�p�\ )
extern	int		Graphics_Hardware_D3D9_Shader_GetValidShaderVersion_PF( void )
{
	D_D3DCAPS9 DevCaps ;

	// �V�F�[�_���g���Ȃ��ꍇ�͂O��Ԃ�
	if( GAPIWin.Direct3DDevice9Object != NULL && GSYS.HardInfo.UseShader == FALSE )
		return 0 ;

	// �\�͂��擾
	Direct3D9_GetDeviceCaps( ( DWORD )( GSYS.Screen.ValidUseDisplayIndex ? GSYS.Screen.UseDisplayIndex : D_D3DADAPTER_DEFAULT ), D_D3DDEVTYPE_HAL, &DevCaps ) ;

	if( ( DevCaps.VertexShaderVersion & 0xffff ) < 0x200 &&
		( DevCaps.PixelShaderVersion  & 0xffff ) < 0x200 )
	{
		return 0 ;
	}
	else
	if( ( DevCaps.VertexShaderVersion & 0xffff ) >= 0x300 &&
		( DevCaps.PixelShaderVersion  & 0xffff ) >= 0x300 )
	{
		return 300 ;
	}
	else
	{
		return 200 ;
	}
}

// �w��̖��O�����萔���g�p����V�F�[�_�[�萔�̔ԍ����擾����
extern	int		Graphics_Hardware_D3D9_Shader_GetConstIndex_PF( const wchar_t *ConstantName, SHADERHANDLEDATA *Shader )
{
	D_D3DXSHADER_CONSTANTINFO *Info ;

	// �w��̖��O�����萔��{��
	Info = Graphics_D3D9_GetShaderConstInfo( Shader, ConstantName ) ;

	// ����������G���[
	if( Info == NULL )
		return -1 ;

	// �g�p���Ă��郌�W�X�^�ԍ���Ԃ�
	return ( int )Info->RegisterIndex ;
}

// �w��̖��O�����萔���g�p����V�F�[�_�[�萔�̐����擾����
extern	int		Graphics_Hardware_D3D9_Shader_GetConstCount_PF( const wchar_t *ConstantName, SHADERHANDLEDATA *Shader )
{
	D_D3DXSHADER_CONSTANTINFO *Info ;

	// �w��̖��O�����萔��{��
	Info = Graphics_D3D9_GetShaderConstInfo( Shader, ConstantName ) ;

	// ����������G���[
	if( Info == NULL )
		return -1 ;

	// �g�p���Ă��郌�W�X�^����Ԃ�
	return ( int )Info->RegisterCount ;
}

// �w��̖��O�������������_�萔�̃f�t�H���g�p�����[�^���i�[����Ă��郁�����A�h���X���擾����
extern	const FLOAT4 *Graphics_Hardware_D3D9_Shader_GetConstDefaultParamF_PF( const wchar_t *ConstantName, SHADERHANDLEDATA *Shader )
{
	D_D3DXSHADER_CONSTANTINFO *Info ;

	// �w��̖��O�����萔��{��
	Info = Graphics_D3D9_GetShaderConstInfo( Shader, ConstantName ) ;

	// ����������G���[
	if( Info == NULL )
		return NULL ;

	// �����Ă��f�t�H���g�p�����[�^�������ꍇ�̓G���[
	if( Info->DefaultValue == 0 )
		return NULL ;

	// �f�t�H���g�p�����[�^�̂���ʒu��Ԃ�
	return ( FLOAT4 * )( ( char * )Shader->FunctionCode + Info->DefaultValue + 12 ) ;
}

// �V�F�[�_�[�萔����ݒ肷��
extern	int		Graphics_Hardware_D3D9_Shader_SetConst_PF( int TypeIndex, int SetIndex, int ConstantIndex, const void *Param, int ParamNum, int UpdateUseArea )
{
	// �萔�Z�b�g
	Graphics_D3D9_ShaderConstant_InfoSet_SetParam( &GD3D9.Device.Shader.ShaderConstantInfo, TypeIndex, SetIndex, ConstantIndex, Param, ParamNum, UpdateUseArea ) ;

	// ����I��
	return 0 ;
}

// �w��̈�̃V�F�[�_�[�萔�������Z�b�g����
extern	int		Graphics_Hardware_D3D9_Shader_ResetConst_PF( int TypeIndex, int SetIndex, int ConstantIndex, int ParamNum )
{
	// �萔���Z�b�g
	Graphics_D3D9_ShaderConstant_InfoSet_ResetParam( &GD3D9.Device.Shader.ShaderConstantInfo, TypeIndex, SetIndex, ConstantIndex, ParamNum ) ;

	// ����I��
	return 0 ;
}

#ifndef DX_NON_MODEL
// �R�c���f���p�̃V�F�[�_�[�R�[�h�̏��������s��
extern	int		Graphics_Hardware_D3D9_Shader_ModelCode_Init_PF( void )
{
	if( Graphics_D3D9_ShaderCode_Model_Initialize() == FALSE )
	{
		return -1 ;
	}

	// ����I��
	return 0 ;
}
#endif // DX_NON_MODEL






























// ���ˑ��V�F�[�_�[�p�萔�o�b�t�@�֌W

// �V�F�[�_�[�p�萔�o�b�t�@�n���h��������������
extern	int		Graphics_Hardware_D3D9_ShaderConstantBuffer_Create_PF( int ShaderConstantBufferHandle, int BufferSize, int ASyncThread )
{
	return -1 ;
}

// �V�F�[�_�[�p�萔�o�b�t�@�n���h���̌�n��
extern	int		Graphics_Hardware_D3D9_ShaderConstantBuffer_TerminateHandle_PF( SHADERCONSTANTBUFFERHANDLEDATA *ShaderConstantBuffer )
{
	return -1 ;
}

// �V�F�[�_�[�p�萔�o�b�t�@�n���h���̒萔�o�b�t�@�̃A�h���X���擾����
extern	void *	Graphics_Hardware_D3D9_ShaderConstantBuffer_GetBuffer_PF( SHADERCONSTANTBUFFERHANDLEDATA *ShaderConstantBuffer )
{
	return NULL ;
}

// �V�F�[�_�[�p�萔�o�b�t�@�n���h���̒萔�o�b�t�@�ւ̕ύX��K�p����
extern	int		Graphics_Hardware_D3D9_ShaderConstantBuffer_Update_PF( SHADERCONSTANTBUFFERHANDLEDATA *ShaderConstantBuffer )
{
	return -1 ;
}

// �V�F�[�_�[�p�萔�o�b�t�@�n���h���̒萔�o�b�t�@���w��̃V�F�[�_�[�̎w��̃X���b�g�ɃZ�b�g����
extern	int		Graphics_Hardware_D3D9_ShaderConstantBuffer_Set_PF( SHADERCONSTANTBUFFERHANDLEDATA *ShaderConstantBuffer, int TargetShader /* DX_SHADERTYPE_VERTEX �Ȃ� */, int Slot )
{
	return -1 ;
}


























// ���ˑ��`��֌W

// �w��_���狫�E�F������Ƃ���܂œh��Ԃ�
extern	int		Graphics_Hardware_D3D9_Paint_PF( int x, int y, unsigned int FillColor, ULONGLONG BoundaryColor )
{
	// �f�t�H���g�̏������s��
	return 2 ;

//	MEMIMG MemImg ;
//
//	if( GAPIWin.Direct3DDevice9Object == NULL ) return -1 ;
//
//	// �`��ҋ@���Ă���`�敨��`��
//	DRAWSTOCKINFO
//
//	// �`����I�������Ă���
//	Graphics_D3D9_EndScene() ;
//
//	// �t���X�N���[���A���`�G�C���A�X���g�p���Ă��邩�ǂ����ŏ����𕪊�
//	//if( ( D_D3DMULTISAMPLE_TYPE )GSYS.Setting.FSAAMultiSampleCount != D_D3DMULTISAMPLE_NONE )
//	if( true )
//	{
//		BASEIMAGE ScreenImage ;
//		int       DrawMode ;
//		int       DrawBlendMode ;
//		int       DrawBlendParam ;
//		RGBCOLOR  DrawBright ;
//		RECT      DrawArea ;
//
//		// ��ʂ̃C���[�W���擾
//		NS_CreateXRGB8ColorBaseImage( GSYS.DrawSetting.DrawSizeX, GSYS.DrawSetting.DrawSizeY, &ScreenImage ) ;
//		NS_GetDrawScreenBaseImage( 0, 0, GSYS.DrawSetting.DrawSizeX, GSYS.DrawSetting.DrawSizeY, &ScreenImage ) ;
//
//		// Paint �����p�� MEMIMG ���Z�b�g�A�b�v����
//		_MEMSET( &MemImg, 0, sizeof( MEMIMG ) ) ;
//		InitializeMemImg(
//			&MemImg,
//			GSYS.DrawSetting.DrawSizeX,
//			GSYS.DrawSetting.DrawSizeY,
//			ScreenImage.Pitch,
//			0,
//			1,
//			FALSE, FALSE, FALSE, ScreenImage.GraphData ) ;
//
//		// Paint �������s��
//		PaintMemImg( &MemImg, x, y, FillColor, BoundaryColor ) ;
//
//		// MEMIMG �̌�n��
//		TerminateMemImg( &MemImg ) ;
//
//		// ��ʂɃy�C���g��̉摜��`��
//		DrawBlendMode  = GSYS.DrawSetting.BlendMode ;
//		DrawBlendParam = GSYS.DrawSetting.BlendParam ;
//		DrawMode       = GSYS.DrawSetting.DrawMode ;
//		DrawBright     = GSYS.DrawSetting.DrawBright ;
//		DrawArea       = GSYS.DrawSetting.DrawArea ;
//
//		NS_SetDrawBlendMode( DX_BLENDMODE_SRCCOLOR, 255 ) ;
//		NS_SetDrawMode( DX_DRAWMODE_NEAREST ) ;
//		NS_SetDrawBright( 255, 255, 255 ) ;
//		NS_SetDrawArea( 0, 0, GSYS.DrawSetting.DrawSizeX, GSYS.DrawSetting.DrawSizeY ) ;
//
//		NS_DrawBaseImage( 0, 0, &ScreenImage ) ;
//
//		NS_SetDrawBlendMode( DrawBlendMode, DrawBlendParam ) ;
//		NS_SetDrawMode( DrawMode ) ;
//		NS_SetDrawBright( DrawBright.Red, DrawBright.Green, DrawBright.Blue ) ;
//		NS_SetDrawArea( DrawArea.left, DrawArea.top, DrawArea.right, DrawArea.bottom ) ;
//
//		NS_ReleaseBaseImage( &ScreenImage ) ;
//	}
//	else
//	{
//		D_D3DLOCKED_RECT     LockRect ;
//		COLORDATA           *ColorData ;
//		D_D3DFORMAT          TargetFormat ;
//		D_IDirect3DSurface9 *TargetSurface, *SMSurface ;
//		int                  Width ;
//		int                  Height ;
//		IMAGEDATA           *Image ;
//
//		// �G���[����
//		if( GRAPHCHKFULL( GSYS.DrawSetting.TargetScreen[ 0 ], Image ) )
//			Image = NULL ;
//
//		// �`���ɂ���ď����𕪊�
//		if( Image )
//		{
//			// �`��悪�`��\�T�[�t�F�X�̏ꍇ
//			TargetSurface = Image->Orig->Hard.Tex[ 0 ].PF->D3D9.Surface[ GSYS.DrawSetting.TargetScreenSurface[ 0 ] ] ;
//			TargetFormat  = GD3D9.Device.Caps.TextureFormat[ Image->Orig->ColorFormat ] ;
//			Width         = Image->Orig->Hard.Tex[ 0 ].TexWidth ;
//			Height        = Image->Orig->Hard.Tex[ 0 ].TexHeight ;
//		}
//		else
//		{
//			// �`��悪�T�u�o�b�N�o�b�t�@���̓o�b�N�o�b�t�@�̏ꍇ
//			TargetSurface = GD3D9.Device.Screen.SubBackBufferSurface ? GD3D9.Device.Screen.SubBackBufferSurface : GD3D9.Device.Screen.BackBufferSurface ; 
//			TargetFormat  = GD3D9.Device.Caps.ScreenFormat ;
//			Width         = GSYS.Screen.MainScreenSizeX ;
//			Height        = GSYS.Screen.MainScreenSizeY ;
//		}
//
//		ColorData = Graphics_D3D9_GetD3DFormatColorData( TargetFormat ) ;
//
//		// �`���̃R�s�[���擾����V�X�e���������T�[�t�F�X���쐬
//		if( Direct3DDevice9_CreateOffscreenPlainSurface(
//				( UINT )Width, ( UINT )Height, TargetFormat, D_D3DPOOL_SYSTEMMEM, &SMSurface, NULL ) != D_D3D_OK )
//		{
//			DXST_ERRORLOG_ADDUTF16LE( L"Paint �p�V�X�e���������T�[�t�F�X�̍쐬�Ɏ��s���܂���\n" ) ;
//			return -1 ;
//		}
//
//		// �V�X�e���������T�[�t�F�X�ɕ`���̓��e��]������
//		Direct3DDevice9_GetRenderTargetData(
//			TargetSurface, SMSurface ) ;
//
//		// �V�X�e���������T�[�t�F�X�����b�N
//		Direct3DSurface9_LockRect( SMSurface, &LockRect, NULL, D_D3DLOCK_READONLY ) ; 
//
//		// Paint �����p�� MEMIMG ���Z�b�g�A�b�v����
//		_MEMSET( &MemImg, 0, sizeof( MEMIMG ) ) ;
//		InitializeMemImg(
//			&MemImg,
//			GSYS.DrawSetting.DrawSizeX,
//			GSYS.DrawSetting.DrawSizeY,
//			LockRect.Pitch,
//			0,
//			ColorData->ColorBitDepth == 16 ? 0 : 1,
//			FALSE, FALSE, FALSE, LockRect.pBits ) ;
//
//		// Paint �������s��
//		PaintMemImg( &MemImg, x, y, FillColor, BoundaryColor ) ;
//
//		// MEMIMG �̌�n��
//		TerminateMemImg( &MemImg ) ;
//
//		// �V�X�e���������T�[�t�F�X�̃��b�N������
//		Direct3DSurface9_UnlockRect( SMSurface ) ;
//
//		// �V�X�e���������T�[�t�F�X�̓��e��`���ɓ]������
//		Direct3DDevice9_UpdateSurface(
//			SMSurface,     NULL,
//			TargetSurface, NULL ) ;
//
//		// �V�X�e���������T�[�t�F�X�̉��
//		Direct3D9_ObjectRelease( SMSurface ) ;
//	}
//
//	// ����I��
//	return 0 ;
}


























#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

#endif // DX_NON_DIRECT3D9

#endif // DX_NON_GRAPHICS


