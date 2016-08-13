// -------------------------------------------------------------------------------
// 
// 		�c�w���C�u����		WindowsOS�p���f���f�[�^����v���O�����w�b�_�t�@�C��
// 
// 				Ver 3.16d
// 
// -------------------------------------------------------------------------------

#ifndef __DXMODELWIN_H__
#define __DXMODELWIN_H__

// �C���N���[�h ------------------------------------------------------------------
#include "../DxCompileConfig.h"

#ifndef DX_NON_MODEL

#include "../DxLib.h"
#include "../DxModel.h"
#include "DxModelD3D9.h"
#include "DxModelD3D11.h"

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// �}�N����` --------------------------------------------------------------------

// �\���̒�` --------------------------------------------------------------------

// Windows�p ���_�o�b�t�@���ˑ����
struct MV1_VERTEXBUFFER_PF
{
	union
	{
		int									Dummy ;
#ifndef DX_NON_DIRECT3D11
		MV1_VERTEXBUFFER_DIRECT3D11			D3D11 ;		// Direct3D11�p���_�o�b�t�@���
#endif // DX_NON_DIRECT3D11
#ifndef DX_NON_DIRECT3D9
		MV1_VERTEXBUFFER_DIRECT3D9			D3D9 ;		// Direct3D9�p���_�o�b�t�@���
#endif // DX_NON_DIRECT3D9
	} ;
} ;

// Windows�p �g���C�A���O�����X�g���ˑ����
struct MV1_TRIANGLE_LIST_PF
{
	union
	{
		int									Dummy ;
#ifndef DX_NON_DIRECT3D11
		MV1_TRIANGLE_LIST_DIRECT3D11		D3D11 ;		// Direct3D11�p�g���C�A���O�����X�g���
#endif // DX_NON_DIRECT3D11
#ifndef DX_NON_DIRECT3D9
		MV1_TRIANGLE_LIST_DIRECT3D9			D3D9 ;		// Direct3D9�p�g���C�A���O�����X�g���
#endif // DX_NON_DIRECT3D9
	} ;
} ;

// Windows�p �g���C�A���O�����X�g��f�[�^���ˑ����
struct MV1_TRIANGLE_LIST_BASE_PF
{
	union
	{
		int									Dummy ;
#ifndef DX_NON_DIRECT3D11
		MV1_TRIANGLE_LIST_BASE_DIRECT3D11	D3D11 ;		// Direct3D11�p�g���C�A���O�����X�g��f�[�^���
#endif // DX_NON_DIRECT3D11
#ifndef DX_NON_DIRECT3D9
		MV1_TRIANGLE_LIST_BASE_DIRECT3D9	D3D9 ;		// Direct3D9�p�g���C�A���O�����X�g��f�[�^���
#endif // DX_NON_DIRECT3D9
	} ;
} ;

// �������ϐ��錾 --------------------------------------------------------------

// �֐��v���g�^�C�v�錾-----------------------------------------------------------

#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

#endif // DX_NON_MODEL

#endif // __DXMODELWIN_H__
