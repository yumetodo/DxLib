// -------------------------------------------------------------------------------
// 
// 		�c�w���C�u����		�}�X�N�����v���O����( Direct3D9 )�w�b�_�t�@�C��
// 
// 				Ver 3.16d
// 
// -------------------------------------------------------------------------------

#ifndef __DXMASKD3D9_H__
#define __DXMASKD3D9_H__

#include "../DxCompileConfig.h"

#ifndef DX_NON_GRAPHICS

#ifndef DX_NON_DIRECT3D9

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

#define MASKD3D9					MaskManageData_D3D9


// �\���̒�` --------------------------------------------------------------------


// Direct3D9 ���g�����O���t�B�b�N�X�������̍\����
struct MASKMANAGEDATA_DIRECT3D9
{
	int						MaskTextureSizeX, MaskTextureSizeY ;	// �}�X�N�e�N�X�`���̃T�C�Y
	D_IDirect3DTexture9		*MaskImageTexture ;						// �}�X�N�C���[�W�e�N�X�`��
//	D_IDirect3DSurface9		*MaskImageSurface ;						// �}�X�N�C���[�W�T�[�t�F�X
//	D_IDirect3DSurface9		*MaskMemImageSurface ;					// �}�X�N�������C���[�W�T�[�t�F�X
	D_IDirect3DTexture9		*MaskScreenTexture ;					// �}�X�N�X�N���[���e�N�X�`��
	D_IDirect3DSurface9		*MaskScreenSurface ;					// �}�X�N�X�N���[���T�[�t�F�X

	D_IDirect3DTexture9		*MaskImageTextureOld ;
//	D_IDirect3DSurface9		*MaskImageSurfaceOld ;
//	D_IDirect3DSurface9		*MaskMemImageSurfaceOld ;
	D_IDirect3DTexture9		*MaskScreenTextureOld ;
	D_IDirect3DSurface9		*MaskScreenSurfaceOld ;

	D_IDirect3DSurface9		*UseBackupSurface ;
	D_IDirect3DSurface9		*DestTargetSurface ;
} ;


// �������ϐ��錾 --------------------------------------------------------------

extern MASKMANAGEDATA_DIRECT3D9 MaskManageData_D3D9 ;

// �֐��v���g�^�C�v�錾-----------------------------------------------------------


// ���ˑ��֐�
extern	int			Mask_D3D9_CreateScreenFunction_Timing0_PF( void ) ;														// �}�X�N�X�N���[�����쐬����֐�
extern	int			Mask_D3D9_CreateScreenFunction_Timing1_PF( int Width, int Height ) ;									// �}�X�N�X�N���[�����쐬����֐�
extern	int			Mask_D3D9_CreateScreenFunction_Timing2_PF( int MaskBufferSizeXOld, int MaskBufferSizeYOld ) ;			// �}�X�N�X�N���[�����쐬����֐�
extern	int			Mask_D3D9_ReleaseSurface_PF( void ) ;																	// �}�X�N�X�N���[�����ꎞ�폜����
extern	int			Mask_D3D9_SetUseMaskScreenFlag_PF( void ) ;																// �}�X�N�g�p���[�h��ύX
extern	int			Mask_D3D9_DrawBeginFunction_PF( RECT *Rect ) ;															// �}�X�N���g�p�����`��̑O�ɌĂԊ֐�( ���łɃT�u�o�b�t�@���g�p�����`��G���A�@�\���g�p���Ă���ꍇ�̏���������Ă��܂��Ă���� )
extern	int			Mask_D3D9_DrawAfterFunction_PF( RECT *Rect ) ;															// �}�X�N���g�p�����`��̌�ɌĂԊ֐�( ���łɃT�u�o�b�t�@���g�p�����`��G���A�@�\���g�p���Ă���ꍇ�̏���������Ă��܂��Ă���� )
extern	int			Mask_D3D9_FillMaskScreen_PF( int Flag ) ;																// �}�X�N�X�N���[�����w��̐F�œh��Ԃ�
extern	int			Mask_D3D9_UpdateMaskImageTexture_PF( RECT *Rect ) ;															// �w��̈�̃}�X�N�C���[�W�e�N�X�`�����X�V����


#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

#endif // DX_NON_MASK

#endif // DX_NON_DIRECT3D9

#endif // DX_NON_GRAPHICS

#endif // __DXMASKD3D9_H__

