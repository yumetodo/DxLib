// -------------------------------------------------------------------------------
// 
// 		�c�w���C�u����		�`�揈���v���O����( Direct3D11 )�w�b�_�t�@�C��
// 
// 				Ver 3.16d
// 
// -------------------------------------------------------------------------------

#ifndef __DXGRAPHICSD3D11_H__
#define __DXGRAPHICSD3D11_H__

#include "../DxCompileConfig.h"

#ifndef DX_NON_GRAPHICS

#ifndef DX_NON_DIRECT3D11

// �C���N���[�h ------------------------------------------------------------------
#include "../DxLib.h"
#include "../DxGraphics.h"
#include "../DxArchive_.h"
#include "DxGraphicsAPIWin.h"
#include "DxShader_DataType_D3D11.h"
#include "DxShader_PS_D3D11.h"
#include "DxShader_VS_D3D11.h"

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// �}�N����` --------------------------------------------------------------------

#define GD3D11										GraphicsHardDataDirect3D11

// �񋓂���A�_�v�^�[�̍ő吔
#define DX_D3D11_MAX_ADAPTER_LISTUP					(16)

// �񋓂���A�E�g�v�b�g�̍ő吔
#define DX_D3D11_MAX_OUTPUT_LISTUP					(32)

// �o�͐�E�C���h�E�̍ő吔
#define DX_D3D11_MAX_OUTPUTWINDOW					(256)

// ��Ǘ��e�N�X�`���ւ̃f�[�^�]���p�V�X�e���������z�u�e�N�X�`���̐�
#define DIRECT3D11_SYSMEMTEXTURE_NUM				(2048)

// ��Ǘ��e�N�X�`���ւ̃f�[�^�]���p�V�X�e���������z�u�T�[�t�F�X�̐�
#define DIRECT3D11_SYSMEMSURFACE_NUM				(2048)

// �����@�\���x���̐�
#define DIRECT3D11_FEATURE_LEVEL_NUM				(7)

// �o�b�N�o�b�t�@�̃t�H�[�}�b�g
#define DIRECT3D11_BACKBUFFER_FORMAT				D_DXGI_FORMAT_R8G8B8A8_UNORM

// 16�r�b�g�[�x�o�b�t�@��Texture2D�t�H�[�}�b�g
#define DIRECT3D11_DEPTHBUFFER_TEXTURE2D_FORMAT		D_DXGI_FORMAT_D16_UNORM

// 16�r�b�g�[�x�o�b�t�@��DepthStencil�t�H�[�}�b�g
#define DIRECT3D11_DEPTHBUFFER_DEPTHSTENCIL_FORMAT	D_DXGI_FORMAT_D16_UNORM

// �f�t�H���g�T���v���[�X�e�[�g�̃t�B���^�[���[�h�̐�
#define DIRECT3D11_DEFAULTRS_FILTER_NUM				(18)

// Graphics_D3D11_DrawPreparation �֐��ɓn���t���O
#define DX_D3D11_DRAWPREP_TRANS				(0x00001)
#define DX_D3D11_DRAWPREP_VECTORINT			(0x00002)
#define DX_D3D11_DRAWPREP_GOURAUDSHADE		(0x00008)
#define DX_D3D11_DRAWPREP_PERSPECTIVE		(0x00010)
#define DX_D3D11_DRAWPREP_DIFFUSERGB		(0x00020)
#define DX_D3D11_DRAWPREP_DIFFUSEALPHA		(0x00040)
#define DX_D3D11_DRAWPREP_FOG				(0x00080)
#define DX_D3D11_DRAWPREP_NOBLENDSETTING	(0x00100)
#define DX_D3D11_DRAWPREP_LIGHTING			(0x00200)
#define DX_D3D11_DRAWPREP_SPECULAR			(0x00400)
#define DX_D3D11_DRAWPREP_3D				(0x00800)
#define DX_D3D11_DRAWPREP_TEXADDRESS		(0x01000)
#define DX_D3D11_DRAWPREP_NOTSHADERRESET	(0x02000)
#define DX_D3D11_DRAWPREP_CULLING			(0x04000)
#define DX_D3D11_DRAWPREP_TEXTURE			(0x08000)
#define DX_D3D11_DRAWPREP_TEXALPHACH		(0x10000)
#define DX_D3D11_DRAWPREP_TEXALPHATEST		(0x20000)

// �f�t�H���g���X�^���C�U�[�X�e�[�g�̃t�B�����[�h
#define DIRECT3D11_DEFAULTRS_FILLMODE_WIRE					(0)
#define DIRECT3D11_DEFAULTRS_FILLMODE_SOLID					(1)
#define DIRECT3D11_DEFAULTRS_FILLMODE_NUM					(2)

// �f�t�H���g�X�e�[�g�p�̔�r���[�h��`
#define DIRECT3D11_DEFAULT_CMP_NEVER						(0)				// FALSE
#define DIRECT3D11_DEFAULT_CMP_LESS							(1)				// Src <  Dest		DrawAlpha <  TestParam
#define DIRECT3D11_DEFAULT_CMP_EQUAL						(2)				// Src == Dest		DrawAlpha == TestParam
#define DIRECT3D11_DEFAULT_CMP_LESSEQUAL					(3)				// Src <= Dest		DrawAlpha <= TestParam
#define DIRECT3D11_DEFAULT_CMP_GREATER						(4)				// Src >  Dest		DrawAlpha >  TestParam
#define DIRECT3D11_DEFAULT_CMP_NOTEQUAL						(5)				// Src != Dest		DrawAlpha != TestParam
#define DIRECT3D11_DEFAULT_CMP_GREATEREQUAL					(6)				// Src >= Dest		DrawAlpha >= TestParam
#define DIRECT3D11_DEFAULT_CMP_ALWAYS						(7)				// TRUE
#define DIRECT3D11_DEFAULT_CMP_NUM							(8)

// �s�N�Z���V�F�[�_�[�̃A���t�@�e�X�g���[�h
#define DIRECT3D11_PS_ALPHATEST_CMP_GREATER					(0)				// �`��A���t�@�l���e�X�g�A���t�@�l���傫����Ε`��
#define DIRECT3D11_PS_ALPHATEST_CMP_OTHER					(1)				// GREATER �ȊO�̔�r���[�h
#define DIRECT3D11_PS_ALPHATEST_NUM							(2)

// �W���`�撸�_�t�H�[�}�b�g
#define D3D11_VERTEX_INPUTLAYOUT_NOTEX_2D		(0)				// ( VERTEX_NOTEX_2D                      �\���̌`�� )���C���A�{�b�N�X�`��p���_�t�H�[�}�b�g
#define D3D11_VERTEX_INPUTLAYOUT_2D				(1)				// ( VERTEX2D or VERTEX_2D                �\���̌`�� )�g�����X�t�H�[���ςݒ��_�t�H�[�}�b�g
#define D3D11_VERTEX_INPUTLAYOUT_BLENDTEX_2D	(2)				// ( VERTEX_BLENDTEX_2D or VERTEX2DSHADER �\���̌`�� )���u�����h�e�N�X�`���t���g�����X�t�H�[���ςݒ��_�t�H�[�}�b�g
#define D3D11_VERTEX_INPUTLAYOUT_NOTEX_3D		(3)				// ( VERTEX_NOTEX_3D                      �\���̌`�� )���C���A�{�b�N�X�`��p���_�t�H�[�}�b�g�A�R�c�p( ���o�[�W�����p )
#define D3D11_VERTEX_INPUTLAYOUT_3D				(4)				// ( VERTEX_3D                            �\���̌`�� )�O���t�B�b�N�X�`��p���_�t�H�[�}�b�g�A�R�c�p( ���o�[�W�����p )
#define D3D11_VERTEX_INPUTLAYOUT_3D_LIGHT		(5)				// ( VERTEX3D                             �\���̌`�� )�O���t�B�b�N�X�`��p���_�t�H�[�}�b�g
#define D3D11_VERTEX_INPUTLAYOUT_SHADER_3D		(6)				// ( VERTEX3DSHADER                       �\���̌`�� )�V�F�[�_�[�`��p���_�t�H�[�}�b�g
#define D3D11_VERTEX_INPUTLAYOUT_NUM			(7)

// ���_�o�b�t�@�̃T�C�Y
#define D3D11_VERTEXBUFFER_SIZE					(64 * 1024)

// �e�萔�o�b�t�@�̃Z�b�g���W�X�^�ԍ�
#define DX_D3D11_VS_CONSTANTBUFFER_COMMON		(0)
#define DX_D3D11_VS_CONSTANTBUFFER_BASE			(1)
#define DX_D3D11_VS_CONSTANTBUFFER_OTHERMATRIX	(2)
#define DX_D3D11_VS_CONSTANTBUFFER_MATRIX		(3)

#define DX_D3D11_PS_CONSTANTBUFFER_COMMON		(0)
#define DX_D3D11_PS_CONSTANTBUFFER_BASE			(1)
#define DX_D3D11_PS_CONSTANTBUFFER_SHADOWMAP	(2)
#define DX_D3D11_PS_CONSTANTBUFFER_FILTER		(3)

// �\���̒�` --------------------------------------------------------------------

// Direct3D11�p �n�[�h�E�G�A�����_�����O�p�I���W�i���摜�e�N�X�`�����
struct IMAGEDATA_ORIG_HARD_TEX_DIRECT3D11
{
	D_ID3D11Texture2D *				MSTexture ;								// �}���`�T���v���e�N�X�`��
	D_ID3D11Texture2D *				Texture ;								// �e�N�X�`��
	D_ID3D11ShaderResourceView *	TextureSRV ;							// �e�N�X�`���p�V�F�[�_�[���\�[�X�r���[
	D_ID3D11RenderTargetView *		TextureRTV[ CUBEMAP_SURFACE_NUM ] ;		// �e�N�X�`���p�����_�����O�^�[�Q�b�g�r���[

//	D_ID3D11Texture2D *				CubeMapTexture ;						// �L���[�u�}�b�v�e�N�X�`��
//	D_ID3D11ShaderResourceView *	CubeMapTextureSRV ;						// �L���[�u�}�b�v�e�N�X�`���p�V�F�[�_�[���\�[�X�r���[
//	D_ID3D11RenderTargetView *		CubeMapTextureRTV[ CUBEMAP_SURFACE_NUM ] ;	// �L���[�u�}�b�v�p�����_�����O�^�[�Q�b�g�r���[

//	D_ID3D11Texture2D *				RenderTarget ;							// �����_�����O�^�[�Q�b�g�e�N�X�`��
//	D_ID3D11RenderTargetView *		RenderTargetRTV ;						// �����_�����O�^�[�Q�b�g�e�N�X�`���p�����_�����O�^�[�Q�b�g�r���[

	D_ID3D11Texture2D *				DepthBuffer ;							// �[�x�o�b�t�@
	D_ID3D11DepthStencilView *		DepthBufferDSV ;						// �[�x�o�b�t�@�p�f�v�X�X�e���V���r���[

//	D_IDirect3DTexture9	*			MemTexture ;							// D3DPOOL_MANAGED ���g�p�ł��Ȃ��摜�p�������e�N�X�`��
//	D_IDirect3DSurface9	*			Surface[ CUBEMAP_SURFACE_NUM ] ;		// �T�[�t�F�X
//	D_IDirect3DSurface9	*			RenderTargetSurface ;					// �����_�����O�^�[�Q�b�g�T�[�t�F�X
//	D_IDirect3DSurface9	*			DepthBuffer ;							// �[�x�o�b�t�@
} ;

// Direct3D11�p �V���h�E�}�b�v���
struct SHADOWMAPDATA_DIRECT3D11
{
	D_ID3D11Texture2D *				DepthTexture ;							// �[�x�l�������ݗp�e�N�X�`��
	D_ID3D11ShaderResourceView *	DepthTextureSRV ;						// �[�x�l�������ݗp�e�N�X�`���p�V�F�[�_�[���\�[�X�r���[
	D_ID3D11RenderTargetView *		DepthTextureRTV ;						// �[�x�l�������ݗp�e�N�X�`���p�����_�����O�^�[�Q�b�g�r���[

	D_ID3D11Texture2D *				DepthBuffer ;							// �[�x�o�b�t�@
	D_ID3D11DepthStencilView *		DepthBufferDSV ;						// �[�x�o�b�t�@�p�f�v�X�X�e���V���r���[
} ;

// Direct3D11�p ���_�o�b�t�@�n���h�����
struct VERTEXBUFFERHANDLEDATA_DIRECT3D11
{
	D_ID3D11Buffer					*VertexBuffer ;							// ID3D11Buffer �̃|�C���^
} ;

// Direct3D11�p �C���f�b�N�X�o�b�t�@�n���h�����
struct INDEXBUFFERHANDLEDATA_DIRECT3D11
{
	D_ID3D11Buffer					*IndexBuffer ;							// ID3D11Buffer �̃|�C���^
} ;

// Direct3D11�p �V�F�[�_�[�n���h�����
struct SHADERHANDLEDATA_DIRECT3D11
{
	union
	{
		D_ID3D11PixelShader			*PixelShader ;							// ID3D11PixelShader �̃|�C���^
		D_ID3D11VertexShader		*VertexShader ;							// ID3D11VertexShader �̃|�C���^
	} ;
} ;

// �萔�o�b�t�@���\����
struct CONSTANTBUFFER_DIRECT3D11
{
	DWORD							Size ;									// �T�C�Y( �o�C�g�P�� )
	D_ID3D11Buffer *				ConstantBuffer ;						// ID3D11Buffer �̃|�C���^
	void *							SysmemBuffer ;							// �萔�o�b�t�@�Ɠ��T�C�Y�̃V�X�e����������̃o�b�t�@
	int								ChangeFlag ;							// �V�X�e����������̃o�b�t�@���ύX����AID3DBuffer �ւ̔��f���K�v���ǂ���( TRUE:�K�v  FALSE:�s�v )

	CONSTANTBUFFER_DIRECT3D11 *		NextData ;								// ���X�g�\���̎��̃f�[�^�ւ̃|�C���^
	CONSTANTBUFFER_DIRECT3D11 *		PrevData ;								// ���X�g�\���̑O�̃f�[�^�ւ̃|�C���^
} ;

// �萔�o�b�t�@�Ǘ����\����
struct CONSTANTBUFFER_DIRECT3D11_MANAGE
{
	int								BufferNum ;								// �萔�o�b�t�@�\���̂̐�
	CONSTANTBUFFER_DIRECT3D11 *		FirstBuffer ;							// �ŏ��̒萔�o�b�t�@�\���̂ւ̃|�C���^
} ;

// Direct3D11�p �V�F�[�_�[�p�萔�o�b�t�@�n���h�����
struct SHADERCONSTANTBUFFERHANDLEDATA_DIRECT3D11
{
	int								SetFlag ;								// �V�F�[�_�[�ɃZ�b�g���Ă��邩�ǂ���
	CONSTANTBUFFER_DIRECT3D11 *		ConstBuffer ;							// �萔�o�b�t�@
} ;

// Direct3D11 �̕W���`��p�̃V�F�[�_�[��Z�߂��\����
struct GRAPHICS_HARDWARE_DIRECT3D11_SHADER_BASE
{
	// �W���`��p�̕��G�ȏ������s��Ȃ����_�V�F�[�_�[
	D_ID3D11VertexShader					*BaseSimple_VS[ D3D11_VERTEX_INPUTLAYOUT_NUM ] ;

	// �Œ�@�\�p�C�v���C���݊��̃s�N�Z���V�F�[�_�[( �e�N�X�`���Ȃ� )
	// [ �������( 0:�ʏ�`��  1:��Z�`��  2:RGB���]  3:�`��P�x�S�{  4:��Z�ς݃��u�����h���[�h�̒ʏ�`��  5:��Z�ς݃��u�����h���[�h��RGB���]  6:��Z�ς݃��u�����h���[�h�̕`��P�x4�{ ) ]
	// [ �s�N�Z���V�F�[�_�[�̃A���t�@�e�X�g���[�h ]
	D_ID3D11PixelShader						*BaseNoneTex_PS[ 7 ][ DIRECT3D11_PS_ALPHATEST_NUM ] ;

	// �Œ�@�\�p�C�v���C���݊��̃s�N�Z���V�F�[�_�[( �e�N�X�`������ )
	// [ �u�����h�摜�Ƃ̃u�����h�^�C�v( 0:�Ȃ�  1:DX_BLENDGRAPHTYPE_NORMAL  2:DX_BLENDGRAPHTYPE_WIPE  3:DX_BLENDGRAPHTYPE_ALPHA ) ]
	// [ �������( 0:�ʏ�`��  1:��Z�`��  2:RGB���]  3:�`��P�x�S�{  4:��Z�ς݃��u�����h���[�h�̒ʏ�`��  5:��Z�ς݃��u�����h���[�h��RGB���]  6:��Z�ς݃��u�����h���[�h�̕`��P�x4�{ ) ]
	// [ �e�N�X�`��RGB����( 0:�������Ȃ�  1:�������� ) ]
	// [ �e�N�X�`��Alpha����( 0:�������Ȃ�  1:�������� ) ]
	// [ �s�N�Z���V�F�[�_�[�̃A���t�@�e�X�g���[�h ]
	D_ID3D11PixelShader						*BaseUseTex_PS[ 4 ][ 7 ][ 2 ][ 2 ][ DIRECT3D11_PS_ALPHATEST_NUM ] ;

	// �}�X�N�����p�̃s�N�Z���V�F�[�_�[
	D_ID3D11PixelShader						*MaskEffect_PS ;
	D_ID3D11PixelShader						*MaskEffect_ReverseEffect_PS ;
	D_ID3D11PixelShader						*MaskEffect_UseGraphHandle_PS[ 4 ] ;
	D_ID3D11PixelShader						*MaskEffect_UseGraphHandle_ReverseEffect_PS[ 4 ] ;

	// �P���]���p�֌W
	D_ID3D11SamplerState					*StretchRect_SamplerState[ 2 /* 0:Point  1:Linear */ ] ;
	D_ID3D11RasterizerState					*StretchRect_RasterizerState ;
	D_ID3D11DepthStencilState				*StretchRect_DepthStencilState ;
	D_ID3D11BlendState						*StretchRect_BlendState[ 2 /* 0:NoBlend  1:AlphaBlend */ ] ;
	D_ID3D11VertexShader					*StretchRect_VS ;
	D_ID3D11VertexShader					*StretchRectTex8_VS ;
	D_ID3D11PixelShader						*StretchRect_PS ;
} ;

// Direct3D11 �̕W���R�c�`��p�̃V�F�[�_�[��Z�߂��\����
struct GRAPHICS_HARDWARE_DIRECT3D11_SHADER_BASE3D
{
	// �W���R�c�`��p�s�N�Z���P�ʃ��C�e�B���O�^�C�v�̒��_�V�F�[�_�[( ���X�g�͔z��̍����� )
	// [ �V���h�E�}�b�v�̗L��( 0:���� 1:�L�� ) ]
	// /* [ ���b�V���^�C�v( 0:���̃��b�V�� 1:4�{�[�����X�L�j���O���b�V�� 2:8�{�[�����X�L�j���O���b�V�� ) ] */
	// /* [ �o���v�}�b�v�̗L��( 0:���� 1:�L�� ) ] */
	// [ �t�H�O�^�C�v ]
	D_ID3D11VertexShader					*Base3D_PixelLighting_VS[ 2 ]/*[ 3 ]*//*[ 2 ]*/[ 4 ] ;

	// �W���R�c�`��p�s�N�Z���P�ʃ��C�e�B���O�^�C�v�̃s�N�Z���V�F�[�_�[( ���X�g�͔z��̍����� )
	// [ �V���h�E�}�b�v�̗L��( 0:���� 1:�L�� ) ]
	// /*[ �}���`�e�N�X�`���̃u�����h���[�h�AMV1_LAYERBLEND_TYPE_TRANSLUCENT �Ȃǂ� +1 �������� ( 0:�}���`�e�N�X�`������  1:���u�����h  2:���Z�u�����h  3:��Z�u�����h  4:��Z�u�����h�~2 ) ]*/
	// /*[ �X�y�L�����}�b�v ( 0:���� 1:���� ) ]*/
	// /*[ �o���v�}�b�v ( 0:�Ȃ� 1:���� ) ]*/
	// [ ���C�g�C���f�b�N�X ]
	// /*[ �X�y�L�����̗L��( 0:���� 1:�L�� ) ]*/
	// [ �������( 0:�ʏ�`��  1:��Z�`��  2:RGB���]  3:�`��P�x�S�{  4:��Z�ς݃��u�����h���[�h�̒ʏ�`��  5:��Z�ς݃��u�����h���[�h��RGB���]  6:��Z�ς݃��u�����h���[�h�̕`��P�x4�{ ) ]
	// [ �s�N�Z���V�F�[�_�[�̃A���t�@�e�X�g���[�h ]
	D_ID3D11PixelShader						*Base3D_PixelLighting_Normal_PS[ 2 ]/*[ 5 ]*//*[ 2 ]*//*[ 2 ]*/[ 84 ]/*[ 2 ]*/[ 7 ][ DIRECT3D11_PS_ALPHATEST_NUM ] ;



	// �W���R�c�`��p�̃V���h�E�}�b�v�ւ̃����_�����O�p���_�V�F�[�_�[
	// /*[ ���b�V���^�C�v( 0:���̃��b�V�� 1:4�{�[�����X�L�j���O���b�V��  2:8�{�[�����X�L�j���O���b�V�� ) ]*/
	// [ �o���v�}�b�v�̗L��( 0:���� 1:�L�� ) ]
	D_ID3D11VertexShader					*Base3D_ShadowMap_VS/*[ 3 ]*/[ 2 ] ;

	// �W���R�c�`��p�̃��C�e�B���O�Ȃ����_�V�F�[�_�[
	// /*[ ���b�V���^�C�v( 0:���̃��b�V�� 1:4�{�[�����X�L�j���O���b�V��  2:8�{�[�����X�L�j���O���b�V�� ) ]*/
	// [ �t�H�O�^�C�v ]
	D_ID3D11VertexShader					*Base3D_NoLighting_VS/*[ 3 ]*/[ 4 ] ;

	// �W���R�c�`��p�̒��_�P�ʃ��C�e�B���O���_�V�F�[�_�[
	// [ �V���h�E�}�b�v�̗L��( 0:���� 1:�L�� ) ]
	// /*[ ���b�V���^�C�v( 0:���̃��b�V�� 1:4�{�[�����X�L�j���O���b�V��  2:8�{�[�����X�L�j���O���b�V�� ) ]*/
	// /*[ �o���v�}�b�v�̗L��( 0:���� 1:�L�� ) ]*/
	// [ �t�H�O�^�C�v ]
	// [ ���C�g�C���f�b�N�X ]
	// /*[ �X�y�L�����̗L��( 0:���� 1:�L�� ) ]*/
	D_ID3D11VertexShader					*Base3D_VertexLighting_VS[ 2 ]/*[ 3 ]*//*[ 2 ]*/[ 4 ][ 20 ]/*[ 2 ]*/ ;

	// �W���R�c�`��p�̃V���h�E�}�b�v�ւ̃����_�����O�p�s�N�Z���V�F�[�_�[
	// [ �s�N�Z���V�F�[�_�[�̃A���t�@�e�X�g���[�h ]
	D_ID3D11PixelShader						*Base3D_ShadowMap_Normal_PS[ DIRECT3D11_PS_ALPHATEST_NUM ] ;

	// �W���R�c�`��p�̃��C�e�B���O�Ȃ��s�N�Z���V�F�[�_�[
	// /*[ �}���`�e�N�X�`���̃u�����h���[�h�AMV1_LAYERBLEND_TYPE_TRANSLUCENT �Ȃǂ� +1 �������� ( 0:�}���`�e�N�X�`������  1:���u�����h  2:���Z�u�����h  3:��Z�u�����h  4:��Z�u�����h�~2 ) ]*/
	// [ �������( 0:�ʏ�`��  1:��Z�`��  2:RGB���]  3:�`��P�x�S�{  4:��Z�ς݃��u�����h���[�h�̒ʏ�`��  5:��Z�ς݃��u�����h���[�h��RGB���]  6:��Z�ς݃��u�����h���[�h�̕`��P�x4�{ ) ]
	// [ �s�N�Z���V�F�[�_�[�̃A���t�@�e�X�g���[�h ]
	D_ID3D11PixelShader						*Base3D_NoLighting_Normal_PS/*[ 5 ]*/[ 7 ][ DIRECT3D11_PS_ALPHATEST_NUM ] ;

	// �W���R�c�`��p�̒��_�P�ʃ��C�e�B���O�s�N�Z���V�F�[�_�[
	// [ �V���h�E�}�b�v�̗L��( 0:���� 1:�L�� ) ]
	// /* [ �}���`�e�N�X�`���̃u�����h���[�h�AMV1_LAYERBLEND_TYPE_TRANSLUCENT �Ȃǂ� +1 �������� ( 0:�}���`�e�N�X�`������  1:���u�����h  2:���Z�u�����h  3:��Z�u�����h  4:��Z�u�����h�~2 ) ] */
	// /* [ �X�y�L�����}�b�v ( 0:���� 1:���� ) ] */
	// /* [ �o���v�}�b�v ( 0:�Ȃ� 1:���� ) ] */
	// [ ���C�g�C���f�b�N�X ]
	// /*[ �X�y�L�����̗L��( 0:���� 1:�L�� ) ]*/
	// [ �������( 0:�ʏ�`��  1:��Z�`��  2:RGB���]  3:�`��P�x�S�{  4:��Z�ς݃��u�����h���[�h�̒ʏ�`��  5:��Z�ς݃��u�����h���[�h��RGB���]  6:��Z�ς݃��u�����h���[�h�̕`��P�x4�{ ) ]
	// [ �s�N�Z���V�F�[�_�[�̃A���t�@�e�X�g���[�h ]
	D_ID3D11PixelShader						*Base3D_VertexLighting_Normal_PS[ 2 ]/*[ 5 ]*//*[ 2 ]*//*[ 2 ]*/[ 10 ]/*[ 2 ]*/[ 7 ][ DIRECT3D11_PS_ALPHATEST_NUM ] ;
} ;


#ifndef DX_NON_MODEL

// Direct3D11 �̃��f���`��p�̃V�F�[�_�[��Z�߂��\����
struct GRAPHICS_HARDWARE_DIRECT3D11_SHADER_MODEL
{
	// ���f���`��p�s�N�Z���P�ʃ��C�e�B���O�^�C�v�̒��_�V�F�[�_�[( ���X�g�͔z��̍����� )
	// [ �V���h�E�}�b�v�̗L��( 0:���� 1:�L�� ) ]
	// [ ���b�V���^�C�v( 0:���̃��b�V�� 1:4�{�[�����X�L�j���O���b�V�� 2:8�{�[�����X�L�j���O���b�V�� ) ]
	// [ �o���v�}�b�v�̗L��( 0:���� 1:�L�� ) ]
	// [ �t�H�O�^�C�v ]
	D_ID3D11VertexShader					*MV1_PixelLighting_VS[ 2 ][ 3 ][ 2 ][ 4 ] ;

	// ���f���`��p�s�N�Z���P�ʃ��C�e�B���O�^�C�v�̃g�D�[���^�C�v�P�p�s�N�Z���V�F�[�_�[( ���X�g�͔z��̍����� )
	// [ �V���h�E�}�b�v�̗L��( 0:���� 1:�L�� ) ]
	// [ �g�D�[�������_�����O�̃f�B�t���[�Y�O���f�[�V�����̃u�����h�^�C�v( 0:MV1_LAYERBLEND_TYPE_TRANSLUCENT  1:MV1_LAYERBLEND_TYPE_MODULATE ) ]
	// [ �g�D�[�������_�����O�̃X�y�L�����O���f�[�V�����̃u�����h�^�C�v( 0:MV1_LAYERBLEND_TYPE_TRANSLUCENT  1:MV1_LAYERBLEND_TYPE_ADDITIVE ) ]
	// [ �o���v�}�b�v ( 0:�Ȃ� 1:���� ) ]
	// [ �L���ȃ��C�g�̐� - 1 ]
	// [ �s�N�Z���V�F�[�_�[�̃A���t�@�e�X�g���[�h ]
	D_ID3D11PixelShader						*MV1_PixelLighting_ToonType1_PS[ 2 ][ 2 ][ 2 ][ 2 ][ DX_D3D11_COMMON_CONST_LIGHT_NUM ][ DIRECT3D11_PS_ALPHATEST_NUM ] ;

	// ���f���`��p�s�N�Z���P�ʃ��C�e�B���O�^�C�v�̃g�D�[���^�C�v�Q�p�s�N�Z���V�F�[�_�[( ���X�g�͔z��̍����� )
	// [ �V���h�E�}�b�v�̗L��( 0:���� 1:�L�� ) ]
	// [ �g�D�[�������_�����O�̃X�t�B�A�}�b�v�̗L���ƃu�����h�^�C�v( 0:�X�t�B�A�}�b�v�͖���   1:MV1_LAYERBLEND_TYPE_MODULATE  2:MV1_LAYERBLEND_TYPE_ADDITIVE ) ]
	// [ �L���ȃ��C�g�̐� - 1 ]
	// [ �s�N�Z���V�F�[�_�[�̃A���t�@�e�X�g���[�h ]
	D_ID3D11PixelShader						*MV1_PixelLighting_ToonType2_PS[ 2 ][ 3 ][ DX_D3D11_COMMON_CONST_LIGHT_NUM ][ DIRECT3D11_PS_ALPHATEST_NUM ] ;

	// ���f���`��p�s�N�Z���P�ʃ��C�e�B���O�^�C�v�̃s�N�Z���V�F�[�_�[( ���X�g�͔z��̍����� )
	// [ �V���h�E�}�b�v�̗L��( 0:���� 1:�L�� ) ]
	// [ �}���`�e�N�X�`���̃u�����h���[�h�AMV1_LAYERBLEND_TYPE_TRANSLUCENT �Ȃǂ� +1 �������� ( 0:�}���`�e�N�X�`������  1:���u�����h  2:���Z�u�����h  3:��Z�u�����h  4:��Z�u�����h�~2 ) ]
	// [ �o���v�}�b�v ( 0:�Ȃ� 1:���� ) ]
	// [ �L���ȃ��C�g�̐� - 1 ]
	// [ �s�N�Z���V�F�[�_�[�̃A���t�@�e�X�g���[�h ]
	D_ID3D11PixelShader						*MV1_PixelLighting_Normal_PS[ 2 ][ 5 ][ 2 ][ DX_D3D11_COMMON_CONST_LIGHT_NUM ][ DIRECT3D11_PS_ALPHATEST_NUM ] ;





	// ���f���`��p�̃V���h�E�}�b�v�ւ̃g�D�[�������_�����O�֊s���`��p���_�V�F�[�_�[
	// [ ���b�V���^�C�v( 0:���̃��b�V�� 1:4�{�[�����X�L�j���O���b�V��  2:8�{�[�����X�L�j���O���b�V�� ) ]
	// [ �o���v�}�b�v�̗L��( 0:���� 1:�L�� ) ]
	D_ID3D11VertexShader					*MV1_ToonOutLine_ShadowMap_VS[ 3 ][ 2 ] ;

	// ���f���`��p�̃g�D�[�������_�����O�̗֊s���`��p���_�V�F�[�_�[
	// [ ���b�V���^�C�v( 0:���̃��b�V�� 1:4�{�[�����X�L�j���O���b�V��  2:8�{�[�����X�L�j���O���b�V�� ) ]
	// [ �o���v�}�b�v�̗L��( 0:���� 1:�L�� ) ]
	// [ �t�H�O�^�C�v ]
	D_ID3D11VertexShader					*MV1_ToonOutLine_VS[ 3 ][ 2 ][ 4 ] ;

	// ���f���`��p�̃V���h�E�}�b�v�ւ̃����_�����O�p���_�V�F�[�_�[
	// [ ���b�V���^�C�v( 0:���̃��b�V�� 1:4�{�[�����X�L�j���O���b�V��  2:8�{�[�����X�L�j���O���b�V�� ) ]
	// [ �o���v�}�b�v�̗L��( 0:���� 1:�L�� ) ]
	D_ID3D11VertexShader					*MV1_ShadowMap_VS[ 3 ][ 2 ] ;

	// ���f���`��p�̃��C�e�B���O�Ȃ����_�V�F�[�_�[
	// [ ���b�V���^�C�v( 0:���̃��b�V�� 1:4�{�[�����X�L�j���O���b�V��  2:8�{�[�����X�L�j���O���b�V�� ) ]
	// [ �o���v�}�b�v�̗L��( 0:���� 1:�L�� ) ]
	// [ �t�H�O�^�C�v ]
	D_ID3D11VertexShader					*MV1_NoLighting_VS[ 3 ][ 2 ][ 4 ] ;

	// ���f���`��p�̒��_�P�ʃ��C�e�B���O���蒸�_�V�F�[�_�[
	// [ �V���h�E�}�b�v�̗L��( 0:���� 1:�L�� ) ]
	// [ ���b�V���^�C�v( 0:���̃��b�V�� 1:4�{�[�����X�L�j���O���b�V��  2:8�{�[�����X�L�j���O���b�V�� ) ]
	// [ �o���v�}�b�v�̗L��( 0:���� 1:�L�� ) ]
	// [ �t�H�O�^�C�v ]
	// [ ���C�g�C���f�b�N�X ]
	D_ID3D11VertexShader					*MV1_VertexLighting_VS[ 2 ][ 3 ][ 2 ][ 4 ][ 20 ] ;

	// ���f���`��p�̃V���h�E�}�b�v�ւ̃g�D�[�������_�����O�p�s�N�Z���V�F�[�_�[
	// [ �g�D�[�������_�����O�^�C�v( 0:�g�D�[�������_�����O type 1   1:�g�D�[�������_�����O type 2 ) ]
	// [ �g�D�[�������_�����O�̃X�t�B�A�}�b�v�̗L���ƃu�����h�^�C�v( 0:�X�t�B�A�}�b�v�͖���   1:MV1_LAYERBLEND_TYPE_MODULATE  2:MV1_LAYERBLEND_TYPE_ADDITIVE ) ]
	// [ �g�D�[�������_�����O�̃f�B�t���[�Y�O���f�[�V�����̃u�����h�^�C�v( 0:MV1_LAYERBLEND_TYPE_TRANSLUCENT  1:MV1_LAYERBLEND_TYPE_MODULATE ) ]
	// [ �s�N�Z���V�F�[�_�[�̃A���t�@�e�X�g���[�h ]
	D_ID3D11PixelShader						*MV1_ShadowMap_Toon_PS[ 2 ][ 3 ][ 2 ][ DIRECT3D11_PS_ALPHATEST_NUM ] ;

	// ���f���`��p�̃V���h�E�}�b�v�ւ̃����_�����O�p�s�N�Z���V�F�[�_�[
	// [ �s�N�Z���V�F�[�_�[�̃A���t�@�e�X�g���[�h ]
	D_ID3D11PixelShader						*MV1_ShadowMap_Normal_PS[ DIRECT3D11_PS_ALPHATEST_NUM ] ;

	// ���f���`��p�̃��C�e�B���O�Ȃ��g�D�[���p�s�N�Z���V�F�[�_�[( ���X�g�͔z��̍����� )
	// [ �g�D�[�������_�����O�^�C�v( 0:�g�D�[�������_�����O type 1   1:�g�D�[�������_�����O type 2 ) ]
	// [ �g�D�[�������_�����O�̃f�B�t���[�Y�O���f�[�V�����̃u�����h�^�C�v( 0:MV1_LAYERBLEND_TYPE_TRANSLUCENT or �g�D�[�������_�����O�ł͂Ȃ�  1:MV1_LAYERBLEND_TYPE_MODULATE ) ]
	// [ �s�N�Z���V�F�[�_�[�̃A���t�@�e�X�g���[�h ]
	D_ID3D11PixelShader						*MV1_NoLighting_Toon_PS[ 2 ][ 2 ][ DIRECT3D11_PS_ALPHATEST_NUM ] ;

	// ���f���`��p�̃��C�e�B���O�Ȃ��s�N�Z���V�F�[�_�[
	// [ �}���`�e�N�X�`���̃u�����h���[�h�AMV1_LAYERBLEND_TYPE_TRANSLUCENT �Ȃǂ� +1 �������� ( 0:�}���`�e�N�X�`������  1:���u�����h  2:���Z�u�����h  3:��Z�u�����h  4:��Z�u�����h�~2 ) ]
	// [ �s�N�Z���V�F�[�_�[�̃A���t�@�e�X�g���[�h ]
	D_ID3D11PixelShader						*MV1_NoLighting_Normal_PS[ 5 ][ DIRECT3D11_PS_ALPHATEST_NUM ] ;

	// ���f���`��p�̒��_�P�ʃ��C�e�B���O����g�D�[���^�C�v�P�p�s�N�Z���V�F�[�_�[
	// [ �V���h�E�}�b�v�̗L��( 0:���� 1:�L�� ) ]
	// [ �g�D�[�������_�����O�̃f�B�t���[�Y�O���f�[�V�����̃u�����h�^�C�v( 0:MV1_LAYERBLEND_TYPE_TRANSLUCENT  1:MV1_LAYERBLEND_TYPE_MODULATE ) ]
	// [ �g�D�[�������_�����O�̃X�y�L�����O���f�[�V�����̃u�����h�^�C�v( 0:MV1_LAYERBLEND_TYPE_TRANSLUCENT  1:MV1_LAYERBLEND_TYPE_ADDITIVE ) ]
	// [ �o���v�}�b�v ( 0:�Ȃ� 1:���� ) ]
	// [ ���C�g�C���f�b�N�X ]
	// [ �s�N�Z���V�F�[�_�[�̃A���t�@�e�X�g���[�h ]
	D_ID3D11PixelShader						*MV1_VertexLighting_ToonType1_PS[ 2 ][ 2 ][ 2 ][ 2 ][ 10 ][ DIRECT3D11_PS_ALPHATEST_NUM ] ;

	// ���f���`��p�̒��_�P�ʃ��C�e�B���O����g�D�[���^�C�v�Q�p�s�N�Z���V�F�[�_�[
	// [ �V���h�E�}�b�v�̗L��( 0:���� 1:�L�� ) ]
	// [ �g�D�[�������_�����O�̃X�t�B�A�}�b�v�̗L���ƃu�����h�^�C�v( 0:�X�t�B�A�}�b�v�͖���   1:MV1_LAYERBLEND_TYPE_MODULATE  2:MV1_LAYERBLEND_TYPE_ADDITIVE ) ]
	// [ ���C�g�C���f�b�N�X ]
	// [ �s�N�Z���V�F�[�_�[�̃A���t�@�e�X�g���[�h ]
	D_ID3D11PixelShader						*MV1_VertexLighting_ToonType2_PS[ 2 ][ 3 ][ 10 ][ DIRECT3D11_PS_ALPHATEST_NUM ] ;

	// ���f���`��p�̒��_�P�ʃ��C�e�B���O����s�N�Z���V�F�[�_�[
	// [ �V���h�E�}�b�v�̗L��( 0:���� 1:�L�� ) ]
	// [ �}���`�e�N�X�`���̃u�����h���[�h�AMV1_LAYERBLEND_TYPE_TRANSLUCENT �Ȃǂ� +1 �������� ( 0:�}���`�e�N�X�`������  1:���u�����h  2:���Z�u�����h  3:��Z�u�����h  4:��Z�u�����h�~2 ) ]
	// [ �o���v�}�b�v ( 0:�Ȃ� 1:���� ) ]
	// [ ���C�g�C���f�b�N�X ]
	// [ �s�N�Z���V�F�[�_�[�̃A���t�@�e�X�g���[�h ]
	D_ID3D11PixelShader						*MV1_VertexLighting_Normal_PS[ 2 ][ 5 ][ 2 ][ 10 ][ DIRECT3D11_PS_ALPHATEST_NUM ] ;
} ;
#endif // DX_NON_MODEL

// Direct3D11 �̒萔����Z�߂��\����
struct GRAPHICS_HARDWARE_DIRECT3D11_CONSTANT
{
	CONSTANTBUFFER_DIRECT3D11_MANAGE			ConstantBufferManage ;			// �萔�o�b�t�@���

	CONSTANTBUFFER_DIRECT3D11 *					ConstBuffer_Common ;			// ���_�V�F�[�_�[���s�N�Z���V�F�[�_�[���ʂ̃p�����[�^�p�萔�o�b�t�@( DX_D3D11_CONST_BUFFER_COMMON �\���� )
	int											SetNormalMaterial ;				// �萔�o�b�t�@�ɕW���`��p�̃}�e���A���p�����[�^���ݒ肳��Ă��邩�ǂ���( TRUE:�ݒ肳��Ă���  FALSE:�ݒ肳��Ă��Ȃ� )

	CONSTANTBUFFER_DIRECT3D11 *					ConstBuffer_VS_Base ;			// ���_�V�F�[�_�[��{�p�����[�^�p�萔�o�b�t�@( DX_D3D11_VS_CONST_BUFFER_BASE �\���� )
	int											SetNormalColorSource ;			// �萔�o�b�t�@�ɕW���`��p�́u�f�B�t���[�Y�J���[�E�X�y�L�����J���[���}�e���A�����g�p���邩�A���_�f�[�^���g�p���邩�v�̐ݒ肪����Ă��邩�ǂ���( TRUE:�ݒ肳��Ă���  FALSE:�ݒ肳��Ă��Ȃ� )

	CONSTANTBUFFER_DIRECT3D11 *					ConstBuffer_VS_OtherMatrix ;	// ���_�V�F�[�_�[���̑��s��p�����[�^�p�萔�o�b�t�@( DX_D3D11_VS_CONST_BUFFER_OTHERMATRIX �\���� )
	int											SetNormalTextureAddressTransformMatrix ;	// �萔�o�b�t�@�ɕW���`��p�̃e�N�X�`�����W�p�s�񂪐ݒ肳��Ă��邩�ǂ���( TRUE:�ݒ肳��Ă���  FALSE:�ݒ肳��Ă��Ȃ� )

	CONSTANTBUFFER_DIRECT3D11 *					ConstBuffer_VS_LocalWorldMatrix ;	// ���_�V�F�[�_�[�X�L�j���O���b�V���p�́@���[�J���@���@���[���h�s��p�����[�^�p�萔�o�b�t�@( DX_D3D11_VS_CONST_BUFFER_LOCALWORLDMATRIX �\���� )

	CONSTANTBUFFER_DIRECT3D11 *					ConstBuffer_PS_Base ;			// �s�N�Z���V�F�[�_�[��{�p�����[�^�p�萔�o�b�t�@( DX_D3D11_PS_CONST_BUFFER_BASE �\���� )
	CONSTANTBUFFER_DIRECT3D11 *					ConstBuffer_PS_ShadowMap ;		// �s�N�Z���V�F�[�_�[�V���h�E�}�b�v�p�萔�o�b�t�@( DX_D3D11_PS_CONST_BUFFER_SHADOWMAP �\���� )
	CONSTANTBUFFER_DIRECT3D11 *					ConstBuffer_PS_Filter ;			// �s�N�Z���V�F�[�_�[�t�B���^�[�p�萔�o�b�t�@
} ;

// Direct3D11 �̃V�F�[�_�[����Z�߂��\����
struct GRAPHICS_HARDWARE_DIRECT3D11_SHADER
{
	GRAPHICS_HARDWARE_DIRECT3D11_CONSTANT		Constant ;						// �萔����Z�߂��\����

	GRAPHICS_HARDWARE_DIRECT3D11_SHADER_BASE	Base ;							// �W���`�揈���p�̃V�F�[�_�[��Z�߂��\����
	GRAPHICS_HARDWARE_DIRECT3D11_SHADER_BASE3D	Base3D ;						// �W���R�c�`�揈���p�̃V�F�[�_�[��Z�߂��\����

#ifndef DX_NON_MODEL
	GRAPHICS_HARDWARE_DIRECT3D11_SHADER_MODEL	Model ;							// ���f���`�揈���p�̃V�F�[�_�[��Z�߂��\����
#endif // DX_NON_MODEL
} ;



// Direct3D11 �̓��̓��C�A�E�g��Z�߂��\����
struct GRAPHICS_HARDWARE_DIRECT3D11_INPUTLAYOUT
{
	// �W���`��p�̒��_�V�F�[�_�[�Ŏg�p���钸�_�f�[�^�t�H�[�}�b�g
	D_ID3D11InputLayout *						BaseInputLayout[ D3D11_VERTEX_INPUTLAYOUT_NUM ] ;

	// �P���]���p�̒��_�V�F�[�_�[�Ŏg�p���钸�_�f�[�^�t�H�[�}�b�g
	D_ID3D11InputLayout *						BaseStretchRectInputLayout ;
	D_ID3D11InputLayout *						BaseStretchRectTex8InputLayout ;

#ifndef DX_NON_MODEL
	// ���̓��C�A�E�g( ���X�g�͔z��̍����� )
	// [ �o���v�}�b�v���t�����ǂ���( 1:�o���v�}�b�v�t�� 0:�t���ĂȂ� ) ]
	// [ �X�L�j���O���b�V�����ǂ���( 0:���̃��b�V�� 1:4�{�[�����X�L�j���O���b�V�� 2:8�{�[�����X�L�j���O���b�V�� ) ]
	D_ID3D11InputLayout *						MV1_VertexInputLayout[ 2 ][ 3 ] ;
#endif // DX_NON_MODEL
} ;






// D3D11Device �ɐݒ肵�Ă���l��Z�߂��\����
struct GRAPHICS_HARDDATA_DIRECT3D11_DEVICE_STATE
{
	int										FogEnable ;										// �t�H�O���L�����ǂ���( TRUE:�L��  FALSE:���� )
	int										FogMode ;										// �t�H�O���[�h
	DWORD									FogColor ;										// �t�H�O�J���[
	float									FogStart ;										// �t�H�O�J�n����
	float									FogEnd ;										// �t�H�O�I������
	float									FogDensity ;									// �t�H�O���x

	COLOR_F									GlobalAmbientLightColor ;						// �O���[�o���A���r�G���g���C�g�J���[
	int										LightEnableMaxIndex ;							// �L���ȃ��C�g�̍ő�C���f�b�N�X
	int										LightEnableFlag[ DX_D3D11_COMMON_CONST_LIGHT_NUM ] ;// ���C�g���L�����ǂ����t���O( TRUE:�L��  FALSE:���� )
	LIGHTPARAM								LightParam[ DX_D3D11_COMMON_CONST_LIGHT_NUM ] ;	// ���C�g�̃p�����[�^
	int										Lighting ;										// ���C�e�B���O���s�����ǂ����̃t���O( TRUE:���C�e�B���O���s��  FALSE:���C�e�B���O�Ȃ� )

	int										UseSpecular ;									// �X�y�L�������g�p���邩�ǂ���
	MATERIALPARAM							Material ;										// �}�e���A��
	int										MaterialUseVertexDiffuseColor ;					// ���_�̃f�B�t���[�Y�J���[���}�e���A���̃f�B�t���[�Y�J���[�Ƃ��Ďg�p���邩�ǂ���
	int										MaterialUseVertexSpecularColor ;				// ���_�̃X�y�L�����J���[���}�e���A���̃X�y�L�����J���[�Ƃ��Ďg�p���邩�ǂ���

	int										DrawMode ;										// �`�惂�[�h( DX_DRAWMODE_BILINEAR �Ȃ� )�A�W���̃t�B���^�����O���[�h�ł͂Ȃ��ꍇ�� -1
	int										MaxAnisotropy ;									// �ő�ٕ����A�S�ẴT���v���ɑ΂��ē����ݒ肪�s���Ă��Ȃ��ꍇ�� -1
	int										ChangeSamplerDesc ;								// �f�o�C�X�̃T���v���[�X�e�[�g�̐ݒ�ɕύX�����������ǂ���( TRUE:�ύX��������  FALSE:�ύX�͂Ȃ� )
	int										ChangeSamplerDescSlot[ USE_TEXTURESTAGE_NUM ] ;	// �f�o�C�X�̊e�X���b�g�̃T���v���[�X�e�[�g�̐ݒ�ɕύX�����������ǂ���( TRUE:�ύX��������  FALSE:�ύX�͂Ȃ� )
	D_D3D11_SAMPLER_DESC					SamplerDesc[ USE_TEXTURESTAGE_NUM ] ;			// �f�o�C�X�̊e�X���b�g�̃T���v���[�X�e�[�g�̐ݒ�l
	D_ID3D11SamplerState *					SamplerState[ USE_TEXTURESTAGE_NUM ] ;			// ���݃f�o�C�X�̊e�X���b�g�ɐݒ肳��Ă���T���v���[�X�e�[�g
	int										ChangeSamplerStateReleaseFlag[ USE_TEXTURESTAGE_NUM ] ;					// ���݃f�o�C�X�̊e�X���b�g�ɐݒ肳��Ă���T���v���[�X�e�[�g��ύX����ۂɃ����[�X���邩�ǂ����̃t���O( TRUE:�����[�X����  FALSE:�����[�X���Ȃ� )
	D_ID3D11SamplerState *					DefaultSamplerStateArray[ DIRECT3D11_DEFAULTRS_FILTER_NUM ][ D_D3D11_TEXTURE_ADDRESS_MIRROR_ONCE + 1 ][ D_D3D11_TEXTURE_ADDRESS_MIRROR_ONCE + 1 ][ D_D3D11_TEXTURE_ADDRESS_MIRROR_ONCE + 1 ][ 2 /* MaxAnisotropy  �v�f�ԍ�0 = 1  �v�f�ԍ�1 = 16 */ ] ;	// �悭�g���ݒ�̃T���v���[�X�e�[�g�̔z��

	D_D3D11_VIEWPORT						Viewport ;										// �r���[�|�[�g���

	int										ChangeRasterizerDesc ;							// �f�o�C�X�̃��X�^���C�U�[�X�e�[�g�̐ݒ�ɕύX�����������ǂ���( TRUE:�ύX��������  FALSE:�ύX�͂Ȃ� )
	D_D3D11_RASTERIZER_DESC					RasterizerDesc ;								// ���݃f�o�C�X�ɐݒ肳��Ă��郉�X�^���C�U�[�X�e�[�g�̐ݒ�l
	D_ID3D11RasterizerState *				RasterizerState ;								// ���݃f�o�C�X�ɐݒ肳��Ă��郉�X�^���C�U�[�X�e�[�g
	int										ChangeRasterizerStateReleaseFlag ;				// ���݃f�o�C�X�ɐݒ肳��Ă��郉�X�^���C�U�[�X�e�[�g��ύX����ۂɃ����[�X���邩�ǂ����̃t���O( TRUE:�����[�X����  FALSE:�����[�X���Ȃ� )
	D_ID3D11RasterizerState *				DefaultRasterizerStateArray[ D_D3D11_CULL_BACK + 1 ][ D_D3D11_FILL_SOLID + 1 ][ 2 /* �[�x�e�X�g�̗L�� */ ][ 2 /* �}���`�T���v���̗L�� */ ] ;	// �悭�g���ݒ�̃��X�^���C�U�[�X�e�[�g�̔z��

	int										ChangeDepthStencilDesc ;						// �f�o�C�X�̐[�x�X�e���V���X�e�[�g�̐ݒ�ɕύX�����������ǂ���( TRUE:�ύX��������  FALSE:�ύX�͂Ȃ� )
	D_D3D11_DEPTH_STENCIL_DESC				DepthStencilDesc ;								// ���݃f�o�C�X�ɐݒ肳��Ă���[�x�X�e���V���e�X�g�X�e�[�g�̐ݒ�l
	D_ID3D11DepthStencilState *				DepthStencilState ;								// ���݃f�o�C�X�ɐݒ肳��Ă���[�x�X�e���V���X�e�[�g
	int										ChangeDepthStencilStateReleaseFlag ;			// ���݃f�o�C�X�ɐݒ肳��Ă���[�x�X�e���V���X�e�[�g��ύX����ۂɃ����[�X���邩�ǂ����̃t���O( TRUE:�����[�X����  FALSE:�����[�X���Ȃ� )
	D_ID3D11DepthStencilState *				DefaultDepthStencilStateArray[ 2 /* �[�x�e�X�g�̗L�� */ ][ 2 /* �[�x�o�b�t�@�ւ̏������݂̗L�� */ ][ D_D3D11_COMPARISON_ALWAYS + 1 ] ;	// �悭�g���ݒ�̐[�x�X�e���V���X�e�[�g�̔z��

	int										BlendMode ;										// ���݃f�o�C�X�ɐݒ肳��Ă���u�����h���[�h�A�v���Z�b�g�̃u�����h���[�h�ȊO�̏ꍇ�� -1 ������
	int										ChangeBlendDesc ;								// �f�o�C�X�̃u�����f�B���O�X�e�[�g�̐ݒ�ɕύX�����������ǂ���( TRUE:�ύX��������  FALSE:�ύX�͂Ȃ� )
	D_D3D11_BLEND_DESC						BlendDesc ;										// ���݃f�o�C�X�ɐݒ肳��Ă���u�����f�B���O�X�e�[�g�̐ݒ�l
	D_ID3D11BlendState *					BlendState ;									// ���݃f�o�C�X�ɐݒ肳��Ă���u�����f�B���O�X�e�[�g
	int										ChangeBlendStateReleaseFlag ;					// ���݃f�o�C�X�ɐݒ肳��Ă���u�����f�B���O�X�e�[�g��ύX����ۂɃ����[�X���邩�ǂ����̃t���O( TRUE:�����[�X����  FALSE:�����[�X���Ȃ� )
	D_ID3D11BlendState *					DefaultBlendStateArray[ DX_BLENDMODE_NUM ] ;	// �悭�g���ݒ�̃u�����f�B���O�X�e�[�g�̔z��

	D_ID3D11ShaderResourceView *			PSSetShaderResourceView[ D_D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT ] ;		// ���݃f�o�C�X�ɐݒ肳��Ă���s�N�Z���V�F�[�_�[���\�[�X�r���[

	RECT									ScissorRect ;									// �V�U�[��`
	int										AlphaTestEnable ;								// �A���t�@�e�X�g���g�p���邩�ǂ���
	int										AlphaTestCmpMode ;								// �A���t�@�e�X�g�Ŏg�p�����r���[�h( DX_CMP_NEVER �Ȃ� )
	int										AlphaTestRef ;									// �A���t�@�e�X�g�Ŏg�p����Q�ƒl
	int										AlphaTestModeShaderIndex ;						// �A���t�@�e�X�g�̐ݒ�l�ɑΉ�����s�N�Z���V�F�[�_�[�̃A���t�@�e�X�g���[�h( DIRECT3D11_PS_ALPHATEST_CMP_GREATER �Ȃ� )

	D_D3D11_PRIMITIVE_TOPOLOGY				PrimitiveTopology ;								// �f�o�C�X�ɐݒ肳��Ă���v���~�e�B�u�^�C�v

	D_ID3D11Buffer *						SetPixelShaderConstantBuffer[  D_D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT ] ;	// ���� ID3D11DeviceContext �Ƀs�N�Z���V�F�[�_�[�p�ɃZ�b�g���Ă���萔�o�b�t�@
	D_ID3D11Buffer *						SetVertexShaderConstantBuffer[ D_D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT ] ;	// ���� ID3D11DeviceContext �ɒ��_�V�F�[�_�[�p�ɃZ�b�g���Ă���萔�o�b�t�@

	int										SetNormalPixelShader ;							// �ʏ�`��p�s�N�Z���V�F�[�_�[���Z�b�g����Ă��邩�ǂ���
	D_ID3D11PixelShader *					SetPixelShader ;								// ���� ID3D11DeviceContext �ɃZ�b�g���Ă���s�N�Z���V�F�[�_�[

	int										SetNormalVertexShader ;							// �ʏ�`��p���_�V�F�[�_�[���Z�b�g����Ă��邩�ǂ���
	D_ID3D11VertexShader *					SetVertexShader ;								// ���� ID3D11DeviceContext �ɃZ�b�g���Ă��钸�_�V�F�[�_�[

	D_ID3D11Buffer *						SetIndexBuffer ;								// ���� ID3D11DeviceContext �ɃZ�b�g���Ă���C���f�b�N�X�o�b�t�@
	D_DXGI_FORMAT							SetIndexBufferFormat ;							// ���� ID3D11DeviceContext �ɃZ�b�g���Ă���C���f�b�N�X�o�b�t�@�̃t�H�[�}�b�g

	D_ID3D11InputLayout *					SetVertexInputLayout ;							// ���� ID3D11DeviceContext �ɃZ�b�g���Ă�����̓��C�A�E�g

	D_ID3D11Buffer *						SetVertexBuffer ;								// ���� ID3D11DeviceContext �ɃZ�b�g���Ă��钸�_�o�b�t�@
	int										SetVertexBufferStride ;							// ���� ID3D11DeviceContext �ɃZ�b�g���Ă��钸�_�o�b�t�@�̃X�g���C�h

	D_ID3D11Texture2D *						TargetTexture2D[ DX_RENDERTARGET_COUNT ] ;		// �`��Ώۂ̃e�N�X�`��
	D_D3D11_TEXTURE2D_DESC					TargetTexture2DDesc[ DX_RENDERTARGET_COUNT ] ;	// �`��Ώۂ̃e�N�X�`���̏��
	D_ID3D11RenderTargetView *				TargetRTV[ DX_RENDERTARGET_COUNT ] ;			// �`��Ώۂ̃����_�����O�^�[�Q�b�g�r���[

	D_ID3D11Texture2D *						TargetDepthTexture2D ;							// �`��Ώۂ̐[�x�e�N�X�`��
	D_D3D11_TEXTURE2D_DESC					TargetDepthTexture2DDesc ;						// �`��Ώۂ̐[�x�e�N�X�`���̏��
	D_ID3D11DepthStencilView *				TargetDepthDSV ;								// �`��Ώۂ̐[�x�X�e���V���r���[

	int										TextureAddressTransformMatrixUse ;				// �e�N�X�`�����W�ϊ��s����g�p���邩�ǂ���( TRUE:�g�p����  FALSE:�g�p���Ȃ� )
	MATRIX									TextureAddressTransformMatrix ;					// �e�N�X�`�����W�ϊ��s��
} ;


// D3D11Device �̕`�揈���Ɋւ����̍\����
struct GRAPHICS_HARDDATA_DIRECT3D11_DRAWINFO
{
	BYTE						VertexBufferTemp[ D3D11_VERTEXBUFFER_SIZE ] ;	// �V�X�e�����������̒��_�o�b�t�@
	BYTE *						VertexBufferAddr ;								// �V�X�e�����������̒��_�o�b�t�@
	BYTE *						VertexBufferNextAddr ;							// ���̃f�[�^���i�[���ׂ����_�o�b�t�@���̃A�h���X
	D_ID3D11Buffer *			VertexBuffer ;									// �`��p ID3D11Buffer
	int							VertexNum ; 									// ���_�o�b�t�@�Ɋi�[����Ă��钸�_�̐�
	int							Use3DVertex ;									// �R�c���_���g�p���Ă��邩�ǂ���( 1:�g�p���Ă���  0:�g�p���Ă��Ȃ� )
	int							VertexType ;									// �g�p���钸�_�^�C�v( VERTEXTYPE_TEX �� )
	D_D3D11_PRIMITIVE_TOPOLOGY	PrimitiveType ;									// ���_�o�b�t�@�Ɋi�[����Ă��钸�_�f�[�^�̃v���~�e�B�u�^�C�v

	DWORD						DiffuseColor ;									// �f�B�t���[�Y�J���[

	int							BeginSceneFlag ;								// Graphics_D3D11_BeginScene �����s���Ă��邩�ǂ���
	int							BlendMaxNotDrawFlag ;							// �u�����h�l���ő�ł��邱�Ƃɂ��`����s��Ȃ����ǂ����A�t���O

	D_ID3D11Buffer *			CommonVertexBuffer ;							// �ėp�`��p���_�o�b�t�@
	int							CommonVertexBufferSize ;						// �ėp�`��p���_�o�b�t�@�̃T�C�Y
	D_ID3D11Buffer *			CommonIndexBuffer ;								// �ėp�`��p�C���f�b�N�X�o�b�t�@
	int							CommonIndexBufferSize ;							// �ėp�`��p�C���f�b�N�X�o�b�t�@�̃T�C�Y
} ;

// D3D11Device �̏o�͐�E�C���h�E�������̏��̍\����
struct GRAPHICS_HARDDATA_DIRECT3D11_OUTPUTWINDOWINFO
{
	int								UseFlag ;								// �z����g�p���Ă��邩( TRUE:�g�p���Ă���  FALSE:�g�p���Ă��Ȃ� )

	D_IDXGISwapChain *				DXGISwapChain ;							// DXGISwapChain
	D_DXGI_SWAP_CHAIN_DESC			DXGISwapChainDesc ;						// DXGISwapChain���
	D_IDXGIOutput *					DXGIOutput ;							// DXGIOutput

	D_ID3D11Texture2D *				BufferTexture2D ;						// �X���b�v�`�F�C���̃o�b�N�o�b�t�@�e�N�X�`��
	D_D3D11_TEXTURE2D_DESC			BufferTexture2DDesc ;					// �X���b�v�`�F�C���̃o�b�N�o�b�t�@�e�N�X�`�����
	D_ID3D11ShaderResourceView *	BufferSRV ;								// �X���b�v�`�F�C���̃o�b�N�o�b�t�@�V�F�[�_�[���\�[�X�r���[
	D_ID3D11RenderTargetView *		BufferRTV ;								// �X���b�v�`�F�C���̃o�b�N�o�b�t�@�����_�[�^�[�Q�b�g�r���[
} ;


// D3D11Device �̉�ʊ֌W�̏��̍\����
struct GRAPHICS_HARDDATA_DIRECT3D11_SCREENINFO
{
	int												OutputWindowNum ;									// �o�͐�E�C���h�E�̐�
	GRAPHICS_HARDDATA_DIRECT3D11_OUTPUTWINDOWINFO	OutputWindowInfo[ DX_D3D11_MAX_OUTPUTWINDOW ] ;		// �o�͐�E�C���h�E�̏��
	int												TargetOutputWindow ;								// ���݂̏o�͐�E�C���h�E�̔ԍ�
	int												FullscreenSatte ;									// �t���X�N���[�����[�h�ɂ��Ă��邩( TRUE:���Ă���  FALSE:���Ă��Ȃ� )
//	D_ID3D11Texture2D *				BackBufferTexture2D ;
//	D_ID3D11ShaderResourceView *	BackBufferSRV ;
//	D_ID3D11RenderTargetView *		BackBufferRTV ;
//	D_D3D11_TEXTURE2D_DESC			BackBufferTexture2DDesc ;

	int								SubBackBufferTextureSizeX ;				// �T�u�o�b�N�o�b�t�@�e�N�X�`���̉���
	int								SubBackBufferTextureSizeY ;				// �T�u�o�b�N�o�b�t�@�e�N�X�`���̏c��
	D_ID3D11Texture2D *				SubBackBufferTexture2D ;				// ScreenCopy �� GetDrawScreen ���������邽�߂Ɏg�p����`��\�e�N�X�`��
	D_ID3D11RenderTargetView *		SubBackBufferRTV ;						// ScreenCopy �� GetDrawScreen ���������邽�߂Ɏg�p����`��\�e�N�X�`���p�̃����_�[�^�[�Q�b�g�r���[
	D_ID3D11ShaderResourceView *	SubBackBufferSRV ;						// ScreenCopy �� GetDrawScreen ���������邽�߂Ɏg�p����`��\�e�N�X�`���p�̃V�F�[�_�[���\�[�X�r���[

	D_ID3D11Texture2D *				DepthBufferTexture2D ;
	D_ID3D11DepthStencilView *		DepthBufferDSV ;
//	D_ID3D11ShaderResourceView *	DepthBufferSRV ;
	int								DepthBufferSizeX ;						// �[�x�o�b�t�@�T�[�t�F�X�̕�
	int								DepthBufferSizeY ;						// �[�x�o�b�t�@�T�[�t�F�X�̍���
	int								DepthBufferBitDepth ;					// �[�x�o�b�t�@�T�[�t�F�X�̃r�b�g�[�x

	D_ID3D11Texture2D *				DrawScreenBufferLockSMTexture ;			// �`��Ώۃo�b�t�@�����b�N�����ۂɎg�p����e�N�X�`���̃|�C���^
} ;


// D3D11Device �̕`��ݒ�Ɋւ����̍\����
struct GRAPHICS_HARDDATA_DIRECT3D11_DRAWSETTING
{
	int								DrawPrepParamFlag ;						// �O�� Graphics_D3D11_DrawPreparation �ɓ������Ƃ��� ParamFlag �p�����[�^
	int								DrawPrepAlwaysFlag ;					// �K�� Graphics_D3D11_DrawPreparation ���s�����ǂ����̃t���O

	int								AlphaChannelValidFlag ;					// ���`�����l���L���t���O
	int								AlphaTestValidFlag ;					// ���e�X�g�L���t���O( Func �͕K�� D_D3DCMP_GREATEREQUAL )
	int								AlphaTestMode ;							// �A���t�@�e�X�g���[�h
	int								AlphaTestParam ;						// �A���t�@�e�X�g�p�����[�^
	int								BlendMode ;								// �u�����h���[�h
	int								ChangeBlendParamFlag ;					// �u�����h�ݒ�Ɋւ�镔���̕ύX�����������A�t���O
	int								ChangeTextureFlag ;						// �e�N�X�`�����ύX���ꂽ���A�t���O
	int								IgnoreGraphColorFlag ;					// �`�悷��摜�̐F�����𖳎����邩�ǂ����̃t���O
//	int								UseDiffuseRGBColor ;					// �f�B�t���[�Y�J���[�̂q�f�a�l���g�p���邩�A�t���O
//	int								UseDiffuseAlphaColor ;					// �f�B�t���[�Y�J���[�̃��l���g�p���邩�A�t���O
	int								BlendGraphType ;						// �u�����h�摜�^�C�v
	int								BlendGraphFadeRatio ;					// �u�����h�摜�̃t�F�[�h�p�����[�^
	int								BlendGraphBorderParam ;					// �u�����h�摜�̋��E�p�����[�^(�O(�u�����h�摜�̉e���O)�@���@(�u�����h�摜�̉e�����Ȃ�)�@���@�P�Q�W(�u�����h�摜�̉e���P�O�O��)�@���@(�u�����h�摜�̉e���𒴂��Ĕ�`�敔����������)�@���Q�T�T(�S���`�悳��Ȃ�) )
	int								BlendGraphBorderRange ;					// �u�����h�摜�̋��E��(�O�`�Q�T�T�@�����`�L���@�������S�i�K)
	float							BlendTextureWidth ;						// �u�����h�e�N�X�`���̕�
	float							BlendTextureHeight ;					// �u�����h�e�N�X�`���̍���
	float							InvBlendTextureWidth ;					// �u�����h�e�N�X�`���̕��̋t��
	float							InvBlendTextureHeight ;					// �u�����h�e�N�X�`���̍����̋t��

	D_ID3D11Texture2D *				RenderTexture ;							// �`�掞�Ɏg�p����e�N�X�`���[
	D_ID3D11ShaderResourceView *	RenderTextureSRV ;						// �`�掞�Ɏg�p����e�N�X�`���[�̃V�F�[�_�[���\�[�X�r���[
	D_ID3D11Texture2D *				BlendTexture ;							// �`�掞�ɕ`��e�N�X�`���[�ƃu�����h���郿�`�����l���e�N�X�`���[
	D_ID3D11ShaderResourceView *	BlendTextureSRV ;						// �`�掞�ɕ`��e�N�X�`���[�ƃu�����h���郿�`�����l���e�N�X�`���[�̃V�F�[�_�[���\�[�X�r���[

	int								CancelSettingEqualCheck ;				// ���̃t���O�������Ă���ꍇ�͌��݂̐ݒ�ƕύX��̐ݒ肪�������ǂ����Ɋւ�炸�A�K���ύX���������s����
} ;


// D3D11Device �֌W�̐ݒ���̍\����
struct GRAPHICS_HARDDATA_DIRECT3D11_DEVICE_SETTING
{
	void					( *DeviceRestoreCallbackFunction)( void *Data ) ;	// �f�o�C�X���X�g���畜�A�����Ƃ��ɌĂԊ֐�
	void					  *DeviceRestoreCallbackData ;						// �f�o�C�X���X�g���畜�A�����Ƃ��ɌĂԊ֐��ɓn���|�C���^

	void					( *DeviceLostCallbackFunction)( void *Data ) ;		// �f�o�C�X���X�g���畜�A����O�ɌĂԊ֐�
	void					  *DeviceLostCallbackData ;							// �f�o�C�X���X�g���畜�A����O�ɌĂԊ֐��ɓn���|�C���^
} ;


// D3D11Device �̔\�͏��\����
struct GRAPHICS_HARDDATA_DIRECT3D11_DEVICE_CAPS
{
	D_DXGI_FORMAT					ScreenFormat ;										// ��ʃJ���[�t�H�[�}�b�g
	D_DXGI_FORMAT					TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_NUM ] ;	// �e�N�X�`���t�H�[�}�b�g
	D_DXGI_FORMAT					MaskColorFormat ;									// �}�X�N�J���[�o�b�t�@�p�t�H�[�}�b�g
	D_DXGI_FORMAT					MaskAlphaFormat ;									// �}�X�N�A���t�@�`�����l���p�t�H�[�}�b�g
	D_DXGI_FORMAT					DepthBufferTexture2DFormat[ ZBUFFER_FORMAT_NUM ] ;		// �[�x�o�b�t�@�pTexture2D�t�H�[�}�b�g
	D_DXGI_FORMAT					DepthBufferDepthStencilFormat[ ZBUFFER_FORMAT_NUM ] ;	// �[�x�o�b�t�@�pDepthStencil�t�H�[�}�b�g
} ;


// Direct3D11 ���g�����`�揈���Ŏg�p����f�[�^�̓��� D3D11Device �̃��Z�b�g��
// ��������������K�v������AD3D11Device �ɖ��ڂɊւ��������Z�߂��\����
struct GRAPHICS_HARDDATA_DIRECT3D11_DEVICE
{
	GRAPHICS_HARDDATA_DIRECT3D11_DEVICE_CAPS		Caps ;					// �f�o�C�X�\�͏��\����		

	GRAPHICS_HARDDATA_DIRECT3D11_DEVICE_SETTING		Setting ;				// �f�o�C�X�֌W�̐ݒ����Z�߂��\����

	GRAPHICS_HARDDATA_DIRECT3D11_SCREENINFO			Screen ;				// ��ʊ֌W�̏��\����

	GRAPHICS_HARDWARE_DIRECT3D11_SHADER				Shader ;				// �V�F�[�_�[��Z�߂��\����

	GRAPHICS_HARDWARE_DIRECT3D11_INPUTLAYOUT		InputLayout ;			// ���̓��C�A�E�g��Z�߂��\����

	GRAPHICS_HARDDATA_DIRECT3D11_DRAWSETTING		DrawSetting ;			// �`��ݒ�Ɋւ�����Z�߂��\����

	GRAPHICS_HARDDATA_DIRECT3D11_DRAWINFO			DrawInfo ;				// �`�揈���Ɋւ�����Z�߂��\����

	GRAPHICS_HARDDATA_DIRECT3D11_DEVICE_STATE		State ;					// D3D11Device �ɐݒ肵�Ă���l��Z�߂��\����
} ;













// �V�F�[�_�[�R�[�h��̏��\����
struct GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO
{
	void					*Binary ;								// �V�F�[�_�[�R�[�h�o�C�i���̃A�h���X
	int						Size ;									// �V�F�[�_�[�R�[�h�o�C�i���̃f�[�^�T�C�Y
} ;


// Direct3D11 �� �W���`��p�̃V�F�[�_�[�R�[�h�̌��f�[�^��Z�߂��\����
struct GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_BASE
{
	int						BaseShaderInitializeFlag ;				// �V�F�[�_�[�o�C�i���̃Z�b�g�A�b�v���������Ă��邩�ǂ����̃t���O( TRUE:�������Ă���  FALSE:�������Ă��Ȃ� )

	// ���C�g�C���f�b�N�X���X�g
	short					LightIndexList84[ 4 ][ 4 ][ 4 ][ 4 ][ 4 ][ 4 ] ;
	short					LightIndexList20[ 4 ][ 4 ][ 4 ] ;
	short					LightIndexList10[ 3 ][ 3 ][ 3 ] ;

	void					*Base2DShaderPackageImage ;				// �W���Q�c�`��p�V�F�[�_�[�p�b�P�[�W�o�C�i���C���[�W

	// �W���`��p�̕��G�ȏ������s��Ȃ����_�V�F�[�_�[
	GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO BaseSimple_VS_Code[ D3D11_VERTEX_INPUTLAYOUT_NUM ] ;

	// �Œ�@�\�p�C�v���C���݊��̃s�N�Z���V�F�[�_�[( �e�N�X�`���Ȃ� )
	// [ �������( 0:�ʏ�`��  1:��Z�`��  2:RGB���]  3:�`��P�x�S�{  4:��Z�ς݃��u�����h���[�h�̒ʏ�`��  5:��Z�ς݃��u�����h���[�h��RGB���]  6:��Z�ς݃��u�����h���[�h�̕`��P�x4�{ ) ]
	// [ �s�N�Z���V�F�[�_�[�̃A���t�@�e�X�g���[�h ]
	GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO BaseNoneTex_PS_Code[ 7 ][ DIRECT3D11_PS_ALPHATEST_NUM ] ;

	// �Œ�@�\�p�C�v���C���݊��̃s�N�Z���V�F�[�_�[( �e�N�X�`������ )
	// [ �u�����h�摜�Ƃ̃u�����h�^�C�v( 0:�Ȃ�  1:DX_BLENDGRAPHTYPE_NORMAL  2:DX_BLENDGRAPHTYPE_WIPE  3:DX_BLENDGRAPHTYPE_ALPHA ) ]
	// [ �������( 0:�ʏ�`��  1:��Z�`��  2:RGB���]  3:�`��P�x�S�{  4:��Z�ς݃��u�����h���[�h�̒ʏ�`��  5:��Z�ς݃��u�����h���[�h��RGB���]  6:��Z�ς݃��u�����h���[�h�̕`��P�x4�{ ) ]
	// [ �e�N�X�`��RGB����( 0:�������Ȃ�  1:�������� ) ]
	// [ �e�N�X�`��Alpha����( 0:�������Ȃ�  1:�������� ) ]
	// [ �s�N�Z���V�F�[�_�[�̃A���t�@�e�X�g���[�h ]
	GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO BaseUseTex_PS_Code[ 4 ][ 7 ][ 2 ][ 2 ][ DIRECT3D11_PS_ALPHATEST_NUM ] ;

	// �}�X�N�����p�̃s�N�Z���V�F�[�_�[
	GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO MaskEffect_PS_Code ;
	GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO MaskEffect_ReverseEffect_PS_Code ;
	GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO MaskEffect_UseGraphHandle_PS_Code[ 4 ] ;
	GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO MaskEffect_UseGraphHandle_ReverseEffect_PS_Code[ 4 ] ;

	// �P���]���p�V�F�[�_�[
	GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO StretchRect_VS_Code ;
	GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO StretchRectTex8_VS_Code ;
	GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO StretchRect_PS_Code ;

//	DXARC					BaseShaderBinDxa ;						// �W���`��p�V�F�[�_�[�I�u�W�F�N�g�t�@�C���c�w�`�\����
//	void					*BaseShaderBinDxaImage ;				// �W���`��p�V�F�[�_�[�I�u�W�F�N�g�t�@�C���c�w�`�̃o�C�i���C���[�W

#ifndef DX_NON_FILTER
	DXARC					FilterShaderBinDxa ;					// �t�B���^�[�V�F�[�_�[�I�u�W�F�N�g�t�@�C���c�w�`�\����
	void					*FilterShaderBinDxaImage ;				// �t�B���^�[�V�F�[�_�[�I�u�W�F�N�g�t�@�C���c�w�`�̃o�C�i���C���[�W

	void					*RGBAMixS_ShaderPackImage ;				// RGBAMix �� S �����̑g�ݍ��킹�Q�T�U�̃V�F�[�_�[�p�b�P�[�W�o�C�i���C���[�W

	// RGBAMix �� S �����̑g�ݍ��킹�Q�T�U�̃V�F�[�_�[[ R ][ G ][ B ][ A ]
	GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO RGBAMixS_PS_Code[ 4 ][ 4 ][ 4 ][ 4 ] ;
#endif // DX_NON_FILTER
} ;

// Direct3D11 �� �W���R�c�`��p�̃V�F�[�_�[�R�[�h�̌��f�[�^��Z�߂��\����
struct GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_BASE3D
{
	volatile int			Base3DShaderInitializeFlag ;			// �V�F�[�_�[�o�C�i���̃Z�b�g�A�b�v���������Ă��邩�ǂ����̃t���O( TRUE:�������Ă���  FALSE:�������Ă��Ȃ� )

	void					*Base3DShaderPackageImage ;				// �W���R�c�`��p�V�F�[�_�[�p�b�P�[�W�o�C�i���C���[�W

	// �W���R�c�`��p�s�N�Z���P�ʃ��C�e�B���O�^�C�v�̒��_�V�F�[�_�[( ���X�g�͔z��̍����� )
	// [ �V���h�E�}�b�v�̗L��( 0:���� 1:�L�� ) ]
	// /* [ ���b�V���^�C�v( 0:���̃��b�V�� 1:4�{�[�����X�L�j���O���b�V�� 2:8�{�[�����X�L�j���O���b�V�� ) ] */
	// /* [ �o���v�}�b�v�̗L��( 0:���� 1:�L�� ) ] */
	// [ �t�H�O�^�C�v ]
	GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO Base3D_PixelLighting_VS_Code[ 2 ]/*[ 3 ]*//*[ 2 ]*/[ 4 ] ;

	// �W���R�c�`��p�s�N�Z���P�ʃ��C�e�B���O�^�C�v�̃s�N�Z���V�F�[�_�[( ���X�g�͔z��̍����� )
	// [ �V���h�E�}�b�v�̗L��( 0:���� 1:�L�� ) ]
	// /*[ �}���`�e�N�X�`���̃u�����h���[�h�AMV1_LAYERBLEND_TYPE_TRANSLUCENT �Ȃǂ� +1 �������� ( 0:�}���`�e�N�X�`������  1:���u�����h  2:���Z�u�����h  3:��Z�u�����h  4:��Z�u�����h�~2 ) ]*/
	// /*[ �X�y�L�����}�b�v ( 0:���� 1:���� ) ]*/
	// /*[ �o���v�}�b�v ( 0:�Ȃ� 1:���� ) ]*/
	// [ ���C�g�C���f�b�N�X ]
	// /*[ �X�y�L�����̗L��( 0:���� 1:�L�� ) ]*/
	// [ �������( 0:�ʏ�`��  1:��Z�`��  2:RGB���]  3:�`��P�x�S�{  4:��Z�ς݃��u�����h���[�h�̒ʏ�`��  5:��Z�ς݃��u�����h���[�h��RGB���]  6:��Z�ς݃��u�����h���[�h�̕`��P�x4�{ ) ]
	// [ �s�N�Z���V�F�[�_�[�̃A���t�@�e�X�g���[�h ]
	GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO Base3D_PixelLighting_Normal_PS_Code[ 2 ]/*[ 5 ]*//*[ 2 ]*//*[ 2 ]*/[ 84 ]/*[ 2 ]*/[ 7 ][ DIRECT3D11_PS_ALPHATEST_NUM ] ;



	// �W���R�c�`��p�̃V���h�E�}�b�v�ւ̃����_�����O�p���_�V�F�[�_�[
	// /*[ ���b�V���^�C�v( 0:���̃��b�V�� 1:4�{�[�����X�L�j���O���b�V��  2:8�{�[�����X�L�j���O���b�V�� ) ]*/
	// [ �o���v�}�b�v�̗L��( 0:���� 1:�L�� ) ]
	GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO Base3D_ShadowMap_VS_Code/*[ 3 ]*/[ 2 ] ;

	// �W���R�c�`��p�̃��C�e�B���O�Ȃ����_�V�F�[�_�[
	// /*[ ���b�V���^�C�v( 0:���̃��b�V�� 1:4�{�[�����X�L�j���O���b�V��  2:8�{�[�����X�L�j���O���b�V�� ) ]*/
	// [ �t�H�O�^�C�v ]
	GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO Base3D_NoLighting_VS_Code/*[ 3 ]*/[ 4 ] ;

	// �W���R�c�`��p�̒��_�P�ʃ��C�e�B���O���_�V�F�[�_�[
	// [ �V���h�E�}�b�v�̗L��( 0:���� 1:�L�� ) ]
	// /*[ ���b�V���^�C�v( 0:���̃��b�V�� 1:4�{�[�����X�L�j���O���b�V��  2:8�{�[�����X�L�j���O���b�V�� ) ]*/
	// /*[ �o���v�}�b�v�̗L��( 0:���� 1:�L�� ) ]*/
	// [ �t�H�O�^�C�v ]
	// [ ���C�g�C���f�b�N�X ]
	// /*[ �X�y�L�����̗L��( 0:���� 1:�L�� ) ]*/
	GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO Base3D_VertexLighting_VS_Code[ 2 ]/*[ 3 ]*//*[ 2 ]*/[ 4 ][ 20 ]/*[ 2 ]*/ ;

	// �W���R�c�`��p�̃V���h�E�}�b�v�ւ̃����_�����O�p�s�N�Z���V�F�[�_�[
	// [ �s�N�Z���V�F�[�_�[�̃A���t�@�e�X�g���[�h ]
	GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO Base3D_ShadowMap_Normal_PS_Code[ DIRECT3D11_PS_ALPHATEST_NUM ] ;

	// �W���R�c�`��p�̃��C�e�B���O�Ȃ��s�N�Z���V�F�[�_�[
	// /*[ �}���`�e�N�X�`���̃u�����h���[�h�AMV1_LAYERBLEND_TYPE_TRANSLUCENT �Ȃǂ� +1 �������� ( 0:�}���`�e�N�X�`������  1:���u�����h  2:���Z�u�����h  3:��Z�u�����h  4:��Z�u�����h�~2 ) ]*/
	// [ �������( 0:�ʏ�`��  1:��Z�`��  2:RGB���]  3:�`��P�x�S�{  4:��Z�ς݃��u�����h���[�h�̒ʏ�`��  5:��Z�ς݃��u�����h���[�h��RGB���]  6:��Z�ς݃��u�����h���[�h�̕`��P�x4�{ ) ]
	// [ �s�N�Z���V�F�[�_�[�̃A���t�@�e�X�g���[�h ]
	GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO Base3D_NoLighting_Normal_PS_Code/*[ 5 ]*/[ 7 ][ DIRECT3D11_PS_ALPHATEST_NUM ] ;

	// �W���R�c�`��p�̒��_�P�ʃ��C�e�B���O�s�N�Z���V�F�[�_�[
	// [ �V���h�E�}�b�v�̗L��( 0:���� 1:�L�� ) ]
	// /* [ �}���`�e�N�X�`���̃u�����h���[�h�AMV1_LAYERBLEND_TYPE_TRANSLUCENT �Ȃǂ� +1 �������� ( 0:�}���`�e�N�X�`������  1:���u�����h  2:���Z�u�����h  3:��Z�u�����h  4:��Z�u�����h�~2 ) ] */
	// /* [ �X�y�L�����}�b�v ( 0:���� 1:���� ) ] */
	// /* [ �o���v�}�b�v ( 0:�Ȃ� 1:���� ) ] */
	// [ ���C�g�C���f�b�N�X ]
	// /*[ �X�y�L�����̗L��( 0:���� 1:�L�� ) ]*/
	// [ �������( 0:�ʏ�`��  1:��Z�`��  2:RGB���]  3:�`��P�x�S�{  4:��Z�ς݃��u�����h���[�h�̒ʏ�`��  5:��Z�ς݃��u�����h���[�h��RGB���]  6:��Z�ς݃��u�����h���[�h�̕`��P�x4�{ ) ]
	// [ �s�N�Z���V�F�[�_�[�̃A���t�@�e�X�g���[�h ]
	GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO Base3D_VertexLighting_Normal_PS_Code[ 2 ]/*[ 5 ]*//*[ 2 ]*//*[ 2 ]*/[ 10 ]/*[ 2 ]*/[ 7 ][ DIRECT3D11_PS_ALPHATEST_NUM ] ;
} ;

#ifndef DX_NON_MODEL

// Direct3D11 �� ���f���`��p�̃V�F�[�_�[�R�[�h�̌��f�[�^��Z�߂��\����
struct GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_MODEL
{
	volatile int			ModelShaderInitializeFlag ;				// ���f���p�̃V�F�[�_�[�o�C�i���̃Z�b�g�A�b�v���������Ă��邩�ǂ����̃t���O( TRUE:�������Ă���  FALSE:�������Ă��Ȃ� )

	void					*ModelShaderPackImage ;					// �R�c���f���p�V�F�[�_�[�p�b�P�[�W�o�C�i���C���[�W

	// ���f���`��p�s�N�Z���P�ʃ��C�e�B���O�^�C�v�̒��_�V�F�[�_�[( ���X�g�͔z��̍����� )
	// [ �V���h�E�}�b�v�̗L��( 0:���� 1:�L�� ) ]
	// [ ���b�V���^�C�v( 0:���̃��b�V�� 1:4�{�[�����X�L�j���O���b�V�� 2:8�{�[�����X�L�j���O���b�V�� ) ]
	// [ �o���v�}�b�v�̗L��( 0:���� 1:�L�� ) ]
	// [ �t�H�O�^�C�v ]
	GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO MV1_PixelLighting_VS_Code[ 2 ][ 3 ][ 2 ][ 4 ] ;

	// ���f���`��p�s�N�Z���P�ʃ��C�e�B���O�^�C�v�̃g�D�[���^�C�v�P�p�s�N�Z���V�F�[�_�[( ���X�g�͔z��̍����� )
	// [ �V���h�E�}�b�v�̗L��( 0:���� 1:�L�� ) ]
	// [ �g�D�[�������_�����O�̃f�B�t���[�Y�O���f�[�V�����̃u�����h�^�C�v( 0:MV1_LAYERBLEND_TYPE_TRANSLUCENT  1:MV1_LAYERBLEND_TYPE_MODULATE ) ]
	// [ �g�D�[�������_�����O�̃X�y�L�����O���f�[�V�����̃u�����h�^�C�v( 0:MV1_LAYERBLEND_TYPE_TRANSLUCENT  1:MV1_LAYERBLEND_TYPE_ADDITIVE ) ]
	// [ �o���v�}�b�v ( 0:�Ȃ� 1:���� ) ]
	// [ �L���ȃ��C�g�̐� - 1 ]
	// [ �s�N�Z���V�F�[�_�[�̃A���t�@�e�X�g���[�h ]
	GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO MV1_PixelLighting_ToonType1_PS_Code[ 2 ][ 2 ][ 2 ][ 2 ][ DX_D3D11_COMMON_CONST_LIGHT_NUM ][ DIRECT3D11_PS_ALPHATEST_NUM ] ;

	// ���f���`��p�s�N�Z���P�ʃ��C�e�B���O�^�C�v�̃g�D�[���^�C�v�Q�p�s�N�Z���V�F�[�_�[( ���X�g�͔z��̍����� )
	// [ �V���h�E�}�b�v�̗L��( 0:���� 1:�L�� ) ]
	// [ �g�D�[�������_�����O�̃X�t�B�A�}�b�v�̗L���ƃu�����h�^�C�v( 0:�X�t�B�A�}�b�v�͖���   1:MV1_LAYERBLEND_TYPE_MODULATE  2:MV1_LAYERBLEND_TYPE_ADDITIVE ) ]
	// [ �L���ȃ��C�g�̐� - 1 ]
	// [ �s�N�Z���V�F�[�_�[�̃A���t�@�e�X�g���[�h ]
	GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO MV1_PixelLighting_ToonType2_PS_Code[ 2 ][ 3 ][ DX_D3D11_COMMON_CONST_LIGHT_NUM ][ DIRECT3D11_PS_ALPHATEST_NUM ] ;

	// ���f���`��p�s�N�Z���P�ʃ��C�e�B���O�^�C�v�̃s�N�Z���V�F�[�_�[( ���X�g�͔z��̍����� )
	// [ �V���h�E�}�b�v�̗L��( 0:���� 1:�L�� ) ]
	// [ �}���`�e�N�X�`���̃u�����h���[�h�AMV1_LAYERBLEND_TYPE_TRANSLUCENT �Ȃǂ� +1 �������� ( 0:�}���`�e�N�X�`������  1:���u�����h  2:���Z�u�����h  3:��Z�u�����h  4:��Z�u�����h�~2 ) ]
	// [ �o���v�}�b�v ( 0:�Ȃ� 1:���� ) ]
	// [ �L���ȃ��C�g�̐� - 1 ]
	// [ �s�N�Z���V�F�[�_�[�̃A���t�@�e�X�g���[�h ]
	GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO MV1_PixelLighting_Normal_PS_Code[ 2 ][ 5 ][ 2 ][ DX_D3D11_COMMON_CONST_LIGHT_NUM ][ DIRECT3D11_PS_ALPHATEST_NUM ] ;





	// ���f���`��p�̃V���h�E�}�b�v�ւ̃g�D�[�������_�����O�֊s���`��p���_�V�F�[�_�[
	// [ ���b�V���^�C�v( 0:���̃��b�V�� 1:4�{�[�����X�L�j���O���b�V��  2:8�{�[�����X�L�j���O���b�V�� ) ]
	// [ �o���v�}�b�v�̗L��( 0:���� 1:�L�� ) ]
	GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO MV1_ToonOutLine_ShadowMap_VS_Code[ 3 ][ 2 ] ;

	// ���f���`��p�̃g�D�[�������_�����O�̗֊s���`��p���_�V�F�[�_�[
	// [ ���b�V���^�C�v( 0:���̃��b�V�� 1:4�{�[�����X�L�j���O���b�V��  2:8�{�[�����X�L�j���O���b�V�� ) ]
	// [ �o���v�}�b�v�̗L��( 0:���� 1:�L�� ) ]
	// [ �t�H�O�^�C�v ]
	GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO MV1_ToonOutLine_VS_Code[ 3 ][ 2 ][ 4 ] ;

	// ���f���`��p�̃V���h�E�}�b�v�ւ̃����_�����O�p���_�V�F�[�_�[
	// [ ���b�V���^�C�v( 0:���̃��b�V�� 1:4�{�[�����X�L�j���O���b�V��  2:8�{�[�����X�L�j���O���b�V�� ) ]
	// [ �o���v�}�b�v�̗L��( 0:���� 1:�L�� ) ]
	GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO MV1_ShadowMap_VS_Code[ 3 ][ 2 ] ;

	// ���f���`��p�̃��C�e�B���O�Ȃ����_�V�F�[�_�[
	// [ ���b�V���^�C�v( 0:���̃��b�V�� 1:4�{�[�����X�L�j���O���b�V��  2:8�{�[�����X�L�j���O���b�V�� ) ]
	// [ �o���v�}�b�v�̗L��( 0:���� 1:�L�� ) ]
	// [ �t�H�O�^�C�v ]
	GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO MV1_NoLighting_VS_Code[ 3 ][ 2 ][ 4 ] ;

	// ���f���`��p�̒��_�P�ʃ��C�e�B���O���蒸�_�V�F�[�_�[
	// [ �V���h�E�}�b�v�̗L��( 0:���� 1:�L�� ) ]
	// [ ���b�V���^�C�v( 0:���̃��b�V�� 1:4�{�[�����X�L�j���O���b�V��  2:8�{�[�����X�L�j���O���b�V�� ) ]
	// [ �o���v�}�b�v�̗L��( 0:���� 1:�L�� ) ]
	// [ �t�H�O�^�C�v ]
	// [ ���C�g�C���f�b�N�X ]
	GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO MV1_VertexLighting_VS_Code[ 2 ][ 3 ][ 2 ][ 4 ][ 20 ] ;

	// ���f���`��p�̃V���h�E�}�b�v�ւ̃g�D�[�������_�����O�p�s�N�Z���V�F�[�_�[
	// [ �g�D�[�������_�����O�^�C�v( 0:�g�D�[�������_�����O type 1   1:�g�D�[�������_�����O type 2 ) ]
	// [ �g�D�[�������_�����O�̃X�t�B�A�}�b�v�̗L���ƃu�����h�^�C�v( 0:�X�t�B�A�}�b�v�͖���   1:MV1_LAYERBLEND_TYPE_MODULATE  2:MV1_LAYERBLEND_TYPE_ADDITIVE ) ]
	// [ �g�D�[�������_�����O�̃f�B�t���[�Y�O���f�[�V�����̃u�����h�^�C�v( 0:MV1_LAYERBLEND_TYPE_TRANSLUCENT  1:MV1_LAYERBLEND_TYPE_MODULATE ) ]
	// [ �s�N�Z���V�F�[�_�[�̃A���t�@�e�X�g���[�h ]
	GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO MV1_ShadowMap_Toon_PS_Code[ 2 ][ 3 ][ 2 ][ DIRECT3D11_PS_ALPHATEST_NUM ] ;

	// ���f���`��p�̃V���h�E�}�b�v�ւ̃����_�����O�p�s�N�Z���V�F�[�_�[
	// [ �s�N�Z���V�F�[�_�[�̃A���t�@�e�X�g���[�h ]
	GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO MV1_ShadowMap_Normal_PS_Code[ DIRECT3D11_PS_ALPHATEST_NUM ] ;

	// ���f���`��p�̃��C�e�B���O�Ȃ��g�D�[���p�s�N�Z���V�F�[�_�[( ���X�g�͔z��̍����� )
	// [ �g�D�[�������_�����O�^�C�v( 0:�g�D�[�������_�����O type 1   1:�g�D�[�������_�����O type 2 ) ]
	// [ �g�D�[�������_�����O�̃f�B�t���[�Y�O���f�[�V�����̃u�����h�^�C�v( 0:MV1_LAYERBLEND_TYPE_TRANSLUCENT or �g�D�[�������_�����O�ł͂Ȃ�  1:MV1_LAYERBLEND_TYPE_MODULATE ) ]
	// [ �s�N�Z���V�F�[�_�[�̃A���t�@�e�X�g���[�h ]
	GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO MV1_NoLighting_Toon_PS_Code[ 2 ][ 2 ][ DIRECT3D11_PS_ALPHATEST_NUM ] ;

	// ���f���`��p�̃��C�e�B���O�Ȃ��s�N�Z���V�F�[�_�[
	// [ �}���`�e�N�X�`���̃u�����h���[�h�AMV1_LAYERBLEND_TYPE_TRANSLUCENT �Ȃǂ� +1 �������� ( 0:�}���`�e�N�X�`������  1:���u�����h  2:���Z�u�����h  3:��Z�u�����h  4:��Z�u�����h�~2 ) ]
	// [ �s�N�Z���V�F�[�_�[�̃A���t�@�e�X�g���[�h ]
	GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO MV1_NoLighting_Normal_PS_Code[ 5 ][ DIRECT3D11_PS_ALPHATEST_NUM ] ;

	// ���f���`��p�̒��_�P�ʃ��C�e�B���O����g�D�[���^�C�v�P�p�s�N�Z���V�F�[�_�[
	// [ �V���h�E�}�b�v�̗L��( 0:���� 1:�L�� ) ]
	// [ �g�D�[�������_�����O�̃f�B�t���[�Y�O���f�[�V�����̃u�����h�^�C�v( 0:MV1_LAYERBLEND_TYPE_TRANSLUCENT  1:MV1_LAYERBLEND_TYPE_MODULATE ) ]
	// [ �g�D�[�������_�����O�̃X�y�L�����O���f�[�V�����̃u�����h�^�C�v( 0:MV1_LAYERBLEND_TYPE_TRANSLUCENT  1:MV1_LAYERBLEND_TYPE_ADDITIVE ) ]
	// [ �o���v�}�b�v ( 0:�Ȃ� 1:���� ) ]
	// [ ���C�g�C���f�b�N�X ]
	// [ �s�N�Z���V�F�[�_�[�̃A���t�@�e�X�g���[�h ]
	GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO MV1_VertexLighting_ToonType1_PS_Code[ 2 ][ 2 ][ 2 ][ 2 ][ 10 ][ DIRECT3D11_PS_ALPHATEST_NUM ] ;

	// ���f���`��p�̒��_�P�ʃ��C�e�B���O����g�D�[���^�C�v�Q�p�s�N�Z���V�F�[�_�[
	// [ �V���h�E�}�b�v�̗L��( 0:���� 1:�L�� ) ]
	// [ �g�D�[�������_�����O�̃X�t�B�A�}�b�v�̗L���ƃu�����h�^�C�v( 0:�X�t�B�A�}�b�v�͖���   1:MV1_LAYERBLEND_TYPE_MODULATE  2:MV1_LAYERBLEND_TYPE_ADDITIVE ) ]
	// [ ���C�g�C���f�b�N�X ]
	// [ �s�N�Z���V�F�[�_�[�̃A���t�@�e�X�g���[�h ]
	GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO MV1_VertexLighting_ToonType2_PS_Code[ 2 ][ 3 ][ 10 ][ DIRECT3D11_PS_ALPHATEST_NUM ] ;

	// ���f���`��p�̒��_�P�ʃ��C�e�B���O����s�N�Z���V�F�[�_�[
	// [ �V���h�E�}�b�v�̗L��( 0:���� 1:�L�� ) ]
	// [ �}���`�e�N�X�`���̃u�����h���[�h�AMV1_LAYERBLEND_TYPE_TRANSLUCENT �Ȃǂ� +1 �������� ( 0:�}���`�e�N�X�`������  1:���u�����h  2:���Z�u�����h  3:��Z�u�����h  4:��Z�u�����h�~2 ) ]
	// [ �o���v�}�b�v ( 0:�Ȃ� 1:���� ) ]
	// [ ���C�g�C���f�b�N�X ]
	// [ �s�N�Z���V�F�[�_�[�̃A���t�@�e�X�g���[�h ]
	GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO MV1_VertexLighting_Normal_PS_Code[ 2 ][ 5 ][ 2 ][ 10 ][ DIRECT3D11_PS_ALPHATEST_NUM ] ;
} ;

#endif // DX_NON_MODEL


// �V�F�[�_�[�R�[�h�̌��f�[�^��Z�߂��\����
struct GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE
{
	GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_BASE	Base ;				// �W���`�揈���p�̃V�F�[�_�[�R�[�h��Z�߂��\����
	GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_BASE3D	Base3D ;			// �W���R�c�`�揈���p�̃V�F�[�_�[�R�[�h��Z�߂��\����

#ifndef DX_NON_MODEL
	GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_MODEL	Model ;				// ���f���`�揈���p�̃V�F�[�_�[�R�[�h��Z�߂��\����
#endif // DX_NON_MODEL
} ;

// �A�_�v�^�[��ӂ�̏��\����
struct GRAPHICS_HARDWARE_DIRECT3D11_ADAPTER_UNIT
{
	D_DXGI_ADAPTER_DESC								Desc ;						// �A�_�v�^�̏��

	int												OutputNum ;					// �o�͐�̐�
	D_DXGI_OUTPUT_DESC								OutputDesc[ DX_D3D11_MAX_OUTPUT_LISTUP ] ; // �o�͐�̏��
} ;


// �A�_�v�^�[�֌W���\����
struct GRAPHICS_HARDWARE_DIRECT3D11_ADAPTER
{
	int												ValidInfo ;					// �A�_�v�^�̏�񂪗L�����ǂ���( TRUE:�L��  FALSE:���� )
	int												InfoNum ;					// �A�_�v�^���̐�
	GRAPHICS_HARDWARE_DIRECT3D11_ADAPTER_UNIT		Info[ DX_D3D11_MAX_ADAPTER_LISTUP ] ;	// �A�_�v�^�̏��

//	int												ValidUseOutputNumber ;		// UseOutputNumber ���L�����ǂ���( TRUE:�L��  FALSE:���� )
//	int												UseOutputNumber ;			// �g�p����o�͔ԍ�
} ;


// Direct3D11 �֌W�̐ݒ���̍\����
struct GRAPHICS_HARDDATA_DIRECT3D11_SETTING
{
	D_D3D_FEATURE_LEVEL				FeatureLevel ;								// �g�p���Ă���@�\���x��
	int								UseMinFeatureLevelDirect3D11 ;				// Direct3D11 �Ŏg�p����Œ�@�\���x��
	int								UserDepthBufferSizeSet ;					// �O������[�x�o�b�t�@�T�C�Y�̎w�肪���������ǂ���
	int								UserDepthBufferBitDepthSet ;				// �O������[�x�o�b�t�@�̃r�b�g�[�x�w�肪���������ǂ���

	int								UseOldDrawModiGraphCodeFlag ;				// �ȑO�� DrawModiGraph �֐��R�[�h���g�p���邩�ǂ����̃t���O
} ;



// Direct3D11 �̃e�N�X�`�������p���̍\����
struct GRAPHICS_HARDDATA_DIRECT3D11_TEXTURE
{
	void *											CommonBuffer ;					// �e�N�X�`���摜�]���p�̋��L������
	unsigned int									CommonBufferSize ;				// �e�N�X�`���摜�]���p�̋��L�������̃T�C�Y
} ;

// Direct3D11 ���g�����O���t�B�b�N�X�������̍\����
struct GRAPHICS_HARDDATA_DIRECT3D11
{
	int												UsePixelLightingShader ;	// �s�N�Z���P�ʂŃ��C�e�B���O���s���^�C�v�̃V�F�[�_�[���g�p���邩�ǂ���( TRUE:�g�p����  FALSE:�g�p���Ȃ� )

#ifndef DX_NON_MODEL
	D_ID3D11Texture3D *								RGBtoVMaxRGBVolumeTexture ;			// RGB�J���[���P�x���ő�ɂ���RGB�l�ɕϊ����邽�߂̃{�����[���e�N�X�`��
	D_ID3D11ShaderResourceView *					RGBtoVMaxRGBVolumeTextureSRV ;		// RGB�J���[���P�x���ő�ɂ���RGB�l�ɕϊ����邽�߂̃{�����[���e�N�X�`���̃V�F�[�_�[���\�[�X�r���[
#endif // DX_NON_MODEL

	GRAPHICS_HARDDATA_DIRECT3D11_SETTING			Setting ;					// Direct3D11 �֌W�̐ݒ���

	GRAPHICS_HARDDATA_DIRECT3D11_DEVICE				Device ;					// D3D11Device �ɖ��ڂɊ֘A�������

	GRAPHICS_HARDWARE_DIRECT3D11_ADAPTER			Adapter ;					// �A�_�v�^�[�֌W���

	GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE			ShaderCode ;				// D_ID3D11PixelShader �� D_ID3D11VertexShader �̌��ƂȂ�V�F�[�_�[�R�[�h�f�[�^��Z�߂��\����

	GRAPHICS_HARDDATA_DIRECT3D11_TEXTURE			Texture ;					// Direct3D11 �̃e�N�X�`�������p���̍\����
} ;


// �������ϐ��錾 --------------------------------------------------------------

extern GRAPHICS_HARDDATA_DIRECT3D11 GraphicsHardDataDirect3D11 ;

// �֐��v���g�^�C�v�錾-----------------------------------------------------------

// Direct3D11 �̏������֌W
extern	int		Graphics_D3D11_Initialize( void ) ;								// Direct3D11 ���g�p�����O���t�B�b�N�X�����̏��������s��( 0:���� -1:���s )
extern	int		Graphics_D3D11_Terminate( void ) ;								// Direct3D11 ���g�p�����O���t�B�b�N�X�����̌�n�����s��

extern	int		Graphics_D3D11_ReleaseObjectAll( void ) ;						// ���ׂĂ� Direct3D11 �n�I�u�W�F�N�g���������
extern	int		Graphics_D3D11_CreateObjectAll( void ) ;						// ���ׂĂ� Direct3D11 �n�I�u�W�F�N�g���쐬����










// �V�F�[�_�[�R�[�h�֌W
extern	int		Graphics_D3D11_ShaderCode_Base_Initialize( void ) ;				// Direct3D11 �̕W���`��p�̃V�F�[�_�[�R�[�h�̏��������s��
extern	int		Graphics_D3D11_ShaderCode_Base_Terminate( void ) ;				// Direct3D11 �̕W���`��p�̃V�F�[�_�[�R�[�h�̌�n�����s��

extern	int		Graphics_D3D11_ShaderCode_Base3D_Initialize( void ) ;			// Direct3D11 �̕W���R�c�`��p�̃V�F�[�_�[�R�[�h�̏��������s��
extern	int		Graphics_D3D11_ShaderCode_Base3D_Terminate( void ) ;			// Direct3D11 �̕W���R�c�`��p�̃V�F�[�_�[�R�[�h�̌�n�����s��

#ifndef DX_NON_MODEL

extern	int		Graphics_D3D11_ShaderCode_Model_Initialize( void ) ;			// Direct3D11 �̃��f���`��p�̃V�F�[�_�[�R�[�h�̏��������s��
extern	int		Graphics_D3D11_ShaderCode_Model_Terminate( void ) ;				// Direct3D11 �̃��f���`��p�̃V�F�[�_�[�R�[�h�̌�n�����s��

#endif // DX_NON_MODEL

extern	int		Graphics_D3D11_Shader_Initialize( void ) ;						// Direct3D11 �̃V�F�[�_�[�̏��������s��
extern	int		Graphics_D3D11_Shader_Terminate( void ) ;						// Direct3D11 �̃V�F�[�_�[�̌�n��������

extern	int		Graphics_D3D11_VertexShader_Create( GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO *Code, D_ID3D11VertexShader **pDestShader, int Num ) ;		// �V�F�[�_�[�R�[�h���璸�_�V�F�[�_�[���쐬����
extern	int		Graphics_D3D11_PixelShader_Create(  GRAPHICS_HARDWARE_DIRECT3D11_SHADERCODE_INFO *Code, D_ID3D11PixelShader  **pDestShader, int Num ) ;		// �V�F�[�_�[�R�[�h����s�N�Z���V�F�[�_�[���쐬����

extern	int		Graphics_D3D11_Shader_Normal3DDraw_Setup( void ) ;				// �R�c�W���`��̎w��̒��_�p�̕`��p�V�F�[�_�[���Z�b�g�A�b�v����
extern	int		Graphics_D3D11_Shader_GetAlphaTestModeIndex( int AlphaTestEnable, int AlphaTestMode /* DX_CMP_NEVER �� */ ) ;	// �A���t�@�e�X�g�̔�r���[�h����s�N�Z���V�F�[�_�[�̃A���t�@�e�X�g���[�h( DIRECT3D11_PS_ALPHATEST_CMP_GREATER �� )���擾����










// ���_�o�b�t�@�E�C���f�b�N�X�o�b�t�@�֌W
extern	int		Graphics_D3D11_VertexBuffer_CreateObject(  VERTEXBUFFERHANDLEDATA *VertexBuffer, int Restore ) ;	// ���_�o�b�t�@�n���h���p�� ID3D11Buffer �I�u�W�F�N�g���쐬����
extern	int		Graphics_D3D11_VertexBuffer_ReleaseObject( VERTEXBUFFERHANDLEDATA *VertexBuffer ) ;					// ���_�o�b�t�@�n���h���p�� ID3D11Buffer �I�u�W�F�N�g���������
extern	int		Graphics_D3D11_IndexBuffer_CreateObject(   INDEXBUFFERHANDLEDATA *IndexBuffer,   int Restore ) ;	// �C���f�b�N�X�o�b�t�@�n���h���p�� ID3D11Buffer �I�u�W�F�N�g���쐬����
extern	int		Graphics_D3D11_IndexBuffer_ReleaseObject(  INDEXBUFFERHANDLEDATA *IndexBuffer ) ;					// �C���f�b�N�X�o�b�t�@�n���h���p�� ID3D11Buffer �I�u�W�F�N�g���������










// Direct3D11 �̉�ʊ֌W
extern	int		Graphics_D3D11_OutputWindow_Add( HWND TargetWindow, int IsMainTarget = FALSE ) ;					// �o�͐�E�C���h�E�̒ǉ����s��( -1:���s  -1�ȊO:�ΏۃE�C���h�E�p���̔z��C���f�b�N�X )
extern	int		Graphics_D3D11_OutputWindow_Sub( HWND TargetWindow, int Index = -1 ) ;								// �w��̃E�C���h�E���o�͐�E�C���h�E����O��
extern	int		Graphics_D3D11_OutputWindow_SetupBuffer( int Index ) ;												// �w��̏o�͐�E�C���h�E�p�̃o�b�N�o�b�t�@�[���Z�b�g�A�b�v����
extern	int		Graphics_D3D11_OutputWindow_ReleaseBuffer( int Index ) ;											// �w��̏o�͐�E�C���h�E�p�̃o�b�N�o�b�t�@�[�̌�n��������
extern	int		Graphics_D3D11_OutputWindow_ResizeBuffer( int Index, int NewSizeX, int NewSizeY ) ;					// �w��̏o�͐�E�C���h�E�̃o�b�N�o�b�t�@�̃T�C�Y��ύX����
extern	int		Graphics_D3D11_OutputWindow_Present( int Index, RECT *BackScreenRect, RECT *WindowClientRect ) ;	// �w��̏o�͐�E�C���h�E�̎w��̗̈�Ɏw��̃o�b�N�o�b�t�@�̗̈��]������
extern	int		Graphics_D3D11_SetupSubBackBuffer( void ) ;															// �T�u�o�b�N�o�b�t�@���g�p���鏀�����s��( 0:����  -1:���s )
extern	int		Graphics_D3D11_CreateDepthBuffer( void ) ;															// �[�x�o�b�t�@�I�u�W�F�N�g�̍쐬( 0:����  -1:���s )















// Direct3D11 �̉摜�֌W
extern COLORDATA *Graphics_D3D11_GetD3DFormatColorData( D_DXGI_FORMAT Format ) ;									// D_DXGI_FORMAT �̃t�H�[�}�b�g�ɍ��킹���J���[�t�H�[�}�b�g�����擾����

// �`���p�e�N�X�`���ƕ`��p�e�N�X�`����������Ă���ꍇ�ɁA�`��p�e�N�X�`���ɕ`���p�e�N�X�`���̓��e�𔽉f����
extern	int		Graphics_D3D11_UpdateDrawTexture( IMAGEDATA_ORIG_HARD_TEX *OrigTex, int TargetSurface, D_DXGI_FORMAT Format ) ;

// ��{�C���[�W�̃t�H�[�}�b�g�� DX_BASEIMAGE_FORMAT_NORMAL �ɕϊ�����K�v�����邩�ǂ������`�F�b�N����
// ( RequiredRgbBaseImageConvFlag �� RequiredAlphaBaseImageConvFlag �ɓ���l  TRUE:�ϊ�����K�v������  FALSE:�ϊ�����K�v�͖��� )
extern	int		Graphics_D3D11_CheckRequiredNormalImageConv_BaseImageFormat_PF(
	IMAGEDATA_ORIG *Orig,
	int             RgbBaseImageFormat,
	int            *RequiredRgbBaseImageConvFlag,
	int             AlphaBaseImageFormat = -1,
	int            *RequiredAlphaBaseImageConvFlag = NULL
) ;

extern	int		Graphics_D3D11_Texture_SetupCommonBuffer( unsigned int Size ) ;		// �e�N�X�`���摜�]���p�̋��L�������̃Z�b�g�A�b�v���s��
extern	int		Graphics_D3D11_Texture_TerminateCommonBuffer( void ) ;				// �e�N�X�`���摜�]���p�̋��L�������̌�n�����s��

extern	int		Graphics_D3D11_StretchRect_Initialize( void ) ;				// �P���]�������̏��������s��
extern	int		Graphics_D3D11_StretchRect_Terminate( void ) ;				// �P���]�������̌�n�����s��

// �����_�[�^�[�Q�b�g�e�N�X�`���Ƀe�N�X�`���̒P���]�����s��
extern	int		Graphics_D3D11_StretchRect(
	D_ID3D11Texture2D *SrcTexture,          D_ID3D11ShaderResourceView  *SrcTextureSRV,          const RECT *SrcRect  /* NULL �őS�� */,
	D_ID3D11Texture2D *DestTexture,         D_ID3D11RenderTargetView    *DestTextureRTV,         const RECT *DestRect /* NULL �őS�� */,
	D_D3D11_FILTER_TYPE FilterType = D_D3D11_FILTER_TYPE_POINT,
	int AlphaBlend = FALSE,
	D_ID3D11VertexShader *VertexShader = NULL,
	D_ID3D11PixelShader  *PixelShader  = NULL,
	D_ID3D11Texture2D *BlendTexture = NULL, D_ID3D11ShaderResourceView  *BlendTextureSRV = NULL, const RECT *BlendRect = NULL /* NULL �őS�� */,
	VERTEX_TEX8_2D *Texcoord8Vertex = NULL
) ;

// �w��̃e�N�X�`���̓��e�ɓǂݍ��݃A�N�Z�X�ł���悤�ɂ���
extern	int		Graphics_D3D11_Texture2D_Map(
	D_ID3D11Texture2D *				TargetTexture,
	D_ID3D11ShaderResourceView *	TargetTextureSRV,
	const RECT *					MapRect /* NULL �őS�� */,
	BASEIMAGE *						MapBaseImage,
	D_ID3D11Texture2D **			MapTempTexture
) ;

// �w��̃e�N�X�`���̓��e�ւ̓ǂݍ��݃A�N�Z�X���I������
extern	int		Graphics_D3D11_Texture2D_Unmap( D_ID3D11Texture2D *MapTempTexture ) ;







// D3D11Device �̏������֌W
extern	int		Graphics_D3D11_Device_Create( void ) ;																// Direct3DDevice11 �I�u�W�F�N�g�̍쐬( 0:����  -1:���s )

extern	int		Graphics_D3D11_Device_Initialize( void ) ;															// Direct3DDevice11 �֌W�̏�����
extern	int		Graphics_D3D11_Device_ReInitialize( void ) ;														// �ꎞ�I�� Direct3DDevice11 ��j�������ۂɁA�j���O�̏�Ԃ����o�����߂̊֐�






// DXGIAdapter �֌W
extern	int		EnumDXGIAdapterInfo( void ) ;																		// DXGI�I�u�W�F�N�g���̗�








// D3D11Device �X�e�[�^�X�֌W
extern	void	Graphics_D3D11_DeviceState_RefreshRenderState( void ) ;												// �`��ݒ�����t���b�V��
extern	int		Graphics_D3D11_DeviceState_SetBaseState( void ) ;													// Direct3DDevice11 �̊�{�ݒ���s��
extern	int		Graphics_D3D11_DeviceState_SetSampleFilterMode( D_D3D11_FILTER Filter, int Sampler = -1 ) ;			// �T���v���[�̃e�N�X�`���t�B���^�����O���[�h��ݒ肷��
extern	int		Graphics_D3D11_DeviceState_SetDepthEnable( BOOL DepthEnable )  ;									// �[�x�o�b�t�@�̗L�������̃Z�b�g
extern	int		Graphics_D3D11_DeviceState_SetDepthWriteEnable( int Flag ) ;										// �[�x�o�b�t�@�̏������݂̗L�����Z�b�g
extern	int		Graphics_D3D11_DeviceState_SetDepthFunc( D_D3D11_COMPARISON_FUNC DepthFunc ) ;						// �[�x�l�̔�r�^�C�v���Z�b�g
extern	int		Graphics_D3D11_DeviceState_SetDepthBias( int DepthBias ) ;											// �[�x�l�̃o�C�A�X���Z�b�g
extern	int		Graphics_D3D11_DeviceState_SetFillMode( D_D3D11_FILL_MODE FillMode ) ;								// �t�B�����[�h���Z�b�g
extern	int		Graphics_D3D11_DeviceState_SetWorldMatrix( const MATRIX *Matrix ) ;									// ���[���h�ϊ��p�s����Z�b�g����
extern	int		Graphics_D3D11_DeviceState_SetViewMatrix( const MATRIX *Matrix ) ;									// �r���[�ϊ��p�s����Z�b�g����
extern	int		Graphics_D3D11_DeviceState_SetProjectionMatrix( const MATRIX *Matrix ) ;							// ���e�ϊ��p�s����Z�b�g����
extern	int		Graphics_D3D11_DeviceState_SetAntiViewportMatrix( const MATRIX *Matrix ) ;							// �A���`�r���[�|�[�g�s����Z�b�g����
extern	int		Graphics_D3D11_DeviceState_SetLightState( int LightNumber, LIGHTPARAM *Light ) ;					// ���C�g�̃Z�b�g
extern	int		Graphics_D3D11_DeviceState_SetLightEnable( int LightNumber, int EnableState ) ;						// ���C�g�̗L���A������ύX
extern	int		Graphics_D3D11_DeviceState_SetMaterial( MATERIALPARAM *Material ) ;									// �}�e���A���̃Z�b�g
extern	int		Graphics_D3D11_DeviceState_SetSpecularEnable( int UseFlag ) ;										// �X�y�L�������g�p���邩�ǂ������Z�b�g
extern	int		Graphics_D3D11_DeviceState_SetUseVertexDiffuseColor( int UseFlag ) ;								// ���_�̃f�B�t���[�Y�J���[���}�e���A���̃f�B�t���[�Y�J���[�Ƃ��Ďg�p���邩�ǂ�����ݒ肷��
extern	int		Graphics_D3D11_DeviceState_SetUseVertexSpecularColor( int UseFlag ) ;								// ���_�̃X�y�L�����J���[���}�e���A���̃X�y�L�����J���[�Ƃ��Ďg�p���邩�ǂ�����ݒ肷��
extern	int		Graphics_D3D11_DeviceState_SetCullMode( int State ) ;												// �J�����O�̗L�����Z�b�g 
extern	int		Graphics_D3D11_DeviceState_SetAmbient( COLOR_F *Color ) ;												// �A���r�G���g�J���[�̃Z�b�g
extern	int		Graphics_D3D11_DeviceState_SetTextureAddress( int AddressMode, int Sampler = -1 ) ;					// �e�N�X�`���[�̃A�h���b�V���O���[�h���Z�b�g����
extern	int		Graphics_D3D11_DeviceState_SetTextureAddressU( int AddressMode, int Sampler = -1 ) ;				// �e�N�X�`���[�̃A�h���b�V���O���[�h���Z�b�g����
extern	int		Graphics_D3D11_DeviceState_SetTextureAddressV( int AddressMode, int Sampler = -1 ) ;				// �e�N�X�`���[�̃A�h���b�V���O���[�h���Z�b�g����
extern	int		Graphics_D3D11_DeviceState_SetTextureAddressW( int AddressMode, int Sampler = -1 ) ;				// �e�N�X�`���[�̃A�h���b�V���O���[�h���Z�b�g����
extern	int		Graphics_D3D11_DeviceState_SetTextureAddressUVW( int AddressModeU, int AddressModeV, int AddressModeW, int Sampler = -1 ) ;		// �e�N�X�`���[�̃A�h���b�V���O���[�h���Z�b�g����
extern	int		Graphics_D3D11_DeviceState_SetTextureAddressTransformMatrix( int Use, MATRIX *Matrix ) ;			// �e�N�X�`�����W�ϊ��s����Z�b�g����
extern	int		Graphics_D3D11_DeviceState_SetFogEnable( int Flag ) ;												// �t�H�O��L���ɂ��邩�ǂ�����ݒ肷��( TRUE:�L��  FALSE:���� )
extern	int		Graphics_D3D11_DeviceState_SetFogVertexMode( int Mode /* DX_FOGMODE_NONE �� */ ) ;					// �t�H�O���[�h��ݒ肷��
extern	int		Graphics_D3D11_DeviceState_SetFogColor( unsigned int Color ) ;												// �t�H�O�J���[��ύX����
extern	int		Graphics_D3D11_DeviceState_SetFogStartEnd( float Start, float End ) ;								// �t�H�O���n�܂鋗���ƏI�����鋗����ݒ肷��( 0.0f �` 1.0f )
extern	int		Graphics_D3D11_DeviceState_SetFogDensity( float Density ) ;											// �t�H�O�̖��x��ݒ肷��( 0.0f �` 1.0f )
extern	int		Graphics_D3D11_DeviceState_SetLighting( int UseFlag ) ;												// ���C�e�B���O�̗L���t���O���Z�b�g����
extern	int		Graphics_D3D11_DeviceState_SetMaxAnisotropy( int MaxAnisotropy, int Sampler = -1 ) ;				// �ő�ٕ������Z�b�g����
extern	int		Graphics_D3D11_DeviceState_SetViewport( D_D3D11_VIEWPORT *Viewport ) ;								// �r���[�|�[�g���Z�b�g����
extern	int		Graphics_D3D11_DeviceState_SetViewportEasy( float x1, float y1, float x2, float y2 ) ;				// �r���[�|�[�g���Z�b�g����( �ȈՔ� )
extern	int		Graphics_D3D11_DeviceState_SetDrawMode( int DrawMode ) ;											// �`�惂�[�h�̃Z�b�g
extern	int		Graphics_D3D11_DeviceState_SetAlphaTestCmpMode( int AlphaTestCmpMode /* DX_CMP_NEVER �Ȃ� */ ) ;	// �A���t�@�e�X�g�Ŏg�p�����r���[�h��ݒ肷��
extern	int		Graphics_D3D11_DeviceState_SetAlphaTestRef( int AlphaTestRef ) ;									// �A���t�@�e�X�g�Ŏg�p����Q�ƒl��ݒ肷��
extern	int		Graphics_D3D11_DeviceState_SetMulAlphaColor( int UseMulAlphaColor ) ;								// �J���[�ɃA���t�@�l����Z���邩�ǂ�����ݒ肷��
extern	int		Graphics_D3D11_DeviceState_SetPrimitiveTopology( D_D3D11_PRIMITIVE_TOPOLOGY PrimitiveTopology ) ;	// �f�o�C�X�Ŏg�p����v���~�e�B�u�^�C�v��ݒ肷��
extern	int		Graphics_D3D11_DeviceState_SetFactorColor( const DX_D3D11_SHADER_FLOAT4 *FactorColor ) ;			// Factor Color ��ݒ肷��
extern	int		Graphics_D3D11_DeviceState_SetToonOutLineSize( float Size ) ;										// �g�D�[�������_�����O�̗֊s���̑�����ݒ肷��
extern	int		Graphics_D3D11_DeviceState_SetBlendMode( int BlendMode ) ;											// �`��u�����h���[�h�̃Z�b�g
extern	int		Graphics_D3D11_DeviceState_SetRenderTarget( D_ID3D11Texture2D *RenderTargetTexture, D_ID3D11RenderTargetView *RenderTargetView, int TargetIndex = 0 ) ;	// �`��Ώۂ̕ύX
extern	int		Graphics_D3D11_DeviceState_SetDepthStencil( D_ID3D11Texture2D *DepthStencilTexture, D_ID3D11DepthStencilView *DepthStencilView ) ;	// �`��Ώۂ̐[�x�o�b�t�@��ύX
extern	int		Graphics_D3D11_DeviceState_SetInputLayout( D_ID3D11InputLayout *InputLayout ) ;						// �g�p������̓��C�A�E�g��ύX����
extern	int		Graphics_D3D11_DeviceState_SetVertexShaderToHandle( int ShaderHandle ) ;								// �w��̒��_�V�F�[�_�[���f�o�C�X�ɃZ�b�g����
extern	int		Graphics_D3D11_DeviceState_SetVertexShader( D_ID3D11VertexShader *VertexShader, int NormalVertexShader = FALSE ) ;	// �g�p���钸�_�V�F�[�_�[��ύX����
extern	int		Graphics_D3D11_DeviceState_ResetVertexShader( int SetNormalVertexShaderCancel = FALSE ) ;						// ���_�V�F�[�_�[�̎g�p���~�߂�
extern	int		Graphics_D3D11_DeviceState_SetVertexShaderConstantBuffers( UINT StartSlot, UINT NumBuffers, D_ID3D11Buffer * const *pConstantBuffers ) ;	// ���_�V�F�[�_�[�Ŏg�p����萔�o�b�t�@�[��ݒ肷��
extern	int		Graphics_D3D11_DeviceState_SetPixelShaderToHandle( int ShaderHandle ) ;								// �w��̃s�N�Z���V�F�[�_�[���f�o�C�X�ɃZ�b�g����
extern	int		Graphics_D3D11_DeviceState_SetPixelShader( D_ID3D11PixelShader *PixelShader, int NormalPixelShader = FALSE ) ;	// �g�p����s�N�Z���V�F�[�_�[��ύX����
extern	int		Graphics_D3D11_DeviceState_ResetPixelShader( int SetNormalPixelShaderCancel = FALSE ) ;							// �s�N�Z���V�F�[�_�[�̎g�p���~�߂�
extern	int		Graphics_D3D11_DeviceState_SetPixelShaderConstantBuffers( UINT StartSlot, UINT NumBuffers, D_ID3D11Buffer * const *pConstantBuffers ) ;	// �s�N�Z���V�F�[�_�[�Ŏg�p����萔�o�b�t�@�[��ݒ肷��
extern	int		Graphics_D3D11_DeviceState_SetVertexBuffer( D_ID3D11Buffer *VertexBuffer, int VertexStride = 0 ) ;	// �g�p���钸�_�o�b�t�@��ύX����
extern	int		Graphics_D3D11_DeviceState_SetIndexBuffer( D_ID3D11Buffer *IndexBuffer, D_DXGI_FORMAT Format ) ;	// �g�p����C���f�b�N�X�o�b�t�@��ύX����
extern	int		Graphics_D3D11_DeviceState_SetPSShaderResouceView( int StartSlot, int Num, D_ID3D11ShaderResourceView * const *ppShaderResourceViews ) ;	// �s�N�Z���V�F�[�_�[�Ŏg�p����V�F�[�_�[���\�[�X�r���[��ύX����
extern	int		Graphics_D3D11_DeviceState_NormalDrawSetup( void ) ;												// �W���`��̏������s��
extern	int		Graphics_D3D11_DeviceState_SetupStateAndConstantBuffer( void ) ;									// ���݂̐ݒ�Ɋ�Â��� ID3D11SamplerState�AID3D11RasterizerState�AID3D11DepthStencilState�AID3D11BlendState �̃Z�b�g�A�b�v�ƁA�萔�o�b�t�@�̃A�b�v�f�[�g���s��






// �`��ݒ�֌W�֐�
extern	int		Graphics_D3D11_DrawSetting_SetDrawBlendMode( int BlendMode, int AlphaTestValidFlag, int AlphaChannelValidFlag ) ;	// �`��u�����h���[�h�̐ݒ�
extern	int		Graphics_D3D11_DrawSetting_SetIgnoreDrawGraphColor( int EnableFlag ) ;							// �`�掞�̉摜�̐F�𖳎����邩�ǂ�����ݒ肷��
extern	int		Graphics_D3D11_DrawSetting_SetDrawAlphaTest( int TestMode, int TestParam ) ;					// �A���t�@�e�X�g�̐ݒ�
extern	int		Graphics_D3D11_DrawSetting_SetBlendTextureParam( int BlendType, int *Param ) ;					// �u�����h����e�N�X�`���̃p�����[�^���Z�b�g����
extern	int		Graphics_D3D11_DrawSetting_SetTexture(      D_ID3D11Texture2D *RenderTexture, D_ID3D11ShaderResourceView *RenderTextureSRV ) ;											// �e�N�X�`�����Z�b�g���� 
extern	int		Graphics_D3D11_DrawSetting_SetBlendTexture( D_ID3D11Texture2D *BlendTexture,  D_ID3D11ShaderResourceView *BlendTextureSRV, int TexWidth = -1, int TexHeight = -1 ) ;	// �u�����h����e�N�X�`�����Z�b�g���� 









// �萔���֌W�֐�
extern int Graphics_D3D11_Constant_Initialize( void ) ;												// Direct3D11 �̒萔���̏��������s��
extern int Graphics_D3D11_Constant_Terminate( void ) ;												// Direct3D11 �̒萔���̌�n�����s��








// �萔�o�b�t�@�֌W�֐�
extern	CONSTANTBUFFER_DIRECT3D11 *Graphics_D3D11_ConstantBuffer_Create( DWORD Size, int ASyncThread = FALSE ) ;	// �萔�o�b�t�@���쐬����
extern	int		Graphics_D3D11_ConstantBuffer_Delete( CONSTANTBUFFER_DIRECT3D11 *ConstantBuffer ) ;					// �萔�o�b�t�@���폜����
extern	int		Graphics_D3D11_ConstantBuffer_Update( CONSTANTBUFFER_DIRECT3D11 *ConstantBuffer ) ;					// �V�X�e����������̃o�b�t�@�ɍs�����ύX��K�p����
extern	int		Graphics_D3D11_ConstantBuffer_VSSet( UINT StartSlot, UINT NumBuffers, CONSTANTBUFFER_DIRECT3D11 * const *pConstantBuffers ) ;	// �w��̒萔�o�b�t�@�𒸓_�V�F�[�_�[�ɐݒ肷��
extern	int		Graphics_D3D11_ConstantBuffer_PSSet( UINT StartSlot, UINT NumBuffers, CONSTANTBUFFER_DIRECT3D11 * const *pConstantBuffers ) ;	// �w��̒萔�o�b�t�@���s�N�Z���V�F�[�_�[�ɐݒ肷��









// ���̓��C�A�E�g�֌W�֐�
extern	int		Graphics_D3D11_InputLayout_Base_Create( void ) ;		// �W���`��p�� ID3D11InputLayout �I�u�W�F�N�g�̍쐬( 0:����  -1:���s )
extern	int		Graphics_D3D11_InputLayout_Base_Terminate( void ) ;		// �W���`��p�� ID3D11InputLayout �I�u�W�F�N�g�̍폜( 0:����  -1:���s )

#ifndef DX_NON_MODEL
extern	int		Graphics_D3D11_InputLayout_Model_Create( int BumpMap, int MeshType ) ;	// ���f���`��p�� ID3D11InputLayout �I�u�W�F�N�g�̍쐬( 0:����  -1:���s )
extern	int		Graphics_D3D11_InputLayout_Model_Terminate( void ) ;					// ���f���`��p�� ID3D11InputLayout �I�u�W�F�N�g�̍폜( 0:����  -1:���s )
#endif // DX_NON_MODEL








// �W���`��֌W
extern	int		Graphics_D3D11_NormalDrawVertexBuffer_Create( void ) ;		// �W���`��p�̒��_�o�b�t�@�̍쐬( 0:����  -1:���s )
extern	int		Graphics_D3D11_NormalDrawVertexBuffer_Terminate( void ) ;	// �W���`��p�̒��_�o�b�t�@�̍폜( 0:����  -1:���s )

extern	int		Graphics_D3D11_CommonBuffer_DrawPrimitive(        int InputLayout, D_D3D11_PRIMITIVE_TOPOLOGY PrimitiveType, const void *VertexData, int VertexNum,                                                                 int UseDefaultVertexShader = TRUE ) ;	// �ėp�`��p���_�o�b�t�@���g�p���ăV�X�e����������̒��_�f�[�^�̕`����s��
extern	int		Graphics_D3D11_CommonBuffer_DrawIndexedPrimitive( int InputLayout, D_D3D11_PRIMITIVE_TOPOLOGY PrimitiveType, const void *VertexData, int VertexNum, const void *IndexData, int IndexNum, D_DXGI_FORMAT IndexFormat, int UseDefaultVertexShader = TRUE ) ;	// �ėp�`��p���_�o�b�t�@�E�C���f�b�N�X�o�b�t�@���g�p���ăV�X�e����������̒��_�f�[�^�E�C���f�b�N�X�f�[�^�̕`����s��
extern	int		Graphics_D3D11_CommonVertexBuffer_Setup( int Size ) ;		// �w��T�C�Y�̔ėp�`��p���_�o�b�t�@�̏������s��
extern	int		Graphics_D3D11_CommonIndexBuffer_Setup( int Size ) ;		// �w��T�C�Y�̔ėp�`��p�C���f�b�N�X�o�b�t�@�̏������s��
extern	int		Graphics_D3D11_CommonVertexBuffer_Terminate( void ) ;		// �ėp�`��p���_�o�b�t�@�̌�n�����s��
extern	int		Graphics_D3D11_CommonIndexBuffer_Terminate( void ) ;		// �ėp�`��p�C���f�b�N�X�o�b�t�@�̌�n�����s��













// Direct3D11 �̕`�揈�������֌W
extern	int		Graphics_D3D11_RenderVertex( int NextUse3DVertex = -1, int ASyncThread = FALSE ) ;	// ���_�o�b�t�@�ɗ��܂������_�f�[�^�������_�����O����
extern	void	FASTCALL Graphics_D3D11_DrawPreparation( int ParamFlag = 0 ) ;						// �`�揀�����s��
extern	void	Graphics_D3D11_RenderBegin( void ) ;												// �`��R�}���h���J�n����
extern	void	Graphics_D3D11_RenderEnd( void ) ;													// �`��R�}���h���I������












// Direct3D11 ���g�����`��֌W
extern	int		Graphics_D3D11_DrawBillboard3D( VECTOR Pos, float cx, float cy, float Size, float Angle, IMAGEDATA *Image, IMAGEDATA *BlendImage, int TransFlag, int TurnFlag, int DrawFlag = TRUE, RECT *DrawArea = NULL ) ;	// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawBillboard3D
extern	int		Graphics_D3D11_DrawModiBillboard3D( VECTOR Pos, float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, IMAGEDATA *Image, IMAGEDATA *BlendImage, int TransFlag, int DrawFlag = TRUE, RECT *DrawArea = NULL ) ;	// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawModiBillboard3D
extern	int		Graphics_D3D11_DrawGraph( int x, int y, float xf, float yf, IMAGEDATA *Image, IMAGEDATA *BlendImage, int TransFlag, int IntFlag ) ;																// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawGraph
extern	int		Graphics_D3D11_DrawExtendGraph( int x1, int y1, int x2, int y2, float x1f, float y1f, float x2f, float y2f, IMAGEDATA *Image, IMAGEDATA *BlendImage, int TransFlag, int IntFlag ) ;				// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawExtendGraph
extern	int		Graphics_D3D11_DrawRotaGraph( int x, int y, float xf, float yf, double ExRate, double Angle, IMAGEDATA *Image, IMAGEDATA *BlendImage, int TransFlag, int TurnFlag, int IntFlag ) ;				// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawRotaGraph
extern	int		Graphics_D3D11_DrawRotaGraphFast( int x, int y, float xf, float yf, float ExRate, float Angle, IMAGEDATA *Image, IMAGEDATA *BlendImage, int TransFlag, int TurnFlag, int IntFlag ) ;			// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawRotaGraphFast
extern	int		Graphics_D3D11_DrawModiGraph( int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, IMAGEDATA *Image, IMAGEDATA *BlendImage, int TransFlag, bool SimpleDrawFlag ) ;					// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawModiGraph
extern	int		Graphics_D3D11_DrawModiGraphF( float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, IMAGEDATA *Image, IMAGEDATA *BlendImage, int TransFlag, bool SimpleDrawFlag ) ;	// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawModiGraphF
extern	void	Graphics_D3D11_DrawModiTex( float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, IMAGEDATA_HARD_DRAW *DrawTex, bool SimpleDrawFlag ) ;								// �e�N�X�`����ό`���ĕ`�悷��
extern	int		Graphics_D3D11_DrawSimpleQuadrangleGraphF( const GRAPHICS_DRAW_DRAWSIMPLEQUADRANGLEGRAPHF_PARAM *Param, IMAGEDATA *Image, IMAGEDATA *BlendImage ) ;												// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawSimpleQuadrangleGraphF
extern	int		Graphics_D3D11_DrawSimpleTriangleGraphF(   const GRAPHICS_DRAW_DRAWSIMPLETRIANGLEGRAPHF_PARAM   *Param, IMAGEDATA *Image, IMAGEDATA *BlendImage ) ;												// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawSimpleTriangleGraphF

extern	int		Graphics_D3D11_DrawFillBox( int x1, int y1, int x2, int y2, unsigned int Color ) ;																		// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawFillBox
extern	int		Graphics_D3D11_DrawLineBox( int x1, int y1, int x2, int y2, unsigned int Color ) ;																		// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawLineBox
extern	int		Graphics_D3D11_DrawLine( int x1, int y1, int x2, int y2, unsigned int Color ) ;																			// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawLine
extern	int		Graphics_D3D11_DrawLine3D( VECTOR Pos1, VECTOR Pos2, unsigned int Color, int DrawFlag = TRUE, RECT *DrawArea = NULL ) ;									// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawLine3D
extern	int		Graphics_D3D11_DrawCircle_Thickness( int x, int y, int r, unsigned int Color, int Thickness ) ;															// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawCircle( �����w�肠�� )
extern	int		Graphics_D3D11_DrawOval_Thickness( int x, int y, int rx, int ry, unsigned int Color, int Thickness ) ;													// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawOval( �����w�肠�� )
extern	int		Graphics_D3D11_DrawCircle( int x, int y, int r, unsigned int Color, int FillFlag ) ;																		// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawCircle
extern	int		Graphics_D3D11_DrawOval( int x, int y, int rx, int ry, unsigned int Color, int FillFlag ) ;																// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawOval
extern	int		Graphics_D3D11_DrawTriangle( int x1, int y1, int x2, int y2, int x3, int y3, unsigned int Color, int FillFlag ) ;											// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawTriangle
extern	int		Graphics_D3D11_DrawTriangle3D( VECTOR Pos1, VECTOR Pos2, VECTOR Pos3, unsigned int Color, int FillFlag, int DrawFlag = TRUE, RECT *DrawArea = NULL ) ;	// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawTriangle3D
extern	int		Graphics_D3D11_DrawQuadrangle( int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, unsigned int Color, int FillFlag ) ;						// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawQuadrangle
extern	int		Graphics_D3D11_DrawPixel( int x, int y, unsigned int Color ) ;																							// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawPixel
extern	int		Graphics_D3D11_DrawPixel3D( VECTOR Pos, unsigned int Color, int DrawFlag = TRUE, RECT *DrawArea = NULL ) ;												// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawPixel3D
extern	int		Graphics_D3D11_DrawPixelSet( const POINTDATA *PointData, int Num ) ;																				// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawPixelSet
extern	int		Graphics_D3D11_DrawLineSet( const LINEDATA *LineData, int Num ) ;																				// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawLineSet

extern	int		Graphics_D3D11_DrawPrimitive(                             const VERTEX_3D *Vertex, int VertexNum,                                    int PrimitiveType, IMAGEDATA *Image, int TransFlag ) ;
extern	int		Graphics_D3D11_DrawIndexedPrimitive(                      const VERTEX_3D *Vertex, int VertexNum, const WORD *Indices, int IndexNum, int PrimitiveType, IMAGEDATA *Image, int TransFlag ) ;
extern	int		Graphics_D3D11_DrawPrimitiveLight(                        const VERTEX3D  *Vertex, int VertexNum,                                    int PrimitiveType, IMAGEDATA *Image, int TransFlag ) ;
extern	int		Graphics_D3D11_DrawIndexedPrimitiveLight(                 const VERTEX3D  *Vertex, int VertexNum, const WORD *Indices, int IndexNum, int PrimitiveType, IMAGEDATA *Image, int TransFlag ) ;
extern	int		Graphics_D3D11_DrawPrimitiveLight_UseVertexBuffer(        VERTEXBUFFERHANDLEDATA *VertexBuffer,                                      int PrimitiveType,                 int StartVertex, int UseVertexNum, IMAGEDATA *Image, int TransFlag ) ;
extern	int		Graphics_D3D11_DrawIndexedPrimitiveLight_UseVertexBuffer( VERTEXBUFFERHANDLEDATA *VertexBuffer, INDEXBUFFERHANDLEDATA *IndexBuffer,  int PrimitiveType, int BaseVertex, int StartVertex, int UseVertexNum, int StartIndex, int UseIndexNum, IMAGEDATA *Image, int TransFlag ) ;
extern	int		Graphics_D3D11_DrawPrimitive2D(                                 VERTEX_2D *Vertex, int VertexNum,                                    int PrimitiveType, IMAGEDATA *Image, int TransFlag, int BillboardFlag, int Is3D, int TurnFlag, int TextureNo, int IsShadowMap ) ;
extern	int		Graphics_D3D11_DrawPrimitive2DUser(                       const VERTEX2D  *Vertex, int VertexNum,                                    int PrimitiveType, IMAGEDATA *Image, int TransFlag, int Is3D, int TurnFlag, int TextureNo ) ;
extern	int		Graphics_D3D11_DrawIndexedPrimitive2DUser(                const VERTEX2D  *Vertex, int VertexNum, const WORD *Indices, int IndexNum, int PrimitiveType, IMAGEDATA *Image, int TransFlag ) ;

extern	void	Graphics_D3D11_DrawPreparationToShader( int ParamFlag, int Is2D ) ;											// �V�F�[�_�[�`��p�`��O�Z�b�g�A�b�v�֐�

extern	int		Graphics_D3D11_DrawPrimitive2DToShader(        const VERTEX2DSHADER *Vertex, int VertexNum,                                              int PrimitiveType /* DX_PRIMTYPE_TRIANGLELIST �� */ ) ;		// �V�F�[�_�[���g���ĂQ�c�v���~�e�B�u��`�悷��
extern	int		Graphics_D3D11_DrawPrimitive3DToShader(        const VERTEX3DSHADER *Vertex, int VertexNum,                                              int PrimitiveType /* DX_PRIMTYPE_TRIANGLELIST �� */ ) ;		// �V�F�[�_�[���g���ĂR�c�v���~�e�B�u��`�悷��
extern	int		Graphics_D3D11_DrawPrimitiveIndexed2DToShader( const VERTEX2DSHADER *Vertex, int VertexNum, const unsigned short *Indices, int IndexNum, int PrimitiveType /* DX_PRIMTYPE_TRIANGLELIST �� */ ) ;		// �V�F�[�_�[���g���ĂQ�c�v���~�e�B�u��`�悷��( ���_�C���f�b�N�X���g�p���� )
extern	int		Graphics_D3D11_DrawPrimitiveIndexed3DToShader( const VERTEX3DSHADER *Vertex, int VertexNum, const unsigned short *Indices, int IndexNum, int PrimitiveType /* DX_PRIMTYPE_TRIANGLELIST �� */ ) ;		// �V�F�[�_�[���g���ĂR�c�v���~�e�B�u��`�悷��( ���_�C���f�b�N�X���g�p���� )

extern	int		Graphics_D3D11_DrawPrimitive3DToShader_UseVertexBuffer2(        int VertexBufHandle,                     int PrimitiveType /* DX_PRIMTYPE_TRIANGLELIST �� */, int StartVertex, int UseVertexNum ) ;	// �V�F�[�_�[���g���ĂR�c�v���~�e�B�u��`�悷��( ���_�o�b�t�@�g�p�� )
extern	int		Graphics_D3D11_DrawPrimitiveIndexed3DToShader_UseVertexBuffer2( int VertexBufHandle, int IndexBufHandle, int PrimitiveType /* DX_PRIMTYPE_TRIANGLELIST �� */, int BaseVertex, int StartVertex, int UseVertexNum, int StartIndex, int UseIndexNum ) ;	// �V�F�[�_�[���g���ĂR�c�v���~�e�B�u��`�悷��( ���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@�g�p�� )








// ���ˑ��������֌W
extern	int		Graphics_D3D11_Initialize_Timing0_PF( void ) ;											// �`�揈���̊��ˑ������̏��������s���֐�( ���s�ӏ���ʂO )
extern	int		Graphics_D3D11_Initialize_Timing1_PF( void ) ;											// �`�揈���̊��ˑ������̏��������s���֐�( ���s�ӏ���ʂQ )
extern	int		Graphics_D3D11_Hardware_Initialize_PF( void ) ;											// �n�[�h�E�G�A�A�N�Z�����[�^���g�p����ꍇ�̊��ˑ��̏������������s��
extern	int		Graphics_D3D11_Terminate_PF( void ) ;													// �`�揈���̊��ˑ������̌�n�����s���֐�
extern	int		Graphics_D3D11_RestoreOrChangeSetupGraphSystem_PF( int Change, int ScreenSizeX = -1, int ScreenSizeY = -1, int ColorBitDepth = -1, int RefreshRate = -1 ) ;		// �O���t�B�b�N�X�V�X�e���̕��A�A���͕ύX�t���̍ăZ�b�g�A�b�v���s��
extern	int		Graphics_D3D11_Hardware_CheckValid_PF( void ) ;											// �`��p�f�o�C�X���L�����ǂ������擾����( �߂�l  TRUE:�L��  FALSE:���� )
extern	void	Graphics_D3D11_ReleaseDirectXObject_PF( void ) ;											// DirectX �̃I�u�W�F�N�g���������








// ���ˑ��V�X�e���֌W
extern	void	Graphics_D3D11_WM_ACTIVATE_ActiveProcess_PF( void ) ;									// WM_ACTIVATE ���b�Z�[�W�̏����ŁA�E�C���h�E���[�h���ɃA�N�e�B�u�ɂȂ����ۂɌĂ΂��֐�









// ���ˑ��`��ݒ�֌W
extern	int		Graphics_Hardware_D3D11_SetRenderTargetToShader_PF( int TargetIndex, int DrawScreen, int SurfaceIndex ) ;	// �V�F�[�_�[�`��ł̕`����ݒ肷��
extern	int		Graphics_Hardware_D3D11_SetDrawBrightToOneParam_PF( DWORD Bright ) ;											// SetDrawBright �̈��������
extern	int		Graphics_Hardware_D3D11_SetDrawBlendMode_PF( int BlendMode, int BlendParam ) ;								// �`��u�����h���[�h���Z�b�g����
extern	int		Graphics_Hardware_D3D11_SetDrawAlphaTest_PF( int TestMode, int TestParam ) ;									// �`�掞�̃A���t�@�e�X�g�̐ݒ���s��( TestMode:DX_CMP_GREATER��( -1:�f�t�H���g����ɖ߂� )  TestParam:�`��A���t�@�l�Ƃ̔�r�Ɏg�p����l )
extern	int		Graphics_Hardware_D3D11_SetDrawMode_PF( int DrawMode ) ;														// �`�惂�[�h���Z�b�g����
extern	int		Graphics_Hardware_D3D11_SetDrawBright_PF( int RedBright, int GreenBright, int BlueBright ) ;					// �`��P�x���Z�b�g
extern	int		Graphics_Hardware_D3D11_SetBlendGraphParamBase_PF( IMAGEDATA *BlendImage, int BlendType, int *Param ) ;		// SetBlendGraphParam �̉ϒ������p�����[�^�t��
extern	int		Graphics_Hardware_D3D11_SetMaxAnisotropy_PF( int MaxAnisotropy ) ;											// �ő�ٕ����̒l���Z�b�g����
extern	int		Graphics_Hardware_D3D11_SetTransformToWorld_PF( const MATRIX *Matrix ) ;										// ���[���h�ϊ��p�s����Z�b�g����
extern	int		Graphics_Hardware_D3D11_SetTransformToView_PF( const MATRIX *Matrix ) ;										// �r���[�ϊ��p�s����Z�b�g����
extern	int		Graphics_Hardware_D3D11_SetTransformToProjection_PF( const MATRIX *Matrix ) ;								// ���e�ϊ��p�s����Z�b�g����
extern	int		Graphics_Hardware_D3D11_SetTransformToViewport_PF( const MATRIX *Matrix ) ;									// �r���[�|�[�g�s����Z�b�g����
extern	int		Graphics_Hardware_D3D11_SetTextureAddressMode_PF( int Mode /* DX_TEXADDRESS_WRAP �� */, int Stage ) ;		// �e�N�X�`���A�h���X���[�h��ݒ肷��
extern	int		Graphics_Hardware_D3D11_SetTextureAddressModeUV_PF( int ModeU, int ModeV, int Stage ) ;						// �e�N�X�`���A�h���X���[�h��ݒ肷��
extern	int		Graphics_Hardware_D3D11_SetTextureAddressTransformMatrix_PF( int UseFlag, MATRIX *Matrix, int Sampler = -1 ) ;// �e�N�X�`�����W�ϊ��s����Z�b�g����
extern	int		Graphics_Hardware_D3D11_SetFogEnable_PF( int Flag ) ;														// �t�H�O��L���ɂ��邩�ǂ�����ݒ肷��( TRUE:�L��  FALSE:���� )
extern	int		Graphics_Hardware_D3D11_SetFogMode_PF( int Mode /* DX_FOGMODE_NONE �� */ ) ;									// �t�H�O���[�h��ݒ肷��
extern	int		Graphics_Hardware_D3D11_SetFogColor_PF( DWORD FogColor ) ;													// �t�H�O�J���[��ύX����
extern	int		Graphics_Hardware_D3D11_SetFogStartEnd_PF( float start, float end ) ;										// �t�H�O���n�܂鋗���ƏI�����鋗����ݒ肷��( 0.0f �` 1.0f )
extern	int		Graphics_Hardware_D3D11_SetFogDensity_PF( float density ) ;													// �t�H�O�̖��x��ݒ肷��( 0.0f �` 1.0f )
extern	int		Graphics_Hardware_D3D11_ApplyLigFogToHardware_PF( void ) ;													// ��{�f�[�^�ɐݒ肳��Ă���t�H�O�����n�[�h�E�F�A�ɔ��f����
extern	int		Graphics_Hardware_D3D11_SetUseOldDrawModiGraphCodeFlag_PF( int Flag ) ;										// �ȑO�� DrawModiGraph �֐��̃R�[�h���g�p���邩�ǂ����̃t���O���Z�b�g����
extern	int		Graphics_Hardware_D3D11_RefreshAlphaChDrawMode_PF( void ) ;													// �`���ɐ��������l���������ނ��ǂ����̃t���O���X�V����








// ���ˑ��ݒ�֌W
extern	int		Graphics_Hardware_D3D11_SetUsePixelLighting_PF( int Flag ) ;																	// �s�N�Z���P�ʂŃ��C�e�B���O���s�����ǂ�����ݒ肷��A�v ShaderModel 3.0( TRUE:�s�N�Z���P�ʂ̃��C�e�B���O���s��  FALSE:���_�P�ʂ̃��C�e�B���O���s��( �f�t�H���g ) )
extern	int		Graphics_Hardware_D3D11_SetGraphicsDeviceRestoreCallbackFunction_PF( void (* Callback )( void *Data ), void *CallbackData ) ;	// �O���t�B�b�N�X�f�o�C�X�����X�g���畜�A�����ۂɌĂ΂��R�[���o�b�N�֐���ݒ肷��
extern	int		Graphics_Hardware_D3D11_SetGraphicsDeviceLostCallbackFunction_PF( void (* Callback )( void *Data ), void *CallbackData ) ;		// �O���t�B�b�N�X�f�o�C�X�����X�g���畜�A����O�ɌĂ΂��R�[���o�b�N�֐���ݒ肷��
extern	int		Graphics_Hardware_D3D11_RefreshDxLibDirect3DSetting_PF( void ) ;																// �c�w���C�u�����̂c�����������R�c�ݒ�����Ȃ���
extern	int		Graphics_D3D11_SetUseDirect3D11MinFeatureLevel_PF( int Level /* DX_DIRECT3D_11_FEATURE_LEVEL_10_0 �Ȃ� */ ) ;					// Direct3D11 �Ŏg�p����Œ�@�\���x�����w�肷��֐��ł��A���ADX_DIRECT3D_11_FEATURE_LEVEL_11_0 ���Ⴂ�@�\���x���ł̐���ȓ���͕ۏ؂��܂���( �f�t�H���g�� DX_DIRECT3D_11_FEATURE_LEVEL_11_0 )








// ���ˑ���ʊ֌W
extern	int		Graphics_Hardware_D3D11_SetupUseZBuffer_PF( void ) ;															// �ݒ�Ɋ�Â��Ďg�p����[�x�o�b�t�@���Z�b�g����
extern	int		Graphics_Hardware_D3D11_ClearDrawScreenZBuffer_PF( const RECT *ClearRect ) ;									// ��ʂ̐[�x�o�b�t�@�̏�Ԃ�����������
extern	int		Graphics_Hardware_D3D11_ClearDrawScreen_PF( const RECT *ClearRect ) ;										// ��ʂ̏�Ԃ�����������
extern	int		Graphics_Hardware_D3D11_SetDrawScreen_PF( int DrawScreen, int OldScreenSurface, IMAGEDATA *NewTargetImage, IMAGEDATA *OldTargetImage, SHADOWMAPDATA *NewTargetShadowMap, SHADOWMAPDATA *OldTargetShadowMap ) ;						// �`����ʂ̃Z�b�g
extern	int		Graphics_Hardware_D3D11_SetDrawScreen_Post_PF( int DrawScreen ) ;											// SetDrawScreen �̍Ō�ŌĂ΂��֐�
extern	int		Graphics_Hardware_D3D11_SetDrawArea_PF( int x1, int y1, int x2, int y2 ) ;									// �`��\�̈�̃Z�b�g
extern	int		Graphics_Hardware_D3D11_LockDrawScreenBuffer_PF( RECT *LockRect, BASEIMAGE *BaseImage, int TargetScreen, IMAGEDATA *TargetImage, int TargetScreenSurface, int ReadOnly, int TargetScreenTextureNo ) ;	// �`���o�b�t�@�����b�N����
extern	int		Graphics_Hardware_D3D11_UnlockDrawScreenBuffer_PF( void ) ;													// �`���o�b�t�@���A�����b�N����
extern	int		Graphics_Hardware_D3D11_ScreenCopy_PF( void ) ;																// ����ʂ̓��e��\��ʂɕ`�悷��
extern	int		Graphics_Hardware_D3D11_WaitVSync_PF( int SyncNum ) ;														// ���������M����҂�
extern	int		Graphics_Hardware_D3D11_ScreenFlipBase_PF( void ) ;															// ����ʂƕ\��ʂ���������
extern	int		Graphics_Hardware_D3D11_BltRectBackScreenToWindow_PF( HWND Window, RECT BackScreenRect, RECT WindowClientRect ) ;		// ����ʂ̎w��̗̈���E�C���h�E�̃N���C�A���g�̈�̎w��̗̈�ɓ]������
extern	int		Graphics_Hardware_D3D11_SetScreenFlipTargetWindow_PF( HWND TargetWindow ) ;									// ScreenFlip �ŉ摜��]�������̃E�C���h�E��ݒ肷��( NULL ���w�肷��Ɛݒ���� )
extern	int		Graphics_Hardware_D3D11_SetZBufferMode_PF( int ZBufferSizeX, int ZBufferSizeY, int ZBufferBitDepth ) ;		// ���C����ʂ̂y�o�b�t�@�̐ݒ��ύX����
extern	int		Graphics_Hardware_D3D11_SetDrawZBuffer_PF( int DrawScreen, IMAGEDATA *Image ) ;								// �`���y�o�b�t�@�̃Z�b�g
extern	const void*	Graphics_Hardware_D3D11_GetUseDirect3D11Device_PF( void ) ;												// �g�p����ID3D11Device�I�u�W�F�N�g���擾����( �߂�l�� ID3D11Device * �ɃL���X�g���Ă������� )
extern	const void*	Graphics_Hardware_D3D11_GetUseDirect3D11DeviceContext_PF( void ) ;										// �g�p����ID3D11DeviceContext�I�u�W�F�N�g���擾����( �߂�l�� ID3D11DeviceContext * �ɃL���X�g���Ă������� )
extern	const void*	Graphics_Hardware_D3D11_GetUseDirect3D11BackBufferTexture2D_PF( void ) ;								// �g�p���̃o�b�N�o�b�t�@��ID3D11Texture2D�I�u�W�F�N�g���擾����( �߂�l�� ID3D11Texture2D * �ɃL���X�g���Ă������� )
extern	const void*	Graphics_Hardware_D3D11_GetUseDirect3D11DepthStencilTexture2D_PF( void ) ;								// �g�p���̐[�x�X�e���V���o�b�t�@��ID3D11Texture2D�I�u�W�F�N�g���擾����( �߂�l�� ID3D11Texture2D * �ɃL���X�g���Ă������� )








// ���ˑ����擾�֌W
extern	const COLORDATA *	Graphics_Hardware_D3D11_GetMainColorData_PF( void ) ;				// GetColor �� GetColor2 �Ŏg�p����J���[�f�[�^���擾����
extern	const COLORDATA *	Graphics_Hardware_D3D11_GetDispColorData_PF( void ) ;				// �f�B�X�v���[�̃J���[�f�[�^�|�C���^�𓾂�
extern	DWORD				Graphics_Hardware_D3D11_GetPixel_PF( int x, int y ) ;				// �w����W�̐F���擾����








// ���ˑ��摜�֌W
extern	int		Graphics_Hardware_D3D11_UpdateGraphMovie_TheoraYUV_PF( MOVIEGRAPH *Movie, IMAGEDATA *Image ) ;			// YUV�T�[�t�F�X���g���� Theora ����̓��e���O���t�B�b�N�X�n���h���̃e�N�X�`���ɓ]������
extern	int		Graphics_Hardware_D3D11_GraphLock_PF( IMAGEDATA *Image, COLORDATA **ColorDataP, int WriteOnly ) ;		// �O���t�B�b�N�������̈�̃��b�N
extern	int		Graphics_Hardware_D3D11_GraphUnlock_PF( IMAGEDATA *Image ) ;												// �O���t�B�b�N�������̈�̃��b�N����
extern	int		Graphics_Hardware_D3D11_CopyGraphZBufferImage_PF( IMAGEDATA *DestImage, IMAGEDATA *SrcImage ) ;			// �O���t�B�b�N�̂y�o�b�t�@�̏�Ԃ�ʂ̃O���t�B�b�N�̂y�o�b�t�@�ɃR�s�[����( DestGrHandle �� SrcGrHandle ���y�o�b�t�@�������Ă���`��\�摜�ŁA���A���`�G�C���A�X�摜�ł͂Ȃ����Ƃ����� )
extern	int		Graphics_Hardware_D3D11_InitGraph_PF( void ) ;															// �摜�f�[�^�̏�����
extern	int		Graphics_Hardware_D3D11_FillGraph_PF( IMAGEDATA *Image, int Red, int Green, int Blue, int Alpha, int ASyncThread ) ;			// �O���t�B�b�N�����̐F�œh��Ԃ�
extern	int		Graphics_Hardware_D3D11_GetDrawScreenGraphBase_PF( IMAGEDATA *Image, IMAGEDATA *TargetImage, int TargetScreen, int TargetScreenSurface, int TargetScreenWidth, int TargetScreenHeight, int x1, int y1, int x2, int y2, int destX, int destY ) ;		// �`��\�摜��o�b�N�o�b�t�@����w��̈�̃O���t�B�b�N���擾����

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
) ;

extern	int		Graphics_Hardware_D3D11_CreateOrigTexture_PF(  IMAGEDATA_ORIG *Orig, int ASyncThread = FALSE ) ;	// �I���W�i���摜��񒆂̃e�N�X�`�����쐬����( 0:����  -1:���s )
extern	int		Graphics_Hardware_D3D11_ReleaseOrigTexture_PF( IMAGEDATA_ORIG *Orig ) ;								// �I���W�i���摜��񒆂̃e�N�X�`�����������
extern	int		Graphics_Hardware_D3D11_GetMultiSampleQuality_PF( int Samples ) ;									// �w��̃}���`�T���v�����Ŏg�p�ł���ő�N�I���e�B�l���擾����
extern	int		Graphics_Hardware_D3D11_SetUsePlatformTextureFormat_PF( int PlatformTextureFormat ) ;				// �쐬����O���t�B�b�N�n���h���Ŏg�p������ˑ��̃e�N�X�`���t�H�[�}�b�g���w�肷��








// ���ˑ����_�o�b�t�@�E�C���f�b�N�X�o�b�t�@�֌W
extern	int		Graphics_Hardware_D3D11_VertexBuffer_Create_PF(    VERTEXBUFFERHANDLEDATA *VertexBuffer ) ;															// ���_�o�b�t�@�n���h���̒��_�o�b�t�@���쐬����
extern	int		Graphics_Hardware_D3D11_VertexBuffer_Terminate_PF( VERTEXBUFFERHANDLEDATA *VertexBuffer ) ;															// ���_�o�b�t�@�n���h���̌�n��
extern	int		Graphics_Hardware_D3D11_VertexBuffer_SetData_PF(   VERTEXBUFFERHANDLEDATA *VertexBuffer, int SetIndex, const void *VertexData, int VertexNum ) ;	// ���_�o�b�t�@�ɒ��_�f�[�^��]������
extern	int		Graphics_Hardware_D3D11_IndexBuffer_Create_PF(     INDEXBUFFERHANDLEDATA *IndexBuffer ) ;															// �C���f�b�N�X�o�b�t�@�n���h���̃Z�b�g�A�b�v���s��
extern	int		Graphics_Hardware_D3D11_IndexBuffer_Terminate_PF(  INDEXBUFFERHANDLEDATA *IndexBuffer ) ;															// �C���f�b�N�X�o�b�t�@�n���h���̌�n��
extern	int		Graphics_Hardware_D3D11_IndexBuffer_SetData_PF(    INDEXBUFFERHANDLEDATA *IndexBuffer, int SetIndex, const void *IndexData, int IndexNum ) ;		// �C���f�b�N�X�o�b�t�@�ɃC���f�b�N�X�f�[�^��]������









// ���ˑ����C�g�֌W
extern	int		Graphics_Hardware_D3D11_Light_SetUse_PF( int Flag ) ;															// ���C�e�B���O���s�����ǂ�����ݒ肷��
extern	int		Graphics_Hardware_D3D11_Light_GlobalAmbient_PF( COLOR_F *Color ) ;												// �O���[�o���A���r�G���g���C�g�J���[��ݒ肷��
extern	int		Graphics_Hardware_D3D11_Light_SetState_PF( int LightNumber, LIGHTPARAM *LightParam ) ;							// ���C�g�p�����[�^���Z�b�g
extern	int		Graphics_Hardware_D3D11_Light_SetEnable_PF( int LightNumber, int EnableState ) ;									// ���C�g�̗L���A������ύX








// ���ˑ��V���h�E�}�b�v�֌W
extern	int		Graphics_Hardware_D3D11_ShadowMap_CreateTexture_PF( SHADOWMAPDATA *ShadowMap, int ASyncThread = FALSE ) ;		// �V���h�E�}�b�v�f�[�^�ɕK�v�ȃe�N�X�`�����쐬����
extern	int		Graphics_Hardware_D3D11_ShadowMap_ReleaseTexture_PF( SHADOWMAPDATA *ShadowMap ) ;								// �V���h�E�}�b�v�f�[�^�ɕK�v�ȃe�N�X�`�����������
extern	void	Graphics_Hardware_D3D11_ShadowMap_RefreshVSParam_PF( void ) ;													// ���_�V�F�[�_�[�ɐݒ肷��V���h�E�}�b�v�̏����X�V����
extern	void	Graphics_Hardware_D3D11_ShadowMap_RefreshPSParam_PF( void ) ;													// �s�N�Z���V�F�[�_�[�ɐݒ肷��V���h�E�}�b�v�̏����X�V����
extern	int		Graphics_Hardware_D3D11_ShadowMap_DrawSetup_PF( SHADOWMAPDATA *ShadowMap ) ;										// �V���h�E�}�b�v�ւ̕`��̏������s��
extern	int		Graphics_Hardware_D3D11_ShadowMap_DrawEnd_PF( SHADOWMAPDATA *ShadowMap ) ;										// �V���h�E�}�b�v�ւ̕`����I������
extern	int		Graphics_Hardware_D3D11_ShadowMap_SetUse_PF( int SlotIndex, SHADOWMAPDATA *ShadowMap ) ;							// �`��Ŏg�p����V���h�E�}�b�v���w�肷��A�X���b�g�͂O���P�����w��\








// ���ˑ��V�F�[�_�[�֌W
extern	int		Graphics_Hardware_D3D11_Shader_Create_PF( int ShaderHandle, int IsVertexShader, void *Image, int ImageSize, int ImageAfterFree, int ASyncThread ) ;		// �V�F�[�_�[�n���h��������������
extern	int		Graphics_Hardware_D3D11_Shader_TerminateHandle_PF( SHADERHANDLEDATA *Shader ) ;																			// �V�F�[�_�[�n���h���̌�n��
extern	int		Graphics_Hardware_D3D11_Shader_GetValidShaderVersion_PF( void ) ;																							// �g�p�ł���V�F�[�_�[�̃o�[�W�������擾����( 0=�g���Ȃ�  200=�V�F�[�_�[���f���Q�D�O���g�p�\  300=�V�F�[�_�[���f���R�D�O���g�p�\ )
extern	int		Graphics_Hardware_D3D11_Shader_GetConstIndex_PF( const wchar_t *ConstantName, SHADERHANDLEDATA *Shader ) ;													// �w��̖��O�����萔���g�p����V�F�[�_�[�萔�̔ԍ����擾����
extern	int		Graphics_Hardware_D3D11_Shader_GetConstCount_PF( const wchar_t *ConstantName, SHADERHANDLEDATA *Shader ) ;													// �w��̖��O�����萔���g�p����V�F�[�_�[�萔�̐����擾����
extern	const FLOAT4 *Graphics_Hardware_D3D11_Shader_GetConstDefaultParamF_PF( const wchar_t *ConstantName, SHADERHANDLEDATA *Shader ) ;									// �w��̖��O�������������_�萔�̃f�t�H���g�p�����[�^���i�[����Ă��郁�����A�h���X���擾����
extern	int		Graphics_Hardware_D3D11_Shader_SetConst_PF(   int TypeIndex, int SetIndex, int ConstantIndex, const void *Param, int ParamNum, int UpdateUseArea ) ;	// �V�F�[�_�[�萔����ݒ肷��
extern	int		Graphics_Hardware_D3D11_Shader_ResetConst_PF( int TypeIndex, int SetIndex, int ConstantIndex, int ParamNum ) ;											// �w��̈�̃V�F�[�_�[�萔�������Z�b�g����
extern	int		Graphics_Hardware_D3D11_Shader_ModelCode_Init_PF( void ) ;																								// �R�c���f���p�̃V�F�[�_�[�R�[�h�̏��������s��








// ���ˑ��V�F�[�_�[�p�萔�o�b�t�@�֌W
extern	int		Graphics_Hardware_D3D11_ShaderConstantBuffer_Create_PF( int ShaderConstantBufferHandle, int BufferSize, int ASyncThread ) ;												// �V�F�[�_�[�p�萔�o�b�t�@�n���h��������������
extern	int		Graphics_Hardware_D3D11_ShaderConstantBuffer_TerminateHandle_PF( SHADERCONSTANTBUFFERHANDLEDATA *ShaderConstantBuffer ) ;													// �V�F�[�_�[�p�萔�o�b�t�@�n���h���̌�n��
extern	void *	Graphics_Hardware_D3D11_ShaderConstantBuffer_GetBuffer_PF( SHADERCONSTANTBUFFERHANDLEDATA *ShaderConstantBuffer ) ;														// �V�F�[�_�[�p�萔�o�b�t�@�n���h���̒萔�o�b�t�@�̃A�h���X���擾����
extern	int		Graphics_Hardware_D3D11_ShaderConstantBuffer_Update_PF( SHADERCONSTANTBUFFERHANDLEDATA *ShaderConstantBuffer ) ;															// �V�F�[�_�[�p�萔�o�b�t�@�n���h���̒萔�o�b�t�@�ւ̕ύX��K�p����
extern	int		Graphics_Hardware_D3D11_ShaderConstantBuffer_Set_PF( SHADERCONSTANTBUFFERHANDLEDATA *ShaderConstantBuffer, int TargetShader /* DX_SHADERTYPE_VERTEX �Ȃ� */, int Slot ) ;	// �V�F�[�_�[�p�萔�o�b�t�@�n���h���̒萔�o�b�t�@���w��̃V�F�[�_�[�̎w��̃X���b�g�ɃZ�b�g����








// ���ˑ��`��֌W
extern	int		Graphics_Hardware_D3D11_Paint_PF( int x, int y, unsigned int FillColor, ULONGLONG BoundaryColor ) ;			// �w��_���狫�E�F������Ƃ���܂œh��Ԃ�






#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

#endif // DX_NON_DIRECT3D11

#endif // DX_NON_GRAPHICS

#endif // __DXGRAPHICSD3D11_H__

