// -------------------------------------------------------------------------------
// 
// 		�c�w���C�u����		���f���f�[�^�ǂݍ��݃v���O����
// 
// 				Ver 3.16d
// 
// -------------------------------------------------------------------------------

#define __DX_MAKE

#include "DxModelRead.h"

#ifndef DX_NON_MODEL

// �C���N���[�h ---------------------------------
#include "DxLib.h"
#include "DxStatic.h"
#include "DxFile.h"
#include "DxModel.h"
#include "DxMemory.h"
#include "DxMath.h"
#include "DxLog.h"
#include "DxSystem.h"

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// �}�N����` -----------------------------------

// ���f�������X�g�ɒǉ�
#define MODELLIST_ADD( model, name )\
	if( model->name##First == NULL )\
	{\
		model->name##First = name ;\
		model->name##Last  = name ;\
	}\
	else\
	{\
		name->DataPrev = model->name##Last ;\
		model->name##Last->DataNext = name ;\
		model->name##Last = name;\
	}\
	name->Index = ( int )model->name##Num ;\
	model->name##Num ++ ;

// �I�u�W�F�N�g�����X�g�ɒǉ�
#define OBJLIST_ADD( obj, name )\
	if( obj->name##First == NULL )\
	{\
		obj->name##First = name ;\
		obj->name##Last  = name ;\
	}\
	else\
	{\
		name->Prev = obj->name##Last ;\
		obj->name##Last->Next = name ;\
		obj->name##Last = name;\
	}\
	obj->name##Num ++ ;

// �A�j���[�V�����L�[�Z�b�g�̃����N���O��
#define KEYSETLIST_SUB( keyset, motion, readmodel )\
	if( keyset->Next )     keyset->Next->Prev         = keyset->Prev ;\
	else                   motion->AnimKeySetLast     = keyset->Prev ;\
\
	if( keyset->Prev )     keyset->Prev->Next         = keyset->Next ;\
	else                   motion->AnimKeySetFirst    = keyset->Next ;\
\
	if( keyset->DataNext ) keyset->DataNext->DataPrev = keyset->DataPrev ;\
	else                   readmodel->AnimKeySetLast  = keyset->DataPrev ;\
\
	if( keyset->DataPrev ) keyset->DataPrev->DataNext = keyset->DataNext ;\
	else                   readmodel->AnimKeySetFirst = keyset->DataNext ;\
\
	motion->AnimKeySetNum -- ;\
	readmodel->AnimKeySetNum -- ;


// �f�[�^�^�錾 ---------------------------------

// �e�X�g�ʒu���̃E�G�C�g���
struct MV1_TESTPOSITIONINFO_SKINWEIGHT
{
	MV1_SKIN_WEIGHT_R *Orig ;
	int BoneNumber ;
	float Weight ;
} ;


// �f�[�^��` -----------------------------------

// 8x8 �� tga�e�N�X�`��
BYTE Tga8x8TextureFileImage[ 84 ] = 
{
	0x00, 0x00, 0x0A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x08, 0x00,
	0x20, 0x08, 0x87, 0xFF, 0xFF, 0xFF, 0xFF, 0x87, 0xFF, 0xFF, 0xFF, 0xFF, 0x87, 0xFF, 0xFF, 0xFF,
	0xFF, 0x87, 0xFF, 0xFF, 0xFF, 0xFF, 0x87, 0xFF, 0xFF, 0xFF, 0xFF, 0x87, 0xFF, 0xFF, 0xFF, 0xFF,
	0x87, 0xFF, 0xFF, 0xFF, 0xFF, 0x87, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x54, 0x52, 0x55, 0x45, 0x56, 0x49, 0x53, 0x49, 0x4F, 0x4E, 0x2D, 0x58, 0x46, 0x49,
	0x4C, 0x45, 0x2E, 0x00
} ;

BYTE Tga8x8BlackTextureFileImage[ 76 ] =
{
	0x00,0x00,0x0a,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x00,0x08,0x00,
	0x18,0x00,0x87,0x00,0x00,0x00,0x87,0x00,0x00,0x00,0x87,0x00,0x00,0x00,0x87,0x00,
	0x00,0x00,0x87,0x00,0x00,0x00,0x87,0x00,0x00,0x00,0x87,0x00,0x00,0x00,0x87,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x54,0x52,0x55,0x45,0x56,0x49,
	0x53,0x49,0x4f,0x4e,0x2d,0x58,0x46,0x49,0x4c,0x45,0x2e,0x00,
} ;

// 256x8 �̉��g�D�[�������_�����O�p tga�e�N�X�`��
BYTE Tga256x8ToonTextureFileImage[ 172 ] =
{
	0x00,0x00,0x0a,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x08,0x00,
	0x18,0x00,0xbf,0x00,0x00,0x00,0xbf,0x55,0x55,0x55,0xbf,0xaa,0xaa,0xaa,0xbf,0xff,
	0xff,0xff,0xbf,0x00,0x00,0x00,0xbf,0x55,0x55,0x55,0xbf,0xaa,0xaa,0xaa,0xbf,0xff,
	0xff,0xff,0xbf,0x00,0x00,0x00,0xbf,0x55,0x55,0x55,0xbf,0xaa,0xaa,0xaa,0xbf,0xff,
	0xff,0xff,0xbf,0x00,0x00,0x00,0xbf,0x55,0x55,0x55,0xbf,0xaa,0xaa,0xaa,0xbf,0xff,
	0xff,0xff,0xbf,0x00,0x00,0x00,0xbf,0x55,0x55,0x55,0xbf,0xaa,0xaa,0xaa,0xbf,0xff,
	0xff,0xff,0xbf,0x00,0x00,0x00,0xbf,0x55,0x55,0x55,0xbf,0xaa,0xaa,0xaa,0xbf,0xff,
	0xff,0xff,0xbf,0x00,0x00,0x00,0xbf,0x55,0x55,0x55,0xbf,0xaa,0xaa,0xaa,0xbf,0xff,
	0xff,0xff,0xbf,0x00,0x00,0x00,0xbf,0x55,0x55,0x55,0xbf,0xaa,0xaa,0xaa,0xbf,0xff,
	0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x54,0x52,0x55,0x45,0x56,0x49,
	0x53,0x49,0x4f,0x4e,0x2d,0x58,0x46,0x49,0x4c,0x45,0x2e,0x00
} ;

// �f�t�H���g�̃f�B�t���[�Y�O���f�[�V����
BYTE TgaDiffuseDefaultGradFileImage[ 172/*156*/ ] =
{
	0x00,0x00,0x0a,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x08,0x00,
	0x18,0x00,0xbf,0x00,0x00,0x00,0xbf,0x55,0x55,0x55,0xbf,0xaa,0xaa,0xaa,0xbf,0xff,
	0xff,0xff,0xbf,0x00,0x00,0x00,0xbf,0x55,0x55,0x55,0xbf,0xaa,0xaa,0xaa,0xbf,0xff,
	0xff,0xff,0xbf,0x00,0x00,0x00,0xbf,0x55,0x55,0x55,0xbf,0xaa,0xaa,0xaa,0xbf,0xff,
	0xff,0xff,0xbf,0x00,0x00,0x00,0xbf,0x55,0x55,0x55,0xbf,0xaa,0xaa,0xaa,0xbf,0xff,
	0xff,0xff,0xbf,0x00,0x00,0x00,0xbf,0x55,0x55,0x55,0xbf,0xaa,0xaa,0xaa,0xbf,0xff,
	0xff,0xff,0xbf,0x00,0x00,0x00,0xbf,0x55,0x55,0x55,0xbf,0xaa,0xaa,0xaa,0xbf,0xff,
	0xff,0xff,0xbf,0x00,0x00,0x00,0xbf,0x55,0x55,0x55,0xbf,0xaa,0xaa,0xaa,0xbf,0xff,
	0xff,0xff,0xbf,0x00,0x00,0x00,0xbf,0x55,0x55,0x55,0xbf,0xaa,0xaa,0xaa,0xbf,0xff,
	0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x54,0x52,0x55,0x45,0x56,0x49,
	0x53,0x49,0x4f,0x4e,0x2d,0x58,0x46,0x49,0x4c,0x45,0x2e,0x00
/*
	0x00,0x01,0x09,0x00,0x00,0x10,0x00,0x18,0x00,0x00,0x00,0x00,0x40,0x00,0x08,0x00,
	0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x00,0x80,0x00,0x00,0x80,0x80,0x80,0x00,
	0x00,0x80,0x00,0x80,0x80,0x80,0x00,0x80,0x80,0x80,0xc0,0xc0,0xc0,0x00,0x00,0xff,
	0x00,0xff,0x00,0x00,0xff,0xff,0xff,0x00,0x00,0xff,0x00,0xff,0xff,0xff,0x00,0xff,
	0xff,0xff,0x87,0x00,0x8c,0x07,0x96,0x08,0x93,0x0f,0x87,0x00,0x8c,0x07,0x96,0x08,
	0x93,0x0f,0x87,0x00,0x8c,0x07,0x96,0x08,0x93,0x0f,0x87,0x00,0x8c,0x07,0x96,0x08,
	0x93,0x0f,0x87,0x00,0x8c,0x07,0x96,0x08,0x93,0x0f,0x87,0x00,0x8c,0x07,0x96,0x08,
	0x93,0x0f,0x87,0x00,0x8c,0x07,0x96,0x08,0x93,0x0f,0x87,0x00,0x8c,0x07,0x96,0x08,
	0x93,0x0f,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x54,0x52,0x55,0x45,0x56,0x49,
	0x53,0x49,0x4f,0x4e,0x2d,0x58,0x46,0x49,0x4c,0x45,0x2e,0x00,
*/
} ;

// �f�t�H���g�̃X�y�L�����O���f�[�V����
BYTE TgaSpecularDefaultGradFileImage[ 172/*124*/ ] =
{
	0x00,0x00,0x0a,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x08,0x00,
	0x18,0x00,0xbf,0x00,0x00,0x00,0xbf,0x55,0x55,0x55,0xbf,0xaa,0xaa,0xaa,0xbf,0xff,
	0xff,0xff,0xbf,0x00,0x00,0x00,0xbf,0x55,0x55,0x55,0xbf,0xaa,0xaa,0xaa,0xbf,0xff,
	0xff,0xff,0xbf,0x00,0x00,0x00,0xbf,0x55,0x55,0x55,0xbf,0xaa,0xaa,0xaa,0xbf,0xff,
	0xff,0xff,0xbf,0x00,0x00,0x00,0xbf,0x55,0x55,0x55,0xbf,0xaa,0xaa,0xaa,0xbf,0xff,
	0xff,0xff,0xbf,0x00,0x00,0x00,0xbf,0x55,0x55,0x55,0xbf,0xaa,0xaa,0xaa,0xbf,0xff,
	0xff,0xff,0xbf,0x00,0x00,0x00,0xbf,0x55,0x55,0x55,0xbf,0xaa,0xaa,0xaa,0xbf,0xff,
	0xff,0xff,0xbf,0x00,0x00,0x00,0xbf,0x55,0x55,0x55,0xbf,0xaa,0xaa,0xaa,0xbf,0xff,
	0xff,0xff,0xbf,0x00,0x00,0x00,0xbf,0x55,0x55,0x55,0xbf,0xaa,0xaa,0xaa,0xbf,0xff,
	0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x54,0x52,0x55,0x45,0x56,0x49,
	0x53,0x49,0x4f,0x4e,0x2d,0x58,0x46,0x49,0x4c,0x45,0x2e,0x00
/*
	0x00,0x01,0x09,0x00,0x00,0x10,0x00,0x18,0x00,0x00,0x00,0x00,0x40,0x00,0x08,0x00,
	0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x00,0x80,0x00,0x00,0x80,0x80,0x80,0x00,
	0x00,0x80,0x00,0x80,0x80,0x80,0x00,0x80,0x80,0x80,0xc0,0xc0,0xc0,0x00,0x00,0xff,
	0x00,0xff,0x00,0x00,0xff,0xff,0xff,0x00,0x00,0xff,0x00,0xff,0xff,0xff,0x00,0xff,
	0xff,0xff,0x9f,0x00,0x9f,0x0f,0x9f,0x00,0x9f,0x0f,0x9f,0x00,0x9f,0x0f,0x9f,0x00,
	0x9f,0x0f,0x9f,0x00,0x9f,0x0f,0x9f,0x00,0x9f,0x0f,0x9f,0x00,0x9f,0x0f,0x9f,0x00,
	0x9f,0x0f,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x54,0x52,0x55,0x45,0x56,0x49,
	0x53,0x49,0x4f,0x4e,0x2d,0x58,0x46,0x49,0x4c,0x45,0x2e,0x00,
*/
} ;

// �֐��錾 -------------------------------------

#ifndef UNICODE
static char    *MV1RGetStringSpace(  MV1_MODEL_BASE *MBase, const char    *String ) ;					// ���f����{�f�[�^�p�̕�����X�y�[�X���m�ۂ���( -1:�G���[ )
#endif
static wchar_t *MV1RGetStringSpaceW( MV1_MODEL_BASE *MBase, const wchar_t *String ) ;					// ���f����{�f�[�^�p�̕�����X�y�[�X���m�ۂ���( -1:�G���[ )
static bool MV1ConvertTrianglePolygon( MV1_MODEL_R *ReadModel, MV1_MESH_R *Mesh ) ;						// ���p�`�|���S�����O�p�`�|���S���ɕϊ�����
static bool MV1NotEqualNormalSideAddZeroAreaPolygon( MV1_MODEL_R *ReadModel, MV1_MESH_R *Mesh ) ;		// �ӂ����L���Ă��āA���@���̕������قȂ�ӂɖʐςO�̃|���S����ǉ�����( �R�p�`�|���S���̏W���ɂȂ��Ă���K�v���� )
static bool MV1OptimizePosition( MV1_MODEL_R *ReadModel, MV1_MESH_R *Mesh ) ;							// ���W�A�X�L�j���O�E�G�C�g�l���S������̒��_�𓝍�����
static bool MV1MakeMeshNormals( MV1_MODEL_R *ReadModel, MV1_MESH_R *Mesh ) ;							// ���b�V���@���̎����v�Z( �S�|���S�����O�p�`�|���S���ł���K�v������܂� )
//static bool MV1MakeMeshBinormalsAndTangents( MV1_MODEL_R *ReadModel, MV1_MESH_R *Mesh ) ;				// ���b�V���̏]�@���Ɛڐ����v�Z����( �S�|���S�����O�p�`�|���S���ł���K�v������܂� )
static bool MV1OptimizeSkinBoneInfo( MV1_MODEL_BASE *Model ) ;											// ���f���f�[�^�̃X�L�j���O���b�V���p�̃{�[�������œK������
static bool MV1OptimizeAnim( MV1_MODEL_R *ReadModel ) ;													// �A�j���[�V�����f�[�^���œK������
static void MV1OptimizeKeySetTimeType( MV1_MODEL_R *ReadModel, MV1_ANIMKEYSET_R *KeySetR, bool NotKeySub = false ) ;	// �L�[�Z�b�g�̃L�[���œK������
static bool MatrixRotateCmp( MATRIX *In1, MATRIX *In2, float f ) ;
static void MV1MultiplyRotateMatrix( MATRIX *DestBuffer, VECTOR *Rotate, int RotateOrder ) ;			// ��]�s�����Z����

// �v���O���� -----------------------------------

static bool MatrixRotateCmp( MATRIX *In1, MATRIX *In2, float f )
{
	return In1->m[ 0 ][ 0 ] > In2->m[ 0 ][ 0 ] - f &&
			In1->m[ 0 ][ 0 ] < In2->m[ 0 ][ 0 ] + f &&
			In1->m[ 0 ][ 1 ] > In2->m[ 0 ][ 1 ] - f &&
			In1->m[ 0 ][ 1 ] < In2->m[ 0 ][ 1 ] + f &&
			In1->m[ 0 ][ 2 ] > In2->m[ 0 ][ 2 ] - f &&
			In1->m[ 0 ][ 2 ] < In2->m[ 0 ][ 2 ] + f &&
			In1->m[ 1 ][ 0 ] > In2->m[ 1 ][ 0 ] - f &&
			In1->m[ 1 ][ 0 ] < In2->m[ 1 ][ 0 ] + f &&
			In1->m[ 1 ][ 1 ] > In2->m[ 1 ][ 1 ] - f &&
			In1->m[ 1 ][ 1 ] < In2->m[ 1 ][ 1 ] + f &&
			In1->m[ 1 ][ 2 ] > In2->m[ 1 ][ 2 ] - f &&
			In1->m[ 1 ][ 2 ] < In2->m[ 1 ][ 2 ] + f &&
			In1->m[ 2 ][ 0 ] > In2->m[ 2 ][ 0 ] - f &&
			In1->m[ 2 ][ 0 ] < In2->m[ 2 ][ 0 ] + f &&
			In1->m[ 2 ][ 1 ] > In2->m[ 2 ][ 1 ] - f &&
			In1->m[ 2 ][ 1 ] < In2->m[ 2 ][ 1 ] + f &&
			In1->m[ 2 ][ 2 ] > In2->m[ 2 ][ 2 ] - f &&
			In1->m[ 2 ][ 2 ] < In2->m[ 2 ][ 2 ] + f ;
}

// ���p�`�|���S�����O�p�`�|���S���ɕϊ�����
static bool MV1ConvertTrianglePolygon( MV1_MODEL_R *ReadModel, MV1_MESH_R *Mesh )
{
	int AddFaceCount, AddNum, i, j, k, p1, p2, pt ;
	MV1_MESHFACE_R *NewFace, *FaceT, *NewFaceT, *NewFaceT2 ;
	DWORD *NewIndex, *NewIndexT ;
	DWORD NewFaceNum ;

	// ���₳�Ȃ���΂Ȃ�Ȃ��ʃf�[�^�̐��𒲂ׂ�
	AddFaceCount = 0 ;
	FaceT = Mesh->Faces ;
	for( i = 0 ; ( DWORD )i < Mesh->FaceNum ; i ++, FaceT ++ )
	{
		AddFaceCount += FaceT->IndexNum - 3 ;
	}

	// ���p�`�ʂ������ꍇ�͂����ŏI��
	if( AddFaceCount == 0 )
		return true ;

	// ���₵���ʂ��i�[���郁�����̈�̊m��
	NewFaceNum = Mesh->FaceNum + AddFaceCount ;
	NewFace = ( MV1_MESHFACE_R * )ADDMEMAREA( ( sizeof( MV1_MESHFACE_R ) + ( 3 + MV1_READ_MAX_UV_NUM ) * 3 * sizeof( DWORD ) ) * NewFaceNum, &ReadModel->Mem ) ;
	if( NewFace == NULL )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\x52\x00\x65\x00\x61\x00\x64\x00\x20\x00\x4d\x00\x6f\x00\x64\x00\x65\x00\x6c\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\x13\xff\xdd\x30\xea\x30\x09\x59\xdb\x63\x8c\x5f\x6e\x30\xc7\x30\xfc\x30\xbf\x30\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Read Model Error : �R�|���ϊ���̃f�[�^���i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
		return false ;
	}
	NewIndex = ( DWORD * )( NewFace + NewFaceNum ) ;

	// �C���f�b�N�X�f�[�^�̃A�h���X���Z�b�g�A�b�v
	NewFaceT = NewFace ;
	NewIndexT = NewIndex ;
	for( i = 0 ; i < ( int )NewFaceNum ; i ++, NewFaceT ++ )
	{
		NewFaceT->VertexIndex = NewIndexT ; NewIndexT += 3 ;
		NewFaceT->NormalIndex = NewIndexT ; NewIndexT += 3 ;
		NewFaceT->VertexColorIndex = NewIndexT ; NewIndexT += 3 ;
		for( j = 0 ; j < MV1_READ_MAX_UV_NUM ; j ++, NewIndexT += 3 )
			NewFaceT->UVIndex[ j ] = NewIndexT ;
	}

	// ���܂ł̖ʂ̏����R�s�[
	FaceT = Mesh->Faces ;
	NewFaceT = NewFace ;
	for( i = 0 ; i < ( int )Mesh->FaceNum ; i ++, FaceT ++, NewFaceT ++ )
	{
		NewFaceT->IndexNum = 3 ;
		NewFaceT->PolygonNum = 1 ;

		for( j = 0 ; j < 3 ; j ++ )
		{
			NewFaceT->VertexIndex[ j ] = FaceT->VertexIndex[ j ] ;
			NewFaceT->NormalIndex[ j ] = FaceT->NormalIndex[ j ] ;
			NewFaceT->VertexColorIndex[ j ] = FaceT->VertexColorIndex[ j ] ;
			for( k = 0 ; k < MV1_READ_MAX_UV_NUM ; k ++ )
			{
				NewFaceT->UVIndex[ k ][ j ] = FaceT->UVIndex[ k ][ j ] ;
			}
		}

		NewFaceT->MaterialIndex = FaceT->MaterialIndex ;
		NewFaceT->Normal = FaceT->Normal ;
	}

	// ���p�`�ʂ��O�p�`�ʂɕϊ�
	FaceT = Mesh->Faces ;
	NewFaceT2 = NewFace + Mesh->FaceNum ;
	NewFaceT = NewFace ;
	for( i = 0 ; ( DWORD )i < Mesh->FaceNum ; i ++, NewFaceT ++, FaceT ++ )
	{
		if( FaceT->IndexNum == 4 )
		{
			VECTOR OutVec[ 3 ] ;

			// �O�p�`( 0, 2, 3 ) �̓����ɒ��_1�����邩�`�F�b�N
			OutVec[ 0 ] = VCross(
				VSub( Mesh->Positions[ FaceT->VertexIndex[ 0 ] ], Mesh->Positions[ FaceT->VertexIndex[ 2 ] ] ),
				VSub( Mesh->Positions[ FaceT->VertexIndex[ 2 ] ], Mesh->Positions[ FaceT->VertexIndex[ 1 ] ] )
			) ;
			OutVec[ 1 ] = VCross(
				VSub( Mesh->Positions[ FaceT->VertexIndex[ 2 ] ], Mesh->Positions[ FaceT->VertexIndex[ 3 ] ] ),
				VSub( Mesh->Positions[ FaceT->VertexIndex[ 3 ] ], Mesh->Positions[ FaceT->VertexIndex[ 1 ] ] )
			) ;
			OutVec[ 2 ] = VCross(
				VSub( Mesh->Positions[ FaceT->VertexIndex[ 3 ] ], Mesh->Positions[ FaceT->VertexIndex[ 0 ] ] ),
				VSub( Mesh->Positions[ FaceT->VertexIndex[ 0 ] ], Mesh->Positions[ FaceT->VertexIndex[ 1 ] ] )
			) ;
			if( VDot( OutVec[ 0 ], OutVec[ 1 ] ) > 0.0f &&
				VDot( OutVec[ 0 ], OutVec[ 2 ] ) > 0.0f )
			{
				// �O�p�`( 0, 2, 3 ) �̓����ɒ��_1������ꍇ�̏���
				NewFaceT2->IndexNum = 3 ;
				NewFaceT2->PolygonNum = 1 ;
				NewFaceT2->VertexIndex[ 0 ] = FaceT->VertexIndex[ 0 ] ;
				NewFaceT2->VertexIndex[ 1 ] = FaceT->VertexIndex[ 1 ] ;
				NewFaceT2->VertexIndex[ 2 ] = FaceT->VertexIndex[ 3 ] ;
				NewFaceT2->NormalIndex[ 0 ] = FaceT->NormalIndex[ 0 ] ;
				NewFaceT2->NormalIndex[ 1 ] = FaceT->NormalIndex[ 1 ] ;
				NewFaceT2->NormalIndex[ 2 ] = FaceT->NormalIndex[ 3 ] ;
				NewFaceT2->VertexColorIndex[ 0 ] = FaceT->VertexColorIndex[ 0 ] ;
				NewFaceT2->VertexColorIndex[ 1 ] = FaceT->VertexColorIndex[ 1 ] ;
				NewFaceT2->VertexColorIndex[ 2 ] = FaceT->VertexColorIndex[ 3 ] ;
				for( k = 0 ; ( DWORD )k < MV1_READ_MAX_UV_NUM ; k ++ )
				{
					NewFaceT2->UVIndex[ k ][ 0 ] = FaceT->UVIndex[ k ][ 0 ] ;
					NewFaceT2->UVIndex[ k ][ 1 ] = FaceT->UVIndex[ k ][ 1 ] ;
					NewFaceT2->UVIndex[ k ][ 2 ] = FaceT->UVIndex[ k ][ 3 ] ;
				}
				NewFaceT2->MaterialIndex = FaceT->MaterialIndex ;
				NewFaceT2 ++ ;

				NewFaceT->VertexIndex[ 0 ] = FaceT->VertexIndex[ 1 ] ;
				NewFaceT->VertexIndex[ 1 ] = FaceT->VertexIndex[ 2 ] ;
				NewFaceT->VertexIndex[ 2 ] = FaceT->VertexIndex[ 3 ] ;
				NewFaceT->NormalIndex[ 0 ] = FaceT->NormalIndex[ 1 ] ;
				NewFaceT->NormalIndex[ 1 ] = FaceT->NormalIndex[ 2 ] ;
				NewFaceT->NormalIndex[ 2 ] = FaceT->NormalIndex[ 3 ] ;
				NewFaceT->VertexColorIndex[ 0 ] = FaceT->VertexColorIndex[ 1 ] ;
				NewFaceT->VertexColorIndex[ 1 ] = FaceT->VertexColorIndex[ 2 ] ;
				NewFaceT->VertexColorIndex[ 2 ] = FaceT->VertexColorIndex[ 3 ] ;
				for( k = 0 ; ( DWORD )k < MV1_READ_MAX_UV_NUM ; k ++ )
				{
					NewFaceT->UVIndex[ k ][ 0 ] = FaceT->UVIndex[ k ][ 1 ] ;
					NewFaceT->UVIndex[ k ][ 1 ] = FaceT->UVIndex[ k ][ 2 ] ;
					NewFaceT->UVIndex[ k ][ 2 ] = FaceT->UVIndex[ k ][ 3 ] ;
				}
				continue ;
			}

			// �O�p�`( 0, 1, 2 ) �̓����ɒ��_3�����邩�`�F�b�N
			OutVec[ 0 ] = VCross(
				VSub( Mesh->Positions[ FaceT->VertexIndex[ 0 ] ], Mesh->Positions[ FaceT->VertexIndex[ 1 ] ] ),
				VSub( Mesh->Positions[ FaceT->VertexIndex[ 1 ] ], Mesh->Positions[ FaceT->VertexIndex[ 3 ] ] )
			) ;
			OutVec[ 1 ] = VCross(
				VSub( Mesh->Positions[ FaceT->VertexIndex[ 1 ] ], Mesh->Positions[ FaceT->VertexIndex[ 2 ] ] ),
				VSub( Mesh->Positions[ FaceT->VertexIndex[ 2 ] ], Mesh->Positions[ FaceT->VertexIndex[ 3 ] ] )
			) ;
			OutVec[ 2 ] = VCross(
				VSub( Mesh->Positions[ FaceT->VertexIndex[ 2 ] ], Mesh->Positions[ FaceT->VertexIndex[ 0 ] ] ),
				VSub( Mesh->Positions[ FaceT->VertexIndex[ 0 ] ], Mesh->Positions[ FaceT->VertexIndex[ 3 ] ] )
			) ;
			if( VDot( OutVec[ 0 ], OutVec[ 1 ] ) > 0.0f &&
				VDot( OutVec[ 0 ], OutVec[ 2 ] ) > 0.0f )
			{
				// �O�p�`( 0, 1, 2 ) �̓����ɒ��_3������ꍇ�̏���
				NewFaceT2->IndexNum = 3 ;
				NewFaceT2->PolygonNum = 1 ;
				NewFaceT2->VertexIndex[ 0 ] = FaceT->VertexIndex[ 0 ] ;
				NewFaceT2->VertexIndex[ 1 ] = FaceT->VertexIndex[ 1 ] ;
				NewFaceT2->VertexIndex[ 2 ] = FaceT->VertexIndex[ 3 ] ;
				NewFaceT2->NormalIndex[ 0 ] = FaceT->NormalIndex[ 0 ] ;
				NewFaceT2->NormalIndex[ 1 ] = FaceT->NormalIndex[ 1 ] ;
				NewFaceT2->NormalIndex[ 2 ] = FaceT->NormalIndex[ 3 ] ;
				NewFaceT2->VertexColorIndex[ 0 ] = FaceT->VertexColorIndex[ 0 ] ;
				NewFaceT2->VertexColorIndex[ 1 ] = FaceT->VertexColorIndex[ 1 ] ;
				NewFaceT2->VertexColorIndex[ 2 ] = FaceT->VertexColorIndex[ 3 ] ;
				for( k = 0 ; ( DWORD )k < MV1_READ_MAX_UV_NUM ; k ++ )
				{
					NewFaceT2->UVIndex[ k ][ 0 ] = FaceT->UVIndex[ k ][ 0 ] ;
					NewFaceT2->UVIndex[ k ][ 1 ] = FaceT->UVIndex[ k ][ 1 ] ;
					NewFaceT2->UVIndex[ k ][ 2 ] = FaceT->UVIndex[ k ][ 3 ] ;
				}
				NewFaceT2->MaterialIndex = FaceT->MaterialIndex ;
				NewFaceT2 ++ ;

				NewFaceT->VertexIndex[ 0 ] = FaceT->VertexIndex[ 1 ] ;
				NewFaceT->VertexIndex[ 1 ] = FaceT->VertexIndex[ 2 ] ;
				NewFaceT->VertexIndex[ 2 ] = FaceT->VertexIndex[ 3 ] ;
				NewFaceT->NormalIndex[ 0 ] = FaceT->NormalIndex[ 1 ] ;
				NewFaceT->NormalIndex[ 1 ] = FaceT->NormalIndex[ 2 ] ;
				NewFaceT->NormalIndex[ 2 ] = FaceT->NormalIndex[ 3 ] ;
				NewFaceT->VertexColorIndex[ 0 ] = FaceT->VertexColorIndex[ 1 ] ;
				NewFaceT->VertexColorIndex[ 1 ] = FaceT->VertexColorIndex[ 2 ] ;
				NewFaceT->VertexColorIndex[ 2 ] = FaceT->VertexColorIndex[ 3 ] ;
				for( k = 0 ; ( DWORD )k < MV1_READ_MAX_UV_NUM ; k ++ )
				{
					NewFaceT->UVIndex[ k ][ 0 ] = FaceT->UVIndex[ k ][ 1 ] ;
					NewFaceT->UVIndex[ k ][ 1 ] = FaceT->UVIndex[ k ][ 2 ] ;
					NewFaceT->UVIndex[ k ][ 2 ] = FaceT->UVIndex[ k ][ 3 ] ;
				}
				continue ;
			}
		}

		AddNum = ( int )( FaceT->IndexNum - 3 ) ;
		if( AddNum == 0 ) continue ;

		p1 = 0 ;
		p2 = 2 ;
		j = 0 ;
		for(;;)
		{
			pt = ( int )( p1 - 1 == -1 ? FaceT->IndexNum - 1 : p1 - 1 ) ;
			NewFaceT2->IndexNum = 3 ;
			NewFaceT2->PolygonNum = 1 ;
			NewFaceT2->VertexIndex[ 0 ] = FaceT->VertexIndex[ p1 ] ;
			NewFaceT2->VertexIndex[ 1 ] = FaceT->VertexIndex[ p2 ] ;
			NewFaceT2->VertexIndex[ 2 ] = FaceT->VertexIndex[ pt ] ;
			NewFaceT2->NormalIndex[ 0 ] = FaceT->NormalIndex[ p1 ] ;
			NewFaceT2->NormalIndex[ 1 ] = FaceT->NormalIndex[ p2 ] ;
			NewFaceT2->NormalIndex[ 2 ] = FaceT->NormalIndex[ pt ] ;
			NewFaceT2->VertexColorIndex[ 0 ] = FaceT->VertexColorIndex[ p1 ] ;
			NewFaceT2->VertexColorIndex[ 1 ] = FaceT->VertexColorIndex[ p2 ] ;
			NewFaceT2->VertexColorIndex[ 2 ] = FaceT->VertexColorIndex[ pt ] ;
			for( k = 0 ; ( DWORD )k < MV1_READ_MAX_UV_NUM ; k ++ )
			{
				NewFaceT2->UVIndex[ k ][ 0 ] = FaceT->UVIndex[ k ][ p1 ] ;
				NewFaceT2->UVIndex[ k ][ 1 ] = FaceT->UVIndex[ k ][ p2 ] ;
				NewFaceT2->UVIndex[ k ][ 2 ] = FaceT->UVIndex[ k ][ pt ] ;
			}
			NewFaceT2->MaterialIndex = FaceT->MaterialIndex ;

			p1 -- ;
			if( p1 == -1 ) p1 = ( int )( FaceT->IndexNum - 1 ) ;
			j ++ ;
			NewFaceT2 ++ ;
			if( AddNum == j ) break ;

			pt = p2 + 1 ;
			NewFaceT2->IndexNum = 3 ;
			NewFaceT2->PolygonNum = 1 ;
			NewFaceT2->VertexIndex[ 0 ] = FaceT->VertexIndex[ p1 ] ;
			NewFaceT2->VertexIndex[ 1 ] = FaceT->VertexIndex[ p2 ] ;
			NewFaceT2->VertexIndex[ 2 ] = FaceT->VertexIndex[ pt ] ;
			NewFaceT2->NormalIndex[ 0 ] = FaceT->NormalIndex[ p1 ] ;
			NewFaceT2->NormalIndex[ 1 ] = FaceT->NormalIndex[ p2 ] ;
			NewFaceT2->NormalIndex[ 2 ] = FaceT->NormalIndex[ pt ] ;
			NewFaceT2->VertexColorIndex[ 0 ] = FaceT->VertexColorIndex[ p1 ] ;
			NewFaceT2->VertexColorIndex[ 1 ] = FaceT->VertexColorIndex[ p2 ] ;
			NewFaceT2->VertexColorIndex[ 2 ] = FaceT->VertexColorIndex[ pt ] ;
			for( k = 0 ; ( DWORD )k < MV1_READ_MAX_UV_NUM ; k ++ )
			{
				NewFaceT2->UVIndex[ k ][ 0 ] = FaceT->UVIndex[ k ][ p1 ] ;
				NewFaceT2->UVIndex[ k ][ 1 ] = FaceT->UVIndex[ k ][ p2 ] ;
				NewFaceT2->UVIndex[ k ][ 2 ] = FaceT->UVIndex[ k ][ pt ] ;
			}
			NewFaceT2->MaterialIndex = FaceT->MaterialIndex ;

			p2 ++ ;
			j ++ ;
			NewFaceT2 ++ ;
			if( AddNum == j ) break ;
		}
	}

	// �ʂ̐��𑝂₷
	Mesh->FaceNum += AddFaceCount ;

	// �������̉���ƍ����ւ�
	SUBMEMAREA( &ReadModel->Mem, Mesh->Faces );
	Mesh->Faces = NewFace ;
	Mesh->FaceIndexBuffer = NewIndex ;

	// ����I��
	return true ;
}

// �ӂ����L���Ă��āA���@���̕������قȂ�ӂɖʐςO�̃|���S����ǉ�����( �R�p�`�|���S���̏W���ɂȂ��Ă���K�v���� )
static bool MV1NotEqualNormalSideAddZeroAreaPolygon( MV1_MODEL_R *ReadModel, MV1_MESH_R *Mesh )
{
	int AddFaceCount ;
	int i, j, k ;
	MV1_MESHFACE_R *NewFace ;
	MV1_MESHFACE_R *FaceT ;
	MV1_MESHFACE_R *FaceT2 ;
	MV1_MESHFACE_R *NewFaceT ;
	MV1_MESHFACE_R *NewFaceT2 ;
	DWORD *NewIndex, *NewIndexT ;
	DWORD NewFaceNum ;
	DWORD TargetSideNum ;
	DWORD *TargetFace1 ;
	DWORD *TargetFace2 ;
	DWORD (*TargetFace1VIndex)[ 2 ] ;
	DWORD (*TargetFace2VIndex)[ 2 ] ;

	// ��Ɨp�̃������̈���m��
	TargetFace1 = ( DWORD * )DXALLOC( ( sizeof( DWORD ) * 6 ) * Mesh->FaceNum * 3 ) ;
	if( TargetFace1 == NULL )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\x52\x00\x65\x00\x61\x00\x64\x00\x20\x00\x4d\x00\x6f\x00\x64\x00\x65\x00\x6c\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\x2e\x7e\x00\x90\xdd\x30\xea\x30\xb4\x30\xf3\x30\xfd\x8f\xa0\x52\x5c\x4f\x6d\x69\x28\x75\x6e\x30\xe1\x30\xe2\x30\xea\x30\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Read Model Error : �k�ރ|���S���ǉ���Ɨp�̃������̊m�ۂɎ��s���܂���\n" @*/ )) ;
		return false ;
	}
	TargetFace2 = TargetFace1 + Mesh->FaceNum * 3 ;
	TargetFace1VIndex = ( DWORD (*)[ 2 ] )( TargetFace2 + Mesh->FaceNum * 3 ) ;
	TargetFace2VIndex = TargetFace1VIndex + Mesh->FaceNum * 3 ;

	// ���₳�Ȃ���΂Ȃ�Ȃ��ʃf�[�^�̐��𒲂ׂ�
	FaceT = Mesh->Faces ;
	TargetSideNum = 0 ;
	for( i = 0 ; ( DWORD )i < Mesh->FaceNum ; i ++, FaceT ++ )
	{
		FaceT2 = FaceT + 1 ;
		for( j = i + 1 ; ( DWORD )j < Mesh->FaceNum ; j++, FaceT2 ++ )
		{
			// �g�p���Ă���Q���_���������A���g�p���Ă���@�����قȂ�ӂ���������ʃf�[�^�𑝂₷
			if( ( FaceT->VertexIndex[ 0 ] == FaceT2->VertexIndex[ 0 ] &&
				  FaceT->VertexIndex[ 1 ] == FaceT2->VertexIndex[ 1 ] &&
				  ( FaceT->NormalIndex[ 0 ] != FaceT2->NormalIndex[ 0 ] ||
				    FaceT->NormalIndex[ 1 ] != FaceT2->NormalIndex[ 1 ] ) ) )
			{
				TargetFace1[ TargetSideNum ] = i ;
				TargetFace1VIndex[ TargetSideNum ][ 0 ] = 0 ;
				TargetFace1VIndex[ TargetSideNum ][ 1 ] = 1 ;

				TargetFace2[ TargetSideNum ] = j ;
				TargetFace2VIndex[ TargetSideNum ][ 0 ] = 0 ;
				TargetFace2VIndex[ TargetSideNum ][ 1 ] = 1 ;

				TargetSideNum++;
			}
			else
			if( ( FaceT->VertexIndex[ 0 ] == FaceT2->VertexIndex[ 1 ] &&
				  FaceT->VertexIndex[ 1 ] == FaceT2->VertexIndex[ 0 ] &&
				  ( FaceT->NormalIndex[ 0 ] != FaceT2->NormalIndex[ 1 ] ||
				    FaceT->NormalIndex[ 1 ] != FaceT2->NormalIndex[ 0 ] ) ) )
			{
				TargetFace1[ TargetSideNum ] = i ;
				TargetFace1VIndex[ TargetSideNum ][ 0 ] = 0 ;
				TargetFace1VIndex[ TargetSideNum ][ 1 ] = 1 ;

				TargetFace2[ TargetSideNum ] = j ;
				TargetFace2VIndex[ TargetSideNum ][ 0 ] = 1 ;
				TargetFace2VIndex[ TargetSideNum ][ 1 ] = 0 ;

				TargetSideNum++;
			}
			else
			if( ( FaceT->VertexIndex[ 0 ] == FaceT2->VertexIndex[ 0 ] &&
				  FaceT->VertexIndex[ 1 ] == FaceT2->VertexIndex[ 2 ] &&
				  ( FaceT->NormalIndex[ 0 ] != FaceT2->NormalIndex[ 0 ] ||
				    FaceT->NormalIndex[ 1 ] != FaceT2->NormalIndex[ 2 ] ) ) )
			{
				TargetFace1[ TargetSideNum ] = i ;
				TargetFace1VIndex[ TargetSideNum ][ 0 ] = 0 ;
				TargetFace1VIndex[ TargetSideNum ][ 1 ] = 1 ;

				TargetFace2[ TargetSideNum ] = j ;
				TargetFace2VIndex[ TargetSideNum ][ 0 ] = 0 ;
				TargetFace2VIndex[ TargetSideNum ][ 1 ] = 2 ;

				TargetSideNum++;
			}
			else
			if( ( FaceT->VertexIndex[ 0 ] == FaceT2->VertexIndex[ 2 ] &&
				  FaceT->VertexIndex[ 1 ] == FaceT2->VertexIndex[ 0 ] &&
				  ( FaceT->NormalIndex[ 0 ] != FaceT2->NormalIndex[ 2 ] ||
				    FaceT->NormalIndex[ 1 ] != FaceT2->NormalIndex[ 0 ] ) ) )
			{
				TargetFace1[ TargetSideNum ] = i ;
				TargetFace1VIndex[ TargetSideNum ][ 0 ] = 0 ;
				TargetFace1VIndex[ TargetSideNum ][ 1 ] = 1 ;

				TargetFace2[ TargetSideNum ] = j ;
				TargetFace2VIndex[ TargetSideNum ][ 0 ] = 2 ;
				TargetFace2VIndex[ TargetSideNum ][ 1 ] = 0 ;

				TargetSideNum++;
			}
			else
			if( ( FaceT->VertexIndex[ 0 ] == FaceT2->VertexIndex[ 1 ] &&
				  FaceT->VertexIndex[ 1 ] == FaceT2->VertexIndex[ 2 ] &&
				  ( FaceT->NormalIndex[ 0 ] != FaceT2->NormalIndex[ 1 ] ||
				    FaceT->NormalIndex[ 1 ] != FaceT2->NormalIndex[ 2 ] ) ) )
			{
				TargetFace1[ TargetSideNum ] = i ;
				TargetFace1VIndex[ TargetSideNum ][ 0 ] = 0 ;
				TargetFace1VIndex[ TargetSideNum ][ 1 ] = 1 ;

				TargetFace2[ TargetSideNum ] = j ;
				TargetFace2VIndex[ TargetSideNum ][ 0 ] = 1 ;
				TargetFace2VIndex[ TargetSideNum ][ 1 ] = 2 ;

				TargetSideNum++;
			}
			else
			if( ( FaceT->VertexIndex[ 0 ] == FaceT2->VertexIndex[ 2 ] &&
				  FaceT->VertexIndex[ 1 ] == FaceT2->VertexIndex[ 1 ] &&
				  ( FaceT->NormalIndex[ 0 ] != FaceT2->NormalIndex[ 2 ] ||
				    FaceT->NormalIndex[ 1 ] != FaceT2->NormalIndex[ 1 ] ) ) )
			{
				TargetFace1[ TargetSideNum ] = i ;
				TargetFace1VIndex[ TargetSideNum ][ 0 ] = 0 ;
				TargetFace1VIndex[ TargetSideNum ][ 1 ] = 1 ;

				TargetFace2[ TargetSideNum ] = j ;
				TargetFace2VIndex[ TargetSideNum ][ 0 ] = 2 ;
				TargetFace2VIndex[ TargetSideNum ][ 1 ] = 1 ;

				TargetSideNum++;
			}

			if( ( FaceT->VertexIndex[ 1 ] == FaceT2->VertexIndex[ 0 ] &&
				  FaceT->VertexIndex[ 2 ] == FaceT2->VertexIndex[ 1 ] &&
				  ( FaceT->NormalIndex[ 1 ] != FaceT2->NormalIndex[ 0 ] ||
				    FaceT->NormalIndex[ 2 ] != FaceT2->NormalIndex[ 1 ] ) ) )
			{
				TargetFace1[ TargetSideNum ] = i ;
				TargetFace1VIndex[ TargetSideNum ][ 0 ] = 1 ;
				TargetFace1VIndex[ TargetSideNum ][ 1 ] = 2 ;

				TargetFace2[ TargetSideNum ] = j ;
				TargetFace2VIndex[ TargetSideNum ][ 0 ] = 0 ;
				TargetFace2VIndex[ TargetSideNum ][ 1 ] = 1 ;

				TargetSideNum++;
			}
			else
			if( ( FaceT->VertexIndex[ 1 ] == FaceT2->VertexIndex[ 1 ] &&
				  FaceT->VertexIndex[ 2 ] == FaceT2->VertexIndex[ 0 ] &&
				  ( FaceT->NormalIndex[ 1 ] != FaceT2->NormalIndex[ 1 ] ||
				    FaceT->NormalIndex[ 2 ] != FaceT2->NormalIndex[ 0 ] ) ) )
			{
				TargetFace1[ TargetSideNum ] = i ;
				TargetFace1VIndex[ TargetSideNum ][ 0 ] = 1 ;
				TargetFace1VIndex[ TargetSideNum ][ 1 ] = 2 ;

				TargetFace2[ TargetSideNum ] = j ;
				TargetFace2VIndex[ TargetSideNum ][ 0 ] = 1 ;
				TargetFace2VIndex[ TargetSideNum ][ 1 ] = 0 ;

				TargetSideNum++;
			}
			else
			if( ( FaceT->VertexIndex[ 1 ] == FaceT2->VertexIndex[ 0 ] &&
				  FaceT->VertexIndex[ 2 ] == FaceT2->VertexIndex[ 2 ] &&
				  ( FaceT->NormalIndex[ 1 ] != FaceT2->NormalIndex[ 0 ] ||
				    FaceT->NormalIndex[ 2 ] != FaceT2->NormalIndex[ 2 ] ) ) )
			{
				TargetFace1[ TargetSideNum ] = i ;
				TargetFace1VIndex[ TargetSideNum ][ 0 ] = 1 ;
				TargetFace1VIndex[ TargetSideNum ][ 1 ] = 2 ;

				TargetFace2[ TargetSideNum ] = j ;
				TargetFace2VIndex[ TargetSideNum ][ 0 ] = 0 ;
				TargetFace2VIndex[ TargetSideNum ][ 1 ] = 2 ;

				TargetSideNum++;
			}
			else
			if( ( FaceT->VertexIndex[ 1 ] == FaceT2->VertexIndex[ 2 ] &&
				  FaceT->VertexIndex[ 2 ] == FaceT2->VertexIndex[ 0 ] &&
				  ( FaceT->NormalIndex[ 1 ] != FaceT2->NormalIndex[ 2 ] ||
				    FaceT->NormalIndex[ 2 ] != FaceT2->NormalIndex[ 0 ] ) ) )
			{
				TargetFace1[ TargetSideNum ] = i ;
				TargetFace1VIndex[ TargetSideNum ][ 0 ] = 1 ;
				TargetFace1VIndex[ TargetSideNum ][ 1 ] = 2 ;

				TargetFace2[ TargetSideNum ] = j ;
				TargetFace2VIndex[ TargetSideNum ][ 0 ] = 2 ;
				TargetFace2VIndex[ TargetSideNum ][ 1 ] = 0 ;

				TargetSideNum++;
			}
			else
			if( ( FaceT->VertexIndex[ 1 ] == FaceT2->VertexIndex[ 1 ] &&
				  FaceT->VertexIndex[ 2 ] == FaceT2->VertexIndex[ 2 ] &&
				  ( FaceT->NormalIndex[ 1 ] != FaceT2->NormalIndex[ 1 ] ||
				    FaceT->NormalIndex[ 2 ] != FaceT2->NormalIndex[ 2 ] ) ) )
			{
				TargetFace1[ TargetSideNum ] = i ;
				TargetFace1VIndex[ TargetSideNum ][ 0 ] = 1 ;
				TargetFace1VIndex[ TargetSideNum ][ 1 ] = 2 ;

				TargetFace2[ TargetSideNum ] = j ;
				TargetFace2VIndex[ TargetSideNum ][ 0 ] = 1 ;
				TargetFace2VIndex[ TargetSideNum ][ 1 ] = 2 ;

				TargetSideNum++;
			}
			else
			if( ( FaceT->VertexIndex[ 1 ] == FaceT2->VertexIndex[ 2 ] &&
				  FaceT->VertexIndex[ 2 ] == FaceT2->VertexIndex[ 1 ] &&
				  ( FaceT->NormalIndex[ 1 ] != FaceT2->NormalIndex[ 2 ] ||
				    FaceT->NormalIndex[ 2 ] != FaceT2->NormalIndex[ 1 ] ) ) )
			{
				TargetFace1[ TargetSideNum ] = i ;
				TargetFace1VIndex[ TargetSideNum ][ 0 ] = 1 ;
				TargetFace1VIndex[ TargetSideNum ][ 1 ] = 2 ;

				TargetFace2[ TargetSideNum ] = j ;
				TargetFace2VIndex[ TargetSideNum ][ 0 ] = 2 ;
				TargetFace2VIndex[ TargetSideNum ][ 1 ] = 1 ;

				TargetSideNum++;
			}

			if( ( FaceT->VertexIndex[ 2 ] == FaceT2->VertexIndex[ 0 ] &&
				  FaceT->VertexIndex[ 0 ] == FaceT2->VertexIndex[ 1 ] &&
				  ( FaceT->NormalIndex[ 2 ] != FaceT2->NormalIndex[ 0 ] ||
				    FaceT->NormalIndex[ 0 ] != FaceT2->NormalIndex[ 1 ] ) ) )
			{
				TargetFace1[ TargetSideNum ] = i ;
				TargetFace1VIndex[ TargetSideNum ][ 0 ] = 2 ;
				TargetFace1VIndex[ TargetSideNum ][ 1 ] = 0 ;

				TargetFace2[ TargetSideNum ] = j ;
				TargetFace2VIndex[ TargetSideNum ][ 0 ] = 0 ;
				TargetFace2VIndex[ TargetSideNum ][ 1 ] = 1 ;

				TargetSideNum++;
			}
			else
			if( ( FaceT->VertexIndex[ 2 ] == FaceT2->VertexIndex[ 1 ] &&
				  FaceT->VertexIndex[ 0 ] == FaceT2->VertexIndex[ 0 ] &&
				  ( FaceT->NormalIndex[ 2 ] != FaceT2->NormalIndex[ 1 ] ||
				    FaceT->NormalIndex[ 0 ] != FaceT2->NormalIndex[ 0 ] ) ) )
			{
				TargetFace1[ TargetSideNum ] = i ;
				TargetFace1VIndex[ TargetSideNum ][ 0 ] = 2 ;
				TargetFace1VIndex[ TargetSideNum ][ 1 ] = 0 ;

				TargetFace2[ TargetSideNum ] = j ;
				TargetFace2VIndex[ TargetSideNum ][ 0 ] = 1 ;
				TargetFace2VIndex[ TargetSideNum ][ 1 ] = 0 ;

				TargetSideNum++;
			}
			else
			if( ( FaceT->VertexIndex[ 2 ] == FaceT2->VertexIndex[ 0 ] &&
				  FaceT->VertexIndex[ 0 ] == FaceT2->VertexIndex[ 2 ] &&
				  ( FaceT->NormalIndex[ 2 ] != FaceT2->NormalIndex[ 0 ] ||
				    FaceT->NormalIndex[ 0 ] != FaceT2->NormalIndex[ 2 ] ) ) )
			{
				TargetFace1[ TargetSideNum ] = i ;
				TargetFace1VIndex[ TargetSideNum ][ 0 ] = 2 ;
				TargetFace1VIndex[ TargetSideNum ][ 1 ] = 0 ;

				TargetFace2[ TargetSideNum ] = j ;
				TargetFace2VIndex[ TargetSideNum ][ 0 ] = 0 ;
				TargetFace2VIndex[ TargetSideNum ][ 1 ] = 2 ;

				TargetSideNum++;
			}
			else
			if( ( FaceT->VertexIndex[ 2 ] == FaceT2->VertexIndex[ 2 ] &&
				  FaceT->VertexIndex[ 0 ] == FaceT2->VertexIndex[ 0 ] &&
				  ( FaceT->NormalIndex[ 2 ] != FaceT2->NormalIndex[ 2 ] ||
				    FaceT->NormalIndex[ 0 ] != FaceT2->NormalIndex[ 0 ] ) ) )
			{
				TargetFace1[ TargetSideNum ] = i ;
				TargetFace1VIndex[ TargetSideNum ][ 0 ] = 2 ;
				TargetFace1VIndex[ TargetSideNum ][ 1 ] = 0 ;

				TargetFace2[ TargetSideNum ] = j ;
				TargetFace2VIndex[ TargetSideNum ][ 0 ] = 2 ;
				TargetFace2VIndex[ TargetSideNum ][ 1 ] = 0 ;

				TargetSideNum++;
			}
			else
			if( ( FaceT->VertexIndex[ 2 ] == FaceT2->VertexIndex[ 1 ] &&
				  FaceT->VertexIndex[ 0 ] == FaceT2->VertexIndex[ 2 ] &&
				  ( FaceT->NormalIndex[ 2 ] != FaceT2->NormalIndex[ 1 ] ||
				    FaceT->NormalIndex[ 0 ] != FaceT2->NormalIndex[ 2 ] ) ) )
			{
				TargetFace1[ TargetSideNum ] = i ;
				TargetFace1VIndex[ TargetSideNum ][ 0 ] = 2 ;
				TargetFace1VIndex[ TargetSideNum ][ 1 ] = 0 ;

				TargetFace2[ TargetSideNum ] = j ;
				TargetFace2VIndex[ TargetSideNum ][ 0 ] = 1 ;
				TargetFace2VIndex[ TargetSideNum ][ 1 ] = 2 ;

				TargetSideNum++;
			}
			else
			if( ( FaceT->VertexIndex[ 2 ] == FaceT2->VertexIndex[ 2 ] &&
				  FaceT->VertexIndex[ 0 ] == FaceT2->VertexIndex[ 1 ] &&
				  ( FaceT->NormalIndex[ 2 ] != FaceT2->NormalIndex[ 2 ] ||
				    FaceT->NormalIndex[ 0 ] != FaceT2->NormalIndex[ 1 ] ) ) )
			{
				TargetFace1[ TargetSideNum ] = i ;
				TargetFace1VIndex[ TargetSideNum ][ 0 ] = 2 ;
				TargetFace1VIndex[ TargetSideNum ][ 1 ] = 0 ;

				TargetFace2[ TargetSideNum ] = j ;
				TargetFace2VIndex[ TargetSideNum ][ 0 ] = 2 ;
				TargetFace2VIndex[ TargetSideNum ][ 1 ] = 1 ;

				TargetSideNum++;
			}
		}
	}
	AddFaceCount = TargetSideNum * 2 ;

	// �ǉ��ʂ������ꍇ�͂����ŏI��
	if( TargetSideNum == 0 )
	{
		DXFREE( TargetFace1 ) ;
		return true ;
	}

	// ���₵���ʂ��i�[���郁�����̈�̊m��
	NewFaceNum = Mesh->FaceNum + AddFaceCount ;
	NewFace = ( MV1_MESHFACE_R * )ADDMEMAREA( ( sizeof( MV1_MESHFACE_R ) + ( 3 + MV1_READ_MAX_UV_NUM ) * 3 * sizeof( DWORD ) ) * NewFaceNum, &ReadModel->Mem ) ;
	if( NewFace == NULL )
	{
		DXFREE( TargetFace1 ) ;
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\x52\x00\x65\x00\x61\x00\x64\x00\x20\x00\x4d\x00\x6f\x00\x64\x00\x65\x00\x6c\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\x2e\x7e\x00\x90\xdd\x30\xea\x30\xb4\x30\xf3\x30\xfd\x8f\xa0\x52\x8c\x5f\x6e\x30\x62\x97\xc7\x30\xfc\x30\xbf\x30\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Read Model Error : �k�ރ|���S���ǉ���̖ʃf�[�^���i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
		return false ;
	}
	NewIndex = ( DWORD * )( NewFace + NewFaceNum ) ;

	// �C���f�b�N�X�f�[�^�̃A�h���X���Z�b�g�A�b�v
	NewFaceT = NewFace ;
	NewIndexT = NewIndex ;
	for( i = 0 ; i < ( int )NewFaceNum ; i ++, NewFaceT ++ )
	{
		NewFaceT->VertexIndex = NewIndexT ; NewIndexT += 3 ;
		NewFaceT->NormalIndex = NewIndexT ; NewIndexT += 3 ;
		NewFaceT->VertexColorIndex = NewIndexT ; NewIndexT += 3 ;
		for( j = 0 ; j < MV1_READ_MAX_UV_NUM ; j ++, NewIndexT += 3 )
			NewFaceT->UVIndex[ j ] = NewIndexT ;
	}

	// �����̖ʂ̏����R�s�[
	FaceT = Mesh->Faces ;
	NewFaceT = NewFace ;
	for( i = 0 ; i < ( int )Mesh->FaceNum ; i ++, FaceT ++, NewFaceT ++ )
	{
		NewFaceT->IndexNum = 3 ;
		NewFaceT->PolygonNum = 1 ;

		for( j = 0 ; j < 3 ; j ++ )
		{
			NewFaceT->VertexIndex[ j ] = FaceT->VertexIndex[ j ] ;
			NewFaceT->NormalIndex[ j ] = FaceT->NormalIndex[ j ] ;
			NewFaceT->VertexColorIndex[ j ] = FaceT->VertexColorIndex[ j ] ;
			for( k = 0 ; k < MV1_READ_MAX_UV_NUM ; k ++ )
			{
				NewFaceT->UVIndex[ k ][ j ] = FaceT->UVIndex[ k ][ j ] ;
			}
		}

		NewFaceT->MaterialIndex = FaceT->MaterialIndex ;
		NewFaceT->Normal = FaceT->Normal ;
	}

	// �k�ރ|���S����ǉ�
	NewFaceT2 = NewFace + Mesh->FaceNum ;
	for( i = 0 ; ( DWORD )i < TargetSideNum ; i ++ )
	{
		int i1, i2, i3 ;

		FaceT  = &Mesh->Faces[ TargetFace1[ i ] ] ;
		FaceT2 = &Mesh->Faces[ TargetFace2[ i ] ] ;

		i1 = TargetFace1VIndex[ i ][ 0 ] ;
		i2 = TargetFace2VIndex[ i ][ 0 ] ;
		i3 = TargetFace1VIndex[ i ][ 1 ] ;
		NewFaceT2->IndexNum = 3 ;
		NewFaceT2->PolygonNum = 1 ;
		NewFaceT2->VertexIndex[ 0 ] = FaceT-> VertexIndex[ i1 ] ;
		NewFaceT2->VertexIndex[ 1 ] = FaceT2->VertexIndex[ i2 ] ;
		NewFaceT2->VertexIndex[ 2 ] = FaceT-> VertexIndex[ i3 ] ;
		NewFaceT2->NormalIndex[ 0 ] = FaceT-> NormalIndex[ i1 ] ;
		NewFaceT2->NormalIndex[ 1 ] = FaceT2->NormalIndex[ i2 ] ;
		NewFaceT2->NormalIndex[ 2 ] = FaceT-> NormalIndex[ i3 ] ;
		NewFaceT2->VertexColorIndex[ 0 ] = FaceT-> VertexColorIndex[ i1 ] ;
		NewFaceT2->VertexColorIndex[ 1 ] = FaceT2->VertexColorIndex[ i2 ] ;
		NewFaceT2->VertexColorIndex[ 2 ] = FaceT-> VertexColorIndex[ i3 ] ;
		for( k = 0 ; ( DWORD )k < MV1_READ_MAX_UV_NUM ; k ++ )
		{
			NewFaceT2->UVIndex[ k ][ 0 ] = FaceT ->UVIndex[ k ][ i1 ] ;
			NewFaceT2->UVIndex[ k ][ 1 ] = FaceT2->UVIndex[ k ][ i2 ] ;
			NewFaceT2->UVIndex[ k ][ 2 ] = FaceT ->UVIndex[ k ][ i3 ] ;
		}
		NewFaceT2->MaterialIndex = FaceT->MaterialIndex ;
		NewFaceT2++;

		i1 = TargetFace1VIndex[ i ][ 1 ] ;
		i2 = TargetFace2VIndex[ i ][ 0 ] ;
		i3 = TargetFace2VIndex[ i ][ 1 ] ;
		NewFaceT2->IndexNum = 3 ;
		NewFaceT2->PolygonNum = 1 ;
		NewFaceT2->VertexIndex[ 0 ] = FaceT ->VertexIndex[ i1 ] ;
		NewFaceT2->VertexIndex[ 1 ] = FaceT2->VertexIndex[ i2 ] ;
		NewFaceT2->VertexIndex[ 2 ] = FaceT2->VertexIndex[ i3 ] ;
		NewFaceT2->NormalIndex[ 0 ] = FaceT ->NormalIndex[ i1 ] ;
		NewFaceT2->NormalIndex[ 1 ] = FaceT2->NormalIndex[ i2 ] ;
		NewFaceT2->NormalIndex[ 2 ] = FaceT2->NormalIndex[ i3 ] ;
		NewFaceT2->VertexColorIndex[ 0 ] = FaceT ->VertexColorIndex[ i1 ] ;
		NewFaceT2->VertexColorIndex[ 1 ] = FaceT2->VertexColorIndex[ i2 ] ;
		NewFaceT2->VertexColorIndex[ 2 ] = FaceT2->VertexColorIndex[ i3 ] ;
		for( k = 0 ; ( DWORD )k < MV1_READ_MAX_UV_NUM ; k ++ )
		{
			NewFaceT2->UVIndex[ k ][ 0 ] = FaceT ->UVIndex[ k ][ i1 ] ;
			NewFaceT2->UVIndex[ k ][ 1 ] = FaceT2->UVIndex[ k ][ i2 ] ;
			NewFaceT2->UVIndex[ k ][ 2 ] = FaceT2->UVIndex[ k ][ i3 ] ;
		}
		NewFaceT2->MaterialIndex = FaceT->MaterialIndex ;
		NewFaceT2++;
	}

	// �ʂ̐��𑝂₷
	Mesh->FaceNum += AddFaceCount ;

	// �������̉���ƍ����ւ�
	SUBMEMAREA( &ReadModel->Mem, Mesh->Faces );
	Mesh->Faces = NewFace ;
	Mesh->FaceIndexBuffer = NewIndex ;

	// ��Ɨp�̊m�ۂ��������������
	DXFREE( TargetFace1 ) ;

	// ����I��
	return true ;
}

// ���W�A�X�L�j���O�E�G�C�g�l���S������̒��_�𓝍�����( �@���͖������� )
struct MV1_OPTIMIZEPOSITION_WEIGHT_ONE
{
	MV1_SKIN_WEIGHT_R *SkinWeight ;
	float Weight ;
} ;

struct MV1_OPTIMIZEPOSITION_WEIGHT
{
	DWORD DataNum ;
	MV1_OPTIMIZEPOSITION_WEIGHT_ONE Data[ 1 ] ;
} ;

static bool MV1OptimizePosition( MV1_MODEL_R * /*ReadModel*/, MV1_MESH_R *Mesh )
{
	int i, j, k, l, SubNum, NewPositionNum ;
	MV1_MESHFACE_R *Face ;
	BYTE *DisableFlag ;
	DWORD *NewVertexIndex ;
	VECTOR *OldPositions, Sa ;
	MV1_SKIN_WEIGHT_R *SkinW ;
	DWORD PosSkinWUnitSize ;
	MV1_OPTIMIZEPOSITION_WEIGHT *PositionSkinW, *PosSkinW, *PosSkinW2 ;

	// �����t���O���i�[���郁�����̈�̊m��
	PosSkinWUnitSize = sizeof( MV1_OPTIMIZEPOSITION_WEIGHT ) + sizeof( MV1_OPTIMIZEPOSITION_WEIGHT_ONE ) * ( Mesh->SkinWeightsNum - 1 ) ;
	OldPositions = ( VECTOR * )DXALLOC( ( sizeof( DWORD ) + sizeof( BYTE ) + sizeof( VECTOR ) + PosSkinWUnitSize ) * Mesh->PositionNum ) ;
	if( OldPositions == NULL )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\x52\x00\x65\x00\x61\x00\x64\x00\x20\x00\x4d\x00\x6f\x00\x64\x00\x65\x00\x6c\x00\x20\x00\x43\x00\x6f\x00\x6e\x00\x76\x00\x65\x00\x72\x00\x74\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\x02\x98\xb9\x70\x00\x67\x69\x90\x16\x53\xe6\x51\x06\x74\x67\x30\x7f\x4f\x28\x75\x59\x30\x8b\x30\xc6\x30\xf3\x30\xdd\x30\xe9\x30\xea\x30\xd0\x30\xc3\x30\xd5\x30\xa1\x30\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Read Model Convert Error : ���_�œK�������Ŏg�p����e���|�����o�b�t�@�̊m�ۂɎ��s���܂���\n" @*/ )) ;
		return false ;
	}
	PositionSkinW = ( MV1_OPTIMIZEPOSITION_WEIGHT * )( OldPositions + Mesh->PositionNum ) ;
	NewVertexIndex = ( DWORD * )( ( BYTE * )PositionSkinW + PosSkinWUnitSize * Mesh->PositionNum ) ;
	DisableFlag = ( BYTE * )( NewVertexIndex + Mesh->PositionNum ) ;
	_MEMSET( PositionSkinW, 0, ( sizeof( DWORD ) + sizeof( BYTE ) + PosSkinWUnitSize ) * Mesh->PositionNum ) ;
	_MEMCPY( OldPositions, Mesh->Positions, sizeof( VECTOR ) * Mesh->PositionNum ) ;

	// �X�L�j���O���b�V���̏ꍇ�A�e���_�̃X�L�j���O���b�V�����X�g���쐬����
	if( Mesh->SkinWeightsNum )
	{
		for( i = 0 ; ( DWORD )i < Mesh->SkinWeightsNum ; i ++ )
		{
			SkinW = Mesh->SkinWeights[ i ] ;
			for( j = 0 ; ( DWORD )j < SkinW->DataNum ; j ++ )
			{
				PosSkinW = ( MV1_OPTIMIZEPOSITION_WEIGHT * )( ( BYTE * )PositionSkinW + PosSkinWUnitSize * SkinW->Data[ j ].TargetVertex ) ;
				PosSkinW->Data[ PosSkinW->DataNum ].SkinWeight = SkinW ;
				PosSkinW->Data[ PosSkinW->DataNum ].Weight     = SkinW->Data[ j ].Weight ;
				PosSkinW->DataNum ++ ;
			}
		}
	}

	// ��v�e�X�g
	SubNum = 0 ;
	PosSkinW = PositionSkinW ;
	for( i = 0 ; ( DWORD )i < Mesh->PositionNum ; i ++, PosSkinW = ( MV1_OPTIMIZEPOSITION_WEIGHT * )( ( BYTE * )PosSkinW + PosSkinWUnitSize ) )
	{
		// ���ɖ����������牽�����Ȃ�
		if( DisableFlag[ i ] ) continue ;

		PosSkinW2 = ( MV1_OPTIMIZEPOSITION_WEIGHT * )( ( BYTE * )PositionSkinW + PosSkinWUnitSize * ( i + 1 ) ) ;
		for( j = i + 1 ; ( DWORD )j < Mesh->PositionNum ; j ++, PosSkinW2 = ( MV1_OPTIMIZEPOSITION_WEIGHT * )( ( BYTE * )PosSkinW2 + PosSkinWUnitSize ) )
		{
			// ���ɖ����������牽�����Ȃ�
			if( DisableFlag[ j ] ) continue ;

			// ���W�����������ׂ�
//			if( *( ( DWORD * )&Mesh->Positions[ i ].x ) != *( ( DWORD * )&Mesh->Positions[ j ].x ) ||
//				*( ( DWORD * )&Mesh->Positions[ i ].y ) != *( ( DWORD * )&Mesh->Positions[ j ].y ) ||
//				*( ( DWORD * )&Mesh->Positions[ i ].z ) != *( ( DWORD * )&Mesh->Positions[ j ].z ) ) continue ;
			Sa = VSub( Mesh->Positions[ i ], Mesh->Positions[ j ] ) ;
			if( ( Sa.x > 0.00001f && Sa.x < -0.00001f ) ||
				( Sa.y > 0.00001f && Sa.y < -0.00001f ) ||
				( Sa.z > 0.00001f && Sa.z < -0.00001f ) ) continue ;

			// �X�L�j���O�E�G�C�g�����������ׂ�
			if( PosSkinW->DataNum != PosSkinW2->DataNum ) continue ;
			for( k = 0 ; ( DWORD )k < PosSkinW->DataNum ; k ++ )
			{
				for( l = 0 ; ( DWORD )l < PosSkinW2->DataNum ; l ++ )
				{
					if( PosSkinW->Data[ k ].SkinWeight              == PosSkinW2->Data[ l ].SkinWeight &&
						*( ( DWORD * )&PosSkinW->Data[ k ].Weight ) == *( ( DWORD * )&PosSkinW2->Data[ l ].Weight ) )
						break ;
				}
				if( ( DWORD )l == PosSkinW2->DataNum ) break ;
			}
			if( ( DWORD )k != PosSkinW->DataNum ) continue ;

			// �������_�𔭌�������ԍ��̎Ⴂ���_�ɓ�������
			Face = Mesh->Faces ;
			for( k = 0 ; ( DWORD )k < Mesh->FaceNum ; k ++, Face ++ )
			{
				for( l = 0 ; ( DWORD )l < Face->IndexNum ; l ++ )
				{
					if( Face->VertexIndex[ l ] != ( DWORD )j ) continue ;
					Face->VertexIndex[ l ] = ( DWORD )i ;
				}
			}

			// �����t���O�𗧂Ă�
			DisableFlag[ j ] = 1 ;

			// ���炵���������Z����
			SubNum ++ ;
		}
	}

	// ��������Ȃ��ꍇ�͂����ŏI��
	if( SubNum == 0 )
	{
		DXFREE( OldPositions ) ;
		OldPositions = NULL ;
		return true ;
	}

	// �g�p����Ă��钸�_�̂ݗ񋓂���
	NewPositionNum = 0 ;
	for( i = 0 ; ( DWORD )i < Mesh->PositionNum ; i ++ )
	{
		if( DisableFlag[ i ] == 1 ) continue ;

		NewVertexIndex[ i ] = ( DWORD )NewPositionNum ;
		Mesh->Positions[ NewPositionNum ] = OldPositions[ i ] ;
		NewPositionNum ++ ;
	}
	Mesh->PositionNum = ( DWORD )NewPositionNum ;

	// �|���S���̍��W�C���f�b�N�X��t������
	Face = Mesh->Faces ;
	for( i = 0 ; ( DWORD )i < Mesh->FaceNum ; i ++, Face ++ )
	{
		for( j = 0 ; ( DWORD )j < Face->IndexNum ; j ++ )
		{
			Face->VertexIndex[ j ] = NewVertexIndex[ Face->VertexIndex[ j ] ] ;
		}
	}

	// �E�G�C�g���̃^�[�Q�b�g���W���Ȃ��Ȃ����ꍇ�̓E�G�C�g�l���O�ɂ���
	for( i = 0 ; ( DWORD )i < Mesh->SkinWeightsNum ; i ++ )
	{
		SkinW = Mesh->SkinWeights[ i ] ;
		for( j = 0 ; ( DWORD )j < SkinW->DataNum ; j ++ )
		{
			if( DisableFlag[ SkinW->Data[ j ].TargetVertex ] == 1 )
			{
				SkinW->Data[ j ].Weight = 0.0f ;
			}
			else
			{
				SkinW->Data[ j ].TargetVertex = NewVertexIndex[ SkinW->Data[ j ].TargetVertex ] ;
			}
		}
	}

	// �m�ۂ������������������
	if( OldPositions )
	{
		DXFREE( OldPositions ) ;
		OldPositions = NULL ;
	}

	return true ;
}

/*
// ���b�V���̏]�@���Ɛڐ����v�Z����( �S�|���S�����O�p�`�|���S���ł���K�v������܂� )
static bool MV1MakeMeshBinormalsAndTangents( MV1_MODEL_R *ReadModel, MV1_MESH_R *Mesh )
{
	int i ;
	MV1_MESHFACE_R *Face ;
	VECTOR v1, v2, vt, du, dv, vb, vn ;

	// �]�@���Ɛڐ����i�[���郁�����̈���m�ۂ���
	if( Mesh->Binormals == NULL )
	{
		Mesh->Binormals = ( VECTOR * )ADDMEMAREA( sizeof( VECTOR ) * Mesh->NormalNum * 2, &ReadModel->Mem ) ;
		Mesh->Tangents = Mesh->Binormals + Mesh->NormalNum ;
	}

	// �S�Ă̖ʂ̐������J��Ԃ�
	Face = Mesh->Faces ;
	for( i = 0 ; ( DWORD )i < Mesh->FaceNum ; i ++, Face ++ )
	{
		v1.x = Mesh->Positions[ Face->VertexIndex[ 1 ] ].x - Mesh->Positions[ Face->VertexIndex[ 0 ] ].x ;
		v1.y = Mesh->UVs[ 0 ][ Face->UVIndex[ 0 ][ 1 ] ].x - Mesh->UVs[ 0 ][ Face->UVIndex[ 0 ][ 0 ] ].x ;
		v1.z = Mesh->UVs[ 0 ][ Face->UVIndex[ 0 ][ 1 ] ].y - Mesh->UVs[ 0 ][ Face->UVIndex[ 0 ][ 0 ] ].y ;

		v2.x = Mesh->Positions[ Face->VertexIndex[ 2 ] ].x - Mesh->Positions[ Face->VertexIndex[ 0 ] ].x ;
		v2.y = Mesh->UVs[ 0 ][ Face->UVIndex[ 0 ][ 2 ] ].x - Mesh->UVs[ 0 ][ Face->UVIndex[ 0 ][ 0 ] ].x ;
		v2.z = Mesh->UVs[ 0 ][ Face->UVIndex[ 0 ][ 2 ] ].y - Mesh->UVs[ 0 ][ Face->UVIndex[ 0 ][ 0 ] ].y ;

		vt = VCross( v1, v2 ) ;
		du.x = 1.0f ;
		if( VDot( vt, vt ) >= 0.0000001f )
		{
			du.x = -vt.y / vt.x ;
			dv.x = -vt.z / vt.x ;
		}

		v1.x = Mesh->Positions[ Face->VertexIndex[ 1 ] ].y - Mesh->Positions[ Face->VertexIndex[ 0 ] ].y ;
		v2.x = Mesh->Positions[ Face->VertexIndex[ 2 ] ].y - Mesh->Positions[ Face->VertexIndex[ 0 ] ].y ;

		vt = VCross( v1, v2 ) ;
		du.y = 1.0f ;
		if( VDot( vt, vt ) >= 0.0000001f )
		{
			du.y = -vt.y / vt.x ;
			dv.y = -vt.z / vt.x ;
		}

		v1.x = Mesh->Positions[ Face->VertexIndex[ 1 ] ].z - Mesh->Positions[ Face->VertexIndex[ 0 ] ].z ;
		v2.x = Mesh->Positions[ Face->VertexIndex[ 2 ] ].z - Mesh->Positions[ Face->VertexIndex[ 0 ] ].z ;

		vt = VCross( v1, v2 ) ;
		du.z = 1.0f ;
		if( VDot( vt, vt ) >= 0.0000001f )
		{
			du.z = -vt.y / vt.x ;
			dv.z = -vt.z / vt.x ;
		}

		VectorAdd( &Mesh->Tangents[ Face->NormalIndex[ 0 ] ], &Mesh->Tangents[ Face->NormalIndex[ 0 ] ], &du ) ;
		VectorAdd( &Mesh->Tangents[ Face->NormalIndex[ 1 ] ], &Mesh->Tangents[ Face->NormalIndex[ 1 ] ], &du ) ;
		VectorAdd( &Mesh->Tangents[ Face->NormalIndex[ 2 ] ], &Mesh->Tangents[ Face->NormalIndex[ 2 ] ], &du ) ;

		VectorAdd( &Mesh->Binormals[ Face->NormalIndex[ 0 ] ], &Mesh->Binormals[ Face->NormalIndex[ 0 ] ], &dv ) ;
		VectorAdd( &Mesh->Binormals[ Face->NormalIndex[ 1 ] ], &Mesh->Binormals[ Face->NormalIndex[ 1 ] ], &dv ) ;
		VectorAdd( &Mesh->Binormals[ Face->NormalIndex[ 2 ] ], &Mesh->Binormals[ Face->NormalIndex[ 2 ] ], &dv ) ;
	}

	// �@���̎Z�o�Ɛ��K��
	for( i = 0 ; ( DWORD )i < Mesh->NormalNum ; i ++ )
	{
		vt = VNorm( Mesh->Tangents[ i ] ) ;
		vn = VNorm( VCross( vt, Mesh->Binormals[ i ] ) ) ;
		vb = VNorm( VCross( vn, vt ) ) ;

		// �@���̕������m�F
		if( VDot( vn, Mesh->Normals[ i ] ) < 0.0f )
		{
			vn.x = -vn.x ;
			vn.y = -vn.y ;
			vn.z = -vn.z ;
		}

		// ���K��
		Mesh->Tangents[ i ]  = vt ;
		Mesh->Binormals[ i ] = vb ;
//		Mesh->Normals[ i ]   = vn ;
	}

	// �I��
	return 0 ;
}
*/


// ���f���f�[�^�̃X�L�j���O���b�V���p�̃{�[�������œK������
static bool MV1OptimizeSkinBoneInfo( MV1_MODEL_BASE *Model )
{
	int AfterNum ;
	int AfterUseFrameNum ;
	int *AfterUseFrame ;
	MV1_SKIN_BONE_USE_FRAME *TempUseFrame ;
	MV1_SKIN_BONE *AfterSkin, *TASK, *SK1, *SK2, *ASK, TempSkin ;
	MV1_FRAME_BASE *Frame ;
	int *BeginToAfterIndex, *LostIndex, i, j, k, *BeginIndex, *AfterIndex ;
	bool Change ;

	if( Model->SkinBoneNum == 0 )
		return true ;

	AfterSkin = ( MV1_SKIN_BONE * )DXALLOC( ( sizeof( int ) + sizeof( MV1_SKIN_BONE_USE_FRAME ) ) * Model->SkinBoneUseFrameNum + ( sizeof( MV1_SKIN_BONE ) + sizeof( int ) ) * Model->SkinBoneNum * 4 ) ;
	if( AfterSkin == NULL )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\x4d\x00\x6f\x00\x64\x00\x65\x00\x6c\x00\x20\x00\x4f\x00\x70\x00\x74\x00\x69\x00\x6d\x00\x69\x00\x7a\x00\x65\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xb9\x30\xad\x30\xcb\x30\xf3\x30\xb0\x30\xe1\x30\xc3\x30\xb7\x30\xe5\x30\xc5\x60\x31\x58\x00\x67\x69\x90\x16\x53\xe6\x51\x06\x74\x67\x30\x7f\x4f\x28\x75\x59\x30\x8b\x30\xc6\x30\xf3\x30\xdd\x30\xe9\x30\xea\x30\xd0\x30\xc3\x30\xd5\x30\xa1\x30\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Model Optimize Error : �X�L�j���O���b�V�����œK�������Ŏg�p����e���|�����o�b�t�@�̊m�ۂɎ��s���܂���\n" @*/ )) ;
		return false ;
	}
	BeginToAfterIndex = ( int * )( AfterSkin + Model->SkinBoneNum ) ;
	LostIndex         = BeginToAfterIndex + Model->SkinBoneNum ;
	AfterIndex        = LostIndex         + Model->SkinBoneNum ;
	BeginIndex        = AfterIndex        + Model->SkinBoneNum ;
	AfterUseFrame     = ( int                     * )( BeginIndex    + Model->SkinBoneNum         ) ;
	TempUseFrame      = ( MV1_SKIN_BONE_USE_FRAME * )( AfterUseFrame + Model->SkinBoneUseFrameNum ) ;
	_MEMSET( BeginToAfterIndex,    0, sizeof( int ) * Model->SkinBoneNum         ) ;
	_MEMSET( LostIndex,            0, sizeof( int ) * Model->SkinBoneNum         ) ;
	_MEMSET( AfterUseFrame,     0xff, sizeof( int ) * Model->SkinBoneUseFrameNum ) ;
	_MEMCPY( TempUseFrame, Model->SkinBoneUseFrame, sizeof( MV1_SKIN_BONE_USE_FRAME ) * Model->SkinBoneUseFrameNum ) ;

	// ������e�̃{�[�������폜������ŁA�{�[���f�[�^���t���[�����ɕ��בւ���
	{
		// ������e�̏�񂪖��������ׂȂ���e�[�u�����쐬
		AfterNum = 0 ;
		SK1 = Model->SkinBone ;
		ASK = AfterSkin ;
		Change = false ;
		for( i = 0 ; i < Model->SkinBoneNum ; i ++, SK1 ++ )
		{
			SK2 = Model->SkinBone ;
			k = 0 ;
			for( j = 0 ; j < i ; j ++, SK2 ++ )
			{
				if( LostIndex[ j ] == 1 ) continue ;

				if( SK1->BoneFrame == SK2->BoneFrame &&
					_MEMCMP( &SK1->ModelLocalMatrix, &SK2->ModelLocalMatrix, sizeof( MATRIX_4X4CT ) ) == 0 )
					break ;
				k ++ ;
			}
			if( j == i )
			{
				*ASK = *SK1 ;
				ASK->UseFrameNum = 0 ;
				BeginIndex[ AfterNum ] = AfterNum ;
				AfterIndex[ AfterNum ] = AfterNum ;
				ASK ++ ;
				AfterNum ++ ;
			}
			else
			{
				LostIndex[ i ] = 1 ;
				Change = true ;
			}
			BeginToAfterIndex[ i ] = k ;

			TASK = &AfterSkin[ k ] ;
			TASK->UseFrameNum += SK1->UseFrameNum ;
			for( j = 0 ; j < SK1->UseFrameNum ; j ++ )
				AfterUseFrame[ SK1->UseFrame - Model->SkinBoneUseFrame + j ] = k ;
		}

		// �{�[���f�[�^��Ώۃt���[�����ɕ��בւ���
		for( i = 0 ; i < AfterNum ; i ++ )
		{
			for( j = i + 1 ; j < AfterNum ; j ++ )
			{
				if( AfterSkin[ i ].BoneFrame > AfterSkin[ j ].BoneFrame )
				{
					Change = true ;

					TempSkin = AfterSkin[ i ] ;
					AfterSkin[ i ] = AfterSkin[ j ] ;
					AfterSkin[ j ] = TempSkin ;

					k = BeginIndex[ i ] ;
					BeginIndex[ i ] = BeginIndex[ j ] ;
					BeginIndex[ j ] = k ;

					AfterIndex[ BeginIndex[ i ] ] = i ;
					AfterIndex[ BeginIndex[ j ] ] = j ;
				}
			}
		}

		// ���̕ύX�������ꍇ�͉������Ȃ�
		if( Change )
		{
			// �X�L�j���O���b�V���̐��Ə����X�V����
			Model->SkinBoneNum = AfterNum ;
			_MEMCPY( Model->SkinBone, AfterSkin, sizeof( MV1_SKIN_BONE ) * Model->SkinBoneNum ) ;

			// �{�[�����g�p����t���[���̔z��̃A�h���X�����蓖�Ă�
			SK1 = Model->SkinBone ;
			AfterUseFrameNum = 0 ;
			for( i = 0 ; i < AfterNum ; i ++, SK1 ++ )
			{
				SK1->UseFrame = Model->SkinBoneUseFrame + AfterUseFrameNum ;
				AfterUseFrameNum += SK1->UseFrameNum ;
				SK1->UseFrameNum = 0 ;
			}

			// �{�[�����g�p����t���[���̔z����Z�b�g����
			for( i = 0 ; i < Model->SkinBoneUseFrameNum ; i ++ )
			{
				if( AfterUseFrame[ i ] == -1 ) continue ;
				SK1 = &Model->SkinBone[ AfterIndex[ AfterUseFrame[ i ] ] ] ;
				SK1->UseFrame[ SK1->UseFrameNum ] = TempUseFrame[ i ] ;

				// �t���[���̕����X�V
				Model->Frame[ TempUseFrame[ i ].Index ].UseSkinBone[ TempUseFrame[ i ].MatrixIndex ] = SK1 ;

				SK1->UseFrameNum ++ ;
			}
		}

		// �t���[���Ɋ֘A����{�[���̃A�h���X���Z�b�g����
		SK1 = Model->SkinBone ;
		for( i = 0 ; i < Model->SkinBoneNum ; i ++, SK1 ++ )
		{
			Frame = &Model->Frame[ SK1->BoneFrame ] ;
			if( Frame->SkinBoneNum == 0 )
				Frame->SkinBone = SK1 ;
			Frame->SkinBoneNum ++ ;
		}
	}

	// �m�ۂ��Ă����������̉��
	DXFREE( AfterSkin ) ;
	AfterSkin = NULL ;

	return true ;
}

// �A�j���[�V�����f�[�^���œK������
static bool MV1OptimizeAnim( MV1_MODEL_R *ReadModel )
{
	MV1_ANIM_R           *AnimR ;
	MV1_ANIMKEYSET_R     *KeySetR ;
//	MV1_ANIM_KEY_16BIT_F  TimeSub ;
//	MV1_ANIM_KEY_16BIT_F  KeySub ;
	int                   i ;
	int                   j ;
	int                   k ;
	int                   l ;
//	float                *Time ;
	float                 f ;
//	WORD                 *NewTimeB16 ;

#if 0 // �s���XYZ�ňقȂ�X�P�[��������Ă���Ɛ���ɏ����ł��Ȃ��̂ŉ����̖ڏ������܂ŃR�����g�A�E�g
	// MV1_ANIMKEY_TYPE_MATRIX4X4C �� MV1_ANIMKEY_TYPE_MATRIX3X3 �� MV1_ANIMKEY_TYPE_VECTOR �ɕ�������
	if( ReadModel->AnimDataNotDecomposition == FALSE )
	{
		AnimR = ReadModel->AnimFirst ;
		for( i = 0 ; ( DWORD )i < ReadModel->AnimNum ; i ++, AnimR = AnimR->DataNext )
		{
			KeySetR = AnimR->AnimKeySetFirst ;
			for( j = 0 ; j < AnimR->AnimKeySetNum ; j ++, KeySetR = KeySetR->Next )
			{
				switch( KeySetR->DataType )
				{
				case MV1_ANIMKEY_DATATYPE_MATRIX4X4C :
					{
						MV1_ANIM_KEY_MATRIX4X4C *Key ;
						MV1_ANIM_KEY_MATRIX3X3 *NewKeyMat3X3 ;
						MV1_ANIMKEYSET_R *NewKeySetR ;
						VECTOR *NewKeyTrans ;

						NewKeyTrans = ( VECTOR * )ADDMEMAREA( ( sizeof( VECTOR ) + sizeof( float ) ) * KeySetR->Num, &ReadModel->Mem ) ;
						if( NewKeyTrans )
						{
							// ���s�ړ��p�̃L�[�Z�b�g��ǉ�����
							NewKeySetR = MV1RAddAnimKeySet( ReadModel, AnimR ) ;
							if( NewKeySetR )
							{
								// �����镪�̃f�[�^�T�C�Y�����Z����
								ReadModel->AnimKeyDataSize -= ( sizeof( MV1_ANIM_KEY_MATRIX4X4C ) + sizeof( float ) ) * KeySetR->Num ;
								ReadModel->AnimKeyDataSize += ( sizeof( MV1_ANIM_KEY_MATRIX3X3 ) + sizeof( VECTOR ) + sizeof( float ) * 2 ) * KeySetR->Num ;

								// �V�����o�b�t�@���R�~�R�s��̃f�[�^���i�[����̂ɂ���
								NewKeySetR->Type = MV1_ANIMKEY_TYPE_MATRIX3X3 ;
								NewKeySetR->DataType = MV1_ANIMKEY_DATATYPE_MATRIX3X3 ;
								NewKeySetR->TotalTime = KeySetR->TotalTime ;
								NewKeySetR->Num = KeySetR->Num ;
								NewKeySetR->KeyMatrix3x3 = KeySetR->KeyMatrix3x3 ;
								NewKeySetR->KeyTime = ( float * )( NewKeyTrans + KeySetR->Num ) ;
								_MEMCPY( NewKeySetR->KeyTime, KeySetR->KeyTime, sizeof( float ) * KeySetR->Num ) ;

								// �����̃o�b�t�@�𕽍s�ړ��̃f�[�^���i�[����̂Ɏg��
								KeySetR->Type = MV1_ANIMKEY_TYPE_VECTOR ;
								KeySetR->DataType = MV1_ANIMKEY_DATATYPE_TRANSLATE ;
								KeySetR->KeyVector = NewKeyTrans ;

								Key  = NewKeySetR->KeyMatrix4x4C ;
								NewKeyTrans = KeySetR->KeyVector ;
								NewKeyMat3X3 = NewKeySetR->KeyMatrix3x3 ;
								for( k = 0 ; k < KeySetR->Num ; k ++, NewKeyTrans ++, NewKeyMat3X3 ++, Key ++ )
								{
									NewKeyMat3X3->Matrix[ 0 ][ 0 ] = Key->Matrix[ 0 ][ 0 ] ;
									NewKeyMat3X3->Matrix[ 0 ][ 1 ] = Key->Matrix[ 0 ][ 1 ] ;
									NewKeyMat3X3->Matrix[ 0 ][ 2 ] = Key->Matrix[ 0 ][ 2 ] ;
									NewKeyMat3X3->Matrix[ 1 ][ 0 ] = Key->Matrix[ 1 ][ 0 ] ;
									NewKeyMat3X3->Matrix[ 1 ][ 1 ] = Key->Matrix[ 1 ][ 1 ] ;
									NewKeyMat3X3->Matrix[ 1 ][ 2 ] = Key->Matrix[ 1 ][ 2 ] ;
									NewKeyMat3X3->Matrix[ 2 ][ 0 ] = Key->Matrix[ 2 ][ 0 ] ;
									NewKeyMat3X3->Matrix[ 2 ][ 1 ] = Key->Matrix[ 2 ][ 1 ] ;
									NewKeyMat3X3->Matrix[ 2 ][ 2 ] = Key->Matrix[ 2 ][ 2 ] ;
									NewKeyTrans->x = Key->Matrix[ 3 ][ 0 ] ;
									NewKeyTrans->y = Key->Matrix[ 3 ][ 1 ] ;
									NewKeyTrans->z = Key->Matrix[ 3 ][ 2 ] ;
								}
							}
						}
					}
					break ;
				}
			}
		}
	}


	// MV1_ANIMKEY_TYPE_MATRIX3X3 �^�C�v�� MV1_ANIMKEY_DATATYPE_MATRIX3X3 ��
	// MV1_ANIMKEY_TYPE_QUATERNION_VMD �^�C�v�� MV1_ANIMKEY_DATATYPE_ROTATE ��
	// MV1_ANIMKEY_TYPE_VECTOR �^�C�v�� MV1_ANIMKEY_DATATYPE_SCALE �ɕϊ�����
	if( ReadModel->AnimDataNotDecomposition == FALSE )
	{
		AnimR = ReadModel->AnimFirst ;
		for( i = 0 ; ( DWORD )i < ReadModel->AnimNum ; i ++, AnimR = AnimR->DataNext )
		{
			KeySetR = AnimR->AnimKeySetFirst ;
			for( j = 0 ; j < AnimR->AnimKeySetNum ; j ++, KeySetR = KeySetR->Next )
			{
				FLOAT4 *NewQKey, *NewBuffer ;
				VECTOR *NewSKey ;
				MV1_ANIM_KEY_MATRIX3X3 *Key ;
				MV1_ANIMKEYSET_R *NewKeySetR ;
				int Error, SizeValid ;

				if( KeySetR->DataType != MV1_ANIMKEY_DATATYPE_MATRIX3X3 ||
					KeySetR->Type     != MV1_ANIMKEY_TYPE_MATRIX3X3 )
					continue ;

				// �N�H�[�^�j�I�����g�p������]�l�ɕϊ�����

				// �L�[�T�C�Y�����炵�đ��₷
				ReadModel->AnimKeyDataSize -= ( sizeof( MV1_ANIM_KEY_MATRIX3X3 ) ) * KeySetR->Num ;
				ReadModel->AnimKeyDataSize += ( sizeof( FLOAT4 )                 ) * KeySetR->Num ;

				// �V�����L�[���ꎞ�I�Ɋi�[���郁�����̈�̊m��
				NewBuffer = ( FLOAT4 * )DXALLOC( ( sizeof( VECTOR ) + sizeof( FLOAT4 ) ) * KeySetR->Num ) ;
				if( NewBuffer )
				{
					// �^�C�v��ύX
					KeySetR->Type = MV1_ANIMKEY_TYPE_QUATERNION_VMD ;
					KeySetR->DataType = MV1_ANIMKEY_DATATYPE_ROTATE ;

					// ��]�I�[�_�[���Z�b�g
					AnimR->RotateOrder = MV1_ROTATE_ORDER_XYZ ;

					// �l��ϊ�
					Key    = KeySetR->KeyMatrix3x3 ;
					NewQKey = NewBuffer ;
					NewSKey = ( VECTOR * )( NewBuffer + KeySetR->Num ) ;
					Error = 0 ;
					SizeValid = 0 ;
					for( l = 0 ; Error != -1 && l < KeySetR->Num ; l ++, Key ++, NewQKey ++, NewSKey ++ )
					{
						VECTOR Size ;
						float s, trace ;

						NewSKey->x = _SQRT( Key->Matrix[ 0 ][ 0 ] * Key->Matrix[ 0 ][ 0 ] + Key->Matrix[ 1 ][ 0 ] * Key->Matrix[ 1 ][ 0 ] + Key->Matrix[ 2 ][ 0 ] * Key->Matrix[ 2 ][ 0 ] ) ;
						NewSKey->y = _SQRT( Key->Matrix[ 0 ][ 1 ] * Key->Matrix[ 0 ][ 1 ] + Key->Matrix[ 1 ][ 1 ] * Key->Matrix[ 1 ][ 1 ] + Key->Matrix[ 2 ][ 1 ] * Key->Matrix[ 2 ][ 1 ] ) ;
						NewSKey->z = _SQRT( Key->Matrix[ 0 ][ 2 ] * Key->Matrix[ 0 ][ 2 ] + Key->Matrix[ 1 ][ 2 ] * Key->Matrix[ 1 ][ 2 ] + Key->Matrix[ 2 ][ 2 ] * Key->Matrix[ 2 ][ 2 ] ) ;
						Size.x = 1.0f / NewSKey->x ;
						Size.y = 1.0f / NewSKey->y ;
						Size.z = 1.0f / NewSKey->z ;
						if( NewSKey->x < 0.999f || NewSKey->x > 1.0001f ||
							NewSKey->y < 0.999f || NewSKey->y > 1.0001f ||
							NewSKey->z < 0.999f || NewSKey->z > 1.0001f )
						{
							SizeValid = 1 ;
						}

						// ���K��
						Key->Matrix[ 0 ][ 0 ] *= Size.x ;
						Key->Matrix[ 1 ][ 0 ] *= Size.x ;
						Key->Matrix[ 2 ][ 0 ] *= Size.x ;

						Key->Matrix[ 0 ][ 1 ] *= Size.y ;
						Key->Matrix[ 1 ][ 1 ] *= Size.y ;
						Key->Matrix[ 2 ][ 1 ] *= Size.y ;

						Key->Matrix[ 0 ][ 2 ] *= Size.z ;
						Key->Matrix[ 1 ][ 2 ] *= Size.z ;
						Key->Matrix[ 2 ][ 2 ] *= Size.z ;

						trace = Key->Matrix[ 0 ][ 0 ] + Key->Matrix[ 1 ][ 1 ] + Key->Matrix[ 2 ][ 2 ] + 1.0f ;
						if( trace >= 1.0f )
						{
							s = 0.5f / _SQRT( trace ) ;
							NewQKey->w = 0.25f / s ;
							NewQKey->x = ( Key->Matrix[ 1 ][ 2 ] - Key->Matrix[ 2 ][ 1 ] ) * s ;
							NewQKey->y = ( Key->Matrix[ 2 ][ 0 ] - Key->Matrix[ 0 ][ 2 ] ) * s ;
							NewQKey->z = ( Key->Matrix[ 0 ][ 1 ] - Key->Matrix[ 1 ][ 0 ] ) * s ;
						}
						else
						{
							float Max ;
							Max = Key->Matrix[ 1 ][ 1 ] > Key->Matrix[ 2 ][ 2 ] ? Key->Matrix[ 1 ][ 1 ] : Key->Matrix[ 2 ][ 2 ] ;
							if( Max < Key->Matrix[ 0 ][ 0 ] )
							{
								s = _SQRT( Key->Matrix[ 0 ][ 0 ] - ( Key->Matrix[ 1 ][ 1 ] + Key->Matrix[ 2 ][ 2 ] ) + 1.0f ) ;
								NewQKey->x = s * 0.5f ;
								s = 0.5f / s ;
								NewQKey->y = ( Key->Matrix[ 0 ][ 1 ] + Key->Matrix[ 1 ][ 0 ] ) * s ;
								NewQKey->z = ( Key->Matrix[ 2 ][ 0 ] + Key->Matrix[ 0 ][ 2 ] ) * s ;
								NewQKey->w = ( Key->Matrix[ 1 ][ 2 ] - Key->Matrix[ 2 ][ 1 ] ) * s ;
							}
							else
							if( Max == Key->Matrix[ 1 ][ 1 ] )
							{
								s = _SQRT( Key->Matrix[ 1 ][ 1 ] - ( Key->Matrix[ 2 ][ 2 ] + Key->Matrix[ 0 ][ 0 ] ) + 1.0f ) ;
								NewQKey->y = s * 0.5f ;
								s = 0.5f / s ;
								NewQKey->x = ( Key->Matrix[ 0 ][ 1 ] + Key->Matrix[ 1 ][ 0 ] ) * s ;
								NewQKey->z = ( Key->Matrix[ 1 ][ 2 ] + Key->Matrix[ 2 ][ 1 ] ) * s ;
								NewQKey->w = ( Key->Matrix[ 2 ][ 0 ] - Key->Matrix[ 0 ][ 2 ] ) * s ;
							}
							else
							{
								s = _SQRT( Key->Matrix[ 2 ][ 2 ] - ( Key->Matrix[ 0 ][ 0 ] + Key->Matrix[ 1 ][ 1 ] ) + 1.0f ) ;
								NewQKey->z = s * 0.5f ;
								s = 0.5f / s ;
								NewQKey->x = ( Key->Matrix[ 2 ][ 0 ] + Key->Matrix[ 0 ][ 2 ] ) * s ;
								NewQKey->y = ( Key->Matrix[ 1 ][ 2 ] + Key->Matrix[ 2 ][ 1 ] ) * s ;
								NewQKey->w = ( Key->Matrix[ 0 ][ 1 ] - Key->Matrix[ 1 ][ 0 ] ) * s ;
							}
						}
					}

					// �N�H�[�^�j�I���̓��e���R�s�[
					_MEMCPY( KeySetR->KeyFloat4, NewBuffer, sizeof( FLOAT4 ) * KeySetR->Num ) ;

					// �T�C�Y�L�[���L���������ꍇ�̓T�C�Y�L�[�Z�b�g��ǉ�����
					if( SizeValid )
					{
						NewKeySetR = MV1RAddAnimKeySet( ReadModel, AnimR ) ;
						if( NewKeySetR )
						{
							// �����镪�̃f�[�^�T�C�Y�����Z����
							ReadModel->AnimKeyDataSize += ( sizeof( float ) + sizeof( VECTOR ) ) * KeySetR->Num ;

							// �L�[�Z�b�g�����Z�b�g
							NewKeySetR->Type = MV1_ANIMKEY_TYPE_VECTOR ;
							NewKeySetR->DataType = MV1_ANIMKEY_DATATYPE_SCALE ;
							NewKeySetR->TotalTime = KeySetR->TotalTime ;
							NewKeySetR->Num = KeySetR->Num ;
							NewKeySetR->KeyVector = ( VECTOR * )( KeySetR->KeyFloat4 + KeySetR->Num ) ;
							NewKeySetR->KeyTime = ( float * )( NewKeySetR->KeyVector + KeySetR->Num ) ;
							_MEMCPY( NewKeySetR->KeyTime, KeySetR->KeyTime, sizeof( float ) * KeySetR->Num ) ;

							// �L�[�����o�b�t�@����R�s�[
							_MEMCPY( NewKeySetR->KeyVector, NewBuffer + KeySetR->Num, sizeof( VECTOR ) * KeySetR->Num ) ;
						}
					}

					// �ꎞ�I�Ɋm�ۂ������������������
					DXFREE( NewBuffer ) ;
				}

				break ;
			}
		}
	}
#endif 


	// MV1_ANIMKEY_TYPE_MATRIX3X3 �^�C�v�� MV1_ANIMKEY_DATATYPE_MATRIX3X3 ��
	// MV1_ANIMKEY_TYPE_VECTOR �^�C�v�� MV1_ANIMKEY_DATATYPE_ROTATE �ɕϊ�����
	AnimR = ReadModel->AnimFirst ;
	for( i = 0 ; ( DWORD )i < ReadModel->AnimNum ; i ++, AnimR = AnimR->DataNext )
	{
		KeySetR = AnimR->AnimKeySetFirst ;
		for( j = 0 ; j < AnimR->AnimKeySetNum ; j ++, KeySetR = KeySetR->Next )
		{
			VECTOR *NewKey, *NewBuffer ;
			MV1_ANIM_KEY_MATRIX3X3 *Key ;
			MATRIX TempMat ;
			int Error ;

			if( KeySetR->DataType != MV1_ANIMKEY_DATATYPE_MATRIX3X3 ||
				KeySetR->Type     != MV1_ANIMKEY_TYPE_MATRIX3X3 )
				continue ;

			// ��]�l�ɕϊ�����

			// �L�[�T�C�Y�����炵�đ��₷
			ReadModel->AnimKeyDataSize -= ( sizeof( MV1_ANIM_KEY_MATRIX3X3 ) ) * KeySetR->Num ;
			ReadModel->AnimKeyDataSize += ( sizeof( VECTOR )                   ) * KeySetR->Num ;

			// �V�����L�[���ꎞ�I�Ɋi�[���郁�����̈�̊m��
			NewBuffer = ( VECTOR * )DXALLOC( sizeof( VECTOR ) * KeySetR->Num ) ;
			if( NewBuffer )
			{
				// �W���o�����b�N���������Ȃ���]�I�[�_�[�����o����
				for( k = 0 ; k < 6 ; k ++ )
				{
					// �l��ϊ�
					Key    = KeySetR->KeyMatrix3x3 ;
					NewKey = NewBuffer ;
					Error = 0 ;
					for( l = 0 ; Error != -1 && l < KeySetR->Num ; l ++, Key ++, NewKey ++ )
					{
						TempMat.m[ 0 ][ 0 ] = Key->Matrix[ 0 ][ 0 ] ;
						TempMat.m[ 0 ][ 1 ] = Key->Matrix[ 0 ][ 1 ] ;
						TempMat.m[ 0 ][ 2 ] = Key->Matrix[ 0 ][ 2 ] ;
						TempMat.m[ 0 ][ 3 ] = 0.0f ;
						TempMat.m[ 1 ][ 0 ] = Key->Matrix[ 1 ][ 0 ] ;
						TempMat.m[ 1 ][ 1 ] = Key->Matrix[ 1 ][ 1 ] ;
						TempMat.m[ 1 ][ 2 ] = Key->Matrix[ 1 ][ 2 ] ;
						TempMat.m[ 1 ][ 3 ] = 0.0f ;
						TempMat.m[ 2 ][ 0 ] = Key->Matrix[ 2 ][ 0 ] ;
						TempMat.m[ 2 ][ 1 ] = Key->Matrix[ 2 ][ 1 ] ;
						TempMat.m[ 2 ][ 2 ] = Key->Matrix[ 2 ][ 2 ] ;
						TempMat.m[ 2 ][ 3 ] = 0.0f ;
						TempMat.m[ 3 ][ 0 ] = 0.0f ;
						TempMat.m[ 3 ][ 1 ] = 0.0f ;
						TempMat.m[ 3 ][ 2 ] = 0.0f ;
						TempMat.m[ 3 ][ 3 ] = 1.0f ;
						switch( k )
						{
						case MV1_ROTATE_ORDER_XYZ : Error = GetMatrixXYZRotation( &TempMat, &NewKey->x, &NewKey->y, &NewKey->z ) ; break ;
						case MV1_ROTATE_ORDER_XZY : Error = GetMatrixXZYRotation( &TempMat, &NewKey->x, &NewKey->y, &NewKey->z ) ; break ;
						case MV1_ROTATE_ORDER_YXZ : Error = GetMatrixYXZRotation( &TempMat, &NewKey->x, &NewKey->y, &NewKey->z ) ; break ;
						case MV1_ROTATE_ORDER_YZX : Error = GetMatrixYZXRotation( &TempMat, &NewKey->x, &NewKey->y, &NewKey->z ) ; break ;
						case MV1_ROTATE_ORDER_ZXY : Error = GetMatrixZXYRotation( &TempMat, &NewKey->x, &NewKey->y, &NewKey->z ) ; break ;
						case MV1_ROTATE_ORDER_ZYX : Error = GetMatrixZYXRotation( &TempMat, &NewKey->x, &NewKey->y, &NewKey->z ) ; break ;
						}
					}
					if( Error == 0 || k == 5 ) break ;
				}

				// �^�C�v��ύX
				KeySetR->Type = MV1_ANIMKEY_TYPE_VECTOR ;
				KeySetR->DataType = MV1_ANIMKEY_DATATYPE_ROTATE ;

				// ��]�I�[�_�[���Z�b�g
				AnimR->RotateOrder = k ;

				// �o�b�t�@�̓��e���R�s�[���Ĉꎞ�I�Ɋm�ۂ������������������
				_MEMCPY( KeySetR->KeyVector, NewBuffer, sizeof( VECTOR ) * KeySetR->Num ) ;
				DXFREE( NewBuffer ) ;

				// ��]�l�̔�т��C������
				NewKey = KeySetR->KeyVector + 1 ;
				for( l = 1 ; l < KeySetR->Num ; l ++, NewKey ++ )
				{
					f = NewKey->x - NewKey[ -1 ].x ;
					if( f >  ( float )DX_PI / 2.0f )
					{
						NewKey->x -= ( float )DX_PI * 2 ;
					}
					else
					if( f < -( float )DX_PI / 2.0f )
					{
						NewKey->x += ( float )DX_PI * 2 ;
					}

					f = NewKey->y - NewKey[ -1 ].y ;
					if( f >  ( float )DX_PI / 2.0f )
					{
						NewKey->y -= ( float )DX_PI * 2 ;
					}
					else
					if( f < -( float )DX_PI / 2.0f )
					{
						NewKey->y += ( float )DX_PI * 2 ;
					}

					f = NewKey->z - NewKey[ -1 ].z ;
					if( f >  ( float )DX_PI / 2.0f )
					{
						NewKey->z -= ( float )DX_PI * 2 ;
					}
					else
					if( f < -( float )DX_PI / 2.0f )
					{
						NewKey->z += ( float )DX_PI * 2 ;
					}
				}
			}
		}
	}

	// ���`��Ԃōςރf�[�^���폜���邩�A���点�鐔�����Ȃ����Ԋu�ŕ��ԃL�[�������ꍇ�͌Œ�Ԋu�ɂ���
	AnimR = ReadModel->AnimFirst ;
	for( i = 0 ; ( DWORD )i < ReadModel->AnimNum ; i ++, AnimR = AnimR->DataNext )
	{
		KeySetR = AnimR->AnimKeySetFirst ;
		for( j = 0 ; j < AnimR->AnimKeySetNum ; j ++, KeySetR = KeySetR->Next )
		{
			MV1OptimizeKeySetTimeType( ReadModel, KeySetR, true ) ;
		}
	}

	// MV1_ANIMKEY_TYPE_VECTOR �^�C�v�̃L�[�𕪉��ł��邩( �������������f�[�^�T�C�Y���������Ȃ邩 )����
	// �������������ǂ��ꍇ�͕�������
	if( ReadModel->AnimDataNotDecomposition == FALSE )
	{
		AnimR = ReadModel->AnimFirst ;
		for( i = 0 ; ( DWORD )i < ReadModel->AnimNum ; i ++, AnimR = AnimR->DataNext )
		{
			// �s�񂩂��]�x�N�g���ɂ����ꍇ�͗v�f�P�ʂŕ������Ȃ�
			if( AnimR->Container->IsMatrixLinearBlend ) continue ;

			KeySetR = AnimR->AnimKeySetFirst ;
			for( j = 0 ; j < AnimR->AnimKeySetNum ; j ++, KeySetR = KeySetR->Next )
			{
				bool ix, iy, iz, comp ;
				int count ;
				VECTOR *Key ;
				int AfterSize, BeginSize ;

				if( KeySetR->Type != MV1_ANIMKEY_TYPE_VECTOR )
					continue ;

				// �L�[�̐����Q�������Ȃ��Ƃ��͉������Ȃ�
				if( KeySetR->Num < 2 )
					continue ;

				// �l�̌���
				ix = true;
				iy = true;
				iz = true;
				Key    = KeySetR->KeyVector + 1 ;
				for( k = 1 ; k < KeySetR->Num ; k ++, Key ++ )
				{
					f = KeySetR->KeyVector[ 0 ].x - Key->x ; if( f < -0.000001f || f > 0.000001f ) ix = false ;
					f = KeySetR->KeyVector[ 0 ].y - Key->y ; if( f < -0.000001f || f > 0.000001f ) iy = false ;
					f = KeySetR->KeyVector[ 0 ].z - Key->z ; if( f < -0.000001f || f > 0.000001f ) iz = false ;
					if( ix == false && iy == false && iz == false ) break ;
				}

				// ����������ł��������Ȃ��ꍇ�͉��������I��
				if( ix == false && iy == false && iz == false )
					continue ;

				// ���������ꍇ�̑��f�[�^�T�C�Y�ƁA�������Ȃ��ꍇ�̃T�C�Y���r���āA�������������ǂ��ꍇ�͕�������
				BeginSize = ( int )( sizeof( VECTOR ) * KeySetR->Num + sizeof( MV1_ANIM_KEYSET_BASE ) ) ;
				if( KeySetR->TimeType == MV1_ANIMKEY_TIME_TYPE_KEY )
					BeginSize += sizeof( float ) * KeySetR->Num ;

				count = 0 ;
				if( ix == false ) count ++ ;
				if( iy == false ) count ++ ;
				if( iz == false ) count ++ ;
				AfterSize = ( int )( sizeof( float ) * KeySetR->Num + sizeof( MV1_ANIM_KEYSET_BASE ) ) ;
				if( KeySetR->TimeType == MV1_ANIMKEY_TIME_TYPE_KEY )
					AfterSize += sizeof( float ) * KeySetR->Num ;
				AfterSize *= count ;

				comp = false ;
	//			if( AfterSize < BeginSize )
				if( 1 )
				{
					MV1_ANIMKEYSET_R *NewKeySetRY, *NewKeySetRZ ;

					NewKeySetRY = NULL ;
					NewKeySetRZ = NULL ;

					NewKeySetRY = MV1RAddAnimKeySet( ReadModel, AnimR ) ;
					if( NewKeySetRY == NULL ) goto BEND ;

					NewKeySetRZ = MV1RAddAnimKeySet( ReadModel, AnimR ) ;
					if( NewKeySetRZ == NULL ) goto BEND ;

					NewKeySetRY->TotalTime = KeySetR->TotalTime ;
					NewKeySetRZ->TotalTime = KeySetR->TotalTime ;
					NewKeySetRY->Type = MV1_ANIMKEY_TYPE_LINEAR ;
					NewKeySetRZ->Type = MV1_ANIMKEY_TYPE_LINEAR ;
					NewKeySetRY->Num = KeySetR->Num ;
					NewKeySetRZ->Num = KeySetR->Num ;
					NewKeySetRY->TimeType = KeySetR->TimeType ;
					NewKeySetRZ->TimeType = KeySetR->TimeType ;
					NewKeySetRY->UnitTime = KeySetR->UnitTime ;
					NewKeySetRZ->UnitTime = KeySetR->UnitTime ;
					NewKeySetRY->StartTime = KeySetR->StartTime ;
					NewKeySetRZ->StartTime = KeySetR->StartTime ;

					if( KeySetR->TimeType == MV1_ANIMKEY_TIME_TYPE_KEY )
					{
						NewKeySetRY->KeyTime = ( float * )ADDMEMAREA( sizeof( float ) * KeySetR->Num * 2, &ReadModel->Mem ) ;
						if( NewKeySetRY->KeyTime == NULL ) goto BEND ;
						NewKeySetRZ->KeyTime = NewKeySetRY->KeyTime + KeySetR->Num ;
						_MEMCPY( NewKeySetRY->KeyTime, KeySetR->KeyTime, sizeof( float ) * KeySetR->Num ) ;
						_MEMCPY( NewKeySetRZ->KeyTime, KeySetR->KeyTime, sizeof( float ) * KeySetR->Num ) ;
					}

					NewKeySetRY->KeyLinear = ( float * )ADDMEMAREA( sizeof( float ) * KeySetR->Num * 4, &ReadModel->Mem ) ;
					if( NewKeySetRY->KeyLinear == NULL ) goto BEND ;
					NewKeySetRZ->KeyLinear = NewKeySetRY->KeyLinear + KeySetR->Num ;

					KeySetR->Type = MV1_ANIMKEY_TYPE_LINEAR ;
					switch( KeySetR->DataType )
					{
					case MV1_ANIMKEY_DATATYPE_ROTATE :
						KeySetR->DataType     = MV1_ANIMKEY_DATATYPE_ROTATE_X ;
						NewKeySetRY->DataType = MV1_ANIMKEY_DATATYPE_ROTATE_Y ;
						NewKeySetRZ->DataType = MV1_ANIMKEY_DATATYPE_ROTATE_Z ;
						break ;

					case MV1_ANIMKEY_DATATYPE_SCALE :
						KeySetR->DataType     = MV1_ANIMKEY_DATATYPE_SCALE_X ;
						NewKeySetRY->DataType = MV1_ANIMKEY_DATATYPE_SCALE_Y ;
						NewKeySetRZ->DataType = MV1_ANIMKEY_DATATYPE_SCALE_Z ;
						break ;

					case MV1_ANIMKEY_DATATYPE_TRANSLATE :
						KeySetR->DataType     = MV1_ANIMKEY_DATATYPE_TRANSLATE_X ;
						NewKeySetRY->DataType = MV1_ANIMKEY_DATATYPE_TRANSLATE_Y ;
						NewKeySetRZ->DataType = MV1_ANIMKEY_DATATYPE_TRANSLATE_Z ;
						break ;
					}

					for( k = 0 ; k < KeySetR->Num ; k ++ )
					{
						KeySetR->KeyLinear[ k ]     = KeySetR->KeyVector[ k ].x ;
						NewKeySetRY->KeyLinear[ k ] = KeySetR->KeyVector[ k ].y ;
						NewKeySetRZ->KeyLinear[ k ] = KeySetR->KeyVector[ k ].z ;
					}

					// �L�[�f�[�^�̃T�C�Y�����炵�đ��₷
					ReadModel->AnimKeyDataSize -= sizeof( float ) * KeySetR->Num ;
					ReadModel->AnimKeyDataSize += sizeof( float ) * KeySetR->Num * 3 ;

					// �������L�[���X�œK������
					MV1OptimizeKeySetTimeType( ReadModel, KeySetR ) ;
					MV1OptimizeKeySetTimeType( ReadModel, NewKeySetRY ) ;
					MV1OptimizeKeySetTimeType( ReadModel, NewKeySetRZ ) ;

					comp = true ;

	BEND :
					if( comp == false )
					{
						if( NewKeySetRY )
						{
							KEYSETLIST_SUB( NewKeySetRY, AnimR, ReadModel ) ;
						}

						if( NewKeySetRZ )
						{
							KEYSETLIST_SUB( NewKeySetRZ, AnimR, ReadModel ) ;
						}
					}
				}
			}
		}
	}

	// ������x���`��Ԃōςރf�[�^���폜���邩�A���点�鐔�����Ȃ����Ԋu�ŕ��ԃL�[�������ꍇ�͌Œ�Ԋu�ɂ���
	AnimR = ReadModel->AnimFirst ;
	for( i = 0 ; ( DWORD )i < ReadModel->AnimNum ; i ++, AnimR = AnimR->DataNext )
	{
		KeySetR = AnimR->AnimKeySetFirst ;
		for( j = 0 ; j < AnimR->AnimKeySetNum ; j ++, KeySetR = KeySetR->Next )
		{
			MV1OptimizeKeySetTimeType( ReadModel, KeySetR, true ) ;
		}
	}
	
	// �����^�C�v�̃L�[�Z�b�g���v�f���ɕ�����Ă������ɂ���
	AnimR = ReadModel->AnimFirst ;
	for( i = 0 ; ( DWORD )i < ReadModel->AnimNum ; i ++, AnimR = AnimR->DataNext )
	{
		int fx, fy, fz, conv ;
		MV1_ANIMKEYSET_R *kx = NULL, *ky = NULL, *kz = NULL ;

		// �L�[�̐��������ȕ��s�ړ��L�[�Z�b�g���w�x�y�Ƒ����Ă������ɂ���
		KeySetR = AnimR->AnimKeySetFirst ;
		fx = 0 ;
		fy = 0 ;
		fz = 0 ;
		for( j = 0 ; j < AnimR->AnimKeySetNum ; j ++, KeySetR = KeySetR->Next )
		{
			switch( KeySetR->DataType )
			{
			case MV1_ANIMKEY_DATATYPE_TRANSLATE_X : fx = 1 ; kx = KeySetR ; break ;
			case MV1_ANIMKEY_DATATYPE_TRANSLATE_Y : fy = 1 ; ky = KeySetR ; break ;
			case MV1_ANIMKEY_DATATYPE_TRANSLATE_Z : fz = 1 ; kz = KeySetR ; break ;
			}
		}
		if( fx && fy && fz &&
			kx->Num == ky->Num && kx->Num == kz->Num )
		{
			conv = 0 ;
			if( kx->TimeType == MV1_ANIMKEY_TIME_TYPE_ONE &&
				ky->TimeType == MV1_ANIMKEY_TIME_TYPE_ONE &&
				kz->TimeType == MV1_ANIMKEY_TIME_TYPE_ONE )
			{
				if( kx->UnitTime  == ky->UnitTime  && kx->UnitTime  == kz->UnitTime &&
				    kx->StartTime - ky->StartTime > -0.0000001f && kx->StartTime - ky->StartTime < 0.0000001f &&
					kx->StartTime - kz->StartTime > -0.0000001f && kx->StartTime - kz->StartTime < 0.0000001f )
				{
					conv = 1 ;
				}
			}
			else
			if( kx->TimeType == MV1_ANIMKEY_TIME_TYPE_KEY &&
				ky->TimeType == MV1_ANIMKEY_TIME_TYPE_KEY &&
				kz->TimeType == MV1_ANIMKEY_TIME_TYPE_KEY )
			{
				for( j = 0; j < kx->Num; j++ )
				{
					if( *( ( DWORD * )&kx->KeyTime[ j ] ) != *( ( DWORD * )&ky->KeyTime[ j ] ) ||
						*( ( DWORD * )&kx->KeyTime[ j ] ) != *( ( DWORD * )&kz->KeyTime[ j ] ) )
						break ;
				}
				if( j == kx->Num )
				{
					conv = 1 ;
				}
			}

			if( conv )
			{
				VECTOR *NewBuffer ;
				NewBuffer = ( VECTOR * )ADDMEMAREA( sizeof( VECTOR ) * kx->Num, &ReadModel->Mem ) ;
				if( NewBuffer )
				{
					for( j = 0 ; j < kx->Num ; j ++ )
					{
						NewBuffer[ j ].x = kx->KeyLinear[ j ] ;
						NewBuffer[ j ].y = ky->KeyLinear[ j ] ;
						NewBuffer[ j ].z = kz->KeyLinear[ j ] ;
					}

					kx->DataType = MV1_ANIMKEY_DATATYPE_TRANSLATE ;
					kx->Type = MV1_ANIMKEY_TYPE_VECTOR ;
					kx->KeyVector = NewBuffer ;

					KEYSETLIST_SUB( ky, AnimR, ReadModel ) ;
					KEYSETLIST_SUB( kz, AnimR, ReadModel ) ;
				}
			}
		}

		// �L�[�̐��������Ȋg��L�[�Z�b�g���w�x�y�Ƒ����Ă������ɂ���
		KeySetR = AnimR->AnimKeySetFirst ;
		fx = 0 ;
		fy = 0 ;
		fz = 0 ;
		for( j = 0 ; j < AnimR->AnimKeySetNum ; j ++, KeySetR = KeySetR->Next )
		{
			switch( KeySetR->DataType )
			{
			case MV1_ANIMKEY_DATATYPE_SCALE_X : fx = 1 ; kx = KeySetR ; break ;
			case MV1_ANIMKEY_DATATYPE_SCALE_Y : fy = 1 ; ky = KeySetR ; break ;
			case MV1_ANIMKEY_DATATYPE_SCALE_Z : fz = 1 ; kz = KeySetR ; break ;
			}
		}
		if( fx && fy && fz &&
			kx->Num == ky->Num && kx->Num == kz->Num )
		{
			conv = 0 ;
			if( kx->TimeType == MV1_ANIMKEY_TIME_TYPE_ONE &&
				ky->TimeType == MV1_ANIMKEY_TIME_TYPE_ONE &&
				kz->TimeType == MV1_ANIMKEY_TIME_TYPE_ONE )
			{
				if( kx->UnitTime  == ky->UnitTime               && kx->UnitTime  == kz->UnitTime &&
				    kx->StartTime - ky->StartTime > -0.0000001f && kx->StartTime - ky->StartTime < 0.0000001f &&
					kx->StartTime - kz->StartTime > -0.0000001f && kx->StartTime - kz->StartTime < 0.0000001f )
				{
					conv = 1 ;
				}
			}
			else
			if( kx->TimeType == MV1_ANIMKEY_TIME_TYPE_KEY &&
				ky->TimeType == MV1_ANIMKEY_TIME_TYPE_KEY &&
				kz->TimeType == MV1_ANIMKEY_TIME_TYPE_KEY )
			{
				for( j = 0; j < kx->Num; j++ )
				{
					if( *( ( DWORD * )&kx->KeyTime[ j ] ) != *( ( DWORD * )&ky->KeyTime[ j ] ) ||
						*( ( DWORD * )&kx->KeyTime[ j ] ) != *( ( DWORD * )&kz->KeyTime[ j ] ) )
						break ;
				}
				if( j == kx->Num )
				{
					conv = 1 ;
				}
			}

			if( conv )
			{
				VECTOR *NewBuffer ;
				NewBuffer = ( VECTOR * )ADDMEMAREA( sizeof( VECTOR ) * kx->Num, &ReadModel->Mem ) ;
				if( NewBuffer )
				{
					for( j = 0 ; j < kx->Num ; j ++ )
					{
						NewBuffer[ j ].x = kx->KeyLinear[ j ] ;
						NewBuffer[ j ].y = ky->KeyLinear[ j ] ;
						NewBuffer[ j ].z = kz->KeyLinear[ j ] ;
					}

					kx->DataType = MV1_ANIMKEY_DATATYPE_SCALE ;
					kx->Type = MV1_ANIMKEY_TYPE_VECTOR ;
					kx->KeyVector = NewBuffer ;

					KEYSETLIST_SUB( ky, AnimR, ReadModel ) ;
					KEYSETLIST_SUB( kz, AnimR, ReadModel ) ;
				}
			}
		}

		// �L�[�̐��������ȉ�]�L�[�Z�b�g���w�x�y�Ƒ����Ă������ɂ���
		KeySetR = AnimR->AnimKeySetFirst ;
		fx = 0 ;
		fy = 0 ;
		fz = 0 ;
		for( j = 0 ; j < AnimR->AnimKeySetNum ; j ++, KeySetR = KeySetR->Next )
		{
			switch( KeySetR->DataType )
			{
			case MV1_ANIMKEY_DATATYPE_ROTATE_X : fx = 1 ; kx = KeySetR ; break ;
			case MV1_ANIMKEY_DATATYPE_ROTATE_Y : fy = 1 ; ky = KeySetR ; break ;
			case MV1_ANIMKEY_DATATYPE_ROTATE_Z : fz = 1 ; kz = KeySetR ; break ;
			}
		}
		if( fx && fy && fz &&
			kx->Num == ky->Num && kx->Num == kz->Num )
		{
			conv = 0 ;
			if( kx->TimeType == MV1_ANIMKEY_TIME_TYPE_ONE &&
				ky->TimeType == MV1_ANIMKEY_TIME_TYPE_ONE &&
				kz->TimeType == MV1_ANIMKEY_TIME_TYPE_ONE )
			{
				if( kx->UnitTime  == ky->UnitTime  && kx->UnitTime  == kz->UnitTime &&
				    kx->StartTime - ky->StartTime > -0.0000001f && kx->StartTime - ky->StartTime < 0.0000001f &&
					kx->StartTime - kz->StartTime > -0.0000001f && kx->StartTime - kz->StartTime < 0.0000001f )
				{
					conv = 1 ;
				}
			}
			else
			if( kx->TimeType == MV1_ANIMKEY_TIME_TYPE_KEY &&
				ky->TimeType == MV1_ANIMKEY_TIME_TYPE_KEY &&
				kz->TimeType == MV1_ANIMKEY_TIME_TYPE_KEY )
			{
				for( j = 0; j < kx->Num; j++ )
				{
					if( *( ( DWORD * )&kx->KeyTime[ j ] ) != *( ( DWORD * )&ky->KeyTime[ j ] ) ||
						*( ( DWORD * )&kx->KeyTime[ j ] ) != *( ( DWORD * )&kz->KeyTime[ j ] ) )
						break ;
				}
				if( j == kx->Num )
				{
					conv = 1 ;
				}
			}

			if( conv )
			{
				VECTOR *NewBuffer ;
				NewBuffer = ( VECTOR * )ADDMEMAREA( sizeof( VECTOR ) * kx->Num, &ReadModel->Mem ) ;
				if( NewBuffer )
				{
					for( j = 0 ; j < kx->Num ; j ++ )
					{
						NewBuffer[ j ].x = kx->KeyLinear[ j ] ;
						NewBuffer[ j ].y = ky->KeyLinear[ j ] ;
						NewBuffer[ j ].z = kz->KeyLinear[ j ] ;
					}

					kx->DataType = MV1_ANIMKEY_DATATYPE_ROTATE ;
					kx->Type = MV1_ANIMKEY_TYPE_VECTOR ;
					kx->KeyVector = NewBuffer ;

					KEYSETLIST_SUB( ky, AnimR, ReadModel ) ;
					KEYSETLIST_SUB( kz, AnimR, ReadModel ) ;
				}
			}
		}
	}

	// �폜�ł���L�[������ꍇ�͍폜����
	AnimR = ReadModel->AnimFirst ;
	for( i = 0 ; ( DWORD )i < ReadModel->AnimNum ; i ++, AnimR = AnimR->DataNext )
	{
		KeySetR = AnimR->AnimKeySetFirst ;
		for( j = 0 ; j < AnimR->AnimKeySetNum ; j ++, KeySetR = KeySetR->Next )
		{
			MV1OptimizeKeySetTimeType( ReadModel, KeySetR ) ;
		}
	}

	return 0 ;
/*
	// 16bit�l�ɕϊ�����
	AnimR = ReadModel->AnimFirst ;
	for( i = 0 ; ( DWORD )i < ReadModel->AnimNum ; i ++, AnimR = AnimR->DataNext )
	{
		KeySetR = AnimR->AnimKeySetFirst ;
		for( j = 0 ; j < AnimR->AnimKeySetNum ; j ++, KeySetR = KeySetR->Next )
		{
			if( KeySetR->Type == MV1_ANIMKEY_TYPE_QUATERNION ) continue ;

			// ���ɂP�U�r�b�g�������牽�����Ȃ�
			if( KeySetR->Bit16 ) continue ;

			// �L�[���P�����������牽�����Ȃ�
			if( KeySetR->Num <= 1 ) continue ;

			// ���ԃf�[�^�������Œ菬���_��
			TimeSub.Unit = 1.0f ;
			TimeSub.Min = 0.0f ;
			if( KeySetR->TimeType == MV1_ANIMKEY_TIME_TYPE_KEY )
			{
				Time = KeySetR->KeyTime ;
				TimeSub.Min  = Time[ 0 ] ;
				TimeSub.Unit = Time[ 0 ] ;
				Time ++ ;
				for( k = 1 ; k < KeySetR->Num ; k ++, Time ++ )
				{
					if( TimeSub.Min  > *Time ) TimeSub.Min  = *Time ;
					if( TimeSub.Unit < *Time ) TimeSub.Unit = *Time ;
				}

				// 16bit�⏕���̍쐬
				KeySetR->Time16BSub.Min  = MV1AnimKey16BitMinFtoB( TimeSub.Min  ) ;
				TimeSub.Min              = MV1AnimKey16BitMinBtoF( KeySetR->Time16BSub.Min ) ;
				TimeSub.Unit            -= TimeSub.Min ;
				if( TimeSub.Unit < 0.00000001f )
					TimeSub.Unit = 1.0f ;
				TimeSub.Unit            /= 60000.0f ;
				KeySetR->Time16BSub.Unit = MV1AnimKey16BitUnitFtoB( TimeSub.Unit ) ;
				TimeSub.Unit             = MV1AnimKey16BitUnitBtoF( KeySetR->Time16BSub.Unit ) ;

				// �l��ϊ�
				Time       = KeySetR->KeyTime ;
				NewTimeB16 = KeySetR->KeyTimeB16 ;
				for( k = 0 ; k < KeySetR->Num ; k ++, Time ++, NewTimeB16 ++ )
				{
					*NewTimeB16 = ( WORD )( _FTOL( ( *Time - TimeSub.Min ) / TimeSub.Unit ) ) ;
				}
			}
			
			switch( KeySetR->Type )
			{
			case MV1_ANIMKEY_TYPE_LINEAR :
				{
					WORD *NewKey ;
					float *Key ;

					// �ŏ��l�ƍő�l���擾
					KeySub.Min  =  1000000000000.0f ;
					KeySub.Unit = -1000000000000.0f ;
					Key = KeySetR->KeyLinear ;
					for( k = 0 ; k < KeySetR->Num ; k ++, Key ++ )
					{
						 if( KeySub.Min  > *Key ) KeySub.Min  = *Key ;
						 if( KeySub.Unit < *Key ) KeySub.Unit = *Key ;
					}

					// 16bit�⏕���̍쐬
					KeySetR->Key16BSub.Min  = MV1AnimKey16BitMinFtoB( KeySub.Min  ) ;
					KeySub.Min              = MV1AnimKey16BitMinBtoF( KeySetR->Key16BSub.Min ) ;
					KeySub.Unit            -= KeySub.Min ;
					if( KeySub.Unit < 0.00000001f )
						KeySub.Unit = 1.0f ;
					KeySub.Unit            /= 60000.0f ;
					KeySetR->Key16BSub.Unit = MV1AnimKey16BitUnitFtoB( KeySub.Unit ) ;
					KeySub.Unit             = MV1AnimKey16BitUnitBtoF( KeySetR->Key16BSub.Unit ) ;

					// 16bit�P�ӂ�̒l�� 0.1f �𒴂��Ă��܂����� 16bit�����Ȃ�
					if( KeySub.Unit >= 0.1f ) continue ;

					// �l��ϊ�
					Key        = KeySetR->KeyLinear ;
					NewKey     = KeySetR->KeyLinearB16 ;
					for( k = 0 ; k < KeySetR->Num ; k ++, Key ++, NewKey ++ )
					{
						*( ( WORD * )NewKey ) = ( WORD )( _FTOL( ( *Key - KeySub.Min ) / KeySub.Unit ) ) ;
					}

					// �L�[�T�C�Y�����炵�đ��₷
					ReadModel->AnimKeyDataSize -= sizeof( float ) * KeySetR->Num ;
					ReadModel->AnimKeyDataSize += sizeof( WORD  ) * KeySetR->Num ;
				}
				break ;

			case MV1_ANIMKEY_TYPE_VECTOR :
				{
					VECTOR_B16 *NewKey ;
					VECTOR *Key ;

					// �ŏ��l�ƍő�l���擾
					KeySub.Min  =  1000000000000.0f ;
					KeySub.Unit = -1000000000000.0f ;
					Key = KeySetR->KeyVector ;
					for( k = 0 ; k < KeySetR->Num ; k ++, Key ++ )
					{
						 if( KeySub.Min  > Key->x ) KeySub.Min  = Key->x ;
						 if( KeySub.Unit < Key->x ) KeySub.Unit = Key->x ;
						 if( KeySub.Min  > Key->y ) KeySub.Min  = Key->y ;
						 if( KeySub.Unit < Key->y ) KeySub.Unit = Key->y ;
						 if( KeySub.Min  > Key->z ) KeySub.Min  = Key->z ;
						 if( KeySub.Unit < Key->z ) KeySub.Unit = Key->z ;
					}

					// 16bit�⏕���̍쐬
					KeySetR->Key16BSub.Min  = MV1AnimKey16BitMinFtoB( KeySub.Min  ) ;
					KeySub.Min              = MV1AnimKey16BitMinBtoF( KeySetR->Key16BSub.Min ) ;
					KeySub.Unit            -= KeySub.Min ;
					if( KeySub.Unit < 0.00000001f )
						KeySub.Unit = 1.0f ;
					KeySub.Unit            /= 60000.0f ;
					KeySetR->Key16BSub.Unit = MV1AnimKey16BitUnitFtoB( KeySub.Unit ) ;
					KeySub.Unit             = MV1AnimKey16BitUnitBtoF( KeySetR->Key16BSub.Unit ) ;

					// 16bit�P�ӂ�̒l�� 0.1f �𒴂��Ă��܂����� 16bit�����Ȃ�
					if( KeySub.Unit >= 0.1f ) continue ;

					// �l��ϊ�
					Key        = KeySetR->KeyVector ;
					NewKey     = KeySetR->KeyVectorB16 ;
					for( k = 0 ; k < KeySetR->Num ; k ++, Key ++, NewKey ++ )
					{
						*( ( WORD * )&NewKey->x ) = ( WORD )( _FTOL( ( Key->x - KeySub.Min ) / KeySub.Unit ) ) ;
						*( ( WORD * )&NewKey->y ) = ( WORD )( _FTOL( ( Key->y - KeySub.Min ) / KeySub.Unit ) ) ;
						*( ( WORD * )&NewKey->z ) = ( WORD )( _FTOL( ( Key->z - KeySub.Min ) / KeySub.Unit ) ) ;
					}

					// �L�[�T�C�Y�����炵�đ��₷
					ReadModel->AnimKeyDataSize -= sizeof( VECTOR )     * KeySetR->Num ;
					ReadModel->AnimKeyDataSize += sizeof( VECTOR_B16 ) * KeySetR->Num ;
				}
				break ;

			case MV1_ANIMKEY_TYPE_QUATERNION :
				{
					FLOAT4_B16 *NewKey ;
					FLOAT4 *Key ;

					// �ŏ��l�ƍő�l���擾
					KeySub.Min  =  1000000000000.0f ;
					KeySub.Unit = -1000000000000.0f ;
					Key = KeySetR->KeyFloat4 ;
					for( k = 0 ; k < KeySetR->Num ; k ++, Key ++ )
					{
						if( KeySub.Min  > Key->x ) KeySub.Min  = Key->x ;
						if( KeySub.Unit < Key->x ) KeySub.Unit = Key->x ;
						if( KeySub.Min  > Key->y ) KeySub.Min  = Key->y ;
						if( KeySub.Unit < Key->y ) KeySub.Unit = Key->y ;
						if( KeySub.Min  > Key->z ) KeySub.Min  = Key->z ;
						if( KeySub.Unit < Key->z ) KeySub.Unit = Key->z ;
						if( KeySub.Min  > Key->w ) KeySub.Min  = Key->w ;
						if( KeySub.Unit < Key->w ) KeySub.Unit = Key->w ;
					}

					// 16bit�⏕���̍쐬
					KeySetR->Key16BSub.Min  = MV1AnimKey16BitMinFtoB( KeySub.Min  ) ;
					KeySub.Min              = MV1AnimKey16BitMinBtoF( KeySetR->Key16BSub.Min ) ;
					KeySub.Unit            -= KeySub.Min ;
					if( KeySub.Unit < 0.00000001f )
						KeySub.Unit = 1.0f ;
					KeySub.Unit            /= 60000.0f ;
					KeySetR->Key16BSub.Unit = MV1AnimKey16BitUnitFtoB( KeySub.Unit ) ;
					KeySub.Unit             = MV1AnimKey16BitUnitBtoF( KeySetR->Key16BSub.Unit ) ;

					// 16bit�P�ӂ�̒l�� 0.1f �𒴂��Ă��܂����� 16bit�����Ȃ�
					if( KeySub.Unit >= 0.1f ) continue ;

					// �l��ϊ�
					Key        = KeySetR->KeyFloat4 ;
					NewKey     = KeySetR->KeyFloat4B16 ;
					for( k = 0 ; k < KeySetR->Num ; k ++, Key ++, NewKey ++ )
					{
						*( ( WORD * )&NewKey->x ) = ( WORD )( _FTOL( ( Key->x - KeySub.Min ) / KeySub.Unit ) ) ;
						*( ( WORD * )&NewKey->y ) = ( WORD )( _FTOL( ( Key->y - KeySub.Min ) / KeySub.Unit ) ) ;
						*( ( WORD * )&NewKey->z ) = ( WORD )( _FTOL( ( Key->z - KeySub.Min ) / KeySub.Unit ) ) ;
						*( ( WORD * )&NewKey->w ) = ( WORD )( _FTOL( ( Key->w - KeySub.Min ) / KeySub.Unit ) ) ;
					}

					// �L�[�T�C�Y�����炵�đ��₷
					ReadModel->AnimKeyDataSize -= sizeof( FLOAT4 )     * KeySetR->Num ;
					ReadModel->AnimKeyDataSize += sizeof( FLOAT4_B16 ) * KeySetR->Num ;
				}
				break ;

			case MV1_ANIMKEY_TYPE_MATRIX3X3 :
				{
					MV1_ANIM_KEY_MATRIX3X3_B16 *NewKey ;
					MV1_ANIM_KEY_MATRIX3X3 *Key ;

					// �ŏ��l�ƍő�l���擾
					KeySub.Min  =  1000000000000.0f ;
					KeySub.Unit = -1000000000000.0f ;
					Key  = KeySetR->KeyMatrix3x3 ;
					for( k = 0 ; k < KeySetR->Num ; k ++, Key ++ )
					{
						if( KeySub.Min  > Key->Matrix[ 0 ][ 0 ] ) KeySub.Min  = Key->Matrix[ 0 ][ 0 ] ;
						if( KeySub.Unit < Key->Matrix[ 0 ][ 0 ] ) KeySub.Unit = Key->Matrix[ 0 ][ 0 ] ;
						if( KeySub.Min  > Key->Matrix[ 0 ][ 1 ] ) KeySub.Min  = Key->Matrix[ 0 ][ 1 ] ;
						if( KeySub.Unit < Key->Matrix[ 0 ][ 1 ] ) KeySub.Unit = Key->Matrix[ 0 ][ 1 ] ;
						if( KeySub.Min  > Key->Matrix[ 0 ][ 2 ] ) KeySub.Min  = Key->Matrix[ 0 ][ 2 ] ;
						if( KeySub.Unit < Key->Matrix[ 0 ][ 2 ] ) KeySub.Unit = Key->Matrix[ 0 ][ 2 ] ;
						if( KeySub.Min  > Key->Matrix[ 1 ][ 0 ] ) KeySub.Min  = Key->Matrix[ 1 ][ 0 ] ;
						if( KeySub.Unit < Key->Matrix[ 1 ][ 0 ] ) KeySub.Unit = Key->Matrix[ 1 ][ 0 ] ;
						if( KeySub.Min  > Key->Matrix[ 1 ][ 1 ] ) KeySub.Min  = Key->Matrix[ 1 ][ 1 ] ;
						if( KeySub.Unit < Key->Matrix[ 1 ][ 1 ] ) KeySub.Unit = Key->Matrix[ 1 ][ 1 ] ;
						if( KeySub.Min  > Key->Matrix[ 1 ][ 2 ] ) KeySub.Min  = Key->Matrix[ 1 ][ 2 ] ;
						if( KeySub.Unit < Key->Matrix[ 1 ][ 2 ] ) KeySub.Unit = Key->Matrix[ 1 ][ 2 ] ;
						if( KeySub.Min  > Key->Matrix[ 2 ][ 0 ] ) KeySub.Min  = Key->Matrix[ 2 ][ 0 ] ;
						if( KeySub.Unit < Key->Matrix[ 2 ][ 0 ] ) KeySub.Unit = Key->Matrix[ 2 ][ 0 ] ;
						if( KeySub.Min  > Key->Matrix[ 2 ][ 1 ] ) KeySub.Min  = Key->Matrix[ 2 ][ 1 ] ;
						if( KeySub.Unit < Key->Matrix[ 2 ][ 1 ] ) KeySub.Unit = Key->Matrix[ 2 ][ 1 ] ;
						if( KeySub.Min  > Key->Matrix[ 2 ][ 2 ] ) KeySub.Min  = Key->Matrix[ 2 ][ 2 ] ;
						if( KeySub.Unit < Key->Matrix[ 2 ][ 2 ] ) KeySub.Unit = Key->Matrix[ 2 ][ 2 ] ;
					}

					// 16bit�⏕���̍쐬
					KeySetR->Key16BSub.Min  = MV1AnimKey16BitMinFtoB( KeySub.Min  ) ;
					KeySub.Min              = MV1AnimKey16BitMinBtoF( KeySetR->Key16BSub.Min ) ;
					KeySub.Unit            -= KeySub.Min ;
					if( KeySub.Unit < 0.00000001f )
						KeySub.Unit = 1.0f ;
					KeySub.Unit            /= 60000.0f ;
					KeySetR->Key16BSub.Unit = MV1AnimKey16BitUnitFtoB( KeySub.Unit ) ;
					KeySub.Unit             = MV1AnimKey16BitUnitBtoF( KeySetR->Key16BSub.Unit ) ;

					// 16bit�P�ӂ�̒l�� 0.1f �𒴂��Ă��܂����� 16bit�����Ȃ�
					if( KeySub.Unit >= 0.1f ) continue ;

					// �l��ϊ�
					Key    = KeySetR->KeyMatrix3x3 ;
					NewKey = KeySetR->KeyMatrix3x3B16 ;
					for( k = 0 ; k < KeySetR->Num ; k ++, Key ++, NewKey ++ )
					{
						NewKey->Matrix[ 0 ][ 0 ] = ( WORD )( _FTOL( ( Key->Matrix[ 0 ][ 0 ] - KeySub.Min ) / KeySub.Unit ) ) ;
						NewKey->Matrix[ 0 ][ 1 ] = ( WORD )( _FTOL( ( Key->Matrix[ 0 ][ 1 ] - KeySub.Min ) / KeySub.Unit ) ) ;
						NewKey->Matrix[ 0 ][ 2 ] = ( WORD )( _FTOL( ( Key->Matrix[ 0 ][ 2 ] - KeySub.Min ) / KeySub.Unit ) ) ;
						NewKey->Matrix[ 1 ][ 0 ] = ( WORD )( _FTOL( ( Key->Matrix[ 1 ][ 0 ] - KeySub.Min ) / KeySub.Unit ) ) ;
						NewKey->Matrix[ 1 ][ 1 ] = ( WORD )( _FTOL( ( Key->Matrix[ 1 ][ 1 ] - KeySub.Min ) / KeySub.Unit ) ) ;
						NewKey->Matrix[ 1 ][ 2 ] = ( WORD )( _FTOL( ( Key->Matrix[ 1 ][ 2 ] - KeySub.Min ) / KeySub.Unit ) ) ;
						NewKey->Matrix[ 2 ][ 0 ] = ( WORD )( _FTOL( ( Key->Matrix[ 2 ][ 0 ] - KeySub.Min ) / KeySub.Unit ) ) ;
						NewKey->Matrix[ 2 ][ 1 ] = ( WORD )( _FTOL( ( Key->Matrix[ 2 ][ 1 ] - KeySub.Min ) / KeySub.Unit ) ) ;
						NewKey->Matrix[ 2 ][ 2 ] = ( WORD )( _FTOL( ( Key->Matrix[ 2 ][ 2 ] - KeySub.Min ) / KeySub.Unit ) ) ;
					}

					// �L�[�T�C�Y�����炵�đ��₷
					ReadModel->AnimKeyDataSize -= sizeof( MV1_ANIM_KEY_MATRIX3X3     ) * KeySetR->Num ;
					ReadModel->AnimKeyDataSize += sizeof( MV1_ANIM_KEY_MATRIX3X3_B16 ) * KeySetR->Num ;
				}
				break ;
			}

			// �^�C�����L�[�^�C�v�̏ꍇ�̓L�[�T�C�Y�����炵�đ��₷
			if( KeySetR->TimeType == MV1_ANIMKEY_TIME_TYPE_KEY )
			{
				ReadModel->AnimKeyDataSize -= sizeof( float ) * KeySetR->Num ;
				ReadModel->AnimKeyDataSize += sizeof( WORD  ) * KeySetR->Num ;
			}

			// 16�r�b�g�t���O�𗧂Ă�
			KeySetR->Bit16 = 1 ;
		}
	}

	// �I��
	return true ;
	*/
}

// �L�[�Z�b�g�̃L�[���œK������
static void MV1OptimizeKeySetTimeType( MV1_MODEL_R *ReadModel, MV1_ANIMKEYSET_R *KeySetR, bool NotKeySub )
{
	float ktime = 0.0f, ntime, f ;
	void *AfterBuffer ;
	float *Time, *NewTime ;
	bool Ittei ;
	int k, num = 0, UnitSize = 0, AfterSizeK, AfterSizeU ;
	float rate ;

	// �e�L�[�����Ԋu���ǂ����𒲂ׂ�
	Ittei = false ;
	if( KeySetR->TimeType == MV1_ANIMKEY_TIME_TYPE_KEY )
	{
		if( KeySetR->Num > 2 )
		{
			Time = KeySetR->KeyTime ;
			ktime = Time[ 1 ] - Time[ 0 ] ;
			Time ++ ;
			for( k = 1 ; k < KeySetR->Num - 1 ; k ++, Time ++ )
			{
				ntime = ( Time[ 1 ] - Time[ 0 ] ) - ktime ;
				if( ntime > 0.000001f || ntime < -0.000001f ) break ;
			}
			if( k == KeySetR->Num - 1 )
			{
				Ittei = true ;
			}
		}
	}
	else
	{
		Ittei = true ;
	}

	AfterBuffer = NULL ;
	switch( KeySetR->Type )
	{
	case MV1_ANIMKEY_TYPE_LINEAR :
		{
			float *Key, *NewBuffer, *NewKey ;

			UnitSize = sizeof( float ) ;
			AfterBuffer = NewBuffer = ( float * )ADDMEMAREA( ( UnitSize + sizeof( float ) ) * KeySetR->Num, &ReadModel->Mem ) ;
			if( NewBuffer )
			{
				// ���`��ԂōςރL�[���폜����
				num = 0 ;
				NewKey  = NewBuffer ;
				NewTime = ( float * )( NewBuffer + KeySetR->Num ) ;
				Key     = KeySetR->KeyLinear ;
				Time    = KeySetR->KeyTime ;
				if( KeySetR->TimeType == MV1_ANIMKEY_TIME_TYPE_ONE )
				{
					rate = 0.5f ;
					for( k = 0 ; k < KeySetR->Num ; k ++, Key ++ )
					{
						if( k != 0 && k != KeySetR->Num - 1 )
						{
							f = ( rate * ( Key[ 1 ] - NewKey[ -1 ] ) + NewKey[ -1 ] ) - Key[ 0 ] ; if( f > 0.00001f || f < -0.00001f ) goto FLOATADDONE ;

							continue ;
						}

						if( k == KeySetR->Num - 1 && num == 1 )
						{
							if( *( ( DWORD * )&Key[ 0 ] ) != *( ( DWORD * )&NewKey[ -1 ] ) ) goto FLOATADDONE ;
							continue ;
						}
FLOATADDONE :
						*NewKey  = *Key ;
						*NewTime = KeySetR->UnitTime * k + KeySetR->StartTime ;
						NewKey  ++ ;
						NewTime ++ ;
						num ++ ;
					}
				}
				else
				{
					for( k = 0 ; k < KeySetR->Num ; k ++, Key ++, Time ++ )
					{
						if( k != 0 && k != KeySetR->Num - 1 )
						{
							rate = ( Time[ 0 ] - NewTime[ -1 ] ) / ( Time[ 1 ] - NewTime[ -1 ] ) ;
							f = ( rate * ( Key[ 1 ] - NewKey[ -1 ] ) + NewKey[ -1 ] ) - Key[ 0 ] ; if( f > 0.00001f || f < -0.00001f ) goto FLOATADD ;

							continue ;
						}

						if( k == KeySetR->Num - 1 && num == 1 )
						{
							if( *( ( DWORD * )&Key[ 0 ] ) != *( ( DWORD * )&NewKey[ -1 ] ) ) goto FLOATADD ;
							continue ;
						}
FLOATADD :
						*NewKey  = *Key ;
						*NewTime = *Time ;
						NewKey  ++ ;
						NewTime ++ ;
						num ++ ;
					}
				}
			}
		}
		break ;

	case MV1_ANIMKEY_TYPE_VECTOR :
		{
			VECTOR *Key, *NewBuffer, *NewKey ;

			UnitSize = sizeof( VECTOR ) ;
			AfterBuffer = NewBuffer = ( VECTOR * )ADDMEMAREA( ( UnitSize + sizeof( float ) ) * KeySetR->Num, &ReadModel->Mem ) ;
			if( NewBuffer )
			{
				// ���`��ԂōςރL�[���폜����
				num = 0 ;
				NewKey  = NewBuffer ;
				NewTime = ( float * )( NewBuffer + KeySetR->Num ) ;
				Key     = KeySetR->KeyVector ;
				Time    = KeySetR->KeyTime ;
				if( KeySetR->TimeType == MV1_ANIMKEY_TIME_TYPE_ONE )
				{
					rate = 0.5f ;
					for( k = 0 ; k < KeySetR->Num ; k ++, Key ++ )
					{
						if( k != 0 && k != KeySetR->Num - 1 )
						{
							f = ( rate * ( Key[ 1 ].x - NewKey[ -1 ].x ) + NewKey[ -1 ].x ) - Key[ 0 ].x ; if( f > 0.00001f || f < -0.00001f ) goto TRANSLATEADDONE ;
							f = ( rate * ( Key[ 1 ].y - NewKey[ -1 ].y ) + NewKey[ -1 ].y ) - Key[ 0 ].y ; if( f > 0.00001f || f < -0.00001f ) goto TRANSLATEADDONE ;
							f = ( rate * ( Key[ 1 ].z - NewKey[ -1 ].z ) + NewKey[ -1 ].z ) - Key[ 0 ].z ; if( f > 0.00001f || f < -0.00001f ) goto TRANSLATEADDONE ;

							continue ;
						}

						if( k == KeySetR->Num - 1 && num == 1 )
						{
							if( *( ( DWORD * )&Key[ 0 ].x ) != *( ( DWORD * )&NewKey[ -1 ].x ) ||
								*( ( DWORD * )&Key[ 0 ].y ) != *( ( DWORD * )&NewKey[ -1 ].y ) ||
								*( ( DWORD * )&Key[ 0 ].z ) != *( ( DWORD * )&NewKey[ -1 ].z ) ) goto TRANSLATEADDONE ;
							continue ;
						}
TRANSLATEADDONE :
						*NewKey  = *Key ;
						*NewTime = KeySetR->UnitTime * k + KeySetR->StartTime ;
						NewKey  ++ ;
						NewTime ++ ;
						num ++ ;
					}
				}
				else
				{
					for( k = 0 ; k < KeySetR->Num ; k ++, Key ++, Time ++ )
					{
						if( k != 0 && k != KeySetR->Num - 1 )
						{
							rate = ( Time[ 0 ] - NewTime[ -1 ] ) / ( Time[ 1 ] - NewTime[ -1 ] ) ;
							f = ( rate * ( Key[ 1 ].x - NewKey[ -1 ].x ) + NewKey[ -1 ].x ) - Key[ 0 ].x ; if( f > 0.00001f || f < -0.00001f ) goto TRANSLATEADD ;
							f = ( rate * ( Key[ 1 ].y - NewKey[ -1 ].y ) + NewKey[ -1 ].y ) - Key[ 0 ].y ; if( f > 0.00001f || f < -0.00001f ) goto TRANSLATEADD ;
							f = ( rate * ( Key[ 1 ].z - NewKey[ -1 ].z ) + NewKey[ -1 ].z ) - Key[ 0 ].z ; if( f > 0.00001f || f < -0.00001f ) goto TRANSLATEADD ;

							continue ;
						}

						if( k == KeySetR->Num - 1 && num == 1 )
						{
							if( *( ( DWORD * )&Key[ 0 ].x ) != *( ( DWORD * )&NewKey[ -1 ].x ) ||
								*( ( DWORD * )&Key[ 0 ].y ) != *( ( DWORD * )&NewKey[ -1 ].y ) ||
								*( ( DWORD * )&Key[ 0 ].z ) != *( ( DWORD * )&NewKey[ -1 ].z ) ) goto TRANSLATEADD ;
							continue ;
						}
TRANSLATEADD :
						*NewKey  = *Key ;
						*NewTime = *Time ;
						NewKey  ++ ;
						NewTime ++ ;
						num ++ ;
					}
				}
			}
		}
		break ;

	case MV1_ANIMKEY_TYPE_QUATERNION_VMD :
	case MV1_ANIMKEY_TYPE_QUATERNION_X :
		{
			FLOAT4 *Key, *NewBuffer, *NewKey ;

			UnitSize = sizeof( FLOAT4 ) ;
			AfterBuffer = NewBuffer = ( FLOAT4 * )ADDMEMAREA( ( UnitSize + sizeof( float ) ) * KeySetR->Num, &ReadModel->Mem ) ;
			if( NewBuffer )
			{
				// ���`��ԂōςރL�[���폜����
				num = 0 ;
				NewKey  = NewBuffer ;
				NewTime = ( float * )( NewBuffer + KeySetR->Num ) ;
				Key     = KeySetR->KeyFloat4 ;
				Time    = KeySetR->KeyTime ;
				if( KeySetR->TimeType == MV1_ANIMKEY_TIME_TYPE_ONE )
				{
					rate = 0.5f ;
					for( k = 0 ; k < KeySetR->Num ; k ++, Key ++ )
					{
						if( k != 0 && k != KeySetR->Num - 1 )
						{
							f = ( rate * ( Key[ 1 ].x - NewKey[ -1 ].x ) + NewKey[ -1 ].x ) - Key[ 0 ].x ; if( f > 0.00001f || f < -0.00001f ) goto QUATERNIONADDONE ;
							f = ( rate * ( Key[ 1 ].y - NewKey[ -1 ].y ) + NewKey[ -1 ].y ) - Key[ 0 ].y ; if( f > 0.00001f || f < -0.00001f ) goto QUATERNIONADDONE ;
							f = ( rate * ( Key[ 1 ].z - NewKey[ -1 ].z ) + NewKey[ -1 ].z ) - Key[ 0 ].z ; if( f > 0.00001f || f < -0.00001f ) goto QUATERNIONADDONE ;
							f = ( rate * ( Key[ 1 ].w - NewKey[ -1 ].w ) + NewKey[ -1 ].w ) - Key[ 0 ].w ; if( f > 0.00001f || f < -0.00001f ) goto QUATERNIONADDONE ;

							continue ;
						}

						if( k == KeySetR->Num - 1 && num == 1 )
						{
							if( *( ( DWORD * )&Key[ 0 ].x ) != *( ( DWORD * )&NewKey[ -1 ].x ) ||
								*( ( DWORD * )&Key[ 0 ].y ) != *( ( DWORD * )&NewKey[ -1 ].y ) ||
								*( ( DWORD * )&Key[ 0 ].z ) != *( ( DWORD * )&NewKey[ -1 ].z ) ||
								*( ( DWORD * )&Key[ 0 ].w ) != *( ( DWORD * )&NewKey[ -1 ].w ) ) goto QUATERNIONADDONE ;
							continue ;
						}
QUATERNIONADDONE :
						*NewKey  = *Key ;
						*NewTime = KeySetR->UnitTime * k + KeySetR->StartTime ;
						NewKey  ++ ;
						NewTime ++ ;
						num ++ ;
					}
				}
				else
				{
					for( k = 0 ; k < KeySetR->Num ; k ++, Key ++, Time ++ )
					{
						if( k != 0 && k != KeySetR->Num - 1 )
						{
							rate = ( Time[ 0 ] - NewTime[ -1 ] ) / ( Time[ 1 ] - NewTime[ -1 ] ) ;
							f = ( rate * ( Key[ 1 ].x - NewKey[ -1 ].x ) + NewKey[ -1 ].x ) - Key[ 0 ].x ; if( f > 0.00001f || f < -0.00001f ) goto QUATERNIONADD ;
							f = ( rate * ( Key[ 1 ].y - NewKey[ -1 ].y ) + NewKey[ -1 ].y ) - Key[ 0 ].y ; if( f > 0.00001f || f < -0.00001f ) goto QUATERNIONADD ;
							f = ( rate * ( Key[ 1 ].z - NewKey[ -1 ].z ) + NewKey[ -1 ].z ) - Key[ 0 ].z ; if( f > 0.00001f || f < -0.00001f ) goto QUATERNIONADD ;
							f = ( rate * ( Key[ 1 ].w - NewKey[ -1 ].w ) + NewKey[ -1 ].w ) - Key[ 0 ].w ; if( f > 0.00001f || f < -0.00001f ) goto QUATERNIONADD ;

							continue ;
						}

						if( k == KeySetR->Num - 1 && num == 1 )
						{
							if( *( ( DWORD * )&Key[ 0 ].x ) != *( ( DWORD * )&NewKey[ -1 ].x ) ||
								*( ( DWORD * )&Key[ 0 ].y ) != *( ( DWORD * )&NewKey[ -1 ].y ) ||
								*( ( DWORD * )&Key[ 0 ].z ) != *( ( DWORD * )&NewKey[ -1 ].z ) ||
								*( ( DWORD * )&Key[ 0 ].w ) != *( ( DWORD * )&NewKey[ -1 ].w ) ) goto QUATERNIONADD ;
							continue ;
						}
QUATERNIONADD :
						*NewKey  = *Key ;
						*NewTime = *Time ;
						NewKey  ++ ;
						NewTime ++ ;
						num ++ ;
					}
				}
			}
		}
		break ;

	case MV1_ANIMKEY_TYPE_MATRIX3X3 :
		{
			MV1_ANIM_KEY_MATRIX3X3 *Key, *NewBuffer, *NewKey ;

			UnitSize = sizeof( MV1_ANIM_KEY_MATRIX3X3 ) ;
			AfterBuffer = NewBuffer = ( MV1_ANIM_KEY_MATRIX3X3 * )ADDMEMAREA( ( UnitSize + sizeof( float ) ) * KeySetR->Num, &ReadModel->Mem ) ;
			if( NewBuffer )
			{
				// ���`��ԂōςރL�[���폜����
				num = 0 ;
				Key     = KeySetR->KeyMatrix3x3 ;
				Time    = KeySetR->KeyTime ;
				NewKey  = NewBuffer ;
				NewTime = ( float * )( NewBuffer + KeySetR->Num ) ;
				if( KeySetR->TimeType == MV1_ANIMKEY_TIME_TYPE_ONE )
				{
					rate = 0.5f ;
					for( k = 0 ; k < KeySetR->Num ; k ++, Key ++ )
					{
						if( k != 0 && k != KeySetR->Num - 1 )
						{
							f = ( rate * ( Key[ 1 ].Matrix[ 0 ][ 0 ] - NewKey[ -1 ].Matrix[ 0 ][ 0 ] ) + NewKey[ -1 ].Matrix[ 0 ][ 0 ] ) - Key[ 0 ].Matrix[ 0 ][ 0 ] ; if( f > 0.00001f || f < -0.00001f ) goto MATRIX3X3ADDONE ;
							f = ( rate * ( Key[ 1 ].Matrix[ 0 ][ 1 ] - NewKey[ -1 ].Matrix[ 0 ][ 1 ] ) + NewKey[ -1 ].Matrix[ 0 ][ 1 ] ) - Key[ 0 ].Matrix[ 0 ][ 1 ] ; if( f > 0.00001f || f < -0.00001f ) goto MATRIX3X3ADDONE ;
							f = ( rate * ( Key[ 1 ].Matrix[ 0 ][ 2 ] - NewKey[ -1 ].Matrix[ 0 ][ 2 ] ) + NewKey[ -1 ].Matrix[ 0 ][ 2 ] ) - Key[ 0 ].Matrix[ 0 ][ 2 ] ; if( f > 0.00001f || f < -0.00001f ) goto MATRIX3X3ADDONE ;
							f = ( rate * ( Key[ 1 ].Matrix[ 1 ][ 0 ] - NewKey[ -1 ].Matrix[ 1 ][ 0 ] ) + NewKey[ -1 ].Matrix[ 1 ][ 0 ] ) - Key[ 0 ].Matrix[ 1 ][ 0 ] ; if( f > 0.00001f || f < -0.00001f ) goto MATRIX3X3ADDONE ;
							f = ( rate * ( Key[ 1 ].Matrix[ 1 ][ 1 ] - NewKey[ -1 ].Matrix[ 1 ][ 1 ] ) + NewKey[ -1 ].Matrix[ 1 ][ 1 ] ) - Key[ 0 ].Matrix[ 1 ][ 1 ] ; if( f > 0.00001f || f < -0.00001f ) goto MATRIX3X3ADDONE ;
							f = ( rate * ( Key[ 1 ].Matrix[ 1 ][ 2 ] - NewKey[ -1 ].Matrix[ 1 ][ 2 ] ) + NewKey[ -1 ].Matrix[ 1 ][ 2 ] ) - Key[ 0 ].Matrix[ 1 ][ 2 ] ; if( f > 0.00001f || f < -0.00001f ) goto MATRIX3X3ADDONE ;
							f = ( rate * ( Key[ 1 ].Matrix[ 2 ][ 0 ] - NewKey[ -1 ].Matrix[ 2 ][ 0 ] ) + NewKey[ -1 ].Matrix[ 2 ][ 0 ] ) - Key[ 0 ].Matrix[ 2 ][ 0 ] ; if( f > 0.00001f || f < -0.00001f ) goto MATRIX3X3ADDONE ;
							f = ( rate * ( Key[ 1 ].Matrix[ 2 ][ 1 ] - NewKey[ -1 ].Matrix[ 2 ][ 1 ] ) + NewKey[ -1 ].Matrix[ 2 ][ 1 ] ) - Key[ 0 ].Matrix[ 2 ][ 1 ] ; if( f > 0.00001f || f < -0.00001f ) goto MATRIX3X3ADDONE ;
							f = ( rate * ( Key[ 1 ].Matrix[ 2 ][ 2 ] - NewKey[ -1 ].Matrix[ 2 ][ 2 ] ) + NewKey[ -1 ].Matrix[ 2 ][ 2 ] ) - Key[ 0 ].Matrix[ 2 ][ 2 ] ; if( f > 0.00001f || f < -0.00001f ) goto MATRIX3X3ADDONE ;

							continue ;
						}

						if( k == KeySetR->Num - 1 && num == 1 )
						{
							if( *( ( DWORD * )&Key[ 0 ].Matrix[ 0 ][ 0 ] ) != *( ( DWORD * )&NewKey[ -1 ].Matrix[ 0 ][ 0 ] ) ||
								*( ( DWORD * )&Key[ 0 ].Matrix[ 0 ][ 1 ] ) != *( ( DWORD * )&NewKey[ -1 ].Matrix[ 0 ][ 1 ] ) ||
								*( ( DWORD * )&Key[ 0 ].Matrix[ 0 ][ 2 ] ) != *( ( DWORD * )&NewKey[ -1 ].Matrix[ 0 ][ 2 ] ) ||
								*( ( DWORD * )&Key[ 0 ].Matrix[ 1 ][ 0 ] ) != *( ( DWORD * )&NewKey[ -1 ].Matrix[ 1 ][ 0 ] ) ||
								*( ( DWORD * )&Key[ 0 ].Matrix[ 1 ][ 1 ] ) != *( ( DWORD * )&NewKey[ -1 ].Matrix[ 1 ][ 1 ] ) ||
								*( ( DWORD * )&Key[ 0 ].Matrix[ 1 ][ 2 ] ) != *( ( DWORD * )&NewKey[ -1 ].Matrix[ 1 ][ 2 ] ) ||
								*( ( DWORD * )&Key[ 0 ].Matrix[ 2 ][ 0 ] ) != *( ( DWORD * )&NewKey[ -1 ].Matrix[ 2 ][ 0 ] ) ||
								*( ( DWORD * )&Key[ 0 ].Matrix[ 2 ][ 1 ] ) != *( ( DWORD * )&NewKey[ -1 ].Matrix[ 2 ][ 1 ] ) ||
								*( ( DWORD * )&Key[ 0 ].Matrix[ 2 ][ 2 ] ) != *( ( DWORD * )&NewKey[ -1 ].Matrix[ 2 ][ 2 ] ) ) goto MATRIX3X3ADDONE ;
							continue ;
						}
MATRIX3X3ADDONE :
						*NewKey  = *Key ;
						*NewTime = KeySetR->UnitTime * k + KeySetR->StartTime ;
						NewKey  ++ ;
						NewTime ++ ;
						num ++ ;
					}
				}
				else
				{
					for( k = 0 ; k < KeySetR->Num ; k ++, Key ++, Time ++ )
					{
						if( k != 0 && k != KeySetR->Num - 1 )
						{
							rate = ( Time[ 0 ] - NewTime[ -1 ] ) / ( Time[ 1 ] - NewTime[ -1 ] ) ;
							f = ( rate * ( Key[ 1 ].Matrix[ 0 ][ 0 ] - NewKey[ -1 ].Matrix[ 0 ][ 0 ] ) + NewKey[ -1 ].Matrix[ 0 ][ 0 ] ) - Key[ 0 ].Matrix[ 0 ][ 0 ] ; if( f > 0.00001f || f < -0.00001f ) goto MATRIX3X3ADD ;
							f = ( rate * ( Key[ 1 ].Matrix[ 0 ][ 1 ] - NewKey[ -1 ].Matrix[ 0 ][ 1 ] ) + NewKey[ -1 ].Matrix[ 0 ][ 1 ] ) - Key[ 0 ].Matrix[ 0 ][ 1 ] ; if( f > 0.00001f || f < -0.00001f ) goto MATRIX3X3ADD ;
							f = ( rate * ( Key[ 1 ].Matrix[ 0 ][ 2 ] - NewKey[ -1 ].Matrix[ 0 ][ 2 ] ) + NewKey[ -1 ].Matrix[ 0 ][ 2 ] ) - Key[ 0 ].Matrix[ 0 ][ 2 ] ; if( f > 0.00001f || f < -0.00001f ) goto MATRIX3X3ADD ;
							f = ( rate * ( Key[ 1 ].Matrix[ 1 ][ 0 ] - NewKey[ -1 ].Matrix[ 1 ][ 0 ] ) + NewKey[ -1 ].Matrix[ 1 ][ 0 ] ) - Key[ 0 ].Matrix[ 1 ][ 0 ] ; if( f > 0.00001f || f < -0.00001f ) goto MATRIX3X3ADD ;
							f = ( rate * ( Key[ 1 ].Matrix[ 1 ][ 1 ] - NewKey[ -1 ].Matrix[ 1 ][ 1 ] ) + NewKey[ -1 ].Matrix[ 1 ][ 1 ] ) - Key[ 0 ].Matrix[ 1 ][ 1 ] ; if( f > 0.00001f || f < -0.00001f ) goto MATRIX3X3ADD ;
							f = ( rate * ( Key[ 1 ].Matrix[ 1 ][ 2 ] - NewKey[ -1 ].Matrix[ 1 ][ 2 ] ) + NewKey[ -1 ].Matrix[ 1 ][ 2 ] ) - Key[ 0 ].Matrix[ 1 ][ 2 ] ; if( f > 0.00001f || f < -0.00001f ) goto MATRIX3X3ADD ;
							f = ( rate * ( Key[ 1 ].Matrix[ 2 ][ 0 ] - NewKey[ -1 ].Matrix[ 2 ][ 0 ] ) + NewKey[ -1 ].Matrix[ 2 ][ 0 ] ) - Key[ 0 ].Matrix[ 2 ][ 0 ] ; if( f > 0.00001f || f < -0.00001f ) goto MATRIX3X3ADD ;
							f = ( rate * ( Key[ 1 ].Matrix[ 2 ][ 1 ] - NewKey[ -1 ].Matrix[ 2 ][ 1 ] ) + NewKey[ -1 ].Matrix[ 2 ][ 1 ] ) - Key[ 0 ].Matrix[ 2 ][ 1 ] ; if( f > 0.00001f || f < -0.00001f ) goto MATRIX3X3ADD ;
							f = ( rate * ( Key[ 1 ].Matrix[ 2 ][ 2 ] - NewKey[ -1 ].Matrix[ 2 ][ 2 ] ) + NewKey[ -1 ].Matrix[ 2 ][ 2 ] ) - Key[ 0 ].Matrix[ 2 ][ 2 ] ; if( f > 0.00001f || f < -0.00001f ) goto MATRIX3X3ADD ;

							continue ;
						}

						if( k == KeySetR->Num - 1 && num == 1 )
						{
							if( *( ( DWORD * )&Key[ 0 ].Matrix[ 0 ][ 0 ] ) != *( ( DWORD * )&NewKey[ -1 ].Matrix[ 0 ][ 0 ] ) ||
								*( ( DWORD * )&Key[ 0 ].Matrix[ 0 ][ 1 ] ) != *( ( DWORD * )&NewKey[ -1 ].Matrix[ 0 ][ 1 ] ) ||
								*( ( DWORD * )&Key[ 0 ].Matrix[ 0 ][ 2 ] ) != *( ( DWORD * )&NewKey[ -1 ].Matrix[ 0 ][ 2 ] ) ||
								*( ( DWORD * )&Key[ 0 ].Matrix[ 1 ][ 0 ] ) != *( ( DWORD * )&NewKey[ -1 ].Matrix[ 1 ][ 0 ] ) ||
								*( ( DWORD * )&Key[ 0 ].Matrix[ 1 ][ 1 ] ) != *( ( DWORD * )&NewKey[ -1 ].Matrix[ 1 ][ 1 ] ) ||
								*( ( DWORD * )&Key[ 0 ].Matrix[ 1 ][ 2 ] ) != *( ( DWORD * )&NewKey[ -1 ].Matrix[ 1 ][ 2 ] ) ||
								*( ( DWORD * )&Key[ 0 ].Matrix[ 2 ][ 0 ] ) != *( ( DWORD * )&NewKey[ -1 ].Matrix[ 2 ][ 0 ] ) ||
								*( ( DWORD * )&Key[ 0 ].Matrix[ 2 ][ 1 ] ) != *( ( DWORD * )&NewKey[ -1 ].Matrix[ 2 ][ 1 ] ) ||
								*( ( DWORD * )&Key[ 0 ].Matrix[ 2 ][ 2 ] ) != *( ( DWORD * )&NewKey[ -1 ].Matrix[ 2 ][ 2 ] ) ) goto MATRIX3X3ADD ;
							continue ;
						}
MATRIX3X3ADD :
						*NewTime = *Time ;
						*NewKey  = *Key ;
						NewTime ++ ;
						NewKey  ++ ;
						num ++ ;
					}
				}
			}
		}
		break ;
	}

	if( AfterBuffer )
	{
		// �팸��̃L�[�̐����P���A
		// �L�[�����炵�č팸�����f�[�^�T�C�Y���Œ�^�C���ɂ���
		// �팸�ł���f�[�^�T�C�Y���傫�����A���������Œ�^�C����
		// �ł��Ȃ��ꍇ�̓L�[�����炷
		AfterSizeK = ( int )( num          * ( UnitSize + sizeof( float ) ) ) ;
		AfterSizeU = KeySetR->Num *   UnitSize ;
		if( num == 1 || ( ( Ittei == false || AfterSizeK < AfterSizeU ) && num < KeySetR->Num ) )
		{
			// �L�[����ɂȂ�ꍇ�ȊO�ŃL�[���폜����t���O�������ꍇ�͉������Ȃ�
			if( NotKeySub == false || num == 1 )
			{
				// �L�[�f�[�^�̃T�C�Y�����炵�đ��₷
				if( KeySetR->TimeType == MV1_ANIMKEY_TIME_TYPE_ONE )
				{
					ReadModel->AnimKeyDataSize -= UnitSize * KeySetR->Num ;
					ReadModel->AnimKeyDataSize += UnitSize * num ;
				}
				else
				{
					ReadModel->AnimKeyDataSize -= ( sizeof( float ) + UnitSize ) * KeySetR->Num ;
					ReadModel->AnimKeyDataSize += ( sizeof( float ) + UnitSize ) * num ;
				}
				KeySetR->KeyFloat4 = ( FLOAT4 * )AfterBuffer ;
				KeySetR->KeyTime = ( float * )( ( BYTE * )AfterBuffer + UnitSize * KeySetR->Num ) ;
				KeySetR->Num = num ;
			}

			// �L�[����̏ꍇ�͍X�ɌŒ�^�C���ɂ���
			if( num == 1 && KeySetR->TimeType == MV1_ANIMKEY_TIME_TYPE_KEY )
			{
				// �L�[�f�[�^�̃T�C�Y�����炷
				ReadModel->AnimKeyDataSize -= sizeof( float ) * num ;

				KeySetR->TimeType = MV1_ANIMKEY_TIME_TYPE_ONE ;
				KeySetR->StartTime = 0.0f ;
				KeySetR->UnitTime = KeySetR->TotalTime ;
			}
		}
		else
		// ����ȊO�̏ꍇ�ŌŒ�^�C���ɂł���ꍇ�͌Œ�^�C���ɂ���
		if( Ittei == true )
		{
			if( KeySetR->TimeType == MV1_ANIMKEY_TIME_TYPE_KEY )
			{
				// �L�[�f�[�^�̃T�C�Y�����炷
				ReadModel->AnimKeyDataSize -= sizeof( float ) * KeySetR->Num ;

				KeySetR->TimeType = MV1_ANIMKEY_TIME_TYPE_ONE ;
				KeySetR->StartTime = KeySetR->KeyTime[ 0 ] ;
				KeySetR->UnitTime = ktime ;
			}
		}
	}
}

// ���b�V���@���̎����v�Z( �S�|���S�����O�p�`�|���S���ł���K�v������܂� )
static bool MV1MakeMeshNormals( MV1_MODEL_R *ReadModel, MV1_MESH_R *Mesh )
{
	MV1_MESHFACE_R *Face, *FaceT ;
	int i, j, k, m, Index ;
	VECTOR *P0, *P1, *P2, V1, V2, V3, Norm, tv, xv, yv, zv, FaceNorm, *Normal ;
	float SmoothCos, Sin ;
	float MinFaceRadi = 0, MinFaceRadi2 = 0, FaceRadi, FaceRadi2 ;
	int FaceCount, MinFaceRadi2Use = 0, FaceRadi2Use, CurFaceIndex, Issyuu ;
	MV1_MESHFACE_R *FaceList[ 1024 ], *CurFace, *MinFace, *BackCurFace ;
	BYTE FaceIndex[ 1024 ] ;
	DWORD IndexNum, UseNormalNum ;
	int MinFaceUra = 0, FaceUra = 0, CmpFlag, MinFaceIndex ;
	MV1_MAKEVERTINDEXINFO **VertexFaceList, *VertexFaceBuffer, *VFBuf ;
	BYTE *NormalUseTable ;
	DWORD *UseNormalIndex, *NewNormalIndex ;

	// �X���[�W���O���s���p�x�̃R�T�C���l�����߂Ă���
	if( Mesh->Container->SmoothingAngle < -0.00001f )
	{
		_SINCOS_PLATFORM( ( float )( 89.5f / 180.0f * DX_PI ), &Sin, &SmoothCos ) ;
	}
	else
	{
		_SINCOS_PLATFORM( Mesh->Container->SmoothingAngle, &Sin, &SmoothCos ) ;
	}

	// �ʏ�񂩂��@�����K�v�����ׂ�
	Face = Mesh->Faces ;
	Mesh->NormalNum = 0 ;
	for( i = 0 ; ( DWORD )i < Mesh->FaceNum ; i ++, Face ++ )
	{
		// ���łɖʂ̖@�����v�Z����
		P0 = &Mesh->Positions[ Face->VertexIndex[ 0 ] ] ;
		P1 = &Mesh->Positions[ Face->VertexIndex[ 1 ] ] ;
		P2 = &Mesh->Positions[ Face->VertexIndex[ 2 ] ] ;

		VectorSub( &V1, P1, P0 ) ;
		VectorSub( &V2, P2, P0 ) ;
		if( ReadModel->MeshFaceRightHand )
		{
			VectorOuterProduct( &Norm, &V2, &V1 ) ;
		}
		else
		{
			VectorOuterProduct( &Norm, &V1, &V2 ) ;
		}
		VectorNormalize( &Face->Normal, &Norm ) ;

		// �@���C���f�b�N�X���Z�b�g
		for( j = 0 ; ( DWORD )j < Face->IndexNum ; j ++ )
			Face->NormalIndex[ j ] = Mesh->NormalNum + j ;

		// �@���̐����C���f�b�N�X�̐��������₷
		Mesh->NormalNum += Face->IndexNum ;
	}
	IndexNum = Mesh->NormalNum ;

	// �@�����i�[���郁�������m�ۂ���
	Mesh->Normals = ( VECTOR * )DXALLOC( ( sizeof( VECTOR ) + sizeof( BYTE ) + sizeof( DWORD ) + sizeof( DWORD ) ) * Mesh->NormalNum ) ;
	if( Mesh->Normals == NULL )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\x52\x00\x65\x00\x61\x00\x64\x00\x20\x00\x4d\x00\x6f\x00\x64\x00\x65\x00\x6c\x00\x20\x00\x43\x00\x6f\x00\x6e\x00\x76\x00\x65\x00\x72\x00\x74\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xd5\x6c\xda\x7d\xea\x81\xd5\x52\x5c\x4f\x10\x62\xe6\x51\x06\x74\x67\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xd5\x6c\xda\x7d\x6e\x30\xe1\x30\xe2\x30\xea\x30\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Read Model Convert Error : �@�������쐬�����Ŋi�[����@���̃������̊m�ۂɎ��s���܂���\n" @*/ )) ;
		return false ;
	}
	_MEMSET( Mesh->Normals, 0, ( sizeof( VECTOR ) + sizeof( BYTE ) + sizeof( DWORD ) + sizeof( DWORD ) ) * Mesh->NormalNum ) ;
	Mesh->NormalSetFlag = ( BYTE * )( Mesh->Normals + Mesh->NormalNum ) ;
	NormalUseTable = ( BYTE * )( Mesh->Normals + Mesh->NormalNum ) ;
	UseNormalIndex = ( DWORD * )( NormalUseTable + Mesh->NormalNum ) ;
	NewNormalIndex = UseNormalIndex + Mesh->NormalNum ;

	// �ʂ̒��_�C���f�b�N�X�̖����ɃC���f�b�N�X��ǉ����邽�߂Ƀo�b�t�@���g������
	if( MV1RSetupMeshFaceBuffer( ReadModel, Mesh, ( int )Mesh->FaceNum, ( int )( Mesh->FaceUnitMaxIndexNum * 2 ) ) < 0 )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\x52\x00\x65\x00\x61\x00\x64\x00\x20\x00\x4d\x00\x6f\x00\x64\x00\x65\x00\x6c\x00\x20\x00\x43\x00\x6f\x00\x6e\x00\x76\x00\x65\x00\x72\x00\x74\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xd5\x6c\xda\x7d\xea\x81\xd5\x52\x5c\x4f\x10\x62\xe6\x51\x06\x74\x67\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\x02\x98\xb9\x70\xa4\x30\xf3\x30\xc7\x30\xc3\x30\xaf\x30\xb9\x30\x6e\x30\xe1\x30\xe2\x30\xea\x30\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Read Model Convert Error : �@�������쐬�����Ŋi�[���钸�_�C���f�b�N�X�̃������̊m�ۂɎ��s���܂���\n" @*/ )) ;
		return false ;
	}

	// �S�Ă̖ʂ̒��_�C���f�b�N�X�̖����ɃC���f�b�N�X��������
	Face = Mesh->Faces ;
	for( i = 0 ; ( DWORD )i < Mesh->FaceNum ; i ++, Face ++ )
	{
		for( j = 0 ; ( DWORD )j < Face->IndexNum ; j ++ )
		{
			Face->VertexIndex[ Face->IndexNum + j ] = Face->VertexIndex[ j ] ;
		}
	}

	// �e���_�Ɋ֌W����ʂ̃��X�g���쐬����
	{
		VertexFaceList = ( MV1_MAKEVERTINDEXINFO ** )DXALLOC( sizeof( MV1_MAKEVERTINDEXINFO * ) * Mesh->PositionNum + sizeof( MV1_MAKEVERTINDEXINFO ) * IndexNum ) ;
		if( VertexFaceList == NULL )
		{
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\x52\x00\x65\x00\x61\x00\x64\x00\x20\x00\x4d\x00\x6f\x00\x64\x00\x65\x00\x6c\x00\x20\x00\x43\x00\x6f\x00\x6e\x00\x76\x00\x65\x00\x72\x00\x74\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xa2\x95\xc2\x4f\x59\x30\x8b\x30\x62\x97\x6e\x30\xc5\x60\x31\x58\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Read Model Convert Error : �֌W����ʂ̏����i�[���郁�����̊m�ۂɎ��s���܂���\n" @*/ )) ;
			return false ;
		}
		_MEMSET( VertexFaceList, 0, sizeof( MV1_MAKEVERTINDEXINFO * ) * Mesh->PositionNum ) ;
		VertexFaceBuffer = ( MV1_MAKEVERTINDEXINFO * )( VertexFaceList + Mesh->PositionNum ) ;

		// �S�Ă̖ʂ̎Q�Ƃ��Ă��钸�_�̏����Z�b�g����
		Face = Mesh->Faces ;
		VFBuf = VertexFaceBuffer ;
		for( i = 0 ; ( DWORD )i < Mesh->FaceNum ; i ++, Face ++ )
		{
			VFBuf->Face = Face ;
			VFBuf->Next = VertexFaceList[ Face->VertexIndex[ 0 ] ] ;
			VertexFaceList[ Face->VertexIndex[ 0 ] ] = VFBuf ;
			VFBuf ++ ;

			VFBuf->Face = Face ;
			VFBuf->Next = VertexFaceList[ Face->VertexIndex[ 1 ] ] ;
			VertexFaceList[ Face->VertexIndex[ 1 ] ] = VFBuf ;
			VFBuf ++ ;

			VFBuf->Face = Face ;
			VFBuf->Next = VertexFaceList[ Face->VertexIndex[ 2 ] ] ;
			VertexFaceList[ Face->VertexIndex[ 2 ] ] = VFBuf ;
			VFBuf ++ ;
		}
	}

	// �ʂ̖@�����\�z����
	Face = Mesh->Faces ;
	for( i = 0 ; ( DWORD )i < Mesh->FaceNum ; i ++, Face ++ )
	{
		// ���ɏ����ς݂̏ꍇ�͉������Ȃ�
		if( Mesh->NormalSetFlag[ Face->NormalIndex[ 0 ] ] &&
			Mesh->NormalSetFlag[ Face->NormalIndex[ 1 ] ] &&
			Mesh->NormalSetFlag[ Face->NormalIndex[ 2 ] ] )
			continue ;

		// ���_�̐������J��Ԃ�
		for( j = 0 ; ( DWORD )j < Face->IndexNum ; j ++ )
		{
			// ���Ɍv�Z�ς݂̏ꍇ�͉������Ȃ�
			if( Mesh->NormalSetFlag[ Face->NormalIndex[ j ] ] ) continue ;

			// �A���ʂ����X�g�A�b�v
			{
				FaceList[ 0 ] = Face ;
				FaceNorm = Face->Normal ;
				FaceIndex[ 0 ] = ( BYTE )j ;
				Mesh->NormalSetFlag[ Face->NormalIndex[ j ] ] |= 2 ;
				FaceCount = 1 ;

				// �Е��̘A���ʂ����X�g�A�b�v
				CurFace = Face ;
				BackCurFace = NULL ;
				CurFaceIndex = j ;
				Issyuu = 0 ;
				for(;;)
				{
					MinFace = NULL ;

					// �@��������o���������_����אڂ��Ă��邩���ׂ����ӂ̒��_�֌������x�N�g�����y�A�����Е��̒��_�֌������x�N�g�����w�Ƃ����O���̍쐬
					VectorSub( &zv, &Mesh->Positions[ CurFace->VertexIndex[ CurFaceIndex + 1 ] ], &Mesh->Positions[ CurFace->VertexIndex[ CurFaceIndex ] ] ) ;
					VectorSub( &xv, &Mesh->Positions[ CurFace->VertexIndex[ CurFaceIndex + 2 ] ], &Mesh->Positions[ CurFace->VertexIndex[ CurFaceIndex ] ] ) ;
					VectorOuterProduct( &yv, &zv, &xv ) ;
					VectorOuterProduct( &xv, &yv, &zv ) ;
					VectorNormalize( &xv, &xv ) ;
					VectorNormalize( &yv, &yv ) ;
					VectorNormalize( &zv, &zv ) ;

					// �אڂ��Ă��邩���ׂ����ӂ̒��_�Ƃ͕ʂ̂�����̒��_�֌������x�N�g������L�ŎZ�o�����O����ɓ��e�����x�N�g�����Z�o
					VectorSub( &tv, &Mesh->Positions[ CurFace->VertexIndex[ CurFaceIndex + 2 ] ], &Mesh->Positions[ CurFace->VertexIndex[ CurFaceIndex ] ] ) ;
					V1.x = xv.x * tv.x + xv.y * tv.y + xv.z * tv.z ;
					V1.y = yv.x * tv.x + yv.y * tv.y + yv.z * tv.z ;
					V1.z = zv.x * tv.x + zv.y * tv.y + zv.z * tv.z ;
					V1.z = 0.0f ;
					VectorNormalize( &V1, &V1 ) ;

					// ����̕ӂ����|���S���̌���
					for( VFBuf = VertexFaceList[ CurFace->VertexIndex[ CurFaceIndex ] ] ; VFBuf ; VFBuf = VFBuf->Next )
					{
						FaceT = ( MV1_MESHFACE_R * )VFBuf->Face ;

						// ���Ɍ��o�ς݂̃|���S���͖���
						if( ( Mesh->NormalSetFlag[ FaceT->NormalIndex[ 0 ] ] & 2 ) ||
							( Mesh->NormalSetFlag[ FaceT->NormalIndex[ 1 ] ] & 2 ) ||
							( Mesh->NormalSetFlag[ FaceT->NormalIndex[ 2 ] ] & 2 ) ) continue ;

						// �������g���A��O�Ɏ����������|���S���͖���
						if( FaceT == CurFace || FaceT == BackCurFace ) continue ;

						// ���_�̐������J��Ԃ�
						for( m = 0 ; ( DWORD )m < FaceT->IndexNum ; m ++ )
						{
							CmpFlag = 0 ;

							// �����ӂ��������ׂ�A���_�w��̏��Ԃ���ʂ̕����𒲂ׂ�
							if( CurFace->VertexIndex[ CurFaceIndex     ] == FaceT->VertexIndex[ m     ] &&
								CurFace->VertexIndex[ CurFaceIndex + 1 ] == FaceT->VertexIndex[ m + 1 ] )
							{
								CmpFlag = 1 ;
								FaceUra = 1 ;
							}

							if( CurFace->VertexIndex[ CurFaceIndex     ] == FaceT->VertexIndex[ m + 1 ] &&
								CurFace->VertexIndex[ CurFaceIndex + 1 ] == FaceT->VertexIndex[ m     ] )
							{
								CmpFlag = 1 ;
								FaceUra = 0 ;
							}

							// �J�����g�ʂƓ���̒��_���g�p����P�W�O�x��]������( �J�����g�ʂ̗��� )�������猟�o�������Ƃ𖳂��������Ƃɂ���
							if( CmpFlag == 1 && CurFace->VertexIndex[ CurFaceIndex + 2 ] == FaceT->VertexIndex[ m + 2 ] )
							{
								CmpFlag = 0 ;
							}

							// ����̕ӂ����������烋�[�v�𔲂���
							if( CmpFlag != 0 ) break ;
						}

						// ����̕ӂ����������珈��
						if( ( DWORD )m != FaceT->IndexNum )
						{
							// �������|���S���́A����̕ӂ𐬂����_�Ƃ͕ʂ̂�����̒��_�֌������x�N�g�����ɎZ�o�����O���֓��e�����x�N�g�����Z�o����
							VectorSub( &tv, &Mesh->Positions[ FaceT->VertexIndex[ m + 2 ] ], &Mesh->Positions[ CurFace->VertexIndex[ CurFaceIndex ] ] ) ;
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
								( ( ( FaceT->VertexIndex[ 0 ] == MinFace->VertexIndex[ 0 ] &&
								      FaceT->VertexIndex[ 1 ] == MinFace->VertexIndex[ 2 ] &&
								      FaceT->VertexIndex[ 2 ] == MinFace->VertexIndex[ 1 ] ) ||
								    ( FaceT->VertexIndex[ 0 ] == MinFace->VertexIndex[ 1 ] &&
								      FaceT->VertexIndex[ 1 ] == MinFace->VertexIndex[ 0 ] &&
								      FaceT->VertexIndex[ 2 ] == MinFace->VertexIndex[ 2 ] ) ||
								    ( FaceT->VertexIndex[ 0 ] == MinFace->VertexIndex[ 2 ] &&
								      FaceT->VertexIndex[ 1 ] == MinFace->VertexIndex[ 1 ] &&
								      FaceT->VertexIndex[ 2 ] == MinFace->VertexIndex[ 0 ] ) ) && FaceUra == 0 && MinFaceUra == 1 ) ||
								( FaceRadi2Use == 0 && MinFaceRadi2Use == 1 ) ||
								( FaceRadi2Use == 1 && MinFaceRadi2Use == 1 && FaceRadi2 > MinFaceRadi2 ) ||
								( FaceRadi2Use == 0 && MinFaceRadi2Use == 0 && FaceRadi  > MinFaceRadi ) )
							{
								MinFace = FaceT ;
								MinFaceRadi = FaceRadi ;
								MinFaceRadi2Use = FaceRadi2Use ;
								MinFaceRadi2 = FaceRadi2 ;
								MinFaceUra = FaceUra ;
								MinFaceIndex = m + 1 ;
								if( ( DWORD )MinFaceIndex == FaceT->IndexNum ) MinFaceIndex = 0 ;
							}
						}
					}

					// �אږʂ𔭌��ł��Ȃ��������A�אږʂ����ʂ��������A�אږʂƂ̖@���̊p�x�����X���[�W���O�Ώۂ�臒l�𒴂��Ă�����G�b�W�Ƃ݂Ȃ�
					if( MinFace == NULL || MinFaceUra || VectorInnerProduct( &MinFace->Normal, &CurFace->Normal ) < SmoothCos )
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
					VectorAdd( &FaceNorm, &FaceNorm, &MinFace->Normal ) ;
					Mesh->NormalSetFlag[ MinFace->NormalIndex[ MinFaceIndex ] ] |= 2 ;
					FaceCount ++ ;

					// �J�����g�ʂ̕ύX
					BackCurFace = CurFace ;
					CurFace = MinFace ;
					CurFaceIndex = MinFaceIndex ;
				}

				// �����Е��̘A���ʂ����X�g�A�b�v
				if( Issyuu == 0 )
				{
					BackCurFace = NULL ;
					CurFace = Face ;
					CurFaceIndex = j ;
					for(;;)
					{
						MinFace = NULL ;

						// �@��������o���������_����אڂ��Ă��邩���ׂ����ӂ̒��_�֌������x�N�g�����y�A�����Е��̒��_�֌������x�N�g�����w�Ƃ����R���x�N�g���̍쐬
						VectorSub( &zv, &Mesh->Positions[ CurFace->VertexIndex[ CurFaceIndex + CurFace->IndexNum - 1 ] ], &Mesh->Positions[ CurFace->VertexIndex[ CurFaceIndex ] ] ) ;
						VectorSub( &xv, &Mesh->Positions[ CurFace->VertexIndex[ CurFaceIndex + CurFace->IndexNum - 2 ] ], &Mesh->Positions[ CurFace->VertexIndex[ CurFaceIndex ] ] ) ;
						VectorOuterProduct( &yv, &zv, &xv ) ;
						VectorOuterProduct( &xv, &yv, &zv ) ;
						VectorNormalize( &xv, &xv ) ;
						VectorNormalize( &yv, &yv ) ;
						VectorNormalize( &zv, &zv ) ;

						// �אڂ��Ă��邩���ׂ����ӂ̒��_�Ƃ͕ʂ̂�����̒��_�֌������x�N�g������L�ŎZ�o�����O����ɓ��e�����x�N�g�����Z�o
						VectorSub( &tv, &Mesh->Positions[ CurFace->VertexIndex[ CurFaceIndex + CurFace->IndexNum - 2 ] ], &Mesh->Positions[ CurFace->VertexIndex[ CurFaceIndex ] ] ) ;
						V1.x = xv.x * tv.x + xv.y * tv.y + xv.z * tv.z ;
						V1.y = yv.x * tv.x + yv.y * tv.y + yv.z * tv.z ;
						V1.z = zv.x * tv.x + zv.y * tv.y + zv.z * tv.z ;
						V1.z = 0.0f ;
						VectorNormalize( &V1, &V1 ) ;

						// ����̕ӂ����|���S���̌���
						for( VFBuf = VertexFaceList[ CurFace->VertexIndex[ CurFaceIndex ] ] ; VFBuf ; VFBuf = VFBuf->Next )
						{
							FaceT = ( MV1_MESHFACE_R * )VFBuf->Face ;

							// ���Ɍ��o�ς݂̃|���S���͖���
							if( ( Mesh->NormalSetFlag[ FaceT->NormalIndex[ 0 ] ] & 2 ) ||
								( Mesh->NormalSetFlag[ FaceT->NormalIndex[ 1 ] ] & 2 ) ||
								( Mesh->NormalSetFlag[ FaceT->NormalIndex[ 2 ] ] & 2 ) ) continue ;

							// �������g���A��O�Ɏ����������|���S���͖���
							if( FaceT == CurFace || FaceT == BackCurFace ) continue ;

							// ���_�̐������J��Ԃ�
							for( m = 0 ; ( DWORD )m < FaceT->IndexNum ; m ++ )
							{
								CmpFlag = 0 ;

								// �����ӂ��������ׂ�A���_�w��̏��Ԃ���ʂ̕����𒲂ׂ�

								if( CurFace->VertexIndex[ CurFaceIndex                         ] == FaceT->VertexIndex[ m                       ] &&
									CurFace->VertexIndex[ CurFaceIndex + CurFace->IndexNum - 1 ] == FaceT->VertexIndex[ m + FaceT->IndexNum - 1 ] )
								{
									CmpFlag = 1 ;
									FaceUra = 1 ;
								}

								if( CurFace->VertexIndex[ CurFaceIndex                         ] == FaceT->VertexIndex[ m + FaceT->IndexNum - 1 ] &&
									CurFace->VertexIndex[ CurFaceIndex + CurFace->IndexNum - 1 ] == FaceT->VertexIndex[ m                       ] )
								{
									CmpFlag = 1 ;
									FaceUra = 0 ;
								}

								// �J�����g�ʂƓ���̒��_���g�p����P�W�O�x��]������( �J�����g�ʂ̗��� )�������猟�o�������Ƃ𖳂��������Ƃɂ���
								if( CmpFlag == 1 && CurFace->VertexIndex[ CurFaceIndex + CurFace->IndexNum - 2 ] == FaceT->VertexIndex[ m + FaceT->IndexNum - 2 ] )
								{
									CmpFlag = 0 ;
								}

								// ����̕ӂ����������烋�[�v�𔲂���
								if( CmpFlag != 0 ) break ;
							}

							// ����̕ӂ����������珈��
							if( ( DWORD )m != FaceT->IndexNum )
							{
								// �������|���S���́A����̕ӂ𐬂����_�Ƃ͕ʂ̂�����̒��_�֌������x�N�g�����ɎZ�o�����O���֓��e�����x�N�g�����Z�o����
								VectorSub( &tv, &Mesh->Positions[ FaceT->VertexIndex[ m + FaceT->IndexNum - 2 ] ], &Mesh->Positions[ CurFace->VertexIndex[ CurFaceIndex ] ] ) ;
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
									( ( ( FaceT->VertexIndex[ 0 ] == MinFace->VertexIndex[ 0 ] &&
										  FaceT->VertexIndex[ 1 ] == MinFace->VertexIndex[ 2 ] &&
										  FaceT->VertexIndex[ 2 ] == MinFace->VertexIndex[ 1 ] ) ||
										( FaceT->VertexIndex[ 0 ] == MinFace->VertexIndex[ 1 ] &&
										  FaceT->VertexIndex[ 1 ] == MinFace->VertexIndex[ 0 ] &&
										  FaceT->VertexIndex[ 2 ] == MinFace->VertexIndex[ 2 ] ) ||
										( FaceT->VertexIndex[ 0 ] == MinFace->VertexIndex[ 2 ] &&
										  FaceT->VertexIndex[ 1 ] == MinFace->VertexIndex[ 1 ] &&
										  FaceT->VertexIndex[ 2 ] == MinFace->VertexIndex[ 0 ] ) ) && FaceUra == 0 && MinFaceUra == 1 ) ||
									( FaceRadi2Use == 0 && MinFaceRadi2Use == 1 ) ||
									( FaceRadi2Use == 1 && MinFaceRadi2Use == 1 && FaceRadi2 > MinFaceRadi2 ) ||
									( FaceRadi2Use == 0 && MinFaceRadi2Use == 0 && FaceRadi  > MinFaceRadi ) )
								{
									MinFace = FaceT ;
									MinFaceRadi = FaceRadi ;
									MinFaceRadi2Use = FaceRadi2Use ;
									MinFaceRadi2 = FaceRadi2 ;
									MinFaceUra = FaceUra ;
									MinFaceIndex = m - 1 ;
									if( MinFaceIndex < 0 ) MinFaceIndex += FaceT->IndexNum ;
								}
							}
						}

						// �אږʂ𔭌��ł��Ȃ��������A�אږʂ����ʂ��������A�אږʂƂ̖@���̊p�x�����X���[�W���O�Ώۂ�臒l�𒴂��Ă�����G�b�W�Ƃ݂Ȃ�
						if( MinFace == NULL || MinFaceUra || VectorInnerProduct( &MinFace->Normal, &CurFace->Normal ) < SmoothCos )
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
						VectorAdd( &FaceNorm, &FaceNorm, &MinFace->Normal ) ;
						Mesh->NormalSetFlag[ MinFace->NormalIndex[ MinFaceIndex ] ] |= 2 ;
						FaceCount ++ ;

						// �J�����g�ʂ̕ύX
						BackCurFace = CurFace ;
						CurFace = MinFace ;
						CurFaceIndex = MinFaceIndex ;
					}
				}

				// ���o�����ʂ̖@���𑫂������̂𐳋K��
				VectorNormalize( &FaceNorm, &FaceNorm ) ;
			}

			// ���X�g�A�b�v���ꂽ�ʂɐ��K�������@�����Z�b�g����
			for( k = 0 ; k < FaceCount ; k ++ )
			{
				Index = ( int )FaceList[ k ]->NormalIndex[ FaceIndex[ k ] ] ;
				Mesh->Normals[ Index ] = FaceNorm ;
				Mesh->NormalSetFlag[ Index ] = 1 ;
				FaceList[ k ]->NormalIndex[ FaceIndex[ k ] ] = FaceList[ 0 ]->NormalIndex[ FaceIndex[ 0 ] ] ;
			}
		}
	}

	// �g�p���Ă���@�����C���f�b�N�X�̎Ⴂ���ɋl�߂�
	{
		// �g�p����Ă���@���̃}�b�v���쐬����
		_MEMSET( NormalUseTable, 0, Mesh->NormalNum ) ;
		Face = Mesh->Faces ;
		UseNormalNum = 0 ;
		for( i = 0 ; ( DWORD )i < Mesh->FaceNum ; i ++, Face ++ )
		{
			if( NormalUseTable[ Face->NormalIndex[ 0 ] ] == 0 )
			{
				NormalUseTable[ Face->NormalIndex[ 0 ] ] = 1 ;
				UseNormalIndex[ UseNormalNum ] = Face->NormalIndex[ 0 ] ;
				NewNormalIndex[ Face->NormalIndex[ 0 ] ] = UseNormalNum ;
				UseNormalNum ++ ;
			}

			if( NormalUseTable[ Face->NormalIndex[ 1 ] ] == 0 )
			{
				NormalUseTable[ Face->NormalIndex[ 1 ] ] = 1 ;
				UseNormalIndex[ UseNormalNum ] = Face->NormalIndex[ 1 ] ;
				NewNormalIndex[ Face->NormalIndex[ 1 ] ] = UseNormalNum ;
				UseNormalNum ++ ;
			}

			if( NormalUseTable[ Face->NormalIndex[ 2 ] ] == 0 )
			{
				NormalUseTable[ Face->NormalIndex[ 2 ] ] = 1 ;
				UseNormalIndex[ UseNormalNum ] = Face->NormalIndex[ 2 ] ;
				NewNormalIndex[ Face->NormalIndex[ 2 ] ] = UseNormalNum ;
				UseNormalNum ++ ;
			}
		}

		// �ŏI�ł̖@�����i�[���邽�߂̃������̊m��
		Normal = Mesh->Normals ;
		Mesh->Normals = ( VECTOR * )ADDMEMAREA( sizeof( VECTOR ) * UseNormalNum, &ReadModel->Mem ) ;
		if( Mesh->Normals == NULL )
		{
			DXFREE( Normal ) ;
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\x52\x00\x65\x00\x61\x00\x64\x00\x20\x00\x4d\x00\x6f\x00\x64\x00\x65\x00\x6c\x00\x20\x00\x43\x00\x6f\x00\x6e\x00\x76\x00\x65\x00\x72\x00\x74\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xd5\x6c\xda\x7d\xea\x81\xd5\x52\x5c\x4f\x10\x62\xe6\x51\x06\x74\x67\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xd5\x6c\xda\x7d\x6e\x30\xe1\x30\xe2\x30\xea\x30\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x20\x00\x5f\x00\x20\x00\x32\x00\x0a\x00\x00"/*@ L"Read Model Convert Error : �@�������쐬�����Ŋi�[����@���̃������̊m�ۂɎ��s���܂��� _ 2\n" @*/ )) ;
			return false ;
		}

		// �ŏI�ł̖@���Z�b�g���쐬����
		for( i = 0 ; ( DWORD )i < UseNormalNum ; i ++ )
		{
			Mesh->Normals[ i ] = Normal[ UseNormalIndex[ i ] ] ;
		}

		// �ʃf�[�^�̖@���C���f�b�N�X���X�V����
		Face = Mesh->Faces ;
		for( i = 0 ; ( DWORD )i < Mesh->FaceNum ; i ++, Face ++ )
		{
			Face->NormalIndex[ 0 ] = NewNormalIndex[ Face->NormalIndex[ 0 ] ] ;
			Face->NormalIndex[ 1 ] = NewNormalIndex[ Face->NormalIndex[ 1 ] ] ;
			Face->NormalIndex[ 2 ] = NewNormalIndex[ Face->NormalIndex[ 2 ] ] ;
		}

		// �ŏI�I�Ȗ@���̐����Z�b�g
		Mesh->NormalNum = UseNormalNum ;
	}

	// �ꎞ�I�ȃf�[�^���i�[���Ă����������̈�̉��
	DXFREE( Normal ) ;
	Normal = NULL ;

	// �������̉��
	if( VertexFaceList )
	{
		DXFREE( VertexFaceList ) ;
		VertexFaceList = NULL ;
	}

	// ����I��
	return true ;
}

// ��]�s�����Z����
static void MV1MultiplyRotateMatrix( MATRIX *DestBuffer, VECTOR *Rotate, int RotateOrder )
{
	MATRIX RotMatX, RotMatY, RotMatZ ;

	CreateRotationXMatrix( &RotMatX, Rotate->x ) ;
	CreateRotationYMatrix( &RotMatY, Rotate->y ) ;
	CreateRotationZMatrix( &RotMatZ, Rotate->z ) ;

	switch( RotateOrder )
	{
	case MV1_ROTATE_ORDER_XYZ :
		CreateMultiplyMatrix( DestBuffer, DestBuffer, &RotMatX ) ;
		CreateMultiplyMatrix( DestBuffer, DestBuffer, &RotMatY ) ;
		CreateMultiplyMatrix( DestBuffer, DestBuffer, &RotMatZ ) ;
		break ;

	case MV1_ROTATE_ORDER_XZY : 
		CreateMultiplyMatrix( DestBuffer, DestBuffer, &RotMatX ) ;
		CreateMultiplyMatrix( DestBuffer, DestBuffer, &RotMatZ ) ;
		CreateMultiplyMatrix( DestBuffer, DestBuffer, &RotMatY ) ;
		break ;

	case MV1_ROTATE_ORDER_YZX :
		CreateMultiplyMatrix( DestBuffer, DestBuffer, &RotMatY ) ;
		CreateMultiplyMatrix( DestBuffer, DestBuffer, &RotMatZ ) ;
		CreateMultiplyMatrix( DestBuffer, DestBuffer, &RotMatX ) ;
		break ;

	case MV1_ROTATE_ORDER_YXZ :
		CreateMultiplyMatrix( DestBuffer, DestBuffer, &RotMatY ) ;
		CreateMultiplyMatrix( DestBuffer, DestBuffer, &RotMatX ) ;
		CreateMultiplyMatrix( DestBuffer, DestBuffer, &RotMatZ ) ;
		break ;

	case MV1_ROTATE_ORDER_ZXY :
		CreateMultiplyMatrix( DestBuffer, DestBuffer, &RotMatZ ) ;
		CreateMultiplyMatrix( DestBuffer, DestBuffer, &RotMatX ) ;
		CreateMultiplyMatrix( DestBuffer, DestBuffer, &RotMatY ) ;
		break ;

	case MV1_ROTATE_ORDER_ZYX :
		CreateMultiplyMatrix( DestBuffer, DestBuffer, &RotMatZ ) ;
		CreateMultiplyMatrix( DestBuffer, DestBuffer, &RotMatY ) ;
		CreateMultiplyMatrix( DestBuffer, DestBuffer, &RotMatX ) ;
		break ;
	}
}

// ���W�ϊ��s��̍쐬
extern int MV1RMakeMatrix( VECTOR *PreRotate, VECTOR *Rotate, VECTOR *PostRotate, VECTOR *Scale, VECTOR *Translate, VECTOR *MatrixRXYZ, MATRIX *DestBuffer, int RotateOrder )
{
	MATRIX TransMat, Matrix3x3 ;

	CreateScalingMatrix( DestBuffer, Scale->x, Scale->y, Scale->z ) ;

	if( PostRotate != NULL )
	{
		MV1MultiplyRotateMatrix( DestBuffer, PostRotate, RotateOrder ) ;
	}

	MV1MultiplyRotateMatrix( DestBuffer, Rotate, RotateOrder ) ;

	if( PreRotate != NULL )
	{
		MV1MultiplyRotateMatrix( DestBuffer, PreRotate, RotateOrder ) ;
	}

	if( MatrixRXYZ )
	{
		CreateRotationXYZMatrix( &Matrix3x3, MatrixRXYZ->x, MatrixRXYZ->y, MatrixRXYZ->z ) ;
		CreateMultiplyMatrix( DestBuffer, DestBuffer, &Matrix3x3 ) ;
	}

	CreateTranslationMatrix( &TransMat, Translate->x, Translate->y, Translate->z ) ;
	CreateMultiplyMatrix( DestBuffer, DestBuffer, &TransMat ) ;

	// �I��
	return 0 ;
}

// �t�@�C�����ۂ��Ɠǂݍ���
extern int MV1RLoadFile( const char *FilePath, void **FileImage, int *FileSize )
{
	DWORD_PTR FileHandle ;
	size_t Size ;
	void *Image ;
	wchar_t FilePathW[ 1024 ] ;

	// �e�N�X�`���t�@�C�����J��
	ConvString( ( const char * )FilePath, DX_CHARCODEFORMAT_SHIFTJIS, ( char * )FilePathW, sizeof( FilePathW ), WCHAR_T_CHARCODEFORMAT ) ;
	FileHandle = DX_FOPEN( FilePathW ) ;

	// �t�@�C�����J���Ȃ�������J�����g�t�H���_����J�����Ƃ��Ă݂�
	if( FileHandle == 0 )
	{
		wchar_t FileName[ 512 ] ;

		AnalysisFileNameAndDirPathW_( FilePathW, FileName, sizeof( FileName ), NULL, 0 ) ;
		FileHandle = DX_FOPEN( FileName ) ;
	}

	// �t�@�C��������������G���[
	if( FileHandle == 0 )
		return -1 ;

	// �t�@�C���T�C�Y���擾
	DX_FSEEK( FileHandle, 0L, SEEK_END ) ;
	Size = ( size_t )DX_FTELL( FileHandle ) ;
	DX_FSEEK( FileHandle, 0L, SEEK_SET ) ;

	// �e�N�X�`���t�@�C�������܂郁�����̈�̊m��
	Image = DXALLOC( Size ) ;
	if( Image == NULL )
	{
		DX_FCLOSE( FileHandle ) ;
		return -1 ;
	}

	// �t�@�C�����ۂ��Ɠǂݍ���
	DX_FREAD( Image, Size, 1, FileHandle ) ;

	// �J�����t�@�C�������
	DX_FCLOSE( FileHandle ) ;

	// �A�h���X���Z�b�g
	if( FileImage ) *FileImage = Image ;
	if( FileSize ) *FileSize = ( int )Size ;

	// �I��
	return 0 ;
}

// �t�@�C�����ۂ��Ɠǂݍ���
extern int MV1RLoadFileW( const wchar_t *FilePathW, void **FileImage, int *FileSize )
{
	DWORD_PTR FileHandle ;
	size_t Size ;
	void *Image ;

	// �e�N�X�`���t�@�C�����J��
	FileHandle = DX_FOPEN( FilePathW ) ;

	// �t�@�C�����J���Ȃ�������J�����g�t�H���_����J�����Ƃ��Ă݂�
	if( FileHandle == 0 )
	{
		wchar_t FileNameW[ 512 ] ;

		AnalysisFileNameAndDirPathW_( FilePathW, FileNameW, sizeof( FileNameW ), NULL, 0 ) ;
		FileHandle = DX_FOPEN( FileNameW ) ;
	}

	// �t�@�C��������������G���[
	if( FileHandle == 0 )
		return -1 ;

	// �t�@�C���T�C�Y���擾
	DX_FSEEK( FileHandle, 0L, SEEK_END ) ;
	Size = ( size_t )DX_FTELL( FileHandle ) ;
	DX_FSEEK( FileHandle, 0L, SEEK_SET ) ;

	// �e�N�X�`���t�@�C�������܂郁�����̈�̊m��
	Image = DXALLOC( Size ) ;
	if( Image == NULL )
	{
		DX_FCLOSE( FileHandle ) ;
		return -1 ;
	}

	// �t�@�C�����ۂ��Ɠǂݍ���
	DX_FREAD( Image, Size, 1, FileHandle ) ;

	// �J�����t�@�C�������
	DX_FCLOSE( FileHandle ) ;

	// �A�h���X���Z�b�g
	if( FileImage ) *FileImage = Image ;
	if( FileSize ) *FileSize = ( int )Size ;

	// �I��
	return 0 ;
}

#ifndef UNICODE
// ���f����{�f�[�^�p�̕�����X�y�[�X���m�ۂ���
static char *MV1RGetStringSpace( MV1_MODEL_BASE *MBase, const char *String )
{
	char *Return ;

	Return = ( char * )( ( BYTE * )MBase->StringBufferA + MBase->StringSizeA ) ;
	_STRCPY( Return, String ) ;
	MBase->StringSizeA += ( ( ( int )_STRLEN( String ) + 1 ) * sizeof( char ) + 3 ) / 4 * 4 ;

	return Return ;
}
#endif

// ���f����{�f�[�^�p�̕�����X�y�[�X���m�ۂ���
static wchar_t *MV1RGetStringSpaceW( MV1_MODEL_BASE *MBase, const wchar_t *String )
{
	wchar_t *Return ;

	Return = ( wchar_t * )( ( BYTE * )MBase->StringBufferW + MBase->StringSizeW ) ;
	_WCSCPY( Return, String ) ;
	MBase->StringSizeW += ( int )( ( ( _WCSLEN( String ) + 1 ) * sizeof( wchar_t ) + 3 ) / 4 * 4 ) ;

	return Return ;
}

// �ǂݍ��ݏ����p���f���\���̂̏�����
extern int MV1InitReadModel( MV1_MODEL_R *ReadModel )
{
	// �[��������
	_MEMSET( ReadModel, 0, sizeof( MV1_MODEL_R ) ) ;

	// �I��
	return 0 ;
}

// �ǂݍ��ݏ����p���f���\���̂̌�n��
extern int MV1TermReadModel( MV1_MODEL_R *ReadModel )
{
	// �������̉��
	ClearMemArea( &ReadModel->Mem ) ;

	if( ReadModel->FilePath )
	{
		DXFREE( ReadModel->FilePath ) ;
		ReadModel->FilePath = NULL ;
	}

	if( ReadModel->Name )
	{
		DXFREE( ReadModel->Name ) ;
		ReadModel->Name = NULL ;
	}

	// �I��
	return 0 ;
}

#ifndef UNICODE

// ������̒ǉ�
extern char *MV1RAddString( MV1_MODEL_R *ReadModel, const char *String )
{
	int Length ;
	char *Buffer ;

	// ������̒������擾
	Length = ( int )_STRLEN( String ) ;

	// �������̊m��
	Buffer = ( char * )AddMemArea( ( size_t )( ( Length + 1 ) * sizeof( char ) ), &ReadModel->Mem ) ;
	if( Buffer == NULL )
		return NULL ;

	// �R�s�[
	_STRCPY( Buffer, String ) ;

	// ������f�[�^�̃T�C�Y�����Z
	ReadModel->StringSizeA += ( Length + 1 ) * sizeof( char ) ;

	// �S�̔{���ɍ��킹��
	ReadModel->StringSizeA = ( ReadModel->StringSizeA + 3 ) / 4 * 4 ;

	// �I��
	return Buffer ;
}

#endif

// ������̒ǉ�
extern wchar_t *MV1RAddStringW( MV1_MODEL_R *ReadModel, const wchar_t *StringW )
{
	int Length ;
	wchar_t *Buffer ;

	// ������̒������擾
	Length = ( int )_WCSLEN( StringW ) ;

	// �������̊m��
	Buffer = ( wchar_t * )AddMemArea( ( size_t )( ( Length + 1 ) * sizeof( wchar_t ) ), &ReadModel->Mem ) ;
	if( Buffer == NULL )
		return NULL ;

	// �R�s�[
	_WCSCPY( Buffer, StringW ) ;

	// ������f�[�^�̃T�C�Y�����Z
	ReadModel->StringSizeW += ( Length + 1 ) * sizeof( wchar_t ) ;

	// �S�̔{���ɍ��킹��
	ReadModel->StringSizeW = ( ReadModel->StringSizeW + 3 ) / 4 * 4 ;

	// �I��
	return Buffer ;
}

#ifndef UNICODE

// ������̒ǉ�
extern char *MV1RAddStringWToA(	MV1_MODEL_R *ReadModel, const wchar_t *String )
{
	char TempBuffer[ 512 ] ;

	ConvString( ( const char * )String, WCHAR_T_CHARCODEFORMAT, TempBuffer, sizeof( TempBuffer ), ReadModel->CharCodeFormat ) ;
	return MV1RAddString( ReadModel, TempBuffer ) ;
}

#endif

// ������̒ǉ�
extern wchar_t *MV1RAddStringAToW( MV1_MODEL_R *ReadModel, const char *String )
{
	wchar_t TempBuffer[ 512 ] ;

	ConvString( String, ReadModel->CharCodeFormat, ( char * )TempBuffer, sizeof( TempBuffer ), WCHAR_T_CHARCODEFORMAT ) ;
	return MV1RAddStringW( ReadModel, TempBuffer ) ;
}

// �t���[���̒ǉ�
static MV1_FRAME_R *MV1RAddFrameBase( MV1_MODEL_R *ReadModel, const char *NameA, const wchar_t *NameW, MV1_FRAME_R *Parent )
{
	MV1_FRAME_R *Frame ;
	MV1_FRAME_R *TempFrame ;
	int i ;
	BYTE TempBuffer[ 512 ] ;

#ifndef UNICODE
	if( NameA == NULL )
	{
		ConvString( ( const char * )NameW, WCHAR_T_CHARCODEFORMAT, ( char * )TempBuffer, sizeof( TempBuffer ), ReadModel->CharCodeFormat ) ;
		NameA = ( const char * )TempBuffer ;
	}
	else
#endif
	if( NameW == NULL )
	{
		ConvString( ( const char * )NameA, ReadModel->CharCodeFormat, ( char * )TempBuffer, sizeof( TempBuffer ), WCHAR_T_CHARCODEFORMAT ) ;
		NameW = ( const wchar_t * )TempBuffer ;
	}

	// �������̊m��
	Frame = ( MV1_FRAME_R * )AddMemArea( sizeof( MV1_FRAME_R ), &ReadModel->Mem ) ;
	if( Frame == NULL )
	{
		return NULL ;
	}

	// ���X�g�ɒǉ�
	if( ReadModel->FrameFirst == NULL )
	{
		ReadModel->FrameFirst = Frame ;
		ReadModel->FrameLast  = Frame ;
	}
	else
	{
		if( Parent == NULL )
		{
			TempFrame = ReadModel->FrameLast ;
		}
		else
		{
			if( Parent->ChildFirst == NULL )
			{
				TempFrame = Parent ;
			}
			else
			{
				for( TempFrame = Parent->ChildLast ; TempFrame->ChildLast ; TempFrame = TempFrame->ChildLast ){}
			}
		}

		Frame->DataPrev = TempFrame ;
		Frame->DataNext = TempFrame->DataNext ;
		TempFrame->DataNext = Frame ;
		if( Frame->DataNext ) Frame->DataNext->DataPrev = Frame ;

		if( ReadModel->FrameLast == TempFrame )
		{
			ReadModel->FrameLast = Frame ;
		}
	}

	// �e�q���X�g�ɒǉ�
	if( Parent )
	{
		if( Parent->ChildFirst == NULL )
		{
			Parent->ChildFirst = Frame ;
			Parent->ChildLast = Frame ;
		}
		else
		{
			Frame->Prev = Parent->ChildLast ;
			Parent->ChildLast->Next = Frame ;
			Parent->ChildLast = Frame ;
		}
		Frame->Parent = Parent ;
	}

	// �C���f�b�N�X�̐U��Ȃ���
	for( i = 0, TempFrame = ReadModel->FrameFirst ; TempFrame ; i ++, TempFrame = TempFrame->DataNext )
	{
		TempFrame->Index = i ;
	}
	ReadModel->FrameNum ++ ;

	// ���O��ۑ�
#ifndef UNICODE
	Frame->NameA = MV1RAddString( ReadModel, NameA ) ;
	if( Frame->NameA == NULL )
	{
		return NULL ;
	}
#endif
	Frame->NameW = MV1RAddStringW( ReadModel, NameW ) ;
	if( Frame->NameW == NULL )
	{
		return NULL ;
	}

	// ��{���̃Z�b�g
	CreateIdentityMatrix( &Frame->Matrix ) ;
	Frame->Scale.x = 1.0f ;
	Frame->Scale.y = 1.0f ;
	Frame->Scale.z = 1.0f ;
	Frame->Quaternion.w = 1.0f ; 
	Frame->RotateOrder = MV1_ROTATE_ORDER_XYZ ;
	Frame->Visible = 1 ;
	Frame->SmoothingAngle = DX_PI_F / 2.0f - 0.05f ;

	// �I��
	return Frame ;
}

// �t���[���̒ǉ�
extern MV1_FRAME_R *MV1RAddFrame( MV1_MODEL_R *ReadModel, const char *Name, MV1_FRAME_R *Parent )
{
	return MV1RAddFrameBase( ReadModel, Name, NULL, Parent ) ;
}

// �t���[���̒ǉ�
extern MV1_FRAME_R *MV1RAddFrameW( MV1_MODEL_R *ReadModel, const wchar_t *Name, MV1_FRAME_R *Parent )
{
	return MV1RAddFrameBase( ReadModel, NULL, Name, Parent ) ;
}

// ���b�V���̒ǉ�
extern MV1_MESH_R *MV1RAddMesh( MV1_MODEL_R *ReadModel, MV1_FRAME_R *Frame )
{
	MV1_MESH_R *Mesh ;

	// �������̊m��
	Mesh = ( MV1_MESH_R * )AddMemArea( sizeof( MV1_MESH_R ), &ReadModel->Mem ) ;
	if( Mesh == NULL )
		return NULL ;

	// ���X�g�ɒǉ�
	MODELLIST_ADD( ReadModel, Mesh ) ;
	if( Frame )
	{
		OBJLIST_ADD( Frame, Mesh ) ;
		Mesh->Container = Frame ;
	}

	// �I��
	return Mesh ;
}

// ���b�V���̖ʗp�̃o�b�t�@���Z�b�g�A�b�v����
extern int MV1RSetupMeshFaceBuffer( MV1_MODEL_R *ReadModel, MV1_MESH_R *Mesh, int FaceNum, int MaxIndexNum )
{
	MV1_MESHFACE_R *OldFaces ;
	DWORD *OldIndexBuffer ;
	DWORD OldMaxIndexNum ;
	DWORD OldFaceNum ;

	// ���ɐ�������Ă����牽�����Ȃ�
	if( Mesh->FaceIndexBuffer != NULL && ( int )Mesh->FaceUnitMaxIndexNum >= MaxIndexNum &&
		Mesh->Faces != NULL && ( int )Mesh->FaceNum >= FaceNum ) return 0 ;

	OldFaceNum     = Mesh->FaceNum ;
	OldFaces       = Mesh->Faces ;
	OldIndexBuffer = Mesh->FaceIndexBuffer ;
	OldMaxIndexNum = Mesh->FaceUnitMaxIndexNum ;

	// �������̊m��
	Mesh->Faces = ( MV1_MESHFACE_R * )AddMemArea( ( ( MV1_READ_MAX_UV_NUM + 3 ) * MaxIndexNum * sizeof( DWORD ) + sizeof( MV1_MESHFACE_R ) ) * FaceNum, &ReadModel->Mem ) ;
	if( Mesh->Faces == NULL )
		return -1 ;
	Mesh->FaceIndexBuffer = ( DWORD * )( Mesh->Faces + FaceNum ) ;
	Mesh->FaceUnitMaxIndexNum = ( DWORD )MaxIndexNum ;
	Mesh->FaceNum             = ( DWORD )FaceNum ;

	// �e�ʂ̃|�C���^���Z�b�g�A�b�v
	{
		MV1_MESHFACE_R *Face ;
		DWORD *Index ;
		int i, j ;

		Face = Mesh->Faces ;
		Index = Mesh->FaceIndexBuffer ;
		for( i = 0 ; i < ( int )Mesh->FaceNum ; i ++, Face ++ )
		{
			Face->VertexIndex = Index ;			Index += Mesh->FaceUnitMaxIndexNum ;
			Face->NormalIndex = Index ;			Index += Mesh->FaceUnitMaxIndexNum ;
			Face->VertexColorIndex = Index ;	Index += Mesh->FaceUnitMaxIndexNum ;
			for( j = 0 ; j < MV1_READ_MAX_UV_NUM ; j ++ )
			{
				Face->UVIndex[ j ] = Index ;	Index += Mesh->FaceUnitMaxIndexNum ;
			}
		}
	}

	// ���܂ł̃f�[�^������ꍇ�̓R�s�[�Ɖ��
	if( OldFaces )
	{
		MV1_MESHFACE_R *DestFace ;
		MV1_MESHFACE_R *SrcFace ;
		DWORD i, k, l ;

		DestFace = Mesh->Faces ;
		SrcFace = OldFaces ;
		for( i = 0 ; i < OldFaceNum ; i ++, DestFace ++, SrcFace ++ )
		{
			DestFace->IndexNum = SrcFace->IndexNum ;
			DestFace->PolygonNum = SrcFace->PolygonNum ;
			for( k = 0 ; k < OldMaxIndexNum ; k ++ )
			{
				DestFace->VertexIndex[ k ] = SrcFace->VertexIndex[ k ] ;
				DestFace->NormalIndex[ k ] = SrcFace->NormalIndex[ k ] ;
				DestFace->VertexColorIndex[ k ] = SrcFace->VertexColorIndex[ k ] ;
				for( l = 0 ; l < MV1_READ_MAX_UV_NUM ; l ++ )
				{
					DestFace->UVIndex[ l ][ k ] = SrcFace->UVIndex[ l ][ k ] ;
				}
			}
			DestFace->MaterialIndex = SrcFace->MaterialIndex ;
			DestFace->Normal = SrcFace->Normal ;
		}

		SubMemArea( &ReadModel->Mem, OldFaces ) ;
	}

	// �I��
	return 0 ;
}

// �V�F�C�v�f�[�^�̒ǉ�
static MV1_SHAPE_R *MV1RAddShapeBase( MV1_MODEL_R *ReadModel, const char *NameA, const wchar_t *NameW, MV1_FRAME_R *Frame )
{
	MV1_SHAPE_R *Shape ;
	BYTE TempBuffer[ 512 ] ;

#ifndef UNICODE
	if( NameA == NULL )
	{
		ConvString( ( const char * )NameW, WCHAR_T_CHARCODEFORMAT, ( char * )TempBuffer, sizeof( TempBuffer ), ReadModel->CharCodeFormat ) ;
		NameA = ( const char * )TempBuffer ;
	}
	else
#endif
	if( NameW == NULL )
	{
		ConvString( ( const char * )NameA, ReadModel->CharCodeFormat, ( char * )TempBuffer, sizeof( TempBuffer ), WCHAR_T_CHARCODEFORMAT ) ;
		NameW = ( const wchar_t * )TempBuffer ;
	}

	// �������̊m��
	Shape = ( MV1_SHAPE_R * )AddMemArea( sizeof( MV1_SHAPE_R ), &ReadModel->Mem ) ;
	if( Shape == NULL )
		return NULL ;

	// ���O��ۑ�
#ifndef UNICODE
	Shape->NameA = MV1RAddString( ReadModel, NameA ) ;
	if( Shape->NameA == NULL )
	{
		return NULL ;
	}
#endif
	Shape->NameW = MV1RAddStringW( ReadModel, NameW ) ;
	if( Shape->NameW == NULL )
	{
		return NULL ;
	}

	// ���X�g�ɒǉ�
	MODELLIST_ADD( ReadModel, Shape ) ;
	if( Frame )
	{
		OBJLIST_ADD( Frame, Shape ) ;
		Shape->Container = Frame ;
	}

	// �I��
	return Shape ;
}

// �V�F�C�v�f�[�^�̒ǉ�
extern MV1_SHAPE_R *MV1RAddShape( MV1_MODEL_R *ReadModel, const char *Name, MV1_FRAME_R *Frame )
{
	return MV1RAddShapeBase( ReadModel, Name, NULL, Frame ) ;
}

// �V�F�C�v�f�[�^�̒ǉ�
extern MV1_SHAPE_R *MV1RAddShapeW( MV1_MODEL_R *ReadModel, const wchar_t *Name, MV1_FRAME_R *Frame )
{
	return MV1RAddShapeBase( ReadModel, NULL, Name, Frame ) ;
}

// �������Z�p���̃f�[�^�̒ǉ�
static MV1_PHYSICS_RIGIDBODY_R *MV1RAddPhysicsRididBodyBase( MV1_MODEL_R *ReadModel, const char *NameA, const wchar_t *NameW, MV1_FRAME_R *TargetFrame )
{
	MV1_PHYSICS_RIGIDBODY_R *PhysicsRigidBody ;
	BYTE TempBuffer[ 512 ] ;

#ifndef UNICODE
	if( NameA == NULL )
	{
		ConvString( ( const char * )NameW, WCHAR_T_CHARCODEFORMAT, ( char * )TempBuffer, sizeof( TempBuffer ), ReadModel->CharCodeFormat ) ;
		NameA = ( const char * )TempBuffer ;
	}
	else
#endif
	if( NameW == NULL )
	{
		ConvString( ( const char * )NameA, ReadModel->CharCodeFormat, ( char * )TempBuffer, sizeof( TempBuffer ), WCHAR_T_CHARCODEFORMAT ) ;
		NameW = ( const wchar_t * )TempBuffer ;
	}

	// �������̊m��
	PhysicsRigidBody = ( MV1_PHYSICS_RIGIDBODY_R * )AddMemArea( sizeof( MV1_PHYSICS_RIGIDBODY_R ), &ReadModel->Mem ) ;
	if( PhysicsRigidBody == NULL )
		return NULL ;

	// ���O��ۑ�
#ifndef UNICODE
	PhysicsRigidBody->NameA = MV1RAddString( ReadModel, NameA ) ;
	if( PhysicsRigidBody->NameA == NULL )
	{
		return NULL ;
	}
#endif
	PhysicsRigidBody->NameW = MV1RAddStringW( ReadModel, NameW ) ;
	if( PhysicsRigidBody->NameW == NULL )
	{
		return NULL ;
	}

	// ���X�g�ɒǉ�
	MODELLIST_ADD( ReadModel, PhysicsRigidBody ) ;

	// �Ώۃt���[�����Z�b�g
	PhysicsRigidBody->TargetFrame = TargetFrame ;

	// �I��
	return PhysicsRigidBody ;
}

// �������Z�p���̃f�[�^�̒ǉ�
extern MV1_PHYSICS_RIGIDBODY_R *MV1RAddPhysicsRididBody( MV1_MODEL_R *ReadModel, const char *Name, MV1_FRAME_R *TargetFrame )
{
	return MV1RAddPhysicsRididBodyBase( ReadModel, Name, NULL, TargetFrame ) ;
}

// �������Z�p���̃f�[�^�̒ǉ�
extern MV1_PHYSICS_RIGIDBODY_R *MV1RAddPhysicsRididBodyW( MV1_MODEL_R *ReadModel, const wchar_t *Name, MV1_FRAME_R *TargetFrame )
{
	return MV1RAddPhysicsRididBodyBase( ReadModel, NULL, Name, TargetFrame ) ;
}


// �������Z�p���̃W���C���g�f�[�^�̒ǉ�
static MV1_PHYSICS_JOINT_R *MV1RAddPhysicsJointBase( MV1_MODEL_R *ReadModel, const char *NameA, const wchar_t *NameW )
{
	MV1_PHYSICS_JOINT_R *PhysicsJoint ;
	BYTE TempBuffer[ 512 ] ;

#ifndef UNICODE
	if( NameA == NULL )
	{
		ConvString( ( const char * )NameW, WCHAR_T_CHARCODEFORMAT, ( char * )TempBuffer, sizeof( TempBuffer ), ReadModel->CharCodeFormat ) ;
		NameA = ( const char * )TempBuffer ;
	}
	else
#endif
	if( NameW == NULL )
	{
		ConvString( ( const char * )NameA, ReadModel->CharCodeFormat, ( char * )TempBuffer, sizeof( TempBuffer ), WCHAR_T_CHARCODEFORMAT ) ;
		NameW = ( const wchar_t * )TempBuffer ;
	}

	// �������̊m��
	PhysicsJoint = ( MV1_PHYSICS_JOINT_R * )AddMemArea( sizeof( MV1_PHYSICS_JOINT_R ), &ReadModel->Mem ) ;
	if( PhysicsJoint == NULL )
		return NULL ;

	// ���O��ۑ�
#ifndef UNICODE
	PhysicsJoint->NameA = MV1RAddString( ReadModel, NameA ) ;
	if( PhysicsJoint->NameA == NULL )
	{
		return NULL ;
	}
#endif
	PhysicsJoint->NameW = MV1RAddStringW( ReadModel, NameW ) ;
	if( PhysicsJoint->NameW == NULL )
	{
		return NULL ;
	}

	// ���X�g�ɒǉ�
	MODELLIST_ADD( ReadModel, PhysicsJoint ) ;

	// �I��
	return PhysicsJoint ;
}

// �������Z�p���̃W���C���g�f�[�^�̒ǉ�
extern MV1_PHYSICS_JOINT_R *MV1RAddPhysicsJoint( MV1_MODEL_R *ReadModel, const char *Name )
{
	return MV1RAddPhysicsJointBase( ReadModel, Name, NULL ) ;
}

// �������Z�p���̃W���C���g�f�[�^�̒ǉ�
extern MV1_PHYSICS_JOINT_R *MV1RAddPhysicsJointW( MV1_MODEL_R *ReadModel, const wchar_t *Name )
{
	return MV1RAddPhysicsJointBase( ReadModel, NULL, Name ) ;
}

// ���b�V���̍폜
extern void MV1RSubMesh( MV1_MODEL_R *ReadModel, MV1_FRAME_R *Frame, MV1_MESH_R *Mesh )
{
	// ���f���̃��X�g����O��
	if( Mesh->DataPrev )
	{
		Mesh->DataPrev->DataNext = Mesh->DataNext ;
	}
	else
	{
		ReadModel->MeshFirst = Mesh->DataNext ;
	}

	if( Mesh->DataNext )
	{
		Mesh->DataNext->DataPrev = Mesh->DataPrev ;
	}
	else
	{
		ReadModel->MeshLast = Mesh->DataPrev ;
	}
	ReadModel->MeshNum -- ;

	// �t���[���̃��X�g����O��
	if( Mesh->Prev )
	{
		Mesh->Prev->Next = Mesh->Next ;
	}
	else
	{
		Frame->MeshFirst = Mesh->Next ;
	}

	if( Mesh->Next )
	{
		Mesh->Next->Prev = Mesh->Prev ;
	}
	else
	{
		Frame->MeshLast = Mesh->Prev ;
	}
	Frame->MeshNum -- ;
}

// �}�e���A���̒ǉ�
static MV1_MATERIAL_R *MV1RAddMaterialBase( MV1_MODEL_R *ReadModel, const char *NameA, const wchar_t *NameW )
{
	MV1_MATERIAL_R *Material ;
	BYTE TempBuffer[ 512 ] ;

#ifndef UNICODE
	if( NameA == NULL )
	{
		ConvString( ( const char * )NameW, WCHAR_T_CHARCODEFORMAT, ( char * )TempBuffer, sizeof( TempBuffer ), ReadModel->CharCodeFormat ) ;
		NameA = ( const char * )TempBuffer ;
	}
	else
#endif
	if( NameW == NULL )
	{
		ConvString( ( const char * )NameA, ReadModel->CharCodeFormat, ( char * )TempBuffer, sizeof( TempBuffer ), WCHAR_T_CHARCODEFORMAT ) ;
		NameW = ( const wchar_t * )TempBuffer ;
	}

	// �������̊m��
	Material = ( MV1_MATERIAL_R * )AddMemArea( sizeof( MV1_MATERIAL_R ), &ReadModel->Mem ) ;
	if( Material == NULL )
		return NULL ;

	// ���X�g�ɒǉ�
	MODELLIST_ADD( ReadModel, Material ) ;

	// ���O��ۑ�
#ifndef UNICODE
	Material->NameA = MV1RAddString( ReadModel, NameA ) ;
	if( Material->NameA == NULL )
	{
		return NULL ;
	}
#endif
	Material->NameW = MV1RAddStringW( ReadModel, NameW ) ;
	if( Material->NameW == NULL )
	{
		return NULL ;
	}

	// �����l���Z�b�g
	Material->Type = DX_MATERIAL_TYPE_NORMAL ;

	Material->DiffuseGradTextureDefault = -1 ;
	Material->SpecularGradTextureDefault = -1 ;
	Material->DiffuseGradBlendType = DX_MATERIAL_BLENDTYPE_MODULATE ;
	Material->SpecularGradBlendType = DX_MATERIAL_BLENDTYPE_ADDITIVE ;
	Material->SphereMapBlendType = DX_MATERIAL_BLENDTYPE_MODULATE ;

	Material->Diffuse.r = 1.0f ;
	Material->Diffuse.g = 1.0f ;
	Material->Diffuse.b = 1.0f ;
	Material->Diffuse.a = 1.0f ;

	Material->DrawBlendMode = DX_BLENDMODE_ALPHA ;
	Material->DrawBlendParam = 255 ;

	// �I��
	return Material ;
}

// �}�e���A���̒ǉ�
extern MV1_MATERIAL_R *MV1RAddMaterial( MV1_MODEL_R *ReadModel, const char *Name )
{
	return MV1RAddMaterialBase( ReadModel, Name, NULL ) ;
}

// �}�e���A���̒ǉ�
extern MV1_MATERIAL_R *MV1RAddMaterialW( MV1_MODEL_R *ReadModel, const wchar_t *Name )
{
	return MV1RAddMaterialBase( ReadModel, NULL, Name ) ;
}

// �w��C���f�b�N�X�̃}�e���A�����擾����
extern MV1_MATERIAL_R *MV1RGetMaterial( MV1_MODEL_R *ReadModel, int Index )
{
	MV1_MATERIAL_R *Material ;
	int i ;

	Material = ReadModel->MaterialFirst ;
	for( i = 0 ; i < Index ; i ++, Material = Material->DataNext ){}

	return Material ;
}

// �e�N�X�`���̒ǉ�
static MV1_TEXTURE_R *MV1RAddTextureBase(
	MV1_MODEL_R *ReadModel,
	const char *NameA,          const wchar_t *NameW,
	const char *ColorFilePathA, const wchar_t *ColorFilePathW,
	const char *AlphaFilePathA, const wchar_t *AlphaFilePathW,
	int BumpMapFlag, float BumpMapNextPixelLength,
	bool FilePathDoubleCancel,
	bool ReverseFlag,
	bool Bmp32AllZeroAlphaToXRGB8Flag )
{
	MV1_TEXTURE_R *Texture ;
	DWORD_PTR FileHandle, i ;
	wchar_t ColorFileTempPath[ 512 ],      AlphaFileTempPath[ 512 ] ;
	wchar_t ColorFileRelativePath[ 1024 ], AlphaFileRelativePath[ 1024 ] ;
	const wchar_t *ColorOpenFilePathW = NULL, *AlphaOpenFilePathW = NULL ;
	const char    *ColorOpenFilePathA = NULL, *AlphaOpenFilePathA = NULL ;
	wchar_t CurrentDirectory[ 1024 ] ;
	BYTE TempNameBuffer[ 512 ] ;
	BYTE TempColorFilePathBuffer[ 2048 ] ;
	BYTE TempAlphaFilePathBuffer[ 2048 ] ;

#ifndef UNICODE
	if( NameA == NULL )
	{
		ConvString( ( const char * )NameW, WCHAR_T_CHARCODEFORMAT, ( char * )TempNameBuffer, sizeof( TempNameBuffer ), ReadModel->CharCodeFormat ) ;
		NameA = ( const char * )TempNameBuffer ;
	}
	else
#endif
	if( NameW == NULL )
	{
		ConvString( ( const char * )NameA, ReadModel->CharCodeFormat, ( char * )TempNameBuffer, sizeof( TempNameBuffer ), WCHAR_T_CHARCODEFORMAT ) ;
		NameW = ( const wchar_t * )TempNameBuffer ;
	}

	if( ColorFilePathA != NULL || ColorFilePathW != NULL )
	{
#ifndef UNICODE
		if( ColorFilePathA == NULL )
		{
			ConvString( ( const char * )ColorFilePathW, WCHAR_T_CHARCODEFORMAT, ( char * )TempColorFilePathBuffer, sizeof( TempColorFilePathBuffer ), ReadModel->CharCodeFormat ) ;
			ColorFilePathA = ( const char * )TempColorFilePathBuffer ;
		}
		else
#endif
		if( ColorFilePathW == NULL )
		{
			ConvString( ( const char * )ColorFilePathA, ReadModel->CharCodeFormat, ( char * )TempColorFilePathBuffer, sizeof( TempColorFilePathBuffer ), WCHAR_T_CHARCODEFORMAT ) ;
			ColorFilePathW = ( const wchar_t * )TempColorFilePathBuffer ;
		}
	}

	if( AlphaFilePathA != NULL || AlphaFilePathW != NULL )
	{
#ifndef UNICODE
		if( AlphaFilePathA == NULL )
		{
			ConvString( ( const char * )AlphaFilePathW, WCHAR_T_CHARCODEFORMAT, ( char * )TempAlphaFilePathBuffer, sizeof( TempAlphaFilePathBuffer ), ReadModel->CharCodeFormat ) ;
			AlphaFilePathA = ( const char * )TempAlphaFilePathBuffer ;
		}
		else
#endif
		if( AlphaFilePathW == NULL )
		{
			ConvString( ( const char * )AlphaFilePathA, ReadModel->CharCodeFormat, ( char * )TempAlphaFilePathBuffer, sizeof( TempAlphaFilePathBuffer ), WCHAR_T_CHARCODEFORMAT ) ;
			AlphaFilePathW = ( const wchar_t * )TempAlphaFilePathBuffer ;
		}
	}

	// ���΃p�X�����߂�
	_WGETCWD( CurrentDirectory, sizeof( CurrentDirectory ) ) ;

	// �J���[�`�����l���t�@�C���p�X��ۑ�
	if( ColorFilePathW )
	{
		CreateRelativePathW_( ColorFilePathW, CurrentDirectory, ColorFileRelativePath, sizeof( ColorFileRelativePath ) ) ;

		// �t�@�C�����J���邩���ׂ�
		FileHandle = DX_FOPEN( ColorFileRelativePath ) ;
		if( FileHandle )
		{
			DX_FCLOSE( FileHandle ) ;
			ColorOpenFilePathW = ColorFileRelativePath ;
		}
		else
		{
			// �J���Ȃ�������J�����g�t�H���_����J�����Ƃ��Ă݂�
			AnalysisFileNameAndDirPathW_( ColorFileRelativePath, ColorFileTempPath, sizeof( ColorFileTempPath ), NULL, 0 ) ;
			FileHandle = DX_FOPEN( ColorFileTempPath ) ;
			if( FileHandle )
			{
				DX_FCLOSE( FileHandle ) ;
				ColorOpenFilePathW = ColorFileTempPath ;
			}
			else
			{
				ColorOpenFilePathW = ColorFileRelativePath ;
			}
		}
	}

	// �A���t�@�`�����l���p�t�@�C���p�X��ۑ�
	if( AlphaFilePathW )
	{
		CreateRelativePathW_( AlphaFilePathW, CurrentDirectory, AlphaFileRelativePath, sizeof( AlphaFileRelativePath ) ) ;

		// �t�@�C�����J���邩���ׂ�
		FileHandle = DX_FOPEN( AlphaFileRelativePath ) ;
		if( FileHandle )
		{
			DX_FCLOSE( FileHandle ) ;
			AlphaOpenFilePathW = AlphaFileRelativePath ;
		}
		else
		{
			// �J���Ȃ�������J�����g�t�H���_����J�����Ƃ��Ă݂�
			AnalysisFileNameAndDirPathW_( AlphaFileRelativePath, AlphaFileTempPath, sizeof( AlphaFileTempPath ), NULL, 0 ) ;
			FileHandle = DX_FOPEN( AlphaFileTempPath ) ;
			if( FileHandle )
			{
				DX_FCLOSE( FileHandle ) ;
				AlphaOpenFilePathW = AlphaFileTempPath ;
			}
			else
			{
				AlphaOpenFilePathW = AlphaFileRelativePath ;
			}
		}
	}

	// �t�@�C���p�X�������e�N�X�`������������L�����Z������t���O�������Ă�����
	// �t�@�C���p�X�������e�N�X�`�������邩���ׂ�
	if( FilePathDoubleCancel )
	{
		Texture = ReadModel->TextureFirst ;
		for( i = 0 ; ( DWORD )i < ReadModel->TextureNum ; i ++, Texture = Texture->DataNext )
		{
			if( ColorFilePathW )
			{
				if( Texture->ColorFileNameW == NULL ) continue ;
				if( _WCSCMP( Texture->ColorFileNameW, ColorOpenFilePathW ) != 0 ) continue ;
			}
			else
			{
				if( Texture->ColorFileNameW != NULL ) continue ;
			}

			if( AlphaFilePathW )
			{
				if( Texture->AlphaFileNameW == NULL ) continue ;
				if( _WCSCMP( Texture->AlphaFileNameW, AlphaOpenFilePathW ) != 0 ) continue ;
			}
			else
			{
				if( Texture->AlphaFileNameW != NULL ) continue ;
			}
			break ;
		}
		if( ( DWORD )i != ReadModel->TextureNum ) return Texture ;
	}

	// �������̊m��
	Texture = ( MV1_TEXTURE_R * )AddMemArea( sizeof( MV1_TEXTURE_R ), &ReadModel->Mem ) ;
	if( Texture == NULL )
	{
		return NULL ;
	}

	// ���X�g�ɒǉ�
	MODELLIST_ADD( ReadModel, Texture ) ;

	// ���O��ۑ�
#ifndef UNICODE
	Texture->NameA = MV1RAddString( ReadModel, NameA ) ;
	if( Texture->NameA == NULL )
	{
		return NULL ;
	}
#endif
	Texture->NameW = MV1RAddStringW( ReadModel, NameW ) ;
	if( Texture->NameW == NULL )
	{
		return NULL ;
	}

	// �f�t�H���g�̃A�h���X���[�h���Z�b�g����
	Texture->AddressModeU = DX_TEXADDRESS_WRAP ;
	Texture->AddressModeV = DX_TEXADDRESS_WRAP ;

	// �t�u���W�̃f�t�H���g�̃X�P�[���l���Z�b�g����
	Texture->ScaleU = 1.0f ;
	Texture->ScaleV = 1.0f ;
	
	// �f�t�H���g�̃t�B���^�����O���[�h���Z�b�g����
	Texture->FilterMode = DX_DRAWMODE_ANISOTROPIC ;

	// �J���[�`�����l�����o���v�}�b�v���ǂ����̃t���O��ۑ�����
	Texture->BumpMapFlag = BumpMapFlag ;

	// �J���[�`�����l�����o���v�}�b�v�̏ꍇ�A�ׂ̃s�N�Z���Ƃ̋�����ۑ�����
	Texture->BumpMapNextPixelLength = BumpMapNextPixelLength ;

	// ���]�t���O��ۑ�����
	Texture->ReverseFlag = ReverseFlag ? 1 : 0 ;

	// �R�Q�r�b�g�a�l�o�̃A���t�@�l���S���O�������� XRGB8 �Ƃ��Ĉ������ǂ����̃t���O���Z�b�g����
	Texture->Bmp32AllZeroAlphaToXRGB8Flag = Bmp32AllZeroAlphaToXRGB8Flag ? 1 : 0 ;

	// �J���[�`�����l���t�@�C���p�X��ۑ�
	if( ColorFilePathW )
	{
#ifndef UNICODE
		ConvString( ( const char * )ColorOpenFilePathW, WCHAR_T_CHARCODEFORMAT, ( char * )TempColorFilePathBuffer, sizeof( TempColorFilePathBuffer ), ReadModel->CharCodeFormat ) ;
		ColorOpenFilePathA = ( const char * )TempColorFilePathBuffer ;

		Texture->ColorFileNameA = MV1RAddString( ReadModel, ColorOpenFilePathA ) ;
		if( Texture->ColorFileNameA == NULL )
		{
			return NULL ;
		}
#endif

		Texture->ColorFileNameW = MV1RAddStringW( ReadModel, ColorOpenFilePathW ) ;
		if( Texture->ColorFileNameW == NULL )
		{
			return NULL ;
		}
	}

	// �A���t�@�`�����l���p�t�@�C���p�X��ۑ�
	if( AlphaFilePathW )
	{
#ifndef UNICODE
		ConvString( ( const char * )AlphaOpenFilePathW, WCHAR_T_CHARCODEFORMAT, ( char * )TempAlphaFilePathBuffer, sizeof( TempAlphaFilePathBuffer ), ReadModel->CharCodeFormat ) ;
		AlphaOpenFilePathA = ( const char * )TempAlphaFilePathBuffer ;

		Texture->AlphaFileNameA = MV1RAddString( ReadModel, AlphaOpenFilePathA ) ;
		if( Texture->AlphaFileNameA == NULL ) 
		{
			return NULL ;
		}
#endif

		Texture->AlphaFileNameW = MV1RAddStringW( ReadModel, AlphaOpenFilePathW ) ;
		if( Texture->AlphaFileNameW == NULL ) 
		{
			return NULL ;
		}
	}

	// �I��
	return Texture ;
}

// �e�N�X�`���̒ǉ�
extern MV1_TEXTURE_R *MV1RAddTexture(
	MV1_MODEL_R *ReadModel,
	const char *Name,
	const char *ColorFilePath,
	const char *AlphaFilePath,
	int BumpMapFlag, float BumpMapNextPixelLength,
	bool FilePathDoubleCancel,
	bool ReverseFlag,
	bool Bmp32AllZeroAlphaToXRGB8Flag )
{
	return MV1RAddTextureBase( ReadModel, Name, NULL, ColorFilePath, NULL, AlphaFilePath, NULL, BumpMapFlag, BumpMapNextPixelLength, FilePathDoubleCancel, ReverseFlag, Bmp32AllZeroAlphaToXRGB8Flag ) ;
}

// �e�N�X�`���̒ǉ�
extern MV1_TEXTURE_R *MV1RAddTextureW(
	MV1_MODEL_R *ReadModel,
	const wchar_t *Name,
	const wchar_t *ColorFilePath,
	const wchar_t *AlphaFilePath,
	int BumpMapFlag, float BumpMapNextPixelLength,
	bool FilePathDoubleCancel,
	bool ReverseFlag,
	bool Bmp32AllZeroAlphaToXRGB8Flag )
{
	return MV1RAddTextureBase( ReadModel, NULL, Name, NULL, ColorFilePath, NULL, AlphaFilePath, BumpMapFlag, BumpMapNextPixelLength, FilePathDoubleCancel, ReverseFlag, Bmp32AllZeroAlphaToXRGB8Flag ) ;
}

// �X�L���E�G�C�g���̒ǉ�
extern MV1_SKIN_WEIGHT_R *MV1RAddSkinWeight( MV1_MODEL_R *ReadModel )
{
	MV1_SKIN_WEIGHT_R *SkinWeight ;

	// �������̊m��
	SkinWeight = ( MV1_SKIN_WEIGHT_R * )AddMemArea( sizeof( MV1_SKIN_WEIGHT_R ), &ReadModel->Mem ) ;
	if( SkinWeight == NULL )
		return NULL ;

	// �X�L���E�G�C�g���̐����C���N�������g
	ReadModel->SkinWeightNum ++ ;

	// �I��
	return SkinWeight ;
}

// �A�j���[�V�����Z�b�g�̒ǉ�
static MV1_ANIMSET_R *MV1RAddAnimSetBase( MV1_MODEL_R *ReadModel, const char *NameA, const wchar_t *NameW )
{
	MV1_ANIMSET_R *AnimSet ;
	BYTE TempBuffer[ 512 ] ;

#ifndef UNICODE
	if( NameA == NULL )
	{
		ConvString( ( const char * )NameW, WCHAR_T_CHARCODEFORMAT, ( char * )TempBuffer, sizeof( TempBuffer ), ReadModel->CharCodeFormat ) ;
		NameA = ( const char * )TempBuffer ;
	}
	else
#endif
	if( NameW == NULL )
	{
		ConvString( ( const char * )NameA, ReadModel->CharCodeFormat, ( char * )TempBuffer, sizeof( TempBuffer ), WCHAR_T_CHARCODEFORMAT ) ;
		NameW = ( const wchar_t * )TempBuffer ;
	}

	// �������̊m��
	AnimSet = ( MV1_ANIMSET_R * )AddMemArea( sizeof( MV1_ANIMSET_R ), &ReadModel->Mem ) ;
	if( AnimSet == NULL )
		return NULL ;

	// ���X�g�ɒǉ�
	MODELLIST_ADD( ReadModel, AnimSet ) ;

	// ���O��ۑ�
#ifndef UNICODE
	AnimSet->NameA = MV1RAddString( ReadModel, NameA ) ;
	if( AnimSet->NameA == NULL )
	{
		return NULL ;
	}
#endif

	AnimSet->NameW = MV1RAddStringW( ReadModel, NameW ) ;
	if( AnimSet->NameW == NULL )
	{
		return NULL ;
	}

	// �I��
	return AnimSet ;
}

// �A�j���[�V�����Z�b�g�̒ǉ�
extern MV1_ANIMSET_R *MV1RAddAnimSet( MV1_MODEL_R *ReadModel, const char *Name )
{
	return MV1RAddAnimSetBase( ReadModel, Name, NULL ) ;
}

// �A�j���[�V�����Z�b�g�̒ǉ�
extern MV1_ANIMSET_R *MV1RAddAnimSetW( MV1_MODEL_R *ReadModel, const wchar_t *Name )
{
	return MV1RAddAnimSetBase( ReadModel, NULL, Name ) ;
}

// �A�j���[�V�����̒ǉ�
extern MV1_ANIM_R *MV1RAddAnim( MV1_MODEL_R *ReadModel, MV1_ANIMSET_R *AnimSet )
{
	MV1_ANIM_R *Anim ;

	// �������̊m��
	Anim = ( MV1_ANIM_R * )AddMemArea( sizeof( MV1_ANIM_R ), &ReadModel->Mem ) ;
	if( Anim == NULL )
		return NULL ;

	// ���X�g�ɒǉ�
	MODELLIST_ADD( ReadModel, Anim ) ;
	OBJLIST_ADD( AnimSet, Anim ) ;
	Anim->Container = AnimSet ;

	// �I��
	return Anim ;
}

// �A�j���[�V�����L�[�Z�b�g�̒ǉ�
extern MV1_ANIMKEYSET_R *MV1RAddAnimKeySet( MV1_MODEL_R *ReadModel, MV1_ANIM_R *Anim )
{
	MV1_ANIMKEYSET_R *AnimKeySet ;

	// �������̊m��
	AnimKeySet = ( MV1_ANIMKEYSET_R * )AddMemArea( sizeof( MV1_ANIMKEYSET_R ), &ReadModel->Mem ) ;
	if( AnimKeySet == NULL )
		return NULL ;

	// ���X�g�ɒǉ�
	MODELLIST_ADD( ReadModel, AnimKeySet ) ;
	OBJLIST_ADD( Anim, AnimKeySet ) ;

	// �p�����[�^�̏�����
	AnimKeySet->TimeType = MV1_ANIMKEY_TIME_TYPE_KEY ;

	// �I��
	return AnimKeySet ;
}

// ���C�g�̒ǉ�
static MV1_LIGHT_R *MV1RAddLightBase( MV1_MODEL_R *ReadModel, const char *NameA, const wchar_t *NameW )
{
	MV1_LIGHT_R *Light ;
	BYTE TempBuffer[ 512 ] ;

#ifndef UNICODE
	if( NameA == NULL )
	{
		ConvString( ( const char * )NameW, WCHAR_T_CHARCODEFORMAT, ( char * )TempBuffer, sizeof( TempBuffer ), ReadModel->CharCodeFormat ) ;
		NameA = ( const char * )TempBuffer ;
	}
	else
#endif
	if( NameW == NULL )
	{
		ConvString( ( const char * )NameA, ReadModel->CharCodeFormat, ( char * )TempBuffer, sizeof( TempBuffer ), WCHAR_T_CHARCODEFORMAT ) ;
		NameW = ( const wchar_t * )TempBuffer ;
	}

	// �������̊m��
	Light = ( MV1_LIGHT_R * )AddMemArea( sizeof( MV1_LIGHT_R ), &ReadModel->Mem ) ;
	if( Light == NULL )
		return NULL ;

	// ���X�g�ɒǉ�
	MODELLIST_ADD( ReadModel, Light ) ;

	// ���O��ۑ�
#ifndef UNICODE
	Light->NameA = MV1RAddString( ReadModel, NameA ) ;
	if( Light->NameA == NULL )
	{
		return NULL ;
	}
#endif

	Light->NameW = MV1RAddStringW( ReadModel, NameW ) ;
	if( Light->NameW == NULL )
	{
		return NULL ;
	}

	// �I��
	return Light ;
}

// ���C�g�̒ǉ�
extern MV1_LIGHT_R *MV1RAddLight( MV1_MODEL_R *ReadModel, const char *Name )
{
	return MV1RAddLightBase( ReadModel, Name, NULL ) ;
}

// ���C�g�̒ǉ�
extern MV1_LIGHT_R *MV1RAddLightW( MV1_MODEL_R *ReadModel, const wchar_t *Name )
{
	return MV1RAddLightBase( ReadModel, NULL, Name ) ;
}

// �ǂݍ��ݏ����p���f�������{���f���f�[�^���쐬����
extern int MV1LoadModelToReadModel(
	const MV1LOADMODEL_GPARAM *	GParam,
	      MV1_MODEL_R *			ReadModel,
	const wchar_t *				CurrentDir,
	const MV1_FILE_READ_FUNC *	ReadFunc,
	      int					ASyncThread
)
{
	MV1_MESH_R					*Mesh ;
	MV1_SKIN_WEIGHT_R			*SkinWeight ;
	MV1_SKIN_WEIGHT_ONE_R		*SkinWeightOne ;
	MV1_MATERIAL_POLY_R			*MPoly ;
	MV1_MESHFACE_R				*MeshFace ;
	MV1_TRIANGLELIST_R			*TList ;
	MV1_MATERIAL_R				*Material ;
	MV1_TEXTURE_R				*Texture ;
	MV1_TEXTURE_R				**Textures = NULL ;
	MV1_ANIMSET_R				*AnimSet ;
	MV1_ANIM_R					*Anim ;
	MV1_ANIMKEYSET_R			*AnimKeySet ;
	MV1_FRAME_R					*Frame ;
	MV1_SHAPE_R					*Shape ;
	MV1_SHAPE_VERTEX_R			*ShapeVertex ;
	MV1_PHYSICS_RIGIDBODY_R		*PhysicsRigidBody ;
	MV1_PHYSICS_JOINT_R			*PhysicsJoint ;
	MV1_MAKEVERTINDEXINFO		**VertInfoTable, *VInfo ;
	MV1_MAKEVERTINDEXINFO		*VertInfoBuffer ;
	int							*VertValidBuffer ;
	MV1_SKIN_BONE_USE_FRAME		*MBSkinWF ;
	int							VertInfoNum ;
//	int							PosIndexList[ MV1_READ_MAX_FACEVERTEX_NUM ] ;
	DWORD						MaxPositionNum ;
	DWORD						MaxTriangleNum ;
	DWORD						MeshStartNum, MeshNowNum ;
	DWORD						MeshPositionStartNum, MeshNormalStartNum, MeshBoneStartNum ;

	wchar_t						DirectoryPath[ FILEPATH_MAX + 2 ] ;
//	char						DirectoryPathA[ FILEPATH_MAX + 2 ] ;
//	wchar_t						DirectoryPathW[ FILEPATH_MAX + 2 ] ;

	MV1_FRAME_BASE				*MBFrame ;
	MV1_SKIN_BONE				*MBSkinW ;
	MV1_MESH_BASE				*MBMesh ;
	MV1_MESH_FACE				*MBFace ;
//	MV1_MESH_VERTEX				*MBMVert ;
//	MV1_MESH_POSITION			*MBMPos ;
//	MV1_MESH_NORMAL				*MBMNorm ;
//	MV1_TLIST_NORMAL_POS		/**NormTVP[  MV1_READ_MAX_FACEVERTEX_NUM ],*/ *NormV ;
//	MV1_TLIST_SKIN_POS_4B		/**Skin4TVP[ MV1_READ_MAX_FACEVERTEX_NUM ],*/ *Skin4BV ;
//	MV1_TLIST_SKIN_POS_8B		/**Skin8TVP[ MV1_READ_MAX_FACEVERTEX_NUM ],*/ *Skin8BV ;
//	MV1_TLIST_SKIN_POS_FREEB	/**SkinFVP[  MV1_READ_MAX_FACEVERTEX_NUM ],*/ *SkinFBV ;
	MV1_SKINBONE_BLEND			*VertB ;
	MV1_SHAPE_BASE				*MBShape ;
	MV1_SHAPE_MESH_BASE			*MBShapeMesh ;
	MV1_SHAPE_VERTEX_BASE		*MBShapeVertex ;
	MV1_PHYSICS_RIGIDBODY_BASE	*MBPhysicsRigidBody ;
	MV1_PHYSICS_JOINT_BASE		*MBPhysicsJoint ;
	MV1_TEXTURE_BASE			*MBTexture ;
	MV1_MATERIAL_BASE			*MBMaterial ;
	MV1_MATERIAL_LAYER			*MBMaterialLayer = NULL ;
	MV1_LIGHT					*MBLight ;
	MV1_TRIANGLE_LIST_BASE		*MBTList ;
	MV1_ANIMSET_BASE			*MBAnimSet ;
	MV1_ANIM_BASE				*MBAnim ;
	MV1_ANIM_KEYSET_BASE		*MBKeySet ;

	int						dirlen ;
	int						FrameMeshCounter ;
	WORD					BitBuf[ 8192 ] ;
//	BYTE					VertBuffer[ 2 ][ 32 * 1024 ] ;
	COLOR_U8				OneColor ;
	COLOR_F					*ColorF ;
	DWORD					i ;
	int j, k, l, m, o, p, r, s, BoneNum, Size, Con, MaxCon, MaxFaceCon, NewHandle = -1, num, StartMatrixIndex ;
	MV1_MODEL_BASE MTBase, *MBase ;
	DWORD_PTR AllocSize ;

	// �f�B���N�g���p�X���擾
	dirlen = 0 ;
	if( CurrentDir != NULL )
	{
		_WCSCPY_S( DirectoryPath, sizeof( DirectoryPath ), CurrentDir ) ;
		dirlen = ( int )_WCSLEN( DirectoryPath ) ;
		if( DirectoryPath[ dirlen - 1 ] != L'\\' && DirectoryPath[ dirlen - 1 ] != L'/' )
		{
			_WCSCAT_S( DirectoryPath, sizeof( DirectoryPath ), L"/" ) ;
			dirlen = ( int )_WCSLEN( DirectoryPath ) ;
		}
	}

	// ���_�̈ꎞ�o�b�t�@��������
//	_MEMSET( VertBuffer, 0, sizeof( VertBuffer ) ) ;

	// ���꒸�_���o�p���̃|�C���^��������
	VertInfoTable = NULL ;

	// �A�j���[�V�����f�[�^�̍œK��
	MV1OptimizeAnim( ReadModel ) ;

	// �V�F�C�v�̐������J��Ԃ��V�F�C�v�̑ΏۂɂȂ��Ă��郁�b�V���Ɉ��t����
	for( Shape = ReadModel->ShapeFirst ; Shape ; Shape = Shape->DataNext )
	{
		Shape->TargetMesh->IsShapeMesh = TRUE ;

		// ���łɃe�[�u�������
		Shape->NextTable = ( DWORD * )ADDMEMAREA( sizeof( DWORD ) * Shape->TargetMesh->PositionNum, &ReadModel->Mem ) ;
		if( Shape->NextTable == NULL )
		{
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\x52\x00\x65\x00\x61\x00\x64\x00\x20\x00\x4d\x00\x6f\x00\x64\x00\x65\x00\x6c\x00\x20\x00\x43\x00\x6f\x00\x6e\x00\x76\x00\x65\x00\x72\x00\x74\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xb7\x30\xa7\x30\xa4\x30\xd7\x30\xfe\x5b\x61\x8c\x02\x98\xb9\x70\xc6\x30\xfc\x30\xd6\x30\xeb\x30\x28\x75\xe1\x30\xe2\x30\xea\x30\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Read Model Convert Error : �V�F�C�v�Ώے��_�e�[�u���p�������̊m�ۂɎ��s���܂���\n" @*/ )) ;
			return -1 ;
		}
		_MEMSET( Shape->NextTable, 0xff, sizeof( DWORD ) * Shape->TargetMesh->PositionNum ) ;

		for( i = 0 ; i < ( DWORD )Shape->VertexNum ; i ++ )
		{
			Shape->NextTable[ Shape->Vertex[ i ].TargetPositionIndex ] = ( DWORD )i ;
		}
	}

	// �}�e���A��������Ȃ��ꍇ�͉��}�e���A����ǉ�����
	if( ReadModel->MaterialNum == 0 )
	{
		MV1_MATERIAL_R *Material ;

		// �}�e���A�������蓖�Ă��Ă��Ȃ����b�V���p�̃}�e���A����ǉ�
		Material = MV1RAddMaterial( ReadModel, "NoMaterial" ) ;
		if( Material == NULL )
		{
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\x52\x00\x65\x00\x61\x00\x64\x00\x20\x00\x4d\x00\x6f\x00\x64\x00\x65\x00\x6c\x00\x20\x00\x43\x00\x6f\x00\x6e\x00\x76\x00\x65\x00\x72\x00\x74\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\x4d\x00\x61\x00\x74\x00\x65\x00\x72\x00\x69\x00\x61\x00\x6c\x00\x20\x00\xaa\x30\xd6\x30\xb8\x30\xa7\x30\xaf\x30\xc8\x30\x6e\x30\xfd\x8f\xa0\x52\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Read Model Convert Error : Material �I�u�W�F�N�g�̒ǉ��Ɏ��s���܂���\n" @*/, i ) ) ;
			return -1 ;
		}

		// ���b�V�����W�f�[�^�̃T�C�Y�����Z
		for( Mesh = ReadModel->MeshFirst ; Mesh ; Mesh = Mesh->DataNext )
		{
			Mesh->MaterialNum = 1 ;
			Mesh->Materials[ 0 ] = Material ;
		}
	}

	// ���b�V���̐������J��Ԃ�
	ReadModel->TriangleNum = 0 ;
	MaxPositionNum = 0 ;
	MaxTriangleNum = 0 ;
	for( Mesh = ReadModel->MeshFirst ; Mesh ; Mesh = Mesh->DataNext )
	{
		// �|���S���̎O�p�`��
		if( MV1ConvertTrianglePolygon( ReadModel, Mesh ) == false )
			return -1 ;

		// �w�肪����ꍇ�͍��W�̍œK��
		if( GParam->LoadModelToPositionOptimize )
		{
			if( MV1OptimizePosition( ReadModel, Mesh ) == false )
				return -1 ;
		}

		// �@�����������w�肪����ꍇ�͖@���̍Čv�Z
		if( Mesh->Normals == NULL || ReadModel->AutoCreateNormal || GParam->LoadModelToReMakeNormal )
		{
			if( ReadModel->AutoCreateNormal == FALSE ) 
			{
				Mesh->Container->SmoothingAngle = GParam->LoadModelToReMakeNormalSmoothingAngle ;
			}

			if( MV1MakeMeshNormals( ReadModel, Mesh ) == false )
				return -1 ;
		}

		// �]�@���Ɛڐ����Z�o
//		if( Mesh->UVNum[ 0 ] != 0 )
//		{
//			MV1MakeMeshBinormalsAndTangents( ReadModel, Mesh ) ;
//		}

		// �w�肪����ꍇ�́A�ӂ����L���Ă��āA���@���̕������قȂ�ӂɖʐςO�̃|���S����ǉ�����( �R�p�`�|���S���̏W���ɂȂ��Ă���K�v���� )
		if( GParam->LoadModelToNotEqNormalSideAddZeroAreaPolygon )
		{
			if( MV1NotEqualNormalSideAddZeroAreaPolygon( ReadModel, Mesh ) == false )
				return -1 ;
		}

		// �|���S���̐����Z�o
		Mesh->TriangleNum = 0 ;
		MeshFace = Mesh->Faces ;
		for( i = 0 ; i < ( int )Mesh->FaceNum ; i ++, MeshFace ++ )
		{
			MeshFace->PolygonNum = MeshFace->IndexNum - 2 ;
			Mesh->TriangleNum += MeshFace->PolygonNum ;
		}
		ReadModel->TriangleNum += Mesh->TriangleNum ;

		// �|�W�V�����̍ő吔�̍X�V
		if( MaxPositionNum < Mesh->PositionNum )
			MaxPositionNum = Mesh->PositionNum ;

		// �g���C�A���O���̍ő吔�̍X�V
		if( MaxTriangleNum < Mesh->TriangleNum )
			MaxTriangleNum = Mesh->TriangleNum ;

		// �X�L�����b�V���̏����擾
		if( Mesh->SkinWeightsNum )
		{
			// �g���C�A���O���^�C�v�z��i�[�p�̃��������m��
			Mesh->SkinFaceBoneNum = ( WORD * )ADDMEMAREA( ( sizeof( BYTE ) + sizeof( WORD ) ) * Mesh->FaceNum, &ReadModel->Mem ) ;
			Mesh->SkinFaceType = ( BYTE * )( Mesh->SkinFaceBoneNum + Mesh->FaceNum ) ;
			if( Mesh->SkinFaceBoneNum == NULL )
			{
				DXST_ERRORLOGFMT_ADDUTF16LE(( "\x52\x00\x65\x00\x61\x00\x64\x00\x20\x00\x4d\x00\x6f\x00\x64\x00\x65\x00\x6c\x00\x20\x00\x43\x00\x6f\x00\x6e\x00\x76\x00\x65\x00\x72\x00\x74\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xc8\x30\xe9\x30\xa4\x30\xa2\x30\xf3\x30\xb0\x30\xeb\x30\xbf\x30\xa4\x30\xd7\x30\x4d\x91\x17\x52\x3c\x68\x0d\x7d\x28\x75\xe1\x30\xe2\x30\xea\x30\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Read Model Convert Error : �g���C�A���O���^�C�v�z��i�[�p�������̊m�ۂɎ��s���܂���\n" @*/ )) ;
				return -1 ;
			}
			_MEMSET( Mesh->SkinFaceBoneNum, 0, ( sizeof( BYTE ) + sizeof( WORD ) ) * Mesh->FaceNum ) ;

			// �e���_���ǂ̃{�[���̉e�����󂯂邩�̏��̏�����
			if( InitBitList( &Mesh->SkinVerticesBlend, ( int )Mesh->SkinWeightsNum, ( int )Mesh->PositionNum, &ReadModel->Mem ) == -1 )
			{
				DXST_ERRORLOGFMT_ADDUTF16LE(( "\x52\x00\x65\x00\x61\x00\x64\x00\x20\x00\x4d\x00\x6f\x00\x64\x00\x65\x00\x6c\x00\x20\x00\x43\x00\x6f\x00\x6e\x00\x76\x00\x65\x00\x72\x00\x74\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\x02\x98\xb9\x70\x78\x30\x6e\x30\xdc\x30\xfc\x30\xf3\x30\x6e\x30\x71\x5f\xff\x97\x6b\x30\xa2\x95\x59\x30\x8b\x30\xc7\x30\xfc\x30\xbf\x30\x6e\x30\x1d\x52\x1f\x67\x16\x53\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Read Model Convert Error : ���_�ւ̃{�[���̉e���Ɋւ���f�[�^�̏������Ɏ��s���܂���\n" @*/ )) ;
				return -1 ;
			}

			// �e���_�̃{�[���̉e���l��ۑ����郁�����̊m��
			Mesh->SkinVerticeWeightInfo = ( float * )ADDMEMAREA( sizeof( float ) * Mesh->SkinWeightsNum * Mesh->PositionNum, &ReadModel->Mem ) ;
			if( Mesh->SkinVerticeWeightInfo == NULL )
			{
				DXST_ERRORLOGFMT_ADDUTF16LE(( "\x52\x00\x65\x00\x61\x00\x64\x00\x20\x00\x4d\x00\x6f\x00\x64\x00\x65\x00\x6c\x00\x20\x00\x43\x00\x6f\x00\x6e\x00\x76\x00\x65\x00\x72\x00\x74\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\x02\x98\xb9\x70\x78\x30\x6e\x30\xdc\x30\xfc\x30\xf3\x30\x6e\x30\x71\x5f\xff\x97\x24\x50\x6e\x30\xc5\x60\x31\x58\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Read Model Convert Error : ���_�ւ̃{�[���̉e���l�̏����i�[���郁�����̊m�ۂɎ��s���܂���\n" @*/ )) ;
				return -1 ;
			}

			// �e�ʂ��ǂ̃{�[���̉e�����󂯂�̂��̏��̏�����
			if( InitBitList( &Mesh->SkinFacesBlend, ( int )Mesh->SkinWeightsNum, ( int )Mesh->FaceNum, &ReadModel->Mem ) == -1 )
			{
				DXST_ERRORLOGFMT_ADDUTF16LE(( "\x52\x00\x65\x00\x61\x00\x64\x00\x20\x00\x4d\x00\x6f\x00\x64\x00\x65\x00\x6c\x00\x20\x00\x43\x00\x6f\x00\x6e\x00\x76\x00\x65\x00\x72\x00\x74\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\x62\x97\x78\x30\x6e\x30\xdc\x30\xfc\x30\xf3\x30\x6e\x30\x71\x5f\xff\x97\x6b\x30\xa2\x95\x59\x30\x8b\x30\xc7\x30\xfc\x30\xbf\x30\x6e\x30\x1d\x52\x1f\x67\x16\x53\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Read Model Convert Error : �ʂւ̃{�[���̉e���Ɋւ���f�[�^�̏������Ɏ��s���܂���\n" @*/ )) ;
				return -1 ;
			}

			// �X�L���E�G�C�g�̊e���_�ւ̔��f���X�g���쐬
			for( i = 0 ; i < ( int )Mesh->SkinWeightsNum ; i ++ )
			{
				SkinWeight = Mesh->SkinWeights[ i ] ;

				// �Ώے��_���̐������J��Ԃ�
				SkinWeightOne = SkinWeight->Data ;
				for( j = 0 ; j < ( int )SkinWeight->DataNum ; j ++, SkinWeightOne ++ )
				{
					// �E�G�C�g�l���O�̏ꍇ�͉������Ȃ�
					if( SkinWeightOne->Weight <= 0.0f || ( *( ( DWORD * )&SkinWeightOne->Weight ) & 0x7fffffff ) == 0 ) continue ;
					
					// �e�����Ă��钸�_�ɏ��𖄂ߍ���
					SetBitList( &Mesh->SkinVerticesBlend, ( int )SkinWeightOne->TargetVertex, ( int )i ) ;
					Mesh->SkinVerticeWeightInfo[ SkinWeightOne->TargetVertex * Mesh->SkinWeightsNum + i ] = SkinWeightOne->Weight ;
				}
			}

			// �t�F�C�X�̃^�C�v���Z�b�g
			MeshFace = Mesh->Faces ;
			for( i = 0 ; ( DWORD )i < Mesh->FaceNum ; i ++, MeshFace ++ )
			{
				// �|���S�����g�p���Ă���{�[���̏����\�z
				GetBitList( &Mesh->SkinVerticesBlend, ( int )MeshFace->VertexIndex[ 0 ], BitBuf ) ; 
				for( j = 1 ; ( DWORD )j < MeshFace->IndexNum ; j ++ )
					OrBitList( &Mesh->SkinVerticesBlend, ( int )MeshFace->VertexIndex[ j ], BitBuf ) ;

				// �|���S�����g�p���Ă���{�[���̑g�ݍ��킹����ۑ�
				CopyBitList( &Mesh->SkinFacesBlend, ( int )i, BitBuf ) ;

				// �|���S�����g�p���Ă���{�[���̐����擾
				MaxFaceCon = GetBitCount( BitBuf, Mesh->SkinVerticesBlend.UnitSize ) ;

				// �P���_���g�p���Ă���ő�{�[�������擾
				MaxCon = 0 ;
				for( j = 0 ; j < ( int )MeshFace->IndexNum ; j ++ )
				{
					GetBitList( &Mesh->SkinVerticesBlend, ( int )MeshFace->VertexIndex[ j ], BitBuf ) ; 
					Con = GetBitCount( BitBuf, Mesh->SkinVerticesBlend.UnitSize ) ;
					if( MaxCon < Con ) MaxCon = Con ;
				}

				// �{�[�����X�ȏ�g�p����Ă��邩�ŏ����𕪊�
				if( MaxCon >= 9 )
				{
					Mesh->SkinFaceType[ i ] = MV1_VERTEX_TYPE_SKIN_FREEBONE ;
				}
				else
				// �{�[�����T�ȏ�g�p����Ă��邩�ŏ����𕪊�
				if( MaxCon >= 5 )
				{
					Mesh->SkinFaceType[ i ] = MV1_VERTEX_TYPE_SKIN_8BONE ;
				}
				else
				// �{�[������ł��g�p����Ă��邩�ǂ����ŏ����𕪊�
				if( MaxCon >= 1 )
				{
					Mesh->SkinFaceType[ i ] = MV1_VERTEX_TYPE_SKIN_4BONE ;
				}
				else
				// �����ɗ����ꍇ�͎g�p����Ă���{�[���͈���Ȃ��Ƃ�������
				{
					Mesh->SkinFaceType[ i ] = MV1_VERTEX_TYPE_NORMAL ;
					MaxCon = 1 ;
//					return DXST_ERRORLOG_ADDUTF16LE( L"Error �{�[���̎w�肪����Ȃ��|���S��������܂� by Fbx\n" ) ;
				}

				// �ւ���Ă���{�[���̐���ۑ�
				Mesh->SkinFaceBoneNum[ i ] = ( WORD )MaxFaceCon ;
			}
		}

		// �}�e���A�����̃|���S���̏����\�z����
		{
			DWORD PolyNum, Ind ;

			// �}�e���A���̎�ނ̐������m��
			Mesh->MaterialPolyList = ( MV1_MATERIAL_POLY_R * )ADDMEMAREA( sizeof( MV1_MATERIAL_POLY_R ) * ( Mesh->MaterialNum == 0 ? 1 : Mesh->MaterialNum ), &ReadModel->Mem ) ;
			if( Mesh->MaterialPolyList == NULL )
			{
				DXST_ERRORLOGFMT_ADDUTF16LE(( "\x52\x00\x65\x00\x61\x00\x64\x00\x20\x00\x4d\x00\x6f\x00\x64\x00\x65\x00\x6c\x00\x20\x00\x43\x00\x6f\x00\x6e\x00\x76\x00\x65\x00\x72\x00\x74\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xde\x30\xc6\x30\xea\x30\xa2\x30\xeb\x30\xce\x6b\x6e\x30\xdd\x30\xea\x30\xb4\x30\xf3\x30\xc7\x30\xfc\x30\xbf\x30\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Read Model Convert Error : �}�e���A�����̃|���S���f�[�^���i�[���郁�����̊m�ۂɎ��s���܂���\n" @*/ )) ;
				return -1 ;
			}

			// ���f�[�^�ł̃}�e���A���̐������Z
			ReadModel->MeshMaterialNum += Mesh->MaterialNum == 0 ? 1 : Mesh->MaterialNum ;

			// �V�F�C�v���b�V���̏ꍇ�̓V�F�C�v���b�V���̐������Z����
			if( Mesh->IsShapeMesh )
			{
				ReadModel->ShapeMeshNum += ( Mesh->MaterialNum == 0 ? 1 : Mesh->MaterialNum ) * Mesh->Container->ShapeNum ;
			}

			// �X�L�����b�V�����ǂ����ŏ����𕪊�
			if( Mesh->SkinWeightsNum )
			{
				// �e�t�F�C�X���g�p����g���C�A���O�����X�g�ԍ���ۑ����郁�����̈�̊m��
				Mesh->FaceUseTriangleList = ( WORD * )ADDMEMAREA( sizeof( WORD ) * Mesh->FaceNum, &ReadModel->Mem ) ;
				if( Mesh->FaceUseTriangleList == NULL )
				{
					DXST_ERRORLOGFMT_ADDUTF16LE(( "\x52\x00\x65\x00\x61\x00\x64\x00\x20\x00\x4d\x00\x6f\x00\x64\x00\x65\x00\x6c\x00\x20\x00\x43\x00\x6f\x00\x6e\x00\x76\x00\x65\x00\x72\x00\x74\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\x04\x54\x62\x97\x68\x30\xc8\x30\xe9\x30\xa4\x30\xa2\x30\xf3\x30\xb0\x30\xeb\x30\xea\x30\xb9\x30\xc8\x30\x6e\x30\xa2\x95\xc2\x4f\x92\x30\xdd\x4f\x58\x5b\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Read Model Convert Error : �e�ʂƃg���C�A���O�����X�g�̊֌W��ۑ����郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
					return -1 ;
				}

				// �X�L�����b�V���̏ꍇ
				for( i = 0 ; ( DWORD )i < Mesh->FaceNum ; i ++ )
				{
					WORD UseMatrix[ MV1_TRIANGLE_LIST_USE_BONE_MAX_NUM ] ;
					int UseBoneNum ;

					PolyNum = Mesh->Faces[ i ].PolygonNum ;
					Ind = Mesh->Faces[ i ].MaterialIndex ;
					switch( Mesh->SkinFaceType[ i ] )
					{
					case MV1_VERTEX_TYPE_SKIN_FREEBONE :
						// �ւ��{�[�����X�ȏ�̏ꍇ

						// �|���S���̐������Z
						Mesh->MaterialPolyList[ Ind ].TypeNum[ MV1_VERTEX_TYPE_SKIN_FREEBONE ] += PolyNum ;

						// �ւ��{�[���̍ő吔���X�V
						if( Mesh->MaterialPolyList[ Ind ].MaxBoneCount < Mesh->SkinFaceBoneNum[ i ] ) 
							Mesh->MaterialPolyList[ Ind ].MaxBoneCount = Mesh->SkinFaceBoneNum[ i ] ;
						break ;

					case MV1_VERTEX_TYPE_SKIN_4BONE :
						// �ւ��{�[�����S�ȓ��̏ꍇ
						MPoly = &Mesh->MaterialPolyList[ Ind ] ;

						// �|���S���̐������Z
						Mesh->MaterialPolyList[ Ind ].TypeNum[ Mesh->SkinFaceType[ i ] ] += PolyNum ;

						// �������g���s��̃��X�g���擾����
						UseBoneNum = GetBitListNumber( &Mesh->SkinFacesBlend, ( int )i, UseMatrix ) ;

						// �������g���s�񂪊��ɂ���g���C�A���O�����X�g���ɂ��邩���ׂ�
						TList = MPoly->SkinB4TriangleList ;
						for( j = 0 ; j < MPoly->SkinB4TriangleListNum ; j ++, TList ++ )
						{
							if( MV1_TRIANGLE_MAX_INDEX <= ( TList->PolyNum + PolyNum ) * 3 ) continue ;

							for( l = 0 ; l < UseBoneNum ; l ++ )
							{
								for( k = 0 ; k < TList->UseBoneNum && TList->UseBone[ k ] != UseMatrix[ l ] ; k ++ ){}
								if( k == TList->UseBoneNum ) break ;
							}

							// �������ꍇ�͏���ǉ�
							if( l == UseBoneNum )
							{
								TList->PolyNum += PolyNum ;
								Mesh->FaceUseTriangleList[ i ] = ( WORD )j ;
								break ;
							}
						}

						// ���������ꍇ�͒ǉ��̏���
						if( j == MPoly->SkinB4TriangleListNum )
						{
							// �g���C�A���O�����X�g�̐����O��������P�ɂ���
							if( MPoly->SkinB4TriangleListNum == 0 )
							{
								MPoly->SkinB4TriangleListNum ++ ;
							}

							for(;;)
							{
								// �s��̐��� MV1_TRIANGLE_LIST_USE_BONE_MAX_NUM �ɒB���Ă��Ȃ��g���C�A���O�����X�g������ꍇ��
								// ���̃g���C�A���O�����X�g�Ŏg�p����s��̐��𑝂₵�Ċi�[����
								TList = MPoly->SkinB4TriangleList ;
								for( j = 0 ; j < MPoly->SkinB4TriangleListNum ; j ++, TList ++ )
								{
									if( MV1_TRIANGLE_MAX_INDEX <= ( TList->PolyNum + PolyNum ) * 3 ) continue ;

									for( l = 0 ; l < UseBoneNum ; l ++ )
									{
										for( k = 0 ; k < TList->UseBoneNum && TList->UseBone[ k ] != UseMatrix[ l ] ; k ++ ){}
										if( k == TList->UseBoneNum )
										{
											if( TList->UseBoneNum >= GParam->LoadModelToTriangleListUseMaxBoneNum ) break ;
											TList->UseBone[ TList->UseBoneNum ] = UseMatrix[ l ] ;
											TList->UseBoneNum ++ ;
										}
									}
									if( l == UseBoneNum )
									{
										TList->PolyNum += PolyNum ;
										Mesh->FaceUseTriangleList[ i ] = ( WORD )j ;
										break ;
									}
								}

								// �����̃g���C�A���O�����X�g����t�������炠����ȃg���C�A���O�����X�g���쐬����
								if( j == MPoly->SkinB4TriangleListNum )
								{
									MPoly->SkinB4TriangleListNum ++ ;
									continue ;
								}
								break ;
							}
						}

						// �ւ��{�[���̍ő吔���X�V
						if( Mesh->MaterialPolyList[ Ind ].MaxBoneCount < 4 ) 
							Mesh->MaterialPolyList[ Ind ].MaxBoneCount = 4 ;
						break ;

					case MV1_VERTEX_TYPE_SKIN_8BONE :
						// �ւ��{�[�����W�ȓ��̏ꍇ
						MPoly = &Mesh->MaterialPolyList[ Ind ] ;

						// �|���S���̐������Z
						Mesh->MaterialPolyList[ Ind ].TypeNum[ Mesh->SkinFaceType[ i ] ] += PolyNum ;

						// �������g���s��̃��X�g���擾����
						UseBoneNum = GetBitListNumber( &Mesh->SkinFacesBlend, ( int )i, UseMatrix ) ;

						// �������g���s�񂪊��ɂ���g���C�A���O�����X�g���ɂ��邩���ׂ�
						TList = MPoly->SkinB8TriangleList ;
						for( j = 0 ; j < MPoly->SkinB8TriangleListNum ; j ++, TList ++ )
						{
							if( MV1_TRIANGLE_MAX_INDEX <= ( TList->PolyNum + PolyNum ) * 3 ) continue ;

							for( l = 0 ; l < UseBoneNum ; l ++ )
							{
								for( k = 0 ; k < TList->UseBoneNum && TList->UseBone[ k ] != UseMatrix[ l ] ; k ++ ){}
								if( k == TList->UseBoneNum ) break ;
							}

							// �������ꍇ�͏���ǉ�
							if( l == UseBoneNum )
							{
								TList->PolyNum += PolyNum ;
								Mesh->FaceUseTriangleList[ i ] = ( WORD )j ;
								break ;
							}
						}

						// ���������ꍇ�͒ǉ��̏���
						if( j == MPoly->SkinB8TriangleListNum )
						{
							// �g���C�A���O�����X�g�̐����O��������P�ɂ���
							if( MPoly->SkinB8TriangleListNum == 0 )
							{
								MPoly->SkinB8TriangleListNum ++ ;
							}

							for(;;)
							{
								// �s��̐��� MV1_TRIANGLE_LIST_USE_BONE_MAX_NUM �ɒB���Ă��Ȃ��g���C�A���O�����X�g������ꍇ��
								// ���̃g���C�A���O�����X�g�Ŏg�p����s��̐��𑝂₵�Ċi�[����
								TList = MPoly->SkinB8TriangleList ;
								for( j = 0 ; j < MPoly->SkinB8TriangleListNum ; j ++, TList ++ )
								{
									if( MV1_TRIANGLE_MAX_INDEX <= ( TList->PolyNum + PolyNum ) * 3 ) continue ;

									for( l = 0 ; l < UseBoneNum ; l ++ )
									{
										for( k = 0 ; k < TList->UseBoneNum && TList->UseBone[ k ] != UseMatrix[ l ] ; k ++ ){}
										if( k == TList->UseBoneNum )
										{
											if( TList->UseBoneNum >= GParam->LoadModelToTriangleListUseMaxBoneNum ) break ;
											TList->UseBone[ TList->UseBoneNum ] = UseMatrix[ l ] ;
											TList->UseBoneNum ++ ;
										}
									}
									if( l == UseBoneNum )
									{
										TList->PolyNum += PolyNum ;
										Mesh->FaceUseTriangleList[ i ] = ( WORD )j ;
										break ;
									}
								}

								// �����̃g���C�A���O�����X�g����t�������炠����ȃg���C�A���O�����X�g���쐬����
								if( j == MPoly->SkinB8TriangleListNum )
								{
									MPoly->SkinB8TriangleListNum ++ ;
									continue ;
								}
								break ;
							}
						}

						// �ւ��{�[���̍ő吔���X�V
						if( Mesh->MaterialPolyList[ Ind ].MaxBoneCount < 8 ) 
							Mesh->MaterialPolyList[ Ind ].MaxBoneCount = 8 ;
						break ;
					}
				}
			}
			else
			{
				// �X�L�����b�V���ł͂Ȃ��ꍇ
				for( i = 0 ; ( DWORD )i < Mesh->FaceNum ; i ++ )
				{
					PolyNum = Mesh->Faces[ i ].PolygonNum ;
					Ind = Mesh->Faces[ i ].MaterialIndex ;
					Mesh->MaterialPolyList[ Ind ].TypeNum[ MV1_VERTEX_TYPE_NORMAL ] += PolyNum ;
				}
			}
		}

		// ���_�f�[�^�̃T�C�Y���Z�o
		{
			DWORD MatNum ;
			DWORD FaceNum ;
			DWORD Size ;

			// �}�e���A���̐����Z�b�g
			MatNum = Mesh->MaterialNum == 0 ? 1 : Mesh->MaterialNum ;

			// �}�e���A���̐������J��Ԃ�
			Mesh->VertexDataTotalSize = 0 ;
			Mesh->MaxBoneUseNum = 0 ;
			MPoly = Mesh->MaterialPolyList ;
			for( i = 0 ; i < MatNum ; i ++, MPoly ++ )
			{
				// �ʂ̐���������
				FaceNum = 0 ;

				// �@���̐������Z
				ReadModel->MeshNormalNum += Mesh->NormalNum ;

				// �ő哯���g�p�{�[�����̍X�V
				if( ( DWORD )MPoly->MaxBoneCount > Mesh->MaxBoneUseNum )
				{
					Mesh->MaxBoneUseNum = ( DWORD )MPoly->MaxBoneCount ;
				}

				// �P�{�[���|���S��
				if( MPoly->TypeNum[ MV1_VERTEX_TYPE_NORMAL ] )
				{
					FaceNum += MPoly->TypeNum[ MV1_VERTEX_TYPE_NORMAL ] ;
					ReadModel->MeshFaceNum += MPoly->TypeNum[ MV1_VERTEX_TYPE_NORMAL ] ;
					Size = sizeof( MV1_TLIST_NORMAL_POS ) * 3 * MPoly->TypeNum[ MV1_VERTEX_TYPE_NORMAL ] ;
					Mesh->VertexDataTotalSize                             += Size ;
					Mesh->TypeNumVertexDataSize[ MV1_VERTEX_TYPE_NORMAL ] += Size ;
					ReadModel->NormalPositionNum                          += 3 * MPoly->TypeNum[ MV1_VERTEX_TYPE_NORMAL ] ;
					ReadModel->MeshVertexIndexNum                         += 3 * MPoly->TypeNum[ MV1_VERTEX_TYPE_NORMAL ] ;
					if( Mesh->IsShapeMesh )
					{
						ReadModel->ShapeNormalPositionNum += 3 * MPoly->TypeNum[ MV1_VERTEX_TYPE_NORMAL ] ;
					}
				}

				// �T�����{�[���|���S��
				if( MPoly->TypeNum[ MV1_VERTEX_TYPE_SKIN_4BONE ] )
				{
					FaceNum += MPoly->TypeNum[ MV1_VERTEX_TYPE_SKIN_4BONE ] ;
					ReadModel->MeshFaceNum += MPoly->TypeNum[ MV1_VERTEX_TYPE_SKIN_4BONE ] ;
					Size = sizeof( MV1_TLIST_SKIN_POS_4B ) * 3 * MPoly->TypeNum[ MV1_VERTEX_TYPE_SKIN_4BONE ] ;
					Mesh->VertexDataTotalSize                                 += Size ;
					Mesh->TypeNumVertexDataSize[ MV1_VERTEX_TYPE_SKIN_4BONE ] += Size ;
					ReadModel->SkinPosition4BNum                              += 3 * MPoly->TypeNum[ MV1_VERTEX_TYPE_SKIN_4BONE ] ;
					ReadModel->MeshVertexIndexNum                             += 3 * MPoly->TypeNum[ MV1_VERTEX_TYPE_SKIN_4BONE ] ;
					if( Mesh->IsShapeMesh )
					{
						ReadModel->ShapeSkinPosition4BNum += 3 * MPoly->TypeNum[ MV1_VERTEX_TYPE_SKIN_4BONE ] ;
					}
				}

				// �X�����{�[���|���S��
				if( MPoly->TypeNum[ MV1_VERTEX_TYPE_SKIN_8BONE ] )
				{
					FaceNum += MPoly->TypeNum[ MV1_VERTEX_TYPE_SKIN_8BONE ] ;
					ReadModel->MeshFaceNum += MPoly->TypeNum[ MV1_VERTEX_TYPE_SKIN_8BONE ] ;
					Size = sizeof( MV1_TLIST_SKIN_POS_8B ) * 3 * MPoly->TypeNum[ MV1_VERTEX_TYPE_SKIN_8BONE ] ;
					Mesh->VertexDataTotalSize                                 += Size ;
					Mesh->TypeNumVertexDataSize[ MV1_VERTEX_TYPE_SKIN_8BONE ] += Size ;
					ReadModel->SkinPosition8BNum                              += 3 * MPoly->TypeNum[ MV1_VERTEX_TYPE_SKIN_8BONE ] ;
					ReadModel->MeshVertexIndexNum                             += 3 * MPoly->TypeNum[ MV1_VERTEX_TYPE_SKIN_8BONE ] ;
					if( Mesh->IsShapeMesh )
					{
						ReadModel->ShapeSkinPosition8BNum += 3 * MPoly->TypeNum[ MV1_VERTEX_TYPE_SKIN_8BONE ] ;
					}
				}

				// �{�[�����������|���S��
				if( MPoly->TypeNum[ MV1_VERTEX_TYPE_SKIN_FREEBONE ] )
				{
					FaceNum += MPoly->TypeNum[ MV1_VERTEX_TYPE_SKIN_FREEBONE ] ;
					ReadModel->MeshFaceNum += MPoly->TypeNum[ MV1_VERTEX_TYPE_SKIN_FREEBONE ] ;
					Size = ( sizeof( MV1_TLIST_SKIN_POS_FREEB ) + ( MPoly->MaxBoneCount - 4 ) * sizeof( MV1_SKINBONE_BLEND ) ) * 3 * MPoly->TypeNum[ MV1_VERTEX_TYPE_SKIN_FREEBONE ] ;
					Size = ( Size + 15 ) / 16 * 16 ;
					Mesh->VertexDataTotalSize                                    += Size ;
					Mesh->TypeNumVertexDataSize[ MV1_VERTEX_TYPE_SKIN_FREEBONE ] += Size ;
					ReadModel->SkinPositionFREEBSize                             += Size ;
					ReadModel->MeshVertexIndexNum                                += 3 * MPoly->TypeNum[ MV1_VERTEX_TYPE_SKIN_FREEBONE ] ;
					if( Mesh->IsShapeMesh )
					{
						ReadModel->SkinPositionFREEBSize += Size ;
					}
				}

				// ���b�V�����_�f�[�^�̃T�C�Y�����Z
				Size = ( sizeof( MV1_MESH_VERTEX ) + ( ( Mesh->MaterialNum == 0 || Mesh->Materials[ i ]->DiffuseTexNum == 0 ? 1 : Mesh->Materials[ i ]->DiffuseTexNum ) - 1 ) * sizeof( float ) * 2 ) * FaceNum * 3 ;
				Mesh->VertexDataTotalSize += Size ;
				ReadModel->MeshVertexSize += Size ;

				if( Mesh->IsShapeMesh )
				{
					ReadModel->ShapeVertexNum += FaceNum * 3 * Mesh->Container->ShapeNum ;
				}
			}

			// �ő哯���g�p�{�[�����̍X�V
			if( Mesh->MaxBoneUseNum > Mesh->Container->MaxBoneUseNum )
				Mesh->Container->MaxBoneUseNum = Mesh->MaxBoneUseNum ;

			// ���b�V���@���f�[�^�̃T�C�Y�����Z
			Size = sizeof( MV1_MESH_NORMAL ) * Mesh->NormalNum ;
			Mesh->VertexDataTotalSize += Size ;
			ReadModel->MeshNormalNum += Mesh->NormalNum ;

			// �g���C�A���O�����X�g�ƃg���C�A���O���C���f�b�N�X�̐����Z�o
			{
				Mesh->TriangleListNum = 0 ;
				Mesh->IndexDataTotalSize = 0 ;

				// �X�L�����b�V�����ǂ����ŏ����𕪊�
				if( Mesh->SkinWeightsNum )
				{
					// �X�L�����b�V���̏ꍇ
					MPoly = Mesh->MaterialPolyList ;
					for( i = 0 ; i < MatNum ; i ++, MPoly ++ )
					{
						// �T�{�[�������̃|���S��
						if( MPoly->TypeNum[ MV1_VERTEX_TYPE_SKIN_4BONE ] )
						{
							Mesh->TriangleListNum += MPoly->SkinB4TriangleListNum ;
							Size = MPoly->TypeNum[ MV1_VERTEX_TYPE_SKIN_4BONE ] * 3 * sizeof( WORD ) ;
							Mesh->IndexDataTotalSize                                 += Size ;
							Mesh->TypeNumIndexDataSize[ MV1_VERTEX_TYPE_SKIN_4BONE ] += Size ;
						}

						// �X�{�[�������̃|���S��
						if( MPoly->TypeNum[ MV1_VERTEX_TYPE_SKIN_8BONE ] )
						{
							Mesh->TriangleListNum += MPoly->SkinB8TriangleListNum ;
							Size = MPoly->TypeNum[ MV1_VERTEX_TYPE_SKIN_8BONE ] * 3 * sizeof( WORD ) ;
							Mesh->IndexDataTotalSize                                 += Size ;
							Mesh->TypeNumIndexDataSize[ MV1_VERTEX_TYPE_SKIN_8BONE ] += Size ;
						}

						// �{�[�����������̃|���S��
						if( MPoly->TypeNum[ MV1_VERTEX_TYPE_SKIN_FREEBONE ] )
						{
							Mesh->TriangleListNum += ( MPoly->TypeNum[ MV1_VERTEX_TYPE_SKIN_FREEBONE ] * 3 + MV1_TRIANGLE_MAX_INDEX - 1 ) / MV1_TRIANGLE_MAX_INDEX ;
							Size = MPoly->TypeNum[ MV1_VERTEX_TYPE_SKIN_FREEBONE ] * 3 * sizeof( WORD ) ;
							Mesh->IndexDataTotalSize                                    += Size ;
							Mesh->TypeNumIndexDataSize[ MV1_VERTEX_TYPE_SKIN_FREEBONE ] += Size ;
						}
					}
				}
				else
				{
					// �X�L�����b�V���ł͂Ȃ��ꍇ
					MPoly = Mesh->MaterialPolyList ;
					for( i = 0 ; i < MatNum ; i ++, MPoly ++ )
					{
						MPoly->SimpleTriangleListNum = ( int )( ( MPoly->TypeNum[ MV1_VERTEX_TYPE_NORMAL ] * 3 + MV1_TRIANGLE_MAX_INDEX - 1 ) / MV1_TRIANGLE_MAX_INDEX ) ;
						Mesh->TriangleListNum += MPoly->SimpleTriangleListNum ;
						Size = MPoly->TypeNum[ MV1_VERTEX_TYPE_NORMAL ] * 3 * sizeof( WORD ) ;
						Mesh->IndexDataTotalSize                             += Size ;
						Mesh->TypeNumIndexDataSize[ MV1_VERTEX_TYPE_NORMAL ] += Size ;
					}
				}

				// �g���C�A���O�����X�g�̐������Z
				ReadModel->TriangleListNum += Mesh->TriangleListNum ;

				// ���_�C���f�b�N�X�̐������Z
				ReadModel->IndexNum += Mesh->IndexDataTotalSize / sizeof( WORD ) ;
			}
		}
	}

	// ���b�V�����W�f�[�^�̃T�C�Y�����Z
	for( Mesh = ReadModel->MeshFirst ; Mesh ; Mesh = Mesh->DataNext )
	{
		Size = ( int )( ( sizeof( MV1_MESH_POSITION ) + ( Mesh->Container->MaxBoneUseNum - 4 ) * sizeof( MV1_SKINBONE_BLEND ) ) * Mesh->PositionNum ) ;
		Mesh->VertexDataTotalSize   += Size ;
		ReadModel->MeshPositionSize += Size ;
	}

	// ��ԕω��Ǘ��e�[�u���ɕK�v�ȃ������T�C�Y���Z�o����
	{
		MV1_FRAME_R *TempFrame ;
		MV1_MESH_R *TempMesh ;
		int MeshNum ;

		// �e�t���[�����]���鑍�t���[�����Ƒ����b�V�������Z�o����
		Frame = ReadModel->FrameFirst ;
		ReadModel->TotalMV1MeshNum = 0 ;
		for( i = 0 ; ( DWORD )i < ReadModel->FrameNum ; i ++, Frame = Frame->DataNext )
		{
			MeshNum = 0 ;
			for( TempMesh = Frame->MeshFirst ; TempMesh ; TempMesh = TempMesh->Next )
				MeshNum += TempMesh->MaterialNum ? TempMesh->MaterialNum : 1 ;
			ReadModel->TotalMV1MeshNum += MeshNum ;

			Frame->TotalChildNum = 0 ;
			Frame->TotalMeshNum = MeshNum ;

			for( TempFrame = Frame->Parent ; TempFrame ; TempFrame = TempFrame->Parent )
			{
				TempFrame->TotalChildNum ++ ;
				TempFrame->TotalMeshNum += MeshNum ;
			}
		}

		// �]���鑍�t���[�����Ƒ����b�V��������e�[�u���ɕK�v�ȃ������T�C�Y���Z�o����
		ReadModel->ChangeDrawMaterialTableSize = 0 ;
		ReadModel->ChangeMatrixTableSize = 0 ;
		Frame = ReadModel->FrameFirst ;
		for( i = 0 ; ( DWORD )i < ReadModel->FrameNum ; i ++, Frame = Frame->DataNext )
		{
			ReadModel->ChangeMatrixTableSize       += ( 1 + Frame->TotalChildNum + ReadModel->FrameNum                       + 31 ) / 32 * 4 + 4 ;
			ReadModel->ChangeDrawMaterialTableSize += ( 1 + Frame->TotalChildNum + ReadModel->FrameNum + Frame->TotalMeshNum + 31 ) / 32 * 4 + 4 ;
		}
		ReadModel->ChangeDrawMaterialTableSize = ( ReadModel->ChangeDrawMaterialTableSize + 15 ) / 16 * 16 ;
		ReadModel->ChangeMatrixTableSize       = ( ReadModel->ChangeMatrixTableSize       + 15 ) / 16 * 16 ;
	}
	ReadModel->MeshVertexSize   = ( ReadModel->MeshVertexSize   + 15 ) / 16 * 16 ; 
	ReadModel->MeshPositionSize = ( ReadModel->MeshPositionSize + 15 ) / 16 * 16 ; 

	// �������̊m�ۃT�C�Y���Z�o
	{
		_MEMSET( &MTBase, 0, sizeof( MTBase ) ) ;
		AllocSize = 0 ;

		// �t�@�C���p�X��ۑ����鑊�΃A�h���X���Z�b�g
		MTBase.FilePath = ( wchar_t * )AllocSize ;
		AllocSize += ( ( _WCSLEN( ReadModel->FilePath ) + 1 ) * sizeof( wchar_t ) + 3 ) / 4 * 4 ;

		// ���f������ۑ����鑊�΃A�h���X���Z�b�g
		MTBase.Name = ( wchar_t * )AllocSize ;
		AllocSize += ( ( _WCSLEN( ReadModel->Name ) + 1 ) * sizeof( wchar_t ) + 3 ) / 4 * 4 ;

		// �f�B���N�g���p�X��ۑ����鑊�΃A�h���X���Z�b�g
		MTBase.DirectoryPath = ( wchar_t * )AllocSize ;
		AllocSize += ( ( dirlen + 1 ) * sizeof( wchar_t ) + 3 ) / 4 * 4 ;

		// �ύX�e�[�u������ۑ����鑊�΃A�h���X���Z�b�g
		MTBase.ChangeDrawMaterialTable = ( DWORD * )AllocSize ;
		AllocSize += ReadModel->ChangeDrawMaterialTableSize ;

		MTBase.ChangeMatrixTable = ( DWORD * )AllocSize ;
		AllocSize += ReadModel->ChangeMatrixTableSize ;

		// �t���[���z��̊m�ۃ������擪����̑��΃A�h���X���Z�b�g
		MTBase.FrameNum = ( int )ReadModel->FrameNum ;
		MTBase.Frame = ( MV1_FRAME_BASE * )AllocSize ;
		AllocSize += MTBase.FrameNum * sizeof( MV1_FRAME_BASE ) ;
		MTBase.FrameUseSkinBoneNum = ( int )ReadModel->SkinWeightNum ;
		MTBase.FrameUseSkinBone = ( MV1_SKIN_BONE ** )AllocSize ;
		AllocSize += MTBase.FrameUseSkinBoneNum * sizeof( MV1_SKIN_BONE * ) ;

		// �}�e���A���z��̊m�ۃ������擪����̑��΃A�h���X���Z�b�g
		MTBase.MaterialNum = ( int )ReadModel->MaterialNum ;
		MTBase.Material = ( MV1_MATERIAL_BASE * )AllocSize ;
		AllocSize += MTBase.MaterialNum * sizeof( MV1_MATERIAL_BASE ) ;

		// �e�N�X�`���z��̊m�ۃ������擪����̑��΃A�h���X���Z�b�g
		MTBase.TextureNum = ( int )ReadModel->TextureNum ;
		MTBase.Texture = ( MV1_TEXTURE_BASE * )AllocSize ;
		AllocSize += MTBase.TextureNum * sizeof( MV1_TEXTURE_BASE ) ;

		// ���C�g�̐����Z�b�g
		MTBase.LightNum = ( int )ReadModel->LightNum ;
		MTBase.Light = ( MV1_LIGHT * )AllocSize ;
		AllocSize += MTBase.LightNum * sizeof( MV1_LIGHT ) ;

		// �X�L�j���O���b�V���p�̃{�[�����̐����Z�b�g
		MTBase.SkinBoneNum = ( int )ReadModel->SkinWeightNum ;
		MTBase.SkinBone = ( MV1_SKIN_BONE * )AllocSize ;
		AllocSize += MTBase.SkinBoneNum * sizeof( MV1_SKIN_BONE ) ;
		MTBase.SkinBoneUseFrameNum = MTBase.SkinBoneNum ;
		MTBase.SkinBoneUseFrame = ( MV1_SKIN_BONE_USE_FRAME * )AllocSize ;
		AllocSize += MTBase.SkinBoneUseFrameNum * sizeof( MV1_SKIN_BONE_USE_FRAME ) ;

		// �V�F�C�v�f�[�^�z��̊m�ۃ������擪����̑��΃A�h���X���Z�b�g
		MTBase.ShapeNum = ( int )ReadModel->ShapeNum ;
		MTBase.Shape = ( MV1_SHAPE_BASE * )AllocSize ;
		AllocSize += MTBase.ShapeNum * sizeof( MV1_SHAPE_BASE ) ;

		// �V�F�C�v���b�V���̃������A�h���X���Z�b�g
		MTBase.ShapeMeshNum = ( int )ReadModel->ShapeMeshNum ;
		MTBase.ShapeMesh = ( MV1_SHAPE_MESH_BASE * )AllocSize ;
		AllocSize += MTBase.ShapeMeshNum * sizeof( MV1_SHAPE_MESH_BASE ) ;

		// �������Z�Ŏg�p���鍄�̃f�[�^�̃������A�h���X���Z�b�g
		MTBase.PhysicsRigidBodyNum = ( int )ReadModel->PhysicsRigidBodyNum ;
		MTBase.PhysicsRigidBody = ( MV1_PHYSICS_RIGIDBODY_BASE * )AllocSize ;
		AllocSize += MTBase.PhysicsRigidBodyNum * sizeof( MV1_PHYSICS_RIGIDBODY_BASE ) ;

		// �������Z�Ŏg�p����W���C���g�f�[�^�̃������A�h���X���Z�b�g
		MTBase.PhysicsJointNum = ( int )ReadModel->PhysicsJointNum ;
		MTBase.PhysicsJoint = ( MV1_PHYSICS_JOINT_BASE * )AllocSize ;
		AllocSize += MTBase.PhysicsJointNum * sizeof( MV1_PHYSICS_JOINT_BASE ) ;

		// ���b�V���̐����Z�b�g
		MTBase.MeshNum = ( int )ReadModel->MeshMaterialNum ;
		MTBase.Mesh = ( MV1_MESH_BASE * )AllocSize ;
		AllocSize += MTBase.MeshNum * sizeof( MV1_MESH_BASE ) ;

		// �g���C�A���O�����X�g�̃������A�h���X���Z�b�g
		MTBase.TriangleListNum = ( int )ReadModel->TriangleListNum ;
		MTBase.TriangleList = ( MV1_TRIANGLE_LIST_BASE * )AllocSize ;
		AllocSize += MTBase.TriangleListNum * sizeof( MV1_TRIANGLE_LIST_BASE ) ;

		// ���_�C���f�b�N�X�̃������A�h���X���Z�b�g
		MTBase.TriangleListIndexNum = ( int )ReadModel->IndexNum ;
		MTBase.TriangleListIndex = ( WORD * )AllocSize ;
		AllocSize += ( MTBase.TriangleListIndexNum * 2 ) * sizeof( WORD ) ;
		AllocSize = ( AllocSize + 15 ) / 16 * 16 ; 

		// �������ۑ����郁�����A�h���X�̃Z�b�g
#ifndef UNICODE
		MTBase.StringSizeA = ( int )ReadModel->StringSizeA ;
		MTBase.StringBufferA = ( char * )AllocSize ;
		AllocSize += MTBase.StringSizeA ;
#endif

		MTBase.StringSizeW = ( int )ReadModel->StringSizeW ;
		MTBase.StringBufferW = ( wchar_t * )AllocSize ;
		AllocSize += MTBase.StringSizeW ;

		// �A�j���[�V�����Z�b�g��ۑ����郁�����A�h���X�̃Z�b�g
		MTBase.AnimSetNum = ( int )ReadModel->AnimSetNum ;
		MTBase.AnimSet = ( MV1_ANIMSET_BASE * )AllocSize ;
		AllocSize += MTBase.AnimSetNum * sizeof( MV1_ANIMSET_BASE ) ;

		// �A�j���[�V������ۑ����郁�����A�h���X�̃Z�b�g
		MTBase.AnimNum = ( int )ReadModel->AnimNum ;
		MTBase.Anim = ( MV1_ANIM_BASE * )AllocSize ;
		AllocSize += MTBase.AnimNum * sizeof( MV1_ANIM_BASE ) ;

		// �A�j���[�V�����L�[�Z�b�g��ۑ����郁�����A�h���X�̃Z�b�g
		MTBase.AnimKeySetNum = ( int )ReadModel->AnimKeySetNum ;
		MTBase.AnimKeySet = ( MV1_ANIM_KEYSET_BASE * )AllocSize ;
		AllocSize += MTBase.AnimKeySetNum * sizeof( MV1_ANIM_KEYSET_BASE ) ;

		// �e�A�j���[�V�����L�[��ۑ����郁�����A�h���X�̃Z�b�g
		MTBase.AnimKeyDataSize = ( int )ReadModel->AnimKeyDataSize ;
		MTBase.AnimKeyData = ( void * )AllocSize ;
		AllocSize += MTBase.AnimKeyDataSize ;

		// �A�j���[�V�����ƃt���[���̑Ή��e�[�u���̃������A�h���X���Z�b�g
		MTBase.AnimTargetFrameTable = ( MV1_ANIM_BASE ** )AllocSize ;
		AllocSize += sizeof( MV1_ANIM_BASE * ) * MTBase.AnimSetNum * MTBase.FrameNum ;
	}

	// ���f����f�[�^�n���h���̍쐬
	NewHandle = MV1AddModelBase( ASyncThread ) ;
	if( NewHandle < 0 )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\x52\x00\x65\x00\x61\x00\x64\x00\x20\x00\x4d\x00\x6f\x00\x64\x00\x65\x00\x6c\x00\x20\x00\x43\x00\x6f\x00\x6e\x00\x76\x00\x65\x00\x72\x00\x74\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xe2\x30\xc7\x30\xeb\x30\xfa\x57\x2c\x67\xc7\x30\xfc\x30\xbf\x30\xcf\x30\xf3\x30\xc9\x30\xeb\x30\x6e\x30\xd6\x53\x97\x5f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Read Model Convert Error : ���f����{�f�[�^�n���h���̎擾�Ɏ��s���܂���\n" @*/ )) ;
		return -1 ;
	}
	MBase = ( MV1_MODEL_BASE * )GetHandleInfo( NewHandle ) ;

	// ���f����f�[�^�̍\�z
	{
		// ����ۑ����郁��������x�Ɋm�ۂ���
		MBase->DataBuffer = MDALLOCMEM( AllocSize ) ;
		if( MBase->DataBuffer == NULL )
		{
			DXST_ERRORLOG_ADDUTF16LE( "\x52\x00\x65\x00\x61\x00\x64\x00\x20\x00\x4d\x00\x6f\x00\x64\x00\x65\x00\x6c\x00\x20\x00\x43\x00\x6f\x00\x6e\x00\x76\x00\x65\x00\x72\x00\x74\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xe2\x30\xc7\x30\xeb\x30\xfa\x57\x2c\x67\xc7\x30\xfc\x30\xbf\x30\xdd\x4f\x58\x5b\x28\x75\x6e\x30\xe1\x30\xe2\x30\xea\x30\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Read Model Convert Error : ���f����{�f�[�^�ۑ��p�̃������̊m�ۂɎ��s���܂���\n" @*/ ) ;
			goto ERRORLABEL ;
		}
		MBase->AllocMemorySize = AllocSize ;
		_MEMSET( MBase->DataBuffer, 0, AllocSize ) ;

		// �g���C�A���O�����X�g�\�z�p�����i�[���郁�����̊m��
		if( MaxPositionNum + MaxTriangleNum > 0 )
		{
			VertInfoTable = ( MV1_MAKEVERTINDEXINFO ** )DXALLOC( sizeof( MV1_MAKEVERTINDEXINFO * ) * MaxPositionNum + sizeof( MV1_MAKEVERTINDEXINFO ) * MaxTriangleNum * 3 ) ;
			if( VertInfoTable == NULL )
			{
				DXST_ERRORLOG_ADDUTF16LE( "\x52\x00\x65\x00\x61\x00\x64\x00\x20\x00\x4d\x00\x6f\x00\x64\x00\x65\x00\x6c\x00\x20\x00\x43\x00\x6f\x00\x6e\x00\x76\x00\x65\x00\x72\x00\x74\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xe2\x30\xc7\x30\xeb\x30\xfa\x57\x2c\x67\xc7\x30\xfc\x30\xbf\x30\xcb\x69\xc9\x7b\x6b\x30\x7f\x4f\x28\x75\x59\x30\x8b\x30\xc6\x30\xf3\x30\xdd\x30\xe9\x30\xea\x30\xd0\x30\xc3\x30\xd5\x30\xa1\x30\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Read Model Convert Error : ���f����{�f�[�^�\�z�Ɏg�p����e���|�����o�b�t�@�̊m�ۂɎ��s���܂���\n" @*/ ) ;
				goto ERRORLABEL ;
			}
			VertInfoBuffer = ( MV1_MAKEVERTINDEXINFO * )( VertInfoTable + MaxPositionNum ) ;
			VertValidBuffer = ( int * )VertInfoTable ;
		}
		else
		{
			VertInfoBuffer  = NULL ;
			VertValidBuffer = NULL ;
		}

		// �e�������A�h���X�̐擪���Z�b�g
		MBase->DirectoryPath		= ( wchar_t * )                   ( ( BYTE * )MBase->DataBuffer + ( DWORD_PTR )MTBase.DirectoryPath        ) ;
		MBase->FilePath				= ( wchar_t * )                   ( ( BYTE * )MBase->DataBuffer + ( DWORD_PTR )MTBase.FilePath             ) ;
		MBase->Name					= ( wchar_t * )                   ( ( BYTE * )MBase->DataBuffer + ( DWORD_PTR )MTBase.Name                 ) ;

		MBase->Frame                = ( MV1_FRAME_BASE * )            ( ( BYTE * )MBase->DataBuffer + ( DWORD_PTR )MTBase.Frame                ) ;
		MBase->FrameUseSkinBone     = ( MV1_SKIN_BONE ** )            ( ( BYTE * )MBase->DataBuffer + ( DWORD_PTR )MTBase.FrameUseSkinBone     ) ;
		MBase->ChangeDrawMaterialTable = ( DWORD * )                  ( ( BYTE * )MBase->DataBuffer + ( DWORD_PTR )MTBase.ChangeDrawMaterialTable ) ;
		MBase->ChangeMatrixTable    = ( DWORD * )                     ( ( BYTE * )MBase->DataBuffer + ( DWORD_PTR )MTBase.ChangeMatrixTable    ) ;
		MBase->Material             = ( MV1_MATERIAL_BASE * )         ( ( BYTE * )MBase->DataBuffer + ( DWORD_PTR )MTBase.Material             ) ;
		MBase->Texture              = ( MV1_TEXTURE_BASE * )	      ( ( BYTE * )MBase->DataBuffer + ( DWORD_PTR )MTBase.Texture              ) ;
		MBase->SkinBone             = ( MV1_SKIN_BONE * )             ( ( BYTE * )MBase->DataBuffer + ( DWORD_PTR )MTBase.SkinBone             ) ;
		MBase->SkinBoneUseFrame     = ( MV1_SKIN_BONE_USE_FRAME * )   ( ( BYTE * )MBase->DataBuffer + ( DWORD_PTR )MTBase.SkinBoneUseFrame     ) ;
		MBase->Mesh                 = ( MV1_MESH_BASE * )             ( ( BYTE * )MBase->DataBuffer + ( DWORD_PTR )MTBase.Mesh                 ) ;
		MBase->Light                = ( MV1_LIGHT * )                 ( ( BYTE * )MBase->DataBuffer + ( DWORD_PTR )MTBase.Light                ) ;
		MBase->TriangleList         = ( MV1_TRIANGLE_LIST_BASE * )    ( ( BYTE * )MBase->DataBuffer + ( DWORD_PTR )MTBase.TriangleList         ) ;
		MBase->TriangleListIndex    = ( WORD * )                      ( ( BYTE * )MBase->DataBuffer + ( DWORD_PTR )MTBase.TriangleListIndex    ) ;
#ifndef UNICODE
		MBase->StringBufferA        = ( char * )                      ( ( BYTE * )MBase->DataBuffer + ( DWORD_PTR )MTBase.StringBufferA        ) ;
#endif
		MBase->StringBufferW        = ( wchar_t * )                   ( ( BYTE * )MBase->DataBuffer + ( DWORD_PTR )MTBase.StringBufferW        ) ;
		MBase->AnimSet              = ( MV1_ANIMSET_BASE * )          ( ( BYTE * )MBase->DataBuffer + ( DWORD_PTR )MTBase.AnimSet              ) ;
		MBase->Anim                 = ( MV1_ANIM_BASE * )             ( ( BYTE * )MBase->DataBuffer + ( DWORD_PTR )MTBase.Anim                 ) ;
		MBase->AnimKeySet           = ( MV1_ANIM_KEYSET_BASE * )      ( ( BYTE * )MBase->DataBuffer + ( DWORD_PTR )MTBase.AnimKeySet           ) ;
		MBase->AnimKeyData          = ( void * )                      ( ( BYTE * )MBase->DataBuffer + ( DWORD_PTR )MTBase.AnimKeyData          ) ;
		MBase->AnimTargetFrameTable = ( MV1_ANIM_BASE ** )            ( ( BYTE * )MBase->DataBuffer + ( DWORD_PTR )MTBase.AnimTargetFrameTable ) ;
		MBase->Shape                = ( MV1_SHAPE_BASE * )            ( ( BYTE * )MBase->DataBuffer + ( DWORD_PTR )MTBase.Shape                ) ;
		MBase->ShapeMesh            = ( MV1_SHAPE_MESH_BASE * )       ( ( BYTE * )MBase->DataBuffer + ( DWORD_PTR )MTBase.ShapeMesh            ) ;
		MBase->PhysicsRigidBody		= ( MV1_PHYSICS_RIGIDBODY_BASE * )( ( BYTE * )MBase->DataBuffer + ( DWORD_PTR )MTBase.PhysicsRigidBody     ) ;
		MBase->PhysicsJoint		    = ( MV1_PHYSICS_JOINT_BASE * )    ( ( BYTE * )MBase->DataBuffer + ( DWORD_PTR )MTBase.PhysicsJoint         ) ;

		// ���_�f�[�^�i�[�p�̃������̈�̊m��
		MBase->ShapeNormalPositionNum = 0 ;
		MBase->ShapeSkinPosition4BNum = 0 ;
		MBase->ShapeSkinPosition8BNum = 0 ;
		MBase->ShapeSkinPositionFREEBSize = 0 ;
		MBase->TriangleListNormalPositionNum = 0 ;
		MBase->TriangleListSkinPosition4BNum = 0 ;
		MBase->TriangleListSkinPosition8BNum = 0 ;
		MBase->TriangleListSkinPositionFREEB = 0 ;
		MBase->MeshPositionSize = 0 ;
		MBase->MeshNormalNum = 0 ;
		MBase->MeshVertexSize = 0 ;
		MBase->MeshFaceNum = 0 ;
		if( ReadModel->NormalPositionNum     ||
			ReadModel->SkinPosition4BNum     ||
			ReadModel->SkinPosition8BNum     ||
			ReadModel->SkinPositionFREEBSize ||
			ReadModel->MeshFaceNum           ||
			ReadModel->MeshPositionSize      ||
			ReadModel->MeshNormalNum         ||
			ReadModel->MeshVertexIndexNum    ||
			ReadModel->MeshVertexSize        ||
			ReadModel->ShapeVertexNum        )
		{
			MBase->VertexData = DXALLOC(
				sizeof( DWORD )                 * ReadModel->MeshVertexIndexNum      +
				sizeof( MV1_MESH_FACE )         * ReadModel->MeshFaceNum             +
				sizeof( MV1_MESH_NORMAL )       * ReadModel->MeshNormalNum           +
				sizeof( MV1_TLIST_NORMAL_POS )  * ReadModel->NormalPositionNum       +
				sizeof( MV1_TLIST_SKIN_POS_4B ) * ReadModel->SkinPosition4BNum       +
				sizeof( MV1_TLIST_SKIN_POS_8B ) * ReadModel->SkinPosition8BNum       +
				ReadModel->SkinPositionFREEBSize                                     + 
				ReadModel->MeshPositionSize                                          +
				ReadModel->MeshVertexSize                                            +
				sizeof( MV1_SHAPE_VERTEX_BASE ) * ReadModel->ShapeVertexNum          + 16 ) ;
			if( MBase->VertexData == NULL )
			{
				DXST_ERRORLOG_ADDUTF16LE( "\x52\x00\x65\x00\x61\x00\x64\x00\x20\x00\x4d\x00\x6f\x00\x64\x00\x65\x00\x6c\x00\x20\x00\x43\x00\x6f\x00\x6e\x00\x76\x00\x65\x00\x72\x00\x74\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\x02\x98\xb9\x70\xa7\x5e\x19\x6a\x68\x30\x02\x98\xb9\x70\xd5\x6c\xda\x7d\x92\x30\x00\x4e\x42\x66\x84\x76\x6b\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Read Model Convert Error : ���_���W�ƒ��_�@�����ꎞ�I�Ɋi�[���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ ) ;
				goto ERRORLABEL ;
			}
			MBase->TriangleListNormalPosition    = ( MV1_TLIST_NORMAL_POS     * )( ( ( DWORD_PTR )MBase->VertexData + 15 ) / 16 * 16 ) ;
			MBase->TriangleListSkinPosition4B    = ( MV1_TLIST_SKIN_POS_4B    * )( MBase->TriangleListNormalPosition              + ReadModel->NormalPositionNum          ) ;
			MBase->TriangleListSkinPosition8B    = ( MV1_TLIST_SKIN_POS_8B    * )( MBase->TriangleListSkinPosition4B              + ReadModel->SkinPosition4BNum          ) ;
			MBase->TriangleListSkinPositionFREEB = ( MV1_TLIST_SKIN_POS_FREEB * )( MBase->TriangleListSkinPosition8B              + ReadModel->SkinPosition8BNum          ) ;
			MBase->MeshVertexIndex               = ( DWORD                    * )( ( BYTE * )MBase->TriangleListSkinPositionFREEB + ReadModel->SkinPositionFREEBSize      ) ;
			MBase->MeshFace                      = ( MV1_MESH_FACE            * )( MBase->MeshVertexIndex                         + ReadModel->MeshVertexIndexNum         ) ;
			MBase->MeshNormal                    = ( MV1_MESH_NORMAL          * )( MBase->MeshFace                                + ReadModel->MeshFaceNum                ) ;
			MBase->MeshPosition                  = ( MV1_MESH_POSITION        * )( MBase->MeshNormal                              + ReadModel->MeshNormalNum              ) ;
			MBase->MeshVertex                    = ( MV1_MESH_VERTEX          * )( ( BYTE * )MBase->MeshPosition                  + ReadModel->MeshPositionSize           ) ;
			MBase->ShapeVertex                   = ( MV1_SHAPE_VERTEX_BASE    * )( ( BYTE * )MBase->MeshVertex                    + ReadModel->MeshVertexSize             ) ;
		}

		// ���f�����ƃt�@�C���p�X�ƃf�B���N�g���p�X��ۑ�
		_WCSCPY( MBase->Name,          ReadModel->Name ) ;
		_WCSCPY( MBase->FilePath,      ReadModel->FilePath ) ;
		_WCSCPY( MBase->DirectoryPath, CurrentDir == NULL ? L"" : DirectoryPath ) ;

		// ���������`��ɑΉ����邩�ǂ�����ۑ�
		MBase->UsePackDraw = GParam->LoadModelToUsePackDraw ;

		// �E����W�n���ǂ�����ۑ�
		MBase->RightHandType = FALSE ;

		// ���蓖�Ă��Ă���}�e���A���̔ԍ����Ⴂ���b�V������`�悷�邩�ǂ�����ۑ�
		MBase->MaterialNumberOrderDraw = ReadModel->MaterialNumberOrderDraw ;

		// �d�̓p�����[�^��ۑ�
		if( GParam->LoadModelToWorldGravityInitialize == FALSE )
		{
			MBase->PhysicsGravity = -9.8f * 12.5f ;
		}
		else
		{
			MBase->PhysicsGravity = GParam->LoadModelToWorldGravity ;
		}

		// �|���S���̐���ۑ�
		MBase->TriangleNum = ( int )ReadModel->TriangleNum ;

		// �f�[�^���Z�b�g
		{
			// �v�f���S���P�̃J���[���쐬����
			OneColor.r = 255 ;
			OneColor.g = 255 ;
			OneColor.b = 255 ;
			OneColor.a = 255 ;

			// �A�j���[�V����������
			AnimSet = ReadModel->AnimSetFirst ;
			MBAnimSet = MBase->AnimSet ;
			MBase->AnimSetNum = ( int )ReadModel->AnimSetNum ;
			for( i = 0 ; i < ( int )ReadModel->AnimSetNum ; i ++, AnimSet = AnimSet->DataNext, MBAnimSet ++ )
			{
				// ���O�̃R�s�[
				MBAnimSet->NameAllocMem = FALSE ;
#ifndef UNICODE
				MBAnimSet->NameA = MV1RGetStringSpace(  MBase, AnimSet->NameA ) ;
#endif
				MBAnimSet->NameW = MV1RGetStringSpaceW( MBase, AnimSet->NameW ) ;

				// ���[�v�A�j���[�V�������ǂ������Z�b�g
				MBAnimSet->IsLoopAnim = AnimSet->IsLoopAnim ;

				// ���Z�A�j���[�V�������ǂ������Z�b�g
				MBAnimSet->IsAddAnim = AnimSet->IsAddAnim ;

				// �L�[���m�̕�Ԃ͍s��̐��`��Ԃ��ǂ������Z�b�g
				MBAnimSet->IsMatrixLinearBlend = AnimSet->IsMatrixLinearBlend ;

				// �C���f�b�N�X�̃Z�b�g
				MBAnimSet->Index = AnimSet->Index ;

				// �A�j���[�V�����̒������Z�b�g
				MBAnimSet->MaxTime = AnimSet->EndTime ;

				// �A�j���[�V�����̐����Z�b�g
				MBAnimSet->AnimNum = AnimSet->AnimNum ;

				// �A�j���[�V�����̐������J��Ԃ�
				MBAnimSet->Anim = MBase->Anim + MBase->AnimNum ;
				MBase->AnimNum += MBAnimSet->AnimNum ;
				MBAnim = MBAnimSet->Anim ;
				Anim = AnimSet->AnimFirst ;
				for( j = 0 ; j < AnimSet->AnimNum ; j ++, Anim = Anim->Next, MBAnim ++ )
				{
					// ���̃A�j���[�V�����f�[�^�����L����A�j���[�V�����Z�b�g�f�[�^�ւ̃|�C���^���Z�b�g
					MBAnim->Container = MBAnimSet ;

					// �ΏۂƂȂ�t���[���̖��O���Z�b�g
					MBAnim->TargetFrame = MBase->Frame + Anim->TargetFrameIndex ;

					// �ΏۂƂȂ�t���[���̃C���f�b�N�X���Z�b�g
					MBAnim->TargetFrameIndex = Anim->TargetFrameIndex ;

					// �A�j���[�V�����ƃt���[���̑Ή��e�[�u���ɃA�h���X���Z�b�g
					MBase->AnimTargetFrameTable[ ReadModel->FrameNum * i + Anim->TargetFrameIndex ] = MBAnim ;

					// �e�L�[�Z�b�g�̒��ň�Ԓ������Ԓl�̃Z�b�g
					MBAnim->MaxTime = Anim->MaxTime ;

					// ��]�I�[�_�[��ۑ�
					MBAnim->RotateOrder = Anim->RotateOrder ;

					// �L�[�Z�b�g�ւ̃|�C���^���Z�b�g
					MBAnim->KeySetNum = Anim->AnimKeySetNum ;
					MBAnim->KeySet = MBase->AnimKeySet + MBase->AnimKeySetNum ;
					MBase->AnimKeySetNum += Anim->AnimKeySetNum ;
					MBKeySet = MBAnim->KeySet ;

					// �L�[�Z�b�g�̐������J��Ԃ�
					AnimKeySet = Anim->AnimKeySetFirst ;
					for( k = 0 ; k < Anim->AnimKeySetNum ; k ++, MBKeySet ++, AnimKeySet = AnimKeySet->Next )
					{
						// �L�[�^�C�v���Z�b�g
						MBKeySet->Type = AnimKeySet->Type ;
						MBKeySet->DataType = AnimKeySet->DataType ;

						// �V�F�C�v�^�[�Q�b�g���Z�b�g
						MBKeySet->TargetShapeIndex = AnimKeySet->TargetShapeIndex ;

						// �^�C���^�C�v��ۑ�
						MBKeySet->TimeType = AnimKeySet->TimeType ;

						// �P�ʎ��ԂƊJ�n���Ԃ�ۑ�
						if( MBKeySet->TimeType == MV1_ANIMKEY_TIME_TYPE_ONE )
						{
							MBKeySet->UnitTime = AnimKeySet->UnitTime ;
							MBKeySet->StartTime = AnimKeySet->StartTime ;
						}

						// �����Ԃ��Z�b�g
//						MBKeySet->TotalTime = AnimKeySet->TotalTime ;

						// �L�[�̐����Z�b�g
						MBKeySet->Num = AnimKeySet->Num ;

						// �v�f���̃R�s�[
						MBKeySet->KeyFloat4 = ( FLOAT4 * )( ( BYTE * )MBase->AnimKeyData + MBase->AnimKeyDataSize ) ;

						// �v�f�^�C�v�ɂ���ăR�s�[�T�C�Y�̕ύX
						Size = 0 ;
						switch( MBKeySet->Type )
						{
						case MV1_ANIMKEY_TYPE_QUATERNION_X :   Size = sizeof( FLOAT4 ) ;                  break ;
						case MV1_ANIMKEY_TYPE_QUATERNION_VMD : Size = sizeof( FLOAT4 ) ;                  break ;
						case MV1_ANIMKEY_TYPE_VECTOR :         Size = sizeof( VECTOR ) ;                  break ;
						case MV1_ANIMKEY_TYPE_MATRIX3X3 :      Size = sizeof( MV1_ANIM_KEY_MATRIX3X3 ) ;  break ;
						case MV1_ANIMKEY_TYPE_MATRIX4X4C :     Size = sizeof( MV1_ANIM_KEY_MATRIX4X4C ) ; break ;

						case MV1_ANIMKEY_TYPE_FLAT :
						case MV1_ANIMKEY_TYPE_LINEAR :
						case MV1_ANIMKEY_TYPE_BLEND :          Size = sizeof( float ) ;                   break ;
						}

						// �L�[�T�C�Y�̕ۑ�
//						MBKeySet->UnitSize = Size ;

						// �]��
						if( GParam->LoadModelToIgnoreScaling &&
							( MBKeySet->DataType == MV1_ANIMKEY_DATATYPE_SCALE ||
							  MBKeySet->DataType == MV1_ANIMKEY_DATATYPE_SCALE_X ||
							  MBKeySet->DataType == MV1_ANIMKEY_DATATYPE_SCALE_Y ||
							  MBKeySet->DataType == MV1_ANIMKEY_DATATYPE_SCALE_Z ) )
						{
							switch( MBKeySet->Type )
							{
							case MV1_ANIMKEY_TYPE_VECTOR :
								for( l = 0; l < AnimKeySet->Num; l++ )
								{
									MBKeySet->KeyVector[ l ] = VGet( 1.0f, 1.0f, 1.0f ) ;
								}
								break ;

							case MV1_ANIMKEY_TYPE_FLAT :
							case MV1_ANIMKEY_TYPE_LINEAR :
							case MV1_ANIMKEY_TYPE_BLEND :
								for( l = 0; l < AnimKeySet->Num; l++ )
								{
									MBKeySet->KeyLinear[ l ] = 1.0f ;
								}
								break ;
							}
						}
						else
						{
							_MEMCPY( MBKeySet->KeyFloat4, AnimKeySet->KeyFloat4, ( size_t )( Size * MBKeySet->Num ) ) ;
						}
						MBase->AnimKeyDataSize += Size * MBKeySet->Num ;
						MBAnimSet->KeyDataSize += Size * MBKeySet->Num ;

						// �L�[�^�C���f�[�^�̕ۑ�
						if( MBKeySet->TimeType == MV1_ANIMKEY_TIME_TYPE_KEY )
						{
							MBKeySet->KeyTime = ( float * )( ( BYTE * )MBase->AnimKeyData + MBase->AnimKeyDataSize ) ;
							_MEMCPY( MBKeySet->KeyTime, AnimKeySet->KeyTime, sizeof( float ) * MBKeySet->Num ) ;
							MBase->AnimKeyDataSize += sizeof( float ) * MBKeySet->Num ;
							MBAnimSet->KeyDataSize += sizeof( float ) * MBKeySet->Num ;
						}
					}
				}
			}

			// �e�N�X�`���f�[�^���\�z����
			MBase->TextureAllocMem = FALSE ;
			Texture = ReadModel->TextureFirst ;
			for( i = 0 ; i < ( int )ReadModel->TextureNum ; i ++, Texture = Texture->DataNext )
			{
				// �A�h���X���Z�b�g
				MBTexture = MBase->Texture + MBase->TextureNum ;
				MBase->TextureNum ++ ;

				// ���]�t���O���Z�b�g����
				MBTexture->ReverseFlag = Texture->ReverseFlag ;

				// �R�Q�r�b�g�a�l�o�̃A���t�@�l���S���O�������� XRGB8 �Ƃ��Ĉ������ǂ����̃t���O���Z�b�g����
				MBTexture->Bmp32AllZeroAlphaToXRGB8Flag = Texture->Bmp32AllZeroAlphaToXRGB8Flag ;

				// ���[�U�[�f�[�^��������
				MBTexture->UserData[ 0 ] = 0 ;
				MBTexture->UserData[ 1 ] = 0 ;
				MBTexture->UseUserGraphHandle = 0 ;
				MBTexture->UserGraphHandle = 0 ;

				// ���O��ۑ�
				MBTexture->NameAllocMem = FALSE ;
#ifndef UNICODE
				MBTexture->NameA = MV1RGetStringSpace(  MBase, Texture->NameA ) ;
#endif
				MBTexture->NameW = MV1RGetStringSpaceW( MBase, Texture->NameW ) ;

				// �e�N�X�`���̓ǂݍ���
				{
					wchar_t ColorPathW[ FILEPATH_MAX ] ;
					wchar_t AlphaPathW[ FILEPATH_MAX ] ;
					wchar_t *UseColorPath = NULL ;
					wchar_t *UseAlphaPath = NULL ;

					if( Texture->ColorFileNameW )
					{
						if( CurrentDir == NULL )
						{
							UseColorPath = Texture->ColorFileNameW ;
						}
						else
						{
							ConvertFullPathW_( Texture->ColorFileNameW, ColorPathW, sizeof( ColorPathW ), DirectoryPath ) ;
							UseColorPath = ColorPathW ;
						}
					}

					if( Texture->AlphaFileNameW )
					{
						if( CurrentDir == NULL )
						{
							UseAlphaPath = Texture->AlphaFileNameW ;
						}
						else
						{
							ConvertFullPathW_( Texture->AlphaFileNameW, AlphaPathW, sizeof( AlphaPathW ), DirectoryPath ) ;
							UseAlphaPath = AlphaPathW ;
						}
					}

					if( __MV1LoadTexture(
							&MBTexture->ColorImage, &MBTexture->ColorImageSize,
							&MBTexture->AlphaImage, &MBTexture->AlphaImageSize,
							&MBTexture->GraphHandle,
							&MBTexture->SemiTransFlag,
							&MBTexture->IsDefaultTexture,
#ifndef UNICODE
							NULL, NULL,
#endif
							NULL, NULL,
							Texture->ColorFileNameW ? UseColorPath : NULL,
							Texture->AlphaFileNameW ? UseAlphaPath : NULL,
							CurrentDir == NULL ? NULL : DirectoryPath,
							Texture->BumpMapFlag, Texture->BumpMapNextPixelLength,
							Texture->ReverseFlag,
							Texture->Bmp32AllZeroAlphaToXRGB8Flag,
							ReadFunc,
							false,
							TRUE,
							ASyncThread ) == -1 )
					{
						DXST_ERRORLOGFMT_ADDW(( L"Read Model Convert Error : Texture Load Error : %s\n", Texture->NameW ) ) ;
						goto ERRORLABEL ;
					}
				}

				// �t�@�C�����p�Ƀ��������m�ۂ����t���O��|��
				MBTexture->ColorImageFilePathAllocMem = FALSE ;
				MBTexture->AlphaImageFilePathAllocMem = FALSE ;

				// �t�@�C���p�X��ۑ�
				if( MBTexture->ColorImage )
				{
#ifndef UNICODE
					MBTexture->ColorFilePathA = MV1RGetStringSpace(  MBase, Texture->ColorFileNameA ) ;
#endif
					MBTexture->ColorFilePathW = MV1RGetStringSpaceW( MBase, Texture->ColorFileNameW ) ;
				}
				if( MBTexture->AlphaImage && Texture->AlphaFileNameW )
				{
#ifndef UNICODE
					MBTexture->AlphaFilePathA = MV1RGetStringSpace(  MBase, Texture->AlphaFileNameA ) ;
#endif
					MBTexture->AlphaFilePathW = MV1RGetStringSpaceW( MBase, Texture->AlphaFileNameW ) ;
				}

				// �A�h���b�V���O���[�h�̃Z�b�g
				MBTexture->AddressModeU = Texture->AddressModeU ;
				MBTexture->AddressModeV = Texture->AddressModeV ;

				// �e�N�X�`���̃X�P�[���l���Z�b�g
				MBTexture->ScaleU = Texture->ScaleU ;
				MBTexture->ScaleV = Texture->ScaleV ;

				// �t�B���^�����O���[�h�̃Z�b�g
				MBTexture->FilterMode = Texture->FilterMode ;

				// �o���v�}�b�v����ۑ�
				MBTexture->BumpImageFlag = Texture->BumpMapFlag ;
				MBTexture->BumpImageNextPixelLength = Texture->BumpMapNextPixelLength ;

				// �摜�̃T�C�Y���擾
				NS_GetGraphSize( MBTexture->GraphHandle, &MBTexture->Width, &MBTexture->Height ) ;
			}

			// �}�e���A��������
			Material = ReadModel->MaterialFirst ;
			for( i = 0 ; i < ( int )ReadModel->MaterialNum ; i ++, Material = Material->DataNext )
			{
				// �A�h���X���Z�b�g
				MBMaterial = MBase->Material + MBase->MaterialNum ;
				MBase->MaterialNum ++ ;

				// ���O��ۑ�
#ifndef UNICODE
				MBMaterial->NameA = MV1RGetStringSpace(  MBase, Material->NameA ) ;
#endif
				MBMaterial->NameW = MV1RGetStringSpaceW( MBase, Material->NameW ) ;

				// �p�����[�^�̃R�s�[
				MBMaterial->Type = Material->Type ;

				MBMaterial->Diffuse  = Material->Diffuse ;
				MBMaterial->Ambient  = Material->Ambient ;
				MBMaterial->Specular = Material->Specular ;
				MBMaterial->Emissive = Material->Emissive ;
				MBMaterial->Power    = Material->Power ;
//				if( ( MBMaterial->Specular.r >= 0.0000001f || 
//					  MBMaterial->Specular.g >= 0.0000001f ||
//					  MBMaterial->Specular.b >= 0.0000001f ||
//					  MBMaterial->Specular.a >= 0.0000001f ) && MBMaterial->Power <= 1.0f ) MBMaterial->Power = 1.0f ;
				MBMaterial->DrawBlendMode = Material->DrawBlendMode ;
				MBMaterial->DrawBlendParam = Material->DrawBlendParam ;

				// �e�N�X�`��������ꍇ�̓e�N�X�`����ǂݍ���
				MBMaterial->DiffuseLayerNum  = Material->DiffuseTexNum ;
				MBMaterial->SpecularLayerNum = Material->SpecularTexNum ;
				MBMaterial->NormalLayerNum   = Material->NormalTexNum ;
				for( k = 0 ; k < 3 ; k ++ )
				{
					switch( k )
					{
					case 0 : num = MBMaterial->DiffuseLayerNum ;  Textures = Material->DiffuseTexs ;  MBMaterialLayer = MBMaterial->DiffuseLayer ;  break ;
					case 1 : num = MBMaterial->SpecularLayerNum ; Textures = Material->SpecularTexs ; MBMaterialLayer = MBMaterial->SpecularLayer ; break ;
					case 2 : num = MBMaterial->NormalLayerNum ;   Textures = Material->NormalTexs ;   MBMaterialLayer = MBMaterial->NormalLayer ;   break ;
					}

					for( j = 0 ; j < num ; j ++ )
					{
						if( Textures[ j ] == NULL ) continue ;
						Texture = Textures[ j ] ;

						// �u�����h�^�C�v�̃Z�b�g
						MBMaterialLayer[ j ].BlendType = Texture->BlendType ;

						// �e�N�X�`���C���f�b�N�X�̕ۑ�
						MBMaterialLayer[ j ].Texture = Texture->Index ;
					}
				}

				MBMaterial->DiffuseGradTexture    = Material->DiffuseGradTexture  ? Material->DiffuseGradTexture->Index  : Material->DiffuseGradTextureDefault ;
				MBMaterial->SpecularGradTexture   = Material->SpecularGradTexture ? Material->SpecularGradTexture->Index : Material->SpecularGradTextureDefault ;
				MBMaterial->SphereMapTexture      = Material->SphereMapTexture    ? Material->SphereMapTexture->Index    : -1 ;
				MBMaterial->DiffuseGradBlendType  = Material->DiffuseGradBlendType ;
				MBMaterial->SpecularGradBlendType = Material->SpecularGradBlendType ;
				MBMaterial->SphereMapBlendType    = Material->SphereMapBlendType ;
				MBMaterial->OutLineWidth          = Material->OutLineWidth ;
				MBMaterial->OutLineDotWidth	      = Material->OutLineDotWidth ;
				MBMaterial->OutLineColor          = Material->OutLineColor ;
			}

			// �t���[��������
			Frame = ReadModel->FrameFirst ;
			MBase->ChangeDrawMaterialTableSize = 0 ;
			MBase->ChangeMatrixTableSize       = 0 ;
			FrameMeshCounter = 0 ;
			for( r = 0 ; r < ( int )ReadModel->FrameNum ; r ++, Frame = Frame->DataNext )
			{
				DWORD TypeNumVertexDataSize[ MV1_VERTEX_TYPE_NUM ] ;
				DWORD TypeNumIndexDataSize ;
				MATRIX TempMatrix ;

				// ��{�f�[�^�Ƀt���[����ǉ�
				MBFrame = MBase->Frame + MBase->FrameNum ;

				// �����@���v�Z�p�̊p�x臒l��ۑ�
				MBFrame->SmoothingAngle = Frame->SmoothingAngle ;
				MBFrame->AutoCreateNormal = ReadModel->AutoCreateNormal ;

				// �t���[���̎�������Z�b�g
				MBFrame->Container = MBase ;

				// �t���[���̃A�h���X���Z�b�g
				Frame->MV1Frame = MBFrame ;

				// ���O���R�s�[
#ifndef UNICODE
				MBFrame->NameA = MV1RGetStringSpace(  MBase, Frame->NameA ) ;
#endif
				MBFrame->NameW = MV1RGetStringSpaceW( MBase, Frame->NameW ) ;

				// �e�ւ̃A�h���X���Z�b�g
				if( Frame->Parent )
				{
					MBFrame->Parent = Frame->Parent->MV1Frame ;

					// �e����̃����N��ǉ�
					if( MBFrame->Parent->FirstChild == NULL )
					{
						MBFrame->Parent->FirstChild = MBFrame ;
						MBFrame->Parent->LastChild  = MBFrame ;
					}
					else
					{
						MBFrame->Prev = MBFrame->Parent->LastChild->Next ;
						MBFrame->Parent->LastChild->Next = MBFrame ;
						MBFrame->Parent->LastChild = MBFrame ;
					}
				}
				else
				{
					if( MBase->FirstTopFrame == NULL )
					{
						MBase->FirstTopFrame = MBFrame ;
						MBase->LastTopFrame  = MBFrame ;
					}
					else
					{
						MBFrame->Prev = MBase->LastTopFrame->Next ;
						MBase->LastTopFrame->Next = MBFrame ;
						MBase->LastTopFrame = MBFrame ;
					}
					MBase->TopFrameNum ++ ;
				}

/*
				if( _STRCMP( MBFrame->Name, "Bone6_Parent_" ) == 0 )
				{
					MBFrame->Name[ 0 ] = MBFrame->Name[ 0 ] ;
				}
*/
				// ���W�ϊ��p�����[�^���Z�b�g
				{
					float One ;
					MATRIX ScaleMatrix, RotateMatrix ;
					VECTOR ScaleVector ;

					TempMatrix = Frame->Matrix ;
					One = 1.0f ;
					MBFrame->Translate   = Frame->Translate ;
					if( GParam->LoadModelToIgnoreScaling )
					{
						MBFrame->Scale   = VGet( 1.0f, 1.0f, 1.0f ) ;
					}
					else
					{
						MBFrame->Scale   = Frame->Scale ;
					}
					MBFrame->Rotate      = Frame->Rotate ;
					MBFrame->RotateOrder = Frame->RotateOrder ;
					MBFrame->Quaternion  = Frame->Quaternion ;

					MBFrame->PreRotate   = Frame->PreRotate ;
					MBFrame->PostRotate  = Frame->PostRotate ;

					// ���s�ړ��v�f���s��Ɏd���܂�Ă�����s�񂩂畽�s�ړ��v�f���擾����
					if( ( *( ( DWORD * )&TempMatrix.m[ 3 ][ 0 ] ) & 0x7fffffff ) != 0 ||
						( *( ( DWORD * )&TempMatrix.m[ 3 ][ 1 ] ) & 0x7fffffff ) != 0 ||
						( *( ( DWORD * )&TempMatrix.m[ 3 ][ 2 ] ) & 0x7fffffff ) != 0 )
					{
						MBFrame->Translate.x = TempMatrix.m[ 3 ][ 0 ] ;
						MBFrame->Translate.y = TempMatrix.m[ 3 ][ 1 ] ;
						MBFrame->Translate.z = TempMatrix.m[ 3 ][ 2 ] ;
						TempMatrix.m[ 3 ][ 0 ] = 0.0f ;
						TempMatrix.m[ 3 ][ 1 ] = 0.0f ;
						TempMatrix.m[ 3 ][ 2 ] = 0.0f ;
					}

					// ��]�v�f���s��Ɏd���܂�Ă�����s�񂩂��]�v�f�𒊏o����
					ScaleVector.x = _SQRT( TempMatrix.m[ 0 ][ 0 ] * TempMatrix.m[ 0 ][ 0 ] + 
									       TempMatrix.m[ 0 ][ 1 ] * TempMatrix.m[ 0 ][ 1 ] + 
									       TempMatrix.m[ 0 ][ 2 ] * TempMatrix.m[ 0 ][ 2 ] ) ;
					ScaleVector.y = _SQRT( TempMatrix.m[ 1 ][ 0 ] * TempMatrix.m[ 1 ][ 0 ] + 
									       TempMatrix.m[ 1 ][ 1 ] * TempMatrix.m[ 1 ][ 1 ] + 
									       TempMatrix.m[ 1 ][ 2 ] * TempMatrix.m[ 1 ][ 2 ] ) ;
					ScaleVector.z = _SQRT( TempMatrix.m[ 2 ][ 0 ] * TempMatrix.m[ 2 ][ 0 ] + 
									       TempMatrix.m[ 2 ][ 1 ] * TempMatrix.m[ 2 ][ 1 ] + 
									       TempMatrix.m[ 2 ][ 2 ] * TempMatrix.m[ 2 ][ 2 ] ) ;
					RotateMatrix.m[ 0 ][ 0 ] = TempMatrix.m[ 0 ][ 0 ] / ScaleVector.x ;
					RotateMatrix.m[ 0 ][ 1 ] = TempMatrix.m[ 0 ][ 1 ] / ScaleVector.x ;
					RotateMatrix.m[ 0 ][ 2 ] = TempMatrix.m[ 0 ][ 2 ] / ScaleVector.x ;
					RotateMatrix.m[ 1 ][ 0 ] = TempMatrix.m[ 1 ][ 0 ] / ScaleVector.y ;
					RotateMatrix.m[ 1 ][ 1 ] = TempMatrix.m[ 1 ][ 1 ] / ScaleVector.y ;
					RotateMatrix.m[ 1 ][ 2 ] = TempMatrix.m[ 1 ][ 2 ] / ScaleVector.y ;
					RotateMatrix.m[ 2 ][ 0 ] = TempMatrix.m[ 2 ][ 0 ] / ScaleVector.z ;
					RotateMatrix.m[ 2 ][ 1 ] = TempMatrix.m[ 2 ][ 1 ] / ScaleVector.z ;
					RotateMatrix.m[ 2 ][ 2 ] = TempMatrix.m[ 2 ][ 2 ] / ScaleVector.z ;
					if( *( ( DWORD * )&RotateMatrix.m[ 0 ][ 0 ] ) != *( ( DWORD * )&One ) ||
						*( ( DWORD * )&RotateMatrix.m[ 1 ][ 1 ] ) != *( ( DWORD * )&One ) ||
						*( ( DWORD * )&RotateMatrix.m[ 2 ][ 2 ] ) != *( ( DWORD * )&One ) ||
						( *( ( DWORD * )&RotateMatrix.m[ 0 ][ 1 ] ) & 0x7fffffff ) != 0 ||
						( *( ( DWORD * )&RotateMatrix.m[ 0 ][ 2 ] ) & 0x7fffffff ) != 0 ||
						( *( ( DWORD * )&RotateMatrix.m[ 1 ][ 0 ] ) & 0x7fffffff ) != 0 ||
						( *( ( DWORD * )&RotateMatrix.m[ 1 ][ 2 ] ) & 0x7fffffff ) != 0 ||
						( *( ( DWORD * )&RotateMatrix.m[ 2 ][ 0 ] ) & 0x7fffffff ) != 0 ||
						( *( ( DWORD * )&RotateMatrix.m[ 2 ][ 1 ] ) & 0x7fffffff ) != 0 )
					{
						MATRIX InvRotateMat ;
						bool MatchFlag ;

						MatchFlag = true ;
						do
						{
							GetMatrixXYZRotation( &RotateMatrix, &MBFrame->Rotate.x, &MBFrame->Rotate.y, &MBFrame->Rotate.z ) ;
							CreateRotationXYZMatrix( &InvRotateMat, MBFrame->Rotate.x, MBFrame->Rotate.y, MBFrame->Rotate.z ) ;
							if( MatrixRotateCmp( &InvRotateMat, &RotateMatrix, 0.00001f ) )
							{
								MBFrame->RotateOrder = MV1_ROTATE_ORDER_XYZ ;
								break ;
							}

							GetMatrixXZYRotation( &RotateMatrix, &MBFrame->Rotate.x, &MBFrame->Rotate.y, &MBFrame->Rotate.z ) ;
							CreateRotationXZYMatrix( &InvRotateMat, MBFrame->Rotate.x, MBFrame->Rotate.y, MBFrame->Rotate.z ) ;
							if( MatrixRotateCmp( &InvRotateMat, &RotateMatrix, 0.00001f ) )
							{
								MBFrame->RotateOrder = MV1_ROTATE_ORDER_XZY ;
								break ;
							}

							GetMatrixYXZRotation( &RotateMatrix, &MBFrame->Rotate.x, &MBFrame->Rotate.y, &MBFrame->Rotate.z ) ;
							CreateRotationYXZMatrix( &InvRotateMat, MBFrame->Rotate.x, MBFrame->Rotate.y, MBFrame->Rotate.z ) ;
							if( MatrixRotateCmp( &InvRotateMat, &RotateMatrix, 0.00001f ) )
							{
								MBFrame->RotateOrder = MV1_ROTATE_ORDER_YXZ ;
								break ;
							}

							GetMatrixYZXRotation( &RotateMatrix, &MBFrame->Rotate.x, &MBFrame->Rotate.y, &MBFrame->Rotate.z ) ;
							CreateRotationYZXMatrix( &InvRotateMat, MBFrame->Rotate.x, MBFrame->Rotate.y, MBFrame->Rotate.z ) ;
							if( MatrixRotateCmp( &InvRotateMat, &RotateMatrix, 0.00001f ) )
							{
								MBFrame->RotateOrder = MV1_ROTATE_ORDER_YZX ;
								break ;
							}

							GetMatrixZXYRotation( &RotateMatrix, &MBFrame->Rotate.x, &MBFrame->Rotate.y, &MBFrame->Rotate.z ) ;
							CreateRotationZXYMatrix( &InvRotateMat, MBFrame->Rotate.x, MBFrame->Rotate.y, MBFrame->Rotate.z ) ;
							if( MatrixRotateCmp( &InvRotateMat, &RotateMatrix, 0.00001f ) )
							{
								MBFrame->RotateOrder = MV1_ROTATE_ORDER_ZXY ;
								break ;
							}

							GetMatrixZYXRotation( &RotateMatrix, &MBFrame->Rotate.x, &MBFrame->Rotate.y, &MBFrame->Rotate.z ) ;
							CreateRotationZYXMatrix( &InvRotateMat, MBFrame->Rotate.x, MBFrame->Rotate.y, MBFrame->Rotate.z ) ;
							if( MatrixRotateCmp( &InvRotateMat, &RotateMatrix, 0.00001f ) )
							{
								MBFrame->RotateOrder = MV1_ROTATE_ORDER_ZYX ;
								break ;
							}
							GetMatrixXYZRotation( &RotateMatrix, &MBFrame->Rotate.x, &MBFrame->Rotate.y, &MBFrame->Rotate.z ) ;
							CreateRotationXYZMatrix( &InvRotateMat, MBFrame->Rotate.x, MBFrame->Rotate.y, MBFrame->Rotate.z ) ;
							MBFrame->RotateOrder = MV1_ROTATE_ORDER_XYZ ;
//							MBFrame->RotateOrder = 0 ;
//							ScaleMatrix = TempMatrix ;
//							MatchFlag = false ;
						}
						while( 0 ) ;

						if( MatchFlag )
						{
							CreateInverseMatrix( &InvRotateMat, &InvRotateMat ) ;
							CreateMultiplyMatrix( &ScaleMatrix, &TempMatrix, &InvRotateMat ) ;
						}
					}
					else
					{
						ScaleMatrix = TempMatrix ;
					}

					// �g��v�f���s��Ɏd���܂�Ă�����s�񂩂�g��v�f�𒊏o����
					if( GParam->LoadModelToIgnoreScaling == FALSE )
					{
						ScaleVector.x = ScaleMatrix.m[ 0 ][ 0 ] ;
						ScaleVector.y = ScaleMatrix.m[ 1 ][ 1 ] ;
						ScaleVector.z = ScaleMatrix.m[ 2 ][ 2 ] ;
						if( ScaleVector.x < 0.99999f || ScaleVector.x > 1.00001f ||
							ScaleVector.y < 0.99999f || ScaleVector.y > 1.00001f ||
							ScaleVector.z < 0.99999f || ScaleVector.z > 1.00001f )
						{
							MBFrame->Scale.x = ScaleVector.x ;
							MBFrame->Scale.y = ScaleVector.y ;
							MBFrame->Scale.z = ScaleVector.z ;
						}
					}
				}

				// �q�t���[���̑����A���b�V���̑������Z�b�g
				MBFrame->TotalChildNum = Frame->TotalChildNum ;
				MBFrame->TotalMeshNum  = Frame->TotalMeshNum ;

				// ��ԕω��Ǘ��p��{���̃Z�b�g
				MV1ChangeInfoSetup( &MBFrame->ChangeDrawMaterialInfo, ( BYTE * )MBase->ChangeDrawMaterialTable + MBase->ChangeDrawMaterialTableSize, FrameMeshCounter + 1, MBFrame->TotalChildNum + MBFrame->TotalMeshNum + 1 ) ;
				MBase->ChangeDrawMaterialTableSize += MBFrame->ChangeDrawMaterialInfo.Size * 4 ;

				MV1ChangeInfoSetup( &MBFrame->ChangeMatrixInfo,       ( BYTE * )MBase->ChangeMatrixTable       + MBase->ChangeMatrixTableSize,       MBase->FrameNum + 1,  MBFrame->TotalChildNum + 1 ) ;
				MBase->ChangeMatrixTableSize += MBFrame->ChangeMatrixInfo.Size * 4 ;

				// �t���[���̃C���f�b�N�X���Z�b�g
				MBFrame->Index = MBase->FrameNum ;
				MBase->FrameNum ++ ;
				FrameMeshCounter ++ ;

				// �ő�{�[���u�����h���ƂP���_�ӂ�̃T�C�Y���Z�b�g����
				MBFrame->MaxBoneBlendNum = ( int )Frame->MaxBoneUseNum ;
				MBFrame->PosUnitSize     = ( int )( sizeof( MV1_MESH_POSITION ) + ( MBFrame->MaxBoneBlendNum - 4 ) * sizeof( MV1_SKINBONE_BLEND ) ) ;

				// �e��t���O���Z�b�g
				MBFrame->Flag = 0 ;
				if( Frame->Visible )
				{
					MBFrame->Flag |= MV1_FRAMEFLAG_VISIBLE ;
				}

				if( Frame->EnablePreRotate != 0 )
				{
					MBFrame->Flag |= MV1_FRAMEFLAG_PREROTATE ;
				}

				if( Frame->EnablePostRotate != 0 )
				{
					MBFrame->Flag |= MV1_FRAMEFLAG_POSTROTATE ;
				}

				// �e�̍s��𖳎����邩�ǂ�����ۑ�����
				MBFrame->IgnoreParentTransform = Frame->IgnoreParentTransform ;

				// �V�F�C�v�f�[�^�̃Z�b�g
				if( Frame->ShapeNum )
				{
					MBFrame->ShapeNum = Frame->ShapeNum ;
					MBFrame->Shape = MBase->Shape + MBase->ShapeNum ;
					MBase->ShapeNum += MBFrame->ShapeNum ;

					Shape = Frame->ShapeFirst ;
					MBShape = MBFrame->Shape ;
					for( i = 0 ; i < ( DWORD )Frame->ShapeNum ; i ++, Shape = Shape->Next, MBShape ++ )
					{
						MBShape->Container = MBFrame ;
#ifndef UNICODE
						MBShape->NameA = MV1RGetStringSpace(  MBase, Shape->NameA ) ;
#endif
						MBShape->NameW = MV1RGetStringSpaceW( MBase, Shape->NameW ) ;
						MBShape->Mesh = NULL ;
						MBShape->MeshNum = 0 ;
					}
				}

				// ���b�V�������݂���ꍇ�̓��b�V���̏����Z�b�g
				if( Frame->MeshNum && Frame->MeshFirst->Positions )
				{
					Mesh = Frame->MeshFirst ;
					for( i = 0 ; i < ( DWORD )Frame->MeshNum ; i ++, Mesh = Mesh->Next )
					{
						_MEMSET( TypeNumVertexDataSize, 0, sizeof( TypeNumVertexDataSize ) ) ;
						TypeNumIndexDataSize = 0 ;

						// �@���f�[�^���Z�b�g����
						{
							MV1_MESH_NORMAL *MBNormal ;
							VECTOR *Normal ;

							if( MBFrame->Normal == NULL )
							{
								MBFrame->Normal = MBase->MeshNormal + MBase->MeshNormalNum ;
								MBFrame->NormalNum = 0 ;
							}

							MBNormal = MBFrame->Normal + MBFrame->NormalNum ;
							Normal = Mesh->Normals ;
							for( j = 0 ; ( DWORD )j < Mesh->NormalNum ; j ++, MBNormal ++, Normal ++ )
							{
								MBNormal->Normal = *Normal ;
							}

							MBNormal = MBFrame->Normal + MBFrame->NormalNum ;
							if( Mesh->Binormals != NULL )
							{
								MBFrame->Flag |= MV1_FRAMEFLAG_TANGENT_BINORMAL ;

								Normal = Mesh->Binormals ;
								for( j = 0 ; ( DWORD )j < Mesh->NormalNum ; j ++, MBNormal ++, Normal ++ )
								{
									MBNormal->Binormal = *Normal ;
								}
							}
							else
							{
								for( j = 0 ; ( DWORD )j < Mesh->NormalNum ; j ++, MBNormal ++ )
								{
									MBNormal->Binormal.x = 0.0f ;
									MBNormal->Binormal.y = 0.0f ;
									MBNormal->Binormal.z = 0.0f ;
								}
							}

							MBNormal = MBFrame->Normal + MBFrame->NormalNum ;
							if( Mesh->Binormals != NULL )
							{
								MBFrame->Flag |= MV1_FRAMEFLAG_TANGENT_BINORMAL ;

								Normal = Mesh->Tangents ;
								for( j = 0 ; ( DWORD )j < Mesh->NormalNum ; j ++, MBNormal ++, Normal ++ )
								{
									MBNormal->Tangent = *Normal ;
								}
							}
							else
							{
								for( j = 0 ; ( DWORD )j < Mesh->NormalNum ; j ++, MBNormal ++ )
								{
									MBNormal->Tangent.x = 0.0f ;
									MBNormal->Tangent.y = 0.0f ;
									MBNormal->Tangent.z = 0.0f ;
								}
							}

							MeshNormalStartNum    = ( DWORD )MBFrame->NormalNum ;
							MBFrame->NormalNum   += Mesh->NormalNum ;
							MBase->MeshNormalNum += Mesh->NormalNum ;
						}

						// ���W�f�[�^���Z�b�g����
						{
							VECTOR *Position ;
							MV1_MESH_POSITION *MBPosition ;

							if( MBFrame->Position == NULL )
							{
								MBFrame->Position = ( MV1_MESH_POSITION * )( ( BYTE * )MBase->MeshPosition + MBase->MeshPositionSize ) ;
								MBFrame->PositionNum = 0 ;
							}

							MeshBoneStartNum = ( DWORD )MBFrame->UseSkinBoneNum ;
							MBPosition = ( MV1_MESH_POSITION * )( ( BYTE * )MBFrame->Position + MBFrame->PosUnitSize * MBFrame->PositionNum ) ;
							Position = Mesh->Positions ;
							for( j = 0 ; ( DWORD )j < Mesh->PositionNum ; j ++, MBPosition = ( MV1_MESH_POSITION * )( ( BYTE * )MBPosition + MBFrame->PosUnitSize ), Position ++ )
							{
								MBPosition->Position = *Position ;

								// �g�p����{�[���C���f�b�N�X�ƃE�G�C�g�l���Z�b�g
								if( MBFrame->MaxBoneBlendNum > 0 )
								{
									BoneNum = GetBitListNumber( &Mesh->SkinVerticesBlend, j, BitBuf ) ;
									VertB   = MBPosition->BoneWeight ;
									for( m = 0 ; m < BoneNum ; m ++, VertB ++ )
									{
										VertB->Index = ( int )( BitBuf[ m ] + MeshBoneStartNum ) ;
										VertB->W     = Mesh->SkinVerticeWeightInfo[ Mesh->SkinWeightsNum * j + BitBuf[ m ] ] ;
									}
									if( BoneNum != MBFrame->MaxBoneBlendNum )
									{
										VertB->Index = -1 ;
										VertB->W     = 0.0f ;
									}
								}
							}
							MeshPositionStartNum     = ( DWORD )MBFrame->PositionNum ;
							MBFrame->PositionNum    += Mesh->PositionNum ;
							MBase->MeshPositionSize += Mesh->PositionNum * MBFrame->PosUnitSize ;
						}

						// �X�L�j���O���b�V���̃E�G�C�g�̏����Z�b�g����
						StartMatrixIndex = MBase->SkinBoneNum ;
						MBSkinW = MBase->SkinBone + MBase->SkinBoneNum ;
						for( j = 0 ; ( DWORD )j < Mesh->SkinWeightsNum ; j ++, MBSkinW ++ )
						{
							MBSkinW->BoneFrame = ( int )Mesh->SkinWeights[ j ]->TargetFrame ;
							if( MBSkinW->BoneFrame == -1 )
							{
								char FrameNameUTF16LE[ 512 ] ;

								ConvString( ( const char * )Frame->NameW, WCHAR_T_CHARCODEFORMAT, FrameNameUTF16LE, sizeof( FrameNameUTF16LE ), DX_CHARCODEFORMAT_UTF16LE ) ;
								DXST_ERRORLOGFMT_ADDUTF16LE( ( "\x52\x00\x65\x00\x61\x00\x64\x00\x20\x00\x4d\x00\x6f\x00\x64\x00\x65\x00\x6c\x00\x20\x00\x43\x00\x6f\x00\x6e\x00\x76\x00\x65\x00\x72\x00\x74\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xd5\x30\xec\x30\xfc\x30\xe0\x30\x20\x00\x25\x00\x73\x00\x20\x00\x6e\x30\xb9\x30\xad\x30\xcb\x30\xf3\x30\xb0\x30\xe1\x30\xc3\x30\xb7\x30\xe5\x30\x6e\x30\xb9\x30\xad\x30\xf3\x30\xa6\x30\xa8\x30\xa4\x30\xc8\x30\xc5\x60\x31\x58\x67\x30\x00\x4e\xf4\x81\x59\x30\x8b\x30\xce\x30\xfc\x30\xc9\x30\x6e\x30\x21\x71\x44\x30\xea\x30\xf3\x30\xaf\x30\xc5\x60\x31\x58\x4c\x30\x42\x30\x8a\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Read Model Convert Error : �t���[�� %s �̃X�L�j���O���b�V���̃X�L���E�G�C�g���ň�v����m�[�h�̖��������N��񂪂���܂���\n" @*/, FrameNameUTF16LE ) ) ;
								goto ERRORLABEL ;
							}
							ConvertMatrixFToMatrix4x4cF( &MBSkinW->ModelLocalMatrix, &Mesh->SkinWeights[ j ]->ModelLocalMatrix ) ;
							if(                  MBSkinW->ModelLocalMatrix.m[ 0 ][ 0 ]                  == 1.0f &&
								( *( ( DWORD * )&MBSkinW->ModelLocalMatrix.m[ 1 ][ 0 ] ) & 0x7fffffff ) == 0 &&
								( *( ( DWORD * )&MBSkinW->ModelLocalMatrix.m[ 2 ][ 0 ] ) & 0x7fffffff ) == 0 &&
							    ( *( ( DWORD * )&MBSkinW->ModelLocalMatrix.m[ 0 ][ 1 ] ) & 0x7fffffff ) == 0 &&
								                 MBSkinW->ModelLocalMatrix.m[ 1 ][ 1 ]                  == 1.0f &&
								( *( ( DWORD * )&MBSkinW->ModelLocalMatrix.m[ 2 ][ 1 ] ) & 0x7fffffff ) == 0 &&
							    ( *( ( DWORD * )&MBSkinW->ModelLocalMatrix.m[ 0 ][ 2 ] ) & 0x7fffffff ) == 0 &&
								( *( ( DWORD * )&MBSkinW->ModelLocalMatrix.m[ 1 ][ 2 ] ) & 0x7fffffff ) == 0 &&
								                 MBSkinW->ModelLocalMatrix.m[ 2 ][ 2 ]                  == 1.0f )
							{
								MBSkinW->ModelLocalMatrixIsTranslateOnly = 1 ;
							}
							else
							{
								MBSkinW->ModelLocalMatrixIsTranslateOnly = 0 ;
							}

							MBSkinW->UseFrameNum = 1 ;
							MBSkinW->UseFrame = MBase->SkinBoneUseFrame + MBase->SkinBoneUseFrameNum ;
							MBSkinW->UseFrame[ 0 ].Index = MBFrame->Index ;
							MBase->SkinBoneUseFrameNum ++ ;

							if( MBFrame->UseSkinBone == NULL )
							{
								MBFrame->UseSkinBone = MBase->FrameUseSkinBone + MBase->FrameUseSkinBoneNum ;
								MBFrame->UseSkinBoneNum = 0 ;
							}
							MBFrame->UseSkinBone[ MBFrame->UseSkinBoneNum ] = MBSkinW ;
							MBFrame->UseSkinBoneNum ++ ;
							MBase->FrameUseSkinBoneNum ++ ;
						}
						MBase->SkinBoneNum += Mesh->SkinWeightsNum ;

						// �}�e���A�����X�g������ꍇ�̓��b�V���̐��̓}�e���A���̐�����
						// �����ꍇ�͈��
						if( MBFrame->Mesh == NULL )
						{
							MBFrame->Mesh = MBase->Mesh + MBase->MeshNum ;
						}
						MeshStartNum      = ( DWORD )MBFrame->MeshNum ;
						MeshNowNum        = Mesh->MaterialNum ? Mesh->MaterialNum : 1 ;
						MBFrame->MeshNum += MeshNowNum ;
						MBase->MeshNum   += MeshNowNum ;

						// �g���C�A���O�����X�g���쐬
						{
							// ���b�V���̐������J��Ԃ�
							MBMesh = MBFrame->Mesh + MeshStartNum ;
							for( o = 0 ; ( DWORD )o < MeshNowNum ; o ++, MBMesh ++ )
							{
								// �e�A�h���X��ۑ�
								MBMesh->Container = MBFrame ;

								// ��ԕω��Ǘ��p��{���̃Z�b�g
								MV1ChangeInfoSetup( &MBMesh->ChangeInfo, NULL, FrameMeshCounter + 1, 0 ) ;
								FrameMeshCounter ++ ;

								// �g���C�A���O�����X�g�̃A�h���X���Z�b�g
								MBMesh->TriangleListNum = 0 ;
								MBMesh->TriangleList = MBase->TriangleList + MBase->TriangleListNum ;

								// ��{�I�Ƀ��b�V���̏�����Ԃ͕\��
								MBMesh->Visible = 1 /*( MBFrame->Flag & MV1_FRAMEFLAG_VISIBLE ) != 0 ? 1 : 0*/ ;

								// ���_�̃f�B�t���[�Y�J���[�ƃX�y�L�����J���[���g�p���邩�ǂ������Z�b�g
								MBMesh->UseVertexDiffuseColor = FALSE ;
								MBMesh->UseVertexSpecularColor = FALSE ;

								// �}�e���A���̃A�h���X���Z�b�g
								if( Mesh->MaterialNum )
								{
									MBMesh->Material = MBase->Material + Mesh->Materials[ o ]->Index ;
								}

								// �o�b�N�J�����O�̗L���̃Z�b�g
								MBMesh->BackCulling = ( BYTE )( Mesh->Materials[ o ] == NULL || Mesh->Materials[ o ]->DisableBackCulling == FALSE ? TRUE : FALSE ) ;

								// ���_�f�[�^�Ɩʃf�[�^���\�z����
								{
									BYTE TVertBuf[ sizeof( MV1_MESH_VERTEX ) + sizeof( float ) * 2 * 32 ] ;
									MV1_MESH_VERTEX *TVertex ;

									TVertex = ( MV1_MESH_VERTEX * )TVertBuf ;

									// �t�u�̐����Z�b�g
									MBMesh->UVSetUnitNum = Mesh->Materials[ o ] == NULL || Mesh->Materials[ o ]->DiffuseTexNum == 0 ? 1 : Mesh->Materials[ o ]->DiffuseTexNum ;
									MBMesh->UVUnitNum = 2 ;

									// ���_�f�[�^�̐擪�A�h���X���Z�b�g
									MBMesh->Vertex       = ( MV1_MESH_VERTEX * )( ( BYTE * )MBase->MeshVertex + MBase->MeshVertexSize ) ;
									MBMesh->VertUnitSize = ( int )( sizeof( MV1_MESH_VERTEX ) + MBMesh->UVSetUnitNum * MBMesh->UVUnitNum * sizeof( float ) - sizeof( float ) * 2 ) ;

									// �ʃf�[�^�̃A�h���X���Z�b�g
									MBMesh->Face = MBase->MeshFace + MBase->MeshFaceNum ;

									// ���_�f�[�^�Ɩʃf�[�^���\�z
									MBMesh->VertexNum = 0 ;
									MeshFace = Mesh->Faces ;
									MBFace = MBMesh->Face ;
									_MEMSET( VertInfoTable, 0, sizeof( MV1_MAKEVERTINDEXINFO * ) * MaxPositionNum ) ;
									VertInfoNum = 0 ;
									for( j = 0 ; ( DWORD )j < Mesh->FaceNum ; j ++, MeshFace ++ )
									{
										// �g�p����}�e���A�����قȂ�ꍇ�͉������Ȃ�
										if( MeshFace->MaterialIndex != ( DWORD )o ) continue ;

										// �����E����W�n��������C���f�b�N�X�̏��Ԃ�ύX����
/*										if( ReadModel->MeshFaceRightHand )
										{
											p = MBFace->VertexIndex[ 1 ] ;
											MBFace->VertexIndex[ 1 ] = MBFace->VertexIndex[ 2 ] ;
											MBFace->VertexIndex[ 2 ] = p ;
										}*/

										// ���_�^�C�v�Ə����g���C�A���O�����X�g�ԍ���ۑ�
										MBFace->VertexType        = ( WORD )( Mesh->SkinFaceType        == NULL ? MV1_VERTEX_TYPE_NORMAL : ( WORD )Mesh->SkinFaceType[ j ]        ) ;
										MBFace->TriangleListIndex = ( WORD )( Mesh->FaceUseTriangleList == NULL ? -1                     : ( WORD )Mesh->FaceUseTriangleList[ j ] ) ;

										// �|���S���̒��_�̐������J��Ԃ�
										for( m = 0 ; m < 3 ; m ++ )
										{
											// �ǉ����悤�Ƃ��Ă��钸�_�f�[�^�̍쐬
											TVertex->ToonOutLineScale = Mesh->PositionToonOutLineScale == NULL ? 1.0f : Mesh->PositionToonOutLineScale[ MeshFace->VertexIndex[ m ] ] ;
											TVertex->PositionIndex = MeshFace->VertexIndex[ m ] + MeshPositionStartNum ;
											TVertex->NormalIndex = MeshFace->NormalIndex[ m ] + MeshNormalStartNum ;
											if( Mesh->VertexColors )
											{
												ColorF = &Mesh->VertexColors[ MeshFace->VertexColorIndex[ m ] ] ;
												TVertex->DiffuseColor.r = ( BYTE )( ColorF->r >= 1.0f ? 255 : ( ColorF->r < 0.0f ? 0 : _FTOL( ColorF->r * 255.0f ) ) ) ;
												TVertex->DiffuseColor.g = ( BYTE )( ColorF->g >= 1.0f ? 255 : ( ColorF->g < 0.0f ? 0 : _FTOL( ColorF->g * 255.0f ) ) ) ;
												TVertex->DiffuseColor.b = ( BYTE )( ColorF->b >= 1.0f ? 255 : ( ColorF->b < 0.0f ? 0 : _FTOL( ColorF->b * 255.0f ) ) ) ;
												TVertex->DiffuseColor.a = ( BYTE )( ColorF->a >= 1.0f ? 255 : ( ColorF->a < 0.0f ? 0 : _FTOL( ColorF->a * 255.0f ) ) ) ;
											}
											else
											{
												if( MBMesh->Material == NULL )
												{
													TVertex->DiffuseColor.r = 255 ;
													TVertex->DiffuseColor.g = 255 ;
													TVertex->DiffuseColor.b = 255 ;
													TVertex->DiffuseColor.a = 255 ;
												}
												else
												{
													TVertex->DiffuseColor.r = ( BYTE )( MBMesh->Material->Diffuse.r >= 1.0f ? 255 : ( MBMesh->Material->Diffuse.r < 0.0f ? 0 : _FTOL( MBMesh->Material->Diffuse.r * 255.0f ) ) ) ;
													TVertex->DiffuseColor.g = ( BYTE )( MBMesh->Material->Diffuse.g >= 1.0f ? 255 : ( MBMesh->Material->Diffuse.g < 0.0f ? 0 : _FTOL( MBMesh->Material->Diffuse.g * 255.0f ) ) ) ;
													TVertex->DiffuseColor.b = ( BYTE )( MBMesh->Material->Diffuse.b >= 1.0f ? 255 : ( MBMesh->Material->Diffuse.b < 0.0f ? 0 : _FTOL( MBMesh->Material->Diffuse.b * 255.0f ) ) ) ;
													TVertex->DiffuseColor.a = ( BYTE )( MBMesh->Material->Diffuse.a >= 1.0f ? 255 : ( MBMesh->Material->Diffuse.a < 0.0f ? 0 : _FTOL( MBMesh->Material->Diffuse.a * 255.0f ) ) ) ;
												}
											}
											if( TVertex->DiffuseColor.a != 255 ) MBMesh->NotOneDiffuseAlpha = TRUE ;

											if( MBMesh->Material == NULL )
											{
												TVertex->SpecularColor.r = 255 ;
												TVertex->SpecularColor.g = 255 ;
												TVertex->SpecularColor.b = 255 ;
												TVertex->SpecularColor.a = 255 ;
											}
											else
											{
												TVertex->SpecularColor.r = ( BYTE )( MBMesh->Material->Specular.r >= 1.0f ? 255 : ( MBMesh->Material->Specular.r < 0.0f ? 0 : _FTOL( MBMesh->Material->Specular.r * 255.0f ) ) ) ;
												TVertex->SpecularColor.g = ( BYTE )( MBMesh->Material->Specular.g >= 1.0f ? 255 : ( MBMesh->Material->Specular.g < 0.0f ? 0 : _FTOL( MBMesh->Material->Specular.g * 255.0f ) ) ) ;
												TVertex->SpecularColor.b = ( BYTE )( MBMesh->Material->Specular.b >= 1.0f ? 255 : ( MBMesh->Material->Specular.b < 0.0f ? 0 : _FTOL( MBMesh->Material->Specular.b * 255.0f ) ) ) ;
												TVertex->SpecularColor.a = ( BYTE )( MBMesh->Material->Specular.a >= 1.0f ? 255 : ( MBMesh->Material->Specular.a < 0.0f ? 0 : _FTOL( MBMesh->Material->Specular.a * 255.0f ) ) ) ;
											}

											for( s = 0 ; s < MBMesh->UVSetUnitNum ; s ++ )
											{
												if( Mesh->UVs[ s ] )
												{
													TVertex->UVs[ s ][ 0 ] = Mesh->UVs[ s ][ MeshFace->UVIndex[ s ][ m ] ].x ;
													TVertex->UVs[ s ][ 1 ] = Mesh->UVs[ s ][ MeshFace->UVIndex[ s ][ m ] ].y ;
												}
												else
												{
													TVertex->UVs[ s ][ 0 ] = 0.0f ;
													TVertex->UVs[ s ][ 1 ] = 0.0f ;
												}
											}

											// ���܂łɓ������_�f�[�^�������������ǂ����𒲂ׂ�
											for( VInfo = VertInfoTable[ MeshFace->VertexIndex[ m ] ] ; VInfo ; VInfo = VInfo->Next )
											{
												if( VInfo->TriangleListIndex == MBFace->TriangleListIndex &&
													_MEMCMP( ( BYTE * )MBMesh->Vertex + VInfo->VertexIndex * MBMesh->VertUnitSize, TVertex, MBMesh->VertUnitSize ) == 0 )
													break ;
											}
											if( VInfo == NULL )
											{
												// ����������f�[�^��ǉ�
												VInfo = &VertInfoBuffer[ VertInfoNum ] ;
												VertInfoNum ++ ;

												VInfo->TriangleListIndex = MBFace->TriangleListIndex ;
												VInfo->VertexIndex = MBMesh->VertexNum ;
												VInfo->Next = VertInfoTable[ MeshFace->VertexIndex[ m ] ] ;
												VertInfoTable[ MeshFace->VertexIndex[ m ] ] = VInfo ;

												_MEMCPY( ( BYTE * )MBMesh->Vertex + MBMesh->VertexNum * MBMesh->VertUnitSize, TVertex, ( size_t )MBMesh->VertUnitSize ) ;
												MBFace->VertexIndex[ m ] = ( DWORD )MBMesh->VertexNum ;
												MBMesh->VertexNum ++ ;
											}
											else
											{
												// ��������C���f�b�N�X���Z�b�g
												MBFace->VertexIndex[ m ] = ( DWORD )VInfo->VertexIndex ;
											}
										}

										// �ʂ̐����C���N�������g
										MBFace ++ ;
										MBMesh->FaceNum ++ ;
									}

									// �����E����W�n��������C���f�b�N�X�̏��Ԃ�ύX����
									if( ReadModel->MeshFaceRightHand )
									{
										MBFace = MBMesh->Face ;
										for( j = 0 ; j < MBMesh->FaceNum ; j ++, MBFace ++ )
										{
											p = ( int )MBFace->VertexIndex[ 1 ] ;
											MBFace->VertexIndex[ 1 ] = MBFace->VertexIndex[ 2 ] ;
											MBFace->VertexIndex[ 2 ] = ( DWORD )p ;
										}
									}

									// �ʂ̑����ƒ��_�f�[�^�̑��T�C�Y�����Z
									MBase->MeshFaceNum += MBMesh->FaceNum ;
									MBase->MeshVertexSize += MBMesh->VertexNum * MBMesh->VertUnitSize ;

									// �]�@���Ɛڐ����Z�o����
//									MV1MakeMeshBinormalsAndTangents( MBMesh ) ;
								}

								// �{�[����̃g���C�A���O�����X�g�̍쐬
								if( Mesh->MaterialPolyList[ o ].TypeNum[ MV1_VERTEX_TYPE_NORMAL ] )
								{
									// �g���C�A���O�����X�g�̃A�h���X���Z�b�g
									MBTList = MBMesh->TriangleList + MBMesh->TriangleListNum ;
									MBase->TriangleListNum  += Mesh->MaterialPolyList[ o ].SimpleTriangleListNum ;
									MBMesh->TriangleListNum += Mesh->MaterialPolyList[ o ].SimpleTriangleListNum ;

									// �g���C�A���O�����X�g�̐������J��Ԃ�
									l = 0 ;
									MBFace = MBMesh->Face ;
									for( j = 0 ; j < Mesh->MaterialPolyList[ o ].SimpleTriangleListNum ; j ++, MBTList ++ )
									{
										// �g���C�A���O�����X�g�̏����Z�b�g
										MBTList->Container = MBMesh ;
										MBTList->VertexType = MV1_VERTEX_TYPE_NORMAL ;

										// �P���_�̃T�C�Y���Z�b�g
										MBTList->PosUnitSize = sizeof( MV1_TLIST_NORMAL_POS ) ;

										// ���_�C���f�b�N�X�f�[�^�̐擪�A�h���X���Z�b�g
										MBTList->Index = MBase->TriangleListIndex + MBase->TriangleListIndexNum ;
										MBTList->IndexNum = 0 ;
										MBTList->ToonOutLineIndex = MBTList->Index + MTBase.TriangleListIndexNum ;
										MBTList->ToonOutLineIndexNum = 0 ;

										// ���_�f�[�^�̐擪�A�h���X���Z�b�g
										MBTList->NormalPosition = MBase->TriangleListNormalPosition + MBase->TriangleListNormalPositionNum ;
										MBTList->NormalPosition = ( MV1_TLIST_NORMAL_POS * )( ( ( DWORD_PTR )MBTList->NormalPosition + 15 ) / 16 * 16 ) ;

										// ���b�V�����_�C���f�b�N�X�f�[�^�̐擪�A�h���X���Z�b�g
										MBTList->MeshVertexIndex = MBase->MeshVertexIndex + MBase->MeshVertexIndexNum ;
										MBTList->VertexNum = 0 ;

										// ���_�����ɑ��݂��Ă��邩�ǂ����̃t���O������������
										_MEMSET( VertValidBuffer, 0xff, MBMesh->VertexNum * sizeof( int ) ) ;

										// ���ׂĂ̖ʂ���������܂Ń��[�v
//										NormV = MBTList->NormalPosition ;
										for( ; l < MBMesh->FaceNum && MBTList->IndexNum < MV1_TRIANGLE_MAX_INDEX ; l ++, MBFace ++ )
										{
											// ���b�V���̃g���C�A���O�����X�g�ԍ��̍X�V
											MBFace->TriangleListIndex = ( WORD )( MBTList - MBMesh->TriangleList ) ;

											// �|���S���̒��_�̐������J��Ԃ�
											for( p = 0 ; p < 3 ; p ++, MBTList->IndexNum ++ )
											{
												// �g���C�A���O�����g�p���Ă��钸�_�f�[�^�����ɂ��邩�ǂ����𒲂ׂ�
												if( VertValidBuffer[ MBFace->VertexIndex[ p ] ] == -1 )
												{
													// ����������ǉ�����
//													MBMVert = ( MV1_MESH_VERTEX   * )( ( BYTE * )MBMesh->Vertex    + MBFace->VertexIndex[ p ] * MBMesh->VertUnitSize   ) ;
//													MBMPos  = ( MV1_MESH_POSITION * )( ( BYTE * )MBFrame->Position + MBFrame->PosUnitSize     * MBMVert->PositionIndex ) ;
//													MBMNorm = MBFrame->Normal + MBMVert->NormalIndex ;
//													*( ( VECTOR * )&NormV->Position ) = MBMPos->Position ;
//													NormV->Position.w = 1.0f ;
//													*( ( VECTOR * )&NormV->Normal ) = MBMNorm->Normal ;
//													NormV->Normal.w = 0.0f ;
//													NormV ++ ;

													VertValidBuffer[ MBFace->VertexIndex[ p ] ] = MBTList->VertexNum ;
													MBTList->Index[ MBTList->IndexNum ] = ( unsigned short )MBTList->VertexNum ;

													MBTList->MeshVertexIndex[ MBTList->VertexNum ] = MBFace->VertexIndex[ p ] ;
													MBTList->VertexNum ++ ;
												}
												else
												{
													MBTList->Index[ MBTList->IndexNum ] = ( unsigned short )VertValidBuffer[ MBFace->VertexIndex[ p ] ] ;
												}
											}
										}

										// ���_�C���f�b�N�X�̐������Z
										MBase->TriangleListIndexNum += MBTList->IndexNum ;

										// ���b�V�����_�C���f�b�N�X�f�[�^�̐������Z����
										MBase->MeshVertexIndexNum += MBTList->VertexNum ;

										// �{�[���������_�f�[�^�̐������Z
										MBase->TriangleListNormalPositionNum += MBTList->VertexNum ;

										// �����A�N�Z�X�p���_�f�[�^�̃Z�b�g�A�b�v
										MV1SetupTriangleListPositionAndNormal( MBTList ) ;

										// �g�D�[���֊s���p���_�C���f�b�N�X�̍쐬
										MV1SetupToonOutLineTriangleList( MBTList ) ;
									}
								}

								// �{�[���T�����̃g���C�A���O�����X�g�̍쐬
								if( Mesh->MaterialPolyList[ o ].TypeNum[ MV1_VERTEX_TYPE_SKIN_4BONE ] )
								{
									MV1_TRIANGLELIST_R *TList ;

									MBFrame->IsSkinMesh = TRUE ;

									// �g���C�A���O�����X�g�̃A�h���X���Z�b�g
									MBTList = MBMesh->TriangleList + MBMesh->TriangleListNum ;
									MBase->TriangleListNum  += Mesh->MaterialPolyList[ o ].SkinB4TriangleListNum ;
									MBMesh->TriangleListNum += Mesh->MaterialPolyList[ o ].SkinB4TriangleListNum ;

									// �g���C�A���O�����X�g�̐������J��Ԃ�
									TList = Mesh->MaterialPolyList[ o ].SkinB4TriangleList ;
									for( j = 0 ; j < Mesh->MaterialPolyList[ o ].SkinB4TriangleListNum ; j ++, TList ++, MBTList ++ )
									{
										// �g���C�A���O�����X�g�̏����Z�b�g
										MBTList->Container = MBMesh ;
										MBTList->VertexType = MV1_VERTEX_TYPE_SKIN_4BONE ;

										// �P���_�̃T�C�Y���Z�b�g
										MBTList->PosUnitSize = sizeof( MV1_TLIST_SKIN_POS_4B ) ;

										// �g�p����{�[���C���f�b�N�X���Z�b�g
										for( l = 0 ; l < TList->UseBoneNum ; l ++ )
											MBTList->UseBone[ l ] = ( int )( TList->UseBone[ l ] + MeshBoneStartNum ) ;
										for( ; l < MV1_TRIANGLE_LIST_USE_BONE_MAX_NUM ; l ++ )
											MBTList->UseBone[ l ] = -1 ;
										MBTList->UseBoneNum = TList->UseBoneNum ;

										// ���_�C���f�b�N�X�f�[�^�̐擪�A�h���X���Z�b�g
										MBTList->Index = MBase->TriangleListIndex + MBase->TriangleListIndexNum ;
										MBTList->IndexNum = 0 ;
										MBTList->ToonOutLineIndex = MBTList->Index + MTBase.TriangleListIndexNum ;
										MBTList->ToonOutLineIndexNum = 0 ;

										// ���_�f�[�^�̐擪�A�h���X���Z�b�g
										MBTList->SkinPosition4B = MBase->TriangleListSkinPosition4B + MBase->TriangleListSkinPosition4BNum ;
										MBTList->SkinPosition4B = ( MV1_TLIST_SKIN_POS_4B * )( ( ( DWORD_PTR )MBTList->SkinPosition4B + 15 ) / 16 * 16 ) ;

										// ���b�V�����_�C���f�b�N�X�f�[�^�̐擪�A�h���X���Z�b�g
										MBTList->MeshVertexIndex = MBase->MeshVertexIndex + MBase->MeshVertexIndexNum ;
										MBTList->VertexNum = 0 ;

										// ���_�����ɑ��݂��Ă��邩�ǂ����̃t���O������������
										_MEMSET( VertValidBuffer, 0xff, MBMesh->VertexNum * sizeof( int ) ) ;

										// ���ׂĂ̖ʂ���������܂Ń��[�v
										MBFace = MBMesh->Face ;
//										Skin4BV = MBTList->SkinPosition4B ;
										for( l = 0 ; l < MBMesh->FaceNum ; l ++, MBFace ++ )
										{
											// �{�[���T�����ł͂Ȃ��ꍇ�͉������Ȃ�
											if( MBFace->VertexType != MV1_VERTEX_TYPE_SKIN_4BONE ) continue ;

											// �g���C�A���O�����X�g�ԍ����Ⴄ�ꍇ���������Ȃ�
											if( MBFace->TriangleListIndex != j ) continue ;

											// ���b�V���̃g���C�A���O�����X�g�ԍ��̍X�V
											MBFace->TriangleListIndex = ( WORD )( MBTList - MBMesh->TriangleList ) ;

											// ���_�f�[�^���Z�b�g
											for( p = 0 ; p < 3 ; p ++, MBTList->IndexNum ++ )
											{
												// �g���C�A���O�����g�p���Ă��钸�_�f�[�^�����ɂ��邩�ǂ����𒲂ׂ�
												if( VertValidBuffer[ MBFace->VertexIndex[ p ] ] == -1 )
												{
													// ����������ǉ�����
//													MBMVert = ( MV1_MESH_VERTEX   * )( ( BYTE * )MBMesh->Vertex    + MBFace->VertexIndex[ p ] * MBMesh->VertUnitSize   ) ;
//													MBMPos  = ( MV1_MESH_POSITION * )( ( BYTE * )MBFrame->Position + MBFrame->PosUnitSize     * MBMVert->PositionIndex ) ;
//													MBMNorm = MBFrame->Normal + MBMVert->NormalIndex ;
//													*( ( VECTOR * )&Skin4BV->Position ) = MBMPos->Position ;
//													Skin4BV->Position.w = 1.0f ;
//													Skin4BV->Normal = MBMNorm->Normal ;
//													VertB = MBMPos->BoneWeight ;
//													for( k = 0 ; k < 4 && k < MBFrame->MaxBoneBlendNum ; k ++, VertB ++ )
//													{
//														if( VertB->Index == -1 ) break ;
//
//														for( m = 0 ; m < MBTList->UseBoneNum && MBTList->UseBone[ m ] != VertB->Index ; m ++ ){}
//														Skin4BV->MatrixIndex[ k ] = m ;
//														Skin4BV->MatrixWeight[ k ] = VertB->W ;
//													}
//													for( ; k < 4 ; k ++ )
//													{
//														Skin4BV->MatrixIndex[ k ] = 0 ;
//														Skin4BV->MatrixWeight[ k ] = 0.0f ;
//													}
//													Skin4BV ++ ;

													VertValidBuffer[ MBFace->VertexIndex[ p ] ] = MBTList->VertexNum ;
													MBTList->Index[ MBTList->IndexNum ] = ( unsigned short )MBTList->VertexNum ;

													MBTList->MeshVertexIndex[ MBTList->VertexNum ] = MBFace->VertexIndex[ p ] ;
													MBTList->VertexNum ++ ;
												}
												else
												{
													MBTList->Index[ MBTList->IndexNum ] = ( unsigned short )VertValidBuffer[ MBFace->VertexIndex[ p ] ] ;
												}
											}
										}

										// ���_�C���f�b�N�X�̐������Z
										MBase->TriangleListIndexNum += MBTList->IndexNum ;

										// ���b�V�����_�C���f�b�N�X�f�[�^�̐������Z����
										MBase->MeshVertexIndexNum += MBTList->VertexNum ;

										// �S�{�[���ȓ��X�L�j���O���b�V�����_�f�[�^�̐������Z
										MBase->TriangleListSkinPosition4BNum += MBTList->VertexNum ;

										// �����A�N�Z�X�p���_�f�[�^�̃Z�b�g�A�b�v
										MV1SetupTriangleListPositionAndNormal( MBTList ) ;

										// �g�D�[���֊s���p���_�C���f�b�N�X�̍쐬
										MV1SetupToonOutLineTriangleList( MBTList ) ;
									}
								}

								// �{�[���X�����̃g���C�A���O�����X�g�̍쐬
								if( Mesh->MaterialPolyList[ o ].TypeNum[ MV1_VERTEX_TYPE_SKIN_8BONE ] )
								{
									MV1_TRIANGLELIST_R *TList ;

									MBFrame->IsSkinMesh = TRUE ;

									// �g���C�A���O�����X�g�̃A�h���X���Z�b�g
									MBTList = MBMesh->TriangleList + MBMesh->TriangleListNum ;
									MBase->TriangleListNum  += Mesh->MaterialPolyList[ o ].SkinB8TriangleListNum ;
									MBMesh->TriangleListNum += Mesh->MaterialPolyList[ o ].SkinB8TriangleListNum ;

									// �g���C�A���O�����X�g�̐������J��Ԃ�
									TList = Mesh->MaterialPolyList[ o ].SkinB8TriangleList ;
									for( j = 0 ; j < Mesh->MaterialPolyList[ o ].SkinB8TriangleListNum ; j ++, TList ++, MBTList ++ )
									{
										// �g���C�A���O�����X�g�̏����Z�b�g
										MBTList->Container = MBMesh ;
										MBTList->VertexType = MV1_VERTEX_TYPE_SKIN_8BONE ;

										// �P���_�̃T�C�Y���Z�b�g
										MBTList->PosUnitSize = sizeof( MV1_TLIST_SKIN_POS_8B ) ;

										// �g�p����{�[���C���f�b�N�X���Z�b�g
										for( l = 0 ; l < TList->UseBoneNum ; l ++ )
											MBTList->UseBone[ l ] = ( int )( TList->UseBone[ l ] + MeshBoneStartNum ) ;
										for( ; l < MV1_TRIANGLE_LIST_USE_BONE_MAX_NUM ; l ++ )
											MBTList->UseBone[ l ] = -1 ;
										MBTList->UseBoneNum = TList->UseBoneNum ;

										// ���_�C���f�b�N�X�f�[�^�̐擪�A�h���X���Z�b�g
										MBTList->Index = MBase->TriangleListIndex + MBase->TriangleListIndexNum ;
										MBTList->IndexNum = 0 ;
										MBTList->ToonOutLineIndex = MBTList->Index + MTBase.TriangleListIndexNum ;
										MBTList->ToonOutLineIndexNum = 0 ;

										// ���_�f�[�^�̐擪�A�h���X���Z�b�g
										MBTList->SkinPosition8B = MBase->TriangleListSkinPosition8B + MBase->TriangleListSkinPosition8BNum ;
										MBTList->SkinPosition8B = ( MV1_TLIST_SKIN_POS_8B * )( ( ( DWORD_PTR )MBTList->SkinPosition8B + 15 ) / 16 * 16 ) ;

										// ���b�V�����_�C���f�b�N�X�f�[�^�̐擪�A�h���X���Z�b�g
										MBTList->MeshVertexIndex = MBase->MeshVertexIndex + MBase->MeshVertexIndexNum ;
										MBTList->VertexNum = 0 ;

										// ���_�����ɑ��݂��Ă��邩�ǂ����̃t���O������������
										_MEMSET( VertValidBuffer, 0xff, MBMesh->VertexNum * sizeof( int ) ) ;

										// ���ׂĂ̖ʂ���������܂Ń��[�v
										MBFace = MBMesh->Face ;
//										Skin8BV = MBTList->SkinPosition8B ;
										for( l = 0 ; l < MBMesh->FaceNum ; l ++, MBFace ++ )
										{
											// �{�[���X�����ł͂Ȃ��ꍇ�͉������Ȃ�
											if( MBFace->VertexType != MV1_VERTEX_TYPE_SKIN_8BONE ) continue ;

											// �g���C�A���O�����X�g�ԍ����Ⴄ�ꍇ���������Ȃ�
											if( MBFace->TriangleListIndex != j ) continue ;

											// ���b�V���̃g���C�A���O�����X�g�ԍ��̍X�V
											MBFace->TriangleListIndex = ( WORD )( MBTList - MBMesh->TriangleList ) ;

											// ���_�f�[�^���Z�b�g
											for( p = 0 ; p < 3 ; p ++, MBTList->IndexNum ++ )
											{
												// �g���C�A���O�����g�p���Ă��钸�_�f�[�^�����ɂ��邩�ǂ����𒲂ׂ�
												if( VertValidBuffer[ MBFace->VertexIndex[ p ] ] == -1 )
												{
													// ����������ǉ�����
/*													MBMVert = ( MV1_MESH_VERTEX   * )( ( BYTE * )MBMesh->Vertex    + MBFace->VertexIndex[ p ] * MBMesh->VertUnitSize   ) ;
													MBMPos  = ( MV1_MESH_POSITION * )( ( BYTE * )MBFrame->Position + MBFrame->PosUnitSize     * MBMVert->PositionIndex ) ;
													MBMNorm = MBFrame->Normal + MBMVert->NormalIndex ;
													Skin8BV->Position = MBMPos->Position ;
													Skin8BV->Normal   = MBMNorm->Normal ;
													VertB = MBMPos->BoneWeight ;
													for( k = 0 ; k < 8 && k < MBFrame->MaxBoneBlendNum ; k ++, VertB ++ )
													{
														if( VertB->Index == -1 ) break ;
														Skin8BV->MatrixWeight[ k ] = VertB->W ;
														for( m = 0 ; m < MBTList->UseBoneNum && MBTList->UseBone[ m ] != VertB->Index ; m ++ ){}
														if( k < 4 )
														{
															Skin8BV->MatrixIndex1[ k ]     = m ;
														}
														else
														{
															Skin8BV->MatrixIndex2[ k - 4 ] = m ;
														}
													}
													for( ; k < 8 ; k ++ )
													{
														Skin8BV->MatrixWeight[ k ] = 0.0f ;
														if( k < 4 )
														{
															Skin8BV->MatrixIndex1[ k ]     = 0 ;
														}
														else
														{
															Skin8BV->MatrixIndex2[ k - 4 ] = 0 ;
														}
													}
													Skin8BV ++ ;
*/
													VertValidBuffer[ MBFace->VertexIndex[ p ] ] = MBTList->VertexNum ;
													MBTList->Index[ MBTList->IndexNum ] = ( unsigned short )MBTList->VertexNum ;

													MBTList->MeshVertexIndex[ MBTList->VertexNum ] = MBFace->VertexIndex[ p ] ;
													MBTList->VertexNum ++ ;
												}
												else
												{
													MBTList->Index[ MBTList->IndexNum ] = ( unsigned short )VertValidBuffer[ MBFace->VertexIndex[ p ] ] ;
												}
											}
										}

										// ���_�C���f�b�N�X�̐������Z
										MBase->TriangleListIndexNum += MBTList->IndexNum ;

										// ���b�V�����_�C���f�b�N�X�f�[�^�̐������Z����
										MBase->MeshVertexIndexNum += MBTList->VertexNum ;

										// �W�{�[���ȓ��X�L�j���O���b�V�����_�f�[�^�̐������Z
										MBase->TriangleListSkinPosition8BNum += MBTList->VertexNum ;

										// �����A�N�Z�X�p���_�f�[�^�̃Z�b�g�A�b�v
										MV1SetupTriangleListPositionAndNormal( MBTList ) ;

										// �g�D�[���֊s���p���_�C���f�b�N�X�̍쐬
										MV1SetupToonOutLineTriangleList( MBTList ) ;
									}
								}

								// �{�[�����������̃g���C�A���O�����X�g�̍쐬
								if( Mesh->MaterialPolyList[ o ].TypeNum[ MV1_VERTEX_TYPE_SKIN_FREEBONE ] )
								{
									MBFrame->IsSkinMesh = TRUE ;

									// �g���C�A���O�����X�g�̃A�h���X���Z�b�g
									MBTList = MBMesh->TriangleList + MBMesh->TriangleListNum ;
									MBase->TriangleListNum  += ( Mesh->MaterialPolyList[ o ].TypeNum[ MV1_VERTEX_TYPE_SKIN_FREEBONE ] * 3 + MV1_TRIANGLE_MAX_INDEX - 1 ) / MV1_TRIANGLE_MAX_INDEX ;
									MBMesh->TriangleListNum += ( Mesh->MaterialPolyList[ o ].TypeNum[ MV1_VERTEX_TYPE_SKIN_FREEBONE ] * 3 + MV1_TRIANGLE_MAX_INDEX - 1 ) / MV1_TRIANGLE_MAX_INDEX ;

									// ���ׂĂ̖ʂ���������܂Ń��[�v
									MBFace = MBMesh->Face ;
									for( l = 0 ; l < MBMesh->FaceNum ; MBTList ++ )
									{
										// �g���C�A���O�����X�g�̏����Z�b�g
										MBTList->Container = MBMesh ;
										MBTList->VertexType = MV1_VERTEX_TYPE_SKIN_FREEBONE ;

										// �P���_������̃T�C�Y���Z�b�g
										MBTList->PosUnitSize = ( unsigned short )( sizeof( MV1_TLIST_SKIN_POS_FREEB ) + sizeof( MV1_SKINBONE_BLEND ) * ( Mesh->MaterialPolyList[ o ].MaxBoneCount - 4 ) ) ;
										MBTList->PosUnitSize = ( unsigned short )( ( MBTList->PosUnitSize + 15 ) / 16 * 16 ) ;

										// �ő�{�[������ۑ�
										MBTList->MaxBoneNum = Mesh->MaterialPolyList[ o ].MaxBoneCount ;

										// ���_�C���f�b�N�X�f�[�^�̐擪�A�h���X���Z�b�g
										MBTList->Index = MBase->TriangleListIndex + MBase->TriangleListIndexNum ;
										MBTList->IndexNum = 0 ;
										MBTList->ToonOutLineIndex = MBTList->Index + MTBase.TriangleListIndexNum ;
										MBTList->ToonOutLineIndexNum = 0 ;

										// ���_�f�[�^�̐擪�A�h���X���Z�b�g
										MBTList->SkinPositionFREEB = ( MV1_TLIST_SKIN_POS_FREEB * )( ( BYTE * )MBase->TriangleListSkinPositionFREEB + MBase->TriangleListSkinPositionFREEBSize ) ;
										MBTList->SkinPositionFREEB = ( MV1_TLIST_SKIN_POS_FREEB * )( ( ( DWORD_PTR )MBTList->SkinPositionFREEB + 15 ) / 16 * 16 ) ;

										// ���b�V�����_�C���f�b�N�X�f�[�^�̐擪�A�h���X���Z�b�g
										MBTList->MeshVertexIndex = MBase->MeshVertexIndex + MBase->MeshVertexIndexNum ;
										MBTList->VertexNum = 0 ;

										// ���_�����ɑ��݂��Ă��邩�ǂ����̃t���O������������
										_MEMSET( VertValidBuffer, 0xff, MBMesh->VertexNum * sizeof( int ) ) ;

										// ���_�f�[�^�̃Z�b�g
//										SkinFBV  = MBTList->SkinPositionFREEB ;
										for( ; l < MBMesh->FaceNum && MBTList->IndexNum < MV1_TRIANGLE_MAX_INDEX ; l ++, MBFace ++ )
										{
											// �{�[�����������ł͂Ȃ��ꍇ�͉������Ȃ�
											if( MBFace->VertexType != MV1_VERTEX_TYPE_SKIN_FREEBONE ) continue ;

											// ���b�V���̃g���C�A���O�����X�g�ԍ��̍X�V
											MBFace->TriangleListIndex = ( WORD )( MBTList - MBMesh->TriangleList ) ;

											// ���_�f�[�^���Z�b�g
											for( p = 0 ; p < 3 ; p ++, MBTList->IndexNum ++ )
											{
												// �g���C�A���O�����g�p���Ă��钸�_�f�[�^�����ɂ��邩�ǂ����𒲂ׂ�
												if( VertValidBuffer[ MBFace->VertexIndex[ p ] ] == -1 )
												{
													// ����������ǉ�����
/*													MBMVert = ( MV1_MESH_VERTEX   * )( ( BYTE * )MBMesh->Vertex    + MBFace->VertexIndex[ p ] * MBMesh->VertUnitSize   ) ;
													MBMPos  = ( MV1_MESH_POSITION * )( ( BYTE * )MBFrame->Position + MBFrame->PosUnitSize     * MBMVert->PositionIndex ) ;
													MBMNorm = MBFrame->Normal + MBMVert->NormalIndex ;
													*( ( VECTOR * )&SkinFBV->Position ) = MBMPos->Position ;
													SkinFBV->Position.w = 1.0f ;
													*( ( VECTOR * )&SkinFBV->Normal ) = MBMNorm->Normal ;
													SkinFBV->Normal.w = 0.0f ;
													VertB = MBMPos->BoneWeight ;
													for( k = 0 ; k < MBTList->MaxBoneNum ; k ++, VertB ++ )
													{
														if( VertB->Index == -1 ) break ;
														SkinFBV->MatrixWeight[ k ].Index = VertB->Index ;
														SkinFBV->MatrixWeight[ k ].W     = VertB->W ;
													}
													if( k != MBTList->MaxBoneNum )
													{
														SkinFBV->MatrixWeight[ k ].Index = -1 ;
														SkinFBV->MatrixWeight[ k ].W     = 0.0f ;
													}
													SkinFBV = ( MV1_TLIST_SKIN_POS_FREEB * )( ( BYTE * )SkinFBV + MBTList->PosUnitSize ) ;
*/
													VertValidBuffer[ MBFace->VertexIndex[ p ] ] = MBTList->VertexNum ;
													MBTList->Index[ MBTList->IndexNum ] = ( unsigned short )MBTList->VertexNum ;

													MBTList->MeshVertexIndex[ MBTList->VertexNum ] = MBFace->VertexIndex[ p ] ;
													MBTList->VertexNum ++ ;
												}
												else
												{
													MBTList->Index[ MBTList->IndexNum ] = ( unsigned short )VertValidBuffer[ MBFace->VertexIndex[ p ] ] ;
												}
											}
										}

										// ���_�C���f�b�N�X�̐������Z
										MBase->TriangleListIndexNum += MBTList->IndexNum ;

										// ���b�V�����_�C���f�b�N�X�f�[�^�̐������Z����
										MBase->MeshVertexIndexNum += MBTList->VertexNum ;

										// �{�[�����������X�L�j���O���b�V�����_�f�[�^�̃T�C�Y�����Z
										MBase->TriangleListSkinPositionFREEBSize += MBTList->VertexNum * MBTList->PosUnitSize ;

										// �����A�N�Z�X�p���_�f�[�^�̃Z�b�g�A�b�v
										MV1SetupTriangleListPositionAndNormal( MBTList ) ;

										// �g�D�[���֊s���p���_�C���f�b�N�X�̍쐬
										MV1SetupToonOutLineTriangleList( MBTList ) ;
									}
								}
							}
						}

						// �V�F�C�v�f�[�^���ΏۂƂ��郁�b�V���������ꍇ�̓V�F�C�v���b�V���̏����Z�b�g�A�b�v����
						Shape = Frame->ShapeFirst ;
						MBShape = MBFrame->Shape ;
						for( j = 0 ; j < Frame->ShapeNum ; j ++, Shape = Shape->Next, MBShape ++ )
						{
							// �Ώۂ̃��b�V���ł͖��������牽�����Ȃ�
							if( Shape->TargetMesh != Mesh ) continue ;

							// ���b�V���̐������J��Ԃ�
							MBMesh = MBFrame->Mesh + MeshStartNum ;
							for( o = 0 ; ( DWORD )o < MeshNowNum ; o ++, MBMesh ++ )
							{
								MV1_MESH_VERTEX *TVertex ;

								// �A�h���X���Z�b�g
								MBShapeMesh = MBase->ShapeMesh + MBase->ShapeMeshNum ;

								MBShapeMesh->TargetMesh = MBMesh ;
								MBShapeMesh->VertexNum = 0 ;
								MBShapeMesh->Vertex = MBase->ShapeVertex + MBase->ShapeVertexNum ;

								// �V�F�C�v�Ώۂ̒��_���
								MBShapeVertex = MBShapeMesh->Vertex ;
								TVertex = MBMesh->Vertex ;
								for( l = 0 ; l < MBMesh->VertexNum ; l ++, TVertex = ( MV1_MESH_VERTEX * )( ( BYTE * )TVertex + MBMesh->VertUnitSize ) )
								{
									if( Shape->NextTable[ TVertex->PositionIndex - MeshPositionStartNum ] == 0xffffffff )
										continue ;
									ShapeVertex = &Shape->Vertex[ Shape->NextTable[ TVertex->PositionIndex - MeshPositionStartNum ] ] ;

									MBShapeVertex->TargetMeshVertex = ( DWORD )l ;
									MBShapeVertex->Position         = ShapeVertex->Position ;
									if( Shape->ValidVertexNormal )
									{
										MBShapeVertex->Normal = ShapeVertex->Normal ;
									}
									else
									{
										MBShapeVertex->Normal = VGet( 0.0f, 0.0f, 0.0f ) ;
									}

									MBShapeVertex ++ ;
									MBShapeMesh->VertexNum ++ ;
									MBase->ShapeVertexNum ++ ;
								}

								// �ւ���Ă��钸�_��������������牽�����Ȃ�
								if( MBShapeMesh->VertexNum == 0 ) continue ;

								// �V�F�C�v���b�V���ł�����t����
								MBMesh->Shape = 1 ;

								// �V�F�C�v���b�V�����̃g���C�A���O�����X�g�̒��_�������Z����
								MBTList = MBMesh->TriangleList ;
								for( l = 0 ; l < MBMesh->TriangleListNum ; l ++, MBTList ++ )
								{
									switch( MBTList->VertexType )
									{
									case MV1_VERTEX_TYPE_NORMAL :        MBase->ShapeNormalPositionNum     += MBTList->VertexNum ; break ;
									case MV1_VERTEX_TYPE_SKIN_4BONE :    MBase->ShapeSkinPosition4BNum     += MBTList->VertexNum ; break ;
									case MV1_VERTEX_TYPE_SKIN_8BONE :    MBase->ShapeSkinPosition8BNum     += MBTList->VertexNum ; break ;
									case MV1_VERTEX_TYPE_SKIN_FREEBONE : MBase->ShapeSkinPositionFREEBSize += MBTList->VertexNum * MBTList->PosUnitSize ; break ;
									}
								}

								// �V�F�C�v���b�V���̒��_�̐������Z
								MBase->ShapeTargetMeshVertexNum += MBMesh->VertexNum ;

								if( MBShape->MeshNum == 0 )
								{
									MBShape->Mesh = MBShapeMesh ;
								}

								MBShape->MeshNum ++ ;
								MBase->ShapeMeshNum ++ ;
							}
						}
					}
				}

				// ���C�g�����݂���ꍇ�̓��C�g�̏����Z�b�g
				if( Frame->Light )
				{
					MBLight = MBase->Light + MBase->LightNum ;
					MBFrame->Light = MBLight ;
					MBase->LightNum ++ ;

					// ���̃R�s�[
					MBLight->Index = Frame->Index ;
					MBLight->FrameIndex = Frame->Light->FrameIndex ;
					MBLight->Type = Frame->Light->Type ;
					MBLight->Diffuse = Frame->Light->Diffuse ;
					MBLight->Specular = Frame->Light->Specular ;
					MBLight->Ambient = Frame->Light->Ambient ;
					MBLight->Range = Frame->Light->Range ;
					MBLight->Falloff = Frame->Light->Falloff ;
					MBLight->Attenuation0 = Frame->Light->Attenuation0 ;
					MBLight->Attenuation1 = Frame->Light->Attenuation1 ;
					MBLight->Attenuation2 = Frame->Light->Attenuation2 ;
					MBLight->Theta = Frame->Light->Theta ;
					MBLight->Phi = Frame->Light->Phi ;

					// ���O���R�s�[
#ifndef UNICODE
					MBLight->NameA = MV1RGetStringSpace(  MBase, Frame->NameA ) ;
#endif
					MBLight->NameW = MV1RGetStringSpaceW( MBase, Frame->NameW ) ;
				}
			}

			// �������Z�Ŏg�p���鍄�̂̏�����������
			PhysicsRigidBody = ReadModel->PhysicsRigidBodyFirst ;
			for( i = 0 ; i < ( int )ReadModel->PhysicsRigidBodyNum ; i ++, PhysicsRigidBody = PhysicsRigidBody->DataNext )
			{
				// �A�h���X���Z�b�g
				MBPhysicsRigidBody = MBase->PhysicsRigidBody + MBase->PhysicsRigidBodyNum ;
				MBase->PhysicsRigidBodyNum ++ ;

				// ���O��ۑ�
#ifndef UNICODE
				MBPhysicsRigidBody->NameA = MV1RGetStringSpace(  MBase, PhysicsRigidBody->NameA ) ;
#endif
				MBPhysicsRigidBody->NameW = MV1RGetStringSpaceW( MBase, PhysicsRigidBody->NameW ) ;

				// �C���f�b�N�X�̃Z�b�g
				MBPhysicsRigidBody->Index = PhysicsRigidBody->Index ;

				// �p�����[�^�̃R�s�[
				MBPhysicsRigidBody->TargetFrame = &MBase->Frame[ PhysicsRigidBody->TargetFrame->Index ] ;
				MBPhysicsRigidBody->TargetFrame->PhysicsRigidBody = MBPhysicsRigidBody ;
				MBPhysicsRigidBody->RigidBodyGroupIndex = PhysicsRigidBody->RigidBodyGroupIndex ;
				MBPhysicsRigidBody->RigidBodyGroupTarget = PhysicsRigidBody->RigidBodyGroupTarget ;
				MBPhysicsRigidBody->ShapeType = PhysicsRigidBody->ShapeType ;
				MBPhysicsRigidBody->ShapeW = PhysicsRigidBody->ShapeW ;
				MBPhysicsRigidBody->ShapeH = PhysicsRigidBody->ShapeH ;
				MBPhysicsRigidBody->ShapeD = PhysicsRigidBody->ShapeD ;
				MBPhysicsRigidBody->Position = PhysicsRigidBody->Position ;
				MBPhysicsRigidBody->Rotation = PhysicsRigidBody->Rotation ;
				MBPhysicsRigidBody->RigidBodyWeight = PhysicsRigidBody->RigidBodyWeight ;
				MBPhysicsRigidBody->RigidBodyPosDim = PhysicsRigidBody->RigidBodyPosDim ;
				MBPhysicsRigidBody->RigidBodyRotDim = PhysicsRigidBody->RigidBodyRotDim ;
				MBPhysicsRigidBody->RigidBodyRecoil = PhysicsRigidBody->RigidBodyRecoil ;
				MBPhysicsRigidBody->RigidBodyFriction = PhysicsRigidBody->RigidBodyFriction ;
				MBPhysicsRigidBody->RigidBodyType = PhysicsRigidBody->RigidBodyType ;
				MBPhysicsRigidBody->NoCopyToBone = PhysicsRigidBody->NoCopyToBone ;
			}

			// �������Z�Ŏg�p����W���C���g�̏�����������
			PhysicsJoint = ReadModel->PhysicsJointFirst ;
			for( i = 0 ; i < ( int )ReadModel->PhysicsJointNum ; i ++, PhysicsJoint = PhysicsJoint->DataNext )
			{
				// �A�h���X���Z�b�g
				MBPhysicsJoint = MBase->PhysicsJoint + MBase->PhysicsJointNum ;
				MBase->PhysicsJointNum ++ ;

				// ���O��ۑ�
#ifndef UNICODE
				MBPhysicsJoint->NameA = MV1RGetStringSpace(  MBase, PhysicsJoint->NameA ) ;
#endif
				MBPhysicsJoint->NameW = MV1RGetStringSpaceW( MBase, PhysicsJoint->NameW ) ;

				// �C���f�b�N�X�̃Z�b�g
				MBPhysicsJoint->Index = PhysicsJoint->Index ;

				// �p�����[�^�̃R�s�[
				MBPhysicsJoint->RigidBodyA = &MBase->PhysicsRigidBody[ PhysicsJoint->RigidBodyA ] ;
				MBPhysicsJoint->RigidBodyB = &MBase->PhysicsRigidBody[ PhysicsJoint->RigidBodyB ] ;
				MBPhysicsJoint->Position = PhysicsJoint->Position ;
				MBPhysicsJoint->Rotation = PhysicsJoint->Rotation ;
				MBPhysicsJoint->ConstrainPosition1 = PhysicsJoint->ConstrainPosition1 ;
				MBPhysicsJoint->ConstrainPosition2 = PhysicsJoint->ConstrainPosition2 ;
				MBPhysicsJoint->ConstrainRotation1 = PhysicsJoint->ConstrainRotation1 ;
				MBPhysicsJoint->ConstrainRotation2 = PhysicsJoint->ConstrainRotation2 ;
				MBPhysicsJoint->SpringPosition = PhysicsJoint->SpringPosition ;
				MBPhysicsJoint->SpringRotation = PhysicsJoint->SpringRotation ;
			}

			// ���ۂɎg�p�����������T�C�Y�ɉ����Ē��_�f�[�^���������m�ۂ��Ȃ���
			if( ReadModel->NormalPositionNum ||
				ReadModel->SkinPosition4BNum ||
				ReadModel->SkinPosition8BNum ||
				ReadModel->SkinPositionFREEBSize ||
				ReadModel->MeshVertexIndexNum ||
				ReadModel->ShapeVertexNum )
			{
				void *VertexData ;
				MV1_TLIST_NORMAL_POS	*NormalPosition ;
				MV1_TLIST_SKIN_POS_4B	*SkinPosition4B ;
				MV1_TLIST_SKIN_POS_8B	*SkinPosition8B ;
				MV1_TLIST_SKIN_POS_FREEB *SkinPositionFREEB ;
				DWORD					*MeshVertexIndex ;
				MV1_MESH_POSITION		*MeshPosition ;
				MV1_MESH_FACE			*MeshFace ;
				MV1_MESH_NORMAL			*MeshNormal ;
				MV1_MESH_VERTEX			*MeshVertex ;
				MV1_SHAPE_VERTEX_BASE	*ShapeVertex ;

				VertexData        = MBase->VertexData ;
				NormalPosition    = MBase->TriangleListNormalPosition ;
				SkinPosition4B    = MBase->TriangleListSkinPosition4B ;
				SkinPosition8B    = MBase->TriangleListSkinPosition8B ;
				SkinPositionFREEB = MBase->TriangleListSkinPositionFREEB ;
				MeshVertexIndex   = MBase->MeshVertexIndex ;
				MeshPosition      = MBase->MeshPosition ;
				MeshFace          = MBase->MeshFace ;
				MeshNormal        = MBase->MeshNormal ;
				MeshVertex        = MBase->MeshVertex ;
				ShapeVertex       = MBase->ShapeVertex ;

				MBase->VertexDataSize =
					sizeof( DWORD )                 * MBase->MeshVertexIndexNum +
					sizeof( MV1_MESH_FACE )         * MBase->MeshFaceNum        +
					sizeof( MV1_MESH_NORMAL )       * MBase->MeshNormalNum      +
					sizeof( MV1_TLIST_NORMAL_POS )  * MBase->TriangleListNormalPositionNum  +
					sizeof( MV1_TLIST_SKIN_POS_4B ) * MBase->TriangleListSkinPosition4BNum  +
					sizeof( MV1_TLIST_SKIN_POS_8B ) * MBase->TriangleListSkinPosition8BNum  +
					MBase->TriangleListSkinPositionFREEBSize                   + 
					MBase->MeshPositionSize                                    +
					MBase->MeshVertexSize                                      +
					sizeof( MV1_SHAPE_VERTEX_BASE ) * MBase->ShapeVertexNum    + 16 ;
				MBase->VertexData = DXALLOC( MBase->VertexDataSize ) ;
				if( MBase->VertexData == NULL )
				{
					DXFREE( VertexData ) ;
					DXST_ERRORLOG_ADDUTF16LE( "\x52\x00\x65\x00\x61\x00\x64\x00\x20\x00\x4d\x00\x6f\x00\x64\x00\x65\x00\x6c\x00\x20\x00\x43\x00\x6f\x00\x6e\x00\x76\x00\x65\x00\x72\x00\x74\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\x02\x98\xb9\x70\xa7\x5e\x19\x6a\x68\x30\x02\x98\xb9\x70\xd5\x6c\xda\x7d\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Read Model Convert Error : ���_���W�ƒ��_�@�����i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ ) ;
					goto ERRORLABEL ;
				}
				MBase->TriangleListNormalPosition    = ( MV1_TLIST_NORMAL_POS     * )( ( ( DWORD_PTR )MBase->VertexData + 15 ) / 16 * 16 ) ;
				MBase->TriangleListSkinPosition4B    = ( MV1_TLIST_SKIN_POS_4B    * )( MBase->TriangleListNormalPosition              + MBase->TriangleListNormalPositionNum     ) ;
				MBase->TriangleListSkinPosition8B    = ( MV1_TLIST_SKIN_POS_8B    * )( MBase->TriangleListSkinPosition4B              + MBase->TriangleListSkinPosition4BNum     ) ;
				MBase->TriangleListSkinPositionFREEB = ( MV1_TLIST_SKIN_POS_FREEB * )( MBase->TriangleListSkinPosition8B              + MBase->TriangleListSkinPosition8BNum     ) ;
				MBase->MeshVertexIndex               = ( DWORD                    * )( ( BYTE * )MBase->TriangleListSkinPositionFREEB + MBase->TriangleListSkinPositionFREEBSize ) ;
				MBase->MeshFace                      = ( MV1_MESH_FACE            * )( MBase->MeshVertexIndex             + MBase->MeshVertexIndexNum    ) ;
				MBase->MeshNormal                    = ( MV1_MESH_NORMAL          * )( MBase->MeshFace                    + MBase->MeshFaceNum           ) ;
				MBase->MeshPosition                  = ( MV1_MESH_POSITION        * )( MBase->MeshNormal                  + MBase->MeshNormalNum         ) ;
				MBase->MeshVertex                    = ( MV1_MESH_VERTEX          * )( ( BYTE * )MBase->MeshPosition      + MBase->MeshPositionSize      ) ;
				MBase->ShapeVertex                   = ( MV1_SHAPE_VERTEX_BASE    * )( ( BYTE * )MBase->MeshVertex        + MBase->MeshVertexSize        ) ;

				// ���_�f�[�^���R�s�[����
				if( MBase->TriangleListNormalPositionNum     ) _MEMCPY( MBase->TriangleListNormalPosition,    NormalPosition,    sizeof( MV1_TLIST_NORMAL_POS )  * MBase->TriangleListNormalPositionNum ) ;
				else                                           MBase->TriangleListNormalPosition    = NULL ;

				if( MBase->TriangleListSkinPosition4BNum     ) _MEMCPY( MBase->TriangleListSkinPosition4B,    SkinPosition4B,    sizeof( MV1_TLIST_SKIN_POS_4B ) * MBase->TriangleListSkinPosition4BNum ) ;
				else                                           MBase->TriangleListSkinPosition4B    = NULL ;

				if( MBase->TriangleListSkinPosition8BNum     ) _MEMCPY( MBase->TriangleListSkinPosition8B,    SkinPosition8B,    sizeof( MV1_TLIST_SKIN_POS_8B ) * MBase->TriangleListSkinPosition8BNum ) ;
				else                                           MBase->TriangleListSkinPosition8B    = NULL ;

				if( MBase->TriangleListSkinPositionFREEBSize ) _MEMCPY( MBase->TriangleListSkinPositionFREEB, SkinPositionFREEB, ( size_t )MBase->TriangleListSkinPositionFREEBSize ) ;
				else                                           MBase->TriangleListSkinPositionFREEB = NULL ;

				if( MBase->MeshVertexIndexNum    ) _MEMCPY( MBase->MeshVertexIndex,   MeshVertexIndex,   sizeof( DWORD ) * MBase->MeshVertexIndexNum ) ;
				else                               MBase->MeshVertexIndex   = NULL ;

				if( MBase->MeshFaceNum           ) _MEMCPY( MBase->MeshFace,          MeshFace,          sizeof( MV1_MESH_FACE ) * MBase->MeshFaceNum ) ;
				else                               MBase->MeshFace          = NULL ;

				if( MBase->MeshNormalNum         ) _MEMCPY( MBase->MeshNormal,        MeshNormal,        sizeof( MV1_MESH_NORMAL ) * MBase->MeshNormalNum ) ;
				else                               MBase->MeshNormal        = NULL ;

				if( MBase->MeshPositionSize      ) _MEMCPY( MBase->MeshPosition,      MeshPosition,      ( size_t )MBase->MeshPositionSize ) ;
				else                               MBase->MeshPosition      = NULL ;

				if( MBase->MeshVertexSize        ) _MEMCPY( MBase->MeshVertex,        MeshVertex,        ( size_t )MBase->MeshVertexSize ) ;
				else                               MBase->MeshVertex        = NULL ;

				if( MBase->ShapeVertexNum        ) _MEMCPY( MBase->ShapeVertex,       ShapeVertex,       sizeof( MV1_SHAPE_VERTEX_BASE ) * MBase->ShapeVertexNum ) ;
				else                               MBase->ShapeVertex       = NULL ;

				// �e�t���[���̃��b�V���Ɋւ���|�C���^�̃A�h���X��ύX����
				MBFrame = MBase->Frame ;
				for( i = 0 ; i < ( DWORD )MBase->FrameNum ; i ++, MBFrame ++ )
				{
					if( MBFrame->Position )
					{
						MBFrame->Position = ( MV1_MESH_POSITION * )( ( DWORD_PTR )MBFrame->Position - ( DWORD_PTR )MeshPosition + ( DWORD_PTR )MBase->MeshPosition ) ;
					}

					if( MBFrame->Normal )
					{
						MBFrame->Normal   = ( MV1_MESH_NORMAL   * )( ( DWORD_PTR )MBFrame->Normal   - ( DWORD_PTR )MeshNormal   + ( DWORD_PTR )MBase->MeshNormal   ) ;
					}
				}
				
				// �e���b�V���̒��_�f�[�^�̃A�h���X��ύX����
				MBMesh = MBase->Mesh ;
				for( i = 0 ; i < ( DWORD )MBase->MeshNum ; i ++, MBMesh ++ )
				{
					if( MBMesh->Face )
					{
						MBMesh->Face = ( MV1_MESH_FACE * )( ( DWORD_PTR )MBMesh->Face - ( DWORD_PTR )MeshFace + ( DWORD_PTR )MBase->MeshFace ) ;
					}

					if( MBMesh->Vertex )
					{
						MBMesh->Vertex = ( MV1_MESH_VERTEX * )( ( DWORD_PTR )MBMesh->Vertex - ( DWORD_PTR )MeshVertex + ( DWORD_PTR )MBase->MeshVertex ) ;
					}
				}

				// �e�V�F�C�v���b�V���̒��_�f�[�^�̃A�h���X��ύX����
				MBShapeMesh = MBase->ShapeMesh ;
				for( i = 0 ; i < ( DWORD )MBase->ShapeMeshNum ; i ++, MBShapeMesh ++ )
				{
					if( MBShapeMesh->Vertex )
					{
						MBShapeMesh->Vertex = ( MV1_SHAPE_VERTEX_BASE * )( ( DWORD_PTR )MBShapeMesh->Vertex - ( DWORD_PTR )ShapeVertex + ( DWORD_PTR )MBase->ShapeVertex ) ;
					}
				}

				// �e�g���C�A���O�����X�g�̃|�C���^�̃A�h���X��ύX����
				MBMesh = MBase->Mesh ;
				for( i = 0 ; i < ( DWORD )MBase->MeshNum ; i ++, MBMesh ++ )
				{
					MBTList = MBMesh->TriangleList ;
					for( j = 0 ; j < MBMesh->TriangleListNum ; j ++, MBTList ++ )
					{
						if( MBTList->VertexNum == 0 ) continue ;

						MBTList->MeshVertexIndex = ( DWORD * )( ( DWORD_PTR )MBTList->MeshVertexIndex - ( DWORD_PTR )MeshVertexIndex + ( DWORD_PTR )MBase->MeshVertexIndex ) ;

						switch( MBTList->VertexType )
						{
						case MV1_VERTEX_TYPE_NORMAL        : MBTList->NormalPosition    = ( MV1_TLIST_NORMAL_POS     * )( ( DWORD_PTR )MBTList->NormalPosition    - ( DWORD_PTR )NormalPosition    + ( DWORD_PTR )MBase->TriangleListNormalPosition    ) ; break ;
						case MV1_VERTEX_TYPE_SKIN_4BONE    : MBTList->SkinPosition4B    = ( MV1_TLIST_SKIN_POS_4B    * )( ( DWORD_PTR )MBTList->SkinPosition4B    - ( DWORD_PTR )SkinPosition4B    + ( DWORD_PTR )MBase->TriangleListSkinPosition4B    ) ; break ;
						case MV1_VERTEX_TYPE_SKIN_8BONE    : MBTList->SkinPosition8B    = ( MV1_TLIST_SKIN_POS_8B    * )( ( DWORD_PTR )MBTList->SkinPosition8B    - ( DWORD_PTR )SkinPosition8B    + ( DWORD_PTR )MBase->TriangleListSkinPosition8B    ) ; break ;
						case MV1_VERTEX_TYPE_SKIN_FREEBONE : MBTList->SkinPositionFREEB = ( MV1_TLIST_SKIN_POS_FREEB * )( ( DWORD_PTR )MBTList->SkinPositionFREEB - ( DWORD_PTR )SkinPositionFREEB + ( DWORD_PTR )MBase->TriangleListSkinPositionFREEB ) ; break ;
						}
					}
				}

				// �ꎞ�I�Ɏg�p���Ă������_�o�b�t�@���������
				DXFREE( VertexData ) ;
			}

			// �e�t���[���̃|���S���̐��ƒ��_�̐����Z�b�g����
			MBMesh = MBase->Mesh ;
			for( i = 0 ; i < ( DWORD )MBase->MeshNum ; i ++, MBMesh ++ )
			{
				MBTList = MBMesh->TriangleList ;
				for( j = 0 ; j < MBMesh->TriangleListNum ; j ++, MBTList ++ )
				{
					MBMesh->Container->TriangleNum += MBTList->IndexNum / 3 ;
					MBMesh->Container->VertexNum += MBTList->VertexNum ;
					MBase->TriangleListVertexNum += MBTList->VertexNum ;
				}
			}

			// �e�{�[���ƃt���[���̎g�p�s��̃����N�����Z�b�g����
			MBSkinW = MBase->SkinBone ;
			for( i = 0 ; i < ( DWORD )MBase->SkinBoneNum ; i ++, MBSkinW ++ )
			{
				MBSkinWF = MBSkinW->UseFrame ;
				for( j = 0 ; j < MBSkinW->UseFrameNum ; j ++, MBSkinWF ++ )
				{
					MBFrame = &MBase->Frame[ MBSkinWF->Index ] ;
					for( k = 0 ; k < MBFrame->UseSkinBoneNum && MBFrame->UseSkinBone[ k ] != MBSkinW ; k ++ ){}
					if( k == MBFrame->UseSkinBoneNum )
					{
						// �����ɂ͂��Ȃ��͂�
						DXST_ERRORLOG_ADDUTF16LE( "\xa8\x30\xe9\x30\xfc\x30\x10\xff\x00"/*@ L"�G���[�O" @*/ ) ;
						return -1 ;
					}
					MBSkinWF->MatrixIndex = k ;
				}
			}

			// �ύX���Ǘ��p�̃f�[�^�T�C�Y���P�U�̔{���ɂ���
			MBase->ChangeDrawMaterialTableSize = ( MBase->ChangeDrawMaterialTableSize + 15 ) / 16 * 16 ;
			MBase->ChangeMatrixTableSize       = ( MBase->ChangeMatrixTableSize       + 15 ) / 16 * 16 ;

			// �X�L�j���O���b�V���p�̃{�[�������œK��
			MV1OptimizeSkinBoneInfo( MBase ) ;
		}
	}

	// �������v�f�̂���}�e���A���������b�V�����o�b�N�J�����O�����ɂ���
	if( ReadModel->TranslateIsBackCulling )
	{
		MBMesh = MBase->Mesh ;
		for( i = 0 ; i < ( DWORD )MBase->MeshNum ; i ++, MBMesh ++ )
		{
			if( MBMesh->Material->Diffuse.a < 0.99999999f )
				MBMesh->BackCulling = 0 ;
		}
	}

	// �����s��̃Z�b�g�A�b�v
	MV1SetupInitializeMatrixBase( MBase ) ;

	// ���b�V���̔��������ǂ����̏����Z�b�g�A�b�v����
	MV1SetupMeshSemiTransStateBase( MBase ) ;

	// ���������`��֌W�̏����Z�b�g�A�b�v����
	if( MBase->UsePackDraw )
	{
		MV1SetupPackDrawInfo( MBase ) ;
	}

	// �������̉��
	if( VertInfoTable )
	{
		DXFREE( VertInfoTable ) ;
		VertInfoTable = NULL ;
	}

	// �w�肪����ꍇ�͍��W�̍œK�����s��
	if( GParam->LoadModelToPositionOptimize )
	{
		MV1PositionOptimizeBase( NewHandle ) ;
	}

	// �w�肪����ꍇ�͖@���̍Čv�Z���s��
	if( GParam->LoadModelToReMakeNormal )
	{
		MV1ReMakeNormalBase( NewHandle, GParam->LoadModelToReMakeNormalSmoothingAngle, ASyncThread ) ;
	}

	// ���������p���_�f�[�^�̍\�z
	MBTList = MBase->TriangleList ;
	for( i = 0 ; i < ( DWORD )MBase->TriangleListNum ; i ++, MBTList ++ )
	{
		MV1SetupTriangleListPositionAndNormal( MBTList ) ;
		MV1SetupToonOutLineTriangleList( MBTList ) ;
	}

#ifndef DX_NON_ASYNCLOAD
	if( ASyncThread )
	{
		DecASyncLoadCount( NewHandle ) ;
	}
#endif // DX_NON_ASYNCLOAD

	// �n���h����Ԃ�
	return NewHandle ;

	// �G���[����
ERRORLABEL :

	// �n���h�����L���ȏꍇ�̓n���h���̍폜
	if( NewHandle != -1 )
	{
#ifndef DX_NON_ASYNCLOAD
		if( ASyncThread )
		{
			DecASyncLoadCount( NewHandle ) ;
		}
#endif // DX_NON_ASYNCLOAD

		MV1SubModelBase( NewHandle ) ;
		NewHandle = -1 ;
	}

	// �������̉��
	if( VertInfoTable )
	{
		DXFREE( VertInfoTable ) ;
		VertInfoTable = NULL ;
	}

	return -1 ;
}

#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

#endif






