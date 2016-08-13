// -------------------------------------------------------------------------------
// 
// 		�c�w���C�u����		�`��v���O�����w�b�_�t�@�C��
// 
// 				Ver 3.16d
// 
// -------------------------------------------------------------------------------

#ifndef __DXGRAPHICS_H__
#define __DXGRAPHICS_H__

#include "DxCompileConfig.h"

#ifndef DX_NON_GRAPHICS

// �C���N���[�h ------------------------------------------------------------------
#include "DxLib.h"
#include "DxStatic.h"
#include "DxHandle.h"
#include "DxMemImg.h"
#include "DxMovie.h"
#include "DxArchive_.h"
#include "DxBaseImage.h"
#include <stdarg.h>

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// �}�N����` --------------------------------------------------------------------

#define GSYS						GraphicsSysData
#define MASKD						MaskManageData

// �L�^���Ă����L���ɂȂ��Ă��郉�C�g�̃C���f�b�N�X�̐�
#define MAX_HARDWAREENABLELIGHTINDEX_NUM	(256)

// �����ɕ`��ł��郌���_�����O�^�[�Q�b�g�̍ő吔
#define DX_RENDERTARGET_COUNT				(8)

// �g�p����e�N�X�`���X�e�[�W�̍ő吔
#define USE_TEXTURESTAGE_NUM				(16)

// �����ɓK�p�ł���V���h�E�}�b�v�̍ő吔
#define MAX_USE_SHADOWMAP_NUM				(3)

// ���L�o�b�t�@�̐�
#define COMMON_BUFFER_NUM					(3)

// ���_�^�C�v
#define VERTEXTYPE_NOTEX					(0)			// �e�N�X�`�����g�p���Ȃ�
#define VERTEXTYPE_TEX						(1)			// �e�N�X�`������g�p����
#define VERTEXTYPE_BLENDTEX					(2)			// �u�����h�e�N�X�`�����g�p����
#define VERTEXTYPE_NUM						(3)

// �O���t�B�b�N�n���h���̗L�����`�F�b�N
#define GRAPHCHKFULL( HAND, GPOINT )			HANDLECHKFULL(       DX_HANDLETYPE_GRAPH, HAND, *( ( HANDLEINFO ** )&GPOINT ) )
#define GRAPHCHKFULL_ASYNC( HAND, GPOINT )		HANDLECHKFULL_ASYNC( DX_HANDLETYPE_GRAPH, HAND, *( ( HANDLEINFO ** )&GPOINT ) )
#define GRAPHCHK( HAND, GPOINT )				HANDLECHK(           DX_HANDLETYPE_GRAPH, HAND, *( ( HANDLEINFO ** )&GPOINT ) )
#define GRAPHCHK_ASYNC( HAND, GPOINT )			HANDLECHK_ASYNC(     DX_HANDLETYPE_GRAPH, HAND, *( ( HANDLEINFO ** )&GPOINT ) )

// �V���h�E�}�b�v�n���h���̗L�����`�F�b�N
#define SHADOWMAPCHKFULL( HAND, SPOINT )		HANDLECHKFULL(       DX_HANDLETYPE_SHADOWMAP, HAND, *( ( HANDLEINFO ** )&SPOINT ) )
#define SHADOWMAPCHKFULL_ASYNC( HAND, SPOINT )	HANDLECHKFULL_ASYNC( DX_HANDLETYPE_SHADOWMAP, HAND, *( ( HANDLEINFO ** )&SPOINT ) )
#define SHADOWMAPCHK( HAND, SPOINT )			HANDLECHK(           DX_HANDLETYPE_SHADOWMAP, HAND, *( ( HANDLEINFO ** )&SPOINT ) )
#define SHADOWMAPCHK_ASYNC( HAND, SPOINT )		HANDLECHK_ASYNC(     DX_HANDLETYPE_SHADOWMAP, HAND, *( ( HANDLEINFO ** )&SPOINT ) )

// �V�F�[�_�[�n���h���̗L�����`�F�b�N
#define SHADERCHK( HAND, SPOINT )				HANDLECHK(       DX_HANDLETYPE_SHADER, HAND, *( ( HANDLEINFO ** )&SPOINT ) )
#define SHADERCHK_ASYNC( HAND, SPOINT )			HANDLECHK_ASYNC( DX_HANDLETYPE_SHADER, HAND, *( ( HANDLEINFO ** )&SPOINT ) )

// �V�F�[�_�[�p�萔�o�b�t�@�n���h���̗L�����`�F�b�N
#define SHADERCONSTANTBUFFERCHK( HAND, SPOINT )				HANDLECHK(       DX_HANDLETYPE_SHADER_CONSTANT_BUFFER, HAND, *( ( HANDLEINFO ** )&SPOINT ) )
#define SHADERCONSTANTBUFFERCHK_ASYNC( HAND, SPOINT )		HANDLECHK_ASYNC( DX_HANDLETYPE_SHADER_CONSTANT_BUFFER, HAND, *( ( HANDLEINFO ** )&SPOINT ) )

// ���_�o�b�t�@�n���h���̗L�����`�F�b�N
#define VERTEXBUFFERCHK( HAND, SPOINT )			HANDLECHK(       DX_HANDLETYPE_VERTEX_BUFFER, HAND, *( ( HANDLEINFO ** )&SPOINT ) )
#define VERTEXBUFFERCHK_ASYNC( HAND, SPOINT )	HANDLECHK_ASYNC( DX_HANDLETYPE_VERTEX_BUFFER, HAND, *( ( HANDLEINFO ** )&SPOINT ) )

// �C���f�b�N�X�o�b�t�@�n���h���̗L�����`�F�b�N
#define INDEXBUFFERCHK( HAND, SPOINT )			HANDLECHK(       DX_HANDLETYPE_INDEX_BUFFER, HAND, *( ( HANDLEINFO ** )&SPOINT ) )
#define INDEXBUFFERCHK_ASYNC( HAND, SPOINT )	HANDLECHK_ASYNC( DX_HANDLETYPE_INDEX_BUFFER, HAND, *( ( HANDLEINFO ** )&SPOINT ) )

// �J�����̐ݒ�^�C�v
#define CAMERA_SETUPTYPE_POS_TARG		(0)
#define CAMERA_SETUPTYPE_POS_TARG_UP	(1)
#define CAMERA_SETUPTYPE_POS_ANGLE		(2)
#define CAMERA_SETUPTYPE_MATRIX			(3)

// �y�o�b�t�@�t�H�[�}�b�g
#define ZBUFFER_FORMAT_16BIT		(0)
#define ZBUFFER_FORMAT_24BIT		(1)
#define ZBUFFER_FORMAT_32BIT		(2)
#define ZBUFFER_FORMAT_NUM			(3)

// �`��ݒ�^�C�v
#define DX_DRAWSETTING_2D			(0)			// �ʏ�̂Q�c�`��p�̐ݒ�
#define DX_DRAWSETTING_SHADER		(1)			// �V�F�[�_�[���g�����`��p�̐ݒ�
#define DX_DRAWSETTING_MASK			(2)			// �}�X�N�����p�̐ݒ�
#define DX_DRAWSETTING_TOONOUTLINE	(3)			// �g�D�[�������_�����O���f���̗֊s���`��p�̐ݒ�
#define DX_DRAWSETTING_MODEL		(4)			// ���f���`��p�̐ݒ�
#define DX_DRAWSETTING_NUM			(5)			// �`��ݒ�^�C�v�̐�

// �V�F�[�_�[�萔�Z�b�g
#define DX_SHADERCONSTANTSET_LIB			0
#define DX_SHADERCONSTANTSET_LIB_SUB		1
#define DX_SHADERCONSTANTSET_MV1			2
#define DX_SHADERCONSTANTSET_USER			3
#define DX_SHADERCONSTANTSET_NUM			4

// �V�F�[�_�[�萔�^�C�v
#define DX_SHADERCONSTANTTYPE_VS_FLOAT		0
#define DX_SHADERCONSTANTTYPE_VS_INT		1
#define DX_SHADERCONSTANTTYPE_VS_BOOL		2
#define DX_SHADERCONSTANTTYPE_PS_FLOAT		3
#define DX_SHADERCONSTANTTYPE_PS_INT		4
#define DX_SHADERCONSTANTTYPE_PS_BOOL		5
#define DX_SHADERCONSTANTTYPE_NUM			6

// �}�X�N��^����
#ifdef DX_NON_MASK
#define MASK_FULLSCREEN_MASKUPDATE
#else
#define MASK_FULLSCREEN_MASKUPDATE					\
	if( MASKD.FullScreenMaskUpdateFlag )			\
	{												\
		RECT MaskRect ;								\
		MaskRect.left   = GSYS.DrawSetting.DrawArea.left ;		\
		MaskRect.right  = GSYS.DrawSetting.DrawArea.right ;	\
		MaskRect.top    = GSYS.DrawSetting.DrawArea.top ;		\
		MaskRect.bottom = GSYS.DrawSetting.DrawArea.bottom ;	\
		MASK_END( MaskRect )						\
		MASKD.FullScreenMaskUpdateFlag = FALSE ;	\
	}
#endif

// �`��͈͂ɓ����Ă��邩�ǂ����̔���( 0 ��������`��̈�ɓ����Ă��Ȃ����������� )
#define IN_DRAWAREA_CHECK( x1, y1, x2, y2 )	\
		( (DWORD)( ( (x2) - ( GSYS.DrawSetting.DrawArea.right  + 1 ) ) & ( GSYS.DrawSetting.DrawArea.left - ( (x1) + 1 ) ) &					\
		           ( (y2) - ( GSYS.DrawSetting.DrawArea.bottom + 1 ) ) & ( GSYS.DrawSetting.DrawArea.top  - ( (y1) + 1 ) ) ) & 0x80000000 )

// �`��͈͂��犮�S�ɊO��Ă��邩�ǂ����̔���( 0 ����Ȃ������犮�S�ɂ͂���Ă��� )
#define OUT_DRAWAREA_CHECK( x1, y1, x2, y2 ) \
		( (DWORD)( ( GSYS.DrawSetting.DrawArea.right  - ( (x1) + 1 ) ) & ( (x2) - ( GSYS.DrawSetting.DrawArea.left + 1 ) ) &		\
		           ( GSYS.DrawSetting.DrawArea.bottom - ( (y1) + 1 ) ) & ( (y2) - ( GSYS.DrawSetting.DrawArea.top  + 1 ) ) ) & 0x80000000 )

#define DRAWRECT_DRAWAREA_CLIP\
		if( IN_DRAWAREA_CHECK( DrawRect.left, DrawRect.top, DrawRect.right, DrawRect.bottom ) == 0 )\
		{\
			if( DrawRect.left   < GSYS.DrawSetting.DrawArea.left   ) DrawRect.left   = GSYS.DrawSetting.DrawArea.left   ;\
			if( DrawRect.right  < GSYS.DrawSetting.DrawArea.left   ) DrawRect.right  = GSYS.DrawSetting.DrawArea.left   ;\
			if( DrawRect.left   > GSYS.DrawSetting.DrawArea.right  ) DrawRect.left   = GSYS.DrawSetting.DrawArea.right  ;\
			if( DrawRect.right  > GSYS.DrawSetting.DrawArea.right  ) DrawRect.right  = GSYS.DrawSetting.DrawArea.right  ;\
			if( DrawRect.top    < GSYS.DrawSetting.DrawArea.top    ) DrawRect.top    = GSYS.DrawSetting.DrawArea.top    ;\
			if( DrawRect.bottom < GSYS.DrawSetting.DrawArea.top    ) DrawRect.bottom = GSYS.DrawSetting.DrawArea.top    ;\
			if( DrawRect.top    > GSYS.DrawSetting.DrawArea.bottom ) DrawRect.top    = GSYS.DrawSetting.DrawArea.bottom ;\
			if( DrawRect.bottom > GSYS.DrawSetting.DrawArea.bottom ) DrawRect.bottom = GSYS.DrawSetting.DrawArea.bottom ;\
		}

#ifdef DX_NON_MASK
#define MASK_BEGIN( RECT )
#else
#define MASK_BEGIN( RECT )		if( MASKD.MaskValidFlag ) Mask_DrawBeginFunction( (RECT) ) ;
#endif

#ifdef DX_NON_MASK
#define MASK_END( RECT )
#else
#define MASK_END( RECT )		if( MASKD.MaskValidFlag ) Mask_DrawAfterFunction( (RECT) ) ;
#endif

// �`���^����
#ifdef DX_NON_2DDRAW
	#ifdef DX_NON_MASK
		#define DRAW_DEF( FUNC3D, FUNC2D, SETRECT, RET, USE3DFLAG )\
			{\
				if( GSYS.DrawSetting.BlendMode == DX_BLENDMODE_SUB && GSYS.HardInfo.ValidSubBlend == FALSE )\
				{\
					RECT DrawRect ;\
					SETRECT\
					Graphics_DrawSetting_BlendModeSub_Pre( &DrawRect ) ;\
					RET = (FUNC3D) ;\
					Graphics_DrawSetting_BlendModeSub_Post( &DrawRect ) ;\
				}\
				else\
				{\
					RET = (FUNC3D) ;\
				}\
			}
	#else // DX_NON_MASK
		#define DRAW_DEF( FUNC3D, FUNC2D, SETRECT, RET, USE3DFLAG )\
			{\
				bool UseSubMode = GSYS.DrawSetting.BlendMode == DX_BLENDMODE_SUB && GSYS.HardInfo.ValidSubBlend == FALSE ;\
				if( UseSubMode || MASKD.MaskValidFlag )\
				{\
					RECT DrawRect ;\
					SETRECT\
					MASK_BEGIN( DrawRect ) ;\
					if( UseSubMode )\
					{\
						Graphics_DrawSetting_BlendModeSub_Pre( &DrawRect ) ;\
						RET = (FUNC3D) ;\
						Graphics_DrawSetting_BlendModeSub_Post( &DrawRect ) ;\
					}\
					else\
					{\
						RET = (FUNC3D) ;\
					}\
					\
					MASK_END( DrawRect ) ;\
				}\
				else\
				{\
					RET = (FUNC3D) ;\
				}\
			}
	#endif // DX_NON_MASK
#else // DX_NON_2DDRAW
	#ifdef DX_NON_MASK
		#define DRAW_DEF( FUNC3D, FUNC2D, SETRECT, RET, USE3DFLAG )\
			{\
				if( GSYS.DrawSetting.BlendMode == DX_BLENDMODE_SUB && GSYS.HardInfo.ValidSubBlend == FALSE && (USE3DFLAG) )\
				{\
					RECT DrawRect ;\
					SETRECT\
					Graphics_DrawSetting_BlendModeSub_Pre( &DrawRect ) ;\
					RET = (FUNC3D) ;\
					Graphics_DrawSetting_BlendModeSub_Post( &DrawRect ) ;\
				}\
				else\
				{\
					RET = (USE3DFLAG) ? (FUNC3D) : (FUNC2D) ;\
				}\
			}
	#else // DX_NON_MASK
		#define DRAW_DEF( FUNC3D, FUNC2D, SETRECT, RET, USE3DFLAG )\
			{\
				bool UseSubMode = GSYS.DrawSetting.BlendMode == DX_BLENDMODE_SUB && GSYS.HardInfo.ValidSubBlend == FALSE && (USE3DFLAG) ;\
				if( UseSubMode || MASKD.MaskValidFlag )\
				{\
					RECT DrawRect ;\
					SETRECT\
					MASK_BEGIN( DrawRect ) ;\
					if( UseSubMode )\
					{\
						Graphics_DrawSetting_BlendModeSub_Pre( &DrawRect ) ;\
						RET = (FUNC3D) ;\
						Graphics_DrawSetting_BlendModeSub_Post( &DrawRect ) ;\
					}\
					else\
					{\
						RET = (USE3DFLAG) ? (FUNC3D) : (FUNC2D) ;\
					}\
					\
					MASK_END( DrawRect ) ;\
				}\
				else\
				{\
					RET = (USE3DFLAG) ? (FUNC3D) : (FUNC2D) ;\
				}\
			}
	#endif // DX_NON_MASK
#endif // DX_NON_2DDRAW

// �`��ҋ@���Ă��钸�_�⃂�f����`�悷��
#ifndef DX_NON_MODEL
	#define MV1DRAWPACKDRAWMODEL		if( MV1Man.PackDrawModel != NULL ) MV1DrawPackDrawModel() ;
#else	// DX_NON_MODEL
	#define MV1DRAWPACKDRAWMODEL
#endif	// DX_NON_MODEL

#define DRAWSTOCKINFO_ASYNC( ASYNCFLAG )	\
	Graphics_Hardware_RenderVertex( ASYNCFLAG ) ;	\
	MV1DRAWPACKDRAWMODEL

#define DRAWSTOCKINFO			\
	Graphics_Hardware_RenderVertex() ;	\
	MV1DRAWPACKDRAWMODEL


// �\���̒�` --------------------------------------------------------------------

// ���C�u���������ŕ`��Ώۂ�ύX����ۂ̃J�����Ȃǂ̐ݒ��ۑ����Ă������߂̍\����
typedef struct tagSCREENDRAWSETTINGINFO
{
	int						Use3DFlag ;
	int						UseSysMemFlag ;
	int						DrawMode ;
	int						WaitVSync ;
	int						DrawScreen ;
	MATRIX_D				ViewMatrix ;
	MATRIX_D				ProjectionMatrix ;
	MATRIX_D				ViewportMatrix ;
	MATRIX_D				WorldMatrix ;
	int						ProjectionMatrixMode ;
	double					ProjFov ;
	double					ProjSize ;
	double					ProjNear ;
	double					ProjFar ;
	double					ProjDotAspect ;
	MATRIX_D				ProjMatrix ;
	RECT					DrawRect ;
} SCREENDRAWSETTINGINFO ;

// Graphics_Draw_DrawSimpleTriangleGraphF, Graphics_Draw_DrawSimpleQuadrangleGraphF �p�\����
typedef struct tagGRAPHICS_DRAW_DRAWSIMPLEANGLEGRAPHF_VERTEX
{
	float					x, y, u, v ;
} GRAPHICS_DRAW_DRAWSIMPLEANGLEGRAPHF_VERTEX ;
typedef struct tagGRAPHICS_DRAW_DRAWSIMPLETRIANGLEGRAPHF_PARAM
{
	int						TriangleNum ;
	GRAPHICS_DRAW_DRAWSIMPLEANGLEGRAPHF_VERTEX *Vertex ;
	int						GraphHandle ;
	int						TransFlag ;
} GRAPHICS_DRAW_DRAWSIMPLETRIANGLEGRAPHF_PARAM ;
typedef struct tagGRAPHICS_DRAW_DRAWSIMPLEQUADRANGLEGRAPHF_PARAM
{
	int						QuadrangleNum ;
	GRAPHICS_DRAW_DRAWSIMPLEANGLEGRAPHF_VERTEX *Vertex ;
	int						GraphHandle ;
	int						TransFlag ;
} GRAPHICS_DRAW_DRAWSIMPLEQUADRANGLEGRAPHF_PARAM ;

// �e�N�X�`�����W8���_�\����( �o�C�L���[�r�b�N��ԕ`��p )
typedef struct tagVERTEX_TEX8_2D
{
	VECTOR					pos ;
	float					rhw ;
	unsigned int			color ;
	float					u0, v0 ;
	float					u1, v1 ;
	float					u2, v2 ;
	float					u3, v3 ;
	float					u4, v4 ;
	float					u5, v5 ;
	float					u6, v6 ;
	float					u7, v7 ;
} VERTEX_TEX8_2D, *LPVERTEX_TEX8_2D ; 

// �Q�c�`��p���_�\����(�e�N�X�`������)
typedef struct tagVERTEX_NOTEX_2D
{
	VECTOR					pos ;
	float					rhw ;
	unsigned int			color ;
} VERTEX_NOTEX_2D, *LPVERTEX_NOTEX_2D ; 

// ��ɂQ�c�`��Ɏg�p���钸�_�f�[�^�^
typedef struct tagVERTEX_2D
{
	VECTOR					pos ;
	float					rhw ;
	unsigned int			color ;
	float					u, v ;
} VERTEX_2D, *LPVERTEX_2D ; 

// �u�����h�摜�t���Q�c�摜�`��p���_�\����
struct VERTEX_BLENDTEX_2D
{
	VECTOR					pos ;
	float					rhw ;
	unsigned int			color ;
	unsigned int			specular ;
	float					u1, v1 ;
	float					u2, v2 ;
} ;

// �R�c�`��Ɏg�p���钸�_�f�[�^�^( �e�N�X�`���Ȃ� )( ���o�[�W�����̂��� )
typedef struct tagVERTEX_NOTEX_3D
{
	VECTOR					pos ;
	unsigned char			b, g, r, a ;
} VERTEX_NOTEX_3D, *LPVERTEX_NOTEX_3D ;

// �\�t�g�E�G�A�����_�����O�p�I���W�i���摜���
struct IMAGEDATA_ORIG_SOFT
{
	MEMIMG					MemImg ;						// MEMIMG
	MEMIMG					*ZBuffer ;						// ZBuffer
} ;

// �n�[�h�E�G�A�����_�����O�p�I���W�i���摜�e�N�X�`�����
struct IMAGEDATA_ORIG_HARD_TEX
{
	struct IMAGEDATA_ORIG_HARD_TEX_PF	*PF ;				// ���ˑ��f�[�^
	int						OrigPosX, OrigPosY ;			// ���摜�Ŏg�p���Ă���̈�̍�����W
	int						UseWidth, UseHeight ;			// ���摜�Ŏg�p���Ă���̈�̃T�C�Y
	int						TexWidth, TexHeight ;			// �e�N�X�`�����̂̕��ƍ���
} ;

// �n�[�h�E�G�A�����_�����O�p�I���W�i���摜���
struct IMAGEDATA_ORIG_HARD
{
	int						MipMapCount ;					// �~�b�v�}�b�v�̐�
	int						TexNum ;						// �e�N�X�`���̐�
	IMAGEDATA_ORIG_HARD_TEX Tex[ 4 ] ;						// �e�N�X�`�����X�g�ւ̃|�C���^
} ;

// �I���W�i���摜���\����
struct IMAGEDATA_ORIG
{
	int						RefCount ;						// �Q�Ƃ���Ă��鐔
	int						ColorFormat ;					// �J���[�t�H�[�}�b�g( DX_GRAPHICSIMAGE_FORMAT_3D_RGB16 �� )
	IMAGEFORMATDESC			FormatDesc ;					// �t�H�[�}�b�g
	unsigned int			TransCode ;						// ���ߐF�R�[�h
	int						Width, Height ;					// �摜�̃T�C�Y
	int						ZBufferFlag ;					// �y�o�b�t�@�����邩�ǂ���( TRUE:����  FALSE:�Ȃ� )
	int						ZBufferBitDepthIndex ;			// �y�o�b�t�@�r�b�g�[�x�C���f�b�N�X

	union
	{
		IMAGEDATA_ORIG_SOFT	Soft ;						// �\�t�g�E�G�A�����_�����O�p���
		IMAGEDATA_ORIG_HARD	Hard ;						// �n�[�h�E�G�A�����_�����O�p���
	} ;
} ;

// �\�t�g�E�G�A�����_�����O�ŃC���[�W�f�[�^�\����
struct IMAGEDATA_SOFT
{
	MEMIMG					MemImg ;						// �`��p MEMIMG
} ;

// �n�[�h�E�G�A�����_�����O�ō��W���\����
struct IMAGEDATA_HARD_VERT
{
	float					x, y ;							// �ʒu
	float					u, v ;							// �t�u�ʒu
} ;

// �n�[�h�E�G�A�����_�����O�ŕ`��p�\����
struct IMAGEDATA_HARD_DRAW
{
	int						DrawPosXI, DrawPosYI ;			// �`�掞�Ɏw�肳�����W����̑��΍��W
	float					DrawPosXF, DrawPosYF ;			// �`�掞�Ɏw�肳�����W����̑��΍��W
	int						UsePosXI, UsePosYI ;			// �e�N�X�`�����Ŏg�p���Ă����`�̍�����W
	float					UsePosXF, UsePosYF ;			// �e�N�X�`�����Ŏg�p���Ă����`�̍�����W
	int						WidthI, HeightI ;				// �e�N�X�`�����Ŏg�p���Ă���T�C�Y
	float					WidthF, HeightF ;				// �e�N�X�`�����Ŏg�p���Ă���T�C�Y

	IMAGEDATA_HARD_VERT		Vertex[ 4 ] ;					// �e�N�X�`���̕`����W���
	unsigned char			VertType[ 4 ] ;					// ���_�^�C�v( TRUE=�O�p�̔�������@FALSE=�� )

	IMAGEDATA_ORIG_HARD_TEX *Tex ;							// �g�p����e�N�X�`���ւ̃|�C���^
} ;

// �n�[�h�E�G�A�����_�����O�ŃC���[�W�f�[�^�\����
struct IMAGEDATA_HARD
{
	int						DrawNum ;						// �`��p���̐�
	IMAGEDATA_HARD_DRAW		Draw[ 4 ] ;						// �`��p���
} ;

// �C���[�W���f�[�^�\����
struct IMAGEDATA_READBASE
{
	int						Type ;							// �^�C�v( 0:�t�@�C�� 1:�������C���[�W 2:BaseImage )

	wchar_t					*FileName ;						// �t�@�C����
	BASEIMAGE				*BaseImage ;					// ��{�C���[�W�f�[�^
	BASEIMAGE				*AlphaBaseImage ;				// ���`�����l���p��{�C���[�W�f�[�^
	void					*MemImage ;						// �������C���[�W
	int						MemImageSize ;					// �������C���[�W�T�C�Y
	void					*AlphaMemImage ;				// ���`�����l���p�������C���[�W
	int						AlphaMemImageSize ;				// ���`�����l���p�������C���[�W�T�C�Y
	int						ReverseFlag ;					// ���]�ǂݍ��݂������ǂ���( TRUE:����  FALSE:���ĂȂ� )�A�ēǂݍ��݂̍ۂɓǂݍ��݌��𔽓]���邩�ǂ����Ŏg�p�����ABASEIMAGE �Ɋւ��Ă͓ǂݍ��݌��ۑ����ɔ��]���邽�߂��̃t���O�͖��������
	int						ConvertPremultipliedAlpha ;		// ��Z�ς݃��`�����l���t���摜�ɕϊ��������ǂ���( TRUE:����  FALSE:���ĂȂ� )�A�ēǂݍ��݂̍ۂɏ�Z�ς݃��ϊ����s�����ǂ����Ŏg�p�����ABASEIMAGE �Ɋւ��Ă͓ǂݍ��݌��ۑ����ɏ�Z�ς݃��ϊ�����邽�߂��̃t���O�͖��������

	int						RefCount ;						// �Q�Ɛ�
} ;

// �C���[�W�f�[�^�\����
struct IMAGEDATA
{
	HANDLEINFO				HandleInfo ;					// �n���h�����ʃf�[�^

	int						*LostFlag ;						// ������ɗ��Ă�t���O�̃|�C���^

#ifndef DX_NON_MOVIE
	int						MovieHandle ;					// ����n���h��
#endif

	IMAGEDATA_READBASE		*ReadBase ;						// ���f�[�^���
	int						UseBaseXI, UseBaseYI ;			// ���f�[�^���Ŏg�p���Ă���͈͂̍�����W
	float					UseBaseXF, UseBaseYF ;			// ���f�[�^���Ŏg�p���Ă���͈͂̍�����W( float�^ )

	IMAGEDATA_ORIG			*Orig ;							// �I���W�i���摜���\���̂ւ̃|�C���^( �I���W�i���摜�ł͂Ȃ��ꍇ�� NULL )

	int						UseOrigXI, UseOrigYI ;			// �I���W�i���摜���̎g�p���Ă����`�̍�����W
	float					UseOrigXF, UseOrigYF ;			// �I���W�i���摜���̎g�p���Ă����`�̍�����W( float�^ )
	int						WidthI, HeightI ;				// �I���W�i���摜���̎g�p���Ă����`�̕��ƍ���
	float					WidthF, HeightF ;				// �I���W�i���摜���̎g�p���Ă����`�̕��ƍ���( float�^ )

	DWORD					*FullColorImage ;				// �t���J���[�C���[�W

	int						LockFlag ;						// ���b�N���Ă��邩�ǂ����t���O
	BYTE					*LockImage ;					// ���b�N���ɍ쐬����e���|�����C���[�W
	DWORD					LockImagePitch ;				// ���b�N�C���[�W�̃s�b�`

	int						NotInitGraphDelete ;			// InitGraph �ō폜���Ȃ����ǂ����̃t���O( TRUE:InitGraph�ł͍폜���Ȃ�  FALSE:InitGraph�ō폜���� )

	int						DeviceLostDeleteFlag ;			// �f�o�C�X���X�g���ɍ폜���邩�ǂ����̃t���O( TRUE:�f�o�C�X���X�g���ɍ폜����  FALSE:�f�o�C�X���X�g���ɍ폜���Ȃ� )

	union
	{
		IMAGEDATA_SOFT		Soft ;							// �\�t�g�E�G�A�����_�����O�p�\���̂ւ̃|�C���^
		IMAGEDATA_HARD		Hard ;							// �n�[�h�E�G�A�����_�����O�p�\���̂ւ̃|�C���^
	} ;
} ;

// �V�F�[�_�[�n���h���Ŏg�p������̍\����
struct SHADERHANDLEDATA
{
	HANDLEINFO				HandleInfo ;						// �n���h�����ʃf�[�^

	int						IsVertexShader ;					// ���_�V�F�[�_�[���ǂ���( TRUE:���_�V�F�[�_�[  FALSE:�s�N�Z���V�F�[�_�[ )
	void					*FunctionCode ;						// �V�F�[�_�[�R�[�h���i�[�����������̈�ւ̃|�C���^
	int						FunctionCodeSize ;					// �V�F�[�_�[�R�[�h�̃T�C�Y

	struct SHADERHANDLEDATA_PF 	*PF ;							// ���ˑ��f�[�^
} ;

// �V�F�[�_�[�p�萔�o�b�t�@�n���h���Ŏg�p������̍\����
struct SHADERCONSTANTBUFFERHANDLEDATA
{
	HANDLEINFO				HandleInfo ;						// �n���h�����ʃf�[�^

	struct SHADERCONSTANTBUFFERHANDLEDATA_PF 	*PF ;					// ���ˑ��f�[�^
} ;

// ���_�o�b�t�@�n���h���Ŏg�p������̍\����
struct VERTEXBUFFERHANDLEDATA
{
	HANDLEINFO				HandleInfo ;						// �n���h�����ʃf�[�^

	int						Type ;								// ���_�f�[�^�̃^�C�v( DX_VERTEX_TYPE_NORMAL_3D �� )
	int						UnitSize ;							// ���_�f�[�^��ӂ�̃o�C�g��
	int						Num ;								// ���_�̐�
	void					*Buffer ;							// �V�X�e���������Ɋm�ۂ��ꂽ�o�b�t�@

	struct VERTEXBUFFERHANDLEDATA_PF *PF ;						// ���ˑ��f�[�^
} ;

// �C���f�b�N�X�o�b�t�@�n���h���Ŏg�p������̍\����
struct INDEXBUFFERHANDLEDATA
{
	HANDLEINFO				HandleInfo ;						// �n���h�����ʃf�[�^

	int						Type ;								// �C���f�b�N�X�f�[�^�̃^�C�v( DX_INDEX_TYPE_NORMAL_3D �� )
	int						UnitSize ;							// �C���f�b�N�X�f�[�^��ӂ�̃o�C�g��
	int						Num ;								// �C���f�b�N�X�̐�
	void					*Buffer ;							// �V�X�e���������Ɋm�ۂ��ꂽ�o�b�t�@

	struct INDEXBUFFERHANDLEDATA_PF *PF ;						// ���ˑ��f�[�^
} ;

// �V���h�E�}�b�v�f�[�^�\����
struct SHADOWMAPDATA
{
	HANDLEINFO				HandleInfo ;					// �n���h�����ʃf�[�^

	int						*LostFlag ;						// ������ɗ��Ă�t���O�̃|�C���^

	int						TexFormat_Float ;				// ���������_�^�t�H�[�}�b�g���ǂ���( TRUE:���������_�^  FALSE:�Œ菬���_�^ )
	int						TexFormat_BitDepth ;			// �r�b�g�[�x
	int						ColorFormat ;					// �J���[�t�H�[�}�b�g
	int						ZBufferFormat ;					// �y�o�b�t�@�t�H�[�}�b�g

	int						EnableDrawArea ;				// DrawAreaMinPosition �� DrawAreaMaxPosition ���L�����ǂ���
	VECTOR					DrawAreaMinPosition ;			// �V���h�E�}�b�v�̕`��̍ۂɓ����͈͂̏��������W
	VECTOR					DrawAreaMaxPosition ;			// �V���h�E�}�b�v�̕`��̍ۂɓ����͈͂̑傫�����W
	VECTOR					DrawAreaViewClipPos[ 8 ] ;		// �V���h�E�}�b�v�ɕ`�悷��͈͂��͂ލ��W

	VECTOR					DefaultViewClipPos[ 8 ] ;		// EnableDrawArea �� FALSE �̍ۂɎg�p����V���h�E�}�b�v�ɕ`�悷��͈͂��͂ލ��W

	int						BaseSizeX ;						// �V���h�E�}�b�v�̃T�C�Y
	int						BaseSizeY ;

	int						DrawSetupFlag ;					// �`�揀�������Ă��邩�ǂ����̃t���O

	MATRIX					ShadowMapViewMatrix ;			// �V���h�E�}�b�v��`�悵���ۂ̃r���[�s��
	MATRIX					ShadowMapProjectionMatrix ;		// �V���h�E�}�b�v��`�悵���ۂ̎ˉe�s��
	MATRIX					ShadowMapViewProjectionMatrix ; // �V���h�E�}�b�v��`�悵���ۂ̃r���[�s��Ǝˉe�s�����Z��������

	float					AdjustDepth ;					// �V���h�E�}�b�v���g�p�����`�掞�̐[�x�␳�l
	int						BlurParam ;						// �V���h�E�}�b�v�ɓK�p����ڂ����t�B���^�[�̒l
	float					GradationParam ;				// �V���h�E�}�b�v�l�̃O���f�[�V�����͈̔�

	int						RenderTargetScreen[ DX_RENDERTARGET_COUNT ] ;			// ���X�̃����_�����O�^�[�Q�b�g
	int						RenderTargetScreenSurface[ DX_RENDERTARGET_COUNT ] ;	// ���X�̃����_�����O�^�[�Q�b�g�T�[�t�F�X

	int						RenderMaskUseFlag ;				// �}�X�N��ʂ��g�p���邩�ǂ����̃t���O

	VECTOR					LightDirection ;				// �V���h�E�}�b�v���z�肷�郉�C�g�̕���
	MATRIX					LightMatrix ;					// �V���h�E�}�b�v���z�肷�郉�C�g�̍s��

	VECTOR_D				RenderCameraPosition ;			// �����_�����O���s���ۂ̃J�����̈ʒu
	VECTOR_D				RenderCameraTarget ;			// �����_�����O���s���ۂ̃J�����̒����_
	VECTOR_D				RenderCameraUp ;				// �����_�����O���s���ۂ̃J�����̃A�b�v�x�N�g��
	double					RenderCameraHRotate ;			// �����_�����O���s���ۂ̃J�����̐����p�x
	double					RenderCameraVRotate ;			// �����_�����O���s���ۂ̃J�����̐����p�x
	double					RenderCameraTRotate ;			// �����_�����O���s���ۂ̃J�����̔P��p�x
	MATRIX_D				RenderCameraMatrix ;			// �����_�����O���s���ۂ̃r���[�s��
	double					RenderCameraScreenCenterX ;		// �����_�����O���s���ۂ̃J�����̏����_
	double					RenderCameraScreenCenterY ;

	int						RenderProjectionMatrixMode ;	// �����_�����O���s���ۂ̎ˉe�s��쐬���[�h
	double					RenderProjNear ;				// �����_�����O���s���ۂ̃J������ Near�N���b�v��
	double					RenderProjFar ;					// �����_�����O���s���ۂ̃J������ Far�N���b�v��
	double					RenderProjDotAspect ;			// �����_�����O���s���ۂ̃h�b�g�A�X�y�N�g��( �c / �� )
	double					RenderProjFov ;					// �����_�����O���s���ۂ̉��ߖ@���̎���p
	double					RenderProjSize ;				// �����_�����O���s���ۂ̐��ˉe���̃T�C�Y
	MATRIX_D				RenderProjMatrix ;				// �����_�����O���s���ۂ̎ˉe�s��

	struct SHADOWMAPDATA_PF	*PF ;							// ���ˑ��f�[�^
} ;


// ���������_�^�� RECT �\����
struct RECTF
{
	float left, top ;
	float right, bottom ;
} ;

// �O���t�B�b�N�n���h���̃Z�b�g�A�b�v�ɕK�v�ȃO���[�o���f�[�^��Z�߂��\����
struct SETUP_GRAPHHANDLE_GPARAM
{
	DWORD					TransColor ;							// ���ߐF

	int						CreateImageColorBitDepth ;				// �쐬����摜�̐F�[�x
	int						FloatTypeGraphCreateFlag ;				// Float�^�摜�쐬�w��t���O( �e�N�X�`���T�[�t�F�X�̂� )
	int						CreateImageChannelNum ;					// �쐬����摜�̃`�����l����( �e�N�X�`���T�[�t�F�X�̂� )
	int						CreateImageChannelBitDepth ;			// �쐬����摜�̂P�`�����l���ӂ�̃r�b�g�[�x( �����炪�ݒ肳��Ă���ꍇ�� CreateImageColorBitDepth ���D�悳��� )
	int						AlphaTestImageCreateFlag ;				// ���e�X�g�t���摜�쐬�w��t���O( AlphaGraphCreateFlag �̕����D��x������ )( �e�N�X�`���T�[�t�F�X�̂� )
	int						AlphaChannelImageCreateFlag ;			// ���`�����l���t���摜�쐬�w��t���O( DrawValidGraphCreateFlag �̕����D��x������ )
	int						CubeMapTextureCreateFlag ;				// �L���[�u�}�b�v�e�N�X�`���쐬�w��t���O( 1:�L���[�u�}�b�v�e�N�X�`�����쐬����  0:�ʏ�e�N�X�`�����쐬���� )
	int						BlendImageCreateFlag ;					// �u�����h�����p�摜�쐬�w��t���O
	int						UseManagedTextureFlag ;					// �}�l�[�W�h�e�N�X�`�����g�p���邩�A�t���O( 1:�g�p����  0:�g�p���Ȃ� )
	int						UseLinearMapTextureFlag ;				// �e�N�X�`���̃������f�[�^�z�u�Ƀ��j�A���I���ł���ꍇ�̓f�[�^�z�u���������j�A�ɂ��邩�ǂ���( TRUE:���j�A���\�ȏꍇ�̓��j�A�ɂ���  FALSE:���j�A���\�ȏꍇ�����Ƀ��j�A���w�肵�Ȃ� )
	int						PlatformTextureFormat ;					// ���ˑ��̃e�N�X�`���t�H�[�}�b�g�𒼐ڎw�肷�邽�߂Ɏg�p���邽�߂̕ϐ�( DX_TEXTUREFORMAT_DIRECT3D9_R8G8B8 �Ȃ� )

	int						DrawValidImageCreateFlag ;				// �`��\�摜�쐬�w��t���O( �e�N�X�`���T�[�t�F�X�̂� )
	int						DrawValidAlphaImageCreateFlag ;			// �`��\�ȃ��`�����l���t���摜�쐬�w��t���O( �e�N�X�`���T�[�t�F�X�̂� )
	int						DrawValidFloatTypeGraphCreateFlag ;		// �`��\��Float�^�摜�쐬�w��t���O( �e�N�X�`���T�[�t�F�X�̂� )
	int						DrawValidGraphCreateZBufferFlag ;		// �`��\�摜���쐬����ۂɐ�p�̂y�o�b�t�@���쐬���邩�ǂ���
	int						CreateDrawValidGraphChannelNum ;		// �`��\�摜�̃`�����l����( �e�N�X�`���T�[�t�F�X�̂� )
	int						CreateDrawValidGraphZBufferBitDepth ;	// �`��\�摜�̂y�o�b�t�@�̃r�b�g�[�x( �e�N�X�`���T�[�t�F�X�̂� )
	int						DrawValidMSSamples ;					// �`��\�ȉ摜�̃}���`�T���v�����O��
	int						DrawValidMSQuality ;					// �`��\�ȉ摜�̃}���`�T���v�����O�N�I���e�B

	int						MipMapCount ;							// �����ō쐬����~�b�v�}�b�v�̐�( -1:�ő僌�x���܂ō쐬���� )
	int						UserMaxTextureSize ;					// ���[�U�[�w��̃e�N�X�`���ő�T�C�Y
	int						NotUseDivFlag ;							// �摜�������s��Ȃ����ǂ���( TRUE:�s��Ȃ�  FALSE:�s�� )
	int						NotUseAlphaImageLoadFlag ;				// _a ���t�����A���t�@�`�����l���p�̉摜�t�@�C����ǉ��œǂݍ��ޏ������s��Ȃ����ǂ���( TRUE:�s��Ȃ�  FALSE:�s�� )
	int						NotInitGraphDelete ;					// InitGraph �ō폜���Ȃ����ǂ����̃t���O( TRUE:InitGraph�ł͍폜���Ȃ�  FALSE:InitGraph�ō폜���� )
} ;

// �V���h�E�}�b�v�n���h���̃Z�b�g�A�b�v�ɕK�v�ȃO���[�o���f�[�^��Z�߂��\����
struct SETUP_SHADOWMAPHANDLE_GPARAM
{
	int						Dummy ;
} ;

// �摜�f�[�^����O���t�B�b�N�n���h���̍쐬�E�摜�f�[�^�̓]���ɕK�v�ȃO���[�o���f�[�^��Z�߂�����
struct CREATE_GRAPHHANDLE_AND_BLTGRAPHIMAGE_GPARAM
{
	SETUP_GRAPHHANDLE_GPARAM InitHandleGParam ;						// �O���t�B�b�N�n���h���̃Z�b�g�A�b�v�ɕK�v�ȃO���[�o���f�[�^

	int						NotUseTransColor;						// ���ߐF���g�p���Ȃ����ǂ����t���O(TRUE:�g�p���Ȃ�  FALSE:�g�p����)
	int						UseTransColorGraphCreateFlag ;			// ���ߐF�Ƃ����łȂ������̋��E���������炩�ɂ��邩�A�t���O
	int						LeftUpColorIsTransColorFlag ;			// �摜����̃s�N�Z���F�𓧉ߐF�ɂ��邩�ǂ����̃t���O
} ;

// �摜�̌��f�[�^�̏��ɕK�v�ȃO���[�o���f�[�^��Z�߂�����
struct SETGRAPHBASEINFO_GPARAM
{
	int						NotGraphBaseDataBackupFlag ;			// �O���t�B�b�N�n���h�����쐬�����ۂɎg�p�����摜�f�[�^���o�b�N�A�b�v���Ȃ����ǂ����̃t���O( TRUE:�o�b�N�A�b�v���Ȃ�  FALSE:�o�b�N�A�b�v���� )
} ;

// �t�@�C������O���t�B�b�N�n���h�����쐬���鏈���ɕK�v�ȃO���[�o���f�[�^��Z�߂�����
struct LOADGRAPH_GPARAM
{
	LOADBASEIMAGE_GPARAM	LoadBaseImageGParam ;					// �摜�f�[�^�̓ǂݍ��݂ɕK�v�ȃO���[�o���f�[�^
#ifndef DX_NON_MOVIE
	OPENMOVIE_GPARAM		OpenMovieGParam ;						// ���[�r�[�t�@�C���̃I�[�v���ɕK�v�ȃO���[�o���f�[�^
#endif
	CREATE_GRAPHHANDLE_AND_BLTGRAPHIMAGE_GPARAM CreateGraphGParam ;	// �O���t�B�b�N�n���h���̍쐬�E�������ɕK�v�ȃO���[�o���f�[�^
	SETGRAPHBASEINFO_GPARAM	SetGraphBaseInfoGParam ;				// �摜�̌��f�[�^�̏��̕ۑ��ɕK�v�ȃO���[�o���f�[�^
} ;

// �O���t�B�b�N�f�[�^�ǂݍ��݈�����Z�߂�����
struct LOADGRAPH_PARAM
{
	LOADGRAPH_GPARAM		GParam ;
	int						ReCreateFlag ;
	int						GrHandle ;
	int						BaseHandle ;

	const wchar_t *			FileName ;
	const void *			RgbMemImage ;
	int						RgbMemImageSize ;
	const void *			AlphaMemImage ;
	int						AlphaMemImageSize ;
	const BITMAPINFO *		RgbBmpInfo ;
	void *					RgbBmpImage ;
	const BITMAPINFO *		AlphaBmpInfo ;
	void *					AlphaBmpImage ;
	BASEIMAGE *				RgbBaseImage ;
	BASEIMAGE *				AlphaBaseImage ;

	int						AllNum ;
	int						XNum ;
	int						YNum ;
	int						IsFloat ;
	int						SizeXI ;
	float					SizeXF ;
	int						SizeYI ;
	float					SizeYF ;
	int *					HandleBuf ;
	int						TextureFlag ;
	int						ReverseFlag ;
	int						SurfaceMode ;
} ;

// ���C�g�n���h���\����
struct LIGHT_HANDLE
{
	unsigned int			ID ;												// �G���[�`�F�b�N�p�̂h�c
	int						Handle ;											// ���g�̃n���h���l

	LIGHTPARAM				Param ;												// ���C�g�̃p�����[�^
	int						SetHardwareIndex ;									// �n�[�h�E�F�A�A�N�Z�����[�^��̃Z�b�g����Ă���C���f�b�N�X( -1:�Z�b�g����Ă��Ȃ� )
	int						EnableFlag ;										// �L���t���O
	int						HardwareChangeFlag ;								// �n�[�h�E�F�A�A�N�Z�����[�^�ɔ��f���Ă��Ȃ��ݒ肪���邩�ǂ����̃t���O( TRUE:����  FALSE:�Ȃ� )
	int						ShadowMapSlotDisableFlag[ MAX_USE_SHADOWMAP_NUM ] ;	// �V���h�E�}�b�v���g�p���Ȃ����ǂ����̃t���O( TRUE:�g�p���Ȃ�  FALSE:�g�p���� )
} ;

// ���C�g�֌W���̍\����
struct GRAPHICSSYS_LIGHTATA
{
	int						ProcessDisable ;						// ���C�g�����𖳌��ɂ��邩�ǂ���
	int						ChangeMaterial ;						// ���C�g�v�Z�p�}�e���A�����ύX�������ǂ����̃t���O
	MATERIALPARAM			Material ;								// ���C�g�v�Z�p�}�e���A��
	int						MaterialNotUseVertexDiffuseColor ;		// ���C�g�v�Z�ɒ��_�̃f�B�t���[�Y�J���[���g�p���Ȃ����ǂ���
	int						MaterialNotUseVertexSpecularColor;		// ���C�g�v�Z�ɒ��_�̃X�y�L�����J���[���g�p���Ȃ����ǂ���
	LIGHT_HANDLE			*Data[ MAX_LIGHT_NUM ] ;				// ���C�g���ւ̃|�C���^
	int						Num ;									// ���C�g�̐�
	int						Area ;									// �L���ȃ��C�g������͈�
	int						HandleID ;								// ���C�g�Ɋ��蓖�Ă�h�c
	int						EnableNum ;								// �L���ɂȂ��Ă��郉�C�g�̐�
	int						MaxHardwareEnableIndex ;				// �n�[�h�E�F�A�A�N�Z�����[�^�ɑ΂��ėL���ɂȂ��Ă��郉�C�g�ň�ԑ傫�Ȓl�̃C���f�b�N�X
	int						EnableHardwareIndex[ MAX_HARDWAREENABLELIGHTINDEX_NUM ] ;	// �n�[�h�E�F�A�A�N�Z�����[�^�ɑ΂��ėL���ɂ��Ă��郉�C�g�̃��X�g
	int						HardwareChange ;						// �n�[�h�E�F�A�A�N�Z�����[�^���ւ̔��f���K�v�ȕύX�����������ǂ����̃t���O
	int						HardwareRefresh ;						// �n�[�h�E�F�A�A�N�Z�����[�^���ւ̑S���ڂ̔��f���K�v���ǂ����̃t���O
	int						DefaultHandle ;							// �f�t�H���g���C�g�p�n���h��
} ;

// �J�����֌W���̍\����
struct GRAPHICSSYS_CAMERA
{
	VECTOR_D				Position ;								// �J�����̈ʒu
	VECTOR_D				Target ;								// �J�����̒����_
	VECTOR_D				Up ;									// �J�����̃A�b�v�x�N�g��
	double					HRotate ;								// �J�����̐����p�x
	double					VRotate ;								// �J�����̐����p�x
	double					TRotate ;								// �J�����̔P��p�x
	MATRIX_D				Matrix ;								// �r���[�s��
	int						SetupType ;								// �Z�b�g�A�b�v�^�C�v( CAMERA_SETUPTYPE_POS_TARG_UP �Ȃ� )
	double					ScreenCenterX ;							// �J�����̏����_
	double					ScreenCenterY ;
} ;

// ���[�U�[�̕`��ݒ���\����
struct GRAPHICSSYS_USERRENDERINFO
{
	int						SetTextureGraphHandle[ 20 ] ;			// ���[�U�[�ݒ�̊e�X�e�[�W�̃e�N�X�`��
	int						SetRenderTargetGraphHandle[ 4 ] ;		// ���[�U�[�ݒ�̊e�����_�����O�^�[�Q�b�g

	int						SetVertexShaderHandle ;					// ���[�U�[�ݒ�Ŏg�p���钸�_�V�F�[�_�[
	int						SetPixelShaderHandle ;					// ���[�U�[�ݒ�Ŏg�p����s�N�Z���V�F�[�_�[
} ;

// �`��ݒ�֌W���̍\����
struct GRAPHICSSYS_DRAWSETTINGDATA
{
	int						EnableZBufferFlag2D ;					// �y�o�b�t�@�̗L���t���O
	int						WriteZBufferFlag2D ;					// �y�o�b�t�@�̍X�V���s�����A�t���O
	int						ZBufferCmpType2D ;						// �y�l�̔�r���[�h
	int						ZBias2D ;								// �y�o�C�A�X

	int						EnableZBufferFlag3D ;					// �y�o�b�t�@�̗L���t���O
	int						WriteZBufferFlag3D ;					// �y�o�b�t�@�̍X�V���s�����A�t���O
	int						ZBufferCmpType3D ;						// �y�l�̔�r���[�h
	int						ZBias3D ;								// �y�o�C�A�X

	int						NotDrawFlagInSetDrawArea ;				// �`��s�\�t���O�iSetDrawArea�p�j
	int						UseNoBlendModeParam ;					// DX_BLENDMODE_NOBLEND ���ł� Param �̒l���g�p���邩�ǂ����̃t���O( TRUE:�g�p����  FALSE:�g�p���Ȃ� )

//	int						NotUseBasicGraphDraw3DDeviceMethodFlag ;// �P���}�`�̕`��ɂR�c�f�o�C�X�̋@�\���g�p���Ȃ����ǂ����̃t���O

	int						FillMode ;								// �t�B�����[�h( DX_FILL_SOLID �Ȃ� )
	int						CullMode ;								// �J�����O���[�h( DX_CULLING_LEFT �Ȃ� )

	int						TexAddressModeU[ USE_TEXTURESTAGE_NUM ] ;	// �e�N�X�`���A�h���X���[�h�t
	int						TexAddressModeV[ USE_TEXTURESTAGE_NUM ] ;	// �e�N�X�`���A�h���X���[�h�u
	int						TexAddressModeW[ USE_TEXTURESTAGE_NUM ] ;	// �e�N�X�`���A�h���X���[�h�v

	int						FogEnable ;								// �t�H�O���L�����ǂ���( TRUE:�L��  FALSE:���� )
	int						FogMode ;								// �t�H�O���[�h
	DWORD					FogColor ;								// �t�H�O�J���[
	float					FogStart, FogEnd ;						// �t�H�O�J�n�A�h���X�ƏI���A�h���X
	float					FogDensity ;							// �t�H�O���x

	float					DrawZ;									// �Q�c�`�掞�ɂy�o�b�t�@�ɏ������ނy�l
	int						DrawMode ;								// �`�惂�[�h

	int						MaxAnisotropy ;							// �ő�ٕ���
	int						Large3DPositionSupport ;				// �R�c�`��Ŏg�p������W�l�� 10000000.0f �Ȃǂ̑傫�Ȓl�ɂȂ��Ă��`��̕�����������}���鏈�����g�p���邩�ǂ�����ݒ肷��( TRUE:�`��̕����}���鏈�����g�p����( CPU���ׂ��オ��܂� )�@�@FALSE:�`��̕����}���鏈���͎g�p���Ȃ�( �f�t�H���g ) )

	int						AlphaChDrawMode ;						// �`���ɐ��������l���������ނ��ǂ���( TRUE:�������l����������  FALSE:�ʏ탂�[�h )

	int						BlendMode ;								// �u�����h���[�h
	int						BlendParam ;							// �u�����h�p�����[�^

	int						BlendGraph ;							// �u�����h�O���t�B�b�N�n���h��
//	int						BlendGraphType ;						// �u�����h�O���t�B�b�N�^�C�v
//	int						BlendGraphFadeRatio ;					// �u�����h�O���t�B�b�N�̃t�F�[�h�p�����[�^
	int						BlendGraphBorderParam ;					// �u�����h�O���t�B�b�N�n���h���̃u�����h���E�l(0�`255)
	int						BlendGraphBorderRange ;					// �u�����h�O���t�B�b�N�n���h���̋��E�����̕�(0�`255)
	int						BlendGraphX, BlendGraphY ;				// �u�����h�O���t�B�b�N�̋N�_���W
	int						BlendGraphPosMode ;						// �u�����h�O���t�B�b�N�̍��W���[�h( DX_BLENDGRAPH_POSMODE_DRAWGRAPH �Ȃ� )

	int						AlphaTestMode ;							// �A���t�@�e�X�g���[�h
	int						AlphaTestParam ;						// �A���t�@�e�X�g�p�����[�^

	int						NotUseSpecular ;						// �X�y�L�������g�p���Ȃ����ǂ���
	int						ShadowMap[ MAX_USE_SHADOWMAP_NUM ] ;	// �g�p����V���h�E�}�b�v�O���t�B�b�N�n���h��
	int						UseShadowMapNum ;						// �L���ȃV���h�E�}�b�v�̐ݒ萔

	int						ShadowMapDrawSetupRequest ;				// �V���h�E�}�b�v�ɑ΂���`�揀�����N�G�X�g
	int						ShadowMapDraw ;							// �V���h�E�}�b�v�ɑ΂���`�悩�ǂ����̃t���O
	int						ShadowMapDrawHandle ;					// �V���h�E�}�b�v�ɑ΂���`��̍ۂ́A�V���h�E�}�b�v�n���h��

	union
	{
		RGBCOLOR			DrawBright ;							// �`��P�x
		DWORD				bDrawBright ;
	} ;
	int						AlwaysDiffuseColorFlag ;				// �����_�����O�f�o�C�X���f�B�t���[�Y�J���[���g�p���邩�ǂ����ŃX�e�[�g���ω�����^�C�v�������ꍇ�A�K���f�B�t���[�Y�J���[���g�p����悤�ɂ��邩�ǂ����̃t���O

	int						IgnoreGraphColorFlag ;					// �`�悷��摜�̐F�����𖳎����邩�ǂ����̃t���O

	int						TargetScreen[ DX_RENDERTARGET_COUNT ] ;	// �`���O���t�B�b�N���ʒl
	int						TargetScreenSurface[ DX_RENDERTARGET_COUNT ] ; // �`���O���t�B�b�N���T�[�t�F�X�C���f�b�N�X
//	int						TargetScreenVramFlag ;					// �`���O���t�B�b�N���u�q�`�l�ɑ��݂��邩�ۂ�
	int						TargetZBuffer ;							// �`���y�o�b�t�@���ʒl
//	RECT					WindowDrawRect ;						// �f�X�N�g�b�v�̂���T�[�t�F�X�ɕ`�揈�����s��
																	// �ꍇ�E�C���h�E�̃N���C�A���g�̈�̋�`�f�[�^�������Ă���

	RECT					OriginalDrawRect ;						// �E�C���h�E�̈ʒu�ɂ���ĉ��ς����O�̎g�p�҂��Ӑ}���鐳�����`���`
	RECT					DrawArea ;								// �`��\��`
	RECTF					DrawAreaF ;								// �`��\��`���������_�^
	int						DrawSizeX, DrawSizeY ;					// �`��Ώۂ̃T�C�Y
	float					Draw3DScale ;							// �R�c�`�揈���̃X�P�[��

	int						MatchHardwareMatrix ;					// �R�c�f�o�C�X�ɐݒ肳��Ă���s��Ɩ{�\���̒��̍s�񂪈�v���Ă��邩�ǂ���( TRUE:��v���Ă���  FALSE:��v���Ă��Ȃ� )

	int						MatchHardwareWorldMatrix ;				// �R�c�f�o�C�X�ɐݒ肳��Ă��郏�[���h�ϊ��s��Ɩ{�\���̒��̃��[���h�ϊ��s�񂪈�v���Ă��邩�ǂ���( TRUE:��v���Ă���  FALSE:��v���Ă��Ȃ� )
	MATRIX_D				WorldMatrix ;							// ���[���h�ϊ��s��
	MATRIX					WorldMatrixF ;							// ���[���h�ϊ��s��

	int						MatchHardwareViewMatrix ;				// �R�c�f�o�C�X�ɐݒ肳��Ă���r���[�ϊ��s��Ɩ{�\���̒��̃r���[�ϊ��s�񂪈�v���Ă��邩�ǂ���( TRUE:��v���Ă���  FALSE:��v���Ă��Ȃ� )
	MATRIX_D				ViewMatrix ;							// �r���[�ϊ��s��
	MATRIX					ViewMatrixF ;							// �r���[�ϊ��s��

	int						MatchHardwareProjectionMatrix ;			// �R�c�f�o�C�X�ɐݒ肳��Ă���ˉe�ϊ��s��Ɩ{�\���̒��̎ˉe�ϊ��s�񂪈�v���Ă��邩�ǂ���( TRUE:��v���Ă���  FALSE:��v���Ă��Ȃ� )
	MATRIX_D				ProjectionMatrix ;						// �ˉe�ϊ��s��
	MATRIX					ProjectionMatrixF ;						// �ˉe�ϊ��s��

	int						ProjectionMatrixMode ;					// �ˉe�s�񃂁[�h( 0:���ߖ@  1:���ˉe  2:�s��w�� )
	double					ProjNear, ProjFar ;						// �y�N���b�s���O�� Near�ʂ� Far��
	double					ProjDotAspect ;							// �h�b�g�A�X�y�N�g��( �c / �� )
	double					ProjFov ;								// ���ߖ@���̎���p
	double					ProjSize ;								// ���ˉe���̃T�C�Y
	MATRIX_D				ProjMatrix ;							// �ˉe�s��

	int						SetDrawScreenNoSettingResetFlag ;		// SetDrawScreen �����s�����ۂɃJ�����Ȃǂ̐ݒ�̃��Z�b�g���s��Ȃ����ǂ���

	MATRIX_D				Direct3DViewportMatrix ;				// �c�����������R�c �I�ȃr���[�|�[�g�s��
	MATRIX					Direct3DViewportMatrixF ;				// �c�����������R�c �I�ȃr���[�|�[�g�s��
	MATRIX_D				Direct3DViewportMatrixAnti ;			// �c�����������R�c �I�ȃr���[�|�[�g�s��𖳌��ɂ���s��
	MATRIX					Direct3DViewportMatrixAntiF ;			// �c�����������R�c �I�ȃr���[�|�[�g�s��𖳌��ɂ���s��
	MATRIX_D				ViewportMatrix ;						// �r���[�|�[�g�s��
	MATRIX					ViewportMatrixF ;						// �r���[�|�[�g�s��
//	MATRIX					ViewportMatrix2D ;						// �Q�c�`��p�r���[�|�[�g�s��
//	MATRIX					ProjectionMatrix2D ;					// �Q�c�`��p�ˉe�s��
//	int						UseProjectionMatrix2D ;					// �Q�c�`��p�ˉe�s����g�p����ݒ�ɂȂ��Ă��邩�ǂ���( TRUE:�g�p����  FALSE:�g�p���Ȃ� )

	int						ValidBlendMatrix ;						// BlendMatrix, ViewProjectionMatrix, BillboardMatrix ���L�����ǂ���( TRUE:�L��  FALSE:���� )
	MATRIX_D				BlendMatrix ;							// ���[���h�ϊ��A�r���[�ϊ��A�ˉe�ϊ��A�r���[�|�[�g�ϊ��s����|�����킹������
	MATRIX					BlendMatrixF ;							// float�^�� BlendMatrix
	int						ValidInverseBlendMatrix ;				// BlendMatrix �̋t�s�񂪗L�����ǂ���( TRUE:�L��  FALSE:���� )
	MATRIX_D				InverseBlendMatrix ;					// BlendMatrix �̋t�s��
	MATRIX					InverseBlendMatrixF ;					// float�^�� InverseBlendMatrix
	MATRIX_D				WorldViewMatrix ;						// ���[���h�s��ƃr���[�s����|�����킹������
	MATRIX_D				ViewProjectionViewportMatrix ;			// �r���[�s��Ǝˉe�s��ƃr���[�|�[�g�s����|�����킹������
	MATRIX_D				BillboardMatrix ;						// �r���{�[�h�p�̍s��
	MATRIX					BillboardMatrixF ;						// �r���{�[�h�p�̍s��
	VECTOR_D				ViewClipPos[ 2 ][ 2 ][ 2 ] ;			// �����䒸�_( [ 0:z+ 1:z- ][ 0:top 1:bottom ][ 0:left 1:right ] )
	DOUBLE4					ClippingPlane[ 6 ] ;					// �N���b�s���O����( 0:-x 1:+x 2:-y 3:+y 4:-z 5:+z )
	int						ValidConvScreenPosToWorldPosMatrix ;	// ��ʍ��W����X�N���[�����W�ɕϊ�����ۂɎg�p����s�񂪗L�����ǂ���( TRUE:�L��  FALSE:���� )
	MATRIX_D				ConvScreenPosToWorldPosMatrix ;			// ��ʍ��W����X�N���[�����W�ɕϊ�����ۂɎg�p����s��

	int						TextureAddressTransformUse ;			// �e�N�X�`�����W�ϊ��������s�����ǂ���( TRUE:�s��  FALSE:�s��Ȃ� )
//	float					TextureTransX ;							// �e�N�X�`�����s�ړ�
//	float					TextureTransY ;
//	float					TextureScaleX ;							// �e�N�X�`���g�嗦
//	float					TextureScaleY ;
//	float					TextureRotateCenterX ;					// �e�N�X�`����]�̒��S���W
//	float					TextureRotateCenterY ;
//	float					TextureRotate ;							// �e�N�X�`����]�l
//	int						TextureMatrixValid ;					// TextureMatrix ���L�����ǂ���( TRUE:�L��  FALSE:���� )
//	MATRIX					TextureMatrix ;							// TextureTransX �� TextureScaleY �Ȃǂ��g�p���Ȃ��Őݒ肷��s��
	int						MatchHardwareTextureAddressTransformMatrix ;	// �R�c�f�o�C�X�ɐݒ肳��Ă���e�N�X�`�����W�ϊ��s��Ɩ{�\���̒��̃e�N�X�`�����W�ϊ��s�񂪈�v���Ă��邩�ǂ���( TRUE:��v���Ă���  FALSE:��v���Ă��Ȃ� )
	MATRIX					TextureAddressTransformMatrix ;			// �e�N�X�`�����W�ϊ��s��

	GRAPHICSSYS_USERRENDERINFO	UserShaderRenderInfo ;				// SetUseTextureToShader �Őݒ肳�ꂽ�e�X�e�[�W�̃e�N�X�`������A���[�U�[�ݒ�̃V�F�[�_�[�萔���Ȃ�
} ;

// �摜�쐬�Ɋ֌W������̍\����
struct GRAPHICSSYS_CREATEIMAGEDATA
{
	int						ColorBitDepth ;							// �쐬����摜�̐F�[�x
	int						ChannelBitDepth ;						// �쐬����摜�̂P�`�����l���ӂ�̃r�b�g�[�x( �����炪�ݒ肳��Ă���ꍇ�� CreateImageColorBitDepth ���D�悳��� )
//	int						TextureImageCreateFlag ;				// �e�N�X�`���摜�쐬�t���O
	int						AlphaChannelFlag ;						// ���`�����l���t���摜�쐬�w��t���O( DrawValidGraphCreateFlag �̕����D��x������ )
	int						AlphaTestFlag ;							// ���e�X�g�t���摜�쐬�w��t���O( AlphaGraphCreateFlag �̕����D��x������ )( �e�N�X�`���T�[�t�F�X�̂� )
	int						CubeMapFlag ;							// �L���[�u�}�b�v�e�N�X�`���쐬�w��t���O( 1:�L���[�u�}�b�v�e�N�X�`�����쐬����  0:�ʏ�e�N�X�`�����쐬���� )
//	int						SystemMemImageCreateFlag ;				// �V�X�e�����������g�p����摜�쐬�w��t���O( �W���T�[�t�F�X�̂� )
	int						BlendImageFlag ;						// �u�����h�����p�摜�쐬�w��t���O
	int						NotUseManagedTextureFlag ;				// �}�l�[�W�h�e�N�X�`�����g�p���Ȃ����A�t���O( 1:�g�p���Ȃ�  0:�g�p���� )
	int						PlatformTextureFormat ;					// ���ˑ��̃e�N�X�`���t�H�[�}�b�g�𒼐ڎw�肷�邽�߂Ɏg�p���邽�߂̕ϐ�( DX_TEXTUREFORMAT_DIRECT3D9_R8G8B8 �Ȃ� )

	int						DrawValidFlag ;							// �`��\�摜�쐬�w��t���O( �e�N�X�`���T�[�t�F�X�̂� )
	int						DrawValidAlphaFlag ;					// �`��\�ȃ��`�����l���t���摜�쐬�w��t���O( �e�N�X�`���T�[�t�F�X�̂� )
	int						DrawValidFloatTypeFlag ;				// �`��\��Float�^�摜�쐬�w��t���O( �e�N�X�`���T�[�t�F�X�̂� )
	int						NotDrawValidCreateZBufferFlag ;			// �`��\�摜���쐬����ۂɐ�p�̂y�o�b�t�@�͍쐬���Ȃ����ǂ���
	int						DrawValidChannelNum ;					// �`��\�摜�̃`�����l����( �e�N�X�`���T�[�t�F�X�̂� )
	int						DrawValidZBufferBitDepth ;				// �`��\�摜�̂y�o�b�t�@�̃r�b�g�[�x( �e�N�X�`���T�[�t�F�X�̂� )
	int						DrawValidMSSamples ;					// �`��\�ȉ摜�̃}���`�T���v�����O��
	int						DrawValidMSQuality ;					// �`��\�ȉ摜�̃}���`�T���v�����O�N�I���e�B

	int						NotGraphBaseDataBackupFlag ;			// �O���t�B�b�N�n���h�����쐬�����ۂɎg�p�����摜�f�[�^���o�b�N�A�b�v���Ȃ����ǂ����̃t���O( TRUE:�o�b�N�A�b�v���Ȃ�  FALSE:�o�b�N�A�b�v���� )
	int						LeftUpColorIsTransColorFlag ;			// �摜����̃s�N�Z���F�𓧉ߐF�ɂ��邩�ǂ����̃t���O
	int						UseTransColorFlag ;						// ���ߐF�Ƃ����łȂ������̋��E���������炩�ɂ��邩�A�t���O
	DWORD					TransColor ;							// ���ߐF
	int						NotUseTransColor;						// ���ߐF���g�p���Ȃ����ǂ����t���O(TRUE:�g�p���Ȃ�  FALSE:�g�p����)
	int						NotUseDivFlag ;							// �摜�������s��Ȃ����ǂ���( TRUE:�s��Ȃ�  FALSE:�s�� )
	int						MipMapCount ;							// �����ō쐬����~�b�v�}�b�v�̐�( -1:�ő僌�x���܂ō쐬���� )
	int						UserMaxTextureSize ;					// ���[�U�[�w��̃e�N�X�`���ő�T�C�Y
	int						NotUseAlphaImageLoadFlag ;				// _a ���t�����A���t�@�`�����l���p�̉摜�t�@�C����ǉ��œǂݍ��ޏ������s��Ȃ����ǂ���( TRUE:�s��Ȃ�  FALSE:�s�� )
} ;

// �f�B�X�v���C�������̏��
struct GRAPHICSSYS_DISPLAYINFO
{
	RECT					DesktopRect ;							// �f�X�N�g�b�v��ł̃��j�^�̈�
	int						DesktopSizeX ;							// �f�X�N�g�b�v��ł̕�
	int						DesktopSizeY ;							// �f�X�N�g�b�v��ł̍���
	int						DesktopColorBitDepth ;					// �f�X�N�g�b�v��ł̃J���[�r�b�g�[�x
	int						DesktopRefreshRate ;					// �f�X�N�g�b�v��ł̃��t���b�V�����[�g
	wchar_t					Name[ 128 ] ;							// ���O

	int						ModeNum ;								// �ύX�\�ȃf�B�X�v���C���[�h�̐�
	DISPLAYMODEDATA			*ModeData ;								// �f�B�X�v���C���[�h���X�g
} ;

// ��ʊ֌W�̏��̍\����
struct GRAPHICSSYS_SCREENDATA
{
	int						MainScreenSizeX ;						// ���C����ʂ̃T�C�Y
	int						MainScreenSizeY ;
	int						MainScreenSizeX_Result ;				// GetDrawScreenSize �̕Ԃ�l�ɂȂ�T�C�Y
	int						MainScreenSizeY_Result ;
	int						MainScreenColorBitDepth ;				// ���C����ʂ̃J���[�r�b�g�[�x
	int						MainScreenRefreshRate ;					// ���C����ʂ̃��t���b�V�����[�g

	int						Emulation320x240Flag ;					// 640x480 �̉�ʂ� 320x240 �̉�ʂ��o�͂��邩�ǂ����̃t���O

	int						FullScreenResolutionMode ;				// �t���X�N���[���𑜓x���[�h( DX_FSRESOLUTIONMODE_NATIVE �� )
	int						FullScreenResolutionModeAct ;			// ���ۂɎg�p����Ă���t���X�N���[���𑜓x���[�h( �Ⴆ�� FullScreenMode �� DX_FSRESOLUTIONMODE_NATIVE �ł��w��̉𑜓x�Ƀ��j�^���Ή����Ă��Ȃ��ꍇ�͂��̕ϐ��� DX_FSRESOLUTIONMODE_MAXIMUM �ɂȂ� )
	int						FullScreenScalingMode ;					// �t���X�N���[���X�P�[�����O���[�h( DX_FSSCALINGMODE_NEAREST �� )
	DISPLAYMODEDATA			FullScreenUseDispModeData ;				// �t���X�N���[�����[�h�Ŏg�p���Ă���f�B�X�v���C���[�h�̏��
	RECT					FullScreenScalingDestRect ;				// DX_FSRESOLUTIONMODE_NATIVE �ȊO�Ńt���X�N���[�����[�h���Ɏg�p����]�����`
	RECT					FullScreenDesktopRect ;					// �t���X�N���[�����[�h�ł̎g�p���Ă���f�X�N�g�b�v��̋�`

//	int						PreSetWaitVSyncFlag ;					// DxLib_Init ���Ă΂��O�� SetWaitVSyncFlag( TRUE ) ; �����s���ꂽ���ǂ����̃t���O( TRUE:���s���ꂽ  FALSE:���s����Ă��Ȃ� )
	int						NotWaitVSyncFlag ;						// �u�r�x�m�b�҂������Ȃ����ǂ����̃t���O�iTRUE�F���Ȃ� FALSE�F����j

	int						ValidGraphDisplayArea ;					// GraphDisplayArea �ɗL���Ȓl�������Ă��邩�ǂ���
	RECT					GraphDisplayArea ;						// �\��ʂɓ]�����闠��ʂ̗̈�

	int						EnableBackgroundColor ;					// �o�b�N�O���E���h�h��ׂ��p�J���[���L�����ǂ����̃t���O
	int						BackgroundRed ;							// �o�b�N�O���E���h�h��ׂ��p�J���[
	int						BackgroundGreen ;
	int						BackgroundBlue ;

	int						ScreenFlipFlag ;						// ScreenFlip�֐����Ăт������t���O
	int						Graphics_Screen_ChangeModeFlag ;		// Graphics_Screen_ChangeMode �����s�����ǂ����̃t���O

//	int						DisplayModeNum ;						// �ύX�\�ȃf�B�X�v���C���[�h�̐�
//	DISPLAYMODEDATA			*DisplayMode ;							// �f�B�X�v���C���[�h���X�g
	int						ValidUseDisplayIndex ;					// UseDisplayIndex ���L�����ǂ���( TRUE:�L��  FALSE:���� )
	int						UseDisplayIndex ;						// �g�p����f�B�X�v���C�ԍ�

	int						DisplayNum ;							// �f�B�X�v���C�̐�
	GRAPHICSSYS_DISPLAYINFO	*DisplayInfo ;							// �f�B�X�v���C�̏��

	int						DrawScreenBufferLockFlag ;				// �o�b�N�o�b�t�@�����b�N���Ă��邩�ǂ����t���O
} ;

// �ݒ�֌W���̍\����
struct GRAPHICSSYS_SETTINGDATA
{
	void					( *GraphRestoreShred )( void ) ;		// �O���t�B�b�N�����֐��̃|�C���^ 

	int						ValidHardware ;							// �n�[�h�E�G�A�`�悪�\���ǂ���( TRUE:�\  FALSE:�s�\ )
	int						NotUseHardware ;						// �n�[�h�E�G�A�`��̋@�\���g�p���Ȃ����ǂ���( TRUE:�g�p���Ȃ�  FALSE:�g�p���� )
	int						ChangeScreenModeNotGraphicsSystemFlag ;	// ��ʃ��[�h�̕ύX���ɉ摜�n���h�����폜���Ȃ����ǂ���( TRUE:���Ȃ�  FALSE:���� )
	int						FSAAMultiSampleCount ;					// FSAA�p�}���`�T���v�����O�s�N�Z����
	int						FSAAMultiSampleQuality ;				// FSAA�p�}���`�T���v�����O�N�I���e�B
} ;

// �������s�p���\�[�X�֌W�̍\����
struct GRAPHICSSYS_RESOURCE
{
	int						TempVertexBufferSize ;					// �ꎞ�I�ɒ��_�f�[�^���i�[���邽�߂̃o�b�t�@�̃T�C�Y
	void					*TempVertexBuffer ;						// �ꎞ�I�ɒ��_�f�[�^���i�[���邽�߂̃o�b�t�@

	void					*CommonBuffer[ COMMON_BUFFER_NUM ] ;	// �ėp�o�b�t�@
	DWORD					CommonBufferSize[ COMMON_BUFFER_NUM ] ;	// �ėp�o�b�t�@�̃������m�ۃT�C�Y

	int						WhiteTexHandle ;						// 8x8�̔����e�N�X�`���̃n���h��
	int						LineTexHandle ;							// �A���`�G�C���A�X�t���̐���`�悷�邽�߂̃e�N�X�`���n���h��
	int						LineTexHandle_PMA ;						// �A���`�G�C���A�X�t���̐���`�悷�邽�߂̃e�N�X�`���n���h��( ��Z�ς݃A���t�@�摜 )
	int						LineOneThicknessTexHandle ;				// �A���`�G�C���A�X�t���̐���`�悷�邽�߂̃e�N�X�`���n���h��( ����1�s�N�Z���p )
	int						LineOneThicknessTexHandle_PMA ;			// �A���`�G�C���A�X�t���̐���`�悷�邽�߂̃e�N�X�`���n���h��( ����1�s�N�Z���p )( ��Z�ς݃A���t�@�摜 )
	int						LineBoxTexHandle ;						// �A���`�G�C���A�X�t���̐���`��`�悷�邽�߂̃e�N�X�`���n���h��
	int						LineBoxTexHandle_PMA ;					// �A���`�G�C���A�X�t���̐���`��`�悷�邽�߂̃e�N�X�`���n���h��( ��Z�ς݃A���t�@�摜 )
	int						LineBoxOneThicknessTexHandle ;			// �A���`�G�C���A�X�t���̐���`��`�悷�邽�߂̃e�N�X�`���n���h��( ����1�s�N�Z���p )
	int						LineBoxOneThicknessTexHandle_PMA ;		// �A���`�G�C���A�X�t���̐���`��`�悷�邽�߂̃e�N�X�`���n���h��( ����1�s�N�Z���p )( ��Z�ς݃A���t�@�摜 )
} ;

// �n�[�h�E�F�A�A�N�Z�����[�^���֌W�̍\����
struct GRAPHICSSYS_HARDWAREINFO
{
	int						ValidSubBlend ;							// ���Z�u�����h���\���ǂ���( TRUE:�\  FALSE:�s�\ )
	int						Support4bitPaletteTexture ;				// 16�F�p���b�g�e�N�X�`�����T�|�[�g���邩�ǂ���( TRUE:�T�|�[�g����  FALSE:�T�|�[�g���Ȃ� )
	int						TextureSquareOnly ;						// �����`�e�N�X�`���̂ݍ쐬�\���ǂ���( TRUE:�����`�e�N�X�`���̂�  FALSE:�����`�e�N�X�`���ȊO���\ )
	int						RenderTargetNum ;						// �����Ƀ����_�����O�ł��郌���_�[�^�[�Q�b�g�̐�
	int						TextureSizeNonPow2Conditional ;			// �����t�Ńe�N�X�`���̃T�C�Y���Q�̂���łȂ��Ă����v���ǂ���
	int						TextureSizePow2 ;						// �e�N�X�`���̃T�C�Y���Q�̂���ł���K�v�����邩�ǂ���
	int						MaxTextureSize ;						// �ő�e�N�X�`���T�C�Y
	int						MaxTextureWidth ;						// �ő�e�N�X�`����
	int						MaxTextureHeight ;						// �ő�e�N�X�`������
	int						MaxPrimitiveCount ;						// ��x�ɕ`��ł���v���~�e�B�u�̍ő吔
	int						MaxVertexIndex ;						// ��x�̎g�p���邱�Ƃ̏o����ő咸�_��
	int						UseShader ;								// �v���O���}�u���V�F�[�_�[���g�p���邩�ǂ���( TRUE:�g�p����  FALSE:�g�p���Ȃ� )
	int						UseVertexColorBGRAFormat ;				// BGRA�J���[�^�C�v�̒��_�J���[���g�p���邩�ǂ���( TRUE:�g�p����  FALSE:�g�p���Ȃ� )
	int						DrawFloatCoordType ;					// DrawGraphF ���̕��������_�l�ō��W���w�肷��֐��ɂ�������W�^�C�v( DX_DRAWFLOATCOORDTYPE_DIRECT3D9 �Ȃ� )
	int						OggTheoraUseDrawValidGraph ;			// OggTehora�Đ��p�̉摜�͕`��\�摜�ɂ��邩�ǂ���( TRUE:�`��\�摜�ɂ���  FALSE:�`��\�摜�ɂ��Ȃ� )
} ;

// �\�t�g�E�G�A�����_�����O�Ŏg�p������̍\����
struct GRAPHICSSYS_SOFTRENDERDATA
{
	MEMIMG					MainBufferMemImg ;					// ���C����ʗp MEMIMG
	MEMIMG					SubBufferMemImg ;					// �⏕��ʗp MEMIMG
	MEMIMG					FontScreenMemImgNormal ;			// �������`��Ȃǂ̎��Ɏg���t�H���g�p MEMIMG ( �A���t�@�Ȃ� )
	MEMIMG					FontScreenMemImgAlpha ;				// �������`��Ȃǂ̎��Ɏg���t�H���g�p MEMIMG ( �A���t�@�� )
	MEMIMG					*TargetMemImg ;						// �`��Ώۂ� MEMIMG
	MEMIMG					*BlendMemImg ;						// �u�����h�`��p MEMIMG
} ;

// �O���t�B�N�X�V�X�e���p�f�[�^�\����
struct GRAPHICSSYSTEMDATA
{
	int								InitializeFlag ;						// �������t���O

//	int								NotDrawFlag ;							// �`��s�\�t���O

//	int								Screen3DWidth ;							// �R�c�`����s���ۂ̑z�肷��X�N���[���̃T�C�Y
//	int								Screen3DHeight ;
//	int								Screen3DCenterX ;						// �R�c�`����s���ۂ̑z�肷��X�N���[���̒��S���W
//	int								Screen3DCenterY ;
//	LONGLONG						FrameCounter ;							// �t���[���J�E���^�[

	GRAPHICSSYS_SETTINGDATA			Setting ;								// �ݒ�֌W�̏��

	GRAPHICSSYS_HARDWAREINFO		HardInfo ;								// �n�[�h�E�F�A�A�N�Z�����[�^�֌W�̏��

	GRAPHICSSYS_SOFTRENDERDATA		SoftRender ;							// �\�t�g�E�G�A�����_�����O�Ŏg�p������

	GRAPHICSSYS_SCREENDATA			Screen ;								// ��ʊ֌W�̏��

	GRAPHICSSYS_CREATEIMAGEDATA		CreateImage ;							// �摜�쐬�֌W�̏��

	GRAPHICSSYS_RESOURCE			Resource ;								// �������s�p���\�[�X�֌W�̍\����

	GRAPHICSSYS_DRAWSETTINGDATA		DrawSetting ;							// �`��ݒ�֌W�̏��

	GRAPHICSSYS_LIGHTATA			Light ;									// ���C�g�֌W�̏��

	GRAPHICSSYS_CAMERA				Camera ;								// �J�����֌W�̏��

	int								ChangeSettingFlag ;						// �ݒ肪�ύX���ꂽ���ǂ���
} ;

// �������ϐ��錾---------------------------------------------------------------

// �`�����̊�{�I�ȏ��
extern GRAPHICSSYSTEMDATA GraphicsSysData ;

// �֐��v���g�^�C�v�錾-----------------------------------------------------------

// �O���t�B�b�N�֘A�̏������ƌ�n��
extern	int		Graphics_Initialize( void ) ;					// �O���t�B�b�N�X�V�X�e���̏�����
extern	int		Graphics_Terminate( void ) ;					// �O���t�B�b�N�V�X�e���̌�n��
extern	int		Graphics_RestoreOrChangeSetupGraphSystem( int Change, int ScreenSizeX = -1, int ScreenSizeY = -1, int ColorBitDepth = -1, int RefreshRate = -1 ) ;		// �O���t�B�b�N�X�V�X�e���̕��A�A���͕ύX�t���̍ăZ�b�g�A�b�v���s��








// ��ʊ֌W�֐�
extern	int		Graphics_Screen_SetupFullScreenModeInfo( void ) ;											// �t���X�N���[�����[�h�̃��[�h�̃`�F�b�N��g�p����𑜓x���̌�����s��
extern	int		Graphics_Screen_SetupFullScreenScalingDestRect( void ) ;									// GSYS.Screen.FullScreenScalingDestRect �̒l���Z�b�g�A�b�v����
extern	int		Graphics_Screen_ScreenPosConvSubBackbufferPos( int ScreenPosX, int ScreenPosY, int *BackBufferPosX, int *BackBufferPosY ) ;	// �X�N���[�����W���T�u�o�b�N�o�b�t�@�[���W�ɕϊ�����
extern	int		Graphics_Screen_SubBackbufferPosConvScreenPos( int BackBufferPosX, int BackBufferPosY, int *ScreenPosX, int *ScreenPosY ) ;	// �T�u�o�b�N�o�b�t�@�[���W���X�N���[�����W�ɕϊ�����
extern	int		Graphics_Screen_SetZBufferMode( int ZBufferSizeX, int ZBufferSizeY, int ZBufferBitDepth ) ;	// ���C����ʂ̂y�o�b�t�@�̐ݒ��ύX����
extern	int		Graphics_Screen_SetupUseZBuffer( void ) ;													// �ݒ�Ɋ�Â��Ďg�p����y�o�b�t�@���Z�b�g����
extern	void	Graphics_Screen_SetMainScreenSize( int SizeX, int SizeY ) ;									// ���C����ʂ̃T�C�Y�l��ύX����
extern	int		Graphics_Screen_ChangeMode( int ScreenSizeX, int ScreenSizeY, int ColorBitDepth, int ChangeWindowFlag, int RefreshRate ) ;				// ��ʃ��[�h�̕ύX�Q
extern	int		Graphics_Screen_LockDrawScreen( RECT *LockRect, BASEIMAGE *BaseImage, int TargetScreen/* = -1*/, int TargetScreenSurface/* = -1*/, int ReadOnly/* = TRUE*/, int TargetScreenTextureNo/* = 0*/ ) ;	// �`���o�b�t�@�����b�N����
extern	int		Graphics_Screen_UnlockDrawScreen( void ) ;													// �`���o�b�t�@���A�����b�N����
extern	int		Graphics_Screen_FlipBase( void ) ;															// ScreenFlip �̃x�[�X�֐�
extern	int		Graphics_Screen_ScreenCopyBase( int DrawTargetFrontScreenMode_Copy ) ;						// ScreenCopy �̃x�[�X�֐�








// �摜�֌W�֐�
extern	int		Graphics_Image_SetupFormatDesc( IMAGEFORMATDESC *Format, SETUP_GRAPHHANDLE_GPARAM *GParam, int Width, int Height, int AlphaValidFlag, int UsePaletteFlag, int PaletteBitDepth, int BaseFormat, int MipMapCount ) ; // �O���t�B�b�N�n���h���ɉ摜�f�[�^��]�����邽�߂̊֐�
extern	int		Graphics_Image_DeleteDeviceLostDelete( void ) ;						// �f�o�C�X���X�g�������ɍ폜����t���O�������Ă���O���t�B�b�N���폜����
extern	int		Graphics_Image_CheckMultiSampleDrawValid( int GrHandle ) ;			// �l�r�`�`���g�p����`��\�摜���ǂ����𒲂ׂ�( TRUE:MSAA�摜  FALSE:MSAA�摜�ł͂Ȃ� )
extern	int		Graphics_Image_AddHandle( int ASyncThread ) ;																			// �V�����O���t�B�b�N�n���h�����m�ۂ���
extern	int		Graphics_Image_SetupHandle_UseGParam( SETUP_GRAPHHANDLE_GPARAM *GParam, int GrHandle, int Width, int Height, int TextureFlag, int AlphaValidFlag, int UsePaletteFlag, int PaletteBitDepth, int BaseFormat/* = DX_BASEIMAGE_FORMAT_NORMAL*/, int MipMapCount, int ASyncThread ) ;							// SetupGraphHandle �̃O���[�o���ϐ��ɃA�N�Z�X���Ȃ��o�[�W����
extern	int		Graphics_Image_ListUpTexSize( int Size, short *SizeList, int NotDivFlag, int Pow2Flag, int MaxTextureSize ) ;			// �w��̃e�N�X�`���[�T�C�Y����肭��������
extern	int		Graphics_Image_InitializeHandle( HANDLEINFO *HandleInfo ) ;																// �O���t�B�b�N�n���h���̏�����
extern	int		Graphics_Image_TerminateHandle( HANDLEINFO *HandleInfo ) ;																// �O���t�B�b�N�n���h���̌�n��
extern	int		Graphics_Image_InitializeDerivationHandle( int GrHandle, int IsFloat, int SrcXI, float SrcXF, int SrcYI, float SrcYF, int WidthI, float WidthF, int HeightI, float HeightF, int SrcGrHandle, int ASyncThread = FALSE ) ;			// �w�蕔�������𔲂��o�����O���t�B�b�N�n���h��������������
extern	int		Graphics_Image_InitializeDrawInfo( int GrHandle, int IsFloat, int ASyncThread = FALSE ) ;			// �O���t�B�b�N�n���h���̕`���������������
extern	int		Graphics_Image_IsValidHandle( int GrHandle ) ;															// �O���t�B�b�N�n���h�����L�����ǂ����𒲂ׂ�( TRUE:�L��  FALSE:���� )
extern	int		Graphics_Image_CreateDXGraph_UseGParam( SETUP_GRAPHHANDLE_GPARAM *GParam, int GrHandle, const BASEIMAGE *RgbBaseImage, const BASEIMAGE *AlphaBaseImage, int TextureFlag, int ASyncThread = FALSE ) ;																										// CreateDXGraph �̃O���[�o���ϐ��ɃA�N�Z�X���Ȃ��o�[�W����
extern	int		Graphics_Image_DerivationGraph_UseGParam( int IsFloat, int SrcXI, float SrcXF, int SrcYI, float SrcYF, int WidthI, float WidthF, int HeightI, float HeightF, int SrcGraphHandle, int ASyncThread = FALSE ) ;																																								// DerivationGraph �̃O���[�o���ϐ��ɃA�N�Z�X���Ȃ��o�[�W����
#ifndef DX_NON_MOVIE
extern	int		Graphics_Image_OpenMovie_UseGParam( LOADGRAPH_GPARAM *GParam, int GrHandle, const wchar_t *GraphName, const void *FileImage, size_t FileImageSize, int TextureFlag, int SurfaceMode = DX_MOVIESURFACE_NORMAL, int ASyncThread = FALSE ) ;
#endif
extern	int		Graphics_Image_DerivationGraphBase( int GrHandle, int IsFloat, int SrcXI, float SrcXF, int SrcYI, float SrcYF, int WidthI, float WidthF, int HeightI, float HeightF, int SrcGraphHandle, int ASyncThread = FALSE ) ;																												// �O���t�B�b�N�n���h�����쐬���Ȃ� DerivationGraph
extern	int		Graphics_Image_SetBaseInfo_UseGParam( SETGRAPHBASEINFO_GPARAM *GParam, int GrHandle, const wchar_t *FileName, const COLORDATA *BmpColorData, HBITMAP RgbBmp, HBITMAP AlphaBmp, const void *MemImage, int MemImageSize,
										   const void *AlphaMemImage, int AlphaMemImageSize, const BASEIMAGE *BaseImage, const BASEIMAGE *AlphaBaseImage, int ReverseFlag, int ConvertPremultipliedAlpha, int UnionGrHandle, int ASyncThread ) ;																														// SetGraphBaseInfo �̃O���[�o���ϐ��ɃA�N�Z�X���Ȃ��o�[�W����
extern	int		Graphics_Image_SetGraphBaseInfo( int GrHandle, const wchar_t *FileName, const COLORDATA *BmpColorData, HBITMAP RgbBmp, HBITMAP AlphaBmp, const void *MemImage, int MemImageSize, const void *AlphaMemImage, int AlphaMemImageSize, const BASEIMAGE *BaseImage, const BASEIMAGE *AlphaBaseImage, int ReverseFlag, int ConvertPremultipliedAlpha, int UnionGrHandle, int ASyncThread ) ;		// �摜�̌��f�[�^�̏���ۑ�����
extern	int		Graphics_Image_SetName( int Handle, const wchar_t *GraphName, int ReverseFlag, int ConvertPremultipliedAlpha, int UnionGrHandle, int ASyncThread ) ;	// ����̃t�@�C������摜��ǂݍ��񂾏ꍇ�̃t�@�C���p�X���Z�b�g����
extern	int		Graphics_Image_FillGraph_UseGParam( int GrHandle, int Red, int Green, int Blue, int Alpha, int ASyncThread ) ;																																																// FillGraph �̃O���[�o���ϐ��ɃA�N�Z�X���Ȃ��o�[�W����
#ifndef DX_NON_MOVIE
extern	void	Graphics_Image_UpdateGraphMovie( MOVIEGRAPH *Movie, DWORD_PTR GrHandle ) ;				// ���[�r�[�摜���X�V����
#endif

// BltBmpOrGraphImageToGraph �̓����֐�
extern	int		Graphics_Image_BltBmpOrGraphImageToGraphBase(
	const BASEIMAGE	*RgbBaseImage,
	const BASEIMAGE	*AlphaBaseImage,
	      int		CopyPointX,
	      int		CopyPointY,
	      int		GrHandle,
	      int		UseTransColorConvAlpha = TRUE,
	      int		ASyncThread = FALSE
) ;

// BltBmpOrGraphImageToGraph2 �̓����֐�
extern	int		Graphics_Image_BltBmpOrGraphImageToGraph2Base(
	const BASEIMAGE	*RgbBaseImage,
	const BASEIMAGE	*AlphaBaseImage,
	const RECT		*SrcRect,
	      int		DestX,
	      int		DestY,
	      int		GrHandle,
	      int		UseTransColorConvAlpha = TRUE,
	      int		ASyncThread = FALSE
) ;

// BltBmpOrGraphImageToDivGraph �̓����֐�
extern	int		Graphics_Image_BltBmpOrGraphImageToDivGraphBase(
	const BASEIMAGE	*RgbBaseImage,
	const BASEIMAGE	*AlphaBaseImage,
	      int		AllNum,
	      int		XNum,
	      int		YNum,
		  int		IsFloat,
	      int		WidthI,
	      float		WidthF,
	      int		HeightI,
	      float		HeightF,
	const int		*GrHandle,
	      int		ReverseFlag,
	      int		UseTransColorConvAlpha = TRUE,
	      int		ASyncThread = FALSE
) ;

// �O���t�B�b�N�n���h���ɉ摜�f�[�^��]�����邽�߂̊֐�
extern	int		Graphics_Image_BltBmpOrBaseImageToGraph3(
	const RECT		*SrcRect,
	      int		DestX,
	      int		DestY,
	      int		GrHandle,
	const BASEIMAGE	*RgbBaseImage,
	const BASEIMAGE	*AlphaBaseImage,
	      int		RedIsAlphaFlag = FALSE,
	      int		UseTransColorConvAlpha = TRUE,
	      int		TargetOrig = FALSE,
	      int		ASyncThread = FALSE
) ;

// �w��̃I���W�i���摜���ɓ]�������`�����쐬����
// �߂�l  -1:�͈͊O   0:����I��
extern	int		Graphics_Image_BltBmpOrBaseImageToGraph3_Make_OrigTex_MoveRect(
	const IMAGEDATA_ORIG_HARD_TEX	*OrigTex,
	const RECT						*SrcRect,
		  int						SrcWidth,
		  int						SrcHeight,
	      int						DestX,
	      int						DestY,
	      RECT						*DestRect,
	      RECT						*MoveRect
) ;

// �w��̕`��p�摜���ɓ]�������`�����쐬����
// �߂�l  -1:�͈͊O   0:����I��
extern	int		Graphics_Image_BltBmpOrBaseImageToGraph3_Make_DrawTex_MoveRect(
	const IMAGEDATA_HARD_DRAW		*DrawTex,
	const RECT						*SrcRect,
		  int						SrcWidth,
		  int						SrcHeight,
	      int						DestX,
	      int						DestY,
	      RECT						*DestRect,
	      RECT						*MoveRect
) ;

// �`��\�摜��o�b�N�o�b�t�@����w��̈�̃O���t�B�b�N���擾����
extern	int			Graphics_Image_GetDrawScreenGraphBase( int TargetScreen, int TargetScreenSurface, int x1, int y1, int x2, int y2, int destX, int destY, int GrHandle ) ;
extern	IMAGEDATA *	Graphics_Image_GetData( int GrHandle, int ASyncThread = FALSE ) ;				// �O���t�B�b�N�̃f�[�^���C���f�b�N�X�l������o��
extern	int			Graphics_Image_GetWhiteTexHandle( void ) ;										// �^�����̃e�N�X�`���̃n���h�����擾����
extern	int			Graphics_Image_GetLineTexHandle( int IsPMA ) ;									// �A���`�G�C���A�X�t���̐���`�悷�邽�߂̃e�N�X�`���n���h�����擾����
extern	int			Graphics_Image_GetLineOneThicknessTexHandle( int IsPMA ) ;						// �A���`�G�C���A�X�t���̐���`�悷�邽�߂̃e�N�X�`���n���h�����擾����( ����1�s�N�Z���p )
extern	int			Graphics_Image_GetLineBoxTexHandle( int IsPMA ) ;								// �A���`�G�C���A�X�t���̐���`��`�悷�邽�߂̃e�N�X�`���n���h�����擾����
extern	int			Graphics_Image_GetLineBoxOneThicknessTexHandle( int IsPMA ) ;					// �A���`�G�C���A�X�t���̐���`��`�悷�邽�߂̃e�N�X�`���n���h�����擾����( ����1�s�N�Z���p )

extern	int		Graphics_Image_MakeGraph_UseGParam( SETUP_GRAPHHANDLE_GPARAM *GParam, int SizeX, int SizeY, int NotUse3DFlag, int UsePaletteFlag, int PaletteBitDepth, int ASyncLoadFlag = FALSE, int ASyncThread = FALSE ) ;																																											// ��̃O���t�B�b�N�n���h�����쐬����֐�
extern	int		Graphics_Image_CreateGraph_UseGParam(                  LOADGRAPH_PARAM *Param, int ASyncLoadFlag = FALSE, int ASyncThread = FALSE ) ;																																																			// �摜�f�[�^����O���t�B�b�N�n���h�����쐬����֐�
extern	int		Graphics_Image_CreateDivGraph_UseGParam(               LOADGRAPH_PARAM *Param, int ASyncLoadFlag = FALSE, int ASyncThread = FALSE ) ;																																																			// �摜�f�[�^�𕪊����ăO���t�B�b�N�n���h�����쐬����֐�
extern	int		Graphics_Image_LoadBmpToGraph_UseGParam(               LOADGRAPH_GPARAM *GParam, int ReCreateFlag, int GrHandle, const wchar_t *GraphName, int TextureFlag, int ReverseFlag, int SurfaceMode = DX_MOVIESURFACE_NORMAL, int ASyncLoadFlag = FALSE ) ;																								// LoadBmpToGraph �̃O���[�o���ϐ��ɃA�N�Z�X���Ȃ��o�[�W����
extern	int		Graphics_Image_LoadDivBmpToGraph_UseGParam(            LOADGRAPH_GPARAM *GParam, int ReCreateFlag, const wchar_t *FileName, int AllNum, int XNum, int YNum, int IsFloat, int SizeXI, float SizeXF, int SizeYI, float SizeYF, int *HandleBuf, int TextureFlag, int ReverseFlag, int ASyncLoadFlag = FALSE, int ASyncThread = FALSE ) ;																	// LoadDivBmpToGraph �̃O���[�o���ϐ��ɃA�N�Z�X���Ȃ��o�[�W����
extern	int		Graphics_Image_CreateGraphFromMem_UseGParam(           LOADGRAPH_GPARAM *GParam, int ReCreateFlag, int GrHandle, const void *MemImage, int MemImageSize, const void *AlphaImage = NULL, int AlphaImageSize = 0, int TextureFlag = TRUE, int ReverseFlag = FALSE, int ASyncLoadFlag = FALSE ) ;																// CreateGraphFromMem �̃O���[�o���ϐ��ɃA�N�Z�X���Ȃ��o�[�W����
extern	int		Graphics_Image_CreateDivGraphFromMem_UseGParam(        LOADGRAPH_GPARAM *GParam, int ReCreateFlag, const void *MemImage, int MemImageSize, int AllNum, int XNum, int YNum, int IsFloat, int SizeXI, float SizeXF, int SizeYI, float SizeYF, int *HandleBuf,int TextureFlag, int ReverseFlag, const void *AlphaImage, int AlphaImageSize, int ASyncLoadFlag = FALSE, int ASyncThread = FALSE ) ;		// CreateDivGraphFromMem �̃O���[�o���ϐ��ɃA�N�Z�X���Ȃ��o�[�W����
extern	int		Graphics_Image_CreateGraphFromBmp_UseGParam(           LOADGRAPH_GPARAM *GParam, int ReCreateFlag, int GrHandle, const BITMAPINFO *BmpInfo, const void *GraphData, const BITMAPINFO *AlphaInfo = NULL, const void *AlphaData = NULL, int TextureFlag = TRUE, int ReverseFlag = FALSE, int ASyncLoadFlag = FALSE ) ;											// CreateGraphFromBmp �̃O���[�o���ϐ��ɃA�N�Z�X���Ȃ��o�[�W����
extern	int		Graphics_Image_CreateDivGraphFromBmp_UseGParam(        LOADGRAPH_GPARAM *GParam, int ReCreateFlag, const BITMAPINFO *BmpInfo, const void *GraphData, int AllNum, int XNum, int YNum, int IsFloat, int SizeXI, float SizeXF, int SizeYI, float SizeYF, int *HandleBuf,int TextureFlag, int ReverseFlag, const BITMAPINFO *AlphaInfo, const void *AlphaData, int ASyncLoadFlag = FALSE ) ;	// CreateDivGraphFromBmp �̃O���[�o���ϐ��ɃA�N�Z�X���Ȃ��o�[�W����
extern	int		Graphics_Image_CreateGraphFromGraphImage_UseGParam(    LOADGRAPH_GPARAM *GParam, int ReCreateFlag, int GrHandle, const BASEIMAGE *Image, const BASEIMAGE *AlphaImage, int TextureFlag = TRUE , int ReverseFlag = FALSE, int ASyncLoadFlag = FALSE, int ASyncThread = FALSE ) ;																	// CreateGraphFromGraphImage �̃O���[�o���ϐ��ɃA�N�Z�X���Ȃ��o�[�W����
extern	int		Graphics_Image_CreateDivGraphFromGraphImage_UseGParam( LOADGRAPH_GPARAM *GParam, int ReCreateFlag, const BASEIMAGE *Image, const BASEIMAGE *AlphaImage, int AllNum, int XNum, int YNum, int IsFloat, int SizeXI, float SizeXF, int SizeYI, float SizeYF, int *HandleBuf, int TextureFlag = TRUE , int ReverseFlag = FALSE, int ASyncLoadFlag = FALSE ) ;									// CreateDivGraphFromGraphImage �̃O���[�o���ϐ��ɃA�N�Z�X���Ȃ��o�[�W����
extern	int		Graphics_Image_CreateGraphFromGraphImageBase_UseGParam(    CREATE_GRAPHHANDLE_AND_BLTGRAPHIMAGE_GPARAM *GParam, int ReCreateFlag, int GrHandle,   BASEIMAGE *Image, const BASEIMAGE *AlphaImage, int TextureFlag, int ASyncThread = FALSE ) ;																							// Graphics_Image_CreateGraphFromGraphImageBase �̃O���[�o���ϐ��ɃA�N�Z�X���Ȃ��o�[�W����
extern	int		Graphics_Image_CreateDivGraphFromGraphImageBase_UseGParam( CREATE_GRAPHHANDLE_AND_BLTGRAPHIMAGE_GPARAM *GParam, int ReCreateFlag, int BaseHandle, BASEIMAGE *Image, const BASEIMAGE *AlphaImage, int AllNum, int XNum, int YNum, int IsFloat, int SizeXI, float SizeXF, int SizeYI, float SizeYF, int *HandleBuf, int TextureFlag, int ReverseFlag, int ASyncThread = FALSE ) ;		// Graphics_Image_CreateDivGraphFromGraphImageBase �̃O���[�o���ϐ��ɃA�N�Z�X���Ȃ��o�[�W����
extern	int		Graphics_Image_CreateGraphFromGraphImageBase(      BASEIMAGE *Image, const BASEIMAGE *AlphaImage, int TextureFlag, int ASyncThread ) ;																								// CreateGraphFromGraphImage �̓����֐�
extern	int		Graphics_Image_CreateDivGraphFromGraphImageBase(   BASEIMAGE *Image, const BASEIMAGE *AlphaImage, int AllNum, int XNum, int YNum, int IsFloat, int SizeXI, float SizeXF, int SizeYI, float SizeYF, int *HandleBuf, int TextureFlag, int ReverseFlag ) ;						// CreateDivGraphFromGraphImage �̓����֐�
extern	int		Graphics_Image_ReCreateGraphFromGraphImageBase(    BASEIMAGE *Image, const BASEIMAGE *AlphaImage, int GrHandle, int TextureFlag ) ;																									// ReCreateGraphFromGraphImage �̓����֐�
extern	int		Graphics_Image_ReCreateDivGraphFromGraphImageBase( BASEIMAGE *Image, const BASEIMAGE *AlphaImage, int AllNum, int XNum, int YNum, int IsFloat, int SizeXI, float SizeXF, int SizeYI, float SizeYF, int *HandleBuf, int TextureFlag , int ReverseFlag ) ;						// ReCreateDivGraphFromGraphImage �̓����֐�
extern	void	Graphics_Image_InitCreateGraphHandleAndBltGraphImageGParam( CREATE_GRAPHHANDLE_AND_BLTGRAPHIMAGE_GPARAM *GParam ) ;			// CREATE_GRAPHHANDLE_AND_BLTGRAPHIMAGE_GPARAM �̃f�[�^���Z�b�g����
extern	void	Graphics_Image_InitSetupGraphHandleGParam( SETUP_GRAPHHANDLE_GPARAM *GParam ) ;												// SETUP_GRAPHHANDLE_GPARAM �̃f�[�^���Z�b�g����
extern	void	Graphics_Image_InitSetupGraphHandleGParam_Normal_NonDrawValid( SETUP_GRAPHHANDLE_GPARAM *GParam, int BitDepth = 32, int AlphaChannel = TRUE, int AlphaTest = FALSE ) ;
extern	void	Graphics_Image_InitSetupGraphHandleGParam_Normal_DrawValid_NoneZBuffer( SETUP_GRAPHHANDLE_GPARAM *GParam, int BitDepth = 32, int AlphaChannel = TRUE ) ;
extern	void	Graphics_Image_InitSetGraphBaseInfoGParam( SETGRAPHBASEINFO_GPARAM *GParam ) ;													// SETGRAPHBASEINFO_GPARAM �̃f�[�^���Z�b�g����
extern	void	Graphics_Image_InitLoadGraphGParam( LOADGRAPH_GPARAM *GParam ) ;																// LOADGRAPH_GPARAM �̃f�[�^���Z�b�g����
#ifndef DX_NON_MOVIE
extern	void	Graphics_Image_InitOpenMovieGParam( OPENMOVIE_GPARAM *GParam ) ;																// OPENMOVIE_GPARAM �̃f�[�^���Z�b�g����
#endif







// �`��ݒ�֌W�֐�
extern	int				Graphics_DrawSetting_Initialize( void ) ;															// �O���t�B�b�N�`��ݒ�֌W�̏���������
extern	void FASTCALL	Graphics_DrawSetting_SetDrawBrightToOneParam( DWORD Bright ) ;										// SetDrawBright �̈��������
extern	void FASTCALL	Graphics_DrawSetting_BlendModeSub_Pre( RECT *DrawRect ) ;
extern	void FASTCALL	Graphics_DrawSetting_BlendModeSub_Post( RECT *DrawRect ) ;
extern	int				Graphics_DrawSetting_SetBlendGraphParamBase( int BlendGraph, int BlendType, va_list ParamList ) ;	// SetBlendGraphParam �̉ϒ������p�����[�^�t��
extern	int				Graphics_DrawSetting_RefreshAlphaChDrawMode( void ) ;												// �`���ɐ��������l���������ނ��ǂ����̃t���O���X�V����
extern	void			Graphics_DrawSetting_ApplyLibMatrixToHardware( void ) ;												// ��{�f�[�^�ɐݒ肳��Ă���s����n�[�h�E�G�A�ɔ��f����
extern	void			Graphics_DrawSetting_ApplyLibFogToHardware( void ) ;												// ��{�f�[�^�ɐݒ肳��Ă���t�H�O�����n�[�h�E�G�A�ɔ��f����
extern	int				Graphics_DrawSetting_SetTextureAddressTransformMatrix_Direct( int Use, MATRIX *Matrix, int Stage = -1 ) ;		// �e�N�X�`�����W�ϊ��s����Z�b�g����
extern	int				Graphics_DrawSetting_SetTransformToWorld_Direct( MATRIX *Matrix ) ;									// ���[�J�������[���h�s���ύX����
extern	int				Graphics_DrawSetting_SetTransformToProjection_Direct( const MATRIX_D *Matrix ) ;					// ���e�ϊ��p�s����Z�b�g����
extern	void			Graphics_DrawSetting_RefreshProjectionMatrix( void ) ;												// �ˉe�s��p�����[�^�Ɋ�Â��Ďˉe�s����\�z����
extern	void			Graphics_DrawSetting_RefreshBlendTransformMatrix( void ) ;											// ���_�ϊ��s����|�����킹���s����X�V����
//extern	void		Graphics_DrawSetting_SetUse2DProjectionMatrix( int Use2DProjectionMatrix ) ;						// �g�p����ˉe�s����R�c�p�ɂ��邩�Q�c�p�ɂ��邩��ݒ肷��
extern	int				Graphics_DrawSetting_GetScreenDrawSettingInfo(       SCREENDRAWSETTINGINFO *ScreenDrawSettingInfo ) ;	// �c�w���C�u���������� SetDrawScreen ���g�p���ĕ`����ύX����ۂ́A���̃J�����ݒ��`��̈�𕜌�����ׂ̏����擾���鏈�����s��
extern	int				Graphics_DrawSetting_SetScreenDrawSettingInfo( const SCREENDRAWSETTINGINFO *ScreenDrawSettingInfo ) ;	// �c�w���C�u���������� SetDrawScreen ���g�p���ĕ`����ύX����ۂ́A���̃J�����ݒ��`��̈�𕜌����鏈�����s��








// �`��֌W�֐�
extern	int		Graphics_Draw_GetCircle_ThicknessDrawPosition( int x, int y, int r, int Thickness, short ( *CirclePos )[ 5 ] ) ;		// ���̕��t���~�̕`��p���_���擾����֐�
extern	int		Graphics_Draw_GetOval_ThicknessDrawPosition( int x, int y, int rx, int ry, int Thickness, short ( *CirclePos )[ 5 ] ) ;	// ���̕��t���ȉ~�̕`��p���_���擾����֐�
extern	int		Graphics_Draw_DrawSimpleQuadrangleGraphF( const GRAPHICS_DRAW_DRAWSIMPLEQUADRANGLEGRAPHF_PARAM *Param ) ;				// ���W�␳���s��Ȃ��Q�|���S���`����s��( �P�e�N�X�`���摜�̂ݗL�� )
extern	int		Graphics_Draw_DrawSimpleTriangleGraphF(   const GRAPHICS_DRAW_DRAWSIMPLETRIANGLEGRAPHF_PARAM   *Param ) ;				// ���W�␳���s��Ȃ��P�|���S���`����s��( �P�e�N�X�`���摜�̂ݗL�� )








// �J�����֌W�֐�
extern	int		Graphics_Camera_CheckCameraViewClip_Box_PosDim(  VECTOR   *CheckBoxPos ) ;					// �W���W�Ō`���{�b�N�X���J�����̎��E�ɓ����Ă��邩�ǂ����𔻒肷��( �߂�l TRUE:���E�ɓ����Ă��Ȃ�  FALSE:���E�ɓ����Ă��� )( CheckPos�� VECTOR 8���̔z��̐擪�A�h���X�A�z��̊e�v�f�ԍ��̓��e 0:+x +y +z   1:-x +y +z   2:-x -y +z   3:+x -y +z   4:+x -y -z   5:+x +y -z   6:-x +y -z   7:-x -y -z )
extern	int		Graphics_Camera_CheckCameraViewClip_Box_PosDimD( VECTOR_D *CheckBoxPos ) ;					// �W���W�Ō`���{�b�N�X���J�����̎��E�ɓ����Ă��邩�ǂ����𔻒肷��( �߂�l TRUE:���E�ɓ����Ă��Ȃ�  FALSE:���E�ɓ����Ă��� )( CheckPos�� VECTOR 8���̔z��̐擪�A�h���X�A�z��̊e�v�f�ԍ��̓��e 0:+x +y +z   1:-x +y +z   2:-x -y +z   3:+x -y +z   4:+x -y -z   5:+x +y -z   6:-x +y -z   7:-x -y -z )
extern	void	Graphics_Camera_CalcCameraRollViewMatrix( void ) ;											// �r���[�s�񂩂琅���A�����A�P��p�x���Z�o����








// ���C�g�֌W�֐�
extern	int		Graphics_Light_AddHandle( void ) ;														// ���C�g�n���h���̒ǉ�
extern	int		Graphics_Light_RefreshState( void ) ;													// ���C�g�̕ύX�𔽉f����








// �V�F�[�_�[�֌W�֐�
extern	int		Graphics_Shader_CreateHandle( int ASyncThread ) ;																	// �V�F�[�_�[�n���h�����쐬����
extern	int		Graphics_Shader_CreateHandle_UseGParam( int IsVertexShader, void *Image, int ImageSize, int ImageAfterFree, int ASyncLoadFlag = FALSE, int ASyncThread = FALSE ) ;		// �V�F�[�_�[�n���h�����쐬����
extern	int		Graphics_Shader_LoadShader_UseGParam( int IsVertexShader, const wchar_t *FileName, int ASyncLoadFlag = FALSE ) ;	// �V�F�[�_�[�o�C�i�����t�@�C������ǂݍ���
extern	int		Graphics_Shader_InitializeHandle( HANDLEINFO *HandleInfo ) ;														// �V�F�[�_�[�n���h���̏�����
extern	int		Graphics_Shader_TerminateHandle( HANDLEINFO *HandleInfo ) ;															// �V�F�[�_�[�n���h���̌�n��
extern	SHADERHANDLEDATA *Graphics_Shader_GetData( int ShaderHandle, int ASyncThread = FALSE ) ;									// �V�F�[�_�[�f�[�^���n���h���l������o��







// �萔�o�b�t�@�֌W�֐�
extern	int		Graphics_ShaderConstantBuffer_CreateHandle( int ASyncThread ) ;																// �V�F�[�_�[�p�萔�o�b�t�@�n���h�����쐬����
extern	int		Graphics_ShaderConstantBuffer_Create( int BufferSize, int ASyncLoadFlag = FALSE,  int ASyncThread = FALSE ) ;				// �V�F�[�_�[�p�萔�o�b�t�@�n���h�����쐬����
extern	int		Graphics_ShaderConstantBuffer_InitializeHandle( HANDLEINFO *HandleInfo ) ;													// �V�F�[�_�[�p�萔�o�b�t�@�n���h���̏�����
extern	int		Graphics_ShaderConstantBuffer_TerminateHandle( HANDLEINFO *HandleInfo ) ;													// �V�F�[�_�[�p�萔�o�b�t�@�n���h���̌�n��








// ���_�o�b�t�@�E�C���f�b�N�X�o�b�t�@�֌W�֐�
extern	int		Graphics_VertexBuffer_Create( int VertexNum, int VertexType /* DX_VERTEX_TYPE_NORMAL_3D �� */, int ASyncThread ) ;			// ���_�o�b�t�@���쐬����
extern	int		Graphics_VertexBuffer_SetupHandle( int VertexBufHandle, int VertexNum, int VertexType /* DX_VERTEX_TYPE_NORMAL_3D �� */ ) ;	// ���_�o�b�t�@�n���h���̃Z�b�g�A�b�v���s��
extern	int		Graphics_VertexBuffer_InitializeHandle( HANDLEINFO *HandleInfo ) ;															// ���_�o�b�t�@�n���h���̏�����
extern	int		Graphics_VertexBuffer_TerminateHandle( HANDLEINFO *HandleInfo ) ;															// ���_�o�b�t�@�n���h���̌�n��

extern	int		Graphics_IndexBuffer_Create( int IndexNum, int IndexType /* DX_INDEX_TYPE_16BIT �� */, int ASyncThread ) ;					// �C���f�b�N�X�o�b�t�@���쐬����
extern	int		Graphics_IndexBuffer_SetupHandle( int IndexBufHandle, int IndexNum, int IndexType /* DX_INDEX_TYPE_16BIT �� */ ) ;			// �C���f�b�N�X�o�b�t�@�n���h���̃Z�b�g�A�b�v���s��
extern	int		Graphics_IndexBuffer_InitializeHandle( HANDLEINFO *HandleInfo ) ;															// �C���f�b�N�X�o�b�t�@�n���h���̏�����
extern	int		Graphics_IndexBuffer_TerminateHandle( HANDLEINFO *HandleInfo ) ;															// �C���f�b�N�X�o�b�t�@�n���h���̌�n��








// �V���h�E�}�b�v�֌W�֐�
extern	int		Graphics_ShadowMap_MakeShadowMap_UseGParam( SETUP_SHADOWMAPHANDLE_GPARAM *GParam, int SizeX, int SizeY, int ASyncLoadFlag = FALSE, int ASyncThread = FALSE ) ;																																														// �V���h�E�}�b�v�n���h�����쐬����
extern	int		Graphics_ShadowMap_AddHandle( int ASyncThread ) ;																			// �V�����V���h�E�}�b�v�n���h�����m�ۂ���
extern	int		Graphics_ShadowMap_SetupHandle_UseGParam( SETUP_SHADOWMAPHANDLE_GPARAM *GParam, int SmHandle, int SizeX, int SizeY, int TexFormat_Float, int TexFormat_BitDepth, int ASyncThread ) ;	// �V���h�E�}�b�v�n���h���̃Z�b�g�A�b�v���s��
extern	int		Graphics_ShadowMap_CreateTexture( SHADOWMAPDATA *ShadowMap, int ASyncThread = FALSE ) ;										// �V���h�E�}�b�v�f�[�^�ɕK�v�ȃe�N�X�`�����쐬����
extern	int		Graphics_ShadowMap_ReleaseTexture( SHADOWMAPDATA *ShadowMap ) ;																// �V���h�E�}�b�v�f�[�^�ɕK�v�ȃe�N�X�`�����������
extern	SHADOWMAPDATA	*Graphics_ShadowMap_GetData( int SmHandle, int ASyncThread = FALSE ) ;												// �V���h�E�}�b�v�f�[�^���n���h���l������o��
extern	void	Graphics_ShadowMap_RefreshVSParam( void ) ;																					// ���_�V�F�[�_�[�ɐݒ肷��V���h�E�}�b�v�̏����X�V����
extern	void	Graphics_ShadowMap_RefreshPSParam( void ) ;																					// �s�N�Z���V�F�[�_�[�ɐݒ肷��V���h�E�}�b�v�̏����X�V����
extern	int		Graphics_ShadowMap_RefreshMatrix( SHADOWMAPDATA *ShadowMap ) ;							// �V���h�E�}�b�v�ւ̃����_�����O�Ŏg�p����s��̏����X�V����

extern	int		Graphics_ShadowMap_InitializeHandle( HANDLEINFO *HandleInfo ) ;																// �V���h�E�}�b�v�n���h���̏�����
extern	int		Graphics_ShadowMap_TerminateHandle( HANDLEINFO *HandleInfo ) ;																// �V���h�E�}�b�v�n���h���̌�n��








// �⏕�֌W�֐�
extern	int		Graphics_Other_AllocCommonBuffer( int Index, DWORD Size ) ;						// ���L�������̊m��
extern	int		Graphics_Other_TerminateCommonBuffer( void ) ;									// ���L�������̉��








// ���ˑ��������֌W
extern	int		Graphics_Initialize_Timing0_PF( void ) ;										// �`�揈���̊��ˑ������̏��������s���֐�( ���s�ӏ���ʂO )
extern	int		Graphics_Initialize_Timing1_PF( void ) ;										// �`�揈���̊��ˑ������̏��������s���֐�( ���s�ӏ���ʂQ )
extern	int		Graphics_Hardware_Initialize_PF( void ) ;										// �n�[�h�E�G�A�A�N�Z�����[�^���g�p����ꍇ�̊��ˑ��̏������������s��
extern	int		Graphics_Terminate_PF( void ) ;													// �`�揈���̊��ˑ������̌�n�����s���֐�
extern	int		Graphics_RestoreOrChangeSetupGraphSystem_PF( int Change, int ScreenSizeX = -1, int ScreenSizeY = -1, int ColorBitDepth = -1, int RefreshRate = -1 ) ;		// �O���t�B�b�N�X�V�X�e���̕��A�A���͕ύX�t���̍ăZ�b�g�A�b�v���s��
extern	int		Graphics_Hardware_CheckValid_PF( void ) ;										// �`��p�f�o�C�X���L�����ǂ������擾����( �߂�l  TRUE:�L��  FALSE:���� )







// ���ˑ��`��ݒ�֌W
extern	int		Graphics_Hardware_SetRenderTargetToShader_PF( int TargetIndex, int DrawScreen, int SurfaceIndex ) ;		// �V�F�[�_�[�`��ł̕`����ݒ肷��
extern	int		Graphics_Hardware_SetBackgroundColor_PF( int Red, int Green, int Blue ) ;								// ���C���E�C���h�E�̔w�i�F��ݒ肷��( Red,Green,Blue:���ꂼ�� �O�`�Q�T�T )
extern	int		Graphics_Hardware_SetDrawBrightToOneParam_PF( DWORD Bright ) ;											// SetDrawBright �̈��������
extern	int		Graphics_Hardware_SetDrawBlendMode_PF( int BlendMode, int BlendParam ) ;								// �`��u�����h���[�h���Z�b�g����
extern	int		Graphics_Hardware_SetDrawAlphaTest_PF( int TestMode, int TestParam ) ;									// �`�掞�̃A���t�@�e�X�g�̐ݒ���s��( TestMode:DX_CMP_GREATER��( -1:�f�t�H���g����ɖ߂� )  TestParam:�`��A���t�@�l�Ƃ̔�r�Ɏg�p����l )
extern	int		Graphics_Hardware_SetDrawMode_PF( int DrawMode ) ;														// �`�惂�[�h���Z�b�g����
extern	int		Graphics_Hardware_SetDrawBright_PF( int RedBright, int GreenBright, int BlueBright ) ;					// �`��P�x���Z�b�g
extern	int		Graphics_Hardware_SetBlendGraphParamBase_PF( IMAGEDATA *BlendImage, int BlendType, int *Param ) ;		// SetBlendGraphParam �̉ϒ������p�����[�^�t��
extern	int		Graphics_Hardware_SetMaxAnisotropy_PF( int MaxAnisotropy ) ;											// �ő�ٕ����̒l���Z�b�g����
extern	int		Graphics_Hardware_SetTransformToWorld_PF( const MATRIX *Matrix ) ;										// ���[���h�ϊ��p�s����Z�b�g����
extern	int		Graphics_Hardware_SetTransformToView_PF( const MATRIX *Matrix ) ;										// �r���[�ϊ��p�s����Z�b�g����
extern	int		Graphics_Hardware_SetTransformToProjection_PF( const MATRIX *Matrix ) ;									// ���e�ϊ��p�s����Z�b�g����
extern	int		Graphics_Hardware_SetTransformToViewport_PF( const MATRIX *Matrix ) ;									// �r���[�|�[�g�s����Z�b�g����
extern	int		Graphics_Hardware_SetTextureAddressMode_PF( int Mode /* DX_TEXADDRESS_WRAP �� */, int Stage ) ;			// �e�N�X�`���A�h���X���[�h��ݒ肷��
extern	int		Graphics_Hardware_SetTextureAddressModeUV_PF( int ModeU, int ModeV, int Stage ) ;						// �e�N�X�`���A�h���X���[�h��ݒ肷��
extern	int		Graphics_Hardware_SetTextureAddressTransformMatrix_PF( int UseFlag, MATRIX *Matrix, int Sampler = -1 ) ;// �e�N�X�`�����W�ϊ��s����Z�b�g����
extern	int		Graphics_Hardware_SetFogEnable_PF( int Flag ) ;															// �t�H�O��L���ɂ��邩�ǂ�����ݒ肷��( TRUE:�L��  FALSE:���� )
extern	int		Graphics_Hardware_SetFogMode_PF( int Mode /* DX_FOGMODE_NONE �� */ ) ;									// �t�H�O���[�h��ݒ肷��
extern	int		Graphics_Hardware_SetFogColor_PF( DWORD FogColor ) ;													// �t�H�O�J���[��ύX����
extern	int		Graphics_Hardware_SetFogStartEnd_PF( float start, float end ) ;											// �t�H�O���n�܂鋗���ƏI�����鋗����ݒ肷��( 0.0f �` 1.0f )
extern	int		Graphics_Hardware_SetFogDensity_PF( float density ) ;													// �t�H�O�̖��x��ݒ肷��( 0.0f �` 1.0f )
extern	int		Graphics_Hardware_DeviceDirect_SetWorldMatrix_PF( const MATRIX *Matrix ) ;								// ���[���h�ϊ��p�s����Z�b�g����
extern	int		Graphics_Hardware_DeviceDirect_SetViewMatrix_PF( const MATRIX *Matrix ) ;								// �r���[�ϊ��p�s����Z�b�g����
extern	int		Graphics_Hardware_DeviceDirect_SetProjectionMatrix_PF( const MATRIX *Matrix ) ;							// ���e�ϊ��p�s����Z�b�g����
extern	int		Graphics_Hardware_ApplyLigFogToHardware_PF( void ) ;													// ��{�f�[�^�ɐݒ肳��Ă���t�H�O�����n�[�h�E�F�A�ɔ��f����
extern	int		Graphics_Hardware_SetUseOldDrawModiGraphCodeFlag_PF( int Flag ) ;										// �ȑO�� DrawModiGraph �֐��̃R�[�h���g�p���邩�ǂ����̃t���O���Z�b�g����
extern	int		Graphics_Hardware_RefreshAlphaChDrawMode_PF( void ) ;													// �`���ɐ��������l���������ނ��ǂ����̃t���O���X�V����
//extern	void	Graphics_Hardware_SetUse2DProjectionMatrix_PF( int Use2DProjectionMatrix ) ;							// �g�p����ˉe�s����R�c�p�ɂ��邩�Q�c�p�ɂ��邩��ݒ肷��









// ���ˑ��ݒ�֌W
extern	int		Graphics_Hardware_SetUseHardwareVertexProcessing_PF( int Flag ) ;						// �n�[�h�E�G�A�̒��_���Z�����@�\���g�p���邩�ǂ�����ݒ肷��
extern	int		Graphics_Hardware_SetUsePixelLighting_PF( int Flag ) ;									// �s�N�Z���P�ʂŃ��C�e�B���O���s�����ǂ�����ݒ肷��A�v ShaderModel 3.0( TRUE:�s�N�Z���P�ʂ̃��C�e�B���O���s��  FALSE:���_�P�ʂ̃��C�e�B���O���s��( �f�t�H���g ) )
extern	int		Graphics_Hardware_SetGraphicsDeviceRestoreCallbackFunction_PF( void (* Callback )( void *Data ), void *CallbackData ) ;			// �O���t�B�b�N�X�f�o�C�X�����X�g���畜�A�����ۂɌĂ΂��R�[���o�b�N�֐���ݒ肷��
extern	int		Graphics_Hardware_SetGraphicsDeviceLostCallbackFunction_PF( void (* Callback )( void *Data ), void *CallbackData ) ;			// �O���t�B�b�N�X�f�o�C�X�����X�g���畜�A����O�ɌĂ΂��R�[���o�b�N�֐���ݒ肷��
extern	int		Graphics_Hardware_SetUseNormalDrawShader_PF( int Flag ) ;								// �ʏ�`��Ƀv���O���}�u���V�F�[�_�[���g�p���邩�ǂ�����ݒ肷��( TRUE:�g�p����( �f�t�H���g )  FALSE:�g�p���Ȃ� )
extern	int		Graphics_Hardware_GetVideoMemorySize_PF( int *AllSize, int *FreeSize ) ;				// �r�f�I�������̗e�ʂ𓾂�
extern	int		Graphics_SetAeroDisableFlag_PF( int Flag ) ;											// Vista�ȍ~�� Windows Aero �𖳌��ɂ��邩�ǂ������Z�b�g����ATRUE:�����ɂ���  FALSE:�L���ɂ���( DxLib_Init �̑O�ɌĂԕK�v������܂� )












// ���ˑ���ʊ֌W
extern	int		Graphics_Hardware_SetupUseZBuffer_PF( void ) ;															// �ݒ�Ɋ�Â��Ďg�p����y�o�b�t�@���Z�b�g����
extern	int		Graphics_Hardware_ClearDrawScreenZBuffer_PF( const RECT *ClearRect ) ;									// ��ʂ̂y�o�b�t�@�̏�Ԃ�����������
extern	int		Graphics_Hardware_ClearDrawScreen_PF( const RECT *ClearRect ) ;											// ��ʂ̏�Ԃ�����������
extern	int		Graphics_Hardware_SetDrawScreen_PF( int DrawScreen, int OldScreenSurface, IMAGEDATA *NewTargetImage, IMAGEDATA *OldTargetImage, SHADOWMAPDATA *NewTargetShadowMap, SHADOWMAPDATA *OldTargetShadowMap ) ;					// �`����ʂ̃Z�b�g
extern	int		Graphics_Hardware_SetDrawScreen_Post_PF( int DrawScreen ) ;												// SetDrawScreen �̍Ō�ŌĂ΂��֐�
extern	int		Graphics_Hardware_SetDrawArea_PF( int x1, int y1, int x2, int y2 ) ;									// �`��\�̈�̃Z�b�g
extern	int		Graphics_Hardware_LockDrawScreenBuffer_PF( RECT *LockRect, BASEIMAGE *BaseImage, int TargetScreen, IMAGEDATA *TargetImage, int TargetScreenSurface, int ReadOnly, int TargetScreenTextureNo ) ;	// �`���o�b�t�@�����b�N����
extern	int		Graphics_Hardware_UnlockDrawScreenBuffer_PF( void ) ;													// �`���o�b�t�@���A�����b�N����
extern	int		Graphics_Hardware_ScreenCopy_PF( int DrawTargetFrontScreenMode_Copy ) ;									// ����ʂ̓��e��\��ʂɕ`�悷��
extern	int		Graphics_SetupDisplayInfo_PF( void ) ;																	// �f�B�X�v���C�̏����Z�b�g�A�b�v����
extern	int		Graphics_Hardware_WaitVSync_PF( int SyncNum ) ;															// ���������M����҂�
extern	int		Graphics_ScreenFlipBase_PF( void ) ;																	// ����ʂƕ\��ʂ���������
#ifdef __WINDOWS__
extern	int		Graphics_BltRectBackScreenToWindow_PF( HWND Window, RECT BackScreenRect, RECT WindowClientRect ) ;		// ����ʂ̎w��̗̈���E�C���h�E�̃N���C�A���g�̈�̎w��̗̈�ɓ]������
extern	int		Graphics_SetScreenFlipTargetWindow_PF( HWND TargetWindow ) ;											// ScreenFlip �ŉ摜��]�������̃E�C���h�E��ݒ肷��( NULL ���w�肷��Ɛݒ���� )
#endif // __WINDOWS__
extern	int		Graphics_Hardware_SetZBufferMode_PF( int ZBufferSizeX, int ZBufferSizeY, int ZBufferBitDepth ) ;			// ���C����ʂ̂y�o�b�t�@�̐ݒ��ύX����
extern	int		Graphics_Hardware_SetDrawZBuffer_PF( int DrawScreen, IMAGEDATA *Image ) ;									// �`���y�o�b�t�@�̃Z�b�g
extern	int		Graphics_GetRefreshRate_PF( void ) ;																	// ���݂̉�ʂ̃��t���b�V�����[�g���擾����













// ���ˑ����擾�֌W
extern	const COLORDATA *	Graphics_Hardware_GetMainColorData_PF( void ) ;				// GetColor �� GetColor2 �Ŏg�p����J���[�f�[�^���擾����
extern	const COLORDATA *	Graphics_Hardware_GetDispColorData_PF( void ) ;				// �f�B�X�v���[�̃J���[�f�[�^�|�C���^�𓾂�
extern	DWORD				Graphics_Hardware_GetPixel_PF( int x, int y ) ;				// �w����W�̐F���擾����











// ���ˑ��摜�֌W
extern	int		Graphics_Hardware_UpdateGraphMovie_TheoraYUV_PF( struct MOVIEGRAPH *Movie, IMAGEDATA *Image ) ;							// YUV�T�[�t�F�X���g���� Theora ����̓��e���O���t�B�b�N�X�n���h���̃e�N�X�`���ɓ]������
extern	int		Graphics_Hardware_GraphLock_PF( IMAGEDATA *Image, COLORDATA **ColorDataP, int WriteOnly ) ;								// �O���t�B�b�N�������̈�̃��b�N
extern	int		Graphics_Hardware_GraphUnlock_PF( IMAGEDATA *Image ) ;																	// �O���t�B�b�N�������̈�̃��b�N����
extern	int		Graphics_Hardware_CopyGraphZBufferImage_PF( IMAGEDATA *DestImage, IMAGEDATA *SrcImage ) ;								// �O���t�B�b�N�̂y�o�b�t�@�̏�Ԃ�ʂ̃O���t�B�b�N�̂y�o�b�t�@�ɃR�s�[����( DestGrHandle �� SrcGrHandle ���y�o�b�t�@�������Ă���`��\�摜�ŁA���A���`�G�C���A�X�摜�ł͂Ȃ����Ƃ����� )
extern	int		Graphics_Hardware_InitGraph_PF( void ) ;																				// �摜�f�[�^�̏�����
extern	int		Graphics_Hardware_FillGraph_PF( IMAGEDATA *Image, int Red, int Green, int Blue, int Alpha, int ASyncThread ) ;			// �O���t�B�b�N�����̐F�œh��Ԃ�
extern	int		Graphics_Hardware_GetDrawScreenGraphBase_PF( IMAGEDATA *Image, IMAGEDATA *TargetImage, int TargetScreen, int TargetScreenSurface, int TargetScreenWidth, int TargetScreenHeight, int x1, int y1, int x2, int y2, int destX, int destY ) ;		// �`��\�摜��o�b�N�o�b�t�@����w��̈�̃O���t�B�b�N���擾����

// Graphics_Image_BltBmpOrBaseImageToGraph3 �̋@��ˑ������p�֐�
extern	int		Graphics_Hardware_BltBmpOrBaseImageToGraph3_PF(
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

// ��{�C���[�W�̃t�H�[�}�b�g�� DX_BASEIMAGE_FORMAT_NORMAL �ɕϊ�����K�v�����邩�ǂ������`�F�b�N����
// ( RequiredRgbBaseImageConvFlag �� RequiredAlphaBaseImageConvFlag �ɓ���l  TRUE:�ϊ�����K�v������  FALSE:�ϊ�����K�v�͖��� )
extern	int		Graphics_CheckRequiredNormalImageConv_BaseImageFormat_PF(
	IMAGEDATA_ORIG *Orig,
	int             RgbBaseImageFormat,
	int            *RequiredRgbBaseImageConvFlag,
	int             AlphaBaseImageFormat = -1,
	int            *RequiredAlphaBaseImageConvFlag = NULL
) ;

extern	int		Graphics_Hardware_CreateOrigTexture_PF(  IMAGEDATA_ORIG *Orig, int ASyncThread = FALSE ) ;			// �I���W�i���摜��񒆂̃e�N�X�`�����쐬����( 0:����  -1:���s )
extern	int		Graphics_Hardware_ReleaseOrigTexture_PF( IMAGEDATA_ORIG *Orig ) ;									// �I���W�i���摜��񒆂̃e�N�X�`�����������
extern	int		Graphics_Hardware_GetMultiSampleQuality_PF( int Samples ) ;											// �w��̃}���`�T���v�����Ŏg�p�ł���ő�N�I���e�B�l���擾����
extern	int		Graphics_Hardware_SetUsePlatformTextureFormat_PF( int PlatformTextureFormat ) ;						// �쐬����O���t�B�b�N�n���h���Ŏg�p������ˑ��̃e�N�X�`���t�H�[�}�b�g���w�肷��













// ���ˑ����_�o�b�t�@�E�C���f�b�N�X�o�b�t�@�֌W
extern	int		Graphics_Hardware_VertexBuffer_Create_PF(    VERTEXBUFFERHANDLEDATA *VertexBuffer ) ;															// ���_�o�b�t�@�n���h���̒��_�o�b�t�@���쐬����
extern	int		Graphics_Hardware_VertexBuffer_Terminate_PF( VERTEXBUFFERHANDLEDATA *VertexBuffer ) ;															// ���_�o�b�t�@�n���h���̌�n��
extern	int		Graphics_Hardware_VertexBuffer_SetData_PF(   VERTEXBUFFERHANDLEDATA *VertexBuffer, int SetIndex, const void *VertexData, int VertexNum ) ;		// ���_�o�b�t�@�ɒ��_�f�[�^��]������
extern	int		Graphics_Hardware_IndexBuffer_Create_PF(     INDEXBUFFERHANDLEDATA *IndexBuffer ) ;																// �C���f�b�N�X�o�b�t�@�n���h���̃Z�b�g�A�b�v���s��
extern	int		Graphics_Hardware_IndexBuffer_Terminate_PF(  INDEXBUFFERHANDLEDATA *IndexBuffer ) ;																// �C���f�b�N�X�o�b�t�@�n���h���̌�n��
extern	int		Graphics_Hardware_IndexBuffer_SetData_PF(    INDEXBUFFERHANDLEDATA *IndexBuffer, int SetIndex, const void *IndexData, int IndexNum ) ;			// �C���f�b�N�X�o�b�t�@�ɃC���f�b�N�X�f�[�^��]������











// ���ˑ����C�g�֌W
extern	int		Graphics_Hardware_Light_SetUse_PF( int Flag ) ;															// ���C�e�B���O���s�����ǂ�����ݒ肷��
extern	int		Graphics_Hardware_Light_GlobalAmbient_PF( COLOR_F *Color ) ;											// �O���[�o���A���r�G���g���C�g�J���[��ݒ肷��
extern	int		Graphics_Hardware_Light_SetState_PF( int LightNumber, LIGHTPARAM *LightParam ) ;						// ���C�g�p�����[�^���Z�b�g
extern	int		Graphics_Hardware_Light_SetEnable_PF( int LightNumber, int EnableState ) ;								// ���C�g�̗L���A������ύX












// ���ˑ��V���h�E�}�b�v�֌W
extern	int		Graphics_Hardware_ShadowMap_CreateTexture_PF( SHADOWMAPDATA *ShadowMap, int ASyncThread = FALSE ) ;		// �V���h�E�}�b�v�f�[�^�ɕK�v�ȃe�N�X�`�����쐬����
extern	int		Graphics_Hardware_ShadowMap_ReleaseTexture_PF( SHADOWMAPDATA *ShadowMap ) ;								// �V���h�E�}�b�v�f�[�^�ɕK�v�ȃe�N�X�`�����������
extern	void	Graphics_Hardware_ShadowMap_RefreshVSParam_PF( void ) ;													// ���_�V�F�[�_�[�ɐݒ肷��V���h�E�}�b�v�̏����X�V����
extern	void	Graphics_Hardware_ShadowMap_RefreshPSParam_PF( void ) ;													// �s�N�Z���V�F�[�_�[�ɐݒ肷��V���h�E�}�b�v�̏����X�V����
extern	int		Graphics_Hardware_ShadowMap_DrawSetup_PF( SHADOWMAPDATA *ShadowMap ) ;									// �V���h�E�}�b�v�ւ̕`��̏������s��
extern	int		Graphics_Hardware_ShadowMap_DrawEnd_PF( SHADOWMAPDATA *ShadowMap ) ;									// �V���h�E�}�b�v�ւ̕`����I������
extern	int		Graphics_Hardware_ShadowMap_SetUse_PF( int SlotIndex, SHADOWMAPDATA *ShadowMap ) ;						// �`��Ŏg�p����V���h�E�}�b�v���w�肷��A�X���b�g�͂O���P�����w��\�@










// ���ˑ��V�F�[�_�[�֌W
extern	int		Graphics_Hardware_Shader_Create_PF( int ShaderHandle, int IsVertexShader, void *Image, int ImageSize, int ImageAfterFree, int ASyncThread ) ;		// �V�F�[�_�[�n���h��������������
extern	int		Graphics_Hardware_Shader_TerminateHandle_PF( SHADERHANDLEDATA *Shader ) ;																			// �V�F�[�_�[�n���h���̌�n��
extern	int		Graphics_Hardware_Shader_GetValidShaderVersion_PF( void ) ;																							// �g�p�ł���V�F�[�_�[�̃o�[�W�������擾����( 0=�g���Ȃ�  200=�V�F�[�_�[���f���Q�D�O���g�p�\  300=�V�F�[�_�[���f���R�D�O���g�p�\ )
extern	int		Graphics_Hardware_Shader_GetConstIndex_PF( const wchar_t *ConstantName, SHADERHANDLEDATA *Shader ) ;													// �w��̖��O�����萔���g�p����V�F�[�_�[�萔�̔ԍ����擾����
extern	int		Graphics_Hardware_Shader_GetConstCount_PF( const wchar_t *ConstantName, SHADERHANDLEDATA *Shader ) ;													// �w��̖��O�����萔���g�p����V�F�[�_�[�萔�̐����擾����
extern	const FLOAT4 *Graphics_Hardware_Shader_GetConstDefaultParamF_PF( const wchar_t *ConstantName, SHADERHANDLEDATA *Shader ) ;									// �w��̖��O�������������_�萔�̃f�t�H���g�p�����[�^���i�[����Ă��郁�����A�h���X���擾����
extern	int		Graphics_Hardware_Shader_SetConst_PF(   int TypeIndex, int SetIndex, int ConstantIndex, const void *Param, int ParamNum, int UpdateUseArea ) ;		// �V�F�[�_�[�萔����ݒ肷��
extern	int		Graphics_Hardware_Shader_ResetConst_PF( int TypeIndex, int SetIndex, int ConstantIndex, int ParamNum ) ;											// �w��̈�̃V�F�[�_�[�萔�������Z�b�g����
#ifndef DX_NON_MODEL
extern	int		Graphics_Hardware_Shader_ModelCode_Init_PF( void ) ;																								// �R�c���f���p�̃V�F�[�_�[�R�[�h�̏��������s��
#endif // DX_NON_MODEL










// ���ˑ��V�F�[�_�[�p�萔�o�b�t�@�֌W
extern	int		Graphics_Hardware_ShaderConstantBuffer_Create_PF( int ShaderConstantBufferHandle, int BufferSize, int ASyncThread ) ;												// �V�F�[�_�[�p�萔�o�b�t�@�n���h��������������
extern	int		Graphics_Hardware_ShaderConstantBuffer_TerminateHandle_PF( SHADERCONSTANTBUFFERHANDLEDATA *ShaderConstantBuffer ) ;													// �V�F�[�_�[�p�萔�o�b�t�@�n���h���̌�n��
extern	void *	Graphics_Hardware_ShaderConstantBuffer_GetBuffer_PF( SHADERCONSTANTBUFFERHANDLEDATA *ShaderConstantBuffer ) ;														// �V�F�[�_�[�p�萔�o�b�t�@�n���h���̒萔�o�b�t�@�̃A�h���X���擾����
extern	int		Graphics_Hardware_ShaderConstantBuffer_Update_PF( SHADERCONSTANTBUFFERHANDLEDATA *ShaderConstantBuffer ) ;															// �V�F�[�_�[�p�萔�o�b�t�@�n���h���̒萔�o�b�t�@�ւ̕ύX��K�p����
extern	int		Graphics_Hardware_ShaderConstantBuffer_Set_PF( SHADERCONSTANTBUFFERHANDLEDATA *ShaderConstantBuffer, int TargetShader /* DX_SHADERTYPE_VERTEX �Ȃ� */, int Slot ) ;	// �V�F�[�_�[�p�萔�o�b�t�@�n���h���̒萔�o�b�t�@���w��̃V�F�[�_�[�̎w��̃X���b�g�ɃZ�b�g����











// ���ˑ��`��֌W
extern	int		Graphics_Hardware_RenderVertex( int ASyncThread = FALSE ) ;																							// ���_�o�b�t�@�ɗ��܂������_�f�[�^�������_�����O����

extern	int		Graphics_Hardware_DrawBillboard3D_PF(     VECTOR Pos, float cx, float cy, float Size, float Angle,                                    IMAGEDATA *Image, IMAGEDATA *BlendImage, int TransFlag, int TurnFlag, int DrawFlag = TRUE, RECT *DrawArea = NULL ) ;	// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawBillboard3D
extern	int		Graphics_Hardware_DrawModiBillboard3D_PF( VECTOR Pos, float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, IMAGEDATA *Image, IMAGEDATA *BlendImage, int TransFlag,               int DrawFlag = TRUE, RECT *DrawArea = NULL ) ;	// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawModiBillboard3D
extern	int		Graphics_Hardware_DrawGraph_PF(           int x,  int y, float xf, float yf,                                                          IMAGEDATA *Image, IMAGEDATA *BlendImage, int TransFlag, int IntFlag ) ;				// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawGraph
extern	int		Graphics_Hardware_DrawExtendGraph_PF(     int x1, int y1, int x2, int y2, float x1f, float y1f, float x2f, float y2f,                 IMAGEDATA *Image, IMAGEDATA *BlendImage, int TransFlag, int IntFlag ) ;				// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawExtendGraph
extern	int		Graphics_Hardware_DrawRotaGraph_PF(       int x,  int y, float xf, float yf, double ExRate, double Angle,                             IMAGEDATA *Image, IMAGEDATA *BlendImage, int TransFlag, int TurnFlag, int IntFlag ) ;	// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawRotaGraph
extern	int		Graphics_Hardware_DrawRotaGraphFast_PF(   int x,  int y, float xf, float yf, float  ExRate, float  Angle,                             IMAGEDATA *Image, IMAGEDATA *BlendImage, int TransFlag, int TurnFlag, int IntFlag ) ;	// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawRotaGraphFast
extern	int		Graphics_Hardware_DrawModiGraph_PF(       int   x1, int   y1, int   x2, int   y2, int   x3, int   y3, int   x4, int   y4,             IMAGEDATA *Image, IMAGEDATA *BlendImage, int TransFlag, bool SimpleDrawFlag ) ;		// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawModiGraph
extern	int		Graphics_Hardware_DrawModiGraphF_PF(      float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4,             IMAGEDATA *Image, IMAGEDATA *BlendImage, int TransFlag, bool SimpleDrawFlag ) ;		// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawModiGraphF
extern	int		Graphics_Hardware_DrawSimpleQuadrangleGraphF_PF( const GRAPHICS_DRAW_DRAWSIMPLEQUADRANGLEGRAPHF_PARAM *Param,						  IMAGEDATA *Image, IMAGEDATA *BlendImage ) ;											// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawSimpleQuadrangleGraphF
extern	int		Graphics_Hardware_DrawSimpleTriangleGraphF_PF(   const GRAPHICS_DRAW_DRAWSIMPLETRIANGLEGRAPHF_PARAM *Param,							  IMAGEDATA *Image, IMAGEDATA *BlendImage ) ;											// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawSimpleTriangleGraphF

extern	int		Graphics_Hardware_DrawFillBox_PF(          int x1, int y1, int x2, int y2,                                 unsigned int Color ) ;																// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawFillBox
extern	int		Graphics_Hardware_DrawLineBox_PF(          int x1, int y1, int x2, int y2,                                 unsigned int Color ) ;																// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawLineBox
extern	int		Graphics_Hardware_DrawLine_PF(             int x1, int y1, int x2, int y2,                                 unsigned int Color ) ;																// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawLine
extern	int		Graphics_Hardware_DrawLine3D_PF(           VECTOR Pos1, VECTOR Pos2,                                       unsigned int Color, int DrawFlag = TRUE, RECT *DrawArea = NULL ) ;					// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawLine3D
extern	int		Graphics_Hardware_DrawCircle_Thickness_PF( int x, int y, int r,                                            unsigned int Color, int Thickness ) ;												// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawCircle( �����w�肠�� )
extern	int		Graphics_Hardware_DrawOval_Thickness_PF(   int x, int y, int rx, int ry,                                   unsigned int Color, int Thickness ) ;												// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawOval( �����w�肠�� )
extern	int		Graphics_Hardware_DrawCircle_PF(           int x, int y, int r,                                            unsigned int Color, int FillFlag ) ;												// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawCircle
extern	int		Graphics_Hardware_DrawOval_PF(             int x, int y, int rx, int ry,                                   unsigned int Color, int FillFlag ) ;												// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawOval
extern	int		Graphics_Hardware_DrawTriangle_PF(         int x1, int y1, int x2, int y2, int x3, int y3,                 unsigned int Color, int FillFlag ) ;												// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawTriangle
extern	int		Graphics_Hardware_DrawTriangle3D_PF(       VECTOR Pos1, VECTOR Pos2, VECTOR Pos3,                          unsigned int Color, int FillFlag, int DrawFlag = TRUE, RECT *DrawArea = NULL ) ;	// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawTriangle3D
extern	int		Graphics_Hardware_DrawQuadrangle_PF(       int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, unsigned int Color, int FillFlag ) ;												// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawQuadrangle
extern	int		Graphics_Hardware_DrawPixel_PF(            int x,  int y,                                                  unsigned int Color ) ;																// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawPixel
extern	int		Graphics_Hardware_DrawPixel3D_PF(          VECTOR Pos,                                                     unsigned int Color, int DrawFlag = TRUE, RECT *DrawArea = NULL ) ;					// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawPixel3D
extern	int		Graphics_Hardware_DrawPixelSet_PF(         const POINTDATA *PointData, int Num ) ;																				// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawPixelSet
extern	int		Graphics_Hardware_DrawLineSet_PF(          const LINEDATA  *LineData,  int Num ) ;																				// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawLineSet

extern	int		Graphics_Hardware_DrawPrimitive_PF(                             const VERTEX_3D *Vertex, int VertexNum,                                    int PrimitiveType, IMAGEDATA *Image, int TransFlag ) ;
extern	int		Graphics_Hardware_DrawIndexedPrimitive_PF(                      const VERTEX_3D *Vertex, int VertexNum, const WORD *Indices, int IndexNum, int PrimitiveType, IMAGEDATA *Image, int TransFlag ) ;
extern	int		Graphics_Hardware_DrawPrimitiveLight_PF(                        const VERTEX3D  *Vertex, int VertexNum,                                    int PrimitiveType, IMAGEDATA *Image, int TransFlag ) ;
extern	int		Graphics_Hardware_DrawIndexedPrimitiveLight_PF(                 const VERTEX3D  *Vertex, int VertexNum, const WORD *Indices, int IndexNum, int PrimitiveType, IMAGEDATA *Image, int TransFlag ) ;
extern	int		Graphics_Hardware_DrawPrimitiveLight_UseVertexBuffer_PF(        VERTEXBUFFERHANDLEDATA *VertexBuffer,                                      int PrimitiveType,                 int StartVertex, int UseVertexNum, IMAGEDATA *Image, int TransFlag ) ;
extern	int		Graphics_Hardware_DrawIndexedPrimitiveLight_UseVertexBuffer_PF( VERTEXBUFFERHANDLEDATA *VertexBuffer, INDEXBUFFERHANDLEDATA *IndexBuffer,  int PrimitiveType, int BaseVertex, int StartVertex, int UseVertexNum, int StartIndex, int UseIndexNum, IMAGEDATA *Image, int TransFlag ) ;
extern	int		Graphics_Hardware_DrawPrimitive2D_PF(                                 VERTEX_2D *Vertex, int VertexNum,                                    int PrimitiveType, IMAGEDATA *Image, int TransFlag, int BillboardFlag, int Is3D, int TurnFlag, int TextureNo, int IsShadowMap ) ;
extern	int		Graphics_Hardware_DrawPrimitive2DUser_PF(                       const VERTEX2D  *Vertex, int VertexNum,                                    int PrimitiveType, IMAGEDATA *Image, int TransFlag, int Is3D, int TurnFlag, int TextureNo ) ;
extern	int		Graphics_Hardware_DrawIndexedPrimitive2DUser_PF(                const VERTEX2D  *Vertex, int VertexNum, const WORD *Indices, int IndexNum, int PrimitiveType, IMAGEDATA *Image, int TransFlag ) ;

extern	int		Graphics_Hardware_DrawPolygon3DToShader_PF(          const VERTEX3DSHADER *Vertex, int PolygonNum ) ;																										// �V�F�[�_�[���g���ĂR�c�|���S����`�悷��
extern	int		Graphics_Hardware_DrawPolygonIndexed3DToShader_PF(   const VERTEX3DSHADER *Vertex, int VertexNum, const unsigned short *Indices, int PolygonNum ) ;														// �V�F�[�_�[���g���ĂR�c�|���S����`�悷��( ���_�C���f�b�N�X���g�p���� )
extern	int		Graphics_Hardware_DrawPrimitive2DToShader_PF(        const VERTEX2DSHADER *Vertex, int VertexNum,                                              int PrimitiveType /* DX_PRIMTYPE_TRIANGLELIST �� */ ) ;		// �V�F�[�_�[���g���ĂQ�c�v���~�e�B�u��`�悷��
extern	int		Graphics_Hardware_DrawPrimitive3DToShader_PF(        const VERTEX3DSHADER *Vertex, int VertexNum,                                              int PrimitiveType /* DX_PRIMTYPE_TRIANGLELIST �� */ ) ;		// �V�F�[�_�[���g���ĂR�c�v���~�e�B�u��`�悷��
extern	int		Graphics_Hardware_DrawPrimitiveIndexed2DToShader_PF( const VERTEX2DSHADER *Vertex, int VertexNum, const unsigned short *Indices, int IndexNum, int PrimitiveType /* DX_PRIMTYPE_TRIANGLELIST �� */ ) ;		// �V�F�[�_�[���g���ĂQ�c�v���~�e�B�u��`�悷��( ���_�C���f�b�N�X���g�p���� )
extern	int		Graphics_Hardware_DrawPrimitiveIndexed3DToShader_PF( const VERTEX3DSHADER *Vertex, int VertexNum, const unsigned short *Indices, int IndexNum, int PrimitiveType /* DX_PRIMTYPE_TRIANGLELIST �� */ ) ;		// �V�F�[�_�[���g���ĂR�c�v���~�e�B�u��`�悷��( ���_�C���f�b�N�X���g�p���� )

extern	int		Graphics_Hardware_DrawPrimitive3DToShader_UseVertexBuffer2_PF(        int VertexBufHandle,                     int PrimitiveType /* DX_PRIMTYPE_TRIANGLELIST �� */, int StartVertex, int UseVertexNum ) ;	// �V�F�[�_�[���g���ĂR�c�v���~�e�B�u��`�悷��( ���_�o�b�t�@�g�p�� )
extern	int		Graphics_Hardware_DrawPrimitiveIndexed3DToShader_UseVertexBuffer2_PF( int VertexBufHandle, int IndexBufHandle, int PrimitiveType /* DX_PRIMTYPE_TRIANGLELIST �� */, int BaseVertex, int StartVertex, int UseVertexNum, int StartIndex, int UseIndexNum ) ;	// �V�F�[�_�[���g���ĂR�c�v���~�e�B�u��`�悷��( ���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@�g�p�� )

extern	int		Graphics_Hardware_Paint_PF( int x, int y, unsigned int FillColor, ULONGLONG BoundaryColor ) ;			// �w��_���狫�E�F������Ƃ���܂œh��Ԃ�











// wchar_t�Ŋ֐�
extern	int			LoadBmpToGraph_WCHAR_T(			const wchar_t *FileName, int TextureFlag, int ReverseFlag, int SurfaceMode = DX_MOVIESURFACE_NORMAL ) ;
extern	int			LoadGraph_WCHAR_T(				const wchar_t *FileName, int NotUse3DFlag = FALSE ) ;
extern	int			LoadReverseGraph_WCHAR_T(		const wchar_t *FileName, int NotUse3DFlag = FALSE ) ;
extern	int			LoadDivBmpToGraph_WCHAR_T(		const wchar_t *FileName, int AllNum, int XNum, int YNum, int IsFloat, int SizeXI, float SizeXF, int SizeYI, float SizeYF, int *HandleBuf, int TextureFlag, int ReverseFlag ) ;
extern	int			LoadBlendGraph_WCHAR_T(			const wchar_t *FileName ) ;
#ifdef __WINDOWS__
extern	int			LoadGraphToResource_WCHAR_T(	const wchar_t *ResourceName, const wchar_t *ResourceType ) ;
extern	int			LoadDivGraphToResource_WCHAR_T(	const wchar_t *ResourceName, const wchar_t *ResourceType, int AllNum, int XNum, int YNum, int XSize, int YSize, int *HandleBuf ) ;
extern	int			LoadDivGraphFToResource_WCHAR_T( const wchar_t *ResourceName, const wchar_t *ResourceType, int AllNum, int XNum, int YNum, float XSize, float YSize, int *HandleBuf ) ;
#endif // __WINDOWS__
extern	int			ReloadGraph_WCHAR_T(			const wchar_t *FileName, int GrHandle, int ReverseFlag = FALSE ) ;
extern	int			ReloadDivGraph_WCHAR_T(			const wchar_t *FileName, int AllNum, int XNum, int YNum, int IsFloat, int XSizeI, float XSizeF, int YSizeI, float YSizeF, const int *HandleBuf, int ReverseFlag = FALSE ) ;
extern	int			ReloadReverseGraph_WCHAR_T(		const wchar_t *FileName, int GrHandle ) ;

extern	int			GetGraphFilePath_WCHAR_T(		int GrHandle, wchar_t *FilePathBuffer ) ;

extern	int			LoadGraphScreen_WCHAR_T(        int x, int y, const wchar_t *GraphName, int TransFlag ) ;

extern	int			SaveDrawScreen_WCHAR_T(			int x1, int y1, int x2, int y2, const wchar_t *FileName, int SaveType = DX_IMAGESAVETYPE_BMP , int Jpeg_Quality = 80 , int Jpeg_Sample2x1 = TRUE , int Png_CompressionLevel = -1 ) ;
extern	int			SaveDrawScreenToBMP_WCHAR_T(	int x1, int y1, int x2, int y2, const wchar_t *FileName ) ;
extern	int			SaveDrawScreenToDDS_WCHAR_T(	int x1, int y1, int x2, int y2, const wchar_t *FileName ) ;
extern	int			SaveDrawScreenToJPEG_WCHAR_T(	int x1, int y1, int x2, int y2, const wchar_t *FileName, int Quality = 80 , int Sample2x1 = TRUE ) ;
extern	int			SaveDrawScreenToPNG_WCHAR_T(	int x1, int y1, int x2, int y2, const wchar_t *FileName, int CompressionLevel = -1 ) ;

extern	int			LoadVertexShader_WCHAR_T(		const wchar_t *FileName ) ;
extern	int			LoadPixelShader_WCHAR_T(		const wchar_t *FileName ) ;

extern	int			GetConstIndexToShader_WCHAR_T(           const wchar_t *ConstantName, int ShaderHandle ) ;
extern	int			GetConstCountToShader_WCHAR_T(           const wchar_t *ConstantName, int ShaderHandle ) ;
extern	const FLOAT4 *GetConstDefaultParamFToShader_WCHAR_T( const wchar_t *ConstantName, int ShaderHandle ) ;

extern	int			PlayMovie_WCHAR_T(						const wchar_t *FileName, int ExRate, int PlayType ) ;
extern	int			OpenMovieToGraph_WCHAR_T(				const wchar_t *FileName, int FullColor = TRUE ) ;




// �C�����C���֐��E�}�N��---------------------------------------------------------

// SetDrawBright �̈��������
#define Graphics_DrawSetting_SetDrawBrightToOneParam( /* DWORD */ Bright )\
{\
	DWORD lTempBright = ( DWORD )( Bright ) & 0xffffff ;\
	if( GSYS.DrawSetting.bDrawBright != lTempBright )\
	{\
		/* �P�x��ۑ� */\
		GSYS.DrawSetting.bDrawBright = lTempBright ;\
\
		/* �n�[�h�E�G�A�A�N�Z�����[�V�����̐ݒ�ɔ��f*/\
		if( GSYS.Setting.ValidHardware )\
		{\
			Graphics_Hardware_SetDrawBrightToOneParam_PF( lTempBright ) ;\
		}\
		else\
		{\
			SetMemImgDrawBright( lTempBright ) ;\
		}\
	}\
}



#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

#endif // DX_NON_GRAPHICS

#endif // __DXGRAPHICS_H__


