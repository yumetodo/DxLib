// -------------------------------------------------------------------------------
// 
// 		�c�w���C�u����		���f���f�[�^����v���O����( Direct3D9 )�w�b�_�t�@�C��
// 
// 				Ver 3.16d
// 
// -------------------------------------------------------------------------------

#ifndef __DXMODELD3D9_H__
#define __DXMODELD3D9_H__

#include "../DxCompileConfig.h"

#ifndef DX_NON_DIRECT3D9

// �C���N���[�h ------------------------------------------------------------------

#ifndef DX_NON_MODEL

#include "DxDirectX.h"
#include "../DxLib.h"
#include "../DxModel.h"

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// �}�N����` --------------------------------------------------------------------

// �\���̒�` --------------------------------------------------------------------

// ���̃��b�V���^�C�v���_�\����
struct MV1_VERTEX_SIMPLE_D3D9
{
	VECTOR					Position ;							// ���W
	VECTOR					Normal ;							// �@��
	COLOR_U8				DiffuseColor ;						// �f�B�t���[�Y�F
	COLOR_U8				SpecularColor ;						// �X�y�L�����[�F
	float					UVs[ 3 ][ 4 ] ;						// �e�N�X�`�����W
} ;

// ���̃��b�V���^�C�v�֊s���p���_�\����
struct MV1_VERTEX_SIMPLE_TOL_D3D9
{
	VECTOR					Position ;							// ���W
	COLOR_U8				DiffuseColor ;						// �f�B�t���[�Y�F
} ;

// ���̃��b�V���^�C�v�o���v�}�b�v�p���_�\����
struct MV1_VERTEX_SIMPLE_BUMP_D3D9
{
	VECTOR					Position ;							// ���W
	VECTOR					Tangent ;							// ���_�̐ڐ�
	VECTOR					Binormal ;							// ���_�̏]�@��
	VECTOR					Normal ;							// �@��
	COLOR_U8				DiffuseColor ;						// �f�B�t���[�Y�F
	COLOR_U8				SpecularColor ;						// �X�y�L�����[�F
	float					UVs[ 1 ][ 4 ] ;						// �e�N�X�`�����W
} ;

// �X�L�����b�V���^�C�v���_�\����(�֘A�{�[���S�ȓ�)
struct MV1_VERTEX_SKIN_B4_D3D9
{
	VECTOR					Position ;							// ���W
	unsigned char			MatrixIndex[ 4 ] ;					// �s��C���f�b�N�X( �s��ԍ��~�S )
	float					MatrixWeight[ 4 ] ;					// �s��E�G�C�g
	VECTOR					Normal ;							// �@��
	COLOR_U8				DiffuseColor ;						// �f�B�t���[�Y�F
	COLOR_U8				SpecularColor ;						// �X�y�L�����[�F
	float					UVs[ 1 ][ 4 ] ;						// �e�N�X�`�����W
} ;

// �X�L�����b�V���^�C�v�o���v�}�b�v�p���_�\����(�֘A�{�[���S�ȓ�)
struct MV1_VERTEX_SKIN_B4_BUMP_D3D9
{
	VECTOR					Position ;							// ���W
	unsigned char			MatrixIndex[ 4 ] ;					// �s��C���f�b�N�X( �s��ԍ��~�S )
	float					MatrixWeight[ 4 ] ;					// �s��E�G�C�g
	VECTOR					Tangent ;							// ���_�̐ڐ�
	VECTOR					Binormal ;							// ���_�̏]�@��
	VECTOR					Normal ;							// �@��
	COLOR_U8				DiffuseColor ;						// �f�B�t���[�Y�F
	COLOR_U8				SpecularColor ;						// �X�y�L�����[�F
	float					UVs[ 1 ][ 4 ] ;						// �e�N�X�`�����W
} ;

// �X�L�����b�V���^�C�v���_�\����(�֘A�{�[���W�ȓ�)
struct MV1_VERTEX_SKIN_B8_D3D9
{
	VECTOR					Position ;							// ���W
	unsigned char			MatrixIndex[ 8 ] ;					// �s��C���f�b�N�X( �s��ԍ��~�S )
	float					MatrixWeight[ 8 ] ;					// �s��E�G�C�g
	VECTOR					Normal ;							// �@��
	COLOR_U8				DiffuseColor ;						// �f�B�t���[�Y�F
	COLOR_U8				SpecularColor ;						// �X�y�L�����[�F
	float					UVs[ 1 ][ 4 ] ;						// �e�N�X�`�����W
} ;

// �X�L�����b�V���^�C�v�o���v�}�b�v�p���_�\����(�֘A�{�[���W�ȓ�)
struct MV1_VERTEX_SKIN_B8_BUMP_D3D9
{
	VECTOR					Position ;							// ���W
	unsigned char			MatrixIndex[ 8 ] ;					// �s��C���f�b�N�X( �s��ԍ��~�W )
	float					MatrixWeight[ 8 ] ;					// �s��E�G�C�g
	VECTOR					Tangent ;							// ���_�̐ڐ�
	VECTOR					Binormal ;							// ���_�̏]�@��
	VECTOR					Normal ;							// �@��
	COLOR_U8				DiffuseColor ;						// �f�B�t���[�Y�F
	COLOR_U8				SpecularColor ;						// �X�y�L�����[�F
	float					UVs[ 1 ][ 4 ] ;						// �e�N�X�`�����W
} ;

// Direct3D9�p ���f���f�[�^�Ǘ��p�\����
struct MV1_MODEL_MANAGE_DIRECT3D9
{
	int Dummy ;
} ;

// Direct3D9�p���_�o�b�t�@���
struct MV1_VERTEXBUFFER_DIRECT3D9
{
	DWORD						FVF ;							// ���_�o�b�t�@�쐬���Ɏg�p�����e�u�e
	D_IDirect3DVertexBuffer9	*VertexBuffer ;					// ���_�o�b�t�@
	D_IDirect3DIndexBuffer9		*IndexBuffer ;					// �C���f�b�N�X�o�b�t�@
} ;

// Direct3D9�p�g���C�A���O�����X�g��f�[�^���
struct MV1_TRIANGLE_LIST_BASE_DIRECT3D9
{
	MV1_VERTEX_SIMPLE_D3D9		*TempSimpleVertex ;					// �V�F�[�_�[���g�p�����ɕ`�悷��ꍇ�̒��_�f�[�^
	MV1_VERTEX_SIMPLE_TOL_D3D9	*TempToonOutLineSimpleVertex ;		// �V�F�[�_�[���g�p�����ɕ`�悷��ꍇ�̃g�D�[���̗֊s���`��p���_�f�[�^
} ;

// Direct3D9�p�g���C�A���O�����X�g���
struct MV1_TRIANGLE_LIST_DIRECT3D9
{
	D_IDirect3DVertexBuffer9	*VertexBuffer ;					// ���_�o�b�t�@
} ;

// �������ϐ��錾 --------------------------------------------------------------

extern MV1_MODEL_MANAGE_DIRECT3D9 MV1Man_D3D9 ;

// �֐��v���g�^�C�v�錾-----------------------------------------------------------

// ���ˑ��֐�
extern	int				MV1_D3D9_Terminate_PF( void ) ;																// ���f���@�\�̌�n��
extern	int				MV1_D3D9_TerminateModelBaseHandle_PF( MV1_MODEL_BASE *ModelBase ) ;							// ���f���f�[�^�n���h���̌�n��
extern	int				MV1_D3D9_TerminateTriangleListBaseTempBuffer_PF( MV1_TRIANGLE_LIST_BASE *MBTList ) ;				// �g���C�A���O�����X�g�̈ꎞ�����p�̃o�b�t�@���J������
extern	void			MV1_D3D9_SetupPackDrawInfo_PF( MV1_MODEL_BASE *ModelBase ) ;								// ���������`��֌W�̏����Z�b�g�A�b�v����
extern	int				MV1_D3D9_SetupVertexBufferBase_PF( int MV1ModelBaseHandle, int DuplicateNum = 1, int ASyncThread = FALSE ) ;	// ���f����f�[�^�̒��_�o�b�t�@�̃Z�b�g�A�b�v������( -1:�G���[ )
extern	int				MV1_D3D9_SetupVertexBuffer_PF( int MHandle, int ASyncThread = FALSE ) ;						// ���f���f�[�^�̒��_�o�b�t�@�̃Z�b�g�A�b�v������( -1:�G���[ )
extern	int				MV1_D3D9_TerminateVertexBufferBase_PF( int MV1ModelBaseHandle ) ;							// ���_�o�b�t�@�̌�n��������( -1:�G���[ )
extern	int				MV1_D3D9_TerminateVertexBuffer_PF( int MV1ModelHandle ) ;									// ���_�o�b�t�@�̌�n��������( -1:�G���[ )
extern	int				MV1_D3D9_SetupShapeVertex_PF( int MHandle ) ;												// �V�F�C�v�f�[�^�̃Z�b�g�A�b�v������
extern	int				MV1_D3D9_BeginRender_PF( MV1_MODEL *Model ) ;												// �R�c���f���̃����_�����O�̏������s��
extern	int				MV1_D3D9_EndRender_PF( void ) ;																// �R�c���f���̃����_�����O�̌�n�����s��
extern	void			MV1_D3D9_DrawMesh_PF( MV1_MESH *Mesh, int TriangleListIndex = -1 ) ;						// ���b�V���`�敔���𔲂��o��������

#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

#endif // DX_NON_MODEL

#endif // DX_NON_DIRECT3D9

#endif // __DXMODELD3D9_H__
