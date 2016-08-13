//-----------------------------------------------------------------------------
// 
// 		�c�w���C�u����		WindowsOS�p�}�X�N�f�[�^�Ǘ��v���O����
// 
//  	Ver 3.16d
// 
//-----------------------------------------------------------------------------

// �c�w���C�u�����쐬���p��`
#define __DX_MAKE

#include "../DxCompileConfig.h"

#ifndef DX_NON_GRAPHICS

#ifndef DX_NON_MASK

// �C���N���[�h ---------------------------------------------------------------
#include "DxGraphicsWin.h"
#include "DxMaskWin.h"
#include "DxMaskD3D9.h"
#include "DxMaskD3D11.h"

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// �}�N����` -----------------------------------------------------------------

// �\���̐錾 -----------------------------------------------------------------

// �f�[�^��` -----------------------------------------------------------------

// �֐��錾 -------------------------------------------------------------------

// �v���O���� -----------------------------------------------------------------

// ���ˑ��֐�

// �}�X�N�X�N���[�����쐬����֐�
extern	int			Mask_CreateScreenFunction_Timing0_PF( void )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Mask_D3D11_CreateScreenFunction_Timing0_PF() ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Mask_D3D9_CreateScreenFunction_Timing0_PF() ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// �}�X�N�X�N���[�����쐬����֐�
extern	int			Mask_CreateScreenFunction_Timing1_PF( int Width, int Height )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Mask_D3D11_CreateScreenFunction_Timing1_PF(  Width,  Height ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Mask_D3D9_CreateScreenFunction_Timing1_PF(  Width,  Height ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// �}�X�N�X�N���[�����쐬����֐�
extern	int			Mask_CreateScreenFunction_Timing2_PF( int MaskBufferSizeXOld, int MaskBufferSizeYOld )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Mask_D3D11_CreateScreenFunction_Timing2_PF(  MaskBufferSizeXOld,  MaskBufferSizeYOld ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Mask_D3D9_CreateScreenFunction_Timing2_PF(  MaskBufferSizeXOld,  MaskBufferSizeYOld ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// �}�X�N�X�N���[�����ꎞ�폜����
extern	int			Mask_ReleaseSurface_PF( void )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Mask_D3D11_ReleaseSurface_PF() ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Mask_D3D9_ReleaseSurface_PF() ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// �}�X�N�g�p���[�h��ύX
extern	int			Mask_SetUseMaskScreenFlag_PF( void )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Mask_D3D11_SetUseMaskScreenFlag_PF() ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Mask_D3D9_SetUseMaskScreenFlag_PF() ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// �}�X�N���g�p�����`��̑O�ɌĂԊ֐�( ���łɃT�u�o�b�t�@���g�p�����`��G���A�@�\���g�p���Ă���ꍇ�̏���������Ă��܂��Ă���� )
extern	int			Mask_DrawBeginFunction_PF( RECT *Rect )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Mask_D3D11_DrawBeginFunction_PF( Rect ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Mask_D3D9_DrawBeginFunction_PF( Rect ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// �}�X�N���g�p�����`��̌�ɌĂԊ֐�( ���łɃT�u�o�b�t�@���g�p�����`��G���A�@�\���g�p���Ă���ꍇ�̏���������Ă��܂��Ă���� )
extern	int			Mask_DrawAfterFunction_PF( RECT *Rect )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Mask_D3D11_DrawAfterFunction_PF( Rect ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Mask_D3D9_DrawAfterFunction_PF( Rect ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// �}�X�N�X�N���[�����w��̐F�œh��Ԃ�
extern	int			Mask_FillMaskScreen_PF( int Flag )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Mask_D3D11_FillMaskScreen_PF( Flag ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Mask_D3D9_FillMaskScreen_PF( Flag ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// �w��̈�̃}�X�N�C���[�W�e�N�X�`�����X�V����
extern	int			Mask_UpdateMaskImageTexture_PF( RECT *Rect )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return Mask_D3D11_UpdateMaskImageTexture_PF( Rect ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return Mask_D3D9_UpdateMaskImageTexture_PF( Rect ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}






#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

#endif // DX_NON_MASK

#endif // DX_NON_GRAPHICS
