// -------------------------------------------------------------------------------
// 
// 		�c�w���C�u����		�o�l�c���f���f�[�^�ǂݍ��݃v���O����
// 
// 				Ver 3.16d
// 
// -------------------------------------------------------------------------------

/*

�{�\�[�X���쐬����ۂɎQ�l�ɂ����Ă����������E�F�u�T�C�g�ƃv���O�����\�[�X


PMD�`���ɂ��ĎQ�l�ɂ����Ă����������E�F�u�T�C�g

<< �ʂ肷����̋L�� >> �ʂ肷����l
http://blog.goo.ne.jp/torisu_tetosuki


VMD�`����Bullet�ɂ�镨�����Z�ɂ��ĎQ�l�ɂ����Ă����������v���O�����\�[�X

<< PY >> PY�l
http://ppyy.hp.infoseek.co.jp/
<< artk_mmd_src.zip >> 


IK�����̃v���O�����\�[�X
���M�l
<< ik.zip >>

*/

#define __DX_MAKE

#include "DxModelLoader3.h"

#ifndef DX_NON_MODEL

// �C���N���[�h ---------------------------------
#include "DxFile.h"
#include "DxLog.h"
#include "DxModelLoaderVMD.h"
#include "DxMemory.h"

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// �}�N����` -----------------------------------

// �f�[�^�錾 -----------------------------------

static int           WCHAR_T_StringSetup = 0 ;
static const char *  HizaString_UTF16LE   = "\x72\x30\x56\x30\x00"/*@ L"�Ђ�" @*/ ;
static const char *  CenterString_UTF16LE = "\xbb\x30\xf3\x30\xbf\x30\xfc\x30\x00"/*@ L"�Z���^�[" @*/ ;
static wchar_t       HizaString_WCHAR_T[ 8 ] ;
static wchar_t       CenterString_WCHAR_T[ 8 ] ;

// �֐��錾 -------------------------------------

// �u�l�c�t�@�C����ǂݍ���( -1:�G���[ )
static int _MV1LoadModelToVMD_PMD(
	int								DataIndex,
	MV1_MODEL_R *					RModel,
	void *							DataBuffer,
	int								DataSize,
	const char *					Name,
	int								LoopMotionFlag,
	int								DisablePhysicsFlag,
	PMD_READ_BONE_INFO *			PmdBoneInfo,
	int								PmdBoneNum,
	PMD_READ_IK_INFO *				PmdIKInfoFirst,
#ifndef DX_NON_BULLET_PHYSICS
	DX_MODELLOADER3_PMD_PHYSICS_INFO *	MLPhysicsInfo,
#endif
	bool							FPS60
) ;

// �s����v�Z����( ������� )
static void MV1LoadModelToPMD_SetupMatrix_One( PMD_READ_BONE_INFO *BoneInfo ) ;

// �s����v�Z����
static void MV1LoadModelToPMD_SetupMatrix( PMD_READ_BONE_INFO *BoneInfo, int BoneNum, int UseInitParam, int IKSkip ) ;

// �h�j�{�[���̃g�����X���[�V��������]�l�ɕϊ�����
//static void MV1LoadModelToPMD_ConvertTransToRotate( PMD_READ_BONE_INFO *BoneInfo, PMD_READ_IK_INFO *IKInfoFirst ) ;

// �h�j���v�Z����
static void MV1LoadModelToPMD_SetupIK( PMD_READ_BONE_INFO *BoneInfo, PMD_READ_IK_INFO *IKInfoFirst ) ;

// �w��̃{�[���ɃA�j���[�V�����̎w��L�[�̃p�����[�^�𔽉f������
static void MV1LoadModelToPMD_SetupOneBoneMatrixFormAnimKey( PMD_READ_BONE_INFO *BoneInfo, int Time, int LoopNo, int MaxTime, int ValidNextRate, float NextRate ) ;

// �v���O���� -----------------------------------

/*
// VMD�̃J�����p�����[�^������W�Ɖ�]�l���Z�o����
void CalculateCameraParam( VECTOR *Position, VECTOR *Rotation, float Length, VECTOR *OutPosition, VECTOR *OutRotation )
{
	VECTOR OutPosition ;
	VECTOR DirVec ;
	MATRIX VRotate, HRotate, TwistRotate ;
	MATRIX PosTransMix ;

	OutPosition = VGet( 0.0f, 0.0f, 1.0f ) ;
	DirVec = VGet( 0.0f, 0.0f, -1.0f ) ;
	VRotate = MGetRotX( Rotation->x * DX_PI_F / 180.0f ) ;
	HRotate = MGetRotY( Rotation->y * DX_PI_F / 180.0f ) ;
	PosTransMix = MMult( VRotate, HRotate ) ;

	DirVec = VTransform( DirVec, PosTransMix ) ;
	TwistRotate = MGetRotAxis( DirVec, Rotation->z * DX_PI_F / 180.0f ) ;

	*OutPosition = VTransform( *Position, PosTransMix ) ;
	OutRotation->x = Rotation->x ;
	OutRotation->y = Rotation->y ;
}
*/
// �o�l�c�t�@�C����ǂݍ���( -1:�G���[  0�ȏ�:���f���n���h�� )
extern int MV1LoadModelToPMD( const MV1_MODEL_LOAD_PARAM *LoadParam, int ASyncThread )
{
	int NewHandle = -1 ;
	int ErrorFlag = 1 ;
	int i, j, weightcount, facecount ;
	PMD_HEADER *PmdHeader ;
	PMD_VERTEX *PmdVertex ;
	DWORD PmdVertexNum ;
	WORD *PmdFaceVertex ;
	DWORD PmdFaceVertexNum ;
	DWORD PmdFaceNum ;
	BYTE *PmdSkin ;
	PMD_MATERIAL *PmdMaterial ;
	PMD_READ_IK_INFO *IKInfoDim = NULL, *IKInfoFirst = NULL, *IKInfo ;
	PMD_READ_BONE_INFO *BoneInfoDim = NULL, *BoneInfo ;
	DWORD PmdMaterialFaceVertexNum ;
	DWORD PmdMaterialNum ;
	PMD_BONE *PmdBone ;
	WORD PmdBoneNum ;
	PMD_IK *PmdIK ;
	WORD PmdIKNum ;
	PMD_SKIN_VERT *PmdBaseSkinVert, *PmdSkinVert ;
	WORD PmdSkinNum ;
	char *PmdToonFileName = NULL ;
	MV1_MODEL_R RModel ;
	MV1_TEXTURE_R *Texture ;
	MV1_MATERIAL_R *Material ;
	MV1_SKIN_WEIGHT_R *SkinWeight, *SkinWeightTemp ;
	MV1_SKIN_WEIGHT_ONE_R *SkinW, *SkinWTemp ;
	MV1_FRAME_R *Frame ;
	char FrameDimEnable[ 1024 ] ;
	MV1_FRAME_R *FrameDim[ 1024 ] ;
	char FrameSkipDim[ 1024 ] ;
	int FrameSkipNum ;
	MV1_MESH_R *Mesh, *SkinMesh ;
	MV1_MESHFACE_R *MeshFace, *MeshFaceTemp ;
	MV1_SHAPE_R *Shape ;
	MV1_SHAPE_VERTEX_R *ShapeVert ;
	DWORD *SkinNextVertIndex = NULL, *SkinPrevVertIndex ;
	DWORD *SkinNextFaceIndex, *SkinPrevFaceIndex ;
	DWORD *SkinNextMaterialIndex, *SkinPrevMaterialIndex ;
	DWORD SkinVertNum, SkinBaseVertNum, SkinTargetVert, SkinFaceNum, NextFaceNum, SkinMaterialNum ;
	char String[ 1024 ] ;
	BYTE *Src ;
	int ValidPhysics = FALSE ;
	int BoneDispNum ;
	BYTE ToonTexIndex ;
#ifndef DX_NON_BULLET_PHYSICS
	DX_MODELLOADER3_PMD_PHYSICS_INFO MLPhysicsInfo ;

	_MEMSET( &MLPhysicsInfo, 0, sizeof( MLPhysicsInfo ) ) ;
#endif

	// ������̃Z�b�g�A�b�v
	if( WCHAR_T_StringSetup == 0 )
	{
		ConvString( HizaString_UTF16LE,   DX_CHARCODEFORMAT_UTF16LE, ( char * )HizaString_WCHAR_T,   sizeof( HizaString_WCHAR_T ),   WCHAR_T_CHARCODEFORMAT ) ;
		ConvString( CenterString_UTF16LE, DX_CHARCODEFORMAT_UTF16LE, ( char * )CenterString_WCHAR_T, sizeof( CenterString_WCHAR_T ), WCHAR_T_CHARCODEFORMAT ) ;
		WCHAR_T_StringSetup = 1 ;
	}

	// �ǂݍ��݂悤�f�[�^�̏�����
	MV1InitReadModel( &RModel ) ;
	RModel.MaterialNumberOrderDraw = TRUE ;
	RModel.MeshFaceRightHand = FALSE ;
	RModel.TranslateIsBackCulling = TRUE ;

	// Pmd���f���f�[�^�̏����Z�b�g
	Src = ( BYTE * )LoadParam->DataBuffer ;
	PmdHeader = ( PMD_HEADER * )LoadParam->DataBuffer ;

	// PMD�t�@�C�����ǂ������m�F
	if( Src[ 0 ] != 'P' || Src[ 1 ] != 'm' || Src[ 2 ] != 'd' )
		return -1 ;

	// �o�[�W�����P�ȊO�͓ǂݍ��߂Ȃ�
	if( *( ( DWORD * )&Src[ 3 ] ) != 0x3f800000 )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x44\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xd0\x30\xfc\x30\xb8\x30\xe7\x30\xf3\x30\x11\xff\x0e\xff\x10\xff\xe5\x4e\x16\x59\x6f\x30\xad\x8a\x7f\x30\xbc\x8f\x81\x30\x7e\x30\x5b\x30\x93\x30\x0a\x00\x00"/*@ L"PMD Load Error : �o�[�W�����P�D�O�ȊO�͓ǂݍ��߂܂���\n" @*/ )) ;
		return -1 ;
	}

	// ���f�����ƃt�@�C�����ƕ����R�[�h�`�����Z�b�g
	RModel.CharCodeFormat = DX_CHARCODEFORMAT_SHIFTJIS ;
	RModel.FilePath = ( wchar_t * )DXALLOC( ( _WCSLEN( LoadParam->FilePath ) + 1 ) * sizeof( wchar_t ) ) ;
	RModel.Name     = ( wchar_t * )DXALLOC( ( _WCSLEN( LoadParam->Name     ) + 1 ) * sizeof( wchar_t ) ) ;
	_WCSCPY( RModel.FilePath, LoadParam->FilePath ) ;
	_WCSCPY( RModel.Name,     LoadParam->Name ) ;

	// �@��̎��������͎g�p���Ȃ�
	RModel.AutoCreateNormal = FALSE ;

	// �e�f�[�^�̐擪�A�h���X���Z�b�g
	{
		Src += 283 ;

		PmdVertexNum = *( ( DWORD * )Src ) ;
		Src += 4 ;

		PmdVertex = ( PMD_VERTEX * )Src ;
		Src += 38 * PmdVertexNum ;

		PmdFaceVertexNum = *( ( DWORD * )Src ) ;
		PmdFaceNum = PmdFaceVertexNum / 3 ;
		Src += 4 ;

		PmdFaceVertex = ( WORD * )Src ;
		Src += 2 * PmdFaceVertexNum ;

		PmdMaterialNum = *( ( DWORD * )Src ) ;
		Src += 4 ;

		PmdMaterial = ( PMD_MATERIAL * )Src ;
		Src += 70 * PmdMaterialNum ;

		PmdBoneNum = *( ( WORD * )Src ) ;
		Src += 2 ;

		PmdBone = ( PMD_BONE * )Src ;
		Src += 39 * PmdBoneNum ;

		PmdIKNum = *( ( WORD * )Src ) ;
		Src += 2 ;

		PmdIK = ( PMD_IK * )Src ;
	}

	// ���b�V�������߂�t���[���̒ǉ�
	Frame = MV1RAddFrame( &RModel, "Mesh", NULL ) ;
	if( Frame == NULL )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x44\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xd5\x30\xec\x30\xfc\x30\xe0\x30\xaa\x30\xd6\x30\xb8\x30\xa7\x30\xaf\x30\xc8\x30\x6e\x30\xfd\x8f\xa0\x52\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMD Load Error : �t���[���I�u�W�F�N�g�̒ǉ��Ɏ��s���܂���\n" @*/ )) ;
		goto ENDLABEL ;
	}

	// �ǂݍ��ݏ����p�̃{�[���f�[�^���i�[���郁�����̈�̊m��
	BoneInfoDim = ( PMD_READ_BONE_INFO * )DXALLOC( sizeof( PMD_READ_BONE_INFO ) * PmdBoneNum ) ;
	if( BoneInfoDim == NULL )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x44\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xad\x8a\x7f\x30\xbc\x8f\x7f\x30\xe6\x51\x06\x74\x28\x75\xdc\x30\xfc\x30\xf3\x30\xc5\x60\x31\x58\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMD Load Error : �ǂݍ��ݏ����p�{�[�������i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
		goto ENDLABEL ;
	}

	// �{�[���f�[�^�̒ǉ�
	_MEMSET( FrameDimEnable, 0, sizeof( FrameDimEnable ) ) ;
	_MEMSET( FrameDim, 0, sizeof( FrameDim ) ) ;
	do
	{
		_MEMSET( FrameSkipDim, 0, sizeof( FrameSkipDim ) ) ;
		Src = ( BYTE * )PmdBone ;
		BoneInfo = BoneInfoDim ;
		FrameSkipNum = 0 ;
		for( i = 0 ; i < PmdBoneNum ; i ++, Src += 39, BoneInfo ++ )
		{
			WORD ParentBoneIndex = *( ( WORD * )&Src[ 20 ] ) ;

			if( ParentBoneIndex != 0xffff && FrameDimEnable[ ParentBoneIndex ] == 0 )
			{
				FrameSkipDim[ i ] = true ;
				FrameSkipNum ++ ;
				continue ;
			}
			if( FrameDimEnable[ i ] != 0 )
				continue ;
			FrameDimEnable[ i ] = 1 ;

			_MEMSET( String, 0, sizeof( String ) ) ;
			_MEMCPY( String, Src, 20 ) ;
			FrameDim[ i ] = MV1RAddFrame( &RModel, String, ParentBoneIndex == 0xffff ? NULL : FrameDim[ ParentBoneIndex ] ) ;
			if( FrameDim[ i ] == NULL )
			{
				DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x44\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xd5\x30\xec\x30\xfc\x30\xe0\x30\xaa\x30\xd6\x30\xb8\x30\xa7\x30\xaf\x30\xc8\x30\x6e\x30\xfd\x8f\xa0\x52\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMD Load Error : �t���[���I�u�W�F�N�g�̒ǉ��Ɏ��s���܂���\n" @*/ )) ;
				goto ENDLABEL ;
			}
//			BoneInfo->IKLimitAngle = _STRCMP( "���Ђ�", FrameDim[ i ]->Name ) == 0 || _STRCMP( "�E�Ђ�", FrameDim[ i ]->Name ) == 0 ? 1 : 0 ;
			BoneInfo->IKLimitAngle = _WCSSTR( FrameDim[ i ]->NameW, HizaString_WCHAR_T ) != NULL ;
			*( ( DWORD * )&BoneInfo->OrgTranslate.x ) = *( ( DWORD * )&Src[ 27 ] ) ;
			*( ( DWORD * )&BoneInfo->OrgTranslate.y ) = *( ( DWORD * )&Src[ 31 ] ) ;
			*( ( DWORD * )&BoneInfo->OrgTranslate.z ) = *( ( DWORD * )&Src[ 35 ] ) ;
			*( ( DWORD * )&FrameDim[ i ]->TempVector.x ) = *( ( DWORD * )&Src[ 27 ] ) ;
			*( ( DWORD * )&FrameDim[ i ]->TempVector.y ) = *( ( DWORD * )&Src[ 31 ] ) ;
			*( ( DWORD * )&FrameDim[ i ]->TempVector.z ) = *( ( DWORD * )&Src[ 35 ] ) ;
	//		FrameDim[ i ]->TempVector.z = -FrameDim[ i ]->TempVector.z ;
			if( ParentBoneIndex != 0xffff )
			{
				FrameDim[ i ]->Translate = VSub( FrameDim[ i ]->TempVector, FrameDim[ ParentBoneIndex ]->TempVector ) ;
			}
			else
			{
				FrameDim[ i ]->Translate = FrameDim[ i ]->TempVector ;
			}
			FrameDim[ i ]->UserData = BoneInfo ;

			BoneInfo->Base = ( PMD_BONE * )Src ;
			BoneInfo->Frame = FrameDim[ i ] ;
			BoneInfo->Type = ( int )Src[ 24 ] ;
			BoneInfo->IsPhysics = FALSE ;
			BoneInfo->IsIK = FALSE ;
			BoneInfo->IsIKAnim = FALSE ;
			BoneInfo->IsIKChild = FALSE ;
			BoneInfo->Translate = FrameDim[ i ]->Translate ;
			BoneInfo->Rotate.x = 0.0f ;
			BoneInfo->Rotate.y = 0.0f ;
			BoneInfo->Rotate.z = 0.0f ;
			BoneInfo->Rotate.w = 1.0f ;
			BoneInfo->KeyMatrix = NULL ;
			BoneInfo->KeyMatrix2 = NULL ;

			BoneInfo->InitTranslate = BoneInfo->Translate ;
			BoneInfo->InitRotate    = BoneInfo->Rotate ;
		}
	}while( FrameSkipNum != 0 ) ;
	MV1LoadModelToPMD_SetupMatrix( BoneInfoDim, PmdBoneNum, TRUE, FALSE ) ;

	// ���b�V����ǉ�
	{
		Mesh = MV1RAddMesh( &RModel, Frame ) ;
		if( Mesh == NULL )
		{
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x44\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xe1\x30\xc3\x30\xb7\x30\xe5\x30\xaa\x30\xd6\x30\xb8\x30\xa7\x30\xaf\x30\xc8\x30\x6e\x30\xfd\x8f\xa0\x52\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMD Load Error : ���b�V���I�u�W�F�N�g�̒ǉ��Ɏ��s���܂���\n" @*/ )) ;
			goto ENDLABEL ;
		}

		// �ʏ����i�[���郁�����̈�̊m��
		if( MV1RSetupMeshFaceBuffer( &RModel, Mesh, ( int )PmdFaceNum, 3 ) < 0 )
		{
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x44\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\x62\x97\xc5\x60\x31\x58\x92\x30\xdd\x4f\x58\x5b\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMD Load Error : �ʏ���ۑ����郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
			goto ENDLABEL ;
		}

		Mesh->PositionNum = PmdVertexNum ;
		Mesh->Positions = ( VECTOR * )ADDMEMAREA( ( sizeof( VECTOR ) + sizeof( float ) ) * Mesh->PositionNum, &RModel.Mem ) ;
		if( Mesh->Positions == NULL )
		{
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x44\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\x02\x98\xb9\x70\xa7\x5e\x19\x6a\x92\x30\xdd\x4f\x58\x5b\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMD Load Error : ���_���W��ۑ����郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
			goto ENDLABEL ;
		}
		Mesh->PositionToonOutLineScale = ( float * )( Mesh->Positions + Mesh->PositionNum ) ;

		Mesh->NormalNum = PmdVertexNum ;
		Mesh->Normals = ( VECTOR * )ADDMEMAREA( sizeof( VECTOR ) * Mesh->NormalNum, &RModel.Mem ) ;
		if( Mesh->Normals == NULL )
		{
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x44\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\x02\x98\xb9\x70\xd5\x6c\xda\x7d\x92\x30\xdd\x4f\x58\x5b\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMD Load Error : ���_�@����ۑ����郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
			goto ENDLABEL ;
		}

		Mesh->UVNum[ 0 ] = PmdVertexNum ;
		Mesh->UVs[ 0 ] = ( FLOAT4 * )ADDMEMAREA( sizeof( FLOAT4 ) * Mesh->UVNum[ 0 ], &RModel.Mem ) ;
		if( Mesh->UVs[ 0 ] == NULL )
		{
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x44\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\x02\x98\xb9\x70\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\xa7\x5e\x19\x6a\x92\x30\xdd\x4f\x58\x5b\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMD Load Error : ���_�e�N�X�`�����W��ۑ����郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
			goto ENDLABEL ;
		}

		// �{�[���̏���ǉ�
		Mesh->SkinWeightsNum = 0 ;
		for( i = 0 ; i < PmdBoneNum ; i ++ )
		{
			// ���̃{�[�����g�p���Ă��钸�_�̐��𐔂���
			Src = ( BYTE * )PmdVertex ;
			weightcount = 0 ;
			for( j = 0 ; ( DWORD )j < PmdVertexNum ; j ++, Src += 38 )
			{
				if( *( ( WORD * )&Src[ 32 ] ) == i || *( ( WORD * )&Src[ 34 ] ) == i )
				{
					weightcount ++ ;
				}
			}

			// �g�p����Ă��Ȃ�������E�G�C�g���͕t���Ȃ�
			if( weightcount == 0 ) continue ;

			Mesh->SkinWeights[ Mesh->SkinWeightsNum ] = MV1RAddSkinWeight( &RModel ) ;
			SkinWeight = Mesh->SkinWeights[ Mesh->SkinWeightsNum ] ;
			Mesh->SkinWeightsNum ++ ;
			SkinWeight->TargetFrame = ( DWORD )FrameDim[ i ]->Index ;
			CreateTranslationMatrix( &SkinWeight->ModelLocalMatrix, -FrameDim[ i ]->TempVector.x, -FrameDim[ i ]->TempVector.y, -FrameDim[ i ]->TempVector.z ) ;

			// �f�[�^���i�[���郁�����̈�̊m��
			SkinWeight->DataNum = ( DWORD )weightcount ;
			SkinWeight->Data = ( MV1_SKIN_WEIGHT_ONE_R * )ADDMEMAREA( sizeof( MV1_SKIN_WEIGHT_ONE_R ) * SkinWeight->DataNum, &RModel.Mem ) ;
			if( SkinWeight->Data == NULL )
			{
				DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x44\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xb9\x30\xad\x30\xcb\x30\xf3\x30\xb0\x30\xe1\x30\xc3\x30\xb7\x30\xe5\x30\xa6\x30\xa8\x30\xa4\x30\xc8\x30\x24\x50\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMD Load Error : �X�L�j���O���b�V���E�G�C�g�l���i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
				return -1 ;
			}

			// �E�G�C�g�̏����i�[����
			SkinW = SkinWeight->Data ;
			Src = ( BYTE * )PmdVertex ;
			for( j = 0 ; ( DWORD )j < PmdVertexNum ; j ++, Src += 38 )
			{
				if( *( ( WORD * )&Src[ 32 ] ) == i && *( ( WORD * )&Src[ 34 ] ) == i )
				{
					SkinW->TargetVertex = ( DWORD )j ;
					SkinW->Weight = 1.0f ;
					SkinW ++ ;
				}
				else
				if( *( ( WORD * )&Src[ 32 ] ) == i )
				{
					SkinW->TargetVertex = ( DWORD )j ;
					SkinW->Weight = Src[ 36 ] / 100.0f ;
					SkinW ++ ;
				}
				else
				if( *( ( WORD * )&Src[ 34 ] ) == i )
				{
					SkinW->TargetVertex = ( DWORD )j ;
					SkinW->Weight = ( 100 - Src[ 36 ] ) / 100.0f ;
					SkinW ++ ;
				}
			}
		}

		// ���_�f�[�^���Z�b�g
		Src = ( BYTE * )PmdVertex ;
		for( i = 0 ; ( DWORD )i < PmdVertexNum ; i ++, Src += 38 )
		{
			*( ( DWORD * )&Mesh->Positions[ i ].x ) = *( ( DWORD * )&Src[ 0 ] ) ;
			*( ( DWORD * )&Mesh->Positions[ i ].y ) = *( ( DWORD * )&Src[ 4 ] ) ;
			*( ( DWORD * )&Mesh->Positions[ i ].z ) = *( ( DWORD * )&Src[ 8 ] ) ;
			*( ( DWORD * )&Mesh->Normals[ i ].x ) = *( ( DWORD * )&Src[ 12 ] ) ;
			*( ( DWORD * )&Mesh->Normals[ i ].y ) = *( ( DWORD * )&Src[ 16 ] ) ;
			*( ( DWORD * )&Mesh->Normals[ i ].z ) = *( ( DWORD * )&Src[ 20 ] ) ;
			*( ( DWORD * )&Mesh->UVs[ 0 ][ i ].x ) = *( ( DWORD * )&Src[ 24 ] ) ;
			*( ( DWORD * )&Mesh->UVs[ 0 ][ i ].y ) = *( ( DWORD * )&Src[ 28 ] ) ;
			Mesh->PositionToonOutLineScale[ i ] = Src[ 37 ] != 0 ? 0.0f : 1.0f ;
		}

		// �ʃf�[�^���Z�b�g
		Src = ( BYTE * )PmdFaceVertex ;
		MeshFace = Mesh->Faces ;
		j = 0 ;
		for( i = 0 ; ( DWORD )i < PmdFaceNum ; i ++, Src += 2 * 3 )
		{
			MeshFace->IndexNum = 3 ;
			MeshFace->VertexIndex[ 0 ] = *( ( WORD * )&Src[ 0 ] ) ;
			MeshFace->VertexIndex[ 1 ] = *( ( WORD * )&Src[ 2 ] ) ;
			MeshFace->VertexIndex[ 2 ] = *( ( WORD * )&Src[ 4 ] ) ;
			MeshFace->NormalIndex[ 0 ] = *( ( WORD * )&Src[ 0 ] ) ;
			MeshFace->NormalIndex[ 1 ] = *( ( WORD * )&Src[ 2 ] ) ;
			MeshFace->NormalIndex[ 2 ] = *( ( WORD * )&Src[ 4 ] ) ;
			MeshFace->UVIndex[ 0 ][ 0 ] = *( ( WORD * )&Src[ 0 ] ) ;
			MeshFace->UVIndex[ 0 ][ 1 ] = *( ( WORD * )&Src[ 2 ] ) ;
			MeshFace->UVIndex[ 0 ][ 2 ] = *( ( WORD * )&Src[ 4 ] ) ;

			MeshFace ++ ;
			j ++ ;
		}
		PmdFaceNum = ( DWORD )j ;
	}

	// �h�j�̏����i�[���郁�����̈�̊m��
	if( PmdIKNum )
	{
		IKInfoDim = ( PMD_READ_IK_INFO * )DXALLOC( sizeof( PMD_READ_IK_INFO ) * PmdIKNum ) ;
		if( IKInfoDim == NULL )
		{
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x44\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xad\x8a\x7f\x30\xbc\x8f\x7f\x30\xe6\x51\x06\x74\x28\x75\x29\xff\x2b\xff\xc5\x60\x31\x58\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMD Load Error : �ǂݍ��ݏ����p�h�j�����i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
			goto ENDLABEL ;
		}
	}

	// �h�j�f�[�^�̒ǉ�
	Src = ( BYTE * )PmdIK ;
	IKInfo = IKInfoDim ;
	IKInfoFirst = NULL ;
	for( i = 0 ; i < PmdIKNum ; i ++, IKInfo ++ )
	{
		PMD_READ_BONE_INFO *BoneBone ;

		// �f�[�^���Z�b�g
		IKInfo->Base = ( PMD_IK * )Src ;
		IKInfo->Bone = &BoneInfoDim[ *( ( WORD * )&Src[ 0 ] ) ] ;
		IKInfo->TargetBone = &BoneInfoDim[ *( ( WORD * )&Src[ 2 ] ) ] ;
		IKInfo->TargetBone->IsIK = TRUE ;
		IKInfo->ChainBoneNum = Src[ 4 ] ;
		IKInfo->Iterations = *( ( WORD * )&Src[ 5 ] ) ;
		IKInfo->ControlWeight = *( ( float * )&Src[ 7 ] ) ;
		IKInfo->LimitAngleIK = ( WORD )( IKInfo->Bone->IKLimitAngle || IKInfo->TargetBone->IKLimitAngle ? 1 : 0 ) ;
		IKInfo->IKTotalLength = 0.0f ;
		Src += 11 ;
		IKInfo->ChainBone = ( WORD * )Src ;
		Src += sizeof( WORD ) * IKInfo->ChainBoneNum ;

		for( j = 0 ; j < IKInfo->ChainBoneNum ; j ++ )
		{
			BoneBone = &BoneInfoDim[ IKInfo->ChainBone[ j ] ] ;
			BoneInfoDim[ IKInfo->ChainBone[ j ] ].IsIK = TRUE ;
			if( BoneBone->IKLimitAngle ) IKInfo->LimitAngleIK = 1 ;
		}

		// ���X�g�ɒǉ�
		if( IKInfoFirst == NULL )
		{
			IKInfoFirst = IKInfo ;
			IKInfo->Prev = NULL ;
			IKInfo->Next = NULL ;
		}
		else
		{
			PMD_READ_IK_INFO *IKInfoTemp ;

			for( IKInfoTemp = IKInfoFirst ; IKInfoTemp->Next != NULL && *IKInfoTemp->ChainBone < *IKInfo->ChainBone ; IKInfoTemp = IKInfoTemp->Next ){}
			if( IKInfoTemp->Next == NULL && *IKInfoTemp->ChainBone < *IKInfo->ChainBone )
			{
				IKInfoTemp->Next = IKInfo ;
				IKInfo->Next = NULL ;
				IKInfo->Prev = IKInfoTemp ;
			}
			else
			{
				if( IKInfoTemp->Prev == NULL )
				{
					IKInfoTemp->Prev = IKInfo ;
					IKInfo->Next = IKInfoTemp ;
					IKInfo->Prev = NULL ;
					IKInfoFirst = IKInfo ;
				}
				else
				{
					IKInfo->Prev = IKInfoTemp->Prev ;
					IKInfo->Next = IKInfoTemp ;
					IKInfoTemp->Prev->Next = IKInfo ;
					IKInfoTemp->Prev = IKInfo ;
				}
			}
		}
	}

	// �h�j�̉e�����󂯂�{�[���̎q�łh�j�̉e�����󂯂Ȃ��{�[���Ɉ������
	BoneInfo = BoneInfoDim ;
	for( i = 0 ; i < PmdBoneNum ; i ++, BoneInfo ++ )
	{
		PMD_READ_BONE_INFO *ParentBone ;

		if( BoneInfo->IsIK )
			continue ;

		if( BoneInfo->Frame->Parent == NULL )
			continue ;

		ParentBone = ( PMD_READ_BONE_INFO * )BoneInfo->Frame->Parent->UserData ;
		if( ParentBone->IsIK )
		{
			BoneInfo->IsIKChild = TRUE ;
		}
	}

	// �\��f�[�^�̃A�h���X��ۑ�
	PmdSkinNum = *( ( WORD * )Src ) ;
	Src += 2 ;
	PmdSkin = Src ;
	if( PmdSkinNum > 1 )
	{
		for( i = 0 ; i < PmdSkinNum ; i ++ )
		{
			Src += 20 ;

			j = *( ( int * )Src ) ;
			Src += 5 + j * 16 ;
		}
	}

	// �\��g�p�\�����X�g�̓ǂݔ�΂�
	i = ( int )*Src ;
	Src += sizeof( BYTE ) + sizeof( WORD ) * i ;

	// �{�[���g�p�g�����X�g�̓ǂݔ�΂�
	BoneDispNum = ( int )*Src ;
	Src += sizeof( BYTE ) + sizeof( char ) * 50 * BoneDispNum ;

	// ����ȏ�f�[�^�����݂��Ȃ������畨�����Z�p�̃f�[�^�͂Ȃ��Ƃ�������
	if( Src - ( BYTE * )LoadParam->DataBuffer >= LoadParam->DataSize ) goto PHYSICSDATAREADEND ;

	// �{�[���g�p�\�����X�g�̓ǂݔ�΂�
	i = ( int )*( ( DWORD * )Src ) ;
	Src += sizeof( DWORD ) + 3 * i ;

	// ����ȏ�f�[�^�����݂��Ȃ������畨�����Z�p�̃f�[�^�͂Ȃ��Ƃ�������
	if( Src - ( BYTE * )LoadParam->DataBuffer >= LoadParam->DataSize ) goto PHYSICSDATAREADEND ;

	// �p�ꖼ���̓ǂݔ�΂�
	i = ( int )*Src ;
	Src ++ ;
	if( i )
	{
		// ���f�����ƃR�����g�̓ǂݔ�΂�
		Src += 276 ;

		// �{�[�����̓ǂݔ�΂�
		Src += PmdBoneNum * 20 ;

		// �\��̓ǂݔ�΂�
		if( PmdSkinNum > 1 )
		{
			Src += ( PmdSkinNum - 1 ) * 20 ;
		}

		// �{�[���g�p�g���̓ǂݔ�΂�
		Src += BoneDispNum * 50 ;
	}

	// ����ȏ�f�[�^�����݂��Ȃ������畨�����Z�p�̃f�[�^�͂Ȃ��Ƃ�������
	if( Src - ( BYTE * )LoadParam->DataBuffer >= LoadParam->DataSize ) goto PHYSICSDATAREADEND ;

	// �g�D�[���V�F�[�f�B���O�p�e�N�X�`���t�@�C�������X�g�̃A�h���X���Z�b�g
	PmdToonFileName = ( char * )Src ;
	Src += 100 * 10 ;

	// ����ȏ�f�[�^�����݂��Ȃ������畨�����Z�p�̃f�[�^�͂Ȃ��Ƃ�������
	if( Src - ( BYTE * )LoadParam->DataBuffer >= LoadParam->DataSize ) goto PHYSICSDATAREADEND ;

#ifndef DX_NON_BULLET_PHYSICS
	// �������Z�f�[�^�̓ǂݍ���
	if( LoadParam->GParam.LoadModelToUsePhysicsMode != DX_LOADMODEL_PHYSICS_DISABLE )
	{
		// �������Z�f�[�^������ꍇ�̂ݏ���������
		if( *( ( DWORD * )Src ) > 0 && *( ( DWORD * )Src ) != 0xfdfdfdfd )
		{
			PMD_READ_PHYSICS_INFO *PhysicsInfo ;
			PMD_READ_PHYSICS_JOINT_INFO *JointInfo ;

			// �����ɂ����畨�����Z�p�f�[�^������Ƃ�������
			ValidPhysics = TRUE ;

			// �������Z���̓ǂݍ���

			// �[��������
			_MEMSET( &MLPhysicsInfo, 0, sizeof( DX_MODELLOADER3_PMD_PHYSICS_INFO ) ) ;

			// �������Z���̐����擾
			MLPhysicsInfo.PmdPhysicsNum = *( ( int * )Src ) ;
			Src += 4 ;

			// �f�[�^���i�[���郁�����̈�̊m��
			MLPhysicsInfo.PmdPhysicsInfoDim = ( PMD_READ_PHYSICS_INFO * )DXALLOC( sizeof( PMD_READ_PHYSICS_INFO ) * MLPhysicsInfo.PmdPhysicsNum ) ;
			if( MLPhysicsInfo.PmdPhysicsInfoDim == NULL )
			{
				DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x44\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\x69\x72\x06\x74\x14\x6f\x97\x7b\xc5\x60\x31\x58\x4d\x91\x17\x52\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMD Load Error : �������Z���z����i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
				goto ENDLABEL ;
			}

			// �f�[�^��ǂݏo��
		//	PmdPhysics = ( PMD_PHYSICS * )Src ;
			PhysicsInfo = MLPhysicsInfo.PmdPhysicsInfoDim ;
			for( i = 0 ; i < MLPhysicsInfo.PmdPhysicsNum ; i ++, PhysicsInfo ++, Src += 83 )
			{
				PhysicsInfo->Base = ( PMD_PHYSICS * )Src ;

				_MEMCPY( PhysicsInfo->Name, ( char * )Src, 20 ) ;
				PhysicsInfo->Name[ 20 ] = '\0' ;
				PhysicsInfo->ShapeType = ( int )Src[ 25 ] ;
				PhysicsInfo->BoneIndex = *( ( WORD * )&Src[ 20 ] ) ;
				if( PhysicsInfo->BoneIndex == 0xffff )
				{
					for( j = 0 ; _WCSCMP( BoneInfoDim[ j ].Frame->NameW, CenterString_WCHAR_T ) != 0 ; j ++ ){}
					PhysicsInfo->Bone = &BoneInfoDim[ j ] ;
				}
				else
				{
					PhysicsInfo->Bone = &BoneInfoDim[ PhysicsInfo->BoneIndex ] ;
				}
				PhysicsInfo->Bone->IsPhysics = 1 ;
				PhysicsInfo->Bone->PhysicsIndex = i ;
				PhysicsInfo->Bone->SetupPhysicsAnim = 0 ;

				// ���̃^�C�v��ۑ�
				PhysicsInfo->RigidBodyType = Src[ 82 ] ;
				PhysicsInfo->NoCopyToBone = _WCSCMP( PhysicsInfo->Bone->Frame->NameW, CenterString_WCHAR_T ) == 0 ;
			}

			// �W���C���g���̐����擾
			MLPhysicsInfo.PmdPhysicsJointNum = *( ( int * )Src ) ;
			Src += 4 ;

			// �W���C���g��񂪂���ꍇ�̂ݏ���
			if( MLPhysicsInfo.PmdPhysicsJointNum != 0 )
			{
				// �f�[�^���i�[���郁�����̈�̊m��
				MLPhysicsInfo.PmdPhysicsJointInfoDim = ( PMD_READ_PHYSICS_JOINT_INFO * )DXALLOC( sizeof( PMD_READ_PHYSICS_JOINT_INFO ) * MLPhysicsInfo.PmdPhysicsJointNum ) ;
				if( MLPhysicsInfo.PmdPhysicsJointInfoDim == NULL )
				{
					DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x44\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\x69\x72\x06\x74\x14\x6f\x97\x7b\xb8\x30\xe7\x30\xa4\x30\xf3\x30\xc8\x30\xc5\x60\x31\x58\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMD Load Error : �������Z�W���C���g�����i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
					goto ENDLABEL ;
				}

				// �f�[�^��ǂݏo��
			//	PmdPhysicsJoint = ( PMD_PHYSICS_JOINT * )Src ;
				JointInfo = MLPhysicsInfo.PmdPhysicsJointInfoDim ;
				for( i = 0 ; i < MLPhysicsInfo.PmdPhysicsJointNum ; i ++, JointInfo ++, Src += 124 )
				{
					JointInfo->Base = ( PMD_PHYSICS_JOINT * )Src ;
				}
			}

			// �������A���^�C�������������s���ꍇ�͓ǂݍ��ݏ��ɕ����{�[���ƕ����W���C���g�̏���������
			if( LoadParam->GParam.LoadModelToUsePhysicsMode == DX_LOADMODEL_PHYSICS_REALTIME )
			{
				MV1_PHYSICS_RIGIDBODY_R *RigidBody ;
				MV1_PHYSICS_JOINT_R *Joint ;

				PhysicsInfo = MLPhysicsInfo.PmdPhysicsInfoDim ;
				for( i = 0 ; i < MLPhysicsInfo.PmdPhysicsNum ; i ++, PhysicsInfo ++ )
				{
					RigidBody = MV1RAddPhysicsRididBody( &RModel, PhysicsInfo->Name, PhysicsInfo->Bone->Frame ) ;
					if( RigidBody == NULL )
					{
						DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x44\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\x69\x72\x06\x74\x08\x8a\x97\x7b\x28\x75\x5b\x52\x53\x4f\xc5\x60\x31\x58\x6e\x30\xfd\x8f\xa0\x52\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMD Load Error : �����v�Z�p���̏��̒ǉ��Ɏ��s���܂���\n" @*/ )) ;
						goto ENDLABEL ;
					}

					RigidBody->RigidBodyGroupIndex = ( int )*( ( BYTE * )&PhysicsInfo->Base->Data[ 22 ] ) ;
					RigidBody->RigidBodyGroupTarget = *( ( WORD * )&PhysicsInfo->Base->Data[ 23 ] ) ;
					RigidBody->ShapeType = ( int )*( ( BYTE * )&PhysicsInfo->Base->Data[ 25 ] ) ;
					*( ( DWORD * )&RigidBody->ShapeW ) = *( ( DWORD * )&PhysicsInfo->Base->Data[ 26 ] ) ;
					*( ( DWORD * )&RigidBody->ShapeH ) = *( ( DWORD * )&PhysicsInfo->Base->Data[ 30 ] ) ;
					*( ( DWORD * )&RigidBody->ShapeD ) = *( ( DWORD * )&PhysicsInfo->Base->Data[ 34 ] ) ;
					*( ( DWORD * )&RigidBody->Position.x ) = *( ( DWORD * )&PhysicsInfo->Base->Data[ 38 ] ) ;
					*( ( DWORD * )&RigidBody->Position.y ) = *( ( DWORD * )&PhysicsInfo->Base->Data[ 42 ] ) ;
					*( ( DWORD * )&RigidBody->Position.z ) = *( ( DWORD * )&PhysicsInfo->Base->Data[ 46 ] ) ;
					*( ( DWORD * )&RigidBody->Rotation.x ) = *( ( DWORD * )&PhysicsInfo->Base->Data[ 50 ] ) ;
					*( ( DWORD * )&RigidBody->Rotation.y ) = *( ( DWORD * )&PhysicsInfo->Base->Data[ 54 ] ) ;
					*( ( DWORD * )&RigidBody->Rotation.z ) = *( ( DWORD * )&PhysicsInfo->Base->Data[ 58 ] ) ;
					*( ( DWORD * )&RigidBody->RigidBodyWeight ) = *( ( DWORD * )&PhysicsInfo->Base->Data[ 62 ] ) ;
					*( ( DWORD * )&RigidBody->RigidBodyPosDim ) = *( ( DWORD * )&PhysicsInfo->Base->Data[ 66 ] ) ;
					*( ( DWORD * )&RigidBody->RigidBodyRotDim ) = *( ( DWORD * )&PhysicsInfo->Base->Data[ 70 ] ) ;
					*( ( DWORD * )&RigidBody->RigidBodyRecoil ) = *( ( DWORD * )&PhysicsInfo->Base->Data[ 74 ] ) ;
					*( ( DWORD * )&RigidBody->RigidBodyFriction ) = *( ( DWORD * )&PhysicsInfo->Base->Data[ 78 ] ) ;
					RigidBody->RigidBodyType = ( int )*( ( BYTE * )&PhysicsInfo->Base->Data[ 82 ] ) ;
					RigidBody->NoCopyToBone = _WCSCMP( PhysicsInfo->Bone->Frame->NameW, CenterString_WCHAR_T ) == 0 ? TRUE : FALSE ;
				}

				JointInfo = MLPhysicsInfo.PmdPhysicsJointInfoDim ;
				for( i = 0 ; i < MLPhysicsInfo.PmdPhysicsJointNum ; i ++, JointInfo ++ )
				{
					Joint = MV1RAddPhysicsJoint( &RModel, JointInfo->Base->Name ) ;
					if( Joint == NULL )
					{
						DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x44\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\x69\x72\x06\x74\x08\x8a\x97\x7b\x28\x75\x5b\x52\x53\x4f\xa5\x63\x08\x54\xc5\x60\x31\x58\x6e\x30\xfd\x8f\xa0\x52\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMD Load Error : �����v�Z�p���̐ڍ����̒ǉ��Ɏ��s���܂���\n" @*/ )) ;
						goto ENDLABEL ;
					}

					Joint->RigidBodyA = JointInfo->Base->RigidBodyA ;
					Joint->RigidBodyB = JointInfo->Base->RigidBodyB ;
					Joint->Position.x = JointInfo->Base->Position[ 0 ] ;
					Joint->Position.y = JointInfo->Base->Position[ 1 ] ;
					Joint->Position.z = JointInfo->Base->Position[ 2 ] ;
					Joint->Rotation.x = JointInfo->Base->Rotation[ 0 ] ;
					Joint->Rotation.y = JointInfo->Base->Rotation[ 1 ] ;
					Joint->Rotation.z = JointInfo->Base->Rotation[ 2 ] ;
					Joint->ConstrainPosition1.x = JointInfo->Base->ConstrainPosition1[ 0 ] ;
					Joint->ConstrainPosition1.y = JointInfo->Base->ConstrainPosition1[ 1 ] ;
					Joint->ConstrainPosition1.z = JointInfo->Base->ConstrainPosition1[ 2 ] ;
					Joint->ConstrainPosition2.x = JointInfo->Base->ConstrainPosition2[ 0 ] ;
					Joint->ConstrainPosition2.y = JointInfo->Base->ConstrainPosition2[ 1 ] ;
					Joint->ConstrainPosition2.z = JointInfo->Base->ConstrainPosition2[ 2 ] ;
					Joint->ConstrainRotation1.x = JointInfo->Base->ConstrainRotation1[ 0 ] ;
					Joint->ConstrainRotation1.y = JointInfo->Base->ConstrainRotation1[ 1 ] ;
					Joint->ConstrainRotation1.z = JointInfo->Base->ConstrainRotation1[ 2 ] ;
					Joint->ConstrainRotation2.x = JointInfo->Base->ConstrainRotation2[ 0 ] ;
					Joint->ConstrainRotation2.y = JointInfo->Base->ConstrainRotation2[ 1 ] ;
					Joint->ConstrainRotation2.z = JointInfo->Base->ConstrainRotation2[ 2 ] ;
					Joint->SpringPosition.x = JointInfo->Base->SpringPosition[ 0 ] ;
					Joint->SpringPosition.y = JointInfo->Base->SpringPosition[ 1 ] ;
					Joint->SpringPosition.z = JointInfo->Base->SpringPosition[ 2 ] ;
					Joint->SpringRotation.x = JointInfo->Base->SpringRotation[ 0 ] * DX_PI_F / 180.0f ;
					Joint->SpringRotation.y = JointInfo->Base->SpringRotation[ 1 ] * DX_PI_F / 180.0f ;
					Joint->SpringRotation.z = JointInfo->Base->SpringRotation[ 2 ] * DX_PI_F / 180.0f ;
				}
			}
		}
	}
	else
#endif
	// �������Z�f�[�^�̕������A�h���X��i�߂�
	{
		// �������Z�f�[�^������ꍇ�̂ݐi�߂�
		if( *( ( DWORD * )Src ) > 0 && *( ( DWORD * )Src ) != 0xfdfdfdfd )
		{
			// �������Z���̕������i�߂�
			i = *( ( int * )Src ) ;
			Src += 83 * i + 4 ;

			// �W���C���g���̕������i�߂�
			i = *( ( int * )Src ) ;
			Src += 124 * i + 4 ;
		}
	}

PHYSICSDATAREADEND :

	// �}�e���A���̓ǂ݂���
	Src = ( BYTE * )PmdMaterial ;
	facecount = 0 ;
	for( i = 0 ; ( DWORD )i < PmdMaterialNum ; i ++ )
	{
		char *ap ;

		_SNPRINTF( String, sizeof( String ), "Mat_%d", i ) ;
		Material = MV1RAddMaterial( &RModel, String ) ;
		if( Material == NULL )
		{
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x44\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\x4e\x00\x6f\x00\x2e\x00\x25\x00\x64\x00\x20\x00\x6e\x30\x20\x00\x4d\x00\x61\x00\x74\x00\x65\x00\x72\x00\x69\x00\x61\x00\x6c\x00\x20\x00\xaa\x30\xd6\x30\xb8\x30\xa7\x30\xaf\x30\xc8\x30\x6e\x30\xfd\x8f\xa0\x52\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMD Load Error : No.%d �� Material �I�u�W�F�N�g�̒ǉ��Ɏ��s���܂���\n" @*/, i ) ) ;
			goto ENDLABEL ;
		}

		// �}�e���A���^�C�v�̓g�D�[��
		Material->Type = DX_MATERIAL_TYPE_TOON_2 ;

		// �����Z�b�g
		*( ( DWORD * )&Material->Diffuse.r ) = *( ( DWORD * )&Src[  0 ] ) ;
		*( ( DWORD * )&Material->Diffuse.g ) = *( ( DWORD * )&Src[  4 ] ) ;
		*( ( DWORD * )&Material->Diffuse.b ) = *( ( DWORD * )&Src[  8 ] ) ;
		Src += 12 ;

		*( ( DWORD * )&Material->Diffuse.a ) = *( ( DWORD * )&Src[ 0 ] ) ;
		Src += 4 ;

		Material->Power = *( ( float * )Src ) ;
		Src += 4 ;

		*( ( DWORD * )&Material->Specular.r ) = *( ( DWORD * )&Src[ 0 ] ) ;
		*( ( DWORD * )&Material->Specular.g ) = *( ( DWORD * )&Src[ 4 ] ) ;
		*( ( DWORD * )&Material->Specular.b ) = *( ( DWORD * )&Src[ 8 ] ) ;
		Material->Specular.a = 0.0f ;
		Src += 12 ;

		*( ( DWORD * )&Material->Ambient.r ) = *( ( DWORD * )&Src[ 0 ] ) ;
		*( ( DWORD * )&Material->Ambient.g ) = *( ( DWORD * )&Src[ 4 ] ) ;
		*( ( DWORD * )&Material->Ambient.b ) = *( ( DWORD * )&Src[ 8 ] ) ;
		Material->Ambient.a = 0.0f ;
		Src += 12 ;

		// �g�D�[���e�N�X�`����ǉ�
		ToonTexIndex = *Src ;
		{
			// �X�t�B�A�}�b�v�̃t�@�C�������܂܂��ꍇ�͂��������
			if( *Src != 0xff && PmdToonFileName != NULL )
			{
				_STRCPY_S( String, sizeof( String ), &PmdToonFileName[ *Src * 100 ] ) ;
				ap = ( char * )_STRCHR( String, '*' ) ;
				if( ap != NULL ) *ap = '\0' ;
			}
			else
			{
				_STRCPY_S( String, sizeof( String ), "toon0.bmp" ) ;
			}
			Texture = MV1RAddTexture( &RModel, String, String, NULL, FALSE, 0.1f, true, true, true ) ;
			if( Texture == NULL )
			{
				DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x44\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\x4e\x00\x6f\x00\x2e\x00\x25\x00\x64\x00\x20\x00\x6e\x30\x20\x00\xc8\x30\xa5\x30\xfc\x30\xf3\x30\x28\x75\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\xaa\x30\xd6\x30\xb8\x30\xa7\x30\xaf\x30\xc8\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMD Load Error : No.%d �� �g�D�[���p�e�N�X�`���I�u�W�F�N�g�̍쐬�Ɏ��s���܂���\n" @*/, i ) ) ;
				goto ENDLABEL ;
			}
			Texture->AddressModeU = DX_TEXADDRESS_CLAMP ;
			Texture->AddressModeV = DX_TEXADDRESS_CLAMP ;
			Material->DiffuseGradTexture = Texture ;
			Material->DiffuseGradBlendType = MV1_LAYERBLEND_TYPE_MODULATE ;
		}
		Material->SpecularGradTextureDefault = -1 ;
		Material->SpecularGradBlendType = MV1_LAYERBLEND_TYPE_ADDITIVE ;
		Src ++ ;

		// ���̑������Z�b�g
		if( *Src )
		{
			Material->OutLineWidth = 0.01f ;
			Material->OutLineDotWidth = 1.1f ;
		}
		Src ++ ;

		// �ʃf�[�^�Ƀ}�e���A���ԍ����Z�b�g����
		PmdMaterialFaceVertexNum = *( ( DWORD * )Src ) ;
		Src += 4 ;
		for( j = 0 ; ( DWORD )j < PmdMaterialFaceVertexNum ; j += 3, facecount ++ )
		{
			Mesh->Faces[ facecount ].MaterialIndex = Mesh->MaterialNum ;
		}
		
		// �e�N�X�`����ǉ�
		_MEMSET( String, 0, sizeof( String ) ) ;
		_MEMCPY( String, Src, 20 ) ;
		Src += 20 ;
		if( String[ 0 ] != '\0' )
		{
			char SphFileName[ 32 ], TexFileName[ 32 ] ;
			int SphBlendType ;
			int len ;

			SphFileName[ 0 ] = '\0' ;
			TexFileName[ 0 ] = '\0' ;
			SphBlendType = DX_MATERIAL_BLENDTYPE_MODULATE ;

			// �t�@�C�������
			for(;;)
			{
				for( j = 0 ; String[ j ] != '.' && String[ j ] != '\0' && String[ j ] != '*' ; j ++ ){}

				// �s���I�h�ɍs��������X�t�B�A�}�b�v���ǂ����𔻒肷�邽�߂Ɋg���q����
				if( String[ j ] == '.' )
				{
					// �ŏ��̃t�@�C�������X�t�B�A�}�b�v���ǂ����ŏ����𕪊�
					if( String[ j + 1 ] == 's' && String[ j + 2 ] == 'p' && ( String[ j + 3 ] == 'h' || String[ j + 3 ] == 'a' ) )
					{
						// �X�t�B�A�}�b�v�̃t�@�C������ۑ�����
						_MEMCPY( SphFileName, String, ( size_t )( j + 4 ) ) ;
						SphFileName[ j + 4 ] = '\0' ;

						// �X�t�B�A�}�b�v�̃u�����h�����𒲂ׂ�
						SphBlendType = String[ j + 3 ] == 'a' ? DX_MATERIAL_BLENDTYPE_ADDITIVE : DX_MATERIAL_BLENDTYPE_MODULATE ;
						break ;
					}

					// �X�t�B�A�}�b�v�ł͂Ȃ������ꍇ�͕��ʂ̃e�N�X�`���Ȃ̂ŁA�p�����ĕ�����̏I�[�܂� j ��i�߂�
					for( j = 0 ; String[ j ] != '\0' && String[ j ] != '*' ; j ++ ){}
				}

				// �e�N�X�`���t�@�C������ۑ�
				_MEMCPY( TexFileName, String, ( size_t )j ) ;
				TexFileName[ j ] = '\0' ;

				// ������̏I�[�������炱���ŏ����͏I��
				if( String[ j ] == '\0' )
					break ;

				// �����ɂ����� String[ j ] == '*' �Ƃ�������
				j ++ ;

				// �X�t�B�A�}�b�v�̃t�@�C������ۑ�����
				len = ( int )_STRLEN( &String[ j ] ) ;
				_MEMCPY( SphFileName, &String[ j ], ( size_t )len ) ;
				SphFileName[ len ] = '\0' ;

				// �X�t�B�A�}�b�v�̃u�����h�����𒲂ׂ�
				SphBlendType = String[ j + len - 1 ] == 'a' ? DX_MATERIAL_BLENDTYPE_ADDITIVE : DX_MATERIAL_BLENDTYPE_MODULATE ;
				break ;
			}

//			ap = _STRCHR( String, '*' ) ;
//			if( ap != NULL ) *ap = '\0' ;

			// ���ʂ̃e�N�X�`��������ꍇ�̏���
			if( TexFileName[ 0 ] != '\0' )
			{
				Texture = MV1RAddTexture( &RModel, TexFileName, TexFileName, NULL, false, 0.1f, true, false, true ) ;
				if( Texture == NULL )
				{
					DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x44\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\x4e\x00\x6f\x00\x2e\x00\x25\x00\x64\x00\x20\x00\x6e\x30\x20\x00\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\xaa\x30\xd6\x30\xb8\x30\xa7\x30\xaf\x30\xc8\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMD Load Error : No.%d �� �e�N�X�`���I�u�W�F�N�g�̍쐬�Ɏ��s���܂���\n" @*/, i ) ) ;
					goto ENDLABEL ;
				}
				Material->DiffuseTexNum = 1 ;
				Material->DiffuseTexs[ 0 ] = Texture ;
			}

			// �X�t�B�A�}�b�v�e�N�X�`��������ꍇ�̏���
			if( SphFileName[ 0 ] != '\0' )
			{
				Texture = MV1RAddTexture( &RModel, SphFileName, SphFileName, NULL, false, 0.1f, true, false, true ) ;
				if( Texture == NULL )
				{
					DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x44\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\x4e\x00\x6f\x00\x2e\x00\x25\x00\x64\x00\x20\x00\x6e\x30\x20\x00\xb9\x30\xd5\x30\xa3\x30\xa2\x30\xde\x30\xc3\x30\xd7\x30\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\xaa\x30\xd6\x30\xb8\x30\xa7\x30\xaf\x30\xc8\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMD Load Error : No.%d �� �X�t�B�A�}�b�v�e�N�X�`���I�u�W�F�N�g�̍쐬�Ɏ��s���܂���\n" @*/, i ) ) ;
					goto ENDLABEL ;
				}
				Material->SphereMapTexture = Texture ;
				Material->SphereMapBlendType = SphBlendType ;
			}
		}


		// �}�e���A�����P�|���S���ɂł��g�p����Ă���ꍇ�̓}�e���A���̃A�h���X���Z�b�g
		if( PmdMaterialFaceVertexNum )
		{
			Mesh->Materials[ Mesh->MaterialNum ] = Material ;
			Mesh->MaterialNum ++ ;
		}
	}

	// �\��f�[�^�̒ǉ�
	if( PmdSkinNum > 1 )
	{
		Src = PmdSkin ;

		// �\��f�[�^�{�̂̒��_�����擾
		Src += 20 ;
		SkinBaseVertNum = *( ( DWORD * )Src ) ;

		// �\��f�[�^�̒��_�����O�̏ꍇ�͕\��f�[�^�̓ǂݍ��݂����Ȃ�
		if( SkinBaseVertNum != 0 )
		{
			SkinMesh = MV1RAddMesh( &RModel, Frame ) ;
			if( SkinMesh == NULL )
			{
				DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x44\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xb9\x30\xad\x30\xf3\x30\xe1\x30\xc3\x30\xb7\x30\xe5\x30\xaa\x30\xd6\x30\xb8\x30\xa7\x30\xaf\x30\xc8\x30\x6e\x30\xfd\x8f\xa0\x52\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMD Load Error : �X�L�����b�V���I�u�W�F�N�g�̒ǉ��Ɏ��s���܂���\n" @*/ )) ;
				goto ENDLABEL ;
			}

			// base ���b�V�����猳���f������g�p���钸�_�Ɩʂ̏�������o��
			SkinNextVertIndex = ( DWORD * )DXALLOC( sizeof( DWORD ) * ( 2 * PmdVertexNum + 2 * PmdFaceNum + 2 * PmdMaterialNum ) ) ;
			if( SkinNextVertIndex == NULL )
			{
				DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x44\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\x00\x4e\x42\x66\xdd\x4f\x58\x5b\x28\x75\x6e\x30\x02\x98\xb9\x70\xc7\x30\xfc\x30\xbf\x30\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMD Load Error : �ꎞ�ۑ��p�̒��_�f�[�^���i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
				goto ENDLABEL ;
			}
			SkinPrevVertIndex     = SkinNextVertIndex     + PmdVertexNum ;
			SkinNextFaceIndex     = SkinPrevVertIndex     + PmdVertexNum ;
			SkinPrevFaceIndex     = SkinNextFaceIndex     + PmdFaceNum ;
			SkinNextMaterialIndex = SkinPrevFaceIndex     + PmdFaceNum ;
			SkinPrevMaterialIndex = SkinNextMaterialIndex + PmdMaterialNum ;
			_MEMSET( SkinNextVertIndex,     0xff, sizeof( DWORD ) * PmdVertexNum   ) ;
			_MEMSET( SkinPrevVertIndex,     0xff, sizeof( DWORD ) * PmdVertexNum   ) ;
			_MEMSET( SkinNextFaceIndex,     0xff, sizeof( DWORD ) * PmdFaceNum     ) ;
			_MEMSET( SkinPrevFaceIndex,     0xff, sizeof( DWORD ) * PmdFaceNum     ) ;
			_MEMSET( SkinNextMaterialIndex, 0xff, sizeof( DWORD ) * PmdMaterialNum ) ;
			_MEMSET( SkinPrevMaterialIndex, 0xff, sizeof( DWORD ) * PmdMaterialNum ) ;

			// �\��f�[�^�{�̂Ŏg�p���钸�_�̌��̃��b�V���ł̒��_�ԍ����擾
			Src += 5 ;
			PmdBaseSkinVert = ( PMD_SKIN_VERT * )Src ;
			SkinVertNum = 0 ;
			for( i = 0 ; ( DWORD )i < SkinBaseVertNum ; i ++, Src += 16 )
			{
				SkinTargetVert = *( ( DWORD * )Src ) ;
				SkinPrevVertIndex[ SkinVertNum ] = SkinTargetVert ;
				SkinNextVertIndex[ SkinTargetVert ] = SkinVertNum ;
				SkinVertNum ++ ;
			}

			// �\��f�[�^�Ŏg�p����Ă��钸�_���g���Ă���ʂ̐��𐔂���
			MeshFace = Mesh->Faces ;
			SkinFaceNum = 0 ;
			SkinMaterialNum = 0 ;
			for( i = 0 ; ( DWORD )i < PmdFaceNum ; i ++, MeshFace ++ )
			{
				if( SkinNextVertIndex[ MeshFace->VertexIndex[ 0 ] ] >= SkinBaseVertNum &&
					SkinNextVertIndex[ MeshFace->VertexIndex[ 1 ] ] >= SkinBaseVertNum &&
					SkinNextVertIndex[ MeshFace->VertexIndex[ 2 ] ] >= SkinBaseVertNum ) continue ;

				if( SkinNextMaterialIndex[ MeshFace->MaterialIndex ] == 0xffffffff )
				{
					SkinPrevMaterialIndex[ SkinMaterialNum ] = MeshFace->MaterialIndex ;
					SkinNextMaterialIndex[ MeshFace->MaterialIndex ] = SkinMaterialNum ;
					SkinMaterialNum ++ ;
				}

				SkinPrevFaceIndex[ SkinFaceNum ] = ( DWORD )i ;
				SkinNextFaceIndex[ i ] = SkinFaceNum ;
				SkinFaceNum ++ ;

				if( SkinNextVertIndex[ MeshFace->VertexIndex[ 0 ] ] == 0xffffffff )
				{
					SkinPrevVertIndex[ SkinVertNum ] = MeshFace->VertexIndex[ 0 ] ;
					SkinNextVertIndex[ MeshFace->VertexIndex[ 0 ] ] = SkinVertNum ;
					SkinVertNum ++ ;
				}

				if( SkinNextVertIndex[ MeshFace->VertexIndex[ 1 ] ] == 0xffffffff )
				{
					SkinPrevVertIndex[ SkinVertNum ] = MeshFace->VertexIndex[ 1 ] ;
					SkinNextVertIndex[ MeshFace->VertexIndex[ 1 ] ] = SkinVertNum ;
					SkinVertNum ++ ;
				}

				if( SkinNextVertIndex[ MeshFace->VertexIndex[ 2 ] ] == 0xffffffff )
				{
					SkinPrevVertIndex[ SkinVertNum ] = MeshFace->VertexIndex[ 2 ] ;
					SkinNextVertIndex[ MeshFace->VertexIndex[ 2 ] ] = SkinVertNum ;
					SkinVertNum ++ ;
				}
			}

			// �g�p����Ă���}�e���A���̃��X�g���쐬����
			SkinMesh->MaterialNum = SkinMaterialNum ;
			for( i = 0 ; ( DWORD )i < SkinMaterialNum ; i ++ )
			{
				SkinMesh->Materials[ i ] = Mesh->Materials[ SkinPrevMaterialIndex[ i ] ] ;
			}

			// �\��f�[�^�Ŏ��p����ʂ�ۑ����郁�����̈���m�ۂ���
			if( MV1RSetupMeshFaceBuffer( &RModel, SkinMesh, ( int )SkinFaceNum, 3 ) < 0 )
			{
				DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x44\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\x68\x88\xc5\x60\xc7\x30\xfc\x30\xbf\x30\x6e\x30\x62\x97\xc5\x60\x31\x58\x92\x30\xdd\x4f\x58\x5b\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMD Load Error : �\��f�[�^�̖ʏ���ۑ����郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
				goto ENDLABEL ;
			}

			// �\��f�[�^�Ŏg�p���钸�_��ۑ����郁�����̈���m�ۂ���
			SkinMesh->PositionNum = SkinVertNum ;
			SkinMesh->Positions = ( VECTOR * )ADDMEMAREA( ( sizeof( VECTOR ) + sizeof( float ) ) * SkinMesh->PositionNum, &RModel.Mem ) ;
			if( SkinMesh->Positions == NULL )
			{
				DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x44\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\x68\x88\xc5\x60\xc7\x30\xfc\x30\xbf\x30\x6e\x30\x02\x98\xb9\x70\xa7\x5e\x19\x6a\x92\x30\xdd\x4f\x58\x5b\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMD Load Error : �\��f�[�^�̒��_���W��ۑ����郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
				goto ENDLABEL ;
			}
			SkinMesh->PositionToonOutLineScale = ( float * )( SkinMesh->Positions + SkinMesh->PositionNum ) ;

			SkinMesh->NormalNum = SkinVertNum ;
			SkinMesh->Normals = ( VECTOR * )ADDMEMAREA( sizeof( VECTOR ) * SkinMesh->NormalNum, &RModel.Mem ) ;
			if( SkinMesh->Normals == NULL )
			{
				DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x44\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\x68\x88\xc5\x60\xc7\x30\xfc\x30\xbf\x30\x6e\x30\x02\x98\xb9\x70\xd5\x6c\xda\x7d\x92\x30\xdd\x4f\x58\x5b\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMD Load Error : �\��f�[�^�̒��_�@����ۑ����郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
				goto ENDLABEL ;
			}

			SkinMesh->UVNum[ 0 ] = SkinVertNum ;
			SkinMesh->UVs[ 0 ] = ( FLOAT4 * )ADDMEMAREA( sizeof( FLOAT4 ) * SkinMesh->UVNum[ 0 ], &RModel.Mem ) ;
			if( SkinMesh->UVs[ 0 ] == NULL )
			{
				DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x44\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\x68\x88\xc5\x60\xc7\x30\xfc\x30\xbf\x30\x6e\x30\x02\x98\xb9\x70\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\xa7\x5e\x19\x6a\x92\x30\xdd\x4f\x58\x5b\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMD Load Error : �\��f�[�^�̒��_�e�N�X�`�����W��ۑ����郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
				goto ENDLABEL ;
			}

			// �\��f�[�^�Ŏg�p���钸�_�̃f�[�^���Z�b�g����
			for( i = 0 ; ( DWORD )i < SkinVertNum ; i ++ )
			{
				SkinMesh->Positions[ i ] = Mesh->Positions[ SkinPrevVertIndex[ i ] ] ;
				SkinMesh->Normals[ i ] = Mesh->Normals[ SkinPrevVertIndex[ i ] ] ;
				SkinMesh->PositionToonOutLineScale[ i ] = Mesh->PositionToonOutLineScale[ SkinPrevVertIndex[ i ] ] ;
				SkinMesh->UVs[ 0 ][ i ] = Mesh->UVs[ 0 ][ SkinPrevVertIndex[ i ] ] ;
			}

			// �\��f�[�^�Ŏg�p����ʂ̃f�[�^���Z�b�g����
			MeshFace = SkinMesh->Faces ;
			for( i = 0 ; ( DWORD )i < SkinFaceNum ; i ++, MeshFace ++ )
			{
				MeshFaceTemp = &Mesh->Faces[ SkinPrevFaceIndex[ i ] ] ;
				MeshFace->IndexNum = 3 ;
				MeshFace->VertexIndex[ 0 ] = SkinNextVertIndex[ MeshFaceTemp->VertexIndex[ 0 ] ] ;
				MeshFace->VertexIndex[ 1 ] = SkinNextVertIndex[ MeshFaceTemp->VertexIndex[ 1 ] ] ;
				MeshFace->VertexIndex[ 2 ] = SkinNextVertIndex[ MeshFaceTemp->VertexIndex[ 2 ] ] ;
				MeshFace->NormalIndex[ 0 ] = MeshFace->VertexIndex[ 0 ] ;
				MeshFace->NormalIndex[ 1 ] = MeshFace->VertexIndex[ 1 ] ;
				MeshFace->NormalIndex[ 2 ] = MeshFace->VertexIndex[ 2 ] ;
				MeshFace->UVIndex[ 0 ][ 0 ] = MeshFace->VertexIndex[ 0 ] ;
				MeshFace->UVIndex[ 0 ][ 1 ] = MeshFace->VertexIndex[ 1 ] ;
				MeshFace->UVIndex[ 0 ][ 2 ] = MeshFace->VertexIndex[ 2 ] ;
				MeshFace->MaterialIndex = SkinNextMaterialIndex[ MeshFaceTemp->MaterialIndex ] ;
			}

			// ���̃��b�V���ŕ\��f�[�^�Ɏg�p����Ă���ʂ𖳌�������
			NextFaceNum = 0 ;
			MeshFaceTemp = Mesh->Faces ;
			MeshFace = Mesh->Faces ;
			for( i = 0 ; ( DWORD )i < Mesh->FaceNum ; i ++, MeshFaceTemp ++ )
			{
				if( SkinNextFaceIndex[ i ] != 0xffffffff ) continue ;
				*MeshFace = *MeshFaceTemp ;
				MeshFace ++ ;
				NextFaceNum ++ ;
			}
			Mesh->FaceNum = NextFaceNum ;

			// �X�L�j���O���b�V�������\�z����
			for( i = 0 ; ( DWORD )i < Mesh->SkinWeightsNum ; i ++ )
			{
				// �\��f�[�^�Ŏg�p���邩�ǂ����𒲂ׂ�
				SkinWeightTemp = Mesh->SkinWeights[ i ] ;
				SkinWTemp = SkinWeightTemp->Data ;
				weightcount = 0 ;
				for( j = 0 ; ( DWORD )j < SkinWeightTemp->DataNum ; j ++, SkinWTemp ++ )
				{
					if( SkinNextVertIndex[ SkinWTemp->TargetVertex ] == 0xffffffff ) continue ;
					weightcount ++ ;
				}
				if( weightcount == 0 ) continue ;

				// �g���ꍇ�͒ǉ�
				SkinMesh->SkinWeights[ SkinMesh->SkinWeightsNum ] = MV1RAddSkinWeight( &RModel ) ;
				SkinWeight = SkinMesh->SkinWeights[ SkinMesh->SkinWeightsNum ] ;
				SkinMesh->SkinWeightsNum ++ ;

				SkinWeight->ModelLocalMatrix = SkinWeightTemp->ModelLocalMatrix ;
				SkinWeight->TargetFrame = SkinWeightTemp->TargetFrame ;
				SkinWeight->Data = ( MV1_SKIN_WEIGHT_ONE_R * )ADDMEMAREA( sizeof( MV1_SKIN_WEIGHT_ONE_R ) * weightcount, &RModel.Mem ) ;
				if( SkinWeight->Data == NULL )
				{
					DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x44\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\x68\x88\xc5\x60\xc7\x30\xfc\x30\xbf\x30\x28\x75\xb9\x30\xad\x30\xcb\x30\xf3\x30\xb0\x30\xe1\x30\xc3\x30\xb7\x30\xe5\x30\xa6\x30\xa8\x30\xa4\x30\xc8\x30\x24\x50\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMD Load Error : �\��f�[�^�p�X�L�j���O���b�V���E�G�C�g�l���i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
					return -1 ;
				}

				// �f�[�^���Z�b�g
				SkinW = SkinWeight->Data ;
				SkinWTemp = SkinWeightTemp->Data ;
				for( j = 0 ; ( DWORD )j < SkinWeightTemp->DataNum ; j ++, SkinWTemp ++ )
				{
					if( SkinNextVertIndex[ SkinWTemp->TargetVertex ] == 0xffffffff ) continue ;
					SkinW->TargetVertex = SkinNextVertIndex[ SkinWTemp->TargetVertex ] ;
					SkinW->Weight = SkinWTemp->Weight ;
					SkinW ++ ;
					SkinWeight->DataNum ++ ;
				}
			}

			// ���̃��b�V���ŕ\��f�[�^�Ɏg�p����Ă���X�L�j���O���b�V�����𖳌�������
			for( i = 0 ; ( DWORD )i < Mesh->SkinWeightsNum ; i ++ )
			{
				// �\��f�[�^�Ŏg�p���邩�ǂ����𒲂ׂ�
				SkinWeightTemp = Mesh->SkinWeights[ i ] ;
				SkinWTemp = SkinWeightTemp->Data ;
				SkinW = SkinWeightTemp->Data ;
				weightcount = 0 ;
				for( j = 0 ; ( DWORD )j < SkinWeightTemp->DataNum - 1 ; j ++, SkinWTemp ++ )
				{
					if( SkinNextVertIndex[ SkinWTemp->TargetVertex ] < SkinBaseVertNum ) continue ;
					*SkinW = *SkinWTemp ;
					SkinW ++ ;
					weightcount ++ ;
				}
				// �Ō�̈�͕ʏ���
				if( weightcount == 0 || SkinNextVertIndex[ SkinWTemp->TargetVertex ] >= SkinBaseVertNum )
				{
					*SkinW = *SkinWTemp ;
					SkinW ++ ;
					weightcount ++ ;
				}
				SkinWeightTemp->DataNum = ( DWORD )weightcount ;
			}

			// �c��̕\��f�[�^��ǉ�����
			for( i = 1 ; i < PmdSkinNum ; i ++ )
			{
				// �\��f�[�^�̒ǉ�
				_MEMSET( String, 0, sizeof( String ) ) ;
				_MEMCPY( String, Src, 20 ) ;
				Shape = MV1RAddShape( &RModel, String, Frame ) ; 
				if( Shape == NULL )
				{
					DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x44\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xb7\x30\xa7\x30\xa4\x30\xd7\x30\xaa\x30\xd6\x30\xb8\x30\xa7\x30\xaf\x30\xc8\x30\x6e\x30\xfd\x8f\xa0\x52\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMD Load Error : �V�F�C�v�I�u�W�F�N�g�̒ǉ��Ɏ��s���܂���\n" @*/ )) ;
					goto ENDLABEL ;
				}
				Src += 20 ;

				// �Ώۃ��b�V���̃Z�b�g
				Shape->TargetMesh = SkinMesh ;

				// �@���͖���
				Shape->ValidVertexNormal = FALSE ;

				// ���_�̐���ۑ�
				Shape->VertexNum = *( ( int * )Src ) ;
				Src += 5 ;

				// ���_�f�[�^���i�[���郁�����̈�̊m��
				Shape->Vertex = ( MV1_SHAPE_VERTEX_R * )ADDMEMAREA( sizeof( MV1_SHAPE_VERTEX_R ) * Shape->VertexNum, &RModel.Mem ) ;
				if( Shape->Vertex == NULL )
				{
					DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x44\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xb7\x30\xa7\x30\xa4\x30\xd7\x30\x02\x98\xb9\x70\xc7\x30\xfc\x30\xbf\x30\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMD Load Error : �V�F�C�v���_�f�[�^���i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
					goto ENDLABEL ;
				}

				// ���_�f�[�^�𖄂߂�
				ShapeVert = Shape->Vertex ;
				for( j = 0 ; j < Shape->VertexNum ; j ++, ShapeVert ++, Src += 16 )
				{
					SkinTargetVert = *( ( DWORD * )&Src[ 0 ] ) ;
					PmdSkinVert = &PmdBaseSkinVert[ SkinTargetVert ] ;
					ShapeVert->TargetPositionIndex = ( int )SkinTargetVert ;
					ShapeVert->Position.x = ( PmdSkinVert->Position[ 0 ] - SkinMesh->Positions[ SkinTargetVert ].x ) + *( ( float * )&Src[  4 ] ) ;
					ShapeVert->Position.y = ( PmdSkinVert->Position[ 1 ] - SkinMesh->Positions[ SkinTargetVert ].y ) + *( ( float * )&Src[  8 ] ) ;
					ShapeVert->Position.z = ( PmdSkinVert->Position[ 2 ] - SkinMesh->Positions[ SkinTargetVert ].z ) + *( ( float * )&Src[ 12 ] ) ;
				}
			}
		}
	}

	// VMD�t�@�C������������ǂݍ���
	{
		int FileSize ;
		void *VmdData ;
		int Result ;
		int LoopMotionFlag ;
		int DisablePhysicsFlag ;
		int GravityNo ;
		VECTOR Gravity ;
		int GravityEnable ;
		float BaseGravity ;
		int NameLen ;
		const wchar_t *Name = NULL ;
		const wchar_t *CurrentDir = NULL ;
		int j, k ;

		k = 0 ;
		for( j = 0 ; j < 2 ; j ++ )
		{
			switch( j )
			{
			case 0 :
				if( LoadParam->GParam.AnimFilePathValid == FALSE )
					continue ;

				Name = LoadParam->GParam.AnimFileName ;
				CurrentDir = LoadParam->GParam.AnimFileDirPath ;
				break ;

			case 1 :
				Name = LoadParam->Name ;
				CurrentDir = LoadParam->CurrentDir ;
				break ;
			}

			NameLen = ( int )_WCSLEN( Name ) ;
			if( LoadParam->GParam.LoadModelToWorldGravityInitialize == FALSE )
			{
				BaseGravity = -9.8f * 12.5f ;
			}
			else
			{
				BaseGravity = LoadParam->GParam.LoadModelToWorldGravity ;
			}

			for( i = 0 ; ; i ++ )
			{
				// VMD�t�@�C���̓ǂݍ���
				if( LoadFile_VMD(
						&VmdData,
						&FileSize,
						i,
						LoadParam->FileReadFunc,
						Name,
						NameLen,
						CurrentDir,
						&LoopMotionFlag,
						&DisablePhysicsFlag,
						&BaseGravity,
						&GravityNo,
						&GravityEnable,
						&Gravity
					) == FALSE )
					break ;

				// �s��̍ăZ�b�g�A�b�v
				MV1LoadModelToPMD_SetupMatrix( BoneInfoDim, PmdBoneNum, TRUE, FALSE ) ;

#ifndef DX_NON_BULLET_PHYSICS
				// ���[�v���[�V�������ǂ����̏����Z�b�g����
				if( LoopMotionFlag )
				{
					MLPhysicsInfo.LoopMotionFlag = TRUE ;
					MLPhysicsInfo.LoopMotionNum = 3 ;	// �����݂��̒l��L���ɂ���ꍇ�͂R�Œ� 
				}
				else
				{
					MLPhysicsInfo.LoopMotionFlag = FALSE ;
					MLPhysicsInfo.LoopMotionNum = 1 ;
				}

				// �d�̓p�����[�^���Z�b�g
				if( GravityEnable )
				{
					MLPhysicsInfo.Gravity = Gravity ;
				}
				else
				if( GravityNo != -1 )
				{
					MLPhysicsInfo.Gravity = MV1Man.LoadCalcPhysicsWorldGravity[ GravityNo ] ;
				}
				else
				{
					MLPhysicsInfo.Gravity.x = 0.0f ;
					MLPhysicsInfo.Gravity.y = BaseGravity ;
					MLPhysicsInfo.Gravity.z = 0.0f ;
				}

				// �v�Z���x�̃p�����[�^���Z�b�g
				MLPhysicsInfo.PhysicsCalcPrecision = LoadParam->GParam.LoadModelToPhysicsCalcPrecision ;
#endif
				_SNPRINTF( String, sizeof( String ), "Anim%03d", k ) ;
				Result = _MV1LoadModelToVMD_PMD(
					k,
					&RModel,
					VmdData,
					FileSize,
					String,
					LoopMotionFlag,
					DisablePhysicsFlag,
					BoneInfoDim,
					PmdBoneNum,
					IKInfoFirst,
	#ifndef DX_NON_BULLET_PHYSICS
					ValidPhysics && LoadParam->GParam.LoadModelToUsePhysicsMode == DX_LOADMODEL_PHYSICS_LOADCALC ? &MLPhysicsInfo : NULL,
	#endif
					false
				) ;
				if( LoadParam->CurrentDir == NULL )
				{
					LoadParam->FileReadFunc->Release( VmdData, LoadParam->FileReadFunc->Data ) ;
				}
				else
				{
					DXFREE( VmdData ) ;
				}

				if( Result != 0 )
					break ;

				k ++ ;
			}
		}
	}

	// ���f����f�[�^�n���h���̍쐬
	NewHandle = MV1LoadModelToReadModel( &LoadParam->GParam, &RModel, LoadParam->CurrentDir, LoadParam->FileReadFunc, ASyncThread ) ;
	if( NewHandle < 0 ) goto ENDLABEL ;

	// �G���[�t���O��|��
	ErrorFlag = 0 ;

ENDLABEL :

	// �G���[�t���O�������Ă����烂�f���n���h�������
	if( ErrorFlag == 1 && NewHandle != -1 )
	{
		MV1SubModelBase( NewHandle ) ;
		NewHandle = -1 ;
	}

	// �������̉��
	if( SkinNextVertIndex != NULL )
	{
		DXFREE( SkinNextVertIndex ) ;
		SkinNextVertIndex = NULL ;
	}
	if( BoneInfoDim != NULL )
	{
		for( i = 0 ; i < PmdBoneNum ; i ++ )
		{
			if( BoneInfoDim[ i ].KeyMatrix != NULL )
			{
				DXFREE( BoneInfoDim[ i ].KeyMatrix ) ;
				BoneInfoDim[ i ].KeyMatrix = NULL ;
			}

			if( BoneInfoDim[ i ].KeyMatrix2 != NULL )
			{
				DXFREE( BoneInfoDim[ i ].KeyMatrix2 ) ;
				BoneInfoDim[ i ].KeyMatrix2 = NULL ;
			}
		}

		DXFREE( BoneInfoDim ) ;
		BoneInfoDim = NULL ;
	}
	if( IKInfoDim != NULL )
	{
		DXFREE( IKInfoDim ) ;
		IKInfoDim = NULL ;
	}

#ifndef DX_NON_BULLET_PHYSICS
	if( LoadParam->GParam.LoadModelToUsePhysicsMode != DX_LOADMODEL_PHYSICS_DISABLE )
	{
		if( MLPhysicsInfo.PmdPhysicsInfoDim != NULL )
		{
			DXFREE( MLPhysicsInfo.PmdPhysicsInfoDim ) ;
			MLPhysicsInfo.PmdPhysicsInfoDim = NULL ;
		}

		if( MLPhysicsInfo.PmdPhysicsJointInfoDim != NULL )
		{
			DXFREE( MLPhysicsInfo.PmdPhysicsJointInfoDim ) ;
			MLPhysicsInfo.PmdPhysicsJointInfoDim = NULL ;
		}
	}
#endif

	// �ǂݍ��݂悤���f�������
	MV1TermReadModel( &RModel ) ; 

	// �n���h����Ԃ�
	return NewHandle ;
}

// �s����v�Z����( ������� )
static void MV1LoadModelToPMD_SetupMatrix_One( PMD_READ_BONE_INFO *BoneInfo )
{
	MATRIX Matrix ;

	float	x2 = BoneInfo->Rotate.x * BoneInfo->Rotate.x * 2.0f ;
	float	y2 = BoneInfo->Rotate.y * BoneInfo->Rotate.y * 2.0f ;
	float	z2 = BoneInfo->Rotate.z * BoneInfo->Rotate.z * 2.0f ;
	float	xy = BoneInfo->Rotate.x * BoneInfo->Rotate.y * 2.0f ;
	float	yz = BoneInfo->Rotate.y * BoneInfo->Rotate.z * 2.0f ;
	float	zx = BoneInfo->Rotate.z * BoneInfo->Rotate.x * 2.0f ;
	float	xw = BoneInfo->Rotate.x * BoneInfo->Rotate.w * 2.0f ;
	float	yw = BoneInfo->Rotate.y * BoneInfo->Rotate.w * 2.0f ;
	float	zw = BoneInfo->Rotate.z * BoneInfo->Rotate.w * 2.0f ;

	Matrix.m[ 0 ][ 0 ] = 1.0f - y2 - z2 ;
	Matrix.m[ 0 ][ 1 ] = xy + zw ;
	Matrix.m[ 0 ][ 2 ] = zx - yw ;
	Matrix.m[ 0 ][ 3 ] = 0.0f ;
	Matrix.m[ 1 ][ 0 ] = xy - zw ;
	Matrix.m[ 1 ][ 1 ] = 1.0f - z2 - x2 ;
	Matrix.m[ 1 ][ 2 ] = yz + xw ;
	Matrix.m[ 1 ][ 3 ] = 0.0f ;
	Matrix.m[ 2 ][ 0 ] = zx + yw ;
	Matrix.m[ 2 ][ 1 ] = yz - xw ;
	Matrix.m[ 2 ][ 2 ] = 1.0f - x2 - y2 ;
	Matrix.m[ 2 ][ 3 ] = 0.0f ;
	Matrix.m[ 3 ][ 0 ] = BoneInfo->Translate.x ;
	Matrix.m[ 3 ][ 1 ] = BoneInfo->Translate.y ;
	Matrix.m[ 3 ][ 2 ] = BoneInfo->Translate.z ;
	Matrix.m[ 3 ][ 3 ] = 1.0f ;

	if( BoneInfo->Frame->Parent == NULL )
	{
		BoneInfo->LocalWorldMatrix = Matrix ;
	}
	else
	{
		MV1LoadModelToVMD_CreateMultiplyMatrix( &BoneInfo->LocalWorldMatrix, &Matrix, &( ( PMD_READ_BONE_INFO * )BoneInfo->Frame->Parent->UserData )->LocalWorldMatrix ) ;
	}
}

// �s����v�Z����
static void MV1LoadModelToPMD_SetupMatrix( PMD_READ_BONE_INFO *BoneInfo, int BoneNum, int UseInitParam, int IKSkip )
{
	int i ;
	int SetupBoneNum ;
	PMD_READ_BONE_INFO *BoneInfoTemp ;

	// ���ׂẴ{�[���̃��[�J���s����v�Z
	BoneInfoTemp = BoneInfo ;
	for( i = 0 ; i < BoneNum ; i ++, BoneInfo ++ )
	{
		float x2 ;
		float y2 ;
		float z2 ;
		float xy ;
		float yz ;
		float zx ;
		float xw ;
		float yw ;
		float zw ;

		BoneInfo->SetupLocalWorldMatrix = FALSE ;

		if( IKSkip && BoneInfo->IsIK ) continue ;

		if( UseInitParam )
		{
			x2 = BoneInfo->InitRotate.x * BoneInfo->InitRotate.x * 2.0f ;
			y2 = BoneInfo->InitRotate.y * BoneInfo->InitRotate.y * 2.0f ;
			z2 = BoneInfo->InitRotate.z * BoneInfo->InitRotate.z * 2.0f ;
			xy = BoneInfo->InitRotate.x * BoneInfo->InitRotate.y * 2.0f ;
			yz = BoneInfo->InitRotate.y * BoneInfo->InitRotate.z * 2.0f ;
			zx = BoneInfo->InitRotate.z * BoneInfo->InitRotate.x * 2.0f ;
			xw = BoneInfo->InitRotate.x * BoneInfo->InitRotate.w * 2.0f ;
			yw = BoneInfo->InitRotate.y * BoneInfo->InitRotate.w * 2.0f ;
			zw = BoneInfo->InitRotate.z * BoneInfo->InitRotate.w * 2.0f ;

			BoneInfo->IKQuat = BoneInfo->InitRotate ;

			BoneInfo->LocalMatrix.m[ 3 ][ 0 ] = BoneInfo->InitTranslate.x ;
			BoneInfo->LocalMatrix.m[ 3 ][ 1 ] = BoneInfo->InitTranslate.y ;
			BoneInfo->LocalMatrix.m[ 3 ][ 2 ] = BoneInfo->InitTranslate.z ;
			BoneInfo->LocalMatrix.m[ 3 ][ 3 ] = 1.0f ;
		}
		else
		{
			x2 = BoneInfo->Rotate.x * BoneInfo->Rotate.x * 2.0f ;
			y2 = BoneInfo->Rotate.y * BoneInfo->Rotate.y * 2.0f ;
			z2 = BoneInfo->Rotate.z * BoneInfo->Rotate.z * 2.0f ;
			xy = BoneInfo->Rotate.x * BoneInfo->Rotate.y * 2.0f ;
			yz = BoneInfo->Rotate.y * BoneInfo->Rotate.z * 2.0f ;
			zx = BoneInfo->Rotate.z * BoneInfo->Rotate.x * 2.0f ;
			xw = BoneInfo->Rotate.x * BoneInfo->Rotate.w * 2.0f ;
			yw = BoneInfo->Rotate.y * BoneInfo->Rotate.w * 2.0f ;
			zw = BoneInfo->Rotate.z * BoneInfo->Rotate.w * 2.0f ;

			BoneInfo->IKQuat = BoneInfo->Rotate ;

			BoneInfo->LocalMatrix.m[ 3 ][ 0 ] = BoneInfo->Translate.x ;
			BoneInfo->LocalMatrix.m[ 3 ][ 1 ] = BoneInfo->Translate.y ;
			BoneInfo->LocalMatrix.m[ 3 ][ 2 ] = BoneInfo->Translate.z ;
			BoneInfo->LocalMatrix.m[ 3 ][ 3 ] = 1.0f ;
		}

		BoneInfo->LocalMatrix.m[ 0 ][ 0 ] = 1.0f - y2 - z2 ;
		BoneInfo->LocalMatrix.m[ 0 ][ 1 ] = xy + zw ;
		BoneInfo->LocalMatrix.m[ 0 ][ 2 ] = zx - yw ;
		BoneInfo->LocalMatrix.m[ 0 ][ 3 ] = 0.0f ;
		BoneInfo->LocalMatrix.m[ 1 ][ 0 ] = xy - zw ;
		BoneInfo->LocalMatrix.m[ 1 ][ 1 ] = 1.0f - z2 - x2 ;
		BoneInfo->LocalMatrix.m[ 1 ][ 2 ] = yz + xw ;
		BoneInfo->LocalMatrix.m[ 1 ][ 3 ] = 0.0f ;
		BoneInfo->LocalMatrix.m[ 2 ][ 0 ] = zx + yw ;
		BoneInfo->LocalMatrix.m[ 2 ][ 1 ] = yz - xw ;
		BoneInfo->LocalMatrix.m[ 2 ][ 2 ] = 1.0f - x2 - y2 ;
		BoneInfo->LocalMatrix.m[ 2 ][ 3 ] = 0.0f ;
	}

	// ���ׂẴ{�[���̐e�q�֌W�̍s����v�Z
	SetupBoneNum = 0 ;
	while( SetupBoneNum < BoneNum )
	{
		BoneInfo = BoneInfoTemp ;
		for( i = 0 ; i < BoneNum ; i ++, BoneInfo ++ )
		{
			// ���łɃZ�b�g�A�b�v������Ă����牽�����Ȃ�
			if( BoneInfo->SetupLocalWorldMatrix )
			{
				continue ;
			}

			// IK �Ōv�Z����{�[���̏ꍇ�͉������Ȃ�
			if( IKSkip && BoneInfo->IsIK )
			{
				BoneInfo->SetupLocalWorldMatrix = TRUE ;
				SetupBoneNum ++ ;
				continue ;
			}

			if( BoneInfo->Frame->Parent == NULL )
			{
				BoneInfo->LocalWorldMatrix = BoneInfo->LocalMatrix ;
				BoneInfo->SetupLocalWorldMatrix = TRUE ;
				SetupBoneNum ++ ;
			}
			else
			{
				PMD_READ_BONE_INFO *ParentBoneInfo ;

				ParentBoneInfo = ( PMD_READ_BONE_INFO * )BoneInfo->Frame->Parent->UserData ;

				// �e�̃Z�b�g�A�b�v���������Ă���ꍇ�̂݃Z�b�g�A�b�v���s��
				if( ParentBoneInfo->SetupLocalWorldMatrix )
				{
					MV1LoadModelToVMD_CreateMultiplyMatrix( &BoneInfo->LocalWorldMatrix, &BoneInfo->LocalMatrix, &ParentBoneInfo->LocalWorldMatrix ) ;
					BoneInfo->SetupLocalWorldMatrix = TRUE ;
					SetupBoneNum ++ ;
				}
			}
		}
	}
}

// �h�j���v�Z����
static void MV1LoadModelToPMD_SetupIK( PMD_READ_BONE_INFO *BoneInfo, PMD_READ_IK_INFO *IKInfoFirst )
{
	int i, j, k ;
	PMD_READ_BONE_INFO *ChainBone ;
	PMD_READ_BONE_INFO *ChainParentBone ;
	VECTOR IKBonePos ;
	VECTOR BonePos ;
	VECTOR TargPos ;
	float Rot ;
	float Cos ;
	float IKsin, IKcos ;
	FLOAT4 qIK ;
	PMD_READ_IK_INFO *IKInfo ;

	for( IKInfo = IKInfoFirst ; IKInfo ; IKInfo = IKInfo->Next )
	{
		// IK�̃^�[�Q�b�g�̃��[���h���W���擾���Ă���
		IKBonePos.x = IKInfo->Bone->LocalWorldMatrix.m[ 3 ][ 0 ];
		IKBonePos.y = IKInfo->Bone->LocalWorldMatrix.m[ 3 ][ 1 ];
		IKBonePos.z = IKInfo->Bone->LocalWorldMatrix.m[ 3 ][ 2 ];

		QuatConvertToMatrix( IKInfo->TargetBone->IKmat, IKInfo->TargetBone->IKQuat, IKInfo->TargetBone->Translate );

		for( i = 0 ; i < IKInfo->Iterations ; i ++ )
		{
			for( j = 0 ; j < IKInfo->ChainBoneNum ; j ++ )
			{
				ChainBone = &BoneInfo[ IKInfo->ChainBone[ j ] ] ;
				ChainParentBone = ( PMD_READ_BONE_INFO * )ChainBone->Frame->Parent->UserData ;

				// �^�[�Q�b�g�{�[���Ɠ����{�[����������f�[�^�̃G���[�Ȃ̂Ŗ���
				if( ChainBone == IKInfo->TargetBone )
					continue ;

				// calculate IK bone position
				TargPos.x = IKInfo->TargetBone->LocalWorldMatrix.m[3][0];
				TargPos.y = IKInfo->TargetBone->LocalWorldMatrix.m[3][1];
				TargPos.z = IKInfo->TargetBone->LocalWorldMatrix.m[3][2];

				// calculate [k]th bone position
				BonePos.x = ChainBone->LocalWorldMatrix.m[3][0];
				BonePos.y = ChainBone->LocalWorldMatrix.m[3][1];
				BonePos.z = ChainBone->LocalWorldMatrix.m[3][2];

				// calculate [k]th bone quaternion
				VECTOR v1;
				v1.x = BonePos.x - TargPos.x;
				v1.y = BonePos.y - TargPos.y;
				v1.z = BonePos.z - TargPos.z;

				VECTOR v2;
				v2.x = BonePos.x - IKBonePos.x;
				v2.y = BonePos.y - IKBonePos.y;
				v2.z = BonePos.z - IKBonePos.z;

				v1 = VNorm( v1 );
				v2 = VNorm( v2 );
				if( ( v1.x - v2.x ) * ( v1.x - v2.x ) + ( v1.y - v2.y ) * ( v1.y - v2.y ) + ( v1.z - v2.z ) * ( v1.z - v2.z ) < 0.0000001f ) break;

				VECTOR v;
				v = VCross( v1, v2 );

				// calculate roll axis
				if( ChainBone->IKLimitAngle != 0 )
				{
					// if bone=knee roll only x-axis +
					v.x =  v.x * ChainParentBone->LocalWorldMatrix.m[ 0 ][ 0 ] + v.y * ChainParentBone->LocalWorldMatrix.m[ 0 ][ 1 ] + v.z * ChainParentBone->LocalWorldMatrix.m[ 0 ][ 2 ] >= 0.0f ? 1.0f : -1.0f ;
					v.y = 0.0f ;
					v.z = 0.0f ;
				}
				else
				{
					// calculate roll axis
					VECTOR vv;
					vv.x = v.x * ChainParentBone->LocalWorldMatrix.m[ 0 ][ 0 ] + v.y * ChainParentBone->LocalWorldMatrix.m[ 0 ][ 1 ] + v.z * ChainParentBone->LocalWorldMatrix.m[ 0 ][ 2 ] ;
					vv.y = v.x * ChainParentBone->LocalWorldMatrix.m[ 1 ][ 0 ] + v.y * ChainParentBone->LocalWorldMatrix.m[ 1 ][ 1 ] + v.z * ChainParentBone->LocalWorldMatrix.m[ 1 ][ 2 ] ;
					vv.z = v.x * ChainParentBone->LocalWorldMatrix.m[ 2 ][ 0 ] + v.y * ChainParentBone->LocalWorldMatrix.m[ 2 ][ 1 ] + v.z * ChainParentBone->LocalWorldMatrix.m[ 2 ][ 2 ] ;

					v = VNorm( vv );
				}

				// calculate roll angle of [k]th bone(limited by p_IK[i].dlimit*(k+1)*2)
				Cos = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z; 
				if( Cos >  1.0f ) Cos =  1.0f;
				if( Cos < -1.0f ) Cos = -1.0f;

				Rot = 0.5f * _ACOS( Cos );
				if( Rot > IKInfo->ControlWeight * ( j + 1 ) * 2 )
					Rot = IKInfo->ControlWeight * ( j + 1 ) * 2 ;

				_SINCOS_PLATFORM( Rot, &IKsin, &IKcos );
				qIK.x = v.x * IKsin;
				qIK.y = v.y * IKsin;
				qIK.z = v.z * IKsin;
				qIK.w = IKcos;

				ChainBone->IKQuat = QuatMul( qIK, ChainBone->IKQuat );

				QuatConvertToMatrix( ChainBone->IKmat, ChainBone->IKQuat, ChainBone->Translate );
				if( ChainBone->IKLimitAngle != 0 )
				{
					if( _ATAN2( ChainBone->IKmat.m[2][1], ChainBone->IKmat.m[2][2] ) < 0.0f )
					{
						ChainBone->IKmat.m[1][2] = -ChainBone->IKmat.m[1][2];
						ChainBone->IKmat.m[2][1] = -ChainBone->IKmat.m[2][1];
						ChainBone->IKQuat.x = -ChainBone->IKQuat.x;
					}
				}

				for( k = j ; k >= 0 ; k-- )
				{
					ChainBone= &BoneInfo[ IKInfo->ChainBone[ k ] ];
					MV1LoadModelToVMD_CreateMultiplyMatrix( &ChainBone->LocalWorldMatrix, &ChainBone->IKmat, &( ( PMD_READ_BONE_INFO * )ChainBone->Frame->Parent->UserData )->LocalWorldMatrix ) ;
				}

				MV1LoadModelToVMD_CreateMultiplyMatrix( &IKInfo->TargetBone->LocalWorldMatrix, &IKInfo->TargetBone->IKmat, &( ( PMD_READ_BONE_INFO * )IKInfo->TargetBone->Frame->Parent->UserData )->LocalWorldMatrix );
			}

			for( j = 0 ; j < IKInfo->ChainBoneNum ; j ++ )
			{
				ChainBone = &BoneInfo[ IKInfo->ChainBone[ j ] ] ;

				ChainBone->Rotate = ChainBone->IKQuat ;
			}
			IKInfo->TargetBone->Rotate = IKInfo->TargetBone->IKQuat ;
		}
	}
}

// �w��̃{�[���ɃA�j���[�V�����̎w��L�[�̃p�����[�^�𔽉f������
static void MV1LoadModelToPMD_SetupOneBoneMatrixFormAnimKey( PMD_READ_BONE_INFO *BoneInfo, int Time, int LoopNo, int MaxTime, int ValidNextRate, float NextRate )
{
	VMD_READ_KEY_INFO *NowKey, *NextKey ;

	if( BoneInfo->IsIK )
	{
		if( BoneInfo->IsIKAnim == FALSE || LoopNo != 0 )
		{
			if( ValidNextRate )
			{
				VECTOR *NowKeyPos ;
				FLOAT4 *NowKeyRot ;
				VECTOR *NextKeyPos ;
				FLOAT4 *NextKeyRot ;

				NowKeyPos = &BoneInfo->KeyPos[ Time ] ;
				NowKeyRot = &BoneInfo->KeyRot[ Time ] ;
				if( Time == MaxTime )
				{
					NextKeyPos = &BoneInfo->KeyPos[ 0 ] ;
					NextKeyRot = &BoneInfo->KeyRot[ 0 ] ;
				}
				else
				{
					NextKeyPos = &BoneInfo->KeyPos[ Time + 1 ] ;
					NextKeyRot = &BoneInfo->KeyRot[ Time + 1 ] ;
				}

				BoneInfo->Translate.x = ( NextKeyPos->x - NowKeyPos->x ) * NextRate + NowKeyPos->x ;
				BoneInfo->Translate.y = ( NextKeyPos->y - NowKeyPos->y ) * NextRate + NowKeyPos->y ;
				BoneInfo->Translate.z = ( NextKeyPos->z - NowKeyPos->z ) * NextRate + NowKeyPos->z ;

				// ���ʐ��`���
				{
					float qr ;
					float t0 ;

					qr =	NowKeyRot->x * NextKeyRot->x +
							NowKeyRot->y * NextKeyRot->y +
							NowKeyRot->z * NextKeyRot->z +
							NowKeyRot->w * NextKeyRot->w ;
					t0 = 1.0f - NextRate ;

					if( qr < 0 )
					{
						BoneInfo->Rotate.x = NowKeyRot->x * t0 - NextKeyRot->x * NextRate ;
						BoneInfo->Rotate.y = NowKeyRot->y * t0 - NextKeyRot->y * NextRate ;
						BoneInfo->Rotate.z = NowKeyRot->z * t0 - NextKeyRot->z * NextRate ;
						BoneInfo->Rotate.w = NowKeyRot->w * t0 - NextKeyRot->w * NextRate ;
					}
					else
					{
						BoneInfo->Rotate.x = NowKeyRot->x * t0 + NextKeyRot->x * NextRate ;
						BoneInfo->Rotate.y = NowKeyRot->y * t0 + NextKeyRot->y * NextRate ;
						BoneInfo->Rotate.z = NowKeyRot->z * t0 + NextKeyRot->z * NextRate ;
						BoneInfo->Rotate.w = NowKeyRot->w * t0 + NextKeyRot->w * NextRate ;
					}
					QuaternionNormalize( &BoneInfo->Rotate, &BoneInfo->Rotate ) ;
				}
//				BoneInfo->Rotate.x    = ( NextKeyRot->x - NowKeyRot->x ) * NextRate + NowKeyRot->x ;
//				BoneInfo->Rotate.y    = ( NextKeyRot->y - NowKeyRot->y ) * NextRate + NowKeyRot->y ;
//				BoneInfo->Rotate.z    = ( NextKeyRot->z - NowKeyRot->z ) * NextRate + NowKeyRot->z ;
//				BoneInfo->Rotate.w    = ( NextKeyRot->w - NowKeyRot->w ) * NextRate + NowKeyRot->w ;
//				QuaternionNormalize( &BoneInfo->Rotate, &BoneInfo->Rotate ) ;
			}
			else
			{
				BoneInfo->Translate = BoneInfo->KeyPos[ Time ] ;
				BoneInfo->Rotate    = BoneInfo->KeyRot[ Time ] ;
			}
		}
		else
		{
			BoneInfo->Translate = BoneInfo->Frame->Translate ;
			BoneInfo->Rotate.x  = 0.0f ;
			BoneInfo->Rotate.y  = 0.0f ;
			BoneInfo->Rotate.z  = 0.0f ;
			BoneInfo->Rotate.w  = 1.0f ;
		}
	}
	else
	if( BoneInfo->IsIK == 0 && BoneInfo->Anim != NULL )
	{
		// �L�[�̏���
		if( BoneInfo->NowKey->Next == NULL )
		{
			NowKey = BoneInfo->NowKey ;
			NextKey = NULL ;
		}
		else
		if( BoneInfo->NowKey->Next->Frame * 2 <= ( DWORD )Time )
		{
			BoneInfo->NowKey = BoneInfo->NowKey->Next ;
			NowKey = BoneInfo->NowKey ;
			NextKey = NowKey->Next ;
		}
		else
		{
			NowKey = BoneInfo->NowKey ;
			NextKey = NowKey->Next ;
		}

		// ���̃L�[���Ȃ������݂̃L�[�̃t���[���ȉ��̏ꍇ�͌��݂̃t���[���̒l���̂܂�
		if( NextKey == NULL || NowKey->Frame * 2 >= ( DWORD )Time )
		{
			BoneInfo->Translate = NowKey->MVRPosKey[ 0 ] ;
			BoneInfo->Rotate    = NowKey->MVRRotKey[ 0 ] ;
		}
		else
		{
			int KeyNo ;

			KeyNo = ( int )( Time - NowKey->Frame * 2 ) ;

			if( ValidNextRate )
			{
				VECTOR *NowKeyPos ;
				FLOAT4 *NowKeyRot ;
				VECTOR *NextKeyPos ;
				FLOAT4 *NextKeyRot ;

				NowKeyPos = &NowKey->MVRPosKey[ KeyNo ] ;
				NowKeyRot = &NowKey->MVRRotKey[ KeyNo ] ;
				if( Time == MaxTime )
				{
					NextKeyPos = &NowKey->MVRPosKey[ 0 ] ;
					NextKeyRot = &NowKey->MVRRotKey[ 0 ] ;
				}
				else
				{
					NextKeyPos = &NowKey->MVRPosKey[ KeyNo + 1 ] ;
					NextKeyRot = &NowKey->MVRRotKey[ KeyNo + 1 ] ;
				}

				BoneInfo->Translate.x = ( NextKeyPos->x - NowKeyPos->x ) * NextRate + NowKeyPos->x ;
				BoneInfo->Translate.y = ( NextKeyPos->y - NowKeyPos->y ) * NextRate + NowKeyPos->y ;
				BoneInfo->Translate.z = ( NextKeyPos->z - NowKeyPos->z ) * NextRate + NowKeyPos->z ;

				// ���ʐ��`���
				{
					float qr ;
					float t0 ;

					qr =	NowKeyRot->x * NextKeyRot->x +
							NowKeyRot->y * NextKeyRot->y +
							NowKeyRot->z * NextKeyRot->z +
							NowKeyRot->w * NextKeyRot->w ;
					t0 = 1.0f - NextRate ;

					if( qr < 0 )
					{
						BoneInfo->Rotate.x = NowKeyRot->x * t0 - NextKeyRot->x * NextRate ;
						BoneInfo->Rotate.y = NowKeyRot->y * t0 - NextKeyRot->y * NextRate ;
						BoneInfo->Rotate.z = NowKeyRot->z * t0 - NextKeyRot->z * NextRate ;
						BoneInfo->Rotate.w = NowKeyRot->w * t0 - NextKeyRot->w * NextRate ;
					}
					else
					{
						BoneInfo->Rotate.x = NowKeyRot->x * t0 + NextKeyRot->x * NextRate ;
						BoneInfo->Rotate.y = NowKeyRot->y * t0 + NextKeyRot->y * NextRate ;
						BoneInfo->Rotate.z = NowKeyRot->z * t0 + NextKeyRot->z * NextRate ;
						BoneInfo->Rotate.w = NowKeyRot->w * t0 + NextKeyRot->w * NextRate ;
					}
					QuaternionNormalize( &BoneInfo->Rotate, &BoneInfo->Rotate ) ;
				}
//				BoneInfo->Rotate.x    = ( NextKeyRot->x - NowKeyRot->x ) * NextRate + NowKeyRot->x ;
//				BoneInfo->Rotate.y    = ( NextKeyRot->y - NowKeyRot->y ) * NextRate + NowKeyRot->y ;
//				BoneInfo->Rotate.z    = ( NextKeyRot->z - NowKeyRot->z ) * NextRate + NowKeyRot->z ;
//				BoneInfo->Rotate.w    = ( NextKeyRot->w - NowKeyRot->w ) * NextRate + NowKeyRot->w ;
//				QuaternionNormalize( &BoneInfo->Rotate, &BoneInfo->Rotate ) ;
			}
			else
			{
				// ���炩���ߎZ�o������Ԓl���Z�b�g
				BoneInfo->Translate = NowKey->MVRPosKey[ KeyNo ] ;
				BoneInfo->Rotate    = NowKey->MVRRotKey[ KeyNo ] ;
			}
		}
	}
}

// �u�l�c�t�@�C����ǂݍ���( -1:�G���[ )
static int _MV1LoadModelToVMD_PMD(
	int								DataIndex,
	MV1_MODEL_R *					RModel,
	void *							DataBuffer,
	int								DataSize,
	const char *					Name,
	int								LoopMotionFlag,
	int								DisablePhysicsFlag,
	PMD_READ_BONE_INFO *			PmdBoneInfo,
	int								PmdBoneNum,
	PMD_READ_IK_INFO *				PmdIKInfoFirst,
#ifndef DX_NON_BULLET_PHYSICS
	DX_MODELLOADER3_PMD_PHYSICS_INFO *	MLPhysicsInfo,
#endif
	bool							FPS60
)
{
	int i, j, k ;
	VMD_READ_NODE_INFO *VmdNode ;
	VMD_READ_KEY_INFO *VmdKey, *VmdKeyTemp1, *VmdKeyTemp2 ;
	VMD_READ_FACE_KEY_SET_INFO *VmdFaceKeySet ;
	VMD_READ_FACE_KEY_INFO *VmdFaceKey ;
	PMD_READ_BONE_INFO *BoneInfo ;
	MV1_SHAPE_R *Shape ;
	MV1_FRAME_R *Frame ;
	MV1_ANIM_R *Anim ;
	MV1_ANIMSET_R *AnimSet ;
	MV1_ANIMKEYSET_R *KeyPosSet, *KeyRotSet, *KeyFactorSet ;
	float *KeyPosTime, *KeyRotTime, *KeyFactorTime ;
	float *KeyFactor ;
	VECTOR *KeyPos ;
	FLOAT4 *KeyRot ;
	VMD_READ_INFO VmdData ;
	wchar_t String[ 256 ] ;

#ifndef DX_NON_BULLET_PHYSICS
	// DisablePhysicsFlag �� TRUE �ŁA���� MV1Man.LoadModelToDisablePhysicsNameWordMode ��
	// DX_LOADMODEL_PHYSICS_DISABLENAMEWORD_ALWAYS �������畨�����������S�ɖ����ɂ���
	if( DisablePhysicsFlag &&
		MV1Man.LoadModelToDisablePhysicsNameWordMode == DX_LOADMODEL_PHYSICS_DISABLENAMEWORD_ALWAYS )
	{
		MLPhysicsInfo = NULL ;
	}
#endif

	// ��{���̓ǂݍ���
	if( LoadVMDBaseData( &VmdData, DataBuffer, DataSize ) < 0 )
		return -1 ;

#ifndef DX_NON_BULLET_PHYSICS
	if( MLPhysicsInfo )
	{
		// �����I�u�W�F�N�g�̏���������
		SetupPhysicsObject_PMDPhysicsInfo( MLPhysicsInfo, DisablePhysicsFlag ) ;
	}
#endif

	// �A�j���[�V�����Z�b�g��ǉ�
	AnimSet = MV1RAddAnimSet( RModel, Name ) ;
	if( AnimSet == NULL )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x44\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xa2\x30\xcb\x30\xe1\x30\xfc\x30\xb7\x30\xe7\x30\xf3\x30\xbb\x30\xc3\x30\xc8\x30\x6e\x30\xfd\x8f\xa0\x52\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMD Load Error : �A�j���[�V�����Z�b�g�̒ǉ��Ɏ��s���܂���\n" @*/ )) ;
		goto ENDLABEL ;
	}

	// ���[�v���[�V�������ǂ������Z�b�g
	AnimSet->IsLoopAnim = LoopMotionFlag ? 1 : 0 ;

	// �{�[���̃A�j���[�V�����|�C���^�̏�����
	BoneInfo = PmdBoneInfo ;
	for( i = 0 ; i < PmdBoneNum ; i ++, BoneInfo ++ )
	{
		BoneInfo->Anim = NULL ;
		BoneInfo->Node = NULL ;
	}

	AnimSet->StartTime = 0.0f ;
	AnimSet->EndTime = ( float )VmdData.MaxTime ;

	// �m�[�h�̐������J��Ԃ�
	VmdNode = VmdData.Node ;
	for( i = 0 ; ( DWORD )i < VmdData.NodeNum ; i ++, VmdNode ++ )
	{
		// �t���[���̌���
		wchar_t VmdNodeNameW[ 64 ] ;
		ConvString( VmdNode->Name, DX_CHARCODEFORMAT_SHIFTJIS, ( char * )VmdNodeNameW, sizeof( VmdNodeNameW ), WCHAR_T_CHARCODEFORMAT ) ;
		for( Frame = RModel->FrameFirst ; Frame && _WCSCMP( Frame->NameW, VmdNodeNameW ) != 0 ; Frame = Frame->DataNext ){}
		if( Frame == NULL ) continue ;

		BoneInfo = ( PMD_READ_BONE_INFO * )Frame->UserData ;

		// �A�j���[�V�����̒ǉ�
		Anim = MV1RAddAnim( RModel, AnimSet ) ;
		if( Anim == NULL )
		{
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x44\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xa2\x30\xcb\x30\xe1\x30\xfc\x30\xb7\x30\xe7\x30\xf3\x30\x6e\x30\xfd\x8f\xa0\x52\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMD Load Error : �A�j���[�V�����̒ǉ��Ɏ��s���܂���\n" @*/ )) ;
			goto ENDLABEL ;
		}

		// �{�[���ɏ��Z�b�g
		BoneInfo->Anim = Anim ;
		BoneInfo->Node = VmdNode ;

		// �Ώۃm�[�h�̃Z�b�g
		Anim->TargetFrameIndex = Frame->Index ;

		// �ő厞�Ԃ��Z�b�g
		if( BoneInfo->IsIK )
		{
			Anim->MaxTime = ( float )VmdData.MaxTime ;
		}
		else
		{
			Anim->MaxTime = ( float )VmdNode->MaxFrame ;
		}

		// �ő厞�Ԃƍŏ����Ԃ��X�V
//		if( AnimSet->StartTime > ( float )VmdNode->MinFrame ) AnimSet->StartTime = ( float )VmdNode->MinFrame ;
//		if( AnimSet->EndTime   < ( float )VmdNode->MaxFrame ) AnimSet->EndTime   = ( float )VmdNode->MaxFrame ;

		// �A�j���[�V�����L�[�̏����Z�b�g
		KeyPosSet = MV1RAddAnimKeySet( RModel, Anim ) ;
		if( KeyPosSet == NULL )
		{
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x44\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xa2\x30\xcb\x30\xe1\x30\xfc\x30\xb7\x30\xe7\x30\xf3\x30\xa7\x5e\x19\x6a\xad\x30\xfc\x30\xbb\x30\xc3\x30\xc8\x30\x6e\x30\xfd\x8f\xa0\x52\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMD Load Error : �A�j���[�V�������W�L�[�Z�b�g�̒ǉ��Ɏ��s���܂���\n" @*/ )) ;
			goto ENDLABEL ;
		}
		KeyRotSet = MV1RAddAnimKeySet( RModel, Anim ) ;
		if( KeyRotSet == NULL )
		{
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x44\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xa2\x30\xcb\x30\xe1\x30\xfc\x30\xb7\x30\xe7\x30\xf3\x30\xde\x56\xe2\x8e\xad\x30\xfc\x30\xbb\x30\xc3\x30\xc8\x30\x6e\x30\xfd\x8f\xa0\x52\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMD Load Error : �A�j���[�V������]�L�[�Z�b�g�̒ǉ��Ɏ��s���܂���\n" @*/ )) ;
			goto ENDLABEL ;
		}

		KeyPosSet->Type = MV1_ANIMKEY_TYPE_VECTOR ;
		KeyPosSet->DataType = MV1_ANIMKEY_DATATYPE_TRANSLATE ;
		KeyPosSet->TimeType = MV1_ANIMKEY_TIME_TYPE_KEY ;
		KeyPosSet->TotalTime = ( float )VmdNode->MaxFrame ;
		KeyPosSet->Num = ( int )( BoneInfo->IsIK ? VmdData.MaxTime : VmdNode->MaxFrame - VmdNode->MinFrame ) ;
		KeyPosSet->Num *= 2 ;
		KeyPosSet->Num += 1 ;

		KeyRotSet->Type = MV1_ANIMKEY_TYPE_QUATERNION_VMD ;
		KeyRotSet->DataType = MV1_ANIMKEY_DATATYPE_ROTATE ;
		KeyRotSet->TimeType = MV1_ANIMKEY_TIME_TYPE_KEY ;
		KeyRotSet->TotalTime = ( float )VmdNode->MaxFrame ;
		KeyRotSet->Num = KeyPosSet->Num ;

		KeyPosSet->KeyTime = ( float * )ADDMEMAREA( sizeof( float ) * KeyPosSet->Num, &RModel->Mem ) ;
		if( KeyPosSet->KeyTime == NULL )
		{
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x44\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xa2\x30\xcb\x30\xe1\x30\xfc\x30\xb7\x30\xe7\x30\xf3\x30\xad\x30\xfc\x30\xbf\x30\xa4\x30\xe0\x30\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMD Load Error : �A�j���[�V�����L�[�^�C�����i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
			goto ENDLABEL ;
		}
		KeyPosSet->KeyVector = ( VECTOR * )ADDMEMAREA( sizeof( VECTOR ) * KeyPosSet->Num, &RModel->Mem ) ;
		if( KeyPosSet->KeyVector == NULL )
		{
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x44\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xa2\x30\xcb\x30\xe1\x30\xfc\x30\xb7\x30\xe7\x30\xf3\x30\xad\x30\xfc\x30\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMD Load Error : �A�j���[�V�����L�[���i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
			goto ENDLABEL ;
		}

		KeyRotSet->KeyTime = ( float * )ADDMEMAREA( sizeof( float ) * KeyRotSet->Num, &RModel->Mem ) ;
		if( KeyRotSet->KeyTime == NULL )
		{
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x44\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xa2\x30\xcb\x30\xe1\x30\xfc\x30\xb7\x30\xe7\x30\xf3\x30\xad\x30\xfc\x30\xbf\x30\xa4\x30\xe0\x30\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMD Load Error : �A�j���[�V�����L�[�^�C�����i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
			goto ENDLABEL ;
		}
		KeyRotSet->KeyVector = ( VECTOR * )ADDMEMAREA( sizeof( FLOAT4 ) * KeyRotSet->Num, &RModel->Mem ) ;
		if( KeyRotSet->KeyVector == NULL )
		{
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x44\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xa2\x30\xcb\x30\xe1\x30\xfc\x30\xb7\x30\xe7\x30\xf3\x30\xad\x30\xfc\x30\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMD Load Error : �A�j���[�V�����L�[���i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
			goto ENDLABEL ;
		}

		RModel->AnimKeyDataSize += KeyRotSet->Num * ( sizeof( float ) * 2 + sizeof( VECTOR ) + sizeof( FLOAT4 ) ) ;

		VmdKey = VmdNode->FirstKey ;
		BoneInfo->KeyPos     = KeyPos     = KeyPosSet->KeyVector ;
		BoneInfo->KeyRot     = KeyRot     = KeyRotSet->KeyFloat4 ;
		BoneInfo->KeyPosTime = KeyPosTime = KeyPosSet->KeyTime ;
		BoneInfo->KeyRotTime = KeyRotTime = KeyRotSet->KeyTime ;
		if( VmdNode->KeyNum == 1 )
		{
			if( BoneInfo->IsIK )
			{
				for( j = 0 ; j < KeyRotSet->Num ; j ++ )
				{
					*KeyPosTime = ( float )j / 2.0f ;
					*KeyRotTime = ( float )j / 2.0f ;
					KeyPos->x = VmdKey->Position[ 0 ] + Frame->Translate.x ;
					KeyPos->y = VmdKey->Position[ 1 ] + Frame->Translate.y ;
					KeyPos->z = VmdKey->Position[ 2 ] + Frame->Translate.z ;
					KeyRot->x = VmdKey->Quaternion[ 0 ] ;
					KeyRot->y = VmdKey->Quaternion[ 1 ] ;
					KeyRot->z = VmdKey->Quaternion[ 2 ] ;
					KeyRot->w = VmdKey->Quaternion[ 3 ] ;
					QuaternionNormalize( KeyRot, KeyRot ) ;
					KeyPos ++ ;
					KeyRot ++ ;
					KeyPosTime ++ ;
					KeyRotTime ++ ;
				}
			}
			else
			{
				VmdKey->MVRPosKey = KeyPos ;
				VmdKey->MVRRotKey = KeyRot ;
				*KeyPosTime = ( float )VmdKey->Frame ;
				*KeyRotTime = ( float )VmdKey->Frame ;
				KeyPos->x = VmdKey->Position[ 0 ] + Frame->Translate.x ;
				KeyPos->y = VmdKey->Position[ 1 ] + Frame->Translate.y ;
				KeyPos->z = VmdKey->Position[ 2 ] + Frame->Translate.z ;
				KeyRot->x = VmdKey->Quaternion[ 0 ] ;
				KeyRot->y = VmdKey->Quaternion[ 1 ] ;
				KeyRot->z = VmdKey->Quaternion[ 2 ] ;
				KeyRot->w = VmdKey->Quaternion[ 3 ] ;
				QuaternionNormalize( KeyRot, KeyRot ) ;
			}
		}
		else
		{
			if( BoneInfo->IsIK && VmdKey->Frame > 0 )
			{
				for( j = 0 ; ( DWORD )j < VmdKey->Frame * 2 ; j ++ )
				{
					*KeyPosTime = ( float )VmdKey->Frame ;
					*KeyRotTime = ( float )VmdKey->Frame ;
					KeyPos->x = VmdKey->Position[ 0 ] + Frame->Translate.x ;
					KeyPos->y = VmdKey->Position[ 1 ] + Frame->Translate.y ;
					KeyPos->z = VmdKey->Position[ 2 ] + Frame->Translate.z ;
					KeyRot->x = VmdKey->Quaternion[ 0 ] ;
					KeyRot->y = VmdKey->Quaternion[ 1 ] ;
					KeyRot->z = VmdKey->Quaternion[ 2 ] ;
					KeyRot->w = VmdKey->Quaternion[ 3 ] ;
					QuaternionNormalize( KeyRot, KeyRot ) ;
					KeyPos ++ ;
					KeyRot ++ ;
					KeyPosTime ++ ;
					KeyRotTime ++ ;
				}
			}

			for( j = 0 ; ( DWORD )j < VmdNode->KeyNum ; j ++, VmdKey = VmdKey->Next )
			{
				int XLinear, YLinear, ZLinear, RLinear ;
				float XX1, XY1, XX2, XY2 ;
				float YX1, YY1, YX2, YY2 ;
				float ZX1, ZY1, ZX2, ZY2 ;
				float RX1, RY1, RX2, RY2 ;

				if( j == 0 ) continue ;

				VmdKey->Prev->MVRPosKey = KeyPos ;
				VmdKey->Prev->MVRRotKey = KeyRot ;

				VmdKeyTemp1 = VmdKey->Prev ;
				VmdKeyTemp2 = VmdKey ;

				XLinear = VmdKeyTemp2->Linear[ 0 ] ;
				YLinear = VmdKeyTemp2->Linear[ 1 ] ;
				ZLinear = VmdKeyTemp2->Linear[ 2 ] ;
				RLinear = VmdKeyTemp2->Linear[ 3 ] ;

				XX1 = VmdKeyTemp2->PosXBezier[ 0 ] ;
				XY1 = VmdKeyTemp2->PosXBezier[ 1 ] ;
				XX2 = VmdKeyTemp2->PosXBezier[ 2 ] ;
				XY2 = VmdKeyTemp2->PosXBezier[ 3 ] ;
				YX1 = VmdKeyTemp2->PosYBezier[ 0 ] ;
				YY1 = VmdKeyTemp2->PosYBezier[ 1 ] ;
				YX2 = VmdKeyTemp2->PosYBezier[ 2 ] ;
				YY2 = VmdKeyTemp2->PosYBezier[ 3 ] ;
				ZX1 = VmdKeyTemp2->PosZBezier[ 0 ] ;
				ZY1 = VmdKeyTemp2->PosZBezier[ 1 ] ;
				ZX2 = VmdKeyTemp2->PosZBezier[ 2 ] ;
				ZY2 = VmdKeyTemp2->PosZBezier[ 3 ] ;
				RX1 = VmdKeyTemp2->RotBezier[ 0 ] ;
				RY1 = VmdKeyTemp2->RotBezier[ 1 ] ;
				RX2 = VmdKeyTemp2->RotBezier[ 2 ] ;
				RY2 = VmdKeyTemp2->RotBezier[ 3 ] ;

				for( k = ( int )( VmdKeyTemp1->Frame * 2 ) ; ( DWORD )k < VmdKeyTemp2->Frame * 2 ; k ++ )
				{
					float Rate, RateH ;
					float fFrame ;

					fFrame = k / 2.0f ;
					Rate = ( fFrame - VmdKeyTemp1->Frame ) / ( float )( VmdKeyTemp2->Frame - VmdKeyTemp1->Frame ) ;
					*KeyPosTime = fFrame ;
					*KeyRotTime = fFrame ;
/*
					KeyPos->x = VmdKeyTemp1->Position[ 0 ] + ( VmdKeyTemp2->Position[ 0 ] - VmdKeyTemp1->Position[ 0 ] ) * Rate + Frame->Translate.x ;
					KeyPos->y = VmdKeyTemp1->Position[ 1 ] + ( VmdKeyTemp2->Position[ 1 ] - VmdKeyTemp1->Position[ 1 ] ) * Rate + Frame->Translate.y ;
					KeyPos->z = VmdKeyTemp1->Position[ 2 ] + ( VmdKeyTemp2->Position[ 2 ] - VmdKeyTemp1->Position[ 2 ] ) * Rate + Frame->Translate.z ;
					KeyRot->x = VmdKeyTemp1->Quaternion[ 0 ] + ( VmdKeyTemp2->Quaternion[ 0 ] - VmdKeyTemp1->Quaternion[ 0 ] ) * Rate ;
					KeyRot->y = VmdKeyTemp1->Quaternion[ 1 ] + ( VmdKeyTemp2->Quaternion[ 1 ] - VmdKeyTemp1->Quaternion[ 1 ] ) * Rate ;
					KeyRot->z = VmdKeyTemp1->Quaternion[ 2 ] + ( VmdKeyTemp2->Quaternion[ 2 ] - VmdKeyTemp1->Quaternion[ 2 ] ) * Rate ;
					KeyRot->w = VmdKeyTemp1->Quaternion[ 3 ] + ( VmdKeyTemp2->Quaternion[ 3 ] - VmdKeyTemp1->Quaternion[ 3 ] ) * Rate ;
*/
					VmdCalcLine( XLinear, Rate, RateH, XX1, XX2, XY1, XY2 ) ;
					KeyPos->x = VmdKeyTemp1->Position[ 0 ] + ( VmdKeyTemp2->Position[ 0 ] - VmdKeyTemp1->Position[ 0 ] ) * RateH + Frame->Translate.x ;

					VmdCalcLine( YLinear, Rate, RateH, YX1, YX2, YY1, YY2 ) ;
					KeyPos->y = VmdKeyTemp1->Position[ 1 ] + ( VmdKeyTemp2->Position[ 1 ] - VmdKeyTemp1->Position[ 1 ] ) * RateH + Frame->Translate.y ;

					VmdCalcLine( ZLinear, Rate, RateH, ZX1, ZX2, ZY1, ZY2 ) ;
					KeyPos->z = VmdKeyTemp1->Position[ 2 ] + ( VmdKeyTemp2->Position[ 2 ] - VmdKeyTemp1->Position[ 2 ] ) * RateH + Frame->Translate.z ;

					// ���ʐ��`���
					VmdCalcLine( RLinear, Rate, RateH, RX1, RX2, RY1, RY2 ) ;
					{
						float qr ;
						float t0 ;

						qr = VmdKeyTemp1->Quaternion[ 0 ] * VmdKeyTemp2->Quaternion[ 0 ] +
							 VmdKeyTemp1->Quaternion[ 1 ] * VmdKeyTemp2->Quaternion[ 1 ] +
							 VmdKeyTemp1->Quaternion[ 2 ] * VmdKeyTemp2->Quaternion[ 2 ] +
							 VmdKeyTemp1->Quaternion[ 3 ] * VmdKeyTemp2->Quaternion[ 3 ] ;
						t0 = 1.0f - RateH ;

						if( qr < 0 )
						{
							KeyRot->x = VmdKeyTemp1->Quaternion[ 0 ] * t0 - VmdKeyTemp2->Quaternion[ 0 ] * RateH ;
							KeyRot->y = VmdKeyTemp1->Quaternion[ 1 ] * t0 - VmdKeyTemp2->Quaternion[ 1 ] * RateH ;
							KeyRot->z = VmdKeyTemp1->Quaternion[ 2 ] * t0 - VmdKeyTemp2->Quaternion[ 2 ] * RateH ;
							KeyRot->w = VmdKeyTemp1->Quaternion[ 3 ] * t0 - VmdKeyTemp2->Quaternion[ 3 ] * RateH ;
						}
						else
						{
							KeyRot->x = VmdKeyTemp1->Quaternion[ 0 ] * t0 + VmdKeyTemp2->Quaternion[ 0 ] * RateH ;
							KeyRot->y = VmdKeyTemp1->Quaternion[ 1 ] * t0 + VmdKeyTemp2->Quaternion[ 1 ] * RateH ;
							KeyRot->z = VmdKeyTemp1->Quaternion[ 2 ] * t0 + VmdKeyTemp2->Quaternion[ 2 ] * RateH ;
							KeyRot->w = VmdKeyTemp1->Quaternion[ 3 ] * t0 + VmdKeyTemp2->Quaternion[ 3 ] * RateH ;
						}
						QuaternionNormalize( KeyRot, KeyRot ) ;
					}

					KeyPos ++ ;
					KeyRot ++ ;
					KeyPosTime ++ ;
					KeyRotTime ++ ;
				}
				if( ( DWORD )j == VmdNode->KeyNum - 1 )
				{
					VmdKey->MVRPosKey = KeyPos ;
					VmdKey->MVRRotKey = KeyRot ;
					*KeyPosTime = ( float )VmdKey->Frame ;
					*KeyRotTime = ( float )VmdKey->Frame ;
					KeyPos->x = VmdKey->Position[ 0 ] + Frame->Translate.x ;
					KeyPos->y = VmdKey->Position[ 1 ] + Frame->Translate.y ;
					KeyPos->z = VmdKey->Position[ 2 ] + Frame->Translate.z ;
					KeyRot->x = VmdKey->Quaternion[ 0 ] ;
					KeyRot->y = VmdKey->Quaternion[ 1 ] ;
					KeyRot->z = VmdKey->Quaternion[ 2 ] ;
					KeyRot->w = VmdKey->Quaternion[ 3 ] ;
					QuaternionNormalize( KeyRot, KeyRot ) ;
					KeyPos ++ ;
					KeyRot ++ ;
					KeyPosTime ++ ;
					KeyRotTime ++ ;
				}
			}

			if( BoneInfo->IsIK && KeyRot - BoneInfo->KeyRot < KeyRotSet->Num )
			{
				for( j = ( int )( KeyRot - BoneInfo->KeyRot ) ; j < KeyRotSet->Num ; j ++ )
				{
					KeyPosTime[ 0 ] = ( float )j / 2.0f ;
					KeyRotTime[ 0 ] = ( float )j / 2.0f ;
					KeyPos[ 0 ] = KeyPos[ -1 ] ;
					KeyRot[ 0 ] = KeyRot[ -1 ] ;
					KeyPos ++ ;
					KeyRot ++ ;
					KeyPosTime ++ ;
					KeyRotTime ++ ;
				}
			}
		}
	}

	// �J�����̃��[�V������񂪂���ꍇ�̓J��������ǉ�����
	if( VmdData.Camera != NULL )
	{
		_SWNPRINTF( String, sizeof( String ) / 2, L"Camera%03d", DataIndex ) ;
		if( SetupVMDCameraAnim( &VmdData, RModel, String, AnimSet ) < 0 )
			goto ENDLABEL ;
	}

	// �{�[����񂪂���ꍇ�݂̂��̐�̏������s��
	if( PmdBoneInfo != NULL )
	{
		// �A�j���[�V�����̍Đ�����
		BoneInfo = PmdBoneInfo ;
		for( i = 0 ; i < PmdBoneNum ; i ++, BoneInfo ++ )
		{
			BoneInfo->IsIKAnim = FALSE ;
			if( BoneInfo->Anim != NULL )
			{
				BoneInfo->NowKey = BoneInfo->Node->FirstKey ;
			}
			else
			if( BoneInfo->IsIK )
			{
				// IK�{�[���ŃA�j���[�V�������Ȃ��ꍇ�̓L�[��ł���������
				if( BoneInfo->Anim == NULL )
				{
					BoneInfo->IsIKAnim = TRUE ;

					// �A�j���[�V�����̒ǉ�
					BoneInfo->Anim = MV1RAddAnim( RModel, AnimSet ) ;
					if( BoneInfo->Anim == NULL )
					{
						DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x44\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xa2\x30\xcb\x30\xe1\x30\xfc\x30\xb7\x30\xe7\x30\xf3\x30\x6e\x30\xfd\x8f\xa0\x52\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMD Load Error : �A�j���[�V�����̒ǉ��Ɏ��s���܂���\n" @*/ )) ;
						goto ENDLABEL ;
					}

					// �Ώۃm�[�h�̃Z�b�g
					BoneInfo->Anim->TargetFrameIndex = BoneInfo->Frame->Index ;
					BoneInfo->Anim->MaxTime = ( float )VmdData.MaxTime ;

					// �A�j���[�V�����L�[�̏����Z�b�g
					KeyPosSet = MV1RAddAnimKeySet( RModel, BoneInfo->Anim ) ;
					if( KeyPosSet == NULL )
					{
						DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x44\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xa2\x30\xcb\x30\xe1\x30\xfc\x30\xb7\x30\xe7\x30\xf3\x30\xad\x30\xfc\x30\xbb\x30\xc3\x30\xc8\x30\x6e\x30\xfd\x8f\xa0\x52\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMD Load Error : �A�j���[�V�����L�[�Z�b�g�̒ǉ��Ɏ��s���܂���\n" @*/ )) ;
						goto ENDLABEL ;
					}
					KeyRotSet = MV1RAddAnimKeySet( RModel, BoneInfo->Anim ) ;
					if( KeyRotSet == NULL )
					{
						DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x44\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xa2\x30\xcb\x30\xe1\x30\xfc\x30\xb7\x30\xe7\x30\xf3\x30\xad\x30\xfc\x30\xbb\x30\xc3\x30\xc8\x30\x6e\x30\xfd\x8f\xa0\x52\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMD Load Error : �A�j���[�V�����L�[�Z�b�g�̒ǉ��Ɏ��s���܂���\n" @*/ )) ;
						goto ENDLABEL ;
					}

					KeyPosSet->Type = MV1_ANIMKEY_TYPE_VECTOR ;
					KeyPosSet->DataType = MV1_ANIMKEY_DATATYPE_TRANSLATE ;
					KeyPosSet->TimeType = MV1_ANIMKEY_TIME_TYPE_KEY ;
					KeyPosSet->TotalTime = ( float )VmdData.MaxTime ;
					KeyPosSet->Num = ( int )( VmdData.MaxTime * 2 + 1 ) ;

					KeyRotSet->Type = MV1_ANIMKEY_TYPE_QUATERNION_VMD ;
					KeyRotSet->DataType = MV1_ANIMKEY_DATATYPE_ROTATE ;
					KeyRotSet->TimeType = MV1_ANIMKEY_TIME_TYPE_KEY ;
					KeyRotSet->TotalTime = ( float )VmdData.MaxTime ;
					KeyRotSet->Num = ( int )( VmdData.MaxTime * 2 + 1 ) ;

					KeyPosSet->KeyTime = ( float * )ADDMEMAREA( sizeof( float ) * KeyPosSet->Num, &RModel->Mem ) ;
					if( KeyPosSet->KeyTime == NULL )
					{
						DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x44\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xa2\x30\xcb\x30\xe1\x30\xfc\x30\xb7\x30\xe7\x30\xf3\x30\xad\x30\xfc\x30\xbf\x30\xa4\x30\xe0\x30\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMD Load Error : �A�j���[�V�����L�[�^�C�����i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
						goto ENDLABEL ;
					}
					KeyPosSet->KeyVector = ( VECTOR * )ADDMEMAREA( sizeof( VECTOR ) * KeyPosSet->Num, &RModel->Mem ) ;
					if( KeyPosSet->KeyVector == NULL )
					{
						DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x44\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xa2\x30\xcb\x30\xe1\x30\xfc\x30\xb7\x30\xe7\x30\xf3\x30\xad\x30\xfc\x30\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMD Load Error : �A�j���[�V�����L�[���i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
						goto ENDLABEL ;
					}

					KeyRotSet->KeyTime = ( float * )ADDMEMAREA( sizeof( float ) * KeyRotSet->Num, &RModel->Mem ) ;
					if( KeyRotSet->KeyTime == NULL )
					{
						DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x44\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xa2\x30\xcb\x30\xe1\x30\xfc\x30\xb7\x30\xe7\x30\xf3\x30\xad\x30\xfc\x30\xbf\x30\xa4\x30\xe0\x30\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMD Load Error : �A�j���[�V�����L�[�^�C�����i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
						goto ENDLABEL ;
					}
					KeyRotSet->KeyVector = ( VECTOR * )ADDMEMAREA( sizeof( FLOAT4 ) * KeyRotSet->Num, &RModel->Mem ) ;
					if( KeyRotSet->KeyVector == NULL )
					{
						DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x44\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xa2\x30\xcb\x30\xe1\x30\xfc\x30\xb7\x30\xe7\x30\xf3\x30\xad\x30\xfc\x30\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMD Load Error : �A�j���[�V�����L�[���i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
						goto ENDLABEL ;
					}

					RModel->AnimKeyDataSize += KeyRotSet->Num * ( sizeof( float ) * 2 + sizeof( VECTOR ) + sizeof( FLOAT4 ) ) ;

					BoneInfo->KeyPos = KeyPosSet->KeyVector ;
					BoneInfo->KeyRot = KeyRotSet->KeyFloat4 ;
					BoneInfo->KeyPosTime = KeyPosSet->KeyTime ;
					BoneInfo->KeyRotTime = KeyRotSet->KeyTime ;
				}
			}
			else
			{
				BoneInfo->Translate = BoneInfo->Frame->Translate ;
				BoneInfo->Rotate.x = 0.0f ;
				BoneInfo->Rotate.y = 0.0f ;
				BoneInfo->Rotate.z = 0.0f ;
				BoneInfo->Rotate.w = 1.0f ;
			}

			if( BoneInfo->IsIKChild )
			{
				BoneInfo->KeyMatrix2 = ( MATRIX * )DXALLOC( sizeof( MATRIX ) * ( ( FPS60 ? VmdData.MaxTime * 2 : VmdData.MaxTime ) + 1 ) ) ;
				if( BoneInfo->KeyMatrix2 == NULL )
				{
					DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x44\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xa2\x30\xcb\x30\xe1\x30\xfc\x30\xb7\x30\xe7\x30\xf3\x30\x4c\x88\x17\x52\xad\x30\xfc\x30\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMD Load Error : �A�j���[�V�����s��L�[���i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
					goto ENDLABEL ;
				}
			}
		}

#ifndef DX_NON_BULLET_PHYSICS
		if( MLPhysicsInfo )
		{
			BoneInfo = PmdBoneInfo ;
			for( i = 0 ; i < PmdBoneNum ; i ++, BoneInfo ++ )
			{
				if( BoneInfo->IsPhysics == 0 ) continue ;

				BoneInfo->KeyMatrix = ( MATRIX * )DXALLOC( sizeof( MATRIX ) * ( ( FPS60 ? VmdData.MaxTime * 2 : VmdData.MaxTime ) + 1 ) ) ;
				if( BoneInfo->KeyMatrix == NULL )
				{
					DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x44\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xa2\x30\xcb\x30\xe1\x30\xfc\x30\xb7\x30\xe7\x30\xf3\x30\x4c\x88\x17\x52\xad\x30\xfc\x30\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMD Load Error : �A�j���[�V�����s��L�[���i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
					goto ENDLABEL ;
				}
				if( MLPhysicsInfo->LoopMotionFlag )
				{
					_MEMSET( BoneInfo->KeyMatrix, 0, sizeof( MATRIX ) * ( ( FPS60 ? VmdData.MaxTime * 2 : VmdData.MaxTime ) + 1 ) ) ;
				}
			}
		}
#endif

		// �A�j���[�V�������Đ�����
		bool IKSkip ;
		int PlayLoopNum ;
		int LoopNo ;
		int TimeNo ;
		int TimeDivLoopNum = 1 ;
		int MaxTime ;
		int ValidNextRate = FALSE ;
		float AddNextRate = 0.0f ;

#ifndef DX_NON_BULLET_PHYSICS
		if( MLPhysicsInfo )
		{
			PlayLoopNum = MLPhysicsInfo->LoopMotionFlag ? MLPhysicsInfo->LoopMotionNum : 1 ;
			MLPhysicsInfo->MotionTotalFrameNum = ( int )( VmdData.MaxTime * 2 + 1 ) ;

			ValidNextRate  = MLPhysicsInfo->PhysicsCalcPrecision == 0 ? FALSE : TRUE ;
			TimeDivLoopNum = 1 << MLPhysicsInfo->PhysicsCalcPrecision ;
			AddNextRate    = 1.0f / TimeDivLoopNum ;
		}
		else
		{
			PlayLoopNum = 1 ;
		}
#else
		PlayLoopNum = 1 ;
#endif
		MaxTime = ( int )( VmdData.MaxTime * 2 ) ;

		for( LoopNo = 0 ; LoopNo < PlayLoopNum ; LoopNo ++ )
		{
			// ���ׂẴt���[���̎Q�ƃA�j���[�V�����L�[�����Z�b�g����
			BoneInfo = PmdBoneInfo ;
			for( i = 0 ; i < PmdBoneNum ; i ++, BoneInfo ++ )
			{
				if( BoneInfo->Anim != NULL && BoneInfo->IsIK == FALSE )
				{
					BoneInfo->NowKey = BoneInfo->Node->FirstKey ;
				}
			}

			for( TimeNo = 0 ; TimeNo <= MaxTime ; TimeNo ++ )
			{
				IKSkip = ( TimeNo % 2 != 0 ) && FPS60 == false && ValidNextRate == FALSE ;

				int TimeDivLoopCount = 0 ;
				float NextRate       = 0.0f ;
				for( TimeDivLoopCount = 0 ; TimeDivLoopCount < TimeDivLoopNum ; TimeDivLoopCount ++, NextRate += AddNextRate )
				{
					if( IKSkip )
					{
						// ���ׂẴt���[���̌��݂̃t���[���ł̃p�����[�^���Z�o����
						for( j = 0 ; j < PmdBoneNum ; j ++ )
						{
							if( PmdBoneInfo[ j ].IsIK == FALSE )
							{
								MV1LoadModelToPMD_SetupOneBoneMatrixFormAnimKey( &PmdBoneInfo[ j ], TimeNo, LoopNo, MaxTime, TimeDivLoopCount == 0 ? FALSE : ValidNextRate, NextRate ) ;
							}
						}

						// �s��̌v�Z
						MV1LoadModelToPMD_SetupMatrix( PmdBoneInfo, PmdBoneNum, FALSE, TRUE ) ;
					}
					else
					{
						// ���ׂẴt���[���̌��݂̃t���[���ł̃p�����[�^���Z�o����
						for( j = 0 ; j < PmdBoneNum ; j ++ )
						{
							MV1LoadModelToPMD_SetupOneBoneMatrixFormAnimKey( &PmdBoneInfo[ j ], TimeNo, LoopNo, MaxTime, TimeDivLoopCount == 0 ? FALSE : ValidNextRate, NextRate ) ;
						}

						// �s��̌v�Z
						MV1LoadModelToPMD_SetupMatrix( PmdBoneInfo, PmdBoneNum, FALSE, FALSE ) ;

						// IK�̌v�Z���s���̂̓��[�V�������[�v�̍ŏ�����
						if( LoopNo == 0 )
						{
							// �h�j�̌v�Z
							if( PmdIKInfoFirst )
							{
								MV1LoadModelToPMD_SetupIK( PmdBoneInfo, PmdIKInfoFirst ) ;
							}
						}
					}

#ifndef DX_NON_BULLET_PHYSICS
					// �������Z���s��
					if( MLPhysicsInfo )
					{
						OneFrameProcess_PMDPhysicsInfo( MLPhysicsInfo, TimeNo, LoopNo, FPS60, TimeDivLoopCount == 0 ? FALSE : ValidNextRate, TimeDivLoopNum ) ;
					}
#endif
				}

				if( LoopNo == 0 && ( ValidNextRate || FPS60 || ( FPS60 == false && TimeNo % 2 == 0 ) ) )
				{
					// �h�j�Ɋւ���Ă���{�[�����͂h�j�̉e�����Ȃ��h�j�{�[���̎q�{�[���̃L�[��ۑ�
					BoneInfo = PmdBoneInfo ;
					for( j = 0 ; j < PmdBoneNum ; j ++, BoneInfo ++ )
					{
						if( BoneInfo->IsIK )
						{
							BoneInfo->KeyPosTime[ TimeNo ] = ( float )TimeNo / 2.0f ;
							BoneInfo->KeyRotTime[ TimeNo ] = ( float )TimeNo / 2.0f ;
							BoneInfo->KeyPos[ TimeNo ] = BoneInfo->Translate ;
							BoneInfo->KeyRot[ TimeNo ] = BoneInfo->Rotate ;
						}
						else
						if( BoneInfo->IsIKChild )
						{
							int DestIndex ;

							DestIndex = FPS60 ? TimeNo : TimeNo / 2 ;

							if( BoneInfo->Frame->Parent )
							{
								MATRIX InvParentBoneLWM ;

								// �e�{�[������̋t�s��������̍s��Ɋ|���āA�{�[���̃��[�J���s����擾����
								MV1LoadModelToVMD_InverseMatrix( ( ( PMD_READ_BONE_INFO * )BoneInfo->Frame->Parent->UserData )->LocalWorldMatrix, InvParentBoneLWM ) ;
								MV1LoadModelToVMD_CreateMultiplyMatrix( &BoneInfo->KeyMatrix2[ DestIndex ], &BoneInfo->LocalWorldMatrix, &InvParentBoneLWM ) ;
							}
							else
							{
								BoneInfo->KeyMatrix2[ DestIndex ] = BoneInfo->LocalWorldMatrix ;
							}
						}
					}
				}
			}
		}

#ifndef DX_NON_BULLET_PHYSICS
		if( MLPhysicsInfo )
		{
			MV1_ANIMKEYSET_R *KeyMatrixSet ;
			int PmdPhysicsNum ;

			// �����K���̃A�j���[�V�����f�[�^���Z�b�g����
			PmdPhysicsNum = MLPhysicsInfo->PmdPhysicsNum ;
			for( i = 0 ; i < PmdPhysicsNum ; i ++ )
			{
				if( CheckDisablePhysicsAnim_PMDPhysicsInfo( MLPhysicsInfo, i ) ) continue ;

				BoneInfo = MLPhysicsInfo->PmdPhysicsInfoDim[ i ].Bone ;

				// ���łɃA�j���̃f�[�^���{�[���ɂ��Ă��Ċ������̃A�j���[�V���������Ă��Ȃ����ǂ����ŏ����𕪊�
				if( BoneInfo->Anim != NULL && BoneInfo->SetupPhysicsAnim == 0 )
				{
					// ���ɂ���ꍇ�͕Е��� MATRIX3X3�^�A�����Е��� VECTOR�^ �� TRANSLATE �ɂ���
					if( BoneInfo->Anim->AnimKeySetFirst->Type == MV1_ANIMKEY_TYPE_VECTOR )
					{
						KeyPosSet = BoneInfo->Anim->AnimKeySetFirst ;
						KeyRotSet = BoneInfo->Anim->AnimKeySetLast ;
					}
					else
					{
						KeyRotSet = BoneInfo->Anim->AnimKeySetFirst ;
						KeyPosSet = BoneInfo->Anim->AnimKeySetLast ;
					}
					KeyMatrixSet = KeyRotSet ;

					// �L�[�p�������̍Ċm��
					KeyMatrixSet->KeyMatrix3x3 = ( MV1_ANIM_KEY_MATRIX3X3 * )ADDMEMAREA( sizeof( MV1_ANIM_KEY_MATRIX3X3 ) * ( ( FPS60 ? VmdData.MaxTime * 2 : VmdData.MaxTime ) + 1 ), &RModel->Mem ) ;
					if( KeyMatrixSet->KeyMatrix3x3 == NULL )
					{
						DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x44\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\x4c\x88\x17\x52\xa2\x30\xcb\x30\xe1\x30\xfc\x30\xb7\x30\xe7\x30\xf3\x30\xad\x30\xfc\x30\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMD Load Error : �s��A�j���[�V�����L�[���i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
						goto ENDLABEL ;
					}
					if( BoneInfo->Anim->AnimKeySetFirst->Num != ( int )( VmdData.MaxTime * 2 + 1 ) )
					{
						RModel->AnimKeyDataSize -= KeyMatrixSet->Num * ( sizeof( float ) * 2 + sizeof( VECTOR ) + sizeof( FLOAT4 ) ) ;

						if( FPS60 )
						{
							KeyPosSet->Num    = ( int )( VmdData.MaxTime * 2 + 1 ) ;
							KeyMatrixSet->Num = ( int )( VmdData.MaxTime * 2 + 1 ) ;
						}
						else
						{
							KeyPosSet->Num    = ( int )( VmdData.MaxTime + 1 ) ;
							KeyMatrixSet->Num = ( int )( VmdData.MaxTime + 1 ) ;
						}

						RModel->AnimKeyDataSize += KeyMatrixSet->Num * ( sizeof( float ) * 2 + sizeof( VECTOR ) + sizeof( MV1_ANIM_KEY_MATRIX3X3 ) ) ;

						KeyPosSet->KeyVector = ( VECTOR * )ADDMEMAREA( sizeof( VECTOR ) * KeyPosSet->Num, &RModel->Mem ) ;
						if( KeyPosSet->KeyVector == NULL )
						{
							DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x44\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xa7\x5e\x19\x6a\xa2\x30\xcb\x30\xe1\x30\xfc\x30\xb7\x30\xe7\x30\xf3\x30\xad\x30\xfc\x30\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMD Load Error : ���W�A�j���[�V�����L�[���i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
							goto ENDLABEL ;
						}
						KeyPosSet->KeyTime = ( float * )ADDMEMAREA( sizeof( float ) * KeyPosSet->Num, &RModel->Mem ) ;
						if( KeyPosSet->KeyTime == NULL )
						{
							DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x44\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xa2\x30\xcb\x30\xe1\x30\xfc\x30\xb7\x30\xe7\x30\xf3\x30\xad\x30\xfc\x30\xbf\x30\xa4\x30\xe0\x30\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMD Load Error : �A�j���[�V�����L�[�^�C�����i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
							goto ENDLABEL ;
						}
						KeyMatrixSet->KeyTime = ( float * )ADDMEMAREA( sizeof( float ) * KeyMatrixSet->Num, &RModel->Mem ) ;
						if( KeyMatrixSet->KeyTime == NULL )
						{
							DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x44\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xa2\x30\xcb\x30\xe1\x30\xfc\x30\xb7\x30\xe7\x30\xf3\x30\xad\x30\xfc\x30\xbf\x30\xa4\x30\xe0\x30\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMD Load Error : �A�j���[�V�����L�[�^�C�����i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
							goto ENDLABEL ;
						}
					}
					else
					{
						RModel->AnimKeyDataSize -= KeyMatrixSet->Num * sizeof( FLOAT4 ) ;

						if( FPS60 == false )
						{
							KeyPosSet->Num    = ( int )( VmdData.MaxTime + 1 ) ;
							KeyMatrixSet->Num = ( int )( VmdData.MaxTime + 1 ) ;
						}

						RModel->AnimKeyDataSize += KeyMatrixSet->Num * sizeof( MV1_ANIM_KEY_MATRIX3X3 ) ;
					}

					KeyMatrixSet->Type = MV1_ANIMKEY_TYPE_MATRIX3X3 ;
					KeyMatrixSet->DataType = MV1_ANIMKEY_DATATYPE_MATRIX3X3 ;

					for( j = 0 ; j < KeyMatrixSet->Num ; j ++ )
					{
						if( FPS60 )
						{
							KeyMatrixSet->KeyTime[ j ] = ( float )j / 2.0f ;
							KeyPosSet->KeyTime[ j ]    = ( float )j / 2.0f ;
						}
						else
						{
							KeyMatrixSet->KeyTime[ j ] = ( float )j ;
							KeyPosSet->KeyTime[ j ]    = ( float )j ;
						}

						KeyMatrixSet->KeyMatrix3x3[ j ].Matrix[ 0 ][ 0 ] = BoneInfo->KeyMatrix[ j ].m[ 0 ][ 0 ] ;
						KeyMatrixSet->KeyMatrix3x3[ j ].Matrix[ 0 ][ 1 ] = BoneInfo->KeyMatrix[ j ].m[ 0 ][ 1 ] ;
						KeyMatrixSet->KeyMatrix3x3[ j ].Matrix[ 0 ][ 2 ] = BoneInfo->KeyMatrix[ j ].m[ 0 ][ 2 ] ;

						KeyMatrixSet->KeyMatrix3x3[ j ].Matrix[ 1 ][ 0 ] = BoneInfo->KeyMatrix[ j ].m[ 1 ][ 0 ] ;
						KeyMatrixSet->KeyMatrix3x3[ j ].Matrix[ 1 ][ 1 ] = BoneInfo->KeyMatrix[ j ].m[ 1 ][ 1 ] ;
						KeyMatrixSet->KeyMatrix3x3[ j ].Matrix[ 1 ][ 2 ] = BoneInfo->KeyMatrix[ j ].m[ 1 ][ 2 ] ;

						KeyMatrixSet->KeyMatrix3x3[ j ].Matrix[ 2 ][ 0 ] = BoneInfo->KeyMatrix[ j ].m[ 2 ][ 0 ] ;
						KeyMatrixSet->KeyMatrix3x3[ j ].Matrix[ 2 ][ 1 ] = BoneInfo->KeyMatrix[ j ].m[ 2 ][ 1 ] ;
						KeyMatrixSet->KeyMatrix3x3[ j ].Matrix[ 2 ][ 2 ] = BoneInfo->KeyMatrix[ j ].m[ 2 ][ 2 ] ;

						KeyPosSet->KeyVector[ j ].x = BoneInfo->KeyMatrix[ j ].m[ 3 ][ 0 ] ;
						KeyPosSet->KeyVector[ j ].y = BoneInfo->KeyMatrix[ j ].m[ 3 ][ 1 ] ;
						KeyPosSet->KeyVector[ j ].z = BoneInfo->KeyMatrix[ j ].m[ 3 ][ 2 ] ;
					}
				}
				else
				{
					// �Ȃ��������̃A�j���[�V�������t���Ă���ꍇ�̏���

					// �V���ɃA�j���[�V������ǉ�
					BoneInfo->Anim = MV1RAddAnim( RModel, AnimSet ) ;
					if( BoneInfo->Anim == NULL )
					{
						DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x44\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\x4c\x88\x17\x52\xa2\x30\xcb\x30\xe1\x30\xfc\x30\xb7\x30\xe7\x30\xf3\x30\x6e\x30\xfd\x8f\xa0\x52\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMD Load Error : �s��A�j���[�V�����̒ǉ��Ɏ��s���܂���\n" @*/ )) ;
						goto ENDLABEL ;
					}

					// �Ώۃm�[�h�̃Z�b�g
					BoneInfo->Anim->TargetFrameIndex = BoneInfo->Frame->Index ;
					BoneInfo->Anim->MaxTime = ( float )VmdData.MaxTime ;

					// �A�j���[�V�����L�[�̏����Z�b�g
					KeyMatrixSet = MV1RAddAnimKeySet( RModel, BoneInfo->Anim ) ;
					if( KeyMatrixSet == NULL )
					{
						DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x44\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\x4c\x88\x17\x52\xa2\x30\xcb\x30\xe1\x30\xfc\x30\xb7\x30\xe7\x30\xf3\x30\xad\x30\xfc\x30\xbb\x30\xc3\x30\xc8\x30\x6e\x30\xfd\x8f\xa0\x52\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMD Load Error : �s��A�j���[�V�����L�[�Z�b�g�̒ǉ��Ɏ��s���܂���\n" @*/ )) ;
						goto ENDLABEL ;
					}

					KeyMatrixSet->Type = MV1_ANIMKEY_TYPE_MATRIX4X4C ;
					KeyMatrixSet->DataType = MV1_ANIMKEY_DATATYPE_MATRIX4X4C ;
					KeyMatrixSet->TimeType = MV1_ANIMKEY_TIME_TYPE_KEY ;
					KeyMatrixSet->TotalTime = ( float )VmdData.MaxTime ;
					KeyMatrixSet->Num = ( int )VmdData.MaxTime ;
					if( FPS60 ) KeyMatrixSet->Num *= 2  ;
					KeyMatrixSet->Num += 1 ;

					KeyMatrixSet->KeyTime = ( float * )ADDMEMAREA( sizeof( float ) * KeyMatrixSet->Num, &RModel->Mem ) ;
					if( KeyMatrixSet->KeyTime == NULL )
					{
						DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x44\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\x4c\x88\x17\x52\xa2\x30\xcb\x30\xe1\x30\xfc\x30\xb7\x30\xe7\x30\xf3\x30\xad\x30\xfc\x30\xbf\x30\xa4\x30\xe0\x30\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMD Load Error : �s��A�j���[�V�����L�[�^�C�����i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
						goto ENDLABEL ;
					}
					KeyMatrixSet->KeyMatrix4x4C = ( MV1_ANIM_KEY_MATRIX4X4C * )ADDMEMAREA( sizeof( MV1_ANIM_KEY_MATRIX4X4C ) * KeyMatrixSet->Num, &RModel->Mem ) ;
					if( KeyMatrixSet->KeyMatrix4x4C == NULL )
					{
						DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x44\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\x4c\x88\x17\x52\xa2\x30\xcb\x30\xe1\x30\xfc\x30\xb7\x30\xe7\x30\xf3\x30\xad\x30\xfc\x30\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMD Load Error : �s��A�j���[�V�����L�[���i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
						goto ENDLABEL ;
					}

					RModel->AnimKeyDataSize += KeyMatrixSet->Num * ( sizeof( float ) + sizeof( MV1_ANIM_KEY_MATRIX4X4C ) ) ;

					for( j = 0 ; j < KeyMatrixSet->Num ; j ++ )
					{
						KeyMatrixSet->KeyTime[ j ] = FPS60 ? ( float )j / 2.0f : ( float )j ;

						KeyMatrixSet->KeyMatrix4x4C[ j ].Matrix[ 0 ][ 0 ] = BoneInfo->KeyMatrix[ j ].m[ 0 ][ 0 ] ;
						KeyMatrixSet->KeyMatrix4x4C[ j ].Matrix[ 0 ][ 1 ] = BoneInfo->KeyMatrix[ j ].m[ 0 ][ 1 ] ;
						KeyMatrixSet->KeyMatrix4x4C[ j ].Matrix[ 0 ][ 2 ] = BoneInfo->KeyMatrix[ j ].m[ 0 ][ 2 ] ;

						KeyMatrixSet->KeyMatrix4x4C[ j ].Matrix[ 1 ][ 0 ] = BoneInfo->KeyMatrix[ j ].m[ 1 ][ 0 ] ;
						KeyMatrixSet->KeyMatrix4x4C[ j ].Matrix[ 1 ][ 1 ] = BoneInfo->KeyMatrix[ j ].m[ 1 ][ 1 ] ;
						KeyMatrixSet->KeyMatrix4x4C[ j ].Matrix[ 1 ][ 2 ] = BoneInfo->KeyMatrix[ j ].m[ 1 ][ 2 ] ;

						KeyMatrixSet->KeyMatrix4x4C[ j ].Matrix[ 2 ][ 0 ] = BoneInfo->KeyMatrix[ j ].m[ 2 ][ 0 ] ;
						KeyMatrixSet->KeyMatrix4x4C[ j ].Matrix[ 2 ][ 1 ] = BoneInfo->KeyMatrix[ j ].m[ 2 ][ 1 ] ;
						KeyMatrixSet->KeyMatrix4x4C[ j ].Matrix[ 2 ][ 2 ] = BoneInfo->KeyMatrix[ j ].m[ 2 ][ 2 ] ;

						KeyMatrixSet->KeyMatrix4x4C[ j ].Matrix[ 3 ][ 0 ] = BoneInfo->KeyMatrix[ j ].m[ 3 ][ 0 ] ;
						KeyMatrixSet->KeyMatrix4x4C[ j ].Matrix[ 3 ][ 1 ] = BoneInfo->KeyMatrix[ j ].m[ 3 ][ 1 ] ;
						KeyMatrixSet->KeyMatrix4x4C[ j ].Matrix[ 3 ][ 2 ] = BoneInfo->KeyMatrix[ j ].m[ 3 ][ 2 ] ;
					}
				}

				// �����̃A�j����t�����؋����c��
				BoneInfo->SetupPhysicsAnim = 1 ;
			}
		}
#endif

		// �h�j�{�[���̉e�����󂯂Ȃ��h�j�{�[���̉e�����󂯂�{�[���̒����ɂ���{�[���̍s��L�[�̏���������
		{
			MV1_ANIMKEYSET_R *KeyMatrixSet ;

			BoneInfo = PmdBoneInfo ;
			for( i = 0 ; i < PmdBoneNum ; i ++, BoneInfo ++ )
			{
				if( BoneInfo->IsIKChild == FALSE )
					continue ;

				if( BoneInfo->IsPhysics )
					continue ;

				// ���łɃA�j���̃f�[�^���{�[���ɂ��Ă��邩�ǂ����ŏ����𕪊�
				if( BoneInfo->Anim != NULL )
				{
					// ���ɂ���ꍇ�͕Е��� MATRIX3X3�^�A�����Е��� VECTOR�^ �� TRANSLATE �ɂ���
					if( BoneInfo->Anim->AnimKeySetFirst->Type == MV1_ANIMKEY_TYPE_VECTOR )
					{
						KeyPosSet = BoneInfo->Anim->AnimKeySetFirst ;
						KeyRotSet = BoneInfo->Anim->AnimKeySetLast ;
					}
					else
					{
						KeyRotSet = BoneInfo->Anim->AnimKeySetFirst ;
						KeyPosSet = BoneInfo->Anim->AnimKeySetLast ;
					}
					KeyMatrixSet = KeyRotSet ;

					// �L�[�p�������̍Ċm��
					KeyMatrixSet->KeyMatrix3x3 = ( MV1_ANIM_KEY_MATRIX3X3 * )ADDMEMAREA( sizeof( MV1_ANIM_KEY_MATRIX3X3 ) * ( ( FPS60 ? VmdData.MaxTime * 2 : VmdData.MaxTime ) + 1 ), &RModel->Mem ) ;
					if( KeyMatrixSet->KeyMatrix3x3 == NULL )
					{
						DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x44\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\x4c\x88\x17\x52\xa2\x30\xcb\x30\xe1\x30\xfc\x30\xb7\x30\xe7\x30\xf3\x30\xad\x30\xfc\x30\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMD Load Error : �s��A�j���[�V�����L�[���i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
						goto ENDLABEL ;
					}
					if( BoneInfo->Anim->AnimKeySetFirst->Num != ( int )( VmdData.MaxTime * 2 + 1 ) )
					{
						RModel->AnimKeyDataSize -= KeyMatrixSet->Num * ( sizeof( float ) * 2 + sizeof( VECTOR ) + sizeof( FLOAT4 ) ) ;

						if( FPS60 )
						{
							KeyPosSet->Num    = ( int )( VmdData.MaxTime * 2 + 1 ) ;
							KeyMatrixSet->Num = ( int )( VmdData.MaxTime * 2 + 1 ) ;
						}
						else
						{
							KeyPosSet->Num    = ( int )( VmdData.MaxTime + 1 ) ;
							KeyMatrixSet->Num = ( int )( VmdData.MaxTime + 1 ) ;
						}

						RModel->AnimKeyDataSize += KeyMatrixSet->Num * ( sizeof( float ) * 2 + sizeof( VECTOR ) + sizeof( MV1_ANIM_KEY_MATRIX3X3 ) ) ;

						KeyPosSet->KeyVector = ( VECTOR * )ADDMEMAREA( sizeof( VECTOR ) * KeyPosSet->Num, &RModel->Mem ) ;
						if( KeyPosSet->KeyVector == NULL )
						{
							DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x44\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xa7\x5e\x19\x6a\xa2\x30\xcb\x30\xe1\x30\xfc\x30\xb7\x30\xe7\x30\xf3\x30\xad\x30\xfc\x30\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMD Load Error : ���W�A�j���[�V�����L�[���i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
							goto ENDLABEL ;
						}
						KeyPosSet->KeyTime = ( float * )ADDMEMAREA( sizeof( float ) * KeyPosSet->Num, &RModel->Mem ) ;
						if( KeyPosSet->KeyTime == NULL )
						{
							DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x44\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xa2\x30\xcb\x30\xe1\x30\xfc\x30\xb7\x30\xe7\x30\xf3\x30\xad\x30\xfc\x30\xbf\x30\xa4\x30\xe0\x30\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMD Load Error : �A�j���[�V�����L�[�^�C�����i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
							goto ENDLABEL ;
						}
						KeyMatrixSet->KeyTime = ( float * )ADDMEMAREA( sizeof( float ) * KeyMatrixSet->Num, &RModel->Mem ) ;
						if( KeyMatrixSet->KeyTime == NULL )
						{
							DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x44\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xa2\x30\xcb\x30\xe1\x30\xfc\x30\xb7\x30\xe7\x30\xf3\x30\xad\x30\xfc\x30\xbf\x30\xa4\x30\xe0\x30\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMD Load Error : �A�j���[�V�����L�[�^�C�����i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
							goto ENDLABEL ;
						}
					}
					else
					{
						RModel->AnimKeyDataSize -= KeyMatrixSet->Num * sizeof( FLOAT4 ) ;

						if( FPS60 == false )
						{
							KeyPosSet->Num    = ( int )( VmdData.MaxTime + 1 ) ;
							KeyMatrixSet->Num = ( int )( VmdData.MaxTime + 1 ) ;
						}

						RModel->AnimKeyDataSize += KeyMatrixSet->Num * sizeof( MV1_ANIM_KEY_MATRIX3X3 ) ;
					}

					KeyMatrixSet->Type = MV1_ANIMKEY_TYPE_MATRIX3X3 ;
					KeyMatrixSet->DataType = MV1_ANIMKEY_DATATYPE_MATRIX3X3 ;

					for( j = 0 ; j < KeyMatrixSet->Num ; j ++ )
					{
						if( FPS60 )
						{
							KeyMatrixSet->KeyTime[ j ] = ( float )j / 2.0f ;
							KeyPosSet->KeyTime[ j ]    = ( float )j / 2.0f ;
						}
						else
						{
							KeyMatrixSet->KeyTime[ j ] = ( float )j ;
							KeyPosSet->KeyTime[ j ]    = ( float )j ;
						}

						KeyMatrixSet->KeyMatrix3x3[ j ].Matrix[ 0 ][ 0 ] = BoneInfo->KeyMatrix2[ j ].m[ 0 ][ 0 ] ;
						KeyMatrixSet->KeyMatrix3x3[ j ].Matrix[ 0 ][ 1 ] = BoneInfo->KeyMatrix2[ j ].m[ 0 ][ 1 ] ;
						KeyMatrixSet->KeyMatrix3x3[ j ].Matrix[ 0 ][ 2 ] = BoneInfo->KeyMatrix2[ j ].m[ 0 ][ 2 ] ;

						KeyMatrixSet->KeyMatrix3x3[ j ].Matrix[ 1 ][ 0 ] = BoneInfo->KeyMatrix2[ j ].m[ 1 ][ 0 ] ;
						KeyMatrixSet->KeyMatrix3x3[ j ].Matrix[ 1 ][ 1 ] = BoneInfo->KeyMatrix2[ j ].m[ 1 ][ 1 ] ;
						KeyMatrixSet->KeyMatrix3x3[ j ].Matrix[ 1 ][ 2 ] = BoneInfo->KeyMatrix2[ j ].m[ 1 ][ 2 ] ;

						KeyMatrixSet->KeyMatrix3x3[ j ].Matrix[ 2 ][ 0 ] = BoneInfo->KeyMatrix2[ j ].m[ 2 ][ 0 ] ;
						KeyMatrixSet->KeyMatrix3x3[ j ].Matrix[ 2 ][ 1 ] = BoneInfo->KeyMatrix2[ j ].m[ 2 ][ 1 ] ;
						KeyMatrixSet->KeyMatrix3x3[ j ].Matrix[ 2 ][ 2 ] = BoneInfo->KeyMatrix2[ j ].m[ 2 ][ 2 ] ;

						KeyPosSet->KeyVector[ j ].x = BoneInfo->KeyMatrix2[ j ].m[ 3 ][ 0 ] ;
						KeyPosSet->KeyVector[ j ].y = BoneInfo->KeyMatrix2[ j ].m[ 3 ][ 1 ] ;
						KeyPosSet->KeyVector[ j ].z = BoneInfo->KeyMatrix2[ j ].m[ 3 ][ 2 ] ;
					}
				}
				else
				{
					// �Ȃ��ꍇ�͂����Œǉ�

					BoneInfo->Anim = MV1RAddAnim( RModel, AnimSet ) ;
					if( BoneInfo->Anim == NULL )
					{
						DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x44\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\x4c\x88\x17\x52\xa2\x30\xcb\x30\xe1\x30\xfc\x30\xb7\x30\xe7\x30\xf3\x30\x6e\x30\xfd\x8f\xa0\x52\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMD Load Error : �s��A�j���[�V�����̒ǉ��Ɏ��s���܂���\n" @*/ )) ;
						goto ENDLABEL ;
					}

					// �Ώۃm�[�h�̃Z�b�g
					BoneInfo->Anim->TargetFrameIndex = BoneInfo->Frame->Index ;
					BoneInfo->Anim->MaxTime = ( float )VmdData.MaxTime ;

					// �A�j���[�V�����L�[�̏����Z�b�g
					KeyMatrixSet = MV1RAddAnimKeySet( RModel, BoneInfo->Anim ) ;
					if( KeyMatrixSet == NULL )
					{
						DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x44\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\x4c\x88\x17\x52\xa2\x30\xcb\x30\xe1\x30\xfc\x30\xb7\x30\xe7\x30\xf3\x30\xad\x30\xfc\x30\xbb\x30\xc3\x30\xc8\x30\x6e\x30\xfd\x8f\xa0\x52\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMD Load Error : �s��A�j���[�V�����L�[�Z�b�g�̒ǉ��Ɏ��s���܂���\n" @*/ )) ;
						goto ENDLABEL ;
					}

					KeyMatrixSet->Type = MV1_ANIMKEY_TYPE_MATRIX4X4C ;
					KeyMatrixSet->DataType = MV1_ANIMKEY_DATATYPE_MATRIX4X4C ;
					KeyMatrixSet->TimeType = MV1_ANIMKEY_TIME_TYPE_KEY ;
					KeyMatrixSet->TotalTime = ( float )VmdData.MaxTime ;
					KeyMatrixSet->Num = ( int )VmdData.MaxTime ;
					if( FPS60 ) KeyMatrixSet->Num *= 2  ;
					KeyMatrixSet->Num += 1 ;

					KeyMatrixSet->KeyTime = ( float * )ADDMEMAREA( sizeof( float ) * KeyMatrixSet->Num, &RModel->Mem ) ;
					if( KeyMatrixSet->KeyTime == NULL )
					{
						DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x44\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\x4c\x88\x17\x52\xa2\x30\xcb\x30\xe1\x30\xfc\x30\xb7\x30\xe7\x30\xf3\x30\xad\x30\xfc\x30\xbf\x30\xa4\x30\xe0\x30\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMD Load Error : �s��A�j���[�V�����L�[�^�C�����i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
						goto ENDLABEL ;
					}
					KeyMatrixSet->KeyMatrix4x4C = ( MV1_ANIM_KEY_MATRIX4X4C * )ADDMEMAREA( sizeof( MV1_ANIM_KEY_MATRIX4X4C ) * KeyMatrixSet->Num, &RModel->Mem ) ;
					if( KeyMatrixSet->KeyMatrix4x4C == NULL )
					{
						DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x44\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\x4c\x88\x17\x52\xa2\x30\xcb\x30\xe1\x30\xfc\x30\xb7\x30\xe7\x30\xf3\x30\xad\x30\xfc\x30\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMD Load Error : �s��A�j���[�V�����L�[���i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
						goto ENDLABEL ;
					}

					RModel->AnimKeyDataSize += KeyMatrixSet->Num * ( sizeof( float ) + sizeof( MV1_ANIM_KEY_MATRIX4X4C ) ) ;

					for( j = 0 ; j < KeyMatrixSet->Num ; j ++ )
					{
						KeyMatrixSet->KeyTime[ j ] = FPS60 ? ( float )j / 2.0f : ( float )j ;

						KeyMatrixSet->KeyMatrix4x4C[ j ].Matrix[ 0 ][ 0 ] = BoneInfo->KeyMatrix2[ j ].m[ 0 ][ 0 ] ;
						KeyMatrixSet->KeyMatrix4x4C[ j ].Matrix[ 0 ][ 1 ] = BoneInfo->KeyMatrix2[ j ].m[ 0 ][ 1 ] ;
						KeyMatrixSet->KeyMatrix4x4C[ j ].Matrix[ 0 ][ 2 ] = BoneInfo->KeyMatrix2[ j ].m[ 0 ][ 2 ] ;

						KeyMatrixSet->KeyMatrix4x4C[ j ].Matrix[ 1 ][ 0 ] = BoneInfo->KeyMatrix2[ j ].m[ 1 ][ 0 ] ;
						KeyMatrixSet->KeyMatrix4x4C[ j ].Matrix[ 1 ][ 1 ] = BoneInfo->KeyMatrix2[ j ].m[ 1 ][ 1 ] ;
						KeyMatrixSet->KeyMatrix4x4C[ j ].Matrix[ 1 ][ 2 ] = BoneInfo->KeyMatrix2[ j ].m[ 1 ][ 2 ] ;

						KeyMatrixSet->KeyMatrix4x4C[ j ].Matrix[ 2 ][ 0 ] = BoneInfo->KeyMatrix2[ j ].m[ 2 ][ 0 ] ;
						KeyMatrixSet->KeyMatrix4x4C[ j ].Matrix[ 2 ][ 1 ] = BoneInfo->KeyMatrix2[ j ].m[ 2 ][ 1 ] ;
						KeyMatrixSet->KeyMatrix4x4C[ j ].Matrix[ 2 ][ 2 ] = BoneInfo->KeyMatrix2[ j ].m[ 2 ][ 2 ] ;

						KeyMatrixSet->KeyMatrix4x4C[ j ].Matrix[ 3 ][ 0 ] = BoneInfo->KeyMatrix2[ j ].m[ 3 ][ 0 ] ;
						KeyMatrixSet->KeyMatrix4x4C[ j ].Matrix[ 3 ][ 1 ] = BoneInfo->KeyMatrix2[ j ].m[ 3 ][ 1 ] ;
						KeyMatrixSet->KeyMatrix4x4C[ j ].Matrix[ 3 ][ 2 ] = BoneInfo->KeyMatrix2[ j ].m[ 3 ][ 2 ] ;
					}
				}
			}
		}

		// �R�O�e�o�r�ۑ��w��̏ꍇ�͂����ŃA�j���[�V�����L�[�����
		if( FPS60 == false )
		{
			BoneInfo = PmdBoneInfo ;
			for( i = 0 ; i < PmdBoneNum ; i ++, BoneInfo ++ )
			{
#ifndef DX_NON_BULLET_PHYSICS
				if( MLPhysicsInfo && BoneInfo->IsPhysics == TRUE && CheckDisablePhysicsAnim_PMDPhysicsInfo( MLPhysicsInfo, BoneInfo->PhysicsIndex ) == false ) continue ;
#endif
				if( BoneInfo->IsIKChild ) continue ;

				if( BoneInfo->Anim == NULL ) continue ;

				if( BoneInfo->Anim->AnimKeySetFirst->Type == MV1_ANIMKEY_TYPE_VECTOR )
				{
					KeyPosSet = BoneInfo->Anim->AnimKeySetFirst ;
					KeyRotSet = BoneInfo->Anim->AnimKeySetLast ;
				}
				else
				{
					KeyRotSet = BoneInfo->Anim->AnimKeySetFirst ;
					KeyPosSet = BoneInfo->Anim->AnimKeySetLast ;
				}

				if( KeyPosSet->Num != KeyRotSet->Num || KeyPosSet->Num == 1 || KeyPosSet->Num % 2 == 0 ) continue ;

				RModel->AnimKeyDataSize -= KeyPosSet->Num * ( sizeof( float ) * 2 + sizeof( VECTOR ) + sizeof( FLOAT4 ) ) ;

				KeyPosSet->Num = KeyPosSet->Num / 2 + 1 ;
				KeyRotSet->Num = KeyRotSet->Num / 2 + 1 ;

				RModel->AnimKeyDataSize += KeyPosSet->Num * ( sizeof( float ) * 2 + sizeof( VECTOR ) + sizeof( FLOAT4 ) ) ;

				for( j = 0 ; j < KeyPosSet->Num ; j ++ )
				{
					KeyRotSet->KeyTime[ j ] = ( float )j ;
					KeyPosSet->KeyTime[ j ] = ( float )j ;

					KeyPosSet->KeyVector[ j ] = KeyPosSet->KeyVector[ j * 2 ] ;
					KeyRotSet->KeyFloat4[ j ] = KeyRotSet->KeyFloat4[ j * 2 ] ;
				}
			}
		}

		// �\��A�j���[�V������ǉ�����
		if( VmdData.FaceKeySetNum != 0 )
		{
			// �A�j���[�V�����̒ǉ�
			Anim = MV1RAddAnim( RModel, AnimSet ) ;
			if( Anim == NULL )
			{
				DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x44\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xa2\x30\xcb\x30\xe1\x30\xfc\x30\xb7\x30\xe7\x30\xf3\x30\x6e\x30\xfd\x8f\xa0\x52\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMD Load Error : �A�j���[�V�����̒ǉ��Ɏ��s���܂���\n" @*/ )) ;
				goto ENDLABEL ;
			}

			// �Ώۃm�[�h�̃Z�b�g
			Anim->TargetFrameIndex = 0 ;

			// �ő厞�Ԃ��Z�b�g
			Anim->MaxTime = 0.0f ;

			// �\��L�[�Z�b�g�̐������J��Ԃ�
			VmdFaceKeySet = VmdData.FaceKeySet ;
			Frame = RModel->FrameFirst ; 
			for( i = 0 ; ( DWORD )i < VmdData.FaceKeySetNum ; i ++, VmdFaceKeySet ++ )
			{
				// �ΏۂƂȂ�V�F�C�v�f�[�^�̌���
				wchar_t VmdFaceKeySetNameW[ 128 ] ;
				ConvString( VmdFaceKeySet->Name, DX_CHARCODEFORMAT_SHIFTJIS, ( char * )VmdFaceKeySetNameW, sizeof( VmdFaceKeySetNameW ), WCHAR_T_CHARCODEFORMAT ) ;

				Shape = Frame->ShapeFirst ;
				for( j = 0 ; j < Frame->ShapeNum ; j ++, Shape = Shape->Next )
				{
					if( _WCSCMP( Shape->NameW, VmdFaceKeySetNameW ) == 0 ) break ;
				}
				if( j == Frame->ShapeNum ) continue ;

				// �Đ����Ԃ̍X�V
				if( Anim->MaxTime < ( float )VmdFaceKeySet->MaxFrame ) Anim->MaxTime = ( float )VmdFaceKeySet->MaxFrame ;

				// �A�j���[�V�����L�[�̏����Z�b�g
				KeyFactorSet = MV1RAddAnimKeySet( RModel, Anim ) ;
				if( KeyFactorSet == NULL )
				{
					DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x44\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xa2\x30\xcb\x30\xe1\x30\xfc\x30\xb7\x30\xe7\x30\xf3\x30\x68\x88\xc5\x60\xad\x30\xfc\x30\xbb\x30\xc3\x30\xc8\x30\x6e\x30\xfd\x8f\xa0\x52\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMD Load Error : �A�j���[�V�����\��L�[�Z�b�g�̒ǉ��Ɏ��s���܂���\n" @*/ )) ;
					goto ENDLABEL ;
				}

				KeyFactorSet->Type = MV1_ANIMKEY_TYPE_LINEAR ;
				KeyFactorSet->DataType = MV1_ANIMKEY_DATATYPE_SHAPE ;
				KeyFactorSet->TimeType = MV1_ANIMKEY_TIME_TYPE_KEY ;
				KeyFactorSet->TotalTime = ( float )VmdFaceKeySet->MaxFrame ;
				KeyFactorSet->TargetShapeIndex = j ;
				KeyFactorSet->Num = ( int )VmdFaceKeySet->KeyNum ;

				KeyFactorSet->KeyTime = ( float * )ADDMEMAREA( sizeof( float ) * KeyFactorSet->Num, &RModel->Mem ) ;
				if( KeyFactorSet->KeyTime == NULL )
				{
					DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x44\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xa2\x30\xcb\x30\xe1\x30\xfc\x30\xb7\x30\xe7\x30\xf3\x30\xad\x30\xfc\x30\xbf\x30\xa4\x30\xe0\x30\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMD Load Error : �A�j���[�V�����L�[�^�C�����i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
					goto ENDLABEL ;
				}
				KeyFactorSet->KeyLinear = ( float * )ADDMEMAREA( sizeof( float ) * KeyFactorSet->Num, &RModel->Mem ) ;
				if( KeyFactorSet->KeyLinear == NULL )
				{
					DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x44\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xa2\x30\xcb\x30\xe1\x30\xfc\x30\xb7\x30\xe7\x30\xf3\x30\xad\x30\xfc\x30\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMD Load Error : �A�j���[�V�����L�[���i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
					goto ENDLABEL ;
				}

				RModel->AnimKeyDataSize += KeyFactorSet->Num * sizeof( float ) * 2 ;

				// �L�[�����Z�b�g
				KeyFactor = KeyFactorSet->KeyLinear ;
				KeyFactorTime = KeyFactorSet->KeyTime ;
				VmdFaceKey = VmdFaceKeySet->FirstKey ;
				for( j = 0 ; ( DWORD )j < VmdFaceKeySet->KeyNum ; j ++, VmdFaceKey = VmdFaceKey->Next, KeyFactor ++, KeyFactorTime ++ )
				{
					*KeyFactorTime = ( float )VmdFaceKey->Frame ;
					*KeyFactor = VmdFaceKey->Factor ;
				}
			}
		}
	}

	// ���I�Ɋm�ۂ����������̉��
	TerminateVMDBaseData( &VmdData ) ;

#ifndef DX_NON_BULLET_PHYSICS
	if( MLPhysicsInfo )
	{
		ReleasePhysicsObject_PMDPhysicsInfo( MLPhysicsInfo ) ;

		BoneInfo = PmdBoneInfo ;
		for( i = 0 ; i < PmdBoneNum ; i ++, BoneInfo ++ )
		{
			if( BoneInfo->KeyMatrix )
			{
				DXFREE( BoneInfo->KeyMatrix ) ;
				BoneInfo->KeyMatrix = NULL ;
			}

			if( BoneInfo->KeyMatrix2 )
			{
				DXFREE( BoneInfo->KeyMatrix2 ) ;
				BoneInfo->KeyMatrix2 = NULL ;
			}
		}
	}
#endif

	BoneInfo = PmdBoneInfo ;
	for( i = 0 ; i < PmdBoneNum ; i ++, BoneInfo ++ )
	{
		if( BoneInfo->KeyMatrix2 )
		{
			DXFREE( BoneInfo->KeyMatrix2 ) ;
			BoneInfo->KeyMatrix2 = NULL ;
		}
	}

	// ����I��
	return 0 ;

ENDLABEL :

	// ���I�Ɋm�ۂ����������̉��
	TerminateVMDBaseData( &VmdData ) ;

#ifndef DX_NON_BULLET_PHYSICS
	if( MLPhysicsInfo )
	{
		ReleasePhysicsObject_PMDPhysicsInfo( MLPhysicsInfo ) ;

		BoneInfo = PmdBoneInfo ;
		for( i = 0 ; i < PmdBoneNum ; i ++, BoneInfo ++ )
		{
			if( BoneInfo->KeyMatrix )
			{
				DXFREE( BoneInfo->KeyMatrix ) ;
				BoneInfo->KeyMatrix = NULL ;
			}
		}
	}
#endif

	BoneInfo = PmdBoneInfo ;
	for( i = 0 ; i < PmdBoneNum ; i ++, BoneInfo ++ )
	{
		if( BoneInfo->KeyMatrix2 )
		{
			DXFREE( BoneInfo->KeyMatrix2 ) ;
			BoneInfo->KeyMatrix2 = NULL ;
		}
	}

	// �G���[
	return -1 ;
}

// �u�l�c�t�@�C����ǂݍ���( -1:�G���[  0�ȏ�:���f���n���h�� )�A�����_�ł̓J��������ǂݍ��ނ���
extern int MV1LoadModelToVMD( const MV1_MODEL_LOAD_PARAM *LoadParam, int ASyncThread )
{
	MV1_MODEL_R RModel ;
	int NewHandle = -1 ;
	int ErrorFlag = 1 ;
	int Result ;

	// �ǂݍ��ݗp�f�[�^�̏�����
	MV1InitReadModel( &RModel ) ;
	RModel.MeshFaceRightHand = FALSE ;
	RModel.TranslateIsBackCulling = TRUE ;
	
	// ���f�����ƃt�@�C�������Z�b�g
	RModel.FilePath = ( wchar_t * )DXALLOC( ( _WCSLEN( LoadParam->FilePath ) + 1 ) * sizeof( wchar_t ) ) ;
	RModel.Name     = ( wchar_t * )DXALLOC( ( _WCSLEN( LoadParam->Name     ) + 1 ) * sizeof( wchar_t ) ) ;
	_WCSCPY( RModel.FilePath, LoadParam->FilePath ) ;
	_WCSCPY( RModel.Name,     LoadParam->Name ) ;

	// �@��̎��������͎g�p���Ȃ�
	RModel.AutoCreateNormal = FALSE ;

	// �A�j���f�[�^�̕��������Ȃ�
	RModel.AnimDataNotDecomposition = TRUE ;

	// �u�l�c�t�@�C���̓ǂݍ���
	Result = _MV1LoadModelToVMD_PMD(
		0,
		&RModel,
		LoadParam->DataBuffer,
		LoadParam->DataSize,
		"Anim000",
		FALSE,
		FALSE,
		NULL,
		0,
		NULL,
#ifndef DX_NON_BULLET_PHYSICS
		NULL,
#endif
		false
	) ;
	if( Result != 0 )
		goto ENDLABEL ;

	// ���f����f�[�^�n���h���̍쐬
	NewHandle = MV1LoadModelToReadModel( &LoadParam->GParam, &RModel, LoadParam->CurrentDir, LoadParam->FileReadFunc, ASyncThread ) ;
	if( NewHandle < 0 ) goto ENDLABEL ;

	// �G���[�t���O��|��
	ErrorFlag = 0 ;

ENDLABEL :

	// �G���[�t���O�������Ă����烂�f���n���h�������
	if( ErrorFlag == 1 && NewHandle != -1 )
	{
		MV1SubModelBase( NewHandle ) ;
		NewHandle = -1 ;
	}

	// �ǂݍ��ݗp���f�������
	MV1TermReadModel( &RModel ) ; 

	// �n���h����Ԃ�
	return NewHandle ;
}

#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

#endif


