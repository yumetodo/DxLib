//-----------------------------------------------------------------------------
// 
// 		�c�w���C�u����		�`�揈���v���O����( Direct3D11 )
// 
//  	Ver 3.16d
// 
//-----------------------------------------------------------------------------

#define VERTEXBUFFERMODE		2

// �c�w���C�u�����쐬���p��`
#define __DX_MAKE

#include "../DxCompileConfig.h"

#ifndef DX_NON_GRAPHICS

#ifndef DX_NON_DIRECT3D11

// �C���N���[�h ---------------------------------------------------------------
#include "DxGraphicsD3D11.h"
#include "DxGraphicsWin.h"
#include "DxMaskD3D11.h"
#include "DxWindow.h"
#include "DxWinAPI.h"
#include "DxGUID.h"
#include "../DxSystem.h"
#include "../DxLog.h"
#include "../DxModel.h"
#include "../DxMovie.h"
#include "../DxMask.h"
#include "../DxMath.h"
#include "../DxBaseFunc.h"
#include "../DxGraphics.h"
#include "../DxGraphicsFilter.h"
#include "../DxInput.h"
#include "../DxASyncLoad.h"

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// �}�N����` -----------------------------------------------------------------

// Graphics_D3D11_DrawPreparation ���ĂԒ�^��
#define DX_D3D11_DRAWPREP_NOTEX( FLAG )													\
	if( GD3D11.Device.DrawSetting.RenderTexture != NULL )								\
		Graphics_D3D11_DrawSetting_SetTexture( NULL, NULL ) ;							\
	if( GSYS.ChangeSettingFlag ||														\
		GD3D11.Device.DrawSetting.DrawPrepAlwaysFlag != FALSE ||						\
		GD3D11.Device.DrawSetting.DrawPrepParamFlag  != FLAG )							\
		Graphics_D3D11_DrawPreparation( FLAG ) ;												


#define DX_D3D11_DRAWPREP_TEX( ORIG, TEX, TEXSRV, FLAG )								\
	FLAG |= DX_D3D11_DRAWPREP_TEXTURE ;													\
	if( (ORIG)->FormatDesc.AlphaChFlag   ) FLAG |= DX_D3D11_DRAWPREP_TEXALPHACH ;		\
	if( (ORIG)->FormatDesc.AlphaTestFlag ) FLAG |= DX_D3D11_DRAWPREP_TEXALPHATEST ;		\
	if( GD3D11.Device.DrawSetting.RenderTexture != (TEX) )								\
		Graphics_D3D11_DrawSetting_SetTexture( (TEX), (TEXSRV) ) ;						\
	if( GSYS.ChangeSettingFlag ||														\
		GD3D11.Device.DrawSetting.DrawPrepAlwaysFlag != FALSE ||						\
		GD3D11.Device.DrawSetting.DrawPrepParamFlag  != FLAG )							\
		Graphics_D3D11_DrawPreparation( FLAG ) ;


#define NORMALVERTEXBUFFER_MAP	\
	if( GD3D11.Device.DrawInfo.VertexBufferAddr == NULL )\
	{\
		D_D3D11_MAPPED_SUBRESOURCE MappedSubResource ;\
		HRESULT                    hr ;\
\
		hr = D3D11DeviceContext_Map_ASync( \
			GD3D11.Device.DrawInfo.VertexBuffer,\
			0,\
			D_D3D11_MAP_WRITE_DISCARD,\
			0,\
			&MappedSubResource\
		) ;\
\
		GD3D11.Device.DrawInfo.VertexBufferAddr = ( BYTE * )MappedSubResource.pData ;\
	}

#define NORMALVERTEXBUFFER_UNMAP	\
	if( GD3D11.Device.DrawInfo.VertexBufferAddr != NULL )\
	{\
		D3D11DeviceContext_Unmap_ASync(\
			GD3D11.Device.DrawInfo.VertexBuffer,\
			0\
		) ;\
		GD3D11.Device.DrawInfo.VertexBufferAddr = NULL ;\
	}

#define GETVERTEX_QUAD( p )		\
	MV1DRAWPACKDRAWMODEL		\
	if(\
		  GD3D11.Device.DrawInfo.Use3DVertex != 0 ||\
		  GD3D11.Device.DrawInfo.PrimitiveType != D_D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST ||\
		  GD3D11.Device.DrawInfo.VertexNum + 6 >= D3D11_VertexBuffer_MaxVertexNum[ D3D11_VertexTypeToInputLayout[ GD3D11.Device.DrawInfo.Use3DVertex ][ GD3D11.Device.DrawInfo.VertexType ] ]\
	  )\
	{\
		Graphics_D3D11_RenderVertex( 0 ) ;\
		GD3D11.Device.DrawInfo.PrimitiveType = D_D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST ;\
	}\
	*( ( void ** )&p ) = ( void * )GD3D11.Device.DrawInfo.VertexBufferNextAddr ;

#define GETVERTEX_BILLBOARD( p )		\
	MV1DRAWPACKDRAWMODEL		\
	if(\
		  GD3D11.Device.DrawInfo.Use3DVertex != 1 ||\
		  GD3D11.Device.DrawInfo.PrimitiveType != D_D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST ||\
		  GD3D11.Device.DrawInfo.VertexNum + 6 >= D3D11_VertexBuffer_MaxVertexNum[ D3D11_VertexTypeToInputLayout[ GD3D11.Device.DrawInfo.Use3DVertex ][ GD3D11.Device.DrawInfo.VertexType ] ]\
	  )\
	{\
		Graphics_D3D11_RenderVertex( 1 ) ;\
		GD3D11.Device.DrawInfo.PrimitiveType = D_D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST ;\
	}\
	*( ( void ** )&p ) = ( void * )GD3D11.Device.DrawInfo.VertexBufferNextAddr ;

#define GETVERTEX_TRIANGLE( p )		\
	MV1DRAWPACKDRAWMODEL		\
	if(\
		  GD3D11.Device.DrawInfo.Use3DVertex != 0 ||\
		  GD3D11.Device.DrawInfo.PrimitiveType != D_D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST ||\
		  GD3D11.Device.DrawInfo.VertexNum + 3 >= D3D11_VertexBuffer_MaxVertexNum[ D3D11_VertexTypeToInputLayout[ GD3D11.Device.DrawInfo.Use3DVertex ][ GD3D11.Device.DrawInfo.VertexType ] ]\
	  )\
	{\
		Graphics_D3D11_RenderVertex( 0 ) ;\
		GD3D11.Device.DrawInfo.PrimitiveType = D_D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST ;\
	}\
	*( ( void ** )&p ) = ( void * )GD3D11.Device.DrawInfo.VertexBufferNextAddr ;

#define GETVERTEX_TRIANGLE3D( p )		\
	MV1DRAWPACKDRAWMODEL		\
	if(\
		  GD3D11.Device.DrawInfo.Use3DVertex != 1 ||\
		  GD3D11.Device.DrawInfo.PrimitiveType != D_D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST ||\
		  GD3D11.Device.DrawInfo.VertexNum + 3 >= D3D11_VertexBuffer_MaxVertexNum[ D3D11_VertexTypeToInputLayout[ GD3D11.Device.DrawInfo.Use3DVertex ][ GD3D11.Device.DrawInfo.VertexType ] ]\
	  )\
	{\
		Graphics_D3D11_RenderVertex( 1 ) ;\
		GD3D11.Device.DrawInfo.PrimitiveType = D_D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST ;\
	}\
	*( ( void ** )&p ) = ( void * )GD3D11.Device.DrawInfo.VertexBufferNextAddr ;

#define GETVERTEX_LINEBOX( p )		\
	MV1DRAWPACKDRAWMODEL		\
	if(\
		  GD3D11.Device.DrawInfo.Use3DVertex != 0 ||\
		  GD3D11.Device.DrawInfo.PrimitiveType != D_D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST ||\
		  GD3D11.Device.DrawInfo.VertexNum + 24 >= D3D11_VertexBuffer_MaxVertexNum[ D3D11_VertexTypeToInputLayout[ GD3D11.Device.DrawInfo.Use3DVertex ][ GD3D11.Device.DrawInfo.VertexType ] ]\
	  )\
	{\
		Graphics_D3D11_RenderVertex( 0 ) ;\
		GD3D11.Device.DrawInfo.PrimitiveType = D_D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST ;\
	}\
	*( ( void ** )&p ) = ( void * )GD3D11.Device.DrawInfo.VertexBufferNextAddr ;

#define GETVERTEX_LINETRIANGLE( p )		\
	MV1DRAWPACKDRAWMODEL		\
	if(\
		  GD3D11.Device.DrawInfo.Use3DVertex != 0 ||\
		  GD3D11.Device.DrawInfo.PrimitiveType != D_D3D11_PRIMITIVE_TOPOLOGY_LINELIST ||\
		  GD3D11.Device.DrawInfo.VertexNum + 6 >= D3D11_VertexBuffer_MaxVertexNum[ D3D11_VertexTypeToInputLayout[ GD3D11.Device.DrawInfo.Use3DVertex ][ GD3D11.Device.DrawInfo.VertexType ] ]\
	  )\
	{\
		Graphics_D3D11_RenderVertex( 0 ) ;\
		GD3D11.Device.DrawInfo.PrimitiveType = D_D3D11_PRIMITIVE_TOPOLOGY_LINELIST ;\
	}\
	*( ( void ** )&p ) = ( void * )GD3D11.Device.DrawInfo.VertexBufferNextAddr ;

#define GETVERTEX_LINETRIANGLE3D( p )		\
	MV1DRAWPACKDRAWMODEL		\
	if(\
		  GD3D11.Device.DrawInfo.Use3DVertex != 1 ||\
		  GD3D11.Device.DrawInfo.PrimitiveType != D_D3D11_PRIMITIVE_TOPOLOGY_LINELIST ||\
		  GD3D11.Device.DrawInfo.VertexNum + 6 >= D3D11_VertexBuffer_MaxVertexNum[ D3D11_VertexTypeToInputLayout[ GD3D11.Device.DrawInfo.Use3DVertex ][ GD3D11.Device.DrawInfo.VertexType ] ]\
	  )\
	{\
		Graphics_D3D11_RenderVertex( 1 ) ;\
		GD3D11.Device.DrawInfo.PrimitiveType = D_D3D11_PRIMITIVE_TOPOLOGY_LINELIST ;\
	}\
	*( ( void ** )&p ) = ( void * )GD3D11.Device.DrawInfo.VertexBufferNextAddr ;

#define GETVERTEX_LINE( p )		\
	MV1DRAWPACKDRAWMODEL		\
	if(\
		  GD3D11.Device.DrawInfo.Use3DVertex == 1 ||\
		  GD3D11.Device.DrawInfo.PrimitiveType != D_D3D11_PRIMITIVE_TOPOLOGY_LINELIST ||\
		  GD3D11.Device.DrawInfo.VertexNum + 2 >= D3D11_VertexBuffer_MaxVertexNum[ D3D11_VertexTypeToInputLayout[ GD3D11.Device.DrawInfo.Use3DVertex ][ GD3D11.Device.DrawInfo.VertexType ] ]\
	  )\
	{\
		Graphics_D3D11_RenderVertex( 0 ) ;\
		GD3D11.Device.DrawInfo.PrimitiveType = D_D3D11_PRIMITIVE_TOPOLOGY_LINELIST ;\
	}\
	*( ( void ** )&p ) = ( void * )GD3D11.Device.DrawInfo.VertexBufferNextAddr ;


#define GETVERTEX_LINE3D( p )		\
	MV1DRAWPACKDRAWMODEL		\
	if(\
		  GD3D11.Device.DrawInfo.Use3DVertex != 1 ||\
		  GD3D11.Device.DrawInfo.PrimitiveType != D_D3D11_PRIMITIVE_TOPOLOGY_LINELIST ||\
		  GD3D11.Device.DrawInfo.VertexNum + 2 >= D3D11_VertexBuffer_MaxVertexNum[ D3D11_VertexTypeToInputLayout[ GD3D11.Device.DrawInfo.Use3DVertex ][ GD3D11.Device.DrawInfo.VertexType ] ]\
	  )\
	{\
		Graphics_D3D11_RenderVertex( 1 ) ;\
		GD3D11.Device.DrawInfo.PrimitiveType = D_D3D11_PRIMITIVE_TOPOLOGY_LINELIST ;\
	}\
	*( ( void ** )&p ) = ( void * )GD3D11.Device.DrawInfo.VertexBufferNextAddr ;


#define GETVERTEX_POINT( p )		\
	MV1DRAWPACKDRAWMODEL		\
	if(\
		  GD3D11.Device.DrawInfo.Use3DVertex != 0 ||\
		  GD3D11.Device.DrawInfo.PrimitiveType != D_D3D11_PRIMITIVE_TOPOLOGY_POINTLIST ||\
		  GD3D11.Device.DrawInfo.VertexNum + 1 >= D3D11_VertexBuffer_MaxVertexNum[ D3D11_VertexTypeToInputLayout[ GD3D11.Device.DrawInfo.Use3DVertex ][ GD3D11.Device.DrawInfo.VertexType ] ]\
	  )\
	{\
		Graphics_D3D11_RenderVertex( 0 ) ;\
		GD3D11.Device.DrawInfo.PrimitiveType = D_D3D11_PRIMITIVE_TOPOLOGY_POINTLIST ;\
	}\
	*( ( void ** )&p ) = ( void * )GD3D11.Device.DrawInfo.VertexBufferNextAddr ;

#define GETVERTEX_POINT3D( p )		\
	MV1DRAWPACKDRAWMODEL		\
	if(\
		  GD3D11.Device.DrawInfo.Use3DVertex != 1 ||\
		  GD3D11.Device.DrawInfo.PrimitiveType != D_D3D11_PRIMITIVE_TOPOLOGY_POINTLIST ||\
		  GD3D11.Device.DrawInfo.VertexNum + 1 >= D3D11_VertexBuffer_MaxVertexNum[ D3D11_VertexTypeToInputLayout[ GD3D11.Device.DrawInfo.Use3DVertex ][ GD3D11.Device.DrawInfo.VertexType ] ]\
	  )\
	{\
		Graphics_D3D11_RenderVertex( 1 ) ;\
		GD3D11.Device.DrawInfo.PrimitiveType = D_D3D11_PRIMITIVE_TOPOLOGY_POINTLIST ;\
	}\
	*( ( void ** )&p ) = ( void * )GD3D11.Device.DrawInfo.VertexBufferNextAddr ;



#define ADD4VERTEX_POINT		\
		GD3D11.Device.DrawInfo.VertexBufferNextAddr += sizeof( VERTEX_NOTEX_2D ) ;	\
		GD3D11.Device.DrawInfo.VertexNum ++ ;

#define ADD4VERTEX_POINT3D		\
		GD3D11.Device.DrawInfo.VertexBufferNextAddr += sizeof( VERTEX_NOTEX_3D ) ;	\
		GD3D11.Device.DrawInfo.VertexNum ++ ;
/*
#define ADD4VERTEX_LINEBOX			\
		GD3D11.Device.DrawInfo.VertexBufferNextAddr += sizeof( VERTEX_NOTEX_2D ) * 8 ;	\
		GD3D11.Device.DrawInfo.VertexNum += 8 ;
*/
#define ADD4VERTEX_LINEBOX			\
		GD3D11.Device.DrawInfo.VertexBufferNextAddr += sizeof( VERTEX_NOTEX_2D ) * 24 ;	\
		GD3D11.Device.DrawInfo.VertexNum += 24 ;

#define ADD4VERTEX_LINETRIANGLE		\
		GD3D11.Device.DrawInfo.VertexBufferNextAddr += sizeof( VERTEX_NOTEX_2D ) * 6 ;	\
		GD3D11.Device.DrawInfo.VertexNum += 6 ;

#define ADD4VERTEX_LINETRIANGLE3D		\
		GD3D11.Device.DrawInfo.VertexBufferNextAddr += sizeof( VERTEX_NOTEX_3D ) * 6 ;	\
		GD3D11.Device.DrawInfo.VertexNum += 6 ;

#define ADD4VERTEX_LINE			\
		GD3D11.Device.DrawInfo.VertexBufferNextAddr += sizeof( VERTEX_NOTEX_2D ) * 2 ;	\
		GD3D11.Device.DrawInfo.VertexNum += 2 ;

#define ADD4VERTEX_LINE3D		\
		GD3D11.Device.DrawInfo.VertexBufferNextAddr += sizeof( VERTEX_NOTEX_3D ) * 2 ;	\
		GD3D11.Device.DrawInfo.VertexNum += 2 ;

#define ADD4VERTEX_NOTEX		\
		GD3D11.Device.DrawInfo.VertexBufferNextAddr += sizeof( VERTEX_NOTEX_2D ) * 6 ;	\
		GD3D11.Device.DrawInfo.VertexNum += 6 ;

#define ADD4VERTEX_TEX			\
		GD3D11.Device.DrawInfo.VertexBufferNextAddr += sizeof( VERTEX_2D ) * 6 ;	\
		GD3D11.Device.DrawInfo.VertexNum += 6 ;

#define ADD4VERTEX_BLENDTEX		\
		GD3D11.Device.DrawInfo.VertexBufferNextAddr += sizeof( VERTEX_BLENDTEX_2D ) * 6 ;	\
		GD3D11.Device.DrawInfo.VertexNum += 6 ;

#define ADD3VERTEX_NOTEX		\
		GD3D11.Device.DrawInfo.VertexBufferNextAddr += sizeof( VERTEX_NOTEX_2D ) * 3 ;	\
		GD3D11.Device.DrawInfo.VertexNum += 3 ;

#define ADD3VERTEX_TEX			\
		GD3D11.Device.DrawInfo.VertexBufferNextAddr += sizeof( VERTEX_2D ) * 3 ;	\
		GD3D11.Device.DrawInfo.VertexNum += 3 ;

#define ADD3VERTEX_BLENDTEX		\
		GD3D11.Device.DrawInfo.VertexBufferNextAddr += sizeof( VERTEX_BLENDTEX_2D ) * 3 ;	\
		GD3D11.Device.DrawInfo.VertexNum += 3 ;

#define ADD3VERTEX_NOTEX3D		\
		GD3D11.Device.DrawInfo.VertexBufferNextAddr += sizeof( VERTEX_NOTEX_3D ) * 3 ;	\
		GD3D11.Device.DrawInfo.VertexNum += 3 ;

#define ADD4VERTEX_BILLBOARD		\
		GD3D11.Device.DrawInfo.VertexBufferNextAddr += sizeof( VERTEX_3D ) * 6 ;	\
		GD3D11.Device.DrawInfo.VertexNum += 6 ;

#define SETUPCOLOR( DEST )		\
	if( GSYS.HardInfo.UseVertexColorBGRAFormat )\
	{\
		DEST  = ( ( ( ( ( 	GD3D11.Device.DrawInfo.DiffuseColor & 0x00ff0000 ) >> 16 ) * Red   ) / 255 ) << 16 ) |\
				( ( ( ( ( 	GD3D11.Device.DrawInfo.DiffuseColor & 0x0000ff00 ) >> 8  ) * Green ) / 255 ) << 8  ) |\
				( ( ( ( (	GD3D11.Device.DrawInfo.DiffuseColor & 0x000000ff )       ) * Blue  ) / 255 )       ) | \
				( 			GD3D11.Device.DrawInfo.DiffuseColor & 0xff000000 ) ;\
	}\
	else\
	{\
		DEST  = ( ( ( ( ( 	GD3D11.Device.DrawInfo.DiffuseColor & 0x000000ff )       ) * Red   ) / 255 )       ) |\
				( ( ( ( ( 	GD3D11.Device.DrawInfo.DiffuseColor & 0x0000ff00 ) >> 8  ) * Green ) / 255 ) << 8  ) |\
				( ( ( ( (	GD3D11.Device.DrawInfo.DiffuseColor & 0x00ff0000 ) >> 16 ) * Blue  ) / 255 ) << 16 ) | \
				( 			GD3D11.Device.DrawInfo.DiffuseColor & 0xff000000 ) ;\
	}


#define D3D11_COMMONBUFFER_ADDSIZE			(16 * 1024)

// RGB�l���P�x�ő��RGB�l�ɕϊ����邽�߂̃{�����[���e�N�X�`���̃T�C�Y
#define RGBTOVMAXRGB_TEX_SIZE		(128)

// �\���̐錾 -----------------------------------------------------------------

// �W���`��p�̕��G�ȏ������s��Ȃ����_�V�F�[�_�[�̏��
typedef struct tagGRAPHICS_HARDWARE_DIRECT3D11_BASE_SIMPLE_VERTEXSHADER_INFO
{
	const char *				ShaderFileName ;						// �g�p���钸�_�V�F�[�_�[�t�@�C����
	int							VertexDataSize ;						// ���_�f�[�^�������̃o�C�g��
	D_D3D11_INPUT_ELEMENT_DESC	InputElementDesc[ 11 ] ;				// ���͒��_�f�[�^�`��
} GRAPHICS_HARDWARE_DIRECT3D11_BASE_SIMPLE_VERTEXSHADER_INFO ;

// �f�[�^��` -----------------------------------------------------------------

#define RENDER_BLEND_TYPE_NORMAL			(0)			// 0:�ʏ�`��
#define RENDER_BLEND_TYPE_MUL				(1)			// 1:��Z�`��
#define RENDER_BLEND_TYPE_INVERSE			(2)			// 2:RGB���]
#define RENDER_BLEND_TYPE_X4				(3)			// 3:�`��P�x4�{
#define RENDER_BLEND_TYPE_PMA_NORMAL		(4)			// 4:��Z�ς݃��u�����h���[�h�̒ʏ�`��
#define RENDER_BLEND_TYPE_PMA_INVERSE		(5)			// 5:��Z�ς݃��u�����h���[�h��RGB���]
#define RENDER_BLEND_TYPE_PMA_X4			(6)			// 6:��Z�ς݃��u�����h���[�h�̕`��P�x4�{
#define RENDER_BLEND_TYPE_NUM				(7)
typedef struct tagDX_DIRECT3D11_RENDER_BLEND_INFO
{
	int									RenderBlendType ;
	D_D3D11_RENDER_TARGET_BLEND_DESC	RenderTargetBlendDesc ;
	int									AlphaZeroNotDrawFlag ;
} DX_DIRECT3D11_RENDER_BLEND_INFO ;
static DX_DIRECT3D11_RENDER_BLEND_INFO g_DefaultBlendDescArray[ DX_BLENDMODE_NUM ] =
{
	{ RENDER_BLEND_TYPE_NORMAL,			{ FALSE, D_D3D11_BLEND_ONE,				D_D3D11_BLEND_ZERO,				D_D3D11_BLEND_OP_ADD,			D_D3D11_BLEND_ONE,				D_D3D11_BLEND_ZERO,				D_D3D11_BLEND_OP_ADD,			D_D3D11_COLOR_WRITE_ENABLE_ALL },  TRUE },	// DX_BLENDMODE_NOBLEND			�m�[�u�����h
	{ RENDER_BLEND_TYPE_NORMAL,			{ TRUE,  D_D3D11_BLEND_SRC_ALPHA,		D_D3D11_BLEND_INV_SRC_ALPHA,	D_D3D11_BLEND_OP_ADD,			D_D3D11_BLEND_SRC_ALPHA,		D_D3D11_BLEND_INV_SRC_ALPHA,	D_D3D11_BLEND_OP_ADD,			D_D3D11_COLOR_WRITE_ENABLE_ALL },  TRUE },	// DX_BLENDMODE_ALPHA			���u�����h
	{ RENDER_BLEND_TYPE_NORMAL,			{ TRUE,  D_D3D11_BLEND_SRC_ALPHA,		D_D3D11_BLEND_ONE,				D_D3D11_BLEND_OP_ADD,			D_D3D11_BLEND_SRC_ALPHA,		D_D3D11_BLEND_ONE,				D_D3D11_BLEND_OP_ADD,			D_D3D11_COLOR_WRITE_ENABLE_ALL },  TRUE },	// DX_BLENDMODE_ADD				���Z�u�����h
	{ RENDER_BLEND_TYPE_NORMAL,			{ TRUE,  D_D3D11_BLEND_INV_DEST_COLOR,	D_D3D11_BLEND_ZERO,				D_D3D11_BLEND_OP_ADD,			D_D3D11_BLEND_INV_DEST_ALPHA,	D_D3D11_BLEND_ZERO,				D_D3D11_BLEND_OP_ADD,			D_D3D11_COLOR_WRITE_ENABLE_ALL },  TRUE },	// DX_BLENDMODE_SUB				���Z�u�����h
	{ RENDER_BLEND_TYPE_NORMAL,			{ TRUE,  D_D3D11_BLEND_ZERO,			D_D3D11_BLEND_SRC_COLOR,		D_D3D11_BLEND_OP_ADD,			D_D3D11_BLEND_ZERO,				D_D3D11_BLEND_SRC_ALPHA,		D_D3D11_BLEND_OP_ADD,			D_D3D11_COLOR_WRITE_ENABLE_ALL }, FALSE },	// DX_BLENDMODE_MUL				��Z�u�����h
	{ RENDER_BLEND_TYPE_NORMAL,			{ TRUE,  D_D3D11_BLEND_SRC_ALPHA,		D_D3D11_BLEND_ONE,				D_D3D11_BLEND_OP_ADD,			D_D3D11_BLEND_SRC_ALPHA,		D_D3D11_BLEND_ONE,				D_D3D11_BLEND_OP_ADD,			D_D3D11_COLOR_WRITE_ENABLE_ALL },  TRUE },	// DX_BLENDMODE_SUB2			���������p���Z�u�����h�P
	{ RENDER_BLEND_TYPE_NORMAL,			{ FALSE, D_D3D11_BLEND_ONE,				D_D3D11_BLEND_ZERO,				D_D3D11_BLEND_OP_ADD,			D_D3D11_BLEND_ONE,				D_D3D11_BLEND_ZERO,				D_D3D11_BLEND_OP_ADD,			D_D3D11_COLOR_WRITE_ENABLE_ALL },  TRUE },	// DX_BLENDMODE_XOR				XOR�u�����h(��Ή�)
	{ RENDER_BLEND_TYPE_NORMAL,			{ FALSE, D_D3D11_BLEND_ONE,				D_D3D11_BLEND_ZERO,				D_D3D11_BLEND_OP_ADD,			D_D3D11_BLEND_ONE,				D_D3D11_BLEND_ZERO,				D_D3D11_BLEND_OP_ADD,			D_D3D11_COLOR_WRITE_ENABLE_ALL },  TRUE },	// ����
	{ RENDER_BLEND_TYPE_NORMAL,			{ TRUE,  D_D3D11_BLEND_ZERO,			D_D3D11_BLEND_ONE,				D_D3D11_BLEND_OP_ADD,			D_D3D11_BLEND_ZERO,				D_D3D11_BLEND_ONE,				D_D3D11_BLEND_OP_ADD,			D_D3D11_COLOR_WRITE_ENABLE_ALL }, FALSE },	// DX_BLENDMODE_DESTCOLOR		�J���[�͍X�V����Ȃ�
	{ RENDER_BLEND_TYPE_NORMAL,			{ TRUE,  D_D3D11_BLEND_INV_DEST_COLOR,	D_D3D11_BLEND_ZERO,				D_D3D11_BLEND_OP_ADD,			D_D3D11_BLEND_INV_DEST_ALPHA,	D_D3D11_BLEND_ZERO,				D_D3D11_BLEND_OP_ADD,			D_D3D11_COLOR_WRITE_ENABLE_ALL }, FALSE },	// DX_BLENDMODE_INVDESTCOLOR	�`���̐F�̔��]�l���|����
	{ RENDER_BLEND_TYPE_INVERSE,		{ TRUE,  D_D3D11_BLEND_SRC_ALPHA,		D_D3D11_BLEND_INV_SRC_ALPHA,	D_D3D11_BLEND_OP_ADD,			D_D3D11_BLEND_SRC_ALPHA,		D_D3D11_BLEND_INV_SRC_ALPHA,	D_D3D11_BLEND_OP_ADD,			D_D3D11_COLOR_WRITE_ENABLE_ALL },  TRUE },	// DX_BLENDMODE_INVSRC			�`�挳�̐F�𔽓]����
	{ RENDER_BLEND_TYPE_MUL,			{ TRUE,  D_D3D11_BLEND_ZERO,			D_D3D11_BLEND_SRC_COLOR,		D_D3D11_BLEND_OP_ADD,			D_D3D11_BLEND_ZERO,				D_D3D11_BLEND_SRC_ALPHA,		D_D3D11_BLEND_OP_ADD,			D_D3D11_COLOR_WRITE_ENABLE_ALL },  TRUE },	// DX_BLENDMODE_MULA			�A���t�@�`�����l���l���t����Z�u�����h
	{ RENDER_BLEND_TYPE_X4,				{ TRUE,  D_D3D11_BLEND_SRC_ALPHA,		D_D3D11_BLEND_INV_SRC_ALPHA,	D_D3D11_BLEND_OP_ADD,			D_D3D11_BLEND_SRC_ALPHA,		D_D3D11_BLEND_INV_SRC_ALPHA,	D_D3D11_BLEND_OP_ADD,			D_D3D11_COLOR_WRITE_ENABLE_ALL },  TRUE },	// DX_BLENDMODE_ALPHA_X4		���u�����h�̕`�摤�̋P�x���ő�S�{�ɂł��郂�[�h
	{ RENDER_BLEND_TYPE_X4,				{ TRUE,  D_D3D11_BLEND_SRC_ALPHA,		D_D3D11_BLEND_ONE,				D_D3D11_BLEND_OP_ADD,			D_D3D11_BLEND_SRC_ALPHA,		D_D3D11_BLEND_ONE,				D_D3D11_BLEND_OP_ADD,			D_D3D11_COLOR_WRITE_ENABLE_ALL },  TRUE },	// DX_BLENDMODE_ADD_X4			���Z�u�����h�̕`�摤�̋P�x���ő�S�{�ɂł��郂�[�h
	{ RENDER_BLEND_TYPE_NORMAL,			{ TRUE,  D_D3D11_BLEND_ONE,				D_D3D11_BLEND_ZERO,				D_D3D11_BLEND_OP_ADD,			D_D3D11_BLEND_ONE,				D_D3D11_BLEND_ZERO,				D_D3D11_BLEND_OP_ADD,			D_D3D11_COLOR_WRITE_ENABLE_ALL }, FALSE },	// DX_BLENDMODE_SRCCOLOR		�`�挳�̃J���[�ł��̂܂ܕ`�悳���
	{ RENDER_BLEND_TYPE_NORMAL,			{ TRUE,  D_D3D11_BLEND_ONE,				D_D3D11_BLEND_INV_SRC_ALPHA,	D_D3D11_BLEND_OP_ADD,			D_D3D11_BLEND_ONE,				D_D3D11_BLEND_INV_SRC_ALPHA,	D_D3D11_BLEND_OP_ADD,			D_D3D11_COLOR_WRITE_ENABLE_ALL }, FALSE },	// DX_BLENDMODE_HALF_ADD		�����Z�u�����h
	{ RENDER_BLEND_TYPE_NORMAL,			{ TRUE,  D_D3D11_BLEND_SRC_ALPHA,		D_D3D11_BLEND_ONE,				D_D3D11_BLEND_OP_REV_SUBTRACT,	D_D3D11_BLEND_SRC_ALPHA,		D_D3D11_BLEND_ONE,				D_D3D11_BLEND_OP_REV_SUBTRACT,	D_D3D11_COLOR_WRITE_ENABLE_ALL },  TRUE },	// DX_BLENDMODE_SUB1			�o�̓u�����h���g�p�\�ȏꍇ�̌��Z�u�����h

	{ RENDER_BLEND_TYPE_PMA_NORMAL,		{ TRUE,  D_D3D11_BLEND_ONE,				D_D3D11_BLEND_INV_SRC_ALPHA,	D_D3D11_BLEND_OP_ADD,			D_D3D11_BLEND_ONE,				D_D3D11_BLEND_INV_SRC_ALPHA,	D_D3D11_BLEND_OP_ADD,			D_D3D11_COLOR_WRITE_ENABLE_ALL },  TRUE },	// DX_BLENDMODE_PMA_ALPHA		��Z�ς݃��u�����h���[�h�̃��u�����h
	{ RENDER_BLEND_TYPE_PMA_NORMAL,		{ TRUE,  D_D3D11_BLEND_ONE,				D_D3D11_BLEND_ONE,				D_D3D11_BLEND_OP_ADD,			D_D3D11_BLEND_ONE,				D_D3D11_BLEND_ONE,				D_D3D11_BLEND_OP_ADD,			D_D3D11_COLOR_WRITE_ENABLE_ALL },  TRUE },	// DX_BLENDMODE_PMA_ADD			��Z�ς݃��u�����h���[�h�̉��Z�u�����h
	{ RENDER_BLEND_TYPE_PMA_NORMAL,		{ TRUE,  D_D3D11_BLEND_ONE,				D_D3D11_BLEND_ONE,				D_D3D11_BLEND_OP_REV_SUBTRACT,	D_D3D11_BLEND_ONE,				D_D3D11_BLEND_ONE,				D_D3D11_BLEND_OP_REV_SUBTRACT,	D_D3D11_COLOR_WRITE_ENABLE_ALL },  TRUE },	// DX_BLENDMODE_PMA_SUB			��Z�ς݃��u�����h���[�h�̌��Z�u�����h
	{ RENDER_BLEND_TYPE_PMA_INVERSE,	{ TRUE,  D_D3D11_BLEND_ONE,				D_D3D11_BLEND_INV_SRC_ALPHA,	D_D3D11_BLEND_OP_ADD,			D_D3D11_BLEND_ONE,				D_D3D11_BLEND_INV_SRC_ALPHA,	D_D3D11_BLEND_OP_ADD,			D_D3D11_COLOR_WRITE_ENABLE_ALL },  TRUE },	// DX_BLENDMODE_PMA_INVSRC		��Z�ς݃��u�����h���[�h�̕`�挳�̐F�𔽓]����
	{ RENDER_BLEND_TYPE_PMA_X4,			{ TRUE,  D_D3D11_BLEND_ONE,				D_D3D11_BLEND_INV_SRC_ALPHA,	D_D3D11_BLEND_OP_ADD,			D_D3D11_BLEND_ONE,				D_D3D11_BLEND_INV_SRC_ALPHA,	D_D3D11_BLEND_OP_ADD,			D_D3D11_COLOR_WRITE_ENABLE_ALL },  TRUE },	// DX_BLENDMODE_PMA_ALPHA_X4	��Z�ς݃��u�����h���[�h�̃��u�����h�̕`�摤�̋P�x���ő�S�{�ɂł��郂�[�h
	{ RENDER_BLEND_TYPE_PMA_X4,			{ TRUE,  D_D3D11_BLEND_ONE,				D_D3D11_BLEND_ONE,				D_D3D11_BLEND_OP_ADD,			D_D3D11_BLEND_ONE,				D_D3D11_BLEND_ONE,				D_D3D11_BLEND_OP_ADD,			D_D3D11_COLOR_WRITE_ENABLE_ALL },  TRUE },	// DX_BLENDMODE_PMA_ADD_X4		��Z�ς݃��u�����h���[�h�̉��Z�u�����h�̕`�摤�̋P�x���ő�S�{�ɂł��郂�[�h
} ;

// ���_�o�b�t�@�Ɋi�[�ł��钸�_�̍ő吔�̃e�[�u��
static const int D3D11_VertexBuffer_MaxVertexNum[ D3D11_VERTEX_INPUTLAYOUT_NUM ] =
{
	D3D11_VERTEXBUFFER_SIZE / sizeof( VERTEX_NOTEX_2D    ),
	D3D11_VERTEXBUFFER_SIZE / sizeof( VERTEX_2D          ),
	D3D11_VERTEXBUFFER_SIZE / sizeof( VERTEX_BLENDTEX_2D ),
	D3D11_VERTEXBUFFER_SIZE / sizeof( VERTEX_NOTEX_3D    ),
	D3D11_VERTEXBUFFER_SIZE / sizeof( VERTEX_3D          ),
	D3D11_VERTEXBUFFER_SIZE / sizeof( VERTEX3D           ),
	D3D11_VERTEXBUFFER_SIZE / sizeof( VERTEX3DSHADER     ),
} ;

// �e���_�f�[�^�̃T�C�Y�̃e�[�u��
static const int D3D11_VertexSize[ D3D11_VERTEX_INPUTLAYOUT_NUM ] =
{
	sizeof( VERTEX_NOTEX_2D    ),
	sizeof( VERTEX_2D          ),
	sizeof( VERTEX_BLENDTEX_2D ),
	sizeof( VERTEX_NOTEX_3D    ),
	sizeof( VERTEX_3D          ),
	sizeof( VERTEX3D           ),
	sizeof( VERTEX3DSHADER     ),
} ;

// �R�c���_���ǂ����ƒ��_�^�C�v�̑g�ݍ��킹�ɑΉ����钸�_�f�[�^�^�C�v[ 0:�Q�c���_  1:�R�c���_ ][ ���_�^�C�v ]
static const DWORD D3D11_VertexTypeToInputLayout[ 2 ][ VERTEXTYPE_NUM ] =
{
	{
		D3D11_VERTEX_INPUTLAYOUT_NOTEX_2D,
		D3D11_VERTEX_INPUTLAYOUT_2D,
		D3D11_VERTEX_INPUTLAYOUT_BLENDTEX_2D,
	},

	{
		D3D11_VERTEX_INPUTLAYOUT_NOTEX_3D,
		D3D11_VERTEX_INPUTLAYOUT_3D
	},
} ;

// �P�ʍs��
static MATRIX D3D11_GlobalIdentMatrix =
{
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f
} ;

// �W���`��p�̕��G�ȏ������s��Ȃ����_�V�F�[�_�[�̏��
static GRAPHICS_HARDWARE_DIRECT3D11_BASE_SIMPLE_VERTEXSHADER_INFO g_BaseSimpleVertexShaderInfo[ D3D11_VERTEX_INPUTLAYOUT_NUM ] =
{
	// D3D11_VERTEX_INPUTLAYOUT_NOTEX_2D( VERTEX_NOTEX_2D )
	{
		"Base_VERTEX_NOTEX_2D.vso",
		sizeof( VERTEX_NOTEX_2D ),
		{
			{ "POSITION",     0, D_DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0,  D_D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",        0, D_DXGI_FORMAT_R8G8B8A8_UNORM,     0, 16, D_D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ NULL },
		},
	},

	// D3D11_VERTEX_INPUTLAYOUT_2D( VERTEX2D or VERTEX_2D )
	{
		"Base_VERTEX2D.vso",
		sizeof( VERTEX2D ),
		{
			{ "POSITION",     0, D_DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0,  D_D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",        0, D_DXGI_FORMAT_R8G8B8A8_UNORM,     0, 16, D_D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD",     0, D_DXGI_FORMAT_R32G32_FLOAT,       0, 20, D_D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ NULL },
		},
	},

	// D3D11_VERTEX_INPUTLAYOUT_BLENDTEX_2D( VERTEX_BLENDTEX_2D or VERTEX2DSHADER )
	{
		"Base_VERTEX_BLENDTEX_2D.vso",		
		sizeof( VERTEX_BLENDTEX_2D ),
		{
			{ "POSITION",     0, D_DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0,  D_D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",        0, D_DXGI_FORMAT_R8G8B8A8_UNORM,     0, 16, D_D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",        1, D_DXGI_FORMAT_R8G8B8A8_UNORM,     0, 20, D_D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD",     0, D_DXGI_FORMAT_R32G32_FLOAT,       0, 24, D_D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD",     1, D_DXGI_FORMAT_R32G32_FLOAT,       0, 32, D_D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ NULL },
		},
	},

	// D3D11_VERTEX_INPUTLAYOUT_NOTEX_3D( VERTEX_NOTEX_3D )
	{
		"Base_VERTEX_NOTEX_3D.vso",
		sizeof( VERTEX_NOTEX_3D ),
		{
			{ "POSITION",     0, D_DXGI_FORMAT_R32G32B32_FLOAT,    0, 0,  D_D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",        0, D_DXGI_FORMAT_R8G8B8A8_UNORM,     0, 12, D_D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ NULL },
		},
	},

	// D3D11_VERTEX_INPUTLAYOUT_3D( VERTEX_3D )
	{
		"Base_VERTEX_3D.vso",
		sizeof( VERTEX_3D ),
		{
			{ "POSITION",     0, D_DXGI_FORMAT_R32G32B32_FLOAT,    0, 0,  D_D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",        0, D_DXGI_FORMAT_R8G8B8A8_UNORM,     0, 12, D_D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD",     0, D_DXGI_FORMAT_R32G32_FLOAT,       0, 16, D_D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ NULL },
		},
	},

	// D3D11_VERTEX_INPUTLAYOUT_3D_LIGHT( VERTEX3D )
	{
		"Base_VERTEX3D.vso",
		sizeof( VERTEX3D ),
		{
			{ "POSITION",     0, D_DXGI_FORMAT_R32G32B32_FLOAT,    0, 0,  D_D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL",       0, D_DXGI_FORMAT_R32G32B32_FLOAT,    0, 12, D_D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",        0, D_DXGI_FORMAT_R8G8B8A8_UNORM,     0, 24, D_D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",        1, D_DXGI_FORMAT_R8G8B8A8_UNORM,     0, 28, D_D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD",     0, D_DXGI_FORMAT_R32G32_FLOAT,       0, 32, D_D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD",     1, D_DXGI_FORMAT_R32G32_FLOAT,       0, 40, D_D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ NULL },
		},
	},

	// D3D11_VERTEX_INPUTLAYOUT_SHADER_3D( VERTEX3DSHADER )
	{
		"Base_VERTEX3DSHADER.vso",
		sizeof( VERTEX3DSHADER ),
		{
			{ "POSITION",     0, D_DXGI_FORMAT_R32G32B32_FLOAT,    0, 0,  D_D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "POSITION",     1, D_DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D_D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL",       0, D_DXGI_FORMAT_R32G32B32_FLOAT,    0, 28, D_D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TANGENT",      0, D_DXGI_FORMAT_R32G32B32_FLOAT,    0, 40, D_D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BINORMAL",     0, D_DXGI_FORMAT_R32G32B32_FLOAT,    0, 52, D_D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",        0, D_DXGI_FORMAT_R8G8B8A8_UNORM,     0, 64, D_D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",        1, D_DXGI_FORMAT_R8G8B8A8_UNORM,     0, 68, D_D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD",     0, D_DXGI_FORMAT_R32G32_FLOAT,       0, 72, D_D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD",     1, D_DXGI_FORMAT_R32G32_FLOAT,       0, 80, D_D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ NULL },
		},
	},
} ;

// Direct3D11 ���g�����O���t�B�b�N�X�������̍\����
GRAPHICS_HARDDATA_DIRECT3D11 GraphicsHardDataDirect3D11 ;

// �V�F�[�_�[�o�C�i���z��
extern int  DxShaderCodeBin_Model_D3D11Convert ;
extern BYTE DxShaderCodeBin_Model_D3D11[] ;

extern int  DxShaderCodeBin_Filter_D3D11Convert ;
extern BYTE DxShaderCodeBin_Filter_D3D11[] ;

extern int  DxShaderCodeBin_Base_D3D11Convert ;
extern BYTE DxShaderCodeBin_Base_D3D11[] ;

extern int  DxShaderCodeBin_Base3D_D3D11Convert ;
extern BYTE DxShaderCodeBin_Base3D_D3D11[] ;

extern int  DxShaderCodeBin_RgbaMix_D3D11Convert ;
extern BYTE DxShaderCodeBin_RgbaMix_D3D11[] ;

// �֐��錾 -------------------------------------------------------------------

static void Graphics_D3D11_VertexShaderArray_Release(      D_ID3D11VertexShader      **pObject, int Num ) ;		// ���_�V�F�[�_�[�z����������
static void Graphics_D3D11_PixelShaderArray_Release(       D_ID3D11PixelShader       **pObject, int Num ) ;		// �s�N�Z���V�F�[�_�[�z����������
static void Graphics_D3D11_DepthStencilStateArray_Release( D_ID3D11DepthStencilState **pObject, int Num ) ;		// �f�v�X�X�e���V���X�e�[�g�z����������
static void Graphics_D3D11_BlendStateArray_Release(        D_ID3D11BlendState        **pObject, int Num ) ;		// �u�����h�X�e�[�g�z����������
static void Graphics_D3D11_RasterizerStateArray_Release(   D_ID3D11RasterizerState   **pObject, int Num ) ;		// ���X�^���C�U�[�X�e�[�g�z����������
static void Graphics_D3D11_SamplerStateArray_Release(      D_ID3D11SamplerState      **pObject, int Num ) ;		// �T���v���[�X�e�[�g�z����������

// D3D11Device �X�e�[�^�X�֌W
static int Graphics_D3D11_DeviceState_SetupSamplerState( void ) ;									// ���݂̐ݒ�Ɋ�Â��� ID3D11SamplerState ���Z�b�g�A�b�v����
static int Graphics_D3D11_DeviceState_TerminateSamplerState( void ) ;								// ID3D11SamplerState ���������

static int Graphics_D3D11_DeviceState_SetupRasterizerState( void ) ;								// ���݂̐ݒ�Ɋ�Â��� ID3D11RasterizerState ���Z�b�g�A�b�v����
static int Graphics_D3D11_DeviceState_TerminateRasterizerState( void ) ;							// ID3D11RasterizerState ���������

static int Graphics_D3D11_DeviceState_SetupDepthStencilState( void ) ;								// ���݂̐ݒ�Ɋ�Â��� ID3D11DepthStencilState ���Z�b�g�A�b�v����
static int Graphics_D3D11_DeviceState_TerminateDepthStencilState( void ) ;							// ID3D11DepthStencilState ���������

static int Graphics_D3D11_DeviceState_SetupBlendState( void ) ;										// ���݂̐ݒ�Ɋ�Â��� ID3D11BlendState ���Z�b�g�A�b�v����
static int Graphics_D3D11_DeviceState_TerminateBlendState( void ) ;									// ID3D11BlendState ���������

static int Graphics_D3D11_DeviceState_SetNormalTextureAddressTransformMatrix_ConstBuffer( void ) ;	// �W���`��p�̃e�N�X�`�����W�ϊ��s���萔�o�b�t�@�ɃZ�b�g����
static int Graphics_D3D11_DeviceState_UpdateConstantFogParam( void ) ;								// �t�H�O�̐F�ȊO�̒萔�����X�V����
static void Graphics_D3D11_DeviceState_RefreshAmbientAndEmissiveParam( void ) ;						// �A���r�G���g���C�g�ƃG�~�b�V�u�J���[���|�����킹���p�����[�^���X�V����
static void Graphics_D3D11_DeviceState_SetRenderTarget_SizeCheck( void ) ;							// �S�Ă̕`��Ώۂ̃T�C�Y����v���Ă����烌���_�����O�^�[�Q�b�g�ɃZ�b�g


// Direct3D11 �ɐݒ肷��ˉe�s����X�V����
static void Graphics_Hardware_D3D11_RefreshProjectionMatrix( void ) ;

static int Graphics_D3D11_BltBmpOrBaseImageToGraph3_MipMapBlt(
	      IMAGEDATA_ORIG	*Orig,
	const RECT				*SrcRect,
	const RECT				*DestRect,
	      D_ID3D11Texture2D	*UseTex,
	      int				TexWidth,
	      int				TexHeight,
	const BASEIMAGE			*RgbBaseImage,
	const BASEIMAGE			*AlphaBaseImage,
	const COLORDATA			*DestColor,
	      int				RedIsAlphaFlag,
	      int				UseTransColorConvAlpha,
	      int				ASyncThread
) ;

static int Graphics_D3D11_BltBmpOrBaseImageToGraph3_NoMipMapBlt(
	      IMAGEDATA_ORIG	*Orig,
	const RECT				*SrcRect,
	const RECT				*DestRect,
	      D_ID3D11Texture2D	*UseTex,
	      int				TexWidth,
	      int				TexHeight,
	      int				TexUseWidth,
	      int				TexUseHeight,
	const BASEIMAGE			*RgbBaseImage,
	const BASEIMAGE			*AlphaBaseImage,
	const COLORDATA			*DestColor,
	      int				RedIsAlphaFlag,
	      int				UseTransColorConvAlpha,
	      int				ASyncThread
) ;

__inline	static DWORD GetDiffuseColor( void )															// ���݂̃f�B�t���[�Y�J���[�𓾂�
			{
				if( GSYS.HardInfo.UseVertexColorBGRAFormat )
				{
					return  ( DWORD )( ( GSYS.DrawSetting.DrawBright.Red   << 16 ) |
									   ( GSYS.DrawSetting.DrawBright.Green << 8  ) |
									   ( GSYS.DrawSetting.DrawBright.Blue        ) |
									   ( ( ( GSYS.DrawSetting.BlendMode != DX_BLENDMODE_NOBLEND || GSYS.DrawSetting.UseNoBlendModeParam ) ? GSYS.DrawSetting.BlendParam : 255 ) << 24 ) ) ;
				}
				else
				{
					return  ( DWORD )( ( GSYS.DrawSetting.DrawBright.Red         ) |
									   ( GSYS.DrawSetting.DrawBright.Green << 8  ) |
									   ( GSYS.DrawSetting.DrawBright.Blue  << 16 ) |
									   ( ( ( GSYS.DrawSetting.BlendMode != DX_BLENDMODE_NOBLEND || GSYS.DrawSetting.UseNoBlendModeParam ) ? GSYS.DrawSetting.BlendParam : 255 ) << 24 ) ) ;
				}
			}

#ifndef DX_NON_MODEL
static	int		CreateRGBtoVMaxRGBVolumeTexture( void ) ;							// RGB�J���[���P�x���ő�ɂ���RGB�l�ɕϊ����邽�߂̃{�����[���e�N�X�`�����쐬����( 0:����  -1:���s )
#endif // DX_NON_MODEL

// �v���O���� -----------------------------------------------------------------

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
static int CreateRGBtoVMaxRGBVolumeTexture( void )
{
	D_D3D11_TEXTURE3D_DESC				Texture3DDesc ;
	D_D3D11_SHADER_RESOURCE_VIEW_DESC	SRVDesc ;
	D_D3D11_BOX							DestBox ;
	void								*TempBuffer ;
	int									r ;
	int									g ;
	int									b ;
	BYTE								*pDest ;
	HRESULT								hr ;

	if( GAPIWin.D3D11DeviceObject == NULL )
	{
		return -1 ;
	}

	GD3D11.RGBtoVMaxRGBVolumeTexture	= NULL ;
	GD3D11.RGBtoVMaxRGBVolumeTextureSRV = NULL ;
	TempBuffer							= NULL ;

	// �{�����[���e�N�X�`���̍쐬
	_MEMSET( &Texture3DDesc, 0, sizeof( Texture3DDesc ) ) ;
	Texture3DDesc.Usage              = D_D3D11_USAGE_DEFAULT ;
	Texture3DDesc.Format             = D_DXGI_FORMAT_R8G8B8A8_UNORM ;
	Texture3DDesc.BindFlags          = D_D3D11_BIND_SHADER_RESOURCE ;
	Texture3DDesc.Width              = RGBTOVMAXRGB_TEX_SIZE ;
	Texture3DDesc.Height             = RGBTOVMAXRGB_TEX_SIZE ;
	Texture3DDesc.Depth              = RGBTOVMAXRGB_TEX_SIZE ;
	Texture3DDesc.CPUAccessFlags     = 0 ;
	Texture3DDesc.MipLevels          = 1 ;
	hr = D3D11Device_CreateTexture3D_ASync( &Texture3DDesc, NULL, &GD3D11.RGBtoVMaxRGBVolumeTexture, FALSE ) ;
	if( FAILED( hr ) )
	{
		DXST_ERRORLOG_ADDUTF16LE( "\x52\x00\x47\x00\x42\x00\xab\x30\xe9\x30\xfc\x30\x92\x30\x1d\x8f\xa6\x5e\x92\x30\x00\x67\x27\x59\x6b\x30\x57\x30\x5f\x30\x52\x00\x47\x00\x42\x00\x24\x50\x6b\x30\x09\x59\xdb\x63\x59\x30\x8b\x30\x5f\x30\x81\x30\x6e\x30\xdc\x30\xea\x30\xe5\x30\xfc\x30\xe0\x30\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"RGB�J���[���P�x���ő�ɂ���RGB�l�ɕϊ����邽�߂̃{�����[���e�N�X�`���̍쐬�Ɏ��s���܂���\n" @*/ ) ;
		goto ERR ;
	}

	// �V�F�[�_�[���\�[�X�r���[�̍쐬
	_MEMSET( &SRVDesc, 0, sizeof( SRVDesc ) );
	SRVDesc.Format						= D_DXGI_FORMAT_R8G8B8A8_UNORM ;
	SRVDesc.ViewDimension				= D_D3D11_SRV_DIMENSION_TEXTURE3D ;
	SRVDesc.Texture3D.MipLevels			= 1 ;
	SRVDesc.Texture3D.MostDetailedMip	= 0 ;
	hr = D3D11Device_CreateShaderResourceView_ASync( GD3D11.RGBtoVMaxRGBVolumeTexture, &SRVDesc, &GD3D11.RGBtoVMaxRGBVolumeTextureSRV, FALSE ) ;
	if( FAILED( hr ) )
	{
		DXST_ERRORLOG_ADDUTF16LE( "\x52\x00\x47\x00\x42\x00\xab\x30\xe9\x30\xfc\x30\x92\x30\x1d\x8f\xa6\x5e\x92\x30\x00\x67\x27\x59\x6b\x30\x57\x30\x5f\x30\x52\x00\x47\x00\x42\x00\x24\x50\x6b\x30\x09\x59\xdb\x63\x59\x30\x8b\x30\xba\x70\x6e\x30\xdc\x30\xea\x30\xe5\x30\xfc\x30\xe0\x30\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\x28\x75\xb7\x30\xa7\x30\xfc\x30\xc0\x30\xfc\x30\xea\x30\xbd\x30\xfc\x30\xb9\x30\xd3\x30\xe5\x30\xfc\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x00"/*@ L"RGB�J���[���P�x���ő�ɂ���RGB�l�ɕϊ�����ׂ̃{�����[���e�N�X�`���p�V�F�[�_�[���\�[�X�r���[�̍쐬�Ɏ��s���܂���" @*/ ) ;
		goto ERR ;
	}

	// �f�[�^�]���p�������̊m��
	TempBuffer = DXALLOC( RGBTOVMAXRGB_TEX_SIZE * RGBTOVMAXRGB_TEX_SIZE * RGBTOVMAXRGB_TEX_SIZE * 4 ) ;
	if( TempBuffer == NULL )
	{
		DXST_ERRORLOG_ADDUTF16LE( "\x52\x00\x47\x00\x42\x00\xab\x30\xe9\x30\xfc\x30\x92\x30\x1d\x8f\xa6\x5e\x92\x30\x00\x67\x27\x59\x6b\x30\x57\x30\x5f\x30\x52\x00\x47\x00\x42\x00\x24\x50\x6b\x30\x09\x59\xdb\x63\x59\x30\x8b\x30\x5f\x30\x81\x30\x6e\x30\xe2\x8e\x01\x90\x43\x51\x28\x75\xe1\x30\xe2\x30\xea\x30\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"RGB�J���[���P�x���ő�ɂ���RGB�l�ɕϊ����邽�߂̓]�����p�������̊m�ۂɎ��s���܂���\n" @*/ ) ;
		goto ERR ;
	}

	// �V�X�e���������{�����[���e�N�X�`���ɕϊ�������������
	for( b = 0 ; b < RGBTOVMAXRGB_TEX_SIZE ; b ++ )
	{
		for( g = 0 ; g < RGBTOVMAXRGB_TEX_SIZE ; g ++ )
		{
			pDest = ( BYTE * )TempBuffer + ( RGBTOVMAXRGB_TEX_SIZE * RGBTOVMAXRGB_TEX_SIZE * 4 ) * b + ( RGBTOVMAXRGB_TEX_SIZE * 4 ) * g ;
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

	// �e�N�X�`���ɓ]��
	DestBox.left   = 0 ;
	DestBox.top    = 0 ;
	DestBox.front  = 0 ;
	DestBox.right  = RGBTOVMAXRGB_TEX_SIZE ;
	DestBox.bottom = RGBTOVMAXRGB_TEX_SIZE ;
	DestBox.back   = RGBTOVMAXRGB_TEX_SIZE ;

	D3D11DeviceContext_UpdateSubresource_ASync(
		GD3D11.RGBtoVMaxRGBVolumeTexture,
		D_D3D11CalcSubresource( 0, 0, 1 ),
		&DestBox,
		TempBuffer,
		( DWORD )( RGBTOVMAXRGB_TEX_SIZE * 4 ),
		( DWORD )( RGBTOVMAXRGB_TEX_SIZE * RGBTOVMAXRGB_TEX_SIZE * 4 ),
		0
	) ;

	// �f�[�^�]���p�̃����������
	DXFREE( TempBuffer ) ;
	TempBuffer = NULL ;

	// �I��
	return 0 ;

ERR :
	if( GD3D11.RGBtoVMaxRGBVolumeTextureSRV )
	{
		Direct3D11_Release_ShaderResourceView( GD3D11.RGBtoVMaxRGBVolumeTextureSRV ) ;
		GD3D11.RGBtoVMaxRGBVolumeTextureSRV = NULL ;
	}

	if( GD3D11.RGBtoVMaxRGBVolumeTexture )
	{
		Direct3D11_Release_Texture3D_ASync( GD3D11.RGBtoVMaxRGBVolumeTexture ) ;
		GD3D11.RGBtoVMaxRGBVolumeTexture = NULL ;
	}

	if( TempBuffer )
	{
		DXFREE( TempBuffer ) ;
		TempBuffer = NULL ;
	}
	
	return -1 ;
}

#endif // DX_NON_MODEL

// Direct3D11 ���g�p�����O���t�B�b�N�X�����̏��������s��( 0:���� -1:���s )
extern	int		Graphics_D3D11_Initialize( void )
{
	// Direct3D11.DLL �̓ǂݍ���
	if( Direct3D11_LoadDLL() < 0 )
	{
		goto ERR ;
	}

	// DXGI.DLL �̓ǂݍ���
	if( DXGI_LoadDLL() < 0 )
	{
		goto ERR ;
	}

	// D3D11Device ���쐬����
	if( Graphics_D3D11_Device_Create() != 0 )
	{
		goto ERR ;
	}

	// �W���`��p���_�o�b�t�@�̍쐬
	if( Graphics_D3D11_NormalDrawVertexBuffer_Create() != 0 )
	{
		goto ERR ;
	}

	// �[�x�o�b�t�@�̍쐬�����݂�
	if( Graphics_D3D11_CreateDepthBuffer() != 0 )
	{
		goto ERR ;
	}

	// �V�F�[�_�[�̍쐬
	if( Graphics_D3D11_Shader_Initialize() != 0 )
	{
		goto ERR ;
	}

	// �萔���̏�����
	if( Graphics_D3D11_Constant_Initialize() != 0 )
	{
		goto ERR ;
	}

	// ���̓��C�A�E�g�̍쐬
	if( Graphics_D3D11_InputLayout_Base_Create() != 0 )
	{
		goto ERR ;
	}

#ifndef DX_NON_MODEL
	// �{�����[���e�N�X�`���̍쐬
	CreateRGBtoVMaxRGBVolumeTexture() ;
#endif // DX_NON_MODEL

	// �t���X�N���[�����[�h�̏ꍇ�̓T�u�o�b�N�o�b�t�@�̃Z�b�g�A�b�v���s��
	if( GetWindowModeFlag() == FALSE )
	{
		if( Graphics_D3D11_SetupSubBackBuffer() != 0 )
		{
			goto ERR ;
		}
	}

	// �P���]�������̏�����
	if( Graphics_D3D11_StretchRect_Initialize() != 0 )
	{
		goto ERR ;
	}

	GSYS.Setting.ValidHardware = TRUE ;

	// �I��
	return 0 ;

	// �G���[�I��
ERR :

	// ��n�����s��
	Graphics_D3D11_Terminate() ;

	return -1 ;
}

// Direct3D11 ���g�p�����O���t�B�b�N�X�����̌�n�����s��
extern int Graphics_D3D11_Terminate( void )
{
	int i ;

#ifndef DX_NON_MODEL
	// ���f���̒��_�o�b�t�@�����
	MV1TerminateVertexBufferAll() ;
#endif // DX_NON_MODEL

	// �P���]�������̌�n��
	Graphics_D3D11_StretchRect_Terminate() ;

	// �e�N�X�`���։摜��]�����邽�߂̃������̌�n�����s��
	Graphics_D3D11_Texture_TerminateCommonBuffer() ;

	// �T���v���[�X�e�[�g�̉��
	Graphics_D3D11_DeviceState_TerminateSamplerState() ;

	// ���X�^���C�U�[�X�e�[�g�̉��
	Graphics_D3D11_DeviceState_TerminateRasterizerState() ;

	// �[�x�X�e���V���X�e�[�g�̉��
	Graphics_D3D11_DeviceState_TerminateDepthStencilState() ;

	// �u�����h�X�e�[�g�̉��
	Graphics_D3D11_DeviceState_TerminateBlendState() ;

	// �ėp�`��p���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@�̉��
	Graphics_D3D11_CommonVertexBuffer_Terminate() ;
	Graphics_D3D11_CommonIndexBuffer_Terminate() ;

	// �W���`��p�̒��_�o�b�t�@�̉��
	Graphics_D3D11_NormalDrawVertexBuffer_Terminate() ;

	// ���̓��C�A�E�g���̌�n��
	Graphics_D3D11_InputLayout_Base_Terminate() ;
#ifndef DX_NON_MODEL
	Graphics_D3D11_InputLayout_Model_Terminate() ;
#endif // DX_NON_MODEL

	// �V�F�[�_�[�̉��
	Graphics_D3D11_Shader_Terminate() ;

	// �萔���̌�n��
	Graphics_D3D11_Constant_Terminate() ;

	// �t���X�N���[�����[�h�ɂ��Ă����ꍇ�͂����ŃE�C���h�E���[�h�ɖ߂�
	if( GD3D11.Device.Screen.FullscreenSatte )
	{
		GD3D11.Device.Screen.FullscreenSatte = FALSE ;

		if( GD3D11.Device.Screen.OutputWindowInfo[ 0 ].DXGISwapChain != NULL )
		{
			DXGISwapChain_SetFullscreenState( GD3D11.Device.Screen.OutputWindowInfo[ 0 ].DXGISwapChain, FALSE, NULL ) ;
		}
	}

	// Z�o�b�t�@ �̉��
	if( GD3D11.Device.Screen.DepthBufferDSV )
	{
		Direct3D11_Release_DepthStencilView( GD3D11.Device.Screen.DepthBufferDSV ) ;
		GD3D11.Device.Screen.DepthBufferDSV = NULL ;
	}
	if( GD3D11.Device.Screen.DepthBufferTexture2D )
	{
		Direct3D11_Release_Texture2D( GD3D11.Device.Screen.DepthBufferTexture2D ) ;
		GD3D11.Device.Screen.DepthBufferTexture2D = NULL ;
	}

	// �T�u�o�b�N�o�b�t�@�̉��
	if( GD3D11.Device.Screen.SubBackBufferSRV )
	{
		Direct3D11_Release_ShaderResourceView( GD3D11.Device.Screen.SubBackBufferSRV ) ;
		GD3D11.Device.Screen.SubBackBufferSRV = NULL ;
	}
	if( GD3D11.Device.Screen.SubBackBufferRTV )
	{
		Direct3D11_Release_RenderTargetView( GD3D11.Device.Screen.SubBackBufferRTV ) ;
		GD3D11.Device.Screen.SubBackBufferRTV = NULL ;
	}
	if( GD3D11.Device.Screen.SubBackBufferTexture2D )
	{
		Direct3D11_Release_Texture2D( GD3D11.Device.Screen.SubBackBufferTexture2D ) ;
		GD3D11.Device.Screen.SubBackBufferTexture2D = NULL ;
	}

#ifndef DX_NON_MODEL
	// �{�����[���e�N�X�`���̉��
	if( GD3D11.RGBtoVMaxRGBVolumeTextureSRV )
	{
		Direct3D11_Release_ShaderResourceView( GD3D11.RGBtoVMaxRGBVolumeTextureSRV ) ;
		GD3D11.RGBtoVMaxRGBVolumeTextureSRV = NULL ;
	}

	if( GD3D11.RGBtoVMaxRGBVolumeTexture )
	{
		Direct3D11_Release_Texture3D( GD3D11.RGBtoVMaxRGBVolumeTexture ) ;
		GD3D11.RGBtoVMaxRGBVolumeTexture = NULL ;
	}
#endif // DX_NON_MODEL

	// �X���b�v�`�F�C�������
	for( i = 0 ; i < DX_D3D11_MAX_OUTPUTWINDOW ; i ++ )
	{
		Graphics_D3D11_OutputWindow_Sub( NULL, i ) ;
	}

	// D3D11Device �̉��
	D3D11Device_Release() ;
	GD3D11.Device.DrawInfo.BeginSceneFlag = FALSE ;

	// DXGIFactory �̉��
	DXGIFactory_Release() ;

	// d3d11.dll �̉��
	Direct3D11_FreeDLL() ;

	// dxgi.dll �̉��
	DXGI_FreeDLL() ;

	// Direct3D11 �̎c��I�u�W�F�N�g�����o��
	Direct3D11_DumpObject() ;

	// �f�o�C�X�Z�b�g����������
	_MEMSET( GD3D11.Device.State.SamplerState, 0, sizeof( GD3D11.Device.State.SamplerState ) ) ;
	GD3D11.Device.DrawInfo.VertexBufferNextAddr	= NULL ;
	GD3D11.Device.DrawInfo.VertexBufferAddr		= NULL ;
	GD3D11.Device.State.RasterizerState      = NULL ;
	GD3D11.Device.State.DepthStencilState    = NULL ;
	GD3D11.Device.State.BlendState           = NULL ;
	_MEMSET( GD3D11.Device.State.PSSetShaderResourceView, 0, sizeof( GD3D11.Device.State.PSSetShaderResourceView ) ) ;
	GD3D11.Device.State.SetVertexInputLayout = NULL ;
	GD3D11.Device.State.SetVertexShader      = NULL ;
	GD3D11.Device.State.SetPixelShader       = NULL ;
	GD3D11.Device.State.SetIndexBuffer       = NULL ;
	GD3D11.Device.State.SetVertexBuffer      = NULL ;
	_MEMSET( GD3D11.Device.State.TargetTexture2D, 0, sizeof( GD3D11.Device.State.TargetTexture2D ) ) ;
	_MEMSET( GD3D11.Device.State.TargetRTV,       0, sizeof( GD3D11.Device.State.TargetRTV ) ) ;
	GD3D11.Device.State.TargetDepthTexture2D = NULL ;
	GD3D11.Device.State.TargetDepthDSV       = NULL ;

	// �I��
	return 0 ;
}

// ���ׂĂ� Direct3D11 �n�I�u�W�F�N�g���������
extern int Graphics_D3D11_ReleaseObjectAll( void )
{
	int i ;
	SHADERHANDLEDATA                          *Shader ;
	VERTEXBUFFERHANDLEDATA                    *VertexBuffer ;
	INDEXBUFFERHANDLEDATA                     *IndexBuffer ;
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

			Graphics_Hardware_D3D11_ShadowMap_ReleaseTexture_PF( ShadowMap  ) ;
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

			Graphics_D3D11_VertexBuffer_ReleaseObject( VertexBuffer ) ;
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

			Graphics_D3D11_IndexBuffer_ReleaseObject( IndexBuffer ) ;
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
				if( Shader->PF->D3D11.VertexShader )
				{
					Direct3D11_Release_VertexShader( Shader->PF->D3D11.VertexShader ) ;
					Shader->PF->D3D11.VertexShader = NULL ;
				}
			}
			else
			{
				if( Shader->PF->D3D11.PixelShader )
				{
					Direct3D11_Release_PixelShader( Shader->PF->D3D11.PixelShader ) ;
					Shader->PF->D3D11.PixelShader = NULL ;
				}
			}
		}
	}

	// �f�o�C�X�Z�b�g����������
//	State �n�͍쐬���ăZ�b�g���Ă�����̂�����̂ŁA�����������͂��Ȃ�
//	_MEMSET( GD3D11.Device.State.SamplerState, 0, sizeof( GD3D11.Device.State.SamplerState ) ) ;
//	GD3D11.Device.State.RasterizerState      = NULL ;
//	GD3D11.Device.State.DepthStencilState    = NULL ;
//	GD3D11.Device.State.BlendState           = NULL ;
	_MEMSET( GD3D11.Device.State.PSSetShaderResourceView, 0, sizeof( GD3D11.Device.State.PSSetShaderResourceView ) ) ;
	GD3D11.Device.State.SetVertexInputLayout = NULL ;
	GD3D11.Device.State.SetVertexShader      = NULL ;
	GD3D11.Device.State.SetPixelShader       = NULL ;
	GD3D11.Device.State.SetIndexBuffer       = NULL ;
	GD3D11.Device.State.SetVertexBuffer      = NULL ;
	_MEMSET( GD3D11.Device.State.TargetTexture2D, 0, sizeof( GD3D11.Device.State.TargetTexture2D ) ) ;
	_MEMSET( GD3D11.Device.State.TargetRTV,       0, sizeof( GD3D11.Device.State.TargetRTV ) ) ;
	GD3D11.Device.State.TargetDepthTexture2D = NULL ;
	GD3D11.Device.State.TargetDepthDSV       = NULL ;

	// �I��
	return 0 ;
}

// ���ׂĂ� Direct3D11 �n�I�u�W�F�N�g���쐬����
extern int Graphics_D3D11_CreateObjectAll( void )
{
	int                                       i ;
	SHADERHANDLEDATA                          *Shader ;
	VERTEXBUFFERHANDLEDATA                    *VertexBuffer ;
	INDEXBUFFERHANDLEDATA                     *IndexBuffer ;
	IMAGEDATA                                 *Image ;
	SHADOWMAPDATA                             *ShadowMap ;

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
			Graphics_Hardware_D3D11_ShadowMap_CreateTexture_PF( ShadowMap ) ;
		}
	}

	if( HandleManageArray[ DX_HANDLETYPE_VERTEX_BUFFER ].InitializeFlag )
	{
		for( i = HandleManageArray[ DX_HANDLETYPE_VERTEX_BUFFER ].AreaMin ; i <= HandleManageArray[ DX_HANDLETYPE_VERTEX_BUFFER ].AreaMax ; i ++ )
		{
			VertexBuffer = ( VERTEXBUFFERHANDLEDATA * )HandleManageArray[ DX_HANDLETYPE_VERTEX_BUFFER ].Handle[ i ] ;
			if( VertexBuffer == NULL ) continue ;
			Graphics_D3D11_VertexBuffer_CreateObject( VertexBuffer, TRUE ) ;
		}
	}

	if( HandleManageArray[ DX_HANDLETYPE_INDEX_BUFFER ].InitializeFlag )
	{
		for( i = HandleManageArray[ DX_HANDLETYPE_INDEX_BUFFER ].AreaMin ; i <= HandleManageArray[ DX_HANDLETYPE_INDEX_BUFFER ].AreaMax ; i ++ )
		{
			IndexBuffer = ( INDEXBUFFERHANDLEDATA * )HandleManageArray[ DX_HANDLETYPE_INDEX_BUFFER ].Handle[ i ] ;
			if( IndexBuffer == NULL ) continue ;
			Graphics_D3D11_IndexBuffer_CreateObject( IndexBuffer, TRUE ) ;
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
				D3D11Device_CreateVertexShader( ( BYTE * )Shader->FunctionCode, ( SIZE_T )Shader->FunctionCodeSize, NULL, &Shader->PF->D3D11.VertexShader ) ;
			}
			else
			{
				D3D11Device_CreatePixelShader(  ( BYTE * )Shader->FunctionCode, ( SIZE_T )Shader->FunctionCodeSize, NULL, &Shader->PF->D3D11.PixelShader ) ;
			}
		}
	}

	// �I��
	return 0 ;
}
















// �V�F�[�_�[�R�[�h�֌W

// �V�F�[�_�[�R�[�h�p�b�P�[�W����V�F�[�_�[�R�[�h�o�C�i���̃A�h���X�ƃT�C�Y���擾����
static void Graphics_D3D11_ShaderCodePackage_GetInfo( WORD **pSize, BYTE **pCode, GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO *DestInfo, int Num )
{
	WORD *Size ;
	BYTE *Code ;
	int i ;

	Size = *pSize ;
	Code = *pCode ;
	for( i = 0 ; i < Num ; i ++, Size ++, DestInfo ++ )
	{
		if( *Size != 0 )
		{
			DestInfo->Binary = ( void * )Code ;
			DestInfo->Size   = *Size ;
			Code            += *Size ;
		}
	}

	*pSize = Size ;
	*pCode = Code ;
}

// Direct3D11 �̕W���`��p�̃V�F�[�_�[�R�[�h�̏��������s��
extern int Graphics_D3D11_ShaderCode_Base_Initialize( void )
{
	GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_BASE *SCBASE = &GraphicsHardDataDirect3D11.ShaderCode.Base ;
	int   Size = 0 ;
	BYTE  *ShaderAddr = NULL ;
	WORD  *SizeBuf    = NULL ;

	// ���łɏ���������Ă����牽�����Ȃ�
	if( SCBASE->BaseShaderInitializeFlag == TRUE )
	{
		return TRUE ;
	}

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
								SCBASE->LightIndexList84[ l0 ][ l1 ][ l2 ][ l3 ][ l4 ][ l5 ] = ( short )ind ;
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
					SCBASE->LightIndexList20[ l0 ][ l1 ][ l2 ] = ( short )ind ;
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
					SCBASE->LightIndexList10[ l0 ][ l1 ][ l2 ] = ( short )ind ;
					ind ++ ;
				}
			}
		}
	}

#ifndef DX_NON_FILTER
	SCBASE->FilterShaderBinDxaImage = NULL ;
	SCBASE->RGBAMixS_ShaderPackImage = NULL ;
#endif // DX_NON_FILTER

	SCBASE->Base2DShaderPackageImage = NULL ;

	// �W���`��p�V�F�[�_�[�I�u�W�F�N�g�t�@�C���c�w�`�����k�����f�[�^���𓀂���
	{
		if( DxShaderCodeBin_Base_D3D11Convert == 0 )
		{
			DxShaderCodeBin_Base_D3D11Convert = 1 ;
			Char128ToBin( DxShaderCodeBin_Base_D3D11, DxShaderCodeBin_Base_D3D11 ) ;
		}
		Size = DXA_Decode( DxShaderCodeBin_Base_D3D11, NULL ) ;
		SCBASE->Base2DShaderPackageImage = DXALLOC( ( size_t )Size ) ;
		if( SCBASE->Base2DShaderPackageImage == NULL )
		{
			goto ERR ;
		}

		DXA_Decode( DxShaderCodeBin_Base_D3D11, SCBASE->Base2DShaderPackageImage ) ;
	}

	// �A�h���X���X�g�̃Z�b�g
	SizeBuf    = ( WORD * )SCBASE->Base2DShaderPackageImage ;
	ShaderAddr = ( BYTE * )SCBASE->Base2DShaderPackageImage +
		sizeof( WORD ) * (
			sizeof( SCBASE->BaseSimple_VS_Code               ) / sizeof( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO ) +
			sizeof( SCBASE->BaseNoneTex_PS_Code              ) / sizeof( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO ) +
			sizeof( SCBASE->BaseUseTex_PS_Code               ) / sizeof( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO ) +
			sizeof( SCBASE->MaskEffect_PS_Code               ) / sizeof( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO ) +
			sizeof( SCBASE->MaskEffect_ReverseEffect_PS_Code ) / sizeof( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO ) +
			sizeof( SCBASE->MaskEffect_UseGraphHandle_PS_Code               ) / sizeof( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO ) +
			sizeof( SCBASE->MaskEffect_UseGraphHandle_ReverseEffect_PS_Code ) / sizeof( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO ) +
			sizeof( SCBASE->StretchRect_VS_Code              ) / sizeof( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO ) +
			sizeof( SCBASE->StretchRectTex8_VS_Code          ) / sizeof( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO ) +
			sizeof( SCBASE->StretchRect_PS_Code              ) / sizeof( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO ) ) ;
	Graphics_D3D11_ShaderCodePackage_GetInfo( &SizeBuf, &ShaderAddr, ( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO * )SCBASE->BaseSimple_VS_Code,                sizeof( SCBASE->BaseSimple_VS_Code               ) / sizeof( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO ) ) ;
	Graphics_D3D11_ShaderCodePackage_GetInfo( &SizeBuf, &ShaderAddr, ( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO * )SCBASE->BaseNoneTex_PS_Code,               sizeof( SCBASE->BaseNoneTex_PS_Code              ) / sizeof( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO ) ) ;
	Graphics_D3D11_ShaderCodePackage_GetInfo( &SizeBuf, &ShaderAddr, ( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO * )SCBASE->BaseUseTex_PS_Code,                sizeof( SCBASE->BaseUseTex_PS_Code               ) / sizeof( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO ) ) ;
	Graphics_D3D11_ShaderCodePackage_GetInfo( &SizeBuf, &ShaderAddr, ( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO * )&SCBASE->MaskEffect_PS_Code,               sizeof( SCBASE->MaskEffect_PS_Code               ) / sizeof( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO ) ) ;
	Graphics_D3D11_ShaderCodePackage_GetInfo( &SizeBuf, &ShaderAddr, ( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO * )&SCBASE->MaskEffect_ReverseEffect_PS_Code, sizeof( SCBASE->MaskEffect_ReverseEffect_PS_Code ) / sizeof( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO ) ) ;
	Graphics_D3D11_ShaderCodePackage_GetInfo( &SizeBuf, &ShaderAddr, ( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO * )SCBASE->MaskEffect_UseGraphHandle_PS_Code,               sizeof( SCBASE->MaskEffect_UseGraphHandle_PS_Code               ) / sizeof( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO ) ) ;
	Graphics_D3D11_ShaderCodePackage_GetInfo( &SizeBuf, &ShaderAddr, ( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO * )SCBASE->MaskEffect_UseGraphHandle_ReverseEffect_PS_Code, sizeof( SCBASE->MaskEffect_UseGraphHandle_ReverseEffect_PS_Code ) / sizeof( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO ) ) ;
	Graphics_D3D11_ShaderCodePackage_GetInfo( &SizeBuf, &ShaderAddr, ( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO * )&SCBASE->StretchRect_VS_Code,              sizeof( SCBASE->StretchRect_VS_Code              ) / sizeof( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO ) ) ;
	Graphics_D3D11_ShaderCodePackage_GetInfo( &SizeBuf, &ShaderAddr, ( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO * )&SCBASE->StretchRectTex8_VS_Code,          sizeof( SCBASE->StretchRectTex8_VS_Code          ) / sizeof( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO ) ) ;
	Graphics_D3D11_ShaderCodePackage_GetInfo( &SizeBuf, &ShaderAddr, ( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO * )&SCBASE->StretchRect_PS_Code,              sizeof( SCBASE->StretchRect_PS_Code              ) / sizeof( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO ) ) ;

#ifndef DX_NON_FILTER

	// RGBAMix�V�F�[�_�[�p�b�N���k�t�@�C����W�J����
	{
		if( DxShaderCodeBin_RgbaMix_D3D11Convert == 0 )
		{
			DxShaderCodeBin_RgbaMix_D3D11Convert = 1 ;
			Char128ToBin( DxShaderCodeBin_RgbaMix_D3D11, DxShaderCodeBin_RgbaMix_D3D11 ) ;
		}
		Size = DXA_Decode( DxShaderCodeBin_RgbaMix_D3D11, NULL ) ;
		SCBASE->RGBAMixS_ShaderPackImage = DXALLOC( ( size_t )Size ) ;
		if( SCBASE->RGBAMixS_ShaderPackImage == NULL )
		{
			goto ERR ;
		}

		DXA_Decode( DxShaderCodeBin_RgbaMix_D3D11, SCBASE->RGBAMixS_ShaderPackImage ) ;

		// �A�h���X���X�g�̃Z�b�g
		SizeBuf    = ( WORD  * )SCBASE->RGBAMixS_ShaderPackImage ;
		ShaderAddr = ( BYTE  * )SCBASE->RGBAMixS_ShaderPackImage +
			sizeof( WORD ) * (
				sizeof( SCBASE->RGBAMixS_PS_Code )  / sizeof( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO ) ) ;
		Graphics_D3D11_ShaderCodePackage_GetInfo( &SizeBuf, &ShaderAddr, ( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO * )SCBASE->RGBAMixS_PS_Code,  sizeof( SCBASE->RGBAMixS_PS_Code )  / sizeof( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO ) ) ;
	}

	// �t�B���^�[�V�F�[�_�[�I�u�W�F�N�g�t�@�C���c�w�`�����k�����f�[�^���𓀂���
	{
		if( DxShaderCodeBin_Filter_D3D11Convert == 0 )
		{
			DxShaderCodeBin_Filter_D3D11Convert = 1 ;
			Char128ToBin( DxShaderCodeBin_Filter_D3D11, DxShaderCodeBin_Filter_D3D11 ) ;
		}
		Size = DXA_Decode( DxShaderCodeBin_Filter_D3D11, NULL ) ;
		SCBASE->FilterShaderBinDxaImage = DXALLOC( ( size_t )Size ) ;
		if( SCBASE->FilterShaderBinDxaImage == NULL )
		{
			goto ERR ;
		}

		DXA_Decode( DxShaderCodeBin_Filter_D3D11, SCBASE->FilterShaderBinDxaImage ) ;

		// �c�w�`�t�@�C�����I�[�v������
		DXA_Initialize( &SCBASE->FilterShaderBinDxa ) ;
		if( DXA_OpenArchiveFromMem( &SCBASE->FilterShaderBinDxa, SCBASE->FilterShaderBinDxaImage, Size, FALSE, FALSE ) != 0 )
		{
			goto ERR ;
		}
	}

#endif // DX_NON_FILTER

	SCBASE->BaseShaderInitializeFlag = TRUE ;

	// ����I��
	return TRUE ;

ERR :
	if( SCBASE->Base2DShaderPackageImage != NULL )
	{
		DXFREE( SCBASE->Base2DShaderPackageImage ) ;
		SCBASE->Base2DShaderPackageImage = NULL ;
	}

#ifndef DX_NON_FILTER
	if( SCBASE->FilterShaderBinDxaImage != NULL )
	{
		DXFREE( SCBASE->FilterShaderBinDxaImage ) ;
		SCBASE->FilterShaderBinDxaImage = NULL ;
	}

	if( SCBASE->RGBAMixS_ShaderPackImage != NULL )
	{
		DXFREE( SCBASE->RGBAMixS_ShaderPackImage ) ;
		SCBASE->RGBAMixS_ShaderPackImage = NULL ;
	}
#endif // DX_NON_FILTER

	return FALSE ;
}

// Direct3D11 �̕W���`��p�̃V�F�[�_�[�R�[�h�̌�n�����s��
extern int Graphics_D3D11_ShaderCode_Base_Terminate( void )
{
	GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_BASE *SCBASE = &GraphicsHardDataDirect3D11.ShaderCode.Base ;

	// ���łɌ�n������Ă����牽�����Ȃ�
	if( SCBASE->BaseShaderInitializeFlag == FALSE )
	{
		return TRUE ;
	}

	// �𓀂����V�F�[�_�[���i�[���Ă����������̉��
	if( SCBASE->Base2DShaderPackageImage != NULL )
	{
		DXFREE( SCBASE->Base2DShaderPackageImage ) ;
		SCBASE->Base2DShaderPackageImage = NULL ;
	}

#ifndef DX_NON_FILTER

	// �t�B���^�[�V�F�[�_�[�p�c�w�`�̌�n��
	DXA_Terminate( &SCBASE->FilterShaderBinDxa ) ;

	if( SCBASE->RGBAMixS_ShaderPackImage )
	{
		DXFREE( SCBASE->RGBAMixS_ShaderPackImage ) ;
		SCBASE->RGBAMixS_ShaderPackImage = NULL ;
	}
	if( SCBASE->FilterShaderBinDxaImage )
	{
		DXFREE( SCBASE->FilterShaderBinDxaImage ) ;
		SCBASE->FilterShaderBinDxaImage = NULL ;
	}

#endif // DX_NON_FILTER

	// �������t���O��|��
	SCBASE->BaseShaderInitializeFlag = FALSE ;

	// ����I��
	return TRUE ;
}

// Direct3D11 �̕W���R�c�`��p�̃V�F�[�_�[�R�[�h�̏��������s��
extern int Graphics_D3D11_ShaderCode_Base3D_Initialize( void )
{
	GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_BASE3D *SCBASE3D = &GraphicsHardDataDirect3D11.ShaderCode.Base3D ;
	HANDLEMANAGE *HandleManage = &HandleManageArray[ DX_HANDLETYPE_GRAPH ] ;
	int   Size = 0 ;
	BYTE  *ShaderAddr = NULL ;
	WORD  *SizeBuf    = NULL ;

	// ���łɏ���������Ă����牽�����Ȃ�
	if( SCBASE3D->Base3DShaderInitializeFlag == TRUE )
	{
		return TRUE ;
	}

	// �N���e�B�J���Z�N�V�����̎擾
	CRITICALSECTION_LOCK( &HandleManage->CriticalSection ) ;

	// �N���e�B�J���Z�N�V�������擾������ŉ��߂ď������m�F
	if( SCBASE3D->Base3DShaderInitializeFlag == TRUE )
	{
		// �N���e�B�J���Z�N�V�����̉��
		CriticalSection_Unlock( &HandleManage->CriticalSection ) ;

		return TRUE ;
	}

	SCBASE3D->Base3DShaderPackageImage = NULL ;

	// ���k�f�[�^�̉�
	{
		if( DxShaderCodeBin_Base3D_D3D11Convert == 0 )
		{
			DxShaderCodeBin_Base3D_D3D11Convert = 1 ;
			Char128ToBin( DxShaderCodeBin_Base3D_D3D11, DxShaderCodeBin_Base3D_D3D11 ) ;
		}
		Size = DXA_Decode( DxShaderCodeBin_Base3D_D3D11, NULL ) ;
		SCBASE3D->Base3DShaderPackageImage = DXCALLOC( ( size_t )Size ) ;
		if( SCBASE3D->Base3DShaderPackageImage == NULL )
		{
			goto ERR ;
		}

		DXA_Decode( DxShaderCodeBin_Base3D_D3D11, SCBASE3D->Base3DShaderPackageImage ) ;
	}

	// �A�h���X���X�g�̃Z�b�g
	SizeBuf    = ( WORD * )SCBASE3D->Base3DShaderPackageImage ;
	ShaderAddr = ( BYTE * )SCBASE3D->Base3DShaderPackageImage +
		sizeof( WORD ) * (
			sizeof( SCBASE3D->Base3D_PixelLighting_VS_Code         ) / sizeof( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO ) +
			sizeof( SCBASE3D->Base3D_PixelLighting_Normal_PS_Code  ) / sizeof( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO ) +
			sizeof( SCBASE3D->Base3D_ShadowMap_VS_Code             ) / sizeof( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO ) +
			sizeof( SCBASE3D->Base3D_NoLighting_VS_Code            ) / sizeof( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO ) +
			sizeof( SCBASE3D->Base3D_VertexLighting_VS_Code        ) / sizeof( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO ) +
			sizeof( SCBASE3D->Base3D_ShadowMap_Normal_PS_Code      ) / sizeof( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO ) +
			sizeof( SCBASE3D->Base3D_NoLighting_Normal_PS_Code     ) / sizeof( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO ) +
			sizeof( SCBASE3D->Base3D_VertexLighting_Normal_PS_Code ) / sizeof( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO ) ) ;
	Graphics_D3D11_ShaderCodePackage_GetInfo( &SizeBuf, &ShaderAddr, ( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO * )SCBASE3D->Base3D_PixelLighting_VS_Code,         sizeof( SCBASE3D->Base3D_PixelLighting_VS_Code         ) / sizeof( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO ) ) ;
	Graphics_D3D11_ShaderCodePackage_GetInfo( &SizeBuf, &ShaderAddr, ( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO * )SCBASE3D->Base3D_PixelLighting_Normal_PS_Code,  sizeof( SCBASE3D->Base3D_PixelLighting_Normal_PS_Code  ) / sizeof( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO ) ) ;
	Graphics_D3D11_ShaderCodePackage_GetInfo( &SizeBuf, &ShaderAddr, ( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO * )SCBASE3D->Base3D_ShadowMap_VS_Code,             sizeof( SCBASE3D->Base3D_ShadowMap_VS_Code             ) / sizeof( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO ) ) ;
	Graphics_D3D11_ShaderCodePackage_GetInfo( &SizeBuf, &ShaderAddr, ( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO * )SCBASE3D->Base3D_NoLighting_VS_Code,            sizeof( SCBASE3D->Base3D_NoLighting_VS_Code            ) / sizeof( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO ) ) ;
	Graphics_D3D11_ShaderCodePackage_GetInfo( &SizeBuf, &ShaderAddr, ( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO * )SCBASE3D->Base3D_VertexLighting_VS_Code,        sizeof( SCBASE3D->Base3D_VertexLighting_VS_Code        ) / sizeof( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO ) ) ;
	Graphics_D3D11_ShaderCodePackage_GetInfo( &SizeBuf, &ShaderAddr, ( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO * )SCBASE3D->Base3D_ShadowMap_Normal_PS_Code,      sizeof( SCBASE3D->Base3D_ShadowMap_Normal_PS_Code      ) / sizeof( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO ) ) ;
	Graphics_D3D11_ShaderCodePackage_GetInfo( &SizeBuf, &ShaderAddr, ( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO * )SCBASE3D->Base3D_NoLighting_Normal_PS_Code,     sizeof( SCBASE3D->Base3D_NoLighting_Normal_PS_Code     ) / sizeof( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO ) ) ;
	Graphics_D3D11_ShaderCodePackage_GetInfo( &SizeBuf, &ShaderAddr, ( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO * )SCBASE3D->Base3D_VertexLighting_Normal_PS_Code, sizeof( SCBASE3D->Base3D_VertexLighting_Normal_PS_Code ) / sizeof( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO ) ) ;

	SCBASE3D->Base3DShaderInitializeFlag = TRUE ;

	// �N���e�B�J���Z�N�V�����̉��
	CriticalSection_Unlock( &HandleManage->CriticalSection ) ;

	// ����I��
	return TRUE ;

ERR :
	if( SCBASE3D->Base3DShaderPackageImage != NULL )
	{
		DXFREE( SCBASE3D->Base3DShaderPackageImage ) ;
		SCBASE3D->Base3DShaderPackageImage = NULL ;
	}

	// �N���e�B�J���Z�N�V�����̉��
	CriticalSection_Unlock( &HandleManage->CriticalSection ) ;

	return FALSE ;
}


// Direct3D11 �̕W���R�c�`��p�̃V�F�[�_�[�R�[�h�̌�n�����s��
extern int Graphics_D3D11_ShaderCode_Base3D_Terminate( void )
{
	GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_BASE3D *SCBASE3D = &GraphicsHardDataDirect3D11.ShaderCode.Base3D ;

	// ���łɌ�n������Ă����牽�����Ȃ�
	if( SCBASE3D->Base3DShaderInitializeFlag == FALSE )
	{
		return TRUE ;
	}

	if( SCBASE3D->Base3DShaderPackageImage )
	{
		DXFREE( SCBASE3D->Base3DShaderPackageImage ) ;
		SCBASE3D->Base3DShaderPackageImage = NULL ;
	}

	// �������t���O��|��
	SCBASE3D->Base3DShaderInitializeFlag = FALSE ;

	// ����I��
	return TRUE ;
}


#ifndef DX_NON_MODEL

// Direct3D11 �̃��f���`��p�̃V�F�[�_�[�R�[�h�̏��������s��
extern	int		Graphics_D3D11_ShaderCode_Model_Initialize( void )
{
	GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_MODEL *SCMODEL = &GraphicsHardDataDirect3D11.ShaderCode.Model ;
	HANDLEMANAGE *HandleManage = &HandleManageArray[ DX_HANDLETYPE_GRAPH ] ;
	int   Size = 0 ;
	BYTE  *ShaderAddr = NULL ;
	WORD  *SizeBuf    = NULL ;

	// �N���e�B�J���Z�N�V�����̎擾
	CRITICALSECTION_LOCK( &HandleManage->CriticalSection ) ;

	// ���łɏ���������Ă����牽�����Ȃ�
	if( SCMODEL->ModelShaderInitializeFlag == TRUE )
	{
		// �N���e�B�J���Z�N�V�����̉��
		CriticalSection_Unlock( &HandleManage->CriticalSection ) ;

		return TRUE ;
	}

	SCMODEL->ModelShaderPackImage = NULL ;

	// ���k�f�[�^�̉�
	{
		if( DxShaderCodeBin_Model_D3D11Convert == 0 )
		{
			DxShaderCodeBin_Model_D3D11Convert = 1 ;
			Char128ToBin( DxShaderCodeBin_Model_D3D11, DxShaderCodeBin_Model_D3D11 ) ;
		}
		Size = DXA_Decode( DxShaderCodeBin_Model_D3D11, NULL ) ;
		SCMODEL->ModelShaderPackImage = DXCALLOC( ( size_t )Size ) ;
		if( SCMODEL->ModelShaderPackImage == NULL )
		{
			goto ERR ;
		}

		DXA_Decode( DxShaderCodeBin_Model_D3D11, SCMODEL->ModelShaderPackImage ) ;
	}

	// �A�h���X���X�g�̃Z�b�g
	SizeBuf    = ( WORD * )SCMODEL->ModelShaderPackImage ;
	ShaderAddr = ( BYTE * )SCMODEL->ModelShaderPackImage +
		sizeof( WORD ) * (
			sizeof( SCMODEL->MV1_PixelLighting_VS_Code            ) / sizeof( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO ) +
			sizeof( SCMODEL->MV1_PixelLighting_ToonType1_PS_Code  ) / sizeof( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO ) +
			sizeof( SCMODEL->MV1_PixelLighting_ToonType2_PS_Code  ) / sizeof( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO ) +
			sizeof( SCMODEL->MV1_PixelLighting_Normal_PS_Code     ) / sizeof( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO ) +
			sizeof( SCMODEL->MV1_ToonOutLine_ShadowMap_VS_Code    ) / sizeof( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO ) +
			sizeof( SCMODEL->MV1_ToonOutLine_VS_Code              ) / sizeof( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO ) +
			sizeof( SCMODEL->MV1_ShadowMap_VS_Code                ) / sizeof( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO ) +
			sizeof( SCMODEL->MV1_NoLighting_VS_Code               ) / sizeof( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO ) +
			sizeof( SCMODEL->MV1_VertexLighting_VS_Code           ) / sizeof( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO ) +
			sizeof( SCMODEL->MV1_ShadowMap_Toon_PS_Code           ) / sizeof( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO ) +
			sizeof( SCMODEL->MV1_ShadowMap_Normal_PS_Code         ) / sizeof( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO ) +
			sizeof( SCMODEL->MV1_NoLighting_Toon_PS_Code          ) / sizeof( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO ) +
			sizeof( SCMODEL->MV1_NoLighting_Normal_PS_Code        ) / sizeof( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO ) +
			sizeof( SCMODEL->MV1_VertexLighting_ToonType1_PS_Code ) / sizeof( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO ) +
			sizeof( SCMODEL->MV1_VertexLighting_ToonType2_PS_Code ) / sizeof( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO ) +
			sizeof( SCMODEL->MV1_VertexLighting_Normal_PS_Code    ) / sizeof( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO ) ) ;
	Graphics_D3D11_ShaderCodePackage_GetInfo( &SizeBuf, &ShaderAddr, ( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO * )SCMODEL->MV1_PixelLighting_VS_Code,            sizeof( SCMODEL->MV1_PixelLighting_VS_Code            ) / sizeof( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO ) ) ;
	Graphics_D3D11_ShaderCodePackage_GetInfo( &SizeBuf, &ShaderAddr, ( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO * )SCMODEL->MV1_PixelLighting_ToonType1_PS_Code,  sizeof( SCMODEL->MV1_PixelLighting_ToonType1_PS_Code  ) / sizeof( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO ) ) ;
	Graphics_D3D11_ShaderCodePackage_GetInfo( &SizeBuf, &ShaderAddr, ( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO * )SCMODEL->MV1_PixelLighting_ToonType2_PS_Code,  sizeof( SCMODEL->MV1_PixelLighting_ToonType2_PS_Code  ) / sizeof( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO ) ) ;
	Graphics_D3D11_ShaderCodePackage_GetInfo( &SizeBuf, &ShaderAddr, ( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO * )SCMODEL->MV1_PixelLighting_Normal_PS_Code,     sizeof( SCMODEL->MV1_PixelLighting_Normal_PS_Code     ) / sizeof( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO ) ) ;
	Graphics_D3D11_ShaderCodePackage_GetInfo( &SizeBuf, &ShaderAddr, ( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO * )SCMODEL->MV1_ToonOutLine_ShadowMap_VS_Code,    sizeof( SCMODEL->MV1_ToonOutLine_ShadowMap_VS_Code    ) / sizeof( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO ) ) ;
	Graphics_D3D11_ShaderCodePackage_GetInfo( &SizeBuf, &ShaderAddr, ( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO * )SCMODEL->MV1_ToonOutLine_VS_Code,              sizeof( SCMODEL->MV1_ToonOutLine_VS_Code              ) / sizeof( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO ) ) ;
	Graphics_D3D11_ShaderCodePackage_GetInfo( &SizeBuf, &ShaderAddr, ( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO * )SCMODEL->MV1_ShadowMap_VS_Code,                sizeof( SCMODEL->MV1_ShadowMap_VS_Code                ) / sizeof( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO ) ) ;
	Graphics_D3D11_ShaderCodePackage_GetInfo( &SizeBuf, &ShaderAddr, ( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO * )SCMODEL->MV1_NoLighting_VS_Code,               sizeof( SCMODEL->MV1_NoLighting_VS_Code               ) / sizeof( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO ) ) ;
	Graphics_D3D11_ShaderCodePackage_GetInfo( &SizeBuf, &ShaderAddr, ( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO * )SCMODEL->MV1_VertexLighting_VS_Code,           sizeof( SCMODEL->MV1_VertexLighting_VS_Code           ) / sizeof( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO ) ) ;
	Graphics_D3D11_ShaderCodePackage_GetInfo( &SizeBuf, &ShaderAddr, ( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO * )SCMODEL->MV1_ShadowMap_Toon_PS_Code,           sizeof( SCMODEL->MV1_ShadowMap_Toon_PS_Code           ) / sizeof( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO ) ) ;
	Graphics_D3D11_ShaderCodePackage_GetInfo( &SizeBuf, &ShaderAddr, ( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO * )SCMODEL->MV1_ShadowMap_Normal_PS_Code,         sizeof( SCMODEL->MV1_ShadowMap_Normal_PS_Code         ) / sizeof( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO ) ) ;
	Graphics_D3D11_ShaderCodePackage_GetInfo( &SizeBuf, &ShaderAddr, ( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO * )SCMODEL->MV1_NoLighting_Toon_PS_Code,          sizeof( SCMODEL->MV1_NoLighting_Toon_PS_Code          ) / sizeof( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO ) ) ;
	Graphics_D3D11_ShaderCodePackage_GetInfo( &SizeBuf, &ShaderAddr, ( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO * )SCMODEL->MV1_NoLighting_Normal_PS_Code,        sizeof( SCMODEL->MV1_NoLighting_Normal_PS_Code        ) / sizeof( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO ) ) ;
	Graphics_D3D11_ShaderCodePackage_GetInfo( &SizeBuf, &ShaderAddr, ( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO * )SCMODEL->MV1_VertexLighting_ToonType1_PS_Code, sizeof( SCMODEL->MV1_VertexLighting_ToonType1_PS_Code ) / sizeof( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO ) ) ;
	Graphics_D3D11_ShaderCodePackage_GetInfo( &SizeBuf, &ShaderAddr, ( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO * )SCMODEL->MV1_VertexLighting_ToonType2_PS_Code, sizeof( SCMODEL->MV1_VertexLighting_ToonType2_PS_Code ) / sizeof( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO ) ) ;
	Graphics_D3D11_ShaderCodePackage_GetInfo( &SizeBuf, &ShaderAddr, ( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO * )SCMODEL->MV1_VertexLighting_Normal_PS_Code,    sizeof( SCMODEL->MV1_VertexLighting_Normal_PS_Code    ) / sizeof( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO ) ) ;

	SCMODEL->ModelShaderInitializeFlag = TRUE ;

	// �N���e�B�J���Z�N�V�����̉��
	CriticalSection_Unlock( &HandleManage->CriticalSection ) ;

	// ����I��
	return TRUE ;

ERR :
	if( SCMODEL->ModelShaderPackImage != NULL )
	{
		DXFREE( SCMODEL->ModelShaderPackImage ) ;
		SCMODEL->ModelShaderPackImage = NULL ;
	}

	// �N���e�B�J���Z�N�V�����̉��
	CriticalSection_Unlock( &HandleManage->CriticalSection ) ;

	return FALSE ;
}

// Direct3D11 �̃��f���`��p�̃V�F�[�_�[�R�[�h�̌�n�����s��
extern	int		Graphics_D3D11_ShaderCode_Model_Terminate( void )
{
	GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_MODEL *SCMODEL = &GraphicsHardDataDirect3D11.ShaderCode.Model ;

	// ���łɌ�n������Ă����牽�����Ȃ�
	if( SCMODEL->ModelShaderInitializeFlag == FALSE )
	{
		return TRUE ;
	}

	if( SCMODEL->ModelShaderPackImage )
	{
		DXFREE( SCMODEL->ModelShaderPackImage ) ;
		SCMODEL->ModelShaderPackImage = NULL ;
	}

	// �������t���O��|��
	SCMODEL->ModelShaderInitializeFlag = FALSE ;

	// ����I��
	return TRUE ;
}

#endif // DX_NON_MODEL



// �V�F�[�_�[�R�[�h���璸�_�V�F�[�_�[���쐬����
extern int Graphics_D3D11_VertexShader_Create( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO *Code, D_ID3D11VertexShader **pDestShader, int Num )
{
	int i ;

	for( i = 0; i < Num; i++, Code++, pDestShader++ )
	{
		if( D3D11Device_CreateVertexShader( ( BYTE * )Code->Binary, ( SIZE_T )Code->Size, NULL, pDestShader ) != S_OK )
		{
			return -1 + i ;
		}
	}

	return 0 ;
}

// �V�F�[�_�[�R�[�h����s�N�Z���V�F�[�_�[���쐬����
extern int Graphics_D3D11_PixelShader_Create( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO *Code, D_ID3D11PixelShader **pDestShader, int Num )
{
	int i ;

	for( i = 0; i < Num; i++, Code++, pDestShader++ )
	{
		if( D3D11Device_CreatePixelShader( ( BYTE * )Code->Binary, ( SIZE_T )Code->Size, NULL, pDestShader ) != S_OK )
		{
			return -1 + i ;
		}
	}

	return 0 ;
}

// ���_�V�F�[�_�[�z����������
static void Graphics_D3D11_VertexShaderArray_Release( D_ID3D11VertexShader **pObject, int Num )
{
	int i ;

	for( i = 0 ; i < Num ; i++, pObject++ )
	{
		if( *pObject )
		{
			Direct3D11_Release_VertexShader( *pObject ) ;
			*pObject = NULL ;
		}
	}
}

// �s�N�Z���V�F�[�_�[�z����������
static void Graphics_D3D11_PixelShaderArray_Release( D_ID3D11PixelShader **pObject, int Num )
{
	int i ;

	for( i = 0 ; i < Num ; i++, pObject++ )
	{
		if( *pObject )
		{
			Direct3D11_Release_PixelShader( *pObject ) ;
			*pObject = NULL ;
		}
	}
}

// �u�����h�X�e�[�g�z����������
static void Graphics_D3D11_BlendStateArray_Release( D_ID3D11BlendState **pObject, int Num )
{
	int i ;

	for( i = 0 ; i < Num ; i++, pObject++ )
	{
		if( *pObject )
		{
			Direct3D11_Release_BlendState( *pObject ) ;
			*pObject = NULL ;
		}
	}
}

// �f�v�X�X�e���V���X�e�[�g�z����������
static void Graphics_D3D11_DepthStencilStateArray_Release( D_ID3D11DepthStencilState **pObject, int Num )
{
	int i ;

	for( i = 0 ; i < Num ; i++, pObject++ )
	{
		if( *pObject )
		{
			Direct3D11_Release_DepthStencilState( *pObject ) ;
			*pObject = NULL ;
		}
	}
}

// ���X�^���C�U�[�X�e�[�g�z����������
static void Graphics_D3D11_RasterizerStateArray_Release( D_ID3D11RasterizerState **pObject, int Num )
{
	int i ;

	for( i = 0 ; i < Num ; i++, pObject++ )
	{
		if( *pObject )
		{
			Direct3D11_Release_RasterizerState( *pObject ) ;
			*pObject = NULL ;
		}
	}
}

// �T���v���[�X�e�[�g�z����������
static void Graphics_D3D11_SamplerStateArray_Release( D_ID3D11SamplerState **pObject, int Num )
{
	int i ;

	for( i = 0 ; i < Num ; i++, pObject++ )
	{
		if( *pObject )
		{
			Direct3D11_Release_SamplerState( *pObject ) ;
			*pObject = NULL ;
		}
	}
}

// Direct3D11 �̃V�F�[�_�[�̏��������s��
extern int Graphics_D3D11_Shader_Initialize( void )
{
	GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE      *ShaderCode = &GraphicsHardDataDirect3D11.ShaderCode ;
	GRAPHICS_HARDWARE_DIRECT3D11_SHADER          *Shader     = &GraphicsHardDataDirect3D11.Device.Shader ;
	int Result ;

	// ���ɍ쐬����Ă����Ƃ��̂��߂ɍ폜�������s��
	//TerminateDirect3D11Shader() ;
	Graphics_D3D11_Shader_Terminate() ;

//	return 0 ;

	DXST_ERRORLOG_ADDUTF16LE( "\xb7\x30\xa7\x30\xfc\x30\xc0\x30\xfc\x30\xb3\x30\xfc\x30\xc9\x30\xa2\x95\xc2\x4f\x6e\x30\x1d\x52\x1f\x67\x16\x53\x2e\x00\x2e\x00\x2e\x00\x2e\x00\x20\x00\x00"/*@ L"�V�F�[�_�[�R�[�h�֌W�̏�����.... " @*/ ) ;

	// Direct3D11 �̏ꍇ�̓V�F�[�_�[�͕K���g�p�ł���
	GSYS.HardInfo.UseShader = TRUE ;

	// �W���`��p�V�F�[�_�[�R�[�h���W�J����Ă��Ȃ��ꍇ�͓W�J
	if( Graphics_D3D11_ShaderCode_Base_Initialize() == FALSE )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\xb7\x30\xa7\x30\xfc\x30\xc0\x30\xfc\x30\xb3\x30\xfc\x30\xc9\x30\x92\x30\x55\x5c\x8b\x95\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�V�F�[�_�[�R�[�h��W�J���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
		GSYS.HardInfo.UseShader = FALSE ;
		goto ERR ;
	}

	// ��{�I�ȕ`�揈���Ɏg�p����V�F�[�_�[���쐬����
	{
		// �W���`��p�̕��G�ȏ������s��Ȃ����_�V�F�[�_�[�̍쐬
		Result = Graphics_D3D11_VertexShader_Create( ( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO * )ShaderCode->Base.BaseSimple_VS_Code, ( D_ID3D11VertexShader ** )Shader->Base.BaseSimple_VS, sizeof( ShaderCode->Base.BaseSimple_VS_Code ) / sizeof( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO ) ) ;
		if( Result < 0 )
		{
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\x19\x6a\x96\x6e\xcf\x63\x3b\x75\x28\x75\x6e\x30\x07\x89\xd1\x96\x6a\x30\xe6\x51\x06\x74\x92\x30\x4c\x88\x8f\x30\x6a\x30\x44\x30\x02\x98\xb9\x70\xb7\x30\xa7\x30\xfc\x30\xc0\x30\xfc\x30\xaa\x30\xd6\x30\xb8\x30\xa7\x30\xaf\x30\xc8\x30\x20\x00\x4e\x00\x6f\x00\x2e\x00\x25\x00\x64\x00\x20\x00\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�W���`��p�̕��G�ȏ������s��Ȃ����_�V�F�[�_�[�I�u�W�F�N�g No.%d �̍쐬�Ɏ��s���܂���\n" @*/, -( Result + 1 ) ) ) ;
			GSYS.HardInfo.UseShader = FALSE ;
			goto ERR ;
		}

		// �Œ�@�\�p�C�v���C���݊��̃s�N�Z���V�F�[�_�[( �e�N�X�`���Ȃ� )�̍쐬
		Result = Graphics_D3D11_PixelShader_Create( ( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO * )ShaderCode->Base.BaseNoneTex_PS_Code, ( D_ID3D11PixelShader ** )Shader->Base.BaseNoneTex_PS, sizeof( ShaderCode->Base.BaseNoneTex_PS_Code ) / sizeof( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO ) ) ;
		if( Result < 0 )
		{
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\xfa\x56\x9a\x5b\x5f\x6a\xfd\x80\xd1\x30\xa4\x30\xd7\x30\xe9\x30\xa4\x30\xf3\x30\x92\x4e\xdb\x63\x6e\x30\xd4\x30\xaf\x30\xbb\x30\xeb\x30\xb7\x30\xa7\x30\xfc\x30\xc0\x30\xfc\x30\xaa\x30\xd6\x30\xb8\x30\xa7\x30\xaf\x30\xc8\x30\x28\x00\x20\x00\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\x6a\x30\x57\x30\x20\x00\x29\x00\x20\x00\x4e\x00\x6f\x00\x2e\x00\x25\x00\x64\x00\x20\x00\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�Œ�@�\�p�C�v���C���݊��̃s�N�Z���V�F�[�_�[�I�u�W�F�N�g( �e�N�X�`���Ȃ� ) No.%d �̍쐬�Ɏ��s���܂���\n" @*/, -( Result + 1 ) ) ) ;
			GSYS.HardInfo.UseShader = FALSE ;
			goto ERR ;
		}

		// �Œ�@�\�p�C�v���C���݊��̃s�N�Z���V�F�[�_�[( �e�N�X�`������ )�̍쐬
		Result = Graphics_D3D11_PixelShader_Create( ( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO * )ShaderCode->Base.BaseUseTex_PS_Code, ( D_ID3D11PixelShader ** )Shader->Base.BaseUseTex_PS, sizeof( ShaderCode->Base.BaseUseTex_PS_Code ) / sizeof( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO ) ) ;
		if( Result < 0 )
		{
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\xfa\x56\x9a\x5b\x5f\x6a\xfd\x80\xd1\x30\xa4\x30\xd7\x30\xe9\x30\xa4\x30\xf3\x30\x92\x4e\xdb\x63\x6e\x30\xd4\x30\xaf\x30\xbb\x30\xeb\x30\xb7\x30\xa7\x30\xfc\x30\xc0\x30\xfc\x30\xaa\x30\xd6\x30\xb8\x30\xa7\x30\xaf\x30\xc8\x30\x28\x00\x20\x00\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\x42\x30\x8a\x30\x20\x00\x29\x00\x20\x00\x4e\x00\x6f\x00\x2e\x00\x25\x00\x64\x00\x20\x00\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�Œ�@�\�p�C�v���C���݊��̃s�N�Z���V�F�[�_�[�I�u�W�F�N�g( �e�N�X�`������ ) No.%d �̍쐬�Ɏ��s���܂���\n" @*/, -( Result + 1 ) ) ) ;
			GSYS.HardInfo.UseShader = FALSE ;
			goto ERR ;
		}

		// �}�X�N�����p�s�N�Z���V�F�[�_�[�̍쐬
		Result = Graphics_D3D11_PixelShader_Create( ( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO * )&ShaderCode->Base.MaskEffect_PS_Code, ( D_ID3D11PixelShader ** )&Shader->Base.MaskEffect_PS, sizeof( ShaderCode->Base.MaskEffect_PS_Code ) / sizeof( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO ) ) ;
		if( Result < 0 )
		{
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\xde\x30\xb9\x30\xaf\x30\xe6\x51\x06\x74\x28\x75\x6e\x30\xd4\x30\xaf\x30\xbb\x30\xeb\x30\xb7\x30\xa7\x30\xfc\x30\xc0\x30\xfc\x30\xaa\x30\xd6\x30\xb8\x30\xa7\x30\xaf\x30\xc8\x30\x20\x00\x4e\x00\x6f\x00\x2e\x00\x25\x00\x64\x00\x20\x00\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�}�X�N�����p�̃s�N�Z���V�F�[�_�[�I�u�W�F�N�g No.%d �̍쐬�Ɏ��s���܂���\n" @*/, -( Result + 1 ) ) ) ;
			GSYS.HardInfo.UseShader = FALSE ;
			goto ERR ;
		}
		Result = Graphics_D3D11_PixelShader_Create( ( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO * )&ShaderCode->Base.MaskEffect_ReverseEffect_PS_Code, ( D_ID3D11PixelShader ** )&Shader->Base.MaskEffect_ReverseEffect_PS, sizeof( ShaderCode->Base.MaskEffect_ReverseEffect_PS_Code ) / sizeof( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO ) ) ;
		if( Result < 0 )
		{
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\xcd\x53\xe2\x8e\xb9\x52\x9c\x67\xde\x30\xb9\x30\xaf\x30\xe6\x51\x06\x74\x28\x75\x6e\x30\xd4\x30\xaf\x30\xbb\x30\xeb\x30\xb7\x30\xa7\x30\xfc\x30\xc0\x30\xfc\x30\xaa\x30\xd6\x30\xb8\x30\xa7\x30\xaf\x30\xc8\x30\x20\x00\x4e\x00\x6f\x00\x2e\x00\x25\x00\x64\x00\x20\x00\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"���]���ʃ}�X�N�����p�̃s�N�Z���V�F�[�_�[�I�u�W�F�N�g No.%d �̍쐬�Ɏ��s���܂���\n" @*/, -( Result + 1 ) ) ) ;
			GSYS.HardInfo.UseShader = FALSE ;
			goto ERR ;
		}
		Result = Graphics_D3D11_PixelShader_Create( ( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO * )ShaderCode->Base.MaskEffect_UseGraphHandle_PS_Code, ( D_ID3D11PixelShader ** )Shader->Base.MaskEffect_UseGraphHandle_PS, sizeof( ShaderCode->Base.MaskEffect_UseGraphHandle_PS_Code ) / sizeof( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO ) ) ;
		if( Result < 0 )
		{
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\xde\x30\xb9\x30\xaf\x30\xe6\x51\x06\x74\x28\x75\x6e\x30\xd4\x30\xaf\x30\xbb\x30\xeb\x30\xb7\x30\xa7\x30\xfc\x30\xc0\x30\xfc\x30\xaa\x30\xd6\x30\xb8\x30\xa7\x30\xaf\x30\xc8\x30\x20\x00\x4e\x00\x6f\x00\x2e\x00\x25\x00\x64\x00\x20\x00\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�}�X�N�����p�̃s�N�Z���V�F�[�_�[�I�u�W�F�N�g No.%d �̍쐬�Ɏ��s���܂���\n" @*/, -( Result + 1 ) ) ) ;
			GSYS.HardInfo.UseShader = FALSE ;
			goto ERR ;
		}
		Result = Graphics_D3D11_PixelShader_Create( ( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO * )ShaderCode->Base.MaskEffect_UseGraphHandle_ReverseEffect_PS_Code, ( D_ID3D11PixelShader ** )Shader->Base.MaskEffect_UseGraphHandle_ReverseEffect_PS, sizeof( ShaderCode->Base.MaskEffect_UseGraphHandle_ReverseEffect_PS_Code ) / sizeof( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO ) ) ;
		if( Result < 0 )
		{
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\xcd\x53\xe2\x8e\xb9\x52\x9c\x67\xde\x30\xb9\x30\xaf\x30\xe6\x51\x06\x74\x28\x75\x6e\x30\xd4\x30\xaf\x30\xbb\x30\xeb\x30\xb7\x30\xa7\x30\xfc\x30\xc0\x30\xfc\x30\xaa\x30\xd6\x30\xb8\x30\xa7\x30\xaf\x30\xc8\x30\x20\x00\x4e\x00\x6f\x00\x2e\x00\x25\x00\x64\x00\x20\x00\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"���]���ʃ}�X�N�����p�̃s�N�Z���V�F�[�_�[�I�u�W�F�N�g No.%d �̍쐬�Ɏ��s���܂���\n" @*/, -( Result + 1 ) ) ) ;
			GSYS.HardInfo.UseShader = FALSE ;
			goto ERR ;
		}

		// �P���]���p�V�F�[�_�[�̍쐬
		Result = Graphics_D3D11_VertexShader_Create( ( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO * )&ShaderCode->Base.StretchRect_VS_Code, ( D_ID3D11VertexShader ** )&Shader->Base.StretchRect_VS, sizeof( ShaderCode->Base.StretchRect_VS_Code ) / sizeof( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO ) ) ;
		if( Result < 0 )
		{
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\x58\x53\x14\x7d\xe2\x8e\x01\x90\x28\x75\x6e\x30\x02\x98\xb9\x70\xb7\x30\xa7\x30\xfc\x30\xc0\x30\xfc\x30\xaa\x30\xd6\x30\xb8\x30\xa7\x30\xaf\x30\xc8\x30\x20\x00\x4e\x00\x6f\x00\x2e\x00\x25\x00\x64\x00\x20\x00\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�P���]���p�̒��_�V�F�[�_�[�I�u�W�F�N�g No.%d �̍쐬�Ɏ��s���܂���\n" @*/, -( Result + 1 ) ) ) ;
			GSYS.HardInfo.UseShader = FALSE ;
			goto ERR ;
		}
		Result = Graphics_D3D11_VertexShader_Create( ( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO * )&ShaderCode->Base.StretchRectTex8_VS_Code, ( D_ID3D11VertexShader ** )&Shader->Base.StretchRectTex8_VS, sizeof( ShaderCode->Base.StretchRectTex8_VS_Code ) / sizeof( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO ) ) ;
		if( Result < 0 )
		{
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\xa7\x5e\x19\x6a\x38\x00\x0b\x50\x6e\x30\x58\x53\x14\x7d\xe2\x8e\x01\x90\x28\x75\x6e\x30\x02\x98\xb9\x70\xb7\x30\xa7\x30\xfc\x30\xc0\x30\xfc\x30\xaa\x30\xd6\x30\xb8\x30\xa7\x30\xaf\x30\xc8\x30\x20\x00\x4e\x00\x6f\x00\x2e\x00\x25\x00\x64\x00\x20\x00\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�e�N�X�`�����W8�̒P���]���p�̒��_�V�F�[�_�[�I�u�W�F�N�g No.%d �̍쐬�Ɏ��s���܂���\n" @*/, -( Result + 1 ) ) ) ;
			GSYS.HardInfo.UseShader = FALSE ;
			goto ERR ;
		}
		Result = Graphics_D3D11_PixelShader_Create( ( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO * )&ShaderCode->Base.StretchRect_PS_Code, ( D_ID3D11PixelShader ** )&Shader->Base.StretchRect_PS, sizeof( ShaderCode->Base.StretchRect_PS_Code ) / sizeof( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO ) ) ;
		if( Result < 0 )
		{
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\x58\x53\x14\x7d\xe2\x8e\x01\x90\x28\x75\x6e\x30\xd4\x30\xaf\x30\xbb\x30\xeb\x30\xb7\x30\xa7\x30\xfc\x30\xc0\x30\xfc\x30\xaa\x30\xd6\x30\xb8\x30\xa7\x30\xaf\x30\xc8\x30\x20\x00\x4e\x00\x6f\x00\x2e\x00\x25\x00\x64\x00\x20\x00\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�P���]���p�̃s�N�Z���V�F�[�_�[�I�u�W�F�N�g No.%d �̍쐬�Ɏ��s���܂���\n" @*/, -( Result + 1 ) ) ) ;
			GSYS.HardInfo.UseShader = FALSE ;
			goto ERR ;
		}
	}

	DXST_ERRORLOG_ADDUTF16LE( "\x10\x62\x9f\x52\x0a\x00\x00"/*@ L"����\n" @*/ ) ;

	// ����I��
	return 0 ;

ERR :

	// �G���[�I��
	return -1 ;
}


// Direct3D11 �̃V�F�[�_�[�̌�n��������
extern int Graphics_D3D11_Shader_Terminate( void )
{
	GRAPHICS_HARDDATA_DIRECT3D11_DEVICE *Device = &GraphicsHardDataDirect3D11.Device ;
	GRAPHICS_HARDWARE_DIRECT3D11_SHADER *Shader = &Device->Shader ;

	if( GAPIWin.D3D11DeviceObject == NULL )
	{
		return -1 ;
	}

	Graphics_D3D11_DeviceState_ResetVertexShader() ;
	Graphics_D3D11_DeviceState_ResetPixelShader() ;

	Graphics_D3D11_VertexShaderArray_Release( ( D_ID3D11VertexShader ** )Shader->Base.BaseSimple_VS,               sizeof( Shader->Base.BaseSimple_VS               ) / sizeof( D_ID3D11VertexShader * ) ) ;
	Graphics_D3D11_PixelShaderArray_Release(  ( D_ID3D11PixelShader ** )Shader->Base.BaseNoneTex_PS,               sizeof( Shader->Base.BaseNoneTex_PS              ) / sizeof( D_ID3D11PixelShader  * ) ) ;
	Graphics_D3D11_PixelShaderArray_Release(  ( D_ID3D11PixelShader ** )Shader->Base.BaseUseTex_PS,                sizeof( Shader->Base.BaseUseTex_PS               ) / sizeof( D_ID3D11PixelShader  * ) ) ;
	Graphics_D3D11_PixelShaderArray_Release(  ( D_ID3D11PixelShader ** )&Shader->Base.MaskEffect_PS,               sizeof( Shader->Base.MaskEffect_PS               ) / sizeof( D_ID3D11PixelShader  * ) ) ;
	Graphics_D3D11_PixelShaderArray_Release(  ( D_ID3D11PixelShader ** )&Shader->Base.MaskEffect_ReverseEffect_PS, sizeof( Shader->Base.MaskEffect_ReverseEffect_PS ) / sizeof( D_ID3D11PixelShader  * ) ) ;
	Graphics_D3D11_PixelShaderArray_Release(  ( D_ID3D11PixelShader ** )Shader->Base.MaskEffect_UseGraphHandle_PS,               sizeof( Shader->Base.MaskEffect_UseGraphHandle_PS               ) / sizeof( D_ID3D11PixelShader  * ) ) ;
	Graphics_D3D11_PixelShaderArray_Release(  ( D_ID3D11PixelShader ** )Shader->Base.MaskEffect_UseGraphHandle_ReverseEffect_PS, sizeof( Shader->Base.MaskEffect_UseGraphHandle_ReverseEffect_PS ) / sizeof( D_ID3D11PixelShader  * ) ) ;
	Graphics_D3D11_VertexShaderArray_Release( ( D_ID3D11VertexShader ** )&Shader->Base.StretchRect_VS,             sizeof( Shader->Base.StretchRect_VS              ) / sizeof( D_ID3D11VertexShader * ) ) ;
	Graphics_D3D11_VertexShaderArray_Release( ( D_ID3D11VertexShader ** )&Shader->Base.StretchRectTex8_VS,         sizeof( Shader->Base.StretchRectTex8_VS          ) / sizeof( D_ID3D11VertexShader * ) ) ;
	Graphics_D3D11_PixelShaderArray_Release(  ( D_ID3D11PixelShader ** )&Shader->Base.StretchRect_PS,              sizeof( Shader->Base.StretchRect_PS              ) / sizeof( D_ID3D11PixelShader  * ) ) ;

	Graphics_D3D11_VertexShaderArray_Release( ( D_ID3D11VertexShader ** )&Shader->Base3D.Base3D_PixelLighting_VS,        sizeof( Shader->Base3D.Base3D_PixelLighting_VS         ) / sizeof( D_ID3D11VertexShader * ) ) ;
	Graphics_D3D11_PixelShaderArray_Release(  ( D_ID3D11PixelShader ** )&Shader->Base3D.Base3D_PixelLighting_Normal_PS,  sizeof( Shader->Base3D.Base3D_PixelLighting_Normal_PS  ) / sizeof( D_ID3D11PixelShader  * ) ) ;
	Graphics_D3D11_VertexShaderArray_Release( ( D_ID3D11VertexShader ** )&Shader->Base3D.Base3D_ShadowMap_VS,            sizeof( Shader->Base3D.Base3D_ShadowMap_VS             ) / sizeof( D_ID3D11VertexShader * ) ) ;
	Graphics_D3D11_VertexShaderArray_Release( ( D_ID3D11VertexShader ** )&Shader->Base3D.Base3D_NoLighting_VS,           sizeof( Shader->Base3D.Base3D_NoLighting_VS            ) / sizeof( D_ID3D11VertexShader * ) ) ;
	Graphics_D3D11_VertexShaderArray_Release( ( D_ID3D11VertexShader ** )&Shader->Base3D.Base3D_VertexLighting_VS,       sizeof( Shader->Base3D.Base3D_VertexLighting_VS        ) / sizeof( D_ID3D11VertexShader * ) ) ;
	Graphics_D3D11_PixelShaderArray_Release(  ( D_ID3D11PixelShader ** )&Shader->Base3D.Base3D_ShadowMap_Normal_PS,      sizeof( Shader->Base3D.Base3D_ShadowMap_Normal_PS      ) / sizeof( D_ID3D11PixelShader  * ) ) ;
	Graphics_D3D11_PixelShaderArray_Release(  ( D_ID3D11PixelShader ** )&Shader->Base3D.Base3D_NoLighting_Normal_PS,     sizeof( Shader->Base3D.Base3D_NoLighting_Normal_PS     ) / sizeof( D_ID3D11PixelShader  * ) ) ;
	Graphics_D3D11_PixelShaderArray_Release(  ( D_ID3D11PixelShader ** )&Shader->Base3D.Base3D_VertexLighting_Normal_PS, sizeof( Shader->Base3D.Base3D_VertexLighting_Normal_PS ) / sizeof( D_ID3D11PixelShader  * ) ) ;

#ifndef DX_NON_MODEL

	Graphics_D3D11_VertexShaderArray_Release( ( D_ID3D11VertexShader ** )&Shader->Model.MV1_PixelLighting_VS,           sizeof( Shader->Model.MV1_PixelLighting_VS            ) / sizeof( D_ID3D11VertexShader * ) ) ;
	Graphics_D3D11_PixelShaderArray_Release(  ( D_ID3D11PixelShader ** )&Shader->Model.MV1_PixelLighting_ToonType1_PS,  sizeof( Shader->Model.MV1_PixelLighting_ToonType1_PS  ) / sizeof( D_ID3D11PixelShader  * ) ) ;
	Graphics_D3D11_PixelShaderArray_Release(  ( D_ID3D11PixelShader ** )&Shader->Model.MV1_PixelLighting_ToonType2_PS,  sizeof( Shader->Model.MV1_PixelLighting_ToonType2_PS  ) / sizeof( D_ID3D11PixelShader  * ) ) ;
	Graphics_D3D11_PixelShaderArray_Release(  ( D_ID3D11PixelShader ** )&Shader->Model.MV1_PixelLighting_Normal_PS,     sizeof( Shader->Model.MV1_PixelLighting_Normal_PS     ) / sizeof( D_ID3D11PixelShader  * ) ) ;
	Graphics_D3D11_VertexShaderArray_Release( ( D_ID3D11VertexShader ** )&Shader->Model.MV1_ToonOutLine_ShadowMap_VS,   sizeof( Shader->Model.MV1_ToonOutLine_ShadowMap_VS    ) / sizeof( D_ID3D11VertexShader * ) ) ;
	Graphics_D3D11_VertexShaderArray_Release( ( D_ID3D11VertexShader ** )&Shader->Model.MV1_ToonOutLine_VS,             sizeof( Shader->Model.MV1_ToonOutLine_VS              ) / sizeof( D_ID3D11VertexShader * ) ) ;
	Graphics_D3D11_VertexShaderArray_Release( ( D_ID3D11VertexShader ** )&Shader->Model.MV1_ShadowMap_VS,               sizeof( Shader->Model.MV1_ShadowMap_VS                ) / sizeof( D_ID3D11VertexShader * ) ) ;
	Graphics_D3D11_VertexShaderArray_Release( ( D_ID3D11VertexShader ** )&Shader->Model.MV1_NoLighting_VS,              sizeof( Shader->Model.MV1_NoLighting_VS               ) / sizeof( D_ID3D11VertexShader * ) ) ;
	Graphics_D3D11_VertexShaderArray_Release( ( D_ID3D11VertexShader ** )&Shader->Model.MV1_VertexLighting_VS,          sizeof( Shader->Model.MV1_VertexLighting_VS           ) / sizeof( D_ID3D11VertexShader * ) ) ;
	Graphics_D3D11_PixelShaderArray_Release(  ( D_ID3D11PixelShader ** )&Shader->Model.MV1_ShadowMap_Toon_PS,           sizeof( Shader->Model.MV1_ShadowMap_Toon_PS           ) / sizeof( D_ID3D11PixelShader  * ) ) ;
	Graphics_D3D11_PixelShaderArray_Release(  ( D_ID3D11PixelShader ** )&Shader->Model.MV1_ShadowMap_Normal_PS,         sizeof( Shader->Model.MV1_ShadowMap_Normal_PS         ) / sizeof( D_ID3D11PixelShader  * ) ) ;
	Graphics_D3D11_PixelShaderArray_Release(  ( D_ID3D11PixelShader ** )&Shader->Model.MV1_NoLighting_Toon_PS,          sizeof( Shader->Model.MV1_NoLighting_Toon_PS          ) / sizeof( D_ID3D11PixelShader  * ) ) ;
	Graphics_D3D11_PixelShaderArray_Release(  ( D_ID3D11PixelShader ** )&Shader->Model.MV1_NoLighting_Normal_PS,        sizeof( Shader->Model.MV1_NoLighting_Normal_PS        ) / sizeof( D_ID3D11PixelShader  * ) ) ;
	Graphics_D3D11_PixelShaderArray_Release(  ( D_ID3D11PixelShader ** )&Shader->Model.MV1_VertexLighting_ToonType1_PS, sizeof( Shader->Model.MV1_VertexLighting_ToonType1_PS ) / sizeof( D_ID3D11PixelShader  * ) ) ;
	Graphics_D3D11_PixelShaderArray_Release(  ( D_ID3D11PixelShader ** )&Shader->Model.MV1_VertexLighting_ToonType2_PS, sizeof( Shader->Model.MV1_VertexLighting_ToonType2_PS ) / sizeof( D_ID3D11PixelShader  * ) ) ;
	Graphics_D3D11_PixelShaderArray_Release(  ( D_ID3D11PixelShader ** )&Shader->Model.MV1_VertexLighting_Normal_PS,    sizeof( Shader->Model.MV1_VertexLighting_Normal_PS    ) / sizeof( D_ID3D11PixelShader  * ) ) ;

#endif // DX_NON_MODEL

	// ����I��
	return 0 ;
}

// �R�c�W���`��̎w��̒��_�p�̕`��p�V�F�[�_�[���Z�b�g�A�b�v����
extern int Graphics_D3D11_Shader_Normal3DDraw_Setup( void )
{
	GRAPHICS_HARDWARE_DIRECT3D11_SHADER_BASE3D		*SB3D  = &GD3D11.Device.Shader.Base3D ;
	GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_BASE3D	*SCB3D = &GD3D11.ShaderCode.Base3D ;
	int												ValidPL ;
	D_ID3D11VertexShader							**VS_PL       = NULL ;
	GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO	*VSAddress_PL = NULL ;
	D_ID3D11PixelShader								**PS_PL       = NULL ;
	GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO	*PSAddress_PL = NULL ;
	D_ID3D11VertexShader							**VS          = NULL ;
	GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO	*VSAddress    = NULL ;
	D_ID3D11PixelShader								**PS          = NULL ;
	GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO	*PSAddress    = NULL ;
	int												ShadowMap ;
	int												FogType ;
	int												LightIndex84 ;
	int												LightIndex20 ;
	int												LightIndex10 ;
	int												AlphaTestMode ;
	int												BumpMap ;
	int												RenderBlendType ;
	int												NextBlendMode ;

	if( GD3D11.ShaderCode.Base3D.Base3DShaderInitializeFlag == FALSE &&
		Graphics_D3D11_ShaderCode_Base3D_Initialize() == FALSE )
	{
		return FALSE ;
	}

	// �u�����h���[�h�̌���
	{
		NextBlendMode = GD3D11.Device.DrawSetting.BlendMode ;
		switch( GD3D11.Device.DrawSetting.BlendMode )
		{
		case DX_BLENDMODE_SUB :
			// ���Z�u�����h�̏ꍇ��14�Ԗڂ̃u�����h���[�h���g�p����
			NextBlendMode = DX_BLENDMODE_SUB1 ;
			break ;

		case DX_BLENDMODE_NOBLEND :
			// �`���Ƀ��l������ꍇ�� DX_BLENDMODE_NOBLEND �ł��u�����h���[�h�� DX_BLENDMODE_SRCCOLOR �ɂ���
			if( GSYS.DrawSetting.AlphaChDrawMode )
			{
				NextBlendMode = DX_BLENDMODE_SRCCOLOR ;
			}

			// �`�挳�摜�Ƀ��`�����l��������ꍇ��u�����h�摜�̗L���ȂǏ�������� DX_BLENDMODE_ALPHA ���g�p����
			if( GD3D11.Device.DrawSetting.RenderTexture != NULL )
			{
				if( GD3D11.Device.DrawSetting.AlphaChannelValidFlag == TRUE )
				{
					NextBlendMode = DX_BLENDMODE_ALPHA ;
				}
			}
			break ;
		}
	}

	ShadowMap       = GSYS.DrawSetting.UseShadowMapNum != 0 ? 1 : 0 ;
	FogType         = GD3D11.Device.State.FogEnable ? GD3D11.Device.State.FogMode : DX_FOGMODE_NONE ;
	RenderBlendType = g_DefaultBlendDescArray[ NextBlendMode ].RenderBlendType ;
	AlphaTestMode   = GD3D11.Device.State.AlphaTestModeShaderIndex ;
	BumpMap         = 0 ;

	if( GSYS.DrawSetting.ShadowMapDraw )
	{
		// �V���h�E�}�b�v�ւ̕`��

		VS           =  &SB3D->Base3D_ShadowMap_VS     [ BumpMap ] ;
		VSAddress    = &SCB3D->Base3D_ShadowMap_VS_Code[ BumpMap ] ;

		PS           =  &SB3D->Base3D_ShadowMap_Normal_PS     [ AlphaTestMode ] ;
		PSAddress    = &SCB3D->Base3D_ShadowMap_Normal_PS_Code[ AlphaTestMode ] ;
	}
	else
	if( GD3D11.Device.State.Lighting )
	{
		// ���C�e�B���O����`��

		int LightMode[ DX_PIXELLIGHTING_LIGHT_NUM ] ;
		int i ;

		for( i = 0 ; i < DX_PIXELLIGHTING_LIGHT_NUM ; i ++ )
		{
			LightMode[ i ] = GD3D11.Device.State.LightEnableFlag[ i ] ? GD3D11.Device.State.LightParam[ i ].LightType : 0 ;
		}

		LightIndex84 = GD3D11.ShaderCode.Base.LightIndexList84[ LightMode[ 0 ] ][ LightMode[ 1 ] ][ LightMode[ 2 ] ][ LightMode[ 3 ] ][ LightMode[ 4 ] ][ LightMode[ 5 ] ] ;
		LightIndex20 = GD3D11.ShaderCode.Base.LightIndexList20[ LightMode[ 0 ] ][ LightMode[ 1 ] ][ LightMode[ 2 ] ] ;
		LightIndex10 = GD3D11.ShaderCode.Base.LightIndexList10
				[ LightMode[ 0 ] == DX_LIGHTTYPE_D3DLIGHT_DIRECTIONAL ? 2 : ( LightMode[ 0 ] ? 1 : 0 ) ]
				[ LightMode[ 1 ] == DX_LIGHTTYPE_D3DLIGHT_DIRECTIONAL ? 2 : ( LightMode[ 1 ] ? 1 : 0 ) ]
				[ LightMode[ 2 ] == DX_LIGHTTYPE_D3DLIGHT_DIRECTIONAL ? 2 : ( LightMode[ 2 ] ? 1 : 0 ) ] ;

		VS_PL        =  &SB3D->Base3D_PixelLighting_VS     [ ShadowMap ][ FogType ] ;
		VSAddress_PL = &SCB3D->Base3D_PixelLighting_VS_Code[ ShadowMap ][ FogType ] ;

		PS_PL        =  &SB3D->Base3D_PixelLighting_Normal_PS     [ ShadowMap ][ LightIndex84 ][ RenderBlendType ][ AlphaTestMode ] ;
		PSAddress_PL = &SCB3D->Base3D_PixelLighting_Normal_PS_Code[ ShadowMap ][ LightIndex84 ][ RenderBlendType ][ AlphaTestMode ] ;

		VS           =  &SB3D->Base3D_VertexLighting_VS     [ ShadowMap ][ FogType ][ LightIndex20 ] ;
		VSAddress    = &SCB3D->Base3D_VertexLighting_VS_Code[ ShadowMap ][ FogType ][ LightIndex20 ] ;

		PS           =  &SB3D->Base3D_VertexLighting_Normal_PS     [ ShadowMap ][ LightIndex10 ][ RenderBlendType ][ AlphaTestMode ] ;
		PSAddress    = &SCB3D->Base3D_VertexLighting_Normal_PS_Code[ ShadowMap ][ LightIndex10 ][ RenderBlendType ][ AlphaTestMode ] ;
	}
	else
	{
		// ���C�e�B���O�Ȃ��`��

		VS           =  &SB3D->Base3D_NoLighting_VS     [ FogType ] ;
		VSAddress    = &SCB3D->Base3D_NoLighting_VS_Code[ FogType ] ;

		PS           =  &SB3D->Base3D_NoLighting_Normal_PS     [ RenderBlendType ][ AlphaTestMode ] ;
		PSAddress    = &SCB3D->Base3D_NoLighting_Normal_PS_Code[ RenderBlendType ][ AlphaTestMode ] ;
	}

	ValidPL = 
		VSAddress_PL != NULL && VSAddress_PL->Binary != NULL &&
		PSAddress_PL != NULL && PSAddress_PL->Binary != NULL ;

	if( ( GSYS.Light.EnableNum > DX_VERTEXLIGHTING_LIGHT_NUM || GD3D11.UsePixelLightingShader ) && ValidPL )
	{
		// �V�F�[�_�[�����邩�ǂ����𒲂ׂ�
		if( *VS_PL == NULL )
		{
			// �V�F�[�_�[�̍쐬�����݂�
			if( Graphics_D3D11_VertexShader_Create( VSAddress_PL, VS_PL, 1 ) != 0 )
			{
				DXST_ERRORLOGFMT_ADDUTF16LE(( "\x47\x00\x72\x00\x61\x00\x70\x00\x68\x00\x69\x00\x63\x00\x73\x00\x5f\x00\x44\x00\x33\x00\x44\x00\x31\x00\x31\x00\x5f\x00\x53\x00\x68\x00\x61\x00\x64\x00\x65\x00\x72\x00\x5f\x00\x4e\x00\x6f\x00\x72\x00\x6d\x00\x61\x00\x6c\x00\x33\x00\x44\x00\x44\x00\x72\x00\x61\x00\x77\x00\x5f\x00\x53\x00\x65\x00\x74\x00\x75\x00\x70\x00\x20\x00\x67\x30\x02\x98\xb9\x70\xb7\x30\xa7\x30\xfc\x30\xc0\x30\xfc\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Graphics_D3D11_Shader_Normal3DDraw_Setup �Œ��_�V�F�[�_�[�̍쐬�Ɏ��s���܂���\n" @*/ )) ;
				return FALSE ;
			}
		}

		// �o�[�e�b�N�X�V�F�[�_�[�̃Z�b�g�A�b�v
		Graphics_D3D11_DeviceState_SetVertexShader( *VS_PL, FALSE ) ;


		// �V�F�[�_�[�����邩�ǂ����𒲂ׂ�
		if( *PS_PL == NULL )
		{
			// �V�F�[�_�[�̍쐬�����݂�
			if( Graphics_D3D11_PixelShader_Create( PSAddress_PL, PS_PL, 1 ) != 0 )
			{
				DXST_ERRORLOGFMT_ADDUTF16LE(( "\x47\x00\x72\x00\x61\x00\x70\x00\x68\x00\x69\x00\x63\x00\x73\x00\x5f\x00\x44\x00\x33\x00\x44\x00\x31\x00\x31\x00\x5f\x00\x53\x00\x68\x00\x61\x00\x64\x00\x65\x00\x72\x00\x5f\x00\x4e\x00\x6f\x00\x72\x00\x6d\x00\x61\x00\x6c\x00\x33\x00\x44\x00\x44\x00\x72\x00\x61\x00\x77\x00\x5f\x00\x53\x00\x65\x00\x74\x00\x75\x00\x70\x00\x20\x00\x67\x30\xd4\x30\xaf\x30\xbb\x30\xeb\x30\xb7\x30\xa7\x30\xfc\x30\xc0\x30\xfc\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Graphics_D3D11_Shader_Normal3DDraw_Setup �Ńs�N�Z���V�F�[�_�[�̍쐬�Ɏ��s���܂���\n" @*/ )) ;
				return FALSE ;
			}
		}

		// �s�N�Z���V�F�[�_�[�̃Z�b�g�A�b�v
		Graphics_D3D11_DeviceState_SetPixelShader( *PS_PL, FALSE ) ;
	}
	else
	{
		// �V�F�[�_�[�����邩�ǂ����𒲂ׂ�
		if( *VS == NULL )
		{
			// �V�F�[�_�[�̍쐬�����݂�
			if( Graphics_D3D11_VertexShader_Create( VSAddress, VS, 1 ) != 0 )
			{
				DXST_ERRORLOGFMT_ADDUTF16LE(( "\x47\x00\x72\x00\x61\x00\x70\x00\x68\x00\x69\x00\x63\x00\x73\x00\x5f\x00\x44\x00\x33\x00\x44\x00\x31\x00\x31\x00\x5f\x00\x53\x00\x68\x00\x61\x00\x64\x00\x65\x00\x72\x00\x5f\x00\x4e\x00\x6f\x00\x72\x00\x6d\x00\x61\x00\x6c\x00\x33\x00\x44\x00\x44\x00\x72\x00\x61\x00\x77\x00\x5f\x00\x53\x00\x65\x00\x74\x00\x75\x00\x70\x00\x20\x00\x67\x30\x02\x98\xb9\x70\xb7\x30\xa7\x30\xfc\x30\xc0\x30\xfc\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Graphics_D3D11_Shader_Normal3DDraw_Setup �Œ��_�V�F�[�_�[�̍쐬�Ɏ��s���܂���\n" @*/ )) ;
				return FALSE ;
			}
		}

		// �o�[�e�b�N�X�V�F�[�_�[�̃Z�b�g�A�b�v
		Graphics_D3D11_DeviceState_SetVertexShader( *VS, FALSE ) ;


		// �V�F�[�_�[�����邩�ǂ����𒲂ׂ�
		if( *PS == NULL )
		{
			// �V�F�[�_�[�̍쐬�����݂�
			if( Graphics_D3D11_PixelShader_Create( PSAddress, PS, 1 ) != 0 )
			{
				DXST_ERRORLOGFMT_ADDUTF16LE(( "\x47\x00\x72\x00\x61\x00\x70\x00\x68\x00\x69\x00\x63\x00\x73\x00\x5f\x00\x44\x00\x33\x00\x44\x00\x31\x00\x31\x00\x5f\x00\x53\x00\x68\x00\x61\x00\x64\x00\x65\x00\x72\x00\x5f\x00\x4e\x00\x6f\x00\x72\x00\x6d\x00\x61\x00\x6c\x00\x33\x00\x44\x00\x44\x00\x72\x00\x61\x00\x77\x00\x5f\x00\x53\x00\x65\x00\x74\x00\x75\x00\x70\x00\x20\x00\x67\x30\xd4\x30\xaf\x30\xbb\x30\xeb\x30\xb7\x30\xa7\x30\xfc\x30\xc0\x30\xfc\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Graphics_D3D11_Shader_Normal3DDraw_Setup �Ńs�N�Z���V�F�[�_�[�̍쐬�Ɏ��s���܂���\n" @*/ )) ;
				return FALSE ;
			}
		}

		// �s�N�Z���V�F�[�_�[�̃Z�b�g�A�b�v
		Graphics_D3D11_DeviceState_SetPixelShader( *PS ) ;
	}


	// �I��
	return TRUE ;
}

// �A���t�@�e�X�g�̔�r���[�h����s�N�Z���V�F�[�_�[�̃A���t�@�e�X�g���[�h( DIRECT3D11_PS_ALPHATEST_CMP_GREATER �� )���擾����
extern int Graphics_D3D11_Shader_GetAlphaTestModeIndex( int AlphaTestEnable, int AlphaTestMode /* DX_CMP_NEVER �� */ )
{
	if( AlphaTestEnable == FALSE )
	{
		return DIRECT3D11_PS_ALPHATEST_CMP_GREATER ;
	}

	switch( AlphaTestMode )
	{
	default:
		return DIRECT3D11_PS_ALPHATEST_CMP_OTHER ;

	case DX_CMP_GREATER :		// DrawAlpha >  TestParam
		return DIRECT3D11_PS_ALPHATEST_CMP_GREATER ;
	}
}

























// ���_�o�b�t�@�E�C���f�b�N�X�o�b�t�@�֌W

// ���_�o�b�t�@�n���h���p�� ID3D11Buffer �I�u�W�F�N�g���쐬����
extern	int Graphics_D3D11_VertexBuffer_CreateObject( VERTEXBUFFERHANDLEDATA *VertexBuffer, int Restore )
{
	D_D3D11_BUFFER_DESC BufferDesc ;
	HRESULT             hr ;

	_MEMSET( &BufferDesc, 0, sizeof( BufferDesc ) ) ;
	BufferDesc.ByteWidth      = ( UINT )( VertexBuffer->UnitSize * VertexBuffer->Num ) ;
	BufferDesc.Usage          = D_D3D11_USAGE_DEFAULT ;
	BufferDesc.BindFlags      = D_D3D11_BIND_VERTEX_BUFFER ;
	BufferDesc.CPUAccessFlags = 0 ;
	hr = D3D11Device_CreateBuffer( &BufferDesc, NULL, &VertexBuffer->PF->D3D11.VertexBuffer ) ;
	if( FAILED( hr ) )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\x02\x98\xb9\x70\xd0\x30\xc3\x30\xd5\x30\xa1\x30\xcf\x30\xf3\x30\xc9\x30\xeb\x30\x28\x75\x6e\x30\x02\x98\xb9\x70\xd0\x30\xc3\x30\xd5\x30\xa1\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"���_�o�b�t�@�n���h���p�̒��_�o�b�t�@�̍쐬�Ɏ��s���܂���\n" @*/ )) ;
		return -1 ;
	}

	// �f�[�^�𕜋A����
	if( Restore )
	{
		NS_SetVertexBufferData( 0, VertexBuffer->Buffer, VertexBuffer->Num, VertexBuffer->HandleInfo.Handle ) ; 
	}

	// ����I��
	return 0 ;
}

// ���_�o�b�t�@�n���h���p�� ID3D11Buffer �I�u�W�F�N�g���������
extern	int	Graphics_D3D11_VertexBuffer_ReleaseObject( VERTEXBUFFERHANDLEDATA *VertexBuffer )
{
	// ���_�o�b�t�@�̉��
	if( VertexBuffer->PF->D3D11.VertexBuffer )
	{
		Direct3D11_Release_Buffer( VertexBuffer->PF->D3D11.VertexBuffer ) ;
		VertexBuffer->PF->D3D11.VertexBuffer = NULL ;
	}

	// ����I��
	return 0 ;
}

// �C���f�b�N�X�o�b�t�@�n���h���p�� ID3D11Buffer �I�u�W�F�N�g���쐬����
extern	int	Graphics_D3D11_IndexBuffer_CreateObject( INDEXBUFFERHANDLEDATA *IndexBuffer, int Restore )
{
	D_D3D11_BUFFER_DESC BufferDesc ;
	HRESULT             hr ;

	_MEMSET( &BufferDesc, 0, sizeof( BufferDesc ) ) ;
	BufferDesc.ByteWidth      = ( UINT )( IndexBuffer->UnitSize * IndexBuffer->Num ) ;
	BufferDesc.Usage          = D_D3D11_USAGE_DEFAULT ;
	BufferDesc.BindFlags      = D_D3D11_BIND_INDEX_BUFFER ;
	BufferDesc.CPUAccessFlags = 0 ;
	hr = D3D11Device_CreateBuffer( &BufferDesc, NULL, &IndexBuffer->PF->D3D11.IndexBuffer ) ;
	if( FAILED( hr ) )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\xa4\x30\xf3\x30\xc7\x30\xc3\x30\xaf\x30\xb9\x30\xd0\x30\xc3\x30\xd5\x30\xa1\x30\xcf\x30\xf3\x30\xc9\x30\xeb\x30\x28\x75\x6e\x30\xa4\x30\xf3\x30\xc7\x30\xc3\x30\xaf\x30\xb9\x30\xd0\x30\xc3\x30\xd5\x30\xa1\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�C���f�b�N�X�o�b�t�@�n���h���p�̃C���f�b�N�X�o�b�t�@�̍쐬�Ɏ��s���܂���\n" @*/ )) ;
		return -1 ;
	}

	// �f�[�^�𕜋A����
	if( Restore )
	{
		NS_SetIndexBufferData( 0, IndexBuffer->Buffer, IndexBuffer->Num, IndexBuffer->HandleInfo.Handle ) ; 
	}

	// ����I��
	return 0 ;
}

// �C���f�b�N�X�o�b�t�@�n���h���p�� ID3D11Buffer �I�u�W�F�N�g���������
extern	int	Graphics_D3D11_IndexBuffer_ReleaseObject( INDEXBUFFERHANDLEDATA *IndexBuffer )
{
	// �C���f�b�N�X�o�b�t�@�̉��
	if( IndexBuffer->PF->D3D11.IndexBuffer )
	{
		Direct3D11_Release_Buffer( IndexBuffer->PF->D3D11.IndexBuffer ) ;
		IndexBuffer->PF->D3D11.IndexBuffer = NULL ;
	}

	// ����I��
	return 0 ;
}





























// Direct3D11 �̉�ʊ֌W

// ScreenCopy �� GetDrawScreen ���������邽�߂Ɏg�p����e���|�����o�b�t�@�̍쐬( 0:����  -1:���s )
extern	int		Graphics_D3D11_SetupSubBackBuffer( void )
{
	HRESULT                           hr ;
	D_D3D11_TEXTURE2D_DESC            Texture2DDesc ;
	D_D3D11_RENDER_TARGET_VIEW_DESC   RTVDesc ;
	D_D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc ;
	float                             ColorRGBA[ 4 ] ;
	RECT                              BltRect ;

	// ���ɃT�u�o�b�N�o�b�t�@���쐬����Ă���ꍇ�͉������Ȃ�
	if( GD3D11.Device.Screen.SubBackBufferTexture2D != NULL ) return 0 ;

	// �f�o�C�X�������̏ꍇ�͉��������I��
	if( GAPIWin.D3D11DeviceObject == NULL )
	{
		return -1 ;
	}

	// �`�����f���o���Ă���
	DRAWSTOCKINFO

	// �T�u�o�b�N�o�b�t�@�̃T�C�Y���Z�b�g
	GD3D11.Device.Screen.SubBackBufferTextureSizeX = GSYS.Screen.MainScreenSizeX ;
	GD3D11.Device.Screen.SubBackBufferTextureSizeY = GSYS.Screen.MainScreenSizeY ;

	// �T�u�o�b�N�o�b�t�@�p�e�N�X�`���̍쐬
	_MEMSET( &Texture2DDesc, 0, sizeof( Texture2DDesc ) );
	Texture2DDesc.Usage              = D_D3D11_USAGE_DEFAULT ;
	Texture2DDesc.Format             = DIRECT3D11_BACKBUFFER_FORMAT ;
	Texture2DDesc.BindFlags          = D_D3D11_BIND_RENDER_TARGET | D_D3D11_BIND_SHADER_RESOURCE ;
	Texture2DDesc.Width              = ( UINT )GD3D11.Device.Screen.SubBackBufferTextureSizeX ;
	Texture2DDesc.Height             = ( UINT )GD3D11.Device.Screen.SubBackBufferTextureSizeY ;
	Texture2DDesc.CPUAccessFlags     = 0 ;
	Texture2DDesc.MipLevels          = 1 ;
	Texture2DDesc.ArraySize          = 1 ;
	if( GSYS.Setting.FSAAMultiSampleCount > 1 )
	{
		// D_D3D_FEATURE_LEVEL_10_0 �����̏ꍇ�̓}���`�T���v���͎g�p�ł��Ȃ�
		if( GD3D11.Setting.FeatureLevel < D_D3D_FEATURE_LEVEL_10_0 )
		{
			Texture2DDesc.SampleDesc.Count		= 1 ;
			Texture2DDesc.SampleDesc.Quality	= 0 ;
		}
		else
		{
			Texture2DDesc.SampleDesc.Count		= ( UINT )GSYS.Setting.FSAAMultiSampleCount ;
			Texture2DDesc.SampleDesc.Quality	= ( UINT )GSYS.Setting.FSAAMultiSampleQuality ;
			D3D11Device_CheckMultiSampleParam( Texture2DDesc.Format, &Texture2DDesc.SampleDesc.Count, &Texture2DDesc.SampleDesc.Quality, FALSE ) ;
		}
	}
	else
	{
		Texture2DDesc.SampleDesc.Count		= 1 ;
		Texture2DDesc.SampleDesc.Quality	= 0 ;
	}
	hr = D3D11Device_CreateTexture2D( &Texture2DDesc, NULL, &GD3D11.Device.Screen.SubBackBufferTexture2D ) ;
	if( FAILED( hr ) )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\xb5\x30\xd6\x30\xd0\x30\xc3\x30\xaf\x30\xd0\x30\xc3\x30\xd5\x30\xa1\x30\x28\x75\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x20\x00\x20\x00\x46\x00\x6f\x00\x72\x00\x6d\x00\x61\x00\x74\x00\x3a\x00\x25\x00\x64\x00\x20\x00\x20\x00\x53\x00\x69\x00\x7a\x00\x65\x00\x58\x00\x3a\x00\x25\x00\x64\x00\x20\x00\x20\x00\x53\x00\x69\x00\x7a\x00\x65\x00\x59\x00\x3a\x00\x25\x00\x64\x00\x20\x00\x0a\x00\x00"/*@ L"�T�u�o�b�N�o�b�t�@�p�e�N�X�`���̍쐬�Ɏ��s���܂���  Format:%d  SizeX:%d  SizeY:%d \n" @*/,
			Texture2DDesc.Format, GD3D11.Device.Screen.SubBackBufferTextureSizeX, GD3D11.Device.Screen.SubBackBufferTextureSizeY )) ;
		return -1 ;
	}

	// �T�u�o�b�N�o�b�t�@�p�����_�[�^�[�Q�b�g�r���[���쐬
	_MEMSET( &RTVDesc, 0, sizeof( RTVDesc ) );
	RTVDesc.Format            = Texture2DDesc.Format ;
	if( Texture2DDesc.SampleDesc.Count > 1 )
	{
		RTVDesc.ViewDimension = D_D3D11_RTV_DIMENSION_TEXTURE2DMS ;
	}
	else
	{
		RTVDesc.ViewDimension = D_D3D11_RTV_DIMENSION_TEXTURE2D ;
	}
	hr = D3D11Device_CreateRenderTargetView( GD3D11.Device.Screen.SubBackBufferTexture2D, &RTVDesc, &GD3D11.Device.Screen.SubBackBufferRTV ) ;
	if( FAILED( hr ) )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\xb5\x30\xd6\x30\xd0\x30\xc3\x30\xaf\x30\xd0\x30\xc3\x30\xd5\x30\xa1\x30\x28\x75\xec\x30\xf3\x30\xc0\x30\xfc\x30\xbf\x30\xfc\x30\xb2\x30\xc3\x30\xc8\x30\xd3\x30\xe5\x30\xfc\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x20\x00\x20\x00\x46\x00\x6f\x00\x72\x00\x6d\x00\x61\x00\x74\x00\x3a\x00\x25\x00\x64\x00\x20\x00\x20\x00\x53\x00\x69\x00\x7a\x00\x65\x00\x58\x00\x3a\x00\x25\x00\x64\x00\x20\x00\x20\x00\x53\x00\x69\x00\x7a\x00\x65\x00\x59\x00\x3a\x00\x25\x00\x64\x00\x20\x00\x0a\x00\x00"/*@ L"�T�u�o�b�N�o�b�t�@�p�����_�[�^�[�Q�b�g�r���[�̍쐬�Ɏ��s���܂���  Format:%d  SizeX:%d  SizeY:%d \n" @*/,
			Texture2DDesc.Format, GD3D11.Device.Screen.SubBackBufferTextureSizeX, GD3D11.Device.Screen.SubBackBufferTextureSizeY )) ;
		return -1 ;
	}

	// �T�u�o�b�N�o�b�t�@�p�̃V�F�[�_���\�[�X�r���[���쐬
	_MEMSET( &SRVDesc, 0, sizeof( SRVDesc ) );
	SRVDesc.Format            = Texture2DDesc.Format ;
	if( Texture2DDesc.SampleDesc.Count > 1 )
	{
		SRVDesc.ViewDimension = D_D3D11_SRV_DIMENSION_TEXTURE2DMS ;
	}
	else
	{
		SRVDesc.ViewDimension = D_D3D11_SRV_DIMENSION_TEXTURE2D ;
	}
	SRVDesc.Texture2D.MipLevels = 1 ;
	hr = D3D11Device_CreateShaderResourceView( GD3D11.Device.Screen.SubBackBufferTexture2D, &SRVDesc, &GD3D11.Device.Screen.SubBackBufferSRV ) ;
	if( FAILED( hr ) )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\xb5\x30\xd6\x30\xd0\x30\xc3\x30\xaf\x30\xd0\x30\xc3\x30\xd5\x30\xa1\x30\x28\x75\xb7\x30\xa7\x30\xfc\x30\xc0\x30\xfc\x30\xea\x30\xbd\x30\xfc\x30\xb9\x30\xd3\x30\xe5\x30\xfc\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x20\x00\x20\x00\x46\x00\x6f\x00\x72\x00\x6d\x00\x61\x00\x74\x00\x3a\x00\x25\x00\x64\x00\x20\x00\x20\x00\x53\x00\x69\x00\x7a\x00\x65\x00\x58\x00\x3a\x00\x25\x00\x64\x00\x20\x00\x20\x00\x53\x00\x69\x00\x7a\x00\x65\x00\x59\x00\x3a\x00\x25\x00\x64\x00\x20\x00\x0a\x00\x00"/*@ L"�T�u�o�b�N�o�b�t�@�p�V�F�[�_�[���\�[�X�r���[�̍쐬�Ɏ��s���܂���  Format:%d  SizeX:%d  SizeY:%d \n" @*/,
			Texture2DDesc.Format, GD3D11.Device.Screen.SubBackBufferTextureSizeX, GD3D11.Device.Screen.SubBackBufferTextureSizeY )) ;
		return -1 ;
	}

	// �{�o�b�N�o�b�t�@�̓��e��]��
	BltRect.left   = 0 ;
	BltRect.top    = 0 ;
	BltRect.right  = GD3D11.Device.Screen.SubBackBufferTextureSizeX ;
	BltRect.bottom = GD3D11.Device.Screen.SubBackBufferTextureSizeY ;
	if( BltRect.right > ( int )GD3D11.Device.Screen.OutputWindowInfo[ 0 ].BufferTexture2DDesc.Width )
	{
		BltRect.right = ( LONG )GD3D11.Device.Screen.OutputWindowInfo[ 0 ].BufferTexture2DDesc.Width ;
	}
	if( BltRect.bottom > ( int )GD3D11.Device.Screen.OutputWindowInfo[ 0 ].BufferTexture2DDesc.Height )
	{
		BltRect.bottom = ( LONG )GD3D11.Device.Screen.OutputWindowInfo[ 0 ].BufferTexture2DDesc.Height ;
	}
	Graphics_D3D11_StretchRect(
		GD3D11.Device.Screen.OutputWindowInfo[ 0 ].BufferTexture2D,
		GD3D11.Device.Screen.OutputWindowInfo[ 0 ].BufferSRV,
		&BltRect,
		GD3D11.Device.Screen.SubBackBufferTexture2D,
		GD3D11.Device.Screen.SubBackBufferRTV,
		&BltRect
	) ;

	// �{�o�b�N�o�b�t�@�̓��e��������
	ColorRGBA[ 0 ] = 0.0f ;
	ColorRGBA[ 1 ] = 0.0f ;
	ColorRGBA[ 2 ] = 0.0f ;
	ColorRGBA[ 3 ] = 0.0f ;
	D3D11DeviceContext_ClearRenderTargetView( GD3D11.Device.Screen.OutputWindowInfo[ 0 ].BufferRTV, ColorRGBA ) ;

	// �����_�����O�^�[�Q�b�g�̕ύX
	GD3D11.Device.DrawSetting.CancelSettingEqualCheck = TRUE ;
	Graphics_D3D11_DeviceState_SetRenderTarget( GD3D11.Device.Screen.SubBackBufferTexture2D, GD3D11.Device.Screen.SubBackBufferRTV ) ;
	GD3D11.Device.DrawSetting.CancelSettingEqualCheck = FALSE ;

	// �I��
	return 0 ;
}

// �o�͐�E�C���h�E�̒ǉ����s��( -1:���s  -1�ȊO:�ΏۃE�C���h�E�p���̔z��C���f�b�N�X )
extern	int		Graphics_D3D11_OutputWindow_Add( HWND TargetWindow, int IsMainTarget )
{
	int                    i ;
	int                    j ;
	int                    NewIndex ;
	HRESULT                Result ;
	D_DXGI_SWAP_CHAIN_DESC SwapChainDesc ;
	GRAPHICS_HARDDATA_DIRECT3D11_OUTPUTWINDOWINFO *OWI ;

	// ���Ɏw��̃E�C���h�E���Ώۂɓ����Ă��邩���ׂ�
	OWI = GD3D11.Device.Screen.OutputWindowInfo ;
	j = 0 ;
	NewIndex = -1 ;
	for( i = 0 ; i < DX_D3D11_MAX_OUTPUTWINDOW && j < GD3D11.Device.Screen.OutputWindowNum ; i ++, OWI ++ )
	{
		if( OWI->UseFlag == FALSE )
		{
			NewIndex = i ;
			continue ;
		}

		j ++ ;

		if( OWI->DXGISwapChainDesc.OutputWindow == TargetWindow )
		{
			// ���ɓ����Ă�����z��̃C���f�b�N�X��Ԃ�
			return i ;
		}
	}

	// ���ɏo�͐�E�C���h�E�̐����ő�ɒB���Ă�����G���[
	if( GD3D11.Device.Screen.OutputWindowNum >= DX_D3D11_MAX_OUTPUTWINDOW )
	{
		return -1 ;
	}

	// �����Ă��Ȃ��ꍇ�͐V�K�ɍ쐬

	// �g�p����Ă��Ȃ��z��v�f����������
	if( NewIndex == -1 )
	{
		for( NewIndex = 0; GD3D11.Device.Screen.OutputWindowInfo[ NewIndex ].UseFlag; NewIndex++ ){}
	}
	OWI = &GD3D11.Device.Screen.OutputWindowInfo[ NewIndex ] ;

	// �X���b�v�`�F�C���̍쐬
	{
/*		UINT FormatSupport ;
		UINT SupportRenderTarget ;
		UINT DepthStencil ;
		Result = GAPIWin.D3D11DeviceObject->CheckFormatSupport( 
//			D_DXGI_FORMAT_D16_UNORM,
//			D_DXGI_FORMAT_B8G8R8X8_UNORM,
			D_DXGI_FORMAT_R24G8_TYPELESS,
			&FormatSupport ) ;
		SupportRenderTarget = FormatSupport & D_D3D11_FORMAT_SUPPORT_RENDER_TARGET ;
		DepthStencil        = FormatSupport & D_D3D11_FORMAT_SUPPORT_DEPTH_STENCIL ;
*/

		_MEMSET( &SwapChainDesc, 0, sizeof( SwapChainDesc ) ) ;
		SwapChainDesc.BufferDesc.RefreshRate.Numerator		= ( UINT )( GSYS.Screen.MainScreenRefreshRate == 0 ? 60 : GSYS.Screen.MainScreenRefreshRate ) ;
		SwapChainDesc.BufferDesc.RefreshRate.Denominator	= 1 ;
		SwapChainDesc.BufferDesc.Format						= DIRECT3D11_BACKBUFFER_FORMAT /* GSYS.Screen.MainScreenColorBitDepth == 16 ? D_DXGI_FORMAT_B5G5R5A1_UNORM : D_DXGI_FORMAT_B8G8R8A8_UNORM */ ;
		SwapChainDesc.BufferDesc.ScanlineOrdering			= D_DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED ;
		SwapChainDesc.BufferDesc.Scaling					= D_DXGI_MODE_SCALING_UNSPECIFIED ;
		SwapChainDesc.BufferCount							= 2 ;
		SwapChainDesc.BufferUsage							= D_DXGI_USAGE_RENDER_TARGET_OUTPUT | D_DXGI_USAGE_SHADER_INPUT ;
		SwapChainDesc.OutputWindow							= TargetWindow ;
		SwapChainDesc.SwapEffect							= D_DXGI_SWAP_EFFECT_DISCARD ;
		SwapChainDesc.Flags									= D_DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH ;
		if( IsMainTarget )
		{
			if( NS_GetWindowModeFlag() == TRUE )
			{
				SwapChainDesc.BufferDesc.Width				= ( UINT )GSYS.Screen.MainScreenSizeX ;
				SwapChainDesc.BufferDesc.Height				= ( UINT )GSYS.Screen.MainScreenSizeY ;
				SwapChainDesc.Windowed						= TRUE ;

				DXST_ERRORLOGFMT_ADDUTF16LE(( "\x5b\x00\xa6\x30\xa4\x30\xf3\x30\xc9\x30\xa6\x30\xe2\x30\xfc\x30\xc9\x30\x20\x00\x25\x00\x64\x00\x78\x00\x25\x00\x64\x00\x5d\x00\x00"/*@ L"[�E�C���h�E���[�h %dx%d]" @*/, SwapChainDesc.BufferDesc.Width, SwapChainDesc.BufferDesc.Height )) ;
			}
			else
			{
				// �t���X�N���[�����[�h�̏����Z�b�g�A�b�v
				Graphics_Screen_SetupFullScreenModeInfo() ;

				SwapChainDesc.BufferDesc.Width				= ( UINT )GSYS.Screen.FullScreenUseDispModeData.Width ;
				SwapChainDesc.BufferDesc.Height				= ( UINT )GSYS.Screen.FullScreenUseDispModeData.Height ;
				SwapChainDesc.Windowed						= FALSE ;

				// �t���X�N���[�����[�h�t���O�𗧂Ă�
				GD3D11.Device.Screen.FullscreenSatte        = TRUE ;

				DXST_ERRORLOGFMT_ADDUTF16LE(( "\x5b\x00\xd5\x30\xeb\x30\xb9\x30\xaf\x30\xea\x30\xfc\x30\xf3\x30\xe2\x30\xfc\x30\xc9\x30\x20\x00\x25\x00\x64\x00\x78\x00\x25\x00\x64\x00\x5d\x00\x00"/*@ L"[�t���X�N���[�����[�h %dx%d]" @*/, SwapChainDesc.BufferDesc.Width, SwapChainDesc.BufferDesc.Height )) ;
			}
		}
		else
		{
			RECT ClientRect ;

			GetClientRect( TargetWindow, &ClientRect ) ;

			SwapChainDesc.BufferDesc.Width					= ( UINT )( ClientRect.right  - ClientRect.left ) ;
			SwapChainDesc.BufferDesc.Height					= ( UINT )( ClientRect.bottom - ClientRect.top  ) ;
			SwapChainDesc.Windowed							= TRUE ;

			DXST_ERRORLOG_ADDUTF16LE( "\x5b\x00\xa6\x30\xa4\x30\xf3\x30\xc9\x30\xa6\x30\xe2\x30\xfc\x30\xc9\x30\x5d\x00\x20\x00\x00"/*@ L"[�E�C���h�E���[�h] " @*/ ) ;
		}

		// FSAA�̐ݒ�l�𒲂ׂ�
		if( GSYS.Setting.FSAAMultiSampleCount > 1 )
		{
			// D_D3D_FEATURE_LEVEL_10_0 �����̏ꍇ�̓}���`�T���v���͎g�p�ł��Ȃ�
			if( GD3D11.Setting.FeatureLevel < D_D3D_FEATURE_LEVEL_10_0 )
			{
				SwapChainDesc.SampleDesc.Count		= 1 ;
				SwapChainDesc.SampleDesc.Quality	= 0 ;
			}
			else
			{
				SwapChainDesc.SampleDesc.Count		  = ( UINT )GSYS.Setting.FSAAMultiSampleCount ;
				SwapChainDesc.SampleDesc.Quality	  = ( UINT )GSYS.Setting.FSAAMultiSampleQuality ;
				D3D11Device_CheckMultiSampleParam( SwapChainDesc.BufferDesc.Format, &SwapChainDesc.SampleDesc.Count, &SwapChainDesc.SampleDesc.Quality, FALSE ) ;
			}
		}
		else
		{
			SwapChainDesc.SampleDesc.Count		= 1 ;
			SwapChainDesc.SampleDesc.Quality	= 0 ;
		}
		GSYS.Setting.FSAAMultiSampleCount   = ( int )SwapChainDesc.SampleDesc.Count ;
		GSYS.Setting.FSAAMultiSampleQuality = ( int )SwapChainDesc.SampleDesc.Quality ;

		DXST_ERRORLOG_ADDUTF16LE( "\x49\x00\x44\x00\x58\x00\x47\x00\x49\x00\x53\x00\x77\x00\x61\x00\x70\x00\x43\x00\x68\x00\x61\x00\x69\x00\x6e\x00\x20\x00\x92\x30\x5c\x4f\x10\x62\x57\x30\x7e\x30\x59\x30\x2e\x00\x2e\x00\x2e\x00\x2e\x00\x20\x00\x00"/*@ L"IDXGISwapChain ���쐬���܂�.... " @*/ ) ;

		// �X���b�v�`�F�C���̍쐬
		Result = DXGIFactory_CreateSwapChain( &SwapChainDesc, &OWI->DXGISwapChain ) ;
//		DXST_ERRORLOGFMT_ADDUTF16LE(( L"CreateSwapChain Result:%d", Result )) ;
		if( OWI->DXGISwapChain == NULL )
		{
			DXST_ERRORLOG_ADDUTF16LE( "\x31\x59\x57\x65\x0a\x00\x00"/*@ L"���s\n" @*/ ) ;
			goto ERR ;
		}

		// ALT+Enter�𖳌�������
		DXGIFactory_MakeWindowAssociation( TargetWindow, D_DXGI_MWA_NO_WINDOW_CHANGES | D_DXGI_MWA_NO_ALT_ENTER ) ;

		DXST_ERRORLOG_ADDUTF16LE( "\x10\x62\x9f\x52\x0a\x00\x00"/*@ L"����\n" @*/ ) ;

		// �X���b�v�`�F�C���̏����擾����
		DXGISwapChain_GetDesc( OWI->DXGISwapChain, &OWI->DXGISwapChainDesc ) ;
	}


	DXST_ERRORLOG_ADDUTF16LE( "\x49\x00\x44\x00\x58\x00\x47\x00\x49\x00\x4f\x00\x75\x00\x74\x00\x70\x00\x75\x00\x74\x00\x20\x00\x92\x30\xd6\x53\x97\x5f\x57\x30\x7e\x30\x59\x30\x2e\x00\x2e\x00\x2e\x00\x2e\x00\x20\x00\x00"/*@ L"IDXGIOutput ���擾���܂�.... " @*/ ) ;

	// IDXGIOutput �̎擾
	Result = DXGISwapChain_GetContainingOutput( OWI->DXGISwapChain, &OWI->DXGIOutput ) ;
	if( Result != S_OK )
	{
		DXST_ERRORLOG_ADDUTF16LE( "\x31\x59\x57\x65\x0a\x00\x00"/*@ L"���s\n" @*/ ) ;
		goto ERR ;
	}

	DXST_ERRORLOG_ADDUTF16LE( "\x10\x62\x9f\x52\x0a\x00\x00"/*@ L"����\n" @*/ ) ;


	// �o�b�N�o�b�t�@�̃Z�b�g�A�b�v
	if( Graphics_D3D11_OutputWindow_SetupBuffer( NewIndex ) < 0 )
	{
		goto ERR ;
	}

	// �g�p���Ă��邩�ǂ����̃t���O�𗧂Ă�
	OWI->UseFlag = TRUE ;

	// �o�͐�E�C���h�E�̐��𑝂₷
	GD3D11.Device.Screen.OutputWindowNum ++ ;

	// ����I��
	return NewIndex ;

	// �G���[����
ERR :
	if( OWI->BufferSRV != NULL )
	{
		Direct3D11_Release_ShaderResourceView( OWI->BufferSRV ) ;
		OWI->BufferSRV = NULL ;
	}

	if( OWI->BufferRTV != NULL )
	{
		Direct3D11_Release_RenderTargetView( OWI->BufferRTV ) ;
		OWI->BufferRTV = NULL ;
	}

	if( OWI->BufferTexture2D != NULL )
	{
		Direct3D11_Release_Texture2D( OWI->BufferTexture2D ) ;
		OWI->BufferTexture2D = NULL ;
	}

	if( OWI->DXGIOutput != NULL )
	{
		Direct3D11_ObjectRelease( OWI->DXGIOutput ) ;
		OWI->DXGIOutput = NULL ;
	}

	if( OWI->DXGISwapChain != NULL )
	{
		Direct3D11_ObjectRelease( OWI->DXGISwapChain ) ;
		OWI->DXGISwapChain = NULL ;
	}

	return -1 ;
}

// �w��̃E�C���h�E���o�͐�E�C���h�E����O��
extern int Graphics_D3D11_OutputWindow_Sub( HWND TargetWindow, int Index )
{
	GRAPHICS_HARDDATA_DIRECT3D11_OUTPUTWINDOWINFO *OWI ;

	// TargetWindow �� NULL �̏ꍇ�� Index ���g�p����
	if( TargetWindow == NULL )
	{
		if( Index < 0 || Index >= DX_D3D11_MAX_OUTPUTWINDOW )
		{
			return -1 ;
		}
		OWI = &GD3D11.Device.Screen.OutputWindowInfo[ Index ] ;

		if( OWI->UseFlag == FALSE )
		{
			return -1 ;
		}
	}
	else
	{
		int i ;

		OWI = GD3D11.Device.Screen.OutputWindowInfo ;
		for( i = 0 ; i < DX_D3D11_MAX_OUTPUTWINDOW ; i ++, OWI ++ )
		{
			if( OWI->UseFlag && OWI->DXGISwapChainDesc.OutputWindow == TargetWindow )
			{
				break ;
			}
		}
		if( i == DX_D3D11_MAX_OUTPUTWINDOW )
		{
			return -1 ;
		}
		Index = i ;
	}

	Graphics_D3D11_OutputWindow_ReleaseBuffer( Index ) ;

	if( OWI->DXGIOutput != NULL )
	{
		Direct3D11_ObjectRelease( OWI->DXGIOutput ) ;
		OWI->DXGIOutput = NULL ;
	}

	if( OWI->DXGISwapChain != NULL )
	{
		Direct3D11_ObjectRelease( OWI->DXGISwapChain ) ;
		OWI->DXGISwapChain = NULL ;
	}

	// �g�p���Ă��邩�ǂ����̃t���O��|��
	if( OWI->UseFlag )
	{
		OWI->UseFlag = FALSE ;

		// �o�͐�E�C���h�E�̐������炷
		GD3D11.Device.Screen.OutputWindowNum -- ;
	}

	// �I��
	return 0 ;
}

// �w��̏o�͐�E�C���h�E�p�̃o�b�N�o�b�t�@�[���Z�b�g�A�b�v����
extern int Graphics_D3D11_OutputWindow_SetupBuffer( int Index )
{
	GRAPHICS_HARDDATA_DIRECT3D11_OUTPUTWINDOWINFO *OWI ;
	HRESULT Result ;

	if( Index < 0 || Index >= DX_D3D11_MAX_OUTPUTWINDOW )
	{
		return -1 ;
	}

	OWI = &GD3D11.Device.Screen.OutputWindowInfo[ Index ] ;


//	DXST_ERRORLOG_ADDUTF16LE( "\xb9\x30\xef\x30\xc3\x30\xd7\x30\xc1\x30\xa7\x30\xa4\x30\xf3\x30\x6e\x30\xd0\x30\xc3\x30\xd5\x30\xa1\x30\x6e\x30\x20\x00\x49\x00\x44\x00\x33\x00\x44\x00\x31\x00\x31\x00\x54\x00\x65\x00\x78\x00\x74\x00\x75\x00\x72\x00\x65\x00\x32\x00\x44\x00\x20\x00\x92\x30\xd6\x53\x97\x5f\x57\x30\x7e\x30\x59\x30\x2e\x00\x2e\x00\x2e\x00\x2e\x00\x20\x00\x00"/*@ L"�X���b�v�`�F�C���̃o�b�t�@�� ID3D11Texture2D ���擾���܂�.... " @*/ ) ;

	// �o�b�N�o�b�t�@���擾
	Result = DXGISwapChain_GetBuffer( OWI->DXGISwapChain, 0, IID_ID3D11TEXTURE2D, ( void ** )&OWI->BufferTexture2D ) ;
	if( Result != S_OK )
	{
//		DXST_ERRORLOG_ADDUTF16LE( "\x31\x59\x57\x65\x0a\x00\x00"/*@ L"���s\n" @*/ ) ;
		DXST_ERRORLOG_ADDUTF16LE( "\xb9\x30\xef\x30\xc3\x30\xd7\x30\xc1\x30\xa7\x30\xa4\x30\xf3\x30\x6e\x30\xd0\x30\xc3\x30\xd5\x30\xa1\x30\x6e\x30\x20\x00\x49\x00\x44\x00\x33\x00\x44\x00\x31\x00\x31\x00\x54\x00\x65\x00\x78\x00\x74\x00\x75\x00\x72\x00\x65\x00\x32\x00\x44\x00\x20\x00\x6e\x30\xd6\x53\x97\x5f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�X���b�v�`�F�C���̃o�b�t�@�� ID3D11Texture2D �̎擾�Ɏ��s���܂���\n" @*/ ) ;
		return -1 ;
	}

//	DXST_ERRORLOG_ADDUTF16LE( "\x10\x62\x9f\x52\x0a\x00\x00"/*@ L"����\n" @*/ ) ;


	// �o�b�N�o�b�t�@�̏����擾
	D3D11Texture2D_GetDesc( OWI->BufferTexture2D, &OWI->BufferTexture2DDesc ) ;


//	DXST_ERRORLOG_ADDUTF16LE( "\xb9\x30\xef\x30\xc3\x30\xd7\x30\xc1\x30\xa7\x30\xa4\x30\xf3\x30\x6e\x30\xd0\x30\xc3\x30\xd5\x30\xa1\x30\x6e\x30\x20\x00\x49\x00\x44\x00\x33\x00\x44\x00\x31\x00\x31\x00\x53\x00\x68\x00\x61\x00\x64\x00\x65\x00\x72\x00\x52\x00\x65\x00\x73\x00\x6f\x00\x75\x00\x72\x00\x63\x00\x65\x00\x56\x00\x69\x00\x65\x00\x77\x00\x20\x00\x92\x30\x5c\x4f\x10\x62\x57\x30\x7e\x30\x59\x30\x2e\x00\x2e\x00\x2e\x00\x2e\x00\x20\x00\x00"/*@ L"�X���b�v�`�F�C���̃o�b�t�@�� ID3D11ShaderResourceView ���쐬���܂�.... " @*/ ) ;

	// �o�b�N�o�b�t�@�p�̃V�F�[�_�[���\�[�X�r���[���쐬
	Result = D3D11Device_CreateShaderResourceView( OWI->BufferTexture2D, NULL, &OWI->BufferSRV ) ;
	if( Result != S_OK )
	{
//		DXST_ERRORLOG_ADDUTF16LE( "\x31\x59\x57\x65\x0a\x00\x00"/*@ L"���s\n" @*/ ) ;
		DXST_ERRORLOG_ADDUTF16LE( "\xb9\x30\xef\x30\xc3\x30\xd7\x30\xc1\x30\xa7\x30\xa4\x30\xf3\x30\x6e\x30\xd0\x30\xc3\x30\xd5\x30\xa1\x30\x6e\x30\x20\x00\x49\x00\x44\x00\x33\x00\x44\x00\x31\x00\x31\x00\x53\x00\x68\x00\x61\x00\x64\x00\x65\x00\x72\x00\x52\x00\x65\x00\x73\x00\x6f\x00\x75\x00\x72\x00\x63\x00\x65\x00\x56\x00\x69\x00\x65\x00\x77\x00\x20\x00\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�X���b�v�`�F�C���̃o�b�t�@�� ID3D11ShaderResourceView �̍쐬�Ɏ��s���܂���\n" @*/ ) ;
		return -1 ;
	}

//	DXST_ERRORLOG_ADDUTF16LE( "\x10\x62\x9f\x52\x0a\x00\x00"/*@ L"����\n" @*/ ) ;


//	DXST_ERRORLOG_ADDUTF16LE( "\xb9\x30\xef\x30\xc3\x30\xd7\x30\xc1\x30\xa7\x30\xa4\x30\xf3\x30\x6e\x30\xd0\x30\xc3\x30\xd5\x30\xa1\x30\x6e\x30\x20\x00\x49\x00\x44\x00\x33\x00\x44\x00\x31\x00\x31\x00\x52\x00\x65\x00\x6e\x00\x64\x00\x65\x00\x72\x00\x54\x00\x61\x00\x72\x00\x67\x00\x65\x00\x74\x00\x56\x00\x69\x00\x65\x00\x77\x00\x20\x00\x92\x30\x5c\x4f\x10\x62\x57\x30\x7e\x30\x59\x30\x2e\x00\x2e\x00\x2e\x00\x2e\x00\x20\x00\x00"/*@ L"�X���b�v�`�F�C���̃o�b�t�@�� ID3D11RenderTargetView ���쐬���܂�.... " @*/ ) ;

	// �o�b�N�o�b�t�@�p�̃����_�[�^�[�Q�b�g���쐬
	Result = D3D11Device_CreateRenderTargetView( OWI->BufferTexture2D, NULL, &OWI->BufferRTV ) ;
	if( Result != S_OK )
	{
//		DXST_ERRORLOG_ADDUTF16LE( "\x31\x59\x57\x65\x0a\x00\x00"/*@ L"���s\n" @*/ ) ;
		DXST_ERRORLOG_ADDUTF16LE( "\xb9\x30\xef\x30\xc3\x30\xd7\x30\xc1\x30\xa7\x30\xa4\x30\xf3\x30\x6e\x30\xd0\x30\xc3\x30\xd5\x30\xa1\x30\x6e\x30\x20\x00\x49\x00\x44\x00\x33\x00\x44\x00\x31\x00\x31\x00\x52\x00\x65\x00\x6e\x00\x64\x00\x65\x00\x72\x00\x54\x00\x61\x00\x72\x00\x67\x00\x65\x00\x74\x00\x56\x00\x69\x00\x65\x00\x77\x00\x20\x00\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�X���b�v�`�F�C���̃o�b�t�@�� ID3D11RenderTargetView �̍쐬�Ɏ��s���܂���\n" @*/ ) ;
		return -1 ;
	}

//	DXST_ERRORLOG_ADDUTF16LE( "\x10\x62\x9f\x52\x0a\x00\x00"/*@ L"����\n" @*/ ) ;


	// ����I��
	return 0 ;
}

// �w��̏o�͐�E�C���h�E�p�̃o�b�N�o�b�t�@�[�̌�n��������
extern int Graphics_D3D11_OutputWindow_ReleaseBuffer( int Index )
{
	GRAPHICS_HARDDATA_DIRECT3D11_OUTPUTWINDOWINFO *OWI ;

	if( Index < 0 || Index >= DX_D3D11_MAX_OUTPUTWINDOW )
	{
		return -1 ;
	}

	OWI = &GD3D11.Device.Screen.OutputWindowInfo[ Index ] ;

	if( OWI->BufferSRV != NULL )
	{
		Direct3D11_Release_ShaderResourceView( OWI->BufferSRV ) ;
		OWI->BufferSRV = NULL ;
	}

	if( OWI->BufferRTV != NULL )
	{
		Direct3D11_Release_RenderTargetView( OWI->BufferRTV ) ;
		OWI->BufferRTV = NULL ;
	}

	if( OWI->BufferTexture2D != NULL )
	{
		Direct3D11_Release_Texture2D( OWI->BufferTexture2D ) ;
		OWI->BufferTexture2D = NULL ;
	}

	// �I��
	return 0 ;
}

// �w��̏o�͐�E�C���h�E�̃o�b�N�o�b�t�@�̃T�C�Y��ύX����
extern int Graphics_D3D11_OutputWindow_ResizeBuffer( int Index, int NewSizeX, int NewSizeY )
{
	int     RenderTargetIndex ;
	int     i ;
	HRESULT hr ;
	GRAPHICS_HARDDATA_DIRECT3D11_OUTPUTWINDOWINFO *OWI ;

	if( Index < 0 || Index >= DX_D3D11_MAX_OUTPUTWINDOW )
	{
		return -1 ;
	}

	OWI = &GD3D11.Device.Screen.OutputWindowInfo[ Index ] ;

	// �w��̃o�b�N�o�b�t�@���`��ΏۂɂȂ��Ă���ꍇ�͈�x�O��
	RenderTargetIndex = -1 ;
	for( i = 0 ; i < DX_RENDERTARGET_COUNT ; i ++ )
	{
		if( GD3D11.Device.State.TargetTexture2D[ i ] == OWI->BufferTexture2D )
		{
			RenderTargetIndex = i ;
			Graphics_D3D11_DeviceState_SetRenderTarget( NULL, NULL, RenderTargetIndex ) ;
			break ;
		}
	}

	// �o�b�N�o�b�t�@�����
	Graphics_D3D11_OutputWindow_ReleaseBuffer( Index ) ;

	// �o�b�N�o�b�t�@�̃T�C�Y��ύX
	hr = DXGISwapChain_ResizeBuffers( OWI->DXGISwapChain, 2, ( UINT )NewSizeX, ( UINT )NewSizeY, DIRECT3D11_BACKBUFFER_FORMAT, 0 ) ;
	if( hr != S_OK )
	{
		DXST_ERRORLOG_ADDUTF16LE( "\xd0\x30\xc3\x30\xaf\x30\xd0\x30\xc3\x30\xd5\x30\xa1\x30\x6e\x30\xb5\x30\xa4\x30\xba\x30\x09\x59\xf4\x66\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�o�b�N�o�b�t�@�̃T�C�Y�ύX�Ɏ��s���܂���\n" @*/ ) ;
		return -1 ;
	}

	// �o�b�N�o�b�t�@�̃Z�b�g�A�b�v
	if( Graphics_D3D11_OutputWindow_SetupBuffer( Index ) < 0 )
	{
		return -1 ;
	}

	// �o�b�N�o�b�t�@���`��ΏۂɂȂ��Ă�����ēx�`��Ώۂɐݒ肷��
	if( RenderTargetIndex != -1 )
	{
		Graphics_D3D11_DeviceState_SetRenderTarget( OWI->BufferTexture2D, OWI->BufferRTV, RenderTargetIndex ) ;
	}

	// �I��
	return 0 ;
}

// �[�x�o�b�t�@�I�u�W�F�N�g�̍쐬( 0:����  -1:���s )
extern	int		Graphics_D3D11_CreateDepthBuffer( void )
{
	HRESULT hr ;
	int depth ;
	int index ;

	if( GAPIWin.D3D11DeviceObject == NULL )
	{
		return -1 ;
	}

	// ���ɍ쐬�ς݂̏ꍇ�͈�x�폜����
	if( GD3D11.Device.Screen.DepthBufferTexture2D )
	{
		Direct3D11_Release_Texture2D( GD3D11.Device.Screen.DepthBufferTexture2D ) ;
		GD3D11.Device.Screen.DepthBufferTexture2D = NULL ;
	}
	if( GD3D11.Device.Screen.DepthBufferDSV )
	{
		Direct3D11_Release_DepthStencilView( GD3D11.Device.Screen.DepthBufferDSV ) ;
		GD3D11.Device.Screen.DepthBufferDSV = NULL ;
	}

	DXST_ERRORLOG_ADDUTF16LE( "\xf1\x6d\xa6\x5e\xd0\x30\xc3\x30\xd5\x30\xa1\x30\x92\x30\x5c\x4f\x10\x62\x57\x30\x7e\x30\x59\x30\x2e\x00\x2e\x00\x2e\x00\x2e\x00\x20\x00\x00"/*@ L"�[�x�o�b�t�@���쐬���܂�.... " @*/ ) ;

	// �[�x�o�b�t�@�̃T�C�Y������
	GD3D11.Device.Screen.DepthBufferSizeX = GSYS.Screen.MainScreenSizeX ;
	GD3D11.Device.Screen.DepthBufferSizeY = GSYS.Screen.MainScreenSizeY ;

	// �[�x�o�b�t�@�̃r�b�g�[�x������
	if( GD3D11.Setting.UserDepthBufferBitDepthSet == FALSE ||
		GD3D11.Device.Screen.DepthBufferBitDepth == 0  )
	{
		depth = DEFAULT_ZBUFFER_BITDEPTH ;
		GD3D11.Device.Screen.DepthBufferBitDepth = DEFAULT_ZBUFFER_BITDEPTH ;
	}
	else
	{
		depth = GD3D11.Device.Screen.DepthBufferBitDepth ;
	}
	switch( depth )
	{
	default :
	case 16 : index = ZBUFFER_FORMAT_16BIT ; break ;
	case 24 : index = ZBUFFER_FORMAT_24BIT ; break ;
	case 32 : index = ZBUFFER_FORMAT_32BIT ; break ;
	}

	// �[�x�o�b�t�@�e�N�X�`���̍쐬
	{
		D_D3D11_TEXTURE2D_DESC Texture2DDesc ;

		_MEMSET( &Texture2DDesc, 0, sizeof( Texture2DDesc ) ) ;
		Texture2DDesc.Width              = ( UINT )GSYS.Screen.MainScreenSizeX ;
		Texture2DDesc.Height             = ( UINT )GSYS.Screen.MainScreenSizeY ;
		Texture2DDesc.MipLevels          = 1 ;
		Texture2DDesc.ArraySize          = 1 ;
		Texture2DDesc.Format             = GD3D11.Device.Caps.DepthBufferTexture2DFormat[ index ] ;
//		Texture2DDesc.Format             = GD3D11.Device.Caps.DepthBufferDepthStencilFormat[ index ] ;
		Texture2DDesc.SampleDesc.Count   = ( UINT )GSYS.Setting.FSAAMultiSampleCount ;
		Texture2DDesc.SampleDesc.Quality = ( UINT )GSYS.Setting.FSAAMultiSampleQuality ;
		Texture2DDesc.Usage              = D_D3D11_USAGE_DEFAULT ;
		Texture2DDesc.BindFlags          = D_D3D11_BIND_DEPTH_STENCIL /* | D_D3D11_BIND_SHADER_RESOURCE */ ;
		Texture2DDesc.CPUAccessFlags     = 0 ;
		Texture2DDesc.MiscFlags          = 0 ;

		// �쐬
		hr = D3D11Device_CreateTexture2D( &Texture2DDesc, NULL, &GD3D11.Device.Screen.DepthBufferTexture2D ) ;
		if( FAILED( hr ) )
		{
			DXST_ERRORLOG_ADDUTF16LE( "\xf1\x6d\xa6\x5e\xd0\x30\xc3\x30\xd5\x30\xa1\x30\x28\x75\x20\x00\x54\x00\x65\x00\x78\x00\x74\x00\x75\x00\x72\x00\x65\x00\x32\x00\x44\x00\x20\x00\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�[�x�o�b�t�@�p Texture2D �̍쐬�Ɏ��s���܂���\n" @*/ ) ;
			return -1 ;
		}
	}

	// �[�x�X�e���V���r���[�̍쐬
	{
		D_D3D11_DEPTH_STENCIL_VIEW_DESC DepthStencilViewDesc ;

		_MEMSET( &DepthStencilViewDesc, 0, sizeof( DepthStencilViewDesc ) ) ;
		DepthStencilViewDesc.Format = GD3D11.Device.Caps.DepthBufferDepthStencilFormat[ index ] ;
		if ( GSYS.Setting.FSAAMultiSampleCount <= 1 )
		{
			DepthStencilViewDesc.ViewDimension      = D_D3D11_DSV_DIMENSION_TEXTURE2D;
			DepthStencilViewDesc.Texture2D.MipSlice = 0;
		}
		else
		{
			DepthStencilViewDesc.ViewDimension		= D_D3D11_DSV_DIMENSION_TEXTURE2DMS;
		}

		hr = D3D11Device_CreateDepthStencilView( GD3D11.Device.Screen.DepthBufferTexture2D, &DepthStencilViewDesc, &GD3D11.Device.Screen.DepthBufferDSV ) ;
		if( FAILED( hr ) )
		{
			DXST_ERRORLOG_ADDUTF16LE( "\xf1\x6d\xa6\x5e\xd0\x30\xc3\x30\xd5\x30\xa1\x30\x28\x75\x20\x00\x44\x00\x65\x00\x70\x00\x74\x00\x68\x00\x53\x00\x74\x00\x65\x00\x6e\x00\x63\x00\x69\x00\x6c\x00\x56\x00\x69\x00\x65\x00\x77\x00\x20\x00\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�[�x�o�b�t�@�p DepthStencilView �̍쐬�Ɏ��s���܂���\n" @*/ ) ;
			return -1 ;
		}
	}

	// �f�o�C�X�ɃZ�b�g
	Graphics_D3D11_DeviceState_SetDepthStencil( GD3D11.Device.Screen.DepthBufferTexture2D, GD3D11.Device.Screen.DepthBufferDSV ) ;

	// �N���A
	D3D11DeviceContext_ClearDepthStencilView( GD3D11.Device.Screen.DepthBufferDSV, D_D3D11_CLEAR_DEPTH, 1.0f, 0 ) ;

	DXST_ERRORLOG_ADDUTF16LE( "\x10\x62\x9f\x52\x0a\x00\x00"/*@ L"����\n" @*/ ) ;

	// �I��
	return 0 ;
}























// Direct3D11 �̉摜�֌W

// D_DXGI_FORMAT �̃t�H�[�}�b�g�ɍ��킹���J���[�t�H�[�}�b�g�����擾����
extern COLORDATA *Graphics_D3D11_GetD3DFormatColorData( D_DXGI_FORMAT Format )
{
	volatile static int InitializeFlag = FALSE ;
	static COLORDATA B5G6R5_ColorData ;
	static COLORDATA R8G8B8A8_ColorData ;
	static COLORDATA B8G8R8X8_ColorData ;
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
		NS_CreateColorData( &B5G6R5_ColorData,         16, 0x0000f800, 0x000007e0, 0x0000001f, 0x00000000 ) ;
		NS_CreateColorData( &R8G8B8A8_ColorData,       32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000 ) ;
		NS_CreateColorData( &B8G8R8X8_ColorData,       32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0x00000000 ) ;
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
		InitializeFlag = TRUE ;
	}

	switch( Format )
	{
	case D_DXGI_FORMAT_B5G6R5_UNORM :       return &B5G6R5_ColorData ;
	case D_DXGI_FORMAT_R8G8B8A8_UNORM :     return &R8G8B8A8_ColorData ;
	case D_DXGI_FORMAT_B8G8R8X8_UNORM :     return &B8G8R8X8_ColorData ;
	case D_DXGI_FORMAT_R16G16B16A16_UNORM : return &A16B16G16R16I_ColorData ;
	case D_DXGI_FORMAT_R16G16B16A16_FLOAT : return &A16B16G16R16F_ColorData ;
	case D_DXGI_FORMAT_R32G32B32A32_FLOAT : return &A32B32G32R32F_ColorData ;
	case D_DXGI_FORMAT_R8_UNORM :           return &ONE_I8_ColorData ;
	case D_DXGI_FORMAT_R16_UNORM :          return &ONE_I16_ColorData ;
	case D_DXGI_FORMAT_R16_FLOAT :          return &ONE_F16_ColorData ;
	case D_DXGI_FORMAT_R32_FLOAT :          return &ONE_F32_ColorData ;
	case D_DXGI_FORMAT_R8G8_UNORM :         return &TWO_I8_ColorData ;
	case D_DXGI_FORMAT_R16G16_UNORM :       return &TWO_I16_ColorData ;
	case D_DXGI_FORMAT_R16G16_FLOAT :       return &TWO_F16_ColorData ;
	case D_DXGI_FORMAT_R32G32_FLOAT :       return &TWO_F32_ColorData ;
	}
	return NULL ;
}

// �`���p�e�N�X�`���ƕ`��p�e�N�X�`����������Ă���ꍇ�ɁA�`��p�e�N�X�`���ɕ`���p�e�N�X�`���̓��e�𔽉f����
extern	int		Graphics_D3D11_UpdateDrawTexture( IMAGEDATA_ORIG_HARD_TEX *OrigTex, int TargetSurface, D_DXGI_FORMAT Format )
{
	D3D11DeviceContext_ResolveSubresource(
		OrigTex->PF->D3D11.Texture,
		D_D3D11CalcSubresource( 0, TargetSurface, 1 ),
		OrigTex->PF->D3D11.MSTexture,
		D_D3D11CalcSubresource( 0, TargetSurface, 1 ),
		Format
	) ;

	// �I��
	return 0 ;
}

// ��{�C���[�W�̃t�H�[�}�b�g�� DX_BASEIMAGE_FORMAT_NORMAL �ɕϊ�����K�v�����邩�ǂ������`�F�b�N����
// ( RequiredRgbBaseImageConvFlag �� RequiredAlphaBaseImageConvFlag �ɓ���l  TRUE:�ϊ�����K�v������  FALSE:�ϊ�����K�v�͖��� )
extern	int		Graphics_D3D11_CheckRequiredNormalImageConv_BaseImageFormat_PF(
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
		if( ( RgbBaseImageFormat == DX_BASEIMAGE_FORMAT_DXT1 && GD3D11.Device.Caps.TextureFormat[ Orig->ColorFormat ] != D_DXGI_FORMAT_BC1_UNORM ) ||
			( RgbBaseImageFormat == DX_BASEIMAGE_FORMAT_DXT2                                                                                     ) ||
			( RgbBaseImageFormat == DX_BASEIMAGE_FORMAT_DXT3 && GD3D11.Device.Caps.TextureFormat[ Orig->ColorFormat ] != D_DXGI_FORMAT_BC2_UNORM ) ||
			( RgbBaseImageFormat == DX_BASEIMAGE_FORMAT_DXT4                                                                                      ) ||
			( RgbBaseImageFormat == DX_BASEIMAGE_FORMAT_DXT5 && GD3D11.Device.Caps.TextureFormat[ Orig->ColorFormat ] != D_DXGI_FORMAT_BC3_UNORM ) )
		{
			RgbConvFlag = TRUE ;
		}

		if( AlphaBaseImageFormat >= 0 )
		{
			if( ( AlphaBaseImageFormat == DX_BASEIMAGE_FORMAT_DXT1 && GD3D11.Device.Caps.TextureFormat[ Orig->ColorFormat ] != D_DXGI_FORMAT_BC1_UNORM ) ||
				( AlphaBaseImageFormat == DX_BASEIMAGE_FORMAT_DXT2                                                                                     ) ||
				( AlphaBaseImageFormat == DX_BASEIMAGE_FORMAT_DXT3 && GD3D11.Device.Caps.TextureFormat[ Orig->ColorFormat ] != D_DXGI_FORMAT_BC2_UNORM ) ||
				( AlphaBaseImageFormat == DX_BASEIMAGE_FORMAT_DXT4                                                                                     ) ||
				( AlphaBaseImageFormat == DX_BASEIMAGE_FORMAT_DXT5 && GD3D11.Device.Caps.TextureFormat[ Orig->ColorFormat ] != D_DXGI_FORMAT_BC3_UNORM ) )
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

// �e�N�X�`���摜�]���p�̋��L�������̃Z�b�g�A�b�v���s��
extern int Graphics_D3D11_Texture_SetupCommonBuffer( unsigned int Size )
{
	// ���Ɏw��̃T�C�Y���m�ۂ��Ă������牽�������ɏI��
	if( GD3D11.Texture.CommonBufferSize >= Size )
	{
		return 0 ;
	}

	// �����̃����������
	DXFREE( GD3D11.Texture.CommonBuffer ) ;
	GD3D11.Texture.CommonBuffer     = NULL ;
	GD3D11.Texture.CommonBufferSize = 0 ;

	// �������̊m��
	GD3D11.Texture.CommonBuffer = DXALLOC( Size ) ;
	if( GD3D11.Texture.CommonBuffer == NULL )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\x6b\x30\x3b\x75\xcf\x50\x92\x30\xe2\x8e\x01\x90\x59\x30\x8b\x30\x5f\x30\x81\x30\x6e\x30\xe1\x30\xe2\x30\xea\x30\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x5b\x00\x47\x00\x72\x00\x61\x00\x70\x00\x68\x00\x69\x00\x63\x00\x73\x00\x5f\x00\x44\x00\x33\x00\x44\x00\x31\x00\x31\x00\x5f\x00\x54\x00\x65\x00\x78\x00\x74\x00\x75\x00\x72\x00\x65\x00\x5f\x00\x53\x00\x65\x00\x74\x00\x75\x00\x70\x00\x43\x00\x6f\x00\x6d\x00\x6d\x00\x6f\x00\x6e\x00\x42\x00\x75\x00\x66\x00\x66\x00\x65\x00\x72\x00\x5d\x00\x0a\x00\x00"/*@ L"�e�N�X�`���ɉ摜��]�����邽�߂̃������̊m�ۂɎ��s���܂���[Graphics_D3D11_Texture_SetupCommonBuffer]\n" @*/ )) ;
		return -1 ;
	}

	// �m�ۂ����������̃T�C�Y��ۑ�
	GD3D11.Texture.CommonBufferSize = Size ;

	// �I��
	return 0 ;
}

// �e�N�X�`���摜�]���p�̋��L�������̌�n�����s��
extern int Graphics_D3D11_Texture_TerminateCommonBuffer( void )
{
	// ���������m�ۂ��Ă�������
	if( GD3D11.Texture.CommonBuffer != NULL )
	{
		DXFREE( GD3D11.Texture.CommonBuffer ) ;
		GD3D11.Texture.CommonBuffer = NULL ;
	}
	GD3D11.Texture.CommonBufferSize = 0 ;

	// �I��
	return 0 ;
}

// �P���]�������̏��������s��
extern int Graphics_D3D11_StretchRect_Initialize( void )
{
	HRESULT hr ;
	int i ;

	DXST_ERRORLOG_ADDUTF16LE( "\x3b\x75\xcf\x50\x6e\x30\x58\x53\x14\x7d\xe2\x8e\x01\x90\xe6\x51\x06\x74\x6e\x30\x1d\x52\x1f\x67\x16\x53\x2e\x00\x2e\x00\x2e\x00\x20\x00\x00"/*@ L"�摜�̒P���]�������̏�����... " @*/ ) ;

	// �u�����h�X�e�[�g�̍쐬
	if( GD3D11.Device.Shader.Base.StretchRect_BlendState[ 0 ] == NULL )
	{
		D_D3D11_BLEND_DESC BlendDesc ;
		static D_D3D11_RENDER_TARGET_BLEND_DESC RenderTargetBlendDesc_NoBlend = 
		{
			FALSE,
			D_D3D11_BLEND_ONE,
			D_D3D11_BLEND_ZERO,
			D_D3D11_BLEND_OP_ADD,
			D_D3D11_BLEND_ONE,
			D_D3D11_BLEND_ZERO,
			D_D3D11_BLEND_OP_ADD,
			D_D3D11_COLOR_WRITE_ENABLE_ALL
		} ;

		static D_D3D11_RENDER_TARGET_BLEND_DESC RenderTargetBlendDesc_AlphaBlend = 
		{
			TRUE,
			D_D3D11_BLEND_SRC_ALPHA,
			D_D3D11_BLEND_INV_SRC_ALPHA,
			D_D3D11_BLEND_OP_ADD,
			D_D3D11_BLEND_SRC_ALPHA,
			D_D3D11_BLEND_INV_SRC_ALPHA,
			D_D3D11_BLEND_OP_ADD,
			D_D3D11_COLOR_WRITE_ENABLE_ALL
		} ;

		BlendDesc.AlphaToCoverageEnable  = FALSE ;
		BlendDesc.IndependentBlendEnable = FALSE ;
		for( i = 0; i < 8; i++ )
		{
			BlendDesc.RenderTarget[ i ] = RenderTargetBlendDesc_NoBlend ;
		}

		hr = D3D11Device_CreateBlendState( &BlendDesc, &GD3D11.Device.Shader.Base.StretchRect_BlendState[ 0 ] ) ;
		if( hr != S_OK )
		{
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\x58\x53\x14\x7d\xe2\x8e\x01\x90\x28\x75\x6e\x30\x20\x00\x49\x00\x44\x00\x33\x00\x44\x00\x31\x00\x31\x00\x42\x00\x6c\x00\x65\x00\x6e\x00\x64\x00\x53\x00\x74\x00\x61\x00\x74\x00\x65\x00\x20\x00\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�P���]���p�� ID3D11BlendState �̍쐬�Ɏ��s���܂���\n" @*/ )) ;
			return -1 ;
		}

		BlendDesc.RenderTarget[ 0 ] = RenderTargetBlendDesc_AlphaBlend ;
		hr = D3D11Device_CreateBlendState( &BlendDesc, &GD3D11.Device.Shader.Base.StretchRect_BlendState[ 1 ] ) ;
		if( hr != S_OK )
		{
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\x58\x53\x14\x7d\xa2\x30\xeb\x30\xd5\x30\xa1\x30\xd6\x30\xec\x30\xf3\x30\xc9\x30\xe2\x8e\x01\x90\x28\x75\x6e\x30\x20\x00\x49\x00\x44\x00\x33\x00\x44\x00\x31\x00\x31\x00\x42\x00\x6c\x00\x65\x00\x6e\x00\x64\x00\x53\x00\x74\x00\x61\x00\x74\x00\x65\x00\x20\x00\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�P���A���t�@�u�����h�]���p�� ID3D11BlendState �̍쐬�Ɏ��s���܂���\n" @*/ )) ;
			return -1 ;
		}
	}

	// �T���v���[�X�e�[�g�̍쐬
	{
		static D_D3D11_SAMPLER_DESC SamplerDesc =
		{
			D_D3D11_FILTER_MIN_MAG_MIP_POINT,
			D_D3D11_TEXTURE_ADDRESS_CLAMP,
			D_D3D11_TEXTURE_ADDRESS_CLAMP,
			D_D3D11_TEXTURE_ADDRESS_CLAMP,
			0.0f,
			16,
			D_D3D11_COMPARISON_ALWAYS,
			{ 0.0f, 0.0f, 0.0f, 0.0f },
			-D_D3D11_FLOAT32_MAX,
			 D_D3D11_FLOAT32_MAX
		} ;

		if( GD3D11.Device.Shader.Base.StretchRect_SamplerState[ 0 ] == NULL )
		{
			SamplerDesc.Filter = D_D3D11_FILTER_MIN_MAG_MIP_POINT ;
			hr = D3D11Device_CreateSamplerState( &SamplerDesc, &GD3D11.Device.Shader.Base.StretchRect_SamplerState[ 0 ] ) ;
			if( hr != S_OK )
			{
				DXST_ERRORLOGFMT_ADDUTF16LE(( "\x00\x67\xd1\x8f\xb9\x70\xb5\x30\xf3\x30\xd7\x30\xea\x30\xf3\x30\xb0\x30\x58\x53\x14\x7d\xe2\x8e\x01\x90\x28\x75\x6e\x30\x20\x00\x49\x00\x44\x00\x33\x00\x44\x00\x31\x00\x31\x00\x53\x00\x61\x00\x6d\x00\x70\x00\x6c\x00\x65\x00\x72\x00\x53\x00\x74\x00\x61\x00\x74\x00\x65\x00\x20\x00\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�ŋߓ_�T���v�����O�P���]���p�� ID3D11SamplerState �̍쐬�Ɏ��s���܂���\n" @*/ )) ;
				return -1 ;
			}
		}

		if( GD3D11.Device.Shader.Base.StretchRect_SamplerState[ 1 ] == NULL )
		{
			SamplerDesc.Filter = D_D3D11_FILTER_MIN_MAG_MIP_LINEAR ;
			hr = D3D11Device_CreateSamplerState( &SamplerDesc, &GD3D11.Device.Shader.Base.StretchRect_SamplerState[ 1 ] ) ;
			if( hr != S_OK )
			{
				DXST_ERRORLOGFMT_ADDUTF16LE(( "\xd0\x30\xa4\x30\xea\x30\xcb\x30\xa2\x30\xdc\x88\x93\x95\x58\x53\x14\x7d\xe2\x8e\x01\x90\x28\x75\x6e\x30\x20\x00\x49\x00\x44\x00\x33\x00\x44\x00\x31\x00\x31\x00\x53\x00\x61\x00\x6d\x00\x70\x00\x6c\x00\x65\x00\x72\x00\x53\x00\x74\x00\x61\x00\x74\x00\x65\x00\x20\x00\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�o�C���j�A��ԒP���]���p�� ID3D11SamplerState �̍쐬�Ɏ��s���܂���\n" @*/ )) ;
				return -1 ;
			}
		}
	}

	// ���X�^���C�U�[�X�e�[�g�̍쐬
	if( GD3D11.Device.Shader.Base.StretchRect_RasterizerState == NULL )
	{
		static D_D3D11_RASTERIZER_DESC RasterizerDesc =
		{
			D_D3D11_FILL_SOLID,
			D_D3D11_CULL_BACK,
			0,
			0,
			0.0f,
			0.0f,
			TRUE,
			FALSE,
			FALSE,
			FALSE
		} ;

		hr = D3D11Device_CreateRasterizerState( &RasterizerDesc, &GD3D11.Device.Shader.Base.StretchRect_RasterizerState ) ;
		if( hr != S_OK )
		{
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\x58\x53\x14\x7d\xe2\x8e\x01\x90\x28\x75\x6e\x30\x20\x00\x49\x00\x44\x00\x33\x00\x44\x00\x31\x00\x31\x00\x52\x00\x61\x00\x73\x00\x74\x00\x65\x00\x72\x00\x69\x00\x7a\x00\x65\x00\x72\x00\x53\x00\x74\x00\x61\x00\x74\x00\x65\x00\x20\x00\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�P���]���p�� ID3D11RasterizerState �̍쐬�Ɏ��s���܂���\n" @*/ )) ;
			return -1 ;
		}
	}

	// �[�x�X�e���V���X�e�[�g�̍쐬
	if( GD3D11.Device.Shader.Base.StretchRect_DepthStencilState == NULL )
	{
		static D_D3D11_DEPTH_STENCIL_DESC DepthStencilDesc =
		{
			FALSE,
			D_D3D11_DEPTH_WRITE_MASK_ZERO,
			D_D3D11_COMPARISON_ALWAYS,
			FALSE,
			D_D3D11_DEFAULT_STENCIL_READ_MASK,
			D_D3D11_DEFAULT_STENCIL_WRITE_MASK,
			{
				D_D3D11_STENCIL_OP_KEEP,
				D_D3D11_STENCIL_OP_KEEP,
				D_D3D11_STENCIL_OP_KEEP,
				D_D3D11_COMPARISON_ALWAYS
			},
			{
				D_D3D11_STENCIL_OP_KEEP,
				D_D3D11_STENCIL_OP_KEEP,
				D_D3D11_STENCIL_OP_KEEP,
				D_D3D11_COMPARISON_ALWAYS
			}
		} ;

		// �悭�g���ݒ�̐[�x�X�e���V���X�e�[�g�ł͂Ȃ��ꍇ�͂����Ő[�x�X�e���V���X�e�[�g���쐬����
		hr = D3D11Device_CreateDepthStencilState( &DepthStencilDesc, &GD3D11.Device.Shader.Base.StretchRect_DepthStencilState ) ;
		if( hr != S_OK )
		{
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\x58\x53\x14\x7d\xe2\x8e\x01\x90\x28\x75\x6e\x30\x20\x00\x49\x00\x44\x00\x33\x00\x44\x00\x31\x00\x31\x00\x44\x00\x65\x00\x70\x00\x74\x00\x68\x00\x53\x00\x74\x00\x65\x00\x6e\x00\x63\x00\x69\x00\x6c\x00\x53\x00\x74\x00\x61\x00\x74\x00\x65\x00\x20\x00\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�P���]���p�� ID3D11DepthStencilState �̍쐬�Ɏ��s���܂���\n" @*/ )) ;
			return -1 ;
		}
	}

	DXST_ERRORLOG_ADDUTF16LE( "\x10\x62\x9f\x52\x0a\x00\x00"/*@ L"����\n" @*/ ) ;

	// ����I��
	return 0 ;
}

// �P���]�������̌�n�����s��
extern int Graphics_D3D11_StretchRect_Terminate( void )
{
	int i ;

	for( i = 0 ; i < 2 ; i ++ )
	{
		if( GD3D11.Device.Shader.Base.StretchRect_SamplerState[ i ] != NULL )
		{
			Direct3D11_Release_SamplerState( GD3D11.Device.Shader.Base.StretchRect_SamplerState[ i ] ) ;
			GD3D11.Device.Shader.Base.StretchRect_SamplerState[ i ] = NULL ;
		}
	}

	if( GD3D11.Device.Shader.Base.StretchRect_RasterizerState != NULL )
	{
		Direct3D11_Release_RasterizerState( GD3D11.Device.Shader.Base.StretchRect_RasterizerState ) ;
		GD3D11.Device.Shader.Base.StretchRect_RasterizerState = NULL ;
	}

	if( GD3D11.Device.Shader.Base.StretchRect_DepthStencilState != NULL )
	{
		Direct3D11_Release_DepthStencilState( GD3D11.Device.Shader.Base.StretchRect_DepthStencilState ) ;
		GD3D11.Device.Shader.Base.StretchRect_DepthStencilState = NULL ;
	}

	for( i = 0 ; i < 2 ; i ++ )
	{
		if( GD3D11.Device.Shader.Base.StretchRect_BlendState[ i ] != NULL )
		{
			Direct3D11_Release_BlendState( GD3D11.Device.Shader.Base.StretchRect_BlendState[ i ] ) ;
			GD3D11.Device.Shader.Base.StretchRect_BlendState[ i ] = NULL ;
		}
	}

	// ����I��
	return 0 ;
}

// �����_�[�^�[�Q�b�g�e�N�X�`���Ƀe�N�X�`���̒P���]�����s��
extern int		Graphics_D3D11_StretchRect(
	D_ID3D11Texture2D *SrcTexture,  D_ID3D11ShaderResourceView *SrcTextureSRV,  const RECT *SrcRect,
	D_ID3D11Texture2D *DestTexture, D_ID3D11RenderTargetView   *DestTextureRTV, const RECT *DestRect,
	D_D3D11_FILTER_TYPE FilterType,
	int AlphaBlend,
	D_ID3D11VertexShader *VertexShader,
	D_ID3D11PixelShader  *PixelShader,
	D_ID3D11Texture2D *BlendTexture, D_ID3D11ShaderResourceView  *BlendTextureSRV, const RECT *BlendRect,
	VERTEX_TEX8_2D *Texcoord8Vertex
)
{
	float                  VertexData[ 4 ][ 6 ] ;
	float                  VertexDataTex8[ 4 ][ 18 ] ;
	D_D3D11_TEXTURE2D_DESC SrcDesc ;
	D_D3D11_TEXTURE2D_DESC DestDesc ;
	D_D3D11_TEXTURE2D_DESC BlendDesc = { 0 } ;
	D_D3D11_VIEWPORT       Viewport ;
	RECTF                  SrcRectF ;
	RECTF                  DestRectF ;
	RECTF                  BlendRectF = { 0.0f } ;
//	D_D3D11_BOX            DestBox ;
	UINT                   Offsets = 0 ;
	UINT                   VertexStride ;
	RECT                   SrcRectTemp ;
	RECT                   DestRectTemp ;
	RECT                   BlendRectTemp ;
	static FLOAT BlendFactor[ 4 ] = { 0.0f } ;

	DRAWSTOCKINFO

	// �e�N�X�`�������擾
	SrcTexture->GetDesc(  &SrcDesc  ) ;
	DestTexture->GetDesc( &DestDesc ) ;
	if( BlendTexture != NULL )
	{
		BlendTexture->GetDesc( &BlendDesc ) ;
	}

	if( SrcRect == NULL )
	{
		SrcRectTemp.left   = 0 ;
		SrcRectTemp.top    = 0 ;
		SrcRectTemp.right  = ( LONG )SrcDesc.Width ;
		SrcRectTemp.bottom = ( LONG )SrcDesc.Height ;

		SrcRect = &SrcRectTemp ;
	}

	if( DestRect == NULL )
	{
		DestRectTemp.left   = 0 ;
		DestRectTemp.top    = 0 ;
		DestRectTemp.right  = ( LONG )DestDesc.Width ;
		DestRectTemp.bottom = ( LONG )DestDesc.Height ;

		DestRect = &DestRectTemp ;
	}

	// ���_�f�[�^������
	if( Texcoord8Vertex == NULL )
	{
		DestRectF.left   =     ( float )DestRect->left   / DestDesc.Width  * 2.0f - 1.0f   ;
		DestRectF.top    = - ( ( float )DestRect->top    / DestDesc.Height * 2.0f - 1.0f ) ;
		DestRectF.right  =     ( float )DestRect->right  / DestDesc.Width  * 2.0f - 1.0f   ;
		DestRectF.bottom = - ( ( float )DestRect->bottom / DestDesc.Height * 2.0f - 1.0f ) ;

		SrcRectF.left    = ( float )SrcRect->left    / SrcDesc.Width ;
		SrcRectF.top     = ( float )SrcRect->top     / SrcDesc.Height ;
		SrcRectF.right   = ( float )SrcRect->right   / SrcDesc.Width ;
		SrcRectF.bottom  = ( float )SrcRect->bottom  / SrcDesc.Height ;

		if( BlendTexture != NULL )
		{
			if( BlendRect == NULL )
			{
				BlendRectTemp.left   = 0 ;
				BlendRectTemp.top    = 0 ;
				BlendRectTemp.right  = ( LONG )BlendDesc.Width ;
				BlendRectTemp.bottom = ( LONG )BlendDesc.Height ;

				BlendRect = &BlendRectTemp ;
			}

			BlendRectF.left    = ( float )BlendRect->left    / BlendDesc.Width ;
			BlendRectF.top     = ( float )BlendRect->top     / BlendDesc.Height ;
			BlendRectF.right   = ( float )BlendRect->right   / BlendDesc.Width ;
			BlendRectF.bottom  = ( float )BlendRect->bottom  / BlendDesc.Height ;
		}

		VertexData[ 0 ][ 0 ] = DestRectF.left ;
		VertexData[ 0 ][ 1 ] = DestRectF.top ;
		VertexData[ 0 ][ 2 ] = SrcRectF.left ;
		VertexData[ 0 ][ 3 ] = SrcRectF.top ;

		VertexData[ 1 ][ 0 ] = DestRectF.right ;
		VertexData[ 1 ][ 1 ] = DestRectF.top ;
		VertexData[ 1 ][ 2 ] = SrcRectF.right ;
		VertexData[ 1 ][ 3 ] = SrcRectF.top ;

		VertexData[ 2 ][ 0 ] = DestRectF.left ;
		VertexData[ 2 ][ 1 ] = DestRectF.bottom ;
		VertexData[ 2 ][ 2 ] = SrcRectF.left ;
		VertexData[ 2 ][ 3 ] = SrcRectF.bottom ;

		VertexData[ 3 ][ 0 ] = DestRectF.right ;
		VertexData[ 3 ][ 1 ] = DestRectF.bottom ;
		VertexData[ 3 ][ 2 ] = SrcRectF.right ;
		VertexData[ 3 ][ 3 ] = SrcRectF.bottom ;

		if( BlendTexture != NULL )
		{
			VertexData[ 0 ][ 4 ] = BlendRectF.left ;
			VertexData[ 0 ][ 5 ] = BlendRectF.top ;

			VertexData[ 1 ][ 4 ] = BlendRectF.right ;
			VertexData[ 1 ][ 5 ] = BlendRectF.top ;

			VertexData[ 2 ][ 4 ] = BlendRectF.left ;
			VertexData[ 2 ][ 5 ] = BlendRectF.bottom ;

			VertexData[ 3 ][ 4 ] = BlendRectF.right ;
			VertexData[ 3 ][ 5 ] = BlendRectF.bottom ;
		}
		else
		{
			VertexData[ 0 ][ 4 ] = 0.0f ;
			VertexData[ 0 ][ 5 ] = 0.0f ;

			VertexData[ 1 ][ 4 ] = 0.0f ;
			VertexData[ 1 ][ 5 ] = 0.0f ;

			VertexData[ 2 ][ 4 ] = 0.0f ;
			VertexData[ 2 ][ 5 ] = 0.0f ;

			VertexData[ 3 ][ 4 ] = 0.0f ;
			VertexData[ 3 ][ 5 ] = 0.0f ;
		}
	}
	else
	{
		int i ;

		for( i = 0 ; i < 4 ; i ++ )
		{
			VertexDataTex8[ i ][  0 ] =     Texcoord8Vertex[ i ].pos.x / DestDesc.Width  * 2.0f - 1.0f   ;
			VertexDataTex8[ i ][  1 ] = - ( Texcoord8Vertex[ i ].pos.y / DestDesc.Height * 2.0f - 1.0f ) ;

			VertexDataTex8[ i ][  2 ] = Texcoord8Vertex[ i ].u0 ;
			VertexDataTex8[ i ][  3 ] = Texcoord8Vertex[ i ].v0 ;
			VertexDataTex8[ i ][  4 ] = Texcoord8Vertex[ i ].u1 ;
			VertexDataTex8[ i ][  5 ] = Texcoord8Vertex[ i ].v1 ;
			VertexDataTex8[ i ][  6 ] = Texcoord8Vertex[ i ].u2 ;
			VertexDataTex8[ i ][  7 ] = Texcoord8Vertex[ i ].v2 ;
			VertexDataTex8[ i ][  8 ] = Texcoord8Vertex[ i ].u3 ;
			VertexDataTex8[ i ][  9 ] = Texcoord8Vertex[ i ].v3 ;
			VertexDataTex8[ i ][ 10 ] = Texcoord8Vertex[ i ].u4 ;
			VertexDataTex8[ i ][ 11 ] = Texcoord8Vertex[ i ].v4 ;
			VertexDataTex8[ i ][ 12 ] = Texcoord8Vertex[ i ].u5 ;
			VertexDataTex8[ i ][ 13 ] = Texcoord8Vertex[ i ].v5 ;
			VertexDataTex8[ i ][ 14 ] = Texcoord8Vertex[ i ].u6 ;
			VertexDataTex8[ i ][ 15 ] = Texcoord8Vertex[ i ].v6 ;
			VertexDataTex8[ i ][ 16 ] = Texcoord8Vertex[ i ].u7 ;
			VertexDataTex8[ i ][ 17 ] = Texcoord8Vertex[ i ].v7 ;
		}
	}

	Graphics_D3D11_CommonVertexBuffer_Setup( Texcoord8Vertex != NULL ? sizeof( VertexDataTex8 ) : sizeof( VertexData ) ) ;

//	DestBox.left   = 0 ;
//	DestBox.top    = 0 ;
//	DestBox.front  = 0 ;
//	DestBox.right  = sizeof( VertexData ) ;
//	DestBox.bottom = 1 ;
//	DestBox.back   = 1 ;
//	D3D11DeviceContext_UpdateSubresource_ASync( GD3D11.Device.DrawInfo.CommonVertexBuffer, 0, &DestBox, VertexData, 0, 0 ) ;
	D_D3D11_MAPPED_SUBRESOURCE MappedSubResource ;
	HRESULT                    hr ;
	hr = D3D11DeviceContext_Map_ASync( 
		GD3D11.Device.DrawInfo.CommonVertexBuffer,
		0,
		D_D3D11_MAP_WRITE_DISCARD,
		0,
		&MappedSubResource
	) ;
	if( Texcoord8Vertex != NULL )
	{
		_MEMCPY( MappedSubResource.pData, VertexDataTex8, sizeof( VertexDataTex8 ) ) ;
	}
	else
	{
		_MEMCPY( MappedSubResource.pData, VertexData, sizeof( VertexData ) ) ;
	}
	D3D11DeviceContext_Unmap_ASync(
		GD3D11.Device.DrawInfo.CommonVertexBuffer,
		0
	) ;

	// �����_�����O�^�[�Q�b�g�̃Z�b�g
	D3D11DeviceContext_OMSetRenderTargets( 1, &DestTextureRTV, NULL ) ;

	// �V�F�[�_�[���\�[�X�r���[���Z�b�g
	D3D11DeviceContext_PSSetShaderResources( 0, 1, &SrcTextureSRV ) ;
	if( BlendTextureSRV != NULL )
	{
		D3D11DeviceContext_PSSetShaderResources( 1, 1, &BlendTextureSRV ) ;
	}

	// �r���[�|�[�g�̃Z�b�g
	Viewport.TopLeftX = 0.0f ;
	Viewport.TopLeftY = 0.0f ;
	Viewport.Width    = ( float )DestDesc.Width ;
	Viewport.Height   = ( float )DestDesc.Height ;
	Viewport.MinDepth = 0.0f ;
	Viewport.MaxDepth = 1.0f ;
	D3D11DeviceContext_RSSetViewports( 1, &Viewport ) ;

	// �T���v���[�X�e�[�g���Z�b�g
	D3D11DeviceContext_PSSetSamplers( 0, 1, &GD3D11.Device.Shader.Base.StretchRect_SamplerState[ FilterType == D_D3D11_FILTER_TYPE_POINT ? 0 : 1 ] ) ;
	if( BlendTexture != NULL )
	{
		D3D11DeviceContext_PSSetSamplers( 1, 1, &GD3D11.Device.Shader.Base.StretchRect_SamplerState[ FilterType == D_D3D11_FILTER_TYPE_POINT ? 0 : 1 ] ) ;
	}

	// ���X�^���C�U�[�X�e�[�g���Z�b�g
	D3D11DeviceContext_RSSetState( GD3D11.Device.Shader.Base.StretchRect_RasterizerState ) ;

	// �[�x�X�e���V���X�e�[�g���Z�b�g
	D3D11DeviceContext_OMSetDepthStencilState( GD3D11.Device.Shader.Base.StretchRect_DepthStencilState, 0 ) ;

	// �u�����h�X�e�[�g���Z�b�g
	D3D11DeviceContext_OMSetBlendState( GD3D11.Device.Shader.Base.StretchRect_BlendState[ AlphaBlend ? 1 : 0 ], BlendFactor, 0xffffffff ) ;

	// �v���~�e�B�u�^�C�v���Z�b�g
	D3D11DeviceContext_IASetPrimitiveTopology( D_D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP ) ;

	// ���̓��C�A�E�g���Z�b�g
	D3D11DeviceContext_IASetInputLayout( Texcoord8Vertex != NULL ? GD3D11.Device.InputLayout.BaseStretchRectTex8InputLayout : GD3D11.Device.InputLayout.BaseStretchRectInputLayout ) ;

	// �s�N�Z���V�F�[�_�[���Z�b�g
	D3D11DeviceContext_PSSetShader( PixelShader  != NULL ? PixelShader  : GD3D11.Device.Shader.Base.StretchRect_PS, NULL, 0 ) ;

	// ���_�V�F�[�_�[���Z�b�g
	D3D11DeviceContext_VSSetShader( VertexShader != NULL ? VertexShader : ( Texcoord8Vertex != NULL ? GD3D11.Device.Shader.Base.StretchRectTex8_VS : GD3D11.Device.Shader.Base.StretchRect_VS ), NULL, 0 ) ;

	// ���_�o�b�t�@���Z�b�g
	VertexStride = Texcoord8Vertex != NULL ? sizeof( VertexDataTex8[ 0 ] ) : sizeof( VertexData[ 0 ] ) ;
	D3D11DeviceContext_IASetVertexBuffers( 0, 1, &GD3D11.Device.DrawInfo.CommonVertexBuffer, &VertexStride, &Offsets ) ;


	// �`��
	D3D11DeviceContext_Draw( 4, 0 ) ;


	// ���_�o�b�t�@�ݒ�𕜋A
	D3D11DeviceContext_IASetVertexBuffers( 0, 1, &GD3D11.Device.State.SetVertexBuffer, ( UINT * )&GD3D11.Device.State.SetVertexBufferStride, &Offsets ) ;

	// �Z�b�g���钸�_�V�F�[�_�[�𕜋A
	D3D11DeviceContext_VSSetShader( GD3D11.Device.State.SetVertexShader, NULL, 0 ) ;

	// �Z�b�g����s�N�Z���V�F�[�_�[�𕜋A
	D3D11DeviceContext_PSSetShader( GD3D11.Device.State.SetPixelShader, NULL, 0 ) ;

	// ���̓��C�A�E�g�ݒ�𕜋A
	D3D11DeviceContext_IASetInputLayout( GD3D11.Device.State.SetVertexInputLayout ) ;

	// �v���~�e�B�u�^�C�v�ݒ�̕��A
	D3D11DeviceContext_IASetPrimitiveTopology( GD3D11.Device.State.PrimitiveTopology ) ;

	// �Z�b�g����u�����h�X�e�[�g���A
	D3D11DeviceContext_OMSetBlendState( GD3D11.Device.State.BlendState, BlendFactor, 0xffffffff ) ;

	// �Z�b�g����[�x�X�e���V���X�e�[�g�𕜋A
	D3D11DeviceContext_OMSetDepthStencilState( GD3D11.Device.State.DepthStencilState, 0 ) ;

	// �Z�b�g���郉�X�^���C�U�[�X�e�[�g�𕜋A
	D3D11DeviceContext_RSSetState( GD3D11.Device.State.RasterizerState ) ;

	// �Z�b�g����T���v���[�X�e�[�g�𕜋A
	D3D11DeviceContext_PSSetSamplers( 0, 1, &GD3D11.Device.State.SamplerState[ 0 ] ) ;
	if( BlendTexture != NULL )
	{
		D3D11DeviceContext_PSSetSamplers( 1, 1, &GD3D11.Device.State.SamplerState[ 1 ] ) ;
	}

	// �Z�b�g���郌���_�����O�^�[�Q�b�g�̕��A
	Graphics_D3D11_DeviceState_SetRenderTarget_SizeCheck() ;

	// �r���[�|�[�g�ݒ�̕��A
	D3D11DeviceContext_RSSetViewports( 1, &GD3D11.Device.State.Viewport ) ;

	// �Z�b�g����V�F�[�_�[���\�[�X�r���[�𕜋A
	D3D11DeviceContext_PSSetShaderResources( 0, 1, &GD3D11.Device.State.PSSetShaderResourceView[ 0 ] ) ;
	if( BlendTextureSRV != NULL )
	{
		D3D11DeviceContext_PSSetShaderResources( 1, 1, &GD3D11.Device.State.PSSetShaderResourceView[ 1 ] ) ;
	}

	return 0 ;
}

// �w��̃e�N�X�`���̓��e�ɓǂݍ��݃A�N�Z�X�ł���悤�ɂ���
extern	int		Graphics_D3D11_Texture2D_Map(
	D_ID3D11Texture2D *				TargetTexture,
	D_ID3D11ShaderResourceView *	TargetTextureSRV,
	const RECT *					MapRect /* NULL �őS�� */,
	BASEIMAGE *						MapBaseImage,
	D_ID3D11Texture2D **			MapTempTexture
)
{
	D_D3D11_TEXTURE2D_DESC			TargetTexture2DDesc ;
	D_D3D11_TEXTURE2D_DESC			Texture2DDesc ;
//	D_D3D11_RENDER_TARGET_VIEW_DESC	RTVDesc ;
	D_D3D11_MAPPED_SUBRESOURCE		MappedSubresource ;
	D_ID3D11Texture2D				*DownSampleTexture   = NULL ;
//	D_ID3D11RenderTargetView		*DrawValidTextureRTV = NULL ;
	COLORDATA						*TargetColorData ;
	RECT							TempRect ;
	int								Width ;
	int								Height ;
	HRESULT							hr ;

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �����擾
	TargetTexture->GetDesc( &TargetTexture2DDesc ) ;
	TargetColorData = Graphics_D3D11_GetD3DFormatColorData( TargetTexture2DDesc.Format ) ;

	if( MapRect == NULL )
	{
		TempRect.left   = 0 ;
		TempRect.top    = 0 ;
		TempRect.right  = ( LONG )TargetTexture2DDesc.Width ;
		TempRect.bottom = ( LONG )TargetTexture2DDesc.Height ;
		MapRect         = &TempRect ;
	}
	Width  = MapRect->right  - MapRect->left ;
	Height = MapRect->bottom - MapRect->top ;

	*MapTempTexture = NULL ;

	// ���b�N�Ώۂ��}���`�T���v���Ή����A�~�b�v�}�b�v�t���̏ꍇ�͓��T�C�Y�̔�}���`�T���v���e�N�X�`���ɓ]��
	if( TargetTexture2DDesc.SampleDesc.Count != 1 ||
		TargetTexture2DDesc.MipLevels        != 1 )
	{
		_MEMSET( &Texture2DDesc, 0, sizeof( Texture2DDesc ) ) ;
		Texture2DDesc.Usage              = D_D3D11_USAGE_DEFAULT ;
		Texture2DDesc.Format             = TargetTexture2DDesc.Format ;
		Texture2DDesc.BindFlags          = 0 ;
		Texture2DDesc.Width              = TargetTexture2DDesc.Width ;
		Texture2DDesc.Height             = TargetTexture2DDesc.Height ;
		Texture2DDesc.CPUAccessFlags     = 0 ;
		Texture2DDesc.MipLevels          = 1 ;
		Texture2DDesc.ArraySize          = 1 ;
		Texture2DDesc.SampleDesc.Count   = 1 ;
		Texture2DDesc.SampleDesc.Quality = 0 ;
		hr = D3D11Device_CreateTexture2D_ASync( &Texture2DDesc, NULL, &DownSampleTexture ) ;
		if( FAILED( hr ) )
		{
			DXST_ERRORLOG_ADDUTF16LE( "\xc0\x30\xa6\x30\xf3\x30\xb5\x30\xf3\x30\xd7\x30\xea\x30\xf3\x30\xb0\x30\x28\x75\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�_�E���T���v�����O�p�e�N�X�`���̍쐬�Ɏ��s���܂���\n" @*/ ) ;
			goto ERR ;
		}

		// �]��
		D3D11DeviceContext_ResolveSubresource(
			DownSampleTexture,
			D_D3D11CalcSubresource( 0, 0, 1 ),
			TargetTexture,
			D_D3D11CalcSubresource( 0, 0, 1 ),
			TargetTexture2DDesc.Format
		) ;

//		_MEMSET( &RTVDesc, 0, sizeof( RTVDesc ) );
//		RTVDesc.Format        = TargetTexture2DDesc.Format ;
//		RTVDesc.ViewDimension = D_D3D11_RTV_DIMENSION_TEXTURE2D ;
//		hr = D3D11Device_CreateRenderTargetView( DrawValidTexture, &RTVDesc, &DrawValidTextureRTV ) ;
//		if( FAILED( hr ) )
//		{
//			DXST_ERRORLOGFMT_ADDUTF16LE(( "\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\xfc\x30\xde\x30\xc3\x30\xd7\x30\x28\x75\xcf\x63\x3b\x75\xef\x53\xfd\x80\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\x28\x75\xec\x30\xf3\x30\xc0\x30\xfc\x30\xbf\x30\xfc\x30\xb2\x30\xc3\x30\xc8\x30\xd3\x30\xe5\x30\xfc\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x00"/*@ L"�e�N�X�`���[�}�b�v�p�`��\�e�N�X�`���p�����_�[�^�[�Q�b�g�r���[�̍쐬�Ɏ��s���܂���" @*/ )) ;
//			goto ERR ;
//		}
//
//		// �]��
//		Graphics_D3D11_StretchRect(
//			TargetTexture,    TargetTextureSRV,    NULL,
//			DrawValidTexture, DrawValidTextureRTV, NULL,
//			D_D3D11_FILTER_TYPE_LINEAR
//		) ;
	}

	// �]���p�e�N�X�`���ɕ`��Ώۉ摜��]��
	{
		_MEMSET( &Texture2DDesc, 0, sizeof( Texture2DDesc ) ) ;
		Texture2DDesc.Usage              = D_D3D11_USAGE_STAGING ;
		Texture2DDesc.Format             = TargetTexture2DDesc.Format ;
		Texture2DDesc.BindFlags          = 0 ;
		Texture2DDesc.Width              = TargetTexture2DDesc.Width ;
		Texture2DDesc.Height             = TargetTexture2DDesc.Height ;
		Texture2DDesc.CPUAccessFlags     = D_D3D11_CPU_ACCESS_READ ;
		Texture2DDesc.MipLevels          = 1 ;
		Texture2DDesc.ArraySize          = 1 ;
		Texture2DDesc.SampleDesc.Count   = 1 ;
		Texture2DDesc.SampleDesc.Quality = 0 ;
		hr = D3D11Device_CreateTexture2D_ASync( &Texture2DDesc, NULL, MapTempTexture ) ;
		if( FAILED( hr ) )
		{
			DXST_ERRORLOG_ADDUTF16LE( "\xcf\x63\x3b\x75\xfe\x5b\x61\x8c\xd0\x30\xc3\x30\xd5\x30\xa1\x30\xad\x8a\x7f\x30\xd6\x53\x8a\x30\x28\x75\x6e\x30\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�`��Ώۃo�b�t�@�ǂݎ��p�̃e�N�X�`���̍쐬�Ɏ��s���܂���\n" @*/ ) ;
			goto ERR ;
		}

		D3D11DeviceContext_CopyResource( *MapTempTexture, DownSampleTexture ? DownSampleTexture : TargetTexture ) ;

		// �}�b�v
		hr = D3D11DeviceContext_Map( *MapTempTexture, D_D3D11CalcSubresource( 0, 0, 1 ), D_D3D11_MAP_READ, 0, &MappedSubresource ) ;
		if( FAILED( hr ) )
		{
			DXST_ERRORLOG_ADDUTF16LE( "\xcf\x63\x3b\x75\xfe\x5b\x61\x8c\xd0\x30\xc3\x30\xd5\x30\xa1\x30\xad\x8a\x7f\x30\xd6\x53\x8a\x30\x28\x75\x6e\x30\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\x6e\x30\xde\x30\xc3\x30\xd4\x30\xf3\x30\xb0\x30\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�`��Ώۃo�b�t�@�ǂݎ��p�̃e�N�X�`���̃}�b�s���O�Ɏ��s���܂���\n" @*/ ) ;
			goto ERR ;
		}
	}

//	// �`��p�e�N�X�`�����g�p�����ꍇ�͂����ŉ��
//	if( DrawValidTextureRTV != NULL )
//	{
//		Direct3D11_Release_RenderTargetView( DrawValidTextureRTV ) ;
//		DrawValidTextureRTV = NULL ;
//	}

	// �_�E���T���v�����O�p�e�N�X�`�����g�p�����ꍇ�͂����ŉ��
	if( DownSampleTexture != NULL )
	{
		Direct3D11_Release_Texture2D( DownSampleTexture ) ;
		DownSampleTexture = NULL ;
	}

	// ��{�C���[�W�\���̂̃f�[�^�ɃZ�b�g
	MapBaseImage->ColorData      = *TargetColorData ;
	MapBaseImage->Width          = Width ;
	MapBaseImage->Height         = Height ;
	MapBaseImage->Pitch          = ( int )MappedSubresource.RowPitch ;
	MapBaseImage->GraphData      = ( BYTE * )MappedSubresource.pData + MapRect->left * TargetColorData->PixelByte + MapRect->top * MappedSubresource.RowPitch ;
	MapBaseImage->MipMapCount    = 0 ;
	MapBaseImage->GraphDataCount = 0 ;

	// ����I��
	return 0 ;

	// �G���[����
ERR :

	if( *MapTempTexture != NULL )
	{
		Direct3D11_Release_Texture2D( *MapTempTexture ) ;
		*MapTempTexture = NULL ;
	}

//	if( DrawValidTextureRTV != NULL )
//	{
//		Direct3D11_Release_RenderTargetView( DrawValidTextureRTV ) ;
//		DrawValidTextureRTV = NULL ;
//	}

	if( DownSampleTexture != NULL )
	{
		Direct3D11_Release_Texture2D( DownSampleTexture ) ;
		DownSampleTexture = NULL ;
	}

	// �G���[�I��
	return -1 ;
}

// �w��̃e�N�X�`���̓��e�ւ̓ǂݍ��݃A�N�Z�X���I������
extern	int		Graphics_D3D11_Texture2D_Unmap( D_ID3D11Texture2D *MapTempTexture )
{
	if( MapTempTexture != NULL )
	{
		Direct3D11_Release_Texture2D( MapTempTexture ) ;
		MapTempTexture = NULL ;
	}

	// �I��
	return 0 ;
}













































// D3D11Device �̏������֌W

// D3D11Device �I�u�W�F�N�g�̍쐬( 0:����  -1:���s )
extern	int		Graphics_D3D11_Device_Create( void )
{
	const char *String = NULL ;

	// ���ɍ쐬�ς݂̏ꍇ�͈�x�폜����
	D3D11Device_Release() ;
	GD3D11.Device.DrawInfo.BeginSceneFlag = FALSE ;

	// DXGIFactory �̍쐬
	if( DXGIFactory_Create() < 0 )
	{
		return -1 ;
	}

	// �A�_�v�^�[���̗�
	if( EnumDXGIAdapterInfo() < 0 )
	{
		return -1 ;
	}

	// �f�o�C�X�̍쐬
	if( D3D11_CreateDevice() < 0 )
	{
		return -1 ;
	}

	// ���C���X���b�v�`�F�C���̍쐬
	if( Graphics_D3D11_OutputWindow_Add( GetDisplayWindowHandle(), TRUE ) < 0 )
	{
		return -1 ;
	}
	GD3D11.Device.Screen.TargetOutputWindow = 0 ;

	// �t���X�N���[�����[�h�̏ꍇ�̓f�X�N�g�b�v��̈ʒu���擾���Ă���
	if( NS_GetWindowModeFlag() == FALSE )
	{
		GSYS.Screen.FullScreenDesktopRect = GD3D11.Adapter.Info[ 0 ].OutputDesc[ GSYS.Screen.ValidUseDisplayIndex ? GSYS.Screen.UseDisplayIndex : 0 ].DesktopCoordinates ;
	}

	// ���Z�u�����h���\���ǂ������Z�b�g����
	GSYS.HardInfo.ValidSubBlend = TRUE ;

	// �e�N�X�`���������`�ł���K�v�����邩�ǂ������擾����
	GSYS.HardInfo.TextureSquareOnly = FALSE ;

	// ��x�Ɏg�p���邱�Ƃ̏o����ő咸�_�����擾����
	GSYS.HardInfo.MaxVertexIndex = 0xffff ;

	// ���_���W�^�C�v�� DX_DRAWFLOATCOORDTYPE_DIRECT3D10
	GSYS.HardInfo.DrawFloatCoordType = DX_DRAWFLOATCOORDTYPE_DIRECT3D10 ;

	switch( GD3D11.Setting.FeatureLevel )
	{
	case D_D3D_FEATURE_LEVEL_11_0 :
	case D_D3D_FEATURE_LEVEL_11_1 :
		// �����Ƀ����_�����O�ł���^�[�Q�b�g�̐���ۑ�
		GSYS.HardInfo.RenderTargetNum = 8 ;

		// �e�N�X�`���̍ő啝�ƍő卂����ۑ�����
		GSYS.HardInfo.MaxTextureWidth  = 16384 ;
		GSYS.HardInfo.MaxTextureHeight = GSYS.HardInfo.MaxTextureWidth ;

		// �e�N�X�`���̃T�C�Y���Q�̂���ł���K�v�͖���
		GSYS.HardInfo.TextureSizePow2 = FALSE ;

		// �����t�Ńe�N�X�`���̃T�C�Y���Q�̂���ł͂Ȃ��Ă����v
		GSYS.HardInfo.TextureSizeNonPow2Conditional = TRUE ;

		// ���_�J���[��BGRA�J���[�^�C�v���g�p����
		GSYS.HardInfo.UseVertexColorBGRAFormat = TRUE ;

		// ��x�ɕ`��ł���ő�v���~�e�B�u�����擾����
		GSYS.HardInfo.MaxPrimitiveCount = 0x7fffffff ;
		break ;

	case D_D3D_FEATURE_LEVEL_10_0 :
	case D_D3D_FEATURE_LEVEL_10_1 :
		// �����Ƀ����_�����O�ł���^�[�Q�b�g�̐���ۑ�
		GSYS.HardInfo.RenderTargetNum = 8 ;

		// �e�N�X�`���̍ő啝�ƍő卂����ۑ�����
		GSYS.HardInfo.MaxTextureWidth  = 8192 ;
		GSYS.HardInfo.MaxTextureHeight = GSYS.HardInfo.MaxTextureWidth ;

		// �e�N�X�`���̃T�C�Y���Q�̂���ł���K�v�͖���
		GSYS.HardInfo.TextureSizePow2 = FALSE ;

		// �����t�Ńe�N�X�`���̃T�C�Y���Q�̂���ł͂Ȃ��Ă����v
		GSYS.HardInfo.TextureSizeNonPow2Conditional = TRUE ;

		// ���_�J���[��BGRA�J���[�^�C�v���g�p����
		GSYS.HardInfo.UseVertexColorBGRAFormat = TRUE ;

		// ��x�ɕ`��ł���ő�v���~�e�B�u�����擾����
		GSYS.HardInfo.MaxPrimitiveCount = 0x7fffffff ;
		break ;

	case D_D3D_FEATURE_LEVEL_9_1 :
	case D_D3D_FEATURE_LEVEL_9_2 :
	case D_D3D_FEATURE_LEVEL_9_3 :
		// �����Ƀ����_�����O�ł���^�[�Q�b�g�̐���ۑ�
		GSYS.HardInfo.RenderTargetNum = GD3D11.Setting.FeatureLevel == D_D3D_FEATURE_LEVEL_9_3 ? 4 : 1 ;

		// �e�N�X�`���̍ő啝�ƍő卂����ۑ�����
		GSYS.HardInfo.MaxTextureWidth  = GD3D11.Setting.FeatureLevel == D_D3D_FEATURE_LEVEL_9_3 ? 4096 : 2048 ;
		GSYS.HardInfo.MaxTextureHeight = GSYS.HardInfo.MaxTextureWidth ;

		// �����t�Ńe�N�X�`���̃T�C�Y���Q�̂���ł͂Ȃ��Ă����v
		GSYS.HardInfo.TextureSizeNonPow2Conditional = TRUE ;

		// ���_�J���[��BGRA�J���[�^�C�v���g�p����
		GSYS.HardInfo.UseVertexColorBGRAFormat = TRUE ;

		// �e�N�X�`���̃T�C�Y���Q�̂���ł���K�v������
//		GSYS.HardInfo.TextureSizePow2 = TRUE ;

		// �����t�Ńe�N�X�`���̃T�C�Y���Q�̂���ł͂Ȃ��Ă����v�ł͂Ȃ�
//		GSYS.HardInfo.TextureSizeNonPow2Conditional = FALSE ;

		// �s�N�Z���P�ʃ��C�e�B���O�͖���
		GD3D11.UsePixelLightingShader = FALSE ;

		// ���_�J���[��BGRA�J���[�^�C�v���g�p���Ȃ�
		GSYS.HardInfo.UseVertexColorBGRAFormat = FALSE ;

		// ��x�ɕ`��ł���ő�v���~�e�B�u�����擾����
		GSYS.HardInfo.MaxPrimitiveCount = GD3D11.Setting.FeatureLevel == D_D3D_FEATURE_LEVEL_9_1 ? 65535 : 1048575 ;
		break ;
	}

	// �T�C�Y�̏������ق����ő�T�C�Y�Ƃ���
	GSYS.HardInfo.MaxTextureSize = GSYS.HardInfo.MaxTextureWidth < GSYS.HardInfo.MaxTextureHeight ? GSYS.HardInfo.MaxTextureWidth : GSYS.HardInfo.MaxTextureHeight ;

	// �����_�[�^�[�Q�b�g�̐��𒲐�
	if( GSYS.HardInfo.RenderTargetNum > DX_RENDERTARGET_COUNT )
	{
		GSYS.HardInfo.RenderTargetNum = DX_RENDERTARGET_COUNT ;
	}

	// Graphics_D3D11_DrawPreparation ���s���ׂ��t���O�𗧂Ă�
	GD3D11.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

	// �h���C�o���o��
	const WCHAR *Description = GD3D11.Adapter.Info[ 0 ].Desc.Description ;
	DXST_ERRORLOGFMT_ADDW(( L"Graphics Device:%s", Description )) ;

	// �e��t�H�[�}�b�g�𒲂ׂ�
	{
		UINT FormatSupport ;

		// ��ʂ̃t�H�[�}�b�g�̎擾
		GD3D11.Device.Caps.ScreenFormat = DIRECT3D11_BACKBUFFER_FORMAT ;
		String = "DXGI_FORMAT_R8G8B8A8_UNORM" ;
		DXST_ERRORLOGFMT_ADDA(( "\x89\xe6\x96\xca\x82\xcc\x83\x74\x83\x48\x81\x5b\x83\x7d\x83\x62\x83\x67\x82\xcd %s \x82\xc5\x82\xb7"/*@ "��ʂ̃t�H�[�}�b�g�� %s �ł�" @*/, String )) ;

		// 16�r�b�g�[�x�o�b�t�@�t�H�[�}�b�g�̐ݒ�
		GD3D11.Device.Caps.DepthBufferTexture2DFormat[ ZBUFFER_FORMAT_16BIT ]    = DIRECT3D11_DEPTHBUFFER_TEXTURE2D_FORMAT ;
		GD3D11.Device.Caps.DepthBufferDepthStencilFormat[ ZBUFFER_FORMAT_16BIT ] = DIRECT3D11_DEPTHBUFFER_DEPTHSTENCIL_FORMAT ;
		String = "DXGI_FORMAT_D16_UNORM" ;
		DXST_ERRORLOGFMT_ADDA(( "16bit Z\x83\x6f\x83\x62\x83\x74\x83\x40\x83\x74\x83\x48\x81\x5b\x83\x7d\x83\x62\x83\x67\x82\xcd %s \x82\xc5\x82\xb7"/*@ "16bit Z�o�b�t�@�t�H�[�}�b�g�� %s �ł�" @*/, String )) ;

		// 24�r�b�g�[�x�o�b�t�@�t�H�[�}�b�g�̑I��
		if( /*D3D11Device_CheckFormatSupport( D_DXGI_FORMAT_R24G8_TYPELESS, &FormatSupport ) == S_OK &&
			( FormatSupport & D_D3D11_FORMAT_SUPPORT_RENDER_TARGET ) != 0 &&*/
			D3D11Device_CheckFormatSupport( D_DXGI_FORMAT_D24_UNORM_S8_UINT, &FormatSupport ) == S_OK &&
			( FormatSupport & D_D3D11_FORMAT_SUPPORT_DEPTH_STENCIL ) != 0 )
		{
			GD3D11.Device.Caps.DepthBufferTexture2DFormat[ ZBUFFER_FORMAT_24BIT ]    = D_DXGI_FORMAT_R24G8_TYPELESS ;
			GD3D11.Device.Caps.DepthBufferDepthStencilFormat[ ZBUFFER_FORMAT_24BIT ] = D_DXGI_FORMAT_D24_UNORM_S8_UINT ;
			String = "DXGI_FORMAT_D24_UNORM_S8_UINT" ;
		}
		else
		{
			GD3D11.Device.Caps.DepthBufferTexture2DFormat[ ZBUFFER_FORMAT_24BIT ]    = GD3D11.Device.Caps.DepthBufferTexture2DFormat[ ZBUFFER_FORMAT_16BIT ] ;
			GD3D11.Device.Caps.DepthBufferDepthStencilFormat[ ZBUFFER_FORMAT_24BIT ] = GD3D11.Device.Caps.DepthBufferDepthStencilFormat[ ZBUFFER_FORMAT_16BIT ] ;
		}
		DXST_ERRORLOGFMT_ADDA(( "24bit Z\x83\x6f\x83\x62\x83\x74\x83\x40\x83\x74\x83\x48\x81\x5b\x83\x7d\x83\x62\x83\x67\x82\xcd %s \x82\xc5\x82\xb7"/*@ "24bit Z�o�b�t�@�t�H�[�}�b�g�� %s �ł�" @*/, String )) ;

		// 32�r�b�g�[�x�o�b�t�@�t�H�[�}�b�g�̐ݒ�
		if( /*D3D11Device_CheckFormatSupport( D_DXGI_FORMAT_R32_TYPELESS, &FormatSupport ) == S_OK &&
			( FormatSupport & D_D3D11_FORMAT_SUPPORT_RENDER_TARGET ) != 0 &&*/
			D3D11Device_CheckFormatSupport( D_DXGI_FORMAT_D32_FLOAT, &FormatSupport ) == S_OK &&
			( FormatSupport & D_D3D11_FORMAT_SUPPORT_DEPTH_STENCIL ) != 0 )
		{
			GD3D11.Device.Caps.DepthBufferTexture2DFormat[ ZBUFFER_FORMAT_32BIT ]    = D_DXGI_FORMAT_R32_TYPELESS ;
			GD3D11.Device.Caps.DepthBufferDepthStencilFormat[ ZBUFFER_FORMAT_32BIT ] = D_DXGI_FORMAT_D32_FLOAT ;
			String = "DXGI_FORMAT_D32_FLOAT" ;
		}
		else
		{
			GD3D11.Device.Caps.DepthBufferTexture2DFormat[ ZBUFFER_FORMAT_32BIT ]    = GD3D11.Device.Caps.DepthBufferTexture2DFormat[ ZBUFFER_FORMAT_24BIT ] ;
			GD3D11.Device.Caps.DepthBufferDepthStencilFormat[ ZBUFFER_FORMAT_32BIT ] = GD3D11.Device.Caps.DepthBufferDepthStencilFormat[ ZBUFFER_FORMAT_24BIT ] ;
		}
		DXST_ERRORLOGFMT_ADDA(( "32bit Z\x83\x6f\x83\x62\x83\x74\x83\x40\x83\x74\x83\x48\x81\x5b\x83\x7d\x83\x62\x83\x67\x82\xcd %s \x82\xc5\x82\xb7"/*@ "32bit Z�o�b�t�@�t�H�[�}�b�g�� %s �ł�" @*/, String )) ;


		// �J���[�t�H�[�}�b�g�̃`�F�b�N
		if( D3D11Device_CheckFormatSupport( D_DXGI_FORMAT_R8G8B8A8_UNORM, &FormatSupport ) != S_OK ||
			( FormatSupport & D_D3D11_FORMAT_SUPPORT_TEXTURE2D ) == 0 )
		{
			DXST_ERRORLOG_ADDA( "\x8e\x67\x97\x70\x82\xc5\x82\xab\x82\xe9\x83\x4a\x83\x89\x81\x5b\x83\x74\x83\x48\x81\x5b\x83\x7d\x83\x62\x83\x67\x82\xaa\x8c\xa9\x82\xc2\x82\xa9\x82\xe8\x82\xdc\x82\xb9\x82\xf1\x82\xc5\x82\xb5\x82\xbd\n"/*@ "�g�p�ł���J���[�t�H�[�}�b�g��������܂���ł���\n" @*/ ) ;
			return -1 ;
		}
		GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_RGB16           ] = D_DXGI_FORMAT_R8G8B8A8_UNORM ;
		GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_RGB32           ] = D_DXGI_FORMAT_R8G8B8A8_UNORM ;
		GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ALPHA_RGB16     ] = D_DXGI_FORMAT_R8G8B8A8_UNORM ;
		GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ALPHA_RGB32     ] = D_DXGI_FORMAT_R8G8B8A8_UNORM ;
		GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ALPHATEST_RGB16 ] = D_DXGI_FORMAT_R8G8B8A8_UNORM ;
		GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ALPHATEST_RGB32 ] = D_DXGI_FORMAT_R8G8B8A8_UNORM ;
		String = "DXGI_FORMAT_R8G8B8A8_UNORM" ;
		DXST_ERRORLOGFMT_ADDA(( "\x83\x4a\x83\x89\x81\x5b\x83\x74\x83\x48\x81\x5b\x83\x7d\x83\x62\x83\x67\x82\xcd %s \x82\xc5\x82\xb7"/*@ "�J���[�t�H�[�}�b�g�� %s �ł�" @*/, String )) ;

		// �p���b�g�J���[�t�H�[�}�b�g�̃Z�b�g
		GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_PAL4           ] = D_DXGI_FORMAT_R8G8B8A8_UNORM ;
		GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_PAL8           ] = D_DXGI_FORMAT_R8G8B8A8_UNORM ;
		GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ALPHA_PAL4     ] = D_DXGI_FORMAT_R8G8B8A8_UNORM ;
		GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ALPHA_PAL8     ] = D_DXGI_FORMAT_R8G8B8A8_UNORM ;
		GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ALPHATEST_PAL4 ] = D_DXGI_FORMAT_R8G8B8A8_UNORM ;
		GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ALPHATEST_PAL8 ] = D_DXGI_FORMAT_R8G8B8A8_UNORM ;

		// DXT1�t�H�[�}�b�g�̃`�F�b�N
		if( D3D11Device_CheckFormatSupport( D_DXGI_FORMAT_BC1_UNORM, &FormatSupport ) == S_OK &&
			( FormatSupport & D_D3D11_FORMAT_SUPPORT_TEXTURE2D ) != 0 )
		{
			GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DXT1 ] = D_DXGI_FORMAT_BC1_UNORM ;
			String = " DXGI_FORMAT_BC1_UNORM \x82\xc5\x82\xb7"/*@ " DXGI_FORMAT_BC1_UNORM �ł�" @*/ ;
		}
		else
		{
			GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DXT1 ] = GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ALPHATEST_RGB16 ] ;
			String = "\x8e\x67\x82\xa6\x82\xdc\x82\xb9\x82\xf1"/*@ "�g���܂���" @*/ ;
		}
		DXST_ERRORLOGFMT_ADDA(( "\x82\x63\x82\x77\x82\x73\x82\x50\x83\x65\x83\x4e\x83\x58\x83\x60\x83\x83\x83\x74\x83\x48\x81\x5b\x83\x7d\x83\x62\x83\x67\x82\xcd%s"/*@ "�c�w�s�P�e�N�X�`���t�H�[�}�b�g��%s" @*/, String )) ;

		// DXT2�t�H�[�}�b�g�͎g�p�ł��Ȃ�
		GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DXT2 ] = GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ALPHA_RGB32 ] ;
		DXST_ERRORLOGFMT_ADDA(( "\x82\x63\x82\x77\x82\x73\x82\x51\x83\x65\x83\x4e\x83\x58\x83\x60\x83\x83\x83\x74\x83\x48\x81\x5b\x83\x7d\x83\x62\x83\x67\x82\xcd\x8e\x67\x82\xa6\x82\xdc\x82\xb9\x82\xf1"/*@ "�c�w�s�Q�e�N�X�`���t�H�[�}�b�g�͎g���܂���" @*/ )) ;

		// DXT3�t�H�[�}�b�g�̃`�F�b�N
		if( D3D11Device_CheckFormatSupport( D_DXGI_FORMAT_BC2_UNORM, &FormatSupport ) == S_OK &&
			( FormatSupport & D_D3D11_FORMAT_SUPPORT_TEXTURE2D ) != 0 )
		{
			GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DXT3 ] = D_DXGI_FORMAT_BC2_UNORM ;
			String = " DXGI_FORMAT_BC2_UNORM \x82\xc5\x82\xb7"/*@ " DXGI_FORMAT_BC2_UNORM �ł�" @*/ ;
		}
		else
		{
			GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DXT3 ] = GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ALPHA_RGB32 ] ;
			String = "\x8e\x67\x82\xa6\x82\xdc\x82\xb9\x82\xf1"/*@ "�g���܂���" @*/ ;
		}
		DXST_ERRORLOGFMT_ADDA(( "\x82\x63\x82\x77\x82\x73\x82\x52\x83\x65\x83\x4e\x83\x58\x83\x60\x83\x83\x83\x74\x83\x48\x81\x5b\x83\x7d\x83\x62\x83\x67\x82\xcd%s"/*@ "�c�w�s�R�e�N�X�`���t�H�[�}�b�g��%s" @*/, String )) ;

		// DXT4�t�H�[�}�b�g�͎g�p�ł��Ȃ�
		GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DXT4 ] = GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ALPHA_RGB32 ] ;
		DXST_ERRORLOGFMT_ADDA(( "\x82\x63\x82\x77\x82\x73\x82\x53\x83\x65\x83\x4e\x83\x58\x83\x60\x83\x83\x83\x74\x83\x48\x81\x5b\x83\x7d\x83\x62\x83\x67\x82\xcd\x8e\x67\x82\xa6\x82\xdc\x82\xb9\x82\xf1"/*@ "�c�w�s�S�e�N�X�`���t�H�[�}�b�g�͎g���܂���" @*/ )) ;

		// DXT5�t�H�[�}�b�g�̃`�F�b�N
		if( D3D11Device_CheckFormatSupport( D_DXGI_FORMAT_BC3_UNORM, &FormatSupport ) == S_OK &&
			( FormatSupport & D_D3D11_FORMAT_SUPPORT_TEXTURE2D ) != 0 )
		{
			GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DXT5 ] = D_DXGI_FORMAT_BC3_UNORM ;
			String = " DXGI_FORMAT_BC3_UNORM \x82\xc5\x82\xb7"/*@ " DXGI_FORMAT_BC3_UNORM �ł�" @*/ ;
		}
		else
		{
			GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DXT5 ] = GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ALPHA_RGB32 ] ;
			String = "\x8e\x67\x82\xa6\x82\xdc\x82\xb9\x82\xf1"/*@ "�g���܂���" @*/ ;
		}
		DXST_ERRORLOGFMT_ADDA(( "\x82\x63\x82\x77\x82\x73\x82\x54\x83\x65\x83\x4e\x83\x58\x83\x60\x83\x83\x83\x74\x83\x48\x81\x5b\x83\x7d\x83\x62\x83\x67\x82\xcd%s"/*@ "�c�w�s�T�e�N�X�`���t�H�[�}�b�g��%s" @*/, String )) ;





		// ARGB����16�r�b�g�^�J���[�t�H�[�}�b�g�̑I��
		if( D3D11Device_CheckFormatSupport( D_DXGI_FORMAT_R16G16B16A16_UNORM, &FormatSupport ) == S_OK &&
			( FormatSupport & D_D3D11_FORMAT_SUPPORT_TEXTURE2D     ) != 0 )
		{
			GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ABGR_I16 ] = D_DXGI_FORMAT_R16G16B16A16_UNORM ;
			String = "DXGI_FORMAT_R16G16B16A16_UNORM" ;
		}
		else
		{
			DXST_ERRORLOG_ADDA( "\x8e\x67\x97\x70\x82\xc5\x82\xab\x82\xe9\x20\x41\x42\x47\x52\x20\x90\xae\x90\x94\x20\x31\x36\x20\x83\x72\x83\x62\x83\x67\x8c\x5e\x83\x4a\x83\x89\x81\x5b\x83\x74\x83\x48\x81\x5b\x83\x7d\x83\x62\x83\x67\x82\xaa\x82\xa0\x82\xe8\x82\xdc\x82\xb9\x82\xf1\x82\xc5\x82\xb5\x82\xbd\x5c\x6e"/*@ "�g�p�ł��� ABGR ���� 16 �r�b�g�^�J���[�t�H�[�}�b�g������܂���ł���\n" @*/ ) ;
			GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ABGR_I16 ] = D_DXGI_FORMAT_UNKNOWN ;
		}

		if( GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ABGR_I16 ] != D_DXGI_FORMAT_UNKNOWN )
		{
			DXST_ERRORLOGFMT_ADDA(( "\x41\x42\x47\x52\x20\x90\xae\x90\x94\x20\x31\x36\x20\x83\x72\x83\x62\x83\x67\x8c\x5e\x83\x4a\x83\x89\x81\x5b\x83\x74\x83\x48\x81\x5b\x83\x7d\x83\x62\x83\x67\x82\xcd\x20\x25\x73\x20\x82\xc5\x82\xb7"/*@ "ABGR ���� 16 �r�b�g�^�J���[�t�H�[�}�b�g�� %s �ł�" @*/, String )) ;
		}

		// ARGB���������_16�r�b�g�^�J���[�t�H�[�}�b�g�̑I��
		if( D3D11Device_CheckFormatSupport( D_DXGI_FORMAT_R16G16B16A16_FLOAT, &FormatSupport ) == S_OK &&
			( FormatSupport & D_D3D11_FORMAT_SUPPORT_TEXTURE2D     ) != 0 )
		{
			GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ABGR_F16 ] = D_DXGI_FORMAT_R16G16B16A16_FLOAT ;
			String = "DXGI_FORMAT_R16G16B16A16_FLOAT" ;
		}
		else
		{
			DXST_ERRORLOG_ADDA( "\x8e\x67\x97\x70\x82\xc5\x82\xab\x82\xe9\x41\x42\x47\x52\x20\x95\x82\x93\xae\x8f\xac\x90\x94\x93\x5f\x20\x31\x36\x20\x83\x72\x83\x62\x83\x67\x8c\x5e\x83\x4a\x83\x89\x81\x5b\x83\x74\x83\x48\x81\x5b\x83\x7d\x83\x62\x83\x67\x82\xaa\x82\xa0\x82\xe8\x82\xdc\x82\xb9\x82\xf1\x82\xc5\x82\xb5\x82\xbd\x5c\x6e"/*@ "�g�p�ł���ABGR ���������_ 16 �r�b�g�^�J���[�t�H�[�}�b�g������܂���ł���\n" @*/ ) ;
			GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ABGR_F16 ] = D_DXGI_FORMAT_UNKNOWN ;
		}

		if( GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ABGR_F16 ] != D_DXGI_FORMAT_UNKNOWN )
		{
			DXST_ERRORLOGFMT_ADDA(( "\x41\x42\x47\x52\x20\x95\x82\x93\xae\x8f\xac\x90\x94\x93\x5f\x20\x31\x36\x20\x83\x72\x83\x62\x83\x67\x8c\x5e\x83\x4a\x83\x89\x81\x5b\x83\x74\x83\x48\x81\x5b\x83\x7d\x83\x62\x83\x67\x82\xcd\x20\x25\x73\x20\x82\xc5\x82\xb7"/*@ "ABGR ���������_ 16 �r�b�g�^�J���[�t�H�[�}�b�g�� %s �ł�" @*/, String )) ;
		}

		// ARGB���������_32�r�b�g�^�J���[�t�H�[�}�b�g�̑I��
		if( D3D11Device_CheckFormatSupport( D_DXGI_FORMAT_R32G32B32A32_FLOAT, &FormatSupport ) == S_OK &&
			( FormatSupport & D_D3D11_FORMAT_SUPPORT_TEXTURE2D     ) != 0 )
		{
			GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ABGR_F32 ] = D_DXGI_FORMAT_R32G32B32A32_FLOAT ;
			String = "DXGI_FORMAT_R32G32B32A32_FLOAT" ;
		}
		else
		if( D3D11Device_CheckFormatSupport( D_DXGI_FORMAT_R16G16B16A16_FLOAT, &FormatSupport ) == S_OK &&
			( FormatSupport & D_D3D11_FORMAT_SUPPORT_TEXTURE2D     ) != 0 )
		{
			GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ABGR_F32 ] = D_DXGI_FORMAT_R16G16B16A16_FLOAT ;
			String = "DXGI_FORMAT_R16G16B16A16_FLOAT" ;
		}
		else
		{
			DXST_ERRORLOG_ADDA( "\x8e\x67\x97\x70\x82\xc5\x82\xab\x82\xe9\x41\x42\x47\x52\x20\x95\x82\x93\xae\x8f\xac\x90\x94\x93\x5f\x20\x33\x32\x20\x83\x72\x83\x62\x83\x67\x8c\x5e\x83\x4a\x83\x89\x81\x5b\x83\x74\x83\x48\x81\x5b\x83\x7d\x83\x62\x83\x67\x82\xaa\x82\xa0\x82\xe8\x82\xdc\x82\xb9\x82\xf1\x82\xc5\x82\xb5\x82\xbd\x5c\x6e"/*@ "�g�p�ł���ABGR ���������_ 32 �r�b�g�^�J���[�t�H�[�}�b�g������܂���ł���\n" @*/ ) ;
			GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ABGR_F32 ] = D_DXGI_FORMAT_UNKNOWN ;
		}

		if( GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ABGR_F32 ] != D_DXGI_FORMAT_UNKNOWN )
		{
			DXST_ERRORLOGFMT_ADDA(( "\x41\x42\x47\x52\x20\x95\x82\x93\xae\x8f\xac\x90\x94\x93\x5f\x20\x33\x32\x20\x83\x72\x83\x62\x83\x67\x8c\x5e\x83\x4a\x83\x89\x81\x5b\x83\x74\x83\x48\x81\x5b\x83\x7d\x83\x62\x83\x67\x82\xcd\x20\x25\x73\x20\x82\xc5\x82\xb7"/*@ "ABGR ���������_ 32 �r�b�g�^�J���[�t�H�[�}�b�g�� %s �ł�" @*/, String )) ;
		}

		// �P�`�����l������8�r�b�g�^�J���[�t�H�[�}�b�g�̑I��
		if( D3D11Device_CheckFormatSupport( D_DXGI_FORMAT_R8_UNORM, &FormatSupport ) == S_OK &&
			( FormatSupport & D_D3D11_FORMAT_SUPPORT_TEXTURE2D     ) != 0 )
		{
			GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ONE_I8 ] = D_DXGI_FORMAT_R8_UNORM ;
			String = "DXGI_FORMAT_R8_UNORM" ;
		}
		else
		if( D3D11Device_CheckFormatSupport( D_DXGI_FORMAT_R8G8_UNORM, &FormatSupport ) == S_OK &&
			( FormatSupport & D_D3D11_FORMAT_SUPPORT_TEXTURE2D     ) != 0 )
		{
			GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ONE_I8 ] = D_DXGI_FORMAT_R8G8_UNORM ;
			String = "DXGI_FORMAT_R8G8_UNORM" ;
		}
		else
		if( D3D11Device_CheckFormatSupport( D_DXGI_FORMAT_R8G8B8A8_UNORM, &FormatSupport ) == S_OK &&
			( FormatSupport & D_D3D11_FORMAT_SUPPORT_TEXTURE2D     ) != 0 )
		{
			GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ONE_I8 ] = D_DXGI_FORMAT_R8G8B8A8_UNORM ;
			String = "DXGI_FORMAT_R8G8B8A8_UNORM" ;
		}
		else
		{
			DXST_ERRORLOG_ADDA( "\x8e\x67\x97\x70\x82\xc5\x82\xab\x82\xe9\x82\x50\x83\x60\x83\x83\x83\x93\x83\x6c\x83\x8b\x90\xae\x90\x94\x20\x38\x20\x83\x72\x83\x62\x83\x67\x8c\x5e\x83\x4a\x83\x89\x81\x5b\x83\x74\x83\x48\x81\x5b\x83\x7d\x83\x62\x83\x67\x82\xaa\x82\xa0\x82\xe8\x82\xdc\x82\xb9\x82\xf1\x82\xc5\x82\xb5\x82\xbd\x5c\x6e"/*@ "�g�p�ł���P�`�����l������ 8 �r�b�g�^�J���[�t�H�[�}�b�g������܂���ł���\n" @*/ ) ;
			GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ONE_I8 ] = D_DXGI_FORMAT_UNKNOWN ;
		}

		if( GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ONE_I8 ] != D_DXGI_FORMAT_UNKNOWN )
		{
			DXST_ERRORLOGFMT_ADDA(( "\x82\x50\x83\x60\x83\x83\x83\x93\x83\x6c\x83\x8b\x90\xae\x90\x94\x20\x38\x20\x83\x72\x83\x62\x83\x67\x8c\x5e\x83\x4a\x83\x89\x81\x5b\x83\x74\x83\x48\x81\x5b\x83\x7d\x83\x62\x83\x67\x82\xcd\x20\x25\x73\x20\x82\xc5\x82\xb7"/*@ "�P�`�����l������ 8 �r�b�g�^�J���[�t�H�[�}�b�g�� %s �ł�" @*/, String )) ;
		}

		// �P�`�����l������16�r�b�g�^�J���[�t�H�[�}�b�g�̑I��
		if( D3D11Device_CheckFormatSupport( D_DXGI_FORMAT_R16_UNORM, &FormatSupport ) == S_OK &&
			( FormatSupport & D_D3D11_FORMAT_SUPPORT_TEXTURE2D     ) != 0 )
		{
			GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ONE_I16 ] = D_DXGI_FORMAT_R16_UNORM ;
			String = "DXGI_FORMAT_R16_UNORM" ;
		}
		else
		if( D3D11Device_CheckFormatSupport( D_DXGI_FORMAT_R16G16_UNORM, &FormatSupport ) == S_OK &&
			( FormatSupport & D_D3D11_FORMAT_SUPPORT_TEXTURE2D     ) != 0 )
		{
			GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ONE_I16 ] = D_DXGI_FORMAT_R16G16_UNORM ;
			String = "DXGI_FORMAT_R16G16_UNORM" ;
		}
		else
		if( D3D11Device_CheckFormatSupport( D_DXGI_FORMAT_R16G16B16A16_UNORM, &FormatSupport ) == S_OK &&
			( FormatSupport & D_D3D11_FORMAT_SUPPORT_TEXTURE2D     ) != 0 )
		{
			GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ONE_I16 ] = D_DXGI_FORMAT_R16G16B16A16_UNORM ;
			String = "DXGI_FORMAT_R16G16B16A16_UNORM" ;
		}
		else
		{
			DXST_ERRORLOG_ADDA( "\x8e\x67\x97\x70\x82\xc5\x82\xab\x82\xe9\x82\x50\x83\x60\x83\x83\x83\x93\x83\x6c\x83\x8b\x90\xae\x90\x94\x20\x31\x36\x20\x83\x72\x83\x62\x83\x67\x8c\x5e\x83\x4a\x83\x89\x81\x5b\x83\x74\x83\x48\x81\x5b\x83\x7d\x83\x62\x83\x67\x82\xaa\x82\xa0\x82\xe8\x82\xdc\x82\xb9\x82\xf1\x82\xc5\x82\xb5\x82\xbd\x5c\x6e"/*@ "�g�p�ł���P�`�����l������ 16 �r�b�g�^�J���[�t�H�[�}�b�g������܂���ł���\n" @*/ ) ;
			GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ONE_I16 ] = D_DXGI_FORMAT_UNKNOWN ;
		}

		if( GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ONE_I16 ] != D_DXGI_FORMAT_UNKNOWN )
		{
			DXST_ERRORLOGFMT_ADDA(( "\x82\x50\x83\x60\x83\x83\x83\x93\x83\x6c\x83\x8b\x90\xae\x90\x94\x20\x31\x36\x20\x83\x72\x83\x62\x83\x67\x8c\x5e\x83\x4a\x83\x89\x81\x5b\x83\x74\x83\x48\x81\x5b\x83\x7d\x83\x62\x83\x67\x82\xcd\x20\x25\x73\x20\x82\xc5\x82\xb7"/*@ "�P�`�����l������ 16 �r�b�g�^�J���[�t�H�[�}�b�g�� %s �ł�" @*/, String )) ;
		}

		// �P�`�����l�����������_16�r�b�g�^�J���[�t�H�[�}�b�g�̑I��
		if( D3D11Device_CheckFormatSupport( D_DXGI_FORMAT_R16_FLOAT, &FormatSupport ) == S_OK &&
			( FormatSupport & D_D3D11_FORMAT_SUPPORT_TEXTURE2D     ) != 0 )
		{
			GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ONE_F16 ] = D_DXGI_FORMAT_R16_FLOAT ;
			String = "DXGI_FORMAT_R16_FLOAT" ;
		}
		else
		if( D3D11Device_CheckFormatSupport( D_DXGI_FORMAT_R16G16_FLOAT, &FormatSupport ) == S_OK &&
			( FormatSupport & D_D3D11_FORMAT_SUPPORT_TEXTURE2D     ) != 0 )
		{
			GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ONE_F16 ] = D_DXGI_FORMAT_R16G16_FLOAT ;
			String = "DXGI_FORMAT_R16G16_FLOAT" ;
		}
		else
		if( D3D11Device_CheckFormatSupport( D_DXGI_FORMAT_R16G16B16A16_FLOAT, &FormatSupport ) == S_OK &&
			( FormatSupport & D_D3D11_FORMAT_SUPPORT_TEXTURE2D     ) != 0 )
		{
			GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ONE_F16 ] = D_DXGI_FORMAT_R16G16B16A16_FLOAT ;
			String = "DXGI_FORMAT_R16G16B16A16_FLOAT" ;
		}
		else
		if( D3D11Device_CheckFormatSupport( D_DXGI_FORMAT_R32G32B32A32_FLOAT, &FormatSupport ) == S_OK &&
			( FormatSupport & D_D3D11_FORMAT_SUPPORT_TEXTURE2D     ) != 0 )
		{
			GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ONE_F16 ] = D_DXGI_FORMAT_R32G32B32A32_FLOAT ;
			String = "DXGI_FORMAT_R32G32B32A32_FLOAT" ;
		}
		else
		{
			DXST_ERRORLOG_ADDA( "\x8e\x67\x97\x70\x82\xc5\x82\xab\x82\xe9\x82\x50\x83\x60\x83\x83\x83\x93\x83\x6c\x83\x8b\x95\x82\x93\xae\x8f\xac\x90\x94\x93\x5f\x20\x31\x36\x20\x83\x72\x83\x62\x83\x67\x8c\x5e\x83\x4a\x83\x89\x81\x5b\x83\x74\x83\x48\x81\x5b\x83\x7d\x83\x62\x83\x67\x82\xaa\x82\xa0\x82\xe8\x82\xdc\x82\xb9\x82\xf1\x82\xc5\x82\xb5\x82\xbd\x5c\x6e"/*@ "�g�p�ł���P�`�����l�����������_ 16 �r�b�g�^�J���[�t�H�[�}�b�g������܂���ł���\n" @*/ ) ;
			GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ONE_F16 ] = D_DXGI_FORMAT_UNKNOWN ;
		}

		if( GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ONE_F16 ] != D_DXGI_FORMAT_UNKNOWN )
		{
			DXST_ERRORLOGFMT_ADDA(( "\x82\x50\x83\x60\x83\x83\x83\x93\x83\x6c\x83\x8b\x95\x82\x93\xae\x8f\xac\x90\x94\x93\x5f\x20\x31\x36\x20\x83\x72\x83\x62\x83\x67\x8c\x5e\x83\x4a\x83\x89\x81\x5b\x83\x74\x83\x48\x81\x5b\x83\x7d\x83\x62\x83\x67\x82\xcd\x20\x25\x73\x20\x82\xc5\x82\xb7"/*@ "�P�`�����l�����������_ 16 �r�b�g�^�J���[�t�H�[�}�b�g�� %s �ł�" @*/, String )) ;
		}

		// �P�`�����l�����������_32�r�b�g�^�J���[�t�H�[�}�b�g�̑I��
		if( D3D11Device_CheckFormatSupport( D_DXGI_FORMAT_R32_FLOAT, &FormatSupport ) == S_OK &&
			( FormatSupport & D_D3D11_FORMAT_SUPPORT_TEXTURE2D     ) != 0 )
		{
			GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ONE_F32 ] = D_DXGI_FORMAT_R32_FLOAT ;
			String = "DXGI_FORMAT_R32_FLOAT" ;
		}
		else
		if( D3D11Device_CheckFormatSupport( D_DXGI_FORMAT_R32G32_FLOAT, &FormatSupport ) == S_OK &&
			( FormatSupport & D_D3D11_FORMAT_SUPPORT_TEXTURE2D     ) != 0 )
		{
			GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ONE_F32 ] = D_DXGI_FORMAT_R32G32_FLOAT ;
			String = "DXGI_FORMAT_R32G32_FLOAT" ;
		}
		else
		if( D3D11Device_CheckFormatSupport( D_DXGI_FORMAT_R32G32B32A32_FLOAT, &FormatSupport ) == S_OK &&
			( FormatSupport & D_D3D11_FORMAT_SUPPORT_TEXTURE2D     ) != 0 )
		{
			GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ONE_F32 ] = D_DXGI_FORMAT_R32G32B32A32_FLOAT ;
			String = "DXGI_FORMAT_R32G32B32A32_FLOAT" ;
		}
		else
		if( D3D11Device_CheckFormatSupport( D_DXGI_FORMAT_R16G16B16A16_FLOAT, &FormatSupport ) == S_OK &&
			( FormatSupport & D_D3D11_FORMAT_SUPPORT_TEXTURE2D     ) != 0 )
		{
			GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ONE_F32 ] = D_DXGI_FORMAT_R16G16B16A16_FLOAT ;
			String = "DXGI_FORMAT_R16G16B16A16_FLOAT" ;
		}
		else
		{
			DXST_ERRORLOG_ADDA( "\x8e\x67\x97\x70\x82\xc5\x82\xab\x82\xe9\x82\x50\x83\x60\x83\x83\x83\x93\x83\x6c\x83\x8b\x95\x82\x93\xae\x8f\xac\x90\x94\x93\x5f\x20\x33\x32\x20\x83\x72\x83\x62\x83\x67\x8c\x5e\x83\x4a\x83\x89\x81\x5b\x83\x74\x83\x48\x81\x5b\x83\x7d\x83\x62\x83\x67\x82\xaa\x82\xa0\x82\xe8\x82\xdc\x82\xb9\x82\xf1\x82\xc5\x82\xb5\x82\xbd\x5c\x6e"/*@ "�g�p�ł���P�`�����l�����������_ 32 �r�b�g�^�J���[�t�H�[�}�b�g������܂���ł���\n" @*/ ) ;
			GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ONE_F32 ] = D_DXGI_FORMAT_UNKNOWN ;
		}

		if( GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_ONE_F32 ] != D_DXGI_FORMAT_UNKNOWN )
		{
			DXST_ERRORLOGFMT_ADDA(( "\x82\x50\x83\x60\x83\x83\x83\x93\x83\x6c\x83\x8b\x95\x82\x93\xae\x8f\xac\x90\x94\x93\x5f\x20\x33\x32\x20\x83\x72\x83\x62\x83\x67\x8c\x5e\x83\x4a\x83\x89\x81\x5b\x83\x74\x83\x48\x81\x5b\x83\x7d\x83\x62\x83\x67\x82\xcd\x20\x25\x73\x20\x82\xc5\x82\xb7"/*@ "�P�`�����l�����������_ 32 �r�b�g�^�J���[�t�H�[�}�b�g�� %s �ł�" @*/, String )) ;
		}

		// �Q�`�����l������8�r�b�g�^�J���[�t�H�[�}�b�g�̑I��
		if( D3D11Device_CheckFormatSupport( D_DXGI_FORMAT_R8G8_UNORM, &FormatSupport ) == S_OK &&
			( FormatSupport & D_D3D11_FORMAT_SUPPORT_TEXTURE2D     ) != 0 )
		{
			GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_TWO_I8 ] = D_DXGI_FORMAT_R8G8_UNORM ;
			String = "DXGI_FORMAT_R8G8_UNORM" ;
		}
		else
		if( D3D11Device_CheckFormatSupport( D_DXGI_FORMAT_R8G8B8A8_UNORM, &FormatSupport ) == S_OK &&
			( FormatSupport & D_D3D11_FORMAT_SUPPORT_TEXTURE2D     ) != 0 )
		{
			GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_TWO_I8 ] = D_DXGI_FORMAT_R8G8B8A8_UNORM ;
			String = "DXGI_FORMAT_R8G8B8A8_UNORM" ;
		}
		else
		{
			DXST_ERRORLOG_ADDA( "\x8e\x67\x97\x70\x82\xc5\x82\xab\x82\xe9\x82\x51\x83\x60\x83\x83\x83\x93\x83\x6c\x83\x8b\x90\xae\x90\x94\x20\x38\x20\x83\x72\x83\x62\x83\x67\x8c\x5e\x83\x4a\x83\x89\x81\x5b\x83\x74\x83\x48\x81\x5b\x83\x7d\x83\x62\x83\x67\x82\xaa\x82\xa0\x82\xe8\x82\xdc\x82\xb9\x82\xf1\x82\xc5\x82\xb5\x82\xbd\x5c\x6e"/*@ "�g�p�ł���Q�`�����l������ 8 �r�b�g�^�J���[�t�H�[�}�b�g������܂���ł���\n" @*/ ) ;
			GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_TWO_I8 ] = D_DXGI_FORMAT_UNKNOWN ;
		}

		if( GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_TWO_I8 ] != D_DXGI_FORMAT_UNKNOWN )
		{
			DXST_ERRORLOGFMT_ADDA(( "\x82\x51\x83\x60\x83\x83\x83\x93\x83\x6c\x83\x8b\x90\xae\x90\x94\x20\x38\x20\x83\x72\x83\x62\x83\x67\x8c\x5e\x83\x4a\x83\x89\x81\x5b\x83\x74\x83\x48\x81\x5b\x83\x7d\x83\x62\x83\x67\x82\xcd\x20\x25\x73\x20\x82\xc5\x82\xb7"/*@ "�Q�`�����l������ 8 �r�b�g�^�J���[�t�H�[�}�b�g�� %s �ł�" @*/, String )) ;
		}

		// �Q�`�����l������16�r�b�g�^�J���[�t�H�[�}�b�g�̑I��
		if( D3D11Device_CheckFormatSupport( D_DXGI_FORMAT_R16G16_UNORM, &FormatSupport ) == S_OK &&
			( FormatSupport & D_D3D11_FORMAT_SUPPORT_TEXTURE2D     ) != 0 )
		{
			GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_TWO_I16 ] = D_DXGI_FORMAT_R16G16_UNORM ;
			String = "DXGI_FORMAT_R16G16_UNORM" ;
		}
		else
		if( D3D11Device_CheckFormatSupport( D_DXGI_FORMAT_R16G16B16A16_UNORM, &FormatSupport ) == S_OK &&
			( FormatSupport & D_D3D11_FORMAT_SUPPORT_TEXTURE2D     ) != 0 )
		{
			GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_TWO_I16 ] = D_DXGI_FORMAT_R16G16B16A16_UNORM ;
			String = "DXGI_FORMAT_R16G16B16A16_UNORM" ;
		}
		else
		{
			DXST_ERRORLOG_ADDA( "\x8e\x67\x97\x70\x82\xc5\x82\xab\x82\xe9\x82\x51\x83\x60\x83\x83\x83\x93\x83\x6c\x83\x8b\x90\xae\x90\x94\x20\x31\x36\x20\x83\x72\x83\x62\x83\x67\x8c\x5e\x83\x4a\x83\x89\x81\x5b\x83\x74\x83\x48\x81\x5b\x83\x7d\x83\x62\x83\x67\x82\xaa\x82\xa0\x82\xe8\x82\xdc\x82\xb9\x82\xf1\x82\xc5\x82\xb5\x82\xbd\x5c\x6e"/*@ "�g�p�ł���Q�`�����l������ 16 �r�b�g�^�J���[�t�H�[�}�b�g������܂���ł���\n" @*/ ) ;
			GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_TWO_I16 ] = D_DXGI_FORMAT_UNKNOWN ;
		}

		if( GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_TWO_I16 ] != D_DXGI_FORMAT_UNKNOWN )
		{
			DXST_ERRORLOGFMT_ADDA(( "\x82\x51\x83\x60\x83\x83\x83\x93\x83\x6c\x83\x8b\x90\xae\x90\x94\x20\x31\x36\x20\x83\x72\x83\x62\x83\x67\x8c\x5e\x83\x4a\x83\x89\x81\x5b\x83\x74\x83\x48\x81\x5b\x83\x7d\x83\x62\x83\x67\x82\xcd\x20\x25\x73\x20\x82\xc5\x82\xb7"/*@ "�Q�`�����l������ 16 �r�b�g�^�J���[�t�H�[�}�b�g�� %s �ł�" @*/, String )) ;
		}

		// �Q�`�����l�����������_16�r�b�g�^�J���[�t�H�[�}�b�g�̑I��
		if( D3D11Device_CheckFormatSupport( D_DXGI_FORMAT_R16G16_FLOAT, &FormatSupport ) == S_OK &&
			( FormatSupport & D_D3D11_FORMAT_SUPPORT_TEXTURE2D     ) != 0 )
		{
			GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_TWO_F16 ] = D_DXGI_FORMAT_R16G16_FLOAT ;
			String = "DXGI_FORMAT_R16G16_FLOAT" ;
		}
		else
		if( D3D11Device_CheckFormatSupport( D_DXGI_FORMAT_R16G16B16A16_FLOAT, &FormatSupport ) == S_OK &&
			( FormatSupport & D_D3D11_FORMAT_SUPPORT_TEXTURE2D     ) != 0 )
		{
			GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_TWO_F16 ] = D_DXGI_FORMAT_R16G16B16A16_FLOAT ;
			String = "DXGI_FORMAT_R16G16B16A16_FLOAT" ;
		}
		else
		if( D3D11Device_CheckFormatSupport( D_DXGI_FORMAT_R32G32B32A32_FLOAT, &FormatSupport ) == S_OK &&
			( FormatSupport & D_D3D11_FORMAT_SUPPORT_TEXTURE2D     ) != 0 )
		{
			GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_TWO_F16 ] = D_DXGI_FORMAT_R32G32B32A32_FLOAT ;
			String = "DXGI_FORMAT_R32G32B32A32_FLOAT" ;
		}
		else
		{
			DXST_ERRORLOG_ADDA( "\x8e\x67\x97\x70\x82\xc5\x82\xab\x82\xe9\x82\x51\x83\x60\x83\x83\x83\x93\x83\x6c\x83\x8b\x95\x82\x93\xae\x8f\xac\x90\x94\x93\x5f\x20\x31\x36\x20\x83\x72\x83\x62\x83\x67\x8c\x5e\x83\x4a\x83\x89\x81\x5b\x83\x74\x83\x48\x81\x5b\x83\x7d\x83\x62\x83\x67\x82\xaa\x82\xa0\x82\xe8\x82\xdc\x82\xb9\x82\xf1\x82\xc5\x82\xb5\x82\xbd\x5c\x6e"/*@ "�g�p�ł���Q�`�����l�����������_ 16 �r�b�g�^�J���[�t�H�[�}�b�g������܂���ł���\n" @*/ ) ;
			GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_TWO_F16 ] = D_DXGI_FORMAT_UNKNOWN ;
		}

		if( GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_TWO_F16 ] != D_DXGI_FORMAT_UNKNOWN )
		{
			DXST_ERRORLOGFMT_ADDA(( "\x82\x51\x83\x60\x83\x83\x83\x93\x83\x6c\x83\x8b\x95\x82\x93\xae\x8f\xac\x90\x94\x93\x5f\x20\x31\x36\x20\x83\x72\x83\x62\x83\x67\x8c\x5e\x83\x4a\x83\x89\x81\x5b\x83\x74\x83\x48\x81\x5b\x83\x7d\x83\x62\x83\x67\x82\xcd\x20\x25\x73\x20\x82\xc5\x82\xb7"/*@ "�Q�`�����l�����������_ 16 �r�b�g�^�J���[�t�H�[�}�b�g�� %s �ł�" @*/, String )) ;
		}

		// �Q�`�����l�����������_32�r�b�g�^�J���[�t�H�[�}�b�g�̑I��
		if( D3D11Device_CheckFormatSupport( D_DXGI_FORMAT_R32G32_FLOAT, &FormatSupport ) == S_OK &&
			( FormatSupport & D_D3D11_FORMAT_SUPPORT_TEXTURE2D     ) != 0 )
		{
			GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_TWO_F32 ] = D_DXGI_FORMAT_R32G32_FLOAT ;
			String = "DXGI_FORMAT_R32G32_FLOAT" ;
		}
		else
		if( D3D11Device_CheckFormatSupport( D_DXGI_FORMAT_R32G32B32A32_FLOAT, &FormatSupport ) == S_OK &&
			( FormatSupport & D_D3D11_FORMAT_SUPPORT_TEXTURE2D     ) != 0 )
		{
			GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_TWO_F32 ] = D_DXGI_FORMAT_R32G32B32A32_FLOAT ;
			String = "DXGI_FORMAT_R32G32B32A32_FLOAT" ;
		}
		else
		if( D3D11Device_CheckFormatSupport( D_DXGI_FORMAT_R16G16B16A16_FLOAT, &FormatSupport ) == S_OK &&
			( FormatSupport & D_D3D11_FORMAT_SUPPORT_TEXTURE2D     ) != 0 )
		{
			GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_TWO_F32 ] = D_DXGI_FORMAT_R16G16B16A16_FLOAT ;
			String = "DXGI_FORMAT_R16G16B16A16_FLOAT" ;
		}
		else
		{
			DXST_ERRORLOG_ADDA( "\x8e\x67\x97\x70\x82\xc5\x82\xab\x82\xe9\x82\x51\x83\x60\x83\x83\x83\x93\x83\x6c\x83\x8b\x95\x82\x93\xae\x8f\xac\x90\x94\x93\x5f\x20\x33\x32\x20\x83\x72\x83\x62\x83\x67\x8c\x5e\x83\x4a\x83\x89\x81\x5b\x83\x74\x83\x48\x81\x5b\x83\x7d\x83\x62\x83\x67\x82\xaa\x82\xa0\x82\xe8\x82\xdc\x82\xb9\x82\xf1\x82\xc5\x82\xb5\x82\xbd\x5c\x6e"/*@ "�g�p�ł���Q�`�����l�����������_ 32 �r�b�g�^�J���[�t�H�[�}�b�g������܂���ł���\n" @*/ ) ;
			GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_TWO_F32 ] = D_DXGI_FORMAT_UNKNOWN ;
		}

		if( GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_TWO_F32 ] != D_DXGI_FORMAT_UNKNOWN )
		{
			DXST_ERRORLOGFMT_ADDA(( "\x82\x51\x83\x60\x83\x83\x83\x93\x83\x6c\x83\x8b\x95\x82\x93\xae\x8f\xac\x90\x94\x93\x5f\x20\x33\x32\x20\x83\x72\x83\x62\x83\x67\x8c\x5e\x83\x4a\x83\x89\x81\x5b\x83\x74\x83\x48\x81\x5b\x83\x7d\x83\x62\x83\x67\x82\xcd\x20\x25\x73\x20\x82\xc5\x82\xb7"/*@ "�Q�`�����l�����������_ 32 �r�b�g�^�J���[�t�H�[�}�b�g�� %s �ł�" @*/, String )) ;
		}





		// 16bit�`��\�J���[�t�H�[�}�b�g�̑I��
		if( D3D11Device_CheckFormatSupport( D_DXGI_FORMAT_B5G6R5_UNORM, &FormatSupport ) == S_OK ||
			( FormatSupport & D_D3D11_FORMAT_SUPPORT_TEXTURE2D     ) != 0 ||
			( FormatSupport & D_D3D11_FORMAT_SUPPORT_RENDER_TARGET ) != 0 )
		{
			GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_RGB16 ] = D_DXGI_FORMAT_B5G6R5_UNORM ;
			String = "DXGI_FORMAT_B5G6R5_UNORM" ;
		}
		else
		if( D3D11Device_CheckFormatSupport( D_DXGI_FORMAT_B8G8R8X8_UNORM, &FormatSupport ) == S_OK ||
			( FormatSupport & D_D3D11_FORMAT_SUPPORT_TEXTURE2D     ) != 0 ||
			( FormatSupport & D_D3D11_FORMAT_SUPPORT_RENDER_TARGET ) != 0 )
		{
			GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_RGB16 ] = D_DXGI_FORMAT_B8G8R8X8_UNORM ;
			String = "DXGI_FORMAT_B8G8R8X8_UNORM" ;
		}
		else
		if( D3D11Device_CheckFormatSupport( D_DXGI_FORMAT_R8G8B8A8_UNORM, &FormatSupport ) == S_OK ||
			( FormatSupport & D_D3D11_FORMAT_SUPPORT_TEXTURE2D     ) != 0 ||
			( FormatSupport & D_D3D11_FORMAT_SUPPORT_RENDER_TARGET ) != 0 )
		{
			GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_RGB16 ] = D_DXGI_FORMAT_R8G8B8A8_UNORM ;
			String = "DXGI_FORMAT_R8G8B8A8_UNORM" ;
		}
		else
		{
			DXST_ERRORLOG_ADDA( "\x8e\x67\x97\x70\x82\xc5\x82\xab\x82\xe9 16bit \x95\x60\x89\xe6\x89\xc2\x94\x5c\x83\x4a\x83\x89\x81\x5b\x83\x74\x83\x48\x81\x5b\x83\x7d\x83\x62\x83\x67\x82\xaa\x8c\xa9\x82\xc2\x82\xa9\x82\xe8\x82\xdc\x82\xb9\x82\xf1\x82\xc5\x82\xb5\x82\xbd\n"/*@ "�g�p�ł��� 16bit �`��\�J���[�t�H�[�}�b�g��������܂���ł���\n" @*/ ) ;
			GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_RGB16 ] = D_DXGI_FORMAT_UNKNOWN ;
		}

		if( GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_RGB16 ] != D_DXGI_FORMAT_UNKNOWN )
		{
			DXST_ERRORLOGFMT_ADDA(( "\x95\x60\x89\xe6\x97\x70 16bit \x83\x4a\x83\x89\x81\x5b\x83\x74\x83\x48\x81\x5b\x83\x7d\x83\x62\x83\x67\x82\xcd %s \x82\xc5\x82\xb7"/*@ "�`��p 16bit �J���[�t�H�[�}�b�g�� %s �ł�" @*/, String )) ;
		}

		// 32bit�`��\�J���[�t�H�[�}�b�g�̑I��
		if( D3D11Device_CheckFormatSupport( D_DXGI_FORMAT_B8G8R8X8_UNORM, &FormatSupport ) == S_OK ||
			( FormatSupport & D_D3D11_FORMAT_SUPPORT_TEXTURE2D     ) != 0 ||
			( FormatSupport & D_D3D11_FORMAT_SUPPORT_RENDER_TARGET ) != 0 )
		{
			GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_RGB32 ] = D_DXGI_FORMAT_B8G8R8X8_UNORM ;
			String = "DXGI_FORMAT_B8G8R8X8_UNORM" ;
		}
		else
		if( D3D11Device_CheckFormatSupport( D_DXGI_FORMAT_R8G8B8A8_UNORM, &FormatSupport ) == S_OK ||
			( FormatSupport & D_D3D11_FORMAT_SUPPORT_TEXTURE2D     ) != 0 ||
			( FormatSupport & D_D3D11_FORMAT_SUPPORT_RENDER_TARGET ) != 0 )
		{
			GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_RGB32 ] = D_DXGI_FORMAT_R8G8B8A8_UNORM ;
			String = "DXGI_FORMAT_R8G8B8A8_UNORM" ;
		}
		else
		{
			DXST_ERRORLOG_ADDA( "\x8e\x67\x97\x70\x82\xc5\x82\xab\x82\xe9 32bit \x95\x60\x89\xe6\x89\xc2\x94\x5c\x83\x4a\x83\x89\x81\x5b\x83\x74\x83\x48\x81\x5b\x83\x7d\x83\x62\x83\x67\x82\xaa\x8c\xa9\x82\xc2\x82\xa9\x82\xe8\x82\xdc\x82\xb9\x82\xf1\x82\xc5\x82\xb5\x82\xbd\n"/*@ "�g�p�ł��� 32bit �`��\�J���[�t�H�[�}�b�g��������܂���ł���\n" @*/ ) ;
			GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_RGB32 ] = D_DXGI_FORMAT_UNKNOWN ;
		}

		if( GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_RGB32 ] != D_DXGI_FORMAT_UNKNOWN )
		{
			DXST_ERRORLOGFMT_ADDA(( "\x95\x60\x89\xe6\x97\x70 32bit \x83\x4a\x83\x89\x81\x5b\x83\x74\x83\x48\x81\x5b\x83\x7d\x83\x62\x83\x67\x82\xcd %s \x82\xc5\x82\xb7"/*@ "�`��p 32bit �J���[�t�H�[�}�b�g�� %s �ł�" @*/, String )) ;
		}

		// �A���t�@�`�����l���t�� 32bit�`��\�J���[�t�H�[�}�b�g�̑I��
		if( D3D11Device_CheckFormatSupport( D_DXGI_FORMAT_R8G8B8A8_UNORM, &FormatSupport ) == S_OK ||
			( FormatSupport & D_D3D11_FORMAT_SUPPORT_TEXTURE2D     ) != 0 ||
			( FormatSupport & D_D3D11_FORMAT_SUPPORT_RENDER_TARGET ) != 0 )
		{
			GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ALPHA_RGB32 ] = D_DXGI_FORMAT_R8G8B8A8_UNORM ;
			String = "DXGI_FORMAT_R8G8B8A8_UNORM" ;
		}
		else
		{
			DXST_ERRORLOG_ADDA( "\x8e\x67\x97\x70\x82\xc5\x82\xab\x82\xe9\x83\x41\x83\x8b\x83\x74\x83\x40\x95\x74\x82\xab 32bit \x95\x60\x89\xe6\x89\xc2\x94\x5c\x83\x4a\x83\x89\x81\x5b\x83\x74\x83\x48\x81\x5b\x83\x7d\x83\x62\x83\x67\x82\xaa\x8c\xa9\x82\xc2\x82\xa9\x82\xe8\x82\xdc\x82\xb9\x82\xf1\x82\xc5\x82\xb5\x82\xbd\n"/*@ "�g�p�ł���A���t�@�t�� 32bit �`��\�J���[�t�H�[�}�b�g��������܂���ł���\n" @*/ ) ;
			GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ALPHA_RGB32 ] = D_DXGI_FORMAT_UNKNOWN ;
		}

		if( GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ALPHA_RGB32 ] != D_DXGI_FORMAT_UNKNOWN )
		{
			DXST_ERRORLOGFMT_ADDA(( "\x95\x60\x89\xe6\x97\x70\x83\x41\x83\x8b\x83\x74\x83\x40\x95\x74\x82\xab 32bit \x83\x4a\x83\x89\x81\x5b\x83\x74\x83\x48\x81\x5b\x83\x7d\x83\x62\x83\x67\x82\xcd %s \x82\xc5\x82\xb7"/*@ "�`��p�A���t�@�t�� 32bit �J���[�t�H�[�}�b�g�� %s �ł�" @*/, String )) ;
		}

		// �`��\ ARGB����16�r�b�g�^�J���[�t�H�[�}�b�g�̑I��
		if( D3D11Device_CheckFormatSupport( D_DXGI_FORMAT_R16G16B16A16_UNORM, &FormatSupport ) == S_OK &&
			( FormatSupport & D_D3D11_FORMAT_SUPPORT_TEXTURE2D     ) != 0 &&
			( FormatSupport & D_D3D11_FORMAT_SUPPORT_RENDER_TARGET ) != 0 )
		{
			GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ABGR_I16 ] = D_DXGI_FORMAT_R16G16B16A16_UNORM ;
			String = "DXGI_FORMAT_R16G16B16A16_UNORM" ;
		}
		else
		{
			DXST_ERRORLOG_ADDA( "\x8e\x67\x97\x70\x82\xc5\x82\xab\x82\xe9\x95\x60\x89\xe6\x97\x70 ABGR \x90\xae\x90\x94 16 \x83\x72\x83\x62\x83\x67\x8c\x5e\x83\x4a\x83\x89\x81\x5b\x83\x74\x83\x48\x81\x5b\x83\x7d\x83\x62\x83\x67\x82\xaa\x82\xa0\x82\xe8\x82\xdc\x82\xb9\x82\xf1\x82\xc5\x82\xb5\x82\xbd\n"/*@ "�g�p�ł���`��p ABGR ���� 16 �r�b�g�^�J���[�t�H�[�}�b�g������܂���ł���\n" @*/ ) ;
			GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ABGR_I16 ] = D_DXGI_FORMAT_UNKNOWN ;
		}

		if( GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ABGR_I16 ] != D_DXGI_FORMAT_UNKNOWN )
		{
			DXST_ERRORLOGFMT_ADDA(( "\x95\x60\x89\xe6\x97\x70 ABGR \x90\xae\x90\x94 16 \x83\x72\x83\x62\x83\x67\x8c\x5e\x83\x4a\x83\x89\x81\x5b\x83\x74\x83\x48\x81\x5b\x83\x7d\x83\x62\x83\x67\x82\xcd %s \x82\xc5\x82\xb7"/*@ "�`��p ABGR ���� 16 �r�b�g�^�J���[�t�H�[�}�b�g�� %s �ł�" @*/, String )) ;
		}

		// �`��\ ARGB���������_16�r�b�g�^�J���[�t�H�[�}�b�g�̑I��
		if( D3D11Device_CheckFormatSupport( D_DXGI_FORMAT_R16G16B16A16_FLOAT, &FormatSupport ) == S_OK &&
			( FormatSupport & D_D3D11_FORMAT_SUPPORT_TEXTURE2D     ) != 0 &&
			( FormatSupport & D_D3D11_FORMAT_SUPPORT_RENDER_TARGET ) != 0 )
		{
			GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ABGR_F16 ] = D_DXGI_FORMAT_R16G16B16A16_FLOAT ;
			String = "DXGI_FORMAT_R16G16B16A16_FLOAT" ;
		}
		else
		{
			DXST_ERRORLOG_ADDA( "\x8e\x67\x97\x70\x82\xc5\x82\xab\x82\xe9\x95\x60\x89\xe6\x97\x70 ABGR \x95\x82\x93\xae\x8f\xac\x90\x94\x93\x5f 16 \x83\x72\x83\x62\x83\x67\x8c\x5e\x83\x4a\x83\x89\x81\x5b\x83\x74\x83\x48\x81\x5b\x83\x7d\x83\x62\x83\x67\x82\xaa\x82\xa0\x82\xe8\x82\xdc\x82\xb9\x82\xf1\x82\xc5\x82\xb5\x82\xbd\n"/*@ "�g�p�ł���`��p ABGR ���������_ 16 �r�b�g�^�J���[�t�H�[�}�b�g������܂���ł���\n" @*/ ) ;
			GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ABGR_F16 ] = D_DXGI_FORMAT_UNKNOWN ;
		}

		if( GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ABGR_F16 ] != D_DXGI_FORMAT_UNKNOWN )
		{
			DXST_ERRORLOGFMT_ADDA(( "\x95\x60\x89\xe6\x97\x70 ABGR \x95\x82\x93\xae\x8f\xac\x90\x94\x93\x5f 16 \x83\x72\x83\x62\x83\x67\x8c\x5e\x83\x4a\x83\x89\x81\x5b\x83\x74\x83\x48\x81\x5b\x83\x7d\x83\x62\x83\x67\x82\xcd %s \x82\xc5\x82\xb7"/*@ "�`��p ABGR ���������_ 16 �r�b�g�^�J���[�t�H�[�}�b�g�� %s �ł�" @*/, String )) ;
		}

		// �`��\ ARGB���������_32�r�b�g�^�J���[�t�H�[�}�b�g�̑I��
		if( D3D11Device_CheckFormatSupport( D_DXGI_FORMAT_R32G32B32A32_FLOAT, &FormatSupport ) == S_OK &&
			( FormatSupport & D_D3D11_FORMAT_SUPPORT_TEXTURE2D     ) != 0 &&
			( FormatSupport & D_D3D11_FORMAT_SUPPORT_RENDER_TARGET ) != 0 )
		{
			GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ABGR_F32 ] = D_DXGI_FORMAT_R32G32B32A32_FLOAT ;
			String = "DXGI_FORMAT_R32G32B32A32_FLOAT" ;
		}
		else
		if( D3D11Device_CheckFormatSupport( D_DXGI_FORMAT_R16G16B16A16_FLOAT, &FormatSupport ) == S_OK &&
			( FormatSupport & D_D3D11_FORMAT_SUPPORT_TEXTURE2D     ) != 0 &&
			( FormatSupport & D_D3D11_FORMAT_SUPPORT_RENDER_TARGET ) != 0 )
		{
			GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ABGR_F32 ] = D_DXGI_FORMAT_R16G16B16A16_FLOAT ;
			String = "DXGI_FORMAT_R16G16B16A16_FLOAT" ;
		}
		else
		{
			DXST_ERRORLOG_ADDA( "\x8e\x67\x97\x70\x82\xc5\x82\xab\x82\xe9\x95\x60\x89\xe6\x97\x70 ABGR \x95\x82\x93\xae\x8f\xac\x90\x94\x93\x5f 32 \x83\x72\x83\x62\x83\x67\x8c\x5e\x83\x4a\x83\x89\x81\x5b\x83\x74\x83\x48\x81\x5b\x83\x7d\x83\x62\x83\x67\x82\xaa\x82\xa0\x82\xe8\x82\xdc\x82\xb9\x82\xf1\x82\xc5\x82\xb5\x82\xbd\n"/*@ "�g�p�ł���`��p ABGR ���������_ 32 �r�b�g�^�J���[�t�H�[�}�b�g������܂���ł���\n" @*/ ) ;
			GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ABGR_F32 ] = D_DXGI_FORMAT_UNKNOWN ;
		}

		if( GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ABGR_F32 ] != D_DXGI_FORMAT_UNKNOWN )
		{
			DXST_ERRORLOGFMT_ADDA(( "\x95\x60\x89\xe6\x97\x70 ABGR \x95\x82\x93\xae\x8f\xac\x90\x94\x93\x5f 32 \x83\x72\x83\x62\x83\x67\x8c\x5e\x83\x4a\x83\x89\x81\x5b\x83\x74\x83\x48\x81\x5b\x83\x7d\x83\x62\x83\x67\x82\xcd %s \x82\xc5\x82\xb7"/*@ "�`��p ABGR ���������_ 32 �r�b�g�^�J���[�t�H�[�}�b�g�� %s �ł�" @*/, String )) ;
		}

		// �`��\ �P�`�����l������8�r�b�g�^�J���[�t�H�[�}�b�g�̑I��
		if( D3D11Device_CheckFormatSupport( D_DXGI_FORMAT_R8_UNORM, &FormatSupport ) == S_OK &&
			( FormatSupport & D_D3D11_FORMAT_SUPPORT_TEXTURE2D     ) != 0 &&
			( FormatSupport & D_D3D11_FORMAT_SUPPORT_RENDER_TARGET ) != 0 )
		{
			GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ONE_I8 ] = D_DXGI_FORMAT_R8_UNORM ;
			String = "DXGI_FORMAT_R8_UNORM" ;
		}
		else
		if( D3D11Device_CheckFormatSupport( D_DXGI_FORMAT_R8G8_UNORM, &FormatSupport ) == S_OK &&
			( FormatSupport & D_D3D11_FORMAT_SUPPORT_TEXTURE2D     ) != 0 &&
			( FormatSupport & D_D3D11_FORMAT_SUPPORT_RENDER_TARGET ) != 0 )
		{
			GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ONE_I8 ] = D_DXGI_FORMAT_R8G8_UNORM ;
			String = "DXGI_FORMAT_R8G8_UNORM" ;
		}
		else
		if( D3D11Device_CheckFormatSupport( D_DXGI_FORMAT_R8G8B8A8_UNORM, &FormatSupport ) == S_OK &&
			( FormatSupport & D_D3D11_FORMAT_SUPPORT_TEXTURE2D     ) != 0 &&
			( FormatSupport & D_D3D11_FORMAT_SUPPORT_RENDER_TARGET ) != 0 )
		{
			GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ONE_I8 ] = D_DXGI_FORMAT_R8G8B8A8_UNORM ;
			String = "DXGI_FORMAT_R8G8B8A8_UNORM" ;
		}
		else
		{
			DXST_ERRORLOG_ADDA( "\x8e\x67\x97\x70\x82\xc5\x82\xab\x82\xe9\x95\x60\x89\xe6\x97\x70\x82\x50\x83\x60\x83\x83\x83\x93\x83\x6c\x83\x8b\x90\xae\x90\x94 8 \x83\x72\x83\x62\x83\x67\x8c\x5e\x83\x4a\x83\x89\x81\x5b\x83\x74\x83\x48\x81\x5b\x83\x7d\x83\x62\x83\x67\x82\xaa\x82\xa0\x82\xe8\x82\xdc\x82\xb9\x82\xf1\x82\xc5\x82\xb5\x82\xbd\n"/*@ "�g�p�ł���`��p�P�`�����l������ 8 �r�b�g�^�J���[�t�H�[�}�b�g������܂���ł���\n" @*/ ) ;
			GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ONE_I8 ] = D_DXGI_FORMAT_UNKNOWN ;
		}

		if( GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ONE_I8 ] != D_DXGI_FORMAT_UNKNOWN )
		{
			DXST_ERRORLOGFMT_ADDA(( "\x95\x60\x89\xe6\x97\x70\x82\x50\x83\x60\x83\x83\x83\x93\x83\x6c\x83\x8b\x90\xae\x90\x94 8 \x83\x72\x83\x62\x83\x67\x8c\x5e\x83\x4a\x83\x89\x81\x5b\x83\x74\x83\x48\x81\x5b\x83\x7d\x83\x62\x83\x67\x82\xcd %s \x82\xc5\x82\xb7"/*@ "�`��p�P�`�����l������ 8 �r�b�g�^�J���[�t�H�[�}�b�g�� %s �ł�" @*/, String )) ;
		}

		// �`��\ �P�`�����l������16�r�b�g�^�J���[�t�H�[�}�b�g�̑I��
		if( D3D11Device_CheckFormatSupport( D_DXGI_FORMAT_R16_UNORM, &FormatSupport ) == S_OK &&
			( FormatSupport & D_D3D11_FORMAT_SUPPORT_TEXTURE2D     ) != 0 &&
			( FormatSupport & D_D3D11_FORMAT_SUPPORT_RENDER_TARGET ) != 0 )
		{
			GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ONE_I16 ] = D_DXGI_FORMAT_R16_UNORM ;
			String = "DXGI_FORMAT_R16_UNORM" ;
		}
		else
		if( D3D11Device_CheckFormatSupport( D_DXGI_FORMAT_R16G16_UNORM, &FormatSupport ) == S_OK &&
			( FormatSupport & D_D3D11_FORMAT_SUPPORT_TEXTURE2D     ) != 0 &&
			( FormatSupport & D_D3D11_FORMAT_SUPPORT_RENDER_TARGET ) != 0 )
		{
			GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ONE_I16 ] = D_DXGI_FORMAT_R16G16_UNORM ;
			String = "DXGI_FORMAT_R16G16_UNORM" ;
		}
		else
		if( D3D11Device_CheckFormatSupport( D_DXGI_FORMAT_R16G16B16A16_UNORM, &FormatSupport ) == S_OK &&
			( FormatSupport & D_D3D11_FORMAT_SUPPORT_TEXTURE2D     ) != 0 &&
			( FormatSupport & D_D3D11_FORMAT_SUPPORT_RENDER_TARGET ) != 0 )
		{
			GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ONE_I16 ] = D_DXGI_FORMAT_R16G16B16A16_UNORM ;
			String = "DXGI_FORMAT_R16G16B16A16_UNORM" ;
		}
		else
		{
			DXST_ERRORLOG_ADDA( "\x8e\x67\x97\x70\x82\xc5\x82\xab\x82\xe9\x95\x60\x89\xe6\x97\x70\x82\x50\x83\x60\x83\x83\x83\x93\x83\x6c\x83\x8b\x90\xae\x90\x94 16 \x83\x72\x83\x62\x83\x67\x8c\x5e\x83\x4a\x83\x89\x81\x5b\x83\x74\x83\x48\x81\x5b\x83\x7d\x83\x62\x83\x67\x82\xaa\x82\xa0\x82\xe8\x82\xdc\x82\xb9\x82\xf1\x82\xc5\x82\xb5\x82\xbd\n"/*@ "�g�p�ł���`��p�P�`�����l������ 16 �r�b�g�^�J���[�t�H�[�}�b�g������܂���ł���\n" @*/ ) ;
			GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ONE_I16 ] = D_DXGI_FORMAT_UNKNOWN ;
		}

		if( GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ONE_I16 ] != D_DXGI_FORMAT_UNKNOWN )
		{
			DXST_ERRORLOGFMT_ADDA(( "\x95\x60\x89\xe6\x97\x70\x82\x50\x83\x60\x83\x83\x83\x93\x83\x6c\x83\x8b\x90\xae\x90\x94 16 \x83\x72\x83\x62\x83\x67\x8c\x5e\x83\x4a\x83\x89\x81\x5b\x83\x74\x83\x48\x81\x5b\x83\x7d\x83\x62\x83\x67\x82\xcd %s \x82\xc5\x82\xb7"/*@ "�`��p�P�`�����l������ 16 �r�b�g�^�J���[�t�H�[�}�b�g�� %s �ł�" @*/, String )) ;
		}

		// �`��\ �P�`�����l�����������_16�r�b�g�^�J���[�t�H�[�}�b�g�̑I��
		if( D3D11Device_CheckFormatSupport( D_DXGI_FORMAT_R16_FLOAT, &FormatSupport ) == S_OK &&
			( FormatSupport & D_D3D11_FORMAT_SUPPORT_TEXTURE2D     ) != 0 &&
			( FormatSupport & D_D3D11_FORMAT_SUPPORT_RENDER_TARGET ) != 0 )
		{
			GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ONE_F16 ] = D_DXGI_FORMAT_R16_FLOAT ;
			String = "DXGI_FORMAT_R16_FLOAT" ;
		}
		else
		if( D3D11Device_CheckFormatSupport( D_DXGI_FORMAT_R16G16_FLOAT, &FormatSupport ) == S_OK &&
			( FormatSupport & D_D3D11_FORMAT_SUPPORT_TEXTURE2D     ) != 0 &&
			( FormatSupport & D_D3D11_FORMAT_SUPPORT_RENDER_TARGET ) != 0 )
		{
			GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ONE_F16 ] = D_DXGI_FORMAT_R16G16_FLOAT ;
			String = "DXGI_FORMAT_R16G16_FLOAT" ;
		}
		else
		if( D3D11Device_CheckFormatSupport( D_DXGI_FORMAT_R16G16B16A16_FLOAT, &FormatSupport ) == S_OK &&
			( FormatSupport & D_D3D11_FORMAT_SUPPORT_TEXTURE2D     ) != 0 &&
			( FormatSupport & D_D3D11_FORMAT_SUPPORT_RENDER_TARGET ) != 0 )
		{
			GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ONE_F16 ] = D_DXGI_FORMAT_R16G16B16A16_FLOAT ;
			String = "DXGI_FORMAT_R16G16B16A16_FLOAT" ;
		}
		else
		if( D3D11Device_CheckFormatSupport( D_DXGI_FORMAT_R32G32B32A32_FLOAT, &FormatSupport ) == S_OK &&
			( FormatSupport & D_D3D11_FORMAT_SUPPORT_TEXTURE2D     ) != 0 &&
			( FormatSupport & D_D3D11_FORMAT_SUPPORT_RENDER_TARGET ) != 0 )
		{
			GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ONE_F16 ] = D_DXGI_FORMAT_R32G32B32A32_FLOAT ;
			String = "DXGI_FORMAT_R32G32B32A32_FLOAT" ;
		}
		else
		{
			DXST_ERRORLOG_ADDA( "\x8e\x67\x97\x70\x82\xc5\x82\xab\x82\xe9\x95\x60\x89\xe6\x97\x70\x82\x50\x83\x60\x83\x83\x83\x93\x83\x6c\x83\x8b\x95\x82\x93\xae\x8f\xac\x90\x94\x93\x5f 16 \x83\x72\x83\x62\x83\x67\x8c\x5e\x83\x4a\x83\x89\x81\x5b\x83\x74\x83\x48\x81\x5b\x83\x7d\x83\x62\x83\x67\x82\xaa\x82\xa0\x82\xe8\x82\xdc\x82\xb9\x82\xf1\x82\xc5\x82\xb5\x82\xbd\n"/*@ "�g�p�ł���`��p�P�`�����l�����������_ 16 �r�b�g�^�J���[�t�H�[�}�b�g������܂���ł���\n" @*/ ) ;
			GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ONE_F16 ] = D_DXGI_FORMAT_UNKNOWN ;
		}

		if( GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ONE_F16 ] != D_DXGI_FORMAT_UNKNOWN )
		{
			DXST_ERRORLOGFMT_ADDA(( "\x95\x60\x89\xe6\x97\x70\x82\x50\x83\x60\x83\x83\x83\x93\x83\x6c\x83\x8b\x95\x82\x93\xae\x8f\xac\x90\x94\x93\x5f 16 \x83\x72\x83\x62\x83\x67\x8c\x5e\x83\x4a\x83\x89\x81\x5b\x83\x74\x83\x48\x81\x5b\x83\x7d\x83\x62\x83\x67\x82\xcd %s \x82\xc5\x82\xb7"/*@ "�`��p�P�`�����l�����������_ 16 �r�b�g�^�J���[�t�H�[�}�b�g�� %s �ł�" @*/, String )) ;
		}

		// �`��\ �P�`�����l�����������_32�r�b�g�^�J���[�t�H�[�}�b�g�̑I��
		if( D3D11Device_CheckFormatSupport( D_DXGI_FORMAT_R32_FLOAT, &FormatSupport ) == S_OK &&
			( FormatSupport & D_D3D11_FORMAT_SUPPORT_TEXTURE2D     ) != 0 &&
			( FormatSupport & D_D3D11_FORMAT_SUPPORT_RENDER_TARGET ) != 0 )
		{
			GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ONE_F32 ] = D_DXGI_FORMAT_R32_FLOAT ;
			String = "DXGI_FORMAT_R32_FLOAT" ;
		}
		else
		if( D3D11Device_CheckFormatSupport( D_DXGI_FORMAT_R32G32_FLOAT, &FormatSupport ) == S_OK &&
			( FormatSupport & D_D3D11_FORMAT_SUPPORT_TEXTURE2D     ) != 0 &&
			( FormatSupport & D_D3D11_FORMAT_SUPPORT_RENDER_TARGET ) != 0 )
		{
			GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ONE_F32 ] = D_DXGI_FORMAT_R32G32_FLOAT ;
			String = "DXGI_FORMAT_R32G32_FLOAT" ;
		}
		else
		if( D3D11Device_CheckFormatSupport( D_DXGI_FORMAT_R32G32B32A32_FLOAT, &FormatSupport ) == S_OK &&
			( FormatSupport & D_D3D11_FORMAT_SUPPORT_TEXTURE2D     ) != 0 &&
			( FormatSupport & D_D3D11_FORMAT_SUPPORT_RENDER_TARGET ) != 0 )
		{
			GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ONE_F32 ] = D_DXGI_FORMAT_R32G32B32A32_FLOAT ;
			String = "DXGI_FORMAT_R32G32B32A32_FLOAT" ;
		}
		else
		if( D3D11Device_CheckFormatSupport( D_DXGI_FORMAT_R16G16B16A16_FLOAT, &FormatSupport ) == S_OK &&
			( FormatSupport & D_D3D11_FORMAT_SUPPORT_TEXTURE2D     ) != 0 &&
			( FormatSupport & D_D3D11_FORMAT_SUPPORT_RENDER_TARGET ) != 0 )
		{
			GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ONE_F32 ] = D_DXGI_FORMAT_R16G16B16A16_FLOAT ;
			String = "DXGI_FORMAT_R16G16B16A16_FLOAT" ;
		}
		else
		{
			DXST_ERRORLOG_ADDA( "\x8e\x67\x97\x70\x82\xc5\x82\xab\x82\xe9\x95\x60\x89\xe6\x97\x70\x82\x50\x83\x60\x83\x83\x83\x93\x83\x6c\x83\x8b\x95\x82\x93\xae\x8f\xac\x90\x94\x93\x5f 32 \x83\x72\x83\x62\x83\x67\x8c\x5e\x83\x4a\x83\x89\x81\x5b\x83\x74\x83\x48\x81\x5b\x83\x7d\x83\x62\x83\x67\x82\xaa\x82\xa0\x82\xe8\x82\xdc\x82\xb9\x82\xf1\x82\xc5\x82\xb5\x82\xbd\n"/*@ "�g�p�ł���`��p�P�`�����l�����������_ 32 �r�b�g�^�J���[�t�H�[�}�b�g������܂���ł���\n" @*/ ) ;
			GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ONE_F32 ] = D_DXGI_FORMAT_UNKNOWN ;
		}

		if( GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ONE_F32 ] != D_DXGI_FORMAT_UNKNOWN )
		{
			DXST_ERRORLOGFMT_ADDA(( "\x95\x60\x89\xe6\x97\x70\x82\x50\x83\x60\x83\x83\x83\x93\x83\x6c\x83\x8b\x95\x82\x93\xae\x8f\xac\x90\x94\x93\x5f 32 \x83\x72\x83\x62\x83\x67\x8c\x5e\x83\x4a\x83\x89\x81\x5b\x83\x74\x83\x48\x81\x5b\x83\x7d\x83\x62\x83\x67\x82\xcd %s \x82\xc5\x82\xb7"/*@ "�`��p�P�`�����l�����������_ 32 �r�b�g�^�J���[�t�H�[�}�b�g�� %s �ł�" @*/, String )) ;
		}

		// �`��\ �Q�`�����l������8�r�b�g�^�J���[�t�H�[�}�b�g�̑I��
		if( D3D11Device_CheckFormatSupport( D_DXGI_FORMAT_R8G8_UNORM, &FormatSupport ) == S_OK &&
			( FormatSupport & D_D3D11_FORMAT_SUPPORT_TEXTURE2D     ) != 0 &&
			( FormatSupport & D_D3D11_FORMAT_SUPPORT_RENDER_TARGET ) != 0 )
		{
			GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_TWO_I8 ] = D_DXGI_FORMAT_R8G8_UNORM ;
			String = "DXGI_FORMAT_R8G8_UNORM" ;
		}
		else
		if( D3D11Device_CheckFormatSupport( D_DXGI_FORMAT_R8G8B8A8_UNORM, &FormatSupport ) == S_OK &&
			( FormatSupport & D_D3D11_FORMAT_SUPPORT_TEXTURE2D     ) != 0 &&
			( FormatSupport & D_D3D11_FORMAT_SUPPORT_RENDER_TARGET ) != 0 )
		{
			GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_TWO_I8 ] = D_DXGI_FORMAT_R8G8B8A8_UNORM ;
			String = "DXGI_FORMAT_R8G8B8A8_UNORM" ;
		}
		else
		{
			DXST_ERRORLOG_ADDA( "\x8e\x67\x97\x70\x82\xc5\x82\xab\x82\xe9\x95\x60\x89\xe6\x97\x70\x82\x51\x83\x60\x83\x83\x83\x93\x83\x6c\x83\x8b\x90\xae\x90\x94 8 \x83\x72\x83\x62\x83\x67\x8c\x5e\x83\x4a\x83\x89\x81\x5b\x83\x74\x83\x48\x81\x5b\x83\x7d\x83\x62\x83\x67\x82\xaa\x82\xa0\x82\xe8\x82\xdc\x82\xb9\x82\xf1\x82\xc5\x82\xb5\x82\xbd\n"/*@ "�g�p�ł���`��p�Q�`�����l������ 8 �r�b�g�^�J���[�t�H�[�}�b�g������܂���ł���\n" @*/ ) ;
			GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_TWO_I8 ] = D_DXGI_FORMAT_UNKNOWN ;
		}

		if( GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_TWO_I8 ] != D_DXGI_FORMAT_UNKNOWN )
		{
			DXST_ERRORLOGFMT_ADDA(( "\x95\x60\x89\xe6\x97\x70\x82\x51\x83\x60\x83\x83\x83\x93\x83\x6c\x83\x8b\x90\xae\x90\x94 8 \x83\x72\x83\x62\x83\x67\x8c\x5e\x83\x4a\x83\x89\x81\x5b\x83\x74\x83\x48\x81\x5b\x83\x7d\x83\x62\x83\x67\x82\xcd %s \x82\xc5\x82\xb7"/*@ "�`��p�Q�`�����l������ 8 �r�b�g�^�J���[�t�H�[�}�b�g�� %s �ł�" @*/, String )) ;
		}

		// �`��\ �Q�`�����l������16�r�b�g�^�J���[�t�H�[�}�b�g�̑I��
		if( D3D11Device_CheckFormatSupport( D_DXGI_FORMAT_R16G16_UNORM, &FormatSupport ) == S_OK &&
			( FormatSupport & D_D3D11_FORMAT_SUPPORT_TEXTURE2D     ) != 0 &&
			( FormatSupport & D_D3D11_FORMAT_SUPPORT_RENDER_TARGET ) != 0 )
		{
			GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_TWO_I16 ] = D_DXGI_FORMAT_R16G16_UNORM ;
			String = "DXGI_FORMAT_R16G16_UNORM" ;
		}
		else
		if( D3D11Device_CheckFormatSupport( D_DXGI_FORMAT_R16G16B16A16_UNORM, &FormatSupport ) == S_OK &&
			( FormatSupport & D_D3D11_FORMAT_SUPPORT_TEXTURE2D     ) != 0 &&
			( FormatSupport & D_D3D11_FORMAT_SUPPORT_RENDER_TARGET ) != 0 )
		{
			GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_TWO_I16 ] = D_DXGI_FORMAT_R16G16B16A16_UNORM ;
			String = "DXGI_FORMAT_R16G16B16A16_UNORM" ;
		}
		else
		{
			DXST_ERRORLOG_ADDA( "\x8e\x67\x97\x70\x82\xc5\x82\xab\x82\xe9\x95\x60\x89\xe6\x97\x70\x82\x51\x83\x60\x83\x83\x83\x93\x83\x6c\x83\x8b\x90\xae\x90\x94 16 \x83\x72\x83\x62\x83\x67\x8c\x5e\x83\x4a\x83\x89\x81\x5b\x83\x74\x83\x48\x81\x5b\x83\x7d\x83\x62\x83\x67\x82\xaa\x82\xa0\x82\xe8\x82\xdc\x82\xb9\x82\xf1\x82\xc5\x82\xb5\x82\xbd\n"/*@ "�g�p�ł���`��p�Q�`�����l������ 16 �r�b�g�^�J���[�t�H�[�}�b�g������܂���ł���\n" @*/ ) ;
			GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_TWO_I16 ] = D_DXGI_FORMAT_UNKNOWN ;
		}

		if( GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_TWO_I16 ] != D_DXGI_FORMAT_UNKNOWN )
		{
			DXST_ERRORLOGFMT_ADDA(( "\x95\x60\x89\xe6\x97\x70\x82\x51\x83\x60\x83\x83\x83\x93\x83\x6c\x83\x8b\x90\xae\x90\x94 16 \x83\x72\x83\x62\x83\x67\x8c\x5e\x83\x4a\x83\x89\x81\x5b\x83\x74\x83\x48\x81\x5b\x83\x7d\x83\x62\x83\x67\x82\xcd %s \x82\xc5\x82\xb7"/*@ "�`��p�Q�`�����l������ 16 �r�b�g�^�J���[�t�H�[�}�b�g�� %s �ł�" @*/, String )) ;
		}

		// �`��\ �Q�`�����l�����������_16�r�b�g�^�J���[�t�H�[�}�b�g�̑I��
		if( D3D11Device_CheckFormatSupport( D_DXGI_FORMAT_R16G16_FLOAT, &FormatSupport ) == S_OK &&
			( FormatSupport & D_D3D11_FORMAT_SUPPORT_TEXTURE2D     ) != 0 &&
			( FormatSupport & D_D3D11_FORMAT_SUPPORT_RENDER_TARGET ) != 0 )
		{
			GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_TWO_F16 ] = D_DXGI_FORMAT_R16G16_FLOAT ;
			String = "DXGI_FORMAT_R16G16_FLOAT" ;
		}
		else
		if( D3D11Device_CheckFormatSupport( D_DXGI_FORMAT_R16G16B16A16_FLOAT, &FormatSupport ) == S_OK &&
			( FormatSupport & D_D3D11_FORMAT_SUPPORT_TEXTURE2D     ) != 0 &&
			( FormatSupport & D_D3D11_FORMAT_SUPPORT_RENDER_TARGET ) != 0 )
		{
			GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_TWO_F16 ] = D_DXGI_FORMAT_R16G16B16A16_FLOAT ;
			String = "DXGI_FORMAT_R16G16B16A16_FLOAT" ;
		}
		else
		if( D3D11Device_CheckFormatSupport( D_DXGI_FORMAT_R32G32B32A32_FLOAT, &FormatSupport ) == S_OK &&
			( FormatSupport & D_D3D11_FORMAT_SUPPORT_TEXTURE2D     ) != 0 &&
			( FormatSupport & D_D3D11_FORMAT_SUPPORT_RENDER_TARGET ) != 0 )
		{
			GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_TWO_F16 ] = D_DXGI_FORMAT_R32G32B32A32_FLOAT ;
			String = "DXGI_FORMAT_R32G32B32A32_FLOAT" ;
		}
		else
		{
			DXST_ERRORLOG_ADDA( "\x8e\x67\x97\x70\x82\xc5\x82\xab\x82\xe9\x95\x60\x89\xe6\x97\x70\x82\x51\x83\x60\x83\x83\x83\x93\x83\x6c\x83\x8b\x95\x82\x93\xae\x8f\xac\x90\x94\x93\x5f 16 \x83\x72\x83\x62\x83\x67\x8c\x5e\x83\x4a\x83\x89\x81\x5b\x83\x74\x83\x48\x81\x5b\x83\x7d\x83\x62\x83\x67\x82\xaa\x82\xa0\x82\xe8\x82\xdc\x82\xb9\x82\xf1\x82\xc5\x82\xb5\x82\xbd\n"/*@ "�g�p�ł���`��p�Q�`�����l�����������_ 16 �r�b�g�^�J���[�t�H�[�}�b�g������܂���ł���\n" @*/ ) ;
			GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_TWO_F16 ] = D_DXGI_FORMAT_UNKNOWN ;
		}

		if( GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_TWO_F16 ] != D_DXGI_FORMAT_UNKNOWN )
		{
			DXST_ERRORLOGFMT_ADDA(( "\x95\x60\x89\xe6\x97\x70\x82\x51\x83\x60\x83\x83\x83\x93\x83\x6c\x83\x8b\x95\x82\x93\xae\x8f\xac\x90\x94\x93\x5f 16 \x83\x72\x83\x62\x83\x67\x8c\x5e\x83\x4a\x83\x89\x81\x5b\x83\x74\x83\x48\x81\x5b\x83\x7d\x83\x62\x83\x67\x82\xcd %s \x82\xc5\x82\xb7"/*@ "�`��p�Q�`�����l�����������_ 16 �r�b�g�^�J���[�t�H�[�}�b�g�� %s �ł�" @*/, String )) ;
		}

		// �`��\ �Q�`�����l�����������_32�r�b�g�^�J���[�t�H�[�}�b�g�̑I��
		if( D3D11Device_CheckFormatSupport( D_DXGI_FORMAT_R32G32_FLOAT, &FormatSupport ) == S_OK &&
			( FormatSupport & D_D3D11_FORMAT_SUPPORT_TEXTURE2D     ) != 0 &&
			( FormatSupport & D_D3D11_FORMAT_SUPPORT_RENDER_TARGET ) != 0 )
		{
			GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_TWO_F32 ] = D_DXGI_FORMAT_R32G32_FLOAT ;
			String = "DXGI_FORMAT_R32G32_FLOAT" ;
		}
		else
		if( D3D11Device_CheckFormatSupport( D_DXGI_FORMAT_R32G32B32A32_FLOAT, &FormatSupport ) == S_OK &&
			( FormatSupport & D_D3D11_FORMAT_SUPPORT_TEXTURE2D     ) != 0 &&
			( FormatSupport & D_D3D11_FORMAT_SUPPORT_RENDER_TARGET ) != 0 )
		{
			GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_TWO_F32 ] = D_DXGI_FORMAT_R32G32B32A32_FLOAT ;
			String = "DXGI_FORMAT_R32G32B32A32_FLOAT" ;
		}
		else
		if( D3D11Device_CheckFormatSupport( D_DXGI_FORMAT_R16G16B16A16_FLOAT, &FormatSupport ) == S_OK &&
			( FormatSupport & D_D3D11_FORMAT_SUPPORT_TEXTURE2D     ) != 0 &&
			( FormatSupport & D_D3D11_FORMAT_SUPPORT_RENDER_TARGET ) != 0 )
		{
			GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_TWO_F32 ] = D_DXGI_FORMAT_R16G16B16A16_FLOAT ;
			String = "DXGI_FORMAT_R16G16B16A16_FLOAT" ;
		}
		else
		{
			DXST_ERRORLOG_ADDA( "\x8e\x67\x97\x70\x82\xc5\x82\xab\x82\xe9\x95\x60\x89\xe6\x97\x70\x82\x51\x83\x60\x83\x83\x83\x93\x83\x6c\x83\x8b\x95\x82\x93\xae\x8f\xac\x90\x94\x93\x5f 32 \x83\x72\x83\x62\x83\x67\x8c\x5e\x83\x4a\x83\x89\x81\x5b\x83\x74\x83\x48\x81\x5b\x83\x7d\x83\x62\x83\x67\x82\xaa\x82\xa0\x82\xe8\x82\xdc\x82\xb9\x82\xf1\x82\xc5\x82\xb5\x82\xbd\n"/*@ "�g�p�ł���`��p�Q�`�����l�����������_ 32 �r�b�g�^�J���[�t�H�[�}�b�g������܂���ł���\n" @*/ ) ;
			GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_TWO_F32 ] = D_DXGI_FORMAT_UNKNOWN ;
		}

		if( GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_TWO_F32 ] != D_DXGI_FORMAT_UNKNOWN )
		{
			DXST_ERRORLOGFMT_ADDA(( "\x95\x60\x89\xe6\x97\x70\x82\x51\x83\x60\x83\x83\x83\x93\x83\x6c\x83\x8b\x95\x82\x93\xae\x8f\xac\x90\x94\x93\x5f 32 \x83\x72\x83\x62\x83\x67\x8c\x5e\x83\x4a\x83\x89\x81\x5b\x83\x74\x83\x48\x81\x5b\x83\x7d\x83\x62\x83\x67\x82\xcd %s \x82\xc5\x82\xb7"/*@ "�`��p�Q�`�����l�����������_ 32 �r�b�g�^�J���[�t�H�[�}�b�g�� %s �ł�" @*/, String )) ;
		}

		// �}�X�N�J���[�o�b�t�@�p�t�H�[�}�b�g�̑I��
		GD3D11.Device.Caps.MaskColorFormat = GSYS.Screen.MainScreenColorBitDepth == 32 ? GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_RGB32 ] : GD3D11.Device.Caps.TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_RGB16 ] ;

		// �}�X�N�A���t�@�t�H�[�}�b�g�̑I��
		GD3D11.Device.Caps.MaskAlphaFormat = D_DXGI_FORMAT_R8_UNORM ;
	}

	// ���o��
	{
		switch( GD3D11.Setting.FeatureLevel )
		{
		case D_D3D_FEATURE_LEVEL_9_1 :
			String = "D3D_FEATURE_LEVEL_9_1" ;
			break ;

		case D_D3D_FEATURE_LEVEL_9_2 :
			String = "D3D_FEATURE_LEVEL_9_2" ;
			break ;

		case D_D3D_FEATURE_LEVEL_9_3 :
			String = "D3D_FEATURE_LEVEL_9_3" ;
			break ;

		case D_D3D_FEATURE_LEVEL_10_0 :
			String = "D3D_FEATURE_LEVEL_10_0" ;
			break ;

		case D_D3D_FEATURE_LEVEL_10_1 :
			String = "D3D_FEATURE_LEVEL_10_1" ;
			break ;

		case D_D3D_FEATURE_LEVEL_11_0 :
			String = "D3D_FEATURE_LEVEL_11_0" ;
			break ;

		case D_D3D_FEATURE_LEVEL_11_1 :
			String = "D3D_FEATURE_LEVEL_11_1" ;
			break ;
		}
		DXST_ERRORLOGFMT_ADDA(( "\x8e\x67\x97\x70\x82\xb7\x82\xe9\x8b\x40\x94\x5c\x83\x8c\x83\x78\x83\x8b:%s"/*@ "�g�p����@�\���x��:%s" @*/, String )) ;
//		DXST_ERRORLOGFMT_ADDA(( "�Ή����Ă���ő咸�_�C���f�b�N�X:%d",     GSYS.HardInfo.MaxVertexIndex )) ;
		DXST_ERRORLOGFMT_ADDA(( "\x93\xaf\x8e\x9e\x82\xc9\x83\x8c\x83\x93\x83\x5f\x83\x8a\x83\x93\x83\x4f\x82\xc5\x82\xab\x82\xe9\x83\x6f\x83\x62\x83\x74\x83\x40\x82\xcc\x90\x94:%d"/*@ "�����Ƀ����_�����O�ł���o�b�t�@�̐�:%d" @*/, GSYS.HardInfo.RenderTargetNum )) ;
		DXST_ERRORLOGFMT_ADDA(( "\x8d\xc5\x91\xe5\x83\x65\x83\x4e\x83\x58\x83\x60\x83\x83\x83\x54\x83\x43\x83\x59\x81\x40\x95\x9d:%d \x8d\x82\x82\xb3:%d"/*@ "�ő�e�N�X�`���T�C�Y�@��:%d ����:%d" @*/, GSYS.HardInfo.MaxTextureWidth, GSYS.HardInfo.MaxTextureHeight )) ;
	}

	// �J���[�t�H�[�}�b�g���̏��������s��
	Graphics_D3D11_GetD3DFormatColorData( D_DXGI_FORMAT_R8G8B8A8_UNORM ) ;

	// �I��
	return 0 ;
}

// Direct3DDevice11 �֌W�̏�����
extern	int		Graphics_D3D11_Device_Initialize( void )
{
	int i ;

#ifndef DX_NON_FILTER
	GraphFilter_Initialize() ;
#endif // DX_NON_FILTER

	// �`��X�e�[�^�X�̃Z�b�g
	{
		GD3D11.Device.DrawSetting.CancelSettingEqualCheck = TRUE ;

		// ���X�^���C�Y�֌W�̏����l���Z�b�g
		GD3D11.Device.State.RasterizerDesc.FillMode              = D_D3D11_FILL_SOLID ;
		GD3D11.Device.State.RasterizerDesc.CullMode              = D_D3D11_CULL_NONE ;
		GD3D11.Device.State.RasterizerDesc.FrontCounterClockwise = 0 ;
		GD3D11.Device.State.RasterizerDesc.DepthBias             = 0 ;
		GD3D11.Device.State.RasterizerDesc.DepthBiasClamp        = 0.0f ;
		GD3D11.Device.State.RasterizerDesc.SlopeScaledDepthBias  = 0.0f ;
		GD3D11.Device.State.RasterizerDesc.DepthClipEnable       = TRUE ;
		GD3D11.Device.State.RasterizerDesc.ScissorEnable         = FALSE ;
		GD3D11.Device.State.RasterizerDesc.MultisampleEnable     = TRUE ;
		GD3D11.Device.State.RasterizerDesc.AntialiasedLineEnable = FALSE ;

		// �[�x�X�e���V���֌W�̏����l���Z�b�g
		GD3D11.Device.State.DepthStencilDesc.DepthEnable					= FALSE ;
		GD3D11.Device.State.DepthStencilDesc.DepthWriteMask					= D_D3D11_DEPTH_WRITE_MASK_ZERO ;
		GD3D11.Device.State.DepthStencilDesc.DepthFunc						= D_D3D11_COMPARISON_LESS_EQUAL ;
		GD3D11.Device.State.DepthStencilDesc.StencilEnable					= FALSE ;
		GD3D11.Device.State.DepthStencilDesc.StencilReadMask				= D_D3D11_DEFAULT_STENCIL_READ_MASK ;
		GD3D11.Device.State.DepthStencilDesc.StencilWriteMask				= D_D3D11_DEFAULT_STENCIL_WRITE_MASK ;
		GD3D11.Device.State.DepthStencilDesc.FrontFace.StencilFailOp		= D_D3D11_STENCIL_OP_KEEP ;
		GD3D11.Device.State.DepthStencilDesc.FrontFace.StencilDepthFailOp	= D_D3D11_STENCIL_OP_KEEP ;
		GD3D11.Device.State.DepthStencilDesc.FrontFace.StencilPassOp		= D_D3D11_STENCIL_OP_KEEP ;
		GD3D11.Device.State.DepthStencilDesc.FrontFace.StencilFunc			= D_D3D11_COMPARISON_ALWAYS ;
		GD3D11.Device.State.DepthStencilDesc.BackFace.StencilFailOp			= D_D3D11_STENCIL_OP_KEEP ;
		GD3D11.Device.State.DepthStencilDesc.BackFace.StencilDepthFailOp	= D_D3D11_STENCIL_OP_KEEP ;
		GD3D11.Device.State.DepthStencilDesc.BackFace.StencilPassOp			= D_D3D11_STENCIL_OP_KEEP ;
		GD3D11.Device.State.DepthStencilDesc.BackFace.StencilFunc			= D_D3D11_COMPARISON_ALWAYS ;

		// �u�����h�X�e�[�g�֌W�̏����l���Z�b�g
		GD3D11.Device.State.BlendDesc.AlphaToCoverageEnable		= FALSE ;
		GD3D11.Device.State.BlendDesc.IndependentBlendEnable	= FALSE ;
		for( i = 0 ; i < 8 ; i ++ )
		{
			GD3D11.Device.State.BlendDesc.RenderTarget[ i ].BlendEnable				= FALSE ;
			GD3D11.Device.State.BlendDesc.RenderTarget[ i ].SrcBlend				= D_D3D11_BLEND_ONE ;
			GD3D11.Device.State.BlendDesc.RenderTarget[ i ].DestBlend				= D_D3D11_BLEND_ZERO ;
			GD3D11.Device.State.BlendDesc.RenderTarget[ i ].BlendOp					= D_D3D11_BLEND_OP_ADD ;
			GD3D11.Device.State.BlendDesc.RenderTarget[ i ].SrcBlendAlpha			= D_D3D11_BLEND_ONE ;
			GD3D11.Device.State.BlendDesc.RenderTarget[ i ].DestBlendAlpha			= D_D3D11_BLEND_ZERO ;
			GD3D11.Device.State.BlendDesc.RenderTarget[ i ].BlendOpAlpha			= D_D3D11_BLEND_OP_ADD ;
			GD3D11.Device.State.BlendDesc.RenderTarget[ i ].RenderTargetWriteMask	= D_D3D11_COLOR_WRITE_ENABLE_ALL ;
		}

		// �T���v���[�X�e�[�g�֌W�̏����l���Z�b�g
		for( i = 0 ; i < USE_TEXTURESTAGE_NUM ; i ++ )
		{
			GD3D11.Device.State.SamplerDesc[ i ].Filter				= D_D3D11_FILTER_MIN_MAG_MIP_POINT ;
			GD3D11.Device.State.SamplerDesc[ i ].AddressU			= D_D3D11_TEXTURE_ADDRESS_CLAMP ;
			GD3D11.Device.State.SamplerDesc[ i ].AddressV			= D_D3D11_TEXTURE_ADDRESS_CLAMP ;
			GD3D11.Device.State.SamplerDesc[ i ].AddressW			= D_D3D11_TEXTURE_ADDRESS_CLAMP ;
			GD3D11.Device.State.SamplerDesc[ i ].MipLODBias			= 0.0f ;
			GD3D11.Device.State.SamplerDesc[ i ].MaxAnisotropy		= 16 ;
			GD3D11.Device.State.SamplerDesc[ i ].ComparisonFunc		= D_D3D11_COMPARISON_ALWAYS ;
			GD3D11.Device.State.SamplerDesc[ i ].BorderColor[ 0 ]	= 0.0f ;
			GD3D11.Device.State.SamplerDesc[ i ].BorderColor[ 1 ]	= 0.0f ;
			GD3D11.Device.State.SamplerDesc[ i ].BorderColor[ 2 ]	= 0.0f ;
			GD3D11.Device.State.SamplerDesc[ i ].BorderColor[ 3 ]	= 0.0f ;
			GD3D11.Device.State.SamplerDesc[ i ].MinLOD				= -D_D3D11_FLOAT32_MAX ;
			GD3D11.Device.State.SamplerDesc[ i ].MaxLOD				=  D_D3D11_FLOAT32_MAX ;
		}

		// �e�N�X�`���A�h���X���[�h�ݒ�
		for( i = 0 ; i < USE_TEXTURESTAGE_NUM ; i ++ )
		{
			if( GSYS.DrawSetting.TexAddressModeU[ i ] == 0 )
			{
				GSYS.DrawSetting.TexAddressModeU[ i ] = D_D3D11_TEXTURE_ADDRESS_CLAMP ;
			}

			if( GSYS.DrawSetting.TexAddressModeV[ i ] == 0 )
			{
				GSYS.DrawSetting.TexAddressModeV[ i ] = D_D3D11_TEXTURE_ADDRESS_CLAMP ;
			}

			if( GSYS.DrawSetting.TexAddressModeW[ i ] == 0 )
			{
				GSYS.DrawSetting.TexAddressModeW[ i ] = D_D3D11_TEXTURE_ADDRESS_CLAMP ;
			}
		}

		// �`�惂�[�h���Z�b�g
		Graphics_D3D11_DeviceState_SetDrawMode( DX_DRAWMODE_NEAREST ) ;

		// �ő�ٕ������Z�b�g
		Graphics_D3D11_DeviceState_SetMaxAnisotropy( 16 ) ;

		// �e�N�X�`�����W�ϊ��s����g�p���邩�ǂ����̐ݒ���Z�b�g
		Graphics_D3D11_DeviceState_SetTextureAddressTransformMatrix(
			FALSE,
			&D3D11_GlobalIdentMatrix ) ;

		// ���C�e�B���O�n�m
		Graphics_D3D11_DeviceState_SetLighting( TRUE ) ;

		// �}�e���A���̃p�����[�^��ݒ�
		{
			MATERIALPARAM Material ;
			Material.Diffuse.r  = 0.8f ;
			Material.Diffuse.g  = 0.8f ;
			Material.Diffuse.b  = 0.8f ;
			Material.Diffuse.a  = 1.0f ;
			Material.Specular.r = 0.8f ;
			Material.Specular.g = 0.8f ;
			Material.Specular.b = 0.8f ;
			Material.Specular.a = 0.0f ;
			Material.Emissive.r = 0.0f ;
			Material.Emissive.g = 0.0f ;
			Material.Emissive.b = 0.0f ;
			Material.Emissive.a = 0.0f ;
			Material.Ambient.r  = 0.0f ;
			Material.Ambient.g  = 0.0f ;
			Material.Ambient.b  = 0.0f ;
			Material.Ambient.a  = 0.0f ;
			Material.Power      = 20.0f ;
			Graphics_D3D11_DeviceState_SetMaterial( &Material ) ;
		}

		// ���_�̃J���[�l���}�e���A���̃f�B�t���[�Y�J���[��X�y�L�����J���[�Ƃ��Ďg�p���邩�ǂ����̐ݒ��������
		Graphics_D3D11_DeviceState_SetUseVertexDiffuseColor( TRUE ) ;
		Graphics_D3D11_DeviceState_SetUseVertexSpecularColor( TRUE ) ;

		// �X�y�L�����������g�p���邩�ǂ�����������
		Graphics_D3D11_DeviceState_SetSpecularEnable( TRUE ) ;

		// �t�H�O�֌W�̐ݒ�
		Graphics_D3D11_DeviceState_SetFogEnable( FALSE ) ;
		Graphics_D3D11_DeviceState_SetFogColor( 0 ) ;
		Graphics_D3D11_DeviceState_SetFogVertexMode( DX_FOGMODE_NONE ) ;
		Graphics_D3D11_DeviceState_SetFogStartEnd( 0.0f, 1.0f ) ;
		Graphics_D3D11_DeviceState_SetFogDensity( 1.0f ) ;

		// �O���[�o���A���r�G���g�J���[�Z�b�g
		{
			COLOR_F GAmbColor = { 0.0f } ;

			Graphics_D3D11_DeviceState_SetAmbient( &GAmbColor ) ;
		}

		// �r���[�|�[�g��ݒ�
		{
			D_D3D11_VIEWPORT Viewport ;

			Viewport.TopLeftX	= 0 ;
			Viewport.TopLeftY	= 0 ;
			Viewport.Width		= ( FLOAT )GSYS.Screen.MainScreenSizeX ;
			Viewport.Height		= ( FLOAT )GSYS.Screen.MainScreenSizeY ;
			Viewport.MinDepth	= 0.0f ;
			Viewport.MaxDepth	= 1.0f ;
			Graphics_D3D11_DeviceState_SetViewport( &Viewport ) ;
		}

		// �`��摜�̐F�𖳎����邩�ǂ������Z�b�g
		Graphics_D3D11_DrawSetting_SetIgnoreDrawGraphColor( FALSE ) ;

		// �`���ݒ�
		Graphics_D3D11_DeviceState_SetRenderTarget(
			GD3D11.Device.Screen.OutputWindowInfo[ 0 ].BufferTexture2D,
			GD3D11.Device.Screen.OutputWindowInfo[ 0 ].BufferRTV
		) ;

		// �u�����h���[�h�ݒ�
		Graphics_D3D11_DeviceState_SetBlendMode( DX_BLENDMODE_NOBLEND ) ;

		// �v���~�e�B�u�^�C�v���Z�b�g
		Graphics_D3D11_DeviceState_SetPrimitiveTopology( D_D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST ) ;

		// �A���t�@�e�X�g�ݒ��������
		Graphics_D3D11_DrawSetting_SetDrawAlphaTest( -1, 0 ) ;

		// �u�����h�e�N�X�`���֌W��������
		Graphics_D3D11_DrawSetting_SetBlendTexture( NULL, NULL ) ;
		GD3D11.Device.DrawSetting.BlendGraphBorderParam	= -1 ;
		GD3D11.Device.DrawSetting.BlendGraphType		= 0 ;

		// �`��e�N�X�`���֌W��������
		Graphics_D3D11_DrawSetting_SetTexture( NULL, NULL ) ;
		GD3D11.Device.DrawSetting.AlphaChannelValidFlag	= FALSE ;
		GD3D11.Device.DrawSetting.AlphaTestValidFlag	= FALSE ;

		GD3D11.Device.DrawSetting.CancelSettingEqualCheck = FALSE ;
	}

	// �f�o�C�X�ݒ�����t���b�V��
	Graphics_D3D11_DeviceState_RefreshRenderState() ;

	// �t���O��������
	GD3D11.Device.DrawSetting.DrawPrepAlwaysFlag	= TRUE ;
	GD3D11.Device.DrawSetting.ChangeBlendParamFlag	= TRUE ;
	GD3D11.Device.DrawSetting.ChangeTextureFlag		= FALSE ;

	// �`�����������
	GD3D11.Device.DrawInfo.DiffuseColor				= 0xffffffff ;
	GD3D11.Device.DrawInfo.BlendMaxNotDrawFlag		= FALSE ;
	GD3D11.Device.DrawInfo.BeginSceneFlag			= FALSE ;
	GD3D11.Device.DrawInfo.VertexNum				= 0 ;
	GD3D11.Device.DrawInfo.Use3DVertex				= 0 ;
	GD3D11.Device.DrawInfo.VertexType				= VERTEXTYPE_TEX ;

#if VERTEXBUFFERMODE == 2
	NORMALVERTEXBUFFER_MAP
	GD3D11.Device.DrawInfo.VertexBufferNextAddr		= GD3D11.Device.DrawInfo.VertexBufferAddr ;
#else
	GD3D11.Device.DrawInfo.VertexBufferNextAddr		= GD3D11.Device.DrawInfo.VertexBufferTemp ;
#endif

	// �I��
	return 0 ;
}

// �ꎞ�I�� Direct3DDevice11 ��j�������ۂɁA�j���O�̏�Ԃ����o�����߂̊֐�
extern	int		Graphics_D3D11_Device_ReInitialize( void )
{
	int i ;

	if( GAPIWin.D3D11DeviceObject == NULL )
	{
		return -1 ;
	}

	// �������t���O�𗧂Ă�
	GD3D11.Device.DrawSetting.CancelSettingEqualCheck = TRUE ;

	// �K�� Graphics_D3D9_DrawPreparation �����s����t���O�𗧂Ă�
	GD3D11.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

	// �폜�O�Ɠ����������o��
	{
		// �`��e�N�X�`���֌W��������
		Graphics_D3D11_DrawSetting_SetTexture( NULL, NULL ) ;
		GD3D11.Device.DrawSetting.AlphaChannelValidFlag	= FALSE ;
		GD3D11.Device.DrawSetting.AlphaTestValidFlag	= FALSE ;

		// �`�撸�_�������Z�b�g
		GD3D11.Device.DrawInfo.VertexNum = 0 ;

		// �`��u�����h���[�h�̐ݒ�
		Graphics_D3D11_DrawSetting_SetDrawBlendMode( GD3D11.Device.DrawSetting.BlendMode, GD3D11.Device.DrawSetting.AlphaTestValidFlag, GD3D11.Device.DrawSetting.AlphaChannelValidFlag ) ;

		// �`��摜�̐F�𖳎����邩�ǂ������Z�b�g����
		Graphics_D3D11_DrawSetting_SetIgnoreDrawGraphColor( GD3D11.Device.DrawSetting.IgnoreGraphColorFlag ) ;

		// �ٕ����t�B���^�����O�̐ݒ���Z�b�g
		Graphics_D3D11_DeviceState_SetMaxAnisotropy( GD3D11.Device.State.MaxAnisotropy ) ;

		// UV�A�h���b�V���O���[�h�̐ݒ���Z�b�g
		for( i = 0 ; i < USE_TEXTURESTAGE_NUM ; i ++ )
		{
			Graphics_D3D11_DeviceState_SetTextureAddressUVW(
				GD3D11.Device.State.SamplerDesc[ i ].AddressU,
				GD3D11.Device.State.SamplerDesc[ i ].AddressV,
				GD3D11.Device.State.SamplerDesc[ i ].AddressW,
				i ) ;
		}

		// �e�N�X�`�����W�ϊ��s����g�p���邩�ǂ����̐ݒ���Z�b�g
		Graphics_D3D11_DeviceState_SetTextureAddressTransformMatrix(
			GD3D11.Device.State.TextureAddressTransformMatrixUse,
			&GD3D11.Device.State.TextureAddressTransformMatrix ) ;

		// �v���~�e�B�u�^�C�v���Z�b�g
		Graphics_D3D11_DeviceState_SetPrimitiveTopology( GD3D11.Device.State.PrimitiveTopology ) ;

		// �t�H�O�̐ݒ���Z�b�g
		Graphics_D3D11_DeviceState_SetFogEnable(     GD3D11.Device.State.FogEnable ) ;
		Graphics_D3D11_DeviceState_SetFogVertexMode( GD3D11.Device.State.FogMode ) ;
		Graphics_D3D11_DeviceState_SetFogColor(      GD3D11.Device.State.FogColor ) ;
		Graphics_D3D11_DeviceState_SetFogStartEnd(   GD3D11.Device.State.FogStart, GD3D11.Device.State.FogEnd ) ;
		Graphics_D3D11_DeviceState_SetFogDensity(    GD3D11.Device.State.FogDensity ) ;

		// �s��̍Đݒ�
		NS_SetTransformToWorldD(                              &GSYS.DrawSetting.WorldMatrix ) ;
		NS_SetTransformToViewD(                               &GSYS.DrawSetting.ViewMatrix ) ;
		Graphics_DrawSetting_SetTransformToProjection_Direct( &GSYS.DrawSetting.ProjectionMatrix ) ;
		NS_SetTransformToViewportD(                           &GSYS.DrawSetting.ViewportMatrix ) ;

		// ���C�g�̍Đݒ�
		Graphics_D3D11_DeviceState_SetAmbient( &GD3D11.Device.State.GlobalAmbientLightColor ) ;
		Graphics_D3D11_DeviceState_SetLighting( GD3D11.Device.State.Lighting ) ;
		for( i = 0 ; i < DX_D3D11_COMMON_CONST_LIGHT_NUM ; i ++ )
		{
			if( GD3D11.Device.State.LightParam[ i ].LightType == 0 )
			{
				continue ;
			}

			Graphics_D3D11_DeviceState_SetLightState( i, &GD3D11.Device.State.LightParam[ i ] ) ;
			Graphics_D3D11_DeviceState_SetLightEnable( i, GD3D11.Device.State.LightEnableFlag[ i ] ) ;
		}
	}

	// �������t���O��|��
	GD3D11.Device.DrawSetting.CancelSettingEqualCheck = FALSE ;

	// ��L�ȊO�̐ݒ�����t���b�V��
	Graphics_D3D11_DeviceState_RefreshRenderState() ;

	// �I��
	return 0 ;
}



























// DXGIAdapter �֌W

// DXGI�I�u�W�F�N�g���̗�
extern int EnumDXGIAdapterInfo( void )
{
	D_IDXGIAdapter *DXGIAdapter ;
	D_IDXGIOutput  *DXGIOutput ;
	int            AfterRelease = FALSE ;
	GRAPHICS_HARDWARE_DIRECT3D11_ADAPTER_UNIT *AdapterInfo ;

	if( GD3D11.Adapter.ValidInfo )
	{
		return 0 ;
	}

	if( DXGIFactory_IsValid() == FALSE )
	{
		AfterRelease = TRUE ;

		// dxgi.DLL �̓ǂݍ���
		if( DXGI_LoadDLL() < 0 )
		{
			return -1 ;
		}

		// IDXGIFactory �̍쐬
		if( DXGIFactory_Create() < 0 )
		{
			return -1 ;
		}
	}

	AdapterInfo = GD3D11.Adapter.Info ;
	for( GD3D11.Adapter.InfoNum = 0 ; GD3D11.Adapter.InfoNum < DX_D3D11_MAX_ADAPTER_LISTUP ; GD3D11.Adapter.InfoNum ++, AdapterInfo ++ )
	{
		if( DXGIFactory_EnumAdapters( ( UINT )GD3D11.Adapter.InfoNum, &DXGIAdapter ) != S_OK )
		{
			break ;
		}

		DXGIAdapter_GetDesc( DXGIAdapter, &AdapterInfo->Desc ) ;

		for( AdapterInfo->OutputNum = 0 ; AdapterInfo->OutputNum < DX_D3D11_MAX_OUTPUT_LISTUP ; AdapterInfo->OutputNum ++ )
		{
			if( DXGIAdapter_EnumOutputs( DXGIAdapter, ( UINT )AdapterInfo->OutputNum, &DXGIOutput ) != S_OK )
			{
				break ;
			}

			DXGIOutput_GetDesc( DXGIOutput, &AdapterInfo->OutputDesc[ AdapterInfo->OutputNum ] ) ;

			Direct3D11_ObjectRelease( DXGIOutput ) ;
		}

		Direct3D11_ObjectRelease( DXGIAdapter ) ;
	}

	if( AfterRelease )
	{
		// IDXGIFactory �̉��
		DXGIFactory_Release() ;

		// dxgi.dll �̉��
		DXGI_FreeDLL() ;
	}

	GD3D11.Adapter.ValidInfo = TRUE ;

	// ����I��
	return 0 ;
}


































// D3D11Device �X�e�[�^�X�֌W

// �`��ݒ�����t���b�V��
extern	void	Graphics_D3D11_DeviceState_RefreshRenderState( void )
{
	if( GAPIWin.D3D11DeviceObject == NULL ) return ;

	// �������t���O�𗧂Ă�
	GD3D11.Device.DrawSetting.CancelSettingEqualCheck = TRUE ;

	// Graphics_D3D11_DrawPreparation ���s���ׂ��t���O�𗧂Ă�
	GD3D11.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

	// �ݒ�̃��t���b�V��
	Graphics_D3D11_DeviceState_SetFogEnable             ( GD3D11.Device.State.FogEnable           ) ;
	Graphics_D3D11_DeviceState_SetViewport              ( &GD3D11.Device.State.Viewport           ) ;
//	Graphics_D3D11_DeviceState_SetScissorRect           ( &GD3D11.Device.State.ScissorRect        ) ;
	Graphics_D3D11_DrawSetting_SetTexture               ( GD3D11.Device.DrawSetting.RenderTexture, GD3D11.Device.DrawSetting.RenderTextureSRV ) ;
	Graphics_D3D11_DeviceState_SetBaseState             (                                        ) ;
	Graphics_D3D11_DeviceState_SetDrawMode              ( GD3D11.Device.State.DrawMode            ) ;
//	Graphics_D3D11_DrawSetting_SetDrawBlendMode         ( GD3D11.Device.DrawSetting.BlendMode, GD3D11.Device.DrawSetting.AlphaChannelValidFlag  ) ;
	Graphics_D3D11_DeviceState_NormalDrawSetup          (                                        ) ;
	Graphics_D3D11_DeviceState_SetDepthEnable           ( GD3D11.Device.State.DepthStencilDesc.DepthEnable ) ;
	Graphics_D3D11_DeviceState_SetDepthWriteEnable      ( GD3D11.Device.State.DepthStencilDesc.DepthWriteMask == D_D3D11_DEPTH_WRITE_MASK_ALL ? TRUE : FALSE ) ;
	Graphics_D3D11_DeviceState_SetDepthFunc             ( GD3D11.Device.State.DepthStencilDesc.DepthFunc ) ;
	Graphics_D3D11_DeviceState_SetDepthBias             ( GD3D11.Device.State.RasterizerDesc.DepthBias ) ;
	Graphics_D3D11_DeviceState_SetFillMode              ( GD3D11.Device.State.RasterizerDesc.FillMode  ) ;
	Graphics_D3D11_DeviceState_SetCullMode              ( GD3D11.Device.State.RasterizerDesc.CullMode == D_D3D11_CULL_FRONT ? 2 : ( GD3D11.Device.State.RasterizerDesc.CullMode == D_D3D11_CULL_BACK ? 1 : 0 ) ) ;
//	Graphics_D3D11_DeviceState_SetShadeMode             ( GD3D11.Device.State.ShadeMode           ) ;
	Graphics_D3D11_DeviceState_SetMaterial              ( &GD3D11.Device.State.Material           ) ;
	Graphics_D3D11_DeviceState_SetSpecularEnable        ( GD3D11.Device.State.UseSpecular         ) ;
	Graphics_D3D11_DeviceState_SetUseVertexDiffuseColor ( GD3D11.Device.State.MaterialUseVertexDiffuseColor ) ;
	Graphics_D3D11_DeviceState_SetUseVertexSpecularColor( GD3D11.Device.State.MaterialUseVertexSpecularColor ) ;
	Graphics_D3D11_DeviceState_SetPrimitiveTopology     ( GD3D11.Device.State.PrimitiveTopology   ) ;
	Graphics_D3D11_DeviceState_SetInputLayout			( GD3D11.Device.State.SetVertexInputLayout ) ;
	Graphics_D3D11_DeviceState_SetVertexShader			( GD3D11.Device.State.SetVertexShader, GD3D11.Device.State.SetNormalVertexShader ) ;
	Graphics_D3D11_DeviceState_SetIndexBuffer			( GD3D11.Device.State.SetIndexBuffer, GD3D11.Device.State.SetIndexBufferFormat ) ;
	Graphics_D3D11_DeviceState_SetPixelShader			( GD3D11.Device.State.SetPixelShader, GD3D11.Device.State.SetNormalPixelShader ) ;
	Graphics_D3D11_DeviceState_SetVertexBuffer			( GD3D11.Device.State.SetVertexBuffer, GD3D11.Device.State.SetVertexBufferStride ) ;

	// �萔�o�b�t�@�����t���b�V��
	GD3D11.Device.Shader.Constant.ConstBuffer_Common->ChangeFlag = TRUE ;
	Graphics_D3D11_ConstantBuffer_Update( GD3D11.Device.Shader.Constant.ConstBuffer_Common ) ;

	GD3D11.Device.Shader.Constant.ConstBuffer_VS_Base->ChangeFlag = TRUE ;
	Graphics_D3D11_ConstantBuffer_Update( GD3D11.Device.Shader.Constant.ConstBuffer_VS_Base ) ;

	GD3D11.Device.Shader.Constant.ConstBuffer_VS_OtherMatrix->ChangeFlag = TRUE ;
	Graphics_D3D11_ConstantBuffer_Update( GD3D11.Device.Shader.Constant.ConstBuffer_VS_OtherMatrix ) ;

	GD3D11.Device.Shader.Constant.ConstBuffer_VS_LocalWorldMatrix->ChangeFlag = TRUE ;
	Graphics_D3D11_ConstantBuffer_Update( GD3D11.Device.Shader.Constant.ConstBuffer_VS_LocalWorldMatrix ) ;

	GD3D11.Device.Shader.Constant.ConstBuffer_PS_Base->ChangeFlag = TRUE ;
	Graphics_D3D11_ConstantBuffer_Update( GD3D11.Device.Shader.Constant.ConstBuffer_PS_Base ) ;

	GD3D11.Device.Shader.Constant.ConstBuffer_PS_ShadowMap->ChangeFlag = TRUE ;
	Graphics_D3D11_ConstantBuffer_Update( GD3D11.Device.Shader.Constant.ConstBuffer_PS_ShadowMap ) ;

	GD3D11.Device.Shader.Constant.ConstBuffer_PS_Filter->ChangeFlag = TRUE ;
	Graphics_D3D11_ConstantBuffer_Update( GD3D11.Device.Shader.Constant.ConstBuffer_PS_Filter ) ;

	// �萔�o�b�t�@���Z�b�g
	Graphics_D3D11_ConstantBuffer_VSSet( DX_D3D11_VS_CONSTANTBUFFER_COMMON,      1, &GD3D11.Device.Shader.Constant.ConstBuffer_Common ) ;
	Graphics_D3D11_ConstantBuffer_VSSet( DX_D3D11_VS_CONSTANTBUFFER_BASE,        1, &GD3D11.Device.Shader.Constant.ConstBuffer_VS_Base ) ;
	Graphics_D3D11_ConstantBuffer_VSSet( DX_D3D11_VS_CONSTANTBUFFER_OTHERMATRIX, 1, &GD3D11.Device.Shader.Constant.ConstBuffer_VS_OtherMatrix ) ;
	Graphics_D3D11_ConstantBuffer_VSSet( DX_D3D11_VS_CONSTANTBUFFER_MATRIX,      1, &GD3D11.Device.Shader.Constant.ConstBuffer_VS_LocalWorldMatrix ) ;

	Graphics_D3D11_ConstantBuffer_PSSet( DX_D3D11_PS_CONSTANTBUFFER_COMMON,      1, &GD3D11.Device.Shader.Constant.ConstBuffer_Common ) ;
	Graphics_D3D11_ConstantBuffer_PSSet( DX_D3D11_PS_CONSTANTBUFFER_BASE,        1, &GD3D11.Device.Shader.Constant.ConstBuffer_PS_Base ) ;
	Graphics_D3D11_ConstantBuffer_PSSet( DX_D3D11_PS_CONSTANTBUFFER_SHADOWMAP,   1, &GD3D11.Device.Shader.Constant.ConstBuffer_PS_ShadowMap ) ;
	Graphics_D3D11_ConstantBuffer_PSSet( DX_D3D11_PS_CONSTANTBUFFER_FILTER,      1, &GD3D11.Device.Shader.Constant.ConstBuffer_PS_Filter ) ;

	// �������t���O��|��
	GD3D11.Device.DrawSetting.CancelSettingEqualCheck = FALSE ;
}

// Direct3DDevice11 �̊�{�ݒ���s��
extern int Graphics_D3D11_DeviceState_SetBaseState( void )
{
	if( GAPIWin.D3D11DeviceObject == NULL )
	{
		return -1 ;
	}

	// �����o��
	DRAWSTOCKINFO

	// �g�p���Ȃ��V�F�[�_�[�𖳌���
	D3D11DeviceContext_GSSetShader( NULL, NULL, 0 ) ;
	D3D11DeviceContext_HSSetShader( NULL, NULL, 0 ) ;
	D3D11DeviceContext_DSSetShader( NULL, NULL, 0 ) ;
	D3D11DeviceContext_CSSetShader( NULL, NULL, 0 ) ;

	// �T���v���[�ݒ�ăZ�b�g�A�b�v
	Graphics_D3D11_DeviceState_SetupSamplerState() ;

	// ���X�^���C�U�X�e�[�g�ăZ�b�g�A�b�v
	Graphics_D3D11_DeviceState_SetupRasterizerState() ;

	// �[�x�X�e���V���X�e�[�g�ăZ�b�g�A�b�v
	Graphics_D3D11_DeviceState_SetupDepthStencilState() ;

	// �u�����h�X�e�[�g�ăZ�b�g�A�b�v
	Graphics_D3D11_DeviceState_SetupBlendState() ;

	// �I��
	return 0 ;
}

// �T���v���[�̃e�N�X�`���t�B���^�����O���[�h��ݒ肷��
extern int Graphics_D3D11_DeviceState_SetSampleFilterMode( D_D3D11_FILTER Filter, int Sampler )
{
	int						i ;
	D_D3D11_SAMPLER_DESC	*SamplerDesc ;

	if( GAPIWin.D3D11DeviceObject == NULL )
	{
		return -1 ;
	}

	if( Sampler < 0 )
	{
		if( Filter == D_D3D11_FILTER_MIN_MAG_MIP_LINEAR ||
			Filter == D_D3D11_FILTER_MIN_MAG_MIP_POINT ||
			Filter == D_D3D11_FILTER_ANISOTROPIC )
		{
			GD3D11.Device.State.DrawMode = -1 ;
		}

		if( GD3D11.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
		{
			SamplerDesc = GD3D11.Device.State.SamplerDesc ;
			for( i = 0 ; i < USE_TEXTURESTAGE_NUM ; i ++, SamplerDesc ++ )
			{
				if( SamplerDesc->Filter != Filter )
				{
					break ;
				}
			}
			if( i == USE_TEXTURESTAGE_NUM )
			{
				return 0 ;
			}
		}

		DRAWSTOCKINFO

		SamplerDesc = GD3D11.Device.State.SamplerDesc ;
		for( i = 0 ; i < USE_TEXTURESTAGE_NUM ; i ++, SamplerDesc ++ )
		{
			if( SamplerDesc->Filter == Filter &&
				GD3D11.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
			{
				continue ;
			}

			GD3D11.Device.State.ChangeSamplerDescSlot[ i ] = TRUE ;
			GD3D11.Device.State.ChangeSamplerDesc = TRUE ;

			SamplerDesc->Filter = Filter ;
		}
	}
	else
	{
		if( Sampler < 0 || Sampler >= USE_TEXTURESTAGE_NUM )
		{
			return 0 ;
		}

		GD3D11.Device.State.DrawMode = -1 ;

		SamplerDesc = &GD3D11.Device.State.SamplerDesc[ Sampler ] ;
		if( SamplerDesc->Filter == Filter &&
			GD3D11.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
		{
			return 0 ;
		}

		DRAWSTOCKINFO

		GD3D11.Device.State.ChangeSamplerDescSlot[ Sampler ] = TRUE ;
		GD3D11.Device.State.ChangeSamplerDesc = TRUE ;

		SamplerDesc->Filter = Filter ;
	}

	GD3D11.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

//	return Graphics_D3D11_DeviceState_SetupSamplerState() ;
	return 0 ;
}

// �[�x�o�b�t�@�̗L�������̃Z�b�g
extern int Graphics_D3D11_DeviceState_SetDepthEnable( BOOL DepthEnable )
{
	if( GAPIWin.D3D11DeviceObject == NULL )
	{
		return -1 ;
	}

	// ���[�h�������������牽�������I��
	if( GD3D11.Device.State.DepthStencilDesc.DepthEnable == DepthEnable &&
		GD3D11.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
	{
		return 0 ;
	}
	
	DRAWSTOCKINFO

	GD3D11.Device.State.ChangeDepthStencilDesc = TRUE ;

	GD3D11.Device.State.DepthStencilDesc.DepthEnable = DepthEnable ;

	GD3D11.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

//	return Graphics_D3D11_DeviceState_SetupDepthStencilState() ;
	return 0 ;
}

// �[�x�o�b�t�@�̏������݂̗L�����Z�b�g
extern int Graphics_D3D11_DeviceState_SetDepthWriteEnable( int Flag )
{
	D_D3D11_DEPTH_WRITE_MASK DepthWriteMask ;

	if( GAPIWin.D3D11DeviceObject == NULL )
	{
		return -1 ;
	}

	DepthWriteMask = Flag == FALSE ? D_D3D11_DEPTH_WRITE_MASK_ZERO : D_D3D11_DEPTH_WRITE_MASK_ALL ;

	// ���[�h�������������牽�������I��
	if( GD3D11.Device.State.DepthStencilDesc.DepthWriteMask == DepthWriteMask &&
		GD3D11.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
	{
		return 0 ;
	}
	
	DRAWSTOCKINFO

	GD3D11.Device.State.ChangeDepthStencilDesc = TRUE ;

	GD3D11.Device.State.DepthStencilDesc.DepthWriteMask = DepthWriteMask ;

	GD3D11.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

//	return Graphics_D3D11_DeviceState_SetupDepthStencilState() ;
	return 0 ;
}

// �[�x�l�̔�r�^�C�v���Z�b�g
extern int Graphics_D3D11_DeviceState_SetDepthFunc( D_D3D11_COMPARISON_FUNC DepthFunc )
{
	if( GAPIWin.D3D11DeviceObject == NULL )
	{
		return -1 ;
	}

	// ���[�h�������������牽�������I��
	if( GD3D11.Device.State.DepthStencilDesc.DepthFunc == DepthFunc &&
		GD3D11.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
	{
		return 0 ;
	}
	
	DRAWSTOCKINFO

	GD3D11.Device.State.ChangeDepthStencilDesc = TRUE ;

	GD3D11.Device.State.DepthStencilDesc.DepthFunc = DepthFunc ;

	GD3D11.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

//	return Graphics_D3D11_DeviceState_SetupDepthStencilState() ;
	return 0 ;
}

// �[�x�l�̃o�C�A�X���Z�b�g
extern int Graphics_D3D11_DeviceState_SetDepthBias( int DepthBias )
{
	if( GAPIWin.D3D11DeviceObject == NULL )
	{
		return -1 ;
	}

	// ���[�h�������������牽�������I��
	if( GD3D11.Device.State.RasterizerDesc.DepthBias == DepthBias &&
		GD3D11.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
	{
		return 0 ;
	}
	
	DRAWSTOCKINFO

	GD3D11.Device.State.ChangeRasterizerDesc = TRUE ;

	GD3D11.Device.State.RasterizerDesc.DepthBias = DepthBias ;

	GD3D11.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

//	return Graphics_D3D11_DeviceState_SetupRasterizerState() ;
	return 0 ;
}

// �t�B�����[�h���Z�b�g
extern int Graphics_D3D11_DeviceState_SetFillMode( D_D3D11_FILL_MODE FillMode )
{
	if( GAPIWin.D3D11DeviceObject == NULL )
	{
		return -1 ;
	}

	// ���[�h�������������牽�������I��
	if( GD3D11.Device.State.RasterizerDesc.FillMode == FillMode &&
		GD3D11.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
	{
		return 0 ;
	}
	
	DRAWSTOCKINFO

	GD3D11.Device.State.ChangeRasterizerDesc = TRUE ;

	GD3D11.Device.State.RasterizerDesc.FillMode = FillMode ;

	GD3D11.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

//	return Graphics_D3D11_DeviceState_SetupRasterizerState() ;
	return 0 ;
}

// ���[���h�ϊ��p�s����Z�b�g����
extern int Graphics_D3D11_DeviceState_SetWorldMatrix( const MATRIX *Matrix )
{
	DX_D3D11_VS_CONST_BUFFER_BASE *ConstantVSBase ;

	if( GAPIWin.D3D11DeviceObject == NULL )
	{
		return -1 ;
	}

	ConstantVSBase = ( DX_D3D11_VS_CONST_BUFFER_BASE * )GD3D11.Device.Shader.Constant.ConstBuffer_VS_Base->SysmemBuffer ;

	// �萔�f�[�^�ɔ��f
	ConstantVSBase->LocalWorldMatrix[ 0 ][ 0 ] = Matrix->m[ 0 ][ 0 ] ;
	ConstantVSBase->LocalWorldMatrix[ 0 ][ 1 ] = Matrix->m[ 1 ][ 0 ] ;
	ConstantVSBase->LocalWorldMatrix[ 0 ][ 2 ] = Matrix->m[ 2 ][ 0 ] ;
	ConstantVSBase->LocalWorldMatrix[ 0 ][ 3 ] = Matrix->m[ 3 ][ 0 ] ;
	ConstantVSBase->LocalWorldMatrix[ 1 ][ 0 ] = Matrix->m[ 0 ][ 1 ] ;
	ConstantVSBase->LocalWorldMatrix[ 1 ][ 1 ] = Matrix->m[ 1 ][ 1 ] ;
	ConstantVSBase->LocalWorldMatrix[ 1 ][ 2 ] = Matrix->m[ 2 ][ 1 ] ;
	ConstantVSBase->LocalWorldMatrix[ 1 ][ 3 ] = Matrix->m[ 3 ][ 1 ] ;
	ConstantVSBase->LocalWorldMatrix[ 2 ][ 0 ] = Matrix->m[ 0 ][ 2 ] ;
	ConstantVSBase->LocalWorldMatrix[ 2 ][ 1 ] = Matrix->m[ 1 ][ 2 ] ;
	ConstantVSBase->LocalWorldMatrix[ 2 ][ 2 ] = Matrix->m[ 2 ][ 2 ] ;
	ConstantVSBase->LocalWorldMatrix[ 2 ][ 3 ] = Matrix->m[ 3 ][ 2 ] ;

	GD3D11.Device.Shader.Constant.ConstBuffer_VS_Base->ChangeFlag = TRUE ;

	// �萔�o�b�t�@���X�V
//	Graphics_D3D11_ConstantBuffer_Update( GD3D11.Device.Shader.Constant.ConstBuffer_VS_Base ) ;

	// �I��
	return 0 ;
}

// �r���[�ϊ��p�s����Z�b�g����
extern int Graphics_D3D11_DeviceState_SetViewMatrix( const MATRIX *Matrix )
{
	DX_D3D11_VS_CONST_BUFFER_BASE *ConstantVSBase ;

	if( GAPIWin.D3D11DeviceObject == NULL )
	{
		return -1 ;
	}

	ConstantVSBase = ( DX_D3D11_VS_CONST_BUFFER_BASE * )GD3D11.Device.Shader.Constant.ConstBuffer_VS_Base->SysmemBuffer ;

	// ���C�g�̈ʒu�E�����̍Čv�Z
	{
		DX_D3D11_CONST_BUFFER_COMMON  *ConstantCommon = ( DX_D3D11_CONST_BUFFER_COMMON  * )GD3D11.Device.Shader.Constant.ConstBuffer_Common->SysmemBuffer ;
		DX_D3D11_CONST_LIGHT          *ConstantLight ;
		int                           i ;
		VECTOR                        Direction ;
		VECTOR                        Position ;

		ConstantLight = ConstantCommon->Light ;
		for( i = 0 ; i < DX_D3D11_COMMON_CONST_LIGHT_NUM ; i ++, ConstantLight ++ )
		{
			VectorTransformSR( &Direction, &GD3D11.Device.State.LightParam[ i ].Direction, Matrix ) ;
			VectorTransform(   &Position,  &GD3D11.Device.State.LightParam[ i ].Position,  Matrix ) ;
			ConstantLight->Position[ 0 ]  = Position.x ;
			ConstantLight->Position[ 1 ]  = Position.y ;
			ConstantLight->Position[ 2 ]  = Position.z ;
			ConstantLight->Direction[ 0 ] = Direction.x ;
			ConstantLight->Direction[ 1 ] = Direction.y ;
			ConstantLight->Direction[ 2 ] = Direction.z ;
		}

		GD3D11.Device.Shader.Constant.ConstBuffer_Common->ChangeFlag = TRUE ;

		// �萔�o�b�t�@���X�V
//		Graphics_D3D11_ConstantBuffer_Update( GD3D11.Device.Shader.Constant.ConstBuffer_Common ) ;
	}

	// �萔�f�[�^�ɔ��f
	ConstantVSBase->ViewMatrix[ 0 ][ 0 ] = Matrix->m[ 0 ][ 0 ] ;
	ConstantVSBase->ViewMatrix[ 0 ][ 1 ] = Matrix->m[ 1 ][ 0 ] ;
	ConstantVSBase->ViewMatrix[ 0 ][ 2 ] = Matrix->m[ 2 ][ 0 ] ;
	ConstantVSBase->ViewMatrix[ 0 ][ 3 ] = Matrix->m[ 3 ][ 0 ] ;
	ConstantVSBase->ViewMatrix[ 1 ][ 0 ] = Matrix->m[ 0 ][ 1 ] ;
	ConstantVSBase->ViewMatrix[ 1 ][ 1 ] = Matrix->m[ 1 ][ 1 ] ;
	ConstantVSBase->ViewMatrix[ 1 ][ 2 ] = Matrix->m[ 2 ][ 1 ] ;
	ConstantVSBase->ViewMatrix[ 1 ][ 3 ] = Matrix->m[ 3 ][ 1 ] ;
	ConstantVSBase->ViewMatrix[ 2 ][ 0 ] = Matrix->m[ 0 ][ 2 ] ;
	ConstantVSBase->ViewMatrix[ 2 ][ 1 ] = Matrix->m[ 1 ][ 2 ] ;
	ConstantVSBase->ViewMatrix[ 2 ][ 2 ] = Matrix->m[ 2 ][ 2 ] ;
	ConstantVSBase->ViewMatrix[ 2 ][ 3 ] = Matrix->m[ 3 ][ 2 ] ;

	GD3D11.Device.Shader.Constant.ConstBuffer_VS_Base->ChangeFlag = TRUE ;

	// �萔�o�b�t�@���X�V
//	Graphics_D3D11_ConstantBuffer_Update( GD3D11.Device.Shader.Constant.ConstBuffer_VS_Base ) ;

	// �I��
	return 0 ;
}

// ���e�ϊ��p�s����Z�b�g����
extern int Graphics_D3D11_DeviceState_SetProjectionMatrix( const MATRIX *Matrix )
{
	DX_D3D11_VS_CONST_BUFFER_BASE *ConstantVSBase ;

	if( GAPIWin.D3D11DeviceObject == NULL )
	{
		return -1 ;
	}

	ConstantVSBase = ( DX_D3D11_VS_CONST_BUFFER_BASE * )GD3D11.Device.Shader.Constant.ConstBuffer_VS_Base->SysmemBuffer ;

	// �萔�f�[�^�ɔ��f
	ConstantVSBase->ProjectionMatrix[ 0 ][ 0 ] = Matrix->m[ 0 ][ 0 ] ;
	ConstantVSBase->ProjectionMatrix[ 0 ][ 1 ] = Matrix->m[ 1 ][ 0 ] ;
	ConstantVSBase->ProjectionMatrix[ 0 ][ 2 ] = Matrix->m[ 2 ][ 0 ] ;
	ConstantVSBase->ProjectionMatrix[ 0 ][ 3 ] = Matrix->m[ 3 ][ 0 ] ;
	ConstantVSBase->ProjectionMatrix[ 1 ][ 0 ] = Matrix->m[ 0 ][ 1 ] ;
	ConstantVSBase->ProjectionMatrix[ 1 ][ 1 ] = Matrix->m[ 1 ][ 1 ] ;
	ConstantVSBase->ProjectionMatrix[ 1 ][ 2 ] = Matrix->m[ 2 ][ 1 ] ;
	ConstantVSBase->ProjectionMatrix[ 1 ][ 3 ] = Matrix->m[ 3 ][ 1 ] ;
	ConstantVSBase->ProjectionMatrix[ 2 ][ 0 ] = Matrix->m[ 0 ][ 2 ] ;
	ConstantVSBase->ProjectionMatrix[ 2 ][ 1 ] = Matrix->m[ 1 ][ 2 ] ;
	ConstantVSBase->ProjectionMatrix[ 2 ][ 2 ] = Matrix->m[ 2 ][ 2 ] ;
	ConstantVSBase->ProjectionMatrix[ 2 ][ 3 ] = Matrix->m[ 3 ][ 2 ] ;
	ConstantVSBase->ProjectionMatrix[ 3 ][ 0 ] = Matrix->m[ 0 ][ 3 ] ;
	ConstantVSBase->ProjectionMatrix[ 3 ][ 1 ] = Matrix->m[ 1 ][ 3 ] ;
	ConstantVSBase->ProjectionMatrix[ 3 ][ 2 ] = Matrix->m[ 2 ][ 3 ] ;
	ConstantVSBase->ProjectionMatrix[ 3 ][ 3 ] = Matrix->m[ 3 ][ 3 ] ;

	GD3D11.Device.Shader.Constant.ConstBuffer_VS_Base->ChangeFlag = TRUE ;

	// �萔�o�b�t�@���X�V
//	Graphics_D3D11_ConstantBuffer_Update( GD3D11.Device.Shader.Constant.ConstBuffer_VS_Base ) ;

	// �I��
	return 0 ;
}

// �A���`�r���[�|�[�g�s����Z�b�g����
extern int Graphics_D3D11_DeviceState_SetAntiViewportMatrix( const MATRIX *Matrix )
{
	DX_D3D11_VS_CONST_BUFFER_BASE *ConstantVSBase ;

	if( GAPIWin.D3D11DeviceObject == NULL )
	{
		return -1 ;
	}

	ConstantVSBase = ( DX_D3D11_VS_CONST_BUFFER_BASE * )GD3D11.Device.Shader.Constant.ConstBuffer_VS_Base->SysmemBuffer ;

	// �萔�f�[�^�ɔ��f
	ConstantVSBase->AntiViewportMatrix[ 0 ][ 0 ] = Matrix->m[ 0 ][ 0 ] ;
	ConstantVSBase->AntiViewportMatrix[ 0 ][ 1 ] = Matrix->m[ 1 ][ 0 ] ;
	ConstantVSBase->AntiViewportMatrix[ 0 ][ 2 ] = Matrix->m[ 2 ][ 0 ] ;
	ConstantVSBase->AntiViewportMatrix[ 0 ][ 3 ] = Matrix->m[ 3 ][ 0 ] ;
	ConstantVSBase->AntiViewportMatrix[ 1 ][ 0 ] = Matrix->m[ 0 ][ 1 ] ;
	ConstantVSBase->AntiViewportMatrix[ 1 ][ 1 ] = Matrix->m[ 1 ][ 1 ] ;
	ConstantVSBase->AntiViewportMatrix[ 1 ][ 2 ] = Matrix->m[ 2 ][ 1 ] ;
	ConstantVSBase->AntiViewportMatrix[ 1 ][ 3 ] = Matrix->m[ 3 ][ 1 ] ;
	ConstantVSBase->AntiViewportMatrix[ 2 ][ 0 ] = Matrix->m[ 0 ][ 2 ] ;
	ConstantVSBase->AntiViewportMatrix[ 2 ][ 1 ] = Matrix->m[ 1 ][ 2 ] ;
	ConstantVSBase->AntiViewportMatrix[ 2 ][ 2 ] = Matrix->m[ 2 ][ 2 ] ;
	ConstantVSBase->AntiViewportMatrix[ 2 ][ 3 ] = Matrix->m[ 3 ][ 2 ] ;
	ConstantVSBase->AntiViewportMatrix[ 3 ][ 0 ] = Matrix->m[ 0 ][ 3 ] ;
	ConstantVSBase->AntiViewportMatrix[ 3 ][ 1 ] = Matrix->m[ 1 ][ 3 ] ;
	ConstantVSBase->AntiViewportMatrix[ 3 ][ 2 ] = Matrix->m[ 2 ][ 3 ] ;
	ConstantVSBase->AntiViewportMatrix[ 3 ][ 3 ] = Matrix->m[ 3 ][ 3 ] ;

	GD3D11.Device.Shader.Constant.ConstBuffer_VS_Base->ChangeFlag = TRUE ;

	// �萔�o�b�t�@���X�V
//	Graphics_D3D11_ConstantBuffer_Update( GD3D11.Device.Shader.Constant.ConstBuffer_VS_Base ) ;

	// �I��
	return 0 ;
}

// �A���r�G���g���C�g�ƃG�~�b�V�u�J���[���|�����킹���p�����[�^���X�V����
extern void Graphics_D3D11_DeviceState_RefreshAmbientAndEmissiveParam( void )
{
	DX_D3D11_CONST_BUFFER_COMMON  *ConstantCommon ;
	DX_D3D11_CONST_LIGHT          *ConstantLight ;
	int                           i ;

	if( GAPIWin.D3D11DeviceObject == NULL )
	{
		return ;
	}

	ConstantCommon = ( DX_D3D11_CONST_BUFFER_COMMON  * )GD3D11.Device.Shader.Constant.ConstBuffer_Common->SysmemBuffer ;

	// �L���ȃ��C�g�̃A���r�G���g�J���[�ƃ}�e���A���̃A���r�G���g�J���[����Z�������̂��V�F�[�_�[�ɃZ�b�g����
	ConstantLight = ConstantCommon->Light ;
	for( i = 0 ; i <= GD3D11.Device.State.LightEnableMaxIndex ; i ++, ConstantLight ++ )
	{
		if( GD3D11.Device.State.LightEnableFlag[ i ] == FALSE )
		{
			continue ;
		}

		// �萔�o�b�t�@�ɔ��f����
		ConstantLight->Ambient[ 0 ] = GD3D11.Device.State.LightParam[ i ].Ambient.r * GD3D11.Device.State.Material.Ambient.r ;
		ConstantLight->Ambient[ 1 ] = GD3D11.Device.State.LightParam[ i ].Ambient.g * GD3D11.Device.State.Material.Ambient.g ;
		ConstantLight->Ambient[ 2 ] = GD3D11.Device.State.LightParam[ i ].Ambient.b * GD3D11.Device.State.Material.Ambient.b ;
		ConstantLight->Ambient[ 3 ] = GD3D11.Device.State.LightParam[ i ].Ambient.a * GD3D11.Device.State.Material.Ambient.a ;
	}

	// �O���[�o���A���r�G���g���C�g�J���[�ƃ}�e���A���A���r�G���g�J���[����Z�������̂ƃG�~�b�V�u�J���[�����Z�������̂��쐬����
	ConstantCommon->Material.Ambient_Emissive[ 0 ] = GD3D11.Device.State.GlobalAmbientLightColor.r * GD3D11.Device.State.Material.Ambient.r + GD3D11.Device.State.Material.Emissive.r ;
	ConstantCommon->Material.Ambient_Emissive[ 1 ] = GD3D11.Device.State.GlobalAmbientLightColor.g * GD3D11.Device.State.Material.Ambient.g + GD3D11.Device.State.Material.Emissive.g ;
	ConstantCommon->Material.Ambient_Emissive[ 2 ] = GD3D11.Device.State.GlobalAmbientLightColor.b * GD3D11.Device.State.Material.Ambient.b + GD3D11.Device.State.Material.Emissive.b ;
	ConstantCommon->Material.Ambient_Emissive[ 3 ] = GD3D11.Device.State.GlobalAmbientLightColor.a * GD3D11.Device.State.Material.Ambient.a + GD3D11.Device.State.Material.Emissive.a ;

	GD3D11.Device.Shader.Constant.ConstBuffer_Common->ChangeFlag = TRUE ;		
}



// ���C�g�̃Z�b�g
extern int Graphics_D3D11_DeviceState_SetLightState( int LightNumber, LIGHTPARAM *Light )
{
	DX_D3D11_CONST_BUFFER_COMMON  *ConstantCommon ;
	DX_D3D11_CONST_LIGHT          *ConstantLight ;
	VECTOR                        Direction ;
	VECTOR                        Position ;
	float                         ThetaSin ;
	float                         ThetaCos ;
	float                         PhiSin ;
	float                         PhiCos ;

	if( GAPIWin.D3D11DeviceObject == NULL )
	{
		return -1 ;
	}

	if( LightNumber < 0 || LightNumber >= DX_D3D11_COMMON_CONST_LIGHT_NUM )
	{
		return -1 ;
	}

	ConstantCommon = ( DX_D3D11_CONST_BUFFER_COMMON  * )GD3D11.Device.Shader.Constant.ConstBuffer_Common->SysmemBuffer ;

	if( _MEMCMP( &GD3D11.Device.State.LightParam[ LightNumber ], Light, sizeof( LIGHTPARAM ) ) == 0 &&
		GD3D11.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
	{
		return 0 ;
	}

	DRAWSTOCKINFO

	// �p�����[�^�̕ۑ�
	GD3D11.Device.State.LightParam[ LightNumber ] = *Light ;

	// �萔�f�[�^�ɔ��f
	ConstantLight = &ConstantCommon->Light[ LightNumber ] ;

	_SINCOS( Light->Theta / 2.0f, &ThetaSin, &ThetaCos ) ;
	_SINCOS( Light->Phi   / 2.0f, &PhiSin,   &PhiCos ) ;

	ConstantLight->Type           = Light->LightType ;
	VectorTransformSR( &Direction, &Light->Direction, &GSYS.DrawSetting.ViewMatrixF ) ;
	VectorTransform(   &Position,  &Light->Position,  &GSYS.DrawSetting.ViewMatrixF ) ;
	ConstantLight->Position[ 0 ]  = Position.x ;
	ConstantLight->Position[ 1 ]  = Position.y ;
	ConstantLight->Position[ 2 ]  = Position.z ;
	ConstantLight->RangePow2      = Light->Range * Light->Range ;
	ConstantLight->Direction[ 0 ] = Direction.x ;
	ConstantLight->Direction[ 1 ] = Direction.y ;
	ConstantLight->Direction[ 2 ] = Direction.z ;
	ConstantLight->FallOff        = Light->Falloff ;
	ConstantLight->Diffuse[ 0 ]   = Light->Diffuse.r ;
	ConstantLight->Diffuse[ 1 ]   = Light->Diffuse.g ;
	ConstantLight->Diffuse[ 2 ]   = Light->Diffuse.b ;
	ConstantLight->SpotParam0     = PhiCos ;
	ConstantLight->Specular[ 0 ]  = Light->Specular.r ;
	ConstantLight->Specular[ 1 ]  = Light->Specular.g ;
	ConstantLight->Specular[ 2 ]  = Light->Specular.b ;
	ConstantLight->SpotParam1     = 1.0f / ( ThetaCos - PhiCos ) ;
	ConstantLight->Ambient[ 0 ]   = Light->Ambient.r ;
	ConstantLight->Ambient[ 1 ]   = Light->Ambient.g ;
	ConstantLight->Ambient[ 2 ]   = Light->Ambient.b ;
	ConstantLight->Ambient[ 3 ]   = Light->Ambient.a ;
	ConstantLight->Attenuation0   = Light->Attenuation0 ;
	ConstantLight->Attenuation1   = Light->Attenuation1 ;
	ConstantLight->Attenuation2   = Light->Attenuation2 ;
	GD3D11.Device.Shader.Constant.ConstBuffer_Common->ChangeFlag = TRUE ;

	// �A���r�G���g�J���[�ƃG�~�b�V�u�J���[���|�����킹�������X�V����
	Graphics_D3D11_DeviceState_RefreshAmbientAndEmissiveParam() ;

	// �萔�o�b�t�@���X�V
//	Graphics_D3D11_ConstantBuffer_Update( GD3D11.Device.Shader.Constant.ConstBuffer_Common ) ;

	// �I��
	return 0 ;
}

// ���C�g�̗L���A������ύX
extern int Graphics_D3D11_DeviceState_SetLightEnable( int LightNumber, int EnableState )
{
	if( GAPIWin.D3D11DeviceObject == NULL )
	{
		return -1 ;
	}

	if( LightNumber < 0 || LightNumber >= DX_D3D11_COMMON_CONST_LIGHT_NUM )
	{
		return -1 ;
	}

	if( GD3D11.Device.State.LightEnableFlag[ LightNumber ] == EnableState &&
		GD3D11.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
	{
		return 0 ;
	}

	DRAWSTOCKINFO

	GD3D11.Device.State.LightEnableFlag[ LightNumber ] = EnableState ;

	// �L���ȃ��C�g�̍ő�C���f�b�N�X���X�V
	if( EnableState == TRUE )
	{
		if( GD3D11.Device.State.LightEnableMaxIndex < LightNumber )
			GD3D11.Device.State.LightEnableMaxIndex = LightNumber ;
	}
	else
	{
		if( GD3D11.Device.State.LightEnableMaxIndex == LightNumber )
		{
			int i ;

			for( i = LightNumber ; i > 0 && GD3D11.Device.State.LightEnableFlag[ i ] == FALSE ; i -- ){}
			GD3D11.Device.State.LightEnableMaxIndex = i ;
		}
	}

	// �A���r�G���g�J���[�ƃG�~�b�V�u�J���[���|�����킹�������X�V����
	Graphics_D3D11_DeviceState_RefreshAmbientAndEmissiveParam() ;

	// �萔�o�b�t�@���X�V
//	Graphics_D3D11_ConstantBuffer_Update( GD3D11.Device.Shader.Constant.ConstBuffer_Common ) ;

	GD3D11.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

	// �I��
	return 0 ;
}

// �}�e���A���̃Z�b�g
extern int Graphics_D3D11_DeviceState_SetMaterial( MATERIALPARAM *Material )
{
	DX_D3D11_CONST_BUFFER_COMMON *ConstantCommon ;

	if( GAPIWin.D3D11DeviceObject == NULL )
	{
		return -1 ;
	}

	if( _MEMCMP( Material, &GD3D11.Device.State.Material, sizeof( MATERIALPARAM ) ) == 0 &&
		GD3D11.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
	{
		return 0 ;
	}

	ConstantCommon = ( DX_D3D11_CONST_BUFFER_COMMON * )GD3D11.Device.Shader.Constant.ConstBuffer_Common->SysmemBuffer ;

	DRAWSTOCKINFO

	GD3D11.Device.State.Material = *Material ;

	// �萔�f�[�^�ɔ��f
	ConstantCommon->Material.Diffuse[ 0 ]  = Material->Diffuse.r ;
	ConstantCommon->Material.Diffuse[ 1 ]  = Material->Diffuse.g ;
	ConstantCommon->Material.Diffuse[ 2 ]  = Material->Diffuse.b ;
	ConstantCommon->Material.Diffuse[ 3 ]  = Material->Diffuse.a ;
	ConstantCommon->Material.Specular[ 0 ] = Material->Specular.r ;
	ConstantCommon->Material.Specular[ 1 ] = Material->Specular.g ;
	ConstantCommon->Material.Specular[ 2 ] = Material->Specular.b ;
	ConstantCommon->Material.Specular[ 3 ] = Material->Specular.a ;
	ConstantCommon->Material.Power         = Material->Power < 0.00000000001f ? 0.00000000001f : Material->Power ;
	GD3D11.Device.Shader.Constant.ConstBuffer_Common->ChangeFlag = TRUE ;

	// �A���r�G���g�J���[�ƃG�~�b�V�u�J���[���|�����킹�������X�V����
	Graphics_D3D11_DeviceState_RefreshAmbientAndEmissiveParam() ;

	// �萔�o�b�t�@���X�V
//	Graphics_D3D11_ConstantBuffer_Update( GD3D11.Device.Shader.Constant.ConstBuffer_Common ) ;

	GD3D11.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

	// �I��
	return 0 ;
}

// �X�y�L�������g�p���邩�ǂ������Z�b�g
extern int Graphics_D3D11_DeviceState_SetSpecularEnable( int UseFlag )
{
	DX_D3D11_VS_CONST_BUFFER_BASE *ConstantVSBase ;

	if( GAPIWin.D3D11DeviceObject == NULL )
	{
		return -1 ;
	}

	// �����̃p�����[�^�Ɠ����ꍇ�͉������Ȃ�
	if( GD3D11.Device.State.UseSpecular == UseFlag &&
		GD3D11.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
	{
		return 0 ;
	}

	ConstantVSBase = ( DX_D3D11_VS_CONST_BUFFER_BASE * )GD3D11.Device.Shader.Constant.ConstBuffer_VS_Base->SysmemBuffer ;

	DRAWSTOCKINFO

	GD3D11.Device.State.UseSpecular = UseFlag ;

	// �萔�f�[�^�ɔ��f
	ConstantVSBase->MulSpecularColor = UseFlag ? 1.0f : 0.0f ;
	GD3D11.Device.Shader.Constant.ConstBuffer_VS_Base->ChangeFlag = TRUE ;

	// �萔�o�b�t�@���X�V
//	Graphics_D3D11_ConstantBuffer_Update( GD3D11.Device.Shader.Constant.ConstBuffer_VS_Base ) ;

	GD3D11.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

	// �I��
	return 0 ;
}

// ���_�̃f�B�t���[�Y�J���[���}�e���A���̃f�B�t���[�Y�J���[�Ƃ��Ďg�p���邩�ǂ�����ݒ肷��
extern int Graphics_D3D11_DeviceState_SetUseVertexDiffuseColor( int UseFlag )
{
	DX_D3D11_VS_CONST_BUFFER_BASE *ConstantVSBase ;

	if( GAPIWin.D3D11DeviceObject == NULL )
	{
		return -1 ;
	}

	if( UseFlag == GD3D11.Device.State.MaterialUseVertexDiffuseColor &&
		GD3D11.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
	{
		return 0 ;
	}

	ConstantVSBase = ( DX_D3D11_VS_CONST_BUFFER_BASE * )GD3D11.Device.Shader.Constant.ConstBuffer_VS_Base->SysmemBuffer ;

	DRAWSTOCKINFO

	// �萔�f�[�^�ɔ��f
	ConstantVSBase->DiffuseSource = UseFlag ? 1.0f : 0.0f ;
	GD3D11.Device.Shader.Constant.ConstBuffer_VS_Base->ChangeFlag = TRUE ;

	// �萔�o�b�t�@���X�V
//	Graphics_D3D11_ConstantBuffer_Update( GD3D11.Device.Shader.Constant.ConstBuffer_VS_Base ) ;

	GD3D11.Device.State.MaterialUseVertexDiffuseColor = UseFlag ;

	GD3D11.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

	// �I��
	return 0 ;
}

// ���_�̃X�y�L�����J���[���}�e���A���̃X�y�L�����J���[�Ƃ��Ďg�p���邩�ǂ�����ݒ肷��
extern int Graphics_D3D11_DeviceState_SetUseVertexSpecularColor( int UseFlag )
{
	DX_D3D11_VS_CONST_BUFFER_BASE *ConstantVSBase ;

	if( GAPIWin.D3D11DeviceObject == NULL )
	{
		return -1 ;
	}

	if( UseFlag == GD3D11.Device.State.MaterialUseVertexSpecularColor &&
		GD3D11.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
	{
		return 0 ;
	}

	ConstantVSBase = ( DX_D3D11_VS_CONST_BUFFER_BASE * )GD3D11.Device.Shader.Constant.ConstBuffer_VS_Base->SysmemBuffer ;

	DRAWSTOCKINFO

	// �萔�f�[�^�ɔ��f
	ConstantVSBase->SpecularSource = UseFlag ? 1.0f : 0.0f ;
	GD3D11.Device.Shader.Constant.ConstBuffer_VS_Base->ChangeFlag = TRUE ;

	// �萔�o�b�t�@���X�V
//	Graphics_D3D11_ConstantBuffer_Update( GD3D11.Device.Shader.Constant.ConstBuffer_VS_Base ) ;

	GD3D11.Device.State.MaterialUseVertexSpecularColor = UseFlag ;

	GD3D11.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

	// �I��
	return 0 ;
}

// �J�����O�̗L�����Z�b�g 
extern int Graphics_D3D11_DeviceState_SetCullMode( int State )
{
	D_D3D11_CULL_MODE D3DCullMode ;

	if( GAPIWin.D3D11DeviceObject == NULL )
	{
		return -1 ;
	}

	switch( State )
	{
	case 0 :
	default :
		D3DCullMode = D_D3D11_CULL_NONE ;
		break ;

	case 1 :
		D3DCullMode = D_D3D11_CULL_BACK ;
		break ;

	case 2 :
		D3DCullMode = D_D3D11_CULL_FRONT ;
		break ;
	}

	// ���[�h�������������牽�������I��
	if( GD3D11.Device.State.RasterizerDesc.CullMode == D3DCullMode &&
		GD3D11.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
	{
		return 0 ;
	}
	
	DRAWSTOCKINFO

	GD3D11.Device.State.ChangeRasterizerDesc = TRUE ;

	GD3D11.Device.State.RasterizerDesc.CullMode = D3DCullMode ;

	GD3D11.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

//	return Graphics_D3D11_DeviceState_SetupRasterizerState() ;
	return 0 ;
}

// �A���r�G���g�J���[�̃Z�b�g
extern int Graphics_D3D11_DeviceState_SetAmbient( COLOR_F *Color )
{
	if( GAPIWin.D3D11DeviceObject == NULL )
	{
		return -1 ;
	}

	if( _MEMCMP( Color, &GD3D11.Device.State.GlobalAmbientLightColor, sizeof( COLOR_F ) ) == 0 &&
		GD3D11.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
	{
		return 0 ;
	}

	DRAWSTOCKINFO

	GD3D11.Device.State.GlobalAmbientLightColor = *Color ;

	// �A���r�G���g�J���[�ƃG�~�b�V�u�J���[���|�����킹�������X�V����
	Graphics_D3D11_DeviceState_RefreshAmbientAndEmissiveParam() ;

	// �萔�o�b�t�@���X�V
//	Graphics_D3D11_ConstantBuffer_Update( GD3D11.Device.Shader.Constant.ConstBuffer_Common ) ;

	GD3D11.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

	// �I��
	return 0 ;
}

// �O�̃t�B���^�[�^�C�v���� D_D3D11_FILTER ���擾����
static D_D3D11_FILTER Graphics_D3D11_DeviceState_GetFilter( int MinFilter /* DX_DRAWMODE_NEAREST �� */, int MagFilter, int MipFilter )
{
	if( MagFilter == DX_DRAWMODE_ANISOTROPIC ||
		MinFilter == DX_DRAWMODE_ANISOTROPIC ||
		MipFilter == DX_DRAWMODE_ANISOTROPIC )
	{
		return D_D3D11_FILTER_ANISOTROPIC ;
	}
	else
	{
		// 0:Point  1:Linear
		static D_D3D11_FILTER FilterTable[ 2 /* Min */ ][ 2 /* Mag */ ][ 2 /* Mip */ ] =
		{
			{
				{
					D_D3D11_FILTER_MIN_MAG_MIP_POINT,
					D_D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR
				},

				{
					D_D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT,
					D_D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR,
				},
			},

			{
				{
					D_D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT,
					D_D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR
				},

				{
					D_D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT,
					D_D3D11_FILTER_MIN_MAG_MIP_LINEAR,
				},
			},
		} ;

		return FilterTable[ MinFilter != DX_DRAWMODE_NEAREST ? 1 : 0 ][ MagFilter != DX_DRAWMODE_NEAREST ? 1 : 0 ][ MipFilter != DX_DRAWMODE_NEAREST ? 1 : 0 ] ;
	}
}

// ���݂̐ݒ�Ɋ�Â��� ID3D11SamplerState ���Z�b�g�A�b�v����
static int Graphics_D3D11_DeviceState_SetupSamplerState( void )
{
	int                     i ;
	int                     SetLength ;
	D_D3D11_SAMPLER_DESC	*SamplerDesc ;
	D_ID3D11SamplerState	*NewSamplerState ;
	D_ID3D11SamplerState	**DefaultSamplerState ;
	HRESULT                 hr ;
	static int				DefaultFilterIndexTableInitialize = FALSE ;
	static int				DefaultFilterIndexTable[ 0xff ] ;

	// D_D3D11_FILTER ����C���f�b�N�X���擾���邽�߂̃e�[�u��������������Ă��Ȃ��ꍇ�͏���������
	if( DefaultFilterIndexTableInitialize == FALSE )
	{
		DefaultFilterIndexTableInitialize = TRUE ;

		DefaultFilterIndexTable[ D_D3D11_FILTER_MIN_MAG_MIP_POINT                          ] = 0 ;
		DefaultFilterIndexTable[ D_D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR                   ] = 1 ;
		DefaultFilterIndexTable[ D_D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT             ] = 2 ;
		DefaultFilterIndexTable[ D_D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR                   ] = 3 ;
		DefaultFilterIndexTable[ D_D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT                   ] = 4 ;
		DefaultFilterIndexTable[ D_D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR            ] = 5 ;
		DefaultFilterIndexTable[ D_D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT                   ] = 6 ;
		DefaultFilterIndexTable[ D_D3D11_FILTER_MIN_MAG_MIP_LINEAR                         ] = 7 ;
		DefaultFilterIndexTable[ D_D3D11_FILTER_ANISOTROPIC                                ] = 8 ;
		DefaultFilterIndexTable[ D_D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT               ] = 9 ;
		DefaultFilterIndexTable[ D_D3D11_FILTER_COMPARISON_MIN_MAG_POINT_MIP_LINEAR        ] = 10 ;
		DefaultFilterIndexTable[ D_D3D11_FILTER_COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT  ] = 11 ;
		DefaultFilterIndexTable[ D_D3D11_FILTER_COMPARISON_MIN_POINT_MAG_MIP_LINEAR        ] = 12 ;
		DefaultFilterIndexTable[ D_D3D11_FILTER_COMPARISON_MIN_LINEAR_MAG_MIP_POINT        ] = 13 ;
		DefaultFilterIndexTable[ D_D3D11_FILTER_COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR ] = 14 ;
		DefaultFilterIndexTable[ D_D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT        ] = 15 ;
		DefaultFilterIndexTable[ D_D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR              ] = 16 ;
		DefaultFilterIndexTable[ D_D3D11_FILTER_COMPARISON_ANISOTROPIC                     ] = 17 ;
	}

	// �ݒ�ɕύX�������ꍇ�͉��������ɏI��
	if( GD3D11.Device.State.ChangeSamplerDesc == FALSE &&
		GD3D11.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
	{
		return 0 ;
	}

	// �ύX�����������ǂ����̃t���O��|��
	GD3D11.Device.State.ChangeSamplerDesc = FALSE ;

	// �T���v���[�X���b�g�̐������J��Ԃ�
	SamplerDesc = GD3D11.Device.State.SamplerDesc ;
	SetLength     = 0 ;
	for( i = 0 ; i < USE_TEXTURESTAGE_NUM ; i ++, SamplerDesc ++ )
	{
		// �ݒ�l�ɕύX�������A�K���Đݒ���s���t���O���|��Ă����玟�̃��[�v��
		if( GD3D11.Device.State.ChangeSamplerDescSlot[ i ] == FALSE &&
			GD3D11.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
		{
			// �ύX����ݒ�̐����O�ł͂Ȃ��ꍇ�̓T���v���[�X�e�[�g���f�o�C�X�R���e�L�X�g�ɐݒ肷��
			if( SetLength > 0 )
			{
				D3D11DeviceContext_PSSetSamplers( ( UINT )( i - SetLength ), ( UINT )SetLength, &GD3D11.Device.State.SamplerState[ i - SetLength ] ) ;
				SetLength = 0 ;
			}
			continue ;
		}

		// �ύX�����������ǂ����̃t���O��|��
		GD3D11.Device.State.ChangeSamplerDescSlot[ i ] = FALSE ;

		// ���ݐݒ肳��Ă���T���v���[������ꍇ�͉�������
		if( GD3D11.Device.State.SamplerState[ i ] != NULL )
		{
			// ���݂̐ݒ��ύX����ۂɃT���v���[�X�e�[�g�������[�X����w��ɂȂ��Ă����烊���[�X����
			if( GD3D11.Device.State.ChangeSamplerStateReleaseFlag[ i ] )
			{
				Direct3D11_Release_SamplerState( GD3D11.Device.State.SamplerState[ i ] ) ;
			}

			GD3D11.Device.State.SamplerState[ i ] = NULL ;
		}

		// �ݒ��ύX����ۂɃT���v���[�X�e�[�g�������[�X���邩�ǂ����̃t���O��|��
		GD3D11.Device.State.ChangeSamplerStateReleaseFlag[ i ] = FALSE ;

		// �悭�g���ݒ�̃T���v���[�X�e�[�g���ǂ����𔻒肷��
		if(	SamplerDesc->MipLODBias       != 0.0f ||
			( SamplerDesc->MaxAnisotropy  != 16 && SamplerDesc->MaxAnisotropy != 1 ) ||
			SamplerDesc->ComparisonFunc   != D_D3D11_COMPARISON_ALWAYS ||
			SamplerDesc->BorderColor[ 0 ] != 0.0f ||
			SamplerDesc->BorderColor[ 1 ] != 0.0f ||
			SamplerDesc->BorderColor[ 2 ] != 0.0f ||
			SamplerDesc->BorderColor[ 3 ] != 0.0f ||
			SamplerDesc->MinLOD           != -D_D3D11_FLOAT32_MAX ||
			SamplerDesc->MaxLOD           !=  D_D3D11_FLOAT32_MAX )
		{
			// �悭�g���ݒ�̃T���v���[�X�e�[�g�ł͂Ȃ��ꍇ�͂����ŃT���v���[�X�e�[�g���쐬����
			hr = D3D11Device_CreateSamplerState( SamplerDesc, &NewSamplerState ) ;
			if( hr != S_OK )
			{
				DXST_ERRORLOGFMT_ADDUTF16LE(( "\x49\x00\x44\x00\x33\x00\x44\x00\x31\x00\x31\x00\x53\x00\x61\x00\x6d\x00\x70\x00\x6c\x00\x65\x00\x72\x00\x53\x00\x74\x00\x61\x00\x74\x00\x65\x00\x20\x00\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x00\x30\x87\x7b\x40\x62\x11\xff\x0a\x00\x00"/*@ L"ID3D11SamplerState �̍쐬�Ɏ��s���܂����@�ӏ��P\n" @*/ )) ;
				return -1 ;
			}

			// �쐬�����T���v���[�X�e�[�g��ݒ�
			GD3D11.Device.State.SamplerState[ i ] = NewSamplerState ;

			// ���̃T���v���[�X�e�[�g�͈ꎞ�I�Ȃ��̂Ȃ̂ŁA�I�����Ƀ����[�X����悤�ɂ���
			GD3D11.Device.State.ChangeSamplerStateReleaseFlag[ i ] = TRUE ;
		}
		else
		{
			// �悭�g���ݒ�̃T���v���[�X�e�[�g�̏ꍇ

			// �w��̂悭�g���ݒ�̃T���v���[�X�e�[�g���܂��쐬����Ă��Ȃ�������쐬����
			DefaultSamplerState = &GD3D11.Device.State.DefaultSamplerStateArray[ DefaultFilterIndexTable[ SamplerDesc->Filter ] ][ SamplerDesc->AddressU ][ SamplerDesc->AddressV ][ SamplerDesc->AddressW ][ SamplerDesc->MaxAnisotropy == 1 ? 0 : 1 ] ;
			if( *DefaultSamplerState == NULL )
			{
				hr = D3D11Device_CreateSamplerState( SamplerDesc, &NewSamplerState ) ;
				if( hr != S_OK )
				{
					DXST_ERRORLOGFMT_ADDUTF16LE(( "\x49\x00\x44\x00\x33\x00\x44\x00\x31\x00\x31\x00\x53\x00\x61\x00\x6d\x00\x70\x00\x6c\x00\x65\x00\x72\x00\x53\x00\x74\x00\x61\x00\x74\x00\x65\x00\x20\x00\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x00\x30\x87\x7b\x40\x62\x12\xff\x0a\x00\x00"/*@ L"ID3D11SamplerState �̍쐬�Ɏ��s���܂����@�ӏ��Q\n" @*/ )) ;
					return -1 ;
				}
				*DefaultSamplerState = NewSamplerState ;
			}

			// �悭�g���ݒ�̃T���v���[�X�e�[�g��ݒ�
			GD3D11.Device.State.SamplerState[ i ] = *DefaultSamplerState ;
		}

		// �ύX����ݒ�̐����C���N�������g
		SetLength ++ ;
	}

	// �ύX����ݒ�̐����O�ł͂Ȃ��ꍇ�̓T���v���[�X�e�[�g���f�o�C�X�R���e�L�X�g�ɐݒ肷��
	if( SetLength > 0 )
	{
		D3D11DeviceContext_PSSetSamplers( ( UINT )( i - SetLength ), ( UINT )SetLength, &GD3D11.Device.State.SamplerState[ i - SetLength ] ) ;
		SetLength = 0 ;
	}

	// ����I��
	return 0 ;
}

// ID3D11SamplerState ���������
static int Graphics_D3D11_DeviceState_TerminateSamplerState( void )
{
	int i ;

	GD3D11.Device.State.ChangeSamplerDesc = FALSE ;

	_MEMSET( GD3D11.Device.State.ChangeSamplerDescSlot, 0, sizeof( GD3D11.Device.State.ChangeSamplerDescSlot ) ) ;
//	_MEMSET( GD3D11.Device.State.SamplerDesc, 0, sizeof( GD3D11.Device.State.SamplerDesc ) ) ;

	for( i = 0 ; i < USE_TEXTURESTAGE_NUM ; i ++ )
	{
		if( GD3D11.Device.State.SamplerState[ i ] == NULL )
		{
			continue ;
		}

		if( GD3D11.Device.State.ChangeSamplerStateReleaseFlag[ i ] )
		{
			Direct3D11_Release_SamplerState( GD3D11.Device.State.SamplerState[ i ] ) ;
			GD3D11.Device.State.ChangeSamplerStateReleaseFlag[ i ] = FALSE ;
		}
		GD3D11.Device.State.SamplerState[ i ] = NULL ;
	}

	Graphics_D3D11_SamplerStateArray_Release( ( D_ID3D11SamplerState ** )GD3D11.Device.State.DefaultSamplerStateArray, sizeof( GD3D11.Device.State.DefaultSamplerStateArray ) / sizeof( D_ID3D11SamplerState * ) ) ;

	// ����I��
	return 0 ;
}

// ���݂̐ݒ�Ɋ�Â��� ID3D11RasterizerState ���Z�b�g�A�b�v����
static int Graphics_D3D11_DeviceState_SetupRasterizerState( void )
{
	D_D3D11_RASTERIZER_DESC *RasterizerDesc ;
	D_ID3D11RasterizerState *NewRasterizerState ;
	D_ID3D11RasterizerState **DefaultRasterizerState ;
	HRESULT                  hr ;

	// �ݒ�ɕύX�������A�K���Đݒ���s���t���O���|��Ă����牽�������I��
	if( GD3D11.Device.State.ChangeRasterizerDesc == FALSE &&
		GD3D11.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
	{
		return 0 ;
	}

	// �ݒ�ɕύX�����������ǂ����̃t���O��|��
	GD3D11.Device.State.ChangeRasterizerDesc = FALSE ;

	// ���ݐݒ肳��Ă��郉�X�^���C�U�[�X�e�[�g������ꍇ�͉�������
	if( GD3D11.Device.State.RasterizerState != NULL )
	{
		// ���݂̐ݒ��ύX����ۂɃ��X�^���C�U�[�X�e�[�g�������[�X����w��ɂȂ��Ă����烊���[�X����
		if( GD3D11.Device.State.ChangeRasterizerStateReleaseFlag )
		{
			Direct3D11_Release_RasterizerState( GD3D11.Device.State.RasterizerState ) ;
		}

		GD3D11.Device.State.RasterizerState = NULL ;
	}

	// �ݒ��ύX����ۂɃ��X�^���C�U�[�X�e�[�g�������[�X���邩�ǂ����̃t���O��|��
	GD3D11.Device.State.ChangeRasterizerStateReleaseFlag = FALSE ;

	// �悭�g���ݒ�̃��X�^���C�U�[�X�e�[�g���ǂ����𔻒肷��
	RasterizerDesc = &GD3D11.Device.State.RasterizerDesc ;
	if( RasterizerDesc->FrontCounterClockwise != FALSE ||
		RasterizerDesc->DepthBias             != 0 ||
		RasterizerDesc->DepthBiasClamp        != 0.0f ||
		RasterizerDesc->SlopeScaledDepthBias  != 0.0f ||
		RasterizerDesc->ScissorEnable         != FALSE ||
		RasterizerDesc->AntialiasedLineEnable != FALSE )
	{
		// �悭�g���ݒ�̃��X�^���C�U�[�X�e�[�g�ł͂Ȃ��ꍇ�͂����Ń��X�^���C�U�[�X�e�[�g���쐬����
		hr = D3D11Device_CreateRasterizerState( RasterizerDesc, &NewRasterizerState ) ;
		if( hr != S_OK )
		{
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\x49\x00\x44\x00\x33\x00\x44\x00\x31\x00\x31\x00\x52\x00\x61\x00\x73\x00\x74\x00\x65\x00\x72\x00\x69\x00\x7a\x00\x65\x00\x72\x00\x53\x00\x74\x00\x61\x00\x74\x00\x65\x00\x20\x00\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x00\x30\x87\x7b\x40\x62\x11\xff\x0a\x00\x00"/*@ L"ID3D11RasterizerState �̍쐬�Ɏ��s���܂����@�ӏ��P\n" @*/ )) ;
			return -1 ;
		}

		// �쐬�������X�^���C�U�[�X�e�[�g��ݒ�
		GD3D11.Device.State.RasterizerState = NewRasterizerState ;

		// ���̃��X�^���C�U�[�X�e�[�g�͈ꎞ�I�Ȃ��̂Ȃ̂ŁA�I�����Ƀ����[�X����悤�ɂ���
		GD3D11.Device.State.ChangeRasterizerStateReleaseFlag = TRUE ;
	}
	else
	{
		// �悭�g���ݒ�̃��X�^���C�U�[�X�e�[�g�̏ꍇ

		// �w��̂悭�g���ݒ�̃��X�^���C�U�[�X�e�[�g���܂��쐬����Ă��Ȃ�������쐬����
		DefaultRasterizerState = &GD3D11.Device.State.DefaultRasterizerStateArray[ RasterizerDesc->CullMode ][ RasterizerDesc->FillMode ][ RasterizerDesc->DepthClipEnable ][ RasterizerDesc->MultisampleEnable ] ;
		if( *DefaultRasterizerState == NULL )
		{
			hr = D3D11Device_CreateRasterizerState( RasterizerDesc, &NewRasterizerState ) ;
			if( hr != S_OK )
			{
				DXST_ERRORLOGFMT_ADDUTF16LE(( "\x49\x00\x44\x00\x33\x00\x44\x00\x31\x00\x31\x00\x52\x00\x61\x00\x73\x00\x74\x00\x65\x00\x72\x00\x69\x00\x7a\x00\x65\x00\x72\x00\x53\x00\x74\x00\x61\x00\x74\x00\x65\x00\x20\x00\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x00\x30\x87\x7b\x40\x62\x12\xff\x0a\x00\x00"/*@ L"ID3D11RasterizerState �̍쐬�Ɏ��s���܂����@�ӏ��Q\n" @*/ )) ;
				return -1 ;
			}
			*DefaultRasterizerState = NewRasterizerState ;
		}

		// �悭�g���ݒ�̃��X�^���C�U�[�X�e�[�g��ݒ�
		GD3D11.Device.State.RasterizerState = *DefaultRasterizerState ;
	}

	// ���X�^���C�U�[�X�e�[�g���f�o�C�X�R���e�L�X�g�ɐݒ肷��
	D3D11DeviceContext_RSSetState( GD3D11.Device.State.RasterizerState ) ;

	// ����I��
	return 0 ;
}

// ID3D11RasterizerState ���������
static int Graphics_D3D11_DeviceState_TerminateRasterizerState( void )
{
	GD3D11.Device.State.ChangeRasterizerDesc = FALSE ;

//	_MEMSET( &GD3D11.Device.State.RasterizerDesc, 0, sizeof( GD3D11.Device.State.RasterizerDesc ) ) ;

	if( GD3D11.Device.State.ChangeRasterizerStateReleaseFlag )
	{
		Direct3D11_Release_RasterizerState( GD3D11.Device.State.RasterizerState ) ;
		GD3D11.Device.State.ChangeRasterizerStateReleaseFlag = FALSE ;
	}
	GD3D11.Device.State.RasterizerState = NULL ;

	Graphics_D3D11_RasterizerStateArray_Release( ( D_ID3D11RasterizerState ** )GD3D11.Device.State.DefaultRasterizerStateArray, sizeof( GD3D11.Device.State.DefaultRasterizerStateArray ) / sizeof( D_ID3D11RasterizerState * ) ) ;

	// ����I��
	return 0 ;
}

// ���݂̐ݒ�Ɋ�Â��� ID3D11DepthStencilState ���Z�b�g�A�b�v����
static int Graphics_D3D11_DeviceState_SetupDepthStencilState( void )
{
	D_D3D11_DEPTH_STENCIL_DESC *DepthStencilDesc ;
	D_ID3D11DepthStencilState  *NewDepthStencilState ;
	D_ID3D11DepthStencilState  **DefaultDepthStencilState ;
	HRESULT                     hr ;

	// �ݒ�ɕύX�������A�K���Đݒ���s���t���O���|��Ă����牽�������I��
	if( GD3D11.Device.State.ChangeDepthStencilDesc == FALSE &&
		GD3D11.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
	{
		return 0 ;
	}

	// �ݒ�ɕύX�����������ǂ����̃t���O��|��
	GD3D11.Device.State.ChangeDepthStencilDesc = FALSE ;

	// ���ݐݒ肳��Ă���[�x�X�e���V���X�e�[�g������ꍇ�͉�������
	if( GD3D11.Device.State.DepthStencilState != NULL )
	{
		// ���݂̐ݒ��ύX����ۂɐ[�x�X�e���V���X�e�[�g�������[�X����w��ɂȂ��Ă����烊���[�X����
		if( GD3D11.Device.State.ChangeDepthStencilStateReleaseFlag )
		{
			Direct3D11_Release_DepthStencilState( GD3D11.Device.State.DepthStencilState ) ;
		}

		GD3D11.Device.State.DepthStencilState = NULL ;
	}

	// �ݒ��ύX����ۂɐ[�x�X�e���V���X�e�[�g�������[�X���邩�ǂ����̃t���O��|��
	GD3D11.Device.State.ChangeDepthStencilStateReleaseFlag = FALSE ;

	// �悭�g���ݒ�̐[�x�X�e���V���X�e�[�g���ǂ����𔻒肷��
	DepthStencilDesc = &GD3D11.Device.State.DepthStencilDesc ;
	if( DepthStencilDesc->StencilEnable					!= FALSE ||
		DepthStencilDesc->StencilReadMask				!= D_D3D11_DEFAULT_STENCIL_READ_MASK ||
		DepthStencilDesc->StencilWriteMask				!= D_D3D11_DEFAULT_STENCIL_WRITE_MASK ||
		DepthStencilDesc->FrontFace.StencilFailOp		!= D_D3D11_STENCIL_OP_KEEP ||
		DepthStencilDesc->FrontFace.StencilDepthFailOp	!= D_D3D11_STENCIL_OP_KEEP ||
		DepthStencilDesc->FrontFace.StencilPassOp		!= D_D3D11_STENCIL_OP_KEEP ||
		DepthStencilDesc->FrontFace.StencilFunc			!= D_D3D11_COMPARISON_ALWAYS ||
		DepthStencilDesc->BackFace.StencilFailOp		!= D_D3D11_STENCIL_OP_KEEP ||
		DepthStencilDesc->BackFace.StencilDepthFailOp	!= D_D3D11_STENCIL_OP_KEEP ||
		DepthStencilDesc->BackFace.StencilPassOp		!= D_D3D11_STENCIL_OP_KEEP ||
		DepthStencilDesc->BackFace.StencilFunc			!= D_D3D11_COMPARISON_ALWAYS )
	{
		// �悭�g���ݒ�̐[�x�X�e���V���X�e�[�g�ł͂Ȃ��ꍇ�͂����Ő[�x�X�e���V���X�e�[�g���쐬����
		hr = D3D11Device_CreateDepthStencilState( DepthStencilDesc, &NewDepthStencilState ) ;
		if( hr != S_OK )
		{
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\x49\x00\x44\x00\x33\x00\x44\x00\x31\x00\x31\x00\x44\x00\x65\x00\x70\x00\x74\x00\x68\x00\x53\x00\x74\x00\x65\x00\x6e\x00\x63\x00\x69\x00\x6c\x00\x53\x00\x74\x00\x61\x00\x74\x00\x65\x00\x20\x00\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x00\x30\x87\x7b\x40\x62\x11\xff\x0a\x00\x00"/*@ L"ID3D11DepthStencilState �̍쐬�Ɏ��s���܂����@�ӏ��P\n" @*/ )) ;
			return -1 ;
		}

		// �쐬�����[�x�X�e���V���X�e�[�g��ݒ�
		GD3D11.Device.State.DepthStencilState = NewDepthStencilState ;

		// ���̐[�x�X�e���V���X�e�[�g�͈ꎞ�I�Ȃ��̂Ȃ̂ŁA�I�����Ƀ����[�X����悤�ɂ���
		GD3D11.Device.State.ChangeDepthStencilStateReleaseFlag = TRUE ;
	}
	else
	{
		// �悭�g���ݒ�̐[�x�X�e���V���X�e�[�g�̏ꍇ

		// �w��̂悭�g���ݒ�̐[�x�X�e���V���X�e�[�g���܂��쐬����Ă��Ȃ�������쐬����
		DefaultDepthStencilState = &GD3D11.Device.State.DefaultDepthStencilStateArray[ DepthStencilDesc->DepthEnable ][ DepthStencilDesc->DepthWriteMask == D_D3D11_DEPTH_WRITE_MASK_ALL ? 1 : 0 ][ DepthStencilDesc->DepthFunc ] ;
		if( *DefaultDepthStencilState == NULL )
		{
			hr = D3D11Device_CreateDepthStencilState( DepthStencilDesc, &NewDepthStencilState ) ;
			if( hr != S_OK )
			{
				DXST_ERRORLOGFMT_ADDUTF16LE(( "\x49\x00\x44\x00\x33\x00\x44\x00\x31\x00\x31\x00\x44\x00\x65\x00\x70\x00\x74\x00\x68\x00\x53\x00\x74\x00\x65\x00\x6e\x00\x63\x00\x69\x00\x6c\x00\x53\x00\x74\x00\x61\x00\x74\x00\x65\x00\x20\x00\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x00\x30\x87\x7b\x40\x62\x12\xff\x0a\x00\x00"/*@ L"ID3D11DepthStencilState �̍쐬�Ɏ��s���܂����@�ӏ��Q\n" @*/ )) ;
				return -1 ;
			}
			*DefaultDepthStencilState = NewDepthStencilState ;
		}

		// �悭�g���ݒ�̐[�x�X�e���V���X�e�[�g��ݒ�
		GD3D11.Device.State.DepthStencilState = *DefaultDepthStencilState ;
	}

	// �[�x�X�e���V���X�e�[�g���f�o�C�X�R���e�L�X�g�ɐݒ肷��
	D3D11DeviceContext_OMSetDepthStencilState( GD3D11.Device.State.DepthStencilState, 0 ) ;

	// ����I��
	return 0 ;
}

// ID3D11DepthStencilState ���������
static int Graphics_D3D11_DeviceState_TerminateDepthStencilState( void )
{
	GD3D11.Device.State.ChangeDepthStencilDesc = FALSE ;

//	_MEMSET( &GD3D11.Device.State.DepthStencilDesc, 0, sizeof( GD3D11.Device.State.DepthStencilDesc ) ) ;

	if( GD3D11.Device.State.ChangeDepthStencilStateReleaseFlag )
	{
		Direct3D11_Release_DepthStencilState( GD3D11.Device.State.DepthStencilState ) ;
		GD3D11.Device.State.ChangeDepthStencilStateReleaseFlag = FALSE ;
	}
	GD3D11.Device.State.DepthStencilState = NULL ;

	Graphics_D3D11_DepthStencilStateArray_Release( ( D_ID3D11DepthStencilState ** )GD3D11.Device.State.DefaultDepthStencilStateArray, sizeof( GD3D11.Device.State.DefaultDepthStencilStateArray ) / sizeof( D_ID3D11DepthStencilState * ) ) ;

	// ����I��
	return 0 ;
}

// ���݂̐ݒ�Ɋ�Â��� ID3D11BlendState ���Z�b�g�A�b�v����
static int Graphics_D3D11_DeviceState_SetupBlendState( void )
{
	D_ID3D11BlendState *NewBlendState ;
	D_ID3D11BlendState **DefaultBlendState ;
	HRESULT            hr ;

	// �ݒ�ɕύX�������A�K���Đݒ���s���t���O���|��Ă����牽�������I��
	if( GD3D11.Device.State.ChangeBlendDesc == FALSE &&
		GD3D11.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
	{
		return 0 ;
	}

	// �ݒ�ɕύX�����������ǂ����̃t���O��|��
	GD3D11.Device.State.ChangeBlendDesc = FALSE ;

	// ���ݐݒ肳��Ă���u�����h�X�e�[�g������ꍇ�͉�������
	if( GD3D11.Device.State.BlendState != NULL )
	{
		// ���݂̐ݒ��ύX����ۂɃu�����h�X�e�[�g�������[�X����w��ɂȂ��Ă����烊���[�X����
		if( GD3D11.Device.State.ChangeBlendStateReleaseFlag )
		{
			Direct3D11_Release_BlendState( GD3D11.Device.State.BlendState ) ;
		}

		GD3D11.Device.State.BlendState = NULL ;
	}

	// �ݒ��ύX����ۂɃu�����h�X�e�[�g�������[�X���邩�ǂ����̃t���O��|��
	GD3D11.Device.State.ChangeBlendStateReleaseFlag = FALSE ;

	// �悭�g���ݒ�̃u�����h�X�e�[�g���ǂ����𔻒肷��
	if( GD3D11.Device.State.BlendMode == -1 )
	{
		// �悭�g���ݒ�̃u�����h�X�e�[�g�ł͂Ȃ��ꍇ�͂����Ńu�����h�X�e�[�g���쐬����
		hr = D3D11Device_CreateBlendState( &GD3D11.Device.State.BlendDesc, &NewBlendState ) ;
		if( hr != S_OK )
		{
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\x49\x00\x44\x00\x33\x00\x44\x00\x31\x00\x31\x00\x42\x00\x6c\x00\x65\x00\x6e\x00\x64\x00\x53\x00\x74\x00\x61\x00\x74\x00\x65\x00\x20\x00\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x00\x30\x87\x7b\x40\x62\x11\xff\x0a\x00\x00"/*@ L"ID3D11BlendState �̍쐬�Ɏ��s���܂����@�ӏ��P\n" @*/ )) ;
			return -1 ;
		}

		// �쐬�����u�����h�X�e�[�g��ݒ�
		GD3D11.Device.State.BlendState = NewBlendState ;

		// ���̃u�����h�X�e�[�g�͈ꎞ�I�Ȃ��̂Ȃ̂ŁA�I�����Ƀ����[�X����悤�ɂ���
		GD3D11.Device.State.ChangeBlendStateReleaseFlag = TRUE ;
	}
	else
	{
		// �悭�g���ݒ�̃u�����h�X�e�[�g�̏ꍇ

		// �w��̂悭�g���ݒ�̃u�����h�X�e�[�g���܂��쐬����Ă��Ȃ�������쐬����
		DefaultBlendState = &GD3D11.Device.State.DefaultBlendStateArray[ GD3D11.Device.State.BlendMode ] ;
		if( *DefaultBlendState == NULL )
		{
			D_D3D11_BLEND_DESC TempBlendDesc ;
			int i ;

			TempBlendDesc.AlphaToCoverageEnable  = FALSE ;
			TempBlendDesc.IndependentBlendEnable = FALSE ;
			for( i = 0; i < DX_RENDERTARGET_COUNT; i++ )
			{
				TempBlendDesc.RenderTarget[ i ] = g_DefaultBlendDescArray[ GD3D11.Device.State.BlendMode ].RenderTargetBlendDesc ;
			}

			hr = D3D11Device_CreateBlendState( &TempBlendDesc, &NewBlendState ) ;
			if( hr != S_OK )
			{
				DXST_ERRORLOGFMT_ADDUTF16LE(( "\x49\x00\x44\x00\x33\x00\x44\x00\x31\x00\x31\x00\x42\x00\x6c\x00\x65\x00\x6e\x00\x64\x00\x53\x00\x74\x00\x61\x00\x74\x00\x65\x00\x20\x00\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x00\x30\x87\x7b\x40\x62\x12\xff\x0a\x00\x00"/*@ L"ID3D11BlendState �̍쐬�Ɏ��s���܂����@�ӏ��Q\n" @*/ )) ;
				return -1 ;
			}
			*DefaultBlendState = NewBlendState ;
		}

		// �悭�g���ݒ�̃u�����h�X�e�[�g��ݒ�
		GD3D11.Device.State.BlendState = *DefaultBlendState ;
	}

	// �u�����h�X�e�[�g���f�o�C�X�R���e�L�X�g�ɐݒ肷��
	static FLOAT BlendFactor[ 4 ] = { 0.0f } ;
	D3D11DeviceContext_OMSetBlendState( GD3D11.Device.State.BlendState, BlendFactor, 0xffffffff ) ;

	// ����I��
	return 0 ;
}

// ID3D11BlendState ���������
static int Graphics_D3D11_DeviceState_TerminateBlendState( void )
{
	GD3D11.Device.State.ChangeBlendDesc = FALSE ;

//	_MEMSET( &GD3D11.Device.State.BlendDesc, 0, sizeof( GD3D11.Device.State.BlendDesc ) ) ;

	if( GD3D11.Device.State.ChangeBlendStateReleaseFlag )
	{
		Direct3D11_Release_BlendState( GD3D11.Device.State.BlendState ) ;
		GD3D11.Device.State.ChangeBlendStateReleaseFlag = FALSE ;
	}
	GD3D11.Device.State.BlendState = NULL ;

	Graphics_D3D11_BlendStateArray_Release( ( D_ID3D11BlendState ** )GD3D11.Device.State.DefaultBlendStateArray, sizeof( GD3D11.Device.State.DefaultBlendStateArray ) / sizeof( D_ID3D11BlendState * ) ) ;

	// ����I��
	return 0 ;
}

// �e�N�X�`���[�̃A�h���b�V���O���[�h���Z�b�g����
extern int  Graphics_D3D11_DeviceState_SetTextureAddress( int AddressMode, int Sampler )
{
	int								i ;
	D_D3D11_SAMPLER_DESC			*SamplerDesc ;
	D_D3D11_TEXTURE_ADDRESS_MODE	D3D11AddressMode ;

	if( GAPIWin.D3D11DeviceObject == NULL )
	{
		return -1 ;
	}

	D3D11AddressMode = ( D_D3D11_TEXTURE_ADDRESS_MODE )AddressMode ;

	if( Sampler < 0 )
	{
		if( GD3D11.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
		{
			SamplerDesc = GD3D11.Device.State.SamplerDesc ;
			for( i = 0 ; i < USE_TEXTURESTAGE_NUM ; i ++, SamplerDesc ++ )
			{
				if( D3D11AddressMode != SamplerDesc->AddressU ||
					D3D11AddressMode != SamplerDesc->AddressV ||
					D3D11AddressMode != SamplerDesc->AddressW )
				{
					break ;
				}
			}
			if( i == USE_TEXTURESTAGE_NUM )
			{
				return 0 ;
			}
		}

		DRAWSTOCKINFO

		SamplerDesc = GD3D11.Device.State.SamplerDesc ;
		for( i = 0 ; i < USE_TEXTURESTAGE_NUM ; i ++, SamplerDesc ++ )
		{
			if( D3D11AddressMode == SamplerDesc->AddressU &&
				D3D11AddressMode == SamplerDesc->AddressV &&
				D3D11AddressMode == SamplerDesc->AddressW &&
				GD3D11.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
			{
				continue ;
			}

			GD3D11.Device.State.ChangeSamplerDescSlot[ i ] = TRUE ;
			GD3D11.Device.State.ChangeSamplerDesc = TRUE ;

			SamplerDesc->AddressU = D3D11AddressMode ;
			SamplerDesc->AddressV = D3D11AddressMode ;
			SamplerDesc->AddressW = D3D11AddressMode ;
		}
	}
	else
	{
		if( Sampler < 0 || Sampler >= USE_TEXTURESTAGE_NUM )
		{
			return -1 ;
		}

		SamplerDesc = &GD3D11.Device.State.SamplerDesc[ Sampler ] ;
		if( D3D11AddressMode == SamplerDesc->AddressU &&
			D3D11AddressMode == SamplerDesc->AddressV &&
			D3D11AddressMode == SamplerDesc->AddressW &&
			GD3D11.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
		{
			return 0 ;
		}

		DRAWSTOCKINFO

		GD3D11.Device.State.ChangeSamplerDescSlot[ Sampler ] = TRUE ;
		GD3D11.Device.State.ChangeSamplerDesc = TRUE ;

		SamplerDesc->AddressU = D3D11AddressMode ;
		SamplerDesc->AddressV = D3D11AddressMode ;
		SamplerDesc->AddressW = D3D11AddressMode ;
	}

	GD3D11.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

//	return Graphics_D3D11_DeviceState_SetupSamplerState() ;
	return 0 ;
}

// �e�N�X�`���[�̃A�h���b�V���O���[�h���Z�b�g����
extern int Graphics_D3D11_DeviceState_SetTextureAddressU( int AddressMode, int Sampler )
{
	int								i ;
	D_D3D11_SAMPLER_DESC			*SamplerDesc ;
	D_D3D11_TEXTURE_ADDRESS_MODE	D3D11AddressMode ;

	if( GAPIWin.D3D11DeviceObject == NULL )
	{
		return -1 ;
	}

	D3D11AddressMode = ( D_D3D11_TEXTURE_ADDRESS_MODE )AddressMode ;

	if( Sampler == -1 )
	{
		if( GD3D11.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
		{
			SamplerDesc = GD3D11.Device.State.SamplerDesc ;
			for( i = 0 ; i < USE_TEXTURESTAGE_NUM ; i ++, SamplerDesc ++ )
			{
				if( D3D11AddressMode != SamplerDesc->AddressU )
				{
					break ;
				}
			}
			if( i == USE_TEXTURESTAGE_NUM )
			{
				return 0 ;
			}
		}

		DRAWSTOCKINFO

		SamplerDesc = GD3D11.Device.State.SamplerDesc ;
		for( i = 0 ; i < USE_TEXTURESTAGE_NUM ; i ++, SamplerDesc ++ )
		{
			if( D3D11AddressMode == SamplerDesc->AddressU &&
				GD3D11.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
			{
				continue ;
			}

			GD3D11.Device.State.ChangeSamplerDescSlot[ i ] = TRUE ;
			GD3D11.Device.State.ChangeSamplerDesc = TRUE ;

			SamplerDesc->AddressU = D3D11AddressMode ;
		}
	}
	else
	{
		if( Sampler < 0 || Sampler >= USE_TEXTURESTAGE_NUM )
		{
			return -1 ;
		}

		SamplerDesc = &GD3D11.Device.State.SamplerDesc[ Sampler ] ;
		if( D3D11AddressMode == SamplerDesc->AddressU &&
			GD3D11.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
		{
			return 0 ;
		}

		DRAWSTOCKINFO

		GD3D11.Device.State.ChangeSamplerDescSlot[ Sampler ] = TRUE ;
		GD3D11.Device.State.ChangeSamplerDesc = TRUE ;

		SamplerDesc->AddressU = D3D11AddressMode ;
	}

	GD3D11.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

//	return Graphics_D3D11_DeviceState_SetupSamplerState() ;
	return 0 ;
}

// �e�N�X�`���[�̃A�h���b�V���O���[�h���Z�b�g����
extern int Graphics_D3D11_DeviceState_SetTextureAddressV( int AddressMode, int Sampler )
{
	int								i ;
	D_D3D11_SAMPLER_DESC			*SamplerDesc ;
	D_D3D11_TEXTURE_ADDRESS_MODE	D3D11AddressMode ;

	if( GAPIWin.D3D11DeviceObject == NULL )
	{
		return -1 ;
	}

	D3D11AddressMode = ( D_D3D11_TEXTURE_ADDRESS_MODE )AddressMode ;

	if( Sampler == -1 )
	{
		if( GD3D11.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
		{
			SamplerDesc = GD3D11.Device.State.SamplerDesc ;
			for( i = 0 ; i < USE_TEXTURESTAGE_NUM ; i ++, SamplerDesc ++ )
			{
				if( D3D11AddressMode != SamplerDesc->AddressV )
				{
					break ;
				}
			}
			if( i == USE_TEXTURESTAGE_NUM )
			{
				return 0 ;
			}
		}

		DRAWSTOCKINFO

		SamplerDesc = GD3D11.Device.State.SamplerDesc ;
		for( i = 0 ; i < USE_TEXTURESTAGE_NUM ; i ++, SamplerDesc ++ )
		{
			if( D3D11AddressMode == SamplerDesc->AddressV &&
				GD3D11.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
			{
				continue ;
			}

			GD3D11.Device.State.ChangeSamplerDescSlot[ i ] = TRUE ;
			GD3D11.Device.State.ChangeSamplerDesc = TRUE ;

			SamplerDesc->AddressV = D3D11AddressMode ;
		}
	}
	else
	{
		if( Sampler < 0 || Sampler >= USE_TEXTURESTAGE_NUM )
		{
			return -1 ;
		}

		SamplerDesc = &GD3D11.Device.State.SamplerDesc[ Sampler ] ;
		if( D3D11AddressMode == SamplerDesc->AddressV &&
			GD3D11.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
		{
			return 0 ;
		}

		DRAWSTOCKINFO

		GD3D11.Device.State.ChangeSamplerDescSlot[ Sampler ] = TRUE ;
		GD3D11.Device.State.ChangeSamplerDesc = TRUE ;

		SamplerDesc->AddressV = D3D11AddressMode ;
	}

	GD3D11.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

//	return Graphics_D3D11_DeviceState_SetupSamplerState() ;
	return 0 ;
}

// �e�N�X�`���[�̃A�h���b�V���O���[�h���Z�b�g����
extern int Graphics_D3D11_DeviceState_SetTextureAddressW( int AddressMode, int Sampler )
{
	int								i ;
	D_D3D11_SAMPLER_DESC			*SamplerDesc ;
	D_D3D11_TEXTURE_ADDRESS_MODE	D3D11AddressMode ;

	if( GAPIWin.D3D11DeviceObject == NULL )
	{
		return -1 ;
	}

	D3D11AddressMode = ( D_D3D11_TEXTURE_ADDRESS_MODE )AddressMode ;

	if( Sampler == -1 )
	{
		if( GD3D11.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
		{
			SamplerDesc = GD3D11.Device.State.SamplerDesc ;
			for( i = 0 ; i < USE_TEXTURESTAGE_NUM ; i ++, SamplerDesc ++ )
			{
				if( D3D11AddressMode != SamplerDesc->AddressW )
				{
					break ;
				}
			}
			if( i == USE_TEXTURESTAGE_NUM )
			{
				return 0 ;
			}
		}

		DRAWSTOCKINFO

		SamplerDesc = GD3D11.Device.State.SamplerDesc ;
		for( i = 0 ; i < USE_TEXTURESTAGE_NUM ; i ++, SamplerDesc ++ )
		{
			if( D3D11AddressMode == SamplerDesc->AddressW &&
				GD3D11.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
			{
				continue ;
			}

			GD3D11.Device.State.ChangeSamplerDescSlot[ i ] = TRUE ;
			GD3D11.Device.State.ChangeSamplerDesc = TRUE ;

			SamplerDesc->AddressW = D3D11AddressMode ;
		}
	}
	else
	{
		if( Sampler < 0 || Sampler >= USE_TEXTURESTAGE_NUM )
		{
			return -1 ;
		}

		SamplerDesc = &GD3D11.Device.State.SamplerDesc[ Sampler ] ;
		if( D3D11AddressMode == SamplerDesc->AddressW &&
			GD3D11.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
		{
			return 0 ;
		}

		DRAWSTOCKINFO

		GD3D11.Device.State.ChangeSamplerDescSlot[ Sampler ] = TRUE ;
		GD3D11.Device.State.ChangeSamplerDesc = TRUE ;

		SamplerDesc->AddressW = D3D11AddressMode ;

	}

	GD3D11.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

//	return Graphics_D3D11_DeviceState_SetupSamplerState() ;
	return 0 ;
}

// �e�N�X�`���[�̃A�h���b�V���O���[�h���Z�b�g����
extern int Graphics_D3D11_DeviceState_SetTextureAddressUVW( int AddressModeU, int AddressModeV, int AddressModeW, int Sampler )
{
	if( Graphics_D3D11_DeviceState_SetTextureAddressU( AddressModeU, Sampler ) < 0 )
	{
		return -1 ;
	}

	if( Graphics_D3D11_DeviceState_SetTextureAddressV( AddressModeV, Sampler ) < 0 )
	{
		return -1 ;
	}

	if( Graphics_D3D11_DeviceState_SetTextureAddressW( AddressModeW, Sampler ) < 0 )
	{
		return -1 ;
	}

	return 0 ;
}

// �W���`��p�̃e�N�X�`�����W�ϊ��s���萔�o�b�t�@�ɃZ�b�g����
static int Graphics_D3D11_DeviceState_SetNormalTextureAddressTransformMatrix_ConstBuffer( void )
{
	DX_D3D11_VS_CONST_BUFFER_OTHERMATRIX *ConstOtherMatrix ;

	if( GAPIWin.D3D11DeviceObject == NULL )
	{
		return -1 ;
	}

	ConstOtherMatrix = ( DX_D3D11_VS_CONST_BUFFER_OTHERMATRIX * )GD3D11.Device.Shader.Constant.ConstBuffer_VS_OtherMatrix->SysmemBuffer ;

	// �萔�f�[�^�ɔ��f
	ConstOtherMatrix->TextureMatrix[ 0 ][ 0 ][ 0 ] = GD3D11.Device.State.TextureAddressTransformMatrix.m[ 0 ][ 0 ] ;
	ConstOtherMatrix->TextureMatrix[ 0 ][ 0 ][ 1 ] = GD3D11.Device.State.TextureAddressTransformMatrix.m[ 1 ][ 0 ] ;
	ConstOtherMatrix->TextureMatrix[ 0 ][ 0 ][ 2 ] = GD3D11.Device.State.TextureAddressTransformMatrix.m[ 2 ][ 0 ] ;
	ConstOtherMatrix->TextureMatrix[ 0 ][ 0 ][ 3 ] = GD3D11.Device.State.TextureAddressTransformMatrix.m[ 3 ][ 0 ] ;
	ConstOtherMatrix->TextureMatrix[ 0 ][ 1 ][ 0 ] = GD3D11.Device.State.TextureAddressTransformMatrix.m[ 0 ][ 1 ] ;
	ConstOtherMatrix->TextureMatrix[ 0 ][ 1 ][ 1 ] = GD3D11.Device.State.TextureAddressTransformMatrix.m[ 1 ][ 1 ] ;
	ConstOtherMatrix->TextureMatrix[ 0 ][ 1 ][ 2 ] = GD3D11.Device.State.TextureAddressTransformMatrix.m[ 2 ][ 1 ] ;
	ConstOtherMatrix->TextureMatrix[ 0 ][ 1 ][ 3 ] = GD3D11.Device.State.TextureAddressTransformMatrix.m[ 3 ][ 1 ] ;
	GD3D11.Device.Shader.Constant.ConstBuffer_VS_OtherMatrix->ChangeFlag = TRUE ;

	// �萔�o�b�t�@���X�V
//	Graphics_D3D11_ConstantBuffer_Update( GD3D11.Device.Shader.Constant.ConstBuffer_VS_OtherMatrix ) ;

	// �W���`��p�̃e�N�X�`���p�����[�^���ݒ肳��Ă��邩�ǂ����t���O�𗧂Ă�
	GD3D11.Device.Shader.Constant.SetNormalTextureAddressTransformMatrix = TRUE ;

	// ����I��
	return 0 ;
}

// �e�N�X�`�����W�ϊ��s����Z�b�g����
extern int  Graphics_D3D11_DeviceState_SetTextureAddressTransformMatrix( int Use, MATRIX *Matrix )
{
	if( GAPIWin.D3D11DeviceObject == NULL )
	{
		return -1 ;
	}

	if( GD3D11.Device.State.TextureAddressTransformMatrixUse == FALSE &&
		Use == FALSE && 
		GD3D11.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
	{
		return 0 ;
	}

	DRAWSTOCKINFO

	// �e�N�X�`�����W�ϊ��s����g�p���邩�ǂ�����ۑ�
	GD3D11.Device.State.TextureAddressTransformMatrixUse = Use ;

	// �e�N�X�`�����W�ϊ��s����Z�b�g
	if( Use == TRUE )
	{
		GD3D11.Device.State.TextureAddressTransformMatrix = *Matrix ;
	}
	else
	{
		CreateIdentityMatrix( &GD3D11.Device.State.TextureAddressTransformMatrix ) ;
	}

	// �萔�o�b�t�@�ɔ��f����
	Graphics_D3D11_DeviceState_SetNormalTextureAddressTransformMatrix_ConstBuffer() ;

	return 0 ;
}

// �t�H�O��L���ɂ��邩�ǂ�����ݒ肷��( TRUE:�L��  FALSE:���� )
extern int  Graphics_D3D11_DeviceState_SetFogEnable( int Flag )
{
	if( GAPIWin.D3D11DeviceObject == NULL )
	{
		return -1 ;
	}

	if( Flag == GD3D11.Device.State.FogEnable &&
		GD3D11.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
	{
		return 0 ;
	}

	DRAWSTOCKINFO

	GD3D11.Device.State.FogEnable = Flag ;

	GD3D11.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

	// �I��
	return 0 ;
}

// �t�H�O���[�h��ݒ肷��
extern int  Graphics_D3D11_DeviceState_SetFogVertexMode( int Mode /* DX_FOGMODE_NONE �� */ )
{
	if( GAPIWin.D3D11DeviceObject == NULL )
	{
		return -1 ;
	}

	if( Mode == GD3D11.Device.State.FogMode &&
		GD3D11.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
	{
		return 0 ;
	}

	DRAWSTOCKINFO

	GD3D11.Device.State.FogMode = Mode ;

	GD3D11.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

	// �I��
	return 0 ;
}

// �t�H�O�J���[��ύX����
extern int  Graphics_D3D11_DeviceState_SetFogColor( unsigned int Color )
{
	DX_D3D11_CONST_BUFFER_COMMON *ConstantCommon ;

	if( GAPIWin.D3D11DeviceObject == NULL )
	{
		return -1 ;
	}

	if( Color == GD3D11.Device.State.FogColor &&
		GD3D11.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
	{
		return 0 ;
	}

	ConstantCommon = ( DX_D3D11_CONST_BUFFER_COMMON * )GD3D11.Device.Shader.Constant.ConstBuffer_Common->SysmemBuffer ;

	DRAWSTOCKINFO

	// �萔�f�[�^�ɔ��f
	ConstantCommon->Fog.Color[ 0 ] = ( ( Color >> 16 ) & 0xff ) / 255.0f ;
	ConstantCommon->Fog.Color[ 1 ] = ( ( Color >>  8 ) & 0xff ) / 255.0f ;
	ConstantCommon->Fog.Color[ 2 ] = ( ( Color >>  0 ) & 0xff ) / 255.0f ;
	ConstantCommon->Fog.Color[ 3 ] = 1.0f ;
	GD3D11.Device.Shader.Constant.ConstBuffer_Common->ChangeFlag = TRUE ;

	// �萔�o�b�t�@���X�V
//	Graphics_D3D11_ConstantBuffer_Update( GD3D11.Device.Shader.Constant.ConstBuffer_Common ) ;

	GD3D11.Device.State.FogColor = Color ;

	GD3D11.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

	// �I��
	return 0 ;
}

// �t�H�O�̐F�ȊO�̒萔�����X�V����
static int Graphics_D3D11_DeviceState_UpdateConstantFogParam( void )
{
	DX_D3D11_CONST_BUFFER_COMMON *ConstantCommon ;

	if( GAPIWin.D3D11DeviceObject == NULL )
	{
		return -1 ;
	}

	ConstantCommon = ( DX_D3D11_CONST_BUFFER_COMMON * )GD3D11.Device.Shader.Constant.ConstBuffer_Common->SysmemBuffer ;

	// �萔�f�[�^�ɔ��f
	ConstantCommon->Fog.LinearAdd = GD3D11.Device.State.FogEnd / ( GD3D11.Device.State.FogEnd - GD3D11.Device.State.FogStart ) ;
	ConstantCommon->Fog.LinearDiv = -1.0f / ( GD3D11.Device.State.FogEnd - GD3D11.Device.State.FogStart ) ;
	ConstantCommon->Fog.Density   = GD3D11.Device.State.FogDensity ;
	ConstantCommon->Fog.E         = 2.71828183f ;
	GD3D11.Device.Shader.Constant.ConstBuffer_Common->ChangeFlag = TRUE ;

	// �萔�o�b�t�@���X�V
//	Graphics_D3D11_ConstantBuffer_Update( GD3D11.Device.Shader.Constant.ConstBuffer_Common ) ;

	// �I��
	return 0 ;
}

// �t�H�O���n�܂鋗���ƏI�����鋗����ݒ肷��( 0.0f �` 1.0f )
extern int  Graphics_D3D11_DeviceState_SetFogStartEnd( float Start, float End )
{
	int UpdateFlag ;

	if( GAPIWin.D3D11DeviceObject == NULL )
	{
		return -1 ;
	}

	DRAWSTOCKINFO

	UpdateFlag = FALSE ;

	if( Start != GD3D11.Device.State.FogStart ||
		GD3D11.Device.DrawSetting.CancelSettingEqualCheck == TRUE )
	{
		GD3D11.Device.State.FogStart = Start ;
		UpdateFlag = TRUE ;
	}

	if( End != GD3D11.Device.State.FogEnd ||
		GD3D11.Device.DrawSetting.CancelSettingEqualCheck == TRUE )
	{
		GD3D11.Device.State.FogEnd = End ;
		UpdateFlag = TRUE ;
	}

	if( UpdateFlag )
	{
		// �萔�����X�V
		Graphics_D3D11_DeviceState_UpdateConstantFogParam() ;
	}

	GD3D11.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

	// �I��
	return 0 ;
}

// �t�H�O�̖��x��ݒ肷��( 0.0f �` 1.0f )
extern int  Graphics_D3D11_DeviceState_SetFogDensity( float Density )
{
	if( GAPIWin.D3D11DeviceObject == NULL )
	{
		return -1 ;
	}

	if( Density == GD3D11.Device.State.FogDensity &&
		GD3D11.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
	{
		return 0 ;
	}

	DRAWSTOCKINFO

	GD3D11.Device.State.FogDensity = Density ;

	// �萔�����X�V
	Graphics_D3D11_DeviceState_UpdateConstantFogParam() ;

	GD3D11.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

	// �I��
	return 0 ;
}

// ���C�e�B���O�̗L���t���O���Z�b�g����
extern int  Graphics_D3D11_DeviceState_SetLighting( int UseFlag )
{
	if( GAPIWin.D3D11DeviceObject == NULL )
	{
		return -1 ;
	}

	if( UseFlag == GD3D11.Device.State.Lighting &&
		GD3D11.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
	{
		return 0 ;
	}

	DRAWSTOCKINFO

	GD3D11.Device.State.Lighting = UseFlag ;

	GD3D11.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

	// �I��
	return 0 ;
}

// �ő�ٕ������Z�b�g����
extern	int		Graphics_D3D11_DeviceState_SetMaxAnisotropy( int MaxAnisotropy, int Sampler )
{
	int						i ;
	D_D3D11_SAMPLER_DESC	*SamplerDesc ;

	if( GAPIWin.D3D11DeviceObject == NULL )
	{
		return -1 ;
	}

	// �l�̕␳
	if( MaxAnisotropy < 1 )
	{
		MaxAnisotropy = 1 ;
	}

	if( Sampler < 0 )
	{
		if( GD3D11.Device.State.MaxAnisotropy == MaxAnisotropy &&
			GD3D11.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
		{
			return 0 ;
		}

		GD3D11.Device.State.MaxAnisotropy = MaxAnisotropy ;

		if( GD3D11.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
		{
			SamplerDesc = GD3D11.Device.State.SamplerDesc ;
			for( i = 0 ; i < USE_TEXTURESTAGE_NUM ; i ++, SamplerDesc ++ )
			{
				if( SamplerDesc->MaxAnisotropy != ( UINT )MaxAnisotropy )
				{
					break ;
				}
			}
			if( i == USE_TEXTURESTAGE_NUM )
			{
				return 0 ;
			}
		}

		DRAWSTOCKINFO

		SamplerDesc = GD3D11.Device.State.SamplerDesc ;
		for( i = 0 ; i < USE_TEXTURESTAGE_NUM ; i ++, SamplerDesc ++ )
		{
			if( SamplerDesc->MaxAnisotropy == ( UINT )MaxAnisotropy &&
				GD3D11.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
			{
				continue ;
			}

			GD3D11.Device.State.ChangeSamplerDescSlot[ i ] = TRUE ;
			GD3D11.Device.State.ChangeSamplerDesc = TRUE ;

			SamplerDesc->MaxAnisotropy = ( UINT )MaxAnisotropy ;
		}
	}
	else
	{
		if( Sampler < 0 || Sampler >= USE_TEXTURESTAGE_NUM )
		{
			return 0 ;
		}

		GD3D11.Device.State.MaxAnisotropy = -1 ;

		SamplerDesc = &GD3D11.Device.State.SamplerDesc[ Sampler ] ;
		if( SamplerDesc->MaxAnisotropy == ( UINT )MaxAnisotropy &&
			GD3D11.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
		{
			return 0 ;
		}

		DRAWSTOCKINFO

		GD3D11.Device.State.ChangeSamplerDescSlot[ Sampler ] = TRUE ;
		GD3D11.Device.State.ChangeSamplerDesc = TRUE ;

		SamplerDesc->MaxAnisotropy = ( UINT )MaxAnisotropy ;
	}

	GD3D11.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

//	return Graphics_D3D11_DeviceState_SetupSamplerState() ;
	return 0 ;
}

// �r���[�|�[�g���Z�b�g����
extern	int		Graphics_D3D11_DeviceState_SetViewport( D_D3D11_VIEWPORT *Viewport )
{
	if( GAPIWin.D3D11DeviceObject == NULL )
	{
		return -1 ;
	}

	if( Viewport->TopLeftX == GD3D11.Device.State.Viewport.TopLeftX &&
		Viewport->TopLeftY == GD3D11.Device.State.Viewport.TopLeftY &&
		Viewport->Width    == GD3D11.Device.State.Viewport.Width    &&
		Viewport->Height   == GD3D11.Device.State.Viewport.Height   &&
		GD3D11.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
	{
		return 0 ;
	}

	// �����o��
	if( GD3D11.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
	{
		DRAWSTOCKINFO
	}

	// �`����I�������Ă���
	Graphics_D3D11_RenderEnd() ;

	// �r���[�|�[�g�̃Z�b�e�B���O
	D3D11DeviceContext_RSSetViewports( 1, Viewport ) ;

	// �r���[�|�[�g�̏��̕ۑ�
	GD3D11.Device.State.Viewport = *Viewport ;

	// �I��
	return 0 ;
}

// �r���[�|�[�g���Z�b�g����( �ȈՔ� )
extern	int		Graphics_D3D11_DeviceState_SetViewportEasy( float x1, float y1, float x2, float y2 )
{
	D_D3D11_VIEWPORT Viewport ;

	Viewport.TopLeftX = x1 ;
	Viewport.TopLeftY = y1 ;
	Viewport.Width    = x2 - x1 ;
	Viewport.Height   = y2 - y1 ;
	Viewport.MinDepth = 0.0f ;
	Viewport.MaxDepth = 1.0f ;

	return Graphics_D3D11_DeviceState_SetViewport( &Viewport ) ;
}

// �`�惂�[�h�̃Z�b�g
extern	int		Graphics_D3D11_DeviceState_SetDrawMode( int DrawMode )
{
	if( GAPIWin.D3D11DeviceObject == NULL )
	{
		return -1 ;
	}

	if( GD3D11.Device.State.DrawMode == DrawMode &&
		GD3D11.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
	{
		return 0 ;
	}

	if( GD3D11.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
	{
		DRAWSTOCKINFO
	}

	// �����_�����O�ݒ��ύX����
	switch( DrawMode )
	{
	case DX_DRAWMODE_BILINEAR :
		// �o�C���j�A�`��
		Graphics_D3D11_DeviceState_SetSampleFilterMode( D_D3D11_FILTER_MIN_MAG_MIP_LINEAR, -1 ) ;
		break ;

	case DX_DRAWMODE_NEAREST :
		// ��A���X�g�l�C�o�[
		Graphics_D3D11_DeviceState_SetSampleFilterMode( D_D3D11_FILTER_MIN_MAG_MIP_POINT, -1 ) ;
		break ;

	case DX_DRAWMODE_ANISOTROPIC :
		// �ٕ����t�B���^�����O
		Graphics_D3D11_DeviceState_SetSampleFilterMode( D_D3D11_FILTER_ANISOTROPIC, -1 ) ;
		break ;

	default : return -1 ;
	}

	GD3D11.Device.State.DrawMode                 = DrawMode ;
	GD3D11.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

	// �I��
	return 0 ;
}

// �A���t�@�e�X�g�Ŏg�p�����r���[�h��ݒ肷��
extern	int		Graphics_D3D11_DeviceState_SetAlphaTestCmpMode( int AlphaTestCmpMode /* DX_CMP_NEVER �Ȃ� */ )
{
	DX_D3D11_PS_CONST_BUFFER_BASE *ConstantPSBase ;

	if( GAPIWin.D3D11DeviceObject == NULL )
	{
		return -1 ;
	}

	if( GD3D11.Device.State.AlphaTestCmpMode == AlphaTestCmpMode &&
		GD3D11.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
	{
		return 0 ;
	}

	ConstantPSBase = ( DX_D3D11_PS_CONST_BUFFER_BASE * )GD3D11.Device.Shader.Constant.ConstBuffer_PS_Base->SysmemBuffer ;

	DRAWSTOCKINFO

	// �l��ۑ�
	GD3D11.Device.State.AlphaTestCmpMode = AlphaTestCmpMode ;

	// �萔�f�[�^�ɔ��f
	ConstantPSBase->AlphaTestCmpMode = AlphaTestCmpMode ;

	GD3D11.Device.Shader.Constant.ConstBuffer_PS_Base->ChangeFlag = TRUE ;
	
	// �萔�o�b�t�@���X�V
//	Graphics_D3D11_ConstantBuffer_Update( GD3D11.Device.Shader.Constant.ConstBuffer_PS_Base ) ;

	// �I��
	return 0 ;
}

// �A���t�@�e�X�g�Ŏg�p����Q�ƒl��ݒ肷��
extern	int		Graphics_D3D11_DeviceState_SetAlphaTestRef( int AlphaTestRef )
{
	DX_D3D11_PS_CONST_BUFFER_BASE *ConstantPSBase ;

	if( GAPIWin.D3D11DeviceObject == NULL )
	{
		return -1 ;
	}

	if( GD3D11.Device.State.AlphaTestRef == AlphaTestRef &&
		GD3D11.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
	{
		return 0 ;
	}

	ConstantPSBase = ( DX_D3D11_PS_CONST_BUFFER_BASE * )GD3D11.Device.Shader.Constant.ConstBuffer_PS_Base->SysmemBuffer ;

	DRAWSTOCKINFO

	// �l��ۑ�
	GD3D11.Device.State.AlphaTestRef = AlphaTestRef ;

	// �萔�f�[�^�ɔ��f
	ConstantPSBase->AlphaTestRef = AlphaTestRef / 255.0f ;

	GD3D11.Device.Shader.Constant.ConstBuffer_PS_Base->ChangeFlag = TRUE ;
	
	// �萔�o�b�t�@���X�V
//	Graphics_D3D11_ConstantBuffer_Update( GD3D11.Device.Shader.Constant.ConstBuffer_PS_Base ) ;

	// �I��
	return 0 ;
}

// �J���[�ɃA���t�@�l����Z���邩�ǂ�����ݒ肷��
extern	int		Graphics_D3D11_DeviceState_SetMulAlphaColor( int UseMulAlphaColor )
{
	DX_D3D11_PS_CONST_BUFFER_BASE *ConstantPSBase ;

	if( GAPIWin.D3D11DeviceObject == NULL )
	{
		return -1 ;
	}

	ConstantPSBase = ( DX_D3D11_PS_CONST_BUFFER_BASE * )GD3D11.Device.Shader.Constant.ConstBuffer_PS_Base->SysmemBuffer ;

	DRAWSTOCKINFO

	// �萔�f�[�^�ɔ��f
	ConstantPSBase->MulAlphaColor = UseMulAlphaColor ? 1.0f : 0.0f ;

	GD3D11.Device.Shader.Constant.ConstBuffer_PS_Base->ChangeFlag = TRUE ;
	
	// �萔�o�b�t�@���X�V
//	Graphics_D3D11_ConstantBuffer_Update( GD3D11.Device.Shader.Constant.ConstBuffer_PS_Base ) ;

	// �I��
	return 0 ;
}

// �f�o�C�X�Ŏg�p����v���~�e�B�u�^�C�v��ݒ肷��
extern	int		Graphics_D3D11_DeviceState_SetPrimitiveTopology( D_D3D11_PRIMITIVE_TOPOLOGY PrimitiveTopology )
{
	if( GAPIWin.D3D11DeviceObject == NULL )
	{
		return -1 ;
	}

	if( GD3D11.Device.State.PrimitiveTopology == PrimitiveTopology &&
		GD3D11.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
	{
		return 0 ;
	}

	// �l��ۑ�
	GD3D11.Device.State.PrimitiveTopology = PrimitiveTopology ;

	// �v���~�e�B�u�^�C�v���Z�b�g
	D3D11DeviceContext_IASetPrimitiveTopology( GD3D11.Device.State.PrimitiveTopology ) ;

	// �I��
	return 0 ;
}

// Factor Color ��ݒ肷��
extern	int		Graphics_D3D11_DeviceState_SetFactorColor( const DX_D3D11_SHADER_FLOAT4 *FactorColor )
{
	DX_D3D11_PS_CONST_BUFFER_BASE *ConstantPSBase ;

	if( GAPIWin.D3D11DeviceObject == NULL )
	{
		return -1 ;
	}

	ConstantPSBase = ( DX_D3D11_PS_CONST_BUFFER_BASE * )GD3D11.Device.Shader.Constant.ConstBuffer_PS_Base->SysmemBuffer ;

	if( ConstantPSBase->FactorColor[ 0 ] == (*FactorColor)[ 0 ] &&
		ConstantPSBase->FactorColor[ 1 ] == (*FactorColor)[ 1 ] &&
		ConstantPSBase->FactorColor[ 2 ] == (*FactorColor)[ 2 ] &&
		ConstantPSBase->FactorColor[ 3 ] == (*FactorColor)[ 3 ] &&
		GD3D11.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
	{
		return 0 ;
	}

	DRAWSTOCKINFO

	// �萔�f�[�^�ɔ��f
	ConstantPSBase->FactorColor[ 0 ] = (*FactorColor)[ 0 ] ;
	ConstantPSBase->FactorColor[ 1 ] = (*FactorColor)[ 1 ] ;
	ConstantPSBase->FactorColor[ 2 ] = (*FactorColor)[ 2 ] ;
	ConstantPSBase->FactorColor[ 3 ] = (*FactorColor)[ 3 ] ;

	GD3D11.Device.Shader.Constant.ConstBuffer_PS_Base->ChangeFlag = TRUE ;
	
	// �萔�o�b�t�@���X�V
//	Graphics_D3D11_ConstantBuffer_Update( GD3D11.Device.Shader.Constant.ConstBuffer_PS_Base ) ;

	// �I��
	return 0 ;
}

// �g�D�[�������_�����O�̗֊s���̑�����ݒ肷��
extern int Graphics_D3D11_DeviceState_SetToonOutLineSize( float Size )
{
	DX_D3D11_VS_CONST_BUFFER_BASE *ConstantVSBase ;

	if( GAPIWin.D3D11DeviceObject == NULL )
	{
		return -1 ;
	}

	ConstantVSBase = ( DX_D3D11_VS_CONST_BUFFER_BASE * )GD3D11.Device.Shader.Constant.ConstBuffer_VS_Base->SysmemBuffer ;

	if( ConstantVSBase->ToonOutLineSize[ 0 ] == Size &&
		GD3D11.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
	{
		return 0 ;
	}

	DRAWSTOCKINFO

	// �萔�f�[�^�ɔ��f
	ConstantVSBase->ToonOutLineSize[ 0 ] = Size ;
	ConstantVSBase->ToonOutLineSize[ 1 ] = Size ;
	ConstantVSBase->ToonOutLineSize[ 2 ] = Size ;
	ConstantVSBase->ToonOutLineSize[ 3 ] = Size ;

	GD3D11.Device.Shader.Constant.ConstBuffer_VS_Base->ChangeFlag = TRUE ;
	
	// �萔�o�b�t�@���X�V
//	Graphics_D3D11_ConstantBuffer_Update( GD3D11.Device.Shader.Constant.ConstBuffer_VS_Base ) ;

	// �I��
	return 0 ;
}

// �`��u�����h���[�h�̃Z�b�g
extern	int		Graphics_D3D11_DeviceState_SetBlendMode( int BlendMode )
{
	if( GAPIWin.D3D11DeviceObject == NULL )
	{
		return -1 ;
	}

	if( GD3D11.Device.State.BlendMode == BlendMode &&
		GD3D11.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
	{
		return 0 ;
	}

	DRAWSTOCKINFO

	GD3D11.Device.State.BlendMode = BlendMode ;
	GD3D11.Device.State.ChangeBlendDesc = TRUE ;

	GD3D11.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

//	return Graphics_D3D11_DeviceState_SetupBlendState() ;
	return 0 ;
}

// �S�Ă̕`��Ώۂ̃T�C�Y����v���Ă����烌���_�����O�^�[�Q�b�g�ɃZ�b�g
static	void	Graphics_D3D11_DeviceState_SetRenderTarget_SizeCheck( void )
{
	int i, Base ;

	for( Base = 0; Base < DX_RENDERTARGET_COUNT ; Base ++ )
	{
		if( GD3D11.Device.State.TargetTexture2D[ Base ] != NULL )
		{
			break ;
		}
	}

	for( i = Base + 1 ; i < DX_RENDERTARGET_COUNT ; i ++ )
	{
		if( GD3D11.Device.State.TargetTexture2D[ i ] != NULL &&
			( GD3D11.Device.State.TargetTexture2DDesc[ i ].Width  != GD3D11.Device.State.TargetTexture2DDesc[ Base ].Width  ||
			  GD3D11.Device.State.TargetTexture2DDesc[ i ].Height != GD3D11.Device.State.TargetTexture2DDesc[ Base ].Height ) )
		{
			return ;
		}
	}

	if( GD3D11.Device.State.TargetDepthTexture2D != NULL &&
		( GD3D11.Device.State.TargetDepthTexture2DDesc.Width  != GD3D11.Device.State.TargetTexture2DDesc[ Base ].Width  ||
		  GD3D11.Device.State.TargetDepthTexture2DDesc.Height != GD3D11.Device.State.TargetTexture2DDesc[ Base ].Height ) )
	{
		return ;
	}

	D3D11DeviceContext_OMSetRenderTargets( DX_RENDERTARGET_COUNT, GD3D11.Device.State.TargetRTV, GD3D11.Device.State.TargetDepthDSV ) ;
}

// �`��Ώۂ̕ύX
extern	int		Graphics_D3D11_DeviceState_SetRenderTarget( D_ID3D11Texture2D *RenderTargetTexture, D_ID3D11RenderTargetView *RenderTargetView, int TargetIndex )
{
	D_D3D11_TEXTURE2D_DESC      Texture2DDesc ;

	if( GAPIWin.D3D11DeviceObject == NULL )
	{
		return -1 ;
	}

	if( TargetIndex < 0 || TargetIndex >= DX_RENDERTARGET_COUNT )
	{
		return -1 ;
	}

	if( GD3D11.Device.State.TargetTexture2D[ TargetIndex ] == RenderTargetTexture &&
		GD3D11.Device.State.TargetRTV[ TargetIndex ]       == RenderTargetView &&
		GD3D11.Device.DrawSetting.CancelSettingEqualCheck  == FALSE )
	{
		return 0 ;
	}

	// ���_�����o��
	DRAWSTOCKINFO

	// �����_�����O�^�[�Q�b�g�̕ۑ�
	GD3D11.Device.State.TargetTexture2D[ TargetIndex ] = RenderTargetTexture ;
	GD3D11.Device.State.TargetRTV[ TargetIndex ]       = RenderTargetView ;
	if( RenderTargetTexture != NULL )
	{
		RenderTargetTexture->GetDesc( &GD3D11.Device.State.TargetTexture2DDesc[ TargetIndex ] ) ;
	}
	else
	{
		_MEMSET( &GD3D11.Device.State.TargetTexture2DDesc[ TargetIndex ], 0, sizeof( D_D3D11_TEXTURE2D_DESC ) ) ;
	}

	// �����_�����O�^�[�Q�b�g�̕ύX
	Graphics_D3D11_DeviceState_SetRenderTarget_SizeCheck() ;

	// �r���[�|�[�g�̐ݒ�l���X�V
	if( TargetIndex == 0 && RenderTargetTexture != NULL )
	{
		D3D11Texture2D_GetDesc( RenderTargetTexture, &Texture2DDesc ) ; 
		Graphics_D3D11_DeviceState_SetViewportEasy( 0.0f, 0.0f, ( float )Texture2DDesc.Width, ( float )Texture2DDesc.Height ) ;
	}

	// �I��
	return  0 ;
}

// �`��Ώۂ̐[�x�o�b�t�@��ύX
extern	int		Graphics_D3D11_DeviceState_SetDepthStencil( D_ID3D11Texture2D *DepthStencilTexture, D_ID3D11DepthStencilView *DepthStencilView )
{
	if( GAPIWin.D3D11DeviceObject == NULL )
	{
		return -1 ;
	}

	if( GD3D11.Device.State.TargetDepthTexture2D == DepthStencilTexture &&
		GD3D11.Device.State.TargetDepthDSV       == DepthStencilView &&
		GD3D11.Device.DrawSetting.CancelSettingEqualCheck  == FALSE )
	{
		return 0 ;
	}

	// ���_�����o��
	DRAWSTOCKINFO

	// �g�p����[�x�o�b�t�@�̕ۑ�
	GD3D11.Device.State.TargetDepthTexture2D = DepthStencilTexture ;
	GD3D11.Device.State.TargetDepthDSV       = DepthStencilView ;
	if( DepthStencilTexture != NULL )
	{
		DepthStencilTexture->GetDesc( &GD3D11.Device.State.TargetDepthTexture2DDesc ) ;
	}
	else
	{
		_MEMSET( &GD3D11.Device.State.TargetDepthTexture2DDesc, 0, sizeof( D_D3D11_TEXTURE2D_DESC ) ) ;
	}

	// �����_�����O�^�[�Q�b�g�̕ύX
	Graphics_D3D11_DeviceState_SetRenderTarget_SizeCheck() ;

	// �I��
	return  0 ;
}

// �g�p������̓��C�A�E�g��ύX����
extern	int		Graphics_D3D11_DeviceState_SetInputLayout( D_ID3D11InputLayout *InputLayout )
{
	if( GAPIWin.D3D11DeviceObject == NULL )
	{
		return -1 ;
	}

	if( GD3D11.Device.State.SetVertexInputLayout == InputLayout &&
		GD3D11.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
	{
		return 0 ;
	}

	if( InputLayout != NULL )
	{
		D3D11DeviceContext_IASetInputLayout( InputLayout ) ;
	}
	GD3D11.Device.State.SetVertexInputLayout = InputLayout ;

	GD3D11.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

	// �I��
	return 0 ;
}

// �w��̒��_�V�F�[�_�[���f�o�C�X�ɃZ�b�g����
extern int Graphics_D3D11_DeviceState_SetVertexShaderToHandle( int ShaderHandle )
{
	SHADERHANDLEDATA *VertexShader ;

	// �n���h���`�F�b�N
	if( SHADERCHK( ShaderHandle, VertexShader ) )
	{
		return -1 ;
	}
	if( VertexShader->IsVertexShader == FALSE )
	{
		return -1 ;
	}

	// �V�F�[�_�[�̃Z�b�g
	Graphics_D3D11_DeviceState_SetVertexShader( VertexShader->PF->D3D11.VertexShader ) ;

	// ����I��
	return 0 ;
}

// �g�p���钸�_�V�F�[�_�[��ύX����
extern	int		Graphics_D3D11_DeviceState_SetVertexShader( D_ID3D11VertexShader *VertexShader, int NormalVertexShader )
{
	if( GD3D11.Device.State.SetVertexShader == VertexShader &&
		GD3D11.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
	{
		return 0 ;
	}

	if( GAPIWin.D3D11DeviceObject == NULL )
	{
		return -1 ;
	}

	D3D11DeviceContext_VSSetShader( VertexShader, NULL, 0 ) ;
	GD3D11.Device.State.SetVertexShader          = VertexShader ;
	GD3D11.Device.State.SetNormalVertexShader    = NormalVertexShader ;
	if( NormalVertexShader == FALSE )
	{
		GD3D11.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;
	}

	// �I��
	return 0 ;
}

// ���_�V�F�[�_�[�̎g�p���~�߂�
extern	int		Graphics_D3D11_DeviceState_ResetVertexShader( int SetNormalVertexShaderCancel )
{
	if( GD3D11.Device.State.SetNormalVertexShader &&
		SetNormalVertexShaderCancel &&
		GD3D11.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
	{
		return 0 ;
	}

	if( GD3D11.Device.DrawSetting.CancelSettingEqualCheck == FALSE &&
		GD3D11.Device.State.SetVertexShader == NULL &&
		GD3D11.Device.State.SetVertexInputLayout == NULL )
	{
		return 0 ;
	}

	if( GAPIWin.D3D11DeviceObject == NULL )
	{
		return -1 ;
	}

	if( GD3D11.Device.DrawSetting.CancelSettingEqualCheck ||
		GD3D11.Device.State.SetVertexShader != NULL )
	{
		D3D11DeviceContext_VSSetShader( NULL, NULL, 0 ) ;
		GD3D11.Device.State.SetVertexShader = NULL ;
	}

	if( GD3D11.Device.DrawSetting.CancelSettingEqualCheck ||
		GD3D11.Device.State.SetVertexInputLayout != NULL )
	{
		D3D11DeviceContext_IASetInputLayout( NULL ) ;
		GD3D11.Device.State.SetVertexInputLayout = NULL ;
	}

	GD3D11.Device.State.SetNormalVertexShader    = FALSE ;
	GD3D11.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

	// �I��
	return 0 ;
}

// ���_�V�F�[�_�[�Ŏg�p����萔�o�b�t�@�[��ݒ肷��
extern	int		Graphics_D3D11_DeviceState_SetVertexShaderConstantBuffers( UINT StartSlot, UINT NumBuffers, D_ID3D11Buffer * const *pConstantBuffers )
{
	UINT i ;

	if( GAPIWin.D3D11DeviceObject == NULL )
	{
		return -1 ;
	}

	if( StartSlot < 0 || StartSlot + NumBuffers > D_D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT )
	{
		return -1 ;
	}

	if( GD3D11.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
	{
		for( i = 0 ; i < NumBuffers ; i ++ )
		{
			if( GD3D11.Device.State.SetVertexShaderConstantBuffer[ i + StartSlot ] != pConstantBuffers[ i ] )
			{
				break ;
			}
		}
		if( i == NumBuffers )
		{
			return 0 ;
		}
	}

	D3D11DeviceContext_VSSetConstantBuffers( StartSlot, NumBuffers, pConstantBuffers ) ;

	for( i = 0 ; i < NumBuffers ; i ++ )
	{
		GD3D11.Device.State.SetVertexShaderConstantBuffer[ i + StartSlot ] = pConstantBuffers[ i ] ;
	}

	// �I��
	return 0 ;
}


// �w��̃s�N�Z���V�F�[�_�[���f�o�C�X�ɃZ�b�g����
extern int Graphics_D3D11_DeviceState_SetPixelShaderToHandle( int ShaderHandle )
{
	SHADERHANDLEDATA *PixelShader ;

	// �n���h���`�F�b�N
	if( SHADERCHK( ShaderHandle, PixelShader ) )
	{
		return -1 ;
	}
	if( PixelShader->IsVertexShader != FALSE )
	{
		return -1 ;
	}

	// �V�F�[�_�[�̃Z�b�g
	Graphics_D3D11_DeviceState_SetPixelShader( PixelShader->PF->D3D11.PixelShader ) ;

	// ����I��
	return 0 ;
}

// �g�p����s�N�Z���V�F�[�_�[��ύX����
extern	int		Graphics_D3D11_DeviceState_SetPixelShader( D_ID3D11PixelShader *PixelShader, int NormalPixelShader )
{
	if( GD3D11.Device.State.SetPixelShader == PixelShader &&
		GD3D11.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
	{
		return 0 ;
	}

	if( GAPIWin.D3D11DeviceObject == NULL )
	{
		return -1 ;
	}

	D3D11DeviceContext_PSSetShader( PixelShader, NULL, 0 ) ;
	GD3D11.Device.State.SetPixelShader       = PixelShader ;
	GD3D11.Device.State.SetNormalPixelShader = NormalPixelShader ;
	if( NormalPixelShader == FALSE )
	{
		GD3D11.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;
	}

	// �I��
	return 0 ;
}

// �s�N�Z���V�F�[�_�[�̎g�p���~�߂�
extern	int		Graphics_D3D11_DeviceState_ResetPixelShader( int SetNormalPixelShaderCancel )
{
	if( GD3D11.Device.State.SetNormalPixelShader &&
		SetNormalPixelShaderCancel &&
		GD3D11.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
	{
		return 0 ;
	}

	if( GD3D11.Device.DrawSetting.CancelSettingEqualCheck == FALSE &&
		GD3D11.Device.State.SetPixelShader == NULL )
	{
		return 0 ;
	}

	if( GAPIWin.D3D11DeviceObject == NULL )
	{
		return -1 ;
	}

	if( GD3D11.Device.DrawSetting.CancelSettingEqualCheck ||
		GD3D11.Device.State.SetPixelShader != NULL )
	{
		D3D11DeviceContext_PSSetShader( NULL, NULL, 0 ) ;
		GD3D11.Device.State.SetPixelShader = NULL ;
	}

	GD3D11.Device.State.SetNormalPixelShader     = FALSE ;
	GD3D11.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

	// �I��
	return 0 ;
}

// �s�N�Z���V�F�[�_�[�Ŏg�p����萔�o�b�t�@�[��ݒ肷��
extern	int		Graphics_D3D11_DeviceState_SetPixelShaderConstantBuffers( UINT StartSlot, UINT NumBuffers, D_ID3D11Buffer * const *pConstantBuffers )
{
	UINT i ;

	if( GAPIWin.D3D11DeviceObject == NULL )
	{
		return -1 ;
	}

	if( StartSlot < 0 || StartSlot + NumBuffers > D_D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT )
	{
		return -1 ;
	}

	if( GD3D11.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
	{
		for( i = 0 ; i < NumBuffers ; i ++ )
		{
			if( GD3D11.Device.State.SetPixelShaderConstantBuffer[ i + StartSlot ] != pConstantBuffers[ i ] )
			{
				break ;
			}
		}
		if( i == NumBuffers )
		{
			return 0 ;
		}
	}

	D3D11DeviceContext_PSSetConstantBuffers( StartSlot, NumBuffers, pConstantBuffers ) ;

	for( i = 0 ; i < NumBuffers ; i ++ )
	{
		GD3D11.Device.State.SetPixelShaderConstantBuffer[ i + StartSlot ] = pConstantBuffers[ i ] ;
	}

	// �I��
	return 0 ;
}

// �g�p���钸�_�o�b�t�@��ύX����
extern	int		Graphics_D3D11_DeviceState_SetVertexBuffer( D_ID3D11Buffer *VertexBuffer, int VertexStride )
{
	UINT Offsets ;

	if( GD3D11.Device.State.SetVertexBuffer       == VertexBuffer &&
		GD3D11.Device.State.SetVertexBufferStride == VertexStride &&
		GD3D11.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
	{
		return 0 ;
	}

	if( GAPIWin.D3D11DeviceObject == NULL )
	{
		return -1 ;
	}

	if( VertexBuffer != NULL )
	{
		Offsets = 0 ;
		D3D11DeviceContext_IASetVertexBuffers( 0, 1, &VertexBuffer, ( UINT* )&VertexStride, &Offsets ) ;
	}
	GD3D11.Device.State.SetVertexBuffer       = VertexBuffer ;
	GD3D11.Device.State.SetVertexBufferStride = VertexStride ;

	// �I��
	return 0 ;
}

// �g�p����C���f�b�N�X�o�b�t�@��ύX����
extern	int		Graphics_D3D11_DeviceState_SetIndexBuffer( D_ID3D11Buffer *IndexBuffer, D_DXGI_FORMAT Format )
{
	if( GD3D11.Device.State.SetIndexBuffer == IndexBuffer &&
		GD3D11.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
	{
		return 0 ;
	}

	if( GAPIWin.D3D11DeviceObject == NULL )
	{
		return -1 ;
	}

	if( IndexBuffer != NULL )
	{
		D3D11DeviceContext_IASetIndexBuffer( IndexBuffer, Format, 0 ) ;
	}
	GD3D11.Device.State.SetIndexBuffer       = IndexBuffer ;
	GD3D11.Device.State.SetIndexBufferFormat = Format ;

	// �I��
	return 0 ;
}

// �s�N�Z���V�F�[�_�[�Ŏg�p����V�F�[�_�[���\�[�X�r���[��ύX����
extern int Graphics_D3D11_DeviceState_SetPSShaderResouceView( int StartSlot, int Num, D_ID3D11ShaderResourceView * const *ppShaderResourceViews )
{
	int i ;
	int Change ;
	int FixStartI ;
	int FixStartSlot ;
	int FixNum ;

	if( GAPIWin.D3D11DeviceObject == NULL )
	{
		return -1 ;
	}

	FixStartSlot = -1 ;
	FixNum = 0 ;
	Change = FALSE ;
	for( i = 0; i < Num; i++ )
	{
		if( GD3D11.Device.State.PSSetShaderResourceView[ StartSlot + i ] != ppShaderResourceViews[ i ] ||
			GD3D11.Device.DrawSetting.CancelSettingEqualCheck )
		{
			if( Change == FALSE )
			{
				Change = TRUE ;
				FixStartI = i ;
				FixStartSlot = StartSlot + i ;
			}
			FixNum = i - FixStartI + 1 ;
			GD3D11.Device.State.PSSetShaderResourceView[ StartSlot + i ] = ppShaderResourceViews[ i ] ;
		}
	}
	if( Change == FALSE )
	{
		return 0 ;
	}
	D3D11DeviceContext_PSSetShaderResources( ( UINT )FixStartSlot, ( UINT )FixNum, &ppShaderResourceViews[ FixStartI ] ) ;

	// �I��
	return 0 ;
}

// �W���`��̏������s��
extern	int		Graphics_D3D11_DeviceState_NormalDrawSetup( void )
{
	int							IgnoreTextureAlpha ;
	int							NextBlendMode ;
	int							UseFloatFactorColor = FALSE ;
	DX_D3D11_SHADER_FLOAT4		FloatFactorColor ;
	int							AlphaTestRef = 0 ;
	int							AlphaTestCmpMode = 0 ;
	D_ID3D11ShaderResourceView	*ShaderResourceView[ USE_TEXTURESTAGE_NUM ] ;
	int							ShaderResourceViewNum = 0 ;
	D_ID3D11PixelShader			*UsePixelShader = NULL ;

	if( GAPIWin.D3D11DeviceObject == NULL )
	{
		return -1 ;
	}

	// �e�N�X�`�����ύX���ꂽ�A�t���O��|��
	GD3D11.Device.DrawSetting.ChangeTextureFlag = FALSE ;
	GD3D11.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

	// �p�����[�^���ύX���ꂽ�A�t���O��|��
	GD3D11.Device.DrawSetting.ChangeBlendParamFlag = FALSE ;

	// �u�����h���[�h�̌���
	{
		NextBlendMode = GD3D11.Device.DrawSetting.BlendMode ;
		switch( GD3D11.Device.DrawSetting.BlendMode )
		{
		case DX_BLENDMODE_SUB :
			// ���Z�u�����h�̏ꍇ��14�Ԗڂ̃u�����h���[�h���g�p����
			NextBlendMode = DX_BLENDMODE_SUB1 ;
			break ;

		case DX_BLENDMODE_NOBLEND :
			// �`���Ƀ��l������ꍇ�� DX_BLENDMODE_NOBLEND �ł��u�����h���[�h�� DX_BLENDMODE_SRCCOLOR �ɂ���
			if( GSYS.DrawSetting.AlphaChDrawMode )
			{
				NextBlendMode = DX_BLENDMODE_SRCCOLOR ;
			}

			// �`�挳�摜�Ƀ��`�����l��������ꍇ��u�����h�摜�̗L���ȂǏ�������� DX_BLENDMODE_ALPHA ���g�p����
			if( GD3D11.Device.DrawSetting.RenderTexture != NULL )
			{
				if( GD3D11.Device.DrawSetting.BlendTexture != NULL )
				{
					switch( GD3D11.Device.DrawSetting.BlendGraphType )
					{
					case DX_BLENDGRAPHTYPE_NORMAL :
					case DX_BLENDGRAPHTYPE_ALPHA :
						if( GD3D11.Device.DrawSetting.AlphaTestValidFlag    == TRUE ||
							GD3D11.Device.DrawSetting.AlphaChannelValidFlag == TRUE )
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
					if( GD3D11.Device.DrawSetting.AlphaChannelValidFlag == TRUE )
					{
						NextBlendMode = DX_BLENDMODE_ALPHA ;
					}
				}
			}
			break ;
		}
	}
	
	// �J�����g�e�N�X�`���������ꍇ�Ƃ���ꍇ�ŕ���
	if( GD3D11.Device.DrawSetting.RenderTexture == NULL )
	{
		// �J�����g�e�N�X�`���������ꍇ�͐��┠�̕`��Ɣ��f�A�e�N�X�`���X�e�[�W�͖����ɂ���

		// �Ƃ肠�����`�揈�����s��
		GD3D11.Device.DrawInfo.BlendMaxNotDrawFlag = FALSE ;

		// �A���t�@�e�X�g�p�����[�^���L���ȏꍇ�͂����D�悷��
		if( GD3D11.Device.DrawSetting.AlphaTestMode != -1 )
		{
			GD3D11.Device.State.AlphaTestEnable = TRUE ;
			AlphaTestCmpMode = GD3D11.Device.DrawSetting.AlphaTestMode ;
			AlphaTestRef     = GD3D11.Device.DrawSetting.AlphaTestParam ;
		}
		else
		{
			GD3D11.Device.State.AlphaTestEnable = FALSE ;
		}

		// �g�p����s�N�Z���V�F�[�_�[�̎擾
		GD3D11.Device.State.AlphaTestModeShaderIndex = Graphics_D3D11_Shader_GetAlphaTestModeIndex( GD3D11.Device.State.AlphaTestEnable, AlphaTestCmpMode ) ;
		UsePixelShader = GD3D11.Device.Shader.Base.BaseNoneTex_PS[ g_DefaultBlendDescArray[ NextBlendMode ].RenderBlendType ][ GD3D11.Device.State.AlphaTestModeShaderIndex ] ;

		// ���_�^�C�v�̃Z�b�g
		GD3D11.Device.DrawInfo.VertexType = VERTEXTYPE_NOTEX ;

#if VERTEXBUFFERMODE == 2
		NORMALVERTEXBUFFER_MAP
		GD3D11.Device.DrawInfo.VertexBufferNextAddr = GD3D11.Device.DrawInfo.VertexBufferAddr ;
#else
		GD3D11.Device.DrawInfo.VertexBufferNextAddr = GD3D11.Device.DrawInfo.VertexBufferTemp ;
#endif
	}
	else
	{
		// �J�����g�e�N�X�`��������ꍇ�̓e�N�X�`���X�e�[�W�u�����h�e�[�u������l���擾����

		// �g�p����e�N�X�`���u�����h�X�e�[�W�X�e�[�^�X�e�[�u���̃C���f�b�N�X���Z�b�g
		IgnoreTextureAlpha = 1 ;
		if( GD3D11.Device.DrawSetting.AlphaChannelValidFlag ||
			GD3D11.Device.DrawSetting.AlphaTestValidFlag    ||
			( GD3D11.Device.State.DepthStencilDesc.DepthEnable &&
			  GD3D11.Device.DrawSetting.BlendTexture == NULL ) )
		{
			IgnoreTextureAlpha = 0 ;
		}

		// �u�����h�e�N�X�`���[���g�p���邩�ǂ����ŕ���
		if( GD3D11.Device.DrawSetting.BlendTexture != NULL )
		{
			int RenderBlendType ;

			// �u�����h�e�N�X�`���[������ꍇ�� AlphaTestValidFlag �͖���

			// �g�p����e�[�u���̃C���f�b�N�X������o��
			RenderBlendType = g_DefaultBlendDescArray[ NextBlendMode ].RenderBlendType ;

			// �g�p����e�N�X�`���[�A�h���X���Z�b�g
			ShaderResourceView[ 0 ] = GD3D11.Device.DrawSetting.RenderTextureSRV ;
			ShaderResourceView[ 1 ] = GD3D11.Device.DrawSetting.BlendTextureSRV ;
			ShaderResourceViewNum = 2 ;

			// ���_�^�C�v�̃Z�b�g
			GD3D11.Device.DrawInfo.VertexType = VERTEXTYPE_BLENDTEX ;

#if VERTEXBUFFERMODE == 2
			NORMALVERTEXBUFFER_MAP
			GD3D11.Device.DrawInfo.VertexBufferNextAddr = GD3D11.Device.DrawInfo.VertexBufferAddr ;
#else
			GD3D11.Device.DrawInfo.VertexBufferNextAddr = GD3D11.Device.DrawInfo.VertexBufferTemp ;
#endif

			// �u�����h�^�C�v�ɂ���ď����𕪊�
			switch( GD3D11.Device.DrawSetting.BlendGraphType )
			{
			case DX_BLENDGRAPHTYPE_NORMAL :
				// �Ƃ肠�����`�揈�����s��
				GD3D11.Device.DrawInfo.BlendMaxNotDrawFlag = FALSE ;

				// �A���t�@�e�X�g�p�����[�^���L���ȏꍇ�͂����D�悷��
				if( GD3D11.Device.DrawSetting.AlphaTestMode != -1 )
				{
					GD3D11.Device.State.AlphaTestEnable = TRUE ;
					AlphaTestCmpMode = GD3D11.Device.DrawSetting.AlphaTestMode ;
					AlphaTestRef     = GD3D11.Device.DrawSetting.AlphaTestParam ;
				}
				else
				{
					// ���������L���ȏꍇ�̓��l���O�̏ꍇ�̂ݕ\������Ȃ��悤�ɂ���
					if( GD3D11.Device.DrawSetting.AlphaTestValidFlag == TRUE ||
						g_DefaultBlendDescArray[ NextBlendMode ].RenderTargetBlendDesc.BlendEnable == TRUE ||
						GD3D11.Device.DrawSetting.AlphaChannelValidFlag == TRUE )
					{
						GD3D11.Device.State.AlphaTestEnable = TRUE ;
						AlphaTestRef     = 0 ;
						AlphaTestCmpMode = DX_CMP_GREATER ;
					}
					else
					{
						GD3D11.Device.State.AlphaTestEnable = FALSE ;
					}
				}

				// �N���X�t�F�[�h����ݒ肷��
				UseFloatFactorColor = TRUE ;
				FloatFactorColor[ 0 ] = 1.0f ;
				FloatFactorColor[ 1 ] = 1.0f ;
				FloatFactorColor[ 2 ] = 1.0f ;
				FloatFactorColor[ 3 ] = GD3D11.Device.DrawSetting.BlendGraphFadeRatio / 255.0f ;

				// �g�p����V�F�[�_�[���Z�b�g
				GD3D11.Device.State.AlphaTestModeShaderIndex = Graphics_D3D11_Shader_GetAlphaTestModeIndex( GD3D11.Device.State.AlphaTestEnable, AlphaTestCmpMode ) ;
				UsePixelShader = GD3D11.Device.Shader.Base.BaseUseTex_PS[ 1 ][ RenderBlendType ][ GD3D11.Device.DrawSetting.IgnoreGraphColorFlag ? 1 : 0 ][ IgnoreTextureAlpha ][ GD3D11.Device.State.AlphaTestModeShaderIndex ] ;
				break ;

			case DX_BLENDGRAPHTYPE_WIPE :
				// �u�����h���E�l���ő�̏ꍇ�͉����`�悵�Ȃ�
				GD3D11.Device.DrawInfo.BlendMaxNotDrawFlag = GD3D11.Device.DrawSetting.BlendGraphBorderParam == 255 ? TRUE : FALSE ;

				// float�^�� FactorColor ���g�p����
				UseFloatFactorColor = TRUE ;
				FloatFactorColor[ 0 ] = 0.0f ;
				FloatFactorColor[ 1 ] = 0.0f ;
				FloatFactorColor[ 2 ] = 0.0f ;
				FloatFactorColor[ 3 ] = 0.0f ;

				// ���E�͈͂��P�ȉ����ǂ����ŏ����𕪊�
				if( GD3D11.Device.DrawSetting.BlendGraphBorderRange <= 1 )
				{
					// �����͂����w�Ǘ�O���� -------------------------------------------

					// ���e�X�g���g�p����
					GD3D11.Device.State.AlphaTestEnable = TRUE ;
					AlphaTestRef     = GD3D11.Device.DrawSetting.BlendGraphBorderParam ;
					AlphaTestCmpMode = DX_CMP_GREATEREQUAL ;

					// ���u�����h�@�\�n�e�e
					NextBlendMode = DX_BLENDMODE_NOBLEND ;

					// �������郿�l�͖���
					FloatFactorColor[ 3 ] = 0.0f ;
					FloatFactorColor[ 2 ] = 1.0f ;

					// �����摜�����`�����l�����g�p���Ă��āA���� BlendGraphBorderParam �� 0 �������� AlphaRef ���O�ɂ��Ȃ�
					if( ( GD3D11.Device.DrawSetting.AlphaChannelValidFlag ||
						  GD3D11.Device.DrawSetting.AlphaTestValidFlag ) &&
						GD3D11.Device.DrawSetting.BlendGraphBorderParam == 0 )
					{
						AlphaTestRef = 1 ;
					}
				}
				else
				{
					int p ;
				
					// ���u�����h�ɂ��e�N�X�`���u�����h

					// ���e�X�g�͍s��Ȃ�
					GD3D11.Device.State.AlphaTestEnable = FALSE ;

					// ���E�͈͂ɂ���ĂS�i�K
					if( GD3D11.Device.DrawSetting.BlendGraphBorderRange <= 64 )
					{
						// �~�S

						p = 1280 - GD3D11.Device.DrawSetting.BlendGraphBorderParam * 1280 / 255 ; 

						// �u�����h�e�N�X�`���̃u�����h�l�ɂ���č����p�����[�^��ύX
						if( p < 1024 )
						{
							FloatFactorColor[ 3 ] = - ( float )( 255 - p * 255 / 1024 ) / 255.0f ;
						}
						else
						{
							FloatFactorColor[ 3 ] =   ( float )( 64 * ( p - 1024 ) / 256 ) / 255.0f ;
						}

						FloatFactorColor[ 2 ] = 4.0f ;
					}
					else
					if( GD3D11.Device.DrawSetting.BlendGraphBorderRange <= 128 )
					{
						// �~�Q

						p = 768 - GD3D11.Device.DrawSetting.BlendGraphBorderParam * 768 / 255 ; 

						// �u�����h�e�N�X�`���̃u�����h�l�ɂ���č����p�����[�^��ύX
						if( p < 512 )
						{
							FloatFactorColor[ 3 ] = - ( float )( 255 - p * 255 / 512 ) / 255.0f ;
						}
						else
						{
							FloatFactorColor[ 3 ] =   ( float )( 128 * ( p - 512 ) / 256 ) / 255.0f ;
						}

						FloatFactorColor[ 2 ] = 2.0f ;
					}
					else
					{
						// �~�P

						p = 512 - GD3D11.Device.DrawSetting.BlendGraphBorderParam * 512 / 255 ; 

						// �u�����h�e�N�X�`���̃u�����h�l�ɂ���č����p�����[�^��ύX
						if( p < 256 )
						{
							FloatFactorColor[ 3 ] = - ( float )( 255 - p * 255 / 256 ) / 255.0f ;
						}
						else
						{
							FloatFactorColor[ 3 ] =   ( float )( 255 * ( p - 256 ) / 256 ) / 255.0f ;
						}

						FloatFactorColor[ 2 ] = 1.0f ;
					}
				}

				// �g�p����V�F�[�_�[���Z�b�g
				GD3D11.Device.State.AlphaTestModeShaderIndex = Graphics_D3D11_Shader_GetAlphaTestModeIndex( GD3D11.Device.State.AlphaTestEnable, AlphaTestCmpMode ) ;
				UsePixelShader = GD3D11.Device.Shader.Base.BaseUseTex_PS[ 2 ][ RenderBlendType ][ GD3D11.Device.DrawSetting.IgnoreGraphColorFlag ? 1 : 0 ][ IgnoreTextureAlpha ][ GD3D11.Device.State.AlphaTestModeShaderIndex ] ;
				break ;

			case DX_BLENDGRAPHTYPE_ALPHA :
				// �Ƃ肠�����`�揈�����s��
				GD3D11.Device.DrawInfo.BlendMaxNotDrawFlag = FALSE ;

				// �A���t�@�e�X�g�p�����[�^���L���ȏꍇ�͂����D�悷��
				if( GD3D11.Device.DrawSetting.AlphaTestMode != -1 )
				{
					GD3D11.Device.State.AlphaTestEnable = TRUE ;
					AlphaTestCmpMode = GD3D11.Device.DrawSetting.AlphaTestMode ;
					AlphaTestRef     = GD3D11.Device.DrawSetting.AlphaTestParam ;
				}
				else
				{
					// ���������L���ȏꍇ�̓��l���O�̏ꍇ�̂ݕ\������Ȃ��悤�ɂ���
					if( g_DefaultBlendDescArray[ NextBlendMode ].RenderTargetBlendDesc.BlendEnable == TRUE )
					{
						GD3D11.Device.State.AlphaTestEnable = TRUE ;
						AlphaTestRef     = 0 ;
						AlphaTestCmpMode = DX_CMP_GREATER ;
					}
					else
					{
						GD3D11.Device.State.AlphaTestEnable = FALSE ;
					}
				}

				// �g�p����V�F�[�_�[���Z�b�g
				GD3D11.Device.State.AlphaTestModeShaderIndex = Graphics_D3D11_Shader_GetAlphaTestModeIndex( GD3D11.Device.State.AlphaTestEnable, AlphaTestCmpMode ) ;
				UsePixelShader = GD3D11.Device.Shader.Base.BaseUseTex_PS[ 3 ][ RenderBlendType ][ GD3D11.Device.DrawSetting.IgnoreGraphColorFlag ? 1 : 0 ][ IgnoreTextureAlpha ][ GD3D11.Device.State.AlphaTestModeShaderIndex ] ;
				break ;
			}
		}
		else
		{
			// �u�����h�e�N�X�`���[�������ꍇ

			// �Ƃ肠�����`�揈�����s��
			GD3D11.Device.DrawInfo.BlendMaxNotDrawFlag = FALSE ;

			// ���_�^�C�v�̃Z�b�g
			GD3D11.Device.DrawInfo.VertexType = VERTEXTYPE_TEX ;

#if VERTEXBUFFERMODE == 2
			NORMALVERTEXBUFFER_MAP
			GD3D11.Device.DrawInfo.VertexBufferNextAddr = GD3D11.Device.DrawInfo.VertexBufferAddr ;
#else
			GD3D11.Device.DrawInfo.VertexBufferNextAddr = GD3D11.Device.DrawInfo.VertexBufferTemp ;
#endif

			// �A���t�@�e�X�g�p�����[�^���L���ȏꍇ�͂����D�悷��
			if( GD3D11.Device.DrawSetting.AlphaTestMode != -1 )
			{
				GD3D11.Device.State.AlphaTestEnable = TRUE ;
				AlphaTestCmpMode = GD3D11.Device.DrawSetting.AlphaTestMode ;
				AlphaTestRef     = GD3D11.Device.DrawSetting.AlphaTestParam ;
			}
			else
			{
				// ���e�X�g���g�p���邩�ǂ����ŏ����𕪊�
				if( GD3D11.Device.DrawSetting.AlphaTestValidFlag == TRUE )
				{
					// ���e�X�g���s��(���ߐF�����ȊO�Ƀ��e�X�g�̓u�����h�e�N�X�`���ł��g�p���Ă���)
					GD3D11.Device.State.AlphaTestEnable = TRUE ;
					AlphaTestRef     = 16 ;
					AlphaTestCmpMode = DX_CMP_GREATER ;
				}
				else
				{
					// ���������L���ȏꍇ�̓u�����h���[�h�ɂ���Ă̓��l���O�̏ꍇ�̂ݕ\������Ȃ��悤�ɂ���
					if( g_DefaultBlendDescArray[ NextBlendMode ].RenderTargetBlendDesc.BlendEnable == TRUE &&
						g_DefaultBlendDescArray[ NextBlendMode ].AlphaZeroNotDrawFlag == TRUE )
					{
						GD3D11.Device.State.AlphaTestEnable = TRUE ;
						AlphaTestRef     = 0 ;
						AlphaTestCmpMode = DX_CMP_GREATER ;
					}
					else
					{
						GD3D11.Device.State.AlphaTestEnable = FALSE ;
					}
				}
			}

			// �g�p����s�N�Z���V�F�[�_�[���Z�b�g
			GD3D11.Device.State.AlphaTestModeShaderIndex = Graphics_D3D11_Shader_GetAlphaTestModeIndex( GD3D11.Device.State.AlphaTestEnable, AlphaTestCmpMode ) ;
			UsePixelShader = GD3D11.Device.Shader.Base.BaseUseTex_PS[ 0 ][ g_DefaultBlendDescArray[ NextBlendMode ].RenderBlendType ][ GD3D11.Device.DrawSetting.IgnoreGraphColorFlag ? 1 : 0 ][ IgnoreTextureAlpha ][ GD3D11.Device.State.AlphaTestModeShaderIndex ] ;

			// �g�p����e�N�X�`���[�A�h���X���Z�b�g
			ShaderResourceView[ 0 ] = GD3D11.Device.DrawSetting.RenderTextureSRV ;

			// �g�p����e�N�X�`���̐��͈��
			ShaderResourceViewNum = 1 ;
		}
	}

	if( ShaderResourceViewNum > 0 )
	{
		Graphics_D3D11_DeviceState_SetPSShaderResouceView( 0, ShaderResourceViewNum, ShaderResourceView ) ;
	}

	if( GD3D11.Device.State.AlphaTestEnable )
	{
		if( GD3D11.Device.State.AlphaTestRef != AlphaTestRef )
		{
			Graphics_D3D11_DeviceState_SetAlphaTestRef( AlphaTestRef ) ;
		}
		if( GD3D11.Device.State.AlphaTestCmpMode != AlphaTestCmpMode )
		{
			Graphics_D3D11_DeviceState_SetAlphaTestCmpMode( AlphaTestCmpMode ) ;
		}
	}
	else
	{
		if( GD3D11.Device.State.AlphaTestRef != -1 )
		{
			Graphics_D3D11_DeviceState_SetAlphaTestRef( -1 ) ;
		}
		if( GD3D11.Device.State.AlphaTestCmpMode != DX_CMP_GREATER )
		{
			Graphics_D3D11_DeviceState_SetAlphaTestCmpMode( DX_CMP_GREATER ) ;
		}
	}

	if( UseFloatFactorColor )
	{
		Graphics_D3D11_DeviceState_SetFactorColor( &FloatFactorColor ) ;
	}

	if( GD3D11.Device.State.BlendMode != NextBlendMode )
	{
		Graphics_D3D11_DeviceState_SetBlendMode( NextBlendMode ) ;
	}

	if( GD3D11.Device.State.SetPixelShader != UsePixelShader )
	{
		Graphics_D3D11_DeviceState_SetPixelShader( UsePixelShader, TRUE ) ;
	}
	
	// �I��
	return 0 ;
}

// ���݂̐ݒ�Ɋ�Â��� ID3D11SamplerState�AID3D11RasterizerState�AID3D11DepthStencilState�AID3D11BlendState �̃Z�b�g�A�b�v�ƁA�萔�o�b�t�@�̃A�b�v�f�[�g���s��
__inline void Graphics_D3D11_DeviceState_SetupStateAndConstantBuffer_Inline( void )
{
	// ���݂̐ݒ�Ɋ�Â��� ID3D11SamplerState ���Z�b�g�A�b�v����
	if( GD3D11.Device.State.ChangeSamplerDesc )
	{
		Graphics_D3D11_DeviceState_SetupSamplerState() ;
	}

	// ���݂̐ݒ�Ɋ�Â��� ID3D11RasterizerState ���Z�b�g�A�b�v����
	if( GD3D11.Device.State.ChangeRasterizerDesc )
	{
		Graphics_D3D11_DeviceState_SetupRasterizerState() ;
	}

	// ���݂̐ݒ�Ɋ�Â��� ID3D11DepthStencilState ���Z�b�g�A�b�v����
	if( GD3D11.Device.State.ChangeDepthStencilDesc )
	{
		Graphics_D3D11_DeviceState_SetupDepthStencilState() ;
	}

	// ���݂̐ݒ�Ɋ�Â��� ID3D11BlendState ���Z�b�g�A�b�v����
	if( GD3D11.Device.State.ChangeBlendDesc )
	{
		Graphics_D3D11_DeviceState_SetupBlendState() ;
	}

	// �萔�o�b�t�@���A�b�v�f�[�g
	if( GD3D11.Device.Shader.Constant.ConstBuffer_Common->ChangeFlag )
	{
		Graphics_D3D11_ConstantBuffer_Update( GD3D11.Device.Shader.Constant.ConstBuffer_Common ) ;
	}
	if( GD3D11.Device.Shader.Constant.ConstBuffer_VS_Base->ChangeFlag )
	{
		Graphics_D3D11_ConstantBuffer_Update( GD3D11.Device.Shader.Constant.ConstBuffer_VS_Base ) ;
	}
	if( GD3D11.Device.Shader.Constant.ConstBuffer_VS_OtherMatrix->ChangeFlag )
	{
		Graphics_D3D11_ConstantBuffer_Update( GD3D11.Device.Shader.Constant.ConstBuffer_VS_OtherMatrix ) ;
	}
	if( GD3D11.Device.Shader.Constant.ConstBuffer_VS_LocalWorldMatrix->ChangeFlag )
	{
		Graphics_D3D11_ConstantBuffer_Update( GD3D11.Device.Shader.Constant.ConstBuffer_VS_LocalWorldMatrix ) ;
	}
	if( GD3D11.Device.Shader.Constant.ConstBuffer_PS_Base->ChangeFlag )
	{
		Graphics_D3D11_ConstantBuffer_Update( GD3D11.Device.Shader.Constant.ConstBuffer_PS_Base ) ;
	}
	if( GD3D11.Device.Shader.Constant.ConstBuffer_PS_ShadowMap->ChangeFlag )
	{
		Graphics_D3D11_ConstantBuffer_Update( GD3D11.Device.Shader.Constant.ConstBuffer_PS_ShadowMap ) ;
	}
	if( GD3D11.Device.Shader.Constant.ConstBuffer_PS_Filter->ChangeFlag )
	{
		Graphics_D3D11_ConstantBuffer_Update( GD3D11.Device.Shader.Constant.ConstBuffer_PS_Filter ) ;
	}
}

extern int Graphics_D3D11_DeviceState_SetupStateAndConstantBuffer( void )
{
	Graphics_D3D11_DeviceState_SetupStateAndConstantBuffer_Inline() ;
	return 0 ;
}






















// �`��ݒ�֌W�֐�

// �`��u�����h���[�h�̐ݒ�
extern int Graphics_D3D11_DrawSetting_SetDrawBlendMode( int BlendMode, int AlphaTestValidFlag, int AlphaChannelValidFlag )
{
	if( GAPIWin.D3D11DeviceObject == NULL )
	{
		return -1 ;
	}

	if( GD3D11.Device.DrawSetting.CancelSettingEqualCheck == FALSE &&
		GD3D11.Device.DrawSetting.BlendMode               == BlendMode &&
		GD3D11.Device.DrawSetting.AlphaTestValidFlag      == AlphaTestValidFlag &&
		GD3D11.Device.DrawSetting.AlphaChannelValidFlag   == AlphaChannelValidFlag )
	{
		return 0 ;
	}

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	GD3D11.Device.DrawSetting.BlendMode             = BlendMode ;
	GD3D11.Device.DrawSetting.AlphaChannelValidFlag = AlphaChannelValidFlag ;
	GD3D11.Device.DrawSetting.AlphaTestValidFlag    = AlphaTestValidFlag ;
	GD3D11.Device.DrawSetting.DrawPrepAlwaysFlag    = TRUE ;

	// �p�����[�^���ύX���ꂽ�A�t���O�𗧂Ă�
	GD3D11.Device.DrawSetting.ChangeBlendParamFlag = TRUE ;

	return 0 ;
}

// �`�掞�̉摜�̐F�𖳎����邩�ǂ�����ݒ肷��
extern int Graphics_D3D11_DrawSetting_SetIgnoreDrawGraphColor( int EnableFlag )
{
	if( GAPIWin.D3D11DeviceObject == NULL )
	{
		return -1 ;
	}

	if( GD3D11.Device.DrawSetting.IgnoreGraphColorFlag == EnableFlag &&
		GD3D11.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
	{
		return 0 ;
	}

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	GD3D11.Device.DrawSetting.IgnoreGraphColorFlag = EnableFlag ;
	GD3D11.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

	// �p�����[�^���ύX���ꂽ�A�t���O�𗧂Ă�
	GD3D11.Device.DrawSetting.ChangeBlendParamFlag = TRUE ;

	return 0 ;
}

// �A���t�@�e�X�g�̐ݒ�
extern int Graphics_D3D11_DrawSetting_SetDrawAlphaTest( int TestMode, int TestParam )
{
	if( GAPIWin.D3D11DeviceObject == NULL )
	{
		return -1 ;
	}

	if( GD3D11.Device.DrawSetting.CancelSettingEqualCheck == FALSE &&
		GD3D11.Device.DrawSetting.AlphaTestMode  == TestMode &&
		GD3D11.Device.DrawSetting.AlphaTestParam == TestParam )
	{
		return 0 ;
	}

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	GD3D11.Device.DrawSetting.AlphaTestMode      = TestMode ;
	GD3D11.Device.DrawSetting.AlphaTestParam     = TestParam ;
	GD3D11.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

	// �p�����[�^���ύX���ꂽ�A�t���O�𗧂Ă�
	GD3D11.Device.DrawSetting.ChangeBlendParamFlag = TRUE ;

	return 0 ;
}

// �u�����h����e�N�X�`���̃p�����[�^���Z�b�g����
extern int Graphics_D3D11_DrawSetting_SetBlendTextureParam( int BlendType, int *Param )
{
	if( GAPIWin.D3D11DeviceObject == NULL )
	{
		return -1 ;
	}

	// �p�����[�^��ۑ�
	switch( BlendType )
	{
	case DX_BLENDGRAPHTYPE_NORMAL :
		if( GD3D11.Device.DrawSetting.CancelSettingEqualCheck == FALSE &&
			BlendType == GD3D11.Device.DrawSetting.BlendGraphType &&
			GD3D11.Device.DrawSetting.BlendGraphFadeRatio == Param[ 0 ] )
		{
			return 0 ;
		}

		if( GD3D11.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
		{
			// �`��ҋ@���Ă���`�敨��`��
			DRAWSTOCKINFO
		}

		GD3D11.Device.DrawSetting.BlendGraphFadeRatio = Param[ 0 ] ;
		break;

	case DX_BLENDGRAPHTYPE_WIPE:
		if( GD3D11.Device.DrawSetting.CancelSettingEqualCheck == FALSE &&
			BlendType == GD3D11.Device.DrawSetting.BlendGraphType &&
			GD3D11.Device.DrawSetting.BlendGraphBorderParam == Param[ 0 ] &&
			GD3D11.Device.DrawSetting.BlendGraphBorderRange == Param[ 1 ] )
		{
			return 0 ;
		}

		if( GD3D11.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
		{
			// �`��ҋ@���Ă���`�敨��`��
			DRAWSTOCKINFO
		}

		GD3D11.Device.DrawSetting.BlendGraphBorderParam = Param[ 0 ] ;
		GD3D11.Device.DrawSetting.BlendGraphBorderRange = Param[ 1 ] ;
		break;

	case DX_BLENDGRAPHTYPE_ALPHA :
		if( GD3D11.Device.DrawSetting.CancelSettingEqualCheck == FALSE &&
			BlendType == GD3D11.Device.DrawSetting.BlendGraphType )
		{
			return 0 ;
		}

		if( GD3D11.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
		{
			// �`��ҋ@���Ă���`�敨��`��
			DRAWSTOCKINFO
		}
		break;
	}

	// �u�����h�^�C�v��ۑ�����
	GD3D11.Device.DrawSetting.BlendGraphType = BlendType ;

	// �p�����[�^���ύX���ꂽ�A�t���O�𗧂Ă�
	GD3D11.Device.DrawSetting.ChangeBlendParamFlag = TRUE ;
	GD3D11.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

	// �I��
	return 0 ;
}

// �e�N�X�`�����Z�b�g���� 
extern int Graphics_D3D11_DrawSetting_SetTexture( D_ID3D11Texture2D *RenderTexture, D_ID3D11ShaderResourceView *RenderTextureSRV )
{
	if( GAPIWin.D3D11DeviceObject == NULL )
	{
		return -1 ;
	}

	if( RenderTexture == GD3D11.Device.DrawSetting.RenderTexture &&
		GD3D11.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
	{
		return 0 ;
	}

	if( GD3D11.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
	{
		// �`��ҋ@���Ă���`�敨��`��
		DRAWSTOCKINFO
	}

	// �e�N�X�`�����ύX���ꂽ�A�t���O�𗧂Ă�
	GD3D11.Device.DrawSetting.ChangeTextureFlag = TRUE ;

	// �e�N�X�`���̗L�薳�����ύX���ꂽ�ꍇ�̓u�����h�p�����[�^�̕ύX���s���t���O���Z�b�g����
	if( ( GD3D11.Device.DrawSetting.RenderTexture == NULL && RenderTexture != NULL ) ||
		( GD3D11.Device.DrawSetting.RenderTexture != NULL && RenderTexture == NULL ) )
	{
		GD3D11.Device.DrawSetting.ChangeBlendParamFlag = TRUE ;
	}

	// �e�N�X�`���Z�b�g
	GD3D11.Device.DrawSetting.RenderTexture      = RenderTexture ;
	GD3D11.Device.DrawSetting.RenderTextureSRV   = RenderTextureSRV ;
	GD3D11.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

	// �I��
	return 0 ;
}

// �u�����h����e�N�X�`�����Z�b�g���� 
extern int Graphics_D3D11_DrawSetting_SetBlendTexture( D_ID3D11Texture2D *BlendTexture,  D_ID3D11ShaderResourceView *BlendTextureSRV, int TexWidth, int TexHeight )
{
	if( GAPIWin.D3D11DeviceObject == NULL )
	{
		return -1 ;
	}

	if( GD3D11.Device.DrawSetting.CancelSettingEqualCheck == FALSE &&
		BlendTexture == GD3D11.Device.DrawSetting.BlendTexture )
	{
		return 0 ;
	}

	if( GD3D11.Device.DrawSetting.CancelSettingEqualCheck == FALSE )
	{
		// �`��ҋ@���Ă���`�敨��`��
		DRAWSTOCKINFO
	}

	// �e�N�X�`�����ύX���ꂽ�A�t���O�𗧂Ă�
	GD3D11.Device.DrawSetting.ChangeTextureFlag = TRUE ;
	GD3D11.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

	// �u�����h�e�N�X�`���̗L�薳�����ύX���ꂽ�ꍇ�̓u�����h�p�����[�^�̕ύX���s���t���O���Z�b�g����
	if( ( GD3D11.Device.DrawSetting.BlendTexture == NULL && BlendTexture != NULL ) ||
		( GD3D11.Device.DrawSetting.BlendTexture != NULL && BlendTexture == NULL ) )
	{
		GD3D11.Device.DrawSetting.ChangeBlendParamFlag = TRUE ;
	}

	// �e�N�X�`���Z�b�g
	GD3D11.Device.DrawSetting.BlendTexture    = BlendTexture ;
	GD3D11.Device.DrawSetting.BlendTextureSRV = BlendTextureSRV ;

	// �e�N�X�`���̕��ƍ�����ۑ�
	if( BlendTexture != NULL )
	{
		GD3D11.Device.DrawSetting.BlendTextureWidth  = ( float )TexWidth  ;
		GD3D11.Device.DrawSetting.BlendTextureHeight = ( float )TexHeight ;
		GD3D11.Device.DrawSetting.InvBlendTextureWidth  = 1.0F / GD3D11.Device.DrawSetting.BlendTextureWidth ;
		GD3D11.Device.DrawSetting.InvBlendTextureHeight = 1.0F / GD3D11.Device.DrawSetting.BlendTextureHeight ;
	}

	// �I��
	return 0 ;
}






























// �萔���֌W�֐�

// Direct3D11 �̒萔���̏��������s��
extern int Graphics_D3D11_Constant_Initialize( void )
{
	DXST_ERRORLOG_ADDUTF16LE( "\x04\x54\x2e\x7a\xb7\x30\xa7\x30\xfc\x30\xc0\x30\xfc\x30\x28\x75\x9a\x5b\x70\x65\xd0\x30\xc3\x30\xd5\x30\xa1\x30\x6e\x30\x5c\x4f\x10\x62\x2e\x00\x2e\x00\x2e\x00\x2e\x00\x20\x00\x00"/*@ L"�e��V�F�[�_�[�p�萔�o�b�t�@�̍쐬.... " @*/ ) ;

	// �e��萔�o�b�t�@���쐬
	GD3D11.Device.Shader.Constant.ConstBuffer_Common = Graphics_D3D11_ConstantBuffer_Create( sizeof( DX_D3D11_CONST_BUFFER_COMMON ) ) ;
	if( GD3D11.Device.Shader.Constant.ConstBuffer_Common == NULL )
	{
		goto ERR ;
	}

	GD3D11.Device.Shader.Constant.ConstBuffer_VS_Base = Graphics_D3D11_ConstantBuffer_Create( sizeof( DX_D3D11_VS_CONST_BUFFER_BASE ) ) ;
	if( GD3D11.Device.Shader.Constant.ConstBuffer_VS_Base == NULL )
	{
		goto ERR ;
	}

	GD3D11.Device.Shader.Constant.ConstBuffer_VS_OtherMatrix = Graphics_D3D11_ConstantBuffer_Create( sizeof( DX_D3D11_VS_CONST_BUFFER_OTHERMATRIX ) ) ;
	if( GD3D11.Device.Shader.Constant.ConstBuffer_VS_OtherMatrix == NULL )
	{
		goto ERR ;
	}

	GD3D11.Device.Shader.Constant.ConstBuffer_VS_LocalWorldMatrix = Graphics_D3D11_ConstantBuffer_Create( sizeof( DX_D3D11_VS_CONST_BUFFER_LOCALWORLDMATRIX ) ) ;
	if( GD3D11.Device.Shader.Constant.ConstBuffer_VS_LocalWorldMatrix == NULL )
	{
		goto ERR ;
	}

	GD3D11.Device.Shader.Constant.ConstBuffer_PS_Base = Graphics_D3D11_ConstantBuffer_Create( sizeof( DX_D3D11_PS_CONST_BUFFER_BASE ) ) ;
	if( GD3D11.Device.Shader.Constant.ConstBuffer_PS_Base == NULL )
	{
		goto ERR ;
	}

	GD3D11.Device.Shader.Constant.ConstBuffer_PS_ShadowMap = Graphics_D3D11_ConstantBuffer_Create( sizeof( DX_D3D11_PS_CONST_BUFFER_SHADOWMAP ) ) ;
	if( GD3D11.Device.Shader.Constant.ConstBuffer_PS_ShadowMap == NULL )
	{
		goto ERR ;
	}

	GD3D11.Device.Shader.Constant.ConstBuffer_PS_Filter = Graphics_D3D11_ConstantBuffer_Create( DX_D3D11_PS_CONST_FILTER_SIZE ) ;
	if( GD3D11.Device.Shader.Constant.ConstBuffer_PS_Filter == NULL )
	{
		goto ERR ;
	}

	DXST_ERRORLOG_ADDUTF16LE( "\x10\x62\x9f\x52\x0a\x00\x00"/*@ L"����\n" @*/ ) ;

	// ����I��
	return 0 ;

ERR :
	DXST_ERRORLOG_ADDUTF16LE( "\x31\x59\x57\x65\x0a\x00\x00"/*@ L"���s\n" @*/ ) ;

	if( GD3D11.Device.Shader.Constant.ConstBuffer_Common != NULL )
	{
		Graphics_D3D11_ConstantBuffer_Delete( GD3D11.Device.Shader.Constant.ConstBuffer_Common ) ;
		GD3D11.Device.Shader.Constant.ConstBuffer_Common = NULL ;
	}

	if( GD3D11.Device.Shader.Constant.ConstBuffer_VS_Base != NULL )
	{
		Graphics_D3D11_ConstantBuffer_Delete( GD3D11.Device.Shader.Constant.ConstBuffer_VS_Base ) ;
		GD3D11.Device.Shader.Constant.ConstBuffer_VS_Base = NULL ;
	}

	if( GD3D11.Device.Shader.Constant.ConstBuffer_VS_OtherMatrix != NULL )
	{
		Graphics_D3D11_ConstantBuffer_Delete( GD3D11.Device.Shader.Constant.ConstBuffer_VS_OtherMatrix ) ;
		GD3D11.Device.Shader.Constant.ConstBuffer_VS_OtherMatrix = NULL ;
	}

	if( GD3D11.Device.Shader.Constant.ConstBuffer_VS_LocalWorldMatrix != NULL )
	{
		Graphics_D3D11_ConstantBuffer_Delete( GD3D11.Device.Shader.Constant.ConstBuffer_VS_LocalWorldMatrix ) ;
		GD3D11.Device.Shader.Constant.ConstBuffer_VS_LocalWorldMatrix = NULL ;
	}

	if( GD3D11.Device.Shader.Constant.ConstBuffer_PS_Base != NULL )
	{
		Graphics_D3D11_ConstantBuffer_Delete( GD3D11.Device.Shader.Constant.ConstBuffer_PS_Base ) ;
		GD3D11.Device.Shader.Constant.ConstBuffer_PS_Base = NULL ;
	}

	if( GD3D11.Device.Shader.Constant.ConstBuffer_PS_ShadowMap != NULL )
	{
		Graphics_D3D11_ConstantBuffer_Delete( GD3D11.Device.Shader.Constant.ConstBuffer_PS_ShadowMap ) ;
		GD3D11.Device.Shader.Constant.ConstBuffer_PS_ShadowMap = NULL ;
	}

	if( GD3D11.Device.Shader.Constant.ConstBuffer_PS_Filter != NULL )
	{
		Graphics_D3D11_ConstantBuffer_Delete( GD3D11.Device.Shader.Constant.ConstBuffer_PS_Filter ) ;
		GD3D11.Device.Shader.Constant.ConstBuffer_PS_Filter = NULL ;
	}

	// �G���[�I��
	return -1 ;
}

// Direct3D11 �̒萔���̌�n�����s��
extern int Graphics_D3D11_Constant_Terminate( void )
{
	if( GD3D11.Device.Shader.Constant.ConstBuffer_Common != NULL )
	{
		Graphics_D3D11_ConstantBuffer_Delete( GD3D11.Device.Shader.Constant.ConstBuffer_Common ) ;
		GD3D11.Device.Shader.Constant.ConstBuffer_Common = NULL ;
	}

	if( GD3D11.Device.Shader.Constant.ConstBuffer_VS_Base != NULL )
	{
		Graphics_D3D11_ConstantBuffer_Delete( GD3D11.Device.Shader.Constant.ConstBuffer_VS_Base ) ;
		GD3D11.Device.Shader.Constant.ConstBuffer_VS_Base = NULL ;
	}

	if( GD3D11.Device.Shader.Constant.ConstBuffer_VS_OtherMatrix != NULL )
	{
		Graphics_D3D11_ConstantBuffer_Delete( GD3D11.Device.Shader.Constant.ConstBuffer_VS_OtherMatrix ) ;
		GD3D11.Device.Shader.Constant.ConstBuffer_VS_OtherMatrix = NULL ;
	}

	if( GD3D11.Device.Shader.Constant.ConstBuffer_VS_LocalWorldMatrix != NULL )
	{
		Graphics_D3D11_ConstantBuffer_Delete( GD3D11.Device.Shader.Constant.ConstBuffer_VS_LocalWorldMatrix ) ;
		GD3D11.Device.Shader.Constant.ConstBuffer_VS_LocalWorldMatrix = NULL ;
	}

	if( GD3D11.Device.Shader.Constant.ConstBuffer_PS_Base != NULL )
	{
		Graphics_D3D11_ConstantBuffer_Delete( GD3D11.Device.Shader.Constant.ConstBuffer_PS_Base ) ;
		GD3D11.Device.Shader.Constant.ConstBuffer_PS_Base = NULL ;
	}

	if( GD3D11.Device.Shader.Constant.ConstBuffer_PS_ShadowMap != NULL )
	{
		Graphics_D3D11_ConstantBuffer_Delete( GD3D11.Device.Shader.Constant.ConstBuffer_PS_ShadowMap ) ;
		GD3D11.Device.Shader.Constant.ConstBuffer_PS_ShadowMap = NULL ;
	}

	if( GD3D11.Device.Shader.Constant.ConstBuffer_PS_Filter != NULL )
	{
		Graphics_D3D11_ConstantBuffer_Delete( GD3D11.Device.Shader.Constant.ConstBuffer_PS_Filter ) ;
		GD3D11.Device.Shader.Constant.ConstBuffer_PS_Filter = NULL ;
	}

	// ����I��
	return 0 ;
}

































// �萔�o�b�t�@�֌W�֐�


// �萔�o�b�t�@���쐬����
extern	CONSTANTBUFFER_DIRECT3D11 *Graphics_D3D11_ConstantBuffer_Create( DWORD Size, int ASyncThread )
{
	CONSTANTBUFFER_DIRECT3D11	*NewBuffer = NULL ;
	DWORD						AllocSize ;
	D_D3D11_BUFFER_DESC			BufferDesc ;

	// �N���e�B�J���Z�N�V�����̎擾
	CRITICALSECTION_LOCK( &HandleManageArray[ DX_HANDLETYPE_SHADER_CONSTANT_BUFFER ].CriticalSection ) ;

	// �T�C�Y�`�F�b�N
	if( Size % 16 != 0 )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\x9a\x5b\x70\x65\xd0\x30\xc3\x30\xd5\x30\xa1\x30\x6e\x30\xb5\x30\xa4\x30\xba\x30\x6f\x30\x11\xff\x16\xff\x6e\x30\x0d\x50\x70\x65\x67\x30\x42\x30\x8b\x30\xc5\x5f\x81\x89\x4c\x30\x42\x30\x8a\x30\x7e\x30\x59\x30\x0a\x00\x00"/*@ L"�萔�o�b�t�@�̃T�C�Y�͂P�U�̔{���ł���K�v������܂�\n" @*/ )) ;
		goto ERR ;
	}

	// �������̊m��
	AllocSize = Size + sizeof( CONSTANTBUFFER_DIRECT3D11 ) ;
	NewBuffer = ( CONSTANTBUFFER_DIRECT3D11 * )DXALLOC( AllocSize ) ;
	if( NewBuffer == NULL )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\x9a\x5b\x70\x65\xd0\x30\xc3\x30\xd5\x30\xa1\x30\xc5\x60\x31\x58\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�萔�o�b�t�@�����i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
		goto ERR ;
	}

	// �돉����
	_MEMSET( NewBuffer, 0, AllocSize ) ;

	// ID3D11Buffer �̍쐬
	_MEMSET( &BufferDesc, 0, sizeof( BufferDesc ) ) ;
	BufferDesc.ByteWidth      = Size ;
	BufferDesc.Usage          = D_D3D11_USAGE_DEFAULT ;
	BufferDesc.BindFlags      = D_D3D11_BIND_CONSTANT_BUFFER ;
	BufferDesc.CPUAccessFlags = 0 ;
	if( D3D11Device_CreateBuffer_ASync( &BufferDesc, NULL, &NewBuffer->ConstantBuffer, ASyncThread ) != S_OK )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\x9a\x5b\x70\x65\xd0\x30\xc3\x30\xd5\x30\xa1\x30\x28\x75\x6e\x30\x20\x00\x49\x00\x44\x00\x33\x00\x44\x00\x31\x00\x31\x00\x42\x00\x75\x00\x66\x00\x66\x00\x65\x00\x72\x00\x20\x00\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�萔�o�b�t�@�p�� ID3D11Buffer �̍쐬�Ɏ��s���܂���\n" @*/ )) ;
		goto ERR ;
	}

	// �����Z�b�g
	NewBuffer->Size         = Size ;
	NewBuffer->SysmemBuffer = NewBuffer + 1 ;
	NewBuffer->ChangeFlag   = FALSE ;

	// ���X�g�ɒǉ�
	if( GD3D11.Device.Shader.Constant.ConstantBufferManage.FirstBuffer != NULL )
	{
		NewBuffer->NextData = GD3D11.Device.Shader.Constant.ConstantBufferManage.FirstBuffer ;
		GD3D11.Device.Shader.Constant.ConstantBufferManage.FirstBuffer->PrevData = NewBuffer ;
	}
	GD3D11.Device.Shader.Constant.ConstantBufferManage.FirstBuffer = NewBuffer ;

	// �o�b�t�@�̐��𑝂₷
	GD3D11.Device.Shader.Constant.ConstantBufferManage.BufferNum ++ ;

	// �N���e�B�J���Z�N�V�����̉��
	CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_SHADER_CONSTANT_BUFFER ].CriticalSection ) ;

	// ����I��
	return NewBuffer ;


	// �G���[����
ERR :
	if( NewBuffer != NULL )
	{
		if( NewBuffer->ConstantBuffer != NULL )
		{
			Direct3D11_Release_Buffer( NewBuffer->ConstantBuffer ) ;
			NewBuffer->ConstantBuffer = NULL ;
		}

		DXFREE( NewBuffer ) ;
	}

	// �N���e�B�J���Z�N�V�����̉��
	CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_SHADER_CONSTANT_BUFFER ].CriticalSection ) ;

	// �G���[�I��
	return NULL ;
}

// �萔�o�b�t�@���폜����
extern int Graphics_D3D11_ConstantBuffer_Delete( CONSTANTBUFFER_DIRECT3D11 *ConstantBuffer )
{
	int i ;

	// �N���e�B�J���Z�N�V�����̎擾
	CRITICALSECTION_LOCK( &HandleManageArray[ DX_HANDLETYPE_SHADER_CONSTANT_BUFFER ].CriticalSection ) ;

	// �f�o�C�X�Ŏg�p����Ă�����O��
	for( i = 0 ; i < D_D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT ; i ++ )
	{
		if( GD3D11.Device.State.SetPixelShaderConstantBuffer[ i ] == ConstantBuffer->ConstantBuffer )
		{
			GD3D11.Device.State.SetPixelShaderConstantBuffer[ i ] = NULL ;
		}

		if( GD3D11.Device.State.SetVertexShaderConstantBuffer[ i ] == ConstantBuffer->ConstantBuffer )
		{
			GD3D11.Device.State.SetVertexShaderConstantBuffer[ i ] = NULL ;
		}
	}

	// ID3D11Buffer �����
	if( ConstantBuffer->ConstantBuffer != NULL )
	{
		Direct3D11_Release_Buffer( ConstantBuffer->ConstantBuffer ) ;
		ConstantBuffer->ConstantBuffer = NULL ;
	}

	// ���X�g����O��
	if( GD3D11.Device.Shader.Constant.ConstantBufferManage.FirstBuffer == ConstantBuffer )
	{
		GD3D11.Device.Shader.Constant.ConstantBufferManage.FirstBuffer = ConstantBuffer->NextData ;
		if( ConstantBuffer->NextData != NULL )
		{
			ConstantBuffer->NextData->PrevData = NULL ;
		}
	}
	else
	{
		ConstantBuffer->PrevData->NextData = ConstantBuffer->NextData ;
		if( ConstantBuffer->NextData != NULL )
		{
			ConstantBuffer->NextData->PrevData = ConstantBuffer->PrevData ;
		}
	}

	// �������̉��
	DXFREE( ConstantBuffer ) ;

	// �o�b�t�@�̐������炷
	GD3D11.Device.Shader.Constant.ConstantBufferManage.BufferNum -- ;

	// �N���e�B�J���Z�N�V�����̉��
	CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_SHADER_CONSTANT_BUFFER ].CriticalSection ) ;

	// �I��
	return 0 ;
}

// �V�X�e����������̃o�b�t�@�ɍs�����ύX��K�p����
extern int Graphics_D3D11_ConstantBuffer_Update( CONSTANTBUFFER_DIRECT3D11 *ConstantBuffer )
{
	// �ύX�������ꍇ�͉��������ɏI��
	if( ConstantBuffer->ChangeFlag == FALSE )
	{
		return 0 ;
	}

	// �X�V
	D3D11DeviceContext_UpdateSubresource( ConstantBuffer->ConstantBuffer, 0, NULL, ConstantBuffer->SysmemBuffer, 0, 0 ) ;

	// �ύX������t���O��|��
	ConstantBuffer->ChangeFlag = FALSE ;

	// �I��
	return 0 ;
}

// �w��̒萔�o�b�t�@�𒸓_�V�F�[�_�[�ɐݒ肷��
extern int Graphics_D3D11_ConstantBuffer_VSSet( UINT StartSlot, UINT NumBuffers, CONSTANTBUFFER_DIRECT3D11 * const *pConstantBuffers )
{
	D_ID3D11Buffer *D3D11ConstantBuffer[ D_D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT ] ;
	UINT                 i ;

	if( StartSlot < 0 || StartSlot + NumBuffers > D_D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT )
	{
		return -1 ;
	}

	if( NumBuffers == 1 )
	{
		return Graphics_D3D11_DeviceState_SetVertexShaderConstantBuffers( StartSlot, NumBuffers, &pConstantBuffers[ 0 ]->ConstantBuffer ) ;
	}
	
	for( i = 0 ; i < NumBuffers ; i ++ )
	{
		D3D11ConstantBuffer[ i ] = pConstantBuffers[ i ]->ConstantBuffer ;
	}

	return Graphics_D3D11_DeviceState_SetVertexShaderConstantBuffers( StartSlot, NumBuffers, D3D11ConstantBuffer ) ;
}

// �w��̒萔�o�b�t�@���s�N�Z���V�F�[�_�[�ɐݒ肷��
extern int Graphics_D3D11_ConstantBuffer_PSSet( UINT StartSlot, UINT NumBuffers, CONSTANTBUFFER_DIRECT3D11 * const *pConstantBuffers )
{
	D_ID3D11Buffer *D3D11ConstantBuffer[ D_D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT ] ;
	UINT                 i ;

	if( StartSlot < 0 || StartSlot + NumBuffers > D_D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT )
	{
		return -1 ;
	}

	if( NumBuffers == 1 )
	{
		return Graphics_D3D11_DeviceState_SetPixelShaderConstantBuffers( StartSlot, NumBuffers, &pConstantBuffers[ 0 ]->ConstantBuffer ) ;
	}
	
	for( i = 0 ; i < NumBuffers ; i ++ )
	{
		D3D11ConstantBuffer[ i ] = pConstantBuffers[ i ]->ConstantBuffer ;
	}

	return Graphics_D3D11_DeviceState_SetPixelShaderConstantBuffers( StartSlot, NumBuffers, D3D11ConstantBuffer ) ;
}
































// ���̓��C�A�E�g�֌W�֐�

static D_D3D11_INPUT_ELEMENT_DESC MakeD3D11InputElement( LPCSTR SemanticName, UINT SemanticIndex, D_DXGI_FORMAT Format, UINT InputSlot, UINT AlignedByteOffset )
{
	D_D3D11_INPUT_ELEMENT_DESC Result = 
	{
		SemanticName,
		SemanticIndex,
		Format,
		InputSlot,
		AlignedByteOffset,
		D_D3D11_INPUT_PER_VERTEX_DATA,
		0
	} ;
	return Result ;
}

// �W���`��p�� ID3D11InputLayout �I�u�W�F�N�g�̍쐬( 0:����  -1:���s )
extern int Graphics_D3D11_InputLayout_Base_Create( void )
{
	int							i ;
	int							j ;
	int							ElementNum ;
	HRESULT						hr ;
	GRAPHICS_HARDWARE_DIRECT3D11_BASE_SIMPLE_VERTEXSHADER_INFO *BSVI ;
	GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO                *Code ;

	// ���ɍ쐬����Ă����Ƃ��̂��߂ɍ폜�������s��
	Graphics_D3D11_InputLayout_Base_Terminate() ;

	DXST_ERRORLOG_ADDUTF16LE( "\x04\x54\x2e\x7a\x20\x00\x49\x00\x44\x00\x33\x00\x44\x00\x31\x00\x31\x00\x49\x00\x6e\x00\x70\x00\x75\x00\x74\x00\x4c\x00\x61\x00\x79\x00\x6f\x00\x75\x00\x74\x00\x20\x00\x6e\x30\x5c\x4f\x10\x62\x2e\x00\x2e\x00\x2e\x00\x2e\x00\x20\x00\x00"/*@ L"�e�� ID3D11InputLayout �̍쐬.... " @*/ ) ;

	// �W���`��p�̃V�F�[�_�[�Ŏg�p���钸�_�f�[�^�`�����쐬
	BSVI = g_BaseSimpleVertexShaderInfo ;
	Code = GraphicsHardDataDirect3D11.ShaderCode.Base.BaseSimple_VS_Code ;
	for( i = 0 ; i < D3D11_VERTEX_INPUTLAYOUT_NUM ; i++, BSVI++, Code++ )
	{
		// ���_�Ɋ܂܂����̐��𐔂���
		for( ElementNum = 0 ; BSVI->InputElementDesc[ ElementNum ].SemanticName != NULL ; ElementNum ++ ){}

		// BGRA�J���[���g�p����ꍇ�͒��_�J���[�t�H�[�}�b�g��ύX����
		if( GSYS.HardInfo.UseVertexColorBGRAFormat )
		{
			for( j = 0 ; j < ElementNum ; j ++ )
			{
				if( BSVI->InputElementDesc[ j ].Format == D_DXGI_FORMAT_R8G8B8A8_UNORM )
				{
					BSVI->InputElementDesc[ j ].Format = D_DXGI_FORMAT_B8G8R8A8_UNORM ;
				}
			}
		}

		// ���_�f�[�^�t�H�[�}�b�g���쐬
		hr = D3D11Device_CreateInputLayout( BSVI->InputElementDesc, ( UINT )ElementNum, Code->Binary, ( SIZE_T )Code->Size, &GD3D11.Device.InputLayout.BaseInputLayout[ i ] ) ;
		if( hr != S_OK )
		{
			DXST_ERRORLOG_ADDUTF16LE( "\x19\x6a\x96\x6e\xcf\x63\x3b\x75\x28\x75\x6e\x30\x20\x00\x49\x00\x44\x00\x33\x00\x44\x00\x31\x00\x31\x00\x49\x00\x6e\x00\x70\x00\x75\x00\x74\x00\x4c\x00\x61\x00\x79\x00\x6f\x00\x75\x00\x74\x00\x20\x00\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x0a\x00\x00"/*@ L"�W���`��p�� ID3D11InputLayout �̍쐬�Ɏ��s\n" @*/ ) ;
			return -1 ;
		}
	}

	// �P���]���p�̒��_�f�[�^�t�H�[�}�b�g���쐬
	{
		static D_D3D11_INPUT_ELEMENT_DESC StretchRectInputElementDesc[] =
		{
			{ "POSITION",     0, D_DXGI_FORMAT_R32G32_FLOAT, 0,  0, D_D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD",     0, D_DXGI_FORMAT_R32G32_FLOAT, 0,  8, D_D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD",     1, D_DXGI_FORMAT_R32G32_FLOAT, 0, 16, D_D3D11_INPUT_PER_VERTEX_DATA, 0 },
		} ;

		static D_D3D11_INPUT_ELEMENT_DESC StretchRectTex8InputElementDesc[] =
		{
			{ "POSITION",     0, D_DXGI_FORMAT_R32G32_FLOAT, 0,  0, D_D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD",     0, D_DXGI_FORMAT_R32G32_FLOAT, 0,  8, D_D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD",     1, D_DXGI_FORMAT_R32G32_FLOAT, 0, 16, D_D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD",     2, D_DXGI_FORMAT_R32G32_FLOAT, 0, 24, D_D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD",     3, D_DXGI_FORMAT_R32G32_FLOAT, 0, 32, D_D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD",     4, D_DXGI_FORMAT_R32G32_FLOAT, 0, 40, D_D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD",     5, D_DXGI_FORMAT_R32G32_FLOAT, 0, 48, D_D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD",     6, D_DXGI_FORMAT_R32G32_FLOAT, 0, 56, D_D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD",     7, D_DXGI_FORMAT_R32G32_FLOAT, 0, 64, D_D3D11_INPUT_PER_VERTEX_DATA, 0 },
		} ;

		hr = D3D11Device_CreateInputLayout(
			StretchRectInputElementDesc,
			sizeof( StretchRectInputElementDesc ) / sizeof( D_D3D11_INPUT_ELEMENT_DESC ),
			GD3D11.ShaderCode.Base.StretchRect_VS_Code.Binary,
			( SIZE_T )GD3D11.ShaderCode.Base.StretchRect_VS_Code.Size,
			&GD3D11.Device.InputLayout.BaseStretchRectInputLayout ) ;
		if( hr != S_OK )
		{
			DXST_ERRORLOG_ADDUTF16LE( "\x58\x53\x14\x7d\xe2\x8e\x01\x90\x28\x75\x6e\x30\x20\x00\x49\x00\x44\x00\x33\x00\x44\x00\x31\x00\x31\x00\x49\x00\x6e\x00\x70\x00\x75\x00\x74\x00\x4c\x00\x61\x00\x79\x00\x6f\x00\x75\x00\x74\x00\x20\x00\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x0a\x00\x00"/*@ L"�P���]���p�� ID3D11InputLayout �̍쐬�Ɏ��s\n" @*/ ) ;
			return -1 ;
		}

		hr = D3D11Device_CreateInputLayout(
			StretchRectTex8InputElementDesc,
			sizeof( StretchRectTex8InputElementDesc ) / sizeof( D_D3D11_INPUT_ELEMENT_DESC ),
			GD3D11.ShaderCode.Base.StretchRectTex8_VS_Code.Binary,
			( SIZE_T )GD3D11.ShaderCode.Base.StretchRectTex8_VS_Code.Size,
			&GD3D11.Device.InputLayout.BaseStretchRectTex8InputLayout ) ;
		if( hr != S_OK )
		{
			DXST_ERRORLOG_ADDUTF16LE( "\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\xa7\x5e\x19\x6a\x38\x00\x0b\x50\x6e\x30\x58\x53\x14\x7d\xe2\x8e\x01\x90\x28\x75\x6e\x30\x20\x00\x49\x00\x44\x00\x33\x00\x44\x00\x31\x00\x31\x00\x49\x00\x6e\x00\x70\x00\x75\x00\x74\x00\x4c\x00\x61\x00\x79\x00\x6f\x00\x75\x00\x74\x00\x20\x00\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x0a\x00\x00"/*@ L"�e�N�X�`�����W8�̒P���]���p�� ID3D11InputLayout �̍쐬�Ɏ��s\n" @*/ ) ;
			return -1 ;
		}
	}

	DXST_ERRORLOG_ADDUTF16LE( "\x10\x62\x9f\x52\x0a\x00\x00"/*@ L"����\n" @*/ ) ;

	// ����I��
	return 0 ;
}

// �W���`��p�� ID3D11InputLayout �I�u�W�F�N�g�̍폜( 0:����  -1:���s )
extern int Graphics_D3D11_InputLayout_Base_Terminate( void )
{
	int							i ;
	GRAPHICS_HARDWARE_DIRECT3D11_BASE_SIMPLE_VERTEXSHADER_INFO *BSVI ;

	if( GAPIWin.D3D11DeviceObject == NULL )
	{
		return -1 ;
	}

	// �W���`��p�̃V�F�[�_�[�Ŏg�p���钸�_�f�[�^�`�������
	BSVI = g_BaseSimpleVertexShaderInfo ;
	for( i = 0 ; i < D3D11_VERTEX_INPUTLAYOUT_NUM ; i++, BSVI++ )
	{
		if( GD3D11.Device.InputLayout.BaseInputLayout[ i ] != NULL )
		{
			Direct3D11_Release_InputLayout( GD3D11.Device.InputLayout.BaseInputLayout[ i ] ) ;
			GD3D11.Device.InputLayout.BaseInputLayout[ i ] = NULL ;
		}
	}

	// �P���]���p�̒��_�f�[�^�t�H�[�}�b�g�����
	if( GD3D11.Device.InputLayout.BaseStretchRectInputLayout != NULL )
	{
		Direct3D11_Release_InputLayout( GD3D11.Device.InputLayout.BaseStretchRectInputLayout ) ;
		GD3D11.Device.InputLayout.BaseStretchRectInputLayout = NULL ;
	}
	if( GD3D11.Device.InputLayout.BaseStretchRectTex8InputLayout != NULL )
	{
		Direct3D11_Release_InputLayout( GD3D11.Device.InputLayout.BaseStretchRectTex8InputLayout ) ;
		GD3D11.Device.InputLayout.BaseStretchRectTex8InputLayout = NULL ;
	}

	// ����I��
	return 0 ;
}

#ifndef DX_NON_MODEL

// ���f���`��p�� ID3D11InputLayout �I�u�W�F�N�g�̍쐬( 0:����  -1:���s )
extern int Graphics_D3D11_InputLayout_Model_Create( int BumpMap, int MeshType )
{
	DWORD offset, index ;
	HRESULT hr ;
	D_ID3D11VertexShader **VS = NULL ;
	GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO *VSAddress = NULL ;
	D_D3D11_INPUT_ELEMENT_DESC InputElement[ 64 ] = 
	{
		{ "POSITION",     0, D_DXGI_FORMAT_R32G32B32_FLOAT,    0,  0, D_D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",       0, D_DXGI_FORMAT_R32G32B32_FLOAT,    0, 12, D_D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",        0, D_DXGI_FORMAT_R8G8B8A8_UNORM,     0, 24, D_D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",        1, D_DXGI_FORMAT_R8G8B8A8_UNORM,     0, 28, D_D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",     0, D_DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 32, D_D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",     1, D_DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 48, D_D3D11_INPUT_PER_VERTEX_DATA, 0 },
	} ;

	if( GAPIWin.D3D11DeviceObject == NULL )
	{
		return -1 ;
	}

	// ���ɂ������牽�������I��
	if( GD3D11.Device.InputLayout.MV1_VertexInputLayout[ BumpMap ][ MeshType ] != NULL )
	{
		return 0 ;
	}

	index  = 6 ;
	offset = 64 ;
	if( BumpMap )
	{
		InputElement[ index ] = MakeD3D11InputElement( "TANGENT",      0, D_DXGI_FORMAT_R32G32B32_FLOAT,    0, offset ) ;
		index++ ;
		offset += sizeof( VECTOR ) ;

		InputElement[ index ] = MakeD3D11InputElement( "BINORMAL",     0, D_DXGI_FORMAT_R32G32B32_FLOAT,    0, offset ) ;
		index++ ;
		offset += sizeof( VECTOR ) ;
	}

	switch( MeshType )
	{
	case 0 :	// ���̃��b�V��
		break ;

	case 1 :	// �S�{�[���ȓ��X�L�j���O���b�V��
	case 2 :	// �W�{�[���ȓ��X�L�j���O���b�V��
		InputElement[ index ] = MakeD3D11InputElement( "BLENDINDICES", 0, D_DXGI_FORMAT_R8G8B8A8_UINT,      0, offset ) ;
		index++ ;
		offset += sizeof( BYTE ) * 4 ;

		InputElement[ index ] = MakeD3D11InputElement( "BLENDWEIGHT",  0, D_DXGI_FORMAT_R32G32B32A32_FLOAT, 0, offset ) ;
		index++ ;
		offset += sizeof( FLOAT4 ) ;

		if( MeshType == 2 )
		{
			InputElement[ index ] = MakeD3D11InputElement( "BLENDINDICES", 1, D_DXGI_FORMAT_R8G8B8A8_UINT,      0, offset ) ;
			index++ ;
			offset += sizeof( BYTE ) * 4 ;

			InputElement[ index ] = MakeD3D11InputElement( "BLENDWEIGHT",  1, D_DXGI_FORMAT_R32G32B32A32_FLOAT, 0, offset ) ;
			index++ ;
			offset += sizeof( FLOAT4 ) ;
		}
		break ;
	}

	VS        = &GD3D11.Device.Shader.Model.MV1_VertexLighting_VS     [ 0 ][ MeshType ][ BumpMap ][ 0 ][ 0 ] ;
	VSAddress =    &GD3D11.ShaderCode.Model.MV1_VertexLighting_VS_Code[ 0 ][ MeshType ][ BumpMap ][ 0 ][ 0 ] ;

	// �V�F�[�_�[�����邩�𒲂ׂ�
	if( *VS == NULL )
	{
		// �V�F�[�_�[�̍쐬�����݂�
		if( Graphics_D3D11_VertexShader_Create( VSAddress, VS, 1 ) != 0 )
		{
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\x47\x00\x72\x00\x61\x00\x70\x00\x68\x00\x69\x00\x63\x00\x73\x00\x5f\x00\x44\x00\x33\x00\x44\x00\x31\x00\x31\x00\x5f\x00\x49\x00\x6e\x00\x70\x00\x75\x00\x74\x00\x4c\x00\x61\x00\x79\x00\x6f\x00\x75\x00\x74\x00\x5f\x00\x4d\x00\x6f\x00\x64\x00\x65\x00\x6c\x00\x5f\x00\x43\x00\x72\x00\x65\x00\x61\x00\x74\x00\x65\x00\x20\x00\x67\x30\x02\x98\xb9\x70\xb7\x30\xa7\x30\xfc\x30\xc0\x30\xfc\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Graphics_D3D11_InputLayout_Model_Create �Œ��_�V�F�[�_�[�̍쐬�Ɏ��s���܂���\n" @*/ )) ;
			return -1 ;
		}
	}

	hr = D3D11Device_CreateInputLayout( InputElement, index, VSAddress->Binary, ( SIZE_T )VSAddress->Size, &GD3D11.Device.InputLayout.MV1_VertexInputLayout[ BumpMap ][ MeshType ] ) ;
	if( hr != S_OK )
	{
		return -1 ;
	}

	// ����I��
	return 0 ;
}

// ���f���`��p�� ID3D11InputLayout �I�u�W�F�N�g�̍폜( 0:����  -1:���s )
extern int Graphics_D3D11_InputLayout_Model_Terminate( void )
{
	int i ;
	int j ;

	if( GAPIWin.D3D11DeviceObject == NULL )
	{
		return -1 ;
	}

	for( i = 0; i < 2; i ++ )
	{
		for( j = 0; j < 3; j ++ )
		{
			if( GD3D11.Device.InputLayout.MV1_VertexInputLayout[ i ][ j ] != NULL )
			{
				Direct3D11_Release_InputLayout( GD3D11.Device.InputLayout.MV1_VertexInputLayout[ i ][ j ] ) ;
				GD3D11.Device.InputLayout.MV1_VertexInputLayout[ i ][ j ] = NULL ;
			}
		}
	}

	return 0 ;
}

#endif // DX_NON_MODEL














// �W���`��֌W

// �W���`��p�̒��_�o�b�t�@�̍쐬( 0:����  -1:���s )
extern	int		Graphics_D3D11_NormalDrawVertexBuffer_Create( void )
{
	D_D3D11_BUFFER_DESC BufferDesc ;
	HRESULT             hr ;

	DXST_ERRORLOG_ADDUTF16LE( "\x19\x6a\x96\x6e\xcf\x63\x3b\x75\x28\x75\x6e\x30\x02\x98\xb9\x70\xd0\x30\xc3\x30\xd5\x30\xa1\x30\x6e\x30\x5c\x4f\x10\x62\x2e\x00\x2e\x00\x2e\x00\x2e\x00\x20\x00\x00"/*@ L"�W���`��p�̒��_�o�b�t�@�̍쐬.... " @*/ ) ;

	_MEMSET( &BufferDesc, 0, sizeof( BufferDesc ) ) ;
	BufferDesc.ByteWidth      = D3D11_VERTEXBUFFER_SIZE ;
	BufferDesc.BindFlags      = D_D3D11_BIND_VERTEX_BUFFER ;
#if VERTEXBUFFERMODE != 0
	BufferDesc.Usage          = D_D3D11_USAGE_DYNAMIC ;
	BufferDesc.CPUAccessFlags = D_D3D11_CPU_ACCESS_WRITE ;
#else
	BufferDesc.Usage          = D_D3D11_USAGE_DEFAULT ;
	BufferDesc.CPUAccessFlags = 0 ;
#endif
	hr = D3D11Device_CreateBuffer( &BufferDesc, NULL, &GD3D11.Device.DrawInfo.VertexBuffer ) ;
	if( FAILED( hr ) )
	{
		DXST_ERRORLOG_ADDUTF16LE( "\x31\x59\x57\x65\x0a\x00\x00"/*@ L"���s\n" @*/ ) ;
		return -1 ;
	}

	DXST_ERRORLOG_ADDUTF16LE( "\x10\x62\x9f\x52\x0a\x00\x00"/*@ L"����\n" @*/ ) ;

	// ����I��
	return 0 ;
}

// �W���`��p�̒��_�o�b�t�@�̍폜( 0:����  -1:���s )
extern	int		Graphics_D3D11_NormalDrawVertexBuffer_Terminate( void )
{
	if( GD3D11.Device.DrawInfo.VertexBuffer )
	{
//		NORMALVERTEXBUFFER_UNMAP
		Direct3D11_Release_Buffer( GD3D11.Device.DrawInfo.VertexBuffer ) ;
		GD3D11.Device.DrawInfo.VertexBuffer = NULL ;
		GD3D11.Device.DrawInfo.VertexBufferAddr = NULL ;
		GD3D11.Device.DrawInfo.VertexBufferNextAddr = NULL ;
	}

	// ����I��
	return 0 ;
}

// �ėp�`��p���_�o�b�t�@���g�p���ăV�X�e����������̒��_�f�[�^�̕`����s��
extern int Graphics_D3D11_CommonBuffer_DrawPrimitive(
	int                        InputLayout,
	D_D3D11_PRIMITIVE_TOPOLOGY PrimitiveType,
	const void *               VertexData,
	int                        VertexNum,
	int                        UseDefaultVertexShader
)
{
	int         VertexSize ;
//	D_D3D11_BOX DestBox ;

	VertexSize = D3D11_VertexSize[ InputLayout ] ;

	Graphics_D3D11_CommonVertexBuffer_Setup( VertexSize * VertexNum ) ;

//	DestBox.left   = 0 ;
//	DestBox.top    = 0 ;
//	DestBox.front  = 0 ;
//	DestBox.bottom = 1 ;
//	DestBox.back   = 1 ;
//	DestBox.right  = ( UINT )( VertexSize * VertexNum ) ;
//	D3D11DeviceContext_UpdateSubresource_ASync( GD3D11.Device.DrawInfo.CommonVertexBuffer, 0, &DestBox, VertexData, 0, 0 ) ;
	D_D3D11_MAPPED_SUBRESOURCE MappedSubResource ;
	HRESULT                    hr ;
	hr = D3D11DeviceContext_Map_ASync( 
		GD3D11.Device.DrawInfo.CommonVertexBuffer,
		0,
		D_D3D11_MAP_WRITE_DISCARD,
		0,
		&MappedSubResource
	) ;
	_MEMCPY( MappedSubResource.pData, VertexData, VertexSize * VertexNum ) ;
	D3D11DeviceContext_Unmap_ASync(
		GD3D11.Device.DrawInfo.CommonVertexBuffer,
		0
	) ;

	// �v���~�e�B�u�^�C�v���Z�b�g
	Graphics_D3D11_DeviceState_SetPrimitiveTopology( PrimitiveType ) ;

	// ���̓��C�A�E�g���Z�b�g
	Graphics_D3D11_DeviceState_SetInputLayout( GD3D11.Device.InputLayout.BaseInputLayout[ InputLayout ] ) ;

	// ���_�V�F�[�_�[���Z�b�g
	if( UseDefaultVertexShader )
	{
		if( InputLayout == D3D11_VERTEX_INPUTLAYOUT_3D_LIGHT )
		{
			if( Graphics_D3D11_Shader_Normal3DDraw_Setup() == FALSE )
			{
				return -1 ;
			}
		}
		else
		{
			Graphics_D3D11_DeviceState_SetVertexShader( GD3D11.Device.Shader.Base.BaseSimple_VS[ InputLayout ], TRUE ) ;
		}
	}

	// ���_�o�b�t�@���Z�b�g
	Graphics_D3D11_DeviceState_SetVertexBuffer( GD3D11.Device.DrawInfo.CommonVertexBuffer, D3D11_VertexSize[ InputLayout ] ) ;

	// �X�e�[�g�ƒ萔�o�b�t�@�̍X�V
	Graphics_D3D11_DeviceState_SetupStateAndConstantBuffer() ;

	// �`��
	D3D11DeviceContext_Draw( ( UINT )VertexNum, 0 ) ;

	// �I��
	return 0 ;
}

// �ėp�`��p���_�o�b�t�@�E�C���f�b�N�X�o�b�t�@���g�p���ăV�X�e����������̒��_�f�[�^�E�C���f�b�N�X�f�[�^�̕`����s��
extern int Graphics_D3D11_CommonBuffer_DrawIndexedPrimitive(
	int							InputLayout,
	D_D3D11_PRIMITIVE_TOPOLOGY	PrimitiveType,
	const void *				VertexData,
	int							VertexNum,
	const void *				IndexData,
	int							IndexNum,
	D_DXGI_FORMAT				IndexFormat,
	int                         UseDefaultVertexShader
)
{
	int         VertexSize ;
	int         IndexSize ;
//	D_D3D11_BOX DestBox ;

	VertexSize = D3D11_VertexSize[ InputLayout ] ;
	IndexSize = 0 ;
	switch( IndexFormat )
	{
	case D_DXGI_FORMAT_R16_UINT :
		IndexSize = 2 ;
		break ;

	case D_DXGI_FORMAT_R32_UINT :
		IndexSize = 4 ;
		break ;
	}

	Graphics_D3D11_CommonVertexBuffer_Setup( VertexSize * VertexNum ) ;
	Graphics_D3D11_CommonIndexBuffer_Setup(  IndexSize  * IndexNum ) ;

//	DestBox.left   = 0 ;
//	DestBox.top    = 0 ;
//	DestBox.front  = 0 ;
//	DestBox.bottom = 1 ;
//	DestBox.back   = 1 ;
//	DestBox.right  = ( UINT )( VertexSize * VertexNum ) ;
//	D3D11DeviceContext_UpdateSubresource_ASync( GD3D11.Device.DrawInfo.CommonVertexBuffer, 0, &DestBox, VertexData, 0, 0 ) ;
	D_D3D11_MAPPED_SUBRESOURCE MappedSubResource ;
	HRESULT                    hr ;
	hr = D3D11DeviceContext_Map_ASync( 
		GD3D11.Device.DrawInfo.CommonVertexBuffer,
		0,
		D_D3D11_MAP_WRITE_DISCARD,
		0,
		&MappedSubResource
	) ;
	_MEMCPY( MappedSubResource.pData, VertexData, VertexSize * VertexNum ) ;
	D3D11DeviceContext_Unmap_ASync(
		GD3D11.Device.DrawInfo.CommonVertexBuffer,
		0
	) ;

//	DestBox.right  = ( UINT )( IndexSize  * IndexNum ) ;
//	D3D11DeviceContext_UpdateSubresource_ASync( GD3D11.Device.DrawInfo.CommonIndexBuffer,  0, &DestBox, IndexData,  0, 0 ) ;
	hr = D3D11DeviceContext_Map_ASync( 
		GD3D11.Device.DrawInfo.CommonIndexBuffer,
		0,
		D_D3D11_MAP_WRITE_DISCARD,
		0,
		&MappedSubResource
	) ;
	_MEMCPY( MappedSubResource.pData, IndexData, IndexSize * IndexNum ) ;
	D3D11DeviceContext_Unmap_ASync(
		GD3D11.Device.DrawInfo.CommonIndexBuffer,
		0
	) ;

	// �v���~�e�B�u�^�C�v���Z�b�g
	Graphics_D3D11_DeviceState_SetPrimitiveTopology( PrimitiveType ) ;

	// ���̓��C�A�E�g���Z�b�g
	Graphics_D3D11_DeviceState_SetInputLayout( GD3D11.Device.InputLayout.BaseInputLayout[ InputLayout ] ) ;

	// ���_�V�F�[�_�[���Z�b�g
	if( UseDefaultVertexShader )
	{
		if( InputLayout == D3D11_VERTEX_INPUTLAYOUT_3D_LIGHT )
		{
			if( Graphics_D3D11_Shader_Normal3DDraw_Setup() == FALSE )
			{
				return -1 ;
			}
		}
		else
		{
			Graphics_D3D11_DeviceState_SetVertexShader( GD3D11.Device.Shader.Base.BaseSimple_VS[ InputLayout ], TRUE ) ;
		}
	}

	// ���_�o�b�t�@���Z�b�g
	Graphics_D3D11_DeviceState_SetVertexBuffer( GD3D11.Device.DrawInfo.CommonVertexBuffer, D3D11_VertexSize[ InputLayout ] ) ;

	// �C���f�b�N�X�o�b�t�@���Z�b�g
	Graphics_D3D11_DeviceState_SetIndexBuffer( GD3D11.Device.DrawInfo.CommonIndexBuffer, IndexFormat ) ;

	// �X�e�[�g�ƒ萔�o�b�t�@�̍X�V
	Graphics_D3D11_DeviceState_SetupStateAndConstantBuffer() ;

	// �`��
	D3D11DeviceContext_DrawIndexed( ( UINT )IndexNum, 0, 0 ) ;

	// �I��
	return 0 ;
}

// �w��T�C�Y�̔ėp�`��p���_�o�b�t�@�̏������s��
extern int Graphics_D3D11_CommonVertexBuffer_Setup( int Size )
{
	D_D3D11_BUFFER_DESC BufferDesc ;
	HRESULT             hr ;

	if( Size <= GD3D11.Device.DrawInfo.CommonVertexBufferSize )
	{
		return 0 ;
	}

	Graphics_D3D11_CommonVertexBuffer_Terminate() ;

	Size += D3D11_COMMONBUFFER_ADDSIZE ;
	Size = ( Size + 15 ) / 16 * 16 ;

	_MEMSET( &BufferDesc, 0, sizeof( BufferDesc ) ) ;
	BufferDesc.ByteWidth      = ( UINT )Size ;
	BufferDesc.Usage          = D_D3D11_USAGE_DYNAMIC ;
//	BufferDesc.Usage          = D_D3D11_USAGE_DEFAULT ;
	BufferDesc.BindFlags      = D_D3D11_BIND_VERTEX_BUFFER ;
	BufferDesc.CPUAccessFlags = D_D3D11_CPU_ACCESS_WRITE ;
//	BufferDesc.CPUAccessFlags = 0 ;
	hr = D3D11Device_CreateBuffer( &BufferDesc, NULL, &GD3D11.Device.DrawInfo.CommonVertexBuffer ) ;
	if( FAILED( hr ) )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\x4e\x6c\x28\x75\xcf\x63\x3b\x75\x28\x75\x6e\x30\x02\x98\xb9\x70\xd0\x30\xc3\x30\xd5\x30\xa1\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x28\x00\x20\x00\xb5\x30\xa4\x30\xba\x30\x20\x00\x25\x00\x64\x00\x20\x00\x62\x00\x79\x00\x74\x00\x65\x00\x20\x00\x29\x00\x0a\x00\x00"/*@ L"�ėp�`��p�̒��_�o�b�t�@�̍쐬�Ɏ��s���܂���( �T�C�Y %d byte )\n" @*/, Size ) ) ;
		return -1 ;
	}

	GD3D11.Device.DrawInfo.CommonVertexBufferSize = Size ;

	// ����I��
	return 0 ;
}

// �w��T�C�Y�̔ėp�`��p�C���f�b�N�X�o�b�t�@�̏������s��
extern int Graphics_D3D11_CommonIndexBuffer_Setup( int Size )
{
	D_D3D11_BUFFER_DESC BufferDesc ;
	HRESULT             hr ;

	if( Size <= GD3D11.Device.DrawInfo.CommonIndexBufferSize )
	{
		return 0 ;
	}

	Graphics_D3D11_CommonIndexBuffer_Terminate() ;

	Size += D3D11_COMMONBUFFER_ADDSIZE ;
	Size = ( Size + 15 ) / 16 * 16 ;

	_MEMSET( &BufferDesc, 0, sizeof( BufferDesc ) ) ;
	BufferDesc.ByteWidth      = ( UINT )Size ;
	BufferDesc.Usage          = D_D3D11_USAGE_DYNAMIC ;
//	BufferDesc.Usage          = D_D3D11_USAGE_DEFAULT ;
	BufferDesc.BindFlags      = D_D3D11_BIND_INDEX_BUFFER ;
	BufferDesc.CPUAccessFlags = D_D3D11_CPU_ACCESS_WRITE ;
//	BufferDesc.CPUAccessFlags = 0 ;
	hr = D3D11Device_CreateBuffer( &BufferDesc, NULL, &GD3D11.Device.DrawInfo.CommonIndexBuffer ) ;
	if( FAILED( hr ) )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\x4e\x6c\x28\x75\xcf\x63\x3b\x75\x28\x75\x6e\x30\xa4\x30\xf3\x30\xc7\x30\xc3\x30\xaf\x30\xb9\x30\xd0\x30\xc3\x30\xd5\x30\xa1\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x28\x00\x20\x00\xb5\x30\xa4\x30\xba\x30\x20\x00\x25\x00\x64\x00\x20\x00\x62\x00\x79\x00\x74\x00\x65\x00\x20\x00\x29\x00\x0a\x00\x00"/*@ L"�ėp�`��p�̃C���f�b�N�X�o�b�t�@�̍쐬�Ɏ��s���܂���( �T�C�Y %d byte )\n" @*/, Size ) ) ;
		return -1 ;
	}

	GD3D11.Device.DrawInfo.CommonIndexBufferSize = Size ;

	// ����I��
	return 0 ;
}

// �ėp�`��p���_�o�b�t�@�̌�n�����s��
extern int Graphics_D3D11_CommonVertexBuffer_Terminate( void )
{
	if( GD3D11.Device.DrawInfo.CommonVertexBuffer == NULL )
	{
		return 0 ;
	}

	if( GD3D11.Device.State.SetVertexBuffer == GD3D11.Device.DrawInfo.CommonVertexBuffer )
	{
		Graphics_D3D11_DeviceState_SetVertexBuffer( NULL, 0 ) ;
	}

	Direct3D11_Release_Buffer( GD3D11.Device.DrawInfo.CommonVertexBuffer ) ;
	GD3D11.Device.DrawInfo.CommonVertexBuffer = NULL ;

	GD3D11.Device.DrawInfo.CommonVertexBufferSize = 0 ;

	return 0 ;
}

// �ėp�`��p�C���f�b�N�X�o�b�t�@�̌�n�����s��
extern int Graphics_D3D11_CommonIndexBuffer_Terminate( void )
{
	if( GD3D11.Device.DrawInfo.CommonIndexBuffer == NULL )
	{
		return 0 ;
	}

	if( GD3D11.Device.State.SetIndexBuffer == GD3D11.Device.DrawInfo.CommonIndexBuffer )
	{
		Graphics_D3D11_DeviceState_SetIndexBuffer( NULL, D_DXGI_FORMAT_R16_UINT ) ;
	}

	Direct3D11_Release_Buffer( GD3D11.Device.DrawInfo.CommonIndexBuffer ) ;
	GD3D11.Device.DrawInfo.CommonIndexBuffer = NULL ;

	GD3D11.Device.DrawInfo.CommonIndexBufferSize = 0 ;

	return 0 ;
}






















// Direct3D11 �̕`�揈�������֌W

#ifndef DX_NON_ASYNCLOAD
static int Graphics_D3D11_RenderVertexASyncCallback( ASYNCLOAD_MAINTHREAD_REQUESTINFO * Info )
{
	return Graphics_D3D11_RenderVertex(
		( int )Info->Data[ 0 ],
		FALSE ) ;
}
#endif // DX_NON_ASYNCLOAD

// ���_�o�b�t�@�ɗ��܂������_�f�[�^�������_�����O����
extern	int		Graphics_D3D11_RenderVertex( int NextUse3DVertex, int ASyncThread )
{
#ifndef DX_NON_ASYNCLOAD
	if( ASyncThread )
	{
		ASYNCLOAD_MAINTHREAD_REQUESTINFO AInfo ;

		AInfo.Function = Graphics_D3D11_RenderVertexASyncCallback ;
		AInfo.Data[ 0 ] = ( DWORD_PTR )NextUse3DVertex ;
		return AddASyncLoadRequestMainThreadInfo( &AInfo ) ;
	}
#endif // DX_NON_ASYNCLOAD

	if( GAPIWin.D3D11DeviceObject == NULL )
	{
		return -1 ;
	}

	// ���_���������������`��͍s��Ȃ�
	if( GD3D11.Device.DrawInfo.VertexNum != 0 && DxLib_GetEndRequest() == FALSE )
	{
		// Graphics_D3D11_BeginScene �����Ă��Ȃ������炷��
//		if( GD3D11.Device.DrawInfo.BeginSceneFlag == FALSE ) Graphics_D3D11_BeginScene() ;

		// ��`��t���O�������Ă��Ȃ���΃����_�����O����
		if( GD3D11.Device.DrawInfo.BlendMaxNotDrawFlag == FALSE )
		{
			DWORD                      InputLayout ;

			InputLayout = D3D11_VertexTypeToInputLayout[ GD3D11.Device.DrawInfo.Use3DVertex ][ GD3D11.Device.DrawInfo.VertexType ] ;

			// ���_�f�[�^�𒸓_�o�b�t�@�ɔ��f
#if VERTEXBUFFERMODE == 2
			NORMALVERTEXBUFFER_UNMAP
#elif VERTEXBUFFERMODE == 1
			D_D3D11_MAPPED_SUBRESOURCE MappedSubResource ;
			HRESULT                    hr ;
			hr = D3D11DeviceContext_Map_ASync( 
				GD3D11.Device.DrawInfo.VertexBuffer,
				0,
				D_D3D11_MAP_WRITE_DISCARD,
				0,
				&MappedSubResource
			) ;
			_MEMCPY( MappedSubResource.pData, GD3D11.Device.DrawInfo.VertexBufferTemp, D3D11_VertexSize[ InputLayout ] * GD3D11.Device.DrawInfo.VertexNum ) ;
			D3D11DeviceContext_Unmap_ASync(
				GD3D11.Device.DrawInfo.VertexBuffer,
				0
			) ;
#else
			D_D3D11_BOX DestBox ;
			DestBox.left   = 0 ;
			DestBox.top    = 0 ;
			DestBox.front  = 0 ;
			DestBox.right  = ( UINT )( D3D11_VertexSize[ InputLayout ] * GD3D11.Device.DrawInfo.VertexNum ) ;
			DestBox.bottom = 1 ;
			DestBox.back   = 1 ;
			D3D11DeviceContext_UpdateSubresource_ASync(
				GD3D11.Device.DrawInfo.VertexBuffer,
				0,
				&DestBox,
				GD3D11.Device.DrawInfo.VertexBufferTemp,
				0,
				0,
				ASyncThread
			) ;
#endif

			// �v���~�e�B�u�^�C�v���Z�b�g
			if( GD3D11.Device.State.PrimitiveTopology != GD3D11.Device.DrawInfo.PrimitiveType )
			{
				Graphics_D3D11_DeviceState_SetPrimitiveTopology( GD3D11.Device.DrawInfo.PrimitiveType ) ;
			}

			// ���̓��C�A�E�g���Z�b�g
			if( GD3D11.Device.State.SetVertexInputLayout != GD3D11.Device.InputLayout.BaseInputLayout[ InputLayout ] )
			{
				Graphics_D3D11_DeviceState_SetInputLayout( GD3D11.Device.InputLayout.BaseInputLayout[ InputLayout ] ) ;
			}

			// ���_�V�F�[�_�[���Z�b�g
			if( InputLayout == D3D11_VERTEX_INPUTLAYOUT_3D_LIGHT )
			{
				if( Graphics_D3D11_Shader_Normal3DDraw_Setup() == FALSE )
				{
					return -1 ;
				}
			}
			else
			{
				if( GD3D11.Device.State.SetVertexShader != GD3D11.Device.Shader.Base.BaseSimple_VS[ InputLayout ] )
				{
					Graphics_D3D11_DeviceState_SetVertexShader( GD3D11.Device.Shader.Base.BaseSimple_VS[ InputLayout ], TRUE ) ;
				}
			}

			// ���_�o�b�t�@���Z�b�g
			if( GD3D11.Device.State.SetVertexBuffer       != GD3D11.Device.DrawInfo.VertexBuffer ||
				GD3D11.Device.State.SetVertexBufferStride != D3D11_VertexSize[ InputLayout ] )
			{
				Graphics_D3D11_DeviceState_SetVertexBuffer( GD3D11.Device.DrawInfo.VertexBuffer, D3D11_VertexSize[ InputLayout ] ) ;
			}

			// �X�e�[�g�ƒ萔�o�b�t�@�̍X�V
			Graphics_D3D11_DeviceState_SetupStateAndConstantBuffer_Inline() ;

			// �`��
			D3D11DeviceContext_Draw( ( UINT )GD3D11.Device.DrawInfo.VertexNum, 0 ) ;
		}
	}

	if( NextUse3DVertex >= 0 )
	{
		GD3D11.Device.DrawInfo.Use3DVertex = NextUse3DVertex ;
	}

	GD3D11.Device.DrawInfo.VertexNum = 0 ;

#if VERTEXBUFFERMODE == 2
	NORMALVERTEXBUFFER_MAP
	GD3D11.Device.DrawInfo.VertexBufferNextAddr = GD3D11.Device.DrawInfo.VertexBufferAddr ;
#else
	GD3D11.Device.DrawInfo.VertexBufferNextAddr = GD3D11.Device.DrawInfo.VertexBufferTemp ;
#endif

	// �I��
	return 0 ;
}

// �`�揀�����s��
extern	void	FASTCALL Graphics_D3D11_DrawPreparation( int ParamFlag )
{
	int AlphaTest ;
	int AlphaChannel ;
	int Specular ;
	int i ;

	// �K�� Graphics_D3D11_DrawPreparation ���s���ׂ��Ƃ����t���O��|��
	GD3D11.Device.DrawSetting.DrawPrepAlwaysFlag = FALSE ;

	// �ݒ肪�ύX���ꂽ���ǂ����̃t���O��|��
	GSYS.ChangeSettingFlag = FALSE ;

	// ���C�g���g�����ǂ����ŏ����𕪊�
	if( ( ParamFlag & DX_D3D11_DRAWPREP_LIGHTING ) != 0 && GSYS.Light.ProcessDisable == FALSE )
	{
		// ���C�g�̐ݒ肪�ύX����Ă�����ύX��K������
		if( GSYS.Light.HardwareChange )
		{
			Graphics_Light_RefreshState() ;
		}
		else
		{
			// ���C�g���L���Ȃ̂Ɉꎞ�I�ɖ����ɂȂ��Ă�����L���ɂ���
			if( GD3D11.Device.State.Lighting == 0 )
			{
				Graphics_D3D11_DeviceState_SetLighting( TRUE ) ;
			}
		}
	}
	else
	{
		// ���C�g���L���ɂȂ��Ă����疳���ɂ���
		if( GD3D11.Device.State.Lighting == 1 )
		{
			Graphics_D3D11_DeviceState_SetLighting( FALSE ) ;
		}
	}

	// �}�e���A���̐ݒ肪�O������ύX����Ă����猳�ɖ߂�
	if( GSYS.Light.ChangeMaterial )
	{
		GSYS.Light.ChangeMaterial = 0 ;
		Graphics_D3D11_DeviceState_SetMaterial( &GSYS.Light.Material ) ;
	}

	// ���_�J���[���}�e���A���̃J���[�Ƃ��Ďg�p���邩�ǂ������Z�b�g����
	if( ( GSYS.Light.MaterialNotUseVertexDiffuseColor ? FALSE : TRUE ) != GD3D11.Device.State.MaterialUseVertexDiffuseColor )
	{
		Graphics_D3D11_DeviceState_SetUseVertexDiffuseColor( GSYS.Light.MaterialNotUseVertexDiffuseColor ? FALSE : TRUE ) ;
	}

	if( ( GSYS.Light.MaterialNotUseVertexSpecularColor ? FALSE : TRUE ) != GD3D11.Device.State.MaterialUseVertexSpecularColor )
	{
		Graphics_D3D11_DeviceState_SetUseVertexSpecularColor( GSYS.Light.MaterialNotUseVertexSpecularColor ? FALSE : TRUE ) ;
	}

	// �X�y�L�������g�p���邩�ǂ����̐ݒ���s��
	Specular = ( GSYS.DrawSetting.NotUseSpecular == FALSE && ( ParamFlag & DX_D3D11_DRAWPREP_SPECULAR ) != 0 ) ? TRUE : FALSE ;
	if( Specular != GD3D11.Device.State.UseSpecular )
	{
		Graphics_D3D11_DeviceState_SetSpecularEnable( Specular ) ;
	}

	// �ˉe�s��̐ݒ���s��
//	Graphics_DrawSetting_SetUse2DProjectionMatrix( ( ParamFlag & DX_D3D11_DRAWPREP_3D ) == 0 ? TRUE : FALSE ) ;

	// �y�o�b�t�@�̐ݒ���s��
	{
		BOOL                    DepthEnable ;
		int                     WriteDepthBuffer ;
		D_D3D11_COMPARISON_FUNC DepthFunc ;
		int						DepthBias ;

		if( ParamFlag & DX_D3D11_DRAWPREP_3D )
		{
			DepthEnable      = GSYS.DrawSetting.EnableZBufferFlag3D ;
			WriteDepthBuffer = GSYS.DrawSetting.WriteZBufferFlag3D ;
			DepthFunc        = ( D_D3D11_COMPARISON_FUNC )GSYS.DrawSetting.ZBufferCmpType3D ;
			DepthBias        = GSYS.DrawSetting.ZBias3D ;
		}
		else
		{
			DepthEnable      = GSYS.DrawSetting.EnableZBufferFlag2D ;
			WriteDepthBuffer = GSYS.DrawSetting.WriteZBufferFlag2D ;
			DepthFunc        = ( D_D3D11_COMPARISON_FUNC )GSYS.DrawSetting.ZBufferCmpType2D ;
			DepthBias        = GSYS.DrawSetting.ZBias2D ;
		}

		if( DepthEnable )
		{
			if( GD3D11.Device.State.DepthStencilDesc.DepthEnable != DepthEnable )
			{
				Graphics_D3D11_DeviceState_SetDepthEnable( DepthEnable ) ;
			}

			if( ( GD3D11.Device.State.DepthStencilDesc.DepthWriteMask == D_D3D11_DEPTH_WRITE_MASK_ALL ? TRUE : FALSE ) != WriteDepthBuffer  )
			{
				Graphics_D3D11_DeviceState_SetDepthWriteEnable( WriteDepthBuffer  ) ;
			}

			if( GD3D11.Device.State.DepthStencilDesc.DepthFunc != DepthFunc )
			{
				Graphics_D3D11_DeviceState_SetDepthFunc( DepthFunc ) ;
			}

			if( GD3D11.Device.State.RasterizerDesc.DepthBias != DepthBias )
			{
				Graphics_D3D11_DeviceState_SetDepthBias( DepthBias ) ;
			}
		}
		else
		{
			if( GD3D11.Device.State.DepthStencilDesc.DepthEnable )
			{
				Graphics_D3D11_DeviceState_SetDepthEnable( FALSE ) ;
			}

			if( GD3D11.Device.State.DepthStencilDesc.DepthWriteMask != D_D3D11_DEPTH_WRITE_MASK_ZERO  )
			{
				Graphics_D3D11_DeviceState_SetDepthWriteEnable( FALSE ) ;
			}
		}
	}

	// �t�B�����[�h�̃Z�b�g
	if( GD3D11.Device.State.RasterizerDesc.FillMode != ( D_D3D11_FILL_MODE )GSYS.DrawSetting.FillMode )
	{
		Graphics_D3D11_DeviceState_SetFillMode( ( D_D3D11_FILL_MODE )GSYS.DrawSetting.FillMode ) ;
	}

	// �t�H�O�̐ݒ���s��
	if( ParamFlag & DX_D3D11_DRAWPREP_FOG )
	{
		if( GD3D11.Device.State.FogEnable != GSYS.DrawSetting.FogEnable )
		{
			Graphics_D3D11_DeviceState_SetFogEnable( GSYS.DrawSetting.FogEnable ) ;
		}
	}
	else
	{
		if( GD3D11.Device.State.FogEnable )
		{
			Graphics_D3D11_DeviceState_SetFogEnable( FALSE ) ;
		}
	}

	// �u�����f�B���O�֌W�̃Z�b�e�B���O���s���ꍇ�̂ݎ��s����
	if( ( ParamFlag & DX_D3D11_DRAWPREP_NOBLENDSETTING ) == 0 )
	{
		// �t���O�̏�����
		AlphaTest    = FALSE ;
		AlphaChannel = FALSE ;

		// �e�N�X�`���[���g�p���邩�ǂ����ŏ����𕪊�
		// �g�p���Ȃ��ꍇ�͏����l�̂܂�
		if( ParamFlag & DX_D3D11_DRAWPREP_TEXTURE )
		{
			// ���ߐF�������s��Ȃ��ꍇ�̓J���[�L�[�����e�X�g�����`�����l�����g�p���Ȃ��̂ŏ����l�̂܂�
			// ���ߐF�������s���ꍇ�̂ݏ���������
			if( ParamFlag & DX_D3D11_DRAWPREP_TRANS )
			{
				int TexAlphaTestFlag = ( ParamFlag & DX_D3D11_DRAWPREP_TEXALPHATEST ) != 0 ? 1 : 0 ;
				int TexAlphaChFlag   = ( ParamFlag & DX_D3D11_DRAWPREP_TEXALPHACH   ) != 0 ? 1 : 0 ;

				// �e�N�X�`���Ƀ��r�b�g�����邩�ǂ����ŏ����𕪊�
				if( TexAlphaTestFlag || TexAlphaChFlag )
				{
					// ���e�X�g�ł����_���W�̃f�[�^�^�����������_�^�ŁA���e�N�X�`���t�B���^�����O���[�h��
					// ���`��Ԃł��邩�A�u�����h���[�h�� DX_BLENDMODE_NOBLEND �ȊO�������烿�`�����l���Ƃ��Ĉ���
					if( ( GSYS.DrawSetting.BlendMode != DX_BLENDMODE_NOBLEND && GSYS.DrawSetting.BlendMode != DX_BLENDMODE_DESTCOLOR ) ||
						( ( ParamFlag & DX_D3D11_DRAWPREP_VECTORINT ) == 0   && GSYS.DrawSetting.DrawMode  == DX_DRAWMODE_BILINEAR   ) )
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

		// �������ꂽ�p�����[�^���Z�b�g����
		if( GD3D11.Device.DrawSetting.IgnoreGraphColorFlag  != GSYS.DrawSetting.IgnoreGraphColorFlag )
		{
			Graphics_D3D11_DrawSetting_SetIgnoreDrawGraphColor( GSYS.DrawSetting.IgnoreGraphColorFlag ) ;
		}

		if( GD3D11.Device.DrawSetting.BlendMode             != GSYS.DrawSetting.BlendMode ||
			GD3D11.Device.DrawSetting.AlphaTestValidFlag    != AlphaTest ||
			GD3D11.Device.DrawSetting.AlphaChannelValidFlag != AlphaChannel               )
		{
			Graphics_D3D11_DrawSetting_SetDrawBlendMode( GSYS.DrawSetting.BlendMode, AlphaTest, AlphaChannel ) ;
		}

		if( GD3D11.Device.DrawSetting.AlphaTestMode         != GSYS.DrawSetting.AlphaTestMode ||
			GD3D11.Device.DrawSetting.AlphaTestParam        != GSYS.DrawSetting.AlphaTestParam )
		{
			Graphics_D3D11_DrawSetting_SetDrawAlphaTest( GSYS.DrawSetting.AlphaTestMode, GSYS.DrawSetting.AlphaTestParam ) ;
		}

		if( GD3D11.Device.DrawSetting.ChangeBlendParamFlag ||
			GD3D11.Device.DrawSetting.ChangeTextureFlag ||
			( GSYS.HardInfo.UseShader &&
			  GD3D11.Device.State.SetNormalPixelShader == FALSE ) )
		{
			Graphics_D3D11_DeviceState_NormalDrawSetup() ;
		}
	}

	if( GD3D11.Device.State.DrawMode != GSYS.DrawSetting.DrawMode )
	{
		Graphics_D3D11_DeviceState_SetDrawMode( GSYS.DrawSetting.DrawMode ) ;
	}

	if( GD3D11.Device.State.MaxAnisotropy != GSYS.DrawSetting.MaxAnisotropy )
	{
		Graphics_D3D11_DeviceState_SetMaxAnisotropy( GSYS.DrawSetting.MaxAnisotropy ) ;
	}

	if( ParamFlag & DX_D3D11_DRAWPREP_CULLING )
	{
		int SetCullMode ;

		switch( GD3D11.Device.State.RasterizerDesc.CullMode )
		{
		case D_D3D11_CULL_NONE :
		default :
			SetCullMode = 0 ;
			break ;

		case D_D3D11_CULL_BACK :
			SetCullMode = 1 ;
			break ;

		case D_D3D11_CULL_FRONT :
			SetCullMode = 2 ;
			break ;
		}

		if( SetCullMode != GSYS.DrawSetting.CullMode )
		{
			Graphics_D3D11_DeviceState_SetCullMode( GSYS.DrawSetting.CullMode ) ;
		}
	}
	else
	{
		if( GD3D11.Device.State.RasterizerDesc.CullMode != D_D3D11_CULL_NONE )
		{
			Graphics_D3D11_DeviceState_SetCullMode( DX_CULLING_NONE ) ;
		}
	}

	if( GSYS.DrawSetting.MatchHardwareTextureAddressTransformMatrix == FALSE )
	{
		Graphics_D3D11_DeviceState_SetTextureAddressTransformMatrix( GSYS.DrawSetting.TextureAddressTransformUse, &GSYS.DrawSetting.TextureAddressTransformMatrix ) ;
		GSYS.DrawSetting.MatchHardwareTextureAddressTransformMatrix = TRUE ;
		GD3D11.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;
	}

	if( ParamFlag & DX_D3D11_DRAWPREP_TEXADDRESS )
	{
		for( i = 0; i < USE_TEXTURESTAGE_NUM; i++ )
		{
			if( GSYS.DrawSetting.TexAddressModeU[ i ] == GSYS.DrawSetting.TexAddressModeV[ i ] )
			{
				if( GD3D11.Device.State.SamplerDesc[ i ].AddressU != ( D_D3D11_TEXTURE_ADDRESS_MODE )GSYS.DrawSetting.TexAddressModeU[ i ] ||
					GD3D11.Device.State.SamplerDesc[ i ].AddressV != ( D_D3D11_TEXTURE_ADDRESS_MODE )GSYS.DrawSetting.TexAddressModeV[ i ] )
				{
					Graphics_D3D11_DeviceState_SetTextureAddress( GSYS.DrawSetting.TexAddressModeU[ i ], i ) ;
				}
			}
			else
			{
				if( GD3D11.Device.State.SamplerDesc[ i ].AddressU != ( D_D3D11_TEXTURE_ADDRESS_MODE )GSYS.DrawSetting.TexAddressModeU[ i ] )
				{
					Graphics_D3D11_DeviceState_SetTextureAddressU( GSYS.DrawSetting.TexAddressModeU[ i ], i ) ;
				}

				if( GD3D11.Device.State.SamplerDesc[ i ].AddressV != ( D_D3D11_TEXTURE_ADDRESS_MODE )GSYS.DrawSetting.TexAddressModeV[ i ] )
				{
					Graphics_D3D11_DeviceState_SetTextureAddressV( GSYS.DrawSetting.TexAddressModeV[ i ], i ) ;
				}
			}
		}
	}
	else
	{
		for( i = 0; i < 4; i++ )
		{
			if( GD3D11.Device.State.SamplerDesc[ i ].AddressU != D_D3D11_TEXTURE_ADDRESS_CLAMP ||
				GD3D11.Device.State.SamplerDesc[ i ].AddressV != D_D3D11_TEXTURE_ADDRESS_CLAMP )
			{
				Graphics_D3D11_DeviceState_SetTextureAddress( D_D3D11_TEXTURE_ADDRESS_CLAMP, i ) ;
			}
		}
	}

	// �p�����[�^��ۑ�
	GD3D11.Device.DrawSetting.DrawPrepParamFlag = ParamFlag ;
}


// �`��R�}���h���J�n����
extern	void	Graphics_D3D11_RenderBegin( void )
{
	if( D3D11Device_IsValid() == 0 || GD3D11.Device.DrawInfo.BeginSceneFlag == TRUE ) return ;

//	D3D11DeviceContext_Begin() ;
	GD3D11.Device.DrawInfo.BeginSceneFlag = TRUE ;
	GD3D11.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;
}

// �`��R�}���h���I������
extern	void	Graphics_D3D11_RenderEnd( void )
{
	if( D3D11Device_IsValid() == 0 || GD3D11.Device.DrawInfo.BeginSceneFlag == FALSE ) return ;

//	D3D11DeviceContext_End() ;
	GD3D11.Device.DrawInfo.BeginSceneFlag = FALSE ;
}




























// Direct3D11 ���g�����`��֌W

// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawBillboard3D
extern	int		Graphics_D3D11_DrawBillboard3D( VECTOR Pos, float cx, float cy, float Size, float Angle, IMAGEDATA *Image, IMAGEDATA *BlendImage, int TransFlag, int TurnFlag, int DrawFlag, RECT *DrawArea )
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

	if( GAPIWin.D3D11DeviceObject == NULL )
	{
		return -1 ;
	}

	// �`�揀��
	if( DrawFlag )
	{
		Flag = TransFlag | DX_D3D11_DRAWPREP_3D | DX_D3D11_DRAWPREP_FOG | DX_D3D11_DRAWPREP_TEXADDRESS ;
		DX_D3D11_DRAWPREP_TEX( Orig, Image->Hard.Draw[ 0 ].Tex->PF->D3D11.Texture, Image->Hard.Draw[ 0 ].Tex->PF->D3D11.TextureSRV, Flag )
	}
	else
	{
		dleft   = -100000000.0f ;
		dright  =  100000000.0f ;
		dtop    = -100000000.0f ;
		dbottom =  100000000.0f ;
	}

	// ���_�f�[�^���擾
	DiffuseColor = GD3D11.Device.DrawInfo.DiffuseColor ;

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
			Graphics_D3D11_DrawSetting_SetTexture( DrawTex->Tex->PF->D3D11.Texture, DrawTex->Tex->PF->D3D11.TextureSRV ) ;

			if( BlendDrawTex != NULL )
			{
				Graphics_D3D11_DrawSetting_SetBlendTexture( BlendDrawTex->Tex->PF->D3D11.Texture, BlendDrawTex->Tex->PF->D3D11.TextureSRV, BlendDrawTex->Tex->TexWidth, BlendDrawTex->Tex->TexHeight );
			}

			if( GD3D11.Device.DrawSetting.ChangeTextureFlag )
			{
				Graphics_D3D11_DeviceState_NormalDrawSetup() ;
			}
		}

		TexVert = DrawTex->Vertex ;
		if( BlendDrawTex != NULL )
		{
			BlendTexVert = BlendDrawTex->Vertex ;
		}

		// �u�����h�O���t�B�b�N���g�p���Ă��Ȃ��āA���`�悷��ꍇ�͍����ȏ������g�p����
		if( GD3D11.Device.DrawInfo.VertexType != VERTEXTYPE_BLENDTEX && DrawFlag == TRUE )
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

				DrawVert3D[ 4 ].pos.x = SrcVec[ 2 ].x * GSYS.DrawSetting.BillboardMatrixF.m[0][0] + SrcVec[ 2 ].y * GSYS.DrawSetting.BillboardMatrixF.m[1][0] + Pos.x ;
				DrawVert3D[ 4 ].pos.y = SrcVec[ 2 ].x * GSYS.DrawSetting.BillboardMatrixF.m[0][1] + SrcVec[ 2 ].y * GSYS.DrawSetting.BillboardMatrixF.m[1][1] + Pos.y ;
				DrawVert3D[ 4 ].pos.z = SrcVec[ 2 ].x * GSYS.DrawSetting.BillboardMatrixF.m[0][2] + SrcVec[ 2 ].y * GSYS.DrawSetting.BillboardMatrixF.m[1][2] + Pos.z ;

				DrawVert3D[ 5 ].pos.x = SrcVec[ 1 ].x * GSYS.DrawSetting.BillboardMatrixF.m[0][0] + SrcVec[ 1 ].y * GSYS.DrawSetting.BillboardMatrixF.m[1][0] + Pos.x ;
				DrawVert3D[ 5 ].pos.y = SrcVec[ 1 ].x * GSYS.DrawSetting.BillboardMatrixF.m[0][1] + SrcVec[ 1 ].y * GSYS.DrawSetting.BillboardMatrixF.m[1][1] + Pos.y ;
				DrawVert3D[ 5 ].pos.z = SrcVec[ 1 ].x * GSYS.DrawSetting.BillboardMatrixF.m[0][2] + SrcVec[ 1 ].y * GSYS.DrawSetting.BillboardMatrixF.m[1][2] + Pos.z ;
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

				DrawVert3D[ 4 ].pos.x = TempVecX[ 0 ].x + TempVecY[ 1 ].x ;
				DrawVert3D[ 4 ].pos.y = TempVecX[ 0 ].y + TempVecY[ 1 ].y ;
				DrawVert3D[ 4 ].pos.z = TempVecX[ 0 ].z + TempVecY[ 1 ].z ;

				DrawVert3D[ 5 ].pos.x = TempVecX[ 1 ].x + TempVecY[ 0 ].x ;
				DrawVert3D[ 5 ].pos.y = TempVecX[ 1 ].y + TempVecY[ 0 ].y ;
				DrawVert3D[ 5 ].pos.z = TempVecX[ 1 ].z + TempVecY[ 0 ].z ;
			}

			*( ( DWORD * )&DrawVert3D[0].b ) = DiffuseColor ;
			*( ( DWORD * )&DrawVert3D[1].b ) = DiffuseColor ;
			*( ( DWORD * )&DrawVert3D[2].b ) = DiffuseColor ;
			*( ( DWORD * )&DrawVert3D[3].b ) = DiffuseColor ;
			*( ( DWORD * )&DrawVert3D[4].b ) = DiffuseColor ; 
			*( ( DWORD * )&DrawVert3D[5].b ) = DiffuseColor ;

			if( TurnFlag )
			{
				DrawVert3D[0].u = TexVert[1].u ;
				DrawVert3D[1].u = TexVert[0].u ;
				DrawVert3D[2].u = TexVert[1].u ;
				DrawVert3D[3].u = TexVert[0].u ;
				DrawVert3D[4].u = TexVert[1].u ;
				DrawVert3D[5].u = TexVert[0].u ;
			}
			else
			{
				DrawVert3D[0].u = TexVert[0].u ;
				DrawVert3D[1].u = TexVert[1].u ;
				DrawVert3D[2].u = TexVert[0].u ;
				DrawVert3D[3].u = TexVert[1].u ;
				DrawVert3D[4].u = TexVert[0].u ;
				DrawVert3D[5].u = TexVert[1].u ;
			}
			DrawVert3D[0].v = TexVert[0].v ;
			DrawVert3D[1].v = TexVert[0].v ;
			DrawVert3D[2].v = TexVert[2].v ;
			DrawVert3D[3].v = TexVert[2].v ;
			DrawVert3D[4].v = TexVert[2].v ;
			DrawVert3D[5].v = TexVert[0].v ;

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
			switch( GD3D11.Device.DrawInfo.VertexType )
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
					u[ 0 ] = ( TexVert[0].x - BlendTexVert[0].x + GSYS.DrawSetting.BlendGraphX ) * GD3D11.Device.DrawSetting.InvBlendTextureWidth ;
					u[ 1 ] = ( TexVert[1].x - BlendTexVert[0].x + GSYS.DrawSetting.BlendGraphX ) * GD3D11.Device.DrawSetting.InvBlendTextureWidth ;
					v[ 0 ] = ( TexVert[0].y - BlendTexVert[0].y + GSYS.DrawSetting.BlendGraphY ) * GD3D11.Device.DrawSetting.InvBlendTextureHeight ;
					v[ 1 ] = ( TexVert[2].y - BlendTexVert[0].y + GSYS.DrawSetting.BlendGraphY ) * GD3D11.Device.DrawSetting.InvBlendTextureHeight ;
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
					DrawVertB[0].u1 = ( DrawPos[ 0 ].x - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D11.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[0].v1 = ( DrawPos[ 0 ].y - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D11.Device.DrawSetting.InvBlendTextureHeight ;
					DrawVertB[1].u1 = ( DrawPos[ 1 ].x - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D11.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[1].v1 = ( DrawPos[ 1 ].y - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D11.Device.DrawSetting.InvBlendTextureHeight ;
					DrawVertB[2].u1 = ( DrawPos[ 2 ].x - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D11.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[2].v1 = ( DrawPos[ 2 ].y - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D11.Device.DrawSetting.InvBlendTextureHeight ;
					DrawVertB[3].u1 = ( DrawPos[ 3 ].x - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D11.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[3].v1 = ( DrawPos[ 3 ].y - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D11.Device.DrawSetting.InvBlendTextureHeight ;
					DrawVertB[4].u1 = ( DrawPos[ 2 ].x - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D11.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[4].v1 = ( DrawPos[ 2 ].y - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D11.Device.DrawSetting.InvBlendTextureHeight ;
					DrawVertB[5].u1 = ( DrawPos[ 1 ].x - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D11.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[5].v1 = ( DrawPos[ 1 ].y - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D11.Device.DrawSetting.InvBlendTextureHeight ;
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
extern	int		Graphics_D3D11_DrawModiBillboard3D( VECTOR Pos, float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, IMAGEDATA *Image, IMAGEDATA *BlendImage, int TransFlag, int DrawFlag, RECT *DrawArea )
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
	float dleft = 0.0f, dright = 0.0f, dtop = 0.0f, dbottom = 0.0f;
	float z ;
	float rhw ;
	float u[ 2 ] ;
	float v[ 2 ] ;

	Orig = Image->Orig ;

	if( GAPIWin.D3D11DeviceObject == NULL ) return -1 ;

	// �`�揀��
	if( DrawFlag )
	{
		Flag = TransFlag | DX_D3D11_DRAWPREP_3D | DX_D3D11_DRAWPREP_FOG | DX_D3D11_DRAWPREP_TEXADDRESS ;
		DX_D3D11_DRAWPREP_TEX( Orig, Image->Hard.Draw[ 0 ].Tex->PF->D3D11.Texture, Image->Hard.Draw[ 0 ].Tex->PF->D3D11.TextureSRV, Flag )
	}
	else
	{
		dleft   = -100000000.0f ;
		dright  =  100000000.0f ;
		dtop    = -100000000.0f ;
		dbottom =  100000000.0f ;
	}

	// ���_�f�[�^���擾
	DiffuseColor = GD3D11.Device.DrawInfo.DiffuseColor ;

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
			Graphics_D3D11_DrawSetting_SetTexture( DrawTex->Tex->PF->D3D11.Texture, DrawTex->Tex->PF->D3D11.TextureSRV ) ;

			if( BlendDrawTex != NULL )
			{
				Graphics_D3D11_DrawSetting_SetBlendTexture( BlendDrawTex->Tex->PF->D3D11.Texture, BlendDrawTex->Tex->PF->D3D11.TextureSRV, BlendDrawTex->Tex->TexWidth, BlendDrawTex->Tex->TexHeight );
			}

			if( GD3D11.Device.DrawSetting.ChangeTextureFlag )
			{
				Graphics_D3D11_DeviceState_NormalDrawSetup() ;
			}
		}

		TexVert = DrawTex->Vertex ;
		if( BlendDrawTex != NULL )
		{
			BlendTexVert = BlendDrawTex->Vertex ;
		}

		// �u�����h�O���t�B�b�N���g�p���Ă��Ȃ��āA���`�悷��ꍇ�͍����ȏ������g�p����
		if( GD3D11.Device.DrawInfo.VertexType != VERTEXTYPE_BLENDTEX && DrawFlag == TRUE )
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

			DrawVert3D[ 4 ].pos.x = x4 * GSYS.DrawSetting.BillboardMatrixF.m[0][0] + y4 * GSYS.DrawSetting.BillboardMatrixF.m[1][0] + Pos.x ;
			DrawVert3D[ 4 ].pos.y = x4 * GSYS.DrawSetting.BillboardMatrixF.m[0][1] + y4 * GSYS.DrawSetting.BillboardMatrixF.m[1][1] + Pos.y ;
			DrawVert3D[ 4 ].pos.z = x4 * GSYS.DrawSetting.BillboardMatrixF.m[0][2] + y4 * GSYS.DrawSetting.BillboardMatrixF.m[1][2] + Pos.z ;

			DrawVert3D[ 5 ].pos.x = x2 * GSYS.DrawSetting.BillboardMatrixF.m[0][0] + y2 * GSYS.DrawSetting.BillboardMatrixF.m[1][0] + Pos.x ;
			DrawVert3D[ 5 ].pos.y = x2 * GSYS.DrawSetting.BillboardMatrixF.m[0][1] + y2 * GSYS.DrawSetting.BillboardMatrixF.m[1][1] + Pos.y ;
			DrawVert3D[ 5 ].pos.z = x2 * GSYS.DrawSetting.BillboardMatrixF.m[0][2] + y2 * GSYS.DrawSetting.BillboardMatrixF.m[1][2] + Pos.z ;

			*( ( DWORD * )&DrawVert3D[0].b ) = DiffuseColor ;
			*( ( DWORD * )&DrawVert3D[1].b ) = DiffuseColor ;
			*( ( DWORD * )&DrawVert3D[2].b ) = DiffuseColor ;
			*( ( DWORD * )&DrawVert3D[3].b ) = DiffuseColor ;
			*( ( DWORD * )&DrawVert3D[4].b ) = DiffuseColor ;
			*( ( DWORD * )&DrawVert3D[5].b ) = DiffuseColor ;

			DrawVert3D[0].u = TexVert[0].u ;
			DrawVert3D[0].v = TexVert[0].v ;
			DrawVert3D[1].u = TexVert[1].u ;
			DrawVert3D[1].v = TexVert[0].v ;
			DrawVert3D[2].u = TexVert[0].u ;
			DrawVert3D[2].v = TexVert[2].v ;
			DrawVert3D[3].u = TexVert[1].u ;
			DrawVert3D[3].v = TexVert[2].v ;
			DrawVert3D[4].u = TexVert[0].u ;
			DrawVert3D[4].v = TexVert[2].v ;
			DrawVert3D[5].u = TexVert[1].u ;
			DrawVert3D[5].v = TexVert[0].v ;

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
			switch( GD3D11.Device.DrawInfo.VertexType )
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
					u[ 0 ] = ( TexVert[0].x - BlendTexVert[0].x + GSYS.DrawSetting.BlendGraphX ) * GD3D11.Device.DrawSetting.InvBlendTextureWidth ;
					u[ 1 ] = ( TexVert[1].x - BlendTexVert[0].x + GSYS.DrawSetting.BlendGraphX ) * GD3D11.Device.DrawSetting.InvBlendTextureWidth ;
					v[ 0 ] = ( TexVert[0].y - BlendTexVert[0].y + GSYS.DrawSetting.BlendGraphY ) * GD3D11.Device.DrawSetting.InvBlendTextureHeight ;
					v[ 1 ] = ( TexVert[2].y - BlendTexVert[0].y + GSYS.DrawSetting.BlendGraphY ) * GD3D11.Device.DrawSetting.InvBlendTextureHeight ;
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
					DrawVertB[0].u1 = ( DrawPos[ 0 ].x - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D11.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[0].v1 = ( DrawPos[ 0 ].y - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D11.Device.DrawSetting.InvBlendTextureHeight ;
					DrawVertB[1].u1 = ( DrawPos[ 1 ].x - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D11.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[1].v1 = ( DrawPos[ 1 ].y - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D11.Device.DrawSetting.InvBlendTextureHeight ;
					DrawVertB[2].u1 = ( DrawPos[ 2 ].x - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D11.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[2].v1 = ( DrawPos[ 2 ].y - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D11.Device.DrawSetting.InvBlendTextureHeight ;
					DrawVertB[3].u1 = ( DrawPos[ 3 ].x - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D11.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[3].v1 = ( DrawPos[ 3 ].y - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D11.Device.DrawSetting.InvBlendTextureHeight ;
					DrawVertB[4].u1 = ( DrawPos[ 2 ].x - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D11.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[4].v1 = ( DrawPos[ 2 ].y - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D11.Device.DrawSetting.InvBlendTextureHeight ;
					DrawVertB[5].u1 = ( DrawPos[ 1 ].x - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D11.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[5].v1 = ( DrawPos[ 1 ].y - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D11.Device.DrawSetting.InvBlendTextureHeight ;
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
extern	int		Graphics_D3D11_DrawGraph( int x, int y, float xf, float yf, IMAGEDATA *Image, IMAGEDATA *BlendImage, int TransFlag, int IntFlag )
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

	if( GAPIWin.D3D11DeviceObject == NULL )
	{
		return -1 ;
	}

	// float���W�␳
//	xf -= 0.5F ;
//	yf -= 0.5F ;

	// �`�揀��
	Flag = TransFlag | ( IntFlag << 1 ) ;
	DX_D3D11_DRAWPREP_TEX( Orig, Image->Hard.Draw[ 0 ].Tex->PF->D3D11.Texture, Image->Hard.Draw[ 0 ].Tex->PF->D3D11.TextureSRV, Flag )

	// ���_�f�[�^���擾
	DiffuseColor = GD3D11.Device.DrawInfo.DiffuseColor ;

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
//	if( x + Image->Width  < GSYS.DrawSetting.DrawArea.right  && x > GSYS.DrawSetting.DrawArea.left &&
//		y + Image->Height < GSYS.DrawSetting.DrawArea.bottom && y > GSYS.DrawSetting.DrawArea.right )
	{
		// �͂ݏo�Ă��Ȃ��ꍇ

		// ���_�^�C�v�ɂ���ď����𕪊�
		if( GD3D11.Device.DrawInfo.VertexType == VERTEXTYPE_BLENDTEX )
		{
			// �u�����h�e�N�X�`�����g�p����

			// �e�N�X�`���[�̐������J��Ԃ�
			for( i = 0 ; i < DrawTexNum ; i ++, DrawTex ++ )
			{
				// �`�悷��e�N�X�`���[�̃Z�b�g
				Graphics_D3D11_DrawSetting_SetTexture( DrawTex->Tex->PF->D3D11.Texture, DrawTex->Tex->PF->D3D11.TextureSRV ) ;
				Graphics_D3D11_DrawSetting_SetBlendTexture( BlendDrawTex->Tex->PF->D3D11.Texture, BlendDrawTex->Tex->PF->D3D11.TextureSRV, BlendDrawTex->Tex->TexWidth, BlendDrawTex->Tex->TexHeight );

				if( GD3D11.Device.DrawSetting.ChangeTextureFlag )
				{
					Graphics_D3D11_DeviceState_NormalDrawSetup() ;
				}

				// ���_�o�b�t�@�̎擾
				GETVERTEX_QUAD( DrawVertB )
				
				TexVert = DrawTex->Vertex ;
				BlendTexVert = BlendDrawTex->Vertex ;

				DrawVertB[0].color = ( DWORD )DiffuseColor ;
				DrawVertB[1].color = ( DWORD )DiffuseColor ;
				DrawVertB[2].color = ( DWORD )DiffuseColor ;
				DrawVertB[3].color = ( DWORD )DiffuseColor ;
				DrawVertB[4].color = ( DWORD )DiffuseColor ;
				DrawVertB[5].color = ( DWORD )DiffuseColor ;

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

				u[ 0 ] *= GD3D11.Device.DrawSetting.InvBlendTextureWidth ;
				u[ 1 ] *= GD3D11.Device.DrawSetting.InvBlendTextureWidth ;
				v[ 0 ] *= GD3D11.Device.DrawSetting.InvBlendTextureHeight ;
				v[ 1 ] *= GD3D11.Device.DrawSetting.InvBlendTextureHeight ;

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

				DrawVertB[0].rhw = 1.0f ;
				DrawVertB[1].rhw = 1.0f ;
				DrawVertB[2].rhw = 1.0f ;
				DrawVertB[3].rhw = 1.0f ;
				DrawVertB[4].rhw = 1.0f ;
				DrawVertB[5].rhw = 1.0f ;

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
				DrawVert[0].color	= DiffuseColor ;
				DrawVert[1].color	= DiffuseColor ;
				DrawVert[2].color	= DiffuseColor ;
				DrawVert[3].color	= DiffuseColor ;
				DrawVert[4].color	= DiffuseColor ;
				DrawVert[5].color	= DiffuseColor ;

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

				DrawVert[0].rhw = 1.0f ;
				DrawVert[1].rhw = 1.0f ;
				DrawVert[2].rhw = 1.0f ;
				DrawVert[3].rhw = 1.0f ;
				DrawVert[4].rhw = 1.0f ;
				DrawVert[5].rhw = 1.0f ;

				// �e�N�X�`���[��`�悷��
				ADD4VERTEX_TEX

				i ++ ;
				if( i >= DrawTexNum ) break ;

				// �`�悷��e�N�X�`���[�̃Z�b�g
				DrawTex ++ ;
				Graphics_D3D11_DrawSetting_SetTexture( DrawTex->Tex->PF->D3D11.Texture, DrawTex->Tex->PF->D3D11.TextureSRV ) ;
				Graphics_D3D11_DeviceState_NormalDrawSetup() ;
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
		switch( GD3D11.Device.DrawInfo.VertexType )
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
				Graphics_D3D11_DrawSetting_SetTexture( DrawTex->Tex->PF->D3D11.Texture, DrawTex->Tex->PF->D3D11.TextureSRV ) ;
				Graphics_D3D11_DrawSetting_SetBlendTexture( BlendDrawTex->Tex->PF->D3D11.Texture, BlendDrawTex->Tex->PF->D3D11.TextureSRV, BlendDrawTex->Tex->TexWidth, BlendDrawTex->Tex->TexHeight );

				if( GD3D11.Device.DrawSetting.ChangeTextureFlag )
				{
					Graphics_D3D11_DeviceState_NormalDrawSetup() ;
				}

				// ���_�o�b�t�@�̎擾
				GETVERTEX_QUAD( DrawVertB )
				
				// ���_�f�[�^������������
				TexVert = DrawTex->Vertex ;
				BlendTexVert = BlendDrawTex->Vertex ;

				Vx  = 1.0F / ( float )DrawTex->Tex->TexWidth  ;
				Vy  = 1.0F / ( float )DrawTex->Tex->TexHeight ;
				VBx = GD3D11.Device.DrawSetting.InvBlendTextureWidth  ;
				VBy = GD3D11.Device.DrawSetting.InvBlendTextureHeight ;

				DrawVertB[0].color = ( DWORD )DiffuseColor ;
				DrawVertB[1].color = ( DWORD )DiffuseColor ;
				DrawVertB[2].color = ( DWORD )DiffuseColor ;
				DrawVertB[3].color = ( DWORD )DiffuseColor ;
				DrawVertB[4].color = ( DWORD )DiffuseColor ;
				DrawVertB[5].color = ( DWORD )DiffuseColor ;

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
					u1[2] = u1[0] = ( TexVert[0].x - BlendTexVert[0].x + GSYS.DrawSetting.BlendGraphX ) * GD3D11.Device.DrawSetting.InvBlendTextureWidth ;
					u1[3] = u1[1] = ( TexVert[1].x - BlendTexVert[0].x + GSYS.DrawSetting.BlendGraphX ) * GD3D11.Device.DrawSetting.InvBlendTextureWidth ;
					v1[1] = v1[0] = ( TexVert[0].y - BlendTexVert[0].y + GSYS.DrawSetting.BlendGraphY ) * GD3D11.Device.DrawSetting.InvBlendTextureHeight ;
					v1[3] = v1[2] = ( TexVert[2].y - BlendTexVert[0].y + GSYS.DrawSetting.BlendGraphY ) * GD3D11.Device.DrawSetting.InvBlendTextureHeight ;
					break ;

				case DX_BLENDGRAPH_POSMODE_SCREEN :
					u1[2] = u1[0] = ( ( xf + TexVert[0].x ) - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D11.Device.DrawSetting.InvBlendTextureWidth ;
					u1[3] = u1[1] = ( ( xf + TexVert[1].x ) - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D11.Device.DrawSetting.InvBlendTextureWidth ;
					v1[1] = v1[0] = ( ( yf + TexVert[0].y ) - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D11.Device.DrawSetting.InvBlendTextureHeight ;
					v1[3] = v1[2] = ( ( yf + TexVert[2].y ) - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D11.Device.DrawSetting.InvBlendTextureHeight ;
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

				DrawVertB[0].rhw = 1.0f ;
				DrawVertB[1].rhw = 1.0f ;
				DrawVertB[2].rhw = 1.0f ;
				DrawVertB[3].rhw = 1.0f ;
				DrawVertB[4].rhw = 1.0f ;
				DrawVertB[5].rhw = 1.0f ;

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
				DrawVert[1].color	= DiffuseColor ;
				DrawVert[2].color	= DiffuseColor ;
				DrawVert[3].color	= DiffuseColor ;
				DrawVert[4].color	= DiffuseColor ;
				DrawVert[5].color	= DiffuseColor ;

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

				DrawVert[0].rhw = 1.0f ;
				DrawVert[1].rhw = 1.0f ;
				DrawVert[2].rhw = 1.0f ;
				DrawVert[3].rhw = 1.0f ;
				DrawVert[4].rhw = 1.0f ;
				DrawVert[5].rhw = 1.0f ;

				// �e�N�X�`���[��`�悷��
				ADD4VERTEX_TEX

				i ++ ;
				if( i >= DrawTexNum ) break ;

				// �`�悷��e�N�X�`���[�̃Z�b�g
				DrawTex ++ ;
				Graphics_D3D11_DrawSetting_SetTexture( DrawTex->Tex->PF->D3D11.Texture, DrawTex->Tex->PF->D3D11.TextureSRV ) ;
				Graphics_D3D11_DeviceState_NormalDrawSetup() ;
			}
			break ;
		}
	}

	// �I��
	return 0 ;
}

// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawExtendGraph
extern	int		Graphics_D3D11_DrawExtendGraph( int x1, int y1, int x2, int y2, float x1f, float y1f, float x2f, float y2f, IMAGEDATA *Image, IMAGEDATA *BlendImage, int TransFlag, int IntFlag )
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
	DWORD drawz;
	int Flag ;
	float XBuf[ 2 ] ;
	float YBuf[ 2 ] ;
	float u[ 2 ] ;
	float v[ 2 ] ;

	Orig = Image->Orig ;

	if( GAPIWin.D3D11DeviceObject == NULL )
	{
		return -1 ;
	}

	// �`�攻��
//	if( x2 - x1 == Image->Width && y2 - y1 == Image->Height ) return G_D3D_DrawGraph( x1, y1, GraphData, TransFlag ) ; 	

	// �����Ȃ������ꍇ�̓G���[
	if( x1 == x2 || y1 == y2 ) return -1 ;

	// �`��̏���
	Flag = TransFlag | ( IntFlag ? ( GSYS.DrawSetting.DrawMode == DX_DRAWMODE_BILINEAR ? 0 : DX_D3D11_DRAWPREP_VECTORINT ) : 0 ) ;
	DX_D3D11_DRAWPREP_TEX( Orig, Image->Hard.Draw[ 0 ].Tex->PF->D3D11.Texture, Image->Hard.Draw[ 0 ].Tex->PF->D3D11.TextureSRV, Flag )

	// �f�B�t���[�Y�J���[�̎擾
	DiffuseColor = GD3D11.Device.DrawInfo.DiffuseColor ;

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

//	x1f -= 0.5F ;
//	y1f -= 0.5F ;

	if( x1 > x2 ){ i = x2; x2 = x1; x1 = i; }
	if( y1 > y2 ){ i = y2; y2 = y1; y1 = i; }

	// �`��͈͂���͂ݏo�Ă��邩�ǂ����ŏ����𕪊�
	if( IN_DRAWAREA_CHECK( x1, y1, x2, y2 ) )
	{
		// ���_�^�C�v�ɂ���ď����𕪊�
		switch( GD3D11.Device.DrawInfo.VertexType )
		{
		case VERTEXTYPE_BLENDTEX :	// �u�����h�e�N�X�`�����g�p����
			// �e�N�X�`���[�̐������J��Ԃ�
			for( i = 0 ; i < DrawTexNum ; i ++, DrawTex ++ )
			{
				// �`�悷��e�N�X�`���[�̃Z�b�g
				Graphics_D3D11_DrawSetting_SetTexture( DrawTex->Tex->PF->D3D11.Texture, DrawTex->Tex->PF->D3D11.TextureSRV ) ;
				Graphics_D3D11_DrawSetting_SetBlendTexture( BlendDrawTex->Tex->PF->D3D11.Texture, BlendDrawTex->Tex->PF->D3D11.TextureSRV, BlendDrawTex->Tex->TexWidth, BlendDrawTex->Tex->TexHeight );

				if( GD3D11.Device.DrawSetting.ChangeTextureFlag )
				{
					Graphics_D3D11_DeviceState_NormalDrawSetup() ;
				}

				// ���_�f�[�^������������
				GETVERTEX_QUAD( DrawVertB )
				
				TexVert = DrawTex->Vertex ;
				BlendTexVert = BlendDrawTex->Vertex ;

				DrawVertB[0].color = ( DWORD )DiffuseColor ;
				DrawVertB[1].color = ( DWORD )DiffuseColor ;
				DrawVertB[2].color = ( DWORD )DiffuseColor ;
				DrawVertB[3].color = ( DWORD )DiffuseColor ;
				DrawVertB[4].color = ( DWORD )DiffuseColor ;
				DrawVertB[5].color = ( DWORD )DiffuseColor ;

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

				u[ 0 ] *= GD3D11.Device.DrawSetting.InvBlendTextureWidth ;
				u[ 1 ] *= GD3D11.Device.DrawSetting.InvBlendTextureWidth ;
				v[ 0 ] *= GD3D11.Device.DrawSetting.InvBlendTextureHeight ;
				v[ 1 ] *= GD3D11.Device.DrawSetting.InvBlendTextureHeight ;

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

				DrawVertB[0].rhw = 1.0f ;
				DrawVertB[1].rhw = 1.0f ;
				DrawVertB[2].rhw = 1.0f ;
				DrawVertB[3].rhw = 1.0f ;
				DrawVertB[4].rhw = 1.0f ;
				DrawVertB[5].rhw = 1.0f ;

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
				DrawVert[1].color = ( DWORD )DiffuseColor ;
				DrawVert[2].color = ( DWORD )DiffuseColor ;
				DrawVert[3].color = ( DWORD )DiffuseColor ;
				DrawVert[4].color = ( DWORD )DiffuseColor ;
				DrawVert[5].color = ( DWORD )DiffuseColor ;

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

				DrawVert[0].rhw = 1.0f ;
				DrawVert[1].rhw = 1.0f ;
				DrawVert[2].rhw = 1.0f ;
				DrawVert[3].rhw = 1.0f ;
				DrawVert[4].rhw = 1.0f ;
				DrawVert[5].rhw = 1.0f ;

				// �e�N�X�`���[��`�悷��
				ADD4VERTEX_TEX

				i ++ ;
				if( i >= DrawTexNum ) break ;

				// �`�悷��e�N�X�`���[�̃Z�b�g
				DrawTex ++ ;
				Graphics_D3D11_DrawSetting_SetTexture( DrawTex->Tex->PF->D3D11.Texture, DrawTex->Tex->PF->D3D11.TextureSRV ) ;
				Graphics_D3D11_DeviceState_NormalDrawSetup() ;
			}
			break ;
		}
	}
	else
	{
		// ���S�ɂ͂ݏo�Ă����牽�������I��
		if( OUT_DRAWAREA_CHECK( x1, y1, x2, y2 ) ) return 0 ;

		// �`���`�̕ۑ�
		left	= GSYS.DrawSetting.DrawAreaF.left   ;
		top		= GSYS.DrawSetting.DrawAreaF.top    ;
		right	= GSYS.DrawSetting.DrawAreaF.right  ;
		bottom	= GSYS.DrawSetting.DrawAreaF.bottom ;

		// ���_�^�C�v�ɂ���ď����𕪊�
		switch( GD3D11.Device.DrawInfo.VertexType )
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
				Graphics_D3D11_DrawSetting_SetTexture( DrawTex->Tex->PF->D3D11.Texture, DrawTex->Tex->PF->D3D11.TextureSRV ) ;
				Graphics_D3D11_DrawSetting_SetBlendTexture( BlendDrawTex->Tex->PF->D3D11.Texture, BlendDrawTex->Tex->PF->D3D11.TextureSRV, BlendDrawTex->Tex->TexWidth, BlendDrawTex->Tex->TexHeight );

				if( GD3D11.Device.DrawSetting.ChangeTextureFlag )
				{
					Graphics_D3D11_DeviceState_NormalDrawSetup() ;
				}

				GETVERTEX_QUAD( DrawVertB )
				
				Vx = 1.0F / ( DrawTex->Tex->TexWidth * GraphExRateX ) ;
				Vy = 1.0F / ( DrawTex->Tex->TexHeight * GraphExRateY ) ;
				VBx = GD3D11.Device.DrawSetting.InvBlendTextureWidth ;
				VBy = GD3D11.Device.DrawSetting.InvBlendTextureHeight ;

				// ���_�f�[�^������������
				TexVert = DrawTex->Vertex ;
				BlendTexVert = BlendDrawTex->Vertex ;

				DrawVertB[0].color = ( DWORD )DiffuseColor ;
				DrawVertB[1].color = ( DWORD )DiffuseColor ;
				DrawVertB[2].color = ( DWORD )DiffuseColor ;
				DrawVertB[3].color = ( DWORD )DiffuseColor ;
				DrawVertB[4].color = ( DWORD )DiffuseColor ;
				DrawVertB[5].color = ( DWORD )DiffuseColor ;

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
					u1[2] = u1[0] = ( TexVert[0].x - BlendTexVert[0].x + GSYS.DrawSetting.BlendGraphX ) * GD3D11.Device.DrawSetting.InvBlendTextureWidth ;
					u1[3] = u1[1] = ( TexVert[1].x - BlendTexVert[0].x + GSYS.DrawSetting.BlendGraphX ) * GD3D11.Device.DrawSetting.InvBlendTextureWidth ;
					v1[1] = v1[0] = ( TexVert[0].y - BlendTexVert[0].y + GSYS.DrawSetting.BlendGraphY ) * GD3D11.Device.DrawSetting.InvBlendTextureHeight ;
					v1[3] = v1[2] = ( TexVert[2].y - BlendTexVert[0].y + GSYS.DrawSetting.BlendGraphY ) * GD3D11.Device.DrawSetting.InvBlendTextureHeight ;
					break ;

				case DX_BLENDGRAPH_POSMODE_SCREEN :
					u1[2] = u1[0] = ( XBuf[ 0 ] - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D11.Device.DrawSetting.InvBlendTextureWidth ;
					u1[3] = u1[1] = ( XBuf[ 1 ] - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D11.Device.DrawSetting.InvBlendTextureWidth ;
					v1[1] = v1[0] = ( YBuf[ 0 ] - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D11.Device.DrawSetting.InvBlendTextureHeight ;
					v1[3] = v1[2] = ( YBuf[ 1 ] - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D11.Device.DrawSetting.InvBlendTextureHeight ;
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

				DrawVertB[0].rhw = 1.0f ;
				DrawVertB[1].rhw = 1.0f ;
				DrawVertB[2].rhw = 1.0f ;
				DrawVertB[3].rhw = 1.0f ;
				DrawVertB[4].rhw = 1.0f ;
				DrawVertB[5].rhw = 1.0f ;

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
				DrawVert[1].color = ( DWORD )DiffuseColor ;
				DrawVert[2].color = ( DWORD )DiffuseColor ;
				DrawVert[3].color = ( DWORD )DiffuseColor ;
				DrawVert[4].color = ( DWORD )DiffuseColor ;
				DrawVert[5].color = ( DWORD )DiffuseColor ;

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

				DrawVert[0].rhw = 1.0f ;
				DrawVert[1].rhw = 1.0f ;
				DrawVert[2].rhw = 1.0f ;
				DrawVert[3].rhw = 1.0f ;
				DrawVert[4].rhw = 1.0f ;
				DrawVert[5].rhw = 1.0f ;

				// �e�N�X�`���[��`�悷��
				ADD4VERTEX_TEX

				i ++ ;
				if( i >= DrawTexNum ) break ;

				// �`�悷��e�N�X�`���[�̃Z�b�g
				DrawTex ++ ;
				Graphics_D3D11_DrawSetting_SetTexture( DrawTex->Tex->PF->D3D11.Texture, DrawTex->Tex->PF->D3D11.TextureSRV ) ;
				Graphics_D3D11_DeviceState_NormalDrawSetup() ;
			}
			break ;
		}
	}

	// �I��
	return 0 ;
}

// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawRotaGraph
extern int Graphics_D3D11_DrawRotaGraph( int x, int y, float xf, float yf, double ExRate, double Angle, IMAGEDATA *Image, IMAGEDATA *BlendImage, int TransFlag, int TurnFlag, int IntFlag )
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
	DWORD drawz;
//	BOOL Adj = FALSE ;
	float CenX, CenY ;
	float fx, fy ;
	int hx = 0, hy = 0 ;
	int FastCode, SinCosNone, AngleCheck, AlwaysCheck ;
	int Flag ;

	if( GAPIWin.D3D11DeviceObject == NULL )
	{
		return -1 ;
	}

	// �����Ȃ������ꍇ�̓G���[
	if( ExRate == 0.0 ) return -1 ;

	// �`��̏���
	Flag = TransFlag ;
	DX_D3D11_DRAWPREP_TEX( Image->Orig, Image->Hard.Draw[ 0 ].Tex->PF->D3D11.Texture, Image->Hard.Draw[ 0 ].Tex->PF->D3D11.TextureSRV, Flag )

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
						xf   = ( float )x ;
						yf   = ( float )y ;
					}
					else
					{
						CenX = ( float )Image->WidthI * 0.5F ;
						CenY = ( float )Image->HeightI * 0.5F ;
						xf   = ( float )x - 0.5F ;
						yf   = ( float )y - 0.5F ;
						FastCode = 1;
					}
				}
				else
				{
					CenX = ( float )Image->WidthI * 0.5F ;
					CenY = ( float )Image->HeightI * 0.5F ;
					xf   = ( float )x ;
					yf   = ( float )y ;
						
					if( hx != 0 ) CenX -= 0.5F ;
					if( hy != 0 ) CenY -= 0.5F ;
				}
			}
			else
			{
				CenX = ( float )Image->WidthI * 0.5F ;
				CenY = ( float )Image->HeightI * 0.5F ;
				xf   = ( float )x ;
				yf   = ( float )y ;
				FastCode = 1 ;
			}
		}
		else
		{
			CenX = ( float )Image->WidthI * 0.5F ;
			CenY = ( float )Image->HeightI * 0.5F ;
			xf   = ( float )x + 0.5f ;
			yf   = ( float )y + 0.5f ;
			AlwaysCheck = 1 ;
		}
	}
	else
	{
		CenX = ( float )Image->WidthF * 0.5F ;
		CenY = ( float )Image->HeightF * 0.5F ;
//		xf -= 0.5F ;
//		yf -= 0.5F ;
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

//	AlwaysCheck = 1 ;
//	FastCode = 1 ;

	// ���_�f�[�^���擾�i���̂Ƃ���F���̃Z�b�g�̂݁j
	DiffuseColor = GD3D11.Device.DrawInfo.DiffuseColor ;

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
	switch( GD3D11.Device.DrawInfo.VertexType )
	{
	case VERTEXTYPE_BLENDTEX :	// �u�����h�e�N�X�`�����g�p����
		// �e�N�X�`���[�̐������J��Ԃ�
		if( DrawTexNum == 1 && FastCode == 1 )
		{
			// �`�悷��e�N�X�`���[�̃Z�b�g
			Graphics_D3D11_DrawSetting_SetTexture( DrawTex->Tex->PF->D3D11.Texture, DrawTex->Tex->PF->D3D11.TextureSRV ) ;
			Graphics_D3D11_DrawSetting_SetBlendTexture( BlendDrawTex->Tex->PF->D3D11.Texture, BlendDrawTex->Tex->PF->D3D11.TextureSRV, BlendDrawTex->Tex->TexWidth, BlendDrawTex->Tex->TexHeight );

			if( GD3D11.Device.DrawSetting.ChangeTextureFlag )
			{
				Graphics_D3D11_DeviceState_NormalDrawSetup() ;
			}

			// ���_�f�[�^������������
			GETVERTEX_QUAD( DrawVertB )

			TexVert = DrawTex->Vertex ;
			BlendTexVert = BlendDrawTex->Vertex ;

			DrawVertB[0].color = ( DWORD )DiffuseColor ;
			DrawVertB[1].color = ( DWORD )DiffuseColor ;
			DrawVertB[2].color = ( DWORD )DiffuseColor ;
			DrawVertB[3].color = ( DWORD )DiffuseColor ;
			DrawVertB[4].color = ( DWORD )DiffuseColor ;
			DrawVertB[5].color = ( DWORD )DiffuseColor ;

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
				u[ 0 ] = TexVert[0].x - BlendTexVert[0].x + GSYS.DrawSetting.BlendGraphX * GD3D11.Device.DrawSetting.InvBlendTextureWidth ;
				u[ 1 ] = TexVert[1].x - BlendTexVert[0].x + GSYS.DrawSetting.BlendGraphX * GD3D11.Device.DrawSetting.InvBlendTextureWidth ;
				v[ 0 ] = TexVert[0].y - BlendTexVert[0].y + GSYS.DrawSetting.BlendGraphY * GD3D11.Device.DrawSetting.InvBlendTextureHeight ;
				v[ 1 ] = TexVert[2].y - BlendTexVert[0].y + GSYS.DrawSetting.BlendGraphY * GD3D11.Device.DrawSetting.InvBlendTextureHeight ;
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
				DrawVertB[0].u1 = ( (  XBuf[1] + xf ) - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D11.Device.DrawSetting.InvBlendTextureWidth ;
				DrawVertB[0].v1 = ( (  YBuf[1] + yf ) - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D11.Device.DrawSetting.InvBlendTextureHeight ;
				DrawVertB[1].u1 = ( (  XBuf[3] + xf ) - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D11.Device.DrawSetting.InvBlendTextureWidth ;
				DrawVertB[1].v1 = ( (  YBuf[3] + yf ) - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D11.Device.DrawSetting.InvBlendTextureHeight ;
				DrawVertB[2].u1 = ( ( -XBuf[3] + xf ) - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D11.Device.DrawSetting.InvBlendTextureWidth ;
				DrawVertB[2].v1 = ( ( -YBuf[3] + yf ) - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D11.Device.DrawSetting.InvBlendTextureHeight ;
				DrawVertB[3].u1 = ( ( -XBuf[1] + xf ) - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D11.Device.DrawSetting.InvBlendTextureWidth ;
				DrawVertB[3].v1 = ( ( -YBuf[1] + yf ) - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D11.Device.DrawSetting.InvBlendTextureHeight ;
				DrawVertB[4].u1 = ( ( -XBuf[3] + xf ) - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D11.Device.DrawSetting.InvBlendTextureWidth ;
				DrawVertB[4].v1 = ( ( -YBuf[3] + yf ) - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D11.Device.DrawSetting.InvBlendTextureHeight ;
				DrawVertB[5].u1 = ( (  XBuf[3] + xf ) - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D11.Device.DrawSetting.InvBlendTextureWidth ;
				DrawVertB[5].v1 = ( (  YBuf[3] + yf ) - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D11.Device.DrawSetting.InvBlendTextureHeight ;
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

			DrawVertB[0].rhw = 1.0f ;
			DrawVertB[1].rhw = 1.0f ;
			DrawVertB[2].rhw = 1.0f ;
			DrawVertB[3].rhw = 1.0f ;
			DrawVertB[4].rhw = 1.0f ;
			DrawVertB[5].rhw = 1.0f ;

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
				Graphics_D3D11_DrawSetting_SetTexture( DrawTex->Tex->PF->D3D11.Texture, DrawTex->Tex->PF->D3D11.TextureSRV ) ;
				Graphics_D3D11_DrawSetting_SetBlendTexture( BlendDrawTex->Tex->PF->D3D11.Texture, BlendDrawTex->Tex->PF->D3D11.TextureSRV, BlendDrawTex->Tex->TexWidth, BlendDrawTex->Tex->TexHeight );

				if( GD3D11.Device.DrawSetting.ChangeTextureFlag )
				{
					Graphics_D3D11_DeviceState_NormalDrawSetup() ;
				}

				// ���_�f�[�^������������
				GETVERTEX_QUAD( DrawVertB )

				TexVert = DrawTex->Vertex ;
				BlendTexVert = BlendDrawTex->Vertex ;

				DrawVertB[0].color = ( DWORD )DiffuseColor ;
				DrawVertB[1].color = ( DWORD )DiffuseColor ;
				DrawVertB[2].color = ( DWORD )DiffuseColor ;
				DrawVertB[3].color = ( DWORD )DiffuseColor ;
				DrawVertB[4].color = ( DWORD )DiffuseColor ;
				DrawVertB[5].color = ( DWORD )DiffuseColor ;

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
					u[ 0 ] = ( TexVert[0].x - BlendTexVert[0].x + GSYS.DrawSetting.BlendGraphX ) * GD3D11.Device.DrawSetting.InvBlendTextureWidth ;
					u[ 1 ] = ( TexVert[1].x - BlendTexVert[0].x + GSYS.DrawSetting.BlendGraphX ) * GD3D11.Device.DrawSetting.InvBlendTextureWidth ;
					u[ 2 ] = ( TexVert[2].x - BlendTexVert[0].x + GSYS.DrawSetting.BlendGraphX ) * GD3D11.Device.DrawSetting.InvBlendTextureWidth ;
					u[ 3 ] = ( TexVert[3].x - BlendTexVert[0].x + GSYS.DrawSetting.BlendGraphX ) * GD3D11.Device.DrawSetting.InvBlendTextureWidth ;
					
					v[ 0 ] = ( TexVert[0].y - BlendTexVert[0].y + GSYS.DrawSetting.BlendGraphY ) * GD3D11.Device.DrawSetting.InvBlendTextureHeight ;
					v[ 1 ] = ( TexVert[1].y - BlendTexVert[0].y + GSYS.DrawSetting.BlendGraphY ) * GD3D11.Device.DrawSetting.InvBlendTextureHeight ;
					v[ 2 ] = ( TexVert[2].y - BlendTexVert[0].y + GSYS.DrawSetting.BlendGraphY ) * GD3D11.Device.DrawSetting.InvBlendTextureHeight ;
					v[ 3 ] = ( TexVert[3].y - BlendTexVert[0].y + GSYS.DrawSetting.BlendGraphY ) * GD3D11.Device.DrawSetting.InvBlendTextureHeight ;

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
					DrawVertB[0].u1 = ( ( XBuf[1] + xf ) - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D11.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[0].v1 = ( ( YBuf[1] + yf ) - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D11.Device.DrawSetting.InvBlendTextureHeight ;
					DrawVertB[1].u1 = ( ( XBuf[3] + xf ) - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D11.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[1].v1 = ( ( YBuf[3] + yf ) - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D11.Device.DrawSetting.InvBlendTextureHeight ;
					DrawVertB[2].u1 = ( ( XBuf[5] + xf ) - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D11.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[2].v1 = ( ( YBuf[5] + yf ) - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D11.Device.DrawSetting.InvBlendTextureHeight ;
					DrawVertB[3].u1 = ( ( XBuf[7] + xf ) - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D11.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[3].v1 = ( ( YBuf[7] + yf ) - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D11.Device.DrawSetting.InvBlendTextureHeight ;
					DrawVertB[4].u1 = ( ( XBuf[5] + xf ) - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D11.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[4].v1 = ( ( YBuf[5] + yf ) - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D11.Device.DrawSetting.InvBlendTextureHeight ;
					DrawVertB[5].u1 = ( ( XBuf[3] + xf ) - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D11.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[5].v1 = ( ( YBuf[3] + yf ) - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D11.Device.DrawSetting.InvBlendTextureHeight ;
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

				DrawVertB[0].rhw = 1.0f ;
				DrawVertB[1].rhw = 1.0f ;
				DrawVertB[2].rhw = 1.0f ;
				DrawVertB[3].rhw = 1.0f ;
				DrawVertB[4].rhw = 1.0f ;
				DrawVertB[5].rhw = 1.0f ;

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
				DrawVert[1].color = ( DWORD )DiffuseColor ;
				DrawVert[2].color = ( DWORD )DiffuseColor ;
				DrawVert[3].color = ( DWORD )DiffuseColor ;
				DrawVert[4].color = ( DWORD )DiffuseColor ;
				DrawVert[5].color = ( DWORD )DiffuseColor ;

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

				DrawVert[0].rhw = 1.0f ;
				DrawVert[1].rhw = 1.0f ;
				DrawVert[2].rhw = 1.0f ;
				DrawVert[3].rhw = 1.0f ;
				DrawVert[4].rhw = 1.0f ;
				DrawVert[5].rhw = 1.0f ;

				// �e�N�X�`���[��`�悷��
				ADD4VERTEX_TEX
			}
			else
			{
				// ���_�f�[�^������������
				GETVERTEX_QUAD( DrawVert )
				
				TexVert = DrawTex->Vertex ;

				DrawVert[0].color = ( DWORD )DiffuseColor ;
				DrawVert[1].color = ( DWORD )DiffuseColor ;
				DrawVert[2].color = ( DWORD )DiffuseColor ;
				DrawVert[3].color = ( DWORD )DiffuseColor ;
				DrawVert[4].color = ( DWORD )DiffuseColor ;
				DrawVert[5].color = ( DWORD )DiffuseColor ;

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

				DrawVert[0].rhw = 1.0f ;
				DrawVert[1].rhw = 1.0f ;
				DrawVert[2].rhw = 1.0f ;
				DrawVert[3].rhw = 1.0f ;
				DrawVert[4].rhw = 1.0f ;
				DrawVert[5].rhw = 1.0f ;

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
					DrawVert[1].color = ( DWORD )DiffuseColor ;
					DrawVert[2].color = ( DWORD )DiffuseColor ;
					DrawVert[3].color = ( DWORD )DiffuseColor ;
					DrawVert[4].color = ( DWORD )DiffuseColor ;
					DrawVert[5].color = ( DWORD )DiffuseColor ;

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

					DrawVert[0].rhw = 1.0f ;
					DrawVert[1].rhw = 1.0f ;
					DrawVert[2].rhw = 1.0f ;
					DrawVert[3].rhw = 1.0f ;
					DrawVert[4].rhw = 1.0f ;
					DrawVert[5].rhw = 1.0f ;

					// �e�N�X�`���[��`�悷��
					ADD4VERTEX_TEX

					i ++ ;
					if( i >= DrawTexNum ) break ;

					// �`�悷��e�N�X�`���[�̃Z�b�g
					DrawTex ++ ;
					Graphics_D3D11_DrawSetting_SetTexture( DrawTex->Tex->PF->D3D11.Texture, DrawTex->Tex->PF->D3D11.TextureSRV ) ;
					Graphics_D3D11_DeviceState_NormalDrawSetup() ;
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
					DrawVert[1].color = ( DWORD )DiffuseColor ;
					DrawVert[2].color = ( DWORD )DiffuseColor ;
					DrawVert[3].color = ( DWORD )DiffuseColor ;
					DrawVert[4].color = ( DWORD )DiffuseColor ;
					DrawVert[5].color = ( DWORD )DiffuseColor ;

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

					DrawVert[0].rhw = 1.0f ;
					DrawVert[1].rhw = 1.0f ;
					DrawVert[2].rhw = 1.0f ;
					DrawVert[3].rhw = 1.0f ;
					DrawVert[4].rhw = 1.0f ;
					DrawVert[5].rhw = 1.0f ;

					// �e�N�X�`���[��`�悷��
					ADD4VERTEX_TEX

					i ++ ;
					if( i >= DrawTexNum ) break ;

					// �`�悷��e�N�X�`���[�̃Z�b�g
					DrawTex ++ ;
					Graphics_D3D11_DrawSetting_SetTexture( DrawTex->Tex->PF->D3D11.Texture, DrawTex->Tex->PF->D3D11.TextureSRV ) ;
					Graphics_D3D11_DeviceState_NormalDrawSetup() ;
				}
			}
		}
		break ;
	}

	// �I��
	return 0 ;
}

// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawRotaGraphFast
extern	int		Graphics_D3D11_DrawRotaGraphFast( int x, int y, float xf, float yf, float ExtendRate, float Angle, IMAGEDATA *Image, IMAGEDATA *BlendImage, int TransFlag, int TurnFlag, int IntFlag )
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
	float XBuf[ 8 ] ;
	float YBuf[ 8 ] ;
	float u[ 4 ] ;
	float v[ 4 ] ;
	float ExtendRateX ;
	DWORD drawz;
//	BOOL Adj = FALSE ;
	float CenX, CenY ;
	int SinCosNone ;
	int Flag ;

	if( GAPIWin.D3D11DeviceObject == NULL )
	{
		return -1 ;
	}

	// �����Ȃ������ꍇ�̓G���[
	if( ExtendRate == 0.0f ) return -1 ;

	// �`��̏���
	Flag = TransFlag ;
	DX_D3D11_DRAWPREP_TEX( Image->Orig, Image->Hard.Draw[ 0 ].Tex->PF->D3D11.Texture, Image->Hard.Draw[ 0 ].Tex->PF->D3D11.TextureSRV, Flag )

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

	// ���S���W�̃Z�b�g
	CenX = ( float )Image->WidthF  * 0.5f ;
	CenY = ( float )Image->HeightF * 0.5f ;

	// ���_�f�[�^���擾�i���̂Ƃ���F���̃Z�b�g�̂݁j
	DiffuseColor = GD3D11.Device.DrawInfo.DiffuseColor ;

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
	switch( GD3D11.Device.DrawInfo.VertexType )
	{
	case VERTEXTYPE_BLENDTEX :	// �u�����h�e�N�X�`�����g�p����
		// �e�N�X�`���[�̐������J��Ԃ�
		if( DrawTexNum == 1 )
		{
			// �`�悷��e�N�X�`���[�̃Z�b�g
			Graphics_D3D11_DrawSetting_SetTexture( DrawTex->Tex->PF->D3D11.Texture, DrawTex->Tex->PF->D3D11.TextureSRV ) ;
			Graphics_D3D11_DrawSetting_SetBlendTexture( BlendDrawTex->Tex->PF->D3D11.Texture, BlendDrawTex->Tex->PF->D3D11.TextureSRV, BlendDrawTex->Tex->TexWidth, BlendDrawTex->Tex->TexHeight );

			if( GD3D11.Device.DrawSetting.ChangeTextureFlag )
			{
				Graphics_D3D11_DeviceState_NormalDrawSetup() ;
			}

			// ���_�f�[�^������������
			GETVERTEX_QUAD( DrawVertB )

			TexVert = DrawTex->Vertex ;
			BlendTexVert = BlendDrawTex->Vertex ;

			DrawVertB[0].color = ( DWORD )DiffuseColor ;
			DrawVertB[1].color = ( DWORD )DiffuseColor ;
			DrawVertB[2].color = ( DWORD )DiffuseColor ;
			DrawVertB[3].color = ( DWORD )DiffuseColor ;
			DrawVertB[4].color = ( DWORD )DiffuseColor ;
			DrawVertB[5].color = ( DWORD )DiffuseColor ;

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
				u[ 0 ] = TexVert[0].x - BlendTexVert[0].x + GSYS.DrawSetting.BlendGraphX * GD3D11.Device.DrawSetting.InvBlendTextureWidth ;
				u[ 1 ] = TexVert[1].x - BlendTexVert[0].x + GSYS.DrawSetting.BlendGraphX * GD3D11.Device.DrawSetting.InvBlendTextureWidth ;
				v[ 0 ] = TexVert[0].y - BlendTexVert[0].y + GSYS.DrawSetting.BlendGraphY * GD3D11.Device.DrawSetting.InvBlendTextureHeight ;
				v[ 1 ] = TexVert[2].y - BlendTexVert[0].y + GSYS.DrawSetting.BlendGraphY * GD3D11.Device.DrawSetting.InvBlendTextureHeight ;
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
				DrawVertB[0].u1 = ( (  XBuf[1] + xf ) - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D11.Device.DrawSetting.InvBlendTextureWidth ;
				DrawVertB[0].v1 = ( (  YBuf[1] + yf ) - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D11.Device.DrawSetting.InvBlendTextureHeight ;
				DrawVertB[1].u1 = ( (  XBuf[3] + xf ) - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D11.Device.DrawSetting.InvBlendTextureWidth ;
				DrawVertB[1].v1 = ( (  YBuf[3] + yf ) - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D11.Device.DrawSetting.InvBlendTextureHeight ;
				DrawVertB[2].u1 = ( ( -XBuf[3] + xf ) - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D11.Device.DrawSetting.InvBlendTextureWidth ;
				DrawVertB[2].v1 = ( ( -YBuf[3] + yf ) - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D11.Device.DrawSetting.InvBlendTextureHeight ;
				DrawVertB[3].u1 = ( ( -XBuf[1] + xf ) - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D11.Device.DrawSetting.InvBlendTextureWidth ;
				DrawVertB[3].v1 = ( ( -YBuf[1] + yf ) - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D11.Device.DrawSetting.InvBlendTextureHeight ;
				DrawVertB[4].u1 = ( ( -XBuf[3] + xf ) - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D11.Device.DrawSetting.InvBlendTextureWidth ;
				DrawVertB[4].v1 = ( ( -YBuf[3] + yf ) - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D11.Device.DrawSetting.InvBlendTextureHeight ;
				DrawVertB[5].u1 = ( (  XBuf[3] + xf ) - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D11.Device.DrawSetting.InvBlendTextureWidth ;
				DrawVertB[5].v1 = ( (  YBuf[3] + yf ) - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D11.Device.DrawSetting.InvBlendTextureHeight ;
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

			DrawVertB[0].rhw = 1.0f ;
			DrawVertB[1].rhw = 1.0f ;
			DrawVertB[2].rhw = 1.0f ;
			DrawVertB[3].rhw = 1.0f ;
			DrawVertB[4].rhw = 1.0f ;
			DrawVertB[5].rhw = 1.0f ;

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
				Graphics_D3D11_DrawSetting_SetTexture( DrawTex->Tex->PF->D3D11.Texture, DrawTex->Tex->PF->D3D11.TextureSRV ) ;
				Graphics_D3D11_DrawSetting_SetBlendTexture( BlendDrawTex->Tex->PF->D3D11.Texture, BlendDrawTex->Tex->PF->D3D11.TextureSRV, BlendDrawTex->Tex->TexWidth, BlendDrawTex->Tex->TexHeight );

				if( GD3D11.Device.DrawSetting.ChangeTextureFlag )
				{
					Graphics_D3D11_DeviceState_NormalDrawSetup() ;
				}

				// ���_�f�[�^������������
				GETVERTEX_QUAD( DrawVertB )

				TexVert = DrawTex->Vertex ;
				BlendTexVert = BlendDrawTex->Vertex ;

				DrawVertB[0].color = ( DWORD )DiffuseColor ;
				DrawVertB[1].color = ( DWORD )DiffuseColor ;
				DrawVertB[2].color = ( DWORD )DiffuseColor ;
				DrawVertB[3].color = ( DWORD )DiffuseColor ;
				DrawVertB[4].color = ( DWORD )DiffuseColor ;
				DrawVertB[5].color = ( DWORD )DiffuseColor ;

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
					u[ 0 ] = ( TexVert[0].x - BlendTexVert[0].x + GSYS.DrawSetting.BlendGraphX ) * GD3D11.Device.DrawSetting.InvBlendTextureWidth ;
					u[ 1 ] = ( TexVert[1].x - BlendTexVert[0].x + GSYS.DrawSetting.BlendGraphX ) * GD3D11.Device.DrawSetting.InvBlendTextureWidth ;
					u[ 2 ] = ( TexVert[2].x - BlendTexVert[0].x + GSYS.DrawSetting.BlendGraphX ) * GD3D11.Device.DrawSetting.InvBlendTextureWidth ;
					u[ 3 ] = ( TexVert[3].x - BlendTexVert[0].x + GSYS.DrawSetting.BlendGraphX ) * GD3D11.Device.DrawSetting.InvBlendTextureWidth ;
					
					v[ 0 ] = ( TexVert[0].y - BlendTexVert[0].y + GSYS.DrawSetting.BlendGraphY ) * GD3D11.Device.DrawSetting.InvBlendTextureHeight ;
					v[ 1 ] = ( TexVert[1].y - BlendTexVert[0].y + GSYS.DrawSetting.BlendGraphY ) * GD3D11.Device.DrawSetting.InvBlendTextureHeight ;
					v[ 2 ] = ( TexVert[2].y - BlendTexVert[0].y + GSYS.DrawSetting.BlendGraphY ) * GD3D11.Device.DrawSetting.InvBlendTextureHeight ;
					v[ 3 ] = ( TexVert[3].y - BlendTexVert[0].y + GSYS.DrawSetting.BlendGraphY ) * GD3D11.Device.DrawSetting.InvBlendTextureHeight ;

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
					DrawVertB[0].u1 = ( ( XBuf[1] + xf ) - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D11.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[0].v1 = ( ( YBuf[1] + yf ) - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D11.Device.DrawSetting.InvBlendTextureHeight ;
					DrawVertB[1].u1 = ( ( XBuf[3] + xf ) - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D11.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[1].v1 = ( ( YBuf[3] + yf ) - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D11.Device.DrawSetting.InvBlendTextureHeight ;
					DrawVertB[2].u1 = ( ( XBuf[5] + xf ) - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D11.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[2].v1 = ( ( YBuf[5] + yf ) - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D11.Device.DrawSetting.InvBlendTextureHeight ;
					DrawVertB[3].u1 = ( ( XBuf[7] + xf ) - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D11.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[3].v1 = ( ( YBuf[7] + yf ) - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D11.Device.DrawSetting.InvBlendTextureHeight ;
					DrawVertB[4].u1 = ( ( XBuf[5] + xf ) - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D11.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[4].v1 = ( ( YBuf[5] + yf ) - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D11.Device.DrawSetting.InvBlendTextureHeight ;
					DrawVertB[5].u1 = ( ( XBuf[3] + xf ) - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D11.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[5].v1 = ( ( YBuf[3] + yf ) - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D11.Device.DrawSetting.InvBlendTextureHeight ;
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

				DrawVertB[0].rhw = 1.0f ;
				DrawVertB[1].rhw = 1.0f ;
				DrawVertB[2].rhw = 1.0f ;
				DrawVertB[3].rhw = 1.0f ;
				DrawVertB[4].rhw = 1.0f ;
				DrawVertB[5].rhw = 1.0f ;

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
				DrawVert[1].color = ( DWORD )DiffuseColor ;
				DrawVert[2].color = ( DWORD )DiffuseColor ;
				DrawVert[3].color = ( DWORD )DiffuseColor ;
				DrawVert[4].color = ( DWORD )DiffuseColor ;
				DrawVert[5].color = ( DWORD )DiffuseColor ;

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

				DrawVert[0].rhw = 1.0f ;
				DrawVert[1].rhw = 1.0f ;
				DrawVert[2].rhw = 1.0f ;
				DrawVert[3].rhw = 1.0f ;
				DrawVert[4].rhw = 1.0f ;
				DrawVert[5].rhw = 1.0f ;

				// �e�N�X�`���[��`�悷��
				ADD4VERTEX_TEX
			}
			else
			{
				// ���_�f�[�^������������
				GETVERTEX_QUAD( DrawVert )
				
				TexVert = DrawTex->Vertex ;

				DrawVert[0].color = ( DWORD )DiffuseColor ;
				DrawVert[1].color = ( DWORD )DiffuseColor ;
				DrawVert[2].color = ( DWORD )DiffuseColor ;
				DrawVert[3].color = ( DWORD )DiffuseColor ;
				DrawVert[4].color = ( DWORD )DiffuseColor ;
				DrawVert[5].color = ( DWORD )DiffuseColor ;

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

				DrawVert[0].rhw = 1.0f ;
				DrawVert[1].rhw = 1.0f ;
				DrawVert[2].rhw = 1.0f ;
				DrawVert[3].rhw = 1.0f ;
				DrawVert[4].rhw = 1.0f ;
				DrawVert[5].rhw = 1.0f ;

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
					DrawVert[1].color = ( DWORD )DiffuseColor ;
					DrawVert[2].color = ( DWORD )DiffuseColor ;
					DrawVert[3].color = ( DWORD )DiffuseColor ;
					DrawVert[4].color = ( DWORD )DiffuseColor ;
					DrawVert[5].color = ( DWORD )DiffuseColor ;

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

					DrawVert[0].rhw = 1.0f ;
					DrawVert[1].rhw = 1.0f ;
					DrawVert[2].rhw = 1.0f ;
					DrawVert[3].rhw = 1.0f ;
					DrawVert[4].rhw = 1.0f ;
					DrawVert[5].rhw = 1.0f ;

					// �e�N�X�`���[��`�悷��
					ADD4VERTEX_TEX

					i ++ ;
					if( i >= DrawTexNum ) break ;

					// �`�悷��e�N�X�`���[�̃Z�b�g
					DrawTex ++ ;
					Graphics_D3D11_DrawSetting_SetTexture( DrawTex->Tex->PF->D3D11.Texture, DrawTex->Tex->PF->D3D11.TextureSRV ) ;
					Graphics_D3D11_DeviceState_NormalDrawSetup() ;
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
					DrawVert[1].color = ( DWORD )DiffuseColor ;
					DrawVert[2].color = ( DWORD )DiffuseColor ;
					DrawVert[3].color = ( DWORD )DiffuseColor ;
					DrawVert[4].color = ( DWORD )DiffuseColor ;
					DrawVert[5].color = ( DWORD )DiffuseColor ;

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

					DrawVert[0].rhw = 1.0f ;
					DrawVert[1].rhw = 1.0f ;
					DrawVert[2].rhw = 1.0f ;
					DrawVert[3].rhw = 1.0f ;
					DrawVert[4].rhw = 1.0f ;
					DrawVert[5].rhw = 1.0f ;

					// �e�N�X�`���[��`�悷��
					ADD4VERTEX_TEX

					i ++ ;
					if( i >= DrawTexNum ) break ;

					// �`�悷��e�N�X�`���[�̃Z�b�g
					DrawTex ++ ;
					Graphics_D3D11_DrawSetting_SetTexture( DrawTex->Tex->PF->D3D11.Texture, DrawTex->Tex->PF->D3D11.TextureSRV ) ;
					Graphics_D3D11_DeviceState_NormalDrawSetup() ;
				}
			}
		}
		break ;
	}

	// �I��
	return 0 ;
}

// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawModiGraph
extern	int		Graphics_D3D11_DrawModiGraph( int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, IMAGEDATA *Image, IMAGEDATA *BlendImage, int TransFlag, bool SimpleDrawFlag )
{
	if( x1 == x3 && x2 == x4 && y1 == y2 && y3 == y4 ) return Graphics_D3D11_DrawExtendGraph( x1, y1, x4, y4, ( float )x1, ( float )y1, ( float )x4, ( float )y4, Image, BlendImage, TransFlag, TRUE ) ; 

	// �����u�����h�e�N�X�`�����g�p�����A���^�֐����g�p����t���O�������Ă��Ȃ����
	// �����������s���`��֐����g�p����
	if( GD3D11.Setting.UseOldDrawModiGraphCodeFlag == FALSE && GSYS.DrawSetting.BlendGraph <= 0 )
		return Graphics_D3D11_DrawModiGraphF( (float)x1, (float)y1, (float)x2, (float)y2, (float)x3, (float)y3, (float)x4, (float)y4, Image, BlendImage, TransFlag, SimpleDrawFlag ) ;

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
	float Reverse0 ;
//	float Reverse1 ;
	float ExRate1X1, ExRate1Y1, ExRate2X1, ExRate2Y1 ;
	float ExRate1X2, ExRate1Y2, ExRate2X2, ExRate2Y2 ;
	DWORD DiffuseColor ;
	VECTOR DrawPos[ 4 ] ;
	float u[ 4 ] ;
	float v[ 4 ] ;
	float  xf1, yf1, xf4, yf4;
	DWORD drawz ;
	int Flag ;

	if( GAPIWin.D3D11DeviceObject == NULL )
	{
		return -1 ;
	}

	// �`��̏���
	Flag = TransFlag | DX_D3D11_DRAWPREP_VECTORINT ;
	DX_D3D11_DRAWPREP_TEX( Image->Orig, Image->Hard.Draw[ 0 ].Tex->PF->D3D11.Texture, Image->Hard.Draw[ 0 ].Tex->PF->D3D11.TextureSRV, Flag )

	GraphSizeX = ( float )Image->WidthF ;  GraphSizeY = ( float )Image->HeightF ;
	ExRate1X1 = ( x2 - x1 ) /  GraphSizeX ;  ExRate1X2 = ( x3 - x1 ) /  GraphSizeY ;
	ExRate1Y1 = ( y3 - y1 ) /  GraphSizeY ;  ExRate1Y2 = ( y2 - y1 ) /  GraphSizeX ;
	ExRate2X1 = ( x3 - x4 ) / -GraphSizeX ;  ExRate2X2 = ( x2 - x4 ) / -GraphSizeY ;
	ExRate2Y1 = ( y2 - y4 ) / -GraphSizeY ;  ExRate2Y2 = ( y3 - y4 ) / -GraphSizeX ;
//	xf1 = (float)x1 - 0.5F ; yf1 = (float)y1 - 0.5F ;
//	xf4 = (float)x4 - 0.5F ; yf4 = (float)y4 - 0.5F ; 
	xf1 = (float)x1 ; yf1 = (float)y1 ;
	xf4 = (float)x4 ; yf4 = (float)y4 ; 

	// ���_�f�[�^���擾�i���̂Ƃ���F���̃Z�b�g�̂݁j
	DiffuseColor = GD3D11.Device.DrawInfo.DiffuseColor ;

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
	switch( GD3D11.Device.DrawInfo.VertexType )
	{
	case VERTEXTYPE_BLENDTEX :	// �u�����h�e�N�X�`�����g�p����
		// �e�N�X�`���[�̐������J��Ԃ�
		for( i = 0 ; i < DrawTexNum ; i ++, DrawTex ++ )
		{
			// �`�悷��e�N�X�`���[�̃Z�b�g
			Graphics_D3D11_DrawSetting_SetTexture( DrawTex->Tex->PF->D3D11.Texture, DrawTex->Tex->PF->D3D11.TextureSRV ) ;
			Graphics_D3D11_DrawSetting_SetBlendTexture( BlendDrawTex->Tex->PF->D3D11.Texture, BlendDrawTex->Tex->PF->D3D11.TextureSRV, BlendDrawTex->Tex->TexWidth, BlendDrawTex->Tex->TexHeight );

			if( GD3D11.Device.DrawSetting.ChangeTextureFlag )
			{
				Graphics_D3D11_DeviceState_NormalDrawSetup() ;
			}

			// ���_�o�b�t�@�̎擾
			GETVERTEX_QUAD( DrawVertB )

			DrawVertB[0].color = ( DWORD )DiffuseColor ;
			DrawVertB[1].color = ( DWORD )DiffuseColor ;
			DrawVertB[2].color = ( DWORD )DiffuseColor ;
			DrawVertB[3].color = ( DWORD )DiffuseColor ;
			DrawVertB[4].color = ( DWORD )DiffuseColor ;
			DrawVertB[5].color = ( DWORD )DiffuseColor ;

			VertType     = DrawTex->VertType ;
			TexVert      = DrawTex->Vertex ;
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

				u[ j ] = ( TexVert->x - BlendTexVert->x + GSYS.DrawSetting.BlendGraphX ) * GD3D11.Device.DrawSetting.InvBlendTextureWidth ;
				v[ j ] = ( TexVert->y - BlendTexVert->y + GSYS.DrawSetting.BlendGraphY ) * GD3D11.Device.DrawSetting.InvBlendTextureHeight ;
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
					DrawVertB[ 0 ].u1 = ( DrawPos[ 0 ].x - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D11.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[ 0 ].v1 = ( DrawPos[ 0 ].y - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D11.Device.DrawSetting.InvBlendTextureHeight ;
					DrawVertB[ 1 ].u1 = ( DrawPos[ 2 ].x - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D11.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[ 1 ].v1 = ( DrawPos[ 2 ].y - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D11.Device.DrawSetting.InvBlendTextureHeight ;
					DrawVertB[ 2 ].u1 = ( DrawPos[ 1 ].x - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D11.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[ 2 ].v1 = ( DrawPos[ 1 ].y - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D11.Device.DrawSetting.InvBlendTextureHeight ;
					DrawVertB[ 3 ].u1 = ( DrawPos[ 3 ].x - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D11.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[ 3 ].v1 = ( DrawPos[ 3 ].y - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D11.Device.DrawSetting.InvBlendTextureHeight ;
					DrawVertB[ 4 ].u1 = ( DrawPos[ 1 ].x - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D11.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[ 4 ].v1 = ( DrawPos[ 1 ].y - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D11.Device.DrawSetting.InvBlendTextureHeight ;
					DrawVertB[ 5 ].u1 = ( DrawPos[ 2 ].x - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D11.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[ 5 ].v1 = ( DrawPos[ 2 ].y - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D11.Device.DrawSetting.InvBlendTextureHeight ;
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
					DrawVertB[ 0 ].u1 = ( DrawPos[ 0 ].x - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D11.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[ 0 ].v1 = ( DrawPos[ 0 ].y - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D11.Device.DrawSetting.InvBlendTextureHeight ;
					DrawVertB[ 1 ].u1 = ( DrawPos[ 1 ].x - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D11.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[ 1 ].v1 = ( DrawPos[ 1 ].y - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D11.Device.DrawSetting.InvBlendTextureHeight ;
					DrawVertB[ 2 ].u1 = ( DrawPos[ 2 ].x - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D11.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[ 2 ].v1 = ( DrawPos[ 2 ].y - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D11.Device.DrawSetting.InvBlendTextureHeight ;
					DrawVertB[ 3 ].u1 = ( DrawPos[ 3 ].x - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D11.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[ 3 ].v1 = ( DrawPos[ 3 ].y - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D11.Device.DrawSetting.InvBlendTextureHeight ;
					DrawVertB[ 4 ].u1 = ( DrawPos[ 2 ].x - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D11.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[ 4 ].v1 = ( DrawPos[ 2 ].y - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D11.Device.DrawSetting.InvBlendTextureHeight ;
					DrawVertB[ 5 ].u1 = ( DrawPos[ 1 ].x - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D11.Device.DrawSetting.InvBlendTextureWidth ;
					DrawVertB[ 5 ].v1 = ( DrawPos[ 1 ].y - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D11.Device.DrawSetting.InvBlendTextureHeight ;
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

			DrawVertB[0].rhw = 1.0f ;
			DrawVertB[1].rhw = 1.0f ;
			DrawVertB[2].rhw = 1.0f ;
			DrawVertB[3].rhw = 1.0f ;
			DrawVertB[4].rhw = 1.0f ;
			DrawVertB[5].rhw = 1.0f ;

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
			// ���_�o�b�t�@�̎擾
			GETVERTEX_QUAD( DrawVert )

			// ���_�f�[�^���Z�b�g����
			DrawVert[0].color = ( DWORD )DiffuseColor ;
			DrawVert[1].color = ( DWORD )DiffuseColor ;
			DrawVert[2].color = ( DWORD )DiffuseColor ;
			DrawVert[3].color = ( DWORD )DiffuseColor ;
			DrawVert[4].color = ( DWORD )DiffuseColor ;
			DrawVert[5].color = ( DWORD )DiffuseColor ;

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

			DrawVert[0].rhw = 1.0f ;
			DrawVert[1].rhw = 1.0f ;
			DrawVert[2].rhw = 1.0f ;
			DrawVert[3].rhw = 1.0f ;
			DrawVert[4].rhw = 1.0f ;
			DrawVert[5].rhw = 1.0f ;

			ADD4VERTEX_TEX

			i ++ ;
			if( i >= DrawTexNum ) break ;

			// �`�悷��e�N�X�`���[�̃Z�b�g
			DrawTex ++ ;
			Graphics_D3D11_DrawSetting_SetTexture( DrawTex->Tex->PF->D3D11.Texture, DrawTex->Tex->PF->D3D11.TextureSRV ) ;
			Graphics_D3D11_DeviceState_NormalDrawSetup() ;
		}
		break ;
	}

	// �I��
	return 0 ;
}

// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawModiGraphF
extern	int		Graphics_D3D11_DrawModiGraphF( float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, IMAGEDATA *Image, IMAGEDATA *BlendImage, int TransFlag, bool SimpleDrawFlag )
{
	int Flag ;

	if( GAPIWin.D3D11DeviceObject == NULL )
	{
		return -1 ;
	}

	// �����u�����h�e�N�X�`���t�̏ꍇ�͓����������s��Ȃ��`��֐����g�p����
	if( GSYS.DrawSetting.BlendGraph > 0 ) return Graphics_D3D11_DrawModiGraph( _FTOL( x1 ), _FTOL( y1 ), _FTOL( x2 ), _FTOL( y2 ), _FTOL( x3 ), _FTOL( y3 ), _FTOL( x4 ), _FTOL( y4 ), Image, BlendImage, TransFlag, SimpleDrawFlag ) ;

	// �`��̏���
	Flag = TransFlag ;
	DX_D3D11_DRAWPREP_TEX( Image->Orig, Image->Hard.Draw[ 0 ].Tex->PF->D3D11.Texture, Image->Hard.Draw[ 0 ].Tex->PF->D3D11.TextureSRV, Flag )

	// �e�N�X�`���[���P���݂̂̏ꍇ�͏����𕪊�
	if( Image->Hard.DrawNum == 1 )
	{
		// ���̂܂ܕ`��
		Graphics_D3D11_DrawModiTex( x1, y1, x2, y2, x3, y3, x4, y4, &Image->Hard.Draw[ 0 ], false ) ;
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
		
//		w = 1.0F / (float)Image->WidthF ;
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
			Graphics_D3D11_DrawModiTex( x[0], y[0], x[1], y[1], x[2], y[2], x[3], y[3], DrawTex, false ) ;
		}
	}

	// �I��
	return 0 ;
}

// �Q�����z��I�ɔz�u���ꂽ���_�f�[�^�𒸓_�o�b�t�@�ɒǉ�����
static int D3D11_Graphics_SetPlaneVertexHardware( VERTEX_2D *GraphVert, int xnum, int ynum )
{
	int i, j, k, l ;

	if( GAPIWin.D3D11DeviceObject == NULL ) return 0 ;

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
				Graphics_D3D11_CommonBuffer_DrawIndexedPrimitive(
					D3D11_VERTEX_INPUTLAYOUT_2D,
					D_D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
					GraphVert,
					xnum * ynum,
					list,
					k,
					D_DXGI_FORMAT_R16_UINT
				) ;
					
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
			Graphics_D3D11_CommonBuffer_DrawIndexedPrimitive(
				D3D11_VERTEX_INPUTLAYOUT_2D,
				D_D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
				GraphVert,
				xnum * ynum,
				list,
				k,
				D_DXGI_FORMAT_R16_UINT
			) ;
		}
#undef	MAX_POSNUM
	}

	// �I��
	return 0 ;
}

// �e�N�X�`����ό`���ĕ`�悷��
extern	void	Graphics_D3D11_DrawModiTex( float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, IMAGEDATA_HARD_DRAW *DrawTex, bool SimpleDrawFlag )
{
	IMAGEDATA_HARD_VERT *TexVert ;
	VERTEX_2D *vec ; 
	float xx1, yy1, xx2, yy2, xx3, yy3, xx4, yy4/*, r1, r2, r3, r4*/ ;
//	float xtmp1, ytmp1, xtmp2, ytmp2 ;
	int c, f1, f2 ;
	DWORD DiffuseColor ;
	float Reverse0, Reverse1;
	DWORD drawz ;
	float u[ 2 ] ;
	float v[ 2 ] ;

	// �`�悷��e�N�X�`���[�̃Z�b�g
	Graphics_D3D11_DrawSetting_SetTexture( DrawTex->Tex->PF->D3D11.Texture, DrawTex->Tex->PF->D3D11.TextureSRV ) ;
	if( GD3D11.Device.DrawSetting.ChangeTextureFlag )
	{
		Graphics_D3D11_DeviceState_NormalDrawSetup() ;
	}

	// ���_�f�[�^���擾�i���̂Ƃ���F���̃Z�b�g�̂݁j
	DiffuseColor = GD3D11.Device.DrawInfo.DiffuseColor ;

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
		D3D11_Graphics_SetPlaneVertexHardware( VertData2, tdn, tdn ) ;

#undef TDIVNUM
#undef TDN
	}

	return ;
		
R1 :
	// ���ʂɕ`��
	GETVERTEX_QUAD( vec )

	vec[0].color = ( DWORD )DiffuseColor ;
	vec[1].color = ( DWORD )DiffuseColor ;
	vec[2].color = ( DWORD )DiffuseColor ;
	vec[3].color = ( DWORD )DiffuseColor ;
	vec[4].color = ( DWORD )DiffuseColor ;
	vec[5].color = ( DWORD )DiffuseColor ;

	u[ 0 ] = TexVert[0].u + 0.0625f / DrawTex->Tex->TexWidth ;
	u[ 1 ] = TexVert[1].u - 0.0625f / DrawTex->Tex->TexWidth ;
	v[ 0 ] = TexVert[0].v + 0.0625f / DrawTex->Tex->TexHeight ;
	v[ 1 ] = TexVert[2].v - 0.0625f / DrawTex->Tex->TexHeight ;

	if( Reverse0 <= 0  &&  Reverse1 <= 0 )
	{
		vec[0].pos.x = x1 ;	
		vec[0].pos.y = y1 ;
		vec[1].pos.x = x3 ;
		vec[1].pos.y = y3 ;
		vec[2].pos.x = x2 ;	
		vec[2].pos.y = y2 ;
		vec[3].pos.x = x4 ;
		vec[3].pos.y = y4 ;
		vec[4].pos.x = x2 ;	
		vec[4].pos.y = y2 ;
		vec[5].pos.x = x3 ;
		vec[5].pos.y = y3 ;

		vec[0].u = u[ 0 ] ;
		vec[0].v = v[ 0 ] ;
		vec[1].u = u[ 0 ] ;
		vec[1].v = v[ 1 ] ;
		vec[2].u = u[ 1 ] ;
		vec[2].v = v[ 0 ] ;
		vec[3].u = u[ 1 ] ;
		vec[3].v = v[ 1 ] ;
		vec[4].u = u[ 1 ] ;
		vec[4].v = v[ 0 ] ;
		vec[5].u = u[ 0 ] ;
		vec[5].v = v[ 1 ] ;
	}
	else
	{
		vec[0].pos.x = x1 ;	
		vec[0].pos.y = y1 ;
		vec[1].pos.x = x2 ;	
		vec[1].pos.y = y2 ;
		vec[2].pos.x = x3 ;
		vec[2].pos.y = y3 ;
		vec[3].pos.x = x4 ;
		vec[3].pos.y = y4 ;
		vec[4].pos.x = x3 ;
		vec[4].pos.y = y3 ;
		vec[5].pos.x = x2 ;	
		vec[5].pos.y = y2 ;

		vec[0].u = u[ 0 ] ;
		vec[0].v = v[ 0 ] ;
		vec[1].u = u[ 1 ] ;
		vec[1].v = v[ 0 ] ;
		vec[2].u = u[ 0 ] ;
		vec[2].v = v[ 1 ] ;
		vec[3].u = u[ 1 ] ;
		vec[3].v = v[ 1 ] ;
		vec[4].u = u[ 0 ] ;
		vec[4].v = v[ 1 ] ;
		vec[5].u = u[ 1 ] ;
		vec[5].v = v[ 0 ] ;
	}

	*((DWORD *)&vec[0].pos.z) = drawz ;
	*((DWORD *)&vec[1].pos.z) = drawz ;
	*((DWORD *)&vec[2].pos.z) = drawz ;
	*((DWORD *)&vec[3].pos.z) = drawz ;
	*((DWORD *)&vec[4].pos.z) = drawz ;
	*((DWORD *)&vec[5].pos.z) = drawz ;

	vec[0].rhw = 1.0f ;
	vec[1].rhw = 1.0f ;
	vec[2].rhw = 1.0f ;
	vec[3].rhw = 1.0f ;
	vec[4].rhw = 1.0f ;
	vec[5].rhw = 1.0f ;

	ADD4VERTEX_TEX
}

// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawSimpleQuadrangleGraphF
extern	int		Graphics_D3D11_DrawSimpleQuadrangleGraphF( const GRAPHICS_DRAW_DRAWSIMPLEQUADRANGLEGRAPHF_PARAM *Param, IMAGEDATA *Image, IMAGEDATA *BlendImage )
{
	VERTEX_2D *DrawVert ;
	VERTEX_BLENDTEX_2D *DrawVertB ;
	DWORD DiffuseColor ;
	IMAGEDATA_HARD_DRAW *DrawTex ;
	IMAGEDATA_HARD_DRAW *BlendDrawTex = NULL ;
	IMAGEDATA_HARD_VERT *TexVert ;
	IMAGEDATA_HARD_VERT *BlendTexVert ;
	DWORD drawz;
	int Flag ;
	float u[ 4 ] ;
	float v[ 4 ] ;
	int i ;
	const GRAPHICS_DRAW_DRAWSIMPLEANGLEGRAPHF_VERTEX *ParamV ;

	if( GAPIWin.D3D11DeviceObject == NULL )
	{
		return -1 ;
	}

	// �e�N�X�`���̐����P�ȊO�̏ꍇ�̓G���[
	if( Image->Hard.DrawNum != 1 )
	{
		return -1 ;
	}

	// �`��̏���
	Flag = Param->TransFlag ;
	DX_D3D11_DRAWPREP_TEX( Image->Orig, Image->Hard.Draw[ 0 ].Tex->PF->D3D11.Texture, Image->Hard.Draw[ 0 ].Tex->PF->D3D11.TextureSRV, Flag )

	// ���_�f�[�^���擾�i���̂Ƃ���F���̃Z�b�g�̂݁j
	DiffuseColor = GD3D11.Device.DrawInfo.DiffuseColor ;

	// �`����z��̃A�h���X���Z�b�g���Ă���
	DrawTex = Image->Hard.Draw ;
	if( BlendImage != NULL )
	{
		BlendDrawTex = BlendImage->Hard.Draw ;
	}

	// �y�o�b�t�@�ɏ������ނy�l���Z�b�g����
	drawz = *((DWORD *)&GSYS.DrawSetting.DrawZ);

	// ���_�^�C�v�ɂ���ď����𕪊�
	switch( GD3D11.Device.DrawInfo.VertexType )
	{
	case VERTEXTYPE_BLENDTEX :	// �u�����h�e�N�X�`�����g�p����
		// �`�悷��e�N�X�`���[�̃Z�b�g
		Graphics_D3D11_DrawSetting_SetTexture( DrawTex->Tex->PF->D3D11.Texture, DrawTex->Tex->PF->D3D11.TextureSRV ) ;
		Graphics_D3D11_DrawSetting_SetBlendTexture( BlendDrawTex->Tex->PF->D3D11.Texture, BlendDrawTex->Tex->PF->D3D11.TextureSRV, BlendDrawTex->Tex->TexWidth, BlendDrawTex->Tex->TexHeight );

		if( GD3D11.Device.DrawSetting.ChangeTextureFlag )
		{
			Graphics_D3D11_DeviceState_NormalDrawSetup() ;
		}

		TexVert = DrawTex->Vertex ;
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
				u[ 0 ] = ( TexVert[0].x - BlendTexVert[0].x + GSYS.DrawSetting.BlendGraphX ) * GD3D11.Device.DrawSetting.InvBlendTextureWidth ;
				v[ 0 ] = ( TexVert[0].y - BlendTexVert[0].y + GSYS.DrawSetting.BlendGraphY ) * GD3D11.Device.DrawSetting.InvBlendTextureHeight ;
				u[ 1 ] = ( TexVert[1].x - BlendTexVert[1].x + GSYS.DrawSetting.BlendGraphX ) * GD3D11.Device.DrawSetting.InvBlendTextureWidth ;
				v[ 1 ] = ( TexVert[1].y - BlendTexVert[1].y + GSYS.DrawSetting.BlendGraphY ) * GD3D11.Device.DrawSetting.InvBlendTextureHeight ;
				u[ 2 ] = ( TexVert[2].x - BlendTexVert[2].x + GSYS.DrawSetting.BlendGraphX ) * GD3D11.Device.DrawSetting.InvBlendTextureWidth ;
				v[ 2 ] = ( TexVert[2].y - BlendTexVert[2].y + GSYS.DrawSetting.BlendGraphY ) * GD3D11.Device.DrawSetting.InvBlendTextureHeight ;
				u[ 3 ] = ( TexVert[3].x - BlendTexVert[3].x + GSYS.DrawSetting.BlendGraphX ) * GD3D11.Device.DrawSetting.InvBlendTextureWidth ;
				v[ 3 ] = ( TexVert[3].y - BlendTexVert[3].y + GSYS.DrawSetting.BlendGraphY ) * GD3D11.Device.DrawSetting.InvBlendTextureHeight ;
				break ;

			case DX_BLENDGRAPH_POSMODE_SCREEN :
				u[ 0 ] = ( ParamV[0].x - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D11.Device.DrawSetting.InvBlendTextureWidth ;
				v[ 0 ] = ( ParamV[0].y - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D11.Device.DrawSetting.InvBlendTextureHeight ;
				u[ 1 ] = ( ParamV[1].x - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D11.Device.DrawSetting.InvBlendTextureWidth ;
				v[ 1 ] = ( ParamV[1].y - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D11.Device.DrawSetting.InvBlendTextureHeight ;
				u[ 2 ] = ( ParamV[2].x - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D11.Device.DrawSetting.InvBlendTextureWidth ;
				v[ 2 ] = ( ParamV[2].y - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D11.Device.DrawSetting.InvBlendTextureHeight ;
				u[ 3 ] = ( ParamV[3].x - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D11.Device.DrawSetting.InvBlendTextureWidth ;
				v[ 3 ] = ( ParamV[3].y - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D11.Device.DrawSetting.InvBlendTextureHeight ;
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

			DrawVert[0].rhw = 1.0f ;
			DrawVert[1].rhw = 1.0f ;
			DrawVert[2].rhw = 1.0f ;
			DrawVert[3].rhw = 1.0f ;
			DrawVert[4].rhw = 1.0f ;
			DrawVert[5].rhw = 1.0f ;

			// �e�N�X�`���[��`�悷��
			ADD4VERTEX_TEX
		}
		break ;
	}

	// �I��
	return 0 ;
}

// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawSimpleTriangleGraphF
extern	int		Graphics_D3D11_DrawSimpleTriangleGraphF( const GRAPHICS_DRAW_DRAWSIMPLETRIANGLEGRAPHF_PARAM *Param, IMAGEDATA *Image, IMAGEDATA *BlendImage )
{
	VERTEX_2D *DrawVert ;
	VERTEX_BLENDTEX_2D *DrawVertB ;
	DWORD DiffuseColor ;
	IMAGEDATA_HARD_DRAW *DrawTex ;
	IMAGEDATA_HARD_DRAW *BlendDrawTex = NULL ;
	IMAGEDATA_HARD_VERT *TexVert ;
	IMAGEDATA_HARD_VERT *BlendTexVert ;
	DWORD drawz;
	int Flag ;
	float u[ 3 ] ;
	float v[ 3 ] ;
	int i ;
	const GRAPHICS_DRAW_DRAWSIMPLEANGLEGRAPHF_VERTEX *ParamV ;

	if( GAPIWin.D3D11DeviceObject == NULL )
	{
		return -1 ;
	}

	// �e�N�X�`���̐����P�ȊO�̏ꍇ�̓G���[
	if( Image->Hard.DrawNum != 1 )
	{
		return -1 ;
	}

	// �`��̏���
	Flag = Param->TransFlag ;
	DX_D3D11_DRAWPREP_TEX( Image->Orig, Image->Hard.Draw[ 0 ].Tex->PF->D3D11.Texture, Image->Hard.Draw[ 0 ].Tex->PF->D3D11.TextureSRV, Flag )

	// ���_�f�[�^���擾�i���̂Ƃ���F���̃Z�b�g�̂݁j
	DiffuseColor = GD3D11.Device.DrawInfo.DiffuseColor ;

	// �`����z��̃A�h���X���Z�b�g���Ă���
	DrawTex = Image->Hard.Draw ;
	if( BlendImage != NULL )
	{
		BlendDrawTex = BlendImage->Hard.Draw ;
	}

	// �y�o�b�t�@�ɏ������ނy�l���Z�b�g����
	drawz = *((DWORD *)&GSYS.DrawSetting.DrawZ);

	// ���_�^�C�v�ɂ���ď����𕪊�
	switch( GD3D11.Device.DrawInfo.VertexType )
	{
	case VERTEXTYPE_BLENDTEX :	// �u�����h�e�N�X�`�����g�p����
		// �`�悷��e�N�X�`���[�̃Z�b�g
		Graphics_D3D11_DrawSetting_SetTexture( DrawTex->Tex->PF->D3D11.Texture, DrawTex->Tex->PF->D3D11.TextureSRV ) ;
		Graphics_D3D11_DrawSetting_SetBlendTexture( BlendDrawTex->Tex->PF->D3D11.Texture, BlendDrawTex->Tex->PF->D3D11.TextureSRV, BlendDrawTex->Tex->TexWidth, BlendDrawTex->Tex->TexHeight );

		if( GD3D11.Device.DrawSetting.ChangeTextureFlag )
		{
			Graphics_D3D11_DeviceState_NormalDrawSetup() ;
		}

		TexVert = DrawTex->Vertex ;
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
				u[ 0 ] = ( TexVert[0].x - BlendTexVert[0].x + GSYS.DrawSetting.BlendGraphX ) * GD3D11.Device.DrawSetting.InvBlendTextureWidth ;
				v[ 0 ] = ( TexVert[0].y - BlendTexVert[0].y + GSYS.DrawSetting.BlendGraphY ) * GD3D11.Device.DrawSetting.InvBlendTextureHeight ;
				u[ 1 ] = ( TexVert[1].x - BlendTexVert[1].x + GSYS.DrawSetting.BlendGraphX ) * GD3D11.Device.DrawSetting.InvBlendTextureWidth ;
				v[ 1 ] = ( TexVert[1].y - BlendTexVert[1].y + GSYS.DrawSetting.BlendGraphY ) * GD3D11.Device.DrawSetting.InvBlendTextureHeight ;
				u[ 2 ] = ( TexVert[2].x - BlendTexVert[2].x + GSYS.DrawSetting.BlendGraphX ) * GD3D11.Device.DrawSetting.InvBlendTextureWidth ;
				v[ 2 ] = ( TexVert[2].y - BlendTexVert[2].y + GSYS.DrawSetting.BlendGraphY ) * GD3D11.Device.DrawSetting.InvBlendTextureHeight ;
				break ;

			case DX_BLENDGRAPH_POSMODE_SCREEN :
				u[ 0 ] = ( ParamV[0].x - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D11.Device.DrawSetting.InvBlendTextureWidth ;
				v[ 0 ] = ( ParamV[0].y - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D11.Device.DrawSetting.InvBlendTextureHeight ;
				u[ 1 ] = ( ParamV[1].x - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D11.Device.DrawSetting.InvBlendTextureWidth ;
				v[ 1 ] = ( ParamV[1].y - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D11.Device.DrawSetting.InvBlendTextureHeight ;
				u[ 2 ] = ( ParamV[2].x - BlendTexVert[0].x - GSYS.DrawSetting.BlendGraphX ) * GD3D11.Device.DrawSetting.InvBlendTextureWidth ;
				v[ 2 ] = ( ParamV[2].y - BlendTexVert[0].y - GSYS.DrawSetting.BlendGraphY ) * GD3D11.Device.DrawSetting.InvBlendTextureHeight ;
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

			DrawVert[0].rhw = 1.0f ;
			DrawVert[1].rhw = 1.0f ;
			DrawVert[2].rhw = 1.0f ;

			// �e�N�X�`���[��`�悷��
			ADD3VERTEX_TEX
		}
		break ;
	}

	// �I��
	return 0 ;
}

// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawFillBox
extern	int		Graphics_D3D11_DrawFillBox( int x1, int y1, int x2, int y2, unsigned int Color )
{
	VERTEX_NOTEX_2D *vec ;
	RECT drect ;
	int Red, Green, Blue, Flag ;
	DWORD drawz;

	if( GAPIWin.D3D11DeviceObject == NULL )
	{
		return -1 ;
	}

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
	Flag = DX_D3D11_DRAWPREP_DIFFUSERGB ;
	DX_D3D11_DRAWPREP_NOTEX( Flag ) ;

	// �F���̑��X�e�[�^�X�̃Z�b�g
	NS_GetColor2( Color, &Red, &Green, &Blue ) ;
	SETUPCOLOR( Color )

	// �y�o�b�t�@�ɏ������ނy�l���Z�b�g����
	drawz = *((DWORD *)&GSYS.DrawSetting.DrawZ);

	// ���_�f�[�^�̃Z�b�g
	GETVERTEX_QUAD( vec )

	vec[0].color = Color ;
	vec[1].color = Color ;
	vec[2].color = Color ;
	vec[3].color = Color ;
	vec[4].color = Color ;
	vec[5].color = Color ;
	
	vec[0].pos.x = ( float )drect.left   ;
	vec[0].pos.y = ( float )drect.top    ;
	vec[1].pos.x = ( float )drect.right  ;
	vec[1].pos.y = ( float )drect.top    ;
	vec[2].pos.x = ( float )drect.left   ;
	vec[2].pos.y = ( float )drect.bottom ;
	vec[3].pos.x = ( float )drect.right  ;
	vec[3].pos.y = ( float )drect.bottom ;
	vec[4].pos.x = ( float )drect.left   ;
	vec[4].pos.y = ( float )drect.bottom ;
	vec[5].pos.x = ( float )drect.right  ;
	vec[5].pos.y = ( float )drect.top    ;

	*((DWORD *)&vec[0].pos.z) = drawz ;
	*((DWORD *)&vec[1].pos.z) = drawz ;
	*((DWORD *)&vec[2].pos.z) = drawz ;
	*((DWORD *)&vec[3].pos.z) = drawz ;
	*((DWORD *)&vec[4].pos.z) = drawz ;
	*((DWORD *)&vec[5].pos.z) = drawz ;

	vec[0].rhw = 1.0f ;
	vec[1].rhw = 1.0f ;
	vec[2].rhw = 1.0f ;
	vec[3].rhw = 1.0f ;
	vec[4].rhw = 1.0f ;
	vec[5].rhw = 1.0f ;

	// ���_�f�[�^�̏o��
	ADD4VERTEX_NOTEX

	// �I��
	return 0 ;
}

// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawLineBox
extern	int		Graphics_D3D11_DrawLineBox( int x1, int y1, int x2, int y2, unsigned int Color )
{
	VERTEX_NOTEX_2D *VertData ;
	int Red, Green, Blue ;
	float fx1, fx2, fy1, fy2 ;
	int Flag;
	DWORD drawz ;

	if( GAPIWin.D3D11DeviceObject == NULL )
	{
		return -1 ;
	}

	// �`��̏���
	Flag = DX_D3D11_DRAWPREP_DIFFUSERGB ;
	DX_D3D11_DRAWPREP_NOTEX( Flag ) ;

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

	VertData[ 0].rhw = 1.0f ;
	VertData[ 1].rhw = 1.0f ;
	VertData[ 2].rhw = 1.0f ;
	VertData[ 3].rhw = 1.0f ;
	VertData[ 4].rhw = 1.0f ;
	VertData[ 5].rhw = 1.0f ;
	VertData[ 6].rhw = 1.0f ;
	VertData[ 7].rhw = 1.0f ;
	VertData[ 8].rhw = 1.0f ;
	VertData[ 9].rhw = 1.0f ;
	VertData[10].rhw = 1.0f ;
	VertData[11].rhw = 1.0f ;
	VertData[12].rhw = 1.0f ;
	VertData[13].rhw = 1.0f ;
	VertData[14].rhw = 1.0f ;
	VertData[15].rhw = 1.0f ;
	VertData[16].rhw = 1.0f ;
	VertData[17].rhw = 1.0f ;
	VertData[18].rhw = 1.0f ;
	VertData[19].rhw = 1.0f ;
	VertData[20].rhw = 1.0f ;
	VertData[21].rhw = 1.0f ;
	VertData[22].rhw = 1.0f ;
	VertData[23].rhw = 1.0f ;

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
extern	int		Graphics_D3D11_DrawLine( int x1, int y1, int x2, int y2, unsigned int Color )
{
	VERTEX_NOTEX_2D *VertData ;
	int Red, Green, Blue, Flag ;
	DWORD drawz ;

	if( GAPIWin.D3D11DeviceObject == NULL )
	{
		return -1 ;
	} 

	if( x2 - x1 == 0 && y2 - y1 == 0 ) return 0 ; 

	// �`��̏���
	Flag = DX_D3D11_DRAWPREP_DIFFUSERGB ;
	DX_D3D11_DRAWPREP_NOTEX( Flag )

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
	VertData[ 0 ].rhw = 1.0f ;
	VertData[ 1 ].rhw = 1.0f ;

	// ���W���Z�b�g
	VertData[ 0 ].pos.x = ( float )( x1 + 0.5f ) ;
	VertData[ 0 ].pos.y = ( float )( y1 + 0.5f ) ;

	VertData[ 1 ].pos.x = ( float )( x2 + 0.5f ) ;
	VertData[ 1 ].pos.y = ( float )( y2 + 0.5f ) ;

	// ���W�̕␳
	if( x1 == x2 )
	{
		if( y1 < y2 )
		{
			VertData[ 0 ].pos.y -= 0.1f ;
			VertData[ 1 ].pos.y -= 0.1f ;
		}
		else
		{
			VertData[ 0 ].pos.y += 0.1f ;
			VertData[ 1 ].pos.y += 0.1f ;
		}
	}

	if( y1 == y2 )
	{
		if( x1 < x2 )
		{
			VertData[ 0 ].pos.x -= 0.1f ;
			VertData[ 1 ].pos.x -= 0.1f ;
		}
		else
		{
			VertData[ 0 ].pos.x += 0.1f ;
			VertData[ 1 ].pos.x += 0.1f ;
		}
	}

	// ���_�̒ǉ�
	ADD4VERTEX_LINE

	// �I��
	return 0 ;
}

// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawLine3D
extern	int		Graphics_D3D11_DrawLine3D( VECTOR Pos1, VECTOR Pos2, unsigned int Color, int DrawFlag, RECT *DrawArea )
{
	VERTEX_NOTEX_3D *VertData ;
	int Red, Green, Blue, Flag ;

	if( DrawFlag )
	{
		if( GAPIWin.D3D11DeviceObject == NULL )
		{
			return -1 ;
		}

		// �`��̏���
		Flag = DX_D3D11_DRAWPREP_DIFFUSERGB | DX_D3D11_DRAWPREP_3D | DX_D3D11_DRAWPREP_FOG ;
		DX_D3D11_DRAWPREP_NOTEX( Flag )

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



#define D3D11_CIRCLE_VERTEX_NUM			(2000)

#define DX_D3D11_CIRCLE_THICKNESS											\
	if( x1 < DrawRect.left      ) x1 = DrawRect.left ;						\
	if( x2 > DrawRect.right - 1 ) x2 = DrawRect.right - 1 ;					\
																			\
	VertBuf[ VertNum ].pos.x  = ( float )x1 + 0.1f ;						\
	VertBuf[ VertNum ].pos.y  = y1f + 0.1f ;								\
	VertBuf[ VertNum ].color = Color ;										\
	VertBuf[ VertNum ].rhw   = 1.0f ;										\
	*((DWORD *)&VertBuf[ VertNum ].pos.z) = drawz ;							\
	VertNum ++ ;															\
																			\
	VertBuf[ VertNum ].pos.x =  ( float )( x2 + 1 ) + 0.1f ;				\
	VertBuf[ VertNum ].pos.y =  y1f + 0.1f ;								\
	VertBuf[ VertNum ].color = Color ;										\
	VertBuf[ VertNum ].rhw   = 1.0f ;										\
	*((DWORD *)&VertBuf[ VertNum ].pos.z) = drawz ;							\
	VertNum  ++ ;															\
																			\
	if( VertNum >= D3D11_CIRCLE_VERTEX_NUM )								\
	{																		\
		Graphics_D3D11_CommonBuffer_DrawPrimitive( D3D11_VERTEX_INPUTLAYOUT_NOTEX_2D, D_D3D11_PRIMITIVE_TOPOLOGY_LINELIST, VertBuf, VertNum ) ;\
		VertNum = 0 ;														\
	}																		

// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawCircle( �����w�肠�� )
extern	int		Graphics_D3D11_DrawCircle_Thickness( int x, int y, int r, unsigned int Color, int Thickness )
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

	if( GAPIWin.D3D11DeviceObject == NULL )
	{
		return -1 ;
	}

	// �`��p�������̊m��
	if( Graphics_Other_AllocCommonBuffer( 0, sizeof( VERTEX_NOTEX_2D ) * D3D11_CIRCLE_VERTEX_NUM ) < 0 )
	{
		return -1 ;
	}
	VertBuf = ( VERTEX_NOTEX_2D * )GSYS.Resource.CommonBuffer[ 0 ] ;

	// �`��̏���
	Flag = DX_D3D11_DRAWPREP_DIFFUSERGB ;
	DX_D3D11_DRAWPREP_NOTEX( Flag )

	// �y�o�b�t�@�ɏ������ނy�l���Z�b�g����
	drawz = *((DWORD *)&GSYS.DrawSetting.DrawZ);

	// �␳
	DrawRect = GSYS.DrawSetting.DrawArea ;

	NS_GetColor2( Color, &Red, &Green, &Blue ) ;
	SETUPCOLOR( Color )

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
			DX_D3D11_CIRCLE_THICKNESS
		}
		else
		{
			x1 = CirclePos[ i ][ 0 ] ;
			x2 = CirclePos[ i ][ 2 ] ;
			DX_D3D11_CIRCLE_THICKNESS

			x1 = CirclePos[ i ][ 3 ] ;
			x2 = CirclePos[ i ][ 1 ] ;
			DX_D3D11_CIRCLE_THICKNESS
		}
	}
	if( VertNum )
	{
		Graphics_D3D11_CommonBuffer_DrawPrimitive( D3D11_VERTEX_INPUTLAYOUT_NOTEX_2D, D_D3D11_PRIMITIVE_TOPOLOGY_LINELIST, VertBuf, VertNum ) ;
	}

	// �I��
	return 0 ;
}

// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawOval( �����w�肠�� )
extern	int		Graphics_D3D11_DrawOval_Thickness( int x, int y, int rx, int ry, unsigned int Color, int Thickness )
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

	if( GAPIWin.D3D11DeviceObject == NULL )
	{
		return -1 ;
	}

	// �`��p�������̊m��
	if( Graphics_Other_AllocCommonBuffer( 0, sizeof( VERTEX_NOTEX_2D ) * D3D11_CIRCLE_VERTEX_NUM ) < 0 )
	{
		return -1 ;
	}
	VertBuf = ( VERTEX_NOTEX_2D * )GSYS.Resource.CommonBuffer[ 0 ] ;

	// �`��̏���
	Flag = DX_D3D11_DRAWPREP_DIFFUSERGB ;
	DX_D3D11_DRAWPREP_NOTEX( Flag )

	// �y�o�b�t�@�ɏ������ނy�l���Z�b�g����
	drawz = *((DWORD *)&GSYS.DrawSetting.DrawZ);

	// �␳
	DrawRect = GSYS.DrawSetting.DrawArea ;

	NS_GetColor2( Color, &Red, &Green, &Blue ) ;
	SETUPCOLOR( Color )

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
			DX_D3D11_CIRCLE_THICKNESS
		}
		else
		{
			x1 = CirclePos[ i ][ 0 ] ;
			x2 = CirclePos[ i ][ 2 ] ;
			DX_D3D11_CIRCLE_THICKNESS

			x1 = CirclePos[ i ][ 3 ] ;
			x2 = CirclePos[ i ][ 1 ] ;
			DX_D3D11_CIRCLE_THICKNESS
		}
	}
	if( VertNum )
	{
		Graphics_D3D11_CommonBuffer_DrawPrimitive( D3D11_VERTEX_INPUTLAYOUT_NOTEX_2D, D_D3D11_PRIMITIVE_TOPOLOGY_LINELIST, VertBuf, VertNum ) ;
	}

	// �I��
	return 0 ;
}




#define DX_D3D11_CIRCLE																	\
			if( !( ( x2 < DrawRect.left ) || ( x1 >= DrawRect.right  ) ||				\
			       ( y1 < DrawRect.top  ) || ( y1 >= DrawRect.bottom ) || LineDrawBuf[ y1 ] ) )		\
			{																			\
				if( x1 < DrawRect.left ) x1 = DrawRect.left ;							\
				if( x2 > DrawRect.right - 1 ) x2 = DrawRect.right - 1 ;					\
																						\
				VertBuf[ VertNum ].pos.x  = ( float )x1 + 0.1f ;						\
				VertBuf[ VertNum ].pos.y  = ( float )y1 + 0.2f ;						\
				VertBuf[ VertNum ].color = Color ;										\
				VertBuf[ VertNum ].rhw   = 1.0f ;										\
				*((DWORD *)&VertBuf[ VertNum ].pos.z) = drawz ;							\
				VertNum ++ ;															\
				LineDrawBuf[y1] = TRUE ;												\
																						\
				VertBuf[ VertNum ].pos.x =  ( float )( x2 + 1 ) + 0.1f ;				\
				VertBuf[ VertNum ].pos.y =  ( float )y1 + 0.2f ;						\
				VertBuf[ VertNum ].color = Color ;										\
				VertBuf[ VertNum ].rhw   = 1.0f ;										\
				*((DWORD *)&VertBuf[ VertNum ].pos.z) = drawz ;							\
				VertNum  ++ ;															\
																						\
				if( VertNum >= D3D11_CIRCLE_VERTEX_NUM )								\
				{																		\
					Graphics_D3D11_CommonBuffer_DrawPrimitive( D3D11_VERTEX_INPUTLAYOUT_NOTEX_2D, D_D3D11_PRIMITIVE_TOPOLOGY_LINELIST, VertBuf, VertNum ) ;\
					VertNum = 0 ;														\
				}																		\
			}

#define DX_D3D11_CIRCLE_PSET															\
			if( !( ( x1 < DrawRect.left ) || ( x1 >= DrawRect.right  ) ||				\
				   ( y1 < DrawRect.top  ) || ( y1 >= DrawRect.bottom ) ) )				\
			{																			\
				VertBuf[ VertNum ].pos.x  = ( float )x1 + 0.1f ;						\
				VertBuf[ VertNum ].pos.y  = ( float )y1 + 0.1f ;						\
				VertBuf[ VertNum ].color = Color ;										\
				VertBuf[ VertNum ].rhw   = 1.0f ;										\
				*((DWORD *)&VertBuf[ VertNum ].pos.z) = drawz ;							\
				VertNum ++ ;															\
																						\
				if( VertNum >= D3D11_CIRCLE_VERTEX_NUM )								\
				{																		\
					Graphics_D3D11_CommonBuffer_DrawPrimitive( D3D11_VERTEX_INPUTLAYOUT_NOTEX_2D, D_D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, VertBuf, VertNum ) ;\
					VertNum = 0 ;														\
				}																		\
			}

// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawCircle
extern	int		Graphics_D3D11_DrawCircle( int x, int y, int r, unsigned int Color, int FillFlag )
{
	VERTEX_NOTEX_2D *VertBuf ;
	int             VertNum ;
	BYTE            *LineDrawBuf ;
	int             Red ;
	int             Green ;
	int             Blue ;
	int             Flag ;
	RECT            DrawRect ;
	DWORD           drawz ;

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	if( GAPIWin.D3D11DeviceObject == NULL )
	{
		return -1 ;
	}

	// �`��p�������̊m��
	if( Graphics_Other_AllocCommonBuffer( 0, sizeof( VERTEX_NOTEX_2D ) * D3D11_CIRCLE_VERTEX_NUM ) < 0 )
	{
		return -1 ;
	}
	VertBuf = ( VERTEX_NOTEX_2D * )GSYS.Resource.CommonBuffer[ 0 ] ;

	// �`��̏���
	Flag = DX_D3D11_DRAWPREP_DIFFUSERGB ;
	DX_D3D11_DRAWPREP_NOTEX( Flag )

	// �y�o�b�t�@�ɏ������ނy�l���Z�b�g����
	drawz = *((DWORD *)&GSYS.DrawSetting.DrawZ);

	// �␳
	DrawRect = GSYS.DrawSetting.DrawArea ;

	NS_GetColor2( Color, &Red, &Green, &Blue ) ;
	SETUPCOLOR( Color )

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
				DX_D3D11_CIRCLE ;

				// ���W�f�[�^��i�߂�
				if( F >= 0 )
				{
					x2 = Dy + x ; x1 = -Dy + x ; y1 = Dx + y ;
					DX_D3D11_CIRCLE ;
					x2 = Dy + x ; x1 = -Dy + x ; y1 = -Dx + y;
					DX_D3D11_CIRCLE ;

					Dx -- ;
					F -= 4 * Dx ;
				}	

				Dy ++ ;
				F += 4 * Dy + 2 ;
			}

			while( Dx >= Dy )
			{
				x2 = Dx + x ; x1 = -Dx + x ; y1 = Dy + y ;
				DX_D3D11_CIRCLE ;
				x2 = Dx + x ; x1 = -Dx + x ; y1 = -Dy + y;
				DX_D3D11_CIRCLE ;

				// ���W�f�[�^��i�߂�
				if( F >= 0 )
				{
					x2 = Dy + x ; x1 = -Dy + x ; y1 = Dx + y ;
					DX_D3D11_CIRCLE ;
					x2 = Dy + x ; x1 = -Dy + x ; y1 = -Dx + y;
					DX_D3D11_CIRCLE ;

					Dx -- ;
					F -= 4 * Dx ;
				}

				Dy ++ ;
				F += 4 * Dy + 2 ;
			}
			if( VertNum )
			{
				Graphics_D3D11_CommonBuffer_DrawPrimitive( D3D11_VERTEX_INPUTLAYOUT_NOTEX_2D, D_D3D11_PRIMITIVE_TOPOLOGY_LINELIST, VertBuf, VertNum ) ;
			}
		}
		else
		{
			// �ŏ��̓_��`��
			{
				x1 = -Dx + x ; y1 = Dy + y ;
				DX_D3D11_CIRCLE_PSET ;
				x1 = Dx + x ;  
				DX_D3D11_CIRCLE_PSET ;

				x1 = Dy + x ; y1 = Dx + y ;
				DX_D3D11_CIRCLE_PSET ;

				x1 = Dy + x ; y1 = -Dx + y;
				DX_D3D11_CIRCLE_PSET ;

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
				DX_D3D11_CIRCLE_PSET ;
				x1 = Dx + x ;  
				DX_D3D11_CIRCLE_PSET ;

				x1 = Dy + x ; y1 = Dx + y ;
				DX_D3D11_CIRCLE_PSET ;
				x1 = -Dy + x ; 
				DX_D3D11_CIRCLE_PSET ;
			
				x1 = Dx + x ; y1 = -Dy + y;
				DX_D3D11_CIRCLE_PSET ;
				x1 = -Dx + x ; 
				DX_D3D11_CIRCLE_PSET ;

				x1 = Dy + x ; y1 = -Dx + y;
				DX_D3D11_CIRCLE_PSET ;
				x1 = -Dy + x ; 
				DX_D3D11_CIRCLE_PSET ;

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
				Graphics_D3D11_CommonBuffer_DrawPrimitive( D3D11_VERTEX_INPUTLAYOUT_NOTEX_2D, D_D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, VertBuf, VertNum ) ;
			}
		}
	}

	// �I��
	return 0 ;
}

// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawOval
extern	int		Graphics_D3D11_DrawOval( int x, int y, int rx, int ry, unsigned int Color, int FillFlag )
{
	VERTEX_NOTEX_2D *VertBuf ;
	int VertNum ;
	BYTE *LineDrawBuf ;
//	VERTEX_NOTEX_2D *VertData ;
//	int i, j, r2 ;
//	float xd, x2, y2 ;
//	float d, yd ;
	int Red, Green, Blue, Flag ;
	RECT DrawRect ;
	DWORD drawz;

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	if( GAPIWin.D3D11DeviceObject == NULL )
	{
		return -1 ;
	}
	if( !rx || !ry ) return -1 ;

	// �`��p�������̊m��
	if( Graphics_Other_AllocCommonBuffer( 0, sizeof( VERTEX_NOTEX_2D ) * D3D11_CIRCLE_VERTEX_NUM ) < 0 )
	{
		return -1 ;
	}
	VertBuf = ( VERTEX_NOTEX_2D * )GSYS.Resource.CommonBuffer[ 0 ] ;

	// �`��̏���
	Flag = DX_D3D11_DRAWPREP_DIFFUSERGB ;
	DX_D3D11_DRAWPREP_NOTEX( Flag )

	// �␳
	if( rx < 0 ) rx *= -1 ;
	if( ry < 0 ) ry *= -1 ;
	DrawRect = GSYS.DrawSetting.DrawArea ;

	NS_GetColor2( Color, &Red, &Green, &Blue ) ;
	SETUPCOLOR( Color )

	// �y�o�b�t�@�ɏ������ނy�l���Z�b�g����
	drawz = *((DWORD *)&GSYS.DrawSetting.DrawZ);

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
					DX_D3D11_CIRCLE ;

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
						DX_D3D11_CIRCLE ;

						y1 = -Dy + y ;
						DX_D3D11_CIRCLE ;
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
				DX_D3D11_CIRCLE ;
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
					DX_D3D11_CIRCLE ;

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
						DX_D3D11_CIRCLE ;

						y1 = -Dy + y ;
						DX_D3D11_CIRCLE ;
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
				DX_D3D11_CIRCLE ;
			}
			if( VertNum )
			{
				Graphics_D3D11_CommonBuffer_DrawPrimitive( D3D11_VERTEX_INPUTLAYOUT_NOTEX_2D, D_D3D11_PRIMITIVE_TOPOLOGY_LINELIST, VertBuf, VertNum ) ;
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
					DX_D3D11_CIRCLE_PSET ;
					x1 = Dx + x ;  
					DX_D3D11_CIRCLE_PSET ;

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
						DX_D3D11_CIRCLE_PSET ;
						x1 = Dx + x ;  
						DX_D3D11_CIRCLE_PSET ;

						x1 = -Dx + x ; y1 = -Dy + y ;
						DX_D3D11_CIRCLE_PSET ;
						x1 = Dx + x ;  
						DX_D3D11_CIRCLE_PSET ;
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
				DX_D3D11_CIRCLE_PSET ;

				x1 = -Dx + x ; y1 = Dy + y ;
				DX_D3D11_CIRCLE_PSET ;
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
					DX_D3D11_CIRCLE_PSET ;
					x1 = Dx + x ;  
					DX_D3D11_CIRCLE_PSET ;

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
						DX_D3D11_CIRCLE_PSET ;
						x1 = Dx + x ;  
						DX_D3D11_CIRCLE_PSET ;

						x1 = -Dx + x ; y1 = -Dy + y ;
						DX_D3D11_CIRCLE_PSET ;
						x1 = Dx + x ;  
						DX_D3D11_CIRCLE_PSET ;
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
				DX_D3D11_CIRCLE_PSET ;

				x1 = -Dx + x ; y1 = Dy + y ;
				DX_D3D11_CIRCLE_PSET ;
			}
			if( VertNum )
			{
				Graphics_D3D11_CommonBuffer_DrawPrimitive( D3D11_VERTEX_INPUTLAYOUT_NOTEX_2D, D_D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, VertBuf, VertNum ) ;
			}
		}
	}

	// �I��
	return 0 ;
}

// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawTriangle
extern	int		Graphics_D3D11_DrawTriangle( int x1, int y1, int x2, int y2, int x3, int y3, unsigned int Color, int FillFlag )
{
	VERTEX_NOTEX_2D *vec ; 
	int Red, Green, Blue ;
	int Flag ;
	int sx1, sx2, sy1, sy2 ;
	DWORD drawz;

	if( GAPIWin.D3D11DeviceObject == NULL )
	{
		return -1 ;
	}

	// �`��̏���
	Flag = DX_D3D11_DRAWPREP_DIFFUSERGB ;
	DX_D3D11_DRAWPREP_NOTEX( Flag )

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
		vec[ 1 ].color = Color ;
		vec[ 2 ].color = Color ;

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

		vec[ 0 ].rhw = 1.0f ;
		vec[ 1 ].rhw = 1.0f ;
		vec[ 2 ].rhw = 1.0f ;

		ADD3VERTEX_NOTEX
	}
	else
	{
		GETVERTEX_LINETRIANGLE( vec )

		vec[ 0 ].color = Color ;
		vec[ 1 ].color = Color ;
		vec[ 2 ].color = Color ;
		vec[ 3 ].color = Color ;
		vec[ 4 ].color = Color ;
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

		vec[ 0 ].rhw = 1.0f ;
		vec[ 1 ].rhw = 1.0f ;
		vec[ 2 ].rhw = 1.0f ;
		vec[ 3 ].rhw = 1.0f ;
		vec[ 4 ].rhw = 1.0f ;
		vec[ 5 ].rhw = 1.0f ;

		ADD4VERTEX_LINETRIANGLE
	}

	// �I��
	return 0 ;
}

// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawTriangle3D
extern	int		Graphics_D3D11_DrawTriangle3D( VECTOR Pos1, VECTOR Pos2, VECTOR Pos3, unsigned int Color, int FillFlag, int DrawFlag, RECT *DrawArea )
{
	VERTEX_NOTEX_3D *vec ; 
	int Red, Green, Blue ;
	int Flag ;

	if( DrawFlag )
	{
		if( GAPIWin.D3D11DeviceObject == NULL )
		{
			return -1 ;
		}

		// �`��̏���
		Flag = DX_D3D11_DRAWPREP_DIFFUSERGB | DX_D3D11_DRAWPREP_3D | DX_D3D11_DRAWPREP_FOG | DX_D3D11_DRAWPREP_CULLING ;
		DX_D3D11_DRAWPREP_NOTEX( Flag )

		// �F���̑��X�e�[�^�X�̃Z�b�g
		NS_GetColor2( Color, &Red, &Green, &Blue ) ;
		SETUPCOLOR( Color )

		// ���C�����ǂ����ŏ����𕪊�
		if( FillFlag )
		{
			GETVERTEX_TRIANGLE3D( vec ) ;

			*( ( DWORD * )&vec[ 0 ].b ) = ( DWORD )Color ;
			*( ( DWORD * )&vec[ 1 ].b ) = ( DWORD )Color ;
			*( ( DWORD * )&vec[ 2 ].b ) = ( DWORD )Color ;
			vec[ 0 ].pos = Pos1 ;
			vec[ 1 ].pos = Pos2 ;
			vec[ 2 ].pos = Pos3 ;

			ADD3VERTEX_NOTEX3D
		}
		else
		{
			GETVERTEX_LINETRIANGLE3D( vec )

			*( ( DWORD * )&vec[ 0 ].b ) = ( DWORD )Color ;
			*( ( DWORD * )&vec[ 1 ].b ) = ( DWORD )Color ;
			*( ( DWORD * )&vec[ 2 ].b ) = ( DWORD )Color ;
			*( ( DWORD * )&vec[ 3 ].b ) = ( DWORD )Color ;
			*( ( DWORD * )&vec[ 4 ].b ) = ( DWORD )Color ;
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
extern	int		Graphics_D3D11_DrawQuadrangle( int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, unsigned int Color, int FillFlag )
{
	VERTEX_NOTEX_2D *vec ;
	int Red, Green, Blue ;
	int Flag ;
	DWORD drawz;

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	if( GAPIWin.D3D11DeviceObject == NULL )
	{
		return -1 ;
	}

	// �`��̏���
	Flag = DX_D3D11_DRAWPREP_DIFFUSERGB ;
	DX_D3D11_DRAWPREP_NOTEX( Flag )

	// �y�o�b�t�@�ɏ������ނy�l���Z�b�g����
	drawz = *((DWORD *)&GSYS.DrawSetting.DrawZ);

	// �F���̑��X�e�[�^�X�̃Z�b�g
	NS_GetColor2( Color, &Red, &Green, &Blue ) ;
	SETUPCOLOR( Color )

	// �h��Ԃ����ǂ����ŏ����𕪊�
	if( FillFlag )
	{
		// �h��Ԃ��̏ꍇ

		// ���_�f�[�^�̃Z�b�g
		GETVERTEX_QUAD( vec )

		vec[ 0 ].color = Color ;
		vec[ 1 ].color = Color ;
		vec[ 2 ].color = Color ;
		vec[ 3 ].color = Color ;
		vec[ 4 ].color = Color ;
		vec[ 5 ].color = Color ;
		
		vec[ 0 ].pos.x = ( float )x1 ; vec[ 0 ].pos.y = ( float )y1 ;
		vec[ 1 ].pos.x = ( float )x2 ; vec[ 1 ].pos.y = ( float )y2 ;
		vec[ 2 ].pos.x = ( float )x4 ; vec[ 2 ].pos.y = ( float )y4 ;
		vec[ 3 ].pos.x = ( float )x3 ; vec[ 3 ].pos.y = ( float )y3 ;
		vec[ 4 ].pos.x = ( float )x4 ; vec[ 4 ].pos.y = ( float )y4 ;
		vec[ 5 ].pos.x = ( float )x2 ; vec[ 5 ].pos.y = ( float )y2 ;

		*((DWORD *)&vec[ 0 ].pos.z) = drawz ;
		*((DWORD *)&vec[ 1 ].pos.z) = drawz ;
		*((DWORD *)&vec[ 2 ].pos.z) = drawz ;
		*((DWORD *)&vec[ 3 ].pos.z) = drawz ;
		*((DWORD *)&vec[ 4 ].pos.z) = drawz ;
		*((DWORD *)&vec[ 5 ].pos.z) = drawz ;

		vec[0].rhw = 1.0f ;
		vec[1].rhw = 1.0f ;
		vec[2].rhw = 1.0f ;
		vec[3].rhw = 1.0f ;
		vec[4].rhw = 1.0f ;
		vec[5].rhw = 1.0f ;

		// ���_�f�[�^�̏o��
		ADD4VERTEX_NOTEX
	}
	else
	{
		// �h��Ԃ��ł͖����ꍇ

		GETVERTEX_LINE( vec ) ;

		vec[ 0 ].color = Color ;
		vec[ 1 ].color = Color ;

		*((DWORD *)&vec[ 0 ].pos.z) = drawz ;
		*((DWORD *)&vec[ 1 ].pos.z) = drawz ;

		vec[ 0 ].rhw = 1.0f ;
		vec[ 1 ].rhw = 1.0f ;

		vec[ 0 ].pos.x = ( float )( x1 ) ;
		vec[ 0 ].pos.y = ( float )( y1 ) ;

		vec[ 1 ].pos.x = ( float )( x2 ) ;
		vec[ 1 ].pos.y = ( float )( y2 ) ;

		// ���_�̒ǉ�
		ADD4VERTEX_LINE


		GETVERTEX_LINE( vec ) ;

		vec[ 0 ].color = Color ;
		vec[ 1 ].color = Color ;

		*((DWORD *)&vec[ 0 ].pos.z) = drawz ;
		*((DWORD *)&vec[ 1 ].pos.z) = drawz ;

		vec[ 0 ].rhw = 1.0f ;
		vec[ 1 ].rhw = 1.0f ;

		vec[ 0 ].pos.x = ( float )( x2 ) ;
		vec[ 0 ].pos.y = ( float )( y2 ) ;

		vec[ 1 ].pos.x = ( float )( x3 ) ;
		vec[ 1 ].pos.y = ( float )( y3 ) ;

		// ���_�̒ǉ�
		ADD4VERTEX_LINE


		GETVERTEX_LINE( vec ) ;

		vec[ 0 ].color = Color ;
		vec[ 1 ].color = Color ;

		*((DWORD *)&vec[ 0 ].pos.z) = drawz ;
		*((DWORD *)&vec[ 1 ].pos.z) = drawz ;

		vec[ 0 ].rhw = 1.0f ;
		vec[ 1 ].rhw = 1.0f ;

		vec[ 0 ].pos.x = ( float )( x3 ) ;
		vec[ 0 ].pos.y = ( float )( y3 ) ;

		vec[ 1 ].pos.x = ( float )( x4 ) ;
		vec[ 1 ].pos.y = ( float )( y4 ) ;

		// ���_�̒ǉ�
		ADD4VERTEX_LINE


		GETVERTEX_LINE( vec ) ;

		vec[ 0 ].color = Color ;
		vec[ 1 ].color = Color ;

		*((DWORD *)&vec[ 0 ].pos.z) = drawz ;
		*((DWORD *)&vec[ 1 ].pos.z) = drawz ;

		vec[ 0 ].rhw = 1.0f ;
		vec[ 1 ].rhw = 1.0f ;

		vec[ 0 ].pos.x = ( float )( x4 ) ;
		vec[ 0 ].pos.y = ( float )( y4 ) ;

		vec[ 1 ].pos.x = ( float )( x1 ) ;
		vec[ 1 ].pos.y = ( float )( y1 ) ;

		// ���_�̒ǉ�
		ADD4VERTEX_LINE
	}

	// �I��
	return 0 ;
}

// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawPixel
extern	int		Graphics_D3D11_DrawPixel( int x, int y, unsigned int Color )
{
	VERTEX_NOTEX_2D *VertData ;
	int Red, Green, Blue, Flag ;

	if( GAPIWin.D3D11DeviceObject == NULL )
	{
		return -1 ; 
	}

	// �`��̏���
	Flag = DX_D3D11_DRAWPREP_DIFFUSERGB ;
	DX_D3D11_DRAWPREP_NOTEX( Flag )

	// �F���̑��X�e�[�^�X�̃Z�b�g
	GETVERTEX_POINT( VertData ) ;

	if( GD3D11.Device.DrawInfo.DiffuseColor == 0xffffffff )
	{
		NS_GetColor2( Color, &Red, &Green, &Blue ) ;
		if( GSYS.HardInfo.UseVertexColorBGRAFormat )
		{
			VertData->color = 0xff000000 | ( Red << 16 ) | ( Green << 8 ) | ( Blue ) ;
		}
		else
		{
			VertData->color = 0xff000000 | ( Red ) | ( Green << 8 ) | ( Blue << 16 ) ;
		}
	}
	else
	{
		NS_GetColor2( Color, &Red, &Green, &Blue ) ;
		SETUPCOLOR( VertData->color )
	}

	// ���W�̃Z�b�g
	VertData->pos.x = ( float )x + 0.1f ;
	VertData->pos.y = ( float )y + 0.1f ;
	VertData->pos.z = GSYS.DrawSetting.DrawZ ;
	VertData->rhw   = 1.0f ;

	// ���_�̒ǉ�
	ADD4VERTEX_POINT

	// �I��
	return 0 ;
}

// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawPixel3D
extern	int		Graphics_D3D11_DrawPixel3D( VECTOR Pos, unsigned int Color, int DrawFlag, RECT *DrawArea )
{
	VERTEX_NOTEX_3D *VertData ; 
	int Red, Green, Blue ;
	int Flag ;

	if( DrawFlag )
	{
		if( GAPIWin.D3D11DeviceObject == NULL )
		{
			return -1 ;
		}

		// �`��̏���
		Flag = DX_D3D11_DRAWPREP_DIFFUSERGB | DX_D3D11_DRAWPREP_3D | DX_D3D11_DRAWPREP_FOG ;
		DX_D3D11_DRAWPREP_NOTEX( Flag )

		// �F���̑��X�e�[�^�X�̃Z�b�g
		GETVERTEX_POINT3D( VertData ) ;

		NS_GetColor2( Color, &Red, &Green, &Blue ) ;
		if( GD3D11.Device.DrawInfo.DiffuseColor == 0xffffffff )
		{
			if( GSYS.HardInfo.UseVertexColorBGRAFormat )
			{
				*( ( DWORD * )&VertData->b ) = 0xff000000 | ( Red << 16 ) | ( Green << 8 ) | ( Blue ) ;
			}
			else
			{
				*( ( DWORD * )&VertData->b ) = 0xff000000 | ( Red ) | ( Green << 8 ) | ( Blue << 16 ) ;
			}
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
extern	int		Graphics_D3D11_DrawPixelSet( const POINTDATA *PointData, int Num )
{
	VERTEX_NOTEX_2D *VertData ;
	int i ;
	COLORDATA ColorData ;
	int MaxRed, MaxGreen, MaxBlue ; 
	unsigned int Color ;
	int Flag ;
	DWORD drawz;

	if( GAPIWin.D3D11DeviceObject == NULL )
	{
		return -1 ;
	}

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �`��̏���
	Flag = DX_D3D11_DRAWPREP_DIFFUSERGB ;
	DX_D3D11_DRAWPREP_NOTEX( Flag )

	// �y�o�b�t�@�ɏ������ނy�l���Z�b�g����
	drawz = *((DWORD *)&GSYS.DrawSetting.DrawZ);

	// �x�N�g����{�X�e�[�^�X���擾
	ColorData = *( ( COLORDATA * )NS_GetDispColorData() ) ;
	
	MaxRed		= ( 1 << ColorData.RedWidth	  ) - 1 ; 
	MaxGreen	= ( 1 << ColorData.GreenWidth ) - 1 ;
	MaxBlue		= ( 1 << ColorData.BlueWidth  ) - 1 ; 

	for( i = 0 ; i < Num ; i ++, PointData ++ )
	{
		GETVERTEX_POINT( VertData ) ;

		Color = PointData->color ;
		VertData->color = ( PointData->pal << 24 ) |
						( ( ( ( ( ( Color & ColorData.RedMask	) >> ColorData.RedLoc	) << 8 ) - 1 ) / MaxRed		)       ) |
						( ( ( ( ( ( Color & ColorData.GreenMask	) >> ColorData.GreenLoc	) << 8 ) - 1 ) / MaxGreen	) << 8  ) |
						( ( ( ( ( ( Color & ColorData.BlueMask	) >> ColorData.BlueLoc	) << 8 ) - 1 ) / MaxBlue	) << 16 ) ; 

		VertData->pos.x = ( float )PointData->x + 0.1f ;
		VertData->pos.y = ( float )PointData->y + 0.1f ;
		*((DWORD *)&VertData->pos.z) = drawz ;
		VertData->rhw    = 1.0f ;

		// ���_�̒ǉ�
		ADD4VERTEX_POINT
	}

	// �I��
	return 0;
}

// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawLineSet
extern	int		Graphics_D3D11_DrawLineSet( const LINEDATA *LineData, int Num )
{
	int i ;
	VERTEX_NOTEX_2D *VertData ;
	COLORDATA ColorData ;
	int MaxRed, MaxGreen, MaxBlue ; 
	unsigned int Color ;
	int Flag ;
	DWORD drawz;

	if( GAPIWin.D3D11DeviceObject == NULL )
	{
		return -1 ;
	}

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �`��̏���
	Flag = DX_D3D11_DRAWPREP_DIFFUSERGB ;
	DX_D3D11_DRAWPREP_NOTEX( Flag )

	// �y�o�b�t�@�ɏ������ނy�l���Z�b�g����
	drawz = *((DWORD *)&GSYS.DrawSetting.DrawZ);

	// �x�N�g����{�X�e�[�^�X���擾
	ColorData = *( ( COLORDATA * )NS_GetDispColorData() ) ;
	
	MaxRed		= ( 1 << ColorData.RedWidth	) - 1 ; 
	MaxGreen	= ( 1 << ColorData.GreenWidth ) - 1 ;
	MaxBlue		= ( 1 << ColorData.BlueWidth ) - 1 ; 

	for( i = 0 ; i < Num ; i ++, LineData ++ )
	{
		GETVERTEX_LINE( VertData ) ;

		Color = LineData->color ;
		VertData->color = ( LineData->pal << 24 ) |
						( ( ( ( ( ( Color & ColorData.RedMask	) >> ColorData.RedLoc	) << 8 ) - 1 ) / MaxRed		)       ) |
						( ( ( ( ( ( Color & ColorData.GreenMask	) >> ColorData.GreenLoc	) << 8 ) - 1 ) / MaxGreen	) << 8  ) |
						( ( ( ( ( ( Color & ColorData.BlueMask	) >> ColorData.BlueLoc	) << 8 ) - 1 ) / MaxBlue	) << 16 ) ; 
		VertData[1].color = VertData[0].color ;
		*((DWORD *)&VertData[0].pos.z) = drawz ;
		*((DWORD *)&VertData[1].pos.z) = drawz ;
		VertData[0].rhw   = 1.0f ;
		VertData[1].rhw   = 1.0f ;
 
		// ���W�̃Z�b�g���E�C���h�E���ڕ`�掞�␳
		VertData[0].pos.x = ( float )LineData->x1 ;
		VertData[0].pos.y = ( float )LineData->y1 ;
		VertData[1].pos.x = ( float )LineData->x2 ;
		VertData[1].pos.y = ( float )LineData->y2 ;

		// ���W�̕␳
		if( LineData->x1 == LineData->x2 ) VertData[ 1 ].pos.y += LineData->y2 > LineData->y1 ? -0.1F : 0.1F ;
		if( LineData->y1 == LineData->y2 ) VertData[ 1 ].pos.x += LineData->x2 > LineData->x1 ? -0.1F : 0.1F ;

		// ���_�̒ǉ�
		ADD4VERTEX_LINE
	}

	// �I��
	return 0 ;
}














// DrawPrimitive3D �n�̋��ʂ̏����������s���֐�
__inline int Graphics_D3D11_DrawPrimitive3DPreparation( int AddFlag, IMAGEDATA *Image, int TransFlag, int TextureNo = 0 )
{
	int Flag ;

	if( GAPIWin.D3D11DeviceObject == NULL )
	{
		return -1 ;
	}

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �`��̏���
	Flag = TransFlag | DX_D3D11_DRAWPREP_3D | DX_D3D11_DRAWPREP_FOG | DX_D3D11_DRAWPREP_TEXADDRESS | DX_D3D11_DRAWPREP_CULLING | AddFlag ;
	if( Image )
	{
		DX_D3D11_DRAWPREP_TEX( Image->Orig, Image->Hard.Draw[ TextureNo ].Tex->PF->D3D11.Texture, Image->Hard.Draw[ TextureNo ].Tex->PF->D3D11.TextureSRV, Flag )
	}
	else
	{
		int WhiteHandle ;

		WhiteHandle = Graphics_Image_GetWhiteTexHandle() ;
		if( GRAPHCHK( WhiteHandle, Image ) )
		{
			return -1 ;
		}
//		DX_D3D11_DRAWPREP_NOTEX( Flag )
		DX_D3D11_DRAWPREP_TEX( Image->Orig, Image->Hard.Draw[ 0 ].Tex->PF->D3D11.Texture, Image->Hard.Draw[ 0 ].Tex->PF->D3D11.TextureSRV, Flag )
	}

	return 0 ;
}

extern	int		Graphics_D3D11_DrawPrimitive( const VERTEX_3D *Vertex, int VertexNum, int PrimitiveType, IMAGEDATA *Image, int TransFlag )
{
	int i ;
	VERTEX_3D *VertP ;
	unsigned char tmp ;

	if( Graphics_D3D11_DrawPrimitive3DPreparation( 0, Image, TransFlag ) < 0 )
	{
		return -1 ;
	}

	if( GSYS.HardInfo.UseVertexColorBGRAFormat == FALSE )
	{
		VertP = ( VERTEX_3D * )Vertex ;
		for( i = 0 ; i < VertexNum ; i ++, VertP ++ )
		{
			tmp = VertP->b ;
			VertP->b = VertP->r ;
			VertP->r = tmp ;
		}
	}

	// �`��
	Graphics_D3D11_CommonBuffer_DrawPrimitive(
		D3D11_VERTEX_INPUTLAYOUT_3D,
		( D_D3D11_PRIMITIVE_TOPOLOGY )PrimitiveType,
		Vertex,
		VertexNum
	) ;

	if( GSYS.HardInfo.UseVertexColorBGRAFormat == FALSE )
	{
		VertP = ( VERTEX_3D * )Vertex ;
		for( i = 0 ; i < VertexNum ; i ++, VertP ++ )
		{
			tmp = VertP->b ;
			VertP->b = VertP->r ;
			VertP->r = tmp ;
		}
	}

	// �I��
	return 0 ;
}

extern	int		Graphics_D3D11_DrawIndexedPrimitive( const VERTEX_3D *Vertex, int VertexNum, const WORD *Indices, int IndexNum, int PrimitiveType, IMAGEDATA *Image, int TransFlag )
{
	int i ;
	VERTEX_3D *VertP ;
	unsigned char tmp ;

	if( Graphics_D3D11_DrawPrimitive3DPreparation( 0, Image, TransFlag ) < 0 )
	{
		return -1 ;
	}

	if( GSYS.HardInfo.UseVertexColorBGRAFormat == FALSE )
	{
		VertP = ( VERTEX_3D * )Vertex ;
		for( i = 0 ; i < VertexNum ; i ++, VertP ++ )
		{
			tmp = VertP->b ;
			VertP->b = VertP->r ;
			VertP->r = tmp ;
		}
	}

	// �`��
	Graphics_D3D11_CommonBuffer_DrawIndexedPrimitive(
		D3D11_VERTEX_INPUTLAYOUT_3D,
		( D_D3D11_PRIMITIVE_TOPOLOGY )PrimitiveType,
		Vertex,
		VertexNum,
		Indices,
		IndexNum,
		D_DXGI_FORMAT_R16_UINT
	) ;

	if( GSYS.HardInfo.UseVertexColorBGRAFormat == FALSE )
	{
		VertP = ( VERTEX_3D * )Vertex ;
		for( i = 0 ; i < VertexNum ; i ++, VertP ++ )
		{
			tmp = VertP->b ;
			VertP->b = VertP->r ;
			VertP->r = tmp ;
		}
	}

	// �I��
	return 0 ;
}

extern	int		Graphics_D3D11_DrawPrimitiveLight( const VERTEX3D *Vertex, int VertexNum, int PrimitiveType, IMAGEDATA *Image, int TransFlag )
{
	int i ;
	VERTEX3D *VertP ;
	unsigned char tmp ;

	if( Graphics_D3D11_DrawPrimitive3DPreparation( DX_D3D11_DRAWPREP_LIGHTING | DX_D3D11_DRAWPREP_SPECULAR, Image, TransFlag ) < 0 )
	{
		return -1 ;
	}

	if( GSYS.HardInfo.UseVertexColorBGRAFormat == FALSE )
	{
		VertP = ( VERTEX3D * )Vertex ;
		for( i = 0 ; i < VertexNum ; i ++, VertP ++ )
		{
			tmp = VertP->dif.b ;
			VertP->dif.b = VertP->dif.r ;
			VertP->dif.r = tmp ;

			tmp = VertP->spc.b ;
			VertP->spc.b = VertP->spc.r ;
			VertP->spc.r = tmp ;
		}
	}

	// �`��
	Graphics_D3D11_CommonBuffer_DrawPrimitive(
		D3D11_VERTEX_INPUTLAYOUT_3D_LIGHT,
		( D_D3D11_PRIMITIVE_TOPOLOGY )PrimitiveType,
		Vertex,
		VertexNum
	) ;

	if( GSYS.HardInfo.UseVertexColorBGRAFormat == FALSE )
	{
		VertP = ( VERTEX3D * )Vertex ;
		for( i = 0 ; i < VertexNum ; i ++, VertP ++ )
		{
			tmp = VertP->dif.b ;
			VertP->dif.b = VertP->dif.r ;
			VertP->dif.r = tmp ;

			tmp = VertP->spc.b ;
			VertP->spc.b = VertP->spc.r ;
			VertP->spc.r = tmp ;
		}
	}

	// �I��
	return 0 ;
}

extern	int		Graphics_D3D11_DrawIndexedPrimitiveLight( const VERTEX3D *Vertex, int VertexNum, const WORD *Indices, int IndexNum, int PrimitiveType, IMAGEDATA *Image, int TransFlag )
{
	int i ;
	VERTEX3D *VertP ;
	unsigned char tmp ;

	if( Graphics_D3D11_DrawPrimitive3DPreparation( DX_D3D11_DRAWPREP_LIGHTING | DX_D3D11_DRAWPREP_SPECULAR, Image, TransFlag ) < 0 )
	{
		return -1 ;
	}

	if( GSYS.HardInfo.UseVertexColorBGRAFormat == FALSE )
	{
		VertP = ( VERTEX3D * )Vertex ;
		for( i = 0 ; i < VertexNum ; i ++, VertP ++ )
		{
			tmp = VertP->dif.b ;
			VertP->dif.b = VertP->dif.r ;
			VertP->dif.r = tmp ;

			tmp = VertP->spc.b ;
			VertP->spc.b = VertP->spc.r ;
			VertP->spc.r = tmp ;
		}
	}

	// �`��
	Graphics_D3D11_CommonBuffer_DrawIndexedPrimitive(
		D3D11_VERTEX_INPUTLAYOUT_3D_LIGHT,
		( D_D3D11_PRIMITIVE_TOPOLOGY )PrimitiveType,
		Vertex,
		VertexNum,
		Indices,
		IndexNum,
		D_DXGI_FORMAT_R16_UINT
	) ;

	if( GSYS.HardInfo.UseVertexColorBGRAFormat == FALSE )
	{
		VertP = ( VERTEX3D * )Vertex ;
		for( i = 0 ; i < VertexNum ; i ++, VertP ++ )
		{
			tmp = VertP->dif.b ;
			VertP->dif.b = VertP->dif.r ;
			VertP->dif.r = tmp ;

			tmp = VertP->spc.b ;
			VertP->spc.b = VertP->spc.r ;
			VertP->spc.r = tmp ;
		}
	}

	// �I��
	return 0 ;
}

extern	int		Graphics_D3D11_DrawPrimitiveLight_UseVertexBuffer( 
	VERTEXBUFFERHANDLEDATA *VertexBuffer,
	int						PrimitiveType,
	int						StartVertex,
	int						UseVertexNum,
	IMAGEDATA *				Image,
	int						TransFlag 
)
{
	// VERTEX3D�\���̂̂ݑΉ�
	if( VertexBuffer->Type != DX_VERTEX_TYPE_NORMAL_3D )
	{
		return -1 ;
	}

	if( Graphics_D3D11_DrawPrimitive3DPreparation( DX_D3D11_DRAWPREP_LIGHTING | DX_D3D11_DRAWPREP_SPECULAR, Image, TransFlag ) < 0 )
	{
		return -1 ;
	}

	// �v���~�e�B�u�^�C�v���Z�b�g
	Graphics_D3D11_DeviceState_SetPrimitiveTopology( ( D_D3D11_PRIMITIVE_TOPOLOGY )PrimitiveType ) ;

	// ���̓��C�A�E�g���Z�b�g
	Graphics_D3D11_DeviceState_SetInputLayout( GD3D11.Device.InputLayout.BaseInputLayout[ D3D11_VERTEX_INPUTLAYOUT_3D_LIGHT ] ) ;

	// ���_�V�F�[�_�[���Z�b�g
	if( Graphics_D3D11_Shader_Normal3DDraw_Setup() == FALSE )
	{
		return -1 ;
	}

	// ���_�o�b�t�@���Z�b�g
	Graphics_D3D11_DeviceState_SetVertexBuffer( VertexBuffer->PF->D3D11.VertexBuffer, D3D11_VertexSize[ D3D11_VERTEX_INPUTLAYOUT_3D_LIGHT ] ) ;

	// �X�e�[�g�ƒ萔�o�b�t�@�̍X�V
	Graphics_D3D11_DeviceState_SetupStateAndConstantBuffer() ;

	// �`��
	D3D11DeviceContext_Draw( ( UINT )UseVertexNum, ( UINT )StartVertex ) ;

	// �I��
	return 0 ;
}

extern	int		Graphics_D3D11_DrawIndexedPrimitiveLight_UseVertexBuffer(
	VERTEXBUFFERHANDLEDATA *	VertexBuffer,
	INDEXBUFFERHANDLEDATA *		IndexBuffer,
	int							PrimitiveType,
	int							BaseVertex,
	int							StartVertex,
	int							UseVertexNum,
	int							StartIndex,
	int							UseIndexNum,
	IMAGEDATA *					Image,
	int							TransFlag
)
{
	D_DXGI_FORMAT IndexFormat ;

	// VERTEX3D�\���̂̂ݑΉ�
	if( VertexBuffer->Type != DX_VERTEX_TYPE_NORMAL_3D )
	{
		return -1 ;
	}

	switch( IndexBuffer->Type )
	{
	default :
	case DX_INDEX_TYPE_16BIT :
		IndexFormat = D_DXGI_FORMAT_R16_UINT ;
		break ;

	case DX_INDEX_TYPE_32BIT :
		IndexFormat = D_DXGI_FORMAT_R32_UINT ;
		break ;
	}

	if( Graphics_D3D11_DrawPrimitive3DPreparation( DX_D3D11_DRAWPREP_LIGHTING | DX_D3D11_DRAWPREP_SPECULAR, Image, TransFlag ) < 0 )
	{
		return -1 ;
	}

	// �v���~�e�B�u�^�C�v���Z�b�g
	Graphics_D3D11_DeviceState_SetPrimitiveTopology( ( D_D3D11_PRIMITIVE_TOPOLOGY )PrimitiveType ) ;

	// ���̓��C�A�E�g���Z�b�g
	Graphics_D3D11_DeviceState_SetInputLayout( GD3D11.Device.InputLayout.BaseInputLayout[ D3D11_VERTEX_INPUTLAYOUT_3D_LIGHT ] ) ;

	// ���_�V�F�[�_�[���Z�b�g
	if( Graphics_D3D11_Shader_Normal3DDraw_Setup() == FALSE )
	{
		return -1 ;
	}

	// ���_�o�b�t�@���Z�b�g
	Graphics_D3D11_DeviceState_SetVertexBuffer( VertexBuffer->PF->D3D11.VertexBuffer, D3D11_VertexSize[ D3D11_VERTEX_INPUTLAYOUT_3D_LIGHT ] ) ;

	// �C���f�b�N�X�o�b�t�@���Z�b�g
	Graphics_D3D11_DeviceState_SetIndexBuffer( IndexBuffer->PF->D3D11.IndexBuffer, IndexFormat ) ;

	// �X�e�[�g�ƒ萔�o�b�t�@�̍X�V
	Graphics_D3D11_DeviceState_SetupStateAndConstantBuffer() ;

	// �`��
	D3D11DeviceContext_DrawIndexed( ( UINT )UseIndexNum, ( UINT )StartIndex, BaseVertex ) ;

	// �I��
	return 0 ;
}

extern	int		Graphics_D3D11_DrawPrimitive2D( VERTEX_2D *Vertex, int VertexNum, int PrimitiveType, IMAGEDATA *Image, int TransFlag, int BillboardFlag, int Is3D, int TurnFlag, int TextureNo, int IsShadowMap )
{
	int Flag ;
	int i ;
	VERTEX_2D *Vert ;
	int SmHandle ;
	SHADOWMAPDATA *ShadowMap ;
	VERTEX_2D *VertP ;

	if( GAPIWin.D3D11DeviceObject == NULL ) return -1 ;

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �`��̏���
	if( BillboardFlag == TRUE )
	{
		Flag = TransFlag | ( Is3D ? DX_D3D11_DRAWPREP_3D | DX_D3D11_DRAWPREP_FOG : 0 ) | DX_D3D11_DRAWPREP_TEXADDRESS ;
	}
	else
	{
		Flag = TransFlag | ( Is3D ? DX_D3D11_DRAWPREP_3D | DX_D3D11_DRAWPREP_FOG : 0 ) | DX_D3D11_DRAWPREP_TEXADDRESS | DX_D3D11_DRAWPREP_CULLING ;
	}
	if( IsShadowMap )
	{
		SmHandle = ( int )( DWORD_PTR )Image ;
		if( !SHADOWMAPCHKFULL( SmHandle, ShadowMap ) )
		{
			Flag |= DX_D3D11_DRAWPREP_TEXTURE ;
			Graphics_D3D11_DrawSetting_SetTexture( ShadowMap->PF->D3D11.DepthTexture, ShadowMap->PF->D3D11.DepthTextureSRV ) ;

			if( GSYS.ChangeSettingFlag ||
				GD3D11.Device.DrawSetting.DrawPrepAlwaysFlag != FALSE ||
				GD3D11.Device.DrawSetting.DrawPrepParamFlag != Flag )
			{
				Graphics_D3D11_DrawPreparation( Flag ) ;
			}
		}
	}
	else
	if( Image )
	{
		DX_D3D11_DRAWPREP_TEX( Image->Orig, Image->Hard.Draw[ TextureNo ].Tex->PF->D3D11.Texture, Image->Hard.Draw[ TextureNo ].Tex->PF->D3D11.TextureSRV, Flag )
	}
	else
	{
		DX_D3D11_DRAWPREP_NOTEX( Flag )
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

	if( GSYS.HardInfo.UseVertexColorBGRAFormat == FALSE )
	{
		VertP = ( VERTEX_2D * )Vertex ;
		for( i = 0 ; i < VertexNum ; i ++, VertP ++ )
		{
			VertP->color =	(   VertP->color & 0xff00ff00 ) |
							( ( VertP->color & 0x00ff0000 ) >> 16 ) |
							( ( VertP->color & 0x000000ff ) << 16 ) ;
		}
	}

	// �`��
	Graphics_D3D11_CommonBuffer_DrawPrimitive(
		D3D11_VERTEX_INPUTLAYOUT_2D,
		( D_D3D11_PRIMITIVE_TOPOLOGY )PrimitiveType,
		Vertex,
		VertexNum
	) ;

	if( GSYS.HardInfo.UseVertexColorBGRAFormat == FALSE )
	{
		VertP = ( VERTEX_2D * )Vertex ;
		for( i = 0 ; i < VertexNum ; i ++, VertP ++ )
		{
			VertP->color =	(   VertP->color & 0xff00ff00 ) |
							( ( VertP->color & 0x00ff0000 ) >> 16 ) |
							( ( VertP->color & 0x000000ff ) << 16 ) ;
		}
	}

	// �I��
	return 0 ;
}

extern	int		Graphics_D3D11_DrawPrimitive2DUser( const VERTEX2D *Vertex, int VertexNum, int PrimitiveType, IMAGEDATA *Image, int TransFlag, int Is3D, int TurnFlag, int TextureNo )
{
	int Flag ;
	int i ;
	VERTEX2D *VertP ;
	unsigned char tmp ;

	if( GAPIWin.D3D11DeviceObject == NULL )
	{
		return -1 ;
	}

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �`��̏���
	Flag = TransFlag | ( Is3D ? DX_D3D11_DRAWPREP_3D | DX_D3D11_DRAWPREP_FOG : 0 ) | DX_D3D11_DRAWPREP_TEXADDRESS | DX_D3D11_DRAWPREP_CULLING ;
	if( Image )
	{
		DX_D3D11_DRAWPREP_TEX( Image->Orig, Image->Hard.Draw[ TextureNo ].Tex->PF->D3D11.Texture, Image->Hard.Draw[ TextureNo ].Tex->PF->D3D11.TextureSRV, Flag )
	}
	else
	{
		DX_D3D11_DRAWPREP_NOTEX( Flag )
	}

	if( GSYS.HardInfo.UseVertexColorBGRAFormat == FALSE )
	{
		VertP = ( VERTEX2D * )Vertex ;
		for( i = 0 ; i < VertexNum ; i ++, VertP ++ )
		{
			tmp = VertP->dif.b ;
			VertP->dif.b = VertP->dif.r ;
			VertP->dif.r = tmp ;
		}
	}

	// �`��
	Graphics_D3D11_CommonBuffer_DrawPrimitive(
		D3D11_VERTEX_INPUTLAYOUT_2D,
		( D_D3D11_PRIMITIVE_TOPOLOGY )PrimitiveType,
		Vertex,
		VertexNum
	) ;

	if( GSYS.HardInfo.UseVertexColorBGRAFormat == FALSE )
	{
		VertP = ( VERTEX2D * )Vertex ;
		for( i = 0 ; i < VertexNum ; i ++, VertP ++ )
		{
			tmp = VertP->dif.b ;
			VertP->dif.b = VertP->dif.r ;
			VertP->dif.r = tmp ;
		}
	}

	// �I��
	return 0 ;
}

extern	int		Graphics_D3D11_DrawIndexedPrimitive2DUser( const VERTEX2D *Vertex, int VertexNum, const WORD *Indices, int IndexNum, int PrimitiveType, IMAGEDATA *Image, int TransFlag )
{
	int Flag ;
	int i ;
	VERTEX2D *VertP ;
	unsigned char tmp ;

	if( GAPIWin.D3D11DeviceObject == NULL )
	{
		return -1 ;
	}

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �`��̏���
	Flag = TransFlag | DX_D3D11_DRAWPREP_TEXADDRESS | DX_D3D11_DRAWPREP_CULLING ;
	if( Image )
	{
		DX_D3D11_DRAWPREP_TEX( Image->Orig, Image->Hard.Draw[ 0 ].Tex->PF->D3D11.Texture, Image->Hard.Draw[ 0 ].Tex->PF->D3D11.TextureSRV, Flag )
	}
	else
	{
		DX_D3D11_DRAWPREP_NOTEX( Flag )
	}

	if( GSYS.HardInfo.UseVertexColorBGRAFormat == FALSE )
	{
		VertP = ( VERTEX2D * )Vertex ;
		for( i = 0 ; i < VertexNum ; i ++, VertP ++ )
		{
			tmp = VertP->dif.b ;
			VertP->dif.b = VertP->dif.r ;
			VertP->dif.r = tmp ;
		}
	}

	// �`��
	Graphics_D3D11_CommonBuffer_DrawIndexedPrimitive(
		D3D11_VERTEX_INPUTLAYOUT_2D,
		( D_D3D11_PRIMITIVE_TOPOLOGY )PrimitiveType,
		Vertex,
		VertexNum,
		Indices,
		IndexNum, 
		D_DXGI_FORMAT_R16_UINT
	) ;

	if( GSYS.HardInfo.UseVertexColorBGRAFormat == FALSE )
	{
		VertP = ( VERTEX2D * )Vertex ;
		for( i = 0 ; i < VertexNum ; i ++, VertP ++ )
		{
			tmp = VertP->dif.b ;
			VertP->dif.b = VertP->dif.r ;
			VertP->dif.r = tmp ;
		}
	}

	// �I��
	return 0 ;
}












// �V�F�[�_�[�`��p�`��O�Z�b�g�A�b�v�֐�
extern void Graphics_D3D11_DrawPreparationToShader( int ParamFlag, int Is2D )
{
	int							i ;
	int							Flag ;
	D_ID3D11ShaderResourceView	*ShaderResourceView[ USE_TEXTURESTAGE_NUM ] ;
	int							ShaderResourceViewNum ;

	// ��{�I�ȃZ�b�g�A�b�v����
	Flag = ParamFlag | DX_D3D11_DRAWPREP_SPECULAR | DX_D3D11_DRAWPREP_TEXADDRESS | DX_D3D11_DRAWPREP_NOBLENDSETTING | ( Is2D ? 0 : ( DX_D3D11_DRAWPREP_CULLING | DX_D3D11_DRAWPREP_3D ) ) ;
	DX_D3D11_DRAWPREP_NOTEX( Flag )

	// �A���t�@�e�X�g�p�����[�^���L���ȏꍇ�͂����D�悷��
	if( GSYS.DrawSetting.AlphaTestMode != -1 )
	{
		Graphics_D3D11_DeviceState_SetAlphaTestRef( GSYS.DrawSetting.AlphaTestParam ) ;
		Graphics_D3D11_DeviceState_SetAlphaTestCmpMode( GSYS.DrawSetting.AlphaTestMode ) ;
	}
	else
	{
		// �f�t�H���g�̃A���t�@�e�X�g�̐ݒ���Z�b�g
		Graphics_D3D11_DeviceState_SetAlphaTestRef( -1 ) ;
		Graphics_D3D11_DeviceState_SetAlphaTestCmpMode( DX_CMP_GREATER ) ;
	}

	// �o�͐�Ƃ̃u�����h�������Z�b�g
	Graphics_D3D11_DeviceState_SetBlendMode( GSYS.DrawSetting.BlendMode == DX_BLENDMODE_SUB ? DX_BLENDMODE_SUB1 : GSYS.DrawSetting.BlendMode ) ;

	// �g�p����e�N�X�`�����
	ShaderResourceViewNum = 0 ;
	for( i = 0 ; i < USE_TEXTURESTAGE_NUM ; i ++ )
	{
		IMAGEDATA     *TempImage ;
		SHADOWMAPDATA *TempShadowMap ;

		if( !GRAPHCHKFULL( GSYS.DrawSetting.UserShaderRenderInfo.SetTextureGraphHandle[ i ], TempImage ) )
		{
			ShaderResourceView[ i ] = TempImage->Hard.Draw[ 0 ].Tex->PF->D3D11.TextureSRV ;
		}
		else
		if( !SHADOWMAPCHKFULL( GSYS.DrawSetting.UserShaderRenderInfo.SetTextureGraphHandle[ i ], TempShadowMap ) )
		{
			ShaderResourceView[ i ] = TempShadowMap->PF->D3D11.DepthTextureSRV ;
		}
		else
		{
			continue ;
		}

		ShaderResourceViewNum = i + 1 ;
	}

	// �g�p����e�N�X�`�����Z�b�g
	if( ShaderResourceViewNum > 0 )
	{
		Graphics_D3D11_DeviceState_SetPSShaderResouceView( 0, ShaderResourceViewNum, ShaderResourceView ) ;
	}

	// �g�p���钸�_�V�F�[�_�[���X�V
	if( Graphics_D3D11_DeviceState_SetVertexShaderToHandle( GSYS.DrawSetting.UserShaderRenderInfo.SetVertexShaderHandle ) < 0 )
	{
		// �I���W�i���̒��_�V�F�[�_�[�̎w�肪�����ꍇ�̓f�t�H���g�̒��_�V�F�[�_�[���g�p����
		Graphics_D3D11_DeviceState_SetVertexShader(
			Is2D ?
				GD3D11.Device.Shader.Base.BaseSimple_VS[ D3D11_VERTEX_INPUTLAYOUT_BLENDTEX_2D ] :
				GD3D11.Device.Shader.Base.BaseSimple_VS[ D3D11_VERTEX_INPUTLAYOUT_SHADER_3D   ], FALSE ) ;
	}

	// �g�p����s�N�Z���V�F�[�_�[���X�V
	Graphics_D3D11_DeviceState_SetPixelShaderToHandle( GSYS.DrawSetting.UserShaderRenderInfo.SetPixelShaderHandle ) ;
}

// �V�F�[�_�[���g���ĂQ�c�v���~�e�B�u��`�悷��
extern	int		Graphics_D3D11_DrawPrimitive2DToShader(        const VERTEX2DSHADER *Vertex, int VertexNum,                                              int PrimitiveType /* DX_PRIMTYPE_TRIANGLELIST �� */ )
{
	if( GAPIWin.D3D11DeviceObject == NULL )
	{
		return -1 ;
	}

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �`��̏���
	Graphics_D3D11_DrawPreparationToShader( 0, TRUE ) ;

	// �`��
	Graphics_D3D11_CommonBuffer_DrawPrimitive(
		D3D11_VERTEX_INPUTLAYOUT_BLENDTEX_2D,
		( D_D3D11_PRIMITIVE_TOPOLOGY )PrimitiveType,
		Vertex,
		VertexNum
	) ;

	// �I��
	return 0 ;
}

// �V�F�[�_�[���g���ĂR�c�v���~�e�B�u��`�悷��
extern	int		Graphics_D3D11_DrawPrimitive3DToShader(        const VERTEX3DSHADER *Vertex, int VertexNum,                                              int PrimitiveType /* DX_PRIMTYPE_TRIANGLELIST �� */ )
{
	if( GAPIWin.D3D11DeviceObject == NULL )
	{
		return -1 ;
	}

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �`��̏���
	Graphics_D3D11_DrawPreparationToShader( DX_D3D11_DRAWPREP_LIGHTING | DX_D3D11_DRAWPREP_FOG, FALSE ) ;

	// �s����n�[�h�E�G�A�ɔ��f����
	Graphics_DrawSetting_ApplyLibMatrixToHardware() ;

	// �`��
	Graphics_D3D11_CommonBuffer_DrawPrimitive(
		D3D11_VERTEX_INPUTLAYOUT_SHADER_3D,
		( D_D3D11_PRIMITIVE_TOPOLOGY )PrimitiveType,
		Vertex,
		VertexNum,
		FALSE
	) ;

	// �I��
	return 0 ;
}

// �V�F�[�_�[���g���ĂQ�c�v���~�e�B�u��`�悷��( ���_�C���f�b�N�X���g�p���� )
extern	int		Graphics_D3D11_DrawPrimitiveIndexed2DToShader( const VERTEX2DSHADER *Vertex, int VertexNum, const unsigned short *Indices, int IndexNum, int PrimitiveType /* DX_PRIMTYPE_TRIANGLELIST �� */ )
{
	if( GAPIWin.D3D11DeviceObject == NULL )
	{
		return -1 ;
	}

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �`��̏���
	Graphics_D3D11_DrawPreparationToShader( 0, TRUE ) ;

	// �`��
	Graphics_D3D11_CommonBuffer_DrawIndexedPrimitive(
		D3D11_VERTEX_INPUTLAYOUT_BLENDTEX_2D,
		( D_D3D11_PRIMITIVE_TOPOLOGY )PrimitiveType,
		Vertex,
		VertexNum,
		Indices,
		IndexNum,
		D_DXGI_FORMAT_R16_UINT
	) ;

	// �I��
	return 0 ;
}

// �V�F�[�_�[���g���ĂR�c�v���~�e�B�u��`�悷��( ���_�C���f�b�N�X���g�p���� )
extern	int		Graphics_D3D11_DrawPrimitiveIndexed3DToShader( const VERTEX3DSHADER *Vertex, int VertexNum, const unsigned short *Indices, int IndexNum, int PrimitiveType /* DX_PRIMTYPE_TRIANGLELIST �� */ )
{
	if( GAPIWin.D3D11DeviceObject == NULL )
	{
		return -1 ;
	}

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �`��̏���
	Graphics_D3D11_DrawPreparationToShader( DX_D3D11_DRAWPREP_LIGHTING | DX_D3D11_DRAWPREP_FOG, FALSE ) ;

	// �s����n�[�h�E�G�A�ɔ��f����
	Graphics_DrawSetting_ApplyLibMatrixToHardware() ;

	// �`��
	Graphics_D3D11_CommonBuffer_DrawIndexedPrimitive(
		D3D11_VERTEX_INPUTLAYOUT_SHADER_3D,
		( D_D3D11_PRIMITIVE_TOPOLOGY )PrimitiveType,
		Vertex,
		VertexNum,
		Indices,
		IndexNum,
		D_DXGI_FORMAT_R16_UINT,
		FALSE
	) ;

	// �I��
	return 0 ;
}

// �V�F�[�_�[���g���ĂR�c�v���~�e�B�u��`�悷��( ���_�o�b�t�@�g�p�� )
extern	int		Graphics_D3D11_DrawPrimitive3DToShader_UseVertexBuffer2(        int VertexBufHandle,                     int PrimitiveType /* DX_PRIMTYPE_TRIANGLELIST �� */, int StartVertex, int UseVertexNum )
{
	VERTEXBUFFERHANDLEDATA *VertexBuffer ;

	// �G���[����
	if( VERTEXBUFFERCHK( VertexBufHandle, VertexBuffer ) )
	{
		return -1 ;
	}

	// �V�F�[�_�[�p���_�f�[�^�ł͂Ȃ��ꍇ�̓G���[
	if( VertexBuffer->Type != DX_VERTEX_TYPE_SHADER_3D )
	{
		return -1 ;
	}

	// �s����n�[�h�E�G�A�ɔ��f����
	Graphics_DrawSetting_ApplyLibMatrixToHardware() ;

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �`��̏���
	Graphics_D3D11_DrawPreparationToShader( DX_D3D11_DRAWPREP_LIGHTING | DX_D3D11_DRAWPREP_FOG, FALSE ) ;

	// �v���~�e�B�u�^�C�v���Z�b�g
	Graphics_D3D11_DeviceState_SetPrimitiveTopology( ( D_D3D11_PRIMITIVE_TOPOLOGY )PrimitiveType ) ;

	// ���̓��C�A�E�g���Z�b�g
	Graphics_D3D11_DeviceState_SetInputLayout( GD3D11.Device.InputLayout.BaseInputLayout[ D3D11_VERTEX_INPUTLAYOUT_SHADER_3D ] ) ;

	// ���_�o�b�t�@���Z�b�g
	Graphics_D3D11_DeviceState_SetVertexBuffer( VertexBuffer->PF->D3D11.VertexBuffer, D3D11_VertexSize[ D3D11_VERTEX_INPUTLAYOUT_SHADER_3D ] ) ;

	// �X�e�[�g�ƒ萔�o�b�t�@�̍X�V
	Graphics_D3D11_DeviceState_SetupStateAndConstantBuffer() ;

	// �`��
	D3D11DeviceContext_Draw( ( UINT )UseVertexNum, ( UINT )StartVertex ) ;

	// �I��
	return 0 ;
}

// �V�F�[�_�[���g���ĂR�c�v���~�e�B�u��`�悷��( ���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@�g�p�� )
extern	int		Graphics_D3D11_DrawPrimitiveIndexed3DToShader_UseVertexBuffer2( int VertexBufHandle, int IndexBufHandle, int PrimitiveType /* DX_PRIMTYPE_TRIANGLELIST �� */, int BaseVertex, int StartVertex, int UseVertexNum, int StartIndex, int UseIndexNum )
{
	VERTEXBUFFERHANDLEDATA *VertexBuffer ;
	INDEXBUFFERHANDLEDATA  *IndexBuffer ;
	D_DXGI_FORMAT IndexFormat ;

	// �G���[����
	if( VERTEXBUFFERCHK( VertexBufHandle, VertexBuffer ) )
	{
		return -1 ;
	}
	if( INDEXBUFFERCHK( IndexBufHandle, IndexBuffer ) )
	{
		return -1 ;
	}

	// �V�F�[�_�[�p���_�f�[�^�ł͂Ȃ��ꍇ�̓G���[
	if( VertexBuffer->Type != DX_VERTEX_TYPE_SHADER_3D )
	{
		return -1 ;
	}

	switch( IndexBuffer->Type )
	{
	default :
	case DX_INDEX_TYPE_16BIT :
		IndexFormat = D_DXGI_FORMAT_R16_UINT ;
		break ;

	case DX_INDEX_TYPE_32BIT :
		IndexFormat = D_DXGI_FORMAT_R32_UINT ;
		break ;
	}

	// �s����n�[�h�E�G�A�ɔ��f����
	Graphics_DrawSetting_ApplyLibMatrixToHardware() ;

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �`��̏���
	Graphics_D3D11_DrawPreparationToShader( DX_D3D11_DRAWPREP_LIGHTING | DX_D3D11_DRAWPREP_FOG, FALSE ) ;

	// �v���~�e�B�u�^�C�v���Z�b�g
	Graphics_D3D11_DeviceState_SetPrimitiveTopology( ( D_D3D11_PRIMITIVE_TOPOLOGY )PrimitiveType ) ;

	// ���̓��C�A�E�g���Z�b�g
	Graphics_D3D11_DeviceState_SetInputLayout( GD3D11.Device.InputLayout.BaseInputLayout[ D3D11_VERTEX_INPUTLAYOUT_SHADER_3D ] ) ;

	// ���_�o�b�t�@���Z�b�g
	Graphics_D3D11_DeviceState_SetVertexBuffer( VertexBuffer->PF->D3D11.VertexBuffer, D3D11_VertexSize[ D3D11_VERTEX_INPUTLAYOUT_SHADER_3D ] ) ;

	// �C���f�b�N�X�o�b�t�@���Z�b�g
	Graphics_D3D11_DeviceState_SetIndexBuffer( IndexBuffer->PF->D3D11.IndexBuffer, IndexFormat ) ;

	// �X�e�[�g�ƒ萔�o�b�t�@�̍X�V
	Graphics_D3D11_DeviceState_SetupStateAndConstantBuffer() ;

	// �`��
	D3D11DeviceContext_DrawIndexed( ( UINT )UseIndexNum, ( UINT )StartIndex, BaseVertex ) ;

	// �I��
	return 0 ;
}




























// ���ˑ��������֌W

// �`�揈���̊��ˑ������̏��������s���֐�( ���s�ӏ���ʂO )
extern	int		Graphics_D3D11_Initialize_Timing0_PF( void )
{
	// Direct3D11 ���g�p�����O���t�B�b�N�X�����̏�����
	if( Graphics_D3D11_Initialize() == -1 )
	{
		return -1 ;
	}

	// ����I��
	return 0 ;
}

// �`�揈���̊��ˑ������̏��������s���֐�( ���s�ӏ���ʂQ )
extern	int		Graphics_D3D11_Initialize_Timing1_PF( void )
{
	// �f�o�C�X�̐ݒ�����t���b�V��
	Graphics_D3D11_DeviceState_RefreshRenderState() ;

	// ��ʂ̏�����
	Graphics_Hardware_D3D11_ClearDrawScreen_PF( NULL ) ;

	// ����I��
	return 0 ;
}

// �n�[�h�E�G�A�A�N�Z�����[�^���g�p����ꍇ�̊��ˑ��̏������������s��
extern	int		Graphics_D3D11_Hardware_Initialize_PF( void )
{
	return Graphics_D3D11_Device_Initialize() ;
}

// �`�揈���̊��ˑ������̌�n�����s���֐�
extern	int		Graphics_D3D11_Terminate_PF( void )
{
	// Direct3D11 �̕`�揈���̌�n��
	Graphics_D3D11_Terminate() ;

	// �V�F�[�_�[�R�[�h�̌�n��
	Graphics_D3D11_ShaderCode_Base_Terminate() ;
	Graphics_D3D11_ShaderCode_Base3D_Terminate() ;

#ifndef DX_NON_MODEL
	Graphics_D3D11_ShaderCode_Model_Terminate() ;
#endif // DX_NON_MODEL

	// �I��
	return 0 ;
}

// �O���t�B�b�N�X�V�X�e���̕��A�A���͕ύX�t���̍ăZ�b�g�A�b�v���s��
extern	int		Graphics_D3D11_RestoreOrChangeSetupGraphSystem_PF( int Change, int ScreenSizeX, int ScreenSizeY, int ColorBitDepth, int RefreshRate )
{
	int i ;

	// �O���t�B�b�N�X�f�o�C�X�̃��X�g���畜�A����O�ɌĂԊ֐����o�^����Ă�������s����
	if( GD3D11.Device.Setting.DeviceLostCallbackFunction )
	{
		GD3D11.Device.Setting.DeviceLostCallbackFunction( GD3D11.Device.Setting.DeviceLostCallbackData ) ;
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
//		SetMemImgDefaultColorType( ColorBitDepth == 32 ? 1 : 0 ) ;
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

		// Direct3D11 �ɂ��`�揈���̏�����
		GSYS.Screen.FullScreenResolutionModeAct = GSYS.Screen.FullScreenResolutionMode ;
		Graphics_D3D11_Initialize() ;

		// �O���t�B�b�N�n���h�������� Direct3D11 �I�u�W�F�N�g�̍č쐬
		Graphics_D3D11_CreateObjectAll() ;

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
		Graphics_D3D11_Device_ReInitialize() ;

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
		if( GD3D11.Device.Setting.DeviceRestoreCallbackFunction )
		{
			GD3D11.Device.Setting.DeviceRestoreCallbackFunction( GD3D11.Device.Setting.DeviceRestoreCallbackData ) ;
		}

		// �n�[�h�E�G�A�̐ݒ�����t���b�V��
		Graphics_D3D11_DeviceState_RefreshRenderState() ;

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
extern	int		Graphics_D3D11_Hardware_CheckValid_PF( void )
{
	return D3D11Device_IsValid() ;
}

// DirectX �̃I�u�W�F�N�g���������
extern	void	Graphics_D3D11_ReleaseDirectXObject_PF( void )
{
#ifndef DX_NON_MASK
	// �}�X�N�T�[�t�F�X���ꎞ�폜
	Mask_ReleaseSurface() ;
#endif
	// �O���t�B�b�N�n���h�������� DirectX �I�u�W�F�N�g�̉��
	Graphics_D3D11_ReleaseObjectAll() ;

	// �V�X�e�������� Direct3D11 �I�u�W�F�N�g�̉��
	Graphics_D3D11_Terminate() ;
}





















// ���ˑ��V�X�e���֌W

// WM_ACTIVATE ���b�Z�[�W�̏����ŁA�E�C���h�E���[�h���ɃA�N�e�B�u�ɂȂ����ۂɌĂ΂��֐�
extern	void	Graphics_D3D11_WM_ACTIVATE_ActiveProcess_PF( void )
{
}
























// ���ˑ��`��ݒ�֌W

// �V�F�[�_�[�`��ł̕`����ݒ肷��
extern	int		Graphics_Hardware_D3D11_SetRenderTargetToShader_PF( int TargetIndex, int DrawScreen, int SurfaceIndex )
{
	IMAGEDATA *Image ;
	IMAGEDATA *OldImage ;

	// �^�[�Q�b�g���O�ȊO�̏ꍇ�̓V�F�[�_�[���g�p�ł��Ȃ��ꍇ�͉������Ȃ�
	if( GSYS.HardInfo.UseShader == FALSE )
		return -1 ;

	// �f�o�C�X�������ɂȂ��Ă����牽�������I��
	if( GAPIWin.D3D11DeviceObject == NULL )
		return -1 ;

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �Z�b�g���Ă����e�N�X�`���[���O��
	Graphics_D3D11_DrawSetting_SetTexture( NULL, NULL ) ;

	// ���܂ł̕`���̉摜���̎擾
	if( GRAPHCHKFULL( GSYS.DrawSetting.TargetScreen[ TargetIndex ], OldImage ) )
	{
		OldImage = NULL ;
	}
	else
	{
		// ���܂ł̕`��悪�}���`�T���v���^�C�v�̃����_�����O�^�[�Q�b�g�������ꍇ�̓_�E���T���v�����O����
		if( OldImage->Hard.Draw[ 0 ].Tex->PF->D3D11.MSTexture != NULL )
		{
			Graphics_D3D11_UpdateDrawTexture(
				OldImage->Hard.Draw[ 0 ].Tex,
				GSYS.DrawSetting.TargetScreenSurface[ TargetIndex ],
				GD3D11.Device.Caps.TextureFormat[ OldImage->Orig->ColorFormat ]
			) ;
		}
	}

	// �摜���ǂ����𔻒�
	if( GRAPHCHKFULL( DrawScreen, Image ) )
	{
		// �摜�ł͂Ȃ��ꍇ�͕`��Ώۂ𖳌��ɂ���
		GSYS.DrawSetting.TargetScreen[ TargetIndex ] = 0 ;
		GSYS.DrawSetting.TargetScreenSurface[ TargetIndex ] = 0 ;
		Graphics_D3D11_DeviceState_SetRenderTarget( NULL, NULL, TargetIndex ) ;
		return 0 ;
	}

	// �摜�������ꍇ�͕`��\�ł͖����ꍇ�̓G���[
	if( Image->Orig->FormatDesc.DrawValidFlag == FALSE )
	{
		return -1 ;
	}

	// �`�����Z�b�g
	Graphics_D3D11_DeviceState_SetRenderTarget( Image->Hard.Draw[ 0 ].Tex->PF->D3D11.Texture, Image->Hard.Draw[ 0 ].Tex->PF->D3D11.TextureRTV[ SurfaceIndex ], TargetIndex ) ;

	// ����I��
	return 0 ;
}

// SetDrawBright �̈��������
extern	int		Graphics_Hardware_D3D11_SetDrawBrightToOneParam_PF( DWORD Bright )
{
	// �f�B�t�[�Y�J���[�̍X�V
	GD3D11.Device.DrawInfo.DiffuseColor = GetDiffuseColor() ;

	// ����I��
	return 0 ;
}

// �`��u�����h���[�h���Z�b�g����
extern	int		Graphics_Hardware_D3D11_SetDrawBlendMode_PF( int BlendMode, int BlendParam )
{
	// �f�B�t�[�Y�J���[�̍X�V
	GD3D11.Device.DrawInfo.DiffuseColor = GetDiffuseColor() ;

	// ����I��
	return 0 ;
}

// �`�掞�̃A���t�@�e�X�g�̐ݒ���s��( TestMode:DX_CMP_GREATER��( -1:�f�t�H���g����ɖ߂� )  TestParam:�`��A���t�@�l�Ƃ̔�r�Ɏg�p����l )
extern	int		Graphics_Hardware_D3D11_SetDrawAlphaTest_PF( int TestMode, int TestParam )
{
	// �f�B�t�[�Y�J���[�̍X�V
	GD3D11.Device.DrawInfo.DiffuseColor = GetDiffuseColor() ;

	// ����I��
	return 0 ;
}

// �`�惂�[�h���Z�b�g����
extern	int		Graphics_Hardware_D3D11_SetDrawMode_PF( int DrawMode )
{
	Graphics_D3D11_DeviceState_SetDrawMode( DrawMode ) ;

	// ����I��
	return 0 ;
}

// �`��P�x���Z�b�g
extern	int		Graphics_Hardware_D3D11_SetDrawBright_PF( int RedBright, int GreenBright, int BlueBright )
{
	// �f�B�t�[�Y�J���[�̍X�V
	GD3D11.Device.DrawInfo.DiffuseColor = GetDiffuseColor() ;

	// ����I��
	return 0 ;
}

// SetBlendGraphParam �̉ϒ������p�����[�^�t��
extern	int		Graphics_Hardware_D3D11_SetBlendGraphParamBase_PF( IMAGEDATA *BlendImage, int BlendType, int *Param )
{
	if( GAPIWin.D3D11DeviceObject == NULL ) return -1 ;
	
	// �u�����h�摜�� NULL �������̓e�N�X�`���ł͂Ȃ�������u�����h�摜�����̉���
	if( BlendImage == NULL || BlendImage->Orig->FormatDesc.TextureFlag == FALSE )
	{
		GSYS.DrawSetting.BlendGraph = -1 ;
		Graphics_D3D11_DrawSetting_SetBlendTexture( NULL, NULL ) ;
	}
	else
	{
		// �u�����h�p�����[�^���Z�b�g����
		Graphics_D3D11_DrawSetting_SetBlendTexture(
			BlendImage->Hard.Draw[ 0 ].Tex[ 0 ].PF->D3D11.Texture,
			BlendImage->Hard.Draw[ 0 ].Tex[ 0 ].PF->D3D11.TextureSRV,
			BlendImage->Hard.Draw[ 0 ].Tex[ 0 ].TexWidth,
			BlendImage->Hard.Draw[ 0 ].Tex[ 0 ].TexHeight
		) ;
		Graphics_D3D11_DrawSetting_SetBlendTextureParam( BlendType, Param ) ;
	}

	// ����I��
	return 0 ;
}

// �ő�ٕ����̒l���Z�b�g����
extern	int		Graphics_Hardware_D3D11_SetMaxAnisotropy_PF( int MaxAnisotropy )
{
	// ����I��
	return 0 ;
}

// ���[���h�ϊ��p�s����Z�b�g����
extern	int		Graphics_Hardware_D3D11_SetTransformToWorld_PF( const MATRIX *Matrix )
{
	Graphics_D3D11_DeviceState_SetWorldMatrix( Matrix ) ;

	// ����I��
	return 0 ;
}

// �r���[�ϊ��p�s����Z�b�g����
extern	int		Graphics_Hardware_D3D11_SetTransformToView_PF( const MATRIX *Matrix )
{
	Graphics_D3D11_DeviceState_SetViewMatrix( Matrix ) ;

	// ����I��
	return 0 ;
}

// Direct3D11 �ɐݒ肷��ˉe�s����X�V����
static void Graphics_Hardware_D3D11_RefreshProjectionMatrix( /* int Update3DProjection, int Update2DProjection, int AlwaysUpdate */ )
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
//	{
//		if( AlwaysUpdate == FALSE && Update3DProjection == FALSE )
//		{
//			return ;
//		}

		UseProjectionMatrix = &GSYS.DrawSetting.ProjectionMatrixF ;
		UseViewportMatrix   = &GSYS.DrawSetting.ViewportMatrixF ;
//	}
	CreateMultiplyMatrix( &TempMatrix, UseProjectionMatrix, UseViewportMatrix                             ) ;
	CreateMultiplyMatrix( &TempMatrix, &TempMatrix,         &GSYS.DrawSetting.Direct3DViewportMatrixAntiF ) ;

	Graphics_D3D11_DeviceState_SetProjectionMatrix( &TempMatrix ) ;
}

// ���e�ϊ��p�s����Z�b�g����
extern	int		Graphics_Hardware_D3D11_SetTransformToProjection_PF( const MATRIX *Matrix )
{
	Graphics_Hardware_D3D11_RefreshProjectionMatrix() ;

	// ����I��
	return 0 ;
}

// �r���[�|�[�g�s����Z�b�g����
extern	int		Graphics_Hardware_D3D11_SetTransformToViewport_PF( const MATRIX * /* Matrix */ )
{
	Graphics_Hardware_D3D11_RefreshProjectionMatrix() ;

	// ����I��
	return 0 ;
}

// �e�N�X�`���A�h���X���[�h��ݒ肷��
extern	int		Graphics_Hardware_D3D11_SetTextureAddressMode_PF( int Mode /* DX_TEXADDRESS_WRAP �� */, int Stage )
{
	Graphics_D3D11_DeviceState_SetTextureAddress( Mode, Stage ) ;

	// ����I��
	return 0 ;
}

// �e�N�X�`���A�h���X���[�h��ݒ肷��
extern	int		Graphics_Hardware_D3D11_SetTextureAddressModeUV_PF( int ModeU, int ModeV, int Stage )
{
	Graphics_D3D11_DeviceState_SetTextureAddressU( ModeU, Stage ) ;
	Graphics_D3D11_DeviceState_SetTextureAddressV( ModeV, Stage ) ;

	// ����I��
	return 0 ;
}

// �e�N�X�`�����W�ϊ��s����Z�b�g����
extern	int		Graphics_Hardware_D3D11_SetTextureAddressTransformMatrix_PF( int UseFlag, MATRIX *Matrix, int Sampler )
{
	Graphics_D3D11_DeviceState_SetTextureAddressTransformMatrix( UseFlag, Matrix ) ;

	// ����I��
	return 0 ;
}

// �t�H�O��L���ɂ��邩�ǂ�����ݒ肷��( TRUE:�L��  FALSE:���� )
extern	int		Graphics_Hardware_D3D11_SetFogEnable_PF( int Flag )
{
	Graphics_D3D11_DeviceState_SetFogEnable( Flag ) ;

	// ����I��
	return 0 ;
}

// �t�H�O���[�h��ݒ肷��
extern	int		Graphics_Hardware_D3D11_SetFogMode_PF( int Mode /* DX_FOGMODE_NONE �� */ )
{
	Graphics_D3D11_DeviceState_SetFogVertexMode( Mode ) ;

	// ����I��
	return 0 ;
}

// �t�H�O�J���[��ύX����
extern	int		Graphics_Hardware_D3D11_SetFogColor_PF( DWORD FogColor )
{
	Graphics_D3D11_DeviceState_SetFogColor( FogColor ) ;

	// ����I��
	return 0 ;
}

// �t�H�O���n�܂鋗���ƏI�����鋗����ݒ肷��( 0.0f �` 1.0f )
extern	int		Graphics_Hardware_D3D11_SetFogStartEnd_PF( float start, float end )
{
	Graphics_D3D11_DeviceState_SetFogStartEnd( start, end ) ;

	// ����I��
	return 0 ;
}

// �t�H�O�̖��x��ݒ肷��( 0.0f �` 1.0f )
extern	int		Graphics_Hardware_D3D11_SetFogDensity_PF( float density )
{
	Graphics_D3D11_DeviceState_SetFogDensity( density ) ;

	// ����I��
	return 0 ;
}

// ��{�f�[�^�ɐݒ肳��Ă���t�H�O�����n�[�h�E�F�A�ɔ��f����
extern	int		Graphics_Hardware_D3D11_ApplyLigFogToHardware_PF( void )
{
	if( GD3D11.Device.State.FogEnable != GSYS.DrawSetting.FogEnable )
	{
		Graphics_D3D11_DeviceState_SetFogEnable( GSYS.DrawSetting.FogEnable ) ;
	}

	// ����I��
	return 0 ;
}

// �ȑO�� DrawModiGraph �֐��̃R�[�h���g�p���邩�ǂ����̃t���O���Z�b�g����
extern	int		Graphics_Hardware_D3D11_SetUseOldDrawModiGraphCodeFlag_PF( int Flag )
{
	GD3D11.Setting.UseOldDrawModiGraphCodeFlag = Flag ;
	
	// �I��
	return 0 ;
}

// �`���ɐ��������l���������ނ��ǂ����̃t���O���X�V����
extern	int		Graphics_Hardware_D3D11_RefreshAlphaChDrawMode_PF( void )
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

		WorkTextureHandle = GetWorkTexture( Image->Orig->FormatDesc.FloatTypeFlag, Image->WidthF, Image->Height, 0 ) ;
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
		GD3D11.Device.DrawSetting.ChangeBlendParamFlag = TRUE ;
		GSYS.ChangeSettingFlag = TRUE ;
	}

	// �I��
	return 0 ;
}
























// ���ˑ��ݒ�֌W

// �s�N�Z���P�ʂŃ��C�e�B���O���s�����ǂ�����ݒ肷��A�v ShaderModel 3.0( TRUE:�s�N�Z���P�ʂ̃��C�e�B���O���s��  FALSE:���_�P�ʂ̃��C�e�B���O���s��( �f�t�H���g ) )
extern	int		Graphics_Hardware_D3D11_SetUsePixelLighting_PF( int Flag )
{
	// �������O�̏ꍇ�͐ݒ�l�����ۑ�����
	if( DxSysData.DxLib_InitializeFlag == FALSE )
	{
		GD3D11.UsePixelLightingShader = Flag ;
	}
	else
	{
		// ��������̏ꍇ�� D_D3D_FEATURE_LEVEL_10_0 �����̏ꍇ�̓s�N�Z���P�ʃ��C�e�B���O�͎g�p�ł��Ȃ�
		if( GD3D11.Setting.FeatureLevel < D_D3D_FEATURE_LEVEL_10_0 )
		{
			GD3D11.UsePixelLightingShader = FALSE ;
		}
		else
		{
			GD3D11.UsePixelLightingShader = Flag ;
		}
	}

	// �I��
	return 0 ;
}

// �O���t�B�b�N�X�f�o�C�X�����X�g���畜�A�����ۂɌĂ΂��R�[���o�b�N�֐���ݒ肷��
extern	int		Graphics_Hardware_D3D11_SetGraphicsDeviceRestoreCallbackFunction_PF( void (* Callback )( void *Data ), void *CallbackData )
{
	GD3D11.Device.Setting.DeviceRestoreCallbackFunction = Callback ;
	GD3D11.Device.Setting.DeviceRestoreCallbackData = CallbackData ;

	// �I��
	return 0 ;
}

// �O���t�B�b�N�X�f�o�C�X�����X�g���畜�A����O�ɌĂ΂��R�[���o�b�N�֐���ݒ肷��
extern	int		Graphics_Hardware_D3D11_SetGraphicsDeviceLostCallbackFunction_PF( void (* Callback )( void *Data ), void *CallbackData )
{
	GD3D11.Device.Setting.DeviceLostCallbackFunction = Callback ;
	GD3D11.Device.Setting.DeviceLostCallbackData = CallbackData ;

	// �I��
	return 0 ;
}

// �c�w���C�u�����̂c�����������R�c�ݒ�����Ȃ���
extern	int		Graphics_Hardware_D3D11_RefreshDxLibDirect3DSetting_PF( void )
{
	if( GAPIWin.D3D11DeviceObject == NULL )
	{
		return -1 ;
	}

	Graphics_D3D11_DeviceState_RefreshRenderState() ;

	// �������t���O�𗧂Ă�
	GD3D11.Device.DrawSetting.CancelSettingEqualCheck = TRUE ;

	// �u�����h���[�h�̍Đݒ�
	Graphics_D3D11_DrawSetting_SetDrawBlendMode( GD3D11.Device.DrawSetting.BlendMode, GD3D11.Device.DrawSetting.AlphaTestValidFlag, GD3D11.Device.DrawSetting.AlphaChannelValidFlag ) ;
	Graphics_D3D11_DrawPreparation() ;

	// �������t���O��|��
	GD3D11.Device.DrawSetting.CancelSettingEqualCheck = FALSE ;

	// �I��
	return 0 ;
}

// Direct3D11 �Ŏg�p����Œ�@�\���x�����w�肷��֐��ł��A���ADX_DIRECT3D_11_FEATURE_LEVEL_11_0 ���Ⴂ�@�\���x���ł̐���ȓ���͕ۏ؂��܂���( �f�t�H���g�� DX_DIRECT3D_11_FEATURE_LEVEL_11_0 )
extern int Graphics_D3D11_SetUseDirect3D11MinFeatureLevel_PF( int Level /* DX_DIRECT3D_11_FEATURE_LEVEL_10_0 �Ȃ� */ )
{
	// �������O�̂ݗL��
	if( DxSysData.DxLib_InitializeFlag == TRUE ) return -1 ;

	// �t���O��ۑ�
	GD3D11.Setting.UseMinFeatureLevelDirect3D11 = Level ;

	// �I��
	return 0 ;
}































// ���ˑ���ʊ֌W

// �ݒ�Ɋ�Â��Ďg�p����y�o�b�t�@���Z�b�g����
extern	int		Graphics_Hardware_D3D11_SetupUseZBuffer_PF( void )
{
	IMAGEDATA *TargetScreenImage = NULL ;
	IMAGEDATA *TargetZBufferImage = NULL ;
	SHADOWMAPDATA *TargetShadowMap = NULL ;

	// �V���h�E�}�b�v���ǂ������`�F�b�N
	if( !SHADOWMAPCHKFULL( GSYS.DrawSetting.TargetScreen[ 0 ], TargetShadowMap ) )
	{
		// �V���h�E�}�b�v�̏ꍇ�͐�p�̂y�o�b�t�@���g�p����
		Graphics_D3D11_DeviceState_SetDepthStencil( TargetShadowMap->PF->D3D11.DepthBuffer, TargetShadowMap->PF->D3D11.DepthBufferDSV ) ;
	}
	else
	{
		// �`���̉摜���̎擾
		GRAPHCHKFULL( GSYS.DrawSetting.TargetScreen[ 0 ], TargetScreenImage ) ;

		// �g�p����y�o�b�t�@�̉摜���̎擾
		GRAPHCHKFULL( GSYS.DrawSetting.TargetZBuffer, TargetZBufferImage ) ;

		// �}�X�N�T�[�t�F�X�����݂��Ă��Ċ��L���ȏꍇ�̓}�X�N�T�[�t�F�X�̂y�o�b�t�@���g�p����
//#ifndef DX_NON_MASK
//		if( MASKD.MaskValidFlag && MASKD3D9.MaskScreenSurface )
//		{
//			Direct3DDevice9_SetDepthStencilSurface( GD3D9.Device.Screen.ZBufferSurface ) ;
//		}
//		else
//#endif
		// �`��\�摜���`��悩�ǂ����ŏ����𕪊�
		if( TargetScreenImage )
		{
			// �`���y�o�b�t�@�������āA���`���ȏ�̃T�C�Y�����ꍇ�͕`���y�o�b�t�@���g�p����
			if( TargetZBufferImage != NULL &&
				TargetZBufferImage->WidthI  >= TargetScreenImage->WidthI &&
				TargetZBufferImage->HeightI >= TargetScreenImage->HeightI )
			{
				Graphics_D3D11_DeviceState_SetDepthStencil( TargetZBufferImage->Hard.Draw[ 0 ].Tex->PF->D3D11.DepthBuffer, TargetZBufferImage->Hard.Draw[ 0 ].Tex->PF->D3D11.DepthBufferDSV ) ;
			}
			else
			// �`��\�摜�̂y�o�b�t�@������ꍇ�͂��̂y�o�b�t�@���g�p����
			if( TargetScreenImage->Hard.Draw[ 0 ].Tex->PF->D3D11.DepthBuffer )
			{
				Graphics_D3D11_DeviceState_SetDepthStencil( TargetScreenImage->Hard.Draw[ 0 ].Tex->PF->D3D11.DepthBuffer, TargetScreenImage->Hard.Draw[ 0 ].Tex->PF->D3D11.DepthBufferDSV ) ;
			}
			else
			// �`��悪�f�t�H���g�̂y�o�b�t�@�����傫��������y�o�b�t�@���O��
			if( TargetScreenImage->WidthI  > GD3D11.Device.Screen.DepthBufferSizeX || 
				TargetScreenImage->HeightI > GD3D11.Device.Screen.DepthBufferSizeY )
			{
				Graphics_D3D11_DeviceState_SetDepthStencil( NULL, NULL ) ;
			}
			else
			{
				// �f�t�H���g�̂y�o�b�t�@���g�p����
				Graphics_D3D11_DeviceState_SetDepthStencil( GD3D11.Device.Screen.DepthBufferTexture2D, GD3D11.Device.Screen.DepthBufferDSV ) ;
			}
		}
		else
		{
			// �`���y�o�b�t�@�������āA���`���ȏ�̃T�C�Y�����ꍇ�͕`���y�o�b�t�@���g�p����
			if( TargetZBufferImage != NULL &&
				TargetZBufferImage->WidthI  >= GSYS.DrawSetting.DrawSizeX &&
				TargetZBufferImage->HeightI >= GSYS.DrawSetting.DrawSizeY )
			{
				Graphics_D3D11_DeviceState_SetDepthStencil( TargetZBufferImage->Hard.Draw[ 0 ].Tex->PF->D3D11.DepthBuffer, TargetZBufferImage->Hard.Draw[ 0 ].Tex->PF->D3D11.DepthBufferDSV ) ;
			}
			else
			{
				// �f�t�H���g�̂y�o�b�t�@���g�p����
				Graphics_D3D11_DeviceState_SetDepthStencil( GD3D11.Device.Screen.DepthBufferTexture2D, GD3D11.Device.Screen.DepthBufferDSV ) ;
			}
		}
	}

	// �I��
	return 0 ;
}

// ��ʂ̂y�o�b�t�@�̏�Ԃ�����������
extern	int		Graphics_Hardware_D3D11_ClearDrawScreenZBuffer_PF( const RECT *ClearRect )
{
	// �`��Ώۂ̐[�x�o�b�t�@��������
	D3D11DeviceContext_ClearDepthStencilView_ASync( GD3D11.Device.State.TargetDepthDSV, D_D3D11_CLEAR_DEPTH, 1.0f, 0 ) ;

	// �I��
	return 0 ;
}

// ��ʂ̏�Ԃ�����������
extern	int		Graphics_Hardware_D3D11_ClearDrawScreen_PF( const RECT *ClearRect )
{
	D_ID3D11Texture2D        *TargetTexture2D ;
	D_ID3D11RenderTargetView *TargetRTV ;
	FLOAT                    ClearColor[ 4 ] ;
	IMAGEDATA                *Image = NULL ;
	SHADOWMAPDATA            *ShadowMap = NULL ;
	int                      i ;
//	HRESULT hr ;

	if( GAPIWin.D3D11DeviceObject == NULL ) return 0 ;

	for( i = 0 ; i < GSYS.HardInfo.RenderTargetNum ; i ++ )
	{
		// �����ȕ`��悾�����ꍇ�͉������Ȃ�
		if( GSYS.DrawSetting.TargetScreen[ i ] == 0 )
		{
			continue ;
		}

		// �`���摜�̉摜�����擾
		Image     = NULL ;
		ShadowMap = NULL ;
		GRAPHCHKFULL(     GSYS.DrawSetting.TargetScreen[ i ], Image ) ;
		SHADOWMAPCHKFULL( GSYS.DrawSetting.TargetScreen[ i ], ShadowMap ) ;

		// �`��ҋ@���Ă���`�敨��`��
		DRAWSTOCKINFO

		// �`��̏I��
	//	Graphics_D3D9_EndScene() ;

		// �V���h�E�}�b�v���g�p����Ă���ꍇ�̓V���h�E�}�b�v��������
		if( ShadowMap )
		{
			TargetTexture2D = ShadowMap->PF->D3D11.DepthTexture ;
			TargetRTV       = ShadowMap->PF->D3D11.DepthTextureRTV ;
		}
		else
		// �`��\�摜���g�p����Ă���ꍇ�͕`��\�摜��������
		if( Image )
		{
			TargetTexture2D = Image->Orig->Hard.Tex[ 0 ].PF->D3D11.Texture ;
			TargetRTV       = Image->Orig->Hard.Tex[ 0 ].PF->D3D11.TextureRTV[ GSYS.DrawSetting.TargetScreenSurface[ i ] ] ;
		}
		else
		// �T�u�o�b�N�o�b�t�@���L���ɂȂ��Ă���ꍇ�̓T�u�o�b�N�o�b�t�@��������
		if( GD3D11.Device.Screen.SubBackBufferTexture2D != NULL )
		{
			TargetTexture2D = GD3D11.Device.Screen.SubBackBufferTexture2D ;
			TargetRTV       = GD3D11.Device.Screen.SubBackBufferRTV ;
		}
		else
		{
			TargetTexture2D = GD3D11.Device.Screen.OutputWindowInfo[ 0 ].BufferTexture2D ;
			TargetRTV       = GD3D11.Device.Screen.OutputWindowInfo[ 0 ].BufferRTV ;
		}

		// �`��Ώۂ̃J���[�o�b�t�@��������
		ClearColor[ 0 ] = GSYS.Screen.BackgroundRed   / 255.0f ;
		ClearColor[ 1 ] = GSYS.Screen.BackgroundGreen / 255.0f ;
		ClearColor[ 2 ] = GSYS.Screen.BackgroundBlue  / 255.0f ;
		ClearColor[ 3 ] = 0.0f ;
		D3D11DeviceContext_ClearRenderTargetView_ASync( TargetRTV, ClearColor ) ;
	}

	// �`��Ώۂ̐[�x�o�b�t�@��������
	D3D11DeviceContext_ClearDepthStencilView_ASync( GD3D11.Device.State.TargetDepthDSV, D_D3D11_CLEAR_DEPTH, 1.0f, 0 ) ;

	// ����I��
	return 0 ;
}

// �`����ʂ̃Z�b�g
extern	int		Graphics_Hardware_D3D11_SetDrawScreen_PF( int DrawScreen, int OldScreenSurface, IMAGEDATA *NewTargetImage, IMAGEDATA *OldTargetImage, SHADOWMAPDATA *NewTargetShadowMap, SHADOWMAPDATA *OldTargetShadowMap )
{
	D_ID3D11ShaderResourceView *pShaderResourceViews[ D_D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT ] = { NULL } ;

	if( GAPIWin.D3D11DeviceObject == NULL )
	{
		return -1 ;
	}

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �Z�b�g���Ă����e�N�X�`���[���O��
	Graphics_D3D11_DrawSetting_SetTexture( NULL, NULL ) ;
	Graphics_D3D11_DeviceState_SetPSShaderResouceView( 0, D_D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, pShaderResourceViews ) ;

	// ���܂ł̕`��悪�}���`�T���v���^�C�v�̃����_�����O�^�[�Q�b�g�������ꍇ�̓_�E���T���v�����O����
	if( OldTargetImage != NULL && OldTargetImage->Hard.Draw[ 0 ].Tex->PF->D3D11.MSTexture != NULL )
	{
		Graphics_D3D11_UpdateDrawTexture(
			OldTargetImage->Hard.Draw[ 0 ].Tex,
			GSYS.DrawSetting.TargetScreenSurface[ 0 ],
			GD3D11.Device.Caps.TextureFormat[ OldTargetImage->Orig->ColorFormat ]
		) ;
	}

	// �`���̕ύX

	// �}�X�N�T�[�t�F�X�����݂��Ă��Ċ��L���ȏꍇ�̓}�X�N�T�[�t�F�X��`��Ώ̂ɂ���
#ifndef DX_NON_MASK
	if( MASKD.MaskValidFlag && MASKD3D11.MaskScreenTexture )
	{
		Graphics_D3D11_DeviceState_SetRenderTarget( MASKD3D11.MaskScreenTexture, MASKD3D11.MaskScreenTextureRTV ) ;
	}
	else
#endif
	// �V���h�E�}�b�v���L���ȏꍇ�̓V���h�E�}�b�v��`��Ώۂɂ���
	if( NewTargetShadowMap )
	{
		Graphics_D3D11_DeviceState_SetRenderTarget( NewTargetShadowMap->PF->D3D11.DepthTexture, NewTargetShadowMap->PF->D3D11.DepthTextureRTV, 0 ) ;
	}
	else
	// �`��\�摜���L���ȏꍇ�͕`��\�摜��`��Ώۂɂ���
	if( NewTargetImage )
	{
		Graphics_D3D11_DeviceState_SetRenderTarget( NewTargetImage->Hard.Draw[ 0 ].Tex->PF->D3D11.Texture, NewTargetImage->Hard.Draw[ 0 ].Tex->PF->D3D11.TextureRTV[ GSYS.DrawSetting.TargetScreenSurface[ 0 ] ], 0 ) ;
	}
	else
	// �T�u�o�b�N�o�b�t�@���L���ȏꍇ�̓T�u�o�b�N�o�b�t�@��`��Ώۂɂ���
	if( GD3D11.Device.Screen.SubBackBufferTexture2D != NULL )
	{
		Graphics_D3D11_DeviceState_SetRenderTarget( GD3D11.Device.Screen.SubBackBufferTexture2D, GD3D11.Device.Screen.SubBackBufferRTV ) ;
	}
	else
	// ����ȊO�̏ꍇ�̓o�b�N�o�b�t�@��`��Ώۂɂ���
	{
		Graphics_D3D11_DeviceState_SetRenderTarget( GD3D11.Device.Screen.OutputWindowInfo[ 0 ].BufferTexture2D, GD3D11.Device.Screen.OutputWindowInfo[ 0 ].BufferRTV ) ;
	}

	// �g�p����y�o�b�t�@�̃Z�b�g�A�b�v
	Graphics_Screen_SetupUseZBuffer() ;

	// ����I��
	return 0 ;
}

// ����I�� ScreenCopy ���s���R�[���o�b�N�֐�
#define GRAPHICS_HARDWARE_D3D11_SCREENFLIPTIMER_ID		(32767)
#if _MSC_VER > 1200 || defined( DX_GCC_COMPILE_4_9_2 )
static VOID CALLBACK Graphics_Hardware_D3D11_ScreenFlipTimerProc( HWND /*hwnd*/, UINT /*uMsg*/, UINT_PTR /*idEvent*/, DWORD /*dwTime*/ )
#else
static VOID CALLBACK Graphics_Hardware_D3D11_ScreenFlipTimerProc( HWND /*hwnd*/, UINT /*uMsg*/, UINT     /*idEvent*/, DWORD /*dwTime*/ )
#endif
{
	KillTimer( NS_GetMainWindowHandle(), GRAPHICS_HARDWARE_D3D11_SCREENFLIPTIMER_ID ) ;

	if( GSYS.DrawSetting.TargetScreen[ 0 ]        == DX_SCREEN_FRONT &&
		GSYS.DrawSetting.TargetScreenSurface[ 0 ] == 0               &&
		WinData.BackBufferTransColorFlag          == FALSE           &&
		WinData.UseUpdateLayerdWindowFlag         == FALSE )
	{
		if( NS_GetActiveFlag() == TRUE )
		{
			NS_ScreenCopy() ;
		}
		SetTimer( NS_GetMainWindowHandle(), GRAPHICS_HARDWARE_D3D11_SCREENFLIPTIMER_ID, 32, Graphics_Hardware_D3D11_ScreenFlipTimerProc ) ;
	}
}

// SetDrawScreen �̍Ō�ŌĂ΂��֐�
extern	int		Graphics_Hardware_D3D11_SetDrawScreen_Post_PF( int DrawScreen )
{
	if( DrawScreen == DX_SCREEN_FRONT )
	{
		SetTimer( NS_GetMainWindowHandle(), GRAPHICS_HARDWARE_D3D11_SCREENFLIPTIMER_ID, 32, Graphics_Hardware_D3D11_ScreenFlipTimerProc ) ;
	}

	// ����I��
	return 0 ;
}

// �`��\�̈�̃Z�b�g
extern	int		Graphics_Hardware_D3D11_SetDrawArea_PF( int x1, int y1, int x2, int y2 )
{
	D_D3D11_VIEWPORT Viewport ;

	if( GAPIWin.D3D11DeviceObject == NULL )
	{
		return -1 ;
	}

	if( GSYS.DrawSetting.DrawArea.right  - GSYS.DrawSetting.DrawArea.left == 0 ||
		GSYS.DrawSetting.DrawArea.bottom - GSYS.DrawSetting.DrawArea.top  == 0 )
	{
		return -1 ;
	}

	// �r���[�|�[�g�̃Z�b�g
	Viewport.TopLeftX	= ( float )GSYS.DrawSetting.DrawArea.left ;
	Viewport.TopLeftY	= ( float )GSYS.DrawSetting.DrawArea.top ;
	Viewport.Width		= ( float )( GSYS.DrawSetting.DrawArea.right  - GSYS.DrawSetting.DrawArea.left ) ;
	Viewport.Height		= ( float )( GSYS.DrawSetting.DrawArea.bottom - GSYS.DrawSetting.DrawArea.top  ) ;
	Viewport.MinDepth	= 0.0f ;
	Viewport.MaxDepth	= 1.0f ;
	Graphics_D3D11_DeviceState_SetViewport( &Viewport ) ;

	// �ˉe�s��ƃr���[�|�[�g�s��̋t�s�����Z�������̂��Z�b�g
	Graphics_Hardware_D3D11_RefreshProjectionMatrix() ;
	GSYS.DrawSetting.MatchHardwareProjectionMatrix = TRUE ;

	// �A���`�r���[�|�[�g�s����X�V
	Graphics_D3D11_DeviceState_SetAntiViewportMatrix( &GSYS.DrawSetting.Direct3DViewportMatrixAntiF ) ;

	// ����I��
	return 0 ;
}

// �`���o�b�t�@�����b�N����
extern	int		Graphics_Hardware_D3D11_LockDrawScreenBuffer_PF( RECT *LockRect, BASEIMAGE *BaseImage, int TargetScreen, IMAGEDATA *TargetImage, int TargetScreenSurface, int ReadOnly, int TargetScreenTextureNo )
{
	D_ID3D11Texture2D                 *TargetTexture ;
	D_ID3D11ShaderResourceView        *TargetTextureSRV ;

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// ��荞�݌��ƂȂ�T�[�t�F�X�̌���

	// �`��\�摜���Ώۂ̏ꍇ
	if( TargetImage )
	{
		TargetTexture    = TargetImage->Orig->Hard.Tex[ TargetScreenTextureNo ].PF->D3D11.Texture ;
		TargetTextureSRV = TargetImage->Orig->Hard.Tex[ TargetScreenTextureNo ].PF->D3D11.TextureSRV ;
	}
	else
	// �T�u�o�b�N�o�b�t�@���L���ɂȂ��Ă���ꍇ�̓T�u�o�b�N�o�b�t�@��������
	if( GD3D11.Device.Screen.SubBackBufferTexture2D != NULL )
	{
		TargetTexture    = GD3D11.Device.Screen.SubBackBufferTexture2D ;
		TargetTextureSRV = GD3D11.Device.Screen.SubBackBufferSRV ;
	}
	else
	{
		TargetTexture    = GD3D11.Device.Screen.OutputWindowInfo[ 0 ].BufferTexture2D ;
		TargetTextureSRV = GD3D11.Device.Screen.OutputWindowInfo[ 0 ].BufferSRV ;
	}

	// �}�b�v
	if( Graphics_D3D11_Texture2D_Map( TargetTexture, TargetTextureSRV, LockRect, BaseImage, &GD3D11.Device.Screen.DrawScreenBufferLockSMTexture ) < 0 )
	{
		DXST_ERRORLOG_ADDUTF16LE( "\xcf\x63\x3b\x75\xfe\x5b\x61\x8c\xd0\x30\xc3\x30\xd5\x30\xa1\x30\x6e\x30\xed\x30\xc3\x30\xaf\x30\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�`��Ώۃo�b�t�@�̃��b�N�Ɏ��s���܂���\n" @*/ ) ;
		return -1 ;
	}

	// ����I��
	return 0 ;
}

// �`���o�b�t�@���A�����b�N����
extern	int		Graphics_Hardware_D3D11_UnlockDrawScreenBuffer_PF( void )
{
	// �}�b�v������
	Graphics_D3D11_Texture2D_Unmap( GD3D11.Device.Screen.DrawScreenBufferLockSMTexture ) ;

	// ����I��
	return 0 ;
}

// ����ʂ̓��e��\��ʂɕ`�悷��
extern	int		Graphics_Hardware_D3D11_ScreenCopy_PF( void )
{
	// �T�u�o�b�N�o�b�t�@�̃Z�b�g�A�b�v
	if( NS_GetWindowModeFlag() == FALSE )
	{
		Graphics_D3D11_SetupSubBackBuffer() ;
	}

	// ����I��
	return 0 ;
}

// ���������M����҂�
extern	int		Graphics_Hardware_D3D11_WaitVSync_PF( int SyncNum )
{
	int i ;

	for( i = 0 ; i < SyncNum ; i ++ )
	{
		DXGIOutput_WaitForVBlank( GD3D11.Device.Screen.OutputWindowInfo[ 0 ].DXGIOutput ) ;
	}

	return 0 ;
}

// ����ʂƕ\��ʂ���������
extern	int		Graphics_Hardware_D3D11_ScreenFlipBase_PF( void )
{
	GRAPHICS_HARDDATA_DIRECT3D11_OUTPUTWINDOWINFO *OWI ;

	if( GAPIWin.D3D11DeviceObject == NULL )
	{
		return -1 ;
	}

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
		Graphics_D3D11_SetupSubBackBuffer() ;
	}

	// ����ʂ̓��e��\��ʂɔ��f
//	return DXGISwapChain_Present() ;

	// �o�͐�E�C���h�E�̏����擾���Ă���
	OWI = &GD3D11.Device.Screen.OutputWindowInfo[ GD3D11.Device.Screen.TargetOutputWindow ] ;

	// �E�C���h�E�̃N���C�A���g�̈�̃T�C�Y���o�b�N�o�b�t�@�[�̃T�C�Y�ƈقȂ�ꍇ�̓o�b�N�o�b�t�@�̃T�C�Y��ύX����
	{
		RECT TargetWindowClientRect ;
		SIZE TargetWindowClientSize ;

		if( GD3D11.Device.Screen.TargetOutputWindow == 0 )
		{
			TargetWindowClientRect = WinData.WindowRect ;
		}
		else
		{
			GetClientRect( GD3D11.Device.Screen.OutputWindowInfo[ GD3D11.Device.Screen.TargetOutputWindow ].DXGISwapChainDesc.OutputWindow, &TargetWindowClientRect ) ;
		}

		TargetWindowClientSize.cx = TargetWindowClientRect.right  - TargetWindowClientRect.left ;
		TargetWindowClientSize.cy = TargetWindowClientRect.bottom - TargetWindowClientRect.top ;
		if( ( UINT )TargetWindowClientSize.cx != OWI->BufferTexture2DDesc.Width ||
			( UINT )TargetWindowClientSize.cy != OWI->BufferTexture2DDesc.Height )
		{
			// �T�u�o�b�N�o�b�t�@���ɃZ�b�g�A�b�v����
			Graphics_D3D11_SetupSubBackBuffer() ;

			Graphics_D3D11_OutputWindow_ResizeBuffer(
				GD3D11.Device.Screen.TargetOutputWindow,
				TargetWindowClientSize.cx,
				TargetWindowClientSize.cy
			) ;
		}
	}

	// �T�u�o�b�N�o�b�t�@���g�p���Ă���ꍇ�͓��e���o�͐�E�C���h�E�̃o�b�N�o�b�t�@�ɓ]������
	if( GD3D11.Device.Screen.SubBackBufferTexture2D != NULL )
	{
		RECT SrcRect ;
		RECT DestRect = { 0 } ;

		// �T�u�o�b�N�o�b�t�@�ɓ]������̈��ݒ肷��
		if( GSYS.Screen.ValidGraphDisplayArea )
		{
			RECT ClipRect ;

			ClipRect.left   = 0 ;
			ClipRect.top    = 0 ;
			ClipRect.right  = GSYS.Screen.MainScreenSizeX ;
			ClipRect.bottom = GSYS.Screen.MainScreenSizeY ;

			SrcRect = GSYS.Screen.GraphDisplayArea ;
			RectClipping_Inline( &SrcRect, &ClipRect ) ;
			if( SrcRect.right  - SrcRect.left <= 0 ||
				SrcRect.bottom - SrcRect.top  <= 0 )
			{
				return -1 ;
			}
		}
		else
		{
			SrcRect.left   = 0 ;
			SrcRect.top    = 0 ;
			SrcRect.right  = GSYS.Screen.MainScreenSizeX ;
			SrcRect.bottom = GSYS.Screen.MainScreenSizeY ;
		}

		if( NS_GetWindowModeFlag() == TRUE )
		{
			// ���C���E�C���h�E�ȊO�ւ̓]���̏ꍇ�͍ő剻�֌W�Ȃ�
			if( GD3D11.Device.Screen.TargetOutputWindow != 0 )
			{
				DestRect.left   = 0 ;
				DestRect.top    = 0 ;
				DestRect.right  = _DTOL( GSYS.Screen.MainScreenSizeX * WinData.WindowSizeExRateX ) ;
				DestRect.bottom = _DTOL( GSYS.Screen.MainScreenSizeY * WinData.WindowSizeExRateY ) ;
			}
			else
			{
				// 320x240��ʃG�~�����[�V�������[�h�̏ꍇ�͓]����̋�`�͌Œ�
				if( GSYS.Screen.Emulation320x240Flag )
				{
					SrcRect.left    = 0 ;
					SrcRect.top     = 0 ;
					SrcRect.right   = 320 ;
					SrcRect.bottom  = 240 ;

					DestRect.left   = 0 ;
					DestRect.top    = 0 ;
					DestRect.right  = 640 ;
					DestRect.bottom = 480 ;
				}
				else
				{
					double ExRateX ;
					double ExRateY ;

					NS_GetWindowSizeExtendRate( &ExRateX, &ExRateY ) ;

					if( WinData.WindowMaximizeFlag &&
						WinData.ScreenNotFitWindowSize == FALSE &&
						WinData.WindowSizeValid == FALSE )
					{
						DestRect.left   = ( ( WinData.WindowMaximizedClientRect.right  - WinData.WindowMaximizedClientRect.left ) - _DTOL( GSYS.Screen.MainScreenSizeX * ExRateX ) ) / 2 ;
						DestRect.top    = ( ( WinData.WindowMaximizedClientRect.bottom - WinData.WindowMaximizedClientRect.top  ) - _DTOL( GSYS.Screen.MainScreenSizeY * ExRateY ) ) / 2 ;
					}
					else
					{
						DestRect.left   = 0 ;
						DestRect.top    = 0 ;
					}

					DestRect.right  = DestRect.left + _DTOL( GSYS.Screen.MainScreenSizeX * ExRateX ) ;
					DestRect.bottom = DestRect.top  + _DTOL( GSYS.Screen.MainScreenSizeY * ExRateY ) ;

					if( WinData.ToolBarUseFlag )
					{
						int ClientHeight ;

						ClientHeight   = WinData.WindowRect.bottom - WinData.WindowRect.top ;
						DestRect.top    = ClientHeight - GSYS.Screen.MainScreenSizeY ;
						DestRect.bottom = ClientHeight ;
					}
				}
			}
		}
		else
		{
			// �t���X�N���[�����[�h�̏ꍇ�̓t���X�N���[���𑜓x���[�h�ɂ���ď����𕪊�
			switch( GSYS.Screen.FullScreenResolutionModeAct )
			{
			case DX_FSRESOLUTIONMODE_NATIVE :
				DestRect.left   = 0 ;
				DestRect.top    = 0 ;
				DestRect.right  = GSYS.Screen.MainScreenSizeX ;
				DestRect.bottom = GSYS.Screen.MainScreenSizeY ;
				break ;

			case DX_FSRESOLUTIONMODE_MAXIMUM :
			case DX_FSRESOLUTIONMODE_DESKTOP :
				// �]�����`�̃Z�b�g�A�b�v
				Graphics_Screen_SetupFullScreenScalingDestRect() ;

				DestRect = GSYS.Screen.FullScreenScalingDestRect ;
				break ;
			}
		}

		if( DestRect.right  > ( int )OWI->BufferTexture2DDesc.Width )
		{
			SrcRect.right  -= ( DestRect.right - OWI->BufferTexture2DDesc.Width ) * SrcRect.right / DestRect.right ;
			DestRect.right  = ( LONG )OWI->BufferTexture2DDesc.Width ;
		}
		if( DestRect.bottom > ( int )OWI->BufferTexture2DDesc.Height )
		{
			SrcRect.bottom -= ( DestRect.bottom - OWI->BufferTexture2DDesc.Height ) * SrcRect.bottom / DestRect.bottom ;
			DestRect.bottom = ( LONG )OWI->BufferTexture2DDesc.Height ;
		}

		// �T�u�o�b�N�o�b�t�@�Ɩ{�o�b�N�o�b�t�@�̃T�C�Y���قȂ�ꍇ�͖{�o�b�N�o�b�t�@���ɃN���A����
		if( OWI->BufferTexture2DDesc.Width  != GD3D11.Device.Screen.SubBackBufferTextureSizeX ||
			OWI->BufferTexture2DDesc.Height != GD3D11.Device.Screen.SubBackBufferTextureSizeY )
		{
			FLOAT ClearColor[ 4 ] ;

			ClearColor[ 0 ] = GSYS.Screen.BackgroundRed   / 255.0f ;
			ClearColor[ 1 ] = GSYS.Screen.BackgroundGreen / 255.0f ;
			ClearColor[ 2 ] = GSYS.Screen.BackgroundBlue  / 255.0f ;
			ClearColor[ 3 ] = 0.0f ;
			D3D11DeviceContext_ClearRenderTargetView_ASync( OWI->BufferRTV, ClearColor ) ;
		}

		// �T�u�o�b�N�o�b�t�@�̓��e��{�o�b�N�o�b�t�@�ɓ]��
		Graphics_D3D11_StretchRect(
			GD3D11.Device.Screen.SubBackBufferTexture2D,
			GD3D11.Device.Screen.SubBackBufferSRV,
			&SrcRect,
			OWI->BufferTexture2D,
			OWI->BufferRTV,
			&DestRect,
			GSYS.Screen.FullScreenScalingMode == DX_FSSCALINGMODE_BILINEAR ? D_D3D11_FILTER_TYPE_LINEAR : D_D3D11_FILTER_TYPE_POINT
		) ;
	}

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �o�b�N�o�b�t�@�̓��ߐF�̕����𓧉߂���t���O���AUpdateLayerdWindow ���g�p����t���O�������Ă���ꍇ�͏����𕪊�
	if( WinData.BackBufferTransColorFlag || WinData.UseUpdateLayerdWindowFlag )
	{
		BASEIMAGE BackBufferImage ;
		RECT      LockRect ;
		int       OldTargetScreen ;
		int       OldTargetScreenSurfaceIndex ;

		// �u�r�x�m�b��҂�	
		if( GSYS.Screen.NotWaitVSyncFlag == FALSE )
		{
			DXGIOutput_WaitForVBlank( GD3D11.Device.Screen.OutputWindowInfo[ 0 ].DXGIOutput ) ;
		}

		// Graphics_Screen_LockDrawScreen ���g�����@
		OldTargetScreen                           = GSYS.DrawSetting.TargetScreen[ 0 ] ;
		OldTargetScreenSurfaceIndex               = GSYS.DrawSetting.TargetScreenSurface [ 0 ] ;
		GSYS.DrawSetting.TargetScreen[ 0 ]        = DX_SCREEN_BACK ;
		GSYS.DrawSetting.TargetScreenSurface[ 0 ] = 0 ;
		LockRect.left                             = 0 ;
		LockRect.top                              = 0 ;
		LockRect.right                            = GSYS.Screen.MainScreenSizeX ;
		LockRect.bottom                           = GSYS.Screen.MainScreenSizeY ;
		if( Graphics_Screen_LockDrawScreen( &LockRect, &BackBufferImage, -1, -1, TRUE, 0 ) < 0 )
		{
			goto ERR ;
		}
		UpdateBackBufferTransColorWindow( &BackBufferImage ) ;
		Graphics_Screen_UnlockDrawScreen() ;
		GSYS.DrawSetting.TargetScreen[ 0 ]        = OldTargetScreen ;
		GSYS.DrawSetting.TargetScreenSurface[ 0 ] = OldTargetScreenSurfaceIndex ;
	}
	else
	{
		// �t���b�v
		if( OWI->DXGISwapChain )
		{
			// �ŏ������Ă��Ȃ��Ƃ����� Present �����s
			if( WinData.WindowMinSizeFlag == FALSE )
			{
				if( FAILED( DXGISwapChain_Present( OWI->DXGISwapChain, ( UINT )( GSYS.Screen.NotWaitVSyncFlag ? 0 : 1 ), 0 ) ) )
				{
					goto ERR ;
				}
			}
			else
			{
				// �ŏ������Ă���ꍇ��VSYNC�҂�������ݒ�ɂȂ��Ă���ꍇ�� VSYNC �҂����s��
				if( GSYS.Screen.NotWaitVSyncFlag == FALSE )
				{
					DXGIOutput_WaitForVBlank( OWI->DXGIOutput ) ;
				}
			}
		}
	}

	// Direct3D 11 �ɑ΂���ݒ���ēx�s��
	NS_RefreshDxLibDirect3DSetting() ;

	// �I��
	return 0 ;

ERR:
	// �G���[�I��
	return -1;
}

// ����ʂ̎w��̗̈���E�C���h�E�̃N���C�A���g�̈�̎w��̗̈�ɓ]������
extern	int		Graphics_Hardware_D3D11_BltRectBackScreenToWindow_PF( HWND Window, RECT BackScreenRect, RECT WindowClientRect )
{
	// ������
	return 0 ;
}

// ScreenFlip �ŉ摜��]�������̃E�C���h�E��ݒ肷��( NULL ���w�肷��Ɛݒ���� )
extern	int		Graphics_Hardware_D3D11_SetScreenFlipTargetWindow_PF( HWND TargetWindow )
{
	int Result ;

	if( TargetWindow == NULL )
	{
		Result = Graphics_D3D11_OutputWindow_Add( GetDisplayWindowHandle(), TRUE ) ;
	}
	else
	{
		Result = Graphics_D3D11_OutputWindow_Add( TargetWindow ) ;
	}
	if( Result == -1 )
	{
		return -1 ;
	}

	// ���C���E�C���h�E�ȊO���^�[�Q�b�g�ɂȂ����ꍇ�̓T�u�o�b�N�o�b�t�@���Z�b�g�A�b�v����
	if( Result > 0 )
	{
		if( Graphics_D3D11_SetupSubBackBuffer() < 0 )
		{
			return -1 ;
		}
	}

	// �^�[�Q�b�g�E�C���h�E�̔z��C���f�b�N�X��ۑ�
	GD3D11.Device.Screen.TargetOutputWindow = Result ;

	// ����I��
	return 0 ;
}

// ���C����ʂ̂y�o�b�t�@�̐ݒ��ύX����
extern	int		Graphics_Hardware_D3D11_SetZBufferMode_PF( int ZBufferSizeX, int ZBufferSizeY, int ZBufferBitDepth )
{
	// �������O�̏ꍇ�̂ݗL��
	if( GAPIWin.D3D11DeviceObject != NULL )
		return 0 ;

	if( ZBufferBitDepth > 0 )
	{
		if( ZBufferBitDepth != 16 && ZBufferBitDepth != 24 && ZBufferBitDepth != 32 )
		{
			return -1 ;
		}
		GD3D11.Device.Screen.DepthBufferBitDepth = ZBufferBitDepth ;
		GD3D11.Setting.UserDepthBufferBitDepthSet = TRUE ;
	}

	return 0 ;
}

// �`���y�o�b�t�@�̃Z�b�g
extern	int		Graphics_Hardware_D3D11_SetDrawZBuffer_PF( int DrawScreen, IMAGEDATA *Image )
{
	// ������
	return 0 ;
}

// �g�p����ID3D11Device�I�u�W�F�N�g���擾����( �߂�l�� ID3D11Device * �ɃL���X�g���Ă������� )
extern const void* Graphics_Hardware_D3D11_GetUseDirect3D11Device_PF( void )
{
	return D3D11Device_Get() ;
}

// �g�p����ID3D11DeviceContext�I�u�W�F�N�g���擾����( �߂�l�� ID3D11DeviceContext * �ɃL���X�g���Ă������� )
extern const void* Graphics_Hardware_D3D11_GetUseDirect3D11DeviceContext_PF( void )
{
	return D3D11DeviceContext_Get() ;
}

// �g�p���̃o�b�N�o�b�t�@��ID3D11Texture2D�I�u�W�F�N�g���擾����( �߂�l�� ID3D11Texture2D * �ɃL���X�g���Ă������� )
extern const void* Graphics_Hardware_D3D11_GetUseDirect3D11BackBufferTexture2D_PF( void )
{
	// �T�u�o�b�N�o�b�t�@���L���ȏꍇ�̓T�u�o�b�N�o�b�t�@�̏���Ԃ�
	if( GD3D11.Device.Screen.SubBackBufferTexture2D != NULL )
	{
		return GD3D11.Device.Screen.SubBackBufferTexture2D ;
	}

	// ����ȊO�̏ꍇ�̓o�b�N�o�b�t�@�̏���Ԃ�
	return GD3D11.Device.Screen.OutputWindowInfo[ 0 ].BufferTexture2D ;
}

// �g�p���̐[�x�X�e���V���o�b�t�@��ID3D11Texture2D�I�u�W�F�N�g���擾����( �߂�l�� ID3D11Texture2D * �ɃL���X�g���Ă������� )
extern const void* Graphics_Hardware_D3D11_GetUseDirect3D11DepthStencilTexture2D_PF( void )
{
	return GD3D11.Device.Screen.DepthBufferTexture2D ;
}























// ���ˑ����擾�֌W

// GetColor �� GetColor2 �Ŏg�p����J���[�f�[�^���擾����
extern const COLORDATA *Graphics_Hardware_D3D11_GetMainColorData_PF( void )
{
	return Graphics_D3D11_GetD3DFormatColorData( DIRECT3D11_BACKBUFFER_FORMAT ) ;
}

// �f�B�X�v���[�̃J���[�f�[�^�|�C���^�𓾂�
extern	const COLORDATA *Graphics_Hardware_D3D11_GetDispColorData_PF( void )
{
	return Graphics_D3D11_GetD3DFormatColorData( GD3D11.Device.Caps.ScreenFormat ) ;
}

// �w����W�̐F���擾����
extern	DWORD Graphics_Hardware_D3D11_GetPixel_PF( int x, int y )
{
	RECT            SrcRect ;
//	const COLORDATA *DestColorData ;
	BASEIMAGE       BufferImage ;
	DWORD           Ret = 0xffffffff ;
	int				Red, Green, Blue, Alpha ;

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �`���o�b�t�@�����b�N
	SrcRect.left   = x ;
	SrcRect.right  = x + 1 ;
	SrcRect.top    = y ;
	SrcRect.bottom = y + 1 ;
	if( Graphics_Screen_LockDrawScreen( &SrcRect, &BufferImage, -1, -1, TRUE, 0 ) < 0 )
	{
		return Ret ;
	}

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

//	DestColorData = Graphics_Hardware_GetMainColorData_PF() ;
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
extern	int		Graphics_Hardware_D3D11_UpdateGraphMovie_TheoraYUV_PF( MOVIEGRAPH *Movie, IMAGEDATA *Image )
{
	// ������
	return 0 ;
}

// �O���t�B�b�N�������̈�̃��b�N
extern	int		Graphics_Hardware_D3D11_GraphLock_PF( IMAGEDATA *Image, COLORDATA **ColorDataP, int WriteOnly )
{
	COLORDATA           *ColorData ;

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �J���[�t�H�[�}�b�g���擾����
	ColorData = Graphics_D3D11_GetD3DFormatColorData( GD3D11.Device.Caps.TextureFormat[ Image->Orig->ColorFormat ] ) ;

	// �W���t�H�[�}�b�g�ȊO�ł̓��b�N�ł��Ȃ�
	if( ColorData->Format != DX_BASEIMAGE_FORMAT_NORMAL )
	{
		return -1 ;
	}

	// �e���|�����o�b�t�@���m�ۂ���
	Image->LockImagePitch	= ( DWORD )( ColorData->PixelByte * Image->WidthI ) ;
	Image->LockImage		= ( BYTE * )DXALLOC( Image->LockImagePitch * Image->HeightI ) ;
	if( Image->LockImage == NULL )
	{
		DXST_ERRORLOG_ADDUTF16LE( "\xed\x30\xc3\x30\xaf\x30\x28\x75\xc6\x30\xf3\x30\xdd\x30\xe9\x30\xea\x30\xa4\x30\xe1\x30\xfc\x30\xb8\x30\x3c\x68\x0d\x7d\x28\x75\xe1\x30\xe2\x30\xea\x30\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"���b�N�p�e���|�����C���[�W�i�[�p�������̊m�ۂɎ��s���܂���\n" @*/ ) ;
		goto ERR ;
	}

	// �������ݐ�p�ł͂Ȃ��ꍇ�̓e���|�����o�b�t�@�Ƀf�[�^��]������
	if( WriteOnly == FALSE )
	{
		int                  i ;
		BYTE                *Dest ;
		BYTE                *Src ;
		DWORD                WidthByte ;
		BASEIMAGE            MapBaseImage ;
		D_ID3D11Texture2D   *MapTempTexture ;
		IMAGEDATA_HARD_DRAW *DrawTex ;

		// �`����̐������J��Ԃ�
		DrawTex = Image->Hard.Draw ;
		for( i = 0 ; i < Image->Hard.DrawNum ; i ++, DrawTex ++ )
		{
			// �}�b�v
			if( Graphics_D3D11_Texture2D_Map(
				DrawTex->Tex->PF->D3D11.Texture,
				DrawTex->Tex->PF->D3D11.TextureSRV,
				NULL,
				&MapBaseImage,
				&MapTempTexture ) < 0 )
			{
				goto ERR ;
			}

			// �]��
			Src  = ( BYTE * )MapBaseImage.GraphData + DrawTex->UsePosXI  * ColorData->PixelByte + DrawTex->UsePosYI  * MapBaseImage.Pitch ;
			Dest = Image->LockImage                 + DrawTex->DrawPosXI * ColorData->PixelByte + DrawTex->DrawPosYI * Image->LockImagePitch ;
			WidthByte = ( DWORD )( DrawTex->WidthI * ColorData->PixelByte ) ;
			for( i = 0 ; i < DrawTex->HeightI ; i ++, Dest += Image->LockImagePitch, Src += MapBaseImage.Pitch )
			{
				_MEMCPY( Dest, Src, WidthByte ) ;
			}

			// �}�b�v�̉���
			Graphics_D3D11_Texture2D_Unmap( MapTempTexture ) ;
		}
	}

	// �J���[�f�[�^�̃A�h���X��ۑ�
	*ColorDataP = ColorData ;

	// ����I��
	return 0 ;

ERR :

	if( Image->LockImage != NULL )
	{
		DXFREE( Image->LockImage ) ;
		Image->LockImage = NULL ;
	}

	// �G���[�I��
	return -1 ;
}

// �O���t�B�b�N�������̈�̃��b�N����
extern	int		Graphics_Hardware_D3D11_GraphUnlock_PF( IMAGEDATA *Image )
{
	COLORDATA           *ColorData ;
	BASEIMAGE            BaseImage ;
	BASEIMAGE            AlphaBaseImage = { 0 } ;
	RECT                 Rect ;

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �J���[�t�H�[�}�b�g���擾����
	ColorData = Graphics_D3D11_GetD3DFormatColorData( GD3D11.Device.Caps.TextureFormat[ Image->Orig->ColorFormat ] ) ;

	// ��{�C���[�W���\�z����
	BaseImage.ColorData      = *ColorData ;
	BaseImage.Width          = Image->WidthI ;
	BaseImage.Height         = Image->HeightI ;
	BaseImage.Pitch          = ColorData->PixelByte * Image->WidthI ;
	BaseImage.GraphData      = Image->LockImage ;
	BaseImage.MipMapCount    = 0 ;
	BaseImage.GraphDataCount = 0 ;

	// �]��
	Rect.left   = 0 ;
	Rect.top    = 0 ;
	Rect.right  = Image->WidthI ;
	Rect.bottom = Image->HeightI ;
	Graphics_Hardware_D3D11_BltBmpOrBaseImageToGraph3_PF( &Rect, 0, 0, Image->HandleInfo.Handle, &BaseImage, &AlphaBaseImage, FALSE, FALSE, TRUE, FALSE ) ;

	// ���b�N�C���[�W�̉��
	if( Image->LockImage )
	{
		DXFREE( Image->LockImage ) ;
		Image->LockImage = NULL ;
	}

	// ����I��
	return 0 ;
}

// �O���t�B�b�N�̂y�o�b�t�@�̏�Ԃ�ʂ̃O���t�B�b�N�̂y�o�b�t�@�ɃR�s�[����( DestGrHandle �� SrcGrHandle ���y�o�b�t�@�������Ă���`��\�摜�ŁA���A���`�G�C���A�X�摜�ł͂Ȃ����Ƃ����� )
extern	int		Graphics_Hardware_D3D11_CopyGraphZBufferImage_PF( IMAGEDATA *DestImage, IMAGEDATA *SrcImage )
{
	// ������
	return 0 ;
}

// �摜�f�[�^�̏�����
extern	int		Graphics_Hardware_D3D11_InitGraph_PF( void )
{
	// ����I��
	return 0 ;
}

// �O���t�B�b�N�����̐F�œh��Ԃ�
extern	int		Graphics_Hardware_D3D11_FillGraph_PF( IMAGEDATA *Image, int Red, int Green, int Blue, int Alpha, int ASyncThread )
{
	// �f�o�C�X������������G���[
	if( GAPIWin.D3D11DeviceObject == NULL ) return -1 ;

	// �`����̐������J��Ԃ�
	if( Image->Orig->FormatDesc.DrawValidFlag )
	{
		IMAGEDATA_HARD_DRAW *DrawTex ;
		FLOAT               ColorRGBA[ 4 ] ;
		int                 i ;

		ColorRGBA[ 0 ] = Red   / 255.0f ;
		ColorRGBA[ 1 ] = Green / 255.0f ;
		ColorRGBA[ 2 ] = Blue  / 255.0f ;
		ColorRGBA[ 3 ] = Alpha / 255.0f ;

		DrawTex = Image->Hard.Draw ;
		for( i = 0 ; i < Image->Hard.DrawNum ; i ++, DrawTex ++ )
		{
			// �g�p����w��F�œh��Ԃ�
			D3D11DeviceContext_ClearRenderTargetView( DrawTex->Tex->PF->D3D11.TextureRTV[ i ], ColorRGBA ) ;
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
extern	int		Graphics_Hardware_D3D11_GetDrawScreenGraphBase_PF( IMAGEDATA *Image, IMAGEDATA *TargetImage, int TargetScreen, int TargetScreenSurface, int TargetScreenWidth, int TargetScreenHeight, int x1, int y1, int x2, int y2, int destX, int destY )
{
	RECT SrcRect ;
	RECT DestRect ;
	RECT Rect ;
	int Width, Height ;

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// ���ƍ������v�Z
	Width  = x2 - x1 ;
	Height = y2 - y1 ;

	// �]����ƂȂ�T�[�t�F�X���`��\�e�N�X�`���������ꍇ�͒P���ɓ]��
	if( Image->Orig->FormatDesc.DrawValidFlag )
	{
		D_ID3D11Texture2D          *TargetTexture ;
		D_ID3D11ShaderResourceView *TargetTextureSRV ;

		// ��荞�݌��ƂȂ�e�N�X�`���̌���

		// �`��\�摜���Ώۂ̏ꍇ
		if( TargetImage )
		{
			TargetTexture    = TargetImage->Orig->Hard.Tex[ 0 ].PF->D3D11.Texture ;
			TargetTextureSRV = TargetImage->Orig->Hard.Tex[ 0 ].PF->D3D11.TextureSRV ;
		}
		else
		// �T�u�o�b�N�o�b�t�@���L���ɂȂ��Ă���ꍇ�̓T�u�o�b�N�o�b�t�@��������
		if( GD3D11.Device.Screen.SubBackBufferTexture2D != NULL )
		{
			TargetTexture    = GD3D11.Device.Screen.SubBackBufferTexture2D ;
			TargetTextureSRV = GD3D11.Device.Screen.SubBackBufferSRV ;
		}
		else
		{
			TargetTexture    = GD3D11.Device.Screen.OutputWindowInfo[ 0 ].BufferTexture2D ;
			TargetTextureSRV = GD3D11.Device.Screen.OutputWindowInfo[ 0 ].BufferSRV ;
		}

		SrcRect.left    = x1 ;
		SrcRect.top     = y1 ;
		SrcRect.right   = x2 ;
		SrcRect.bottom  = y2 ;

		DestRect.left   = destX ;
		DestRect.top    = destY ;
		DestRect.right  = destX + Width ;
		DestRect.bottom = destY + Height ;

		Graphics_D3D11_StretchRect(
			TargetTexture,                                TargetTextureSRV,                                     &SrcRect,
			Image->Orig->Hard.Tex[ 0 ].PF->D3D11.Texture, Image->Orig->Hard.Tex[ 0 ].PF->D3D11.TextureRTV[ 0 ], &DestRect
		) ; 
	}
	else
	// �ʏ�̃e�N�X�`���������ꍇ�͍ŏ��Ƀ��b�N������
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

static int Graphics_D3D11_BltBmpOrBaseImageToGraph3_MipMapBlt(
	      IMAGEDATA_ORIG	*Orig,
	const RECT				*SrcRect,
	const RECT				*DestRect,
	      D_ID3D11Texture2D	*UseTex,
	      int				TexWidth,
	      int				TexHeight,
	const BASEIMAGE			*RgbBaseImage,
	const BASEIMAGE			*AlphaBaseImage,
	const COLORDATA			*DestColor,
	      int				RedIsAlphaFlag,
	      int				UseTransColorConvAlpha,
	      int				ASyncThread
)
{
	int			ImageSize ;
	int			ImageNum ;
	void		*ImageBuffer ;
	int			TempTexWidth ;
	int			TempTexHeight ;
	int			i ;
	int			j ;
	D_D3D11_BOX	DestBox ;
	int			IsDXTFormat ;
	DWORD		SrcScale ;

	IsDXTFormat =
		( RgbBaseImage->ColorData.Format == DX_BASEIMAGE_FORMAT_DXT1 ) ||
		( RgbBaseImage->ColorData.Format == DX_BASEIMAGE_FORMAT_DXT3 ) ||
		( RgbBaseImage->ColorData.Format == DX_BASEIMAGE_FORMAT_DXT5 ) ;

	ImageNum = Orig->FormatDesc.CubeMapTextureFlag ? CUBEMAP_SURFACE_NUM : 1 ;

	// �]���悪�e�N�X�`���S�̂ŁA���]�����Ƀ~�b�v�}�b�v��񂪂���ꍇ�͕���
	if( DestRect->left == 0 && DestRect->right  == TexWidth  &&
		DestRect->top  == 0 && DestRect->bottom == TexHeight &&
		RgbBaseImage->MipMapCount >= Orig->Hard.MipMapCount )
	{
		ImageBuffer = RgbBaseImage->GraphData ;
		if( IsDXTFormat )
		{
			SrcScale = ( DWORD )( RgbBaseImage->ColorData.ColorBitDepth * 4 / 8 ) ;
		}
		else
		{
			SrcScale = ( DWORD )( RgbBaseImage->ColorData.PixelByte ) ;
		}
		for( i = 0 ; i < ImageNum ; i ++ )
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

				// �e�N�X�`���ɓ]��
				DestBox.left   = 0 ;
				DestBox.top    = 0 ;
				DestBox.front  = 0 ;
				if( IsDXTFormat )
				{
					DestBox.right  = ( UINT )( ( TempTexWidth + 3 ) / 4 * 4 ) ;
					DestBox.bottom = ( UINT )( ( TempTexHeight + 3 ) / 4 * 4 ) ;
				}
				else
				{
					DestBox.right  = ( UINT )TempTexWidth ;
					DestBox.bottom = ( UINT )TempTexHeight ;
				}
				DestBox.back   = 1 ;
				D3D11DeviceContext_UpdateSubresource_ASync(
					UseTex,
					D_D3D11CalcSubresource( ( UINT )j, ( UINT )i, Orig->Hard.MipMapCount ),
					&DestBox,
					ImageBuffer,
					IsDXTFormat ? ( TempTexWidth + 3 ) / 4 * 4 * SrcScale : TempTexWidth * SrcScale,
					0,
					ASyncThread
				) ;

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
		if( IsDXTFormat  )
		{
			DXST_ERRORLOG_ADDUTF16LE( "\x44\x00\x58\x00\x54\x00\xd5\x30\xa9\x30\xfc\x30\xde\x30\xc3\x30\xc8\x30\x6e\x30\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\x6b\x30\x19\x6a\x96\x6e\x62\x5f\x0f\x5f\x6e\x30\xa4\x30\xe1\x30\xfc\x30\xb8\x30\x92\x30\xe2\x8e\x01\x90\x59\x30\x8b\x30\x53\x30\x68\x30\x6f\x30\x67\x30\x4d\x30\x7e\x30\x5b\x30\x93\x30\x0a\x00\x00"/*@ L"DXT�t�H�[�}�b�g�̃e�N�X�`���ɕW���`���̃C���[�W��]�����邱�Ƃ͂ł��܂���\n" @*/ ) ;
		}
		else
		{
			int		ImageW ;
			int		ImageH ;
			int		ImageDW ;
			int		ImageDH ;
			int		ImageSW ;
			int		ImageSH ;
			void	*Image1 ;
			void	*Image2 ;
			void	*ImageD ;
			void	*ImageS ;
			int		ImagePitch ;
			int		ts ;
			POINT	DestPoint ;
			int		RgbImageSize ;
			int		AlphaImageSize ;

			DestPoint.x = 0 ;
			DestPoint.y = 0 ;

			// �ꎞ�o�b�t�@�Ɋi�[
			ts = DestRect->right  - DestRect->left ;
			for( ImageW = 1 ; ImageW < ts ; ImageW <<= 1 ){}
			ts = DestRect->bottom - DestRect->top  ;
			for( ImageH = 1 ; ImageH < ts ; ImageH <<= 1 ){}
			ImagePitch  = DestColor->PixelByte * ImageW ;
			ImageSize   = ImagePitch * ImageH ;

			if( ASyncThread )
			{
				ImageBuffer = DXALLOC( ( size_t )( ImageSize * 2 ) ) ;
				if( ImageBuffer == NULL )
				{
					DXST_ERRORLOG_ADDUTF16LE( "\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\x78\x30\xe2\x8e\x01\x90\x59\x30\x8b\x30\x3b\x75\xcf\x50\x92\x30\x00\x4e\x42\x66\x84\x76\x6b\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�e�N�X�`���֓]������摜���ꎞ�I�Ɋi�[���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ ) ;
					return -1 ;
				}
			}
			else
			{
				if( Graphics_D3D11_Texture_SetupCommonBuffer( ( unsigned int )( ImageSize * 2 ) ) < 0 )
				{
					return -1 ;
				}
				ImageBuffer = GD3D11.Texture.CommonBuffer ;
			}
			Image1      = ImageBuffer ;
			Image2      = ( BYTE * )Image1 + ImageSize ;

			RgbImageSize   = RgbBaseImage->Pitch   * RgbBaseImage->Height ;
			AlphaImageSize = AlphaBaseImage->Pitch * AlphaBaseImage->Height ;
			for( i = 0 ; i < ImageNum ; i ++ )
			{
				// �~�b�v�}�b�v�̐[�������J��Ԃ�
				ImageSW = ImageW ;
				ImageSH = ImageH ;
				ImageDW = ImageW ;
				ImageDH = ImageH ;
				for( j = 0 ; j < Orig->Hard.MipMapCount ; j ++ )
				{
					// �k���摜�̍쐬
					if( j == 0 )
					{
						// level0 �̎��͓��{�̏o�̓t�H�[�}�b�g�摜���쐬����
						NS_GraphColorMatchBltVer2(
							Image1,                                                   ImagePitch,            DestColor,
							( BYTE * )RgbBaseImage->GraphData   + RgbImageSize   * i, RgbBaseImage->Pitch,   &RgbBaseImage->ColorData,
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

					// �e�N�X�`���ɓ]��
					DestBox.left   = ( DWORD )DestRect->left   >> j ;
					DestBox.top    = ( DWORD )DestRect->top    >> j ;
					DestBox.front  = 0 ;
					DestBox.right  = ( DWORD )DestRect->right  >> j ;
					DestBox.bottom = ( DWORD )DestRect->bottom >> j ;
					DestBox.back   = 1 ;
					if( DestBox.right == DestBox.left )
					{
						DestBox.right = DestBox.left + 1 ;
					}
					if( DestBox.bottom == DestBox.top )
					{
						DestBox.bottom = DestBox.top + 1 ;
					}

					D3D11DeviceContext_UpdateSubresource_ASync( UseTex, D_D3D11CalcSubresource( ( UINT )j, ( UINT )i, 1 ), &DestBox, ImageD, ( DWORD )ImagePitch, 0, ASyncThread ) ;

					// ���̃C���[�W�̃T�C�Y���Z�b�g
					ImageSW = ImageDW ;
					ImageSH = ImageDH ;
					ImageDW >>= 1 ;
					ImageDH >>= 1 ;
					if( ImageDW == 0 ) ImageDW = 1 ;
					if( ImageDH == 0 ) ImageDH = 1 ;
				}
			}

			// �������̉��
			if( ASyncThread )
			{
				DXFREE( ImageBuffer ) ;
			}
		}
	}

	// ����I��
	return 0 ;
}

static int Graphics_D3D11_BltBmpOrBaseImageToGraph3_NoMipMapBlt(
	      IMAGEDATA_ORIG	*Orig,
	const RECT				*SrcRect,
	const RECT				*DestRect,
	      D_ID3D11Texture2D	*UseTex,
	      int				TexWidth,
	      int				TexHeight,
	      int				TexUseWidth,
	      int				TexUseHeight,
	const BASEIMAGE			*RgbBaseImage,
	const BASEIMAGE			*AlphaBaseImage,
	const COLORDATA			*DestColor,
	      int				RedIsAlphaFlag,
	      int				UseTransColorConvAlpha,
	      int				ASyncThread
)
{
	int				i ;
	int				IsDXTFormat ;
	int				k ;
	POINT			DestPoint ;
	int				H ;
	BYTE			*DestAddr ;
	BYTE			*SrcRgbAddr ;
	BYTE			*SrcAlphaAddr ;
	int				RightLineFillFlag ;
	int				BottomLineFillFlag ;
	int				ImageNum ;
	DWORD			ImageSize ;
	DWORD			AlphaImageSize ;
	void			*TempBuffer = NULL ;
	unsigned int	TempBufferSize ;
	int				TempBufferPitch = 0 ;
	D_D3D11_BOX		DestBox ;

	IsDXTFormat =
		( RgbBaseImage->ColorData.Format == DX_BASEIMAGE_FORMAT_DXT1 ) ||
		( RgbBaseImage->ColorData.Format == DX_BASEIMAGE_FORMAT_DXT3 ) ||
		( RgbBaseImage->ColorData.Format == DX_BASEIMAGE_FORMAT_DXT5 ) ;

	DestPoint.x = 0 ;
	DestPoint.y = 0 ;

	RightLineFillFlag  = FALSE ;
	BottomLineFillFlag = FALSE ;

	if( IsDXTFormat == FALSE )
	{
		if( TexUseWidth  < TexWidth  && TexUseWidth  == DestRect->right )
		{
			RightLineFillFlag = TRUE ;
		}
		if( TexUseHeight < TexHeight && TexUseHeight == DestRect->bottom )
		{
			BottomLineFillFlag = TRUE ;
		}

		TempBufferPitch =                   ( TexUseWidth  + ( RightLineFillFlag  ? 1 : 0 ) ) * DestColor->PixelByte ;
		TempBufferSize  = ( unsigned int )( ( TexUseHeight + ( BottomLineFillFlag ? 1 : 0 ) ) * TempBufferPitch ) ;

		if( ASyncThread )
		{
			TempBuffer      = DXALLOC( TempBufferSize ) ;
			if( TempBuffer == NULL )
			{
				DXST_ERRORLOGFMT_ADDUTF16LE(( "\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\x6b\x30\x3b\x75\xcf\x50\x92\x30\xe2\x8e\x01\x90\x59\x30\x8b\x30\x5f\x30\x81\x30\x6e\x30\xe1\x30\xe2\x30\xea\x30\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x5b\x00\x47\x00\x72\x00\x61\x00\x70\x00\x68\x00\x69\x00\x63\x00\x73\x00\x5f\x00\x44\x00\x33\x00\x44\x00\x31\x00\x31\x00\x5f\x00\x42\x00\x6c\x00\x74\x00\x42\x00\x6d\x00\x70\x00\x4f\x00\x72\x00\x42\x00\x61\x00\x73\x00\x65\x00\x49\x00\x6d\x00\x61\x00\x67\x00\x65\x00\x54\x00\x6f\x00\x47\x00\x72\x00\x61\x00\x70\x00\x68\x00\x33\x00\x5f\x00\x4e\x00\x6f\x00\x4d\x00\x69\x00\x70\x00\x4d\x00\x61\x00\x70\x00\x42\x00\x6c\x00\x74\x00\x5d\x00\x0a\x00\x00"/*@ L"�e�N�X�`���ɉ摜��]�����邽�߂̃������̊m�ۂɎ��s���܂���[Graphics_D3D11_BltBmpOrBaseImageToGraph3_NoMipMapBlt]\n" @*/ )) ;
				return -1 ;
			}
		}
		else
		{
			if( Graphics_D3D11_Texture_SetupCommonBuffer( TempBufferSize ) < 0 )
			{
				return -1 ;
			}
			TempBuffer      = GD3D11.Texture.CommonBuffer ;
		}
	}

	ImageNum = Orig->FormatDesc.CubeMapTextureFlag ? CUBEMAP_SURFACE_NUM : 1 ;

	for( i = 0 ; i < ImageNum ; i ++ )
	{
		// �]������

		// �]���悪�e�N�X�`���S�̂ŁA����DXT�t�H�[�}�b�g�̏ꍇ�͒P���ȃ������]�����s��
		if( DestRect->left == 0 && DestRect->right  == TexWidth  &&
			DestRect->top  == 0 && DestRect->bottom == TexHeight && IsDXTFormat )
		{
			ImageSize = ( unsigned int )( RgbBaseImage->Width * RgbBaseImage->Height * RgbBaseImage->ColorData.ColorBitDepth / 8 ) ;

			DestBox.left   = 0 ;
			DestBox.top    = 0 ;
			DestBox.front  = 0 ;
			DestBox.right  = ( UINT )TexWidth ;
			DestBox.bottom = ( UINT )TexHeight ;
			DestBox.back   = 1 ;

			D3D11DeviceContext_UpdateSubresource_ASync(
				UseTex,
				D_D3D11CalcSubresource( 0, ( UINT )i, 1 ),
				&DestBox,
				( BYTE * )RgbBaseImage->GraphData + ImageSize * i,
				( UINT )( TexWidth * ( RgbBaseImage->ColorData.ColorBitDepth * 4 / 8 ) ),
				0,
				ASyncThread
			) ;
		}
		else
		{
			// �]���悪�W���t�H�[�}�b�g�ł͂Ȃ��ꍇ�͓]���ł��Ȃ�
			if( IsDXTFormat == FALSE )
			{
				ImageSize      = ( DWORD )( RgbBaseImage->Pitch   * RgbBaseImage->Height   ) ;
				AlphaImageSize = ( DWORD )( AlphaBaseImage->Pitch * AlphaBaseImage->Height ) ;

				SrcRgbAddr   = ( BYTE * )RgbBaseImage->GraphData   + i * ImageSize ;
				SrcAlphaAddr = ( BYTE * )AlphaBaseImage->GraphData + i * AlphaImageSize ;
				NS_GraphColorMatchBltVer2(
					TempBuffer,     TempBufferPitch,       DestColor,
					SrcRgbAddr,     RgbBaseImage->Pitch,   &RgbBaseImage->ColorData,
					SrcAlphaAddr,   AlphaBaseImage->Pitch, &AlphaBaseImage->ColorData,
					DestPoint, SrcRect, FALSE,
					UseTransColorConvAlpha && ( DestColor->AlphaWidth != 0 && AlphaBaseImage->GraphData == NULL ), Orig->TransCode,
					BASEIM.ImageShavedMode, FALSE,
					RedIsAlphaFlag, FALSE,
					FALSE ) ;

				if( BottomLineFillFlag )
				{
					_MEMCPY( ( BYTE * )TempBuffer + ( TexUseHeight - DestRect->top     ) * TempBufferPitch,
							 ( BYTE * )TempBuffer + ( TexUseHeight - DestRect->top - 1 ) * TempBufferPitch,
							 ( size_t )( DestColor->PixelByte * ( SrcRect->right - SrcRect->left ) ) ) ;
				}
				if( RightLineFillFlag )
				{
					H = SrcRect->bottom - SrcRect->top ;
					DestAddr = ( BYTE * )TempBuffer + ( TexUseWidth - DestRect->left ) * DestColor->PixelByte ;
					switch( DestColor->PixelByte )
					{
					case 2 :
						for( k = 0 ; k < H ; k ++, DestAddr += TempBufferPitch )
						{
							*( ( WORD * )DestAddr ) = *( ( WORD * )DestAddr - 1 ) ;
						}
						break ;

					case 4 :
						for( k = 0 ; k < H ; k ++, DestAddr += TempBufferPitch )
						{
							*( ( DWORD * )DestAddr ) = *( ( DWORD * )DestAddr - 1 ) ;
						}
						break ;
					}
				}
				if( BottomLineFillFlag && RightLineFillFlag )
				{
					DestAddr = ( BYTE * )TempBuffer + ( TexUseHeight - DestRect->top ) * TempBufferPitch + ( TexUseWidth - DestRect->left ) * DestColor->PixelByte ;
					switch( DestColor->PixelByte )
					{
					case 2 : *( ( WORD  * )DestAddr ) = *( ( WORD  * )( DestAddr - TempBufferPitch ) - 1 ) ; break ;
					case 4 : *( ( DWORD * )DestAddr ) = *( ( DWORD * )( DestAddr - TempBufferPitch ) - 1 ) ;	break ;
					}
				}

				// �e�N�X�`���֓]��
				DestBox.left   = ( UINT )DestRect->left ;
				DestBox.top    = ( UINT )DestRect->top ;
				DestBox.front  = 0 ;
				DestBox.right  = ( UINT )DestRect->right ;
				DestBox.bottom = ( UINT )DestRect->bottom ;
				DestBox.back   = 1 ;

				D3D11DeviceContext_UpdateSubresource_ASync( UseTex, D_D3D11CalcSubresource( 0, ( UINT )i, 1 ), &DestBox, TempBuffer, ( UINT )TempBufferPitch, 0, ASyncThread ) ;
			}
			else
			{
				DXST_ERRORLOG_ADDUTF16LE( "\x44\x00\x58\x00\x54\x00\xd5\x30\xa9\x30\xfc\x30\xde\x30\xc3\x30\xc8\x30\x6e\x30\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\x6b\x30\x19\x6a\x96\x6e\x62\x5f\x0f\x5f\x6e\x30\xa4\x30\xe1\x30\xfc\x30\xb8\x30\x92\x30\xe2\x8e\x01\x90\x59\x30\x8b\x30\x53\x30\x68\x30\x6f\x30\x67\x30\x4d\x30\x7e\x30\x5b\x30\x93\x30\x0a\x00\x00"/*@ L"DXT�t�H�[�}�b�g�̃e�N�X�`���ɕW���`���̃C���[�W��]�����邱�Ƃ͂ł��܂���\n" @*/ ) ;
			}
		}
	}

	if( IsDXTFormat == FALSE )
	{
		if( ASyncThread )
		{
			if( TempBuffer != NULL )
			{
				DXFREE( TempBuffer ) ;
				TempBuffer = NULL ;
			}
		}
	}

	// ����I��
	return 0 ;
}

// Direct3D11 �̃O���t�B�b�N�n���h���ɉ摜�f�[�^��]�����邽�߂̊֐�
extern	int		Graphics_Hardware_D3D11_BltBmpOrBaseImageToGraph3_PF(
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
	IMAGEDATA_ORIG_HARD_TEX		*OrigTex ;
	IMAGEDATA_HARD_DRAW			*DrawTex ;
	int							ImageNum ;
	int							i ;
	POINT						DestPoint ;
	RECT						DestRect ;
	RECT						MoveRect ;
	COLORDATA					*DestColor ;
	D_ID3D11Texture2D			*UseTex ;
	int							SrcWidth ;
	int							SrcHeight ;

	if( GAPIWin.D3D11DeviceObject == NULL )
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
	DestColor = Graphics_D3D11_GetD3DFormatColorData( GD3D11.Device.Caps.TextureFormat[ Orig->ColorFormat ] ) ;

	// �]���ʒu�̃Z�b�g
	DestPoint.x = 0 ;
	DestPoint.y = 0 ;

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

			UseTex = OrigTex->PF->D3D11.Texture ;

			// �~�b�v�}�b�v���ǂ����ŏ����𕪊�
			if( Orig->Hard.MipMapCount > 1 )
			{
				if( Graphics_D3D11_BltBmpOrBaseImageToGraph3_MipMapBlt(
						Orig,
						&MoveRect,
						&DestRect,
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
				if( Graphics_D3D11_BltBmpOrBaseImageToGraph3_NoMipMapBlt(
						Orig,
						&MoveRect,
						&DestRect,
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

			UseTex = DrawTex->Tex->PF->D3D11.Texture ;

			// �~�b�v�}�b�v���ǂ����ŏ����𕪊�
			if( Orig->Hard.MipMapCount > 1 )
			{
				if( Graphics_D3D11_BltBmpOrBaseImageToGraph3_MipMapBlt(
						Orig,
						&MoveRect,
						&DestRect,
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
				if( Graphics_D3D11_BltBmpOrBaseImageToGraph3_NoMipMapBlt(
						Orig,
						&MoveRect,
						&DestRect,
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
		}
	}

	// ����I��
	return 0 ;


	// �G���[�I��
ERR :
	return -1 ;
}

// �I���W�i���摜��񒆂̃e�N�X�`�����쐬����( 0:����  -1:���s )
extern	int		Graphics_Hardware_D3D11_CreateOrigTexture_PF(  IMAGEDATA_ORIG *Orig, int ASyncThread )
{
	int									i ;
	int                                 j ;
	int                                 RTVNum ;
	IMAGEDATA_ORIG_HARD_TEX				*OrigTex ;
	D_D3D11_TEXTURE2D_DESC				Texture2DDesc ;
	D_D3D11_RENDER_TARGET_VIEW_DESC		RTVDesc ;
	D_D3D11_SHADER_RESOURCE_VIEW_DESC	SRVDesc ;
	D_D3D11_DEPTH_STENCIL_VIEW_DESC		DepthStencilViewDesc ;
	HRESULT								hr ;
	UINT								Samples = 1 ;
	UINT								Quality = 0 ;

	if( GAPIWin.D3D11DeviceObject == NULL )
	{
		return -1 ;
	}

	// �n�[�h�E�G�A���g�p����摜�ł͂Ȃ��ꍇ�͉��������ɏI��
	if( Orig->FormatDesc.TextureFlag == FALSE )
	{
		return 0 ;
	}

	// �w��̃t�H�[�}�b�g���쐬�ł��Ȃ��ꍇ�͎��s
	if( GD3D11.Device.Caps.TextureFormat[ Orig->ColorFormat ] == D_DXGI_FORMAT_UNKNOWN )
	{
		DXST_ERRORLOG_ADDUTF16LE( "\x07\x63\x9a\x5b\x6e\x30\xd5\x30\xa9\x30\xfc\x30\xde\x30\xc3\x30\xc8\x30\x67\x30\x5c\x4f\x10\x62\x67\x30\x4d\x30\x8b\x30\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\xd5\x30\xa9\x30\xfc\x30\xde\x30\xc3\x30\xc8\x30\x4c\x30\x42\x30\x8a\x30\x7e\x30\x5b\x30\x93\x30\x67\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�w��̃t�H�[�}�b�g�ō쐬�ł���e�N�X�`���t�H�[�}�b�g������܂���ł���\n" @*/ ) ;
		return -1 ;
	}

	// �}���`�T���v���t�H�[�}�b�g�̏ꍇ�͎g�p�ł���T���v�����ƃN�I���e�B���擾���Ă���
	if( Orig->FormatDesc.MSSamples != 0 )
	{
		// �L���[�u�}�b�v���� D_D3D_FEATURE_LEVEL_10_0 �����̏ꍇ�̓}���`�T���v���͎g�p�ł��Ȃ�
		if( Orig->FormatDesc.CubeMapTextureFlag || GD3D11.Setting.FeatureLevel < D_D3D_FEATURE_LEVEL_10_0 )
		{
			Samples = 1 ;
			Quality = 0 ;
		}
		else
		{
			Samples = ( UINT )Orig->FormatDesc.MSSamples ;
			Quality = ( UINT )Orig->FormatDesc.MSQuality ;
			D3D11Device_CheckMultiSampleParam_ASync( GD3D11.Device.Caps.TextureFormat[ Orig->ColorFormat ], &Samples, &Quality, FALSE, ASyncThread ) ;
		}
	}

	// �e�N�X�`���̏ꍇ�̓e�N�X�`���̐������e�N�X�`�����쐬
	OrigTex = Orig->Hard.Tex ;
	for( i = 0 ; i < Orig->Hard.TexNum ; i ++, OrigTex ++ )
	{
		// Direct3D11 �ɊǗ������͖���
		Orig->FormatDesc.UseManagedTextureFlag = FALSE ;

		// �}���`�T���v���t�H�[�}�b�g�̏ꍇ�̓}���`�T���v���e�N�X�`�����쐬
		if( Samples > 1 )
		{
			_MEMSET( &Texture2DDesc, 0, sizeof( Texture2DDesc ) ) ;
			Texture2DDesc.Usage              = D_D3D11_USAGE_DEFAULT ;
//			Texture2DDesc.Usage              = D_D3D11_USAGE_DYNAMIC ;
			Texture2DDesc.Format             = GD3D11.Device.Caps.TextureFormat[ Orig->ColorFormat ] ;
			Texture2DDesc.BindFlags          = ( UINT )( D_D3D11_BIND_SHADER_RESOURCE | ( Orig->FormatDesc.DrawValidFlag ? D_D3D11_BIND_RENDER_TARGET : 0 ) ) ;
			Texture2DDesc.Width              = ( UINT )OrigTex->TexWidth ;
			Texture2DDesc.Height             = ( UINT )OrigTex->TexHeight ;
			Texture2DDesc.CPUAccessFlags     = 0 ;
//			Texture2DDesc.CPUAccessFlags     = D_D3D11_CPU_ACCESS_WRITE ;
			Texture2DDesc.MipLevels          = ( UINT )Orig->Hard.MipMapCount ;
			Texture2DDesc.ArraySize          = ( UINT )( Orig->FormatDesc.CubeMapTextureFlag ? CUBEMAP_SURFACE_NUM : 1 ) ;
			Texture2DDesc.SampleDesc.Count   = Samples ;
			Texture2DDesc.SampleDesc.Quality = Quality ;
			Texture2DDesc.MiscFlags          = ( UINT )( Orig->FormatDesc.CubeMapTextureFlag ? D_D3D11_RESOURCE_MISC_TEXTURECUBE : 0 ) ;
			hr = D3D11Device_CreateTexture2D_ASync( &Texture2DDesc, NULL, &OrigTex->PF->D3D11.MSTexture, ASyncThread ) ;
			if( FAILED( hr ) )
			{
				DXST_ERRORLOG_ADDUTF16LE( "\xde\x30\xeb\x30\xc1\x30\xb5\x30\xf3\x30\xd7\x30\xeb\x30\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�}���`�T���v���e�N�X�`���̍쐬�Ɏ��s���܂���\n" @*/ ) ;
				return -1 ;
			}
		}

		// �e�N�X�`���̍쐬
		if( OrigTex->PF->D3D11.Texture == NULL )
		{
			_MEMSET( &Texture2DDesc, 0, sizeof( Texture2DDesc ) ) ;
			Texture2DDesc.Usage              = D_D3D11_USAGE_DEFAULT ;
//			Texture2DDesc.Usage              = D_D3D11_USAGE_DYNAMIC ;
			Texture2DDesc.Format             = GD3D11.Device.Caps.TextureFormat[ Orig->ColorFormat ] ;
			Texture2DDesc.BindFlags          = ( UINT )( D_D3D11_BIND_SHADER_RESOURCE | ( Orig->FormatDesc.DrawValidFlag ? D_D3D11_BIND_RENDER_TARGET : 0 ) ) ;
			Texture2DDesc.Width              = ( UINT )OrigTex->TexWidth ;
			Texture2DDesc.Height             = ( UINT )OrigTex->TexHeight ;
			Texture2DDesc.CPUAccessFlags     = 0 ;
//			Texture2DDesc.CPUAccessFlags     = D_D3D11_CPU_ACCESS_WRITE ;
			Texture2DDesc.MipLevels          = ( UINT )Orig->Hard.MipMapCount ;
			Texture2DDesc.ArraySize          = ( UINT )( Orig->FormatDesc.CubeMapTextureFlag ? CUBEMAP_SURFACE_NUM : 1 ) ;
			Texture2DDesc.SampleDesc.Count   = 1 ;
			Texture2DDesc.SampleDesc.Quality = 0 ;
			Texture2DDesc.MiscFlags          = ( UINT )( Orig->FormatDesc.CubeMapTextureFlag ? D_D3D11_RESOURCE_MISC_TEXTURECUBE : 0 ) ;
			hr = D3D11Device_CreateTexture2D_ASync( &Texture2DDesc, NULL, &OrigTex->PF->D3D11.Texture, ASyncThread ) ;
			if( FAILED( hr ) )
			{
				DXST_ERRORLOG_ADDUTF16LE( "\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�e�N�X�`���̍쐬�Ɏ��s���܂���\n" @*/ ) ;
				return -1 ;
			}
		}

		// �`��Ώۂɂ��邱�Ƃ��ł���ꍇ�͏����𕪊�
		if( Orig->FormatDesc.DrawValidFlag )
		{
			RTVNum = Orig->FormatDesc.CubeMapTextureFlag ? CUBEMAP_SURFACE_NUM : 1 ;
			for( j = 0 ; j < RTVNum ; j ++ )
			{
				// �����_�[�^�[�Q�b�g�r���[���쐬
				if( OrigTex->PF->D3D11.TextureRTV[ j ] == NULL )
				{
					_MEMSET( &RTVDesc, 0, sizeof( RTVDesc ) );
					RTVDesc.Format = GD3D11.Device.Caps.TextureFormat[ Orig->ColorFormat ] ;
					if( Orig->FormatDesc.CubeMapTextureFlag )
					{
						RTVDesc.ViewDimension                  = D_D3D11_RTV_DIMENSION_TEXTURE2DARRAY ;
						RTVDesc.Texture2DArray.FirstArraySlice = ( UINT )j ;
						RTVDesc.Texture2DArray.ArraySize       = 1 ;
					}
					else
					{
						if( Samples <= 1 )
						{
							RTVDesc.ViewDimension = D_D3D11_RTV_DIMENSION_TEXTURE2D ;
						}
						else
						{
							RTVDesc.ViewDimension = D_D3D11_RTV_DIMENSION_TEXTURE2DMS ;
						}
					}
					hr = D3D11Device_CreateRenderTargetView_ASync(
						OrigTex->PF->D3D11.MSTexture != NULL ? OrigTex->PF->D3D11.MSTexture : OrigTex->PF->D3D11.Texture,
						&RTVDesc,
						&OrigTex->PF->D3D11.TextureRTV[ j ],
						ASyncThread
					) ;
					if( FAILED( hr ) )
					{
						DXST_ERRORLOGFMT_ADDUTF16LE(( "\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\x28\x75\xec\x30\xf3\x30\xc0\x30\xfc\x30\xbf\x30\xfc\x30\xb2\x30\xc3\x30\xc8\x30\xd3\x30\xe5\x30\xfc\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x20\x00\x20\x00\x46\x00\x6f\x00\x72\x00\x6d\x00\x61\x00\x74\x00\x3a\x00\x25\x00\x64\x00\x20\x00\x20\x00\x53\x00\x69\x00\x7a\x00\x65\x00\x58\x00\x3a\x00\x25\x00\x64\x00\x20\x00\x20\x00\x53\x00\x69\x00\x7a\x00\x65\x00\x59\x00\x3a\x00\x25\x00\x64\x00\x20\x00\x0a\x00\x00"/*@ L"�e�N�X�`���p�����_�[�^�[�Q�b�g�r���[�̍쐬�Ɏ��s���܂���  Format:%d  SizeX:%d  SizeY:%d \n" @*/,
							GD3D11.Device.Caps.TextureFormat[ Orig->ColorFormat ], OrigTex->TexWidth, OrigTex->TexHeight )) ;
						return -1 ;
					}
				}
			}

			// �}���`�T���v�����A�[�x�o�b�t�@���쐬����w�肪����ꍇ�͐[�x�o�b�t�@���쐬
			if( ( ( GSYS.Setting.FSAAMultiSampleCount == 1 && Samples >  1 ) ||
				  ( GSYS.Setting.FSAAMultiSampleCount >  1 && Samples == 1 ) ||
				  Orig->ZBufferFlag ) )
			{
				if( OrigTex->PF->D3D11.DepthBuffer != NULL )
				{
					Direct3D11_Release_Texture2D_ASync( OrigTex->PF->D3D11.DepthBuffer ) ;
					OrigTex->PF->D3D11.DepthBuffer = NULL ;
				}

				if( OrigTex->PF->D3D11.DepthBufferDSV != NULL )
				{
					Direct3D11_Release_DepthStencilView_ASync( OrigTex->PF->D3D11.DepthBufferDSV ) ;
					OrigTex->PF->D3D11.DepthBufferDSV = NULL ;
				}

				// �e�N�X�`���̍쐬
				_MEMSET( &Texture2DDesc, 0, sizeof( Texture2DDesc ) ) ;
				Texture2DDesc.Width              = ( UINT )OrigTex->TexWidth ;
				Texture2DDesc.Height             = ( UINT )OrigTex->TexHeight ;
				Texture2DDesc.MipLevels          = 1 ;
				Texture2DDesc.ArraySize          = 1 ;
				Texture2DDesc.Format             = GD3D11.Device.Caps.DepthBufferTexture2DFormat[ Orig->ZBufferBitDepthIndex ] ;
				Texture2DDesc.SampleDesc.Count   = Samples ;
				Texture2DDesc.SampleDesc.Quality = Quality ;
				Texture2DDesc.Usage              = D_D3D11_USAGE_DEFAULT ;
				Texture2DDesc.BindFlags          = D_D3D11_BIND_DEPTH_STENCIL ;
				Texture2DDesc.CPUAccessFlags     = 0 ;
				Texture2DDesc.MiscFlags          = 0 ;
				hr = D3D11Device_CreateTexture2D_ASync( &Texture2DDesc, NULL, &OrigTex->PF->D3D11.DepthBuffer, ASyncThread ) ;
				if( FAILED( hr ) )
				{
					DXST_ERRORLOG_ADDUTF16LE( "\xf8\x66\x4d\x30\xbc\x8f\x7f\x30\x02\x5c\x28\x75\xde\x30\xeb\x30\xc1\x30\xb5\x30\xf3\x30\xd7\x30\xeb\x30\xf1\x6d\xa6\x5e\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�������ݐ�p�}���`�T���v���[�x�e�N�X�`���̍쐬�Ɏ��s���܂���\n" @*/ ) ;
					return -1 ;
				}

				// �[�x�X�e���V���r���[�̍쐬
				_MEMSET( &DepthStencilViewDesc, 0, sizeof( DepthStencilViewDesc ) ) ;
				DepthStencilViewDesc.Format = GD3D11.Device.Caps.DepthBufferDepthStencilFormat[ Orig->ZBufferBitDepthIndex ] ;
				if( Samples <= 1 )
				{
					DepthStencilViewDesc.ViewDimension      = D_D3D11_DSV_DIMENSION_TEXTURE2D;
					DepthStencilViewDesc.Texture2D.MipSlice = 0;
				}
				else
				{
					DepthStencilViewDesc.ViewDimension		= D_D3D11_DSV_DIMENSION_TEXTURE2DMS;
				}

				hr = D3D11Device_CreateDepthStencilView_ASync( OrigTex->PF->D3D11.DepthBuffer, &DepthStencilViewDesc, &OrigTex->PF->D3D11.DepthBufferDSV, ASyncThread ) ;
				if( FAILED( hr ) )
				{
					DXST_ERRORLOG_ADDUTF16LE( "\xf1\x6d\xa6\x5e\xd0\x30\xc3\x30\xd5\x30\xa1\x30\x28\x75\xf1\x6d\xa6\x5e\xb9\x30\xc6\x30\xf3\x30\xb7\x30\xeb\x30\xd3\x30\xe5\x30\xfc\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�[�x�o�b�t�@�p�[�x�X�e���V���r���[�̍쐬�Ɏ��s���܂���\n" @*/ ) ;
					return -1 ;
				}
			}
			else
			{
				if( OrigTex->PF->D3D11.DepthBuffer != NULL )
				{
					Direct3D11_Release_Texture2D_ASync( OrigTex->PF->D3D11.DepthBuffer ) ;
					OrigTex->PF->D3D11.DepthBuffer = NULL ;
				}

				if( OrigTex->PF->D3D11.DepthBufferDSV != NULL )
				{
					Direct3D11_Release_DepthStencilView_ASync( OrigTex->PF->D3D11.DepthBufferDSV ) ;
					OrigTex->PF->D3D11.DepthBufferDSV = NULL ;
				}
			}
		}

		// �V�F�[�_�[���\�[�X�r���[���쐬
		if( OrigTex->PF->D3D11.TextureSRV == NULL )
		{
			_MEMSET( &SRVDesc, 0, sizeof( SRVDesc ) );
			SRVDesc.Format              = GD3D11.Device.Caps.TextureFormat[ Orig->ColorFormat ] ;
			if( Orig->FormatDesc.CubeMapTextureFlag )
			{
				SRVDesc.ViewDimension               = D_D3D11_SRV_DIMENSION_TEXTURECUBE ;
				SRVDesc.TextureCube.MipLevels       = 1 ;
				SRVDesc.TextureCube.MostDetailedMip = 0 ;
			}
			else
			{
//				if( Samples <= 1 )
//				{
					SRVDesc.ViewDimension       = D_D3D11_SRV_DIMENSION_TEXTURE2D ;
					SRVDesc.Texture2D.MipLevels = ( UINT )Orig->Hard.MipMapCount ;
//				}
//				else
//				{
//					SRVDesc.ViewDimension       = D_D3D11_SRV_DIMENSION_TEXTURE2DMS ;
//				}
			}
			hr = D3D11Device_CreateShaderResourceView_ASync( OrigTex->PF->D3D11.Texture, &SRVDesc, &OrigTex->PF->D3D11.TextureSRV, ASyncThread ) ;
			if( FAILED( hr ) )
			{
				DXST_ERRORLOGFMT_ADDUTF16LE(( "\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\x28\x75\xb7\x30\xa7\x30\xfc\x30\xc0\x30\xfc\x30\xea\x30\xbd\x30\xfc\x30\xb9\x30\xd3\x30\xe5\x30\xfc\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x20\x00\x20\x00\x46\x00\x6f\x00\x72\x00\x6d\x00\x61\x00\x74\x00\x3a\x00\x25\x00\x64\x00\x20\x00\x20\x00\x53\x00\x69\x00\x7a\x00\x65\x00\x58\x00\x3a\x00\x25\x00\x64\x00\x20\x00\x20\x00\x53\x00\x69\x00\x7a\x00\x65\x00\x59\x00\x3a\x00\x25\x00\x64\x00\x20\x00\x0a\x00\x00"/*@ L"�e�N�X�`���p�V�F�[�_�[���\�[�X�r���[�̍쐬�Ɏ��s���܂���  Format:%d  SizeX:%d  SizeY:%d \n" @*/,
					GD3D11.Device.Caps.TextureFormat[ Orig->ColorFormat ], OrigTex->TexWidth, OrigTex->TexHeight )) ;
				return -1 ;
			}
		}

		// �`��Ώۂɏo����摜�̏ꍇ�͐^�����ɓh��Ԃ�
		if( Orig->FormatDesc.DrawValidFlag )
		{
			for( j = 0 ; j < CUBEMAP_SURFACE_NUM ; j ++ )
			{
				if( OrigTex->PF->D3D11.TextureRTV[ j ] != NULL )
				{
					FLOAT ClearColor[ 4 ] = { 0.0f } ;

					D3D11DeviceContext_ClearRenderTargetView_ASync( OrigTex->PF->D3D11.TextureRTV[ j ], ClearColor, ASyncThread ) ;
				}
			}

			if( OrigTex->PF->D3D11.DepthBufferDSV != NULL )
			{
				D3D11DeviceContext_ClearDepthStencilView_ASync( OrigTex->PF->D3D11.DepthBufferDSV, D_D3D11_CLEAR_DEPTH, 1.0f, 0, ASyncThread ) ;
			}
		}
	}

	// �I��
	return 0 ;
}

// �e�N�X�`���V�F�[�_�[���\�[�X�r���[���������
static	int		Graphics_Hardware_D3D11_ReleaseTextureSRV( D_ID3D11ShaderResourceView *TextureSRV )
{
	int i ;
	int Flag ;
	D_ID3D11ShaderResourceView *pShaderResourceView = NULL ;

	// �s�N�Z���V�F�[�_�[�Ŏg�p����e�N�X�`���Ƃ��Đݒ肳��Ă������������
	Flag = FALSE ;
	for( i = 0 ; i < D_D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT ; i ++ )
	{
		if( GD3D11.Device.State.PSSetShaderResourceView[ i ] == TextureSRV )
		{
			if( Flag == FALSE )
			{
				// �`��ҋ@���Ă���`�敨��`��
				DRAWSTOCKINFO
				Flag = TRUE ;
			}

			Graphics_D3D11_DeviceState_SetPSShaderResouceView( i, 1, &pShaderResourceView ) ;
		}
	}

	// �ǂ����̃X���b�g�ɃZ�b�g����Ă�����`��p�e�N�X�`��������
	if( Flag )
	{
		Graphics_D3D11_DrawSetting_SetTexture( NULL, NULL ) ;
	}

	// �V�F�[�_�[���\�[�X�r���[���������
	Direct3D11_Release_ShaderResourceView( TextureSRV ) ;

	return 0 ;
}

// �I���W�i���摜��񒆂̃e�N�X�`�����������
extern	int		Graphics_Hardware_D3D11_ReleaseOrigTexture_PF( IMAGEDATA_ORIG *Orig )
{
	int i ;
	int j ;
	IMAGEDATA_ORIG_HARD_TEX *OrigTex ;

	// �n�[�h�E�G�A���g�p����摜�ł͂Ȃ��ꍇ�͉��������ɏI��
	if( Orig->FormatDesc.TextureFlag == FALSE )
	{
		return 0 ;
	}

	OrigTex = Orig->Hard.Tex ;
	for( i = 0 ; i < Orig->Hard.TexNum ; i ++, OrigTex ++ )
	{
		for( j = 0 ; j < CUBEMAP_SURFACE_NUM ; j ++ )
		{
			if( OrigTex->PF->D3D11.TextureRTV[ j ] )
			{
				Direct3D11_Release_RenderTargetView( OrigTex->PF->D3D11.TextureRTV[ j ] ) ;
				OrigTex->PF->D3D11.TextureRTV[ j ] = NULL ;
			}
		}
		if( OrigTex->PF->D3D11.TextureSRV )
		{
			Graphics_Hardware_D3D11_ReleaseTextureSRV( OrigTex->PF->D3D11.TextureSRV ) ;
			OrigTex->PF->D3D11.TextureSRV = NULL ;
		}
		if( OrigTex->PF->D3D11.MSTexture )
		{
			Direct3D11_Release_Texture2D( OrigTex->PF->D3D11.MSTexture ) ;
			OrigTex->PF->D3D11.MSTexture = NULL ;
		}
		if( OrigTex->PF->D3D11.Texture )
		{
			Direct3D11_Release_Texture2D( OrigTex->PF->D3D11.Texture ) ;
			OrigTex->PF->D3D11.Texture = NULL ;
		}

		if( OrigTex->PF->D3D11.DepthBufferDSV )
		{
			Direct3D11_Release_DepthStencilView( OrigTex->PF->D3D11.DepthBufferDSV ) ;
			OrigTex->PF->D3D11.DepthBufferDSV = NULL ;
		}
		if( OrigTex->PF->D3D11.DepthBuffer )
		{
			Direct3D11_Release_Texture2D( OrigTex->PF->D3D11.DepthBuffer ) ;
			OrigTex->PF->D3D11.DepthBuffer = NULL ;
		}
	}

	// �I��
	return 0 ;
}

// �w��̃}���`�T���v�����Ŏg�p�ł���ő�N�I���e�B�l���擾����
extern	int		Graphics_Hardware_D3D11_GetMultiSampleQuality_PF( int Samples )
{
	IMAGEFORMATDESC				Format ;
	SETUP_GRAPHHANDLE_GPARAM	GParam ;
	int							FormatIndex ;
	D_DXGI_FORMAT				D3DFormat ;
	UINT						Quality ;

	if( Samples > 16 )
	{
		Samples = 16 ;
	}

	if( GAPIWin.D3D11DeviceObject == NULL )
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
	D3DFormat = GD3D11.Device.Caps.TextureFormat[ FormatIndex ] ;
	if( D3DFormat == D_DXGI_FORMAT_UNKNOWN )
	{
		return -1 ;
	}

	// �ݒ�ł���N�I���e�B���擾
	Quality = 10000 ;
	if( D3D11Device_CheckMultiSampleParam_ASync( D3DFormat, ( UINT * )&Samples, &Quality, TRUE ) < 0 )
	{
		return -1 ;
	}

	// �N�I���e�B��Ԃ�
	return ( int )Quality ;
}

// �쐬����O���t�B�b�N�n���h���Ŏg�p������ˑ��̃e�N�X�`���t�H�[�}�b�g���w�肷��
extern int Graphics_Hardware_D3D11_SetUsePlatformTextureFormat_PF( int PlatformTextureFormat )
{
	return 0 ;
}




























// ���ˑ����_�o�b�t�@�E�C���f�b�N�X�o�b�t�@�֌W

// ���_�o�b�t�@�n���h���̒��_�o�b�t�@���쐬����
extern	int		Graphics_Hardware_D3D11_VertexBuffer_Create_PF( VERTEXBUFFERHANDLEDATA *VertexBuffer )
{
	return Graphics_D3D11_VertexBuffer_CreateObject( VertexBuffer, FALSE ) ;
}

// ���_�o�b�t�@�n���h���̌�n��
extern	int		Graphics_Hardware_D3D11_VertexBuffer_Terminate_PF( VERTEXBUFFERHANDLEDATA *VertexBuffer )
{
	return Graphics_D3D11_VertexBuffer_ReleaseObject( VertexBuffer ) ;
}

// ���_�o�b�t�@�ɒ��_�f�[�^��]������
extern	int		Graphics_Hardware_D3D11_VertexBuffer_SetData_PF( VERTEXBUFFERHANDLEDATA *VertexBuffer, int SetIndex, const void *VertexData, int VertexNum )
{
	D_D3D11_BOX DestBox ;

	DestBox.left   = ( UINT )(   SetIndex               * VertexBuffer->UnitSize ) ;
	DestBox.top    = 0 ;
	DestBox.front  = 0 ;
	DestBox.right  = ( UINT )( ( SetIndex + VertexNum ) * VertexBuffer->UnitSize ) ;
	DestBox.bottom = 1 ;
	DestBox.back   = 1 ;

	D3D11DeviceContext_UpdateSubresource_ASync( VertexBuffer->PF->D3D11.VertexBuffer, 0, &DestBox, VertexData, 0, 0 ) ;

	// ����I��
	return 0 ;
}

// �C���f�b�N�X�o�b�t�@�n���h���̃Z�b�g�A�b�v���s��
extern	int		Graphics_Hardware_D3D11_IndexBuffer_Create_PF( INDEXBUFFERHANDLEDATA *IndexBuffer )
{
	return Graphics_D3D11_IndexBuffer_CreateObject( IndexBuffer, FALSE ) ;
}

// �C���f�b�N�X�o�b�t�@�n���h���̌�n��
extern	int		Graphics_Hardware_D3D11_IndexBuffer_Terminate_PF( INDEXBUFFERHANDLEDATA *IndexBuffer )
{
	return Graphics_D3D11_IndexBuffer_ReleaseObject( IndexBuffer ) ;
}

// �C���f�b�N�X�o�b�t�@�ɃC���f�b�N�X�f�[�^��]������
extern	int		Graphics_Hardware_D3D11_IndexBuffer_SetData_PF( INDEXBUFFERHANDLEDATA *IndexBuffer, int SetIndex, const void *IndexData, int IndexNum )
{
	D_D3D11_BOX DestBox ;

	DestBox.left   = ( UINT )(   SetIndex              * IndexBuffer->UnitSize ) ;
	DestBox.top    = 0 ;
	DestBox.front  = 0 ;
	DestBox.right  = ( UINT )( ( SetIndex + IndexNum ) * IndexBuffer->UnitSize ) ;
	DestBox.bottom = 1 ;
	DestBox.back   = 1 ;

	D3D11DeviceContext_UpdateSubresource_ASync( IndexBuffer->PF->D3D11.IndexBuffer, 0, &DestBox, IndexData, 0, 0 ) ;

	// ����I��
	return 0 ;
}























// ���ˑ����C�g�֌W

// ���C�e�B���O���s�����ǂ�����ݒ肷��
extern	int		Graphics_Hardware_D3D11_Light_SetUse_PF( int Flag )
{
	return Graphics_D3D11_DeviceState_SetLighting( Flag ) ;
}

// �O���[�o���A���r�G���g���C�g�J���[��ݒ肷��
extern	int		Graphics_Hardware_D3D11_Light_GlobalAmbient_PF( COLOR_F *Color )
{
	return Graphics_D3D11_DeviceState_SetAmbient( Color ) ;
}

// ���C�g�p�����[�^���Z�b�g
extern	int		Graphics_Hardware_D3D11_Light_SetState_PF( int LightNumber, LIGHTPARAM *LightParam )
{
	return Graphics_D3D11_DeviceState_SetLightState( LightNumber, LightParam ) ;
}

// ���C�g�̗L���A������ύX
extern	int		Graphics_Hardware_D3D11_Light_SetEnable_PF( int LightNumber, int EnableState )
{
	return Graphics_D3D11_DeviceState_SetLightEnable( LightNumber, EnableState ) ;
}




























// ���ˑ��V���h�E�}�b�v�֌W

// �V���h�E�}�b�v�f�[�^�ɕK�v�ȃe�N�X�`�����쐬����
extern	int		Graphics_Hardware_D3D11_ShadowMap_CreateTexture_PF( SHADOWMAPDATA *ShadowMap, int ASyncThread )
{
	D_D3D11_TEXTURE2D_DESC				Texture2DDesc ;
	D_D3D11_RENDER_TARGET_VIEW_DESC		RTVDesc ;
	D_D3D11_SHADER_RESOURCE_VIEW_DESC	SRVDesc ;
	D_D3D11_DEPTH_STENCIL_VIEW_DESC		DepthStencilViewDesc ;
	HRESULT								hr ;

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
	if( GD3D11.Device.Caps.TextureFormat[ ShadowMap->ColorFormat ] == D_DXGI_FORMAT_UNKNOWN )
	{
		DXST_ERRORLOG_ADDUTF16LE( "\xb7\x30\xe3\x30\xc9\x30\xa6\x30\xde\x30\xc3\x30\xd7\x30\xc7\x30\xfc\x30\xbf\x30\x28\x75\x6e\x30\xd5\x30\xa9\x30\xfc\x30\xde\x30\xc3\x30\xc8\x30\x67\x30\x5c\x4f\x10\x62\x67\x30\x4d\x30\x8b\x30\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\xd5\x30\xa9\x30\xfc\x30\xde\x30\xc3\x30\xc8\x30\x4c\x30\x42\x30\x8a\x30\x7e\x30\x5b\x30\x93\x30\x67\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�V���h�E�}�b�v�f�[�^�p�̃t�H�[�}�b�g�ō쐬�ł���e�N�X�`���t�H�[�}�b�g������܂���ł���\n" @*/ ) ;
		return -1 ;
	}

	// �����_�����O�^�[�Q�b�g�[�x�o�b�t�@�̍쐬
	if( ShadowMap->PF->D3D11.DepthBuffer == NULL )
	{
		// �e�N�X�`���̍쐬
		_MEMSET( &Texture2DDesc, 0, sizeof( Texture2DDesc ) ) ;
		Texture2DDesc.Width              = ( UINT )ShadowMap->BaseSizeX ;
		Texture2DDesc.Height             = ( UINT )ShadowMap->BaseSizeY ;
		Texture2DDesc.MipLevels          = 1 ;
		Texture2DDesc.ArraySize          = 1 ;
		Texture2DDesc.Format             = GD3D11.Device.Caps.DepthBufferTexture2DFormat[ ShadowMap->ZBufferFormat ] ;
		Texture2DDesc.SampleDesc.Count   = 1 ;
		Texture2DDesc.SampleDesc.Quality = 0 ;
		Texture2DDesc.Usage              = D_D3D11_USAGE_DEFAULT ;
		Texture2DDesc.BindFlags          = D_D3D11_BIND_DEPTH_STENCIL ;
		Texture2DDesc.CPUAccessFlags     = 0 ;
		Texture2DDesc.MiscFlags          = 0 ;
		hr = D3D11Device_CreateTexture2D_ASync( &Texture2DDesc, NULL, &ShadowMap->PF->D3D11.DepthBuffer, ASyncThread ) ;
		if( FAILED( hr ) )
		{
			DXST_ERRORLOG_ADDUTF16LE( "\xb7\x30\xe3\x30\xc9\x30\xa6\x30\xde\x30\xc3\x30\xd7\x30\x28\x75\xf1\x6d\xa6\x5e\xd0\x30\xc3\x30\xd5\x30\xa1\x30\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�V���h�E�}�b�v�p�[�x�o�b�t�@�e�N�X�`���̍쐬�Ɏ��s���܂���\n" @*/ ) ;
			return -1 ;
		}

		// �[�x�X�e���V���r���[�̍쐬
		_MEMSET( &DepthStencilViewDesc, 0, sizeof( DepthStencilViewDesc ) ) ;
		DepthStencilViewDesc.Format             = GD3D11.Device.Caps.DepthBufferDepthStencilFormat[ ShadowMap->ZBufferFormat ] ;
		DepthStencilViewDesc.ViewDimension      = D_D3D11_DSV_DIMENSION_TEXTURE2D;
		DepthStencilViewDesc.Texture2D.MipSlice = 0;

		hr = D3D11Device_CreateDepthStencilView_ASync( ShadowMap->PF->D3D11.DepthBuffer, &DepthStencilViewDesc, &ShadowMap->PF->D3D11.DepthBufferDSV, ASyncThread ) ;
		if( FAILED( hr ) )
		{
			DXST_ERRORLOG_ADDUTF16LE( "\xb7\x30\xe3\x30\xc9\x30\xa6\x30\xde\x30\xc3\x30\xd7\x30\x28\x75\xf1\x6d\xa6\x5e\xb9\x30\xc6\x30\xf3\x30\xb7\x30\xeb\x30\xd3\x30\xe5\x30\xfc\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�V���h�E�}�b�v�p�[�x�X�e���V���r���[�̍쐬�Ɏ��s���܂���\n" @*/ ) ;
			return -1 ;
		}
	}

	// �����_�[�^�[�Q�b�g�ɂł���e�N�X�`���̍쐬
	if( ShadowMap->PF->D3D11.DepthTexture == NULL )
	{
		_MEMSET( &Texture2DDesc, 0, sizeof( Texture2DDesc ) ) ;
		Texture2DDesc.Usage              = D_D3D11_USAGE_DEFAULT ;
		Texture2DDesc.Format             = GD3D11.Device.Caps.TextureFormat[ ShadowMap->ColorFormat ] ;
		Texture2DDesc.BindFlags          = D_D3D11_BIND_SHADER_RESOURCE | D_D3D11_BIND_RENDER_TARGET ;
		Texture2DDesc.Width              = ( UINT )ShadowMap->BaseSizeX ;
		Texture2DDesc.Height             = ( UINT )ShadowMap->BaseSizeY ;
		Texture2DDesc.CPUAccessFlags     = 0 ;
		Texture2DDesc.MipLevels          = 1 ;
		Texture2DDesc.ArraySize          = 1 ;
		Texture2DDesc.SampleDesc.Count   = 1 ;
		Texture2DDesc.SampleDesc.Quality = 0 ;
		hr = D3D11Device_CreateTexture2D_ASync( &Texture2DDesc, NULL, &ShadowMap->PF->D3D11.DepthTexture, ASyncThread ) ;
		if( FAILED( hr ) )
		{
			DXST_ERRORLOG_ADDUTF16LE( "\xb7\x30\xe3\x30\xc9\x30\xa6\x30\xde\x30\xc3\x30\xd7\x30\x28\x75\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�V���h�E�}�b�v�p�e�N�X�`���̍쐬�Ɏ��s���܂���\n" @*/ ) ;
			return -1 ;
		}

		_MEMSET( &RTVDesc, 0, sizeof( RTVDesc ) );
		RTVDesc.Format        = GD3D11.Device.Caps.TextureFormat[ ShadowMap->ColorFormat ] ;
		RTVDesc.ViewDimension = D_D3D11_RTV_DIMENSION_TEXTURE2D ;
		hr = D3D11Device_CreateRenderTargetView_ASync( ShadowMap->PF->D3D11.DepthTexture, &RTVDesc, &ShadowMap->PF->D3D11.DepthTextureRTV, ASyncThread ) ;
		if( FAILED( hr ) )
		{
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\xb7\x30\xe3\x30\xc9\x30\xa6\x30\xde\x30\xc3\x30\xd7\x30\x28\x75\xec\x30\xf3\x30\xc0\x30\xfc\x30\xbf\x30\xfc\x30\xb2\x30\xc3\x30\xc8\x30\xd3\x30\xe5\x30\xfc\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x20\x00\x20\x00\x46\x00\x6f\x00\x72\x00\x6d\x00\x61\x00\x74\x00\x3a\x00\x25\x00\x64\x00\x20\x00\x20\x00\x53\x00\x69\x00\x7a\x00\x65\x00\x58\x00\x3a\x00\x25\x00\x64\x00\x20\x00\x20\x00\x53\x00\x69\x00\x7a\x00\x65\x00\x59\x00\x3a\x00\x25\x00\x64\x00\x20\x00\x0a\x00\x00"/*@ L"�V���h�E�}�b�v�p�����_�[�^�[�Q�b�g�r���[�̍쐬�Ɏ��s���܂���  Format:%d  SizeX:%d  SizeY:%d \n" @*/,
				GD3D11.Device.Caps.TextureFormat[ ShadowMap->ColorFormat ], ShadowMap->BaseSizeX, ShadowMap->BaseSizeY )) ;
			return -1 ;
		}

		_MEMSET( &SRVDesc, 0, sizeof( SRVDesc ) );
		SRVDesc.Format              = GD3D11.Device.Caps.TextureFormat[ ShadowMap->ColorFormat ] ;
		SRVDesc.ViewDimension       = D_D3D11_SRV_DIMENSION_TEXTURE2D ;
		SRVDesc.Texture2D.MipLevels = 1 ;
		hr = D3D11Device_CreateShaderResourceView_ASync( ShadowMap->PF->D3D11.DepthTexture, &SRVDesc, &ShadowMap->PF->D3D11.DepthTextureSRV, ASyncThread ) ;
		if( FAILED( hr ) )
		{
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\xb7\x30\xe3\x30\xc9\x30\xa6\x30\xde\x30\xc3\x30\xd7\x30\x28\x75\xb7\x30\xa7\x30\xfc\x30\xc0\x30\xfc\x30\xea\x30\xbd\x30\xfc\x30\xb9\x30\xd3\x30\xe5\x30\xfc\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x20\x00\x20\x00\x46\x00\x6f\x00\x72\x00\x6d\x00\x61\x00\x74\x00\x3a\x00\x25\x00\x64\x00\x20\x00\x20\x00\x53\x00\x69\x00\x7a\x00\x65\x00\x58\x00\x3a\x00\x25\x00\x64\x00\x20\x00\x20\x00\x53\x00\x69\x00\x7a\x00\x65\x00\x59\x00\x3a\x00\x25\x00\x64\x00\x20\x00\x0a\x00\x00"/*@ L"�V���h�E�}�b�v�p�V�F�[�_�[���\�[�X�r���[�̍쐬�Ɏ��s���܂���  Format:%d  SizeX:%d  SizeY:%d \n" @*/,
				GD3D11.Device.Caps.TextureFormat[ ShadowMap->ColorFormat ], ShadowMap->BaseSizeX, ShadowMap->BaseSizeY )) ;
			return -1 ;
		}
	}

	// �I��
	return 0 ;
}

// �V���h�E�}�b�v�f�[�^�ɕK�v�ȃe�N�X�`�����������
extern	int		Graphics_Hardware_D3D11_ShadowMap_ReleaseTexture_PF( SHADOWMAPDATA *ShadowMap )
{
	if( ShadowMap->PF->D3D11.DepthTexture != NULL )
	{
		Direct3D11_Release_Texture2D( ShadowMap->PF->D3D11.DepthTexture ) ;
		ShadowMap->PF->D3D11.DepthTexture = NULL ;
	}

	if( ShadowMap->PF->D3D11.DepthTextureSRV != NULL )
	{
		Direct3D11_Release_ShaderResourceView( ShadowMap->PF->D3D11.DepthTextureSRV ) ;
		ShadowMap->PF->D3D11.DepthTextureSRV = NULL ;
	}

	if( ShadowMap->PF->D3D11.DepthTextureRTV != NULL )
	{
		Direct3D11_Release_RenderTargetView( ShadowMap->PF->D3D11.DepthTextureRTV ) ;
		ShadowMap->PF->D3D11.DepthTextureRTV = NULL ;
	}

	if( ShadowMap->PF->D3D11.DepthBuffer != NULL )
	{
		Direct3D11_Release_Texture2D( ShadowMap->PF->D3D11.DepthBuffer ) ;
		ShadowMap->PF->D3D11.DepthBuffer = NULL ;
	}

	if( ShadowMap->PF->D3D11.DepthBufferDSV != NULL )
	{
		Direct3D11_Release_DepthStencilView( ShadowMap->PF->D3D11.DepthBufferDSV ) ;
		ShadowMap->PF->D3D11.DepthBufferDSV = NULL ;
	}

	// �I��
	return 0 ;
}

// ���_�V�F�[�_�[�ɐݒ肷��V���h�E�}�b�v�̏����X�V����
extern	void	Graphics_Hardware_D3D11_ShadowMap_RefreshVSParam_PF( void )
{
	DX_D3D11_VS_CONST_BUFFER_OTHERMATRIX *ConstOtherMatrix ;
	SHADOWMAPDATA *ShadowMap ;
	static float TranspMatDef[ 4 ][ 4 ] =
	{
		{ 0.0f, 0.0f, 0.0f, -1.0f },
		{ 0.0f, 0.0f, 0.0f, -1.0f },
		{ 0.0f, 0.0f, 0.0f, -1.0f },
		{ 0.0f, 0.0f, 0.0f, -1.0f },
	} ;
	int i ;

	if( GAPIWin.D3D11DeviceObject == NULL )
	{
		return ;
	}

	ConstOtherMatrix = ( DX_D3D11_VS_CONST_BUFFER_OTHERMATRIX * )GD3D11.Device.Shader.Constant.ConstBuffer_VS_OtherMatrix->SysmemBuffer ;

	for( i = 0 ; i < MAX_USE_SHADOWMAP_NUM ; i ++ )
	{
		// �A�h���X�̎擾
		if( SHADOWMAPCHKFULL( GSYS.DrawSetting.ShadowMap[ i ], ShadowMap ) )
		{
			_MEMCPY( ConstOtherMatrix->ShadowMapLightViewProjectionMatrix[ i ], TranspMatDef, sizeof( TranspMatDef ) ) ;
		}
		else
		{
			// �s��̃Z�b�g
			ConstOtherMatrix->ShadowMapLightViewProjectionMatrix[ i ][ 0 ][ 0 ] = ShadowMap->ShadowMapViewProjectionMatrix.m[ 0 ][ 0 ] ;
			ConstOtherMatrix->ShadowMapLightViewProjectionMatrix[ i ][ 0 ][ 1 ] = ShadowMap->ShadowMapViewProjectionMatrix.m[ 1 ][ 0 ] ;
			ConstOtherMatrix->ShadowMapLightViewProjectionMatrix[ i ][ 0 ][ 2 ] = ShadowMap->ShadowMapViewProjectionMatrix.m[ 2 ][ 0 ] ;
			ConstOtherMatrix->ShadowMapLightViewProjectionMatrix[ i ][ 0 ][ 3 ] = ShadowMap->ShadowMapViewProjectionMatrix.m[ 3 ][ 0 ] ;

			ConstOtherMatrix->ShadowMapLightViewProjectionMatrix[ i ][ 1 ][ 0 ] = ShadowMap->ShadowMapViewProjectionMatrix.m[ 0 ][ 1 ] ;
			ConstOtherMatrix->ShadowMapLightViewProjectionMatrix[ i ][ 1 ][ 1 ] = ShadowMap->ShadowMapViewProjectionMatrix.m[ 1 ][ 1 ] ;
			ConstOtherMatrix->ShadowMapLightViewProjectionMatrix[ i ][ 1 ][ 2 ] = ShadowMap->ShadowMapViewProjectionMatrix.m[ 2 ][ 1 ] ;
			ConstOtherMatrix->ShadowMapLightViewProjectionMatrix[ i ][ 1 ][ 3 ] = ShadowMap->ShadowMapViewProjectionMatrix.m[ 3 ][ 1 ] ;

			ConstOtherMatrix->ShadowMapLightViewProjectionMatrix[ i ][ 2 ][ 0 ] = ShadowMap->ShadowMapViewProjectionMatrix.m[ 0 ][ 2 ] ;
			ConstOtherMatrix->ShadowMapLightViewProjectionMatrix[ i ][ 2 ][ 1 ] = ShadowMap->ShadowMapViewProjectionMatrix.m[ 1 ][ 2 ] ;
			ConstOtherMatrix->ShadowMapLightViewProjectionMatrix[ i ][ 2 ][ 2 ] = ShadowMap->ShadowMapViewProjectionMatrix.m[ 2 ][ 2 ] ;
			ConstOtherMatrix->ShadowMapLightViewProjectionMatrix[ i ][ 2 ][ 3 ] = ShadowMap->ShadowMapViewProjectionMatrix.m[ 3 ][ 2 ] ;

			ConstOtherMatrix->ShadowMapLightViewProjectionMatrix[ i ][ 3 ][ 0 ] = ShadowMap->ShadowMapViewProjectionMatrix.m[ 0 ][ 3 ] ;
			ConstOtherMatrix->ShadowMapLightViewProjectionMatrix[ i ][ 3 ][ 1 ] = ShadowMap->ShadowMapViewProjectionMatrix.m[ 1 ][ 3 ] ;
			ConstOtherMatrix->ShadowMapLightViewProjectionMatrix[ i ][ 3 ][ 2 ] = ShadowMap->ShadowMapViewProjectionMatrix.m[ 2 ][ 3 ] ;
			ConstOtherMatrix->ShadowMapLightViewProjectionMatrix[ i ][ 3 ][ 3 ] = ShadowMap->ShadowMapViewProjectionMatrix.m[ 3 ][ 3 ] ;
		}
	}

	GD3D11.Device.Shader.Constant.ConstBuffer_VS_OtherMatrix->ChangeFlag = TRUE ;

	// �萔�o�b�t�@���X�V
//	Graphics_D3D11_ConstantBuffer_Update( GD3D11.Device.Shader.Constant.ConstBuffer_VS_OtherMatrix ) ;
}

// �s�N�Z���V�F�[�_�[�ɐݒ肷��V���h�E�}�b�v�̏����X�V����
extern	void	Graphics_Hardware_D3D11_ShadowMap_RefreshPSParam_PF( void )
{
	DX_D3D11_PS_CONST_BUFFER_SHADOWMAP *ConstantPSShadowMap ;
	SHADOWMAPDATA *ShadowMap ;
	LIGHT_HANDLE  *pLH ;
	int           CheckNum ;
	int           i ;
	int           j ;

	if( GAPIWin.D3D11DeviceObject == NULL )
	{
		return ;
	}

	ConstantPSShadowMap = ( DX_D3D11_PS_CONST_BUFFER_SHADOWMAP * )GD3D11.Device.Shader.Constant.ConstBuffer_PS_ShadowMap->SysmemBuffer ;

	CheckNum = GSYS.Light.EnableNum > 3 ? 3 : GSYS.Light.EnableNum ;

	for( i = 0 ; i < MAX_USE_SHADOWMAP_NUM ; i ++ )
	{
		ConstantPSShadowMap->Data[ i ].Enable_Light0 = 1.0f ;
		ConstantPSShadowMap->Data[ i ].Enable_Light1 = 1.0f ;
		ConstantPSShadowMap->Data[ i ].Enable_Light2 = 1.0f ;

		// �A�h���X�̎擾
		if( SHADOWMAPCHKFULL( GSYS.DrawSetting.ShadowMap[ i ], ShadowMap ) )
		{
			ConstantPSShadowMap->Data[ i ].AdjustDepth    = 1.0f ;
			ConstantPSShadowMap->Data[ i ].GradationParam = 0.0f ;
		}
		else
		{
			ConstantPSShadowMap->Data[ i ].AdjustDepth    = ShadowMap->AdjustDepth ;
			ConstantPSShadowMap->Data[ i ].GradationParam = ShadowMap->GradationParam ;

			for( j = 0 ; j < CheckNum ; j ++ )
			{
				pLH = GSYS.Light.Data[ GSYS.Light.EnableHardwareIndex[ j ] ] ;
				if( pLH->ShadowMapSlotDisableFlag[ i ] == FALSE )
				{
					switch( j )
					{
					case 0 :
						ConstantPSShadowMap->Data[ i ].Enable_Light0 = 0.0f ;
						break ;

					case 1 :
						ConstantPSShadowMap->Data[ i ].Enable_Light1 = 0.0f ;
						break ;

					case 2 :
						ConstantPSShadowMap->Data[ i ].Enable_Light2 = 0.0f ;
						break ;
					}
				}
			}
		}
	}
	GD3D11.Device.Shader.Constant.ConstBuffer_PS_ShadowMap->ChangeFlag = TRUE ;

	// �萔�o�b�t�@���X�V
//	Graphics_D3D11_ConstantBuffer_Update( GD3D11.Device.Shader.Constant.ConstBuffer_PS_ShadowMap ) ;
}

// �V���h�E�}�b�v�ւ̕`��̏������s��
extern	int		Graphics_Hardware_D3D11_ShadowMap_DrawSetup_PF( SHADOWMAPDATA *ShadowMap )
{
	// ����I��
	return 0 ;
}

// �V���h�E�}�b�v�ւ̕`����I������
extern	int		Graphics_Hardware_D3D11_ShadowMap_DrawEnd_PF( SHADOWMAPDATA *ShadowMap )
{
	// ����I��
	return 0 ;
}

// �`��Ŏg�p����V���h�E�}�b�v���w�肷��( �X���b�g�͂O���P�����w��\ )
extern	int		Graphics_Hardware_D3D11_ShadowMap_SetUse_PF( int SlotIndex, SHADOWMAPDATA *ShadowMap )
{
	// ����I��
	return 0 ;
}































// ���ˑ��V�F�[�_�[�֌W

// �V�F�[�_�[�n���h��������������
extern	int		Graphics_Hardware_D3D11_Shader_Create_PF( int ShaderHandle, int IsVertexShader, void *Image, int ImageSize, int ImageAfterFree, int ASyncThread )
{
	SHADERHANDLEDATA     *pShader ;
	D_ID3D11PixelShader  *PixelShader  = NULL ;
	D_ID3D11VertexShader *VertexShader = NULL ;

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
		if( D3D11Device_CreateVertexShader_ASync( Image, ( SIZE_T )ImageSize, NULL, &VertexShader, ASyncThread ) != S_OK )
		{
			DXST_ERRORLOG_ADDUTF16LE( "\x02\x98\xb9\x70\xb7\x30\xa7\x30\xfc\x30\xc0\x30\xfc\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"���_�V�F�[�_�[�̍쐬�Ɏ��s���܂���\n" @*/ ) ;
			goto ERR ;
		}
	}
	else
	{
		if( D3D11Device_CreatePixelShader_ASync( Image, ( SIZE_T )ImageSize, NULL, &PixelShader, ASyncThread ) != S_OK )
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

	// �o�C�i���C���[�W�̃T�C�Y��ۑ�����
	pShader->FunctionCodeSize = ImageSize ;

	// ���_�V�F�[�_�[���ǂ�����ۑ�����
	pShader->IsVertexShader = IsVertexShader ? TRUE : FALSE ;

	// �V�F�[�_�[�̃A�h���X��ۑ�
	if( IsVertexShader )
	{
		pShader->PF->D3D11.VertexShader = VertexShader ;
	}
	else
	{
		pShader->PF->D3D11.PixelShader = PixelShader ;
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
		Direct3D11_Release_VertexShader_ASync( VertexShader, ASyncThread ) ;
	}

	if( PixelShader )
	{
		Direct3D11_Release_PixelShader_ASync( PixelShader, ASyncThread ) ;
	}

	// ������ɉ�����ׂ��̃t���O�������Ă�����������
	if( ImageAfterFree )
	{
		DXFREE( Image ) ;
	}

	return -1 ;
}

// �V�F�[�_�[�n���h���̌�n��
extern	int		Graphics_Hardware_D3D11_Shader_TerminateHandle_PF( SHADERHANDLEDATA *Shader )
{
	// �V�F�[�_�[�̉��
	if( Shader->IsVertexShader )
	{
		if( Shader->PF->D3D11.VertexShader )
		{
			Direct3D11_Release_VertexShader( Shader->PF->D3D11.VertexShader ) ;
			Shader->PF->D3D11.VertexShader = NULL ;
		}
	}
	else
	{
		if( Shader->PF->D3D11.PixelShader )
		{
			Direct3D11_Release_PixelShader( Shader->PF->D3D11.PixelShader ) ;
			Shader->PF->D3D11.PixelShader = NULL ;
		}
	}

	// ����I��
	return 0 ;
}

// �g�p�ł���V�F�[�_�[�̃o�[�W�������擾����( 0=�g���Ȃ�  200=�V�F�[�_�[���f���Q�D�O���g�p�\  300=�V�F�[�_�[���f���R�D�O���g�p�\ )
extern	int		Graphics_Hardware_D3D11_Shader_GetValidShaderVersion_PF( void )
{
	switch( GD3D11.Setting.FeatureLevel )
	{
	case D_D3D_FEATURE_LEVEL_9_1 :
	case D_D3D_FEATURE_LEVEL_9_2 :
	case D_D3D_FEATURE_LEVEL_9_3 :
		return 200 ;

	case D_D3D_FEATURE_LEVEL_10_0 :
	case D_D3D_FEATURE_LEVEL_10_1 :
		return 400 ;

	case D_D3D_FEATURE_LEVEL_11_0 :
	case D_D3D_FEATURE_LEVEL_11_1 :
		return 500 ;
	}

	return 0 ;
}

// �w��̖��O�����萔���g�p����V�F�[�_�[�萔�̔ԍ����擾����
extern	int		Graphics_Hardware_D3D11_Shader_GetConstIndex_PF( const wchar_t *ConstantName, SHADERHANDLEDATA *Shader )
{
	return 0 ;
}

// �w��̖��O�����萔���g�p����V�F�[�_�[�萔�̐����擾����
extern	int		Graphics_Hardware_D3D11_Shader_GetConstCount_PF( const wchar_t *ConstantName, SHADERHANDLEDATA *Shader )
{
	return 0 ;
}

// �w��̖��O�������������_�萔�̃f�t�H���g�p�����[�^���i�[����Ă��郁�����A�h���X���擾����
extern	const FLOAT4 *Graphics_Hardware_D3D11_Shader_GetConstDefaultParamF_PF( const wchar_t *ConstantName, SHADERHANDLEDATA *Shader )
{
	return 0 ;
}

// �V�F�[�_�[�萔����ݒ肷��
extern	int		Graphics_Hardware_D3D11_Shader_SetConst_PF( int TypeIndex, int SetIndex, int ConstantIndex, const void *Param, int ParamNum, int UpdateUseArea )
{
	// ����I��
	return 0 ;
}

// �w��̈�̃V�F�[�_�[�萔�������Z�b�g����
extern	int		Graphics_Hardware_D3D11_Shader_ResetConst_PF( int TypeIndex, int SetIndex, int ConstantIndex, int ParamNum )
{
	// ����I��
	return 0 ;
}

#ifndef DX_NON_MODEL
// �R�c���f���p�̃V�F�[�_�[�R�[�h�̏��������s��
extern	int		Graphics_Hardware_D3D11_Shader_ModelCode_Init_PF( void )
{
	if( Graphics_D3D11_ShaderCode_Model_Initialize() == FALSE )
	{
		return -1 ;
	}

	// ����I��
	return 0 ;
}
#endif // DX_NON_MODEL
































// ���ˑ��V�F�[�_�[�p�萔�o�b�t�@�֌W

// �V�F�[�_�[�p�萔�o�b�t�@�n���h��������������
extern	int		Graphics_Hardware_D3D11_ShaderConstantBuffer_Create_PF( int ShaderConstantBufferHandle, int BufferSize, int ASyncThread ) 
{
	SHADERCONSTANTBUFFERHANDLEDATA *ShaderConstantBuffer ;

	// �G���[����
	if( ASyncThread )
	{
		if( SHADERCONSTANTBUFFERCHK_ASYNC( ShaderConstantBufferHandle, ShaderConstantBuffer ) )
			return -1 ;
	}
	else
	{
		if( SHADERCONSTANTBUFFERCHK( ShaderConstantBufferHandle, ShaderConstantBuffer ) )
			return -1 ;
	}

	// �V�F�[�_�[�p�萔�o�b�t�@���쐬����
	ShaderConstantBuffer->PF->D3D11.ConstBuffer = Graphics_D3D11_ConstantBuffer_Create( ( DWORD )BufferSize, ASyncThread ) ;
	if( ShaderConstantBuffer->PF->D3D11.ConstBuffer == NULL )
	{
		return -1 ;
	}

	// ����I��
	return 0 ;
}

// �V�F�[�_�[�p�萔�o�b�t�@�n���h���̌�n��
extern	int		Graphics_Hardware_D3D11_ShaderConstantBuffer_TerminateHandle_PF( SHADERCONSTANTBUFFERHANDLEDATA *ShaderConstantBuffer )
{
	// �V�F�[�_�[�p�萔�o�b�t�@���폜����
	if( ShaderConstantBuffer->PF->D3D11.ConstBuffer != NULL )
	{
		Graphics_D3D11_ConstantBuffer_Delete( ShaderConstantBuffer->PF->D3D11.ConstBuffer ) ;
		ShaderConstantBuffer->PF->D3D11.ConstBuffer = NULL ;
	}

	// ����I��
	return 0 ;
}

// �V�F�[�_�[�p�萔�o�b�t�@�n���h���̒萔�o�b�t�@�̃A�h���X���擾����
extern	void *	Graphics_Hardware_D3D11_ShaderConstantBuffer_GetBuffer_PF( SHADERCONSTANTBUFFERHANDLEDATA *ShaderConstantBuffer )
{
	return ShaderConstantBuffer->PF->D3D11.ConstBuffer->SysmemBuffer ;
}

// �V�F�[�_�[�p�萔�o�b�t�@�n���h���̒萔�o�b�t�@�ւ̕ύX��K�p����
extern	int		Graphics_Hardware_D3D11_ShaderConstantBuffer_Update_PF( SHADERCONSTANTBUFFERHANDLEDATA *ShaderConstantBuffer )
{
	ShaderConstantBuffer->PF->D3D11.ConstBuffer->ChangeFlag = TRUE ;
	Graphics_D3D11_ConstantBuffer_Update( ShaderConstantBuffer->PF->D3D11.ConstBuffer ) ;

	// �I��
	return 0 ;
}

// �V�F�[�_�[�p�萔�o�b�t�@�n���h���̒萔�o�b�t�@���w��̃V�F�[�_�[�̎w��̃X���b�g�ɃZ�b�g����
extern	int		Graphics_Hardware_D3D11_ShaderConstantBuffer_Set_PF( SHADERCONSTANTBUFFERHANDLEDATA *ShaderConstantBuffer, int TargetShader /* DX_SHADERTYPE_VERTEX �Ȃ� */, int Slot )
{
	switch( TargetShader )
	{
	case DX_SHADERTYPE_VERTEX :			// ���_�V�F�[�_�[
		Graphics_D3D11_ConstantBuffer_VSSet( ( UINT )Slot, 1, &ShaderConstantBuffer->PF->D3D11.ConstBuffer ) ;
		break ;

	case DX_SHADERTYPE_PIXEL :			// �s�N�Z���V�F�[�_�[
		Graphics_D3D11_ConstantBuffer_PSSet( ( UINT )Slot, 1, &ShaderConstantBuffer->PF->D3D11.ConstBuffer ) ;
		break ;

	case DX_SHADERTYPE_GEOMETRY :		// �W�I���g���V�F�[�_�[
		break ;

	case DX_SHADERTYPE_COMPUTE :		// �R���s���[�g�V�F�[�_�[
		break ;

	case DX_SHADERTYPE_DOMAIN :			// �h���C���V�F�[�_�[
		break ;

	case DX_SHADERTYPE_HULL :			// �n���V�F�[�_�[
		break ;
	}

	// �I��
	return 0 ;
}





































// ���ˑ��`��֌W

// �w��_���狫�E�F������Ƃ���܂œh��Ԃ�
extern	int		Graphics_Hardware_D3D11_Paint_PF( int x, int y, unsigned int FillColor, ULONGLONG BoundaryColor )
{
	// �f�t�H���g�̏������s��
	return 2 ;
}


























#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

#endif // DX_NON_DIRECT3D11

#endif // DX_NON_GRAPHICS


