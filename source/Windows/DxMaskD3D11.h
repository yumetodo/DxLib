// -------------------------------------------------------------------------------
// 
// 		�c�w���C�u����		�}�X�N�����v���O����( Direct3D11 )�w�b�_�t�@�C��
// 
// 				Ver 3.16d
// 
// -------------------------------------------------------------------------------

#ifndef __DXMASKD3D11_H__
#define __DXMASKD3D11_H__

#include "../DxCompileConfig.h"

#ifndef DX_NON_GRAPHICS

#ifndef DX_NON_DIRECT3D11

#ifndef DX_NON_MASK

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

#define MASKD3D11					MaskManageData_D3D11


// �\���̒�` --------------------------------------------------------------------


// Direct3D11 ���g�����O���t�B�b�N�X�������̍\����
struct MASKMANAGEDATA_DIRECT3D11
{
	int								MaskTextureSizeX ;				// �}�X�N�e�N�X�`���̃T�C�Y
	int								MaskTextureSizeY ;

	D_ID3D11Texture2D *				MaskScreenTexture ;				// �}�X�N�����p����ʃe�N�X�`��
	D_ID3D11ShaderResourceView *	MaskScreenTextureSRV ;			// �}�X�N�����p����ʃe�N�X�`���V�F�[�_�[���\�[�X�r���[
	D_ID3D11RenderTargetView *		MaskScreenTextureRTV ;			// �}�X�N�����p����ʃe�N�X�`�������_�����O�^�[�Q�b�g�r���[

	D_ID3D11Texture2D *				MaskScreenTextureOld ;			// �Â��}�X�N�����p����ʃe�N�X�`��
	D_ID3D11ShaderResourceView *	MaskScreenTextureOldSRV ;		// �Â��}�X�N�����p����ʃe�N�X�`���V�F�[�_�[���\�[�X�r���[
	D_ID3D11RenderTargetView *		MaskScreenTextureOldRTV ;		// �Â��}�X�N�����p����ʃe�N�X�`�������_�����O�^�[�Q�b�g�r���[

	D_ID3D11Texture2D *				MaskImageTexture ;				// �}�X�N�C���[�W�e�N�X�`��
	D_ID3D11ShaderResourceView *	MaskImageTextureSRV ;			// �}�X�N�C���[�W�e�N�X�`���V�F�[�_�[���\�[�X�r���[

	D_ID3D11Texture2D *				DestTargetTexture2D ;			// �`��Ώۂ̃e�N�X�`��
	D_ID3D11ShaderResourceView *	DestTargetSRV ;					// �`��Ώۂ̃V�F�[�_�[���\�[�X�r���[
	D_ID3D11RenderTargetView *		DestTargetRTV ;					// �`��Ώۂ̃����_�����O�^�[�Q�b�g�r���[

	D_ID3D11Texture2D *				DestTargetDepthTexture2D ;		// �`��Ώۂ̐[�x�e�N�X�`��
	D_ID3D11DepthStencilView *		DestTargetDepthDSV ;			// �`��Ώۂ̐[�x�X�e���V���r���[
} ;


// �������ϐ��錾 --------------------------------------------------------------

extern MASKMANAGEDATA_DIRECT3D11 MaskManageData_D3D11 ;

// �֐��v���g�^�C�v�錾-----------------------------------------------------------


// ���ˑ��֐�
extern	int			Mask_D3D11_CreateScreenFunction_Timing0_PF( void ) ;														// �}�X�N�X�N���[�����쐬����֐�
extern	int			Mask_D3D11_CreateScreenFunction_Timing1_PF( int Width, int Height ) ;									// �}�X�N�X�N���[�����쐬����֐�
extern	int			Mask_D3D11_CreateScreenFunction_Timing2_PF( int MaskBufferSizeXOld, int MaskBufferSizeYOld ) ;			// �}�X�N�X�N���[�����쐬����֐�
extern	int			Mask_D3D11_ReleaseSurface_PF( void ) ;																	// �}�X�N�X�N���[�����ꎞ�폜����
extern	int			Mask_D3D11_SetUseMaskScreenFlag_PF( void ) ;																// �}�X�N�g�p���[�h��ύX
extern	int			Mask_D3D11_DrawBeginFunction_PF( RECT *Rect ) ;															// �}�X�N���g�p�����`��̑O�ɌĂԊ֐�( ���łɃT�u�o�b�t�@���g�p�����`��G���A�@�\���g�p���Ă���ꍇ�̏���������Ă��܂��Ă���� )
extern	int			Mask_D3D11_DrawAfterFunction_PF( RECT *Rect ) ;															// �}�X�N���g�p�����`��̌�ɌĂԊ֐�( ���łɃT�u�o�b�t�@���g�p�����`��G���A�@�\���g�p���Ă���ꍇ�̏���������Ă��܂��Ă���� )
extern	int			Mask_D3D11_FillMaskScreen_PF( int Flag ) ;																// �}�X�N�X�N���[�����w��̐F�œh��Ԃ�
extern	int			Mask_D3D11_UpdateMaskImageTexture_PF( RECT *Rect ) ;															// �w��̈�̃}�X�N�C���[�W�e�N�X�`�����X�V����


#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

#endif // DX_NON_MASK

#endif // DX_NON_DIRECT3D11

#endif // DX_NON_GRAPHICS

#endif // __DXMASKD3D11_H__

