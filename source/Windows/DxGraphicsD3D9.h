// -------------------------------------------------------------------------------
// 
// 		�c�w���C�u����		�`�揈���v���O����( Direct3D9 )�w�b�_�t�@�C��
// 
// 				Ver 3.16d
// 
// -------------------------------------------------------------------------------

#ifndef __DXGRAPHICSD3D9_H__
#define __DXGRAPHICSD3D9_H__

#include "../DxCompileConfig.h"

#ifndef DX_NON_GRAPHICS

#ifndef DX_NON_DIRECT3D9

// �C���N���[�h ------------------------------------------------------------------
#include "../DxLib.h"
#include "../DxGraphics.h"
#include "../DxArchive_.h"
#include "DxGraphicsAPIWin.h"

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// �}�N����` --------------------------------------------------------------------

#define GD3D9								GraphicsHardDataDirect3D9

// �f�o�C�X�񋓂̍ő吔
#define DX_D3D9_MAX_DEVICE_LISTUP			(32)

// Graphics_D3D9_DrawPreparation �֐��ɓn���t���O
#define DX_D3D9_DRAWPREP_TRANS				(0x00001)
#define DX_D3D9_DRAWPREP_VECTORINT			(0x00002)
#define DX_D3D9_DRAWPREP_GOURAUDSHADE		(0x00008)
#define DX_D3D9_DRAWPREP_PERSPECTIVE		(0x00010)
#define DX_D3D9_DRAWPREP_DIFFUSERGB			(0x00020)
#define DX_D3D9_DRAWPREP_DIFFUSEALPHA		(0x00040)
#define DX_D3D9_DRAWPREP_FOG				(0x00080)
#define DX_D3D9_DRAWPREP_NOBLENDSETTING		(0x00100)
#define DX_D3D9_DRAWPREP_LIGHTING			(0x00200)
#define DX_D3D9_DRAWPREP_SPECULAR			(0x00400)
#define DX_D3D9_DRAWPREP_3D					(0x00800)
#define DX_D3D9_DRAWPREP_TEXADDRESS			(0x01000)
#define DX_D3D9_DRAWPREP_NOTSHADERRESET		(0x02000)
#define DX_D3D9_DRAWPREP_CULLING			(0x04000)
#define DX_D3D9_DRAWPREP_TEXTURE			(0x08000)
#define DX_D3D9_DRAWPREP_TEXALPHACH			(0x10000)
#define DX_D3D9_DRAWPREP_TEXALPHATEST		(0x20000)
//#define DX_D3D9_DRAWPREP_EDGEFONT			(0x40000)

// �V�F�[�_�[�萔�Z�b�g�}�X�N
#define DX_SHADERCONSTANTSET_MASK_LIB		0x0001
#define DX_SHADERCONSTANTSET_MASK_LIB_SUB	0x0002
#define DX_SHADERCONSTANTSET_MASK_MV1		0x0004
#define DX_SHADERCONSTANTSET_MASK_USER		0x0008

// �s�N�Z���P�ʃ��C�e�B���O�^�C�v�̒��_�V�F�[�_�[���ʃR�[�h�쐬�p�}�N��
#define D3D9_PIXELLIGHTING_VERTEXSHADER_SHADOWMAP( use )		( ( use )  * 3 * 2 * 4 )
#define D3D9_PIXELLIGHTING_VERTEXSHADER_SKINMESH( use )			( ( use )  * 2 * 4 )
#define D3D9_PIXELLIGHTING_VERTEXSHADER_BUMPMAP( use )			( ( use )  * 4 )
#define D3D9_PIXELLIGHTING_VERTEXSHADER_FOGMODE( mode )			( ( mode ) )

// �s�N�Z���P�ʃ��C�e�B���O�^�C�v�̒��_�V�F�[�_�[���ʃR�[�h����e�v�f���擾���邽�߂̃}�N��
#define D3D9_PIXELLIGHTING_VERTEXSHADER_GET_SHADOWMAP( index )	( ( index ) / ( 3 * 2 * 4 ) % 2 )
#define D3D9_PIXELLIGHTING_VERTEXSHADER_GET_SKINMESH( index )	( ( index ) / ( 2 * 4 ) % 3 )
#define D3D9_PIXELLIGHTING_VERTEXSHADER_GET_BUMPMAP( index )	( ( index ) / ( 4 ) % 2 )
#define D3D9_PIXELLIGHTING_VERTEXSHADER_GET_FOGMODE( index )	( ( index ) % 4 )

// �s�N�Z���P�ʃ��C�e�B���O�^�C�v�̃s�N�Z���V�F�[�_�[�^�C�v
#define D3D9_PIXELLIGHTING_PIXELSHADER_TYPE_NORMAL				0	
#define D3D9_PIXELLIGHTING_PIXELSHADER_TYPE_TOON				1

// �s�N�Z���P�ʃ��C�e�B���O�^�C�v�̃s�N�Z���V�F�[�_�[���ʃR�[�h�쐬�p�}�N��
#define D3D9_PIXELLIGHTING_PIXELSHADER_TYPE( type )				( ( type  ) * 2 * 5 * 2 * 3 * 2 * 2 * 2 * 2 * 84 * 2 )
#define D3D9_PIXELLIGHTING_PIXELSHADER_SHADOWMAP( use )			( ( use   ) * 5 * 2 * 3 * 2 * 2 * 2 * 2 * 84 * 2 )
#define D3D9_PIXELLIGHTING_PIXELSHADER_MULTITEX( type )			( ( type  ) * 2 * 3 * 2 * 2 * 2 * 2 * 84 * 2 )
#define D3D9_PIXELLIGHTING_PIXELSHADER_TOONTYPE( type )			( ( type  ) * 3 * 2 * 2 * 2 * 2 * 84 * 2 )
#define D3D9_PIXELLIGHTING_PIXELSHADER_TOONSPHEREOP( type )		( ( type  ) * 2 * 2 * 2 * 2 * 84 * 2 )
#define D3D9_PIXELLIGHTING_PIXELSHADER_TOONDIFBLDOP( type )		( ( type  ) * 2 * 2 * 2 * 84 * 2 )
#define D3D9_PIXELLIGHTING_PIXELSHADER_TOONSPCBLDOP( type )		( ( type  ) * 2 * 2 * 84 * 2 )
#define D3D9_PIXELLIGHTING_PIXELSHADER_SPECULARMAP( use )		( ( use   ) * 2 * 84 * 2 )
#define D3D9_PIXELLIGHTING_PIXELSHADER_BUMPMAP( use )			( ( use   ) * 84 * 2 )
#define D3D9_PIXELLIGHTING_PIXELSHADER_LIGHTINDEX( index )		( ( index ) * 2 )
#define D3D9_PIXELLIGHTING_PIXELSHADER_SPECULAR( use )			( ( use   ) )

// �s�N�Z���P�ʃ��C�e�B���O�^�C�v�̃s�N�Z���V�F�[�_�[���ʃR�[�h����e�v�f���擾���邽�߂̃}�N��
#define D3D9_PIXELLIGHTING_PIXELSHADER_GET_TYPE( index )			( ( index ) / ( 2 * 5 * 2 * 3 * 2 * 2 * 2 * 2 * 84 * 2 ) % 2 )
#define D3D9_PIXELLIGHTING_PIXELSHADER_GET_SHADOWMAP( index )		( ( index ) / ( 5 * 2 * 3 * 2 * 2 * 2 * 2 * 84 * 2 ) % 2 )
#define D3D9_PIXELLIGHTING_PIXELSHADER_GET_MULTITEX( index )		( ( index ) / ( 2 * 3 * 2 * 2 * 2 * 2 * 84 * 2 ) % 5 )
#define D3D9_PIXELLIGHTING_PIXELSHADER_GET_TOONTYPE( index )		( ( index ) / ( 3 * 2 * 2 * 2 * 2 * 84 * 2 ) % 2 )
#define D3D9_PIXELLIGHTING_PIXELSHADER_GET_TOONSPHEREOP( index )	( ( index ) / ( 2 * 2 * 2 * 2 * 84 * 2 ) % 3 )
#define D3D9_PIXELLIGHTING_PIXELSHADER_GET_TOONDIFBLDOP( index )	( ( index ) / ( 2 * 2 * 2 * 84 * 2 ) % 2 )
#define D3D9_PIXELLIGHTING_PIXELSHADER_GET_TOONSPCBLDOP( index )	( ( index ) / ( 2 * 2 * 84 * 2 ) % 2 )
#define D3D9_PIXELLIGHTING_PIXELSHADER_GET_SPECULARMAP( index )		( ( index ) / ( 2 * 84 * 2 ) % 2 )
#define D3D9_PIXELLIGHTING_PIXELSHADER_GET_BUMPMAP( index )			( ( index ) / ( 84 * 2 ) % 2 )
#define D3D9_PIXELLIGHTING_PIXELSHADER_GET_LIGHTINDEX( index )		( ( index ) / ( 2 ) % 84 )
#define D3D9_PIXELLIGHTING_PIXELSHADER_GET_SPECULAR( index )		( ( index ) % 2 )

	// ���_�V�F�[�_�[�^�C�v
#define D3D9_VERTEXSHADER_TYPE_NORMAL					0
#define D3D9_VERTEXSHADER_TYPE_DRAW_SHADOWMAP			1
#define D3D9_VERTEXSHADER_TYPE_TOON_OUTLINE				2

// ���_�V�F�[�_�[���ʃR�[�h�쐬�p�}�N��
#define D3D9_VERTEXSHADER_TYPE( type )					( ( type  ) * 2 * 2 * 3 * 2 * 4 * 20 * 2 )
#define D3D9_VERTEXSHADER_SHADERMODEL( ver )			( ( ver   ) * 2 * 3 * 2 * 4 * 20 * 2 )
#define D3D9_VERTEXSHADER_SHADOWMAP( use )				( ( use   ) * 3 * 2 * 4 * 20 * 2 )
#define D3D9_VERTEXSHADER_MESHTYPE( type )				( ( type  ) * 2 * 4 * 20 * 2 )
#define D3D9_VERTEXSHADER_BUMPMAP( use )				( ( use   ) * 4 * 20 * 2 )
#define D3D9_VERTEXSHADER_FOGMODE( mode )				( ( mode  ) * 20 * 2 )
#define D3D9_VERTEXSHADER_LIGHTINDEX( index )			( ( index ) * 2 )
#define D3D9_VERTEXSHADER_SPECULAR( use )				( ( use   ) )

// ���_�V�F�[�_�[���ʃR�[�h����e�v�f���擾���邽�߂̃}�N��
#define D3D9_VERTEXSHADER_GET_TYPE( index )				( ( index ) / ( 2 * 2 * 3 * 2 * 4 * 20 * 2 ) % 3 )
#define D3D9_VERTEXSHADER_GET_SHADERMODEL( index )		( ( index ) / ( 2 * 3 * 2 * 4 * 20 * 2 ) % 2 )
#define D3D9_VERTEXSHADER_GET_SHADOWMAP( index )		( ( index ) / ( 3 * 2 * 4 * 20 * 2 ) % 2 )
#define D3D9_VERTEXSHADER_GET_MESHTYPE( index )			( ( index ) / ( 2 * 4 * 20 * 2 ) % 3 )
#define D3D9_VERTEXSHADER_GET_BUMPMAP( index )			( ( index ) / ( 4 * 20 * 2 ) % 2 )
#define D3D9_VERTEXSHADER_GET_FOGMODE( index )			( ( index ) / ( 20 * 2 ) % 4 )
#define D3D9_VERTEXSHADER_GET_LIGHTINDEX( index )		( ( index ) / ( 2 ) % 20 )
#define D3D9_VERTEXSHADER_GET_SPECULAR( index )			( ( index ) % 2 )

// �s�N�Z���V�F�[�_�[�^�C�v
#define D3D9_PIXELSHADER_TYPE_NORMAL					0
#define D3D9_PIXELSHADER_TYPE_DRAW_SHADOWMAP			1
#define D3D9_PIXELSHADER_TYPE_TOON_OUTLINE				2

// �s�N�Z���V�F�[�_�[���ʃR�[�h�쐬�p�}�N��
#define D3D9_PIXELSHADER_TYPE( type )					( ( type  ) * 2 * 2 * 5 * 2 * 2 * 3 * 2 * 2 * 2 * 2 * 10 * 2 )
#define D3D9_PIXELSHADER_SHADERMODEL( ver )				( ( ver   ) * 2 * 5 * 2 * 2 * 3 * 2 * 2 * 2 * 2 * 10 * 2 )
#define D3D9_PIXELSHADER_SHADOWMAP( use )				( ( use   ) * 5 * 2 * 2 * 3 * 2 * 2 * 2 * 2 * 10 * 2 )
#define D3D9_PIXELSHADER_MULTITEX( type )				( ( type  ) * 2 * 2 * 3 * 2 * 2 * 2 * 2 * 10 * 2 )
#define D3D9_PIXELSHADER_TOON( use )					( ( use   ) * 2 * 3 * 2 * 2 * 2 * 2 * 10 * 2 )
#define D3D9_PIXELSHADER_TOONTYPE( type )				( ( type  ) * 3 * 2 * 2 * 2 * 2 * 10 * 2 )
#define D3D9_PIXELSHADER_TOONSPHEREOP( type )			( ( type  ) * 2 * 2 * 2 * 2 * 10 * 2 )
#define D3D9_PIXELSHADER_TOONDIFBLDOP( type )			( ( type  ) * 2 * 2 * 2 * 10 * 2 )
#define D3D9_PIXELSHADER_TOONSPCBLDOP( type )			( ( type  ) * 2 * 2 * 10 * 2 )
#define D3D9_PIXELSHADER_SPECULARMAP( use )				( ( use   ) * 2 * 10 * 2 )
#define D3D9_PIXELSHADER_BUMPMAP( use )					( ( use   ) * 10 * 2 )
#define D3D9_PIXELSHADER_LIGHTINDEX( index )			( ( index ) * 2 )
#define D3D9_PIXELSHADER_SPECULAR( use )				( ( use ) )

// �s�N�Z���V�F�[�_�[���ʃR�[�h����e�v�f���擾���邽�߂̃}�N��
#define D3D9_PIXELSHADER_GET_TYPE( index )				( ( index ) / ( 2 * 2 * 5 * 2 * 2 * 3 * 2 * 2 * 2 * 2 * 10 * 2 ) % 3 )
#define D3D9_PIXELSHADER_GET_SHADERMODEL( index )		( ( index ) / ( 2 * 5 * 2 * 2 * 3 * 2 * 2 * 2 * 2 * 10 * 2 ) % 2 )
#define D3D9_PIXELSHADER_GET_SHADOWMAP( index )			( ( index ) / ( 5 * 2 * 2 * 3 * 2 * 2 * 2 * 2 * 10 * 2 ) % 2 )
#define D3D9_PIXELSHADER_GET_MULTITEX( index )			( ( index ) / ( 2 * 2 * 3 * 2 * 2 * 2 * 2 * 10 * 2 ) % 5 )
#define D3D9_PIXELSHADER_GET_TOON( index )				( ( index ) / ( 2 * 3 * 2 * 2 * 2 * 2 * 10 * 2 ) % 2 )
#define D3D9_PIXELSHADER_GET_TOONTYPE( index )			( ( index ) / ( 3 * 2 * 2 * 2 * 2 * 10 * 2 ) % 2 )
#define D3D9_PIXELSHADER_GET_TOONSPHEREOP( index )		( ( index ) / ( 2 * 2 * 2 * 2 * 10 * 2 ) % 3 )
#define D3D9_PIXELSHADER_GET_TOONDIFBLDOP( index )		( ( index ) / ( 2 * 2 * 2 * 10 * 2 ) % 2 )
#define D3D9_PIXELSHADER_GET_TOONSPCBLDOP( index )		( ( index ) / ( 2 * 2 * 10 * 2 ) % 2 )
#define D3D9_PIXELSHADER_GET_SPECULARMAP( index )		( ( index ) / ( 2 * 10 * 2 ) % 2 )
#define D3D9_PIXELSHADER_GET_BUMPMAP( index )			( ( index ) / ( 10 * 2 ) % 2 )
#define D3D9_PIXELSHADER_GET_LIGHTINDEX( index )		( ( index ) / ( 2 ) % 10 )
#define D3D9_PIXELSHADER_GET_SPECULAR( index )			( ( index ) % 2 )

// ���_�t�H�[�}�b�g
#define VERTEXFVF_2D_USER					( D_D3DFVF_XYZRHW | D_D3DFVF_DIFFUSE | D_D3DFVF_TEX1 )										// �g�����X�t�H�[���ςݒ��_�t�H�[�}�b�g
#define VERTEXFVF_NOTEX_2D					( D_D3DFVF_XYZRHW | D_D3DFVF_DIFFUSE )														// ���C���A�{�b�N�X�`��p���_�t�H�[�}�b�g
#define VERTEXFVF_2D						( D_D3DFVF_XYZRHW | D_D3DFVF_DIFFUSE | D_D3DFVF_TEX1 )										// �g�����X�t�H�[���ςݒ��_�t�H�[�}�b�g
#define VERTEXFVF_BLENDTEX_2D				( D_D3DFVF_XYZRHW | D_D3DFVF_DIFFUSE | D_D3DFVF_SPECULAR | D_D3DFVF_TEX2 )					// ���u�����h�e�N�X�`���t���g�����X�t�H�[���ςݒ��_�t�H�[�}�b�g
#define VERTEXFVF_SHADER_2D					( D_D3DFVF_XYZRHW | D_D3DFVF_DIFFUSE | D_D3DFVF_SPECULAR | D_D3DFVF_TEX2 )					// �V�F�[�_�[�`��p���_�t�H�[�}�b�g
#define VERTEXFVF_NOTEX_3D					( D_D3DFVF_XYZ | D_D3DFVF_DIFFUSE /*| D_D3DFVF_SPECULAR*/ )									// ���C���A�{�b�N�X�`��p���_�t�H�[�}�b�g�A�R�c�p( ���o�[�W�����p )
#define VERTEXFVF_3D						( D_D3DFVF_XYZ | D_D3DFVF_DIFFUSE /*| D_D3DFVF_SPECULAR*/ | D_D3DFVF_TEX1 )					// �O���t�B�b�N�X�`��p���_�t�H�[�}�b�g�A�R�c�p( ���o�[�W�����p )
#define VERTEXFVF_3D_LIGHT					( D_D3DFVF_XYZ | D_D3DFVF_NORMAL | D_D3DFVF_DIFFUSE | D_D3DFVF_SPECULAR | D_D3DFVF_TEX2 )	// �O���t�B�b�N�X�`��p���_�t�H�[�}�b�g
#define VERTEXFVF_SHADER_3D					( D_D3DFVF_XYZ | D_D3DFVF_NORMAL | D_D3DFVF_DIFFUSE | D_D3DFVF_SPECULAR | D_D3DFVF_TEX2 )	// �V�F�[�_�[�`��p���_�t�H�[�}�b�g
#define VERTEXFVF_TEX8_2D					( D_D3DFVF_XYZRHW | D_D3DFVF_DIFFUSE | D_D3DFVF_TEX8 )										// �g�����X�t�H�[���ςݒ��_�t�H�[�}�b�g

#define VERTEXFVF_DECL_2D_USER				(0)			// �g�����X�t�H�[���ςݒ��_�t�H�[�}�b�g
#define VERTEXFVF_DECL_NOTEX_2D				(1)			// ���C���A�{�b�N�X�`��p���_�t�H�[�}�b�g
#define VERTEXFVF_DECL_2D					(2)			// �g�����X�t�H�[���ςݒ��_�t�H�[�}�b�g
#define VERTEXFVF_DECL_BLENDTEX_2D			(3)			// ���u�����h�e�N�X�`���t���g�����X�t�H�[���ςݒ��_�t�H�[�}�b�g
#define VERTEXFVF_DECL_SHADER_2D			(4)			// �V�F�[�_�[�`��p���_�t�H�[�}�b�g
#define VERTEXFVF_DECL_NOTEX_3D				(5)			// ���C���A�{�b�N�X�`��p���_�t�H�[�}�b�g�A�R�c�p( ���o�[�W�����p )
#define VERTEXFVF_DECL_3D					(6)			// �O���t�B�b�N�X�`��p���_�t�H�[�}�b�g�A�R�c�p( ���o�[�W�����p )
#define VERTEXFVF_DECL_3D_LIGHT				(7)			// �O���t�B�b�N�X�`��p���_�t�H�[�}�b�g
#define VERTEXFVF_DECL_SHADER_3D			(8)			// �V�F�[�_�[�`��p���_�t�H�[�}�b�g
#define VERTEXFVF_DECL_TEX8_2D				(9)			// �o�C�L���[�r�b�N��ԗp���_�t�H�[�}�b�g
#define VERTEXFVF_DECL_NUM					(10)

// ���_�o�b�t�@�̃T�C�Y
#define D3DDEV_NOTEX_VERTBUFFERSIZE			(63 * 1024)		// 63KB
#define D3DDEV_TEX_VERTBUFFERSIZE			(64 * 1024)		// 64KB
#define D3DDEV_BLENDTEX_VERTBUFFERSIZE		(64 * 1024)		// 64KB
#define D3DDEV_NOTEX_3D_VERTBUFFERSIZE		(64 * 1024)		// 64KB
#define D3DDEV_TEX_3D_VERTBUFFERSIZE		(64 * 1024)		// 64KB
#define D3DDEV_NOTEX_VERTMAXNUM				(D3DDEV_NOTEX_VERTBUFFERSIZE     / sizeof( VERTEX_NOTEX_2D    ))
#define D3DDEV_TEX_VERTMAXNUM				(D3DDEV_TEX_VERTBUFFERSIZE       / sizeof( VERTEX_2D          ))
#define D3DDEV_BLENDTEX_VERTMAXNUM			(D3DDEV_BLENDTEX_VERTBUFFERSIZE  / sizeof( VERTEX_BLENDTEX_2D ))
#define D3DDEV_NOTEX_3D_VERTMAXNUM			(D3DDEV_NOTEX_3D_VERTBUFFERSIZE  / sizeof( VERTEX_NOTEX_3D    ))
#define D3DDEV_TEX_3D_VERTMAXNUM			(D3DDEV_TEX_3D_VERTBUFFERSIZE    / sizeof( VERTEX_3D          ))

// �v���~�e�B�u�̐����Z�o
#define GETPRIMNUM( primtype, vertnum )	\
	( DWORD )( \
	( ( (primtype) == D_D3DPT_TRIANGLELIST  ) ? (vertnum) / 3 :\
    ( ( (primtype) == D_D3DPT_LINELIST      ) ? (vertnum) / 2 :\
	( ( (primtype) == D_D3DPT_TRIANGLEFAN   ) ? (vertnum) - 2 :\
	( ( (primtype) == D_D3DPT_TRIANGLESTRIP ) ? (vertnum) - 2 :\
	( ( (primtype) == D_D3DPT_LINESTRIP     ) ? (vertnum) - 1 :\
	( ( (primtype) == D_D3DPT_POINTLIST     ) ? (vertnum)     : 0 ) ) ) ) ) ) )


// ��Ǘ��e�N�X�`���ւ̃f�[�^�]���p�V�X�e���������z�u�e�N�X�`���̐�
#define DIRECT3D9_SYSMEMTEXTURE_NUM					(2048)

// ��Ǘ��e�N�X�`���ւ̃f�[�^�]���p�V�X�e���������z�u�T�[�t�F�X�̐�
#define DIRECT3D9_SYSMEMSURFACE_NUM					(2048)


// �\���̒�` --------------------------------------------------------------------

// Direct3D9�p �n�[�h�E�G�A�����_�����O�p�I���W�i���摜�e�N�X�`�����
struct IMAGEDATA_ORIG_HARD_TEX_DIRECT3D9
{
	union
	{
		D_IDirect3DCubeTexture9	*	CubeTexture ;							// �L���[�u�e�N�X�`��
		D_IDirect3DTexture9		*	Texture ;								// �e�N�X�`��
	} ;
//	D_IDirect3DTexture9	*			MemTexture ;							// D3DPOOL_MANAGED ���g�p�ł��Ȃ��摜�p�������e�N�X�`��
	D_IDirect3DSurface9	*			Surface[ CUBEMAP_SURFACE_NUM ] ;		// �T�[�t�F�X
	D_IDirect3DSurface9	*			RenderTargetSurface ;					// �����_�����O�^�[�Q�b�g�T�[�t�F�X
	D_IDirect3DSurface9	*			ZBuffer ;								// �y�o�b�t�@
} ;

// Direct3D9�p �V���h�E�}�b�v���
struct SHADOWMAPDATA_DIRECT3D9
{
	D_IDirect3DTexture9		*Texture ;										// �e�N�X�`��

	D_IDirect3DSurface9		*Surface ;										// �T�[�t�F�X
	D_IDirect3DSurface9		*ZBufferSurface ;								// �y�o�b�t�@�T�[�t�F�X
} ;

// Direct3D9�p ���_�o�b�t�@�n���h�����
struct VERTEXBUFFERHANDLEDATA_DIRECT3D9
{
	D_IDirect3DVertexBuffer9	*VertexBuffer ;								// IDirect3DVertexBuffer9 �̃|�C���^
} ;

// Direct3D9�p �C���f�b�N�X�o�b�t�@�n���h�����
struct INDEXBUFFERHANDLEDATA_DIRECT3D9
{
	D_IDirect3DIndexBuffer9		*IndexBuffer ;								// IDirect3DIndexBuffer9 �̃|�C���^
} ;

// Direct3D9�p �V�F�[�_�[�n���h�����
struct SHADERHANDLEDATA_DIRECT3D9
{
	union
	{
		D_IDirect3DPixelShader9			*PixelShader ;						// IDirect3DPixelShader9 �̃|�C���^
		D_IDirect3DVertexShader9		*VertexShader ;						// IDirect3DVertexShader9 �̃|�C���^
	} ;

	int									ConstantNum ;						// �萔�̐�
	D_D3DXSHADER_CONSTANTINFO			*ConstantInfo ;						// �萔���z��̐擪�A�h���X
} ;

// Direct3D9�p �V�F�[�_�[�p�萔�o�b�t�@�n���h�����
struct SHADERCONSTANTBUFFERHANDLEDATA_DIRECT3D9
{
	int			Dummy ;
} ;

// �V�F�[�_�[�萔�̈�̎g�p�̈���\����
struct DIRECT3D9_SHADERCONST_ONE_USEAREA
{
	WORD								Start ;								// �g�p�̈�̊J�n�ԍ�
	WORD								EndPlusOne ;						// �g�p�̈�̏I���ԍ��{�P
} ;

// �V�F�[�_�[�萔�g�p�̈���\����
struct DIRECT3D9_SHADERCONSTANT_USEAREA
{
	DIRECT3D9_SHADERCONST_ONE_USEAREA	AreaInfo[ 256 ] ;					// �g�p�̈���
	int									AreaInfoNum ;						// �g�p�̈���̐�
	int									TotalSize ;							// �Ǘ�����̈�̃T�C�Y
} ;

// �V�F�[�_�[�萔���\����
struct DIRECT3D9_SHADERCONSTANTINFO
{
	DIRECT3D9_SHADERCONSTANT_USEAREA	UseArea ;							// ���[�U�[�p���_�V�F�[�_�[�萔�̎g�p�̈���
	int									UnitSize ;							// �f�[�^�������̃T�C�Y

	union
	{
		BYTE							Data[ 256 * 16 ] ;					// �T�C�Y�ۏؗp�f�[�^

		FLOAT4							Float4[ 256 ] ;						// ���[�U�[�p���_�V�F�[�_�[ float �^�萔
		INT4							Int4[ 16 ] ;						// ���[�U�[�p���_�V�F�[�_�[ int �^�萔
		BOOL							Bool[ 16 ] ;						// ���[�U�[�p���_�V�F�[�_�[ BOOL �^�萔
	} ;
} ;

// �V�F�[�_�[�萔���Z�b�g�\����
struct DIRECT3D9_SHADERCONSTANTINFOSET
{
	int									IsApply[ DX_SHADERCONSTANTSET_NUM ] ;			// �萔����K�p���邩�ǂ���( TRUE:�K�p����  FALSE:�K�p���Ȃ� )
	int									ApplyMask ;										// �K�p�}�X�N( DX_SHADERCONSTANTSET_MASK_LIB | DX_SHADERCONSTANTSET_MASK_LIB_SUB �� )
	DIRECT3D9_SHADERCONSTANTINFO		Info[ DX_SHADERCONSTANTTYPE_NUM ][ DX_SHADERCONSTANTSET_NUM ] ;				// �萔������
	BYTE								UseMap[ DX_SHADERCONSTANTTYPE_NUM ][ DX_SHADERCONSTANTSET_NUM ][ 256 ] ;	// �萔�g�p�}�b�v

	DIRECT3D9_SHADERCONSTANTINFO		FixInfo[ DX_SHADERCONSTANTTYPE_NUM ] ;			// ���ۂɃV�F�[�_�[�ɓK�p����Ă���萔���

	BYTE								SetMap[ DX_SHADERCONSTANTTYPE_NUM ][ 256 ] ;	// �V�F�[�_�[�萔�̎g�p���Z�b�g�}�b�v( DX_SHADERCONSTANTSET_LIB ���A0xff ���g�p )
} ;


// Direct3D9 �̂Q�c�`��p�̃V�F�[�_�[��Z�߂��\����
struct GRAPHICS_HARDWARE_DIRECT3D9_SHADER_2D
{
#ifndef DX_NON_NORMAL_DRAW_SHADER
	// �Œ�@�\�p�C�v���C���݊��p�̒��_�V�F�[�_�[
//	D_IDirect3DVertexShader9				*Base2DVertexShader ;
//	D_IDirect3DVertexShader9				*Base3DVertexShader ;

	// �Œ�@�\�p�C�v���C���݊��̃V�F�[�_�[( �e�N�X�`���Ȃ� )
	// [ �������( 0:�ʏ�`��  1:��Z�`��  2:RGB���]  3:�`��P�x�S�{  4:��Z�ς݃��u�����h���[�h�̒ʏ�`��  5:��Z�ς݃��u�����h���[�h��RGB���]  6:��Z�ς݃��u�����h���[�h�̕`��P�x4�{ ) ]
	// [ ���`�����l���l���ł��ǂ���( 0:�l�����Ȃ�  1:�l������ ) ]
	// [ ��Z�ς݃��u�����h�p���ǂ���( 0:��Z�ς݃��u�����h�p�ł͂Ȃ�  1:��Z�ς݃��u�����h�p ) ]
	D_IDirect3DPixelShader9					*BaseNoneTexPixelShader[ 7 ][ 2 ] ;

	// �Œ�@�\�p�C�v���C���݊��̃V�F�[�_�[( �e�N�X�`������ )
	// [ �u�����h�摜�Ƃ̃u�����h�^�C�v( 0:�Ȃ�  1:DX_BLENDGRAPHTYPE_NORMAL  2:DX_BLENDGRAPHTYPE_WIPE  3:DX_BLENDGRAPHTYPE_ALPHA ) ]
	// [ �������( 0:�ʏ�`��  1:��Z�`��  2:RGB���]  3:�`��P�x�S�{  4:��Z�ς݃��u�����h���[�h�̒ʏ�`��  5:��Z�ς݃��u�����h���[�h��RGB���]  6:��Z�ς݃��u�����h���[�h�̕`��P�x4�{ ) ]
	// [ �e�N�X�`��RGB����( 0:�������Ȃ�  1:�������� ) ]
	// [ �e�N�X�`��Alpha����( 0:�������Ȃ�  1:�������� ) ]
	// [ ���`�����l���l���ł��ǂ���( 0:�l�����Ȃ�  1:�l������ ) ]
	D_IDirect3DPixelShader9					*BaseUseTexPixelShader[ 4 ][ 7 ][ 2 ][ 2 ][ 2 ] ;

	// �}�X�N�����p�̃V�F�[�_�[
	D_IDirect3DPixelShader9					*MaskEffectPixelShader ;
	D_IDirect3DPixelShader9					*MaskEffect_UseGraphHandle_PixelShader[ 4 ] ;
	D_IDirect3DPixelShader9					*MaskEffect_UseGraphHandle_ReverseEffect_PixelShader[ 4 ] ;
#endif // DX_NON_NORMAL_DRAW_SHADER
} ;


#ifndef DX_NON_MODEL

// Direct3D9 �̃��f���`��p�̃V�F�[�_�[��Z�߂��\����
struct GRAPHICS_HARDWARE_DIRECT3D9_SHADER_MODEL
{
	// ���f���`��p�s�N�Z���P�ʃ��C�e�B���O�^�C�v�̒��_�V�F�[�_�[( ���X�g�͔z��̍����� )
	// [ �V���h�E�}�b�v�̗L��( 0:���� 1:�L�� ) ]
	// [ ���b�V���^�C�v( 0:���̃��b�V�� 1:4�{�[�����X�L�j���O���b�V�� 2:8�{�[�����X�L�j���O���b�V�� ) ]
	// [ �o���v�}�b�v�̗L��( 0:���� 1:�L�� ) ]
	// [ �t�H�O�^�C�v ]
	D_IDirect3DVertexShader9				*MV1_PixelLighting_VertexShader[ 2 ][ 3 ][ 2 ][ 4 ] ;

	// ���f���`��p�s�N�Z���P�ʃ��C�e�B���O�^�C�v�̃g�D�[���p�s�N�Z���V�F�[�_�[( ���X�g�͔z��̍����� )
	// [ �V���h�E�}�b�v�̗L��( 0:���� 1:�L�� ) ]
	// [ �g�D�[�������_�����O�^�C�v( 0:�g�D�[�������_�����O type 1   1:�g�D�[�������_�����O Type 2 ) ]
	// [ �g�D�[�������_�����O�̃X�t�B�A�}�b�v�̗L���ƃu�����h�^�C�v( 0:�X�t�B�A�}�b�v�͖���   1:MV1_LAYERBLEND_TYPE_MODULATE  2:MV1_LAYERBLEND_TYPE_ADDITIVE ) ]
	// [ �g�D�[�������_�����O�̃f�B�t���[�Y�O���f�[�V�����̃u�����h�^�C�v( 0:MV1_LAYERBLEND_TYPE_TRANSLUCENT  1:MV1_LAYERBLEND_TYPE_MODULATE ) ]
	// [ �g�D�[�������_�����O�̃X�y�L�����O���f�[�V�����̃u�����h�^�C�v( 0:MV1_LAYERBLEND_TYPE_TRANSLUCENT  1:MV1_LAYERBLEND_TYPE_ADDITIVE ) ]
	// [ �X�y�L�����}�b�v ( 0:���� 1:���� ) ]
	// [ �o���v�}�b�v ( 0:�Ȃ� 1:���� ) ]
	// [ ���C�g�C���f�b�N�X ]
	// [ �X�y�L�����̗L��( 0:���� 1:�L�� ) ]
	D_IDirect3DPixelShader9					*MV1_PixelLighting_ToonPixelShader[ 2 ][ 2 ][ 3 ][ 2 ][ 2 ][ 2 ][ 2 ][ 84 ][ 2 ] ;

	// ���f���`��p�s�N�Z���P�ʃ��C�e�B���O�^�C�v�̃s�N�Z���V�F�[�_�[( ���X�g�͔z��̍����� )
	// [ �V���h�E�}�b�v�̗L��( 0:���� 1:�L�� ) ]
	// [ �}���`�e�N�X�`���̃u�����h���[�h�AMV1_LAYERBLEND_TYPE_TRANSLUCENT �Ȃǂ� +1 �������� ( 0:�}���`�e�N�X�`������  1:���u�����h  2:���Z�u�����h  3:��Z�u�����h  4:��Z�u�����h�~2 ) ]
	// [ �X�y�L�����}�b�v ( 0:���� 1:���� ) ]
	// [ �o���v�}�b�v ( 0:�Ȃ� 1:���� ) ]
	// [ ���C�g�C���f�b�N�X ]
	// [ �X�y�L�����̗L��( 0:���� 1:�L�� ) ]
	D_IDirect3DPixelShader9					*MV1_PixelLighting_PixelShader[ 2 ][ 5 ][ 2 ][ 2 ][ 84 ][ 2 ] ;





	// ���f���`��p�̃g�D�[�������_�����O�̗֊s���`��p���_�V�F�[�_�[
	// [ ���b�V���^�C�v( 0:���̃��b�V�� 1:4�{�[�����X�L�j���O���b�V��  2:8�{�[�����X�L�j���O���b�V�� ) ]
	// [ �t�H�O�^�C�v ]
	D_IDirect3DVertexShader9				*MV1_ToonOutLineVertexShader[ 3 ][ 4 ] ;

	// ���f���`��p�̃V���h�E�}�b�v�ւ̃����_�����O�p���_�V�F�[�_�[
	// [ ���b�V���^�C�v( 0:���̃��b�V�� 1:4�{�[�����X�L�j���O���b�V��  2:8�{�[�����X�L�j���O���b�V�� ) ]
	D_IDirect3DVertexShader9				*MV1_ShadowMapVertexShader[ 3 ] ;

	// ���f���`��p�̃��C�e�B���O�Ȃ����_�V�F�[�_�[
	// [ ���b�V���^�C�v( 0:���̃��b�V�� 1:4�{�[�����X�L�j���O���b�V��  2:8�{�[�����X�L�j���O���b�V�� ) ]
	// [ �t�H�O�^�C�v ]
	D_IDirect3DVertexShader9				*MV1_NoLightingVertexShader[ 3 ][ 4 ] ;

	// ���f���`��p�̃��C�e�B���O���蒸�_�V�F�[�_�[
	// [ �V�F�[�_�[���f��( 0:SM2  1:SM3 )
	// [ �V���h�E�}�b�v�̗L��( 0:���� 1:�L�� ) ]
	// [ ���b�V���^�C�v( 0:���̃��b�V�� 1:4�{�[�����X�L�j���O���b�V��  2:8�{�[�����X�L�j���O���b�V�� ) ]
	// [ �o���v�}�b�v�̗L��( 0:���� 1:�L�� ) ]
	// [ �t�H�O�^�C�v ]
	// [ ���C�g�C���f�b�N�X ]
	// [ �X�y�L�����̗L��( 0:���� 1:�L�� ) ]
	D_IDirect3DVertexShader9				*MV1_LightingVertexShader[ 2 ][ 2 ][ 3 ][ 2 ][ 4 ][ 20 ][ 2 ] ;


	// ���f���`��p�̃g�D�[�������_�����O�̗֊s���`��p�s�N�Z���V�F�[�_�[
	D_IDirect3DPixelShader9					*MV1_ToonOutLinePixelShader ;

	// ���f���`��p�̃V���h�E�}�b�v�ւ̃g�D�[�������_�����O�p�s�N�Z���V�F�[�_�[
	// [ �g�D�[�������_�����O�^�C�v( 0:�g�D�[�������_�����O type 1   1:�g�D�[�������_�����O type 2 ) ]
	// [ �g�D�[�������_�����O�̃X�t�B�A�}�b�v�̗L���ƃu�����h�^�C�v( 0:�X�t�B�A�}�b�v�͖���   1:MV1_LAYERBLEND_TYPE_MODULATE  2:MV1_LAYERBLEND_TYPE_ADDITIVE ) ]
	// [ �g�D�[�������_�����O�̃f�B�t���[�Y�O���f�[�V�����̃u�����h�^�C�v( 0:MV1_LAYERBLEND_TYPE_TRANSLUCENT  1:MV1_LAYERBLEND_TYPE_MODULATE ) ]
	D_IDirect3DPixelShader9					*MV1_ShadowMapToonPixelShader[ 2 ][ 3 ][ 2 ] ;

	// ���f���`��p�̃V���h�E�}�b�v�ւ̃����_�����O�p�s�N�Z���V�F�[�_�[
	D_IDirect3DPixelShader9					*MV1_ShadowMapPixelShader ;

	// ���f���`��p�̃��C�e�B���O�Ȃ��g�D�[���p�s�N�Z���V�F�[�_�[( ���X�g�͔z��̍����� )
	// [ �g�D�[�������_�����O�^�C�v( 0:�g�D�[�������_�����O type 1   1:�g�D�[�������_�����O type 2 ) ]
	// [ �g�D�[�������_�����O�̃f�B�t���[�Y�O���f�[�V�����̃u�����h�^�C�v( 0:MV1_LAYERBLEND_TYPE_TRANSLUCENT  1:MV1_LAYERBLEND_TYPE_MODULATE ) ]
	D_IDirect3DPixelShader9					*MV1_ToonNoLightingPixelShader[ 2 ][ 2 ] ;

	// ���f���`��p�̃��C�e�B���O�Ȃ��s�N�Z���V�F�[�_�[
	// [ �}���`�e�N�X�`���̃u�����h���[�h�AMV1_LAYERBLEND_TYPE_TRANSLUCENT �Ȃǂ� +1 �������� ( 0:�}���`�e�N�X�`������  1:���u�����h  2:���Z�u�����h  3:��Z�u�����h  4:��Z�u�����h�~2 ) ]
	D_IDirect3DPixelShader9					*MV1_NoLightingPixelShader[ 5 ] ;

	// ���f���`��p�̃��C�e�B���O����g�D�[���p�s�N�Z���V�F�[�_�[
	// [ �V�F�[�_�[���f��( 0:SM2  1:SM3 )
	// [ �V���h�E�}�b�v�̗L��( 0:���� 1:�L�� ) ]
	// [ �g�D�[�������_�����O�^�C�v( 0:�g�D�[�������_�����O type 1   1:�g�D�[�������_�����O type 2 ) ]
	// [ �g�D�[�������_�����O�̃X�t�B�A�}�b�v�̗L���ƃu�����h�^�C�v( 0:�X�t�B�A�}�b�v�͖���   1:MV1_LAYERBLEND_TYPE_MODULATE  2:MV1_LAYERBLEND_TYPE_ADDITIVE ) ]
	// [ �g�D�[�������_�����O�̃f�B�t���[�Y�O���f�[�V�����̃u�����h�^�C�v( 0:MV1_LAYERBLEND_TYPE_TRANSLUCENT  1:MV1_LAYERBLEND_TYPE_MODULATE ) ]
	// [ �g�D�[�������_�����O�̃X�y�L�����O���f�[�V�����̃u�����h�^�C�v( 0:MV1_LAYERBLEND_TYPE_TRANSLUCENT  1:MV1_LAYERBLEND_TYPE_ADDITIVE ) ]
	// [ �X�y�L�����}�b�v ( 0:���� 1:���� ) ]
	// [ �o���v�}�b�v ( 0:�Ȃ� 1:���� ) ]
	// [ ���C�g�C���f�b�N�X ]
	// [ �X�y�L�����̗L��( 0:���� 1:�L�� ) ]
	D_IDirect3DPixelShader9					*MV1_ToonLightingPixelShader[ 2 ][ 2 ][ 2 ][ 3 ][ 2 ][ 2 ][ 2 ][ 2 ][ 10 ][ 2 ] ;

	// ���f���`��p�̃��C�e�B���O����s�N�Z���V�F�[�_�[
	// [ �V�F�[�_�[���f��( 0:SM2  1:SM3 )
	// [ �V���h�E�}�b�v�̗L��( 0:���� 1:�L�� ) ]
	// [ �}���`�e�N�X�`���̃u�����h���[�h�AMV1_LAYERBLEND_TYPE_TRANSLUCENT �Ȃǂ� +1 �������� ( 0:�}���`�e�N�X�`������  1:���u�����h  2:���Z�u�����h  3:��Z�u�����h  4:��Z�u�����h�~2 ) ]
	// [ �X�y�L�����}�b�v ( 0:���� 1:���� ) ]
	// [ �o���v�}�b�v ( 0:�Ȃ� 1:���� ) ]
	// [ ���C�g�C���f�b�N�X ]
	// [ �X�y�L�����̗L��( 0:���� 1:�L�� ) ]
	D_IDirect3DPixelShader9					*MV1_LightingPixelShader[ 2 ][ 2 ][ 5 ][ 2 ][ 2 ][ 10 ][ 2 ] ;
} ;
#endif // DX_NON_MODEL


// Direct3D9 �̃V�F�[�_�[����Z�߂��\����
struct GRAPHICS_HARDWARE_DIRECT3D9_SHADER
{
//	int											UseShader ;						// �v���O���}�u���V�F�[�_�[���g�p���邩�ǂ���( TRUE:�g�p����  FALSE:�g�p���Ȃ� )

	DWORD										NativeVertexShaderVersion ;		// �G�~�����[�V���������̒��_�V�F�[�_�[�̃o�[�W����

	int											ValidVertexShader ;				// ���_�V�F�[�_�[���g�p�\���ǂ���( TRUE:�g�p�\  FALSE:�s�\ )
	int											ValidPixelShader ;				// �s�N�Z���V�F�[�_�[���g�p�\���ǂ���( TRUE:�g�p�\  FALSE:�s�\ )
	int											ValidVertexShader_SM3 ;			// Shader Model 3.0 �̒��_�V�F�[�_�[���g�p�\���ǂ���( TRUE:�g�p�\  FALSE:�s�\ )
	int											ValidPixelShader_SM3 ;			// Shader Model 3.0 �̃s�N�Z���V�F�[�_�[���g�p�\���ǂ���( TRUE:�g�p�\  FALSE:�s�\ )

	DIRECT3D9_SHADERCONSTANTINFOSET				ShaderConstantInfo ;			// �V�F�[�_�[�̒萔���

	GRAPHICS_HARDWARE_DIRECT3D9_SHADER_2D		_2D ;							// �Q�c�`�揈���p�̃V�F�[�_�[��Z�߂��\����

#ifndef DX_NON_MODEL
	int											UseBaseVertexShaderIndex ;		// �g�p���钸�_�V�F�[�_�[�̃��C�g�E�t�H�O�E�t�H���V�F�[�f�B���O�̗L���̂ݐݒ肵���l
	int											UseBasePixelShaderIndex ;		// �g�p����s�N�Z���V�F�[�_�[�̃��C�g�E�t�H���V�F�[�f�B���O�̗L���̂ݐݒ肵���l
	int											UseBaseVertexShaderIndex_PL ;	// �s�N�Z���P�ʃ��C�e�B���O�^�C�v�Ŏg�p���钸�_�V�F�[�_�[�̃��C�g�E�t�H�O�E�t�H���V�F�[�f�B���O�̗L���̂ݐݒ肵���l
	int											UseBasePixelShaderIndex_PL ;	// �s�N�Z���P�ʃ��C�e�B���O�^�C�v�Ŏg�p����s�N�Z���V�F�[�_�[�̃��C�g�E�t�H���V�F�[�f�B���O�̗L���̂ݐݒ肵���l
	int											UseOnlyPixelLightingTypeCode ;	// �s�N�Z���P�ʃ��C�e�B���O�^�C�v�̃V�F�[�_�[�R�[�h���g�p����w��ɂȂ��Ă��邩�ǂ���

	GRAPHICS_HARDWARE_DIRECT3D9_SHADER_MODEL	Model ;							// ���f���`�揈���p�̃V�F�[�_�[��Z�߂��\����
#endif // DX_NON_MODEL
} ;



// Direct3D9 �̒��_�V�F�[�_�錾��Z�߂��\����
struct GRAPHICS_HARDWARE_DIRECT3D9_VERTEXDECLARATION
{
	// �Œ�p�C�v���C���݊��V�F�[�_�[�Ŏg�p���钸�_�f�[�^�t�H�[�}�b�g
	D_IDirect3DVertexDeclaration9 *				Base2DDeclaration[ VERTEXFVF_DECL_NUM ] ;

	// ���[�U�[�V�F�[�_���[�h�Ŏg�p���钸�_�f�[�^�t�H�[�}�b�g
	D_IDirect3DVertexDeclaration9 *				UserShaderDeclaration[ DX_VERTEX_TYPE_NUM ] ;

#ifndef DX_NON_MODEL
	// ���_�V�F�[�_�錾( ���X�g�͔z��̍����� )
	// [ �o���v�}�b�v���t�����ǂ���( 1:�o���v�}�b�v�t�� 0:�t���ĂȂ� ) ]
	// [ �X�L�j���O���b�V�����ǂ���( 0:���̃��b�V�� 1:4�{�[�����X�L�j���O���b�V�� 2:8�{�[�����X�L�j���O���b�V�� ) ]
	// [ UV�̐� ]
	D_IDirect3DVertexDeclaration9 *				MV1_VertexDeclaration[ 2 ][ 3 ][ 9 ] ;
#endif // DX_NON_MODEL
} ;



// �e�N�X�`���X�e�[�W�̏��
struct GRAPHICS_HARDDATA_DIRECT3D9_TEXTURESTAGEINFO
{
	void *						Texture ;							// �X�e�[�W�ɃZ�b�g����Ă���e�N�X�`���T�[�t�F�X
	int							TextureCoordIndex ;					// �g�p����e�N�X�`�����W�C���f�b�N�X
	int							ResultTempARG ;						// �o�͐惌�W�X�^���e���|�����ɂ��邩�ǂ���
	short						AlphaARG1, AlphaARG2, AlphaOP ;		// Direct3DDevice �� D3DTSS_ALPHAOP �� D3DTSS_ALPHAARG1 �� D3DTSS_ALPHAARG2 �̐ݒ�l
	short						ColorARG1, ColorARG2, ColorOP ;		// Direct3DDevice �� D3DTSS_COLOROP �� D3DTSS_COLORARG1 �� D3DTSS_COLORARG2 �̐ݒ�l
} ;

// �c�����������R�c �̃u�����h�ݒ�Ɋւ�ݒ�̏��
struct GRAPHICS_HARDDATA_DIRECT3D9_BLENDINFO
{
	int							AlphaTestEnable ;							// Direct3DDevice9 �� D3DRS_ALPHATESTENABLE �̐ݒ�l
	int							AlphaRef ;									// Direct3DDevice9 �� D3DRS_ALPHAREF �̐ݒ�l
	int							AlphaFunc ;									// Direct3DDevice9 �� D3DRS_ALPHAFUNC �̐ݒ�l
	int							AlphaBlendEnable ;							// Direct3DDevice9 �� D3DRS_ALPHABLENDENABLE �̐ݒ�l
	int							SeparateAlphaBlendEnable ;					// Direct3DDevice9 �� D3DRS_SEPARATEALPHABLENDENABLE �̐ݒ�l

	DWORD						FactorColor ;								// Direct3DDevice9 �� D3DRS_TEXTUREFACTOR �̐ݒ�l
	int							SrcBlend, DestBlend ;						// Direct3DDevice9 �� D3DRS_DESTBLEND �� D3DRS_SRCBLEND �̐ݒ�l
	int							BlendOp ;									// Direct3DDevice9 �� D3DRS_BLENDOP �̐ݒ�l
	int							SrcBlendAlpha, DestBlendAlpha ;				// Direct3DDevice9 �� D3DRS_DESTBLENDALPHA �� D3DRS_SRCBLENDALPHA �̐ݒ�l
	int							BlendOpAlpha ;								// Direct3DDevice9 �� D3DRS_BLENDOPALPHA �̐ݒ�l
	int							UseTextureStageNum ;						// �g�p���Ă���e�N�X�`���X�e�[�W�̐�
	GRAPHICS_HARDDATA_DIRECT3D9_TEXTURESTAGEINFO TextureStageInfo[ USE_TEXTURESTAGE_NUM] ;	// Direct3DDevice �̃e�N�X�`���X�e�[�W�ݒ�l( Direct3DDevice �ɂ��ꂩ��K������\��̐ݒ�l )
} ;

// Direct3D9Device �ɐݒ肵�Ă���l��Z�߂��\����
struct GRAPHICS_HARDDATA_DIRECT3D9_DEVICE_STATE
{
	GRAPHICS_HARDDATA_DIRECT3D9_BLENDINFO	BlendInfo ;										// �u�����h�֌W�̐ݒ���
	int										CullMode ;										// �J�����O���[�h
	int										DepthBias ;										// �[�x�l�o�C�A�X
	int										DrawMode ;										// �`�惂�[�h
	float									FactorColorPSConstantF[ 4 ] ;					// �s�N�Z���V�F�[�_�[���g�����c�w���C�u�����W�������p�� FactorColor �̒l
	int										FillMode ;										// �t�B�����[�h
	int										FogEnable ;										// �t�H�O���L�����ǂ���( TRUE:�L��  FALSE:���� )
	int										FogMode ;										// �t�H�O���[�h
	DWORD									FogColor ;										// �t�H�O�J���[
	float									FogStart ;										// �t�H�O�J�n����
	float									FogEnd ;										// �t�H�O�I������
	float									FogDensity ;									// �t�H�O���x
	COLOR_F									GlobalAmbientLightColor ;						// �O���[�o���A���r�G���g���C�g�J���[
	int										LightEnableMaxIndex ;							// �L���ȃ��C�g�̍ő�C���f�b�N�X
	int										LightEnableFlag[ 256 ] ;						// ���C�g���L�����ǂ����t���O( TRUE:�L��  FALSE:���� )
	D_D3DLIGHT9								LightParam[ 256 ] ;								// ���C�g�̃p�����[�^
	int										Lighting ;										// ���C�g���g�p���邩�t���O
	D_D3DMATERIAL9							Material ;										// �}�e���A��
	int										MaterialUseVertexDiffuseColor ;					// ���_�̃f�B�t���[�Y�J���[���}�e���A���̃f�B�t���[�Y�J���[�Ƃ��Ďg�p���邩�ǂ���
	int										MaterialUseVertexSpecularColor ;				// ���_�̃X�y�L�����J���[���}�e���A���̃X�y�L�����J���[�Ƃ��Ďg�p���邩�ǂ���
	int										MaxAnisotropy ;									// �ő�ٕ���
	int										MaxAnisotropyDim[ USE_TEXTURESTAGE_NUM ] ;		// �e�T���v���̍ő�ٕ���
	D_D3DTEXTUREFILTERTYPE					MagFilter[ USE_TEXTURESTAGE_NUM ] ;				// �e�T���v���̊g��t�B���^
	D_D3DTEXTUREFILTERTYPE					MinFilter[ USE_TEXTURESTAGE_NUM ] ;				// �e�T���v���̏k���t�B���^
	D_D3DTEXTUREFILTERTYPE					MipFilter[ USE_TEXTURESTAGE_NUM ] ;				// �e�T���v���̃~�b�v�}�b�v�t�B���^
	RECT									ScissorRect ;									// �V�U�[��`
	DWORD									SetFVF ;										// ���� Direct3DDevice9 �ɃZ�b�g���Ă��� FVF �R�[�h
	D_IDirect3DIndexBuffer9 *				SetIndexBuffer ;								// ���� Direct3DDevice9 �ɃZ�b�g���Ă���C���f�b�N�X�o�b�t�@
	int										SetNormalVertexShader ;							// �ʏ�`��p���_�V�F�[�_�[���Z�b�g����Ă��邩�ǂ���
	int										SetNormalPixelShader ;							// �ʏ�`��p�s�N�Z���V�F�[�_�[���Z�b�g����Ă��邩�ǂ���
	D_IDirect3DPixelShader9 *				SetPixelShader ;								// ���� Direct3DDevice9 �ɃZ�b�g���Ă���s�N�Z���V�F�[�_�[
	D_IDirect3DVertexDeclaration9 *			SetVertexDeclaration ;							// ���� Direct3DDevice9 �ɃZ�b�g���Ă��钸�_�V�F�[�_�錾
	D_IDirect3DVertexShader9 *				SetVertexShader ;								// ���� Direct3DDevice9 �ɃZ�b�g���Ă��钸�_�V�F�[�_�[
	D_IDirect3DVertexBuffer9 *				SetVertexBuffer ;								// ���� Direct3DDevice9 �ɃZ�b�g���Ă��钸�_�o�b�t�@
	int										SetVertexBufferStride ;							// ���� Direct3DDevice9 �ɃZ�b�g���Ă��钸�_�o�b�t�@�̃X�g���C�h
	int										ShadeMode ;										// �V�F�[�f�B���O���[�h
	D_IDirect3DSurface9	*					TargetSurface[DX_RENDERTARGET_COUNT] ;			// �`��Ώۂ̃T�[�t�F�X
	int										TexAddressModeU[ USE_TEXTURESTAGE_NUM ] ;		// �e�N�X�`���A�h���X���[�h�t
	int										TexAddressModeV[ USE_TEXTURESTAGE_NUM ] ;		// �e�N�X�`���A�h���X���[�h�u
	int										TexAddressModeW[ USE_TEXTURESTAGE_NUM ] ;		// �e�N�X�`���A�h���X���[�h�v
	int										TextureTransformUse[ USE_TEXTURESTAGE_NUM ] ;	// �e�N�X�`�����W�ϊ��s����g�p���邩�ǂ���( TRUE:�g�p����  FALSE:�g�p���Ȃ� )
	MATRIX									TextureTransformMatrix[ USE_TEXTURESTAGE_NUM ] ;// �e�N�X�`�����W�ϊ��s��
	int										UseSpecular ;									// �X�y�L�������g�p���邩�ǂ���
	D_D3DVIEWPORT9							Viewport ;										// �r���[�|�[�g���
	int										ZEnable ;										// �y�o�b�t�@�̗L���t���O
	int										ZFunc ;											// �y�l�̔�r���[�h
	int										ZWriteEnable ;									// �y�o�b�t�@�ւ̏������݂��s�����A�t���O
} ;


// Direct3DDevice9 �̕`�揈���Ɋւ����̍\����
struct GRAPHICS_HARDDATA_DIRECT3D9_DRAWINFO
{
	BYTE *					VertexBufferNextAddr ;					// ���̃f�[�^���i�[���ׂ����_�o�b�t�@���̃A�h���X
	BYTE *					VertexBufferAddr[ 2 ][ 3 ] ;			// �e���_�o�b�t�@�ւ̃|�C���^( [ �R�c���_���ǂ��� ][ ���_�^�C�v ] )
	BYTE					VertexBuffer_NoTex[     D3DDEV_NOTEX_VERTBUFFERSIZE     ] ;	// �e�N�X�`�����g�p���Ȃ����_�o�b�t�@
	BYTE					VertexBuffer_Tex[       D3DDEV_TEX_VERTBUFFERSIZE       ] ;	// �e�N�X�`�����g�p���钸�_�o�b�t�@
	BYTE					VertexBuffer_BlendTex[  D3DDEV_BLENDTEX_VERTBUFFERSIZE  ] ;	// �u�����h�e�N�X�`�����g�p���钸�_�o�b�t�@
	BYTE					VertexBuffer_3D[        D3DDEV_NOTEX_3D_VERTBUFFERSIZE  ] ;	// �R�c�p���_�o�b�t�@
	int						Use3DVertex ;							// �R�c���_���g�p���Ă��邩�ǂ���( 1:�g�p���Ă���  0:�g�p���Ă��Ȃ� )
	int						VertexNum ; 							// ���_�o�b�t�@�Ɋi�[����Ă��钸�_�̐�
	int						VertexType ;							// ���_�o�b�t�@�Ɋi�[����Ă��钸�_�f�[�^( 0:�e�N�X�`���Ȃ�  1:�e�N�X�`������  2:�u�����h�e�N�X�`������ )
	D_D3DPRIMITIVETYPE		PrimitiveType ;							// ���_�o�b�t�@�Ɋi�[����Ă��钸�_�f�[�^�̃v���~�e�B�u�^�C�v

	DWORD					DiffuseColor ;							// �f�B�t���[�Y�J���[

//	RECT					DrawRect ;								// �`��͈�
	int						BeginSceneFlag ;						// Graphics_D3D9_BeginScene �����s���Ă��邩�ǂ���
	int						BlendMaxNotDrawFlag ;					// �u�����h�l���ő�ł��邱�Ƃɂ��`����s��Ȃ����ǂ����A�t���O
} ;


// Direct3DDevice9 �̉�ʊ֌W�̏��̍\����
struct GRAPHICS_HARDDATA_DIRECT3D9_SCREENINFO
{
	D_IDirect3DSurface9 *	BackBufferSurface ;						// �f�o�C�X�����o�b�N�o�b�t�@�T�[�t�F�X
	int						SubBackBufferTextureSizeX ;				// �T�u�o�b�N�o�b�t�@�e�N�X�`���̉���
	int						SubBackBufferTextureSizeY ;				// �T�u�o�b�N�o�b�t�@�e�N�X�`���̏c��
	D_IDirect3DTexture9 *	SubBackBufferTexture ;					// ScreenCopy �� GetDrawScreen ���������邽�߂Ɏg�p����`��\�e�N�X�`��
	D_IDirect3DSurface9 *	SubBackBufferSurface ;					// ScreenCopy �� GetDrawScreen ���������邽�߂Ɏg�p����`��\�T�[�t�F�X
	D_IDirect3DSurface9 *	ZBufferSurface ;						// ���C���Ŏg�p����y�o�b�t�@
	int						ZBufferSizeX ;							// �y�o�b�t�@�T�[�t�F�X�̕�
	int						ZBufferSizeY ;							// �y�o�b�t�@�T�[�t�F�X�̍���
	int						ZBufferBitDepth ;						// �y�o�b�t�@�T�[�t�F�X�̃r�b�g�[�x

	D_IDirect3DSurface9 *	DrawScreenBufferLockSMSurface ;			// �o�b�N�o�b�t�@�����b�N�����ۂɒ��ڃ��b�N�ł��Ȃ��ꍇ�Ɏg�p����T�[�t�F�X�̃|�C���^
	int						DrawScreenBufferLockSMSurfaceIndex ;	// �o�b�N�o�b�t�@�����b�N�����ۂɒ��ڃ��b�N�ł��Ȃ��ꍇ�Ɏg�p����T�[�t�F�X�L���b�V���̃C���f�b�N�X
	D_IDirect3DSurface9 *	DrawScreenBufferLockSurface ;			// ���b�N�����o�b�N�o�b�t�@

	D_IDirect3DSurface9 *   SaveDrawScreen_SMSurface ;				// SaveDrawScreen �Ŏg�p����V�X�e���������T�[�t�F�X

	int						WM_PAINTDrawCounter ;					// WM_PAINT �������㉽�� Direct3D 9 �Ǌ��O�̃N���C�A���g�̈���������邽�߂̃J�E���^
} ;


// Direct3DDevice9 �̕`��ݒ�Ɋւ����̍\����
struct GRAPHICS_HARDDATA_DIRECT3D9_DRAWSETTING
{
	int						DrawPrepParamFlag ;						// �O�� Graphics_D3D9_DrawPreparation �ɓ������Ƃ��� ParamFlag �p�����[�^
	int						DrawPrepAlwaysFlag ;					// �K�� Graphics_D3D9_DrawPreparation ���s�����ǂ����̃t���O

	int						AlphaChannelValidFlag ;					// ���`�����l���L���t���O
	int						AlphaTestValidFlag ;					// ���e�X�g�L���t���O( Func �͕K�� D_D3DCMP_GREATEREQUAL )
	int						AlphaTestMode ;							// �A���t�@�e�X�g���[�h
	int						AlphaTestParam ;						// �A���t�@�e�X�g�p�����[�^
	int						BlendMode ;								// �u�����h���[�h
	int						ChangeBlendParamFlag ;					// �u�����h�ݒ�Ɋւ�镔���̕ύX�����������A�t���O
	int						ChangeTextureFlag ;						// �e�N�X�`�����ύX���ꂽ���A�t���O
	int						IgnoreGraphColorFlag ;					// �`�悷��摜�̐F�����𖳎����邩�ǂ����̃t���O
	int						UseDiffuseRGBColor ;					// �f�B�t���[�Y�J���[�̂q�f�a�l���g�p���邩�A�t���O
	int						UseDiffuseAlphaColor ;					// �f�B�t���[�Y�J���[�̃��l���g�p���邩�A�t���O
	int						BlendGraphType ;						// �u�����h�摜�^�C�v
	int						BlendGraphFadeRatio ;					// �u�����h�摜�̃t�F�[�h�p�����[�^
	int						BlendGraphBorderParam ;					// �u�����h�摜�̋��E�p�����[�^(�O(�u�����h�摜�̉e���O)�@���@(�u�����h�摜�̉e�����Ȃ�)�@���@�P�Q�W(�u�����h�摜�̉e���P�O�O��)�@���@(�u�����h�摜�̉e���𒴂��Ĕ�`�敔����������)�@���Q�T�T(�S���`�悳��Ȃ�) )
	int						BlendGraphBorderRange ;					// �u�����h�摜�̋��E��(�O�`�Q�T�T�@�����`�L���@�������S�i�K)
	float					BlendTextureWidth ;						// �u�����h�e�N�X�`���̕�
	float					BlendTextureHeight ;					// �u�����h�e�N�X�`���̍���
	float					InvBlendTextureWidth ;					// �u�����h�e�N�X�`���̕��̋t��
	float					InvBlendTextureHeight ;					// �u�����h�e�N�X�`���̍����̋t��

	D_IDirect3DTexture9		*RenderTexture ;						// �`�掞�Ɏg�p����e�N�X�`���[
	D_IDirect3DTexture9		*BlendTexture ;							// �`�掞�ɕ`��e�N�X�`���[�ƃu�����h���郿�`�����l���e�N�X�`���[

	int						CancelSettingEqualCheck ;				// ���̃t���O�������Ă���ꍇ�͌��݂̐ݒ�ƕύX��̐ݒ肪�������ǂ����Ɋւ�炸�A�K���ύX���������s����
} ;


// Direct3DDevice9 �֌W�̐ݒ���̍\����
struct GRAPHICS_HARDDATA_DIRECT3D9_DEVICE_SETTING
{
	void					( *DeviceRestoreCallbackFunction)( void *Data ) ;	// �f�o�C�X���X�g���畜�A�����Ƃ��ɌĂԊ֐�
	void					  *DeviceRestoreCallbackData ;						// �f�o�C�X���X�g���畜�A�����Ƃ��ɌĂԊ֐��ɓn���|�C���^

	void					( *DeviceLostCallbackFunction)( void *Data ) ;		// �f�o�C�X���X�g���畜�A����O�ɌĂԊ֐�
	void					  *DeviceLostCallbackData ;							// �f�o�C�X���X�g���畜�A����O�ɌĂԊ֐��ɓn���|�C���^
} ;


// Direct3DDevice9 �̔\�͏��\����
struct GRAPHICS_HARDDATA_DIRECT3D9_DEVICE_CAPS
{
	D_D3DCAPS9				DeviceCaps ;										// �f�o�C�X���

	int						VertexHardwareProcess ;								// ���_���Z���n�[�h�E�G�A�ŏ������邩�ǂ���( TRUE:��������  FALSE:�������Ȃ� )
//	int						TextureSizePow2 ;									// �e�N�X�`���̃T�C�Y���Q�̂���ł���K�v�����邩�ǂ���
//	int						TextureSizeNonPow2Conditional ;						// �����t�Ńe�N�X�`���̃T�C�Y���Q�̂���łȂ��Ă����v���ǂ���
//	int						MaxTextureSize ;									// �ő�e�N�X�`���T�C�Y
	int						MaxTextureBlendStages ;								// �Œ�@�\�p�C�v���C���Ŏg�p�ł���ő�e�N�X�`���X�e�[�W��
	int						ValidTexTempRegFlag ;								// �e�N�X�`���X�e�[�W�̃e���|�������W�X�^���g�p�ł��邩�ǂ����̃t���O
	int						ValidDestBlendOp ;									// D3DBLENDOP_ADD �ȊO���g�p�\���ǂ���( TRUE:�g�p�\  FALSE:�g�p�s�\ )
//	int						RenderTargetNum ;									// �����ɐݒ�ł��郌���_�����O�^�[�Q�b�g�̐�
//	int						MaxPrimitiveCount ;									// ��x�ɕ`��ł���v���~�e�B�u�̍ő吔
	int						UseRenderTargetLock ;								// �`���T�[�t�F�X�̃��b�N���s�����ǂ���( TRUE:�s��  FALSE:�s��Ȃ� )
	float					DrawFillCircleLeftVertAddX ;						// ����h��Ԃ��~�̕`��̍ۂɍ��[���W�� x ���W�ɑ����l
	float					DrawFillCircleRightVertAddX ;						// ����h��Ԃ��~�̕`��̍ۂɉE�[���W�� x ���W�ɑ����l

#ifndef DX_NON_FILTER
	int						ValidRenderTargetInputTexture ;						// �`������̓e�N�X�`���Ƃ��Ďg�p�ł��邩�ǂ���( TRUE:�g�p�ł���  FALSE:�g�p�ł��Ȃ� )
#endif // DX_NON_FILTER

	D_D3DFORMAT				ScreenFormat ;										// ��ʃJ���[�t�H�[�}�b�g
	D_D3DFORMAT				TextureFormat[ DX_GRAPHICSIMAGE_FORMAT_3D_NUM ] ;	// �e�N�X�`���t�H�[�}�b�g
	D_D3DFORMAT				MaskColorFormat ;									// �}�X�N�J���[�o�b�t�@�p�t�H�[�}�b�g
	D_D3DFORMAT				MaskAlphaFormat ;									// �}�X�N�A���t�@�`�����l���p�t�H�[�}�b�g
	D_D3DFORMAT				ZBufferFormat[ ZBUFFER_FORMAT_NUM ] ;				// �y�o�b�t�@�t�H�[�}�b�g
} ;


// Direct3D9 ���g�����`�揈���Ŏg�p����f�[�^�̓��� Direct3D9Device �̃��Z�b�g��
// ��������������K�v������ADirect3D9Device �ɖ��ڂɊւ��������Z�߂��\����
struct GRAPHICS_HARDDATA_DIRECT3D9_DEVICE
{
	GRAPHICS_HARDDATA_DIRECT3D9_DEVICE_CAPS			Caps ;					// �f�o�C�X�\�͏��\����		

	GRAPHICS_HARDDATA_DIRECT3D9_DEVICE_SETTING		Setting ;				// �f�o�C�X�֌W�̐ݒ����Z�߂��\����

	GRAPHICS_HARDDATA_DIRECT3D9_SCREENINFO			Screen ;				// ��ʊ֌W�̏��\����

	GRAPHICS_HARDWARE_DIRECT3D9_SHADER				Shader ;				// �V�F�[�_�[��Z�߂��\����

	GRAPHICS_HARDWARE_DIRECT3D9_VERTEXDECLARATION	VertexDeclaration ;		// ���_�V�F�[�_�錾��Z�߂��\����

	GRAPHICS_HARDDATA_DIRECT3D9_DRAWSETTING			DrawSetting ;			// �`��ݒ�Ɋւ�����Z�߂��\����

	GRAPHICS_HARDDATA_DIRECT3D9_DRAWINFO			DrawInfo ;				// �`�揈���Ɋւ�����Z�߂��\����

	GRAPHICS_HARDDATA_DIRECT3D9_DEVICE_STATE		State ;					// Direct3D9Device �ɐݒ肵�Ă���l��Z�߂��\����
} ;














// Direct3D9 �� �Q�c�`��p�̃V�F�[�_�[�R�[�h�̌��f�[�^��Z�߂��\����
struct GRAPHICS_HARDWARE_DIRECT3D9_SHADERCODE_2D
{
	int						_2DShaderInitializeFlag ;				// �V�F�[�_�[�o�C�i���̃Z�b�g�A�b�v���������Ă��邩�ǂ����̃t���O( TRUE:�������Ă���  FALSE:�������Ă��Ȃ� )

#ifndef DX_NON_NORMAL_DRAW_SHADER
	DXARC					BaseShaderBinDxa ;						// ��{�V�F�[�_�[�I�u�W�F�N�g�t�@�C���c�w�`�\����
	void					*BaseShaderBinDxaImage ;				// ��{�V�F�[�_�[�I�u�W�F�N�g�t�@�C���c�w�`�̃o�C�i���C���[�W
#endif // DX_NON_NORMAL_DRAW_SHADER

#ifndef DX_NON_FILTER
	DXARC					FilterShaderBinDxa ;					// �t�B���^�[�V�F�[�_�[�I�u�W�F�N�g�t�@�C���c�w�`�\����
	void					*FilterShaderBinDxaImage ;				// �t�B���^�[�V�F�[�_�[�I�u�W�F�N�g�t�@�C���c�w�`�̃o�C�i���C���[�W

	void					*RGBAMixS_ShaderPackImage ;				// RGBAMix �� S �����̑g�ݍ��킹�Q�T�U�̃V�F�[�_�[�p�b�P�[�W�o�C�i���C���[�W

	// RGBAMix �� S �����̑g�ݍ��킹�Q�T�U�̃V�F�[�_�[[ R ][ G ][ B ][ A ]
	void					*RGBAMixS_ShaderAddress[ 4 ][ 4 ][ 4 ][ 4 ] ;
	short					RGBAMixS_ShaderSize[ 4 ][ 4 ][ 4 ][ 4 ] ;
#endif // DX_NON_FILTER
} ;


#ifndef DX_NON_MODEL

// Direct3D9 �� ���f���`��p�̃V�F�[�_�[�R�[�h�̌��f�[�^��Z�߂��\����
struct GRAPHICS_HARDWARE_DIRECT3D9_SHADERCODE_MODEL
{
	volatile int			ModelShaderInitializeFlag ;				// ���f���p�̃V�F�[�_�[�o�C�i���̃Z�b�g�A�b�v���������Ă��邩�ǂ����̃t���O( TRUE:�������Ă���  FALSE:�������Ă��Ȃ� )

	// ���C�g�C���f�b�N�X���X�g
	short					LightIndexList84[ 4 ][ 4 ][ 4 ][ 4 ][ 4 ][ 4 ] ;
	short					LightIndexList20[ 4 ][ 4 ][ 4 ] ;

	short					LightIndexList10[ 3 ][ 3 ][ 3 ] ;

	void					*ModelShaderPackImage ;					// �R�c���f���p�V�F�[�_�[�p�b�P�[�W�o�C�i���C���[�W

	// ���f���`��p�s�N�Z���P�ʃ��C�e�B���O�^�C�v�̒��_�V�F�[�_�[( ���X�g�͔z��̍����� )
	// [ �V���h�E�}�b�v�̗L��( 0:���� 1:�L�� ) ]
	// [ ���b�V���^�C�v( 0:���̃��b�V�� 1:4�{�[�����X�L�j���O���b�V�� 2:8�{�[�����X�L�j���O���b�V�� ) ]
	// [ �o���v�}�b�v�̗L��( 0:���� 1:�L�� ) ]
	// [ �t�H�O�^�C�v ]
	void					*MV1_PixelLighting_VertexShaderAddress[ 2 ][ 3 ][ 2 ][ 4 ] ;

	// ���f���`��p�s�N�Z���P�ʃ��C�e�B���O�^�C�v�̃g�D�[���p�s�N�Z���V�F�[�_�[( ���X�g�͔z��̍����� )
	// [ �V���h�E�}�b�v�̗L��( 0:���� 1:�L�� ) ]
	// [ �g�D�[�������_�����O���ǂ���( 0:�g�D�[�������_�����O type 1   1:�g�D�[�������_�����O Type 2 ) ]
	// [ �g�D�[�������_�����O�̃X�t�B�A�}�b�v�̗L���ƃu�����h�^�C�v( 0:�X�t�B�A�}�b�v�͖���   1:MV1_LAYERBLEND_TYPE_MODULATE  2:MV1_LAYERBLEND_TYPE_ADDITIVE ) ]
	// [ �g�D�[�������_�����O�̃f�B�t���[�Y�O���f�[�V�����̃u�����h�^�C�v( 0:MV1_LAYERBLEND_TYPE_TRANSLUCENT  1:MV1_LAYERBLEND_TYPE_MODULATE ) ]
	// [ �g�D�[�������_�����O�̃X�y�L�����O���f�[�V�����̃u�����h�^�C�v( 0:MV1_LAYERBLEND_TYPE_TRANSLUCENT  1:MV1_LAYERBLEND_TYPE_ADDITIVE ) ]
	// [ �X�y�L�����}�b�v ( 0:���� 1:���� ) ]
	// [ �o���v�}�b�v ( 0:�Ȃ� 1:���� ) ]
	// [ ���C�g�C���f�b�N�X ]
	// [ �X�y�L�����̗L��( 0:���� 1:�L�� ) ]
	void					*MV1_PixelLighting_ToonPixelShaderAddress[ 2 ][ 2 ][ 3 ][ 2 ][ 2 ][ 2 ][ 2 ][ 84 ][ 2 ] ;

	// ���f���`��p�s�N�Z���P�ʃ��C�e�B���O�^�C�v�̃s�N�Z���V�F�[�_�[( ���X�g�͔z��̍����� )
	// [ �V���h�E�}�b�v�̗L��( 0:���� 1:�L�� ) ]
	// [ �}���`�e�N�X�`���̃u�����h���[�h�AMV1_LAYERBLEND_TYPE_TRANSLUCENT �Ȃǂ� +1 �������� ( 0:�}���`�e�N�X�`������  1:���u�����h  2:���Z�u�����h  3:��Z�u�����h  4:��Z�u�����h�~2 ) ]
	// [ �X�y�L�����}�b�v ( 0:���� 1:���� ) ]
	// [ �o���v�}�b�v ( 0:�Ȃ� 1:���� ) ]
	// [ ���C�g�C���f�b�N�X ]
	// [ �X�y�L�����̗L��( 0:���� 1:�L�� ) ]
	void					*MV1_PixelLighting_PixelShaderAddress[ 2 ][ 5 ][ 2 ][ 2 ][ 84 ][ 2 ] ;






	// ���f���`��p�̃g�D�[�������_�����O�̗֊s���`��p���_�V�F�[�_�[
	// [ ���b�V���^�C�v( 0:���̃��b�V�� 1:4�{�[�����X�L�j���O���b�V��  2:8�{�[�����X�L�j���O���b�V�� ) ]
	// [ �t�H�O�^�C�v ]
	void					*MV1_ToonOutLineVertexShaderAddress[ 3 ][ 4 ] ;

	// ���f���`��p�̃V���h�E�}�b�v�ւ̃����_�����O�p���_�V�F�[�_�[
	// [ ���b�V���^�C�v( 0:���̃��b�V�� 1:4�{�[�����X�L�j���O���b�V��  2:8�{�[�����X�L�j���O���b�V�� ) ]
	void					*MV1_ShadowMapVertexShaderAddress[ 3 ] ;

	// ���f���`��p�̃��C�e�B���O�Ȃ����_�V�F�[�_�[
	// [ ���b�V���^�C�v( 0:���̃��b�V�� 1:4�{�[�����X�L�j���O���b�V��  2:8�{�[�����X�L�j���O���b�V�� ) ]
	// [ �t�H�O�^�C�v ]
	void					*MV1_NoLightingVertexShaderAddress[ 3 ][ 4 ] ;

	// ���f���`��p�̃��C�e�B���O���蒸�_�V�F�[�_�[
	// [ �V�F�[�_�[���f��( 0:SM2  1:SM3 )
	// [ �V���h�E�}�b�v�̗L��( 0:���� 1:�L�� ) ]
	// [ ���b�V���^�C�v( 0:���̃��b�V�� 1:4�{�[�����X�L�j���O���b�V��  2:8�{�[�����X�L�j���O���b�V�� ) ]
	// [ �o���v�}�b�v�̗L��( 0:���� 1:�L�� ) ]
	// [ �t�H�O�^�C�v ]
	// [ ���C�g�C���f�b�N�X ]
	// [ �X�y�L�����̗L��( 0:���� 1:�L�� ) ]
	void					*MV1_LightingVertexShaderAddress[ 2 ][ 2 ][ 3 ][ 2 ][ 4 ][ 20 ][ 2 ] ;


	// ���f���`��p�̃g�D�[�������_�����O�̗֊s���`��p�s�N�Z���V�F�[�_�[
	void					*MV1_ToonOutLinePixelShaderAddress ;

	// ���f���`��p�̃V���h�E�}�b�v�ւ̃g�D�[�������_�����O�p�s�N�Z���V�F�[�_�[
	// [ �g�D�[�������_�����O�^�C�v( 0:�g�D�[�������_�����O type 1   1:�g�D�[�������_�����O type 2 ) ]
	// [ �g�D�[�������_�����O�̃X�t�B�A�}�b�v�̗L���ƃu�����h�^�C�v( 0:�X�t�B�A�}�b�v�͖���   1:MV1_LAYERBLEND_TYPE_MODULATE  2:MV1_LAYERBLEND_TYPE_ADDITIVE ) ]
	// [ �g�D�[�������_�����O�̃f�B�t���[�Y�O���f�[�V�����̃u�����h�^�C�v( 0:MV1_LAYERBLEND_TYPE_TRANSLUCENT  1:MV1_LAYERBLEND_TYPE_MODULATE ) ]
	void					*MV1_ShadowMapToonPixelShaderAddress[ 2 ][ 3 ][ 2 ] ;

	// ���f���`��p�̃V���h�E�}�b�v�ւ̃����_�����O�p�s�N�Z���V�F�[�_�[
	void					*MV1_ShadowMapPixelShaderAddress ;

	// ���f���`��p�̃��C�e�B���O�Ȃ��g�D�[���p�s�N�Z���V�F�[�_�[( ���X�g�͔z��̍����� )
	// [ �g�D�[�������_�����O�^�C�v( 0:�g�D�[�������_�����O type 1   1:�g�D�[�������_�����O type 2 ) ]
	// [ �g�D�[�������_�����O�̃f�B�t���[�Y�O���f�[�V�����̃u�����h�^�C�v( 0:MV1_LAYERBLEND_TYPE_TRANSLUCENT or �g�D�[�������_�����O�ł͂Ȃ�  1:MV1_LAYERBLEND_TYPE_MODULATE ) ]
	void					*MV1_ToonNoLightingPixelShaderAddress[ 2 ][ 2 ] ;

	// ���f���`��p�̃��C�e�B���O�Ȃ��s�N�Z���V�F�[�_�[
	// [ �}���`�e�N�X�`���̃u�����h���[�h�AMV1_LAYERBLEND_TYPE_TRANSLUCENT �Ȃǂ� +1 �������� ( 0:�}���`�e�N�X�`������  1:���u�����h  2:���Z�u�����h  3:��Z�u�����h  4:��Z�u�����h�~2 ) ]
	void					*MV1_NoLightingPixelShaderAddress[ 5 ] ;

	// ���f���`��p�̃��C�e�B���O����g�D�[���p�s�N�Z���V�F�[�_�[
	// [ �V�F�[�_�[���f��( 0:SM2  1:SM3 )
	// [ �V���h�E�}�b�v�̗L��( 0:���� 1:�L�� ) ]
	// [ �g�D�[�������_�����O�^�C�v( 0:�g�D�[�������_�����O type 1   1:�g�D�[�������_�����O type 2 ) ]
	// [ �g�D�[�������_�����O�̃X�t�B�A�}�b�v�̗L���ƃu�����h�^�C�v( 0:�X�t�B�A�}�b�v�͖���   1:MV1_LAYERBLEND_TYPE_MODULATE  2:MV1_LAYERBLEND_TYPE_ADDITIVE ) ]
	// [ �g�D�[�������_�����O�̃f�B�t���[�Y�O���f�[�V�����̃u�����h�^�C�v( 0:MV1_LAYERBLEND_TYPE_TRANSLUCENT  1:MV1_LAYERBLEND_TYPE_MODULATE ) ]
	// [ �g�D�[�������_�����O�̃X�y�L�����O���f�[�V�����̃u�����h�^�C�v( 0:MV1_LAYERBLEND_TYPE_TRANSLUCENT  1:MV1_LAYERBLEND_TYPE_ADDITIVE ) ]
	// [ �X�y�L�����}�b�v ( 0:���� 1:���� ) ]
	// [ �o���v�}�b�v ( 0:�Ȃ� 1:���� ) ]
	// [ ���C�g�C���f�b�N�X ]
	// [ �X�y�L�����̗L��( 0:���� 1:�L�� ) ]
	void					*MV1_ToonLightingPixelShaderAddress[ 2 ][ 2 ][ 2 ][ 3 ][ 2 ][ 2 ][ 2 ][ 2 ][ 10 ][ 2 ] ;

	// ���f���`��p�̃��C�e�B���O����s�N�Z���V�F�[�_�[
	// [ �V�F�[�_�[���f��( 0:SM2  1:SM3 )
	// [ �V���h�E�}�b�v�̗L��( 0:���� 1:�L�� ) ]
	// [ �}���`�e�N�X�`���̃u�����h���[�h�AMV1_LAYERBLEND_TYPE_TRANSLUCENT �Ȃǂ� +1 �������� ( 0:�}���`�e�N�X�`������  1:���u�����h  2:���Z�u�����h  3:��Z�u�����h  4:��Z�u�����h�~2 ) ]
	// [ �X�y�L�����}�b�v ( 0:���� 1:���� ) ]
	// [ �o���v�}�b�v ( 0:�Ȃ� 1:���� ) ]
	// [ ���C�g�C���f�b�N�X ]
	// [ �X�y�L�����̗L��( 0:���� 1:�L�� ) ]
	void					*MV1_LightingPixelShaderAddress[ 2 ][ 2 ][ 5 ][ 2 ][ 2 ][ 10 ][ 2 ] ;
} ;

#endif // DX_NON_MODEL


// �V�F�[�_�[�R�[�h�̌��f�[�^��Z�߂��\����
struct GRAPHICS_HARDWARE_DIRECT3D9_SHADERCODE
{
	GRAPHICS_HARDWARE_DIRECT3D9_SHADERCODE_2D		_2D ;				// �Q�c�`�揈���p�̃V�F�[�_�[�R�[�h��Z�߂��\����

#ifndef DX_NON_MODEL
	GRAPHICS_HARDWARE_DIRECT3D9_SHADERCODE_MODEL	Model ;				// ���f���`�揈���p�̃V�F�[�_�[�R�[�h��Z�߂��\����
#endif // DX_NON_MODEL
} ;


// �A�_�v�^�[�֌W���\����
struct GRAPHICS_HARDWARE_DIRECT3D9_ADAPTER
{
	int												ValidInfo ;					// �A�_�v�^�̏�񂪗L�����ǂ���( TRUE:�L��  FALSE:���� )
	int												InfoNum ;					// �A�_�v�^���̐�
	D_D3DADAPTER_IDENTIFIER9						Info[ DX_D3D9_MAX_DEVICE_LISTUP ] ;	// �A�_�v�^�̏��

//	int												ValidUseAdapterNumber ;		// UseAdapterNumber ���L�����ǂ���( TRUE:�L��  FALSE:���� )
//	int												UseAdapterNumber ;			// �g�p����f�o�C�X�A�_�v�^�ԍ�
} ;


// Direct3D9 �֌W�̐ݒ���̍\����
struct GRAPHICS_HARDDATA_DIRECT3D9_SETTING
{
	int						NotUseDirect3D9Ex ;						// Direct3D9Ex ���g�p���Ȃ����ǂ���
	int						NonUseVertexHardwareProcess ;			// �����I�ɒ��_���Z���n�[�h�E�G�A�ŏ������Ȃ����ǂ���( TRUE:�������Ȃ�  FALSE:�������� )
//	int						DisableAeroFlag ;						// Aero �𖳌��ɂ��邩�ǂ����̃t���O( TRUE:�����ɂ���  FALSE:�����ɂ��Ȃ� )
//	D_D3DMULTISAMPLE_TYPE	FSAAMultiSampleType ;					// FSAA�p�}���`�T���v�����O�^�C�v
//	int						FSAAMultiSampleQuality ;				// FSAA�p�}���`�T���v�����O�N�I���e�B
	int						UseOldDrawModiGraphCodeFlag ;			// �ȑO�� DrawModiGraph �֐��R�[�h���g�p���邩�ǂ����̃t���O
	int						UserZBufferSizeSet ;					// �O������y�o�b�t�@�T�C�Y�̎w�肪���������ǂ���
	int						UserZBufferBitDepthSet ;				// �O������y�o�b�t�@�̃r�b�g�[�x�w�肪���������ǂ���
} ;


// ��Ǘ��e�N�X�`���ւ̃f�[�^�]���p�V�X�e���������z�u�e�N�X�`���̏��
struct GRAPHICS_HARDDATA_DIRECT3D9_SYSMEMTEXTURE
{
	unsigned char					InitializeFlag ;						// �g�p�\�ȏ�Ԃ��ǂ����̃t���O
	unsigned char					UseFlag ;								// �g�p�����ǂ����̃t���O
	unsigned char					CubeMap ;								// �L���[�u�}�b�v���ǂ���( 1:�L���[�u�}�b�v�e�N�X�`��  0:�ʏ�e�N�X�`�� )
	unsigned char					MipMapCount ;							// �~�b�v�}�b�v�̐�
	short							Width ;									// ��
	short							Height ;								// ����
	D_D3DFORMAT						Format ;								// �t�H�[�}�b�g
	unsigned int					UseCount ;								// �g�p��
	int								UseTime ;								// �Ō�Ɏg�p��������
	D_IDirect3DBaseTexture9	*		MemTexture ;							// �V�X�e���������e�N�X�`��
} ;


// ��Ǘ��e�N�X�`���ւ̃f�[�^�]���p�V�X�e���������z�u�T�[�t�F�X�̏��
struct GRAPHICS_HARDDATA_DIRECT3D9_SYSMEMSURFACE
{
	unsigned char					InitializeFlag ;						// �g�p�\�ȏ�Ԃ��ǂ����̃t���O
	unsigned char					UseFlag ;								// �g�p�����ǂ����̃t���O
	short							Width ;									// ��
	short							Height ;								// ����
	D_D3DFORMAT						Format ;								// �t�H�[�}�b�g
	unsigned int					UseCount ;								// �g�p��
	int								UseTime ;								// �Ō�Ɏg�p��������
	D_IDirect3DSurface9	*			MemSurface ;							// �V�X�e���������T�[�t�F�X
} ;


// Direct3D9 �̃V�X�e���������e�N�X�`���E�T�[�t�F�X���̍\����
struct GRAPHICS_HARDDATA_DIRECT3D9_SYSMEM_TEX_SURF
{
	GRAPHICS_HARDDATA_DIRECT3D9_SYSMEMTEXTURE		Texture[ DIRECT3D9_SYSMEMTEXTURE_NUM ] ;	// ��Ǘ��e�N�X�`���ւ̃f�[�^�]���p�V�X�e���������z�u�e�N�X�`��
	int												TextureInitNum ;				// �������ς݂̃V�X�e���������e�N�X�`���̐�

	GRAPHICS_HARDDATA_DIRECT3D9_SYSMEMSURFACE		Surface[ DIRECT3D9_SYSMEMSURFACE_NUM ] ;	// ��Ǘ��e�N�X�`���ւ̃f�[�^�]���p�V�X�e���������z�u�T�[�t�F�X
	int												SurfaceInitNum ;				// �������ς݂̃V�X�e���������T�[�t�F�X�̐�
} ;


// Direct3D9 ���g�����O���t�B�b�N�X�������̍\����
struct GRAPHICS_HARDDATA_DIRECT3D9
{
//	int												NormalDraw_UseVertexShader ;	// �ʏ�`��ɒ��_�V�F�[�_�[���g�p���邩�ǂ���( TRUE:�g�p����  FALSE:�g�p���Ȃ� )
	int												NormalDraw_NotUsePixelShader ;	// �ʏ�`��Ƀs�N�Z���V�F�[�_�[���g�p���Ȃ����ǂ���( TRUE:�g�p���Ȃ�  FALSE:�g�p���� )
	int												UsePixelLightingShader ;	// �s�N�Z���P�ʂŃ��C�e�B���O���s���^�C�v�̃V�F�[�_�[���g�p���邩�ǂ���( TRUE:�g�p����  FALSE:�g�p���Ȃ� )

#ifndef DX_NON_MODEL
	D_IDirect3DVolumeTexture9 *						RGBtoVMaxRGBVolumeTexture ;	// RGB�J���[���P�x���ő�ɂ���RGB�l�ɕϊ����邽�߂̃{�����[���e�N�X�`��
#endif // DX_NON_MODEL

	GRAPHICS_HARDDATA_DIRECT3D9_SETTING				Setting ;					// Direct3D9 �֌W�̐ݒ���

	GRAPHICS_HARDDATA_DIRECT3D9_DEVICE				Device ;					// Direct3D9Device �ɖ��ڂɊ֘A�������

	GRAPHICS_HARDWARE_DIRECT3D9_ADAPTER				Adapter ;					// �A�_�v�^�[�֌W���

	GRAPHICS_HARDWARE_DIRECT3D9_SHADERCODE			ShaderCode ;				// D_IDirect3DPixelShader9 �� D_IDirect3DVertexShader9 �̌��ƂȂ�V�F�[�_�[�R�[�h�f�[�^��Z�߂��\����

	GRAPHICS_HARDDATA_DIRECT3D9_SYSMEM_TEX_SURF		SysMemTexSurf ;				// �V�X�e���������e�N�X�`���E�T�[�t�F�X���
} ;


// �������ϐ��錾 --------------------------------------------------------------

extern GRAPHICS_HARDDATA_DIRECT3D9 GraphicsHardDataDirect3D9 ;

// �֐��v���g�^�C�v�錾-----------------------------------------------------------


// Direct3D9 �̏������֌W
extern	int		Graphics_D3D9_Initialize( void ) ;								// Direct3D9 ���g�p�����O���t�B�b�N�X�����̏��������s��( 0:���� -1:���s )
extern	int		Graphics_D3D9_Terminate( void ) ;								// Direct3D9 ���g�p�����O���t�B�b�N�X�����̌�n�����s��

extern	int		Graphics_D3D9_ReleaseObjectAll( void ) ;						// ���ׂĂ� Direct3D9 �n�I�u�W�F�N�g���������
extern	int		Graphics_D3D9_CreateObjectAll( void ) ;							// ���ׂĂ� Direct3D9 �n�I�u�W�F�N�g���쐬����

extern	int		Graphics_D3D9_SetupRenderTargetInputTextureFlag( void ) ;		// �s�N�Z���V�F�[�_�[���g�p����ꍇ�͏o�͐���e�N�X�`���Ƃ��Ďg�p�ł��邩�ǂ����̃`�F�b�N���s��








// �V�F�[�_�[�R�[�h�֌W
extern	int		Graphics_D3D9_ShaderCode_2D_Initialize( void ) ;				// Direct3D9 �̂Q�c�`��p�̃V�F�[�_�[�R�[�h�̏��������s��
extern	int		Graphics_D3D9_ShaderCode_2D_Terminate( void ) ;					// Direct3D9 �̂Q�c�`��p�̃V�F�[�_�[�R�[�h�̌�n�����s��

#ifndef DX_NON_MODEL

extern	int		Graphics_D3D9_ShaderCode_Model_Initialize( void ) ;				// Direct3D9 �̃��f���`��p�̃V�F�[�_�[�R�[�h�̏��������s��
extern	int		Graphics_D3D9_ShaderCode_Model_Terminate( void ) ;				// Direct3D9 �̃��f���`��p�̃V�F�[�_�[�R�[�h�̌�n�����s��

extern	int		Graphics_D3D9_Shader_Model_Setup( int VertexShaderIndex, int VertexShaderIndex_PL, int PixelShaderIndex, int PixelShaderIndex_PL ) ;		// �w��̃��f���`��p�V�F�[�_�[���Z�b�g�A�b�v����( TRUE:����  FALSE:���s )

#endif // DX_NON_MODEL

extern	int		Graphics_D3D9_Shader_Initialize( void ) ;						// Direct3D9 �̃V�F�[�_�[�̏��������s��
extern	int		Graphics_D3D9_Shader_Terminate( void ) ;						// Direct3D9 �̃V�F�[�_�[�̌�n��������

extern	void	Graphics_D3D9_Shader_RefreshUseIndex( void ) ;					// �g�p����V�F�[�_�[�̃C���f�b�N�X�̒l���X�V����

extern	D_D3DXSHADER_CONSTANTINFO *Graphics_D3D9_GetShaderConstInfo( SHADERHANDLEDATA *Shader, const wchar_t *ConstantName ) ;				// �V�F�[�_�[�̒萔���𓾂�








// ���_�o�b�t�@�E�C���f�b�N�X�o�b�t�@�֌W
extern	int		Graphics_D3D9_VertexBuffer_CreateObject(  VERTEXBUFFERHANDLEDATA *VertexBuffer, int Restore ) ;	// ���_�o�b�t�@�n���h���p�� Direct3DVertexBuffer9 �I�u�W�F�N�g���쐬����
extern	int		Graphics_D3D9_VertexBuffer_ReleaseObject( VERTEXBUFFERHANDLEDATA *VertexBuffer ) ;				// ���_�o�b�t�@�n���h���p�� Direct3DVertexBuffer9 �I�u�W�F�N�g���������
extern	int		Graphics_D3D9_IndexBuffer_CreateObject(   INDEXBUFFERHANDLEDATA *IndexBuffer,   int Restore ) ;	// �C���f�b�N�X�o�b�t�@�n���h���p�� Direct3DIndexBuffer9 �I�u�W�F�N�g���쐬����
extern	int		Graphics_D3D9_IndexBuffer_ReleaseObject(  INDEXBUFFERHANDLEDATA *IndexBuffer ) ;				// �C���f�b�N�X�o�b�t�@�n���h���p�� Direct3DIndexBuffer9 �I�u�W�F�N�g���������








// Direct3D9 �̉�ʊ֌W
extern	int		Graphics_D3D9_SetupSubBackBuffer( void ) ;						// �T�u�o�b�N�o�b�t�@���g�p���鏀�����s��( 0:����  -1:���s )
extern	int		Graphics_D3D9_CreateZBuffer( void ) ;							// �y�o�b�t�@�I�u�W�F�N�g�̍쐬( 0:����  -1:���s )








// Direct3D9 �̉摜�֌W
extern COLORDATA *Graphics_D3D9_GetD3DFormatColorData( D_D3DFORMAT Format ) ;									// D3DFORMAT �̃t�H�[�}�b�g�ɍ��킹���J���[�t�H�[�}�b�g�����擾����
extern	int		Graphics_D3D9_UpdateDrawTexture( IMAGEDATA_ORIG_HARD_TEX *OrigTex, int TargetSurface ) ;		// �`���p�e�N�X�`���ƕ`��p�e�N�X�`����������Ă���ꍇ�ɁA�`��p�e�N�X�`���ɕ`���p�e�N�X�`���̓��e�𔽉f����

// ��{�C���[�W�̃t�H�[�}�b�g�� DX_BASEIMAGE_FORMAT_NORMAL �ɕϊ�����K�v�����邩�ǂ������`�F�b�N����
// ( RequiredRgbBaseImageConvFlag �� RequiredAlphaBaseImageConvFlag �ɓ���l  TRUE:�ϊ�����K�v������  FALSE:�ϊ�����K�v�͖��� )
extern	int		Graphics_D3D9_CheckRequiredNormalImageConv_BaseImageFormat_PF(
	IMAGEDATA_ORIG *Orig,
	int             RgbBaseImageFormat,
	int            *RequiredRgbBaseImageConvFlag,
	int             AlphaBaseImageFormat = -1,
	int            *RequiredAlphaBaseImageConvFlag = NULL
) ;








// Direct3DDevice �̏������֌W
extern	int		Graphics_D3D9_Device_Create( void ) ;																// Direct3DDevice9 �I�u�W�F�N�g�̍쐬( 0:����  -1:���s )

extern	int		Graphics_D3D9_Device_Initialize( void ) ;															// Direct3DDevice9 �֌W�̏�����
extern	int		Graphics_D3D9_Device_ReInitialize( void ) ;															// �ꎞ�I�� Direct3DDevice9 ��j�������ۂɁA�j���O�̏�Ԃ����o�����߂̊֐�








// Direct3DDevice9 �X�e�[�^�X�֌W
extern	void	Graphics_D3D9_DeviceState_RefreshRenderState( void ) ;												// �`��ݒ�����t���b�V��
extern	int		Graphics_D3D9_DeviceState_SetBaseState( void ) ;													// Direct3DDevice9 �̊�{�ݒ���s��
extern	int		Graphics_D3D9_DeviceState_SetSampleFilterMode( int Sampler, int SetTarget, D_D3DTEXTUREFILTERTYPE FilterType ) ;		// �T���v���[�̃e�N�X�`���t�B���^�����O���[�h��ݒ肷��
extern	int		Graphics_D3D9_DeviceState_SetZEnable( int Flag )  ;													// �y�o�b�t�@�̗L�������̃Z�b�g
extern	int		Graphics_D3D9_DeviceState_SetZWriteEnable( int Flag ) ;												// �y�o�b�t�@�̏������݂̗L�����Z�b�g
extern	int		Graphics_D3D9_DeviceState_SetZFunc( int CmpType ) ;													// �[�x�l�̔�r�^�C�v���Z�b�g
extern	int		Graphics_D3D9_DeviceState_SetDepthBias( int Bias ) ;												// �[�x�l�̃o�C�A�X���Z�b�g
extern	int		Graphics_D3D9_DeviceState_SetFillMode( int FillMode ) ;												// �t�B�����[�h���Z�b�g
extern	int		Graphics_D3D9_DeviceState_SetWorldMatrix( const MATRIX *Matrix ) ;									// ���[���h�ϊ��p�s����Z�b�g����
extern	int		Graphics_D3D9_DeviceState_SetViewMatrix( const MATRIX *Matrix ) ;									// �r���[�ϊ��p�s����Z�b�g����
extern	int		Graphics_D3D9_DeviceState_SetProjectionMatrix( const MATRIX *Matrix ) ;								// ���e�ϊ��p�s����Z�b�g����
extern	int		Graphics_D3D9_DeviceState_SetLightState( int LightNumber, D_D3DLIGHT9 *Light ) ;					// ���C�g�̃Z�b�g
extern	int		Graphics_D3D9_DeviceState_SetLightEnable( int LightNumber, int EnableState ) ;						// ���C�g�̗L���A������ύX
extern	int		Graphics_D3D9_DeviceState_SetMaterial( D_D3DMATERIAL9 *Material ) ;									// �}�e���A���̃Z�b�g
extern	int		Graphics_D3D9_DeviceState_SetSpecularEnable( int UseFlag ) ;										// �X�y�L�������g�p���邩�ǂ������Z�b�g
extern	int		Graphics_D3D9_DeviceState_SetDiffuseMaterialSource( int UseFlag ) ;									// ���_�̃f�B�t���[�Y�J���[���}�e���A���̃f�B�t���[�Y�J���[�Ƃ��Ďg�p���邩�ǂ�����ݒ肷��
extern	int		Graphics_D3D9_DeviceState_SetSpecularMaterialSource( int UseFlag ) ;								// ���_�̃X�y�L�����J���[���}�e���A���̃X�y�L�����J���[�Ƃ��Ďg�p���邩�ǂ�����ݒ肷��
extern	int		Graphics_D3D9_DeviceState_SetCullMode( int State ) ;												// �J�����O�̗L�����Z�b�g 
extern	int		Graphics_D3D9_DeviceState_SetAmbient( unsigned int Color ) ;										// �A���r�G���g�J���[�̃Z�b�g
extern	int		Graphics_D3D9_DeviceState_SetShadeMode( int ShadeMode ) ;											// �V�F�[�f�B���O���[�h�̃Z�b�g
extern	int		Graphics_D3D9_DeviceState_SetTextureAddress( int AddressMode, int Sampler = -1 ) ;					// �e�N�X�`���[�̃A�h���b�V���O���[�h���Z�b�g����
extern	int		Graphics_D3D9_DeviceState_SetTextureAddressU( int AddressMode, int Sampler = -1 ) ;					// �e�N�X�`���[�̃A�h���b�V���O���[�h���Z�b�g����
extern	int		Graphics_D3D9_DeviceState_SetTextureAddressV( int AddressMode, int Sampler = -1 ) ;					// �e�N�X�`���[�̃A�h���b�V���O���[�h���Z�b�g����
extern	int		Graphics_D3D9_DeviceState_SetTextureAddressW( int AddressMode, int Sampler = -1 ) ;					// �e�N�X�`���[�̃A�h���b�V���O���[�h���Z�b�g����
extern	int		Graphics_D3D9_DeviceState_SetTextureAddressUVW( int AddressModeU, int AddressModeV, int AddressModeW, int Sampler = -1 ) ;		// �e�N�X�`���[�̃A�h���b�V���O���[�h���Z�b�g����
extern	int		Graphics_D3D9_DeviceState_SetTextureAddressTransformMatrix( int Use, MATRIX *Matrix, int Samplar = -1 ) ;	// �e�N�X�`�����W�ϊ��s����Z�b�g����
extern	int		Graphics_D3D9_DeviceState_SetFogEnable( int Flag ) ;												// �t�H�O��L���ɂ��邩�ǂ�����ݒ肷��( TRUE:�L��  FALSE:���� )
extern	int		Graphics_D3D9_DeviceState_SetFogVertexMode( int Mode /* DX_FOGMODE_NONE �� */ ) ;					// �t�H�O���[�h��ݒ肷��
extern	int		Graphics_D3D9_DeviceState_SetFogColor( unsigned int Color ) ;										// �t�H�O�J���[��ύX����
extern	int		Graphics_D3D9_DeviceState_SetFogStartEnd( float Start, float End ) ;								// �t�H�O���n�܂鋗���ƏI�����鋗����ݒ肷��( 0.0f �` 1.0f )
extern	int		Graphics_D3D9_DeviceState_SetFogDensity( float Density ) ;											// �t�H�O�̖��x��ݒ肷��( 0.0f �` 1.0f )
extern	int		Graphics_D3D9_DeviceState_SetLighting( int Flag ) ;													// ���C�g�̗L���t���O���Z�b�g����
extern	int		Graphics_D3D9_DeviceState_SetMaxAnisotropy( int MaxAnisotropy, int Sampler = -1 ) ;					// �ő�ٕ������Z�b�g����
extern	int		Graphics_D3D9_DeviceState_SetViewport( D_D3DVIEWPORT9 *Viewport ) ;									// �r���[�|�[�g���Z�b�g����
extern	int		Graphics_D3D9_DeviceState_SetViewportEasy( int x1, int y1, int x2, int y2 ) ;						// �r���[�|�[�g���Z�b�g����( �ȈՔ� )
extern	int		Graphics_D3D9_DeviceState_SetScissorRect( RECT *Rect ) ;											// �V�U�[��`��ݒ肷��
extern	int		Graphics_D3D9_DeviceState_SetDrawMode( int DrawMode ) ;												// �`�惂�[�h�̃Z�b�g
extern	int		Graphics_D3D9_DeviceState_SetRenderTarget( D_IDirect3DSurface9 *TargetSurface, int TargetIndex = 0 ) ;	// �`��Ώۂ̕ύX
extern	int		Graphics_D3D9_DeviceState_SetFVF( int FVF ) ;														// �g�p����FVF��ύX����
extern	int		Graphics_D3D9_DeviceState_SetVertexDeclaration( D_IDirect3DVertexDeclaration9 *VertexDeclaration ) ;	// �g�p���钸�_�V�F�[�_�錾��ύX����
#ifndef DX_NON_MODEL
extern	int		Graphics_D3D9_DeviceState_SetMV1VertexDeclaration( int BumpMap, int SkinMesh, int UVNum ) ;			// �w��̃��f���p�̒��_�V�F�[�_�錾���Z�b�g����
#endif // DX_NON_MODEL
extern	int		Graphics_D3D9_DeviceState_SetVertexShaderToHandle( int ShaderHandle ) ;								// �w��̒��_�V�F�[�_�[���f�o�C�X�ɃZ�b�g����
extern	int		Graphics_D3D9_DeviceState_SetVertexShader( D_IDirect3DVertexShader9 *VertexShader, int NormalVertexShader = FALSE ) ;	// �g�p���钸�_�V�F�[�_�[��ύX����
extern	int		Graphics_D3D9_DeviceState_ResetVertexShader( int SetNormalVertexShaderCancel = FALSE ) ;			// ���_�V�F�[�_�[�̎g�p���~�߂�( �Œ�@�\�p�C�v���C�����g�p�����Ԃɂ��� )
extern	int		Graphics_D3D9_DeviceState_SetPixelShaderToHandle( int ShaderHandle ) ;								// �w��̃s�N�Z���V�F�[�_�[���f�o�C�X�ɃZ�b�g����
extern	int		Graphics_D3D9_DeviceState_SetPixelShader( D_IDirect3DPixelShader9 *PixelShader, int NormalPixelShader = FALSE ) ;	// �g�p����s�N�Z���V�F�[�_�[��ύX����
extern	int		Graphics_D3D9_DeviceState_ResetPixelShader( int SetNormalPixelShaderCancel = FALSE ) ;				// �s�N�Z���V�F�[�_�[�̎g�p���~�߂�( �Œ�@�\�p�C�v���C�����g�p�����Ԃɂ��� )
extern	int		Graphics_D3D9_DeviceState_SetVertexBuffer( D_IDirect3DVertexBuffer9 *VertexBuffer, int VertexStride = 0 ) ;	// �g�p���钸�_�o�b�t�@��ύX����
extern	int		Graphics_D3D9_DeviceState_SetIndexBuffer( D_IDirect3DIndexBuffer9 *IndexBuffer ) ;					// �g�p����C���f�b�N�X�o�b�t�@��ύX����
extern	int		Graphics_D3D9_DeviceState_ResetTextureCoord( void ) ;												// �e�X�e�[�W���g�p����e�N�X�`���A�h���X���X�e�[�W�ԍ��Ɠ����ɂ���
extern	int		Graphics_D3D9_DeviceState_SetUserBlendInfo(  GRAPHICS_HARDDATA_DIRECT3D9_BLENDINFO *BlendInfo, int TextureStageIsTextureAndTextureCoordOnlyFlag, int TextureIsGraphHandleFlag = TRUE, int UseShader = TRUE ) ;					// ���[�U�[�̃u�����h�C���t�H��K������
extern	int		Graphics_D3D9_DeviceState_RefreshBlendState( GRAPHICS_HARDDATA_DIRECT3D9_BLENDINFO *pUserInfo = NULL, int TextureStageIsTextureAndTextureCoordOnlyFlag = FALSE, int TextureIsGraphHandleFlag = TRUE, int UseShader = TRUE ) ;	// �u�����h�ݒ���X�V(Direct3D �ɔ��f)����
extern	void	Graphics_D3D9_DeviceState_RefreshAmbientAndEmissiveParam( void ) ;									// �A���r�G���g���C�g�ƃG�~�b�V�u�J���[���|�����킹���p�����[�^���X�V����








// �`��ݒ�֌W�֐�
extern	int		Graphics_D3D9_DrawSetting_SetDrawBlendMode( int BlendMode, int AlphaTestValidFlag, int AlphaChannelValidFlag ) ;	// �`��u�����h���[�h�̐ݒ�
extern	int		Graphics_D3D9_DrawSetting_SetIgnoreDrawGraphColor( int EnableFlag ) ;							// �`�掞�̉摜�̐F�𖳎����邩�ǂ�����ݒ肷��
extern	int		Graphics_D3D9_DrawSetting_SetDrawAlphaTest( int TestMode, int TestParam ) ;					// �A���t�@�e�X�g�̐ݒ�
extern	int		Graphics_D3D9_DrawSetting_SetUseDiffuseAlphaColorFlag( int UseFlag ) ;						// �f�B�t���[�Y�J���[�̃��l���g�p���邩�ǂ����t���O���Z�b�g����
extern	int		Graphics_D3D9_DrawSetting_SetUseDiffuseRGBColorFlag( int UseFlag ) ;							// �f�B�t���[�Y�J���[�̂q�f�a�l���g�p���邩�ǂ����t���O���Z�b�g����
extern	int		Graphics_D3D9_DrawSetting_SetBlendTextureParam( int BlendType, int *Param ) ;					// �u�����h����e�N�X�`���̃p�����[�^���Z�b�g����
extern	int		Graphics_D3D9_DrawSetting_SetTexture( D_IDirect3DTexture9 *RenderTexture ) ;					// �e�N�X�`�����Z�b�g���� 
extern	int		Graphics_D3D9_DrawSetting_SetBlendTexture( D_IDirect3DTexture9 *BlendTexture, int TexWidth = -1, int TexHeight = -1 ) ;	// �u�����h����e�N�X�`�����Z�b�g���� 








// �V�F�[�_�[�萔�Ǘ��\���̏����֐�
extern	int		Graphics_D3D9_ShaderConstant_UseArea_Initialize(  DIRECT3D9_SHADERCONSTANT_USEAREA *UseArea, int TotalSize ) ;															// �V�F�[�_�[�萔�g�p�̈��������������
extern	int		Graphics_D3D9_ShaderConstant_UseArea_Set(         DIRECT3D9_SHADERCONSTANT_USEAREA *UseArea, int IsUse, int Index, int Num ) ;											// �V�F�[�_�[�萔�g�p�̈����ύX����
extern	int		Graphics_D3D9_ShaderConstant_UseArea_Create(      DIRECT3D9_SHADERCONSTANT_USEAREA *UseArea, BYTE *Map, BYTE SetNumber ) ;												// �V�F�[�_�[�萔�g�p�̈��񂩂�g�p�}�b�v���쐬����

extern	int		Graphics_D3D9_ShaderConstant_InfoSet_Initialize(  DIRECT3D9_SHADERCONSTANTINFOSET *ConstInfoSet ) ;																		// �V�F�[�_�[�萔���̏�����
extern	int		Graphics_D3D9_ShaderConstant_InfoSet_SetUseState( DIRECT3D9_SHADERCONSTANTINFOSET *ConstInfoSet, int ApplyMask ) ;														// �w��̃V�F�[�_�[�萔�Z�b�g��K�p���邩�ǂ�����ݒ肷�� DX_SHADERCONSTANTSET_MASK_LIB | DX_SHADERCONSTANTSET_MASK_LIB_SUB ��
extern	int		Graphics_D3D9_ShaderConstant_InfoSet_SetParam(    DIRECT3D9_SHADERCONSTANTINFOSET *ConstInfoSet, int TypeIndex, int SetIndex, int ConstantIndex, const void *Param, int ParamNum, int UpdateUseArea ) ;	// �V�F�[�_�[�萔����ݒ肷��
extern	int		Graphics_D3D9_ShaderConstant_InfoSet_ResetParam(  DIRECT3D9_SHADERCONSTANTINFOSET *ConstInfoSet, int TypeIndex, int SetIndex, int ConstantIndex, int ParamNum ) ;										// �w��̈�̃V�F�[�_�[�萔�������Z�b�g����








// ���_�V�F�[�_�錾�֌W�֐�
extern	int		Graphics_D3D9_VertexDeclaration_Create( void ) ;		// Direct3DVertexDeclaration9 �I�u�W�F�N�g�̍쐬( 0:����  -1:���s )
extern	int		Graphics_D3D9_VertexDeclaration_Terminate( void ) ;		// Direct3DVertexDeclaration9 �I�u�W�F�N�g�̍폜( 0:����  -1:���s )








// �V�X�e���e�N�X�`���E�T�[�t�F�X�֌W�֐�
extern	int		Graphics_D3D9_GetSysMemTextureFromOrig( struct IMAGEDATA_ORIG *Orig, struct IMAGEDATA_ORIG_HARD_TEX *OrigTex, int ASyncThread = FALSE ) ;		// IMAGEDATA_ORIG �� IMAGEDATA_ORIG_HARD_TEX �̏������� Graphics_D3D9_GetSysMemTexture ���g�p����
extern	int		Graphics_D3D9_GetSysMemTexture( int CubeMap, int Width, int Height, int MipMapCount, D_D3DFORMAT Format, int ASyncThread ) ;		// �Ǘ��e�N�X�`���ւ̓]���p�̃V�X�e���������e�N�X�`�����擾����
extern	int		Graphics_D3D9_GetSysMemSurface( int Width, int Height, D_D3DFORMAT Format, int ASyncThread ) ;										// �Ǘ��e�N�X�`���ւ̓]���p�̃V�X�e���������T�[�t�F�X���擾����
extern	int		Graphics_D3D9_CreateSysMemTextureDirect3D9Texture( GRAPHICS_HARDDATA_DIRECT3D9_SYSMEMTEXTURE *MemTex ) ;							// �\���̂̃f�[�^�����ɓ]���p�̃V�X�e���������e�N�X�`�����쐬����
extern	int		Graphics_D3D9_CreateSysMemSurfaceDirect3D9Surface( GRAPHICS_HARDDATA_DIRECT3D9_SYSMEMSURFACE *MemSurface ) ;						// �\���̂̃f�[�^�����ɓ]���p�̃V�X�e���������T�[�t�F�X���쐬����
extern	int		Graphics_D3D9_ReleaseSysMemTexture( int Index, int ASyncThread = FALSE ) ;															// �Ǘ��e�N�X�`���ւ̓]���p�̃V�X�e���������e�N�X�`�����������
extern	int		Graphics_D3D9_ReleaseSysMemSurface( int Index, int ASyncThread = FALSE ) ;															// �Ǘ��e�N�X�`���ւ̓]���p�̃V�X�e���������T�[�t�F�X���������
extern	int		Graphics_D3D9_ReleaseSysMemTextureAll( int IsNotUseOnly = FALSE ) ;																	// �Ǘ��e�N�X�`���ւ̓]���p�̃V�X�e���������e�N�X�`�������ׂĉ������
extern	int		Graphics_D3D9_ReleaseSysMemSurfaceAll( int IsNotUseOnly = FALSE ) ;																	// �Ǘ��e�N�X�`���ւ̓]���p�̃V�X�e���������T�[�t�F�X�����ׂĉ������

extern	int		Graphics_D3D9_SysMemTextureProcess( void ) ;																						// �Ǘ��e�N�X�`���ւ̓]���p�̃V�X�e���������e�N�X�`���̒���������s��
extern	int		Graphics_D3D9_SysMemSurfaceProcess( void ) ;																						// �Ǘ��e�N�X�`���ւ̓]���p�̃V�X�e���������T�[�t�F�X�̒���������s��








// Direct3D9 �̕`�揈�������֌W
extern	int		Graphics_D3D9_RenderVertex( int ASyncThread = FALSE ) ;							// ���_�o�b�t�@�ɗ��܂������_�f�[�^�������_�����O����
extern	void	FASTCALL Graphics_D3D9_DrawPreparation( int ParamFlag = 0 ) ;					// �`�揀�����s��
extern	void	Graphics_D3D9_BeginScene( void ) ;												// �r�M���V�[�����s��
extern	void	Graphics_D3D9_EndScene( void ) ;												// �G���h�V�[�����s��	








// Direct3D9 ���g�����`��֌W
extern	int		Graphics_D3D9_DrawBillboard3D( VECTOR Pos, float cx, float cy, float Size, float Angle, IMAGEDATA *Image, IMAGEDATA *BlendImage, int TransFlag, int TurnFlag, int DrawFlag = TRUE, RECT *DrawArea = NULL ) ;	// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawBillboard3D
extern	int		Graphics_D3D9_DrawModiBillboard3D( VECTOR Pos, float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, IMAGEDATA *Image, IMAGEDATA *BlendImage, int TransFlag, int DrawFlag = TRUE, RECT *DrawArea = NULL ) ;	// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawModiBillboard3D
extern	int		Graphics_D3D9_DrawGraph( int x, int y, float xf, float yf, IMAGEDATA *Image, IMAGEDATA *BlendImage, int TransFlag, int IntFlag ) ;																// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawGraph
extern	int		Graphics_D3D9_DrawExtendGraph( int x1, int y1, int x2, int y2, float x1f, float y1f, float x2f, float y2f, IMAGEDATA *Image, IMAGEDATA *BlendImage, int TransFlag, int IntFlag ) ;				// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawExtendGraph
extern	int		Graphics_D3D9_DrawRotaGraph( int x, int y, float xf, float yf, double ExRate, double Angle, IMAGEDATA *Image, IMAGEDATA *BlendImage, int TransFlag, int TurnFlag, int IntFlag ) ;				// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawRotaGraph
extern	int		Graphics_D3D9_DrawRotaGraphFast( int x, int y, float xf, float yf, float ExRate, float Angle, IMAGEDATA *Image, IMAGEDATA *BlendImage, int TransFlag, int TurnFlag, int IntFlag ) ;				// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawRotaGraphFast
extern	int		Graphics_D3D9_DrawModiGraph( int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, IMAGEDATA *Image, IMAGEDATA *BlendImage, int TransFlag, bool SimpleDrawFlag ) ;					// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawModiGraph
extern	int		Graphics_D3D9_DrawModiGraphF( float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, IMAGEDATA *Image, IMAGEDATA *BlendImage, int TransFlag, bool SimpleDrawFlag ) ;	// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawModiGraphF
extern	void	Graphics_D3D9_DrawModiTex( float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, IMAGEDATA_HARD_DRAW *DrawTex, bool SimpleDrawFlag ) ;								// �e�N�X�`����ό`���ĕ`�悷��
extern	int		Graphics_D3D9_DrawSimpleQuadrangleGraphF( const GRAPHICS_DRAW_DRAWSIMPLEQUADRANGLEGRAPHF_PARAM *Param, IMAGEDATA *Image, IMAGEDATA *BlendImage ) ;												// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawSimpleQuadrangleGraphF
extern	int		Graphics_D3D9_DrawSimpleTriangleGraphF(   const GRAPHICS_DRAW_DRAWSIMPLETRIANGLEGRAPHF_PARAM   *Param, IMAGEDATA *Image, IMAGEDATA *BlendImage ) ;												// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawSimpleTriangleGraphF

extern	int		Graphics_D3D9_DrawFillBox( int x1, int y1, int x2, int y2, unsigned int Color ) ;																		// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawFillBox
extern	int		Graphics_D3D9_DrawLineBox( int x1, int y1, int x2, int y2, unsigned int Color ) ;																		// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawLineBox
extern	int		Graphics_D3D9_DrawLine( int x1, int y1, int x2, int y2, unsigned int Color ) ;																			// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawLine
extern	int		Graphics_D3D9_DrawLine3D( VECTOR Pos1, VECTOR Pos2, unsigned int Color, int DrawFlag = TRUE, RECT *DrawArea = NULL ) ;									// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawLine3D
extern	int		Graphics_D3D9_DrawCircle_Thickness( int x, int y, int r, unsigned int Color, int Thickness ) ;															// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawCircle( �����w�肠�� )
extern	int		Graphics_D3D9_DrawOval_Thickness( int x, int y, int rx, int ry, unsigned int Color, int Thickness ) ;													// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawOval( �����w�肠�� )
extern	int		Graphics_D3D9_DrawCircle( int x, int y, int r, unsigned int Color, int FillFlag ) ;																		// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawCircle
extern	int		Graphics_D3D9_DrawOval( int x, int y, int rx, int ry, unsigned int Color, int FillFlag ) ;																// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawOval
extern	int		Graphics_D3D9_DrawTriangle( int x1, int y1, int x2, int y2, int x3, int y3, unsigned int Color, int FillFlag ) ;										// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawTriangle
extern	int		Graphics_D3D9_DrawTriangle3D( VECTOR Pos1, VECTOR Pos2, VECTOR Pos3, unsigned int Color, int FillFlag, int DrawFlag = TRUE, RECT *DrawArea = NULL ) ;	// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawTriangle3D
extern	int		Graphics_D3D9_DrawQuadrangle( int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, unsigned int Color, int FillFlag ) ;						// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawQuadrangle
extern	int		Graphics_D3D9_DrawPixel( int x, int y, unsigned int Color ) ;																							// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawPixel
extern	int		Graphics_D3D9_DrawPixel3D( VECTOR Pos, unsigned int Color, int DrawFlag = TRUE, RECT *DrawArea = NULL ) ;												// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawPixel3D
extern	int		Graphics_D3D9_DrawPixelSet( const POINTDATA *PointData, int Num ) ;																						// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawPixelSet
extern	int		Graphics_D3D9_DrawLineSet( const LINEDATA *LineData, int Num ) ;																						// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawLineSet

extern	int		Graphics_D3D9_DrawPrimitive(                             const VERTEX_3D *Vertex, int VertexNum,                                    int PrimitiveType, IMAGEDATA *Image, int TransFlag ) ;
extern	int		Graphics_D3D9_DrawIndexedPrimitive(                      const VERTEX_3D *Vertex, int VertexNum, const WORD *Indices, int IndexNum, int PrimitiveType, IMAGEDATA *Image, int TransFlag ) ;
extern	int		Graphics_D3D9_DrawPrimitiveLight(                        const VERTEX3D  *Vertex, int VertexNum,                                    int PrimitiveType, IMAGEDATA *Image, int TransFlag ) ;
extern	int		Graphics_D3D9_DrawIndexedPrimitiveLight(                 const VERTEX3D  *Vertex, int VertexNum, const WORD *Indices, int IndexNum, int PrimitiveType, IMAGEDATA *Image, int TransFlag ) ;
extern	int		Graphics_D3D9_DrawPrimitiveLight_UseVertexBuffer(        VERTEXBUFFERHANDLEDATA *VertexBuffer,                                      int PrimitiveType,                 int StartVertex, int UseVertexNum, IMAGEDATA *Image, int TransFlag ) ;
extern	int		Graphics_D3D9_DrawIndexedPrimitiveLight_UseVertexBuffer( VERTEXBUFFERHANDLEDATA *VertexBuffer, INDEXBUFFERHANDLEDATA *IndexBuffer,  int PrimitiveType, int BaseVertex, int StartVertex, int UseVertexNum, int StartIndex, int UseIndexNum, IMAGEDATA *Image, int TransFlag ) ;
extern	int		Graphics_D3D9_DrawPrimitive2D(                                 VERTEX_2D *Vertex, int VertexNum,                                    int PrimitiveType, IMAGEDATA *Image, int TransFlag, int BillboardFlag, int Is3D, int TurnFlag, int TextureNo, int IsShadowMap ) ;
extern	int		Graphics_D3D9_DrawPrimitive2DUser(                       const VERTEX2D  *Vertex, int VertexNum,                                    int PrimitiveType, IMAGEDATA *Image, int TransFlag, int Is3D, int TurnFlag, int TextureNo ) ;
extern	int		Graphics_D3D9_DrawIndexedPrimitive2DUser(                const VERTEX2D  *Vertex, int VertexNum, const WORD *Indices, int IndexNum, int PrimitiveType, IMAGEDATA *Image, int TransFlag ) ;

extern	void	Graphics_D3D9_DrawPreparationToShader( int ParamFlag, int UseVertexShaderNo, int Is2D, int UseLibSubShaderConst ) ;							// �V�F�[�_�[�`��p�`��O�Z�b�g�A�b�v�֐�

extern	int		Graphics_D3D9_DrawPrimitive2DToShader(        const VERTEX2DSHADER *Vertex, int VertexNum,                                              int PrimitiveType /* DX_PRIMTYPE_TRIANGLELIST �� */ ) ;		// �V�F�[�_�[���g���ĂQ�c�v���~�e�B�u��`�悷��
extern	int		Graphics_D3D9_DrawPrimitive3DToShader(        const VERTEX3DSHADER *Vertex, int VertexNum,                                              int PrimitiveType /* DX_PRIMTYPE_TRIANGLELIST �� */ ) ;		// �V�F�[�_�[���g���ĂR�c�v���~�e�B�u��`�悷��
extern	int		Graphics_D3D9_DrawPrimitiveIndexed2DToShader( const VERTEX2DSHADER *Vertex, int VertexNum, const unsigned short *Indices, int IndexNum, int PrimitiveType /* DX_PRIMTYPE_TRIANGLELIST �� */ ) ;		// �V�F�[�_�[���g���ĂQ�c�v���~�e�B�u��`�悷��( ���_�C���f�b�N�X���g�p���� )
extern	int		Graphics_D3D9_DrawPrimitiveIndexed3DToShader( const VERTEX3DSHADER *Vertex, int VertexNum, const unsigned short *Indices, int IndexNum, int PrimitiveType /* DX_PRIMTYPE_TRIANGLELIST �� */ ) ;		// �V�F�[�_�[���g���ĂR�c�v���~�e�B�u��`�悷��( ���_�C���f�b�N�X���g�p���� )

extern	int		Graphics_D3D9_DrawPrimitive3DToShader_UseVertexBuffer2(        int VertexBufHandle,                     int PrimitiveType /* DX_PRIMTYPE_TRIANGLELIST �� */, int StartVertex, int UseVertexNum ) ;	// �V�F�[�_�[���g���ĂR�c�v���~�e�B�u��`�悷��( ���_�o�b�t�@�g�p�� )
extern	int		Graphics_D3D9_DrawPrimitiveIndexed3DToShader_UseVertexBuffer2( int VertexBufHandle, int IndexBufHandle, int PrimitiveType /* DX_PRIMTYPE_TRIANGLELIST �� */, int BaseVertex, int StartVertex, int UseVertexNum, int StartIndex, int UseIndexNum ) ;	// �V�F�[�_�[���g���ĂR�c�v���~�e�B�u��`�悷��( ���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@�g�p�� )








// ���ˑ��������֌W
extern	int		Graphics_D3D9_Initialize_Timing0_PF( void ) ;											// �`�揈���̊��ˑ������̏��������s���֐�( ���s�ӏ���ʂO )
extern	int		Graphics_D3D9_Initialize_Timing1_PF( void ) ;											// �`�揈���̊��ˑ������̏��������s���֐�( ���s�ӏ���ʂQ )
extern	int		Graphics_D3D9_Hardware_Initialize_PF( void ) ;											// �n�[�h�E�G�A�A�N�Z�����[�^���g�p����ꍇ�̊��ˑ��̏������������s��
extern	int		Graphics_D3D9_Terminate_PF( void ) ;													// �`�揈���̊��ˑ������̌�n�����s���֐�
extern	int		Graphics_D3D9_RestoreOrChangeSetupGraphSystem_PF( int Change, int ScreenSizeX = -1, int ScreenSizeY = -1, int ColorBitDepth = -1, int RefreshRate = -1 ) ;		// �O���t�B�b�N�X�V�X�e���̕��A�A���͕ύX�t���̍ăZ�b�g�A�b�v���s��
extern	int		Graphics_D3D9_Hardware_CheckValid_PF( void ) ;											// �`��p�f�o�C�X���L�����ǂ������擾����( �߂�l  TRUE:�L��  FALSE:���� )
extern	void	Graphics_D3D9_ReleaseDirectXObject_PF( void ) ;											// DirectX �̃I�u�W�F�N�g���������








// ���ˑ��V�X�e���֌W
extern	void	Graphics_D3D9_WM_ACTIVATE_ActiveProcess_PF( void ) ;									// WM_ACTIVATE ���b�Z�[�W�̏����ŁA�E�C���h�E���[�h���ɃA�N�e�B�u�ɂȂ����ۂɌĂ΂��֐�









// ���ˑ��`��ݒ�֌W
extern	int		Graphics_Hardware_D3D9_SetRenderTargetToShader_PF( int TargetIndex, int DrawScreen, int SurfaceIndex ) ;	// �V�F�[�_�[�`��ł̕`����ݒ肷��
extern	int		Graphics_Hardware_D3D9_SetDrawBrightToOneParam_PF( DWORD Bright ) ;											// SetDrawBright �̈��������
extern	int		Graphics_Hardware_D3D9_SetDrawBlendMode_PF( int BlendMode, int BlendParam ) ;								// �`��u�����h���[�h���Z�b�g����
extern	int		Graphics_Hardware_D3D9_SetDrawAlphaTest_PF( int TestMode, int TestParam ) ;									// �`�掞�̃A���t�@�e�X�g�̐ݒ���s��( TestMode:DX_CMP_GREATER��( -1:�f�t�H���g����ɖ߂� )  TestParam:�`��A���t�@�l�Ƃ̔�r�Ɏg�p����l )
extern	int		Graphics_Hardware_D3D9_SetDrawMode_PF( int DrawMode ) ;														// �`�惂�[�h���Z�b�g����
extern	int		Graphics_Hardware_D3D9_SetDrawBright_PF( int RedBright, int GreenBright, int BlueBright ) ;					// �`��P�x���Z�b�g
extern	int		Graphics_Hardware_D3D9_SetBlendGraphParamBase_PF( IMAGEDATA *BlendImage, int BlendType, int *Param ) ;		// SetBlendGraphParam �̉ϒ������p�����[�^�t��
extern	int		Graphics_Hardware_D3D9_SetMaxAnisotropy_PF( int MaxAnisotropy ) ;											// �ő�ٕ����̒l���Z�b�g����
extern	int		Graphics_Hardware_D3D9_SetTransformToWorld_PF( const MATRIX *Matrix ) ;										// ���[���h�ϊ��p�s����Z�b�g����
extern	int		Graphics_Hardware_D3D9_SetTransformToView_PF( const MATRIX *Matrix ) ;										// �r���[�ϊ��p�s����Z�b�g����
extern	int		Graphics_Hardware_D3D9_SetTransformToProjection_PF( const MATRIX *Matrix ) ;								// ���e�ϊ��p�s����Z�b�g����
extern	int		Graphics_Hardware_D3D9_SetTransformToViewport_PF( const MATRIX *Matrix ) ;									// �r���[�|�[�g�s����Z�b�g����
extern	int		Graphics_Hardware_D3D9_SetTextureAddressMode_PF( int Mode /* DX_TEXADDRESS_WRAP �� */, int Stage ) ;		// �e�N�X�`���A�h���X���[�h��ݒ肷��
extern	int		Graphics_Hardware_D3D9_SetTextureAddressModeUV_PF( int ModeU, int ModeV, int Stage ) ;						// �e�N�X�`���A�h���X���[�h��ݒ肷��
extern	int		Graphics_Hardware_D3D9_SetTextureAddressTransformMatrix_PF( int UseFlag, MATRIX *Matrix, int Sampler = -1 ) ;// �e�N�X�`�����W�ϊ��s����Z�b�g����
extern	int		Graphics_Hardware_D3D9_SetFogEnable_PF( int Flag ) ;														// �t�H�O��L���ɂ��邩�ǂ�����ݒ肷��( TRUE:�L��  FALSE:���� )
extern	int		Graphics_Hardware_D3D9_SetFogMode_PF( int Mode /* DX_FOGMODE_NONE �� */ ) ;									// �t�H�O���[�h��ݒ肷��
extern	int		Graphics_Hardware_D3D9_SetFogColor_PF( DWORD FogColor ) ;													// �t�H�O�J���[��ύX����
extern	int		Graphics_Hardware_D3D9_SetFogStartEnd_PF( float start, float end ) ;										// �t�H�O���n�܂鋗���ƏI�����鋗����ݒ肷��( 0.0f �` 1.0f )
extern	int		Graphics_Hardware_D3D9_SetFogDensity_PF( float density ) ;													// �t�H�O�̖��x��ݒ肷��( 0.0f �` 1.0f )
extern	int		Graphics_Hardware_D3D9_ApplyLigFogToHardware_PF( void ) ;													// ��{�f�[�^�ɐݒ肳��Ă���t�H�O�����n�[�h�E�F�A�ɔ��f����
extern	int		Graphics_Hardware_D3D9_SetUseOldDrawModiGraphCodeFlag_PF( int Flag ) ;										// �ȑO�� DrawModiGraph �֐��̃R�[�h���g�p���邩�ǂ����̃t���O���Z�b�g����
extern	int		Graphics_Hardware_D3D9_RefreshAlphaChDrawMode_PF( void ) ;													// �`���ɐ��������l���������ނ��ǂ����̃t���O���X�V����








// ���ˑ��ݒ�֌W
extern	int		Graphics_Hardware_D3D9_SetUseHardwareVertexProcessing_PF( int Flag ) ;						// �n�[�h�E�G�A�̒��_���Z�����@�\���g�p���邩�ǂ�����ݒ肷��
extern	int		Graphics_Hardware_D3D9_SetUsePixelLighting_PF( int Flag ) ;									// �s�N�Z���P�ʂŃ��C�e�B���O���s�����ǂ�����ݒ肷��A�v ShaderModel 3.0( TRUE:�s�N�Z���P�ʂ̃��C�e�B���O���s��  FALSE:���_�P�ʂ̃��C�e�B���O���s��( �f�t�H���g ) )
extern	int		Graphics_Hardware_D3D9_SetGraphicsDeviceRestoreCallbackFunction_PF( void (* Callback )( void *Data ), void *CallbackData ) ;			// �O���t�B�b�N�X�f�o�C�X�����X�g���畜�A�����ۂɌĂ΂��R�[���o�b�N�֐���ݒ肷��
extern	int		Graphics_Hardware_D3D9_SetGraphicsDeviceLostCallbackFunction_PF( void (* Callback )( void *Data ), void *CallbackData ) ;			// �O���t�B�b�N�X�f�o�C�X�����X�g���畜�A����O�ɌĂ΂��R�[���o�b�N�֐���ݒ肷��
extern	int		Graphics_Hardware_D3D9_SetUseNormalDrawShader_PF( int Flag ) ;								// �ʏ�`��Ƀv���O���}�u���V�F�[�_�[���g�p���邩�ǂ�����ݒ肷��( TRUE:�g�p����( �f�t�H���g )  FALSE:�g�p���Ȃ� )
extern	int		Graphics_Hardware_D3D9_GetVideoMemorySize_PF( int *AllSize, int *FreeSize ) ;				// �r�f�I�������̗e�ʂ𓾂�
extern	const D_IDirect3DDevice9 * Graphics_Hardware_D3D9_GetUseDirect3DDevice9_PF( void ) ;				// �g�p���̂c�����������R�c�c�����������X�I�u�W�F�N�g�𓾂�
extern	const D_IDirect3DSurface9* Graphics_Hardware_D3D9_GetUseDirect3D9BackBufferSurface_PF( void ) ;		// �g�p���̃o�b�N�o�b�t�@��Direct3DSurface9�I�u�W�F�N�g���擾����
extern	int		Graphics_Hardware_D3D9_RefreshDxLibDirect3DSetting_PF( void ) ;								// �c�w���C�u�����̂c�����������R�c�ݒ�����Ȃ���
extern	int		Graphics_D3D9_SetDDrawUseGuid_PF( const GUID FAR *Guid ) ;									// �c�����������c���������g�p����f�t�h�c��ݒ肷��
extern	const GUID *Graphics_D3D9_GetDirectDrawDeviceGUID_PF( int Number ) ;								// �L���� DirectDraw �f�o�C�X�� GUID ���擾����
extern	int		Graphics_D3D9_GetDirectDrawDeviceDescription_PF( int Number, char *StringBuffer ) ;			// �L���� DirectDraw �f�o�C�X�̖��O�𓾂�
extern	int		Graphics_D3D9_GetDirectDrawDeviceNum_PF( void ) ;											// �L���� DirectDraw �f�o�C�X�̐����擾����
//extern	int		Graphics_D3D9_SetUseDirectDrawDeviceIndex_PF( int Index ) ;									// �g�p���� DirectDraw �f�o�C�X�̃C���f�b�N�X��ݒ肷��
extern	int		Graphics_D3D9_SetUseDirect3D9Ex_PF( int Flag ) ;											// Vista�ȍ~�̊��� Direct3D9Ex ���g�p���邩�ǂ�����ݒ肷��( TRUE:�g�p����( �f�t�H���g )  FALSE:�g�p���Ȃ� )
extern	int		Graphics_D3D9_SetDialogBoxMode_PF( int Flag ) ;												// �_�C�A���O�{�b�N�X���[�h��ύX����








// ���ˑ���ʊ֌W
extern	int		Graphics_Hardware_D3D9_SetupUseZBuffer_PF( void ) ;															// �ݒ�Ɋ�Â��Ďg�p����y�o�b�t�@���Z�b�g����
extern	int		Graphics_Hardware_D3D9_ClearDrawScreenZBuffer_PF( const RECT *ClearRect ) ;									// ��ʂ̂y�o�b�t�@�̏�Ԃ�����������
extern	int		Graphics_Hardware_D3D9_ClearDrawScreen_PF( const RECT *ClearRect ) ;										// ��ʂ̏�Ԃ�����������
extern	int		Graphics_Hardware_D3D9_SetDrawScreen_PF( int DrawScreen, int OldScreenSurface, IMAGEDATA *NewTargetImage, IMAGEDATA *OldTargetImage, SHADOWMAPDATA *NewTargetShadowMap, SHADOWMAPDATA *OldTargetShadowMap ) ;						// �`����ʂ̃Z�b�g
extern	int		Graphics_Hardware_D3D9_SetDrawScreen_Post_PF( int DrawScreen ) ;											// SetDrawScreen �̍Ō�ŌĂ΂��֐�
extern	int		Graphics_Hardware_D3D9_SetDrawArea_PF( int x1, int y1, int x2, int y2 ) ;									// �`��\�̈�̃Z�b�g
extern	int		Graphics_Hardware_D3D9_LockDrawScreenBuffer_PF( RECT *LockRect, BASEIMAGE *BaseImage, int TargetScreen, IMAGEDATA *TargetImage, int TargetScreenSurface, int ReadOnly, int TargetScreenTextureNo ) ;	// �`���o�b�t�@�����b�N����
extern	int		Graphics_Hardware_D3D9_UnlockDrawScreenBuffer_PF( void ) ;													// �`���o�b�t�@���A�����b�N����
extern	int		Graphics_Hardware_D3D9_ScreenCopy_PF( void ) ;																// ����ʂ̓��e��\��ʂɕ`�悷��
extern	int		Graphics_Hardware_D3D9_SaveDrawScreen_Begin_PF( int x1, int y1, int x2, int y2, BASEIMAGE *BaseImage ) ;	// ���ݕ`��ΏۂɂȂ��Ă����ʂ�ۑ�����
extern	int		Graphics_Hardware_D3D9_SaveDrawScreen_Post_PF( void ) ;														// ���ݕ`��ΏۂɂȂ��Ă����ʂ�ۑ�����
extern	int		Graphics_Hardware_D3D9_WaitVSync_PF( int SyncNum ) ;														// ���������M����҂�
extern	int		Graphics_Hardware_D3D9_ScreenFlipBase_PF( void ) ;															// ����ʂƕ\��ʂ���������
extern	int		Graphics_Hardware_D3D9_BltRectBackScreenToWindow_PF( HWND Window, RECT BackScreenRect, RECT WindowClientRect ) ;		// ����ʂ̎w��̗̈���E�C���h�E�̃N���C�A���g�̈�̎w��̗̈�ɓ]������
extern	int		Graphics_Hardware_D3D9_SetScreenFlipTargetWindow_PF( HWND TargetWindow ) ;									// ScreenFlip �ŉ摜��]�������̃E�C���h�E��ݒ肷��( NULL ���w�肷��Ɛݒ���� )
extern	int		Graphics_Hardware_D3D9_SetZBufferMode_PF( int ZBufferSizeX, int ZBufferSizeY, int ZBufferBitDepth ) ;		// ���C����ʂ̂y�o�b�t�@�̐ݒ��ύX����
extern	int		Graphics_Hardware_D3D9_SetDrawZBuffer_PF( int DrawScreen, IMAGEDATA *Image ) ;								// �`���y�o�b�t�@�̃Z�b�g








// ���ˑ����擾�֌W
extern	const COLORDATA *	Graphics_Hardware_D3D9_GetMainColorData_PF( void ) ;				// GetColor �� GetColor2 �Ŏg�p����J���[�f�[�^���擾����
extern	const COLORDATA *	Graphics_Hardware_D3D9_GetDispColorData_PF( void ) ;				// �f�B�X�v���[�̃J���[�f�[�^�|�C���^�𓾂�
extern	DWORD				Graphics_Hardware_D3D9_GetPixel_PF( int x, int y ) ;				// �w����W�̐F���擾����








// ���ˑ��摜�֌W
extern	int		Graphics_Hardware_D3D9_UpdateGraphMovie_TheoraYUV_PF( MOVIEGRAPH *Movie, IMAGEDATA *Image ) ;			// YUV�T�[�t�F�X���g���� Theora ����̓��e���O���t�B�b�N�X�n���h���̃e�N�X�`���ɓ]������
extern	int		Graphics_Hardware_D3D9_GraphLock_PF( IMAGEDATA *Image, COLORDATA **ColorDataP, int WriteOnly ) ;		// �O���t�B�b�N�������̈�̃��b�N
extern	int		Graphics_Hardware_D3D9_GraphUnlock_PF( IMAGEDATA *Image ) ;												// �O���t�B�b�N�������̈�̃��b�N����
extern	int		Graphics_Hardware_D3D9_CopyGraphZBufferImage_PF( IMAGEDATA *DestImage, IMAGEDATA *SrcImage ) ;			// �O���t�B�b�N�̂y�o�b�t�@�̏�Ԃ�ʂ̃O���t�B�b�N�̂y�o�b�t�@�ɃR�s�[����( DestGrHandle �� SrcGrHandle ���y�o�b�t�@�������Ă���`��\�摜�ŁA���A���`�G�C���A�X�摜�ł͂Ȃ����Ƃ����� )
extern	int		Graphics_Hardware_D3D9_InitGraph_PF( void ) ;															// �摜�f�[�^�̏�����
extern	int		Graphics_Hardware_D3D9_FillGraph_PF( IMAGEDATA *Image, int Red, int Green, int Blue, int Alpha, int ASyncThread ) ;			// �O���t�B�b�N�����̐F�œh��Ԃ�
extern	int		Graphics_Hardware_D3D9_GetDrawScreenGraphBase_PF( IMAGEDATA *Image, IMAGEDATA *TargetImage, int TargetScreen, int TargetScreenSurface, int TargetScreenWidth, int TargetScreenHeight, int x1, int y1, int x2, int y2, int destX, int destY ) ;		// �`��\�摜��o�b�N�o�b�t�@����w��̈�̃O���t�B�b�N���擾����

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
) ;

extern	int		Graphics_Hardware_D3D9_CreateOrigTexture_PF(  IMAGEDATA_ORIG *Orig, int ASyncThread = FALSE ) ;		// �I���W�i���摜��񒆂̃e�N�X�`�����쐬����( 0:����  -1:���s )
extern	int		Graphics_Hardware_D3D9_ReleaseOrigTexture_PF( IMAGEDATA_ORIG *Orig ) ;								// �I���W�i���摜��񒆂̃e�N�X�`�����������
extern	int		Graphics_Hardware_D3D9_GetMultiSampleQuality_PF( int Samples ) ;									// �w��̃}���`�T���v�����Ŏg�p�ł���ő�N�I���e�B�l���擾����
extern	int		Graphics_Hardware_D3D9_SetUsePlatformTextureFormat_PF( int PlatformTextureFormat ) ;				// �쐬����O���t�B�b�N�n���h���Ŏg�p������ˑ��̃e�N�X�`���t�H�[�}�b�g���w�肷��








// ���ˑ����_�o�b�t�@�E�C���f�b�N�X�o�b�t�@�֌W
extern	int		Graphics_Hardware_D3D9_VertexBuffer_Create_PF(    VERTEXBUFFERHANDLEDATA *VertexBuffer ) ;															// ���_�o�b�t�@�n���h���̒��_�o�b�t�@���쐬����
extern	int		Graphics_Hardware_D3D9_VertexBuffer_Terminate_PF( VERTEXBUFFERHANDLEDATA *VertexBuffer ) ;															// ���_�o�b�t�@�n���h���̌�n��
extern	int		Graphics_Hardware_D3D9_VertexBuffer_SetData_PF(   VERTEXBUFFERHANDLEDATA *VertexBuffer, int SetIndex, const void *VertexData, int VertexNum ) ;		// ���_�o�b�t�@�ɒ��_�f�[�^��]������
extern	int		Graphics_Hardware_D3D9_IndexBuffer_Create_PF(     INDEXBUFFERHANDLEDATA *IndexBuffer ) ;															// �C���f�b�N�X�o�b�t�@�n���h���̃Z�b�g�A�b�v���s��
extern	int		Graphics_Hardware_D3D9_IndexBuffer_Terminate_PF(  INDEXBUFFERHANDLEDATA *IndexBuffer ) ;															// �C���f�b�N�X�o�b�t�@�n���h���̌�n��
extern	int		Graphics_Hardware_D3D9_IndexBuffer_SetData_PF(    INDEXBUFFERHANDLEDATA *IndexBuffer, int SetIndex, const void *IndexData, int IndexNum ) ;			// �C���f�b�N�X�o�b�t�@�ɃC���f�b�N�X�f�[�^��]������









// ���ˑ����C�g�֌W
extern	int		Graphics_Hardware_D3D9_Light_SetUse_PF( int Flag ) ;															// ���C�e�B���O���s�����ǂ�����ݒ肷��
extern	int		Graphics_Hardware_D3D9_Light_GlobalAmbient_PF( COLOR_F *Color ) ;												// �O���[�o���A���r�G���g���C�g�J���[��ݒ肷��
extern	int		Graphics_Hardware_D3D9_Light_SetState_PF( int LightNumber, LIGHTPARAM *LightParam ) ;							// ���C�g�p�����[�^���Z�b�g
extern	int		Graphics_Hardware_D3D9_Light_SetEnable_PF( int LightNumber, int EnableState ) ;									// ���C�g�̗L���A������ύX








// ���ˑ��V���h�E�}�b�v�֌W
extern	int		Graphics_Hardware_D3D9_ShadowMap_CreateTexture_PF( SHADOWMAPDATA *ShadowMap, int ASyncThread = FALSE ) ;		// �V���h�E�}�b�v�f�[�^�ɕK�v�ȃe�N�X�`�����쐬����
extern	int		Graphics_Hardware_D3D9_ShadowMap_ReleaseTexture_PF( SHADOWMAPDATA *ShadowMap ) ;								// �V���h�E�}�b�v�f�[�^�ɕK�v�ȃe�N�X�`�����������
extern	void	Graphics_Hardware_D3D9_ShadowMap_RefreshVSParam_PF( void ) ;													// ���_�V�F�[�_�[�ɐݒ肷��V���h�E�}�b�v�̏����X�V����
extern	void	Graphics_Hardware_D3D9_ShadowMap_RefreshPSParam_PF( void ) ;													// �s�N�Z���V�F�[�_�[�ɐݒ肷��V���h�E�}�b�v�̏����X�V����
extern	int		Graphics_Hardware_D3D9_ShadowMap_DrawSetup_PF( SHADOWMAPDATA *ShadowMap ) ;										// �V���h�E�}�b�v�ւ̕`��̏������s��
extern	int		Graphics_Hardware_D3D9_ShadowMap_DrawEnd_PF( SHADOWMAPDATA *ShadowMap ) ;										// �V���h�E�}�b�v�ւ̕`����I������
extern	int		Graphics_Hardware_D3D9_ShadowMap_SetUse_PF( int SlotIndex, SHADOWMAPDATA *ShadowMap ) ;							// �`��Ŏg�p����V���h�E�}�b�v���w�肷��A�X���b�g�͂O���P�����w��\








// ���ˑ��V�F�[�_�[�֌W
extern	int		Graphics_Hardware_D3D9_Shader_Create_PF( int ShaderHandle, int IsVertexShader, void *Image, int ImageSize, int ImageAfterFree, int ASyncThread ) ;		// �V�F�[�_�[�n���h��������������
extern	int		Graphics_Hardware_D3D9_Shader_TerminateHandle_PF( SHADERHANDLEDATA *Shader ) ;																			// �V�F�[�_�[�n���h���̌�n��
extern	int		Graphics_Hardware_D3D9_Shader_GetValidShaderVersion_PF( void ) ;																							// �g�p�ł���V�F�[�_�[�̃o�[�W�������擾����( 0=�g���Ȃ�  200=�V�F�[�_�[���f���Q�D�O���g�p�\  300=�V�F�[�_�[���f���R�D�O���g�p�\ )
extern	int		Graphics_Hardware_D3D9_Shader_GetConstIndex_PF( const wchar_t *ConstantName, SHADERHANDLEDATA *Shader ) ;													// �w��̖��O�����萔���g�p����V�F�[�_�[�萔�̔ԍ����擾����
extern	int		Graphics_Hardware_D3D9_Shader_GetConstCount_PF( const wchar_t *ConstantName, SHADERHANDLEDATA *Shader ) ;													// �w��̖��O�����萔���g�p����V�F�[�_�[�萔�̐����擾����
extern	const FLOAT4 *Graphics_Hardware_D3D9_Shader_GetConstDefaultParamF_PF( const wchar_t *ConstantName, SHADERHANDLEDATA *Shader ) ;									// �w��̖��O�������������_�萔�̃f�t�H���g�p�����[�^���i�[����Ă��郁�����A�h���X���擾����
extern	int		Graphics_Hardware_D3D9_Shader_SetConst_PF(   int TypeIndex, int SetIndex, int ConstantIndex, const void *Param, int ParamNum, int UpdateUseArea ) ;	// �V�F�[�_�[�萔����ݒ肷��
extern	int		Graphics_Hardware_D3D9_Shader_ResetConst_PF( int TypeIndex, int SetIndex, int ConstantIndex, int ParamNum ) ;											// �w��̈�̃V�F�[�_�[�萔�������Z�b�g����
extern	int		Graphics_Hardware_D3D9_Shader_ModelCode_Init_PF( void ) ;																								// �R�c���f���p�̃V�F�[�_�[�R�[�h�̏��������s��








// ���ˑ��V�F�[�_�[�p�萔�o�b�t�@�֌W
extern	int		Graphics_Hardware_D3D9_ShaderConstantBuffer_Create_PF( int ShaderConstantBufferHandle, int BufferSize, int ASyncThread ) ;												// �V�F�[�_�[�p�萔�o�b�t�@�n���h��������������
extern	int		Graphics_Hardware_D3D9_ShaderConstantBuffer_TerminateHandle_PF( SHADERCONSTANTBUFFERHANDLEDATA *ShaderConstantBuffer ) ;													// �V�F�[�_�[�p�萔�o�b�t�@�n���h���̌�n��
extern	void *	Graphics_Hardware_D3D9_ShaderConstantBuffer_GetBuffer_PF( SHADERCONSTANTBUFFERHANDLEDATA *ShaderConstantBuffer ) ;														// �V�F�[�_�[�p�萔�o�b�t�@�n���h���̒萔�o�b�t�@�̃A�h���X���擾����
extern	int		Graphics_Hardware_D3D9_ShaderConstantBuffer_Update_PF( SHADERCONSTANTBUFFERHANDLEDATA *ShaderConstantBuffer ) ;															// �V�F�[�_�[�p�萔�o�b�t�@�n���h���̒萔�o�b�t�@�ւ̕ύX��K�p����
extern	int		Graphics_Hardware_D3D9_ShaderConstantBuffer_Set_PF( SHADERCONSTANTBUFFERHANDLEDATA *ShaderConstantBuffer, int TargetShader /* DX_SHADERTYPE_VERTEX �Ȃ� */, int Slot ) ;	// �V�F�[�_�[�p�萔�o�b�t�@�n���h���̒萔�o�b�t�@���w��̃V�F�[�_�[�̎w��̃X���b�g�ɃZ�b�g����








// ���ˑ��`��֌W
extern	int		Graphics_Hardware_D3D9_Paint_PF( int x, int y, unsigned int FillColor, ULONGLONG BoundaryColor ) ;			// �w��_���狫�E�F������Ƃ���܂œh��Ԃ�






#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

#endif // DX_NON_DIRECT3D9

#endif // DX_NON_GRAPHICS

#endif // __DXGRAPHICSD3D9_H__
