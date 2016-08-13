// -------------------------------------------------------------------------------
// 
// 		�c�w���C�u����		�o�l�w���f���f�[�^�ǂݍ��݃v���O����
// 
// 				Ver 3.16d
// 
// -------------------------------------------------------------------------------

/*

�{�\�[�X���쐬����ۂɎQ�l�ɂ����Ă����������v���O�����\�[�X

PMX�d�l
PMX�G�f�B�^�t�� PMX�d�l.txt

IK�����̃v���O�����\�[�X
���M�l
<< ikx.zip >>

*/

#define __DX_MAKE

#include "DxModelLoader4.h"

#ifndef DX_NON_MODEL

// �C���N���[�h ---------------------------------
#include "DxModelLoaderVMD.h"
#include "DxFile.h"
#include "DxLog.h"
#include "DxMemory.h"
#include "DxSystem.h"

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// �}�N����` -----------------------------------

// �f�[�^�錾 -----------------------------------

static int           WCHAR_T_StringSetup = 0 ;
static const char *  CenterString_UTF16LE = "\xbb\x30\xf3\x30\xbf\x30\xfc\x30\x00"/*@ L"�Z���^�[" @*/ ;
static wchar_t       CenterString_WCHAR_T[ 8 ] ;

// �֐��錾 -------------------------------------

// �u�l�c�t�@�C����ǂݍ���( -1:�G���[ )
static int _MV1LoadModelToVMD_PMX(
	int								DataIndex,
	MV1_MODEL_R *					RModel,
	void *							DataBuffer,
	int								DataSize,
	const wchar_t *					Name,
	int								LoopMotionFlag,
	int								DisablePhysicsFlag,
	PMX_READ_BONE_INFO *			PmxBoneInfo,
	int								PmxBoneNum,
	PMX_READ_IK_INFO *				PmxIKInfoFirst,
#ifndef DX_NON_BULLET_PHYSICS
	DX_MODELLOADER3_PMX_PHYSICS_INFO *	MLPhysicsInfo,
#endif
	bool							FPS60
) ;

// �e�L�X�g�f�[�^��wchar_t�ɕϊ����Ď擾
__inline void MV1LoadModelToPMX_GetString( BYTE **Src, wchar_t *DestBuffer, BYTE EncodeType )
{
	char Buffer[ 1024 ] ;
	wchar_t WBuffer[ 1024 ] ;
	DWORD TextSize ;

	TextSize = *( ( DWORD * )*Src ) ;
	*Src = *Src + 4 ;

	if( EncodeType == 0 )
	{
		// UTF16

		_MEMCPY( WBuffer, *Src, TextSize ) ;
		( ( BYTE * )WBuffer )[ TextSize     ] = 0 ;
		( ( BYTE * )WBuffer )[ TextSize + 1 ] = 0 ;
		*Src = *Src + TextSize ;

		// UTF16�� wchar_t �ɕϊ�
		ConvString( ( const char * )WBuffer, DX_CHARCODEFORMAT_UTF16LE, ( char * )DestBuffer, BUFFERBYTES_CANCEL, WCHAR_T_CHARCODEFORMAT ) ;
	}
	else
	if( EncodeType == 1 )
	{
		// UTF8
		_MEMCPY( Buffer, *Src, TextSize ) ;
		Buffer[ TextSize ] = '\0' ;
		*Src = *Src + TextSize ;

		// UTF-8�� wchar_t �ɕϊ�
		ConvString( ( const char * )Buffer, DX_CHARCODEFORMAT_UTF8, ( char * )DestBuffer, BUFFERBYTES_CANCEL, WCHAR_T_CHARCODEFORMAT ) ;
	}
}

// �����l�T�C�Y�ɍ��킹�������l���擾����(��������)
__inline int MV1LoadModelToPMX_GetInt( BYTE **Src, BYTE Size )
{
	int res = 0 ;

	switch( Size )
	{
	case 1 :
		res = ( char )( ( *Src )[ 0 ] ) ;
		*Src = *Src + 1 ;
		break ;

	case 2 :
		res = ( short )( ( ( WORD * )*Src )[ 0 ] ) ;
		*Src = *Src + 2 ;
		break ;

	case 4 :
		res = ( ( int * )*Src )[ 0 ] ;
		*Src = *Src + 4 ;
		break ;
	}

	return res ;
}

// �����l�T�C�Y�ɍ��킹�������l���擾����(32bit�ȊO�����Ȃ�)
__inline int MV1LoadModelToPMX_GetUInt( BYTE **Src, BYTE Size )
{
	int res = 0 ;

	switch( Size )
	{
	case 1 :
		res = ( int )( ( *Src )[ 0 ] ) ;
		*Src = *Src + 1 ;
		break ;

	case 2 :
		res = ( int )( ( ( WORD * )*Src )[ 0 ] ) ;
		*Src = *Src + 2 ;
		break ;

	case 4 :
		res = ( ( int * )*Src )[ 0 ] ;
		*Src = *Src + 4 ;
		break ;
	}

	return res ;
}

// �s����v�Z����
static void MV1LoadModelToPMX_SetupMatrix( PMX_READ_BONE_INFO *BoneInfo, int BoneNum, int UseInitParam, int IKSkip ) ;

// �h�j���v�Z����
static void MV1LoadModelToPMX_SetupIK( PMX_READ_BONE_INFO *BoneInfo, PMX_READ_IK_INFO *IKInfoFirst ) ;

// �w��̃{�[���ɃA�j���[�V�����̎w��L�[�̃p�����[�^�𔽉f������
static void MV1LoadModelToPMX_SetupOneBoneMatrixFormAnimKey( PMX_READ_BONE_INFO *BoneInfo, int Time, int LoopNo, int MaxTime, int ValidNextRate, float NextRate ) ;

// �v���O���� -----------------------------------

// �o�l�w�t�@�C����ǂݍ���( -1:�G���[  0�ȏ�:���f���n���h�� )
extern int MV1LoadModelToPMX( const MV1_MODEL_LOAD_PARAM *LoadParam, int ASyncThread )
{
	int NewHandle = -1 ;
	int ErrorFlag = 1 ;
	int i, j, k, weightcount, facecount ;
	PMX_VERTEX *PmxVertex ;
	DWORD PmxVertexNum ;
	PMX_FACE *PmxFace ;
	DWORD PmxTextureNum ;
	PMX_TEXTURE *PmxTexture ;
	DWORD PmxFaceNum ;
	PMX_MATERIAL *PmxMaterial ;
	PMX_READ_IK_INFO *IKInfoDim = NULL, *IKInfoFirst = NULL, *IKInfo ;
	PMX_READ_BONE_INFO *BoneInfoDim = NULL, *BoneInfo ;
	DWORD PmxMaterialNum ;
	DWORD PmxBoneNum = 0 ;
	PMX_BONE *PmxBone ;
	DWORD PmxMorphNum ;
	PMX_MORPH *PmxMorph ;
	DWORD PmxRigidbodyNum ;
	PMX_RIGIDBODY *PmxRigidbody ;
	DWORD PmxJointNum ;
	PMX_JOINT *PmxJoint = NULL ;
	DWORD PmxIKNum ;
	DWORD PmxSkinNum ;
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
	DWORD SkinVertNum, SkinBaseVertNum, SkinFaceNum, NextFaceNum, SkinMaterialNum ;
	wchar_t String[ 1024 ] ;
	BYTE *Src ;
	int ValidPhysics = FALSE ;
#ifndef DX_NON_BULLET_PHYSICS
	DX_MODELLOADER3_PMX_PHYSICS_INFO MLPhysicsInfo ;

	_MEMSET( &MLPhysicsInfo, 0, sizeof( MLPhysicsInfo ) ) ;
#endif
	PMX_BASEINFO PmxInfo ;
	BYTE AddHeadDataSize ;

	// ������̃Z�b�g�A�b�v
	if( WCHAR_T_StringSetup == 0 )
	{
		ConvString( CenterString_UTF16LE, DX_CHARCODEFORMAT_UTF16LE, ( char * )CenterString_WCHAR_T, sizeof( CenterString_WCHAR_T ), WCHAR_T_CHARCODEFORMAT ) ;
		WCHAR_T_StringSetup = 1 ;
	}

	// �ǂݍ��݂悤�f�[�^�̏�����
	MV1InitReadModel( &RModel ) ;
	RModel.MaterialNumberOrderDraw = TRUE ;
	RModel.MeshFaceRightHand = FALSE ;

	// Pmx���f���f�[�^�̏����Z�b�g
	Src = ( BYTE * )LoadParam->DataBuffer ;

	// PMX�t�@�C�����ǂ������m�F
	if( Src[ 0 ] != 'P' || Src[ 1 ] != 'M' || Src[ 2 ] != 'X' || Src[ 3 ] != ' ' )
		return -1 ;
	Src += 4 ;

	// Ver2.0 �����m�F
	if( Src[ 0 ] != 0x00 || Src[ 1 ] != 0x00 || Src[ 2 ] != 0x00 || Src[ 3 ] != 0x40 )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x58\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xd0\x30\xfc\x30\xb8\x30\xe7\x30\xf3\x30\x12\xff\x0e\xff\x10\xff\x67\x30\x6f\x30\x42\x30\x8a\x30\x7e\x30\x5b\x30\x93\x30\x0a\x00\x00"/*@ L"PMX Load Error : �o�[�W�����Q�D�O�ł͂���܂���\n" @*/ )) ;
		return -1 ;
	}
	Src += 4 ;

	// �ǉ����̃T�C�Y���擾
	AddHeadDataSize = Src[ 0 ] ;
	Src ++ ;

	// �ǉ������擾
	PmxInfo.EncodeType        = Src[ 0 ] ;
	PmxInfo.UVNum             = Src[ 1 ] ;
	PmxInfo.VertexIndexSize   = Src[ 2 ] ;
	PmxInfo.TextureIndexSize  = Src[ 3 ] ;
	PmxInfo.MaterialIndexSize = Src[ 4 ] ;
	PmxInfo.BoneIndexSize     = Src[ 5 ] ;
	PmxInfo.MorphIndexSize    = Src[ 6 ] ;
	PmxInfo.RigidIndexSize    = Src[ 7 ] ;
	Src += AddHeadDataSize ;

	// ���f�����X�L�b�v
	Src += *( ( DWORD * )Src ) + 4 ;
	Src += *( ( DWORD * )Src ) + 4 ;
	Src += *( ( DWORD * )Src ) + 4 ;
	Src += *( ( DWORD * )Src ) + 4 ;

	// ���f�����ƃt�@�C�����ƕ����R�[�h�`�����Z�b�g
	RModel.CharCodeFormat = DX_CHARCODEFORMAT_UTF16LE ;
	RModel.FilePath = ( wchar_t * )DXALLOC( ( _WCSLEN( LoadParam->FilePath ) + 1 ) * sizeof( wchar_t ) ) ;
	RModel.Name     = ( wchar_t * )DXALLOC( ( _WCSLEN( LoadParam->Name     ) + 1 ) * sizeof( wchar_t ) ) ;
	_WCSCPY( RModel.FilePath, LoadParam->FilePath ) ;
	_WCSCPY( RModel.Name,     LoadParam->Name ) ;

	// �@��̎��������͎g�p���Ȃ�
	RModel.AutoCreateNormal = FALSE ;

	// ���_�����擾
	PmxVertexNum = *( ( DWORD * )Src ) ;
	Src += 4 ;

	// ���_�f�[�^���i�[���郁�����̈�̊m��
	PmxVertex = ( PMX_VERTEX * )ADDMEMAREA( sizeof( PMX_VERTEX ) * PmxVertexNum, &RModel.Mem ) ;
	if( PmxVertex == NULL )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x58\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\x02\x98\xb9\x70\xc7\x30\xfc\x30\xbf\x30\x6e\x30\x00\x4e\x42\x66\x18\x8a\xb6\x61\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMX Load Error : ���_�f�[�^�̈ꎞ�L���̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
		goto ENDLABEL ;
	}

	// ���_�f�[�^���擾
	for( i = 0 ; ( DWORD )i < PmxVertexNum ; i ++ )
	{
		*( ( DWORD * )&PmxVertex[ i ].Position[ 0 ] ) = *( ( DWORD * )&Src[ 0 ] ) ;
		*( ( DWORD * )&PmxVertex[ i ].Position[ 1 ] ) = *( ( DWORD * )&Src[ 4 ] ) ;
		*( ( DWORD * )&PmxVertex[ i ].Position[ 2 ] ) = *( ( DWORD * )&Src[ 8 ] ) ;
		*( ( DWORD * )&PmxVertex[ i ].Normal[ 0 ] ) = *( ( DWORD * )&Src[ 12 ] ) ;
		*( ( DWORD * )&PmxVertex[ i ].Normal[ 1 ] ) = *( ( DWORD * )&Src[ 16 ] ) ;
		*( ( DWORD * )&PmxVertex[ i ].Normal[ 2 ] ) = *( ( DWORD * )&Src[ 20 ] ) ;
		*( ( DWORD * )&PmxVertex[ i ].UV[ 0 ] ) = *( ( DWORD * )&Src[ 24 ] ) ;
		*( ( DWORD * )&PmxVertex[ i ].UV[ 1 ] ) = *( ( DWORD * )&Src[ 28 ] ) ;

		Src += 32 ;
		for( j = 0 ; j < PmxInfo.UVNum ; j ++ )
		{
			*( ( DWORD * )&PmxVertex[ i ].AddUV[ j ][ 0 ] ) = *( ( DWORD * )&Src[  0 ] ) ;
			*( ( DWORD * )&PmxVertex[ i ].AddUV[ j ][ 0 ] ) = *( ( DWORD * )&Src[  4 ] ) ;
			*( ( DWORD * )&PmxVertex[ i ].AddUV[ j ][ 0 ] ) = *( ( DWORD * )&Src[  8 ] ) ;
			*( ( DWORD * )&PmxVertex[ i ].AddUV[ j ][ 0 ] ) = *( ( DWORD * )&Src[ 12 ] ) ;
			Src += 16 ;
		}

		PmxVertex[ i ].WeightType = Src[ 0 ] ;
		Src ++ ;

		PmxVertex[ i ].BoneIndex[ 0 ] = -1 ;
		PmxVertex[ i ].BoneIndex[ 1 ] = -1 ;
		PmxVertex[ i ].BoneIndex[ 2 ] = -1 ;
		PmxVertex[ i ].BoneIndex[ 3 ] = -1 ;
		switch( PmxVertex[ i ].WeightType )
		{
		case 0 :	// BDEF1
			PmxVertex[ i ].BoneIndex[ 0 ] = MV1LoadModelToPMX_GetInt( &Src, PmxInfo.BoneIndexSize ) ;
			PmxVertex[ i ].BoneWeight[ 0 ] = 1.0f ;
			break ;

		case 1 :	// BDEF2
			PmxVertex[ i ].BoneIndex[ 0 ] = MV1LoadModelToPMX_GetInt( &Src, PmxInfo.BoneIndexSize ) ;
			PmxVertex[ i ].BoneIndex[ 1 ] = MV1LoadModelToPMX_GetInt( &Src, PmxInfo.BoneIndexSize ) ;
			*( ( DWORD * )&PmxVertex[ i ].BoneWeight[ 0 ] ) = *( ( DWORD * )Src ) ;
			Src += 4 ;
			PmxVertex[ i ].BoneWeight[ 1 ] = 1.0f - PmxVertex[ i ].BoneWeight[ 0 ] ;
			break ;

		case 2 :	// BDEF4
			PmxVertex[ i ].BoneIndex[ 0 ] = MV1LoadModelToPMX_GetInt( &Src, PmxInfo.BoneIndexSize ) ;
			PmxVertex[ i ].BoneIndex[ 1 ] = MV1LoadModelToPMX_GetInt( &Src, PmxInfo.BoneIndexSize ) ;
			PmxVertex[ i ].BoneIndex[ 2 ] = MV1LoadModelToPMX_GetInt( &Src, PmxInfo.BoneIndexSize ) ;
			PmxVertex[ i ].BoneIndex[ 3 ] = MV1LoadModelToPMX_GetInt( &Src, PmxInfo.BoneIndexSize ) ;
			*( ( DWORD * )&PmxVertex[ i ].BoneWeight[ 0 ] ) = *( ( DWORD * )&Src[ 0 ] ) ;
			*( ( DWORD * )&PmxVertex[ i ].BoneWeight[ 1 ] ) = *( ( DWORD * )&Src[ 4 ] ) ;
			*( ( DWORD * )&PmxVertex[ i ].BoneWeight[ 2 ] ) = *( ( DWORD * )&Src[ 8 ] ) ;
			*( ( DWORD * )&PmxVertex[ i ].BoneWeight[ 3 ] ) = *( ( DWORD * )&Src[ 12 ] ) ;
			Src += 16 ;
			break ;

		case 3 :	// SDEF
			PmxVertex[ i ].BoneIndex[ 0 ] = MV1LoadModelToPMX_GetInt( &Src, PmxInfo.BoneIndexSize ) ;
			PmxVertex[ i ].BoneIndex[ 1 ] = MV1LoadModelToPMX_GetInt( &Src, PmxInfo.BoneIndexSize ) ;
			*( ( DWORD * )&PmxVertex[ i ].BoneWeight[ 0 ] ) = *( ( DWORD * )Src ) ;
			Src += 4 ;
			PmxVertex[ i ].BoneWeight[ 1 ] = 1.0f - PmxVertex[ i ].BoneWeight[ 0 ] ;
			*( ( DWORD * )&PmxVertex[ i ].SDEF_C[ 0 ] ) = *( ( DWORD * )&Src[ 0 ] ) ;
			*( ( DWORD * )&PmxVertex[ i ].SDEF_C[ 1 ] ) = *( ( DWORD * )&Src[ 4 ] ) ;
			*( ( DWORD * )&PmxVertex[ i ].SDEF_C[ 2 ] ) = *( ( DWORD * )&Src[ 8 ] ) ;
			*( ( DWORD * )&PmxVertex[ i ].SDEF_R0[ 0 ] ) = *( ( DWORD * )&Src[ 12 ] ) ;
			*( ( DWORD * )&PmxVertex[ i ].SDEF_R0[ 1 ] ) = *( ( DWORD * )&Src[ 16 ] ) ;
			*( ( DWORD * )&PmxVertex[ i ].SDEF_R0[ 2 ] ) = *( ( DWORD * )&Src[ 20 ] ) ;
			*( ( DWORD * )&PmxVertex[ i ].SDEF_R1[ 0 ] ) = *( ( DWORD * )&Src[ 24 ] ) ;
			*( ( DWORD * )&PmxVertex[ i ].SDEF_R1[ 1 ] ) = *( ( DWORD * )&Src[ 28 ] ) ;
			*( ( DWORD * )&PmxVertex[ i ].SDEF_R1[ 2 ] ) = *( ( DWORD * )&Src[ 32 ] ) ;
			Src += 36 ;
			break ;
		}

		*( ( DWORD * )&PmxVertex[ i ].ToonEdgeScale ) = *( ( DWORD * )&Src[ 0 ] ) ;
		Src += 4 ;
	}

	// �ʂ̐����擾
	PmxFaceNum = *( ( DWORD * )Src ) / 3 ;
	Src += 4 ;

	// �ʃf�[�^���i�[���郁�����̈�̊m��
	PmxFace = ( PMX_FACE * )ADDMEMAREA( sizeof( PMX_FACE ) * PmxFaceNum, &RModel.Mem ) ;
	if( PmxFace == NULL )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x58\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\x62\x97\xc7\x30\xfc\x30\xbf\x30\x6e\x30\x00\x4e\x42\x66\x18\x8a\xb6\x61\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMX Load Error : �ʃf�[�^�̈ꎞ�L���̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
		goto ENDLABEL ;
	}

	// �ʂ��\�����钸�_�C���f�b�N�X�̎擾
	switch( PmxInfo.VertexIndexSize )
	{
	case 1 :
		for( i = 0 ; ( DWORD )i < PmxFaceNum ; i ++ )
		{
			PmxFace[ i ].VertexIndex[ 0 ] = ( int )( Src[ 0 ] ) ;
			PmxFace[ i ].VertexIndex[ 1 ] = ( int )( Src[ 1 ] ) ;
			PmxFace[ i ].VertexIndex[ 2 ] = ( int )( Src[ 2 ] ) ;
			Src += 3 ;
		}
		break ;

	case 2 :
		for( i = 0 ; ( DWORD )i < PmxFaceNum ; i ++ )
		{
			PmxFace[ i ].VertexIndex[ 0 ] = ( int )( *( ( WORD * )&Src[ 0 ] ) ) ;
			PmxFace[ i ].VertexIndex[ 1 ] = ( int )( *( ( WORD * )&Src[ 2 ] ) ) ;
			PmxFace[ i ].VertexIndex[ 2 ] = ( int )( *( ( WORD * )&Src[ 4 ] ) ) ;
			Src += 6 ;
		}
		break ;

	case 4 :
		for( i = 0 ; ( DWORD )i < PmxFaceNum ; i ++ )
		{
			PmxFace[ i ].VertexIndex[ 0 ] = *( ( int * )&Src[ 0 ] ) ;
			PmxFace[ i ].VertexIndex[ 1 ] = *( ( int * )&Src[ 4 ] ) ;
			PmxFace[ i ].VertexIndex[ 2 ] = *( ( int * )&Src[ 8 ] ) ;
			Src += 12 ;
		}
		break ;
	}


	// �e�N�X�`���̐����擾
	PmxTextureNum = *( ( DWORD * )Src ) ;
	Src += 4 ;

	// �e�N�X�`���f�[�^���i�[���郁�����̈�̊m��
	PmxTexture = ( PMX_TEXTURE * )ADDMEMAREA( sizeof( PMX_TEXTURE ) * PmxTextureNum, &RModel.Mem ) ;
	if( PmxTexture == NULL )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x58\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\xc7\x30\xfc\x30\xbf\x30\x6e\x30\x00\x4e\x42\x66\x18\x8a\xb6\x61\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMX Load Error : �e�N�X�`���f�[�^�̈ꎞ�L���̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
		goto ENDLABEL ;
	}

	// �e�N�X�`���̏����擾
	for( i = 0 ; ( DWORD )i < PmxTextureNum ; i ++ )
	{
		MV1LoadModelToPMX_GetString( &Src, PmxTexture[ i ].TexturePath, PmxInfo.EncodeType ) ;
	}


	// �}�e���A���̐����擾
	PmxMaterialNum = *( ( DWORD * )Src ) ;
	Src += 4 ;

	// �}�e���A���f�[�^���i�[���郁�����̈�̊m��
	PmxMaterial = ( PMX_MATERIAL * )ADDMEMAREA( sizeof( PMX_MATERIAL ) * PmxMaterialNum, &RModel.Mem ) ;
	if( PmxMaterial == NULL )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x58\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xde\x30\xc6\x30\xea\x30\xa2\x30\xeb\x30\xc7\x30\xfc\x30\xbf\x30\x6e\x30\x00\x4e\x42\x66\x18\x8a\xb6\x61\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMX Load Error : �}�e���A���f�[�^�̈ꎞ�L���̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
		goto ENDLABEL ;
	}

	// �}�e���A���̓ǂ݂���
	for( i = 0 ; ( DWORD )i < PmxMaterialNum ; i ++ )
	{
		// �ގ����̎擾
		MV1LoadModelToPMX_GetString( &Src, PmxMaterial[ i ].Name, PmxInfo.EncodeType ) ;
		if( PmxMaterial[ i ].Name[ 0 ] == L'\0' )
		{
			MV1LoadModelToPMX_GetString( &Src, PmxMaterial[ i ].Name, PmxInfo.EncodeType ) ;
			if( PmxMaterial[ i ].Name[ 0 ] == L'\0' )
			{
				_SWNPRINTF( PmxMaterial[ i ].Name, sizeof( PmxMaterial[ i ].Name ) / 2, L"Mat_%d", i ) ;
			}
		}
		else
		{
			Src += *( ( DWORD * )Src ) + 4 ;
		}

		*( ( DWORD * )&PmxMaterial[ i ].Diffuse[ 0 ] ) = *( ( DWORD * )&Src[  0 ] ) ;
		*( ( DWORD * )&PmxMaterial[ i ].Diffuse[ 1 ] ) = *( ( DWORD * )&Src[  4 ] ) ;
		*( ( DWORD * )&PmxMaterial[ i ].Diffuse[ 2 ] ) = *( ( DWORD * )&Src[  8 ] ) ;
		*( ( DWORD * )&PmxMaterial[ i ].Diffuse[ 3 ] ) = *( ( DWORD * )&Src[ 12 ] ) ;
		Src += 16 ;

		*( ( DWORD * )&PmxMaterial[ i ].Specular[ 0 ] ) = *( ( DWORD * )&Src[ 0 ] ) ;
		*( ( DWORD * )&PmxMaterial[ i ].Specular[ 1 ] ) = *( ( DWORD * )&Src[ 4 ] ) ;
		*( ( DWORD * )&PmxMaterial[ i ].Specular[ 2 ] ) = *( ( DWORD * )&Src[ 8 ] ) ;
		Src += 12 ;

		*( ( DWORD * )&PmxMaterial[ i ].SpecularPower ) = *( ( DWORD * )&Src[ 0 ] ) ;
		Src += 4 ;

		*( ( DWORD * )&PmxMaterial[ i ].Ambient[ 0 ] ) = *( ( DWORD * )&Src[ 0 ] ) ;
		*( ( DWORD * )&PmxMaterial[ i ].Ambient[ 1 ] ) = *( ( DWORD * )&Src[ 4 ] ) ;
		*( ( DWORD * )&PmxMaterial[ i ].Ambient[ 2 ] ) = *( ( DWORD * )&Src[ 8 ] ) ;
		Src += 12 ;

		PmxMaterial[ i ].CullingOff     = ( BYTE )( ( *Src & 0x01 ) ? 1 : 0 ) ;
		PmxMaterial[ i ].GroundShadow   = ( BYTE )( ( *Src & 0x02 ) ? 1 : 0 ) ;
		PmxMaterial[ i ].SelfShadowMap  = ( BYTE )( ( *Src & 0x04 ) ? 1 : 0 ) ;
		PmxMaterial[ i ].SelfShadowDraw = ( BYTE )( ( *Src & 0x08 ) ? 1 : 0 ) ;
		PmxMaterial[ i ].EdgeDraw       = ( BYTE )( ( *Src & 0x10 ) ? 1 : 0 ) ;
		Src ++ ;

		*( ( DWORD * )&PmxMaterial[ i ].EdgeColor[ 0 ] ) = *( ( DWORD * )&Src[ 0 ] ) ;
		*( ( DWORD * )&PmxMaterial[ i ].EdgeColor[ 1 ] ) = *( ( DWORD * )&Src[ 4 ] ) ;
		*( ( DWORD * )&PmxMaterial[ i ].EdgeColor[ 2 ] ) = *( ( DWORD * )&Src[ 8 ] ) ;
		*( ( DWORD * )&PmxMaterial[ i ].EdgeColor[ 3 ] ) = *( ( DWORD * )&Src[ 12 ] ) ;
		Src += 16 ;

		*( ( DWORD * )&PmxMaterial[ i ].EdgeSize ) = *( ( DWORD * )Src ) ;
		Src += 4 ;

		PmxMaterial[ i ].TextureIndex = MV1LoadModelToPMX_GetInt( &Src, PmxInfo.TextureIndexSize ) ;
		PmxMaterial[ i ].SphereTextureIndex = MV1LoadModelToPMX_GetInt( &Src, PmxInfo.TextureIndexSize ) ;

		PmxMaterial[ i ].SphereMode = *Src ;
		Src ++ ;

		PmxMaterial[ i ].ToonFlag = *Src ;
		Src ++ ;
		if( PmxMaterial[ i ].ToonFlag == 0 )
		{
			PmxMaterial[ i ].ToonTextureIndex = MV1LoadModelToPMX_GetInt( &Src, PmxInfo.TextureIndexSize ) ;
		}
		else
		{
			PmxMaterial[ i ].ToonTextureIndex = *Src ;
			Src ++ ;
		}

		// �����̓X�L�b�v
		Src += *( ( DWORD * )Src ) + 4 ;

		PmxMaterial[ i ].MaterialFaceNum = *( ( int * )Src ) ;
		Src += 4 ;
	}


	// �{�[���̐����擾
	PmxBoneNum = *( ( DWORD * )Src ) ;
	Src += 4 ;

	// �{�[���f�[�^���i�[���郁�����̈�̊m��
	PmxBone = ( PMX_BONE * )ADDMEMAREA( sizeof( PMX_BONE ) * PmxBoneNum, &RModel.Mem ) ;
	if( PmxBone == NULL )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x58\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xdc\x30\xfc\x30\xf3\x30\xc7\x30\xfc\x30\xbf\x30\x6e\x30\x00\x4e\x42\x66\x18\x8a\xb6\x61\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMX Load Error : �{�[���f�[�^�̈ꎞ�L���̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
		goto ENDLABEL ;
	}

	// �{�[�����̎擾
	PmxIKNum = 0 ;
	for( i = 0 ; ( DWORD )i < PmxBoneNum ; i ++ )
	{
		MV1LoadModelToPMX_GetString( &Src, PmxBone[ i ].Name, PmxInfo.EncodeType ) ;
		if( PmxBone[ i ].Name[ 0 ] == '\0' ) 
		{
			MV1LoadModelToPMX_GetString( &Src, PmxBone[ i ].Name, PmxInfo.EncodeType ) ;
		}
		else
		{
			Src += *( ( DWORD * )Src ) + 4 ;
		}

		*( ( DWORD * )&PmxBone[ i ].Position[ 0 ] ) = *( ( DWORD * )&Src[ 0 ] ) ;
		*( ( DWORD * )&PmxBone[ i ].Position[ 1 ] ) = *( ( DWORD * )&Src[ 4 ] ) ;
		*( ( DWORD * )&PmxBone[ i ].Position[ 2 ] ) = *( ( DWORD * )&Src[ 8 ] ) ;
		Src += 12 ;

		PmxBone[ i ].ParentBoneIndex = MV1LoadModelToPMX_GetInt( &Src, PmxInfo.BoneIndexSize ) ;
		PmxBone[ i ].TransformLayer = *( ( int * )Src ) ;
		Src += 4 ;

		WORD Flag = *( ( WORD * )Src ) ;
		Src += 2 ;
		PmxBone[ i ].Flag_LinkDest              = ( BYTE )( ( Flag & 0x0001 ) != 0 ? 1 : 0 ) ;
		PmxBone[ i ].Flag_EnableRot             = ( BYTE )( ( Flag & 0x0002 ) != 0 ? 1 : 0 ) ;
		PmxBone[ i ].Flag_EnableMov             = ( BYTE )( ( Flag & 0x0004 ) != 0 ? 1 : 0 ) ;
		PmxBone[ i ].Flag_Disp                  = ( BYTE )( ( Flag & 0x0008 ) != 0 ? 1 : 0 ) ;
		PmxBone[ i ].Flag_EnableControl         = ( BYTE )( ( Flag & 0x0010 ) != 0 ? 1 : 0 ) ;
		PmxBone[ i ].Flag_IK                    = ( BYTE )( ( Flag & 0x0020 ) != 0 ? 1 : 0 ) ;
		PmxBone[ i ].Flag_AddRot                = ( BYTE )( ( Flag & 0x0100 ) != 0 ? 1 : 0 ) ;
		PmxBone[ i ].Flag_AddMov                = ( BYTE )( ( Flag & 0x0200 ) != 0 ? 1 : 0 ) ;
		PmxBone[ i ].Flag_LockAxis              = ( BYTE )( ( Flag & 0x0400 ) != 0 ? 1 : 0 ) ;
		PmxBone[ i ].Flag_LocalAxis             = ( BYTE )( ( Flag & 0x0800 ) != 0 ? 1 : 0 ) ;
		PmxBone[ i ].Flag_AfterPhysicsTransform = ( BYTE )( ( Flag & 0x1000 ) != 0 ? 1 : 0 ) ;
		PmxBone[ i ].Flag_OutParentTransform    = ( BYTE )( ( Flag & 0x2000 ) != 0 ? 1 : 0 ) ;

		if( PmxBone[ i ].Flag_LinkDest == 0 )
		{
			*( ( DWORD * )&PmxBone[ i ].OffsetPosition[ 0 ] ) = *( ( DWORD * )&Src[ 0 ] ) ;
			*( ( DWORD * )&PmxBone[ i ].OffsetPosition[ 1 ] ) = *( ( DWORD * )&Src[ 4 ] ) ;
			*( ( DWORD * )&PmxBone[ i ].OffsetPosition[ 2 ] ) = *( ( DWORD * )&Src[ 8 ] ) ;
			Src += 12 ;
		}
		else
		{
			PmxBone[ i ].LinkBoneIndex = MV1LoadModelToPMX_GetInt( &Src, PmxInfo.BoneIndexSize ) ;
		}

		if( PmxBone[ i ].Flag_AddRot == 1 || PmxBone[ i ].Flag_AddMov == 1 )
		{
			PmxBone[ i ].AddParentBoneIndex = MV1LoadModelToPMX_GetInt( &Src, PmxInfo.BoneIndexSize ) ;
			*( ( DWORD * )&PmxBone[ i ].AddRatio ) = *( ( DWORD * )Src ) ;
			Src += 4 ;
		}

		if( PmxBone[ i ].Flag_LockAxis == 1 )
		{
			*( ( DWORD * )&PmxBone[ i ].LockAxisVector[ 0 ] ) = *( ( DWORD * )&Src[ 0 ] ) ;
			*( ( DWORD * )&PmxBone[ i ].LockAxisVector[ 1 ] ) = *( ( DWORD * )&Src[ 4 ] ) ;
			*( ( DWORD * )&PmxBone[ i ].LockAxisVector[ 2 ] ) = *( ( DWORD * )&Src[ 8 ] ) ;
			Src += 12 ;
		}

		if( PmxBone[ i ].Flag_LocalAxis == 1 )
		{
			*( ( DWORD * )&PmxBone[ i ].LocalAxisXVector[ 0 ] ) = *( ( DWORD * )&Src[ 0 ] ) ;
			*( ( DWORD * )&PmxBone[ i ].LocalAxisXVector[ 1 ] ) = *( ( DWORD * )&Src[ 4 ] ) ;
			*( ( DWORD * )&PmxBone[ i ].LocalAxisXVector[ 2 ] ) = *( ( DWORD * )&Src[ 8 ] ) ;
			Src += 12 ;

			*( ( DWORD * )&PmxBone[ i ].LocalAxisZVector[ 0 ] ) = *( ( DWORD * )&Src[ 0 ] ) ;
			*( ( DWORD * )&PmxBone[ i ].LocalAxisZVector[ 1 ] ) = *( ( DWORD * )&Src[ 4 ] ) ;
			*( ( DWORD * )&PmxBone[ i ].LocalAxisZVector[ 2 ] ) = *( ( DWORD * )&Src[ 8 ] ) ;
			Src += 12 ;
		}

		if( PmxBone[ i ].Flag_OutParentTransform == 1 )
		{
			PmxBone[ i ].OutParentTransformKey = *( ( int * )Src ) ;
			Src += 4 ;
		}

		if( PmxBone[ i ].Flag_IK == 1 )
		{
			PmxIKNum ++ ;

			PmxBone[ i ].IKInfo.TargetBoneIndex = MV1LoadModelToPMX_GetInt( &Src, PmxInfo.BoneIndexSize ) ;
			PmxBone[ i ].IKInfo.LoopNum = *( ( int * )Src ) ;
			Src += 4 ;

			*( ( DWORD * )&PmxBone[ i ].IKInfo.RotLimit ) = *( ( DWORD * )Src ) ;
			Src += 4 ;

			PmxBone[ i ].IKInfo.LinkNum = *( ( int * )Src ) ;
			Src += 4 ;
			if( PmxBone[ i ].IKInfo.LinkNum >= PMX_MAX_IKLINKNUM )
			{
				DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x58\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\x29\xff\x2b\xff\xea\x30\xf3\x30\xaf\x30\x6e\x30\x70\x65\x4c\x30\xfe\x5b\xdc\x5f\x70\x65\x92\x30\x85\x8d\x48\x30\x66\x30\x44\x30\x7e\x30\x59\x30\x0a\x00\x00"/*@ L"PMX Load Error : �h�j�����N�̐����Ή����𒴂��Ă��܂�\n" @*/ )) ;
				goto ENDLABEL ;
			}

			for( j = 0 ; j < PmxBone[ i ].IKInfo.LinkNum ; j ++ )
			{
				PmxBone[ i ].IKInfo.Link[ j ].BoneIndex = MV1LoadModelToPMX_GetInt( &Src, PmxInfo.BoneIndexSize ) ;
				PmxBone[ i ].IKInfo.Link[ j ].RotLockFlag = Src[ 0 ] ;
				Src ++ ;

				if( PmxBone[ i ].IKInfo.Link[ j ].RotLockFlag == 1 )
				{
					*( ( DWORD * )&PmxBone[ i ].IKInfo.Link[ j ].RotLockMin[ 0 ] ) = *( ( DWORD * )&Src[ 0 ] ) ;
					*( ( DWORD * )&PmxBone[ i ].IKInfo.Link[ j ].RotLockMin[ 1 ] ) = *( ( DWORD * )&Src[ 4 ] ) ;
					*( ( DWORD * )&PmxBone[ i ].IKInfo.Link[ j ].RotLockMin[ 2 ] ) = *( ( DWORD * )&Src[ 8 ] ) ;
					Src += 12 ;

					*( ( DWORD * )&PmxBone[ i ].IKInfo.Link[ j ].RotLockMax[ 0 ] ) = *( ( DWORD * )&Src[ 0 ] ) ;
					*( ( DWORD * )&PmxBone[ i ].IKInfo.Link[ j ].RotLockMax[ 1 ] ) = *( ( DWORD * )&Src[ 4 ] ) ;
					*( ( DWORD * )&PmxBone[ i ].IKInfo.Link[ j ].RotLockMax[ 2 ] ) = *( ( DWORD * )&Src[ 8 ] ) ;
					Src += 12 ;
				}
			}
		}
	}


	// ���[�t���̐����擾
	PmxMorphNum = *( ( DWORD * )Src ) ;
	Src += 4 ;

	// ���[�t�f�[�^���i�[���郁�����̈�̊m��
	PmxMorph = ( PMX_MORPH * )ADDMEMAREA( sizeof( PMX_MORPH ) * PmxMorphNum, &RModel.Mem ) ;
	if( PmxMorph == NULL )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x58\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xe2\x30\xfc\x30\xd5\x30\xc7\x30\xfc\x30\xbf\x30\x6e\x30\x00\x4e\x42\x66\x18\x8a\xb6\x61\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMX Load Error : ���[�t�f�[�^�̈ꎞ�L���̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
		goto ENDLABEL ;
	}

	// ���[�t���̓ǂݍ���
	PmxSkinNum = 0 ;
	for( i = 0 ; ( DWORD )i < PmxMorphNum ; i ++ )
	{
		MV1LoadModelToPMX_GetString( &Src, PmxMorph[ i ].Name, PmxInfo.EncodeType ) ;
		if( PmxMorph[ i ].Name[ 0 ] == '\0' ) 
		{
			MV1LoadModelToPMX_GetString( &Src, PmxMorph[ i ].Name, PmxInfo.EncodeType ) ;
		}
		else
		{
			Src += *( ( DWORD * )Src ) + 4 ;
		}

		PmxMorph[ i ].ControlPanel = Src[ 0 ] ;
		PmxMorph[ i ].Type = Src[ 1 ] ;
		PmxMorph[ i ].DataNum = *( ( int * )&Src[ 2 ] ) ;
		Src += 6 ;

		switch( PmxMorph[ i ].Type )
		{
		case 0 :	// �O���[�v���[�t
			PmxMorph[ i ].Group = ( PMX_MORPH_GROUP * )ADDMEMAREA( sizeof( PMX_MORPH_GROUP ) * PmxMorph[ i ].DataNum, &RModel.Mem ) ;
			if( PmxMorph[ i ].Group == NULL )
			{
				DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x58\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xb0\x30\xeb\x30\xfc\x30\xd7\x30\xe2\x30\xfc\x30\xd5\x30\xc7\x30\xfc\x30\xbf\x30\x6e\x30\x00\x4e\x42\x66\x18\x8a\xb6\x61\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMX Load Error : �O���[�v���[�t�f�[�^�̈ꎞ�L���̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
				goto ENDLABEL ;
			}

			for( j = 0 ; j < PmxMorph[ i ].DataNum ; j ++ )
			{
				PmxMorph[ i ].Group[ j ].Index = MV1LoadModelToPMX_GetInt( &Src, PmxInfo.MorphIndexSize ) ;
				*( ( DWORD * )&PmxMorph[ i ].Group[ j ].Ratio ) = *( ( DWORD * )Src ) ;
				Src += 4 ;
			}
			break ;

		case 1 :	// ���_
			PmxSkinNum ++ ;
			PmxMorph[ i ].Vertex = ( PMX_MORPH_VERTEX * )ADDMEMAREA( sizeof( PMX_MORPH_VERTEX ) * PmxMorph[ i ].DataNum, &RModel.Mem ) ;
			if( PmxMorph[ i ].Vertex == NULL )
			{
				DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x58\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\x02\x98\xb9\x70\xe2\x30\xfc\x30\xd5\x30\xc7\x30\xfc\x30\xbf\x30\x6e\x30\x00\x4e\x42\x66\x18\x8a\xb6\x61\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMX Load Error : ���_���[�t�f�[�^�̈ꎞ�L���̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
				goto ENDLABEL ;
			}

			for( j = 0 ; j < PmxMorph[ i ].DataNum ; j ++ )
			{
				PmxMorph[ i ].Vertex[ j ].Index = MV1LoadModelToPMX_GetUInt( &Src, PmxInfo.VertexIndexSize ) ;
				*( ( DWORD * )&PmxMorph[ i ].Vertex[ j ].Offset[ 0 ] ) = *( ( DWORD * )&Src[ 0 ] ) ;
				*( ( DWORD * )&PmxMorph[ i ].Vertex[ j ].Offset[ 1 ] ) = *( ( DWORD * )&Src[ 4 ] ) ;
				*( ( DWORD * )&PmxMorph[ i ].Vertex[ j ].Offset[ 2 ] ) = *( ( DWORD * )&Src[ 8 ] ) ;
				Src += 12 ;
			}
			break ;

		case 2 :	// �{�[�����[�t
			PmxMorph[ i ].Bone = ( PMX_MORPH_BONE * )ADDMEMAREA( sizeof( PMX_MORPH_BONE ) * PmxMorph[ i ].DataNum, &RModel.Mem ) ;
			if( PmxMorph[ i ].Bone == NULL )
			{
				DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x58\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xdc\x30\xfc\x30\xf3\x30\xe2\x30\xfc\x30\xd5\x30\xc7\x30\xfc\x30\xbf\x30\x6e\x30\x00\x4e\x42\x66\x18\x8a\xb6\x61\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMX Load Error : �{�[�����[�t�f�[�^�̈ꎞ�L���̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
				goto ENDLABEL ;
			}

			for( j = 0 ; j < PmxMorph[ i ].DataNum ; j ++ )
			{
				PmxMorph[ i ].Bone[ j ].Index = MV1LoadModelToPMX_GetInt( &Src, PmxInfo.BoneIndexSize ) ;
				*( ( DWORD * )&PmxMorph[ i ].Bone[ j ].Offset[ 0 ] ) = *( ( DWORD * )&Src[ 0 ] ) ;
				*( ( DWORD * )&PmxMorph[ i ].Bone[ j ].Offset[ 1 ] ) = *( ( DWORD * )&Src[ 4 ] ) ;
				*( ( DWORD * )&PmxMorph[ i ].Bone[ j ].Offset[ 2 ] ) = *( ( DWORD * )&Src[ 8 ] ) ;
				Src += 12 ;

				*( ( DWORD * )&PmxMorph[ i ].Bone[ j ].Quat[ 0 ] ) = *( ( DWORD * )&Src[  0 ] ) ;
				*( ( DWORD * )&PmxMorph[ i ].Bone[ j ].Quat[ 1 ] ) = *( ( DWORD * )&Src[  4 ] ) ;
				*( ( DWORD * )&PmxMorph[ i ].Bone[ j ].Quat[ 2 ] ) = *( ( DWORD * )&Src[  8 ] ) ;
				*( ( DWORD * )&PmxMorph[ i ].Bone[ j ].Quat[ 3 ] ) = *( ( DWORD * )&Src[ 12 ] ) ;
				Src += 16 ;
			}
			break ;

		case 3 :	// UV���[�t
		case 4 :	// �ǉ�UV1���[�t
		case 5 :	// �ǉ�UV2���[�t
		case 6 :	// �ǉ�UV3���[�t
		case 7 :	// �ǉ�UV4���[�t
			PmxMorph[ i ].UV = ( PMX_MORPH_UV * )ADDMEMAREA( sizeof( PMX_MORPH_UV ) * PmxMorph[ i ].DataNum, &RModel.Mem ) ;
			if( PmxMorph[ i ].UV == NULL )
			{
				DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x58\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\x55\x00\x56\x00\xe2\x30\xfc\x30\xd5\x30\xc7\x30\xfc\x30\xbf\x30\x6e\x30\x00\x4e\x42\x66\x18\x8a\xb6\x61\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMX Load Error : UV���[�t�f�[�^�̈ꎞ�L���̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
				goto ENDLABEL ;
			}

			for( j = 0 ; j < PmxMorph[ i ].DataNum ; j ++ )
			{
				PmxMorph[ i ].UV[ j ].Index = MV1LoadModelToPMX_GetUInt( &Src, PmxInfo.VertexIndexSize ) ;
				*( ( DWORD * )&PmxMorph[ i ].UV[ j ].Offset.x ) = *( ( DWORD * )&Src[  0 ] ) ;
				*( ( DWORD * )&PmxMorph[ i ].UV[ j ].Offset.y ) = *( ( DWORD * )&Src[  4 ] ) ;
				*( ( DWORD * )&PmxMorph[ i ].UV[ j ].Offset.z ) = *( ( DWORD * )&Src[  8 ] ) ;
				*( ( DWORD * )&PmxMorph[ i ].UV[ j ].Offset.w ) = *( ( DWORD * )&Src[ 12 ] ) ;
				Src += 16 ;
			}
			break ;

		case 8 :	// �ގ����[�t
			PmxMorph[ i ].Material = ( PMX_MORPH_MATERIAL * )ADDMEMAREA( sizeof( PMX_MORPH_MATERIAL ) * PmxMorph[ i ].DataNum, &RModel.Mem ) ;
			if( PmxMorph[ i ].Material == NULL )
			{
				DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x58\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xde\x30\xc6\x30\xea\x30\xa2\x30\xeb\x30\xe2\x30\xfc\x30\xd5\x30\xc7\x30\xfc\x30\xbf\x30\x6e\x30\x00\x4e\x42\x66\x18\x8a\xb6\x61\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMX Load Error : �}�e���A�����[�t�f�[�^�̈ꎞ�L���̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
				goto ENDLABEL ;
			}

			for( j = 0 ; j < PmxMorph[ i ].DataNum ; j ++ )
			{
				PmxMorph[ i ].Material[ j ].Index = MV1LoadModelToPMX_GetInt( &Src, PmxInfo.MaterialIndexSize ) ;
				PmxMorph[ i ].Material[ j ].CalcType = Src[ 0 ] ;
				Src ++ ;

				*( ( DWORD * )&PmxMorph[ i ].Material[ j ].Diffuse[ 0 ] ) = *( ( DWORD * )&Src[  0 ] ) ;
				*( ( DWORD * )&PmxMorph[ i ].Material[ j ].Diffuse[ 1 ] ) = *( ( DWORD * )&Src[  4 ] ) ;
				*( ( DWORD * )&PmxMorph[ i ].Material[ j ].Diffuse[ 2 ] ) = *( ( DWORD * )&Src[  8 ] ) ;
				*( ( DWORD * )&PmxMorph[ i ].Material[ j ].Diffuse[ 3 ] ) = *( ( DWORD * )&Src[ 12 ] ) ;
				Src += 16 ;

				*( ( DWORD * )&PmxMorph[ i ].Material[ j ].Specular[ 0 ] ) = *( ( DWORD * )&Src[  0 ] ) ;
				*( ( DWORD * )&PmxMorph[ i ].Material[ j ].Specular[ 1 ] ) = *( ( DWORD * )&Src[  4 ] ) ;
				*( ( DWORD * )&PmxMorph[ i ].Material[ j ].Specular[ 2 ] ) = *( ( DWORD * )&Src[  8 ] ) ;
				Src += 12 ;

				*( ( DWORD * )&PmxMorph[ i ].Material[ j ].SpecularPower ) = *( ( DWORD * )&Src[  0 ] ) ;
				Src += 4 ;

				*( ( DWORD * )&PmxMorph[ i ].Material[ j ].Ambient[ 0 ] ) = *( ( DWORD * )&Src[  0 ] ) ;
				*( ( DWORD * )&PmxMorph[ i ].Material[ j ].Ambient[ 1 ] ) = *( ( DWORD * )&Src[  4 ] ) ;
				*( ( DWORD * )&PmxMorph[ i ].Material[ j ].Ambient[ 2 ] ) = *( ( DWORD * )&Src[  8 ] ) ;
				Src += 12 ;

				*( ( DWORD * )&PmxMorph[ i ].Material[ j ].EdgeColor[ 0 ] ) = *( ( DWORD * )&Src[  0 ] ) ;
				*( ( DWORD * )&PmxMorph[ i ].Material[ j ].EdgeColor[ 1 ] ) = *( ( DWORD * )&Src[  4 ] ) ;
				*( ( DWORD * )&PmxMorph[ i ].Material[ j ].EdgeColor[ 2 ] ) = *( ( DWORD * )&Src[  8 ] ) ;
				*( ( DWORD * )&PmxMorph[ i ].Material[ j ].EdgeColor[ 3 ] ) = *( ( DWORD * )&Src[ 12 ] ) ;
				Src += 16 ;

				*( ( DWORD * )&PmxMorph[ i ].Material[ j ].EdgeSize ) = *( ( DWORD * )&Src[  0 ] ) ;
				Src += 4 ;

				*( ( DWORD * )&PmxMorph[ i ].Material[ j ].TextureScale[ 0 ] ) = *( ( DWORD * )&Src[  0 ] ) ;
				*( ( DWORD * )&PmxMorph[ i ].Material[ j ].TextureScale[ 1 ] ) = *( ( DWORD * )&Src[  4 ] ) ;
				*( ( DWORD * )&PmxMorph[ i ].Material[ j ].TextureScale[ 2 ] ) = *( ( DWORD * )&Src[  8 ] ) ;
				*( ( DWORD * )&PmxMorph[ i ].Material[ j ].TextureScale[ 3 ] ) = *( ( DWORD * )&Src[ 12 ] ) ;
				Src += 16 ;

				*( ( DWORD * )&PmxMorph[ i ].Material[ j ].SphereTextureScale[ 0 ] ) = *( ( DWORD * )&Src[  0 ] ) ;
				*( ( DWORD * )&PmxMorph[ i ].Material[ j ].SphereTextureScale[ 1 ] ) = *( ( DWORD * )&Src[  4 ] ) ;
				*( ( DWORD * )&PmxMorph[ i ].Material[ j ].SphereTextureScale[ 2 ] ) = *( ( DWORD * )&Src[  8 ] ) ;
				*( ( DWORD * )&PmxMorph[ i ].Material[ j ].SphereTextureScale[ 3 ] ) = *( ( DWORD * )&Src[ 12 ] ) ;
				Src += 16 ;

				*( ( DWORD * )&PmxMorph[ i ].Material[ j ].ToonTextureScale[ 0 ] ) = *( ( DWORD * )&Src[  0 ] ) ;
				*( ( DWORD * )&PmxMorph[ i ].Material[ j ].ToonTextureScale[ 1 ] ) = *( ( DWORD * )&Src[  4 ] ) ;
				*( ( DWORD * )&PmxMorph[ i ].Material[ j ].ToonTextureScale[ 2 ] ) = *( ( DWORD * )&Src[  8 ] ) ;
				*( ( DWORD * )&PmxMorph[ i ].Material[ j ].ToonTextureScale[ 3 ] ) = *( ( DWORD * )&Src[ 12 ] ) ;
				Src += 16 ;
			}
			break ;
		}
	}


	// �\���g�f�[�^�̓ǂݍ��݂̓X�L�b�v
	{
		DWORD PmxDispFrameNum ;
		DWORD Num ;
		BYTE Target ;

		// �\���g�f�[�^�̐����擾
		PmxDispFrameNum = *( ( DWORD * )Src ) ;
		Src += 4 ;

		// �g�̐������J��Ԃ�
		for( i = 0 ; ( DWORD )i < PmxDispFrameNum ; i ++ )
		{
			// ���O�ǂݔ�΂�
			Src += *( ( DWORD * )Src ) + 4 ;
			Src += *( ( DWORD * )Src ) + 4 ;

			// ����g�t���O�ǂݔ�΂�
			Src ++ ;

			// �g���v�f�̐����擾
			Num = *( ( DWORD * )Src ) ;
			Src += 4 ;
			
			// �g���v�f�̓ǂݔ�΂�
			for( j = 0 ; ( DWORD )j < Num ; j ++ )
			{
				// �v�f�Ώۂ��擾
				Target = Src[ 0 ] ;
				Src ++ ;

				// �v�f�Ώۂɂ���ď����𕪊�
				switch( Target )
				{
				case 0 :	// �{�[�����^�[�Q�b�g�̏ꍇ
					MV1LoadModelToPMX_GetInt( &Src, PmxInfo.BoneIndexSize ) ;
					break ;

				case 1 :	// ���[�t���^�[�Q�b�g�̏ꍇ
					MV1LoadModelToPMX_GetInt( &Src, PmxInfo.MorphIndexSize ) ;
					break ;
				}
			}
		}
	}


	// ���̏��̐����擾
	PmxRigidbodyNum = *( ( DWORD * )Src ) ;
	Src += 4 ;

	// ���̃f�[�^���i�[���郁�����̈�̊m��
	PmxRigidbody = ( PMX_RIGIDBODY * )ADDMEMAREA( sizeof( PMX_RIGIDBODY ) * PmxRigidbodyNum, &RModel.Mem ) ;
	if( PmxRigidbody == NULL )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x58\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\x5b\x52\x53\x4f\xc7\x30\xfc\x30\xbf\x30\x6e\x30\x00\x4e\x42\x66\x18\x8a\xb6\x61\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMX Load Error : ���̃f�[�^�̈ꎞ�L���̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
		goto ENDLABEL ;
	}

	// ���̏��̓ǂݍ���
	for( i = 0 ; ( DWORD )i < PmxRigidbodyNum ; i++ )
	{
		MV1LoadModelToPMX_GetString( &Src, PmxRigidbody[ i ].Name, PmxInfo.EncodeType ) ;
		if( PmxRigidbody[ i ].Name[ 0 ] == '\0' ) 
		{
			MV1LoadModelToPMX_GetString( &Src, PmxRigidbody[ i ].Name, PmxInfo.EncodeType ) ;
		}
		else
		{
			Src += *( ( DWORD * )Src ) + 4 ;
		}

		PmxRigidbody[ i ].BoneIndex = MV1LoadModelToPMX_GetInt( &Src, PmxInfo.BoneIndexSize ) ;

		PmxRigidbody[ i ].RigidBodyGroupIndex = Src[ 0 ] ;
		Src ++ ;

		*( ( WORD * )&PmxRigidbody[ i ].RigidBodyGroupTarget ) = *( ( WORD * )&Src[ 0 ] ) ;
		Src += 2 ;

		PmxRigidbody[ i ].ShapeType = Src[ 0 ] ;
		Src ++ ;

		*( ( DWORD * )&PmxRigidbody[ i ].ShapeW ) = *( ( DWORD * )&Src[ 0 ] ) ;
		*( ( DWORD * )&PmxRigidbody[ i ].ShapeH ) = *( ( DWORD * )&Src[ 4 ] ) ;
		*( ( DWORD * )&PmxRigidbody[ i ].ShapeD ) = *( ( DWORD * )&Src[ 8 ] ) ;
		Src += 12 ;

		*( ( DWORD * )&PmxRigidbody[ i ].Position[ 0 ] ) = *( ( DWORD * )&Src[  0 ] ) ;
		*( ( DWORD * )&PmxRigidbody[ i ].Position[ 1 ] ) = *( ( DWORD * )&Src[  4 ] ) ;
		*( ( DWORD * )&PmxRigidbody[ i ].Position[ 2 ] ) = *( ( DWORD * )&Src[  8 ] ) ;
		Src += 12 ;

		*( ( DWORD * )&PmxRigidbody[ i ].Rotation[ 0 ] ) = *( ( DWORD * )&Src[  0 ] ) ;
		*( ( DWORD * )&PmxRigidbody[ i ].Rotation[ 1 ] ) = *( ( DWORD * )&Src[  4 ] ) ;
		*( ( DWORD * )&PmxRigidbody[ i ].Rotation[ 2 ] ) = *( ( DWORD * )&Src[  8 ] ) ;
		Src += 12 ;

		*( ( DWORD * )&PmxRigidbody[ i ].RigidBodyWeight ) = *( ( DWORD * )&Src[  0 ] ) ;
		*( ( DWORD * )&PmxRigidbody[ i ].RigidBodyPosDim ) = *( ( DWORD * )&Src[  4 ] ) ;
		*( ( DWORD * )&PmxRigidbody[ i ].RigidBodyRotDim ) = *( ( DWORD * )&Src[  8 ] ) ;
		*( ( DWORD * )&PmxRigidbody[ i ].RigidBodyRecoil ) = *( ( DWORD * )&Src[ 12 ] ) ;
		*( ( DWORD * )&PmxRigidbody[ i ].RigidBodyFriction ) = *( ( DWORD * )&Src[ 16 ] ) ;
		Src += 20 ;

		PmxRigidbody[ i ].RigidBodyType = Src[ 0 ] ;
		Src ++ ;
	}


	// �W���C���g���̐����擾
	PmxJointNum = *( ( DWORD * )Src ) ;
	Src += 4 ;

	// �W���C���g�f�[�^���i�[���郁�����̈�̊m��
	if( PmxJointNum != 0 )
	{
		PmxJoint = ( PMX_JOINT * )ADDMEMAREA( sizeof( PMX_JOINT ) * PmxJointNum, &RModel.Mem ) ;
		if( PmxJoint == NULL )
		{
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x58\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xb8\x30\xe7\x30\xa4\x30\xf3\x30\xc8\x30\xc7\x30\xfc\x30\xbf\x30\x6e\x30\x00\x4e\x42\x66\x18\x8a\xb6\x61\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMX Load Error : �W���C���g�f�[�^�̈ꎞ�L���̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
			goto ENDLABEL ;
		}

		// �W���C���g���̓ǂݍ���
		for( i = 0 ; ( DWORD )i < PmxJointNum ; i ++ )
		{
			MV1LoadModelToPMX_GetString( &Src, PmxJoint[ i ].Name, PmxInfo.EncodeType ) ;
			if( PmxJoint[ i ].Name[ 0 ] == '\0' ) 
			{
				MV1LoadModelToPMX_GetString( &Src, PmxJoint[ i ].Name, PmxInfo.EncodeType ) ;
			}
			else
			{
				Src += *( ( DWORD * )Src ) + 4 ;
			}

			PmxJoint[ i ].Type = Src[ 0 ] ;
			Src ++ ;

			PmxJoint[ i ].RigidBodyAIndex = MV1LoadModelToPMX_GetInt( &Src, PmxInfo.RigidIndexSize ) ;
			PmxJoint[ i ].RigidBodyBIndex = MV1LoadModelToPMX_GetInt( &Src, PmxInfo.RigidIndexSize ) ;

			*( ( DWORD * )&PmxJoint[ i ].Position[ 0 ] ) = *( ( DWORD * )&Src[  0 ] ) ;
			*( ( DWORD * )&PmxJoint[ i ].Position[ 1 ] ) = *( ( DWORD * )&Src[  4 ] ) ;
			*( ( DWORD * )&PmxJoint[ i ].Position[ 2 ] ) = *( ( DWORD * )&Src[  8 ] ) ;
			Src += 12 ;

			*( ( DWORD * )&PmxJoint[ i ].Rotation[ 0 ] ) = *( ( DWORD * )&Src[  0 ] ) ;
			*( ( DWORD * )&PmxJoint[ i ].Rotation[ 1 ] ) = *( ( DWORD * )&Src[  4 ] ) ;
			*( ( DWORD * )&PmxJoint[ i ].Rotation[ 2 ] ) = *( ( DWORD * )&Src[  8 ] ) ;
			Src += 12 ;

			*( ( DWORD * )&PmxJoint[ i ].ConstrainPositionMin[ 0 ] ) = *( ( DWORD * )&Src[  0 ] ) ;
			*( ( DWORD * )&PmxJoint[ i ].ConstrainPositionMin[ 1 ] ) = *( ( DWORD * )&Src[  4 ] ) ;
			*( ( DWORD * )&PmxJoint[ i ].ConstrainPositionMin[ 2 ] ) = *( ( DWORD * )&Src[  8 ] ) ;
			Src += 12 ;

			*( ( DWORD * )&PmxJoint[ i ].ConstrainPositionMax[ 0 ] ) = *( ( DWORD * )&Src[  0 ] ) ;
			*( ( DWORD * )&PmxJoint[ i ].ConstrainPositionMax[ 1 ] ) = *( ( DWORD * )&Src[  4 ] ) ;
			*( ( DWORD * )&PmxJoint[ i ].ConstrainPositionMax[ 2 ] ) = *( ( DWORD * )&Src[  8 ] ) ;
			Src += 12 ;

			*( ( DWORD * )&PmxJoint[ i ].ConstrainRotationMin[ 0 ] ) = *( ( DWORD * )&Src[  0 ] ) ;
			*( ( DWORD * )&PmxJoint[ i ].ConstrainRotationMin[ 1 ] ) = *( ( DWORD * )&Src[  4 ] ) ;
			*( ( DWORD * )&PmxJoint[ i ].ConstrainRotationMin[ 2 ] ) = *( ( DWORD * )&Src[  8 ] ) ;
			Src += 12 ;

			*( ( DWORD * )&PmxJoint[ i ].ConstrainRotationMax[ 0 ] ) = *( ( DWORD * )&Src[  0 ] ) ;
			*( ( DWORD * )&PmxJoint[ i ].ConstrainRotationMax[ 1 ] ) = *( ( DWORD * )&Src[  4 ] ) ;
			*( ( DWORD * )&PmxJoint[ i ].ConstrainRotationMax[ 2 ] ) = *( ( DWORD * )&Src[  8 ] ) ;
			Src += 12 ;

			*( ( DWORD * )&PmxJoint[ i ].SpringPosition[ 0 ] ) = *( ( DWORD * )&Src[  0 ] ) ;
			*( ( DWORD * )&PmxJoint[ i ].SpringPosition[ 1 ] ) = *( ( DWORD * )&Src[  4 ] ) ;
			*( ( DWORD * )&PmxJoint[ i ].SpringPosition[ 2 ] ) = *( ( DWORD * )&Src[  8 ] ) ;
			Src += 12 ;

			*( ( DWORD * )&PmxJoint[ i ].SpringRotation[ 0 ] ) = *( ( DWORD * )&Src[  0 ] ) ;
			*( ( DWORD * )&PmxJoint[ i ].SpringRotation[ 1 ] ) = *( ( DWORD * )&Src[  4 ] ) ;
			*( ( DWORD * )&PmxJoint[ i ].SpringRotation[ 2 ] ) = *( ( DWORD * )&Src[  8 ] ) ;
			Src += 12 ;
		}
	}





	// ���b�V�������߂�t���[���̒ǉ�
	Frame = MV1RAddFrameW( &RModel, L"Mesh", NULL ) ;
	if( Frame == NULL )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x58\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xd5\x30\xec\x30\xfc\x30\xe0\x30\xaa\x30\xd6\x30\xb8\x30\xa7\x30\xaf\x30\xc8\x30\x6e\x30\xfd\x8f\xa0\x52\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMX Load Error : �t���[���I�u�W�F�N�g�̒ǉ��Ɏ��s���܂���\n" @*/ )) ;
		goto ENDLABEL ;
	}

	// �ǂݍ��ݏ����p�̃{�[���f�[�^���i�[���郁�����̈�̊m��
	BoneInfoDim = ( PMX_READ_BONE_INFO * )DXALLOC( sizeof( PMX_READ_BONE_INFO ) * PmxBoneNum ) ;
	if( BoneInfoDim == NULL )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x58\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xad\x8a\x7f\x30\xbc\x8f\x7f\x30\xe6\x51\x06\x74\x28\x75\xdc\x30\xfc\x30\xf3\x30\xc5\x60\x31\x58\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMX Load Error : �ǂݍ��ݏ����p�{�[�������i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
		goto ENDLABEL ;
	}

	// �{�[���f�[�^�̒ǉ�
	_MEMSET( FrameDimEnable, 0, sizeof( FrameDimEnable ) ) ;
	_MEMSET( FrameDim, 0, sizeof( FrameDim ) ) ;
	do
	{
		_MEMSET( FrameSkipDim, 0, sizeof( FrameSkipDim ) ) ;
		BoneInfo = BoneInfoDim ;
		FrameSkipNum = 0 ;
		for( i = 0 ; ( DWORD )i < PmxBoneNum ; i ++, BoneInfo ++ )
		{
			if( PmxBone[ i ].ParentBoneIndex != -1 && FrameDimEnable[ PmxBone[ i ].ParentBoneIndex ] == 0 )
			{
				FrameSkipDim[ i ] = true ;
				FrameSkipNum ++ ;
				continue ;
			}
			if( FrameDimEnable[ i ] != 0 )
				continue ;
			FrameDimEnable[ i ] = 1 ;

			FrameDim[ i ] = MV1RAddFrameW( &RModel, PmxBone[ i ].Name, PmxBone[ i ].ParentBoneIndex == -1 ? NULL : FrameDim[ PmxBone[ i ].ParentBoneIndex ] ) ;
			if( FrameDim[ i ] == NULL )
			{
				DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x58\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xd5\x30\xec\x30\xfc\x30\xe0\x30\xaa\x30\xd6\x30\xb8\x30\xa7\x30\xaf\x30\xc8\x30\x6e\x30\xfd\x8f\xa0\x52\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMX Load Error : �t���[���I�u�W�F�N�g�̒ǉ��Ɏ��s���܂���\n" @*/ )) ;
				goto ENDLABEL ;
			}

			BoneInfo->OrgTranslate.x = PmxBone[ i ].Position[ 0 ] ;
			BoneInfo->OrgTranslate.y = PmxBone[ i ].Position[ 1 ] ;
			BoneInfo->OrgTranslate.z = PmxBone[ i ].Position[ 2 ] ;
			FrameDim[ i ]->TempVector.x = PmxBone[ i ].Position[ 0 ] ;
			FrameDim[ i ]->TempVector.y = PmxBone[ i ].Position[ 1 ] ;
			FrameDim[ i ]->TempVector.z = PmxBone[ i ].Position[ 2 ] ;
			if( PmxBone[ i ].ParentBoneIndex != -1 )
			{
				FrameDim[ i ]->Translate = VSub( FrameDim[ i ]->TempVector, FrameDim[ PmxBone[ i ].ParentBoneIndex ]->TempVector ) ;
			}
			else
			{
				FrameDim[ i ]->Translate = FrameDim[ i ]->TempVector ;
			}
			FrameDim[ i ]->UserData = BoneInfo ;

			BoneInfo->Base = &PmxBone[ i ] ;
			BoneInfo->Frame = FrameDim[ i ] ;
			BoneInfo->IsPhysics = FALSE ;
			BoneInfo->IsIK = FALSE ;
			BoneInfo->IsAddParent = PmxBone[ i ].Flag_AddMov == 1 || PmxBone[ i ].Flag_AddRot == 1 ? 1 : 0 ;
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
	MV1LoadModelToPMX_SetupMatrix( BoneInfoDim, ( int )PmxBoneNum, TRUE, FALSE ) ;


	// ���b�V����ǉ�
	{
		// ���b�V����ǉ�
		Mesh = MV1RAddMesh( &RModel, Frame ) ;
		if( Mesh == NULL )
		{
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x58\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xe1\x30\xc3\x30\xb7\x30\xe5\x30\xaa\x30\xd6\x30\xb8\x30\xa7\x30\xaf\x30\xc8\x30\x6e\x30\xfd\x8f\xa0\x52\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMX Load Error : ���b�V���I�u�W�F�N�g�̒ǉ��Ɏ��s���܂���\n" @*/ )) ;
			goto ENDLABEL ;
		}

		// �ʏ����i�[���郁�����̈�̊m��
		if( MV1RSetupMeshFaceBuffer( &RModel, Mesh, ( int )PmxFaceNum, 3 ) < 0 )
		{
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x58\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\x62\x97\xc5\x60\x31\x58\x92\x30\xdd\x4f\x58\x5b\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMX Load Error : �ʏ���ۑ����郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
			goto ENDLABEL ;
		}

		Mesh->PositionNum = PmxVertexNum ;
		Mesh->Positions = ( VECTOR * )ADDMEMAREA( ( sizeof( VECTOR ) + sizeof( float ) ) * Mesh->PositionNum, &RModel.Mem ) ;
		if( Mesh->Positions == NULL )
		{
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x58\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\x02\x98\xb9\x70\xa7\x5e\x19\x6a\x92\x30\xdd\x4f\x58\x5b\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMX Load Error : ���_���W��ۑ����郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
			goto ENDLABEL ;
		}
		Mesh->PositionToonOutLineScale = ( float * )( Mesh->Positions + Mesh->PositionNum ) ;

		Mesh->NormalNum = PmxVertexNum ;
		Mesh->Normals = ( VECTOR * )ADDMEMAREA( sizeof( VECTOR ) * Mesh->NormalNum, &RModel.Mem ) ;
		if( Mesh->Normals == NULL )
		{
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x58\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\x02\x98\xb9\x70\xd5\x6c\xda\x7d\x92\x30\xdd\x4f\x58\x5b\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMX Load Error : ���_�@����ۑ����郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
			goto ENDLABEL ;
		}

		Mesh->UVNum[ 0 ] = PmxVertexNum ;
		Mesh->UVs[ 0 ] = ( FLOAT4 * )ADDMEMAREA( sizeof( FLOAT4 ) * Mesh->UVNum[ 0 ], &RModel.Mem ) ;
		if( Mesh->UVs[ 0 ] == NULL )
		{
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x58\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\x02\x98\xb9\x70\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\xa7\x5e\x19\x6a\x92\x30\xdd\x4f\x58\x5b\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMX Load Error : ���_�e�N�X�`�����W��ۑ����郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
			goto ENDLABEL ;
		}
		for( i = 0 ; i < PmxInfo.UVNum; i ++ )
		{
			Mesh->UVNum[ i + 1 ] = PmxVertexNum ;
			Mesh->UVs[ i + 1 ] = ( FLOAT4 * )ADDMEMAREA( sizeof( FLOAT4 ) * Mesh->UVNum[ i + 1 ], &RModel.Mem ) ;
			if( Mesh->UVs[ i + 1 ] == NULL )
			{
				DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x58\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\x02\x98\xb9\x70\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\xa7\x5e\x19\x6a\x92\x30\xdd\x4f\x58\x5b\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMX Load Error : ���_�e�N�X�`�����W��ۑ����郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
				goto ENDLABEL ;
			}
		}

		// �{�[���̏���ǉ�
		Mesh->SkinWeightsNum = 0 ;
		for( i = 0 ; ( DWORD )i < PmxBoneNum ; i ++ )
		{
			// ���̃{�[�����g�p���Ă��钸�_�̐��𐔂���
			weightcount = 0 ;
			for( j = 0 ; ( DWORD )j < PmxVertexNum ; j ++ )
			{
				if( PmxVertex[ j ].BoneIndex[ 0 ] == i ||
					PmxVertex[ j ].BoneIndex[ 1 ] == i ||
					PmxVertex[ j ].BoneIndex[ 2 ] == i ||
					PmxVertex[ j ].BoneIndex[ 3 ] == i )
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
				DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x58\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xb9\x30\xad\x30\xcb\x30\xf3\x30\xb0\x30\xe1\x30\xc3\x30\xb7\x30\xe5\x30\xa6\x30\xa8\x30\xa4\x30\xc8\x30\x24\x50\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMX Load Error : �X�L�j���O���b�V���E�G�C�g�l���i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
				return -1 ;
			}

			// �E�G�C�g�̏����i�[����
			SkinW = SkinWeight->Data ;
			for( j = 0 ; ( DWORD )j < PmxVertexNum ; j ++ )
			{
				bool Use = false ;
				float Ratio = 0.0f ;

				if( PmxVertex[ j ].BoneIndex[ 0 ] == i )
				{
					Use = true ;
					Ratio += PmxVertex[ j ].BoneWeight[ 0 ] ;
				}

				if( PmxVertex[ j ].BoneIndex[ 1 ] == i )
				{
					Use = true ;
					Ratio += PmxVertex[ j ].BoneWeight[ 1 ] ;
				}

				if( PmxVertex[ j ].BoneIndex[ 2 ] == i )
				{
					Use = true ;
					Ratio += PmxVertex[ j ].BoneWeight[ 2 ] ;
				}

				if( PmxVertex[ j ].BoneIndex[ 3 ] == i )
				{
					Use = true ;
					Ratio += PmxVertex[ j ].BoneWeight[ 3 ] ;
				}

				if( Use )
				{
					SkinW->TargetVertex = ( DWORD )j ;
					SkinW->Weight       = Ratio ;
					SkinW ++ ;
				}
			}
		}

		// ���_�f�[�^���Z�b�g
		for( i = 0 ; ( DWORD )i < PmxVertexNum ; i ++ )
		{
			Mesh->Positions[ i ].x = PmxVertex[ i ].Position[ 0 ] ;
			Mesh->Positions[ i ].y = PmxVertex[ i ].Position[ 1 ] ;
			Mesh->Positions[ i ].z = PmxVertex[ i ].Position[ 2 ] ;
			Mesh->Normals[ i ].x = PmxVertex[ i ].Normal[ 0 ] ;
			Mesh->Normals[ i ].y = PmxVertex[ i ].Normal[ 1 ] ;
			Mesh->Normals[ i ].z = PmxVertex[ i ].Normal[ 2 ] ;
			Mesh->UVs[ 0 ][ i ].x = PmxVertex[ i ].UV[ 0 ] ;
			Mesh->UVs[ 0 ][ i ].y = PmxVertex[ i ].UV[ 1 ] ;

			for( j = 0 ; j < PmxInfo.UVNum ; j ++ )
			{
				Mesh->UVs[ j + 1 ][ i ].x = PmxVertex[ i ].AddUV[ j ][ 0 ] ;
				Mesh->UVs[ j + 1 ][ i ].y = PmxVertex[ i ].AddUV[ j ][ 1 ] ;
				Mesh->UVs[ j + 1 ][ i ].z = PmxVertex[ i ].AddUV[ j ][ 2 ] ;
				Mesh->UVs[ j + 1 ][ i ].w = PmxVertex[ i ].AddUV[ j ][ 3 ] ;
			}

			Mesh->PositionToonOutLineScale[ i ] = PmxVertex[ i ].ToonEdgeScale ;
		}

		// �ʃf�[�^���Z�b�g
		MeshFace = Mesh->Faces ;
		for( i = 0 ; ( DWORD )i < PmxFaceNum ; i ++ )
		{
			MeshFace->IndexNum = 3 ;
			MeshFace->VertexIndex[ 0 ] = ( DWORD )PmxFace[ i ].VertexIndex[ 0 ] ;
			MeshFace->VertexIndex[ 1 ] = ( DWORD )PmxFace[ i ].VertexIndex[ 1 ] ;
			MeshFace->VertexIndex[ 2 ] = ( DWORD )PmxFace[ i ].VertexIndex[ 2 ] ;
			MeshFace->NormalIndex[ 0 ] = ( DWORD )PmxFace[ i ].VertexIndex[ 0 ] ;
			MeshFace->NormalIndex[ 1 ] = ( DWORD )PmxFace[ i ].VertexIndex[ 1 ] ;
			MeshFace->NormalIndex[ 2 ] = ( DWORD )PmxFace[ i ].VertexIndex[ 2 ] ;
			MeshFace->UVIndex[ 0 ][ 0 ] = ( DWORD )PmxFace[ i ].VertexIndex[ 0 ] ;
			MeshFace->UVIndex[ 0 ][ 1 ] = ( DWORD )PmxFace[ i ].VertexIndex[ 1 ] ;
			MeshFace->UVIndex[ 0 ][ 2 ] = ( DWORD )PmxFace[ i ].VertexIndex[ 2 ] ;

			MeshFace ++ ;
		}
	}

	// �h�j�̏����i�[���郁�����̈�̊m��
	if( PmxIKNum )
	{
		IKInfoDim = ( PMX_READ_IK_INFO * )DXALLOC( sizeof( PMX_READ_IK_INFO ) * PmxIKNum ) ;
		if( IKInfoDim == NULL )
		{
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x58\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xad\x8a\x7f\x30\xbc\x8f\x7f\x30\xe6\x51\x06\x74\x28\x75\x29\xff\x2b\xff\xc5\x60\x31\x58\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMX Load Error : �ǂݍ��ݏ����p�h�j�����i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
			goto ENDLABEL ;
		}
	}

	// �h�j�f�[�^�̒ǉ�
	IKInfo = IKInfoDim ;
	IKInfoFirst = NULL ;
	i = 0 ;
	for( k = 0 ; ( DWORD )k < PmxBoneNum ; k ++ )
	{
		if( PmxBone[ k ].Flag_IK == 0 ) continue ;

		PMX_READ_BONE_INFO *BoneBone ;

		// �f�[�^���Z�b�g
		IKInfo->Base = &PmxBone[ k ].IKInfo ;
		IKInfo->Bone = &BoneInfoDim[ k ] ;
		IKInfo->TargetBone = &BoneInfoDim[ PmxBone[ k ].IKInfo.TargetBoneIndex ] ;
		IKInfo->TargetBone->IsIK = TRUE ;

		for( j = 0 ; j < IKInfo->Base->LinkNum ; j ++ )
		{
			BoneBone = &BoneInfoDim[ IKInfo->Base->Link[ j ].BoneIndex ] ;
			BoneInfoDim[ IKInfo->Base->Link[ j ].BoneIndex ].IsIK = TRUE ;
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
			PMX_READ_IK_INFO *IKInfoTemp ;

			for( IKInfoTemp = IKInfoFirst ; IKInfoTemp->Next != NULL && IKInfoTemp->Base->Link[ 0 ].BoneIndex < IKInfo->Base->Link[ 0 ].BoneIndex ; IKInfoTemp = IKInfoTemp->Next ){}
			if( IKInfoTemp->Next == NULL && IKInfoTemp->Base->Link[ 0 ].BoneIndex < IKInfo->Base->Link[ 0 ].BoneIndex )
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

		IKInfo ++ ;
	}

	// �h�j�̉e�����󂯂�{�[���̎q�łh�j�̉e�����󂯂Ȃ��{�[���Ɉ������
	BoneInfo = BoneInfoDim ;
	for( i = 0 ; ( DWORD )i < PmxBoneNum ; i ++, BoneInfo ++ )
	{
		PMX_READ_BONE_INFO *ParentBone ;

		if( BoneInfo->IsIK || BoneInfo->IsAddParent )
			continue ;

		if( BoneInfo->Frame->Parent == NULL )
			continue ;

		ParentBone = ( PMX_READ_BONE_INFO * )BoneInfo->Frame->Parent->UserData ;
		if( ParentBone->IsIK || ParentBone->IsAddParent )
		{
			BoneInfo->IsIKChild = TRUE ;
		}
	}

#ifndef DX_NON_BULLET_PHYSICS
	// �������Z�f�[�^�̓ǂݍ���
	if( LoadParam->GParam.LoadModelToUsePhysicsMode != DX_LOADMODEL_PHYSICS_DISABLE )
	{
		// �������Z�f�[�^������ꍇ�̂ݏ���������
		if( PmxRigidbodyNum > 0 && PmxJointNum > 0 )
		{
			PMX_READ_PHYSICS_INFO *PhysicsInfo ;
			PMX_READ_PHYSICS_JOINT_INFO *JointInfo ;

			// �����ɂ����畨�����Z�p�f�[�^������Ƃ�������
			ValidPhysics = TRUE ;

			// �������Z���̓ǂݍ���

			// �[��������
			_MEMSET( &MLPhysicsInfo, 0, sizeof( DX_MODELLOADER3_PMX_PHYSICS_INFO ) ) ;

			// �������Z���̐����擾
			MLPhysicsInfo.PmxPhysicsNum = ( int )PmxRigidbodyNum ;

			// �f�[�^���i�[���郁�����̈�̊m��
			MLPhysicsInfo.PmxPhysicsInfoDim = ( PMX_READ_PHYSICS_INFO * )DXALLOC( sizeof( PMX_READ_PHYSICS_INFO ) * MLPhysicsInfo.PmxPhysicsNum ) ;
			if( MLPhysicsInfo.PmxPhysicsInfoDim == NULL )
			{
				DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x58\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\x69\x72\x06\x74\x14\x6f\x97\x7b\xc5\x60\x31\x58\x4d\x91\x17\x52\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMX Load Error : �������Z���z����i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
				goto ENDLABEL ;
			}
			_MEMSET( MLPhysicsInfo.PmxPhysicsInfoDim, 0, sizeof( PMX_READ_PHYSICS_INFO ) * MLPhysicsInfo.PmxPhysicsNum ) ;

			// �f�[�^��ǂݏo��
			PhysicsInfo = MLPhysicsInfo.PmxPhysicsInfoDim ;
			for( i = 0 ; i < MLPhysicsInfo.PmxPhysicsNum ; i ++, PhysicsInfo ++ )
			{
				PhysicsInfo->Base = &PmxRigidbody[ i ] ;

				if( PhysicsInfo->Base->BoneIndex == -1 )
				{
					for( j = 0 ; _WCSCMP( BoneInfoDim[ j ].Frame->NameW, CenterString_WCHAR_T ) != 0 ; j ++ ){}
					PhysicsInfo->Bone = &BoneInfoDim[ j ] ;
				}
				else
				{
					PhysicsInfo->Bone = &BoneInfoDim[ PhysicsInfo->Base->BoneIndex ] ;
				}
				PhysicsInfo->Bone->IsPhysics = 1 ;
				PhysicsInfo->Bone->PhysicsIndex = i ;
				PhysicsInfo->Bone->SetupPhysicsAnim = 0 ;

				// ���̃^�C�v��ۑ�
				PhysicsInfo->NoCopyToBone = _WCSCMP( PhysicsInfo->Bone->Frame->NameW, CenterString_WCHAR_T ) == 0 ;
			}

			// �W���C���g���̐����擾
			MLPhysicsInfo.PmxPhysicsJointNum = ( int )PmxJointNum ;

			// �f�[�^���i�[���郁�����̈�̊m��
			MLPhysicsInfo.PmxPhysicsJointInfoDim = ( PMX_READ_PHYSICS_JOINT_INFO * )DXALLOC( sizeof( PMX_READ_PHYSICS_JOINT_INFO ) * MLPhysicsInfo.PmxPhysicsJointNum ) ;
			if( MLPhysicsInfo.PmxPhysicsJointInfoDim == NULL )
			{
				DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x58\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\x69\x72\x06\x74\x14\x6f\x97\x7b\xb8\x30\xe7\x30\xa4\x30\xf3\x30\xc8\x30\xc5\x60\x31\x58\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMX Load Error : �������Z�W���C���g�����i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
				goto ENDLABEL ;
			}
			_MEMSET( MLPhysicsInfo.PmxPhysicsJointInfoDim, 0, sizeof( PMX_READ_PHYSICS_JOINT_INFO ) * MLPhysicsInfo.PmxPhysicsJointNum ) ;

			// �f�[�^��ǂݏo��
			JointInfo = MLPhysicsInfo.PmxPhysicsJointInfoDim ;
			for( i = 0 ; i < MLPhysicsInfo.PmxPhysicsJointNum ; i ++, JointInfo ++ )
			{
				JointInfo->Base = &PmxJoint[ i ] ;
			}


			// �������A���^�C�������������s���ꍇ�͓ǂݍ��ݏ��ɕ����{�[���ƕ����W���C���g�̏���������
			if( LoadParam->GParam.LoadModelToUsePhysicsMode == DX_LOADMODEL_PHYSICS_REALTIME )
			{
				MV1_PHYSICS_RIGIDBODY_R *RigidBody ;
				MV1_PHYSICS_JOINT_R *Joint ;

				PhysicsInfo = MLPhysicsInfo.PmxPhysicsInfoDim ;
				for( i = 0 ; i < MLPhysicsInfo.PmxPhysicsNum ; i ++, PhysicsInfo ++ )
				{
					RigidBody = MV1RAddPhysicsRididBodyW( &RModel, PhysicsInfo->Base->Name, PhysicsInfo->Bone->Frame ) ;
					if( RigidBody == NULL )
					{
						DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x58\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\x69\x72\x06\x74\x08\x8a\x97\x7b\x28\x75\x5b\x52\x53\x4f\xc5\x60\x31\x58\x6e\x30\xfd\x8f\xa0\x52\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMX Load Error : �����v�Z�p���̏��̒ǉ��Ɏ��s���܂���\n" @*/ )) ;
						goto ENDLABEL ;
					}

					RigidBody->RigidBodyGroupIndex = PhysicsInfo->Base->RigidBodyGroupIndex ;
					RigidBody->RigidBodyGroupTarget = PhysicsInfo->Base->RigidBodyGroupTarget ;
					RigidBody->ShapeType = PhysicsInfo->Base->ShapeType ;
					RigidBody->ShapeW = PhysicsInfo->Base->ShapeW ;
					RigidBody->ShapeH = PhysicsInfo->Base->ShapeH ;
					RigidBody->ShapeD = PhysicsInfo->Base->ShapeD ;
					RigidBody->Position.x = PhysicsInfo->Base->Position[ 0 ] - PhysicsInfo->Bone->LocalWorldMatrix.m[ 3 ][ 0 ] ;
					RigidBody->Position.y = PhysicsInfo->Base->Position[ 1 ] - PhysicsInfo->Bone->LocalWorldMatrix.m[ 3 ][ 1 ] ;
					RigidBody->Position.z = PhysicsInfo->Base->Position[ 2 ] - PhysicsInfo->Bone->LocalWorldMatrix.m[ 3 ][ 2 ] ;
					RigidBody->Rotation.x = PhysicsInfo->Base->Rotation[ 0 ] ;
					RigidBody->Rotation.y = PhysicsInfo->Base->Rotation[ 1 ] ;
					RigidBody->Rotation.z = PhysicsInfo->Base->Rotation[ 2 ] ;
					RigidBody->RigidBodyWeight = PhysicsInfo->Base->RigidBodyWeight ;
					RigidBody->RigidBodyPosDim = PhysicsInfo->Base->RigidBodyPosDim ;
					RigidBody->RigidBodyRotDim = PhysicsInfo->Base->RigidBodyRotDim ;
					RigidBody->RigidBodyRecoil = PhysicsInfo->Base->RigidBodyRecoil ;
					RigidBody->RigidBodyFriction = PhysicsInfo->Base->RigidBodyFriction ;
					RigidBody->RigidBodyType = PhysicsInfo->Base->RigidBodyType ;
					RigidBody->NoCopyToBone = PhysicsInfo->NoCopyToBone ? TRUE : FALSE ;
				}

				JointInfo = MLPhysicsInfo.PmxPhysicsJointInfoDim ;
				for( i = 0 ; i < MLPhysicsInfo.PmxPhysicsJointNum ; i ++, JointInfo ++ )
				{
					Joint = MV1RAddPhysicsJointW( &RModel, JointInfo->Base->Name ) ;
					if( Joint == NULL )
					{
						DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x58\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\x69\x72\x06\x74\x08\x8a\x97\x7b\x28\x75\x5b\x52\x53\x4f\xa5\x63\x08\x54\xc5\x60\x31\x58\x6e\x30\xfd\x8f\xa0\x52\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMX Load Error : �����v�Z�p���̐ڍ����̒ǉ��Ɏ��s���܂���\n" @*/ )) ;
						goto ENDLABEL ;
					}

					Joint->RigidBodyA = ( DWORD )JointInfo->Base->RigidBodyAIndex ;
					Joint->RigidBodyB = ( DWORD )JointInfo->Base->RigidBodyBIndex ;
					Joint->Position.x = JointInfo->Base->Position[ 0 ] ;
					Joint->Position.y = JointInfo->Base->Position[ 1 ] ;
					Joint->Position.z = JointInfo->Base->Position[ 2 ] ;
					Joint->Rotation.x = JointInfo->Base->Rotation[ 0 ] ;
					Joint->Rotation.y = JointInfo->Base->Rotation[ 1 ] ;
					Joint->Rotation.z = JointInfo->Base->Rotation[ 2 ] ;
					Joint->ConstrainPosition1.x = JointInfo->Base->ConstrainPositionMin[ 0 ] ;
					Joint->ConstrainPosition1.y = JointInfo->Base->ConstrainPositionMin[ 1 ] ;
					Joint->ConstrainPosition1.z = JointInfo->Base->ConstrainPositionMin[ 2 ] ;
					Joint->ConstrainPosition2.x = JointInfo->Base->ConstrainPositionMax[ 0 ] ;
					Joint->ConstrainPosition2.y = JointInfo->Base->ConstrainPositionMax[ 1 ] ;
					Joint->ConstrainPosition2.z = JointInfo->Base->ConstrainPositionMax[ 2 ] ;
					Joint->ConstrainRotation1.x = JointInfo->Base->ConstrainRotationMin[ 0 ] ;
					Joint->ConstrainRotation1.y = JointInfo->Base->ConstrainRotationMin[ 1 ] ;
					Joint->ConstrainRotation1.z = JointInfo->Base->ConstrainRotationMin[ 2 ] ;
					Joint->ConstrainRotation2.x = JointInfo->Base->ConstrainRotationMax[ 0 ] ;
					Joint->ConstrainRotation2.y = JointInfo->Base->ConstrainRotationMax[ 1 ] ;
					Joint->ConstrainRotation2.z = JointInfo->Base->ConstrainRotationMax[ 2 ] ;
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
#endif

	// �}�e���A���̓ǂ݂���
	facecount = 0 ;
	for( i = 0 ; ( DWORD )i < PmxMaterialNum ; i ++ )
	{
		Material = MV1RAddMaterialW( &RModel, PmxMaterial[ i ].Name ) ;
		if( Material == NULL )
		{
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x58\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\x4e\x00\x6f\x00\x2e\x00\x25\x00\x64\x00\x20\x00\x6e\x30\x20\x00\x4d\x00\x61\x00\x74\x00\x65\x00\x72\x00\x69\x00\x61\x00\x6c\x00\x20\x00\xaa\x30\xd6\x30\xb8\x30\xa7\x30\xaf\x30\xc8\x30\x6e\x30\xfd\x8f\xa0\x52\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMX Load Error : No.%d �� Material �I�u�W�F�N�g�̒ǉ��Ɏ��s���܂���\n" @*/, i ) ) ;
			goto ENDLABEL ;
		}

		// �}�e���A���^�C�v�̓g�D�[��
		Material->Type = DX_MATERIAL_TYPE_TOON_2 ;

		// �����Z�b�g
		Material->Diffuse.r = PmxMaterial[ i ].Diffuse[ 0 ] ;
		Material->Diffuse.g = PmxMaterial[ i ].Diffuse[ 1 ] ;
		Material->Diffuse.b = PmxMaterial[ i ].Diffuse[ 2 ] ;
		Material->Diffuse.a = PmxMaterial[ i ].Diffuse[ 3 ] ;

		Material->Specular.r = PmxMaterial[ i ].Specular[ 0 ] ;
		Material->Specular.g = PmxMaterial[ i ].Specular[ 1 ] ;
		Material->Specular.b = PmxMaterial[ i ].Specular[ 2 ] ;
		Material->Specular.a = 0.0f ;

		Material->Power = PmxMaterial[ i ].SpecularPower ;

		Material->Ambient.r = PmxMaterial[ i ].Ambient[ 0 ] ;
		Material->Ambient.g = PmxMaterial[ i ].Ambient[ 1 ] ;
		Material->Ambient.b = PmxMaterial[ i ].Ambient[ 2 ] ;
		Material->Ambient.a = 0.0f ;

		Material->OutLineColor.r = PmxMaterial[ i ].EdgeColor[ 0 ] ;
		Material->OutLineColor.g = PmxMaterial[ i ].EdgeColor[ 1 ] ;
		Material->OutLineColor.b = PmxMaterial[ i ].EdgeColor[ 2 ] ;
		Material->OutLineColor.a = PmxMaterial[ i ].EdgeColor[ 3 ] ;

		Material->DisableBackCulling = PmxMaterial[ i ].CullingOff ;

		// �֊s���̑������Z�b�g
		if( PmxMaterial[ i ].EdgeDraw == 1 )
		{
			Material->OutLineDotWidth = PmxMaterial[ i ].EdgeSize ;
			Material->OutLineWidth = PmxMaterial[ i ].EdgeSize / 100.0f ;
		}

		// �g�D�[���e�N�X�`����ǉ�
		if( PmxMaterial[ i ].ToonTextureIndex >= 0 )
		{
			// �X�t�B�A�}�b�v�̃t�@�C�������܂܂��ꍇ�͂��������
			if( PmxMaterial[ i ].ToonFlag == 0 )
			{
				_WCSCPY_S( String, sizeof( String ), PmxTexture[ PmxMaterial[ i ].ToonTextureIndex ].TexturePath ) ;
			}
			else
			{
				_SWNPRINTF( String, sizeof( String ) / 2, L"toon%02d.bmp", PmxMaterial[ i ].ToonTextureIndex + 1 ) ;
			}
			Texture = MV1RAddTextureW( &RModel, String, String, NULL, FALSE, 0.1f, true, true, true ) ;
			if( Texture == NULL )
			{
				DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x58\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\x4e\x00\x6f\x00\x2e\x00\x25\x00\x64\x00\x20\x00\x6e\x30\x20\x00\xc8\x30\xa5\x30\xfc\x30\xf3\x30\x28\x75\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\xaa\x30\xd6\x30\xb8\x30\xa7\x30\xaf\x30\xc8\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMX Load Error : No.%d �� �g�D�[���p�e�N�X�`���I�u�W�F�N�g�̍쐬�Ɏ��s���܂���\n" @*/, i ) ) ;
				goto ENDLABEL ;
			}
			Texture->AddressModeU = DX_TEXADDRESS_CLAMP ;
			Texture->AddressModeV = DX_TEXADDRESS_CLAMP ;
			Material->DiffuseGradTexture = Texture ;
			Material->DiffuseGradBlendType = MV1_LAYERBLEND_TYPE_MODULATE ;
		}
		Material->SpecularGradTextureDefault = -1 ;
		Material->SpecularGradBlendType = MV1_LAYERBLEND_TYPE_ADDITIVE ;

		// �ʃf�[�^�Ƀ}�e���A���ԍ����Z�b�g����
		for( j = 0 ; j < PmxMaterial[ i ].MaterialFaceNum ; j += 3, facecount ++ )
		{
			Mesh->Faces[ facecount ].MaterialIndex = Mesh->MaterialNum ;
		}

		// �e�N�X�`����ǉ�
		if( PmxMaterial[ i ].TextureIndex >= 0 &&
			PmxTexture[ PmxMaterial[ i ].TextureIndex ].TexturePath[ 0 ] != '\0' )
		{
			Texture = MV1RAddTextureW( &RModel, PmxTexture[ PmxMaterial[ i ].TextureIndex ].TexturePath, PmxTexture[ PmxMaterial[ i ].TextureIndex ].TexturePath, NULL, false, 0.1f, true, false, true ) ;
			if( Texture == NULL )
			{
				DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x58\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\x4e\x00\x6f\x00\x2e\x00\x25\x00\x64\x00\x20\x00\x6e\x30\x20\x00\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\xaa\x30\xd6\x30\xb8\x30\xa7\x30\xaf\x30\xc8\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMX Load Error : No.%d �� �e�N�X�`���I�u�W�F�N�g�̍쐬�Ɏ��s���܂���\n" @*/, i ) ) ;
				goto ENDLABEL ;
			}
			Material->DiffuseTexNum = 1 ;
			Material->DiffuseTexs[ 0 ] = Texture ;
		}

		// �X�t�B�A�}�b�v��ǉ�
		if( PmxMaterial[ i ].SphereTextureIndex >= 0 &&
			PmxTexture[ PmxMaterial[ i ].SphereTextureIndex ].TexturePath[ 0 ] != '\0' )
		{
			Texture = MV1RAddTextureW( &RModel, PmxTexture[ PmxMaterial[ i ].SphereTextureIndex ].TexturePath, PmxTexture[ PmxMaterial[ i ].SphereTextureIndex ].TexturePath, NULL, false, 0.1f, true, false, true ) ;
			if( Texture == NULL )
			{
				DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x58\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\x4e\x00\x6f\x00\x2e\x00\x25\x00\x64\x00\x20\x00\x6e\x30\x20\x00\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\xaa\x30\xd6\x30\xb8\x30\xa7\x30\xaf\x30\xc8\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMX Load Error : No.%d �� �e�N�X�`���I�u�W�F�N�g�̍쐬�Ɏ��s���܂���\n" @*/, i ) ) ;
				goto ENDLABEL ;
			}

			Material->SphereMapTexture = Texture ;
			switch( PmxMaterial[ i ].SphereMode )
			{
			default :
				Material->SphereMapBlendType = DX_MATERIAL_BLENDTYPE_NONE ;
				break ;

			case 1 :
				Material->SphereMapBlendType = DX_MATERIAL_BLENDTYPE_MODULATE ;
				break ;

			case 2 :
				Material->SphereMapBlendType = DX_MATERIAL_BLENDTYPE_ADDITIVE ;
				break ;
			}
		}

		// �}�e���A�����P�|���S���ɂł��g�p����Ă���ꍇ�̓}�e���A���̃A�h���X���Z�b�g
		if( PmxMaterial[ i ].MaterialFaceNum != 0 )
		{
			Mesh->Materials[ Mesh->MaterialNum ] = Material ;
			Mesh->MaterialNum ++ ;
		}
	}

	// �\��f�[�^�̒ǉ�
	if( PmxSkinNum > 0 )
	{
		SkinMesh = MV1RAddMesh( &RModel, Frame ) ;
		if( SkinMesh == NULL )
		{
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x58\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xb9\x30\xad\x30\xf3\x30\xe1\x30\xc3\x30\xb7\x30\xe5\x30\xaa\x30\xd6\x30\xb8\x30\xa7\x30\xaf\x30\xc8\x30\x6e\x30\xfd\x8f\xa0\x52\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMX Load Error : �X�L�����b�V���I�u�W�F�N�g�̒ǉ��Ɏ��s���܂���\n" @*/ )) ;
			goto ENDLABEL ;
		}

		// base ���b�V�����猳���f������g�p���钸�_�Ɩʂ̏�������o��
		SkinNextVertIndex = ( DWORD * )DXALLOC( sizeof( DWORD ) * ( 2 * PmxVertexNum + 2 * PmxFaceNum + 2 * PmxMaterialNum ) ) ;
		if( SkinNextVertIndex == NULL )
		{
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x58\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\x00\x4e\x42\x66\xdd\x4f\x58\x5b\x28\x75\x6e\x30\x02\x98\xb9\x70\xc7\x30\xfc\x30\xbf\x30\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMX Load Error : �ꎞ�ۑ��p�̒��_�f�[�^���i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
			goto ENDLABEL ;
		}
		SkinPrevVertIndex     = SkinNextVertIndex     + PmxVertexNum ;
		SkinNextFaceIndex     = SkinPrevVertIndex     + PmxVertexNum ;
		SkinPrevFaceIndex     = SkinNextFaceIndex     + PmxFaceNum ;
		SkinNextMaterialIndex = SkinPrevFaceIndex     + PmxFaceNum ;
		SkinPrevMaterialIndex = SkinNextMaterialIndex + PmxMaterialNum ;
		_MEMSET( SkinNextVertIndex,     0xff, sizeof( DWORD ) * PmxVertexNum   ) ;
		_MEMSET( SkinPrevVertIndex,     0xff, sizeof( DWORD ) * PmxVertexNum   ) ;
		_MEMSET( SkinNextFaceIndex,     0xff, sizeof( DWORD ) * PmxFaceNum     ) ;
		_MEMSET( SkinPrevFaceIndex,     0xff, sizeof( DWORD ) * PmxFaceNum     ) ;
		_MEMSET( SkinNextMaterialIndex, 0xff, sizeof( DWORD ) * PmxMaterialNum ) ;
		_MEMSET( SkinPrevMaterialIndex, 0xff, sizeof( DWORD ) * PmxMaterialNum ) ;

		// �\��f�[�^�{�̂Ŏg�p���钸�_�̌��̃��b�V���ł̒��_�ԍ����擾
		SkinVertNum = 0 ;
		for( i = 0 ; ( DWORD )i < PmxMorphNum ; i ++ )
		{
			if( PmxMorph[ i ].Type != 1 ) continue ;

			for( j = 0 ; j < PmxMorph[ i ].DataNum ; j ++ )
			{
				for( k = 0 ; ( DWORD )k < SkinVertNum && SkinPrevVertIndex[ k ] != ( DWORD )PmxMorph[ i ].Vertex[ j ].Index ; k ++ ){}
				if( ( DWORD )k == SkinVertNum )
				{
					SkinPrevVertIndex[ SkinVertNum ] = ( DWORD )PmxMorph[ i ].Vertex[ j ].Index ;
					SkinNextVertIndex[ PmxMorph[ i ].Vertex[ j ].Index ] = SkinVertNum ;
					SkinVertNum ++ ;
				}
			}
		}

		// �\��f�[�^�Ŏg�p����Ă��钸�_���g���Ă���ʂ̐��𐔂���
		MeshFace = Mesh->Faces ;
		SkinFaceNum = 0 ;
		SkinMaterialNum = 0 ;
		SkinBaseVertNum = SkinVertNum ;
		for( i = 0 ; ( DWORD )i < PmxFaceNum ; i ++, MeshFace ++ )
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

		// �\��f�[�^�Ŏg�p����ʂ�ۑ����郁�����̈���m�ۂ���
		if( MV1RSetupMeshFaceBuffer( &RModel, SkinMesh, ( int )SkinFaceNum, 3 ) < 0 )
		{
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x58\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\x68\x88\xc5\x60\xc7\x30\xfc\x30\xbf\x30\x6e\x30\x62\x97\xc5\x60\x31\x58\x92\x30\xdd\x4f\x58\x5b\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMX Load Error : �\��f�[�^�̖ʏ���ۑ����郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
			goto ENDLABEL ;
		}

		// �\��f�[�^�Ŏg�p���钸�_��ۑ����郁�����̈���m�ۂ���
		SkinMesh->PositionNum = SkinVertNum ;
		SkinMesh->Positions = ( VECTOR * )ADDMEMAREA( ( sizeof( VECTOR ) + sizeof( float ) ) * SkinMesh->PositionNum, &RModel.Mem ) ;
		if( SkinMesh->Positions == NULL )
		{
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x58\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\x68\x88\xc5\x60\xc7\x30\xfc\x30\xbf\x30\x6e\x30\x02\x98\xb9\x70\xa7\x5e\x19\x6a\x92\x30\xdd\x4f\x58\x5b\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMX Load Error : �\��f�[�^�̒��_���W��ۑ����郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
			goto ENDLABEL ;
		}
		SkinMesh->PositionToonOutLineScale = ( float * )( SkinMesh->Positions + SkinMesh->PositionNum ) ;

		SkinMesh->NormalNum = SkinVertNum ;
		SkinMesh->Normals = ( VECTOR * )ADDMEMAREA( sizeof( VECTOR ) * SkinMesh->NormalNum, &RModel.Mem ) ;
		if( SkinMesh->Normals == NULL )
		{
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x58\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\x68\x88\xc5\x60\xc7\x30\xfc\x30\xbf\x30\x6e\x30\x02\x98\xb9\x70\xd5\x6c\xda\x7d\x92\x30\xdd\x4f\x58\x5b\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMX Load Error : �\��f�[�^�̒��_�@����ۑ����郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
			goto ENDLABEL ;
		}

		SkinMesh->UVNum[ 0 ] = SkinVertNum ;
		SkinMesh->UVs[ 0 ] = ( FLOAT4 * )ADDMEMAREA( sizeof( FLOAT4 ) * SkinMesh->UVNum[ 0 ], &RModel.Mem ) ;
		if( SkinMesh->UVs[ 0 ] == NULL )
		{
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x58\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\x68\x88\xc5\x60\xc7\x30\xfc\x30\xbf\x30\x6e\x30\x02\x98\xb9\x70\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\xa7\x5e\x19\x6a\x92\x30\xdd\x4f\x58\x5b\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMX Load Error : �\��f�[�^�̒��_�e�N�X�`�����W��ۑ����郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
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
				DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x58\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\x68\x88\xc5\x60\xc7\x30\xfc\x30\xbf\x30\x28\x75\xb9\x30\xad\x30\xcb\x30\xf3\x30\xb0\x30\xe1\x30\xc3\x30\xb7\x30\xe5\x30\xa6\x30\xa8\x30\xa4\x30\xc8\x30\x24\x50\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMX Load Error : �\��f�[�^�p�X�L�j���O���b�V���E�G�C�g�l���i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
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

		// �\��f�[�^��ǉ�����
		for( i = 0 ; ( DWORD )i < PmxMorphNum ; i ++ )
		{
			if( PmxMorph[ i ].Type != 1 ) continue ;

			// �\��f�[�^�̒ǉ�
			Shape = MV1RAddShapeW( &RModel, PmxMorph[ i ].Name, Frame ) ; 
			if( Shape == NULL )
			{
				DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x58\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xe2\x30\xfc\x30\xd5\x30\xaa\x30\xd6\x30\xb8\x30\xa7\x30\xaf\x30\xc8\x30\x6e\x30\xfd\x8f\xa0\x52\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMX Load Error : ���[�t�I�u�W�F�N�g�̒ǉ��Ɏ��s���܂���\n" @*/ )) ;
				goto ENDLABEL ;
			}

			// �Ώۃ��b�V���̃Z�b�g
			Shape->TargetMesh = SkinMesh ;

			// �@���͖���
			Shape->ValidVertexNormal = FALSE ;

			// ���_�̐���ۑ�
			Shape->VertexNum = PmxMorph[ i ].DataNum ;

			// ���_�f�[�^���i�[���郁�����̈�̊m��
			Shape->Vertex = ( MV1_SHAPE_VERTEX_R * )ADDMEMAREA( sizeof( MV1_SHAPE_VERTEX_R ) * Shape->VertexNum, &RModel.Mem ) ;
			if( Shape->Vertex == NULL )
			{
				DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x58\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xe2\x30\xfc\x30\xd5\x30\x02\x98\xb9\x70\xc7\x30\xfc\x30\xbf\x30\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMX Load Error : ���[�t���_�f�[�^���i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
				goto ENDLABEL ;
			}

			// ���_�f�[�^�𖄂߂�
			ShapeVert = Shape->Vertex ;
			for( j = 0 ; j < Shape->VertexNum ; j ++, ShapeVert ++ )
			{
				ShapeVert->TargetPositionIndex = ( int )SkinNextVertIndex[ PmxMorph[ i ].Vertex[ j ].Index ] ;
				ShapeVert->Position.x = PmxMorph[ i ].Vertex[ j ].Offset[ 0 ] ;
				ShapeVert->Position.y = PmxMorph[ i ].Vertex[ j ].Offset[ 1 ] ;
				ShapeVert->Position.z = PmxMorph[ i ].Vertex[ j ].Offset[ 2 ] ;
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
				MV1LoadModelToPMX_SetupMatrix( BoneInfoDim, ( int )PmxBoneNum, TRUE, FALSE ) ;

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
				_SWNPRINTF( String, sizeof( String ) / 2, L"Anim%03d", k ) ;
				Result = _MV1LoadModelToVMD_PMX(
					k,
					&RModel,
					VmdData,
					FileSize,
					String,
					LoopMotionFlag,
					DisablePhysicsFlag,
					BoneInfoDim,
					( int )PmxBoneNum,
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
		for( i = 0 ; ( DWORD )i < PmxBoneNum ; i ++ )
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
		if( MLPhysicsInfo.PmxPhysicsInfoDim != NULL )
		{
			DXFREE( MLPhysicsInfo.PmxPhysicsInfoDim ) ;
			MLPhysicsInfo.PmxPhysicsInfoDim = NULL ;
		}

		if( MLPhysicsInfo.PmxPhysicsJointInfoDim != NULL )
		{
			DXFREE( MLPhysicsInfo.PmxPhysicsJointInfoDim ) ;
			MLPhysicsInfo.PmxPhysicsJointInfoDim = NULL ;
		}
	}
#endif

	// �ǂݍ��݂悤���f�������
	MV1TermReadModel( &RModel ) ; 

	// �n���h����Ԃ�
	return NewHandle ;
}

// �s����v�Z����
static void MV1LoadModelToPMX_SetupMatrix( PMX_READ_BONE_INFO *BoneInfo, int BoneNum, int UseInitParam, int IKSkip )
{
	int i ;
	int SetupBoneNum ;
	PMX_READ_BONE_INFO *BoneInfoTemp ;

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
				PMX_READ_BONE_INFO *ParentBoneInfo ;

				ParentBoneInfo = ( PMX_READ_BONE_INFO * )BoneInfo->Frame->Parent->UserData ;

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
static void MV1LoadModelToPMX_SetupIK( PMX_READ_BONE_INFO *BoneInfo, PMX_READ_IK_INFO *IKInfoFirst )
{
	int i, j, k ;
	PMX_READ_BONE_INFO *ChainBone ;
	PMX_READ_BONE_INFO *ChainParentBone ;
	VECTOR IKBonePos ;
	VECTOR BonePos ;
	VECTOR TargPos ;
	float Rot ;
	float Cos ;
	float IKsin, IKcos ;
	float Dummy ;
	FLOAT4 qIK ;
	PMX_READ_IK_INFO *IKInfo ;

	for( IKInfo = IKInfoFirst ; IKInfo ; IKInfo = IKInfo->Next )
	{
		// IK�̃^�[�Q�b�g�̃��[���h���W���擾���Ă���
		IKBonePos.x = IKInfo->Bone->LocalWorldMatrix.m[ 3 ][ 0 ];
		IKBonePos.y = IKInfo->Bone->LocalWorldMatrix.m[ 3 ][ 1 ];
		IKBonePos.z = IKInfo->Bone->LocalWorldMatrix.m[ 3 ][ 2 ];

		QuatConvertToMatrix( IKInfo->TargetBone->IKmat, IKInfo->TargetBone->IKQuat, IKInfo->TargetBone->Translate );

		int ikt = IKInfo->Base->LoopNum / 2 ;
		for( i = 0 ; i < IKInfo->Base->LoopNum ; i ++ )
		{
			for( j = 0 ; j < IKInfo->Base->LinkNum ; j ++ )
			{
				ChainBone = &BoneInfo[ IKInfo->Base->Link[ j ].BoneIndex ] ;
				ChainParentBone = ( PMX_READ_BONE_INFO * )ChainBone->Frame->Parent->UserData ;

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
				if( IKInfo->Base->Link[ j ].RotLockFlag == 1 && i < ikt )
				{
					if( IKInfo->Base->Link[ j ].RotLockMin[ 1 ] == 0 && IKInfo->Base->Link[ j ].RotLockMax[ 1 ] == 0 && IKInfo->Base->Link[ j ].RotLockMin[ 2 ] == 0 && IKInfo->Base->Link[ j ].RotLockMax[ 2 ] == 0 )
					{
						float vvx = v.x * ChainParentBone->LocalWorldMatrix.m[ 0 ][ 0 ] + v.y * ChainParentBone->LocalWorldMatrix.m[ 0 ][ 1 ] + v.z * ChainParentBone->LocalWorldMatrix.m[ 0 ][ 2 ] ;
						v.x = vvx >= 0.0f ? 1.0f : -1.0f ;
						v.y = 0.0f ;
						v.z = 0.0f ;
					}
					else
					if( IKInfo->Base->Link[ j ].RotLockMin[ 0 ] == 0 && IKInfo->Base->Link[ j ].RotLockMax[ 0 ] == 0 && IKInfo->Base->Link[ j ].RotLockMin[ 2 ] == 0 && IKInfo->Base->Link[ j ].RotLockMax[ 2 ] == 0 )
					{
						float vvy = v.x * ChainParentBone->LocalWorldMatrix.m[ 1 ][ 0 ] + v.y * ChainParentBone->LocalWorldMatrix.m[ 1 ][ 1 ] + v.z * ChainParentBone->LocalWorldMatrix.m[ 1 ][ 2 ] ;
						v.y = vvy >= 0.0f ? 1.0f : -1.0f ;
						v.x = 0.0f ;
						v.z = 0.0f ;
					}
					else
					if( IKInfo->Base->Link[ j ].RotLockMin[ 0 ] == 0 && IKInfo->Base->Link[ j ].RotLockMax[ 0 ] == 0 && IKInfo->Base->Link[ j ].RotLockMin[ 1 ] == 0 && IKInfo->Base->Link[ j ].RotLockMax[ 1 ] == 0 )
					{
						float vvz = v.x * ChainParentBone->LocalWorldMatrix.m[ 2 ][ 0 ] + v.y * ChainParentBone->LocalWorldMatrix.m[ 2 ][ 1 ] + v.z * ChainParentBone->LocalWorldMatrix.m[ 2 ][ 2 ] ;
						v.z = vvz >= 0.0f ? 1.0f : -1.0f ;
						v.x = 0.0f ;
						v.y = 0.0f ;
					}
					else
					{
						// calculate roll axis
						VECTOR vv;

						vv.x = v.x * ChainParentBone->LocalWorldMatrix.m[ 0 ][ 0 ] + v.y * ChainParentBone->LocalWorldMatrix.m[ 0 ][ 1 ] + v.z * ChainParentBone->LocalWorldMatrix.m[ 0 ][ 2 ] ;
						vv.y = v.x * ChainParentBone->LocalWorldMatrix.m[ 1 ][ 0 ] + v.y * ChainParentBone->LocalWorldMatrix.m[ 1 ][ 1 ] + v.z * ChainParentBone->LocalWorldMatrix.m[ 1 ][ 2 ] ;
						vv.z = v.x * ChainParentBone->LocalWorldMatrix.m[ 2 ][ 0 ] + v.y * ChainParentBone->LocalWorldMatrix.m[ 2 ][ 1 ] + v.z * ChainParentBone->LocalWorldMatrix.m[ 2 ][ 2 ] ;

						v = VNorm( vv ) ;
					}
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
				if( Rot > IKInfo->Base->RotLimit * ( j + 1 ) * 2 )
					Rot = IKInfo->Base->RotLimit * ( j + 1 ) * 2 ;

				_SINCOS_PLATFORM( Rot, &IKsin, &IKcos );
				qIK.x = v.x * IKsin;
				qIK.y = v.y * IKsin;
				qIK.z = v.z * IKsin;
				qIK.w = IKcos;

				ChainBone->IKQuat = QuatMul( qIK, ChainBone->IKQuat );

				QuatConvertToMatrix( ChainBone->IKmat, ChainBone->IKQuat, ChainBone->Translate );

				if( IKInfo->Base->Link[ j ].RotLockFlag == 1 )
				{
					// ����]�p�x���Z�o
					if( ( IKInfo->Base->Link[ j ].RotLockMin[ 0 ] > -1.570796f ) & ( IKInfo->Base->Link[ j ].RotLockMax[ 0 ] < 1.570796f ) )
					{
						// Z*X*Y��
						// X�����
						float fLimit = 1.535889f ;			// 88.0f/180.0f*3.14159265f;
						float fSX = -ChainBone->IKmat.m[ 2 ][ 1 ] ;				// sin(��x)
						float fX  = _ASIN( fSX ) ;			// X����茈��
						float fCX ;
						_SINCOS_PLATFORM( fX, &Dummy, &fCX ) ;

						// �W���o�����b�N���
						if( _FABS( fX ) > fLimit )
						{
							fX  = ( fX < 0 ) ? - fLimit : fLimit ;
							_SINCOS_PLATFORM( fX, &Dummy, &fCX ) ;
						}

						// Y�����
						float fSY = ChainBone->IKmat.m[ 2 ][ 0 ] / fCX ;
						float fCY = ChainBone->IKmat.m[ 2 ][ 2 ] / fCX ;
						float fY = _ATAN2( fSY, fCY ) ;	// Y����茈��

						// Z�����
						float fSZ = ChainBone->IKmat.m[ 0 ][ 1 ] / fCX ;
						float fCZ = ChainBone->IKmat.m[ 1 ][ 1 ] / fCX ;
						float fZ = _ATAN2( fSZ, fCZ ) ;

						// �p�x�̐���
						if( fX < IKInfo->Base->Link[ j ].RotLockMin[ 0 ] )
						{
							float tf = 2 * IKInfo->Base->Link[ j ].RotLockMin[ 0 ] - fX ;
							fX = tf <= IKInfo->Base->Link[ j ].RotLockMax[ 0 ] && j < ikt ? tf : IKInfo->Base->Link[ j ].RotLockMin[ 0 ] ;
						}
						if( fX > IKInfo->Base->Link[ j ].RotLockMax[ 0 ] )
						{
							float tf = 2 * IKInfo->Base->Link[ j ].RotLockMax[ 0 ] - fX ;
							fX = tf >= IKInfo->Base->Link[ j ].RotLockMin[ 0 ] && j < ikt ? tf : IKInfo->Base->Link[ j ].RotLockMax[ 0 ] ;
						}
						if( fY < IKInfo->Base->Link[ j ].RotLockMin[ 1 ] )
						{
							float tf = 2 * IKInfo->Base->Link[ j ].RotLockMin[ 1 ] - fY ;
							fY = tf <= IKInfo->Base->Link[ j ].RotLockMax[ 1 ] && j < ikt ? tf : IKInfo->Base->Link[ j ].RotLockMin[ 1 ] ;
						}
						if( fY > IKInfo->Base->Link[ j ].RotLockMax[ 1 ] )
						{
							float tf = 2 * IKInfo->Base->Link[ j ].RotLockMax[ 1 ] - fY ;
							fY = tf >= IKInfo->Base->Link[ j ].RotLockMin[ 1 ] && j < ikt ? tf : IKInfo->Base->Link[ j ].RotLockMax[ 1 ] ;
						}
						if( fZ < IKInfo->Base->Link[ j ].RotLockMin[ 2 ] )
						{
							float tf = 2 * IKInfo->Base->Link[ j ].RotLockMin[ 2 ] - fZ ;
							fZ = tf <= IKInfo->Base->Link[ j ].RotLockMax[ 2 ] && j < ikt ? tf : IKInfo->Base->Link[ j ].RotLockMin[ 2 ] ;
						}
						if( fZ > IKInfo->Base->Link[ j ].RotLockMax[ 2 ] )
						{
							float tf = 2 * IKInfo->Base->Link[ j ].RotLockMax[ 2 ] - fZ ;
							fZ = tf >= IKInfo->Base->Link[ j ].RotLockMin[ 2 ] && j < ikt ? tf : IKInfo->Base->Link[ j ].RotLockMax[ 2 ] ;
						}

						// ���肵���p�x�Ńx�N�g������]
						MATRIX mX, mY, mZ, mT ;

						CreateRotationXMatrix( &mX, fX ) ;
						CreateRotationXMatrix( &mY, fY ) ;
						CreateRotationXMatrix( &mZ, fZ ) ;

						MV1LoadModelToVMD_CreateMultiplyMatrixRotOnly(               &mT, &mZ, &mX ) ;
						MV1LoadModelToVMD_CreateMultiplyMatrixRotOnly( &ChainBone->IKmat, &mT, &mY ) ;
					}
					else
					if((IKInfo->Base->Link[ j ].RotLockMin[ 1 ]>-1.570796f)&(IKInfo->Base->Link[ j ].RotLockMax[ 1 ]<1.570796f))
					{
						// X*Y*Z��
						// Y�����
						float fLimit = 1.535889f ;		// 88.0f/180.0f*3.14159265f;
						float fSY = -ChainBone->IKmat.m[ 0 ][ 2 ] ;			// sin(��y)
						float fY  = _ASIN( fSY ) ;	// Y����茈��
						float fCY ;
						_SINCOS_PLATFORM( fY, &Dummy, &fCY ) ;

						// �W���o�����b�N���
						if( _FABS( fY ) > fLimit )
						{
							fY = ( fY < 0 ) ? - fLimit : fLimit ;
							_SINCOS_PLATFORM( fY, &Dummy, &fCY ) ;
						}

						// X�����
						float fSX = ChainBone->IKmat.m[ 1 ][ 2 ] / fCY ;
						float fCX = ChainBone->IKmat.m[ 2 ][ 2 ] / fCY ;
						float fX  = _ATAN2( fSX, fCX ) ;	// X����茈��

						// Z�����
						float fSZ = ChainBone->IKmat.m[ 0 ][ 1 ] / fCY ;
						float fCZ = ChainBone->IKmat.m[ 0 ][ 0 ] / fCY ;
						float fZ  = _ATAN2( fSZ, fCZ ) ;	// Z����茈��

						// �p�x�̐���
						if( fX < IKInfo->Base->Link[ j ].RotLockMin[ 0 ] )
						{
							float tf = 2 * IKInfo->Base->Link[ j ].RotLockMin[ 0 ] - fX ;
							fX = tf <= IKInfo->Base->Link[ j ].RotLockMax[ 0 ] && j < ikt ? tf : IKInfo->Base->Link[ j ].RotLockMin[ 0 ] ;
						}
						if( fX > IKInfo->Base->Link[ j ].RotLockMax[ 0 ] )
						{
							float tf = 2 * IKInfo->Base->Link[ j ].RotLockMax[ 0 ] - fX ;
							fX = tf >= IKInfo->Base->Link[ j ].RotLockMin[ 0 ] && j < ikt ? tf : IKInfo->Base->Link[ j ].RotLockMax[ 0 ] ;
						}
						if( fY < IKInfo->Base->Link[ j ].RotLockMin[ 1 ] )
						{
							float tf = 2 * IKInfo->Base->Link[ j ].RotLockMin[ 1 ] - fY ;
							fY = tf <= IKInfo->Base->Link[ j ].RotLockMax[ 1 ] && j < ikt ? tf : IKInfo->Base->Link[ j ].RotLockMin[ 1 ] ;
						}
						if( fY > IKInfo->Base->Link[ j ].RotLockMax[ 1 ] )
						{
							float tf = 2 * IKInfo->Base->Link[ j ].RotLockMax[ 1 ] - fY ;
							fY = tf >= IKInfo->Base->Link[ j ].RotLockMin[ 1 ] && j < ikt ? tf : IKInfo->Base->Link[ j ].RotLockMax[ 1 ] ;
						}
						if( fZ < IKInfo->Base->Link[ j ].RotLockMin[ 2 ] )
						{
							float tf = 2 * IKInfo->Base->Link[ j ].RotLockMin[ 2 ] - fZ ;
							fZ = tf <= IKInfo->Base->Link[ j ].RotLockMax[ 2 ] && j < ikt ? tf : IKInfo->Base->Link[ j ].RotLockMin[ 2 ] ;
						}
						if( fZ > IKInfo->Base->Link[ j ].RotLockMax[ 2 ] )
						{
							float tf = 2 * IKInfo->Base->Link[ j ].RotLockMax[ 2 ] - fZ ;
							fZ = tf >= IKInfo->Base->Link[ j ].RotLockMin[ 2 ] && j < ikt ? tf : IKInfo->Base->Link[ j ].RotLockMax[ 2 ] ;
						}

						// ���肵���p�x�Ńx�N�g������]
						MATRIX mX, mY, mZ, mT ;

						CreateRotationXMatrix( &mX, fX ) ;
						CreateRotationXMatrix( &mY, fY ) ;
						CreateRotationXMatrix( &mZ, fZ ) ;

						MV1LoadModelToVMD_CreateMultiplyMatrixRotOnly(               &mT, &mX, &mY ) ;
						MV1LoadModelToVMD_CreateMultiplyMatrixRotOnly( &ChainBone->IKmat, &mT, &mZ ) ;
					}
					else
					{
						// Y*Z*X��
						// Z�����
						float fLimit = 1.535889f ;		// 88.0f/180.0f*3.14159265f;
						float fSZ = -ChainBone->IKmat.m[ 1 ][ 0 ] ;			// sin(��y)
						float fZ  = _ASIN( fSZ ) ;	// Y����茈��
						float fCZ ;
						_SINCOS_PLATFORM( fZ, &Dummy, &fCZ ) ;

						// �W���o�����b�N���
						if( _FABS( fZ ) > fLimit )
						{
							fZ = ( fZ < 0 ) ? - fLimit : fLimit ;
							_SINCOS_PLATFORM( fZ, &Dummy, &fCZ ) ;
						}

						// X�����
						float fSX = ChainBone->IKmat.m[ 1 ][ 2 ] / fCZ ;
						float fCX = ChainBone->IKmat.m[ 1 ][ 1 ] / fCZ ;
						float fX  = _ATAN2( fSX, fCX ) ;	// X����茈��

						// Y�����
						float fSY = ChainBone->IKmat.m[ 2 ][ 0 ] / fCZ ;
						float fCY = ChainBone->IKmat.m[ 0 ][ 0 ] / fCZ ;
						float fY  = _ATAN2( fSY, fCY ) ;	// Z����茈��

						// �p�x�̐���
						if( fX < IKInfo->Base->Link[ j ].RotLockMin[ 0 ] )
						{
							float tf= 2 * IKInfo->Base->Link[ j ].RotLockMin[ 0 ] - fX;
							fX = tf <= IKInfo->Base->Link[ j ].RotLockMax[ 0 ] && j < ikt ? tf : IKInfo->Base->Link[ j ].RotLockMin[ 0 ] ;
						}
						if( fX > IKInfo->Base->Link[ j ].RotLockMax[ 0 ] )
						{
							float tf= 2 * IKInfo->Base->Link[ j ].RotLockMax[ 0 ] - fX;
							fX = tf >= IKInfo->Base->Link[ j ].RotLockMin[ 0 ] && j < ikt ? tf : IKInfo->Base->Link[ j ].RotLockMax[ 0 ] ;
						}
						if( fY < IKInfo->Base->Link[ j ].RotLockMin[ 1 ] )
						{
							float tf= 2 * IKInfo->Base->Link[ j ].RotLockMin[ 1 ] - fY;
							fY = tf <= IKInfo->Base->Link[ j ].RotLockMax[ 1 ] && j < ikt ? tf : IKInfo->Base->Link[ j ].RotLockMin[ 1 ] ;
						}
						if( fY > IKInfo->Base->Link[ j ].RotLockMax[ 1 ] )
						{
							float tf= 2 * IKInfo->Base->Link[ j ].RotLockMax[ 1 ] - fY;
							fY = tf >= IKInfo->Base->Link[ j ].RotLockMin[ 1 ] && j < ikt ? tf : IKInfo->Base->Link[ j ].RotLockMax[ 1 ] ;
						}
						if( fZ < IKInfo->Base->Link[ j ].RotLockMin[ 2 ] )
						{
							float tf= 2 * IKInfo->Base->Link[ j ].RotLockMin[ 2 ] - fZ;
							fZ = tf <= IKInfo->Base->Link[ j ].RotLockMax[ 2 ] && j < ikt ? tf : IKInfo->Base->Link[ j ].RotLockMin[ 2 ] ;
						}
						if( fZ > IKInfo->Base->Link[ j ].RotLockMax[ 2 ] )
						{
							float tf= 2 * IKInfo->Base->Link[ j ].RotLockMax[ 2 ] - fZ;
							fZ = tf >= IKInfo->Base->Link[ j ].RotLockMin[ 2 ] && j < ikt ? tf : IKInfo->Base->Link[ j ].RotLockMax[ 2 ] ;
						}

						// ���肵���p�x�Ńx�N�g������]
						MATRIX mX, mY, mZ, mT ;

						CreateRotationXMatrix( &mX, fX ) ;
						CreateRotationXMatrix( &mY, fY ) ;
						CreateRotationXMatrix( &mZ, fZ ) ;

						MV1LoadModelToVMD_CreateMultiplyMatrixRotOnly(               &mT, &mY, &mZ ) ;
						MV1LoadModelToVMD_CreateMultiplyMatrixRotOnly( &ChainBone->IKmat, &mT, &mX ) ;
					}
					QuatConvertFromMatrix( ChainBone->IKQuat, ChainBone->IKmat ) ;
				}

				for( k = j ; k >= 0 ; k-- )
				{
					ChainBone= &BoneInfo[ IKInfo->Base->Link[ k ].BoneIndex ];
					MV1LoadModelToVMD_CreateMultiplyMatrix( &ChainBone->LocalWorldMatrix, &ChainBone->IKmat, &( ( PMX_READ_BONE_INFO * )ChainBone->Frame->Parent->UserData )->LocalWorldMatrix ) ;
				}

				MV1LoadModelToVMD_CreateMultiplyMatrix( &IKInfo->TargetBone->LocalWorldMatrix, &IKInfo->TargetBone->IKmat, &( ( PMX_READ_BONE_INFO * )IKInfo->TargetBone->Frame->Parent->UserData )->LocalWorldMatrix );
			}

			for( j = 0 ; j < IKInfo->Base->LinkNum ; j ++ )
			{
				ChainBone = &BoneInfo[ IKInfo->Base->Link[ j ].BoneIndex ] ;

				ChainBone->Rotate = ChainBone->IKQuat ;
			}
			IKInfo->TargetBone->Rotate = IKInfo->TargetBone->IKQuat ;
		}
	}
}


// �w��̃{�[���ɃA�j���[�V�����̎w��L�[�̃p�����[�^�𔽉f������
static void MV1LoadModelToPMX_SetupOneBoneMatrixFormAnimKey( PMX_READ_BONE_INFO *BoneInfo, int Time, int LoopNo, int MaxTime, int ValidNextRate, float NextRate )
{
	VMD_READ_KEY_INFO *NowKey, *NextKey ;

	if( BoneInfo->IsIK || BoneInfo->IsAddParent )
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
			BoneInfo->Rotate = NowKey->MVRRotKey[ 0 ] ;
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
static int _MV1LoadModelToVMD_PMX(
	int								DataIndex,
	MV1_MODEL_R *					RModel,
	void *							DataBuffer,
	int								DataSize,
	const wchar_t *					Name,
	int								LoopMotionFlag,
	int								DisablePhysicsFlag,
	PMX_READ_BONE_INFO *			PmxBoneInfo,
	int								PmxBoneNum,
	PMX_READ_IK_INFO *				PmxIKInfoFirst,
#ifndef DX_NON_BULLET_PHYSICS
	DX_MODELLOADER3_PMX_PHYSICS_INFO *	MLPhysicsInfo,
#endif
	bool							FPS60
)
{
	int i, j, k ;
	VMD_READ_NODE_INFO *VmdNode ;
	VMD_READ_KEY_INFO *VmdKey, *VmdKeyTemp1, *VmdKeyTemp2 ;
	VMD_READ_FACE_KEY_SET_INFO *VmdFaceKeySet ;
	VMD_READ_FACE_KEY_INFO *VmdFaceKey ;
	PMX_READ_BONE_INFO *BoneInfo ;
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
		SetupPhysicsObject_PMXPhysicsInfo( MLPhysicsInfo, DisablePhysicsFlag ) ;
	}
#endif

	// �A�j���[�V�����Z�b�g��ǉ�
	AnimSet = MV1RAddAnimSetW( RModel, Name ) ;
	if( AnimSet == NULL )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x58\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xa2\x30\xcb\x30\xe1\x30\xfc\x30\xb7\x30\xe7\x30\xf3\x30\xbb\x30\xc3\x30\xc8\x30\x6e\x30\xfd\x8f\xa0\x52\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMX Load Error : �A�j���[�V�����Z�b�g�̒ǉ��Ɏ��s���܂���\n" @*/ )) ;
		goto ENDLABEL ;
	}

	// ���[�v���[�V�������ǂ������Z�b�g
	AnimSet->IsLoopAnim = LoopMotionFlag ? 1 : 0 ;

	// �{�[���̃A�j���[�V�����|�C���^�̏�����
	BoneInfo = PmxBoneInfo ;
	for( i = 0 ; i < PmxBoneNum ; i ++, BoneInfo ++ )
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
		wchar_t VmdNodeNameW[ 128 ] ;
		ConvString( VmdNode->Name, DX_CHARCODEFORMAT_SHIFTJIS, ( char * )VmdNodeNameW, sizeof( VmdNodeNameW ), WCHAR_T_CHARCODEFORMAT ) ;
		for( Frame = RModel->FrameFirst ; Frame && _WCSCMP( Frame->NameW, VmdNodeNameW ) != 0 ; Frame = Frame->DataNext ){}
		if( Frame == NULL ) continue ;

		BoneInfo = ( PMX_READ_BONE_INFO * )Frame->UserData ;

		// �A�j���[�V�����̒ǉ�
		Anim = MV1RAddAnim( RModel, AnimSet ) ;
		if( Anim == NULL )
		{
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x58\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xa2\x30\xcb\x30\xe1\x30\xfc\x30\xb7\x30\xe7\x30\xf3\x30\x6e\x30\xfd\x8f\xa0\x52\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMX Load Error : �A�j���[�V�����̒ǉ��Ɏ��s���܂���\n" @*/ )) ;
			goto ENDLABEL ;
		}

		// �{�[���ɏ��Z�b�g
		BoneInfo->Anim = Anim ;
		BoneInfo->Node = VmdNode ;

		// �Ώۃm�[�h�̃Z�b�g
		Anim->TargetFrameIndex = Frame->Index ;

		// �ő厞�Ԃ��Z�b�g
		if( BoneInfo->IsIK || BoneInfo->IsAddParent )
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
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x58\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xa2\x30\xcb\x30\xe1\x30\xfc\x30\xb7\x30\xe7\x30\xf3\x30\xa7\x5e\x19\x6a\xad\x30\xfc\x30\xbb\x30\xc3\x30\xc8\x30\x6e\x30\xfd\x8f\xa0\x52\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMX Load Error : �A�j���[�V�������W�L�[�Z�b�g�̒ǉ��Ɏ��s���܂���\n" @*/ )) ;
			goto ENDLABEL ;
		}
		KeyRotSet = MV1RAddAnimKeySet( RModel, Anim ) ;
		if( KeyRotSet == NULL )
		{
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x58\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xa2\x30\xcb\x30\xe1\x30\xfc\x30\xb7\x30\xe7\x30\xf3\x30\xde\x56\xe2\x8e\xad\x30\xfc\x30\xbb\x30\xc3\x30\xc8\x30\x6e\x30\xfd\x8f\xa0\x52\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMX Load Error : �A�j���[�V������]�L�[�Z�b�g�̒ǉ��Ɏ��s���܂���\n" @*/ )) ;
			goto ENDLABEL ;
		}

		KeyPosSet->Type = MV1_ANIMKEY_TYPE_VECTOR ;
		KeyPosSet->DataType = MV1_ANIMKEY_DATATYPE_TRANSLATE ;
		KeyPosSet->TimeType = MV1_ANIMKEY_TIME_TYPE_KEY ;
		KeyPosSet->TotalTime = ( float )VmdNode->MaxFrame ;
		KeyPosSet->Num = ( int )( BoneInfo->IsIK || BoneInfo->IsAddParent ? VmdData.MaxTime : VmdNode->MaxFrame - VmdNode->MinFrame ) ;
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
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x58\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xa2\x30\xcb\x30\xe1\x30\xfc\x30\xb7\x30\xe7\x30\xf3\x30\xad\x30\xfc\x30\xbf\x30\xa4\x30\xe0\x30\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMX Load Error : �A�j���[�V�����L�[�^�C�����i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
			goto ENDLABEL ;
		}
		KeyPosSet->KeyVector = ( VECTOR * )ADDMEMAREA( sizeof( VECTOR ) * KeyPosSet->Num, &RModel->Mem ) ;
		if( KeyPosSet->KeyVector == NULL )
		{
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x58\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xa2\x30\xcb\x30\xe1\x30\xfc\x30\xb7\x30\xe7\x30\xf3\x30\xad\x30\xfc\x30\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMX Load Error : �A�j���[�V�����L�[���i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
			goto ENDLABEL ;
		}

		KeyRotSet->KeyTime = ( float * )ADDMEMAREA( sizeof( float ) * KeyRotSet->Num, &RModel->Mem ) ;
		if( KeyRotSet->KeyTime == NULL )
		{
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x58\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xa2\x30\xcb\x30\xe1\x30\xfc\x30\xb7\x30\xe7\x30\xf3\x30\xad\x30\xfc\x30\xbf\x30\xa4\x30\xe0\x30\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMX Load Error : �A�j���[�V�����L�[�^�C�����i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
			goto ENDLABEL ;
		}
		KeyRotSet->KeyVector = ( VECTOR * )ADDMEMAREA( sizeof( FLOAT4 ) * KeyRotSet->Num, &RModel->Mem ) ;
		if( KeyRotSet->KeyVector == NULL )
		{
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x58\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xa2\x30\xcb\x30\xe1\x30\xfc\x30\xb7\x30\xe7\x30\xf3\x30\xad\x30\xfc\x30\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMX Load Error : �A�j���[�V�����L�[���i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
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
			if( BoneInfo->IsIK || BoneInfo->IsAddParent )
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
			if( ( BoneInfo->IsIK || BoneInfo->IsAddParent ) && VmdKey->Frame > 0 )
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

			if( ( BoneInfo->IsIK || BoneInfo->IsAddParent ) && KeyRot - BoneInfo->KeyRot < KeyRotSet->Num )
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
	if( PmxBoneInfo != NULL )
	{
		// �A�j���[�V�����̍Đ�����
		BoneInfo = PmxBoneInfo ;
		for( i = 0 ; i < PmxBoneNum ; i ++, BoneInfo ++ )
		{
			BoneInfo->IsIKAnim = FALSE ;
			if( BoneInfo->Anim != NULL )
			{
				BoneInfo->NowKey = BoneInfo->Node->FirstKey ;
			}
			else
			if( BoneInfo->IsIK || BoneInfo->IsAddParent )
			{
				// IK�{�[���ŃA�j���[�V�������Ȃ��ꍇ�̓L�[��ł���������
				if( BoneInfo->Anim == NULL )
				{
					BoneInfo->IsIKAnim = TRUE ;

					// �A�j���[�V�����̒ǉ�
					BoneInfo->Anim = MV1RAddAnim( RModel, AnimSet ) ;
					if( BoneInfo->Anim == NULL )
					{
						DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x58\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xa2\x30\xcb\x30\xe1\x30\xfc\x30\xb7\x30\xe7\x30\xf3\x30\x6e\x30\xfd\x8f\xa0\x52\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMX Load Error : �A�j���[�V�����̒ǉ��Ɏ��s���܂���\n" @*/ )) ;
						goto ENDLABEL ;
					}

					// �Ώۃm�[�h�̃Z�b�g
					BoneInfo->Anim->TargetFrameIndex = BoneInfo->Frame->Index ;
					BoneInfo->Anim->MaxTime = ( float )VmdData.MaxTime ;

					// �A�j���[�V�����L�[�̏����Z�b�g
					KeyPosSet = MV1RAddAnimKeySet( RModel, BoneInfo->Anim ) ;
					if( KeyPosSet == NULL )
					{
						DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x58\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xa2\x30\xcb\x30\xe1\x30\xfc\x30\xb7\x30\xe7\x30\xf3\x30\xad\x30\xfc\x30\xbb\x30\xc3\x30\xc8\x30\x6e\x30\xfd\x8f\xa0\x52\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMX Load Error : �A�j���[�V�����L�[�Z�b�g�̒ǉ��Ɏ��s���܂���\n" @*/ )) ;
						goto ENDLABEL ;
					}
					KeyRotSet = MV1RAddAnimKeySet( RModel, BoneInfo->Anim ) ;
					if( KeyRotSet == NULL )
					{
						DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x58\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xa2\x30\xcb\x30\xe1\x30\xfc\x30\xb7\x30\xe7\x30\xf3\x30\xad\x30\xfc\x30\xbb\x30\xc3\x30\xc8\x30\x6e\x30\xfd\x8f\xa0\x52\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMX Load Error : �A�j���[�V�����L�[�Z�b�g�̒ǉ��Ɏ��s���܂���\n" @*/ )) ;
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
						DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x58\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xa2\x30\xcb\x30\xe1\x30\xfc\x30\xb7\x30\xe7\x30\xf3\x30\xad\x30\xfc\x30\xbf\x30\xa4\x30\xe0\x30\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMX Load Error : �A�j���[�V�����L�[�^�C�����i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
						goto ENDLABEL ;
					}
					KeyPosSet->KeyVector = ( VECTOR * )ADDMEMAREA( sizeof( VECTOR ) * KeyPosSet->Num, &RModel->Mem ) ;
					if( KeyPosSet->KeyVector == NULL )
					{
						DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x58\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xa2\x30\xcb\x30\xe1\x30\xfc\x30\xb7\x30\xe7\x30\xf3\x30\xad\x30\xfc\x30\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMX Load Error : �A�j���[�V�����L�[���i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
						goto ENDLABEL ;
					}

					KeyRotSet->KeyTime = ( float * )ADDMEMAREA( sizeof( float ) * KeyRotSet->Num, &RModel->Mem ) ;
					if( KeyRotSet->KeyTime == NULL )
					{
						DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x58\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xa2\x30\xcb\x30\xe1\x30\xfc\x30\xb7\x30\xe7\x30\xf3\x30\xad\x30\xfc\x30\xbf\x30\xa4\x30\xe0\x30\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMX Load Error : �A�j���[�V�����L�[�^�C�����i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
						goto ENDLABEL ;
					}
					KeyRotSet->KeyVector = ( VECTOR * )ADDMEMAREA( sizeof( FLOAT4 ) * KeyRotSet->Num, &RModel->Mem ) ;
					if( KeyRotSet->KeyVector == NULL )
					{
						DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x58\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xa2\x30\xcb\x30\xe1\x30\xfc\x30\xb7\x30\xe7\x30\xf3\x30\xad\x30\xfc\x30\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMX Load Error : �A�j���[�V�����L�[���i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
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
					DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x58\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xa2\x30\xcb\x30\xe1\x30\xfc\x30\xb7\x30\xe7\x30\xf3\x30\x4c\x88\x17\x52\xad\x30\xfc\x30\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMX Load Error : �A�j���[�V�����s��L�[���i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
					goto ENDLABEL ;
				}
			}
		}

#ifndef DX_NON_BULLET_PHYSICS
		if( MLPhysicsInfo )
		{
			BoneInfo = PmxBoneInfo ;
			for( i = 0 ; i < PmxBoneNum ; i ++, BoneInfo ++ )
			{
				if( BoneInfo->IsPhysics == 0 ) continue ;

				BoneInfo->KeyMatrix = ( MATRIX * )DXALLOC( sizeof( MATRIX ) * ( ( FPS60 ? VmdData.MaxTime * 2 : VmdData.MaxTime ) + 1 ) ) ;
				if( BoneInfo->KeyMatrix == NULL )
				{
					DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x58\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xa2\x30\xcb\x30\xe1\x30\xfc\x30\xb7\x30\xe7\x30\xf3\x30\x4c\x88\x17\x52\xad\x30\xfc\x30\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMX Load Error : �A�j���[�V�����s��L�[���i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
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
			BoneInfo = PmxBoneInfo ;
			for( i = 0 ; i < PmxBoneNum ; i ++, BoneInfo ++ )
			{
				if( BoneInfo->Anim != NULL && BoneInfo->IsIK == FALSE && BoneInfo->IsAddParent == FALSE )
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
						for( j = 0 ; j < PmxBoneNum ; j ++ )
						{
							if( PmxBoneInfo[ j ].IsIK == FALSE && BoneInfo->IsAddParent == FALSE )
							{
								MV1LoadModelToPMX_SetupOneBoneMatrixFormAnimKey( &PmxBoneInfo[ j ], TimeNo, LoopNo, MaxTime, TimeDivLoopCount == 0 ? FALSE : ValidNextRate, NextRate ) ;
							}
						}

						// �s��̌v�Z
						MV1LoadModelToPMX_SetupMatrix( PmxBoneInfo, PmxBoneNum, FALSE, TRUE ) ;
					}
					else
					{
						// ���ׂẴt���[���̌��݂̃t���[���ł̃p�����[�^���Z�o����
						for( j = 0 ; j < PmxBoneNum ; j ++ )
						{
							MV1LoadModelToPMX_SetupOneBoneMatrixFormAnimKey( &PmxBoneInfo[ j ], TimeNo, LoopNo, MaxTime, TimeDivLoopCount == 0 ? FALSE : ValidNextRate, NextRate ) ;
						}

						// �s��̌v�Z
						MV1LoadModelToPMX_SetupMatrix( PmxBoneInfo, PmxBoneNum, FALSE, FALSE ) ;

						// IK�̌v�Z���s���̂̓��[�V�������[�v�̍ŏ�����
						if( LoopNo == 0 )
						{
							// �h�j�̌v�Z
							if( PmxIKInfoFirst )
							{
								MV1LoadModelToPMX_SetupIK( PmxBoneInfo, PmxIKInfoFirst ) ;
							}
						}
					}

#ifndef DX_NON_BULLET_PHYSICS
					// �������Z���s��
					if( MLPhysicsInfo )
					{
						OneFrameProcess_PMXPhysicsInfo( MLPhysicsInfo, TimeNo, LoopNo, FPS60, TimeDivLoopCount == 0 ? FALSE : ValidNextRate, TimeDivLoopNum ) ;
					}
#endif
				}

				if( LoopNo == 0 && ( ValidNextRate || FPS60 || ( FPS60 == false && TimeNo % 2 == 0 ) ) )
				{
					// �h�j�Ɋւ���Ă���{�[�����͂h�j�̉e�����Ȃ��h�j�{�[���̎q�{�[���̃L�[��ۑ�
					BoneInfo = PmxBoneInfo ;
					for( j = 0 ; j < PmxBoneNum ; j ++, BoneInfo ++ )
					{
						if( BoneInfo->IsIK || BoneInfo->IsAddParent )
						{
							BoneInfo->KeyPosTime[ TimeNo ] = ( float )TimeNo / 2.0f ;
							BoneInfo->KeyRotTime[ TimeNo ] = ( float )TimeNo / 2.0f ;

							// �t�^����
							if( BoneInfo->IsAddParent )
							{
								PMX_READ_BONE_INFO *AddBoneInfo ;

								if( BoneInfo->Base->Flag_AddMov && BoneInfo->Base->AddParentBoneIndex >= 0 )
								{
									AddBoneInfo = &PmxBoneInfo[ BoneInfo->Base->AddParentBoneIndex ] ;
									while( AddBoneInfo->Base->Flag_AddMov )
									{
										AddBoneInfo = &PmxBoneInfo[ AddBoneInfo->Base->AddParentBoneIndex ] ;
									}

									if( BoneInfo->Base->AddRatio != 1.0f )
									{
										BoneInfo->KeyPos[ TimeNo ].x = BoneInfo->Translate.x + ( AddBoneInfo->Translate.x - BoneInfo->Translate.x ) * BoneInfo->Base->AddRatio ;
										BoneInfo->KeyPos[ TimeNo ].y = BoneInfo->Translate.y + ( AddBoneInfo->Translate.y - BoneInfo->Translate.y ) * BoneInfo->Base->AddRatio ;
										BoneInfo->KeyPos[ TimeNo ].z = BoneInfo->Translate.z + ( AddBoneInfo->Translate.z - BoneInfo->Translate.z ) * BoneInfo->Base->AddRatio ;
									}
									else
									{
										BoneInfo->KeyPos[ TimeNo ] = AddBoneInfo->Translate ;
									}
								}
								else
								{
									BoneInfo->KeyPos[ TimeNo ] = BoneInfo->Translate ;
								}

								if( BoneInfo->Base->Flag_AddRot && BoneInfo->Base->AddParentBoneIndex >= 0 )
								{
									AddBoneInfo = &PmxBoneInfo[ BoneInfo->Base->AddParentBoneIndex ] ;
									while( AddBoneInfo->Base->Flag_AddRot )
									{
										// �����̐e�Ǝ����������ꍇ�̓G���[�Ȃ̂Ŕ�����
										if( AddBoneInfo == &PmxBoneInfo[ AddBoneInfo->Base->AddParentBoneIndex ] )
										{
											break ;
										}

										AddBoneInfo = &PmxBoneInfo[ AddBoneInfo->Base->AddParentBoneIndex ] ;
									}

									if( BoneInfo->Base->AddRatio != 1.0f )
									{
										_MV1SphereLinear( &BoneInfo->Rotate, &AddBoneInfo->Rotate, BoneInfo->Base->AddRatio, &BoneInfo->KeyRot[ TimeNo ] ) ;
									}
									else
									{
										BoneInfo->KeyRot[ TimeNo ] = AddBoneInfo->Rotate ;
									}
								}
								else
								{
									BoneInfo->KeyRot[ TimeNo ] = BoneInfo->Rotate ;
								}
							}
							else
							{
								BoneInfo->KeyPos[ TimeNo ] = BoneInfo->Translate ;
								BoneInfo->KeyRot[ TimeNo ] = BoneInfo->Rotate ;
							}
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
								MV1LoadModelToVMD_InverseMatrix( ( ( PMX_READ_BONE_INFO * )BoneInfo->Frame->Parent->UserData )->LocalWorldMatrix, InvParentBoneLWM ) ;
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
			int PmxPhysicsNum ;

			// �����K���̃A�j���[�V�����f�[�^���Z�b�g����
			PmxPhysicsNum = MLPhysicsInfo->PmxPhysicsNum ;
			for( i = 0 ; i < PmxPhysicsNum ; i ++ )
			{
				if( CheckDisablePhysicsAnim_PMXPhysicsInfo( MLPhysicsInfo, i ) ) continue ;

				BoneInfo = MLPhysicsInfo->PmxPhysicsInfoDim[ i ].Bone ;

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
						DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x58\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\x4c\x88\x17\x52\xa2\x30\xcb\x30\xe1\x30\xfc\x30\xb7\x30\xe7\x30\xf3\x30\xad\x30\xfc\x30\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMX Load Error : �s��A�j���[�V�����L�[���i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
						goto ENDLABEL ;
					}
					if( ( DWORD )BoneInfo->Anim->AnimKeySetFirst->Num != VmdData.MaxTime * 2 + 1 )
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
							DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x58\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xa7\x5e\x19\x6a\xa2\x30\xcb\x30\xe1\x30\xfc\x30\xb7\x30\xe7\x30\xf3\x30\xad\x30\xfc\x30\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMX Load Error : ���W�A�j���[�V�����L�[���i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
							goto ENDLABEL ;
						}
						KeyPosSet->KeyTime = ( float * )ADDMEMAREA( sizeof( float ) * KeyPosSet->Num, &RModel->Mem ) ;
						if( KeyPosSet->KeyTime == NULL )
						{
							DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x58\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xa2\x30\xcb\x30\xe1\x30\xfc\x30\xb7\x30\xe7\x30\xf3\x30\xad\x30\xfc\x30\xbf\x30\xa4\x30\xe0\x30\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMX Load Error : �A�j���[�V�����L�[�^�C�����i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
							goto ENDLABEL ;
						}
						KeyMatrixSet->KeyTime = ( float * )ADDMEMAREA( sizeof( float ) * KeyMatrixSet->Num, &RModel->Mem ) ;
						if( KeyMatrixSet->KeyTime == NULL )
						{
							DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x58\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xa2\x30\xcb\x30\xe1\x30\xfc\x30\xb7\x30\xe7\x30\xf3\x30\xad\x30\xfc\x30\xbf\x30\xa4\x30\xe0\x30\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMX Load Error : �A�j���[�V�����L�[�^�C�����i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
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
					// �Ȃ��ꍇ�͂����Œǉ�

					BoneInfo->Anim = MV1RAddAnim( RModel, AnimSet ) ;
					if( BoneInfo->Anim == NULL )
					{
						DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x58\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\x4c\x88\x17\x52\xa2\x30\xcb\x30\xe1\x30\xfc\x30\xb7\x30\xe7\x30\xf3\x30\x6e\x30\xfd\x8f\xa0\x52\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMX Load Error : �s��A�j���[�V�����̒ǉ��Ɏ��s���܂���\n" @*/ )) ;
						goto ENDLABEL ;
					}

					// �Ώۃm�[�h�̃Z�b�g
					BoneInfo->Anim->TargetFrameIndex = BoneInfo->Frame->Index ;
					BoneInfo->Anim->MaxTime = ( float )VmdData.MaxTime ;

					// �A�j���[�V�����L�[�̏����Z�b�g
					KeyMatrixSet = MV1RAddAnimKeySet( RModel, BoneInfo->Anim ) ;
					if( KeyMatrixSet == NULL )
					{
						DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x58\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\x4c\x88\x17\x52\xa2\x30\xcb\x30\xe1\x30\xfc\x30\xb7\x30\xe7\x30\xf3\x30\xad\x30\xfc\x30\xbb\x30\xc3\x30\xc8\x30\x6e\x30\xfd\x8f\xa0\x52\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMX Load Error : �s��A�j���[�V�����L�[�Z�b�g�̒ǉ��Ɏ��s���܂���\n" @*/ )) ;
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
						DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x58\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\x4c\x88\x17\x52\xa2\x30\xcb\x30\xe1\x30\xfc\x30\xb7\x30\xe7\x30\xf3\x30\xad\x30\xfc\x30\xbf\x30\xa4\x30\xe0\x30\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMX Load Error : �s��A�j���[�V�����L�[�^�C�����i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
						goto ENDLABEL ;
					}
					KeyMatrixSet->KeyMatrix4x4C = ( MV1_ANIM_KEY_MATRIX4X4C * )ADDMEMAREA( sizeof( MV1_ANIM_KEY_MATRIX4X4C ) * KeyMatrixSet->Num, &RModel->Mem ) ;
					if( KeyMatrixSet->KeyMatrix4x4C == NULL )
					{
						DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x58\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\x4c\x88\x17\x52\xa2\x30\xcb\x30\xe1\x30\xfc\x30\xb7\x30\xe7\x30\xf3\x30\xad\x30\xfc\x30\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMX Load Error : �s��A�j���[�V�����L�[���i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
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

			BoneInfo = PmxBoneInfo ;
			for( i = 0 ; i < PmxBoneNum ; i ++, BoneInfo ++ )
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
						DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x58\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\x4c\x88\x17\x52\xa2\x30\xcb\x30\xe1\x30\xfc\x30\xb7\x30\xe7\x30\xf3\x30\xad\x30\xfc\x30\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMX Load Error : �s��A�j���[�V�����L�[���i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
						goto ENDLABEL ;
					}
					if( ( DWORD )BoneInfo->Anim->AnimKeySetFirst->Num != VmdData.MaxTime * 2 + 1 )
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
							DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x58\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xa7\x5e\x19\x6a\xa2\x30\xcb\x30\xe1\x30\xfc\x30\xb7\x30\xe7\x30\xf3\x30\xad\x30\xfc\x30\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMX Load Error : ���W�A�j���[�V�����L�[���i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
							goto ENDLABEL ;
						}
						KeyPosSet->KeyTime = ( float * )ADDMEMAREA( sizeof( float ) * KeyPosSet->Num, &RModel->Mem ) ;
						if( KeyPosSet->KeyTime == NULL )
						{
							DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x58\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xa2\x30\xcb\x30\xe1\x30\xfc\x30\xb7\x30\xe7\x30\xf3\x30\xad\x30\xfc\x30\xbf\x30\xa4\x30\xe0\x30\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMX Load Error : �A�j���[�V�����L�[�^�C�����i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
							goto ENDLABEL ;
						}
						KeyMatrixSet->KeyTime = ( float * )ADDMEMAREA( sizeof( float ) * KeyMatrixSet->Num, &RModel->Mem ) ;
						if( KeyMatrixSet->KeyTime == NULL )
						{
							DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x58\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xa2\x30\xcb\x30\xe1\x30\xfc\x30\xb7\x30\xe7\x30\xf3\x30\xad\x30\xfc\x30\xbf\x30\xa4\x30\xe0\x30\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMX Load Error : �A�j���[�V�����L�[�^�C�����i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
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
						DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x58\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\x4c\x88\x17\x52\xa2\x30\xcb\x30\xe1\x30\xfc\x30\xb7\x30\xe7\x30\xf3\x30\x6e\x30\xfd\x8f\xa0\x52\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMX Load Error : �s��A�j���[�V�����̒ǉ��Ɏ��s���܂���\n" @*/ )) ;
						goto ENDLABEL ;
					}

					// �Ώۃm�[�h�̃Z�b�g
					BoneInfo->Anim->TargetFrameIndex = BoneInfo->Frame->Index ;
					BoneInfo->Anim->MaxTime = ( float )VmdData.MaxTime ;

					// �A�j���[�V�����L�[�̏����Z�b�g
					KeyMatrixSet = MV1RAddAnimKeySet( RModel, BoneInfo->Anim ) ;
					if( KeyMatrixSet == NULL )
					{
						DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x58\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\x4c\x88\x17\x52\xa2\x30\xcb\x30\xe1\x30\xfc\x30\xb7\x30\xe7\x30\xf3\x30\xad\x30\xfc\x30\xbb\x30\xc3\x30\xc8\x30\x6e\x30\xfd\x8f\xa0\x52\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMX Load Error : �s��A�j���[�V�����L�[�Z�b�g�̒ǉ��Ɏ��s���܂���\n" @*/ )) ;
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
						DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x58\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\x4c\x88\x17\x52\xa2\x30\xcb\x30\xe1\x30\xfc\x30\xb7\x30\xe7\x30\xf3\x30\xad\x30\xfc\x30\xbf\x30\xa4\x30\xe0\x30\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMX Load Error : �s��A�j���[�V�����L�[�^�C�����i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
						goto ENDLABEL ;
					}
					KeyMatrixSet->KeyMatrix4x4C = ( MV1_ANIM_KEY_MATRIX4X4C * )ADDMEMAREA( sizeof( MV1_ANIM_KEY_MATRIX4X4C ) * KeyMatrixSet->Num, &RModel->Mem ) ;
					if( KeyMatrixSet->KeyMatrix4x4C == NULL )
					{
						DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x58\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\x4c\x88\x17\x52\xa2\x30\xcb\x30\xe1\x30\xfc\x30\xb7\x30\xe7\x30\xf3\x30\xad\x30\xfc\x30\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMX Load Error : �s��A�j���[�V�����L�[���i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
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
			BoneInfo = PmxBoneInfo ;
			for( i = 0 ; i < PmxBoneNum ; i ++, BoneInfo ++ )
			{
#ifndef DX_NON_BULLET_PHYSICS
				if( MLPhysicsInfo && BoneInfo->IsPhysics == TRUE && CheckDisablePhysicsAnim_PMXPhysicsInfo( MLPhysicsInfo, BoneInfo->PhysicsIndex ) == false ) continue ;
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
				DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x58\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xa2\x30\xcb\x30\xe1\x30\xfc\x30\xb7\x30\xe7\x30\xf3\x30\x6e\x30\xfd\x8f\xa0\x52\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMX Load Error : �A�j���[�V�����̒ǉ��Ɏ��s���܂���\n" @*/ )) ;
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
					DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x58\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xa2\x30\xcb\x30\xe1\x30\xfc\x30\xb7\x30\xe7\x30\xf3\x30\x68\x88\xc5\x60\xad\x30\xfc\x30\xbb\x30\xc3\x30\xc8\x30\x6e\x30\xfd\x8f\xa0\x52\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMX Load Error : �A�j���[�V�����\��L�[�Z�b�g�̒ǉ��Ɏ��s���܂���\n" @*/ )) ;
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
					DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x58\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xa2\x30\xcb\x30\xe1\x30\xfc\x30\xb7\x30\xe7\x30\xf3\x30\xad\x30\xfc\x30\xbf\x30\xa4\x30\xe0\x30\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMX Load Error : �A�j���[�V�����L�[�^�C�����i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
					goto ENDLABEL ;
				}
				KeyFactorSet->KeyLinear = ( float * )ADDMEMAREA( sizeof( float ) * KeyFactorSet->Num, &RModel->Mem ) ;
				if( KeyFactorSet->KeyLinear == NULL )
				{
					DXST_ERRORLOGFMT_ADDUTF16LE(( "\x50\x00\x4d\x00\x58\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xa2\x30\xcb\x30\xe1\x30\xfc\x30\xb7\x30\xe7\x30\xf3\x30\xad\x30\xfc\x30\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"PMX Load Error : �A�j���[�V�����L�[���i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
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
		ReleasePhysicsObject_PMXPhysicsInfo( MLPhysicsInfo ) ;

		BoneInfo = PmxBoneInfo ;
		for( i = 0 ; i < PmxBoneNum ; i ++, BoneInfo ++ )
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

	BoneInfo = PmxBoneInfo ;
	for( i = 0 ; i < PmxBoneNum ; i ++, BoneInfo ++ )
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
		ReleasePhysicsObject_PMXPhysicsInfo( MLPhysicsInfo ) ;

		BoneInfo = PmxBoneInfo ;
		for( i = 0 ; i < PmxBoneNum ; i ++, BoneInfo ++ )
		{
			if( BoneInfo->KeyMatrix )
			{
				DXFREE( BoneInfo->KeyMatrix ) ;
				BoneInfo->KeyMatrix = NULL ;
			}
		}
	}
#endif

	BoneInfo = PmxBoneInfo ;
	for( i = 0 ; i < PmxBoneNum ; i ++, BoneInfo ++ )
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


#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

#endif


