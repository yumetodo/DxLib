//-----------------------------------------------------------------------------
// 
// 		�c�w���C�u����		WindowsOS�pBaseImage�v���O����
// 
//  	Ver 3.16d
// 
//-----------------------------------------------------------------------------

// �c�w���C�u�����쐬���p��`
#define __DX_MAKE

#include "../DxCompileConfig.h"

// �C���N���[�h ---------------------------------------------------------------
#include "DxBaseImageWin.h"

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// �}�N����` -----------------------------------------------------------------

// �\���̐錾 -----------------------------------------------------------------

// �f�[�^��` -----------------------------------------------------------------

int ( *DefaultImageLoadFunc_PF[] )( STREAMDATA *Src, BASEIMAGE *BaseImage, int GetFormatOnly ) =
{
	NULL
} ;

// �֐��錾 -------------------------------------------------------------------

// �v���O���� -----------------------------------------------------------------

// ���ˑ��������E�I���֐�

// ��{�C���[�W�Ǘ����̊��ˑ������̏�����
extern int InitializeBaseImageManage_PF( void )
{
	return 0 ;
}

// ��{�C���[�W�Ǘ����̊��ˑ������̌�n��
extern int TerminateBaseImageManage_PF( void )
{
	return 0 ;
}

#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

