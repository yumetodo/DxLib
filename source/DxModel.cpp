// -------------------------------------------------------------------------------
// 
// 		�c�w���C�u����		���f���f�[�^����v���O����
// 
// 				Ver 3.16d
// 
// -------------------------------------------------------------------------------

#define __DX_MAKE
//#define DXLIB_DEBUG
//#define NOT_RENDER

#include "DxModel.h"

#ifndef DX_NON_MODEL

// �C���N���[�h ---------------------------------
#include "DxModelFile.h"
#include "DxModelRead.h"

#include "DxModelLoader3.h"
#include "DxFile.h"
#include "DxFont.h"
#include "DxLog.h"

#include "DxBaseFunc.h"
#include "DxMemory.h"
#include "DxSystem.h"
#include "DxMovie.h"
#include "DxMask.h"
#include "DxBaseImage.h"
#include "DxMath.h"
#include "DxGraphics.h"
#include "DxASyncLoad.h"

#ifdef __WINDOWS__
#include "Windows/DxModelWin.h"
#endif // __WINDOWS__

#ifdef __psp2__
#include "PSVita/DxModelPSVita.h"
#endif // __psp2__

#ifdef __ORBIS__
#include "PS4/DxModelPS4.h"
#endif // __ORBIS__

#ifdef __ANDROID__
#include "Android/DxModelAndroid.h"
#endif // __ANDROID__

#include <math.h>

#ifdef DXLIB_DEBUG
#include "DxShaderPath.h"
#endif

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// �}�N����` -----------------------------------


// ���f���f�[�^�����������֐��̖`�����ʕ�
#define MV1START( MODELHAND, MODEL, MODELBASE, ERR )\
	MV1_MODEL *MODEL ;\
	MV1_MODEL_BASE *MODELBASE ;\
\
	/* ����������Ă��Ȃ�������G���[ */\
	if( MV1Man.Initialize == false )\
		return ERR ;\
\
	/* �A�h���X�擾 */\
	if( MV1MDLCHK( MODELHAND, MODEL ) )\
		return ERR ;\
	MODELBASE = MODEL->BaseData ;



// �t���[���������֐��̖`�����ʕ�
#define MV1FRAMESTART( MODELHAND, MODEL, MODELBASE, FRAME, FRAMEIND, ERR )\
	MV1_MODEL *MODEL ;\
	MV1_MODEL_BASE *MODELBASE ;\
	MV1_FRAME *FRAME ;\
\
	/* ����������Ă��Ȃ�������G���[ */\
	if( MV1Man.Initialize == false )\
		return ERR ;\
\
	/* �A�h���X�擾 */\
	if( MV1MDLCHK( MODELHAND, MODEL ) )\
		return ERR ;\
	MODELBASE = MODEL->BaseData ;\
\
	/* �C���f�b�N�X���s����������G���[ */\
	if( FRAMEIND < 0 || FRAMEIND >= MODELBASE->FrameNum )\
		return ERR ;\
	FRAME = &MODEL->Frame[ FRAMEIND ] ;


// ���b�V���������֐��̖`�����ʕ�
#define MV1MESHSTART( MODELHAND, MODEL, MODELBASE, MESH, MESHIND, ERR )\
	MV1_MODEL *MODEL ;\
	MV1_MODEL_BASE *MODELBASE ;\
	MV1_MESH *MESH ;\
\
	/* ����������Ă��Ȃ�������G���[ */\
	if( MV1Man.Initialize == false )\
		return ERR ;\
\
	/* �A�h���X�擾 */\
	if( MV1MDLCHK( MODELHAND, MODEL ) )\
		return ERR ;\
	MODELBASE = MODEL->BaseData ;\
\
	/* �C���f�b�N�X���s����������G���[ */\
	if( MESHIND < 0 || MESHIND >= MODELBASE->MeshNum )\
		return ERR ;\
	MESH = &MODEL->Mesh[ MESHIND ] ;


// �V�F�C�v�������֐��̖`�����ʕ�
#define MV1SHAPESTART( MODELHAND, MODEL, MODELBASE, SHAPE, SHAPEIND, ERR )\
	MV1_MODEL *MODEL ;\
	MV1_MODEL_BASE *MODELBASE ;\
	MV1_SHAPE *SHAPE ;\
\
	/* ����������Ă��Ȃ�������G���[ */\
	if( MV1Man.Initialize == false )\
		return ERR ;\
\
	/* �A�h���X�擾 */\
	if( MV1MDLCHK( MODELHAND, MODEL ) )\
		return ERR ;\
	MODELBASE = MODEL->BaseData ;\
\
	/* �C���f�b�N�X���s����������G���[ */\
	if( SHAPEIND < 0 || SHAPEIND >= MODELBASE->ShapeNum )\
		return ERR ;\
	SHAPE = &MODEL->Shape[ SHAPEIND ] ;


// �g���C�A���O�����X�g�������֐��̖`�����ʕ�
#define MV1TLISTSTART( MODELHAND, MODEL, MODELBASE, TLIST, TLISTIND, ERR )\
	MV1_MODEL *MODEL ;\
	MV1_MODEL_BASE *MODELBASE ;\
	MV1_TRIANGLE_LIST_BASE *TLIST ;\
\
	/* ����������Ă��Ȃ�������G���[ */\
	if( MV1Man.Initialize == false )\
		return ERR ;\
\
	/* �A�h���X�擾 */\
	if( MV1MDLCHK( MODELHAND, MODEL ) )\
		return ERR ;\
	MODELBASE = MODEL->BaseData ;\
\
	/* �C���f�b�N�X���s����������G���[ */\
	if( TLISTIND < 0 || TLISTIND >= MODELBASE->TriangleNum )\
		return ERR ;\
	TLIST = &MODELBASE->TriangleList[ TLISTIND ] ;


// �}�e���A���������֐��̖`�����ʕ�
#define MV1MATERIALSTART( MODELHAND, MODEL, MODELBASE, MATERIAL, MATERIALIND, ERR )\
	MV1_MODEL *MODEL ;\
	MV1_MODEL_BASE *MODELBASE ;\
	MV1_MATERIAL *MATERIAL ;\
\
	/* ����������Ă��Ȃ�������G���[ */\
	if( MV1Man.Initialize == false )\
		return ERR ;\
\
	/* �A�h���X�擾 */\
	if( MV1MDLCHK( MODELHAND, MODEL ) )\
		return ERR ;\
	MODELBASE = MODEL->BaseData ;\
\
	/* �C���f�b�N�X���s����������G���[ */\
	if( MATERIALIND < 0 || MATERIALIND >= MODELBASE->MaterialNum )\
		return ERR ;\
	MATERIAL = &MODEL->Material[ MATERIALIND ] ;


// �e�N�X�`���������֐��̖`�����ʕ�
#define MV1TEXTURESTART( MODELHAND, MODEL, MODELBASE, TEXTURE, TEXTUREIND, ERR )\
	MV1_MODEL *MODEL ;\
	MV1_MODEL_BASE *MODELBASE ;\
	MV1_TEXTURE *TEXTURE ;\
\
	/* ����������Ă��Ȃ�������G���[ */\
	if( MV1Man.Initialize == false )\
		return ERR ;\
\
	/* �A�h���X�擾 */\
	if( MV1MDLCHK( MODELHAND, MODEL ) )\
		return ERR ;\
	MODELBASE = MODEL->BaseData ;\
\
	/* �C���f�b�N�X���s����������G���[ */\
	if( TEXTUREIND < 0 || TEXTUREIND >= MODELBASE->TextureNum )\
		return ERR ;\
	TEXTURE = &MODEL->Texture[ TEXTUREIND ] ;


// ��{�f�[�^�̃}�e���A���������֐��̖`�����ʕ�
#define MV1BASEMATERIALSTART( MODELBASEHAND, MODELBASE, MATERIAL, MATERIALIND, ERR )\
	MV1_MODEL_BASE *MODELBASE ;\
	MV1_MATERIAL_BASE *MATERIAL ;\
\
	/* ����������Ă��Ȃ�������G���[ */\
	if( MV1Man.Initialize == false )\
		return ERR ;\
\
	/* �A�h���X�擾 */\
	if( MV1BMDLCHK( MODELBASEHAND, MODELBASE ) )\
		return ERR ;\
\
	/* �C���f�b�N�X���s����������G���[ */\
	if( MATERIALIND < 0 || MATERIALIND >= MODELBASE->MaterialNum )\
		return ERR ;\
	MATERIAL = &MODELBASE->Material[ MATERIALIND ] ;



// ��{�f�[�^�̃e�N�X�`���������֐��̖`�����ʕ�
#define MV1BASETEXTURELSTART( MODELBASEHAND, MODELBASE, TEXTURE, TEXTUREIND, ERR )\
	MV1_MODEL_BASE *MODELBASE ;\
	MV1_TEXTURE_BASE *TEXTURE ;\
\
	/* ����������Ă��Ȃ�������G���[ */\
	if( MV1Man.Initialize == false )\
		return ERR ;\
\
	/* �A�h���X�擾 */\
	if( MV1BMDLCHK( MODELBASEHAND, MODELBASE ) )\
		return ERR ;\
\
	/* �C���f�b�N�X���s����������G���[ */\
	if( TEXTUREIND < 0 || TEXTUREIND >= MODELBASE->TextureNum )\
		return ERR ;\
	TEXTURE = &MODELBASE->Texture[ TEXTUREIND ] ;



// ��{�f�[�^�̃��b�V���������֐��̖`�����ʕ�
#define MV1BASEMESHSTART( MODELBASEHAND, MODELBASE, MESH, MESHIND, ERR )\
	MV1_MODEL_BASE *MODELBASE ;\
	MV1_MESH_BASE *MESH ;\
\
	/* ����������Ă��Ȃ�������G���[ */\
	if( MV1Man.Initialize == false )\
		return ERR ;\
\
	/* �A�h���X�擾 */\
	if( MV1BMDLCHK( MODELBASEHAND, MODELBASE ) )\
		return ERR ;\
\
	/* �C���f�b�N�X���s����������G���[ */\
	if( MESHIND < 0 || MESHIND >= MODELBASE->MeshNum )\
		return ERR ;\
	MESH = &MODELBASE->Mesh[ MESHIND ] ;



// ��{�f�[�^�̃V�F�C�v�������֐��̖`�����ʕ�
#define MV1BASESHAPESTART( MODELBASEHAND, MODELBASE, SHAPE, SHAPEIND, ERR )\
	MV1_MODEL_BASE *MODELBASE ;\
	MV1_SHAPE_BASE *SHAPE ;\
\
	/* ����������Ă��Ȃ�������G���[ */\
	if( MV1Man.Initialize == false )\
		return ERR ;\
\
	/* �A�h���X�擾 */\
	if( MV1BMDLCHK( MODELBASEHAND, MODELBASE ) )\
		return ERR ;\
\
	/* �C���f�b�N�X���s����������G���[ */\
	if( SHAPEIND < 0 || SHAPEIND >= MODELBASE->ShapeNum )\
		return ERR ;\
	SHAPE = &MODELBASE->Shape[ SHAPEIND ] ;


// ���b�V���̔������X�e�[�^�X�Z�b�g�A�b�v�����t���O�������Ă�����|��
#define MV1MESH_RESET_SEMITRANSSETUP( MESH )\
	if( (MESH)->SemiTransStateSetupFlag )\
	{\
		MV1_FRAME *Frame ;\
		(MESH)->SemiTransStateSetupFlag = false ;\
		for( Frame = MESH->Container ; Frame ; Frame = Frame->Parent )\
			Frame->SemiTransStateSetupFlag = false ;\
		Mesh->Container->Container->SemiTransStateSetupFlag = false ;\
	}


// �t���[���̔������X�e�[�^�X�Z�b�g�A�b�v�����t���O�������Ă�����|��
#define MV1FRAME_RESET_SEMITRANSSETUP( FRAME )\
	if( (FRAME)->SemiTransStateSetupFlag )\
	{\
		MV1_FRAME *_Frame ;\
		(FRAME)->SemiTransStateSetupFlag = false ;\
		for( _Frame = (FRAME)->Parent ; _Frame ; _Frame = _Frame->Parent )\
			_Frame->SemiTransStateSetupFlag = false ;\
		(FRAME)->Container->SemiTransStateSetupFlag = false ;\
	}



// �X�V���K�v���`�F�b�N( �K�v�Ȃ� 0 �ȊO )
#define MV1CCHK( CHANGE )				( *( CHANGE ).Target & ( CHANGE ).BaseData->CheckBit )

// �����̃r�b�g�����Z�b�g����
#define MV1CRST( CHANGE )				( *( CHANGE ).Target &= ~( CHANGE ).BaseData->CheckBit )

// ���f���̃��[�g�s����X�V����K�v������ꍇ�ɍX�V������
#define MV1SETUPMATRIX( MODEL )	\
	if( ( MODEL )->LocalWorldMatrixSetupFlag == false )\
		MV1SetupMatrix( ( MODEL ) ) ;

// ���b�V���̕`��}�e���A�������X�V����K�v������ꍇ�ɍX�V������
#define MV1SETUPDRAWMATERIALMESH( MESH )	\
	if( MV1CCHK( ( MESH )->DrawMaterialChange ) )	\
		MV1SetupDrawMaterial( NULL, ( MESH ) ) ;

// �t���[���̕`��}�e���A�������X�V����K�v������ꍇ�ɍX�V������
#define MV1SETUPDRAWMATERIALFRAME( FRAME )	\
	if( MV1CCHK( ( FRAME )->DrawMaterialChange ) )	\
		MV1SetupDrawMaterial( ( FRAME ), NULL ) ;

// �f�[�^�^�錾 ---------------------------------

// �f�[�^�錾 -----------------------------------

extern BYTE Tga256x8ToonTextureFileImage[ 172 ] ;
extern BYTE TgaSpecularDefaultGradFileImage[ 172 /*124*/ ] ;
extern BYTE TgaDiffuseDefaultGradFileImage[ 172 /*156*/ ] ;
extern BYTE Tga8x8TextureFileImage[ 84 ] ;
extern BYTE Tga8x8BlackTextureFileImage[ 76 ] ;




MV1_MODEL_MANAGE MV1Man;

// �֐��錾 -------------------------------------

static  int					_MV1GetAnimKeyDataIndexFromTime( MV1_ANIM_KEYSET_BASE *AnimKeySetBase, float Time, float &Rate ) ;		// �w��̎��Ԃ𒴂����ԏ������ԍ��̃L�[�̃C���f�b�N�X���擾����
static	int					_MV1AnimSetSyncNowKey( MV1_ANIMSET *AnimSet, bool AboutSetting = false ) ;						// ���݂̃A�j���[�V�����Đ����ԂɊe�L�[�� NowKey �̒l�����킹��
static	MV1_ANIMSET_BASE	*MV1GetAnimSetBase( int MV1ModelHandle, const wchar_t *Name = NULL, int Index = -1 ) ;				// ���O��C���f�b�N�X���烂�f����{�f�[�^���̃A�j���[�V�������擾����
static	MV1_ANIMSET			*MV1CreateAnimSet( MV1_ANIMSET_BASE *MV1AnimSetBase ) ;											// �A�j���[�V�����Z�b�g��{�f�[�^������s�p�A�j���[�V�����Z�b�g���쐬����
static	MV1_ANIM			*MV1GetAnimSetAnim( MV1_ANIMSET *AnimSet, const wchar_t *Name = NULL, int Index = -1 ) ;			// �A�j���[�V�����Z�b�g�������̖��O�A�Ⴕ���̓C���f�b�N�X�̃A�j���[�V�����𓾂�
//static	int					MV1PlayAnimSet( MV1_ANIMSET *AnimSet, int Loop ) ;											// �A�j���[�V�������Đ�����
static	int					MV1SetAnimSetTime( MV1_ANIMSET *AnimSet, float Time ) ;											// �A�j���[�V�������w��̎��Ԃɐݒ肷��

//static	int					MV1AnimSetAddTime( MV1_ANIMSET *AnimSet, float AddTime ) ;									// �A�j���[�V������i�߂�
//static	int					MV1StopAnimSet( MV1_ANIMSET *AnimSet ) ;													// �A�j���[�V�������~�߂�
//static	int					MV1GetAnimSetState( MV1_ANIMSET *AnimSet ) ;												// �A�j���[�V�������Đ������ǂ������擾����( TRUE:�Đ���  FALSE:��~�� )
static	int					MV1AnimSetSetupParam( MV1_ANIMSET *AnimSet ) ;													// �A�j���[�V�����̌��݂̍Đ��o�ߎ��Ԃɍ��킹���p�����[�^���v�Z����
static	void				MV1SetupTransformMatrix( MATRIX_4X4CT_F * RST BlendMatrix, int ValidFlag, VECTOR * RST Translate, VECTOR * RST Scale, int RotateOrder, VECTOR * RST PreRotate, VECTOR * RST Rotate, VECTOR * RST PostRotate, FLOAT4 * RST Quaternion ) ;	// ���W�ϊ������g�p���č��W�ϊ��s����쐬����
static	int					MV1SetupReferenceMeshFrame( MV1_MODEL *Model, MV1_MODEL_BASE *ModelBase, MV1_FRAME *Frame, MV1_REF_POLYGONLIST *DestBuffer, int VIndexTarget, bool IsTransform, bool IsPositionOnly ) ;	// �Q�Ɨp���b�V���̃Z�b�g�A�b�v���s��
static	int					MV1RefreshReferenceMeshFrame( MV1_FRAME *Frame, int IsPositionOnly, MV1_REF_POLYGONLIST *DestBuffer ) ;	// �Q�Ɨp���b�V���̃��t���b�V�����s��
static	void				MV1SetupAnimMatrix( MV1_MODEL *Model ) ;														// �A�j���[�V�����̍s����Z�b�g�A�b�v����
static	void				MV1SetupMatrix( MV1_MODEL *Model ) ;															// �`��p�̍s����\�z����
static	void				MV1SetupDrawMaterial( MV1_FRAME *Frame = NULL, MV1_MESH *Mesh = NULL ) ;						// �`��p�̃}�e���A�������\�z����AFrame ���w�肵���ꍇ�� Mesh �� NULL�ŁAMesh ���w�肵���ꍇ�� Frame �� NULL�ł���K�v������܂�( ���œn���ꂽ���b�V�����̂̍X�V�`�F�b�N�͂��Ȃ� )
static	void				MV1BitSetChange( MV1_CHANGE *Change ) ;															// ��ԕύX�Ǘ��f�[�^�ɐݒ肳��Ă���Ώۃr�b�g�𗧂Ă�
static	void				MV1BitResetChange( MV1_CHANGE *Change ) ;														// ��ԕύX�Ǘ��f�[�^�ɐݒ肳��Ă���Ώۃr�b�g��|��
static	bool				MV1SetDrawMaterialDif( MV1_DRAW_MATERIAL *DrawMaterial, MV1_CHANGE *Change, COLOR_F Scale ) ;	// �`��p�}�e���A�����̃f�B�t���[�Y�J���[��ύX����
static	bool				MV1SetDrawMaterialAmb( MV1_DRAW_MATERIAL *DrawMaterial, MV1_CHANGE *Change, COLOR_F Scale ) ;	// �`��p�}�e���A�����̃A���r�G���g�J���[��ύX����
static	bool				MV1SetDrawMaterialSpc( MV1_DRAW_MATERIAL *DrawMaterial, MV1_CHANGE *Change, COLOR_F Scale ) ;	// �`��p�}�e���A�����̃X�y�L�����J���[��ύX����
static	bool				MV1SetDrawMaterialEmi( MV1_DRAW_MATERIAL *DrawMaterial, MV1_CHANGE *Change, COLOR_F Scale ) ;	// �`��p�}�e���A�����̃G�~�b�V�u�J���[��ύX����
static	bool				MV1SetDrawMaterialOpacityRate( MV1_DRAW_MATERIAL *DrawMaterial, MV1_CHANGE *Change, float Rate) ;	// �`��p�}�e���A�����̕s�����x��ύX����
static	bool				MV1SetDrawMaterialVisible( MV1_DRAW_MATERIAL *DrawMaterial, MV1_CHANGE *Change, BYTE Visible ) ;	// �`��p�}�e���A�����̕\���ݒ��ύX����
static	int					MV1DrawModelBase( MV1_MODEL *Model ) ;															// ���f���̕`�揈�����s��
static	int					_MV1ReCreateGradationGraph( int GrHandle ) ;													// �O���f�[�V�����摜���č쐬����
static	int					_MV1ReMakeNormalFrameBase( MV1_FRAME_BASE *Frame, float SmoothingAngle ) ;						// �t���[���̖@�����Čv�Z����
static	int					_MV1PositionOptimizeFrameBase( MV1_FRAME_BASE *Frame ) ;										// �t���[���̍��W�����œK������
static	int					_MV1SetupCombineMesh( MV1_FRAME_BASE *Frame ) ;													// �R���o�C�����b�V���̃Z�b�g�A�b�v���s��
static	void				_MV1SetupReferenceMeshMaxAndMinPosition( MV1_REF_POLYGONLIST *PolyList ) ;						// �Q�Ɨp���b�V���̍ő�l�ƍŏ��l���X�V����
static	bool				_MV1CreateWideCharNameBase( MV1_MODEL_BASE *MBase, const char *NameA, wchar_t **NameWP ) ;		// �}���`�o�C�g���������烏�C�h���������쐬����
static	bool				_MV1CreateMultiByteNameBase( MV1_MODEL_BASE *MBase, const wchar_t *NameW, char **NameAP ) ;		// ���C�h����������}���`�o�C�g���������쐬����
static	bool				_MV1CreateWideCharName( const char *NameA, wchar_t **NameWP ) ;									// �}���`�o�C�g���������烏�C�h���������쐬����
static	bool				_MV1CreateMultiByteName( const wchar_t *NameW, char **NameAP ) ;								// ���C�h����������}���`�o�C�g���������쐬����
static	bool				_MV1AllocAndMultiByteNameCopy( const char *NameA, char **NameAP ) ;								// �}���`�o�C�g��������V���Ƀ��������m�ۂ��ăR�s�[����
static	bool				_MV1AllocAndWideCharNameCopy( const wchar_t *NameW, wchar_t **NameWP ) ;						// ���C�h��������V���Ƀ��������m�ۂ��ăR�s�[����
static	void				_MV1CreateFileNameAndCurrentDirectory( const wchar_t *FilePath, wchar_t *FileName, size_t FileNameBytes, wchar_t *CurrentDirectory, size_t CurrentDirectoryBytes ) ;	// �t�@�C���p�X����t�@�C�����ƃf�B���N�g���p�X���쐬����

// �e�N�X�`����ǂݍ���

static	int					_MV1TextureLoadBase(
									MV1_MODEL_BASE *ModelBase, MV1_TEXTURE_BASE *Texture,
									const wchar_t *ColorFilePathW, const wchar_t *AlphaFilePathW,
									int BumpImageFlag, float BumpImageNextPixelLength,
									bool ReverseFlag,
									bool Bmp32AllZeroAlphaToXRGB8Flag,
									int ASyncThread ) ;
static	int					_MV1TextureLoad(
									MV1_MODEL_BASE *ModelBase, MV1_TEXTURE *Texture,
									const wchar_t *ColorFilePathW, const wchar_t *AlphaFilePathW,
									int BumpImageFlag, float BumpImageNextPixelLength,
									bool ReverseFlag,
									bool Bmp32AllZeroAlphaToXRGB8Flag,
									int ASyncThread ) ;



// �v���O���� -----------------------------------

static __inline int DrawMeshListResize( int RequestSize )
{
	if( RequestSize > MV1Man.DrawMeshListSize )
	{
		if( MV1Man.DrawMeshList )
		{
			DXFREE( MV1Man.DrawMeshList ) ;
		}
		MV1Man.DrawMeshList = ( MV1_MESH ** )DXALLOC( RequestSize * 3 * sizeof( MV1_MESH * ) ) ;
		if( MV1Man.DrawMeshList == NULL ) return -1 ;
		MV1Man.DrawMeshListSize = RequestSize ;
	}
	return 0 ;
}

// �N�H�[�^�j�I�������]�s����쐬����
static __inline void CreateQuaternionRotateMatrix4X4CTD( MATRIX_4X4CT_D * RST Out, FLOAT4 * RST In )
{
	MATRIX_D RotateQ1Mat, RotateQ2Mat, RotateMat ;

	RotateQ1Mat.m[0][0] =  In->w ; RotateQ1Mat.m[0][1] = -In->z ; RotateQ1Mat.m[0][2] =  In->y ; RotateQ1Mat.m[0][3] =  In->x ;
	RotateQ1Mat.m[1][0] =  In->z ; RotateQ1Mat.m[1][1] =  In->w ; RotateQ1Mat.m[1][2] = -In->x ; RotateQ1Mat.m[1][3] =  In->y ;
	RotateQ1Mat.m[2][0] = -In->y ; RotateQ1Mat.m[2][1] =  In->x ; RotateQ1Mat.m[2][2] =  In->w ; RotateQ1Mat.m[2][3] =  In->z ;
	RotateQ1Mat.m[3][0] = -In->x ; RotateQ1Mat.m[3][1] = -In->y ; RotateQ1Mat.m[3][2] = -In->z ; RotateQ1Mat.m[3][3] =  In->w ;

	RotateQ2Mat.m[0][0] =  In->w ; RotateQ2Mat.m[0][1] = -In->z ; RotateQ2Mat.m[0][2] =  In->y ; RotateQ2Mat.m[0][3] = -In->x ;
	RotateQ2Mat.m[1][0] =  In->z ; RotateQ2Mat.m[1][1] =  In->w ; RotateQ2Mat.m[1][2] = -In->x ; RotateQ2Mat.m[1][3] = -In->y ;
	RotateQ2Mat.m[2][0] = -In->y ; RotateQ2Mat.m[2][1] =  In->x ; RotateQ2Mat.m[2][2] =  In->w ; RotateQ2Mat.m[2][3] = -In->z ;
	RotateQ2Mat.m[3][0] =  In->x ; RotateQ2Mat.m[3][1] =  In->y ; RotateQ2Mat.m[3][2] =  In->z ; RotateQ2Mat.m[3][3] =  In->w ;

	CreateMultiplyMatrixD( &RotateMat, &RotateQ2Mat, &RotateQ1Mat ) ;
	ConvertMatrixDToMatrix4x4cD( Out, &RotateMat ) ;
}

static __inline void CreateQuaternionRotateMatrix4X4CTF( MATRIX_4X4CT_F * RST Out, FLOAT4 * RST In )
{
	MATRIX RotateQ1Mat, RotateQ2Mat, RotateMat ;

	RotateQ1Mat.m[0][0] =  In->w ; RotateQ1Mat.m[0][1] = -In->z ; RotateQ1Mat.m[0][2] =  In->y ; RotateQ1Mat.m[0][3] =  In->x ;
	RotateQ1Mat.m[1][0] =  In->z ; RotateQ1Mat.m[1][1] =  In->w ; RotateQ1Mat.m[1][2] = -In->x ; RotateQ1Mat.m[1][3] =  In->y ;
	RotateQ1Mat.m[2][0] = -In->y ; RotateQ1Mat.m[2][1] =  In->x ; RotateQ1Mat.m[2][2] =  In->w ; RotateQ1Mat.m[2][3] =  In->z ;
	RotateQ1Mat.m[3][0] = -In->x ; RotateQ1Mat.m[3][1] = -In->y ; RotateQ1Mat.m[3][2] = -In->z ; RotateQ1Mat.m[3][3] =  In->w ;

	RotateQ2Mat.m[0][0] =  In->w ; RotateQ2Mat.m[0][1] = -In->z ; RotateQ2Mat.m[0][2] =  In->y ; RotateQ2Mat.m[0][3] = -In->x ;
	RotateQ2Mat.m[1][0] =  In->z ; RotateQ2Mat.m[1][1] =  In->w ; RotateQ2Mat.m[1][2] = -In->x ; RotateQ2Mat.m[1][3] = -In->y ;
	RotateQ2Mat.m[2][0] = -In->y ; RotateQ2Mat.m[2][1] =  In->x ; RotateQ2Mat.m[2][2] =  In->w ; RotateQ2Mat.m[2][3] = -In->z ;
	RotateQ2Mat.m[3][0] =  In->x ; RotateQ2Mat.m[3][1] =  In->y ; RotateQ2Mat.m[3][2] =  In->z ; RotateQ2Mat.m[3][3] =  In->w ;

	CreateMultiplyMatrix( &RotateMat, &RotateQ2Mat, &RotateQ1Mat ) ;
	ConvertMatrixFToMatrix4x4cF( Out, &RotateMat ) ;
}

static __inline void CreateQuaternionRotateMatrix4X4CT( MATRIX_4X4CT * RST Out, FLOAT4 * RST In )
{
	if( GSYS.DrawSetting.Large3DPositionSupport )
	{
		CreateQuaternionRotateMatrix4X4CTD( &Out->md, In ) ;
	}
	else
	{
		CreateQuaternionRotateMatrix4X4CTF( &Out->mf, In ) ;
	}
}

// �P�ʍs����쐬����
static __inline void CreateIdentityMatrix4X4CTF( MATRIX_4X4CT_F * RST Out )
{
	Out->m[ 0 ][ 0 ] = 1.0f ;
	Out->m[ 0 ][ 1 ] = 0.0f ;
	Out->m[ 0 ][ 2 ] = 0.0f ;
	Out->m[ 0 ][ 3 ] = 0.0f ;

	Out->m[ 1 ][ 0 ] = 0.0f ;
	Out->m[ 1 ][ 1 ] = 1.0f ;
	Out->m[ 1 ][ 2 ] = 0.0f ;
	Out->m[ 1 ][ 3 ] = 0.0f ;

	Out->m[ 2 ][ 0 ] = 0.0f ;
	Out->m[ 2 ][ 1 ] = 0.0f ;
	Out->m[ 2 ][ 2 ] = 1.0f ;
	Out->m[ 2 ][ 3 ] = 0.0f ;
}

static __inline void CreateIdentityMatrix4X4CTD( MATRIX_4X4CT_D * RST Out )
{
	Out->m[ 0 ][ 0 ] = 1.0 ;
	Out->m[ 0 ][ 1 ] = 0.0 ;
	Out->m[ 0 ][ 2 ] = 0.0 ;
	Out->m[ 0 ][ 3 ] = 0.0 ;

	Out->m[ 1 ][ 0 ] = 0.0 ;
	Out->m[ 1 ][ 1 ] = 1.0 ;
	Out->m[ 1 ][ 2 ] = 0.0 ;
	Out->m[ 1 ][ 3 ] = 0.0 ;

	Out->m[ 2 ][ 0 ] = 0.0 ;
	Out->m[ 2 ][ 1 ] = 0.0 ;
	Out->m[ 2 ][ 2 ] = 1.0 ;
	Out->m[ 2 ][ 3 ] = 0.0 ;
}

static __inline void CreateIdentityMatrix4X4CTD( MATRIX_4X4CT * RST Out )
{
	if( GSYS.DrawSetting.Large3DPositionSupport )
	{
		CreateIdentityMatrix4X4CTD( &Out->md ) ;
	}
	else
	{
		CreateIdentityMatrix4X4CTF( &Out->mf ) ;
	}
}

// ���s�ړ��s����쐬����
static __inline void CreateTranslationMatrix4X4CTF( MATRIX_4X4CT_F *Out, float x, float y, float z )
{
	_MEMSET( Out, 0, sizeof( *Out ) ) ;
	Out->m[ 0 ][ 3 ] = x ;
	Out->m[ 1 ][ 3 ] = y ;
	Out->m[ 2 ][ 3 ] = z ;
	Out->m[ 0 ][ 0 ] = 1.0f ;
	Out->m[ 1 ][ 1 ] = 1.0f ;
	Out->m[ 2 ][ 2 ] = 1.0f ;
}

static __inline void CreateTranslationMatrix4X4CTD( MATRIX_4X4CT_D *Out, double x, double y, double z )
{
	_MEMSET( Out, 0, sizeof( *Out ) ) ;
	Out->m[ 0 ][ 3 ] = x ;
	Out->m[ 1 ][ 3 ] = y ;
	Out->m[ 2 ][ 3 ] = z ;
	Out->m[ 0 ][ 0 ] = 1.0 ;
	Out->m[ 1 ][ 1 ] = 1.0 ;
	Out->m[ 2 ][ 2 ] = 1.0 ;
}

static __inline void CreateTranslationMatrix4X4CT( MATRIX_4X4CT *Out, double x, double y, double z )
{
	if( GSYS.DrawSetting.Large3DPositionSupport )
	{
		CreateTranslationMatrix4X4CTD( &Out->md, x, y, z ) ;
	}
	else
	{
		CreateTranslationMatrix4X4CTF( &Out->mf, ( float )x, ( float )y, ( float )z ) ;
	}
}

// �X�P�[�����O�s����쐬����
static __inline void CreateScalingMatrix4X4CTF( MATRIX_4X4CT_F *Out, float sx, float sy, float sz )
{
	_MEMSET( Out, 0, sizeof( *Out ) ) ;

	Out->m[ 0 ][ 0 ] = sx ;
	Out->m[ 1 ][ 1 ] = sy ;
	Out->m[ 2 ][ 2 ] = sz ;
}

static __inline void CreateScalingMatrix4X4CTD( MATRIX_4X4CT_D *Out, double sx, double sy, double sz )
{
	_MEMSET( Out, 0, sizeof( *Out ) ) ;

	Out->m[ 0 ][ 0 ] = sx ;
	Out->m[ 1 ][ 1 ] = sy ;
	Out->m[ 2 ][ 2 ] = sz ;
}

static __inline void CreateScalingMatrix4X4CT( MATRIX_4X4CT *Out, double sx, double sy, double sz )
{
	if( GSYS.DrawSetting.Large3DPositionSupport )
	{
		CreateScalingMatrix4X4CTD( &Out->md, sx, sy, sz ) ;
	}
	else
	{
		CreateScalingMatrix4X4CTF( &Out->mf, ( float )sx, ( float )sy, ( float )sz ) ;
	}
}

// �w����]���x����]���y����]�����������s����쐬����
static __inline void CreateRotationXYZMatrix4X4CTF( MATRIX_4X4CT_F * RST Out, float XRot, float YRot, float ZRot )
{
	float SinX, CosX, SinY, CosY, SinZ, CosZ ;
	float SinXSinY, SinYCosX ;

	_TABLE_SINCOS( XRot, &SinX, &CosX ) ;
	_TABLE_SINCOS( YRot, &SinY, &CosY ) ;
	_TABLE_SINCOS( ZRot, &SinZ, &CosZ ) ;

	SinXSinY = SinX * SinY ;
	SinYCosX = SinY * CosX ;

	Out->m[ 0 ][ 0 ] = CosY * CosZ ;
	Out->m[ 0 ][ 1 ] = SinXSinY * CosZ - CosX * SinZ ;
	Out->m[ 0 ][ 2 ] = SinYCosX * CosZ + SinX * SinZ ;
	Out->m[ 0 ][ 3 ] = 0.0f ;
	Out->m[ 1 ][ 0 ] = CosY * SinZ ;
	Out->m[ 1 ][ 1 ] = SinXSinY * SinZ + CosX * CosZ ;
	Out->m[ 1 ][ 2 ] = SinYCosX * SinZ - SinX * CosZ ;
	Out->m[ 1 ][ 3 ] = 0.0f ;
	Out->m[ 2 ][ 0 ] = -SinY ;
	Out->m[ 2 ][ 1 ] = SinX * CosY ;
	Out->m[ 2 ][ 2 ] = CosX * CosY ;
	Out->m[ 2 ][ 3 ] = 0.0f ;
}

static __inline void CreateRotationXYZMatrix4X4CTD( MATRIX_4X4CT_D * RST Out, float XRot, float YRot, float ZRot )
{
	float SinX, CosX, SinY, CosY, SinZ, CosZ ;
	float SinXSinY, SinYCosX ;

	_TABLE_SINCOS( XRot, &SinX, &CosX ) ;
	_TABLE_SINCOS( YRot, &SinY, &CosY ) ;
	_TABLE_SINCOS( ZRot, &SinZ, &CosZ ) ;

	SinXSinY = SinX * SinY ;
	SinYCosX = SinY * CosX ;

	Out->m[ 0 ][ 0 ] = CosY * CosZ ;
	Out->m[ 0 ][ 1 ] = SinXSinY * CosZ - CosX * SinZ ;
	Out->m[ 0 ][ 2 ] = SinYCosX * CosZ + SinX * SinZ ;
	Out->m[ 0 ][ 3 ] = 0.0 ;
	Out->m[ 1 ][ 0 ] = CosY * SinZ ;
	Out->m[ 1 ][ 1 ] = SinXSinY * SinZ + CosX * CosZ ;
	Out->m[ 1 ][ 2 ] = SinYCosX * SinZ - SinX * CosZ ;
	Out->m[ 1 ][ 3 ] = 0.0 ;
	Out->m[ 2 ][ 0 ] = -SinY ;
	Out->m[ 2 ][ 1 ] = SinX * CosY ;
	Out->m[ 2 ][ 2 ] = CosX * CosY ;
	Out->m[ 2 ][ 3 ] = 0.0 ;
}

static __inline void CreateRotationXYZMatrix4X4CT( MATRIX_4X4CT * RST Out, float XRot, float YRot, float ZRot )
{
	if( GSYS.DrawSetting.Large3DPositionSupport )
	{
		CreateRotationXYZMatrix4X4CTD( &Out->md, XRot, YRot, ZRot ) ;
	}
	else
	{
		CreateRotationXYZMatrix4X4CTF( &Out->mf, XRot, YRot, ZRot ) ;
	}
}

// �w����]���y����]���x����]�����������s����쐬����
static __inline void CreateRotationXZYMatrix4X4CTF( MATRIX_4X4CT_F * RST Out, float XRot, float YRot, float ZRot )
{
	float SinX, CosX, SinY, CosY, SinZ, CosZ ;
	float CosXSinZ, SinXSinZ ;

	_TABLE_SINCOS( XRot, &SinX, &CosX ) ;
	_TABLE_SINCOS( YRot, &SinY, &CosY ) ;
	_TABLE_SINCOS( ZRot, &SinZ, &CosZ ) ;

	CosXSinZ = CosX * SinZ ;
	SinXSinZ = SinX * SinZ ;

	Out->m[ 0 ][ 0 ] = CosY * CosZ ;
	Out->m[ 0 ][ 1 ] = CosXSinZ * -CosY + SinX * SinY ;
	Out->m[ 0 ][ 2 ] = SinXSinZ *  CosY + CosX * SinY ;
	Out->m[ 0 ][ 3 ] = 0.0f ;
	Out->m[ 1 ][ 0 ] = SinZ ;
	Out->m[ 1 ][ 1 ] = CosX * CosZ ;
	Out->m[ 1 ][ 2 ] = -SinX * CosZ ;
	Out->m[ 1 ][ 3 ] = 0.0f ;
	Out->m[ 2 ][ 0 ] = CosZ * -SinY ;
	Out->m[ 2 ][ 1 ] = CosXSinZ *  SinY + SinX * CosY ;
	Out->m[ 2 ][ 2 ] = SinXSinZ * -SinY + CosX * CosY ;
	Out->m[ 2 ][ 3 ] = 0.0f ;
}

static __inline void CreateRotationXZYMatrix4X4CTD( MATRIX_4X4CT_D * RST Out, float XRot, float YRot, float ZRot )
{
	float SinX, CosX, SinY, CosY, SinZ, CosZ ;
	float CosXSinZ, SinXSinZ ;

	_TABLE_SINCOS( XRot, &SinX, &CosX ) ;
	_TABLE_SINCOS( YRot, &SinY, &CosY ) ;
	_TABLE_SINCOS( ZRot, &SinZ, &CosZ ) ;

	CosXSinZ = CosX * SinZ ;
	SinXSinZ = SinX * SinZ ;

	Out->m[ 0 ][ 0 ] = CosY * CosZ ;
	Out->m[ 0 ][ 1 ] = CosXSinZ * -CosY + SinX * SinY ;
	Out->m[ 0 ][ 2 ] = SinXSinZ *  CosY + CosX * SinY ;
	Out->m[ 0 ][ 3 ] = 0.0f ;
	Out->m[ 1 ][ 0 ] = SinZ ;
	Out->m[ 1 ][ 1 ] = CosX * CosZ ;
	Out->m[ 1 ][ 2 ] = -SinX * CosZ ;
	Out->m[ 1 ][ 3 ] = 0.0f ;
	Out->m[ 2 ][ 0 ] = CosZ * -SinY ;
	Out->m[ 2 ][ 1 ] = CosXSinZ *  SinY + SinX * CosY ;
	Out->m[ 2 ][ 2 ] = SinXSinZ * -SinY + CosX * CosY ;
	Out->m[ 2 ][ 3 ] = 0.0f ;
}

static __inline void CreateRotationXZYMatrix4X4CT( MATRIX_4X4CT * RST Out, float XRot, float YRot, float ZRot )
{
	if( GSYS.DrawSetting.Large3DPositionSupport )
	{
		CreateRotationXZYMatrix4X4CTD( &Out->md, XRot, YRot, ZRot ) ;
	}
	else
	{
		CreateRotationXZYMatrix4X4CTF( &Out->mf, XRot, YRot, ZRot ) ;
	}
}

// �x����]���w����]���y����]�����������s����쐬����
static __inline void CreateRotationYXZMatrix4X4CTF( MATRIX_4X4CT_F * RST Out, float XRot, float YRot, float ZRot )
{
	float SinX, CosX, SinY, CosY, SinZ, CosZ ;
	float SinXSinY, CosYSinX ;

	_TABLE_SINCOS( XRot, &SinX, &CosX ) ;
	_TABLE_SINCOS( YRot, &SinY, &CosY ) ;
	_TABLE_SINCOS( ZRot, &SinZ, &CosZ ) ;

	SinXSinY = SinX * SinY ;
	CosYSinX = CosY * SinX ;

	Out->m[ 0 ][ 0 ] = CosY * CosZ - SinXSinY * SinZ ;
	Out->m[ 0 ][ 1 ] = CosX * -SinZ ;
	Out->m[ 0 ][ 2 ] = SinY * CosZ + CosYSinX * SinZ ;
	Out->m[ 0 ][ 3 ] = 0.0f ;
	Out->m[ 1 ][ 0 ] = CosY * SinZ + SinXSinY * CosZ ;
	Out->m[ 1 ][ 1 ] = CosX * CosZ ;
	Out->m[ 1 ][ 2 ] = SinY * SinZ - CosYSinX * CosZ ;
	Out->m[ 1 ][ 3 ] = 0.0f ;
	Out->m[ 2 ][ 0 ] = -SinY * CosX ;
	Out->m[ 2 ][ 1 ] = SinX ;
	Out->m[ 2 ][ 2 ] = CosY * CosX ;
	Out->m[ 2 ][ 3 ] = 0.0f ;
}

static __inline void CreateRotationYXZMatrix4X4CTD( MATRIX_4X4CT_D * RST Out, float XRot, float YRot, float ZRot )
{
	float SinX, CosX, SinY, CosY, SinZ, CosZ ;
	float SinXSinY, CosYSinX ;

	_TABLE_SINCOS( XRot, &SinX, &CosX ) ;
	_TABLE_SINCOS( YRot, &SinY, &CosY ) ;
	_TABLE_SINCOS( ZRot, &SinZ, &CosZ ) ;

	SinXSinY = SinX * SinY ;
	CosYSinX = CosY * SinX ;

	Out->m[ 0 ][ 0 ] = CosY * CosZ - SinXSinY * SinZ ;
	Out->m[ 0 ][ 1 ] = CosX * -SinZ ;
	Out->m[ 0 ][ 2 ] = SinY * CosZ + CosYSinX * SinZ ;
	Out->m[ 0 ][ 3 ] = 0.0f ;
	Out->m[ 1 ][ 0 ] = CosY * SinZ + SinXSinY * CosZ ;
	Out->m[ 1 ][ 1 ] = CosX * CosZ ;
	Out->m[ 1 ][ 2 ] = SinY * SinZ - CosYSinX * CosZ ;
	Out->m[ 1 ][ 3 ] = 0.0f ;
	Out->m[ 2 ][ 0 ] = -SinY * CosX ;
	Out->m[ 2 ][ 1 ] = SinX ;
	Out->m[ 2 ][ 2 ] = CosY * CosX ;
	Out->m[ 2 ][ 3 ] = 0.0f ;
}

static __inline void CreateRotationYXZMatrix4X4CT( MATRIX_4X4CT * RST Out, float XRot, float YRot, float ZRot )
{
	if( GSYS.DrawSetting.Large3DPositionSupport )
	{
		CreateRotationYXZMatrix4X4CTD( &Out->md, XRot, YRot, ZRot ) ;
	}
	else
	{
		CreateRotationYXZMatrix4X4CTF( &Out->mf, XRot, YRot, ZRot ) ;
	}
}

// �x����]���y����]���w����]�����������s����쐬����
static __inline void CreateRotationYZXMatrix4X4CTF( MATRIX_4X4CT_F * RST Out, float XRot, float YRot, float ZRot )
{
	float SinX, CosX, SinY, CosY, SinZ, CosZ ;
	float CosYSinZ, SinYSinZ ;

	_TABLE_SINCOS( XRot, &SinX, &CosX ) ;
	_TABLE_SINCOS( YRot, &SinY, &CosY ) ;
	_TABLE_SINCOS( ZRot, &SinZ, &CosZ ) ;

	CosYSinZ = CosY * SinZ ;
	SinYSinZ = SinY * SinZ ;

	Out->m[ 0 ][ 0 ] = CosY * CosZ ;
	Out->m[ 0 ][ 1 ] = -SinZ ;
	Out->m[ 0 ][ 2 ] = SinY * CosZ ;
	Out->m[ 0 ][ 3 ] = 0.0f ;
	Out->m[ 1 ][ 0 ] = CosYSinZ * CosX + SinY * SinX ;
	Out->m[ 1 ][ 1 ] = CosZ * CosX ;
	Out->m[ 1 ][ 2 ] = SinYSinZ * CosX - CosY * SinX ;
	Out->m[ 1 ][ 3 ] = 0.0f ;
	Out->m[ 2 ][ 0 ] = CosYSinZ * SinX - SinY * CosX ;
	Out->m[ 2 ][ 1 ] = CosZ * SinX ;
	Out->m[ 2 ][ 2 ] = SinYSinZ * SinX + CosY * CosX ;
	Out->m[ 2 ][ 3 ] = 0.0f ;
}

static __inline void CreateRotationYZXMatrix4X4CTD( MATRIX_4X4CT_D * RST Out, float XRot, float YRot, float ZRot )
{
	float SinX, CosX, SinY, CosY, SinZ, CosZ ;
	float CosYSinZ, SinYSinZ ;

	_TABLE_SINCOS( XRot, &SinX, &CosX ) ;
	_TABLE_SINCOS( YRot, &SinY, &CosY ) ;
	_TABLE_SINCOS( ZRot, &SinZ, &CosZ ) ;

	CosYSinZ = CosY * SinZ ;
	SinYSinZ = SinY * SinZ ;

	Out->m[ 0 ][ 0 ] = CosY * CosZ ;
	Out->m[ 0 ][ 1 ] = -SinZ ;
	Out->m[ 0 ][ 2 ] = SinY * CosZ ;
	Out->m[ 0 ][ 3 ] = 0.0f ;
	Out->m[ 1 ][ 0 ] = CosYSinZ * CosX + SinY * SinX ;
	Out->m[ 1 ][ 1 ] = CosZ * CosX ;
	Out->m[ 1 ][ 2 ] = SinYSinZ * CosX - CosY * SinX ;
	Out->m[ 1 ][ 3 ] = 0.0f ;
	Out->m[ 2 ][ 0 ] = CosYSinZ * SinX - SinY * CosX ;
	Out->m[ 2 ][ 1 ] = CosZ * SinX ;
	Out->m[ 2 ][ 2 ] = SinYSinZ * SinX + CosY * CosX ;
	Out->m[ 2 ][ 3 ] = 0.0f ;
}

static __inline void CreateRotationYZXMatrix4X4CT( MATRIX_4X4CT * RST Out, float XRot, float YRot, float ZRot )
{
	if( GSYS.DrawSetting.Large3DPositionSupport )
	{
		CreateRotationYZXMatrix4X4CTD( &Out->md, XRot, YRot, ZRot ) ;
	}
	else
	{
		CreateRotationYZXMatrix4X4CTF( &Out->mf, XRot, YRot, ZRot ) ;
	}
}

// �y����]���w����]���x����]�����������s����쐬����
static __inline void CreateRotationZXYMatrix4X4CTF( MATRIX_4X4CT_F * RST Out, float XRot, float YRot, float ZRot )
{
	float SinX, CosX, SinY, CosY, SinZ, CosZ ;
	float SinXSinZ, SinXCosZ ;

	_TABLE_SINCOS( XRot, &SinX, &CosX ) ;
	_TABLE_SINCOS( YRot, &SinY, &CosY ) ;
	_TABLE_SINCOS( ZRot, &SinZ, &CosZ ) ;

	SinXSinZ = SinX * SinZ ;
	SinXCosZ = SinX * CosZ ;

	Out->m[ 0 ][ 0 ] = CosY *  CosZ + SinXSinZ * SinY ;
	Out->m[ 0 ][ 1 ] = CosY * -SinZ + SinXCosZ * SinY ;
	Out->m[ 0 ][ 2 ] = CosX * SinY ;
	Out->m[ 0 ][ 3 ] = 0.0f ;
	Out->m[ 1 ][ 0 ] = CosX * SinZ ;
	Out->m[ 1 ][ 1 ] = CosX * CosZ ;
	Out->m[ 1 ][ 2 ] = -SinX ;
	Out->m[ 1 ][ 3 ] = 0.0f ;
	Out->m[ 2 ][ 0 ] = CosZ * -SinY + SinXSinZ * CosY ;
	Out->m[ 2 ][ 1 ] = SinZ *  SinY + SinXCosZ * CosY ;
	Out->m[ 2 ][ 2 ] = CosX * CosY ;
	Out->m[ 2 ][ 3 ] = 0.0f ;
}

static __inline void CreateRotationZXYMatrix4X4CTD( MATRIX_4X4CT_D * RST Out, float XRot, float YRot, float ZRot )
{
	float SinX, CosX, SinY, CosY, SinZ, CosZ ;
	float SinXSinZ, SinXCosZ ;

	_TABLE_SINCOS( XRot, &SinX, &CosX ) ;
	_TABLE_SINCOS( YRot, &SinY, &CosY ) ;
	_TABLE_SINCOS( ZRot, &SinZ, &CosZ ) ;

	SinXSinZ = SinX * SinZ ;
	SinXCosZ = SinX * CosZ ;

	Out->m[ 0 ][ 0 ] = CosY *  CosZ + SinXSinZ * SinY ;
	Out->m[ 0 ][ 1 ] = CosY * -SinZ + SinXCosZ * SinY ;
	Out->m[ 0 ][ 2 ] = CosX * SinY ;
	Out->m[ 0 ][ 3 ] = 0.0f ;
	Out->m[ 1 ][ 0 ] = CosX * SinZ ;
	Out->m[ 1 ][ 1 ] = CosX * CosZ ;
	Out->m[ 1 ][ 2 ] = -SinX ;
	Out->m[ 1 ][ 3 ] = 0.0f ;
	Out->m[ 2 ][ 0 ] = CosZ * -SinY + SinXSinZ * CosY ;
	Out->m[ 2 ][ 1 ] = SinZ *  SinY + SinXCosZ * CosY ;
	Out->m[ 2 ][ 2 ] = CosX * CosY ;
	Out->m[ 2 ][ 3 ] = 0.0f ;
}

static __inline void CreateRotationZXYMatrix4X4CT( MATRIX_4X4CT * RST Out, float XRot, float YRot, float ZRot )
{
	if( GSYS.DrawSetting.Large3DPositionSupport )
	{
		CreateRotationZXYMatrix4X4CTD( &Out->md, XRot, YRot, ZRot ) ;
	}
	else
	{
		CreateRotationZXYMatrix4X4CTF( &Out->mf, XRot, YRot, ZRot ) ;
	}
}

// �y����]���x����]���w����]�����������s����쐬����
static __inline void CreateRotationZYXMatrix4X4CTF( MATRIX_4X4CT_F * RST Out, float XRot, float YRot, float ZRot )
{
	float SinX, CosX, SinY, CosY, SinZ, CosZ ;
	float CosZSinY, SinZSinY ;

	_TABLE_SINCOS( XRot, &SinX, &CosX ) ;
	_TABLE_SINCOS( YRot, &SinY, &CosY ) ;
	_TABLE_SINCOS( ZRot, &SinZ, &CosZ ) ;

	CosZSinY = CosZ * SinY ;
	SinZSinY = SinZ * SinY ;

	Out->m[ 0 ][ 0 ] = CosY * CosZ ;
	Out->m[ 0 ][ 1 ] = -SinZ * CosY ;
	Out->m[ 0 ][ 2 ] = SinY ;
	Out->m[ 0 ][ 3 ] = 0.0f ;
	Out->m[ 1 ][ 0 ] = SinZ * CosX + CosZSinY * SinX ;
	Out->m[ 1 ][ 1 ] = CosZ * CosX - SinZSinY * SinX ;
	Out->m[ 1 ][ 2 ] = CosY * -SinX ;
	Out->m[ 1 ][ 3 ] = 0.0f ;
	Out->m[ 2 ][ 0 ] = SinZ * SinX - CosZSinY * CosX ;
	Out->m[ 2 ][ 1 ] = CosZ * SinX + SinZSinY * CosX ;
	Out->m[ 2 ][ 2 ] = CosY *  CosX ;
	Out->m[ 2 ][ 3 ] = 0.0f ;
}

static __inline void CreateRotationZYXMatrix4X4CTD( MATRIX_4X4CT_D * RST Out, float XRot, float YRot, float ZRot )
{
	float SinX, CosX, SinY, CosY, SinZ, CosZ ;
	float CosZSinY, SinZSinY ;

	_TABLE_SINCOS( XRot, &SinX, &CosX ) ;
	_TABLE_SINCOS( YRot, &SinY, &CosY ) ;
	_TABLE_SINCOS( ZRot, &SinZ, &CosZ ) ;

	CosZSinY = CosZ * SinY ;
	SinZSinY = SinZ * SinY ;

	Out->m[ 0 ][ 0 ] = CosY * CosZ ;
	Out->m[ 0 ][ 1 ] = -SinZ * CosY ;
	Out->m[ 0 ][ 2 ] = SinY ;
	Out->m[ 0 ][ 3 ] = 0.0f ;
	Out->m[ 1 ][ 0 ] = SinZ * CosX + CosZSinY * SinX ;
	Out->m[ 1 ][ 1 ] = CosZ * CosX - SinZSinY * SinX ;
	Out->m[ 1 ][ 2 ] = CosY * -SinX ;
	Out->m[ 1 ][ 3 ] = 0.0f ;
	Out->m[ 2 ][ 0 ] = SinZ * SinX - CosZSinY * CosX ;
	Out->m[ 2 ][ 1 ] = CosZ * SinX + SinZSinY * CosX ;
	Out->m[ 2 ][ 2 ] = CosY *  CosX ;
	Out->m[ 2 ][ 3 ] = 0.0f ;
}

static __inline void CreateRotationZYXMatrix4X4CT( MATRIX_4X4CT * RST Out, float XRot, float YRot, float ZRot )
{
	if( GSYS.DrawSetting.Large3DPositionSupport )
	{
		CreateRotationZYXMatrix4X4CTD( &Out->md, XRot, YRot, ZRot ) ;
	}
	else
	{
		CreateRotationZYXMatrix4X4CTF( &Out->mf, XRot, YRot, ZRot ) ;
	}
}

static __inline void UnSafeTranslateOnlyMultiplyMatrix4X4CTF( MATRIX_4X4CT_F * RST Out, MATRIX_4X4CT_F * RST In1, MATRIX_4X4CT_F * RST In2 )
{
	Out->m[ 0 ][ 0 ] = In2->m[ 0 ][ 0 ] ;
	Out->m[ 0 ][ 1 ] = In2->m[ 0 ][ 1 ] ;
	Out->m[ 0 ][ 2 ] = In2->m[ 0 ][ 2 ] ;
	Out->m[ 0 ][ 3 ] = In1->m[ 0 ][ 3 ] * In2->m[ 0 ][ 0 ] + In1->m[ 1 ][ 3 ] * In2->m[ 0 ][ 1 ] + In1->m[ 2 ][ 3 ] * In2->m[ 0 ][ 2 ] + In2->m[ 0 ][ 3 ] ;
	Out->m[ 1 ][ 0 ] = In2->m[ 1 ][ 0 ] ;
	Out->m[ 1 ][ 1 ] = In2->m[ 1 ][ 1 ] ;
	Out->m[ 1 ][ 2 ] = In2->m[ 1 ][ 2 ] ;
	Out->m[ 1 ][ 3 ] = In1->m[ 0 ][ 3 ] * In2->m[ 1 ][ 0 ] + In1->m[ 1 ][ 3 ] * In2->m[ 1 ][ 1 ] + In1->m[ 2 ][ 3 ] * In2->m[ 1 ][ 2 ] + In2->m[ 1 ][ 3 ] ;
	Out->m[ 2 ][ 0 ] = In2->m[ 2 ][ 0 ] ;
	Out->m[ 2 ][ 1 ] = In2->m[ 2 ][ 1 ] ;
	Out->m[ 2 ][ 2 ] = In2->m[ 2 ][ 2 ] ;
	Out->m[ 2 ][ 3 ] = In1->m[ 0 ][ 3 ] * In2->m[ 2 ][ 0 ] + In1->m[ 1 ][ 3 ] * In2->m[ 2 ][ 1 ] + In1->m[ 2 ][ 3 ] * In2->m[ 2 ][ 2 ] + In2->m[ 2 ][ 3 ] ;
}

static __inline void UnSafeTranslateOnlyMultiplyMatrix4X4CTD( MATRIX_4X4CT_D * RST Out, MATRIX_4X4CT_D * RST In1, MATRIX_4X4CT_D * RST In2 )
{
	Out->m[ 0 ][ 0 ] = In2->m[ 0 ][ 0 ] ;
	Out->m[ 0 ][ 1 ] = In2->m[ 0 ][ 1 ] ;
	Out->m[ 0 ][ 2 ] = In2->m[ 0 ][ 2 ] ;
	Out->m[ 0 ][ 3 ] = In1->m[ 0 ][ 3 ] * In2->m[ 0 ][ 0 ] + In1->m[ 1 ][ 3 ] * In2->m[ 0 ][ 1 ] + In1->m[ 2 ][ 3 ] * In2->m[ 0 ][ 2 ] + In2->m[ 0 ][ 3 ] ;
	Out->m[ 1 ][ 0 ] = In2->m[ 1 ][ 0 ] ;
	Out->m[ 1 ][ 1 ] = In2->m[ 1 ][ 1 ] ;
	Out->m[ 1 ][ 2 ] = In2->m[ 1 ][ 2 ] ;
	Out->m[ 1 ][ 3 ] = In1->m[ 0 ][ 3 ] * In2->m[ 1 ][ 0 ] + In1->m[ 1 ][ 3 ] * In2->m[ 1 ][ 1 ] + In1->m[ 2 ][ 3 ] * In2->m[ 1 ][ 2 ] + In2->m[ 1 ][ 3 ] ;
	Out->m[ 2 ][ 0 ] = In2->m[ 2 ][ 0 ] ;
	Out->m[ 2 ][ 1 ] = In2->m[ 2 ][ 1 ] ;
	Out->m[ 2 ][ 2 ] = In2->m[ 2 ][ 2 ] ;
	Out->m[ 2 ][ 3 ] = In1->m[ 0 ][ 3 ] * In2->m[ 2 ][ 0 ] + In1->m[ 1 ][ 3 ] * In2->m[ 2 ][ 1 ] + In1->m[ 2 ][ 3 ] * In2->m[ 2 ][ 2 ] + In2->m[ 2 ][ 3 ] ;
}

static __inline void UnSafeTranslateOnlyMultiplyMatrix4X4CT_DF_D( MATRIX_4X4CT_D * RST Out, MATRIX_4X4CT_D * RST In1, MATRIX_4X4CT_F * RST In2 )
{
	Out->m[ 0 ][ 0 ] = In2->m[ 0 ][ 0 ] ;
	Out->m[ 0 ][ 1 ] = In2->m[ 0 ][ 1 ] ;
	Out->m[ 0 ][ 2 ] = In2->m[ 0 ][ 2 ] ;
	Out->m[ 0 ][ 3 ] = In1->m[ 0 ][ 3 ] * In2->m[ 0 ][ 0 ] + In1->m[ 1 ][ 3 ] * In2->m[ 0 ][ 1 ] + In1->m[ 2 ][ 3 ] * In2->m[ 0 ][ 2 ] + In2->m[ 0 ][ 3 ] ;
	Out->m[ 1 ][ 0 ] = In2->m[ 1 ][ 0 ] ;
	Out->m[ 1 ][ 1 ] = In2->m[ 1 ][ 1 ] ;
	Out->m[ 1 ][ 2 ] = In2->m[ 1 ][ 2 ] ;
	Out->m[ 1 ][ 3 ] = In1->m[ 0 ][ 3 ] * In2->m[ 1 ][ 0 ] + In1->m[ 1 ][ 3 ] * In2->m[ 1 ][ 1 ] + In1->m[ 2 ][ 3 ] * In2->m[ 1 ][ 2 ] + In2->m[ 1 ][ 3 ] ;
	Out->m[ 2 ][ 0 ] = In2->m[ 2 ][ 0 ] ;
	Out->m[ 2 ][ 1 ] = In2->m[ 2 ][ 1 ] ;
	Out->m[ 2 ][ 2 ] = In2->m[ 2 ][ 2 ] ;
	Out->m[ 2 ][ 3 ] = In1->m[ 0 ][ 3 ] * In2->m[ 2 ][ 0 ] + In1->m[ 1 ][ 3 ] * In2->m[ 2 ][ 1 ] + In1->m[ 2 ][ 3 ] * In2->m[ 2 ][ 2 ] + In2->m[ 2 ][ 3 ] ;
}

static __inline void UnSafeTranslateOnlyMultiplyMatrix4X4CT_FD_D( MATRIX_4X4CT_D * RST Out, MATRIX_4X4CT_F * RST In1, MATRIX_4X4CT_D * RST In2 )
{
	Out->m[ 0 ][ 0 ] = In2->m[ 0 ][ 0 ] ;
	Out->m[ 0 ][ 1 ] = In2->m[ 0 ][ 1 ] ;
	Out->m[ 0 ][ 2 ] = In2->m[ 0 ][ 2 ] ;
	Out->m[ 0 ][ 3 ] = In1->m[ 0 ][ 3 ] * In2->m[ 0 ][ 0 ] + In1->m[ 1 ][ 3 ] * In2->m[ 0 ][ 1 ] + In1->m[ 2 ][ 3 ] * In2->m[ 0 ][ 2 ] + In2->m[ 0 ][ 3 ] ;
	Out->m[ 1 ][ 0 ] = In2->m[ 1 ][ 0 ] ;
	Out->m[ 1 ][ 1 ] = In2->m[ 1 ][ 1 ] ;
	Out->m[ 1 ][ 2 ] = In2->m[ 1 ][ 2 ] ;
	Out->m[ 1 ][ 3 ] = In1->m[ 0 ][ 3 ] * In2->m[ 1 ][ 0 ] + In1->m[ 1 ][ 3 ] * In2->m[ 1 ][ 1 ] + In1->m[ 2 ][ 3 ] * In2->m[ 1 ][ 2 ] + In2->m[ 1 ][ 3 ] ;
	Out->m[ 2 ][ 0 ] = In2->m[ 2 ][ 0 ] ;
	Out->m[ 2 ][ 1 ] = In2->m[ 2 ][ 1 ] ;
	Out->m[ 2 ][ 2 ] = In2->m[ 2 ][ 2 ] ;
	Out->m[ 2 ][ 3 ] = In1->m[ 0 ][ 3 ] * In2->m[ 2 ][ 0 ] + In1->m[ 1 ][ 3 ] * In2->m[ 2 ][ 1 ] + In1->m[ 2 ][ 3 ] * In2->m[ 2 ][ 2 ] + In2->m[ 2 ][ 3 ] ;
}

static __inline void UnSafeTranslateOnlyMultiplyMatrix4X4CT( MATRIX_4X4CT * RST Out, MATRIX_4X4CT * RST In1, MATRIX_4X4CT * RST In2 )
{
	if( GSYS.DrawSetting.Large3DPositionSupport )
	{
		UnSafeTranslateOnlyMultiplyMatrix4X4CTD( &Out->md, &In1->md, &In2->md ) ;
	}
	else
	{
		UnSafeTranslateOnlyMultiplyMatrix4X4CTF( &Out->mf, &In1->mf, &In2->mf ) ;
	}
}

static __inline void UnSafeTranslateOnlyMultiplyMatrix4X4CT_FC( MATRIX_4X4CT * RST Out, MATRIX_4X4CT_F * RST In1, MATRIX_4X4CT * RST In2 )
{
	if( GSYS.DrawSetting.Large3DPositionSupport )
	{
		UnSafeTranslateOnlyMultiplyMatrix4X4CT_FD_D( &Out->md, In1, &In2->md ) ;
	}
	else
	{
		UnSafeTranslateOnlyMultiplyMatrix4X4CTF( &Out->mf, In1, &In2->mf ) ;
	}
}

static __inline void UnSafeTranslateOnlyMultiplyMatrix4X4CT_CF( MATRIX_4X4CT * RST Out, MATRIX_4X4CT * RST In1, MATRIX_4X4CT_F * RST In2 )
{
	if( GSYS.DrawSetting.Large3DPositionSupport )
	{
		UnSafeTranslateOnlyMultiplyMatrix4X4CT_DF_D( &Out->md, &In1->md, In2 ) ;
	}
	else
	{
		UnSafeTranslateOnlyMultiplyMatrix4X4CTF( &Out->mf, &In1->mf, In2 ) ;
	}
}

static __inline void UnSafeMatrix4X4CT_F_EqPlus_F_Sub_F( MATRIX_4X4CT_F *RST Out, MATRIX_4X4CT_F * RST In1, MATRIX_4X4CT_F * RST In2 )
{
	Out->m[ 0 ][ 0 ] += In1->m[ 0 ][ 0 ] - In2->m[ 0 ][ 0 ] ;
	Out->m[ 0 ][ 1 ] += In1->m[ 0 ][ 1 ] - In2->m[ 0 ][ 1 ] ;
	Out->m[ 0 ][ 2 ] += In1->m[ 0 ][ 2 ] - In2->m[ 0 ][ 2 ] ;
	Out->m[ 0 ][ 3 ] += In1->m[ 0 ][ 3 ] - In2->m[ 0 ][ 3 ] ;

	Out->m[ 1 ][ 0 ] += In1->m[ 1 ][ 0 ] - In2->m[ 1 ][ 0 ] ;
	Out->m[ 1 ][ 1 ] += In1->m[ 1 ][ 1 ] - In2->m[ 1 ][ 1 ] ;
	Out->m[ 1 ][ 2 ] += In1->m[ 1 ][ 2 ] - In2->m[ 1 ][ 2 ] ;
	Out->m[ 1 ][ 3 ] += In1->m[ 1 ][ 3 ] - In2->m[ 1 ][ 3 ] ;

	Out->m[ 2 ][ 0 ] += In1->m[ 2 ][ 0 ] - In2->m[ 2 ][ 0 ] ;
	Out->m[ 2 ][ 1 ] += In1->m[ 2 ][ 1 ] - In2->m[ 2 ][ 1 ] ;
	Out->m[ 2 ][ 2 ] += In1->m[ 2 ][ 2 ] - In2->m[ 2 ][ 2 ] ;
	Out->m[ 2 ][ 3 ] += In1->m[ 2 ][ 3 ] - In2->m[ 2 ][ 3 ] ;
}

static __inline void UnSafeMatrix4X4CT_F_EqPlus_F_Sub_F_Mul_S( MATRIX_4X4CT_F *RST Out, MATRIX_4X4CT_F * RST In1, MATRIX_4X4CT_F * RST In2, float In3 )
{
	Out->m[ 0 ][ 0 ] += ( In1->m[ 0 ][ 0 ] - In2->m[ 0 ][ 0 ] ) * In3 ;
	Out->m[ 0 ][ 1 ] += ( In1->m[ 0 ][ 1 ] - In2->m[ 0 ][ 1 ] ) * In3 ;
	Out->m[ 0 ][ 2 ] += ( In1->m[ 0 ][ 2 ] - In2->m[ 0 ][ 2 ] ) * In3 ;
	Out->m[ 0 ][ 3 ] += ( In1->m[ 0 ][ 3 ] - In2->m[ 0 ][ 3 ] ) * In3 ;

	Out->m[ 1 ][ 0 ] += ( In1->m[ 1 ][ 0 ] - In2->m[ 1 ][ 0 ] ) * In3 ;
	Out->m[ 1 ][ 1 ] += ( In1->m[ 1 ][ 1 ] - In2->m[ 1 ][ 1 ] ) * In3 ;
	Out->m[ 1 ][ 2 ] += ( In1->m[ 1 ][ 2 ] - In2->m[ 1 ][ 2 ] ) * In3 ;
	Out->m[ 1 ][ 3 ] += ( In1->m[ 1 ][ 3 ] - In2->m[ 1 ][ 3 ] ) * In3 ;

	Out->m[ 2 ][ 0 ] += ( In1->m[ 2 ][ 0 ] - In2->m[ 2 ][ 0 ] ) * In3 ;
	Out->m[ 2 ][ 1 ] += ( In1->m[ 2 ][ 1 ] - In2->m[ 2 ][ 1 ] ) * In3 ;
	Out->m[ 2 ][ 2 ] += ( In1->m[ 2 ][ 2 ] - In2->m[ 2 ][ 2 ] ) * In3 ;
	Out->m[ 2 ][ 3 ] += ( In1->m[ 2 ][ 3 ] - In2->m[ 2 ][ 3 ] ) * In3 ;
}

static __inline void UnSafeMatrix4X4CT_F_EqPlus_F( MATRIX_4X4CT_F *RST Out, MATRIX_4X4CT_F * RST In1 )
{
	Out->m[ 0 ][ 0 ] += In1->m[ 0 ][ 0 ] ;
	Out->m[ 0 ][ 1 ] += In1->m[ 0 ][ 1 ] ;
	Out->m[ 0 ][ 2 ] += In1->m[ 0 ][ 2 ] ;
	Out->m[ 0 ][ 3 ] += In1->m[ 0 ][ 3 ] ;

	Out->m[ 1 ][ 0 ] += In1->m[ 1 ][ 0 ] ;
	Out->m[ 1 ][ 1 ] += In1->m[ 1 ][ 1 ] ;
	Out->m[ 1 ][ 2 ] += In1->m[ 1 ][ 2 ] ;
	Out->m[ 1 ][ 3 ] += In1->m[ 1 ][ 3 ] ;

	Out->m[ 2 ][ 0 ] += In1->m[ 2 ][ 0 ] ;
	Out->m[ 2 ][ 1 ] += In1->m[ 2 ][ 1 ] ;
	Out->m[ 2 ][ 2 ] += In1->m[ 2 ][ 2 ] ;
	Out->m[ 2 ][ 3 ] += In1->m[ 2 ][ 3 ] ;
}

static __inline void SetupSkiningBoneMatrix( MV1_MODEL &Model, MV1_FRAME &Frame )
{
	MV1_SKIN_BONE *SkinW ;
	int index ;
	int i ;
	int j ;

	if( Frame.BaseData->SkinBoneNum != 0 )
	{
		SkinW = Frame.BaseData->SkinBone ;
		index = ( int )( SkinW - Model.BaseData->SkinBone ) ;
		for( i = 0 ; i < Frame.BaseData->SkinBoneNum ; i ++, SkinW ++, index ++ )
		{
			if( SkinW->ModelLocalMatrixIsTranslateOnly == 0 )
			{
				UnSafeMultiplyMatrix4X4CT_FC( &Model.SkinBoneMatrix[ index ], &SkinW->ModelLocalMatrix, &Frame.LocalWorldMatrix ) ;
			}
			else
			{
				UnSafeTranslateOnlyMultiplyMatrix4X4CT_FC( &Model.SkinBoneMatrix[ index ], &SkinW->ModelLocalMatrix, &Frame.LocalWorldMatrix ) ;
			}

			// �|���S�����X�g�X�V�t���O��|��
			for( j = 0 ; j < SkinW->UseFrameNum ; j ++ )
			{
				MV1_FRAME *FrameT ;

				FrameT = Model.Frame + SkinW->UseFrame[ j ].Index ;
				FrameT->SetupRefPolygon[ 1 ][ 0 ] = false ;
				FrameT->SetupRefPolygon[ 1 ][ 1 ] = false ;
			}
		}
	}
}

static 
#ifndef __BCC
__inline
#endif
int __FTOL( float Real )
{
#ifdef DX_NON_INLINE_ASM
	return (int)Real ;
#else
	DWORD Result ;
	WORD STFlag, DSTFlag ;
	__asm
	{
		fnstcw	STFlag
		mov		ax, STFlag
		or		ax, 0xC00
		mov		DSTFlag, ax
		fld		Real
		fldcw	DSTFlag
		fistp	Result
		fldcw	STFlag
	}
	return (int)Result ;
#endif
}

// ���W�ϊ������g�p���č��W�ϊ��s����쐬����
static void MV1SetupTransformMatrix( MATRIX_4X4CT_F * RST BlendMatrix, int ValidFlag, VECTOR * RST Translate, VECTOR * RST Scale, int RotateOrder, VECTOR * RST PreRotate, VECTOR * RST Rotate, VECTOR * RST PostRotate, FLOAT4 * RST Quaternion )
{
	// ��]
	if( ValidFlag & MV1_ANIMVALUE_ROTATE )
	{
		if( PreRotate == NULL && PostRotate == NULL )
		{
			switch( RotateOrder )
			{
			case MV1_ROTATE_ORDER_XYZ : CreateRotationXYZMatrix4X4CTF( BlendMatrix, Rotate->x, Rotate->y, Rotate->z ) ; break ;
			case MV1_ROTATE_ORDER_XZY : CreateRotationXZYMatrix4X4CTF( BlendMatrix, Rotate->x, Rotate->y, Rotate->z ) ; break ;
			case MV1_ROTATE_ORDER_YXZ : CreateRotationYXZMatrix4X4CTF( BlendMatrix, Rotate->x, Rotate->y, Rotate->z ) ; break ;
			case MV1_ROTATE_ORDER_YZX : CreateRotationYZXMatrix4X4CTF( BlendMatrix, Rotate->x, Rotate->y, Rotate->z ) ; break ;
			case MV1_ROTATE_ORDER_ZXY : CreateRotationZXYMatrix4X4CTF( BlendMatrix, Rotate->x, Rotate->y, Rotate->z ) ; break ;
			case MV1_ROTATE_ORDER_ZYX : CreateRotationZYXMatrix4X4CTF( BlendMatrix, Rotate->x, Rotate->y, Rotate->z ) ; break ;
			}
		}
		else
		{
			MATRIX_4X4CT_F PreRotMat, PostRotMat, RotMat ;

			switch( RotateOrder )
			{
			case MV1_ROTATE_ORDER_XYZ :
				if( PreRotate )  CreateRotationXYZMatrix4X4CTF( &PreRotMat,  PreRotate->x,  PreRotate->y,  PreRotate->z ) ;
				if( PostRotate ) CreateRotationXYZMatrix4X4CTF( &PostRotMat, PostRotate->x, PostRotate->y, PostRotate->z ) ;
				CreateRotationXYZMatrix4X4CTF( &RotMat, Rotate->x, Rotate->y, Rotate->z ) ;
				break ;

			case MV1_ROTATE_ORDER_XZY :
				if( PreRotate )  CreateRotationXZYMatrix4X4CTF( &PreRotMat,  PreRotate->x,  PreRotate->y,  PreRotate->z ) ;
				if( PostRotate ) CreateRotationXZYMatrix4X4CTF( &PostRotMat, PostRotate->x, PostRotate->y, PostRotate->z ) ;
				CreateRotationXZYMatrix4X4CTF( &RotMat, Rotate->x, Rotate->y, Rotate->z ) ;
				break ;

			case MV1_ROTATE_ORDER_YXZ :
				if( PreRotate )  CreateRotationYXZMatrix4X4CTF( &PreRotMat,  PreRotate->x,  PreRotate->y,  PreRotate->z ) ;
				if( PostRotate ) CreateRotationYXZMatrix4X4CTF( &PostRotMat, PostRotate->x, PostRotate->y, PostRotate->z ) ;
				CreateRotationYXZMatrix4X4CTF( &RotMat, Rotate->x, Rotate->y, Rotate->z ) ;
				break ;

			case MV1_ROTATE_ORDER_YZX :
				if( PreRotate )  CreateRotationYZXMatrix4X4CTF( &PreRotMat,  PreRotate->x,  PreRotate->y,  PreRotate->z ) ;
				if( PostRotate ) CreateRotationYZXMatrix4X4CTF( &PostRotMat, PostRotate->x, PostRotate->y, PostRotate->z ) ;
				CreateRotationYZXMatrix4X4CTF( &RotMat, Rotate->x, Rotate->y, Rotate->z ) ;
				break ;

			case MV1_ROTATE_ORDER_ZXY :
				if( PreRotate )  CreateRotationZXYMatrix4X4CTF( &PreRotMat,  PreRotate->x,  PreRotate->y,  PreRotate->z ) ;
				if( PostRotate ) CreateRotationZXYMatrix4X4CTF( &PostRotMat, PostRotate->x, PostRotate->y, PostRotate->z ) ;
				CreateRotationZXYMatrix4X4CTF( &RotMat, Rotate->x, Rotate->y, Rotate->z ) ;
				break ;

			case MV1_ROTATE_ORDER_ZYX :
				if( PreRotate )  CreateRotationZYXMatrix4X4CTF( &PreRotMat,  PreRotate->x,  PreRotate->y,  PreRotate->z ) ;
				if( PostRotate ) CreateRotationZYXMatrix4X4CTF( &PostRotMat, PostRotate->x, PostRotate->y, PostRotate->z ) ;
				CreateRotationZYXMatrix4X4CTF( &RotMat, Rotate->x, Rotate->y, Rotate->z ) ;
				break ;
			}

			if( PreRotate && PostRotate )
			{
				MATRIX_4X4CT_F TempMatrix ;

				UnSafeMultiplyMatrix4X4CTF( &TempMatrix, &RotMat, &PreRotMat ) ;
				UnSafeMultiplyMatrix4X4CTF( BlendMatrix, &PostRotMat, &TempMatrix ) ;
			}
			else
			if( PreRotate )
			{
				UnSafeMultiplyMatrix4X4CTF( BlendMatrix, &RotMat, &PreRotMat ) ;
			}
			else
			{
				UnSafeMultiplyMatrix4X4CTF( BlendMatrix, &PostRotMat, &RotMat ) ;
			}
		}
	}
	else
	// �N�H�[�^�j�I����]
	if( ValidFlag & MV1_ANIMVALUE_QUATERNION_X )
	{
		MATRIX TempMatrix1, TempMatrix2 ;

		TempMatrix1.m[3][0] = -Quaternion->x ; TempMatrix1.m[3][1] = -Quaternion->y ; TempMatrix1.m[3][2] = -Quaternion->z ; TempMatrix1.m[3][3] =  Quaternion->w ;
		TempMatrix1.m[2][0] = -Quaternion->y ; TempMatrix1.m[2][1] =  Quaternion->x ; TempMatrix1.m[2][2] =  Quaternion->w ; TempMatrix1.m[2][3] =  Quaternion->z ;
		TempMatrix1.m[1][0] =  Quaternion->z ; TempMatrix1.m[1][1] =  Quaternion->w ; TempMatrix1.m[1][2] = -Quaternion->x ; TempMatrix1.m[1][3] =  Quaternion->y ;
		TempMatrix1.m[0][0] =  Quaternion->w ; TempMatrix1.m[0][1] = -Quaternion->z ; TempMatrix1.m[0][2] =  Quaternion->y ; TempMatrix1.m[0][3] =  Quaternion->x ;

		TempMatrix2.m[3][0] =  Quaternion->x ; TempMatrix2.m[3][1] =  Quaternion->y ; TempMatrix2.m[3][2] =  Quaternion->z ; TempMatrix2.m[3][3] =  Quaternion->w ;
		TempMatrix2.m[2][0] = -Quaternion->y ; TempMatrix2.m[2][1] =  Quaternion->x ; TempMatrix2.m[2][2] =  Quaternion->w ; TempMatrix2.m[2][3] = -Quaternion->z ;
		TempMatrix2.m[1][0] =  Quaternion->z ; TempMatrix2.m[1][1] =  Quaternion->w ; TempMatrix2.m[1][2] = -Quaternion->x ; TempMatrix2.m[1][3] = -Quaternion->y ;
		TempMatrix2.m[0][0] =  Quaternion->w ; TempMatrix2.m[0][1] = -Quaternion->z ; TempMatrix2.m[0][2] =  Quaternion->y ; TempMatrix2.m[0][3] = -Quaternion->x ;

		CreateMultiplyMatrix( &TempMatrix1, &TempMatrix2, &TempMatrix1 ) ;
		ConvertMatrixFToMatrix4x4cF( BlendMatrix, &TempMatrix1 ) ;
	}
	else
	if( ValidFlag & MV1_ANIMVALUE_QUATERNION_VMD )
	{
		float	x2 = Quaternion->x * Quaternion->x * 2.0f ;
		float	y2 = Quaternion->y * Quaternion->y * 2.0f ;
		float	z2 = Quaternion->z * Quaternion->z * 2.0f ;
		float	xy = Quaternion->x * Quaternion->y * 2.0f ;
		float	yz = Quaternion->y * Quaternion->z * 2.0f ;
		float	zx = Quaternion->z * Quaternion->x * 2.0f ;
		float	xw = Quaternion->x * Quaternion->w * 2.0f ;
		float	yw = Quaternion->y * Quaternion->w * 2.0f ;
		float	zw = Quaternion->z * Quaternion->w * 2.0f ;

		BlendMatrix->m[ 0 ][ 0 ] = 1.0f - y2 - z2 ;
		BlendMatrix->m[ 1 ][ 0 ] = xy + zw ;
		BlendMatrix->m[ 2 ][ 0 ] = zx - yw ;
		BlendMatrix->m[ 0 ][ 1 ] = xy - zw ;
		BlendMatrix->m[ 1 ][ 1 ] = 1.0f - z2 - x2 ;
		BlendMatrix->m[ 2 ][ 1 ] = yz + xw ;
		BlendMatrix->m[ 0 ][ 2 ] = zx + yw ;
		BlendMatrix->m[ 1 ][ 2 ] = yz - xw ;
		BlendMatrix->m[ 2 ][ 2 ] = 1.0f - x2 - y2 ;

		BlendMatrix->m[ 0 ][ 3 ] =
		BlendMatrix->m[ 1 ][ 3 ] =
		BlendMatrix->m[ 2 ][ 3 ] = 0.0f ;
	}
	else
	{
		BlendMatrix->m[ 0 ][ 0 ] = 1.0f ;
		BlendMatrix->m[ 0 ][ 1 ] = 0.0f ;
		BlendMatrix->m[ 0 ][ 2 ] = 0.0f ;
		BlendMatrix->m[ 1 ][ 0 ] = 0.0f ;
		BlendMatrix->m[ 1 ][ 1 ] = 1.0f ;
		BlendMatrix->m[ 1 ][ 2 ] = 0.0f ;
		BlendMatrix->m[ 2 ][ 0 ] = 0.0f ;
		BlendMatrix->m[ 2 ][ 1 ] = 0.0f ;
		BlendMatrix->m[ 2 ][ 2 ] = 1.0f ;
	}

	// ���s�ړ�
	if( ValidFlag & MV1_ANIMVALUE_TRANSLATE )
	{
		BlendMatrix->m[ 0 ][ 3 ] = Translate->x ;
		BlendMatrix->m[ 1 ][ 3 ] = Translate->y ;
		BlendMatrix->m[ 2 ][ 3 ] = Translate->z ;
	}
	else
	{
		BlendMatrix->m[ 0 ][ 3 ] = 0.0f ;
		BlendMatrix->m[ 1 ][ 3 ] = 0.0f ;
		BlendMatrix->m[ 2 ][ 3 ] = 0.0f ;
	}

	// �X�P�[�����O
	if( ValidFlag & MV1_ANIMVALUE_SCALE )
	{
		BlendMatrix->m[ 0 ][ 0 ] *= Scale->x ;
		BlendMatrix->m[ 0 ][ 1 ] *= Scale->y ;
		BlendMatrix->m[ 0 ][ 2 ] *= Scale->z ;
		BlendMatrix->m[ 1 ][ 0 ] *= Scale->x ;
		BlendMatrix->m[ 1 ][ 1 ] *= Scale->y ;
		BlendMatrix->m[ 1 ][ 2 ] *= Scale->z ;
		BlendMatrix->m[ 2 ][ 0 ] *= Scale->x ;
		BlendMatrix->m[ 2 ][ 1 ] *= Scale->y ;
		BlendMatrix->m[ 2 ][ 2 ] *= Scale->z ;
	}
}

// �A�j���[�V�����̍s����Z�b�g�A�b�v����
static void MV1SetupAnimMatrix( MV1_MODEL *Model )
{
	int i ;

	for( i = 0 ; i < Model->AnimSetMaxNum ; i ++ )
	{
		if( Model->AnimSet[ i ].Use == false || Model->AnimSet[ i ].AnimSet->ParamSetup ) continue ;
		MV1AnimSetSetupParam( Model->AnimSet[ i ].AnimSet ) ;
	}

	Model->AnimSetupFlag = true ;
}

// �`��p�̍s����\�z����
static void MV1SetupMatrix( MV1_MODEL *Model )
{
	int i, j, mcon ;
	MV1_FRAME *Frame ;
	MV1_MODEL_BASE *MBase ;
	MV1_FRAME_BASE *FrameBase ;
	MV1_MODEL_ANIM *MAnim, *MAnim2, *MAnim3 = NULL;
	VECTOR DivSize ;

	MBase = Model->BaseData ;

	// �s�񂪃Z�b�g�A�b�v������Ԃ̏ꍇ�͉��������ɏI��
	if( Model->LocalWorldMatrixSetupFlag == true )
		return ;

	// �|���S�����X�g�X�V�t���O��|��
	Model->SetupRefPolygon[ 1 ][ 0 ] = false ;
	Model->SetupRefPolygon[ 1 ][ 1 ] = false ;

	// �A�^�b�`���Ă���A�j���[�V�����̃p�����[�^�ɕω�������ꍇ�͍s����v�Z
	if( Model->AnimSetupFlag == false )
		MV1SetupAnimMatrix( Model ) ;

	// �g�b�v�̍X�V�`�F�b�N
	if( Model->ChangeMatrixFlag[ 0 ] & 1 )
	{
		// �X�P�[�����O���g�p���Ă��邩�ǂ����̃t���O��|��
		Model->LocalWorldMatrixUseScaling = false ;

		// �s�񂪗L���ȏꍇ�Ɩ����ȏꍇ�ŏ����𕪊�
		if( Model->ValidMatrix )
		{
			// �s�񂪗L���ȏꍇ�͂��̂܂܎g�p����
			Model->LocalWorldMatrix = Model->Matrix ;

			// �X�P�[�����O���g�p���Ă��邩�ǂ����̃t���O�𗧂Ă�
			Model->LocalWorldMatrixUseScaling = true ;
		}
		else
		{
			MATRIX_4X4CT TranslateMat, ScaleMat, RotateMat, TransformMat ;

			// �����ȏꍇ�̓g�����X�t�H�[���p�����[�^����s����쐬����
			CreateTranslationMatrix4X4CT( &TranslateMat, Model->Translation.x, Model->Translation.y, Model->Translation.z ) ;
			if( Model->Scale.x != 1.0f || Model->Scale.y != 1.0f || Model->Scale.z != 1.0f )
			{
				CreateScalingMatrix4X4CT( &ScaleMat, Model->Scale.x, Model->Scale.y, Model->Scale.z ) ;

				// �X�P�[�����O���g�p���Ă��邩�ǂ����̃t���O�𗧂Ă�
				Model->LocalWorldMatrixUseScaling = true ;
			}
			else
			{
				CreateIdentityMatrix4X4CTD( &ScaleMat ) ;
			}
			switch( Model->Rotation.Type )
			{
			case MV1_ROTATE_TYPE_XYZROT :
				CreateRotationXYZMatrix4X4CT( &RotateMat, Model->Rotation.XYZRot.x, Model->Rotation.XYZRot.y, Model->Rotation.XYZRot.z ) ;
				break ;

			case MV1_ROTATE_TYPE_QUATERNION :
				CreateQuaternionRotateMatrix4X4CT( &RotateMat, &Model->Rotation.Qt ) ;
				break ;

			case MV1_ROTATE_TYPE_MATRIX :
				ConvertMatrix4x4cFToMatrix4x4c( &RotateMat, &Model->Rotation.Mat ) ;

				// �X�P�[�����O���g�p���Ă��邩�ǂ����̃t���O�𗧂Ă�
				Model->LocalWorldMatrixUseScaling = true ;
				break ;

			case MV1_ROTATE_TYPE_ZAXIS :
				{
					MATRIX TempMatrix ;

					TempMatrix = MMult( MGetRotZ( Model->Rotation.ZAxis.Twist ), MGetAxis1( VCross( Model->Rotation.ZAxis.Up, Model->Rotation.ZAxis.Z ), Model->Rotation.ZAxis.Up, Model->Rotation.ZAxis.Z, VGet( 0.0f, 0.0f, 0.0f ) ) ) ;
					ConvertMatrixFToMatrix4x4c( &RotateMat, &TempMatrix ) ;
				}
				break ;
			}

			UnSafeMultiplyMatrix4X4CT( &TransformMat,            &ScaleMat,     &RotateMat ) ;
			UnSafeMultiplyMatrix4X4CT( &Model->LocalWorldMatrix, &TransformMat, &TranslateMat ) ;
		}
	}

	// �t���[���̍X�V�`�F�b�N
	Frame = Model->Frame ;
	MAnim = Model->Anim ;
	for( i = 0 ; i < MBase->FrameNum ; i ++, Frame ++, MAnim += Model->AnimSetMaxNum )
	{
		MATRIX_4X4CT *ParentMatrix ;
		bool ParentUseScaling ;

		// �������Z�ōs��𓱂��o���悤�ɂȂ��Ă���ꍇ�͉������Ȃ�
		if( Frame->PhysicsRigidBody != NULL ) continue ;

		// �X�V���K�v�Ȃ��ꍇ�͉������Ȃ�
		if( MV1CCHK( Frame->LocalWorldMatrixChange ) == 0 ) continue ;
		FrameBase = Frame->BaseData ;

		// �|���S�����X�g�X�V�t���O��|��
		Frame->SetupRefPolygon[ 1 ][ 0 ] = false ;
		Frame->SetupRefPolygon[ 1 ][ 1 ] = false ;

		// �X�P�[�����O���g�p���Ă��邩�ǂ����̃t���O��|��
		Frame->LocalWorldMatrixUseScaling = false ;

		// �e�s����Z�b�g����
		if( FrameBase->IgnoreParentTransform || Frame->Parent == NULL )
		{
			ParentMatrix = &Model->LocalWorldMatrix ;
			ParentUseScaling = Model->LocalWorldMatrixUseScaling ;
		}
		else
		{
			ParentMatrix = &Frame->Parent->LocalWorldMatrix ;
			ParentUseScaling = Frame->Parent->LocalWorldMatrixUseScaling ;
		}

		// ���[�U�[�w��̍s�񂪂���ꍇ�͂�����g�p����
		if( Frame->ValidUserLocalTransformMatrix )
		{
			UnSafeMultiplyMatrix4X4CT( &Frame->LocalWorldMatrix, &Frame->UserLocalTransformMatrix, ParentMatrix ) ;

			// �X�P�[�����O���g�p���Ă��邩�ǂ����̃t���O�𗧂Ă�
			Frame->LocalWorldMatrixUseScaling = true ;
		}
		else
		// �A�j���[�V����������ꍇ�Ɩ����ꍇ�ŏ����𕪊�
		if( Model->AnimSetNum == 0 )
		{
			// �A�j���[�V�������Ȃ��ꍇ
			UnSafeMultiplyMatrix4X4CT_FC( &Frame->LocalWorldMatrix, &FrameBase->LocalTransformMatrix, ParentMatrix ) ;

			// �e�̃t���[�����̓f�t�H���g�̐ݒ�ŃX�P�[�����O���g�p���Ă�����X�P�[�����O���g�p���Ă��邩�ǂ����̃t���O�𗧂Ă�
			if( FrameBase->LocalTransformMatrixUseScaling || ParentUseScaling )
			{
				Frame->LocalWorldMatrixUseScaling = true ;
			}
		}
		else
		{
			// �A�j���[�V����������ꍇ
			MATRIX_4X4CT_F BlendMat ;
			float BlendRate ;
			VECTOR Translate, Scale, Rotate ;
			FLOAT4 Quaternion ;
			int BlendFlag ;

			// �p�����[�^���x���̃u�����h���s���邩�𒲂ׂ�
			MAnim2 = MAnim ;
			BlendFlag = 0 ;
			mcon = 0 ;
			for( j = 0 ; j < Model->AnimSetMaxNum ; j ++, MAnim2 ++ )
			{
				if( MAnim2->Use == false || MAnim2->BlendRate == 0.0f )
					continue ;
				mcon ++ ;
				MAnim3 = MAnim2 ;

				BlendFlag |= MAnim2->Anim->ValidFlag ;
			}

			// �A�j���[�V�������Đ�����Ă��Ȃ��ꍇ�̓f�t�H���g�̍s���K������
			if( mcon == 0 )
			{
				if( FrameBase->LocalTransformMatrixType == 0 )
				{
					if( Frame->Parent == NULL )
					{
						Frame->LocalWorldMatrix = Model->LocalWorldMatrix ;
						Frame->LocalWorldMatrixUseScaling = Model->LocalWorldMatrixUseScaling ;
					}
					else
					{
						Frame->LocalWorldMatrix = Frame->Parent->LocalWorldMatrix ;
						Frame->LocalWorldMatrixUseScaling = Frame->Parent->LocalWorldMatrixUseScaling ;
					}
				}
				else
				if( FrameBase->LocalTransformMatrixType == 1 )
				{
					UnSafeTranslateOnlyMultiplyMatrix4X4CT_FC( &Frame->LocalWorldMatrix, &FrameBase->LocalTransformMatrix, ParentMatrix ) ;
					if( FrameBase->LocalTransformMatrixUseScaling || ParentUseScaling )
					{
						Frame->LocalWorldMatrixUseScaling = true ;
					}
				}
				else
				{
					UnSafeMultiplyMatrix4X4CT_FC( &Frame->LocalWorldMatrix, &FrameBase->LocalTransformMatrix, ParentMatrix ) ;
					if( FrameBase->LocalTransformMatrixUseScaling || ParentUseScaling )
					{
						Frame->LocalWorldMatrixUseScaling = true ;
					}
				}
			}
			else
			// �A�j���[�V������������Đ�����Ă���ꍇ�͕ʏ���
			if( mcon == 1 )
			{
				// �s��̃Z�b�g�A�b�v
				if( BlendFlag & MV1_ANIMVALUE_MATRIX )
				{
					// �e�t���[���̍s����|����
					UnSafeMultiplyMatrix4X4CT_FC( &Frame->LocalWorldMatrix, &MAnim3->Anim->Matrix, ParentMatrix ) ;

					Frame->LocalWorldMatrixUseScaling = true ;
				}
				else
				{
					// �e�t���[���̍s����|����
					if( MAnim3->Anim->ValidBlendMatrix == false )
					{
						MV1SetupTransformMatrix(
							&MAnim3->Anim->BlendMatrix,
							BlendFlag,
							&MAnim3->Anim->Translate,
							&MAnim3->Anim->Scale,
							MAnim3->Anim->RotateOrder,
							( FrameBase->Flag & MV1_FRAMEFLAG_PREROTATE ) != 0 ? &FrameBase->PreRotate : NULL,
							&MAnim3->Anim->Rotate,
							( FrameBase->Flag & MV1_FRAMEFLAG_POSTROTATE ) != 0 ? &FrameBase->PostRotate : NULL,
							&MAnim3->Anim->Quaternion
						) ;

						MAnim3->Anim->ValidBlendMatrix = true ;

						if( BlendFlag & MV1_ANIMVALUE_SCALE )
						{
							MAnim3->Anim->BlendMatrixUseScaling = true ;
						}
					}
					BlendMat = MAnim3->Anim->BlendMatrix ;

					if( MAnim3->Anim->BlendMatrixUseScaling )
					{
						Frame->LocalWorldMatrixUseScaling = true ;
					}

					/*
					NS_ErrorLogFmtAdd( "No:%d Flag:%d Trans:%.2f,%.2f,%.2f Scale:%.2f,%.2f,%.2f RotO:%d Rotate:%.2f,%.2f,%.2f Qt:%.2f,%.2f,%.2f,%.2f        Name:%s",
						i,
						BlendFlag,
						MAnim3->Anim->Translate.x, MAnim3->Anim->Translate.y, MAnim3->Anim->Translate.z, 
						MAnim3->Anim->Scale.x, MAnim3->Anim->Scale.y, MAnim3->Anim->Scale.z,
						MAnim3->Anim->RotateOrder,
						MAnim3->Anim->Rotate.x, MAnim3->Anim->Rotate.y, MAnim3->Anim->Rotate.z,
						MAnim3->Anim->Quaternion.x, MAnim3->Anim->Quaternion.y, MAnim3->Anim->Quaternion.z, MAnim3->Anim->Quaternion.w, 
						Frame->BaseData->Name ); 
					*/

					/*
					NS_ErrorLogFmtAdd( "no:%d Mat:%.2f,%.2f,%.2f,%.2f, %.2f,%.2f,%.2f,%.2f, %.2f,%.2f,%.2f,%.2f",
						i,
						BlendMat.m[0][0], BlendMat.m[0][1], BlendMat.m[0][2], BlendMat.m[0][3], 
						BlendMat.m[1][0], BlendMat.m[1][1], BlendMat.m[1][2], BlendMat.m[1][3], 
						BlendMat.m[2][0], BlendMat.m[2][1], BlendMat.m[2][2], BlendMat.m[2][3] );
					*/

					UnSafeMultiplyMatrix4X4CT_FC( &Frame->LocalWorldMatrix, &BlendMat, ParentMatrix ) ;
					//NS_ErrorLogFmtAdd( "no:%d proc:%d", i, 6 );
				}
			}
			else
			{
				MV1_ANIM * RST Anim ;

				// �s�񂪂��邩�A�N�H�[�^�j�I���Ƃw�x�y����]���������Ă��邩
				// �f�t�H���g�p�����[�^�������ȏ�ɓ��ĂĂ���A�j���[�V�����̎�ނ��Ⴄ�ꍇ�͍s��u�����h
//				if( ( BlendFlag & MV1_ANIMVALUE_MATRIX ) ||
//					( BlendFlag & ( MV1_ANIMVALUE_ROTATE | MV1_ANIMVALUE_QUATERNION_X ) ) == ( MV1_ANIMVALUE_ROTATE | MV1_ANIMVALUE_QUATERNION_X ) ||
//					( BlendFlag & ( MV1_ANIMVALUE_ROTATE | MV1_ANIMVALUE_QUATERNION_VMD ) ) == ( MV1_ANIMVALUE_ROTATE | MV1_ANIMVALUE_QUATERNION_VMD ) )
				if( 1 )
				{
					_MEMSET( &BlendMat, 0, sizeof( BlendMat ) ) ;
					MAnim2 = MAnim ;
					for( j = 0 ; j < Model->AnimSetMaxNum ; j ++, MAnim2 ++ )
					{
						if( MAnim2->Use == false || MAnim2->BlendRate == 0.0f ) continue ;

//						BlendRate = Model->AnimSet[ j ].BlendRate ;
						BlendRate = MAnim2->BlendRate ;
						Anim = MAnim2->Anim ;

						if( Anim->ValidFlag & MV1_ANIMVALUE_MATRIX )
						{
							if( BlendRate == 1.0f )
							{
								UnSafeMatrix4X4CT_F_EqPlus_F_Sub_F( &BlendMat, &Anim->Matrix, &FrameBase->LocalTransformMatrix ) ;
							}
							else
							{
								UnSafeMatrix4X4CT_F_EqPlus_F_Sub_F_Mul_S( &BlendMat, &Anim->Matrix, &FrameBase->LocalTransformMatrix, BlendRate ) ;
							}

							Frame->LocalWorldMatrixUseScaling = true ;
						}
						else
						{
							// �s��̃Z�b�g�A�b�v
							if( Anim->ValidBlendMatrix == false )
							{
								MV1SetupTransformMatrix(
									&Anim->BlendMatrix,
									Anim->ValidFlag,
									&Anim->Translate,
									&Anim->Scale,
									Anim->RotateOrder,
									( FrameBase->Flag & MV1_FRAMEFLAG_PREROTATE ) != 0 ? &FrameBase->PreRotate : NULL,
									&Anim->Rotate,
									( FrameBase->Flag & MV1_FRAMEFLAG_POSTROTATE ) != 0 ? &FrameBase->PostRotate : NULL,
									&Anim->Quaternion ) ;
								Anim->ValidBlendMatrix = true ;

								if( ( Anim->ValidFlag & MV1_ANIMVALUE_SCALE ) &&
									( Anim->Scale.x < 0.9999999f || Anim->Scale.x > 1.0000001f ||
									  Anim->Scale.y < 0.9999999f || Anim->Scale.y > 1.0000001f ||
									  Anim->Scale.z < 0.9999999f || Anim->Scale.z > 1.0000001f ) )
								{
									Anim->BlendMatrixUseScaling = true ;
								}
							}

							if( Anim->BlendMatrixUseScaling )
							{
								Frame->LocalWorldMatrixUseScaling = true ;
							}

							if( BlendRate == 1.0f )
							{
								UnSafeMatrix4X4CT_F_EqPlus_F_Sub_F( &BlendMat, &Anim->BlendMatrix, &FrameBase->LocalTransformMatrix ) ;
							}
							else
							{
								UnSafeMatrix4X4CT_F_EqPlus_F_Sub_F_Mul_S( &BlendMat, &Anim->BlendMatrix, &FrameBase->LocalTransformMatrix, BlendRate ) ;
							}
						}
					}

					UnSafeMatrix4X4CT_F_EqPlus_F( &BlendMat, &FrameBase->LocalTransformMatrix ) ;

					if( FrameBase->LocalTransformMatrixUseScaling )
					{
						Frame->LocalWorldMatrixUseScaling = true ;
					}

					// �X�P�[�����O���g�p����Ă��Ȃ�������s��̉�]�����̐��K��
					if( Frame->LocalWorldMatrixUseScaling == false )
					{
						DivSize.x = 1.0f / _SQRT( BlendMat.m[ 0 ][ 0 ] * BlendMat.m[ 0 ][ 0 ] + BlendMat.m[ 0 ][ 1 ] * BlendMat.m[ 0 ][ 1 ] + BlendMat.m[ 0 ][ 2 ] * BlendMat.m[ 0 ][ 2 ] ) ;
						DivSize.y = 1.0f / _SQRT( BlendMat.m[ 1 ][ 0 ] * BlendMat.m[ 1 ][ 0 ] + BlendMat.m[ 1 ][ 1 ] * BlendMat.m[ 1 ][ 1 ] + BlendMat.m[ 1 ][ 2 ] * BlendMat.m[ 1 ][ 2 ] ) ;
						DivSize.z = 1.0f / _SQRT( BlendMat.m[ 2 ][ 0 ] * BlendMat.m[ 2 ][ 0 ] + BlendMat.m[ 2 ][ 1 ] * BlendMat.m[ 2 ][ 1 ] + BlendMat.m[ 2 ][ 2 ] * BlendMat.m[ 2 ][ 2 ] ) ;

						BlendMat.m[ 0 ][ 0 ] *= DivSize.x ;
						BlendMat.m[ 0 ][ 1 ] *= DivSize.x ;
						BlendMat.m[ 0 ][ 2 ] *= DivSize.x ;

						BlendMat.m[ 1 ][ 0 ] *= DivSize.y ;
						BlendMat.m[ 1 ][ 1 ] *= DivSize.y ;
						BlendMat.m[ 1 ][ 2 ] *= DivSize.y ;

						BlendMat.m[ 2 ][ 0 ] *= DivSize.z ;
						BlendMat.m[ 2 ][ 1 ] *= DivSize.z ;
						BlendMat.m[ 2 ][ 2 ] *= DivSize.z ;
					}
				}
				else
				{
					// ����ȊO�̏ꍇ�̓p�����[�^���x���̃u�����h����
					Translate.x = 0.0f ;
					Translate.y = 0.0f ;
					Translate.z = 0.0f ;
					Scale.x = 0.0f ;
					Scale.y = 0.0f ;
					Scale.z = 0.0f ;
					Rotate.x = 0.0f ;
					Rotate.y = 0.0f ;
					Rotate.z = 0.0f ;
					Quaternion.x = 0.0f ;
					Quaternion.y = 0.0f ;
					Quaternion.z = 0.0f ;
					Quaternion.w = 1.0f ;

					MAnim2 = MAnim ;
					for( j = 0 ; j < Model->AnimSetMaxNum ; j ++, MAnim2 ++ )
					{
						if( MAnim2->Use == false || MAnim2->BlendRate == 0.0f ) continue ;

						BlendRate = MAnim2->BlendRate ;
						Anim = MAnim2->Anim ;

						if( BlendRate == 1.0f )
						{
							if( Anim->ValidFlag & MV1_ANIMVALUE_TRANSLATE )
							{
								Translate.x += Anim->Translate.x - ( float )FrameBase->Translate.x ;
								Translate.y += Anim->Translate.y - ( float )FrameBase->Translate.y ;
								Translate.z += Anim->Translate.z - ( float )FrameBase->Translate.z ;
							}

							if( Anim->ValidFlag & MV1_ANIMVALUE_SCALE )
							{
								Scale.x += Anim->Scale.x - FrameBase->Scale.x ;
								Scale.y += Anim->Scale.y - FrameBase->Scale.y ;
								Scale.z += Anim->Scale.z - FrameBase->Scale.z ;
							}

							if( Anim->ValidFlag & MV1_ANIMVALUE_ROTATE )
							{
								Rotate.x += Anim->Rotate.x - FrameBase->Rotate.x ;
								Rotate.y += Anim->Rotate.y - FrameBase->Rotate.y ;
								Rotate.z += Anim->Rotate.z - FrameBase->Rotate.z ;
							}

							if( Anim->ValidFlag & ( MV1_ANIMVALUE_QUATERNION_X | MV1_ANIMVALUE_QUATERNION_VMD ) )
							{
								Quaternion.x = Anim->Quaternion.x - FrameBase->Quaternion.x ;
								Quaternion.y = Anim->Quaternion.y - FrameBase->Quaternion.y ;
								Quaternion.z = Anim->Quaternion.z - FrameBase->Quaternion.z ;
								Quaternion.w = Anim->Quaternion.w - FrameBase->Quaternion.w ;
							}
						}
						else
						{
							if( Anim->ValidFlag & MV1_ANIMVALUE_TRANSLATE )
							{
								Translate.x += ( Anim->Translate.x - ( float )FrameBase->Translate.x ) * BlendRate ;
								Translate.y += ( Anim->Translate.y - ( float )FrameBase->Translate.y ) * BlendRate ;
								Translate.z += ( Anim->Translate.z - ( float )FrameBase->Translate.z ) * BlendRate ;
							}

							if( Anim->ValidFlag & MV1_ANIMVALUE_SCALE )
							{
								Scale.x += ( Anim->Scale.x - FrameBase->Scale.x ) * BlendRate ;
								Scale.y += ( Anim->Scale.y - FrameBase->Scale.y ) * BlendRate ;
								Scale.z += ( Anim->Scale.z - FrameBase->Scale.z ) * BlendRate ;
							}

							if( Anim->ValidFlag & MV1_ANIMVALUE_ROTATE )
							{
								Rotate.x += ( Anim->Rotate.x - FrameBase->Rotate.x ) * BlendRate ;
								Rotate.y += ( Anim->Rotate.y - FrameBase->Rotate.y ) * BlendRate ;
								Rotate.z += ( Anim->Rotate.z - FrameBase->Rotate.z ) * BlendRate ;
							}

							if( Anim->ValidFlag & ( MV1_ANIMVALUE_QUATERNION_X | MV1_ANIMVALUE_QUATERNION_VMD ) )
							{
								Quaternion.x = ( Anim->Quaternion.x - FrameBase->Quaternion.x ) * BlendRate ;
								Quaternion.y = ( Anim->Quaternion.y - FrameBase->Quaternion.y ) * BlendRate ;
								Quaternion.z = ( Anim->Quaternion.z - FrameBase->Quaternion.z ) * BlendRate ;
								Quaternion.w = ( Anim->Quaternion.w - FrameBase->Quaternion.w ) * BlendRate ;
							}
						}
					}

					Translate.x += ( float )FrameBase->Translate.x ;
					Translate.y += ( float )FrameBase->Translate.y ;
					Translate.z += ( float )FrameBase->Translate.z ;

					Scale.x += FrameBase->Scale.x ;
					Scale.y += FrameBase->Scale.y ;
					Scale.z += FrameBase->Scale.z ;

					Rotate.x += FrameBase->Rotate.x ;
					Rotate.y += FrameBase->Rotate.y ;
					Rotate.z += FrameBase->Rotate.z ;

					Quaternion.x += FrameBase->Quaternion.x ;
					Quaternion.y += FrameBase->Quaternion.y ;
					Quaternion.z += FrameBase->Quaternion.z ;
					Quaternion.w += FrameBase->Quaternion.w ;

					// �s��̃Z�b�g�A�b�v
					MV1SetupTransformMatrix(
						&BlendMat,
						BlendFlag,
						&Translate,
						&Scale,
						FrameBase->RotateOrder,
						( FrameBase->Flag & MV1_FRAMEFLAG_PREROTATE ) != 0 ? &FrameBase->PreRotate : NULL,
						&Rotate,
						( FrameBase->Flag & MV1_FRAMEFLAG_POSTROTATE ) != 0 ? &FrameBase->PostRotate : NULL,
						&Quaternion
					) ;

					if( BlendFlag & MV1_ANIMVALUE_SCALE )
					{
						Frame->LocalWorldMatrixUseScaling = true ;
					}
				}

				// �e�t���[���̍s����|����
				UnSafeMultiplyMatrix4X4CT_FC( &Frame->LocalWorldMatrix, &BlendMat, ParentMatrix ) ;
				if( ParentUseScaling )
				{
					Frame->LocalWorldMatrixUseScaling = true ;
				}
			}
		}

		// �Œ�@�\�p�C�v���C���p�s��𖳌��ɂ���
		Frame->ValidLocalWorldMatrixNM = false ;

		// ���̃t���[����ΏۂƂ��Ă���{�[��������ꍇ�́A���̃{�[���̍s����X�V����
		SetupSkiningBoneMatrix( *Model, *Frame ) ;

		//NS_ErrorLogFmtAdd( "no:%d parent:%d", i, Frame->Parent != NULL ? ( Frame - Model->Frame ) - ( Frame->Parent - Model->Frame ) : -1 );

		/*
		NS_ErrorLogFmtAdd( "no:%d Mat:%.2f,%.2f,%.2f,%.2f, %.2f,%.2f,%.2f,%.2f, %.2f,%.2f,%.2f,%.2f",
			i,
			Frame->LocalWorldMatrix.m[0][0], Frame->LocalWorldMatrix.m[0][1], Frame->LocalWorldMatrix.m[0][2], Frame->LocalWorldMatrix.m[0][3], 
			Frame->LocalWorldMatrix.m[1][0], Frame->LocalWorldMatrix.m[1][1], Frame->LocalWorldMatrix.m[1][2], Frame->LocalWorldMatrix.m[1][3], 
			Frame->LocalWorldMatrix.m[2][0], Frame->LocalWorldMatrix.m[2][1], Frame->LocalWorldMatrix.m[2][2], Frame->LocalWorldMatrix.m[2][3] );
		*/
	}
/*
	for( i = 0; i < Model->BaseData->SkinBoneNum ; i ++ )
	{
		NS_ErrorLogFmtAdd( "Mat%d:%.2f,%.2f,%.2f,%.2f, %.2f,%.2f,%.2f,%.2f, %.2f,%.2f,%.2f,%.2f",
			i,
			Model->SkinBoneMatrix[i].m[0][0], Model->SkinBoneMatrix[i].m[0][1], Model->SkinBoneMatrix[i].m[0][2], Model->SkinBoneMatrix[i].m[0][3], 
			Model->SkinBoneMatrix[i].m[1][0], Model->SkinBoneMatrix[i].m[1][1], Model->SkinBoneMatrix[i].m[1][2], Model->SkinBoneMatrix[i].m[1][3], 
			Model->SkinBoneMatrix[i].m[2][0], Model->SkinBoneMatrix[i].m[2][1], Model->SkinBoneMatrix[i].m[2][2], Model->SkinBoneMatrix[i].m[2][3] );
	}
*/
	// �X�V���K�v�t���O�����Z�b�g����
	_MEMSET( Model->ChangeMatrixFlag, 0, Model->ChangeMatrixFlagSize ) ;

	// ���f�����̍s��Z�b�g�A�b�v�����t���O�𗧂Ă�
	Model->LocalWorldMatrixSetupFlag = true ;
}



#define MV1MATCOLOR_UPDATE( PARENT, CHILD )	\
	if( ( PARENT ).UseColorScale )\
	{\
		if( ( CHILD )->DrawMaterial.UseColorScale )\
		{\
			( CHILD )->SetupDrawMaterial.DiffuseScale.r = ( ( CHILD )->DrawMaterial.DiffuseScale.r * ( PARENT ).DiffuseScale.r ) ;\
			( CHILD )->SetupDrawMaterial.DiffuseScale.g = ( ( CHILD )->DrawMaterial.DiffuseScale.g * ( PARENT ).DiffuseScale.g ) ;\
			( CHILD )->SetupDrawMaterial.DiffuseScale.b = ( ( CHILD )->DrawMaterial.DiffuseScale.b * ( PARENT ).DiffuseScale.b ) ;\
			( CHILD )->SetupDrawMaterial.DiffuseScale.a = ( ( CHILD )->DrawMaterial.DiffuseScale.a * ( PARENT ).DiffuseScale.a ) ;\
			( CHILD )->SetupDrawMaterial.AmbientScale.r = ( ( CHILD )->DrawMaterial.AmbientScale.r * ( PARENT ).AmbientScale.r ) ;\
			( CHILD )->SetupDrawMaterial.AmbientScale.g = ( ( CHILD )->DrawMaterial.AmbientScale.g * ( PARENT ).AmbientScale.g ) ;\
			( CHILD )->SetupDrawMaterial.AmbientScale.b = ( ( CHILD )->DrawMaterial.AmbientScale.b * ( PARENT ).AmbientScale.b ) ;\
			( CHILD )->SetupDrawMaterial.AmbientScale.a = ( ( CHILD )->DrawMaterial.AmbientScale.a * ( PARENT ).AmbientScale.a ) ;\
			( CHILD )->SetupDrawMaterial.SpecularScale.r = ( ( CHILD )->DrawMaterial.SpecularScale.r * ( PARENT ).SpecularScale.r ) ;\
			( CHILD )->SetupDrawMaterial.SpecularScale.g = ( ( CHILD )->DrawMaterial.SpecularScale.g * ( PARENT ).SpecularScale.g ) ;\
			( CHILD )->SetupDrawMaterial.SpecularScale.b = ( ( CHILD )->DrawMaterial.SpecularScale.b * ( PARENT ).SpecularScale.b ) ;\
			( CHILD )->SetupDrawMaterial.SpecularScale.a = ( ( CHILD )->DrawMaterial.SpecularScale.a * ( PARENT ).SpecularScale.a ) ;\
			( CHILD )->SetupDrawMaterial.EmissiveScale.r = ( ( CHILD )->DrawMaterial.EmissiveScale.r * ( PARENT ).EmissiveScale.r ) ;\
			( CHILD )->SetupDrawMaterial.EmissiveScale.g = ( ( CHILD )->DrawMaterial.EmissiveScale.g * ( PARENT ).EmissiveScale.g ) ;\
			( CHILD )->SetupDrawMaterial.EmissiveScale.b = ( ( CHILD )->DrawMaterial.EmissiveScale.b * ( PARENT ).EmissiveScale.b ) ;\
			( CHILD )->SetupDrawMaterial.EmissiveScale.a = ( ( CHILD )->DrawMaterial.EmissiveScale.a * ( PARENT ).EmissiveScale.a ) ;\
		}\
		else\
		{\
			( CHILD )->SetupDrawMaterial.DiffuseScale = ( PARENT ).DiffuseScale ;\
			( CHILD )->SetupDrawMaterial.AmbientScale = ( PARENT ).AmbientScale ;\
			( CHILD )->SetupDrawMaterial.SpecularScale = ( PARENT ).SpecularScale ;\
			( CHILD )->SetupDrawMaterial.EmissiveScale = ( PARENT ).EmissiveScale ;\
		}\
		( CHILD )->SetupDrawMaterial.UseColorScale = true ;\
	}\
	else\
	{\
		if( ( CHILD )->DrawMaterial.UseColorScale )\
		{\
			( CHILD )->SetupDrawMaterial.DiffuseScale = ( CHILD )->DrawMaterial.DiffuseScale ;\
			( CHILD )->SetupDrawMaterial.AmbientScale = ( CHILD )->DrawMaterial.AmbientScale ;\
			( CHILD )->SetupDrawMaterial.SpecularScale = ( CHILD )->DrawMaterial.SpecularScale ;\
			( CHILD )->SetupDrawMaterial.EmissiveScale = ( CHILD )->DrawMaterial.EmissiveScale ;\
		}\
		( CHILD )->SetupDrawMaterial.UseColorScale = ( CHILD )->DrawMaterial.UseColorScale ;\
	}\
\
	( CHILD )->SetupDrawMaterial.OpacityRate = ( CHILD )->DrawMaterial.OpacityRate * ( PARENT ).OpacityRate ;\
	( CHILD )->SetupDrawMaterial.Visible     = ( BYTE )( ( CHILD )->DrawMaterial.Visible != 0 && ( PARENT ).Visible != 0 ? 1 : 0 ) ;

// �`��p�̃}�e���A�������\�z����( ���Ń��b�V�����̂̍X�V�`�F�b�N�͂��܂��� )
static void MV1SetupDrawMaterial( MV1_FRAME *Frame, MV1_MESH *Mesh )
{
	int StackNum, i ;
	MV1_MODEL *Model ;
	MV1_FRAME *TopFrame, *TempFrame, *ParentFrame, *StackFrame[ 1024 ] ;
	MV1_MESH *TempMesh ;
	char NextFlag[ 1024 ] ;

	if( Frame == NULL ) Frame = Mesh->Container ;
	Model = Frame->Container ;

	// �����ɉe��������ŏ㋉�t���[���̍X�V
	if( MV1CCHK( Frame->DrawMaterialChange ) != 0 )
	{
		// �g�b�v���X�V����Ă����炻������
		if( Model->ChangeDrawMaterialFlag[ 0 ] & 1 )
		{
			_MEMSET( Model->ChangeDrawMaterialFlag, 0, Model->ChangeDrawMaterialFlagSize ) ;

			StackFrame[ 0 ] = Model->Frame ;
			NextFlag[ 0 ] = 1 ;
		}
		else
		{
			for( TopFrame = Frame ; TopFrame->Parent && MV1CCHK( TopFrame->Parent->DrawMaterialChange ) ; TopFrame = TopFrame->Parent ){}

			MV1BitResetChange( &TopFrame->DrawMaterialChange ) ;

			StackFrame[ 0 ] = TopFrame ;
			NextFlag[ 0 ] = 0 ;
		}
		StackNum = 1 ;

		while( StackNum )
		{
			StackNum -- ;
			TempFrame = StackFrame[ StackNum ] ;

			if( TempFrame->Parent == NULL )
			{
				MV1MATCOLOR_UPDATE( Model->DrawMaterial, TempFrame )
			}
			else
			{
				ParentFrame = TempFrame->Parent ;
				MV1MATCOLOR_UPDATE( ParentFrame->SetupDrawMaterial, TempFrame )
			}
			TempFrame->SemiTransStateSetupFlag = false ;

			if( TempFrame->BaseData->MeshNum )
			{
				TempMesh = TempFrame->Mesh ;
				for( i = 0 ; i < TempFrame->BaseData->MeshNum ; i ++, TempMesh ++ )
				{
					MV1MATCOLOR_UPDATE( TempFrame->SetupDrawMaterial, TempMesh )
					TempMesh->SemiTransStateSetupFlag = false ;
				}
			}

			if( NextFlag[ StackNum ] && TempFrame->Next )
			{
				StackFrame[ StackNum ] = TempFrame->Next ;
				NextFlag[ StackNum ] = 1 ;
				StackNum ++ ;
			}

			if( TempFrame->Child )
			{
				StackFrame[ StackNum ] = TempFrame->Child ;
				NextFlag[ StackNum ] = 1 ;
				StackNum ++ ;
			}
		}
	}
	else
	{
		if( Mesh )
		{
			MV1MATCOLOR_UPDATE( Frame->SetupDrawMaterial, Mesh )
			MV1CRST( Mesh->DrawMaterialChange ) ;
			Mesh->SemiTransStateSetupFlag = false ;
		}
	}
}

// ��ԕύX�Ǘ��f�[�^�ɐݒ肳��Ă���Ώۃr�b�g�𗧂Ă�
static void MV1BitSetChange( MV1_CHANGE *Change )
{
	// ���Ƀr�b�g�������Ă����牽�����Ȃ�
	if( MV1CCHK( *Change ) ) return ;

	if( Change->BaseData->Fill )
	{
		DWORD i, Size ;
		DWORD *Target, *Src ;

		Target = Change->Target ;
		Src    = Change->BaseData->Fill ;
		Size   = Change->BaseData->Size ;
		for( i = 0 ; i < Size ; i ++ )
			Target[ i ] |= Src[ i ] ;
	}
	else
	{
		*Change->Target |= Change->BaseData->CheckBit ;
	}
}

// ��ԕύX�Ǘ��f�[�^�ɐݒ肳��Ă���Ώۃr�b�g��|��
static void MV1BitResetChange( MV1_CHANGE *Change )
{
	if( Change->BaseData->Fill )
	{
		DWORD i, Size ;
		DWORD *Target, *Src ;

		Target = Change->Target ;
		Src    = Change->BaseData->Fill ;
		Size   = Change->BaseData->Size ;
		for( i = 0 ; i < Size ; i ++ )
			Target[ i ] &= ~Src[ i ] ;
	}
	else
	{
		*Change->Target &= ~Change->BaseData->CheckBit ;
	}
}

// �`��p�}�e���A�����̃f�B�t���[�Y�J���[��ύX����
static bool MV1SetDrawMaterialDif( MV1_DRAW_MATERIAL *DrawMaterial, MV1_CHANGE *Change, COLOR_F Scale )
{
	// ���܂łƒl�������ꍇ�͉��������I��
	if( *( ( DWORD * )&Scale.r ) == *( ( DWORD * )&DrawMaterial->DiffuseScale.r ) &&
		*( ( DWORD * )&Scale.g ) == *( ( DWORD * )&DrawMaterial->DiffuseScale.g ) &&
		*( ( DWORD * )&Scale.b ) == *( ( DWORD * )&DrawMaterial->DiffuseScale.b ) &&
		*( ( DWORD * )&Scale.a ) == *( ( DWORD * )&DrawMaterial->DiffuseScale.a ) )
		return false ;

	// �S�Ă̗v�f�� 1.0f ���ǂ����ŏ����𕪊�
	if( *( ( DWORD * )&Scale.r ) == 0x3f800000 &&
		*( ( DWORD * )&Scale.g ) == 0x3f800000 &&
		*( ( DWORD * )&Scale.b ) == 0x3f800000 &&
		*( ( DWORD * )&Scale.a ) == 0x3f800000 )
	{
		// �f�B�t���[�Y�J���[�� 1.0f ����
		*( ( DWORD * )&DrawMaterial->DiffuseScale.r ) = 0x3f800000 ;
		*( ( DWORD * )&DrawMaterial->DiffuseScale.g ) = 0x3f800000 ;
		*( ( DWORD * )&DrawMaterial->DiffuseScale.b ) = 0x3f800000 ;
		*( ( DWORD * )&DrawMaterial->DiffuseScale.a ) = 0x3f800000 ;

		// ���̗v�f�� 1.0f �ȊO�����ׂ�
		if( *( ( DWORD * )&DrawMaterial->AmbientScale.r ) == 0x3f800000 &&
			*( ( DWORD * )&DrawMaterial->AmbientScale.g ) == 0x3f800000 &&
			*( ( DWORD * )&DrawMaterial->AmbientScale.b ) == 0x3f800000 &&
			*( ( DWORD * )&DrawMaterial->AmbientScale.a ) == 0x3f800000 &&
		    *( ( DWORD * )&DrawMaterial->SpecularScale.r ) == 0x3f800000 &&
			*( ( DWORD * )&DrawMaterial->SpecularScale.g ) == 0x3f800000 &&
			*( ( DWORD * )&DrawMaterial->SpecularScale.b ) == 0x3f800000 &&
			*( ( DWORD * )&DrawMaterial->SpecularScale.a ) == 0x3f800000 &&
		    *( ( DWORD * )&DrawMaterial->EmissiveScale.r ) == 0x3f800000 &&
			*( ( DWORD * )&DrawMaterial->EmissiveScale.g ) == 0x3f800000 &&
			*( ( DWORD * )&DrawMaterial->EmissiveScale.b ) == 0x3f800000 &&
			*( ( DWORD * )&DrawMaterial->EmissiveScale.a ) == 0x3f800000 )
		{
			// �J���[�X�P�[���͎g�p���Ă��Ȃ��A�ɂ���
			DrawMaterial->UseColorScale = false ;
		}
	}
	else
	{
		// �f�B�t�[�Y�J���[�ɒl����
		DrawMaterial->DiffuseScale = Scale ;

		// �J���[�X�P�[�����g�p���Ă���A�ɂ���
		DrawMaterial->UseColorScale = true ;
	}

	// �ύX�̈��t����
	if( Change ) MV1BitSetChange( Change ) ;

	return true ;
}

// �`��p�}�e���A�����̃A���r�G���g�J���[��ύX����
static bool MV1SetDrawMaterialAmb( MV1_DRAW_MATERIAL *DrawMaterial, MV1_CHANGE *Change, COLOR_F Scale )
{
	// ���܂łƒl�������ꍇ�͉��������I��
	if( *( ( DWORD * )&Scale.r ) == *( ( DWORD * )&DrawMaterial->AmbientScale.r ) &&
		*( ( DWORD * )&Scale.g ) == *( ( DWORD * )&DrawMaterial->AmbientScale.g ) &&
		*( ( DWORD * )&Scale.b ) == *( ( DWORD * )&DrawMaterial->AmbientScale.b ) &&
		*( ( DWORD * )&Scale.a ) == *( ( DWORD * )&DrawMaterial->AmbientScale.a ) )
		return false ;

	// �S�Ă̗v�f�� 1.0f ���ǂ����ŏ����𕪊�
	if( *( ( DWORD * )&Scale.r ) == 0x3f800000 &&
		*( ( DWORD * )&Scale.g ) == 0x3f800000 &&
		*( ( DWORD * )&Scale.b ) == 0x3f800000 &&
		*( ( DWORD * )&Scale.a ) == 0x3f800000 )
	{
		// �A���r�G���g�J���[�� 1.0f ����
		*( ( DWORD * )&DrawMaterial->AmbientScale.r ) = 0x3f800000 ;
		*( ( DWORD * )&DrawMaterial->AmbientScale.g ) = 0x3f800000 ;
		*( ( DWORD * )&DrawMaterial->AmbientScale.b ) = 0x3f800000 ;
		*( ( DWORD * )&DrawMaterial->AmbientScale.a ) = 0x3f800000 ;

		// ���̗v�f�� 1.0f �ȊO�����ׂ�
		if( *( ( DWORD * )&DrawMaterial->DiffuseScale.r ) == 0x3f800000 &&
			*( ( DWORD * )&DrawMaterial->DiffuseScale.g ) == 0x3f800000 &&
			*( ( DWORD * )&DrawMaterial->DiffuseScale.b ) == 0x3f800000 &&
			*( ( DWORD * )&DrawMaterial->DiffuseScale.a ) == 0x3f800000 &&
		    *( ( DWORD * )&DrawMaterial->SpecularScale.r ) == 0x3f800000 &&
			*( ( DWORD * )&DrawMaterial->SpecularScale.g ) == 0x3f800000 &&
			*( ( DWORD * )&DrawMaterial->SpecularScale.b ) == 0x3f800000 &&
			*( ( DWORD * )&DrawMaterial->SpecularScale.a ) == 0x3f800000 &&
		    *( ( DWORD * )&DrawMaterial->EmissiveScale.r ) == 0x3f800000 &&
			*( ( DWORD * )&DrawMaterial->EmissiveScale.g ) == 0x3f800000 &&
			*( ( DWORD * )&DrawMaterial->EmissiveScale.b ) == 0x3f800000 &&
			*( ( DWORD * )&DrawMaterial->EmissiveScale.a ) == 0x3f800000 )
		{
			// �J���[�X�P�[���͎g�p���Ă��Ȃ��A�ɂ���
			DrawMaterial->UseColorScale = false ;
		}
	}
	else
	{
		// �A���r�G���g�J���[�ɒl����
		DrawMaterial->AmbientScale = Scale ;

		// �J���[�X�P�[�����g�p���Ă���A�ɂ���
		DrawMaterial->UseColorScale = true ;
	}

	// �ύX�̈��t����
	if( Change ) MV1BitSetChange( Change ) ;

	return true ;
}

// �`��p�}�e���A�����̃X�y�L�����J���[��ύX����
static bool MV1SetDrawMaterialSpc( MV1_DRAW_MATERIAL *DrawMaterial, MV1_CHANGE *Change, COLOR_F Scale )
{
	// ���܂łƒl�������ꍇ�͉��������I��
	if( *( ( DWORD * )&Scale.r ) == *( ( DWORD * )&DrawMaterial->SpecularScale.r ) &&
		*( ( DWORD * )&Scale.g ) == *( ( DWORD * )&DrawMaterial->SpecularScale.g ) &&
		*( ( DWORD * )&Scale.b ) == *( ( DWORD * )&DrawMaterial->SpecularScale.b ) &&
		*( ( DWORD * )&Scale.a ) == *( ( DWORD * )&DrawMaterial->SpecularScale.a ) )
		return false ;

	// �S�Ă̗v�f�� 1.0f ���ǂ����ŏ����𕪊�
	if( *( ( DWORD * )&Scale.r ) == 0x3f800000 &&
		*( ( DWORD * )&Scale.g ) == 0x3f800000 &&
		*( ( DWORD * )&Scale.b ) == 0x3f800000 &&
		*( ( DWORD * )&Scale.a ) == 0x3f800000 )
	{
		// �X�y�L�����J���[�� 1.0f ����
		*( ( DWORD * )&DrawMaterial->SpecularScale.r ) = 0x3f800000 ;
		*( ( DWORD * )&DrawMaterial->SpecularScale.g ) = 0x3f800000 ;
		*( ( DWORD * )&DrawMaterial->SpecularScale.b ) = 0x3f800000 ;
		*( ( DWORD * )&DrawMaterial->SpecularScale.a ) = 0x3f800000 ;

		// ���̗v�f�� 1.0f �ȊO�����ׂ�
		if( *( ( DWORD * )&DrawMaterial->DiffuseScale.r ) == 0x3f800000 &&
			*( ( DWORD * )&DrawMaterial->DiffuseScale.g ) == 0x3f800000 &&
			*( ( DWORD * )&DrawMaterial->DiffuseScale.b ) == 0x3f800000 &&
			*( ( DWORD * )&DrawMaterial->DiffuseScale.a ) == 0x3f800000 &&
		    *( ( DWORD * )&DrawMaterial->AmbientScale.r ) == 0x3f800000 &&
			*( ( DWORD * )&DrawMaterial->AmbientScale.g ) == 0x3f800000 &&
			*( ( DWORD * )&DrawMaterial->AmbientScale.b ) == 0x3f800000 &&
			*( ( DWORD * )&DrawMaterial->AmbientScale.a ) == 0x3f800000 &&
		    *( ( DWORD * )&DrawMaterial->EmissiveScale.r ) == 0x3f800000 &&
			*( ( DWORD * )&DrawMaterial->EmissiveScale.g ) == 0x3f800000 &&
			*( ( DWORD * )&DrawMaterial->EmissiveScale.b ) == 0x3f800000 &&
			*( ( DWORD * )&DrawMaterial->EmissiveScale.a ) == 0x3f800000 )
		{
			// �J���[�X�P�[���͎g�p���Ă��Ȃ��A�ɂ���
			DrawMaterial->UseColorScale = false ;
		}
	}
	else
	{
		// �X�y�L�����J���[�ɒl����
		DrawMaterial->SpecularScale = Scale ;

		// �J���[�X�P�[�����g�p���Ă���A�ɂ���
		DrawMaterial->UseColorScale = true ;
	}

	// �ύX�̈��t����
	if( Change ) MV1BitSetChange( Change ) ;

	return true ;
}

// �`��p�}�e���A�����̃G�~�b�V�u�J���[��ύX����
static bool MV1SetDrawMaterialEmi( MV1_DRAW_MATERIAL *DrawMaterial, MV1_CHANGE *Change, COLOR_F Scale )
{
	// ���܂łƒl�������ꍇ�͉��������I��
	if( *( ( DWORD * )&Scale.r ) == *( ( DWORD * )&DrawMaterial->EmissiveScale.r ) &&
		*( ( DWORD * )&Scale.g ) == *( ( DWORD * )&DrawMaterial->EmissiveScale.g ) &&
		*( ( DWORD * )&Scale.b ) == *( ( DWORD * )&DrawMaterial->EmissiveScale.b ) &&
		*( ( DWORD * )&Scale.a ) == *( ( DWORD * )&DrawMaterial->EmissiveScale.a ) )
		return false ;

	// �S�Ă̗v�f�� 1.0f ���ǂ����ŏ����𕪊�
	if( *( ( DWORD * )&Scale.r ) == 0x3f800000 &&
		*( ( DWORD * )&Scale.g ) == 0x3f800000 &&
		*( ( DWORD * )&Scale.b ) == 0x3f800000 &&
		*( ( DWORD * )&Scale.a ) == 0x3f800000 )
	{
		// �G�~�b�V�u�J���[�� 1.0f ����
		*( ( DWORD * )&DrawMaterial->EmissiveScale.r ) = 0x3f800000 ;
		*( ( DWORD * )&DrawMaterial->EmissiveScale.g ) = 0x3f800000 ;
		*( ( DWORD * )&DrawMaterial->EmissiveScale.b ) = 0x3f800000 ;
		*( ( DWORD * )&DrawMaterial->EmissiveScale.a ) = 0x3f800000 ;

		// ���̗v�f�� 1.0f �ȊO�����ׂ�
		if( *( ( DWORD * )&DrawMaterial->DiffuseScale.r ) == 0x3f800000 &&
			*( ( DWORD * )&DrawMaterial->DiffuseScale.g ) == 0x3f800000 &&
			*( ( DWORD * )&DrawMaterial->DiffuseScale.b ) == 0x3f800000 &&
			*( ( DWORD * )&DrawMaterial->DiffuseScale.a ) == 0x3f800000 &&
		    *( ( DWORD * )&DrawMaterial->AmbientScale.r ) == 0x3f800000 &&
			*( ( DWORD * )&DrawMaterial->AmbientScale.g ) == 0x3f800000 &&
			*( ( DWORD * )&DrawMaterial->AmbientScale.b ) == 0x3f800000 &&
			*( ( DWORD * )&DrawMaterial->AmbientScale.a ) == 0x3f800000 &&
		    *( ( DWORD * )&DrawMaterial->SpecularScale.r ) == 0x3f800000 &&
			*( ( DWORD * )&DrawMaterial->SpecularScale.g ) == 0x3f800000 &&
			*( ( DWORD * )&DrawMaterial->SpecularScale.b ) == 0x3f800000 &&
			*( ( DWORD * )&DrawMaterial->SpecularScale.a ) == 0x3f800000 )
		{
			// �J���[�X�P�[���͎g�p���Ă��Ȃ��A�ɂ���
			DrawMaterial->UseColorScale = false ;
		}
	}
	else
	{
		// �G�~�b�V�u�J���[�ɒl����
		DrawMaterial->EmissiveScale = Scale ;

		// �J���[�X�P�[�����g�p���Ă���A�ɂ���
		DrawMaterial->UseColorScale = true ;
	}

	// �ύX�̈��t����
	if( Change ) MV1BitSetChange( Change ) ;

	return true ;
}

// �`��p�}�e���A�����̕s�����x��ύX����
static bool MV1SetDrawMaterialOpacityRate( MV1_DRAW_MATERIAL *DrawMaterial, MV1_CHANGE *Change, float Rate )
{
	// ���܂łƒl�������ꍇ�͉��������I��
	if( *( ( DWORD * )&Rate ) == *( ( DWORD * )&DrawMaterial->OpacityRate ) )
		return false ;

	// �l����
	DrawMaterial->OpacityRate = Rate ;

	// �ύX�̈��t����
	if( Change ) MV1BitSetChange( Change ) ;

	return true ;
}

// �`��p�}�e���A�����̕\���ݒ��ύX����
static bool MV1SetDrawMaterialVisible( MV1_DRAW_MATERIAL *DrawMaterial, MV1_CHANGE *Change, BYTE Visible )
{
	// ���܂łƒl�������ꍇ�͉��������I��
	if( Visible == DrawMaterial->Visible )
		return false ;

	// �l����
	DrawMaterial->Visible = Visible ;

	// �ύX�̈��t����
	if( Change ) MV1BitSetChange( Change ) ;

	return true ;
}

// �O���f�[�V�����摜���č쐬����
static int _MV1ReCreateGradationGraph( int GrHandle )
{
	DWORD i ;
	BASEIMAGE BaseImage ;
	DWORD *Dest, DestData ;
	LOADGRAPH_GPARAM GParam ;

	if( NS_CreateXRGB8ColorBaseImage( 256, 8, &BaseImage ) < 0 )
		return -1 ;

	Dest = ( DWORD * )BaseImage.GraphData ;
	for( i = 0 ; i < 256 ; i ++, Dest ++ )
	{
		DestData = i | ( i << 8 ) | ( i << 16 ) ;
		Dest[ 256 * 0 ] = DestData ;
		Dest[ 256 * 1 ] = DestData ;
		Dest[ 256 * 2 ] = DestData ;
		Dest[ 256 * 3 ] = DestData ;
		Dest[ 256 * 4 ] = DestData ;
		Dest[ 256 * 5 ] = DestData ;
		Dest[ 256 * 6 ] = DestData ;
		Dest[ 256 * 7 ] = DestData ;
	}

	Graphics_Image_InitLoadGraphGParam( &GParam ) ;
	GParam.CreateGraphGParam.NotUseTransColor = TRUE ;
	Graphics_Image_InitSetupGraphHandleGParam_Normal_NonDrawValid( &GParam.CreateGraphGParam.InitHandleGParam, 32, TRUE, FALSE ) ;
	Graphics_Image_CreateGraphFromGraphImage_UseGParam( &GParam, TRUE, GrHandle, &BaseImage, NULL, TRUE, FALSE, FALSE, FALSE ) ;

	NS_ReleaseBaseImage( &BaseImage ) ;

	return GrHandle ;
}




// �r�b�g�f�[�^���X�g������������( -1:���s  0:���� )
int InitBitList( BITLIST *BitList, int BitDepth, int DataNum, MEMINFO **FirstMem )
{
	BitList->BitDepth = BitDepth ;
	BitList->UnitSize = ( BitDepth + 7 ) / 8 + 1 ;
	BitList->DataNum = 0 ;
	BitList->MaxDataNum = DataNum ;

	BitList->Data = ADDMEMAREA( ( size_t )( ( BitList->UnitSize + 4 ) * DataNum ), FirstMem ) ;
	if( BitList->Data == NULL )
	{
		DXST_ERRORLOG_ADDUTF16LE( "\xd3\x30\xc3\x30\xc8\x30\xc7\x30\xfc\x30\xbf\x30\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�r�b�g�f�[�^���i�[���郁�����̊m�ۂɎ��s���܂���\n" @*/ ) ;
		return -1 ;
	}
	BitList->PressData = ( int * )( ( BYTE * )BitList->Data + BitList->UnitSize * DataNum ) ;

	// ����I��
	return 0 ;
}

// �r�b�g�f�[�^���X�g���̎w��̗v�f�̃r�b�g�𗧂Ă�
int SetBitList( BITLIST *BitList, int Index, int SetBitIndex )
{
	BYTE *Addr, *SetAddr, *FlagAddr ;
	int SetBit ;

	// �f�[�^�A�h���X�̌v�Z
	Addr = ( BYTE * )BitList->Data + BitList->UnitSize * Index ;

	// �r�b�g���Z�b�g����A�h���X�̌v�Z
	SetAddr = Addr + SetBitIndex / 8 ;

	// �t���O������A�h���X�̌v�Z
	FlagAddr = Addr + BitList->UnitSize - 1 ;

	// ���Ă�r�b�g���v�Z
	SetBit = 1 << ( SetBitIndex % 8 ) ;

	// ���܂ň�x�ł��f�[�^�������ꂽ���Ƃ�����������r�b�g�𗧂Ă�
	if( *FlagAddr == 0 )
	{
		*FlagAddr = 1 ;

		// �f�[�^�̐����C���N�������g
		BitList->DataNum ++ ;
	}

	// �r�b�g�𗧂Ă�
	*SetAddr |= SetBit ;

	// ����I��
	return 0 ;
}

// �r�b�g�f�[�^���X�g���̒l�̎�ނ𒊏o����( �o�͑��͏���������Ă���K�v�͖��� )( -1:���s  0:���� )
int PressBitList( BITLIST *BitListDest, BITLIST *BitListSrc, MEMINFO **FirstMem )
{
	int i, j, k, l, m, UnitSize, vnum ;
	BYTE *SrcData ;
	BYTE *DestData ;

	// ���o�����f�[�^���i�[����r�b�g�f�[�^���X�g�̏�����
	if( InitBitList( BitListDest, BitListSrc->BitDepth, BitListSrc->MaxDataNum, FirstMem ) == -1 )
		return -1 ;

	// ���o���̃f�[�^�̐������J��Ԃ�
	UnitSize = BitListSrc->UnitSize ;
	SrcData = ( BYTE * )BitListSrc->Data ;
	vnum = 0 ;
	for( i = 0 ; vnum < BitListSrc->DataNum ; i ++, SrcData += UnitSize )
	{
		// �g�p����Ă��Ȃ��f�[�^�̏ꍇ�͉������Ȃ�
		if( SrcData[ UnitSize - 1 ] == 0 )
		{
			BitListSrc->PressData[ i ] = -1 ;
			continue ;
		}
		vnum ++ ;

		// ���o��Ɋ��ɓ����f�[�^�����邩�ǂ����𒲂ׂ�
		DestData = ( BYTE * )BitListDest->Data ;
		for( j = 0 ; j < BitListDest->DataNum ; j ++, DestData += UnitSize )
		{
			for( k = 0 ; k < UnitSize - 1 && SrcData[ k ] == DestData[ k ] ; k ++ ){}
			if( k == UnitSize - 1 ) break ;
		}

		// ����������ǉ�
		if( j == BitListDest->DataNum )
		{
			DestData = ( BYTE * )BitListDest->Data + UnitSize * BitListDest->DataNum ;

			// �f�[�^���R�s�[����Ɠ����ɗ����Ă���r�b�g�̐���������
			m = 0 ;
			for( k = 0 ; k < UnitSize - 1 ; k ++ )
			{
				for( l = 0 ; l < 8 ; l ++ )
					if( SrcData[ k ] & ( 1 << l ) ) m ++ ;
				DestData[ k ] = SrcData[ k ] ;
			}

			// �r�b�g�̐���ۑ�
			DestData[ k ] = ( BYTE )m ;

			// �o�͐�f�[�^�̐��𑝂₷
			BitListDest->DataNum ++ ;
			BitListDest->PressData[ j ] = 1 ;
		}
		else
		{
			// ��������f�[�^�̐����C���N�������g
			BitListDest->PressData[ j ] ++ ;
		}

		// ���o�ԍ����Z�b�g
		BitListSrc->PressData[ i ] = j ;
	}

	// �I��
	return 0 ;
}

// �w��̃r�b�g�f�[�^�Ɉ�v����^�[�Q�b�g���̃f�[�^�C���f�b�N�X�𓾂�( -1:�G���[�A���͖�������  -1�ȊO:�f�[�^�C���f�b�N�X )
int SearchBitList( BITLIST *BitListTarget, void *Buffer )
{
	int i, j, vnum, UnitSize ;
	BYTE *TargetData ;

	// �w��̃f�[�^�Ɠ����f�[�^���Ώۂ̃r�b�g�f�[�^���X�g�ɂ��邩�ǂ������ׂ�
	UnitSize = BitListTarget->UnitSize ;
	TargetData = ( BYTE * )BitListTarget->Data ;
	vnum = BitListTarget->DataNum ;
	for( i = 0 ; vnum ; i ++, TargetData += UnitSize )
	{
		if( ( TargetData[ UnitSize - 1 ] & 1 ) == 0 ) continue ;
		vnum -- ;

		for( j = 0 ; j < UnitSize - 1 && TargetData[ j ] == *( ( BYTE * )Buffer + j ) ; j ++ ){}
		if( j == UnitSize - 1 )
		{
			vnum ++ ;
			break ;
		}
	}

	// ���ʂ�Ԃ�
	return vnum ? i : -1 ;
}

// ��̗v�f����v���Ă��邩�ǂ������擾����( 0:��v���Ă���  0:��v���Ă��Ȃ� )
int CmpBitList( BITLIST *BitList1, int Index1, BITLIST *BitList2, int Index2 )
{
	return _MEMCMP( ( BYTE * )BitList1->Data + BitList1->UnitSize * Index1,
					( BYTE * )BitList2->Data + BitList2->UnitSize * Index2,
					BitList1->UnitSize - 1 ) == 0 ? 0 : 1 ;
}

// �\�[�X���̎w��̃f�[�^�Ɉ�v����^�[�Q�b�g���̃f�[�^�C���f�b�N�X�𓾂�( -1:�G���[�A���͖�������  -1�ȊO:�f�[�^�C���f�b�N�X )
int SearchBitList( BITLIST *BitListTarget, BITLIST *BitListSrc, int SrcIndex )
{
	// �P�f�[�^�̃T�C�Y���������G���[
	if( BitListSrc->UnitSize != BitListTarget->UnitSize )
		return -1 ;

	// ���ׂ�
	return SearchBitList( BitListTarget, ( BYTE * )BitListSrc->Data + BitListSrc->UnitSize * SrcIndex ) ;
}


// �r�b�g�f�[�^���X�g����w��ԍ��̃f�[�^���擾����
int GetBitList( BITLIST *BitList, int Index, void *Buffer )
{
	int i ;
	BYTE *Dest, *Src ;

	Src  = ( BYTE * )BitList->Data + Index * BitList->UnitSize ;
	Dest = ( BYTE * )Buffer ;
	for( i = BitList->UnitSize - 2 ; i >= 0 ; i -- )
		Dest[ i ] = Src[ i ] ;

	*( ( BYTE * )Buffer + BitList->UnitSize - 1 ) = 0 ;

	// �I��
	return 0 ;
}

// �w��̃r�b�g�f�[�^���w��̃C���f�b�N�X�ɃR�s�[����
int CopyBitList( BITLIST *BitListDest, int Index, void *Buffer )
{
	BYTE *Dest ;
	int i ;

	// ���܂Ŏg�p����Ă��Ȃ������f�[�^�̏ꍇ�̓f�[�^���𑝂₷
	Dest = ( BYTE * )BitListDest->Data + BitListDest->UnitSize * Index ;
	if( ( Dest[ BitListDest->UnitSize - 1 ] & 1 ) == 0 )
	{
		BitListDest->DataNum ++ ;
	}

	// �f�[�^���R�s�[
	for( i = 0 ; i < BitListDest->UnitSize ; i ++ )
		Dest[ i ] = ( ( BYTE * )Buffer )[ i ] ;

	// �I��
	return 0 ;
}

// �C�ӂ̃r�b�g�f�[�^�ƃr�b�g�f�[�^���X�g���̎w��̃f�[�^�� Or ���Z����
int OrBitList( BITLIST *BitList, int Index, void *Buffer )
{
	int i ;
	BYTE *Dest, *Src ;

	Src  = ( BYTE * )BitList->Data + Index * BitList->UnitSize ;
	Dest = ( BYTE * )Buffer ;
	for( i = BitList->UnitSize - 2 ; i >= 0 ; i -- )
		Dest[ i ] |= Src[ i ] ;

	// �I��
	return 0 ;
}

// �w��̃r�b�g�f�[�^���A���r�b�g�����Ă��邩���ׂ�
int GetBitCount( void *Buffer, int UnitSize )
{
	int i, m ;

	m = 0 ;
	for( i = 0 ; i < UnitSize - 1 ; i ++, Buffer = ( BYTE * )Buffer + 1 )
	{
		if( *( ( BYTE * )Buffer ) & ( 1 << 0 ) ) m ++ ;
		if( *( ( BYTE * )Buffer ) & ( 1 << 1 ) ) m ++ ;
		if( *( ( BYTE * )Buffer ) & ( 1 << 2 ) ) m ++ ;
		if( *( ( BYTE * )Buffer ) & ( 1 << 3 ) ) m ++ ;
		if( *( ( BYTE * )Buffer ) & ( 1 << 4 ) ) m ++ ;
		if( *( ( BYTE * )Buffer ) & ( 1 << 5 ) ) m ++ ;
		if( *( ( BYTE * )Buffer ) & ( 1 << 6 ) ) m ++ ;
		if( *( ( BYTE * )Buffer ) & ( 1 << 7 ) ) m ++ ;
	}

	return m ;
}

// �w��̃r�b�g�f�[�^���A�^�[�Q�b�g���̃r�b�g�f�[�^�ɉ�����
int AddBitList( BITLIST *BitListDest, void *Buffer, int RefCount )
{
	int DestIndex, i, UnitSize ;
	BYTE *Dest ;

	UnitSize = BitListDest->UnitSize ;

	// ���ɂ��邩�ǂ������ׂ�
	DestIndex = SearchBitList( BitListDest, Buffer ) ;
	if( DestIndex == -1 )
	{
		// ����������V�K�ǉ�
		Dest = ( BYTE * )BitListDest->Data + UnitSize * BitListDest->DataNum ;
		for( i = 0 ; i < UnitSize ; i ++ )
			*( Dest + i ) = *( ( BYTE * )Buffer + i ) ;
		*( Dest + UnitSize - 1 ) |= 1 ;

		// �Q�Ɛ����Z�b�g
		BitListDest->PressData[ BitListDest->DataNum ] = RefCount ;

		// �f�[�^�̐����C���N�������g
		BitListDest->DataNum ++ ;
	}
	else
	{
		// ��������Q�Ɛ���ǉ�
		BitListDest->PressData[ DestIndex ] += RefCount ;
	}

	// �I��
	return 0 ;
}

// �\�[�X���̎w��̃r�b�g�f�[�^���A�^�[�Q�b�g���̃r�b�g�f�[�^�ɉ�����
int AddBitList( BITLIST *BitListDest, BITLIST *BitListSrc, int SrcIndex, int RefCount )
{
	// �f�[�^�T�C�Y���������G���[
	if( BitListDest->UnitSize != BitListSrc->UnitSize )
		return -1 ;

	// �ǉ�����
	return AddBitList( BitListDest, ( BYTE * )BitListSrc->Data + BitListSrc->UnitSize * SrcIndex, RefCount ) ;
}

// �w��̃r�b�g�f�[�^���ŗ����Ă���r�b�g�̃��X�g���擾����( �߂�l  -1:�G���[  0�ȏ�:�����Ă���r�b�g�̐� )
int GetBitListNumber( BITLIST *BitList, int Index, WORD *Buffer )
{
	BYTE *Src ;
	int i, ByteNum, BitNum, Num ;
	WORD BitCount ;

	Src = ( BYTE * )BitList->Data + Index * BitList->UnitSize ;
	ByteNum = BitList->BitDepth / 8 ;
	Num = 0 ;
	BitCount = 0 ;
	for( i = 0 ; i < ByteNum ; i ++, BitCount += 8, Src ++ )
	{
		if( *Src & 0x01 ){ Buffer[ Num ] = ( WORD )( BitCount + 0 ) ; Num ++ ; }
		if( *Src & 0x02 ){ Buffer[ Num ] = ( WORD )( BitCount + 1 ) ; Num ++ ; }
		if( *Src & 0x04 ){ Buffer[ Num ] = ( WORD )( BitCount + 2 ) ; Num ++ ; }
		if( *Src & 0x08 ){ Buffer[ Num ] = ( WORD )( BitCount + 3 ) ; Num ++ ; }
		if( *Src & 0x10 ){ Buffer[ Num ] = ( WORD )( BitCount + 4 ) ; Num ++ ; }
		if( *Src & 0x20 ){ Buffer[ Num ] = ( WORD )( BitCount + 5 ) ; Num ++ ; }
		if( *Src & 0x40 ){ Buffer[ Num ] = ( WORD )( BitCount + 6 ) ; Num ++ ; }
		if( *Src & 0x80 ){ Buffer[ Num ] = ( WORD )( BitCount + 7 ) ; Num ++ ; }
	}

	BitNum = BitList->BitDepth - ByteNum * 8 ;
	for( i = 0 ; i < BitNum ; i ++ )
	{
		if( *Src & ( 1 << i ) )
		{
			Buffer[ Num ] = ( WORD )( BitCount + i ) ;
			Num ++ ;
		}
	}

	// �I��
	return Num ;
}


// 16bit�⏕���֌W

// float�^�̒l���� MV1_ANIM_KEY_16BIT�\���̂� Min �ϐ��p�̒l���쐬����
extern BYTE MV1AnimKey16BitMinFtoB( float Min )
{
	float f ;

	// �ŏ��ɗ납�ǂ����𒲂ׂ�
	if( ( *( ( DWORD * )&Min ) & 0x7fffffff ) == 0 )
	{
		return 0x80;
	}
	else
	{
		BYTE ret ;

		// �߂�l��������
		ret = 0 ;

		// �������Z�b�g
		if( *( ( DWORD * )&Min ) & 0x80000000 )
		{
			ret |= 0x40 ;
			*( ( DWORD * )&Min ) &= 0x7fffffff ;
		}

		// �搔�����ɂ���ď����𕪊�
		if( Min < 1.0f )
		{
			// 1.0f �ȉ�
			ret |= 0x20 ;

			// �s�b�^���Ȓl�����邩���ׂ�
			f = 0.1f ;               if( *( ( DWORD * )&f ) == *( ( DWORD * )&Min ) ) return ( BYTE )( ret | 1 ) ;
			f = 0.01f ;              if( *( ( DWORD * )&f ) == *( ( DWORD * )&Min ) ) return ( BYTE )( ret | 2 ) ;
			f = 0.001f ;             if( *( ( DWORD * )&f ) == *( ( DWORD * )&Min ) ) return ( BYTE )( ret | 3 ) ;
			f = 0.0001f ;            if( *( ( DWORD * )&f ) == *( ( DWORD * )&Min ) ) return ( BYTE )( ret | 4 ) ;
			f = 0.00001f ;           if( *( ( DWORD * )&f ) == *( ( DWORD * )&Min ) ) return ( BYTE )( ret | 5 ) ;
			f = 0.000001f ;          if( *( ( DWORD * )&f ) == *( ( DWORD * )&Min ) ) return ( BYTE )( ret | 6 ) ;
			f = 0.0000001f ;         if( *( ( DWORD * )&f ) == *( ( DWORD * )&Min ) ) return ( BYTE )( ret | 7 ) ;
			f = 0.00000001f ;        if( *( ( DWORD * )&f ) == *( ( DWORD * )&Min ) ) return ( BYTE )( ret | 8 ) ;
			f = 0.000000001f ;       if( *( ( DWORD * )&f ) == *( ( DWORD * )&Min ) ) return ( BYTE )( ret | 9 ) ;
			f = 0.0000000001f ;      if( *( ( DWORD * )&f ) == *( ( DWORD * )&Min ) ) return ( BYTE )( ret | 10 ) ;
			f = 0.00000000001f ;     if( *( ( DWORD * )&f ) == *( ( DWORD * )&Min ) ) return ( BYTE )( ret | 11 ) ;
			f = 0.000000000001f ;    if( *( ( DWORD * )&f ) == *( ( DWORD * )&Min ) ) return ( BYTE )( ret | 12 ) ;
			f = 0.0000000000001f ;   if( *( ( DWORD * )&f ) == *( ( DWORD * )&Min ) ) return ( BYTE )( ret | 13 ) ;
			f = 0.00000000000001f ;  if( *( ( DWORD * )&f ) == *( ( DWORD * )&Min ) ) return ( BYTE )( ret | 14 ) ;
			f = 0.000000000000001f ; if( *( ( DWORD * )&f ) == *( ( DWORD * )&Min ) ) return ( BYTE )( ret | 15 ) ;

			// �}�C�i�X���ǂ����ŏ����𕪊�
			// ���̒l��菬�����l��ݒ肷��
			if( ret & 0x40 )
			{
				// �}�C�i�X�̏ꍇ
				if( Min < 0.00000000000001f && Min >= 0.000000000000001f ) return ( BYTE )( ret | 14 ) ;
				if( Min < 0.0000000000001f  && Min >= 0.00000000000001f  ) return ( BYTE )( ret | 13 ) ;
				if( Min < 0.000000000001f   && Min >= 0.0000000000001f   ) return ( BYTE )( ret | 12 ) ;
				if( Min < 0.00000000001f    && Min >= 0.000000000001f    ) return ( BYTE )( ret | 11 ) ;
				if( Min < 0.0000000001f     && Min >= 0.00000000001f     ) return ( BYTE )( ret | 10 ) ;
				if( Min < 0.000000001f      && Min >= 0.0000000001f      ) return ( BYTE )( ret | 9 ) ;
				if( Min < 0.00000001f       && Min >= 0.000000001f       ) return ( BYTE )( ret | 8 ) ;
				if( Min < 0.0000001f        && Min >= 0.00000001f        ) return ( BYTE )( ret | 7 ) ;
				if( Min < 0.000001f         && Min >= 0.0000001f         ) return ( BYTE )( ret | 6 ) ;
				if( Min < 0.00001f          && Min >= 0.000001f          ) return ( BYTE )( ret | 5 ) ;
				if( Min < 0.0001f           && Min >= 0.00001f           ) return ( BYTE )( ret | 4 ) ;
				if( Min < 0.001f            && Min >= 0.0001f            ) return ( BYTE )( ret | 3 ) ;
				if( Min < 0.01f             && Min >= 0.001f             ) return ( BYTE )( ret | 2 ) ;
				if( Min < 0.1f              && Min >= 0.01f              ) return ( BYTE )( ret | 1 ) ;
				if( Min < 1.0f              && Min >= 0.1f               ) return ( BYTE )( ret | 0 ) ;
				return ( BYTE )( ret | 0 ) ;
			}
			else
			{
				// �v���X�̏ꍇ
				if( Min < 0.00000000000001f && Min >= 0.000000000000001f ) return ( BYTE )( ret | 15 ) ;
				if( Min < 0.0000000000001f  && Min >= 0.00000000000001f  ) return ( BYTE )( ret | 14 ) ;
				if( Min < 0.000000000001f   && Min >= 0.0000000000001f   ) return ( BYTE )( ret | 13 ) ;
				if( Min < 0.00000000001f    && Min >= 0.000000000001f    ) return ( BYTE )( ret | 12 ) ;
				if( Min < 0.0000000001f     && Min >= 0.00000000001f     ) return ( BYTE )( ret | 11 ) ;
				if( Min < 0.000000001f      && Min >= 0.0000000001f      ) return ( BYTE )( ret | 10 ) ;
				if( Min < 0.00000001f       && Min >= 0.000000001f       ) return ( BYTE )( ret | 9 ) ;
				if( Min < 0.0000001f        && Min >= 0.00000001f        ) return ( BYTE )( ret | 8 ) ;
				if( Min < 0.000001f         && Min >= 0.0000001f         ) return ( BYTE )( ret | 7 ) ;
				if( Min < 0.00001f          && Min >= 0.000001f          ) return ( BYTE )( ret | 6 ) ;
				if( Min < 0.0001f           && Min >= 0.00001f           ) return ( BYTE )( ret | 5 ) ;
				if( Min < 0.001f            && Min >= 0.0001f            ) return ( BYTE )( ret | 4 ) ;
				if( Min < 0.01f             && Min >= 0.001f             ) return ( BYTE )( ret | 3 ) ;
				if( Min < 0.1f              && Min >= 0.01f              ) return ( BYTE )( ret | 2 ) ;
				if( Min < 1.0f              && Min >= 0.1f               ) return ( BYTE )( ret | 1 ) ;
				return 0x80 ;
			}
		}
		else
		{
			// �s�b�^���Ȓl�����邩���ׂ�
			f = 1.0f ;                if( *( ( DWORD * )&f ) == *( ( DWORD * )&Min ) ) return ( BYTE )( ret | 0 ) ;
			f = 10.0f ;               if( *( ( DWORD * )&f ) == *( ( DWORD * )&Min ) ) return ( BYTE )( ret | 1 ) ;
			f = 100.0f ;              if( *( ( DWORD * )&f ) == *( ( DWORD * )&Min ) ) return ( BYTE )( ret | 2 ) ;
			f = 1000.0f ;             if( *( ( DWORD * )&f ) == *( ( DWORD * )&Min ) ) return ( BYTE )( ret | 3 ) ;
			f = 10000.0f ;            if( *( ( DWORD * )&f ) == *( ( DWORD * )&Min ) ) return ( BYTE )( ret | 4 ) ;
			f = 100000.0f ;           if( *( ( DWORD * )&f ) == *( ( DWORD * )&Min ) ) return ( BYTE )( ret | 5 ) ;
			f = 1000000.0f ;          if( *( ( DWORD * )&f ) == *( ( DWORD * )&Min ) ) return ( BYTE )( ret | 6 ) ;
			f = 10000000.0f ;         if( *( ( DWORD * )&f ) == *( ( DWORD * )&Min ) ) return ( BYTE )( ret | 7 ) ;
			f = 100000000.0f ;        if( *( ( DWORD * )&f ) == *( ( DWORD * )&Min ) ) return ( BYTE )( ret | 8 ) ;
			f = 1000000000.0f ;       if( *( ( DWORD * )&f ) == *( ( DWORD * )&Min ) ) return ( BYTE )( ret | 9 ) ;
			f = 10000000000.0f ;      if( *( ( DWORD * )&f ) == *( ( DWORD * )&Min ) ) return ( BYTE )( ret | 10 ) ;
			f = 100000000000.0f ;     if( *( ( DWORD * )&f ) == *( ( DWORD * )&Min ) ) return ( BYTE )( ret | 11 ) ;
			f = 1000000000000.0f ;    if( *( ( DWORD * )&f ) == *( ( DWORD * )&Min ) ) return ( BYTE )( ret | 12 ) ;
			f = 10000000000000.0f ;   if( *( ( DWORD * )&f ) == *( ( DWORD * )&Min ) ) return ( BYTE )( ret | 13 ) ;
			f = 100000000000000.0f ;  if( *( ( DWORD * )&f ) == *( ( DWORD * )&Min ) ) return ( BYTE )( ret | 14 ) ;
			f = 1000000000000000.0f ; if( *( ( DWORD * )&f ) == *( ( DWORD * )&Min ) ) return ( BYTE )( ret | 15 ) ;

			// �}�C�i�X���ǂ����ŏ����𕪊�
			// ���̒l��菬�����l��ݒ肷��
			if( ret & 0x40 )
			{
				// �}�C�i�X�̏ꍇ
				if( Min < 1.0f                && Min >= 0.0f               ) return ( BYTE )( ret | 0 ) ;
				if( Min < 10.0f               && Min >= 1.0f               ) return ( BYTE )( ret | 1 ) ;
				if( Min < 100.0f              && Min >= 10.0f              ) return ( BYTE )( ret | 2 ) ;
				if( Min < 1000.0f             && Min >= 100.0f             ) return ( BYTE )( ret | 3 ) ;
				if( Min < 10000.0f            && Min >= 1000.0f            ) return ( BYTE )( ret | 4 ) ;
				if( Min < 100000.0f           && Min >= 10000.0f           ) return ( BYTE )( ret | 5 ) ;
				if( Min < 1000000.0f          && Min >= 100000.0f          ) return ( BYTE )( ret | 6 ) ;
				if( Min < 10000000.0f         && Min >= 1000000.0f         ) return ( BYTE )( ret | 7 ) ;
				if( Min < 100000000.0f        && Min >= 10000000.0f        ) return ( BYTE )( ret | 8 ) ;
				if( Min < 1000000000.0f       && Min >= 100000000.0f       ) return ( BYTE )( ret | 9 ) ;
				if( Min < 10000000000.0f      && Min >= 1000000000.0f      ) return ( BYTE )( ret | 10 ) ;
				if( Min < 100000000000.0f     && Min >= 10000000000.0f     ) return ( BYTE )( ret | 11 ) ;
				if( Min < 1000000000000.0f    && Min >= 100000000000.0f    ) return ( BYTE )( ret | 12 ) ;
				if( Min < 10000000000000.0f   && Min >= 1000000000000.0f   ) return ( BYTE )( ret | 13 ) ;
				if( Min < 100000000000000.0f  && Min >= 10000000000000.0f  ) return ( BYTE )( ret | 14 ) ;
				if( Min < 1000000000000000.0f && Min >= 100000000000000.0f ) return ( BYTE )( ret | 15 ) ;
				return ( BYTE )( ret | 15 ) ;
			}
			else
			{
				// �v���X�̏ꍇ
				if( Min < 1.0f                 && Min >= 0.0f                ) return 0x80 ;
				if( Min < 10.0f                && Min >= 1.0f                ) return ( BYTE )( ret | 0 ) ;
				if( Min < 100.0f               && Min >= 10.0f               ) return ( BYTE )( ret | 1 ) ;
				if( Min < 1000.0f              && Min >= 100.0f              ) return ( BYTE )( ret | 2 ) ;
				if( Min < 10000.0f             && Min >= 1000.0f             ) return ( BYTE )( ret | 3 ) ;
				if( Min < 100000.0f            && Min >= 10000.0f            ) return ( BYTE )( ret | 4 ) ;
				if( Min < 1000000.0f           && Min >= 100000.0f           ) return ( BYTE )( ret | 5 ) ;
				if( Min < 10000000.0f          && Min >= 1000000.0f          ) return ( BYTE )( ret | 6 ) ;
				if( Min < 100000000.0f         && Min >= 10000000.0f         ) return ( BYTE )( ret | 7 ) ;
				if( Min < 1000000000.0f        && Min >= 100000000.0f        ) return ( BYTE )( ret | 8 ) ;
				if( Min < 10000000000.0f       && Min >= 1000000000.0f       ) return ( BYTE )( ret | 9 ) ;
				if( Min < 100000000000.0f      && Min >= 10000000000.0f      ) return ( BYTE )( ret | 10 ) ;
				if( Min < 1000000000000.0f     && Min >= 100000000000.0f     ) return ( BYTE )( ret | 11 ) ;
				if( Min < 10000000000000.0f    && Min >= 1000000000000.0f    ) return ( BYTE )( ret | 12 ) ;
				if( Min < 100000000000000.0f   && Min >= 10000000000000.0f   ) return ( BYTE )( ret | 13 ) ;
				if( Min < 1000000000000000.0f  && Min >= 100000000000000.0f  ) return ( BYTE )( ret | 14 ) ;
				if( Min < 10000000000000000.0f && Min >= 1000000000000000.0f ) return ( BYTE )( ret | 15 ) ;
				return ( BYTE )( ret | 15 ) ;
			}
		}
	}
}

// float�^�̒l���� MV1_ANIM_KEY_16BIT�\���̂� Unit �ϐ��p�̒l���쐬����
extern BYTE MV1AnimKey16BitUnitFtoB( float Unit )
{
	// �}�C�i�X��������G���[
	if( Unit < 0.0f )
	{
		return 0 ;
	}

	// �ŏ��Ƀ[�����ǂ������ׂ�
	if( * ( ( DWORD * )&Unit ) == 0.0f )
	{
		return 0 ;
	}
	else
	{
		int i, ret, mb, mi ;
		float f, mf ;

		// �s�b�^���̒l�����������ׂ�
		mf = 1000000000000.0f ;
		mb = -1 ;
		mi = -1 ;
		if( Unit < 1.0f )
		{
			ret = 0x80 ;

			// �����_�ȉ�
			for( i = 1 ; i < 15 ; i ++ ){ f = i / 10.0f       - Unit ; if( f > -0.0000000001f   && f < 0.0000000001f   ) return ( BYTE )( ret | ( 1 << 4 ) | i ) ; if( f > 0.0f && mf > f ){ mi = i ; mb = 1 ; mf = f ; } }
			for( i = 1 ; i < 15 ; i ++ ){ f = i / 100.0f      - Unit ; if( f > -0.0000000001f   && f < 0.0000000001f   ) return ( BYTE )( ret | ( 2 << 4 ) | i ) ; if( f > 0.0f && mf > f ){ mi = i ; mb = 2 ; mf = f ; } }
			for( i = 1 ; i < 15 ; i ++ ){ f = i / 1000.0f     - Unit ; if( f > -0.00000000001f  && f < 0.00000000001f  ) return ( BYTE )( ret | ( 3 << 4 ) | i ) ; if( f > 0.0f && mf > f ){ mi = i ; mb = 3 ; mf = f ; } }
			for( i = 1 ; i < 15 ; i ++ ){ f = i / 10000.0f    - Unit ; if( f > -0.00000000001f  && f < 0.00000000001f  ) return ( BYTE )( ret | ( 4 << 4 ) | i ) ; if( f > 0.0f && mf > f ){ mi = i ; mb = 4 ; mf = f ; } }
			for( i = 1 ; i < 15 ; i ++ ){ f = i / 100000.0f   - Unit ; if( f > -0.00000000001f  && f < 0.00000000001f  ) return ( BYTE )( ret | ( 5 << 4 ) | i ) ; if( f > 0.0f && mf > f ){ mi = i ; mb = 5 ; mf = f ; } }
			for( i = 1 ; i < 15 ; i ++ ){ f = i / 1000000.0f  - Unit ; if( f > -0.000000000001f && f < 0.000000000001f ) return ( BYTE )( ret | ( 6 << 4 ) | i ) ; if( f > 0.0f && mf > f ){ mi = i ; mb = 6 ; mf = f ; } }
			for( i = 1 ; i < 15 ; i ++ ){ f = i / 10000000.0f - Unit ; if( f > -0.000000000001f && f < 0.000000000001f ) return ( BYTE )( ret | ( 7 << 4 ) | i ) ; if( f > 0.0f && mf > f ){ mi = i ; mb = 7 ; mf = f ; } }
			return ( BYTE )( ret | ( mb << 4 ) | mi ) ;
		}
		else
		{
			ret = 0 ;

			// �P�D�O�ȏ�
			for( i = 1 ; i < 15 ; i ++ ){ f = i * 1.0f        - Unit ; if( f > -0.000000001f && f < 0.000000001f ) return ( BYTE )( ret | ( 0 << 4 ) | i ) ; if( f > 0.0f && mf > f ){ mi = i ; mb = 0 ; mf = f ; } }
			for( i = 1 ; i < 15 ; i ++ ){ f = i * 10.0f       - Unit ; if( f > -0.000000001f && f < 0.000000001f ) return ( BYTE )( ret | ( 1 << 4 ) | i ) ; if( f > 0.0f && mf > f ){ mi = i ; mb = 1 ; mf = f ; } }
			for( i = 1 ; i < 15 ; i ++ ){ f = i * 100.0f      - Unit ; if( f > -0.000000001f && f < 0.000000001f ) return ( BYTE )( ret | ( 2 << 4 ) | i ) ; if( f > 0.0f && mf > f ){ mi = i ; mb = 2 ; mf = f ; } }
			for( i = 1 ; i < 15 ; i ++ ){ f = i * 1000.0f     - Unit ; if( f > -0.000000001f && f < 0.000000001f ) return ( BYTE )( ret | ( 3 << 4 ) | i ) ; if( f > 0.0f && mf > f ){ mi = i ; mb = 3 ; mf = f ; } }
			for( i = 1 ; i < 15 ; i ++ ){ f = i * 10000.0f    - Unit ; if( f > -0.000000001f && f < 0.000000001f ) return ( BYTE )( ret | ( 4 << 4 ) | i ) ; if( f > 0.0f && mf > f ){ mi = i ; mb = 4 ; mf = f ; } }
			for( i = 1 ; i < 15 ; i ++ ){ f = i * 100000.0f   - Unit ; if( f > -0.000000001f && f < 0.000000001f ) return ( BYTE )( ret | ( 5 << 4 ) | i ) ; if( f > 0.0f && mf > f ){ mi = i ; mb = 5 ; mf = f ; } }
			for( i = 1 ; i < 15 ; i ++ ){ f = i * 1000000.0f  - Unit ; if( f > -0.000000001f && f < 0.000000001f ) return ( BYTE )( ret | ( 6 << 4 ) | i ) ; if( f > 0.0f && mf > f ){ mi = i ; mb = 6 ; mf = f ; } }
			for( i = 1 ; i < 15 ; i ++ ){ f = i * 10000000.0f - Unit ; if( f > -0.000000001f && f < 0.000000001f ) return ( BYTE )( ret | ( 7 << 4 ) | i ) ; if( f > 0.0f && mf > f ){ mi = i ; mb = 7 ; mf = f ; } }
			return ( BYTE )( ret | ( mb << 4 ) | mi ) ;
		}
	}
}

// MV1_ANIM_KEY_16BIT�\���̂� Min �ϐ��̒l���� float�^�̒l���쐬����
extern float MV1AnimKey16BitMinBtoF( BYTE Min )
{
	static float Table[ 2 ][ 16 ] =
	{
		1.0f,
		10.0f,
		100.0f,
		1000.0f,
		10000.0f,
		100000.0f,
		1000000.0f,
		10000000.0f,
		100000000.0f,
		1000000000.0f,
		10000000000.0f,
		100000000000.0f,
		1000000000000.0f,
		10000000000000.0f,
		100000000000000.0f,
		1000000000000000.0f,

		1.0f,
		0.1f,
		0.01f,
		0.001f,
		0.0001f,
		0.00001f,
		0.000001f,
		0.0000001f,
		0.00000001f,
		0.000000001f,
		0.0000000001f,
		0.00000000001f,
		0.000000000001f,
		0.0000000000001f,
		0.00000000000001f,
		0.000000000000001f,
	} ;

	if( Min & 0x80 )
	{
		return 0 ;
	}
	else
	{
		return Table[ ( Min >> 5 ) & 1 ][ Min & 0x1f ] * ( ( Min & 0x40 ) ? -1.0f : 1.0f ) ;
	}
}

// MV1_ANIM_KEY_16BIT�\���̂� Unit �ϐ��̒l���� float�^�̒l���쐬����
extern float MV1AnimKey16BitUnitBtoF( BYTE Unit )
{
	static float Table[ 2 ][ 8 ] =
	{
		1.0f,
		10.0f,
		100.0f,
		1000.0f,
		10000.0f,
		100000.0f,
		1000000.0f,
		10000000.0f,

		1.0f,
		0.1f,
		0.01f,
		0.001f,
		0.0001f,
		0.00001f,
		0.000001f,
		0.0000001f,
	} ;
	return Table[ Unit >> 7 ][ ( Unit >> 4 ) & 0x7 ] * ( Unit & 0x0f ) ;
}













// MATRIX �\���̂� MATRIX_4X4CT_F �\���̂ɕϊ�����
extern void ConvertMatrixFToMatrix4x4cF( MATRIX_4X4CT_F *Out, const MATRIX *In )
{
	Out->m[ 0 ][ 0 ] = In->m[ 0 ][ 0 ] ;
	Out->m[ 1 ][ 0 ] = In->m[ 0 ][ 1 ] ;
	Out->m[ 2 ][ 0 ] = In->m[ 0 ][ 2 ] ;

	Out->m[ 0 ][ 1 ] = In->m[ 1 ][ 0 ] ;
	Out->m[ 1 ][ 1 ] = In->m[ 1 ][ 1 ] ;
	Out->m[ 2 ][ 1 ] = In->m[ 1 ][ 2 ] ;

	Out->m[ 0 ][ 2 ] = In->m[ 2 ][ 0 ] ;
	Out->m[ 1 ][ 2 ] = In->m[ 2 ][ 1 ] ;
	Out->m[ 2 ][ 2 ] = In->m[ 2 ][ 2 ] ;

	Out->m[ 0 ][ 3 ] = In->m[ 3 ][ 0 ] ;
	Out->m[ 1 ][ 3 ] = In->m[ 3 ][ 1 ] ;
	Out->m[ 2 ][ 3 ] = In->m[ 3 ][ 2 ] ;
}

// MATRIX_D �\���̂� MATRIX_4X4CT_D �\���̂ɕϊ�����
extern void ConvertMatrixDToMatrix4x4cD( MATRIX_4X4CT_D *Out, const MATRIX_D *In )
{
	Out->m[ 0 ][ 0 ] = In->m[ 0 ][ 0 ] ;
	Out->m[ 1 ][ 0 ] = In->m[ 0 ][ 1 ] ;
	Out->m[ 2 ][ 0 ] = In->m[ 0 ][ 2 ] ;

	Out->m[ 0 ][ 1 ] = In->m[ 1 ][ 0 ] ;
	Out->m[ 1 ][ 1 ] = In->m[ 1 ][ 1 ] ;
	Out->m[ 2 ][ 1 ] = In->m[ 1 ][ 2 ] ;

	Out->m[ 0 ][ 2 ] = In->m[ 2 ][ 0 ] ;
	Out->m[ 1 ][ 2 ] = In->m[ 2 ][ 1 ] ;
	Out->m[ 2 ][ 2 ] = In->m[ 2 ][ 2 ] ;

	Out->m[ 0 ][ 3 ] = In->m[ 3 ][ 0 ] ;
	Out->m[ 1 ][ 3 ] = In->m[ 3 ][ 1 ] ;
	Out->m[ 2 ][ 3 ] = In->m[ 3 ][ 2 ] ;
}

// MATRIX_D �\���̂� MATRIX_4X4CT_F �\���̂ɕϊ�����
extern void ConvertMatrixDToMatrix4x4cF( MATRIX_4X4CT_F *Out, const MATRIX_D *In )
{
	Out->m[ 0 ][ 0 ] = ( float )In->m[ 0 ][ 0 ] ;
	Out->m[ 1 ][ 0 ] = ( float )In->m[ 0 ][ 1 ] ;
	Out->m[ 2 ][ 0 ] = ( float )In->m[ 0 ][ 2 ] ;

	Out->m[ 0 ][ 1 ] = ( float )In->m[ 1 ][ 0 ] ;
	Out->m[ 1 ][ 1 ] = ( float )In->m[ 1 ][ 1 ] ;
	Out->m[ 2 ][ 1 ] = ( float )In->m[ 1 ][ 2 ] ;

	Out->m[ 0 ][ 2 ] = ( float )In->m[ 2 ][ 0 ] ;
	Out->m[ 1 ][ 2 ] = ( float )In->m[ 2 ][ 1 ] ;
	Out->m[ 2 ][ 2 ] = ( float )In->m[ 2 ][ 2 ] ;

	Out->m[ 0 ][ 3 ] = ( float )In->m[ 3 ][ 0 ] ;
	Out->m[ 1 ][ 3 ] = ( float )In->m[ 3 ][ 1 ] ;
	Out->m[ 2 ][ 3 ] = ( float )In->m[ 3 ][ 2 ] ;
}

// MATRIX �\���̂� MATRIX_4X4CT_D �\���̂ɕϊ�����
extern void ConvertMatrixFToMatrix4x4cD( MATRIX_4X4CT_D *Out, const MATRIX *In )
{
	Out->m[ 0 ][ 0 ] = In->m[ 0 ][ 0 ] ;
	Out->m[ 1 ][ 0 ] = In->m[ 0 ][ 1 ] ;
	Out->m[ 2 ][ 0 ] = In->m[ 0 ][ 2 ] ;

	Out->m[ 0 ][ 1 ] = In->m[ 1 ][ 0 ] ;
	Out->m[ 1 ][ 1 ] = In->m[ 1 ][ 1 ] ;
	Out->m[ 2 ][ 1 ] = In->m[ 1 ][ 2 ] ;

	Out->m[ 0 ][ 2 ] = In->m[ 2 ][ 0 ] ;
	Out->m[ 1 ][ 2 ] = In->m[ 2 ][ 1 ] ;
	Out->m[ 2 ][ 2 ] = In->m[ 2 ][ 2 ] ;

	Out->m[ 0 ][ 3 ] = In->m[ 3 ][ 0 ] ;
	Out->m[ 1 ][ 3 ] = In->m[ 3 ][ 1 ] ;
	Out->m[ 2 ][ 3 ] = In->m[ 3 ][ 2 ] ;
}

// MATRIX_4X4CT_F �\���̂� MATRIX �\���̂ɕϊ�����
extern void ConvertMatrix4x4cFToMatrixF( MATRIX *Out, const MATRIX_4X4CT_F *In )
{
	Out->m[ 0 ][ 0 ] = In->m[ 0 ][ 0 ] ;
	Out->m[ 0 ][ 1 ] = In->m[ 1 ][ 0 ] ;
	Out->m[ 0 ][ 2 ] = In->m[ 2 ][ 0 ] ;
	Out->m[ 0 ][ 3 ] = 0.0f ;
	Out->m[ 1 ][ 0 ] = In->m[ 0 ][ 1 ] ;
	Out->m[ 1 ][ 1 ] = In->m[ 1 ][ 1 ] ;
	Out->m[ 1 ][ 2 ] = In->m[ 2 ][ 1 ] ;
	Out->m[ 1 ][ 3 ] = 0.0f ;
	Out->m[ 2 ][ 0 ] = In->m[ 0 ][ 2 ] ;
	Out->m[ 2 ][ 1 ] = In->m[ 1 ][ 2 ] ;
	Out->m[ 2 ][ 2 ] = In->m[ 2 ][ 2 ] ;
	Out->m[ 2 ][ 3 ] = 0.0f ;
	Out->m[ 3 ][ 0 ] = In->m[ 0 ][ 3 ] ;
	Out->m[ 3 ][ 1 ] = In->m[ 1 ][ 3 ] ;
	Out->m[ 3 ][ 2 ] = In->m[ 2 ][ 3 ] ;
	Out->m[ 3 ][ 3 ] = 1.0f ;
}

// MATRIX_4X4CT_D �\���̂� MATRIX_D �\���̂ɕϊ�����
extern void ConvertMatrix4x4cDToMatrixD( MATRIX_D *Out, const MATRIX_4X4CT_D *In )
{
	Out->m[ 0 ][ 0 ] = In->m[ 0 ][ 0 ] ;
	Out->m[ 0 ][ 1 ] = In->m[ 1 ][ 0 ] ;
	Out->m[ 0 ][ 2 ] = In->m[ 2 ][ 0 ] ;
	Out->m[ 0 ][ 3 ] = 0.0 ;
	Out->m[ 1 ][ 0 ] = In->m[ 0 ][ 1 ] ;
	Out->m[ 1 ][ 1 ] = In->m[ 1 ][ 1 ] ;
	Out->m[ 1 ][ 2 ] = In->m[ 2 ][ 1 ] ;
	Out->m[ 1 ][ 3 ] = 0.0 ;
	Out->m[ 2 ][ 0 ] = In->m[ 0 ][ 2 ] ;
	Out->m[ 2 ][ 1 ] = In->m[ 1 ][ 2 ] ;
	Out->m[ 2 ][ 2 ] = In->m[ 2 ][ 2 ] ;
	Out->m[ 2 ][ 3 ] = 0.0 ;
	Out->m[ 3 ][ 0 ] = In->m[ 0 ][ 3 ] ;
	Out->m[ 3 ][ 1 ] = In->m[ 1 ][ 3 ] ;
	Out->m[ 3 ][ 2 ] = In->m[ 2 ][ 3 ] ;
	Out->m[ 3 ][ 3 ] = 1.0 ;
}

// MATRIX_4X4CT_D �\���̂� MATRIX   �\���̂ɕϊ�����
extern void ConvertMatrix4x4cDToMatrixF( MATRIX   *Out, const MATRIX_4X4CT_D *In )
{
	Out->m[ 0 ][ 0 ] = ( float )In->m[ 0 ][ 0 ] ;
	Out->m[ 0 ][ 1 ] = ( float )In->m[ 1 ][ 0 ] ;
	Out->m[ 0 ][ 2 ] = ( float )In->m[ 2 ][ 0 ] ;
	Out->m[ 0 ][ 3 ] = 0.0f ;
	Out->m[ 1 ][ 0 ] = ( float )In->m[ 0 ][ 1 ] ;
	Out->m[ 1 ][ 1 ] = ( float )In->m[ 1 ][ 1 ] ;
	Out->m[ 1 ][ 2 ] = ( float )In->m[ 2 ][ 1 ] ;
	Out->m[ 1 ][ 3 ] = 0.0f ;
	Out->m[ 2 ][ 0 ] = ( float )In->m[ 0 ][ 2 ] ;
	Out->m[ 2 ][ 1 ] = ( float )In->m[ 1 ][ 2 ] ;
	Out->m[ 2 ][ 2 ] = ( float )In->m[ 2 ][ 2 ] ;
	Out->m[ 2 ][ 3 ] = 0.0f ;
	Out->m[ 3 ][ 0 ] = ( float )In->m[ 0 ][ 3 ] ;
	Out->m[ 3 ][ 1 ] = ( float )In->m[ 1 ][ 3 ] ;
	Out->m[ 3 ][ 2 ] = ( float )In->m[ 2 ][ 3 ] ;
	Out->m[ 3 ][ 3 ] = 1.0f ;
}

// MATRIX_4X4CT_F �\���̂� MATRIX_D �\���̂ɕϊ�����
extern void ConvertMatrix4x4cFToMatrixD( MATRIX_D *Out, const MATRIX_4X4CT_F *In )
{
	Out->m[ 0 ][ 0 ] = In->m[ 0 ][ 0 ] ;
	Out->m[ 0 ][ 1 ] = In->m[ 1 ][ 0 ] ;
	Out->m[ 0 ][ 2 ] = In->m[ 2 ][ 0 ] ;
	Out->m[ 0 ][ 3 ] = 0.0 ;
	Out->m[ 1 ][ 0 ] = In->m[ 0 ][ 1 ] ;
	Out->m[ 1 ][ 1 ] = In->m[ 1 ][ 1 ] ;
	Out->m[ 1 ][ 2 ] = In->m[ 2 ][ 1 ] ;
	Out->m[ 1 ][ 3 ] = 0.0 ;
	Out->m[ 2 ][ 0 ] = In->m[ 0 ][ 2 ] ;
	Out->m[ 2 ][ 1 ] = In->m[ 1 ][ 2 ] ;
	Out->m[ 2 ][ 2 ] = In->m[ 2 ][ 2 ] ;
	Out->m[ 2 ][ 3 ] = 0.0 ;
	Out->m[ 3 ][ 0 ] = In->m[ 0 ][ 3 ] ;
	Out->m[ 3 ][ 1 ] = In->m[ 1 ][ 3 ] ;
	Out->m[ 3 ][ 2 ] = In->m[ 2 ][ 3 ] ;
	Out->m[ 3 ][ 3 ] = 1.0 ;
}

// MATRIX_4X4CT_F   �\���̂� MATRIX_4X4CT_D �\���̂ɕϊ�����
extern void ConvertMatrix4x4cFToMatrix4x4cD(  MATRIX_4X4CT_D *Out, const MATRIX_4X4CT_F *In )
{
	Out->m[ 0 ][ 0 ] = In->m[ 0 ][ 0 ] ;
	Out->m[ 0 ][ 1 ] = In->m[ 0 ][ 1 ] ;
	Out->m[ 0 ][ 2 ] = In->m[ 0 ][ 2 ] ;
	Out->m[ 0 ][ 3 ] = In->m[ 0 ][ 3 ] ;

	Out->m[ 1 ][ 0 ] = In->m[ 1 ][ 0 ] ;
	Out->m[ 1 ][ 1 ] = In->m[ 1 ][ 1 ] ;
	Out->m[ 1 ][ 2 ] = In->m[ 1 ][ 2 ] ;
	Out->m[ 1 ][ 3 ] = In->m[ 1 ][ 3 ] ;

	Out->m[ 2 ][ 0 ] = In->m[ 2 ][ 0 ] ;
	Out->m[ 2 ][ 1 ] = In->m[ 2 ][ 1 ] ;
	Out->m[ 2 ][ 2 ] = In->m[ 2 ][ 2 ] ;
	Out->m[ 2 ][ 3 ] = In->m[ 2 ][ 3 ] ;
}

// MATRIX_4X4CT_F   �\���̂� MATRIX_4X4CT_D �\���̂ɕϊ�����
extern void ConvertMatrix4x4cDToMatrix4x4cF(  MATRIX_4X4CT_F   *Out, const MATRIX_4X4CT_D *In )
{
	Out->m[ 0 ][ 0 ] = ( float )In->m[ 0 ][ 0 ] ;
	Out->m[ 0 ][ 1 ] = ( float )In->m[ 0 ][ 1 ] ;
	Out->m[ 0 ][ 2 ] = ( float )In->m[ 0 ][ 2 ] ;
	Out->m[ 0 ][ 3 ] = ( float )In->m[ 0 ][ 3 ] ;

	Out->m[ 1 ][ 0 ] = ( float )In->m[ 1 ][ 0 ] ;
	Out->m[ 1 ][ 1 ] = ( float )In->m[ 1 ][ 1 ] ;
	Out->m[ 1 ][ 2 ] = ( float )In->m[ 1 ][ 2 ] ;
	Out->m[ 1 ][ 3 ] = ( float )In->m[ 1 ][ 3 ] ;

	Out->m[ 2 ][ 0 ] = ( float )In->m[ 2 ][ 0 ] ;
	Out->m[ 2 ][ 1 ] = ( float )In->m[ 2 ][ 1 ] ;
	Out->m[ 2 ][ 2 ] = ( float )In->m[ 2 ][ 2 ] ;
	Out->m[ 2 ][ 3 ] = ( float )In->m[ 2 ][ 3 ] ;
}

// MATRIX �\���̂� MATRIX_4X4CT �\���̂ɕϊ�����
extern void ConvertMatrixFToMatrix4x4c( MATRIX_4X4CT *Out, const MATRIX   *In )
{
	if( GSYS.DrawSetting.Large3DPositionSupport )
	{
		ConvertMatrixFToMatrix4x4cD( &Out->md, In ) ;
	}
	else
	{
		ConvertMatrixFToMatrix4x4cF( &Out->mf, In ) ;
	}
}

// MATRIX_D �\���̂� MATRIX_4X4CT �\���̂ɕϊ�����
extern void ConvertMatrixDToMatrix4x4c( MATRIX_4X4CT *Out, const MATRIX_D *In )
{
	if( GSYS.DrawSetting.Large3DPositionSupport )
	{
		ConvertMatrixDToMatrix4x4cD( &Out->md, In ) ;
	}
	else
	{
		ConvertMatrixDToMatrix4x4cF( &Out->mf, In ) ;
	}
}

// MATRIX_4X4CT �\���̂� MATRIX �\���̂ɕϊ�����
extern void ConvertMatrix4x4cToMatrixF( MATRIX   *Out, const MATRIX_4X4CT *In )
{
	if( GSYS.DrawSetting.Large3DPositionSupport )
	{
		ConvertMatrix4x4cDToMatrixF( Out, &In->md ) ;
	}
	else
	{
		ConvertMatrix4x4cFToMatrixF( Out, &In->mf ) ;
	}
}

// MATRIX_4X4CT   �\���̂� MATRIX_D �\���̂ɕϊ�����
extern void ConvertMatrix4x4cToMatrixD( MATRIX_D *Out, const MATRIX_4X4CT *In )
{
	if( GSYS.DrawSetting.Large3DPositionSupport )
	{
		ConvertMatrix4x4cDToMatrixD( Out, &In->md ) ;
	}
	else
	{
		ConvertMatrix4x4cFToMatrixD( Out, &In->mf ) ;
	}
}

// MATRIX_4X4CT   �\���̂� MATRIX_4X4CT_F �\���̂ɕϊ�����
extern void ConvertMatrix4x4cToMatrix4x4cF(  MATRIX_4X4CT_F *Out, const MATRIX_4X4CT *In )
{
	if( GSYS.DrawSetting.Large3DPositionSupport )
	{
		ConvertMatrix4x4cDToMatrix4x4cF( Out, &In->md ) ;
	}
	else
	{
		*Out = In->mf ;
	}
}

// MATRIX_4X4CT   �\���̂� MATRIX_4X4CT_D �\���̂ɕϊ�����
extern void ConvertMatrix4x4cToMatrix4x4cD(  MATRIX_4X4CT_D *Out, const MATRIX_4X4CT *In )
{
	if( GSYS.DrawSetting.Large3DPositionSupport )
	{
		*Out = In->md ;
	}
	else
	{
		ConvertMatrix4x4cFToMatrix4x4cD( Out, &In->mf ) ;
	}
}

// MATRIX_4X4CT_F �\���̂� MATRIX_4X4CT �\���̂ɕϊ�����
extern void ConvertMatrix4x4cFToMatrix4x4c( MATRIX_4X4CT *Out, const MATRIX_4X4CT_F *In )
{
	if( GSYS.DrawSetting.Large3DPositionSupport )
	{
		ConvertMatrix4x4cFToMatrix4x4cD( &Out->md, In ) ;
	}
	else
	{
		Out->mf = *In ;
	}
}

// MATRIX_4X4CT_D �\���̂� MATRIX_4X4CT �\���̂ɕϊ�����
extern void ConvertMatrix4x4cDToMatrix4x4c( MATRIX_4X4CT *Out, const MATRIX_4X4CT_D *In )
{
	if( GSYS.DrawSetting.Large3DPositionSupport )
	{
		Out->md = *In ;
	}
	else
	{
		ConvertMatrix4x4cDToMatrix4x4cF( &Out->mf, In ) ;
	}
}






// �w��̎��Ԃ𒴂����ԏ������ԍ��̃L�[�̃C���f�b�N�X���擾����
__inline static int _MV1GetAnimKeyDataIndexFromTime_inline( MV1_ANIM_KEYSET_BASE *AnimKeySetBase, float Time, float TotalRate, int NowKey, bool AboutSetting )
{
	int KeyNum ;
	float *TimeList ;

	KeyNum = AnimKeySetBase->Num ;

	// �L�[���������Ă��邩�ǂ����ŏ����𕪊�
	if( AnimKeySetBase->TimeType == MV1_ANIMKEY_TIME_TYPE_KEY )
	{
		// �A�o�E�g�Z�b�e�B���O�w��̏ꍇ�͍ŏ��ɑ�̂̃A�N�e�B�u�L�[���Z�b�g����
		if( AboutSetting )
		{
			// ��̂̃L�[�ԍ��𒲂ׂ�
			NowKey = _FTOL( AnimKeySetBase->Num * TotalRate ) ;

			// �O�a����
				 if( NowKey <                    0 ) NowKey = 0 ;
			else if( NowKey >= AnimKeySetBase->Num ) NowKey = AnimKeySetBase->Num - 1 ;
		}

		// �K�������L�[�Ɉړ�����
		TimeList = AnimKeySetBase->KeyTime ;
		if( TimeList[ NowKey ] > Time )
		{
			while( NowKey !=          0 && TimeList[ NowKey     ] >  Time )
				NowKey -- ;
		}
		else
		{
			while( NowKey <  KeyNum - 1 && TimeList[ NowKey + 1 ] <= Time )
				NowKey ++ ;
		}
	}
	else
	{
		// �e�L�[�̊Ԋu�����̏ꍇ�͒P���v�Z
		NowKey = _FTOL( ( Time - AnimKeySetBase->StartTime ) / AnimKeySetBase->UnitTime ) ;
		     if( NowKey <  0      ) NowKey = 0 ;
		else if( NowKey >= KeyNum ) NowKey = KeyNum - 1 ;
	}

	return NowKey ;
}

static int _MV1GetAnimKeyDataIndexFromTime( MV1_ANIM_KEYSET_BASE *AnimKeySetBase, float Time, float &Rate )
{
	int KeyIndex ;

	// �L�[���������Ă��邩�ǂ����ŏ����𕪊�
	if( AnimKeySetBase->TimeType == MV1_ANIMKEY_TIME_TYPE_KEY )
	{
		KeyIndex = _MV1GetAnimKeyDataIndexFromTime_inline( AnimKeySetBase, Time, ( Time - AnimKeySetBase->StartTime ) / ( AnimKeySetBase->KeyTime[ AnimKeySetBase->Num - 1 ] - AnimKeySetBase->StartTime ), 0, true ) ;
	}
	else
	{
		KeyIndex = _MV1GetAnimKeyDataIndexFromTime_inline( AnimKeySetBase, Time, 0.0f, 0, true ) ;
	}

	if( KeyIndex == AnimKeySetBase->Num - 1 || Time < AnimKeySetBase->StartTime )
	{
		Rate = 0.0f ;
		return KeyIndex ;
	}
	else
	{
		// �L�[���������Ă��邩�ǂ����ŏ����𕪊�
		if( AnimKeySetBase->TimeType == MV1_ANIMKEY_TIME_TYPE_KEY )
		{
			Rate = ( Time - AnimKeySetBase->KeyTime[ KeyIndex ] ) / (  AnimKeySetBase->KeyTime[ KeyIndex + 1 ] - AnimKeySetBase->KeyTime[ KeyIndex ] ) ;
		}
		else
		{
			Rate = ( Time - KeyIndex * AnimKeySetBase->UnitTime - AnimKeySetBase->StartTime ) / AnimKeySetBase->UnitTime ;
		}
	}

	return KeyIndex ;
}

// ���݂̃A�j���[�V�����Đ����ԂɊe�L�[�� NowKey �̒l�����킹��
static int _MV1AnimSetSyncNowKey( MV1_ANIMSET *AnimSet, bool AboutSetting )
{
	MV1_ANIM				*Anim ;
	MV1_ANIM_KEYSET			*KeySet ;
	int i, j ;
	float NowTime, NowRate ;

	// ���݂̍Đ����Ԃ��Z�b�g
	NowTime = AnimSet->NowTime ;

	// �S�̂̎��Ԃɑ΂���w�莞�Ԃ̔䗦�����߂�
	NowRate = NowTime / AnimSet->BaseData->MaxTime ;

	// �ݒ肳��Ă���A�N�e�B�u�L�[���猻�݂̓K�؂ȃA�N�e�B�u�L�[����������
	Anim = AnimSet->Anim ;
	for( i = 0 ; i < AnimSet->BaseData->AnimNum ; i ++, Anim ++ )
	{
		KeySet = Anim->KeySet ;
		for( j = 0 ; j < Anim->BaseData->KeySetNum ; j ++, KeySet ++ )
		{
			KeySet->NowKey = _MV1GetAnimKeyDataIndexFromTime_inline( KeySet->BaseData, NowTime, NowRate, KeySet->NowKey, AboutSetting ) ;
		}
	}

	// �I��
	return 0 ;
}

// ���O��C���f�b�N�X���烂�f����{�f�[�^���̃A�j���[�V�������擾����
static MV1_ANIMSET_BASE *MV1GetAnimSetBase( int MV1ModelHandle, const wchar_t *Name, int Index )
{
	MV1_MODEL_BASE *ModelBase ;
	MV1_ANIMSET_BASE *AnimSetBase ;
	int i ;

	// �A�h���X�擾
	if( MV1BMDLCHK( MV1ModelHandle, ModelBase ) )
		return NULL ;

	// �w��̃A�j���[�V�����𒲂ׂ�
	if( Index >= 0 )
	{
		// �w��̃C���f�b�N�X���A�j���[�V�����Z�b�g�̐�����O��Ă�����G���[
		if( Index >= ModelBase->AnimSetNum ) return NULL ;
		return &ModelBase->AnimSet[ Index ] ;
	}
	else
	{
		// ���O�������ȃA�j���[�V�����Z�b�g��T��
		AnimSetBase = ModelBase->AnimSet ;
		for( i = 0 ; i < ModelBase->AnimSetNum && _WCSCMP( AnimSetBase->NameW, Name ) != 0 ; i ++, AnimSetBase ++ ){}
		return i == ModelBase->AnimSetNum ? NULL : &ModelBase->AnimSet[ i ] ;
	}
}

// �A�j���[�V�����Z�b�g��{�f�[�^������s�p�A�j���[�V�����Z�b�g���쐬����
static MV1_ANIMSET *MV1CreateAnimSet( MV1_ANIMSET_BASE *MV1AnimSetBase )
{
	MV1_ANIM_BASE *AnimBase ;
	MV1_ANIMSET *AnimSet ;
	MV1_ANIM *Anim ;
	int AllocSize ;
	int i, j ;

	// �m�ۂ��ׂ��������̃T�C�Y���Z�o����
	AllocSize = sizeof( MV1_ANIMSET ) ;
	AllocSize += MV1AnimSetBase->AnimNum * sizeof( MV1_ANIM ) ;
	AnimBase = MV1AnimSetBase->Anim ;
	for( i = 0 ; i < MV1AnimSetBase->AnimNum ; i ++, AnimBase ++ )
	{
		AllocSize += sizeof( MV1_ANIM_KEYSET ) * AnimBase->KeySetNum ;
		AllocSize += sizeof( float ) * AnimBase->TargetFrame->ShapeNum ;
	}

	// �������̊m��
	AnimSet = ( MV1_ANIMSET * )MDALLOCMEM( ( size_t )AllocSize ) ;
	if( AnimSet == NULL ) return NULL ;
	_MEMSET( AnimSet, 0, ( size_t )AllocSize ) ;

	// ���̃Z�b�g
	AllocSize = 0 ;
	AnimSet->BaseData = MV1AnimSetBase ;
	AllocSize += sizeof( MV1_ANIMSET ) ;

	AnimSet->Anim = ( MV1_ANIM * )( ( BYTE * )AnimSet + AllocSize ) ;
	AllocSize += sizeof( MV1_ANIM ) * MV1AnimSetBase->AnimNum ;

	AnimBase = MV1AnimSetBase->Anim ;
	Anim = AnimSet->Anim ;
	for( i = 0 ; i < MV1AnimSetBase->AnimNum ; i ++, AnimBase ++, Anim ++ )
	{
		Anim->BaseData = AnimBase ;
		Anim->KeySet = ( MV1_ANIM_KEYSET * )( ( BYTE * )AnimSet + AllocSize ) ;
		AllocSize += sizeof( MV1_ANIM_KEYSET ) * AnimBase->KeySetNum ;
		for( j = 0 ; j < AnimBase->KeySetNum ; j ++ )
		{
			Anim->KeySet[ j ].BaseData = &AnimBase->KeySet[ j ] ;
			Anim->KeySet[ j ].ShapeTargetIndex = -1 ;
		}
		Anim->ShapeRate = ( float * )( ( BYTE * )AnimSet + AllocSize ) ;
		AllocSize += sizeof( float ) * Anim->BaseData->TargetFrame->ShapeNum ;
		for( j = 0 ; j < Anim->BaseData->TargetFrame->ShapeNum ; j ++ )
		{
			Anim->ShapeRate[ j ] = 0.0f ;
		}
	}

	// �I��
	return AnimSet ;
}

// �A�j���[�V�����Z�b�g�������̖��O�A�Ⴕ���̓C���f�b�N�X�̃A�j���[�V�����𓾂�
static MV1_ANIM *MV1GetAnimSetAnim( MV1_ANIMSET *AnimSet, const wchar_t *Name, int Index )
{
	MV1_ANIM_BASE *AnimBase ;
	MV1_ANIMSET_BASE *AnimSetBase ;
	int i ;

	// ���O�������ȏꍇ�̓C���f�b�N�X����A�j���[�V������Ԃ�
	if( Name == NULL ) return AnimSet->Anim + Index ;

	// ���O���L���ȏꍇ�͖��O���猟��
	AnimSetBase = AnimSet->BaseData ;
	AnimBase = AnimSetBase->Anim ;
	for( i = 0 ; i < AnimSetBase->AnimNum ; i ++, AnimBase ++ )
	{
		if( _WCSCMP( AnimBase->TargetFrame->NameW, Name ) == 0 ) break ;
	}
	return i == AnimSetBase->AnimNum ? NULL : AnimSet->Anim + i ;
}

// �A�j���[�V�������w��̎��Ԃ܂Ői�߂�
static int MV1SetAnimSetTime( MV1_ANIMSET *AnimSet, float Time )
{
	// �w��̎��Ԃ��͈͂𒴂��Ă�����N�����v����
	     if( AnimSet->BaseData->MaxTime < Time ) Time = AnimSet->BaseData->MaxTime ;
	else if(                       0.0f > Time ) Time = 0.0f ;

	// �^�C�����Z�b�g����
	AnimSet->NowTime = Time ;

	// ��̂̈ʒu���Z�b�g����
	_MV1AnimSetSyncNowKey( AnimSet, true ) ;

	// ���[�v�t���O��|��
//	AnimSet->LoopCompFlag = false ;

	// �I��
	return 0 ;
}

// �A�j���[�V�����̌��݂̍Đ��o�ߎ��Ԃɍ��킹���p�����[�^���v�Z����
static int MV1AnimSetSetupParam( MV1_ANIMSET *AnimSet )
{
	MV1_ANIM                     *Anim ;
	MV1_ANIMSET_BASE             *AnimSetBase ;
	MV1_ANIM_KEYSET_BASE         *KeySetBase ;
	VECTOR                  * RST KeyVector1 ;
	VECTOR                  * RST KeyVector2 ;
	float                   * RST KeyLinear1 ;
	float                   * RST KeyLinear2 ;
	MV1_ANIM_KEY_MATRIX4X4C * RST KeyMatrix4x4C1 ;
	MV1_ANIM_KEY_MATRIX4X4C * RST KeyMatrix4x4C2 ;
	VECTOR                        Translate ;
	VECTOR                        Scale ;
	VECTOR                        Rotate ;
	VECTOR                        vdata ;
	int                           RotateOrder = 0 ;
	int                           i ;
	int                           j ;
	int                           NowKey ;
	int                           NextKey ;
	int                           QtType = 0 ;
	FLOAT4                        Quaternion ;
	FLOAT4                  * RST q1 ;
	FLOAT4                  * RST q2 ;
	MATRIX_4X4CT_F                Matrix ;
	int                           ValidMatrix ;
	float                         t ;
	float                         fdata ;
	bool                          AddParam ;
	float                         Sub ;
	VECTOR                        SubVec ;

	// �p�����[�^�̃Z�b�g�A�b�v���ς�ł���ꍇ�͉��������ɏI��
	if( AnimSet->ParamSetup ) return 0 ;

	// �A�j���[�V�����̐������J��Ԃ�
	AnimSetBase = AnimSet->BaseData ;
	AddParam = AnimSetBase->IsAddAnim == 1 ;

	// �s�񃌃x���̃u�����h�������s�����ǂ����ŏ����𕪊�
	if( AnimSetBase->IsMatrixLinearBlend )
//	if( 1 )
	{
		VECTOR Pos, NowRot, NextRot, Scale ;
		FLOAT4 NowQt, NextQt ;
		float NowRotX, NowRotY, NowRotZ ;
		float NextRotX, NextRotY, NextRotZ ;
		float RotRateX, RotRateY, RotRateZ, RotRate ;
		float QtRate ;
		VECTOR *PreRotate, *PostRotate ;

MATRIXLINEARBLEND :

		Anim = AnimSet->Anim ;
		for( i = 0 ; i < AnimSetBase->AnimNum ; i ++, Anim ++ )
		{
			MATRIX_4X4CT_F RotMat1, RotMat2 ;
			MATRIX_4X4CT_F PreRotMat, PostRotMat ;

			// �Z�b�g�A�b�v�������Z�b�g
			Anim->ValidFlag = 0 ;

			// ������Ԃł͖ڕW�m�[�h�̃f�t�H���g�l�����Ă���
			RotRate = -1.0f ;
			RotRateX = -1.0f ;
			RotRateY = -1.0f ;
			RotRateZ = -1.0f ;
			QtRate = -1.0f ;
			if( Anim->Frame )
			{
				NextRot    = NowRot    = Anim->Frame->BaseData->Rotate ;
				NextRotX   = NowRotX   = NowRot.x ;
				NextRotY   = NowRotY   = NowRot.y ;
				NextRotZ   = NowRotZ   = NowRot.z ;
				NextQt     = NowQt     = Anim->Frame->BaseData->Quaternion ;
				Pos    = Anim->Frame->BaseData->Translate ;
				Scale  = Anim->Frame->BaseData->Scale ;

				if( ( Anim->Frame->BaseData->Flag & MV1_FRAMEFLAG_PREROTATE  ) != 0 )
				{
					PreRotate = &Anim->Frame->BaseData->PreRotate ;
					switch( RotateOrder )
					{
					case MV1_ROTATE_ORDER_XYZ : CreateRotationXYZMatrix4X4CTF( &PreRotMat, PreRotate->x, PreRotate->y, PreRotate->z ) ; break ;
					case MV1_ROTATE_ORDER_XZY : CreateRotationXZYMatrix4X4CTF( &PreRotMat, PreRotate->x, PreRotate->y, PreRotate->z ) ; break ;
					case MV1_ROTATE_ORDER_YXZ : CreateRotationYXZMatrix4X4CTF( &PreRotMat, PreRotate->x, PreRotate->y, PreRotate->z ) ; break ;
					case MV1_ROTATE_ORDER_YZX : CreateRotationYZXMatrix4X4CTF( &PreRotMat, PreRotate->x, PreRotate->y, PreRotate->z ) ; break ;
					case MV1_ROTATE_ORDER_ZXY : CreateRotationZXYMatrix4X4CTF( &PreRotMat, PreRotate->x, PreRotate->y, PreRotate->z ) ; break ;
					case MV1_ROTATE_ORDER_ZYX : CreateRotationZYXMatrix4X4CTF( &PreRotMat, PreRotate->x, PreRotate->y, PreRotate->z ) ; break ;
					default: return -1;
					}
				}
				else
				{
					PreRotate = NULL ;
				}

				if( ( Anim->Frame->BaseData->Flag & MV1_FRAMEFLAG_POSTROTATE  ) != 0 )
				{
					PostRotate = &Anim->Frame->BaseData->PostRotate ;
					switch( RotateOrder )
					{
					case MV1_ROTATE_ORDER_XYZ : CreateRotationXYZMatrix4X4CTF( &PostRotMat, PostRotate->x, PostRotate->y, PostRotate->z ) ; break ;
					case MV1_ROTATE_ORDER_XZY : CreateRotationXZYMatrix4X4CTF( &PostRotMat, PostRotate->x, PostRotate->y, PostRotate->z ) ; break ;
					case MV1_ROTATE_ORDER_YXZ : CreateRotationYXZMatrix4X4CTF( &PostRotMat, PostRotate->x, PostRotate->y, PostRotate->z ) ; break ;
					case MV1_ROTATE_ORDER_YZX : CreateRotationYZXMatrix4X4CTF( &PostRotMat, PostRotate->x, PostRotate->y, PostRotate->z ) ; break ;
					case MV1_ROTATE_ORDER_ZXY : CreateRotationZXYMatrix4X4CTF( &PostRotMat, PostRotate->x, PostRotate->y, PostRotate->z ) ; break ;
					case MV1_ROTATE_ORDER_ZYX : CreateRotationZYXMatrix4X4CTF( &PostRotMat, PostRotate->x, PostRotate->y, PostRotate->z ) ; break ;
					default: return -1;
					}
				}
				else
				{
					PostRotate = NULL ;
				}
			}
			else
			{
				PreRotate = NULL ;
				PostRotate = NULL ;

				Pos.x = 0.0f ;
				Pos.y = 0.0f ;
				Pos.z = 0.0f ;

				NextRotX = NowRotX = NextRot.x = NowRot.x = 0.0f ;
				NextRotY = NowRotY = NextRot.y = NowRot.y = 0.0f ;
				NextRotZ = NowRotZ = NextRot.z = NowRot.z = 0.0f ;

				Scale.x = 1.0f ;
				Scale.y = 1.0f ;
				Scale.z = 1.0f ;

				NextQt.w = NowQt.w = 1.0f ;
				NextQt.x = NowQt.x = 0.0f ;
				NextQt.y = NowQt.y = 0.0f ;
				NextQt.z = NowQt.z = 0.0f ;
			}
			RotateOrder = Anim->RotateOrder ;

			// �e�v�f�̌��ݒl����o��
			for( j = 0 ; j < Anim->BaseData->KeySetNum ; j ++ )
			{
				NowKey     = Anim->KeySet[ j ].NowKey ;
				KeySetBase = Anim->KeySet[ j ].BaseData ;

				if( KeySetBase->Num == 0 )
				{
					continue ;
				}

				NextKey    = NowKey == KeySetBase->Num - 1 ? 0 : NowKey + 1 ;

				if( NowKey == KeySetBase->Num - 1 )
				{
					t = 0.0f ;
				}
				else
				{
					if( KeySetBase->TimeType == MV1_ANIMKEY_TIME_TYPE_ONE )
					{
						t = ( AnimSet->NowTime - ( KeySetBase->UnitTime * NowKey + KeySetBase->StartTime ) ) / KeySetBase->UnitTime ;
					}
					else
					{
						t = ( AnimSet->NowTime - KeySetBase->KeyTime[ NowKey ] ) / ( KeySetBase->KeyTime[ NowKey + 1 ] - KeySetBase->KeyTime[ NowKey ] ) ;
					}
					if( t < 0.0f ) t = 0.0f ;
				}

				switch( KeySetBase->Type )
				{
				case MV1_ANIMKEY_TYPE_LINEAR :
					KeyLinear1 = &KeySetBase->KeyLinear[ NowKey  ] ;
					KeyLinear2 = &KeySetBase->KeyLinear[ NextKey ] ;

					switch( KeySetBase->DataType )
					{
					case MV1_ANIMKEY_DATATYPE_TRANSLATE_X : Pos.x   = *KeyLinear1 + ( *KeyLinear2 - *KeyLinear1 ) * t ; break ;
					case MV1_ANIMKEY_DATATYPE_TRANSLATE_Y : Pos.y   = *KeyLinear1 + ( *KeyLinear2 - *KeyLinear1 ) * t ; break ;
					case MV1_ANIMKEY_DATATYPE_TRANSLATE_Z : Pos.z   = *KeyLinear1 + ( *KeyLinear2 - *KeyLinear1 ) * t ; break ;
					case MV1_ANIMKEY_DATATYPE_SCALE_X :     Scale.x = *KeyLinear1 + ( *KeyLinear2 - *KeyLinear1 ) * t ; break ;
					case MV1_ANIMKEY_DATATYPE_SCALE_Y :     Scale.y = *KeyLinear1 + ( *KeyLinear2 - *KeyLinear1 ) * t ; break ;
					case MV1_ANIMKEY_DATATYPE_SCALE_Z :     Scale.z = *KeyLinear1 + ( *KeyLinear2 - *KeyLinear1 ) * t ; break ;
					case MV1_ANIMKEY_DATATYPE_ROTATE_X :    NowRotX   = *KeyLinear1 ; NextRotX   = *KeyLinear2 ; RotRateX = t ; break ;
					case MV1_ANIMKEY_DATATYPE_ROTATE_Y :    NowRotY   = *KeyLinear1 ; NextRotY   = *KeyLinear2 ; RotRateY = t ; break ;
					case MV1_ANIMKEY_DATATYPE_ROTATE_Z :    NowRotZ   = *KeyLinear1 ; NextRotZ   = *KeyLinear2 ; RotRateZ = t ; break ;
					case MV1_ANIMKEY_DATATYPE_SHAPE :
						fdata = *KeyLinear1 + ( *KeyLinear2 - *KeyLinear1 ) * t ;
						Anim->KeySet[ j ].ShapeKeyFactor = fdata ;

						if( Anim->KeySet[ j ].ShapeTargetIndex != -1 )
						{
							float delta ;

							if( Anim->Frame )
							{
								delta = Anim->ShapeRate[ Anim->KeySet[ j ].ShapeTargetIndex ] - Anim->KeySet[ j ].ShapeKeyFactor ;
								if( delta < 0.0f ) delta = -delta ;

								if( delta >= 0.0000001f )
								{
									Anim->Frame->ShapeChangeFlag = true ;
									Anim->Frame->Container->ShapeChangeFlag = true ;
								}
							}

							Anim->ShapeRate[ Anim->KeySet[ j ].ShapeTargetIndex ] = Anim->KeySet[ j ].ShapeKeyFactor ;
						}

//						if( Anim->Frame )
//						{
//							if( Anim->KeySet[ j ].ShapeTargetIndex != -1 )
//							{
//								MV1_SHAPE *Shape ;
//								float delta ;
//
//								Shape = &Anim->Frame->Shape[ Anim->KeySet[ j ].ShapeTargetIndex ] ;
//								if( Anim->Frame->ShapeChangeFlag == false )
//								{
//									delta = Shape->Rate - Anim->KeySet[ j ].ShapeKeyFactor ;
//									if( delta < 0 ) delta = -delta ;
//									if( delta >= 0.0000001f )
//									{
//										Anim->Frame->ShapeChangeFlag = true ;
//										Anim->Frame->Container->ShapeChangeFlag = true ;
//									}
//								}
//								Shape->Rate = Anim->KeySet[ j ].ShapeKeyFactor ;
//							}
//						}
						break ;
					}
					break ;

				case MV1_ANIMKEY_TYPE_VECTOR :
					KeyVector1 = &KeySetBase->KeyVector[ NowKey ] ;
					KeyVector2 = &KeySetBase->KeyVector[ NextKey ] ;

					switch( KeySetBase->DataType )
					{
					case MV1_ANIMKEY_DATATYPE_ROTATE :
						NowRot  = *KeyVector1 ;
						NextRot = *KeyVector2 ;
						RotRate = t ;
						break ;

					case MV1_ANIMKEY_DATATYPE_SCALE :
						Scale.x = KeyVector1->x + ( KeyVector2->x - KeyVector1->x ) * t ;
						Scale.y = KeyVector1->y + ( KeyVector2->y - KeyVector1->y ) * t ;
						Scale.z = KeyVector1->z + ( KeyVector2->z - KeyVector1->z ) * t ;
						break ;

					case MV1_ANIMKEY_DATATYPE_TRANSLATE :
						Pos.x = KeyVector1->x + ( KeyVector2->x - KeyVector1->x ) * t ;
						Pos.y = KeyVector1->y + ( KeyVector2->y - KeyVector1->y ) * t ;
						Pos.z = KeyVector1->z + ( KeyVector2->z - KeyVector1->z ) * t ;
						break ;
					}
					break ;

				case MV1_ANIMKEY_TYPE_QUATERNION_X :
				case MV1_ANIMKEY_TYPE_QUATERNION_VMD :
					QtType = KeySetBase->Type ;
					q1 = &KeySetBase->KeyFloat4[ NowKey ] ;
					q2 = &KeySetBase->KeyFloat4[ NextKey ] ;

					NowQt = *q1 ;
					NextQt = *q2 ;
					QtRate = t ;
					break ;
				}
			}

			// �����Z�b�g
			Anim->ValidFlag = MV1_ANIMVALUE_MATRIX ;

			if( RotRate > -0.5f )
			{
				switch( RotateOrder )
				{
				case MV1_ROTATE_ORDER_XYZ : CreateRotationXYZMatrix4X4CTF( &RotMat1, NowRot.x, NowRot.y, NowRot.z ) ; CreateRotationXYZMatrix4X4CTF( &RotMat2, NextRot.x, NextRot.y, NextRot.z ) ; break ;
				case MV1_ROTATE_ORDER_XZY : CreateRotationXZYMatrix4X4CTF( &RotMat1, NowRot.x, NowRot.y, NowRot.z ) ; CreateRotationXZYMatrix4X4CTF( &RotMat2, NextRot.x, NextRot.y, NextRot.z ) ; break ;
				case MV1_ROTATE_ORDER_YXZ : CreateRotationYXZMatrix4X4CTF( &RotMat1, NowRot.x, NowRot.y, NowRot.z ) ; CreateRotationYXZMatrix4X4CTF( &RotMat2, NextRot.x, NextRot.y, NextRot.z ) ; break ;
				case MV1_ROTATE_ORDER_YZX : CreateRotationYZXMatrix4X4CTF( &RotMat1, NowRot.x, NowRot.y, NowRot.z ) ; CreateRotationYZXMatrix4X4CTF( &RotMat2, NextRot.x, NextRot.y, NextRot.z ) ; break ;
				case MV1_ROTATE_ORDER_ZXY : CreateRotationZXYMatrix4X4CTF( &RotMat1, NowRot.x, NowRot.y, NowRot.z ) ; CreateRotationZXYMatrix4X4CTF( &RotMat2, NextRot.x, NextRot.y, NextRot.z ) ; break ;
				case MV1_ROTATE_ORDER_ZYX : CreateRotationZYXMatrix4X4CTF( &RotMat1, NowRot.x, NowRot.y, NowRot.z ) ; CreateRotationZYXMatrix4X4CTF( &RotMat2, NextRot.x, NextRot.y, NextRot.z ) ; break ;
				default: return -1;
				}

				Anim->Matrix.m[ 0 ][ 0 ] = RotMat1.m[ 0 ][ 0 ] + ( RotMat2.m[ 0 ][ 0 ] - RotMat1.m[ 0 ][ 0 ] ) * RotRate ;
				Anim->Matrix.m[ 0 ][ 1 ] = RotMat1.m[ 0 ][ 1 ] + ( RotMat2.m[ 0 ][ 1 ] - RotMat1.m[ 0 ][ 1 ] ) * RotRate ;
				Anim->Matrix.m[ 0 ][ 2 ] = RotMat1.m[ 0 ][ 2 ] + ( RotMat2.m[ 0 ][ 2 ] - RotMat1.m[ 0 ][ 2 ] ) * RotRate ;

				Anim->Matrix.m[ 1 ][ 0 ] = RotMat1.m[ 1 ][ 0 ] + ( RotMat2.m[ 1 ][ 0 ] - RotMat1.m[ 1 ][ 0 ] ) * RotRate ;
				Anim->Matrix.m[ 1 ][ 1 ] = RotMat1.m[ 1 ][ 1 ] + ( RotMat2.m[ 1 ][ 1 ] - RotMat1.m[ 1 ][ 1 ] ) * RotRate ;
				Anim->Matrix.m[ 1 ][ 2 ] = RotMat1.m[ 1 ][ 2 ] + ( RotMat2.m[ 1 ][ 2 ] - RotMat1.m[ 1 ][ 2 ] ) * RotRate ;

				Anim->Matrix.m[ 2 ][ 0 ] = RotMat1.m[ 2 ][ 0 ] + ( RotMat2.m[ 2 ][ 0 ] - RotMat1.m[ 2 ][ 0 ] ) * RotRate ;
				Anim->Matrix.m[ 2 ][ 1 ] = RotMat1.m[ 2 ][ 1 ] + ( RotMat2.m[ 2 ][ 1 ] - RotMat1.m[ 2 ][ 1 ] ) * RotRate ;
				Anim->Matrix.m[ 2 ][ 2 ] = RotMat1.m[ 2 ][ 2 ] + ( RotMat2.m[ 2 ][ 2 ] - RotMat1.m[ 2 ][ 2 ] ) * RotRate ;
			}
			else
			if( QtRate > 0.0f )
			{
				switch( QtType )
				{
				case MV1_ANIMKEY_TYPE_QUATERNION_X :
					CreateQuaternionRotateMatrix4X4CTF( &RotMat1, &NowQt  ) ;
					CreateQuaternionRotateMatrix4X4CTF( &RotMat2, &NextQt ) ;
					break ;

				case MV1_ANIMKEY_TYPE_QUATERNION_VMD :
					float x2, y2, z2, xy, yz, zx, xw, yw, zw ;

					x2 = NowQt.x * NowQt.x * 2.0f ;
					y2 = NowQt.y * NowQt.y * 2.0f ;
					z2 = NowQt.z * NowQt.z * 2.0f ;
					xy = NowQt.x * NowQt.y * 2.0f ;
					yz = NowQt.y * NowQt.z * 2.0f ;
					zx = NowQt.z * NowQt.x * 2.0f ;
					xw = NowQt.x * NowQt.w * 2.0f ;
					yw = NowQt.y * NowQt.w * 2.0f ;
					zw = NowQt.z * NowQt.w * 2.0f ;

					RotMat1.m[ 0 ][ 0 ] = 1.0f - y2 - z2 ;
					RotMat1.m[ 1 ][ 0 ] = xy + zw ;
					RotMat1.m[ 2 ][ 0 ] = zx - yw ;
					RotMat1.m[ 0 ][ 1 ] = xy - zw ;
					RotMat1.m[ 1 ][ 1 ] = 1.0f - z2 - x2 ;
					RotMat1.m[ 2 ][ 1 ] = yz + xw ;
					RotMat1.m[ 0 ][ 2 ] = zx + yw ;
					RotMat1.m[ 1 ][ 2 ] = yz - xw ;
					RotMat1.m[ 2 ][ 2 ] = 1.0f - x2 - y2 ;

					x2 = NextQt.x * NextQt.x * 2.0f ;
					y2 = NextQt.y * NextQt.y * 2.0f ;
					z2 = NextQt.z * NextQt.z * 2.0f ;
					xy = NextQt.x * NextQt.y * 2.0f ;
					yz = NextQt.y * NextQt.z * 2.0f ;
					zx = NextQt.z * NextQt.x * 2.0f ;
					xw = NextQt.x * NextQt.w * 2.0f ;
					yw = NextQt.y * NextQt.w * 2.0f ;
					zw = NextQt.z * NextQt.w * 2.0f ;

					RotMat2.m[ 0 ][ 0 ] = 1.0f - y2 - z2 ;
					RotMat2.m[ 1 ][ 0 ] = xy + zw ;
					RotMat2.m[ 2 ][ 0 ] = zx - yw ;
					RotMat2.m[ 0 ][ 1 ] = xy - zw ;
					RotMat2.m[ 1 ][ 1 ] = 1.0f - z2 - x2 ;
					RotMat2.m[ 2 ][ 1 ] = yz + xw ;
					RotMat2.m[ 0 ][ 2 ] = zx + yw ;
					RotMat2.m[ 1 ][ 2 ] = yz - xw ;
					RotMat2.m[ 2 ][ 2 ] = 1.0f - x2 - y2 ;
					break ;

				default :
					return -1 ;
				}

				Anim->Matrix.m[ 0 ][ 0 ] = RotMat1.m[ 0 ][ 0 ] + ( RotMat2.m[ 0 ][ 0 ] - RotMat1.m[ 0 ][ 0 ] ) * QtRate ;
				Anim->Matrix.m[ 0 ][ 1 ] = RotMat1.m[ 0 ][ 1 ] + ( RotMat2.m[ 0 ][ 1 ] - RotMat1.m[ 0 ][ 1 ] ) * QtRate ;
				Anim->Matrix.m[ 0 ][ 2 ] = RotMat1.m[ 0 ][ 2 ] + ( RotMat2.m[ 0 ][ 2 ] - RotMat1.m[ 0 ][ 2 ] ) * QtRate ;

				Anim->Matrix.m[ 1 ][ 0 ] = RotMat1.m[ 1 ][ 0 ] + ( RotMat2.m[ 1 ][ 0 ] - RotMat1.m[ 1 ][ 0 ] ) * QtRate ;
				Anim->Matrix.m[ 1 ][ 1 ] = RotMat1.m[ 1 ][ 1 ] + ( RotMat2.m[ 1 ][ 1 ] - RotMat1.m[ 1 ][ 1 ] ) * QtRate ;
				Anim->Matrix.m[ 1 ][ 2 ] = RotMat1.m[ 1 ][ 2 ] + ( RotMat2.m[ 1 ][ 2 ] - RotMat1.m[ 1 ][ 2 ] ) * QtRate ;

				Anim->Matrix.m[ 2 ][ 0 ] = RotMat1.m[ 2 ][ 0 ] + ( RotMat2.m[ 2 ][ 0 ] - RotMat1.m[ 2 ][ 0 ] ) * QtRate ;
				Anim->Matrix.m[ 2 ][ 1 ] = RotMat1.m[ 2 ][ 1 ] + ( RotMat2.m[ 2 ][ 1 ] - RotMat1.m[ 2 ][ 1 ] ) * QtRate ;
				Anim->Matrix.m[ 2 ][ 2 ] = RotMat1.m[ 2 ][ 2 ] + ( RotMat2.m[ 2 ][ 2 ] - RotMat1.m[ 2 ][ 2 ] ) * QtRate ;
			}
			else
			if( RotRateX < -0.5f && RotRateY < -0.5f && RotRateZ < -0.5f ) 
			{
				if( NowRot.x != 0.0f || NowRot.y != 0.0f || NowRot.z != 0.0f )
				{
					switch( RotateOrder )
					{
					case MV1_ROTATE_ORDER_XYZ : CreateRotationXYZMatrix4X4CTF( &Anim->Matrix, NowRot.x, NowRot.y, NowRot.z ) ; break ;
					case MV1_ROTATE_ORDER_XZY : CreateRotationXZYMatrix4X4CTF( &Anim->Matrix, NowRot.x, NowRot.y, NowRot.z ) ; break ;
					case MV1_ROTATE_ORDER_YXZ : CreateRotationYXZMatrix4X4CTF( &Anim->Matrix, NowRot.x, NowRot.y, NowRot.z ) ; break ;
					case MV1_ROTATE_ORDER_YZX : CreateRotationYZXMatrix4X4CTF( &Anim->Matrix, NowRot.x, NowRot.y, NowRot.z ) ; break ;
					case MV1_ROTATE_ORDER_ZXY : CreateRotationZXYMatrix4X4CTF( &Anim->Matrix, NowRot.x, NowRot.y, NowRot.z ) ; break ;
					case MV1_ROTATE_ORDER_ZYX : CreateRotationZYXMatrix4X4CTF( &Anim->Matrix, NowRot.x, NowRot.y, NowRot.z ) ; break ;
					default :
						return -1 ;
					}
				}
				else
				{
					Anim->Matrix.m[ 0 ][ 0 ] = 1.0f ;
					Anim->Matrix.m[ 0 ][ 1 ] = 0.0f ;
					Anim->Matrix.m[ 0 ][ 2 ] = 0.0f ;

					Anim->Matrix.m[ 1 ][ 0 ] = 0.0f ;
					Anim->Matrix.m[ 1 ][ 1 ] = 1.0f ;
					Anim->Matrix.m[ 1 ][ 2 ] = 0.0f ;

					Anim->Matrix.m[ 2 ][ 0 ] = 0.0f ;
					Anim->Matrix.m[ 2 ][ 1 ] = 0.0f ;
					Anim->Matrix.m[ 2 ][ 2 ] = 1.0f ;
				}
			}
			else
			{
				MATRIX_4X4CT_F RotXMatTemp, RotYMatTemp, RotZMatTemp ;
				MATRIX_4X4CT_F TempMat ;
				float Sin1, Cos1, Sin2, Cos2 ;


				RotXMatTemp.m[ 0 ][ 0 ] = 1.0f ;
				RotXMatTemp.m[ 0 ][ 1 ] = 0.0f ;
				RotXMatTemp.m[ 0 ][ 2 ] = 0.0f ;
				RotXMatTemp.m[ 0 ][ 3 ] = 0.0f ;

				RotXMatTemp.m[ 1 ][ 0 ] = 0.0f ;
				RotXMatTemp.m[ 1 ][ 3 ] = 0.0f ;

				RotXMatTemp.m[ 2 ][ 0 ] = 0.0f ;
				RotXMatTemp.m[ 2 ][ 3 ] = 0.0f ;
				if( RotRateX > -0.5f )
				{
					_TABLE_SINCOS( NowRotX,  &Sin1, &Cos1 ) ;
					_TABLE_SINCOS( NextRotX, &Sin2, &Cos2 ) ;

					RotXMatTemp.m[ 1 ][ 1 ] =  Cos1 + ( Cos2 - Cos1 ) * RotRateX ;
					RotXMatTemp.m[ 2 ][ 1 ] =  Sin1 + ( Sin2 - Sin1 ) * RotRateX ;
				}
				else
				{
					_TABLE_SINCOS( NowRotX,  &Sin1, &Cos1 ) ;

					RotXMatTemp.m[ 1 ][ 1 ] =  Cos1 ;
					RotXMatTemp.m[ 2 ][ 1 ] =  Sin1 ;
				}
				RotXMatTemp.m[ 1 ][ 2 ] = -RotXMatTemp.m[ 2 ][ 1 ] ;
				RotXMatTemp.m[ 2 ][ 2 ] =  RotXMatTemp.m[ 1 ][ 1 ] ;


				RotYMatTemp.m[ 0 ][ 1 ] = 0.0f ;
				RotYMatTemp.m[ 0 ][ 3 ] = 0.0f ;

				RotYMatTemp.m[ 1 ][ 0 ] = 0.0f ;
				RotYMatTemp.m[ 1 ][ 1 ] = 1.0f ;
				RotYMatTemp.m[ 1 ][ 2 ] = 0.0f ;
				RotYMatTemp.m[ 1 ][ 3 ] = 0.0f ;

				RotYMatTemp.m[ 2 ][ 1 ] = 0.0f ;
				RotYMatTemp.m[ 2 ][ 3 ] = 0.0f ;
				if( RotRateY > -0.5f )
				{
					_TABLE_SINCOS( NowRotY,  &Sin1, &Cos1 ) ;
					_TABLE_SINCOS( NextRotY, &Sin2, &Cos2 ) ;

					RotYMatTemp.m[ 0 ][ 0 ] =  Cos1 + ( Cos2 - Cos1 ) * RotRateY ;
					RotYMatTemp.m[ 0 ][ 2 ] =  Sin1 + ( Sin2 - Sin1 ) * RotRateY ;
				}
				else
				{
					_TABLE_SINCOS( NowRotY,  &Sin1, &Cos1 ) ;

					RotYMatTemp.m[ 0 ][ 0 ] =  Cos1 ;
					RotYMatTemp.m[ 0 ][ 2 ] =  Sin1 ;
				}
				RotYMatTemp.m[ 2 ][ 0 ] = -RotYMatTemp.m[ 0 ][ 2 ] ;
				RotYMatTemp.m[ 2 ][ 2 ] =  RotYMatTemp.m[ 0 ][ 0 ] ;


				RotZMatTemp.m[ 0 ][ 2 ] = 0.0f ;
				RotZMatTemp.m[ 0 ][ 3 ] = 0.0f ;

				RotZMatTemp.m[ 1 ][ 2 ] = 0.0f ;
				RotZMatTemp.m[ 1 ][ 3 ] = 0.0f ;

				RotZMatTemp.m[ 2 ][ 0 ] = 0.0f ;
				RotZMatTemp.m[ 2 ][ 1 ] = 0.0f ;
				RotZMatTemp.m[ 2 ][ 2 ] = 1.0f ;
				RotZMatTemp.m[ 2 ][ 3 ] = 0.0f ;
				if( RotRateZ > -0.5f )
				{
					_TABLE_SINCOS( NowRotZ,  &Sin1, &Cos1 ) ;
					_TABLE_SINCOS( NextRotZ, &Sin2, &Cos2 ) ;

					RotZMatTemp.m[ 0 ][ 0 ] = Cos1 + ( Cos2 - Cos1 ) * RotRateZ ;
					RotZMatTemp.m[ 1 ][ 0 ] = Sin1 + ( Sin2 - Sin1 ) * RotRateZ ;
				}
				else
				{
					_TABLE_SINCOS( NowRotZ,  &Sin1, &Cos1 ) ;

					RotZMatTemp.m[ 0 ][ 0 ] = Cos1 ;
					RotZMatTemp.m[ 1 ][ 0 ] = Sin1 ;
				}
				RotZMatTemp.m[ 0 ][ 1 ] = -RotZMatTemp.m[ 1 ][ 0 ] ;
				RotZMatTemp.m[ 1 ][ 1 ] =  RotZMatTemp.m[ 0 ][ 0 ] ;


				switch( RotateOrder )
				{
				case MV1_ROTATE_ORDER_XYZ :
					UnSafeMultiplyMatrix4X4CTF( &TempMat, &RotXMatTemp, &RotYMatTemp ) ;
					UnSafeMultiplyMatrix4X4CTF( &Anim->Matrix, &TempMat, &RotZMatTemp ) ;
					break ;

				case MV1_ROTATE_ORDER_XZY :
					UnSafeMultiplyMatrix4X4CTF( &TempMat, &RotXMatTemp, &RotZMatTemp ) ;
					UnSafeMultiplyMatrix4X4CTF( &Anim->Matrix, &TempMat, &RotYMatTemp ) ;
					break ;

				case MV1_ROTATE_ORDER_YZX :
					UnSafeMultiplyMatrix4X4CTF( &TempMat, &RotYMatTemp, &RotZMatTemp ) ;
					UnSafeMultiplyMatrix4X4CTF( &Anim->Matrix, &TempMat, &RotXMatTemp ) ;
					break ;

				case MV1_ROTATE_ORDER_ZXY :
					UnSafeMultiplyMatrix4X4CTF( &TempMat, &RotZMatTemp, &RotXMatTemp ) ;
					UnSafeMultiplyMatrix4X4CTF( &Anim->Matrix, &TempMat, &RotYMatTemp ) ;
					break ;

				case MV1_ROTATE_ORDER_ZYX :
					UnSafeMultiplyMatrix4X4CTF( &TempMat, &RotZMatTemp, &RotYMatTemp ) ;
					UnSafeMultiplyMatrix4X4CTF( &Anim->Matrix, &TempMat, &RotXMatTemp ) ;
					break ;
				}
			}

			// �O��s��̊|�����킹
			{
				MATRIX_4X4CT_F TempMatrix ;

				if( PreRotate != NULL && PostRotate != NULL )
				{
					UnSafeMultiplyMatrix4X4CTF( &TempMatrix, &Anim->Matrix, &PreRotMat ) ;
					UnSafeMultiplyMatrix4X4CTF( &Anim->Matrix, &PostRotMat, &TempMatrix ) ;
				}
				else
				if( PreRotate != NULL )
				{
					UnSafeMultiplyMatrix4X4CTF( &TempMatrix, &Anim->Matrix, &PreRotMat ) ;
					Anim->Matrix = TempMatrix ;
				}
				else
				if( PostRotate != NULL )
				{
					UnSafeMultiplyMatrix4X4CTF( &TempMatrix, &PostRotMat, &Anim->Matrix ) ;
					Anim->Matrix = TempMatrix ;
				}
			}

			Anim->Matrix.m[ 0 ][ 3 ] = Pos.x ;
			Anim->Matrix.m[ 1 ][ 3 ] = Pos.y ;
			Anim->Matrix.m[ 2 ][ 3 ] = Pos.z ;

			if( Scale.x != 1.0f || Scale.y != 1.0f || Scale.z != 1.0f )
			{
				Anim->Matrix.m[ 0 ][ 0 ] *= Scale.x ;
				Anim->Matrix.m[ 0 ][ 1 ] *= Scale.y ;
				Anim->Matrix.m[ 0 ][ 2 ] *= Scale.z ;
				Anim->Matrix.m[ 1 ][ 0 ] *= Scale.x ;
				Anim->Matrix.m[ 1 ][ 1 ] *= Scale.y ;
				Anim->Matrix.m[ 1 ][ 2 ] *= Scale.z ;
				Anim->Matrix.m[ 2 ][ 0 ] *= Scale.x ;
				Anim->Matrix.m[ 2 ][ 1 ] *= Scale.y ;
				Anim->Matrix.m[ 2 ][ 2 ] *= Scale.z ;
			}

			// �ڕW�m�[�h�̍s����X�V����悤�ɃZ�b�g
			if( Anim->Frame )
			{
				Anim->Frame->Container->LocalWorldMatrixSetupFlag = false ;
				MV1BitSetChange( &Anim->Frame->LocalWorldMatrixChange ) ;
			}
		}
	}
	else
	{
		Anim = AnimSet->Anim ;
		for( i = 0 ; i < AnimSetBase->AnimNum ; i ++, Anim ++ )
		{
			// �Z�b�g�A�b�v�������Z�b�g
			Anim->ValidFlag = 0 ;

			// ������Ԃł͖ڕW�m�[�h�̃f�t�H���g�l�����Ă���
			ValidMatrix = FALSE ;
			if( Anim->Frame )
			{
				Translate   = Anim->Frame->BaseData->Translate ;
				Rotate      = Anim->Frame->BaseData->Rotate ;
				Scale       = Anim->Frame->BaseData->Scale ;
				Quaternion  = Anim->Frame->BaseData->Quaternion ;
			}
			else
			{
				Translate.x = 0.0f ;
				Translate.y = 0.0f ;
				Translate.z = 0.0f ;

				Rotate.x = 0.0f ;
				Rotate.y = 0.0f ;
				Rotate.z = 0.0f ;

				Scale.x = 1.0f ;
				Scale.y = 1.0f ;
				Scale.z = 1.0f ;

				Quaternion.w = 1.0f ;
				Quaternion.x = 0.0f ;
				Quaternion.y = 0.0f ;
				Quaternion.z = 0.0f ;
			}
			RotateOrder = Anim->RotateOrder ;

			// �e�v�f�̌��ݒl����o��
			for( j = 0 ; j < Anim->BaseData->KeySetNum ; j ++ )
			{
				NowKey     = Anim->KeySet[ j ].NowKey ;
				KeySetBase = Anim->KeySet[ j ].BaseData ;

				if( KeySetBase->Num == 0 )
				{
					continue ;
				}

				NextKey    = NowKey == KeySetBase->Num - 1 ? 0 : NowKey + 1 ;

				if( NowKey == KeySetBase->Num - 1 )
				{
					t = 0.0f ;
				}
				else
				{
					if( KeySetBase->TimeType == MV1_ANIMKEY_TIME_TYPE_ONE )
					{
						t = ( AnimSet->NowTime - ( KeySetBase->UnitTime * NowKey + KeySetBase->StartTime ) ) / KeySetBase->UnitTime ;
					}
					else
					{
						t = ( AnimSet->NowTime -   KeySetBase->KeyTime[ NowKey ]                                                                ) /   ( KeySetBase->KeyTime[ NowKey + 1 ]                                                                -   KeySetBase->KeyTime[ NowKey ]                                                                ) ;
					}
					if( t < 0.0f ) t = 0.0f ;
				}

				switch( KeySetBase->Type )
				{
				case MV1_ANIMKEY_TYPE_LINEAR :
					KeyLinear1 = &KeySetBase->KeyLinear[ NowKey  ] ;
					KeyLinear2 = &KeySetBase->KeyLinear[ NextKey ] ;

					Sub = *KeyLinear2 - *KeyLinear1 ;
					// 90�x�ȏ�̋}���ȕω����������ꍇ�͍s��u�����h���s��
					if( KeySetBase->DataType >= MV1_ANIMKEY_DATATYPE_ROTATE_X &&
						KeySetBase->DataType <= MV1_ANIMKEY_DATATYPE_ROTATE_Z &&
						( Sub < -DX_PI_F * 0.5f || Sub > DX_PI_F * 0.5f ) )
						goto MATRIXLINEARBLEND ;
					fdata = *KeyLinear1 + Sub * t ;

					if( AddParam )
					{
						switch( KeySetBase->DataType )
						{
						case MV1_ANIMKEY_DATATYPE_TRANSLATE_X	: Translate.x += fdata ; break ;
						case MV1_ANIMKEY_DATATYPE_TRANSLATE_Y	: Translate.y += fdata ; break ;
						case MV1_ANIMKEY_DATATYPE_TRANSLATE_Z	: Translate.z += fdata ; break ;
						case MV1_ANIMKEY_DATATYPE_SCALE_X :     Scale.x     += fdata ; break ;
						case MV1_ANIMKEY_DATATYPE_SCALE_Y :     Scale.y     += fdata ; break ;
						case MV1_ANIMKEY_DATATYPE_SCALE_Z :     Scale.z     += fdata ; break ;
						case MV1_ANIMKEY_DATATYPE_ROTATE_X :    Rotate.x    += fdata ; break ;
						case MV1_ANIMKEY_DATATYPE_ROTATE_Y :    Rotate.y    += fdata ; break ;
						case MV1_ANIMKEY_DATATYPE_ROTATE_Z :    Rotate.z    += fdata ; break ;
						case MV1_ANIMKEY_DATATYPE_SHAPE :
							Anim->KeySet[ j ].ShapeKeyFactor = fdata ;

							if( Anim->KeySet[ j ].ShapeTargetIndex != -1 )
							{
								float delta ;

								if( Anim->Frame )
								{
									delta = Anim->ShapeRate[ Anim->KeySet[ j ].ShapeTargetIndex ] - Anim->KeySet[ j ].ShapeKeyFactor ;
									if( delta < 0.0f ) delta = -delta ;

									if( delta >= 0.0000001f )
									{
										Anim->Frame->ShapeChangeFlag = true ;
										Anim->Frame->Container->ShapeChangeFlag = true ;
									}
								}

								Anim->ShapeRate[ Anim->KeySet[ j ].ShapeTargetIndex ] = Anim->KeySet[ j ].ShapeKeyFactor ;
							}

//							if( Anim->Frame )
//							{
//								if( Anim->KeySet[ j ].ShapeTargetIndex != -1 )
//								{
//									MV1_SHAPE *Shape ;
//									float delta ;
//
//									Shape = &Anim->Frame->Shape[ Anim->KeySet[ j ].ShapeTargetIndex ] ;
//									if( Anim->Frame->ShapeChangeFlag == false )
//									{
//										delta = Shape->Rate - Anim->KeySet[ j ].ShapeKeyFactor ;
//										if( delta < 0 ) delta = -delta ;
//										if( delta >= 0.0000001f )
//										{
//											Anim->Frame->ShapeChangeFlag = true ;
//											Anim->Frame->Container->ShapeChangeFlag = true ;
//										}
//									}
//									Shape->Rate = Anim->KeySet[ j ].ShapeKeyFactor ;
//								}
//							}
							break ;
						}
					}
					else
					{
						switch( KeySetBase->DataType )
						{
						case MV1_ANIMKEY_DATATYPE_TRANSLATE_X	: Translate.x = fdata ; break ;
						case MV1_ANIMKEY_DATATYPE_TRANSLATE_Y	: Translate.y = fdata ; break ;
						case MV1_ANIMKEY_DATATYPE_TRANSLATE_Z	: Translate.z = fdata ; break ;
						case MV1_ANIMKEY_DATATYPE_SCALE_X :     Scale.x     = fdata ; break ;
						case MV1_ANIMKEY_DATATYPE_SCALE_Y :     Scale.y     = fdata ; break ;
						case MV1_ANIMKEY_DATATYPE_SCALE_Z :     Scale.z     = fdata ; break ;
						case MV1_ANIMKEY_DATATYPE_ROTATE_X :    Rotate.x    = fdata ; break ;
						case MV1_ANIMKEY_DATATYPE_ROTATE_Y :    Rotate.y    = fdata ; break ;
						case MV1_ANIMKEY_DATATYPE_ROTATE_Z :    Rotate.z    = fdata ; break ;
						case MV1_ANIMKEY_DATATYPE_SHAPE :
							Anim->KeySet[ j ].ShapeKeyFactor = fdata ;

							if( Anim->KeySet[ j ].ShapeTargetIndex != -1 )
							{
								float delta ;

								if( Anim->Frame )
								{
									delta = Anim->ShapeRate[ Anim->KeySet[ j ].ShapeTargetIndex ] - Anim->KeySet[ j ].ShapeKeyFactor ;
									if( delta < 0.0f ) delta = -delta ;

									if( delta >= 0.0000001f )
									{
										Anim->Frame->ShapeChangeFlag = true ;
										Anim->Frame->Container->ShapeChangeFlag = true ;
									}
								}

								Anim->ShapeRate[ Anim->KeySet[ j ].ShapeTargetIndex ] = Anim->KeySet[ j ].ShapeKeyFactor ;
							}

//							if( Anim->Frame )
//							{
//								if( Anim->KeySet[ j ].ShapeTargetIndex != -1 )
//								{
//									MV1_SHAPE *Shape ;
//									float delta ;
//
//									Shape = &Anim->Frame->Shape[ Anim->KeySet[ j ].ShapeTargetIndex ] ;
//									if( Anim->Frame->ShapeChangeFlag == false )
//									{
//										delta = Shape->Rate - Anim->KeySet[ j ].ShapeKeyFactor ;
//										if( delta < 0 ) delta = -delta ;
//										if( delta >= 0.0000001f )
//										{
//											Anim->Frame->ShapeChangeFlag = true ;
//											Anim->Frame->Container->ShapeChangeFlag = true ;
//										}
//									}
//									Shape->Rate = Anim->KeySet[ j ].ShapeKeyFactor ;
//								}
//							}
							break ;
						}
					}
					break ;

				case MV1_ANIMKEY_TYPE_VECTOR :
					KeyVector1 = &KeySetBase->KeyVector[ NowKey ] ;
					KeyVector2 = &KeySetBase->KeyVector[ NextKey ] ;

					SubVec.x = KeyVector2->x - KeyVector1->x ;
					SubVec.y = KeyVector2->y - KeyVector1->y ;
					SubVec.z = KeyVector2->z - KeyVector1->z ;
					// 90�x�ȏ�̋}���ȕω����������ꍇ�͍s��u�����h���s��
					if( KeySetBase->DataType == MV1_ANIMKEY_DATATYPE_ROTATE &&
						( SubVec.x < -DX_PI_F * 0.5f || SubVec.x > DX_PI_F * 0.5f ||
						  SubVec.y < -DX_PI_F * 0.5f || SubVec.y > DX_PI_F * 0.5f ||
						  SubVec.z < -DX_PI_F * 0.5f || SubVec.z > DX_PI_F * 0.5f ) )
						goto MATRIXLINEARBLEND ;
					vdata.x = KeyVector1->x + SubVec.x * t ;
					vdata.y = KeyVector1->y + SubVec.y * t ;
					vdata.z = KeyVector1->z + SubVec.z * t ;

					if( AddParam )
					{
						switch( KeySetBase->DataType )
						{
						case MV1_ANIMKEY_DATATYPE_ROTATE :     Rotate     = VAdd( Rotate,    vdata ) ; break ;
						case MV1_ANIMKEY_DATATYPE_SCALE :      Scale      = VAdd( Scale,     vdata ) ; break ;
						case MV1_ANIMKEY_DATATYPE_TRANSLATE :  Translate  = VAdd( Translate, vdata ) ; break ;
						}
					}
					else
					{
						switch( KeySetBase->DataType )
						{
						case MV1_ANIMKEY_DATATYPE_ROTATE :     Rotate     = vdata ; break ;
						case MV1_ANIMKEY_DATATYPE_SCALE :      Scale      = vdata ; break ;
						case MV1_ANIMKEY_DATATYPE_TRANSLATE :  Translate  = vdata ; break ;
						}
					}
					break ;

				case MV1_ANIMKEY_TYPE_QUATERNION_X :
				case MV1_ANIMKEY_TYPE_QUATERNION_VMD :
					QtType = KeySetBase->Type ;
					q1 = &KeySetBase->KeyFloat4[ NowKey ] ;
					q2 = &KeySetBase->KeyFloat4[ NextKey ] ;

#if 0
					// ���ʐ��`���
					float d, f, angle, sin1, sin2, sin3 ;
					f = q2->x * q1->x + q2->y * q1->y + q2->z * q1->z + q2->w * q1->w ;
					if( f >  0.999999f ) f =  0.999999f ;
					if( f < -0.999999f ) f = -0.999999f ;

					angle = _ACOS( f ) ;

					_TABLE_SINCOS( angle                , &sin1, &d ) ;
					_TABLE_SINCOS( angle * ( 1.0f - t ) , &sin2, &d ) ;
					_TABLE_SINCOS( angle * t            , &sin3, &d ) ;
					sin1 = 1.0f / sin1 ;

					Quaternion.x = ( q1->x * sin2 + q2->x * sin3 ) * sin1 ;
					Quaternion.y = ( q1->y * sin2 + q2->y * sin3 ) * sin1 ;
					Quaternion.z = ( q1->z * sin2 + q2->z * sin3 ) * sin1 ;
					Quaternion.w = ( q1->w * sin2 + q2->w * sin3 ) * sin1 ;
#endif
#if 0
					float	qr = q1->x * q2->x + q1->y * q2->y + q1->z * q2->z + q1->w * q2->w ;
					float	t0 = 1.0f - t ;
					FLOAT4  qdata ;

					if( qr < 0 )
					{
						qdata.x = q1->x * t0 - q2->x * t ;
						qdata.y = q1->y * t0 - q2->y * t ;
						qdata.z = q1->z * t0 - q2->z * t ;
						qdata.w = q1->w * t0 - q2->w * t ;
					}
					else
					{
						qdata.x = q1->x * t0 + q2->x * t ;
						qdata.y = q1->y * t0 + q2->y * t ;
						qdata.z = q1->z * t0 + q2->z * t ;
						qdata.w = q1->w * t0 + q2->w * t ;
					}

					float Sqr = 1.0f / _SQRT( qdata.x * qdata.x + qdata.y * qdata.y + qdata.z * qdata.z + qdata.w * qdata.w ) ;

					Quaternion.x = qdata.x * Sqr ;
					Quaternion.y = qdata.y * Sqr ;
					Quaternion.z = qdata.z * Sqr ;
					Quaternion.w = qdata.w * Sqr ;
#endif
#if 1
					_MV1SphereLinear( q1, q2, t, &Quaternion ) ;
#endif
#if 0
					Quaternion = *q1 ;
#endif
					break ;

				case MV1_ANIMKEY_TYPE_MATRIX4X4C :
					KeyMatrix4x4C1 = &KeySetBase->KeyMatrix4x4C[ NowKey  ] ;
					KeyMatrix4x4C2 = &KeySetBase->KeyMatrix4x4C[ NextKey ] ;

					ValidMatrix = TRUE ;
					Matrix.m[ 0 ][ 0 ] = ( KeyMatrix4x4C2->Matrix[ 0 ][ 0 ] - KeyMatrix4x4C1->Matrix[ 0 ][ 0 ] ) * t + KeyMatrix4x4C1->Matrix[ 0 ][ 0 ] ;
					Matrix.m[ 0 ][ 1 ] = ( KeyMatrix4x4C2->Matrix[ 1 ][ 0 ] - KeyMatrix4x4C1->Matrix[ 1 ][ 0 ] ) * t + KeyMatrix4x4C1->Matrix[ 1 ][ 0 ] ;
					Matrix.m[ 0 ][ 2 ] = ( KeyMatrix4x4C2->Matrix[ 2 ][ 0 ] - KeyMatrix4x4C1->Matrix[ 2 ][ 0 ] ) * t + KeyMatrix4x4C1->Matrix[ 2 ][ 0 ] ;
					Matrix.m[ 0 ][ 3 ] = ( KeyMatrix4x4C2->Matrix[ 3 ][ 0 ] - KeyMatrix4x4C1->Matrix[ 3 ][ 0 ] ) * t + KeyMatrix4x4C1->Matrix[ 3 ][ 0 ] ;

					Matrix.m[ 1 ][ 0 ] = ( KeyMatrix4x4C2->Matrix[ 0 ][ 1 ] - KeyMatrix4x4C1->Matrix[ 0 ][ 1 ] ) * t + KeyMatrix4x4C1->Matrix[ 0 ][ 1 ] ;
					Matrix.m[ 1 ][ 1 ] = ( KeyMatrix4x4C2->Matrix[ 1 ][ 1 ] - KeyMatrix4x4C1->Matrix[ 1 ][ 1 ] ) * t + KeyMatrix4x4C1->Matrix[ 1 ][ 1 ] ;
					Matrix.m[ 1 ][ 2 ] = ( KeyMatrix4x4C2->Matrix[ 2 ][ 1 ] - KeyMatrix4x4C1->Matrix[ 2 ][ 1 ] ) * t + KeyMatrix4x4C1->Matrix[ 2 ][ 1 ] ;
					Matrix.m[ 1 ][ 3 ] = ( KeyMatrix4x4C2->Matrix[ 3 ][ 1 ] - KeyMatrix4x4C1->Matrix[ 3 ][ 1 ] ) * t + KeyMatrix4x4C1->Matrix[ 3 ][ 1 ] ;

					Matrix.m[ 2 ][ 0 ] = ( KeyMatrix4x4C2->Matrix[ 0 ][ 2 ] - KeyMatrix4x4C1->Matrix[ 0 ][ 2 ] ) * t + KeyMatrix4x4C1->Matrix[ 0 ][ 2 ] ;
					Matrix.m[ 2 ][ 1 ] = ( KeyMatrix4x4C2->Matrix[ 1 ][ 2 ] - KeyMatrix4x4C1->Matrix[ 1 ][ 2 ] ) * t + KeyMatrix4x4C1->Matrix[ 1 ][ 2 ] ;
					Matrix.m[ 2 ][ 2 ] = ( KeyMatrix4x4C2->Matrix[ 2 ][ 2 ] - KeyMatrix4x4C1->Matrix[ 2 ][ 2 ] ) * t + KeyMatrix4x4C1->Matrix[ 2 ][ 2 ] ;
					Matrix.m[ 2 ][ 3 ] = ( KeyMatrix4x4C2->Matrix[ 3 ][ 2 ] - KeyMatrix4x4C1->Matrix[ 3 ][ 2 ] ) * t + KeyMatrix4x4C1->Matrix[ 3 ][ 2 ] ;
					break ;
				}
			}

			// �����Z�b�g
			if( ValidMatrix )
			{
				Anim->Matrix = Matrix ;
				Anim->ValidFlag = MV1_ANIMVALUE_MATRIX ;
			}
			else
			{
				Anim->Translate = Translate ;
				if( Translate.x != 0.0f || Translate.y != 0.0f || Translate.z != 0.0f )
					Anim->ValidFlag |= MV1_ANIMVALUE_TRANSLATE ;

				Anim->Scale = Scale ;
				if( Scale.x != 1.0f || Scale.y != 1.0f || Scale.z != 1.0f )
					Anim->ValidFlag |= MV1_ANIMVALUE_SCALE ;

				Anim->Rotate = Rotate ;
				Anim->RotateOrder = RotateOrder ;
				if( Rotate.x != 0.0f || Rotate.y != 0.0f || Rotate.z != 0.0f )
					Anim->ValidFlag |= MV1_ANIMVALUE_ROTATE ;

				Anim->Quaternion = Quaternion ;
				if( Quaternion.w != 1.0f || Quaternion.x != 0.0f || Quaternion.y != 0.0f || Quaternion.z != 0.0f )
				{
					Anim->ValidFlag |= QtType == MV1_ANIMKEY_TYPE_QUATERNION_X ? MV1_ANIMVALUE_QUATERNION_X : MV1_ANIMVALUE_QUATERNION_VMD ;
					Anim->ValidFlag &= ~MV1_ANIMVALUE_ROTATE ;
				}
			}

			// �ڕW�m�[�h�̍s����X�V����悤�ɃZ�b�g
			if( Anim->Frame )
			{
				Anim->Frame->Container->LocalWorldMatrixSetupFlag = false ;
				MV1BitSetChange( &Anim->Frame->LocalWorldMatrixChange ) ;
			}

			// �u�����h�s����쐬�����t���O��|��
			Anim->ValidBlendMatrix = false ;
		}
	}

	// �p�����[�^�̃Z�b�g�A�b�v���ς񂾃t���O�𗧂Ă�
	AnimSet->ParamSetup = true ;

	// �I��
	return 0 ;
}







// �������Ǘ��n

// �������̈�̊m��
#if !defined( __BCC ) || defined( _DEBUG )
extern void *AddMemArea( size_t Size, MEMINFO **FirstMem, const char *FileName, int Line )
#else
extern void *AddMemArea( size_t Size, MEMINFO **FirstMem )
#endif
{
	MEMINFO *MI ;

	// �������A�h���X�ۑ��f�[�^��ۑ����郁�����̈�Ǝ��ۂ̃f�[�^��ۑ����郁�����̈���m�ۂ���
#if !defined( __BCC ) || defined( _DEBUG )
	if( ( MI = ( MEMINFO * )NS_DxAlloc( sizeof( MEMINFO ) + Size , FileName, Line ) ) == NULL )
#else
	if( ( MI = ( MEMINFO * )NS_DxAlloc( sizeof( MEMINFO ) + Size ) ) == NULL )
#endif
	{
		DXST_ERRORLOG_ADDUTF16LE( "\xe1\x30\xe2\x30\xea\x30\xa2\x30\xc9\x30\xec\x30\xb9\x30\xdd\x4f\x58\x5b\xc7\x30\xfc\x30\xbf\x30\x0b\xff\x9f\x5b\x9b\x96\x6e\x30\xc7\x30\xfc\x30\xbf\x30\x92\x30\xdd\x4f\x58\x5b\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�������A�h���X�ۑ��f�[�^�{���ۂ̃f�[�^��ۑ����郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ ) ;
		return NULL ;
	}

	// �f�[�^���N���A
	_MEMSET( MI, 0, sizeof( MEMINFO ) + Size ) ;

	// ���ۂ̃f�[�^�̗̈���Z�b�g����
	MI->Data = ( BYTE * )MI + sizeof( MEMINFO ) ;

	// �f�[�^��������
	MI->PrevInfo = NULL ;
	MI->NextInfo = *FirstMem ;
	if( *FirstMem == NULL )
	{
		MI->TotalSize = ( int )Size ;
	}
	else
	{
		( *FirstMem )->PrevInfo = MI ;
		MI->TotalSize = ( int )( (*FirstMem)->TotalSize + Size ) ;
	}
	*FirstMem = MI;

	// �m�ۂ����������̈�̃A�h���X��Ԃ�
	return MI->Data ;
}

// �������̈�̉��
extern int SubMemArea( MEMINFO **FirstMem, void *Buffer )
{
	MEMINFO *MI/*, *MIB*/ ;

	if( Buffer == NULL )
		return 0 ;

	MI = ( MEMINFO * )( ( BYTE * )Buffer - sizeof( MEMINFO ) ) ;

	if( MI->NextInfo )
	{
		MI->NextInfo->PrevInfo = MI->PrevInfo ;
	}

	if( MI->PrevInfo )
	{
		MI->PrevInfo->NextInfo = MI->NextInfo ;
	}

	if( MI == *FirstMem )
	{
		*FirstMem = MI->NextInfo ;
	}

	DXFREE( MI ) ;

/*
	// �w��̃������̈����������
	MI = *FirstMem ;
	MIB = NULL ;
	while( MI && MI->Data != Buffer )
	{
		MIB = MI ;
		MI = MI->NextInfo ;
	}

	// ��������������
	if( MI != NULL )
	{
		if( MIB == NULL )
		{
			*FirstMem = MI->NextInfo ;
		}
		else
		{
			MIB->NextInfo = MI->NextInfo ;
		}
		DXFREE( MI ) ;
	}
*/

	// �I��
	return 0 ;
}

// �S�Ẵ������̈�̉��
extern int ClearMemArea( MEMINFO **FirstMem )
{
	MEMINFO *MI, *MIB ;

	// ���f���� AddMemArea �֐��Ŋm�ۂ��Ă����������̈�����ׂĉ��
	MI = *FirstMem ;
	while( MI != NULL )
	{
		MIB = MI->NextInfo ;
		DXFREE( MI ) ;
		MI = MIB ;
	}

	// �m�t�k�k���Z�b�g
	*FirstMem = NULL ;

	// �I��
	return 0 ;
}
























// �f�[�^��v�����n

// �o�C�i���C���[�W����f�[�^��v�����f�[�^���쐬����
extern void MakeDataCmpInfo( DATACMPINFO *CmpInfo, void *Image, int Size )
{
	int PackNum, AmariNum, i ;
	DWORD *Src ;

	// �[��������
	_MEMSET( CmpInfo, 0, sizeof( DATACMPINFO ) ) ;

	// �T�C�Y��ۑ�
	CmpInfo->Size = ( DWORD )Size ;

	// �R�Q�o�C�g���������ׂ�
	PackNum = Size / 32 ;

	// �p�b�N�̐������`�F�b�N�p�f�[�^���쐬
	Src = ( DWORD * )Image ;
	for( i = PackNum ; i ; i --, Src += 8 )
	{
		CmpInfo->Data[ 0 ] += Src[ 0 ] ;
		CmpInfo->Data[ 1 ] += Src[ 1 ] ;
		CmpInfo->Data[ 2 ] += Src[ 2 ] ;
		CmpInfo->Data[ 3 ] += Src[ 3 ] ;
		CmpInfo->Data[ 4 ] += Src[ 4 ] ;
		CmpInfo->Data[ 5 ] += Src[ 5 ] ;
		CmpInfo->Data[ 6 ] += Src[ 6 ] ;
		CmpInfo->Data[ 7 ] += Src[ 7 ] ;
	}

	// �R�Q�o�C�g�̗]��̕�����ǉ�
	AmariNum = Size - PackNum * 32 ;
	for( i = 0 ; i < AmariNum ; i ++ )
	{
		( ( BYTE * )CmpInfo->Data )[ i ] += ( ( BYTE * )Src )[ i ] ;
	}
}

// ��̃`�F�b�N�f�[�^����v���Ă��邩�ǂ������擾����( 1:��v���Ă���  0:��v���Ă��Ȃ� )
extern int CheckDataCmpInfo( DATACMPINFO *CmpInfo1, DATACMPINFO *CmpInfo2 )
{
	return CmpInfo1->Size != CmpInfo2->Size ||
			CmpInfo1->Data[ 0 ] != CmpInfo2->Data[ 0 ] ||
			CmpInfo1->Data[ 1 ] != CmpInfo2->Data[ 1 ] ||
			CmpInfo1->Data[ 2 ] != CmpInfo2->Data[ 2 ] ||
			CmpInfo1->Data[ 3 ] != CmpInfo2->Data[ 3 ] ||
			CmpInfo1->Data[ 4 ] != CmpInfo2->Data[ 4 ] ||
			CmpInfo1->Data[ 5 ] != CmpInfo2->Data[ 5 ] ||
			CmpInfo1->Data[ 6 ] != CmpInfo2->Data[ 6 ] ||
			CmpInfo1->Data[ 7 ] != CmpInfo2->Data[ 7 ] ? 0 : 1 ;
}










// �X�V���o���\�z�p�֐�

// ��ԕω��Ǘ��p�\���̂̃Z�b�g�A�b�v���s��
extern void MV1ChangeInfoSetup( MV1_CHANGE_BASE *ChangeB, void *FillTable, int BitAddress, int FillBitNum )
{
	DWORD *Dest ;
	DWORD Bit, i ;

	ChangeB->Target   = ( DWORD )( BitAddress / 32 ) ;
	ChangeB->Fill     = ( DWORD * )FillTable ;
	ChangeB->CheckBit = ( DWORD )( 1 << ( BitAddress % 32 ) ) ;
	ChangeB->Size     = ( DWORD )( ( BitAddress % 32 + FillBitNum + 31 ) / 32 ) ;

	if( ChangeB->Fill )
	{
		Bit = ChangeB->CheckBit ;
		Dest = ChangeB->Fill ;
		*Dest = 0 ;
		for( i = 0 ; i < ( DWORD )FillBitNum ; i ++ )
		{
			*Dest |= Bit ;
			Bit <<= 1 ;
			if( Bit == 0 )
			{
				Dest ++ ;
				Bit = 1 ;
			}
		}
	}
}





// ���f���ǂݍ��ݕ⏕�n

// MV1_MAKEVERTINDEXINFO �𗘗p������������f�[�^������⏕����֐�
extern int AddVertexInfo( MV1_MAKEVERTINDEXINFO **InfoTable, MV1_MAKEVERTINDEXINFO *InfoBuffer, int *InfoNum, void *DataBuffer, int DataUnitSize, int DataNum, int AddDataIndex, void *AddData )
{
	MV1_MAKEVERTINDEXINFO *IInfo ;

	for( IInfo = InfoTable[ AddDataIndex ] ; IInfo ; IInfo = IInfo->Next )
	{
		if( _MEMCMP( ( BYTE * )DataBuffer + IInfo->VertexIndex * DataUnitSize, AddData, DataUnitSize ) == 0 )
			return IInfo->VertexIndex ;
	}

	IInfo = &InfoBuffer[ *InfoNum ] ;
	( *InfoNum ) ++ ;

	IInfo->VertexIndex = DataNum ;
	IInfo->Next = InfoTable[ AddDataIndex ] ;
	InfoTable[ AddDataIndex ] = IInfo ;

	return -1 ;
}


// ���b�V���̏]�@���Ɛڐ����v�Z����
extern int MV1MakeMeshBinormalsAndTangents( MV1_MESH_BASE *Mesh )
{
	int i, VertUnitSize, PosUnitSize, NormalNum ;
	MV1_MESH_FACE *Face ;
	MV1_MESH_NORMAL *Normal, *Norm[ 3 ], *NM ;
	MV1_MESH_POSITION *Position, *Pos[ 3 ] ;
	MV1_MESH_VERTEX *Vertex, *Vert[ 3 ], *VT ;
	VECTOR v1, v2, vt, du, dv, vb, vn ;
	BYTE *UseFlag ;

	// �⏕���̏���
	Normal = Mesh->Container->Normal ;
	NormalNum = Mesh->Container->NormalNum ;
	Position = Mesh->Container->Position ;
	Vertex = Mesh->Vertex ;
	VertUnitSize = Mesh->VertUnitSize ;
	PosUnitSize = Mesh->Container->PosUnitSize ;

	// �g�p���Ă��钸�_�̃e�[�u�����쐬����
	{
		UseFlag = ( BYTE * )DXALLOC( sizeof( BYTE ) * NormalNum ) ;
		if( UseFlag == NULL )
		{
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\x02\x98\xb9\x70\x6e\x30\xa5\x63\xda\x7d\x68\x30\x93\x5f\xd5\x6c\xda\x7d\x6e\x30\x5c\x4f\x10\x62\x5c\x4f\x6d\x69\x6b\x30\xc5\x5f\x81\x89\x6a\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"���_�̐ڐ��Ə]�@���̍쐬��ƂɕK�v�ȃ������̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
			return -1 ;
		}
		_MEMSET( UseFlag, 0, ( size_t )Mesh->Container->NormalNum ) ;

		VT = Vertex ;
		for( i = 0 ; i < Mesh->VertexNum ; i ++, VT = ( MV1_MESH_VERTEX * )( ( BYTE * )VT + VertUnitSize ) )
		{
			UseFlag[ VT->NormalIndex ] = 1 ;
		}
	}

	// �ڐ��Ə]�@���̏�����
	NM = Normal ;
	for( i = 0 ; i < NormalNum ; i ++, NM ++ )
	{
		if( UseFlag[ i ] == 0 ) continue ;
		NM->Binormal.x = 0.0f ;
		NM->Binormal.y = 0.0f ;
		NM->Binormal.z = 0.0f ;
		NM->Tangent.x = 0.0f ;
		NM->Tangent.y = 0.0f ;
		NM->Tangent.z = 0.0f ;
	}

	// �S�Ă̖ʂ̐������J��Ԃ�
	Face = Mesh->Face ;
	for( i = 0 ; i < Mesh->FaceNum ; i ++, Face ++ )
	{
		Vert[ 0 ] = ( MV1_MESH_VERTEX   * )( ( BYTE * )Vertex   + VertUnitSize * Face->VertexIndex[ 0 ] ) ;
		Vert[ 1 ] = ( MV1_MESH_VERTEX   * )( ( BYTE * )Vertex   + VertUnitSize * Face->VertexIndex[ 1 ] ) ;
		Vert[ 2 ] = ( MV1_MESH_VERTEX   * )( ( BYTE * )Vertex   + VertUnitSize * Face->VertexIndex[ 2 ] ) ;
		Pos[ 0 ]  = ( MV1_MESH_POSITION * )( ( BYTE * )Position + PosUnitSize  * Vert[ 0 ]->PositionIndex ) ;
		Pos[ 1 ]  = ( MV1_MESH_POSITION * )( ( BYTE * )Position + PosUnitSize  * Vert[ 1 ]->PositionIndex ) ;
		Pos[ 2 ]  = ( MV1_MESH_POSITION * )( ( BYTE * )Position + PosUnitSize  * Vert[ 2 ]->PositionIndex ) ;
		Norm[ 0 ] = Normal + Vert[ 0 ]->NormalIndex ;
		Norm[ 1 ] = Normal + Vert[ 1 ]->NormalIndex ;
		Norm[ 2 ] = Normal + Vert[ 2 ]->NormalIndex ;

		v1.x = Pos[ 1 ]->Position.x - Pos[ 0 ]->Position.x ;
		v1.y = Vert[ 1 ]->UVs[ 0 ][ 0 ] - Vert[ 0 ]->UVs[ 0 ][ 0 ] ;
		v1.z = Vert[ 1 ]->UVs[ 0 ][ 1 ] - Vert[ 0 ]->UVs[ 0 ][ 1 ] ;

		v2.x = Pos[ 2 ]->Position.x - Pos[ 0 ]->Position.x ;
		v2.y = Vert[ 2 ]->UVs[ 0 ][ 0 ] - Vert[ 0 ]->UVs[ 0 ][ 0 ] ;
		v2.z = Vert[ 2 ]->UVs[ 0 ][ 1 ] - Vert[ 0 ]->UVs[ 0 ][ 1 ] ;

		vt = VCross( v1, v2 ) ;
		du.x = 1.0f ;
		if( VDot( vt, vt ) >= 0.0000001f )
		{
			du.x = -vt.y / vt.x ;
			dv.x = -vt.z / vt.x ;
		}

		v1.x = Pos[ 1 ]->Position.y - Pos[ 0 ]->Position.y ;
		v2.x = Pos[ 2 ]->Position.y - Pos[ 0 ]->Position.y ;

		vt = VCross( v1, v2 ) ;
		du.y = 1.0f ;
		if( VDot( vt, vt ) >= 0.0000001f )
		{
			du.y = -vt.y / vt.x ;
			dv.y = -vt.z / vt.x ;
		}

		v1.x = Pos[ 1 ]->Position.z - Pos[ 0 ]->Position.z ;
		v2.x = Pos[ 2 ]->Position.z - Pos[ 0 ]->Position.z ;

		vt = VCross( v1, v2 ) ;
		du.z = 1.0f ;
		if( VDot( vt, vt ) >= 0.0000001f )
		{
			du.z = -vt.y / vt.x ;
			dv.z = -vt.z / vt.x ;
		}

		VectorAdd( &Norm[ 0 ]->Tangent, &Norm[ 0 ]->Tangent, &du ) ;
		VectorAdd( &Norm[ 1 ]->Tangent, &Norm[ 1 ]->Tangent, &du ) ;
		VectorAdd( &Norm[ 2 ]->Tangent, &Norm[ 2 ]->Tangent, &du ) ;

		VectorAdd( &Norm[ 0 ]->Binormal, &Norm[ 0 ]->Binormal, &dv ) ;
		VectorAdd( &Norm[ 1 ]->Binormal, &Norm[ 1 ]->Binormal, &dv ) ;
		VectorAdd( &Norm[ 2 ]->Binormal, &Norm[ 2 ]->Binormal, &dv ) ;
	}

	// �@���̎Z�o�Ɛ��K��
	NM = Normal ;
	for( i = 0 ; i < NormalNum ; i ++, NM ++ )
	{
		if( UseFlag[ i ] == 0 ) continue ;

		vt = VNorm( NM->Tangent ) ;
		vn = VNorm( VCross( vt, NM->Binormal ) ) ;
		vb = VNorm( VCross( vn, vt ) ) ;

		// �@���̕������m�F
		if( VDot( vn, NM->Normal ) < 0.0f )
		{
			vn.x = -vn.x ;
			vn.y = -vn.y ;
			vn.z = -vn.z ;
		}

		// ���K��
		NM->Tangent  = vt ;
		NM->Binormal = vb ;
//		NM->Normal   = vn ;
	}

	// �������̉��
	DXFREE( UseFlag ) ;

	// �I��
	return 0 ;
}

// �g���C�A���O�����X�g�̍��W�Ɩ@�����̃Z�b�g�A�b�v���s��
extern int MV1SetupTriangleListPositionAndNormal( MV1_TRIANGLE_LIST_BASE *List )
{
	int i, j, k ;
	MV1_TLIST_NORMAL_POS *NrmPos ;
	MV1_TLIST_SKIN_POS_4B *SK4BPos ;
	MV1_TLIST_SKIN_POS_8B *SK8BPos ;
	MV1_TLIST_SKIN_POS_FREEB *SKFBPos ;
	DWORD *MInd ;
	MV1_MESH_VERTEX *Vert ;
	MV1_MESH_POSITION *Pos ;
	MV1_MESH_NORMAL *Nrm ;
	MV1_SKINBONE_BLEND *VBlend ;
	MV1_MESH_BASE *Mesh ;
	MV1_FRAME_BASE *Frame ;

	Mesh = List->Container ;
	Frame = Mesh->Container ;

	MInd = List->MeshVertexIndex ;
	switch( List->VertexType )
	{
	case MV1_VERTEX_TYPE_NORMAL :
		NrmPos = ( MV1_TLIST_NORMAL_POS * )ADDR16( List->NormalPosition ) ;
		for( i = 0 ; i < List->VertexNum ; i ++, NrmPos ++, MInd ++ )
		{
			Vert = ( MV1_MESH_VERTEX * )( ( BYTE * )Mesh->Vertex + *MInd * Mesh->VertUnitSize ) ;
			Pos = ( MV1_MESH_POSITION * )( ( BYTE * )Frame->Position + Vert->PositionIndex * Frame->PosUnitSize ) ;
			Nrm = Frame->Normal + Vert->NormalIndex ;

			*( ( VECTOR * )&NrmPos->Position ) = Pos->Position ;
			NrmPos->Position.w = 1.0f ;
			*( ( VECTOR * )&NrmPos->Normal ) = Nrm->Normal ;
			NrmPos->Normal.w = 0.0f ;
		}
		break ;

	case MV1_VERTEX_TYPE_SKIN_4BONE :
		SK4BPos = ( MV1_TLIST_SKIN_POS_4B * )ADDR16( List->SkinPosition4B ) ;
		for( i = 0 ; i < List->VertexNum ; i ++, SK4BPos ++, MInd ++ )
		{
			Vert = ( MV1_MESH_VERTEX * )( ( BYTE * )Mesh->Vertex + *MInd * Mesh->VertUnitSize ) ;
			Pos = ( MV1_MESH_POSITION * )( ( BYTE * )Frame->Position + Vert->PositionIndex * Frame->PosUnitSize ) ;
			Nrm = Frame->Normal + Vert->NormalIndex ;

			*( ( VECTOR * )&SK4BPos->Position ) = Pos->Position ;
			SK4BPos->Position.w = 1.0f ;
			SK4BPos->Normal = Nrm->Normal ;

			VBlend = Pos->BoneWeight ;
			for( j = 0 ; j < Frame->UseSkinBoneNum && j < 4 && VBlend->Index != -1 ; j ++, VBlend ++ )
			{
				SK4BPos->MatrixWeight[ j ] = VBlend->W ;
				for( k = 0 ; k < List->UseBoneNum && List->UseBone[ k ] != VBlend->Index ; k ++ ){}
				SK4BPos->MatrixIndex[ j ] = ( BYTE )k ;
			}
			for( ; j < 4 ; j ++ )
			{
				SK4BPos->MatrixWeight[ j ] = 0.0f ;
				SK4BPos->MatrixIndex[ j ] = 0 ;
			}
		}
		break ;

	case MV1_VERTEX_TYPE_SKIN_8BONE :
		SK8BPos = ( MV1_TLIST_SKIN_POS_8B * )ADDR16( List->SkinPosition8B ) ;
		for( i = 0 ; i < List->VertexNum ; i ++, SK8BPos ++, MInd ++ )
		{
			Vert = ( MV1_MESH_VERTEX * )( ( BYTE * )Mesh->Vertex + *MInd * Mesh->VertUnitSize ) ;
			Pos = ( MV1_MESH_POSITION * )( ( BYTE * )Frame->Position + Vert->PositionIndex * Frame->PosUnitSize ) ;
			Nrm = Frame->Normal + Vert->NormalIndex ;

			SK8BPos->Position = Pos->Position ;
			SK8BPos->Normal = Nrm->Normal ;

			VBlend = Pos->BoneWeight ;
			for( j = 0 ; j < Frame->UseSkinBoneNum && j < 8 && VBlend->Index != -1 ; j ++, VBlend ++ )
			{
				SK8BPos->MatrixWeight[ j ] = VBlend->W ;

				for( k = 0 ; k < List->UseBoneNum && List->UseBone[ k ] != VBlend->Index ; k ++ ){}
				if( j < 4 )
				{
					SK8BPos->MatrixIndex1[ j ] = ( BYTE )k ;
				}
				else
				{
					SK8BPos->MatrixIndex2[ j - 4 ] = ( BYTE )k ;
				}
			}
			for( ; j < 8 ; j ++ )
			{
				SK8BPos->MatrixWeight[ j ] = 0.0f ;
				if( j < 4 )
				{
					SK8BPos->MatrixIndex1[ j ] = 0 ;
				}
				else
				{
					SK8BPos->MatrixIndex2[ j - 4 ] = 0 ;
				}
			}
		}
		break ;

	case MV1_VERTEX_TYPE_SKIN_FREEBONE :
		SKFBPos = ( MV1_TLIST_SKIN_POS_FREEB * )ADDR16( List->SkinPositionFREEB ) ;
		for( i = 0 ; i < List->VertexNum ; i ++, SKFBPos = ( MV1_TLIST_SKIN_POS_FREEB * )( ( BYTE * )SKFBPos + List->PosUnitSize ), MInd ++ )
		{
			Vert = ( MV1_MESH_VERTEX * )( ( BYTE * )Mesh->Vertex + *MInd * Mesh->VertUnitSize ) ;
			Pos = ( MV1_MESH_POSITION * )( ( BYTE * )Frame->Position + Vert->PositionIndex * Frame->PosUnitSize ) ;
			Nrm = Frame->Normal + Vert->NormalIndex ;

			*( ( VECTOR * )&SKFBPos->Position ) = Pos->Position ;
			SKFBPos->Position.w = 1.0f ;
			*( ( VECTOR * )&SKFBPos->Normal ) = Nrm->Normal ;
			SKFBPos->Normal.w = 0.0f ;

			VBlend = Pos->BoneWeight ;
			for( j = 0 ; j < List->MaxBoneNum && VBlend->Index != -1 ; j ++, VBlend ++ )
			{
				SKFBPos->MatrixWeight[ j ].Index = VBlend->Index ;
				SKFBPos->MatrixWeight[ j ].W     = VBlend->W ;
			}
			for( ; j < List->MaxBoneNum ; j ++ )
			{
				SKFBPos->MatrixWeight[ j ].Index = -1 ;
				SKFBPos->MatrixWeight[ j ].W = 0.0f ;
			}
		}
		break ;
	}

	// �I��
	return 0 ;
}

// �V�F�C�v�p�g���C�A���O�����X�g�̍��W�Ɩ@�����̃Z�b�g�A�b�v���s��
extern int MV1SetupShapeTriangleListPositionAndNormal( MV1_TRIANGLE_LIST *TList )
{
	int i, j, k ;
	MV1_TLIST_NORMAL_POS *NrmPos ;
	MV1_TLIST_SKIN_POS_4B *SK4BPos ;
	MV1_TLIST_SKIN_POS_8B *SK8BPos ;
	MV1_TLIST_SKIN_POS_FREEB *SKFBPos ;
	DWORD *MInd ;
	MV1_MESH_VERTEX *Vert ;
	MV1_MESH_POSITION *Pos ;
	MV1_MESH_NORMAL *Nrm ;
	MV1_SKINBONE_BLEND *VBlend ;
	MV1_MESH_BASE *MBMesh ;
	MV1_FRAME_BASE *MBFrame ;
	MV1_TRIANGLE_LIST_BASE *MBTList ;

	MBTList = TList->BaseData ;
	MBMesh = MBTList->Container ;
	MBFrame = MBMesh->Container ;

	MInd = MBTList->MeshVertexIndex ;
	switch( MBTList->VertexType )
	{
	case MV1_VERTEX_TYPE_NORMAL :
		NrmPos = ( MV1_TLIST_NORMAL_POS * )ADDR16( TList->NormalPosition ) ;
		for( i = 0 ; i < MBTList->VertexNum ; i ++, NrmPos ++, MInd ++ )
		{
			Vert = ( MV1_MESH_VERTEX * )( ( BYTE * )MBMesh->Vertex + *MInd * MBMesh->VertUnitSize ) ;
			Pos = ( MV1_MESH_POSITION * )( ( BYTE * )MBFrame->Position + Vert->PositionIndex * MBFrame->PosUnitSize ) ;
			Nrm = MBFrame->Normal + Vert->NormalIndex ;

			*( ( VECTOR * )&NrmPos->Position ) = Pos->Position ;
			NrmPos->Position.w = 1.0f ;
			*( ( VECTOR * )&NrmPos->Normal ) = Nrm->Normal ;
			NrmPos->Normal.w = 0.0f ;
		}
		break ;

	case MV1_VERTEX_TYPE_SKIN_4BONE :
		SK4BPos = ( MV1_TLIST_SKIN_POS_4B * )ADDR16( TList->SkinPosition4B ) ;
		for( i = 0 ; i < MBTList->VertexNum ; i ++, SK4BPos ++, MInd ++ )
		{
			Vert = ( MV1_MESH_VERTEX * )( ( BYTE * )MBMesh->Vertex + *MInd * MBMesh->VertUnitSize ) ;
			Pos = ( MV1_MESH_POSITION * )( ( BYTE * )MBFrame->Position + Vert->PositionIndex * MBFrame->PosUnitSize ) ;
			Nrm = MBFrame->Normal + Vert->NormalIndex ;

			*( ( VECTOR * )&SK4BPos->Position ) = Pos->Position ;
			SK4BPos->Position.w = 1.0f ;
			SK4BPos->Normal = Nrm->Normal ;

			VBlend = Pos->BoneWeight ;
			for( j = 0 ; j < MBFrame->UseSkinBoneNum && j < 4 && VBlend->Index != -1 ; j ++, VBlend ++ )
			{
				SK4BPos->MatrixWeight[ j ] = VBlend->W ;
				for( k = 0 ; k < MBTList->UseBoneNum && MBTList->UseBone[ k ] != VBlend->Index ; k ++ ){}
				SK4BPos->MatrixIndex[ j ] = ( BYTE )k ;
			}
			for( ; j < 4 ; j ++ )
			{
				SK4BPos->MatrixWeight[ j ] = 0.0f ;
				SK4BPos->MatrixIndex[ j ] = 0 ;
			}
		}
		break ;

	case MV1_VERTEX_TYPE_SKIN_8BONE :
		SK8BPos = ( MV1_TLIST_SKIN_POS_8B * )ADDR16( TList->SkinPosition8B ) ;
		for( i = 0 ; i < MBTList->VertexNum ; i ++, SK8BPos ++, MInd ++ )
		{
			Vert = ( MV1_MESH_VERTEX * )( ( BYTE * )MBMesh->Vertex + *MInd * MBMesh->VertUnitSize ) ;
			Pos = ( MV1_MESH_POSITION * )( ( BYTE * )MBFrame->Position + Vert->PositionIndex * MBFrame->PosUnitSize ) ;
			Nrm = MBFrame->Normal + Vert->NormalIndex ;

			SK8BPos->Position = Pos->Position ;
			SK8BPos->Normal = Nrm->Normal ;

			VBlend = Pos->BoneWeight ;
			for( j = 0 ; j < MBFrame->UseSkinBoneNum && j < 8 && VBlend->Index != -1 ; j ++, VBlend ++ )
			{
				SK8BPos->MatrixWeight[ j ] = VBlend->W ;

				for( k = 0 ; k < MBTList->UseBoneNum && MBTList->UseBone[ k ] != VBlend->Index ; k ++ ){}
				if( j < 4 )
				{
					SK8BPos->MatrixIndex1[ j ] = ( BYTE )k ;
				}
				else
				{
					SK8BPos->MatrixIndex2[ j - 4 ] = ( BYTE )k ;
				}
			}
			for( ; j < 8 ; j ++ )
			{
				SK8BPos->MatrixWeight[ j ] = 0.0f ;
				if( j < 4 )
				{
					SK8BPos->MatrixIndex1[ j ] = 0 ;
				}
				else
				{
					SK8BPos->MatrixIndex2[ j - 4 ] = 0 ;
				}
			}
		}
		break ;

	case MV1_VERTEX_TYPE_SKIN_FREEBONE :
		SKFBPos =( MV1_TLIST_SKIN_POS_FREEB * )ADDR16( TList->SkinPositionFREEB ) ;
		for( i = 0 ; i < MBTList->VertexNum ; i ++, SKFBPos = ( MV1_TLIST_SKIN_POS_FREEB * )( ( BYTE * )SKFBPos + MBTList->PosUnitSize ), MInd ++ )
		{
			Vert = ( MV1_MESH_VERTEX * )( ( BYTE * )MBMesh->Vertex + *MInd * MBMesh->VertUnitSize ) ;
			Pos = ( MV1_MESH_POSITION * )( ( BYTE * )MBFrame->Position + Vert->PositionIndex * MBFrame->PosUnitSize ) ;
			Nrm = MBFrame->Normal + Vert->NormalIndex ;

			*( ( VECTOR * )&SKFBPos->Position ) = Pos->Position ;
			SKFBPos->Position.w = 1.0f ;
			*( ( VECTOR * )&SKFBPos->Normal ) = Nrm->Normal ;
			SKFBPos->Normal.w = 0.0f ;

			VBlend = Pos->BoneWeight ;
			for( j = 0 ; j < MBTList->MaxBoneNum && VBlend->Index != -1 ; j ++, VBlend ++ )
			{
				SKFBPos->MatrixWeight[ j ].Index = VBlend->Index ;
				SKFBPos->MatrixWeight[ j ].W     = VBlend->W ;
			}
			for( ; j < MBTList->MaxBoneNum ; j ++ )
			{
				SKFBPos->MatrixWeight[ j ].Index = -1 ;
				SKFBPos->MatrixWeight[ j ].W = 0.0f ;
			}
		}
		break ;
	}

	// �I��
	return 0 ;
}


















// ���ʃf�[�^�n

// �O���f�[�V�����摜���쐬����
extern int MV1CreateGradationGraph( void )
{
	DWORD i ;
	BASEIMAGE BaseImage ;
	DWORD *Dest, DestData ;
	int Handle ;
	LOADGRAPH_GPARAM GParam ;

	if( NS_CreateXRGB8ColorBaseImage( 256, 8, &BaseImage ) < 0 )
		return -1 ;

	Dest = ( DWORD * )BaseImage.GraphData ;
	for( i = 0 ; i < 256 ; i ++, Dest ++ )
	{
		DestData = i | ( i << 8 ) | ( i << 16 ) ;
		Dest[ 256 * 0 ] = DestData ;
		Dest[ 256 * 1 ] = DestData ;
		Dest[ 256 * 2 ] = DestData ;
		Dest[ 256 * 3 ] = DestData ;
		Dest[ 256 * 4 ] = DestData ;
		Dest[ 256 * 5 ] = DestData ;
		Dest[ 256 * 6 ] = DestData ;
		Dest[ 256 * 7 ] = DestData ;
	}

	Graphics_Image_InitLoadGraphGParam( &GParam ) ;
	GParam.CreateGraphGParam.NotUseTransColor = TRUE ;
	Graphics_Image_InitSetupGraphHandleGParam_Normal_NonDrawValid( &GParam.CreateGraphGParam.InitHandleGParam, 32, TRUE, FALSE ) ;
	Handle = Graphics_Image_CreateGraphFromGraphImage_UseGParam( &GParam, FALSE, -1, &BaseImage, NULL, TRUE, FALSE, FALSE, FALSE ) ;

	NS_ReleaseBaseImage( &BaseImage ) ;

	return Handle ;
}

// �f�t�H���g�g�D�[���e�N�X�`�����擾����
extern int MV1GetDefaultToonTexture( int Type )
{
	switch( Type )
	{
	case -1 :
		if( MV1Man.ToonDefaultGradTexHandle[ 0 ] < 0 )
		{
			LOADGRAPH_GPARAM GParam ;

			Graphics_Image_InitLoadGraphGParam( &GParam ) ;
			GParam.CreateGraphGParam.NotUseTransColor = TRUE ;
			Graphics_Image_InitSetupGraphHandleGParam_Normal_NonDrawValid( &GParam.CreateGraphGParam.InitHandleGParam, 32, TRUE, FALSE ) ;
			MV1Man.ToonDefaultGradTexHandle[ 0 ] = Graphics_Image_CreateGraphFromMem_UseGParam( &GParam, FALSE, -1, TgaDiffuseDefaultGradFileImage, sizeof( TgaDiffuseDefaultGradFileImage ), NULL, 0, TRUE, FALSE, FALSE ) ;
			NS_SetDeleteHandleFlag( MV1Man.ToonDefaultGradTexHandle[ 0 ], &MV1Man.ToonDefaultGradTexHandle[ 0 ] ) ;
			NS_SetDeviceLostDeleteGraphFlag( MV1Man.ToonDefaultGradTexHandle[ 0 ], TRUE ) ;
		}
		return MV1Man.ToonDefaultGradTexHandle[ 0 ] ;

	case -2 :
		if( MV1Man.ToonDefaultGradTexHandle[ 1 ] < 0 )
		{
			MV1Man.ToonDefaultGradTexHandle[ 1 ] = MV1CreateGradationGraph() ;
			NS_SetDeleteHandleFlag( MV1Man.ToonDefaultGradTexHandle[ 1 ], &MV1Man.ToonDefaultGradTexHandle[ 1 ] ) ;
			NS_SetDeviceLostDeleteGraphFlag( MV1Man.ToonDefaultGradTexHandle[ 1 ], TRUE ) ;
		}
		return MV1Man.ToonDefaultGradTexHandle[ 1 ] ;
	}

	return -1 ;
}

// TexNoneHandle �̃Z�b�g�A�b�v���s��
extern void MV1SetupTexNoneHandle( void )
{
	LOADGRAPH_GPARAM GParam ;
	Graphics_Image_InitLoadGraphGParam( &GParam ) ;
	GParam.CreateGraphGParam.NotUseTransColor            = FALSE ;
	GParam.CreateGraphGParam.LeftUpColorIsTransColorFlag = FALSE ;
	Graphics_Image_InitSetupGraphHandleGParam_Normal_NonDrawValid( &GParam.CreateGraphGParam.InitHandleGParam, 32, TRUE, FALSE ) ;
	GParam.CreateGraphGParam.InitHandleGParam.TransColor = ( BYTE )255 << 16 | ( BYTE )0 << 8 | ( BYTE )255 ;
	MV1Man.TexNoneHandle      = Graphics_Image_CreateGraphFromMem_UseGParam( &GParam, FALSE, -1, Tga8x8TextureFileImage,      sizeof( Tga8x8TextureFileImage      ), NULL, 0, TRUE, FALSE, FALSE ) ;
	MV1Man.TexNoneBlackHandle = Graphics_Image_CreateGraphFromMem_UseGParam( &GParam, FALSE, -1, Tga8x8BlackTextureFileImage, sizeof( Tga8x8BlackTextureFileImage ), NULL, 0, TRUE, FALSE, FALSE ) ;
	NS_SetDeleteHandleFlag( MV1Man.TexNoneHandle,      &MV1Man.TexNoneHandle      ) ;
	NS_SetDeleteHandleFlag( MV1Man.TexNoneBlackHandle, &MV1Man.TexNoneBlackHandle ) ;
	NS_SetDeviceLostDeleteGraphFlag( MV1Man.TexNoneHandle,      TRUE ) ;
	NS_SetDeviceLostDeleteGraphFlag( MV1Man.TexNoneBlackHandle, TRUE ) ;
}



















// ���f���@�\�̏�����
extern int MV1Initialize()
{
	// ���ɏ���������Ă����牽�������ɏI��
	if( MV1Man.Initialize ) return 0 ;

	// �n�[�h�E�G�A�̋@�\���g�p�ł��Ȃ��ꍇ�͉��������I��
	if( GSYS.Setting.ValidHardware == FALSE ) return 0 ;

	// �g���C�A���O�����X�g��f�[�^�̊��ˑ��p�o�b�t�@�̃T�C�Y�`�F�b�N
	if( MV1_TRIANGLE_LIST_BASE_PF_BUFFER_SIZE < sizeof( MV1_TRIANGLE_LIST_BASE_PF ) )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\xc8\x30\xe9\x30\xa4\x30\xa2\x30\xf3\x30\xb0\x30\xeb\x30\xea\x30\xb9\x30\xc8\x30\xfa\x57\xc7\x30\xfc\x30\xbf\x30\x6e\x30\xb0\x74\x83\x58\x9d\x4f\x58\x5b\x28\x75\xd0\x30\xc3\x30\xd5\x30\xa1\x30\x6e\x30\xb5\x30\xa4\x30\xba\x30\x4c\x30\xb3\x8d\x8a\x30\x7e\x30\x5b\x30\x93\x30\x20\x00\x4d\x00\x56\x00\x31\x00\x5f\x00\x54\x00\x52\x00\x49\x00\x41\x00\x4e\x00\x47\x00\x4c\x00\x45\x00\x5f\x00\x4c\x00\x49\x00\x53\x00\x54\x00\x5f\x00\x42\x00\x41\x00\x53\x00\x45\x00\x5f\x00\x50\x00\x46\x00\x5f\x00\x42\x00\x55\x00\x46\x00\x46\x00\x45\x00\x52\x00\x5f\x00\x53\x00\x49\x00\x5a\x00\x45\x00\x3a\x00\x25\x00\x64\x00\x62\x00\x79\x00\x74\x00\x65\x00\x20\x00\x20\x00\x73\x00\x69\x00\x7a\x00\x65\x00\x6f\x00\x66\x00\x28\x00\x20\x00\x4d\x00\x56\x00\x31\x00\x5f\x00\x54\x00\x52\x00\x49\x00\x41\x00\x4e\x00\x47\x00\x4c\x00\x45\x00\x5f\x00\x4c\x00\x49\x00\x53\x00\x54\x00\x5f\x00\x42\x00\x41\x00\x53\x00\x45\x00\x5f\x00\x50\x00\x46\x00\x20\x00\x29\x00\x3a\x00\x25\x00\x64\x00\x62\x00\x79\x00\x74\x00\x65\x00\x00"/*@ L"�g���C�A���O�����X�g��f�[�^�̊��ˑ��p�o�b�t�@�̃T�C�Y������܂��� MV1_TRIANGLE_LIST_BASE_PF_BUFFER_SIZE:%dbyte  sizeof( MV1_TRIANGLE_LIST_BASE_PF ):%dbyte" @*/,
			MV1_TRIANGLE_LIST_BASE_PF_BUFFER_SIZE, sizeof( MV1_TRIANGLE_LIST_BASE_PF ) ));
		return -1 ;
	}

	// ���f����{�f�[�^�n���h���̏�����
	InitializeHandleManage( DX_HANDLETYPE_MODEL_BASE, sizeof( MV1_MODEL_BASE ), MAX_MODEL_BASE_NUM, InitializeModelBaseHandle, TerminateModelBaseHandle, L"ModelBase" ) ;

	// ���f���f�[�^�n���h���̏�����
	InitializeHandleManage( DX_HANDLETYPE_MODEL, sizeof( MV1_MODEL ), MAX_MODEL_NUM, InitializeModelHandle, TerminateModelHandle, L"Model" ) ;

	// �e��ϐ���������
	MV1Man.ModelBaseNum = 0 ;
	MV1Man.ModelBaseMaxNum = 0 ;
	MV1Man.ModelBaseNextIndex = 0 ;
	MV1Man.ModelBase = 0 ;

	MV1Man.ModelNum = 0 ;
	MV1Man.ModelMaxNum = 0 ;
	MV1Man.ModelNextIndex = 0 ;
	MV1Man.ModelNextCheckNumber = 0 ;
	MV1Man.Model = 0 ;

//	MV1Man.TextureNum = 0 ;
//	MV1Man.TextureMaxNum = 0 ;
//	MV1Man.TextureNextIndex = 0 ;
//	MV1Man.Texture = 0 ;

	MV1Man.LoadModelToReMakeNormalSmoothingAngle = 89.5f * ( float )DX_PI / 180.0f ;

	// �f�t�H���g�e�N�X�`���̃n���h���l�𖳌���
	MV1Man.TexNoneHandle = -1 ;
	MV1Man.ToonDefaultGradTexHandle[ 0 ] = -1 ;
	MV1Man.ToonDefaultGradTexHandle[ 1 ] = -1 ;

	// �������t���O�𗧂Ă�
	MV1Man.Initialize = true ;

	// �I��
	return 0 ;
}

// ���f���@�\�̌�n��
extern int MV1Terminate()
{
	// ����������Ă��Ȃ������牽�������ɏI��
	if( MV1Man.Initialize == false ) return 0 ;

	// �L���Ȃ��ׂẴ��f�����������
	NS_MV1InitModel() ;

	// �L���Ȃ��ׂẴ��f����f�[�^���������
	MV1InitModelBase() ;

	// �`��p�Ɋm�ۂ��ꂽ�������̈�̊m��
	if( MV1Man.DrawMeshList )
	{
		DXFREE( MV1Man.DrawMeshList ) ;
		MV1Man.DrawMeshList = NULL ;
		MV1Man.DrawMeshListSize = 0 ;
	}

	// ���ˑ��̌�n�����������s
	MV1_Terminate_PF() ;

	// ���f���f�[�^�n���h���Ǘ����̌�n��
	TerminateHandleManage( DX_HANDLETYPE_MODEL ) ;

	// ���f����{�f�[�^�n���h���Ǘ����̌�n��
	TerminateHandleManage( DX_HANDLETYPE_MODEL_BASE ) ;

	// �������t���O��|��
	MV1Man.Initialize = false ;

	// �I��
	return 0 ;
}


// ���f���p�e�N�X�`���̃J���[�C���[�W���쐬����
extern int MV1CreateTextureColorBaseImage(
				BASEIMAGE *DestColorBaseImage,
				BASEIMAGE *DestAlphaBaseImage,
				void *ColorFileImage, int ColorFileSize,
				void *AlphaFileImage, int AlphaFileSize,
				int BumpImageFlag, float BumpImageNextPixelLength,
				int ReverseFlag )
{
	BASEIMAGE ScaleBaseImage, ScaleAlphaBaseImage ;
	int SizeX, SizeY ;

	// �t�@�C�����ǂݍ��߂Ȃ�������G���[
	if( ColorFileImage )
	{
		if( NS_CreateBaseImage( NULL, ColorFileImage, ColorFileSize, LOADIMAGE_TYPE_MEM, DestColorBaseImage, FALSE ) < 0 )
		{
			DXST_ERRORLOGFMT_ADDW(( L"MV1CreateTextureColorBaseImage : Error 1\n" )) ;
			return -1 ;
		}
	}

	if( AlphaFileImage )
	{
		if( NS_CreateBaseImage( NULL, AlphaFileImage, AlphaFileSize, LOADIMAGE_TYPE_MEM, DestAlphaBaseImage, FALSE ) < 0 )
		{
			if( ColorFileImage ) NS_ReleaseBaseImage( DestColorBaseImage ) ;
			DXST_ERRORLOGFMT_ADDW(( L"MV1CreateTextureColorBaseImage : Error 2\n" )) ;
			return -1 ;
		}
	}

	// �J���[�e�N�X�`���������ꍇ�͑�։摜���[�Ă�
	if( ColorFileImage == NULL )
	{
		if( NS_CreateXRGB8ColorBaseImage( DestAlphaBaseImage->Width, DestAlphaBaseImage->Height, DestColorBaseImage ) < 0 )
		{
			if( AlphaFileImage ) NS_ReleaseBaseImage( DestAlphaBaseImage ) ;
			DXST_ERRORLOGFMT_ADDW(( L"MV1CreateTextureColorBaseImage : Error 3\n" )) ;
			return -1 ;
		}
		_MEMSET( DestColorBaseImage->GraphData, 0xff, ( size_t )( DestColorBaseImage->Pitch * DestColorBaseImage->Height ) ) ;
	}

	// �o���v�}�b�v�w��̏ꍇ�͖@���}�b�v������
	if( BumpImageFlag )
	{
		int c00, c01, c10, i, j ;
		VECTOR v1, v2, v ;
		BASEIMAGE NormalBaseImage ;

		if( BumpImageNextPixelLength < 0.000001f )
			BumpImageNextPixelLength = 0.000001f ;

		if( NS_CreateXRGB8ColorBaseImage( DestColorBaseImage->Width, DestColorBaseImage->Height, &NormalBaseImage ) < 0 )
		{
			NS_ReleaseBaseImage( DestColorBaseImage ) ;
			if( AlphaFileImage ) NS_ReleaseBaseImage( DestAlphaBaseImage ) ;
			DXST_ERRORLOGFMT_ADDW(( L"MV1CreateTextureColorBaseImage : Error 4\n" )) ;
			return -1 ;
		}

		for( i = 0 ; i < DestColorBaseImage->Height ; i ++ )
		{
			for( j = 0 ; j < DestColorBaseImage->Width ; j ++ )
			{
				NS_GetPixelBaseImage( DestColorBaseImage, j, i, &c00, NULL, NULL, NULL ) ;
				NS_GetPixelBaseImage( DestColorBaseImage, j + 1 == DestColorBaseImage->Width ? 0 : j + 1, i,  &c01, NULL, NULL, NULL ) ;
				NS_GetPixelBaseImage( DestColorBaseImage, j, i + 1 == DestColorBaseImage->Height ? 0 : i + 1, &c10, NULL, NULL, NULL ) ;

				v1 = VGet( BumpImageNextPixelLength, 0.0f, ( c01 - c00 ) / 255.0f ) ;
				v2 = VGet( 0.0f, BumpImageNextPixelLength, ( c10 - c00 ) / 255.0f ) ;
				v = VNorm( VCross( v1, v2 ) ) ;
				NS_SetPixelBaseImage( &NormalBaseImage, j, i, _FTOL( ( v.x + 1.0f ) * 127.5f ), _FTOL( ( v.y + 1.0f ) * 127.5f ), _FTOL( ( v.z + 1.0f ) * 127.5f ), 0 ) ;
			}
		}
//		NS_SaveBaseImageToBmp( _T( "TestNormalMap.bmp" ), &NormalBaseImage ) ; 

		// �J���[�摜�����
		NS_ReleaseBaseImage( DestColorBaseImage ) ;

		// �@���}�b�v�摜���J���[�摜�Ƃ���
		*DestColorBaseImage = NormalBaseImage ;
	}

	// �J���[�摜�̃T�C�Y���Q�̂���ł͂Ȃ�������X�P�[�����O����
	if( ColorFileImage != NULL )
	{
		for( SizeX = 1 ; SizeX < DestColorBaseImage->Width  ; SizeX <<= 1 ){}
		for( SizeY = 1 ; SizeY < DestColorBaseImage->Height ; SizeY <<= 1 ){}
		if( SizeX != DestColorBaseImage->Width || SizeY != DestColorBaseImage->Height )
		{
			// �n�[�h�E�G�A���Ή����Ă���ő�e�N�X�`���T�C�Y / 2�͒����Ȃ��悤�ɂ���
			if( GSYS.HardInfo.MaxTextureSize / 2 < SizeX ) SizeX = GSYS.HardInfo.MaxTextureSize / 2 ;
			if( GSYS.HardInfo.MaxTextureSize / 2 < SizeY ) SizeY = GSYS.HardInfo.MaxTextureSize / 2 ;

			if( NS_CreateColorDataBaseImage( SizeX, SizeY, &DestColorBaseImage->ColorData, &ScaleBaseImage ) < 0 )
			{
				NS_ReleaseBaseImage( DestColorBaseImage ) ;
				if( AlphaFileImage ) NS_ReleaseBaseImage( DestAlphaBaseImage ) ;
				DXST_ERRORLOGFMT_ADDW(( L"MV1CreateTextureColorBaseImage : Error 5\n" )) ;
				return -1 ;
			}
			
			ScalingBltBaseImage(
				0, 0, DestColorBaseImage->Width, DestColorBaseImage->Height, DestColorBaseImage,
				0, 0, SizeX, SizeY, &ScaleBaseImage, TRUE ) ;

			NS_ReleaseBaseImage( DestColorBaseImage ) ;
			*DestColorBaseImage = ScaleBaseImage ;
		}

		// ���]�t���O�������Ă����甽�]����
		if( ReverseFlag )
		{
			NS_ReverseBaseImage( DestColorBaseImage ) ;
		}
	}

	// ���摜�̃T�C�Y���Q�̂���ł͂Ȃ�������X�P�[�����O����
	if( AlphaFileImage != NULL )
	{
		for( SizeX = 1 ; SizeX < DestAlphaBaseImage->Width  ; SizeX <<= 1 ){}
		for( SizeY = 1 ; SizeY < DestAlphaBaseImage->Height ; SizeY <<= 1 ){}
		if( SizeX != DestAlphaBaseImage->Width || SizeY != DestAlphaBaseImage->Height )
		{
			// �n�[�h�E�G�A���Ή����Ă���ő�e�N�X�`���T�C�Y�͒����Ȃ��悤�ɂ���
			if( GSYS.HardInfo.MaxTextureSize / 2 < SizeX ) SizeX = GSYS.HardInfo.MaxTextureSize / 2 ;
			if( GSYS.HardInfo.MaxTextureSize / 2 < SizeY ) SizeY = GSYS.HardInfo.MaxTextureSize / 2 ;

			if( NS_CreateColorDataBaseImage( SizeX, SizeY, &DestAlphaBaseImage->ColorData, &ScaleAlphaBaseImage ) < 0 )
			{
				NS_ReleaseBaseImage( DestColorBaseImage ) ;
				NS_ReleaseBaseImage( DestAlphaBaseImage ) ;
				DXST_ERRORLOGFMT_ADDW(( L"MV1CreateTextureColorBaseImage : Error 6\n" )) ;
				return -1 ;
			}
			
			ScalingBltBaseImage(
				0, 0, DestAlphaBaseImage->Width, DestAlphaBaseImage->Height, DestAlphaBaseImage,
				0, 0, SizeX, SizeY, &ScaleAlphaBaseImage, TRUE ) ;

			NS_ReleaseBaseImage( DestAlphaBaseImage ) ;
			*DestAlphaBaseImage = ScaleAlphaBaseImage ;
		}

		// ���]�t���O�������Ă����甽�]����
		if( ReverseFlag )
		{
			NS_ReverseBaseImage( DestAlphaBaseImage ) ;
		}
	}

	// ��̉摜�̃T�C�Y���قȂ�ꍇ�͑傫�����ɍ��킹��
	if( AlphaFileImage != NULL && ColorFileImage != NULL )
	{
		if( DestColorBaseImage->Width > DestAlphaBaseImage->Width ||
			DestColorBaseImage->Height > DestAlphaBaseImage->Height )
		{
			if( NS_CreateColorDataBaseImage( DestColorBaseImage->Width, DestColorBaseImage->Height, &DestAlphaBaseImage->ColorData, &ScaleAlphaBaseImage ) < 0 )
			{
				NS_ReleaseBaseImage( DestColorBaseImage ) ;
				NS_ReleaseBaseImage( DestAlphaBaseImage ) ;
				DXST_ERRORLOGFMT_ADDW(( L"MV1CreateTextureColorBaseImage : Error 7\n" )) ;
				return -1 ;
			}
			
			ScalingBltBaseImage(
				0, 0, DestAlphaBaseImage->Width, DestAlphaBaseImage->Height, DestAlphaBaseImage,
				0, 0, DestColorBaseImage->Width, DestColorBaseImage->Height, &ScaleAlphaBaseImage, TRUE ) ;

			NS_ReleaseBaseImage( DestAlphaBaseImage ) ;
			*DestAlphaBaseImage = ScaleAlphaBaseImage ;
		}
		else
		{
			if( NS_CreateColorDataBaseImage( DestAlphaBaseImage->Width, DestAlphaBaseImage->Height, &DestColorBaseImage->ColorData, &ScaleBaseImage ) < 0 )
			{
				NS_ReleaseBaseImage( DestColorBaseImage ) ;
				NS_ReleaseBaseImage( DestAlphaBaseImage ) ;
				DXST_ERRORLOGFMT_ADDW(( L"MV1CreateTextureColorBaseImage : Error 8\n" )) ;
				return -1 ;
			}
			
			ScalingBltBaseImage(
				0, 0, DestColorBaseImage->Width, DestColorBaseImage->Height, DestColorBaseImage,
				0, 0, DestAlphaBaseImage->Width, DestAlphaBaseImage->Height, &ScaleBaseImage, TRUE ) ;

			NS_ReleaseBaseImage( DestColorBaseImage ) ;
			*DestColorBaseImage = ScaleBaseImage ;
		}
	}

	// �I��
	return 0 ;
}

// �e�N�X�`���̍ēǂݍ���
extern int MV1ReloadTexture( void )
{
	int i, j ;
	MV1_TEXTURE_BASE *TexBase ;
	MV1_TEXTURE *Tex ;
	MV1_MODEL_BASE *ModelBase ;
	MV1_MODEL *Model ;
	BASEIMAGE ColorBaseImage, AlphaBaseImage ;
	LOADGRAPH_GPARAM GParam ;

	if( HandleManageArray[ DX_HANDLETYPE_MODEL_BASE ].InitializeFlag == FALSE )
		return -1 ;

	Graphics_Image_InitLoadGraphGParam( &GParam ) ;
	GParam.CreateGraphGParam.NotUseTransColor = TRUE ;
	Graphics_Image_InitSetupGraphHandleGParam_Normal_NonDrawValid( &GParam.CreateGraphGParam.InitHandleGParam, 32, TRUE, FALSE ) ;
	Graphics_Image_CreateGraphFromMem_UseGParam( &GParam, MV1Man.TexNoneHandle,                 -1, Tga8x8TextureFileImage,         sizeof( Tga8x8TextureFileImage ),         NULL, 0, TRUE, FALSE, FALSE ) ;
	Graphics_Image_CreateGraphFromMem_UseGParam( &GParam, MV1Man.TexNoneBlackHandle,            -1, Tga8x8BlackTextureFileImage,    sizeof( Tga8x8BlackTextureFileImage ),    NULL, 0, TRUE, FALSE, FALSE ) ;
	Graphics_Image_CreateGraphFromMem_UseGParam( &GParam, MV1Man.ToonDefaultGradTexHandle[ 0 ], -1, TgaDiffuseDefaultGradFileImage, sizeof( TgaDiffuseDefaultGradFileImage ), NULL, 0, TRUE, FALSE, FALSE ) ;
//	NS_ReCreateGraphFromMem( &TgaSpecularDefaultGradFileImage, sizeof( TgaSpecularDefaultGradFileImage ), MV1Man.ToonDefaultGradTexHandle[ 1 ] ) ;
	_MV1ReCreateGradationGraph( MV1Man.ToonDefaultGradTexHandle[ 1 ] ) ;

	for( i = HandleManageArray[ DX_HANDLETYPE_MODEL_BASE ].AreaMin ; i <= HandleManageArray[ DX_HANDLETYPE_MODEL_BASE ].AreaMax ; i ++ )
	{
		ModelBase = ( MV1_MODEL_BASE * )HandleManageArray[ DX_HANDLETYPE_MODEL_BASE ].Handle[ i ] ;
		if( ModelBase == NULL ) continue ;

		TexBase = ModelBase->Texture ;
		for( j = 0 ; j < ModelBase->TextureNum ; j ++, TexBase ++ )
		{
			if( MV1CreateTextureColorBaseImage(
					&ColorBaseImage,
					&AlphaBaseImage,
					TexBase->ColorImage, TexBase->ColorImageSize,
					TexBase->AlphaImage, TexBase->AlphaImageSize,
					TexBase->BumpImageFlag, TexBase->BumpImageNextPixelLength ) == 0 )
			{
				Graphics_Image_CreateGraphFromGraphImage_UseGParam( &GParam, TRUE, TexBase->GraphHandle, &ColorBaseImage, TexBase->AlphaImage ? &AlphaBaseImage : NULL, TRUE, FALSE, FALSE, FALSE ) ;

				NS_ReleaseBaseImage( &ColorBaseImage ) ;
				if( TexBase->AlphaImage ) NS_ReleaseBaseImage( &AlphaBaseImage ) ;
			}
		}
	}

	for( i = HandleManageArray[ DX_HANDLETYPE_MODEL ].AreaMin ; i <= HandleManageArray[ DX_HANDLETYPE_MODEL ].AreaMax ; i ++ )
	{
		Model = ( MV1_MODEL * )HandleManageArray[ DX_HANDLETYPE_MODEL ].Handle[ i ] ;
		if( Model == NULL ) continue ;
		ModelBase = Model->BaseData ;

		Tex = Model->Texture ;
		for( j = 0 ; j < ModelBase->TextureNum ; j ++, Tex ++ )
		{
			if( Tex->UseGraphHandle == FALSE ) continue ;

			if( MV1CreateTextureColorBaseImage(
					&ColorBaseImage,
					&AlphaBaseImage,
					Tex->ColorImage, Tex->ColorImageSize,
					Tex->AlphaImage, Tex->AlphaImageSize,
					Tex->BumpImageFlag, Tex->BumpImageNextPixelLength ) == 0 )
			{
				Graphics_Image_CreateGraphFromGraphImage_UseGParam( &GParam, TRUE, Tex->GraphHandle, &ColorBaseImage, Tex->AlphaImage ? &AlphaBaseImage : NULL, TRUE, FALSE, FALSE, FALSE ) ;

				NS_ReleaseBaseImage( &ColorBaseImage ) ;
				if( Tex->AlphaImage ) NS_ReleaseBaseImage( &AlphaBaseImage ) ;
			}
		}
	}

	// �I��
	return 0 ;
}


// �ǉ����[�h�֐��֌W

// ���f���ǂݍ��݊֐���ǉ�����
extern int MV1AddLoadFunc( int ( *AddLoadFunc )( const MV1_MODEL_LOAD_PARAM *LoadParam ) )
{
	int i ;

	if( MV1Man.AddLoadFuncNum == MV1_ADD_LOAD_FUNC_MAXNUM ) return -1 ;

	for( i = 0 ; i < MV1Man.AddLoadFuncNum && MV1Man.AddLoadFunc[ i ] != AddLoadFunc ; i ++ ){}
	if( i != MV1Man.AddLoadFuncNum ) return 0 ;

	MV1Man.AddLoadFunc[ MV1Man.AddLoadFuncNum ] = AddLoadFunc ;
	MV1Man.AddLoadFuncNum ++ ;

	// �I��
	return 0 ;
}

// ���f���ǂݍ��݊֐����폜����
extern int MV1SubLoadFunc( int ( *AddLoadFunc )( const MV1_MODEL_LOAD_PARAM *LoadParam ) )
{
	int i ;

	for( i = 0 ; i < MV1Man.AddLoadFuncNum && MV1Man.AddLoadFunc[ i ] != AddLoadFunc ; i ++ ){}
	if( i == MV1Man.AddLoadFuncNum ) return 0 ;

	MV1Man.AddLoadFuncNum -- ;

	if( i != MV1Man.AddLoadFuncNum )
	{
		_MEMMOVE( &MV1Man.AddLoadFunc[ i ], &MV1Man.AddLoadFunc[ i + 1 ], ( MV1Man.AddLoadFuncNum - i ) * sizeof( int ( * )( const MV1_MODEL_LOAD_PARAM * ) ) ) ;
	}

	// �I��
	return 0 ;
}


// �L���ȃ��f����{�f�[�^�����ׂč폜����
extern int MV1InitModelBase( void )
{
	return AllHandleSub( DX_HANDLETYPE_MODEL_BASE ) ;
}

// ���f���f�[�^�n���h���̏�����
extern int InitializeModelBaseHandle( HANDLEINFO * )
{
	// ���ɉ������Ȃ�
	return 0 ;
}

// ���f���f�[�^�n���h���̌�n��
extern int TerminateModelBaseHandle( HANDLEINFO *HandleInfo )
{
	MV1_MODEL_BASE *MBase = ( MV1_MODEL_BASE * )HandleInfo ;
	MV1_TRIANGLE_LIST_BASE *TList ;
	MV1_ANIMSET_BASE *AnimSetBase ;
	MV1_TEXTURE_BASE *Texture ;
	int i ;

	// �Q�ƃJ�E���g���f�N�������g
	MBase->RefCount -- ;

	// �Q�ƃJ�E���g���O�ȏ�̏ꍇ�͂܂�������Ȃ�
	if( MBase->RefCount > 0 )
		return 1 ;

	// ���_�o�b�t�@���J��
	MV1_TerminateVertexBufferBase_PF( HandleInfo->Handle ) ;

	// ���ˑ��̃f�[�^�̌�n��
	MV1_TerminateModelBaseHandle_PF( MBase ) ;

	// ���_�f�[�^�̉��
	if( MBase->VertexData != NULL )
		DXFREE( MBase->VertexData ) ;
	MBase->TriangleListNormalPosition = NULL ;
	MBase->TriangleListSkinPosition4B = NULL ;
	MBase->TriangleListSkinPosition8B = NULL ;
	MBase->TriangleListSkinPositionFREEB = NULL ;
	MBase->MeshVertexIndex = NULL ;
	MBase->MeshPosition = NULL ;
	MBase->MeshNormal = NULL ;
	MBase->MeshFace = NULL ;
	MBase->MeshVertex = NULL ;

	// �e�N�X�`�����������
	Texture = MBase->Texture ;
	for( i = 0 ; i < MBase->TextureNum ; i ++, Texture ++ )
	{
		// ���O�̉��
		if( Texture->NameAllocMem )
		{
#ifndef UNICODE
			if( Texture->NameA )
			{
				DXFREE( Texture->NameA ) ;
				Texture->NameA = NULL ;
			}
#endif

			if( Texture->NameW )
			{
				DXFREE( Texture->NameW ) ;
				Texture->NameW = NULL ;
			}
		}

		// �摜�n���h�����폜
		NS_DeleteGraph( Texture->GraphHandle ) ;
		Texture->GraphHandle = 0 ;

		// �t�@�C���p�X�̉��
		if( Texture->ColorImageFilePathAllocMem )
		{
#ifndef UNICODE
			if( Texture->ColorFilePathA )
			{
				DXFREE( Texture->ColorFilePathA ) ;
			}
			Texture->ColorFilePathA = NULL ;
#endif
			if( Texture->ColorFilePathW )
			{
				DXFREE( Texture->ColorFilePathW ) ;
			}
			Texture->ColorFilePathW = NULL ;
		}

		if( Texture->AlphaImageFilePathAllocMem )
		{
#ifndef UNICODE
			if( Texture->AlphaFilePathA )
			{
				DXFREE( Texture->AlphaFilePathA ) ;
			}
			Texture->AlphaFilePathA = NULL ;
#endif
			if( Texture->AlphaFilePathW )
			{
				DXFREE( Texture->AlphaFilePathW ) ;
			}
			Texture->AlphaFilePathW = NULL ;
		}

		// �t�@�C���C���[�W�̉��
		if( Texture->ColorImage )
		{
			DXFREE( Texture->ColorImage ) ;
			Texture->ColorImage = NULL ;
		}

		if( Texture->AlphaImage )
		{
			DXFREE( Texture->AlphaImage ) ;
			Texture->AlphaImage = NULL ;
		}
	}
	if( MBase->TextureAllocMem )
	{
		DXFREE( MBase->Texture ) ;
		MBase->Texture = NULL ;
	}

	// �ʌɃV�F�C�v���_�f�[�^�p�̃��������m�ۂ��Ă����ꍇ�͉������
	for( i = 0 ; i < MBase->ShapeMeshNum ; i ++ )
	{
		if( MBase->ShapeMesh[ i ].VertexAllocMem )
		{
			DXFREE( MBase->ShapeMesh[ i ].Vertex ) ;
			MBase->ShapeMesh[ i ].Vertex = NULL ;
		}
	}

	// �ʌɖ@���f�[�^�p�̃��������m�ۂ��Ă����ꍇ�͉������
	for( i = 0 ; i < MBase->FrameNum ; i ++ )
	{
		if( MBase->Frame[ i ].NormalAllocMem )
		{
			DXFREE( MBase->Frame[ i ].Normal ) ;
			MBase->Frame[ i ].Normal = NULL ;
		}
	}

	// �ʌɒ��_�f�[�^�p�̃��������m�ۂ��Ă����ꍇ�͉������
	for( i = 0 ; i < MBase->MeshNum ; i ++ )
	{
		if( MBase->Mesh[ i ].VertexAllocMem )
		{
			DXFREE( MBase->Mesh[ i ].Vertex ) ;
			MBase->Mesh[ i ].Vertex = NULL ;
		}
	}

	// �ʌɊm�ۂ��Ă������������������ꍇ�͉������
	TList = MBase->TriangleList ;
	for( i = 0 ; i < MBase->TriangleListNum ; i ++, TList ++ )
	{
		MV1_TRIANGLE_LIST_BASE_PF *TListPF ;

		TListPF = ( MV1_TRIANGLE_LIST_BASE_PF * )TList->PFBuffer ;

		if( TList->MeshVertexIndexAllocMem )
		{
			DXFREE( TList->MeshVertexIndex ) ;
			TList->MeshVertexIndex = NULL ;
		}
		if( TList->PositionAllocMem )
		{
			DXFREE( TList->NormalPosition ) ;
			TList->NormalPosition = NULL ;
		}

		MV1_TerminateTriangleListBaseTempBuffer_PF( TList ) ;
	}

	// �A�j���[�V���������ォ��ύX�����ꍇ�͊J������
	AnimSetBase = MBase->AnimSet ;
	for( i = 0 ; i < MBase->AnimSetNum ; i ++, AnimSetBase ++ )
	{
		if( AnimSetBase->NameAllocMem )
		{
#ifndef UNICODE
			if( AnimSetBase->NameA )
			{
				DXFREE( AnimSetBase->NameA ) ;
				AnimSetBase->NameA = NULL ;
			}
#endif
			if( AnimSetBase->NameW )
			{
				DXFREE( AnimSetBase->NameW ) ;
				AnimSetBase->NameW = NULL ;
			}
		}
	}


	// �ǉ��m�ۃ������̉��
	ClearMemArea( &MBase->AddFirstMem ) ;

	// �f�[�^���i�[���Ă����������̉��
	if( MBase->DataBuffer )
	{
		DXFREE( MBase->DataBuffer ) ;
		MBase->DataBuffer = NULL ;
	}

	// �I��
	return 0 ;
}

// ���f����{�f�[�^��ǉ�����( -1:�G���[  0�ȏ�:���f����{�f�[�^�n���h�� )
int MV1AddModelBase( int ASyncThread )
{
	return AddHandle( DX_HANDLETYPE_MODEL_BASE, ASyncThread, -1 ) ;
}

// ���f����{�f�[�^���폜����
extern int MV1SubModelBase( int MBHandle )
{
	return SubHandle( MBHandle ) ;
}

// ���f����{�f�[�^�𕡐�����
extern int MV1CreateCloneModelBase( int SrcMBHandle )
{
	MV1_MODEL_BASE MTBase, *MBase = NULL, *FHeader ;
	MV1_FRAME_BASE *Frame ;
	MV1_FRAME_BASE *F1Frame ;
	MV1_MATERIAL_BASE			*Material ;
	MV1_MATERIAL_BASE			*F1Material ;
	MV1_MATERIAL_LAYER			*MaterialLayer ;
	MV1_MATERIAL_LAYER			*F1MaterialLayer ;
	MV1_LIGHT					*Light ;
	MV1_LIGHT					*F1Light ;
	MV1_TEXTURE_BASE			*Texture ;
	MV1_TEXTURE_BASE			*F1Texture ;
	MV1_MESH_BASE				*Mesh ;
	MV1_MESH_BASE				*F1Mesh ;
	MV1_MESH_FACE				*Face ;
	MV1_SKIN_BONE				*SkinBone ;
	MV1_SKIN_BONE				*F1SkinBone ;
	MV1_TRIANGLE_LIST_BASE		*TriangleList ;
	MV1_TRIANGLE_LIST_BASE		*F1TriangleList ;
	MV1_ANIM_KEYSET_BASE		*AnimKeySet ;
	MV1_ANIM_KEYSET_BASE		*F1AnimKeySet ;
	MV1_ANIM_BASE				*Anim ;
	MV1_ANIM_BASE				*F1Anim ;
	MV1_ANIMSET_BASE			*AnimSet ;
	MV1_ANIMSET_BASE			*F1AnimSet ;
	MV1_SHAPE_BASE				*Shape ;
	MV1_SHAPE_BASE				*F1Shape ;
	MV1_SHAPE_MESH_BASE			*ShapeMesh ;
	MV1_SHAPE_MESH_BASE			*F1ShapeMesh ;
	MV1_PHYSICS_RIGIDBODY_BASE	*PhysicsRigidBody ;
	MV1_PHYSICS_RIGIDBODY_BASE	*F1PhysicsRigidBody ;
	MV1_PHYSICS_JOINT_BASE		*PhysicsJoint ;
	MV1_PHYSICS_JOINT_BASE		*F1PhysicsJoint ;
	int i, j, k, NewHandle = 0 ;
	DWORD_PTR AllocSize ;

	// ����������Ă��Ȃ�������G���[
	if( MV1Man.Initialize == false ) return -1 ;

	// �n���h���`�F�b�N
	if( MV1BMDLCHK( SrcMBHandle, FHeader ) )
		return -1 ;

	// �K�v�ȃ������T�C�Y�̎Z�o
	{
		_MEMSET( &MTBase, 0, sizeof( MTBase ) ) ;
		AllocSize = 0 ;

		MTBase.Name = ( wchar_t * )AllocSize ;
		AllocSize += ( ( _WCSLEN( FHeader->Name ) + 1 ) * sizeof( wchar_t ) + 3 ) / 4 * 4 ;

		MTBase.FilePath = ( wchar_t * )AllocSize ;
		AllocSize += ( ( _WCSLEN( FHeader->FilePath ) + 1 ) * sizeof( wchar_t ) + 3 ) / 4 * 4 ;

		MTBase.DirectoryPath = ( wchar_t * )AllocSize ;
		AllocSize += ( ( _WCSLEN( FHeader->DirectoryPath ) + 1 ) * sizeof( wchar_t ) + 3 ) / 4 * 4 ;

		MTBase.Frame = ( MV1_FRAME_BASE * )AllocSize ;
		AllocSize += FHeader->FrameNum * sizeof( MV1_FRAME_BASE ) ;

		MTBase.FrameUseSkinBone = ( MV1_SKIN_BONE ** )AllocSize ;
		AllocSize += FHeader->FrameUseSkinBoneNum * sizeof( MV1_SKIN_BONE * ) ;

		MTBase.Material = ( MV1_MATERIAL_BASE * )AllocSize ;
		AllocSize += FHeader->MaterialNum * sizeof( MV1_MATERIAL_BASE ) ;

		MTBase.Light = ( MV1_LIGHT * )AllocSize ;
		AllocSize += FHeader->LightNum * sizeof( MV1_LIGHT ) ;

		MTBase.Texture = ( MV1_TEXTURE_BASE * )AllocSize ;
		AllocSize += FHeader->TextureNum * sizeof( MV1_TEXTURE_BASE ) ;

		MTBase.Mesh = ( MV1_MESH_BASE * )AllocSize ;
		AllocSize += FHeader->MeshNum * sizeof( MV1_MESH_BASE ) ;

		MTBase.SkinBone = ( MV1_SKIN_BONE * )AllocSize ;
		AllocSize += FHeader->SkinBoneNum * sizeof( MV1_SKIN_BONE ) ;
		MTBase.SkinBoneUseFrame = ( MV1_SKIN_BONE_USE_FRAME * )AllocSize ;
		AllocSize += FHeader->SkinBoneUseFrameNum * sizeof( MV1_SKIN_BONE_USE_FRAME ) ;

		MTBase.TriangleList = ( MV1_TRIANGLE_LIST_BASE * )AllocSize ;
		AllocSize += FHeader->TriangleListNum * sizeof( MV1_TRIANGLE_LIST_BASE ) ;

		MTBase.TriangleListIndex = ( WORD * )AllocSize ;
		AllocSize += sizeof( WORD ) * FHeader->TriangleListIndexNum * 2 ;

#ifndef UNICODE
		MTBase.StringBufferA = ( char * )AllocSize ;
		AllocSize += FHeader->StringSizeA ;
#endif

		MTBase.StringBufferW = ( wchar_t * )AllocSize ;
		AllocSize += FHeader->StringSizeW ;

		MTBase.ChangeDrawMaterialTable = ( DWORD * )AllocSize ;
		AllocSize += FHeader->ChangeDrawMaterialTableSize ;

		MTBase.ChangeMatrixTable = ( DWORD * )AllocSize ;
		AllocSize += FHeader->ChangeMatrixTableSize ;

		MTBase.AnimKeyData = ( void * )AllocSize ;
		AllocSize += FHeader->AnimKeyDataSize ;

		MTBase.AnimKeySet = ( MV1_ANIM_KEYSET_BASE * )AllocSize ;
		AllocSize += FHeader->AnimKeySetNum * sizeof( MV1_ANIM_KEYSET_BASE ) ;

		MTBase.Anim = ( MV1_ANIM_BASE * )AllocSize ;
		AllocSize += FHeader->AnimNum * sizeof( MV1_ANIM_BASE ) ;

		MTBase.AnimSet = ( MV1_ANIMSET_BASE * )AllocSize ;
		AllocSize += FHeader->AnimSetNum * sizeof( MV1_ANIMSET_BASE ) ;

		MTBase.AnimTargetFrameTable = ( MV1_ANIM_BASE ** )AllocSize ;
		AllocSize += FHeader->FrameNum * FHeader->AnimSetNum * sizeof( MV1_ANIM_BASE * ) ;

		if( FHeader->Shape != NULL )
		{
			MTBase.Shape = ( MV1_SHAPE_BASE * )AllocSize ;
			AllocSize += FHeader->ShapeNum * sizeof( MV1_SHAPE_BASE ) ;

			MTBase.ShapeMesh = ( MV1_SHAPE_MESH_BASE * )AllocSize ;
			AllocSize += FHeader->ShapeMeshNum * sizeof( MV1_SHAPE_MESH_BASE ) ;
		}

		if( FHeader->PhysicsJoint != NULL )
		{
			MTBase.PhysicsRigidBody = ( MV1_PHYSICS_RIGIDBODY_BASE * )AllocSize ;
			AllocSize += FHeader->PhysicsRigidBodyNum * sizeof( MV1_PHYSICS_RIGIDBODY_BASE ) ;

			MTBase.PhysicsJoint = ( MV1_PHYSICS_JOINT_BASE * )AllocSize ;
			AllocSize += FHeader->PhysicsJointNum * sizeof( MV1_PHYSICS_JOINT_BASE ) ;
		}
	}

	// ���f����f�[�^�n���h���̍쐬
	NewHandle = MV1AddModelBase( FALSE ) ;
	if( NewHandle < 0 )
	{
		goto ERRORLABEL ;
	}

	// �������̊m��
	if( MV1BMDLCHK_ASYNC( NewHandle, MBase ) )
	{
		return -1 ;
	}
	MBase->DataBuffer = MDALLOCMEM( AllocSize ) ;
	if( MBase->DataBuffer == NULL )
	{
		goto ERRORLABEL ;
	}
	_MEMSET( MBase->DataBuffer, 0, AllocSize ) ;
	MBase->AllocMemorySize = AllocSize ;

	// �������A�h���X�̃Z�b�g
	MBase->Name                   = ( wchar_t * )                   ( ( BYTE * )MBase->DataBuffer + ( DWORD_PTR )MTBase.Name                 ) ;
	MBase->FilePath               = ( wchar_t * )                   ( ( BYTE * )MBase->DataBuffer + ( DWORD_PTR )MTBase.FilePath             ) ;
	MBase->DirectoryPath          = ( wchar_t * )                   ( ( BYTE * )MBase->DataBuffer + ( DWORD_PTR )MTBase.DirectoryPath        ) ;
	MBase->ChangeMatrixTable      = ( DWORD * )                     ( ( BYTE * )MBase->DataBuffer + ( DWORD_PTR )MTBase.ChangeMatrixTable    ) ;
	MBase->ChangeDrawMaterialTable = ( DWORD * )                    ( ( BYTE * )MBase->DataBuffer + ( DWORD_PTR )MTBase.ChangeDrawMaterialTable ) ;
	MBase->Frame                  = ( MV1_FRAME_BASE * )            ( ( BYTE * )MBase->DataBuffer + ( DWORD_PTR )MTBase.Frame                ) ;
	MBase->FrameUseSkinBone       = ( MV1_SKIN_BONE ** )            ( ( BYTE * )MBase->DataBuffer + ( DWORD_PTR )MTBase.FrameUseSkinBone     ) ;
	MBase->Material               = ( MV1_MATERIAL_BASE * )         ( ( BYTE * )MBase->DataBuffer + ( DWORD_PTR )MTBase.Material             ) ;
	MBase->Light                  = ( MV1_LIGHT * )                 ( ( BYTE * )MBase->DataBuffer + ( DWORD_PTR )MTBase.Light                ) ;
	MBase->Texture                = ( MV1_TEXTURE_BASE * )          ( ( BYTE * )MBase->DataBuffer + ( DWORD_PTR )MTBase.Texture              ) ;
	MBase->SkinBone               = ( MV1_SKIN_BONE * )             ( ( BYTE * )MBase->DataBuffer + ( DWORD_PTR )MTBase.SkinBone             ) ;
	MBase->SkinBoneUseFrame       = ( MV1_SKIN_BONE_USE_FRAME * )   ( ( BYTE * )MBase->DataBuffer + ( DWORD_PTR )MTBase.SkinBoneUseFrame     ) ;
	MBase->Mesh                   = ( MV1_MESH_BASE * )             ( ( BYTE * )MBase->DataBuffer + ( DWORD_PTR )MTBase.Mesh                 ) ;
	MBase->TriangleList           = ( MV1_TRIANGLE_LIST_BASE * )    ( ( BYTE * )MBase->DataBuffer + ( DWORD_PTR )MTBase.TriangleList         ) ;
	MBase->TriangleListIndex      = ( WORD * )                      ( ( BYTE * )MBase->DataBuffer + ( DWORD_PTR )MTBase.TriangleListIndex    ) ;
#ifndef UNICODE
	MBase->StringBufferA          = ( char * )                      ( ( BYTE * )MBase->DataBuffer + ( DWORD_PTR )MTBase.StringBufferA        ) ;
#endif
	MBase->StringBufferW          = ( wchar_t * )                   ( ( BYTE * )MBase->DataBuffer + ( DWORD_PTR )MTBase.StringBufferW        ) ;
	MBase->AnimSet                = ( MV1_ANIMSET_BASE * )          ( ( BYTE * )MBase->DataBuffer + ( DWORD_PTR )MTBase.AnimSet              ) ;
	MBase->Anim                   = ( MV1_ANIM_BASE * )             ( ( BYTE * )MBase->DataBuffer + ( DWORD_PTR )MTBase.Anim                 ) ;
	MBase->AnimKeySet             = ( MV1_ANIM_KEYSET_BASE * )      ( ( BYTE * )MBase->DataBuffer + ( DWORD_PTR )MTBase.AnimKeySet           ) ;
	MBase->AnimKeyData            = ( void * )                      ( ( BYTE * )MBase->DataBuffer + ( DWORD_PTR )MTBase.AnimKeyData          ) ;
	MBase->AnimTargetFrameTable   = ( MV1_ANIM_BASE ** )            ( ( BYTE * )MBase->DataBuffer + ( DWORD_PTR )MTBase.AnimTargetFrameTable ) ;
	if( FHeader->Shape )
	{
		MBase->Shape              = ( MV1_SHAPE_BASE * )            ( ( BYTE * )MBase->DataBuffer + ( DWORD_PTR )MTBase.Shape                ) ;
		MBase->ShapeMesh          = ( MV1_SHAPE_MESH_BASE * )       ( ( BYTE * )MBase->DataBuffer + ( DWORD_PTR )MTBase.ShapeMesh            ) ;
	}
	if( FHeader->PhysicsRigidBody )
	{
		MBase->PhysicsRigidBody   = ( MV1_PHYSICS_RIGIDBODY_BASE * )( ( BYTE * )MBase->DataBuffer + ( DWORD_PTR )MTBase.PhysicsRigidBody     ) ;
		MBase->PhysicsJoint       = ( MV1_PHYSICS_JOINT_BASE *     )( ( BYTE * )MBase->DataBuffer + ( DWORD_PTR )MTBase.PhysicsJoint         ) ;
	}

	// ���_�f�[�^�C���f�b�N�X�l�̃R�s�[
	if( FHeader->TriangleListIndexNum != 0 )
	{
		_MEMCPY( MBase->TriangleListIndex, FHeader->TriangleListIndex, FHeader->TriangleListIndexNum * sizeof( WORD ) * 2 ) ;
	}

	// �A�j���[�V�����L�[�f�[�^�̃R�s�[
	if( FHeader->AnimKeyDataSize != 0 )
	{
		_MEMCPY( MBase->AnimKeyData, FHeader->AnimKeyData, ( size_t )FHeader->AnimKeyDataSize ) ;
	}

	// ���f�����ƃt�@�C���p�X�ƃt�H���_�p�X��ۑ�
	_WCSCPY( MBase->Name,          FHeader->Name ) ;
	_WCSCPY( MBase->FilePath,      FHeader->FilePath ) ;
	_WCSCPY( MBase->DirectoryPath, FHeader->DirectoryPath ) ;

	// ���������`��ɑΉ����邩�ǂ�����ۑ�
	MBase->UsePackDraw = FHeader->UsePackDraw ;

	// ���W�n����ۑ�
	MBase->RightHandType = FHeader->RightHandType ;

	// ���蓖�Ă��Ă���}�e���A���̔ԍ����Ⴂ���b�V������`�悷�邩�ǂ����̏���ۑ�
	MBase->MaterialNumberOrderDraw = FHeader->MaterialNumberOrderDraw ;

	// �ύX�`�F�b�N�p�e�[�u���ɕK�v�ȃf�[�^�̃T�C�Y�̕ۑ��ƃR�s�[
	MBase->ChangeDrawMaterialTableSize  = FHeader->ChangeDrawMaterialTableSize ;
	MBase->ChangeMatrixTableSize        = FHeader->ChangeMatrixTableSize ;
	_MEMCPY( MBase->ChangeDrawMaterialTable, FHeader->ChangeDrawMaterialTable, ( size_t )FHeader->ChangeDrawMaterialTableSize ) ;
	_MEMCPY( MBase->ChangeMatrixTable,       FHeader->ChangeMatrixTable,       ( size_t )FHeader->ChangeMatrixTableSize ) ;

	// �|���S������ۑ�
	MBase->TriangleNum = FHeader->TriangleNum ;

	// ���_�f�[�^�̐���ۑ�
	MBase->TriangleListVertexNum = FHeader->TriangleListVertexNum ;

	// �t���[�����g�p���Ă���{�[���ւ̃|�C���^�̏����R�s�[
	MBase->FrameUseSkinBoneNum = FHeader->FrameUseSkinBoneNum ;
	for( i = 0 ; i < MBase->FrameUseSkinBoneNum ; i ++ )
	{
		MBase->FrameUseSkinBone[ i ] = MBase->SkinBone + ( FHeader->FrameUseSkinBone[ i ] - FHeader->SkinBone ) ;
	}

	// �X�L�j���O�p�{�[�����g�p����t���[���̏����R�s�[
	_MEMCPY( MBase->SkinBoneUseFrame, FHeader->SkinBoneUseFrame, FHeader->SkinBoneUseFrameNum * sizeof( MV1_SKIN_BONE_USE_FRAME_F1 ) ) ;

	// ������f�[�^���R�s�[
#ifndef UNICODE
	_MEMCPY( MBase->StringBufferA, FHeader->StringBufferA, ( size_t )FHeader->StringSizeA ) ;
	MBase->StringSizeA = FHeader->StringSizeA ;
#endif
	_MEMCPY( MBase->StringBufferW, FHeader->StringBufferW, ( size_t )FHeader->StringSizeW ) ;
	MBase->StringSizeW = FHeader->StringSizeW ;

	// �e�I�u�W�F�N�g�̐��̏���ۑ�
	MBase->FrameNum              = FHeader->FrameNum ;
	MBase->MaterialNum           = FHeader->MaterialNum ;
	MBase->LightNum              = FHeader->LightNum ;
	MBase->TextureNum            = FHeader->TextureNum ;
	MBase->MeshNum               = FHeader->MeshNum ;
	MBase->SkinBoneNum           = FHeader->SkinBoneNum ;
	MBase->SkinBoneUseFrameNum   = FHeader->SkinBoneUseFrameNum ;
	MBase->TriangleListNum       = FHeader->TriangleListNum ;
	MBase->AnimKeyDataSize       = FHeader->AnimKeyDataSize ;
	MBase->AnimKeySetNum         = FHeader->AnimKeySetNum ;
	MBase->AnimNum               = FHeader->AnimNum ;
	MBase->AnimSetNum            = FHeader->AnimSetNum ;
	if( FHeader->Shape )
	{
		MBase->ShapeNum          = FHeader->ShapeNum ;
		MBase->ShapeMeshNum      = FHeader->ShapeMeshNum ;

		MBase->ShapeNormalPositionNum = FHeader->ShapeNormalPositionNum ;
		MBase->ShapeSkinPosition4BNum = FHeader->ShapeSkinPosition4BNum ;
		MBase->ShapeSkinPosition8BNum = FHeader->ShapeSkinPosition8BNum ;
		MBase->ShapeSkinPositionFREEBSize = FHeader->ShapeSkinPositionFREEBSize ;

		MBase->ShapeTargetMeshVertexNum = FHeader->ShapeTargetMeshVertexNum ;
	}
	if( FHeader->PhysicsRigidBody )
	{
		MBase->PhysicsGravity    = FHeader->PhysicsGravity ;

		MBase->PhysicsRigidBodyNum = FHeader->PhysicsRigidBodyNum ;
		MBase->PhysicsJointNum   = FHeader->PhysicsJointNum ;
	}

	// ���_�f�[�^���i�[���邽�߂̃������̈�̊m��
	{
		MBase->VertexDataSize = 
			FHeader->MeshVertexIndexNum                * sizeof( DWORD )                 +
			FHeader->MeshFaceNum                       * sizeof( MV1_MESH_FACE )         +
			FHeader->MeshNormalNum                     * sizeof( MV1_MESH_NORMAL )       +
			FHeader->TriangleListNormalPositionNum     * sizeof( MV1_TLIST_NORMAL_POS )  +
			FHeader->TriangleListSkinPosition4BNum     * sizeof( MV1_TLIST_SKIN_POS_4B ) +
			FHeader->TriangleListSkinPosition8BNum     * sizeof( MV1_TLIST_SKIN_POS_8B ) +
			FHeader->TriangleListSkinPositionFREEBSize                                   +
			FHeader->MeshPositionSize                                                    +
			FHeader->MeshVertexSize                                                      + 16 ;
		/*
		F1Mesh = FHeader->Mesh ;
		MeshVertexNum = 0 ;
		for( i = 0 ; i < FHeader->MeshNum ; i ++, F1Mesh ++ )
		{
			MeshVertexNum += F1Mesh->VertexNum ;
		}
		MBase->VertexDataSize += MeshVertexNum * sizeof( DWORD ) ;
		*/
		if( FHeader->Shape )
		{
			MBase->VertexDataSize += FHeader->ShapeVertexNum * sizeof( MV1_SHAPE_VERTEX_BASE ) ;
		}
		if( MBase->VertexDataSize != 0 )
		{
			MBase->VertexData = DXALLOC( MBase->VertexDataSize + 16 ) ;
			if( MBase->VertexData == NULL ) goto ERRORLABEL ;
			_MEMCPY( ( void * )( ( ( DWORD_PTR )MBase->VertexData + 15 ) / 16 * 16 ), ( void * )( ( ( DWORD_PTR )FHeader->VertexData + 15 ) / 16 * 16 ), MBase->VertexDataSize ) ;
			//_MEMSET( ( void * )( ( ( DWORD_PTR )MBase->VertexData + 15 ) / 16 * 16 ), 0, MBase->VertexDataSize ) ;
		}

		MBase->TriangleListNormalPosition    = ( MV1_TLIST_NORMAL_POS     * )( ( ( DWORD_PTR )MBase->VertexData + 15 ) / 16 * 16 ) ;
		MBase->TriangleListSkinPosition4B    = ( MV1_TLIST_SKIN_POS_4B    * )( MBase->TriangleListNormalPosition              + FHeader->TriangleListNormalPositionNum     ) ;
		MBase->TriangleListSkinPosition8B    = ( MV1_TLIST_SKIN_POS_8B    * )( MBase->TriangleListSkinPosition4B              + FHeader->TriangleListSkinPosition4BNum     ) ;
		MBase->TriangleListSkinPositionFREEB = ( MV1_TLIST_SKIN_POS_FREEB * )( MBase->TriangleListSkinPosition8B              + FHeader->TriangleListSkinPosition8BNum     ) ;
		MBase->MeshVertexIndex               = ( DWORD                    * )( ( BYTE * )MBase->TriangleListSkinPositionFREEB + FHeader->TriangleListSkinPositionFREEBSize ) ;
		MBase->MeshFace                      = ( MV1_MESH_FACE            * )( MBase->MeshVertexIndex                         + FHeader->MeshVertexIndexNum    ) ;
		MBase->MeshNormal                    = ( MV1_MESH_NORMAL          * )( MBase->MeshFace                                + FHeader->MeshFaceNum           ) ;
		MBase->MeshPosition                  = ( MV1_MESH_POSITION        * )( MBase->MeshNormal                              + FHeader->MeshNormalNum         ) ;
		MBase->MeshVertex                    = ( MV1_MESH_VERTEX          * )( ( BYTE * )MBase->MeshPosition                  + FHeader->MeshPositionSize      ) ;
		if( FHeader->Shape )
		{
			MBase->ShapeVertex               = ( MV1_SHAPE_VERTEX_BASE    * )( ( BYTE * )MBase->MeshVertex                    + FHeader->MeshVertexSize        ) ;
		}
		if( FHeader->TriangleListNormalPositionNum == 0     ) MBase->TriangleListNormalPosition    = NULL ;
		if( FHeader->TriangleListSkinPosition4BNum == 0     ) MBase->TriangleListSkinPosition4B    = NULL ;
		if( FHeader->TriangleListSkinPosition8BNum == 0     ) MBase->TriangleListSkinPosition8B    = NULL ;
		if( FHeader->TriangleListSkinPositionFREEBSize == 0 ) MBase->TriangleListSkinPositionFREEB = NULL ;
		if( FHeader->MeshVertexIndexNum == 0                ) MBase->MeshVertexIndex   = NULL ;
		if( FHeader->MeshFaceNum == 0                       ) MBase->MeshFace          = NULL ;
		if( FHeader->MeshNormalNum == 0                     ) MBase->MeshNormal        = NULL ;
		if( FHeader->MeshPositionSize == 0                  ) MBase->MeshPosition      = NULL ;
		if( FHeader->MeshVertexSize == 0                    ) MBase->MeshVertex        = NULL ;
		if( FHeader->Shape == NULL || FHeader->ShapeVertexNum == 0 ) MBase->ShapeVertex = NULL ;
	}

	// �t���[���̏����Z�b�g
	MBase->TopFrameNum  = FHeader->TopFrameNum ;
	MBase->FirstTopFrame = FHeader->FirstTopFrame ? MBase->Frame + FHeader->FirstTopFrame->Index : NULL ;
	MBase->LastTopFrame  = FHeader->LastTopFrame  ? MBase->Frame + FHeader->LastTopFrame->Index  : NULL ;
	Frame = MBase->Frame ;
	F1Frame = FHeader->Frame ;
	for( i = 0 ; i < MBase->FrameNum ; i ++, Frame ++, F1Frame ++ )
	{
		Frame->Container = MBase ;

#ifndef UNICODE
		Frame->NameA = MBase->StringBufferA + ( F1Frame->NameA - FHeader->StringBufferA ) ;
#endif
		Frame->NameW = MBase->StringBufferW + ( F1Frame->NameW - FHeader->StringBufferW ) ;
		Frame->AutoCreateNormal = F1Frame->AutoCreateNormal ;

		Frame->Index = F1Frame->Index ;

		Frame->ChangeDrawMaterialInfo.Target   = F1Frame->ChangeDrawMaterialInfo.Target ;
		Frame->ChangeDrawMaterialInfo.Fill     = MBase->ChangeDrawMaterialTable + ( F1Frame->ChangeDrawMaterialInfo.Fill - FHeader->ChangeDrawMaterialTable ) ;
		Frame->ChangeDrawMaterialInfo.Size     = F1Frame->ChangeDrawMaterialInfo.Size ;
		Frame->ChangeDrawMaterialInfo.CheckBit = F1Frame->ChangeDrawMaterialInfo.CheckBit ;

		Frame->ChangeMatrixInfo.Target         = F1Frame->ChangeMatrixInfo.Target ;
		Frame->ChangeMatrixInfo.Fill           = MBase->ChangeMatrixTable       + ( F1Frame->ChangeMatrixInfo.Fill       - FHeader->ChangeMatrixTable ) ;
		Frame->ChangeMatrixInfo.Size           = F1Frame->ChangeMatrixInfo.Size ;
		Frame->ChangeMatrixInfo.CheckBit       = F1Frame->ChangeMatrixInfo.CheckBit ;

		Frame->TotalMeshNum  = F1Frame->TotalMeshNum ;
		Frame->TotalChildNum = F1Frame->TotalChildNum ;
		Frame->Parent     = F1Frame->Parent     ? MBase->Frame + F1Frame->Parent->Index : NULL ;
		Frame->FirstChild = F1Frame->FirstChild ? MBase->Frame + F1Frame->FirstChild->Index : NULL ;
		Frame->LastChild  = F1Frame->LastChild  ? MBase->Frame + F1Frame->LastChild->Index : NULL ;
		Frame->Prev       = F1Frame->Prev       ? MBase->Frame + F1Frame->Prev->Index : NULL ;
		Frame->Next       = F1Frame->Next       ? MBase->Frame + F1Frame->Next->Index : NULL ;

		Frame->Translate = F1Frame->Translate ;
		Frame->Scale = F1Frame->Scale ;
		Frame->Rotate = F1Frame->Rotate ;
		Frame->RotateOrder = F1Frame->RotateOrder ;
		Frame->Quaternion = F1Frame->Quaternion ;

		Frame->Flag = F1Frame->Flag ;
		if( F1Frame->Flag & MV1_FRAMEFLAG_IGNOREPARENTTRANS ) Frame->IgnoreParentTransform = 1 ;

		if( F1Frame->Flag & MV1_FRAMEFLAG_PREROTATE )
		{
			Frame->PreRotate = F1Frame->PreRotate ;
		}

		if( F1Frame->Flag & MV1_FRAMEFLAG_POSTROTATE )
		{
			Frame->PreRotate = F1Frame->PostRotate ;
		}

		Frame->IsSkinMesh = F1Frame->IsSkinMesh ;
		Frame->TriangleNum = F1Frame->TriangleNum ;
		Frame->VertexNum = F1Frame->VertexNum ;
		Frame->MeshNum = F1Frame->MeshNum ;
		if( F1Frame->Mesh )
		{
			Frame->Mesh = MBase->Mesh + ( F1Frame->Mesh - FHeader->Mesh ) ;
		}

		if( F1Frame->Shape )
		{
			Frame->ShapeNum = F1Frame->ShapeNum ;
			Frame->Shape = MBase->Shape + ( F1Frame->Shape - FHeader->Shape ) ;
		}

		Frame->SkinBoneNum = F1Frame->SkinBoneNum ;
		if( F1Frame->SkinBone )
		{
			Frame->SkinBone = MBase->SkinBone + ( F1Frame->SkinBone - FHeader->SkinBone ) ;
		}

		if( F1Frame->Light )
		{
			Frame->Light = MBase->Light + F1Frame->Light->Index ;
		}

		Frame->UseSkinBoneNum = F1Frame->UseSkinBoneNum ;
		if( F1Frame->UseSkinBoneNum )
		{
			Frame->UseSkinBone = MBase->FrameUseSkinBone + ( F1Frame->UseSkinBone - FHeader->FrameUseSkinBone ) ;
		}

		Frame->SmoothingAngle = F1Frame->SmoothingAngle ;

		Frame->MaxBoneBlendNum = F1Frame->MaxBoneBlendNum ;
		Frame->PositionNum     = F1Frame->PositionNum ;
		Frame->NormalNum       = F1Frame->NormalNum ;

		// ���W�f�[�^�Ɩ@���f�[�^�̃R�s�[
		if( F1Frame->PositionNum == 0 )
		{
			Frame->Position = NULL ;
			Frame->Normal = NULL ;
		}
		else
		{
			Frame->PosUnitSize = ( int )( sizeof( MV1_MESH_POSITION ) + ( Frame->MaxBoneBlendNum - 4 ) * sizeof( MV1_SKINBONE_BLEND ) ) ;

			Frame->Position = ( MV1_MESH_POSITION * )( ( BYTE * )MBase->MeshPosition + MBase->MeshPositionSize ) ;
			Frame->Normal   = MBase->MeshNormal + MBase->MeshNormalNum ;
			MBase->MeshPositionSize += Frame->PosUnitSize * Frame->PositionNum ;
			MBase->MeshNormalNum    += Frame->NormalNum ;

			// ���_�f�[�^�̃R�s�[
			_MEMCPY( Frame->Position, F1Frame->Position, ( size_t )( Frame->PosUnitSize * Frame->PositionNum ) ) ;

			// �@���f�[�^�̃R�s�[
			_MEMCPY( Frame->Normal, F1Frame->Normal, sizeof( MV1_MESH_NORMAL ) * Frame->NormalNum ) ;
		}
	}

	// �e�N�X�`���̏����Z�b�g
	MBase->TextureAllocMem = FALSE ;
	Texture = MBase->Texture ;
	F1Texture = FHeader->Texture ;
	for( i = 0 ; i < MBase->TextureNum ; i ++, Texture ++, F1Texture ++ )
	{
		// ���[�U�[�f�[�^��������
		Texture->UserData[ 0 ] = 0 ;
		Texture->UserData[ 1 ] = 0 ;
		Texture->UseUserGraphHandle = 0 ;
		Texture->UserGraphHandle = 0 ;

		// ���O��ۑ�
		Texture->NameAllocMem = FALSE ;
#ifndef UNICODE
		Texture->NameA = MBase->StringBufferA + ( F1Texture->NameA - FHeader->StringBufferA ) ;
#endif
		Texture->NameW = MBase->StringBufferW + ( F1Texture->NameW - FHeader->StringBufferW ) ;

		// ���]�t���O���Z�b�g����
		Texture->ReverseFlag = F1Texture->ReverseFlag ;

		// �R�Q�r�b�g�a�l�o�̃A���t�@�l���S���O�������� XRGB8 �Ƃ��Ĉ������ǂ����̃t���O���Z�b�g����
		Texture->Bmp32AllZeroAlphaToXRGB8Flag = F1Texture->Bmp32AllZeroAlphaToXRGB8Flag ;

		// �e�N�X�`���̓ǂݍ���
		if( __MV1LoadTexture(
				&F1Texture->ColorImage, &F1Texture->ColorImageSize,
				&F1Texture->AlphaImage, &F1Texture->AlphaImageSize,
				&Texture->GraphHandle,
				&Texture->SemiTransFlag,
				&Texture->IsDefaultTexture,
#ifndef UNICODE
				NULL, NULL,
#endif
				NULL, NULL,
				NULL,
				NULL,
				NULL,
				F1Texture->BumpImageFlag, F1Texture->BumpImageNextPixelLength,
				Texture->ReverseFlag,
				Texture->Bmp32AllZeroAlphaToXRGB8Flag,
				NULL,
				true,
				TRUE,
				FALSE ) == -1 )
		{
			DXST_ERRORLOGFMT_ADDW(( L"MV1 CloneModel Error : Texture LoadError : %s\n", Texture->NameW ) ) ;
			goto ERRORLABEL ;
		}

		// �t�@�C���p�X�p�Ƀ��������m�ۂ����t���O��|��
		Texture->ColorImageFilePathAllocMem = FALSE ;
		Texture->AlphaImageFilePathAllocMem = FALSE ;

		// �t�@�C���p�X��ۑ�
		if( Texture->ColorImage )
		{
#ifndef UNICODE
			Texture->ColorFilePathA = MBase->StringBufferA + ( F1Texture->ColorFilePathA - FHeader->StringBufferA ) ;
#endif
			Texture->ColorFilePathW = MBase->StringBufferW + ( F1Texture->ColorFilePathW - FHeader->StringBufferW ) ;
		}
		if( Texture->AlphaImage )
		{
#ifndef UNICODE
			Texture->AlphaFilePathA = MBase->StringBufferA + ( F1Texture->AlphaFilePathA - FHeader->StringBufferA ) ;
#endif
			Texture->AlphaFilePathW = MBase->StringBufferW + ( F1Texture->AlphaFilePathW - FHeader->StringBufferW ) ;
		}

		// �A�h���b�V���O���[�h�̃Z�b�g
		Texture->AddressModeU = F1Texture->AddressModeU ;
		Texture->AddressModeV = F1Texture->AddressModeV ;

		// �t�u�̃X�P�[���l���Z�b�g
		Texture->ScaleU = F1Texture->ScaleU ;
		Texture->ScaleV = F1Texture->ScaleV ;

		// �t�B���^�����O���[�h�̃Z�b�g
		Texture->FilterMode = F1Texture->FilterMode ;

		// �o���v�}�b�v����ۑ�
		Texture->BumpImageFlag = F1Texture->BumpImageFlag ;
		Texture->BumpImageNextPixelLength = F1Texture->BumpImageNextPixelLength ;

		// �摜�̃T�C�Y���擾
		NS_GetGraphSize( Texture->GraphHandle, &Texture->Width, &Texture->Height ) ;
	}

	// �}�e���A���̏����Z�b�g
	Material = MBase->Material ;
	F1Material = FHeader->Material ;
	for( i = 0 ; i < MBase->MaterialNum ; i ++, Material ++, F1Material ++ )
	{
#ifndef UNICODE
		Material->NameA = MBase->StringBufferA + ( F1Material->NameA - FHeader->StringBufferA ) ;
#endif
		Material->NameW = MBase->StringBufferW + ( F1Material->NameW - FHeader->StringBufferW ) ;

		Material->Diffuse  = F1Material->Diffuse ;
		Material->Ambient  = F1Material->Ambient ;
		Material->Specular = F1Material->Specular ;
		Material->Emissive = F1Material->Emissive ;
		Material->Power    = F1Material->Power ;

		Material->UseAlphaTest = F1Material->UseAlphaTest ;
		Material->AlphaFunc    = F1Material->AlphaFunc ;
		Material->AlphaRef     = F1Material->AlphaRef ;

		Material->DiffuseLayerNum = F1Material->DiffuseLayerNum ;
		MaterialLayer      = Material->DiffuseLayer ;
		F1MaterialLayer    = F1Material->DiffuseLayer ;
		for( j = 0 ; j < 8 ; j ++, MaterialLayer ++, F1MaterialLayer ++ )
		{
			MaterialLayer->BlendType = F1MaterialLayer->BlendType ;
			MaterialLayer->Texture = F1MaterialLayer->Texture ;
		}

		Material->SpecularLayerNum = F1Material->SpecularLayerNum ;
		MaterialLayer      = Material->SpecularLayer ;
		F1MaterialLayer    = F1Material->SpecularLayer ;
		for( j = 0 ; j < 8 ; j ++, MaterialLayer ++, F1MaterialLayer ++ )
		{
			MaterialLayer->BlendType = F1MaterialLayer->BlendType ;
			MaterialLayer->Texture = F1MaterialLayer->Texture ;
		}

		Material->NormalLayerNum = F1Material->NormalLayerNum ;
		MaterialLayer      = Material->NormalLayer ;
		F1MaterialLayer    = F1Material->NormalLayer ;
		for( j = 0 ; j < 8 ; j ++, MaterialLayer ++, F1MaterialLayer ++ )
		{
			MaterialLayer->BlendType = F1MaterialLayer->BlendType ;
			MaterialLayer->Texture = F1MaterialLayer->Texture ;
		}

		Material->DrawBlendMode = F1Material->DrawBlendMode ;
		Material->DrawBlendParam = F1Material->DrawBlendParam ;

		Material->Type = F1Material->Type ;
		Material->DiffuseGradBlendType = F1Material->DiffuseGradBlendType ;
		Material->DiffuseGradTexture = F1Material->DiffuseGradTexture ;
		Material->SphereMapTexture = F1Material->SphereMapTexture ;
		Material->SpecularGradBlendType = F1Material->SpecularGradBlendType ;
		Material->SpecularGradTexture = F1Material->SpecularGradTexture ;
		Material->SphereMapBlendType = F1Material->SphereMapBlendType ;
		Material->OutLineColor = F1Material->OutLineColor ;
		Material->OutLineWidth = F1Material->OutLineWidth ;
		Material->OutLineDotWidth = F1Material->OutLineDotWidth ;
	}

	// ���C�g�̏����Z�b�g
	Light = MBase->Light ;
	F1Light = FHeader->Light ;
	for( i = 0 ; i < MBase->LightNum ; i ++, Light ++, F1Light ++ )
	{
#ifndef UNICODE
		Light->NameA = MBase->StringBufferA + ( F1Light->NameA - FHeader->StringBufferA ) ;
#endif
		Light->NameW = MBase->StringBufferW + ( F1Light->NameW - FHeader->StringBufferW ) ;

		Light->Index = F1Light->Index ;
		Light->FrameIndex = F1Light->FrameIndex ;
		Light->Type = F1Light->Type ;
		Light->Diffuse = F1Light->Diffuse ;
		Light->Specular = F1Light->Specular ;
		Light->Ambient = F1Light->Ambient ;
		Light->Range = F1Light->Range ;
		Light->Falloff = F1Light->Falloff ;
		Light->Attenuation0 = F1Light->Attenuation0 ;
		Light->Attenuation1 = F1Light->Attenuation1 ;
		Light->Attenuation2 = F1Light->Attenuation2 ;
		Light->Theta = F1Light->Theta ;
		Light->Phi = F1Light->Phi ;
	}

	// ���b�V���̏����Z�b�g
	Mesh = MBase->Mesh ;
	F1Mesh = FHeader->Mesh ;
	for( i = 0 ; i < MBase->MeshNum ; i ++, Mesh ++, F1Mesh ++ )
	{
		if( F1Mesh->Container )
		{
			Mesh->Container = MBase->Frame + F1Mesh->Container->Index ;
		}
		if( F1Mesh->Material )
		{
			Mesh->Material = MBase->Material + ( F1Mesh->Material - FHeader->Material ) ;
		}

		Mesh->ChangeInfo.Target   = F1Mesh->ChangeInfo.Target ;
		Mesh->ChangeInfo.Fill     = NULL ;
		Mesh->ChangeInfo.Size     = F1Mesh->ChangeInfo.Size ;
		Mesh->ChangeInfo.CheckBit = F1Mesh->ChangeInfo.CheckBit ;

		Mesh->NotOneDiffuseAlpha = F1Mesh->NotOneDiffuseAlpha ;
		Mesh->Shape = F1Mesh->Shape ;
		Mesh->UseVertexDiffuseColor = F1Mesh->UseVertexDiffuseColor ;
		Mesh->UseVertexSpecularColor = F1Mesh->UseVertexSpecularColor ;

		Mesh->TriangleListNum = F1Mesh->TriangleListNum ;
		if( F1Mesh->TriangleList )
		{
			Mesh->TriangleList = MBase->TriangleList + ( F1Mesh->TriangleList - FHeader->TriangleList ) ;
		}

		Mesh->Visible = F1Mesh->Visible ;
		Mesh->BackCulling = F1Mesh->BackCulling ;

		Mesh->UVSetUnitNum = F1Mesh->UVSetUnitNum ;
		Mesh->UVUnitNum = F1Mesh->UVUnitNum ;

		Mesh->VertexNum = F1Mesh->VertexNum ;
		Mesh->FaceNum = 0 ;

		Mesh->VertUnitSize = ( int )( sizeof( MV1_MESH_VERTEX ) + Mesh->UVSetUnitNum * Mesh->UVUnitNum * sizeof( float ) - sizeof( float ) * 2 ) ;

		// ���_�f�[�^���擾����
		{
			Mesh->Vertex = ( MV1_MESH_VERTEX * )( ( BYTE * )MBase->MeshVertex + MBase->MeshVertexSize ) ;
			Mesh->Face   = MBase->MeshFace + MBase->MeshFaceNum ;
			MBase->MeshVertexSize += Mesh->VertexNum * Mesh->VertUnitSize ;
			MBase->MeshFaceNum    += F1Mesh->FaceNum ;
		}
	}

	if( FHeader->Shape )
	{
		// �V�F�C�v�̏����Z�b�g
		Shape = MBase->Shape ;
		F1Shape = FHeader->Shape ;
		for( i = 0 ; i < MBase->ShapeNum ; i ++, Shape ++, F1Shape ++ )
		{
			if( F1Shape->Container )
			{
				Shape->Container = MBase->Frame + F1Shape->Container->Index ;
			}

#ifndef UNICODE
			Shape->NameA = MBase->StringBufferA + ( F1Shape->NameA - FHeader->StringBufferA ) ;
#endif
			Shape->NameW = MBase->StringBufferW + ( F1Shape->NameW - FHeader->StringBufferW ) ;

			Shape->MeshNum = F1Shape->MeshNum ;

			if( F1Shape->Mesh )
			{
				Shape->Mesh = MBase->ShapeMesh + ( F1Shape->Mesh - FHeader->ShapeMesh ) ;
			}
		}

		// �V�F�C�v���b�V���̏����Z�b�g
		ShapeMesh = MBase->ShapeMesh ;
		F1ShapeMesh = FHeader->ShapeMesh ;
		for( i = 0 ; i < MBase->ShapeMeshNum ; i ++, ShapeMesh ++, F1ShapeMesh ++ )
		{
			if( F1ShapeMesh->TargetMesh )
			{
				ShapeMesh->TargetMesh = MBase->Mesh + ( F1ShapeMesh->TargetMesh - FHeader->Mesh ) ;
			}

			ShapeMesh->VertexNum = F1ShapeMesh->VertexNum ;

			if( F1ShapeMesh->Vertex )
			{
				ShapeMesh->Vertex = MBase->ShapeVertex + MBase->ShapeVertexNum ;
				MBase->ShapeVertexNum += ShapeMesh->VertexNum ;
			}
		}
	}

	if( FHeader->PhysicsRigidBody )
	{
		// ���̂̏����Z�b�g
		PhysicsRigidBody = MBase->PhysicsRigidBody ;
		F1PhysicsRigidBody = FHeader->PhysicsRigidBody ;
		for( i = 0 ; i < MBase->PhysicsRigidBodyNum ; i ++, PhysicsRigidBody ++, F1PhysicsRigidBody ++ )
		{
			PhysicsRigidBody->Index = F1PhysicsRigidBody->Index ;

#ifndef UNICODE
			PhysicsRigidBody->NameA = MBase->StringBufferA + ( F1PhysicsRigidBody->NameA - FHeader->StringBufferA ) ;
#endif
			PhysicsRigidBody->NameW = MBase->StringBufferW + ( F1PhysicsRigidBody->NameW - FHeader->StringBufferW ) ;

			if( F1PhysicsRigidBody->TargetFrame )
			{
				PhysicsRigidBody->TargetFrame = MBase->Frame + F1PhysicsRigidBody->TargetFrame->Index ;
			}

			PhysicsRigidBody->RigidBodyGroupIndex = F1PhysicsRigidBody->RigidBodyGroupIndex ;
			PhysicsRigidBody->RigidBodyGroupTarget = F1PhysicsRigidBody->RigidBodyGroupTarget ;
			PhysicsRigidBody->ShapeType = F1PhysicsRigidBody->ShapeType ;
			PhysicsRigidBody->ShapeW = F1PhysicsRigidBody->ShapeW ;
			PhysicsRigidBody->ShapeH = F1PhysicsRigidBody->ShapeH ;
			PhysicsRigidBody->ShapeD = F1PhysicsRigidBody->ShapeD ;
			PhysicsRigidBody->Position = F1PhysicsRigidBody->Position ;
			PhysicsRigidBody->Rotation = F1PhysicsRigidBody->Rotation ;
			PhysicsRigidBody->RigidBodyWeight = F1PhysicsRigidBody->RigidBodyWeight ;
			PhysicsRigidBody->RigidBodyPosDim = F1PhysicsRigidBody->RigidBodyPosDim ;
			PhysicsRigidBody->RigidBodyRotDim = F1PhysicsRigidBody->RigidBodyRotDim ;
			PhysicsRigidBody->RigidBodyRecoil = F1PhysicsRigidBody->RigidBodyRecoil ;
			PhysicsRigidBody->RigidBodyFriction = F1PhysicsRigidBody->RigidBodyFriction ;
			PhysicsRigidBody->RigidBodyType = F1PhysicsRigidBody->RigidBodyType ;
			PhysicsRigidBody->NoCopyToBone = F1PhysicsRigidBody->NoCopyToBone ;
		}

		// ���̂̃W���C���g�����Z�b�g
		PhysicsJoint = MBase->PhysicsJoint ;
		F1PhysicsJoint = FHeader->PhysicsJoint ;
		for( i = 0 ; i < MBase->PhysicsJointNum ; i ++, PhysicsJoint ++, F1PhysicsJoint ++ )
		{
			PhysicsJoint->Index = F1PhysicsJoint->Index ;

#ifndef UNICODE
			PhysicsJoint->NameA = MBase->StringBufferA + ( F1PhysicsJoint->NameA - FHeader->StringBufferA ) ;
#endif
			PhysicsJoint->NameW = MBase->StringBufferW + ( F1PhysicsJoint->NameW - FHeader->StringBufferW ) ;

			if( F1PhysicsJoint->RigidBodyA )
			{
				PhysicsJoint->RigidBodyA = MBase->PhysicsRigidBody + F1PhysicsJoint->RigidBodyA->Index ;
			}

			if( F1PhysicsJoint->RigidBodyB )
			{
				PhysicsJoint->RigidBodyB = MBase->PhysicsRigidBody + F1PhysicsJoint->RigidBodyB->Index ;
			}

			PhysicsJoint->Position = F1PhysicsJoint->Position ;
			PhysicsJoint->Rotation = F1PhysicsJoint->Rotation ;
			PhysicsJoint->ConstrainPosition1 = F1PhysicsJoint->ConstrainPosition1 ;
			PhysicsJoint->ConstrainPosition2 = F1PhysicsJoint->ConstrainPosition2 ;
			PhysicsJoint->ConstrainRotation1 = F1PhysicsJoint->ConstrainRotation1 ;
			PhysicsJoint->ConstrainRotation2 = F1PhysicsJoint->ConstrainRotation2 ;
			PhysicsJoint->SpringPosition = F1PhysicsJoint->SpringPosition ;
			PhysicsJoint->SpringRotation = F1PhysicsJoint->SpringRotation ;
		}
	}

	// �X�L�j���O���b�V���p�̃{�[�������Z�b�g
	SkinBone = MBase->SkinBone ;
	F1SkinBone = FHeader->SkinBone ;
	for( i = 0 ; i < MBase->SkinBoneNum ; i ++, SkinBone ++, F1SkinBone ++ )
	{
		SkinBone->BoneFrame = F1SkinBone->BoneFrame ;
		SkinBone->ModelLocalMatrix = F1SkinBone->ModelLocalMatrix ;
		SkinBone->ModelLocalMatrixIsTranslateOnly = F1SkinBone->ModelLocalMatrixIsTranslateOnly ;
		SkinBone->UseFrameNum = F1SkinBone->UseFrameNum ;
		if( F1SkinBone->UseFrame )
		{
			SkinBone->UseFrame = MBase->SkinBoneUseFrame + ( F1SkinBone->UseFrame - FHeader->SkinBoneUseFrame ) ;
		}
	}

	// �g���C�A���O�����X�g�̏����Z�b�g
	TriangleList = MBase->TriangleList ;
	F1TriangleList = FHeader->TriangleList ;
	for( i = 0 ; i < MBase->TriangleListNum ; i ++, TriangleList ++, F1TriangleList ++ )
	{
		if( F1TriangleList->Container )
		{
			TriangleList->Container = MBase->Mesh + ( F1TriangleList->Container - FHeader->Mesh ) ;
		}
		TriangleList->VertexType = F1TriangleList->VertexType ;
		TriangleList->VertexNum = F1TriangleList->VertexNum ;
		TriangleList->IndexNum = F1TriangleList->IndexNum ;

		// ���_�f�[�^�̓ǂݍ���
		{
			TriangleList->Index = MBase->TriangleListIndex + MBase->TriangleListIndexNum ;
			TriangleList->ToonOutLineIndex = TriangleList->Index + FHeader->TriangleListIndexNum ;
			TriangleList->ToonOutLineIndexNum = 0 ;
			TriangleList->MeshVertexIndex   = MBase->MeshVertexIndex   + MBase->MeshVertexIndexNum ;
			MBase->TriangleListIndexNum += TriangleList->IndexNum ;
			MBase->MeshVertexIndexNum   += TriangleList->VertexNum ;

			// �{�[���̏����擾
			switch( TriangleList->VertexType )
			{
			case MV1_VERTEX_TYPE_SKIN_4BONE :
			case MV1_VERTEX_TYPE_SKIN_8BONE :
				TriangleList->UseBoneNum = F1TriangleList->UseBoneNum ;
				for( j = 0 ; j < TriangleList->UseBoneNum ; j ++ )
				{
					TriangleList->UseBone[ j ] = F1TriangleList->UseBone[ j ] ;
				}
				break ;

			case MV1_VERTEX_TYPE_SKIN_FREEBONE :
				TriangleList->MaxBoneNum = F1TriangleList->MaxBoneNum ;
				break ;
			}

			// �����A�N�Z�X�p�̒��_���W�f�[�^���Z�b�g
			switch( TriangleList->VertexType )
			{
			case MV1_VERTEX_TYPE_NORMAL :
				TriangleList->NormalPosition = MBase->TriangleListNormalPosition + MBase->TriangleListNormalPositionNum ;
				TriangleList->NormalPosition = ( MV1_TLIST_NORMAL_POS * )( ( ( DWORD_PTR )TriangleList->NormalPosition + 15 ) / 16 * 16 ) ;
				TriangleList->PosUnitSize = sizeof( MV1_TLIST_NORMAL_POS ) ;
				MBase->TriangleListNormalPositionNum += TriangleList->VertexNum ;
				break ;

			case MV1_VERTEX_TYPE_SKIN_4BONE :
				TriangleList->SkinPosition4B = MBase->TriangleListSkinPosition4B + MBase->TriangleListSkinPosition4BNum ;
				TriangleList->SkinPosition4B = ( MV1_TLIST_SKIN_POS_4B * )( ( ( DWORD_PTR )TriangleList->SkinPosition4B + 15 ) / 16 * 16 ) ;
				TriangleList->PosUnitSize = sizeof( MV1_TLIST_SKIN_POS_4B ) ;
				MBase->TriangleListSkinPosition4BNum += TriangleList->VertexNum ;
				break ;

			case MV1_VERTEX_TYPE_SKIN_8BONE :
				TriangleList->SkinPosition8B = MBase->TriangleListSkinPosition8B + MBase->TriangleListSkinPosition8BNum ;
				TriangleList->SkinPosition8B = ( MV1_TLIST_SKIN_POS_8B * )( ( ( DWORD_PTR )TriangleList->SkinPosition8B + 15 ) / 16 * 16 ) ;
				TriangleList->PosUnitSize = sizeof( MV1_TLIST_SKIN_POS_8B ) ;
				MBase->TriangleListSkinPosition8BNum += TriangleList->VertexNum ;
				break ;

			case MV1_VERTEX_TYPE_SKIN_FREEBONE :
				TriangleList->SkinPositionFREEB = ( MV1_TLIST_SKIN_POS_FREEB * )( ( BYTE * )MBase->TriangleListSkinPositionFREEB + MBase->TriangleListSkinPositionFREEBSize ) ;
				TriangleList->SkinPositionFREEB = ( MV1_TLIST_SKIN_POS_FREEB * )( ( ( DWORD_PTR )TriangleList->SkinPositionFREEB + 15 ) / 16 * 16 ) ;
				TriangleList->PosUnitSize = ( unsigned short )( sizeof( MV1_TLIST_SKIN_POS_FREEB ) + sizeof( MV1_SKINBONE_BLEND ) * ( TriangleList->MaxBoneNum - 4 ) ) ;
				TriangleList->PosUnitSize = ( unsigned short )( ( TriangleList->PosUnitSize + 15 ) / 16 * 16 ) ;
				MBase->TriangleListSkinPositionFREEBSize += TriangleList->PosUnitSize * TriangleList->VertexNum ;
				break ;
			}
//			MV1SetupTriangleListPositionAndNormal( TriangleList ) ;
		}
	}

	// ���b�V���̃t�F�C�X�����\�z����
	{
		WORD *Ind ;
		DWORD *MInd, TListInd ;

		TriangleList = MBase->TriangleList ;
		for( i = 0 ; i < MBase->TriangleListNum ; i ++, TriangleList ++ )
		{
			Mesh = TriangleList->Container ;
			Ind = TriangleList->Index ;
			MInd = TriangleList->MeshVertexIndex ;
			TListInd = ( DWORD )( TriangleList - Mesh->TriangleList ) ;
			for( j = 0 ; j < TriangleList->IndexNum ; j += 3, Ind += 3 )
			{
				Face = &Mesh->Face[ Mesh->FaceNum ] ;
				Face->TriangleListIndex = ( WORD )TListInd ;
				Face->VertexType = TriangleList->VertexType ;
				Face->VertexIndex[ 0 ] = MInd[ Ind[ 0 ] ] ;
				Face->VertexIndex[ 1 ] = MInd[ Ind[ 1 ] ] ;
				Face->VertexIndex[ 2 ] = MInd[ Ind[ 2 ] ] ;
				Mesh->FaceNum ++ ;
			}
		}
	}

	// �@���̍Đ����̎w�肪����ꍇ�͐������s��
	if( MV1Man.LoadModelToReMakeNormal )
	{
		// �@���̎�������
		{
			VECTOR Nrm ;
			MV1_MESH_NORMAL *MNrm ;
			MV1_MESH_VERTEX *Vertex[ 3 ] ;
			DWORD VertUnitSize, PosUnitSize ;

			// �S�Ẵt���[��������
			Frame = MBase->Frame ;
			for( i = 0 ; i < MBase->FrameNum ; i ++, Frame ++ )
			{
				if( Frame->AutoCreateNormal == 0 ) continue ;

				PosUnitSize = ( DWORD )Frame->PosUnitSize ;

				// �ʂ̖@�����Z�o���Ȃ��瑫���Ă���
				Mesh = Frame->Mesh ;
				for( j = 0 ; j < Frame->MeshNum ; j ++, Mesh ++ )
				{
					VertUnitSize = ( DWORD )Mesh->VertUnitSize ;

					Face = Mesh->Face ;
					for( k = 0 ; k < Mesh->FaceNum ; k ++, Face ++ )
					{
						Vertex[ 0 ] = ( MV1_MESH_VERTEX * )( ( BYTE * )Mesh->Vertex + VertUnitSize * Face->VertexIndex[ 0 ] ) ;
						Vertex[ 1 ] = ( MV1_MESH_VERTEX * )( ( BYTE * )Mesh->Vertex + VertUnitSize * Face->VertexIndex[ 1 ] ) ;
						Vertex[ 2 ] = ( MV1_MESH_VERTEX * )( ( BYTE * )Mesh->Vertex + VertUnitSize * Face->VertexIndex[ 2 ] ) ;

						Nrm = VNorm( VCross( 
							VSub( ( ( MV1_MESH_POSITION * )( ( BYTE * )Frame->Position + PosUnitSize * Vertex[ 1 ]->PositionIndex ) )->Position, 
								  ( ( MV1_MESH_POSITION * )( ( BYTE * )Frame->Position + PosUnitSize * Vertex[ 0 ]->PositionIndex ) )->Position ),
							VSub( ( ( MV1_MESH_POSITION * )( ( BYTE * )Frame->Position + PosUnitSize * Vertex[ 2 ]->PositionIndex ) )->Position, 
								  ( ( MV1_MESH_POSITION * )( ( BYTE * )Frame->Position + PosUnitSize * Vertex[ 0 ]->PositionIndex ) )->Position ) ) ) ;

						MNrm = &Frame->Normal[ Vertex[ 0 ]->NormalIndex ] ;
						MNrm->Normal = VAdd( MNrm->Normal, Nrm ) ;

						MNrm = &Frame->Normal[ Vertex[ 1 ]->NormalIndex ] ;
						MNrm->Normal = VAdd( MNrm->Normal, Nrm ) ;

						MNrm = &Frame->Normal[ Vertex[ 2 ]->NormalIndex ] ;
						MNrm->Normal = VAdd( MNrm->Normal, Nrm ) ;
					}
				}

				// �@���𐳋K������
				MNrm = Frame->Normal ;
				for( j = 0 ; j < Frame->NormalNum ; j ++, MNrm ++ )
				{
					MNrm->Normal = VNorm( MNrm->Normal ) ;
				}
			}
		}

		// �ڐ��Ə]�@���̍\�z
		Mesh = MBase->Mesh ;
		for( i = 0 ; i < MBase->MeshNum ; i ++, Mesh ++ )
		{
			MV1MakeMeshBinormalsAndTangents( Mesh ) ;
		}
	}

	// �A�j���[�V�����L�[�Z�b�g�̏����Z�b�g
	AnimKeySet = MBase->AnimKeySet ;
	F1AnimKeySet = FHeader->AnimKeySet ;
	for( i = 0 ; i < MBase->AnimKeySetNum ; i ++, AnimKeySet ++, F1AnimKeySet ++ )
	{
		AnimKeySet->Type = F1AnimKeySet->Type ;
		AnimKeySet->DataType = F1AnimKeySet->DataType ;

		AnimKeySet->TargetShapeIndex = F1AnimKeySet->TargetShapeIndex ;

		if( F1AnimKeySet->TimeType == MV1_ANIMKEY_TIME_TYPE_ONE )
		{
			AnimKeySet->TimeType = MV1_ANIMKEY_TIME_TYPE_ONE ;
			AnimKeySet->StartTime = F1AnimKeySet->StartTime ;
			AnimKeySet->UnitTime = F1AnimKeySet->UnitTime ;
		}
		else
		{
			AnimKeySet->TimeType = MV1_ANIMKEY_TIME_TYPE_KEY ;
			AnimKeySet->KeyTime = ( float * )( ( DWORD_PTR )MBase->AnimKeyData + ( ( DWORD_PTR )F1AnimKeySet->KeyTime - ( DWORD_PTR )FHeader->AnimKeyData ) ) ;
		}
		AnimKeySet->Num = F1AnimKeySet->Num ;

		AnimKeySet->KeyLinear = ( float * )( ( DWORD_PTR )MBase->AnimKeyData  + ( ( DWORD_PTR )F1AnimKeySet->KeyLinear - ( DWORD_PTR )FHeader->AnimKeyData ) ) ;
	}

	// �A�j���[�V�����Z�b�g�̏����Z�b�g
	AnimSet = MBase->AnimSet ;
	F1AnimSet = FHeader->AnimSet ;
	for( i = 0 ; i < MBase->AnimSetNum ; i ++, AnimSet ++, F1AnimSet ++ )
	{
#ifndef UNICODE
		AnimSet->NameA = MBase->StringBufferA + ( F1AnimSet->NameA - FHeader->StringBufferA ) ;
#endif
		AnimSet->NameW = MBase->StringBufferW + ( F1AnimSet->NameW - FHeader->StringBufferW ) ;

		AnimSet->Index = F1AnimSet->Index ;
		AnimSet->MaxTime = F1AnimSet->MaxTime ;
		AnimSet->AnimNum = F1AnimSet->AnimNum ;
		if( F1AnimSet->Anim )
		{
			AnimSet->Anim = MBase->Anim + ( F1AnimSet->Anim - FHeader->Anim ) ;
		}
		AnimSet->IsAddAnim = F1AnimSet->IsAddAnim ;
		AnimSet->IsMatrixLinearBlend = F1AnimSet->IsMatrixLinearBlend ;
		AnimSet->IsLoopAnim = F1AnimSet->IsLoopAnim ;
	}

	// �A�j���[�V�����̏����Z�b�g
	Anim = MBase->Anim ;
	F1Anim = FHeader->Anim ;
	for( i = 0 ; i < MBase->AnimNum ; i ++, Anim ++, F1Anim ++ )
	{
		if( F1Anim->Container )
		{
			Anim->Container = MBase->AnimSet + F1Anim->Container->Index ;
		}

		Anim->TargetFrame = MBase->Frame + F1Anim->TargetFrameIndex ;
		Anim->TargetFrameIndex = F1Anim->TargetFrameIndex ;
		Anim->MaxTime = F1Anim->MaxTime ;
		Anim->RotateOrder = F1Anim->RotateOrder ;
		Anim->KeySetNum = F1Anim->KeySetNum ;
		Anim->KeySet = MBase->AnimKeySet + ( F1Anim->KeySet - FHeader->AnimKeySet ) ;

		MBase->AnimTargetFrameTable[ Anim->Container->Index * MBase->FrameNum + Anim->TargetFrameIndex ] = Anim ;
	}

	// �s��̃Z�b�g�A�b�v
	MV1SetupInitializeMatrixBase( MBase ) ;

	// ���b�V���̔��������ǂ����̏����Z�b�g�A�b�v����
	MV1SetupMeshSemiTransStateBase( MBase ) ;

	// ���������`��֌W�̏����Z�b�g�A�b�v����
	if( MBase->UsePackDraw )
	{
		MV1SetupPackDrawInfo( MBase ) ;
	}

	// �������̉��
	//DXFREE( FHeader ) ;

	// �w�肪����ꍇ�͍��W�̍œK�����s��
	if( MV1Man.LoadModelToPositionOptimize )
	{
		MV1PositionOptimizeBase( NewHandle ) ;
	}

	// �w�肪����ꍇ�͖@���̍Čv�Z���s��
	if( MV1Man.LoadModelToReMakeNormal )
	{
		MV1ReMakeNormalBase( NewHandle, MV1Man.LoadModelToReMakeNormalSmoothingAngle ) ;
	}

	// ���������p���_�f�[�^�̍\�z
	TriangleList = MBase->TriangleList ;
	for( i = 0 ; i < MBase->TriangleListNum ; i ++, TriangleList ++ )
	{
		MV1SetupTriangleListPositionAndNormal( TriangleList ) ;
		MV1SetupToonOutLineTriangleList( TriangleList ) ;
	}

	// ����I��
	return NewHandle ;

	// �G���[����
ERRORLABEL :
	if( NewHandle )
	{
		MV1SubModelBase( NewHandle ) ;
		NewHandle = 0 ;
	}

	// �G���[�I��
	return -1 ;
}



// �����Z�b�g�A�b�v�n

// ������Ԃ̕ϊ��s����Z�b�g�A�b�v����
extern void MV1SetupInitializeMatrixBase( MV1_MODEL_BASE *ModelBase )
{
	MV1_FRAME_BASE *MBFrame ;
	MATRIX TempMatrix ;
	int i ;

	MBFrame = ModelBase->Frame ;
	for( i = 0 ; i < ModelBase->FrameNum ; i ++, MBFrame ++ )
	{
		// �X�P�[�����O���g�p���Ă��邩�ǂ����̃t���O���Z�b�g����
		MBFrame->LocalTransformMatrixUseScaling =
			MBFrame->Scale.x < 0.9999999f || MBFrame->Scale.x > 1.0000001f ||
			MBFrame->Scale.y < 0.9999999f || MBFrame->Scale.y > 1.0000001f ||
			MBFrame->Scale.z < 0.9999999f || MBFrame->Scale.z > 1.0000001f;

		// �P�ʍs�񂩂ǂ����𒲂ׂ�
		if( ( *( ( DWORD * )&MBFrame->Rotate.x ) & 0x7fffffff ) == 0 &&
			( *( ( DWORD * )&MBFrame->Rotate.y ) & 0x7fffffff ) == 0 &&
			( *( ( DWORD * )&MBFrame->Rotate.z ) & 0x7fffffff ) == 0 &&
			MBFrame->LocalTransformMatrixUseScaling == false )
		{
			if( ( *( ( DWORD * )&MBFrame->Translate.x ) & 0x7fffffff ) == 0 &&
				( *( ( DWORD * )&MBFrame->Translate.y ) & 0x7fffffff ) == 0 &&
				( *( ( DWORD * )&MBFrame->Translate.z ) & 0x7fffffff ) == 0 )
			{
				MBFrame->LocalTransformMatrixType = 0 ;
				CreateIdentityMatrix( &TempMatrix ) ;
			}
			else
			{
				MBFrame->LocalTransformMatrixType = 1 ;
				CreateTranslationMatrix( &TempMatrix, ( float )MBFrame->Translate.x, ( float )MBFrame->Translate.y, ( float )MBFrame->Translate.z ) ;
			}
		}
		else
		{
			MBFrame->LocalTransformMatrixType = 2 ;

			// �f�t�H���g���W�l�����ɕϊ��s����쐬����
			MV1RMakeMatrix(
				( MBFrame->Flag & MV1_FRAMEFLAG_PREROTATE ) != 0 ? &MBFrame->PreRotate : NULL,
				&MBFrame->Rotate,
				( MBFrame->Flag & MV1_FRAMEFLAG_POSTROTATE ) != 0 ? &MBFrame->PostRotate : NULL,
				&MBFrame->Scale,
				&MBFrame->Translate,
				NULL,
				&TempMatrix,
				MBFrame->RotateOrder
			) ;
		}
		ConvertMatrixFToMatrix4x4cF( &MBFrame->LocalTransformMatrix, &TempMatrix ) ;

		// �����̃��[�J�������[���h�s����쐬
		if( MBFrame->Parent == NULL )
		{
			ConvertMatrix4x4cFToMatrixF( &TempMatrix, &MBFrame->LocalTransformMatrix ) ;
			MBFrame->TransformMatrix = MBFrame->LocalTransformMatrix ;
		}
		else
		{
			MATRIX LocalMatrix, ParentMatrix ;

			ConvertMatrix4x4cFToMatrixF( &LocalMatrix,  &MBFrame->LocalTransformMatrix ) ;
			ConvertMatrix4x4cFToMatrixF( &ParentMatrix, &MBFrame->Parent->TransformMatrix ) ;
			CreateMultiplyMatrix( &TempMatrix, &LocalMatrix, &ParentMatrix ) ;
			ConvertMatrixFToMatrix4x4cF( &MBFrame->TransformMatrix, &TempMatrix ) ;
		}
		CreateInverseMatrix( &TempMatrix, &TempMatrix ) ;
		ConvertMatrixFToMatrix4x4cF( &MBFrame->InverseTransformMatrix, &TempMatrix ) ;
	}
}

// ���������`��֌W�̏����Z�b�g�A�b�v����
extern void MV1SetupPackDrawInfo( MV1_MODEL_BASE *ModelBase )
{
	MV1_TRIANGLE_LIST_BASE *MBTList ;
	int i ;
	int UseBoneNum ;

	// �g���C�A���O�����X�g�������ꍇ�͓K�p�ł��Ȃ�
	if( ModelBase->TriangleListNum == 0 )
	{
		ModelBase->UsePackDraw = FALSE ;
		return ;
	}

	// ���_�V�F�[�_�[���g�p�ł��Ȃ��ꍇ�͓��������`��͓K�p�ł��Ȃ�
	if( GSYS.HardInfo.UseShader == FALSE )
	{
		ModelBase->UsePackDraw = FALSE ;
		return ;
	}

	// �P�g���C�A���O�����X�g�œ����Ɏg�p����{�[�������W��葽�����̂�����ꍇ�͓��������`��͓K�p�ł��Ȃ�
	if( ModelBase->TriangleListSkinPositionFREEBSize > 0 )
	{
		ModelBase->UsePackDraw = FALSE ;
		return ;
	}

	// �V�F�C�v���g�p����Ă��郂�f���ł͓��������`��̓K�p�͂ł��Ȃ�
	if( ModelBase->ShapeNum > 0 )
	{
		ModelBase->UsePackDraw = FALSE ;
		return ;
	}

	// �ŏ��E�ő咸�_���ƍŏ��E�ő�g�p�s�񐔂��擾����
	MBTList = ModelBase->TriangleList ;
	ModelBase->TriangleListMaxVertexNum = 0 ;
	ModelBase->TriangleListMinVertexNum = 0x7fffffff ;
	ModelBase->TriangleListMaxIndexNum = 0 ;
	ModelBase->TriangleListMinIndexNum = 0x7fffffff ;
	ModelBase->TriangleListMaxMatrixNum = 0 ;
	ModelBase->TriangleListMinMatrixNum = 0x7fffffff ;
	for( i = 0 ; i < ModelBase->TriangleListNum ; i ++, MBTList ++ )
	{
		switch( MBTList->VertexType )
		{
		case MV1_VERTEX_TYPE_NORMAL :
			UseBoneNum = 1 ;
			break ;

		case MV1_VERTEX_TYPE_SKIN_FREEBONE :
			continue ;

		default :
			UseBoneNum = MBTList->UseBoneNum ;
			break ;
		}

		if( UseBoneNum > ModelBase->TriangleListMaxMatrixNum )
		{
			ModelBase->TriangleListMaxMatrixNum = UseBoneNum ;
		}

		if( UseBoneNum < ModelBase->TriangleListMinMatrixNum )
		{
			ModelBase->TriangleListMinMatrixNum = UseBoneNum ;
		}

		if( MBTList->IndexNum + MBTList->ToonOutLineIndexNum > ModelBase->TriangleListMaxIndexNum )
		{
			ModelBase->TriangleListMaxIndexNum = MBTList->IndexNum + MBTList->ToonOutLineIndexNum ;
		}

		if( MBTList->IndexNum + MBTList->ToonOutLineIndexNum < ModelBase->TriangleListMinIndexNum )
		{
			ModelBase->TriangleListMinIndexNum = MBTList->IndexNum + MBTList->ToonOutLineIndexNum ;
		}

		if( MBTList->VertexNum > ModelBase->TriangleListMaxVertexNum )
		{
			ModelBase->TriangleListMaxVertexNum = MBTList->VertexNum ;
		}

		if( MBTList->VertexNum < ModelBase->TriangleListMinVertexNum )
		{
			ModelBase->TriangleListMinVertexNum = MBTList->VertexNum ;
		}
	}

	// ���_��C���f�b�N�X����������ꍇ�͓K�p�ł��Ȃ�
	if( ModelBase->TriangleListMaxVertexNum == 0 ||
		ModelBase->TriangleListMaxIndexNum  == 0 ||
		ModelBase->TriangleListMaxMatrixNum == 0 )
	{
		ModelBase->UsePackDraw = FALSE ;
		return ;
	}

	// ���������`��֌W�̏��̃Z�b�g�A�b�v�̊��ˑ�����
	MV1_SetupPackDrawInfo_PF( ModelBase ) ;

	// ���������`��̈�`�敪�Ŏg�p����s��̐����Z�b�g����
	ModelBase->PackDrawMatrixUnitNum = ModelBase->SkinBoneNum + ModelBase->FrameNum ;
}

// �g�D�[���֊s���p�̃��b�V�����쐬����
//#define CHECKPARAM	( 0.125f / 2.0f )
#define CHECKPARAM	( 0.0f )
extern void MV1SetupToonOutLineTriangleList( MV1_TRIANGLE_LIST_BASE *MBTList )
{
	int i, tindNum ;
	unsigned short *ind ;
	unsigned short *tind ;
//	VECTOR FaceDir ;
	MV1_MESH_BASE *MBMesh ;

	// ���_�̐������J��Ԃ�
	ind = MBTList->Index ;
	tind = MBTList->ToonOutLineIndex ;
	tindNum = 0 ;
	MBMesh = MBTList->Container ;
	for( i = 0 ; i < MBTList->IndexNum ; i += 3, ind += 3 )
	{
		if( ( ( MV1_MESH_VERTEX * )( ( BYTE * )MBMesh->Vertex + MBMesh->VertUnitSize * MBTList->MeshVertexIndex[ ind[ 0 ] ] ) )->ToonOutLineScale < 0.0001f ) continue ;
		if( ( ( MV1_MESH_VERTEX * )( ( BYTE * )MBMesh->Vertex + MBMesh->VertUnitSize * MBTList->MeshVertexIndex[ ind[ 1 ] ] ) )->ToonOutLineScale < 0.0001f ) continue ;
		if( ( ( MV1_MESH_VERTEX * )( ( BYTE * )MBMesh->Vertex + MBMesh->VertUnitSize * MBTList->MeshVertexIndex[ ind[ 2 ] ] ) )->ToonOutLineScale < 0.0001f ) continue ;

		tind[ 0 ] = ind[ 0 ] ;
		tind[ 1 ] = ind[ 1 ] ;
		tind[ 2 ] = ind[ 2 ] ;
		tind += 3 ;
		tindNum += 3 ;
	}
/*
	switch( MBTList->VertexType )
	{
	case MV1_VERTEX_TYPE_NORMAL :
		{
			MV1_TLIST_NORMAL_POS *Norm, *Norm1, *Norm2, *Norm3 ;

			Norm = ( MV1_TLIST_NORMAL_POS * )ADDR16( MBTList->NormalPosition ) ;
			for( i = 0 ; i < MBTList->IndexNum ; i += 3, ind += 3 )
			{
				Norm1 = &Norm[ ind[ 0 ] ] ;
				Norm2 = &Norm[ ind[ 1 ] ] ;
				Norm3 = &Norm[ ind[ 2 ] ] ;

				FaceDir =	VNorm
							(
								VCross
								(
									VSub
									(
										*( ( VECTOR * )&Norm2->Position ),
										*( ( VECTOR * )&Norm1->Position )
									),
									VSub
									(
										*( ( VECTOR * )&Norm3->Position ),
										*( ( VECTOR * )&Norm1->Position )
									)
								)
							) ;
				if( VDot( FaceDir, *( ( VECTOR * )&Norm1->Normal ) ) < CHECKPARAM ||
					VDot( FaceDir, *( ( VECTOR * )&Norm2->Normal ) ) < CHECKPARAM ||
					VDot( FaceDir, *( ( VECTOR * )&Norm3->Normal ) ) < CHECKPARAM )
				{
//					continue ;
				}

				tind[ 0 ] = ind[ 0 ] ;
				tind[ 1 ] = ind[ 1 ] ;
				tind[ 2 ] = ind[ 2 ] ;
				tind += 3 ;
				tindNum += 3 ;
			}
		}
		break ;

	case MV1_VERTEX_TYPE_SKIN_4BONE :
		{
			MV1_TLIST_SKIN_POS_4B *SK4, *SK41, *SK42, *SK43 ;

			SK4 = ( MV1_TLIST_SKIN_POS_4B * )ADDR16( MBTList->SkinPosition4B ) ;
			for( i = 0 ; i < MBTList->IndexNum ; i += 3, ind += 3 )
			{
				SK41 = &SK4[ ind[ 0 ] ] ;
				SK42 = &SK4[ ind[ 1 ] ] ;
				SK43 = &SK4[ ind[ 2 ] ] ;

				FaceDir =	VNorm
							(
								VCross
								(
									VSub
									(
										*( ( VECTOR * )&SK42->Position ),
										*( ( VECTOR * )&SK41->Position )
									),
									VSub
									(
										*( ( VECTOR * )&SK43->Position ),
										*( ( VECTOR * )&SK41->Position )
									)
								)
							) ;
				if( VDot( FaceDir, SK41->Normal ) < CHECKPARAM ||
					VDot( FaceDir, SK42->Normal ) < CHECKPARAM ||
					VDot( FaceDir, SK43->Normal ) < CHECKPARAM )
				{
//					continue ;
				}

				tind[ 0 ] = ind[ 0 ] ;
				tind[ 1 ] = ind[ 1 ] ;
				tind[ 2 ] = ind[ 2 ] ;
				tind += 3 ;
				tindNum += 3 ;
			}
		}
		break ;

	case MV1_VERTEX_TYPE_SKIN_8BONE :
		{
			MV1_TLIST_SKIN_POS_8B *SK8, *SK81, *SK82, *SK83 ;

			SK8 = ( MV1_TLIST_SKIN_POS_8B * )ADDR16( MBTList->SkinPosition8B ) ;
			for( i = 0 ; i < MBTList->IndexNum ; i += 3, ind += 3 )
			{
				SK81 = &SK8[ ind[ 0 ] ] ;
				SK82 = &SK8[ ind[ 1 ] ] ;
				SK83 = &SK8[ ind[ 2 ] ] ;

				FaceDir =	VNorm
							(
								VCross
								(
									VSub
									(
										SK82->Position,
										SK81->Position
									),
									VSub
									(
										SK83->Position,
										SK81->Position
									)
								)
							) ;
				if( VDot( FaceDir, SK81->Normal ) < CHECKPARAM ||
					VDot( FaceDir, SK82->Normal ) < CHECKPARAM ||
					VDot( FaceDir, SK83->Normal ) < CHECKPARAM )
				{
//					continue ;
				}

				tind[ 0 ] = ind[ 0 ] ;
				tind[ 1 ] = ind[ 1 ] ;
				tind[ 2 ] = ind[ 2 ] ;
				tind += 3 ;
				tindNum += 3 ;
			}
		}
		break ;

	case MV1_VERTEX_TYPE_SKIN_FREEBONE :
		{
			MV1_TLIST_SKIN_POS_8B *SK8, *SK81, *SK82, *SK83 ;

			SK8 = ( MV1_TLIST_SKIN_POS_8B * )ADDR16( MBTList->SkinPosition8B ) ;
			for( i = 0 ; i < MBTList->IndexNum ; i += 3, ind += 3 )
			{
				SK81 = ( MV1_TLIST_SKIN_POS_8B * )( ( BYTE * )SK8 + MBTList->PosUnitSize * ind[ 0 ] ) ;
				SK82 = ( MV1_TLIST_SKIN_POS_8B * )( ( BYTE * )SK8 + MBTList->PosUnitSize * ind[ 1 ] ) ;
				SK83 = ( MV1_TLIST_SKIN_POS_8B * )( ( BYTE * )SK8 + MBTList->PosUnitSize * ind[ 2 ] ) ;

				FaceDir =	VNorm
							(
								VCross
								(
									VSub
									(
										*( ( VECTOR * )&SK82->Position ),
										*( ( VECTOR * )&SK81->Position )
									),
									VSub
									(
										*( ( VECTOR * )&SK83->Position ),
										*( ( VECTOR * )&SK81->Position )
									)
								)
							) ;
				if( VDot( FaceDir, *( ( VECTOR * )&SK81->Normal ) ) < CHECKPARAM ||
					VDot( FaceDir, *( ( VECTOR * )&SK82->Normal ) ) < CHECKPARAM ||
					VDot( FaceDir, *( ( VECTOR * )&SK83->Normal ) ) < CHECKPARAM )
				{
//					continue ;
				}

				tind[ 0 ] = ind[ 0 ] ;
				tind[ 1 ] = ind[ 1 ] ;
				tind[ 2 ] = ind[ 2 ] ;
				tind += 3 ;
				tindNum += 3 ;
			}
		}
		break ;
	}
*/
	MBTList->ToonOutLineIndexNum = ( unsigned short )tindNum ;

	return ;
}

// ���f�����̃��b�V���̔������v�f�����邩�ǂ����𒲂ׂ�
extern void MV1SetupMeshSemiTransStateBase( MV1_MODEL_BASE *ModelBase )
{
	MV1_MESH_BASE     *MBMesh ;
	MV1_MATERIAL_BASE *MBMaterial ;
	MV1_TEXTURE_BASE  *MBTexture ;
	int               i ;

	// ���b�V���̐������J��Ԃ�
	MBMesh = ModelBase->Mesh ;
	for( i = 0 ; i < ModelBase->MeshNum ; i ++, MBMesh ++ )
	{
		do
		{
			MBMesh->SemiTransState = 1 ;

			MBMaterial = MBMesh->Material ;

			// �`�惂�[�h���A���t�@�u�����h�ȊO�Ȃ甼����
			if( MBMaterial->DrawBlendMode != DX_BLENDMODE_ALPHA )
				break ;

			// �u�����h�p�����[�^�� 255 �ȊO�Ȃ甼����
			if( MBMaterial->DrawBlendParam != 255 )
				break ;

			// ���_�J���[���g�p���邩�ǂ����ŕ���
			if( MBMesh->UseVertexDiffuseColor )
			{
				// ���_�f�B�t���[�Y�J���[�� 1.0 �ȊO�̃A���t�@�l���������甼����
				if( MBMesh->NotOneDiffuseAlpha )
					break ;
			}
			else
			{
				// �}�e���A���̃f�B�t���[�Y�J���[�̃A���t�@�l���P�D�O�ȊO�Ȃ甼����
				if( MBMaterial->Diffuse.a != 1.0f )
					break ;
			}

			// �������e�N�X�`�����g�p���Ă����甼����
			if( MBMaterial->DiffuseLayerNum )
			{
				MBTexture = &ModelBase->Texture[ MBMaterial->DiffuseLayer[ 0 ].Texture ] ;
				if( MBTexture->SemiTransFlag )
				{
					break ;
				}
			}

			// �����ɂ����甼�����v�f�͂Ȃ��Ƃ�������
			MBMesh->SemiTransState = 0 ;
		}while( 0 ) ;
	}
}

// ���f���S�̖̂@�����Čv�Z����
extern int MV1ReMakeNormalBase( int MBHandle, float SmoothingAngle, int ASyncThread )
{
	MV1_MODEL_BASE *ModelBase ;
	MV1_FRAME_BASE *Frame ;
	int i ;

	// ����������Ă��Ȃ�������G���[
	if( MV1Man.Initialize == false ) return -1 ;

	// �A�h���X�擾
	if( MV1BMDLCHK( MBHandle, ModelBase ) )
		return -1 ;

	// ���_�o�b�t�@���������
	MV1_TerminateVertexBufferBase_PF( MBHandle ) ;

	// �t���[���̐������J��Ԃ�
	Frame = ModelBase->Frame ;
	for( i = 0 ; i < ModelBase->FrameNum ; i ++, Frame ++ )
	{
		if( Frame->MeshNum == 0 ) continue ;

		if( _MV1ReMakeNormalFrameBase( Frame, SmoothingAngle ) == -1 )
			return -1 ;
	}

	// ���_�o�b�t�@���\�z����
	MV1_SetupVertexBufferBase_PF( MBHandle, 1, ASyncThread ) ;

	// �I��
	return 0 ;
}

// �w��t���[���������b�V���̖@�����Čv�Z����
extern int MV1ReMakeNormalFrameBase( int MBHandle, int FrameIndex, float SmoothingAngle )
{
	MV1_MODEL_BASE *ModelBase ;
	MV1_FRAME_BASE *Frame ;

	// ����������Ă��Ȃ�������G���[
	if( MV1Man.Initialize == false ) return -1 ;

	// �A�h���X�擾
	if( MV1BMDLCHK( MBHandle, ModelBase ) )
		return -1 ;

	// �t���[���C���f�b�N�X�̃`�F�b�N
	if( FrameIndex < 0 || FrameIndex >= ModelBase->FrameNum )
		return -1 ;
	Frame = ModelBase->Frame + FrameIndex ;

	// ���b�V����������Ή��������I��
	if( Frame->MeshNum == 0 )
		return -1 ;

	// ���_�o�b�t�@���������
	MV1_TerminateVertexBufferBase_PF( MBHandle ) ;

	// �t���[���̍Čv�Z
	if( _MV1ReMakeNormalFrameBase( Frame, SmoothingAngle ) == -1 )
		return -1 ;

	// ���_�o�b�t�@���\�z����
	MV1_SetupVertexBufferBase_PF( MBHandle ) ;

	// �I��
	return 0 ;
}

// �t���[���̖@�����Čv�Z����
static int _MV1ReMakeNormalFrameBase( MV1_FRAME_BASE *Frame, float SmoothingAngle )
{
	MV1_MODEL_BASE *ModelBase ;
	int FaceNum, NewVertexNum, NewTVertexNum, i, j, k, m ;
	MV1_TRIANGLE_LIST_BASE *TList ;
	MV1_MESH_BASE *Mesh, *TMesh, *CurMesh, *MinMesh ;
	MV1_MESH_VERTEX *NewVertex, *Vert ;
	MV1_MESH_FACE *Face, *FaceT, *OldFace = NULL, *OFace ;
	VECTOR *FaceNormal, *FNorm, Pos[ 3 ], TPos[ 3 ] ;
	int *FaceNormalIndex, *FNInd, *FNInd2, *MFNInd = NULL, Index, *VertValidBuffer ;
	int NormalNum, IndexNum, PosInd[ 3 ], TPosInd[ 3 ], MPosInd[ 3 ] ;
	VECTOR *P0, *P1, *P2, V1, V2, V3, Norm, tv, xv, yv, zv, FaceNorm ;
	VECTOR *NormalBuf = NULL ;
	MV1_MESH_NORMAL *RNormal ;
	BYTE *NormalSetFlag, *NormalUseTable ;
	DWORD *UseNormalIndex, *NewNormalIndex, *NewMeshVertexIndex ;
	MV1_MAKEVERTINDEXINFO **VertexFaceList = NULL, *VertexFaceBuffer, *VFBuf ;
	MV1_MESH_FACE *FaceList[ 1024 ], *CurFace, *MinFace, *BackCurFace ;
	float SmoothCos, Sin ;
	float MinFaceRadi, MinFaceRadi2, FaceRadi, FaceRadi2 ;
	int FaceCount, MinFaceRadi2Use, FaceRadi2Use, CurFaceIndex, Issyuu ;
	BYTE FaceIndex[ 1024 ] ;
	int UseNormalNum, MinFaceUra, FaceUra, CmpFlag, MinFaceIndex, VertInfoNum, NewShapeVertexNum ;
	MV1_MAKEVERTINDEXINFO **VertInfoTable, *VInfo ;
	MV1_MAKEVERTINDEXINFO *VertInfoBuffer ;
	MV1_SHAPE_VERTEX_BASE *ShapeFrameVertBuffer, *ShapeFrameVert, *ShapeVert, *NewShapeVertBuffer ;
	MV1_SHAPE_MESH_BASE *ShapeMesh ;
	MV1_SHAPE_BASE *Shape ;

	// ����������Ă��Ȃ�������G���[
	if( MV1Man.Initialize == false ) return -1 ;

	// ���b�V����������Ή��������ɏI��
	if( Frame->MeshNum == 0 ) return 0 ;

	// �|�C���^�̏�����
	ModelBase = Frame->Container ;
	FaceNormal = NULL ;
	FaceNormalIndex = NULL ;
	NewVertex = NULL ;
	VertInfoTable = NULL ;
	NewMeshVertexIndex = NULL ;
	NewShapeVertBuffer = NULL ;

	// �X���[�W���O���s���p�x�̃R�T�C���l�����߂Ă���
	Frame->SmoothingAngle = SmoothingAngle ;
	_SINCOS_PLATFORM( Frame->SmoothingAngle, &Sin, &SmoothCos ) ;
	Frame->AutoCreateNormal = TRUE ;

	// �ʂ̐��𐔂���
	Mesh = Frame->Mesh ;
	FaceNum = 0 ;
	for( i = 0 ; i < Frame->MeshNum ; i ++, Mesh ++ )
		FaceNum += Mesh->FaceNum ;

	// �ʂ̖@���Ɩʂ��g�p����@���̃C���f�b�N�X�ƁA�V�F�C�v���_�̃t���[�����_�Ƃ�
	// �֘A�f�[�^��ۑ����郁�����̈�̊m�ۂ���
	FaceNormal = ( VECTOR * )DXALLOC(
		sizeof( VECTOR ) * FaceNum +
		sizeof( int ) * 3 * FaceNum +
		sizeof( MV1_SHAPE_VERTEX_BASE ) * Frame->PositionNum * Frame->ShapeNum ) ;
	if( FaceNormal == NULL )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\xd5\x6c\xda\x7d\x8d\x51\x08\x8a\x97\x7b\x28\x75\x6e\x30\xe1\x30\xe2\x30\xea\x30\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�@���Čv�Z�p�̃������̊m�ۂɎ��s���܂���\n" @*/ )) ;
		goto ERR ;
	}
	FaceNormalIndex = ( int * )( FaceNormal + FaceNum ) ;
	ShapeFrameVertBuffer = ( MV1_SHAPE_VERTEX_BASE * )( FaceNormalIndex + 3 * FaceNum ) ;

	// �V�F�C�v���ƃt���[�����_�̊֘A�f�[�^���\�z����
	if( Frame->ShapeNum )
	{
		_MEMSET( ShapeFrameVertBuffer, 0xff, sizeof( MV1_SHAPE_VERTEX_BASE ) * Frame->PositionNum * Frame->ShapeNum ) ;
		Shape = Frame->Shape ;
		for( i = 0 ; i < Frame->ShapeNum ; i ++, Shape ++ )
		{
			ShapeFrameVert = ShapeFrameVertBuffer + i * Frame->PositionNum ;
			ShapeMesh = Shape->Mesh ;
			for( j = 0 ; j < Shape->MeshNum ; j ++, ShapeMesh ++ )
			{
				Mesh = ShapeMesh->TargetMesh ;
				ShapeVert = ShapeMesh->Vertex ;
				for( k = 0 ; ( DWORD )k < ShapeMesh->VertexNum ; k ++, ShapeVert ++ )
				{
					Vert = ( MV1_MESH_VERTEX * )( ( BYTE * )Mesh->Vertex + ShapeVert->TargetMeshVertex * Mesh->VertUnitSize ) ;
					ShapeFrameVert[ Vert->PositionIndex ] = *ShapeVert ;
				}
			}
		}
	}

	// �ʏ�񂩂��@�����K�v�����ׂ�
	NormalNum = 0 ;
	FaceNum = 0 ;
	FNInd = FaceNormalIndex ;
	Mesh = Frame->Mesh ;
	for( i = 0 ; i < Frame->MeshNum ; i ++, Mesh ++ )
	{
		Face = Mesh->Face ;
		for( j = 0 ; j < Mesh->FaceNum ; j ++, Face ++, FaceNum ++ )
		{
			// ���łɖʂ̖@�����v�Z����
			P0 = &( ( MV1_MESH_POSITION * )( ( BYTE * )Frame->Position + Frame->PosUnitSize * ( ( MV1_MESH_VERTEX * )( ( BYTE * )Mesh->Vertex + Mesh->VertUnitSize * Face->VertexIndex[ 0 ] ) )->PositionIndex ) )->Position ;
			P1 = &( ( MV1_MESH_POSITION * )( ( BYTE * )Frame->Position + Frame->PosUnitSize * ( ( MV1_MESH_VERTEX * )( ( BYTE * )Mesh->Vertex + Mesh->VertUnitSize * Face->VertexIndex[ 1 ] ) )->PositionIndex ) )->Position ;
			P2 = &( ( MV1_MESH_POSITION * )( ( BYTE * )Frame->Position + Frame->PosUnitSize * ( ( MV1_MESH_VERTEX * )( ( BYTE * )Mesh->Vertex + Mesh->VertUnitSize * Face->VertexIndex[ 2 ] ) )->PositionIndex ) )->Position ;

			VectorSub( &V1, P1, P0 ) ;
			VectorSub( &V2, P2, P0 ) ;
			VectorOuterProduct( &Norm, &V1, &V2 ) ;
			VectorNormalize( &FaceNormal[ FaceNum ], &Norm ) ;

			// �@���C���f�b�N�X���Z�b�g
			FNInd[ 0 ] = NormalNum ;
			FNInd[ 1 ] = NormalNum + 1 ;
			FNInd[ 2 ] = NormalNum + 2 ;

			// �@���̐����C���f�b�N�X�̐��������₷
			FNInd += 3 ;
			NormalNum += 3 ;
		}
	}
	IndexNum = NormalNum ;

	// �@�����i�[���郁�������m�ۂ���
	NormalBuf = ( VECTOR * )DXALLOC( ( sizeof( VECTOR ) + sizeof( BYTE ) + sizeof( DWORD ) + sizeof( DWORD ) ) * NormalNum ) ;
	if( NormalBuf == NULL )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\xd5\x6c\xda\x7d\x8d\x51\x08\x8a\x97\x7b\x28\x75\x6e\x30\xe1\x30\xe2\x30\xea\x30\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x5f\x00\x32\x00\x0a\x00\x00"/*@ L"�@���Čv�Z�p�̃������̊m�ۂɎ��s���܂���_2\n" @*/ )) ;
		goto ERR ;
	}
	_MEMSET( NormalBuf, 0, ( sizeof( VECTOR ) + sizeof( BYTE ) + sizeof( DWORD ) + sizeof( DWORD ) ) * NormalNum ) ;
	NormalSetFlag = ( BYTE * )( NormalBuf + NormalNum ) ;
	NormalUseTable = ( BYTE * )( NormalBuf + NormalNum ) ;
	UseNormalIndex = ( DWORD * )( NormalUseTable + NormalNum ) ;
	NewNormalIndex = UseNormalIndex + NormalNum ;

	// �e���_�Ɋ֌W����ʂ̃��X�g���쐬����
	{
		VertexFaceList = ( MV1_MAKEVERTINDEXINFO ** )DXALLOC( sizeof( MV1_MAKEVERTINDEXINFO * ) * Frame->PositionNum + sizeof( MV1_MAKEVERTINDEXINFO ) * IndexNum ) ;
		if( VertexFaceList == NULL )
		{
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\xd5\x6c\xda\x7d\x8d\x51\x08\x8a\x97\x7b\x28\x75\x6e\x30\xe1\x30\xe2\x30\xea\x30\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x5f\x00\x33\x00\x0a\x00\x00"/*@ L"�@���Čv�Z�p�̃������̊m�ۂɎ��s���܂���_3\n" @*/ )) ;
			goto ERR ;
		}
		_MEMSET( VertexFaceList, 0, sizeof( MV1_MAKEVERTINDEXINFO * ) * Frame->PositionNum ) ;
		VertexFaceBuffer = ( MV1_MAKEVERTINDEXINFO * )( VertexFaceList + Frame->PositionNum ) ;

		// �S�Ă̖ʂ̎Q�Ƃ��Ă��钸�_�̏����Z�b�g����
		Mesh = Frame->Mesh ;
		VFBuf = VertexFaceBuffer ;
		for( i = 0 ; i < Frame->MeshNum ; i ++, Mesh ++ )
		{
			Face = Mesh->Face ;
			for( j = 0 ; j < Mesh->FaceNum ; j ++, Face ++ )
			{
				PosInd[ 0 ] = ( int )( ( MV1_MESH_VERTEX * )( ( BYTE * )Mesh->Vertex + Mesh->VertUnitSize * Face->VertexIndex[ 0 ] ) )->PositionIndex ;
				PosInd[ 1 ] = ( int )( ( MV1_MESH_VERTEX * )( ( BYTE * )Mesh->Vertex + Mesh->VertUnitSize * Face->VertexIndex[ 1 ] ) )->PositionIndex ;
				PosInd[ 2 ] = ( int )( ( MV1_MESH_VERTEX * )( ( BYTE * )Mesh->Vertex + Mesh->VertUnitSize * Face->VertexIndex[ 2 ] ) )->PositionIndex ;

				VFBuf->Face = Face ;
				VFBuf->Mesh = Mesh ;
				VFBuf->Next = VertexFaceList[ PosInd[ 0 ] ] ;
				VertexFaceList[ PosInd[ 0 ] ] = VFBuf ;
				VFBuf ++ ;

				VFBuf->Face = Face ;
				VFBuf->Mesh = Mesh ;
				VFBuf->Next = VertexFaceList[ PosInd[ 1 ] ] ;
				VertexFaceList[ PosInd[ 1 ] ] = VFBuf ;
				VFBuf ++ ;

				VFBuf->Face = Face ;
				VFBuf->Mesh = Mesh ;
				VFBuf->Next = VertexFaceList[ PosInd[ 2 ] ] ;
				VertexFaceList[ PosInd[ 2 ] ] = VFBuf ;
				VFBuf ++ ;
			}
		}
	}

	// �ʂ̖@�����\�z����
	FNorm = FaceNormal ;
	FNInd = FaceNormalIndex ;
	Mesh = Frame->Mesh ;
	for( i = 0 ; i < Frame->MeshNum ; i ++, Mesh ++ )
	{
		Face = Mesh->Face ;
		for( j = 0 ; j < Mesh->FaceNum ; j ++, Face ++, FNInd += 3, FNorm ++ )
		{
			// ���ɏ����ς݂̏ꍇ�͉������Ȃ�
			if( NormalSetFlag[ FNInd[ 0 ] ] &&
				NormalSetFlag[ FNInd[ 1 ] ] &&
				NormalSetFlag[ FNInd[ 2 ] ] )
				continue ;

			// ���_�̐������J��Ԃ�
			for( k = 0 ; k < 3 ; k ++ )
			{
				// ���Ɍv�Z�ς݂̏ꍇ�͉������Ȃ�
				if( NormalSetFlag[ FNInd[ k ] ] ) continue ;

				// �A���ʂ����X�g�A�b�v
				{
					FaceList[ 0 ] = Face ;
					FaceNorm = *FNorm ;
					FaceIndex[ 0 ] = ( BYTE )k ;
					NormalSetFlag[ FNInd[ k ] ] |= 2 ;
					FaceCount = 1 ;

					// �Е��̘A���ʂ����X�g�A�b�v
					CurFace = Face ;
					CurMesh = Mesh ;
					BackCurFace = NULL ;
					CurFaceIndex = k ;
					Issyuu = 0 ;
					for(;;)
					{
						MinFace = NULL ;
						MinMesh = NULL ;
						MPosInd[ 0 ] = -1 ;
						MPosInd[ 1 ] = -1 ;
						MPosInd[ 2 ] = -1 ;

						// ���W�C���f�b�N�X���Z�b�g���Ă���
						PosInd[ 0 ] = ( int )( ( MV1_MESH_VERTEX * )( ( BYTE * )CurMesh->Vertex + CurMesh->VertUnitSize * CurFace->VertexIndex[ 0 ] ) )->PositionIndex ;
						PosInd[ 1 ] = ( int )( ( MV1_MESH_VERTEX * )( ( BYTE * )CurMesh->Vertex + CurMesh->VertUnitSize * CurFace->VertexIndex[ 1 ] ) )->PositionIndex ;
						PosInd[ 2 ] = ( int )( ( MV1_MESH_VERTEX * )( ( BYTE * )CurMesh->Vertex + CurMesh->VertUnitSize * CurFace->VertexIndex[ 2 ] ) )->PositionIndex ;

						// ���W���Z�b�g���Ă���
						Pos[ 0 ] = ( ( MV1_MESH_POSITION * )( ( BYTE * )Frame->Position + Frame->PosUnitSize * PosInd[ 0 ] ) )->Position ;
						Pos[ 1 ] = ( ( MV1_MESH_POSITION * )( ( BYTE * )Frame->Position + Frame->PosUnitSize * PosInd[ 1 ] ) )->Position ;
						Pos[ 2 ] = ( ( MV1_MESH_POSITION * )( ( BYTE * )Frame->Position + Frame->PosUnitSize * PosInd[ 2 ] ) )->Position ;

						// �@��������o���������_����אڂ��Ă��邩���ׂ����ӂ̒��_�֌������x�N�g�����y�A�����Е��̒��_�֌������x�N�g�����w�Ƃ����O���̍쐬
						VectorSub( &zv, &Pos[ ( CurFaceIndex + 1 ) % 3 ], &Pos[ CurFaceIndex % 3 ] ) ;
						VectorSub( &xv, &Pos[ ( CurFaceIndex + 2 ) % 3 ], &Pos[ CurFaceIndex % 3 ] ) ;
						VectorOuterProduct( &yv, &zv, &xv ) ;
						VectorOuterProduct( &xv, &yv, &zv ) ;
						VectorNormalize( &xv, &xv ) ;
						VectorNormalize( &yv, &yv ) ;
						VectorNormalize( &zv, &zv ) ;

						// �אڂ��Ă��邩���ׂ����ӂ̒��_�Ƃ͕ʂ̂�����̒��_�֌������x�N�g������L�ŎZ�o�����O����ɓ��e�����x�N�g�����Z�o
						VectorSub( &tv, &Pos[ ( CurFaceIndex + 2 ) % 3 ], &Pos[ CurFaceIndex % 3 ] ) ;
						V1.x = xv.x * tv.x + xv.y * tv.y + xv.z * tv.z ;
						V1.y = yv.x * tv.x + yv.y * tv.y + yv.z * tv.z ;
						V1.z = zv.x * tv.x + zv.y * tv.y + zv.z * tv.z ;
						V1.z = 0.0f ;
						VectorNormalize( &V1, &V1 ) ;

						// ����̕ӂ����|���S���̌���
						for( VFBuf = VertexFaceList[ PosInd[ CurFaceIndex % 3 ] ] ; VFBuf ; VFBuf = VFBuf->Next )
						{
							FaceT = ( MV1_MESH_FACE * )VFBuf->Face ;
							TMesh = ( MV1_MESH_BASE * )VFBuf->Mesh ;

							// ���Ɍ��o�ς݂̃|���S���͖���
							FNInd2 = FaceNormalIndex + ( FaceT - Frame->Mesh->Face ) * 3 ;
							if( ( NormalSetFlag[ FNInd2[ 0 ] ] & 2 ) ||
								( NormalSetFlag[ FNInd2[ 1 ] ] & 2 ) ||
								( NormalSetFlag[ FNInd2[ 2 ] ] & 2 ) ) continue ;

							// �������g���A��O�Ɏ����������|���S���͖���
							if( FaceT == CurFace || FaceT == BackCurFace ) continue ;

							// ���W�C���f�b�N�X���Z�b�g���Ă���
							TPosInd[ 0 ] = ( int )( ( MV1_MESH_VERTEX * )( ( BYTE * )TMesh->Vertex + TMesh->VertUnitSize * FaceT->VertexIndex[ 0 ] ) )->PositionIndex ;
							TPosInd[ 1 ] = ( int )( ( MV1_MESH_VERTEX * )( ( BYTE * )TMesh->Vertex + TMesh->VertUnitSize * FaceT->VertexIndex[ 1 ] ) )->PositionIndex ;
							TPosInd[ 2 ] = ( int )( ( MV1_MESH_VERTEX * )( ( BYTE * )TMesh->Vertex + TMesh->VertUnitSize * FaceT->VertexIndex[ 2 ] ) )->PositionIndex ;

							// ���W���Z�b�g���Ă���
							TPos[ 0 ] = ( ( MV1_MESH_POSITION * )( ( BYTE * )Frame->Position + Frame->PosUnitSize * TPosInd[ 0 ] ) )->Position ;
							TPos[ 1 ] = ( ( MV1_MESH_POSITION * )( ( BYTE * )Frame->Position + Frame->PosUnitSize * TPosInd[ 1 ] ) )->Position ;
							TPos[ 2 ] = ( ( MV1_MESH_POSITION * )( ( BYTE * )Frame->Position + Frame->PosUnitSize * TPosInd[ 2 ] ) )->Position ;

							// ���_�̐������J��Ԃ�
							for( m = 0 ; m < 3 ; m ++ )
							{
								CmpFlag = 0 ;

								// �����ӂ��������ׂ�A���_�w��̏��Ԃ���ʂ̕����𒲂ׂ�
								if( PosInd[ CurFaceIndex         % 3 ] == TPosInd[ m         % 3 ] &&
									PosInd[ ( CurFaceIndex + 1 ) % 3 ] == TPosInd[ ( m + 1 ) % 3 ] )
								{
									CmpFlag = 1 ;
									FaceUra = 1 ;
								}

								if( PosInd[ CurFaceIndex         % 3 ] == TPosInd[ ( m + 1 ) % 3 ] &&
									PosInd[ ( CurFaceIndex + 1 ) % 3 ] == TPosInd[ m         % 3 ] )
								{
									CmpFlag = 1 ;
									FaceUra = 0 ;
								}

								// �J�����g�ʂƓ���̒��_���g�p����P�W�O�x��]������( �J�����g�ʂ̗��� )�������猟�o�������Ƃ𖳂��������Ƃɂ���
								if( CmpFlag == 1 && PosInd[ ( CurFaceIndex + 2 ) % 3 ] == TPosInd[ ( m + 2 ) % 3 ] )
								{
									CmpFlag = 0 ;
								}

								// ����̕ӂ����������烋�[�v�𔲂���
								if( CmpFlag != 0 ) break ;
							}

							// ����̕ӂ����������珈��
							if( m != 3 )
							{
								// �������|���S���́A����̕ӂ𐬂����_�Ƃ͕ʂ̂�����̒��_�֌������x�N�g�����ɎZ�o�����O���֓��e�����x�N�g�����Z�o����
								VectorSub( &tv, &TPos[ ( m + 2 ) % 3 ], &Pos[ CurFaceIndex % 3 ] ) ;
								V2.x = xv.x * tv.x + xv.y * tv.y + xv.z * tv.z ;
								V2.y = yv.x * tv.x + yv.y * tv.y + yv.z * tv.z ;
								V2.z = zv.x * tv.x + zv.y * tv.y + zv.z * tv.z ;
								V2.z = 0.0f ;

								// ��̖ʂ�^�����猩���Ƃ��ɂł�����������p�̃R�T�C���l�𒲂ׂ�
								// ( �J�����g�ʂ̕\���������ɉ�]�����Ƃ��ɓ�ӂ������p�̊p�x�𒲂ׂ� )
								{
									// �O�ς��g���ĂP�W�O�x�ȏォ�ǂ����𔻒�
									VectorOuterProduct( &tv, &V1, &V2 ) ;
									VectorNormalize( &V2, &V2 ) ;
									if( tv.z < 0.0f )
									{
										// �P�W�O�x�ȏゾ�����ꍇ�͂P�W�O�x�ȏゾ�������Ƃ��L�^�������
										// �J�����g�ʑ��̃x�N�g�����t�]���ē��ς����߂�( �R�T�C���l�����߂� )
										FaceRadi = -1.0f ;
										FaceRadi2Use = 1 ;
										V3.x = -V1.x ;
										V3.y = -V1.y ;
										V3.z = -V1.z ;
										FaceRadi2 = VectorInnerProduct( &V3, &V2 ) ;
									}
									else
									{
										// �P�W�O�x�ȉ��������ꍇ�͂��̂܂ܓ�̐��̓��ς����߂�( �R�T�C���l�����߂� )
										FaceRadi = VectorInnerProduct( &V1, &V2 ) ;
										FaceRadi2Use = 0 ;
										FaceRadi2 = 1.0f ;
									}
								}

								// �܂��אڂ���ʂ������Ă��Ȃ���
								// ���Ɍ����Ă��邪����̖ʂ̗��ʂ�
								// ���Ɍ����Ă���אږʂ����p�x�I�ɋ߂��ʂ�������אږʂƂ��ċL�^����
								if( MinFace == NULL ||
									( ( ( TPosInd[ 0 ] == MPosInd[ 0 ] &&
										  TPosInd[ 1 ] == MPosInd[ 2 ] &&
										  TPosInd[ 2 ] == MPosInd[ 1 ] ) ||
										( TPosInd[ 0 ] == MPosInd[ 1 ] &&
										  TPosInd[ 1 ] == MPosInd[ 0 ] &&
										  TPosInd[ 2 ] == MPosInd[ 2 ] ) ||
										( TPosInd[ 0 ] == MPosInd[ 2 ] &&
										  TPosInd[ 1 ] == MPosInd[ 1 ] &&
										  TPosInd[ 2 ] == MPosInd[ 0 ] ) ) && FaceUra == 0 && MinFaceUra == 1 ) ||
									( FaceRadi2Use == 0 && MinFaceRadi2Use == 1 ) ||
									( FaceRadi2Use == 1 && MinFaceRadi2Use == 1 && FaceRadi2 > MinFaceRadi2 ) ||
									( FaceRadi2Use == 0 && MinFaceRadi2Use == 0 && FaceRadi  > MinFaceRadi ) )
								{
									MPosInd[ 0 ] = TPosInd[ 0 ] ;
									MPosInd[ 1 ] = TPosInd[ 1 ] ;
									MPosInd[ 2 ] = TPosInd[ 2 ] ;
									MFNInd = FNInd2 ;
									MinMesh = TMesh ;
									MinFace = FaceT ;
									MinFaceRadi = FaceRadi ;
									MinFaceRadi2Use = FaceRadi2Use ;
									MinFaceRadi2 = FaceRadi2 ;
									MinFaceUra = FaceUra ;
									MinFaceIndex = m + 1 ;
									if( MinFaceIndex == 3 ) MinFaceIndex = 0 ;
								}
							}
						}

						// �אږʂ𔭌��ł��Ȃ��������A�אږʂ����ʂ��������A�אږʂƂ̖@���̊p�x�����X���[�W���O�Ώۂ�臒l�𒴂��Ă�����G�b�W�Ƃ݂Ȃ�
						if( MinFace == NULL || MinFaceUra || VectorInnerProduct( FaceNormal + ( MinFace - Frame->Mesh->Face ), FaceNormal + ( CurFace - Frame->Mesh->Face ) ) < SmoothCos )
							break ;

						// ���������אږʂ������J�n�̖ʂ������ꍇ�͈�������Ƃ�������
						if( MinFace == FaceList[ 0 ] )
						{
							Issyuu = 1 ;
							break ;
						}

						// �m�肵���אږʂ��L�^
						FaceList[ FaceCount ] = MinFace ;
						FaceIndex[ FaceCount ] = ( BYTE )MinFaceIndex ;
						VectorAdd( &FaceNorm, &FaceNorm, FaceNormal + ( MinFace - Frame->Mesh->Face ) ) ;
						NormalSetFlag[ MFNInd[ MinFaceIndex ] ] |= 2 ;
						FaceCount ++ ;

						// �J�����g�ʂ̕ύX
						BackCurFace = CurFace ;
						CurMesh = MinMesh ;
						CurFace = MinFace ;
						CurFaceIndex = MinFaceIndex ;
					}

					// �����Е��̘A���ʂ����X�g�A�b�v
					if( Issyuu == 0 )
					{
						BackCurFace = NULL ;
						CurMesh = Mesh ;
						CurFace = Face ;
						CurFaceIndex = k ;
						for(;;)
						{
							MinFace = NULL ;
							MinMesh = NULL ;
							MPosInd[ 0 ] = -1 ;
							MPosInd[ 1 ] = -1 ;
							MPosInd[ 2 ] = -1 ;

							// ���W�C���f�b�N�X���Z�b�g���Ă���
							PosInd[ 0 ] = ( int )( ( MV1_MESH_VERTEX * )( ( BYTE * )CurMesh->Vertex + CurMesh->VertUnitSize * CurFace->VertexIndex[ 0 ] ) )->PositionIndex ;
							PosInd[ 1 ] = ( int )( ( MV1_MESH_VERTEX * )( ( BYTE * )CurMesh->Vertex + CurMesh->VertUnitSize * CurFace->VertexIndex[ 1 ] ) )->PositionIndex ;
							PosInd[ 2 ] = ( int )( ( MV1_MESH_VERTEX * )( ( BYTE * )CurMesh->Vertex + CurMesh->VertUnitSize * CurFace->VertexIndex[ 2 ] ) )->PositionIndex ;

							// ���W���Z�b�g���Ă���
							Pos[ 0 ] = ( ( MV1_MESH_POSITION * )( ( BYTE * )Frame->Position + Frame->PosUnitSize * PosInd[ 0 ] ) )->Position ;
							Pos[ 1 ] = ( ( MV1_MESH_POSITION * )( ( BYTE * )Frame->Position + Frame->PosUnitSize * PosInd[ 1 ] ) )->Position ;
							Pos[ 2 ] = ( ( MV1_MESH_POSITION * )( ( BYTE * )Frame->Position + Frame->PosUnitSize * PosInd[ 2 ] ) )->Position ;

							// �@��������o���������_����אڂ��Ă��邩���ׂ����ӂ̒��_�֌������x�N�g�����y�A�����Е��̒��_�֌������x�N�g�����w�Ƃ����R���x�N�g���̍쐬
							VectorSub( &zv, &Pos[ ( CurFaceIndex + 3 - 1 ) % 3 ], &Pos[ CurFaceIndex % 3 ] ) ;
							VectorSub( &xv, &Pos[ ( CurFaceIndex + 3 - 2 ) % 3 ], &Pos[ CurFaceIndex % 3 ] ) ;
							VectorOuterProduct( &yv, &zv, &xv ) ;
							VectorOuterProduct( &xv, &yv, &zv ) ;
							VectorNormalize( &xv, &xv ) ;
							VectorNormalize( &yv, &yv ) ;
							VectorNormalize( &zv, &zv ) ;

							// �אڂ��Ă��邩���ׂ����ӂ̒��_�Ƃ͕ʂ̂�����̒��_�֌������x�N�g������L�ŎZ�o�����O����ɓ��e�����x�N�g�����Z�o
							VectorSub( &tv, &Pos[ ( CurFaceIndex + 3 - 2 ) % 3 ], &Pos[ CurFaceIndex % 3 ] ) ;
							V1.x = xv.x * tv.x + xv.y * tv.y + xv.z * tv.z ;
							V1.y = yv.x * tv.x + yv.y * tv.y + yv.z * tv.z ;
							V1.z = zv.x * tv.x + zv.y * tv.y + zv.z * tv.z ;
							V1.z = 0.0f ;
							VectorNormalize( &V1, &V1 ) ;

							// ����̕ӂ����|���S���̌���
							for( VFBuf = VertexFaceList[ PosInd[ CurFaceIndex % 3 ] ] ; VFBuf ; VFBuf = VFBuf->Next )
							{
								FaceT = ( MV1_MESH_FACE * )VFBuf->Face ;
								TMesh = ( MV1_MESH_BASE * )VFBuf->Mesh ;

								// ���Ɍ��o�ς݂̃|���S���͖���
								FNInd2 = FaceNormalIndex + ( FaceT - Frame->Mesh->Face ) * 3 ;
								if( ( NormalSetFlag[ FNInd2[ 0 ] ] & 2 ) ||
									( NormalSetFlag[ FNInd2[ 1 ] ] & 2 ) ||
									( NormalSetFlag[ FNInd2[ 2 ] ] & 2 ) ) continue ;

								// �������g���A��O�Ɏ����������|���S���͖���
								if( FaceT == CurFace || FaceT == BackCurFace ) continue ;

								// ���W�C���f�b�N�X���Z�b�g���Ă���
								TPosInd[ 0 ] = ( int )( ( MV1_MESH_VERTEX * )( ( BYTE * )TMesh->Vertex + TMesh->VertUnitSize * FaceT->VertexIndex[ 0 ] ) )->PositionIndex ;
								TPosInd[ 1 ] = ( int )( ( MV1_MESH_VERTEX * )( ( BYTE * )TMesh->Vertex + TMesh->VertUnitSize * FaceT->VertexIndex[ 1 ] ) )->PositionIndex ;
								TPosInd[ 2 ] = ( int )( ( MV1_MESH_VERTEX * )( ( BYTE * )TMesh->Vertex + TMesh->VertUnitSize * FaceT->VertexIndex[ 2 ] ) )->PositionIndex ;

								// ���W���Z�b�g���Ă���
								TPos[ 0 ] = ( ( MV1_MESH_POSITION * )( ( BYTE * )Frame->Position + Frame->PosUnitSize * TPosInd[ 0 ] ) )->Position ;
								TPos[ 1 ] = ( ( MV1_MESH_POSITION * )( ( BYTE * )Frame->Position + Frame->PosUnitSize * TPosInd[ 1 ] ) )->Position ;
								TPos[ 2 ] = ( ( MV1_MESH_POSITION * )( ( BYTE * )Frame->Position + Frame->PosUnitSize * TPosInd[ 2 ] ) )->Position ;

								// ���_�̐������J��Ԃ�
								for( m = 0 ; m < 3 ; m ++ )
								{
									CmpFlag = 0 ;

									// �����ӂ��������ׂ�A���_�w��̏��Ԃ���ʂ̕����𒲂ׂ�
									if( PosInd[ CurFaceIndex             % 3 ] == TPosInd[ m             % 3 ] &&
										PosInd[ ( CurFaceIndex + 3 - 1 ) % 3 ] == TPosInd[ ( m + 3 - 1 ) % 3 ] )
									{
										CmpFlag = 1 ;
										FaceUra = 1 ;
									}

									if( PosInd[ CurFaceIndex             % 3 ] == TPosInd[ ( m + 3 - 1 ) % 3 ] &&
										PosInd[ ( CurFaceIndex + 3 - 1 ) % 3 ] == TPosInd[ m             % 3 ] )
									{
										CmpFlag = 1 ;
										FaceUra = 0 ;
									}

									// �J�����g�ʂƓ���̒��_���g�p����P�W�O�x��]������( �J�����g�ʂ̗��� )�������猟�o�������Ƃ𖳂��������Ƃɂ���
									if( CmpFlag == 1 && PosInd[ ( CurFaceIndex + 3 - 2 ) % 3 ] == TPosInd[ ( m + 3 - 2 ) % 3 ] )
									{
										CmpFlag = 0 ;
									}

									// ����̕ӂ����������烋�[�v�𔲂���
									if( CmpFlag != 0 ) break ;
								}

								// ����̕ӂ����������珈��
								if( m != 3 )
								{
									// �������|���S���́A����̕ӂ𐬂����_�Ƃ͕ʂ̂�����̒��_�֌������x�N�g�����ɎZ�o�����O���֓��e�����x�N�g�����Z�o����
									VectorSub( &tv, &TPos[ ( m + 3 - 2 ) % 3 ], &Pos[ CurFaceIndex % 3 ] ) ;
									V2.x = xv.x * tv.x + xv.y * tv.y + xv.z * tv.z ;
									V2.y = yv.x * tv.x + yv.y * tv.y + yv.z * tv.z ;
									V2.z = zv.x * tv.x + zv.y * tv.y + zv.z * tv.z ;
									V2.z = 0.0f ;

									// ��̖ʂ�^�����猩���Ƃ��ɂł�����������p�̃R�T�C���l�𒲂ׂ�
									// ( �J�����g�ʂ̕\���������ɉ�]�����Ƃ��ɓ�ӂ������p�̊p�x�𒲂ׂ� )
									{
										// �O�ς��g���ĂP�W�O�x�ȏォ�ǂ����𔻒�
										VectorOuterProduct( &tv, &V1, &V2 ) ;
										VectorNormalize( &V2, &V2 ) ;
										if( tv.z > 0.0f )
										{
											// �P�W�O�x�ȏゾ�����ꍇ�͂P�W�O�x�ȏゾ�������Ƃ��L�^�������
											// �J�����g�ʑ��̃x�N�g�����t�]���ē��ς����߂�( �R�T�C���l�����߂� )
											FaceRadi = -1.0f ;
											FaceRadi2Use = 1 ;
											V3.x = -V1.x ;
											V3.y = -V1.y ;
											V3.z = -V1.z ;
											FaceRadi2 = VectorInnerProduct( &V3, &V2 ) ;
										}
										else
										{
											// �P�W�O�x�ȉ��������ꍇ�͂��̂܂ܓ�̐��̓��ς����߂�( �R�T�C���l�����߂� )
											FaceRadi = VectorInnerProduct( &V1, &V2 ) ;
											FaceRadi2Use = 0 ;
											FaceRadi2 = 1.0f ;
										}
									}

									// �܂��אڂ���ʂ������Ă��Ȃ���
									// ���Ɍ����Ă��邪����̖ʂ̗��ʂ�
									// ���Ɍ����Ă���אږʂ����p�x�I�ɋ߂��ʂ�������אږʂƂ��ċL�^����
									if( MinFace == NULL ||
										( ( ( TPosInd[ 0 ] == MPosInd[ 0 ] &&
											  TPosInd[ 1 ] == MPosInd[ 2 ] &&
											  TPosInd[ 2 ] == MPosInd[ 1 ] ) ||
											( TPosInd[ 0 ] == MPosInd[ 1 ] &&
											  TPosInd[ 1 ] == MPosInd[ 0 ] &&
											  TPosInd[ 2 ] == MPosInd[ 2 ] ) ||
											( TPosInd[ 0 ] == MPosInd[ 2 ] &&
											  TPosInd[ 1 ] == MPosInd[ 1 ] &&
											  TPosInd[ 2 ] == MPosInd[ 0 ] ) ) && FaceUra == 0 && MinFaceUra == 1 ) ||
										( FaceRadi2Use == 0 && MinFaceRadi2Use == 1 ) ||
										( FaceRadi2Use == 1 && MinFaceRadi2Use == 1 && FaceRadi2 > MinFaceRadi2 ) ||
										( FaceRadi2Use == 0 && MinFaceRadi2Use == 0 && FaceRadi  > MinFaceRadi ) )
									{
										MPosInd[ 0 ] = TPosInd[ 0 ] ;
										MPosInd[ 1 ] = TPosInd[ 1 ] ;
										MPosInd[ 2 ] = TPosInd[ 2 ] ;
										MFNInd = FNInd2 ;
										MinMesh = TMesh ;
										MinFace = FaceT ;
										MinFaceRadi = FaceRadi ;
										MinFaceRadi2Use = FaceRadi2Use ;
										MinFaceRadi2 = FaceRadi2 ;
										MinFaceUra = FaceUra ;
										MinFaceIndex = m - 1 ;
										if( MinFaceIndex < 0 ) MinFaceIndex += 3 ;
									}
								}
							}

							// �אږʂ𔭌��ł��Ȃ��������A�אږʂ����ʂ��������A�אږʂƂ̖@���̊p�x�����X���[�W���O�Ώۂ�臒l�𒴂��Ă�����G�b�W�Ƃ݂Ȃ�
							if( MinFace == NULL || MinFaceUra || VectorInnerProduct( FaceNormal + ( MinFace - Frame->Mesh->Face ), FaceNormal + ( CurFace - Frame->Mesh->Face ) ) < SmoothCos )
								break ;

							// ���������אږʂ������J�n�̖ʂ������ꍇ�͈�������Ƃ�������
							if( MinFace == FaceList[ 0 ] )
							{
								Issyuu = 1 ;
								break ;
							}

							// �m�肵���אږʂ��L�^
							FaceList[ FaceCount ] = MinFace ;
							FaceIndex[ FaceCount ] = ( BYTE )MinFaceIndex ;
							VectorAdd( &FaceNorm, &FaceNorm, FaceNormal + ( MinFace - Frame->Mesh->Face ) ) ;
							NormalSetFlag[ MFNInd[ MinFaceIndex ] ] |= 2 ;
							FaceCount ++ ;

							// �J�����g�ʂ̕ύX
							BackCurFace = CurFace ;
							CurMesh = MinMesh ;
							CurFace = MinFace ;
							CurFaceIndex = MinFaceIndex ;
						}
					}

					// ���o�����ʂ̖@���𑫂������̂𐳋K��
					VectorNormalize( &FaceNorm, &FaceNorm ) ;
				}

				// ���X�g�A�b�v���ꂽ�ʂɐ��K�������@�����Z�b�g����
				if( FaceCount != 0 )
				{
					MFNInd = FaceNormalIndex + ( FaceList[ 0 ] - Frame->Mesh->Face ) * 3 ;
					for( m = 0 ; m < FaceCount ; m ++ )
					{
						FNInd2 = FaceNormalIndex + ( FaceList[ m ] - Frame->Mesh->Face ) * 3 ;
						Index = FNInd2[ FaceIndex[ m ] ] ;
						NormalBuf[ Index ] = FaceNorm ;
						NormalSetFlag[ Index ] = 1 ;
						FNInd2[ FaceIndex[ m ] ] = MFNInd[ FaceIndex[ 0 ] ] ;
					}
				}
			}
		}
	}

	// �g�p���Ă���@�����C���f�b�N�X�̎Ⴂ���ɋl�߂�
	{
		// �g�p����Ă���@���̃}�b�v���쐬����
		_MEMSET( NormalUseTable, 0, ( size_t )NormalNum ) ;
		Mesh = Frame->Mesh ;
		UseNormalNum = 0 ;
		FNInd = FaceNormalIndex ;
		for( i = 0 ; i < Frame->MeshNum ; i ++, Mesh ++ )
		{
			Face = Mesh->Face ;
			for( j = 0 ; j < Mesh->FaceNum ; j ++, Face ++, FNInd += 3 )
			{
				if( NormalUseTable[ FNInd[ 0 ] ] == 0 )
				{
					NormalUseTable[ FNInd[ 0 ] ]   = 1 ;
					UseNormalIndex[ UseNormalNum ] = ( DWORD )FNInd[ 0 ] ;
					NewNormalIndex[ FNInd[ 0 ] ]   = ( DWORD )UseNormalNum ;
					UseNormalNum ++ ;
				}

				if( NormalUseTable[ FNInd[ 1 ] ] == 0 )
				{
					NormalUseTable[ FNInd[ 1 ] ]   = 1 ;
					UseNormalIndex[ UseNormalNum ] = ( DWORD )FNInd[ 1 ] ;
					NewNormalIndex[ FNInd[ 1 ] ]   = ( DWORD )UseNormalNum ;
					UseNormalNum ++ ;
				}

				if( NormalUseTable[ FNInd[ 2 ] ] == 0 )
				{
					NormalUseTable[ FNInd[ 2 ] ]   = 1 ;
					UseNormalIndex[ UseNormalNum ] = ( DWORD )FNInd[ 2 ] ;
					NewNormalIndex[ FNInd[ 2 ] ]   = ( DWORD )UseNormalNum ;
					UseNormalNum ++ ;
				}
			}
		}

		// �@���̐������������ǂ����Ŋi�[��̃�������ύX
		if( UseNormalNum <= Frame->NormalNum )
		{
			// �������ꍇ

			// �V���ȃ������͊m�ۂ��Ȃ�
			RNormal = Frame->Normal ;
		}
		else
		{
			// �������ꍇ
			if( Frame->NormalAllocMem )
			{
				DXFREE( Frame->Normal ) ;
			}
			RNormal = ( MV1_MESH_NORMAL * )DXALLOC( sizeof( MV1_MESH_NORMAL ) * UseNormalNum ) ;
			if( RNormal == NULL )
			{
				DXST_ERRORLOGFMT_ADDUTF16LE(( "\xd5\x6c\xda\x7d\x8d\x51\x08\x8a\x97\x7b\x67\x30\xd5\x6c\xda\x7d\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�@���Čv�Z�Ŗ@�����i�[���郁�����̊m�ۂɎ��s���܂���\n" @*/ )) ;
				goto ERR ;
			}
			Frame->Normal = RNormal ;
			Frame->NormalAllocMem = TRUE ;
		}

		// �ŏI�ł̖@���Z�b�g���쐬����
		for( i = 0 ; i < UseNormalNum ; i ++ )
		{
			RNormal[ i ].Normal = NormalBuf[ UseNormalIndex[ i ] ] ;
		}

		// �ʃf�[�^�̖@���C���f�b�N�X���X�V����
		FNInd = FaceNormalIndex ;
		Mesh = Frame->Mesh ;
		for( i = 0 ; i < Frame->MeshNum ; i ++, Mesh ++ )
		{
			Face = Mesh->Face ;
			for( j = 0 ; j < Mesh->FaceNum ; j ++, Face ++, FNInd += 3 )
			{
				FNInd[ 0 ] = ( int )NewNormalIndex[ FNInd[ 0 ] ] ;
				FNInd[ 1 ] = ( int )NewNormalIndex[ FNInd[ 1 ] ] ;
				FNInd[ 2 ] = ( int )NewNormalIndex[ FNInd[ 2 ] ] ;
			}
		}

		// �f�[�^�̃T�C�Y��ύX
		{
			int AddSize ;

			AddSize = UseNormalNum - Frame->NormalNum ;
			ModelBase->MeshNormalNum += AddSize ;
			ModelBase->VertexDataSize += AddSize * sizeof( MV1_MESH_NORMAL ) ;
		}

		// �ŏI�I�Ȗ@���̐����Z�b�g
		Frame->NormalNum = UseNormalNum ;
	}

	// �V�����@���������ɒ��_�f�[�^�z����č\�z����
	{
		BYTE TVertBuf[ sizeof( MV1_MESH_VERTEX ) + sizeof( float ) * 2 * 32 ] ;
		MV1_MESH_VERTEX *TVertex ;

		TVertex = ( MV1_MESH_VERTEX * )TVertBuf ;

		// �g���C�A���O�����X�g�\�z�p�����i�[���郁�����̊m��
		VertInfoTable = ( MV1_MAKEVERTINDEXINFO ** )DXALLOC( sizeof( MV1_MAKEVERTINDEXINFO * ) * Frame->PositionNum + sizeof( MV1_MAKEVERTINDEXINFO ) * Frame->TriangleNum * 3 ) ;
		if( VertInfoTable == NULL )
		{
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\xd5\x6c\xda\x7d\x8d\x51\x08\x8a\x97\x7b\x6b\x30\x7f\x4f\x28\x75\x59\x30\x8b\x30\x5c\x4f\x6d\x69\x28\x75\xe1\x30\xe2\x30\xea\x30\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x5f\x00\x33\x00\x0a\x00\x00"/*@ L"�@���Čv�Z�Ɏg�p�����Ɨp�������̊m�ۂɎ��s���܂���_3\n" @*/ )) ;
			goto ERR ;
		}
		VertInfoBuffer = ( MV1_MAKEVERTINDEXINFO * )( VertInfoTable + Frame->PositionNum ) ;
		VertValidBuffer = ( int * )VertInfoBuffer ;

		// ���b�V���̐������J��Ԃ�
		Mesh = Frame->Mesh ;
		FNInd = FaceNormalIndex ;
		for( i = 0 ; i < Frame->MeshNum ; i ++, Mesh ++ )
		{
			if( Mesh->FaceNum == 0 )
				continue ;

			// �V���Ȓ��_�f�[�^���i�[���郁�����̈�̊m��
			NewVertex = ( MV1_MESH_VERTEX * )DXALLOC( ( size_t )( Mesh->VertUnitSize * Mesh->FaceNum * 3 ) ) ;
			if( NewVertex == NULL )
			{
				DXST_ERRORLOGFMT_ADDUTF16LE(( "\xd5\x6c\xda\x7d\x8d\x51\x08\x8a\x97\x7b\x6b\x30\x7f\x4f\x28\x75\x59\x30\x8b\x30\x5c\x4f\x6d\x69\x28\x75\xe1\x30\xe2\x30\xea\x30\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�@���Čv�Z�Ɏg�p�����Ɨp�������̊m�ۂɎ��s���܂���\n" @*/ )) ;
				goto ERR ;
			}

			// ���܂ł̖ʏ����i�[���郁�����̈�̊m��
			OldFace = ( MV1_MESH_FACE * )DXALLOC( sizeof( MV1_MESH_FACE ) * Mesh->FaceNum ) ;
			if( OldFace == NULL )
			{
				DXST_ERRORLOGFMT_ADDUTF16LE(( "\xd5\x6c\xda\x7d\x8d\x51\x08\x8a\x97\x7b\x6b\x30\x7f\x4f\x28\x75\x59\x30\x8b\x30\x5c\x4f\x6d\x69\x28\x75\xe1\x30\xe2\x30\xea\x30\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x5f\x00\x32\x00\x0a\x00\x00"/*@ L"�@���Čv�Z�Ɏg�p�����Ɨp�������̊m�ۂɎ��s���܂���_2\n" @*/ )) ;
				goto ERR ;
			}
			_MEMCPY( OldFace, Mesh->Face, sizeof( MV1_MESH_FACE ) * Mesh->FaceNum ) ;

			// ���_�f�[�^�Ɩʃf�[�^���\�z
			NewVertexNum = 0 ;
			OFace = OldFace ;
			Face = Mesh->Face ;
			_MEMSET( VertInfoTable, 0, sizeof( MV1_MAKEVERTINDEXINFO * ) * Frame->PositionNum ) ;
			VertInfoNum = 0 ;
			for( j = 0 ; j < Mesh->FaceNum ; j ++, OFace ++, Face ++, FNInd += 3 )
			{
				// �|���S���̒��_�̐������J��Ԃ�
				for( m = 0 ; m < 3 ; m ++ )
				{
					// �ǉ����悤�Ƃ��Ă��钸�_�f�[�^�̍쐬
					Vert = ( MV1_MESH_VERTEX * )( ( BYTE * )Mesh->Vertex + Mesh->VertUnitSize * OFace->VertexIndex[ m ] ) ;
					_MEMCPY( TVertex, Vert, ( size_t )Mesh->VertUnitSize ) ;
					TVertex->NormalIndex = ( DWORD )FNInd[ m ] ;

					// ���܂łɓ������_�f�[�^�������������ǂ����𒲂ׂ�
					for( VInfo = VertInfoTable[ Vert->PositionIndex ] ; VInfo ; VInfo = VInfo->Next )
					{
						if( _MEMCMP( ( BYTE * )NewVertex + VInfo->VertexIndex * Mesh->VertUnitSize, TVertex, Mesh->VertUnitSize ) == 0 )
							break ;
					}
					if( VInfo == NULL )
					{
						// ����������f�[�^��ǉ�
						VInfo = &VertInfoBuffer[ VertInfoNum ] ;
						VertInfoNum ++ ;

						VInfo->VertexIndex = NewVertexNum ;
						VInfo->Next = VertInfoTable[ Vert->PositionIndex ] ;
						VertInfoTable[ Vert->PositionIndex ] = VInfo ;

						_MEMCPY( ( BYTE * )NewVertex + NewVertexNum * Mesh->VertUnitSize, TVertex, ( size_t )Mesh->VertUnitSize ) ;
						Face->VertexIndex[ m ] = ( DWORD )NewVertexNum ;
						NewVertexNum ++ ;
					}
					else
					{
						// ��������C���f�b�N�X���Z�b�g
						Face->VertexIndex[ m ] = ( DWORD )VInfo->VertexIndex ;
					}
				}
			}

			// ���܂ł̖ʏ����i�[���Ă����������̉��
			DXFREE( OldFace ) ;
			OldFace = NULL ;

			// ���_�f�[�^�̐����������ꍇ�ƌ������ꍇ�ŏ����𕪊�
			if( NewVertexNum <= Mesh->VertexNum )
			{
				// �������ꍇ

				// �V���Ɋm�ۂ������_�p�̃������͉������
				_MEMCPY( Mesh->Vertex, NewVertex, ( size_t )( NewVertexNum * Mesh->VertUnitSize ) ) ;
				DXFREE( NewVertex ) ;
			}
			else
			{
				// �������ꍇ
				if( Mesh->VertexAllocMem )
				{
					DXFREE( Mesh->Vertex ) ;
				}
				Mesh->Vertex = NewVertex ;
				Mesh->VertexAllocMem = TRUE ;
			}
			NewVertex = NULL ;

			// �f�[�^�̃T�C�Y��ύX
			{
				int AddSize ;

				AddSize = ( NewVertexNum - Mesh->VertexNum ) * Mesh->VertUnitSize ;
				ModelBase->MeshVertexSize += AddSize ;
				ModelBase->VertexDataSize += AddSize ;
				Frame->VertexNum += NewVertexNum - Mesh->VertexNum ;
			}

			// ���_�̐���ۑ�
			Mesh->VertexNum = NewVertexNum ;

			// �]�@���Ɛڐ����Z�o����
			MV1MakeMeshBinormalsAndTangents( Mesh ) ;

			// �g���C�A���O�����X�g�̍č\�z
			TList = Mesh->TriangleList ;
			for( j = 0 ; j < Mesh->TriangleListNum ; j ++, TList ++ )
			{
				// ���ˑ��̃e���|�����o�b�t�@���J��
				MV1_TerminateTriangleListBaseTempBuffer_PF( TList ) ;

				// ���_�����ɑ��݂��Ă��邩�ǂ����̃t���O������������
				_MEMSET( VertValidBuffer, 0xff, Mesh->VertexNum * sizeof( int ) ) ;

				// �V���Ȓ��_�C���f�b�N�X�z����i�[���邽�߂̃������̈�̊m��
				NewMeshVertexIndex = ( DWORD * )DXALLOC( sizeof( int ) * Mesh->VertexNum ) ;
				if( NewMeshVertexIndex == NULL )
				{
					DXST_ERRORLOGFMT_ADDUTF16LE(( "\xc8\x30\xe9\x30\xa4\x30\xa2\x30\xf3\x30\xb0\x30\xeb\x30\xea\x30\xb9\x30\xc8\x30\x6e\x30\xe1\x30\xc3\x30\xb7\x30\xe5\x30\x02\x98\xb9\x70\xa4\x30\xf3\x30\xc7\x30\xc3\x30\xaf\x30\xb9\x30\x3c\x68\x0d\x7d\x28\x75\xe1\x30\xe2\x30\xea\x30\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�g���C�A���O�����X�g�̃��b�V�����_�C���f�b�N�X�i�[�p�������̊m�ۂɎ��s���܂���\n" @*/ )) ;
					goto ERR ;
				}

				// ���b�V���̖ʂ̐������J��Ԃ�
				NewTVertexNum = 0 ;
				Face = Mesh->Face ;
				IndexNum = 0 ;
				for( k = 0 ; k < Mesh->FaceNum ; k ++, Face ++ )
				{
					// �g���C�A���O�����X�g�i���o�[��������牽�����Ȃ�
					if( Face->TriangleListIndex != j ) continue ;

					// ���_�̐������J��Ԃ�
					for( m = 0 ; m < 3 ; m ++, IndexNum ++ )
					{
						// �g���C�A���O�����g�p���Ă��钸�_�f�[�^�����ɂ��邩�ǂ����𒲂ׂ�
						if( VertValidBuffer[ Face->VertexIndex[ m ] ] == -1 )
						{
							// ����������ǉ�����
							VertValidBuffer[ Face->VertexIndex[ m ] ] = NewTVertexNum ;
							TList->Index[ IndexNum ] = ( unsigned short )NewTVertexNum ;

							NewMeshVertexIndex[ NewTVertexNum ] = Face->VertexIndex[ m ] ;
							NewTVertexNum ++ ;
						}
						else
						{
							TList->Index[ IndexNum ] = ( unsigned short )VertValidBuffer[ Face->VertexIndex[ m ] ] ;
						}
					}
				}

				// ���܂ł̒��_�̐���葽�����ǂ����ŏ����𕪊�
				if( NewTVertexNum <= TList->VertexNum )
				{
					// �������ꍇ

					// �V���Ɋm�ۂ������_�C���f�b�N�X�z��p�̃������͉������
					_MEMCPY( TList->MeshVertexIndex, NewMeshVertexIndex, sizeof( DWORD ) * NewTVertexNum ) ;
					DXFREE( NewMeshVertexIndex ) ;
				}
				else
				{
					// �������ꍇ
					if( TList->MeshVertexIndexAllocMem )
					{
						DXFREE( TList->MeshVertexIndex ) ;
					}
					TList->MeshVertexIndex = NewMeshVertexIndex ;
					TList->MeshVertexIndexAllocMem = TRUE ;

					// �������_�f�[�^�A�N�Z�X�p���������V���Ȓ��_�f�[�^�̃T�C�Y�ɉ����čĊm�ۂ���
					if( TList->PositionAllocMem )
					{
						DXFREE( TList->NormalPosition ) ;
					}
					TList->NormalPosition = ( MV1_TLIST_NORMAL_POS * )DXALLOC( ( size_t )( TList->PosUnitSize * NewTVertexNum + 16 ) ) ;
					if( TList->NormalPosition == NULL )
					{
						DXST_ERRORLOGFMT_ADDUTF16LE(( "\xc8\x30\xe9\x30\xa4\x30\xa2\x30\xf3\x30\xb0\x30\xeb\x30\xea\x30\xb9\x30\xc8\x30\x6e\x30\xa7\x5e\x19\x6a\xc7\x30\xfc\x30\xbf\x30\x3c\x68\x0d\x7d\x28\x75\xe1\x30\xe2\x30\xea\x30\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�g���C�A���O�����X�g�̍��W�f�[�^�i�[�p�������̊m�ۂɎ��s���܂���\n" @*/ )) ;
						goto ERR ;
					}
					TList->PositionAllocMem = TRUE ;
				}
				NewMeshVertexIndex = NULL ;

				// �f�[�^�̃T�C�Y��ύX
				{
					int AddSize ;

					AddSize = NewTVertexNum - TList->VertexNum ;
					switch( TList->VertexType )
					{
					case MV1_VERTEX_TYPE_NORMAL :        ModelBase->TriangleListNormalPositionNum     += AddSize ;                      break ;
					case MV1_VERTEX_TYPE_SKIN_4BONE :    ModelBase->TriangleListSkinPosition4BNum     += AddSize ;                      break ;
					case MV1_VERTEX_TYPE_SKIN_8BONE :    ModelBase->TriangleListSkinPosition8BNum     += AddSize ;                      break ;
					case MV1_VERTEX_TYPE_SKIN_FREEBONE : ModelBase->TriangleListSkinPositionFREEBSize += AddSize * TList->PosUnitSize ; break ;
					}

					AddSize = ( NewTVertexNum - TList->VertexNum ) * TList->PosUnitSize ;
					ModelBase->VertexDataSize += AddSize ;
					ModelBase->MeshVertexIndexNum += NewTVertexNum - TList->VertexNum ;
					ModelBase->TriangleListVertexNum += NewTVertexNum - TList->VertexNum ;
				}

				// ���_�f�[�^�̐����Z�b�g
				TList->VertexNum = NewTVertexNum ;

				// ���������p���_�f�[�^�̍č\�z
				MV1SetupTriangleListPositionAndNormal( TList ) ;

				// �g�D�[���֊s���p���_�C���f�b�N�X�̍쐬
				MV1SetupToonOutLineTriangleList( TList ) ;
			}
		}

		// �V�F�C�v���_�f�[�^�̍X�V
		Shape = Frame->Shape ;
		for( i = 0 ; i < Frame->ShapeNum ; i ++, Shape ++ )
		{
			ShapeFrameVert = ShapeFrameVertBuffer + i * Frame->PositionNum ;
			ShapeMesh = Shape->Mesh ;
			for( j = 0 ; j < Shape->MeshNum ; j ++, ShapeMesh ++ )
			{
				Mesh = ShapeMesh->TargetMesh ;
				NewShapeVertBuffer = ( MV1_SHAPE_VERTEX_BASE * )DXALLOC( sizeof( MV1_SHAPE_VERTEX_BASE ) * Mesh->VertexNum ) ;
				if( NewShapeVertBuffer == NULL )
				{
					DXST_ERRORLOGFMT_ADDUTF16LE(( "\xb7\x30\xa7\x30\xa4\x30\xd7\x30\x02\x98\xb9\x70\xc7\x30\xfc\x30\xbf\x30\x3c\x68\x0d\x7d\x28\x75\xe1\x30\xe2\x30\xea\x30\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�V�F�C�v���_�f�[�^�i�[�p�������̊m�ۂɎ��s���܂���\n" @*/ )) ;
					goto ERR ;
				}

				NewShapeVertexNum = 0 ;
				Vert = Mesh->Vertex ;
				for( k = 0 ; k < Mesh->VertexNum ; k ++, Vert = ( MV1_MESH_VERTEX * )( ( BYTE * )Vert + Mesh->VertUnitSize ) )
				{
					// ���_���g�p���Ă�����W�l���V�F�C�v�Ώۂł͂Ȃ��ꍇ�͉������Ȃ�
					if( ShapeFrameVert[ Vert->PositionIndex ].TargetMeshVertex == -1 ) continue ;

					// ���̒ǉ�
					NewShapeVertBuffer[ NewShapeVertexNum ].TargetMeshVertex = k ;
					NewShapeVertBuffer[ NewShapeVertexNum ].Position         = ShapeFrameVert[ Vert->PositionIndex ].Position ;
					NewShapeVertBuffer[ NewShapeVertexNum ].Normal           = VGet( 0.0f, 0.0f, 0.0f ) ;
					NewShapeVertexNum ++ ;
				}

				// ��񂪑������ꍇ�͐V���Ɋm�ۂ������������g�p����
				if( ( DWORD )NewShapeVertexNum > ShapeMesh->VertexNum )
				{
					if( ShapeMesh->VertexAllocMem )
					{
						DXFREE( ShapeMesh->Vertex ) ;
					}
					ShapeMesh->Vertex = NewShapeVertBuffer ;
					ShapeMesh->VertexAllocMem = TRUE ;
				}
				else
				{
					// �����ł͂Ȃ��ꍇ�͍��܂ł̃o�b�t�@�ɐV�����f�[�^���R�s�[����
					_MEMCPY( ShapeMesh->Vertex, NewShapeVertBuffer, sizeof( MV1_SHAPE_VERTEX_BASE ) * NewShapeVertexNum ) ;

					// �m�ۂ����������͉������
					DXFREE( NewShapeVertBuffer ) ;
				}
				NewShapeVertBuffer = NULL ;

				// �V�F�C�v�̒��_������ύX����
				ModelBase->ShapeVertexNum += NewShapeVertexNum - ShapeMesh->VertexNum ;

				// �V�F�C�v�̒��_�����X�V����
				ShapeMesh->VertexNum = ( DWORD )NewShapeVertexNum ;
			}
		}
	}

	// �������̉��
	if( VertexFaceList )
	{
		DXFREE( VertexFaceList ) ;
		VertexFaceList = NULL ;
	}
	if( FaceNormal )
	{
		DXFREE( FaceNormal ) ;
		FaceNormal = NULL ;
	}
	if( NormalBuf )
	{
		DXFREE( NormalBuf ) ;
		NormalBuf = NULL ;
	}
	if( OldFace )
	{
		DXFREE( OldFace ) ;
		OldFace = NULL ;
	}
	if( VertInfoTable )
	{
		DXFREE( VertInfoTable ) ;
		VertInfoTable = NULL ;
	}
	if( NewShapeVertBuffer )
	{
		DXFREE( NewShapeVertBuffer ) ;
		NewShapeVertBuffer = NULL ;
	}
	
	// ����I��
	return true ;

ERR :
	// �������̉��
	if( VertexFaceList )
	{
		DXFREE( VertexFaceList ) ;
		VertexFaceList = NULL ;
	}
	if( FaceNormal )
	{
		DXFREE( FaceNormal ) ;
		FaceNormal = NULL ;
	}
	if( NormalBuf )
	{
		DXFREE( NormalBuf ) ;
		NormalBuf = NULL ;
	}
	if( NewVertex )
	{
		DXFREE( NewVertex ) ;
		NewVertex = NULL ;
	}
	if( OldFace )
	{
		DXFREE( OldFace ) ;
		OldFace = NULL ;
	}
	if( VertInfoTable )
	{
		DXFREE( VertInfoTable ) ;
		VertInfoTable = NULL ;
	}
	if( NewMeshVertexIndex )
	{
		DXFREE( NewMeshVertexIndex ) ;
		NewMeshVertexIndex = NULL ;
	}
	if( NewShapeVertBuffer )
	{
		DXFREE( NewShapeVertBuffer ) ;
		NewShapeVertBuffer = NULL ;
	}

	// �G���[
	return -1 ;
}

// ���f���S�̂̍��W�����œK������
extern int MV1PositionOptimizeBase( int MBHandle )
{
	MV1_MODEL_BASE *ModelBase ;
	MV1_FRAME_BASE *Frame ;
	int i ;

	// ����������Ă��Ȃ�������G���[
	if( MV1Man.Initialize == false ) return -1 ;

	// �A�h���X�擾
	if( MV1BMDLCHK( MBHandle, ModelBase ) )
		return -1 ;

	// ���_�o�b�t�@���������
	MV1_TerminateVertexBufferBase_PF( MBHandle ) ;

	// �t���[���̐������J��Ԃ�
	Frame = ModelBase->Frame ;
	for( i = 0 ; i < ModelBase->FrameNum ; i ++, Frame ++ )
	{
		if( Frame->MeshNum == 0 ) continue ;

		if( _MV1PositionOptimizeFrameBase( Frame ) == -1 )
			return -1 ;
	}

	// ���_�o�b�t�@���\�z����
	MV1_SetupVertexBufferBase_PF( MBHandle ) ;

	// �I��
	return 0 ;
}

// �w��t���[���������b�V���̍��W�����œK������
extern int MV1PositionOptimizeFrameBase( int MBHandle, int FrameIndex )
{
	MV1_MODEL_BASE *ModelBase ;
	MV1_FRAME_BASE *Frame ;

	// ����������Ă��Ȃ�������G���[
	if( MV1Man.Initialize == false ) return -1 ;

	// �A�h���X�擾
	if( MV1BMDLCHK( MBHandle, ModelBase ) )
		return -1 ;

	// �t���[���C���f�b�N�X�̃`�F�b�N
	if( FrameIndex < 0 || FrameIndex >= ModelBase->FrameNum )
		return -1 ;
	Frame = ModelBase->Frame + FrameIndex ;

	// ���b�V����������Ή��������I��
	if( Frame->MeshNum == 0 )
		return -1 ;

	// ���_�o�b�t�@���������
	MV1_TerminateVertexBufferBase_PF( MBHandle ) ;

	// �t���[���̍Čv�Z
	if( _MV1PositionOptimizeFrameBase( Frame ) == -1 )
		return -1 ;

	// ���_�o�b�t�@���\�z����
	MV1_SetupVertexBufferBase_PF( MBHandle ) ;

	// �I��
	return 0 ;
}

// �t���[���̍��W�����œK������
static int _MV1PositionOptimizeFrameBase( MV1_FRAME_BASE *Frame )
{
	int i, j, k, l, m, SubNum, BoneNum1, BoneNum2, NewNum, *NewIndex, *NewMVertIndex, Size, MaxNum ;
	MV1_MESH_BASE *Mesh ;
	BYTE *DisableFlag ;
	MV1_MESH_POSITION *Pos1, *Pos2 ;
	MV1_MESH_VERTEX *Vert, *Vert1, *Vert2 ;
	MV1_MESH_FACE *Face ;
	MV1_MODEL_BASE *ModelBase ;
	MV1_SHAPE_BASE *Shape ;
	MV1_SHAPE_MESH_BASE *ShapeMesh ;
	MV1_SHAPE_VERTEX_BASE *ShapeVertex ;
	MV1_TRIANGLE_LIST_BASE *TList ;
	DWORD *MInd1, *MInd2 ;
	WORD *Ind ;
	BYTE *TPos1, *TPos2 ;

	// ���b�V�������������牽�����Ȃ�
	if( Frame->MeshNum == 0 )
		return 0 ;

	// �|�C���^�̏�����
	ModelBase = Frame->Container ;
	DisableFlag = NULL ;

	// �ł����_�f�[�^�̐����������b�V���̒��_�����擾����
	Mesh = Frame->Mesh ;
	MaxNum = 0 ;
	for( i = 0 ; i < Frame->MeshNum ; i ++, Mesh ++ )
	{
		if( Mesh->VertexNum > MaxNum )
			MaxNum = Mesh->VertexNum ;
	}
	if( MaxNum < Frame->PositionNum )
		MaxNum = Frame->PositionNum ;

	// ��Ɨp�������̊m��
	DisableFlag = ( BYTE * )DXALLOC( ( sizeof( int ) * 2 + sizeof( BYTE ) ) * MaxNum ) ;
	if( DisableFlag == NULL )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\x02\x98\xb9\x70\xc5\x60\x31\x58\x00\x67\x69\x90\x16\x53\xe6\x51\x06\x74\x28\x75\x6e\x30\xe1\x30\xe2\x30\xea\x30\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"���_���œK�������p�̃������̊m�ۂɎ��s���܂���\n" @*/ )) ;
		goto ERR ;
	}
	NewIndex = ( int * )( DisableFlag + MaxNum ) ;
	NewMVertIndex = NewIndex + MaxNum ;
	_MEMSET( NewIndex, 0, sizeof( int ) * MaxNum ) ;
	_MEMSET( NewMVertIndex, 0, sizeof( int ) * MaxNum ) ;

	// ���W���̈�v���钸�_��񋓂���
	SubNum = 0 ;
	Pos1 = Frame->Position ;
	_MEMSET( DisableFlag, 0, sizeof( BYTE ) * MaxNum ) ;
	for( i = 0 ; i < Frame->PositionNum ; i ++, Pos1 = ( MV1_MESH_POSITION * )( ( BYTE * )Pos1 + Frame->PosUnitSize ) )
	{
		// ���ɖ����������牽�����Ȃ�
		if( DisableFlag[ i ] ) continue ;

		Pos2 = ( MV1_MESH_POSITION * )( ( BYTE * )Frame->Position + Frame->PosUnitSize * ( i + 1 ) ) ;
		for( j = i + 1 ; j < Frame->PositionNum ; j ++, Pos2 = ( MV1_MESH_POSITION * )( ( BYTE * )Pos2 + Frame->PosUnitSize ) )
		{
			// ���ɖ����������牽�����Ȃ�
			if( DisableFlag[ j ] ) continue ;

			// ���W�����������ׂ�
			if( *( ( DWORD * )&Pos1->Position.x ) != *( ( DWORD * )&Pos2->Position.x ) ||
				*( ( DWORD * )&Pos1->Position.y ) != *( ( DWORD * )&Pos2->Position.y ) ||
				*( ( DWORD * )&Pos1->Position.z ) != *( ( DWORD * )&Pos2->Position.z ) ) continue ;

			// �X�L�j���O�E�G�C�g�����������ׂ�
			for( BoneNum1 = 0 ; BoneNum1 < Frame->UseSkinBoneNum && Pos1->BoneWeight[ BoneNum1 ].Index != -1 ; BoneNum1 ++ ){}
			for( BoneNum2 = 0 ; BoneNum2 < Frame->UseSkinBoneNum && Pos2->BoneWeight[ BoneNum2 ].Index != -1 ; BoneNum2 ++ ){}
			if( BoneNum1 != BoneNum2 ) continue ;
			for( k = 0 ; k < BoneNum1 ; k ++ )
			{
				if( Pos1->BoneWeight[ k ].Index              != Pos2->BoneWeight[ k ].Index ||
					*( ( DWORD * )&Pos1->BoneWeight[ k ].W ) != *( ( DWORD * )&Pos2->BoneWeight[ k ].W ) )
					break ;
			}
			if( k != BoneNum1 ) continue ;

			// �������_�𔭌�������ԍ��̎Ⴂ���_�ɓ�������
			Mesh = Frame->Mesh ;
			for( k = 0 ; k < Frame->MeshNum ; k ++, Mesh ++ )
			{
				Vert = Mesh->Vertex ;
				for( l = 0 ; l < Mesh->VertexNum ; l ++, Vert = ( MV1_MESH_VERTEX * )( ( BYTE * )Vert + Mesh->VertUnitSize ) )
				{
					if( Vert->PositionIndex != ( DWORD )j ) continue ;
					Vert->PositionIndex = ( DWORD )i ;
/*
					int n, o, p, q ;
					MV1_TRIANGLE_LIST_BASE *TTList ;
					MV1_FRAME_BASE *FFrame ;
					MV1_MESH_BASE *MMesh ;
					MV1_MESH_POSITION *PPos ;
					DWORD *MMInd ;
					MV1_MESH_VERTEX *VVert ;

					TTList = ModelBase->TriangleList ;
					for( o = 0 ; o < ModelBase->TriangleListNum ; o ++, TTList ++ )
					{
						MMesh = TTList->Container ;
						FFrame = MMesh->Container ;
						MMInd = TTList->MeshVertexIndex ;
						for( p = 0 ; p < TTList->VertexNum ; p ++, MMInd ++ )
						{
							VVert = ( MV1_MESH_VERTEX * )( ( BYTE * )MMesh->Vertex + *MMInd * MMesh->VertUnitSize ) ;
							PPos = ( MV1_MESH_POSITION * )( ( BYTE * )FFrame->Position + VVert->PositionIndex * FFrame->PosUnitSize ) ;
							for( n = 0 ; n < FFrame->UseSkinBoneNum && PPos->BoneWeight[ n ].Index != -1 ; n ++ )
							{
								for( m = 0 ; m < TTList->UseBoneNum && TTList->UseBone[ m ] != PPos->BoneWeight[ n ].Index ; m ++ ){}
								if( m == TTList->UseBoneNum )
								{
									m = 0 ;
								}
							}
						}
					}
*/
				}
			}

			// �����t���O�𗧂Ă�
			DisableFlag[ j ] = 1 ;

			// ���炵���������Z����
			SubNum ++ ;
		}
	}

	// �������Ȃ��ꍇ�͉������Ȃ�
	if( SubNum != 0 )
	{
		// ���炵�����������W�f�[�^�T�C�Y�����Z����
		Size = Frame->PosUnitSize * SubNum ;
		ModelBase->MeshPositionSize -= Size ;
		ModelBase->VertexDataSize -= Size ;

		// �g�p����Ă�����W��񋓂���
		NewNum = 0 ;
		Pos1 = Frame->Position ;
		Pos2 = Frame->Position ;
		for( i = 0 ; i < Frame->PositionNum ; i ++, Pos1 = ( MV1_MESH_POSITION * )( ( BYTE * )Pos1 + Frame->PosUnitSize ) )
		{
			if( DisableFlag[ i ] ) continue ;

			NewIndex[ i ] = NewNum ;
			_MEMCPY( Pos2, Pos1, ( size_t )Frame->PosUnitSize ) ;
			Pos2 = ( MV1_MESH_POSITION * )( ( BYTE * )Pos2 + Frame->PosUnitSize ) ;
			NewNum ++ ;
		}

		// �V�������W�����Z�b�g����
		Frame->PositionNum = NewNum ;

		// ���_�f�[�^�̍��W�ԍ���ύX����
		Mesh = Frame->Mesh ;
		for( i = 0 ; i < Frame->MeshNum ; i ++, Mesh ++ )
		{
			Vert = Mesh->Vertex ;
			for( j = 0 ; j < Mesh->VertexNum ; j ++, Vert = ( MV1_MESH_VERTEX * )( ( BYTE * )Vert + Mesh->VertUnitSize ) )
			{
				Vert->PositionIndex = ( DWORD )NewIndex[ Vert->PositionIndex ] ;
			}
		}

		// ���_���̈�v���钸�_��񋓂���
		Mesh = Frame->Mesh ;	
		_MEMSET( NewMVertIndex, 0xff, sizeof( int ) * MaxNum ) ;
		for( i = 0 ; i < Frame->MeshNum ; i ++, Mesh ++ )
		{
			SubNum = 0 ;
			Vert1 = Mesh->Vertex ;
			_MEMSET( DisableFlag, 0, sizeof( BYTE ) * MaxNum ) ;
			for( j = 0 ; j < Mesh->VertexNum ; j ++, Vert1 = ( MV1_MESH_VERTEX * )( ( BYTE * )Vert1 + Mesh->VertUnitSize ) )
			{
				// ���ɖ����������牽�����Ȃ�
				if( DisableFlag[ j ] ) continue ;

				Vert2 = ( MV1_MESH_VERTEX * )( ( BYTE * )Mesh->Vertex + Mesh->VertUnitSize * ( j + 1 ) ) ;
				for( k = j + 1 ; k < Mesh->VertexNum ; k ++, Vert2 = ( MV1_MESH_VERTEX * )( ( BYTE * )Vert2 + Mesh->VertUnitSize ) )
				{
					// ���ɖ����������牽�����Ȃ�
					if( DisableFlag[ k ] ) continue ;

					// ��񂪓��������ׂ�
					if( _MEMCMP( Vert1, Vert2, Mesh->VertUnitSize ) != 0 )
						continue ;

					// �������_�𔭌�������ԍ��̎Ⴂ���_�ɓ�������
					NewMVertIndex[ k ] = j ;
					Face = Mesh->Face ;
					for( l = 0 ; l < Mesh->FaceNum ; l ++, Face ++ )
					{
						if( Face->VertexIndex[ 0 ] == ( DWORD )k )
							Face->VertexIndex[ 0 ] = ( DWORD )j ;
						if( Face->VertexIndex[ 1 ] == ( DWORD )k )
							Face->VertexIndex[ 1 ] = ( DWORD )j ;
						if( Face->VertexIndex[ 2 ] == ( DWORD )k )
							Face->VertexIndex[ 2 ] = ( DWORD )j ;
					}
					TList = Mesh->TriangleList ;
					for( l = 0 ; l < Mesh->TriangleListNum ; l ++, TList ++ )
					{
						MInd1 = TList->MeshVertexIndex ;
						for( m = 0 ; m < TList->VertexNum ; m ++, MInd1 ++ )
						{
							if( *MInd1 == ( DWORD )k )
								*MInd1 = ( DWORD )j ;
						}
					}

					// �����t���O�𗧂Ă�
					DisableFlag[ k ] = 1 ;

					// ���炵���������Z����
					SubNum ++ ;
				}
			}

			// �������Ȃ��ꍇ�͉������Ȃ�
			if( SubNum != 0 )
			{
				// �V�F�C�v���b�V��������ꍇ�̓V�F�C�v���b�V���̑Ώے��_���ύX����
				Shape = Frame->Shape ;
				for( j = 0 ; j < Frame->ShapeNum ; j ++, Shape ++ )
				{
					ShapeMesh = Shape->Mesh ;
					for( k = 0 ; k < Shape->MeshNum ; k ++, ShapeMesh ++ )
					{
						if( ShapeMesh->TargetMesh != Mesh ) continue ;

						ShapeVertex = ShapeMesh->Vertex ;
						for( l = 0 ; ( DWORD )l < ShapeMesh->VertexNum ; l ++, ShapeVertex ++ )
						{
							if( NewMVertIndex[ ShapeVertex->TargetMeshVertex ] == -1 ) continue ;
							ShapeVertex->TargetMeshVertex = NewMVertIndex[ ShapeVertex->TargetMeshVertex ] ;
						}
					}
				}

				// ���炵�����������_�f�[�^�T�C�Y�����Z����
				Size = Mesh->VertUnitSize * SubNum ;
				ModelBase->MeshVertexSize -= Size ;
				ModelBase->VertexDataSize -= Size ;
				Frame->VertexNum -= SubNum ;

				// �g�p����Ă��钸�_��񋓂���
				NewNum = 0 ;
				Vert1 = Mesh->Vertex ;
				Vert2 = Mesh->Vertex ;
				for( j = 0 ; j < Mesh->VertexNum ; j ++, Vert1 = ( MV1_MESH_VERTEX * )( ( BYTE * )Vert1 + Mesh->VertUnitSize ) )
				{
					if( DisableFlag[ j ] ) continue ;

					NewIndex[ j ] = NewNum ;
					_MEMCPY( Vert2, Vert1, ( size_t )Mesh->VertUnitSize ) ;
					Vert2 = ( MV1_MESH_VERTEX * )( ( BYTE * )Vert2 + Mesh->VertUnitSize ) ;
					NewNum ++ ;
				}

				// �V�������W�����Z�b�g����
				Mesh->VertexNum = NewNum ;

				// ���_�f�[�^�̒��_�ԍ���ύX����
				Face = Mesh->Face ;
				for( j = 0 ; j < Mesh->FaceNum ; j ++, Face ++ )
				{
					Face->VertexIndex[ 0 ] = ( DWORD )NewIndex[ Face->VertexIndex[ 0 ] ] ;
					Face->VertexIndex[ 1 ] = ( DWORD )NewIndex[ Face->VertexIndex[ 1 ] ] ;
					Face->VertexIndex[ 2 ] = ( DWORD )NewIndex[ Face->VertexIndex[ 2 ] ] ;
				}
				TList = Mesh->TriangleList ;
				for( j = 0 ; j < Mesh->TriangleListNum ; j ++, TList ++ )
				{
					for( k = 0 ; k < TList->VertexNum ; k ++ )
						TList->MeshVertexIndex[ k ] = ( DWORD )NewIndex[ TList->MeshVertexIndex[ k ] ] ;
				}

				// �g���C�A���O�����X�g�̃V�F�[�_�[���g�p���̒��_�f�[�^���������
				TList = Mesh->TriangleList ;
				for( j = 0 ; j < Mesh->TriangleListNum ; j ++, TList ++ )
				{
					MV1_TerminateTriangleListBaseTempBuffer_PF( TList ) ;
				}

				// �g���C�A���O�����X�g���̃��b�V�����_�C���f�b�N�X�ŏd�����Ă������񋓂���
				TList = Mesh->TriangleList ;
				for( j = 0 ; j < Mesh->TriangleListNum ; j ++, TList ++ )
				{
					SubNum = 0 ;
					_MEMSET( DisableFlag, 0, sizeof( BYTE ) * TList->VertexNum ) ;
					MInd1 = TList->MeshVertexIndex ;
					for( k = 0 ; k < TList->VertexNum ; k ++, MInd1 ++ )
					{
						// ���ɖ����������牽�����Ȃ�
						if( DisableFlag[ k ] ) continue ;

						MInd2 = TList->MeshVertexIndex + k + 1 ;
						for( l = k + 1 ; l < TList->VertexNum ; l ++, MInd2 ++ )
						{
							// ���ɖ����������牽�����Ȃ�
							if( DisableFlag[ l ] ) continue ;

							// ���_��񂪓��������ׂ�
							if( *MInd1 != *MInd2 ) continue ;

							// �������_�𔭌�������ԍ��̎Ⴂ���_�ɓ�������
							Ind = TList->Index ;
							for( m = 0 ; m < TList->IndexNum ; m ++, Ind ++ )
							{
								if( *Ind == l )
									*Ind = ( WORD )k ;
							}

							// �����t���O�𗧂Ă�
							DisableFlag[ l ] = 1 ;

							// ���炵���������Z����
							SubNum ++ ;
						}
					}

					// �������Ȃ��ꍇ�͉������Ȃ�
					if( SubNum != 0 )
					{
						// ���炷���������_�f�[�^�T�C�Y�����Z����
						ModelBase->MeshVertexIndexNum -= SubNum ;
						ModelBase->TriangleListVertexNum -= SubNum ;
						ModelBase->VertexDataSize -= SubNum * sizeof( DWORD ) ;
						switch( TList->VertexType )
						{
						case MV1_VERTEX_TYPE_NORMAL :
							ModelBase->TriangleListNormalPositionNum -= SubNum ;
							ModelBase->VertexDataSize -= SubNum * sizeof( MV1_TLIST_NORMAL_POS ) ;
							break ;

						case MV1_VERTEX_TYPE_SKIN_4BONE :
							ModelBase->TriangleListSkinPosition4BNum -= SubNum ;
							ModelBase->VertexDataSize -= SubNum * sizeof( MV1_TLIST_SKIN_POS_4B ) ;
							break ;

						case MV1_VERTEX_TYPE_SKIN_8BONE :
							ModelBase->TriangleListSkinPosition8BNum -= SubNum ;
							ModelBase->VertexDataSize -= SubNum * sizeof( MV1_TLIST_SKIN_POS_8B ) ;
							break ;

						case MV1_VERTEX_TYPE_SKIN_FREEBONE :
							ModelBase->TriangleListSkinPositionFREEBSize -= TList->PosUnitSize * SubNum ;
							ModelBase->VertexDataSize -= TList->PosUnitSize * SubNum ;
							break ;
						}

						// �g�p����Ă��郁�b�V�����_�C���f�b�N�X��񋓂���
						NewNum = 0 ;
						MInd1 = TList->MeshVertexIndex ;
						MInd2 = TList->MeshVertexIndex ;
						TPos1 = ADDR16( TList->NormalPosition ) ;
						TPos2 = ADDR16( TList->NormalPosition ) ;
						for( k = 0 ; k < TList->VertexNum ; k ++, MInd1 ++, TPos1 += TList->PosUnitSize )
						{
							if( DisableFlag[ k ] ) continue ;

							NewIndex[ k ] = NewNum ;
							*MInd2 = *MInd1 ;
							_MEMCPY( TPos2, TPos1, TList->PosUnitSize ) ;
							MInd2 ++ ;
							TPos2 += TList->PosUnitSize ;
							NewNum ++ ;
						}

						// �V�������b�V�����_�C���f�b�N�X�����Z�b�g����
						TList->VertexNum = NewNum ;

						// ���_�f�[�^�̒��_�ԍ���ύX����
						Ind = TList->Index ;
						for( k = 0 ; k < TList->IndexNum ; k ++, Ind ++ )
						{
							*Ind = ( WORD )NewIndex[ *Ind ] ;
						}
					}
				}
			}
		}
	}

	if( DisableFlag )
	{
		DXFREE( DisableFlag ) ;
		DisableFlag = NULL ;
	}

	// ����I��
	return 0 ;

ERR:
	if( DisableFlag )
	{
		DXFREE( DisableFlag ) ;
		DisableFlag = NULL ;
	}

	// �G���[
	return -1 ;
}

#if 0
// �R���o�C�����b�V���̃Z�b�g�A�b�v���s��
static int _MV1SetupCombineMesh( MV1_FRAME_BASE *Frame )
{
	MV1_MESH_BASE *BaseMesh = NULL, *MBMesh ;
	MV1_FRAME_BASE *MBFrame ;
	int TotalPolygonNum = 0 ;
	int i, MeshFrameNum = 0 ;
	int TotalVertexNum = 0 ;
	int TotalIndexNum = 0 ;
	int TotalMeshVertexNum = 0 ;
	MV1_MESH_BASE *CMesh ;
	MV1_TRIANGLE_LIST_BASE *CTList, *MBTList ;
	int AllocMemSize ;

	// ���ɃZ�b�g�A�b�v�ς݂̏ꍇ�͉������Ȃ�
	if( Frame->IsCombineMesh == true )
		return 0 ;

	// �R���o�C�����b�V�����Z�b�g�A�b�v������������������ׂ�

	// �q�����Ȃ��ꍇ�̓R���o�C���������Ȃ�
	if( Frame->TotalChildNum == 0 ) return 0 ;

	// ���b�V��������A��\���ł͂Ȃ��ꍇ�̂ݍs���`�F�b�N
	if( Frame->MeshNum != 0 && Frame->Mesh->Visible == 1 )
	{
		// �}�e���A�����ȏ�g���ꍇ�͕s�K��
		if( Frame->MeshNum >= 2 ) return 0 ;

		// �X�L�j���O���b�V�����V�F�C�v���b�V�����s�K��
		if( Frame->IsSkinMesh || Frame->ShapeNum != 0 )
			return 0 ;

		// �|���S�����Ⓒ�_��C���f�b�N�X�̐����Z�b�g
		TotalPolygonNum = Frame->TriangleNum ;
		TotalVertexNum = Frame->Mesh->TriangleList->VertexNum ;
		TotalIndexNum = Frame->Mesh->TriangleList->IndexNum ;
		TotalMeshVertexNum += Frame->Mesh->VertexNum ;

		// ���b�V���������Ă���t���[���̐����P�ɃZ�b�g
		MeshFrameNum = 1 ;

		// �����p���b�V���̃Z�b�g
		BaseMesh = Frame->Mesh ;
	}

	// �q�ɂ����l�̃`�F�b�N�����Ȃ��瑍�|���S�����𐔂���
	MBFrame = Frame->FirstChild ;
	for( i = 0 ; i < Frame->TotalChildNum ; i ++, MBFrame ++ )
	{
		// ���b�V���������ꍇ�͉������Ȃ�
		if( MBFrame->MeshNum == 0 ) continue ;
		MBMesh = MBFrame->Mesh ;

		// �}�e���A�����ȏ�g���ꍇ�͕s�K��
		if( MBFrame->MeshNum >= 2  )
			return 0 ;

		// �X�L�j���O���b�V����V�F�C�v���b�V���̏ꍇ���s�K��
		if( MBFrame->IsSkinMesh || MBFrame->ShapeNum != 0 )
			return 0 ;

		// �g���C�A���O�����X�g�̐����Q�ȏ�̏ꍇ���s�K��
		if( MBMesh->TriangleListNum >= 2 )
			return 0 ;

		// �����p���b�V�������Ɍ��o����Ă��邩�ǂ����ŏ����𕪊�
		if( BaseMesh == NULL )
		{
			BaseMesh = MBMesh ;
		}
		else
		{
			// �e�ƈႤ�}�e���A���̏ꍇ�͕s�K��
			if( MBMesh->Material != BaseMesh->Material )
				return 0 ;

			// �e�t���[���̃��b�V���ƒ��_�J���[�̈������������s�K��
			if( MBMesh->UseVertexDiffuseColor  != BaseMesh->UseVertexDiffuseColor ||
				MBMesh->UseVertexSpecularColor != BaseMesh->UseVertexSpecularColor )
				return 0 ;

			// �o�b�N�J�����O�ݒ肪�e�t���[���ƈ���Ă�����s�K��
			if( MBMesh->BackCulling != BaseMesh->BackCulling )
				return 0 ;

			// ���_��ӂ�̃T�C�Y���e�t���[���ƈ���Ă�����s�K��
			if( MBMesh->VertUnitSize != BaseMesh->VertUnitSize )
				return 0 ;

			// ���_�f�[�^��Ɋ܂܂��e�N�X�`�����W�̐����������s�K��
			if( MBMesh->UVSetUnitNum != BaseMesh->UVSetUnitNum ||
				MBMesh->UVUnitNum    != BaseMesh->UVUnitNum )
				return 0 ;
		}

		// ��\���̏ꍇ�͉������Ȃ�
		if( MBFrame->Mesh->Visible == 0 ) continue ;

		// �|���S���ƒ��_�ƃC���f�b�N�X�̑��������Z
		TotalPolygonNum += MBFrame->TriangleNum ;
		TotalVertexNum += MBMesh->TriangleList->VertexNum ;
		TotalIndexNum += MBMesh->TriangleList->IndexNum ;
		TotalMeshVertexNum += MBMesh->VertexNum ;

		// ���b�V���������Ă���t���[���̐����C���N�������g
		MeshFrameNum ++ ;
	}

	// �|���S���̑����� 5000 �𒴂���ꍇ�͕s�K��
	if( TotalPolygonNum > 5000 ) return 0 ;

	// ���_��C���f�b�N�X�̐��� 32767 �𒴂���ꍇ�͕s�K��
	if( TotalVertexNum > 32767 || TotalIndexNum > 32767 || TotalMeshVertexNum > 32767 ) return 0 ;

	// ���b�V���������Ă���t���[�����ЂƂȉ��������ꍇ�̓R���o�C�����b�V�����g���K�v�������̂ŕs�K��
	if( MeshFrameNum <= 1 ) return 0 ;

	// �m�ۃ������̃T�C�Y���v�Z����
	AllocMemSize =
		TotalVertexNum     * ( BaseMesh->TriangleList->PosUnitSize + sizeof( DWORD ) ) +
		TotalIndexNum      * sizeof( unsigned short ) +
		TotalMeshVertexNum * BaseMesh->VertUnitSize +
		sizeof( MV1_TRIANGLE_LIST_BASE ) +
		sizeof( MV1_MESH_BASE ) +
		sizeof( MV1_VERTEXBUFFER ) ;

	// �������̊m��
	Frame->CombineMesh = ( MV1_MESH_BASE * )DXALLOC( AllocMemSize ) ;
	if( Frame->CombineMesh == NULL ) return 0 ;
	_MEMSET( Frame->CombineMesh, 0, AllocMemSize ) ;

	// �������A�h���X�̃Z�b�g
	CMesh = Frame->CombineMesh ;
	CTList = ( MV1_TRIANGLE_LIST_BASE * )( CMesh + 1 ) ;
	CMesh->VertexNum = TotalMeshVertexNum ;
	CMesh->Vertex = ( MV1_MESH_VERTEX * )( CTList + 1 ) ;
	CTList->MeshVertexIndex = ( DWORD * )( ( BYTE * )CMesh->Vertex + TotalMeshVertexNum * MBMesh->VertUnitSize ) ;
	CTList->NormalPosition = ( MV1_LIST_NORMAL_POS * )( ( BYTE * )

	// ���b�V���̏����Z�b�g
	*CMesh = *BaseMesh ;
	CMesh->TriangleListNum = 1 ;
	CMesh->TriangleList = CTList ;
	CMesh->VertexAllocMem = FALSE ;
	CMesh->FaceNum = 0 ;
	CMesh->Face = NULL ;

	// �g���C�A���O�����X�g�̏����Z�b�g
	CTList->Container = CMesh ;
	CTList->VertexType = MBMesh->TriangleList->VertexType ;
	CTList->PosUnitSize = MBMesh->TriangleList->PosUnitSize ;
	CTList->TempUnitSize = MBMesh->TriangleList->TempUnitSize ;
	CTList->VertexNum = TotalVertexNum ;
	CTList->MaxBoneNum = 0 ;
	CTList->UseBoneNum = 0 ;

	// ���b�V�������_�����O�ɕK�v�̖����f�[�^�̓N���A
	TCMesh.VertexNum      = 0 ;
	TCMesh.VertUnitSize   = 0 ;
	TCMesh.Vertex         = NULL ;
	TCMesh.VertexAllocMem = FALSE ;
	TCMesh.FaceNum        = 0 ;
	TCMesh.Face           = NULL ;

	// �g���C�A���O�����X�g�͈����
	TCMesh.TriangleListNum = 1 ;

	// �R���o�C���g���C�A���O�����X�g�̉�������
	_MEMSET( &TCTList, 0, sizeof( TCTList ) ) ;
	MBTList = BaseMesh->TriangleList ;
	TCTList.VertexType = MBTList->VertexType ;

	Frame->IsCombineMesh = TRUE ;

}

#endif

// �Q�Ɨp���b�V���̍ő�l�ƍŏ��l���X�V����
static void _MV1SetupReferenceMeshMaxAndMinPosition( MV1_REF_POLYGONLIST *PolyList )
{
	MV1_REF_POLYGON *Poly ;
	int i ;
	VECTOR *V0, *V1, *V2 ;

	Poly = PolyList->Polygons ;
	PolyList->MinPosition = PolyList->MaxPosition = PolyList->Vertexs[ Poly->VIndex[ 0 ] ].Position ;
	for( i = 0 ; i < PolyList->PolygonNum ; i ++, Poly ++ )
	{
		V0 = &PolyList->Vertexs[ Poly->VIndex[ 0 ] ].Position ;
		V1 = &PolyList->Vertexs[ Poly->VIndex[ 1 ] ].Position ;
		V2 = &PolyList->Vertexs[ Poly->VIndex[ 2 ] ].Position ;
		if( V0->x > V1->x )
		{
			Poly->MaxPosition.x = V0->x < V2->x ? V2->x : V0->x ;
			Poly->MinPosition.x = V1->x > V2->x ? V2->x : V1->x ;
		}
		else
		{
			Poly->MaxPosition.x = V1->x < V2->x ? V2->x : V1->x ;
			Poly->MinPosition.x = V0->x > V2->x ? V2->x : V0->x ;
		}

		if( V0->y > V1->y )
		{
			Poly->MaxPosition.y = V0->y < V2->y ? V2->y : V0->y ;
			Poly->MinPosition.y = V1->y > V2->y ? V2->y : V1->y ;
		}
		else
		{
			Poly->MaxPosition.y = V1->y < V2->y ? V2->y : V1->y ;
			Poly->MinPosition.y = V0->y > V2->y ? V2->y : V0->y ;
		}

		if( V0->z > V1->z )
		{
			Poly->MaxPosition.z = V0->z < V2->z ? V2->z : V0->z ;
			Poly->MinPosition.z = V1->z > V2->z ? V2->z : V1->z ;
		}
		else
		{
			Poly->MaxPosition.z = V1->z < V2->z ? V2->z : V1->z ;
			Poly->MinPosition.z = V0->z > V2->z ? V2->z : V0->z ;
		}

		if( Poly->MaxPosition.x > PolyList->MaxPosition.x ) PolyList->MaxPosition.x = Poly->MaxPosition.x ;
		if( Poly->MinPosition.x < PolyList->MinPosition.x ) PolyList->MinPosition.x = Poly->MinPosition.x ;
		if( Poly->MaxPosition.y > PolyList->MaxPosition.y ) PolyList->MaxPosition.y = Poly->MaxPosition.y ;
		if( Poly->MinPosition.y < PolyList->MinPosition.y ) PolyList->MinPosition.y = Poly->MinPosition.y ;
		if( Poly->MaxPosition.z > PolyList->MaxPosition.z ) PolyList->MaxPosition.z = Poly->MaxPosition.z ;
		if( Poly->MinPosition.z < PolyList->MinPosition.z ) PolyList->MinPosition.z = Poly->MinPosition.z ;
	}
}

// �}���`�o�C�g���������烏�C�h���������쐬����
static bool _MV1CreateWideCharNameBase( MV1_MODEL_BASE *MBase, const char *NameA, wchar_t **NameWP )
{
	wchar_t TempName[ 512 ] ;

	ConvString( NameA, CHAR_CHARCODEFORMAT, ( char * )TempName, sizeof( TempName ), WCHAR_T_CHARCODEFORMAT ) ;
	*NameWP = ( wchar_t * )ADDMEMAREA( ( _WCSLEN( TempName ) + 1 ) * sizeof( wchar_t ), &MBase->AddFirstMem ) ;
	if( *NameWP == NULL )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\xef\x30\xa4\x30\xc9\x30\x87\x65\x57\x5b\xdd\x4f\x58\x5b\x28\x75\x6e\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x28\x00\x11\xff\x29\x00\x00"/*@ L"���C�h�����ۑ��p�̃������̈�̊m�ۂɎ��s���܂���(�P)" @*/ )) ;
		return false ;
	}
	_WCSCPY( *NameWP, TempName ) ;

	return true ;
}

// ���C�h����������}���`�o�C�g���������쐬����
static bool _MV1CreateMultiByteNameBase( MV1_MODEL_BASE *MBase, const wchar_t *NameW, char **NameAP )
{
	char TempName[ 512 ] ;

	ConvString( ( const char * )NameW, WCHAR_T_CHARCODEFORMAT, TempName, sizeof( TempName ), CHAR_CHARCODEFORMAT ) ;
	*NameAP = ( char * )ADDMEMAREA( _STRLEN( TempName ) + 1, &MBase->AddFirstMem ) ;
	if( *NameAP == NULL )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\xde\x30\xeb\x30\xc1\x30\xd0\x30\xa4\x30\xc8\x30\x87\x65\x57\x5b\xdd\x4f\x58\x5b\x28\x75\x6e\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x28\x00\x11\xff\x29\x00\x00"/*@ L"�}���`�o�C�g�����ۑ��p�̃������̈�̊m�ۂɎ��s���܂���(�P)" @*/ )) ;
		return false ;
	}
	_STRCPY( *NameAP, TempName ) ;

	return true ;
}

// �t�@�C���p�X����t�@�C�����ƃf�B���N�g���p�X���쐬����
static void _MV1CreateFileNameAndCurrentDirectory( const wchar_t *FilePath, wchar_t *FileName, size_t FileNameBytes, wchar_t *CurrentDirectory, size_t CurrentDirectoryBytes )
{
	wchar_t FileName2[ FILEPATH_MAX ], *cpt ;
	int i, Last = 0 ;

	// ���f���t�@�C���̂���f�B���N�g���p�X���쐬����
	if( _WCSCHR( FilePath, L'\\' ) != NULL || _WCSCHR( FilePath, L'/' ) != NULL || _WCSCHR( FilePath, L':' ) != NULL )
	{
		wchar_t TempDirectory[ 1024 ] ;

		// �Ō�Ɂw\�x���́w/�x���o�Ă���Ƃ����T��
		for( i = 0 ; FilePath[ i ] != L'\0' ; )
		{
			if( CHECK_WCHAR_T_DOUBLE( FilePath[ i ] ) )
			{
				i += 2 ;
			}
			else
			{
				if( FilePath[ i ] == L'\\' || 
					FilePath[ i ] == L'/' || 
					FilePath[ i ] == L':' ) Last = i ;
				i ++ ;
			}
		}

		// �f�B���N�g���w�蕔���݂̂��R�s�[
		_WCSNCPY_S( TempDirectory, sizeof( TempDirectory ), FilePath, Last + 1 + 1 ) ;

		// �I�[�������Z�b�g
		if( ( TempDirectory[ Last ] == L'/' || TempDirectory[ Last ] == L'\\' ) && ( Last == 0 || TempDirectory[ Last - 1 ] != L':' ) )
		{
			TempDirectory[ Last ] = L'\0' ;
		}
		else
		{
			TempDirectory[ Last + 1 ] = L'\0' ;
		}

		// �f�B���N�g�����t���p�X�ɂ���
		ConvertFullPathW_( TempDirectory, CurrentDirectory, CurrentDirectoryBytes ) ;

		// �t�@�C���������݂̂��R�s�[
		_WCSCPY_S( FileName2, sizeof( FileName2 ), &FilePath[ Last + 1 ] ) ;
	}
	else
	{
		// �J�����g�f�B���N�g���𓾂�
		DX_FGETDIR( CurrentDirectory, CurrentDirectoryBytes ) ;

		// �t�@�C�������R�s�[
		_WCSCPY_S( FileName2, sizeof( FileName2 ), FilePath ) ;
	}

	// �g���q�𔲂����t�@�C�����𓾂�
	_WCSCPY_S( FileName, FileNameBytes, FileName2 ) ;
	cpt = ( wchar_t * )_WCSRCHR( FileName, L'.' ) ;
	if( cpt ) *cpt = L'\0' ;
}

// �}���`�o�C�g���������烏�C�h���������쐬����
static bool _MV1CreateWideCharName( const char *NameA, wchar_t **NameWP )
{
	wchar_t TempName[ 512 ] ;

	ConvString( NameA, CHAR_CHARCODEFORMAT, ( char * )TempName, sizeof( TempName ), WCHAR_T_CHARCODEFORMAT ) ;

	*NameWP = ( wchar_t * )DXALLOC( ( _WCSLEN( TempName ) + 1 ) * sizeof( wchar_t ) ) ;
	if( *NameWP == NULL )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\xef\x30\xa4\x30\xc9\x30\x87\x65\x57\x5b\xdd\x4f\x58\x5b\x28\x75\x6e\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x28\x00\x12\xff\x29\x00\x00"/*@ L"���C�h�����ۑ��p�̃������̈�̊m�ۂɎ��s���܂���(�Q)" @*/ )) ;
		return false ;
	}
	_WCSCPY( *NameWP, TempName ) ;

	return true ;
}

// ���C�h����������}���`�o�C�g���������쐬����
static bool _MV1CreateMultiByteName( const wchar_t *NameW, char **NameAP )
{
	char TempName[ 512 ] ;

	ConvString( ( const char * )NameW, WCHAR_T_CHARCODEFORMAT, TempName, sizeof( TempName ), CHAR_CHARCODEFORMAT ) ;

	*NameAP = ( char * )DXALLOC( _STRLEN( TempName ) + 1 ) ;
	if( *NameAP == NULL )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\xde\x30\xeb\x30\xc1\x30\xd0\x30\xa4\x30\xc8\x30\x87\x65\x57\x5b\xdd\x4f\x58\x5b\x28\x75\x6e\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x28\x00\x12\xff\x29\x00\x00"/*@ L"�}���`�o�C�g�����ۑ��p�̃������̈�̊m�ۂɎ��s���܂���(�Q)" @*/ )) ;
		return false ;
	}
	_STRCPY( *NameAP, TempName ) ;

	return true ;
}

// �}���`�o�C�g��������V���Ƀ��������m�ۂ��ăR�s�[����
static bool _MV1AllocAndMultiByteNameCopy( const char *NameA, char **NameAP )
{
	*NameAP = ( char * )DXALLOC( ( _STRLEN( NameA ) + 1 ) * sizeof( char ) ) ;
	if( *NameAP == NULL )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\xde\x30\xeb\x30\xc1\x30\xd0\x30\xa4\x30\xc8\x30\x87\x65\x57\x5b\xdd\x4f\x58\x5b\x28\x75\x6e\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x28\x00\x13\xff\x29\x00\x00"/*@ L"�}���`�o�C�g�����ۑ��p�̃������̈�̊m�ۂɎ��s���܂���(�R)" @*/ )) ;
		return false ;
	}
	_STRCPY( *NameAP, NameA ) ;

	return true ;
}

// ���C�h��������V���Ƀ��������m�ۂ��ăR�s�[����
static bool _MV1AllocAndWideCharNameCopy( const wchar_t *NameW, wchar_t **NameWP )
{
	*NameWP = ( wchar_t * )DXALLOC( ( _WCSLEN( NameW ) + 1 ) * sizeof( wchar_t ) ) ;
	if( *NameWP == NULL )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\xef\x30\xa4\x30\xc9\x30\x87\x65\x57\x5b\xdd\x4f\x58\x5b\x28\x75\x6e\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x28\x00\x13\xff\x29\x00\x00"/*@ L"���C�h�����ۑ��p�̃������̈�̊m�ۂɎ��s���܂���(�R)" @*/ )) ;
		return false ;
	}
	_WCSCPY( *NameWP, NameW ) ;

	return true ;
}

extern int __MV1LoadTexture(
	void **ColorImage, int *ColorImageSize,
	void **AlphaImage, int *AlphaImageSize,
	int *GraphHandle,
	int *SemiTransFlag,
	int *DefaultTextureFlag,
#ifndef UNICODE
	char    **ColorFilePathAMem, char    **AlphaFilePathAMem,
#endif
	wchar_t **ColorFilePathWMem, wchar_t **AlphaFilePathWMem,
	const wchar_t *ColorFilePath, const wchar_t *AlphaFilePath, const wchar_t *StartFolderPath,
	int BumpImageFlag, float BumpImageNextPixelLength,
	int ReverseFlag,
	int Bmp32AllZeroAlphaToXRGB8Flag,
	const MV1_FILE_READ_FUNC *FileReadFunc,
	bool ValidImageAddr,
	int NotInitGraphDelete,
	int ASyncThread
	)
{
	BASEIMAGE ColorBaseImage, AlphaBaseImage ;
#ifndef UNICODE
	char TempPath[ 1024 ] ;
#endif
	int i/*, j*/, Result ;
	void *DataAddr ;

	// �|�C���^�̏�����
	if( ValidImageAddr == false )
	{
#ifndef UNICODE
		if( ColorFilePathAMem ) *ColorFilePathAMem = NULL ;
		if( AlphaFilePathAMem ) *AlphaFilePathAMem = NULL ;
#endif
		if( ColorFilePathWMem ) *ColorFilePathWMem = NULL ;
		if( AlphaFilePathWMem ) *AlphaFilePathWMem = NULL ;
		*AlphaImage = NULL ;
		*ColorImage = NULL ;
	}
	*GraphHandle = -1 ;
	*DefaultTextureFlag = FALSE ;
	AlphaBaseImage.GraphData = NULL ;
	ColorBaseImage.GraphData = NULL ;

	if( ValidImageAddr == false )
	{
#ifndef UNICODE
		char    RelativePathA[ 1024 ] ;
		int StrLengthA ;
#endif
		wchar_t RelativePathW[ 1024 ] ;
		int StrLengthW ;

		// �t�@�C���p�X��ۑ����郁�����̈�̊m��
		if( ColorFilePath && ColorFilePathWMem && StartFolderPath )
		{
			CreateRelativePathW_( ColorFilePath, StartFolderPath, RelativePathW, sizeof( RelativePathW ) ) ;
			StrLengthW = ( int )_WCSLEN( RelativePathW ) ;

#ifndef UNICODE
			ConvString( ( const char * )RelativePathW, WCHAR_T_CHARCODEFORMAT, RelativePathA, sizeof( RelativePathA ), CHAR_CHARCODEFORMAT ) ;
			StrLengthA = ( int )_STRLEN( RelativePathA ) ;

			*ColorFilePathAMem = ( char * )DXALLOC( ( size_t )( ( StrLengthA + 1 ) * sizeof( char ) ) ) ;
			if( *ColorFilePathAMem == NULL )
			{
				DXST_ERRORLOGFMT_ADDUTF16LE(( "\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x54\x00\x65\x00\x78\x00\x74\x00\x75\x00\x72\x00\x65\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\x28\x75\x6e\x30\xab\x30\xe9\x30\xfc\x30\x3b\x75\xcf\x50\xd5\x30\xa1\x30\xa4\x30\xeb\x30\xd1\x30\xb9\x30\x92\x30\xdd\x4f\x58\x5b\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Load Texture Error : �e�N�X�`���p�̃J���[�摜�t�@�C���p�X��ۑ����郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
				goto ERRORLABEL ;
			}
			_STRCPY( *ColorFilePathAMem, RelativePathA ) ;
#endif
			*ColorFilePathWMem = ( wchar_t * )DXALLOC( ( size_t )( ( StrLengthW + 1 ) * sizeof( wchar_t ) ) ) ;
			if( *ColorFilePathWMem == NULL )
			{
				DXST_ERRORLOGFMT_ADDUTF16LE(( "\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x54\x00\x65\x00\x78\x00\x74\x00\x75\x00\x72\x00\x65\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\x28\x75\x6e\x30\xab\x30\xe9\x30\xfc\x30\x3b\x75\xcf\x50\xd5\x30\xa1\x30\xa4\x30\xeb\x30\xd1\x30\xb9\x30\x92\x30\xdd\x4f\x58\x5b\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Load Texture Error : �e�N�X�`���p�̃J���[�摜�t�@�C���p�X��ۑ����郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
				goto ERRORLABEL ;
			}
			_WCSCPY( *ColorFilePathWMem, RelativePathW ) ;
		}
		if( AlphaFilePath && AlphaFilePathWMem && StartFolderPath )
		{
			CreateRelativePathW_( AlphaFilePath, StartFolderPath, RelativePathW, sizeof( RelativePathW ) ) ;
			StrLengthW = ( int )_WCSLEN( RelativePathW ) ;

#ifndef UNICODE
			ConvString( ( const char * )RelativePathW, WCHAR_T_CHARCODEFORMAT, RelativePathA, sizeof( RelativePathA ), CHAR_CHARCODEFORMAT ) ;
			StrLengthA = ( int )_STRLEN( RelativePathA ) ;

			*AlphaFilePathAMem = ( char * )DXALLOC( ( size_t )( ( StrLengthA + 1 ) * sizeof( char ) ) ) ;
			if( AlphaFilePathAMem == NULL )
			{
				DXST_ERRORLOGFMT_ADDUTF16LE(( "\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x54\x00\x65\x00\x78\x00\x74\x00\x75\x00\x72\x00\x65\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\x28\x75\x6e\x30\xa2\x30\xeb\x30\xd5\x30\xa1\x30\x3b\x75\xcf\x50\xd5\x30\xa1\x30\xa4\x30\xeb\x30\xd1\x30\xb9\x30\x92\x30\xdd\x4f\x58\x5b\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Load Texture Error : �e�N�X�`���p�̃A���t�@�摜�t�@�C���p�X��ۑ����郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
				goto ERRORLABEL ;
			}
			_STRCPY( *AlphaFilePathAMem, RelativePathA ) ;
#endif

			*AlphaFilePathWMem = ( wchar_t * )DXALLOC( ( size_t )( ( StrLengthW + 1 ) * sizeof( wchar_t ) ) ) ;
			if( AlphaFilePathWMem == NULL )
			{
				DXST_ERRORLOGFMT_ADDUTF16LE(( "\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x54\x00\x65\x00\x78\x00\x74\x00\x75\x00\x72\x00\x65\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\x28\x75\x6e\x30\xa2\x30\xeb\x30\xd5\x30\xa1\x30\x3b\x75\xcf\x50\xd5\x30\xa1\x30\xa4\x30\xeb\x30\xd1\x30\xb9\x30\x92\x30\xdd\x4f\x58\x5b\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Load Texture Error : �e�N�X�`���p�̃A���t�@�摜�t�@�C���p�X��ۑ����郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
				goto ERRORLABEL ;
			}
			_WCSCPY( *AlphaFilePathWMem, RelativePathW ) ;
		}

		// �J���[�e�N�X�`���t�@�C����ǂݍ���
		*ColorImageSize = 0 ;
		if( ColorFilePath )
		{
			if( FileReadFunc )
			{
#ifdef UNICODE
				Result = FileReadFunc->Read( ColorFilePath, &DataAddr, ColorImageSize, FileReadFunc->Data ) ;
#else
				ConvString( ( const char * )ColorFilePath, WCHAR_T_CHARCODEFORMAT, TempPath, sizeof( TempPath ), CHAR_CHARCODEFORMAT ) ;
				Result = FileReadFunc->Read( TempPath, &DataAddr, ColorImageSize, FileReadFunc->Data ) ;
#endif
				if( Result != -1 )
				{
					*ColorImage = DXALLOC( ( size_t )( *ColorImageSize ) ) ;
					if( *ColorImage == NULL )
					{
#ifndef DX_GCC_COMPILE
						DXST_ERRORLOGFMT_ADDW(( L"Load Texture Error : Color Channel Image File : Memory Alloc Error : %s\n", ColorFilePath ) ) ;
#endif
						Result = -1 ;
					}
					else
					{
						_MEMCPY( *ColorImage, DataAddr, ( size_t )( *ColorImageSize ) ) ;
					}
					if( FileReadFunc->Release )
					{
						FileReadFunc->Release( DataAddr, FileReadFunc->Data ) ;
					}
				}
			}
			else
			{
				Result = MV1RLoadFileW( ColorFilePath, ColorImage, ColorImageSize ) ;
			}

			if( Result == -1 )
			{
#ifndef DX_GCC_COMPILE
				DXST_ERRORLOGFMT_ADDW(( L"Load Texture Error : Color Channel Image File : Read Error : %s\n", ColorFilePath ) ) ;
#endif

				// �ǂݍ��݂Ɏ��s�����ꍇ�̓G���[���p�e�N�X�`�����[�Ă�
				*ColorImage = DXALLOC( sizeof( Tga8x8TextureFileImage ) ) ;
				if( *ColorImage == NULL )
				{
					DXST_ERRORLOGFMT_ADDUTF16LE(( "\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x54\x00\x65\x00\x78\x00\x74\x00\x75\x00\x72\x00\x65\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xa8\x30\xe9\x30\xfc\x30\xde\x56\x7f\x90\x28\x75\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x00"/*@ L"Load Texture Error : �G���[���p�e�N�X�`�����i�[����̈�̊m�ۂɎ��s���܂���" @*/ )) ;
					goto ERRORLABEL ;
				}
				_MEMCPY( *ColorImage, Tga8x8TextureFileImage, sizeof( Tga8x8TextureFileImage ) ) ;
				*ColorImageSize = sizeof( Tga8x8TextureFileImage ) ;

				*DefaultTextureFlag = TRUE ;
			}
		}

		// �A���t�@�`�����l���p�摜�t�@�C����ǂݍ���
		*AlphaImageSize = 0 ;
		if( AlphaFilePath )
		{
			if( FileReadFunc )
			{
#ifdef UNICODE
				Result = FileReadFunc->Read( AlphaFilePath, &DataAddr, AlphaImageSize, FileReadFunc->Data ) ;
#else
				ConvString( ( const char * )AlphaFilePath, WCHAR_T_CHARCODEFORMAT, TempPath, sizeof( TempPath ), CHAR_CHARCODEFORMAT ) ;
				Result = FileReadFunc->Read( TempPath, &DataAddr, AlphaImageSize, FileReadFunc->Data ) ;
#endif

				if( Result != -1 )
				{
					*AlphaImage = DXALLOC( ( size_t )( *AlphaImageSize ) ) ;
					if( *AlphaImage == NULL )
					{
#ifndef DX_GCC_COMPILE
						DXST_ERRORLOGFMT_ADDW(( L"Load Texture Error : Alpha Channel Image : Memory Alloc Error : %s\n", AlphaFilePath ) ) ;
#endif
						Result = -1 ;
					}
					else
					{
						_MEMCPY( *AlphaImage, DataAddr, ( size_t )( *AlphaImageSize ) ) ;
					}
					if( FileReadFunc->Release )
					{
						FileReadFunc->Release( DataAddr, FileReadFunc->Data ) ;
					}
				}
			}
			else
			{
				Result = MV1RLoadFileW( AlphaFilePath, AlphaImage, AlphaImageSize ) ;
			}

			if( Result == -1 )
			{
#ifndef DX_GCC_COMPILE
				DXST_ERRORLOGFMT_ADDW(( L"Load Texture Error : Alpha Channel Image : Read Error : %s\n", AlphaFilePath ) ) ;
#endif

				// �ǂݍ��݂Ɏ��s�����ꍇ�̓G���[���p�e�N�X�`�����[�Ă�
				*AlphaImage = DXALLOC( sizeof( Tga8x8TextureFileImage ) ) ;
				if( *AlphaImage == NULL )
				{
					DXST_ERRORLOGFMT_ADDUTF16LE(( "\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x54\x00\x65\x00\x78\x00\x74\x00\x75\x00\x72\x00\x65\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xa8\x30\xe9\x30\xfc\x30\xde\x56\x7f\x90\x28\x75\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x00"/*@ L"Load Texture Error : �G���[���p�e�N�X�`�����i�[����̈�̊m�ۂɎ��s���܂���" @*/ )) ;
					goto ERRORLABEL ;
				}
				_MEMCPY( *AlphaImage, Tga8x8TextureFileImage, sizeof( Tga8x8TextureFileImage ) ) ;
				*AlphaImageSize = sizeof( Tga8x8TextureFileImage ) ;
			}
		}
		else
		{
			// ���`�����l���p�̃t�@�C���p�X�������ꍇ�̓J���[�e�N�X�`���t�@�C���̖��O�� _a ���������t�@�C�������������ׂ�
			wchar_t TempAlphaFilePath[ 1024 ] ;
			int len ;

			_WCSCPY_S( TempAlphaFilePath, sizeof( TempAlphaFilePath ), ColorFilePath ) ;
			len = ( int )_WCSLEN( TempAlphaFilePath ) ;
			for( i = len - 1 ; i >= 0 && TempAlphaFilePath[ i ] != L'.' && TempAlphaFilePath[ i ] != L'\0' && TempAlphaFilePath[ i ] != L'\\' && TempAlphaFilePath[ i ] != L':' ; i -- ){}
			if( TempAlphaFilePath[ i ] == L'.' )
			{
				if( i < sizeof( TempAlphaFilePath ) / sizeof( wchar_t ) - 3 )
				{
					_WCSCPY_S( &TempAlphaFilePath[ i     ], sizeof( TempAlphaFilePath ) -   i       * sizeof( wchar_t ), L"_a" ) ;
					_WCSCPY_S( &TempAlphaFilePath[ i + 2 ], sizeof( TempAlphaFilePath ) - ( i - 2 ) * sizeof( wchar_t ), &ColorFilePath[ i ] ) ;
				}
			}
			else
			{
				_WCSCAT_S( TempAlphaFilePath, sizeof( TempAlphaFilePath ), L"_a" ) ;
			}

			if( FileReadFunc )
			{
#ifdef UNICODE
				Result = FileReadFunc->Read( TempAlphaFilePath, &DataAddr, AlphaImageSize, FileReadFunc->Data ) ;
#else
				ConvString( ( const char * )TempAlphaFilePath, WCHAR_T_CHARCODEFORMAT, TempPath, sizeof( TempPath ), CHAR_CHARCODEFORMAT ) ;
				Result = FileReadFunc->Read( TempPath, &DataAddr, AlphaImageSize, FileReadFunc->Data ) ;
#endif
				if( Result != -1 )
				{
					*AlphaImage = DXALLOC( ( size_t )( *AlphaImageSize ) ) ;
					if( *AlphaImage == NULL )
					{
#ifndef DX_GCC_COMPILE
						DXST_ERRORLOGFMT_ADDW(( L"Load Texture Error : Alpha Channel Image File : Memory Alloc Error : %s\n", AlphaFilePath ) ) ;
#endif
						Result = -1 ;
					}
					else
					{
						_MEMCPY( *AlphaImage, DataAddr, ( size_t )( *AlphaImageSize ) ) ;
					}
					if( FileReadFunc->Release )
					{
						FileReadFunc->Release( DataAddr, FileReadFunc->Data ) ;
					}
				}
			}
			else
			{
				Result = MV1RLoadFileW( TempAlphaFilePath, AlphaImage, AlphaImageSize ) ;
			}
		}
	}

	// BASEIMAGE ���\�z
	SetBmp32AllZeroAlphaToXRGB8( Bmp32AllZeroAlphaToXRGB8Flag ) ;
	if( MV1CreateTextureColorBaseImage(
				&ColorBaseImage,
				&AlphaBaseImage,
				*ColorImage, *ColorImageSize,
				*AlphaImage, *AlphaImageSize,
				BumpImageFlag, BumpImageNextPixelLength, ReverseFlag ) == -1 )
	{
		SetBmp32AllZeroAlphaToXRGB8( FALSE ) ;
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x54\x00\x65\x00\x78\x00\x74\x00\x75\x00\x72\x00\x65\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\x28\x75\x6e\x30\x20\x00\x42\x00\x41\x00\x53\x00\x45\x00\x49\x00\x4d\x00\x41\x00\x47\x00\x45\x00\x20\x00\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Load Texture Error : �e�N�X�`���p�� BASEIMAGE �̍쐬�Ɏ��s���܂���\n" @*/ )) ;
		goto ERRORLABEL ;
	}
	SetBmp32AllZeroAlphaToXRGB8( FALSE ) ;

	// �n���h���𓾂�
	LOADGRAPH_GPARAM GParam ;
	Graphics_Image_InitLoadGraphGParam( &GParam ) ;
	GParam.LoadBaseImageGParam.ConvertPremultipliedAlpha = FALSE ;
	GParam.CreateGraphGParam.NotUseTransColor = TRUE ;
	GParam.CreateGraphGParam.InitHandleGParam.MipMapCount = -1 ;
	GParam.CreateGraphGParam.InitHandleGParam.NotInitGraphDelete = NotInitGraphDelete ;
	*GraphHandle = Graphics_Image_CreateGraphFromGraphImage_UseGParam( &GParam, FALSE, -1, &ColorBaseImage, *AlphaImage ? &AlphaBaseImage : NULL, TRUE, FALSE, FALSE, ASyncThread ) ;
	if( *GraphHandle < 0 )
	{
		NS_ReleaseBaseImage( &ColorBaseImage ) ;
		if( AlphaImage ) NS_ReleaseBaseImage( &AlphaBaseImage ) ;
		goto ERRORLABEL ;
	}

	// �������v�f�����邩�ǂ����𒲂ׂ�
	*SemiTransFlag = FALSE ;
	if( *AlphaImage ) *SemiTransFlag = TRUE ;
	if( *SemiTransFlag == FALSE && ColorBaseImage.ColorData.AlphaWidth != 0 )
	{
//		int r, g, b, a ;

		// �t�H�[�}�b�g���W���t�H�[�}�b�g�ł͂Ȃ�������DXT�t�H�[�}�b�g�Ŕ��f����
		if( ColorBaseImage.ColorData.Format != DX_BASEIMAGE_FORMAT_NORMAL )
		{
			//NS_ConvertNormalFormatBaseImage( &ColorBaseImage ) ;
			switch( ColorBaseImage.ColorData.Format )
			{
			case DX_BASEIMAGE_FORMAT_DXT1 :
				*SemiTransFlag = FALSE ;
				break ;

			case DX_BASEIMAGE_FORMAT_DXT2 :
				*SemiTransFlag = TRUE ;
				break ;

			case DX_BASEIMAGE_FORMAT_DXT3 :
				*SemiTransFlag = TRUE ;
				break ;

			case DX_BASEIMAGE_FORMAT_DXT4 :
				*SemiTransFlag = TRUE ;
				break ;

			case DX_BASEIMAGE_FORMAT_DXT5 :
				*SemiTransFlag = TRUE ;
				break ;
			}
		}
/*
		for( i = 0 ; i < ColorBaseImage.Height ; i ++ )
		{
			for( j = 0 ; j < ColorBaseImage.Width ; j ++ )
			{
				NS_GetPixelBaseImage( &ColorBaseImage, j, i, &r, &g, &b, &a ) ;
				if( a != 255 )
				{
					*SemiTransFlag = TRUE ;
					break ;
				}
			}
			if( *SemiTransFlag ) break ;
		}
*/
		else
		{
			// �S�s�N�Z���𒲂ׂĔ������v�f�����邩�ǂ����𒲂ׂ�
			*SemiTransFlag = NS_CheckPixelAlphaBaseImage( &ColorBaseImage ) <= 1 ? 0 : 1 ; 
		}
	}

	// ��{�C���[�W�͉��
	NS_ReleaseBaseImage( &ColorBaseImage ) ;
	if( *AlphaImage ) NS_ReleaseBaseImage( &AlphaBaseImage ) ;

	// �I��
	return 0 ;

ERRORLABEL :
	if( ValidImageAddr == false )
	{
#ifndef UNICODE
		if( ColorFilePathAMem && *ColorFilePathAMem )
		{
			DXFREE( *ColorFilePathAMem ) ;
			*ColorFilePathAMem = NULL ;
		}
		if( AlphaFilePathAMem && *AlphaFilePathAMem )
		{
			DXFREE( *AlphaFilePathAMem ) ;
			*AlphaFilePathAMem = NULL ;
		}
#endif
		if( ColorFilePathWMem && *ColorFilePathWMem )
		{
			DXFREE( *ColorFilePathWMem ) ;
			*ColorFilePathWMem = NULL ;
		}
		if( AlphaFilePathWMem && *AlphaFilePathWMem )
		{
			DXFREE( *AlphaFilePathWMem ) ;
			*AlphaFilePathWMem = NULL ;
		}
		if( *ColorImage )
		{
			DXFREE( *ColorImage ) ;
			*ColorImage = NULL ;
		}
		if( *AlphaImage )
		{
			DXFREE( *AlphaImage ) ;
			*AlphaImage = NULL ;
		}
	}
	if( *GraphHandle != -1 )
	{
		NS_DeleteGraph( *GraphHandle ) ;
		*GraphHandle = -1 ;
	}

	// �G���[�I��
	return -1 ;
}


// ���݂̐ݒ�Ńe�N�X�`����ǂݍ���
static int _MV1TextureLoadBase(
	MV1_MODEL_BASE *ModelBase, MV1_TEXTURE_BASE *Texture,
	const wchar_t *ColorFilePathW, const wchar_t *AlphaFilePathW,
	int BumpImageFlag, float BumpImageNextPixelLength,
	bool ReverseFlag,
	bool Bmp32AllZeroAlphaToXRGB8Flag,
	int ASyncThread )
{
	void *ColorImage, *AlphaImage ;
	wchar_t *ColorFilePathWMem, *AlphaFilePathWMem ;
#ifndef UNICODE
	char    *ColorFilePathAMem, *AlphaFilePathAMem ;
#endif
	int ColorImageSize, AlphaImageSize ;
	int GraphHandle, SemiTransFlag, DefaultTextureFlag ;
	wchar_t *DirW ;
	int Result ;

	DirW = ModelBase->DirectoryPath ;

	// �e�N�X�`���̓ǂݍ���
	Result =  __MV1LoadTexture(
					&ColorImage, &ColorImageSize,
					&AlphaImage, &AlphaImageSize,
					&GraphHandle, &SemiTransFlag, &DefaultTextureFlag,
#ifndef UNICODE
					&ColorFilePathAMem, &AlphaFilePathAMem,
#endif
					&ColorFilePathWMem, &AlphaFilePathWMem,
					ColorFilePathW, AlphaFilePathW, DirW,
					BumpImageFlag, BumpImageNextPixelLength,
					ReverseFlag,
					Bmp32AllZeroAlphaToXRGB8Flag,
					NULL,
					false,
					TRUE,
					ASyncThread
			) ;

	if( Result == -1 )
	{
		DXST_ERRORLOGFMT_ADDW(( L"Load Texture Error : Texture File : Load Error : %s\n", Texture->NameW ) ) ;
		return -1 ;
	}

	// ���ɃC���[�W��ǂݍ���ł����烁�������������
	if( Texture->ColorImage )
	{
		DXFREE( Texture->ColorImage ) ;
		Texture->ColorImage = NULL ;
	}
	if( Texture->AlphaImage )
	{
		DXFREE( Texture->AlphaImage ) ;
		Texture->AlphaImage = NULL ;
	}

	// �V�����������A�h���X��ۑ�
	Texture->ColorImage = ColorImage ;
	Texture->ColorImageSize = ColorImageSize ;

	Texture->AlphaImage = AlphaImage ;
	Texture->AlphaImageSize = AlphaImageSize ;

	// �t�@�C���p�X��ۑ�
	if( Texture->ColorImage )
	{
		if( Texture->ColorImageFilePathAllocMem )
		{
#ifndef UNICODE
			if( Texture->ColorFilePathA )
			{
				DXFREE( Texture->ColorFilePathA ) ;
				Texture->ColorFilePathA = NULL ;
			}
#endif
			if( Texture->ColorFilePathW )
			{
				DXFREE( Texture->ColorFilePathW ) ;
				Texture->ColorFilePathW = NULL ;
			}
		}
#ifndef UNICODE
		Texture->ColorFilePathA = ColorFilePathAMem ;
#endif
		Texture->ColorFilePathW = ColorFilePathWMem ;
		Texture->ColorImageFilePathAllocMem = TRUE ;
	}
	if( Texture->AlphaImage )
	{
		if( Texture->AlphaImageFilePathAllocMem )
		{
#ifndef UNICODE
			if( Texture->AlphaFilePathA )
			{
				DXFREE( Texture->AlphaFilePathA ) ;
				Texture->AlphaFilePathA = NULL ;
			}
#endif
			if( Texture->AlphaFilePathW )
			{
				DXFREE( Texture->AlphaFilePathW ) ;
				Texture->AlphaFilePathW = NULL ;
			}
		}
#ifndef UNICODE
		Texture->AlphaFilePathA = AlphaFilePathAMem ;
#endif
		Texture->AlphaFilePathW = AlphaFilePathWMem ;
		Texture->AlphaImageFilePathAllocMem = TRUE ;
	}

	// �摜�n���h���̕ۑ�
	Texture->GraphHandle = GraphHandle ;

	// �f�t�H���g�摜���g�p���ꂽ���ǂ�����ۑ�
	Texture->IsDefaultTexture = DefaultTextureFlag ;

	// �o���v�}�b�v����ۑ�
	Texture->BumpImageFlag = BumpImageFlag ;
	Texture->BumpImageNextPixelLength = BumpImageNextPixelLength ;

	// �t���O��ۑ�
	Texture->ReverseFlag = ReverseFlag ? 1 : 0 ;
	Texture->Bmp32AllZeroAlphaToXRGB8Flag = Bmp32AllZeroAlphaToXRGB8Flag ? 1 : 0 ;

	// �摜�̃T�C�Y���擾
	NS_GetGraphSize( Texture->GraphHandle, &Texture->Width, &Texture->Height ) ;

	// �I��
	return 0 ;
}

static int _MV1TextureLoad(
	MV1_MODEL_BASE *ModelBase, MV1_TEXTURE *Texture,
	const wchar_t *ColorFilePathW, const wchar_t *AlphaFilePathW,
	int BumpImageFlag, float BumpImageNextPixelLength,
	bool ReverseFlag,
	bool Bmp32AllZeroAlphaToXRGB8Flag,
	int ASyncThread )
{
	void *ColorImage, *AlphaImage ;
	int ColorImageSize, AlphaImageSize ;
#ifndef UNICODE
	char    *ColorFilePathAMem, *AlphaFilePathAMem ;
#endif
	wchar_t *ColorFilePathWMem, *AlphaFilePathWMem ;
	int GraphHandle, SemiTransFlag, DefaultTextureFlag ;
	wchar_t *DirW ;
	int Result ;

	DirW = ModelBase->DirectoryPath ;

	// �e�N�X�`���̓ǂݍ���
	Result =  __MV1LoadTexture(
					&ColorImage, &ColorImageSize,
					&AlphaImage, &AlphaImageSize,
					&GraphHandle, &SemiTransFlag, &DefaultTextureFlag,
#ifndef UNICODE
					&ColorFilePathAMem, &AlphaFilePathAMem,
#endif
					&ColorFilePathWMem, &AlphaFilePathWMem,
					ColorFilePathW, AlphaFilePathW, DirW,
					BumpImageFlag, BumpImageNextPixelLength,
					ReverseFlag,
					Bmp32AllZeroAlphaToXRGB8Flag,
					NULL,
					false,
					TRUE,
					ASyncThread ) ;

	if( Result == -1 )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x54\x00\x65\x00\x78\x00\x74\x00\x75\x00\x72\x00\x65\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\x6e\x30\xad\x8a\x7f\x30\xbc\x8f\x7f\x30\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Load Texture Error : �e�N�X�`���̓ǂݍ��݂Ɏ��s���܂���\n" @*/ )) ;
		return -1 ;
	}

	// ���ɃC���[�W��ǂݍ���ł����烁�������������
	if( Texture->UseGraphHandle )
	{
		if( Texture->ColorImage )
		{
			DXFREE( Texture->ColorImage ) ;
			Texture->ColorImage = NULL ;
		}
		if( Texture->AlphaImage )
		{
			DXFREE( Texture->AlphaImage ) ;
			Texture->AlphaImage = NULL ;
		}
#ifndef UNICODE
		if( Texture->ColorFilePathA_ )
		{
			DXFREE( Texture->ColorFilePathA_ ) ;
			Texture->ColorFilePathA_ = NULL ;
		}
		if( Texture->AlphaFilePathA_ )
		{
			DXFREE( Texture->AlphaFilePathA_ ) ;
			Texture->AlphaFilePathA_ = NULL ;
		}
#endif
		if( Texture->ColorFilePathW_ )
		{
			DXFREE( Texture->ColorFilePathW_ ) ;
			Texture->ColorFilePathW_ = NULL ;
		}
		if( Texture->AlphaFilePathW_ )
		{
			DXFREE( Texture->AlphaFilePathW_ ) ;
			Texture->AlphaFilePathW_ = NULL ;
		}
	}

	// �V�����������A�h���X��ۑ�
	Texture->ColorImage = ColorImage ;
	Texture->ColorImageSize = ColorImageSize ;

	Texture->AlphaImage = AlphaImage ;
	Texture->AlphaImageSize = AlphaImageSize ;

	// �t�@�C���p�X��ۑ�
	if( Texture->ColorImage )
	{
#ifndef UNICODE
		Texture->ColorFilePathA_ = ColorFilePathAMem ;
#endif
		Texture->ColorFilePathW_ = ColorFilePathWMem ;
	}
	else
	{
#ifndef UNICODE
		Texture->ColorFilePathA_ = NULL ;
#endif
		Texture->ColorFilePathW_ = NULL ;
	}

	if( Texture->AlphaImage )
	{
#ifndef UNICODE
		Texture->AlphaFilePathA_ = AlphaFilePathAMem ;
#endif
		Texture->AlphaFilePathW_ = AlphaFilePathWMem ;
	}
	else
	{
#ifndef UNICODE
		Texture->AlphaFilePathA_ = NULL ;
#endif
		Texture->AlphaFilePathW_ = NULL ;
	}

	// �摜�n���h���̕ۑ�
	Texture->GraphHandle = GraphHandle ;
	Texture->UseGraphHandle = TRUE ;

	// �f�t�H���g�摜���g�p���ꂽ���ǂ�����ۑ�
	Texture->IsDefaultTexture = DefaultTextureFlag ;

	// �o���v�}�b�v����ۑ�
	Texture->BumpImageFlag = BumpImageFlag ;
	Texture->BumpImageNextPixelLength = BumpImageNextPixelLength ;

	// �t���O��ۑ�
	Texture->ReverseFlag = ReverseFlag ? 1 : 0 ;
	Texture->Bmp32AllZeroAlphaToXRGB8Flag = Bmp32AllZeroAlphaToXRGB8Flag ? 1 : 0 ;

	// �摜�̃T�C�Y���擾
	NS_GetGraphSize( Texture->GraphHandle, &Texture->Width, &Texture->Height ) ;

	// �I��
	return 0 ;
}

// ���f���Ŏg�p���Ă���}�e���A���̐����擾����
extern int MV1GetMaterialNumBase( int MBHandle )
{
	MV1_MODEL_BASE *ModelBase ;

	// ����������Ă��Ȃ�������G���[
	if( MV1Man.Initialize == false ) return -1 ;

	// �A�h���X�擾
	if( MV1BMDLCHK( MBHandle, ModelBase ) )
		return -1 ;

	// �}�e���A���̐���Ԃ�
	return ModelBase->MaterialNum ;
}

#ifndef UNICODE

// �w��̃}�e���A���̖��O���擾����
extern	const char *MV1GetMaterialNameBase( int MBHandle, int MaterialIndex )
{
	MV1BASEMATERIALSTART( MBHandle, ModelBase, Material, MaterialIndex, NULL ) ;

	// �Ԃ�
	return Material->NameA ;
}

#endif

// �w��̃}�e���A���̖��O���擾����
extern	const wchar_t *MV1GetMaterialNameBaseW( int MBHandle, int MaterialIndex )
{
	MV1BASEMATERIALSTART( MBHandle, ModelBase, Material, MaterialIndex, NULL ) ;

	// �Ԃ�
	return Material->NameW ;
}

// �w��̃}�e���A���̃^�C�v��ύX����( Type : DX_MATERIAL_TYPE_NORMAL �Ȃ� )
extern int MV1SetMaterialTypeBase( int MBHandle, int MaterialIndex, int Type )
{
	MV1_MODEL *Model ;
	MV1_MESH_BASE * MBMesh ;
	MV1_MESH *Mesh ;
	int i ;

	MV1BASEMATERIALSTART( MBHandle, ModelBase, Material, MaterialIndex, -1 ) ;

	// ���܂łƃ^�C�v�������������ꍇ�͉������Ȃ�
	if( Material->Type == Type ) return 0 ;

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �^�C�v��ۑ�
	Material->Type = Type ;

	// ���̃}�e���A�����g�p���Ă��郁�b�V���̃Z�b�g�A�b�v�����t���O��|��
	MBMesh = ModelBase->Mesh ;
	for( i = 0 ; i < ModelBase->MeshNum ; i ++, MBMesh ++ )
	{
		if( MBMesh->Material != Material ) continue ;

		for( Model = ModelBase->UseFirst ; Model ; Model = Model->UseBaseDataNext )
		{
			Mesh = &Model->Mesh[ i ] ;
			MV1MESH_RESET_SEMITRANSSETUP( Mesh )

			// �}�e���A�������X�V
			if( MV1CCHK( Mesh->DrawMaterialChange ) == 0 )
				MV1BitSetChange( &Mesh->DrawMaterialChange ) ;
		}
	}

	// �I��
	return 0 ;
}

// �w��̃}�e���A���̃^�C�v���擾����( �߂�l : DX_MATERIAL_TYPE_NORMAL �Ȃ� )
extern int MV1GetMaterialTypeBase( int MBHandle, int MaterialIndex )
{
	MV1BASEMATERIALSTART( MBHandle, ModelBase, Material, MaterialIndex, -1 ) ;

	// �Ԃ�
	return Material->Type ;
}

// �w��̃}�e���A���̃f�B�t���[�Y�J���[���擾����
extern	COLOR_F		MV1GetMaterialDifColorBase( int MBHandle, int MaterialIndex )
{
	COLOR_F RetColor = { 0.0f, 0.0f, 0.0f, 0.0f } ;
	MV1BASEMATERIALSTART( MBHandle, ModelBase, Material, MaterialIndex, RetColor ) ;

	// �Ԃ�
	return Material->Diffuse ;
}

// �w��̃}�e���A���̃X�y�L�����J���[���擾����
extern	COLOR_F		MV1GetMaterialSpcColorBase( int MBHandle, int MaterialIndex )
{
	COLOR_F RetColor = { 0.0f, 0.0f, 0.0f, 0.0f } ;
	MV1BASEMATERIALSTART( MBHandle, ModelBase, Material, MaterialIndex, RetColor ) ;

	// �Ԃ�
	return Material->Specular ;
}

// �w��̃}�e���A���̃G�~�b�V�u�J���[���擾����
extern	COLOR_F		MV1GetMaterialEmiColorBase( int MBHandle, int MaterialIndex )
{
	COLOR_F RetColor = { 0.0f, 0.0f, 0.0f, 0.0f } ;
	MV1BASEMATERIALSTART( MBHandle, ModelBase, Material, MaterialIndex, RetColor ) ;

	// �Ԃ�
	return Material->Emissive ;
}

// �w��̃}�e���A���̃A���r�G���g�J���[���擾����
extern	COLOR_F		MV1GetMaterialAmbColorBase( int MBHandle, int MaterialIndex )
{
	COLOR_F RetColor = { 0.0f, 0.0f, 0.0f, 0.0f } ;
	MV1BASEMATERIALSTART( MBHandle, ModelBase, Material, MaterialIndex, RetColor ) ;

	// �Ԃ�
	return Material->Ambient ;
}

// �w��̃}�e���A���̃X�y�L�����̋������擾����
extern	float		MV1GetMaterialSpcPowerBase( int MBHandle, int MaterialIndex )
{
	MV1BASEMATERIALSTART( MBHandle, ModelBase, Material, MaterialIndex, -1.0f ) ;

	// �Ԃ�
	return Material->Power ;
}

// �w��̃}�e���A���Ńf�B�t���[�Y�}�b�v�Ƃ��Ďg�p����e�N�X�`�����w�肷��
extern int MV1SetMaterialDifMapTextureBase( int MBHandle, int MaterialIndex, int TexIndex )
{
	MV1_MODEL *Model ;
	MV1_MESH_BASE * MBMesh ;
	MV1_MESH *Mesh ;
	int i ;

	MV1BASEMATERIALSTART( MBHandle, ModelBase, Material, MaterialIndex, -1 ) ;

	if( TexIndex < 0 || TexIndex >= ModelBase->TextureNum )
		return -1 ;

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// ���܂Ńf�B�t���[�Y�}�b�v�����������ꍇ�͂P�ɂ���
	if( Material->DiffuseLayerNum == 0 )
	{
		Material->DiffuseLayerNum = 1 ;
	}

	// �ݒ�
	Material->DiffuseLayer[ 0 ].Texture = TexIndex ;

	// ���̃}�e���A�����g�p���Ă��郁�b�V���̃Z�b�g�A�b�v�����t���O��|��
	MBMesh = ModelBase->Mesh ;
	for( i = 0 ; i < ModelBase->MeshNum ; i ++, MBMesh ++ )
	{
		if( MBMesh->Material != Material ) continue ;

		for( Model = ModelBase->UseFirst ; Model ; Model = Model->UseBaseDataNext )
		{
			Mesh = &Model->Mesh[ i ] ;
			MV1MESH_RESET_SEMITRANSSETUP( Mesh )

			// �}�e���A�������X�V
			if( MV1CCHK( Mesh->DrawMaterialChange ) == 0 )
				MV1BitSetChange( &Mesh->DrawMaterialChange ) ;
		}
	}

	// �I��
	return 0 ;
}

// �w��̃}�e���A���Ńf�B�t���[�Y�}�b�v�Ƃ��Ďg�p����Ă���e�N�X�`���̃C���f�b�N�X���擾����
extern int MV1GetMaterialDifMapTextureBase( int MBHandle, int MaterialIndex )
{
	MV1BASEMATERIALSTART( MBHandle, ModelBase, Material, MaterialIndex, -1 ) ;

	if( Material->DiffuseLayerNum == 0 )
		return -1 ;

	return Material->DiffuseLayer[ 0 ].Texture ;
}

// �w��̃}�e���A���ŃX�y�L�����}�b�v�Ƃ��Ďg�p����e�N�X�`�����w�肷��
extern int MV1SetMaterialSpcMapTextureBase( int MBHandle, int MaterialIndex, int TexIndex )
{
	MV1BASEMATERIALSTART( MBHandle, ModelBase, Material, MaterialIndex, -1 ) ;

	if( TexIndex >= ModelBase->TextureNum )
		return -1 ;

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �e�N�X�`���C���f�b�N�X�� 0 �ȉ��������ꍇ�̓X�y�L�����}�b�v����������
	if( TexIndex < 0 )
	{
		Material->SpecularLayerNum = 0 ;
	}
	else
	{
		// �X�y�L�����}�b�v�̐����O��������P�ɂ���
		if( Material->SpecularLayerNum == 0 )
		{
			Material->SpecularLayerNum = 1 ;
		}

		// �ݒ�
		Material->SpecularLayer[ 0 ].Texture = TexIndex ;
	}

	// �I��
	return 0 ;
}

// �w��̃}�e���A���ŃX�y�L�����}�b�v�Ƃ��Ďg�p����Ă���e�N�X�`���̃C���f�b�N�X���擾����
extern int MV1GetMaterialSpcMapTextureBase( int MBHandle, int MaterialIndex )
{
	MV1BASEMATERIALSTART( MBHandle, ModelBase, Material, MaterialIndex, -1 ) ;

	if( Material->SpecularLayerNum == 0 )
		return -1 ;

	return Material->SpecularLayer[ 0 ].Texture ;
}

// �w��̃}�e���A���Ŗ@���}�b�v�Ƃ��Ďg�p����e�N�X�`�����w�肷��
extern int MV1SetMaterialNormalMapTextureBase( int MBHandle, int MaterialIndex, int TexIndex )
{
	MV1BASEMATERIALSTART( MBHandle, ModelBase, Material, MaterialIndex, -1 ) ;

	if( TexIndex >= ModelBase->TextureNum )
		return -1 ;

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �e�N�X�`���C���f�b�N�X�� 0 �ȉ��������ꍇ�͖@���}�b�v����������
	if( TexIndex < 0 )
	{
		Material->NormalLayerNum = 0 ;
	}
	else
	{
		// �@���}�b�v�̐����O��������P�ɂ���
		if( Material->NormalLayerNum == 0 )
		{
			Material->NormalLayerNum = 1 ;
		}

		// �ݒ�
		Material->NormalLayer[ 0 ].Texture = TexIndex ;
	}

	// ���_�o�b�t�@�̍�蒼��
	MV1_TerminateVertexBufferBase_PF( MBHandle ) ;
	MV1_SetupVertexBufferBase_PF( MBHandle ) ;

	// �I��
	return 0 ;
}

// �w��̃}�e���A���Ŗ@���}�b�v�Ƃ��Ďg�p����Ă���e�N�X�`���̃C���f�b�N�X���擾����
extern int NS_MV1GetMaterialNormalMapTextureBase( int MBHandle, int MaterialIndex )
{
	MV1BASEMATERIALSTART( MBHandle, ModelBase, Material, MaterialIndex, -1 ) ;

	if( Material->NormalLayerNum == 0 )
		return -1 ;

	return Material->NormalLayer[ 0 ].Texture ;
}


// �w��̃}�e���A���̃f�B�t���[�Y�J���[��ݒ肷��
extern int MV1SetMaterialDifColorBase( int MBHandle, int MaterialIndex, COLOR_F Color )
{
	MV1_MODEL *Model ;
	MV1_MESH_BASE * MBMesh ;
	MV1_MESH *Mesh ;
	int i ;
	MV1BASEMATERIALSTART( MBHandle, ModelBase, Material, MaterialIndex, -1 ) ;

	// ���܂łƓ����������牽�����Ȃ�
	if( *( ( DWORD * )&Material->Diffuse.r ) == *( ( DWORD * )&Color.r ) &&
		*( ( DWORD * )&Material->Diffuse.g ) == *( ( DWORD * )&Color.g ) &&
		*( ( DWORD * )&Material->Diffuse.b ) == *( ( DWORD * )&Color.b ) &&
		*( ( DWORD * )&Material->Diffuse.a ) == *( ( DWORD * )&Color.a ) )
		return 0 ;

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �Z�b�g
	Material->Diffuse = Color ;

	// ���̃}�e���A�����g�p���Ă��郁�b�V���̔������v�f�L�����̃Z�b�g�A�b�v�����t���O��|��
	MBMesh = ModelBase->Mesh ;
	for( i = 0 ; i < ModelBase->MeshNum ; i ++, MBMesh ++ )
	{
		if( MBMesh->Material != Material ) continue ;

		for( Model = ModelBase->UseFirst ; Model ; Model = Model->UseBaseDataNext )
		{
			Mesh = &Model->Mesh[ i ] ;
			MV1MESH_RESET_SEMITRANSSETUP( Mesh )

			if( MV1CCHK( Mesh->DrawMaterialChange ) == 0 )
				MV1BitSetChange( &Mesh->DrawMaterialChange ) ;
		}
	}

	// �I��
	return 0 ;
}

// �w��̃}�e���A���̃X�y�L�����J���[��ݒ肷��
extern int MV1SetMaterialSpcColorBase( int MBHandle, int MaterialIndex, COLOR_F Color )
{
	MV1_MODEL *Model ;
	MV1_MESH_BASE * MBMesh ;
	MV1_MESH *Mesh ;
	int i ;
	MV1BASEMATERIALSTART( MBHandle, ModelBase, Material, MaterialIndex, -1 ) ;

	// ���܂łƓ����������牽�����Ȃ�
	if( *( ( DWORD * )&Material->Specular.r ) == *( ( DWORD * )&Color.r ) &&
		*( ( DWORD * )&Material->Specular.g ) == *( ( DWORD * )&Color.g ) &&
		*( ( DWORD * )&Material->Specular.b ) == *( ( DWORD * )&Color.b ) &&
		*( ( DWORD * )&Material->Specular.a ) == *( ( DWORD * )&Color.a ) )
		return 0 ;

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �Z�b�g
	Material->Specular = Color ;

	// ���̃}�e���A�����g�p���Ă��郁�b�V���̃Z�b�g�A�b�v�����t���O��|��
	MBMesh = ModelBase->Mesh ;
	for( i = 0 ; i < ModelBase->MeshNum ; i ++, MBMesh ++ )
	{
		if( MBMesh->Material != Material ) continue ;

		for( Model = ModelBase->UseFirst ; Model ; Model = Model->UseBaseDataNext )
		{
			Mesh = &Model->Mesh[ i ] ;

			if( MV1CCHK( Mesh->DrawMaterialChange ) == 0 )
				MV1BitSetChange( &Mesh->DrawMaterialChange ) ;
		}
	}

	// �I��
	return 0 ;
}

// �w��̃}�e���A���̃G�~�b�V�u�J���[��ݒ肷��
extern int MV1SetMaterialEmiColorBase( int MBHandle, int MaterialIndex, COLOR_F Color )
{
	MV1_MODEL *Model ;
	MV1_MESH_BASE * MBMesh ;
	MV1_MESH *Mesh ;
	int i ;
	MV1BASEMATERIALSTART( MBHandle, ModelBase, Material, MaterialIndex, -1 ) ;

	// ���܂łƓ����������牽�����Ȃ�
	if( *( ( DWORD * )&Material->Emissive.r ) == *( ( DWORD * )&Color.r ) &&
		*( ( DWORD * )&Material->Emissive.g ) == *( ( DWORD * )&Color.g ) &&
		*( ( DWORD * )&Material->Emissive.b ) == *( ( DWORD * )&Color.b ) &&
		*( ( DWORD * )&Material->Emissive.a ) == *( ( DWORD * )&Color.a ) )
		return 0 ;

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �Z�b�g
	Material->Emissive = Color ;

	// ���̃}�e���A�����g�p���Ă��郁�b�V���̃Z�b�g�A�b�v�����t���O��|��
	MBMesh = ModelBase->Mesh ;
	for( i = 0 ; i < ModelBase->MeshNum ; i ++, MBMesh ++ )
	{
		if( MBMesh->Material != Material ) continue ;

		for( Model = ModelBase->UseFirst ; Model ; Model = Model->UseBaseDataNext )
		{
			Mesh = &Model->Mesh[ i ] ;

			if( MV1CCHK( Mesh->DrawMaterialChange ) == 0 )
				MV1BitSetChange( &Mesh->DrawMaterialChange ) ;
		}
	}

	// �I��
	return 0 ;
}

// �w��̃}�e���A���̃A���r�G���g�J���[��ݒ肷��
extern int MV1SetMaterialAmbColorBase( int MBHandle, int MaterialIndex, COLOR_F Color )
{
	MV1_MODEL *Model ;
	MV1_MESH_BASE * MBMesh ;
	MV1_MESH *Mesh ;
	int i ;
	MV1BASEMATERIALSTART( MBHandle, ModelBase, Material, MaterialIndex, -1 ) ;

	// ���܂łƓ����������牽�����Ȃ�
	if( *( ( DWORD * )&Material->Ambient.r ) == *( ( DWORD * )&Color.r ) &&
		*( ( DWORD * )&Material->Ambient.g ) == *( ( DWORD * )&Color.g ) &&
		*( ( DWORD * )&Material->Ambient.b ) == *( ( DWORD * )&Color.b ) &&
		*( ( DWORD * )&Material->Ambient.a ) == *( ( DWORD * )&Color.a ) )
		return 0 ;

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �Z�b�g
	Material->Ambient = Color ;

	// ���̃}�e���A�����g�p���Ă��郁�b�V���̃Z�b�g�A�b�v�����t���O��|��
	MBMesh = ModelBase->Mesh ;
	for( i = 0 ; i < ModelBase->MeshNum ; i ++, MBMesh ++ )
	{
		if( MBMesh->Material != Material ) continue ;

		for( Model = ModelBase->UseFirst ; Model ; Model = Model->UseBaseDataNext )
		{
			Mesh = &Model->Mesh[ i ] ;

			if( MV1CCHK( Mesh->DrawMaterialChange ) == 0 )
				MV1BitSetChange( &Mesh->DrawMaterialChange ) ;
		}
	}

	// �I��
	return 0 ;
}

// �w��̃}�e���A���̃X�y�L�����̋�����ݒ肷��
extern int MV1SetMaterialSpcPowerBase( int MBHandle, int MaterialIndex, float Power )
{
	MV1BASEMATERIALSTART( MBHandle, ModelBase, Material, MaterialIndex, -1 ) ;

	if( Material->Power == Power )
	{
		return 0 ;
	}

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �Z�b�g
	Material->Power = Power ;

	// �I��
	return 0 ;
}


// �w��̃}�e���A���Ńg�D�[�������_�����O�̃f�B�t���[�Y�O���f�[�V�����}�b�v�Ƃ��Ďg�p����e�N�X�`����ݒ肷��
extern int MV1SetMaterialDifGradTextureBase( int MBHandle, int MaterialIndex, int TexIndex )
{
	MV1_MODEL *Model ;
	MV1_MESH_BASE * MBMesh ;
	MV1_MESH *Mesh ;
	int i ;

	MV1BASEMATERIALSTART( MBHandle, ModelBase, Material, MaterialIndex, -1 ) ;

	if( TexIndex < -1 || TexIndex >= ModelBase->TextureNum )
		return -1 ;

	if( Material->DiffuseGradTexture == TexIndex )
	{
		return 0 ;
	}

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �ݒ�
	Material->DiffuseGradTexture = TexIndex ;

	// ���̃}�e���A�����g�p���Ă��郁�b�V���̃Z�b�g�A�b�v�����t���O��|��
	MBMesh = ModelBase->Mesh ;
	for( i = 0 ; i < ModelBase->MeshNum ; i ++, MBMesh ++ )
	{
		if( MBMesh->Material != Material ) continue ;

		for( Model = ModelBase->UseFirst ; Model ; Model = Model->UseBaseDataNext )
		{
			Mesh = &Model->Mesh[ i ] ;
			MV1MESH_RESET_SEMITRANSSETUP( Mesh )

			// �}�e���A�������X�V
			if( MV1CCHK( Mesh->DrawMaterialChange ) == 0 )
				MV1BitSetChange( &Mesh->DrawMaterialChange ) ;
		}
	}

	// �I��
	return 0 ;
}

// �w��̃}�e���A���Ńg�D�[�������_�����O�̃f�B�t���[�Y�O���f�[�V�����}�b�v�Ƃ��Ďg�p����e�N�X�`�����擾����
extern int MV1GetMaterialDifGradTextureBase( int MBHandle, int MaterialIndex )
{
	MV1BASEMATERIALSTART( MBHandle, ModelBase, Material, MaterialIndex, -1 ) ;

	// �p�����[�^��Ԃ�
	return Material->DiffuseGradTexture ;
}

// �w��̃}�e���A���Ńg�D�[�������_�����O�̃X�y�L�����O���f�[�V�����}�b�v�Ƃ��Ďg�p����e�N�X�`����ݒ肷��
extern int MV1SetMaterialSpcGradTextureBase( int MBHandle, int MaterialIndex, int TexIndex )
{
	MV1_MODEL *Model ;
	MV1_MESH_BASE * MBMesh ;
	MV1_MESH *Mesh ;
	int i ;

	MV1BASEMATERIALSTART( MBHandle, ModelBase, Material, MaterialIndex, -1 ) ;

	if( TexIndex < -1 || TexIndex >= ModelBase->TextureNum )
		return -1 ;

	if( Material->SpecularGradTexture == TexIndex )
	{
		return 0 ;
	}

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �ݒ�
	Material->SpecularGradTexture = TexIndex ;

	// ���̃}�e���A�����g�p���Ă��郁�b�V���̃Z�b�g�A�b�v�����t���O��|��
	MBMesh = ModelBase->Mesh ;
	for( i = 0 ; i < ModelBase->MeshNum ; i ++, MBMesh ++ )
	{
		if( MBMesh->Material != Material ) continue ;

		for( Model = ModelBase->UseFirst ; Model ; Model = Model->UseBaseDataNext )
		{
			Mesh = &Model->Mesh[ i ] ;
			MV1MESH_RESET_SEMITRANSSETUP( Mesh )

			// �}�e���A�������X�V
			if( MV1CCHK( Mesh->DrawMaterialChange ) == 0 )
				MV1BitSetChange( &Mesh->DrawMaterialChange ) ;
		}
	}

	// �I��
	return 0 ;
}

// �w��̃}�e���A���Ńg�D�[�������_�����O�̃X�y�L�����O���f�[�V�����}�b�v�Ƃ��Ďg�p����e�N�X�`�����擾����
extern int MV1GetMaterialSpcGradTextureBase( int MBHandle, int MaterialIndex )
{
	MV1BASEMATERIALSTART( MBHandle, ModelBase, Material, MaterialIndex, -1 ) ;

	// �p�����[�^��Ԃ�
	return Material->SpecularGradTexture ;
}

// �w��̃}�e���A���Ńg�D�[�������_�����O�̃X�t�B�A�}�b�v�Ƃ��Ďg�p����e�N�X�`����ݒ肷��
extern int MV1SetMaterialSphereMapTextureBase( int MBHandle, int MaterialIndex, int TexIndex )
{
	MV1_MODEL *Model ;
	MV1_MESH_BASE * MBMesh ;
	MV1_MESH *Mesh ;
	int i ;

	MV1BASEMATERIALSTART( MBHandle, ModelBase, Material, MaterialIndex, -1 ) ;

	if( TexIndex < -1 || TexIndex >= ModelBase->TextureNum )
		return -1 ;

	if( Material->SphereMapTexture == TexIndex )
	{
		return 0 ;
	}

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �ݒ�
	Material->SphereMapTexture = TexIndex ;

	// ���̃}�e���A�����g�p���Ă��郁�b�V���̃Z�b�g�A�b�v�����t���O��|��
	MBMesh = ModelBase->Mesh ;
	for( i = 0 ; i < ModelBase->MeshNum ; i ++, MBMesh ++ )
	{
		if( MBMesh->Material != Material ) continue ;

		for( Model = ModelBase->UseFirst ; Model ; Model = Model->UseBaseDataNext )
		{
			Mesh = &Model->Mesh[ i ] ;
			MV1MESH_RESET_SEMITRANSSETUP( Mesh )

			// �}�e���A�������X�V
			if( MV1CCHK( Mesh->DrawMaterialChange ) == 0 )
				MV1BitSetChange( &Mesh->DrawMaterialChange ) ;
		}
	}

	// �I��
	return 0 ;
}

// �w��̃}�e���A���Ńg�D�[�������_�����O�̃X�t�B�A�}�b�v�Ƃ��Ďg�p����e�N�X�`�����擾����
extern int MV1GetMaterialSphereMapTextureBase( int MBHandle, int MaterialIndex )
{
	MV1BASEMATERIALSTART( MBHandle, ModelBase, Material, MaterialIndex, -1 ) ;

	// �p�����[�^��Ԃ�
	return Material->SphereMapTexture ;
}

// �w��̃}�e���A���̃g�D�[�������_�����O�Ŏg�p����f�B�t���[�Y�O���f�[�V�����}�b�v�ƃf�B�t���[�Y�J���[�̍������@��ݒ肷��( DX_MATERIAL_BLENDTYPE_ADDITIVE �Ȃ� )
extern int MV1SetMaterialDifGradBlendTypeBase( int MBHandle, int MaterialIndex, int BlendType )
{
	MV1_MODEL *Model ;
	MV1_MESH_BASE * MBMesh ;
	MV1_MESH *Mesh ;
	int i ;

	MV1BASEMATERIALSTART( MBHandle, ModelBase, Material, MaterialIndex, -1 ) ;

	if( Material->DiffuseGradBlendType == BlendType )
	{
		return 0 ;
	}

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �ݒ�
	Material->DiffuseGradBlendType = BlendType ;

	// ���̃}�e���A�����g�p���Ă��郁�b�V���̃Z�b�g�A�b�v�����t���O��|��
	MBMesh = ModelBase->Mesh ;
	for( i = 0 ; i < ModelBase->MeshNum ; i ++, MBMesh ++ )
	{
		if( MBMesh->Material != Material ) continue ;

		for( Model = ModelBase->UseFirst ; Model ; Model = Model->UseBaseDataNext )
		{
			Mesh = &Model->Mesh[ i ] ;
			MV1MESH_RESET_SEMITRANSSETUP( Mesh )

			// �}�e���A�������X�V
			if( MV1CCHK( Mesh->DrawMaterialChange ) == 0 )
				MV1BitSetChange( &Mesh->DrawMaterialChange ) ;
		}
	}

	// �I��
	return 0 ;
}

// �w��̃}�e���A���̃g�D�[�������_�����O�Ŏg�p����f�B�t���[�Y�O���f�[�V�����}�b�v�ƃf�B�t���[�Y�J���[�̍������@���擾����( DX_MATERIAL_BLENDTYPE_ADDITIVE �Ȃ� )
extern int MV1GetMaterialDifGradBlendTypeBase( int MBHandle, int MaterialIndex )
{
	MV1BASEMATERIALSTART( MBHandle, ModelBase, Material, MaterialIndex, -1 ) ;

	// �p�����[�^��Ԃ�
	return Material->DiffuseGradBlendType ;
}

// �w��̃}�e���A���̃g�D�[�������_�����O�Ŏg�p����X�y�L�����O���f�[�V�����}�b�v�ƃX�y�L�����J���[�̍������@��ݒ肷��( DX_MATERIAL_BLENDTYPE_ADDITIVE �Ȃ� )
extern int MV1SetMaterialSpcGradBlendTypeBase( int MBHandle, int MaterialIndex, int BlendType )
{
	MV1_MODEL *Model ;
	MV1_MESH_BASE * MBMesh ;
	MV1_MESH *Mesh ;
	int i ;

	MV1BASEMATERIALSTART( MBHandle, ModelBase, Material, MaterialIndex, -1 ) ;

	if( Material->SpecularGradBlendType == BlendType )
	{
		return 0 ;
	}

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �ݒ�
	Material->SpecularGradBlendType = BlendType ;

	// ���̃}�e���A�����g�p���Ă��郁�b�V���̃Z�b�g�A�b�v�����t���O��|��
	MBMesh = ModelBase->Mesh ;
	for( i = 0 ; i < ModelBase->MeshNum ; i ++, MBMesh ++ )
	{
		if( MBMesh->Material != Material ) continue ;

		for( Model = ModelBase->UseFirst ; Model ; Model = Model->UseBaseDataNext )
		{
			Mesh = &Model->Mesh[ i ] ;
			MV1MESH_RESET_SEMITRANSSETUP( Mesh )

			// �}�e���A�������X�V
			if( MV1CCHK( Mesh->DrawMaterialChange ) == 0 )
				MV1BitSetChange( &Mesh->DrawMaterialChange ) ;
		}
	}

	// �I��
	return 0 ;
}

// �w��̃}�e���A���̃g�D�[�������_�����O�Ŏg�p����X�y�L�����O���f�[�V�����}�b�v�ƃX�y�L�����J���[�̍������@���擾����( DX_MATERIAL_BLENDTYPE_ADDITIVE �Ȃ� )
extern int MV1GetMaterialSpcGradBlendTypeBase( int MBHandle, int MaterialIndex )
{
	MV1BASEMATERIALSTART( MBHandle, ModelBase, Material, MaterialIndex, -1 ) ;

	// �p�����[�^��Ԃ�
	return Material->SpecularGradBlendType ;
}

// �w��̃}�e���A���̃g�D�[�������_�����O�Ŏg�p����X�t�B�A�}�b�v�̍������@��ݒ肷��( DX_MATERIAL_BLENDTYPE_ADDITIVE �Ȃ� )
extern int MV1SetMaterialSphereMapBlendTypeBase( int MBHandle, int MaterialIndex, int BlendType )
{
	MV1_MODEL *Model ;
	MV1_MESH_BASE * MBMesh ;
	MV1_MESH *Mesh ;
	int i ;

	MV1BASEMATERIALSTART( MBHandle, ModelBase, Material, MaterialIndex, -1 ) ;

	if( Material->SphereMapBlendType == BlendType )
	{
		return 0 ;
	}

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �ݒ�
	Material->SphereMapBlendType = BlendType ;

	// ���̃}�e���A�����g�p���Ă��郁�b�V���̃Z�b�g�A�b�v�����t���O��|��
	MBMesh = ModelBase->Mesh ;
	for( i = 0 ; i < ModelBase->MeshNum ; i ++, MBMesh ++ )
	{
		if( MBMesh->Material != Material ) continue ;

		for( Model = ModelBase->UseFirst ; Model ; Model = Model->UseBaseDataNext )
		{
			Mesh = &Model->Mesh[ i ] ;
			MV1MESH_RESET_SEMITRANSSETUP( Mesh )

			// �}�e���A�������X�V
			if( MV1CCHK( Mesh->DrawMaterialChange ) == 0 )
				MV1BitSetChange( &Mesh->DrawMaterialChange ) ;
		}
	}

	// �I��
	return 0 ;
}

// �w��̃}�e���A���̃g�D�[�������_�����O�Ŏg�p����X�t�B�A�}�b�v�̍������@���擾����( DX_MATERIAL_BLENDTYPE_ADDITIVE �Ȃ� )
extern int MV1GetMaterialSphereMapBlendTypeBase( int MBHandle, int MaterialIndex )
{
	MV1BASEMATERIALSTART( MBHandle, ModelBase, Material, MaterialIndex, -1 ) ;

	// �p�����[�^��Ԃ�
	return Material->SphereMapBlendType ;
}

// �w��̃}�e���A���̃g�D�[�������_�����O�Ŏg�p����֊s���̑�����ݒ肷��( 0.0f �` 1.0f )
extern int MV1SetMaterialOutLineWidthBase( int MBHandle, int MaterialIndex, float Width )
{
	MV1_MODEL *Model ;
	MV1_MESH_BASE * MBMesh ;
	MV1_MESH *Mesh ;
	int i ;

	MV1BASEMATERIALSTART( MBHandle, ModelBase, Material, MaterialIndex, -1 ) ;

	if( Material->OutLineWidth == Width )
	{
		return 0 ;
	}

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �ݒ�
	Material->OutLineWidth = Width ;

	// ���̃}�e���A�����g�p���Ă��郁�b�V���̃Z�b�g�A�b�v�����t���O��|��
	MBMesh = ModelBase->Mesh ;
	for( i = 0 ; i < ModelBase->MeshNum ; i ++, MBMesh ++ )
	{
		if( MBMesh->Material != Material ) continue ;

		for( Model = ModelBase->UseFirst ; Model ; Model = Model->UseBaseDataNext )
		{
			Mesh = &Model->Mesh[ i ] ;
			MV1MESH_RESET_SEMITRANSSETUP( Mesh )

			// �}�e���A�������X�V
			if( MV1CCHK( Mesh->DrawMaterialChange ) == 0 )
				MV1BitSetChange( &Mesh->DrawMaterialChange ) ;
		}
	}

	// �I��
	return 0 ;
}

// �w��̃}�e���A���̃g�D�[�������_�����O�Ŏg�p����֊s���̑������擾����( 0.0f �` 1.0f )
extern float MV1GetMaterialOutLineWidthBase( int MBHandle, int MaterialIndex )
{
	MV1BASEMATERIALSTART( MBHandle, ModelBase, Material, MaterialIndex, -1 ) ;

	// �p�����[�^��Ԃ�
	return Material->OutLineWidth ;
}

// �w��̃}�e���A���̃g�D�[�������_�����O�Ŏg�p����֊s���̃h�b�g�P�ʂ̑�����ݒ肷��
extern int MV1SetMaterialOutLineDotWidthBase( int MBHandle, int MaterialIndex, float Width )
{
	MV1_MODEL *Model ;
	MV1_MESH_BASE * MBMesh ;
	MV1_MESH *Mesh ;
	int i ;

	MV1BASEMATERIALSTART( MBHandle, ModelBase, Material, MaterialIndex, -1 ) ;

	if( Material->OutLineDotWidth == Width )
	{
		return 0 ;
	}

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �ݒ�
	Material->OutLineDotWidth = Width ;

	// ���̃}�e���A�����g�p���Ă��郁�b�V���̃Z�b�g�A�b�v�����t���O��|��
	MBMesh = ModelBase->Mesh ;
	for( i = 0 ; i < ModelBase->MeshNum ; i ++, MBMesh ++ )
	{
		if( MBMesh->Material != Material ) continue ;

		for( Model = ModelBase->UseFirst ; Model ; Model = Model->UseBaseDataNext )
		{
			Mesh = &Model->Mesh[ i ] ;
			MV1MESH_RESET_SEMITRANSSETUP( Mesh )

			// �}�e���A�������X�V
			if( MV1CCHK( Mesh->DrawMaterialChange ) == 0 )
				MV1BitSetChange( &Mesh->DrawMaterialChange ) ;
		}
	}

	// �I��
	return 0 ;
}

// �w��̃}�e���A���̃g�D�[�������_�����O�Ŏg�p����֊s���̃h�b�g�P�ʂ̑������擾����
extern float MV1GetMaterialOutLineDotWidthBase( int MBHandle, int MaterialIndex )
{
	MV1BASEMATERIALSTART( MBHandle, ModelBase, Material, MaterialIndex, -1 ) ;

	// �p�����[�^��Ԃ�
	return Material->OutLineDotWidth ;
}

// �w��̃}�e���A���̃g�D�[�������_�����O�Ŏg�p����֊s���̐F��ݒ肷��
extern int MV1SetMaterialOutLineColorBase( int MBHandle, int MaterialIndex, COLOR_F Color )
{
	MV1_MODEL *Model ;
	MV1_MESH_BASE * MBMesh ;
	MV1_MESH *Mesh ;
	int i ;

	MV1BASEMATERIALSTART( MBHandle, ModelBase, Material, MaterialIndex, -1 ) ;

	if( Material->OutLineColor.r == Color.r &&
		Material->OutLineColor.g == Color.g &&
		Material->OutLineColor.b == Color.b &&
		Material->OutLineColor.a == Color.a )
	{
		return 0 ;
	}

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �ݒ�
	Material->OutLineColor = Color ;

	// ���̃}�e���A�����g�p���Ă��郁�b�V���̃Z�b�g�A�b�v�����t���O��|��
	MBMesh = ModelBase->Mesh ;
	for( i = 0 ; i < ModelBase->MeshNum ; i ++, MBMesh ++ )
	{
		if( MBMesh->Material != Material ) continue ;

		for( Model = ModelBase->UseFirst ; Model ; Model = Model->UseBaseDataNext )
		{
			Mesh = &Model->Mesh[ i ] ;
			MV1MESH_RESET_SEMITRANSSETUP( Mesh )

			// �}�e���A�������X�V
			if( MV1CCHK( Mesh->DrawMaterialChange ) == 0 )
				MV1BitSetChange( &Mesh->DrawMaterialChange ) ;
		}
	}

	// �I��
	return 0 ;
}

// �w��̃}�e���A���̃g�D�[�������_�����O�Ŏg�p����֊s���̐F���擾����
extern COLOR_F MV1GetMaterialOutLineColorBase( int MBHandle, int MaterialIndex )
{
	COLOR_F RetColor = { 0.0f, 0.0f, 0.0f, 0.0f } ;
	MV1BASEMATERIALSTART( MBHandle, ModelBase, Material, MaterialIndex, RetColor ) ;

	// �p�����[�^��Ԃ�
	return Material->OutLineColor ;
}

// �w��̃}�e���A���̕`��u�����h���[�h��ݒ肷��( DX_BLENDMODE_ALPHA �� )
extern int MV1SetMaterialDrawBlendModeBase( int MBHandle, int MaterialIndex, int BlendMode )
{
	MV1_MODEL *Model ;
	MV1_MESH_BASE * MBMesh ;
	MV1_MESH *Mesh ;
	int i ;
	MV1BASEMATERIALSTART( MBHandle, ModelBase, Material, MaterialIndex, -1 ) ;

	if( Material->DrawBlendMode == BlendMode )
	{
		return 0 ;
	}

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �u�����h���[�h�̃Z�b�g
	Material->DrawBlendMode = BlendMode ;

	// ���̃}�e���A�����g�p���Ă��郁�b�V���̃Z�b�g�A�b�v�����t���O��|��
	MBMesh = ModelBase->Mesh ;
	for( i = 0 ; i < ModelBase->MeshNum ; i ++, MBMesh ++ )
	{
		if( MBMesh->Material != Material ) continue ;

		for( Model = ModelBase->UseFirst ; Model ; Model = Model->UseBaseDataNext )
		{
			Mesh = &Model->Mesh[ i ] ;
			MV1MESH_RESET_SEMITRANSSETUP( Mesh )

			if( MV1CCHK( Mesh->DrawMaterialChange ) == 0 )
				MV1BitSetChange( &Mesh->DrawMaterialChange ) ;
		}
	}

	// �I��
	return 0 ;
}

// �w��̃}�e���A���̕`��u�����h�p�����[�^��ݒ肷��
extern int MV1SetMaterialDrawBlendParamBase( int MBHandle, int MaterialIndex, int BlendParam )
{
	MV1_MODEL *Model ;
	MV1_MESH_BASE * MBMesh ;
	MV1_MESH *Mesh ;
	int i ;
	MV1BASEMATERIALSTART( MBHandle, ModelBase, Material, MaterialIndex, -1 ) ;

	if( Material->DrawBlendParam == BlendParam )
	{
		return 0 ;
	}

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �u�����h�p�����[�^�̃Z�b�g
	Material->DrawBlendParam = BlendParam ;

	// ���̃}�e���A�����g�p���Ă��郁�b�V���̃Z�b�g�A�b�v�����t���O��|��
	MBMesh = ModelBase->Mesh ;
	for( i = 0 ; i < ModelBase->MeshNum ; i ++, MBMesh ++ )
	{
		if( MBMesh->Material != Material ) continue ;

		for( Model = ModelBase->UseFirst ; Model ; Model = Model->UseBaseDataNext )
		{
			Mesh = &Model->Mesh[ i ] ;
			MV1MESH_RESET_SEMITRANSSETUP( Mesh )

			if( MV1CCHK( Mesh->DrawMaterialChange ) == 0 )
				MV1BitSetChange( &Mesh->DrawMaterialChange ) ;
		}
	}

	// �I��
	return 0 ;
}

// �w��̃}�e���A���̕`��u�����h���[�h���擾����( DX_BLENDMODE_ALPHA �� )
extern int MV1GetMaterialDrawBlendModeBase( int MBHandle, int MaterialIndex )
{
	MV1BASEMATERIALSTART( MBHandle, ModelBase, Material, MaterialIndex, -1 ) ;

	// �u�����h���[�h��Ԃ�
	return Material->DrawBlendMode ;
}

// �w��̃}�e���A���̕`��u�����h�p�����[�^��ݒ肷��
extern int MV1GetMaterialDrawBlendParamBase( int MBHandle, int MaterialIndex )
{
	MV1BASEMATERIALSTART( MBHandle, ModelBase, Material, MaterialIndex, -1 ) ;

	// �u�����h�p�����[�^��Ԃ�
	return Material->DrawBlendParam ;
}

// �w��̃}�e���A���̕`�掞�̃A���t�@�e�X�g�̐ݒ���s��( Enable:���e�X�g���s�����ǂ���( TRUE:�s��  FALSE:�s��Ȃ�( �f�t�H���g )  Mode:�e�X�g���[�h( DX_CMP_GREATER�� )  Param:�`��A���t�@�l�Ƃ̔�r�Ɏg�p����l( 0�`255 ) )
extern int MV1SetMaterialDrawAlphaTestBase( int MBHandle, int MaterialIndex, int Enable, int Mode, int Param )
{
	MV1BASEMATERIALSTART( MBHandle, ModelBase, Material, MaterialIndex, -1 ) ;

	if( Material->UseAlphaTest == Enable &&
		Material->AlphaFunc == Mode &&
		Material->AlphaRef == Param )
	{
		return 0 ;
	}

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �p�����[�^�̃Z�b�g
	Material->UseAlphaTest = Enable ;
	Material->AlphaFunc = Mode ;
	Material->AlphaRef = Param ;

	// �I��
	return 0 ;
}

// �w��̃}�e���A���̕`�掞�̃A���t�@�e�X�g���s�����ǂ������擾����( �߂�l  TRUE:�A���t�@�e�X�g���s��  FALSE:�A���t�@�e�X�g���s��Ȃ� )
extern int MV1GetMaterialDrawAlphaTestEnableBase( int MBHandle, int MaterialIndex )
{
	MV1BASEMATERIALSTART( MBHandle, ModelBase, Material, MaterialIndex, -1 ) ;

	return Material->UseAlphaTest ;
}

// �w��̃}�e���A���̕`�掞�̃A���t�@�e�X�g�̃e�X�g���[�h���擾����( �߂�l  �e�X�g���[�h( DX_CMP_GREATER�� ) )
extern int MV1GetMaterialDrawAlphaTestModeBase( int MBHandle, int MaterialIndex )
{
	MV1BASEMATERIALSTART( MBHandle, ModelBase, Material, MaterialIndex, -1 ) ;

	return Material->AlphaFunc ;
}

// �w��̃}�e���A���̕`�掞�̃A���t�@�e�X�g�̕`��A���t�@�n�Ƃ̔�r�Ɏg�p����l( 0�`255 )���擾����
extern int MV1GetMaterialDrawAlphaTestParamBase( int MBHandle, int MaterialIndex )
{
	MV1BASEMATERIALSTART( MBHandle, ModelBase, Material, MaterialIndex, -1 ) ;

	return Material->AlphaRef ;
}







// ��{�f�[�^���e�N�X�`���֌W

// �e�N�X�`���̐����擾
extern int MV1GetTextureNumBase( int MBHandle )
{
	MV1_MODEL_BASE *ModelBase ;

	if( MV1BMDLCHK( MBHandle, ModelBase ) )
		return -1 ;

	// �e�N�X�`���̐���Ԃ�
	return ModelBase->TextureNum ;
}

// �e�N�X�`���̒ǉ�
extern int MV1AddTextureBase( 
	int MBHandle,
	const wchar_t *Name,
	const wchar_t *ColorFilePathW, const wchar_t *AlphaFilePathW,
	void * /*ColorFileImage*/, void * /*AlphaFileImage*/,
	int AddressModeU, int AddressModeV, int FilterMode,
	int BumpImageFlag, float BumpImageNextPixelLength,
	bool ReverseFlag,
	bool Bmp32AllZeroAlphaToXRGB8Flag,
	int ASyncThread )
{
	MV1_MODEL_BASE *ModelBase ;
	MV1_MODEL *Model ;
	MV1_TEXTURE_BASE *NewMBTexDim = NULL, *MBTexture = NULL ;
	MV1_TEXTURE *NewTexDim = NULL, *Texture ;
	int i, j ;
	wchar_t *ColorFilePathWMem = NULL, *AlphaFilePathWMem = NULL ;
#ifndef UNICODE
	char    *ColorFilePathAMem = NULL, *AlphaFilePathAMem = NULL ;
#endif
	wchar_t *DirW ;
	int Result ;

	if( MV1BMDLCHK( MBHandle, ModelBase ) )
		return -1 ;

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �V�����e�N�X�`���̏����i�[���郁�����̈�̊m��
	NewMBTexDim = ( MV1_TEXTURE_BASE * )DXALLOC( sizeof( MV1_TEXTURE_BASE ) * ( ModelBase->TextureNum + 1 ) ) ;
	if( NewMBTexDim == NULL )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\xb0\x65\x57\x30\x44\x30\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\xfa\x57\x2c\x67\xc5\x60\x31\x58\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�V�����e�N�X�`����{�����i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
		goto ERRORLABEL ;
	}

	DirW = ModelBase->DirectoryPath ;

	// �����̃f�[�^���ہX�R�s�[
	_MEMCPY( NewMBTexDim, ModelBase->Texture, sizeof( MV1_TEXTURE_BASE ) * ModelBase->TextureNum ) ;

	// �V�����e�N�X�`���̏����\�z
	{
		MBTexture = NewMBTexDim + ModelBase->TextureNum ;
		_MEMSET( MBTexture, 0, sizeof( MV1_TEXTURE_BASE ) ) ;

		// ���[�U�[�f�[�^��������
		MBTexture->UserData[ 0 ] = 0 ;
		MBTexture->UserData[ 1 ] = 0 ;
		MBTexture->UseUserGraphHandle = 0 ;
		MBTexture->UserGraphHandle = 0 ;

		// ���O��ۑ�
		if( Name == NULL )
		{
			Name = L"NoName" ;
		}
		MBTexture->NameAllocMem = TRUE ;
		MBTexture->NameW = ( wchar_t * )DXALLOC( ( size_t )( ( _WCSLEN( Name ) + 1 ) * sizeof( wchar_t ) ) ) ;
		_WCSCPY( MBTexture->NameW, Name ) ;

		// ���]�t���O���Z�b�g
		MBTexture->ReverseFlag = ReverseFlag ? TRUE : FALSE ;

		// �R�Q�r�b�g�a�l�o�̃A���t�@�l���S���O�������� XRGB8 �Ƃ��Ĉ������ǂ����̃t���O���Z�b�g����
		MBTexture->Bmp32AllZeroAlphaToXRGB8Flag = Bmp32AllZeroAlphaToXRGB8Flag ? TRUE : FALSE ;

		// �e�N�X�`���̓ǂݍ���
		Result = __MV1LoadTexture(
				&MBTexture->ColorImage, &MBTexture->ColorImageSize,
				&MBTexture->AlphaImage, &MBTexture->AlphaImageSize,
				&MBTexture->GraphHandle,
				&MBTexture->SemiTransFlag,
				&MBTexture->IsDefaultTexture,
#ifndef UNICODE
				&ColorFilePathAMem, &AlphaFilePathAMem,
#endif
				&ColorFilePathWMem, &AlphaFilePathWMem,
				ColorFilePathW, AlphaFilePathW, DirW,
				BumpImageFlag, BumpImageNextPixelLength,
				MBTexture->ReverseFlag,
				MBTexture->Bmp32AllZeroAlphaToXRGB8Flag,
				NULL,
				false,
				TRUE,
				ASyncThread ) ;

		if( Result == -1 )
		{
			DXST_ERRORLOGFMT_ADDW(( L"Read Model Convert Error : Texture File : Load Error : %s\n", Name ) ) ;
			goto ERRORLABEL ;
		}

		// �t�@�C���p�X��ۑ�
		if( MBTexture->ColorImage )
		{
			MBTexture->ColorImageFilePathAllocMem = TRUE ;
			MBTexture->ColorFilePathW = ColorFilePathWMem ;
#ifndef UNICODE
			MBTexture->ColorFilePathA = ColorFilePathAMem ;
#endif
		}
		if( MBTexture->AlphaImage )
		{
			MBTexture->AlphaImageFilePathAllocMem = TRUE ;
			MBTexture->AlphaFilePathW = AlphaFilePathWMem ;
#ifndef UNICODE
			MBTexture->AlphaFilePathA = AlphaFilePathAMem ;
#endif
		}

		// �A�h���b�V���O���[�h�̃Z�b�g
		MBTexture->AddressModeU = AddressModeU ;
		MBTexture->AddressModeV = AddressModeV ;

		// �t�u�̃X�P�[���l���Z�b�g
		MBTexture->ScaleU = 1.0f ;
		MBTexture->ScaleV = 1.0f ;

		// �t�B���^�����O���[�h�̃Z�b�g
		MBTexture->FilterMode = FilterMode ;

		// �o���v�}�b�v����ۑ�
		MBTexture->BumpImageFlag = BumpImageFlag ;
		MBTexture->BumpImageNextPixelLength = BumpImageNextPixelLength ;

		// �摜�̃T�C�Y���擾
		NS_GetGraphSize( MBTexture->GraphHandle, &MBTexture->Width, &MBTexture->Height ) ;
	}

	// ���̃��f����{�f�[�^���g�p���Ă��郂�f���S�Ẵe�N�X�`���f�[�^�𑝂₷
	Model = ModelBase->UseFirst ;
	for( i = 0 ; i < ModelBase->UseNum ; i ++, Model = Model->UseBaseDataNext )
	{
		// �V�����e�N�X�`���̏����i�[���郁�����̈�̊m��
		NewTexDim = ( MV1_TEXTURE * )DXALLOC( sizeof( MV1_TEXTURE ) * ( ModelBase->TextureNum + 1 ) ) ;
		if( NewTexDim == NULL )
		{
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\xb0\x65\x57\x30\x44\x30\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\xc5\x60\x31\x58\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�V�����e�N�X�`�������i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
			goto ERRORLABEL ;
		}

		// �����̃f�[�^���ہX�R�s�[
		_MEMCPY( NewTexDim, Model->Texture, sizeof( MV1_TEXTURE ) * ModelBase->TextureNum ) ;

		// �V�����e�N�X�`���̏����\�z
		{
			Texture = NewTexDim + ModelBase->TextureNum ;

			Texture->BaseData = MBTexture ;

#ifndef UNICODE
			Texture->AlphaFilePathA_ = MBTexture->AlphaFilePathA ;
#endif
			Texture->AlphaFilePathW_ = MBTexture->AlphaFilePathW ;
			Texture->AlphaImage = NULL ;
			Texture->AlphaImageSize = 0 ;

#ifndef UNICODE
			Texture->ColorFilePathA_ = MBTexture->ColorFilePathA ;
#endif
			Texture->ColorFilePathW_ = MBTexture->ColorFilePathW ;
			Texture->ColorImage = NULL ;
			Texture->ColorImageSize = 0 ;

			Texture->Width = 0 ;
			Texture->Height = 0 ;
			Texture->SemiTransFlag = 0 ;
			Texture->BumpImageFlag = MBTexture->BumpImageFlag ;
			Texture->BumpImageNextPixelLength = MBTexture->BumpImageNextPixelLength ;

			Texture->UseGraphHandle = 0 ;
			Texture->GraphHandle = -1 ;

			Texture->UseUserGraphHandle = MBTexture->UseUserGraphHandle ;
			Texture->UserGraphHandle = MBTexture->UserGraphHandle ;
			Texture->UserGraphWidth = MBTexture->UserGraphWidth ;
			Texture->UserGraphHeight = MBTexture->UserGraphHeight ;
			Texture->UserGraphHandleSemiTransFlag = MBTexture->UserGraphHandleSemiTransFlag ;

			Texture->AddressModeU = MBTexture->AddressModeU ;
			Texture->AddressModeV = MBTexture->AddressModeV ;
			Texture->ScaleU = MBTexture->ScaleU ;
			Texture->ScaleV = MBTexture->ScaleV ;
			Texture->FilterMode = MBTexture->FilterMode ;

			Texture->ReverseFlag = MBTexture->ReverseFlag ;
			Texture->Bmp32AllZeroAlphaToXRGB8Flag = MBTexture->Bmp32AllZeroAlphaToXRGB8Flag ;
		}

		// ���Ƀe�N�X�`���p�Ƀ��������m�ۂ���Ă�������
		if( Model->TextureAllocMem )
		{
			DXFREE( Model->Texture ) ;
			Model->Texture = NULL ;
		}

		// �V�������ʒu�̃Z�b�g
		Model->TextureAllocMem = TRUE ;
		Model->Texture = NewTexDim ;
	}

	// ���Ƀe�N�X�`���p�Ƀ��������m�ۂ���Ă�������
	if( ModelBase->TextureAllocMem )
	{
		DXFREE( ModelBase->Texture ) ;
		ModelBase->Texture = NULL ;
	}

	// �V�������ʒu�̃Z�b�g
	ModelBase->TextureAllocMem = TRUE ;
	ModelBase->Texture = NewMBTexDim ;

	// �e�N�X�`���̐��𑝂₷
	ModelBase->TextureNum ++ ;

	// �e�N�X�`����{�f�[�^�ւ̃A�h���X��ύX
	Model = ModelBase->UseFirst ;
	for( i = 0 ; i < ModelBase->UseNum ; i ++, Model = Model->UseBaseDataNext )
	{
		Texture = Model->Texture ;
		MBTexture = ModelBase->Texture ;
		for( j = 0 ; j < ModelBase->TextureNum ; j ++, Texture ++, MBTexture ++ ) 
		{
			Texture->BaseData = MBTexture ;
		}
	}

	// ����I��
	return 0 ;

ERRORLABEL :
	if( NewMBTexDim )
	{
		DXFREE( NewMBTexDim ) ;
		NewMBTexDim = NULL ;
	}

	if( NewTexDim )
	{
		DXFREE( NewTexDim ) ;
		NewTexDim = NULL ;
	}

#ifndef UNICODE
	if( ColorFilePathAMem )
	{
		DXFREE( ColorFilePathAMem ) ;
		ColorFilePathAMem = NULL ;
	}

	if( AlphaFilePathAMem )
	{
		DXFREE( AlphaFilePathAMem ) ;
		AlphaFilePathAMem = NULL ;
	}
#endif

	if( ColorFilePathWMem )
	{
		DXFREE( ColorFilePathWMem ) ;
		ColorFilePathWMem = NULL ;
	}

	if( AlphaFilePathWMem )
	{
		DXFREE( AlphaFilePathWMem ) ;
		AlphaFilePathWMem = NULL ;
	}

	if( MBTexture )
	{
		if( MBTexture->AlphaFilePathW )
		{
#ifndef UNICODE
			DXFREE( MBTexture->AlphaFilePathA ) ;
			MBTexture->AlphaFilePathA = NULL ;
#endif
			DXFREE( MBTexture->AlphaFilePathW ) ;
			MBTexture->AlphaFilePathW = NULL ;

			DXFREE( MBTexture->AlphaImage ) ;
			MBTexture->AlphaImage = NULL ;
		}

		if( MBTexture->ColorFilePathW )
		{
#ifndef UNICODE
			DXFREE( MBTexture->ColorFilePathA ) ;
			MBTexture->ColorFilePathA = NULL ;
#endif
			DXFREE( MBTexture->ColorFilePathW ) ;
			MBTexture->ColorFilePathW = NULL ;

			DXFREE( MBTexture->ColorImage ) ;
			MBTexture->ColorImage = NULL ;
		}
	}

	return -1 ;
}

// �e�N�X�`���̍폜
extern int MV1DeleteTextureBase( int MBHandle, int TexIndex )
{
	MV1_MODEL_BASE *ModelBase ;
	MV1_MODEL *Model ;
	MV1_MATERIAL_BASE *MBMaterial ;
	MV1_MATERIAL *Material ;
	int i, j, k ;

	if( MV1BMDLCHK( MBHandle, ModelBase ) )
		return -1 ;

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �w��̃e�N�X�`���C���f�b�N�X����{�f�[�^�Ɗ�{�f�[�^���g�p���Ă��郂�f���f�[�^�Ŏg�p����Ă��Ȃ������ׂ�
	MBMaterial = ModelBase->Material ;
	for( i = 0 ; i < ModelBase->MaterialNum ; i ++, MBMaterial ++ )
	{
		for( j = 0 ; j < MBMaterial->DiffuseLayerNum && TexIndex != MBMaterial->DiffuseLayer[ j ].Texture ; j ++ ){}
		if( j != MBMaterial->DiffuseLayerNum ) break ;

		for( j = 0 ; j < MBMaterial->SpecularLayerNum && TexIndex != MBMaterial->SpecularLayer[ j ].Texture ; j ++ ){}
		if( j != MBMaterial->SpecularLayerNum ) break ;

		for( j = 0 ; j < MBMaterial->NormalLayerNum && TexIndex != MBMaterial->NormalLayer[ j ].Texture ; j ++ ){}
		if( j != MBMaterial->NormalLayerNum ) break ;

		if( MBMaterial->DiffuseGradTexture == TexIndex ) break ;
		if( MBMaterial->SpecularGradTexture == TexIndex ) break ;
	}

	// �g�p����Ă�����G���[
	if( i != ModelBase->MaterialNum )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\x4a\x52\x64\x96\x57\x30\x88\x30\x46\x30\x68\x30\x57\x30\x5f\x30\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\x6f\x30\xfa\x57\x2c\x67\xc7\x30\xfc\x30\xbf\x30\x6e\x30\xde\x30\xc6\x30\xea\x30\xa2\x30\xeb\x30\x67\x30\x7f\x4f\x28\x75\x55\x30\x8c\x30\x66\x30\x44\x30\x7e\x30\x59\x30\x0a\x00\x00"/*@ L"�폜���悤�Ƃ����e�N�X�`���͊�{�f�[�^�̃}�e���A���Ŏg�p����Ă��܂�\n" @*/ )) ;
		return -1 ;
	}

	// �w��̃e�N�X�`���C���f�b�N�X����{�f�[�^���g�p���Ă��郂�f���Ŏg�p����Ă��Ȃ������ׂ�
	Model = ModelBase->UseFirst ;
	for( k = 0 ; k < ModelBase->UseNum ; k ++, Model = Model->UseBaseDataNext )
	{
		Material = Model->Material ;
		for( i = 0 ; i < ModelBase->MaterialNum ; i ++, Material ++ )
		{
			for( j = 0 ; j < Material->DiffuseLayerNum && TexIndex != Material->DiffuseLayer[ j ].Texture ; j ++ ){}
			if( j != Material->DiffuseLayerNum ) break ;

			for( j = 0 ; j < Material->SpecularLayerNum && TexIndex != Material->SpecularLayer[ j ].Texture ; j ++ ){}
			if( j != Material->SpecularLayerNum ) break ;
		}
		if( i != ModelBase->MaterialNum )
			break ;
	}

	// �g�p����Ă�����G���[
	if( k != ModelBase->UseNum )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\x4a\x52\x64\x96\x57\x30\x88\x30\x46\x30\x68\x30\x57\x30\x5f\x30\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\x6f\x30\xe2\x30\xc7\x30\xeb\x30\xc7\x30\xfc\x30\xbf\x30\x6e\x30\xde\x30\xc6\x30\xea\x30\xa2\x30\xeb\x30\x67\x30\x7f\x4f\x28\x75\x55\x30\x8c\x30\x66\x30\x44\x30\x7e\x30\x59\x30\x0a\x00\x00"/*@ L"�폜���悤�Ƃ����e�N�X�`���̓��f���f�[�^�̃}�e���A���Ŏg�p����Ă��܂�\n" @*/ )) ;
		return -1 ;
	}

	// �e�N�X�`���̐������炷
	ModelBase->TextureNum -- ;

	// �g�p����Ă��Ȃ����Ƃ��m�F�ł�����f�[�^�z����k�߂�
	if( TexIndex != ModelBase->TextureNum )
	{
		_MEMMOVE( &ModelBase->Texture[ TexIndex ],
			      &ModelBase->Texture[ TexIndex + 1 ],
				  sizeof( MV1_TEXTURE_BASE ) * ( ModelBase->TextureNum - TexIndex ) ) ;

		Model = ModelBase->UseFirst ;
		for( i = 0 ; i < ModelBase->UseNum ; i ++, Model = Model->UseBaseDataNext )
		{
			_MEMMOVE( &Model->Texture[ TexIndex ],
					  &Model->Texture[ TexIndex + 1 ],
					  sizeof( MV1_TEXTURE ) * ( ModelBase->TextureNum - TexIndex ) ) ;
		}
	}

	// �e�N�X�`���C���f�b�N�X��U��Ȃ���
	MBMaterial = ModelBase->Material ;
	for( i = 0 ; i < ModelBase->MaterialNum ; i ++, MBMaterial ++ )
	{
		for( j = 0 ; j < MBMaterial->DiffuseLayerNum ; j ++ )
		{
			if( MBMaterial->DiffuseLayer[ j ].Texture > TexIndex )
				MBMaterial->DiffuseLayer[ j ].Texture -- ;
		}

		for( j = 0 ; j < MBMaterial->SpecularLayerNum ; j ++ )
		{
			if( MBMaterial->SpecularLayer[ j ].Texture > TexIndex )
				MBMaterial->SpecularLayer[ j ].Texture -- ;
		}

		for( j = 0 ; j < MBMaterial->NormalLayerNum ; j ++ )
		{
			if( MBMaterial->NormalLayer[ j ].Texture > TexIndex )
				MBMaterial->NormalLayer[ j ].Texture -- ;
		}
	}

	Model = ModelBase->UseFirst ;
	for( k = 0 ; k < ModelBase->UseNum ; k ++, Model = Model->UseBaseDataNext )
	{
		Material = Model->Material ;
		for( i = 0 ; i < ModelBase->MaterialNum ; i ++, Material ++ )
		{
			for( j = 0 ; j < Material->DiffuseLayerNum ; j ++ )
			{
				if( Material->DiffuseLayer[ j ].Texture > TexIndex )
					Material->DiffuseLayer[ j ].Texture -- ;
			}

			for( j = 0 ; j < Material->SpecularLayerNum ; j ++ )
			{
				if( Material->SpecularLayer[ j ].Texture > TexIndex )
					Material->SpecularLayer[ j ].Texture -- ;
			}
		}
	}

	// �I��
	return 0 ;
}

#ifndef UNICODE
// �e�N�X�`���̖��O���擾
extern const char *MV1GetTextureNameBase( int MBHandle, int TexIndex )
{
	MV1BASETEXTURELSTART( MBHandle, ModelBase, Texture, TexIndex, NULL ) ;

	// �l��Ԃ�
	return Texture->NameA ;
}
#endif

// �e�N�X�`���̖��O���擾
extern const wchar_t *MV1GetTextureNameBaseW( int MBHandle, int TexIndex )
{
	MV1BASETEXTURELSTART( MBHandle, ModelBase, Texture, TexIndex, NULL ) ;

	// �l��Ԃ�
	return Texture->NameW ;
}

// �J���[�e�N�X�`���̃t�@�C���p�X��ύX����
extern int MV1SetTextureColorFilePathBaseW( int MBHandle, int TexIndex, const wchar_t *FilePathW )
{
	MV1BASETEXTURELSTART( MBHandle, ModelBase, Texture, TexIndex, -1 ) ;

	return _MV1TextureLoadBase(
				ModelBase, Texture,
				FilePathW, Texture->AlphaFilePathW,
				Texture->BumpImageFlag, Texture->BumpImageNextPixelLength,
				Texture->ReverseFlag != 0,
				Texture->Bmp32AllZeroAlphaToXRGB8Flag != 0,
				FALSE ) ;
}

// �J���[�e�N�X�`���̃t�@�C���p�X���擾
extern const wchar_t *MV1GetTextureColorFilePathBaseW( int MBHandle, int TexIndex )
{
	MV1BASETEXTURELSTART( MBHandle, ModelBase, Texture, TexIndex, NULL ) ;

	// �l��Ԃ�
	return Texture->ColorFilePathW ;
}

// �A���t�@�e�N�X�`���̃t�@�C���p�X��ύX����
extern int MV1SetTextureAlphaFilePathBaseW( int MBHandle, int TexIndex, const wchar_t *FilePathW )
{
	MV1BASETEXTURELSTART( MBHandle, ModelBase, Texture, TexIndex, -1 ) ;

	return _MV1TextureLoadBase(
				ModelBase, Texture,
				Texture->ColorFilePathW, FilePathW,
				Texture->BumpImageFlag, Texture->BumpImageNextPixelLength,
				Texture->ReverseFlag != 0,
				Texture->Bmp32AllZeroAlphaToXRGB8Flag != 0,
				FALSE ) ;
}

// �A���t�@�e�N�X�`���̃t�@�C���p�X���擾
extern const wchar_t *MV1GetTextureAlphaFilePathBaseW( int MBHandle, int TexIndex )
{
	MV1BASETEXTURELSTART( MBHandle, ModelBase, Texture, TexIndex, NULL ) ;

	// �l��Ԃ�
	return Texture->AlphaFilePathW ;
}

// �e�N�X�`���Ŏg�p����O���t�B�b�N�n���h����ύX����( GrHandle �� -1 �ɂ���Ɖ��� )
extern int MV1SetTextureGraphHandleBase( int MBHandle, int TexIndex, int GrHandle, int SemiTransFlag )
{
	MV1BASETEXTURELSTART( MBHandle, ModelBase, Texture, TexIndex, -1 ) ;

	// GrHandle �� -1 ���ǂ����ŏ����𕪊�
	if( GrHandle == -1 )
	{
		Texture->UseUserGraphHandle = FALSE ;
		Texture->UserGraphHandle = 0 ;
	}
	else
	{
		Texture->UseUserGraphHandle = TRUE ;
		Texture->UserGraphHandle = GrHandle ;
		Texture->UserGraphHandleSemiTransFlag = SemiTransFlag ;
		NS_GetGraphSize( Texture->UserGraphHandle, &Texture->UserGraphWidth, &Texture->UserGraphHeight ) ;
	}

	// �I��
	return 0 ;
}

// �e�N�X�`���̃O���t�B�b�N�n���h�����擾����
extern int MV1GetTextureGraphHandleBase( int MBHandle, int TexIndex )
{
	MV1BASETEXTURELSTART( MBHandle, ModelBase, Texture, TexIndex, -1 ) ;

	return Texture->UseUserGraphHandle == FALSE ? Texture->GraphHandle : Texture->UserGraphHandle ;
}

// �e�N�X�`���̃A�h���X���[�h��ݒ肷��
extern int MV1SetTextureAddressModeBase( int MBHandle, int TexIndex, int AddrUMode, int AddrVMode )
{
	MV1BASETEXTURELSTART( MBHandle, ModelBase, Texture, TexIndex, -1 ) ;

	if( Texture->AddressModeU == AddrUMode &&
		Texture->AddressModeV == AddrVMode )
	{
		return 0 ;
	}

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	Texture->AddressModeU = AddrUMode ;
	Texture->AddressModeV = AddrVMode ;

	// �I��
	return 0 ;
}

// �e�N�X�`���̂t�l�̃A�h���X���[�h���擾����( �߂�l:DX_TEXADDRESS_WRAP �� )
extern int MV1GetTextureAddressModeUBase( int MBHandle, int TexIndex )
{
	MV1BASETEXTURELSTART( MBHandle, ModelBase, Texture, TexIndex, -1 ) ;

	return Texture->AddressModeU ;
}

// �e�N�X�`���̂u�l�̃A�h���X���[�h���擾����( �߂�l:DX_TEXADDRESS_WRAP �� )
extern int MV1GetTextureAddressModeVBase( int MBHandle, int TexIndex )
{
	MV1BASETEXTURELSTART( MBHandle, ModelBase, Texture, TexIndex, -1 ) ;

	return Texture->AddressModeV ;
}

// �e�N�X�`���̕����擾����
extern int MV1GetTextureWidthBase( int MBHandle, int TexIndex )
{
	MV1BASETEXTURELSTART( MBHandle, ModelBase, Texture, TexIndex, -1 ) ;

	return Texture->UseUserGraphHandle ? Texture->UserGraphWidth : Texture->Width ;
}

// �e�N�X�`���̍������擾����
extern int MV1GetTextureHeightBase( int MBHandle, int TexIndex )
{
	MV1BASETEXTURELSTART( MBHandle, ModelBase, Texture, TexIndex, -1 ) ;

	return Texture->UseUserGraphHandle ? Texture->UserGraphHeight : Texture->Height ;
}

// �e�N�X�`���ɔ������v�f�����邩�ǂ������擾����( �߂�l  TRUE:����  FALSE:�Ȃ� )
extern int MV1GetTextureSemiTransStateBase( int MBHandle, int TexIndex )
{
	MV1BASETEXTURELSTART( MBHandle, ModelBase, Texture, TexIndex, -1 ) ;

	return Texture->UseUserGraphHandle ? Texture->UserGraphHandleSemiTransFlag : Texture->SemiTransFlag ;
}

// �e�N�X�`���Ŏg�p���Ă���摜���o���v�}�b�v���ǂ�����ݒ肷��
extern int MV1SetTextureBumpImageFlagBase( int MBHandle, int TexIndex, int Flag )
{
	MV1BASETEXTURELSTART( MBHandle, ModelBase, Texture, TexIndex, -1 ) ;

	return _MV1TextureLoadBase(
				ModelBase, Texture,
				Texture->ColorFilePathW, Texture->AlphaFilePathW,
				Flag, Texture->BumpImageNextPixelLength,
				Texture->ReverseFlag != 0,
				Texture->Bmp32AllZeroAlphaToXRGB8Flag != 0,
				FALSE ) ;
}

// �e�N�X�`�����o���v�}�b�v���ǂ������擾����( �߂�l  TRUE:�o���v�}�b�v  FALSE:�Ⴄ )
extern int MV1GetTextureBumpImageFlagBase( int MBHandle, int TexIndex )
{
	MV1BASETEXTURELSTART( MBHandle, ModelBase, Texture, TexIndex, -1 ) ;

	return Texture->BumpImageFlag ;
}

// �o���v�}�b�v�摜�̏ꍇ�ׂ̗̃s�N�Z���Ƃ̋�����ݒ肷��
extern int MV1SetTextureBumpImageNextPixelLengthBase( int MBHandle, int TexIndex, float Length )
{
	MV1BASETEXTURELSTART( MBHandle, ModelBase, Texture, TexIndex, -1 ) ;

	if( Texture->BumpImageNextPixelLength == Length )
	{
		return 0 ;
	}

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	Texture->BumpImageNextPixelLength = Length ;

	return _MV1TextureLoadBase(
				ModelBase, Texture,
				Texture->ColorFilePathW, Texture->AlphaFilePathW,
				Texture->BumpImageFlag, Length,
				Texture->ReverseFlag != 0,
				Texture->Bmp32AllZeroAlphaToXRGB8Flag != 0,
				FALSE ) ;
}

// �o���v�}�b�v�摜�̏ꍇ�ׂ̗̃s�N�Z���Ƃ̋������擾����
extern float MV1GetTextureBumpImageNextPixelLengthBase( int MBHandle, int TexIndex )
{
	MV1BASETEXTURELSTART( MBHandle, ModelBase, Texture, TexIndex, -1.0f ) ;

	return Texture->BumpImageNextPixelLength ;
}

// �e�N�X�`���̃t�B���^�����O���[�h��ݒ肷��
extern int MV1SetTextureSampleFilterModeBase( int MBHandle, int TexIndex, int FilterMode )
{
	MV1BASETEXTURELSTART( MBHandle, ModelBase, Texture, TexIndex, -1 ) ;

	if( Texture->FilterMode == FilterMode )
	{
		return 0 ;
	}

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	Texture->FilterMode = FilterMode ;

	// �I��
	return 0 ;
}

// �e�N�X�`���̃t�B���^�����O���[�h���擾����( �߂�l  MV1_TEXTURE_FILTER_MODE_POINT�� )
extern int MV1GetTextureSampleFilterModeBase( int MBHandle, int TexIndex )
{
	MV1BASETEXTURELSTART( MBHandle, ModelBase, Texture, TexIndex, -1 ) ;

	return Texture->FilterMode ;
}








// �w��̃��b�V���̒��_�f�B�t���[�Y�J���[���}�e���A���̃f�B�t���[�Y�J���[�̑���Ɏg�p���邩�ǂ�����ݒ肷��( TRUE:�}�e���A���J���[�̑���Ɏg�p����  FALSE:�}�e���A���J���[���g�p���� )
extern int MV1SetMeshUseVertDifColorBase( int MBHandle, int MeshIndex, int UseFlag )
{
	MV1_MODEL *Model ;
	MV1_MESH *Mesh ;
	MV1BASEMESHSTART( MBHandle, ModelBase, MBMesh, MeshIndex, -1 ) ;

	// ���܂łƃt���O�������ł���ꍇ�͉������Ȃ�
	if( MBMesh->UseVertexDiffuseColor == UseFlag )
		return 0 ;

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �ݒ��ۑ�����
	MBMesh->UseVertexDiffuseColor = UseFlag ;

	// ���̃��b�V�����g�p���Ă��郂�f���̔��������L�����̃Z�b�g�A�b�v�����t���O��|��
	for( Model = ModelBase->UseFirst ; Model ; Model = Model->UseBaseDataNext )
	{
		Mesh = &Model->Mesh[ MeshIndex ] ;
		MV1MESH_RESET_SEMITRANSSETUP( Mesh )

		if( MV1CCHK( Mesh->DrawMaterialChange ) == 0 )
			MV1BitSetChange( &Mesh->DrawMaterialChange ) ;
	}

	// �I��
	return 0 ;
}

// �w��̃��b�V���̒��_�X�y�L�����J���[���}�e���A���̃X�y�L�����J���[�̑���Ɏg�p���邩�ǂ�����ݒ肷��( TRUE:�}�e���A���J���[�̑���Ɏg�p����  FALSE:�}�e���A���J���[���g�p���� )
extern int MV1SetMeshUseVertSpcColorBase( int MBHandle, int MeshIndex, int UseFlag )
{
	MV1BASEMESHSTART( MBHandle, ModelBase, MBMesh, MeshIndex, -1 ) ;

	// ���܂łƃt���O�������ł���ꍇ�͉������Ȃ�
	if( MBMesh->UseVertexSpecularColor == UseFlag )
		return 0 ;

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �ݒ��ۑ�����
	MBMesh->UseVertexSpecularColor = UseFlag ;

	// �I��
	return 0 ;
}

// �w��̃��b�V���̒��_�f�B�t���[�Y�J���[���}�e���A���̃f�B�t���[�Y�J���[�̑���Ɏg�p���邩�ǂ����̐ݒ���擾����( �߂�l  TRUE:�}�e���A���J���[�̑���Ɏg�p����  FALSE:�}�e���A���J���[���g�p���� )
extern int MV1GetMeshUseVertDifColorBase( int MBHandle, int MeshIndex )
{
	MV1BASEMESHSTART( MBHandle, ModelBase, Mesh, MeshIndex, -1 ) ;

	return Mesh->UseVertexDiffuseColor ;
}

// �w��̃��b�V���̒��_�X�y�L�����J���[���}�e���A���̃X�y�L�����J���[�̑���Ɏg�p���邩�ǂ����̐ݒ���擾����( �߂�l  TRUE:�}�e���A���J���[�̑���Ɏg�p����  FALSE:�}�e���A���J���[���g�p���� )
extern int MV1GetMeshUseVertSpcColorBase( int MBHandle, int MeshIndex )
{
	MV1BASEMESHSTART( MBHandle, ModelBase, Mesh, MeshIndex, -1 ) ;

	return Mesh->UseVertexSpecularColor ;
}

// �w��̃��b�V�����V�F�C�v���b�V�����ǂ������擾����( �߂�l TRUE:�V�F�C�v���b�V��  FALSE:�ʏ탁�b�V�� )
extern int MV1GetMeshShapeFlagBase( int MBHandle, int MeshIndex )
{
	MV1BASEMESHSTART( MBHandle, ModelBase, Mesh, MeshIndex, -1 ) ;

	return Mesh->Shape ;
}













// �V�F�C�v�֌W

// ���f���Ɋ܂܂��V�F�C�v�̐����擾����
extern int MV1GetShapeNumBase( int MBHandle )
{
	MV1_MODEL_BASE *ModelBase ;

	// ����������Ă��Ȃ�������G���[
	if( MV1Man.Initialize == false ) return -1 ;

	// �A�h���X�擾
	if( MV1BMDLCHK( MBHandle, ModelBase ) )
		return -1 ;

	// �V�F�C�v�̐���Ԃ�
	return ModelBase->ShapeNum ;
}

// �V�F�C�v�̖��O���烂�f�����̃V�F�C�v�̃V�F�C�v�C���f�b�N�X���擾����( ���������ꍇ�͖߂�l��-1 )
extern int MV1SearchShapeBase( int MBHandle, const wchar_t *ShapeName )
{
	MV1_MODEL_BASE *ModelBase ;
	MV1_SHAPE_BASE *MBShape ;
	int i ;

	// ����������Ă��Ȃ�������G���[
	if( MV1Man.Initialize == false ) return -1 ;

	// �A�h���X�擾
	if( MV1BMDLCHK( MBHandle, ModelBase ) )
		return -1 ;

	// �����̃V�F�C�v��T��
	MBShape = ModelBase->Shape ;
	for( i = 0 ; i < ModelBase->ShapeNum && _WCSCMP( MBShape->NameW, ShapeName ) != 0 ; i ++, MBShape ++ ){}

	// �V�F�C�v�̃C���f�b�N�X��Ԃ�
	return i == ModelBase->ShapeNum ? -2 : i ;
}


#ifndef UNICODE
// �w��V�F�C�v�̖��O���擾����
extern const char *MV1GetShapeNameBaseA( int MBHandle, int ShapeIndex )
{
	MV1BASESHAPESTART( MBHandle, ModelBase, Shape, ShapeIndex, NULL ) ;

	// �V�F�C�v�̖��O��Ԃ�
	return Shape->NameA ;
}
#endif

// �w��V�F�C�v�̖��O���擾����
extern const wchar_t *MV1GetShapeNameBaseW( int MBHandle, int ShapeIndex )
{
	MV1BASESHAPESTART( MBHandle, ModelBase, Shape, ShapeIndex, NULL ) ;

	// �V�F�C�v�̖��O��Ԃ�
	return Shape->NameW ;
}

// �w��V�F�C�v���ΏۂƂ��Ă��郁�b�V���̐����擾����
extern int MV1GetShapeTargetMeshNumBase( int MBHandle, int ShapeIndex )
{
	MV1BASESHAPESTART( MBHandle, ModelBase, Shape, ShapeIndex, -1 ) ;

	return Shape->MeshNum ;
}

// �w��V�F�C�v���ΏۂƂ��Ă��郁�b�V���̃��b�V���C���f�b�N�X���擾����
extern int MV1GetShapeTargetMeshBase( int MBHandle, int ShapeIndex, int Index )
{
	MV1BASESHAPESTART( MBHandle, ModelBase, Shape, ShapeIndex, -1 ) ;

	if( Index < 0 || Shape->MeshNum <= Index )
	{
		return -1 ;
	}

	return ( int )( Shape->Mesh[ Index ].TargetMesh - ModelBase->Mesh ) ;
}
























// ���f���f�[�^�n���h���̏�����
extern int InitializeModelHandle( HANDLEINFO * )
{
	// ���ɉ������Ȃ�
	return 0 ;
}

// ���f���f�[�^�n���h���̌�n��
extern int TerminateModelHandle( HANDLEINFO *HandleInfo )
{
	MV1_MODEL *Model = ( MV1_MODEL * )HandleInfo ;
	MV1_TEXTURE *Texture ;
	int i ;

	// ����������łȂ���Ή���������s��
	if( Model->BaseData != NULL )
	{
		// �A�j���[�V�����̃f�^�b�`
		for( i = 0 ; i < Model->AnimSetMaxNum ; i ++ )
			NS_MV1DetachAnim( Model->HandleInfo.Handle, i ) ;

		// ���_�f�[�^�̉��
		MV1_TerminateVertexBuffer_PF( Model->HandleInfo.Handle ) ;

		// �A�j���[�V�����|�C���^�i�[�p�������̉��
		if( Model->AnimSet )
		{
			DXFREE( Model->AnimSet ) ;
			Model->AnimSet = NULL ;
		}

		// �Q�Ɨp���b�V���̉��
		NS_MV1TerminateReferenceMesh( Model->HandleInfo.Handle, -1, FALSE, FALSE ) ;
		NS_MV1TerminateReferenceMesh( Model->HandleInfo.Handle, -1, TRUE,  FALSE ) ;
		NS_MV1TerminateReferenceMesh( Model->HandleInfo.Handle, -1, FALSE, TRUE ) ;
		NS_MV1TerminateReferenceMesh( Model->HandleInfo.Handle, -1, TRUE,  TRUE ) ;
		for( i = 0 ; i < Model->BaseData->FrameNum ; i ++ )
		{
			NS_MV1TerminateReferenceMesh( Model->HandleInfo.Handle, i, FALSE, FALSE ) ;
			NS_MV1TerminateReferenceMesh( Model->HandleInfo.Handle, i, TRUE,  FALSE ) ;
			NS_MV1TerminateReferenceMesh( Model->HandleInfo.Handle, i, FALSE, TRUE ) ;
			NS_MV1TerminateReferenceMesh( Model->HandleInfo.Handle, i, TRUE,  TRUE ) ;
		}

		// �R���W�������̉��
		NS_MV1TerminateCollInfo( Model->HandleInfo.Handle, -1 ) ;
		for( i = 0 ; i < Model->BaseData->FrameNum ; i ++ )
		{
			NS_MV1TerminateCollInfo( Model->HandleInfo.Handle, i ) ;
		}

#ifndef DX_NON_BULLET_PHYSICS
		// �������Z�����̌�n��
		if( Model->BaseData->PhysicsRigidBodyNum != 0 )
			ReleasePhysicsObject_ModelPhysicsInfo( Model ) ;
#endif

		// �e�N�X�`���̍폜����
		Texture = Model->Texture ;
		for( i = 0 ; i < Model->BaseData->TextureNum ; i ++, Texture ++ )
		{
			if( Texture->UseGraphHandle )
			{
#ifndef UNICODE
				if( Texture->ColorFilePathA_ )
				{
					DXFREE( Texture->ColorFilePathA_ ) ;
					Texture->ColorFilePathA_ = NULL ;
				}

				if( Texture->AlphaFilePathA_ )
				{
					DXFREE( Texture->AlphaFilePathA_ ) ;
					Texture->AlphaFilePathA_ = NULL ;
				}
#endif
				if( Texture->ColorFilePathW_ )
				{
					DXFREE( Texture->ColorFilePathW_ ) ;
					Texture->ColorFilePathW_ = NULL ;
				}

				if( Texture->AlphaFilePathW_ )
				{
					DXFREE( Texture->AlphaFilePathW_ ) ;
					Texture->AlphaFilePathW_ = NULL ;
				}

				if( Texture->ColorImage )
				{
					DXFREE( Texture->ColorImage ) ;
					Texture->ColorImage = NULL ;
				}

				if( Texture->AlphaImage )
				{
					DXFREE( Texture->AlphaImage ) ;
					Texture->AlphaImage = NULL ;
				}

				NS_DeleteGraph( Texture->GraphHandle ) ;
				Texture->GraphHandle = -1 ;
				Texture->UseGraphHandle = 0 ;
			}
		}
		if( Model->TextureAllocMem )
		{
			DXFREE( Model->Texture ) ;
			Model->Texture = NULL ;
		}

		// ���X�g����͂���
		if( Model->UseBaseDataNext )
		{
			Model->UseBaseDataNext->UseBaseDataPrev = Model->UseBaseDataPrev ;
		}
		if( Model->UseBaseDataPrev )
		{
			Model->UseBaseDataPrev->UseBaseDataNext = Model->UseBaseDataNext ;
		}
		if( Model->BaseData->UseFirst == Model )
		{
			Model->BaseData->UseFirst = Model->UseBaseDataNext ;
		}
		if( Model->BaseData->UseLast == Model )
		{
			Model->BaseData->UseLast = Model->UseBaseDataPrev ;
		}
		Model->BaseData->UseNum -- ;

		// ���f����{�f�[�^�̉��
		MV1SubModelBase( Model->BaseDataHandle ) ;
	}

	// �f�[�^�o�b�t�@�̉��
	if( Model->DataBuffer )
	{
		DXFREE( Model->DataBuffer ) ;
		Model->DataBuffer = NULL ;
	}

	// ����I��
	return 0 ;
}

// ���f���f�[�^��ǉ�����( -1:�G���[  0�ȏ�:���f���f�[�^�n���h�� )
extern int MV1AddModel( int ASyncThread )
{
	return AddHandle( DX_HANDLETYPE_MODEL, ASyncThread, -1 ) ;
}

// ���f����f�[�^���烂�f���f�[�^���\�z����( -1:�G���[ 0:���� )
extern int MV1MakeModel( int MV1ModelHandle, int MV1ModelBaseHandle, int ASyncThread )
{
	MV1_MODEL *Model ;
	MV1_MODEL_BASE *MBase ;
	MV1_FRAME *Frame ;
	MV1_FRAME_BASE *MBFrame ;
	MV1_MESH_BASE *MBMesh ;
	MV1_MESH *Mesh ;
	MV1_SHAPE_BASE *MBShape ;
	MV1_SHAPE *Shape ;
	MV1_TRIANGLE_LIST_BASE *MBTList ;
	MV1_TRIANGLE_LIST *TList ;
	MV1_MATERIAL *Material ;
	MV1_MATERIAL_BASE *MBMaterial ;
	MV1_MATERIAL_LAYER *Layer, *MBLayer ;
	MV1_TEXTURE_BASE *MBTexture ;
	MV1_TEXTURE *Texture ;
	MV1_PHYSICS_RIGIDBODY_BASE *MBPhysicsRigidBody ;
	MV1_PHYSICS_RIGIDBODY *PhysicsRigidBody ;
	MV1_PHYSICS_JOINT_BASE *MBPhysicsJoint ;
	MV1_PHYSICS_JOINT *PhysicsJoint ;
	int i, j, k ;
	DWORD Size, Num ;
	MV1_DRAW_MATERIAL InitDrawMat ;
	int ShapeNormalPositionNum ;
	int ShapeSkinPosition4BNum ;
	int ShapeSkinPosition8BNum ;
	int ShapeSkinPositionFREEBSize ;
	DWORD ChangeDrawMaterialFlagSize, ChangeMatrixFlagSize ;

	ShapeNormalPositionNum = 0 ;
	ShapeSkinPosition4BNum = 0 ;
	ShapeSkinPosition8BNum = 0 ;
	ShapeSkinPositionFREEBSize = 0 ;

	// �����l�̃Z�b�g
	InitDrawMat.UseColorScale = false ;
	InitDrawMat.DiffuseScale.r = 1.0f ;
	InitDrawMat.DiffuseScale.g = 1.0f ;
	InitDrawMat.DiffuseScale.b = 1.0f ;
	InitDrawMat.DiffuseScale.a = 1.0f ;
	InitDrawMat.AmbientScale.r = 1.0f ;
	InitDrawMat.AmbientScale.g = 1.0f ;
	InitDrawMat.AmbientScale.b = 1.0f ;
	InitDrawMat.AmbientScale.a = 1.0f ;
	InitDrawMat.SpecularScale.r = 1.0f ;
	InitDrawMat.SpecularScale.g = 1.0f ;
	InitDrawMat.SpecularScale.b = 1.0f ;
	InitDrawMat.SpecularScale.a = 1.0f ;
	InitDrawMat.EmissiveScale.r = 1.0f ;
	InitDrawMat.EmissiveScale.g = 1.0f ;
	InitDrawMat.EmissiveScale.b = 1.0f ;
	InitDrawMat.EmissiveScale.a = 1.0f ;
	InitDrawMat.OpacityRate = 1.0f ;
	InitDrawMat.Visible = 1 ;

	// ���f����{�f�[�^�n���h���`�F�b�N
	if( MV1BMDLCHK( MV1ModelBaseHandle, MBase ) )
		return -1 ;

	// ���f���f�[�^�n���h���`�F�b�N
	if( ASyncThread )
	{
		if( MV1MDLCHK_ASYNC( MV1ModelHandle, Model ) )
			return -1 ;
	}
	else
	{
		if( MV1MDLCHK( MV1ModelHandle, Model ) )
			return -1 ;
	}

	// ���f���f�[�^���i�[���郁�����̈�̊m��
	ChangeDrawMaterialFlagSize = ( DWORD )( ( 1 + MBase->FrameNum + MBase->MeshNum + 31 ) / 32 ) ;
	ChangeMatrixFlagSize       = ( DWORD )( ( 1 + MBase->FrameNum                  + 31 ) / 32 ) ;
	Size = 
		sizeof( MV1_FRAME             ) * MBase->FrameNum * 2             +
		sizeof( MATRIX_4X4CT          ) * MBase->SkinBoneNum              + 16 +
		sizeof( MATRIX_4X4CT_F        ) * ( MBase->PackDrawMatrixUnitNum * MBase->PackDrawMaxNum ) +
		sizeof( MATRIX_4X4CT *        ) * MBase->FrameUseSkinBoneNum      +
		sizeof( MV1_MESH              ) * MBase->MeshNum                  + 
		sizeof( MV1_SHAPE             ) * MBase->ShapeNum                 + 
		( sizeof( MV1_TRIANGLE_LIST ) + sizeof( MV1_TRIANGLE_LIST_PF ) ) * MBase->TriangleListNum          +
		sizeof( MV1_MATERIAL          ) * MBase->MaterialNum              +
		sizeof( MV1_TEXTURE           ) * MBase->TextureNum               +
		sizeof( MV1_TLIST_NORMAL_POS  ) * MBase->ShapeNormalPositionNum   + 16 +
		sizeof( MV1_TLIST_SKIN_POS_4B ) * MBase->ShapeSkinPosition4BNum   +
		sizeof( MV1_TLIST_SKIN_POS_8B ) * MBase->ShapeSkinPosition8BNum   +
		MBase->ShapeSkinPositionFREEBSize + 
		ChangeDrawMaterialFlagSize * 4    +
		ChangeMatrixFlagSize       * 4    +
		sizeof( MV1_SHAPE_VERTEX      ) * MBase->ShapeTargetMeshVertexNum +
		sizeof( MV1_PHYSICS_RIGIDBODY ) * MBase->PhysicsRigidBodyNum      +
		sizeof( MV1_PHYSICS_JOINT     ) * MBase->PhysicsJointNum ;
	Model->DataBuffer = ( MV1_MODEL * )MDALLOCMEM( Size ) ;
	if( Model->DataBuffer == NULL ) return -1 ;
	_MEMSET( Model->DataBuffer, 0, Size ) ;
	Model->Frame                    = ( MV1_FRAME                * )( Model->DataBuffer                                            ) ;
	Model->Mesh                     = ( MV1_MESH                 * )( Model->Frame                    + MBase->FrameNum * 2        ) ;
	Model->Shape                    = ( MV1_SHAPE                * )( Model->Mesh                     + MBase->MeshNum             ) ;
	Model->TriangleList             = ( MV1_TRIANGLE_LIST        * )( Model->Shape                    + MBase->ShapeNum            ) ;
	Model->Material                 = ( MV1_MATERIAL             * )( ( BYTE * )Model->TriangleList + ( sizeof( MV1_TRIANGLE_LIST ) + sizeof( MV1_TRIANGLE_LIST_PF ) ) * MBase->TriangleListNum ) ;
	Model->Texture                  = ( MV1_TEXTURE              * )( Model->Material                 + MBase->MaterialNum         ) ;
	Model->PhysicsRigidBody         = ( MV1_PHYSICS_RIGIDBODY    * )( Model->Texture                  + MBase->TextureNum          ) ;
	Model->PhysicsJoint             = ( MV1_PHYSICS_JOINT        * )( Model->PhysicsRigidBody         + MBase->PhysicsRigidBodyNum ) ;
	Model->ChangeDrawMaterialFlag   = ( DWORD                    * )( Model->PhysicsJoint             + MBase->PhysicsJointNum     ) ;
	Model->ChangeMatrixFlag         = ( DWORD                    * )( Model->ChangeDrawMaterialFlag   + ChangeDrawMaterialFlagSize ) ;
	Model->SkinBoneMatrix           = ( MATRIX_4X4CT             * )( ( ( DWORD_PTR )( Model->ChangeMatrixFlag + ChangeMatrixFlagSize ) + 15 ) / 16 * 16 ) ;
	Model->PackDrawMatrix           = ( MATRIX_4X4CT_F           * )( Model->SkinBoneMatrix           + MBase->SkinBoneNum         ) ;
	Model->SkinBoneUseFrameMatrix   = ( MATRIX_4X4CT            ** )( Model->PackDrawMatrix           + MBase->PackDrawMatrixUnitNum * MBase->PackDrawMaxNum ) ;
	Model->ShapeVertex              = ( MV1_SHAPE_VERTEX         * )( Model->SkinBoneUseFrameMatrix   + MBase->FrameUseSkinBoneNum ) ;
	Model->ShapeNormalPosition      = ( MV1_TLIST_NORMAL_POS     * )( ( ( DWORD_PTR )( Model->ShapeVertex + MBase->ShapeTargetMeshVertexNum ) + 15 ) / 16 * 16 ) ;
	Model->ShapeSkinPosition4B      = ( MV1_TLIST_SKIN_POS_4B    * )( Model->ShapeNormalPosition      + MBase->ShapeNormalPositionNum ) ;
	Model->ShapeSkinPosition8B      = ( MV1_TLIST_SKIN_POS_8B    * )( Model->ShapeSkinPosition4B      + MBase->ShapeSkinPosition4BNum ) ;
	Model->ShapeSkinPositionFREEB   = ( MV1_TLIST_SKIN_POS_FREEB * )( Model->ShapeSkinPosition8B      + MBase->ShapeSkinPosition8BNum ) ;

	// ���X�g�ɂȂ�
	if( MBase->UseFirst )
	{
		Model->UseBaseDataNext = MBase->UseFirst ;
		MBase->UseFirst->UseBaseDataPrev = Model ;
		Model->UseBaseDataPrev = NULL ;
		MBase->UseFirst = Model ;
	}
	else
	{
		MBase->UseFirst = Model ;
		MBase->UseLast = Model ;
		Model->UseBaseDataNext = NULL ;
		Model->UseBaseDataPrev = NULL ;
	}
	MBase->UseNum ++ ;

	// ���f�������Z�b�g
	Model->BaseDataHandle = MV1ModelBaseHandle ;
	Model->BaseData = MBase ;

	// �A�j���[�V�����͍ŏ��A�^�b�`����Ă��Ȃ��̂ŃZ�b�g�A�b�v�͊������Ă���
	Model->AnimSetupFlag = true ;

	// �V�F�C�v���͍ŏ��͕K���X�V����
	Model->ShapeChangeFlag = true ;

	// �d�̓p�����[�^���Z�b�g
	Model->PhysicsGravity = MBase->PhysicsGravity ;

	// �X�P�[���͓��{
	Model->Scale.x = 1.0f ;
	Model->Scale.y = 1.0f ;
	Model->Scale.z = 1.0f ;

	// ���b�V���J�e�S�����̔�\���t���O���Z�b�g
	Model->MeshCategoryHide[ DX_MV1_MESHCATEGORY_NORMAL ] = FALSE ;
	Model->MeshCategoryHide[ DX_MV1_MESHCATEGORY_OUTLINE ] = FALSE ;
	Model->MeshCategoryHide[ DX_MV1_MESHCATEGORY_OUTLINE_ORIG_SHADER ] = TRUE ;

	// �y�o�b�t�@�֌W���Z�b�g
	Model->EnableZBufferFlag = TRUE ;
	Model->WriteZBufferFlag = TRUE ;
	Model->ZBufferCmpType = DX_CMP_LESSEQUAL ;
	Model->ZBias = 0 ;

	// �ٕ����t�B���^�����O�̍ő原���̏����l�͂Q
	Model->MaxAnisotropy = 2 ;

	// �f�t�H���g�ł̓��C���[�t���[���`��͂��Ȃ�
	Model->WireFrame = false ;

	// ���f���̋��ʃ}�e���A���֌W��������
	Model->DrawMaterial = InitDrawMat ;

	// ��ԕύX���ꂽ��Ԃɂ���
	Model->ChangeDrawMaterialFlagSize = ChangeDrawMaterialFlagSize * 4 ;
	Model->ChangeMatrixFlagSize       = ChangeMatrixFlagSize       * 4 ;
	_MEMSET( Model->ChangeDrawMaterialFlag, 0xff, Model->ChangeDrawMaterialFlagSize ) ;
	_MEMSET( Model->ChangeMatrixFlag,       0xff, Model->ChangeMatrixFlagSize       ) ;

	// �e�N�X�`�����̃R�s�[
	Model->TextureAllocMem = FALSE ;
	MBTexture = MBase->Texture ;
	Texture = Model->Texture ;
	for( i = 0 ; i < MBase->TextureNum ; i ++, MBTexture ++, Texture ++ )
	{
		Texture->BaseData = MBTexture ;

#ifndef UNICODE
		Texture->AlphaFilePathA_ = MBTexture->AlphaFilePathA ;
#endif
		Texture->AlphaFilePathW_ = MBTexture->AlphaFilePathW ;
		Texture->AlphaImage = NULL ;
		Texture->AlphaImageSize = 0 ;

#ifndef UNICODE
		Texture->ColorFilePathA_ = MBTexture->ColorFilePathA ;
#endif
		Texture->ColorFilePathW_ = MBTexture->ColorFilePathW ;
		Texture->ColorImage = NULL ;
		Texture->ColorImageSize = 0 ;

		Texture->Width = 0 ;
		Texture->Height = 0 ;
		Texture->SemiTransFlag = 0 ;
		Texture->BumpImageFlag = MBTexture->BumpImageFlag ;
		Texture->BumpImageNextPixelLength = MBTexture->BumpImageNextPixelLength ;

		Texture->UseGraphHandle = 0 ;
		Texture->GraphHandle = -1 ;

		Texture->UseUserGraphHandle = MBTexture->UseUserGraphHandle ;
		Texture->UserGraphHandle = MBTexture->UserGraphHandle ;
		Texture->UserGraphWidth = MBTexture->UserGraphWidth ;
		Texture->UserGraphHeight = MBTexture->UserGraphHeight ;
		Texture->UserGraphHandleSemiTransFlag = MBTexture->UserGraphHandleSemiTransFlag ;

		Texture->AddressModeU = MBTexture->AddressModeU ;
		Texture->AddressModeV = MBTexture->AddressModeV ;
		Texture->ScaleU = MBTexture->ScaleU ;
		Texture->ScaleV = MBTexture->ScaleV ;
		Texture->FilterMode = MBTexture->FilterMode ;

		Texture->ReverseFlag = MBTexture->ReverseFlag ;
		Texture->Bmp32AllZeroAlphaToXRGB8Flag = MBTexture->Bmp32AllZeroAlphaToXRGB8Flag ;
	}

	// �}�e���A�����̃R�s�[
	MBMaterial = MBase->Material ;
	Material = Model->Material ;
	for( i = 0 ; i < MBase->MaterialNum ; i ++, MBMaterial ++, Material ++ )
	{
		Material->BaseData = MBMaterial ;

//		Material->Type = MBMaterial->Type ;

		Material->Diffuse = MBMaterial->Diffuse ;
		Material->Ambient = MBMaterial->Ambient ;
		Material->Specular = MBMaterial->Specular ;
		Material->Emissive = MBMaterial->Emissive ;
		Material->Power = MBMaterial->Power ;

		Material->DiffuseLayerNum = MBMaterial->DiffuseLayerNum ;
		Layer = Material->DiffuseLayer ;
		MBLayer = MBMaterial->DiffuseLayer ;
		for( j = 0 ; j < MBMaterial->DiffuseLayerNum ; j ++, Layer ++, MBLayer ++ )
		{
			Layer->IsGraphHandleAlpha = MBLayer->IsGraphHandleAlpha ;
			Layer->GraphHandle        = MBLayer->GraphHandle ;
			Layer->BlendType          = MBLayer->BlendType ;
			Layer->Texture            = MBLayer->Texture ;
//			MV1AddTexture( MBase->Texture[ Layer->Texture ] ) ;
		}

		Material->SpecularLayerNum = MBMaterial->SpecularLayerNum ;
		Layer = Material->SpecularLayer ;
		MBLayer = MBMaterial->SpecularLayer ;
		for( j = 0 ; j < MBMaterial->SpecularLayerNum ; j ++, Layer ++, MBLayer ++ )
		{
			Layer->IsGraphHandleAlpha = MBLayer->IsGraphHandleAlpha ;
			Layer->GraphHandle        = MBLayer->GraphHandle ;
			Layer->BlendType          = MBLayer->BlendType ;
			Layer->Texture            = MBLayer->Texture ;
//			MV1AddTexture( MBase->Texture[ Layer->Texture ] ) ;
		}

		Material->DiffuseGradTexture = MBMaterial->DiffuseGradTexture ;
		Material->SpecularGradTexture = MBMaterial->SpecularGradTexture ;
		Material->SphereMapTexture = MBMaterial->SphereMapTexture ;
		Material->DiffuseGradBlendType = MBMaterial->DiffuseGradBlendType ;
		Material->SpecularGradBlendType = MBMaterial->SpecularGradBlendType ;
		Material->SphereMapBlendType = MBMaterial->SphereMapBlendType ;
		Material->OutLineWidth = MBMaterial->OutLineWidth ;
		Material->OutLineDotWidth = MBMaterial->OutLineDotWidth ;
		Material->OutLineColor = MBMaterial->OutLineColor ;

		Material->UseAlphaTest = MBMaterial->UseAlphaTest ;
		Material->AlphaFunc = MBMaterial->AlphaFunc ;
		Material->AlphaRef = MBMaterial->AlphaRef ;
		Material->DrawBlendMode = MBMaterial->DrawBlendMode ;
		Material->DrawBlendParam = MBMaterial->DrawBlendParam ;
	}

	// �������Z�̍��̏����Z�b�g�A�b�v
	MBPhysicsRigidBody = MBase->PhysicsRigidBody ;
	PhysicsRigidBody = Model->PhysicsRigidBody ;
	for( i = 0 ; i < MBase->PhysicsRigidBodyNum ; i ++, MBPhysicsRigidBody ++, PhysicsRigidBody ++ )
	{
		PhysicsRigidBody->BaseData = MBPhysicsRigidBody ;
		PhysicsRigidBody->TargetFrame = &Model->Frame[ MBPhysicsRigidBody->TargetFrame->Index ] ;
	}

	// �������Z�̃W���C���g�����Z�b�g�A�b�v
	MBPhysicsJoint = MBase->PhysicsJoint ;
	PhysicsJoint = Model->PhysicsJoint ;
	for( i = 0 ; i < MBase->PhysicsJointNum ; i ++, MBPhysicsJoint ++, PhysicsJoint ++ )
	{
		PhysicsJoint->BaseData = MBPhysicsJoint ;
	}

	// �g���C�A���O�����X�g�̏����Z�b�g
	MBTList = MBase->TriangleList ;
	TList = Model->TriangleList ;
	for( i = 0 ; i < MBase->TriangleListNum ; i ++, MBTList ++, TList = ( MV1_TRIANGLE_LIST * )( ( BYTE * )TList + sizeof( MV1_TRIANGLE_LIST ) + sizeof( MV1_TRIANGLE_LIST_PF ) ) )
	{
		TList->PF = ( MV1_TRIANGLE_LIST_PF * )( TList + 1 ) ;
		TList->BaseData = MBTList ;
		TList->Container = Model->Mesh + ( MBTList->Container - MBase->Mesh ) ;
		TList->NormalPosition = NULL ;
	}

	// �V�F�C�v�̏����Z�b�g
	MBShape = MBase->Shape ;
	Shape = Model->Shape ;
	for( i = 0 ; i < MBase->ShapeNum ; i ++, MBShape ++, Shape ++ )
	{
		Shape->Container = Model->Frame + ( MBShape->Container - MBase->Frame ) ;
		Shape->BaseData = MBShape ;

		Shape->Rate = 0.0f ;
	}

	// ���b�V���̏����Z�b�g
	MBMesh = MBase->Mesh ;
	Mesh = Model->Mesh ;
	j = 0 ;
	for( i = 0 ; i < MBase->MeshNum ; i ++, MBMesh ++, Mesh ++ )
	{
		Mesh->Container = Model->Frame + ( MBMesh->Container - MBase->Frame ) ;
		Mesh->DrawMaterialChange.BaseData = &MBMesh->ChangeInfo ;
		Mesh->Material = Model->Material + ( MBMesh->Material - MBase->Material ) ;
		Mesh->DrawMaterialChange.Target   = Model->ChangeDrawMaterialFlag + MBMesh->ChangeInfo.Target ;
		Mesh->BaseData = MBMesh ;
		Mesh->DrawMaterial = InitDrawMat ;
		Mesh->DrawMaterial.Visible = MBMesh->Visible ;
		Mesh->DrawBlendMode = -1 ;
		Mesh->DrawBlendParam = -1 ;
		Mesh->SemiTransStateSetupFlag = false ;

		if( MBMesh->TriangleListNum )
		{
			Mesh->TriangleList = ( MV1_TRIANGLE_LIST * )( ( BYTE * )Model->TriangleList + ( MBMesh->TriangleList - MBase->TriangleList ) * ( sizeof( MV1_TRIANGLE_LIST ) + sizeof( MV1_TRIANGLE_LIST_PF ) ) );

			if( MBMesh->Shape )
			{
				Mesh->ShapeVertex = Model->ShapeVertex + j ;
				j += MBMesh->VertexNum ;

				TList = Mesh->TriangleList ;
				MBTList = MBMesh->TriangleList ;
				for( k = 0 ; k < MBMesh->TriangleListNum ; k ++, MBTList ++, TList = ( MV1_TRIANGLE_LIST * )( ( BYTE * )TList + sizeof( MV1_TRIANGLE_LIST ) + sizeof( MV1_TRIANGLE_LIST_PF ) ) )
				{
					switch( MBTList->VertexType )
					{
					case MV1_VERTEX_TYPE_NORMAL :
						TList->NormalPosition = Model->ShapeNormalPosition + ShapeNormalPositionNum ;
						ShapeNormalPositionNum += MBTList->VertexNum ;
						break ;

					case MV1_VERTEX_TYPE_SKIN_4BONE :
						TList->SkinPosition4B = Model->ShapeSkinPosition4B + ShapeSkinPosition4BNum ;
						ShapeSkinPosition4BNum += MBTList->VertexNum ;
						break ;

					case MV1_VERTEX_TYPE_SKIN_8BONE :
						TList->SkinPosition8B = Model->ShapeSkinPosition8B + ShapeSkinPosition8BNum ;
						ShapeSkinPosition8BNum += MBTList->VertexNum ;
						break ;

					case MV1_VERTEX_TYPE_SKIN_FREEBONE :
						TList->SkinPositionFREEB = ( MV1_TLIST_SKIN_POS_FREEB * )( ( BYTE * )Model->ShapeSkinPositionFREEB + ShapeSkinPositionFREEBSize ) ;
						ShapeSkinPositionFREEBSize += MBTList->VertexNum * MBTList->PosUnitSize ;
						break ;
					}

					MV1SetupShapeTriangleListPositionAndNormal( TList ) ;
				}
			}
		}
	}

	// �t���[���̏����Z�b�g
	Model->TopFrameNum = 0 ;
	Frame = Model->Frame ;
	MBFrame = MBase->Frame ;
	for( i = 0 ; i < MBase->FrameNum ; i ++, Frame ++, MBFrame ++ )
	{
		Frame->Container = Model ;
		Frame->ModelHandle = MV1ModelHandle ;
		Frame->BaseData = MBFrame ;
		Frame->Parent = MBFrame->Parent     == NULL ? NULL : Model->Frame + MBFrame->Parent->Index ;
		Frame->Child  = MBFrame->FirstChild == NULL ? NULL : Model->Frame + MBFrame->FirstChild->Index ;
		Frame->Prev   = MBFrame->Prev       == NULL ? NULL : Model->Frame + MBFrame->Prev->Index ;
		Frame->Next   = MBFrame->Next       == NULL ? NULL : Model->Frame + MBFrame->Next->Index ;
		Frame->ChildNum = 0 ;

		CreateIdentityMatrix( &Frame->TextureAddressTransformMatrix ) ;
		Frame->LocalWorldMatrixChange.BaseData = &MBFrame->ChangeMatrixInfo ;
		Frame->LocalWorldMatrixChange.Target   = Model->ChangeMatrixFlag + MBFrame->ChangeMatrixInfo.Target ;

		Frame->DrawMaterialChange.BaseData = &MBFrame->ChangeDrawMaterialInfo ;
		Frame->DrawMaterialChange.Target   = Model->ChangeDrawMaterialFlag + MBFrame->ChangeDrawMaterialInfo.Target ;

		Frame->DrawMaterial = InitDrawMat ;
		Frame->DrawMaterial.Visible = ( BYTE )( ( MBFrame->Flag & MV1_FRAMEFLAG_VISIBLE ) != 0 ? 1 : 0 ) ;

		if( MBFrame->MeshNum )
		{
			Frame->Mesh = ( MV1_MESH * )( Model->Mesh + ( MBFrame->Mesh - MBase->Mesh ) ) ;
		}
		else
		{
			Frame->Mesh = NULL ;
		}

		if( MBFrame->ShapeNum )
		{
			Frame->Shape = ( MV1_SHAPE * )( Model->Shape + ( MBFrame->Shape - MBase->Shape ) ) ;
		}
		else
		{
			Frame->Shape = NULL ;
		}

		if( Frame->Parent )
		{
			Frame->ChildIndex = Frame->Parent->ChildNum ;
			Frame->Parent->ChildNum ++ ;
		}
		else
		{
			Frame->ChildIndex = Model->TopFrameNum ;
			Model->TopFrameNum ++ ;
		}

		if( MBFrame->UseSkinBoneNum )
		{
			Frame->UseSkinBoneMatrix = Model->SkinBoneUseFrameMatrix + ( MBFrame->UseSkinBone - MBase->FrameUseSkinBone ) ;

			for( j = 0 ; j < MBFrame->UseSkinBoneNum ; j ++ )
			{
				Frame->UseSkinBoneMatrix[ j ] = &Model->SkinBoneMatrix[ MBFrame->UseSkinBone[ j ] - MBase->SkinBone ] ;
			}
		}

		Frame->ShapeChangeFlag = true ;
	}

	// �e�t���[���̎q���X�g���쐬����
	Model->TopFrameList = ( MV1_FRAME ** )( ( MV1_FRAME * )Model->DataBuffer + MBase->FrameNum ) ;
	Num     = ( DWORD )Model->TopFrameNum ;
	Frame   = Model->Frame ;
	MBFrame = MBase->Frame ;
	for( i = 0 ; i < MBase->FrameNum ; i ++, Frame ++, MBFrame ++ )
	{
		if( Frame->Parent )
		{
			Frame->Parent->ChildList[ Frame->ChildIndex ] = Frame ;
		}
		else
		{
			Model->TopFrameList[ Frame->ChildIndex ] = Frame ;
		}
		if( Frame->ChildNum )
		{
			Frame->ChildList = ( MV1_FRAME ** )( ( MV1_FRAME * )Model->DataBuffer + MBase->FrameNum ) + Num ;
			Num += Frame->ChildNum ;
		}
	}

	// ���f����{�f�[�^�̎Q�ƃJ�E���g���C���N�������g����
	MBase->RefCount ++ ;

	// �����s��̃Z�b�g�A�b�v
	MV1SetupMatrix( Model ) ;

	// �f�t�H���g�|�[�Y�̍ŏ����W�l�ƍő���W�l���Z�o����
	MV1GetMaxMinPosition( MV1ModelHandle, NULL, NULL, ASyncThread ) ;

	// ���_�o�b�t�@�̃Z�b�g�A�b�v���s��
	MV1_SetupVertexBuffer_PF( MV1ModelHandle, ASyncThread ) ;

#ifndef DX_NON_BULLET_PHYSICS
	// �������Z�̃Z�b�g�A�b�v
	if( MBase->PhysicsRigidBodyNum != 0 )
		SetupPhysicsObject_ModelPhysicsInfo( Model ) ;
#endif

	// ����I��
	return 0 ;
}

// ���f���̍ő咸�_���W�ƍŏ����_���W���擾����
extern int MV1GetMaxMinPosition( int MHandle, VECTOR *MaxPosition, VECTOR *MinPosition, int ASyncThread )
{
	MV1_MODEL *Model ;
	MV1_MODEL_BASE *MBase ;
	MV1_FRAME *Frame ;
	MV1_FRAME_BASE *MBFrame ;
	MV1_MESH_BASE *Mesh ;
	MV1_TRIANGLE_LIST_BASE *TList ;
	MATRIX_4X4CT BlendMat, *pBlendMat[ MV1_TRIANGLE_LIST_USE_BONE_MAX_NUM ] ;
	int i, k, l, m, n ;
	float Weight ;
	VECTOR DestVector ;
	MV1_TLIST_NORMAL_POS *Norm ;
	MV1_TLIST_SKIN_POS_4B *Skin4B ;
	MV1_TLIST_SKIN_POS_8B *Skin8B ;
	MV1_TLIST_SKIN_POS_FREEB *SkinFB ;
	MV1_SKINBONE_BLEND *VBlend ;

	// �A�h���X�擾
	if( ASyncThread )
	{
		if( MV1MDLCHK_ASYNC( MHandle, Model ) )
			return -1 ;
	}
	else
	{
		if( MV1MDLCHK( MHandle, Model ) )
			return -1 ;
	}
	MBase = Model->BaseData ;

	// �ő���W�ƍŏ����W���Z�o����Ă��Ȃ��ꍇ�͂����ŎZ�o����
	if( MBase->ValidMaxMinPosition == 0 )
	{
		// �s��̃Z�b�g�A�b�v
		MV1SetupMatrix( Model ) ;

		// ���ׂẴ��b�V���̒��_����ő���W�ƍŏ����W���Z�o����
		MBase->MaxPosition.x =
		MBase->MaxPosition.y =
		MBase->MaxPosition.z = -100000000.0f ;
		MBase->MinPosition.x =
		MBase->MinPosition.y =
		MBase->MinPosition.z =  100000000.0f ;

		// �t���[���̐������J��Ԃ�
		Frame = Model->Frame ;
		for( i = 0 ; i < Model->BaseData->FrameNum ; i ++ , Frame ++ )
		{
			MBFrame = Frame->BaseData ;

			// ���b�V���������������΂�
			if( MBFrame->MeshNum == 0 )
			{
				MBFrame->MaxPosition.x =
				MBFrame->MaxPosition.y =
				MBFrame->MaxPosition.z = 0.0f ;
				MBFrame->MinPosition.x =
				MBFrame->MinPosition.y =
				MBFrame->MinPosition.z = 0.0f ;
				continue ;
			}

			// �ő�E�ŏ��l�̏�����
			MBFrame->MaxPosition.x =
			MBFrame->MaxPosition.y =
			MBFrame->MaxPosition.z = -100000000.0f ;
			MBFrame->MinPosition.x =
			MBFrame->MinPosition.y =
			MBFrame->MinPosition.z =  100000000.0f ;

			// ���b�V���̐������J��Ԃ�
			Mesh = MBFrame->Mesh ;
			for( k = 0 ; k < MBFrame->MeshNum ; k ++ , Mesh ++ )
			{
				// �ő�E�ŏ��l�̏�����
				Mesh->MaxPosition.x =
				Mesh->MaxPosition.y =
				Mesh->MaxPosition.z = -100000000.0f ;
				Mesh->MinPosition.x =
				Mesh->MinPosition.y =
				Mesh->MinPosition.z =  100000000.0f ;

				// �g���C�A���O�����X�g�̐������J��Ԃ�
				TList = Mesh->TriangleList ;
				for( l = 0 ; l < Mesh->TriangleListNum ; l ++ , TList ++ )
				{
					// ���_�̃^�C�v�ɂ���ď����𕪊�
					switch( TList->VertexType )
					{
						// ���̃g���C�A���O�����X�g
					case MV1_VERTEX_TYPE_NORMAL :
						Norm = ( MV1_TLIST_NORMAL_POS * )ADDR16( TList->NormalPosition ) ;
						for( m = 0 ; m < TList->VertexNum ; m ++, Norm ++ )
						{
							VectorTransform4X4CT( &DestVector, ( VECTOR * )&Norm->Position, &Frame->LocalWorldMatrix ) ;
							if( MBase->MaxPosition.x < DestVector.x ) MBase->MaxPosition.x = DestVector.x ;
							if( MBase->MinPosition.x > DestVector.x ) MBase->MinPosition.x = DestVector.x ;
							if( MBase->MaxPosition.y < DestVector.y ) MBase->MaxPosition.y = DestVector.y ;
							if( MBase->MinPosition.y > DestVector.y ) MBase->MinPosition.y = DestVector.y ;
							if( MBase->MaxPosition.z < DestVector.z ) MBase->MaxPosition.z = DestVector.z ;
							if( MBase->MinPosition.z > DestVector.z ) MBase->MinPosition.z = DestVector.z ;

							if( Mesh->MaxPosition.x < Norm->Position.x ) Mesh->MaxPosition.x = Norm->Position.x ;
							if( Mesh->MinPosition.x > Norm->Position.x ) Mesh->MinPosition.x = Norm->Position.x ;
							if( Mesh->MaxPosition.y < Norm->Position.y ) Mesh->MaxPosition.y = Norm->Position.y ;
							if( Mesh->MinPosition.y > Norm->Position.y ) Mesh->MinPosition.y = Norm->Position.y ;
							if( Mesh->MaxPosition.z < Norm->Position.z ) Mesh->MaxPosition.z = Norm->Position.z ;
							if( Mesh->MinPosition.z > Norm->Position.z ) Mesh->MinPosition.z = Norm->Position.z ;
						}
						break ;

						// �S�ȓ��{�[���g���C�A���O�����X�g
					case MV1_VERTEX_TYPE_SKIN_4BONE :

						// �s��̃|�C���^������
						for( m = 0 ; m < TList->UseBoneNum ; m ++ )
						{
							pBlendMat[ m ] = Frame->UseSkinBoneMatrix[ TList->UseBone[ m ] ] ;
						}

						// �f�[�^�̃Z�b�g
						Skin4B = ( MV1_TLIST_SKIN_POS_4B * )ADDR16( TList->SkinPosition4B ) ;
						for( m = 0 ; m < TList->VertexNum ; m ++, Skin4B ++ )
						{
							// �u�����h�s��̍쐬
							_MEMSET( &BlendMat, 0, sizeof( BlendMat ) ) ;
							for( n = 0 ; n < 4 ; n ++ )
							{
								Weight = Skin4B->MatrixWeight[ n ] ;

								if( Weight == 0.0f ) continue ;
								UnSafeMatrix4X4CT_C_EqPlus_C_Mul_S( &BlendMat, pBlendMat[ Skin4B->MatrixIndex[ n ] ], Weight ) ;
							}

							VectorTransform4X4CT( &DestVector, ( VECTOR * )&Skin4B->Position, &BlendMat ) ;
							if( MBase->MaxPosition.x < DestVector.x ) MBase->MaxPosition.x = DestVector.x ;
							if( MBase->MinPosition.x > DestVector.x ) MBase->MinPosition.x = DestVector.x ;
							if( MBase->MaxPosition.y < DestVector.y ) MBase->MaxPosition.y = DestVector.y ;
							if( MBase->MinPosition.y > DestVector.y ) MBase->MinPosition.y = DestVector.y ;
							if( MBase->MaxPosition.z < DestVector.z ) MBase->MaxPosition.z = DestVector.z ;
							if( MBase->MinPosition.z > DestVector.z ) MBase->MinPosition.z = DestVector.z ;

							if( Mesh->MaxPosition.x < Skin4B->Position.x ) Mesh->MaxPosition.x = Skin4B->Position.x ;
							if( Mesh->MinPosition.x > Skin4B->Position.x ) Mesh->MinPosition.x = Skin4B->Position.x ;
							if( Mesh->MaxPosition.y < Skin4B->Position.y ) Mesh->MaxPosition.y = Skin4B->Position.y ;
							if( Mesh->MinPosition.y > Skin4B->Position.y ) Mesh->MinPosition.y = Skin4B->Position.y ;
							if( Mesh->MaxPosition.z < Skin4B->Position.z ) Mesh->MaxPosition.z = Skin4B->Position.z ;
							if( Mesh->MinPosition.z > Skin4B->Position.z ) Mesh->MinPosition.z = Skin4B->Position.z ;
						}
						break ;

						// �W�ȓ��{�[���g���C�A���O�����X�g
					case MV1_VERTEX_TYPE_SKIN_8BONE :

						// �s��̃|�C���^������
						for( m = 0 ; m < TList->UseBoneNum ; m ++ )
							pBlendMat[ m ] = Frame->UseSkinBoneMatrix[ TList->UseBone[ m ] ] ;

						// �f�[�^�̃Z�b�g
						Skin8B = ( MV1_TLIST_SKIN_POS_8B * )ADDR16( TList->SkinPosition8B ) ;
						for( m = 0 ; m < TList->VertexNum ; m ++, Skin8B ++ )
						{
							// �u�����h�s��̍쐬
							_MEMSET( &BlendMat, 0, sizeof( BlendMat ) ) ;
							for( n = 0 ; n < 8 ; n ++ )
							{
								Weight = Skin8B->MatrixWeight[ n ] ;

								if( Weight == 0.0f ) continue ;
								UnSafeMatrix4X4CT_C_EqPlus_C_Mul_S( &BlendMat, pBlendMat[ n < 4 ? Skin8B->MatrixIndex1[ n ] : Skin8B->MatrixIndex2[ n - 4 ] ], Weight ) ;
							}

							VectorTransform4X4CT( &DestVector, &Skin8B->Position, &BlendMat ) ;
							if( MBase->MaxPosition.x < DestVector.x ) MBase->MaxPosition.x = DestVector.x ;
							if( MBase->MinPosition.x > DestVector.x ) MBase->MinPosition.x = DestVector.x ;
							if( MBase->MaxPosition.y < DestVector.y ) MBase->MaxPosition.y = DestVector.y ;
							if( MBase->MinPosition.y > DestVector.y ) MBase->MinPosition.y = DestVector.y ;
							if( MBase->MaxPosition.z < DestVector.z ) MBase->MaxPosition.z = DestVector.z ;
							if( MBase->MinPosition.z > DestVector.z ) MBase->MinPosition.z = DestVector.z ;

							if( Mesh->MaxPosition.x < Skin8B->Position.x ) Mesh->MaxPosition.x = Skin8B->Position.x ;
							if( Mesh->MinPosition.x > Skin8B->Position.x ) Mesh->MinPosition.x = Skin8B->Position.x ;
							if( Mesh->MaxPosition.y < Skin8B->Position.y ) Mesh->MaxPosition.y = Skin8B->Position.y ;
							if( Mesh->MinPosition.y > Skin8B->Position.y ) Mesh->MinPosition.y = Skin8B->Position.y ;
							if( Mesh->MaxPosition.z < Skin8B->Position.z ) Mesh->MaxPosition.z = Skin8B->Position.z ;
							if( Mesh->MinPosition.z > Skin8B->Position.z ) Mesh->MinPosition.z = Skin8B->Position.z ;
						}
						break ;

						// �{�[�����������g���C�A���O�����X�g
					case MV1_VERTEX_TYPE_SKIN_FREEBONE :
						// �f�[�^�̃Z�b�g
						SkinFB = ( MV1_TLIST_SKIN_POS_FREEB * )ADDR16( TList->SkinPositionFREEB ) ;
						for( m = 0 ; m < TList->VertexNum ; m ++, SkinFB = ( MV1_TLIST_SKIN_POS_FREEB * )( ( BYTE * )SkinFB + TList->PosUnitSize ) )
						{
							// �g���Ă���{�[�����ЂƂ̏ꍇ�̓u�����h�s��̍쐬�̓X�L�b�v����
							if( SkinFB->MatrixWeight[ 0 ].W == 1.0f )
							{
								BlendMat = *Frame->UseSkinBoneMatrix[ SkinFB->MatrixWeight[ 0 ].Index ] ;
							}
							else
							{
								// �u�����h�s��̍쐬
								VBlend = SkinFB->MatrixWeight ;
								Weight = VBlend->W ;
								_MEMSET( &BlendMat, 0, sizeof( BlendMat ) ) ;
								for( n = 0 ; n < TList->MaxBoneNum && VBlend->Index != -1 ; n ++, VBlend ++ )
								{
									Weight = VBlend->W ;

									if( Weight == 0.0f ) continue ;
									UnSafeMatrix4X4CT_C_EqPlus_C_Mul_S( &BlendMat, Frame->UseSkinBoneMatrix[ VBlend->Index ], Weight ) ;
								}
							}

							VectorTransform4X4CT( &DestVector, ( VECTOR * )&SkinFB->Position, &BlendMat ) ;
							if( MBase->MaxPosition.x < DestVector.x ) MBase->MaxPosition.x = DestVector.x ;
							if( MBase->MinPosition.x > DestVector.x ) MBase->MinPosition.x = DestVector.x ;
							if( MBase->MaxPosition.y < DestVector.y ) MBase->MaxPosition.y = DestVector.y ;
							if( MBase->MinPosition.y > DestVector.y ) MBase->MinPosition.y = DestVector.y ;
							if( MBase->MaxPosition.z < DestVector.z ) MBase->MaxPosition.z = DestVector.z ;
							if( MBase->MinPosition.z > DestVector.z ) MBase->MinPosition.z = DestVector.z ;

							if( Mesh->MaxPosition.x < SkinFB->Position.x ) Mesh->MaxPosition.x = SkinFB->Position.x ;
							if( Mesh->MinPosition.x > SkinFB->Position.x ) Mesh->MinPosition.x = SkinFB->Position.x ;
							if( Mesh->MaxPosition.y < SkinFB->Position.y ) Mesh->MaxPosition.y = SkinFB->Position.y ;
							if( Mesh->MinPosition.y > SkinFB->Position.y ) Mesh->MinPosition.y = SkinFB->Position.y ;
							if( Mesh->MaxPosition.z < SkinFB->Position.z ) Mesh->MaxPosition.z = SkinFB->Position.z ;
							if( Mesh->MinPosition.z > SkinFB->Position.z ) Mesh->MinPosition.z = SkinFB->Position.z ;
						}
						break ;
					}
				}

				if( MBFrame->MaxPosition.x < Mesh->MaxPosition.x ) MBFrame->MaxPosition.x = Mesh->MaxPosition.x ;
				if( MBFrame->MinPosition.x > Mesh->MinPosition.x ) MBFrame->MinPosition.x = Mesh->MinPosition.x ;
				if( MBFrame->MaxPosition.y < Mesh->MaxPosition.y ) MBFrame->MaxPosition.y = Mesh->MaxPosition.y ;
				if( MBFrame->MinPosition.y > Mesh->MinPosition.y ) MBFrame->MinPosition.y = Mesh->MinPosition.y ;
				if( MBFrame->MaxPosition.z < Mesh->MaxPosition.z ) MBFrame->MaxPosition.z = Mesh->MaxPosition.z ;
				if( MBFrame->MinPosition.z > Mesh->MinPosition.z ) MBFrame->MinPosition.z = Mesh->MinPosition.z ;
			}

			MBFrame->ValidMaxMinPosition = 1 ;
		}

		// �Z�o���I�������L���t���O�𗧂Ă�
		MBase->ValidMaxMinPosition = 1 ;
	}

	// ���̃Z�b�g
	if( MaxPosition ) *MaxPosition = MBase->MaxPosition ;
	if( MinPosition ) *MinPosition = MBase->MinPosition ;

	// �I��
	return 0 ;
}

// ���f���f�[�^���폜����
extern int MV1SubModel( int MV1ModelHandle )
{
	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	return SubHandle( MV1ModelHandle ) ;
}

// MV1LoadModelFromMem �̎������֐�
static int MV1LoadModelFromMem_Static( 
	MV1LOADMODEL_GPARAM *GParam,
	int MHandle,
	const void *FileImage,
	int FileSize,
	int (* FileReadFunc )( const TCHAR *FilePath, void **FileImageAddr, int *FileSize, void *FileReadFuncData ),
	int (* FileReleaseFunc )( void *MemoryAddr, void *FileReadFuncData ),
	void *FileReadFuncData,
	int ASyncThread
)
{
	void *DataBuffer = NULL ;
	int NewBaseHandle = -1 ;
	MV1_MODEL_LOAD_PARAM LoadParam ;
	MV1_FILE_READ_FUNC FileReadFuncParam ;
	int i ;

	// ����������Ă��Ȃ�������G���[
	if( MV1Man.Initialize == false ) return -1 ;

	// �R�c���f���p�V�F�[�_�[�̏������`�F�b�N
	if( Graphics_Hardware_Shader_ModelCode_Init_PF() < 0 )
	{
		return -1 ;
	}

	// �t�@�C�����i�[���郁�������m��
	DataBuffer = DXALLOC( ( size_t )( FileSize + 1 ) ) ;
	if( DataBuffer == NULL )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\xe2\x30\xc7\x30\xeb\x30\xd5\x30\xa1\x30\xa4\x30\xeb\x30\xa4\x30\xe1\x30\xfc\x30\xb8\x30\x6e\x30\xc7\x30\xfc\x30\xbf\x30\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x00"/*@ L"���f���t�@�C���C���[�W�̃f�[�^���i�[���郁�����̈�̊m�ۂɎ��s���܂���" @*/ )) ;
		goto ERRORLABEL ;
	}

	// �f�[�^���R�s�[
	_MEMCPY( DataBuffer, FileImage,  ( size_t )FileSize ) ;

	// �e�L�X�g�`���̏ꍇ�p�ɏI�[�������Z�b�g
	( ( BYTE * )DataBuffer )[  FileSize ] = '\0' ;

	// �֐��̈���������
	FileReadFuncParam.Read		= FileReadFunc ;
	FileReadFuncParam.Release	= FileReleaseFunc ;
	FileReadFuncParam.Data		= FileReadFuncData ;

	LoadParam.GParam			= *GParam ;
	LoadParam.DataBuffer		= DataBuffer ;
	LoadParam.DataSize			= FileSize ;
	LoadParam.FilePath			= L"" ;
	LoadParam.Name				= L"" ;
	LoadParam.CurrentDir		= NULL ;
	LoadParam.FileReadFunc		= &FileReadFuncParam ;

	// �I���W�i�����f�����[�h�֐�������
	for( i = 0 ; i < MV1Man.AddLoadFuncNum ; i ++ )
	{
		NewBaseHandle = MV1Man.AddLoadFunc[ i ]( &LoadParam ) ;
		if( NewBaseHandle >= 0 ) goto LOADCOMPLABEL ;
	}

	// �l�u�P�t�@�C���Ƃ��ēǂݍ���ł݂�
	NewBaseHandle = MV1LoadModelToMV1( &LoadParam, ASyncThread ) ;
	if( NewBaseHandle >= 0 ) goto LOADCOMPLABEL ;

	// �l�p�n�t�@�C���Ƃ��ēǂݍ���ł݂�
	NewBaseHandle = MV1LoadModelToMQO( &LoadParam, ASyncThread ) ;
	if( NewBaseHandle >= 0 ) goto LOADCOMPLABEL ;

	// �w�t�@�C���Ƃ��ēǂݍ���ł݂�
	NewBaseHandle = MV1LoadModelToX( &LoadParam, ASyncThread ) ;
	if( NewBaseHandle >= 0 ) goto LOADCOMPLABEL ;

	// �o�l�w�t�@�C���Ƃ��ēǂݍ���ł݂�
	NewBaseHandle = MV1LoadModelToPMX( &LoadParam, ASyncThread ) ;
	if( NewBaseHandle >= 0 ) goto LOADCOMPLABEL ;

	// �o�l�c�t�@�C���Ƃ��ēǂݍ���ł݂�
	NewBaseHandle = MV1LoadModelToPMD( &LoadParam, ASyncThread ) ;
	if( NewBaseHandle >= 0 ) goto LOADCOMPLABEL ;

	// �u�l�c�t�@�C���Ƃ��ēǂݍ���ł݂�
	NewBaseHandle = MV1LoadModelToVMD( &LoadParam, ASyncThread ) ;
	if( NewBaseHandle >= 0 ) goto LOADCOMPLABEL ;

#ifdef DX_LOAD_FBX_MODEL

	// �e�a�w�t�@�C���Ƃ��ēǂݍ���ł݂�
	NewBaseHandle = MV1LoadModelToFBX( &LoadParam, ASyncThread ) ;
	if( NewBaseHandle >= 0 ) goto LOADCOMPLABEL ;

#endif

	// �����ɂ�����ǂݍ��߂Ȃ������Ƃ�������
	goto ERRORLABEL ;

LOADCOMPLABEL :
	// ���_�o�b�t�@�̃Z�b�g�A�b�v���s��
	MV1_SetupVertexBufferBase_PF( NewBaseHandle, 1, ASyncThread ) ;

	// �t�@�C������ǂݍ��񂾃f�[�^���������
	if( DataBuffer )
	{
		DXFREE( DataBuffer ) ;
		DataBuffer = NULL ;
	}

	// �\�z
	if( MV1MakeModel( MHandle, NewBaseHandle, ASyncThread ) < 0 )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\xe2\x30\xc7\x30\xeb\x30\xd5\x30\xa1\x30\xa4\x30\xeb\x30\xa4\x30\xe1\x30\xfc\x30\xb8\x30\x4b\x30\x89\x30\x6e\x30\xe2\x30\xc7\x30\xeb\x30\xfa\x57\x2c\x67\xc7\x30\xfc\x30\xbf\x30\x4b\x30\x89\x30\xe2\x30\xc7\x30\xeb\x30\xc7\x30\xfc\x30\xbf\x30\x78\x30\x6e\x30\x09\x59\xdb\x63\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x00"/*@ L"���f���t�@�C���C���[�W����̃��f����{�f�[�^���烂�f���f�[�^�ւ̕ϊ��Ɏ��s���܂���" @*/ )) ;
		goto ERRORLABEL ;
	}

	// ����I��
	return 0 ;


ERRORLABEL :
	// ���f����{�f�[�^�̉��
	if( NewBaseHandle != -1 )
	{
		MV1SubModelBase( NewBaseHandle ) ;
	}
/*
	// ���f���f�[�^�̉��
	if( NewHandle != -1 )
	{
		MV1SubModel( NewHandle ) ;
	}
*/
	// �t�@�C������ǂݍ��񂾃f�[�^���������
	if( DataBuffer )
	{
		DXFREE( DataBuffer ) ;
		DataBuffer = NULL ;
	}

	// �l��Ԃ�
	return -1 ;
}

#ifndef DX_NON_ASYNCLOAD
// MV1LoadModelFromMem �̔񓯊��ǂݍ��݃X���b�h����Ă΂��֐�
static void MV1LoadModelFromMem_ASync( ASYNCLOADDATA_COMMON *AParam )
{
	MV1LOADMODEL_GPARAM *GParam ;
	int MHandle ;
	const void *FileImage ;
	int FileSize ;
	int (* FileReadFunc )( const TCHAR *FilePath, void **FileImageAddr, int *FileSize, void *FileReadFuncData ) ;
	int (* FileReleaseFunc )( void *MemoryAddr, void *FileReadFuncData ) ;
	void *FileReadFuncData ;
	int Addr ;
	int Result ;

	Addr				= 0 ;
	GParam				= ( MV1LOADMODEL_GPARAM * )GetASyncLoadParamStruct( AParam->Data, &Addr ) ;
	MHandle				= GetASyncLoadParamInt( AParam->Data, &Addr ) ;
	FileImage			= GetASyncLoadParamVoidP( AParam->Data, &Addr ) ;
	FileSize			= GetASyncLoadParamInt( AParam->Data, &Addr ) ;
	FileReadFunc		= ( int (*)( const TCHAR *FilePath, void **FileImageAddr, int *FileSize, void *FileReadFuncData ) )GetASyncLoadParamVoidP( AParam->Data, &Addr ) ;
	FileReleaseFunc		= ( int (*)( void *MemoryAddr, void *FileReadFuncData ) )GetASyncLoadParamVoidP( AParam->Data, &Addr ) ;
	FileReadFuncData	= GetASyncLoadParamVoidP( AParam->Data, &Addr ) ;

	Result = MV1LoadModelFromMem_Static( GParam, MHandle, FileImage, FileSize, FileReadFunc, FileReleaseFunc, FileReadFuncData, TRUE ) ;
	DecASyncLoadCount( MHandle ) ;
	if( Result < 0 )
	{
		MV1SubModel( MHandle ) ;
	}
}
#endif // DX_NON_ASYNCLOAD

// MV1LoadModelFromMem �̃O���[�o���ϐ��ɃA�N�Z�X���Ȃ��o�[�W����
extern int MV1LoadModelFromMem_UseGParam( 
	MV1LOADMODEL_GPARAM *GParam,
	const void *FileImage,
	int FileSize,
	int (* FileReadFunc )( const TCHAR *FilePath, void **FileImageAddr, int *FileSize, void *FileReadFuncData ),
	int (* FileReleaseFunc )( void *MemoryAddr, void *FileReadFuncData ),
	void *FileReadFuncData,
	int ASyncLoadFlag
)
{
	int MHandle ;

	CheckActiveState() ;

	MHandle = MV1AddModel( FALSE ) ;
	if( MHandle < 0 )
	{
		return -1 ;
	}

#ifndef DX_NON_ASYNCLOAD
	if( ASyncLoadFlag )
	{
		ASYNCLOADDATA_COMMON *AParam = NULL ;
		int Addr ;

		// �p�����[�^�ɕK�v�ȃ������̃T�C�Y���Z�o
		Addr = 0 ;
		AddASyncLoadParamStruct( NULL, &Addr, GParam, sizeof( *GParam ) ) ;
		AddASyncLoadParamInt( NULL, &Addr, MHandle ) ;
		AddASyncLoadParamConstVoidP( NULL, &Addr, FileImage ) ;
		AddASyncLoadParamInt( NULL, &Addr, FileSize ) ;
		AddASyncLoadParamConstVoidP( NULL, &Addr, ( void * )FileReadFunc ) ;
		AddASyncLoadParamConstVoidP( NULL, &Addr, ( void * )FileReleaseFunc ) ;
		AddASyncLoadParamConstVoidP( NULL, &Addr, ( void * )FileReadFuncData ) ;

		// �������̊m��
		AParam = AllocASyncLoadDataMemory( Addr ) ;
		if( AParam == NULL )
			goto ERR ;

		// �����ɕK�v�ȏ����Z�b�g
		AParam->ProcessFunction = MV1LoadModelFromMem_ASync ;
		Addr = 0 ;
		AddASyncLoadParamStruct( AParam->Data, &Addr, GParam, sizeof( *GParam ) ) ;
		AddASyncLoadParamInt( AParam->Data, &Addr, MHandle ) ;
		AddASyncLoadParamConstVoidP( AParam->Data, &Addr, FileImage ) ;
		AddASyncLoadParamInt( AParam->Data, &Addr, FileSize ) ;
		AddASyncLoadParamConstVoidP( AParam->Data, &Addr, ( void * )FileReadFunc ) ;
		AddASyncLoadParamConstVoidP( AParam->Data, &Addr, ( void * )FileReleaseFunc ) ;
		AddASyncLoadParamConstVoidP( AParam->Data, &Addr, ( void * )FileReadFuncData ) ;

		// �f�[�^��ǉ�
		if( AddASyncLoadData( AParam ) < 0 )
		{
			DXFREE( AParam ) ;
			AParam = NULL ;
			goto ERR ;
		}

		// �񓯊��ǂݍ��݃J�E���g���C���N�������g
		IncASyncLoadCount( MHandle, AParam->Index ) ;
	}
	else
#endif // DX_NON_ASYNCLOAD
	{
		if( MV1LoadModelFromMem_Static( GParam, MHandle, FileImage, FileSize, FileReadFunc, FileReleaseFunc, FileReadFuncData, FALSE ) < 0 )
			goto ERR ;
	}

	// �n���h����Ԃ�
	return MHandle ;

ERR :
	MV1SubModel( MHandle ) ;
	MHandle = -1 ;

	return -1 ;
}

// �������Z�������Ȗ��O���ǂ������`�F�b�N����( �߂�l�@TRUE:����  FALSE:�L�� )( ShiftJIS�R�[�h�� )
extern int MV1LoadModelDisablePhysicsNameCheck_ShiftJIS( const char *Name, int DisablePhysicsFile )
{
	int i ;

	if( DisablePhysicsFile )
	{
		if( MV1Man.LoadModelToDisablePhysicsNameWordMode == DX_LOADMODEL_PHYSICS_DISABLENAMEWORD_DISABLEPHYSICSFILEONLY )
		{
			for( i = 0 ; i < MV1Man.LoadModelToDisablePhysicsNameWordNum ; i ++ )
			{
				if( CL_strstr( DX_CHARCODEFORMAT_SHIFTJIS, Name, MV1Man.LoadModelToDisablePhysicsNameWord_ShiftJIS[ i ] ) != NULL )
				{
					return TRUE ;
				}
			}
		}
		else
		{
			return TRUE ;
		}
	}
	else
	{
		if( MV1Man.LoadModelToDisablePhysicsNameWordMode == DX_LOADMODEL_PHYSICS_DISABLENAMEWORD_ALWAYS )
		{
			for( i = 0 ; i < MV1Man.LoadModelToDisablePhysicsNameWordNum ; i ++ )
			{
				if( CL_strstr( DX_CHARCODEFORMAT_SHIFTJIS, Name, MV1Man.LoadModelToDisablePhysicsNameWord_ShiftJIS[ i ] ) != NULL )
				{
					return TRUE ;
				}
			}
		}
	}

	return FALSE ;
}

// �������Z�������Ȗ��O���ǂ������`�F�b�N����( �߂�l�@TRUE:����  FALSE:�L�� )( wchar_t�� )
extern int MV1LoadModelDisablePhysicsNameCheck_WCHAR_T( const WORD *Name, int DisablePhysicsFile )
{
	int i ;

	if( DisablePhysicsFile )
	{
		if( MV1Man.LoadModelToDisablePhysicsNameWordMode == DX_LOADMODEL_PHYSICS_DISABLENAMEWORD_DISABLEPHYSICSFILEONLY )
		{
			for( i = 0 ; i < MV1Man.LoadModelToDisablePhysicsNameWordNum ; i ++ )
			{
				if( _WCSSTR( ( wchar_t * )Name, MV1Man.LoadModelToDisablePhysicsNameWord[ i ] ) != NULL )
				{
					return TRUE ;
				}
			}
		}
		else
		{
			return TRUE ;
		}
	}
	else
	{
		if( MV1Man.LoadModelToDisablePhysicsNameWordMode == DX_LOADMODEL_PHYSICS_DISABLENAMEWORD_ALWAYS )
		{
			for( i = 0 ; i < MV1Man.LoadModelToDisablePhysicsNameWordNum ; i ++ )
			{
				if( _WCSSTR( ( wchar_t * )Name, MV1Man.LoadModelToDisablePhysicsNameWord[ i ] ) != NULL )
				{
					return TRUE ;
				}
			}
		}
	}

	return FALSE ;
}

// ��������̃��f���t�@�C���C���[�W�ƓƎ��̓ǂݍ��݃��[�`�����g�p���ă��f����ǂݍ���
extern int NS_MV1LoadModelFromMem(
	const void *FileImage,
	int FileSize,
	int (* FileReadFunc )( const TCHAR *FilePath, void **FileImageAddr, int *FileSize, void *FileReadFuncData ),
	int (* FileReleaseFunc )( void *MemoryAddr, void *FileReadFuncData ),
	void *FileReadFuncData
)
{
	MV1LOADMODEL_GPARAM GParam ;

	InitMV1LoadModelGParam( &GParam ) ;

	return MV1LoadModelFromMem_UseGParam( &GParam, FileImage, FileSize, FileReadFunc, FileReleaseFunc, FileReadFuncData, GetASyncLoadFlag() ) ;
}

// MV1LoadModel �̎������֐�
static int MV1LoadModel_Static(
	MV1LOADMODEL_GPARAM *GParam,
	int MHandle,
	const wchar_t *FileName,
	const wchar_t *Directory,
	const wchar_t *Name,
	int ASyncThread
)
{
	void *DataBuffer = NULL ;
	size_t DataSize ;
	int NewBaseHandle = -1 ;
	DWORD_PTR FileHandle = 0 ;
	MV1_MODEL_LOAD_PARAM LoadParam ;
	wchar_t FullPath[ 1024 ] ;

	ConvertFullPathW_( FileName, FullPath, sizeof( FullPath ), NULL ) ;

	// ����������Ă��Ȃ�������G���[
	if( MV1Man.Initialize == false ) return -1 ;

	// �R�c���f���p�V�F�[�_�[�̏������`�F�b�N
	if( Graphics_Hardware_Shader_ModelCode_Init_PF() < 0 )
	{
		return -1 ;
	}

	// �t�@�C�����ۂ��Ɠǂݍ���
	FileHandle = DX_FOPEN( FileName ) ;
	if( FileHandle == 0 )
	{
		DXST_ERRORLOGFMT_ADDW(( L"Model File Open Error : %s", FullPath )) ;
		goto ERRORLABEL ;
	}

	// �t�@�C���̃T�C�Y���擾����
	DX_FSEEK( FileHandle, 0, SEEK_END ) ;
	DataSize = ( size_t )DX_FTELL( FileHandle ) ;
	DX_FSEEK( FileHandle, 0, SEEK_SET ) ;

	// �t�@�C�����i�[���郁�������m��
	DataBuffer = DXALLOC( DataSize + 1 ) ;
	if( DataBuffer == NULL )
	{
		DXST_ERRORLOGFMT_ADDW(( L"Model File Memory Alloc Error : %s", FullPath )) ;
		goto ERRORLABEL ;
	}

	// �f�[�^��ǂݍ���
	DX_FREAD( DataBuffer, DataSize, 1, FileHandle ) ;

	// �e�L�X�g�`���̏ꍇ�p�ɏI�[�������Z�b�g
	( ( BYTE * )DataBuffer )[ DataSize ] = '\0' ;

	// �t�@�C�������
	DX_FCLOSE( FileHandle ) ;
	FileHandle = 0 ;

	// �֐��̈���������
	LoadParam.GParam			= *GParam ;
	LoadParam.DataBuffer		= DataBuffer ;
	LoadParam.DataSize			= ( int )DataSize ;
	LoadParam.FilePath			= FileName ;
	LoadParam.Name				= Name ;
	LoadParam.CurrentDir		= Directory ;
	LoadParam.FileReadFunc		= NULL ;

	// �l�u�P�t�@�C���Ƃ��ēǂݍ���ł݂�
	NewBaseHandle = MV1LoadModelToMV1( &LoadParam, ASyncThread ) ;
	if( NewBaseHandle >= 0 ) goto LOADCOMPLABEL ;

	// �l�p�n�t�@�C���Ƃ��ēǂݍ���ł݂�
	NewBaseHandle = MV1LoadModelToMQO( &LoadParam, ASyncThread ) ;
	if( NewBaseHandle >= 0 ) goto LOADCOMPLABEL ;

	// �w�t�@�C���Ƃ��ēǂݍ���ł݂�
	NewBaseHandle = MV1LoadModelToX( &LoadParam, ASyncThread ) ;
	if( NewBaseHandle >= 0 ) goto LOADCOMPLABEL ;

	// �o�l�w�t�@�C���Ƃ��ēǂݍ���ł݂�
	NewBaseHandle = MV1LoadModelToPMX( &LoadParam, ASyncThread ) ;
	if( NewBaseHandle >= 0 ) goto LOADCOMPLABEL ;

	// �o�l�c�t�@�C���Ƃ��ēǂݍ���ł݂�
	NewBaseHandle = MV1LoadModelToPMD( &LoadParam, ASyncThread ) ;
	if( NewBaseHandle >= 0 ) goto LOADCOMPLABEL ;

	// �u�l�c�t�@�C���Ƃ��ēǂݍ���ł݂�
	NewBaseHandle = MV1LoadModelToVMD( &LoadParam, ASyncThread ) ;
	if( NewBaseHandle >= 0 ) goto LOADCOMPLABEL ;

#ifdef DX_LOAD_FBX_MODEL

	// �e�a�w�t�@�C���Ƃ��ēǂݍ���ł݂�
	NewBaseHandle = MV1LoadModelToFBX( &LoadParam, ASyncThread ) ;
	if( NewBaseHandle >= 0 ) goto LOADCOMPLABEL ;

#endif

	// �����ɂ�����ǂݍ��߂Ȃ������Ƃ�������
	goto ERRORLABEL ;

LOADCOMPLABEL :
	// ���_�o�b�t�@�̃Z�b�g�A�b�v���s��
	MV1_SetupVertexBufferBase_PF( NewBaseHandle, 1, ASyncThread ) ;

	// �t�@�C������ǂݍ��񂾃f�[�^���������
	if( DataBuffer )
	{
		DXFREE( DataBuffer ) ;
		DataBuffer = NULL ;
	}

	// �t�@�C�������
	if( FileHandle )
	{
		DX_FCLOSE( FileHandle ) ;
		FileHandle = 0 ;
	}

	// �\�z
	if( MV1MakeModel( MHandle, NewBaseHandle, ASyncThread ) < 0 )
	{
		DXST_ERRORLOGFMT_ADDW(( L"MV1MakeModel Error : %s", FullPath )) ;
		goto ERRORLABEL ;
	}

	// ����I��
	return 0 ;


ERRORLABEL :
	// ���f����{�f�[�^�̉��
	if( NewBaseHandle != -1 )
	{
		MV1SubModelBase( NewBaseHandle ) ;
	}
/*
	// ���f���f�[�^�̉��
	if( NewHandle != -1 )
	{
		MV1SubModel( NewHandle ) ;
	}
*/
	// �t�@�C������ǂݍ��񂾃f�[�^���������
	if( DataBuffer )
	{
		DXFREE( DataBuffer ) ;
		DataBuffer = NULL ;
	}

	// �t�@�C�������
	if( FileHandle )
	{
		DX_FCLOSE( FileHandle ) ;
		FileHandle = 0 ;
	}

	// �l��Ԃ�
	return -1 ;
}

#ifndef DX_NON_ASYNCLOAD
// MV1LoadModel �̔񓯊��ǂݍ��݃X���b�h����Ă΂��֐�
static void MV1LoadModel_ASync( ASYNCLOADDATA_COMMON *AParam )
{
	MV1LOADMODEL_GPARAM *GParam ;
	int MHandle ;
	const wchar_t *FileName ;
	const wchar_t *Directory ;
	const wchar_t *Name ;
	int Addr ;
	int Result ;

	Addr = 0 ;
	GParam = ( MV1LOADMODEL_GPARAM * )GetASyncLoadParamStruct( AParam->Data, &Addr ) ;
	MHandle = GetASyncLoadParamInt( AParam->Data, &Addr ) ;
	FileName = GetASyncLoadParamString( AParam->Data, &Addr ) ;
	Directory = GetASyncLoadParamString( AParam->Data, &Addr ) ;
	Name = GetASyncLoadParamString( AParam->Data, &Addr ) ;

	Result = MV1LoadModel_Static( GParam, MHandle, FileName, Directory, Name, TRUE ) ;
	DecASyncLoadCount( MHandle ) ;
	if( Result < 0 )
	{
		MV1SubModel( MHandle ) ;
	}
}
#endif // DX_NON_ASYNCLOAD

// MV1LoadModel �̃O���[�o���ϐ��ɃA�N�Z�X���Ȃ��o�[�W����
extern int MV1LoadModel_UseGParam( MV1LOADMODEL_GPARAM *GParam, const wchar_t *FileName, int ASyncLoadFlag )
{
	wchar_t Directory[ 1024 ], Name[ FILEPATH_MAX ] ;
	int MHandle ;

	// ���f���t�@�C���̂���f�B���N�g���p�X�ƃt�@�C�������擾����
	_MV1CreateFileNameAndCurrentDirectory( FileName, Name, sizeof( Name ), Directory, sizeof( Directory ) ) ;

	CheckActiveState() ;

	MHandle = MV1AddModel( FALSE ) ;
	if( MHandle < 0 )
	{
		return -1 ;
	}

#ifndef DX_NON_ASYNCLOAD
	if( ASyncLoadFlag )
	{
		ASYNCLOADDATA_COMMON *AParam = NULL ;
		int Addr ;

		// �p�����[�^�ɕK�v�ȃ������̃T�C�Y���Z�o
		Addr = 0 ;
		AddASyncLoadParamStruct( NULL, &Addr, GParam, sizeof( *GParam ) ) ;
		AddASyncLoadParamInt( NULL, &Addr, MHandle ) ;
		AddASyncLoadParamString( NULL, &Addr, FileName ) ;
		AddASyncLoadParamString( NULL, &Addr, Directory ) ;
		AddASyncLoadParamString( NULL, &Addr, Name ) ;

		// �������̊m��
		AParam = AllocASyncLoadDataMemory( Addr ) ;
		if( AParam == NULL )
			goto ERR ;

		// �����ɕK�v�ȏ����Z�b�g
		AParam->ProcessFunction = MV1LoadModel_ASync ;
		Addr = 0 ;
		AddASyncLoadParamStruct( AParam->Data, &Addr, GParam, sizeof( *GParam ) ) ;
		AddASyncLoadParamInt( AParam->Data, &Addr, MHandle ) ;
		AddASyncLoadParamString( AParam->Data, &Addr, FileName ) ;
		AddASyncLoadParamString( AParam->Data, &Addr, Directory ) ;
		AddASyncLoadParamString( AParam->Data, &Addr, Name ) ;

		// �f�[�^��ǉ�
		if( AddASyncLoadData( AParam ) < 0 )
		{
			DXFREE( AParam ) ;
			AParam = NULL ;
			goto ERR ;
		}

		// �񓯊��ǂݍ��݃J�E���g���C���N�������g
		IncASyncLoadCount( MHandle, AParam->Index ) ;
	}
	else
#endif // DX_NON_ASYNCLOAD
	{
		if( MV1LoadModel_Static( GParam, MHandle, FileName, Directory, Name, FALSE ) < 0 )
			goto ERR ;
	}

	// �n���h����Ԃ�
	return MHandle ;

ERR :
	MV1SubModel( MHandle ) ;
	MHandle = -1 ;

	return -1 ;
}

// MV1LOADMODEL_GPARAM �̃f�[�^���Z�b�g����
extern void InitMV1LoadModelGParam( MV1LOADMODEL_GPARAM *GParam )
{
	Graphics_Image_InitLoadGraphGParam( &GParam->LoadGraphGParam ) ;

	GParam->LoadModelToReMakeNormal							= MV1Man.LoadModelToReMakeNormal ;
	GParam->LoadModelToReMakeNormalSmoothingAngle			= MV1Man.LoadModelToReMakeNormalSmoothingAngle ;
	GParam->LoadModelToIgnoreScaling						= MV1Man.LoadModelToIgnoreScaling ;
	GParam->LoadModelToPositionOptimize						= MV1Man.LoadModelToPositionOptimize ;
	GParam->LoadModelToNotEqNormalSideAddZeroAreaPolygon	= MV1Man.LoadModelToNotEqNormalSideAddZeroAreaPolygon ;
	GParam->LoadModelToUsePhysicsMode						= MV1Man.LoadModelToUsePhysicsMode ;
	GParam->LoadModelToWorldGravityInitialize				= MV1Man.LoadModelToWorldGravityInitialize ;
	GParam->LoadModelToWorldGravity							= MV1Man.LoadModelToWorldGravity ;
	GParam->LoadModelToPhysicsCalcPrecision					= MV1Man.LoadModelToPhysicsCalcPrecision ;
	GParam->LoadModelToUsePackDraw							= MV1Man.LoadModelToUsePackDraw ;
	GParam->LoadModelToTriangleListUseMaxBoneNum			= MV1Man.LoadModelToTriangleListUseMaxBoneNum == 0 ? MV1_TRIANGLE_LIST_USE_BONE_MAX_NUM : MV1Man.LoadModelToTriangleListUseMaxBoneNum ;
	_MEMCPY( GParam->LoadCalcPhysicsWorldGravity, MV1Man.LoadCalcPhysicsWorldGravity, sizeof( MV1Man.LoadCalcPhysicsWorldGravity ) ) ;

	GParam->AnimFilePathValid = MV1Man.AnimFilePathValid ;
	if( GParam->AnimFilePathValid )
	{
		_WCSCPY_S( GParam->AnimFileName,    sizeof( GParam->AnimFileName    ), MV1Man.AnimFileName ) ;
		_WCSCPY_S( GParam->AnimFileDirPath, sizeof( GParam->AnimFileDirPath ), MV1Man.AnimFileDirPath ) ;
	}
}

// ���f���̓ǂݍ���( -1:�G���[  0�ȏ�:���f���n���h�� )
extern int NS_MV1LoadModel( const TCHAR *FileName )
{
#ifdef UNICODE
	return MV1LoadModel_WCHAR_T(
		FileName
	) ;
#else
	int Result ;

	TCHAR_TO_WCHAR_T_STRING_ONE_BEGIN( FileName, return -1 )

	Result = MV1LoadModel_WCHAR_T(
		UseFileNameBuffer
	) ;

	TCHAR_TO_WCHAR_T_STRING_END( FileName )

	return Result ;
#endif
}

// ���f���̓ǂݍ���( -1:�G���[  0�ȏ�:���f���n���h�� )
extern int MV1LoadModel_WCHAR_T( const wchar_t *FileName )
{
	MV1LOADMODEL_GPARAM GParam ;

	InitMV1LoadModelGParam( &GParam ) ;

	return MV1LoadModel_UseGParam( &GParam, FileName, GetASyncLoadFlag() ) ;
}


// ���f�����폜����
extern int NS_MV1DeleteModel( int MHandle )
{
	return MV1SubModel( MHandle ) ;
}

// ���ׂẴ��f�����폜����
extern int NS_MV1InitModel( void )
{
	return AllHandleSub( DX_HANDLETYPE_MODEL ) ;
}

// MV1LoadModelToMV1 �ł̕�����擾����
__inline void MV1LoadModelToMV1_GetString(
	MV1MODEL_FILEHEADER_F1 *	FHeader,
	MV1_MODEL_BASE *			MBase,
	BYTE *						FileStringBuffer,
	DWORD						FileStringAddr,
#ifndef UNICODE
	char **						StringA,
#endif
	wchar_t **					StringW
)
{
	if( FHeader->IsStringUTF8 )
	{
		*StringW = ( wchar_t * )( ( BYTE * )MBase->StringBufferW + MBase->StringSizeW ) ;
		MBase->StringSizeW += ConvString( ( const char * )( FileStringBuffer + FileStringAddr ), DX_CHARCODEFORMAT_UTF8, ( char * )*StringW, BUFFERBYTES_CANCEL, WCHAR_T_CHARCODEFORMAT ) ;

#ifndef UNICODE
		*StringA = ( char    * )( ( BYTE * )MBase->StringBufferA + MBase->StringSizeA ) ;
		MBase->StringSizeA += ConvString( ( const char * )*StringW, WCHAR_T_CHARCODEFORMAT, *StringA, BUFFERBYTES_CANCEL, CHAR_CHARCODEFORMAT ) ;
#endif
	}
	else
	{
		*StringW   = ( wchar_t * )( ( BYTE * )MBase->StringBufferW + MBase->StringSizeW ) ;
		MBase->StringSizeW += ConvString( ( const char * )( FileStringBuffer + FileStringAddr ), DX_CHARCODEFORMAT_SHIFTJIS, ( char * )*StringW, BUFFERBYTES_CANCEL, WCHAR_T_CHARCODEFORMAT ) ;

#ifndef UNICODE
		*StringA   = MBase->StringBufferA + FileStringAddr ;
#endif
	}
}

// �l�u�P�t�@�C����ǂݍ���( -1:�G���[  0�ȏ�:���f���n���h�� )
extern int MV1LoadModelToMV1( const MV1_MODEL_LOAD_PARAM *LoadParam, int ASyncThread )
{
	MV1MODEL_FILEHEADER_F1 *FHeader ;
	MV1_MODEL_BASE MTBase, *MBase = NULL ;
	MV1_FRAME_BASE			*Frame ;
	MV1_FRAME_F1			*F1Frame ;
	MV1_MATERIAL_BASE		*Material ;
	MV1_MATERIAL_F1			*F1Material ;
	MV1_MATERIAL_LAYER		*MaterialLayer ;
	MV1_MATERIAL_LAYER_F1	*F1MaterialLayer ;
	MV1_MATERIAL_TOON_F1	*F1MaterialToon ;
	MV1_LIGHT				*Light ;
	MV1_LIGHT_F1			*F1Light ;
	MV1_TEXTURE_BASE		*Texture ;
	MV1_TEXTURE_F1			*F1Texture ;
	MV1_MESH_BASE			*Mesh ;
	MV1_MESH_F1				*F1Mesh ;
	MV1_MESH_FACE			*Face ;
	MV1_SKIN_BONE			*SkinBone ;
	MV1_SKIN_BONE_F1		*F1SkinBone ;
	MV1_TRIANGLE_LIST_BASE	*TriangleList ;
	MV1_TRIANGLE_LIST_F1	*F1TriangleList ;
	MV1_ANIM_KEYSET_BASE	*AnimKeySet ;
	MV1_ANIM_KEYSET_F1		*F1AnimKeySet ;
	MV1_ANIM_BASE			*Anim ;
	MV1_ANIM_F1				*F1Anim ;
	MV1_ANIMSET_BASE		*AnimSet ;
	MV1_ANIMSET_F1			*F1AnimSet ;
	MV1_FILEHEAD_SHAPE_F1	*F1FileHeadShape ;
	MV1_FRAME_SHAPE_F1		*F1FrameShape ;
	MV1_SHAPE_BASE			*Shape ;
	MV1_SHAPE_F1			*F1Shape ;
	MV1_SHAPE_MESH_BASE		*ShapeMesh ;
	MV1_SHAPE_MESH_F1		*F1ShapeMesh ;
	MV1_SHAPE_VERTEX_BASE	*ShapeVertex ;
	MV1_SHAPE_VERTEX_F1		*F1ShapeVertex ;
	MV1_FILEHEAD_PHYSICS_F1		*F1FileHeadPhysics ;
	MV1_PHYSICS_RIGIDBODY_BASE	*PhysicsRigidBody ;
	MV1_PHYSICS_RIGIDBODY_F1	*F1PhysicsRigidBody ;
	MV1_PHYSICS_JOINT_BASE		*PhysicsJoint ;
	MV1_PHYSICS_JOINT_F1		*F1PhysicsJoint ;
	BYTE *FileStringBuffer ;
	int i, j, k, NewHandle = 0, dirlen ;
	DWORD_PTR AllocSize ;
	int MeshVertexNum ;
	int AutoNormalCreateFlag ;

	wchar_t DirectoryPath[ FILEPATH_MAX + 2 ] ;
	BYTE *Src ;

	AutoNormalCreateFlag = FALSE ;

	F1FileHeadShape = NULL ;
	F1FileHeadPhysics = NULL ;
	FHeader = ( MV1MODEL_FILEHEADER_F1 * )LoadParam->DataBuffer ;

	// �f�B���N�g���p�X���擾
	dirlen = 0 ;
	if( LoadParam->CurrentDir != NULL )
	{
		_WCSCPY_S( DirectoryPath, sizeof( DirectoryPath ), LoadParam->CurrentDir ) ;
		dirlen = ( int )_WCSLEN( DirectoryPath ) ;
		if( DirectoryPath[ dirlen - 1 ] != L'\\' && DirectoryPath[ dirlen - 1 ] != L'/' )
		{
			_WCSCAT_S( DirectoryPath, sizeof( DirectoryPath ), L"/" ) ;
			dirlen = ( int )_WCSLEN( DirectoryPath ) ;
		}
	}

	// �h�c�`�F�b�N
	if( ( ( BYTE * )LoadParam->DataBuffer )[ 0 ] != 'M' || ( ( BYTE * )LoadParam->DataBuffer )[ 1 ] != 'V' ||
		( ( BYTE * )LoadParam->DataBuffer )[ 2 ] != '1' || ( ( BYTE * )LoadParam->DataBuffer )[ 3 ] != '1' )
		return -1 ;

	// ���k�f�[�^���𓀂��郁�����̈�̊m��
	FHeader = ( MV1MODEL_FILEHEADER_F1 * )DXALLOC( ( size_t )( DXA_Decode( ( BYTE * )LoadParam->DataBuffer + 4, NULL ) + 4 ) ) ;
	if( FHeader == NULL )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\x4d\x00\x56\x00\x31\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x4d\x00\x6f\x00\x64\x00\x65\x00\x6c\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\x27\x57\x2e\x7e\xc7\x30\xfc\x30\xbf\x30\x92\x30\xe3\x89\xcd\x51\x59\x30\x8b\x30\x5f\x30\x81\x30\x6e\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x00"/*@ L"MV1 LoadModel Error : ���k�f�[�^���𓀂��邽�߂̃������̈�̊m�ۂɎ��s���܂���" @*/ )) ;
		return -1 ;
	}
	DXA_Decode( ( BYTE * )LoadParam->DataBuffer + 4, ( BYTE * )FHeader + 4 ) ;

	// �K�v�ȃ������T�C�Y�̎Z�o
	{
		_MEMSET( &MTBase, 0, sizeof( MTBase ) ) ;
		AllocSize = 0 ;

		MTBase.Name = ( wchar_t * )AllocSize ;
		AllocSize += ( ( _WCSLEN( LoadParam->Name ) + 1 ) * sizeof( wchar_t ) + 3 ) / 4 * 4 ;

		MTBase.FilePath = ( wchar_t * )AllocSize ;
		AllocSize += ( ( _WCSLEN( LoadParam->FilePath ) + 1 ) * sizeof( wchar_t ) + 3 ) / 4 * 4 ;

		MTBase.DirectoryPath = ( wchar_t * )AllocSize ;
		AllocSize += ( ( dirlen + 1 ) * sizeof( wchar_t ) + 3 ) / 4 * 4 ;

		MTBase.Frame = ( MV1_FRAME_BASE * )AllocSize ;
		AllocSize += FHeader->FrameNum * sizeof( MV1_FRAME_BASE ) ;

		MTBase.FrameUseSkinBone = ( MV1_SKIN_BONE ** )AllocSize ;
		AllocSize += FHeader->FrameUseSkinBoneNum * sizeof( MV1_SKIN_BONE * ) ;

		MTBase.Material = ( MV1_MATERIAL_BASE * )AllocSize ;
		AllocSize += FHeader->MaterialNum * sizeof( MV1_MATERIAL_BASE ) ;

		MTBase.Light = ( MV1_LIGHT * )AllocSize ;
		AllocSize += FHeader->LightNum * sizeof( MV1_LIGHT ) ;

		MTBase.Texture = ( MV1_TEXTURE_BASE * )AllocSize ;
		AllocSize += FHeader->TextureNum * sizeof( MV1_TEXTURE_BASE ) ;

		MTBase.Mesh = ( MV1_MESH_BASE * )AllocSize ;
		AllocSize += FHeader->MeshNum * sizeof( MV1_MESH_BASE ) ;

		MTBase.SkinBone = ( MV1_SKIN_BONE * )AllocSize ;
		AllocSize += FHeader->SkinBoneNum * sizeof( MV1_SKIN_BONE ) ;
		MTBase.SkinBoneUseFrame = ( MV1_SKIN_BONE_USE_FRAME * )AllocSize ;
		AllocSize += FHeader->SkinBoneUseFrameNum * sizeof( MV1_SKIN_BONE_USE_FRAME ) ;

		MTBase.TriangleList = ( MV1_TRIANGLE_LIST_BASE * )AllocSize ;
		AllocSize += FHeader->TriangleListNum * sizeof( MV1_TRIANGLE_LIST_BASE ) ;

		MTBase.TriangleListIndex = ( WORD * )AllocSize ;
		AllocSize += sizeof( WORD ) * FHeader->TriangleListIndexNum * 2 ;

#ifndef UNICODE
		MTBase.StringBufferA = ( char * )AllocSize ;
		AllocSize += FHeader->StringSize * sizeof( char ) ;
#endif

		MTBase.StringBufferW = ( wchar_t * )AllocSize ;
		AllocSize += FHeader->StringSize * sizeof( wchar_t ) ;

		MTBase.ChangeDrawMaterialTable = ( DWORD * )AllocSize ;
		AllocSize += FHeader->ChangeDrawMaterialTableSize ;

		MTBase.ChangeMatrixTable = ( DWORD * )AllocSize ;
		AllocSize += FHeader->ChangeMatrixTableSize ;

		MTBase.AnimKeyData = ( void * )AllocSize ;
		AllocSize += FHeader->OriginalAnimKeyDataSize ;

		MTBase.AnimKeySet = ( MV1_ANIM_KEYSET_BASE * )AllocSize ;
		AllocSize += FHeader->AnimKeySetNum * sizeof( MV1_ANIM_KEYSET_BASE ) ;

		MTBase.Anim = ( MV1_ANIM_BASE * )AllocSize ;
		AllocSize += FHeader->AnimNum * sizeof( MV1_ANIM_BASE ) ;

		MTBase.AnimSet = ( MV1_ANIMSET_BASE * )AllocSize ;
		AllocSize += FHeader->AnimSetNum * sizeof( MV1_ANIMSET_BASE ) ;

		MTBase.AnimTargetFrameTable = ( MV1_ANIM_BASE ** )AllocSize ;
		AllocSize += FHeader->FrameNum * FHeader->AnimSetNum * sizeof( MV1_ANIM_BASE * ) ;

		if( FHeader->Shape != 0 )
		{
			F1FileHeadShape = ( MV1_FILEHEAD_SHAPE_F1 * )( ( DWORD_PTR )FHeader + ( DWORD_PTR )FHeader->Shape ) ;

			MTBase.Shape = ( MV1_SHAPE_BASE * )AllocSize ;
			AllocSize += F1FileHeadShape->DataNum * sizeof( MV1_SHAPE_BASE ) ;

			MTBase.ShapeMesh = ( MV1_SHAPE_MESH_BASE * )AllocSize ;
			AllocSize += F1FileHeadShape->MeshNum * sizeof( MV1_SHAPE_MESH_BASE ) ;
		}

		if( FHeader->Physics != 0 && LoadParam->GParam.LoadModelToUsePhysicsMode != DX_LOADMODEL_PHYSICS_DISABLE )
		{
			F1FileHeadPhysics = ( MV1_FILEHEAD_PHYSICS_F1 * )( ( DWORD_PTR )FHeader + ( DWORD_PTR )FHeader->Physics ) ;

			MTBase.PhysicsRigidBody = ( MV1_PHYSICS_RIGIDBODY_BASE * )AllocSize ;
			AllocSize += F1FileHeadPhysics->RigidBodyNum * sizeof( MV1_PHYSICS_RIGIDBODY_BASE ) ;

			MTBase.PhysicsJoint = ( MV1_PHYSICS_JOINT_BASE * )AllocSize ;
			AllocSize += F1FileHeadPhysics->JointNum * sizeof( MV1_PHYSICS_JOINT_BASE ) ;
		}
	}

	// ���f����f�[�^�n���h���̍쐬
	NewHandle = MV1AddModelBase( ASyncThread ) ;
	if( NewHandle < 0 )
	{
		goto ERRORLABEL ;
	}

	// �������̊m��
	MV1BMDLCHK_ASYNC( NewHandle, MBase ) ;
	MBase->DataBuffer = MDALLOCMEM( AllocSize ) ;
	if( MBase->DataBuffer == NULL )
	{
		goto ERRORLABEL ;
	}
	_MEMSET( MBase->DataBuffer, 0, AllocSize ) ;
	MBase->AllocMemorySize = AllocSize ;

	// �������A�h���X�̃Z�b�g
	MBase->Name                   = ( wchar_t * )                   ( ( BYTE * )MBase->DataBuffer + ( DWORD_PTR )MTBase.Name                 ) ;
	MBase->FilePath               = ( wchar_t * )                   ( ( BYTE * )MBase->DataBuffer + ( DWORD_PTR )MTBase.FilePath             ) ;
	MBase->DirectoryPath          = ( wchar_t * )                   ( ( BYTE * )MBase->DataBuffer + ( DWORD_PTR )MTBase.DirectoryPath        ) ;
	MBase->ChangeMatrixTable      = ( DWORD * )                     ( ( BYTE * )MBase->DataBuffer + ( DWORD_PTR )MTBase.ChangeMatrixTable    ) ;
	MBase->ChangeDrawMaterialTable = ( DWORD * )                    ( ( BYTE * )MBase->DataBuffer + ( DWORD_PTR )MTBase.ChangeDrawMaterialTable ) ;
	MBase->Frame                  = ( MV1_FRAME_BASE * )            ( ( BYTE * )MBase->DataBuffer + ( DWORD_PTR )MTBase.Frame                ) ;
	MBase->FrameUseSkinBone       = ( MV1_SKIN_BONE ** )            ( ( BYTE * )MBase->DataBuffer + ( DWORD_PTR )MTBase.FrameUseSkinBone     ) ;
	MBase->Material               = ( MV1_MATERIAL_BASE * )         ( ( BYTE * )MBase->DataBuffer + ( DWORD_PTR )MTBase.Material             ) ;
	MBase->Light                  = ( MV1_LIGHT * )                 ( ( BYTE * )MBase->DataBuffer + ( DWORD_PTR )MTBase.Light                ) ;
	MBase->Texture                = ( MV1_TEXTURE_BASE * )          ( ( BYTE * )MBase->DataBuffer + ( DWORD_PTR )MTBase.Texture              ) ;
	MBase->SkinBone               = ( MV1_SKIN_BONE * )             ( ( BYTE * )MBase->DataBuffer + ( DWORD_PTR )MTBase.SkinBone             ) ;
	MBase->SkinBoneUseFrame       = ( MV1_SKIN_BONE_USE_FRAME * )   ( ( BYTE * )MBase->DataBuffer + ( DWORD_PTR )MTBase.SkinBoneUseFrame     ) ;
	MBase->Mesh                   = ( MV1_MESH_BASE * )             ( ( BYTE * )MBase->DataBuffer + ( DWORD_PTR )MTBase.Mesh                 ) ;
	MBase->TriangleList           = ( MV1_TRIANGLE_LIST_BASE * )    ( ( BYTE * )MBase->DataBuffer + ( DWORD_PTR )MTBase.TriangleList         ) ;
	MBase->TriangleListIndex      = ( WORD * )                      ( ( BYTE * )MBase->DataBuffer + ( DWORD_PTR )MTBase.TriangleListIndex    ) ;
#ifndef UNICODE
	MBase->StringBufferA          = ( char * )                      ( ( BYTE * )MBase->DataBuffer + ( DWORD_PTR )MTBase.StringBufferA        ) ;
#endif
	MBase->StringBufferW          = ( wchar_t * )                   ( ( BYTE * )MBase->DataBuffer + ( DWORD_PTR )MTBase.StringBufferW        ) ;
	MBase->AnimSet                = ( MV1_ANIMSET_BASE * )          ( ( BYTE * )MBase->DataBuffer + ( DWORD_PTR )MTBase.AnimSet              ) ;
	MBase->Anim                   = ( MV1_ANIM_BASE * )             ( ( BYTE * )MBase->DataBuffer + ( DWORD_PTR )MTBase.Anim                 ) ;
	MBase->AnimKeySet             = ( MV1_ANIM_KEYSET_BASE * )      ( ( BYTE * )MBase->DataBuffer + ( DWORD_PTR )MTBase.AnimKeySet           ) ;
	MBase->AnimKeyData            = ( void * )                      ( ( BYTE * )MBase->DataBuffer + ( DWORD_PTR )MTBase.AnimKeyData          ) ;
	MBase->AnimTargetFrameTable   = ( MV1_ANIM_BASE ** )            ( ( BYTE * )MBase->DataBuffer + ( DWORD_PTR )MTBase.AnimTargetFrameTable ) ;
	if( FHeader->Shape )
	{
		MBase->Shape              = ( MV1_SHAPE_BASE * )            ( ( BYTE * )MBase->DataBuffer + ( DWORD_PTR )MTBase.Shape                ) ;
		MBase->ShapeMesh          = ( MV1_SHAPE_MESH_BASE * )       ( ( BYTE * )MBase->DataBuffer + ( DWORD_PTR )MTBase.ShapeMesh            ) ;
	}
	if( FHeader->Physics && LoadParam->GParam.LoadModelToUsePhysicsMode != DX_LOADMODEL_PHYSICS_DISABLE )
	{
		MBase->PhysicsRigidBody   = ( MV1_PHYSICS_RIGIDBODY_BASE * )( ( BYTE * )MBase->DataBuffer + ( DWORD_PTR )MTBase.PhysicsRigidBody     ) ;
		MBase->PhysicsJoint       = ( MV1_PHYSICS_JOINT_BASE *     )( ( BYTE * )MBase->DataBuffer + ( DWORD_PTR )MTBase.PhysicsJoint         ) ;
	}

	// ���f�����ƃt�@�C���p�X�ƃt�H���_�p�X��ۑ�
	_WCSCPY( MBase->Name,          LoadParam->Name ) ;
	_WCSCPY( MBase->FilePath,      LoadParam->FilePath ) ;
	_WCSCPY( MBase->DirectoryPath, LoadParam->CurrentDir == NULL ? L"" : DirectoryPath ) ;

	// ���������`��ɑΉ����邩�ǂ�����ۑ�
	MBase->UsePackDraw = LoadParam->GParam.LoadModelToUsePackDraw ;

	// ���W�n����ۑ�
	MBase->RightHandType = FHeader->RightHandType ;

	// ���蓖�Ă��Ă���}�e���A���̔ԍ����Ⴂ���b�V������`�悷�邩�ǂ����̏���ۑ�
	MBase->MaterialNumberOrderDraw = FHeader->MaterialNumberOrderDraw ;

	// �ύX�`�F�b�N�p�e�[�u���ɕK�v�ȃf�[�^�̃T�C�Y�̕ۑ��ƃR�s�[
	MBase->ChangeDrawMaterialTableSize  = FHeader->ChangeDrawMaterialTableSize ;
	MBase->ChangeMatrixTableSize        = FHeader->ChangeMatrixTableSize ;
	_MEMCPY( MBase->ChangeDrawMaterialTable, ( BYTE * )FHeader + ( DWORD_PTR )FHeader->ChangeDrawMaterialTable, ( size_t )FHeader->ChangeDrawMaterialTableSize ) ;
	_MEMCPY( MBase->ChangeMatrixTable,       ( BYTE * )FHeader + ( DWORD_PTR )FHeader->ChangeMatrixTable,       ( size_t )FHeader->ChangeMatrixTableSize ) ;

	// �|���S������ۑ�
	MBase->TriangleNum = FHeader->TriangleNum ;

	// ���_�f�[�^�̐���ۑ�
	MBase->TriangleListVertexNum = FHeader->TriangleListVertexNum ;

	// �t���[�����g�p���Ă���{�[���ւ̃|�C���^�̏����R�s�[
	MBase->FrameUseSkinBoneNum = FHeader->FrameUseSkinBoneNum ;
	for( i = 0 ; i < MBase->FrameUseSkinBoneNum ; i ++ )
	{
		MBase->FrameUseSkinBone[ i ] = MBase->SkinBone + ( ( ( DWORD * )( ( DWORD_PTR )FHeader + ( DWORD_PTR )FHeader->FrameUseSkinBone ) )[ i ] - FHeader->SkinBone ) / sizeof( MV1_SKIN_BONE_F1 ) ;
	}

	// �X�L�j���O�p�{�[�����g�p����t���[���̏����R�s�[
	_MEMCPY( MBase->SkinBoneUseFrame, ( BYTE * )FHeader + ( DWORD_PTR )FHeader->SkinBoneUseFrame, FHeader->SkinBoneUseFrameNum * sizeof( MV1_SKIN_BONE_USE_FRAME_F1 ) ) ;

	// ������f�[�^���R�s�[
	FileStringBuffer = ( BYTE * )FHeader + FHeader->StringBuffer ;
	if( FHeader->IsStringUTF8 )
	{
#ifndef UNICODE
		MBase->StringSizeA = 0 ;
#endif
		MBase->StringSizeW = 0 ;
	}
	else
	{
#ifndef UNICODE
		_MEMCPY( MBase->StringBufferA, FileStringBuffer, ( size_t )FHeader->StringSize ) ;
		MBase->StringSizeA = FHeader->StringSize ;
#endif
		MBase->StringSizeW = 0 ;
	}

	// �e�I�u�W�F�N�g�̐��̏���ۑ�
	MBase->FrameNum              = FHeader->FrameNum ;
	MBase->MaterialNum           = FHeader->MaterialNum ;
	MBase->LightNum              = FHeader->LightNum ;
	MBase->TextureNum            = FHeader->TextureNum ;
	MBase->MeshNum               = FHeader->MeshNum ;
	MBase->SkinBoneNum           = FHeader->SkinBoneNum ;
	MBase->SkinBoneUseFrameNum   = FHeader->SkinBoneUseFrameNum ;
	MBase->TriangleListNum       = FHeader->TriangleListNum ;
	MBase->AnimKeyDataSize       = 0 ;
	MBase->AnimKeySetNum         = FHeader->AnimKeySetNum ;
	MBase->AnimNum               = FHeader->AnimNum ;
	MBase->AnimSetNum            = FHeader->AnimSetNum ;
	if( FHeader->Shape )
	{
		MBase->ShapeNum          = F1FileHeadShape->DataNum ;
		MBase->ShapeMeshNum      = F1FileHeadShape->MeshNum ;

		MBase->ShapeNormalPositionNum = F1FileHeadShape->NormalPositionNum ;
		MBase->ShapeSkinPosition4BNum = F1FileHeadShape->SkinPosition4BNum ;
		MBase->ShapeSkinPosition8BNum = F1FileHeadShape->SkinPosition8BNum ;
		MBase->ShapeSkinPositionFREEBSize = F1FileHeadShape->SkinPositionFREEBSize ;

		MBase->ShapeTargetMeshVertexNum = F1FileHeadShape->TargetMeshVertexNum ;
	}
	if( FHeader->Physics && LoadParam->GParam.LoadModelToUsePhysicsMode != DX_LOADMODEL_PHYSICS_DISABLE )
	{
		MBase->PhysicsGravity    = F1FileHeadPhysics->WorldGravity ;

		MBase->PhysicsRigidBodyNum = F1FileHeadPhysics->RigidBodyNum ;
		MBase->PhysicsJointNum   = F1FileHeadPhysics->JointNum ;
	}

	// ���b�V�����_�f�[�^�T�C�Y�̌v�Z( FHeader->MeshVertexSize ���Â��o�[�W�����ƐV�����o�[�W�����ňقȂ��Ă��܂����̂� )
	{
		DWORD MeshVertexSize ;

		MeshVertexSize = 0 ;

		Mesh = MBase->Mesh ;
		F1Mesh = ( MV1_MESH_F1 * )( ( BYTE * )FHeader + ( DWORD_PTR )FHeader->Mesh ) ;
		for( i = 0 ; i < MBase->MeshNum ; i ++, Mesh ++, F1Mesh = ( MV1_MESH_F1 * )( ( DWORD_PTR )FHeader + ( DWORD_PTR )F1Mesh->DimNext ) )
		{
			Mesh->UVSetUnitNum = F1Mesh->UVSetUnitNum ;
			Mesh->UVUnitNum = F1Mesh->UVUnitNum ;
			Mesh->VertexNum = F1Mesh->VertexNum ;
			Mesh->VertUnitSize = ( int )( sizeof( MV1_MESH_VERTEX ) + Mesh->UVSetUnitNum * Mesh->UVUnitNum * sizeof( float ) - sizeof( float ) * 2 ) ;

			MeshVertexSize += Mesh->VertexNum * Mesh->VertUnitSize ;
		}

		FHeader->MeshVertexSize = ( int )MeshVertexSize ;
	}


	// ���_�f�[�^���i�[���邽�߂̃������̈�̊m��
	{
		MBase->VertexDataSize = 
			FHeader->MeshVertexIndexNum                * sizeof( DWORD )                 +
			FHeader->MeshFaceNum                       * sizeof( MV1_MESH_FACE )         +
			FHeader->MeshNormalNum                     * sizeof( MV1_MESH_NORMAL )       +
			FHeader->TriangleListNormalPositionNum     * sizeof( MV1_TLIST_NORMAL_POS )  +
			FHeader->TriangleListSkinPosition4BNum     * sizeof( MV1_TLIST_SKIN_POS_4B ) +
			FHeader->TriangleListSkinPosition8BNum     * sizeof( MV1_TLIST_SKIN_POS_8B ) +
			FHeader->TriangleListSkinPositionFREEBSize                                   +
			FHeader->MeshPositionSize                                                    +
			FHeader->MeshVertexSize                                                      + 16 ;
		F1Mesh = ( MV1_MESH_F1 * )( ( BYTE * )FHeader + ( DWORD_PTR )FHeader->Mesh ) ;
		if( ( F1Mesh->VertFlag & MV1_MESH_VERT_FLAG_NON_TOON_OUTLINE ) == 0 )
		{
			MeshVertexNum = 0 ;
			for( i = 0 ; i < FHeader->MeshNum ; i ++, F1Mesh = ( MV1_MESH_F1 * )( ( DWORD_PTR )FHeader + ( DWORD_PTR )F1Mesh->DimNext ) )
			{
				MeshVertexNum += F1Mesh->VertexNum ;
			}
			MBase->MeshVertexSize += MeshVertexNum * sizeof( DWORD ) ;
			MBase->VertexDataSize += MeshVertexNum * sizeof( DWORD ) ;
		}
		if( FHeader->Shape )
		{
			MBase->VertexDataSize += F1FileHeadShape->VertexNum * sizeof( MV1_SHAPE_VERTEX_BASE ) ;
		}
		MBase->VertexData = DXALLOC( MBase->VertexDataSize ) ;
		if( MBase->VertexData == NULL ) goto ERRORLABEL ;
		_MEMSET( MBase->VertexData, 0, MBase->VertexDataSize ) ;

		MBase->TriangleListNormalPosition    = ( MV1_TLIST_NORMAL_POS     * )( ( ( DWORD_PTR )MBase->VertexData + 15 ) / 16 * 16 ) ;
		MBase->TriangleListSkinPosition4B    = ( MV1_TLIST_SKIN_POS_4B    * )( MBase->TriangleListNormalPosition              + FHeader->TriangleListNormalPositionNum     ) ;
		MBase->TriangleListSkinPosition8B    = ( MV1_TLIST_SKIN_POS_8B    * )( MBase->TriangleListSkinPosition4B              + FHeader->TriangleListSkinPosition4BNum     ) ;
		MBase->TriangleListSkinPositionFREEB = ( MV1_TLIST_SKIN_POS_FREEB * )( MBase->TriangleListSkinPosition8B              + FHeader->TriangleListSkinPosition8BNum     ) ;
		MBase->MeshVertexIndex               = ( DWORD                    * )( ( BYTE * )MBase->TriangleListSkinPositionFREEB + FHeader->TriangleListSkinPositionFREEBSize ) ;
		MBase->MeshFace                      = ( MV1_MESH_FACE            * )( MBase->MeshVertexIndex                         + FHeader->MeshVertexIndexNum    ) ;
		MBase->MeshNormal                    = ( MV1_MESH_NORMAL          * )( MBase->MeshFace                                + FHeader->MeshFaceNum           ) ;
		MBase->MeshPosition                  = ( MV1_MESH_POSITION        * )( MBase->MeshNormal                              + FHeader->MeshNormalNum         ) ;
		MBase->MeshVertex                    = ( MV1_MESH_VERTEX          * )( ( BYTE * )MBase->MeshPosition                  + FHeader->MeshPositionSize      ) ;
		if( FHeader->Shape )
		{
			MBase->ShapeVertex               = ( MV1_SHAPE_VERTEX_BASE    * )( ( BYTE * )MBase->MeshVertex                    + FHeader->MeshVertexSize        ) ;
		}
		if( FHeader->TriangleListNormalPositionNum == 0     ) MBase->TriangleListNormalPosition    = NULL ;
		if( FHeader->TriangleListSkinPosition4BNum == 0     ) MBase->TriangleListSkinPosition4B    = NULL ;
		if( FHeader->TriangleListSkinPosition8BNum == 0     ) MBase->TriangleListSkinPosition8B    = NULL ;
		if( FHeader->TriangleListSkinPositionFREEBSize == 0 ) MBase->TriangleListSkinPositionFREEB = NULL ;
		if( FHeader->MeshVertexIndexNum == 0                ) MBase->MeshVertexIndex   = NULL ;
		if( FHeader->MeshFaceNum == 0                       ) MBase->MeshFace          = NULL ;
		if( FHeader->MeshNormalNum == 0                     ) MBase->MeshNormal        = NULL ;
		if( FHeader->MeshPositionSize == 0                  ) MBase->MeshPosition      = NULL ;
		if( FHeader->MeshVertexSize == 0                    ) MBase->MeshVertex        = NULL ;
		if( FHeader->Shape == 0 || F1FileHeadShape->VertexNum == 0 ) MBase->ShapeVertex = NULL ;
	}

	// �t���[���̏����Z�b�g
	MBase->TopFrameNum  = FHeader->TopFrameNum ;
	MBase->FirstTopFrame = FHeader->FirstTopFrame ? MBase->Frame + ( ( MV1_FRAME_F1 * )( ( BYTE * )FHeader + ( DWORD_PTR )FHeader->FirstTopFrame ) )->Index : NULL ;
	MBase->LastTopFrame  = FHeader->LastTopFrame  ? MBase->Frame + ( ( MV1_FRAME_F1 * )( ( BYTE * )FHeader + ( DWORD_PTR )FHeader->LastTopFrame ) )->Index  : NULL ;
	Frame = MBase->Frame ;
	F1Frame = ( MV1_FRAME_F1 * )( ( BYTE * )FHeader + ( DWORD_PTR )FHeader->Frame ) ;
	for( i = 0 ; i < MBase->FrameNum ; i ++, Frame ++, F1Frame = ( MV1_FRAME_F1 * )( ( DWORD_PTR )FHeader + ( DWORD_PTR )F1Frame->DimNext ) )
	{
		Frame->Container = MBase ;

		MV1LoadModelToMV1_GetString( FHeader, MBase, FileStringBuffer, F1Frame->Name,
#ifndef UNICODE
			&Frame->NameA,
#endif
			&Frame->NameW
		) ;

		Frame->AutoCreateNormal = F1Frame->AutoCreateNormal ;

		Frame->Index = F1Frame->Index ;

		Frame->ChangeDrawMaterialInfo.Target   = F1Frame->ChangeDrawMaterialInfo.Target ;
		Frame->ChangeDrawMaterialInfo.Fill     = MBase->ChangeDrawMaterialTable + ( ( DWORD * )( DWORD_PTR )F1Frame->ChangeDrawMaterialInfo.Fill - ( DWORD * )( DWORD_PTR )FHeader->ChangeDrawMaterialTable ) ;
		Frame->ChangeDrawMaterialInfo.Size     = F1Frame->ChangeDrawMaterialInfo.Size ;
		Frame->ChangeDrawMaterialInfo.CheckBit = F1Frame->ChangeDrawMaterialInfo.CheckBit ;

		Frame->ChangeMatrixInfo.Target         = F1Frame->ChangeMatrixInfo.Target ;
		Frame->ChangeMatrixInfo.Fill           = MBase->ChangeMatrixTable       + ( ( DWORD * )( DWORD_PTR )F1Frame->ChangeMatrixInfo.Fill - ( DWORD * )( DWORD_PTR )FHeader->ChangeMatrixTable ) ;
		Frame->ChangeMatrixInfo.Size           = F1Frame->ChangeMatrixInfo.Size ;
		Frame->ChangeMatrixInfo.CheckBit       = F1Frame->ChangeMatrixInfo.CheckBit ;

		Frame->TotalMeshNum  = F1Frame->TotalMeshNum ;
		Frame->TotalChildNum = F1Frame->TotalChildNum ;
		Frame->Parent     = F1Frame->Parent     ? MBase->Frame + ( ( MV1_FRAME_F1 * )( ( BYTE * )FHeader + ( DWORD_PTR )F1Frame->Parent     ) )->Index : NULL ;
		Frame->FirstChild = F1Frame->FirstChild ? MBase->Frame + ( ( MV1_FRAME_F1 * )( ( BYTE * )FHeader + ( DWORD_PTR )F1Frame->FirstChild ) )->Index : NULL ;
		Frame->LastChild  = F1Frame->LastChild  ? MBase->Frame + ( ( MV1_FRAME_F1 * )( ( BYTE * )FHeader + ( DWORD_PTR )F1Frame->LastChild  ) )->Index : NULL ;
		Frame->Prev       = F1Frame->Prev       ? MBase->Frame + ( ( MV1_FRAME_F1 * )( ( BYTE * )FHeader + ( DWORD_PTR )F1Frame->Prev       ) )->Index : NULL ;
		Frame->Next       = F1Frame->Next       ? MBase->Frame + ( ( MV1_FRAME_F1 * )( ( BYTE * )FHeader + ( DWORD_PTR )F1Frame->Next       ) )->Index : NULL ;

		Frame->Translate = F1Frame->Translate ;
		if( LoadParam->GParam.LoadModelToIgnoreScaling )
		{
			Frame->Scale = VGet( 1.0f, 1.0f, 1.0f ); ;
		}
		else
		{
			Frame->Scale = F1Frame->Scale ;
		}
		Frame->Rotate = F1Frame->Rotate ;
		Frame->RotateOrder = F1Frame->RotateOrder ;
		Frame->Quaternion = F1Frame->Quaternion ;

		Frame->Flag = F1Frame->Flag ;
		if( F1Frame->Flag & MV1_FRAMEFLAG_IGNOREPARENTTRANS ) Frame->IgnoreParentTransform = 1 ;

		if( F1Frame->Flag & MV1_FRAMEFLAG_PREROTATE )
		{
			Frame->PreRotate = F1Frame->PreRotate ;
		}

		if( F1Frame->Flag & MV1_FRAMEFLAG_POSTROTATE )
		{
			Frame->PreRotate = F1Frame->PostRotate ;
		}

		Frame->IsSkinMesh = F1Frame->IsSkinMesh ;
		Frame->TriangleNum = F1Frame->TriangleNum ;
		Frame->VertexNum = F1Frame->VertexNum ;
		Frame->MeshNum = F1Frame->MeshNum ;
		if( F1Frame->Mesh )
		{
			Frame->Mesh = MBase->Mesh + ( ( MV1_MESH_F1 * )( ( DWORD_PTR )F1Frame->Mesh + ( DWORD_PTR )FHeader ) )->Index ;
		}

		if( F1Frame->FrameShape )
		{
			F1FrameShape = ( MV1_FRAME_SHAPE_F1 * )( ( DWORD_PTR )FHeader + ( DWORD_PTR )F1Frame->FrameShape ) ;
			Frame->ShapeNum = F1FrameShape->ShapeNum ;
			Frame->Shape = MBase->Shape + ( ( MV1_SHAPE_F1 * )( ( DWORD_PTR )F1FrameShape->Shape + ( DWORD_PTR )FHeader ) )->Index ;
		}

		Frame->SkinBoneNum = F1Frame->SkinBoneNum ;
		if( F1Frame->SkinBone )
		{
			Frame->SkinBone = MBase->SkinBone + ( ( MV1_SKIN_BONE_F1 * )( ( DWORD_PTR )F1Frame->SkinBone + ( DWORD_PTR )FHeader ) )->Index ;
		}

		if( F1Frame->Light )
		{
			Frame->Light = MBase->Light + ( ( MV1_LIGHT_F1 * )( ( DWORD_PTR )F1Frame->Light + ( DWORD_PTR )FHeader ) )->Index ;
		}

		Frame->UseSkinBoneNum = F1Frame->UseSkinBoneNum ;
		if( F1Frame->UseSkinBoneNum )
		{
			Frame->UseSkinBone = MBase->FrameUseSkinBone + ( F1Frame->UseSkinBone - FHeader->FrameUseSkinBone ) / sizeof( DWORD /* MV1_SKIN_BONE_F1** */ ) ;
		}

		Frame->SmoothingAngle = F1Frame->SmoothingAngle ;

		Frame->MaxBoneBlendNum = F1Frame->MaxBoneBlendNum ;
		Frame->PositionNum     = F1Frame->PositionNum ;
		Frame->NormalNum       = F1Frame->NormalNum ;

		// ���W�f�[�^�Ɩ@���f�[�^�̃R�s�[
		if( F1Frame->PositionNum == 0 )
		{
			Frame->Position = NULL ;
			Frame->Normal = NULL ;
		}
		else
		{
			MV1_MESH_POSITION *Pos ;
			MV1_MESH_NORMAL *Nrm ;
			MV1_POSITION_16BIT_SUBINFO_F1 xs, ys, zs ;

			Frame->PosUnitSize = ( int )( sizeof( MV1_MESH_POSITION ) + ( Frame->MaxBoneBlendNum - 4 ) * sizeof( MV1_SKINBONE_BLEND ) ) ;

			Frame->Position = ( MV1_MESH_POSITION * )( ( BYTE * )MBase->MeshPosition + MBase->MeshPositionSize ) ;
			Frame->Normal   = MBase->MeshNormal + MBase->MeshNormalNum ;
			MBase->MeshPositionSize += Frame->PosUnitSize * Frame->PositionNum ;
			MBase->MeshNormalNum    += Frame->NormalNum ;

			Src = ( BYTE * )( DWORD_PTR )F1Frame->PositionAndNormalData + ( DWORD_PTR )FHeader ;

			// ���W�f�[�^�̎擾
			if( F1Frame->VertFlag & MV1_FRAME_VERT_FLAG_POSITION_B16 )
			{
				xs = ( ( MV1_POSITION_16BIT_SUBINFO_F1 * )Src )[ 0 ] ;
				ys = ( ( MV1_POSITION_16BIT_SUBINFO_F1 * )Src )[ 1 ] ;
				zs = ( ( MV1_POSITION_16BIT_SUBINFO_F1 * )Src )[ 2 ] ;
				Src += 24 ;

				Pos = Frame->Position ;
				for( j = 0 ; j < Frame->PositionNum ; j ++, Pos = ( MV1_MESH_POSITION * )( ( BYTE * )Pos + Frame->PosUnitSize ) )
				{
					Pos->Position.x = ( ( WORD * )Src )[ 0 ] * xs.Width / 60000.0f + xs.Min ;
					Pos->Position.y = ( ( WORD * )Src )[ 1 ] * ys.Width / 60000.0f + ys.Min ;
					Pos->Position.z = ( ( WORD * )Src )[ 2 ] * zs.Width / 60000.0f + zs.Min ;
					Src += 6 ;
				}
			}
			else
			{
				Pos = Frame->Position ;
				for( j = 0 ; j < Frame->PositionNum ; j ++, Pos = ( MV1_MESH_POSITION * )( ( BYTE * )Pos + Frame->PosUnitSize ) )
				{
					Pos->Position = *( ( VECTOR * )Src ) ;
					Src += 12 ;
				}
			}

			// ���_�E�G�C�g�f�[�^�̎擾
			if( ( F1Frame->VertFlag & MV1_FRAME_VERT_FLAG_MATRIX_WEIGHT_NONE ) == 0 )
			{
				DWORD IndexType, WeightType ;
				float TotalWeight ;
				int LastIndex ;

				IndexType  = ( DWORD )( ( F1Frame->VertFlag & MV1_FRAME_VERT_FLAG_MATRIX_INDEX_MASK  ) >> 4 ) ;
				WeightType = ( DWORD )( ( F1Frame->VertFlag & MV1_FRAME_VERT_FLAG_MATRIX_WEIGHT_MASK ) >> 5 ) ;

				Pos = Frame->Position ;
				for( j = 0 ; j < Frame->PositionNum ; j ++, Pos = ( MV1_MESH_POSITION * )( ( BYTE * )Pos + Frame->PosUnitSize ) )
				{
					TotalWeight = 0.0f ;
					LastIndex = -1 ;
					for( k = 0 ; k < Frame->MaxBoneBlendNum ; k ++ )
					{
						if( IndexType == MV1_FRAME_MATRIX_INDEX_TYPE_U8 )
						{
							Pos->BoneWeight[ k ].Index = *Src == 255 ? -1 : ( int )*Src ;
							Src ++ ;
						}
						else
						{
							Pos->BoneWeight[ k ].Index = *( ( WORD * )Src ) == 65535 ? -1 : ( int )*( ( WORD * )Src ) ;
							Src += 2 ;
						}

						if( Pos->BoneWeight[ k ].Index == -1 )
							break ;

						LastIndex = k ;

						if( WeightType == MV1_FRAME_MATRIX_WEIGHT_TYPE_U8 )
						{
							Pos->BoneWeight[ k ].W = *Src == 255 ? 1.0f : *Src / 255.0f ;
							Src ++ ;
						}
						else
						{
							Pos->BoneWeight[ k ].W = *( ( WORD * )Src ) == 65535 ? 1.0f : *( ( WORD * )Src ) / 65535.0f ;
							Src += 2 ;
						}
						TotalWeight += Pos->BoneWeight[ k ].W ;
					}
					if( TotalWeight < 1.0f && LastIndex > 0 )
					{
						Pos->BoneWeight[ LastIndex ].W += 1.0f - TotalWeight ;
					}
				}
			}

			// �@���f�[�^�̎擾
			Nrm = Frame->Normal ;
			switch( F1Frame->VertFlag & MV1_FRAME_VERT_FLAG_NORMAL_TYPE_MASK )
			{
			case MV1_FRAME_NORMAL_TYPE_NONE :
				AutoNormalCreateFlag = TRUE ;
				break ;

			case MV1_FRAME_NORMAL_TYPE_S8 :
				if( F1Frame->VertFlag & MV1_FRAME_VERT_FLAG_NOMRAL_TANGENT_BINORMAL )
				{
					for( j = 0 ; j < Frame->NormalNum ; j ++, Nrm ++ )
					{
						Nrm->Normal.x   = ( ( char * )Src )[ 0 ] / 127.0f ;
						Nrm->Normal.y   = ( ( char * )Src )[ 1 ] / 127.0f ;
						Nrm->Normal.z   = ( ( char * )Src )[ 2 ] / 127.0f ;

						Nrm->Tangent.x  = ( ( char * )Src )[ 3 ] / 127.0f ;
						Nrm->Tangent.y  = ( ( char * )Src )[ 4 ] / 127.0f ;
						Nrm->Tangent.z  = ( ( char * )Src )[ 5 ] / 127.0f ;

						Nrm->Binormal.x = ( ( char * )Src )[ 6 ] / 127.0f ;
						Nrm->Binormal.y = ( ( char * )Src )[ 7 ] / 127.0f ;
						Nrm->Binormal.z = ( ( char * )Src )[ 8 ] / 127.0f ;
						Src += 9 ;
					}
				}
				else
				{
					for( j = 0 ; j < Frame->NormalNum ; j ++, Nrm ++ )
					{
						Nrm->Normal.x = ( ( char * )Src )[ 0 ] / 127.0f ;
						Nrm->Normal.y = ( ( char * )Src )[ 1 ] / 127.0f ;
						Nrm->Normal.z = ( ( char * )Src )[ 2 ] / 127.0f ;
						Src += 3 ;
					}
				}
				break ;

			case MV1_FRAME_NORMAL_TYPE_S16 :
				if( F1Frame->VertFlag & MV1_FRAME_VERT_FLAG_NOMRAL_TANGENT_BINORMAL )
				{
					for( j = 0 ; j < Frame->NormalNum ; j ++, Nrm ++ )
					{
						Nrm->Normal.x   = ( ( short * )Src )[ 0 ] / 32767.0f ;
						Nrm->Normal.y   = ( ( short * )Src )[ 1 ] / 32767.0f ;
						Nrm->Normal.z   = ( ( short * )Src )[ 2 ] / 32767.0f ;

						Nrm->Tangent.x  = ( ( short * )Src )[ 3 ] / 32767.0f ;
						Nrm->Tangent.y  = ( ( short * )Src )[ 4 ] / 32767.0f ;
						Nrm->Tangent.z  = ( ( short * )Src )[ 5 ] / 32767.0f ;

						Nrm->Binormal.x = ( ( short * )Src )[ 6 ] / 32767.0f ;
						Nrm->Binormal.y = ( ( short * )Src )[ 7 ] / 32767.0f ;
						Nrm->Binormal.z = ( ( short * )Src )[ 8 ] / 32767.0f ;
						Src += 18 ;
					}
				}
				else
				{
					for( j = 0 ; j < Frame->NormalNum ; j ++, Nrm ++ )
					{
						Nrm->Normal.x = ( ( short * )Src )[ 0 ] / 32767.0f ;
						Nrm->Normal.y = ( ( short * )Src )[ 1 ] / 32767.0f ;
						Nrm->Normal.z = ( ( short * )Src )[ 2 ] / 32767.0f ;
						Src += 6 ;
					}
				}
				break ;

			case MV1_FRAME_NORMAL_TYPE_F32 :
				if( F1Frame->VertFlag & MV1_FRAME_VERT_FLAG_NOMRAL_TANGENT_BINORMAL )
				{
					for( j = 0 ; j < Frame->NormalNum ; j ++, Nrm ++ )
					{
						Nrm->Normal.x   = ( ( float * )Src )[ 0 ] ;
						Nrm->Normal.y   = ( ( float * )Src )[ 1 ] ;
						Nrm->Normal.z   = ( ( float * )Src )[ 2 ] ;

						Nrm->Tangent.x  = ( ( float * )Src )[ 3 ] ;
						Nrm->Tangent.y  = ( ( float * )Src )[ 4 ] ;
						Nrm->Tangent.z  = ( ( float * )Src )[ 5 ] ;

						Nrm->Binormal.x = ( ( float * )Src )[ 6 ] ;
						Nrm->Binormal.y = ( ( float * )Src )[ 7 ] ;
						Nrm->Binormal.z = ( ( float * )Src )[ 8 ] ;
						Src += 36 ;
					}
				}
				else
				{
					for( j = 0 ; j < Frame->NormalNum ; j ++, Nrm ++ )
					{
						Nrm->Normal.x = ( ( float * )Src )[ 0 ] ;
						Nrm->Normal.y = ( ( float * )Src )[ 1 ] ;
						Nrm->Normal.z = ( ( float * )Src )[ 2 ] ;
						Src += 12 ;
					}
				}
				break ;
			}
		}
	}

	// �e�N�X�`���̏����Z�b�g
	MBase->TextureAllocMem = FALSE ;
	Texture = MBase->Texture ;
	F1Texture = ( MV1_TEXTURE_F1 * )( ( BYTE * )FHeader + ( DWORD_PTR )FHeader->Texture ) ;
	for( i = 0 ; i < MBase->TextureNum ; i ++, Texture ++, F1Texture = ( MV1_TEXTURE_F1 * )( ( DWORD_PTR )FHeader + ( DWORD_PTR )F1Texture->DimNext ) )
	{
		// ���[�U�[�f�[�^��������
		Texture->UserData[ 0 ] = 0 ;
		Texture->UserData[ 1 ] = 0 ;
		Texture->UseUserGraphHandle = 0 ;
		Texture->UserGraphHandle = 0 ;

		// ���O��ۑ�
		Texture->NameAllocMem = FALSE ;
		MV1LoadModelToMV1_GetString( FHeader, MBase, FileStringBuffer, F1Texture->Name,
#ifndef UNICODE
			&Texture->NameA,
#endif
			&Texture->NameW
		) ;

		// ���]�t���O���Z�b�g����
		Texture->ReverseFlag = ( F1Texture->Flag & MV1_TEXTURE_FLAG_REVERSE ) != 0 ? 1 : 0 ;

		// �R�Q�r�b�g�a�l�o�̃A���t�@�l���S���O�������� XRGB8 �Ƃ��Ĉ������ǂ����̃t���O���Z�b�g����
		Texture->Bmp32AllZeroAlphaToXRGB8Flag = ( F1Texture->Flag & MV1_TEXTURE_FLAG_BMP32_ALL_ZERO_ALPHA_TO_XRGB8 ) != 0 ? 1 : 0 ;

		// �e�N�X�`���̓ǂݍ���
		{
			wchar_t ColorPathW[ FILEPATH_MAX ] ;
			wchar_t AlphaPathW[ FILEPATH_MAX ] ;

			if( F1Texture->ColorFilePath )
			{
				MV1LoadModelToMV1_GetString( FHeader, MBase, FileStringBuffer, F1Texture->ColorFilePath,
#ifndef UNICODE
					&Texture->ColorFilePathA,
#endif
					&Texture->ColorFilePathW
				) ;

				if( LoadParam->CurrentDir == NULL )
				{
					_WCSCPY_S( ColorPathW, sizeof( ColorPathW ), Texture->ColorFilePathW ) ;
				}
				else
				{
					ConvertFullPathW_( Texture->ColorFilePathW, ColorPathW, sizeof( ColorPathW ), DirectoryPath ) ;
				}
			}

			if( F1Texture->AlphaFilePath )
			{
				MV1LoadModelToMV1_GetString( FHeader, MBase, FileStringBuffer, F1Texture->AlphaFilePath,
#ifndef UNICODE
					&Texture->AlphaFilePathA,
#endif
					&Texture->AlphaFilePathW
				) ;

				if( LoadParam->CurrentDir == NULL )
				{
					_WCSCPY_S( AlphaPathW, sizeof( AlphaPathW ), Texture->AlphaFilePathW ) ;
				}
				else
				{
					ConvertFullPathW_( Texture->AlphaFilePathW, AlphaPathW, sizeof( AlphaPathW ), DirectoryPath ) ;
				}
			}

			if( __MV1LoadTexture(
					&Texture->ColorImage, &Texture->ColorImageSize,
					&Texture->AlphaImage, &Texture->AlphaImageSize,
					&Texture->GraphHandle,
					&Texture->SemiTransFlag,
					&Texture->IsDefaultTexture,
#ifndef UNICODE
					NULL, NULL,
#endif
					NULL, NULL,
					F1Texture->ColorFilePath ? ColorPathW : NULL,
					F1Texture->AlphaFilePath ? AlphaPathW : NULL,
					LoadParam->CurrentDir == NULL ? NULL : DirectoryPath,
					F1Texture->BumpImageFlag, F1Texture->BumpImageNextPixelLength,
					Texture->ReverseFlag,
					Texture->Bmp32AllZeroAlphaToXRGB8Flag,
					LoadParam->FileReadFunc,
					false,
					TRUE,
					ASyncThread ) == -1 )
			{
				DXST_ERRORLOGFMT_ADDW(( L"MV1 LoadModel Error : Txture Load Error : %s\n", Texture->NameW ) ) ;
				goto ERRORLABEL ;
			}

			// �t�@�C���p�X�p�Ƀ��������m�ۂ����t���O��|��
			Texture->ColorImageFilePathAllocMem = FALSE ;
			Texture->AlphaImageFilePathAllocMem = FALSE ;

			// �t�@�C���p�X��ۑ�
			if( Texture->ColorImage == NULL || F1Texture->ColorFilePath == 0 )
			{
#ifndef UNICODE
				Texture->ColorFilePathA = NULL ;
#endif
				Texture->ColorFilePathW = NULL ;
			}
			if( Texture->AlphaImage == NULL || F1Texture->AlphaFilePath == 0 )
			{
#ifndef UNICODE
				Texture->AlphaFilePathA = NULL ;
#endif
				Texture->AlphaFilePathW = NULL ;
			}
		}

		// �A�h���b�V���O���[�h�̃Z�b�g
		Texture->AddressModeU = F1Texture->AddressModeU ;
		Texture->AddressModeV = F1Texture->AddressModeV ;

		// �X�P�[�����O�̃Z�b�g
		if( ( F1Texture->Flag & MV1_TEXTURE_FLAG_VALID_SCALE_UV ) != 0 )
		{
			Texture->ScaleU = F1Texture->ScaleU ;
			Texture->ScaleV = F1Texture->ScaleV ;
		}
		else
		{
			Texture->ScaleU = 1.0f ;
			Texture->ScaleV = 1.0f ;
		}

		// �t�B���^�����O���[�h�̃Z�b�g
		Texture->FilterMode = F1Texture->FilterMode ;

		// �o���v�}�b�v����ۑ�
		Texture->BumpImageFlag = F1Texture->BumpImageFlag ;
		Texture->BumpImageNextPixelLength = F1Texture->BumpImageNextPixelLength ;

		// �摜�̃T�C�Y���擾
		NS_GetGraphSize( Texture->GraphHandle, &Texture->Width, &Texture->Height ) ;
	}

	// �}�e���A���̏����Z�b�g
	Material = MBase->Material ;
	F1Material = ( MV1_MATERIAL_F1 * )( ( BYTE * )FHeader + ( DWORD_PTR )FHeader->Material ) ;
	for( i = 0 ; i < MBase->MaterialNum ; i ++, Material ++, F1Material = ( MV1_MATERIAL_F1 * )( ( DWORD_PTR )FHeader + ( DWORD_PTR )F1Material->DimNext ) )
	{
		MV1LoadModelToMV1_GetString( FHeader, MBase, FileStringBuffer, F1Material->Name,
#ifndef UNICODE
			&Material->NameA,
#endif
			&Material->NameW
		) ;

		Material->Diffuse  = F1Material->Diffuse ;
		Material->Ambient  = F1Material->Ambient ;
		Material->Specular = F1Material->Specular ;
		Material->Emissive = F1Material->Emissive ;
		Material->Power    = F1Material->Power ;

		Material->UseAlphaTest = F1Material->UseAlphaTest ;
		Material->AlphaFunc    = F1Material->AlphaFunc ;
		Material->AlphaRef     = F1Material->AlphaRef ;

		Material->DiffuseLayerNum = F1Material->DiffuseLayerNum ;
		MaterialLayer      = Material->DiffuseLayer ;
		F1MaterialLayer    = F1Material->DiffuseLayer ;
		for( j = 0 ; j < 8 ; j ++, MaterialLayer ++, F1MaterialLayer ++ )
		{
			MaterialLayer->BlendType = F1MaterialLayer->BlendType ;
			MaterialLayer->Texture = F1MaterialLayer->Texture ;
		}

		Material->SpecularLayerNum = F1Material->SpecularLayerNum ;
		MaterialLayer      = Material->SpecularLayer ;
		F1MaterialLayer    = F1Material->SpecularLayer ;
		for( j = 0 ; j < 8 ; j ++, MaterialLayer ++, F1MaterialLayer ++ )
		{
			MaterialLayer->BlendType = F1MaterialLayer->BlendType ;
			MaterialLayer->Texture = F1MaterialLayer->Texture ;
		}

		Material->NormalLayerNum = F1Material->NormalLayerNum ;
		MaterialLayer      = Material->NormalLayer ;
		F1MaterialLayer    = F1Material->NormalLayer ;
		for( j = 0 ; j < 8 ; j ++, MaterialLayer ++, F1MaterialLayer ++ )
		{
			MaterialLayer->BlendType = F1MaterialLayer->BlendType ;
			MaterialLayer->Texture = F1MaterialLayer->Texture ;
		}

		Material->DrawBlendMode = F1Material->DrawBlendMode ;
		Material->DrawBlendParam = F1Material->DrawBlendParam ;

		if( F1Material->ToonInfo )
		{
			F1MaterialToon = ( MV1_MATERIAL_TOON_F1 * )( ( DWORD_PTR )F1Material->ToonInfo + ( DWORD_PTR )FHeader ) ;

			Material->Type = F1MaterialToon->Type ;
			Material->DiffuseGradBlendType = F1MaterialToon->DiffuseGradBlendType ;
			Material->DiffuseGradTexture = F1MaterialToon->DiffuseGradTexture ;
			Material->SpecularGradBlendType = F1MaterialToon->SpecularGradBlendType ;
			Material->SpecularGradTexture = F1MaterialToon->SpecularGradTexture ;
			Material->OutLineColor = F1MaterialToon->OutLineColor ;
			Material->OutLineWidth = F1MaterialToon->OutLineWidth ;
			Material->OutLineDotWidth = F1MaterialToon->OutLineDotWidth ;

			if( F1MaterialToon->EnableSphereMap != 0 )
			{
				Material->SphereMapTexture = ( int )F1MaterialToon->SphereMapTexture ;
				Material->SphereMapBlendType = ( int )F1MaterialToon->SphereMapBlendType ;
			}
			else
			{
				Material->SphereMapTexture = -1 ;
				Material->SphereMapBlendType = DX_MATERIAL_BLENDTYPE_MODULATE ;
			}
		}
		else
		{
			Material->Type = DX_MATERIAL_TYPE_NORMAL ;
			Material->DiffuseGradBlendType = DX_MATERIAL_BLENDTYPE_MODULATE ;
			Material->DiffuseGradTexture = -1 ;
			Material->SphereMapTexture = -1 ;
			Material->SpecularGradBlendType = DX_MATERIAL_BLENDTYPE_ADDITIVE ;
			Material->SpecularGradTexture = -1 ;
			Material->SphereMapBlendType = DX_MATERIAL_BLENDTYPE_MODULATE ;
			Material->OutLineColor = GetColorF( 0.0f, 0.0f, 0.0f, 1.0f ) ;
			Material->OutLineWidth = 0.0f ;
			Material->OutLineDotWidth = 0.0f ;
		}
	}

	// ���C�g�̏����Z�b�g
	Light = MBase->Light ;
	F1Light = ( MV1_LIGHT_F1 * )( ( BYTE * )FHeader + ( DWORD_PTR )FHeader->Light ) ;
	for( i = 0 ; i < MBase->LightNum ; i ++, Light ++, F1Light = ( MV1_LIGHT_F1 * )( ( DWORD_PTR )FHeader + ( DWORD_PTR )F1Light->DimNext ) )
	{
		MV1LoadModelToMV1_GetString( FHeader, MBase, FileStringBuffer, F1Light->Name,
#ifndef UNICODE
			&Light->NameA,
#endif
			&Light->NameW
		) ;

		Light->Index = F1Light->Index ;
		Light->FrameIndex = F1Light->FrameIndex ;
		Light->Type = F1Light->Type ;
		Light->Diffuse = F1Light->Diffuse ;
		Light->Specular = F1Light->Specular ;
		Light->Ambient = F1Light->Ambient ;
		Light->Range = F1Light->Range ;
		Light->Falloff = F1Light->Falloff ;
		Light->Attenuation0 = F1Light->Attenuation0 ;
		Light->Attenuation1 = F1Light->Attenuation1 ;
		Light->Attenuation2 = F1Light->Attenuation2 ;
		Light->Theta = F1Light->Theta ;
		Light->Phi = F1Light->Phi ;
	}

	// ���b�V���̏����Z�b�g
	Mesh = MBase->Mesh ;
	F1Mesh = ( MV1_MESH_F1 * )( ( BYTE * )FHeader + ( DWORD_PTR )FHeader->Mesh ) ;
	for( i = 0 ; i < MBase->MeshNum ; i ++, Mesh ++, F1Mesh = ( MV1_MESH_F1 * )( ( DWORD_PTR )FHeader + ( DWORD_PTR )F1Mesh->DimNext ) )
	{
		if( F1Mesh->Container )
		{
			Mesh->Container = MBase->Frame + ( ( MV1_FRAME_F1 * )( ( DWORD_PTR )FHeader + ( DWORD_PTR )F1Mesh->Container ) )->Index ;
		}
		if( F1Mesh->Material )
		{
			Mesh->Material = MBase->Material + ( ( MV1_MATERIAL_F1 * )( ( DWORD_PTR )FHeader + ( DWORD_PTR )F1Mesh->Material ) )->Index ;
		}

		Mesh->ChangeInfo.Target   = F1Mesh->ChangeInfo.Target ;
		Mesh->ChangeInfo.Fill     = NULL ;
		Mesh->ChangeInfo.Size     = F1Mesh->ChangeInfo.Size ;
		Mesh->ChangeInfo.CheckBit = F1Mesh->ChangeInfo.CheckBit ;

		Mesh->NotOneDiffuseAlpha = F1Mesh->NotOneDiffuseAlpha ;
		Mesh->Shape = F1Mesh->Shape ;
		Mesh->UseVertexDiffuseColor = F1Mesh->UseVertexDiffuseColor ;
		Mesh->UseVertexSpecularColor = F1Mesh->UseVertexSpecularColor ;

		Mesh->TriangleListNum = F1Mesh->TriangleListNum ;
		if( F1Mesh->TriangleList )
		{
			Mesh->TriangleList = MBase->TriangleList + ( ( MV1_TRIANGLE_LIST_F1 * )( ( DWORD_PTR )FHeader + ( DWORD_PTR )F1Mesh->TriangleList ) )->Index ;
		}

		Mesh->Visible = F1Mesh->Visible ;
		Mesh->BackCulling = F1Mesh->BackCulling ;

		Mesh->FaceNum = 0 ;

		// ���_�f�[�^���擾����
		{
			MV1_MESH_VERTEX *Vert ;

			Mesh->Vertex = ( MV1_MESH_VERTEX * )( ( BYTE * )MBase->MeshVertex + MBase->MeshVertexSize ) ;
			Mesh->Face   = MBase->MeshFace + MBase->MeshFaceNum ;
			MBase->MeshVertexSize += Mesh->VertexNum * Mesh->VertUnitSize ;
			MBase->MeshFaceNum    += F1Mesh->FaceNum ;

			Src = ( BYTE * )( DWORD_PTR )F1Mesh->VertexData + ( DWORD_PTR )FHeader ;

			// ���L���_�J���[���ǂ����ŏ����𕪊�
			if( F1Mesh->VertFlag & MV1_MESH_VERT_FLAG_COMMON_COLOR )
			{
				COLOR_U8 DiffuseColor, SpecularColor ;

				// ���L���_�J���[���擾����
				DiffuseColor  = ( ( COLOR_U8 * )Src )[ 0 ] ;
				SpecularColor = ( ( COLOR_U8 * )Src )[ 1 ] ;
				Src += 8 ;

				// �S�Ă̒��_�̃J���[�ɃZ�b�g����
				Vert = Mesh->Vertex ;
				for( j = 0 ; j < Mesh->VertexNum ; j ++, Vert = ( MV1_MESH_VERTEX * )( ( BYTE * )Vert + Mesh->VertUnitSize ) )
				{
					Vert->DiffuseColor  = DiffuseColor ;
					Vert->SpecularColor = SpecularColor ;
				}
			}

			// ���W�C���f�b�N�X���擾����
			switch( F1Mesh->VertFlag & MV1_MESH_VERT_FLAG_POS_IND_TYPE_MASK )
			{
			case MV1_MESH_VERT_INDEX_TYPE_U8 :
				Vert = Mesh->Vertex ;
				for( j = 0 ; j < Mesh->VertexNum ; j ++, Vert = ( MV1_MESH_VERTEX * )( ( BYTE * )Vert + Mesh->VertUnitSize ) )
				{
					Vert->PositionIndex = *Src ;
					Src ++ ;
				}
				break ;

			case MV1_MESH_VERT_INDEX_TYPE_U16 :
				Vert = Mesh->Vertex ;
				for( j = 0 ; j < Mesh->VertexNum ; j ++, Vert = ( MV1_MESH_VERTEX * )( ( BYTE * )Vert + Mesh->VertUnitSize ) )
				{
					Vert->PositionIndex = *( ( WORD * )Src ) ;
					Src += 2 ;
				}
				break ;

			case MV1_MESH_VERT_INDEX_TYPE_U32 :
				Vert = Mesh->Vertex ;
				for( j = 0 ; j < Mesh->VertexNum ; j ++, Vert = ( MV1_MESH_VERTEX * )( ( BYTE * )Vert + Mesh->VertUnitSize ) )
				{
					Vert->PositionIndex = *( ( DWORD * )Src ) ;
					Src += 4 ;
				}
				break ;
			}

			// �@���C���f�b�N�X���擾����
			switch( ( F1Mesh->VertFlag & MV1_MESH_VERT_FLAG_NRM_IND_TYPE_MASK ) >> 2 )
			{
			case MV1_MESH_VERT_INDEX_TYPE_NONE :
				break ;

			case MV1_MESH_VERT_INDEX_TYPE_U8 :
				Vert = Mesh->Vertex ;
				for( j = 0 ; j < Mesh->VertexNum ; j ++, Vert = ( MV1_MESH_VERTEX * )( ( BYTE * )Vert + Mesh->VertUnitSize ) )
				{
					Vert->NormalIndex = *Src ;
					Src ++ ;
				}
				break ;

			case MV1_MESH_VERT_INDEX_TYPE_U16 :
				Vert = Mesh->Vertex ;
				for( j = 0 ; j < Mesh->VertexNum ; j ++, Vert = ( MV1_MESH_VERTEX * )( ( BYTE * )Vert + Mesh->VertUnitSize ) )
				{
					Vert->NormalIndex = *( ( WORD * )Src ) ;
					Src += 2 ;
				}
				break ;

			case MV1_MESH_VERT_INDEX_TYPE_U32 :
				Vert = Mesh->Vertex ;
				for( j = 0 ; j < Mesh->VertexNum ; j ++, Vert = ( MV1_MESH_VERTEX * )( ( BYTE * )Vert + Mesh->VertUnitSize ) )
				{
					Vert->NormalIndex = *( ( DWORD * )Src ) ;
					Src += 4 ;
				}
				break ;
			}

			// ���_�J���[���擾����
			if( ( F1Mesh->VertFlag & MV1_MESH_VERT_FLAG_COMMON_COLOR ) == 0 )
			{
				Vert = Mesh->Vertex ;
				for( j = 0 ; j < Mesh->VertexNum ; j ++, Vert = ( MV1_MESH_VERTEX * )( ( BYTE * )Vert + Mesh->VertUnitSize ) )
				{
					Vert->DiffuseColor  = ( ( COLOR_U8 * )Src )[ 0 ] ;
					Vert->SpecularColor = ( ( COLOR_U8 * )Src )[ 1 ] ;
					Src += 8 ;
				}
			}

			// �t�u�l���擾����
			if( F1Mesh->VertFlag & MV1_MESH_VERT_FLAG_UV_U16 )
			{
				Vert = Mesh->Vertex ;
				for( j = 0 ; j < Mesh->VertexNum ; j ++, Vert = ( MV1_MESH_VERTEX * )( ( BYTE * )Vert + Mesh->VertUnitSize ) )
				{
					for( k = 0 ; k < Mesh->UVSetUnitNum ; k ++ )
					{
						Vert->UVs[ k ][ 0 ] = ( ( WORD * )Src )[ 0 ] / 65535.0f ;
						Vert->UVs[ k ][ 1 ] = ( ( WORD * )Src )[ 1 ] / 65535.0f ;
						Src += 4 ;
					}
				}
			}
			else
			{
				Vert = Mesh->Vertex ;
				for( j = 0 ; j < Mesh->VertexNum ; j ++, Vert = ( MV1_MESH_VERTEX * )( ( BYTE * )Vert + Mesh->VertUnitSize ) )
				{
					for( k = 0 ; k < Mesh->UVSetUnitNum ; k ++ )
					{
						Vert->UVs[ k ][ 0 ] = ( ( float * )Src )[ 0 ] ;
						Vert->UVs[ k ][ 1 ] = ( ( float * )Src )[ 1 ] ;
						Src += 8 ;
					}
				}
			}

			// �g�D�[���p�̗֊s����\�����邩�ǂ����̏����擾����
			if( F1Mesh->VertFlag & MV1_MESH_VERT_FLAG_NON_TOON_OUTLINE )
			{
				DWORD BitNum ;
				BYTE In ;

				Vert = Mesh->Vertex ;
				BitNum = 0 ;
				In = 0 ;
				for( j = 0 ; j < Mesh->VertexNum ; j ++, Vert = ( MV1_MESH_VERTEX * )( ( BYTE * )Vert + Mesh->VertUnitSize ) )
				{
					if( BitNum == 0 )
					{
						In = *Src ;
						Src ++ ;
						BitNum = 8 ;
					}
					if( In & ( 1 << ( 8 - BitNum ) ) )
					{
						Vert->ToonOutLineScale = 0.0f ;
					}
					else
					{
						Vert->ToonOutLineScale = 1.0f ;
					}
					BitNum -- ;
				}
			}
		}
	}

	if( F1FileHeadShape )
	{
		// �V�F�C�v�̏����Z�b�g
		Shape = MBase->Shape ;
		F1Shape = ( MV1_SHAPE_F1 * )( ( BYTE * )FHeader + ( DWORD_PTR )F1FileHeadShape->Data ) ;
		for( i = 0 ; i < MBase->ShapeNum ; i ++, Shape ++, F1Shape = ( MV1_SHAPE_F1 * )( ( DWORD_PTR )FHeader + ( DWORD_PTR )F1Shape->DimNext ) )
		{
			if( F1Shape->Container )
			{
				Shape->Container = MBase->Frame + ( ( MV1_FRAME_F1 * )( ( DWORD_PTR )FHeader + ( DWORD_PTR )F1Shape->Container ) )->Index ;
			}

			MV1LoadModelToMV1_GetString( FHeader, MBase, FileStringBuffer, F1Shape->Name,
#ifndef UNICODE
				&Shape->NameA,
#endif
				&Shape->NameW
			) ;

			Shape->MeshNum = F1Shape->MeshNum ;

			if( F1Shape->Mesh )
			{
				Shape->Mesh = MBase->ShapeMesh + ( ( MV1_SHAPE_MESH_F1 * )( ( DWORD_PTR )FHeader + ( DWORD_PTR )F1Shape->Mesh ) )->Index ;
			}
		}

		// �V�F�C�v���b�V���̏����Z�b�g
		ShapeMesh = MBase->ShapeMesh ;
		F1ShapeMesh = ( MV1_SHAPE_MESH_F1 * )( ( BYTE * )FHeader + ( DWORD_PTR )F1FileHeadShape->Mesh ) ;
		for( i = 0 ; i < MBase->ShapeMeshNum ; i ++, ShapeMesh ++, F1ShapeMesh = ( MV1_SHAPE_MESH_F1 * )( ( DWORD_PTR )FHeader + ( DWORD_PTR )F1ShapeMesh->DimNext ) )
		{
			if( F1ShapeMesh->TargetMesh )
			{
				ShapeMesh->TargetMesh = MBase->Mesh + ( ( MV1_MESH_F1 * )( ( DWORD_PTR )FHeader + ( DWORD_PTR )F1ShapeMesh->TargetMesh ) )->Index ;
			}

			ShapeMesh->VertexNum = F1ShapeMesh->VertexNum ;

			if( F1ShapeMesh->Vertex )
			{
				ShapeMesh->Vertex = MBase->ShapeVertex + MBase->ShapeVertexNum ;
				MBase->ShapeVertexNum += ShapeMesh->VertexNum ;
				ShapeVertex = ShapeMesh->Vertex ;
				F1ShapeVertex = ( MV1_SHAPE_VERTEX_F1 * )( ( DWORD_PTR )F1ShapeMesh->Vertex + ( DWORD_PTR )FHeader ) ;
				for( j = 0 ; ( DWORD )j < F1ShapeMesh->VertexNum ; j ++, ShapeVertex ++, F1ShapeVertex = ( MV1_SHAPE_VERTEX_F1 * )( ( BYTE * )F1ShapeVertex + F1FileHeadShape->ShapeVertexUnitSize ) )
				{
					ShapeVertex->TargetMeshVertex = F1ShapeVertex->TargetMeshVertex ;
					ShapeVertex->Position = F1ShapeVertex->Position ;
					ShapeVertex->Normal = F1ShapeVertex->Normal ;
				}
			}
		}
	}

	if( F1FileHeadPhysics )
	{
		// ���̂̏����Z�b�g
		PhysicsRigidBody = MBase->PhysicsRigidBody ;
		F1PhysicsRigidBody = ( MV1_PHYSICS_RIGIDBODY_F1 * )( ( BYTE * )FHeader + ( DWORD_PTR )F1FileHeadPhysics->RigidBody ) ;
		for( i = 0 ; i < MBase->PhysicsRigidBodyNum ; i ++, PhysicsRigidBody ++, F1PhysicsRigidBody = ( MV1_PHYSICS_RIGIDBODY_F1 * )( ( DWORD_PTR )FHeader + ( DWORD_PTR )F1PhysicsRigidBody->DimNext ) )
		{
			PhysicsRigidBody->Index = F1PhysicsRigidBody->Index ;
			
			MV1LoadModelToMV1_GetString( FHeader, MBase, FileStringBuffer, F1PhysicsRigidBody->Name,
#ifndef UNICODE
				&PhysicsRigidBody->NameA,
#endif
				&PhysicsRigidBody->NameW
			) ;

			if( F1PhysicsRigidBody->TargetFrame )
			{
				PhysicsRigidBody->TargetFrame = MBase->Frame + ( ( MV1_FRAME_F1 * )( ( DWORD_PTR )FHeader + ( DWORD_PTR )F1PhysicsRigidBody->TargetFrame ) )->Index ;
			}

			PhysicsRigidBody->RigidBodyGroupIndex = F1PhysicsRigidBody->RigidBodyGroupIndex ;
			PhysicsRigidBody->RigidBodyGroupTarget = F1PhysicsRigidBody->RigidBodyGroupTarget ;
			PhysicsRigidBody->ShapeType = F1PhysicsRigidBody->ShapeType ;
			PhysicsRigidBody->ShapeW = F1PhysicsRigidBody->ShapeW ;
			PhysicsRigidBody->ShapeH = F1PhysicsRigidBody->ShapeH ;
			PhysicsRigidBody->ShapeD = F1PhysicsRigidBody->ShapeD ;
			PhysicsRigidBody->Position = F1PhysicsRigidBody->Position ;
			PhysicsRigidBody->Rotation = F1PhysicsRigidBody->Rotation ;
			PhysicsRigidBody->RigidBodyWeight = F1PhysicsRigidBody->RigidBodyWeight ;
			PhysicsRigidBody->RigidBodyPosDim = F1PhysicsRigidBody->RigidBodyPosDim ;
			PhysicsRigidBody->RigidBodyRotDim = F1PhysicsRigidBody->RigidBodyRotDim ;
			PhysicsRigidBody->RigidBodyRecoil = F1PhysicsRigidBody->RigidBodyRecoil ;
			PhysicsRigidBody->RigidBodyFriction = F1PhysicsRigidBody->RigidBodyFriction ;
			PhysicsRigidBody->RigidBodyType = F1PhysicsRigidBody->RigidBodyType ;
			PhysicsRigidBody->NoCopyToBone = F1PhysicsRigidBody->NoCopyToBone ;
		}

		// ���̂̃W���C���g�����Z�b�g
		PhysicsJoint = MBase->PhysicsJoint ;
		F1PhysicsJoint = ( MV1_PHYSICS_JOINT_F1 * )( ( BYTE * )FHeader + ( DWORD_PTR )F1FileHeadPhysics->Joint ) ;
		for( i = 0 ; i < MBase->PhysicsJointNum ; i ++, PhysicsJoint ++, F1PhysicsJoint = ( MV1_PHYSICS_JOINT_F1 * )( ( DWORD_PTR )FHeader + ( DWORD_PTR )F1PhysicsJoint->DimNext ) )
		{
			PhysicsJoint->Index = F1PhysicsJoint->Index ;
			
			MV1LoadModelToMV1_GetString( FHeader, MBase, FileStringBuffer, F1PhysicsJoint->Name,
#ifndef UNICODE
				&PhysicsJoint->NameA,
#endif
				&PhysicsJoint->NameW
			) ;

			if( F1PhysicsJoint->RigidBodyA )
			{
				PhysicsJoint->RigidBodyA = MBase->PhysicsRigidBody + ( ( MV1_PHYSICS_RIGIDBODY_F1 * )( ( DWORD_PTR )FHeader + ( DWORD_PTR )F1PhysicsJoint->RigidBodyA ) )->Index ;
			}

			if( F1PhysicsJoint->RigidBodyB )
			{
				PhysicsJoint->RigidBodyB = MBase->PhysicsRigidBody + ( ( MV1_PHYSICS_RIGIDBODY_F1 * )( ( DWORD_PTR )FHeader + ( DWORD_PTR )F1PhysicsJoint->RigidBodyB ) )->Index ;
			}

			PhysicsJoint->Position = F1PhysicsJoint->Position ;
			PhysicsJoint->Rotation = F1PhysicsJoint->Rotation ;
			PhysicsJoint->ConstrainPosition1 = F1PhysicsJoint->ConstrainPosition1 ;
			PhysicsJoint->ConstrainPosition2 = F1PhysicsJoint->ConstrainPosition2 ;
			PhysicsJoint->ConstrainRotation1 = F1PhysicsJoint->ConstrainRotation1 ;
			PhysicsJoint->ConstrainRotation2 = F1PhysicsJoint->ConstrainRotation2 ;
			PhysicsJoint->SpringPosition = F1PhysicsJoint->SpringPosition ;
			PhysicsJoint->SpringRotation = F1PhysicsJoint->SpringRotation ;
		}
	}

	// �X�L�j���O���b�V���p�̃{�[�������Z�b�g
	{
		int UseFrameInfoNoneFlag = TRUE ;

		SkinBone = MBase->SkinBone ;
		F1SkinBone = ( MV1_SKIN_BONE_F1 * )( ( BYTE * )FHeader + ( DWORD_PTR )FHeader->SkinBone ) ;
		for( i = 0 ; i < MBase->SkinBoneNum ; i ++, SkinBone ++, F1SkinBone = ( MV1_SKIN_BONE_F1 * )( ( DWORD_PTR )FHeader + ( DWORD_PTR )F1SkinBone->DimNext ) )
		{
			SkinBone->BoneFrame = F1SkinBone->BoneFrame ;
			SkinBone->ModelLocalMatrix = F1SkinBone->ModelLocalMatrix ;
			SkinBone->ModelLocalMatrixIsTranslateOnly = F1SkinBone->ModelLocalMatrixIsTranslateOnly ;
			SkinBone->UseFrameNum = F1SkinBone->UseFrameNum ;
			if( F1SkinBone->UseFrame )
			{
				UseFrameInfoNoneFlag = FALSE ;
				SkinBone->UseFrame = MBase->SkinBoneUseFrame + ( ( MV1_SKIN_BONE_USE_FRAME_F1 * )( DWORD_PTR )F1SkinBone->UseFrame - ( MV1_SKIN_BONE_USE_FRAME_F1 * )( DWORD_PTR )FHeader->SkinBoneUseFrame ) ;
			}
		}
		
		// �E�G�C�g���g�p���Ă���{�[����񂪖��������ꍇ�͂����ō\�z����
		if( UseFrameInfoNoneFlag )
		{
			// �A�h���X���Z�b�g
			MBase->FrameUseSkinBoneNum = 0 ;
			SkinBone = MBase->SkinBone ;
			for( i = 0; i < MBase->SkinBoneNum; i ++, SkinBone ++ )
			{
				if( SkinBone->UseFrameNum <= 0 )
				{
					continue ;
				}

				SkinBone->UseFrame = MBase->SkinBoneUseFrame + MBase->FrameUseSkinBoneNum ;
				MBase->FrameUseSkinBoneNum += SkinBone->UseFrameNum ;
			}

			// �{�[���̐������Z�b�g
			SkinBone = MBase->SkinBone ;
			for( i = 0; i < MBase->SkinBoneNum; i ++, SkinBone ++ )
			{
				SkinBone->UseFrameNum = 0 ;
			}

			// �t���[�����g�p���Ă���{�[���̏����Z�b�g
			Frame = MBase->Frame ;
			for( i = 0 ; i < MBase->FrameNum ; i ++, Frame ++ )
			{
				for( j = 0 ; j < Frame->UseSkinBoneNum ; j ++ )
				{
					Frame->UseSkinBone[ j ]->UseFrame[ Frame->UseSkinBone[ j ]->UseFrameNum ].Index       = ( int )( Frame - MBase->Frame ) ;
					Frame->UseSkinBone[ j ]->UseFrame[ Frame->UseSkinBone[ j ]->UseFrameNum ].MatrixIndex = j ;
					Frame->UseSkinBone[ j ]->UseFrameNum ++ ;
				}
			}
		}
	}

	// �g���C�A���O�����X�g�̏����Z�b�g
	TriangleList = MBase->TriangleList ;
	F1TriangleList = ( MV1_TRIANGLE_LIST_F1 * )( ( BYTE * )FHeader + ( DWORD_PTR )FHeader->TriangleList ) ;
	for( i = 0 ; i < MBase->TriangleListNum ; i ++, TriangleList ++, F1TriangleList = ( MV1_TRIANGLE_LIST_F1 * )( ( DWORD_PTR )FHeader + ( DWORD_PTR )F1TriangleList->DimNext ) )
	{
		if( F1TriangleList->Container )
		{
			TriangleList->Container = MBase->Mesh + ( ( MV1_MESH_F1 * )( ( DWORD_PTR )FHeader + ( DWORD_PTR )F1TriangleList->Container ) )->Index ;
		}
		TriangleList->VertexType = F1TriangleList->VertexType ;
		TriangleList->VertexNum = F1TriangleList->VertexNum ;
		TriangleList->IndexNum = F1TriangleList->IndexNum ;

		// ���_�f�[�^�̓ǂݍ���
		{
			DWORD *MInd ;
			WORD *Ind ;

			TriangleList->Index = MBase->TriangleListIndex + MBase->TriangleListIndexNum ;
			TriangleList->ToonOutLineIndex = TriangleList->Index + FHeader->TriangleListIndexNum ;
			TriangleList->ToonOutLineIndexNum = 0 ;
			TriangleList->MeshVertexIndex   = MBase->MeshVertexIndex   + MBase->MeshVertexIndexNum ;
			MBase->TriangleListIndexNum += TriangleList->IndexNum ;
			MBase->MeshVertexIndexNum   += TriangleList->VertexNum ;

			Src = ( BYTE * )( DWORD_PTR )F1TriangleList->MeshVertexIndexAndIndexData + ( DWORD_PTR )FHeader ;

			// �{�[���̏����擾
			switch( TriangleList->VertexType )
			{
			case MV1_VERTEX_TYPE_SKIN_4BONE :
			case MV1_VERTEX_TYPE_SKIN_8BONE :
				TriangleList->UseBoneNum = *( ( WORD * )Src ) ;
				Src += 2 ;
				for( j = 0 ; j < TriangleList->UseBoneNum ; j ++ )
				{
					TriangleList->UseBone[ j ] = *( ( WORD * )Src ) ;
					Src += 2 ;
				}
				break ;

			case MV1_VERTEX_TYPE_SKIN_FREEBONE :
				TriangleList->MaxBoneNum = *( ( WORD * )Src ) ;
				Src += 2 ;
				break ;
			}

			// ���b�V�����_�C���f�b�N�X���擾����
			MInd = TriangleList->MeshVertexIndex ;
			switch( F1TriangleList->Flag & MV1_TRIANGLE_LIST_FLAG_MVERT_INDEX_MASK )
			{
			case MV1_TRIANGLE_LIST_INDEX_TYPE_U8 :
				for( j = 0 ; j < TriangleList->VertexNum ; j ++, MInd ++ )
				{
					*MInd = *Src ;
					Src ++ ;
				}
				break ;

			case MV1_TRIANGLE_LIST_INDEX_TYPE_U16 :
				for( j = 0 ; j < TriangleList->VertexNum ; j ++, MInd ++ )
				{
					*MInd = *( ( WORD * )Src ) ;
					Src += 2 ;
				}
				break ;

			case MV1_TRIANGLE_LIST_INDEX_TYPE_U32 :
				for( j = 0 ; j < TriangleList->VertexNum ; j ++, MInd ++ )
				{
					*MInd = *( ( DWORD * )Src ) ;
					Src += 4 ;
				}
				break ;
			}

			// �����A�N�Z�X�p�̒��_���W�f�[�^���Z�b�g
			switch( TriangleList->VertexType )
			{
			case MV1_VERTEX_TYPE_NORMAL :
				TriangleList->NormalPosition = MBase->TriangleListNormalPosition + MBase->TriangleListNormalPositionNum ;
				TriangleList->NormalPosition = ( MV1_TLIST_NORMAL_POS * )( ( ( DWORD_PTR )TriangleList->NormalPosition + 15 ) / 16 * 16 ) ;
				TriangleList->PosUnitSize = sizeof( MV1_TLIST_NORMAL_POS ) ;
				MBase->TriangleListNormalPositionNum += TriangleList->VertexNum ;
				break ;

			case MV1_VERTEX_TYPE_SKIN_4BONE :
				TriangleList->SkinPosition4B = MBase->TriangleListSkinPosition4B + MBase->TriangleListSkinPosition4BNum ;
				TriangleList->SkinPosition4B = ( MV1_TLIST_SKIN_POS_4B * )( ( ( DWORD_PTR )TriangleList->SkinPosition4B + 15 ) / 16 * 16 ) ;
				TriangleList->PosUnitSize = sizeof( MV1_TLIST_SKIN_POS_4B ) ;
				MBase->TriangleListSkinPosition4BNum += TriangleList->VertexNum ;
				break ;

			case MV1_VERTEX_TYPE_SKIN_8BONE :
				TriangleList->SkinPosition8B = MBase->TriangleListSkinPosition8B + MBase->TriangleListSkinPosition8BNum ;
				TriangleList->SkinPosition8B = ( MV1_TLIST_SKIN_POS_8B * )( ( ( DWORD_PTR )TriangleList->SkinPosition8B + 15 ) / 16 * 16 ) ;
				TriangleList->PosUnitSize = sizeof( MV1_TLIST_SKIN_POS_8B ) ;
				MBase->TriangleListSkinPosition8BNum += TriangleList->VertexNum ;
				break ;

			case MV1_VERTEX_TYPE_SKIN_FREEBONE :
				TriangleList->SkinPositionFREEB = ( MV1_TLIST_SKIN_POS_FREEB * )( ( BYTE * )MBase->TriangleListSkinPositionFREEB + MBase->TriangleListSkinPositionFREEBSize ) ;
				TriangleList->SkinPositionFREEB = ( MV1_TLIST_SKIN_POS_FREEB * )( ( ( DWORD_PTR )TriangleList->SkinPositionFREEB + 15 ) / 16 * 16 ) ;
				TriangleList->PosUnitSize = ( unsigned short )( sizeof( MV1_TLIST_SKIN_POS_FREEB ) + sizeof( MV1_SKINBONE_BLEND ) * ( TriangleList->MaxBoneNum - 4 ) ) ;
				TriangleList->PosUnitSize = ( unsigned short )( ( TriangleList->PosUnitSize + 15 ) / 16 * 16 ) ;
				MBase->TriangleListSkinPositionFREEBSize += TriangleList->PosUnitSize * TriangleList->VertexNum ;
				break ;
			}
//			MV1SetupTriangleListPositionAndNormal( TriangleList ) ;

			// ���_�C���f�b�N�X���擾����
			Ind = TriangleList->Index ;
			switch( ( F1TriangleList->Flag & MV1_TRIANGLE_LIST_FLAG_INDEX_MASK ) >> 2 )
			{
			case MV1_TRIANGLE_LIST_INDEX_TYPE_U8 :
				for( j = 0 ; j < TriangleList->IndexNum ; j ++, Ind ++ )
				{
					*Ind = *Src ;
					Src ++ ;
				}
				break ;

			case MV1_TRIANGLE_LIST_INDEX_TYPE_U16 :
				for( j = 0 ; j < TriangleList->IndexNum ; j ++, Ind ++ )
				{
					*Ind = *( ( WORD * )Src ) ;
					Src += 2 ;
				}
				break ;
			}
		}
	}

	// ���b�V���̃t�F�C�X�����\�z����
	{
		WORD *Ind ;
		DWORD *MInd, TListInd ;

		TriangleList = MBase->TriangleList ;
		for( i = 0 ; i < MBase->TriangleListNum ; i ++, TriangleList ++ )
		{
			Mesh = TriangleList->Container ;
			Ind = TriangleList->Index ;
			MInd = TriangleList->MeshVertexIndex ;
			TListInd = ( DWORD )( TriangleList - Mesh->TriangleList ) ;
			for( j = 0 ; j < TriangleList->IndexNum ; j += 3, Ind += 3 )
			{
				Face = &Mesh->Face[ Mesh->FaceNum ] ;
				Face->TriangleListIndex = ( WORD )TListInd ;
				Face->VertexType = TriangleList->VertexType ;
				Face->VertexIndex[ 0 ] = MInd[ Ind[ 0 ] ] ;
				Face->VertexIndex[ 1 ] = MInd[ Ind[ 1 ] ] ;
				Face->VertexIndex[ 2 ] = MInd[ Ind[ 2 ] ] ;
				Mesh->FaceNum ++ ;
			}
		}
	}

	// �@���̍Đ����̎w�肪����ꍇ�͐������s��
	if( MV1Man.LoadModelToReMakeNormal || AutoNormalCreateFlag )
	{
		// �@���̎�������
		{
			VECTOR Nrm ;
			MV1_MESH_NORMAL *MNrm ;
			MV1_MESH_VERTEX *Vertex[ 3 ] ;
			DWORD VertUnitSize, PosUnitSize ;

			// �S�Ẵt���[��������
			Frame = MBase->Frame ;
			for( i = 0 ; i < MBase->FrameNum ; i ++, Frame ++ )
			{
				if( Frame->AutoCreateNormal == 0 ) continue ;

				PosUnitSize = ( DWORD )Frame->PosUnitSize ;

				// �ʂ̖@�����Z�o���Ȃ��瑫���Ă���
				Mesh = Frame->Mesh ;
				for( j = 0 ; j < Frame->MeshNum ; j ++, Mesh ++ )
				{
					VertUnitSize = ( DWORD )Mesh->VertUnitSize ;

					Face = Mesh->Face ;
					for( k = 0 ; k < Mesh->FaceNum ; k ++, Face ++ )
					{
						Vertex[ 0 ] = ( MV1_MESH_VERTEX * )( ( BYTE * )Mesh->Vertex + VertUnitSize * Face->VertexIndex[ 0 ] ) ;
						Vertex[ 1 ] = ( MV1_MESH_VERTEX * )( ( BYTE * )Mesh->Vertex + VertUnitSize * Face->VertexIndex[ 1 ] ) ;
						Vertex[ 2 ] = ( MV1_MESH_VERTEX * )( ( BYTE * )Mesh->Vertex + VertUnitSize * Face->VertexIndex[ 2 ] ) ;

						Nrm = VNorm( VCross( 
							VSub( ( ( MV1_MESH_POSITION * )( ( BYTE * )Frame->Position + PosUnitSize * Vertex[ 1 ]->PositionIndex ) )->Position, 
								  ( ( MV1_MESH_POSITION * )( ( BYTE * )Frame->Position + PosUnitSize * Vertex[ 0 ]->PositionIndex ) )->Position ),
							VSub( ( ( MV1_MESH_POSITION * )( ( BYTE * )Frame->Position + PosUnitSize * Vertex[ 2 ]->PositionIndex ) )->Position, 
								  ( ( MV1_MESH_POSITION * )( ( BYTE * )Frame->Position + PosUnitSize * Vertex[ 0 ]->PositionIndex ) )->Position ) ) ) ;

						MNrm = &Frame->Normal[ Vertex[ 0 ]->NormalIndex ] ;
						MNrm->Normal = VAdd( MNrm->Normal, Nrm ) ;

						MNrm = &Frame->Normal[ Vertex[ 1 ]->NormalIndex ] ;
						MNrm->Normal = VAdd( MNrm->Normal, Nrm ) ;

						MNrm = &Frame->Normal[ Vertex[ 2 ]->NormalIndex ] ;
						MNrm->Normal = VAdd( MNrm->Normal, Nrm ) ;
					}
				}

				// �@���𐳋K������
				MNrm = Frame->Normal ;
				for( j = 0 ; j < Frame->NormalNum ; j ++, MNrm ++ )
				{
					MNrm->Normal = VNorm( MNrm->Normal ) ;
				}
			}
		}

		// �ڐ��Ə]�@���̍\�z
		Mesh = MBase->Mesh ;
		for( i = 0 ; i < MBase->MeshNum ; i ++, Mesh ++ )
		{
			MV1MakeMeshBinormalsAndTangents( Mesh ) ;
		}
	}

	// �A�j���[�V�����L�[�Z�b�g�̏����Z�b�g
	AnimKeySet = MBase->AnimKeySet ;
	F1AnimKeySet = ( MV1_ANIM_KEYSET_F1 * )( ( BYTE * )FHeader + ( DWORD_PTR )FHeader->AnimKeySet ) ;
	for( i = 0 ; i < MBase->AnimKeySetNum ; i ++, AnimKeySet ++, F1AnimKeySet = ( MV1_ANIM_KEYSET_F1 * )( ( DWORD_PTR )F1AnimKeySet + FHeader->AnimKeySetUnitSize ) )
	{
		MV1_ANIM_KEY_16BIT_F KeySubB16F ;
		float *Float ;
		VECTOR *Vector ;
		FLOAT4 *Quaternion ;
		BYTE *Src ;

		AnimKeySet->Type = F1AnimKeySet->Type ;
		AnimKeySet->DataType = F1AnimKeySet->DataType ;

		Src = ( BYTE * )FHeader + ( DWORD_PTR )F1AnimKeySet->KeyData ;

		if( AnimKeySet->DataType == MV1_ANIMKEY_DATATYPE_SHAPE )
		{
			AnimKeySet->TargetShapeIndex = *( ( WORD * )Src ) ;
			Src += 2 ;
		}

		if( F1AnimKeySet->Flag & MV1_ANIM_KEYSET_FLAG_KEY_ONE )
		{
			AnimKeySet->TimeType = MV1_ANIMKEY_TIME_TYPE_ONE ;
			AnimKeySet->UnitTime = 1.0f ;
			AnimKeySet->StartTime = 0.0f ;
			AnimKeySet->Num = 1 ;
		}
		else
		{
			if( F1AnimKeySet->Flag & MV1_ANIM_KEYSET_FLAG_KEYNUM_B )
			{
				AnimKeySet->Num = ( int )*Src ;
				Src ++ ;
			}
			else
			if( F1AnimKeySet->Flag & MV1_ANIM_KEYSET_FLAG_KEYNUM_W )
			{
				AnimKeySet->Num = ( int )*( ( WORD * )Src ) ;
				Src += 2 ;
			}
			else
			{
				AnimKeySet->Num = ( int )*( ( DWORD * )Src ) ;
				Src += 4 ;
			}

			if( F1AnimKeySet->Flag & MV1_ANIM_KEYSET_FLAG_TIME_UNIT )
			{
				AnimKeySet->TimeType = MV1_ANIMKEY_TIME_TYPE_ONE ;

				if( F1AnimKeySet->Flag & MV1_ANIM_KEYSET_FLAG_TIME_UNIT_ST_W )
				{
					AnimKeySet->StartTime = *( ( WORD * )Src ) ;
					Src += 2 ;
				}
				else
				if( F1AnimKeySet->Flag & MV1_ANIM_KEYSET_FLAG_TIME_UNIT_ST_Z )
				{
					AnimKeySet->StartTime = 0.0f ;
				}
				else
				{
					AnimKeySet->StartTime = *( ( float * )Src ) ;
					Src += 4 ;
				}

				if( F1AnimKeySet->Flag & MV1_ANIM_KEYSET_FLAG_TIME_UNIT_UN_W )
				{
					AnimKeySet->UnitTime = *( ( WORD * )Src ) ;
					Src += 2 ;
				}
				else
				{
					AnimKeySet->UnitTime = *( ( float * )Src ) ;
					Src += 4 ;
				}
			}
			else
			{
				AnimKeySet->TimeType = MV1_ANIMKEY_TIME_TYPE_KEY ;

				AnimKeySet->KeyTime = ( float * )( ( DWORD_PTR )MBase->AnimKeyData + MBase->AnimKeyDataSize ) ;
				MBase->AnimKeyDataSize += sizeof( float ) * AnimKeySet->Num ;
				if( F1AnimKeySet->Flag & MV1_ANIM_KEYSET_FLAG_TIME_BIT16 )
				{
					KeySubB16F.Min  = MV1AnimKey16BitMinBtoF(  ( ( MV1_ANIM_KEY_16BIT_F1 * )Src )->Min  ) ;
					KeySubB16F.Unit = MV1AnimKey16BitUnitBtoF( ( ( MV1_ANIM_KEY_16BIT_F1 * )Src )->Unit ) ;
					Src += 2 ;

					Float = AnimKeySet->KeyTime ;
					for( j = 0 ; j < AnimKeySet->Num ; j ++, Float ++ )
					{
						*Float = *( ( WORD * )Src ) * KeySubB16F.Unit + KeySubB16F.Min ;
						Src += 2 ;
					}
				}
				else
				{
					_MEMCPY( AnimKeySet->KeyTime, Src, sizeof( float ) * AnimKeySet->Num ) ;
					Src += sizeof( float ) * AnimKeySet->Num ;
				}
			}
		}

		AnimKeySet->KeyLinear = ( float * )( ( DWORD_PTR )MBase->AnimKeyData + MBase->AnimKeyDataSize ) ;
		if( ( F1AnimKeySet->Flag & MV1_ANIM_KEYSET_FLAG_KEY_BIT16 ) == 0 )
		{
			switch( AnimKeySet->Type )
			{
			case MV1_ANIMKEY_TYPE_LINEAR :
				_MEMCPY( AnimKeySet->KeyLinear, Src, AnimKeySet->Num * sizeof( float ) ) ;
				Src += AnimKeySet->Num * sizeof( float ) ;
				MBase->AnimKeyDataSize += sizeof( float ) * AnimKeySet->Num ;
				break ;

			case MV1_ANIMKEY_TYPE_VECTOR :
				_MEMCPY( AnimKeySet->KeyVector, Src, AnimKeySet->Num * sizeof( VECTOR ) ) ;
				Src += AnimKeySet->Num * sizeof( VECTOR ) ;
				MBase->AnimKeyDataSize += sizeof( VECTOR ) * AnimKeySet->Num ;
				break ;

			case MV1_ANIMKEY_TYPE_QUATERNION_X :
			case MV1_ANIMKEY_TYPE_QUATERNION_VMD :
				_MEMCPY( AnimKeySet->KeyFloat4, Src, AnimKeySet->Num * sizeof( FLOAT4 ) ) ;
				Src += AnimKeySet->Num * sizeof( FLOAT4 ) ;
				MBase->AnimKeyDataSize += sizeof( FLOAT4 ) * AnimKeySet->Num ;
				break ;

			case MV1_ANIMKEY_TYPE_MATRIX4X4C :
				_MEMCPY( AnimKeySet->KeyMatrix4x4C, Src, AnimKeySet->Num * sizeof( MATRIX_4X4CT_F ) ) ;
				Src += AnimKeySet->Num * sizeof( MATRIX_4X4CT_F ) ;
				MBase->AnimKeyDataSize += sizeof( MATRIX_4X4CT_F ) * AnimKeySet->Num ;
				break ;
			}
		}
		else
		{
			if( ( F1AnimKeySet->Flag & MV1_ANIM_KEYSET_FLAG_KEY_MP_PP ) == 0 &&
				( F1AnimKeySet->Flag & MV1_ANIM_KEYSET_FLAG_KEY_Z_TP ) == 0 )
			{
				KeySubB16F.Min  = MV1AnimKey16BitMinBtoF(  ( ( MV1_ANIM_KEY_16BIT_F1 * )Src )->Min  ) ;
				KeySubB16F.Unit = MV1AnimKey16BitUnitBtoF( ( ( MV1_ANIM_KEY_16BIT_F1 * )Src )->Unit ) ;
				Src += 2 ;
			}

			switch( AnimKeySet->Type )
			{
			case MV1_ANIMKEY_TYPE_LINEAR :
				Float = AnimKeySet->KeyLinear ;
				if( F1AnimKeySet->Flag & MV1_ANIM_KEYSET_FLAG_KEY_MP_PP )
				{
					for( k = 0 ; k < AnimKeySet->Num ; k ++, Float ++ )
					{
						*Float  = *( ( WORD * )Src ) * ( float )( DX_PI * 2.0f ) / 65535.0f - ( float )DX_PI ;
						Src += 2 ;
					}
				}
				else
				if( F1AnimKeySet->Flag & MV1_ANIM_KEYSET_FLAG_KEY_Z_TP )
				{
					for( k = 0 ; k < AnimKeySet->Num ; k ++, Float ++ )
					{
						*Float  = *( ( WORD * )Src ) * ( float )( DX_PI * 2.0f ) / 65535.0f ;
						Src += 2 ;
					}
				}
				else
				{
					for( k = 0 ; k < AnimKeySet->Num ; k ++, Float ++ )
					{
						*Float  = *( ( WORD * )Src ) * KeySubB16F.Unit + KeySubB16F.Min ;
						Src += 2 ;
					}
				}
				MBase->AnimKeyDataSize += sizeof( float ) * AnimKeySet->Num ;
				break ;

			case MV1_ANIMKEY_TYPE_VECTOR :
				Vector = AnimKeySet->KeyVector ;
				if( F1AnimKeySet->Flag & MV1_ANIM_KEYSET_FLAG_KEY_MP_PP )
				{
					for( k = 0 ; k < AnimKeySet->Num ; k ++, Vector ++ )
					{
						Vector->x  = *( ( WORD * )Src ) * ( float )( DX_PI * 2.0f ) / 65535.0f - ( float )DX_PI ;
						Src += 2 ;
						Vector->y  = *( ( WORD * )Src ) * ( float )( DX_PI * 2.0f ) / 65535.0f - ( float )DX_PI ;
						Src += 2 ;
						Vector->z  = *( ( WORD * )Src ) * ( float )( DX_PI * 2.0f ) / 65535.0f - ( float )DX_PI ;
						Src += 2 ;
					}
				}
				else
				if( F1AnimKeySet->Flag & MV1_ANIM_KEYSET_FLAG_KEY_Z_TP )
				{
					for( k = 0 ; k < AnimKeySet->Num ; k ++, Vector ++ )
					{
						Vector->x  = *( ( WORD * )Src ) * ( float )( DX_PI * 2.0f ) / 65535.0f ;
						Src += 2 ;
						Vector->y  = *( ( WORD * )Src ) * ( float )( DX_PI * 2.0f ) / 65535.0f ;
						Src += 2 ;
						Vector->z  = *( ( WORD * )Src ) * ( float )( DX_PI * 2.0f ) / 65535.0f ;
						Src += 2 ;
					}
				}
				else
				{
					for( k = 0 ; k < AnimKeySet->Num ; k ++, Vector ++ )
					{
						Vector->x  = *( ( WORD * )Src ) * KeySubB16F.Unit + KeySubB16F.Min ;
						Src += 2 ;
						Vector->y  = *( ( WORD * )Src ) * KeySubB16F.Unit + KeySubB16F.Min ;
						Src += 2 ;
						Vector->z  = *( ( WORD * )Src ) * KeySubB16F.Unit + KeySubB16F.Min ;
						Src += 2 ;
					}
				}
				MBase->AnimKeyDataSize += sizeof( VECTOR ) * AnimKeySet->Num ;
				break ;

			case MV1_ANIMKEY_TYPE_QUATERNION_X :
			case MV1_ANIMKEY_TYPE_QUATERNION_VMD :
				Quaternion = AnimKeySet->KeyFloat4 ;
				for( k = 0 ; k < AnimKeySet->Num ; k ++, Quaternion ++ )
				{
					Quaternion->x  = *( ( WORD * )Src ) * KeySubB16F.Unit + KeySubB16F.Min ;
					Src += 2 ;
					Quaternion->y  = *( ( WORD * )Src ) * KeySubB16F.Unit + KeySubB16F.Min ;
					Src += 2 ;
					Quaternion->z  = *( ( WORD * )Src ) * KeySubB16F.Unit + KeySubB16F.Min ;
					Src += 2 ;
					Quaternion->w  = *( ( WORD * )Src ) * KeySubB16F.Unit + KeySubB16F.Min ;
					Src += 2 ;
				}
				MBase->AnimKeyDataSize += sizeof( FLOAT4 ) * AnimKeySet->Num ;
				break ;
			}
		}

		if( LoadParam->GParam.LoadModelToIgnoreScaling &&
			( AnimKeySet->DataType == MV1_ANIMKEY_DATATYPE_SCALE ||
			  AnimKeySet->DataType == MV1_ANIMKEY_DATATYPE_SCALE_X ||
			  AnimKeySet->DataType == MV1_ANIMKEY_DATATYPE_SCALE_Y ||
			  AnimKeySet->DataType == MV1_ANIMKEY_DATATYPE_SCALE_Z  ) )
		{
			AnimKeySet->Num = 0 ;
		}
	}

	// �A�j���[�V�����Z�b�g�̏����Z�b�g
	AnimSet = MBase->AnimSet ;
	F1AnimSet = ( MV1_ANIMSET_F1 * )( ( BYTE * )FHeader + ( DWORD_PTR )FHeader->AnimSet ) ;
	for( i = 0 ; i < MBase->AnimSetNum ; i ++, AnimSet ++, F1AnimSet = ( MV1_ANIMSET_F1 * )( ( DWORD_PTR )FHeader + ( DWORD_PTR )F1AnimSet->DimNext ) )
	{
		MV1LoadModelToMV1_GetString( FHeader, MBase, FileStringBuffer, F1AnimSet->Name,
#ifndef UNICODE
			&AnimSet->NameA,
#endif
			&AnimSet->NameW
		) ;

		AnimSet->Index = F1AnimSet->Index ;
		AnimSet->MaxTime = F1AnimSet->MaxTime ;
		AnimSet->AnimNum = F1AnimSet->AnimNum ;
		if( F1AnimSet->Anim )
		{
			AnimSet->Anim = MBase->Anim + ( ( MV1_ANIM_F1 * )( ( DWORD_PTR )FHeader + ( DWORD_PTR )F1AnimSet->Anim ) )->Index ;
		}
		if( F1AnimSet->Flag & 1 ) AnimSet->IsAddAnim = 1 ;
		if( F1AnimSet->Flag & 2 ) AnimSet->IsMatrixLinearBlend = 1 ;
		if( F1AnimSet->Flag & 4 ) AnimSet->IsLoopAnim = 1 ;
	}

	// �A�j���[�V�����̏����Z�b�g
	Anim = MBase->Anim ;
	F1Anim = ( MV1_ANIM_F1 * )( ( BYTE * )FHeader + ( DWORD_PTR )FHeader->Anim ) ;
	for( i = 0 ; i < MBase->AnimNum ; i ++, Anim ++, F1Anim = ( MV1_ANIM_F1 * )( ( BYTE * )F1Anim + FHeader->AnimUnitSize ) )
	{
		if( F1Anim->Container )
		{
			Anim->Container = MBase->AnimSet + ( ( MV1_ANIMSET_F1 * )( ( DWORD_PTR )FHeader + ( DWORD_PTR )F1Anim->Container ) )->Index ;
		}

		Anim->TargetFrame = MBase->Frame + F1Anim->TargetFrameIndex ;
		Anim->TargetFrameIndex = F1Anim->TargetFrameIndex ;
		Anim->MaxTime = F1Anim->MaxTime ;
		Anim->RotateOrder = F1Anim->RotateOrder ;
		Anim->KeySetNum = F1Anim->KeySetNum ;
		Anim->KeySet = MBase->AnimKeySet + ( ( MV1_ANIM_KEYSET_F1 * )( DWORD_PTR )F1Anim->KeySet - ( MV1_ANIM_KEYSET_F1 * )( DWORD_PTR )FHeader->AnimKeySet ) ;

		MBase->AnimTargetFrameTable[ Anim->Container->Index * MBase->FrameNum + Anim->TargetFrameIndex ] = Anim ;
	}

	// �s��̃Z�b�g�A�b�v
	MV1SetupInitializeMatrixBase( MBase ) ;

	// ���b�V���̔��������ǂ����̏����Z�b�g�A�b�v����
	MV1SetupMeshSemiTransStateBase( MBase ) ;

	// ���������`��֌W�̏����Z�b�g�A�b�v����
	if( MBase->UsePackDraw )
	{
		MV1SetupPackDrawInfo( MBase ) ;
	}

	// �������̉��
	DXFREE( FHeader ) ;

	// �w�肪����ꍇ�͍��W�̍œK�����s��
	if( LoadParam->GParam.LoadModelToPositionOptimize )
	{
		MV1PositionOptimizeBase( NewHandle ) ;
	}

	// �w�肪����ꍇ�͖@���̍Čv�Z���s��
	if( LoadParam->GParam.LoadModelToReMakeNormal )
	{
		MV1ReMakeNormalBase( NewHandle, LoadParam->GParam.LoadModelToReMakeNormalSmoothingAngle, ASyncThread ) ;
	}

	// ���������p���_�f�[�^�̍\�z
	TriangleList = MBase->TriangleList ;
	for( i = 0 ; i < MBase->TriangleListNum ; i ++, TriangleList ++ )
	{
		MV1SetupTriangleListPositionAndNormal( TriangleList ) ;
		MV1SetupToonOutLineTriangleList( TriangleList ) ;
	}

#ifndef DX_NON_ASYNCLOAD
	if( ASyncThread )
	{
		DecASyncLoadCount( NewHandle ) ;
	}
#endif // DX_NON_ASYNCLOAD

	// ����I��
	return NewHandle ;

	// �G���[����
ERRORLABEL :
	if( NewHandle )
	{
#ifndef DX_NON_ASYNCLOAD
		if( ASyncThread )
		{
			DecASyncLoadCount( NewHandle ) ;
		}
#endif // DX_NON_ASYNCLOAD

		MV1SubModelBase( NewHandle ) ;
		NewHandle = 0 ;
	}

	// �������̉��
	DXFREE( FHeader ) ;

	// �G���[�I��
	return -1 ;
}

// ���_�o�b�t�@�̃Z�b�g�A�b�v������( -1:�G���[ )
extern int MV1SetupVertexBufferAll( int ASyncThread )
{
	int i ;
	MV1_MODEL_BASE *MBase ;

	if( HandleManageArray[ DX_HANDLETYPE_MODEL_BASE ].InitializeFlag == FALSE )
		return -1 ;

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �L���Ȃ��ׂẴ��f����f�[�^�̒��_�o�b�t�@���Z�b�g�A�b�v����
	for( i = HandleManageArray[ DX_HANDLETYPE_MODEL_BASE ].AreaMin ; i <= HandleManageArray[ DX_HANDLETYPE_MODEL_BASE ].AreaMax ; i ++ )
	{
		MBase = ( MV1_MODEL_BASE * )HandleManageArray[ DX_HANDLETYPE_MODEL_BASE ].Handle[ i ] ;
		if( MBase == NULL ) continue ;

		MV1_SetupVertexBufferBase_PF( MBase->HandleInfo.Handle, 1, ASyncThread ) ;
	}

	// �I��
	return 0 ;
}

// �S�Ă̒��_�o�b�t�@�̌�n��������( -1:�G���[ )
extern int MV1TerminateVertexBufferAll( void )
{
	int i ;
	MV1_MODEL_BASE *MBase ;
	MV1_MODEL *Model ;

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	if( HandleManageArray[ DX_HANDLETYPE_MODEL_BASE ].InitializeFlag )
	{
		// �L���Ȃ��ׂẴ��f����f�[�^�̒��_�o�b�t�@���������
		for( i = HandleManageArray[ DX_HANDLETYPE_MODEL_BASE ].AreaMin ; i <= HandleManageArray[ DX_HANDLETYPE_MODEL_BASE ].AreaMax ; i ++ )
		{
			MBase = ( MV1_MODEL_BASE * )HandleManageArray[ DX_HANDLETYPE_MODEL_BASE ].Handle[ i ] ;
			if( MBase == NULL ) continue ;

#ifndef DX_NON_ASYNCLOAD
			WaitASyncLoad( MBase->HandleInfo.Handle ) ;
#endif

			MV1_TerminateVertexBufferBase_PF( MBase->HandleInfo.Handle ) ;
		}
	}

	if( HandleManageArray[ DX_HANDLETYPE_MODEL ].InitializeFlag )
	{
		// �L���ȑS�Ẵ��f���f�[�^�̃V�F�C�v���_�o�b�t�@���J������
		for( i = HandleManageArray[ DX_HANDLETYPE_MODEL ].AreaMin ; i <= HandleManageArray[ DX_HANDLETYPE_MODEL ].AreaMax ; i ++ )
		{
			Model = ( MV1_MODEL * )HandleManageArray[ DX_HANDLETYPE_MODEL ].Handle[ i ] ;
			if( Model == NULL ) continue ;

#ifndef DX_NON_ASYNCLOAD
			WaitASyncLoad( Model->HandleInfo.Handle ) ;
#endif

			MV1_TerminateVertexBuffer_PF( Model->HandleInfo.Handle ) ;
		}
	}

	// �I��
	return 0 ;
}

// ���[�h�������f���̃t�@�C�������擾����
extern	const wchar_t *MV1GetModelFileName( int MHandle )
{
	MV1_MODEL *Model ;

	// ����������Ă��Ȃ�������G���[
	if( MV1Man.Initialize == false ) return NULL ;

	// �A�h���X�擾
	if( MV1MDLCHK( MHandle, Model ) )
		return NULL ;

	// �A�h���X��Ԃ�
	return Model->BaseData->FilePath ;
}

// ���[�h�������f�������݂���f�B���N�g���p�X���擾����( ���[�� / �� \ ���t���Ă��܂� )
extern	const wchar_t *MV1GetModelDirectoryPath( int MHandle )
{
	MV1_MODEL *Model ;

	// ����������Ă��Ȃ�������G���[
	if( MV1Man.Initialize == false ) return NULL ;

	// �A�h���X�擾
	if( MV1MDLCHK( MHandle, Model ) )
		return NULL ;

	// �A�h���X��Ԃ�
	return Model->BaseData->DirectoryPath ;
}

// �w��̃��f���Ɠ������f����{�f�[�^���g�p���ă��f�����쐬����( -1:�G���[  0�ȏ�:���f���n���h�� )
extern int NS_MV1DuplicateModel( int SrcMHandle )
{
	MV1_MODEL *Model ;
	int NewHandle ;
	int Result ;

	// ����������Ă��Ȃ�������G���[
	if( MV1Man.Initialize == false ) return -1 ;

	// �A�h���X�擾
	if( MV1MDLCHK( SrcMHandle, Model ) )
		return -1 ;

	// ���f���f�[�^���쐬����
	NewHandle = MV1AddModel( FALSE ) ;
	if( NewHandle == -1 )
	{
		return -1 ;
	}

	// �\�z
	Result = MV1MakeModel( NewHandle, Model->BaseDataHandle ) ;
	if( Result < 0 )
	{
		MV1SubModel( NewHandle ) ;
		return -1 ;
	}

	// ����I��
	return NewHandle ;
}

// �w��̃��f���ƑS�������������ʂ����كf�[�^�n���h�����쐬����( -1:�G���[  0�ȏ�:���f���n���h�� )
extern int NS_MV1CreateCloneModel( int SrcMHandle )
{
	MV1_MODEL *Model ;
	int NewHandle = -1 ;
	int NewBaseHandle = -1 ;

	// ����������Ă��Ȃ�������G���[
	if( MV1Man.Initialize == false ) return -1 ;

	// �A�h���X�擾
	if( MV1MDLCHK( SrcMHandle, Model ) )
		return -1 ;

	// ��{�f�[�^�𕡐�����
	NewBaseHandle = MV1CreateCloneModelBase( Model->BaseDataHandle ) ;
	if( NewBaseHandle == -1 )
	{
		goto ERR ;
	}

	// ���_�o�b�t�@�̃Z�b�g�A�b�v���s��
	MV1_SetupVertexBufferBase_PF( NewBaseHandle ) ;

	// ���f���f�[�^���쐬����
	NewHandle = MV1AddModel( FALSE ) ;
	if( NewHandle == -1 )
	{
		goto ERR;
	}

	// �\�z
	if( MV1MakeModel( NewHandle, NewBaseHandle ) < 0 )
	{
		goto ERR ;
	}

	// ����I��
	return NewHandle ;

	// �G���[
ERR:
	if( NewHandle != -1 )
	{
		MV1SubModel( NewHandle ) ;
		NewHandle = -1 ;
	}

	if( NewBaseHandle != -1 )
	{
		MV1SubModelBase( NewBaseHandle ) ;
		NewBaseHandle = -1 ;
	}

	// �G���[�I��
	return -1 ;
}


// ���f����ǂݍ��ލۂɖ@���̍Čv�Z���s�����ǂ�����ݒ肷��( TRUE:�s��  FALSE:�s��Ȃ� )
extern int NS_MV1SetLoadModelReMakeNormal( int Flag )
{
	MV1Man.LoadModelToReMakeNormal = Flag ;

	// ����I��
	return 0 ;
}

// ���f����ǂݍ��ލۂɍs���@��̍Čv�Z�Ŏg�p����X���[�W���O�p�x��ݒ肷��
extern int NS_MV1SetLoadModelReMakeNormalSmoothingAngle( float SmoothingAngle )
{
	MV1Man.LoadModelToReMakeNormalSmoothingAngle = SmoothingAngle ;

	// ����I��
	return 0 ;
}

// ���f����ǂݍ��ލۂɃX�P�[�����O�f�[�^�𖳎����邩�ǂ�����ݒ肷��( TRUE:��������  FALSE:�������Ȃ�( �f�t�H���g ) )
extern int NS_MV1SetLoadModelIgnoreScaling( int Flag )
{
	MV1Man.LoadModelToIgnoreScaling = Flag ;

	// ����I��
	return 0 ;
}

// ���f����ǂݍ��ލۂɍ��W�f�[�^�̍œK�����s�����ǂ�����ݒ肷��( TRUE:�s��  FALSE:�s��Ȃ� )�A( MV1�t�@�C���̓ǂݍ��݂ł͂��̊֐��̐ݒ�͖�������A�|���S���̖��ߍ��݂͎��s����܂��� )
extern int NS_MV1SetLoadModelPositionOptimize( int Flag )
{
	MV1Man.LoadModelToPositionOptimize = Flag ;

	// ����I��
	return 0 ;
}

// ���f����ǂݍ��ލۂɃ|���S���̕ӂ��ڂ��Ă��āA���@���̕������قȂ�ӂɖʐςO�̃|���S���𖄂ߍ��ނ��ǂ�����ݒ肷��( TRUE:���ߍ��ށ@FALSE:���ߍ��܂Ȃ�( �f�t�H���g ) )
extern int NS_MV1SetLoadModelNotEqNormalSide_AddZeroAreaPolygon( int Flag )
{
	MV1Man.LoadModelToNotEqNormalSideAddZeroAreaPolygon = Flag ;

	// ����I��
	return 0 ;
}

// �ǂݍ��ރ��f���̕������Z���[�h��ݒ肷��
extern int NS_MV1SetLoadModelUsePhysicsMode( int PhysicsMode /* DX_LOADMODEL_PHYSICS_LOADCALC �� */ )
{
	MV1Man.LoadModelToUsePhysicsMode = PhysicsMode ;

	// ����I��
	return 0 ;
}

// �ǂݍ��ރ��f���̕������Z�ɓK�p����d�̓p�����[�^
extern int NS_MV1SetLoadModelPhysicsWorldGravity( float Gravity )
{
	MV1Man.LoadModelToWorldGravity = Gravity ;
	MV1Man.LoadModelToWorldGravityInitialize = TRUE ;

	// ����I��
	return 0 ;
}

// �ǂݍ��ރ��f���̕������Z���[�h�����O�v�Z( DX_LOADMODEL_PHYSICS_LOADCALC )�������ꍇ�ɓK�p����镨�����Z�̎��Ԑi�s�̐��x��ݒ肷��( 0:60FPS  1:120FPS  2:240FPS  3:480FPS  4:960FPS  5:1920FPS )
extern int NS_MV1SetLoadModelPhysicsCalcPrecision( int Precision )
{
	if( Precision < 0 )
	{
		return -1 ;
	}

	// �ꉞ 1920FPS�ȏ�͎w��ł��Ȃ��悤�ɂ���( 4:960FPS  5:1920FPS )
	if( Precision > 5 )
	{
		Precision = 5 ;
	}

	MV1Man.LoadModelToPhysicsCalcPrecision = Precision ;

	// ����I��
	return 0 ;
}

// �ǂݍ��ރ��f���̕������Z���[�h�����O�v�Z( DX_LOADMODEL_PHYSICS_LOADCALC )�������ꍇ�ɓK�p�����d�͂̐ݒ������
extern int NS_MV1SetLoadCalcPhysicsWorldGravity( int GravityNo, VECTOR Gravity )
{
	if( GravityNo < 0 || GravityNo > 255 )
		return -1 ;

	MV1Man.LoadCalcPhysicsWorldGravity[ GravityNo ] = Gravity ;

	// ����I��
	return 0 ;
}

// �ǂݍ��ރ��f���̕������Z�����̍��̂̂ݖ����ɂ��邽�߂̖��O�̃��[�h��ǉ�����A�ǉ��ł��郏�[�h�̐��͍ő� 256 ��( DisablePhysicsFileOnly = TRUE �̏ꍇ�́A�������Z�������� vmd �t�@�C���ɑ΂��Ă̂ݖ������[�h�𔽉f���� )
extern int NS_MV1AddLoadModelDisablePhysicsNameWord( const TCHAR *Word )
{
#ifdef UNICODE
	return MV1AddLoadModelDisablePhysicsNameWord_WCHAR_T( Word ) ;
#else
	int Result ;

	TCHAR_TO_WCHAR_T_STRING_ONE_BEGIN( Word, return -1 )

	Result = MV1AddLoadModelDisablePhysicsNameWord_WCHAR_T( UseWordBuffer ) ;

	TCHAR_TO_WCHAR_T_STRING_END( Word )

	return Result ;
#endif
}

// �ǂݍ��ރ��f���̕������Z�����̍��̂̂ݖ����ɂ��邽�߂̖��O�̃��[�h��ǉ�����A�ǉ��ł��郏�[�h�̐��͍ő� 256 ��( DisablePhysicsFileOnly = TRUE �̏ꍇ�́A�������Z�������� vmd �t�@�C���ɑ΂��Ă̂ݖ������[�h�𔽉f���� )
extern int MV1AddLoadModelDisablePhysicsNameWord_WCHAR_T( const wchar_t *NameWord )
{
	int Length ;
	char NameWord_ShiftJIS[ MV1_DISABLEPHYSICS_WORD_MAXLENGTH * 4 ] ;
	int Length_ShiftJIS ;

	// �����ȕ�����̏ꍇ�̓G���[
	if( NameWord == NULL || *NameWord == L'\0' )
	{
		return -1 ;
	}

	// ���łɃ��[�h�̐�����t�̏ꍇ���G���[
	if( MV1Man.LoadModelToDisablePhysicsNameWordNum >= MV1_DISABLEPHYSICS_WORD_MAXNUM )
	{
		return -1 ;
	}

	// ���[�h�̒������ő咷�𒴂��Ă���ꍇ���G���[
	Length = _WCSLEN( NameWord ) ;
	if( Length >= MV1_DISABLEPHYSICS_WORD_MAXLENGTH )
	{
		return -1 ;
	}
	ConvString( ( char * )NameWord, WCHAR_T_CHARCODEFORMAT, NameWord_ShiftJIS, sizeof( NameWord_ShiftJIS ), DX_CHARCODEFORMAT_SHIFTJIS ) ;
	Length_ShiftJIS = CL_strlen( DX_CHARCODEFORMAT_SHIFTJIS, NameWord_ShiftJIS ) ;
	if( Length_ShiftJIS >= MV1_DISABLEPHYSICS_WORD_MAXLENGTH )
	{
		return -1 ;
	}

	// ���[�h��ۑ�
	_MEMSET( MV1Man.LoadModelToDisablePhysicsNameWord[ MV1Man.LoadModelToDisablePhysicsNameWordNum ], 0, sizeof( MV1Man.LoadModelToDisablePhysicsNameWord[ MV1Man.LoadModelToDisablePhysicsNameWordNum ] ) ) ;
	_MEMCPY( MV1Man.LoadModelToDisablePhysicsNameWord[ MV1Man.LoadModelToDisablePhysicsNameWordNum ], NameWord, sizeof( wchar_t ) * Length ) ;

	_MEMSET( MV1Man.LoadModelToDisablePhysicsNameWord_ShiftJIS[ MV1Man.LoadModelToDisablePhysicsNameWordNum ], 0, sizeof( MV1Man.LoadModelToDisablePhysicsNameWord_ShiftJIS[ MV1Man.LoadModelToDisablePhysicsNameWordNum ] ) ) ;
	_MEMCPY( MV1Man.LoadModelToDisablePhysicsNameWord_ShiftJIS[ MV1Man.LoadModelToDisablePhysicsNameWordNum ], NameWord_ShiftJIS, Length_ShiftJIS ) ;

	// ���[�h�̐��𑝂₷
	MV1Man.LoadModelToDisablePhysicsNameWordNum ++ ;

	// ����I��
	return 0 ;
}

// MV1AddLoadModelDisablePhysicsNameWord �Œǉ��������̖̂������[�h�����Z�b�g���Ė������[�h�����̏�����Ԃɖ߂�
extern int NS_MV1ResetLoadModelDisablePhysicsNameWord( void )
{
	// �������[�h�̐��� 0 �ɂ���
	MV1Man.LoadModelToDisablePhysicsNameWordNum = 0 ;

	// ����I��
	return 0 ;
}

// MV1AddLoadModelDisablePhysicsNameWord �Œǉ��������̖̂������[�h�̓K�p���[����ύX����
extern int NS_MV1SetLoadModelDisablePhysicsNameWordMode( int DisableNameWordMode /* DX_LOADMODEL_PHYSICS_DISABLENAMEWORD_ALWAYS �� */ )
{
	// �l�̗L�����`�F�b�N
	if( DisableNameWordMode < 0 || DisableNameWordMode >= DX_LOADMODEL_PHYSICS_DISABLENAMEWORD_NUM )
	{
		return -1 ;
	}

	// ���[����ۑ�
	MV1Man.LoadModelToDisablePhysicsNameWordMode = DisableNameWordMode ;

	// ����I��
	return 0 ;
}

// �ǂݍ��ރ��f���ɓK�p����A�j���[�V�����t�@�C���̃p�X��ݒ肷��ANULL��n���Ɛݒ胊�Z�b�g( ���݂� PMD,PMX �݂̂Ɍ��ʂ��� )
extern int NS_MV1SetLoadModelAnimFilePath( const TCHAR *FileName )
{
#ifdef UNICODE
	return MV1SetLoadModelAnimFilePath_WCHAR_T(
		FileName
	) ;
#else
	int Result ;

	TCHAR_TO_WCHAR_T_STRING_ONE_BEGIN( FileName, return -1 )

	Result = MV1SetLoadModelAnimFilePath_WCHAR_T(
		UseFileNameBuffer
	) ;

	TCHAR_TO_WCHAR_T_STRING_END( FileName )

	return Result ;
#endif
}

// �ǂݍ��ރ��f���ɓK�p����A�j���[�V�����t�@�C���̃p�X��ݒ肷��ANULL��n���Ɛݒ胊�Z�b�g( ���݂� PMD,PMX �݂̂Ɍ��ʂ��� )
extern int MV1SetLoadModelAnimFilePath_WCHAR_T( const wchar_t *FileName )
{
	if( FileName == NULL )
	{
		MV1Man.AnimFilePathValid = FALSE ;
	}
	else
	{
		MV1Man.AnimFilePathValid = TRUE ;

		// �t�@�C���̃f�B���N�g���p�X�ƃt�@�C�������擾����
		_MV1CreateFileNameAndCurrentDirectory( FileName, MV1Man.AnimFileName, sizeof( MV1Man.AnimFileName ), MV1Man.AnimFileDirPath, sizeof( MV1Man.AnimFileDirPath ) ) ;
	}

	// �I��
	return 0 ;
}

// �ǂݍ��ރ��f���𓯎������`��ɑΉ������邩�ǂ�����ݒ肷��( TRUE:�Ή�������  FALSE:�Ή������Ȃ�( �f�t�H���g ) )�A( �u�Ή�������v�ɂ���ƕ`�悪�����ɂȂ�\�����������ɏ���VRAM�������܂� )
extern int NS_MV1SetLoadModelUsePackDraw( int Flag )
{
	MV1Man.LoadModelToUsePackDraw = Flag ;

	// �I��
	return 0 ;
}

// �ǂݍ��ރ��f���̂ЂƂ̃g���C�A���O�����X�g�Ŏg�p�ł���ő�{�[������ݒ肷��( UseMaxBoneNum �Ŏw��ł���l�͈̔͂� 8 �` 54�A 0 ���w�肷��ƃf�t�H���g����ɖ߂� )
extern int NS_MV1SetLoadModelTriangleListUseMaxBoneNum( int UseMaxBoneNum )
{
	// �s���Ȓl�̏ꍇ�̓G���[
	if( UseMaxBoneNum != 0 && ( UseMaxBoneNum < 8 || UseMaxBoneNum > MV1_TRIANGLE_LIST_USE_BONE_MAX_NUM ) )
	{
		return -1 ;
	}

	MV1Man.LoadModelToTriangleListUseMaxBoneNum = UseMaxBoneNum ;

	// �I��
	return 0 ;
}



// �w��̃p�X�Ƀ��f����ۑ�����
extern int NS_MV1SaveModelToMV1File(
	int MHandle,
	const TCHAR *FileName,
	int SaveType,
	int AnimMHandle,
	int AnimNameCheck,
	int Normal8BitFlag,
	int Position16BitFlag,
	int Weight8BitFlag,
	int Anim16BitFlag
)
{
#ifdef UNICODE
	return MV1SaveModelToMV1File_WCHAR_T(
		MHandle,
		FileName,
		SaveType,
		AnimMHandle,
		AnimNameCheck,
		Normal8BitFlag,
		Position16BitFlag,
		Weight8BitFlag,
		Anim16BitFlag
	) ;
#else
	int Result ;

	TCHAR_TO_WCHAR_T_STRING_ONE_BEGIN( FileName, return -1 )

	Result = MV1SaveModelToMV1File_WCHAR_T(
		MHandle,
		UseFileNameBuffer,
		SaveType,
		AnimMHandle,
		AnimNameCheck,
		Normal8BitFlag,
		Position16BitFlag,
		Weight8BitFlag,
		Anim16BitFlag
	) ;

	TCHAR_TO_WCHAR_T_STRING_END( FileName )

	return Result ;
#endif
}

// �w��̃p�X�Ƀ��f����ۑ�����
extern int MV1SaveModelToMV1File_WCHAR_T(
	int MHandle,
	const wchar_t *FileName,
	int SaveType,
	int AnimMHandle,
	int AnimNameCheck,
	int Normal8BitFlag,
	int Position16BitFlag,
	int Weight8BitFlag,
	int Anim16BitFlag )
{
	MV1_MODEL					*Model, *AnimModel ;
	MV1_MODEL_BASE				*ModelBase, *AnimModelBase ;
	MV1_FRAME_BASE				*Frame ;
	MV1_FRAME_F1				*F1Frame ;
	MV1_MATERIAL_BASE			*MaterialBase ;
	MV1_MATERIAL				*Material ;
	MV1_MATERIAL_F1				*F1Material ;
	MV1_MATERIAL_LAYER			*MaterialLayer ;
	MV1_MATERIAL_LAYER_F1		*F1MaterialLayer ;
	MV1_MATERIAL_TOON_F1		*F1MaterialToonAddr = NULL, *F1MaterialToon ;
	MV1_LIGHT					*Light ;
	MV1_LIGHT_F1				*F1Light ;
	MV1_TEXTURE					*Texture ;
	MV1_TEXTURE_BASE			*TextureBase ;
	MV1_TEXTURE_F1				*F1Texture ;
	MV1_MESH_BASE				*Mesh ;
	MV1_MESH_F1					*F1Mesh ;
	MV1_SKIN_BONE				*SkinBone ;
	MV1_SKIN_BONE_F1			*F1SkinBone ;
	MV1_TRIANGLE_LIST_BASE		*TriangleList ;
	MV1_TRIANGLE_LIST_F1		*F1TriangleList ;
	MV1_ANIM_KEYSET_BASE		*AnimKeySet ;
	MV1_ANIM_KEYSET_F1			*F1AnimKeySet ;
	MV1_ANIM_BASE				*Anim ;
	MV1_ANIM_F1					*F1Anim ;
	MV1_ANIMSET_BASE			*AnimSet ;
	MV1_ANIMSET_F1				*F1AnimSet ;
	MV1_FILEHEAD_SHAPE_F1		*F1FileHeadShape ;
	MV1_FRAME_SHAPE_F1			*F1FrameShape ;
	MV1_SHAPE_BASE				*Shape ;
	MV1_SHAPE_F1				*F1Shape ;
	MV1_SHAPE_MESH_BASE			*ShapeMesh ;
	MV1_SHAPE_MESH_F1			*F1ShapeMesh ;
	MV1_SHAPE_VERTEX_BASE		*ShapeVertex ;
	MV1_SHAPE_VERTEX_F1			*F1ShapeVertex ;
	MV1_FILEHEAD_PHYSICS_F1		*F1FileHeadPhysics ;
	MV1_PHYSICS_RIGIDBODY_BASE	*PhysicsRigidBody ;
	MV1_PHYSICS_RIGIDBODY_F1	*F1PhysicsRigidBody ;
	MV1_PHYSICS_JOINT_BASE		*PhysicsJoint ;
	MV1_PHYSICS_JOINT_F1		*F1PhysicsJoint ;
	DWORD_PTR FileHandle ;
	void *TempBuffer = NULL, *AnimBuffer = NULL, *VertexBuffer = NULL, *ChangeDrawMaterialTableBuffer = NULL ;
	DWORD HeaderSize, i, j, k ;
	int AttachIndex = 0 ;
	MV1MODEL_FILEHEADER_F1 *FHeader ;
	VECTOR PosMin, PosMax, PosWidth ;
	BYTE *Dest, *Start ;
	DWORD Size ;
	void *PressData ;
	DWORD PressDataSize ;
	DWORD TempBufferUseSize ;
	DWORD TempBufferSize, HeaderBufferSize, VertexBufferSize, AnimBufferSize, ChangeDrawMaterialTableBufferSize ;
	bool MeshSave, AnimSave ;
	int Err = -1 ;

	MeshSave = ( SaveType & MV1_SAVETYPE_MESH ) ? true : false ;
	AnimSave = ( SaveType & MV1_SAVETYPE_ANIM ) ? true : false ;
	F1FileHeadShape = NULL ;
	F1FileHeadPhysics = NULL ;

	// �A�h���X�擾
	if( MV1MDLCHK( MHandle, Model ) )
		return -1 ;
	ModelBase = Model->BaseData ;

	// �A�j���[�V�����p�̃��f���n���h���̃`�F�b�N
	if( AnimMHandle < 0 || MV1MDLCHK( AnimMHandle, AnimModel ) )
	{
		AnimModelBase = ModelBase ;
		AnimModel = Model ;
		AnimMHandle = -1 ;
	}
	else
	{
		AnimModelBase = AnimModel->BaseData ;
	}

	// �ő咸�_���W�ƍŏ����_���W���Z�o����
	MV1GetMaxMinPosition( MHandle, NULL, NULL ) ;

	// �w�b�_�T�C�Y�̊ȈՌv�Z
	{
		int FrameNum ;

		HeaderSize =
			sizeof( MV1MODEL_FILEHEADER_F1 ) + 128 +
			ModelBase->ChangeMatrixTableSize +
			sizeof( MV1_FRAME_F1 ) * ModelBase->FrameNum ;

		if( MeshSave )
		{
			HeaderSize +=
				sizeof( DWORD /*MV1_SKIN_BONE_F1 **/ )         * ModelBase->FrameUseSkinBoneNum +
				( sizeof( MV1_MATERIAL_F1 ) + sizeof( MV1_MATERIAL_TOON_F1 ) ) * ModelBase->MaterialNum +
				sizeof( MV1_LIGHT_F1 )               * ModelBase->LightNum +
				sizeof( MV1_TEXTURE_F1 )             * ModelBase->TextureNum +
				sizeof( MV1_MESH_F1 )                * ModelBase->MeshNum +
				sizeof( MV1_SKIN_BONE_F1 )           * ModelBase->SkinBoneNum +
				sizeof( MV1_SKIN_BONE_USE_FRAME_F1 ) * ModelBase->SkinBoneUseFrameNum +
				sizeof( MV1_TRIANGLE_LIST_F1 )       * ModelBase->TriangleListNum ;
			if( ModelBase->ShapeNum != 0 )
			{
				Frame = ModelBase->Frame ;
				FrameNum = 0 ;
				for( i = 0 ; i < ( DWORD )ModelBase->FrameNum ; i ++, Frame ++ )
				{
					if( Frame->ShapeNum != 0 )
						FrameNum ++ ;
				}

				HeaderSize +=
					sizeof( MV1_FILEHEAD_SHAPE_F1 ) +
					sizeof( MV1_FRAME_SHAPE_F1 )    * FrameNum +
					sizeof( MV1_SHAPE_F1 )          * ModelBase->ShapeNum +
					sizeof( MV1_SHAPE_MESH_F1 )     * ModelBase->ShapeMeshNum +
					sizeof( MV1_SHAPE_VERTEX_F1 )   * ModelBase->ShapeVertexNum ;
			}
		}

		if( AnimSave )
		{
			HeaderSize +=
				sizeof( MV1_ANIM_KEYSET_F1 ) * AnimModelBase->AnimKeySetNum +
				sizeof( MV1_ANIM_F1 )        * AnimModelBase->AnimNum +
				sizeof( MV1_ANIMSET_F1 )     * AnimModelBase->AnimSetNum ;
		}
	}

	// �e�o�b�t�@�̏����ő�T�C�Y���Z�b�g
	AnimBufferSize                    = AnimSave ? AnimModelBase->AnimKeyDataSize + sizeof( MV1_ANIM_KEYSET ) * AnimModelBase->AnimKeySetNum : 0 ;
	VertexBufferSize                  = MeshSave ? ModelBase->VertexDataSize * 2 : 0 ;
	ChangeDrawMaterialTableBufferSize = ( DWORD )( MeshSave ? 0 : 4 * 1024 * 1024 ) ;
	HeaderBufferSize                  = HeaderSize * 2 + 4 * 1024 * 1024 ;

SAVELOOP :
	TempBufferSize = ( HeaderBufferSize + VertexBufferSize + AnimBufferSize + ChangeDrawMaterialTableBufferSize ) * 2 ;

	if( TempBuffer )
	{
		DXFREE( TempBuffer ) ;
		TempBuffer = NULL ;
	}
	if( AnimBuffer )
	{
		DXFREE( AnimBuffer ) ;
		AnimBuffer = NULL ;
	}
	if( VertexBuffer )
	{
		DXFREE( VertexBuffer ) ;
		VertexBuffer = NULL ;
	}
	if( ChangeDrawMaterialTableBuffer )
	{
		DXFREE( ChangeDrawMaterialTableBuffer ) ;
		ChangeDrawMaterialTableBuffer = NULL ;
	}

	// �o�b�t�@���m�ۂ���
	if( TempBufferSize )
	{
		TempBuffer = DXALLOC( TempBufferSize ) ;
		if( TempBuffer == NULL ) return -1 ;
		_MEMSET( TempBuffer, 0, TempBufferSize ) ;
	}

	if( VertexBufferSize )
	{
		VertexBuffer = DXALLOC( VertexBufferSize ) ;
		if( VertexBuffer == NULL ) return -1 ;
		_MEMSET( VertexBuffer, 0, VertexBufferSize ) ;
	}

	if( AnimBufferSize )
	{
		AnimBuffer = DXALLOC( AnimBufferSize ) ;
		if( AnimBuffer == NULL ) return -1 ;
		_MEMSET( AnimBuffer, 0, AnimBufferSize ) ;
	}

	if( ChangeDrawMaterialTableBufferSize )
	{
		ChangeDrawMaterialTableBuffer = DXALLOC( ChangeDrawMaterialTableBufferSize ) ;
		if( ChangeDrawMaterialTableBuffer == NULL ) return -1 ;
		_MEMSET( ChangeDrawMaterialTableBuffer, 0, ChangeDrawMaterialTableBufferSize ) ;
	}

	// �w�b�_�̃Z�b�g
	FHeader = ( MV1MODEL_FILEHEADER_F1 * )TempBuffer ;
	TempBufferUseSize = sizeof( MV1MODEL_FILEHEADER_F1 ) + 128 ;
	FHeader->CheckID[ 0 ] = 'M' ;
	FHeader->CheckID[ 1 ] = 'V' ;
	FHeader->CheckID[ 2 ] = '1' ;
	FHeader->CheckID[ 3 ] = '1' ;
	FHeader->Version = 0 ;

	FHeader->RightHandType             = ModelBase->RightHandType ;
	FHeader->MaterialNumberOrderDraw   = ( BYTE )ModelBase->MaterialNumberOrderDraw ;
	FHeader->IsStringUTF8              = 1 ;

	if( MeshSave )
	{
		FHeader->ChangeDrawMaterialTableSize  = ModelBase->ChangeDrawMaterialTableSize ;
		FHeader->ChangeDrawMaterialTable      = TempBufferUseSize ;
		TempBufferUseSize                     += ModelBase->ChangeDrawMaterialTableSize ;
	}
	else
	{
		if( ChangeDrawMaterialTableBuffer == NULL ) goto ERRORLABEL ;
		FHeader->ChangeDrawMaterialTableSize  = 0 ;
	}

	FHeader->ChangeMatrixTableSize     = ModelBase->ChangeMatrixTableSize ;
	FHeader->ChangeMatrixTable         = TempBufferUseSize ;
	TempBufferUseSize                 += ModelBase->ChangeMatrixTableSize ;

	FHeader->FrameNum                  = ModelBase->FrameNum ;
	FHeader->Frame                     = TempBufferUseSize ;
	TempBufferUseSize                 += sizeof( MV1_FRAME_F1 ) * ModelBase->FrameNum ;

	if( MeshSave )
	{
		FHeader->FrameUseSkinBoneNum       = ModelBase->FrameUseSkinBoneNum ;
		FHeader->FrameUseSkinBone          = TempBufferUseSize ;
		TempBufferUseSize                 += sizeof( DWORD /*MV1_SKIN_BONE_F1 **/ ) * ModelBase->FrameUseSkinBoneNum ;

		FHeader->MaterialNum               = ModelBase->MaterialNum ;
		FHeader->Material                  = TempBufferUseSize ;
		TempBufferUseSize                 += sizeof( MV1_MATERIAL_F1 ) * ModelBase->MaterialNum ;
		F1MaterialToonAddr                 = ( MV1_MATERIAL_TOON_F1 * )( DWORD_PTR )TempBufferUseSize ;
		TempBufferUseSize                 += sizeof( MV1_MATERIAL_TOON_F1 ) * ModelBase->MaterialNum ;

		FHeader->LightNum                  = ModelBase->LightNum ;
		FHeader->Light                     = TempBufferUseSize ;
		TempBufferUseSize                 += sizeof( MV1_LIGHT_F1 ) * ModelBase->LightNum ;

		FHeader->TextureNum                = ModelBase->TextureNum ;
		FHeader->Texture                   = TempBufferUseSize ;
		TempBufferUseSize                 += sizeof( MV1_TEXTURE_F1 ) * ModelBase->TextureNum ;

		FHeader->MeshNum                   = ModelBase->MeshNum ;
		FHeader->Mesh                      = TempBufferUseSize ;
		TempBufferUseSize                 += sizeof( MV1_MESH_F1 ) * ModelBase->MeshNum ;

		FHeader->SkinBoneNum               = ModelBase->SkinBoneNum ;
		FHeader->SkinBone                  = TempBufferUseSize ;
		TempBufferUseSize                 += sizeof( MV1_SKIN_BONE_F1 ) * ModelBase->SkinBoneNum ;
		FHeader->SkinBoneUseFrameNum       = ModelBase->SkinBoneUseFrameNum ;
		FHeader->SkinBoneUseFrame          = TempBufferUseSize ;
		TempBufferUseSize                 += sizeof( MV1_SKIN_BONE_USE_FRAME_F1 ) * ModelBase->SkinBoneUseFrameNum ;

		FHeader->TriangleListNum           = ModelBase->TriangleListNum ;
		FHeader->TriangleList              = TempBufferUseSize ;
		TempBufferUseSize                 += sizeof( MV1_TRIANGLE_LIST_F1 ) * ModelBase->TriangleListNum ;

		FHeader->TriangleNum               = ModelBase->TriangleNum ;
		FHeader->TriangleListVertexNum     = ModelBase->TriangleListVertexNum ;

		FHeader->TriangleListNormalPositionNum     = ModelBase->TriangleListNormalPositionNum ;
		FHeader->TriangleListSkinPosition4BNum     = ModelBase->TriangleListSkinPosition4BNum ;
		FHeader->TriangleListSkinPosition8BNum     = ModelBase->TriangleListSkinPosition8BNum ;
		FHeader->TriangleListSkinPositionFREEBSize = ModelBase->TriangleListSkinPositionFREEBSize ;
		FHeader->MeshPositionSize          = ModelBase->MeshPositionSize ;
		FHeader->MeshNormalNum             = ModelBase->MeshNormalNum ;
		FHeader->MeshVertexSize            = ModelBase->MeshVertexSize ;
		FHeader->MeshFaceNum               = ModelBase->MeshFaceNum ;
		FHeader->MeshVertexIndexNum        = ModelBase->MeshVertexIndexNum ;
		FHeader->TriangleListIndexNum      = ModelBase->TriangleListIndexNum ;

		FHeader->VertexDataSize            = 0 ;
		if( VertexBuffer == NULL )  goto ERRORLABEL ;

		if( ModelBase->ShapeNum != 0 )
		{
			FHeader->Shape                 = TempBufferUseSize ;
			TempBufferUseSize             += sizeof( MV1_FILEHEAD_SHAPE_F1 ) ;

			F1FileHeadShape                = ( MV1_FILEHEAD_SHAPE_F1 * )( ( DWORD_PTR )FHeader->Shape + ( DWORD_PTR )FHeader ) ;

			// �V�F�C�v�f�[�^�̑��݂���t���[���𐔂���
			Frame = ModelBase->Frame ;
			for( i = 0 ; i < ( DWORD )ModelBase->FrameNum ; i ++, Frame ++ )
			{
				if( Frame->ShapeNum != 0 )
					F1FileHeadShape->FrameNum ++ ;
			}
			F1FileHeadShape->Frame         = TempBufferUseSize ;
			TempBufferUseSize             += sizeof( MV1_FRAME_SHAPE_F1 ) * F1FileHeadShape->FrameNum ;

			F1FileHeadShape->DataNum       = ModelBase->ShapeNum ;
			F1FileHeadShape->Data          = TempBufferUseSize ;
			TempBufferUseSize             += sizeof( MV1_SHAPE_F1 ) * ModelBase->ShapeNum ;

			F1FileHeadShape->MeshNum       = ModelBase->ShapeMeshNum ;
			F1FileHeadShape->Mesh          = TempBufferUseSize ;
			TempBufferUseSize             += sizeof( MV1_SHAPE_MESH_F1 ) * ModelBase->ShapeMeshNum ;

			F1FileHeadShape->VertexNum       = ModelBase->ShapeVertexNum ;
			F1FileHeadShape->Vertex          = TempBufferUseSize ;
			TempBufferUseSize               += sizeof( MV1_SHAPE_VERTEX_F1 ) * ModelBase->ShapeVertexNum ;
			F1FileHeadShape->VertexNum       = 0 ;

			F1FileHeadShape->PressVertexDataSize = 0 ;
			F1FileHeadShape->PressVertexData = 0 ;

			F1FileHeadShape->TargetMeshVertexNum   = ModelBase->ShapeTargetMeshVertexNum ;
			F1FileHeadShape->ShapeVertexUnitSize   = sizeof( MV1_SHAPE_VERTEX_F1 ) ;
			F1FileHeadShape->NormalPositionNum     = ModelBase->ShapeNormalPositionNum ;
			F1FileHeadShape->SkinPosition4BNum     = ModelBase->ShapeSkinPosition4BNum ;
			F1FileHeadShape->SkinPosition8BNum     = ModelBase->ShapeSkinPosition8BNum ;
			F1FileHeadShape->SkinPositionFREEBSize = ModelBase->ShapeSkinPositionFREEBSize ;
		}

		if( ModelBase->PhysicsJointNum != 0 || ModelBase->PhysicsJointNum != 0 )
		{
			FHeader->Physics               = TempBufferUseSize ;
			TempBufferUseSize             += sizeof( MV1_FILEHEAD_PHYSICS_F1 ) ;

			F1FileHeadPhysics              = ( MV1_FILEHEAD_PHYSICS_F1 * )( ( DWORD_PTR )FHeader->Physics + ( DWORD_PTR )FHeader ) ;

			F1FileHeadPhysics->WorldGravity = ModelBase->PhysicsGravity ;

			F1FileHeadPhysics->RigidBodyNum = ModelBase->PhysicsRigidBodyNum ;
			F1FileHeadPhysics->RigidBody   = TempBufferUseSize ;
			TempBufferUseSize             += sizeof( MV1_PHYSICS_RIGIDBODY_F1 ) * F1FileHeadPhysics->RigidBodyNum ;

			F1FileHeadPhysics->JointNum    = ModelBase->PhysicsJointNum ;
			F1FileHeadPhysics->Joint       = TempBufferUseSize ;
			TempBufferUseSize             += sizeof( MV1_PHYSICS_JOINT_F1 ) * F1FileHeadPhysics->JointNum ;
		}
	}

	if( AnimSave )
	{
		FHeader->OriginalAnimKeyDataSize = AnimModelBase->AnimKeyDataSize ;
		FHeader->AnimKeyDataSize         = 0 ;
		if( AnimModelBase->AnimKeyDataSize != 0 || AnimModelBase->AnimKeySetNum != 0 )
		{
			if( AnimBuffer == NULL )  goto ERRORLABEL ;
		}
		else
		{
			FHeader->AnimKeyData           = 0 ;
		}

		FHeader->AnimKeySetNum             = AnimModelBase->AnimKeySetNum ;
		FHeader->AnimKeySetUnitSize        = sizeof( MV1_ANIM_KEYSET_F1 ) ;
		FHeader->AnimKeySet                = TempBufferUseSize ;
		TempBufferUseSize                 += sizeof( MV1_ANIM_KEYSET_F1 ) * AnimModelBase->AnimKeySetNum ;

		FHeader->AnimNum                   = AnimModelBase->AnimNum ;
		FHeader->AnimUnitSize              = sizeof( MV1_ANIM_F1 ) ;
		FHeader->Anim                      = TempBufferUseSize ;
		TempBufferUseSize                 += sizeof( MV1_ANIM_F1 ) * AnimModelBase->AnimNum ;

		FHeader->AnimSetNum                = AnimModelBase->AnimSetNum ;
		FHeader->AnimSet                   = TempBufferUseSize ;
		TempBufferUseSize                 += sizeof( MV1_ANIMSET_F1 ) * AnimModelBase->AnimSetNum ;
	}

	FHeader->StringSize                = 0 ;
	FHeader->StringBuffer              = TempBufferUseSize ;

	// �O���f�[�^�̕ۑ�
	FHeader->UserData[ 0 ] = ModelBase->UserData[ 0 ] ;
	FHeader->UserData[ 1 ] = ModelBase->UserData[ 1 ] ;
	FHeader->UserData[ 2 ] = ModelBase->UserData[ 2 ] ;
	FHeader->UserData[ 3 ] = ModelBase->UserData[ 3 ] ;

	// �s��ύX���e�[�u���̕ۑ�
	_MEMCPY( ( BYTE * )FHeader + ( DWORD_PTR )FHeader->ChangeMatrixTable,       ModelBase->ChangeMatrixTable,       ( size_t )ModelBase->ChangeMatrixTableSize ) ;

	if( MeshSave )
	{
		// �}�e���A���ύX���e�[�u���̕ۑ�
		_MEMCPY( ( BYTE * )FHeader + ( DWORD_PTR )FHeader->ChangeDrawMaterialTable, ModelBase->ChangeDrawMaterialTable, ( size_t )ModelBase->ChangeDrawMaterialTableSize ) ;

		// �X�L�j���O���b�V���̃{�[�����g�p����t���[���̏���ۑ�
		_MEMCPY( ( BYTE * )FHeader + ( DWORD_PTR )FHeader->SkinBoneUseFrame, ModelBase->SkinBoneUseFrame, sizeof( MV1_SKIN_BONE_USE_FRAME_F1 ) * ModelBase->SkinBoneUseFrameNum ) ;

		// �t���[�����g�p���Ă���{�[���ւ̃|�C���^�̏���ۑ�
		for( i = 0 ; i < ( DWORD )ModelBase->FrameUseSkinBoneNum ; i ++ )
		{
			*( ( DWORD * )( ( BYTE * )FHeader + ( DWORD_PTR )FHeader->FrameUseSkinBone + sizeof( DWORD ) * i ) ) = ( DWORD )( DWORD_PTR )( ( MV1_SKIN_BONE_F1 * )( DWORD_PTR )FHeader->SkinBone + ( ModelBase->FrameUseSkinBone[ i ] - ModelBase->SkinBone ) ) ;
		}
	}

	// �t���[���̏��𖄂߂�
	FHeader->TopFrameNum   = ModelBase->TopFrameNum ;
	FHeader->FirstTopFrame = ModelBase->FirstTopFrame ? FHeader->Frame + sizeof( MV1_FRAME_F1 ) * ModelBase->FirstTopFrame->Index : 0 ;
	FHeader->LastTopFrame  = ModelBase->LastTopFrame  ? FHeader->Frame + sizeof( MV1_FRAME_F1 ) * ModelBase->LastTopFrame->Index  : 0 ;
	F1Frame = ( MV1_FRAME_F1 * )( ( DWORD_PTR )FHeader->Frame + ( DWORD_PTR )FHeader ) ;
	Frame = ModelBase->Frame ;
	if( FHeader->Shape )
	{
		F1FileHeadShape->FrameNum = 0 ;
	}
	for( i = 0 ; i < ( DWORD )ModelBase->FrameNum ; i ++, F1Frame ++, Frame ++ )
	{
		F1Frame->Name = ( DWORD )FHeader->StringSize ;
		FHeader->StringSize += ConvString( ( const char * )Frame->NameW, WCHAR_T_CHARCODEFORMAT, ( char * )( ( BYTE * )FHeader + FHeader->StringBuffer + F1Frame->Name ), BUFFERBYTES_CANCEL, DX_CHARCODEFORMAT_UTF8 ) ;
		FHeader->StringSize = ( FHeader->StringSize + 3 ) / 4 * 4 ;

		F1Frame->Index = Frame->Index ;

		if( MeshSave == false )
		{
			MV1_CHANGE_BASE ChangeInfo ;
			MV1ChangeInfoSetup( &ChangeInfo, ( BYTE * )ChangeDrawMaterialTableBuffer + FHeader->ChangeDrawMaterialTableSize, ( int )( i + 1 ), Frame->TotalChildNum + 1 ) ;
			FHeader->ChangeDrawMaterialTableSize += F1Frame->ChangeDrawMaterialInfo.Size * 4 ;
			F1Frame->ChangeDrawMaterialInfo.Target   = ChangeInfo.Target ;
			F1Frame->ChangeDrawMaterialInfo.Fill	 = ( DWORD )FHeader->ChangeDrawMaterialTableSize ;
			F1Frame->ChangeDrawMaterialInfo.Size     = ChangeInfo.Size ;
			F1Frame->ChangeDrawMaterialInfo.CheckBit = ChangeInfo.CheckBit ;
		}
		else
		{
			F1Frame->ChangeDrawMaterialInfo.Target   = Frame->ChangeDrawMaterialInfo.Target ;
			F1Frame->ChangeDrawMaterialInfo.Fill     = ( DWORD )( DWORD_PTR )( ( DWORD * )( DWORD_PTR )FHeader->ChangeDrawMaterialTable + ( Frame->ChangeDrawMaterialInfo.Fill - ModelBase->ChangeDrawMaterialTable ) ) ;
			F1Frame->ChangeDrawMaterialInfo.Size     = Frame->ChangeDrawMaterialInfo.Size ;
			F1Frame->ChangeDrawMaterialInfo.CheckBit = Frame->ChangeDrawMaterialInfo.CheckBit ;
		}

		F1Frame->ChangeMatrixInfo.Target         = Frame->ChangeMatrixInfo.Target ;
		F1Frame->ChangeMatrixInfo.Fill           = ( DWORD )( DWORD_PTR )( ( DWORD * )( DWORD_PTR )FHeader->ChangeMatrixTable       + ( Frame->ChangeMatrixInfo.Fill       - ModelBase->ChangeMatrixTable ) ) ;
		F1Frame->ChangeMatrixInfo.Size           = Frame->ChangeMatrixInfo.Size ;
		F1Frame->ChangeMatrixInfo.CheckBit       = Frame->ChangeMatrixInfo.CheckBit ;

		F1Frame->TotalChildNum = Frame->TotalChildNum ;
		F1Frame->Parent     = Frame->Parent     ? FHeader->Frame + sizeof( MV1_FRAME_F1 ) * Frame->Parent->Index     : 0 ;
		F1Frame->FirstChild = Frame->FirstChild ? FHeader->Frame + sizeof( MV1_FRAME_F1 ) * Frame->FirstChild->Index : 0 ;
		F1Frame->LastChild  = Frame->LastChild  ? FHeader->Frame + sizeof( MV1_FRAME_F1 ) * Frame->LastChild->Index  : 0 ;
		F1Frame->Prev       = Frame->Prev       ? FHeader->Frame + sizeof( MV1_FRAME_F1 ) * Frame->Prev->Index       : 0 ;
		F1Frame->Next       = Frame->Next       ? FHeader->Frame + sizeof( MV1_FRAME_F1 ) * Frame->Next->Index       : 0 ;

		F1Frame->Translate = Frame->Translate ;
		F1Frame->Scale = Frame->Scale ;
		F1Frame->Rotate = Frame->Rotate ;
		F1Frame->RotateOrder = Frame->RotateOrder ;
		F1Frame->Quaternion = Frame->Quaternion ;

		F1Frame->Flag = Frame->Flag ;
		if( Frame->IgnoreParentTransform ) F1Frame->Flag |= MV1_FRAMEFLAG_IGNOREPARENTTRANS ;

		F1Frame->UserData[ 0 ] = Frame->UserData[ 0 ] ;
		F1Frame->UserData[ 1 ] = Frame->UserData[ 1 ] ;
		F1Frame->UserData[ 2 ] = Frame->UserData[ 2 ] ;
		F1Frame->UserData[ 3 ] = Frame->UserData[ 3 ] ;

		F1Frame->SmoothingAngle = Frame->SmoothingAngle ;
		F1Frame->AutoCreateNormal = Frame->AutoCreateNormal ;

		F1Frame->PreRotate = Frame->PreRotate ;
		F1Frame->PostRotate = Frame->PostRotate ;

		if( MeshSave )
		{
			F1Frame->IsSkinMesh = Frame->IsSkinMesh ;
			F1Frame->TriangleNum = Frame->TriangleNum ;
			F1Frame->VertexNum = Frame->VertexNum ;
			F1Frame->MeshNum = Frame->MeshNum ;
			if( Frame->Mesh )
			{
				F1Frame->Mesh = ( DWORD )( DWORD_PTR )( ( MV1_MESH_F1 * )( DWORD_PTR )FHeader->Mesh + ( Frame->Mesh - ModelBase->Mesh ) ) ;
			}

			F1Frame->SkinBoneNum = Frame->SkinBoneNum ;
			if( Frame->SkinBone )
			{
				F1Frame->SkinBone = ( DWORD )( DWORD_PTR )( ( MV1_SKIN_BONE_F1 * )( DWORD_PTR )FHeader->SkinBone + ( Frame->SkinBone - ModelBase->SkinBone ) ) ;
			}

			if( Frame->Light )
			{
				F1Frame->Light = ( DWORD )( DWORD_PTR )( ( MV1_LIGHT_F1 * )( DWORD_PTR )FHeader->Light + ( Frame->Light - ModelBase->Light ) ) ;
			}

			F1Frame->UseSkinBoneNum = Frame->UseSkinBoneNum ;
			if( Frame->UseSkinBoneNum )
			{
				F1Frame->UseSkinBone = ( DWORD )( FHeader->FrameUseSkinBone + ( Frame->UseSkinBone - ModelBase->FrameUseSkinBone ) * sizeof( DWORD /* MV1_SKIN_BONE_F1* */ ) ) ;
			}

			F1Frame->TotalMeshNum = Frame->TotalMeshNum ;
			F1Frame->MaxBoneBlendNum = ( unsigned short )Frame->MaxBoneBlendNum ;
			F1Frame->PositionNum = Frame->PositionNum ;
			F1Frame->NormalNum = Frame->NormalNum ;

			// ���W�f�[�^�Ɩ@���f�[�^�̍쐬
			if( F1Frame->PositionNum != 0 )
			{
				MV1_MESH_POSITION *Pos ;
				MV1_MESH_NORMAL *Nrm ;

				// �A�h���X�̃Z�b�g
				F1Frame->PositionAndNormalData = FHeader->VertexDataSize ;
				Dest = ( BYTE * )VertexBuffer + FHeader->VertexDataSize ;
				Start = Dest ;

				// ���_�f�[�^�^�C�v�̌���
				F1Frame->VertFlag = 0 ;
//				if( F1Frame->AutoCreateNormal )
//				{
//					F1Frame->VertFlag |= MV1_FRAME_NORMAL_TYPE_NONE ;
//				}
//				else
				{
					if( Normal8BitFlag )
					{
						F1Frame->VertFlag |= MV1_FRAME_NORMAL_TYPE_S8 ;
					}
					else
					{
						F1Frame->VertFlag |= MV1_FRAME_NORMAL_TYPE_S16 ;
					}
				}
				if( Position16BitFlag )
				{
					F1Frame->VertFlag |= MV1_FRAME_VERT_FLAG_POSITION_B16 ;
				}

				// �@���̑��ɐڐ��Ə]�@�������邩���Z�b�g����
				if( Frame->Flag & MV1_FRAMEFLAG_TANGENT_BINORMAL )
				{
					F1Frame->VertFlag |= MV1_FRAME_VERT_FLAG_NOMRAL_TANGENT_BINORMAL ;
				}

				// ���_�̃E�G�C�g�l�����邩�ǂ������Z�b�g����
				if( F1Frame->UseSkinBoneNum == 0 )
				{
					F1Frame->VertFlag |= MV1_FRAME_VERT_FLAG_MATRIX_WEIGHT_NONE ;
				}
				else
				{
					// ���_�̍s��C���f�b�N�X�l���O�`�Q�T�S�ȓ��Ɏ��܂��Ă��邩���Z�b�g����
					if( F1Frame->UseSkinBoneNum > 255 )
						F1Frame->VertFlag |= MV1_FRAME_MATRIX_INDEX_TYPE_U16 << 4 ;

					// �s��E�G�C�g�l���W�r�b�g���ǂ����̎w����Z�b�g����
					if( Weight8BitFlag == 0 )
						F1Frame->VertFlag |= MV1_FRAME_MATRIX_WEIGHT_TYPE_U16 << 5 ;
				}

				// 16bit���ǂ����ŏ����𕪊�
				if( F1Frame->VertFlag & MV1_FRAME_VERT_FLAG_POSITION_B16 )
				{
					// ���W�l�w�x�y�̍ő�l�ƍŏ��l�����߂�
					PosMax.x = -1000000000000.0f ;
					PosMax.y = -1000000000000.0f ;
					PosMax.z = -1000000000000.0f ;
					PosMin.x =  1000000000000.0f ;
					PosMin.y =  1000000000000.0f ;
					PosMin.z =  1000000000000.0f ;

					Pos = Frame->Position ;
					for( j = 0 ; j < ( DWORD )F1Frame->PositionNum ; j ++, Pos = ( MV1_MESH_POSITION * )( ( BYTE * )Pos + Frame->PosUnitSize ) )
					{
						if( Pos->Position.x > PosMax.x ) PosMax.x = Pos->Position.x ;
						if( Pos->Position.x < PosMin.x ) PosMin.x = Pos->Position.x ;
						if( Pos->Position.y > PosMax.y ) PosMax.y = Pos->Position.y ;
						if( Pos->Position.y < PosMin.y ) PosMin.y = Pos->Position.y ;
						if( Pos->Position.z > PosMax.z ) PosMax.z = Pos->Position.z ;
						if( Pos->Position.z < PosMin.z ) PosMin.z = Pos->Position.z ;
					}
					PosWidth.x = PosMax.x - PosMin.x ;
					PosWidth.y = PosMax.y - PosMin.y ;
					PosWidth.z = PosMax.z - PosMin.z ;

					// �⏕�����i�[����
					( ( MV1_POSITION_16BIT_SUBINFO_F1 * )Dest )[ 0 ].Min   = PosMin.x ;
					( ( MV1_POSITION_16BIT_SUBINFO_F1 * )Dest )[ 0 ].Width = PosWidth.x ;
					( ( MV1_POSITION_16BIT_SUBINFO_F1 * )Dest )[ 1 ].Min   = PosMin.y ;
					( ( MV1_POSITION_16BIT_SUBINFO_F1 * )Dest )[ 1 ].Width = PosWidth.y ;
					( ( MV1_POSITION_16BIT_SUBINFO_F1 * )Dest )[ 2 ].Min   = PosMin.z ;
					( ( MV1_POSITION_16BIT_SUBINFO_F1 * )Dest )[ 2 ].Width = PosWidth.z ;
					Dest += 24 ;

					Pos = Frame->Position ;
					for( j = 0 ; j < ( DWORD )F1Frame->PositionNum ; j ++, Pos = ( MV1_MESH_POSITION * )( ( BYTE * )Pos + Frame->PosUnitSize ) )
					{
						( ( WORD * )Dest )[ 0 ] = ( WORD )_FTOL( ( Pos->Position.x - PosMin.x ) * 60000.0f / PosWidth.x ) ;
						( ( WORD * )Dest )[ 1 ] = ( WORD )_FTOL( ( Pos->Position.y - PosMin.y ) * 60000.0f / PosWidth.y ) ;
						( ( WORD * )Dest )[ 2 ] = ( WORD )_FTOL( ( Pos->Position.z - PosMin.z ) * 60000.0f / PosWidth.z ) ;
						Dest += 6 ;
					}
				}
				else
				{
					Pos = Frame->Position ;
					for( j = 0 ; j < ( DWORD )F1Frame->PositionNum ; j ++, Pos = ( MV1_MESH_POSITION * )( ( BYTE * )Pos + Frame->PosUnitSize ) )
					{
						*( ( VECTOR * )Dest ) = Pos->Position ;
						Dest += 12 ;
					}
				}

				// �X�L�j���O��񂪂���ꍇ�͕ۑ�
				if( ( F1Frame->VertFlag & MV1_FRAME_VERT_FLAG_MATRIX_WEIGHT_NONE ) == 0 )
				{
					Pos = Frame->Position ;
					for( j = 0 ; j < ( DWORD )F1Frame->PositionNum ; j ++, Pos = ( MV1_MESH_POSITION * )( ( BYTE * )Pos + Frame->PosUnitSize ) )
					{
						for( k = 0 ; k < F1Frame->MaxBoneBlendNum ; k ++ )
						{
							int WeightParam ;

							if( ( ( F1Frame->VertFlag & MV1_FRAME_VERT_FLAG_MATRIX_INDEX_MASK ) >> 4 ) == MV1_FRAME_MATRIX_INDEX_TYPE_U8 )
							{
								*Dest = ( BYTE )( Pos->BoneWeight[ k ].Index == -1 ? 255 : Pos->BoneWeight[ k ].Index ) ;
								Dest ++ ;
							}
							else
							{
								*( ( WORD * )Dest ) = ( WORD )( Pos->BoneWeight[ k ].Index == -1 ? 65535 : Pos->BoneWeight[ k ].Index ) ;
								Dest += 2 ;
							}
							if( Pos->BoneWeight[ k ].Index == -1 )
								break ;

							if( ( ( F1Frame->VertFlag & MV1_FRAME_VERT_FLAG_MATRIX_WEIGHT_MASK ) >> 5 ) == MV1_FRAME_MATRIX_WEIGHT_TYPE_U8 )
							{
								WeightParam = _FTOL( Pos->BoneWeight[ k ].W * 255.0f ) ;
								if( WeightParam > 255 )
								{
									*Dest = 255 ;
								}
								else
								if( WeightParam < 0 )
								{
									*Dest = 0 ;
								}
								else
								{
									*Dest = ( BYTE )WeightParam ;
								}
								Dest ++ ;
							}
							else
							{
								WeightParam = _FTOL( Pos->BoneWeight[ k ].W * 65535.0f ) ;
								if( WeightParam > 65535 )
								{
									*( ( WORD * )Dest ) = 65535 ;
								}
								else
								if( WeightParam < 0 )
								{
									*( ( WORD * )Dest ) = 0 ;
								}
								else
								{
									*( ( WORD * )Dest ) = ( WORD )WeightParam ;
								}
								Dest += 2 ;
							}
						}
					}
				}

				// �@�����̕ۑ�
				Nrm = Frame->Normal ;
				switch( F1Frame->VertFlag & MV1_FRAME_VERT_FLAG_NORMAL_TYPE_MASK )
				{
				case MV1_FRAME_NORMAL_TYPE_NONE :
					break ;

				case MV1_FRAME_NORMAL_TYPE_S8 :
					if( F1Frame->VertFlag & MV1_FRAME_VERT_FLAG_NOMRAL_TANGENT_BINORMAL )
					{
						for( j = 0 ; j < ( DWORD )Frame->NormalNum ; j ++, Nrm ++ )
						{
							( ( char * )Dest )[ 0 ] = ( char )_FTOL( Nrm->Normal.x   * 127.0f ) ;
							( ( char * )Dest )[ 1 ] = ( char )_FTOL( Nrm->Normal.y   * 127.0f ) ;
							( ( char * )Dest )[ 2 ] = ( char )_FTOL( Nrm->Normal.z   * 127.0f ) ;

							( ( char * )Dest )[ 3 ] = ( char )_FTOL( Nrm->Tangent.x  * 127.0f ) ;
							( ( char * )Dest )[ 4 ] = ( char )_FTOL( Nrm->Tangent.y  * 127.0f ) ;
							( ( char * )Dest )[ 5 ] = ( char )_FTOL( Nrm->Tangent.z  * 127.0f ) ;

							( ( char * )Dest )[ 6 ] = ( char )_FTOL( Nrm->Binormal.x * 127.0f ) ;
							( ( char * )Dest )[ 7 ] = ( char )_FTOL( Nrm->Binormal.y * 127.0f ) ;
							( ( char * )Dest )[ 8 ] = ( char )_FTOL( Nrm->Binormal.z * 127.0f ) ;
							Dest += 9 ;
						}
					}
					else
					{
						for( j = 0 ; j < ( DWORD )Frame->NormalNum ; j ++, Nrm ++ )
						{
							( ( char * )Dest )[ 0 ] = ( char )_FTOL( Nrm->Normal.x * 127.0f ) ;
							( ( char * )Dest )[ 1 ] = ( char )_FTOL( Nrm->Normal.y * 127.0f ) ;
							( ( char * )Dest )[ 2 ] = ( char )_FTOL( Nrm->Normal.z * 127.0f ) ;
							Dest += 3 ;
						}
					}
					break ;

				case MV1_FRAME_NORMAL_TYPE_S16 :
					if( F1Frame->VertFlag & MV1_FRAME_VERT_FLAG_NOMRAL_TANGENT_BINORMAL )
					{
						for( j = 0 ; j < ( DWORD )Frame->NormalNum ; j ++, Nrm ++ )
						{
							( ( WORD * )Dest )[ 0 ] = ( WORD )_FTOL( Nrm->Normal.x   * 32767.0f ) ;
							( ( WORD * )Dest )[ 1 ] = ( WORD )_FTOL( Nrm->Normal.y   * 32767.0f ) ;
							( ( WORD * )Dest )[ 2 ] = ( WORD )_FTOL( Nrm->Normal.z   * 32767.0f ) ;

							( ( WORD * )Dest )[ 3 ] = ( WORD )_FTOL( Nrm->Tangent.x  * 32767.0f ) ;
							( ( WORD * )Dest )[ 4 ] = ( WORD )_FTOL( Nrm->Tangent.y  * 32767.0f ) ;
							( ( WORD * )Dest )[ 5 ] = ( WORD )_FTOL( Nrm->Tangent.z  * 32767.0f ) ;

							( ( WORD * )Dest )[ 6 ] = ( WORD )_FTOL( Nrm->Binormal.x * 32767.0f ) ;
							( ( WORD * )Dest )[ 7 ] = ( WORD )_FTOL( Nrm->Binormal.y * 32767.0f ) ;
							( ( WORD * )Dest )[ 8 ] = ( WORD )_FTOL( Nrm->Binormal.z * 32767.0f ) ;
							Dest += 18 ;
						}
					}
					else
					{
						for( j = 0 ; j < ( DWORD )Frame->NormalNum ; j ++, Nrm ++ )
						{
							( ( WORD * )Dest )[ 0 ] = ( WORD )_FTOL( Nrm->Normal.x * 32767.0f ) ;
							( ( WORD * )Dest )[ 1 ] = ( WORD )_FTOL( Nrm->Normal.y * 32767.0f ) ;
							( ( WORD * )Dest )[ 2 ] = ( WORD )_FTOL( Nrm->Normal.z * 32767.0f ) ;
							Dest += 6 ;
						}
					}
					break ;

				case MV1_FRAME_NORMAL_TYPE_F32 :
					if( F1Frame->VertFlag & MV1_FRAME_VERT_FLAG_NOMRAL_TANGENT_BINORMAL )
					{
							( ( VECTOR * )Dest )[ 0 ] = Nrm->Normal ;
							( ( VECTOR * )Dest )[ 1 ] = Nrm->Tangent ;
							( ( VECTOR * )Dest )[ 2 ] = Nrm->Binormal ;
							Dest += 36 ;
					}
					else
					{
						for( j = 0 ; j < ( DWORD )Frame->NormalNum ; j ++, Nrm ++ )
						{
							*( ( VECTOR * )Dest ) = Nrm->Normal ;
							Dest += 12 ;
						}
					}
					break ;
				}

				Size = ( DWORD )( Dest - Start ) ;
				Size = ( Size + 3 ) / 4 * 4 ;
				FHeader->VertexDataSize += Size ;
			}

			// �V�F�C�v�f�[�^������ꍇ�͂���̕ۑ�
			if( Frame->ShapeNum )
			{
				F1Frame->FrameShape = ( DWORD )( ( DWORD_PTR )( F1FileHeadShape->Frame + sizeof( MV1_FRAME_SHAPE_F1 ) * F1FileHeadShape->FrameNum ) ) ;
				F1FileHeadShape->FrameNum ++ ;

				F1FrameShape = ( MV1_FRAME_SHAPE_F1 * )( ( DWORD_PTR )F1Frame->FrameShape + ( DWORD_PTR )FHeader ) ;
				F1FrameShape->ShapeNum = Frame->ShapeNum ;
				F1FrameShape->Shape = ( DWORD )( DWORD_PTR )( ( MV1_SHAPE_F1 * )( DWORD_PTR )F1FileHeadShape->Data + ( Frame->Shape - ModelBase->Shape ) ) ;
			}
		}

		F1Frame->DimPrev = i == 0                                ? 0 : FHeader->Frame + sizeof( MV1_FRAME_F1 ) * ( i - 1 ) ;
		F1Frame->DimNext = i == ( DWORD )ModelBase->FrameNum - 1 ? 0 : FHeader->Frame + sizeof( MV1_FRAME_F1 ) * ( i + 1 ) ;
	}

	if( MeshSave )
	{
		// �V�F�C�v�̏��𖄂߂�
		if( F1FileHeadShape )
		{
			F1Shape = ( MV1_SHAPE_F1 * )( ( DWORD_PTR )F1FileHeadShape->Data + ( DWORD_PTR )FHeader ) ;
			Shape = ModelBase->Shape ;
			for( i = 0 ; i < ( DWORD )ModelBase->ShapeNum ; i ++, Shape ++, F1Shape ++ )
			{
				F1Shape->Index = ( int )i ;

				F1Shape->Name = ( DWORD )FHeader->StringSize ;
				FHeader->StringSize += ConvString( ( const char * )Shape->NameW, WCHAR_T_CHARCODEFORMAT, ( char * )( ( BYTE * )FHeader + FHeader->StringBuffer + F1Shape->Name ), BUFFERBYTES_CANCEL, DX_CHARCODEFORMAT_UTF8 ) ;
				FHeader->StringSize = ( FHeader->StringSize + 3 ) / 4 * 4 ;

				if( Shape->Container )
				{
					F1Shape->Container = ( DWORD )( DWORD_PTR )( ( MV1_FRAME_F1 * )( DWORD_PTR )FHeader->Frame + ( Shape->Container - ModelBase->Frame ) ) ;
				}

				F1Shape->MeshNum = Shape->MeshNum ;
				if( Shape->MeshNum )
				{
					F1Shape->Mesh = ( DWORD )( DWORD_PTR )( ( MV1_SHAPE_MESH_F1 * )( DWORD_PTR )F1FileHeadShape->Mesh + ( Shape->Mesh - ModelBase->ShapeMesh ) ) ;
				}

				F1Shape->DimPrev = i == 0                                ? 0 : F1FileHeadShape->Data + sizeof( MV1_SHAPE_F1 ) * ( i - 1 ) ;
				F1Shape->DimNext = i == ( DWORD )ModelBase->ShapeNum - 1 ? 0 : F1FileHeadShape->Data + sizeof( MV1_SHAPE_F1 ) * ( i + 1 ) ;
			}

			// �V�F�C�v���b�V���̏��𖄂߂�
			F1ShapeMesh = ( MV1_SHAPE_MESH_F1 * )( ( DWORD_PTR )F1FileHeadShape->Mesh + ( DWORD_PTR )FHeader ) ;
			ShapeMesh = ModelBase->ShapeMesh ;
			for( i = 0 ; i < ( DWORD )ModelBase->ShapeMeshNum ; i ++, ShapeMesh ++, F1ShapeMesh ++ )
			{
				F1ShapeMesh->Index = ( int )i ;

				F1ShapeMesh->TargetMesh = ( DWORD )( DWORD_PTR )( ( MV1_MESH_F1 * )( DWORD_PTR )FHeader->Mesh + ( ShapeMesh->TargetMesh - ModelBase->Mesh ) ) ;
				F1ShapeMesh->IsVertexPress = 0 ;
				F1ShapeMesh->VertexPressParam = 0 ;
				F1ShapeMesh->VertexNum = ShapeMesh->VertexNum ;
				if( F1ShapeMesh->VertexNum )
				{
					F1ShapeMesh->Vertex = F1FileHeadShape->Vertex + sizeof( MV1_SHAPE_VERTEX_F1 ) * F1FileHeadShape->VertexNum ;
					F1ShapeVertex = ( MV1_SHAPE_VERTEX_F1 * )( ( BYTE * )( DWORD_PTR )F1ShapeMesh->Vertex + ( DWORD_PTR )FHeader ) ;
					ShapeVertex = ShapeMesh->Vertex ;
					for( j = 0 ; ( DWORD )j < ShapeMesh->VertexNum ; j ++, F1ShapeVertex ++, ShapeVertex ++ )
					{
						F1ShapeVertex->TargetMeshVertex = ShapeVertex->TargetMeshVertex ;
						F1ShapeVertex->Position = ShapeVertex->Position ;
						F1ShapeVertex->Normal = ShapeVertex->Normal ;
					}
					F1FileHeadShape->VertexNum += ShapeMesh->VertexNum ;
				}

				F1ShapeMesh->DimPrev = i == 0                                    ? 0 : F1FileHeadShape->Mesh + sizeof( MV1_SHAPE_MESH_F1 ) * ( i - 1 ) ;
				F1ShapeMesh->DimNext = i == ( DWORD )ModelBase->ShapeMeshNum - 1 ? 0 : F1FileHeadShape->Mesh + sizeof( MV1_SHAPE_MESH_F1 ) * ( i + 1 ) ;
			}

			// �V�F�C�v���_�̏��𖄂߂�
/*
			F1ShapeVertex = ( MV1_SHAPE_VERTEX_F1 * )( ( DWORD_PTR )F1FileHeadShape->Vertex + ( DWORD_PTR )FHeader ) ;
			ShapeVertex = ModelBase->ShapeVertex ;
			for( i = 0 ; i < ModelBase->ShapeVertexNum ; i ++, ShapeVertex ++, F1ShapeVertex ++ )
			{
				F1ShapeVertex->TargetMeshVertex = ShapeVertex->TargetMeshVertex ;
				F1ShapeVertex->Position = ShapeVertex->Position ;
				F1ShapeVertex->Normal = ShapeVertex->Normal ;
			}
*/
		}

		// �������Z�p�̏��𖄂߂�
		if( F1FileHeadPhysics )
		{
			F1PhysicsRigidBody = ( MV1_PHYSICS_RIGIDBODY_F1 * )( ( DWORD_PTR )F1FileHeadPhysics->RigidBody + ( DWORD_PTR )FHeader ) ;
			PhysicsRigidBody = ModelBase->PhysicsRigidBody ;
			for( i = 0 ; i < ( DWORD )ModelBase->PhysicsRigidBodyNum ; i ++, PhysicsRigidBody ++, F1PhysicsRigidBody ++ )
			{
				F1PhysicsRigidBody->Index = ( int )i ;

				F1PhysicsRigidBody->Name = ( DWORD )FHeader->StringSize ;
				FHeader->StringSize += ConvString( ( const char * )PhysicsRigidBody->NameW, WCHAR_T_CHARCODEFORMAT, ( char * )( ( BYTE * )FHeader + FHeader->StringBuffer + F1PhysicsRigidBody->Name ), BUFFERBYTES_CANCEL, DX_CHARCODEFORMAT_UTF8 ) ;
				FHeader->StringSize = ( FHeader->StringSize + 3 ) / 4 * 4 ;

				if( PhysicsRigidBody->TargetFrame )
				{
					F1PhysicsRigidBody->TargetFrame = ( DWORD )( DWORD_PTR )( ( MV1_FRAME_F1 * )( DWORD_PTR )FHeader->Frame + ( PhysicsRigidBody->TargetFrame - ModelBase->Frame ) ) ;
				}

				F1PhysicsRigidBody->RigidBodyGroupIndex = PhysicsRigidBody->RigidBodyGroupIndex ;
				F1PhysicsRigidBody->RigidBodyGroupTarget = PhysicsRigidBody->RigidBodyGroupTarget ;
				F1PhysicsRigidBody->ShapeType = PhysicsRigidBody->ShapeType ;
				F1PhysicsRigidBody->ShapeW = PhysicsRigidBody->ShapeW ;
				F1PhysicsRigidBody->ShapeH = PhysicsRigidBody->ShapeH ;
				F1PhysicsRigidBody->ShapeD = PhysicsRigidBody->ShapeD ;
				F1PhysicsRigidBody->Position = PhysicsRigidBody->Position ;
				F1PhysicsRigidBody->Rotation = PhysicsRigidBody->Rotation ;
				F1PhysicsRigidBody->RigidBodyWeight = PhysicsRigidBody->RigidBodyWeight ;
				F1PhysicsRigidBody->RigidBodyPosDim = PhysicsRigidBody->RigidBodyPosDim ;
				F1PhysicsRigidBody->RigidBodyRotDim = PhysicsRigidBody->RigidBodyRotDim ;
				F1PhysicsRigidBody->RigidBodyRecoil = PhysicsRigidBody->RigidBodyRecoil ;
				F1PhysicsRigidBody->RigidBodyFriction = PhysicsRigidBody->RigidBodyFriction ;
				F1PhysicsRigidBody->RigidBodyType = PhysicsRigidBody->RigidBodyType ;
				F1PhysicsRigidBody->NoCopyToBone = PhysicsRigidBody->NoCopyToBone ;

				F1PhysicsRigidBody->DimPrev = i == 0                                           ? 0 : F1FileHeadPhysics->RigidBody + sizeof( MV1_PHYSICS_RIGIDBODY_F1 ) * ( i - 1 ) ;
				F1PhysicsRigidBody->DimNext = i == ( DWORD )ModelBase->PhysicsRigidBodyNum - 1 ? 0 : F1FileHeadPhysics->RigidBody + sizeof( MV1_PHYSICS_RIGIDBODY_F1 ) * ( i + 1 ) ;
			}

			F1PhysicsJoint = ( MV1_PHYSICS_JOINT_F1 * )( ( DWORD_PTR )F1FileHeadPhysics->Joint + ( DWORD_PTR )FHeader ) ;
			PhysicsJoint = ModelBase->PhysicsJoint ;
			for( i = 0 ; i < ( DWORD )ModelBase->PhysicsJointNum ; i ++, PhysicsJoint ++, F1PhysicsJoint ++ )
			{
				F1PhysicsJoint->Index = ( int )i ;

				F1PhysicsJoint->Name = ( DWORD )FHeader->StringSize ;
				FHeader->StringSize += ConvString( ( const char * )PhysicsJoint->NameW, WCHAR_T_CHARCODEFORMAT, ( char * )( ( BYTE * )FHeader + FHeader->StringBuffer + F1PhysicsJoint->Name ), BUFFERBYTES_CANCEL, DX_CHARCODEFORMAT_UTF8 ) ;
				FHeader->StringSize = ( FHeader->StringSize + 3 ) / 4 * 4 ;

				if( PhysicsJoint->RigidBodyA )
				{
					F1PhysicsJoint->RigidBodyA = ( DWORD )( DWORD_PTR )( ( MV1_PHYSICS_RIGIDBODY_F1 * )( DWORD_PTR )F1FileHeadPhysics->RigidBody + ( PhysicsJoint->RigidBodyA - ModelBase->PhysicsRigidBody ) ) ;
				}

				if( PhysicsJoint->RigidBodyB )
				{
					F1PhysicsJoint->RigidBodyB = ( DWORD )( DWORD_PTR )( ( MV1_PHYSICS_RIGIDBODY_F1 * )( DWORD_PTR )F1FileHeadPhysics->RigidBody + ( PhysicsJoint->RigidBodyB - ModelBase->PhysicsRigidBody ) ) ;
				}

				F1PhysicsJoint->Position = PhysicsJoint->Position ;
				F1PhysicsJoint->Rotation = PhysicsJoint->Rotation ;
				F1PhysicsJoint->ConstrainPosition1 = PhysicsJoint->ConstrainPosition1 ;
				F1PhysicsJoint->ConstrainPosition2 = PhysicsJoint->ConstrainPosition2 ;
				F1PhysicsJoint->ConstrainRotation1 = PhysicsJoint->ConstrainRotation1 ;
				F1PhysicsJoint->ConstrainRotation2 = PhysicsJoint->ConstrainRotation2 ;
				F1PhysicsJoint->SpringPosition = PhysicsJoint->SpringPosition ;
				F1PhysicsJoint->SpringRotation = PhysicsJoint->SpringRotation ;

				F1PhysicsJoint->DimPrev = i == 0                                       ? 0 : F1FileHeadPhysics->Joint + sizeof( MV1_PHYSICS_JOINT_F1 ) * ( i - 1 ) ;
				F1PhysicsJoint->DimNext = i == ( DWORD )ModelBase->PhysicsJointNum - 1 ? 0 : F1FileHeadPhysics->Joint + sizeof( MV1_PHYSICS_JOINT_F1 ) * ( i + 1 ) ;
			}
		}

		// �}�e���A���̏��𖄂߂�
		F1Material = ( MV1_MATERIAL_F1 * )( ( DWORD_PTR )FHeader->Material + ( DWORD_PTR )FHeader ) ;
		MaterialBase = ModelBase->Material ;
		Material = Model->Material ;
		F1MaterialToon = ( MV1_MATERIAL_TOON_F1 * )( ( DWORD_PTR )F1MaterialToonAddr + ( DWORD_PTR )FHeader ) ;
		for( i = 0 ; i < ( DWORD )ModelBase->MaterialNum ; i ++, F1Material ++, F1MaterialToon ++, MaterialBase ++, Material ++ )
		{
			F1Material->Index = ( int )i ;

			F1Material->Name = ( DWORD )FHeader->StringSize ;
			FHeader->StringSize += ConvString( ( const char * )MaterialBase->NameW, WCHAR_T_CHARCODEFORMAT, ( char * )( ( BYTE * )FHeader + FHeader->StringBuffer + F1Material->Name ), BUFFERBYTES_CANCEL, DX_CHARCODEFORMAT_UTF8 ) ;
			FHeader->StringSize = ( FHeader->StringSize + 3 ) / 4 * 4 ;

			F1Material->Diffuse = Material->Diffuse ;
			F1Material->Ambient = Material->Ambient ;
			F1Material->Specular = Material->Specular ;
			F1Material->Emissive = Material->Emissive ;
			F1Material->Power = Material->Power ;

			F1Material->UseAlphaTest = Material->UseAlphaTest ;
			F1Material->AlphaFunc = Material->AlphaFunc ;
			F1Material->AlphaRef = Material->AlphaRef ;

			F1Material->DiffuseLayerNum = Material->DiffuseLayerNum ;
			F1MaterialLayer = F1Material->DiffuseLayer ;
			MaterialLayer = Material->DiffuseLayer ;
			for( j = 0 ; j < ( DWORD )Material->DiffuseLayerNum ; j ++, F1MaterialLayer ++, MaterialLayer ++ )
			{
				F1MaterialLayer->BlendType = MaterialLayer->BlendType ;
				F1MaterialLayer->Texture = MaterialLayer->Texture ;
			}

			F1Material->SpecularLayerNum = Material->SpecularLayerNum ;
			F1MaterialLayer = F1Material->SpecularLayer ;
			MaterialLayer = Material->SpecularLayer ;
			for( j = 0 ; j < ( DWORD )Material->SpecularLayerNum ; j ++, F1MaterialLayer ++, MaterialLayer ++ )
			{
				F1MaterialLayer->BlendType = MaterialLayer->BlendType ;
				F1MaterialLayer->Texture = MaterialLayer->Texture ;
			}

			F1Material->NormalLayerNum = MaterialBase->NormalLayerNum ;
			F1MaterialLayer = F1Material->NormalLayer ;
			MaterialLayer = MaterialBase->NormalLayer ;
			for( j = 0 ; j < ( DWORD )MaterialBase->NormalLayerNum ; j ++, F1MaterialLayer ++, MaterialLayer ++ )
			{
				F1MaterialLayer->BlendType = MaterialLayer->BlendType ;
				F1MaterialLayer->Texture = MaterialLayer->Texture ;
			}

			F1Material->DrawBlendMode = Material->DrawBlendMode ;
			F1Material->DrawBlendParam = Material->DrawBlendParam ;

			F1Material->UserData[ 0 ] = MaterialBase->UserData[ 0 ] ;
			F1Material->UserData[ 1 ] = MaterialBase->UserData[ 1 ] ;
			F1Material->UserData[ 2 ] = MaterialBase->UserData[ 2 ] ;
			F1Material->UserData[ 3 ] = MaterialBase->UserData[ 3 ] ;

			F1Material->DimPrev = i == 0                                   ? 0 : FHeader->Material + sizeof( MV1_MATERIAL_F1 ) * ( i - 1 ) ;
			F1Material->DimNext = i == ( DWORD )ModelBase->MaterialNum - 1 ? 0 : FHeader->Material + sizeof( MV1_MATERIAL_F1 ) * ( i + 1 ) ;

			F1Material->ToonInfo = ( DWORD )( ( DWORD_PTR )F1MaterialToon - ( DWORD_PTR )FHeader ) ;

			F1MaterialToon->Type = ( WORD )MaterialBase->Type ;
			F1MaterialToon->DiffuseGradTexture = Material->DiffuseGradTexture ;
			F1MaterialToon->DiffuseGradBlendType = Material->DiffuseGradBlendType ;
			F1MaterialToon->SpecularGradTexture = Material->SpecularGradTexture ;
			F1MaterialToon->SpecularGradBlendType = Material->SpecularGradBlendType ;
			F1MaterialToon->OutLineWidth = Material->OutLineWidth ;
			F1MaterialToon->OutLineDotWidth = Material->OutLineDotWidth ;
			F1MaterialToon->OutLineColor = Material->OutLineColor ;

			if( MaterialBase->SphereMapTexture < 0 )
			{
				F1MaterialToon->EnableSphereMap = 0 ;
				F1MaterialToon->SphereMapTexture = 0 ;
				F1MaterialToon->SphereMapBlendType = 0 ;
			}
			else
			{
				F1MaterialToon->EnableSphereMap = 1 ;
				F1MaterialToon->SphereMapTexture = ( short )Material->SphereMapTexture ;
				F1MaterialToon->SphereMapBlendType = ( BYTE )Material->SphereMapBlendType ;
			}
		}

		// ���C�g�̏��𖄂߂�
		F1Light = ( MV1_LIGHT_F1 * )( ( DWORD_PTR )FHeader->Light + ( DWORD_PTR )FHeader ) ;
		Light = ModelBase->Light ;
		for( i = 0 ; i < ( DWORD )ModelBase->LightNum ; i ++, F1Light ++, Light ++ )
		{
			F1Light->Index = ( int )i ;

			F1Light->Name = ( DWORD )FHeader->StringSize ;
			FHeader->StringSize += ConvString( ( const char * )Light->NameW, WCHAR_T_CHARCODEFORMAT, ( char * )( ( BYTE * )FHeader + FHeader->StringBuffer + F1Light->Name ), BUFFERBYTES_CANCEL, DX_CHARCODEFORMAT_UTF8 ) ;
			FHeader->StringSize = ( FHeader->StringSize + 3 ) / 4 * 4 ;

			F1Light->FrameIndex = Light->FrameIndex ;
			F1Light->Type = Light->Type ;
			F1Light->Diffuse = Light->Diffuse ;
			F1Light->Specular = Light->Specular ;
			F1Light->Ambient = Light->Ambient ;
			F1Light->Range = Light->Range ;
			F1Light->Falloff = Light->Falloff ;
			F1Light->Attenuation0 = Light->Attenuation0 ;
			F1Light->Attenuation1 = Light->Attenuation1 ;
			F1Light->Attenuation2 = Light->Attenuation2 ;
			F1Light->Theta = Light->Theta ;
			F1Light->Phi = Light->Phi ;

			F1Light->UserData[ 0 ] = Light->UserData[ 0 ] ;
			F1Light->UserData[ 1 ] = Light->UserData[ 1 ] ;

			F1Light->DimPrev = i == 0                                ? 0 : FHeader->Light + sizeof( MV1_LIGHT_F1 ) * ( i - 1 ) ;
			F1Light->DimNext = i == ( DWORD )ModelBase->LightNum - 1 ? 0 : FHeader->Light + sizeof( MV1_LIGHT_F1 ) * ( i + 1 ) ;
		}

		// �e�N�X�`���̏��𖄂߂�
		F1Texture = ( MV1_TEXTURE_F1 * )( ( DWORD_PTR )FHeader->Texture + ( DWORD_PTR )FHeader ) ;
		TextureBase = ModelBase->Texture ;
		Texture = Model->Texture ;
		for( i = 0 ; i < ( DWORD )ModelBase->TextureNum ; i ++, F1Texture ++, TextureBase ++, Texture ++ )
		{
			F1Texture->Index = ( int )i ;
		
			F1Texture->BumpImageFlag = Texture->BumpImageFlag ;
			F1Texture->BumpImageNextPixelLength = Texture->BumpImageNextPixelLength ;

			F1Texture->Name = ( DWORD )FHeader->StringSize ;
			FHeader->StringSize += ConvString( ( const char * )TextureBase->NameW, WCHAR_T_CHARCODEFORMAT, ( char * )( ( BYTE * )FHeader + FHeader->StringBuffer + F1Texture->Name ), BUFFERBYTES_CANCEL, DX_CHARCODEFORMAT_UTF8 ) ;
			FHeader->StringSize = ( FHeader->StringSize + 3 ) / 4 * 4 ;

			F1Texture->AddressModeU = Texture->AddressModeU ;
			F1Texture->AddressModeV = Texture->AddressModeV ;
			F1Texture->ScaleU = Texture->ScaleU ;
			F1Texture->ScaleV = Texture->ScaleV ;
			F1Texture->FilterMode = Texture->FilterMode ;
			F1Texture->Flag = 0 ;
			if( Texture->ReverseFlag )
			{
				F1Texture->Flag |= MV1_TEXTURE_FLAG_REVERSE ;
			}
			if( Texture->Bmp32AllZeroAlphaToXRGB8Flag )
			{
				F1Texture->Flag |= MV1_TEXTURE_FLAG_BMP32_ALL_ZERO_ALPHA_TO_XRGB8 ;
			}
			F1Texture->Flag |= MV1_TEXTURE_FLAG_VALID_SCALE_UV ;

			if( Texture->ColorFilePathW_ == NULL )
			{
				F1Texture->ColorFilePath = 0 ;
			}
			else
			{
				F1Texture->ColorFilePath = ( DWORD )FHeader->StringSize ;
				FHeader->StringSize += ConvString( ( const char * )Texture->ColorFilePathW_, WCHAR_T_CHARCODEFORMAT, ( char * )( ( BYTE * )FHeader + FHeader->StringBuffer + F1Texture->ColorFilePath ), BUFFERBYTES_CANCEL, DX_CHARCODEFORMAT_UTF8 ) ;
				FHeader->StringSize = ( FHeader->StringSize + 3 ) / 4 * 4 ;
			}

			if( Texture->AlphaFilePathW_ == NULL )
			{
				F1Texture->AlphaFilePath = 0 ;
			}
			else
			{
				F1Texture->AlphaFilePath = ( DWORD )FHeader->StringSize ;
				FHeader->StringSize += ConvString( ( const char * )Texture->AlphaFilePathW_, WCHAR_T_CHARCODEFORMAT, ( char * )( ( BYTE * )FHeader + FHeader->StringBuffer + F1Texture->AlphaFilePath ), BUFFERBYTES_CANCEL, DX_CHARCODEFORMAT_UTF8 ) ;
				FHeader->StringSize = ( FHeader->StringSize + 3 ) / 4 * 4 ;
			}

			F1Texture->UserData[ 0 ] = TextureBase->UserData[ 0 ] ;
			F1Texture->UserData[ 1 ] = TextureBase->UserData[ 1 ] ;

			F1Texture->DimPrev = i == 0                                  ? 0 : FHeader->Texture + sizeof( MV1_TEXTURE_F1 ) * ( i - 1 ) ;
			F1Texture->DimNext = i == ( DWORD )ModelBase->TextureNum - 1 ? 0 : FHeader->Texture + sizeof( MV1_TEXTURE_F1 ) * ( i + 1 ) ;
		}

		// ���b�V���̏��𖄂߂�
		F1Mesh = ( MV1_MESH_F1 * )( ( DWORD_PTR )FHeader->Mesh + ( DWORD_PTR )FHeader ) ;
		Mesh = ModelBase->Mesh ;
		for( i = 0 ; i < ( DWORD )ModelBase->MeshNum ; i ++, F1Mesh ++, Mesh ++ )
		{
			F1Mesh->Index = ( int )i ;
			if( Mesh->Container )
			{
				F1Mesh->Container = ( DWORD )( DWORD_PTR )( ( MV1_FRAME_F1 * )( DWORD_PTR )FHeader->Frame + ( Mesh->Container - ModelBase->Frame ) ) ;
			}
			if( Mesh->Material )
			{
				F1Mesh->Material = ( DWORD )( DWORD_PTR )( ( MV1_MATERIAL_F1 * )( DWORD_PTR )FHeader->Material + ( Mesh->Material - ModelBase->Material ) ) ;
			}

			F1Mesh->ChangeInfo.Target   = Mesh->ChangeInfo.Target ;
			F1Mesh->ChangeInfo.Fill     = 0 ;
			F1Mesh->ChangeInfo.Size     = Mesh->ChangeInfo.Size ;
			F1Mesh->ChangeInfo.CheckBit = Mesh->ChangeInfo.CheckBit ;

			F1Mesh->NotOneDiffuseAlpha = ( BYTE )Mesh->NotOneDiffuseAlpha ;
			F1Mesh->Shape = ( BYTE )Mesh->Shape ;
			F1Mesh->UseVertexDiffuseColor = Mesh->UseVertexDiffuseColor ;
			F1Mesh->UseVertexSpecularColor = Mesh->UseVertexSpecularColor ;
			F1Mesh->TriangleListNum = Mesh->TriangleListNum ;
			if( Mesh->TriangleList )
			{
				F1Mesh->TriangleList = ( DWORD )( DWORD_PTR )( ( MV1_TRIANGLE_LIST_F1 * )( DWORD_PTR )FHeader->TriangleList + ( Mesh->TriangleList - ModelBase->TriangleList ) ) ;
			}
			F1Mesh->Visible = Mesh->Visible ;
			F1Mesh->BackCulling = Mesh->BackCulling ;

			F1Mesh->UserData[ 0 ] = Mesh->UserData[ 0 ] ;
			F1Mesh->UserData[ 1 ] = Mesh->UserData[ 1 ] ;
			F1Mesh->UserData[ 2 ] = Mesh->UserData[ 2 ] ;
			F1Mesh->UserData[ 3 ] = Mesh->UserData[ 3 ] ;

			F1Mesh->UVSetUnitNum = ( BYTE )Mesh->UVSetUnitNum ;
			F1Mesh->UVUnitNum = ( BYTE )Mesh->UVUnitNum ;

			F1Mesh->VertexNum = Mesh->VertexNum ;
			F1Mesh->FaceNum = Mesh->FaceNum ;

			// ���_�f�[�^�̍쐬
			{
				MV1_MESH_VERTEX *Vert ;

				// �A�h���X�̃Z�b�g
				F1Mesh->VertexData = FHeader->VertexDataSize ;
				Dest = ( BYTE * )VertexBuffer + FHeader->VertexDataSize ;
				Start = Dest ;

				// �t���O��������
				F1Mesh->VertFlag = 0 ;

				// �g�D�[���p�̃G�b�W�����邩�Ȃ��������o�͂���t���O���Z�b�g
				F1Mesh->VertFlag |= MV1_MESH_VERT_FLAG_NON_TOON_OUTLINE ;

				// ���W�C���f�b�N�X�ɉ��r�b�g�K�v�����ׂ�
				if( Mesh->Container->PositionNum < 256 )
				{
					F1Mesh->VertFlag |= MV1_MESH_VERT_INDEX_TYPE_U8 ;
				}
				else
				if( Mesh->Container->PositionNum < 65536 )
				{
					F1Mesh->VertFlag |= MV1_MESH_VERT_INDEX_TYPE_U16 ;
				}
				else
				{
					F1Mesh->VertFlag |= MV1_MESH_VERT_INDEX_TYPE_U32 ;
				}

				// �@���C���f�b�N�X�ɉ��r�b�g�K�v�����ׂ�
				if( Mesh->Container->NormalNum < 256 )
				{
					F1Mesh->VertFlag |= MV1_MESH_VERT_INDEX_TYPE_U8 << 2 ;
				}
				else
				if( Mesh->Container->NormalNum < 65536 )
				{
					F1Mesh->VertFlag |= MV1_MESH_VERT_INDEX_TYPE_U16 << 2 ;
				}
				else
				{
					F1Mesh->VertFlag |= MV1_MESH_VERT_INDEX_TYPE_U32 << 2 ;
				}

				// UV�l�� 0.0�`1.0 �͈͓̔������ׂ�
				Vert = Mesh->Vertex ;
				for( j = 0 ; j < ( DWORD )Mesh->VertexNum ; j ++, Vert = ( MV1_MESH_VERTEX * )( ( BYTE * )Vert + Mesh->VertUnitSize ) )
				{
					for( k = 0 ; k < ( DWORD )Mesh->UVSetUnitNum ; k ++ )
					{
						if( Vert->UVs[ k ][ 0 ] < 0.0f || Vert->UVs[ k ][ 0 ] > 1.0f ||
							Vert->UVs[ k ][ 1 ] < 0.0f || Vert->UVs[ k ][ 1 ] > 1.0f )
							break ;
					}
					if( k != ( DWORD )Mesh->UVSetUnitNum )
						break ;
				}
				if( j == ( DWORD )F1Mesh->VertexNum )
				{
					F1Mesh->VertFlag |= MV1_MESH_VERT_FLAG_UV_U16 ;
				}

				// ���_�F����F���ǂ����𒲂ׂ�
				Vert = Mesh->Vertex ;
				for( j = 0 ; j < ( DWORD )Mesh->VertexNum - 1 ; j ++, Vert = ( MV1_MESH_VERTEX * )( ( BYTE * )Vert + Mesh->VertUnitSize ) )
				{
					if( *( ( DWORD * )&Vert->DiffuseColor  ) != *( ( DWORD * )&( ( MV1_MESH_VERTEX * )( ( BYTE * )Vert + Mesh->VertUnitSize ) )->DiffuseColor  ) ||
						*( ( DWORD * )&Vert->SpecularColor ) != *( ( DWORD * )&( ( MV1_MESH_VERTEX * )( ( BYTE * )Vert + Mesh->VertUnitSize ) )->SpecularColor ) )
						break ;
				}
				if( Mesh->VertexNum == 1 || j == ( DWORD )Mesh->VertexNum - 1 )
				{
					F1Mesh->VertFlag |= MV1_MESH_VERT_FLAG_COMMON_COLOR ;

					( ( COLOR_U8 * )Dest )[ 0 ] = Mesh->Vertex->DiffuseColor ;
					( ( COLOR_U8 * )Dest )[ 1 ] = Mesh->Vertex->SpecularColor ;
					Dest += 8 ;
				}

				// ���_�C���f�b�N�X�̊i�[
				Vert = Mesh->Vertex ;
				switch( F1Mesh->VertFlag & MV1_MESH_VERT_FLAG_POS_IND_TYPE_MASK )
				{
				case MV1_MESH_VERT_INDEX_TYPE_U8 :
					for( j = 0 ; j < ( DWORD )Mesh->VertexNum ; j ++, Vert = ( MV1_MESH_VERTEX * )( ( BYTE * )Vert + Mesh->VertUnitSize ) )
					{
						*Dest = ( BYTE )Vert->PositionIndex ;
						Dest ++ ;
					}
					break ;

				case MV1_MESH_VERT_INDEX_TYPE_U16 :
					for( j = 0 ; j < ( DWORD )Mesh->VertexNum ; j ++, Vert = ( MV1_MESH_VERTEX * )( ( BYTE * )Vert + Mesh->VertUnitSize ) )
					{
						*( ( WORD * )Dest ) = ( WORD )Vert->PositionIndex ;
						Dest += 2 ;
					}
					break ;

				case MV1_MESH_VERT_INDEX_TYPE_U32 :
					for( j = 0 ; j < ( DWORD )Mesh->VertexNum ; j ++, Vert = ( MV1_MESH_VERTEX * )( ( BYTE * )Vert + Mesh->VertUnitSize ) )
					{
						*( ( DWORD * )Dest ) = ( DWORD )Vert->PositionIndex ;
						Dest += 4 ;
					}
					break ;
				}

				// �@���C���f�b�N�X�̊i�[
				Vert = Mesh->Vertex ;
				switch( ( F1Mesh->VertFlag & MV1_MESH_VERT_FLAG_NRM_IND_TYPE_MASK ) >> 2 )
				{
				case MV1_MESH_VERT_INDEX_TYPE_NONE :
					break ;

				case MV1_MESH_VERT_INDEX_TYPE_U8 :
					for( j = 0 ; j < ( DWORD )Mesh->VertexNum ; j ++, Vert = ( MV1_MESH_VERTEX * )( ( BYTE * )Vert + Mesh->VertUnitSize ) )
					{
						*Dest = ( BYTE )Vert->NormalIndex ;
						Dest ++ ;
					}
					break ;

				case MV1_MESH_VERT_INDEX_TYPE_U16 :
					for( j = 0 ; j < ( DWORD )Mesh->VertexNum ; j ++, Vert = ( MV1_MESH_VERTEX * )( ( BYTE * )Vert + Mesh->VertUnitSize ) )
					{
						*( ( WORD * )Dest ) = ( WORD )Vert->NormalIndex ;
						Dest += 2 ;
					}
					break ;

				case MV1_MESH_VERT_INDEX_TYPE_U32 :
					for( j = 0 ; j < ( DWORD )Mesh->VertexNum ; j ++, Vert = ( MV1_MESH_VERTEX * )( ( BYTE * )Vert + Mesh->VertUnitSize ) )
					{
						*( ( DWORD * )Dest ) = ( DWORD )Vert->NormalIndex ;
						Dest += 4 ;
					}
					break ;
				}

				// ���_�J���[�̊i�[
				if( ( F1Mesh->VertFlag & MV1_MESH_VERT_FLAG_COMMON_COLOR ) == 0 )
				{
					Vert = Mesh->Vertex ;
					for( j = 0 ; j < ( DWORD )Mesh->VertexNum ; j ++, Vert = ( MV1_MESH_VERTEX * )( ( BYTE * )Vert + Mesh->VertUnitSize ) )
					{
						( ( COLOR_U8 * )Dest )[ 0 ] = Vert->DiffuseColor ;
						( ( COLOR_U8 * )Dest )[ 1 ] = Vert->SpecularColor ;
						Dest += 8 ;
					}
				}

				// �t�u�l�̊i�[
				Vert = Mesh->Vertex ;
				if( F1Mesh->VertFlag & MV1_MESH_VERT_FLAG_UV_U16 )
				{
					for( j = 0 ; j < ( DWORD )Mesh->VertexNum ; j ++, Vert = ( MV1_MESH_VERTEX * )( ( BYTE * )Vert + Mesh->VertUnitSize ) )
					{
						for( k = 0 ; k < ( DWORD )Mesh->UVSetUnitNum ; k ++ )
						{
							( ( WORD * )Dest )[ 0 ] = ( WORD )_FTOL( Vert->UVs[ k ][ 0 ] * 65535 ) ;
							( ( WORD * )Dest )[ 1 ] = ( WORD )_FTOL( Vert->UVs[ k ][ 1 ] * 65535 ) ;
							Dest += 4 ;
						}
					}
				}
				else
				{
					for( j = 0 ; j < ( DWORD )Mesh->VertexNum ; j ++, Vert = ( MV1_MESH_VERTEX * )( ( BYTE * )Vert + Mesh->VertUnitSize ) )
					{
						for( k = 0 ; k < ( DWORD )Mesh->UVSetUnitNum ; k ++ )
						{
							( ( float * )Dest )[ 0 ] = Vert->UVs[ k ][ 0 ] ;
							( ( float * )Dest )[ 1 ] = Vert->UVs[ k ][ 1 ] ;
							Dest += 8 ;
						}
					}
				}

				// �g�D�[���p�̗֊s����\�����邩�ǂ����̃t���O���o�͂���
				if( F1Mesh->VertFlag & MV1_MESH_VERT_FLAG_NON_TOON_OUTLINE )
				{
					DWORD BitCount ;
					BYTE Out ;

					Vert = Mesh->Vertex ;
					Out = 0 ;
					BitCount = 0 ;
					for( j = 0 ; j < ( DWORD )Mesh->VertexNum ; j ++, Vert = ( MV1_MESH_VERTEX * )( ( BYTE * )Vert + Mesh->VertUnitSize ) )
					{
						Out |= ( Vert->ToonOutLineScale > 0.0f ? 0 : 1 ) << BitCount ;
						BitCount ++ ;
						if( BitCount == 8 )
						{
							*Dest = Out ;
							Dest ++ ;
							BitCount = 0 ;
							Out = 0 ;
						}
					}
					if( BitCount != 0 )
					{
						*Dest = Out ;
						Dest ++ ;
						BitCount = 0 ;
						Out = 0 ;
					}
				}

				Size = ( DWORD )( Dest - Start ) ;
				Size = ( Size + 3 ) / 4 * 4 ;
				FHeader->VertexDataSize += Size ;
			}

			F1Mesh->DimPrev = i == 0                               ? 0 : FHeader->Mesh + sizeof( MV1_MESH_F1 ) * ( i - 1 ) ;
			F1Mesh->DimNext = i == ( DWORD )ModelBase->MeshNum - 1 ? 0 : FHeader->Mesh + sizeof( MV1_MESH_F1 ) * ( i + 1 ) ;
		}

		// �X�L�j���O���b�V���p�̃{�[�����𖄂߂�
		F1SkinBone = ( MV1_SKIN_BONE_F1 * )( ( DWORD_PTR )FHeader->SkinBone + ( DWORD_PTR )FHeader ) ;
		SkinBone = ModelBase->SkinBone ;
		for( i = 0 ; i < ( DWORD )ModelBase->SkinBoneNum ; i ++, F1SkinBone ++, SkinBone ++ )
		{
			F1SkinBone->Index = ( int )i ;
			F1SkinBone->BoneFrame = SkinBone->BoneFrame ;
			F1SkinBone->ModelLocalMatrix = SkinBone->ModelLocalMatrix ;
			F1SkinBone->ModelLocalMatrixIsTranslateOnly = SkinBone->ModelLocalMatrixIsTranslateOnly ;
			F1SkinBone->UseFrameNum = SkinBone->UseFrameNum ;
			if( SkinBone->UseFrame )
			{
				F1SkinBone->UseFrame = ( DWORD )( DWORD_PTR )( ( MV1_SKIN_BONE_USE_FRAME_F1 ** )( DWORD_PTR )FHeader->SkinBoneUseFrame + ( SkinBone->UseFrame - ModelBase->SkinBoneUseFrame ) ) ;
			}

			F1SkinBone->DimPrev = i == 0                                   ? 0 : FHeader->SkinBone + sizeof( MV1_SKIN_BONE_F1 ) * ( i - 1 ) ;
			F1SkinBone->DimNext = i == ( DWORD )ModelBase->SkinBoneNum - 1 ? 0 : FHeader->SkinBone + sizeof( MV1_SKIN_BONE_F1 ) * ( i + 1 ) ;
		}

		// �g���C�A���O�����X�g�̏��𖄂߂�
		F1TriangleList = ( MV1_TRIANGLE_LIST_F1 * )( ( DWORD_PTR )FHeader->TriangleList + ( DWORD_PTR )FHeader ) ;
		TriangleList = ModelBase->TriangleList ;
		for( i = 0 ; i < ( DWORD )ModelBase->TriangleListNum ; i ++, F1TriangleList ++, TriangleList ++ )
		{
			F1TriangleList->Index = ( int )i ;

			if( TriangleList->Container )
			{
				F1TriangleList->Container = ( DWORD )( DWORD_PTR )( ( MV1_MESH_F1 * )( DWORD_PTR )FHeader->Mesh + ( TriangleList->Container - ModelBase->Mesh ) ) ;
			}
			F1TriangleList->VertexType = TriangleList->VertexType ;
			F1TriangleList->VertexNum = ( unsigned short )TriangleList->VertexNum ;
			F1TriangleList->IndexNum = TriangleList->IndexNum ;

			// ���_�f�[�^�̍쐬
			{
				DWORD *MVInd ;
				WORD *Ind ;

				// �A�h���X�̃Z�b�g
				F1TriangleList->MeshVertexIndexAndIndexData = FHeader->VertexDataSize ;
				Dest = ( BYTE * )VertexBuffer + FHeader->VertexDataSize ;
				Start = Dest ;

				// �t���O�̏�����
				F1TriangleList->Flag = 0 ;

				// ���b�V�����_�C���f�b�N�X��ӂ�̃r�b�g���𒲂ׂ�
				MVInd = TriangleList->MeshVertexIndex ;
				for( j = 0 ; j < ( DWORD )TriangleList->VertexNum ; j ++, MVInd ++ )
				{
					if( *MVInd > 65535 )
					{
						F1TriangleList->Flag = MV1_TRIANGLE_LIST_INDEX_TYPE_U32 ;
						break ;
					}
					else
					if( *MVInd > 255 )
					{
						if( F1TriangleList->Flag != MV1_TRIANGLE_LIST_INDEX_TYPE_U32 )
							F1TriangleList->Flag = MV1_TRIANGLE_LIST_INDEX_TYPE_U16 ;
					}
					else
					{
						if( F1TriangleList->Flag == 0 )
							F1TriangleList->Flag = MV1_TRIANGLE_LIST_INDEX_TYPE_U8 ;
					}
				}

				// ���_�C���f�b�N�X��ӂ�̃r�b�g���𒲂ׂ�
				Ind = TriangleList->Index ;
				for( j = 0 ; j < TriangleList->IndexNum && *Ind < 256 ; j ++, Ind ++ ){}
				F1TriangleList->Flag |= j == TriangleList->IndexNum ? ( MV1_TRIANGLE_LIST_INDEX_TYPE_U8 << 2 ) : ( MV1_TRIANGLE_LIST_INDEX_TYPE_U16 << 2 ) ;

				// �{�[�����̕ۑ�
				switch( TriangleList->VertexType )
				{
				case MV1_VERTEX_TYPE_SKIN_4BONE :
				case MV1_VERTEX_TYPE_SKIN_8BONE :
					*( ( WORD * )Dest ) = ( WORD )TriangleList->UseBoneNum ;
					Dest += 2 ;
					for( j = 0 ; j < ( DWORD )TriangleList->UseBoneNum ; j ++, Dest += 2 )
						*( ( WORD * )Dest ) = ( WORD )TriangleList->UseBone[ j ] ;
					break ;

				case MV1_VERTEX_TYPE_SKIN_FREEBONE :
					*( ( WORD * )Dest ) = ( WORD )TriangleList->MaxBoneNum ;
					Dest += 2 ;
					break ;
				}

				// ���b�V�����_�C���f�b�N�X�̊i�[
				MVInd = TriangleList->MeshVertexIndex ;
				switch( F1TriangleList->Flag & MV1_TRIANGLE_LIST_FLAG_MVERT_INDEX_MASK )
				{
				case MV1_TRIANGLE_LIST_INDEX_TYPE_U8 :
					for( j = 0 ; j < ( DWORD )TriangleList->VertexNum ; j ++, MVInd ++ )
					{
						*Dest = ( BYTE )*MVInd ;
						Dest ++ ;
					}
					break ;

				case MV1_TRIANGLE_LIST_INDEX_TYPE_U16 :
					for( j = 0 ; j < ( DWORD )TriangleList->VertexNum ; j ++, MVInd ++ )
					{
						*( ( WORD * )Dest ) = ( WORD )*MVInd ;
						Dest += 2 ;
					}
					break ;

				case MV1_TRIANGLE_LIST_INDEX_TYPE_U32 :
					for( j = 0 ; j < ( DWORD )TriangleList->VertexNum ; j ++, MVInd ++ )
					{
						*( ( DWORD * )Dest ) = ( DWORD )*MVInd ;
						Dest += 4 ;
					}
					break ;
				}

				// ���_�C���f�b�N�X�̊i�[
				Ind = TriangleList->Index ;
				switch( ( F1TriangleList->Flag & MV1_TRIANGLE_LIST_FLAG_INDEX_MASK ) >> 2 )
				{
				case MV1_TRIANGLE_LIST_INDEX_TYPE_U8 :
					for( j = 0 ; j < TriangleList->IndexNum ; j ++, Ind ++ )
					{
						*Dest = ( BYTE )*Ind ;
						Dest ++ ;
					}
					break ;

				case MV1_TRIANGLE_LIST_INDEX_TYPE_U16 :
					for( j = 0 ; j < TriangleList->IndexNum ; j ++, Ind ++ )
					{
						*( ( WORD * )Dest ) = ( WORD )*Ind ;
						Dest += 2 ;
					}
					break ;
				}

				Size = ( DWORD )( Dest - Start ) ;
				Size = ( Size + 3 ) / 4 * 4 ;
				FHeader->VertexDataSize += Size ;
			}

			F1TriangleList->DimPrev = i == 0                                       ? 0 : FHeader->TriangleList + sizeof( MV1_TRIANGLE_LIST_F1 ) * ( i - 1 ) ;
			F1TriangleList->DimNext = i == ( DWORD )ModelBase->TriangleListNum - 1 ? 0 : FHeader->TriangleList + sizeof( MV1_TRIANGLE_LIST_F1 ) * ( i + 1 ) ;
		}
	}

	if( AnimSave )
	{
		// �A�j���[�V�����L�[�Z�b�g�̏��𖄂߂�
		F1AnimKeySet = ( MV1_ANIM_KEYSET_F1 * )( ( DWORD_PTR )FHeader->AnimKeySet + ( DWORD_PTR )FHeader ) ;
		AnimKeySet = AnimModelBase->AnimKeySet ;
		for( i = 0 ; i < ( DWORD )AnimModelBase->AnimKeySetNum ; i ++, F1AnimKeySet ++, AnimKeySet ++ )
		{
			MV1_ANIM_KEY_16BIT_F  Time16BSubF = { 0.0f } ;
			MV1_ANIM_KEY_16BIT_F  Key16BSubF = { 0.0f } ;
			MV1_ANIM_KEY_16BIT_F1 Time16BSub = { 0 } ;
			MV1_ANIM_KEY_16BIT_F1 Key16BSub = { 0 } ;
			int KeyBit16, TimeBit16, MPPP, ZTP ;
			VECTOR *Vector ;
			FLOAT4 *Quaternion ;
			float *Float ;
			float f ;

			F1AnimKeySet->UserData[ 0 ] = AnimKeySet->UserData[ 0 ] ;

			F1AnimKeySet->Type = AnimKeySet->Type ;
			F1AnimKeySet->DataType = AnimKeySet->DataType ;

			F1AnimKeySet->KeyData = ( DWORD )FHeader->AnimKeyDataSize ;
			Dest = ( BYTE * )AnimBuffer + ( DWORD_PTR )F1AnimKeySet->KeyData ;
			Start = Dest ;
			
			F1AnimKeySet->Flag = 0 ;
			KeyBit16 = 0 ;
			TimeBit16 = 0 ;
			MPPP = 0 ;
			ZTP = 0 ;

			// ��]�L�[�̏ꍇ�͏����𕪊�
			if( AnimKeySet->DataType >= MV1_ANIMKEY_DATATYPE_ROTATE && AnimKeySet->DataType <= MV1_ANIMKEY_DATATYPE_ROTATE_Z )
			{
				// �L�[�f�[�^����]�� -DX_PI�`DX_PI �̊Ԃɒl�����܂��Ă��邩�ƁA0�`2*DX_PI �̊ԂɎ��܂��Ă��邩�ǂ����𒲂ׂ�
				switch( AnimKeySet->Type )
				{
				case MV1_ANIMKEY_TYPE_VECTOR :
					MPPP = 1 ;
					ZTP = 1 ;
					Vector = AnimKeySet->KeyVector ;
					for( j = 0 ; j < ( DWORD )AnimKeySet->Num ; j ++, Vector ++ )
					{
						if( Vector->x < 0.0f         || Vector->x > 2 * DX_PI      ) ZTP  = 0 ;
						if( Vector->y < 0.0f         || Vector->y > 2 * DX_PI      ) ZTP  = 0 ;
						if( Vector->z < 0.0f         || Vector->z > 2 * DX_PI      ) ZTP  = 0 ;
						if( Vector->x < -( float )DX_PI || Vector->x > ( float )DX_PI ) MPPP = 0 ;
						if( Vector->y < -( float )DX_PI || Vector->y > ( float )DX_PI ) MPPP = 0 ;
						if( Vector->z < -( float )DX_PI || Vector->z > ( float )DX_PI ) MPPP = 0 ;
						if( MPPP == 0 && ZTP == 0 ) break ;
					}
					break ;

				case MV1_ANIMKEY_TYPE_LINEAR :
					MPPP = 1 ;
					ZTP = 1 ;
					Float = AnimKeySet->KeyLinear ;
					for( j = 0 ; j < ( DWORD )AnimKeySet->Num ; j ++, Float ++ )
					{
						if( *Float < 0.0f         || *Float > 2 * DX_PI      ) ZTP  = 0 ;
						if( *Float < -( float )DX_PI || *Float > ( float )DX_PI ) MPPP = 0 ;
						if( MPPP == 0 && ZTP == 0 ) break ;
					}
					break ;
				}
			}
			if( MPPP && ZTP )
			{
				MPPP = 0 ;
			}

			// 16�r�b�g���L�[�������Ă���ꍇ�͂P�U�r�b�g���ł��邩���ׂ�
			if( Anim16BitFlag )
			{
				// �f�[�^��������������ꍇ�͏����𕪊�
				if( AnimKeySet->Num == 1 )
				{
					// �⏕�L�[���v��Ȃ��f�[�^�`�����\�ȏꍇ�݂̂P�U�r�b�g��
					if( MPPP || ZTP )
						KeyBit16 = 1 ;
				}
				else
				{
					// �^�C���l���P�U�r�b�g���ł��邩���ׂ�
					if( AnimKeySet->TimeType == MV1_ANIMKEY_TIME_TYPE_KEY )
					{
						// ���ԃf�[�^�������Œ菬���_��
						Time16BSubF.Unit = 1.0f ;
						Time16BSubF.Min = 0.0f ;

						Float = AnimKeySet->KeyTime ;
						Time16BSubF.Min  = Float[ 0 ] ;
						Time16BSubF.Unit = Float[ 0 ] ;
						Float ++ ;
						for( k = 1 ; k < ( DWORD )AnimKeySet->Num ; k ++, Float ++ )
						{
							if( Time16BSubF.Min  > *Float ) Time16BSubF.Min  = *Float ;
							if( Time16BSubF.Unit < *Float ) Time16BSubF.Unit = *Float ;
						}

						// 16bit�⏕���̍쐬
						Time16BSub.Min    = MV1AnimKey16BitMinFtoB( Time16BSubF.Min  ) ;
						Time16BSubF.Min   = MV1AnimKey16BitMinBtoF( Time16BSub.Min ) ;
						Time16BSubF.Unit -= Time16BSubF.Min ;
						if( Time16BSubF.Unit < 0.00000001f )
							Time16BSubF.Unit = 1.0f ;
						Time16BSubF.Unit /= 60000.0f ;
						Time16BSub.Unit   = MV1AnimKey16BitUnitFtoB( Time16BSubF.Unit ) ;
						Time16BSubF.Unit  = MV1AnimKey16BitUnitBtoF( Time16BSub.Unit ) ;

						TimeBit16 = 1 ;
					}

					// �L�[���P�U�r�b�g���ł��邩���ׂ�
					if( MPPP || ZTP )
					{
						KeyBit16 = 1 ;
					}
					else
					{
						switch( AnimKeySet->Type )
						{
						case MV1_ANIMKEY_TYPE_LINEAR :
							{
								float *Key ;

								// �ŏ��l�ƍő�l���擾
								Key16BSubF.Min  =  1000000000000.0f ;
								Key16BSubF.Unit = -1000000000000.0f ;
								Key = AnimKeySet->KeyLinear ;
								for( k = 0 ; k < ( DWORD )AnimKeySet->Num ; k ++, Key ++ )
								{
									 if( Key16BSubF.Min  > *Key ) Key16BSubF.Min  = *Key ;
									 if( Key16BSubF.Unit < *Key ) Key16BSubF.Unit = *Key ;
								}
								KeyBit16 = 1 ;
							}
							break ;

						case MV1_ANIMKEY_TYPE_VECTOR :
							{
								VECTOR *Key ;

								// �ŏ��l�ƍő�l���擾
								Key16BSubF.Min  =  1000000000000.0f ;
								Key16BSubF.Unit = -1000000000000.0f ;
								Key = AnimKeySet->KeyVector ;
								for( k = 0 ; k < ( DWORD )AnimKeySet->Num ; k ++, Key ++ )
								{
									 if( Key16BSubF.Min  > Key->x ) Key16BSubF.Min  = Key->x ;
									 if( Key16BSubF.Unit < Key->x ) Key16BSubF.Unit = Key->x ;
									 if( Key16BSubF.Min  > Key->y ) Key16BSubF.Min  = Key->y ;
									 if( Key16BSubF.Unit < Key->y ) Key16BSubF.Unit = Key->y ;
									 if( Key16BSubF.Min  > Key->z ) Key16BSubF.Min  = Key->z ;
									 if( Key16BSubF.Unit < Key->z ) Key16BSubF.Unit = Key->z ;
								}
								KeyBit16 = 1 ;
							}
							break ;

						case MV1_ANIMKEY_TYPE_QUATERNION_X :
						case MV1_ANIMKEY_TYPE_QUATERNION_VMD :
							{
								FLOAT4 *Key ;

								// �ŏ��l�ƍő�l���擾
								Key16BSubF.Min  =  1000000000000.0f ;
								Key16BSubF.Unit = -1000000000000.0f ;
								Key = AnimKeySet->KeyFloat4 ;
								for( k = 0 ; k < ( DWORD )AnimKeySet->Num ; k ++, Key ++ )
								{
									if( Key16BSubF.Min  > Key->x ) Key16BSubF.Min  = Key->x ;
									if( Key16BSubF.Unit < Key->x ) Key16BSubF.Unit = Key->x ;
									if( Key16BSubF.Min  > Key->y ) Key16BSubF.Min  = Key->y ;
									if( Key16BSubF.Unit < Key->y ) Key16BSubF.Unit = Key->y ;
									if( Key16BSubF.Min  > Key->z ) Key16BSubF.Min  = Key->z ;
									if( Key16BSubF.Unit < Key->z ) Key16BSubF.Unit = Key->z ;
									if( Key16BSubF.Min  > Key->w ) Key16BSubF.Min  = Key->w ;
									if( Key16BSubF.Unit < Key->w ) Key16BSubF.Unit = Key->w ;
								}
								KeyBit16 = 1 ;
							}
							break ;
						}
						if( KeyBit16 )
						{
							// 16bit�⏕���̍쐬
							Key16BSub.Min    = MV1AnimKey16BitMinFtoB( Key16BSubF.Min  ) ;
							Key16BSubF.Min   = MV1AnimKey16BitMinBtoF( Key16BSub.Min ) ;
							Key16BSubF.Unit -= Key16BSubF.Min ;
							if( Key16BSubF.Unit < 0.00000001f )
								Key16BSubF.Unit = 1.0f ;
							Key16BSubF.Unit /= 60000.0f ;
							Key16BSub.Unit   = MV1AnimKey16BitUnitFtoB( Key16BSubF.Unit ) ;
							Key16BSubF.Unit  = MV1AnimKey16BitUnitBtoF( Key16BSub.Unit ) ;
						}
					}
				}
			}

			// �L�[�^�C�v���V�F�C�v�̏ꍇ�͑Ώۂ̃V�F�C�v�C���f�b�N�X��ۑ�����
			if( AnimKeySet->DataType == MV1_ANIMKEY_DATATYPE_SHAPE )
			{
				*( ( WORD * )Dest ) = ( WORD )AnimKeySet->TargetShapeIndex ;
				Dest += 2 ;
			}

			// �L�[������ǂ����ŏ����𕪊�
			if( AnimKeySet->Num == 1 )
			{
				F1AnimKeySet->Flag |= MV1_ANIM_KEYSET_FLAG_KEY_ONE ;
			}
			else
			{
				// �L�[�̐���ۑ�
				if( AnimKeySet->Num < 256 )
				{
					F1AnimKeySet->Flag |= MV1_ANIM_KEYSET_FLAG_KEYNUM_B ;
					*Dest = ( BYTE )AnimKeySet->Num ;
					Dest ++ ;
				}
				else
				if( AnimKeySet->Num < 65536 )
				{
					F1AnimKeySet->Flag |= MV1_ANIM_KEYSET_FLAG_KEYNUM_W ;
					*( ( WORD * )Dest ) = ( WORD )AnimKeySet->Num ;
					Dest += 2 ;
				}
				else
				{
					*( ( DWORD * )Dest ) = ( DWORD )AnimKeySet->Num ;
					Dest += 4 ;
				}

				// �^�C���l�����Ԋu���ǂ����ŏ����𕪊�
				if( AnimKeySet->TimeType == MV1_ANIMKEY_TIME_TYPE_ONE )
				{
					F1AnimKeySet->Flag |= MV1_ANIM_KEYSET_FLAG_TIME_UNIT ;

					// �J�n�l���O���ǂ������ׂ�
					if( AnimKeySet->StartTime > -0.000001f && AnimKeySet->StartTime < 0.000001f )
					{
						// �[���̏ꍇ�͒l���o�͂��Ȃ�
						F1AnimKeySet->Flag |= MV1_ANIM_KEYSET_FLAG_TIME_UNIT_ST_Z ;
					}
					else
					{
						// �l�������l�� 65535 �ȉ����ǂ����𒲂ׂ�
						f = AnimKeySet->StartTime - _FTOL( AnimKeySet->StartTime ) ;
						if( f > -0.000001f && f < 0.000001f && AnimKeySet->StartTime >= 0.0f && AnimKeySet->StartTime <= 65535.0f )
						{
							// �J�n�l��WORD�l�ŕۑ�
							F1AnimKeySet->Flag |= MV1_ANIM_KEYSET_FLAG_TIME_UNIT_ST_W ;
							*( ( WORD * )Dest ) = ( WORD )_FTOL( AnimKeySet->StartTime ) ;
							Dest += 2 ;
						}
						else
						{
							// �J�n�l��float�l�ŕۑ�
							*( ( float * )Dest ) = AnimKeySet->StartTime ;
							Dest += 4 ;
						}
					}

					// �Ԋu�l�������l�� 65535 �ȉ����ǂ����𒲂ׂ�
					f = AnimKeySet->UnitTime - _FTOL( AnimKeySet->UnitTime ) ;
					if( f > -0.000001f && f < 0.000001f && AnimKeySet->UnitTime >= 0.0f && AnimKeySet->UnitTime <= 65535.0f )
					{
						// �Ԋu�l��WORD�l�ŕۑ�
						F1AnimKeySet->Flag |= MV1_ANIM_KEYSET_FLAG_TIME_UNIT_UN_W ;
						*( ( WORD * )Dest ) = ( WORD )_FTOL( AnimKeySet->UnitTime ) ;
						Dest += 2 ;
					}
					else
					{
						// �Ԋu�l��float�l�ŕۑ�
						*( ( float * )Dest ) = AnimKeySet->UnitTime ;
						Dest += 4 ;
					}
				}
				else
				// 16bit������ꍇ�͕⏕����ۑ�
				if( TimeBit16 )
				{
					F1AnimKeySet->Flag |= MV1_ANIM_KEYSET_FLAG_TIME_BIT16 ;

					*( ( MV1_ANIM_KEY_16BIT_F1 * )Dest ) = Time16BSub ;
					Dest += 2 ;

					// �l��ϊ�
					Float = AnimKeySet->KeyTime ;
					for( k = 0 ; k < ( DWORD )AnimKeySet->Num ; k ++, Float ++ )
					{
						*( ( WORD * )Dest ) = ( WORD )_FTOL( ( *Float - Time16BSubF.Min ) / Time16BSubF.Unit ) ;
						Dest += 2 ;
					}
				}
				else
				{
					_MEMCPY( Dest, AnimKeySet->KeyTime, AnimKeySet->Num * sizeof( float ) ) ;
					Dest += AnimKeySet->Num * sizeof( float ) ;
				}

				// 16bit���⏕��񂪕K�v�ȏꍇ�͕ۑ�
				if( MPPP == 0 && ZTP == 0 && KeyBit16 )
				{
					*( ( MV1_ANIM_KEY_16BIT_F1 * )Dest ) = Key16BSub ;
					Dest += 2 ;
				}
			}

			// �L�[�̒l��ۑ�
			if( KeyBit16 == 0 )
			{
				switch( AnimKeySet->Type )
				{
				case MV1_ANIMKEY_TYPE_LINEAR :
					_MEMCPY( Dest, AnimKeySet->KeyLinear, AnimKeySet->Num * sizeof( float ) ) ;
					Dest += AnimKeySet->Num * sizeof( float ) ;
					break ;

				case MV1_ANIMKEY_TYPE_VECTOR :
					_MEMCPY( Dest, AnimKeySet->KeyVector, AnimKeySet->Num * sizeof( VECTOR ) ) ;
					Dest += AnimKeySet->Num * sizeof( VECTOR ) ;
					break ;

				case MV1_ANIMKEY_TYPE_QUATERNION_X :
				case MV1_ANIMKEY_TYPE_QUATERNION_VMD :
					_MEMCPY( Dest, AnimKeySet->KeyFloat4, AnimKeySet->Num * sizeof( FLOAT4 ) ) ;
					Dest += AnimKeySet->Num * sizeof( FLOAT4 ) ;
					break ;

				case MV1_ANIMKEY_TYPE_MATRIX4X4C :
					_MEMCPY( Dest, AnimKeySet->KeyMatrix4x4C, AnimKeySet->Num * sizeof( MATRIX_4X4CT_F ) ) ;
					Dest += AnimKeySet->Num * sizeof( MATRIX_4X4CT_F ) ;
					break ;
				}
			}
			else
			{
				float f ;

				F1AnimKeySet->Flag |= MV1_ANIM_KEYSET_FLAG_KEY_BIT16 ;
				if( MPPP ) 
				{
					F1AnimKeySet->Flag |= MV1_ANIM_KEYSET_FLAG_KEY_MP_PP ;
				}
				if( ZTP )
				{
					F1AnimKeySet->Flag |= MV1_ANIM_KEYSET_FLAG_KEY_Z_TP ;
				}

				switch( AnimKeySet->Type )
				{
				case MV1_ANIMKEY_TYPE_LINEAR :
					if( MPPP )
					{
						Float = AnimKeySet->KeyLinear ;
						for( k = 0 ; k < ( DWORD )AnimKeySet->Num ; k ++, Float ++ )
						{
							*( ( WORD * )Dest ) = ( WORD )_FTOL( ( *Float + ( float )DX_PI ) * 65535.0f / ( float )( DX_PI * 2.0f ) ) ;
							f = *Float - ( *( ( WORD * )Dest ) / 65535.0f * ( float )( DX_PI * 2.0f ) - ( float )DX_PI ) ;
							if( f < -0.5f )
							{
								*( ( WORD * )Dest ) = 0 ;
							}
							else
							if( f > 0.5f )
							{
								*( ( WORD * )Dest ) = 65535 ;
							}
							Dest += 2 ;
						}
					}
					else
					if( ZTP )
					{
						Float = AnimKeySet->KeyLinear ;
						for( k = 0 ; k < ( DWORD )AnimKeySet->Num ; k ++, Float ++ )
						{
							*( ( WORD * )Dest ) = ( WORD )_FTOL( *Float * 65535.0f / ( float )( DX_PI * 2.0f ) ) ;
							f = *Float - ( *( ( WORD * )Dest ) / 65535.0f * ( float )( DX_PI * 2.0f ) ) ;
							if( f < -0.5f )
							{
								*( ( WORD * )Dest ) = 0 ;
							}
							else
							if( f > 0.5f )
							{
								*( ( WORD * )Dest ) = 65535 ;
							}
							Dest += 2 ;
						}
					}
					else
					{
						Float = AnimKeySet->KeyLinear ;
						for( k = 0 ; k < ( DWORD )AnimKeySet->Num ; k ++, Float ++ )
						{
							*( ( WORD * )Dest ) = ( WORD )_FTOL( ( *Float - Key16BSubF.Min ) / Key16BSubF.Unit ) ;
							Dest += 2 ;
						}
					}
					break ;

				case MV1_ANIMKEY_TYPE_VECTOR :
					if( MPPP )
					{
						Vector = AnimKeySet->KeyVector ;
						for( k = 0 ; k < ( DWORD )AnimKeySet->Num ; k ++, Vector ++ )
						{
							( ( WORD * )Dest )[ 0 ] = ( WORD )_FTOL( ( Vector->x + ( float )DX_PI ) * 65535.0f / ( float )( DX_PI * 2.0f ) ) ;
							f = Vector->x - ( ( ( WORD * )Dest )[ 0 ] / 65535.0f * ( float )( DX_PI * 2.0f ) - ( float )DX_PI ) ;
							if( f < -0.5f )
							{
								( ( WORD * )Dest )[ 0 ] = 0 ;
							}
							else
							if( f > 0.5f )
							{
								( ( WORD * )Dest )[ 0 ] = 65535 ;
							}

							( ( WORD * )Dest )[ 1 ] = ( WORD )_FTOL( ( Vector->y + ( float )DX_PI ) * 65535.0f / ( float )( DX_PI * 2.0f ) ) ;
							f = Vector->y - ( ( ( WORD * )Dest )[ 1 ] / 65535.0f * ( float )( DX_PI * 2.0f ) - ( float )DX_PI ) ;
							if( f < -0.5f )
							{
								( ( WORD * )Dest )[ 1 ] = 0 ;
							}
							else
							if( f > 0.5f )
							{
								( ( WORD * )Dest )[ 1 ] = 65535 ;
							}

							( ( WORD * )Dest )[ 2 ] = ( WORD )_FTOL( ( Vector->z + ( float )DX_PI ) * 65535.0f / ( float )( DX_PI * 2.0f ) ) ;
							f = Vector->z - ( ( ( WORD * )Dest )[ 2 ] / 65535.0f * ( float )( DX_PI * 2.0f ) - ( float )DX_PI ) ;
							if( f < -0.5f )
							{
								( ( WORD * )Dest )[ 2 ] = 0 ;
							}
							else
							if( f > 0.5f )
							{
								( ( WORD * )Dest )[ 2 ] = 65535 ;
							}

							Dest += 6 ;
						}
					}
					else
					if( ZTP )
					{
						Vector = AnimKeySet->KeyVector ;
						for( k = 0 ; k < ( DWORD )AnimKeySet->Num ; k ++, Vector ++ )
						{
							( ( WORD * )Dest )[ 0 ] = ( WORD )_FTOL( Vector->x * 65535.0f / ( float )( DX_PI * 2.0f ) ) ;
							f = Vector->x - ( ( ( WORD * )Dest )[ 0 ] / 65535.0f * ( float )( DX_PI * 2.0f ) ) ;
							if( f < -0.5f )
							{
								( ( WORD * )Dest )[ 0 ] = 0 ;
							}
							else
							if( f > 0.5f )
							{
								( ( WORD * )Dest )[ 0 ] = 65535 ;
							}

							( ( WORD * )Dest )[ 1 ] = ( WORD )_FTOL( Vector->y * 65535.0f / ( float )( DX_PI * 2.0f ) ) ;
							f = Vector->y - ( ( ( WORD * )Dest )[ 1 ] / 65535.0f * ( float )( DX_PI * 2.0f ) ) ;
							if( f < -0.5f )
							{
								( ( WORD * )Dest )[ 1 ] = 0 ;
							}
							else
							if( f > 0.5f )
							{
								( ( WORD * )Dest )[ 1 ] = 65535 ;
							}

							( ( WORD * )Dest )[ 2 ] = ( WORD )_FTOL( Vector->z * 65535.0f / ( float )( DX_PI * 2.0f ) ) ;
							f = Vector->z - ( ( ( WORD * )Dest )[ 2 ] / 65535.0f * ( float )( DX_PI * 2.0f ) ) ;
							if( f < -0.5f )
							{
								( ( WORD * )Dest )[ 2 ] = 0 ;
							}
							else
							if( f > 0.5f )
							{
								( ( WORD * )Dest )[ 2 ] = 65535 ;
							}

							Dest += 6 ;
						}
					}
					else
					{
						Vector = AnimKeySet->KeyVector ;
						for( k = 0 ; k < ( DWORD )AnimKeySet->Num ; k ++, Vector ++ )
						{
							( ( WORD * )Dest )[ 0 ] = ( WORD )_FTOL( ( Vector->x - Key16BSubF.Min ) / Key16BSubF.Unit ) ;
							( ( WORD * )Dest )[ 1 ] = ( WORD )_FTOL( ( Vector->y - Key16BSubF.Min ) / Key16BSubF.Unit ) ;
							( ( WORD * )Dest )[ 2 ] = ( WORD )_FTOL( ( Vector->z - Key16BSubF.Min ) / Key16BSubF.Unit ) ;
							Dest += 6 ;
						}
					}
					break ;

				case MV1_ANIMKEY_TYPE_QUATERNION_X :
				case MV1_ANIMKEY_TYPE_QUATERNION_VMD :
					Quaternion        = AnimKeySet->KeyFloat4 ;
					for( k = 0 ; k < ( DWORD )AnimKeySet->Num ; k ++, Quaternion ++ )
					{
						( ( WORD * )Dest )[ 0 ] = ( WORD )_FTOL( ( Quaternion->x - Key16BSubF.Min ) / Key16BSubF.Unit ) ;
						( ( WORD * )Dest )[ 1 ] = ( WORD )_FTOL( ( Quaternion->y - Key16BSubF.Min ) / Key16BSubF.Unit ) ;
						( ( WORD * )Dest )[ 2 ] = ( WORD )_FTOL( ( Quaternion->z - Key16BSubF.Min ) / Key16BSubF.Unit ) ;
						( ( WORD * )Dest )[ 3 ] = ( WORD )_FTOL( ( Quaternion->w - Key16BSubF.Min ) / Key16BSubF.Unit ) ;
						Dest += 8 ;
					}
					break ;
				}
			}
			FHeader->AnimKeyDataSize += ( int )( Dest - Start ) ;
			FHeader->AnimKeyDataSize = ( FHeader->AnimKeyDataSize + 3 ) / 4 * 4 ;
		}

		// �A�j���[�V�����Z�b�g�ƃA�j���[�V�����̏��𖄂߂�
		F1AnimSet = ( MV1_ANIMSET_F1 * )( ( DWORD_PTR )FHeader->AnimSet + ( DWORD_PTR )FHeader ) ;
		AnimSet = AnimModelBase->AnimSet ;
		for( i = 0 ; i < ( DWORD )AnimModelBase->AnimSetNum ; i ++, F1AnimSet ++, AnimSet ++ )
		{
			F1AnimSet->Name = ( DWORD )FHeader->StringSize ;
			FHeader->StringSize += ConvString( ( const char * )AnimSet->NameW, WCHAR_T_CHARCODEFORMAT, ( char * )( ( BYTE * )FHeader + FHeader->StringBuffer + F1AnimSet->Name ), BUFFERBYTES_CANCEL, DX_CHARCODEFORMAT_UTF8 ) ;
			FHeader->StringSize = ( FHeader->StringSize + 3 ) / 4 * 4 ;

			F1AnimSet->Index = AnimSet->Index ;
			F1AnimSet->MaxTime = AnimSet->MaxTime ;
			F1AnimSet->AnimNum = AnimSet->AnimNum ;
			if( AnimSet->Anim )
			{
				F1AnimSet->Anim = ( DWORD )( DWORD_PTR )( ( MV1_ANIM_F1 * )( DWORD_PTR )FHeader->Anim + ( AnimSet->Anim - AnimModelBase->Anim ) ) ;
			}

			if( AnimSet->IsAddAnim )           F1AnimSet->Flag |= 1 ;
			if( AnimSet->IsMatrixLinearBlend ) F1AnimSet->Flag |= 2 ;
			if( AnimSet->IsLoopAnim )          F1AnimSet->Flag |= 4 ;

			F1AnimSet->UserData[ 0 ] = AnimSet->UserData[ 0 ] ;
			F1AnimSet->UserData[ 1 ] = AnimSet->UserData[ 1 ] ;
			F1AnimSet->UserData[ 2 ] = AnimSet->UserData[ 2 ] ;
			F1AnimSet->UserData[ 3 ] = AnimSet->UserData[ 3 ] ;

			F1AnimSet->DimPrev = i == 0                                      ? 0 : FHeader->AnimSet + sizeof( MV1_ANIMSET_F1 ) * ( i - 1 ) ;
			F1AnimSet->DimNext = i == ( DWORD )AnimModelBase->AnimSetNum - 1 ? 0 : FHeader->AnimSet + sizeof( MV1_ANIMSET_F1 ) * ( i + 1 ) ;

			if( AnimMHandle >= 0 )
			{
				AttachIndex = NS_MV1AttachAnim( MHandle, ( int )i, AnimMHandle, AnimNameCheck ) ;
				if( AttachIndex == -1 )  goto ERRORLABEL ;
			}

			F1Anim = ( MV1_ANIM_F1 * )( ( DWORD_PTR )FHeader->Anim + ( DWORD_PTR )FHeader ) + ( AnimSet->Anim - AnimModelBase->Anim ) ;
			Anim = AnimSet->Anim ;
			for( j = 0 ; j < ( DWORD )AnimSet->AnimNum ; j ++, F1Anim ++, Anim ++ )
			{
				F1Anim->Index = ( int )( Anim - AnimModelBase->Anim ) ;

				if( Anim->Container )
				{
					F1Anim->Container = ( DWORD )( DWORD_PTR )( ( MV1_ANIMSET_F1 * )( DWORD_PTR )FHeader->AnimSet + ( Anim->Container - AnimModelBase->AnimSet ) ) ;
				}
				if( AnimMHandle < 0 )
				{
					F1Anim->TargetFrameIndex = Anim->TargetFrameIndex ;
				}
				else
				{
					MV1_MODEL_ANIM *MAnim ;

					for( k = 0 ; k < ( DWORD )ModelBase->FrameNum ; k ++ )
					{
						MAnim = &Model->Anim[ AttachIndex + Model->AnimSetMaxNum * k ] ;
						if( MAnim->Use == false ) continue ;
						if( MAnim->Anim->BaseData == Anim ) break ;
					}
					if( k == ( DWORD )ModelBase->FrameNum )
					{
						NS_MV1DetachAnim( MHandle, AttachIndex ) ;
						Err = -2 ;
						goto ERRORLABEL ;
					}
					else
					{
						F1Anim->TargetFrameIndex = ( int )k ;
					}
				}
				F1Anim->MaxTime = Anim->MaxTime ;
				F1Anim->RotateOrder = Anim->RotateOrder ;
				F1Anim->KeySetNum = Anim->KeySetNum ;
				F1Anim->KeySet = ( DWORD )( ( DWORD_PTR )( ( MV1_ANIM_KEYSET_F1 * )( DWORD_PTR )FHeader->AnimKeySet + ( Anim->KeySet - AnimModelBase->AnimKeySet ) ) ) ;

				F1Anim->UserData[ 0 ] = Anim->UserData[ 0 ] ;
				F1Anim->UserData[ 1 ] = Anim->UserData[ 1 ] ;
			}

			if( AnimMHandle >= 0 )
			{
				NS_MV1DetachAnim( MHandle, AttachIndex ) ;
				AttachIndex = 0 ;
			}
		}
	}

	// ������f�[�^�̃T�C�Y���P�U�̔{���ɍ��킹��
	FHeader->StringSize = ( FHeader->StringSize + 15 ) / 16 * 16 ;
	TempBufferUseSize += FHeader->StringSize ;

	// ���_�f�[�^���t�@�C���̖��[�ɔz�u����
	if( MeshSave )
	{
		if( TempBufferUseSize + FHeader->VertexDataSize > TempBufferSize )
		{
			VertexBufferSize += TempBufferUseSize + FHeader->VertexDataSize - TempBufferSize + 1024 * 1024 ;
			goto SAVELOOP ;
		}

		FHeader->VertexData = TempBufferUseSize ;
		_MEMCPY( ( BYTE * )FHeader + ( DWORD_PTR )FHeader->VertexData, VertexBuffer, FHeader->VertexDataSize ) ;
		TempBufferUseSize += FHeader->VertexDataSize ;

		// ���_�f�[�^�A�h���X��␳
		F1Frame = ( MV1_FRAME_F1 * )( ( DWORD_PTR )FHeader->Frame + ( DWORD_PTR )FHeader ) ;
		for( i = 0 ; i < ( DWORD )ModelBase->FrameNum ; i ++, F1Frame ++ )
		{
			F1Frame->PositionAndNormalData = ( DWORD )( ( DWORD_PTR )F1Frame->PositionAndNormalData + ( DWORD_PTR )FHeader->VertexData ) ;
		}
		F1Mesh = ( MV1_MESH_F1 * )( ( DWORD_PTR )FHeader->Mesh + ( DWORD_PTR )FHeader ) ;
		for( i = 0 ; i < ( DWORD )ModelBase->MeshNum ; i ++, F1Mesh ++ )
		{
			F1Mesh->VertexData = ( DWORD )( ( DWORD_PTR )F1Mesh->VertexData + ( DWORD_PTR )FHeader->VertexData ) ;
		}
		F1TriangleList = ( MV1_TRIANGLE_LIST_F1 * )( ( DWORD_PTR )FHeader->TriangleList + ( DWORD_PTR )FHeader ) ;
		for( i = 0 ; i < ( DWORD )ModelBase->TriangleListNum ; i ++, F1TriangleList ++ )
		{
			F1TriangleList->MeshVertexIndexAndIndexData = ( DWORD )( ( DWORD_PTR )F1TriangleList->MeshVertexIndexAndIndexData + ( DWORD_PTR )FHeader->VertexData ) ;
		}
	}

	// �}�e���A�����X�V���o�p�e�[�u�����t�@�C���̖��[�ɔz�u����
	if( MeshSave == false )
	{
		DWORD *Table ;

		if( TempBufferUseSize + FHeader->ChangeDrawMaterialTableSize > TempBufferSize )
		{
			HeaderSize += TempBufferUseSize + FHeader->ChangeDrawMaterialTableSize - TempBufferSize + 1024 * 1024 ;
			goto SAVELOOP ;
		}

		Table = ( DWORD * )ChangeDrawMaterialTableBuffer ;
		FHeader->ChangeDrawMaterialTable = TempBufferUseSize ;
		_MEMCPY( ( BYTE * )FHeader + ( DWORD_PTR )FHeader->ChangeDrawMaterialTable, Table, ( size_t )FHeader->ChangeDrawMaterialTableSize ) ;
		TempBufferUseSize += FHeader->ChangeDrawMaterialTableSize ;

		// ���o�p�e�[�u���̃A�h���X��␳
		F1Frame = ( MV1_FRAME_F1 * )( ( DWORD_PTR )FHeader->Frame + ( DWORD_PTR )FHeader ) ;
		for( i = 0 ; i < ( DWORD )ModelBase->FrameNum ; i ++, F1Frame ++ )
		{
			F1Frame->ChangeDrawMaterialInfo.Fill = ( DWORD )( ( DWORD_PTR )( ( DWORD * )( ( BYTE * )ChangeDrawMaterialTableBuffer + F1Frame->ChangeDrawMaterialInfo.Fill ) - Table ) + ( DWORD_PTR )FHeader->ChangeDrawMaterialTable ) ;
		}
	}

	// �A�j���[�V�����L�[�f�[�^���t�@�C���̖��[�ɔz�u����
	if( AnimSave )
	{
		if( TempBufferUseSize + FHeader->AnimKeyDataSize > TempBufferSize )
		{
			AnimBufferSize += TempBufferUseSize + FHeader->AnimKeyDataSize - TempBufferSize + 1024 * 1024 ;
			goto SAVELOOP ;
		}

		FHeader->AnimKeyData = TempBufferUseSize ;
		_MEMCPY( ( BYTE * )FHeader + ( DWORD_PTR )FHeader->AnimKeyData, AnimBuffer, ( size_t )FHeader->AnimKeyDataSize ) ;
		TempBufferUseSize += FHeader->AnimKeyDataSize ;

		// �A�j���[�V�����L�[�Z�b�g�̃L�[�f�[�^�A�h���X��␳
		F1AnimKeySet = ( MV1_ANIM_KEYSET_F1 * )( ( DWORD_PTR )FHeader->AnimKeySet + ( DWORD_PTR )FHeader ) ;
		for( i = 0 ; i < ( DWORD )AnimModelBase->AnimKeySetNum ; i ++, F1AnimKeySet ++ )
		{
			F1AnimKeySet->KeyData = ( DWORD )( ( DWORD_PTR )F1AnimKeySet->KeyData + ( DWORD_PTR )FHeader->AnimKeyData ) ;
		}
	}

	// �f�[�^�����k
	{
		if( TempBufferUseSize * 2 + TempBufferUseSize / 2 > TempBufferSize )
		{
			HeaderBufferSize += TempBufferUseSize * 2 + TempBufferUseSize / 2 - TempBufferSize + 1024 * 1024 ;
			goto SAVELOOP ;
		}

		PressData = ( BYTE * )TempBuffer + TempBufferUseSize ;
		( ( MV1MODEL_FILEHEADER_F1 * )PressData )->CheckID[ 0 ] = FHeader->CheckID[ 0 ] ;
		( ( MV1MODEL_FILEHEADER_F1 * )PressData )->CheckID[ 1 ] = FHeader->CheckID[ 1 ] ;
		( ( MV1MODEL_FILEHEADER_F1 * )PressData )->CheckID[ 2 ] = FHeader->CheckID[ 2 ] ;
		( ( MV1MODEL_FILEHEADER_F1 * )PressData )->CheckID[ 3 ] = FHeader->CheckID[ 3 ] ;
		PressDataSize = ( DWORD )DXA_Encode( ( BYTE * )FHeader + 4, ( DWORD )( TempBufferUseSize - 4 ), ( BYTE * )PressData + 4 ) ;
	}

	// �t�@�C���ɏ����o��
	FileHandle = WriteOnlyFileAccessOpen( FileName ) ;
	if( FileHandle )
	{
		WriteOnlyFileAccessWrite( FileHandle, PressData, PressDataSize + 4 ) ;
		WriteOnlyFileAccessClose( FileHandle ) ;
	}

	// �������̉��
	if( TempBuffer )
	{
		DXFREE( TempBuffer ) ;
		TempBuffer = NULL ;
	}

	if( AnimBuffer )
	{
		DXFREE( AnimBuffer ) ;
		AnimBuffer = NULL ;
	}

	if( VertexBuffer )
	{
		DXFREE( VertexBuffer ) ;
		VertexBuffer = NULL ;
	}

	if( ChangeDrawMaterialTableBuffer )
	{
		DXFREE( ChangeDrawMaterialTableBuffer ) ;
		ChangeDrawMaterialTableBuffer = NULL ;
	}

	// �I��
	return 0 ;

ERRORLABEL :
	if( TempBuffer )
	{
		DXFREE( TempBuffer ) ;
		TempBuffer = NULL ;
	}

	if( AnimBuffer )
	{
		DXFREE( AnimBuffer ) ;
		AnimBuffer = NULL ;
	}

	if( VertexBuffer )
	{
		DXFREE( VertexBuffer ) ;
		VertexBuffer = NULL ;
	}

	if( ChangeDrawMaterialTableBuffer )
	{
		DXFREE( ChangeDrawMaterialTableBuffer ) ;
		ChangeDrawMaterialTableBuffer = NULL ;
	}

	return Err ;
}

// �X�y�[�X�� _ �ɕύX���ĕԂ�
static const char *MV1SaveModelToXFileConvSpace( const char *String )
{
	static char TempChar[ 512 ] ;
	int i ;

	for( i = 0 ; String[ i ] ; i ++ )
	{
		if( String[ i ] == ' ' || String[ i ] == ':' )
		{
			TempChar[ i ] = '_' ;
		}
		else
		{
			TempChar[ i ] = String[ i ] ;
		}
	}
	TempChar[ i ] = '\0' ;

	return TempChar ;
}

// �w�萔�̃^�u���o�͂���
static	void MV1SaveModelToXFileOutputTab( DWORD_PTR fp, int TabNum )
{
	int i ;
	char Temp[ 512 ] ;

	for( i = 0 ; i < TabNum ; i ++ )
	{
		Temp[ i ] = '\t' ;
	}
	Temp[ i ] = '\0' ;
	WriteOnlyFileAccessPrintf( fp, Temp ) ;
}

#ifndef DX_NON_SAVEFUNCTION

// �w��̃p�X�Ƀ��f�����w�t�@�C���`���ŕۑ�����
extern int NS_MV1SaveModelToXFile( int MHandle, const TCHAR *FileName, int SaveType, int AnimMHandle, int AnimNameCheck )
{
#ifdef UNICODE
	return MV1SaveModelToXFile_WCHAR_T(
		MHandle, FileName, SaveType, AnimMHandle, AnimNameCheck
	) ;
#else
	int Result ;

	TCHAR_TO_WCHAR_T_STRING_ONE_BEGIN( FileName, return -1 )

	Result = MV1SaveModelToXFile_WCHAR_T(
		MHandle, UseFileNameBuffer, SaveType, AnimMHandle, AnimNameCheck
	) ;

	TCHAR_TO_WCHAR_T_STRING_END( FileName )

	return Result ;
#endif
}

// �w��̃p�X�Ƀ��f�����w�t�@�C���`���ŕۑ�����
extern int MV1SaveModelToXFile_WCHAR_T( int MHandle, const wchar_t *FileName, int SaveType, int AnimMHandle, int AnimNameCheck )
{
	MV1_MODEL *Model, *AnimModel ;
	MV1_MODEL_BASE *ModelBase, *AnimModelBase ;
	MV1_MATERIAL *Material ;
	MV1_MATERIAL_BASE *MaterialBase ;
	MV1_MESH_BASE *Mesh ;
	MV1_MESH_FACE *Face ;
	MV1_MESH_POSITION *Pos ;
	MV1_MESH_NORMAL *Nrm ;
	MV1_MESH_VERTEX *MVert ;
	MV1_SKIN_BONE *SkinB ;
	MV1_FRAME_BASE *Frame, *FrameStack[ 1024 ] ;
	MV1_ANIMSET_BASE *AnimSet ;
	MV1_ANIMSET *MotSet ;
	MV1_ANIM_BASE *Anim ;
	MV1_ANIM *Mot ;
	MV1_ANIM_KEYSET_BASE *KeySet ;
	int FrameStackStep[ 1024 ] ;
	int i, j, k, l, m, FrameStackNum, UseNum, FrameNum, VertexNum, VertexStartIndex ;
	int AnimIndex, AnimNum, Err = -1 ;
	int *KeyTiming = NULL, time, KeyTimingNum ;
	MATRIX_4X4CT_F *KeyMatrix, *Matrix ;
	DWORD_PTR fp = 0 ;
	MV1_SKINBONE_BLEND *UseBoneMap = NULL ;
	bool MeshSave, AnimSave ;
	char String[ 1024 ] ;

	MeshSave = ( SaveType & MV1_SAVETYPE_MESH ) ? true : false ;
	AnimSave = ( SaveType & MV1_SAVETYPE_ANIM ) ? true : false ;

	// ����������Ă��Ȃ�������G���[
	if( MV1Man.Initialize == false ) return -1 ;

	// �A�h���X�擾
	if( MV1MDLCHK( MHandle, Model ) )
		return -1 ;
	ModelBase = Model->BaseData ;

	// �A�j���[�V�����p�̃��f���n���h���̃`�F�b�N
	if( MV1MDLCHK( AnimMHandle, AnimModel ) )
	{
		AnimModelBase = ModelBase ;
		AnimModel = Model ;
		AnimMHandle = -1 ;
	}
	else
	{
		AnimModelBase = AnimModel->BaseData ;
	}

	// �t�@�C�����J��
	fp = WriteOnlyFileAccessOpen( FileName ) ;
	if( fp == 0 )
	{
		return -1 ;
	}

	// �w�b�_�̏o��
	WriteOnlyFileAccessPrintf( fp, "xof 0303txt 0032\n" ) ;

	if( MeshSave )
	{
		// �}�e���A���̏o��
		MaterialBase = ModelBase->Material ;
		Material = Model->Material ;
		for( i = 0 ; i < ModelBase->MaterialNum ; i ++, MaterialBase ++, Material ++ )
		{
			if( MaterialBase->NameW[ 0 ] == L'\0' )
			{
				WriteOnlyFileAccessPrintf( fp, "Material Material_%03d {\n", i ) ;
			}
			else
			{
				ConvString( ( const char * )MaterialBase->NameW, WCHAR_T_CHARCODEFORMAT, String, sizeof( String ), DX_CHARCODEFORMAT_SHIFTJIS ) ;
				WriteOnlyFileAccessPrintf( fp, "Material %s {\n", MV1SaveModelToXFileConvSpace( String ) ) ;
			}
			WriteOnlyFileAccessPrintf( fp, "\t%f;%f;%f;%f;;\n", Material->Diffuse.r, Material->Diffuse.g, Material->Diffuse.b, Material->DrawBlendParam / 255.0f ) ;
			WriteOnlyFileAccessPrintf( fp, "\t%f;\n", Material->Power ) ;
			WriteOnlyFileAccessPrintf( fp, "\t%f;%f;%f;;\n", Material->Specular.r, Material->Specular.g, Material->Specular.b ) ;
			WriteOnlyFileAccessPrintf( fp, "\t%f;%f;%f;;\n", Material->Emissive.r, Material->Emissive.g, Material->Emissive.b ) ;
			if( Material->DiffuseLayerNum )
			{
				MV1_TEXTURE *Texture ;

				Texture = &Model->Texture[ Material->DiffuseLayer[ 0 ].Texture ] ;

				ConvString( ( const char * )Texture->ColorFilePathW_, WCHAR_T_CHARCODEFORMAT, String, sizeof( String ), DX_CHARCODEFORMAT_SHIFTJIS ) ;
				WriteOnlyFileAccessPrintf( fp, "\tTextureFilename {\n\t\t\"%s\";\n\t}\n", String ) ;
			}
			WriteOnlyFileAccessPrintf( fp, "}\n" ) ;
		}

		// �X�L�j���O���b�V���p�̃������̈�ƍ��W�C���f�b�N�X�쐬�p�̃������̈���m��
		UseBoneMap = ( MV1_SKINBONE_BLEND * )DXALLOC( ( ModelBase->TriangleNum * 3 ) * ( sizeof( VECTOR ) + sizeof( int ) ) ) ;
		if( UseBoneMap == NULL ) goto ERRORLABEL ;
	}

	// �g�b�v�t���[������J�n
	{
		Frame = Model->TopFrameList[ 0 ]->BaseData ;
		FrameStack[ 0 ] = Frame ;
		FrameStackStep[ 0 ] = 0 ;
		FrameStackNum = 1 ;

		while( FrameStackNum != 0 )
		{
			Frame = FrameStack[ FrameStackNum - 1 ] ;

			switch( FrameStackStep[ FrameStackNum - 1 ] )
			{
			case 0 :
				MV1SaveModelToXFileOutputTab( fp, FrameStackNum - 1 ) ;

				ConvString( ( const char * )Frame->NameW, WCHAR_T_CHARCODEFORMAT, String, sizeof( String ), DX_CHARCODEFORMAT_SHIFTJIS ) ;
				WriteOnlyFileAccessPrintf( fp, "Frame %s {\n", MV1SaveModelToXFileConvSpace( String ) ) ;

				// �s��̏o��
				{
					MV1SaveModelToXFileOutputTab( fp, FrameStackNum ) ;
					WriteOnlyFileAccessPrintf( fp, "FrameTransformMatrix {\n" ) ;
					MV1SaveModelToXFileOutputTab( fp, FrameStackNum ) ;
					WriteOnlyFileAccessPrintf( fp, "\t%f,%f,%f,%f,\n",
						Frame->LocalTransformMatrix.m[0][0],
						Frame->LocalTransformMatrix.m[1][0],
						Frame->LocalTransformMatrix.m[2][0],
						0.0f ) ;
					MV1SaveModelToXFileOutputTab( fp, FrameStackNum ) ;
					WriteOnlyFileAccessPrintf( fp, "\t%f,%f,%f,%f,\n",
						Frame->LocalTransformMatrix.m[0][1],
						Frame->LocalTransformMatrix.m[1][1],
						Frame->LocalTransformMatrix.m[2][1],
						0.0f ) ;
					MV1SaveModelToXFileOutputTab( fp, FrameStackNum ) ;
					WriteOnlyFileAccessPrintf( fp, "\t%f,%f,%f,%f,\n",
						Frame->LocalTransformMatrix.m[0][2],
						Frame->LocalTransformMatrix.m[1][2],
						Frame->LocalTransformMatrix.m[2][2],
						0.0f ) ;
					MV1SaveModelToXFileOutputTab( fp, FrameStackNum ) ;
					WriteOnlyFileAccessPrintf( fp, "\t%f,%f,%f,%f;;\n",
						Frame->LocalTransformMatrix.m[0][3],
						Frame->LocalTransformMatrix.m[1][3],
						Frame->LocalTransformMatrix.m[2][3],
						1.0f ) ;
					MV1SaveModelToXFileOutputTab( fp, FrameStackNum ) ;
					WriteOnlyFileAccessPrintf( fp, "}\n" ) ;
				}

				if( MeshSave )
				{
					// Mesh�̏o��
					if( Frame->MeshNum != 0 )
					{
						MV1SaveModelToXFileOutputTab( fp, FrameStackNum ) ;
						WriteOnlyFileAccessPrintf( fp, "Mesh {\n" ) ;

						VertexNum = 0 ;
						Mesh = Frame->Mesh ;
						for( j = 0 ; j < Frame->MeshNum ; j ++, Mesh ++ )
						{
							VertexNum += Mesh->VertexNum ;
						}

						MV1SaveModelToXFileOutputTab( fp, FrameStackNum + 1 ) ;
						WriteOnlyFileAccessPrintf( fp, "%d;\n", VertexNum ) ;

						Mesh = Frame->Mesh ;
						for( j = 0 ; j < Frame->MeshNum ; j ++, Mesh ++ )
						{
							MVert = Mesh->Vertex ;
							for( k = 0 ; k < Mesh->VertexNum ; k ++, MVert = ( MV1_MESH_VERTEX * )( ( BYTE * )MVert + Mesh->VertUnitSize ) )
							{
								Pos = ( MV1_MESH_POSITION * )( ( BYTE * )Frame->Position + Frame->PosUnitSize * MVert->PositionIndex ) ;

								MV1SaveModelToXFileOutputTab( fp, FrameStackNum + 1 ) ;
								WriteOnlyFileAccessPrintf( fp, "%f;%f;%f;", Pos->Position.x, Pos->Position.y, Pos->Position.z ) ;
								WriteOnlyFileAccessPrintf( fp, k == Mesh->VertexNum - 1 && j == Frame->MeshNum - 1 ? ";\n" : ",\n" ) ;
							}
						}

						MV1SaveModelToXFileOutputTab( fp, FrameStackNum + 1 ) ;
						WriteOnlyFileAccessPrintf( fp, "%d;\n", Frame->TriangleNum ) ;

						VertexStartIndex = 0 ;
						Mesh = Frame->Mesh ;
						for( j = 0 ; j < Frame->MeshNum ; j ++, Mesh ++ )
						{
							Face = Mesh->Face ;
							for( k = 0 ; k < Mesh->FaceNum ; k ++, Face ++ )
							{
								MV1SaveModelToXFileOutputTab( fp, FrameStackNum + 1 ) ;
								WriteOnlyFileAccessPrintf( fp, "3;%d,%d,%d;", Face->VertexIndex[ 0 ] + VertexStartIndex, Face->VertexIndex[ 1 ] + VertexStartIndex, Face->VertexIndex[ 2 ] + VertexStartIndex ) ;
								WriteOnlyFileAccessPrintf( fp, j == Frame->MeshNum - 1 && k == Mesh->FaceNum - 1 ? ";\n" : ",\n" ) ;
							}
							VertexStartIndex += Mesh->VertexNum ;
						}

						// �@���̏o��
						{
							MV1SaveModelToXFileOutputTab( fp, FrameStackNum + 1 ) ;
							WriteOnlyFileAccessPrintf( fp, "MeshNormals {\n" ) ;

							MV1SaveModelToXFileOutputTab( fp, FrameStackNum + 2 ) ;
							WriteOnlyFileAccessPrintf( fp, "%d;\n", VertexNum ) ;

							Mesh = Frame->Mesh ;
							for( j = 0 ; j < Frame->MeshNum ; j ++, Mesh ++ )
							{
								MVert = Mesh->Vertex ;
								for( k = 0 ; k < Mesh->VertexNum ; k ++, MVert = ( MV1_MESH_VERTEX * )( ( BYTE * )MVert + Mesh->VertUnitSize ) )
								{
									Nrm = Frame->Normal + MVert->NormalIndex ;

									MV1SaveModelToXFileOutputTab( fp, FrameStackNum + 2 ) ;
									WriteOnlyFileAccessPrintf( fp, "%f;%f;%f;", Nrm->Normal.x, Nrm->Normal.y, Nrm->Normal.z ) ;
									WriteOnlyFileAccessPrintf( fp, k == Mesh->VertexNum - 1 && j == Frame->MeshNum - 1 ? ";\n" : ",\n" ) ;
								}
							}

							MV1SaveModelToXFileOutputTab( fp, FrameStackNum + 2 ) ;
							WriteOnlyFileAccessPrintf( fp, "%d;\n", Frame->TriangleNum ) ;

							VertexStartIndex = 0 ;
							Mesh = Frame->Mesh ;
							for( j = 0 ; j < Frame->MeshNum ; j ++, Mesh ++ )
							{
								Face = Mesh->Face ;
								for( k = 0 ; k < Mesh->FaceNum ; k ++, Face ++ )
								{
									MV1SaveModelToXFileOutputTab( fp, FrameStackNum + 2 ) ;
									WriteOnlyFileAccessPrintf( fp, "3;%d,%d,%d;", Face->VertexIndex[ 0 ] + VertexStartIndex, Face->VertexIndex[ 1 ] + VertexStartIndex, Face->VertexIndex[ 2 ] + VertexStartIndex ) ;
									WriteOnlyFileAccessPrintf( fp, j == Frame->MeshNum - 1 && k == Mesh->FaceNum - 1 ? ";\n" : ",\n" ) ;
								}
								VertexStartIndex += Mesh->VertexNum ;
							}

							MV1SaveModelToXFileOutputTab( fp, FrameStackNum + 1 ) ;
							WriteOnlyFileAccessPrintf( fp, "}\n" ) ;
						}

						// �e�N�X�`���A�h���X�̏o��
						{
							MV1SaveModelToXFileOutputTab( fp, FrameStackNum + 1 ) ;
							WriteOnlyFileAccessPrintf( fp, "MeshTextureCoords {\n" ) ;

							MV1SaveModelToXFileOutputTab( fp, FrameStackNum + 2 ) ;
							WriteOnlyFileAccessPrintf( fp, "%d;\n", VertexNum ) ;

							Mesh = Frame->Mesh ;
							for( j = 0 ; j < Frame->MeshNum ; j ++, Mesh ++ )
							{
								MVert = Mesh->Vertex ;
								for( k = 0 ; k < Mesh->VertexNum ; k ++, MVert = ( MV1_MESH_VERTEX * )( ( BYTE * )MVert + Mesh->VertUnitSize ) )
								{
									MV1SaveModelToXFileOutputTab( fp, FrameStackNum + 2 ) ;
									WriteOnlyFileAccessPrintf( fp, "%f;%f;", MVert->UVs[ 0 ][ 0 ], MVert->UVs[ 0 ][ 1 ] ) ;
									WriteOnlyFileAccessPrintf( fp, k == Mesh->VertexNum - 1 && j == Frame->MeshNum - 1 ? ";\n" : ",\n" ) ;
								}
							}

							MV1SaveModelToXFileOutputTab( fp, FrameStackNum + 1 ) ;
							WriteOnlyFileAccessPrintf( fp, "}\n" ) ;
						}

						// MeshMaterialList �̏o��
						{
							MV1SaveModelToXFileOutputTab( fp, FrameStackNum + 1 ) ;
							WriteOnlyFileAccessPrintf( fp, "MeshMaterialList {\n" ) ;

							MV1SaveModelToXFileOutputTab( fp, FrameStackNum + 2 ) ;
							WriteOnlyFileAccessPrintf( fp, "%d;\n", Frame->MeshNum ) ;
							MV1SaveModelToXFileOutputTab( fp, FrameStackNum + 2 ) ;
							WriteOnlyFileAccessPrintf( fp, "%d;\n", Frame->TriangleNum ) ;
							Mesh = Frame->Mesh ;
							for( j = 0 ; j < Frame->MeshNum ; j ++, Mesh ++ )
							{
								Face = Mesh->Face ;
								for( k = 0 ; k < Mesh->FaceNum ; k ++, Face ++ )
								{
									MV1SaveModelToXFileOutputTab( fp, FrameStackNum + 2 ) ;
									WriteOnlyFileAccessPrintf( fp, "%d", j ) ;
									WriteOnlyFileAccessPrintf( fp, j == Frame->MeshNum - 1 && k == Mesh->FaceNum - 1 ? ";\n" : ",\n" ) ;
								}
							}

							Mesh = Frame->Mesh ;
							for( j = 0 ; j < Frame->MeshNum ; j ++, Mesh ++ )
							{
								MV1SaveModelToXFileOutputTab( fp, FrameStackNum + 2 ) ;
								if( Mesh->Material->NameW[ 0 ] == L'\0' )
								{
									WriteOnlyFileAccessPrintf( fp, "{Material_%03d}\n", Mesh->Material - ModelBase->Material ) ;
								}
								else
								{
									ConvString( ( const char * )Mesh->Material->NameW, WCHAR_T_CHARCODEFORMAT, String, sizeof( String ), DX_CHARCODEFORMAT_SHIFTJIS ) ;
									WriteOnlyFileAccessPrintf( fp, "{%s}\n", MV1SaveModelToXFileConvSpace( String ) ) ;
								}
							}

							MV1SaveModelToXFileOutputTab( fp, FrameStackNum + 1 ) ;
							WriteOnlyFileAccessPrintf( fp, "}\n" ) ;
						}

						// �X�L�j���O���b�V�����̏o��
						if( Frame->UseSkinBoneNum != 0 )
						{
							// XSkinMeshHeader �̏o��
							{
								MV1SaveModelToXFileOutputTab( fp, FrameStackNum + 1 ) ;
								WriteOnlyFileAccessPrintf( fp, "XSkinMeshHeader {\n" ) ;

								MV1SaveModelToXFileOutputTab( fp, FrameStackNum + 2 ) ;
								WriteOnlyFileAccessPrintf( fp, "%d;\n", Frame->MaxBoneBlendNum ) ;
								MV1SaveModelToXFileOutputTab( fp, FrameStackNum + 2 ) ;
								WriteOnlyFileAccessPrintf( fp, "%d;\n", Frame->MaxBoneBlendNum * 3 ) ;
								MV1SaveModelToXFileOutputTab( fp, FrameStackNum + 2 ) ;
								WriteOnlyFileAccessPrintf( fp, "%d;\n", Frame->UseSkinBoneNum ) ;

								MV1SaveModelToXFileOutputTab( fp, FrameStackNum + 1 ) ;
								WriteOnlyFileAccessPrintf( fp, "}\n" ) ;
							}

							// �X�L�j���O�E�G�C�g���̏o��
							for( j = 0 ; j < Frame->UseSkinBoneNum ; j ++ )
							{
								SkinB = Frame->UseSkinBone[ j ] ;

								MV1SaveModelToXFileOutputTab( fp, FrameStackNum + 1 ) ;
								WriteOnlyFileAccessPrintf( fp, "SkinWeights {\n" ) ;

								MV1SaveModelToXFileOutputTab( fp, FrameStackNum + 2 ) ;

								ConvString( ( const char * )ModelBase->Frame[ SkinB->BoneFrame ].NameW, WCHAR_T_CHARCODEFORMAT, String, sizeof( String ), DX_CHARCODEFORMAT_SHIFTJIS ) ;
								WriteOnlyFileAccessPrintf( fp, "\"%s\";\n", MV1SaveModelToXFileConvSpace( String ) ) ;

								UseNum = 0 ;
								VertexStartIndex = 0 ;
								Mesh = Frame->Mesh ;
								for( k = 0 ; k < Frame->MeshNum ; k ++, Mesh ++ )
								{
									MVert = Mesh->Vertex ;
									for( l = 0 ; l < Mesh->VertexNum ; l ++, MVert = ( MV1_MESH_VERTEX * )( ( BYTE * )MVert + Mesh->VertUnitSize ) )
									{
										Pos = ( MV1_MESH_POSITION * )( ( BYTE * )Frame->Position + Frame->PosUnitSize * MVert->PositionIndex ) ;
										for( m = 0 ; m < Frame->MaxBoneBlendNum ; m ++ )
										{
											if( Pos->BoneWeight[ m ].Index == j )
												break ;
										}
										if( m != Frame->MaxBoneBlendNum && ( *( ( DWORD * )&Pos->BoneWeight[ m ].W ) & 0x7fffffff ) != 0 )
										{
											UseBoneMap[ UseNum ].Index = l + VertexStartIndex ;
											UseBoneMap[ UseNum ].W = Pos->BoneWeight[ m ].W ;
											UseNum ++ ;
										}
									}
									VertexStartIndex += Mesh->VertexNum ;
								}

								MV1SaveModelToXFileOutputTab( fp, FrameStackNum + 2 ) ;
								WriteOnlyFileAccessPrintf( fp, "%d;\n", UseNum ) ;

								for( k = 0 ; k < UseNum ; k ++ )
								{
									MV1SaveModelToXFileOutputTab( fp, FrameStackNum + 2 ) ;
									WriteOnlyFileAccessPrintf( fp, "%d", UseBoneMap[ k ].Index ) ;
									if( k != UseNum - 1 )
									{
										WriteOnlyFileAccessPrintf( fp, ",\n" ) ;
									}
								}
								WriteOnlyFileAccessPrintf( fp, ";\n" ) ;

								for( k = 0 ; k < UseNum ; k ++ )
								{
									MV1SaveModelToXFileOutputTab( fp, FrameStackNum + 2 ) ;
									WriteOnlyFileAccessPrintf( fp, "%f", UseBoneMap[ k ].W ) ;
									if( k != UseNum - 1 )
									{
										WriteOnlyFileAccessPrintf( fp, ",\n" ) ;
									}
								}
								WriteOnlyFileAccessPrintf( fp, ";\n" ) ;

								MV1SaveModelToXFileOutputTab( fp, FrameStackNum + 2 ) ;
								WriteOnlyFileAccessPrintf( fp, "%f,%f,%f,%f,\n",
									SkinB->ModelLocalMatrix.m[ 0 ][ 0 ],
									SkinB->ModelLocalMatrix.m[ 1 ][ 0 ],
									SkinB->ModelLocalMatrix.m[ 2 ][ 0 ],
									0.0f ) ;
								MV1SaveModelToXFileOutputTab( fp, FrameStackNum + 2 ) ;
								WriteOnlyFileAccessPrintf( fp, "%f,%f,%f,%f,\n",
									SkinB->ModelLocalMatrix.m[ 0 ][ 1 ],
									SkinB->ModelLocalMatrix.m[ 1 ][ 1 ],
									SkinB->ModelLocalMatrix.m[ 2 ][ 1 ],
									0.0f ) ;
								MV1SaveModelToXFileOutputTab( fp, FrameStackNum + 2 ) ;
								WriteOnlyFileAccessPrintf( fp, "%f,%f,%f,%f,\n",
									SkinB->ModelLocalMatrix.m[ 0 ][ 2 ],
									SkinB->ModelLocalMatrix.m[ 1 ][ 2 ],
									SkinB->ModelLocalMatrix.m[ 2 ][ 2 ],
									0.0f ) ;
								MV1SaveModelToXFileOutputTab( fp, FrameStackNum + 2 ) ;
								WriteOnlyFileAccessPrintf( fp, "%f,%f,%f,%f;;\n",
									SkinB->ModelLocalMatrix.m[ 0 ][ 3 ],
									SkinB->ModelLocalMatrix.m[ 1 ][ 3 ],
									SkinB->ModelLocalMatrix.m[ 2 ][ 3 ],
									1.0f ) ;

								MV1SaveModelToXFileOutputTab( fp, FrameStackNum + 1 ) ;
								WriteOnlyFileAccessPrintf( fp, "}\n" ) ;
							}
						}

						MV1SaveModelToXFileOutputTab( fp, FrameStackNum ) ;
						WriteOnlyFileAccessPrintf( fp, "}\n" ) ;
					}
				}

				FrameStackStep[ FrameStackNum - 1 ] = 1 ;
				if( Frame->FirstChild )
				{
					FrameStack[ FrameStackNum ] = Frame->FirstChild ;
					FrameStackStep[ FrameStackNum ] = 0 ;
					FrameStackNum ++ ;
				}
				break ;

			case 1 :
				MV1SaveModelToXFileOutputTab( fp, FrameStackNum - 1 ) ;
				WriteOnlyFileAccessPrintf( fp, "}\n" ) ;

				FrameStackNum -- ;
				if( Frame->Next )
				{
					FrameStack[ FrameStackNum ] = Frame->Next ;
					FrameStackStep[ FrameStackNum ] = 0 ;
					FrameStackNum ++ ;
				}
				break ;
			}
		}
	}

	// �A�j���[�V�����̏o��
	if( AnimSave )
	{
		KeyTiming = NULL ;
		if( AnimModelBase->AnimSetNum != 0 )
		{
			j = 0 ;
			KeySet = AnimModelBase->AnimKeySet ;
			for( i = 0 ; i < AnimModelBase->AnimKeySetNum ; i ++, KeySet ++ )
				j += KeySet->Num ;

			FrameNum = AnimModelBase->FrameNum ;
			KeyTiming = ( int * )DXALLOC( ( sizeof( MATRIX_4X4CT_F ) * AnimModelBase->FrameNum + sizeof( int ) ) * j ) ;
			if( KeyTiming == NULL ) goto ERRORLABEL ;
			KeyMatrix = ( MATRIX_4X4CT_F * )( KeyTiming + j ) ;

			AnimSet = AnimModelBase->AnimSet ;
			for( i = 0 ; i < AnimModelBase->AnimSetNum ; i ++, AnimSet ++ )
			{
				AnimNum = AnimSet->AnimNum ;

				ConvString( ( const char * )AnimSet->NameW, WCHAR_T_CHARCODEFORMAT, String, sizeof( String ), DX_CHARCODEFORMAT_SHIFTJIS ) ;
				WriteOnlyFileAccessPrintf( fp, "AnimationSet %s {\n", MV1SaveModelToXFileConvSpace( String ) ) ;

				// �S�ẴL�[�^�C�~���O���
				KeyTimingNum = 0 ;
				Anim = AnimSet->Anim ;
				for( j = 0 ; j < AnimSet->AnimNum ; j ++, Anim ++ )
				{
					KeySet = Anim->KeySet ;
					for( k = 0 ; k < Anim->KeySetNum ; k ++, KeySet ++ )
					{
						for( l = 0 ; l < KeySet->Num ; l ++ )
						{
							switch( KeySet->TimeType )
							{
							case MV1_ANIMKEY_TIME_TYPE_ONE :
								time = _FTOL( l * KeySet->UnitTime + KeySet->StartTime ) ;
								break ;

							case MV1_ANIMKEY_TIME_TYPE_KEY :
								time = _FTOL( KeySet->KeyTime[ l ] ) ;
								break ;
							}
							for( m = 0 ; m < KeyTimingNum && KeyTiming[ m ] < time ; m ++ ){}
							if( m == KeyTimingNum || KeyTiming[ m ] != time )
							{
								if( m != KeyTimingNum )
								{
									_MEMMOVE( &KeyTiming[ m + 1 ], &KeyTiming[ m ], sizeof( int ) * ( KeyTimingNum - m ) ) ;
								}
								KeyTiming[ m ] = time ;
								KeyTimingNum ++ ;
							}
						}
					}
				}

				// �S�ẴL�[�^�C�~���O�ł̍s��̊���o��
				AnimIndex = NS_MV1AttachAnim( MHandle, i, AnimMHandle >= 0 ? AnimMHandle : MHandle, AnimNameCheck ) ;
				if( AnimIndex == -1 ) goto ERRORLABEL ;

				MotSet = Model->AnimSet[ AnimIndex ].AnimSet ;
				Matrix = KeyMatrix ;
				for( j = 0 ; j < KeyTimingNum ; j ++ )
				{
					NS_MV1SetAttachAnimTime( MHandle, AnimIndex, ( float )KeyTiming[ j ] ) ;
					MV1SetupAnimMatrix( Model ) ;

					Mot = MotSet->Anim ;
					for( k = 0 ; k < AnimSet->AnimNum ; k ++, Mot ++, Matrix ++ )
					{
						MV1SetupTransformMatrix(
							Matrix,
							Mot->ValidFlag,
							&Mot->Translate,
							&Mot->Scale,
							Mot->RotateOrder,
							( Mot->Frame->BaseData->Flag & MV1_FRAMEFLAG_PREROTATE ) != 0 ? &Mot->Frame->BaseData->PreRotate : NULL,
							&Mot->Rotate,
							( Mot->Frame->BaseData->Flag & MV1_FRAMEFLAG_POSTROTATE ) != 0 ? &Mot->Frame->BaseData->PostRotate : NULL,
							&Mot->Quaternion
						) ;
					}
				}

				// �L�[���̏o��
				Anim = AnimSet->Anim ;
				for( j = 0 ; j < AnimSet->AnimNum ; j ++, Anim ++ )
				{
					if( AnimMHandle < 0 )
					{
						ConvString( ( const char * )Anim->TargetFrame->NameW, WCHAR_T_CHARCODEFORMAT, String, sizeof( String ), DX_CHARCODEFORMAT_SHIFTJIS ) ;
						WriteOnlyFileAccessPrintf( fp, "\tAnimation {\n\t\t{%s}\n", MV1SaveModelToXFileConvSpace( String ) ) ;
					}
					else
					{
						MV1_MODEL_ANIM *MAnim ;

						for( k = 0 ; k < ModelBase->FrameNum ; k ++ )
						{
							MAnim = &Model->Anim[ AnimIndex + Model->AnimSetMaxNum * k ] ;
							if( MAnim->Use == false ) continue ;
							if( MAnim->Anim->BaseData == Anim ) break ;
						}
						if( k == ModelBase->FrameNum )
						{
							NS_MV1DetachAnim( AnimMHandle, AnimIndex ) ;
							Err = -2 ;
							goto ERRORLABEL ;
						}

						ConvString( ( const char * )ModelBase->Frame[ k ].NameW, WCHAR_T_CHARCODEFORMAT, String, sizeof( String ), DX_CHARCODEFORMAT_SHIFTJIS ) ;
						WriteOnlyFileAccessPrintf( fp, "\tAnimation {\n\t\t{%s}\n", MV1SaveModelToXFileConvSpace( String ) ) ;
					}

					WriteOnlyFileAccessPrintf( fp, "\t\tAnimationKey {\n\t\t\t4;\n\t\t\t%d;\n", KeyTimingNum ) ;
					Matrix = KeyMatrix + j ;
					for( k = 0 ; k < KeyTimingNum ; k ++, Matrix += AnimNum )
					{
						WriteOnlyFileAccessPrintf( fp, "\t\t\t%d;16;\n\t\t\t%f,%f,%f,%f,\n\t\t\t%f,%f,%f,%f,\n\t\t\t%f,%f,%f,%f,\n\t\t\t%f,%f,%f,%f;;",
							KeyTiming[ k ],
							Matrix->m[ 0 ][ 0 ], Matrix->m[ 1 ][ 0 ], Matrix->m[ 2 ][ 0 ], 0.0f,
							Matrix->m[ 0 ][ 1 ], Matrix->m[ 1 ][ 1 ], Matrix->m[ 2 ][ 1 ], 0.0f,
							Matrix->m[ 0 ][ 2 ], Matrix->m[ 1 ][ 2 ], Matrix->m[ 2 ][ 2 ], 0.0f,
							Matrix->m[ 0 ][ 3 ], Matrix->m[ 1 ][ 3 ], Matrix->m[ 2 ][ 3 ], 1.0f ) ;
						WriteOnlyFileAccessPrintf( fp, k == KeyTimingNum - 1 ? ";\n" : ",\n" ) ;
					}

					WriteOnlyFileAccessPrintf( fp, "\t\t}\n" ) ;
					WriteOnlyFileAccessPrintf( fp, "\t}\n" ) ;
				}
				WriteOnlyFileAccessPrintf( fp, "}\n" ) ;

				NS_MV1DetachAnim( MHandle, AnimIndex ) ;
			}
		}
	}

	// �������̉��
	if( UseBoneMap != 0 )
	{
		DXFREE( UseBoneMap ) ;
		UseBoneMap = NULL ;
	}

	if( KeyTiming != 0 )
	{
		DXFREE( KeyTiming ) ;
		KeyTiming = NULL ;
	}

	// �t�@�C�������
	WriteOnlyFileAccessClose( fp ) ;

	// �I��
	return 0 ;

ERRORLABEL :
	if( UseBoneMap != 0 )
	{
		DXFREE( UseBoneMap ) ;
		UseBoneMap = NULL ;
	}

	if( KeyTiming != 0 )
	{
		DXFREE( KeyTiming ) ;
		KeyTiming = NULL ;
	}

	if( fp )
	{
		WriteOnlyFileAccessClose( fp ) ;
		fp = 0 ;
	}

	return Err ;
}

#endif // DX_NON_SAVEFUNCTION
































// ���f���n���h���Ŏg�p����Ă��郂�f����{�f�[�^�n���h�����擾����
extern int MV1GetModelBaseHandle( int MHandle )
{
	MV1_MODEL *Model ;

	// ����������Ă��Ȃ�������G���[
	if( MV1Man.Initialize == false ) return -1 ;

	// �A�h���X�擾
	if( MV1MDLCHK( MHandle, Model ) )
		return -1 ;

	// ���f����{�f�[�^�n���h����Ԃ�
	return Model->BaseDataHandle ;
}

// ���f���̃f�[�^�T�C�Y���擾����
extern int MV1GetModelDataSize( int MHandle, int DataType )
{
	MV1_MODEL *Model ;
	MV1_MODEL_BASE *ModelBase ;

	// ����������Ă��Ȃ�������G���[
	if( MV1Man.Initialize == false ) return -1 ;

	// �A�h���X�擾
	if( MV1MDLCHK( MHandle, Model ) )
		return -1 ;
	ModelBase = Model->BaseData ;

	// �f�[�^�^�C�v�ɂ���ď����𕪊�
	switch( DataType )
	{
//	case MV1_DATASIZE_TOTAL :
//		return ModelBase->TotalDataSize ;

	case MV1_DATASIZE_VERTEX :
		return ( int )ModelBase->VertexDataSize ;

	case MV1_DATASIZE_STRING :
		return
#ifndef UNICODE
			ModelBase->StringSizeA +
#endif
			ModelBase->StringSizeW ;

	case MV1_DATASIZE_ANIM :
		return ( int )(
				ModelBase->AnimKeyDataSize +
				ModelBase->AnimSetNum      * sizeof( MV1_ANIMSET_BASE     ) +
				ModelBase->AnimNum         * sizeof( MV1_ANIM_BASE        ) +
				ModelBase->AnimKeySetNum   * sizeof( MV1_ANIM_KEYSET_BASE ) ) ;

//	case MV1_DATASIZE_OTHER :
//		return ModelBase->TotalDataSize -
//			( MV1GetModelDataSize( MHandle, MV1_DATASIZE_VERTEX ) +
//			  MV1GetModelDataSize( MHandle, MV1_DATASIZE_STRING ) +
//			  MV1GetModelDataSize( MHandle, MV1_DATASIZE_ANIM ) ) ;
	}

	// �G���[
	return -1 ;
}

// �A�j���[�V�����̃f�[�^�T�C�Y���擾����
extern int MV1GetAnimDataSize( int MHandle, const wchar_t *AnimName, int AnimIndex )
{
	MV1_MODEL *Model ;
	MV1_MODEL_BASE *ModelBase ;
	MV1_ANIMSET_BASE *AnimSetBase ;
	MV1_ANIM_BASE *AnimBase ;
	DWORD DataSize, i ;

	// ����������Ă��Ȃ�������G���[
	if( MV1Man.Initialize == false ) return -1 ;

	// �A�h���X�擾
	if( MV1MDLCHK( MHandle, Model ) )
		return -1 ;
	ModelBase = Model->BaseData ;

	// �C���f�b�N�X���s����������G���[
	if( AnimName == NULL && ( AnimIndex < 0 || AnimIndex >= Model->BaseData->AnimSetNum ) )
		return -1 ;

	// �A�j���[�V�������擾
	AnimSetBase = MV1GetAnimSetBase( Model->BaseDataHandle, AnimName, AnimIndex ) ;
	if( AnimSetBase == NULL ) return -1 ;

	// �f�[�^�T�C�Y�̊���o���J�n
	DataSize = AnimSetBase->KeyDataSize + sizeof( MV1_ANIMSET_BASE ) + sizeof( MV1_ANIM_BASE ) * AnimSetBase->AnimNum ;
	
	// �A�j���[�V�����̐������J��Ԃ�
	AnimBase = AnimSetBase->Anim ;
	for( i = 0 ; i < ( DWORD )AnimSetBase->AnimNum ; i ++, AnimBase ++ )
	{
		DataSize += AnimBase->KeySetNum * sizeof( MV1_ANIM_KEYSET_BASE  ) ;
	}

	// �f�[�^�T�C�Y��Ԃ�
	return ( int )DataSize ;
}


















// ���f���̃��[�J�����W���烏�[���h���W�ɕϊ�����s��𓾂�
extern MATRIX NS_MV1GetLocalWorldMatrix( int MHandle )
{
	MV1_MODEL *Model ;
	MATRIX ResultMatrix ;

	// ����������Ă��Ȃ�������G���[
	if( MV1Man.Initialize == false ) return MGetIdent() ;

	// �A�h���X�擾
	if( MV1MDLCHK( MHandle, Model ) )
		return MGetIdent() ;

	// �s���Ԃ�
	MV1SETUPMATRIX( Model ) ;
	ConvertMatrix4x4cToMatrixF( &ResultMatrix, &Model->LocalWorldMatrix ) ;
	return ResultMatrix ;
}

// ���f���̃��[�J�����W���烏�[���h���W�ɕϊ�����s��𓾂�
extern MATRIX_D NS_MV1GetLocalWorldMatrixD( int MHandle )
{
	MV1_MODEL *Model ;
	MATRIX_D ResultMatrix ;

	// ����������Ă��Ȃ�������G���[
	if( MV1Man.Initialize == false ) return MGetIdentD() ;

	// �A�h���X�擾
	if( MV1MDLCHK( MHandle, Model ) )
		return MGetIdentD() ;

	// �s���Ԃ�
	MV1SETUPMATRIX( Model ) ;
	ConvertMatrix4x4cToMatrixD( &ResultMatrix, &Model->LocalWorldMatrix ) ;
	return ResultMatrix ;
}

// ���f���̍��W���Z�b�g
extern int NS_MV1SetPosition( int MHandle, VECTOR Position )
{
	MV1_MODEL *Model ;

	// ����������Ă��Ȃ�������G���[
	if( MV1Man.Initialize == false ) return -1 ;

	// �A�h���X�擾
	if( MV1MDLCHK( MHandle, Model ) )
		return -1 ;

	// ���܂łƓ������W�������牽�������ɏI��
	if( Model->Translation.x == ( double )Position.x &&
		Model->Translation.y == ( double )Position.y &&
		Model->Translation.z == ( double )Position.z )
	{
		return 0 ;
	}

	// ���s�ړ��l�̃Z�b�g
	Model->Translation = VConvFtoD( Position ) ;

	// �X�V�t���O�𗧂Ă�
	Model->LocalWorldMatrixSetupFlag = false ;
	if( ( Model->ChangeMatrixFlag[ 0 ] & 1 ) == 0 )
		_MEMSET( Model->ChangeMatrixFlag, 0xff, Model->ChangeMatrixFlagSize ) ;

	// �I��
	return 0 ;
}

// ���f���̍��W���Z�b�g
extern int NS_MV1SetPositionD( int MHandle, VECTOR_D Position )
{
	MV1_MODEL *Model ;

	// ����������Ă��Ȃ�������G���[
	if( MV1Man.Initialize == false ) return -1 ;

	// �A�h���X�擾
	if( MV1MDLCHK( MHandle, Model ) )
		return -1 ;

	// ���܂łƓ������W�������牽�������ɏI��
	if( Model->Translation.x == Position.x &&
		Model->Translation.y == Position.y &&
		Model->Translation.z == Position.z )
	{
		return 0 ;
	}

	// ���s�ړ��l�̃Z�b�g
	Model->Translation = Position ;

	// �X�V�t���O�𗧂Ă�
	Model->LocalWorldMatrixSetupFlag = false ;
	if( ( Model->ChangeMatrixFlag[ 0 ] & 1 ) == 0 )
		_MEMSET( Model->ChangeMatrixFlag, 0xff, Model->ChangeMatrixFlagSize ) ;

	// �I��
	return 0 ;
}

// ���f���̍��W���擾
extern VECTOR NS_MV1GetPosition( int MHandle )
{
	MV1_MODEL *Model ;
	VECTOR NullPos = { 0.0f, 0.0f, 0.0f } ;

	// ����������Ă��Ȃ�������G���[
	if( MV1Man.Initialize == false ) return NullPos ;

	// �A�h���X�擾
	if( MV1MDLCHK( MHandle, Model ) )
		return NullPos ;

	// ���s�ړ��l��Ԃ�
	return VConvDtoF( Model->Translation ) ;
}

// ���f���̍��W���擾
extern VECTOR_D NS_MV1GetPositionD( int MHandle )
{
	MV1_MODEL *Model ;
	VECTOR_D NullPos = { 0.0, 0.0, 0.0 } ;

	// ����������Ă��Ȃ�������G���[
	if( MV1Man.Initialize == false ) return NullPos ;

	// �A�h���X�擾
	if( MV1MDLCHK( MHandle, Model ) )
		return NullPos ;

	// ���s�ړ��l��Ԃ�
	return Model->Translation ;
}

// ���f���̊g��l���Z�b�g
extern int NS_MV1SetScale( int MHandle, VECTOR Scale )
{
	MV1_MODEL *Model ;

	// ����������Ă��Ȃ�������G���[
	if( MV1Man.Initialize == false ) return -1 ;

	// �A�h���X�擾
	if( MV1MDLCHK( MHandle, Model ) )
		return -1 ;

	// ���܂łƓ������W�������牽�������ɏI��
	if( *( ( DWORD * )&Model->Scale.x ) == *( ( DWORD * )&Scale.x ) &&
		*( ( DWORD * )&Model->Scale.y ) == *( ( DWORD * )&Scale.y ) &&
		*( ( DWORD * )&Model->Scale.z ) == *( ( DWORD * )&Scale.z ) )
		return 0 ;

	// �X�P�[���l�̃Z�b�g
	Model->Scale = Scale ;

	// �X�V�t���O�𗧂Ă�
	Model->LocalWorldMatrixSetupFlag = false ;
	if( ( Model->ChangeMatrixFlag[ 0 ] & 1 ) == 0 )
		_MEMSET( Model->ChangeMatrixFlag, 0xff, Model->ChangeMatrixFlagSize ) ;

	// �I��
	return 0 ;
}

// ���f���̊g��l���擾
extern VECTOR NS_MV1GetScale( int MHandle )
{
	MV1_MODEL *Model ;
	VECTOR NullPos = { 0.0f, 0.0f, 0.0f } ;

	// ����������Ă��Ȃ�������G���[
	if( MV1Man.Initialize == false ) return NullPos ;

	// �A�h���X�擾
	if( MV1MDLCHK( MHandle, Model ) )
		return NullPos ;

	// �X�P�[���l��Ԃ�
	return Model->Scale ;
}

// ���f���̉�]�l���Z�b�g( X����]��Y����]��Z����]���� )
extern int NS_MV1SetRotationXYZ( int MHandle, VECTOR Rotate )
{
	MV1_MODEL *Model ;

	// ����������Ă��Ȃ�������G���[
	if( MV1Man.Initialize == false ) return -1 ;

	// �A�h���X�擾
	if( MV1MDLCHK( MHandle, Model ) )
		return -1 ;

	// ���܂łƓ������W�������牽�������ɏI��
	if( Model->Rotation.Type == MV1_ROTATE_TYPE_XYZROT &&
		*( ( DWORD * )&Model->Rotation.XYZRot.x ) == *( ( DWORD * )&Rotate.x ) &&
		*( ( DWORD * )&Model->Rotation.XYZRot.y ) == *( ( DWORD * )&Rotate.y ) &&
		*( ( DWORD * )&Model->Rotation.XYZRot.z ) == *( ( DWORD * )&Rotate.z ) )
		return 0 ;

	// ��]�l�̃Z�b�g
	Model->Rotation.Type = MV1_ROTATE_TYPE_XYZROT ;
	Model->Rotation.XYZRot = Rotate ;

	// �X�V�t���O�𗧂Ă�
	Model->LocalWorldMatrixSetupFlag = false ;
	if( ( Model->ChangeMatrixFlag[ 0 ] & 1 ) == 0 )
		_MEMSET( Model->ChangeMatrixFlag, 0xff, Model->ChangeMatrixFlagSize ) ;

	// �I��
	return 0 ;
}

// ���f���̉�]�l���擾( X����]��Y����]��Z����]���� )
extern VECTOR NS_MV1GetRotationXYZ( int MHandle )
{
	MV1_MODEL *Model ;
	VECTOR NullPos = { 0.0f, 0.0f, 0.0f } ;

	// ����������Ă��Ȃ�������G���[
	if( MV1Man.Initialize == false ) return NullPos ;

	// �A�h���X�擾
	if( MV1MDLCHK( MHandle, Model ) )
		return NullPos ;

	// ��]�l��Ԃ�
	return Model->Rotation.XYZRot ;
}

// ���f���̉�]�l���Z�b�g( �y���̌�����������w�肷����� )
extern	int	NS_MV1SetRotationZYAxis( int MHandle, VECTOR ZAxisDirection, VECTOR YAxisDirection, float ZAxisTwistRotate )
{
	MV1_MODEL *Model ;

	// ����������Ă��Ȃ�������G���[
	if( MV1Man.Initialize == false ) return -1 ;

	// �A�h���X�擾
	if( MV1MDLCHK( MHandle, Model ) )
		return -1 ;

	// ��]�l�̃Z�b�g
	Model->Rotation.Type = MV1_ROTATE_TYPE_ZAXIS ;
	Model->Rotation.ZAxis.Z = VNorm( ZAxisDirection ) ;
	Model->Rotation.ZAxis.Up = VNorm( VCross( VCross( Model->Rotation.ZAxis.Z, YAxisDirection ), Model->Rotation.ZAxis.Z ) ) ; 
	Model->Rotation.ZAxis.Twist = ZAxisTwistRotate ;

	// �X�V�t���O�𗧂Ă�
	Model->LocalWorldMatrixSetupFlag = false ;
	if( ( Model->ChangeMatrixFlag[ 0 ] & 1 ) == 0 )
		_MEMSET( Model->ChangeMatrixFlag, 0xff, Model->ChangeMatrixFlagSize ) ;

	// �I��
	return 0 ;
}

// ���f���̂y���̕����𓾂�
extern	VECTOR MV1GetRotationZAxisZ( int MHandle )
{
	MV1_MODEL *Model ;
	VECTOR NullPos = { 0.0f, 0.0f, 0.0f } ;

	// ����������Ă��Ȃ�������G���[
	if( MV1Man.Initialize == false ) return NullPos ;

	// �A�h���X�擾
	if( MV1MDLCHK( MHandle, Model ) )
		return NullPos ;

	// ��������Ԃ�
	return Model->Rotation.ZAxis.Z ;
}

// ���f���̂y���̕������w�肵���ۂ̏�����x�N�g���𓾂�
extern VECTOR MV1GetRotationZAxisUp( int MHandle )
{
	MV1_MODEL *Model ;
	VECTOR NullPos = { 0.0f, 0.0f, 0.0f } ;

	// ����������Ă��Ȃ�������G���[
	if( MV1Man.Initialize == false ) return NullPos ;

	// �A�h���X�擾
	if( MV1MDLCHK( MHandle, Model ) )
		return NullPos ;

	// ��������Ԃ�
	return Model->Rotation.ZAxis.Up ;
}

// ���f���̂y���̕������w�肵���ۂ̔P��p�x�𓾂�
extern	float MV1GetRotationZAxisTwist( int MHandle )
{
	MV1_MODEL *Model ;

	// ����������Ă��Ȃ�������G���[
	if( MV1Man.Initialize == false ) return -1 ;

	// �A�h���X�擾
	if( MV1MDLCHK( MHandle, Model ) )
		return -1 ;

	// ��������Ԃ�
	return Model->Rotation.ZAxis.Twist ;
}

// ���f���̉�]�p�s����Z�b�g����
extern	int NS_MV1SetRotationMatrix( int MHandle, MATRIX Matrix )
{
	MV1_MODEL *Model ;

	// ����������Ă��Ȃ�������G���[
	if( MV1Man.Initialize == false ) return -1 ;

	// �A�h���X�擾
	if( MV1MDLCHK( MHandle, Model ) )
		return -1 ;

	// ���܂łƓ����������ꍇ�͉��������ɏI��
	if( Model->Rotation.Type == MV1_ROTATE_TYPE_MATRIX &&
		*( ( DWORD * )&Model->Rotation.Mat.m[ 0 ][ 0 ] ) == *( ( DWORD * )&Matrix.m[ 0 ][ 0 ] ) &&
		*( ( DWORD * )&Model->Rotation.Mat.m[ 0 ][ 1 ] ) == *( ( DWORD * )&Matrix.m[ 1 ][ 0 ] ) &&
		*( ( DWORD * )&Model->Rotation.Mat.m[ 0 ][ 2 ] ) == *( ( DWORD * )&Matrix.m[ 2 ][ 0 ] ) &&
		*( ( DWORD * )&Model->Rotation.Mat.m[ 1 ][ 0 ] ) == *( ( DWORD * )&Matrix.m[ 0 ][ 1 ] ) &&
		*( ( DWORD * )&Model->Rotation.Mat.m[ 1 ][ 1 ] ) == *( ( DWORD * )&Matrix.m[ 1 ][ 1 ] ) &&
		*( ( DWORD * )&Model->Rotation.Mat.m[ 1 ][ 2 ] ) == *( ( DWORD * )&Matrix.m[ 2 ][ 1 ] ) &&
		*( ( DWORD * )&Model->Rotation.Mat.m[ 2 ][ 0 ] ) == *( ( DWORD * )&Matrix.m[ 0 ][ 2 ] ) &&
		*( ( DWORD * )&Model->Rotation.Mat.m[ 2 ][ 1 ] ) == *( ( DWORD * )&Matrix.m[ 1 ][ 2 ] ) &&
		*( ( DWORD * )&Model->Rotation.Mat.m[ 2 ][ 2 ] ) == *( ( DWORD * )&Matrix.m[ 2 ][ 2 ] ) )
		return 0 ;

	// ��]�l�̃Z�b�g
	Model->Rotation.Type = MV1_ROTATE_TYPE_MATRIX ;
	ConvertMatrixFToMatrix4x4cF( &Model->Rotation.Mat, &Matrix ) ;
	Model->Rotation.Mat.m[ 0 ][ 3 ] = 0.0f ;
	Model->Rotation.Mat.m[ 1 ][ 3 ] = 0.0f ;
	Model->Rotation.Mat.m[ 2 ][ 3 ] = 0.0f ;

	// �X�V�t���O�𗧂Ă�
	Model->LocalWorldMatrixSetupFlag = false ;
	if( ( Model->ChangeMatrixFlag[ 0 ] & 1 ) == 0 )
		_MEMSET( Model->ChangeMatrixFlag, 0xff, Model->ChangeMatrixFlagSize ) ;

	// �I��
	return 0 ;
}

// ���f���̉�]�p�s����擾����
extern	MATRIX NS_MV1GetRotationMatrix( int MHandle )
{
	MV1_MODEL *Model ;
	static MATRIX NullMat = { 0.0f, 0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 0.0f, 0.0f } ;
	MATRIX ResultMatrix ;

	// ����������Ă��Ȃ�������G���[
	if( MV1Man.Initialize == false ) return NullMat ;

	// �A�h���X�擾
	if( MV1MDLCHK( MHandle, Model ) )
		return NullMat ;

	// ��]�s���Ԃ�
	ConvertMatrix4x4cFToMatrixF( &ResultMatrix, &Model->Rotation.Mat ) ;
	return ResultMatrix ;
}

// ���f���̕ό`�p�s����Z�b�g����
extern int NS_MV1SetMatrix( int MHandle, MATRIX Matrix )
{
	static const MATRIX IdentityMatrix =
	{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
	} ;
	MATRIX_4X4CT Matrix4X4CT ;
	bool ValidMatrix ;
	MV1_MODEL *Model ;

	// ����������Ă��Ȃ�������G���[
	if( MV1Man.Initialize == false ) return -1 ;

	// �A�h���X�擾
	if( MV1MDLCHK( MHandle, Model ) )
		return -1 ;

	// ���܂łƓ����ꍇ�͉��������ɏI��
	ValidMatrix = _MEMCMP( &IdentityMatrix, &Matrix, sizeof( MATRIX ) ) != 0 ;
	ConvertMatrixFToMatrix4x4c( &Matrix4X4CT, &Matrix ) ;
	if( ( Model->ValidMatrix == false && ValidMatrix == false ) ||
		( Model->ValidMatrix == true  && ValidMatrix == true  &&
		  _MEMCMP( &Matrix4X4CT, &Model->Matrix, sizeof( MATRIX_4X4CT ) ) == 0 ) )
	{
		return 0 ;
	}

	// �s��̃Z�b�g
	Model->ValidMatrix = ValidMatrix ;
	if( Model->ValidMatrix )
	{
		Model->Matrix = Matrix4X4CT ;
	}

	// �X�V�t���O�𗧂Ă�
	Model->LocalWorldMatrixSetupFlag = false ;
	if( ( Model->ChangeMatrixFlag[ 0 ] & 1 ) == 0 )
	{
		_MEMSET( Model->ChangeMatrixFlag, 0xff, Model->ChangeMatrixFlagSize ) ;
	}

	// �I��
	return 0 ;
}

// ���f���̕ό`�p�s����Z�b�g����
extern int NS_MV1SetMatrixD( int MHandle, MATRIX_D Matrix )
{
	static const MATRIX_D IdentityMatrix =
	{
		1.0, 0.0, 0.0, 0.0,
		0.0, 1.0, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		0.0, 0.0, 0.0, 1.0,
	} ;
	MATRIX_4X4CT Matrix4X4CT ;
	bool ValidMatrix ;
	MV1_MODEL *Model ;

	// ����������Ă��Ȃ�������G���[
	if( MV1Man.Initialize == false ) return -1 ;

	// �A�h���X�擾
	if( MV1MDLCHK( MHandle, Model ) )
		return -1 ;

	// ���܂łƓ����ꍇ�͉��������ɏI��
	ValidMatrix = _MEMCMP( &IdentityMatrix, &Matrix, sizeof( MATRIX_D ) ) != 0 ;
	ConvertMatrixDToMatrix4x4c( &Matrix4X4CT, &Matrix ) ;
	if( ( Model->ValidMatrix == false && ValidMatrix == false ) ||
		( Model->ValidMatrix == true  && ValidMatrix == true  &&
		  _MEMCMP( &Matrix4X4CT, &Model->Matrix, sizeof( MATRIX_4X4CT ) ) == 0 ) )
	{
		return 0 ;
	}

	// �s��̃Z�b�g
	Model->ValidMatrix = ValidMatrix ;
	if( Model->ValidMatrix )
	{
		Model->Matrix = Matrix4X4CT ;
	}

	// �X�V�t���O�𗧂Ă�
	Model->LocalWorldMatrixSetupFlag = false ;
	if( ( Model->ChangeMatrixFlag[ 0 ] & 1 ) == 0 )
	{
		_MEMSET( Model->ChangeMatrixFlag, 0xff, Model->ChangeMatrixFlagSize ) ;
	}

	// �I��
	return 0 ;
}

// ���f���̕ό`�p�s����擾����
extern MATRIX NS_MV1GetMatrix( int MHandle )
{
	MV1_MODEL *Model ;
	MATRIX ResultMatrix ;

	// ����������Ă��Ȃ�������G���[
	if( MV1Man.Initialize == false ) return MGetIdent() ;

	// �A�h���X�擾
	if( MV1MDLCHK( MHandle, Model ) )
		return MGetIdent() ;

	// �s���Ԃ�
	if( Model->ValidMatrix )
	{
		ConvertMatrix4x4cToMatrixF( &ResultMatrix, &Model->Matrix ) ;
	}
	else
	{
		MV1SETUPMATRIX( Model ) ;
		ConvertMatrix4x4cToMatrixF( &ResultMatrix, &Model->LocalWorldMatrix ) ;
	}

	return ResultMatrix ;
}

// ���f���̕ό`�p�s����擾����
extern MATRIX_D NS_MV1GetMatrixD( int MHandle )
{
	MV1_MODEL *Model ;
	MATRIX_D ResultMatrix ;

	// ����������Ă��Ȃ�������G���[
	if( MV1Man.Initialize == false ) return MGetIdentD() ;

	// �A�h���X�擾
	if( MV1MDLCHK( MHandle, Model ) )
		return MGetIdentD() ;

	// �s���Ԃ�
	if( Model->ValidMatrix )
	{
		ConvertMatrix4x4cToMatrixD( &ResultMatrix, &Model->Matrix ) ;
	}
	else
	{
		MV1SETUPMATRIX( Model ) ;
		ConvertMatrix4x4cToMatrixD( &ResultMatrix, &Model->LocalWorldMatrix ) ;
	}

	return ResultMatrix ;
}

// ���f���̕\���A��\����Ԃ�ύX����( TRUE:�\��  FALSE:��\�� )
extern int NS_MV1SetVisible( int MHandle, int VisibleFlag )
{
	MV1START( MHandle, Model, ModelBase, -1 ) ;

	// �l���Z�b�g
	if( MV1SetDrawMaterialVisible( &Model->DrawMaterial, NULL, ( BYTE )VisibleFlag ) )
	{
		// �`��ҋ@���Ă���`�敨��`��
		DRAWSTOCKINFO

		// �X�V�r�b�g�������Ă��Ȃ������痧�Ă�
		if( ( Model->ChangeDrawMaterialFlag[ 0 ] & 1 ) == 0 )
			_MEMSET( Model->ChangeDrawMaterialFlag, 0xff, Model->ChangeDrawMaterialFlagSize ) ;
	}

	// �I��
	return 0 ;
}

// ���f���̕\���A��\����Ԃ��擾����( TRUE:�\��  FALSE:��\�� )
extern int NS_MV1GetVisible( int MHandle )
{
	MV1START( MHandle, Model, ModelBase, -1 ) ;

	// �t���O��Ԃ�
	return Model->DrawMaterial.Visible ;
}

// ���f���̃��b�V���̎��( DX_MV1_MESHCATEGORY_NORMAL �Ȃ� )���̕\���A��\����ݒ肷��( TRUE:�\��  FALSE:��\�� )
extern int NS_MV1SetMeshCategoryVisible( int MHandle, int MeshCategory, int VisibleFlag )
{
	int HideFlag ;

	MV1START( MHandle, Model, ModelBase, -1 ) ;

	// �l�̗L���`�F�b�N
	if( MeshCategory < 0 || MeshCategory >= DX_MV1_MESHCATEGORY_NUM )
		return -1 ;

	HideFlag = VisibleFlag == FALSE ? TRUE : FALSE ;

	if( HideFlag == Model->MeshCategoryHide[ MeshCategory ] )
	{
		return 0 ;
	}

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �t���O��ۑ�
	Model->MeshCategoryHide[ MeshCategory ] = HideFlag ;

	// �I��
	return 0 ;
}

// ���f���̃��b�V���̎��( DX_MV1_MESHCATEGORY_NORMAL �Ȃ� )���̕\���A��\�����擾����( TRUE:�\��  FALSE:��\�� )
extern int NS_MV1GetMeshCategoryVisible( int MHandle, int MeshCategory )
{
	MV1START( MHandle, Model, ModelBase, -1 ) ;

	// �l�̗L���`�F�b�N
	if( MeshCategory < 0 || MeshCategory >= DX_MV1_MESHCATEGORY_NUM )
		return -1 ;

	// �t���O��Ԃ�
	return Model->MeshCategoryHide[ MeshCategory ] == FALSE ? TRUE : FALSE ;
}

// ���f���̃f�B�t���[�Y�J���[�̃X�P�[���l��ݒ肷��( �f�t�H���g�l�� 1.0f )
extern	int			NS_MV1SetDifColorScale( int MHandle, COLOR_F Scale )
{
	MV1START( MHandle, Model, ModelBase, -1 ) ;

	// �l���Z�b�g
	if( MV1SetDrawMaterialDif( &Model->DrawMaterial, NULL, Scale ) )
	{
		// �`��ҋ@���Ă���`�敨��`��
		DRAWSTOCKINFO

		// �������v�f�L�����̃Z�b�g�A�b�v�����t���O��|��
		Model->SemiTransStateSetupFlag = false ;

		// �X�V�r�b�g�������Ă��Ȃ������痧�Ă�
		if( ( Model->ChangeDrawMaterialFlag[ 0 ] & 1 ) == 0 )
			_MEMSET( Model->ChangeDrawMaterialFlag, 0xff, Model->ChangeDrawMaterialFlagSize ) ;
	}

	// �I��
	return 0 ;
}

// ���f���̃X�y�L�����J���[�̃X�P�[���l��ݒ肷��( �f�t�H���g�l�� 1.0f )
extern	int			NS_MV1SetSpcColorScale( int MHandle, COLOR_F Scale )
{
	MV1START( MHandle, Model, ModelBase, -1 ) ;

	// �l���Z�b�g
	if( MV1SetDrawMaterialSpc( &Model->DrawMaterial, NULL, Scale ) )
	{
		// �`��ҋ@���Ă���`�敨��`��
		DRAWSTOCKINFO

		// �X�V�r�b�g�������Ă��Ȃ������痧�Ă�
		if( ( Model->ChangeDrawMaterialFlag[ 0 ] & 1 ) == 0 )
			_MEMSET( Model->ChangeDrawMaterialFlag, 0xff, Model->ChangeDrawMaterialFlagSize ) ;
	}

	// �I��
	return 0 ;
}

// ���f���̃G�~�b�V�u�J���[�̃X�P�[���l��ݒ肷��( �f�t�H���g�l�� 1.0f )
extern	int			NS_MV1SetEmiColorScale( int MHandle, COLOR_F Scale )
{
	MV1START( MHandle, Model, ModelBase, -1 ) ;

	// �l���Z�b�g
	if( MV1SetDrawMaterialEmi( &Model->DrawMaterial, NULL, Scale ) )
	{
		// �`��ҋ@���Ă���`�敨��`��
		DRAWSTOCKINFO

		// �X�V�r�b�g�������Ă��Ȃ������痧�Ă�
		if( ( Model->ChangeDrawMaterialFlag[ 0 ] & 1 ) == 0 )
			_MEMSET( Model->ChangeDrawMaterialFlag, 0xff, Model->ChangeDrawMaterialFlagSize ) ;
	}

	// �I��
	return 0 ;
}

// ���f���̃A���r�G���g�J���[�̃X�P�[���l��ݒ肷��( �f�t�H���g�l�� 1.0f )
extern	int			NS_MV1SetAmbColorScale( int MHandle, COLOR_F Scale )
{
	MV1START( MHandle, Model, ModelBase, -1 ) ;

	// �l���Z�b�g
	if( MV1SetDrawMaterialAmb( &Model->DrawMaterial, NULL, Scale ) )
	{
		// �`��ҋ@���Ă���`�敨��`��
		DRAWSTOCKINFO

		// �X�V�r�b�g�������Ă��Ȃ������痧�Ă�
		if( ( Model->ChangeDrawMaterialFlag[ 0 ] & 1 ) == 0 )
			_MEMSET( Model->ChangeDrawMaterialFlag, 0xff, Model->ChangeDrawMaterialFlagSize ) ;
	}

	// �I��
	return 0 ;
}

// ���f���̃f�B�t���[�Y�J���[�̃X�P�[���l���擾����( �f�t�H���g�l�� 1.0f )
extern	COLOR_F		NS_MV1GetDifColorScale( int MHandle )
{
	COLOR_F ErrorResult = { -1.0f, -1.0f, -1.0f, -1.0f } ;
	MV1START( MHandle, Model, ModelBase, ErrorResult ) ;

	// �l��Ԃ�
	return Model->DrawMaterial.DiffuseScale ;
}

// ���f���̃X�y�L�����J���[�̃X�P�[���l���擾����( �f�t�H���g�l�� 1.0f )
extern	COLOR_F		NS_MV1GetSpcColorScale( int MHandle )
{
	COLOR_F ErrorResult = { -1.0f, -1.0f, -1.0f, -1.0f } ;
	MV1START( MHandle, Model, ModelBase, ErrorResult ) ;

	// �l��Ԃ�
	return Model->DrawMaterial.SpecularScale ;
}

// ���f���̃G�~�b�V�u�J���[�̃X�P�[���l���擾����( �f�t�H���g�l�� 1.0f )
extern	COLOR_F		NS_MV1GetEmiColorScale( int MHandle )
{
	COLOR_F ErrorResult = { -1.0f, -1.0f, -1.0f, -1.0f } ;
	MV1START( MHandle, Model, ModelBase, ErrorResult ) ;

	// �l��Ԃ�
	return Model->DrawMaterial.EmissiveScale ;
}

// ���f���̃A���r�G���g�J���[�̃X�P�[���l���擾����( �f�t�H���g�l�� 1.0f )
extern	COLOR_F		NS_MV1GetAmbColorScale( int MHandle )
{
	COLOR_F ErrorResult = { -1.0f, -1.0f, -1.0f, -1.0f } ;
	MV1START( MHandle, Model, ModelBase, ErrorResult ) ;

	// �l��Ԃ�
	return Model->DrawMaterial.AmbientScale ;
}

// ���f���ɔ������v�f�����邩�ǂ������擾����( �߂�l TRUE:����  FALSE:�Ȃ� )
extern int NS_MV1GetSemiTransState( int MHandle )
{
	int i ;
	MV1START( MHandle, Model, ModelBase, -1 ) ;

	// �������v�f�L�����̃Z�b�g�A�b�v���������Ă��Ȃ���΃Z�b�g�A�b�v������
	if( Model->SemiTransStateSetupFlag == false )
	{
		// �g�b�v�t���[���̒��ɔ������v�f���������t���[������ł�����Δ������v�f�����������f���Ƃ�������
		Model->SemiTransState = false ;

		for( i = 0 ; i < Model->TopFrameNum ; i ++ )
		{
			if( NS_MV1GetFrameSemiTransState( MHandle, ( int )( Model->TopFrameList[ i ] - Model->Frame ) ) )
				Model->SemiTransState = true ;
		}

		// �Z�b�g�A�b�v����
		Model->SemiTransStateSetupFlag = true ;
	}

	// �Ԃ�
	return Model->SemiTransState ;
}

// ���f���̕s�����x��ݒ肷��( �s���� 1.0f �` ���� 0.0f )
extern int NS_MV1SetOpacityRate( int MHandle, float Rate )
{
	MV1START( MHandle, Model, ModelBase, -1 ) ;

	// �͈͂𒴂��Ă�����␳����
	     if( Rate < 0.0f ) Rate = 0.0f ;
	else if( Rate > 1.0f ) Rate = 1.0f ;

	// �l���Z�b�g
	if( MV1SetDrawMaterialOpacityRate( &Model->DrawMaterial, NULL, Rate ) )
	{
		// �`��ҋ@���Ă���`�敨��`��
		DRAWSTOCKINFO

		// �������v�f�L�����̃Z�b�g�A�b�v�����t���O��|��
		Model->SemiTransStateSetupFlag = false ;

		// �X�V�r�b�g�������Ă��Ȃ������痧�Ă�
		if( ( Model->ChangeDrawMaterialFlag[ 0 ] & 1 ) == 0 )
			_MEMSET( Model->ChangeDrawMaterialFlag, 0xff, Model->ChangeDrawMaterialFlagSize ) ;
	}

	// �I��
	return 0 ;
}

// ���f���̕s�����x���擾����( �s���� 1.0f �` ���� 0.0f )
extern	float		NS_MV1GetOpacityRate( int MHandle )
{
	MV1START( MHandle, Model, ModelBase, -1.0f ) ;

	// �s�����x��Ԃ�
	return Model->DrawMaterial.OpacityRate ;
}

// ���f����`�悷��ۂ�RGB�l�ɑ΂���A�l����Z���邩�ǂ�����ݒ肷��
// ( �`�挋�ʂ���Z�ς݃A���t�@�摜�ɂȂ�܂� )( Flag   TRUE:RGB�l�ɑ΂���A�l����Z����  FALSE:��Z���Ȃ�(�f�t�H���g) )
extern int NS_MV1SetUseDrawMulAlphaColor( int MHandle, int Flag )
{
	bool BoolFlag ;

	MV1START( MHandle, Model, ModelBase, -1 ) ;

	BoolFlag = Flag != FALSE ;

	if( Model->UseDrawMulAlphaColor == BoolFlag )
	{
		return 0 ;
	}

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �l��ۑ�
	Model->UseDrawMulAlphaColor = BoolFlag ;

	// �I��
	return 0 ;
}

// ���f����`�悷��ۂ�RGB�l�ɑ΂���A�l����Z���邩�ǂ������擾����
// ( �`�挋�ʂ���Z�ς݃A���t�@�摜�ɂȂ�܂� )( �߂�l TRUE:RGB�l�ɑ΂���A�l����Z����  FALSE:��Z���Ȃ�(�f�t�H���g) )
extern int NS_MV1GetUseDrawMulAlphaColor( int MHandle )
{
	MV1START( MHandle, Model, ModelBase, -1 ) ;

	return Model->UseDrawMulAlphaColor ? TRUE : FALSE ;
}

// ���f����`�悷��ۂɂy�o�b�t�@���g�p���邩�ǂ�����ݒ肷��
extern int NS_MV1SetUseZBuffer( int MHandle, int Flag )
{
	MV1START( MHandle, Model, ModelBase, -1 ) ;

	if( Model->EnableZBufferFlag == Flag )
	{
		return 0 ;
	}

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	Model->EnableZBufferFlag = Flag ;

	// �I��
	return 0 ;
}

// ���f����`�悷��ۂɂy�o�b�t�@�ɏ������݂��s�����ǂ�����ݒ肷��
extern int NS_MV1SetWriteZBuffer( int MHandle, int Flag )
{
	MV1START( MHandle, Model, ModelBase, -1 ) ;

	if( Model->WriteZBufferFlag == Flag )
	{
		return 0 ;
	}

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	Model->WriteZBufferFlag = Flag ;

	// �I��
	return 0 ;
}

// ���f���̕`�掞�̂y�l�̔�r���[�h��ݒ肷��
extern int NS_MV1SetZBufferCmpType( int MHandle, int CmpType /* DX_CMP_NEVER �� */ )
{
	MV1START( MHandle, Model, ModelBase, -1 ) ;

	if( Model->ZBufferCmpType == CmpType )
	{
		return 0 ;
	}

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	Model->ZBufferCmpType = CmpType ;

	// �I��
	return 0 ;
}

// ���f���̕`�掞�̏������ނy�l�̃o�C�A�X��ݒ肷��
extern int NS_MV1SetZBias( int MHandle, int Bias )
{
	MV1START( MHandle, Model, ModelBase, -1 ) ;

	if( Model->ZBias == Bias )
	{
		return 0 ;
	}

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	Model->ZBias = Bias ;

	// �I��
	return 0 ;
}

// ���f���̊܂܂�郁�b�V���̒��_�f�B�t���[�Y�J���[���}�e���A���̃f�B�t���[�Y�J���[�̑���Ɏg�p���邩�ǂ�����ݒ肷��( TRUE:�}�e���A���J���[�̑���Ɏg�p����  FALSE:�}�e���A���J���[���g�p���� )
extern int NS_MV1SetUseVertDifColor( int MHandle, int UseFlag )
{
	int i ;

	MV1START( MHandle, Model, ModelBase, -1 ) ;

	// ���ׂẴ��b�V���ɐݒ肷��
	for( i = 0 ; i < ModelBase->MeshNum ; i ++ )
		NS_MV1SetMeshUseVertDifColor( MHandle, i, UseFlag ) ;
	
	// �I��
	return 0 ;
}

// ���f���Ɋ܂܂�郁�b�V���̒��_�X�y�L�����J���[���}�e���A���̃X�y�L�����J���[�̑���Ɏg�p���邩�ǂ�����ݒ肷��( TRUE:�}�e���A���J���[�̑���Ɏg�p����  FALSE:�}�e���A���J���[���g�p���� )
extern int NS_MV1SetUseVertSpcColor( int MHandle, int UseFlag )
{
	int i ;

	MV1START( MHandle, Model, ModelBase, -1 ) ;

	// ���ׂẴ��b�V���ɐݒ肷��
	for( i = 0 ; i < ModelBase->MeshNum ; i ++ )
		NS_MV1SetMeshUseVertSpcColor( MHandle, i, UseFlag ) ;
	
	// �I��
	return 0 ;
}

// ���f���̃T���v���t�B���^�[���[�h��ύX����
extern int NS_MV1SetSampleFilterMode( int MHandle, int FilterMode )
{
	int i ;

	MV1START( MHandle, Model, ModelBase, -1 ) ;

	// �S�Ẵe�N�X�`���̃t�B���^�[���[�h��ύX����
	for( i = 0 ; i < ModelBase->TextureNum ; i ++ )
	{
		NS_MV1SetTextureSampleFilterMode( MHandle, i, FilterMode ) ;
	}

	// �I��
	return 0 ;
}

// ���f���ٕ̈����t�B���^�����O�̍ő原����ݒ肷��
extern int NS_MV1SetMaxAnisotropy( int MHandle, int MaxAnisotropy )
{
	MV1START( MHandle, Model, ModelBase, -1 ) ;

	if( Model->MaxAnisotropy == MaxAnisotropy )
	{
		return 0 ;
	}

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	Model->MaxAnisotropy = MaxAnisotropy ;

	// �I��
	return 0 ;
}

// ���f�������C���[�t���[���ŕ`�悷�邩�ǂ�����ݒ肷��
extern int NS_MV1SetWireFrameDrawFlag( int MHandle, int Flag )
{
	bool WireFrameFlag ;

	MV1START( MHandle, Model, ModelBase, -1 ) ;

	WireFrameFlag = Flag != 0 ;

	if( WireFrameFlag == Model->WireFrame )
	{
		return 0 ;
	}

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	Model->WireFrame = WireFrameFlag ;

	// �I��
	return 0 ;
}

// ���f���̒��_�J���[�����ݐݒ肳��Ă���}�e���A���̃J���[�ɂ���
extern int NS_MV1RefreshVertColorFromMaterial( int MHandle )
{
	MV1_MESH_BASE *MBMesh ;
	MV1_MESH *Mesh ;
	MV1_MESH_VERTEX *MBVert ;
	MV1_TRIANGLE_LIST_BASE *MBTList ;
	int i, j ;
	int VertexNum ;
	COLOR_U8 DiffuseColor ;
	COLOR_U8 SpecularColor ;

	MV1START( MHandle, Model, ModelBase, -1 ) ;

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	MBMesh = ModelBase->Mesh ;
	Mesh = Model->Mesh ;
	for( i = 0 ; i < ModelBase->MeshNum ; i ++, MBMesh ++, Mesh ++ )
	{
		if( MBMesh->Vertex == NULL ) continue ;

		VertexNum = MBMesh->VertexNum ;
		MBVert = MBMesh->Vertex ;
		DiffuseColor.r = ( BYTE )( Mesh->Material->Diffuse.r >= 1.0f ? 255 : ( Mesh->Material->Diffuse.r < 0.0f ? 0 : _FTOL( Mesh->Material->Diffuse.r * 255.0f ) ) ) ;
		DiffuseColor.g = ( BYTE )( Mesh->Material->Diffuse.g >= 1.0f ? 255 : ( Mesh->Material->Diffuse.g < 0.0f ? 0 : _FTOL( Mesh->Material->Diffuse.g * 255.0f ) ) ) ;
		DiffuseColor.b = ( BYTE )( Mesh->Material->Diffuse.b >= 1.0f ? 255 : ( Mesh->Material->Diffuse.b < 0.0f ? 0 : _FTOL( Mesh->Material->Diffuse.b * 255.0f ) ) ) ;
		DiffuseColor.a = ( BYTE )( Mesh->Material->Diffuse.a >= 1.0f ? 255 : ( Mesh->Material->Diffuse.a < 0.0f ? 0 : _FTOL( Mesh->Material->Diffuse.a * 255.0f ) ) ) ;
		SpecularColor.r = ( BYTE )( Mesh->Material->Specular.r >= 1.0f ? 255 : ( Mesh->Material->Specular.r < 0.0f ? 0 : _FTOL( Mesh->Material->Specular.r * 255.0f ) ) ) ;
		SpecularColor.g = ( BYTE )( Mesh->Material->Specular.g >= 1.0f ? 255 : ( Mesh->Material->Specular.g < 0.0f ? 0 : _FTOL( Mesh->Material->Specular.g * 255.0f ) ) ) ;
		SpecularColor.b = ( BYTE )( Mesh->Material->Specular.b >= 1.0f ? 255 : ( Mesh->Material->Specular.b < 0.0f ? 0 : _FTOL( Mesh->Material->Specular.b * 255.0f ) ) ) ;
		SpecularColor.a = ( BYTE )( Mesh->Material->Specular.a >= 1.0f ? 255 : ( Mesh->Material->Specular.a < 0.0f ? 0 : _FTOL( Mesh->Material->Specular.a * 255.0f ) ) ) ;
		for( j = 0 ; j < VertexNum ; j ++, MBVert ++ )
		{
			MBVert->DiffuseColor = DiffuseColor ;
			MBVert->SpecularColor = SpecularColor ;
		}
	}

	// �e���|�����o�b�t�@������ꍇ�͈�x�S�ĉ������
	MBTList = ModelBase->TriangleList ;
	for( i = 0 ; i < ModelBase->TriangleListNum ; i ++, MBTList ++ )
	{
		// ���ˑ��̃e���|�����o�b�t�@���J��
		MV1_TerminateTriangleListBaseTempBuffer_PF( MBTList ) ;
	}

	// ���_�o�b�t�@�̍�蒼��
	MV1_TerminateVertexBufferBase_PF( ModelBase->HandleInfo.Handle ) ;
	MV1_SetupVertexBufferBase_PF( ModelBase->HandleInfo.Handle ) ;

	// �I��
	return 0 ;
}

// ���f���̕������Z�̏d�͂�ݒ肷��
extern int NS_MV1SetPhysicsWorldGravity( int MHandle, VECTOR Gravity )
{
#ifndef DX_NON_BULLET_PHYSICS
	MV1START( MHandle, Model, ModelBase, -1 ) ;

	// �����f�[�^�������ꍇ�͉������Ȃ�
	if( Model->BaseData->PhysicsRigidBodyNum == 0 )
		return 0 ;

	SetWorldGravity_ModelPhysiceInfo( Model, Gravity ) ;
#endif

	// �I��
	return 0 ;
}

// ���f���̕������Z���w�莞�ԕ��o�߂����Ɖ��肵�Čv�Z����( MillisecondTime �Ŏw�肷�鎞�Ԃ̒P�ʂ̓~���b )
int NS_MV1PhysicsCalculation( int MHandle, float MillisecondTime )
{
	return MV1PhysicsCalculationBase( MHandle, MillisecondTime, GetASyncLoadFlag() ) ;
}

// ���f���̕������Z���w�莞�ԕ��o�߂����Ɖ��肵�Čv�Z����( MillisecondTime �Ŏw�肷�鎞�Ԃ̒P�ʂ̓~���b )
static int MV1PhysicsCalculationBase_Static( int MHandle, float MillisecondTime, int ASyncThread )
{
#ifndef DX_NON_BULLET_PHYSICS
	MV1_PHYSICS_RIGIDBODY *PhysicsRigidBody ;
	int i ;
	MV1_MODEL *Model ;

	// ����������Ă��Ȃ�������G���[
	if( MV1Man.Initialize == false )
	{
		return -1 ;
	}

	// �A�h���X�擾
	if( ASyncThread )
	{
		if( MV1MDLCHK_ASYNC( MHandle, Model ) )
		{
			return -1 ;
		}
	}
	else
	{
		if( MV1MDLCHK( MHandle, Model ) )
		{
			return -1 ;
		}
	}

	// �����f�[�^�������ꍇ�͉������Ȃ�
	if( Model->BaseData->PhysicsRigidBodyNum == 0 )
	{
		return 0 ;
	}

	MV1SetupMatrix( Model ) ;

	StepSimulation_ModelPhysicsInfo( Model, MillisecondTime / 1000.0f ) ;

	// �X�V�����s��Ɋւ��X�L�j���O�E�F�C�g�{�[���̍s����X�V
	PhysicsRigidBody = Model->PhysicsRigidBody ;
	for( i = 0 ; i < Model->BaseData->PhysicsRigidBodyNum ; i ++, PhysicsRigidBody ++ )
	{
		if( PhysicsRigidBody->BaseData->RigidBodyType == 0 || PhysicsRigidBody->BaseData->NoCopyToBone ) continue ;

		SetupSkiningBoneMatrix( *Model, *PhysicsRigidBody->TargetFrame ) ;
	}
#endif

	// �I��
	return 0 ;
}

#ifndef DX_NON_ASYNCLOAD

// CreateGraph �̔񓯊��ǂݍ��݃X���b�h����Ă΂��֐�
static void MV1PhysicsCalculationBase_ASync( ASYNCLOADDATA_COMMON *AParam )
{
	int MHandle ;
	float MillisecondTime ;
	int Addr ;
	int Result ;

	Addr = 0 ;
	MHandle         = GetASyncLoadParamInt(   AParam->Data, &Addr ) ;
	MillisecondTime = GetASyncLoadParamFloat( AParam->Data, &Addr ) ;

	Result = MV1PhysicsCalculationBase_Static( MHandle, MillisecondTime, TRUE ) ;
	DecASyncLoadCount( MHandle ) ;
}

#endif // DX_NON_ASYNCLOAD

// ���f���̕������Z���w�莞�ԕ��o�߂����Ɖ��肵�Čv�Z����( MillisecondTime �Ŏw�肷�鎞�Ԃ̒P�ʂ̓~���b )
extern int MV1PhysicsCalculationBase( int MHandle, float MillisecondTime, int ASyncLoadFlag )
{
#ifndef DX_NON_ASYNCLOAD
	if( ASyncLoadFlag )
	{
		ASYNCLOADDATA_COMMON *AParam = NULL ;
		int Addr ;

		// �p�����[�^�ɕK�v�ȃ������̃T�C�Y���Z�o
		Addr = 0 ;
		AddASyncLoadParamInt(   NULL, &Addr, MHandle ) ;
		AddASyncLoadParamFloat( NULL, &Addr, MillisecondTime ) ;

		// �������̊m��
		AParam = AllocASyncLoadDataMemory( Addr ) ;
		if( AParam == NULL )
			goto ERR ;

		// �����ɕK�v�ȏ����Z�b�g
		AParam->ProcessFunction = MV1PhysicsCalculationBase_ASync ;
		Addr = 0 ;
		AddASyncLoadParamInt(   AParam->Data, &Addr, MHandle ) ;
		AddASyncLoadParamFloat( AParam->Data, &Addr, MillisecondTime ) ;

		// �f�[�^��ǉ�
		if( AddASyncLoadData( AParam ) < 0 )
		{
			DXFREE( AParam ) ;
			AParam = NULL ;
			goto ERR ;
		}

		// �񓯊��ǂݍ��݃J�E���g���C���N�������g
		IncASyncLoadCount( MHandle, AParam->Index ) ;
	}
	else
#endif // DX_NON_ASYNCLOAD
	{
		if( MV1PhysicsCalculationBase_Static( MHandle, MillisecondTime, FALSE ) < 0 )
		{
			goto ERR ;
		}
	}

	// ����I��
	return 0 ;

ERR :
	return -1 ;
}

// ���f���̕������Z�̏�Ԃ����Z�b�g����( �ʒu�����[�v�����Ƃ��p )
extern int NS_MV1PhysicsResetState( int MHandle )
{
#ifndef DX_NON_BULLET_PHYSICS
	MV1START( MHandle, Model, ModelBase, -1 ) ;

	// �����f�[�^�������ꍇ�͉������Ȃ�
	if( Model->BaseData->PhysicsRigidBodyNum == 0 )
		return 0 ;

	MV1SetupMatrix( Model ) ;
	ResetState_ModelPhysicsInfo( Model ) ;
#endif

	// �I��
	return 0 ;
}

// ���f���̃V�F�C�v�@�\���g�p���邩�ǂ�����ݒ肷��
extern int NS_MV1SetUseShapeFlag( int MHandle, int Flag )
{
	int i ;
	MV1_FRAME *Frame ;

	MV1START( MHandle, Model, ModelBase, -1 ) ;

	// �t���O�������ꍇ�͉������Ȃ�
	if( Model->ShapeDisableFlag == ( Flag == 0 ) )
		return 0 ;

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �t���O��ۑ�����
	Model->ShapeDisableFlag = ( Flag == 0 ) ;

	// �V�F�C�v�̏�Ԃɕω����������t���O�𗧂Ă�
	Model->ShapeChangeFlag = true ;
	Frame = Model->Frame ;
	for( i = 0 ; i < ModelBase->FrameNum ; i ++, Frame ++ )
	{
		if( Frame->BaseData->MeshNum > 0 && Frame->Mesh->BaseData->Shape == 0 )
			continue ;

		Frame->ShapeChangeFlag = true ;
	}

	// �I��
	return 0 ;
}

// ���f���̃}�e���A���ԍ����Ƀ��b�V����`�悷�邩�ǂ����̃t���O���擾����( TRUE:�}�e���A���ԍ����ɕ`��  FALSE:�s�������b�V���̌㔼�������b�V�� )
extern int NS_MV1GetMaterialNumberOrderFlag( int MHandle )
{
	MV1START( MHandle, Model, ModelBase, -1 ) ;

	// �t���O��Ԃ�
	return ModelBase->MaterialNumberOrderDraw ;
}






















// ���C�g�̐����擾����
extern int MV1GetLightNum( int MHandle )
{
	MV1_MODEL *Model ;
	MV1_MODEL_BASE *ModelBase ;

	// ����������Ă��Ȃ�������G���[
	if( MV1Man.Initialize == false ) return -1 ;

	// �A�h���X�擾
	if( MV1MDLCHK( MHandle, Model ) )
		return -1 ;
	ModelBase = Model->BaseData ;

	// ���C�g�̐���Ԃ�
	return ModelBase->LightNum ;
}

// �A�j���[�V�������A�^�b�`����
extern int NS_MV1AttachAnim( int MHandle, int AnimIndex, int AnimSrcMHandle, int NameCheck )
{
	MV1_MODEL *Model, *AModel ;
	MV1_MODEL_BASE *MBase, *AMBase ;
	MV1_ANIMSET_BASE *AnimSetBase ;
	MV1_ANIM_BASE **ABaseTable ;
	MV1_MODEL_ANIM *MAnim, *Dest, *Src ;
	MV1_MODEL_ANIMSET *MAnimSet ;
	MV1_FRAME *Frame, *AFrame = NULL, *CFrame, *CAFrame = NULL ;
	int i, j, k, l, UnitSize, MaxNum, NewMaxNum, AttachIndex, Count ;
	MV1_FRAME *FrameStack[ 1024 ], *AFrameStack[ 1024 ] ;
	int StackCount[ 1024 ] ;
	int StackNum ;
	MV1_ANIM_BASE *AAnimBase ;
	MV1_ANIM_KEYSET_BASE *AKeySetBase ;
	MV1_FRAME_BASE *AFrameBase, *FrameBase ;
	const wchar_t *AShapeName ;

	// ����������Ă��Ȃ�������G���[
	if( MV1Man.Initialize == false ) return -1 ;

	// �A�h���X�擾
	if( MV1MDLCHK( MHandle, Model ) )
		return -1 ;
	MBase = Model->BaseData ;
	if( AnimSrcMHandle < 0 )
	{
		AModel = Model ;
		AMBase = AModel->BaseData ;
		NameCheck = FALSE ;
	}
	else
	{
		if( MV1MDLCHK( AnimSrcMHandle, AModel ) )
			return -1 ;
		AMBase = AModel->BaseData ;
	}

	// �C���f�b�N�X���s����������G���[
	if( AnimIndex < 0 || AnimIndex >= AMBase->AnimSetNum )
		return -1 ;

	// �w��̃A�j���[�V�������擾����
	AnimSetBase = MV1GetAnimSetBase( AModel->BaseDataHandle, NULL, AnimIndex ) ;
	if( AnimSetBase == NULL ) return -1 ;
	ABaseTable = AMBase->AnimTargetFrameTable + AMBase->FrameNum * AnimIndex ;

	// �f�[�^�|�C���^�z��g��
	if( Model->AnimSetNum >= Model->AnimSetMaxNum )
	{
		UnitSize       = ( int )( sizeof( MV1_MODEL_ANIMSET ) + sizeof( MV1_MODEL_ANIM ) * MBase->FrameNum ) ;
		Model->AnimSet = ( MV1_MODEL_ANIMSET * )DXREALLOC( Model->AnimSet, ( size_t )( UnitSize * ( Model->AnimSetMaxNum + MV1_ANIMSET_NUM_UNIT ) ) ) ;
		if( Model->AnimSet == NULL ) return -1 ;
		
		// �V���Ɋm�ۂ��ꂽ�̈�ɍ��܂ł̏����R�s�[
		MaxNum = Model->AnimSetMaxNum ;
		NewMaxNum = MaxNum + MV1_ANIMSET_NUM_UNIT ;

		// ������ړ�
		Src  = ( MV1_MODEL_ANIM * )( Model->AnimSet + MaxNum    ) + MaxNum    * ( MBase->FrameNum - 1 ) ;
		Dest = ( MV1_MODEL_ANIM * )( Model->AnimSet + NewMaxNum ) + NewMaxNum * ( MBase->FrameNum - 1 ) ;
		for( i = 0 ; i < MBase->FrameNum ; i ++ )
		{
			for( j = NewMaxNum - 1 ; j >= MaxNum ; j -- )
				_MEMSET( &Dest[ j ], 0, sizeof( MV1_MODEL_ANIM ) ) ;

			for( j = MaxNum    - 1 ; j >=      0 ; j -- )
				Dest[ j ] = Src[ j ] ;

			Dest -= NewMaxNum ;
			Src  -= MaxNum    ;
		}
		for( i = NewMaxNum - 1 ; i >= MaxNum ; i -- )
			_MEMSET( &Model->AnimSet[ i ], 0, sizeof( MV1_MODEL_ANIMSET ) ) ;

		// �V���Ɋm�ۂ����A�h���X���Z�b�g
		Model->Anim = ( MV1_MODEL_ANIM * )( Model->AnimSet + NewMaxNum ) ;

		// �ő吔�����Z
		Model->AnimSetMaxNum += MV1_ANIMSET_NUM_UNIT ;
	}

	// �g�p����Ă��Ȃ��A�^�b�`�z���_��T��
	for( AttachIndex = 0 ; Model->AnimSet[ AttachIndex ].Use ; AttachIndex ++ ){}

	// �A�h���X���Z�b�g
	MAnimSet = &Model->AnimSet[ AttachIndex ] ;

	// �A�j���[�V�����Z�b�g�̍쐬
	MAnimSet->AnimSet = MV1CreateAnimSet( AnimSetBase ) ; 
	if( MAnimSet->AnimSet == NULL ) return -1 ;

	// �c��̏���������
	MAnimSet->Use = true ;
	MAnimSet->DisableShapeFlag = false ;
	MAnimSet->BaseDataHandle = AModel->BaseDataHandle ;
	MAnimSet->BaseDataAnimIndex = AnimSetBase->Index ;
	MAnimSet->UseAnimNum = 0 ;

	// �e�t���[���̃A�j���[�V�������̏�����
	MAnim = &Model->Anim[ AttachIndex ] ;
	for( i = 0 ; i < MBase->FrameNum ; i ++, MAnim += Model->AnimSetMaxNum )
	{
		MAnim->Anim = NULL ;
		MAnim->BlendRate = 1.0f ;
		MAnim->Use = false ;
	}

	// ���O�݂̂Ō������邩�ǂ����ŏ����𕪊�
/*	if( MBase->AnimAttachNameSearch )
	{
		// �e�t���[���̃A�j���[�V�����̃A�h���X���Z�b�g
		Frame = Model->Frame ;
		MAnim = &Model->Anim[ AttachIndex ] ;
		for( i = 0 ; i < MBase->FrameNum ; i ++, Frame ++, MAnim += Model->AnimSetMaxNum )
		{
			MAnim->Anim = MV1GetAnimSetAnim( MAnimSet->AnimSet, Frame->BaseData->Name ) ;
			MAnim->Use = MAnim->Anim != NULL ;
			if( MAnim->Use )
			{
				MAnim->Anim->Frame = Frame ;
				MAnimSet->UseAnimNum ++ ;
			}
		}
	}
	else*/
	// ���O���`�F�b�N���邩�ǂ����ŏ����𕪊�
	if( NameCheck == FALSE )
	{
		// �`�F�b�N���Ȃ�

		for( i = 0 ; i < Model->TopFrameNum && i < AModel->TopFrameNum ; i ++ )
		{
			Frame = Model->TopFrameList[ i ] ;
			AFrame = AModel->TopFrameList[ i ] ;
			if( ABaseTable[ AFrame->BaseData->Index ] != NULL )
			{
				MAnim = &Model->Anim[ AttachIndex ] + Model->AnimSetMaxNum * Frame->BaseData->Index ;
				MAnim->Use = true ;
				MAnim->Anim = MAnimSet->AnimSet->Anim + ( ABaseTable[ AFrame->BaseData->Index ] - AnimSetBase->Anim ) ;
				MAnim->Anim->Frame = Frame ;

				AAnimBase = MAnim->Anim->BaseData ;
				AKeySetBase = AAnimBase->KeySet ;
				for( k = 0 ; k < AAnimBase->KeySetNum ; k ++, AKeySetBase ++ )
				{
					if( AKeySetBase->DataType != MV1_ANIMKEY_DATATYPE_SHAPE ) continue ;
					MAnim->Anim->KeySet[ k ].ShapeTargetIndex = AKeySetBase->TargetShapeIndex ;
				}

				MAnimSet->UseAnimNum ++ ;
			}

			FrameStack[ 0 ] = Frame ;
			AFrameStack[ 0 ] = AFrame ;
			StackCount[ 0 ] = 0 ;
			StackNum = 1 ;
			while( StackNum != 0 )
			{
				Frame = FrameStack[ StackNum - 1 ] ;
				AFrame = AFrameStack[ StackNum - 1 ] ;

				Count = StackCount[ StackNum - 1 ] ;
				if( Count < Frame->ChildNum && Count < AFrame->ChildNum )
				{
					StackCount[ StackNum - 1 ] ++ ;

					CFrame = Frame->ChildList[ Count ] ;
					CAFrame = AFrame->ChildList[ Count ] ;
					if( ABaseTable[ CAFrame->BaseData->Index ] )
					{
						MAnim = &Model->Anim[ AttachIndex ] + Model->AnimSetMaxNum * CFrame->BaseData->Index ;
						MAnim->Use = true ;
						MAnim->Anim = MAnimSet->AnimSet->Anim + ( ABaseTable[ CAFrame->BaseData->Index ] - AnimSetBase->Anim ) ;
						MAnim->Anim->Frame = CFrame ;

						AAnimBase = MAnim->Anim->BaseData ;
						AKeySetBase = AAnimBase->KeySet ;
						for( k = 0 ; k < AAnimBase->KeySetNum ; k ++, AKeySetBase ++ )
						{
							if( AKeySetBase->DataType != MV1_ANIMKEY_DATATYPE_SHAPE ) continue ;
							MAnim->Anim->KeySet[ k ].ShapeTargetIndex = AKeySetBase->TargetShapeIndex ;
						}

						MAnimSet->UseAnimNum ++ ;
					}

					FrameStack[ StackNum ] = CFrame ;
					AFrameStack[ StackNum ] = CAFrame ;
					StackCount[ StackNum ] = 0 ;
					StackNum ++ ;
				}
				else
				{
					StackNum -- ;
				}
			}
		}
	}
	else
	{
		// ���O���`�F�b�N����

		for( i = 0 ; i < Model->TopFrameNum ; i ++ )
		{
			Frame = Model->TopFrameList[ i ] ;
			FrameBase = Frame->BaseData ;

			for( j = 0 ; j < AModel->TopFrameNum ; j ++ )
			{
				AFrame = AModel->TopFrameList[ j ] ;
				if( _WCSCMP( Frame->BaseData->NameW, AFrame->BaseData->NameW ) == 0 )
					break ;
			}
			if( j == AModel->TopFrameNum ) continue ;

			if( ABaseTable[ AFrame->BaseData->Index ] )
			{
				MAnim = &Model->Anim[ AttachIndex ] + Model->AnimSetMaxNum * Frame->BaseData->Index ;
				MAnim->Use = true ;
				MAnim->Anim = MAnimSet->AnimSet->Anim + ( ABaseTable[ AFrame->BaseData->Index ] - AnimSetBase->Anim ) ;
				MAnim->Anim->Frame = Frame ;

				AAnimBase = MAnim->Anim->BaseData ;
				AKeySetBase = AAnimBase->KeySet ;
				for( k = 0 ; k < AAnimBase->KeySetNum ; k ++, AKeySetBase ++ )
				{
					if( AKeySetBase->DataType != MV1_ANIMKEY_DATATYPE_SHAPE ) continue ;

					AFrameBase = &AMBase->Frame[ AAnimBase->TargetFrameIndex ] ;
					AShapeName = AFrameBase->Shape[ AKeySetBase->TargetShapeIndex ].NameW ;
					for( l = 0 ; l < FrameBase->ShapeNum && _WCSCMP( FrameBase->Shape[ l ].NameW, AShapeName ) != 0 ; l ++ ){}
					if( l != FrameBase->ShapeNum )
					{
						MAnim->Anim->KeySet[ k ].ShapeTargetIndex = l ;
					}
				}

				MAnimSet->UseAnimNum ++ ;
			}

			FrameStack[ 0 ] = Frame ;
			AFrameStack[ 0 ] = AFrame ;
			StackCount[ 0 ] = 0 ;
			StackNum = 1 ;
			while( StackNum != 0 )
			{
				Frame = FrameStack[ StackNum - 1 ] ;
				AFrame = AFrameStack[ StackNum - 1 ] ;

				Count = StackCount[ StackNum - 1 ] ;
				if( Count < Frame->ChildNum )
				{
					StackCount[ StackNum - 1 ] ++ ;

					CFrame = Frame->ChildList[ Count ] ;
					for( j = 0 ; j < AFrame->ChildNum ; j ++ )
					{
						CAFrame = AFrame->ChildList[ j ] ;
						if( _WCSCMP( CFrame->BaseData->NameW, CAFrame->BaseData->NameW ) == 0 )
							break ;
					}
					if( j != AFrame->ChildNum )
					{
						if( ABaseTable[ CAFrame->BaseData->Index ] )
						{
							MAnim = &Model->Anim[ AttachIndex ] + Model->AnimSetMaxNum * CFrame->BaseData->Index ;
							MAnim->Use = true ;
							MAnim->Anim = MAnimSet->AnimSet->Anim + ( ABaseTable[ CAFrame->BaseData->Index ] - AnimSetBase->Anim ) ;
							MAnim->Anim->Frame = CFrame ;

							AAnimBase = MAnim->Anim->BaseData ;
							AKeySetBase = AAnimBase->KeySet ;
							for( k = 0 ; k < AAnimBase->KeySetNum ; k ++, AKeySetBase ++ )
							{
								if( AKeySetBase->DataType != MV1_ANIMKEY_DATATYPE_SHAPE ) continue ;

								AFrameBase = &AMBase->Frame[ AAnimBase->TargetFrameIndex ] ;
								AShapeName = AFrameBase->Shape[ AKeySetBase->TargetShapeIndex ].NameW ;
								for( l = 0 ; l < FrameBase->ShapeNum && _WCSCMP( FrameBase->Shape[ l ].NameW, AShapeName ) != 0 ; l ++ ){}
								if( l != FrameBase->ShapeNum )
								{
									MAnim->Anim->KeySet[ k ].ShapeTargetIndex = l ;
								}
							}

							MAnimSet->UseAnimNum ++ ;
						}

						FrameStack[ StackNum ] = CFrame ;
						AFrameStack[ StackNum ] = CAFrame ;
						StackCount[ StackNum ] = 0 ;
						StackNum ++ ;
					}
				}
				else
				{
					StackNum -- ;
				}
			}
		}
	}
/*
	// �e�t���[���̃A�j���[�V�����̃A�h���X���Z�b�g
	Frame = Model->Frame ;
	MAnim = &Model->Anim[ AttachIndex ] ;
	for( i = 0 ; i < MBase->FrameNum ; i ++, Frame ++, MAnim += Model->AnimSetMaxNum )
	{
		MAnim->Anim = MV1GetAnimSetAnim( MAnimSet->AnimSet, Frame->BaseData->Name ) ;
		MAnim->Use = MAnim->Anim != NULL ;
		if( MAnim->Use )
		{
			MAnim->Anim->Frame = Frame ;
			MAnimSet->UseAnimNum ++ ;
		}
	}
*/

	// �V�F�C�v�̏�Ԃɕω����������t���O�𗧂Ă�
	Model->ShapeChangeFlag = true ;
	Frame = Model->Frame ;
	for( i = 0 ; i < MBase->FrameNum ; i ++, Frame ++ )
	{
		if( Frame->BaseData->ShapeNum == 0 )
			continue ;

		Frame->ShapeChangeFlag = true ;
	}

	// �A�^�b�`����Ă���A�j���[�V�����̐����C���N�������g
	Model->AnimSetNum ++ ;

	// �A�j���[�V�����p�����[�^�����ɂ����s�񂪃Z�b�g�A�b�v����Ă��Ȃ���Ԃɂ���
	Model->AnimSetupFlag = false ;
	Model->LocalWorldMatrixSetupFlag = false ;

	// �A�j���[�V�����̎��Ԃ�����������
	MV1SetAnimSetTime( MAnimSet->AnimSet, 0.0f ) ;

	// �A�^�b�`�����C���f�b�N�X��Ԃ�
	return AttachIndex ;
}

// �A�j���[�V�������f�^�b�`����
extern int NS_MV1DetachAnim( int MHandle, int AttachIndex )
{
	MV1_MODEL *Model ;
	MV1_MODEL_ANIM *MAnim ;
	MV1_FRAME *Frame ;
	int i, FrameNum ;

	// ����������Ă��Ȃ�������G���[
	if( MV1Man.Initialize == false ) return -1 ;

	// �A�h���X�擾
	if( MV1MDLCHK( MHandle, Model ) )
		return -1 ;

	// �m�ۂ��Ă���A�^�b�`�C���f�b�N�X�O�������ꍇ�͉��������ɏI��
	if( AttachIndex < 0 || AttachIndex >= Model->AnimSetMaxNum ) return -1 ;

	// �A�^�b�`����Ă����烁���������
	if( Model->AnimSet[ AttachIndex ].Use )
	{
		MDFREEMEM( Model->AnimSet[ AttachIndex ].AnimSet ) ;
		Model->AnimSet[ AttachIndex ].AnimSet = NULL ;
		Model->AnimSet[ AttachIndex ].Use = false ;

		// �e�t���[���̃A�j���[�V�������̏�����
		MAnim = &Model->Anim[ AttachIndex ] ;
		FrameNum = Model->BaseData->FrameNum ;
		for( i = 0 ; i < FrameNum ; i ++, MAnim += Model->AnimSetMaxNum )
		{
			MAnim->Anim = NULL ;
			MAnim->Use = false ;
		}

		// �V�F�C�v�̏�Ԃɕω����������t���O�𗧂Ă�
		Model->ShapeChangeFlag = true ;
		Frame = Model->Frame ;
		for( i = 0 ; i < FrameNum ; i ++, Frame ++ )
		{
			if( Frame->BaseData->ShapeNum == 0 )
				continue ;

			Frame->ShapeChangeFlag = true ;
		}

		// �A�^�b�`����Ă���A�j���[�V�����̐����f�N�������g
		Model->AnimSetNum -- ;
	}

	// �A�j���[�V�����p�����[�^�����ɂ����s�񂪃Z�b�g�A�b�v����Ă��Ȃ���Ԃɂ���
	Model->AnimSetupFlag = false ;
	Model->LocalWorldMatrixSetupFlag = false ;

	// ����I��
	return 0 ;
}





















// ���������`��ׂ̈ɕ`��ҋ@���Ă��郂�f����`�悷��
extern int MV1DrawPackDrawModel( void )
{
	MV1_MODEL *Model ;

	if( MV1Man.PackDrawModel == NULL )
	{
		return 0 ;
	}

	Model = MV1Man.PackDrawModel ;
	MV1Man.PackDrawModel = NULL ;

	return MV1DrawModelBase( Model ) ;
}

// ���f���̕`�揈�����s��
static int MV1DrawModelBase( MV1_MODEL *Model )
{
	MV1_MODEL_BASE *ModelBase ;
	MV1_FRAME *Frame ;
	MV1_MESH *Mesh ;
	int i, j, k, l ;
	VECTOR CheckPos[ 8 ], Tmp ;
	MV1_MESH **OpacityMeshList ;
	MV1_MESH **SemiTransBaseOpacityMeshList ;
	MV1_MESH **SemiTransMeshList ;
	int OpacityMeshNum ;
	int SemiTransBaseOpacityMeshNum ;
	int SemiTransMeshNum ;

	// ����������Ă��Ȃ�������G���[
	if( MV1Man.Initialize == false ) return -1 ;

	// �A�h���X�擾
	ModelBase = Model->BaseData ;

	// �g�p���Ă���e�N�X�`���̒��Ƀ��[�r�[���܂܂�Ă����炻�̍X�V�`�F�b�N������
#ifndef DX_NON_MOVIE
	{
		int ind ;
		IMAGEDATA *Image ;

		for( ind = 0 ; ind < ModelBase->TextureNum ; ind ++ )
		{
			if( Model->Texture[ ind ].UseUserGraphHandle )
			{
				Image = Graphics_Image_GetData( Model->Texture[ ind ].UserGraphHandle ) ;
				if( Image != NULL && Image->MovieHandle != -1 )
				{
					UpdateMovie( Image->MovieHandle, FALSE ) ;
				}
			}
		}
	}
#endif

	// �`�悷�郁�b�V���̃A�h���X�z����i�[���郁�����̈�̊m��
	if( DrawMeshListResize( Model->BaseData->MeshNum ) < 0 )
	{
		return -1 ;
	}
	OpacityMeshList              = MV1Man.DrawMeshList ;
	SemiTransBaseOpacityMeshList = OpacityMeshList              + Model->BaseData->MeshNum ;
	SemiTransMeshList            = SemiTransBaseOpacityMeshList + Model->BaseData->MeshNum ;
	OpacityMeshNum               = 0 ;
	SemiTransBaseOpacityMeshNum  = 0 ;
	SemiTransMeshNum             = 0 ;

	// �V�F�C�v�f�[�^�̃Z�b�g�A�b�v
	if( Model->BaseData->ShapeMeshNum != 0 )
		MV1_SetupShapeVertex_PF( Model->HandleInfo.Handle ) ;

	// �����_�����O�̏���
	MV1_BeginRender_PF( Model ) ;

	// �`�悷�郁�b�V���̃��X�g���쐬����
	Frame = Model->Frame ;
	for( i = 0 ; i < Model->BaseData->FrameNum ; i ++ , Frame ++ )
	{
		// ���b�V���������������΂�
		if( Frame->BaseData->MeshNum == 0 ) continue ;

		// �}�e���A���X�V�`�F�b�N
		MV1SETUPDRAWMATERIALFRAME( Frame ) ;

		// ��\���w�肾�����牽�����Ȃ�
		if( Frame->SetupDrawMaterial.Visible == FALSE ) continue ;

		// ���������`��Ή��n���h���ł͂Ȃ��A�V���h�E�}�b�v�ւ̕`�撆�ł��Ȃ��X�L�����b�V���ł��Ȃ��ꍇ�͉�����
		if( ModelBase->UsePackDraw == FALSE && GSYS.DrawSetting.ShadowMapDraw == FALSE && Frame->BaseData->IsSkinMesh == FALSE )
		{
			// ���`�F�b�N�p���_���W���Z�o����
			Tmp   = Frame->BaseData->MaxPosition   ; VectorTransform4X4CT( &CheckPos[ 0 ], &Tmp, &Frame->LocalWorldMatrix ) ;
			Tmp.x = Frame->BaseData->MinPosition.x ; VectorTransform4X4CT( &CheckPos[ 1 ], &Tmp, &Frame->LocalWorldMatrix ) ;
			Tmp.y = Frame->BaseData->MinPosition.y ; VectorTransform4X4CT( &CheckPos[ 2 ], &Tmp, &Frame->LocalWorldMatrix ) ;
			Tmp.x = Frame->BaseData->MaxPosition.x ; VectorTransform4X4CT( &CheckPos[ 3 ], &Tmp, &Frame->LocalWorldMatrix ) ;
			Tmp.z = Frame->BaseData->MinPosition.z ; VectorTransform4X4CT( &CheckPos[ 4 ], &Tmp, &Frame->LocalWorldMatrix ) ;
			Tmp.y = Frame->BaseData->MaxPosition.y ; VectorTransform4X4CT( &CheckPos[ 5 ], &Tmp, &Frame->LocalWorldMatrix ) ;
			Tmp.x = Frame->BaseData->MinPosition.x ; VectorTransform4X4CT( &CheckPos[ 6 ], &Tmp, &Frame->LocalWorldMatrix ) ;
			Tmp.y = Frame->BaseData->MinPosition.y ; VectorTransform4X4CT( &CheckPos[ 7 ], &Tmp, &Frame->LocalWorldMatrix ) ;

			// ���`�F�b�N
			if( Graphics_Camera_CheckCameraViewClip_Box_PosDim( CheckPos ) == TRUE )
				continue ;
		}

		// MaterialNumberOrderDraw �������Ă��邩�ǂ����ŏ����𕪊�
		if( Model->BaseData->MaterialNumberOrderDraw != 0 )
		{
			int MaterialIndex ;

			// MaterialNumberOrderDraw �������Ă���ꍇ�̓��b�V���Ɋ��蓖�Ă��Ă���}�e���A���̔ԍ����Ⴂ���ʕ`�悷��

			// ���b�V���̐������J��Ԃ�
			Mesh = Frame->Mesh ;
			for( k = 0 ; k < Frame->BaseData->MeshNum ; k ++ , Mesh ++ )
			{
				// �}�e���A���X�V�`�F�b�N
				MV1SETUPDRAWMATERIALMESH( Mesh ) ;

				// ��\���̏ꍇ�͉������Ȃ�
				if( Mesh->SetupDrawMaterial.Visible == 0 ) continue ;

				// �}�e���A���ԍ��Ń\�[�g�}��
				MaterialIndex = ( int )( Mesh->Material->BaseData - Model->BaseData->Material ) ;
				for( j = 0 ; j < OpacityMeshNum ; j ++ )
				{
					if( MaterialIndex < OpacityMeshList[ j ]->Material->BaseData - Model->BaseData->Material )
						break ;
				}

				for( l = OpacityMeshNum - 1 ; l >= j ; l -- )
				{
					OpacityMeshList[ l + 1 ] = OpacityMeshList[ l ] ;
				}

				OpacityMeshList[ j ] = Mesh ;
				OpacityMeshNum ++ ;
			}
		}
		else
		{
			// MaterialNumberOrderDraw �������Ă��Ȃ��ꍇ�͕s�����̃��b�V����`�悵����ɔ������v�f�̂��郁�b�V����`�悷��

			// ���b�V���̐������J��Ԃ�
			Mesh = Frame->Mesh ;
			for( k = 0 ; k < Frame->BaseData->MeshNum ; k ++ , Mesh ++ )
			{
				// �}�e���A���X�V�`�F�b�N
				MV1SETUPDRAWMATERIALMESH( Mesh ) ;

				// ��\���̏ꍇ�͉������Ȃ�
				if( Mesh->SetupDrawMaterial.Visible == 0 ) continue ;

				// �������v�f�����邩�ǂ����𒲂ׂ�
				if( Mesh->SemiTransStateSetupFlag == false )
				{
					NS_MV1GetMeshSemiTransState( Model->HandleInfo.Handle, ( int )( Mesh - Model->Mesh ) ) ;
				}

				if( Mesh->SemiTransState == false )
				{
					OpacityMeshList[ OpacityMeshNum ] = Mesh ;
					OpacityMeshNum ++ ;
				}
				else
				if( Mesh->BaseData->SemiTransState == 0 )
				{
					SemiTransBaseOpacityMeshList[ SemiTransBaseOpacityMeshNum ] = Mesh ;
					SemiTransBaseOpacityMeshNum ++ ;
				}
				else
				{
					SemiTransMeshList[ SemiTransMeshNum ] = Mesh ;
					SemiTransMeshNum ++ ;
				}
			}
		}
	}

	// �s�����I�u�W�F�N�g�̕`��
	if( MV1Man.SemiTransDrawMode == DX_SEMITRANSDRAWMODE_ALWAYS ||
		MV1Man.SemiTransDrawMode == DX_SEMITRANSDRAWMODE_NOT_SEMITRANS_ONLY )
	{
		for( i = 0 ; i < OpacityMeshNum ; i ++ )
		{
			MV1_DrawMesh_PF( OpacityMeshList[ i ] ) ;
		}
	}

	// �������I�u�W�F�N�g�̕`��
	if( MV1Man.SemiTransDrawMode == DX_SEMITRANSDRAWMODE_ALWAYS ||
		MV1Man.SemiTransDrawMode == DX_SEMITRANSDRAWMODE_SEMITRANS_ONLY )
	{
		// �������I�u�W�F�N�g�Ȃ��珉����Ԃł͕s�����̃I�u�W�F�N�g���ɕ`��
		for( i = 0 ; i < SemiTransBaseOpacityMeshNum ; i ++ )
		{
			MV1_DrawMesh_PF( SemiTransBaseOpacityMeshList[ i ] ) ;
		}

		for( i = 0 ; i < SemiTransMeshNum ; i ++ )
		{
			MV1_DrawMesh_PF( SemiTransMeshList[ i ] ) ;
		}
	}

	// �����_�����O�̌�n��
	MV1_EndRender_PF() ;

	// �`��X�g�b�N�̏�����
	Model->PackDrawStockNum = 0 ;

	// �I��
	return 0 ;
}

// ���f����`�悷��
extern int NS_MV1DrawModel( int MHandle )
{
	MV1_MODEL *Model ;
	MV1_MODEL_BASE *ModelBase ;
	MV1_FRAME *Frame ;
	int i ;

	// ����������Ă��Ȃ�������G���[
	if( MV1Man.Initialize == false ) return -1 ;

	// �A�h���X�擾
	if( MV1MDLCHK( MHandle, Model ) )
		return -1 ;
	ModelBase = Model->BaseData ;

	// ��\���ݒ肾������`�悵�Ȃ�
	if( Model->DrawMaterial.Visible == 0 )
		return 0 ;

	// ���_�f�[�^�̕`����I��点�Ă���
	Graphics_Hardware_RenderVertex() ;

	// �s��̃Z�b�g�A�b�v
	MV1SETUPMATRIX( Model ) ;

	// ���������`��ɑΉ����Ă���ꍇ�̓X�g�b�N�ɏ���ǉ�����
	if( ModelBase->UsePackDraw )
	{
		MATRIX_4X4CT_F *DestMatrix ;

		// ���ɕ`��҂�����Ă��郂�f������������`�悷��
		if( MV1Man.PackDrawModel != NULL && MV1Man.PackDrawModel != Model )
		{
			MV1DrawPackDrawModel() ;
		}

		// �X�g�b�N�̐������E�ɒB���Ă�����`�揈�����s��
		if( Model->PackDrawStockNum >= ModelBase->PackDrawMaxNum )
		{
			MV1DrawPackDrawModel() ;
		}

		// �X�g�b�N�i�[��̃A�h���X���Z�o
		DestMatrix = Model->PackDrawMatrix + Model->PackDrawStockNum * ModelBase->PackDrawMatrixUnitNum ;

		// �X�L�j���O���b�V���̍s����R�s�[����
		if( ModelBase->SkinBoneNum > 0 )
		{
			for( i = 0 ; i < ModelBase->SkinBoneNum ; i ++ )
			{
				ConvertMatrix4x4cToMatrix4x4cF( &DestMatrix[ i ], &Model->SkinBoneMatrix[ i ] ) ;
			}
		}
		DestMatrix += ModelBase->SkinBoneNum ;

		// �t���[���̍s����R�s�[����
		Frame = Model->Frame ;
		for( i = 0 ; i < ModelBase->FrameNum ; i ++ )
		{
			ConvertMatrix4x4cToMatrix4x4cF( &DestMatrix[ i ], &Frame[ i ].LocalWorldMatrix ) ;
		}

		// �X�g�b�N�̐��𑝂₷
		Model->PackDrawStockNum ++ ;

		// ���������`��ׂ̈ɕ`��ҋ@�����Ă��郂�f���̃A�h���X�Ƃ��ĕۑ�
		MV1Man.PackDrawModel = Model ;
	}
	else
	{
		// ���������`��ɑΉ����Ă��Ȃ��ꍇ�͕��ʂɕ`�悷��
		MV1DrawModelBase( Model ) ;
	}

	// �I��
	return 0 ;
}

// ���f���̎w��̃t���[����`�悷��
extern int NS_MV1DrawFrame( int MHandle, int FrameIndex )
{
	MV1_MODEL *Model ;
	MV1_MODEL_BASE *ModelBase ;
	MV1_FRAME *Frame ;
	MV1_MESH *Mesh ;
	int k ;
	VECTOR CheckPos[ 8 ], Tmp ;
	MV1_MESH **OpacityMeshList ;
	MV1_MESH **SemiTransBaseOpacityMeshList ;
	MV1_MESH **SemiTransMeshList ;
	int OpacityMeshNum ;
	int SemiTransBaseOpacityMeshNum ;
	int SemiTransMeshNum ;

	// ����������Ă��Ȃ�������G���[
	if( MV1Man.Initialize == false ) return -1 ;

	// �A�h���X�擾
	if( MV1MDLCHK( MHandle, Model ) )
		return -1 ;
	ModelBase = Model->BaseData ;

	// ��\���ݒ肾������`�悵�Ȃ�
	if( Model->DrawMaterial.Visible == 0 )
		return 0 ;

	// �g�p���Ă���e�N�X�`���̒��Ƀ��[�r�[���܂܂�Ă����炻�̍X�V�`�F�b�N������
#ifndef DX_NON_MOVIE
	{
		int ind ;
		IMAGEDATA *Image ;

		for( ind = 0 ; ind < ModelBase->TextureNum ; ind ++ )
		{
			if( Model->Texture[ ind ].UseUserGraphHandle )
			{
				Image = Graphics_Image_GetData( Model->Texture[ ind ].UserGraphHandle ) ;
				if( Image != NULL && Image->MovieHandle != -1 )
				{
					UpdateMovie( Image->MovieHandle, FALSE ) ;
				}
			}
		}
	}
#endif

	// �t���[���C���f�b�N�X�̃`�F�b�N
	if( FrameIndex < 0 || FrameIndex >= Model->BaseData->FrameNum )
		return -1 ;
	Frame = Model->Frame + FrameIndex ;

	// ���b�V�������������牽�����Ȃ�
	if( Frame->BaseData->MeshNum == 0 )
		return -1 ;

	// �`�悷�郁�b�V���̃A�h���X�z����i�[���郁�����̈�̊m��
	if( DrawMeshListResize( Model->BaseData->MeshNum ) < 0 )
	{
		return -1 ;
	}
	OpacityMeshList              = MV1Man.DrawMeshList ;
	SemiTransBaseOpacityMeshList = OpacityMeshList              + Model->BaseData->MeshNum ;
	SemiTransMeshList            = SemiTransBaseOpacityMeshList + Model->BaseData->MeshNum ;
	OpacityMeshNum               = 0 ;
	SemiTransBaseOpacityMeshNum  = 0 ;
	SemiTransMeshNum             = 0 ;

	// �}�e���A���X�V�`�F�b�N
	MV1SETUPDRAWMATERIALFRAME( Frame ) ;

	// ��\���ݒ肾������`�悵�Ȃ�
	if( Frame->SetupDrawMaterial.Visible == 0 )
		return 0 ;

	// �s��̃Z�b�g�A�b�v
	MV1SETUPMATRIX( Model ) ;

	// �V���h�E�}�b�v�ւ̕`�撆�ł͂Ȃ��X�L�����b�V���ł��Ȃ��ꍇ�͉�����
	if( GSYS.DrawSetting.ShadowMapDraw == FALSE && Frame->BaseData->IsSkinMesh == FALSE )
	{
		// ���`�F�b�N�p���_���W���Z�o����
		Tmp   = Frame->BaseData->MaxPosition   ; VectorTransform4X4CT( &CheckPos[ 0 ], &Tmp, &Frame->LocalWorldMatrix ) ;
		Tmp.x = Frame->BaseData->MinPosition.x ; VectorTransform4X4CT( &CheckPos[ 1 ], &Tmp, &Frame->LocalWorldMatrix ) ;
		Tmp.y = Frame->BaseData->MinPosition.y ; VectorTransform4X4CT( &CheckPos[ 2 ], &Tmp, &Frame->LocalWorldMatrix ) ;
		Tmp.x = Frame->BaseData->MaxPosition.x ; VectorTransform4X4CT( &CheckPos[ 3 ], &Tmp, &Frame->LocalWorldMatrix ) ;
		Tmp.z = Frame->BaseData->MinPosition.z ; VectorTransform4X4CT( &CheckPos[ 4 ], &Tmp, &Frame->LocalWorldMatrix ) ;
		Tmp.y = Frame->BaseData->MaxPosition.y ; VectorTransform4X4CT( &CheckPos[ 5 ], &Tmp, &Frame->LocalWorldMatrix ) ;
		Tmp.x = Frame->BaseData->MinPosition.x ; VectorTransform4X4CT( &CheckPos[ 6 ], &Tmp, &Frame->LocalWorldMatrix ) ;
		Tmp.y = Frame->BaseData->MinPosition.y ; VectorTransform4X4CT( &CheckPos[ 7 ], &Tmp, &Frame->LocalWorldMatrix ) ;

		// ���`�F�b�N
		if( Graphics_Camera_CheckCameraViewClip_Box_PosDim( CheckPos ) == TRUE )
			return -1 ;
	}

	// �V�F�C�v�f�[�^�̃Z�b�g�A�b�v
	if( Model->BaseData->ShapeMeshNum != 0 )
		MV1_SetupShapeVertex_PF( MHandle ) ;

	// �����_�����O�̏���
	MV1_BeginRender_PF( Model ) ;

	// �`�悷�郁�b�V���̐U�蕪��
	Mesh = Frame->Mesh ;
	for( k = 0 ; k < Frame->BaseData->MeshNum ; k ++ , Mesh ++ )
	{
		// �}�e���A���X�V�`�F�b�N
		MV1SETUPDRAWMATERIALMESH( Mesh ) ;

		// ��\���̏ꍇ�͉������Ȃ�
		if( Mesh->SetupDrawMaterial.Visible == 0 ) continue ;

		// �������v�f�����邩�ǂ����𒲂ׂ�
		if( Mesh->SemiTransStateSetupFlag == false )
		{
			NS_MV1GetMeshSemiTransState( MHandle, ( int )( Mesh - Model->Mesh ) ) ;
		}

		if( Mesh->SemiTransState == false )
		{
			OpacityMeshList[ OpacityMeshNum ] = Mesh ;
			OpacityMeshNum ++ ;
		}
		else
		if( Mesh->BaseData->SemiTransState == 0 )
		{
			SemiTransBaseOpacityMeshList[ SemiTransBaseOpacityMeshNum ] = Mesh ;
			SemiTransBaseOpacityMeshNum ++ ;
		}
		else
		{
			SemiTransMeshList[ SemiTransMeshNum ] = Mesh ;
			SemiTransMeshNum ++ ;
		}
	}

	// �s�������b�V���̕`��
	if( MV1Man.SemiTransDrawMode == DX_SEMITRANSDRAWMODE_ALWAYS ||
		MV1Man.SemiTransDrawMode == DX_SEMITRANSDRAWMODE_NOT_SEMITRANS_ONLY )
	{
		for( k = 0 ; k < OpacityMeshNum ; k ++ )
		{
			MV1_DrawMesh_PF( OpacityMeshList[ k ] ) ;
		}
	}

	// ���������b�V���̕`��
	if( MV1Man.SemiTransDrawMode == DX_SEMITRANSDRAWMODE_ALWAYS ||
		MV1Man.SemiTransDrawMode == DX_SEMITRANSDRAWMODE_SEMITRANS_ONLY )
	{
		// �������I�u�W�F�N�g�Ȃ��珉����Ԃł͕s�����̃I�u�W�F�N�g���ɕ`��
		for( k = 0 ; k < SemiTransBaseOpacityMeshNum ; k ++ )
		{
			MV1_DrawMesh_PF( SemiTransBaseOpacityMeshList[ k ] ) ;
		}

		for( k = 0 ; k < SemiTransMeshNum ; k ++ )
		{
			MV1_DrawMesh_PF( SemiTransMeshList[ k ] ) ;
		}
	}

	// �����_�����O�̌�n��
	MV1_EndRender_PF() ;

	// �I��
	return 0 ;
}

// ���f���̎w��̃��b�V����`�悷��
extern int NS_MV1DrawMesh( int MHandle, int MeshIndex )
{
	MV1_MODEL *Model ;
	MV1_MODEL_BASE *ModelBase ;
	MV1_FRAME *Frame ;
	MV1_MESH *Mesh ;

	// ����������Ă��Ȃ�������G���[
	if( MV1Man.Initialize == false ) return -1 ;

	// �A�h���X�擾
	if( MV1MDLCHK( MHandle, Model ) )
		return -1 ;
	ModelBase = Model->BaseData ;

	// ��\���ݒ肾������`�悵�Ȃ�
	if( Model->DrawMaterial.Visible == 0 )
		return 0 ;

	// �g�p���Ă���e�N�X�`���̒��Ƀ��[�r�[���܂܂�Ă����炻�̍X�V�`�F�b�N������
#ifndef DX_NON_MOVIE
	{
		int ind ;
		IMAGEDATA *Image ;

		for( ind = 0 ; ind < ModelBase->TextureNum ; ind ++ )
		{
			if( Model->Texture[ ind ].UseUserGraphHandle )
			{
				Image = Graphics_Image_GetData( Model->Texture[ ind ].UserGraphHandle ) ;
				if( Image != NULL && Image->MovieHandle != -1 )
				{
					UpdateMovie( Image->MovieHandle, FALSE ) ;
				}
			}
		}
	}
#endif

	// ���b�V���C���f�b�N�X�̃`�F�b�N
	if( MeshIndex < 0 || MeshIndex >= ModelBase->MeshNum )
		return -1 ;
	Mesh = Model->Mesh + MeshIndex ;
	Frame = Mesh->Container ;

	// �}�e���A���X�V�`�F�b�N
	MV1SETUPDRAWMATERIALMESH( Mesh ) ;

	// ��\���̏ꍇ�͉������Ȃ�
	if( Mesh->SetupDrawMaterial.Visible == 0 )
		return 0 ;

	// �s��̃Z�b�g�A�b�v
	MV1SETUPMATRIX( Model ) ;

	// �V�F�C�v�f�[�^�̃Z�b�g�A�b�v
	if( Model->BaseData->ShapeMeshNum != 0 )
		MV1_SetupShapeVertex_PF( MHandle ) ;

	// �����_�����O�̏���
	MV1_BeginRender_PF( Model ) ;

	// ���b�V���̕`��
	MV1_DrawMesh_PF( Mesh ) ;

	// �����_�����O�̌�n��
	MV1_EndRender_PF() ;

	// �I��
	return 0 ;
}

// ���f���̎w��̃g���C�A���O�����X�g��`�悷��
extern int NS_MV1DrawTriangleList( int MHandle, int TriangleListIndex )
{
	MV1_MODEL *Model ;
	MV1_MODEL_BASE *ModelBase ;
	MV1_FRAME *Frame ;
	MV1_MESH *Mesh ;
	MV1_TRIANGLE_LIST *TList ;

	// ����������Ă��Ȃ�������G���[
	if( MV1Man.Initialize == false ) return -1 ;

	// �A�h���X�擾
	if( MV1MDLCHK( MHandle, Model ) )
		return -1 ;
	ModelBase = Model->BaseData ;

	// ��\���ݒ肾������`�悵�Ȃ�
	if( Model->DrawMaterial.Visible == 0 )
		return 0 ;

	// �g�p���Ă���e�N�X�`���̒��Ƀ��[�r�[���܂܂�Ă����炻�̍X�V�`�F�b�N������
#ifndef DX_NON_MOVIE
	{
		int ind ;
		IMAGEDATA *Image ;

		for( ind = 0 ; ind < ModelBase->TextureNum ; ind ++ )
		{
			if( Model->Texture[ ind ].UseUserGraphHandle )
			{
				Image = Graphics_Image_GetData( Model->Texture[ ind ].UserGraphHandle ) ;
				if( Image != NULL && Image->MovieHandle != -1 )
				{
					UpdateMovie( Image->MovieHandle, FALSE ) ;
				}
			}
		}
	}
#endif

	// �g���C�A���O�����X�g�C���f�b�N�X�̃`�F�b�N
	if( TriangleListIndex < 0 || TriangleListIndex >= ModelBase->TriangleListNum )
		return -1 ;
	TList = ( MV1_TRIANGLE_LIST * )( ( BYTE * )Model->TriangleList + TriangleListIndex * ( sizeof( MV1_TRIANGLE_LIST ) + sizeof( MV1_TRIANGLE_LIST_PF ) ) );
	Mesh = TList->Container ;
	Frame = Mesh->Container ;

	// �}�e���A���X�V�`�F�b�N
	MV1SETUPDRAWMATERIALMESH( Mesh ) ;

	// ��\���̏ꍇ�͉������Ȃ�
	if( Mesh->SetupDrawMaterial.Visible == 0 )
		return 0 ;

	// �s��̃Z�b�g�A�b�v
	MV1SETUPMATRIX( Model ) ;

	// �V�F�C�v�f�[�^�̃Z�b�g�A�b�v
	if( Model->BaseData->ShapeMeshNum != 0 )
		MV1_SetupShapeVertex_PF( MHandle ) ;

	// �����_�����O�̏���
	MV1_BeginRender_PF( Model ) ;

	// �g���C�A���O�����X�g�̕`��
	MV1_DrawMesh_PF( Mesh, ( int )( ( int )( ( ( BYTE * )TList - ( BYTE * )Mesh->TriangleList ) ) / ( sizeof( MV1_TRIANGLE_LIST ) + sizeof( MV1_TRIANGLE_LIST_PF ) ) ) ) ;

	// �����_�����O�̌�n��
	MV1_EndRender_PF() ;

	// �I��
	return 0 ;
}

// ���f���̃f�o�b�O�`��
extern int NS_MV1DrawModelDebug(
	  int MHandle, unsigned int Color,
	  int IsNormalLine, float NormalLineLength,
	  int IsPolyLine,
	  int IsCollisionBox )
{
	MV1_MODEL *Model ;
	MV1_MODEL_BASE *ModelBase ;
	MV1_REF_POLYGONLIST PolyList ;
	MV1_REF_POLYGON *Poly ;
	MV1_REF_VERTEX *Vert ;
	int i, j, VertCount ;
	VERTEX_3D Vertex[ 300 ], *vert ;
	int r, g, b ;
	MATRIX Mat, OrigTransMat ;

	// �A�h���X�擾
	if( MV1MDLCHK( MHandle, Model ) )
		return -1 ;
	ModelBase = Model->BaseData ;

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// ���X�ݒ肳��Ă������[�J�������[���h�s���ۑ����ĒP�ʍs����Z�b�g����
	NS_GetTransformToWorldMatrix( &OrigTransMat ) ;
	CreateIdentityMatrix( &Mat ) ;
	NS_SetTransformToWorld( &Mat ) ;

	// �F�̕���
	NS_GetColor2( Color, &r, &g, &b ) ;

	// �Q�Ɨp�|���S���̎擾
	if( IsPolyLine && IsNormalLine == FALSE )
	{
		NS_MV1RefreshReferenceMesh( MHandle, -1, TRUE, TRUE ) ;
		PolyList = NS_MV1GetReferenceMesh( MHandle, -1, TRUE, TRUE ) ;
	}
	else
	{
		NS_MV1RefreshReferenceMesh( MHandle, -1, TRUE, FALSE ) ;
		PolyList = NS_MV1GetReferenceMesh( MHandle, -1, TRUE, FALSE ) ;
	}

	// �@�����C���̕`��
	if( IsNormalLine )
	{
		Poly = PolyList.Polygons ;
		Vert = PolyList.Vertexs ;
		VertCount = 0 ;
		_MEMSET( Vertex, 0, sizeof( Vertex ) ) ;
		vert = Vertex ;
		for( i = 0 ; i < PolyList.PolygonNum ; i ++, Poly ++ )
		{
			for( j = 0 ; j < 3 ; j ++ )
			{
				vert[ 0 ].pos = Vert[ Poly->VIndex[ j ] ].Position ;
				vert[ 0 ].r = ( unsigned char )r ;
				vert[ 0 ].g = ( unsigned char )g ;
				vert[ 0 ].b = ( unsigned char )b ;
				vert[ 0 ].a = 255 ;

				vert[ 1 ].pos.x = Vert[ Poly->VIndex[ j ] ].Position.x + Vert[ Poly->VIndex[ j ] ].Normal.x * NormalLineLength ;
				vert[ 1 ].pos.y = Vert[ Poly->VIndex[ j ] ].Position.y + Vert[ Poly->VIndex[ j ] ].Normal.y * NormalLineLength ;
				vert[ 1 ].pos.z = Vert[ Poly->VIndex[ j ] ].Position.z + Vert[ Poly->VIndex[ j ] ].Normal.z * NormalLineLength ;
				vert[ 1 ].r = ( unsigned char )r ;
				vert[ 1 ].g = ( unsigned char )g ;
				vert[ 1 ].b = ( unsigned char )b ;
				vert[ 1 ].a = 255 ;

				vert += 2 ;
			}
			VertCount += 3 * 2 ;
			if( VertCount == 300 )
			{
				NS_DrawPolygon3DBase( Vertex, VertCount, DX_PRIMTYPE_LINELIST, DX_NONE_GRAPH, FALSE ) ;
				VertCount = 0 ;
				vert = Vertex ;
			}
		}
		if( VertCount )
		{
			NS_DrawPolygon3DBase( Vertex, VertCount, DX_PRIMTYPE_LINELIST, DX_NONE_GRAPH, FALSE ) ;
		}
	}

	// �|���S�����C���̕`��
	if( IsPolyLine )
	{
		Poly = PolyList.Polygons ;
		Vert = PolyList.Vertexs ;
		VertCount = 0 ;
		_MEMSET( Vertex, 0, sizeof( Vertex ) ) ;
		vert = Vertex ;
		for( i = 0 ; i < PolyList.PolygonNum ; i ++, Poly ++ )
		{
			for( j = 0 ; j < 2 ; j ++ )
			{
				vert[ 0 ].pos = Vert[ Poly->VIndex[ j ] ].Position ;
				vert[ 0 ].r = ( unsigned char )r ;
				vert[ 0 ].g = ( unsigned char )g ;
				vert[ 0 ].b = ( unsigned char )b ;
				vert[ 0 ].a = 255 ;

				vert[ 1 ].pos = Vert[ Poly->VIndex[ j + 1 ] ].Position ;
				vert[ 1 ].r = ( unsigned char )r ;
				vert[ 1 ].g = ( unsigned char )g ;
				vert[ 1 ].b = ( unsigned char )b ;
				vert[ 1 ].a = 255 ;

				vert += 2 ;
			}
			vert[ 0 ].pos = Vert[ Poly->VIndex[ 2 ] ].Position ;
			vert[ 0 ].r = ( unsigned char )r ;
			vert[ 0 ].g = ( unsigned char )g ;
			vert[ 0 ].b = ( unsigned char )b ;
			vert[ 0 ].a = 255 ;

			vert[ 1 ].pos = Vert[ Poly->VIndex[ 0 ] ].Position ;
			vert[ 1 ].r = ( unsigned char )r ;
			vert[ 1 ].g = ( unsigned char )g ;
			vert[ 1 ].b = ( unsigned char )b ;
			vert[ 1 ].a = 255 ;

			vert += 2 ;

			VertCount += 3 * 2 ;
			if( VertCount == 300 )
			{
				NS_DrawPolygon3DBase( Vertex, VertCount, DX_PRIMTYPE_LINELIST, DX_NONE_GRAPH, FALSE ) ;
				VertCount = 0 ;
				vert = Vertex ;
			}
		}
		if( VertCount )
		{
			NS_DrawPolygon3DBase( Vertex, VertCount, DX_PRIMTYPE_LINELIST, DX_NONE_GRAPH, FALSE ) ;
		}
	}

	// �R���W�����{�b�N�X�̕`��
	if( IsCollisionBox )
	{
		if( Model->Collision )
		{
			VECTOR Pos ;
			MV1_COLLISION *Collision ;

			Collision = Model->Collision ;
			_MEMSET( Vertex, 0, sizeof( Vertex ) ) ;
			vert = Vertex ;
			VertCount = 0 ;
			Pos.z = Collision->MinPosition.z ;
			for( i = 0 ; i <= Collision->ZDivNum ; i ++, Pos.z += Collision->UnitSize.z )
			{
				Pos.y = Collision->MinPosition.y ;
				Pos.x = Collision->MinPosition.x ;
				for( j = 0 ; j <= Collision->XDivNum ; j ++, Pos.x += Collision->UnitSize.x )
				{
					vert[ 0 ].pos = Pos ;
					vert[ 0 ].r = ( unsigned char )r ;
					vert[ 0 ].g = ( unsigned char )g ;
					vert[ 0 ].b = ( unsigned char )b ;
					vert[ 0 ].a = 255 ;

					vert[ 1 ].pos = Pos ;
					vert[ 1 ].pos.y = Collision->MaxPosition.y ;
					vert[ 1 ].r = ( unsigned char )r ;
					vert[ 1 ].g = ( unsigned char )g ;
					vert[ 1 ].b = ( unsigned char )b ;
					vert[ 1 ].a = 255 ;

					vert += 2 ;

					VertCount += 2 ;
					if( VertCount == 300 )
					{
						NS_DrawPolygon3DBase( Vertex, VertCount, DX_PRIMTYPE_LINELIST, DX_NONE_GRAPH, FALSE ) ;
						VertCount = 0 ;
						vert = Vertex ;
					}
				}

				Pos.y = Collision->MinPosition.y ;
				Pos.x = Collision->MinPosition.x ;
				for( j = 0 ; j <= Collision->YDivNum ; j ++, Pos.y += Collision->UnitSize.y )
				{
					vert[ 0 ].pos = Pos ;
					vert[ 0 ].r = ( unsigned char )r ;
					vert[ 0 ].g = ( unsigned char )g ;
					vert[ 0 ].b = ( unsigned char )b ;
					vert[ 0 ].a = 255 ;

					vert[ 1 ].pos = Pos ;
					vert[ 1 ].pos.x = Collision->MaxPosition.x ;
					vert[ 1 ].r = ( unsigned char )r ;
					vert[ 1 ].g = ( unsigned char )g ;
					vert[ 1 ].b = ( unsigned char )b ;
					vert[ 1 ].a = 255 ;

					vert += 2 ;

					VertCount += 2 ;
					if( VertCount == 300 )
					{
						NS_DrawPolygon3DBase( Vertex, VertCount, DX_PRIMTYPE_LINELIST, DX_NONE_GRAPH, FALSE ) ;
						VertCount = 0 ;
						vert = Vertex ;
					}
				}
			}

			Pos.z = Collision->MinPosition.z ;
			Pos.y = Collision->MinPosition.y ;
			for( i = 0 ; i <= Collision->YDivNum ; i ++, Pos.y += Collision->UnitSize.y )
			{
				Pos.x = Collision->MinPosition.x ;
				for( j = 0 ; j <= Collision->XDivNum ; j ++, Pos.x += Collision->UnitSize.x )
				{
					vert[ 0 ].pos = Pos ;
					vert[ 0 ].r = ( unsigned char )r ;
					vert[ 0 ].g = ( unsigned char )g ;
					vert[ 0 ].b = ( unsigned char )b ;
					vert[ 0 ].a = 255 ;

					vert[ 1 ].pos = Pos ;
					vert[ 1 ].pos.z = Collision->MaxPosition.z ;
					vert[ 1 ].r = ( unsigned char )r ;
					vert[ 1 ].g = ( unsigned char )g ;
					vert[ 1 ].b = ( unsigned char )b ;
					vert[ 1 ].a = 255 ;

					vert += 2 ;

					VertCount += 2 ;
					if( VertCount == 300 )
					{
						NS_DrawPolygon3DBase( Vertex, VertCount, DX_PRIMTYPE_LINELIST, DX_NONE_GRAPH, FALSE ) ;
						VertCount = 0 ;
						vert = Vertex ;
					}
				}
			}

			if( VertCount )
			{
				NS_DrawPolygon3DBase( Vertex, VertCount, DX_PRIMTYPE_LINELIST, DX_NONE_GRAPH, FALSE ) ;
				VertCount = 0 ;
				vert = Vertex ;
			}
		}
	}

	// ���X�ݒ肳��Ă������[���h���r���[�g�����X�t�H�[���s������ɖ߂�
	NS_SetTransformToWorld( &OrigTransMat ) ;

	// �I��
	return 0 ;
}




















// �`��ݒ�֌W

// ���f���̕`��� SetUseVertexShader, SetUsePixelShader �Ŏw�肵���V�F�[�_�[���g�p���邩�ǂ�����ݒ肷��( TRUE:�g�p����  FALSE:�g�p���Ȃ�( �f�t�H���g ) )
extern int NS_MV1SetUseOrigShader( int UseFlag )
{
	if( MV1Man.UseOrigShaderFlag == UseFlag )
	{
		return 0 ;
	}

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �t���O��ۑ�����
	MV1Man.UseOrigShaderFlag = UseFlag ;

	// �I��
	return 0 ;
}

// ���f���̔������v�f�����镔���ɂ��Ă̕`�惂�[�h��ݒ肷��
extern int NS_MV1SetSemiTransDrawMode( int DrawMode /* DX_SEMITRANSDRAWMODE_ALWAYS �� */ )
{
	if( MV1Man.SemiTransDrawMode == DrawMode )
	{
		return 0 ;
	}

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �`�惂�[�h��ۑ�����
	MV1Man.SemiTransDrawMode = DrawMode ;

	// �I��
	return 0 ;
}






















// �A�^�b�`���Ă���A�j���[�V�����̍Đ����Ԃ�ݒ肷��
extern int NS_MV1SetAttachAnimTime( int MHandle, int AttachIndex, float Time )
{
	MV1_MODEL *Model ;
	MV1_ANIMSET *AnimSet ;

	// ����������Ă��Ȃ�������G���[
	if( MV1Man.Initialize == false ) return -1 ;

	// �A�h���X�擾
	if( MV1MDLCHK( MHandle, Model ) )
		return -1 ;

	// �m�ۂ��Ă���A�^�b�`�C���f�b�N�X�O�������ꍇ�͉��������ɏI��
	if( AttachIndex < 0 || AttachIndex >= Model->AnimSetMaxNum ) return -1 ;

	// �A�j���[�V�������A�^�b�`����Ă��Ȃ������牽�������ɏI��
	if( Model->AnimSet[ AttachIndex ].Use == false ) return -1 ;
	AnimSet = Model->AnimSet[ AttachIndex ].AnimSet ;

	// �p�����[�^�̃Z�b�g�A�b�v�t���O��|��
	AnimSet->ParamSetup = false ;

	// �A�j���[�V�����p�����[�^�����ɂ����s�񂪃Z�b�g�A�b�v����Ă��Ȃ���Ԃɂ���
	Model->AnimSetupFlag = false ;
	Model->LocalWorldMatrixSetupFlag = false ;

	// �A�j���[�V�����̎��Ԃ��Z�b�g����
	MV1SetAnimSetTime( AnimSet, Time ) ;

	// �I��
	return 0 ;
}

// �A�^�b�`���Ă���A�j���[�V�����̍Đ����Ԃ��擾����
extern float NS_MV1GetAttachAnimTime( int MHandle, int AttachIndex )
{
	MV1_MODEL *Model ;

	// ����������Ă��Ȃ�������G���[
	if( MV1Man.Initialize == false ) return -1.0f ;

	// �A�h���X�擾
	if( MV1MDLCHK( MHandle, Model ) )
		return -1.0f ;

	// �m�ۂ��Ă���A�^�b�`�C���f�b�N�X�O�������ꍇ�͉��������ɏI��
	if( AttachIndex < 0 || AttachIndex >= Model->AnimSetMaxNum ) return -1.0f ;

	// �A�j���[�V�������A�^�b�`����Ă��Ȃ������牽�������ɏI��
	if( Model->AnimSet[ AttachIndex ].Use == false ) return -1.0f ;

	// �A�j���[�V�����̎��Ԃ�Ԃ�
	return Model->AnimSet[ AttachIndex ].AnimSet->NowTime ;
}

// �A�^�b�`���Ă���A�j���[�V�����̑����Ԃ𓾂�
extern float NS_MV1GetAttachAnimTotalTime( int MHandle, int AttachIndex )
{
	MV1_MODEL *Model ;

	// ����������Ă��Ȃ�������G���[
	if( MV1Man.Initialize == false ) return -1.0f ;

	// �A�h���X�擾
	if( MV1MDLCHK( MHandle, Model ) )
		return -1.0f ;

	// �m�ۂ��Ă���A�^�b�`�C���f�b�N�X�O�������ꍇ�͉��������ɏI��
	if( AttachIndex < 0 || AttachIndex >= Model->AnimSetMaxNum ) return -1.0f ;

	// �A�j���[�V�������A�^�b�`����Ă��Ȃ������牽�������ɏI��
	if( Model->AnimSet[ AttachIndex ].Use == false ) return -1.0f ;

	// �A�j���[�V�����̑����Ԃ�Ԃ�
	return Model->AnimSet[ AttachIndex ].AnimSet->BaseData->MaxTime ;
}


// �A�^�b�`���Ă���A�j���[�V�����̃u�����h����ݒ肷��
extern int NS_MV1SetAttachAnimBlendRate( int MHandle, int AttachIndex, float Rate )
{
	MV1_MODEL *Model ;
	MV1_MODEL_ANIMSET *MAnimSet ;
	MV1_MODEL_ANIM *MAnim ;
	MV1_FRAME *Frame ;
	int i, FrameNum ;

	// ����������Ă��Ȃ�������G���[
	if( MV1Man.Initialize == false ) return -1 ;

	// �A�h���X�擾
	if( MV1MDLCHK( MHandle, Model ) )
		return -1 ;

	// �m�ۂ��Ă���A�^�b�`�C���f�b�N�X�O�������ꍇ�͉��������ɏI��
	if( AttachIndex < 0 || AttachIndex >= Model->AnimSetMaxNum ) return -1 ;

	// �A�j���[�V�������A�^�b�`����Ă��Ȃ������牽�������ɏI��
	if( Model->AnimSet[ AttachIndex ].Use == false ) return -1 ;
	MAnimSet = &Model->AnimSet[ AttachIndex ] ;

	// �u�����h���[�g�����܂łƓ����ꍇ�͉��������ɏI��
//	if( *( ( DWORD * )&Model->AnimSet[ AttachIndex ].BlendRate ) == *( ( DWORD * )&Rate ) ) return 0 ;

	// �u�����h���[�g��ύX����
	FrameNum = Model->BaseData->FrameNum ;
	MAnim = &Model->Anim[ AttachIndex ] ;
	for( i = 0 ; i < FrameNum ; i ++, MAnim += Model->AnimSetMaxNum )
	{
		MAnim->BlendRate = Rate ;
	}

	// �s��̑S�X�V
	_MEMSET( Model->ChangeMatrixFlag, 0xff, Model->ChangeMatrixFlagSize ) ;

	// �s��̍X�V���ς�ł���t���O��|��
	Model->LocalWorldMatrixSetupFlag = false ;

	// �V�F�C�v�̏�Ԃɕω����������t���O�𗧂Ă�
	Model->ShapeChangeFlag = true ;
	Frame = Model->Frame ;
	for( i = 0 ; i < FrameNum ; i ++, Frame ++ )
	{
		if( Frame->BaseData->ShapeNum == 0 )
			continue ;

		Frame->ShapeChangeFlag = true ;
	}

	// ����I��
	return 0 ;
}

// �A�^�b�`���Ă���A�j���[�V�����̃u�����h�����擾����
extern float NS_MV1GetAttachAnimBlendRate( int MHandle, int AttachIndex )
{
	MV1_MODEL *Model ;

	// ����������Ă��Ȃ�������G���[
	if( MV1Man.Initialize == false ) return -1.0f ;

	// �A�h���X�擾
	if( MV1MDLCHK( MHandle, Model ) )
		return -1.0f ;

	// �m�ۂ��Ă���A�^�b�`�C���f�b�N�X�O�������ꍇ�͉��������ɏI��
	if( AttachIndex < 0 || AttachIndex >= Model->AnimSetMaxNum ) return -1.0f ;

	// �A�j���[�V�������A�^�b�`����Ă��Ȃ������牽�������ɏI��
	if( Model->AnimSet[ AttachIndex ].Use == false ) return -1.0f ;

	// �u�����h���[�g��Ԃ�
	return Model->Anim[ AttachIndex ].BlendRate ;
}

// �A�^�b�`���Ă���A�j���[�V�����̃u�����h����ݒ肷��( �t���[���P�� )
extern int NS_MV1SetAttachAnimBlendRateToFrame( int MHandle, int AttachIndex, int FrameIndex, float Rate, int SetChild )
{
	MV1_MODEL *Model ;
	MV1_MODEL_ANIM *MAnim ;
	MV1_FRAME *Frame ;
	int i ;

	// ����������Ă��Ȃ�������G���[
	if( MV1Man.Initialize == false ) return -1 ;

	// �A�h���X�擾
	if( MV1MDLCHK( MHandle, Model ) )
		return -1 ;

	// �m�ۂ��Ă���A�^�b�`�C���f�b�N�X�O�������ꍇ�͉��������ɏI��
	if( AttachIndex < 0 || AttachIndex >= Model->AnimSetMaxNum ) return -1 ;

	// �t���[���C���f�b�N�X���s���������牽�������ɏI��
	if( FrameIndex < 0 || FrameIndex >= Model->BaseData->FrameNum ) return -1 ;
	Frame = &Model->Frame[ FrameIndex ] ;
	MAnim = &Model->Anim[ AttachIndex ] + Model->AnimSetMaxNum * FrameIndex ;

	// �A�j���[�V�������A�^�b�`����Ă��Ȃ������牽�������ɏI��
	if( Model->AnimSet[ AttachIndex ].Use == false ) return -1 ;

	// �u�����h���[�g�����܂łƓ����ꍇ�͉��������ɏI��
	if( *( ( DWORD * )&MAnim->BlendRate ) == *( ( DWORD * )&Rate ) ) return 0 ;

	// �u�����h���[�g��ύX����
	MAnim->BlendRate = Rate ;

	// �q�t���[�����Z�b�g����w�肪�������ꍇ�͎q�t���[�����Z�b�g����
	if( SetChild )
	{
		MV1_MODEL_ANIM *MAnim2 ;

		MAnim2 = MAnim + Model->AnimSetMaxNum ;
		for( i = 0 ; i < Frame->BaseData->TotalChildNum ; i ++, MAnim2 += Model->AnimSetMaxNum )
			MAnim2->BlendRate = Rate ;
	}

	// �s�񂪃Z�b�g�A�b�v����Ă��Ȃ���Ԃɂ���
	Model->LocalWorldMatrixSetupFlag = false ;
	MV1BitSetChange( &Model->Frame[ FrameIndex ].LocalWorldMatrixChange ) ;

	// ����I��
	return 0 ;
}

// �A�^�b�`���Ă���A�j���[�V�����̃u�����h����ݒ肷��( �t���[���P�� )
extern float NS_MV1GetAttachAnimBlendRateToFrame( int MHandle, int AttachIndex, int FrameIndex )
{
	MV1_MODEL *Model ;
	MV1_MODEL_ANIM *MAnim ;

	// ����������Ă��Ȃ�������G���[
	if( MV1Man.Initialize == false ) return -1.0f ;

	// �A�h���X�擾
	if( MV1MDLCHK( MHandle, Model ) )
		return -1.0f ;

	// �m�ۂ��Ă���A�^�b�`�C���f�b�N�X�O�������ꍇ�͉��������ɏI��
	if( AttachIndex < 0 || AttachIndex >= Model->AnimSetMaxNum ) return -1.0f ;

	// �t���[���C���f�b�N�X���s���������牽�������ɏI��
	if( FrameIndex < 0 || FrameIndex >= Model->BaseData->FrameNum ) return -1.0f ;
	MAnim = &Model->Anim[ AttachIndex ] + Model->AnimSetMaxNum * FrameIndex ;

	// �A�j���[�V�������A�^�b�`����Ă��Ȃ������牽�������ɏI��
	if( Model->AnimSet[ AttachIndex ].Use == false ) return -1.0f ;

	// �u�����h���[�g��Ԃ�
	return MAnim->BlendRate ;
}

// �A�^�b�`���Ă���A�j���[�V�������^�[�Q�b�g�Ƃ���t���[���̐����擾����
extern int MV1GetAttachAnimTargetFrameNum( int MHandle, int AttachIndex )
{
	MV1_MODEL *Model ;
	MV1_MODEL_ANIMSET *MAnimSet ;

	// ����������Ă��Ȃ�������G���[
	if( MV1Man.Initialize == false ) return -1 ;

	// �A�h���X�擾
	if( MV1MDLCHK( MHandle, Model ) )
		return -1 ;

	// �m�ۂ��Ă���A�^�b�`�C���f�b�N�X�O�������ꍇ�͉��������ɏI��
	if( AttachIndex < 0 || AttachIndex >= Model->AnimSetMaxNum ) return -1 ;
	MAnimSet = &Model->AnimSet[ AttachIndex ] ;

	// �A�j���[�V�������A�^�b�`����Ă��Ȃ������牽�������ɏI��
	if( MAnimSet->Use == false ) return -1 ;

	// �K�����Ă���A�j���[�V�����̐���Ԃ�
	return MAnimSet->UseAnimNum ;
}

// �A�^�b�`���Ă���A�j���[�V�������^�[�Q�b�g�Ƃ���t���[���̃C���f�b�N�X����������
extern int MV1GetAttachAnimTargetFrame( int MHandle, int AttachIndex, int Index )
{
	MV1_MODEL *Model ;
	MV1_MODEL_ANIM *MAnim ;
	MV1_MODEL_ANIMSET *MAnimSet ;
	int con ;

	// ����������Ă��Ȃ�������G���[
	if( MV1Man.Initialize == false ) return -1 ;

	// �A�h���X�擾
	if( MV1MDLCHK( MHandle, Model ) )
		return -1 ;

	// �m�ۂ��Ă���A�^�b�`�C���f�b�N�X�O�������ꍇ�͉��������ɏI��
	if( AttachIndex < 0 || AttachIndex >= Model->AnimSetMaxNum ) return -1 ;
	MAnimSet = &Model->AnimSet[ AttachIndex ] ;

	// �A�j���[�V�������A�^�b�`����Ă��Ȃ������牽�������ɏI��
	if( MAnimSet->Use == false ) return -1 ;

	// �s���ȃC���f�b�N�X�̏ꍇ�͉��������ɏI��
	if( Index < 0 || Index >= MAnimSet->UseAnimNum )
		return -1 ;

	// �w��̗L���C���f�b�N�X�̃t���[����T��
	con = 0 ;
	for( MAnim = &Model->Anim[ AttachIndex ] ; ; MAnim += Model->AnimSetMaxNum )
	{
		if( MAnim->Use == false ) continue ;
		if( con == Index ) break ;
		con ++ ;
	}

	// �t���[���̃C���f�b�N�X��Ԃ�
	return ( int )( MAnim->Anim->Frame - Model->Frame ) ;
}

// �A�^�b�`���Ă���A�j���[�V�����̃A�j���[�V�����C���f�b�N�X���擾����
extern int NS_MV1GetAttachAnim( int MHandle, int AttachIndex )
{
	MV1_MODEL *Model ;
	MV1_MODEL_ANIMSET *MAnimSet ;

	// ����������Ă��Ȃ�������G���[
	if( MV1Man.Initialize == false ) return -1 ;

	// �A�h���X�擾
	if( MV1MDLCHK( MHandle, Model ) )
		return -1 ;

	// �m�ۂ��Ă���A�^�b�`�C���f�b�N�X�O�������ꍇ�͉��������ɏI��
	if( AttachIndex < 0 || AttachIndex >= Model->AnimSetMaxNum ) return -1 ;
	MAnimSet = &Model->AnimSet[ AttachIndex ] ;

	// �A�j���[�V�������A�^�b�`����Ă��Ȃ������牽�������ɏI��
	if( MAnimSet->Use == false ) return -1 ;

	// �A�j���[�V�����C���f�b�N�X��Ԃ�
	return MAnimSet->BaseDataAnimIndex ;
}

// �A�^�b�`���Ă���A�j���[�V�����̃V�F�C�v���g�p���邩�ǂ�����ݒ肷��( UseFlag  TRUE:�g�p����( �f�t�H���g )  FALSE:�g�p���Ȃ� )
extern int NS_MV1SetAttachAnimUseShapeFlag( int MHandle, int AttachIndex, int UseFlag )
{
	MV1_MODEL *Model ;
	MV1_MODEL_ANIMSET *MAnimSet ;
	MV1_FRAME *Frame ;
	int i ;
	int FrameNum ;
	bool bDisableFlag ;

	bDisableFlag = UseFlag == FALSE;

	// ����������Ă��Ȃ�������G���[
	if( MV1Man.Initialize == false ) return -1 ;

	// �A�h���X�擾
	if( MV1MDLCHK( MHandle, Model ) )
		return -1 ;

	// �m�ۂ��Ă���A�^�b�`�C���f�b�N�X�O�������ꍇ�͉��������ɏI��
	if( AttachIndex < 0 || AttachIndex >= Model->AnimSetMaxNum ) return -1 ;

	// �A�j���[�V�������A�^�b�`����Ă��Ȃ������牽�������ɏI��
	if( Model->AnimSet[ AttachIndex ].Use == false ) return -1 ;
	MAnimSet = &Model->AnimSet[ AttachIndex ] ;

	// �t���O�����܂łƓ����ꍇ�͉��������ɏI��
	if( MAnimSet->DisableShapeFlag == bDisableFlag ) return 0 ;

	// �t���O��ۑ�
	MAnimSet->DisableShapeFlag = bDisableFlag ;

	// �V�F�C�v�̏�Ԃɕω����������t���O�𗧂Ă�
	Model->ShapeChangeFlag = true ;
	Frame = Model->Frame ;
	FrameNum = Model->BaseData->FrameNum ;
	for( i = 0 ; i < FrameNum ; i ++, Frame ++ )
	{
		if( Frame->BaseData->MeshNum > 0 && Frame->Mesh->BaseData->Shape == 0 )
			continue ;

		Frame->ShapeChangeFlag = true ;
	}

	// ����I��
	return 0 ;
}

// �A�^�b�`���Ă���A�j���[�V�����̃V�F�C�v���g�p���邩�ǂ������擾����
extern int NS_MV1GetAttachAnimUseShapeFlag( int MHandle, int AttachIndex )
{
	MV1_MODEL *Model ;
	MV1_MODEL_ANIMSET *MAnimSet ;

	// ����������Ă��Ȃ�������G���[
	if( MV1Man.Initialize == false ) return -1 ;

	// �A�h���X�擾
	if( MV1MDLCHK( MHandle, Model ) )
		return -1 ;

	// �m�ۂ��Ă���A�^�b�`�C���f�b�N�X�O�������ꍇ�͉��������ɏI��
	if( AttachIndex < 0 || AttachIndex >= Model->AnimSetMaxNum ) return -1 ;

	// �A�j���[�V�������A�^�b�`����Ă��Ȃ������牽�������ɏI��
	if( Model->AnimSet[ AttachIndex ].Use == false ) return -1 ;
	MAnimSet = &Model->AnimSet[ AttachIndex ] ;

	// �t���O��Ԃ�
	return MAnimSet->DisableShapeFlag ? FALSE : TRUE ;
}

// �A�^�b�`���Ă���A�j���[�V�����̎w��̃t���[���̌��݂̃��[�J���ϊ��s����擾����
extern MATRIX NS_MV1GetAttachAnimFrameLocalMatrix(	int MHandle, int AttachIndex, int FrameIndex )
{
	static MATRIX ErrorValue = { -1.0f, -1.0f, -1.0f, -1.0f,   -1.0f, -1.0f, -1.0f, -1.0f,   -1.0f, -1.0f, -1.0f, -1.0f,   -1.0f, -1.0f, -1.0f, -1.0f  } ;
	MV1_MODEL *Model ;
	MATRIX ResultMatrix ;
	MV1_MODEL_ANIM *MAnim ;
	MV1_FRAME *Frame ;
	MV1_FRAME_BASE *FrameBase ;

	// ����������Ă��Ȃ�������G���[
	if( MV1Man.Initialize == false )
		return ErrorValue ;

	// �A�h���X�擾
	if( MV1MDLCHK( MHandle, Model ) )
		return ErrorValue ;

	// ���݂��Ȃ��t���[���ԍ��������ꍇ�͉��������ɏI��
	if( FrameIndex < 0 || FrameIndex >= Model->BaseData->FrameNum )
		return ErrorValue ;

	// �m�ۂ��Ă���A�^�b�`�C���f�b�N�X�O�������ꍇ�͉��������ɏI��
	if( AttachIndex < 0 || AttachIndex >= Model->AnimSetMaxNum )
		return ErrorValue ;

	// �A�j���[�V�������A�^�b�`����Ă��Ȃ������牽�������ɏI��
	if( Model->AnimSet[ AttachIndex ].Use == false )
		return ErrorValue ;

	// �s��̃Z�b�g�A�b�v���s��
	MV1SETUPMATRIX( Model ) ;

	Frame = &Model->Frame[ FrameIndex ] ;
	FrameBase = Frame->BaseData ;
	MAnim = Model->Anim + Model->AnimSetMaxNum * FrameBase->Index + AttachIndex ;

	// �A�j���[�V���������݂��Ȃ�������P�ʍs���Ԃ�
	if( MAnim->Use == false )
	{
		return MGetIdent() ;
	}

	if( MAnim->Anim->ValidFlag & MV1_ANIMVALUE_MATRIX )
	{
		ConvertMatrix4x4cFToMatrixF( &ResultMatrix, &MAnim->Anim->Matrix ) ;
	}
	else
	{
		// �s��̃Z�b�g�A�b�v
		if( MAnim->Anim->ValidBlendMatrix == false )
		{
			MV1SetupTransformMatrix(
				&MAnim->Anim->BlendMatrix,
				MAnim->Anim->ValidFlag,
				&MAnim->Anim->Translate,
				&MAnim->Anim->Scale,
				MAnim->Anim->RotateOrder,
				( FrameBase->Flag & MV1_FRAMEFLAG_PREROTATE ) != 0 ? &FrameBase->PreRotate : NULL,
				&MAnim->Anim->Rotate,
				( FrameBase->Flag & MV1_FRAMEFLAG_POSTROTATE ) != 0 ? &FrameBase->PostRotate : NULL,
				&MAnim->Anim->Quaternion
			) ;
			MAnim->Anim->ValidBlendMatrix = true ;
		}
		ConvertMatrix4x4cFToMatrixF( &ResultMatrix, &MAnim->Anim->BlendMatrix ) ;
	}

	// ���[�J���s���Ԃ�
	return ResultMatrix ;
}

// �A�^�b�`���Ă���A�j���[�V�����̎w��̃t���[���̌��݂̃��[�J�����W���擾����
extern VECTOR NS_MV1GetAttachAnimFrameLocalPosition( int MHandle, int AttachIndex, int FrameIndex )
{
	static VECTOR ErrorValue = { -1.0f, -1.0f, -1.0f } ;
	MV1_MODEL *Model ;
	VECTOR ResultPosition ;
	MV1_MODEL_ANIM *MAnim ;
	MV1_FRAME *Frame ;
	MV1_FRAME_BASE *FrameBase ;

	// ����������Ă��Ȃ�������G���[
	if( MV1Man.Initialize == false )
		return ErrorValue ;

	// �A�h���X�擾
	if( MV1MDLCHK( MHandle, Model ) )
		return ErrorValue ;

	// ���݂��Ȃ��t���[���ԍ��������ꍇ�͉��������ɏI��
	if( FrameIndex < 0 || FrameIndex >= Model->BaseData->FrameNum )
		return ErrorValue ;

	// �m�ۂ��Ă���A�^�b�`�C���f�b�N�X�O�������ꍇ�͉��������ɏI��
	if( AttachIndex < 0 || AttachIndex >= Model->AnimSetMaxNum )
		return ErrorValue ;

	// �A�j���[�V�������A�^�b�`����Ă��Ȃ������牽�������ɏI��
	if( Model->AnimSet[ AttachIndex ].Use == false )
		return ErrorValue ;

	// �s��̃Z�b�g�A�b�v���s��
	MV1SETUPMATRIX( Model ) ;

	Frame = &Model->Frame[ FrameIndex ] ;
	FrameBase = Frame->BaseData ;
	MAnim = Model->Anim + Model->AnimSetMaxNum * FrameBase->Index + AttachIndex ;

	// �A�j���[�V���������݂��Ȃ�������O�x�N�g����Ԃ�
	if( MAnim->Use == false )
	{
		return VGet( 0.0f, 0.0f, 0.0f ) ;
	}

	if( MAnim->Anim->ValidFlag & MV1_ANIMVALUE_MATRIX )
	{
		ResultPosition.x = MAnim->Anim->Matrix.m[ 0 ][ 3 ] ;
		ResultPosition.y = MAnim->Anim->Matrix.m[ 1 ][ 3 ] ;
		ResultPosition.z = MAnim->Anim->Matrix.m[ 2 ][ 3 ] ;
	}
	else
	{
		// �s��̃Z�b�g�A�b�v
		if( MAnim->Anim->ValidBlendMatrix == false )
		{
			MV1SetupTransformMatrix(
				&MAnim->Anim->BlendMatrix,
				MAnim->Anim->ValidFlag,
				&MAnim->Anim->Translate,
				&MAnim->Anim->Scale,
				MAnim->Anim->RotateOrder,
				( FrameBase->Flag & MV1_FRAMEFLAG_PREROTATE ) != 0 ? &FrameBase->PreRotate : NULL,
				&MAnim->Anim->Rotate,
				( FrameBase->Flag & MV1_FRAMEFLAG_POSTROTATE ) != 0 ? &FrameBase->PostRotate : NULL,
				&MAnim->Anim->Quaternion
			) ;
			MAnim->Anim->ValidBlendMatrix = true ;
		}
		ResultPosition.x = MAnim->Anim->BlendMatrix.m[ 0 ][ 3 ] ;
		ResultPosition.y = MAnim->Anim->BlendMatrix.m[ 1 ][ 3 ] ;
		ResultPosition.z = MAnim->Anim->BlendMatrix.m[ 2 ][ 3 ] ;
	}

	// ���[�J�����W��Ԃ�
	return ResultPosition ;
}


// �w��̃t���[�����A�^�b�`���Ă���A�j���[�V�����̉��Ԗڂ̃^�[�Q�b�g�t���[�������擾����( AnimFrameIndex �Ƃ��Ďg�p���� )
extern int MV1GetAttachAnimTargetFrameToAnimFrameIndex( int MHandle, int AttachIndex, int FrameIndex )
{
	MV1_MODEL *Model ;
	MV1_MODEL_BASE *ModelBase ;
	MV1_MODEL_ANIMSET *MAnimSet ;
	MV1_MODEL_ANIM *MAnim ;

	// ����������Ă��Ȃ�������G���[
	if( MV1Man.Initialize == false ) return -1 ;

	// �A�h���X�擾
	if( MV1MDLCHK( MHandle, Model ) )
		return -1 ;
	ModelBase = Model->BaseData ;

	// �m�ۂ��Ă���A�^�b�`�C���f�b�N�X�O�������ꍇ�͉��������ɏI��
	if( AttachIndex < 0 || AttachIndex >= Model->AnimSetMaxNum ) return -1 ;
	MAnimSet = &Model->AnimSet[ AttachIndex ] ;

	// �t���[���C���f�b�N�X���s���������牽�������I��
	if( FrameIndex < 0 || FrameIndex >= ModelBase->FrameNum )
		return -1 ;

	// �A�j���[�V�������A�^�b�`����Ă��Ȃ������牽�������ɏI��
	if( MAnimSet->Use == false ) return -1 ;

	// �w��̃t���[���ɃA�j���[�V�������A�^�b�`����Ă��Ȃ������牽�������ɏI��
	MAnim = &Model->Anim[ AttachIndex + Model->AnimSetMaxNum * FrameIndex ] ;
	if( MAnim->Use == false ) return -1 ;

	// �w��̃t���[���Ŏg�p����Ă���A�j���[�V�������t���[���C���f�b�N�X��Ԃ�
	return ( int )( MAnim->Anim->BaseData - MAnimSet->AnimSet->BaseData->Anim ) ;
}

// �A�j���[�V�����̐����擾����
extern int NS_MV1GetAnimNum( int MHandle )
{
	MV1_MODEL *Model ;

	// ����������Ă��Ȃ�������G���[
	if( MV1Man.Initialize == false ) return -1 ;

	// �A�h���X�擾
	if( MV1MDLCHK( MHandle, Model ) )
		return -1 ;

	// �A�j���[�V�����̐���Ԃ�
	return Model->BaseData->AnimSetNum ;
}

// �w��ԍ��̃A�j���[�V���������擾����
extern const TCHAR *NS_MV1GetAnimName( int MHandle, int AnimIndex )
{
	MV1_MODEL *Model ;

	// ����������Ă��Ȃ�������G���[
	if( MV1Man.Initialize == false ) return NULL ;

	// �A�h���X�擾
	if( MV1MDLCHK( MHandle, Model ) )
		return NULL ;

	// �C���f�b�N�X���A�j���[�V�����̐��𒴂��Ă�����G���[
	if( Model->BaseData->AnimSetNum <= AnimIndex ) return NULL ;

	// �A�j���[�V��������Ԃ�
#ifdef UNICODE
	return Model->BaseData->AnimSet[ AnimIndex ].NameW ;
#else
	return Model->BaseData->AnimSet[ AnimIndex ].NameA ;
#endif
}

// �w��ԍ��̃A�j���[�V���������擾����
extern const wchar_t *MV1GetAnimName_WCHAR_T( int MHandle, int AnimIndex )
{
	MV1_MODEL *Model ;

	// ����������Ă��Ȃ�������G���[
	if( MV1Man.Initialize == false ) return NULL ;

	// �A�h���X�擾
	if( MV1MDLCHK( MHandle, Model ) )
		return NULL ;

	// �C���f�b�N�X���A�j���[�V�����̐��𒴂��Ă�����G���[
	if( Model->BaseData->AnimSetNum <= AnimIndex ) return NULL ;

	// �A�j���[�V��������Ԃ�
	return Model->BaseData->AnimSet[ AnimIndex ].NameW ;
}

// �w��ԍ��̃A�j���[�V��������ύX����
extern int NS_MV1SetAnimName( int MHandle, int AnimIndex, const TCHAR *AnimName )
{
#ifdef UNICODE
	return MV1SetAnimName_WCHAR_T(
		MHandle, AnimIndex, AnimName
	) ;
#else
	int Result ;

	TCHAR_TO_WCHAR_T_STRING_ONE_BEGIN( AnimName, return -1 )

	Result = MV1SetAnimName_WCHAR_T(
		MHandle, AnimIndex, UseAnimNameBuffer
	) ;

	TCHAR_TO_WCHAR_T_STRING_END( AnimName )

	return Result ;
#endif
}

// �w��ԍ��̃A�j���[�V��������ύX����
extern int MV1SetAnimName_WCHAR_T( int MHandle, int AnimIndex, const wchar_t *AnimName )
{
	MV1_MODEL *Model ;
	MV1_MODEL_BASE *MBase ;
	MV1_ANIMSET_BASE *AnimSetBase ;

	// ����������Ă��Ȃ�������G���[
	if( MV1Man.Initialize == false ) return -1 ;

	// �A�h���X�擾
	if( MV1MDLCHK( MHandle, Model ) )
		return -1 ;
	MBase = Model->BaseData ;

	// �C���f�b�N�X���A�j���[�V�����̐��𒴂��Ă�����G���[
	if( Model->BaseData->AnimSetNum <= AnimIndex ) return -1 ;
	AnimSetBase = &MBase->AnimSet[ AnimIndex ] ;

	if( AnimSetBase->NameAllocMem )
	{
#ifndef UNICODE
		if( AnimSetBase->NameA != NULL )
		{
			DXFREE( AnimSetBase->NameA ) ;
			AnimSetBase->NameA = NULL ;
		}
#endif
		if( AnimSetBase->NameW != NULL )
		{
			DXFREE( AnimSetBase->NameW ) ;
			AnimSetBase->NameW = NULL ;
		}
	}

	AnimSetBase->NameAllocMem = TRUE ;

	if( _MV1AllocAndWideCharNameCopy( AnimName, &AnimSetBase->NameW ) == false )
	{
		return -1 ;
	}

#ifndef UNICODE
	if( _MV1CreateMultiByteName( AnimName, &AnimSetBase->NameA ) == false )
	{
		return -1 ;
	}
#endif

	// ����I��
	return 0 ;
}

// �w�薼�̃A�j���[�V�����ԍ����擾����( -1:�G���[ )
extern int NS_MV1GetAnimIndex( int MHandle, const TCHAR *AnimName )
{
#ifdef UNICODE
	return MV1GetAnimIndex_WCHAR_T(
		MHandle, AnimName
	) ;
#else
	int Result ;

	TCHAR_TO_WCHAR_T_STRING_ONE_BEGIN( AnimName, return -1 )

	Result = MV1GetAnimIndex_WCHAR_T(
		MHandle, UseAnimNameBuffer
	) ;

	TCHAR_TO_WCHAR_T_STRING_END( AnimName )

	return Result ;
#endif
}

// �w�薼�̃A�j���[�V�����ԍ����擾����( -1:�G���[ )
extern int MV1GetAnimIndex_WCHAR_T( int MHandle, const wchar_t *AnimName )
{
	MV1_MODEL *Model ;
	MV1_MODEL_BASE *MBase ;
	int i ;

	// ����������Ă��Ȃ�������G���[
	if( MV1Man.Initialize == false ) return -1 ;

	// �A�h���X�擾
	if( MV1MDLCHK( MHandle, Model ) )
		return -1 ;
	MBase = Model->BaseData ;

	// �����̃A�j���[�V������T��
	for( i = 0 ; i < MBase->AnimSetNum && _WCSCMP( MBase->AnimSet[ i ].NameW, AnimName ) != 0 ; i ++ ){}

	return i == MBase->AnimSetNum ? -1 : i ;
}

// �w��ԍ��̃A�j���[�V�����̑����Ԃ𓾂�
extern float NS_MV1GetAnimTotalTime( int MHandle, int AnimIndex )
{
	MV1_MODEL *Model ;
	MV1_ANIMSET_BASE *AnimSetBase ;

	// ����������Ă��Ȃ�������G���[
	if( MV1Man.Initialize == false ) return -1.0f ;

	// �A�h���X�擾
	if( MV1MDLCHK( MHandle, Model ) )
		return -1.0f ;

	// �C���f�b�N�X���s����������G���[
	if( AnimIndex < 0 || AnimIndex >= Model->BaseData->AnimSetNum )
		return -1 ;

	// �A�j���[�V�������A�^�b�`����Ă��Ȃ������牽�������ɏI��
	AnimSetBase = MV1GetAnimSetBase( Model->BaseDataHandle, NULL, AnimIndex ) ;
	if( AnimSetBase == NULL ) return -1.0f ;

	// �A�j���[�V�����̑����Ԃ�Ԃ�
	return AnimSetBase->MaxTime ;
}

// �w��̃A�j���[�V���������[�v�^�C�v���ǂ������擾����( �߂�l  TRUE:���[�v�^�C�v  FALSE:�ʏ�^�C�v )
extern int NS_MV1GetAnimLoopFlag( int MHandle, int AnimIndex )
{
	MV1_MODEL *Model ;
	MV1_MODEL_BASE *ModelBase ;
	MV1_ANIMSET_BASE *AnimSetBase ;

	// ����������Ă��Ȃ�������G���[
	if( MV1Man.Initialize == false ) return -1 ;

	// �A�h���X�擾
	if( MV1MDLCHK( MHandle, Model ) )
		return -1 ;
	ModelBase = Model->BaseData ;

	// �C���f�b�N�X���s���������ꍇ�͉��������ɏI��
	if( AnimIndex < 0 || AnimIndex >= ModelBase->AnimSetNum )
		return -1 ;
	AnimSetBase = &ModelBase->AnimSet[ AnimIndex ] ;

	// �A�j���[�V�����̃��[�v�t���O��Ԃ�
	return AnimSetBase->IsLoopAnim != FALSE ? TRUE : FALSE ;
}

// �w��̃A�j���[�V�������^�[�Q�b�g�Ƃ���t���[���̐����擾����
extern int NS_MV1GetAnimTargetFrameNum( int MHandle, int AnimIndex )
{
	MV1_MODEL *Model ;
	MV1_MODEL_BASE *ModelBase ;
	MV1_ANIMSET_BASE *AnimSetBase ;

	// ����������Ă��Ȃ�������G���[
	if( MV1Man.Initialize == false ) return -1 ;

	// �A�h���X�擾
	if( MV1MDLCHK( MHandle, Model ) )
		return -1 ;
	ModelBase = Model->BaseData ;

	// �C���f�b�N�X���s���������ꍇ�͉��������ɏI��
	if( AnimIndex < 0 || AnimIndex >= ModelBase->AnimSetNum )
		return -1 ;
	AnimSetBase = &ModelBase->AnimSet[ AnimIndex ] ;

	// �^�[�Q�b�g�Ƃ���t���[���̐���Ԃ�
	return AnimSetBase->AnimNum ;
}

// �w��̃A�j���[�V�������^�[�Q�b�g�Ƃ���t���[���̖��O���擾����
extern const TCHAR *NS_MV1GetAnimTargetFrameName( int MHandle, int AnimIndex, int AnimFrameIndex )
{
	MV1_MODEL *Model ;
	MV1_MODEL_BASE *ModelBase ;
	MV1_ANIMSET_BASE *AnimSetBase ;
	MV1_ANIM_BASE *AnimBase ;

	// ����������Ă��Ȃ�������G���[
	if( MV1Man.Initialize == false ) return NULL ;

	// �A�h���X�擾
	if( MV1MDLCHK( MHandle, Model ) )
		return NULL ;
	ModelBase = Model->BaseData ;

	// �C���f�b�N�X���s���������ꍇ�͉��������ɏI��
	if( AnimIndex < 0 || AnimIndex >= ModelBase->AnimSetNum )
		return NULL ;
	AnimSetBase = &ModelBase->AnimSet[ AnimIndex ] ;

	// �C���f�b�N�X���s���������ꍇ�͉��������ɏI��
	if( AnimFrameIndex < 0 || AnimFrameIndex >= AnimSetBase->AnimNum )
		return NULL ;
	AnimBase = &AnimSetBase->Anim[ AnimFrameIndex ] ;

	// �^�[�Q�b�g�Ƃ���t���[���̖��O��Ԃ�
#ifdef UNICODE
	return AnimBase->TargetFrame->NameW ;
#else
	return AnimBase->TargetFrame->NameA ;
#endif
}

// �w��̃A�j���[�V�������^�[�Q�b�g�Ƃ���t���[���̖��O���擾����
extern const wchar_t *MV1GetAnimTargetFrameName_WCHAR_T( int MHandle, int AnimIndex, int AnimFrameIndex )
{
	MV1_MODEL *Model ;
	MV1_MODEL_BASE *ModelBase ;
	MV1_ANIMSET_BASE *AnimSetBase ;
	MV1_ANIM_BASE *AnimBase ;

	// ����������Ă��Ȃ�������G���[
	if( MV1Man.Initialize == false ) return NULL ;

	// �A�h���X�擾
	if( MV1MDLCHK( MHandle, Model ) )
		return NULL ;
	ModelBase = Model->BaseData ;

	// �C���f�b�N�X���s���������ꍇ�͉��������ɏI��
	if( AnimIndex < 0 || AnimIndex >= ModelBase->AnimSetNum )
		return NULL ;
	AnimSetBase = &ModelBase->AnimSet[ AnimIndex ] ;

	// �C���f�b�N�X���s���������ꍇ�͉��������ɏI��
	if( AnimFrameIndex < 0 || AnimFrameIndex >= AnimSetBase->AnimNum )
		return NULL ;
	AnimBase = &AnimSetBase->Anim[ AnimFrameIndex ] ;

	// �^�[�Q�b�g�Ƃ���t���[���̖��O��Ԃ�
	return AnimBase->TargetFrame->NameW ;
}

// �w��̃A�j���[�V�������^�[�Q�b�g�Ƃ���t���[���̔ԍ����擾����
extern int NS_MV1GetAnimTargetFrame( int MHandle, int AnimIndex, int AnimFrameIndex )
{
	MV1_MODEL *Model ;
	MV1_MODEL_BASE *ModelBase ;
	MV1_ANIMSET_BASE *AnimSetBase ;
	MV1_ANIM_BASE *AnimBase ;

	// ����������Ă��Ȃ�������G���[
	if( MV1Man.Initialize == false ) return -1 ;

	// �A�h���X�擾
	if( MV1MDLCHK( MHandle, Model ) )
		return -1 ;
	ModelBase = Model->BaseData ;

	// �C���f�b�N�X���s���������ꍇ�͉��������ɏI��
	if( AnimIndex < 0 || AnimIndex >= ModelBase->AnimSetNum )
		return -1 ;
	AnimSetBase = &ModelBase->AnimSet[ AnimIndex ] ;

	// �C���f�b�N�X���s���������ꍇ�͉��������ɏI��
	if( AnimFrameIndex < 0 || AnimFrameIndex >= AnimSetBase->AnimNum )
		return -1 ;
	AnimBase = &AnimSetBase->Anim[ AnimFrameIndex ] ;

	// �^�[�Q�b�g�Ƃ���t���[���̖��O��Ԃ�
	return AnimBase->TargetFrameIndex ;
}

// �w��̃A�j���[�V�������^�[�Q�b�g�Ƃ���t���[���p�̃A�j���[�V�����L�[�Z�b�g�̐����擾����
extern int NS_MV1GetAnimTargetFrameKeySetNum( int MHandle, int AnimIndex, int AnimFrameIndex )
{
	MV1_MODEL *Model ;
	MV1_MODEL_BASE *ModelBase ;
	MV1_ANIMSET_BASE *AnimSetBase ;
	MV1_ANIM_BASE *AnimBase ;

	// ����������Ă��Ȃ�������G���[
	if( MV1Man.Initialize == false ) return -1 ;

	// �A�h���X�擾
	if( MV1MDLCHK( MHandle, Model ) )
		return -1 ;
	ModelBase = Model->BaseData ;

	// �C���f�b�N�X���s���������ꍇ�͉��������ɏI��
	if( AnimIndex < 0 || AnimIndex >= ModelBase->AnimSetNum )
		return -1 ;
	AnimSetBase = &ModelBase->AnimSet[ AnimIndex ] ;

	// �C���f�b�N�X���s���������ꍇ�͉��������ɏI��
	if( AnimFrameIndex < 0 || AnimFrameIndex >= AnimSetBase->AnimNum )
		return -1 ;
	AnimBase = &AnimSetBase->Anim[ AnimFrameIndex ] ;

	// �L�[�Z�b�g�̐���Ԃ�
	return AnimBase->KeySetNum ;
}

// �w��̃A�j���[�V�������^�[�Q�b�g�Ƃ���t���[���p�̃A�j���[�V�����L�[�Z�b�g�L�[�Z�b�g�C���f�b�N�X���擾����
extern int NS_MV1GetAnimTargetFrameKeySet( int MHandle, int AnimIndex, int AnimFrameIndex, int Index )
{
	MV1_MODEL *Model ;
	MV1_MODEL_BASE *ModelBase ;
	MV1_ANIMSET_BASE *AnimSetBase ;
	MV1_ANIM_BASE *AnimBase ;

	// ����������Ă��Ȃ�������G���[
	if( MV1Man.Initialize == false ) return -1 ;

	// �A�h���X�擾
	if( MV1MDLCHK( MHandle, Model ) )
		return -1 ;
	ModelBase = Model->BaseData ;

	// �C���f�b�N�X���s���������ꍇ�͉��������ɏI��
	if( AnimIndex < 0 || AnimIndex >= ModelBase->AnimSetNum )
		return -1 ;
	AnimSetBase = &ModelBase->AnimSet[ AnimIndex ] ;

	// �C���f�b�N�X���s���������ꍇ�͉��������ɏI��
	if( AnimFrameIndex < 0 || AnimFrameIndex >= AnimSetBase->AnimNum )
		return -1 ;
	AnimBase = &AnimSetBase->Anim[ AnimFrameIndex ] ;

	// �C���f�b�N�X���s���������ꍇ�͉��������ɏI��
	if( Index < 0 || Index >= AnimBase->KeySetNum )
		return -1 ;

	// �L�[�Z�b�g�C���f�b�N�X��Ԃ�
	return ( int )( &AnimBase->KeySet[ Index ] - ModelBase->AnimKeySet ) ;
}

// ���f���Ɋ܂܂��A�j���[�V�����L�[�Z�b�g�̑����𓾂�
extern int NS_MV1GetAnimKeySetNum( int MHandle )
{
	MV1_MODEL *Model ;
	MV1_MODEL_BASE *ModelBase ;

	// ����������Ă��Ȃ�������G���[
	if( MV1Man.Initialize == false ) return -1 ;

	// �A�h���X�擾
	if( MV1MDLCHK( MHandle, Model ) )
		return -1 ;
	ModelBase = Model->BaseData ;

	// �A�j���[�V�����L�[�Z�b�g�̐���Ԃ�
	return ModelBase->AnimKeySetNum ;
}

// �w��̃A�j���[�V�����L�[�Z�b�g�̃^�C�v���擾����
extern int NS_MV1GetAnimKeySetType( int MHandle, int AnimKeySetIndex )
{
	MV1_MODEL *Model ;
	MV1_MODEL_BASE *ModelBase ;
	MV1_ANIM_KEYSET_BASE *AnimKeySetBase ;

	// ����������Ă��Ȃ�������G���[
	if( MV1Man.Initialize == false ) return -1 ;

	// �A�h���X�擾
	if( MV1MDLCHK( MHandle, Model ) )
		return -1 ;
	ModelBase = Model->BaseData ;

	// �C���f�b�N�X���s���������牽�������ɏI��
	if( AnimKeySetIndex < 0 || AnimKeySetIndex >= ModelBase->AnimKeySetNum )
		return -1 ;
	AnimKeySetBase = &ModelBase->AnimKeySet[ AnimKeySetIndex ] ;

	// �L�[�Z�b�g�̃^�C�v��Ԃ�
	return AnimKeySetBase->Type ;
}

// �w��̃A�j���[�V�����L�[�Z�b�g�̃f�[�^�^�C�v���擾����
extern int NS_MV1GetAnimKeySetDataType( int MHandle, int AnimKeySetIndex )
{
	MV1_MODEL *Model ;
	MV1_MODEL_BASE *ModelBase ;
	MV1_ANIM_KEYSET_BASE *AnimKeySetBase ;

	// ����������Ă��Ȃ�������G���[
	if( MV1Man.Initialize == false ) return -1 ;

	// �A�h���X�擾
	if( MV1MDLCHK( MHandle, Model ) )
		return -1 ;
	ModelBase = Model->BaseData ;

	// �C���f�b�N�X���s���������牽�������ɏI��
	if( AnimKeySetIndex < 0 || AnimKeySetIndex >= ModelBase->AnimKeySetNum )
		return -1 ;
	AnimKeySetBase = &ModelBase->AnimKeySet[ AnimKeySetIndex ] ;

	// �L�[�Z�b�g�̃f�[�^�^�C�v��Ԃ�
	return AnimKeySetBase->DataType ;
}

// �w��̃A�j���[�V�����L�[�Z�b�g�̃L�[�̎��ԃf�[�^�^�C�v���擾����( MV1_ANIMKEY_TIME_TYPE_ONE �� )
extern int NS_MV1GetAnimKeySetTimeType( int MHandle, int AnimKeySetIndex )
{
	MV1_MODEL *Model ;
	MV1_MODEL_BASE *ModelBase ;
	MV1_ANIM_KEYSET_BASE *AnimKeySetBase ;

	// ����������Ă��Ȃ�������G���[
	if( MV1Man.Initialize == false ) return -1 ;

	// �A�h���X�擾
	if( MV1MDLCHK( MHandle, Model ) )
		return -1 ;
	ModelBase = Model->BaseData ;

	// �C���f�b�N�X���s���������牽�������ɏI��
	if( AnimKeySetIndex < 0 || AnimKeySetIndex >= ModelBase->AnimKeySetNum )
		return -1 ;
	AnimKeySetBase = &ModelBase->AnimKeySet[ AnimKeySetIndex ] ;

	// �L�[�Z�b�g�̎��ԃf�[�^�^�C�v��Ԃ�
	return AnimKeySetBase->TimeType ;
}

// �w��̃A�j���[�V�����L�[�Z�b�g�̃L�[�̐����擾����
extern int NS_MV1GetAnimKeySetDataNum( int MHandle, int AnimKeySetIndex )
{
	MV1_MODEL *Model ;
	MV1_MODEL_BASE *ModelBase ;
	MV1_ANIM_KEYSET_BASE *AnimKeySetBase ;

	// ����������Ă��Ȃ�������G���[
	if( MV1Man.Initialize == false ) return -1 ;

	// �A�h���X�擾
	if( MV1MDLCHK( MHandle, Model ) )
		return -1 ;
	ModelBase = Model->BaseData ;

	// �C���f�b�N�X���s���������牽�������ɏI��
	if( AnimKeySetIndex < 0 || AnimKeySetIndex >= ModelBase->AnimKeySetNum )
		return -1 ;
	AnimKeySetBase = &ModelBase->AnimKeySet[ AnimKeySetIndex ] ;

	// �L�[�̐���Ԃ�
	return AnimKeySetBase->Num ;
}

// �w��̃A�j���[�V�����L�[�Z�b�g�̃L�[�̎��Ԃ��擾����
extern	float NS_MV1GetAnimKeyDataTime( int MHandle, int AnimKeySetIndex, int Index )
{
	MV1_MODEL *Model ;
	MV1_MODEL_BASE *ModelBase ;
	MV1_ANIM_KEYSET_BASE *AnimKeySetBase ;

	// ����������Ă��Ȃ�������G���[
	if( MV1Man.Initialize == false ) return -1 ;

	// �A�h���X�擾
	if( MV1MDLCHK( MHandle, Model ) )
		return -1 ;
	ModelBase = Model->BaseData ;

	// �C���f�b�N�X���s���������牽�������ɏI��
	if( AnimKeySetIndex < 0 || AnimKeySetIndex >= ModelBase->AnimKeySetNum )
		return -1 ;
	AnimKeySetBase = &ModelBase->AnimKeySet[ AnimKeySetIndex ] ;

	// �L�[�̃C���f�b�N�X���s���������牽�������ɏI��
	if( Index < 0 || Index >= AnimKeySetBase->Num )
		return -1 ;

	// �L�[�̎��Ԃ�Ԃ�
	if( AnimKeySetBase->TimeType == MV1_ANIMKEY_TIME_TYPE_KEY )
	{
		return AnimKeySetBase->KeyTime[ Index ] ;
	}
	else
	{
		return AnimKeySetBase->StartTime + AnimKeySetBase->UnitTime * Index ;
	}
}

// �w��̃A�j���[�V�����L�[�Z�b�g�̎w��̎��Ԃł̃L�[�̔ԍ����擾����
extern int NS_MV1GetAnimKeyDataIndexFromTime( int MHandle, int AnimKeySetIndex, float Time )
{
	MV1_MODEL *Model ;
	MV1_MODEL_BASE *ModelBase ;
	MV1_ANIM_KEYSET_BASE *AnimKeySetBase ;
	int KeyIndex ;
	float Rate ;

	// ����������Ă��Ȃ�������G���[
	if( MV1Man.Initialize == false ) return -1 ;

	// �A�h���X�擾
	if( MV1MDLCHK( MHandle, Model ) )
		return -1 ;
	ModelBase = Model->BaseData ;

	// �C���f�b�N�X���s���������牽�������ɏI��
	if( AnimKeySetIndex < 0 || AnimKeySetIndex >= ModelBase->AnimKeySetNum )
		return -1 ;
	AnimKeySetBase = &ModelBase->AnimKeySet[ AnimKeySetIndex ] ;

	// �L�[�̃C���f�b�N�X���擾
	KeyIndex = _MV1GetAnimKeyDataIndexFromTime( AnimKeySetBase, Time, Rate ) ;

	// ��Ԃ����L�[�̔ԍ���Ԃ�
	return KeyIndex ;
}

// �w��̃A�j���[�V�����L�[�Z�b�g�̃L�[���擾����A�L�[�^�C�v�� MV1_ANIMKEY_TYPE_QUATERNION_X �� MV1_ANIMKEY_TYPE_QUATERNION_VMD �ł͖��������ꍇ�͎��s����
extern	FLOAT4			NS_MV1GetAnimKeyDataToQuaternion( int MHandle, int AnimKeySetIndex, int Index )
{
	MV1_MODEL *Model ;
	MV1_MODEL_BASE *ModelBase ;
	MV1_ANIM_KEYSET_BASE *AnimKeySetBase ;
	FLOAT4 ErrorValue = { -1.0f, -1.0f, -1.0f, -1.0f } ;

	// ����������Ă��Ȃ�������G���[
	if( MV1Man.Initialize == false ) return ErrorValue ;

	// �A�h���X�擾
	if( MV1MDLCHK( MHandle, Model ) )
		return ErrorValue ;
	ModelBase = Model->BaseData ;

	// �C���f�b�N�X���s���������牽�������ɏI��
	if( AnimKeySetIndex < 0 || AnimKeySetIndex >= ModelBase->AnimKeySetNum )
		return ErrorValue ;
	AnimKeySetBase = &ModelBase->AnimKeySet[ AnimKeySetIndex ] ;

	// �L�[�̃C���f�b�N�X���s���������牽�������ɏI��
	if( Index < 0 || Index >= AnimKeySetBase->Num )
		return ErrorValue ;

	// �L�[�̃^�C�v�� MV1_ANIMKEY_TYPE_QUATERNION_X �� MV1_ANIMKEY_TYPE_QUATERNION_VMD �ł͂Ȃ�������G���[
	if( AnimKeySetBase->Type != MV1_ANIMKEY_TYPE_QUATERNION_X &&
		AnimKeySetBase->Type != MV1_ANIMKEY_TYPE_QUATERNION_VMD )
		return ErrorValue ;

	// �L�[�̒l��Ԃ�
	return AnimKeySetBase->KeyFloat4[ Index ] ;
}

// �w��̃A�j���[�V�����L�[�Z�b�g�̃L�[���擾����A�L�[�^�C�v�� MV1_ANIMKEY_TYPE_QUATERNION �ł͖��������ꍇ�͎��s����( ���Ԏw��� )
extern	FLOAT4		NS_MV1GetAnimKeyDataToQuaternionFromTime( int MHandle, int AnimKeySetIndex, float Time )
{
	MV1_MODEL *Model ;
	MV1_MODEL_BASE *ModelBase ;
	MV1_ANIM_KEYSET_BASE *AnimKeySetBase ;
	FLOAT4 ErrorValue = { -1.0f, -1.0f, -1.0f, -1.0f } ;
	FLOAT4 Ret ;
	int KeyIndex ;
	float Rate ;

	// ����������Ă��Ȃ�������G���[
	if( MV1Man.Initialize == false ) return ErrorValue ;

	// �A�h���X�擾
	if( MV1MDLCHK( MHandle, Model ) )
		return ErrorValue ;
	ModelBase = Model->BaseData ;

	// �C���f�b�N�X���s���������牽�������ɏI��
	if( AnimKeySetIndex < 0 || AnimKeySetIndex >= ModelBase->AnimKeySetNum )
		return ErrorValue ;
	AnimKeySetBase = &ModelBase->AnimKeySet[ AnimKeySetIndex ] ;

	// �L�[�̃C���f�b�N�X���擾
	KeyIndex = _MV1GetAnimKeyDataIndexFromTime( AnimKeySetBase, Time, Rate ) ;

	// �L�[�̃^�C�v�� MV1_ANIMKEY_TYPE_QUATERNION_X �� MV1_ANIMKEY_TYPE_QUATERNION_VMD �ł͂Ȃ�������G���[
	if( AnimKeySetBase->Type != MV1_ANIMKEY_TYPE_QUATERNION_X &&
		AnimKeySetBase->Type != MV1_ANIMKEY_TYPE_QUATERNION_VMD )
		return ErrorValue ;

	// �I�[���������Ԃ��Ȃ�
	if( KeyIndex == AnimKeySetBase->Num - 1 )
	{
		return AnimKeySetBase->KeyFloat4[ KeyIndex ] ;
	}

	// ��ԏ���
	_MV1SphereLinear( &AnimKeySetBase->KeyFloat4[ KeyIndex ], &AnimKeySetBase->KeyFloat4[ KeyIndex + 1 ], Rate, &Ret ) ; 

	// ��Ԃ����L�[�̒l��Ԃ�
	return Ret ;
}

// �w��̃A�j���[�V�����L�[�Z�b�g�̃L�[���擾����A�L�[�^�C�v�� MV1_ANIMKEY_TYPE_VECTOR �ł͖��������ꍇ�͎��s����
extern	VECTOR		NS_MV1GetAnimKeyDataToVector( int MHandle, int AnimKeySetIndex, int Index )
{
	MV1_MODEL *Model ;
	MV1_MODEL_BASE *ModelBase ;
	MV1_ANIM_KEYSET_BASE *AnimKeySetBase ;
	VECTOR ErrorValue = { -1.0f, -1.0f, -1.0f } ;

	// ����������Ă��Ȃ�������G���[
	if( MV1Man.Initialize == false ) return ErrorValue ;

	// �A�h���X�擾
	if( MV1MDLCHK( MHandle, Model ) )
		return ErrorValue ;
	ModelBase = Model->BaseData ;

	// �C���f�b�N�X���s���������牽�������ɏI��
	if( AnimKeySetIndex < 0 || AnimKeySetIndex >= ModelBase->AnimKeySetNum )
		return ErrorValue ;
	AnimKeySetBase = &ModelBase->AnimKeySet[ AnimKeySetIndex ] ;

	// �L�[�̃C���f�b�N�X���s���������牽�������ɏI��
	if( Index < 0 || Index >= AnimKeySetBase->Num )
		return ErrorValue ;

	// �L�[�̃^�C�v�� MV1_ANIMKEY_TYPE_VECTOR �ł͂Ȃ�������G���[
	if( AnimKeySetBase->Type != MV1_ANIMKEY_TYPE_VECTOR )
		return ErrorValue ;

	// �L�[��Ԃ�
	return AnimKeySetBase->KeyVector[ Index ] ;
}

// �w��̃A�j���[�V�����L�[�Z�b�g�̃L�[���擾����A�L�[�^�C�v�� MV1_ANIMKEY_TYPE_VECTOR �ł͖��������ꍇ�͎��s����( ���Ԏw��� )
extern	VECTOR		NS_MV1GetAnimKeyDataToVectorFromTime( int MHandle, int AnimKeySetIndex, float Time )
{
	MV1_MODEL *Model ;
	MV1_MODEL_BASE *ModelBase ;
	MV1_ANIM_KEYSET_BASE *AnimKeySetBase ;
	VECTOR ErrorValue = { -1.0f, -1.0f, -1.0f } ;
	VECTOR Ret ;
	int KeyIndex ;
	float Rate ;

	// ����������Ă��Ȃ�������G���[
	if( MV1Man.Initialize == false ) return ErrorValue ;

	// �A�h���X�擾
	if( MV1MDLCHK( MHandle, Model ) )
		return ErrorValue ;
	ModelBase = Model->BaseData ;

	// �C���f�b�N�X���s���������牽�������ɏI��
	if( AnimKeySetIndex < 0 || AnimKeySetIndex >= ModelBase->AnimKeySetNum )
		return ErrorValue ;
	AnimKeySetBase = &ModelBase->AnimKeySet[ AnimKeySetIndex ] ;

	// �L�[�̃C���f�b�N�X���擾
	KeyIndex = _MV1GetAnimKeyDataIndexFromTime( AnimKeySetBase, Time, Rate ) ;

	// �L�[�̃^�C�v�� MV1_ANIMKEY_TYPE_VECTOR �ł͂Ȃ�������G���[
	if( AnimKeySetBase->Type != MV1_ANIMKEY_TYPE_VECTOR )
		return ErrorValue ;

	// �I�[���������Ԃ��Ȃ�
	if( KeyIndex == AnimKeySetBase->Num - 1 )
	{
		return AnimKeySetBase->KeyVector[ KeyIndex ] ;
	}

	// ��ԏ���
	Ret.x = ( 1.0f - Rate ) * AnimKeySetBase->KeyVector[ KeyIndex ].x + Rate * AnimKeySetBase->KeyVector[ KeyIndex + 1 ].x ;
	Ret.y = ( 1.0f - Rate ) * AnimKeySetBase->KeyVector[ KeyIndex ].y + Rate * AnimKeySetBase->KeyVector[ KeyIndex + 1 ].y ;
	Ret.z = ( 1.0f - Rate ) * AnimKeySetBase->KeyVector[ KeyIndex ].z + Rate * AnimKeySetBase->KeyVector[ KeyIndex + 1 ].z ;

	// ��Ԃ����L�[�̒l��Ԃ�
	return Ret ;
}

// �w��̃A�j���[�V�����L�[�Z�b�g�̃L�[���擾����A�L�[�^�C�v�� MV1_ANIMKEY_TYPE_MATRIX3X3 �� MV1_ANIMKEY_TYPE_MATRIX4X4C �ł͖��������ꍇ�͎��s����
extern	MATRIX		NS_MV1GetAnimKeyDataToMatrix( int MHandle, int AnimKeySetIndex, int Index )
{
	MV1_MODEL *Model ;
	MV1_MODEL_BASE *ModelBase ;
	MV1_ANIM_KEYSET_BASE *AnimKeySetBase ;
	static MATRIX ErrorValue = { -1.0f, -1.0f, -1.0f, -1.0f,   -1.0f, -1.0f, -1.0f, -1.0f,   -1.0f, -1.0f, -1.0f, -1.0f,   -1.0f, -1.0f, -1.0f, -1.0f  } ;
	MATRIX Matrix ;

	// ����������Ă��Ȃ�������G���[
	if( MV1Man.Initialize == false ) return ErrorValue ;

	// �A�h���X�擾
	if( MV1MDLCHK( MHandle, Model ) )
		return ErrorValue ;
	ModelBase = Model->BaseData ;

	// �C���f�b�N�X���s���������牽�������ɏI��
	if( AnimKeySetIndex < 0 || AnimKeySetIndex >= ModelBase->AnimKeySetNum )
		return ErrorValue ;
	AnimKeySetBase = &ModelBase->AnimKeySet[ AnimKeySetIndex ] ;

	// �L�[�̃C���f�b�N�X���s���������牽�������ɏI��
	if( Index < 0 || Index >= AnimKeySetBase->Num )
		return ErrorValue ;

	// �L�[�̃f�[�^�^�C�v�� MV1_ANIMKEY_DATATYPE_MATRIX4X4C �� MV1_ANIMKEY_DATATYPE_MATRIX3X3 �ł͂Ȃ�������G���[
	if( AnimKeySetBase->Type != MV1_ANIMKEY_TYPE_MATRIX4X4C &&
		AnimKeySetBase->Type != MV1_ANIMKEY_TYPE_MATRIX3X3 )
		return ErrorValue ;

	// �L�[�̒l��Ԃ�
	switch( AnimKeySetBase->Type )
	{
	case MV1_ANIMKEY_TYPE_MATRIX4X4C :
		Matrix.m[ 0 ][ 0 ] = AnimKeySetBase->KeyMatrix4x4C[ Index ].Matrix[ 0 ][ 0 ] ;
		Matrix.m[ 0 ][ 1 ] = AnimKeySetBase->KeyMatrix4x4C[ Index ].Matrix[ 0 ][ 1 ] ;
		Matrix.m[ 0 ][ 2 ] = AnimKeySetBase->KeyMatrix4x4C[ Index ].Matrix[ 0 ][ 2 ] ;
		Matrix.m[ 0 ][ 3 ] = 0.0f ;

		Matrix.m[ 1 ][ 0 ] = AnimKeySetBase->KeyMatrix4x4C[ Index ].Matrix[ 1 ][ 0 ] ;
		Matrix.m[ 1 ][ 1 ] = AnimKeySetBase->KeyMatrix4x4C[ Index ].Matrix[ 1 ][ 1 ] ;
		Matrix.m[ 1 ][ 2 ] = AnimKeySetBase->KeyMatrix4x4C[ Index ].Matrix[ 1 ][ 2 ] ;
		Matrix.m[ 1 ][ 3 ] = 0.0f ;

		Matrix.m[ 2 ][ 0 ] = AnimKeySetBase->KeyMatrix4x4C[ Index ].Matrix[ 2 ][ 0 ] ;
		Matrix.m[ 2 ][ 1 ] = AnimKeySetBase->KeyMatrix4x4C[ Index ].Matrix[ 2 ][ 1 ] ;
		Matrix.m[ 2 ][ 2 ] = AnimKeySetBase->KeyMatrix4x4C[ Index ].Matrix[ 2 ][ 2 ] ;
		Matrix.m[ 2 ][ 3 ] = 0.0f ;

		Matrix.m[ 3 ][ 0 ] = AnimKeySetBase->KeyMatrix4x4C[ Index ].Matrix[ 3 ][ 0 ] ;
		Matrix.m[ 3 ][ 1 ] = AnimKeySetBase->KeyMatrix4x4C[ Index ].Matrix[ 3 ][ 1 ] ;
		Matrix.m[ 3 ][ 2 ] = AnimKeySetBase->KeyMatrix4x4C[ Index ].Matrix[ 3 ][ 2 ] ;
		Matrix.m[ 3 ][ 3 ] = 1.0f ;
		break ;

	case MV1_ANIMKEY_TYPE_MATRIX3X3 :
		Matrix.m[ 0 ][ 0 ] = AnimKeySetBase->KeyMatrix3x3[ Index ].Matrix[ 0 ][ 0 ] ;
		Matrix.m[ 0 ][ 1 ] = AnimKeySetBase->KeyMatrix3x3[ Index ].Matrix[ 0 ][ 1 ] ;
		Matrix.m[ 0 ][ 2 ] = AnimKeySetBase->KeyMatrix3x3[ Index ].Matrix[ 0 ][ 2 ] ;
		Matrix.m[ 0 ][ 3 ] = 0.0f ;

		Matrix.m[ 1 ][ 0 ] = AnimKeySetBase->KeyMatrix3x3[ Index ].Matrix[ 1 ][ 0 ] ;
		Matrix.m[ 1 ][ 1 ] = AnimKeySetBase->KeyMatrix3x3[ Index ].Matrix[ 1 ][ 1 ] ;
		Matrix.m[ 1 ][ 2 ] = AnimKeySetBase->KeyMatrix3x3[ Index ].Matrix[ 1 ][ 2 ] ;
		Matrix.m[ 1 ][ 3 ] = 0.0f ;

		Matrix.m[ 2 ][ 0 ] = AnimKeySetBase->KeyMatrix3x3[ Index ].Matrix[ 2 ][ 0 ] ;
		Matrix.m[ 2 ][ 1 ] = AnimKeySetBase->KeyMatrix3x3[ Index ].Matrix[ 2 ][ 1 ] ;
		Matrix.m[ 2 ][ 2 ] = AnimKeySetBase->KeyMatrix3x3[ Index ].Matrix[ 2 ][ 2 ] ;
		Matrix.m[ 2 ][ 3 ] = 0.0f ;

		Matrix.m[ 3 ][ 0 ] = 0.0f ;
		Matrix.m[ 3 ][ 1 ] = 0.0f ;
		Matrix.m[ 3 ][ 2 ] = 0.0f ;
		Matrix.m[ 3 ][ 3 ] = 1.0f ;
		break ;

	default:
		return ErrorValue ;
	}

	return Matrix ;
}


// �w��̃A�j���[�V�����L�[�Z�b�g�̃L�[���擾����A�L�[�^�C�v�� MV1_ANIMKEY_TYPE_MATRIX4X4C �� MV1_ANIMKEY_TYPE_MATRIX3X3 �ł͖��������ꍇ�͎��s����( ���Ԏw��� )
extern	MATRIX		NS_MV1GetAnimKeyDataToMatrixFromTime( int MHandle, int AnimKeySetIndex, float Time )
{
	MV1_MODEL *Model ;
	MV1_MODEL_BASE *ModelBase ;
	MV1_ANIM_KEYSET_BASE *AnimKeySetBase ;
	static MATRIX ErrorValue = { -1.0f, -1.0f, -1.0f, -1.0f,   -1.0f, -1.0f, -1.0f, -1.0f,   -1.0f, -1.0f, -1.0f, -1.0f,   -1.0f, -1.0f, -1.0f, -1.0f  } ;
	MATRIX Ret ;
	int KeyIndex ;
	float Rate ;

	// ����������Ă��Ȃ�������G���[
	if( MV1Man.Initialize == false ) return ErrorValue ;

	// �A�h���X�擾
	if( MV1MDLCHK( MHandle, Model ) )
		return ErrorValue ;
	ModelBase = Model->BaseData ;

	// �C���f�b�N�X���s���������牽�������ɏI��
	if( AnimKeySetIndex < 0 || AnimKeySetIndex >= ModelBase->AnimKeySetNum )
		return ErrorValue ;
	AnimKeySetBase = &ModelBase->AnimKeySet[ AnimKeySetIndex ] ;

	// �L�[�̃C���f�b�N�X���擾
	KeyIndex = _MV1GetAnimKeyDataIndexFromTime( AnimKeySetBase, Time, Rate ) ;

	// �L�[�̃f�[�^�^�C�v�� MV1_ANIMKEY_DATATYPE_MATRIX4X4C �� MV1_ANIMKEY_DATATYPE_MATRIX3X3 �ł͂Ȃ�������G���[
	if( AnimKeySetBase->Type != MV1_ANIMKEY_TYPE_MATRIX4X4C &&
		AnimKeySetBase->Type != MV1_ANIMKEY_TYPE_MATRIX3X3 )
		return ErrorValue ;

	// �I�[���������Ԃ��Ȃ�
	if( KeyIndex == AnimKeySetBase->Num - 1 )
	{
		// �L�[�̒l��Ԃ�
		switch( AnimKeySetBase->Type )
		{
		case MV1_ANIMKEY_TYPE_MATRIX4X4C :
			Ret.m[ 0 ][ 0 ] = AnimKeySetBase->KeyMatrix4x4C[ KeyIndex ].Matrix[ 0 ][ 0 ] ;
			Ret.m[ 0 ][ 1 ] = AnimKeySetBase->KeyMatrix4x4C[ KeyIndex ].Matrix[ 0 ][ 1 ] ;
			Ret.m[ 0 ][ 2 ] = AnimKeySetBase->KeyMatrix4x4C[ KeyIndex ].Matrix[ 0 ][ 2 ] ;
			Ret.m[ 0 ][ 3 ] = 0.0f ;

			Ret.m[ 1 ][ 0 ] = AnimKeySetBase->KeyMatrix4x4C[ KeyIndex ].Matrix[ 1 ][ 0 ] ;
			Ret.m[ 1 ][ 1 ] = AnimKeySetBase->KeyMatrix4x4C[ KeyIndex ].Matrix[ 1 ][ 1 ] ;
			Ret.m[ 1 ][ 2 ] = AnimKeySetBase->KeyMatrix4x4C[ KeyIndex ].Matrix[ 1 ][ 2 ] ;
			Ret.m[ 1 ][ 3 ] = 0.0f ;

			Ret.m[ 2 ][ 0 ] = AnimKeySetBase->KeyMatrix4x4C[ KeyIndex ].Matrix[ 2 ][ 0 ] ;
			Ret.m[ 2 ][ 1 ] = AnimKeySetBase->KeyMatrix4x4C[ KeyIndex ].Matrix[ 2 ][ 1 ] ;
			Ret.m[ 2 ][ 2 ] = AnimKeySetBase->KeyMatrix4x4C[ KeyIndex ].Matrix[ 2 ][ 2 ] ;
			Ret.m[ 2 ][ 3 ] = 0.0f ;

			Ret.m[ 3 ][ 0 ] = AnimKeySetBase->KeyMatrix4x4C[ KeyIndex ].Matrix[ 3 ][ 0 ] ;
			Ret.m[ 3 ][ 1 ] = AnimKeySetBase->KeyMatrix4x4C[ KeyIndex ].Matrix[ 3 ][ 1 ] ;
			Ret.m[ 3 ][ 2 ] = AnimKeySetBase->KeyMatrix4x4C[ KeyIndex ].Matrix[ 3 ][ 2 ] ;
			Ret.m[ 3 ][ 3 ] = 1.0f ;
			break ;

		case MV1_ANIMKEY_TYPE_MATRIX3X3 :
			Ret.m[ 0 ][ 0 ] = AnimKeySetBase->KeyMatrix3x3[ KeyIndex ].Matrix[ 0 ][ 0 ] ;
			Ret.m[ 0 ][ 1 ] = AnimKeySetBase->KeyMatrix3x3[ KeyIndex ].Matrix[ 0 ][ 1 ] ;
			Ret.m[ 0 ][ 2 ] = AnimKeySetBase->KeyMatrix3x3[ KeyIndex ].Matrix[ 0 ][ 2 ] ;
			Ret.m[ 0 ][ 3 ] = 0.0f ;

			Ret.m[ 1 ][ 0 ] = AnimKeySetBase->KeyMatrix3x3[ KeyIndex ].Matrix[ 1 ][ 0 ] ;
			Ret.m[ 1 ][ 1 ] = AnimKeySetBase->KeyMatrix3x3[ KeyIndex ].Matrix[ 1 ][ 1 ] ;
			Ret.m[ 1 ][ 2 ] = AnimKeySetBase->KeyMatrix3x3[ KeyIndex ].Matrix[ 1 ][ 2 ] ;
			Ret.m[ 1 ][ 3 ] = 0.0f ;

			Ret.m[ 2 ][ 0 ] = AnimKeySetBase->KeyMatrix3x3[ KeyIndex ].Matrix[ 2 ][ 0 ] ;
			Ret.m[ 2 ][ 1 ] = AnimKeySetBase->KeyMatrix3x3[ KeyIndex ].Matrix[ 2 ][ 1 ] ;
			Ret.m[ 2 ][ 2 ] = AnimKeySetBase->KeyMatrix3x3[ KeyIndex ].Matrix[ 2 ][ 2 ] ;
			Ret.m[ 2 ][ 3 ] = 0.0f ;

			Ret.m[ 3 ][ 0 ] = 0.0f ;
			Ret.m[ 3 ][ 1 ] = 0.0f ;
			Ret.m[ 3 ][ 2 ] = 0.0f ;
			Ret.m[ 3 ][ 3 ] = 1.0f ;
			break ;

		default :
			return ErrorValue ;
		}
		return Ret ;
	}

	// ��ԏ���
	switch( AnimKeySetBase->Type )
	{
	case MV1_ANIMKEY_TYPE_MATRIX4X4C :
		Ret.m[ 0 ][ 0 ] = ( 1.0f - Rate ) * AnimKeySetBase->KeyMatrix4x4C[ KeyIndex ].Matrix[ 0 ][ 0 ] + Rate * AnimKeySetBase->KeyMatrix4x4C[ KeyIndex + 1 ].Matrix[ 0 ][ 0 ] ;
		Ret.m[ 0 ][ 1 ] = ( 1.0f - Rate ) * AnimKeySetBase->KeyMatrix4x4C[ KeyIndex ].Matrix[ 0 ][ 1 ] + Rate * AnimKeySetBase->KeyMatrix4x4C[ KeyIndex + 1 ].Matrix[ 0 ][ 1 ] ;
		Ret.m[ 0 ][ 2 ] = ( 1.0f - Rate ) * AnimKeySetBase->KeyMatrix4x4C[ KeyIndex ].Matrix[ 0 ][ 2 ] + Rate * AnimKeySetBase->KeyMatrix4x4C[ KeyIndex + 1 ].Matrix[ 0 ][ 2 ] ;
		Ret.m[ 0 ][ 3 ] = 0.0f ;

		Ret.m[ 1 ][ 0 ] = ( 1.0f - Rate ) * AnimKeySetBase->KeyMatrix4x4C[ KeyIndex ].Matrix[ 1 ][ 0 ] + Rate * AnimKeySetBase->KeyMatrix4x4C[ KeyIndex + 1 ].Matrix[ 1 ][ 0 ] ;
		Ret.m[ 1 ][ 1 ] = ( 1.0f - Rate ) * AnimKeySetBase->KeyMatrix4x4C[ KeyIndex ].Matrix[ 1 ][ 1 ] + Rate * AnimKeySetBase->KeyMatrix4x4C[ KeyIndex + 1 ].Matrix[ 1 ][ 1 ] ;
		Ret.m[ 1 ][ 2 ] = ( 1.0f - Rate ) * AnimKeySetBase->KeyMatrix4x4C[ KeyIndex ].Matrix[ 1 ][ 2 ] + Rate * AnimKeySetBase->KeyMatrix4x4C[ KeyIndex + 1 ].Matrix[ 1 ][ 2 ] ;
		Ret.m[ 1 ][ 3 ] = 0.0f ;

		Ret.m[ 2 ][ 0 ] = ( 1.0f - Rate ) * AnimKeySetBase->KeyMatrix4x4C[ KeyIndex ].Matrix[ 2 ][ 0 ] + Rate * AnimKeySetBase->KeyMatrix4x4C[ KeyIndex + 1 ].Matrix[ 2 ][ 0 ] ;
		Ret.m[ 2 ][ 1 ] = ( 1.0f - Rate ) * AnimKeySetBase->KeyMatrix4x4C[ KeyIndex ].Matrix[ 2 ][ 1 ] + Rate * AnimKeySetBase->KeyMatrix4x4C[ KeyIndex + 1 ].Matrix[ 2 ][ 1 ] ;
		Ret.m[ 2 ][ 2 ] = ( 1.0f - Rate ) * AnimKeySetBase->KeyMatrix4x4C[ KeyIndex ].Matrix[ 2 ][ 2 ] + Rate * AnimKeySetBase->KeyMatrix4x4C[ KeyIndex + 1 ].Matrix[ 2 ][ 2 ] ;
		Ret.m[ 2 ][ 3 ] = 0.0f ;

		Ret.m[ 3 ][ 0 ] = ( 1.0f - Rate ) * AnimKeySetBase->KeyMatrix4x4C[ KeyIndex ].Matrix[ 3 ][ 0 ] + Rate * AnimKeySetBase->KeyMatrix4x4C[ KeyIndex + 1 ].Matrix[ 3 ][ 0 ] ;
		Ret.m[ 3 ][ 1 ] = ( 1.0f - Rate ) * AnimKeySetBase->KeyMatrix4x4C[ KeyIndex ].Matrix[ 3 ][ 1 ] + Rate * AnimKeySetBase->KeyMatrix4x4C[ KeyIndex + 1 ].Matrix[ 3 ][ 1 ] ;
		Ret.m[ 3 ][ 2 ] = ( 1.0f - Rate ) * AnimKeySetBase->KeyMatrix4x4C[ KeyIndex ].Matrix[ 3 ][ 2 ] + Rate * AnimKeySetBase->KeyMatrix4x4C[ KeyIndex + 1 ].Matrix[ 3 ][ 2 ] ;
		Ret.m[ 3 ][ 3 ] = 1.0f ;
		break ;

	case MV1_ANIMKEY_TYPE_MATRIX3X3 :
		Ret.m[ 0 ][ 0 ] = ( 1.0f - Rate ) * AnimKeySetBase->KeyMatrix3x3[ KeyIndex ].Matrix[ 0 ][ 0 ] + Rate * AnimKeySetBase->KeyMatrix3x3[ KeyIndex + 1 ].Matrix[ 0 ][ 0 ] ;
		Ret.m[ 0 ][ 1 ] = ( 1.0f - Rate ) * AnimKeySetBase->KeyMatrix3x3[ KeyIndex ].Matrix[ 0 ][ 1 ] + Rate * AnimKeySetBase->KeyMatrix3x3[ KeyIndex + 1 ].Matrix[ 0 ][ 1 ] ;
		Ret.m[ 0 ][ 2 ] = ( 1.0f - Rate ) * AnimKeySetBase->KeyMatrix3x3[ KeyIndex ].Matrix[ 0 ][ 2 ] + Rate * AnimKeySetBase->KeyMatrix3x3[ KeyIndex + 1 ].Matrix[ 0 ][ 2 ] ;
		Ret.m[ 0 ][ 3 ] = 0.0f ;

		Ret.m[ 1 ][ 0 ] = ( 1.0f - Rate ) * AnimKeySetBase->KeyMatrix3x3[ KeyIndex ].Matrix[ 1 ][ 0 ] + Rate * AnimKeySetBase->KeyMatrix3x3[ KeyIndex + 1 ].Matrix[ 1 ][ 0 ] ;
		Ret.m[ 1 ][ 1 ] = ( 1.0f - Rate ) * AnimKeySetBase->KeyMatrix3x3[ KeyIndex ].Matrix[ 1 ][ 1 ] + Rate * AnimKeySetBase->KeyMatrix3x3[ KeyIndex + 1 ].Matrix[ 1 ][ 1 ] ;
		Ret.m[ 1 ][ 2 ] = ( 1.0f - Rate ) * AnimKeySetBase->KeyMatrix3x3[ KeyIndex ].Matrix[ 1 ][ 2 ] + Rate * AnimKeySetBase->KeyMatrix3x3[ KeyIndex + 1 ].Matrix[ 1 ][ 2 ] ;
		Ret.m[ 1 ][ 3 ] = 0.0f ;

		Ret.m[ 2 ][ 0 ] = ( 1.0f - Rate ) * AnimKeySetBase->KeyMatrix3x3[ KeyIndex ].Matrix[ 2 ][ 0 ] + Rate * AnimKeySetBase->KeyMatrix3x3[ KeyIndex + 1 ].Matrix[ 2 ][ 0 ] ;
		Ret.m[ 2 ][ 1 ] = ( 1.0f - Rate ) * AnimKeySetBase->KeyMatrix3x3[ KeyIndex ].Matrix[ 2 ][ 1 ] + Rate * AnimKeySetBase->KeyMatrix3x3[ KeyIndex + 1 ].Matrix[ 2 ][ 1 ] ;
		Ret.m[ 2 ][ 2 ] = ( 1.0f - Rate ) * AnimKeySetBase->KeyMatrix3x3[ KeyIndex ].Matrix[ 2 ][ 2 ] + Rate * AnimKeySetBase->KeyMatrix3x3[ KeyIndex + 1 ].Matrix[ 2 ][ 2 ] ;
		Ret.m[ 2 ][ 3 ] = 0.0f ;

		Ret.m[ 3 ][ 0 ] = 0.0f ;
		Ret.m[ 3 ][ 1 ] = 0.0f ;
		Ret.m[ 3 ][ 2 ] = 0.0f ;
		Ret.m[ 3 ][ 3 ] = 1.0f ;
		break ;

	default :
		return ErrorValue ;
	}

	// ��ԏ����������s���Ԃ�
	return Ret ;
}

// �w��̃A�j���[�V�����L�[�Z�b�g�̃L�[���擾����A�L�[�^�C�v�� MV1_ANIMKEY_TYPE_FLAT �ł͖��������ꍇ�͎��s����
extern	float		NS_MV1GetAnimKeyDataToFlat( int MHandle, int AnimKeySetIndex, int Index )
{
	MV1_MODEL *Model ;
	MV1_MODEL_BASE *ModelBase ;
	MV1_ANIM_KEYSET_BASE *AnimKeySetBase ;

	// ����������Ă��Ȃ�������G���[
	if( MV1Man.Initialize == false ) return -1.0f ;

	// �A�h���X�擾
	if( MV1MDLCHK( MHandle, Model ) )
		return -1.0f ;
	ModelBase = Model->BaseData ;

	// �C���f�b�N�X���s���������牽�������ɏI��
	if( AnimKeySetIndex < 0 || AnimKeySetIndex >= ModelBase->AnimKeySetNum )
		return -1.0f ;
	AnimKeySetBase = &ModelBase->AnimKeySet[ AnimKeySetIndex ] ;

	// �L�[�̃C���f�b�N�X���s���������牽�������ɏI��
	if( Index < 0 || Index >= AnimKeySetBase->Num )
		return -1.0f ;

	// �L�[�̃^�C�v�� MV1_ANIMKEY_TYPE_FLAT �ł͂Ȃ�������G���[
	if( AnimKeySetBase->Type != MV1_ANIMKEY_TYPE_FLAT )
		return -1.0f ;

	// �L�[�̒l��Ԃ�
	return AnimKeySetBase->KeyFlat[ Index ] ;
}

// �w��̃A�j���[�V�����L�[�Z�b�g�̃L�[���擾����A�L�[�^�C�v�� MV1_ANIMKEY_TYPE_FLAT �ł͖��������ꍇ�͎��s����( ���Ԏw��� )
extern	float		NS_MV1GetAnimKeyDataToFlatFromTime( int MHandle, int AnimKeySetIndex, float Time )
{
	MV1_MODEL *Model ;
	MV1_MODEL_BASE *ModelBase ;
	MV1_ANIM_KEYSET_BASE *AnimKeySetBase ;
	int KeyIndex ;
	float Rate ;

	// ����������Ă��Ȃ�������G���[
	if( MV1Man.Initialize == false ) return -1.0f ;

	// �A�h���X�擾
	if( MV1MDLCHK( MHandle, Model ) )
		return -1.0f ;
	ModelBase = Model->BaseData ;

	// �C���f�b�N�X���s���������牽�������ɏI��
	if( AnimKeySetIndex < 0 || AnimKeySetIndex >= ModelBase->AnimKeySetNum )
		return -1.0f ;
	AnimKeySetBase = &ModelBase->AnimKeySet[ AnimKeySetIndex ] ;

	// �L�[�̃C���f�b�N�X���擾
	KeyIndex = _MV1GetAnimKeyDataIndexFromTime( AnimKeySetBase, Time, Rate ) ;

	// �L�[�̃^�C�v�� MV1_ANIMKEY_TYPE_FLAT �ł͂Ȃ�������G���[
	if( AnimKeySetBase->Type != MV1_ANIMKEY_TYPE_FLAT )
		return -1.0f ;

	// �L�[�̒l��Ԃ�
	return AnimKeySetBase->KeyFlat[ KeyIndex ] ;
}

// �w��̃A�j���[�V�����L�[�Z�b�g�̃L�[���擾����A�L�[�^�C�v�� MV1_ANIMKEY_TYPE_LINEAR �ł͖��������ꍇ�͎��s����
extern	float		NS_MV1GetAnimKeyDataToLinear( int MHandle, int AnimKeySetIndex, int Index )
{
	MV1_MODEL *Model ;
	MV1_MODEL_BASE *ModelBase ;
	MV1_ANIM_KEYSET_BASE *AnimKeySetBase ;

	// ����������Ă��Ȃ�������G���[
	if( MV1Man.Initialize == false ) return -1.0f ;

	// �A�h���X�擾
	if( MV1MDLCHK( MHandle, Model ) )
		return -1.0f ;
	ModelBase = Model->BaseData ;

	// �C���f�b�N�X���s���������牽�������ɏI��
	if( AnimKeySetIndex < 0 || AnimKeySetIndex >= ModelBase->AnimKeySetNum )
		return -1.0f ;
	AnimKeySetBase = &ModelBase->AnimKeySet[ AnimKeySetIndex ] ;

	// �L�[�̃C���f�b�N�X���s���������牽�������ɏI��
	if( Index < 0 || Index >= AnimKeySetBase->Num )
		return -1.0f ;

	// �L�[�̃^�C�v�� MV1_ANIMKEY_TYPE_LINEAR �ł͂Ȃ�������G���[
	if( AnimKeySetBase->Type != MV1_ANIMKEY_TYPE_LINEAR )
		return -1.0f ;

	// �L�[�̒l��Ԃ�
	return AnimKeySetBase->KeyLinear[ Index ] ;
}

// �w��̃A�j���[�V�����L�[�Z�b�g�̃L�[���擾����A�L�[�^�C�v�� MV1_ANIMKEY_TYPE_LINEAR �ł͖��������ꍇ�͎��s����( ���Ԏw��� )
extern	float		NS_MV1GetAnimKeyDataToLinearFromTime( int MHandle, int AnimKeySetIndex, float Time )
{
	MV1_MODEL *Model ;
	MV1_MODEL_BASE *ModelBase ;
	MV1_ANIM_KEYSET_BASE *AnimKeySetBase ;
	int KeyIndex ;
	float Rate ;
	float Ret ;

	// ����������Ă��Ȃ�������G���[
	if( MV1Man.Initialize == false ) return -1.0f ;

	// �A�h���X�擾
	if( MV1MDLCHK( MHandle, Model ) )
		return -1.0f ;
	ModelBase = Model->BaseData ;

	// �C���f�b�N�X���s���������牽�������ɏI��
	if( AnimKeySetIndex < 0 || AnimKeySetIndex >= ModelBase->AnimKeySetNum )
		return -1.0f ;
	AnimKeySetBase = &ModelBase->AnimKeySet[ AnimKeySetIndex ] ;

	// �L�[�̃C���f�b�N�X���擾
	KeyIndex = _MV1GetAnimKeyDataIndexFromTime( AnimKeySetBase, Time, Rate ) ;

	// �L�[�̃^�C�v�� MV1_ANIMKEY_TYPE_LINEAR �ł͂Ȃ�������G���[
	if( AnimKeySetBase->Type != MV1_ANIMKEY_TYPE_LINEAR )
		return -1.0f ;

	// �I�[���������Ԃ��Ȃ�
	if( KeyIndex == AnimKeySetBase->Num - 1 )
	{
		return AnimKeySetBase->KeyLinear[ KeyIndex ] ;
	}

	// ��ԏ���
	Ret = ( 1.0f - Rate ) * AnimKeySetBase->KeyLinear[ KeyIndex ] + Rate * AnimKeySetBase->KeyLinear[ KeyIndex + 1 ] ;

	// ��Ԃ����L�[�̒l��Ԃ�
	return Ret ;
}


// ���f���Ŏg�p���Ă���}�e���A���̐����擾����
extern int NS_MV1GetMaterialNum( int MHandle )
{
	return MV1GetMaterialNumBase( MV1GetModelBaseHandle( MHandle ) ) ;
}

// �w��̃}�e���A���̖��O���擾����
extern	const TCHAR *NS_MV1GetMaterialName( int MHandle, int MaterialIndex )
{
#ifdef UNICODE
	return MV1GetMaterialNameBaseW( MV1GetModelBaseHandle( MHandle ), MaterialIndex ) ;
#else
	return MV1GetMaterialNameBase( MV1GetModelBaseHandle( MHandle ), MaterialIndex ) ;
#endif
}

// �w��̃}�e���A���̖��O���擾����
extern	const wchar_t *MV1GetMaterialName_WCHAR_T( int MHandle, int MaterialIndex )
{
	return MV1GetMaterialNameBaseW( MV1GetModelBaseHandle( MHandle ), MaterialIndex ) ;
}

// �w��̃}�e���A���̃^�C�v��ύX����( Type : DX_MATERIAL_TYPE_NORMAL �Ȃ� )
extern int NS_MV1SetMaterialType( int MHandle, int MaterialIndex, int Type )
{
	return MV1SetMaterialTypeBase( MV1GetModelBaseHandle( MHandle ), MaterialIndex, Type ) ;
}

// �w��̃}�e���A���̃^�C�v���擾����( �߂�l : DX_MATERIAL_TYPE_NORMAL �Ȃ� )
extern int NS_MV1GetMaterialType( int MHandle, int MaterialIndex )
{
	return MV1GetMaterialTypeBase( MV1GetModelBaseHandle( MHandle ), MaterialIndex ) ;
}

// �w��̃}�e���A���̃f�B�t���[�Y�J���[���擾����
extern	COLOR_F		NS_MV1GetMaterialDifColor( int MHandle, int MaterialIndex )
{
	COLOR_F ErrorRet = { -1.0f, -1.0f, -1.0f, -1.0f } ;
	MV1MATERIALSTART( MHandle, Model, ModelBase, Material, MaterialIndex, ErrorRet ) ;

	return Material->Diffuse ;
}

// �w��̃}�e���A���̃X�y�L�����J���[���擾����
extern	COLOR_F		NS_MV1GetMaterialSpcColor( int MHandle, int MaterialIndex )
{
	COLOR_F ErrorRet = { -1.0f, -1.0f, -1.0f, -1.0f } ;
	MV1MATERIALSTART( MHandle, Model, ModelBase, Material, MaterialIndex, ErrorRet ) ;

	return Material->Specular ;
}

// �w��̃}�e���A���̃G�~�b�V�u�J���[���擾����
extern	COLOR_F		NS_MV1GetMaterialEmiColor( int MHandle, int MaterialIndex )
{
	COLOR_F ErrorRet = { -1.0f, -1.0f, -1.0f, -1.0f } ;
	MV1MATERIALSTART( MHandle, Model, ModelBase, Material, MaterialIndex, ErrorRet ) ;

	return Material->Emissive ;
}

// �w��̃}�e���A���̃A���r�G���g�J���[���擾����
extern	COLOR_F		NS_MV1GetMaterialAmbColor( int MHandle, int MaterialIndex )
{
	COLOR_F ErrorRet = { -1.0f, -1.0f, -1.0f, -1.0f } ;
	MV1MATERIALSTART( MHandle, Model, ModelBase, Material, MaterialIndex, ErrorRet ) ;

	return Material->Ambient ;
}

// �w��̃}�e���A���̃X�y�L�����̋������擾����
extern	float		NS_MV1GetMaterialSpcPower( int MHandle, int MaterialIndex )
{
	MV1MATERIALSTART( MHandle, Model, ModelBase, Material, MaterialIndex, -1.0f ) ;

	return Material->Power ;
}




// �w��̃}�e���A���Ńf�B�t���[�Y�}�b�v�Ƃ��Ďg�p����e�N�X�`�����w�肷��
extern int NS_MV1SetMaterialDifMapTexture( int MHandle, int MaterialIndex, int TexIndex )
{
	MV1_MESH *Mesh ;
	int i ;

	MV1MATERIALSTART( MHandle, Model, ModelBase, Material, MaterialIndex, -1 ) ;

	if( TexIndex < 0 || TexIndex >= ModelBase->TextureNum )
		return -1 ;

	if( Material->DiffuseLayerNum != 0 && Material->DiffuseLayer[ 0 ].Texture == TexIndex )
	{
		return 0 ;
	}

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �f�B�t���[�Y���C���[�����������ꍇ�͂P�ɂ���
	if( Material->DiffuseLayerNum == 0 )
	{
		Material->DiffuseLayerNum = 1 ;
	}

	// �ݒ�
	Material->DiffuseLayer[ 0 ].Texture = TexIndex ;

	// ���̃}�e���A�����g�p���Ă��郁�b�V���̔������v�f�L�����̃Z�b�g�A�b�v�����t���O��|��
	Mesh = Model->Mesh ;
	for( i = 0 ; i < ModelBase->MeshNum ; i ++, Mesh ++ )
	{
		if( Mesh->Material != Material ) continue ;

		MV1MESH_RESET_SEMITRANSSETUP( Mesh )
		if( MV1CCHK( Mesh->DrawMaterialChange ) == 0 )
			MV1BitSetChange( &Mesh->DrawMaterialChange ) ;
	}

	// �I��
	return 0 ;
}

// �w��̃}�e���A���Ńf�B�t���[�Y�}�b�v�Ƃ��Ďg�p����Ă���e�N�X�`���̃C���f�b�N�X���擾����
extern int NS_MV1GetMaterialDifMapTexture( int MHandle, int MaterialIndex )
{
	MV1MATERIALSTART( MHandle, Model, ModelBase, Material, MaterialIndex, -1 ) ;

	if( Material->DiffuseLayerNum == 0 )
		return -1 ;

	return Material->DiffuseLayer[ 0 ].Texture ;
}

// �w��̃}�e���A���ŃX�y�L�����}�b�v�Ƃ��Ďg�p����e�N�X�`�����w�肷��
extern int NS_MV1SetMaterialSpcMapTexture( int MHandle, int MaterialIndex, int TexIndex )
{
	MV1MATERIALSTART( MHandle, Model, ModelBase, Material, MaterialIndex, -1 ) ;

	if( TexIndex >= ModelBase->TextureNum )
		return -1 ;

	// �e�N�X�`���C���f�b�N�X�� 0 �ȉ��������ꍇ�̓X�y�L�����}�b�v����������
	if( TexIndex < 0 )
	{
		if( Material->SpecularLayerNum == 0 )
		{
			return 0 ;
		}

		// �`��ҋ@���Ă���`�敨��`��
		DRAWSTOCKINFO

		Material->SpecularLayerNum = 0 ;
	}
	else
	{
		if( Material->SpecularLayerNum != 0 && Material->SpecularLayer[ 0 ].Texture == TexIndex )
		{
			return 0 ;
		}

		// �`��ҋ@���Ă���`�敨��`��
		DRAWSTOCKINFO

		// �X�y�L�����}�b�v�̐����O��������P�ɂ���
		if( Material->SpecularLayerNum == 0 )
		{
			Material->SpecularLayerNum = 1 ;
		}

		// �ݒ�
		Material->SpecularLayer[ 0 ].Texture = TexIndex ;
	}

	// �I��
	return 0 ;
}

// �w��̃}�e���A���ŃX�y�L�����}�b�v�Ƃ��Ďg�p����Ă���e�N�X�`���̃C���f�b�N�X���擾����
extern int NS_MV1GetMaterialSpcMapTexture( int MHandle, int MaterialIndex )
{
	MV1MATERIALSTART( MHandle, Model, ModelBase, Material, MaterialIndex, -1 ) ;

	if( Material->SpecularLayerNum == 0 )
		return -1 ;

	return Material->SpecularLayer[ 0 ].Texture ;
}

// �w��̃}�e���A���Ŗ@���}�b�v�Ƃ��Ďg�p����Ă���e�N�X�`���̃C���f�b�N�X���擾����
extern int NS_MV1GetMaterialNormalMapTexture( int MHandle, int MaterialIndex )
{
	return NS_MV1GetMaterialNormalMapTextureBase( MV1GetModelBaseHandle( MHandle ), MaterialIndex ) ;
}

// �w��̃}�e���A���̃f�B�t���[�Y�J���[��ݒ肷��
extern int NS_MV1SetMaterialDifColor( int MHandle, int MaterialIndex, COLOR_F Color )
{
	MV1_MESH *Mesh ;
	int i ;
	MV1MATERIALSTART( MHandle, Model, ModelBase, Material, MaterialIndex, -1 ) ;

	// ���܂łƓ����������牽�����Ȃ�
	if( *( ( DWORD * )&Material->Diffuse.r ) == *( ( DWORD * )&Color.r ) &&
		*( ( DWORD * )&Material->Diffuse.g ) == *( ( DWORD * )&Color.g ) &&
		*( ( DWORD * )&Material->Diffuse.b ) == *( ( DWORD * )&Color.b ) &&
		*( ( DWORD * )&Material->Diffuse.a ) == *( ( DWORD * )&Color.a ) )
		return 0 ;

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �Z�b�g
	Material->Diffuse = Color ;

	// ���̃}�e���A�����g�p���Ă��郁�b�V���̔������v�f�L�����̃Z�b�g�A�b�v�����t���O��|��
	Mesh = Model->Mesh ;
	for( i = 0 ; i < ModelBase->MeshNum ; i ++, Mesh ++ )
	{
		if( Mesh->Material != Material ) continue ;

		MV1MESH_RESET_SEMITRANSSETUP( Mesh )
		if( MV1CCHK( Mesh->DrawMaterialChange ) == 0 )
			MV1BitSetChange( &Mesh->DrawMaterialChange ) ;
	}

	// �I��
	return 0 ;
}

// �w��̃}�e���A���̃X�y�L�����J���[��ݒ肷��
extern int NS_MV1SetMaterialSpcColor( int MHandle, int MaterialIndex, COLOR_F Color )
{
	MV1_MESH *Mesh ;
	int i ;
	MV1MATERIALSTART( MHandle, Model, ModelBase, Material, MaterialIndex, -1 ) ;

	// ���܂łƓ����������牽�����Ȃ�
	if( *( ( DWORD * )&Material->Specular.r ) == *( ( DWORD * )&Color.r ) &&
		*( ( DWORD * )&Material->Specular.g ) == *( ( DWORD * )&Color.g ) &&
		*( ( DWORD * )&Material->Specular.b ) == *( ( DWORD * )&Color.b ) &&
		*( ( DWORD * )&Material->Specular.a ) == *( ( DWORD * )&Color.a ) )
		return 0 ;

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �Z�b�g
	Material->Specular = Color ;

	// ���̃}�e���A�����g�p���Ă��郁�b�V���̔������v�f�L�����̃Z�b�g�A�b�v�����t���O��|��
	Mesh = Model->Mesh ;
	for( i = 0 ; i < ModelBase->MeshNum ; i ++, Mesh ++ )
	{
		if( Mesh->Material != Material ) continue ;

		MV1MESH_RESET_SEMITRANSSETUP( Mesh )
		if( MV1CCHK( Mesh->DrawMaterialChange ) == 0 )
			MV1BitSetChange( &Mesh->DrawMaterialChange ) ;
	}

	// �I��
	return 0 ;
}

// �w��̃}�e���A���̃G�~�b�V�u�J���[��ݒ肷��
extern int NS_MV1SetMaterialEmiColor( int MHandle, int MaterialIndex, COLOR_F Color )
{
	MV1_MESH *Mesh ;
	int i ;
	MV1MATERIALSTART( MHandle, Model, ModelBase, Material, MaterialIndex, -1 ) ;

	// ���܂łƓ����������牽�����Ȃ�
	if( *( ( DWORD * )&Material->Emissive.r ) == *( ( DWORD * )&Color.r ) &&
		*( ( DWORD * )&Material->Emissive.g ) == *( ( DWORD * )&Color.g ) &&
		*( ( DWORD * )&Material->Emissive.b ) == *( ( DWORD * )&Color.b ) &&
		*( ( DWORD * )&Material->Emissive.a ) == *( ( DWORD * )&Color.a ) )
		return 0 ;

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �Z�b�g
	Material->Emissive = Color ;

	// ���̃}�e���A�����g�p���Ă��郁�b�V���̔������v�f�L�����̃Z�b�g�A�b�v�����t���O��|��
	Mesh = Model->Mesh ;
	for( i = 0 ; i < ModelBase->MeshNum ; i ++, Mesh ++ )
	{
		if( Mesh->Material != Material ) continue ;

		MV1MESH_RESET_SEMITRANSSETUP( Mesh )
		if( MV1CCHK( Mesh->DrawMaterialChange ) == 0 )
			MV1BitSetChange( &Mesh->DrawMaterialChange ) ;
	}

	// �I��
	return 0 ;
}

// �w��̃}�e���A���̃A���r�G���g�J���[��ݒ肷��
extern int NS_MV1SetMaterialAmbColor( int MHandle, int MaterialIndex, COLOR_F Color )
{
	MV1_MESH *Mesh ;
	int i ;
	MV1MATERIALSTART( MHandle, Model, ModelBase, Material, MaterialIndex, -1 ) ;

	// ���܂łƓ����������牽�����Ȃ�
	if( *( ( DWORD * )&Material->Ambient.r ) == *( ( DWORD * )&Color.r ) &&
		*( ( DWORD * )&Material->Ambient.g ) == *( ( DWORD * )&Color.g ) &&
		*( ( DWORD * )&Material->Ambient.b ) == *( ( DWORD * )&Color.b ) &&
		*( ( DWORD * )&Material->Ambient.a ) == *( ( DWORD * )&Color.a ) )
		return 0 ;

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �Z�b�g
	Material->Ambient = Color ;

	// ���̃}�e���A�����g�p���Ă��郁�b�V���̔������v�f�L�����̃Z�b�g�A�b�v�����t���O��|��
	Mesh = Model->Mesh ;
	for( i = 0 ; i < ModelBase->MeshNum ; i ++, Mesh ++ )
	{
		if( Mesh->Material != Material ) continue ;

		MV1MESH_RESET_SEMITRANSSETUP( Mesh )
		if( MV1CCHK( Mesh->DrawMaterialChange ) == 0 )
			MV1BitSetChange( &Mesh->DrawMaterialChange ) ;
	}

	// �I��
	return 0 ;
}

// �w��̃}�e���A���̃X�y�L�����̋�����ݒ肷��
extern int NS_MV1SetMaterialSpcPower( int MHandle, int MaterialIndex, float Power )
{
	MV1MATERIALSTART( MHandle, Model, ModelBase, Material, MaterialIndex, -1 ) ;

	if( Material->Power == Power )
	{
		return 0 ;
	}

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �Z�b�g
	Material->Power = Power ;

	// �I��
	return 0 ;
}

// �w��̃}�e���A���Ńg�D�[�������_�����O�̃f�B�t���[�Y�O���f�[�V�����}�b�v�Ƃ��Ďg�p����e�N�X�`����ݒ肷��
extern int NS_MV1SetMaterialDifGradTexture( int MHandle, int MaterialIndex, int TexIndex )
{
	MV1_MESH *Mesh ;
	int i ;

	MV1MATERIALSTART( MHandle, Model, ModelBase, Material, MaterialIndex, -1 ) ;

	if( TexIndex < -1 || TexIndex >= ModelBase->TextureNum )
		return -1 ;

	if( Material->DiffuseGradTexture == TexIndex )
		return 0 ;

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �ݒ�
	Material->DiffuseGradTexture = TexIndex ;

	// ���̃}�e���A�����g�p���Ă��郁�b�V���̔������v�f�L�����̃Z�b�g�A�b�v�����t���O��|��
	Mesh = Model->Mesh ;
	for( i = 0 ; i < ModelBase->MeshNum ; i ++, Mesh ++ )
	{
		if( Mesh->Material != Material ) continue ;

		MV1MESH_RESET_SEMITRANSSETUP( Mesh )
		if( MV1CCHK( Mesh->DrawMaterialChange ) == 0 )
			MV1BitSetChange( &Mesh->DrawMaterialChange ) ;
	}

	// �I��
	return 0 ;
}

// �w��̃}�e���A���Ńg�D�[�������_�����O�̃f�B�t���[�Y�O���f�[�V�����}�b�v�Ƃ��Ďg�p����e�N�X�`�����擾����
extern int NS_MV1GetMaterialDifGradTexture( int MHandle, int MaterialIndex )
{
	MV1MATERIALSTART( MHandle, Model, ModelBase, Material, MaterialIndex, -1 ) ;

	return Material->DiffuseGradTexture ;
}

// �w��̃}�e���A���Ńg�D�[�������_�����O�̃X�y�L�����O���f�[�V�����}�b�v�Ƃ��Ďg�p����e�N�X�`����ݒ肷��
extern int NS_MV1SetMaterialSpcGradTexture( int MHandle, int MaterialIndex, int TexIndex )
{
	MV1_MESH *Mesh ;
	int i ;

	MV1MATERIALSTART( MHandle, Model, ModelBase, Material, MaterialIndex, -1 ) ;

	if( TexIndex < -1 || TexIndex >= ModelBase->TextureNum )
		return -1 ;

	if( Material->SpecularGradTexture == TexIndex )
		return 0 ;

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �ݒ�
	Material->SpecularGradTexture = TexIndex ;

	// ���̃}�e���A�����g�p���Ă��郁�b�V���̔������v�f�L�����̃Z�b�g�A�b�v�����t���O��|��
	Mesh = Model->Mesh ;
	for( i = 0 ; i < ModelBase->MeshNum ; i ++, Mesh ++ )
	{
		if( Mesh->Material != Material ) continue ;

		MV1MESH_RESET_SEMITRANSSETUP( Mesh )
		if( MV1CCHK( Mesh->DrawMaterialChange ) == 0 )
			MV1BitSetChange( &Mesh->DrawMaterialChange ) ;
	}

	// �I��
	return 0 ;
}

// �w��̃}�e���A���Ńg�D�[�������_�����O�̃X�y�L�����O���f�[�V�����}�b�v�Ƃ��Ďg�p����e�N�X�`�����擾����
extern int NS_MV1GetMaterialSpcGradTexture( int MHandle, int MaterialIndex )
{
	MV1MATERIALSTART( MHandle, Model, ModelBase, Material, MaterialIndex, -1 ) ;

	return Material->SpecularGradTexture ;
}

// �w��̃}�e���A���Ńg�D�[�������_�����O�̃X�t�B�A�}�b�v�Ƃ��Ďg�p����e�N�X�`����ݒ肷��
extern int NS_MV1SetMaterialSphereMapTexture( int MHandle, int MaterialIndex, int TexIndex )
{
	MV1_MESH *Mesh ;
	int i ;

	MV1MATERIALSTART( MHandle, Model, ModelBase, Material, MaterialIndex, -1 ) ;

	if( TexIndex < -1 || TexIndex >= ModelBase->TextureNum )
		return -1 ;

	if( Material->SphereMapTexture == TexIndex )
		return 0 ;

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �ݒ�
	Material->SphereMapTexture = TexIndex ;

	// ���̃}�e���A�����g�p���Ă��郁�b�V���̔������v�f�L�����̃Z�b�g�A�b�v�����t���O��|��
	Mesh = Model->Mesh ;
	for( i = 0 ; i < ModelBase->MeshNum ; i ++, Mesh ++ )
	{
		if( Mesh->Material != Material ) continue ;

		MV1MESH_RESET_SEMITRANSSETUP( Mesh )
		if( MV1CCHK( Mesh->DrawMaterialChange ) == 0 )
			MV1BitSetChange( &Mesh->DrawMaterialChange ) ;
	}

	// �I��
	return 0 ;
}

// �w��̃}�e���A���Ńg�D�[�������_�����O�̃X�t�B�A�}�b�v�Ƃ��Ďg�p����e�N�X�`�����擾����
extern int NS_MV1GetMaterialSphereMapTexture( int MHandle, int MaterialIndex )
{
	MV1MATERIALSTART( MHandle, Model, ModelBase, Material, MaterialIndex, -1 ) ;

	return Material->SphereMapTexture ;
}

// �w��̃}�e���A���̃g�D�[�������_�����O�Ŏg�p����f�B�t���[�Y�O���f�[�V�����}�b�v�ƃf�B�t���[�Y�J���[�̍������@��ݒ肷��( DX_MATERIAL_BLENDTYPE_ADDITIVE �Ȃ� )
extern int NS_MV1SetMaterialDifGradBlendType( int MHandle, int MaterialIndex, int BlendType )
{
	MV1_MESH *Mesh ;
	int i ;

	MV1MATERIALSTART( MHandle, Model, ModelBase, Material, MaterialIndex, -1 ) ;

	if( Material->DiffuseGradBlendType == BlendType )
		return 0 ;

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �ݒ�
	Material->DiffuseGradBlendType = BlendType ;

	// ���̃}�e���A�����g�p���Ă��郁�b�V���̔������v�f�L�����̃Z�b�g�A�b�v�����t���O��|��
	Mesh = Model->Mesh ;
	for( i = 0 ; i < ModelBase->MeshNum ; i ++, Mesh ++ )
	{
		if( Mesh->Material != Material ) continue ;

		MV1MESH_RESET_SEMITRANSSETUP( Mesh )
		if( MV1CCHK( Mesh->DrawMaterialChange ) == 0 )
			MV1BitSetChange( &Mesh->DrawMaterialChange ) ;
	}

	// �I��
	return 0 ;
}

// �w��̃}�e���A���̃g�D�[�������_�����O�Ŏg�p����f�B�t���[�Y�O���f�[�V�����}�b�v�ƃf�B�t���[�Y�J���[�̍������@���擾����( DX_MATERIAL_BLENDTYPE_ADDITIVE �Ȃ� )
extern int NS_MV1GetMaterialDifGradBlendType( int MHandle, int MaterialIndex )
{
	MV1MATERIALSTART( MHandle, Model, ModelBase, Material, MaterialIndex, -1 ) ;

	return Material->DiffuseGradBlendType ;
}

// �w��̃}�e���A���̃g�D�[�������_�����O�Ŏg�p����X�y�L�����O���f�[�V�����}�b�v�ƃX�y�L�����J���[�̍������@��ݒ肷��( DX_MATERIAL_BLENDTYPE_ADDITIVE �Ȃ� )
extern int NS_MV1SetMaterialSpcGradBlendType( int MHandle, int MaterialIndex, int BlendType )
{
	MV1_MESH *Mesh ;
	int i ;

	MV1MATERIALSTART( MHandle, Model, ModelBase, Material, MaterialIndex, -1 ) ;

	if( Material->SpecularGradBlendType == BlendType )
		return 0 ;

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �ݒ�
	Material->SpecularGradBlendType = BlendType ;

	// ���̃}�e���A�����g�p���Ă��郁�b�V���̔������v�f�L�����̃Z�b�g�A�b�v�����t���O��|��
	Mesh = Model->Mesh ;
	for( i = 0 ; i < ModelBase->MeshNum ; i ++, Mesh ++ )
	{
		if( Mesh->Material != Material ) continue ;

		MV1MESH_RESET_SEMITRANSSETUP( Mesh )
		if( MV1CCHK( Mesh->DrawMaterialChange ) == 0 )
			MV1BitSetChange( &Mesh->DrawMaterialChange ) ;
	}

	// �I��
	return 0 ;
}

// �w��̃}�e���A���̃g�D�[�������_�����O�Ŏg�p����X�y�L�����O���f�[�V�����}�b�v�ƃX�y�L�����J���[�̍������@���擾����( DX_MATERIAL_BLENDTYPE_ADDITIVE �Ȃ� )
extern int NS_MV1GetMaterialSpcGradBlendType( int MHandle, int MaterialIndex )
{
	MV1MATERIALSTART( MHandle, Model, ModelBase, Material, MaterialIndex, -1 ) ;

	return Material->SpecularGradBlendType ;
}

// �w��̃}�e���A���̃g�D�[�������_�����O�Ŏg�p����X�t�B�A�}�b�v�̍������@��ݒ肷��( DX_MATERIAL_BLENDTYPE_ADDITIVE �Ȃ� )
extern int NS_MV1SetMaterialSphereMapBlendType(	int MHandle, int MaterialIndex, int BlendType )
{
	MV1_MESH *Mesh ;
	int i ;

	MV1MATERIALSTART( MHandle, Model, ModelBase, Material, MaterialIndex, -1 ) ;

	if( Material->SphereMapBlendType == BlendType )
		return 0 ;

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �ݒ�
	Material->SphereMapBlendType = BlendType ;

	// ���̃}�e���A�����g�p���Ă��郁�b�V���̔������v�f�L�����̃Z�b�g�A�b�v�����t���O��|��
	Mesh = Model->Mesh ;
	for( i = 0 ; i < ModelBase->MeshNum ; i ++, Mesh ++ )
	{
		if( Mesh->Material != Material ) continue ;

		MV1MESH_RESET_SEMITRANSSETUP( Mesh )
		if( MV1CCHK( Mesh->DrawMaterialChange ) == 0 )
			MV1BitSetChange( &Mesh->DrawMaterialChange ) ;
	}

	// �I��
	return 0 ;
}

// �w��̃}�e���A���̃g�D�[�������_�����O�Ŏg�p����X�t�B�A�}�b�v�̍������@���擾����( DX_MATERIAL_BLENDTYPE_ADDITIVE �Ȃ� )
extern int NS_MV1GetMaterialSphereMapBlendType(	int MHandle, int MaterialIndex )
{
	MV1MATERIALSTART( MHandle, Model, ModelBase, Material, MaterialIndex, -1 ) ;

	return Material->SphereMapBlendType ;
}

// �w��̃}�e���A���̃g�D�[�������_�����O�Ŏg�p����֊s���̑�����ݒ肷��( 0.0f �` 1.0f )
extern int NS_MV1SetMaterialOutLineWidth( int MHandle, int MaterialIndex, float Width )
{
	MV1_MESH *Mesh ;
	int i ;

	MV1MATERIALSTART( MHandle, Model, ModelBase, Material, MaterialIndex, -1 ) ;

	// �����������牽�����Ȃ�
	if( Material->OutLineWidth == Width )
		return 0 ;

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �ݒ�
	Material->OutLineWidth = Width ;

	// ���̃}�e���A�����g�p���Ă��郁�b�V���̔������v�f�L�����̃Z�b�g�A�b�v�����t���O��|��
	Mesh = Model->Mesh ;
	for( i = 0 ; i < ModelBase->MeshNum ; i ++, Mesh ++ )
	{
		if( Mesh->Material != Material ) continue ;

		MV1MESH_RESET_SEMITRANSSETUP( Mesh )
		if( MV1CCHK( Mesh->DrawMaterialChange ) == 0 )
			MV1BitSetChange( &Mesh->DrawMaterialChange ) ;
	}

	// �I��
	return 0 ;
}

// �w��̃}�e���A���̃g�D�[�������_�����O�Ŏg�p����֊s���̑������擾����( 0.0f �` 1.0f )
extern float NS_MV1GetMaterialOutLineWidth( int MHandle, int MaterialIndex )
{
	MV1MATERIALSTART( MHandle, Model, ModelBase, Material, MaterialIndex, -1 ) ;

	return Material->OutLineWidth ;
}

// �w��̃}�e���A���̃g�D�[�������_�����O�Ŏg�p����֊s���̃h�b�g�P�ʂ̑�����ݒ肷��
extern int NS_MV1SetMaterialOutLineDotWidth( int MHandle, int MaterialIndex, float Width )
{
	MV1_MESH *Mesh ;
	int i ;

	MV1MATERIALSTART( MHandle, Model, ModelBase, Material, MaterialIndex, -1 ) ;

	// �����������牽�����Ȃ�
	if( Material->OutLineDotWidth == Width )
		return 0 ;

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �ݒ�
	Material->OutLineDotWidth = Width ;

	// ���̃}�e���A�����g�p���Ă��郁�b�V���̔������v�f�L�����̃Z�b�g�A�b�v�����t���O��|��
	Mesh = Model->Mesh ;
	for( i = 0 ; i < ModelBase->MeshNum ; i ++, Mesh ++ )
	{
		if( Mesh->Material != Material ) continue ;

		MV1MESH_RESET_SEMITRANSSETUP( Mesh )
		if( MV1CCHK( Mesh->DrawMaterialChange ) == 0 )
			MV1BitSetChange( &Mesh->DrawMaterialChange ) ;
	}

	// �I��
	return 0 ;
}

// �w��̃}�e���A���̃g�D�[�������_�����O�Ŏg�p����֊s���̃h�b�g�P�ʂ̑������擾����
extern float NS_MV1GetMaterialOutLineDotWidth( int MHandle, int MaterialIndex )
{
	MV1MATERIALSTART( MHandle, Model, ModelBase, Material, MaterialIndex, -1 ) ;

	return Material->OutLineDotWidth ;
}

// �w��̃}�e���A���̃g�D�[�������_�����O�Ŏg�p����֊s���̐F��ݒ肷��
extern int NS_MV1SetMaterialOutLineColor( int MHandle, int MaterialIndex, COLOR_F Color )
{
	MV1_MESH *Mesh ;
	int i ;

	MV1MATERIALSTART( MHandle, Model, ModelBase, Material, MaterialIndex, -1 ) ;

	// �����������牽�����Ȃ�
	if( ( ( DWORD * )&Material->OutLineColor )[ 0 ] == ( ( DWORD * )&Color )[ 0 ] &&
		( ( DWORD * )&Material->OutLineColor )[ 1 ] == ( ( DWORD * )&Color )[ 1 ] &&
		( ( DWORD * )&Material->OutLineColor )[ 2 ] == ( ( DWORD * )&Color )[ 2 ] &&
		( ( DWORD * )&Material->OutLineColor )[ 3 ] == ( ( DWORD * )&Color )[ 3 ] ) return 0 ;

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �ݒ�
	Material->OutLineColor = Color ;

	// ���̃}�e���A�����g�p���Ă��郁�b�V���̔������v�f�L�����̃Z�b�g�A�b�v�����t���O��|��
	Mesh = Model->Mesh ;
	for( i = 0 ; i < ModelBase->MeshNum ; i ++, Mesh ++ )
	{
		if( Mesh->Material != Material ) continue ;

		MV1MESH_RESET_SEMITRANSSETUP( Mesh )
		if( MV1CCHK( Mesh->DrawMaterialChange ) == 0 )
			MV1BitSetChange( &Mesh->DrawMaterialChange ) ;
	}

	// �I��
	return 0 ;
}

// �w��̃}�e���A���̃g�D�[�������_�����O�Ŏg�p����֊s���̐F���擾����
extern COLOR_F NS_MV1GetMaterialOutLineColor( int MHandle, int MaterialIndex )
{
	COLOR_F RetColor = { 0.0f, 0.0f, 0.0f, 0.0f } ;
	MV1MATERIALSTART( MHandle, Model, ModelBase, Material, MaterialIndex, RetColor ) ;

	return Material->OutLineColor ;
}

// �w��̃}�e���A���̕`��u�����h���[�h��ݒ肷��( DX_BLENDMODE_ALPHA �� )
extern int NS_MV1SetMaterialDrawBlendMode( int MHandle, int MaterialIndex, int BlendMode )
{
	MV1_MESH *Mesh ;
	int i ;
	MV1MATERIALSTART( MHandle, Model, ModelBase, Material, MaterialIndex, -1 ) ;

	if( Material->DrawBlendMode == BlendMode )
	{
		return 0 ;
	}

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �u�����h���[�h�̃Z�b�g
	Material->DrawBlendMode = BlendMode ;

	// ���̃}�e���A�����g�p���Ă��郁�b�V���̔������v�f�L�����̃Z�b�g�A�b�v�����t���O��|��
	Mesh = Model->Mesh ;
	for( i = 0 ; i < ModelBase->MeshNum ; i ++, Mesh ++ )
	{
		if( Mesh->Material != Material ) continue ;

		MV1MESH_RESET_SEMITRANSSETUP( Mesh )
		if( MV1CCHK( Mesh->DrawMaterialChange ) == 0 )
			MV1BitSetChange( &Mesh->DrawMaterialChange ) ;
	}

	// �I��
	return 0 ;
}

// �w��̃}�e���A���̕`��u�����h�p�����[�^��ݒ肷��
extern int NS_MV1SetMaterialDrawBlendParam( int MHandle, int MaterialIndex, int BlendParam )
{
	MV1_MESH *Mesh ;
	int i ;
	MV1MATERIALSTART( MHandle, Model, ModelBase, Material, MaterialIndex, -1 ) ;

	if( Material->DrawBlendParam == BlendParam )
	{
		return 0 ;
	}

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �u�����h���[�h�̃Z�b�g
	Material->DrawBlendParam = BlendParam ;

	// ���̃}�e���A�����g�p���Ă��郁�b�V���̔������v�f�L�����̃Z�b�g�A�b�v�����t���O��|��
	Mesh = Model->Mesh ;
	for( i = 0 ; i < ModelBase->MeshNum ; i ++, Mesh ++ )
	{
		if( Mesh->Material != Material ) continue ;

		MV1MESH_RESET_SEMITRANSSETUP( Mesh )
		if( MV1CCHK( Mesh->DrawMaterialChange ) == 0 )
			MV1BitSetChange( &Mesh->DrawMaterialChange ) ;
	}

	// �I��
	return 0 ;
}

// �w��̃}�e���A���̕`��u�����h���[�h���擾����( DX_BLENDMODE_ALPHA �� )
extern int NS_MV1GetMaterialDrawBlendMode( int MHandle, int MaterialIndex )
{
	MV1MATERIALSTART( MHandle, Model, ModelBase, Material, MaterialIndex, -1 ) ;

	// �u�����h���[�h��Ԃ�
	return Material->DrawBlendMode ;
}

// �w��̃}�e���A���̕`��u�����h�p�����[�^��ݒ肷��
extern int NS_MV1GetMaterialDrawBlendParam( int MHandle, int MaterialIndex )
{
	MV1MATERIALSTART( MHandle, Model, ModelBase, Material, MaterialIndex, -1 ) ;

	// �u�����h���[�h��Ԃ�
	return Material->DrawBlendParam ;
}

// �w��̃}�e���A���̕`�掞�̃A���t�@�e�X�g�̐ݒ���s��( Enable:���e�X�g���s�����ǂ���( TRUE:�s��  FALSE:�s��Ȃ�( �f�t�H���g )  Mode:�e�X�g���[�h( DX_CMP_GREATER�� )  Param:�`��A���t�@�l�Ƃ̔�r�Ɏg�p����l( 0�`255 ) )
extern int NS_MV1SetMaterialDrawAlphaTest( int MHandle, int MaterialIndex,	int Enable, int Mode, int Param )
{
	MV1MATERIALSTART( MHandle, Model, ModelBase, Material, MaterialIndex, -1 ) ;

	if( Material->UseAlphaTest == Enable &&
		Material->AlphaFunc == Mode &&
		Material->AlphaRef == Param )
	{
		return 0 ;
	}

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �p�����[�^���Z�b�g
	Material->UseAlphaTest = Enable ;
	Material->AlphaFunc = Mode ;
	Material->AlphaRef = Param ;

	// �I��
	return 0 ;
}

// �w��̃}�e���A���̕`�掞�̃A���t�@�e�X�g���s�����ǂ������擾����( �߂�l  TRUE:�A���t�@�e�X�g���s��  FALSE:�A���t�@�e�X�g���s��Ȃ� )
extern int NS_MV1GetMaterialDrawAlphaTestEnable( int MHandle, int MaterialIndex )
{
	MV1MATERIALSTART( MHandle, Model, ModelBase, Material, MaterialIndex, -1 ) ;

	return Material->UseAlphaTest ;
}

// �w��̃}�e���A���̕`�掞�̃A���t�@�e�X�g�̃e�X�g���[�h���擾����( �߂�l  �e�X�g���[�h( DX_CMP_GREATER�� ) )
extern int NS_MV1GetMaterialDrawAlphaTestMode( int MHandle, int MaterialIndex )
{
	MV1MATERIALSTART( MHandle, Model, ModelBase, Material, MaterialIndex, -1 ) ;

	return Material->AlphaFunc ;
}

// �w��̃}�e���A���̕`�掞�̃A���t�@�e�X�g�̕`��A���t�@�n�Ƃ̔�r�Ɏg�p����l( 0�`255 )���擾����
extern int NS_MV1GetMaterialDrawAlphaTestParam( int MHandle, int MaterialIndex )
{
	MV1MATERIALSTART( MHandle, Model, ModelBase, Material, MaterialIndex, -1 ) ;

	return Material->AlphaRef ;
}

// �S�Ẵ}�e���A���̃^�C�v��ύX����( Type : DX_MATERIAL_TYPE_NORMAL �Ȃ� )
extern int NS_MV1SetMaterialTypeAll(				int MHandle, int Type )
{
	int i ;
	MV1START( MHandle, Model, ModelBase, -1 ) ;

	for( i = 0 ; i < ModelBase->MaterialNum ; i ++ )
	{
		if( NS_MV1SetMaterialType( MHandle, i, Type ) != 0 )
		{
			return -1 ;
		}
	}

	return 0 ;
}

// �S�Ẵ}�e���A���̃g�D�[�������_�����O�Ŏg�p����f�B�t���[�Y�O���f�[�V�����}�b�v�ƃf�B�t���[�Y�J���[�̍������@��ݒ肷��( DX_MATERIAL_BLENDTYPE_ADDITIVE �Ȃ� )
extern int NS_MV1SetMaterialDifGradBlendTypeAll(	int MHandle, int BlendType )
{
	int i ;
	MV1START( MHandle, Model, ModelBase, -1 ) ;

	for( i = 0 ; i < ModelBase->MaterialNum ; i ++ )
	{
		if( NS_MV1SetMaterialDifGradBlendType( MHandle, i, BlendType ) != 0 )
		{
			return -1 ;
		}
	}

	return 0 ;
}

// �S�Ẵ}�e���A���̃g�D�[�������_�����O�Ŏg�p����X�y�L�����O���f�[�V�����}�b�v�ƃX�y�L�����J���[�̍������@��ݒ肷��( DX_MATERIAL_BLENDTYPE_ADDITIVE �Ȃ� )
extern int NS_MV1SetMaterialSpcGradBlendTypeAll(	int MHandle, int BlendType )
{
	int i ;
	MV1START( MHandle, Model, ModelBase, -1 ) ;

	for( i = 0 ; i < ModelBase->MaterialNum ; i ++ )
	{
		if( NS_MV1SetMaterialSpcGradBlendType( MHandle, i, BlendType ) != 0 )
		{
			return -1 ;
		}
	}

	return 0 ;
}

// �S�Ẵ}�e���A���̃g�D�[�������_�����O�Ŏg�p����X�t�B�A�}�b�v�̍������@��ݒ肷��( DX_MATERIAL_BLENDTYPE_ADDITIVE �Ȃ� )
extern int NS_MV1SetMaterialSphereMapBlendTypeAll( int MHandle,  int BlendType )
{
	int i ;
	MV1START( MHandle, Model, ModelBase, -1 ) ;

	for( i = 0 ; i < ModelBase->MaterialNum ; i ++ )
	{
		if( NS_MV1SetMaterialSphereMapBlendType( MHandle, i, BlendType ) != 0 )
		{
			return -1 ;
		}
	}

	return 0 ;
}

// �S�Ẵ}�e���A���̃g�D�[�������_�����O�Ŏg�p����֊s���̑�����ݒ肷��
extern int NS_MV1SetMaterialOutLineWidthAll(		int MHandle, float Width )
{
	int i ;
	MV1START( MHandle, Model, ModelBase, -1 ) ;

	for( i = 0 ; i < ModelBase->MaterialNum ; i ++ )
	{
		if( NS_MV1SetMaterialOutLineWidth( MHandle, i, Width ) != 0 )
		{
			return -1 ;
		}
	}

	return 0 ;
}

// �S�Ẵ}�e���A���̃g�D�[�������_�����O�Ŏg�p����֊s���̃h�b�g�P�ʂ̑�����ݒ肷��
extern int NS_MV1SetMaterialOutLineDotWidthAll(	int MHandle,     float Width )
{
	int i ;
	MV1START( MHandle, Model, ModelBase, -1 ) ;

	for( i = 0 ; i < ModelBase->MaterialNum ; i ++ )
	{
		if( NS_MV1SetMaterialOutLineDotWidth( MHandle, i, Width ) != 0 )
		{
			return -1 ;
		}
	}

	return 0 ;
}

// �S�Ẵ}�e���A���̃g�D�[�������_�����O�Ŏg�p����֊s���̐F��ݒ肷��
extern int NS_MV1SetMaterialOutLineColorAll(		int MHandle, COLOR_F Color )
{
	int i ;
	MV1START( MHandle, Model, ModelBase, -1 ) ;

	for( i = 0 ; i < ModelBase->MaterialNum ; i ++ )
	{
		if( NS_MV1SetMaterialOutLineColor( MHandle, i, Color ) != 0 )
		{
			return -1 ;
		}
	}

	return 0 ;
}

// �S�Ẵ}�e���A���̕`��u�����h���[�h��ݒ肷��( DX_BLENDMODE_ALPHA �� )
extern int NS_MV1SetMaterialDrawBlendModeAll(		int MHandle, int BlendMode )
{
	int i ;
	MV1START( MHandle, Model, ModelBase, -1 ) ;

	for( i = 0 ; i < ModelBase->MaterialNum ; i ++ )
	{
		if( NS_MV1SetMaterialDrawBlendMode( MHandle, i, BlendMode ) != 0 )
		{
			return -1 ;
		}
	}

	return 0 ;
}

// �S�Ẵ}�e���A���̕`��u�����h�p�����[�^��ݒ肷��
extern int NS_MV1SetMaterialDrawBlendParamAll(	int MHandle,     int BlendParam )
{
	int i ;
	MV1START( MHandle, Model, ModelBase, -1 ) ;

	for( i = 0 ; i < ModelBase->MaterialNum ; i ++ )
	{
		if( NS_MV1SetMaterialDrawBlendParam( MHandle, i, BlendParam ) != 0 )
		{
			return -1 ;
		}
	}

	return 0 ;
}

// �S�Ẵ}�e���A���̕`�掞�̃A���t�@�e�X�g�̐ݒ���s��( Enable:���e�X�g���s�����ǂ���( TRUE:�s��  FALSE:�s��Ȃ�( �f�t�H���g ) ) Mode:�e�X�g���[�h( DX_CMP_GREATER�� )  Param:�`��A���t�@�l�Ƃ̔�r�Ɏg�p����l( 0�`255 ) )
extern int NS_MV1SetMaterialDrawAlphaTestAll(		int MHandle, int Enable, int Mode, int Param )
{
	int i ;
	MV1START( MHandle, Model, ModelBase, -1 ) ;

	for( i = 0 ; i < ModelBase->MaterialNum ; i ++ )
	{
		if( NS_MV1SetMaterialDrawAlphaTest( MHandle, i, Enable, Mode, Param ) != 0 )
		{
			return -1 ;
		}
	}

	return 0 ;
}











// �e�N�X�`���֌W

// �e�N�X�`���֌W

// �e�N�X�`���̐����擾
extern int NS_MV1GetTextureNum( int MHandle )
{
	return MV1GetTextureNumBase( MV1GetModelBaseHandle( MHandle ) ) ;
}

// �e�N�X�`���̖��O���擾
extern	const TCHAR *NS_MV1GetTextureName( int MHandle, int TexIndex )
{
#ifdef UNICODE
	return MV1GetTextureNameBaseW( MV1GetModelBaseHandle( MHandle ), TexIndex ) ;
#else
	return MV1GetTextureNameBase( MV1GetModelBaseHandle( MHandle ), TexIndex ) ;
#endif
}

// �e�N�X�`���̖��O���擾
extern	const wchar_t *MV1GetTextureName_WCHAR_T( int MHandle, int TexIndex )
{
	return MV1GetTextureNameBaseW( MV1GetModelBaseHandle( MHandle ), TexIndex ) ;
}

// �J���[�e�N�X�`���̃t�@�C���p�X��ύX����
extern int NS_MV1SetTextureColorFilePath( int MHandle, int TexIndex, const TCHAR *FilePath )
{
#ifdef UNICODE
	return MV1SetTextureColorFilePath_WCHAR_T(
		MHandle, TexIndex, FilePath
	) ;
#else
	int Result ;

	TCHAR_TO_WCHAR_T_STRING_ONE_BEGIN( FilePath, return -1 )

	Result = MV1SetTextureColorFilePath_WCHAR_T(
		MHandle, TexIndex, UseFilePathBuffer
	) ;

	TCHAR_TO_WCHAR_T_STRING_END( FilePath )

	return Result ;
#endif
}

// �J���[�e�N�X�`���̃t�@�C���p�X��ύX����
extern int MV1SetTextureColorFilePath_WCHAR_T( int MHandle, int TexIndex, const wchar_t *FilePath )
{
	MV1TEXTURESTART( MHandle, Model, ModelBase, Texture, TexIndex, -1 ) ;

	return _MV1TextureLoad(
				ModelBase, Texture,
				FilePath, Texture->AlphaFilePathW_,
				Texture->BumpImageFlag, Texture->BumpImageNextPixelLength,
				Texture->ReverseFlag != 0,
				Texture->Bmp32AllZeroAlphaToXRGB8Flag != 0,
				FALSE ) ;
}

// �J���[�e�N�X�`���̃t�@�C���p�X���擾
extern	const TCHAR *NS_MV1GetTextureColorFilePath( int MHandle, int TexIndex )
{
	MV1TEXTURESTART( MHandle, Model, ModelBase, Texture, TexIndex, NULL ) ;

#ifdef UNICODE
	return Texture->ColorFilePathW_ ;
#else
	return Texture->ColorFilePathA_ ;
#endif
}

// �J���[�e�N�X�`���̃t�@�C���p�X���擾
extern	const wchar_t *MV1GetTextureColorFilePath_WCHAR_T( int MHandle, int TexIndex )
{
	MV1TEXTURESTART( MHandle, Model, ModelBase, Texture, TexIndex, NULL ) ;

	return Texture->ColorFilePathW_ ;
}

// �A���t�@�e�N�X�`���̃t�@�C���p�X��ύX����
extern int NS_MV1SetTextureAlphaFilePath( int MHandle, int TexIndex, const TCHAR *FilePath )
{
#ifdef UNICODE
	return MV1SetTextureAlphaFilePath_WCHAR_T(
		MHandle, TexIndex, FilePath
	) ;
#else
	int Result ;

	TCHAR_TO_WCHAR_T_STRING_ONE_BEGIN( FilePath, return -1 )

	Result = MV1SetTextureAlphaFilePath_WCHAR_T(
		MHandle, TexIndex, UseFilePathBuffer
	) ;

	TCHAR_TO_WCHAR_T_STRING_END( FilePath )

	return Result ;
#endif
}

// �A���t�@�e�N�X�`���̃t�@�C���p�X��ύX����
extern int MV1SetTextureAlphaFilePath_WCHAR_T( int MHandle, int TexIndex, const wchar_t *FilePath )
{
	MV1TEXTURESTART( MHandle, Model, ModelBase, Texture, TexIndex, -1 ) ;

	return _MV1TextureLoad(
				ModelBase, Texture,
				Texture->ColorFilePathW_, FilePath,
				Texture->BumpImageFlag, Texture->BumpImageNextPixelLength,
				Texture->ReverseFlag != 0,
				Texture->Bmp32AllZeroAlphaToXRGB8Flag != 0,
				FALSE ) ;
}

// �A���t�@�e�N�X�`���̃t�@�C���p�X���擾
extern	const TCHAR *NS_MV1GetTextureAlphaFilePath( int MHandle, int TexIndex )
{
	MV1TEXTURESTART( MHandle, Model, ModelBase, Texture, TexIndex, NULL ) ;

#ifdef UNICODE
	return Texture->AlphaFilePathW_ ;
#else
	return Texture->AlphaFilePathA_ ;
#endif
}

// �A���t�@�e�N�X�`���̃t�@�C���p�X���擾
extern	const wchar_t *MV1GetTextureAlphaFilePath_WCHAR_T( int MHandle, int TexIndex )
{
	MV1TEXTURESTART( MHandle, Model, ModelBase, Texture, TexIndex, NULL ) ;

	return Texture->AlphaFilePathW_ ;
}

// �e�N�X�`���Ŏg�p����O���t�B�b�N�n���h����ύX����( GrHandle �� -1 �ɂ���Ɖ��� )
extern int NS_MV1SetTextureGraphHandle( int MHandle, int TexIndex, int GrHandle, int SemiTransFlag )
{
	MV1TEXTURESTART( MHandle, Model, ModelBase, Texture, TexIndex, -1 ) ;

	// GrHandle �� -1 ���ǂ����ŏ����𕪊�
	if( GrHandle == -1 )
	{
		if( Texture->UseUserGraphHandle == FALSE && Texture->UseGraphHandle == 0 )
		{
			return 0 ;
		}

		// �`��ҋ@���Ă���`�敨��`��
		DRAWSTOCKINFO

		Texture->UseUserGraphHandle = FALSE ;
		Texture->UserGraphHandle = 0 ;
	}
	else
	{
		int TexWidth, TexHeight ;

		NS_GetGraphSize( GrHandle, &TexWidth, &TexHeight ) ;
		if( Texture->UseGraphHandle == TRUE &&
			Texture->UserGraphHandle == GrHandle &&
			Texture->UserGraphHandleSemiTransFlag == SemiTransFlag &&
			TexWidth == Texture->UserGraphWidth &&
			TexHeight == Texture->UserGraphHeight )
		{
			return 0 ;
		}

		// �`��ҋ@���Ă���`�敨��`��
		DRAWSTOCKINFO

		Texture->UseUserGraphHandle = TRUE ;
		Texture->UserGraphHandle = GrHandle ;
		Texture->UserGraphHandleSemiTransFlag = SemiTransFlag ;
		Texture->UserGraphWidth = TexWidth ;
		Texture->UserGraphHeight = TexHeight ;
	}

	// �I��
	return 0 ;
}

// �e�N�X�`���̃O���t�B�b�N�n���h�����擾����
extern int NS_MV1GetTextureGraphHandle( int MHandle, int TexIndex )
{
	MV1TEXTURESTART( MHandle, Model, ModelBase, Texture, TexIndex, -1 ) ;

	if( Texture->UseUserGraphHandle )
	{
		return Texture->UserGraphHandle ;
	}
	else
	if( Texture->UseGraphHandle )
	{
		return Texture->GraphHandle ;
	}
	else
	{
		return MV1GetTextureGraphHandleBase( MV1GetModelBaseHandle( MHandle ), TexIndex ) ;
	}
}

// �e�N�X�`���̃A�h���X���[�h��ݒ肷��
extern int NS_MV1SetTextureAddressMode( int MHandle, int TexIndex, int AddrUMode, int AddrVMode )
{
	MV1TEXTURESTART( MHandle, Model, ModelBase, Texture, TexIndex, -1 ) ;

	if( Texture->AddressModeU == AddrUMode &&
		Texture->AddressModeV == AddrVMode )
	{
		return 0 ;
	}

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	Texture->AddressModeU = AddrUMode ;
	Texture->AddressModeV = AddrVMode ;

	// �I��
	return 0 ;
}

// �e�N�X�`���̂t�l�̃A�h���X���[�h���擾����( �߂�l:DX_TEXADDRESS_WRAP �� )
extern int NS_MV1GetTextureAddressModeU( int MHandle, int TexIndex )
{
	MV1TEXTURESTART( MHandle, Model, ModelBase, Texture, TexIndex, -1 ) ;

	return Texture->AddressModeU ;
}

// �e�N�X�`���̂u�l�̃A�h���X���[�h���擾����( �߂�l:DX_TEXADDRESS_WRAP �� )
extern int NS_MV1GetTextureAddressModeV( int MHandle, int TexIndex )
{
	MV1TEXTURESTART( MHandle, Model, ModelBase, Texture, TexIndex, -1 ) ;

	return Texture->AddressModeV ;
}

// �e�N�X�`���̕����擾����
extern int NS_MV1GetTextureWidth( int MHandle, int TexIndex )
{
	MV1TEXTURESTART( MHandle, Model, ModelBase, Texture, TexIndex, -1 ) ;

	if( Texture->UseUserGraphHandle )
	{
		return Texture->UserGraphWidth ;
	}
	else
	if( Texture->UseGraphHandle )
	{
		return Texture->Width ;
	}
	else
	{
		return MV1GetTextureWidthBase( MV1GetModelBaseHandle( MHandle ), TexIndex ) ;
	}
}

// �e�N�X�`���̍������擾����
extern int NS_MV1GetTextureHeight( int MHandle, int TexIndex )
{
	MV1TEXTURESTART( MHandle, Model, ModelBase, Texture, TexIndex, -1 ) ;

	if( Texture->UseUserGraphHandle )
	{
		return Texture->UserGraphHeight ;
	}
	else
	if( Texture->UseGraphHandle )
	{
		return Texture->Height ;
	}
	else
	{
		return MV1GetTextureHeightBase( MV1GetModelBaseHandle( MHandle ), TexIndex ) ;
	}
}

// �e�N�X�`���ɔ������v�f�����邩�ǂ������擾����( �߂�l  TRUE:����  FALSE:�Ȃ� )
extern int NS_MV1GetTextureSemiTransState( int MHandle, int TexIndex )
{
	MV1TEXTURESTART( MHandle, Model, ModelBase, Texture, TexIndex, -1 ) ;

	if( Texture->UseUserGraphHandle )
	{
		return Texture->UserGraphHandleSemiTransFlag ;
	}
	else
	if( Texture->UseGraphHandle )
	{
		return Texture->SemiTransFlag ;
	}
	else
	{
		return MV1GetTextureSemiTransStateBase( MV1GetModelBaseHandle( MHandle ), TexIndex ) ;
	}
}

// �e�N�X�`���Ŏg�p���Ă���摜���o���v�}�b�v���ǂ�����ݒ肷��
extern int NS_MV1SetTextureBumpImageFlag( int MHandle, int TexIndex, int Flag )
{
	MV1TEXTURESTART( MHandle, Model, ModelBase, Texture, TexIndex, -1 ) ;

	return _MV1TextureLoad(
				ModelBase, Texture,
				Texture->ColorFilePathW_, Texture->AlphaFilePathW_,
				Flag, Texture->BumpImageNextPixelLength,
				Texture->ReverseFlag != 0,
				Texture->Bmp32AllZeroAlphaToXRGB8Flag != 0, FALSE ) ;
}

// �e�N�X�`�����o���v�}�b�v���ǂ������擾����( �߂�l  TRUE:�o���v�}�b�v  FALSE:�Ⴄ )
extern int NS_MV1GetTextureBumpImageFlag( int MHandle, int TexIndex )
{
	MV1TEXTURESTART( MHandle, Model, ModelBase, Texture, TexIndex, -1 ) ;

	return Texture->BumpImageFlag ;
}

// �o���v�}�b�v�摜�̏ꍇ�ׂ̗̃s�N�Z���Ƃ̋�����ݒ肷��
extern int NS_MV1SetTextureBumpImageNextPixelLength( int MHandle, int TexIndex, float Length )
{
	MV1TEXTURESTART( MHandle, Model, ModelBase, Texture, TexIndex, -1 ) ;

	if( Texture->BumpImageNextPixelLength == Length )
	{
		return 0 ;
	}

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	Texture->BumpImageNextPixelLength = Length ;

	return _MV1TextureLoad(
				ModelBase, Texture,
				Texture->ColorFilePathW_, Texture->AlphaFilePathW_,
				Texture->BumpImageFlag, Length,
				Texture->ReverseFlag != 0,
				Texture->Bmp32AllZeroAlphaToXRGB8Flag != 0, FALSE ) ;
}

// �o���v�}�b�v�摜�̏ꍇ�ׂ̗̃s�N�Z���Ƃ̋������擾����
extern float NS_MV1GetTextureBumpImageNextPixelLength( int MHandle, int TexIndex )
{
	MV1TEXTURESTART( MHandle, Model, ModelBase, Texture, TexIndex, -1.0f ) ;

	return Texture->BumpImageNextPixelLength ;
}

// �e�N�X�`���̃t�B���^�����O���[�h��ݒ肷��
extern int NS_MV1SetTextureSampleFilterMode( int MHandle, int TexIndex, int FilterMode )
{
	MV1TEXTURESTART( MHandle, Model, ModelBase, Texture, TexIndex, -1 ) ;

	if( Texture->FilterMode == FilterMode )
	{
		return 0 ;
	}

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	Texture->FilterMode = FilterMode ;

	// �I��
	return 0 ;
}

// �e�N�X�`���̃t�B���^�����O���[�h���擾����( �߂�l  MV1_TEXTURE_FILTER_MODE_POINT�� )
extern int NS_MV1GetTextureSampleFilterMode( int MHandle, int TexIndex )
{
	MV1TEXTURESTART( MHandle, Model, ModelBase, Texture, TexIndex, -1 ) ;

	return Texture->FilterMode ;
}

// �R�c���f���ɓ\��t����̂Ɍ������摜�̓ǂݍ��ݕ����ŉ摜��ǂݍ���( �߂�l  -1:�G���[  0�ȏ�:�O���t�B�b�N�n���h�� )
extern int NS_MV1LoadTexture( const TCHAR *FilePath )
{
#ifdef UNICODE
	return MV1LoadTexture_WCHAR_T(
		FilePath
	) ;
#else
	int Result ;

	TCHAR_TO_WCHAR_T_STRING_ONE_BEGIN( FilePath, return -1 )

	Result = MV1LoadTexture_WCHAR_T(
		UseFilePathBuffer
	) ;

	TCHAR_TO_WCHAR_T_STRING_END( FilePath )

	return Result ;
#endif
}

// �R�c���f���ɓ\��t����̂Ɍ������摜�̓ǂݍ��ݕ����ŉ摜��ǂݍ���( �߂�l  -1:�G���[  0�ȏ�:�O���t�B�b�N�n���h�� )
extern int MV1LoadTexture_WCHAR_T( const wchar_t *FilePath )
{
	int NewGraphHandle ;
	void *ColorImage, *AlphaImage ;
	int ColorImageSize, AlphaImageSize ;
	int SemiTransFlag, DefaultTextureFlag ;
	const wchar_t *ColorFilePath ;

	ColorFilePath = FilePath ;
	if( __MV1LoadTexture(
			&ColorImage, &ColorImageSize,
			&AlphaImage, &AlphaImageSize,
			&NewGraphHandle,
			&SemiTransFlag,
			&DefaultTextureFlag,
#ifndef UNICODE
			NULL, NULL,
#endif
			NULL, NULL,
			ColorFilePath, NULL, NULL,
			FALSE, 0.1f,
			FALSE,
			FALSE,
			NULL,
			false,
			FALSE,
			FALSE ) == -1 )
		return -1 ;

	if( ColorImage )
	{
		DXFREE( ColorImage ) ;
		ColorImage = NULL ;
	}

	if( AlphaImage )
	{
		DXFREE( AlphaImage ) ;
		AlphaImage = NULL ;
	}

	// �n���h����Ԃ�
	return NewGraphHandle ;
}















// �t���[���̐����擾����
extern int NS_MV1GetFrameNum( int MHandle )
{
	MV1_MODEL *Model ;
	MV1_MODEL_BASE *ModelBase ;

	// ����������Ă��Ȃ�������G���[
	if( MV1Man.Initialize == false ) return -1 ;

	// �A�h���X�擾
	if( MV1MDLCHK( MHandle, Model ) )
		return -1 ;
	ModelBase = Model->BaseData ;

	// �t���[���̐���Ԃ�
	return ModelBase->FrameNum ;
}

// �t���[���̖��O���烂�f�����̃t���[���̃C���f�b�N�X���擾����( ���������ꍇ�͖߂�l��-1 )
extern int NS_MV1SearchFrame( int MHandle, const TCHAR *FrameName )
{
#ifdef UNICODE
	return MV1SearchFrame_WCHAR_T(
		MHandle, FrameName
	) ;
#else
	int Result ;

	TCHAR_TO_WCHAR_T_STRING_ONE_BEGIN( FrameName, return -1 )

	Result = MV1SearchFrame_WCHAR_T(
		MHandle, UseFrameNameBuffer
	) ;

	TCHAR_TO_WCHAR_T_STRING_END( FrameName )

	return Result ;
#endif
}

// �t���[���̖��O���烂�f�����̃t���[���̃C���f�b�N�X���擾����( ���������ꍇ�͖߂�l��-1 )
extern int MV1SearchFrame_WCHAR_T( int MHandle, const wchar_t *FrameName )
{
	MV1_MODEL *Model ;
	MV1_MODEL_BASE *ModelBase ;
	MV1_FRAME_BASE *MBFrame ;
	int i ;

	// ����������Ă��Ȃ�������G���[
	if( MV1Man.Initialize == false ) return -1 ;

	// �A�h���X�擾
	if( MV1MDLCHK( MHandle, Model ) )
		return -1 ;
	ModelBase = Model->BaseData ;

	// �����̃t���[����T��
	MBFrame = ModelBase->Frame ;
	for( i = 0 ; i < ModelBase->FrameNum && _WCSCMP( MBFrame->NameW, FrameName ) != 0 ; i ++, MBFrame ++ ){}

	// �t���[���̃C���f�b�N�X��Ԃ�
	return i == ModelBase->FrameNum ? -2 : i ;
}

// �t���[���̖��O����w��̃t���[���̎q�t���[���̃t���[���C���f�b�N�X���擾����( ���O�w��� )( FrameIndex �� -1 �ɂ���Ɛe�������Ȃ��t���[���� ChildIndex �Ŏw�肷�� )( ���������ꍇ�͖߂�l��-1 )
extern int NS_MV1SearchFrameChild( int MHandle, int FrameIndex, const TCHAR *ChildName )
{
#ifdef UNICODE
	return MV1SearchFrameChild_WCHAR_T(
		MHandle, FrameIndex, ChildName
	) ;
#else
	int Result ;

	TCHAR_TO_WCHAR_T_STRING_ONE_BEGIN( ChildName, return -1 )

	Result = MV1SearchFrameChild_WCHAR_T(
		MHandle, FrameIndex, UseChildNameBuffer
	) ;

	TCHAR_TO_WCHAR_T_STRING_END( ChildName )

	return Result ;
#endif
}

// �t���[���̖��O����w��̃t���[���̎q�t���[���̃t���[���C���f�b�N�X���擾����( ���O�w��� )( FrameIndex �� -1 �ɂ���Ɛe�������Ȃ��t���[���� ChildIndex �Ŏw�肷�� )( ���������ꍇ�͖߂�l��-1 )
extern int MV1SearchFrameChild_WCHAR_T( int MHandle, int FrameIndex, const wchar_t *ChildName )
{
	MV1_MODEL *Model ;
	MV1_MODEL_BASE *ModelBase ;
	MV1_FRAME *Frame ;
	int i ;

	// ����������Ă��Ȃ�������G���[
	if( MV1Man.Initialize == false ) return -1 ;

	// ���O�̃A�h���X���s����������G���[
	if( ChildName == NULL ) return -1 ;

	// �A�h���X�擾
	if( MV1MDLCHK( MHandle, Model ) )
		return -1 ;
	ModelBase = Model->BaseData ;

	// �C���f�b�N�X���s���������� -1 ��Ԃ�
	if( FrameIndex < -1 || FrameIndex >= ModelBase->FrameNum )
		return -1 ;

	// �t���[���C���f�b�N�X�� -1 ���ǂ����ŏ����𕪊�
	if( FrameIndex == -1 )
	{
		// �g�b�v�t���[���̒�����w��̖��O�̃t���[����T��
		for( i = 0 ; i < Model->TopFrameNum && _WCSCMP( Model->TopFrameList[ i ]->BaseData->NameW, ChildName ) != 0 ; i ++ ){}
		return i == Model->TopFrameNum ? -2 : ( int )( Model->TopFrameList[ i ] - Model->Frame ) ;
	}
	else
	{
		// �q�C���f�b�N�X���s���������� -1 ��Ԃ�
		Frame = &Model->Frame[ FrameIndex ] ;

		// �q�t���[���̒�����w��̖��O�̃t���[����T��
		for( i = 0 ; i < Frame->BaseData->TotalChildNum && _WCSCMP( Frame->Child[ i ].BaseData->NameW, ChildName ) != 0 ; i ++ ){}
		return i == Frame->BaseData->TotalChildNum ? -2 : ( int )( &Frame->Child[ i ] - Model->Frame ) ;
	}
}

// �w��̃t���[���̖��O���擾����( �G���[�̏ꍇ�͖߂�l�� NULL )
extern const TCHAR *NS_MV1GetFrameName( int MHandle, int FrameIndex )
{
	MV1FRAMESTART( MHandle, Model, ModelBase, Frame, FrameIndex, NULL ) ;

	// �t���[���̖��O��Ԃ�
#ifdef UNICODE
	return Frame->BaseData->NameW ;
#else
	return Frame->BaseData->NameA ;
#endif
}

// �w��̃t���[���̖��O���擾����( �G���[�̏ꍇ�͖߂�l�� NULL )
extern const wchar_t *MV1GetFrameName_WCHAR_T( int MHandle, int FrameIndex )
{
	MV1FRAMESTART( MHandle, Model, ModelBase, Frame, FrameIndex, NULL ) ;

	// �t���[���̖��O��Ԃ�
	return Frame->BaseData->NameW ;
}

// �w��̃t���[���̖��O���擾����( �߂�l   -1:�G���[  -1�ȊO:������̃T�C�Y )
extern int NS_MV1GetFrameName2( int MHandle, int FrameIndex, TCHAR *StrBuffer )
{
#ifdef UNICODE
	return MV1GetFrameName2_WCHAR_T( MHandle, FrameIndex, StrBuffer ) ;
#else
	int Length = 0 ;
	MV1FRAMESTART( MHandle, Model, ModelBase, Frame, FrameIndex, -1 ) ;

	if( StrBuffer != NULL )
	{
		_STRCPY( StrBuffer, Frame->BaseData->NameA ) ;
	}

	Length = ( int )_STRLEN( Frame->BaseData->NameA ) ;

	return Length ;
#endif
}

// �w��̃t���[���̖��O���擾����( �߂�l   -1:�G���[  -1�ȊO:������̃T�C�Y )
extern int MV1GetFrameName2_WCHAR_T( int MHandle, int FrameIndex, wchar_t *StrBuffer )
{
	int Length = 0 ;
	MV1FRAMESTART( MHandle, Model, ModelBase, Frame, FrameIndex, -1 ) ;

	if( StrBuffer != NULL )
	{
		_WCSCPY( StrBuffer, Frame->BaseData->NameW ) ;
	}

	Length = ( int )_WCSLEN( Frame->BaseData->NameW ) ;

	return Length ;
}

// �w��̃t���[���̎q�t���[���̐����擾����
extern int NS_MV1GetFrameChildNum( int MHandle, int FrameIndex )
{
	MV1_MODEL *Model ;
	MV1_MODEL_BASE *ModelBase ;

	// ����������Ă��Ȃ�������G���[
	if( MV1Man.Initialize == false ) return -1 ;

	// �A�h���X�擾
	if( MV1MDLCHK( MHandle, Model ) )
		return -1 ;
	ModelBase = Model->BaseData ;

	// �C���f�b�N�X���s���������� -1 ��Ԃ�
	if( FrameIndex < -1 || FrameIndex >= ModelBase->FrameNum )
		return -1 ;

	// �q�t���[���̐���Ԃ�
	return FrameIndex == -1 ? Model->TopFrameNum : Model->Frame[ FrameIndex ].ChildNum ;
}

// �w��̃t���[���̐e�t���[���̃C���f�b�N�X�𓾂�( �e�����Ȃ��ꍇ�� -2 ���Ԃ� )
extern int NS_MV1GetFrameParent( int MHandle, int FrameIndex )
{
	MV1FRAMESTART( MHandle, Model, ModelBase, Frame, FrameIndex, -1 ) ;

	// �e�t���[���̃t���[���C���f�b�N�X��Ԃ�
	return Frame->Parent == NULL ? -2 : ( int )( Frame->Parent - Model->Frame ) ;
}

// �w��̃t���[���̎q�t���[���̃t���[���C���f�b�N�X���擾����( �ԍ��w��� )( FrameIndex �� -1 �ɂ���Ɛe�������Ȃ��t���[���� ChildIndex �Ŏw�肷�� )( �G���[�̏ꍇ�͖߂�l��-1 )
extern int NS_MV1GetFrameChild( int MHandle, int FrameIndex, int ChildIndex )
{
	MV1_MODEL *Model ;
	MV1_MODEL_BASE *ModelBase ;
	MV1_FRAME *Frame ;

	// ����������Ă��Ȃ�������G���[
	if( MV1Man.Initialize == false ) return -1 ;

	// �A�h���X�擾
	if( MV1MDLCHK( MHandle, Model ) )
		return -1 ;
	ModelBase = Model->BaseData ;

	// �C���f�b�N�X���s���������� -1 ��Ԃ�
	if( FrameIndex < -1 || FrameIndex >= ModelBase->FrameNum )
		return -1 ;

	// �t���[���C���f�b�N�X�� -1 ���ǂ����ŏ����𕪊�
	if( FrameIndex == -1 )
	{
		// �q�C���f�b�N�X���s���������� -1 ��Ԃ�
		if( ChildIndex < 0 || ChildIndex >= Model->TopFrameNum )
			return -1 ;

		// �g�b�v�t���[���̃t���[���C���f�b�N�X��Ԃ�
		return ( int )( Model->TopFrameList[ ChildIndex ] - Model->Frame ) ;
	}
	else
	{
		// �q�C���f�b�N�X���s���������� -1 ��Ԃ�
		Frame = &Model->Frame[ FrameIndex ] ;
		if( ChildIndex < 0 || ChildIndex >= Frame->ChildNum )
			return -1 ;

		// �q�t���[���̃t���[���C���f�b�N�X��Ԃ�
		return ( int )( Frame->ChildList[ ChildIndex ] - Model->Frame ) ;
	}
}

// �w��̃t���[���̍��W���擾����
extern VECTOR NS_MV1GetFramePosition( int MHandle, int FrameIndex )
{
	VECTOR Pos ;
	VECTOR ErrorRet = { 0.0f, 0.0f, 0.0f } ;
	MV1FRAMESTART( MHandle, Model, ModelBase, Frame, FrameIndex, ErrorRet ) ;

	// �s��̃Z�b�g�A�b�v
	MV1SETUPMATRIX( Model ) ;

	// �t���[���̍��W��Ԃ�
	if( GSYS.DrawSetting.Large3DPositionSupport )
	{
		Pos.x = ( float )Frame->LocalWorldMatrix.md.m[ 0 ][ 3 ] ;
		Pos.y = ( float )Frame->LocalWorldMatrix.md.m[ 1 ][ 3 ] ;
		Pos.z = ( float )Frame->LocalWorldMatrix.md.m[ 2 ][ 3 ] ;
	}
	else
	{
		Pos.x = Frame->LocalWorldMatrix.mf.m[ 0 ][ 3 ] ;
		Pos.y = Frame->LocalWorldMatrix.mf.m[ 1 ][ 3 ] ;
		Pos.z = Frame->LocalWorldMatrix.mf.m[ 2 ][ 3 ] ;
	}

	return Pos ;
}

// �w��̃t���[���̍��W���擾����
extern VECTOR_D NS_MV1GetFramePositionD( int MHandle, int FrameIndex )
{
	VECTOR_D Pos ;
	VECTOR_D ErrorRet = { 0.0f, 0.0f, 0.0f } ;
	MV1FRAMESTART( MHandle, Model, ModelBase, Frame, FrameIndex, ErrorRet ) ;

	// �s��̃Z�b�g�A�b�v
	MV1SETUPMATRIX( Model ) ;

	// �t���[���̍��W��Ԃ�
	if( GSYS.DrawSetting.Large3DPositionSupport )
	{
		Pos.x = Frame->LocalWorldMatrix.md.m[ 0 ][ 3 ] ;
		Pos.y = Frame->LocalWorldMatrix.md.m[ 1 ][ 3 ] ;
		Pos.z = Frame->LocalWorldMatrix.md.m[ 2 ][ 3 ] ;
	}
	else
	{
		Pos.x = Frame->LocalWorldMatrix.mf.m[ 0 ][ 3 ] ;
		Pos.y = Frame->LocalWorldMatrix.mf.m[ 1 ][ 3 ] ;
		Pos.z = Frame->LocalWorldMatrix.mf.m[ 2 ][ 3 ] ;
	}

	return Pos ;
}

// �w��̃t���[���̏�����Ԃł̍��W�ϊ��s����擾����
extern MATRIX NS_MV1GetFrameBaseLocalMatrix( int MHandle, int FrameIndex )
{
	MATRIX ResultMatrix ;

	MV1FRAMESTART( MHandle, Model, ModelBase, Frame, FrameIndex, MGetIdent() ) ;

	// �s��̃Z�b�g�A�b�v
	MV1SETUPMATRIX( Model ) ;

	// �s���Ԃ�
	ConvertMatrix4x4cFToMatrixF( &ResultMatrix, &Frame->BaseData->LocalTransformMatrix ) ;
	return ResultMatrix ;
}

// �w��̃t���[���̏�����Ԃł̍��W�ϊ��s����擾����
extern MATRIX_D NS_MV1GetFrameBaseLocalMatrixD( int MHandle, int FrameIndex )
{
	MATRIX_D ResultMatrix ;

	MV1FRAMESTART( MHandle, Model, ModelBase, Frame, FrameIndex, MGetIdentD() ) ;

	// �s��̃Z�b�g�A�b�v
	MV1SETUPMATRIX( Model ) ;

	// �s���Ԃ�
	ConvertMatrix4x4cFToMatrixD( &ResultMatrix, &Frame->BaseData->LocalTransformMatrix ) ;
	return ResultMatrix ;
}

// �w��̃t���[���̃��[�J�����W�ϊ��s����擾����
#ifdef __BCC
static DummyFunc_MV1GetFrameLocalMatrixF(){}
#endif
extern MATRIX NS_MV1GetFrameLocalMatrix( int MHandle, int FrameIndex )
{
	MATRIX ResultMatrix ;
	MV1_FRAME_BASE *FrameBase ;

// BorlandC++ �̍œK���o�O�}���p
#ifdef __BCC
	DummyFunc_MV1GetFrameLocalMatrixF();
#endif

	MV1FRAMESTART( MHandle, Model, ModelBase, Frame, FrameIndex, MGetIdent() ) ;
	FrameBase = Frame->BaseData ;

	// �s��̃Z�b�g�A�b�v���s��
	MV1SETUPMATRIX( Model ) ;

	// ���[�U�[�w��̍s�񂪂���ꍇ�͂����Ԃ�
	if( Frame->ValidUserLocalTransformMatrix )
	{
		ConvertMatrix4x4cToMatrixF( &ResultMatrix, &Frame->UserLocalTransformMatrix ) ;
	}
	else
	// �A�j���[�V����������ꍇ�Ɩ����ꍇ�ŏ����𕪊�
	if( Model->AnimSetNum == 0 )
	{
		// �A�j���[�V�������Ȃ��ꍇ�̓f�t�H���g�s���Ԃ�
		ConvertMatrix4x4cFToMatrixF( &ResultMatrix, &FrameBase->LocalTransformMatrix ) ;
	}
	else
	{
		// �A�j���[�V����������ꍇ
		MATRIX_4X4CT_F BlendMat ;
		float BlendRate ;
		VECTOR Translate, Scale, Rotate ;
		FLOAT4 Quaternion ;
		int BlendFlag, mcon, i ;
		MV1_MODEL_ANIM *MAnim, *MAnim2, *MAnim3 = NULL ;

		// �p�����[�^���x���̃u�����h���s���邩�𒲂ׂ�
		MAnim = Model->Anim + Model->AnimSetMaxNum * Frame->BaseData->Index ;
		MAnim2 = MAnim ;
		BlendFlag = 0 ;
		mcon = 0 ;
		for( i = 0 ; i < Model->AnimSetMaxNum ; i ++, MAnim2 ++ )
		{
			if( MAnim2->Use == false || MAnim2->BlendRate == 0.0f )
				continue ;
			mcon ++ ;
			MAnim3 = MAnim2 ;

			BlendFlag |= MAnim2->Anim->ValidFlag ;
		}

		// �A�j���[�V�������Đ�����Ă��Ȃ��ꍇ�̓f�t�H���g�̍s���K������
		if( mcon == 0 )
		{
			if( FrameBase->LocalTransformMatrixType == 0 )
			{
				CreateIdentityMatrix( &ResultMatrix ) ;
			}
			else
			if( FrameBase->LocalTransformMatrixType == 1 )
			{
				CreateTranslationMatrix( &ResultMatrix, FrameBase->LocalTransformMatrix.m[ 0 ][ 3 ], FrameBase->LocalTransformMatrix.m[ 1 ][ 3 ], FrameBase->LocalTransformMatrix.m[ 2 ][ 3 ] ) ;
			}
			else
			{
				ConvertMatrix4x4cFToMatrixF( &ResultMatrix, &FrameBase->LocalTransformMatrix ) ;
			}
		}
		else
		// �A�j���[�V������������Đ�����Ă���ꍇ�͕ʏ���
		if( mcon == 1 )
		{
			if( MAnim3->Anim->ValidFlag & MV1_ANIMVALUE_MATRIX )
			{
				ConvertMatrix4x4cFToMatrixF( &ResultMatrix, &MAnim3->Anim->Matrix ) ;
			}
			else
			{
				// �s��̃Z�b�g�A�b�v
				if( MAnim3->Anim->ValidBlendMatrix == false )
				{
					MV1SetupTransformMatrix(
						&MAnim3->Anim->BlendMatrix,
						MAnim3->Anim->ValidFlag,
						&MAnim3->Anim->Translate,
						&MAnim3->Anim->Scale,
						MAnim3->Anim->RotateOrder,
						( FrameBase->Flag & MV1_FRAMEFLAG_PREROTATE ) != 0 ? &FrameBase->PreRotate : NULL,
						&MAnim3->Anim->Rotate,
						( FrameBase->Flag & MV1_FRAMEFLAG_POSTROTATE ) != 0 ? &FrameBase->PostRotate : NULL,
						&MAnim3->Anim->Quaternion
					) ;
					MAnim3->Anim->ValidBlendMatrix = true ;
				}
				ConvertMatrix4x4cFToMatrixF( &ResultMatrix, &MAnim3->Anim->BlendMatrix ) ;
			}
		}
		else
		{
			MV1_ANIM * RST Anim ;
			VECTOR DivSize ;

			// �s�񂪂��邩�A�N�H�[�^�j�I���Ƃw�x�y����]���������Ă��邩
			// �f�t�H���g�p�����[�^�������ȏ�ɓ��ĂĂ���A�j���[�V�����̎�ނ��Ⴄ�ꍇ�͍s��u�����h
//			if( ( BlendFlag & MV1_ANIMVALUE_MATRIX ) ||
//				( BlendFlag & ( MV1_ANIMVALUE_ROTATE | MV1_ANIMVALUE_QUATERNION_X   ) ) == ( MV1_ANIMVALUE_ROTATE | MV1_ANIMVALUE_QUATERNION_X ) ||
//				( BlendFlag & ( MV1_ANIMVALUE_ROTATE | MV1_ANIMVALUE_QUATERNION_VMD ) ) == ( MV1_ANIMVALUE_ROTATE | MV1_ANIMVALUE_QUATERNION_VMD ) )
			if( 1 )
			{
				int Normalize ;

				Normalize = TRUE ;

				_MEMSET( &BlendMat, 0, sizeof( BlendMat ) ) ;
				MAnim2 = MAnim ;
				for( i = 0 ; i < Model->AnimSetMaxNum ; i ++, MAnim2 ++ )
				{
					if( MAnim2->Use == false || MAnim2->BlendRate == 0.0f ) continue ;

					BlendRate = MAnim2->BlendRate ;
					Anim = MAnim2->Anim ;

					if( Anim->ValidFlag & MV1_ANIMVALUE_MATRIX )
					{
						if( BlendRate == 1.0f )
						{
							BlendMat.m[ 0 ][ 0 ] += Anim->Matrix.m[ 0 ][ 0 ] - FrameBase->LocalTransformMatrix.m[ 0 ][ 0 ] ;
							BlendMat.m[ 0 ][ 1 ] += Anim->Matrix.m[ 0 ][ 1 ] - FrameBase->LocalTransformMatrix.m[ 0 ][ 1 ] ;
							BlendMat.m[ 0 ][ 2 ] += Anim->Matrix.m[ 0 ][ 2 ] - FrameBase->LocalTransformMatrix.m[ 0 ][ 2 ] ;
							BlendMat.m[ 0 ][ 3 ] += Anim->Matrix.m[ 0 ][ 3 ] - FrameBase->LocalTransformMatrix.m[ 0 ][ 3 ] ;

							BlendMat.m[ 1 ][ 0 ] += Anim->Matrix.m[ 1 ][ 0 ] - FrameBase->LocalTransformMatrix.m[ 1 ][ 0 ] ;
							BlendMat.m[ 1 ][ 1 ] += Anim->Matrix.m[ 1 ][ 1 ] - FrameBase->LocalTransformMatrix.m[ 1 ][ 1 ] ;
							BlendMat.m[ 1 ][ 2 ] += Anim->Matrix.m[ 1 ][ 2 ] - FrameBase->LocalTransformMatrix.m[ 1 ][ 2 ] ;
							BlendMat.m[ 1 ][ 3 ] += Anim->Matrix.m[ 1 ][ 3 ] - FrameBase->LocalTransformMatrix.m[ 1 ][ 3 ] ;

							BlendMat.m[ 2 ][ 0 ] += Anim->Matrix.m[ 2 ][ 0 ] - FrameBase->LocalTransformMatrix.m[ 2 ][ 0 ] ;
							BlendMat.m[ 2 ][ 1 ] += Anim->Matrix.m[ 2 ][ 1 ] - FrameBase->LocalTransformMatrix.m[ 2 ][ 1 ] ;
							BlendMat.m[ 2 ][ 2 ] += Anim->Matrix.m[ 2 ][ 2 ] - FrameBase->LocalTransformMatrix.m[ 2 ][ 2 ] ;
							BlendMat.m[ 2 ][ 3 ] += Anim->Matrix.m[ 2 ][ 3 ] - FrameBase->LocalTransformMatrix.m[ 2 ][ 3 ] ;
						}
						else
						{
							BlendMat.m[ 0 ][ 0 ] += ( Anim->Matrix.m[ 0 ][ 0 ] - FrameBase->LocalTransformMatrix.m[ 0 ][ 0 ] ) * BlendRate ;
							BlendMat.m[ 0 ][ 1 ] += ( Anim->Matrix.m[ 0 ][ 1 ] - FrameBase->LocalTransformMatrix.m[ 0 ][ 1 ] ) * BlendRate ;
							BlendMat.m[ 0 ][ 2 ] += ( Anim->Matrix.m[ 0 ][ 2 ] - FrameBase->LocalTransformMatrix.m[ 0 ][ 2 ] ) * BlendRate ;
							BlendMat.m[ 0 ][ 3 ] += ( Anim->Matrix.m[ 0 ][ 3 ] - FrameBase->LocalTransformMatrix.m[ 0 ][ 3 ] ) * BlendRate ;

							BlendMat.m[ 1 ][ 0 ] += ( Anim->Matrix.m[ 1 ][ 0 ] - FrameBase->LocalTransformMatrix.m[ 1 ][ 0 ] ) * BlendRate ;
							BlendMat.m[ 1 ][ 1 ] += ( Anim->Matrix.m[ 1 ][ 1 ] - FrameBase->LocalTransformMatrix.m[ 1 ][ 1 ] ) * BlendRate ;
							BlendMat.m[ 1 ][ 2 ] += ( Anim->Matrix.m[ 1 ][ 2 ] - FrameBase->LocalTransformMatrix.m[ 1 ][ 2 ] ) * BlendRate ;
							BlendMat.m[ 1 ][ 3 ] += ( Anim->Matrix.m[ 1 ][ 3 ] - FrameBase->LocalTransformMatrix.m[ 1 ][ 3 ] ) * BlendRate ;

							BlendMat.m[ 2 ][ 0 ] += ( Anim->Matrix.m[ 2 ][ 0 ] - FrameBase->LocalTransformMatrix.m[ 2 ][ 0 ] ) * BlendRate ;
							BlendMat.m[ 2 ][ 1 ] += ( Anim->Matrix.m[ 2 ][ 1 ] - FrameBase->LocalTransformMatrix.m[ 2 ][ 1 ] ) * BlendRate ;
							BlendMat.m[ 2 ][ 2 ] += ( Anim->Matrix.m[ 2 ][ 2 ] - FrameBase->LocalTransformMatrix.m[ 2 ][ 2 ] ) * BlendRate ;
							BlendMat.m[ 2 ][ 3 ] += ( Anim->Matrix.m[ 2 ][ 3 ] - FrameBase->LocalTransformMatrix.m[ 2 ][ 3 ] ) * BlendRate ;
						}

						Normalize = FALSE ;
					}
					else
					{
						// �s��̃Z�b�g�A�b�v
						if( Anim->ValidBlendMatrix == false )
						{
							MV1SetupTransformMatrix(
								&Anim->BlendMatrix,
								Anim->ValidFlag,
								&Anim->Translate,
								&Anim->Scale,
								Anim->RotateOrder,
								( FrameBase->Flag & MV1_FRAMEFLAG_PREROTATE ) != 0 ? &FrameBase->PreRotate : NULL,
								&Anim->Rotate,
								( FrameBase->Flag & MV1_FRAMEFLAG_POSTROTATE ) != 0 ? &FrameBase->PostRotate : NULL,
								&Anim->Quaternion ) ;
							Anim->ValidBlendMatrix = true ;

							if( ( Anim->ValidFlag & MV1_ANIMVALUE_SCALE ) &&
								( Anim->Scale.x < 0.9999999f || Anim->Scale.x > 1.0000001f ||
								  Anim->Scale.y < 0.9999999f || Anim->Scale.y > 1.0000001f ||
								  Anim->Scale.z < 0.9999999f || Anim->Scale.z > 1.0000001f ) )
							{
								Anim->BlendMatrixUseScaling = true ;
							}
						}

						if( Anim->BlendMatrixUseScaling )
						{
							Normalize = FALSE ;
						}

						if( BlendRate == 1.0f )
						{
							BlendMat.m[ 0 ][ 0 ] += Anim->BlendMatrix.m[ 0 ][ 0 ] - FrameBase->LocalTransformMatrix.m[ 0 ][ 0 ] ;
							BlendMat.m[ 0 ][ 1 ] += Anim->BlendMatrix.m[ 0 ][ 1 ] - FrameBase->LocalTransformMatrix.m[ 0 ][ 1 ] ;
							BlendMat.m[ 0 ][ 2 ] += Anim->BlendMatrix.m[ 0 ][ 2 ] - FrameBase->LocalTransformMatrix.m[ 0 ][ 2 ] ;
							BlendMat.m[ 0 ][ 3 ] += Anim->BlendMatrix.m[ 0 ][ 3 ] - FrameBase->LocalTransformMatrix.m[ 0 ][ 3 ] ;

							BlendMat.m[ 1 ][ 0 ] += Anim->BlendMatrix.m[ 1 ][ 0 ] - FrameBase->LocalTransformMatrix.m[ 1 ][ 0 ] ;
							BlendMat.m[ 1 ][ 1 ] += Anim->BlendMatrix.m[ 1 ][ 1 ] - FrameBase->LocalTransformMatrix.m[ 1 ][ 1 ] ;
							BlendMat.m[ 1 ][ 2 ] += Anim->BlendMatrix.m[ 1 ][ 2 ] - FrameBase->LocalTransformMatrix.m[ 1 ][ 2 ] ;
							BlendMat.m[ 1 ][ 3 ] += Anim->BlendMatrix.m[ 1 ][ 3 ] - FrameBase->LocalTransformMatrix.m[ 1 ][ 3 ] ;

							BlendMat.m[ 2 ][ 0 ] += Anim->BlendMatrix.m[ 2 ][ 0 ] - FrameBase->LocalTransformMatrix.m[ 2 ][ 0 ] ;
							BlendMat.m[ 2 ][ 1 ] += Anim->BlendMatrix.m[ 2 ][ 1 ] - FrameBase->LocalTransformMatrix.m[ 2 ][ 1 ] ;
							BlendMat.m[ 2 ][ 2 ] += Anim->BlendMatrix.m[ 2 ][ 2 ] - FrameBase->LocalTransformMatrix.m[ 2 ][ 2 ] ;
							BlendMat.m[ 2 ][ 3 ] += Anim->BlendMatrix.m[ 2 ][ 3 ] - FrameBase->LocalTransformMatrix.m[ 2 ][ 3 ] ;
						}
						else
						{
							BlendMat.m[ 0 ][ 0 ] += ( Anim->BlendMatrix.m[ 0 ][ 0 ] - FrameBase->LocalTransformMatrix.m[ 0 ][ 0 ] ) * BlendRate ;
							BlendMat.m[ 0 ][ 1 ] += ( Anim->BlendMatrix.m[ 0 ][ 1 ] - FrameBase->LocalTransformMatrix.m[ 0 ][ 1 ] ) * BlendRate ;
							BlendMat.m[ 0 ][ 2 ] += ( Anim->BlendMatrix.m[ 0 ][ 2 ] - FrameBase->LocalTransformMatrix.m[ 0 ][ 2 ] ) * BlendRate ;
							BlendMat.m[ 0 ][ 3 ] += ( Anim->BlendMatrix.m[ 0 ][ 3 ] - FrameBase->LocalTransformMatrix.m[ 0 ][ 3 ] ) * BlendRate ;

							BlendMat.m[ 1 ][ 0 ] += ( Anim->BlendMatrix.m[ 1 ][ 0 ] - FrameBase->LocalTransformMatrix.m[ 1 ][ 0 ] ) * BlendRate ;
							BlendMat.m[ 1 ][ 1 ] += ( Anim->BlendMatrix.m[ 1 ][ 1 ] - FrameBase->LocalTransformMatrix.m[ 1 ][ 1 ] ) * BlendRate ;
							BlendMat.m[ 1 ][ 2 ] += ( Anim->BlendMatrix.m[ 1 ][ 2 ] - FrameBase->LocalTransformMatrix.m[ 1 ][ 2 ] ) * BlendRate ;
							BlendMat.m[ 1 ][ 3 ] += ( Anim->BlendMatrix.m[ 1 ][ 3 ] - FrameBase->LocalTransformMatrix.m[ 1 ][ 3 ] ) * BlendRate ;

							BlendMat.m[ 2 ][ 0 ] += ( Anim->BlendMatrix.m[ 2 ][ 0 ] - FrameBase->LocalTransformMatrix.m[ 2 ][ 0 ] ) * BlendRate ;
							BlendMat.m[ 2 ][ 1 ] += ( Anim->BlendMatrix.m[ 2 ][ 1 ] - FrameBase->LocalTransformMatrix.m[ 2 ][ 1 ] ) * BlendRate ;
							BlendMat.m[ 2 ][ 2 ] += ( Anim->BlendMatrix.m[ 2 ][ 2 ] - FrameBase->LocalTransformMatrix.m[ 2 ][ 2 ] ) * BlendRate ;
							BlendMat.m[ 2 ][ 3 ] += ( Anim->BlendMatrix.m[ 2 ][ 3 ] - FrameBase->LocalTransformMatrix.m[ 2 ][ 3 ] ) * BlendRate ;
						}
					}
				}

				BlendMat.m[ 0 ][ 0 ] += FrameBase->LocalTransformMatrix.m[ 0 ][ 0 ] ;
				BlendMat.m[ 0 ][ 1 ] += FrameBase->LocalTransformMatrix.m[ 0 ][ 1 ] ;
				BlendMat.m[ 0 ][ 2 ] += FrameBase->LocalTransformMatrix.m[ 0 ][ 2 ] ;
				BlendMat.m[ 0 ][ 3 ] += FrameBase->LocalTransformMatrix.m[ 0 ][ 3 ] ;

				BlendMat.m[ 1 ][ 0 ] += FrameBase->LocalTransformMatrix.m[ 1 ][ 0 ] ;
				BlendMat.m[ 1 ][ 1 ] += FrameBase->LocalTransformMatrix.m[ 1 ][ 1 ] ;
				BlendMat.m[ 1 ][ 2 ] += FrameBase->LocalTransformMatrix.m[ 1 ][ 2 ] ;
				BlendMat.m[ 1 ][ 3 ] += FrameBase->LocalTransformMatrix.m[ 1 ][ 3 ] ;

				BlendMat.m[ 2 ][ 0 ] += FrameBase->LocalTransformMatrix.m[ 2 ][ 0 ] ;
				BlendMat.m[ 2 ][ 1 ] += FrameBase->LocalTransformMatrix.m[ 2 ][ 1 ] ;
				BlendMat.m[ 2 ][ 2 ] += FrameBase->LocalTransformMatrix.m[ 2 ][ 2 ] ;
				BlendMat.m[ 2 ][ 3 ] += FrameBase->LocalTransformMatrix.m[ 2 ][ 3 ] ;

				if( FrameBase->LocalTransformMatrixUseScaling )
				{
					Normalize = FALSE ;
				}

				if( Normalize )
				{
					DivSize.x = 1.0f / _SQRT( BlendMat.m[ 0 ][ 0 ] * BlendMat.m[ 0 ][ 0 ] + BlendMat.m[ 0 ][ 1 ] * BlendMat.m[ 0 ][ 1 ] + BlendMat.m[ 0 ][ 2 ] * BlendMat.m[ 0 ][ 2 ] ) ;
					DivSize.y = 1.0f / _SQRT( BlendMat.m[ 1 ][ 0 ] * BlendMat.m[ 1 ][ 0 ] + BlendMat.m[ 1 ][ 1 ] * BlendMat.m[ 1 ][ 1 ] + BlendMat.m[ 1 ][ 2 ] * BlendMat.m[ 1 ][ 2 ] ) ;
					DivSize.z = 1.0f / _SQRT( BlendMat.m[ 2 ][ 0 ] * BlendMat.m[ 2 ][ 0 ] + BlendMat.m[ 2 ][ 1 ] * BlendMat.m[ 2 ][ 1 ] + BlendMat.m[ 2 ][ 2 ] * BlendMat.m[ 2 ][ 2 ] ) ;

					BlendMat.m[ 0 ][ 0 ] *= DivSize.x ;
					BlendMat.m[ 0 ][ 1 ] *= DivSize.x ;
					BlendMat.m[ 0 ][ 2 ] *= DivSize.x ;

					BlendMat.m[ 1 ][ 0 ] *= DivSize.y ;
					BlendMat.m[ 1 ][ 1 ] *= DivSize.y ;
					BlendMat.m[ 1 ][ 2 ] *= DivSize.y ;

					BlendMat.m[ 2 ][ 0 ] *= DivSize.z ;
					BlendMat.m[ 2 ][ 1 ] *= DivSize.z ;
					BlendMat.m[ 2 ][ 2 ] *= DivSize.z ;
				}
			}
			else
			{
				// ����ȊO�̏ꍇ�̓p�����[�^���x���̃u�����h����
				Translate.x = 0.0f ;
				Translate.y = 0.0f ;
				Translate.z = 0.0f ;
				Scale.x = 0.0f ;
				Scale.y = 0.0f ;
				Scale.z = 0.0f ;
				Rotate.x = 0.0f ;
				Rotate.y = 0.0f ;
				Rotate.z = 0.0f ;
				Quaternion.x = 0.0f ;
				Quaternion.y = 0.0f ;
				Quaternion.z = 0.0f ;
				Quaternion.w = 1.0f ;

				MAnim2 = MAnim ;
				for( i = 0 ; i < Model->AnimSetMaxNum ; i ++, MAnim2 ++ )
				{
					if( MAnim2->Use == false || MAnim2->BlendRate == 0.0f ) continue ;

					BlendRate = MAnim2->BlendRate ;
					Anim = MAnim2->Anim ;

					if( BlendRate == 1.0f )
					{
						if( Anim->ValidFlag & MV1_ANIMVALUE_TRANSLATE )
						{
							Translate.x += Anim->Translate.x - FrameBase->Translate.x ;
							Translate.y += Anim->Translate.y - FrameBase->Translate.y ;
							Translate.z += Anim->Translate.z - FrameBase->Translate.z ;
						}

						if( Anim->ValidFlag & MV1_ANIMVALUE_SCALE )
						{
							Scale.x += Anim->Scale.x - FrameBase->Scale.x ;
							Scale.y += Anim->Scale.y - FrameBase->Scale.y ;
							Scale.z += Anim->Scale.z - FrameBase->Scale.z ;
						}

						if( Anim->ValidFlag & MV1_ANIMVALUE_ROTATE )
						{
							Rotate.x += Anim->Rotate.x - FrameBase->Rotate.x ;
							Rotate.y += Anim->Rotate.y - FrameBase->Rotate.y ;
							Rotate.z += Anim->Rotate.z - FrameBase->Rotate.z ;
						}

						if( Anim->ValidFlag & ( MV1_ANIMVALUE_QUATERNION_X | MV1_ANIMVALUE_QUATERNION_VMD ) )
						{
							Quaternion.x = Anim->Quaternion.x - FrameBase->Quaternion.x ;
							Quaternion.y = Anim->Quaternion.y - FrameBase->Quaternion.y ;
							Quaternion.z = Anim->Quaternion.z - FrameBase->Quaternion.z ;
							Quaternion.w = Anim->Quaternion.w - FrameBase->Quaternion.w ;
						}
					}
					else
					{
						if( Anim->ValidFlag & MV1_ANIMVALUE_TRANSLATE )
						{
							Translate.x += ( Anim->Translate.x - FrameBase->Translate.x ) * BlendRate ;
							Translate.y += ( Anim->Translate.y - FrameBase->Translate.y ) * BlendRate ;
							Translate.z += ( Anim->Translate.z - FrameBase->Translate.z ) * BlendRate ;
						}

						if( Anim->ValidFlag & MV1_ANIMVALUE_SCALE )
						{
							Scale.x += ( Anim->Scale.x - FrameBase->Scale.x ) * BlendRate ;
							Scale.y += ( Anim->Scale.y - FrameBase->Scale.y ) * BlendRate ;
							Scale.z += ( Anim->Scale.z - FrameBase->Scale.z ) * BlendRate ;
						}

						if( Anim->ValidFlag & MV1_ANIMVALUE_ROTATE )
						{
							Rotate.x += ( Anim->Rotate.x - FrameBase->Rotate.x ) * BlendRate ;
							Rotate.y += ( Anim->Rotate.y - FrameBase->Rotate.y ) * BlendRate ;
							Rotate.z += ( Anim->Rotate.z - FrameBase->Rotate.z ) * BlendRate ;
						}

						if( Anim->ValidFlag & ( MV1_ANIMVALUE_QUATERNION_X | MV1_ANIMVALUE_QUATERNION_VMD ) )
						{
							Quaternion.x = ( Anim->Quaternion.x - FrameBase->Quaternion.x ) * BlendRate ;
							Quaternion.y = ( Anim->Quaternion.y - FrameBase->Quaternion.y ) * BlendRate ;
							Quaternion.z = ( Anim->Quaternion.z - FrameBase->Quaternion.z ) * BlendRate ;
							Quaternion.w = ( Anim->Quaternion.w - FrameBase->Quaternion.w ) * BlendRate ;
						}
					}
				}

				Translate.x += FrameBase->Translate.x ;
				Translate.y += FrameBase->Translate.y ;
				Translate.z += FrameBase->Translate.z ;

				Scale.x += FrameBase->Scale.x ;
				Scale.y += FrameBase->Scale.y ;
				Scale.z += FrameBase->Scale.z ;

				Rotate.x += FrameBase->Rotate.x ;
				Rotate.y += FrameBase->Rotate.y ;
				Rotate.z += FrameBase->Rotate.z ;

				Quaternion.x += FrameBase->Quaternion.x ;
				Quaternion.y += FrameBase->Quaternion.y ;
				Quaternion.z += FrameBase->Quaternion.z ;
				Quaternion.w += FrameBase->Quaternion.w ;

				// �s��̃Z�b�g�A�b�v
				MV1SetupTransformMatrix(
					&BlendMat,
					BlendFlag,
					&Translate,
					&Scale,
					FrameBase->RotateOrder,
					( FrameBase->Flag & MV1_FRAMEFLAG_PREROTATE ) != 0 ? &FrameBase->PreRotate : NULL,
					&Rotate,
					( FrameBase->Flag & MV1_FRAMEFLAG_POSTROTATE ) != 0 ? &FrameBase->PostRotate : NULL,
					&Quaternion
				) ;
			}

			// �߂�l�p�̍s��ɒu��������
			ConvertMatrix4x4cFToMatrixF( &ResultMatrix, &BlendMat ) ;
		}
	}

	// �߂�l�Ƃ��ĕԂ�
	return ResultMatrix ;
}

// �w��̃t���[���̃��[�J�����W�ϊ��s����擾����
#ifdef __BCC
static DummyFunc_MV1GetFrameLocalMatrixD(){}
#endif
extern MATRIX_D NS_MV1GetFrameLocalMatrixD( int MHandle, int FrameIndex )
{
	MATRIX_D ResultMatrix ;
	MV1_FRAME_BASE *FrameBase ;

// BorlandC++ �̍œK���o�O�}���p
#ifdef __BCC
	DummyFunc_MV1GetFrameLocalMatrixD();
#endif

	MV1FRAMESTART( MHandle, Model, ModelBase, Frame, FrameIndex, MGetIdentD() ) ;
	FrameBase = Frame->BaseData ;

	// �s��̃Z�b�g�A�b�v���s��
	MV1SETUPMATRIX( Model ) ;

	// ���[�U�[�w��̍s�񂪂���ꍇ�͂����Ԃ�
	if( Frame->ValidUserLocalTransformMatrix )
	{
		ConvertMatrix4x4cToMatrixD( &ResultMatrix, &Frame->UserLocalTransformMatrix ) ;
	}
	else
	// �A�j���[�V����������ꍇ�Ɩ����ꍇ�ŏ����𕪊�
	if( Model->AnimSetNum == 0 )
	{
		// �A�j���[�V�������Ȃ��ꍇ�̓f�t�H���g�s���Ԃ�
		ConvertMatrix4x4cFToMatrixD( &ResultMatrix, &FrameBase->LocalTransformMatrix ) ;
	}
	else
	{
		// �A�j���[�V����������ꍇ
		MATRIX_4X4CT_F BlendMat ;
		float BlendRate ;
		VECTOR Translate, Scale, Rotate ;
		FLOAT4 Quaternion ;
		int BlendFlag, mcon, i ;
		MV1_MODEL_ANIM *MAnim, *MAnim2, *MAnim3 = NULL ;

		// �p�����[�^���x���̃u�����h���s���邩�𒲂ׂ�
		MAnim = Model->Anim + Model->AnimSetMaxNum * Frame->BaseData->Index ;
		MAnim2 = MAnim ;
		BlendFlag = 0 ;
		mcon = 0 ;
		for( i = 0 ; i < Model->AnimSetMaxNum ; i ++, MAnim2 ++ )
		{
			if( MAnim2->Use == false || MAnim2->BlendRate == 0.0f )
				continue ;
			mcon ++ ;
			MAnim3 = MAnim2 ;

			BlendFlag |= MAnim2->Anim->ValidFlag ;
		}

		// �A�j���[�V�������Đ�����Ă��Ȃ��ꍇ�̓f�t�H���g�̍s���K������
		if( mcon == 0 )
		{
			if( FrameBase->LocalTransformMatrixType == 0 )
			{
				CreateIdentityMatrixD( &ResultMatrix ) ;
			}
			else
			if( FrameBase->LocalTransformMatrixType == 1 )
			{
				CreateTranslationMatrixD( &ResultMatrix, FrameBase->LocalTransformMatrix.m[ 0 ][ 3 ], FrameBase->LocalTransformMatrix.m[ 1 ][ 3 ], FrameBase->LocalTransformMatrix.m[ 2 ][ 3 ] ) ;
			}
			else
			{
				ConvertMatrix4x4cFToMatrixD( &ResultMatrix, &FrameBase->LocalTransformMatrix ) ;
			}
		}
		else
		// �A�j���[�V������������Đ�����Ă���ꍇ�͕ʏ���
		if( mcon == 1 )
		{
			if( MAnim3->Anim->ValidFlag & MV1_ANIMVALUE_MATRIX )
			{
				ConvertMatrix4x4cFToMatrixD( &ResultMatrix, &MAnim3->Anim->Matrix ) ;
			}
			else
			{
				// �s��̃Z�b�g�A�b�v
				if( MAnim3->Anim->ValidBlendMatrix == false )
				{
					MV1SetupTransformMatrix(
						&MAnim3->Anim->BlendMatrix,
						MAnim3->Anim->ValidFlag,
						&MAnim3->Anim->Translate,
						&MAnim3->Anim->Scale,
						MAnim3->Anim->RotateOrder,
						( FrameBase->Flag & MV1_FRAMEFLAG_PREROTATE ) != 0 ? &FrameBase->PreRotate : NULL,
						&MAnim3->Anim->Rotate,
						( FrameBase->Flag & MV1_FRAMEFLAG_POSTROTATE ) != 0 ? &FrameBase->PostRotate : NULL,
						&MAnim3->Anim->Quaternion
					) ;
					MAnim3->Anim->ValidBlendMatrix = true ;
				}
				ConvertMatrix4x4cFToMatrixD( &ResultMatrix, &MAnim3->Anim->BlendMatrix ) ;
			}
		}
		else
		{
			MV1_ANIM * RST Anim ;
			VECTOR DivSize ;

			// �s�񂪂��邩�A�N�H�[�^�j�I���Ƃw�x�y����]���������Ă��邩
			// �f�t�H���g�p�����[�^�������ȏ�ɓ��ĂĂ���A�j���[�V�����̎�ނ��Ⴄ�ꍇ�͍s��u�����h
//			if( ( BlendFlag & MV1_ANIMVALUE_MATRIX ) ||
//				( BlendFlag & ( MV1_ANIMVALUE_ROTATE | MV1_ANIMVALUE_QUATERNION_X   ) ) == ( MV1_ANIMVALUE_ROTATE | MV1_ANIMVALUE_QUATERNION_X ) ||
//				( BlendFlag & ( MV1_ANIMVALUE_ROTATE | MV1_ANIMVALUE_QUATERNION_VMD ) ) == ( MV1_ANIMVALUE_ROTATE | MV1_ANIMVALUE_QUATERNION_VMD ) )
			if( 1 )
			{
				int Normalize ;

				Normalize = TRUE ;

				_MEMSET( &BlendMat, 0, sizeof( BlendMat ) ) ;
				MAnim2 = MAnim ;
				for( i = 0 ; i < Model->AnimSetMaxNum ; i ++, MAnim2 ++ )
				{
					if( MAnim2->Use == false || MAnim2->BlendRate == 0.0f ) continue ;

					BlendRate = MAnim2->BlendRate ;
					Anim = MAnim2->Anim ;

					if( Anim->ValidFlag & MV1_ANIMVALUE_MATRIX )
					{
						if( BlendRate == 1.0f )
						{
							BlendMat.m[ 0 ][ 0 ] += Anim->Matrix.m[ 0 ][ 0 ] - FrameBase->LocalTransformMatrix.m[ 0 ][ 0 ] ;
							BlendMat.m[ 0 ][ 1 ] += Anim->Matrix.m[ 0 ][ 1 ] - FrameBase->LocalTransformMatrix.m[ 0 ][ 1 ] ;
							BlendMat.m[ 0 ][ 2 ] += Anim->Matrix.m[ 0 ][ 2 ] - FrameBase->LocalTransformMatrix.m[ 0 ][ 2 ] ;
							BlendMat.m[ 0 ][ 3 ] += Anim->Matrix.m[ 0 ][ 3 ] - FrameBase->LocalTransformMatrix.m[ 0 ][ 3 ] ;

							BlendMat.m[ 1 ][ 0 ] += Anim->Matrix.m[ 1 ][ 0 ] - FrameBase->LocalTransformMatrix.m[ 1 ][ 0 ] ;
							BlendMat.m[ 1 ][ 1 ] += Anim->Matrix.m[ 1 ][ 1 ] - FrameBase->LocalTransformMatrix.m[ 1 ][ 1 ] ;
							BlendMat.m[ 1 ][ 2 ] += Anim->Matrix.m[ 1 ][ 2 ] - FrameBase->LocalTransformMatrix.m[ 1 ][ 2 ] ;
							BlendMat.m[ 1 ][ 3 ] += Anim->Matrix.m[ 1 ][ 3 ] - FrameBase->LocalTransformMatrix.m[ 1 ][ 3 ] ;

							BlendMat.m[ 2 ][ 0 ] += Anim->Matrix.m[ 2 ][ 0 ] - FrameBase->LocalTransformMatrix.m[ 2 ][ 0 ] ;
							BlendMat.m[ 2 ][ 1 ] += Anim->Matrix.m[ 2 ][ 1 ] - FrameBase->LocalTransformMatrix.m[ 2 ][ 1 ] ;
							BlendMat.m[ 2 ][ 2 ] += Anim->Matrix.m[ 2 ][ 2 ] - FrameBase->LocalTransformMatrix.m[ 2 ][ 2 ] ;
							BlendMat.m[ 2 ][ 3 ] += Anim->Matrix.m[ 2 ][ 3 ] - FrameBase->LocalTransformMatrix.m[ 2 ][ 3 ] ;
						}
						else
						{
							BlendMat.m[ 0 ][ 0 ] += ( Anim->Matrix.m[ 0 ][ 0 ] - FrameBase->LocalTransformMatrix.m[ 0 ][ 0 ] ) * BlendRate ;
							BlendMat.m[ 0 ][ 1 ] += ( Anim->Matrix.m[ 0 ][ 1 ] - FrameBase->LocalTransformMatrix.m[ 0 ][ 1 ] ) * BlendRate ;
							BlendMat.m[ 0 ][ 2 ] += ( Anim->Matrix.m[ 0 ][ 2 ] - FrameBase->LocalTransformMatrix.m[ 0 ][ 2 ] ) * BlendRate ;
							BlendMat.m[ 0 ][ 3 ] += ( Anim->Matrix.m[ 0 ][ 3 ] - FrameBase->LocalTransformMatrix.m[ 0 ][ 3 ] ) * BlendRate ;

							BlendMat.m[ 1 ][ 0 ] += ( Anim->Matrix.m[ 1 ][ 0 ] - FrameBase->LocalTransformMatrix.m[ 1 ][ 0 ] ) * BlendRate ;
							BlendMat.m[ 1 ][ 1 ] += ( Anim->Matrix.m[ 1 ][ 1 ] - FrameBase->LocalTransformMatrix.m[ 1 ][ 1 ] ) * BlendRate ;
							BlendMat.m[ 1 ][ 2 ] += ( Anim->Matrix.m[ 1 ][ 2 ] - FrameBase->LocalTransformMatrix.m[ 1 ][ 2 ] ) * BlendRate ;
							BlendMat.m[ 1 ][ 3 ] += ( Anim->Matrix.m[ 1 ][ 3 ] - FrameBase->LocalTransformMatrix.m[ 1 ][ 3 ] ) * BlendRate ;

							BlendMat.m[ 2 ][ 0 ] += ( Anim->Matrix.m[ 2 ][ 0 ] - FrameBase->LocalTransformMatrix.m[ 2 ][ 0 ] ) * BlendRate ;
							BlendMat.m[ 2 ][ 1 ] += ( Anim->Matrix.m[ 2 ][ 1 ] - FrameBase->LocalTransformMatrix.m[ 2 ][ 1 ] ) * BlendRate ;
							BlendMat.m[ 2 ][ 2 ] += ( Anim->Matrix.m[ 2 ][ 2 ] - FrameBase->LocalTransformMatrix.m[ 2 ][ 2 ] ) * BlendRate ;
							BlendMat.m[ 2 ][ 3 ] += ( Anim->Matrix.m[ 2 ][ 3 ] - FrameBase->LocalTransformMatrix.m[ 2 ][ 3 ] ) * BlendRate ;
						}

						Normalize = FALSE ;
					}
					else
					{
						// �s��̃Z�b�g�A�b�v
						if( Anim->ValidBlendMatrix == false )
						{
							MV1SetupTransformMatrix(
								&Anim->BlendMatrix,
								Anim->ValidFlag,
								&Anim->Translate,
								&Anim->Scale,
								Anim->RotateOrder,
								( FrameBase->Flag & MV1_FRAMEFLAG_PREROTATE ) != 0 ? &FrameBase->PreRotate : NULL,
								&Anim->Rotate,
								( FrameBase->Flag & MV1_FRAMEFLAG_POSTROTATE ) != 0 ? &FrameBase->PostRotate : NULL,
								&Anim->Quaternion ) ;
							Anim->ValidBlendMatrix = true ;

							if( ( Anim->ValidFlag & MV1_ANIMVALUE_SCALE ) &&
								( Anim->Scale.x < 0.9999999f || Anim->Scale.x > 1.0000001f ||
								  Anim->Scale.y < 0.9999999f || Anim->Scale.y > 1.0000001f ||
								  Anim->Scale.z < 0.9999999f || Anim->Scale.z > 1.0000001f ) )
							{
								Anim->BlendMatrixUseScaling = true ;
							}
						}

						if( Anim->BlendMatrixUseScaling )
						{
							Normalize = FALSE ;
						}

						if( BlendRate == 1.0f )
						{
							BlendMat.m[ 0 ][ 0 ] += Anim->BlendMatrix.m[ 0 ][ 0 ] - FrameBase->LocalTransformMatrix.m[ 0 ][ 0 ] ;
							BlendMat.m[ 0 ][ 1 ] += Anim->BlendMatrix.m[ 0 ][ 1 ] - FrameBase->LocalTransformMatrix.m[ 0 ][ 1 ] ;
							BlendMat.m[ 0 ][ 2 ] += Anim->BlendMatrix.m[ 0 ][ 2 ] - FrameBase->LocalTransformMatrix.m[ 0 ][ 2 ] ;
							BlendMat.m[ 0 ][ 3 ] += Anim->BlendMatrix.m[ 0 ][ 3 ] - FrameBase->LocalTransformMatrix.m[ 0 ][ 3 ] ;

							BlendMat.m[ 1 ][ 0 ] += Anim->BlendMatrix.m[ 1 ][ 0 ] - FrameBase->LocalTransformMatrix.m[ 1 ][ 0 ] ;
							BlendMat.m[ 1 ][ 1 ] += Anim->BlendMatrix.m[ 1 ][ 1 ] - FrameBase->LocalTransformMatrix.m[ 1 ][ 1 ] ;
							BlendMat.m[ 1 ][ 2 ] += Anim->BlendMatrix.m[ 1 ][ 2 ] - FrameBase->LocalTransformMatrix.m[ 1 ][ 2 ] ;
							BlendMat.m[ 1 ][ 3 ] += Anim->BlendMatrix.m[ 1 ][ 3 ] - FrameBase->LocalTransformMatrix.m[ 1 ][ 3 ] ;

							BlendMat.m[ 2 ][ 0 ] += Anim->BlendMatrix.m[ 2 ][ 0 ] - FrameBase->LocalTransformMatrix.m[ 2 ][ 0 ] ;
							BlendMat.m[ 2 ][ 1 ] += Anim->BlendMatrix.m[ 2 ][ 1 ] - FrameBase->LocalTransformMatrix.m[ 2 ][ 1 ] ;
							BlendMat.m[ 2 ][ 2 ] += Anim->BlendMatrix.m[ 2 ][ 2 ] - FrameBase->LocalTransformMatrix.m[ 2 ][ 2 ] ;
							BlendMat.m[ 2 ][ 3 ] += Anim->BlendMatrix.m[ 2 ][ 3 ] - FrameBase->LocalTransformMatrix.m[ 2 ][ 3 ] ;
						}
						else
						{
							BlendMat.m[ 0 ][ 0 ] += ( Anim->BlendMatrix.m[ 0 ][ 0 ] - FrameBase->LocalTransformMatrix.m[ 0 ][ 0 ] ) * BlendRate ;
							BlendMat.m[ 0 ][ 1 ] += ( Anim->BlendMatrix.m[ 0 ][ 1 ] - FrameBase->LocalTransformMatrix.m[ 0 ][ 1 ] ) * BlendRate ;
							BlendMat.m[ 0 ][ 2 ] += ( Anim->BlendMatrix.m[ 0 ][ 2 ] - FrameBase->LocalTransformMatrix.m[ 0 ][ 2 ] ) * BlendRate ;
							BlendMat.m[ 0 ][ 3 ] += ( Anim->BlendMatrix.m[ 0 ][ 3 ] - FrameBase->LocalTransformMatrix.m[ 0 ][ 3 ] ) * BlendRate ;

							BlendMat.m[ 1 ][ 0 ] += ( Anim->BlendMatrix.m[ 1 ][ 0 ] - FrameBase->LocalTransformMatrix.m[ 1 ][ 0 ] ) * BlendRate ;
							BlendMat.m[ 1 ][ 1 ] += ( Anim->BlendMatrix.m[ 1 ][ 1 ] - FrameBase->LocalTransformMatrix.m[ 1 ][ 1 ] ) * BlendRate ;
							BlendMat.m[ 1 ][ 2 ] += ( Anim->BlendMatrix.m[ 1 ][ 2 ] - FrameBase->LocalTransformMatrix.m[ 1 ][ 2 ] ) * BlendRate ;
							BlendMat.m[ 1 ][ 3 ] += ( Anim->BlendMatrix.m[ 1 ][ 3 ] - FrameBase->LocalTransformMatrix.m[ 1 ][ 3 ] ) * BlendRate ;

							BlendMat.m[ 2 ][ 0 ] += ( Anim->BlendMatrix.m[ 2 ][ 0 ] - FrameBase->LocalTransformMatrix.m[ 2 ][ 0 ] ) * BlendRate ;
							BlendMat.m[ 2 ][ 1 ] += ( Anim->BlendMatrix.m[ 2 ][ 1 ] - FrameBase->LocalTransformMatrix.m[ 2 ][ 1 ] ) * BlendRate ;
							BlendMat.m[ 2 ][ 2 ] += ( Anim->BlendMatrix.m[ 2 ][ 2 ] - FrameBase->LocalTransformMatrix.m[ 2 ][ 2 ] ) * BlendRate ;
							BlendMat.m[ 2 ][ 3 ] += ( Anim->BlendMatrix.m[ 2 ][ 3 ] - FrameBase->LocalTransformMatrix.m[ 2 ][ 3 ] ) * BlendRate ;
						}
					}
				}

				BlendMat.m[ 0 ][ 0 ] += FrameBase->LocalTransformMatrix.m[ 0 ][ 0 ] ;
				BlendMat.m[ 0 ][ 1 ] += FrameBase->LocalTransformMatrix.m[ 0 ][ 1 ] ;
				BlendMat.m[ 0 ][ 2 ] += FrameBase->LocalTransformMatrix.m[ 0 ][ 2 ] ;
				BlendMat.m[ 0 ][ 3 ] += FrameBase->LocalTransformMatrix.m[ 0 ][ 3 ] ;

				BlendMat.m[ 1 ][ 0 ] += FrameBase->LocalTransformMatrix.m[ 1 ][ 0 ] ;
				BlendMat.m[ 1 ][ 1 ] += FrameBase->LocalTransformMatrix.m[ 1 ][ 1 ] ;
				BlendMat.m[ 1 ][ 2 ] += FrameBase->LocalTransformMatrix.m[ 1 ][ 2 ] ;
				BlendMat.m[ 1 ][ 3 ] += FrameBase->LocalTransformMatrix.m[ 1 ][ 3 ] ;

				BlendMat.m[ 2 ][ 0 ] += FrameBase->LocalTransformMatrix.m[ 2 ][ 0 ] ;
				BlendMat.m[ 2 ][ 1 ] += FrameBase->LocalTransformMatrix.m[ 2 ][ 1 ] ;
				BlendMat.m[ 2 ][ 2 ] += FrameBase->LocalTransformMatrix.m[ 2 ][ 2 ] ;
				BlendMat.m[ 2 ][ 3 ] += FrameBase->LocalTransformMatrix.m[ 2 ][ 3 ] ;

				if( FrameBase->LocalTransformMatrixUseScaling )
				{
					Normalize = FALSE ;
				}

				if( Normalize )
				{
					DivSize.x = 1.0f / _SQRT( BlendMat.m[ 0 ][ 0 ] * BlendMat.m[ 0 ][ 0 ] + BlendMat.m[ 0 ][ 1 ] * BlendMat.m[ 0 ][ 1 ] + BlendMat.m[ 0 ][ 2 ] * BlendMat.m[ 0 ][ 2 ] ) ;
					DivSize.y = 1.0f / _SQRT( BlendMat.m[ 1 ][ 0 ] * BlendMat.m[ 1 ][ 0 ] + BlendMat.m[ 1 ][ 1 ] * BlendMat.m[ 1 ][ 1 ] + BlendMat.m[ 1 ][ 2 ] * BlendMat.m[ 1 ][ 2 ] ) ;
					DivSize.z = 1.0f / _SQRT( BlendMat.m[ 2 ][ 0 ] * BlendMat.m[ 2 ][ 0 ] + BlendMat.m[ 2 ][ 1 ] * BlendMat.m[ 2 ][ 1 ] + BlendMat.m[ 2 ][ 2 ] * BlendMat.m[ 2 ][ 2 ] ) ;

					BlendMat.m[ 0 ][ 0 ] *= DivSize.x ;
					BlendMat.m[ 0 ][ 1 ] *= DivSize.x ;
					BlendMat.m[ 0 ][ 2 ] *= DivSize.x ;

					BlendMat.m[ 1 ][ 0 ] *= DivSize.y ;
					BlendMat.m[ 1 ][ 1 ] *= DivSize.y ;
					BlendMat.m[ 1 ][ 2 ] *= DivSize.y ;

					BlendMat.m[ 2 ][ 0 ] *= DivSize.z ;
					BlendMat.m[ 2 ][ 1 ] *= DivSize.z ;
					BlendMat.m[ 2 ][ 2 ] *= DivSize.z ;
				}
			}
			else
			{
				// ����ȊO�̏ꍇ�̓p�����[�^���x���̃u�����h����
				Translate.x = 0.0f ;
				Translate.y = 0.0f ;
				Translate.z = 0.0f ;
				Scale.x = 0.0f ;
				Scale.y = 0.0f ;
				Scale.z = 0.0f ;
				Rotate.x = 0.0f ;
				Rotate.y = 0.0f ;
				Rotate.z = 0.0f ;
				Quaternion.x = 0.0f ;
				Quaternion.y = 0.0f ;
				Quaternion.z = 0.0f ;
				Quaternion.w = 1.0f ;

				MAnim2 = MAnim ;
				for( i = 0 ; i < Model->AnimSetMaxNum ; i ++, MAnim2 ++ )
				{
					if( MAnim2->Use == false || MAnim2->BlendRate == 0.0f ) continue ;

					BlendRate = MAnim2->BlendRate ;
					Anim = MAnim2->Anim ;

					if( BlendRate == 1.0f )
					{
						if( Anim->ValidFlag & MV1_ANIMVALUE_TRANSLATE )
						{
							Translate.x += Anim->Translate.x - FrameBase->Translate.x ;
							Translate.y += Anim->Translate.y - FrameBase->Translate.y ;
							Translate.z += Anim->Translate.z - FrameBase->Translate.z ;
						}

						if( Anim->ValidFlag & MV1_ANIMVALUE_SCALE )
						{
							Scale.x += Anim->Scale.x - FrameBase->Scale.x ;
							Scale.y += Anim->Scale.y - FrameBase->Scale.y ;
							Scale.z += Anim->Scale.z - FrameBase->Scale.z ;
						}

						if( Anim->ValidFlag & MV1_ANIMVALUE_ROTATE )
						{
							Rotate.x += Anim->Rotate.x - FrameBase->Rotate.x ;
							Rotate.y += Anim->Rotate.y - FrameBase->Rotate.y ;
							Rotate.z += Anim->Rotate.z - FrameBase->Rotate.z ;
						}

						if( Anim->ValidFlag & ( MV1_ANIMVALUE_QUATERNION_X | MV1_ANIMVALUE_QUATERNION_VMD ) )
						{
							Quaternion.x = Anim->Quaternion.x - FrameBase->Quaternion.x ;
							Quaternion.y = Anim->Quaternion.y - FrameBase->Quaternion.y ;
							Quaternion.z = Anim->Quaternion.z - FrameBase->Quaternion.z ;
							Quaternion.w = Anim->Quaternion.w - FrameBase->Quaternion.w ;
						}
					}
					else
					{
						if( Anim->ValidFlag & MV1_ANIMVALUE_TRANSLATE )
						{
							Translate.x += ( Anim->Translate.x - FrameBase->Translate.x ) * BlendRate ;
							Translate.y += ( Anim->Translate.y - FrameBase->Translate.y ) * BlendRate ;
							Translate.z += ( Anim->Translate.z - FrameBase->Translate.z ) * BlendRate ;
						}

						if( Anim->ValidFlag & MV1_ANIMVALUE_SCALE )
						{
							Scale.x += ( Anim->Scale.x - FrameBase->Scale.x ) * BlendRate ;
							Scale.y += ( Anim->Scale.y - FrameBase->Scale.y ) * BlendRate ;
							Scale.z += ( Anim->Scale.z - FrameBase->Scale.z ) * BlendRate ;
						}

						if( Anim->ValidFlag & MV1_ANIMVALUE_ROTATE )
						{
							Rotate.x += ( Anim->Rotate.x - FrameBase->Rotate.x ) * BlendRate ;
							Rotate.y += ( Anim->Rotate.y - FrameBase->Rotate.y ) * BlendRate ;
							Rotate.z += ( Anim->Rotate.z - FrameBase->Rotate.z ) * BlendRate ;
						}

						if( Anim->ValidFlag & ( MV1_ANIMVALUE_QUATERNION_X | MV1_ANIMVALUE_QUATERNION_VMD ) )
						{
							Quaternion.x = ( Anim->Quaternion.x - FrameBase->Quaternion.x ) * BlendRate ;
							Quaternion.y = ( Anim->Quaternion.y - FrameBase->Quaternion.y ) * BlendRate ;
							Quaternion.z = ( Anim->Quaternion.z - FrameBase->Quaternion.z ) * BlendRate ;
							Quaternion.w = ( Anim->Quaternion.w - FrameBase->Quaternion.w ) * BlendRate ;
						}
					}
				}

				Translate.x += FrameBase->Translate.x ;
				Translate.y += FrameBase->Translate.y ;
				Translate.z += FrameBase->Translate.z ;

				Scale.x += FrameBase->Scale.x ;
				Scale.y += FrameBase->Scale.y ;
				Scale.z += FrameBase->Scale.z ;

				Rotate.x += FrameBase->Rotate.x ;
				Rotate.y += FrameBase->Rotate.y ;
				Rotate.z += FrameBase->Rotate.z ;

				Quaternion.x += FrameBase->Quaternion.x ;
				Quaternion.y += FrameBase->Quaternion.y ;
				Quaternion.z += FrameBase->Quaternion.z ;
				Quaternion.w += FrameBase->Quaternion.w ;

				// �s��̃Z�b�g�A�b�v
				MV1SetupTransformMatrix(
					&BlendMat,
					BlendFlag,
					&Translate,
					&Scale,
					FrameBase->RotateOrder,
					( FrameBase->Flag & MV1_FRAMEFLAG_PREROTATE ) != 0 ? &FrameBase->PreRotate : NULL,
					&Rotate,
					( FrameBase->Flag & MV1_FRAMEFLAG_POSTROTATE ) != 0 ? &FrameBase->PostRotate : NULL,
					&Quaternion
				) ;
			}

			// �߂�l�p�̍s��ɒu��������
			ConvertMatrix4x4cFToMatrixD( &ResultMatrix, &BlendMat ) ;
		}
	}

	// �߂�l�Ƃ��ĕԂ�
	return ResultMatrix ;
}

// �w��̃t���[���̃��[�J�����W���烏�[���h���W�ɕϊ�����s��𓾂�
extern MATRIX NS_MV1GetFrameLocalWorldMatrix( int MHandle, int FrameIndex )
{
	MATRIX ResultMatrix ;

	MV1FRAMESTART( MHandle, Model, ModelBase, Frame, FrameIndex, MGetIdent() ) ;

	// �s��̃Z�b�g�A�b�v
	MV1SETUPMATRIX( Model ) ;

	// �s���Ԃ�
	ConvertMatrix4x4cToMatrixF( &ResultMatrix, &Frame->LocalWorldMatrix ) ;
	return ResultMatrix ;
}

// �w��̃t���[���̃��[�J�����W���烏�[���h���W�ɕϊ�����s��𓾂�
extern MATRIX_D NS_MV1GetFrameLocalWorldMatrixD( int MHandle, int FrameIndex )
{
	MATRIX_D ResultMatrix ;

	MV1FRAMESTART( MHandle, Model, ModelBase, Frame, FrameIndex, MGetIdentD() ) ;

	// �s��̃Z�b�g�A�b�v
	MV1SETUPMATRIX( Model ) ;

	// �s���Ԃ�
	ConvertMatrix4x4cToMatrixD( &ResultMatrix, &Frame->LocalWorldMatrix ) ;
	return ResultMatrix ;
}

// �w��̃t���[���̃��[�J�����W�ϊ��s���ݒ肷��
extern int NS_MV1SetFrameUserLocalMatrix( int MHandle, int FrameIndex, MATRIX Matrix )
{
	MV1FRAMESTART( MHandle, Model, ModelBase, Frame, FrameIndex, -1 ) ;

	// �s����Z�b�g
	ConvertMatrixFToMatrix4x4c( &Frame->UserLocalTransformMatrix, &Matrix ) ;

	// �Ǝ��s����g�p����t���O�𗧂Ă�
	Frame->ValidUserLocalTransformMatrix = true ;

	// �s�񂪃Z�b�g�A�b�v����Ă��Ȃ���Ԃɂ���
	Model->LocalWorldMatrixSetupFlag = false ;
	MV1BitSetChange( &Frame->LocalWorldMatrixChange ) ;

	// �I��
	return 0 ;
}

// �w��̃t���[���̃��[�J�����W�ϊ��s���ݒ肷��
extern int NS_MV1SetFrameUserLocalMatrixD( int MHandle, int FrameIndex, MATRIX_D Matrix )
{
	MV1FRAMESTART( MHandle, Model, ModelBase, Frame, FrameIndex, -1 ) ;

	// �s����Z�b�g
	ConvertMatrixDToMatrix4x4c( &Frame->UserLocalTransformMatrix, &Matrix ) ;

	// �Ǝ��s����g�p����t���O�𗧂Ă�
	Frame->ValidUserLocalTransformMatrix = true ;

	// �s�񂪃Z�b�g�A�b�v����Ă��Ȃ���Ԃɂ���
	Model->LocalWorldMatrixSetupFlag = false ;
	MV1BitSetChange( &Frame->LocalWorldMatrixChange ) ;

	// �I��
	return 0 ;
}

// �w��̃t���[���̃��[�J�����W�ϊ��s����f�t�H���g�ɖ߂�
extern int NS_MV1ResetFrameUserLocalMatrix( int MHandle, int FrameIndex )
{
	MV1FRAMESTART( MHandle, Model, ModelBase, Frame, FrameIndex, -1 ) ;

	// ���Ƀt���O���|��Ă����牽�����Ȃ�
	if( Frame->ValidUserLocalTransformMatrix == false ) return 0 ;

	// �Ǝ��s����g�p����t���O��|��
	Frame->ValidUserLocalTransformMatrix = false ;

	// �s�񂪃Z�b�g�A�b�v����Ă��Ȃ���Ԃɂ���
	Model->LocalWorldMatrixSetupFlag = false ;
	MV1BitSetChange( &Frame->LocalWorldMatrixChange ) ;

	// �I��
	return 0 ;
}

// �w��̃t���[���������b�V�����_�̃��[�J�����W�ł̍ő�l�𓾂�
extern VECTOR NS_MV1GetFrameMaxVertexLocalPosition( int MHandle, int FrameIndex )
{
	VECTOR ErrorRet = { 0.0f, 0.0f, 0.0f } ;
	MV1FRAMESTART( MHandle, Model, ModelBase, Frame, FrameIndex, ErrorRet ) ;

	// �ő���W�l��Ԃ�
	return Frame->BaseData->MaxPosition ;
}

// �w��̃t���[���������b�V�����_�̃��[�J�����W�ł̍ő�l�𓾂�
extern VECTOR_D NS_MV1GetFrameMaxVertexLocalPositionD( int MHandle, int FrameIndex )
{
	VECTOR_D ErrorRet = { 0.0, 0.0, 0.0 } ;
	MV1FRAMESTART( MHandle, Model, ModelBase, Frame, FrameIndex, ErrorRet ) ;

	// �ő���W�l��Ԃ�
	return VConvFtoD( Frame->BaseData->MaxPosition ) ;
}

// �w��̃t���[���������b�V�����_�̃��[�J�����W�ł̍ŏ��l�𓾂�
extern VECTOR NS_MV1GetFrameMinVertexLocalPosition( int MHandle, int FrameIndex )
{
	VECTOR ErrorRet = { 0.0f, 0.0f, 0.0f } ;
	MV1FRAMESTART( MHandle, Model, ModelBase, Frame, FrameIndex, ErrorRet ) ;

	// �ŏ����W�l��Ԃ�
	return Frame->BaseData->MinPosition ;
}

// �w��̃t���[���������b�V�����_�̃��[�J�����W�ł̍ŏ��l�𓾂�
extern VECTOR_D NS_MV1GetFrameMinVertexLocalPositionD( int MHandle, int FrameIndex )
{
	VECTOR_D ErrorRet = { 0.0, 0.0, 0.0 } ;
	MV1FRAMESTART( MHandle, Model, ModelBase, Frame, FrameIndex, ErrorRet ) ;

	// �ŏ����W�l��Ԃ�
	return VConvFtoD( Frame->BaseData->MinPosition ) ;
}

// �w��̃t���[���������b�V�����_�̃��[�J�����W�ł̕��ϒl�𓾂�
extern VECTOR NS_MV1GetFrameAvgVertexLocalPosition( int MHandle, int FrameIndex )
{
	VECTOR ErrorRet = { 0.0f, 0.0f, 0.0f } ;
	MV1FRAMESTART( MHandle, Model, ModelBase, Frame, FrameIndex, ErrorRet ) ;

	// ���ύ��W�l��Ԃ�
	return VScale( VAdd( Frame->BaseData->MaxPosition, Frame->BaseData->MinPosition ), 0.5f ) ;
}

// �w��̃t���[���������b�V�����_�̃��[�J�����W�ł̕��ϒl�𓾂�
extern VECTOR_D NS_MV1GetFrameAvgVertexLocalPositionD( int MHandle, int FrameIndex )
{
	VECTOR_D ErrorRet = { 0.0, 0.0, 0.0 } ;
	MV1FRAMESTART( MHandle, Model, ModelBase, Frame, FrameIndex, ErrorRet ) ;

	// ���ύ��W�l��Ԃ�
	return VConvFtoD( VScale( VAdd( Frame->BaseData->MaxPosition, Frame->BaseData->MinPosition ), 0.5f ) ) ;
}

// �w��̃t���[���Ɋ܂܂��|���S���̐����擾����
extern int NS_MV1GetFrameTriangleNum( int MHandle, int FrameIndex )
{
	MV1FRAMESTART( MHandle, Model, ModelBase, Frame, FrameIndex, -1 ) ;

	// �|���S���̐���Ԃ�
	return Frame->BaseData->TriangleNum ;
}

// �w��̃t���[���������b�V���̐����擾����
extern int NS_MV1GetFrameMeshNum( int MHandle, int FrameIndex )
{
	MV1FRAMESTART( MHandle, Model, ModelBase, Frame, FrameIndex, -1 ) ;

	// ���b�V���̐���Ԃ�
	return Frame->BaseData->MeshNum ;
}

// �w��̃t���[���������b�V���̃��b�V���C���f�b�N�X���擾����
extern int NS_MV1GetFrameMesh( int MHandle, int FrameIndex, int Index )
{
	MV1_MESH_BASE *Mesh ;
	MV1FRAMESTART( MHandle, Model, ModelBase, Frame, FrameIndex, -1 ) ;

	// ���b�V���C���f�b�N�X���s����������G���[
	if( Index < 0 || Index >= Frame->BaseData->MeshNum )
		return -1 ;
	Mesh = &Frame->BaseData->Mesh[ Index ] ;

	// ���b�V���̃C���f�b�N�X��Ԃ�
	return ( int )( Mesh - ModelBase->Mesh ) ;
}

// �w��̃t���[���̕\���A��\����ύX����( TRUE:�\��  FALSE:��\�� )
extern int NS_MV1SetFrameVisible( int MHandle, int FrameIndex, int VisibleFlag )
{
	MV1FRAMESTART( MHandle, Model, ModelBase, Frame, FrameIndex, -1 ) ;

	// �l���Z�b�g
	if( MV1SetDrawMaterialVisible( &Frame->DrawMaterial, &Frame->DrawMaterialChange, ( BYTE )VisibleFlag ) )
	{
		// �`��ҋ@���Ă���`�敨��`��
		DRAWSTOCKINFO
	}

	// �I��
	return 0 ;
}

// �w��̃t���[���̕\���A��\����Ԃ��擾����( TRUE:�\��  FALSE:��\�� )
extern int NS_MV1GetFrameVisible( int MHandle, int FrameIndex )
{
	MV1FRAMESTART( MHandle, Model, ModelBase, Frame, FrameIndex, -1 ) ;

	// �t���O��Ԃ�
	return Frame->DrawMaterial.Visible ;
}


// �w��̃t���[���̃f�B�t���[�Y�J���[�̃X�P�[���l��ݒ肷��( �f�t�H���g�l�� 1.0f )
extern	int			NS_MV1SetFrameDifColorScale( int MHandle, int FrameIndex, COLOR_F Scale )
{
	MV1FRAMESTART( MHandle, Model, ModelBase, Frame, FrameIndex, -1 ) ;

	// �l���Z�b�g
	if( MV1SetDrawMaterialDif( &Frame->DrawMaterial, &Frame->DrawMaterialChange, Scale ) )
	{
		// �`��ҋ@���Ă���`�敨��`��
		DRAWSTOCKINFO

		// �������v�f�L�����̃Z�b�g�A�b�v�����t���O��|��
		MV1FRAME_RESET_SEMITRANSSETUP( Frame ) ;
	}

	// �I��
	return 0 ;
}

// �w��̃t���[���̃X�y�L�����J���[�̃X�P�[���l��ݒ肷��( �f�t�H���g�l�� 1.0f )
extern	int			NS_MV1SetFrameSpcColorScale( int MHandle, int FrameIndex, COLOR_F Scale )
{
	MV1FRAMESTART( MHandle, Model, ModelBase, Frame, FrameIndex, -1 ) ;

	// �l���Z�b�g
	if( MV1SetDrawMaterialSpc( &Frame->DrawMaterial, &Frame->DrawMaterialChange, Scale ) )
	{
		// �`��ҋ@���Ă���`�敨��`��
		DRAWSTOCKINFO
	}

	// �I��
	return 0 ;
}

// �w��̃t���[���̃G�~�b�V�u�J���[�̃X�P�[���l��ݒ肷��( �f�t�H���g�l�� 1.0f )
extern	int			NS_MV1SetFrameEmiColorScale( int MHandle, int FrameIndex, COLOR_F Scale )
{
	MV1FRAMESTART( MHandle, Model, ModelBase, Frame, FrameIndex, -1 ) ;

	// �l���Z�b�g
	if( MV1SetDrawMaterialEmi( &Frame->DrawMaterial, &Frame->DrawMaterialChange, Scale ) )
	{
		// �`��ҋ@���Ă���`�敨��`��
		DRAWSTOCKINFO
	}

	// �I��
	return 0 ;
}

// �w��̃t���[���̃A���r�G���g�J���[�̃X�P�[���l��ݒ肷��( �f�t�H���g�l�� 1.0f )
extern	int			NS_MV1SetFrameAmbColorScale( int MHandle, int FrameIndex, COLOR_F Scale )
{
	MV1FRAMESTART( MHandle, Model, ModelBase, Frame, FrameIndex, -1 ) ;

	// �l���Z�b�g
	if( MV1SetDrawMaterialAmb( &Frame->DrawMaterial, &Frame->DrawMaterialChange, Scale ) )
	{
		// �`��ҋ@���Ă���`�敨��`��
		DRAWSTOCKINFO
	}

	// �I��
	return 0 ;
}

// �w��̃t���[���̃f�B�t���[�Y�J���[�̃X�P�[���l���擾����( �f�t�H���g�l�� 1.0f )
extern	COLOR_F		NS_MV1GetFrameDifColorScale( int MHandle, int FrameIndex )
{
	COLOR_F ErrorResult = { -1.0f, -1.0f, -1.0f, -1.0f } ;
	MV1FRAMESTART( MHandle, Model, ModelBase, Frame, FrameIndex, ErrorResult ) ;

	// �l��Ԃ�
	return Frame->DrawMaterial.DiffuseScale ;
}

// �w��̃t���[���̃X�y�L�����J���[�̃X�P�[���l���擾����( �f�t�H���g�l�� 1.0f )
extern	COLOR_F		NS_MV1GetFrameSpcColorScale( int MHandle, int FrameIndex )
{
	COLOR_F ErrorResult = { -1.0f, -1.0f, -1.0f, -1.0f } ;
	MV1FRAMESTART( MHandle, Model, ModelBase, Frame, FrameIndex, ErrorResult ) ;

	// �l��Ԃ�
	return Frame->DrawMaterial.SpecularScale ;
}

// �w��̃t���[���̃G�~�b�V�u�J���[�̃X�P�[���l���擾����( �f�t�H���g�l�� 1.0f )
extern	COLOR_F		NS_MV1GetFrameEmiColorScale( int MHandle, int FrameIndex )
{
	COLOR_F ErrorResult = { -1.0f, -1.0f, -1.0f, -1.0f } ;
	MV1FRAMESTART( MHandle, Model, ModelBase, Frame, FrameIndex, ErrorResult ) ;

	// �l��Ԃ�
	return Frame->DrawMaterial.EmissiveScale ;
}

// �w��̃t���[���̃A���r�G���g�J���[�̃X�P�[���l���擾����( �f�t�H���g�l�� 1.0f )
extern	COLOR_F		NS_MV1GetFrameAmbColorScale( int MHandle, int FrameIndex )
{
	COLOR_F ErrorResult = { -1.0f, -1.0f, -1.0f, -1.0f } ;
	MV1FRAMESTART( MHandle, Model, ModelBase, Frame, FrameIndex, ErrorResult ) ;

	// �l��Ԃ�
	return Frame->DrawMaterial.AmbientScale ;
}

// �w��̃t���[���ɔ������v�f�����邩�ǂ������擾����( �߂�l TRUE:����  FALSE:�Ȃ� )
extern int NS_MV1GetFrameSemiTransState( int MHandle, int FrameIndex )
{
	int i, MeshIndex ;
	MV1FRAMESTART( MHandle, Model, ModelBase, Frame, FrameIndex, -1 ) ;

	// �`��}�e���A���̍X�V
	MV1SETUPDRAWMATERIALFRAME( Frame ) ;

	// �Z�b�g�A�b�v���������Ă��Ȃ��ꍇ�̓Z�b�g�A�b�v���s��
	if( Frame->SemiTransStateSetupFlag == false )
	{
		// ���ʃt���[���A���b�V���̔������v�f�����邩�ǂ����𒲂ׁA
		// �ǂꂩ��ł��������v�f������΂��̃t���[�����������v�f������Ƃ������ƂɂȂ�
		Frame->SemiTransState = false ;

		// �ŏ��Ƀ��b�V������
		MeshIndex = ( int )( Frame->Mesh - Model->Mesh ) ;
		for( i = 0 ; i < Frame->BaseData->MeshNum ; i ++, MeshIndex ++ )
		{
			if( NS_MV1GetMeshSemiTransState( MHandle, MeshIndex ) )
			{
				Frame->SemiTransState = true ;
			}
		}

		// ���Ɏq�t���[��
		for( i = 0 ; i < Frame->ChildNum ; i ++ )
		{
			if( NS_MV1GetFrameSemiTransState( MHandle, ( int )( Frame->ChildList[ i ] - Model->Frame ) ) )
				Frame->SemiTransState = true ;
		}

		// �Z�b�g�A�b�v�����t���O�𗧂Ă�
		Frame->SemiTransStateSetupFlag = true ;
	}

	// �������v�f�`�F�b�N�t���O��Ԃ�
	return Frame->SemiTransState ;
}


// �w��̃t���[���̕s�����x��ݒ肷��( �s���� 1.0f �` ���� 0.0f )
extern	int			NS_MV1SetFrameOpacityRate( int MHandle, int FrameIndex, float Rate )
{
	MV1FRAMESTART( MHandle, Model, ModelBase, Frame, FrameIndex, -1 ) ;

	// �l���Z�b�g
	if( MV1SetDrawMaterialOpacityRate( &Frame->DrawMaterial, &Frame->DrawMaterialChange, Rate ) )
	{
		// �`��ҋ@���Ă���`�敨��`��
		DRAWSTOCKINFO

		// �������v�f�L�����̃Z�b�g�A�b�v�����t���O��|��
		MV1FRAME_RESET_SEMITRANSSETUP( Frame ) ;
	}

	// �I��
	return 0 ;
}

// �w��̃t���[���̕s�����x���擾����( �s���� 1.0f �` ���� 0.0f )
extern	float		NS_MV1GetFrameOpacityRate( int MHandle, int FrameIndex )
{
	MV1FRAMESTART( MHandle, Model, ModelBase, Frame, FrameIndex, -1.0f ) ;

	// �s�����x��Ԃ�
	return Frame->DrawMaterial.OpacityRate ;
}

// �w��̃t���[���̏����\����Ԃ�ݒ肷��
extern int NS_MV1SetFrameBaseVisible( int MHandle, int FrameIndex, int VisibleFlag )
{
	MV1FRAMESTART( MHandle, Model, ModelBase, Frame, FrameIndex, -1 ) ;

	// �����\����Ԃ�ݒ肷��
	if( VisibleFlag )
	{
		if( Frame->BaseData->Flag & MV1_FRAMEFLAG_VISIBLE )
		{
			return 0 ;
		}

		// �`��ҋ@���Ă���`�敨��`��
		DRAWSTOCKINFO

		Frame->BaseData->Flag |= MV1_FRAMEFLAG_VISIBLE ;
	}
	else
	{
		if( ( Frame->BaseData->Flag & MV1_FRAMEFLAG_VISIBLE ) == 0 )
		{
			return 0 ;
		}

		// �`��ҋ@���Ă���`�敨��`��
		DRAWSTOCKINFO

		Frame->BaseData->Flag &= ~MV1_FRAMEFLAG_VISIBLE ;
	}

	// �I��
	return 0 ;
}

// �w��̃t���[���̏����\����Ԃ��擾����( �߂�l  TRUE:�\��   FALSE:��\�� )
extern int NS_MV1GetFrameBaseVisible( int MHandle, int FrameIndex )
{
	MV1FRAMESTART( MHandle, Model, ModelBase, Frame, FrameIndex, -1 ) ;

	// �����\����Ԃ�Ԃ�
	return ( Frame->BaseData->Flag & MV1_FRAMEFLAG_VISIBLE ) != 0 ? TRUE : FALSE ;
}

// �w��̃t���[���̃e�N�X�`�����W�ϊ��p�����[�^��ݒ肷��
extern int NS_MV1SetFrameTextureAddressTransform( int MHandle, int FrameIndex, float TransU, float TransV, float ScaleU, float ScaleV, float RotCenterU, float RotCenterV, float Rotate )
{
	MATRIX Transform, Temp1, Temp2, Temp3 ;
	int UseFlag ;

	MV1FRAMESTART( MHandle, Model, ModelBase, Frame, FrameIndex, -1 ) ;

	UseFlag = FALSE ;

	if( Rotate != 0.0f )
	{
		CreateIdentityMatrix( &Temp1 ) ;
		Temp1.m[ 2 ][ 0 ] = -RotCenterU ;
		Temp1.m[ 2 ][ 1 ] = -RotCenterV ;
//		CreateTranslationMatrix( &Temp1, -RotCenterU, -RotCenterV, 0.0f ) ; 
		CreateRotationZMatrix( &Temp2, Rotate ) ;
//		CreateTranslationMatrix( &Temp3,  RotCenterU,  RotCenterV, 0.0f ) ; 
		CreateIdentityMatrix( &Temp3 ) ;
		Temp3.m[ 2 ][ 0 ] = RotCenterU ;
		Temp3.m[ 2 ][ 1 ] = RotCenterV ;
		CreateMultiplyMatrix( &Transform, &Temp1, &Temp2 ) ;
		CreateMultiplyMatrix( &Transform, &Transform, &Temp3 ) ;
		UseFlag = TRUE ;
	}
	else
	{
		CreateIdentityMatrix( &Transform ) ;
	}

	if( TransU != 0.0f || TransV != 0.0f )
	{
//		CreateTranslationMatrix( &Temp1, TransU, TransV, 0.0f ) ;
		CreateIdentityMatrix( &Temp1 ) ;
		Temp1.m[ 2 ][ 0 ] = TransU ;
		Temp1.m[ 2 ][ 1 ] = TransV ;
		CreateMultiplyMatrix( &Transform, &Transform, &Temp1 ) ;
		UseFlag = TRUE ;
	}

	if( ScaleU != 1.0f || ScaleV != 1.0f )
	{
		CreateScalingMatrix( &Temp1, ScaleU, ScaleV, 1.0f ) ;
		CreateMultiplyMatrix( &Transform, &Transform, &Temp1 ) ;
		UseFlag = TRUE ;
	}

	if( UseFlag || Frame->TextureAddressTransformUse )
	{
		// �`��ҋ@���Ă���`�敨��`��
		DRAWSTOCKINFO
	}

	Frame->TextureAddressTransformUse = UseFlag ;
	Frame->TextureAddressTransformMatrix = Transform ;

	// �I��
	return 0 ;
}

// �w��̃t���[���̃e�N�X�`�����W�ϊ��s����Z�b�g����
extern int NS_MV1SetFrameTextureAddressTransformMatrix( int MHandle, int FrameIndex, MATRIX Matrix )
{
	MV1FRAMESTART( MHandle, Model, ModelBase, Frame, FrameIndex, -1 ) ;

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	Frame->TextureAddressTransformUse = TRUE ;
	Frame->TextureAddressTransformMatrix = Matrix ;

	// �I��
	return 0 ;
}

// �w��̃t���[���̃e�N�X�`�����W�ϊ��p�����[�^�����Z�b�g����
extern int NS_MV1ResetFrameTextureAddressTransform( int MHandle, int FrameIndex )
{
	MV1FRAMESTART( MHandle, Model, ModelBase, Frame, FrameIndex, -1 ) ;

	if( Frame->TextureAddressTransformUse )
	{
		// �`��ҋ@���Ă���`�敨��`��
		DRAWSTOCKINFO
	}

	Frame->TextureAddressTransformUse = FALSE ;
	CreateIdentityMatrix( &Frame->TextureAddressTransformMatrix ) ;

	// �I��
	return 0 ;
}

// ���f���Ɋ܂܂�郁�b�V���̐����擾����
extern int NS_MV1GetMeshNum( int MHandle )
{
	MV1_MODEL *Model ;
	MV1_MODEL_BASE *ModelBase ;

	// ����������Ă��Ȃ�������G���[
	if( MV1Man.Initialize == false )
		return -1 ;

	// �A�h���X�擾
	if( MV1MDLCHK( MHandle, Model ) )
		return -1 ;
	ModelBase = Model->BaseData ;

	// ���b�V���̐���Ԃ�
	return ModelBase->MeshNum ;
}




// �w�胁�b�V�����g�p���Ă���}�e���A���̃C���f�b�N�X���擾����
extern int NS_MV1GetMeshMaterial( int MHandle, int MeshIndex )
{
	MV1MESHSTART( MHandle, Model, ModelBase, Mesh, MeshIndex, -1 ) ;

	// �g�p���Ă���}�e���A���̃C���f�b�N�X��Ԃ�
	return ( int )( Mesh->BaseData->Material - ModelBase->Material ) ;
}

// �w�胁�b�V���Ɋ܂܂��O�p�`�|���S���̐����擾����
extern int NS_MV1GetMeshTriangleNum( int MHandle, int MeshIndex )
{
	MV1MESHSTART( MHandle, Model, ModelBase, Mesh, MeshIndex, -1 ) ;
	MV1_MESH_BASE *MeshBase ;
	int i, Num ;

	// �܂܂��O�p�`�|���S���̐���Ԃ�
	MeshBase = Mesh->BaseData ;
	Num = 0 ;
	for( i = 0 ; i < MeshBase->TriangleListNum ; i ++ )
		Num += MeshBase->TriangleList[ i ].IndexNum / 3 ;
	return Num ;
}

// �w�胁�b�V���̕\���A��\����Ԃ�ύX����( TRUE:�\��  FALSE:��\�� )
extern int NS_MV1SetMeshVisible( int MHandle, int MeshIndex, int VisibleFlag )
{
	MV1MESHSTART( MHandle, Model, ModelBase, Mesh, MeshIndex, -1 ) ;

	// �l���Z�b�g
	if( MV1SetDrawMaterialVisible( &Mesh->DrawMaterial, &Mesh->DrawMaterialChange, ( BYTE )VisibleFlag ) )
	{
		// �`��ҋ@���Ă���`�敨��`��
		DRAWSTOCKINFO
	}

	// �I��
	return 0 ;
}

// �w�胁�b�V���̕\���A��\����Ԃ��擾����( TRUE:�\��  FALSE:��\�� )
extern int NS_MV1GetMeshVisible( int MHandle, int MeshIndex )
{
	MV1MESHSTART( MHandle, Model, ModelBase, Mesh, MeshIndex, -1 ) ;

	// �t���O��Ԃ�
	return Mesh->DrawMaterial.Visible ;
}

// �w��̃��b�V���̃f�B�t���[�Y�J���[�̃X�P�[���l��ݒ肷��( �f�t�H���g�l�� 1.0f )
extern	int			NS_MV1SetMeshDifColorScale( int MHandle, int MeshIndex, COLOR_F Scale )
{
	MV1MESHSTART( MHandle, Model, ModelBase, Mesh, MeshIndex, -1 ) ;

	// �l���Z�b�g
	if( MV1SetDrawMaterialDif( &Mesh->DrawMaterial, &Mesh->DrawMaterialChange, Scale ) )
	{
		// �`��ҋ@���Ă���`�敨��`��
		DRAWSTOCKINFO

		// �������X�e�[�^�X�Z�b�g�A�b�v�����t���O�������Ă�����|��
		MV1MESH_RESET_SEMITRANSSETUP( Mesh )
	}

	// �I��
	return 0 ;
}

// �w��̃��b�V���̃X�y�L�����J���[�̃X�P�[���l��ݒ肷��( �f�t�H���g�l�� 1.0f )
extern	int			NS_MV1SetMeshSpcColorScale( int MHandle, int MeshIndex, COLOR_F Scale )
{
	MV1MESHSTART( MHandle, Model, ModelBase, Mesh, MeshIndex, -1 ) ;

	// �l���Z�b�g
	if( MV1SetDrawMaterialSpc( &Mesh->DrawMaterial, &Mesh->DrawMaterialChange, Scale ) )
	{
		// �`��ҋ@���Ă���`�敨��`��
		DRAWSTOCKINFO
	}

	// �I��
	return 0 ;
}

// �w��̃��b�V���̃G�~�b�V�u�J���[�̃X�P�[���l��ݒ肷��( �f�t�H���g�l�� 1.0f )
extern	int			NS_MV1SetMeshEmiColorScale( int MHandle, int MeshIndex, COLOR_F Scale )
{
	MV1MESHSTART( MHandle, Model, ModelBase, Mesh, MeshIndex, -1 ) ;

	// �l���Z�b�g
	if( MV1SetDrawMaterialEmi( &Mesh->DrawMaterial, &Mesh->DrawMaterialChange, Scale ) )
	{
		// �`��ҋ@���Ă���`�敨��`��
		DRAWSTOCKINFO
	}

	// �I��
	return 0 ;
}

// �w��̃��b�V���̃A���r�G���g�J���[�̃X�P�[���l��ݒ肷��( �f�t�H���g�l�� 1.0f )
extern	int			NS_MV1SetMeshAmbColorScale( int MHandle, int MeshIndex, COLOR_F Scale )
{
	MV1MESHSTART( MHandle, Model, ModelBase, Mesh, MeshIndex, -1 ) ;

	// �l���Z�b�g
	if( MV1SetDrawMaterialAmb( &Mesh->DrawMaterial, &Mesh->DrawMaterialChange, Scale ) )
	{
		// �`��ҋ@���Ă���`�敨��`��
		DRAWSTOCKINFO
	}

	// �I��
	return 0 ;
}

// �w��̃��b�V���̃f�B�t���[�Y�J���[�̃X�P�[���l���擾����( �f�t�H���g�l�� 1.0f )
extern	COLOR_F		NS_MV1GetMeshDifColorScale( int MHandle, int MeshIndex )
{
	COLOR_F ErrorResult = { -1.0f, -1.0f, -1.0f, -1.0f } ;
	MV1MESHSTART( MHandle, Model, ModelBase, Mesh, MeshIndex, ErrorResult ) ;

	// �l��Ԃ�
	return Mesh->DrawMaterial.DiffuseScale ;
}

// �w��̃��b�V���̃X�y�L�����J���[�̃X�P�[���l���擾����( �f�t�H���g�l�� 1.0f )
extern	COLOR_F		NS_MV1GetMeshSpcColorScale( int MHandle, int MeshIndex )
{
	COLOR_F ErrorResult = { -1.0f, -1.0f, -1.0f, -1.0f } ;
	MV1MESHSTART( MHandle, Model, ModelBase, Mesh, MeshIndex, ErrorResult ) ;

	// �l��Ԃ�
	return Mesh->DrawMaterial.SpecularScale ;
}

// �w��̃��b�V���̃G�~�b�V�u�J���[�̃X�P�[���l���擾����( �f�t�H���g�l�� 1.0f )
extern	COLOR_F		NS_MV1GetMeshEmiColorScale( int MHandle, int MeshIndex )
{
	COLOR_F ErrorResult = { -1.0f, -1.0f, -1.0f, -1.0f } ;
	MV1MESHSTART( MHandle, Model, ModelBase, Mesh, MeshIndex, ErrorResult ) ;

	// �l��Ԃ�
	return Mesh->DrawMaterial.EmissiveScale ;
}

// �w��̃��b�V���̃A���r�G���g�J���[�̃X�P�[���l���擾����( �f�t�H���g�l�� 1.0f )
extern	COLOR_F		NS_MV1GetMeshAmbColorScale( int MHandle, int MeshIndex )
{
	COLOR_F ErrorResult = { -1.0f, -1.0f, -1.0f, -1.0f } ;
	MV1MESHSTART( MHandle, Model, ModelBase, Mesh, MeshIndex, ErrorResult ) ;

	// �l��Ԃ�
	return Mesh->DrawMaterial.AmbientScale ;
}

// �w��̃��b�V���̕s�����x��ݒ肷��( �s���� 1.0f �` ���� 0.0f )
extern	int			NS_MV1SetMeshOpacityRate( int MHandle, int MeshIndex, float Rate )
{
	MV1MESHSTART( MHandle, Model, ModelBase, Mesh, MeshIndex, -1 ) ;

	// �l���Z�b�g
	if( MV1SetDrawMaterialOpacityRate( &Mesh->DrawMaterial, &Mesh->DrawMaterialChange, Rate ) )
	{
		// �`��ҋ@���Ă���`�敨��`��
		DRAWSTOCKINFO

		// �������X�e�[�^�X�Z�b�g�A�b�v�����t���O�������Ă�����|��
		MV1MESH_RESET_SEMITRANSSETUP( Mesh )
	}

	// �I��
	return 0 ;
}


// �w��̃��b�V���̕s�����x���擾����( �s���� 1.0f �` ���� 0.0f )
extern	float		NS_MV1GetMeshOpacityRate( int MHandle, int MeshIndex )
{
	MV1MESHSTART( MHandle, Model, ModelBase, Mesh, MeshIndex, -1.0f ) ;

	// �s�����x��Ԃ�
	return Mesh->DrawMaterial.OpacityRate ;
}

// �w��̃��b�V���̕`��u�����h���[�h��ݒ肷��( DX_BLENDMODE_ALPHA �� )
extern	int			NS_MV1SetMeshDrawBlendMode( int MHandle, int MeshIndex, int BlendMode )
{
	MV1MESHSTART( MHandle, Model, ModelBase, Mesh, MeshIndex, -1 ) ;

	// ���܂łƃt���O�������������牽�������ɏI��
	if( Mesh->DrawBlendMode == BlendMode )
		return 0 ;

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �u�����h���[�h��ݒ肷��
	Mesh->DrawBlendMode = BlendMode ;

	// �������X�e�[�^�X�Z�b�g�A�b�v�����t���O�������Ă�����|��
	MV1MESH_RESET_SEMITRANSSETUP( Mesh )

	// �}�e���A�������X�V
	if( MV1CCHK( Mesh->DrawMaterialChange ) == 0 )
		MV1BitSetChange( &Mesh->DrawMaterialChange ) ;

	// �I��
	return 0 ;
}

// �w��̃��b�V���̕`��u�����h�p�����[�^��ݒ肷��
extern int NS_MV1SetMeshDrawBlendParam( int MHandle, int MeshIndex, int BlendParam )
{
	MV1MESHSTART( MHandle, Model, ModelBase, Mesh, MeshIndex, -1 ) ;

	// ���܂łƃp�����[�^�������ꍇ�͉��������ɏI��
	if( Mesh->DrawBlendParam == BlendParam )
		return 0 ;

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �u�����h�p�����[�^��ݒ肷��
	Mesh->DrawBlendParam = BlendParam ;

	// �������X�e�[�^�X�Z�b�g�A�b�v�����t���O�������Ă�����|��
	MV1MESH_RESET_SEMITRANSSETUP( Mesh )

	// �}�e���A�������X�V
	if( MV1CCHK( Mesh->DrawMaterialChange ) == 0 )
		MV1BitSetChange( &Mesh->DrawMaterialChange ) ;

	// �I��
	return 0 ;
}

// �w��̃��b�V���̕`��u�����h���[�h���擾����( DX_BLENDMODE_ALPHA �� )
extern int NS_MV1GetMeshDrawBlendMode( int MHandle, int MeshIndex )
{
	MV1MESHSTART( MHandle, Model, ModelBase, Mesh, MeshIndex, -1 ) ;

	// �u�����h���[�h��Ԃ�
	return Mesh->DrawBlendMode ;
}

// �w��̃��b�V���̕`��u�����h�p�����[�^��ݒ肷��
extern int NS_MV1GetMeshDrawBlendParam( int MHandle, int MeshIndex )
{
	MV1MESHSTART( MHandle, Model, ModelBase, Mesh, MeshIndex, -1 ) ;

	// �u�����h�p�����[�^��Ԃ�
	return Mesh->DrawBlendParam ;
}

// �w��̃��b�V���̏����\����Ԃ�ݒ肷��( TRUE:�\��  FALSE:��\�� )
extern int NS_MV1SetMeshBaseVisible( int MHandle, int MeshIndex, int VisibleFlag )
{
	MV1MESHSTART( MHandle, Model, ModelBase, Mesh, MeshIndex, -1 ) ;

	VisibleFlag = VisibleFlag != 0 ? 1 : 0 ;

	if( VisibleFlag == Mesh->BaseData->Visible )
	{
		return 0 ;
	}

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �p�����[�^��ݒ肷��
	Mesh->BaseData->Visible = ( BYTE )VisibleFlag ;

	// �I��
	return 0 ;
}

// �w��̃��b�V���̏����\����Ԃ��擾����( TRUE:�\��  FALSE:��\�� )
extern int NS_MV1GetMeshBaseVisible( int MHandle, int MeshIndex )
{
	MV1MESHSTART( MHandle, Model, ModelBase, Mesh, MeshIndex, -1 ) ;

	// �p�����[�^��Ԃ�
	return Mesh->BaseData->Visible ;
}

// �w��̃��b�V���̃o�b�N�J�����O���s�����ǂ�����ݒ肷��( DX_CULLING_LEFT �� )
extern int NS_MV1SetMeshBackCulling( int MHandle, int MeshIndex, int CullingFlag )
{
	MV1MESHSTART( MHandle, Model, ModelBase, Mesh, MeshIndex, -1 ) ;

	if( Mesh->BaseData->BackCulling == CullingFlag )
	{
		return 0 ;
	}

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �p�����[�^��ݒ肷��
	Mesh->BaseData->BackCulling = ( BYTE )CullingFlag ;

	// �I��
	return 0 ;
}

// �w��̃��b�V���̃o�b�N�J�����O���s�����ǂ������擾����( DX_CULLING_LEFT �� )
extern int NS_MV1GetMeshBackCulling( int MHandle, int MeshIndex )
{
	MV1MESHSTART( MHandle, Model, ModelBase, Mesh, MeshIndex, -1 ) ;

	// �p�����[�^��Ԃ�
	return Mesh->BaseData->BackCulling ;
}

// �w��̃��b�V���Ɋ܂܂��|���S���̍ő働�[�J�����W���擾����
extern VECTOR NS_MV1GetMeshMaxPosition( int MHandle, int MeshIndex )
{
	VECTOR ErrorValue = { -1.0f, -1.0f, -1.0f } ;
	MV1MESHSTART( MHandle, Model, ModelBase, Mesh, MeshIndex, ErrorValue ) ;

	// �p�����[�^��Ԃ�
	return Mesh->BaseData->MaxPosition ;
}

// �w��̃��b�V���Ɋ܂܂��|���S���̍ŏ����[�J�����W���擾����
extern VECTOR NS_MV1GetMeshMinPosition( int MHandle, int MeshIndex )
{
	VECTOR ErrorValue = { -1.0f, -1.0f, -1.0f } ;
	MV1MESHSTART( MHandle, Model, ModelBase, Mesh, MeshIndex, ErrorValue ) ;

	// �p�����[�^��Ԃ�
	return Mesh->BaseData->MinPosition ;
}

// �w��̃��b�V���Ɋ܂܂��g���C�A���O�����X�g�̐����擾����
extern int NS_MV1GetMeshTListNum( int MHandle, int MeshIndex )
{
	MV1MESHSTART( MHandle, Model, ModelBase, Mesh, MeshIndex, -1 ) ;

	// �g���C�A���O�����X�g�̐���Ԃ�
	return Mesh->BaseData->TriangleListNum ;
}

// �w��̃��b�V�����������v�f�����邩�ǂ������擾����( �߂�l TRUE:����  FALSE:�Ȃ� )
extern int NS_MV1GetMeshSemiTransState( int MHandle, int MeshIndex )
{
	MV1MESHSTART( MHandle, Model, ModelBase, Mesh, MeshIndex, -1 ) ;
	MV1_MATERIAL *Material ;
	MV1_TEXTURE *Texture ;
	MV1_TEXTURE_BASE *TextureBase ;

	Material = Mesh->Material ;

	// �`��}�e���A���̍X�V�`�F�b�N
	MV1SETUPDRAWMATERIALMESH( Mesh ) ;

	// SemiTransStateSetupFlag �������Ă������ꍇ�͔������v�f�����邩�ǂ����𒲂ׂ�
	if( Mesh->SemiTransStateSetupFlag == false )
	{
		do
		{
			Mesh->SemiTransState = true ;

			// �`�惂�[�h���A���t�@�u�����h�ȊO�Ȃ甼����
			if( Mesh->DrawBlendMode != -1 )
			{
				if( Mesh->DrawBlendMode != DX_BLENDMODE_ALPHA )
					break ;
			}
			else
			{
				if( Material->DrawBlendMode != DX_BLENDMODE_ALPHA )
					break ;
			}

			// �u�����h�p�����[�^�� 255 �ȊO�Ȃ甼����
			if( Mesh->DrawBlendParam != -1 )
			{
				if( Mesh->DrawBlendParam != 255 )
					break ;
			}
			else
			{
				if( Material->DrawBlendParam != 255 )
					break ;
			}

			// ���_�J���[���g�p���邩�ǂ����ŕ���
			if( Mesh->BaseData->UseVertexDiffuseColor )
			{
				// ���_�f�B�t���[�Y�J���[�� 1.0 �ȊO�̃A���t�@�l���������甼����
				if( Mesh->BaseData->NotOneDiffuseAlpha )
					break ;
			}
			else
			{
				// �}�e���A���̃f�B�t���[�Y�J���[�̃A���t�@�l���P�D�O�ȊO�Ȃ甼����
				if( Material->Diffuse.a != 1.0f || ( Mesh->SetupDrawMaterial.UseColorScale && Mesh->SetupDrawMaterial.DiffuseScale.a != 1.0f ) )
					break ;
			}

			// ���b�V���̕s�����x�ݒ肪 1.0f �ȊO�Ȃ甼����
			if( Mesh->SetupDrawMaterial.OpacityRate != 1.0f )
				break ;

			// �������e�N�X�`�����g�p���Ă����甼����
			if( Material->DiffuseLayerNum )
			{
				Texture = &Model->Texture[ Material->DiffuseLayer[ 0 ].Texture ] ;
				TextureBase = Texture->BaseData ;

				if( Texture->UseUserGraphHandle )
				{
					if( Texture->UserGraphHandleSemiTransFlag )
						break ;
				}
				else
				if( Texture->UseGraphHandle )
				{
					if( Texture->SemiTransFlag )
						break ;
				}
				else
				if( TextureBase->UseUserGraphHandle )
				{
					if( TextureBase->UserGraphHandleSemiTransFlag )
						break ;
				}
				else
				if( TextureBase->SemiTransFlag )
				{
					break ;
				}
			}

			// �����ɂ����甼�����v�f�͂Ȃ��Ƃ�������
			Mesh->SemiTransState = false ;
		}while( 0 ) ;

		// �Z�b�g�A�b�v�����t���O�𗧂Ă�
		Mesh->SemiTransStateSetupFlag = true ;
	}

	// �X�e�[�^�X��Ԃ�
	return Mesh->SemiTransState ;
}

// �w��̃��b�V���Ɋ܂܂��g���C�A���O�����X�g�̃C���f�b�N�X���擾����
extern int NS_MV1GetMeshTList( int MHandle, int MeshIndex, int Index )
{
	MV1MESHSTART( MHandle, Model, ModelBase, Mesh, MeshIndex, -1 ) ;

	// �C���f�b�N�X���s����������G���[
	if( Index < 0 || Index >= Mesh->BaseData->TriangleListNum )
		return -1 ;

	// �g���C�A���O�����X�g�̃C���f�b�N�X��Ԃ�
	return ( int )( &Mesh->BaseData->TriangleList[ Index ] - ModelBase->TriangleList ) ;
}

// �w��̃��b�V���̒��_�f�B�t���[�Y�J���[���}�e���A���̃f�B�t���[�Y�J���[�̑���Ɏg�p���邩�ǂ�����ݒ肷��( TRUE:�}�e���A���J���[�̑���Ɏg�p����  FALSE:�}�e���A���J���[���g�p���� )
extern int NS_MV1SetMeshUseVertDifColor( int MHandle, int MeshIndex, int UseFlag )
{
	return MV1SetMeshUseVertDifColorBase( MV1GetModelBaseHandle( MHandle ), MeshIndex, UseFlag ) ;
}

// �w��̃��b�V���̒��_�X�y�L�����J���[���}�e���A���̃X�y�L�����J���[�̑���Ɏg�p���邩�ǂ�����ݒ肷��( TRUE:�}�e���A���J���[�̑���Ɏg�p����  FALSE:�}�e���A���J���[���g�p���� )
extern int NS_MV1SetMeshUseVertSpcColor( int MHandle, int MeshIndex, int UseFlag )
{
	return MV1SetMeshUseVertSpcColorBase( MV1GetModelBaseHandle( MHandle ), MeshIndex, UseFlag ) ;
}

// �w��̃��b�V���̒��_�f�B�t���[�Y�J���[���}�e���A���̃f�B�t���[�Y�J���[�̑���Ɏg�p���邩�ǂ����̐ݒ���擾����( �߂�l  TRUE:�}�e���A���J���[�̑���Ɏg�p����  FALSE:�}�e���A���J���[���g�p���� )
extern int NS_MV1GetMeshUseVertDifColor( int MHandle, int MeshIndex )
{
	return MV1GetMeshUseVertDifColorBase( MV1GetModelBaseHandle( MHandle ), MeshIndex ) ;
}

// �w��̃��b�V���̒��_�X�y�L�����J���[���}�e���A���̃X�y�L�����J���[�̑���Ɏg�p���邩�ǂ����̐ݒ���擾����( �߂�l  TRUE:�}�e���A���J���[�̑���Ɏg�p����  FALSE:�}�e���A���J���[���g�p���� )
extern int NS_MV1GetMeshUseVertSpcColor( int MHandle, int MeshIndex )
{
	return MV1GetMeshUseVertSpcColorBase( MV1GetModelBaseHandle( MHandle ), MeshIndex ) ;
}

// �w��̃��b�V�����V�F�C�v���b�V�����ǂ������擾����( �߂�l TRUE:�V�F�C�v���b�V��  FALSE:�ʏ탁�b�V�� )
extern int NS_MV1GetMeshShapeFlag( int MHandle, int MeshIndex )
{
	return MV1GetMeshShapeFlagBase( MV1GetModelBaseHandle( MHandle ), MeshIndex ) ;
}























// �V�F�C�v�֌W

// ���f���Ɋ܂܂��V�F�C�v�̐����擾����
extern int NS_MV1GetShapeNum( int MHandle )
{
	return MV1GetShapeNumBase( MV1GetModelBaseHandle( MHandle ) ) ;
}

// �V�F�C�v�̖��O���烂�f�����̃V�F�C�v�̃V�F�C�v�C���f�b�N�X���擾����( ���������ꍇ�͖߂�l��-1 )
extern int NS_MV1SearchShape( int MHandle, const TCHAR *ShapeName )
{
#ifdef UNICODE
	return MV1SearchShape_WCHAR_T(
		MHandle, ShapeName
	) ;
#else
	int Result ;

	TCHAR_TO_WCHAR_T_STRING_ONE_BEGIN( ShapeName, return -1 )

	Result = MV1SearchShape_WCHAR_T(
		MHandle, UseShapeNameBuffer
	) ;

	TCHAR_TO_WCHAR_T_STRING_END( ShapeName )

	return Result ;
#endif
}

// �V�F�C�v�̖��O���烂�f�����̃V�F�C�v�̃V�F�C�v�C���f�b�N�X���擾����( ���������ꍇ�͖߂�l��-1 )
extern int MV1SearchShape_WCHAR_T( int MHandle, const wchar_t *ShapeName )
{
	return MV1SearchShapeBase( MV1GetModelBaseHandle( MHandle ), ShapeName ) ;
}

// �w��V�F�C�v�̖��O���擾����
extern const TCHAR *NS_MV1GetShapeName( int MHandle, int ShapeIndex )
{
#ifdef UNICODE
	return MV1GetShapeNameBaseW( MV1GetModelBaseHandle( MHandle ), ShapeIndex ) ;
#else
	return MV1GetShapeNameBaseA( MV1GetModelBaseHandle( MHandle ), ShapeIndex ) ;
#endif
}

// �w��V�F�C�v�̖��O���擾����
extern const wchar_t *MV1GetShapeName_WCHAR_T( int MHandle, int ShapeIndex )
{
	return MV1GetShapeNameBaseW( MV1GetModelBaseHandle( MHandle ), ShapeIndex ) ;
}

// �w��V�F�C�v���ΏۂƂ��Ă��郁�b�V���̐����擾����
extern int NS_MV1GetShapeTargetMeshNum( int MHandle, int ShapeIndex )
{
	return MV1GetShapeTargetMeshNumBase( MV1GetModelBaseHandle( MHandle ), ShapeIndex ) ;
}

// �w��V�F�C�v���ΏۂƂ��Ă��郁�b�V���̃��b�V���C���f�b�N�X���擾����
extern int NS_MV1GetShapeTargetMesh( int MHandle, int ShapeIndex, int Index )
{
	return MV1GetShapeTargetMeshBase( MV1GetModelBaseHandle( MHandle ), ShapeIndex, Index ) ;
}

// �w��V�F�C�v�̗L������ݒ肷��( Rate  0.0f:0% �` 1.0f:100% )
extern int NS_MV1SetShapeRate( int MHandle, int ShapeIndex, float Rate )
{
	MV1SHAPESTART( MHandle, Model, ModelBase, Shape, ShapeIndex, -1 ) ;

	// �قڒl���ω����Ȃ��ꍇ�͉������Ȃ�
	if( _FABS( Shape->Rate - Rate ) < 0.0000001f )
	{
		return 0 ;
	}

	// �L������ۑ�
	Shape->Rate = Rate ;

	// �V�F�C�v�̏�Ԃ��ω������t���O�𗧂Ă�
	Shape->Container->ShapeChangeFlag = true ;
	Shape->Container->Container->ShapeChangeFlag = true ;

	// ����I��
	return 0 ;
}

// �w��V�F�C�v�̗L�������擾����( �߂�l  0.0f:0% �` 1.0f:100% )
extern float NS_MV1GetShapeRate( int MHandle, int ShapeIndex )
{
	MV1SHAPESTART( MHandle, Model, ModelBase, Shape, ShapeIndex, -1.0f ) ;

	// �L������Ԃ�
	return Shape->Rate ;
}
























// �g���C�A���O�����X�g�̐����擾����
extern int NS_MV1GetTriangleListNum( int MHandle )
{
	MV1_MODEL *Model ;
	MV1_MODEL_BASE *ModelBase ;

	// ����������Ă��Ȃ�������G���[
	if( MV1Man.Initialize == false ) return -1 ;

	// �A�h���X�擾
	if( MV1MDLCHK( MHandle, Model ) )
		return -1 ;
	ModelBase = Model->BaseData ;

	// �}�e���A���̐���Ԃ�
	return ModelBase->TriangleListNum ;
}

// �w��̃g���C�A���O�����X�g�̒��_�f�[�^�^�C�v���擾����( MV1_VERTEX_TYPE_NORMAL �� )
extern int NS_MV1GetTriangleListVertexType( int MHandle, int TListIndex )
{
	int VertexType ;

	MV1TLISTSTART( MHandle, Model, ModelBase, TList, TListIndex, -1 ) ;

	// ���_�f�[�^�^�C�v��Ԃ�
	VertexType = TList->VertexType ;
	if( TList->Container->Material->NormalLayerNum != 0 )
	{
		VertexType += DX_MV1_VERTEX_TYPE_NMAP_1FRAME ;
	}

	return VertexType ;
}

// �w��̃g���C�A���O�����X�g�Ɋ܂܂��|���S���̐����擾����
extern int NS_MV1GetTriangleListPolygonNum( int MHandle, int TListIndex )
{
	MV1TLISTSTART( MHandle, Model, ModelBase, TList, TListIndex, -1 ) ;

	// �|���S���̐���Ԃ�
	return TList->IndexNum / 3 ;
}

// �w��̃g���C�A���O�����X�g�Ɋ܂܂�钸�_�f�[�^�̐����擾����
extern int NS_MV1GetTriangleListVertexNum( int MHandle, int TListIndex )
{
	MV1TLISTSTART( MHandle, Model, ModelBase, TList, TListIndex, -1 ) ;

	// ���_�f�[�^�̐���Ԃ�
	return TList->VertexNum ;
}

// �w��̃g���C�A���O�����X�g���g�p������W�ϊ��s��̐����擾����
extern int NS_MV1GetTriangleListLocalWorldMatrixNum( int MHandle, int TListIndex )
{
	MV1TLISTSTART( MHandle, Model, ModelBase, TList, TListIndex, -1 ) ;

	// ���W�ϊ��s��̐���Ԃ�
	return TList->UseBoneNum ;
}

// �w��̃g���C�A���O�����X�g�̎w��̃|���S�����g�p���Ă��钸�_�̍��W���擾����( �߂�l  �G���[�F-1  0�ȏ�F�|���S�����g�p���Ă��钸�_�̐� )
extern int NS_MV1GetTriangleListPolygonVertexPosition( int MHandle, int TListIndex, int PolygonIndex, VECTOR *VertexPositions )
{
	MV1TLISTSTART( MHandle, Model, ModelBase, TList, TListIndex, -1 ) ;
	WORD *Index ;

	// �|���S���̐����͈͂𒴂��Ă�����G���[
	if( PolygonIndex < 0 || PolygonIndex >= TList->IndexNum / 3 )
	{
		return -1 ;
	}

	// VertexPositions �� NULL �ł͖����ꍇ�͍��W���Z�b�g����
	if( VertexPositions != NULL )
	{
		Index = &TList->Index[ PolygonIndex * 3 ] ;

		switch( TList->VertexType )
		{
		case MV1_VERTEX_TYPE_NORMAL :
			VertexPositions[ 0 ].x = TList->NormalPosition[ Index[ 0 ] ].Position.x ;
			VertexPositions[ 0 ].y = TList->NormalPosition[ Index[ 0 ] ].Position.y ;
			VertexPositions[ 0 ].z = TList->NormalPosition[ Index[ 0 ] ].Position.z ;
			VertexPositions[ 1 ].x = TList->NormalPosition[ Index[ 1 ] ].Position.x ;
			VertexPositions[ 1 ].y = TList->NormalPosition[ Index[ 1 ] ].Position.y ;
			VertexPositions[ 1 ].z = TList->NormalPosition[ Index[ 1 ] ].Position.z ;
			VertexPositions[ 2 ].x = TList->NormalPosition[ Index[ 2 ] ].Position.x ;
			VertexPositions[ 2 ].y = TList->NormalPosition[ Index[ 2 ] ].Position.y ;
			VertexPositions[ 2 ].z = TList->NormalPosition[ Index[ 2 ] ].Position.z ;
			break ;

		case MV1_VERTEX_TYPE_SKIN_4BONE :
			VertexPositions[ 0 ].x = TList->SkinPosition4B[ Index[ 0 ] ].Position.x ;
			VertexPositions[ 0 ].y = TList->SkinPosition4B[ Index[ 0 ] ].Position.y ;
			VertexPositions[ 0 ].z = TList->SkinPosition4B[ Index[ 0 ] ].Position.z ;
			VertexPositions[ 1 ].x = TList->SkinPosition4B[ Index[ 1 ] ].Position.x ;
			VertexPositions[ 1 ].y = TList->SkinPosition4B[ Index[ 1 ] ].Position.y ;
			VertexPositions[ 1 ].z = TList->SkinPosition4B[ Index[ 1 ] ].Position.z ;
			VertexPositions[ 2 ].x = TList->SkinPosition4B[ Index[ 2 ] ].Position.x ;
			VertexPositions[ 2 ].y = TList->SkinPosition4B[ Index[ 2 ] ].Position.y ;
			VertexPositions[ 2 ].z = TList->SkinPosition4B[ Index[ 2 ] ].Position.z ;
			break ;

		case MV1_VERTEX_TYPE_SKIN_8BONE :
			VertexPositions[ 0 ] = TList->SkinPosition8B[ Index[ 0 ] ].Position ;
			VertexPositions[ 1 ] = TList->SkinPosition8B[ Index[ 1 ] ].Position ;
			VertexPositions[ 2 ] = TList->SkinPosition8B[ Index[ 2 ] ].Position ;
			break ;

		case MV1_VERTEX_TYPE_SKIN_FREEBONE :
			VertexPositions[ 0 ].x = TList->SkinPositionFREEB[ Index[ 0 ] ].Position.x ;
			VertexPositions[ 0 ].y = TList->SkinPositionFREEB[ Index[ 0 ] ].Position.y ;
			VertexPositions[ 0 ].z = TList->SkinPositionFREEB[ Index[ 0 ] ].Position.z ;
			VertexPositions[ 1 ].x = TList->SkinPositionFREEB[ Index[ 1 ] ].Position.x ;
			VertexPositions[ 1 ].y = TList->SkinPositionFREEB[ Index[ 1 ] ].Position.y ;
			VertexPositions[ 1 ].z = TList->SkinPositionFREEB[ Index[ 1 ] ].Position.z ;
			VertexPositions[ 2 ].x = TList->SkinPositionFREEB[ Index[ 2 ] ].Position.x ;
			VertexPositions[ 2 ].y = TList->SkinPositionFREEB[ Index[ 2 ] ].Position.y ;
			VertexPositions[ 2 ].z = TList->SkinPositionFREEB[ Index[ 2 ] ].Position.z ;
			break ;
		}
	}

	// ���W�̐���Ԃ�
	return 3 ;
}

// �w��̃g���C�A���O�����X�g���g�p������W�ϊ��s��( ���[�J�������[���h )���擾����
extern MATRIX NS_MV1GetTriangleListLocalWorldMatrix( int MHandle, int TListIndex, int LWMatrixIndex )
{
	MATRIX Result = { 0.0f } ;
	MV1_FRAME *Frame ;
	MV1TLISTSTART( MHandle, Model, ModelBase, TList, TListIndex, Result ) ;

	Frame = &Model->Frame[ TList->Container->Container->Index ] ;

	// �s��̃Z�b�g�A�b�v
	MV1SETUPMATRIX( Model ) ;

	// �s���Ԃ�
	ConvertMatrix4x4cToMatrixF( &Result, Frame->UseSkinBoneMatrix[ TList->UseBone[ LWMatrixIndex ] ] ) ;
	return Result ;
}

// �w��̃g���C�A���O�����X�g���g�p���Ă���{�[���t���[���̐����擾����
extern int MV1GetTriangleListUseBoneFrameNum( int MHandle, int TListIndex )
{
	MV1TLISTSTART( MHandle, Model, ModelBase, TList, TListIndex, -1 ) ;

	// �g�p���Ă���{�[���̐���Ԃ�
	return TList->UseBoneNum ;
}

// �w��̃g���C�A���O�����X�g���g�p���Ă���{�[���t���[���̃t���[���C���f�b�N�X���擾����
extern int MV1GetTriangleListUseBoneFrame( int MHandle, int TListIndex, int Index )
{
	int i, con ;
	MV1_FRAME_BASE *FrameBase ;

	MV1TLISTSTART( MHandle, Model, ModelBase, TList, TListIndex, -1 ) ;

	FrameBase = TList->Container->Container ;

	// �w��̃{�[���Ƃ��Ďg�p���Ă���t���[����Ԃ�
	con = 0 ;
	for( i = 0 ; i < MV1_TRIANGLE_LIST_USE_BONE_MAX_NUM ; i ++ )
	{
		if( TList->UseBone[ i ] == -1 ) continue ;

		if( con == Index )
			return FrameBase->UseSkinBone[ TList->UseBone[ i ] ]->BoneFrame ;
		con ++ ;
	}

	// �����ɂ�����G���[
	return -1 ;
}






// �R���W���������\�z����
extern int NS_MV1SetupCollInfo( int MHandle, int FrameIndex, int XDivNum, int YDivNum, int ZDivNum )
{
	MV1_MODEL *Model ;
	MV1_MODEL_BASE *ModelBase ;
	MV1_FRAME *Frame ;

	// �A�h���X�擾
	if( MV1MDLCHK( MHandle, Model ) )
		return -1 ;
	ModelBase = Model->BaseData ;

	// �t���[���P�̂����f���S�̂��ŏ����𕪊�
	if( FrameIndex == -1 )
	{
		// ���f���S�̂̏ꍇ

		// �Q�Ɨp�|���S����񂪂Ȃ��ꍇ�͍\�z����
		if( Model->RefPolygon[ 1 ][ 1 ] == NULL )
			if( NS_MV1RefreshReferenceMesh( MHandle, FrameIndex, TRUE, TRUE ) < 0 )
				return -1 ;

		// �m�ۂ���Ă��Ȃ��ꍇ�̂݊m��
		if( Model->Collision == NULL )
		{
			Model->Collision = ( MV1_COLLISION * )DXALLOC( sizeof( MV1_COLLISION ) + sizeof( MV1_COLL_POLYGON * ) * XDivNum * YDivNum * ZDivNum + sizeof( MV1_COLL_POLY_BUFFER ) + sizeof( MV1_COLL_POLYGON ) * ( Model->RefPolygon[ 1 ][ 1 ]->PolygonNum * 2 ) ) ;
			if( Model->Collision == NULL )
			{
				DXST_ERRORLOGFMT_ADDUTF16LE(( "\xb3\x30\xea\x30\xb8\x30\xe7\x30\xf3\x30\xc5\x60\x31\x58\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�R���W���������i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
				return -1 ;
			}
			Model->Collision->Polygon = ( MV1_COLL_POLYGON ** )( Model->Collision + 1 ) ;
			Model->Collision->LastBuffer = Model->Collision->FirstBuffer = ( MV1_COLL_POLY_BUFFER * )( Model->Collision->Polygon + XDivNum * YDivNum * ZDivNum ) ;
			Model->Collision->FirstBuffer->BufferSize = Model->RefPolygon[ 1 ][ 1 ]->PolygonNum * 2 ;
			Model->Collision->FirstBuffer->UseSize = 0 ;
			Model->Collision->FirstBuffer->Next = NULL ;
			Model->Collision->FirstBuffer->Buffer = ( MV1_COLL_POLYGON * )( Model->Collision->FirstBuffer + 1 ) ;
		}

		Model->Collision->XDivNum = XDivNum ;
		Model->Collision->YDivNum = YDivNum ;
		Model->Collision->ZDivNum = ZDivNum ;
	}
	else
	{
		// �t���[���C���f�b�N�X�̃`�F�b�N
		if( FrameIndex < 0 || FrameIndex >= ModelBase->FrameNum )
			return -1 ;
		Frame = Model->Frame + FrameIndex ;

		// �Q�Ɨp�|���S����񂪂Ȃ��ꍇ�͍\�z����
		if( Frame->RefPolygon[ 1 ][ 1 ] == NULL )
			if( NS_MV1RefreshReferenceMesh( MHandle, FrameIndex, TRUE, TRUE ) < 0 )
				return -1 ;

		// �m�ۂ���Ă��Ȃ��ꍇ�̂݊m��
		if( Frame->Collision == NULL )
		{
			Frame->Collision = ( MV1_COLLISION * )DXALLOC( sizeof( MV1_COLLISION ) + sizeof( MV1_COLL_POLYGON * ) * XDivNum * YDivNum * ZDivNum + sizeof( MV1_COLL_POLY_BUFFER ) + sizeof( MV1_COLL_POLYGON ) * ( Frame->RefPolygon[ 1 ][ 1 ]->PolygonNum * 2 ) ) ;
			if( Frame->Collision == NULL )
			{
				DXST_ERRORLOGFMT_ADDUTF16LE(( "\xb3\x30\xea\x30\xb8\x30\xe7\x30\xf3\x30\xc5\x60\x31\x58\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�R���W���������i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
				return -1 ;
			}
			Frame->Collision->Polygon = ( MV1_COLL_POLYGON ** )( Frame->Collision + 1 ) ;
			Frame->Collision->LastBuffer = Frame->Collision->FirstBuffer = ( MV1_COLL_POLY_BUFFER * )( Frame->Collision->Polygon + XDivNum * YDivNum * ZDivNum ) ;
			Frame->Collision->FirstBuffer->BufferSize = Frame->RefPolygon[ 1 ][ 1 ]->PolygonNum * 2 ;
			Frame->Collision->FirstBuffer->UseSize = 0 ;
			Frame->Collision->FirstBuffer->Next = NULL ;
			Frame->Collision->FirstBuffer->Buffer = ( MV1_COLL_POLYGON * )( Frame->Collision->FirstBuffer + 1 ) ;
		}

		Frame->Collision->XDivNum = XDivNum ;
		Frame->Collision->YDivNum = YDivNum ;
		Frame->Collision->ZDivNum = ZDivNum ;
	}

	// �R���W���������\�z���Ă���
	NS_MV1RefreshCollInfo( MHandle, FrameIndex ) ;

	// �I��
	return 0 ;
}

// �R���W�������̌�n��
extern int NS_MV1TerminateCollInfo( int MHandle, int FrameIndex )
{
	MV1_MODEL *Model ;
	MV1_MODEL_BASE *ModelBase ;
	MV1_FRAME *Frame ;
	MV1_COLL_POLY_BUFFER *Buffer, *NextBuffer ;
	MV1_COLLISION *Collision ;

	// �A�h���X�擾
	if( MV1MDLCHK( MHandle, Model ) )
		return -1 ;
	ModelBase = Model->BaseData ;
	Collision = NULL ;

	// �t���[���P�̂����f���S�̂��ŏ����𕪊�
	if( FrameIndex == -1 )
	{
		// ���f���S�̂̏ꍇ
		Collision = Model->Collision ;
		Model->Collision = NULL ;
	}
	else
	{
		// �t���[���C���f�b�N�X�̃`�F�b�N
		if( FrameIndex < 0 || FrameIndex >= ModelBase->FrameNum )
			return -1 ;
		Frame = Model->Frame + FrameIndex ;

		// �t���[���P�̂̏ꍇ
		Collision = Frame->Collision ;
		Frame->Collision = NULL ;
	}

	// �R���W������񂪂���������
	if( Collision )
	{
		// �o�b�t�@���J��
		for( Buffer = Collision->FirstBuffer->Next ; Buffer ; Buffer = NextBuffer )
		{
			NextBuffer = Buffer->Next ;
			DXFREE( Buffer ) ;
		}

		// �{�̂����
		DXFREE( Collision ) ;
	}

	// �I��
	return 0 ;
}

// �R���W���������X�V����
extern int NS_MV1RefreshCollInfo( int MHandle, int FrameIndex )
{
	MV1_MODEL *Model ;
	MV1_MODEL_BASE *ModelBase ;
	MV1_FRAME *Frame ;
	MV1_REF_POLYGONLIST *PolyList ;
	MV1_REF_POLYGON *Poly ;
	MV1_COLLISION *Collision ;
	MV1_COLL_POLYGON *ColPoly ;
	MV1_COLL_POLY_BUFFER *ColBuffer ;
	int i, xc, yc, zc, ZStep, YStep ;
	int MinX, MinY, MinZ, MaxX, MaxY, MaxZ, PostIndexZ, PostIndexY, PostIndexX ;
	VECTOR UnitDiv, MinAreaPos ;

	// �A�h���X�擾
	if( MV1MDLCHK( MHandle, Model ) )
		return -1 ;
	ModelBase = Model->BaseData ;

	// �t���[���P�̂����f���S�̂��ŏ����𕪊�
	if( FrameIndex == -1 )
	{
		// ���f���S�̂̏ꍇ

		// �R���W������񂪖���������Z�b�g�A�b�v����
		if( Model->Collision == NULL )
			if( NS_MV1SetupCollInfo( MHandle, FrameIndex ) < 0 )
				return -1 ;

		// �Q�Ɨp���b�V���̍X�V
		NS_MV1RefreshReferenceMesh( MHandle, FrameIndex, TRUE, TRUE ) ;

		// �R���W�������̍X�V���������Ă����牽�����Ȃ�
		if( Model->SetupCollision == true )
			return 0 ;

		// �|���S�����̃Z�b�g
		PolyList = Model->RefPolygon[ 1 ][ 1 ] ;
		Collision = Model->Collision ;

		// �R���W�������̍X�V�����t���O�𗧂Ă�
		Model->SetupCollision = true ;
	}
	else
	{
		// �t���[���P�̂̏ꍇ

		// �t���[���C���f�b�N�X�̃`�F�b�N
		if( FrameIndex < 0 || FrameIndex >= ModelBase->FrameNum )
			return -1 ;
		Frame = Model->Frame + FrameIndex ;

		// �R���W������񂪖���������Z�b�g�A�b�v����
		if( Frame->Collision == NULL )
			if( NS_MV1SetupCollInfo( MHandle, FrameIndex ) < 0 )
				return -1 ;

		// �Q�Ɨp���b�V���̍X�V
		NS_MV1RefreshReferenceMesh( MHandle, FrameIndex, TRUE, TRUE ) ;

		// �R���W�������̍X�V���������Ă����牽�����Ȃ�
		if( Frame->SetupCollision == true )
			return 0 ;

		// �|���S�����̃Z�b�g
		PolyList = Frame->RefPolygon[ 1 ][ 1 ] ;
		Collision = Frame->Collision ;

		// �R���W�������̍X�V�����t���O�𗧂Ă�
		Frame->SetupCollision = true ;
	}

	// �ő�l�ƍŏ��l������Z�b�g
	Collision->MaxPosition = PolyList->MaxPosition ;
	Collision->MinPosition = PolyList->MinPosition ;
	VectorSub( &Collision->Size, &Collision->MaxPosition, &Collision->MinPosition ) ;
	Collision->UnitSize.x = Collision->Size.x / ( float )Collision->XDivNum ;
	Collision->UnitSize.y = Collision->Size.y / ( float )Collision->YDivNum ;
	Collision->UnitSize.z = Collision->Size.z / ( float )Collision->ZDivNum ;
	if( Collision->UnitSize.x < 0.00001f ) Collision->UnitSize.x = 1.0f ;
	if( Collision->UnitSize.y < 0.00001f ) Collision->UnitSize.y = 1.0f ;
	if( Collision->UnitSize.z < 0.00001f ) Collision->UnitSize.z = 1.0f ;
	Collision->UnitSizeRev.x = 1.0f / Collision->UnitSize.x ;
	Collision->UnitSizeRev.y = 1.0f / Collision->UnitSize.y ;
	Collision->UnitSizeRev.z = 1.0f / Collision->UnitSize.z ;

	// �e�[�u����������
	_MEMSET( Collision->Polygon, 0, sizeof( MV1_COLL_POLYGON * ) * Collision->XDivNum * Collision->YDivNum * Collision->ZDivNum ) ;

	// �o�b�t�@��������
	Collision->FirstBuffer->UseSize = 0 ;

	// �R���W�������\�z
	Poly = PolyList->Polygons ;
	ColBuffer = Collision->FirstBuffer ;
	UnitDiv = Collision->UnitSizeRev ;
	MinAreaPos = PolyList->MinPosition ;
	ZStep = Collision->YDivNum * Collision->XDivNum ;
	YStep = Collision->XDivNum ;
	for( i = 0 ; i < PolyList->PolygonNum ; i ++, Poly ++ )
	{
		// �͈͂𐮐��l�ɕϊ�
		MinX = __FTOL( ( Poly->MinPosition.x - MinAreaPos.x ) * UnitDiv.x ) ;
		MinY = __FTOL( ( Poly->MinPosition.y - MinAreaPos.y ) * UnitDiv.y ) ;
		MinZ = __FTOL( ( Poly->MinPosition.z - MinAreaPos.z ) * UnitDiv.z ) ;
		MaxX = __FTOL( ( Poly->MaxPosition.x - MinAreaPos.x ) * UnitDiv.x ) ;
		MaxY = __FTOL( ( Poly->MaxPosition.y - MinAreaPos.y ) * UnitDiv.y ) ;
		MaxZ = __FTOL( ( Poly->MaxPosition.z - MinAreaPos.z ) * UnitDiv.z ) ;
		if( MaxX == Collision->XDivNum ) MaxX = Collision->XDivNum - 1 ;
		if( MaxY == Collision->YDivNum ) MaxY = Collision->YDivNum - 1 ;
		if( MaxZ == Collision->ZDivNum ) MaxZ = Collision->ZDivNum - 1 ;
		if( MinX == Collision->XDivNum ) MinX = Collision->XDivNum - 1 ;
		if( MinY == Collision->YDivNum ) MinY = Collision->YDivNum - 1 ;
		if( MinZ == Collision->ZDivNum ) MinZ = Collision->ZDivNum - 1 ;

		// �|���S�������X�g�ɒǉ�
		PostIndexZ = MinX + MinY * YStep + MinZ * ZStep ;
		for( zc = MinZ ; zc <= MaxZ ; zc ++, PostIndexZ += ZStep )
		{
			PostIndexY = PostIndexZ ;
			for( yc = MinY ; yc <= MaxY ; yc ++, PostIndexY += YStep )
			{
				PostIndexX = PostIndexY ;
				for( xc = MinX ; xc <= MaxX ; xc ++, PostIndexX ++ )
				{
					// �ǉ������Ɏg���|���S�����̊m��
					if( ColBuffer->UseSize == ColBuffer->BufferSize )
					{
						// ���̃o�b�t�@������������V�����������̈���m��
						if( ColBuffer->Next == NULL )
						{
							ColBuffer->Next = ( MV1_COLL_POLY_BUFFER * )DXALLOC( sizeof( MV1_COLL_POLY_BUFFER ) + sizeof( MV1_COLL_POLYGON ) * PolyList->PolygonNum ) ;
							if( ColBuffer->Next == NULL )
							{
								DXST_ERRORLOGFMT_ADDUTF16LE(( "\xb3\x30\xea\x30\xb8\x30\xe7\x30\xf3\x30\xfd\x8f\xa0\x52\xc5\x60\x31\x58\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�R���W�����ǉ������i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
								return -1 ;
							}
							ColBuffer->Next->BufferSize = PolyList->PolygonNum ;
							ColBuffer->Next->UseSize = 0 ;
							ColBuffer->Next->Buffer = ( MV1_COLL_POLYGON * )( ColBuffer->Next + 1 ) ;
							ColBuffer->Next->Next = NULL ;
							Collision->LastBuffer = ColBuffer->Next ;
						}
						ColBuffer = ColBuffer->Next ;
						ColBuffer->UseSize = 0 ;
					}
					ColPoly = &ColBuffer->Buffer[ ColBuffer->UseSize ] ;
					ColBuffer->UseSize ++ ;

					// �R���W�����|���S�������Z�b�g
					ColPoly->Polygon = Poly ;
					ColPoly->Next = Collision->Polygon[ PostIndexX ] ;
					Collision->Polygon[ PostIndexX ] = ColPoly ;
				}
			}
		}
	}

	// �I��
	return 0 ;
}

// ���ƃ��f���̓����蔻��
extern MV1_COLL_RESULT_POLY NS_MV1CollCheck_Line( int MHandle, int FrameIndex, VECTOR PosStart, VECTOR PosEnd )
{
	MV1_MODEL *Model ;
	MV1_MODEL_BASE *ModelBase ;
	MV1_FRAME *Frame ;
	MV1_REF_POLYGONLIST *PolyList ;
	MV1_REF_POLYGON *Poly, *MinPoly ;
	MV1_COLLISION *Collision ;
	MV1_COLL_POLYGON *ColPoly ;
	int xc, yc, zc, ZStep, YStep, PolyIndex ;
	int MinX, MinY, MinZ, MaxX, MaxY, MaxZ, MinPolyIndex = 0, PostIndexZ, PostIndexY, PostIndexX ;
	VECTOR UnitDiv, MinPos, MaxPos, Sa, MinHitPos = { 0.0f } ;
	MV1_COLL_RESULT_POLY Result ;
	float MinLength = 0.0f, Length ;
	HITRESULT_LINE LineRes ;
	BYTE *BitBuffer = NULL ;

	// �߂�l��������
	_MEMSET( &Result, 0, sizeof( Result ) ) ;

	// �A�h���X�擾
	if( MV1MDLCHK( MHandle, Model ) )
		return Result ;
	ModelBase = Model->BaseData ;

	// �t���[���P�̂����f���S�̂��ŏ����𕪊�
	if( FrameIndex == -1 )
	{
		// ���f���S�̂̏ꍇ

		// �R���W������񂪖���������Z�b�g�A�b�v����
		if( Model->Collision == NULL )
			if( NS_MV1RefreshCollInfo( MHandle, FrameIndex ) < 0 )
				return Result ;

		// �|���S�����̃Z�b�g
		PolyList = Model->RefPolygon[ 1 ][ 1 ] ;
		Collision = Model->Collision ;
	}
	else
	{
		// �t���[���P�̂̏ꍇ

		// �t���[���C���f�b�N�X�̃`�F�b�N
		if( FrameIndex < 0 || FrameIndex >= ModelBase->FrameNum )
			return Result ;
		Frame = Model->Frame + FrameIndex ;

		// �R���W������񂪖���������Z�b�g�A�b�v����
		if( Frame->Collision == NULL )
			if( NS_MV1RefreshCollInfo( MHandle, FrameIndex ) < 0 )
				return Result ;

		// �|���S�����̃Z�b�g
		PolyList = Frame->RefPolygon[ 1 ][ 1 ] ;
		Collision = Frame->Collision ;
	}

	// �w����W�̍ŏ��l�ƍő�l���Z�b�g
	if( PosStart.x > PosEnd.x )
	{
		MaxPos.x = PosStart.x ;
		MinPos.x = PosEnd.x ;
	}
	else
	{
		MaxPos.x = PosEnd.x ;
		MinPos.x = PosStart.x ;
	}

	if( PosStart.y > PosEnd.y )
	{
		MaxPos.y = PosStart.y ;
		MinPos.y = PosEnd.y ;
	}
	else
	{
		MaxPos.y = PosEnd.y ;
		MinPos.y = PosStart.y ;
	}

	if( PosStart.z > PosEnd.z )
	{
		MaxPos.z = PosStart.z ;
		MinPos.z = PosEnd.z ;
	}
	else
	{
		MaxPos.z = PosEnd.z ;
		MinPos.z = PosStart.z ;
	}

	// �͈͂��R���W������Ԃ���O��Ă�����ǂ̃|���S���ɂ�������Ȃ��Ƃ�������
	if( Collision->MaxPosition.x < MinPos.x ||
		Collision->MaxPosition.y < MinPos.y ||
		Collision->MaxPosition.z < MinPos.z ||
		Collision->MinPosition.x > MaxPos.x ||
		Collision->MinPosition.y > MaxPos.y ||
		Collision->MinPosition.z > MaxPos.z )
		goto END ;

	// �����蔻����s�������ǂ�����ۑ����郁�����̈�̊m��
	BitBuffer = ( BYTE * )DXALLOC( sizeof( BYTE ) * ( ( PolyList->PolygonNum + 7 ) / 8 ) ) ;
	if( BitBuffer == NULL )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\x53\x5f\x5f\x30\x8a\x30\x24\x52\x9a\x5b\xe6\x51\x06\x74\x28\x75\xc6\x30\xf3\x30\xdd\x30\xe9\x30\xea\x30\xd0\x30\xc3\x30\xd5\x30\xa1\x30\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�����蔻�菈���p�e���|�����o�b�t�@�̊m�ۂɎ��s���܂���\n" @*/ )) ;
		return Result ;
	}
	_MEMSET( BitBuffer, 0, sizeof( BYTE ) * ( ( PolyList->PolygonNum + 7 ) / 8 ) ) ;

	// �����͈͂�����
	UnitDiv = Collision->UnitSizeRev ;
	MinX = __FTOL( ( MinPos.x - Collision->MinPosition.x ) * UnitDiv.x ) ;
	MinY = __FTOL( ( MinPos.y - Collision->MinPosition.y ) * UnitDiv.y ) ;
	MinZ = __FTOL( ( MinPos.z - Collision->MinPosition.z ) * UnitDiv.z ) ;
	MaxX = __FTOL( ( MaxPos.x - Collision->MinPosition.x ) * UnitDiv.x ) ;
	MaxY = __FTOL( ( MaxPos.y - Collision->MinPosition.y ) * UnitDiv.y ) ;
	MaxZ = __FTOL( ( MaxPos.z - Collision->MinPosition.z ) * UnitDiv.z ) ;
	if( MinX < 0 ) MinX = 0 ;
	if( MinY < 0 ) MinY = 0 ;
	if( MinZ < 0 ) MinZ = 0 ;
	if( MaxX >= Collision->XDivNum ) MaxX = Collision->XDivNum - 1 ;
	if( MaxY >= Collision->YDivNum ) MaxY = Collision->YDivNum - 1 ;
	if( MaxZ >= Collision->ZDivNum ) MaxZ = Collision->ZDivNum - 1 ;

	// �͈͓��̑S�Ẵ|���S���Ɠ����蔻��
	ZStep = Collision->YDivNum * Collision->XDivNum ;
	YStep = Collision->XDivNum ;
	PostIndexZ = MinX + MinY * YStep + MinZ * ZStep ;
	MinPoly = NULL ;
	for( zc = MinZ ; zc <= MaxZ ; zc ++, PostIndexZ += ZStep )
	{
		PostIndexY = PostIndexZ ;
		for( yc = MinY ; yc <= MaxY ; yc ++, PostIndexY += YStep )
		{
			PostIndexX = PostIndexY ;
			for( xc = MinX ; xc <= MaxX ; xc ++, PostIndexX ++ )
			{
				for( ColPoly = Collision->Polygon[ PostIndexX ] ; ColPoly ; ColPoly = ColPoly->Next )
				{
					Poly = ColPoly->Polygon ;
					PolyIndex = ( int )( Poly - PolyList->Polygons ) ;

					// ���Ƀ`�F�b�N�ς݂̃|���S���������ꍇ�͉������Ȃ�
					if( BitBuffer[ PolyIndex >> 3 ] & ( 1 << ( PolyIndex & 7 ) ) ) continue ;

					// �ŏ��ɗ����̒P�ʂ̓����蔻����s��
					if( ( MinPos.x > Poly->MaxPosition.x ||
						  MinPos.y > Poly->MaxPosition.y ||
						  MinPos.z > Poly->MaxPosition.z ||
						  MaxPos.x < Poly->MinPosition.x ||
						  MaxPos.y < Poly->MinPosition.y ||
						  MaxPos.z < Poly->MinPosition.z ) == false )
					{
						// �O�p�`�Ɛ��̓����蔻��
						LineRes = HitCheck_Line_Triangle(
										*( ( VECTOR * )&PosStart ),
										*( ( VECTOR * )&PosEnd ),
										PolyList->Vertexs[ Poly->VIndex[ 0 ] ].Position,
										PolyList->Vertexs[ Poly->VIndex[ 1 ] ].Position,
										PolyList->Vertexs[ Poly->VIndex[ 2 ] ].Position ) ;
						if( LineRes.HitFlag )
						{
							VectorSub( &Sa, &LineRes.Position, ( VECTOR * )&PosStart ) ;
							Length = Sa.x * Sa.x + Sa.y * Sa.y + Sa.z * Sa.z ;
							if( MinPoly == NULL || Length < MinLength )
							{
								if( VSquareSize( VSub( PolyList->Vertexs[ Poly->VIndex[ 0 ] ].Position, PolyList->Vertexs[ Poly->VIndex[ 1 ] ].Position ) ) > 0.0000001f &&
									VSquareSize( VSub( PolyList->Vertexs[ Poly->VIndex[ 0 ] ].Position, PolyList->Vertexs[ Poly->VIndex[ 2 ] ].Position ) ) > 0.0000001f &&
									VSquareSize( VSub( PolyList->Vertexs[ Poly->VIndex[ 1 ] ].Position, PolyList->Vertexs[ Poly->VIndex[ 2 ] ].Position ) ) > 0.0000001f )
								{
									MinPoly = Poly ;
									MinLength = Length ;
									MinHitPos = LineRes.Position ;
									MinPolyIndex = PolyIndex ;
								}
							}
						}
					}

					// �`�F�b�N�ς݃t���O�𗧂Ă�
					BitBuffer[ PolyIndex >> 3 ] |= 1 << ( PolyIndex & 7 ) ;
				}
			}
		}
	}

	// ���ʂ���
	if( MinPoly )
	{
		Result.HitFlag = 1 ;
		Result.HitPosition = MinHitPos ;

		Result.FrameIndex = MinPoly->FrameIndex ;
		Result.MaterialIndex = MinPoly->MaterialIndex ;
		Result.PolygonIndex = MinPolyIndex ;
		Result.Position[ 0 ] = PolyList->Vertexs[ MinPoly->VIndex[ 0 ] ].Position ;
		Result.Position[ 1 ] = PolyList->Vertexs[ MinPoly->VIndex[ 1 ] ].Position ;
		Result.Position[ 2 ] = PolyList->Vertexs[ MinPoly->VIndex[ 2 ] ].Position ;
		Result.Normal = VNorm( VCross( VSub( Result.Position[ 1 ], Result.Position[ 0 ] ), VSub( Result.Position[ 2 ], Result.Position[ 0 ] ) ) ) ; 
	}

END :

	// �������̉��
	if( BitBuffer )
	{
		DXFREE( BitBuffer ) ;
		BitBuffer = NULL ;
	}

	return Result ;
}

// ���ƃ��f���̓����蔻��( �߂�l�� MV1_COLL_RESULT_POLY_DIM )
extern	MV1_COLL_RESULT_POLY_DIM NS_MV1CollCheck_LineDim( int MHandle, int FrameIndex, VECTOR PosStart, VECTOR PosEnd )
{
	int MaxNum ;
	MV1_MODEL *Model ;
	MV1_MODEL_BASE *ModelBase ;
	MV1_FRAME *Frame ;
	MV1_REF_POLYGONLIST *PolyList ;
	MV1_REF_POLYGON *Poly, *MinPoly ;
	MV1_COLLISION *Collision ;
	MV1_COLL_POLYGON *ColPoly ;
	int xc, yc, zc, ZStep, YStep, PolyIndex ;
	int MinX, MinY, MinZ, MaxX, MaxY, MaxZ, PostIndexZ, PostIndexY, PostIndexX ;
	VECTOR UnitDiv, MinPos, MaxPos ;
	MV1_COLL_RESULT_POLY_DIM Result ;
	MV1_COLL_RESULT_POLY *RPoly ;
	HITRESULT_LINE LineRes ;
	BYTE *BitBuffer = NULL ;

	// ���ʏ�����
	Result.HitNum = 0 ;
	Result.Dim = NULL ;

	// �A�h���X�擾
	if( MV1MDLCHK( MHandle, Model ) )
		return Result ;
	ModelBase = Model->BaseData ;

	// �t���[���P�̂����f���S�̂��ŏ����𕪊�
	if( FrameIndex == -1 )
	{
		// ���f���S�̂̏ꍇ

		// �R���W������񂪖���������Z�b�g�A�b�v����
		if( Model->Collision == NULL )
			if( NS_MV1RefreshCollInfo( MHandle, FrameIndex ) < 0 )
				return Result ;

		// �|���S�����̃Z�b�g
		PolyList = Model->RefPolygon[ 1 ][ 1 ] ;
		Collision = Model->Collision ;
	}
	else
	{
		// �t���[���P�̂̏ꍇ

		// �t���[���C���f�b�N�X�̃`�F�b�N
		if( FrameIndex < 0 || FrameIndex >= ModelBase->FrameNum )
			return Result ;
		Frame = Model->Frame + FrameIndex ;

		// �R���W������񂪖���������Z�b�g�A�b�v����
		if( Frame->Collision == NULL )
			if( NS_MV1RefreshCollInfo( MHandle, FrameIndex ) < 0 )
				return Result ;

		// �|���S�����̃Z�b�g
		PolyList = Frame->RefPolygon[ 1 ][ 1 ] ;
		Collision = Frame->Collision ;
	}

	// �w����W�̍ŏ��l�ƍő�l���Z�b�g
	if( PosStart.x > PosEnd.x )
	{
		MaxPos.x = PosStart.x ;
		MinPos.x = PosEnd.x ;
	}
	else
	{
		MaxPos.x = PosEnd.x ;
		MinPos.x = PosStart.x ;
	}

	if( PosStart.y > PosEnd.y )
	{
		MaxPos.y = PosStart.y ;
		MinPos.y = PosEnd.y ;
	}
	else
	{
		MaxPos.y = PosEnd.y ;
		MinPos.y = PosStart.y ;
	}

	if( PosStart.z > PosEnd.z )
	{
		MaxPos.z = PosStart.z ;
		MinPos.z = PosEnd.z ;
	}
	else
	{
		MaxPos.z = PosEnd.z ;
		MinPos.z = PosStart.z ;
	}

	// �͈͂��R���W������Ԃ���O��Ă�����ǂ̃|���S���ɂ�������Ȃ��Ƃ�������
	if( Collision->MaxPosition.x < MinPos.x ||
		Collision->MaxPosition.y < MinPos.y ||
		Collision->MaxPosition.z < MinPos.z ||
		Collision->MinPosition.x > MaxPos.x ||
		Collision->MinPosition.y > MaxPos.y ||
		Collision->MinPosition.z > MaxPos.z )
		goto END ;

	// �����o�b�t�@���m��
	MaxNum = 1000 ;
	Result.Dim = ( MV1_COLL_RESULT_POLY * )DXALLOC( sizeof( MV1_COLL_RESULT_POLY ) * MaxNum ) ;
	if( Result.Dim == NULL ) return Result ;

	// �����蔻����s�������ǂ�����ۑ����郁�����̈�̊m��
	BitBuffer = ( BYTE * )DXALLOC( sizeof( BYTE ) * ( ( PolyList->PolygonNum + 7 ) / 8 ) ) ;
	if( BitBuffer == NULL )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\x53\x5f\x5f\x30\x8a\x30\x24\x52\x9a\x5b\xe6\x51\x06\x74\x28\x75\xc6\x30\xf3\x30\xdd\x30\xe9\x30\xea\x30\xd0\x30\xc3\x30\xd5\x30\xa1\x30\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�����蔻�菈���p�e���|�����o�b�t�@�̊m�ۂɎ��s���܂���\n" @*/ )) ;
		return Result ;
	}
	_MEMSET( BitBuffer, 0, sizeof( BYTE ) * ( ( PolyList->PolygonNum + 7 ) / 8 ) ) ;

	// �����͈͂�����
	UnitDiv = Collision->UnitSizeRev ;
	MinX = __FTOL( ( MinPos.x - Collision->MinPosition.x ) * UnitDiv.x ) ;
	MinY = __FTOL( ( MinPos.y - Collision->MinPosition.y ) * UnitDiv.y ) ;
	MinZ = __FTOL( ( MinPos.z - Collision->MinPosition.z ) * UnitDiv.z ) ;
	MaxX = __FTOL( ( MaxPos.x - Collision->MinPosition.x ) * UnitDiv.x ) ;
	MaxY = __FTOL( ( MaxPos.y - Collision->MinPosition.y ) * UnitDiv.y ) ;
	MaxZ = __FTOL( ( MaxPos.z - Collision->MinPosition.z ) * UnitDiv.z ) ;
	if( MinX < 0 ) MinX = 0 ;
	if( MinY < 0 ) MinY = 0 ;
	if( MinZ < 0 ) MinZ = 0 ;
	if( MaxX >= Collision->XDivNum ) MaxX = Collision->XDivNum - 1 ;
	if( MaxY >= Collision->YDivNum ) MaxY = Collision->YDivNum - 1 ;
	if( MaxZ >= Collision->ZDivNum ) MaxZ = Collision->ZDivNum - 1 ;

	// �͈͓��̑S�Ẵ|���S���Ɠ����蔻��
	ZStep = Collision->YDivNum * Collision->XDivNum ;
	YStep = Collision->XDivNum ;
	PostIndexZ = MinX + MinY * YStep + MinZ * ZStep ;
	MinPoly = NULL ;
	for( zc = MinZ ; zc <= MaxZ ; zc ++, PostIndexZ += ZStep )
	{
		PostIndexY = PostIndexZ ;
		for( yc = MinY ; yc <= MaxY ; yc ++, PostIndexY += YStep )
		{
			PostIndexX = PostIndexY ;
			for( xc = MinX ; xc <= MaxX ; xc ++, PostIndexX ++ )
			{
				for( ColPoly = Collision->Polygon[ PostIndexX ] ; ColPoly ; ColPoly = ColPoly->Next )
				{
					Poly = ColPoly->Polygon ;
					PolyIndex = ( int )( Poly - PolyList->Polygons ) ;

					// ���Ƀ`�F�b�N�ς݂̃|���S���������ꍇ�͉������Ȃ�
					if( BitBuffer[ PolyIndex >> 3 ] & ( 1 << ( PolyIndex & 7 ) ) ) continue ;

					// �ŏ��ɗ����̒P�ʂ̓����蔻����s��
					if( ( MinPos.x > Poly->MaxPosition.x ||
						  MinPos.y > Poly->MaxPosition.y ||
						  MinPos.z > Poly->MaxPosition.z ||
						  MaxPos.x < Poly->MinPosition.x ||
						  MaxPos.y < Poly->MinPosition.y ||
						  MaxPos.z < Poly->MinPosition.z ) == false )
					{
						// �O�p�`�Ɛ��̓����蔻��
						LineRes = HitCheck_Line_Triangle(
										*( ( VECTOR * )&PosStart ),
										*( ( VECTOR * )&PosEnd ),
										PolyList->Vertexs[ Poly->VIndex[ 0 ] ].Position,
										PolyList->Vertexs[ Poly->VIndex[ 1 ] ].Position,
										PolyList->Vertexs[ Poly->VIndex[ 2 ] ].Position ) ;
						if( LineRes.HitFlag )
						{
							// �����ő吔�ɒB���Ă�����Ċm��
							if( Result.HitNum == MaxNum )
							{
								MV1_COLL_RESULT_POLY *NewBuffer ;
								int NewMaxNum ;

								NewMaxNum = MaxNum * 2 ;
								NewBuffer = ( MV1_COLL_RESULT_POLY * )DXREALLOC( Result.Dim, sizeof( MV1_COLL_RESULT_POLY ) * NewMaxNum ) ;
								if( NewBuffer == NULL )
								{
									DXST_ERRORLOGFMT_ADDUTF16LE(( "\xe1\x30\xe2\x30\xea\x30\x0d\x4e\xb3\x8d\x6e\x30\xba\x70\x53\x5f\x5f\x30\x63\x30\x5f\x30\xdd\x30\xea\x30\xb4\x30\xf3\x30\x59\x30\x79\x30\x66\x30\x92\x30\xdd\x4f\x58\x5b\x59\x30\x8b\x30\x53\x30\x68\x30\x4c\x30\x67\x30\x4d\x30\x7e\x30\x5b\x30\x93\x30\x67\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�������s���̈ד��������|���S�����ׂĂ�ۑ����邱�Ƃ��ł��܂���ł���\n" @*/ )) ;
									goto END ;
								}

								// �V�����o�b�t�@�̕ۑ�
								Result.Dim = NewBuffer ;
								MaxNum = NewMaxNum ;
							}

							// �������Ă���|���S�����X�g�ɒǉ�����
							RPoly = &Result.Dim[ Result.HitNum ] ;
							Result.HitNum ++ ;

							RPoly->HitFlag = 1 ;
							RPoly->HitPosition = LineRes.Position ;

							RPoly->FrameIndex = Poly->FrameIndex ;
							RPoly->MaterialIndex = Poly->MaterialIndex ;
							RPoly->PolygonIndex = PolyIndex ;
							RPoly->Position[ 0 ] = PolyList->Vertexs[ Poly->VIndex[ 0 ] ].Position ;
							RPoly->Position[ 1 ] = PolyList->Vertexs[ Poly->VIndex[ 1 ] ].Position ;
							RPoly->Position[ 2 ] = PolyList->Vertexs[ Poly->VIndex[ 2 ] ].Position ;
							RPoly->Normal = VNorm( VCross( VSub( RPoly->Position[ 1 ], RPoly->Position[ 0 ] ), VSub( RPoly->Position[ 2 ], RPoly->Position[ 0 ] ) ) ) ; 
						}
					}

					// �`�F�b�N�ς݃t���O�𗧂Ă�
					BitBuffer[ PolyIndex >> 3 ] |= 1 << ( PolyIndex & 7 ) ;
				}
			}
		}
	}

	// ����������Ă��Ȃ������瓖�����Ă���|���S�����X�g�p�Ɋm�ۂ��������������
	if( Result.HitNum == 0 )
	{
		if( Result.Dim != NULL )
		{
			DXFREE( Result.Dim ) ;
			Result.Dim = NULL ;
		}
	}

END :

	// �������̉��
	if( BitBuffer )
	{
		DXFREE( BitBuffer ) ;
		BitBuffer = NULL ;
	}

	return Result ;
}

// ���ƃ��f���̓����蔻��
extern	MV1_COLL_RESULT_POLY_DIM NS_MV1CollCheck_Sphere( int MHandle, int FrameIndex, VECTOR CenterPos, float r )
{
	int MaxNum ;
	MV1_MODEL *Model ;
	MV1_MODEL_BASE *ModelBase ;
	MV1_FRAME *Frame ;
	MV1_COLL_RESULT_POLY_DIM Result ;
	MV1_COLL_RESULT_POLY *RPoly ;
	MV1_REF_POLYGONLIST *PolyList ;
	MV1_REF_POLYGON *Poly ;
	MV1_COLLISION *Collision ;
	MV1_COLL_POLYGON *ColPoly ;
	BYTE *BitBuffer = NULL ;
	VECTOR UnitDiv, MinPos, MaxPos, Sa, ResultPos ;
	int xc, yc, zc, ZStep, YStep, PolyIndex ;
	int MinX, MinY, MinZ, MaxX, MaxY, MaxZ, PostIndexZ, PostIndexY, PostIndexX ;

	// ���ʏ�����
	Result.HitNum = 0 ;
	Result.Dim = NULL ;

	// �A�h���X�擾
	if( MV1MDLCHK( MHandle, Model ) )
		return Result ;
	ModelBase = Model->BaseData ;

	// �t���[���P�̂����f���S�̂��ŏ����𕪊�
	if( FrameIndex == -1 )
	{
		// ���f���S�̂̏ꍇ

		// �R���W������񂪖���������Z�b�g�A�b�v����
		if( Model->Collision == NULL )
			if( NS_MV1RefreshCollInfo( MHandle, FrameIndex ) < 0 )
				return Result ;

		// �|���S�����̃Z�b�g
		PolyList = Model->RefPolygon[ 1 ][ 1 ] ;
		Collision = Model->Collision ;
	}
	else
	{
		// �t���[���P�̂̏ꍇ

		// �t���[���C���f�b�N�X�̃`�F�b�N
		if( FrameIndex < 0 || FrameIndex >= ModelBase->FrameNum )
			return Result ;
		Frame = Model->Frame + FrameIndex ;

		// �R���W������񂪖���������Z�b�g�A�b�v����
		if( Frame->Collision == NULL )
			if( NS_MV1RefreshCollInfo( MHandle, FrameIndex ) < 0 )
				return Result ;

		// �|���S�����̃Z�b�g
		PolyList = Frame->RefPolygon[ 1 ][ 1 ] ;
		Collision = Frame->Collision ;
	}

	// ���W�l�̍ŏ��l�ƍő�l���Z�b�g
	MaxPos.x = CenterPos.x + r ;
	MaxPos.y = CenterPos.y + r ;
	MaxPos.z = CenterPos.z + r ;
	MinPos.x = CenterPos.x - r ;
	MinPos.y = CenterPos.y - r ;
	MinPos.z = CenterPos.z - r ;

	// �͈͂��R���W������Ԃ���O��Ă�����ǂ̃|���S���ɂ�������Ȃ��Ƃ�������
	if( Collision->MaxPosition.x < MinPos.x ||
		Collision->MaxPosition.y < MinPos.y ||
		Collision->MaxPosition.z < MinPos.z ||
		Collision->MinPosition.x > MaxPos.x ||
		Collision->MinPosition.y > MaxPos.y ||
		Collision->MinPosition.z > MaxPos.z )
		goto END ;

	// �����o�b�t�@���m��
	MaxNum = 1000 ;
	Result.Dim = ( MV1_COLL_RESULT_POLY * )DXALLOC( sizeof( MV1_COLL_RESULT_POLY ) * MaxNum ) ;
	if( Result.Dim == NULL ) return Result ;

	// �����蔻����s�������ǂ�����ۑ����郁�����̈�̊m��
	BitBuffer = ( BYTE * )DXALLOC( sizeof( BYTE ) * ( ( PolyList->PolygonNum + 7 ) / 8 ) ) ;
	if( BitBuffer == NULL )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\x53\x5f\x5f\x30\x8a\x30\x24\x52\x9a\x5b\xe6\x51\x06\x74\x28\x75\xc6\x30\xf3\x30\xdd\x30\xe9\x30\xea\x30\xd0\x30\xc3\x30\xd5\x30\xa1\x30\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x32\x00\x0a\x00\x00"/*@ L"�����蔻�菈���p�e���|�����o�b�t�@�̊m�ۂɎ��s���܂���2\n" @*/ )) ;
		DXFREE( Result.Dim ) ;
		return Result ;
	}
	_MEMSET( BitBuffer, 0, sizeof( BYTE ) * ( ( PolyList->PolygonNum + 7 ) / 8 ) ) ;

	// �����͈͂�����
	UnitDiv = Collision->UnitSizeRev ;
	MinX = __FTOL( ( MinPos.x - Collision->MinPosition.x ) * UnitDiv.x ) ;
	MinY = __FTOL( ( MinPos.y - Collision->MinPosition.y ) * UnitDiv.y ) ;
	MinZ = __FTOL( ( MinPos.z - Collision->MinPosition.z ) * UnitDiv.z ) ;
	MaxX = __FTOL( ( MaxPos.x - Collision->MinPosition.x ) * UnitDiv.x ) + 1 ;
	MaxY = __FTOL( ( MaxPos.y - Collision->MinPosition.y ) * UnitDiv.y ) + 1 ;
	MaxZ = __FTOL( ( MaxPos.z - Collision->MinPosition.z ) * UnitDiv.z ) + 1 ;
	if( MinX < 0 ) MinX = 0 ;
	if( MinY < 0 ) MinY = 0 ;
	if( MinZ < 0 ) MinZ = 0 ;
	if( MaxX >= Collision->XDivNum ) MaxX = Collision->XDivNum - 1 ;
	if( MaxY >= Collision->YDivNum ) MaxY = Collision->YDivNum - 1 ;
	if( MaxZ >= Collision->ZDivNum ) MaxZ = Collision->ZDivNum - 1 ;

	// �͈͓��̑S�Ẵ|���S���Ɠ����蔻��
	ZStep = Collision->YDivNum * Collision->XDivNum ;
	YStep = Collision->XDivNum ;
	PostIndexZ = MinX + MinY * YStep + MinZ * ZStep ;
	for( zc = MinZ ; zc <= MaxZ ; zc ++, PostIndexZ += ZStep )
	{
		PostIndexY = PostIndexZ ;
		for( yc = MinY ; yc <= MaxY ; yc ++, PostIndexY += YStep )
		{
			PostIndexX = PostIndexY ;
			for( xc = MinX ; xc <= MaxX ; xc ++, PostIndexX ++ )
			{
				for( ColPoly = Collision->Polygon[ PostIndexX ] ; ColPoly ; ColPoly = ColPoly->Next )
				{
					Poly = ColPoly->Polygon ;
					PolyIndex = ( int )( Poly - PolyList->Polygons ) ;

					// ���Ƀ`�F�b�N�ς݂̃|���S���������ꍇ�͉������Ȃ�
					if( BitBuffer[ PolyIndex >> 3 ] & ( 1 << ( PolyIndex & 7 ) ) ) continue ;

					// �ŏ��ɗ����̒P�ʂ̓����蔻����s��
					if( ( MinPos.x > Poly->MaxPosition.x ||
						  MinPos.y > Poly->MaxPosition.y ||
						  MinPos.z > Poly->MaxPosition.z ||
						  MaxPos.x < Poly->MinPosition.x ||
						  MaxPos.y < Poly->MinPosition.y ||
						  MaxPos.z < Poly->MinPosition.z ) == false )
					{
						// �O�p�`�Ƌ��̓����蔻��

						// ���Ɉ�ԋ߂��O�p�`��̍��W�Ƃ̋������A���̔��a�ȉ��������瓖�����Ă���Ƃ�������
						ResultPos = Get_Triangle_Point_MinPosition( CenterPos, 
										PolyList->Vertexs[ Poly->VIndex[ 0 ] ].Position,
										PolyList->Vertexs[ Poly->VIndex[ 1 ] ].Position,
										PolyList->Vertexs[ Poly->VIndex[ 2 ] ].Position ) ;
						Sa.x = CenterPos.x - ResultPos.x ;
						Sa.y = CenterPos.y - ResultPos.y ;
						Sa.z = CenterPos.z - ResultPos.z ;
						if( Sa.x * Sa.x + Sa.y * Sa.y + Sa.z * Sa.z <= r * r )
						{
							// �����ő吔�ɒB���Ă�����Ċm��
							if( Result.HitNum == MaxNum )
							{
								MV1_COLL_RESULT_POLY *NewBuffer ;
								int NewMaxNum ;

								NewMaxNum = MaxNum * 2 ;
								NewBuffer = ( MV1_COLL_RESULT_POLY * )DXREALLOC( Result.Dim, sizeof( MV1_COLL_RESULT_POLY ) * NewMaxNum ) ;
								if( NewBuffer == NULL )
								{
									DXST_ERRORLOGFMT_ADDUTF16LE(( "\xe1\x30\xe2\x30\xea\x30\x0d\x4e\xb3\x8d\x6e\x30\xba\x70\x53\x5f\x5f\x30\x63\x30\x5f\x30\xdd\x30\xea\x30\xb4\x30\xf3\x30\x59\x30\x79\x30\x66\x30\x92\x30\xdd\x4f\x58\x5b\x59\x30\x8b\x30\x53\x30\x68\x30\x4c\x30\x67\x30\x4d\x30\x7e\x30\x5b\x30\x93\x30\x67\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�������s���̈ד��������|���S�����ׂĂ�ۑ����邱�Ƃ��ł��܂���ł���\n" @*/ )) ;
									goto END ;
								}

								// �V�����o�b�t�@�̕ۑ�
								Result.Dim = NewBuffer ;
								MaxNum = NewMaxNum ;
							}

							// �������Ă���|���S�����X�g�ɒǉ�����
							RPoly = &Result.Dim[ Result.HitNum ] ;
							Result.HitNum ++ ;

							RPoly->HitFlag = 1 ;
							RPoly->HitPosition = ResultPos ;

							RPoly->FrameIndex = Poly->FrameIndex ;
							RPoly->MaterialIndex = Poly->MaterialIndex ;
							RPoly->PolygonIndex = PolyIndex ;
							RPoly->Position[ 0 ] = PolyList->Vertexs[ Poly->VIndex[ 0 ] ].Position ;
							RPoly->Position[ 1 ] = PolyList->Vertexs[ Poly->VIndex[ 1 ] ].Position ;
							RPoly->Position[ 2 ] = PolyList->Vertexs[ Poly->VIndex[ 2 ] ].Position ;
							RPoly->Normal = VNorm( VCross( VSub( RPoly->Position[ 1 ], RPoly->Position[ 0 ] ), VSub( RPoly->Position[ 2 ], RPoly->Position[ 0 ] ) ) ) ; 
						}
					}

					// �`�F�b�N�ς݃t���O�𗧂Ă�
					BitBuffer[ PolyIndex >> 3 ] |= 1 << ( PolyIndex & 7 ) ;
				}
			}
		}
	}

	// ����������Ă��Ȃ������瓖�����Ă���|���S�����X�g�p�Ɋm�ۂ��������������
	if( Result.HitNum == 0 )
	{
		if( Result.Dim != NULL )
		{
			DXFREE( Result.Dim ) ;
			Result.Dim = NULL ;
		}
	}

END :

	// �������̉��
	if( BitBuffer )
	{
		DXFREE( BitBuffer ) ;
		BitBuffer = NULL ;
	}

	return Result ;
}

// �J�v�Z���ƃ��f���̓����蔻��
extern MV1_COLL_RESULT_POLY_DIM NS_MV1CollCheck_Capsule( int MHandle, int FrameIndex, VECTOR Pos1, VECTOR Pos2, float r )
{
	int MaxNum ;
	MV1_MODEL *Model ;
	MV1_MODEL_BASE *ModelBase ;
	MV1_FRAME *Frame ;
	MV1_COLL_RESULT_POLY_DIM Result ;
	MV1_COLL_RESULT_POLY *RPoly ;
	MV1_REF_POLYGONLIST *PolyList ;
	MV1_REF_POLYGON *Poly ;
	MV1_COLLISION *Collision ;
	MV1_COLL_POLYGON *ColPoly ;
	BYTE *BitBuffer = NULL ;
	VECTOR UnitDiv, MinPos, MaxPos/*, Sa, ResultPos */ ;
	int xc, yc, zc, ZStep, YStep, PolyIndex ;
	int MinX, MinY, MinZ, MaxX, MaxY, MaxZ, PostIndexZ, PostIndexY, PostIndexX ;

	// ���ʏ�����
	Result.HitNum = 0 ;
	Result.Dim = NULL ;

	// �A�h���X�擾
	if( MV1MDLCHK( MHandle, Model ) )
		return Result ;
	ModelBase = Model->BaseData ;

	// �t���[���P�̂����f���S�̂��ŏ����𕪊�
	if( FrameIndex == -1 )
	{
		// ���f���S�̂̏ꍇ

		// �R���W������񂪖���������Z�b�g�A�b�v����
		if( Model->Collision == NULL )
			if( NS_MV1RefreshCollInfo( MHandle, FrameIndex ) < 0 )
				return Result ;

		// �|���S�����̃Z�b�g
		PolyList = Model->RefPolygon[ 1 ][ 1 ] ;
		Collision = Model->Collision ;
	}
	else
	{
		// �t���[���P�̂̏ꍇ

		// �t���[���C���f�b�N�X�̃`�F�b�N
		if( FrameIndex < 0 || FrameIndex >= ModelBase->FrameNum )
			return Result ;
		Frame = Model->Frame + FrameIndex ;

		// �R���W������񂪖���������Z�b�g�A�b�v����
		if( Frame->Collision == NULL )
			if( NS_MV1RefreshCollInfo( MHandle, FrameIndex ) < 0 )
				return Result ;

		// �|���S�����̃Z�b�g
		PolyList = Frame->RefPolygon[ 1 ][ 1 ] ;
		Collision = Frame->Collision ;
	}

	// ���W�l�̍ŏ��l�ƍő�l���Z�b�g
	if( Pos1.x < Pos2.x )
	{
		MaxPos.x = Pos2.x + r ;
		MinPos.x = Pos1.x - r ;
	}
	else
	{
		MaxPos.x = Pos1.x + r ;
		MinPos.x = Pos2.x - r ;
	}
	if( Pos1.y < Pos2.y )
	{
		MaxPos.y = Pos2.y + r ;
		MinPos.y = Pos1.y - r ;
	}
	else
	{
		MaxPos.y = Pos1.y + r ;
		MinPos.y = Pos2.y - r ;
	}
	if( Pos1.z < Pos2.z )
	{
		MaxPos.z = Pos2.z + r ;
		MinPos.z = Pos1.z - r ;
	}
	else
	{
		MaxPos.z = Pos1.z + r ;
		MinPos.z = Pos2.z - r ;
	}

	// �͈͂��R���W������Ԃ���O��Ă�����ǂ̃|���S���ɂ�������Ȃ��Ƃ�������
	if( Collision->MaxPosition.x < MinPos.x ||
		Collision->MaxPosition.y < MinPos.y ||
		Collision->MaxPosition.z < MinPos.z ||
		Collision->MinPosition.x > MaxPos.x ||
		Collision->MinPosition.y > MaxPos.y ||
		Collision->MinPosition.z > MaxPos.z )
		goto END ;

	// �����o�b�t�@���m��
	MaxNum = 1000 ;
	Result.Dim = ( MV1_COLL_RESULT_POLY * )DXALLOC( sizeof( MV1_COLL_RESULT_POLY ) * MaxNum ) ;
	if( Result.Dim == NULL ) return Result ;

	// �����蔻����s�������ǂ�����ۑ����郁�����̈�̊m��
	BitBuffer = ( BYTE * )DXALLOC( sizeof( BYTE ) * ( ( PolyList->PolygonNum + 7 ) / 8 ) ) ;
	if( BitBuffer == NULL )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\x53\x5f\x5f\x30\x8a\x30\x24\x52\x9a\x5b\xe6\x51\x06\x74\x28\x75\xc6\x30\xf3\x30\xdd\x30\xe9\x30\xea\x30\xd0\x30\xc3\x30\xd5\x30\xa1\x30\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x32\x00\x0a\x00\x00"/*@ L"�����蔻�菈���p�e���|�����o�b�t�@�̊m�ۂɎ��s���܂���2\n" @*/ )) ;
		DXFREE( Result.Dim ) ;
		return Result ;
	}
	_MEMSET( BitBuffer, 0, sizeof( BYTE ) * ( ( PolyList->PolygonNum + 7 ) / 8 ) ) ;

	// �����͈͂�����
	UnitDiv = Collision->UnitSizeRev ;
	MinX = __FTOL( ( MinPos.x - Collision->MinPosition.x ) * UnitDiv.x ) ;
	MinY = __FTOL( ( MinPos.y - Collision->MinPosition.y ) * UnitDiv.y ) ;
	MinZ = __FTOL( ( MinPos.z - Collision->MinPosition.z ) * UnitDiv.z ) ;
	MaxX = __FTOL( ( MaxPos.x - Collision->MinPosition.x ) * UnitDiv.x ) + 1 ;
	MaxY = __FTOL( ( MaxPos.y - Collision->MinPosition.y ) * UnitDiv.y ) + 1 ;
	MaxZ = __FTOL( ( MaxPos.z - Collision->MinPosition.z ) * UnitDiv.z ) + 1 ;
	if( MinX < 0 ) MinX = 0 ;
	if( MinY < 0 ) MinY = 0 ;
	if( MinZ < 0 ) MinZ = 0 ;
	if( MaxX >= Collision->XDivNum ) MaxX = Collision->XDivNum - 1 ;
	if( MaxY >= Collision->YDivNum ) MaxY = Collision->YDivNum - 1 ;
	if( MaxZ >= Collision->ZDivNum ) MaxZ = Collision->ZDivNum - 1 ;

	// �͈͓��̑S�Ẵ|���S���Ɠ����蔻��
	ZStep = Collision->YDivNum * Collision->XDivNum ;
	YStep = Collision->XDivNum ;
	PostIndexZ = MinX + MinY * YStep + MinZ * ZStep ;
	for( zc = MinZ ; zc <= MaxZ ; zc ++, PostIndexZ += ZStep )
	{
		PostIndexY = PostIndexZ ;
		for( yc = MinY ; yc <= MaxY ; yc ++, PostIndexY += YStep )
		{
			PostIndexX = PostIndexY ;
			for( xc = MinX ; xc <= MaxX ; xc ++, PostIndexX ++ )
			{
				for( ColPoly = Collision->Polygon[ PostIndexX ] ; ColPoly ; ColPoly = ColPoly->Next )
				{
					Poly = ColPoly->Polygon ;
					PolyIndex = ( int )( Poly - PolyList->Polygons ) ;

					// ���Ƀ`�F�b�N�ς݂̃|���S���������ꍇ�͉������Ȃ�
					if( BitBuffer[ PolyIndex >> 3 ] & ( 1 << ( PolyIndex & 7 ) ) ) continue ;

					// �ŏ��ɗ����̒P�ʂ̓����蔻����s��
					if( ( MinPos.x > Poly->MaxPosition.x ||
						  MinPos.y > Poly->MaxPosition.y ||
						  MinPos.z > Poly->MaxPosition.z ||
						  MaxPos.x < Poly->MinPosition.x ||
						  MaxPos.y < Poly->MinPosition.y ||
						  MaxPos.z < Poly->MinPosition.z ) == false )
					{
						// �O�p�`�Ƌ��̓����蔻��

						// ���Ɉ�ԋ߂��O�p�`��̍��W�Ƃ̋������A���̔��a�ȉ��������瓖�����Ă���Ƃ�������
/*						ResultPos = Get_Triangle_Point_MinPosition( CenterPos, 
										PolyList->Vertexs[ Poly->VIndex[ 0 ] ].Position,
										PolyList->Vertexs[ Poly->VIndex[ 1 ] ].Position,
										PolyList->Vertexs[ Poly->VIndex[ 2 ] ].Position ) ;
						Sa.x = CenterPos.x - ResultPos.x ;
						Sa.y = CenterPos.y - ResultPos.y ;
						Sa.z = CenterPos.z - ResultPos.z ;
						if( Sa.x * Sa.x + Sa.y * Sa.y + Sa.z * Sa.z <= r * r )
*/						if( Segment_Triangle_MinLength_Square(
							Pos1, Pos2, 
							PolyList->Vertexs[ Poly->VIndex[ 0 ] ].Position,
							PolyList->Vertexs[ Poly->VIndex[ 1 ] ].Position,
							PolyList->Vertexs[ Poly->VIndex[ 2 ] ].Position ) <= r * r )
						{
							// �����ő吔�ɒB���Ă�����Ċm��
							if( Result.HitNum == MaxNum )
							{
								MV1_COLL_RESULT_POLY *NewBuffer ;
								int NewMaxNum ;

								NewMaxNum = MaxNum * 2 ;
								NewBuffer = ( MV1_COLL_RESULT_POLY * )DXREALLOC( Result.Dim, sizeof( MV1_COLL_RESULT_POLY ) * NewMaxNum ) ;
								if( NewBuffer == NULL )
								{
									DXST_ERRORLOGFMT_ADDUTF16LE(( "\xe1\x30\xe2\x30\xea\x30\x0d\x4e\xb3\x8d\x6e\x30\xba\x70\x53\x5f\x5f\x30\x63\x30\x5f\x30\xdd\x30\xea\x30\xb4\x30\xf3\x30\x59\x30\x79\x30\x66\x30\x92\x30\xdd\x4f\x58\x5b\x59\x30\x8b\x30\x53\x30\x68\x30\x4c\x30\x67\x30\x4d\x30\x7e\x30\x5b\x30\x93\x30\x67\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�������s���̈ד��������|���S�����ׂĂ�ۑ����邱�Ƃ��ł��܂���ł���\n" @*/ )) ;
									goto END ;
								}

								// �V�����o�b�t�@�̕ۑ�
								Result.Dim = NewBuffer ;
								MaxNum = NewMaxNum ;
							}

							// �������Ă���|���S�����X�g�ɒǉ�����
							RPoly = &Result.Dim[ Result.HitNum ] ;
							Result.HitNum ++ ;

							RPoly->HitFlag = 1 ;
//							RPoly->HitPosition = ResultPos ;
							RPoly->HitPosition.x = 0.0f ;
							RPoly->HitPosition.y = 0.0f ;
							RPoly->HitPosition.z = 0.0f ;

							RPoly->FrameIndex = Poly->FrameIndex ;
							RPoly->MaterialIndex = Poly->MaterialIndex ;
							RPoly->PolygonIndex = PolyIndex ;
							RPoly->Position[ 0 ] = PolyList->Vertexs[ Poly->VIndex[ 0 ] ].Position ;
							RPoly->Position[ 1 ] = PolyList->Vertexs[ Poly->VIndex[ 1 ] ].Position ;
							RPoly->Position[ 2 ] = PolyList->Vertexs[ Poly->VIndex[ 2 ] ].Position ;
							RPoly->Normal = VNorm( VCross( VSub( RPoly->Position[ 1 ], RPoly->Position[ 0 ] ), VSub( RPoly->Position[ 2 ], RPoly->Position[ 0 ] ) ) ) ; 
						}
					}

					// �`�F�b�N�ς݃t���O�𗧂Ă�
					BitBuffer[ PolyIndex >> 3 ] |= 1 << ( PolyIndex & 7 ) ;
				}
			}
		}
	}

	// ����������Ă��Ȃ������瓖�����Ă���|���S�����X�g�p�Ɋm�ۂ��������������
	if( Result.HitNum == 0 )
	{
		if( Result.Dim != NULL )
		{
			DXFREE( Result.Dim ) ;
			Result.Dim = NULL ;
		}
	}

END :

	// �������̉��
	if( BitBuffer )
	{
		DXFREE( BitBuffer ) ;
		BitBuffer = NULL ;
	}

	return Result ;
}

// �O�p�`�ƃ��f���̓����蔻��
extern MV1_COLL_RESULT_POLY_DIM NS_MV1CollCheck_Triangle( int MHandle, int FrameIndex, VECTOR Pos1, VECTOR Pos2, VECTOR Pos3 )
{
	int MaxNum ;
	MV1_MODEL *Model ;
	MV1_MODEL_BASE *ModelBase ;
	MV1_FRAME *Frame ;
	MV1_COLL_RESULT_POLY_DIM Result ;
	MV1_COLL_RESULT_POLY *RPoly ;
	MV1_REF_POLYGONLIST *PolyList ;
	MV1_REF_POLYGON *Poly ;
	MV1_COLLISION *Collision ;
	MV1_COLL_POLYGON *ColPoly ;
	BYTE *BitBuffer = NULL ;
	VECTOR UnitDiv, MinPos, MaxPos/*, Sa, ResultPos */ ;
	int xc, yc, zc, ZStep, YStep, PolyIndex ;
	int MinX, MinY, MinZ, MaxX, MaxY, MaxZ, PostIndexZ, PostIndexY, PostIndexX ;

	// ���ʏ�����
	Result.HitNum = 0 ;
	Result.Dim = NULL ;

	// �A�h���X�擾
	if( MV1MDLCHK( MHandle, Model ) )
		return Result ;
	ModelBase = Model->BaseData ;

	// �t���[���P�̂����f���S�̂��ŏ����𕪊�
	if( FrameIndex == -1 )
	{
		// ���f���S�̂̏ꍇ

		// �R���W������񂪖���������Z�b�g�A�b�v����
		if( Model->Collision == NULL )
			if( NS_MV1RefreshCollInfo( MHandle, FrameIndex ) < 0 )
				return Result ;

		// �|���S�����̃Z�b�g
		PolyList = Model->RefPolygon[ 1 ][ 1 ] ;
		Collision = Model->Collision ;
	}
	else
	{
		// �t���[���P�̂̏ꍇ

		// �t���[���C���f�b�N�X�̃`�F�b�N
		if( FrameIndex < 0 || FrameIndex >= ModelBase->FrameNum )
			return Result ;
		Frame = Model->Frame + FrameIndex ;

		// �R���W������񂪖���������Z�b�g�A�b�v����
		if( Frame->Collision == NULL )
			if( NS_MV1RefreshCollInfo( MHandle, FrameIndex ) < 0 )
				return Result ;

		// �|���S�����̃Z�b�g
		PolyList = Frame->RefPolygon[ 1 ][ 1 ] ;
		Collision = Frame->Collision ;
	}

	// ���W�l�̍ŏ��l�ƍő�l���Z�b�g
	if( Pos1.x < Pos2.x )
	{
		if( Pos1.x < Pos3.x )
		{
			MinPos.x = Pos1.x ;
		}
		else
		{
			MinPos.x = Pos3.x ;
		}

		if( Pos2.x > Pos3.x )
		{
			MaxPos.x = Pos2.x ;
		}
		else
		{
			MaxPos.x = Pos3.x ;
		}
	}
	else
	{
		if( Pos2.x < Pos3.x )
		{
			MinPos.x = Pos2.x ;
		}
		else
		{
			MinPos.x = Pos3.x ;
		}

		if( Pos1.x > Pos3.x )
		{
			MaxPos.x = Pos1.x ;
		}
		else
		{
			MaxPos.x = Pos3.x ;
		}
	}

	if( Pos1.y < Pos2.y )
	{
		if( Pos1.y < Pos3.y )
		{
			MinPos.y = Pos1.y ;
		}
		else
		{
			MinPos.y = Pos3.y ;
		}

		if( Pos2.y > Pos3.y )
		{
			MaxPos.y = Pos2.y ;
		}
		else
		{
			MaxPos.y = Pos3.y ;
		}
	}
	else
	{
		if( Pos2.y < Pos3.y )
		{
			MinPos.y = Pos2.y ;
		}
		else
		{
			MinPos.y = Pos3.y ;
		}

		if( Pos1.y > Pos3.y )
		{
			MaxPos.y = Pos1.y ;
		}
		else
		{
			MaxPos.y = Pos3.y ;
		}
	}

	if( Pos1.z < Pos2.z )
	{
		if( Pos1.z < Pos3.z )
		{
			MinPos.z = Pos1.z ;
		}
		else
		{
			MinPos.z = Pos3.z ;
		}

		if( Pos2.z > Pos3.z )
		{
			MaxPos.z = Pos2.z ;
		}
		else
		{
			MaxPos.z = Pos3.z ;
		}
	}
	else
	{
		if( Pos2.z < Pos3.z )
		{
			MinPos.z = Pos2.z ;
		}
		else
		{
			MinPos.z = Pos3.z ;
		}

		if( Pos1.z > Pos3.z )
		{
			MaxPos.z = Pos1.z ;
		}
		else
		{
			MaxPos.z = Pos3.z ;
		}
	}

	// �͈͂��R���W������Ԃ���O��Ă�����ǂ̃|���S���ɂ�������Ȃ��Ƃ�������
	if( Collision->MaxPosition.x < MinPos.x ||
		Collision->MaxPosition.y < MinPos.y ||
		Collision->MaxPosition.z < MinPos.z ||
		Collision->MinPosition.x > MaxPos.x ||
		Collision->MinPosition.y > MaxPos.y ||
		Collision->MinPosition.z > MaxPos.z )
		goto END ;

	// �����o�b�t�@���m��
	MaxNum = 1000 ;
	Result.Dim = ( MV1_COLL_RESULT_POLY * )DXALLOC( sizeof( MV1_COLL_RESULT_POLY ) * MaxNum ) ;
	if( Result.Dim == NULL ) return Result ;

	// �����蔻����s�������ǂ�����ۑ����郁�����̈�̊m��
	BitBuffer = ( BYTE * )DXALLOC( sizeof( BYTE ) * ( ( PolyList->PolygonNum + 7 ) / 8 ) ) ;
	if( BitBuffer == NULL )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\x53\x5f\x5f\x30\x8a\x30\x24\x52\x9a\x5b\xe6\x51\x06\x74\x28\x75\xc6\x30\xf3\x30\xdd\x30\xe9\x30\xea\x30\xd0\x30\xc3\x30\xd5\x30\xa1\x30\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x32\x00\x0a\x00\x00"/*@ L"�����蔻�菈���p�e���|�����o�b�t�@�̊m�ۂɎ��s���܂���2\n" @*/ )) ;
		DXFREE( Result.Dim ) ;
		return Result ;
	}
	_MEMSET( BitBuffer, 0, sizeof( BYTE ) * ( ( PolyList->PolygonNum + 7 ) / 8 ) ) ;

	// �����͈͂�����
	UnitDiv = Collision->UnitSizeRev ;
	MinX = __FTOL( ( MinPos.x - Collision->MinPosition.x ) * UnitDiv.x ) ;
	MinY = __FTOL( ( MinPos.y - Collision->MinPosition.y ) * UnitDiv.y ) ;
	MinZ = __FTOL( ( MinPos.z - Collision->MinPosition.z ) * UnitDiv.z ) ;
	MaxX = __FTOL( ( MaxPos.x - Collision->MinPosition.x ) * UnitDiv.x ) + 1 ;
	MaxY = __FTOL( ( MaxPos.y - Collision->MinPosition.y ) * UnitDiv.y ) + 1 ;
	MaxZ = __FTOL( ( MaxPos.z - Collision->MinPosition.z ) * UnitDiv.z ) + 1 ;
	if( MinX < 0 ) MinX = 0 ;
	if( MinY < 0 ) MinY = 0 ;
	if( MinZ < 0 ) MinZ = 0 ;
	if( MaxX >= Collision->XDivNum ) MaxX = Collision->XDivNum - 1 ;
	if( MaxY >= Collision->YDivNum ) MaxY = Collision->YDivNum - 1 ;
	if( MaxZ >= Collision->ZDivNum ) MaxZ = Collision->ZDivNum - 1 ;

	// �͈͓��̑S�Ẵ|���S���Ɠ����蔻��
	ZStep = Collision->YDivNum * Collision->XDivNum ;
	YStep = Collision->XDivNum ;
	PostIndexZ = MinX + MinY * YStep + MinZ * ZStep ;
	for( zc = MinZ ; zc <= MaxZ ; zc ++, PostIndexZ += ZStep )
	{
		PostIndexY = PostIndexZ ;
		for( yc = MinY ; yc <= MaxY ; yc ++, PostIndexY += YStep )
		{
			PostIndexX = PostIndexY ;
			for( xc = MinX ; xc <= MaxX ; xc ++, PostIndexX ++ )
			{
				for( ColPoly = Collision->Polygon[ PostIndexX ] ; ColPoly ; ColPoly = ColPoly->Next )
				{
					Poly = ColPoly->Polygon ;
					PolyIndex = ( int )( Poly - PolyList->Polygons ) ;

					// ���Ƀ`�F�b�N�ς݂̃|���S���������ꍇ�͉������Ȃ�
					if( BitBuffer[ PolyIndex >> 3 ] & ( 1 << ( PolyIndex & 7 ) ) ) continue ;

					// �ŏ��ɗ����̒P�ʂ̓����蔻����s��
					if( ( MinPos.x > Poly->MaxPosition.x ||
						  MinPos.y > Poly->MaxPosition.y ||
						  MinPos.z > Poly->MaxPosition.z ||
						  MaxPos.x < Poly->MinPosition.x ||
						  MaxPos.y < Poly->MinPosition.y ||
						  MaxPos.z < Poly->MinPosition.z ) == false )
					{
						// �O�p�`�ƎO�p�`�̓����蔻��
						if( HitCheck_Triangle_Triangle(
							Pos1, Pos2, Pos3, 
							PolyList->Vertexs[ Poly->VIndex[ 0 ] ].Position,
							PolyList->Vertexs[ Poly->VIndex[ 1 ] ].Position,
							PolyList->Vertexs[ Poly->VIndex[ 2 ] ].Position ) )
						{
							// �����ő吔�ɒB���Ă�����Ċm��
							if( Result.HitNum == MaxNum )
							{
								MV1_COLL_RESULT_POLY *NewBuffer ;
								int NewMaxNum ;

								NewMaxNum = MaxNum * 2 ;
								NewBuffer = ( MV1_COLL_RESULT_POLY * )DXREALLOC( Result.Dim, sizeof( MV1_COLL_RESULT_POLY ) * NewMaxNum ) ;
								if( NewBuffer == NULL )
								{
									DXST_ERRORLOGFMT_ADDUTF16LE(( "\xe1\x30\xe2\x30\xea\x30\x0d\x4e\xb3\x8d\x6e\x30\xba\x70\x53\x5f\x5f\x30\x63\x30\x5f\x30\xdd\x30\xea\x30\xb4\x30\xf3\x30\x59\x30\x79\x30\x66\x30\x92\x30\xdd\x4f\x58\x5b\x59\x30\x8b\x30\x53\x30\x68\x30\x4c\x30\x67\x30\x4d\x30\x7e\x30\x5b\x30\x93\x30\x67\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�������s���̈ד��������|���S�����ׂĂ�ۑ����邱�Ƃ��ł��܂���ł���\n" @*/ )) ;
									goto END ;
								}

								// �V�����o�b�t�@�̕ۑ�
								Result.Dim = NewBuffer ;
								MaxNum = NewMaxNum ;
							}

							// �������Ă���|���S�����X�g�ɒǉ�����
							RPoly = &Result.Dim[ Result.HitNum ] ;
							Result.HitNum ++ ;

							RPoly->HitFlag = 1 ;
							RPoly->HitPosition.x = 0.0f ;
							RPoly->HitPosition.y = 0.0f ;
							RPoly->HitPosition.z = 0.0f ;

							RPoly->FrameIndex = Poly->FrameIndex ;
							RPoly->MaterialIndex = Poly->MaterialIndex ;
							RPoly->PolygonIndex = PolyIndex ;
							RPoly->Position[ 0 ] = PolyList->Vertexs[ Poly->VIndex[ 0 ] ].Position ;
							RPoly->Position[ 1 ] = PolyList->Vertexs[ Poly->VIndex[ 1 ] ].Position ;
							RPoly->Position[ 2 ] = PolyList->Vertexs[ Poly->VIndex[ 2 ] ].Position ;
							RPoly->Normal = VNorm( VCross( VSub( RPoly->Position[ 1 ], RPoly->Position[ 0 ] ), VSub( RPoly->Position[ 2 ], RPoly->Position[ 0 ] ) ) ) ; 
						}
					}

					// �`�F�b�N�ς݃t���O�𗧂Ă�
					BitBuffer[ PolyIndex >> 3 ] |= 1 << ( PolyIndex & 7 ) ;
				}
			}
		}
	}

END :

	// ����������Ă��Ȃ������瓖�����Ă���|���S�����X�g�p�Ɋm�ۂ��������������
	if( Result.HitNum == 0 )
	{
		if( Result.Dim != NULL )
		{
			DXFREE( Result.Dim ) ;
			Result.Dim = NULL ;
		}
	}

	// �������̉��
	if( BitBuffer )
	{
		DXFREE( BitBuffer ) ;
		BitBuffer = NULL ;
	}

	return Result ;
}

// �R���W�������ʃ|���S���z�񂩂�w��ԍ��̃|���S�������擾����
extern MV1_COLL_RESULT_POLY NS_MV1CollCheck_GetResultPoly( MV1_COLL_RESULT_POLY_DIM ResultPolyDim, int PolyNo )
{
	MV1_COLL_RESULT_POLY ErrorResult ;

	if( PolyNo < 0 || PolyNo >= ResultPolyDim.HitNum || ResultPolyDim.Dim == NULL )
	{
		_MEMSET( &ErrorResult, 0, sizeof( ErrorResult ) ) ;
		return ErrorResult ;
	}

	return ResultPolyDim.Dim[ PolyNo ] ;
}

// �R���W�������ʃ|���S���z��̌�n��������
extern	int NS_MV1CollResultPolyDimTerminate( MV1_COLL_RESULT_POLY_DIM ResultPolyDim )
{
	if( ResultPolyDim.Dim != NULL )
	{
		DXFREE( ResultPolyDim.Dim ) ;
		ResultPolyDim.Dim = NULL ;
	}
	ResultPolyDim.HitNum = 0 ;

	// �I��
	return 0 ;
}

// �Q�Ɨp���b�V���̃Z�b�g�A�b�v
extern int NS_MV1SetupReferenceMesh( int MHandle, int FrameIndex, int IsTransform, int IsPositionOnly )
{
	int i ;
	MV1_MODEL *Model ;
	MV1_MODEL_BASE *ModelBase ;
	MV1_FRAME *Frame ;
	int VertexNum ;

	IsTransform = IsTransform != 0 ? 1 : 0 ;
	IsPositionOnly = IsPositionOnly != 0 ? 1 : 0 ;

	// �A�h���X�擾
	if( MV1MDLCHK( MHandle, Model ) )
		return -1 ;
	ModelBase = Model->BaseData ;

	// �t���[���P�̂����f���S�̂��ŏ����𕪊�
	if( FrameIndex == -1 )
	{
		// ���f���S�̂̏ꍇ

		// �Q�ƃ|���S�����i�[���郁�����̈�̊m��
		// �m�ۂ���Ă��Ȃ��ꍇ�̂݊m��
		if( Model->RefPolygon[ IsTransform ][ IsPositionOnly ] == NULL )
		{
			if( IsPositionOnly )
			{
				MV1_FRAME_BASE *FrameBase ;

				FrameBase = ModelBase->Frame ;
				VertexNum = 0 ;
				for( i = 0 ; i < ModelBase->FrameNum ; i ++, FrameBase ++ )
				{
					VertexNum += FrameBase->PositionNum ;
				}
			}
			else
			{
				VertexNum = ModelBase->TriangleListVertexNum ;
			}
			Model->RefPolygon[ IsTransform ][ IsPositionOnly ] = ( MV1_REF_POLYGONLIST * )DXALLOC( sizeof( MV1_REF_POLYGONLIST ) + sizeof( MV1_REF_POLYGON ) * ModelBase->TriangleNum + sizeof( MV1_REF_VERTEX ) * VertexNum ) ;
			if( Model->RefPolygon[ IsTransform ][ IsPositionOnly ] == NULL )
			{
				DXST_ERRORLOGFMT_ADDUTF16LE(( "\xc2\x53\x67\x71\x28\x75\xdd\x30\xea\x30\xb4\x30\xf3\x30\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�Q�Ɨp�|���S�����i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
				return -1 ;
			}
			Model->RefPolygon[ IsTransform ][ IsPositionOnly ]->Polygons = ( MV1_REF_POLYGON * )( Model->RefPolygon[ IsTransform ][ IsPositionOnly ] + 1 ) ;
			Model->RefPolygon[ IsTransform ][ IsPositionOnly ]->Vertexs  = ( MV1_REF_VERTEX  * )( Model->RefPolygon[ IsTransform ][ IsPositionOnly ]->Polygons + ModelBase->TriangleNum ) ;

			// �|���S�����͍ŏ��ɃZ�b�g�A�b�v���Ă���
			Model->RefPolygon[ IsTransform ][ IsPositionOnly ]->PolygonNum = 0 ;
			Model->RefPolygon[ IsTransform ][ IsPositionOnly ]->VertexNum = 0 ;
			for( i = 0 ; i < ModelBase->FrameNum ; i ++ )
			{
				MV1SetupReferenceMeshFrame( Model, ModelBase, Model->Frame + i, Model->RefPolygon[ IsTransform ][ IsPositionOnly ], 0, IsTransform ? true : false, IsPositionOnly ? true : false ) ;
			}

			// �ŏ��l�ƍő�l���v�Z���Ă���
			if( IsTransform == false )
			{
				_MV1SetupReferenceMeshMaxAndMinPosition( Model->RefPolygon[ IsTransform ][ IsPositionOnly ] ) ;
			}
		}
/*
		// �Q�ƃ|���S�����i�[���郁�����̈�̊m��
		if( IsTransform )
		{
			// �m�ۂ���Ă��Ȃ��ꍇ�̂݊m��
			if( Model->TransformPolygon == NULL )
			{
				Model->TransformPolygon = ( MV1_REF_POLYGONLIST * )DXALLOC( sizeof( MV1_REF_POLYGONLIST ) + sizeof( MV1_REF_POLYGON ) * ModelBase->TriangleNum + sizeof( MV1_REF_VERTEX ) * ModelBase->TriangleListVertexNum ) ;
				if( Model->TransformPolygon == NULL )
				{
					DXST_ERRORLOGFMT_ADDW(( L"�Q�Ɨp�|���S�����i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" )) ;
					return -1 ;
				}
				Model->TransformPolygon->Polygons = ( MV1_REF_POLYGON * )( Model->TransformPolygon + 1 ) ;
				Model->TransformPolygon->Vertexs = ( MV1_REF_VERTEX * )( Model->TransformPolygon->Polygons + ModelBase->TriangleNum ) ;

				// �|���S�����͍ŏ��ɃZ�b�g�A�b�v���Ă���
				Model->TransformPolygon->PolygonNum = 0 ;
				Model->TransformPolygon->VertexNum = 0 ;
				for( i = 0 ; i < ModelBase->FrameNum ; i ++ )
				{
					MV1SetupReferenceMeshFrame( Model, ModelBase, Model->Frame + i, Model->TransformPolygon, 0, IsTransform ? true : false ) ;
				}
			}
		}
		else
		{
			// �m�ۂ���Ă��Ȃ��ꍇ�̂݊m��
			if( Model->NonTransformPolygon == NULL )
			{
				Model->NonTransformPolygon = ( MV1_REF_POLYGONLIST * )DXALLOC( sizeof( MV1_REF_POLYGONLIST ) + sizeof( MV1_REF_POLYGON ) * ModelBase->TriangleNum + sizeof( MV1_REF_VERTEX ) * ModelBase->TriangleListVertexNum ) ;
				if( Model->NonTransformPolygon == NULL )
				{
					DXST_ERRORLOGFMT_ADDW(( L"�Q�Ɨp�|���S�����i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" )) ;
					return -1 ;
				}
				Model->NonTransformPolygon->Polygons = ( MV1_REF_POLYGON * )( Model->NonTransformPolygon + 1 ) ;
				Model->NonTransformPolygon->Vertexs = ( MV1_REF_VERTEX * )( Model->NonTransformPolygon->Polygons + ModelBase->TriangleNum ) ;

				// �|���S�����͍ŏ��ɃZ�b�g�A�b�v���Ă���
				Model->NonTransformPolygon->PolygonNum = 0 ;
				Model->NonTransformPolygon->VertexNum = 0 ;
				for( i = 0 ; i < ModelBase->FrameNum ; i ++ )
				{
					MV1SetupReferenceMeshFrame( Model, ModelBase, Model->Frame + i, Model->NonTransformPolygon, 0, IsTransform ? true : false ) ;
				}

				// �ŏ��l�ƍő�l���v�Z���Ă���
				_MV1SetupReferenceMeshMaxAndMinPosition( Model->NonTransformPolygon ) ;
			}
		}
*/
	}
	else
	{
		// �t���[���C���f�b�N�X�̃`�F�b�N
		if( FrameIndex < 0 || FrameIndex >= ModelBase->FrameNum )
			return -1 ;
		Frame = Model->Frame + FrameIndex ;

		// �Q�ƃ|���S�����i�[���郁�����̈�̊m��
		// �m�ۂ���Ă��Ȃ��ꍇ�̂݊m��
		if( Frame->RefPolygon[ IsTransform ][ IsPositionOnly ] == NULL )
		{
			if( IsPositionOnly )
			{
				VertexNum = Frame->BaseData->PositionNum ;
			}
			else
			{
				VertexNum = Frame->BaseData->VertexNum ;
			}
			Frame->RefPolygon[ IsTransform ][ IsPositionOnly ] = ( MV1_REF_POLYGONLIST * )DXALLOC( sizeof( MV1_REF_POLYGONLIST ) + sizeof( MV1_REF_POLYGON ) * Frame->BaseData->TriangleNum + sizeof( MV1_REF_VERTEX ) * VertexNum ) ;
			if( Frame->RefPolygon[ IsTransform ][ IsPositionOnly ] == NULL )
			{
				DXST_ERRORLOGFMT_ADDUTF16LE(( "\xc2\x53\x67\x71\x28\x75\xdd\x30\xea\x30\xb4\x30\xf3\x30\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�Q�Ɨp�|���S�����i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
				return -1 ;
			}
			Frame->RefPolygon[ IsTransform ][ IsPositionOnly ]->Polygons = ( MV1_REF_POLYGON * )( Frame->RefPolygon[ IsTransform ][ IsPositionOnly ] + 1 ) ;
			Frame->RefPolygon[ IsTransform ][ IsPositionOnly ]->Vertexs = ( MV1_REF_VERTEX * )( Frame->RefPolygon[ IsTransform ][ IsPositionOnly ]->Polygons + Frame->BaseData->TriangleNum ) ;

			// �|���S�����͍ŏ��ɃZ�b�g�A�b�v���Ă���
			Frame->RefPolygon[ IsTransform ][ IsPositionOnly ]->PolygonNum = 0 ;
			Frame->RefPolygon[ IsTransform ][ IsPositionOnly ]->VertexNum = 0 ;
			MV1SetupReferenceMeshFrame( Model, ModelBase, Frame, Frame->RefPolygon[ IsTransform ][ IsPositionOnly ], 1, IsTransform ? true : false, IsPositionOnly ? true : false ) ;

			// �ŏ��l�ƍő�l���v�Z���Ă���
			if( IsTransform == false )
			{
				_MV1SetupReferenceMeshMaxAndMinPosition( Frame->RefPolygon[ IsTransform ][ IsPositionOnly ] ) ;
			}
		}
/*
		// �Q�ƃ|���S�����i�[���郁�����̈�̊m��
		if( IsTransform )
		{
			// �m�ۂ���Ă��Ȃ��ꍇ�̂݊m��
			if( Frame->TransformPolygon == NULL )
			{
				Frame->TransformPolygon = ( MV1_REF_POLYGONLIST * )DXALLOC( sizeof( MV1_REF_POLYGONLIST ) + sizeof( MV1_REF_POLYGON ) * Frame->BaseData->TriangleNum + sizeof( MV1_REF_VERTEX ) * Frame->BaseData->VertexNum ) ;
				if( Frame->TransformPolygon == NULL )
				{
					DXST_ERRORLOGFMT_ADDW(( L"�Q�Ɨp�|���S�����i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" )) ;
					return -1 ;
				}
				Frame->TransformPolygon->Polygons = ( MV1_REF_POLYGON * )( Frame->TransformPolygon + 1 ) ;
				Frame->TransformPolygon->Vertexs = ( MV1_REF_VERTEX * )( Frame->TransformPolygon->Polygons + Frame->BaseData->TriangleNum ) ;

				// �|���S�����͍ŏ��ɃZ�b�g�A�b�v���Ă���
				Frame->TransformPolygon->PolygonNum = 0 ;
				Frame->TransformPolygon->VertexNum = 0 ;
				MV1SetupReferenceMeshFrame( Model, ModelBase, Frame, Frame->TransformPolygon, 1, IsTransform ? true : false  ) ;
			}
		}
		else
		{
			// �m�ۂ���Ă��Ȃ��ꍇ�̂݊m��
			if( Frame->NonTransformPolygon == NULL )
			{
				Frame->NonTransformPolygon = ( MV1_REF_POLYGONLIST * )DXALLOC( sizeof( MV1_REF_POLYGONLIST ) + sizeof( MV1_REF_POLYGON ) * Frame->BaseData->TriangleNum + sizeof( MV1_REF_VERTEX ) * Frame->BaseData->VertexNum ) ;
				if( Frame->NonTransformPolygon == NULL )
				{
					DXST_ERRORLOGFMT_ADDW(( L"�Q�Ɨp�|���S�����i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" )) ;
					return -1 ;
				}
				Frame->NonTransformPolygon->Polygons = ( MV1_REF_POLYGON * )( Frame->NonTransformPolygon + 1 ) ;
				Frame->NonTransformPolygon->Vertexs = ( MV1_REF_VERTEX * )( Frame->NonTransformPolygon->Polygons + Frame->BaseData->TriangleNum ) ;

				// �|���S�����͍ŏ��ɃZ�b�g�A�b�v���Ă���
				Frame->NonTransformPolygon->PolygonNum = 0 ;
				Frame->NonTransformPolygon->VertexNum = 0 ;
				MV1SetupReferenceMeshFrame( Model, ModelBase, Frame, Frame->NonTransformPolygon, 1, IsTransform ? true : false  ) ;

				// �ŏ��l�ƍő�l���v�Z���Ă���
				_MV1SetupReferenceMeshMaxAndMinPosition( Frame->NonTransformPolygon ) ;
			}
		}
*/
	}

	// �Q�Ɨp���b�V�����\�z���Ă���
	NS_MV1RefreshReferenceMesh( MHandle, FrameIndex, IsTransform, IsPositionOnly ) ;

	// �I��
	return 0 ;
}

// �Q�Ɨp���b�V���̌�n��
extern int NS_MV1TerminateReferenceMesh( int MHandle, int FrameIndex, int IsTransform, int IsPositionOnly )
{
	MV1_MODEL *Model ;
	MV1_MODEL_BASE *ModelBase ;
	MV1_FRAME *Frame ;

	IsTransform = IsTransform != 0 ? 1 : 0 ;
	IsPositionOnly = IsPositionOnly != 0 ? 1 : 0 ;

	// �A�h���X�擾
	if( MV1MDLCHK( MHandle, Model ) )
		return -1 ;
	ModelBase = Model->BaseData ;

	// �t���[���P�̂����f���S�̂��ŏ����𕪊�
	if( FrameIndex == -1 )
	{
		// ���f���S�̂̏ꍇ

		// ���������m�ۂ���Ă�������
		if( Model->RefPolygon[ IsTransform ][ IsPositionOnly ] )
		{
			DXFREE( Model->RefPolygon[ IsTransform ][ IsPositionOnly ] ) ;
			Model->RefPolygon[ IsTransform ][ IsPositionOnly ] = NULL ;
		}
/*
		// �ϊ����_���擾���邩�ǂ����ŏ����𕪊�
		if( IsTransform )
		{
			// ���������m�ۂ���Ă�������
			if( Model->TransformPolygon )
			{
				DXFREE( Model->TransformPolygon ) ;
				Model->TransformPolygon = NULL ;
			}
		}
		else
		{
			// �m�ۂ���Ă��Ȃ��ꍇ�̂݊m��
			if( Model->NonTransformPolygon )
			{
				DXFREE( Model->NonTransformPolygon ) ;
				Model->NonTransformPolygon = NULL ;
			}
		}
*/
	}
	else
	{
		// �t���[���C���f�b�N�X�̃`�F�b�N
		if( FrameIndex < 0 || FrameIndex >= ModelBase->FrameNum )
			return -1 ;
		Frame = Model->Frame + FrameIndex ;

		// ���������m�ۂ���Ă�������
		if( Frame->RefPolygon[ IsTransform ][ IsPositionOnly ] )
		{
			DXFREE( Frame->RefPolygon[ IsTransform ][ IsPositionOnly ] ) ;
			Frame->RefPolygon[ IsTransform ][ IsPositionOnly ] = NULL ;
		}
/*
		// �Q�ƃ|���S�����i�[���郁�����̈�̊m��
		if( IsTransform )
		{
			// �m�ۂ���Ă��Ȃ��ꍇ�̂݊m��
			if( Frame->TransformPolygon )
			{
				DXFREE( Frame->TransformPolygon ) ;
				Frame->TransformPolygon = NULL ;
			}
		}
		else
		{
			// �m�ۂ���Ă��Ȃ��ꍇ�̂݊m��
			if( Frame->NonTransformPolygon )
			{
				DXFREE( Frame->NonTransformPolygon ) ;
				Frame->NonTransformPolygon = NULL ;
			}
		}
*/
	}

	// �I��
	return 0 ;
}

// �Q�Ɨp���b�V���̃Z�b�g�A�b�v���s��
static int MV1SetupReferenceMeshFrame(
	  MV1_MODEL           * /*Model*/,
	  MV1_MODEL_BASE      *ModelBase,
	  MV1_FRAME           *Frame,
	  MV1_REF_POLYGONLIST *DestBuffer,
	  int                  VIndexTarget,
	  bool                 IsTransform,
	  bool                 IsPositionOnly )
{
	MV1_MESH_BASE *Mesh ;
	MV1_TRIANGLE_LIST_BASE *TList ;
	MV1_REF_POLYGON *Poly ;
	MV1_REF_VERTEX *Vert, *TVert ;
	MV1_TLIST_NORMAL_POS *Norm ;
	MV1_TLIST_SKIN_POS_4B *SkinB4 ;
	MV1_TLIST_SKIN_POS_8B *SkinB8 ;
	MV1_TLIST_SKIN_POS_FREEB *SkinBF ;
	MV1_MESH_POSITION *Position ;
	MV1_MESH_FACE *Face ;
	MV1_MESH_VERTEX *MeshVert, *MVert ;
	DWORD *MVertIndex ;
	int i, j, k, l, StartIndex, MVertUnitSize, PosUnitSize, UVNum ;

	// �o�̓A�h���X���Z�b�g
	Poly = DestBuffer->Polygons + DestBuffer->PolygonNum ;
	Vert = DestBuffer->Vertexs  + DestBuffer->VertexNum ;

	// ���̃t���[�������L���Ă���|���S���̏��ƕω����Ȃ����_�̏����Z�b�g����
	Mesh = Frame->BaseData->Mesh ;
	for( i = 0 ; i < Frame->BaseData->MeshNum ; i ++, Mesh ++ )
	{
		MVertUnitSize = Mesh->VertUnitSize ;
		MeshVert = Mesh->Vertex ;

		// ���W�݂̂��ǂ����ŏ����𕪊�
		if( IsPositionOnly )
		{
			// �|���S���̃f�[�^���Z�b�g
			Face = Mesh->Face ;
			StartIndex = DestBuffer->VertexNum ;
			for( k = 0 ; k < Mesh->FaceNum ; k ++, Poly ++, Face ++ )
			{
				Poly->FrameIndex = ( unsigned short )Frame->BaseData->Index ;
				Poly->MaterialIndex = ( unsigned short )( Mesh->Material - ModelBase->Material ) ;
				Poly->VIndexTarget = VIndexTarget ;
				Poly->VIndex[ 0 ] = ( int )( ( ( MV1_MESH_VERTEX * )( ( BYTE * )MeshVert + MVertUnitSize * Face->VertexIndex[ 0 ] ) )->PositionIndex + StartIndex ) ;
				Poly->VIndex[ 1 ] = ( int )( ( ( MV1_MESH_VERTEX * )( ( BYTE * )MeshVert + MVertUnitSize * Face->VertexIndex[ 1 ] ) )->PositionIndex + StartIndex ) ;
				Poly->VIndex[ 2 ] = ( int )( ( ( MV1_MESH_VERTEX * )( ( BYTE * )MeshVert + MVertUnitSize * Face->VertexIndex[ 2 ] ) )->PositionIndex + StartIndex ) ;
			}

			// �|���S���̐��𑝂₷
			DestBuffer->PolygonNum += Mesh->FaceNum ;
		}
		else
		{
			UVNum = Mesh->UVSetUnitNum > 2 ? 2 : Mesh->UVSetUnitNum ;

			TList = Mesh->TriangleList ;
			for( j = 0 ; j < Mesh->TriangleListNum ; j ++, TList ++ )
			{
				// �|���S���f�[�^�̃Z�b�g
				StartIndex = DestBuffer->VertexNum ;
				for( k = 0 ; k < TList->IndexNum ; k += 3, Poly ++ )
				{
					Poly->FrameIndex = ( unsigned short )Frame->BaseData->Index ;
					Poly->MaterialIndex = ( unsigned short )( Mesh->Material - ModelBase->Material ) ;
					Poly->VIndexTarget = VIndexTarget ;
					Poly->VIndex[ 0 ] = TList->Index[ k     ] + StartIndex ;
					Poly->VIndex[ 1 ] = TList->Index[ k + 1 ] + StartIndex ;
					Poly->VIndex[ 2 ] = TList->Index[ k + 2 ] + StartIndex ;
				}
				DestBuffer->PolygonNum += TList->IndexNum / 3 ;

				// ���_�f�[�^�̃Z�b�g
				if( IsTransform == false )
				{
					TVert = Vert ;
					switch( TList->VertexType )
					{
					case MV1_VERTEX_TYPE_NORMAL :
						Norm = ( MV1_TLIST_NORMAL_POS * )ADDR16( TList->NormalPosition ) ;
						for( k = 0 ; k < TList->VertexNum ; k ++, Vert ++, Norm ++ )
						{
							Vert->Position = *( ( VECTOR * )&Norm->Position ) ;
							Vert->Normal   = *( ( VECTOR * )&Norm->Normal ) ;
						}
						break ;

					case MV1_VERTEX_TYPE_SKIN_4BONE :
						SkinB4 = ( MV1_TLIST_SKIN_POS_4B * )ADDR16( TList->SkinPosition4B ) ;
						for( k = 0 ; k < TList->VertexNum ; k ++, Vert ++, SkinB4 ++ )
						{
							Vert->Position = *( ( VECTOR * )&SkinB4->Position ) ;
							Vert->Normal   = SkinB4->Normal ;
						}
						break ;

					case MV1_VERTEX_TYPE_SKIN_8BONE :
						SkinB8 = ( MV1_TLIST_SKIN_POS_8B * )ADDR16( TList->SkinPosition8B ) ;
						for( k = 0 ; k < TList->VertexNum ; k ++, Vert ++, SkinB8 ++ )
						{
							Vert->Position = SkinB8->Position ;
							Vert->Normal   = SkinB8->Normal ;
						}
						break ;

					case MV1_VERTEX_TYPE_SKIN_FREEBONE :
						SkinBF = ( MV1_TLIST_SKIN_POS_FREEB * )ADDR16( TList->SkinPositionFREEB ) ;
						for( k = 0 ; k < TList->VertexNum ; k ++, Vert ++, SkinBF = ( MV1_TLIST_SKIN_POS_FREEB * )( ( BYTE * )SkinBF + TList->PosUnitSize ) )
						{
							Vert->Position = *( ( VECTOR * )&SkinBF->Position ) ;
							Vert->Normal   = *( ( VECTOR * )&SkinBF->Normal ) ;
						}
						break ;
					}
					Vert = TVert ;
				}

				MVertIndex = TList->MeshVertexIndex ;
				for( k = 0 ; k < TList->VertexNum ; Vert ++, k ++, MVertIndex ++ )
				{
					MVert = ( MV1_MESH_VERTEX * )( ( BYTE * )MeshVert + MVertUnitSize * *MVertIndex ) ;
					Vert->DiffuseColor = MVert->DiffuseColor ;
					Vert->SpecularColor = MVert->SpecularColor ;
					for( l = 0 ; l < UVNum ; l ++ )
					{
						Vert->TexCoord[ l ].u = MVert->UVs[ l ][ 0 ] ;
						Vert->TexCoord[ l ].v = MVert->UVs[ l ][ 1 ] ;
					}
				}

				// ���_�f�[�^�̐��𑝂₷
				DestBuffer->VertexNum += TList->VertexNum ;
			}
		}
	}

	if( IsPositionOnly )
	{
		// ���_�f�[�^�̃Z�b�g
		if( IsTransform == false )
		{
			PosUnitSize = Frame->BaseData->PosUnitSize ;
			Position = Frame->BaseData->Position ;
			for( k = 0 ; k < Frame->BaseData->PositionNum ; k ++, Vert ++, Position = ( MV1_MESH_POSITION * )( ( BYTE * )Position + PosUnitSize ) )
			{
				Vert->Position = Position->Position ;
			}
		}

		// ���_�f�[�^�̐��𑝂₷
		DestBuffer->VertexNum += Frame->BaseData->PositionNum ;
	}

	// �I��
	return 0 ;
}

// �Q�Ɨp���b�V���̃��t���b�V�����s��
static int MV1RefreshReferenceMeshFrame(
	MV1_FRAME			*Frame,
	int					IsPositionOnly,
	MV1_REF_POLYGONLIST *DestBuffer )
{
	MV1_MODEL *Model ;
	MV1_MODEL_BASE *ModelBase ;
	MV1_MESH_BASE *Mesh ;
	MV1_FRAME_BASE *MBFrame ;
	MV1_TRIANGLE_LIST_BASE *TList ;
	MV1_REF_VERTEX *Vert ;
	int i, k, l, m, VertNum, PosUnitSize ;
	MV1_TLIST_NORMAL_POS *Norm ;
	MV1_TLIST_SKIN_POS_4B *SkinB4 ;
	MV1_TLIST_SKIN_POS_8B *SkinB8 ;
	MV1_TLIST_SKIN_POS_FREEB *SkinBF ;
	MV1_SKINBONE_BLEND *Blend ;
	MATRIX_4X4CT BlendMat, *BMat ;
	float Weight ;

	// �A�h���X���Z�b�g
	Model = Frame->Container ;
	MBFrame = Frame->BaseData ;
	ModelBase = Model->BaseData ;

	// �o�̓A�h���X���Z�b�g
	Vert = DestBuffer->Vertexs + DestBuffer->VertexNum ;

	// ���̃t���[�������L���Ă���|���S���̏����Z�b�g����
	if( IsPositionOnly )
	{
		MV1_MESH_POSITION *Position ;
		int PosUnitSize ;
		int MaxWeightNum ;

		Position     = Frame->BaseData->Position ;
		PosUnitSize  = Frame->BaseData->PosUnitSize ;
		MaxWeightNum = ( int )( ( PosUnitSize - sizeof( VECTOR ) ) / sizeof( MV1_SKINBONE_BLEND ) ) ;
		for( i = 0 ; i < Frame->BaseData->PositionNum ; i ++, Vert ++, Position = ( MV1_MESH_POSITION * )( ( BYTE * )Position + PosUnitSize ) )
		{
			if( MaxWeightNum == 0 || Position->BoneWeight[ 0 ].Index == -1 )
			{
				VectorTransform4X4CT( &Vert->Position, &Position->Position, &Frame->LocalWorldMatrix ) ;
			}
			else
			{
				// �u�����h�s��̍쐬
				if( Position->BoneWeight[ 0 ].W == 1.0f )
				{
					BMat = Frame->UseSkinBoneMatrix[ Position->BoneWeight[ 0 ].Index ] ;
					VectorTransform4X4CT( &Vert->Position, &Position->Position, BMat ) ;
				}
				else
				{
					Blend = Position->BoneWeight ;
					Weight = Blend->W ;
					BMat = Frame->UseSkinBoneMatrix[ Blend->Index ] ;
					UnSafeMatrix4X4CT_C_Eq_C_Mul_S( &BlendMat, BMat, Weight ) ;

					Blend ++ ;
					for( m = 1 ; m < MaxWeightNum && Blend->Index != -1 ; m ++, Blend ++ )
					{
						Weight = Blend->W ;
						if( Weight == 0.0f ) continue ;

						BMat = Frame->UseSkinBoneMatrix[ Blend->Index ] ;
						UnSafeMatrix4X4CT_C_EqPlus_C_Mul_S( &BlendMat, BMat, Weight ) ;
					}

					// �ϊ����W�̍쐬
					VectorTransform4X4CT( &Vert->Position, &Position->Position, &BlendMat ) ;
				}
			}
		}

		// ���_�f�[�^�̐��𑝂₷
		DestBuffer->VertexNum += Frame->BaseData->PositionNum ;
	}
	else
	{
		Mesh = Frame->BaseData->Mesh ;
		for( i = 0 ; i < Frame->BaseData->MeshNum ; i ++, Mesh ++ )
		{
			TList = Mesh->TriangleList ;
			for( k = 0 ; k < Mesh->TriangleListNum ; k ++, TList ++ )
			{
				// ���_���̕ϊ�����
				VertNum = TList->VertexNum ;
				switch( TList->VertexType )
				{
				case MV1_VERTEX_TYPE_NORMAL :
					Norm = ( MV1_TLIST_NORMAL_POS * )ADDR16( TList->NormalPosition ) ;
					for( l = 0 ; l < VertNum ; l ++, Vert ++, Norm ++ )
					{
						VectorTransform4X4CT(   &Vert->Position, ( VECTOR * )&Norm->Position, &Frame->LocalWorldMatrix ) ;
						VectorTransformSR4X4CT( &Vert->Normal,   ( VECTOR * )&Norm->Normal,   &Frame->LocalWorldMatrix ) ;
					}
					break ;

				case MV1_VERTEX_TYPE_SKIN_4BONE :
					SkinB4 = ( MV1_TLIST_SKIN_POS_4B * )ADDR16( TList->SkinPosition4B ) ;
					for( l = 0 ; l < VertNum ; l ++, Vert ++, SkinB4 ++ )
					{
						// �u�����h�s��̍쐬
						BMat = Frame->UseSkinBoneMatrix[ TList->UseBone[ SkinB4->MatrixIndex[ 0 ] ] ] ;
						Weight = SkinB4->MatrixWeight[ 0 ] ;
						if( Weight == 1.0f )
						{
							// �ϊ����W�̍쐬
							VectorTransform4X4CT(   &Vert->Position, ( VECTOR * )&SkinB4->Position, BMat ) ;
							VectorTransformSR4X4CT( &Vert->Normal,   ( VECTOR * )&SkinB4->Normal,   BMat ) ;
						}
						else
						{
							UnSafeMatrix4X4CT_C_Eq_C_Mul_S( &BlendMat, BMat, Weight ) ;
							for( m = 1 ; m < 4 ; m ++ )
							{
								Weight = SkinB4->MatrixWeight[ m ] ;
								if( Weight == 0.0f ) continue ;

								BMat = Frame->UseSkinBoneMatrix[ TList->UseBone[ SkinB4->MatrixIndex[ m ] ] ] ;
								UnSafeMatrix4X4CT_C_EqPlus_C_Mul_S( &BlendMat, BMat, Weight ) ;
							}

							// �ϊ����W�̍쐬
							VectorTransform4X4CT(   &Vert->Position, ( VECTOR * )&SkinB4->Position, &BlendMat ) ;
							VectorTransformSR4X4CT( &Vert->Normal,   ( VECTOR * )&SkinB4->Normal,   &BlendMat ) ;
						}
					}
					break ;

				case MV1_VERTEX_TYPE_SKIN_8BONE :
					SkinB8 = ( MV1_TLIST_SKIN_POS_8B * )ADDR16( TList->SkinPosition8B ) ;
					for( l = 0 ; l < VertNum ; l ++, Vert ++, SkinB8 ++ )
					{
						// �u�����h�s��̍쐬
						BMat = Frame->UseSkinBoneMatrix[ TList->UseBone[ SkinB8->MatrixIndex1[ 0 ] ] ] ;
						Weight = SkinB8->MatrixWeight[ 0 ] ;
						if( Weight == 1.0f )
						{
							// �ϊ����W�̍쐬
							VectorTransform4X4CT(   &Vert->Position, &SkinB8->Position, BMat ) ;
							VectorTransformSR4X4CT( &Vert->Normal,   &SkinB8->Normal,   BMat ) ;
						}
						else
						{
							UnSafeMatrix4X4CT_C_Eq_C_Mul_S( &BlendMat, BMat, Weight ) ;
							for( m = 1 ; m < 8 ; m ++ )
							{
								Weight = SkinB8->MatrixWeight[ m ] ;
								if( Weight == 0.0f ) continue ;

								BMat = Frame->UseSkinBoneMatrix[ TList->UseBone[ m < 4 ? SkinB8->MatrixIndex1[ m ] : SkinB8->MatrixIndex2[ m - 4 ] ] ] ;
								UnSafeMatrix4X4CT_C_EqPlus_C_Mul_S( &BlendMat, BMat, Weight ) ;
							}

							// �ϊ����W�̍쐬
							VectorTransform4X4CT(   &Vert->Position, &SkinB8->Position, &BlendMat ) ;
							VectorTransformSR4X4CT( &Vert->Normal,   &SkinB8->Normal,   &BlendMat ) ;
						}
					}
					break ;

				case MV1_VERTEX_TYPE_SKIN_FREEBONE :
					SkinBF = ( MV1_TLIST_SKIN_POS_FREEB * )ADDR16( TList->SkinPositionFREEB ) ;
					PosUnitSize = TList->PosUnitSize ;
					for( l = 0 ; l < VertNum ; l ++, Vert ++, SkinBF = ( MV1_TLIST_SKIN_POS_FREEB * )( ( BYTE * )SkinBF + PosUnitSize ) )
					{
						// �u�����h�s��̍쐬
						if( SkinBF->MatrixWeight[ 0 ].W == 1.0f )
						{
							BMat = Frame->UseSkinBoneMatrix[ SkinBF->MatrixWeight[ 0 ].Index ] ;
							VectorTransform4X4CT(   &Vert->Position, ( VECTOR * )&SkinBF->Position, BMat ) ;
							VectorTransformSR4X4CT( &Vert->Normal,   ( VECTOR * )&SkinBF->Normal,   BMat ) ;
						}
						else
						{
							Blend = SkinBF->MatrixWeight ;
							Weight = Blend->W ;
							BMat = Frame->UseSkinBoneMatrix[ Blend->Index ] ;
							UnSafeMatrix4X4CT_C_Eq_C_Mul_S( &BlendMat, BMat, Weight ) ;

							Blend ++ ;
							for( m = 1 ; m < TList->MaxBoneNum && Blend->Index != -1 ; m ++, Blend ++ )
							{
								Weight = Blend->W ;
								if( Weight == 0.0f ) continue ;

								BMat = Frame->UseSkinBoneMatrix[ Blend->Index ] ;
								UnSafeMatrix4X4CT_C_EqPlus_C_Mul_S( &BlendMat, BMat, Weight ) ;
							}

							// �ϊ����W�̍쐬
							VectorTransform4X4CT(   &Vert->Position, ( VECTOR * )&SkinBF->Position, &BlendMat ) ;
							VectorTransformSR4X4CT( &Vert->Normal,   ( VECTOR * )&SkinBF->Normal,   &BlendMat ) ;
						}
					}
					break ;
				}

				// ���_�f�[�^�̐��𑝂₷
				DestBuffer->VertexNum += TList->VertexNum ;
			}
		}
	}

	// �I��
	return 0 ;
}

// �Q�Ɨp���b�V���̍X�V
extern int NS_MV1RefreshReferenceMesh( int MHandle, int FrameIndex, int IsTransform, int IsPositionOnly )
{
	MV1_MODEL *Model ;
	MV1_MODEL_BASE *ModelBase ;
	MV1_FRAME *Frame ;
	MV1_REF_POLYGONLIST *PolyList ;
	int i ;
	bool Change ;

	IsTransform = IsTransform != 0 ? 1 : 0 ;
	IsPositionOnly = IsPositionOnly != 0 ? 1 : 0 ;

	// �A�h���X�擾
	if( MV1MDLCHK( MHandle, Model ) )
		return -1 ;
	ModelBase = Model->BaseData ;

	// �s��̃Z�b�g�A�b�v
	MV1SETUPMATRIX( Model ) ;

	// �t���[���P�̂����f���S�̂��ŏ����𕪊�
	Change = false ;
	if( FrameIndex == -1 )
	{
		// ���f���S�̂̏ꍇ

		// �Z�b�g�A�b�v������Ă��Ȃ�������Z�b�g�A�b�v
		if( Model->RefPolygon[ IsTransform ][ IsPositionOnly ] == NULL )
		{
			Change = true ;
			if( NS_MV1SetupReferenceMesh( MHandle, FrameIndex, IsTransform, IsPositionOnly ) < 0 )
				return -1 ;
		}

		// �f�[�^���X�V����K�v�������ꍇ�͍X�V���Ȃ�
		if( Model->SetupRefPolygon[ IsTransform ][ IsPositionOnly ] )
			return 0 ;

		// �X�V����|���S���z��̐擪�A�h���X���Z�b�g
		PolyList = Model->RefPolygon[ IsTransform ][ IsPositionOnly ] ;

		// �X�V�����t���O�𗧂Ă�
		Model->SetupRefPolygon[ IsTransform ][ IsPositionOnly ] = true ;

		// ���_�ϊ����K�v���ǂ����ŏ����𕪊�
		if( IsTransform )
		{
			// �R���W�����̍X�V�����t���O��|��
			Model->SetupCollision = false ;

			// �S�Ẵt���[���̃|���S�����\�z
			PolyList->VertexNum = 0 ;
			for( i = 0 ; i < ModelBase->FrameNum ; i ++ )
				MV1RefreshReferenceMeshFrame( Model->Frame + i, IsPositionOnly, PolyList ) ;

			Change = true ;
		}
	}
	else
	{
		// �t���[���C���f�b�N�X�̃`�F�b�N
		if( FrameIndex < 0 || FrameIndex >= ModelBase->FrameNum )
			return -1 ;
		Frame = Model->Frame + FrameIndex ;

		// �Z�b�g�A�b�v������Ă��Ȃ�������Z�b�g�A�b�v
		if( Frame->RefPolygon[ IsTransform ][ IsPositionOnly ] == NULL )
		{
			Change = true ;
			if( NS_MV1SetupReferenceMesh( MHandle, FrameIndex, IsTransform, IsPositionOnly ) < 0 )
				return -1 ;
		}

		// �f�[�^���X�V����K�v�������ꍇ�͍X�V���Ȃ�
		if( Frame->SetupRefPolygon[ IsTransform ][ IsPositionOnly ] )
			return 0 ;

		// �X�V����|���S���z��̐擪�A�h���X���Z�b�g
		PolyList = Frame->RefPolygon[ IsTransform ][ IsPositionOnly ] ;

		// �X�V�����t���O�𗧂Ă�
		Frame->SetupRefPolygon[ IsTransform ][ IsPositionOnly ] = true ;

		// ���_�ϊ����K�v���ǂ����ŏ����𕪊�
		if( IsTransform )
		{
			// �R���W�����̍X�V�����t���O��|��
			Frame->SetupCollision = false ;

			// �t���[���̃|���S�����\�z
			PolyList->VertexNum = 0 ;
			MV1RefreshReferenceMeshFrame( Frame, IsPositionOnly, PolyList ) ;

			Change = true ;
		}
	}

	// �|���S���̍ő���W�l�A�ŏ����W�l�̍X�V
	if( PolyList->PolygonNum )
	{
		if( Change )
		{
			_MV1SetupReferenceMeshMaxAndMinPosition( PolyList ) ;
		}
	}
	else
	{
		PolyList->MaxPosition.x = -10000000000000.0f ;
		PolyList->MaxPosition.y = -10000000000000.0f ;
		PolyList->MaxPosition.z = -10000000000000.0f ;
		PolyList->MinPosition.x = -10000000000000.0f ;
		PolyList->MinPosition.y = -10000000000000.0f ;
		PolyList->MinPosition.z = -10000000000000.0f ;
	}

	// �I��
	return 0 ;
}

// �Q�Ɨp���b�V�����擾����
extern MV1_REF_POLYGONLIST NS_MV1GetReferenceMesh( int MHandle, int FrameIndex, int IsTransform, int IsPositionOnly )
{
	MV1_MODEL *Model ;
	MV1_MODEL_BASE *ModelBase ;
	MV1_FRAME *Frame ;
	MV1_REF_POLYGONLIST PolyList ;

	IsTransform = IsTransform != 0 ? 1 : 0 ;
	IsPositionOnly = IsPositionOnly != 0 ? 1 : 0 ;

	// �A�h���X�擾
	if( MV1MDLCHK( MHandle, Model ) )
		goto ERR ;
	ModelBase = Model->BaseData ;

	// �t���[���C���f�b�N�X�� -1 �������烂�f���S��
	if( FrameIndex == -1 )
	{
		// �Z�b�g�A�b�v������Ă��Ȃ������ꍇ�̓Z�b�g�A�b�v���s��
		if( Model->RefPolygon[ IsTransform ][ IsPositionOnly ] == NULL )
			if( NS_MV1RefreshReferenceMesh( MHandle, FrameIndex, IsTransform, IsPositionOnly ) < 0 )
				goto ERR ;

		return *Model->RefPolygon[ IsTransform ][ IsPositionOnly ] ;

/*
		// ���_�ϊ����K�v���ǂ����ŏ����𕪊�
		if( IsTransform )
		{
			// �Z�b�g�A�b�v������Ă��Ȃ������ꍇ�̓Z�b�g�A�b�v���s��
			if( Model->TransformPolygon == NULL )
				if( NS_MV1RefreshReferenceMesh( MHandle, FrameIndex, IsTransform ) < 0 )
					return PolyList ;

			return *Model->TransformPolygon ;
		}
		else
		{
			// �Z�b�g�A�b�v������Ă��Ȃ������ꍇ�̓Z�b�g�A�b�v���s��
			if( Model->NonTransformPolygon == NULL )
				if( NS_MV1RefreshReferenceMesh( MHandle, FrameIndex, IsTransform ) < 0 )
					return PolyList ;

			return *Model->NonTransformPolygon ;
		}
*/
	}
	else
	{
		// �t���[���C���f�b�N�X�̃`�F�b�N
		if( FrameIndex < 0 || FrameIndex >= ModelBase->FrameNum )
			goto ERR ;
		Frame = Model->Frame + FrameIndex ;

		// �Z�b�g�A�b�v������Ă��Ȃ������ꍇ�̓Z�b�g�A�b�v���s��
		if( Frame->RefPolygon[ IsTransform ][ IsPositionOnly ] == NULL )
			if( NS_MV1RefreshReferenceMesh( MHandle, FrameIndex, IsTransform, IsPositionOnly ) < 0 )
				goto ERR ;

		return *Frame->RefPolygon[ IsTransform ][ IsPositionOnly ] ;
/*
		// ���_�ϊ����K�v���ǂ����ŏ����𕪊�
		if( IsTransform )
		{
			// �Z�b�g�A�b�v������Ă��Ȃ������ꍇ�̓Z�b�g�A�b�v���s��
			if( Frame->TransformPolygon == NULL )
				if( NS_MV1RefreshReferenceMesh( MHandle, FrameIndex, IsTransform ) < 0 )
					return PolyList ;

			return *Frame->TransformPolygon ;
		}
		else
		{
			// �Z�b�g�A�b�v������Ă��Ȃ������ꍇ�̓Z�b�g�A�b�v���s��
			if( Frame->NonTransformPolygon == NULL )
				if( NS_MV1RefreshReferenceMesh( MHandle, FrameIndex, IsTransform ) < 0 )
					return PolyList ;

			return *Frame->NonTransformPolygon ;
		}
*/
	}

ERR :
	_MEMSET( &PolyList, 0, sizeof( PolyList ) ) ;
	return PolyList ;
}


#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

#endif  // DX_NON_MODEL


