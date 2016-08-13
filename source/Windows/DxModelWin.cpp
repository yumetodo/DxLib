//-----------------------------------------------------------------------------
// 
// 		�c�w���C�u����		WindowsOS�p���f���f�[�^����v���O����
// 
//  	Ver 3.16d
// 
//-----------------------------------------------------------------------------

// �c�w���C�u�����쐬���p��`
#define __DX_MAKE

#include "DxModelWin.h"

#ifndef DX_NON_MODEL

// �C���N���[�h ---------------------------------------------------------------
#include "DxGraphicsWin.h"
#include "DxGraphicsD3D9.h"
#include "DxGraphicsD3D11.h"
#include "DxModelD3D9.h"
#include "DxModelD3D11.h"
#include "DxWinAPI.h"
#include "DxWindow.h"
#include "../DxMask.h"

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// �}�N����` -----------------------------------------------------------------

// �\���̐錾 -----------------------------------------------------------------

// �f�[�^��` -----------------------------------------------------------------

// �֐��錾 -------------------------------------------------------------------


// �v���O���� -----------------------------------------------------------------

// ���f���@�\�̌�n��
extern int MV1_Terminate_PF( void )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return MV1_D3D11_Terminate_PF() ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return MV1_D3D9_Terminate_PF() ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// ���f���f�[�^�n���h���̌�n��
extern int 	MV1_TerminateModelBaseHandle_PF( MV1_MODEL_BASE *ModelBase )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return MV1_D3D11_TerminateModelBaseHandle_PF( ModelBase ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return MV1_D3D9_TerminateModelBaseHandle_PF( ModelBase ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// �g���C�A���O�����X�g�̈ꎞ�����p�̃o�b�t�@���J������
extern int MV1_TerminateTriangleListBaseTempBuffer_PF( MV1_TRIANGLE_LIST_BASE *MBTList )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return MV1_D3D11_TerminateTriangleListBaseTempBuffer_PF( MBTList ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return MV1_D3D9_TerminateTriangleListBaseTempBuffer_PF( MBTList ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// ���������`��֌W�̏����Z�b�g�A�b�v����
extern void MV1_SetupPackDrawInfo_PF( MV1_MODEL_BASE *ModelBase )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		MV1_D3D11_SetupPackDrawInfo_PF( ModelBase ) ;
		break ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		MV1_D3D9_SetupPackDrawInfo_PF( ModelBase ) ;
		break ;
#endif // DX_NON_DIRECT3D9

	default :
		break ;
	}
}

// ���_�o�b�t�@�̃Z�b�g�A�b�v������( -1:�G���[ )
extern int MV1_SetupVertexBufferBase_PF( int MV1ModelBaseHandle, int DuplicateNum, int ASyncThread )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return MV1_D3D11_SetupVertexBufferBase_PF( MV1ModelBaseHandle, DuplicateNum, ASyncThread ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return MV1_D3D9_SetupVertexBufferBase_PF( MV1ModelBaseHandle, DuplicateNum, ASyncThread ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// ���f���f�[�^�̒��_�o�b�t�@�̃Z�b�g�A�b�v������( -1:�G���[ )
extern int MV1_SetupVertexBuffer_PF( int MHandle, int ASyncThread )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return MV1_D3D11_SetupVertexBuffer_PF( MHandle, ASyncThread ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return MV1_D3D9_SetupVertexBuffer_PF( MHandle, ASyncThread ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// ���_�o�b�t�@�̌�n��������( -1:�G���[ )
extern int MV1_TerminateVertexBufferBase_PF( int MV1ModelBaseHandle )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return MV1_D3D11_TerminateVertexBufferBase_PF( MV1ModelBaseHandle ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return MV1_D3D9_TerminateVertexBufferBase_PF( MV1ModelBaseHandle ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// ���_�o�b�t�@�̌�n��������( -1:�G���[ )
extern int MV1_TerminateVertexBuffer_PF( int MV1ModelHandle )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return MV1_D3D11_TerminateVertexBuffer_PF( MV1ModelHandle ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return MV1_D3D9_TerminateVertexBuffer_PF( MV1ModelHandle ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// �V�F�C�v�f�[�^�̃Z�b�g�A�b�v������
extern int MV1_SetupShapeVertex_PF( int MHandle )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return MV1_D3D11_SetupShapeVertex_PF( MHandle ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return MV1_D3D9_SetupShapeVertex_PF( MHandle ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// �R�c���f���̃����_�����O�̏������s��
extern int MV1_BeginRender_PF( MV1_MODEL *Model )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return MV1_D3D11_BeginRender_PF( Model ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return MV1_D3D9_BeginRender_PF( Model ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// �R�c���f���̃����_�����O�̌�n�����s��
extern int MV1_EndRender_PF( void )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return MV1_D3D11_EndRender_PF() ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return MV1_D3D9_EndRender_PF() ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// ���b�V���`�敔���𔲂��o��������
extern void MV1_DrawMesh_PF( MV1_MESH *Mesh, int TriangleListIndex )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		MV1_D3D11_DrawMesh_PF( Mesh, TriangleListIndex ) ;
		break ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		MV1_D3D9_DrawMesh_PF( Mesh, TriangleListIndex ) ;
		break ;
#endif // DX_NON_DIRECT3D9

	default :
		break ;
	}
}



#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

#endif  // DX_NON_MODEL
