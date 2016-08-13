//-----------------------------------------------------------------------------
// 
// 		�c�w���C�u����		�}�X�N�����v���O����( Direct3D9 )
// 
//  	Ver 3.16d
// 
//-----------------------------------------------------------------------------

// �c�w���C�u�����쐬���p��`
#define __DX_MAKE

#include "../DxCompileConfig.h"

#ifndef DX_NON_GRAPHICS

#ifndef DX_NON_DIRECT3D9

#ifndef DX_NON_MASK

// �C���N���[�h----------------------------------------------------------------
#include "../DxLib.h"
#include "../DxStatic.h"
#include "../DxSystem.h"
#include "../DxModel.h"
#include "../DxMath.h"
//#include "../DxBaseFunc.h"
#include "../DxLog.h"
#include "../DxMask.h"
#include "DxMaskD3D9.h"
#include "DxGraphicsD3D9.h"
#include "DxGraphicsWin.h"

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// �}�N����`------------------------------------------------------------------

// �^��`----------------------------------------------------------------------

// �f�[�^�錾------------------------------------------------------------------

MASKMANAGEDATA_DIRECT3D9 MaskManageData_D3D9 ;

// �֐��v���g�^�C�v�錾 -------------------------------------------------------

// �v���O����------------------------------------------------------------------

// ���ˑ��֐�

// �}�X�N�X�N���[�����쐬����֐�
extern int Mask_D3D9_CreateScreenFunction_Timing0_PF( void )
{
	MASKD3D9.MaskImageTextureOld     = MASKD3D9.MaskImageTexture ;
	MASKD3D9.MaskImageTexture        = NULL ;

//	MASKD3D9.MaskImageSurfaceOld     = MASKD3D9.MaskImageSurface ;
//	MASKD3D9.MaskImageSurface        = NULL ;

//	MASKD3D9.MaskMemImageSurfaceOld  = MASKD3D9.MaskMemImageSurface ;
//	MASKD3D9.MaskMemImageSurface     = NULL ;

	MASKD3D9.MaskScreenTextureOld    = MASKD3D9.MaskScreenTexture ;
	MASKD3D9.MaskScreenTexture       = NULL ;

	MASKD3D9.MaskScreenSurfaceOld    = MASKD3D9.MaskScreenSurface ;
	MASKD3D9.MaskScreenSurface       = NULL ;

	// ����I��
	return 0 ;
}


// �}�X�N�X�N���[�����쐬����֐�
extern int Mask_D3D9_CreateScreenFunction_Timing1_PF( int Width, int Height )
{
	UINT w, h ;

	// �J���[�o�b�t�@���A���t�@�o�b�t�@���쐬�s�\�ȏꍇ�͂��G���[
	if( GD3D9.Device.Caps.MaskColorFormat == D_D3DFMT_UNKNOWN )
	{
		return DxLib_ErrorUTF16LE( "\xde\x30\xb9\x30\xaf\x30\xcf\x63\x3b\x75\x28\x75\xab\x30\xe9\x30\xfc\x30\xd0\x30\xc3\x30\xd5\x30\xa1\x30\x67\x30\x7f\x4f\x28\x75\x67\x30\x4d\x30\x8b\x30\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\xd5\x30\xa9\x30\xfc\x30\xde\x30\xc3\x30\xc8\x30\x4c\x30\x42\x30\x8a\x30\x7e\x30\x5b\x30\x93\x30\x67\x30\x57\x30\x5f\x30\x00"/*@ L"�}�X�N�`��p�J���[�o�b�t�@�Ŏg�p�ł���e�N�X�`���t�H�[�}�b�g������܂���ł���" @*/ ) ;
	}

	if( GD3D9.Device.Caps.MaskAlphaFormat == D_D3DFMT_UNKNOWN )
	{
		return DxLib_ErrorUTF16LE( "\xde\x30\xb9\x30\xaf\x30\xcf\x63\x3b\x75\x28\x75\xa2\x30\xeb\x30\xd5\x30\xa1\x30\xc1\x30\xe3\x30\xf3\x30\xcd\x30\xeb\x30\xd0\x30\xc3\x30\xd5\x30\xa1\x30\x67\x30\x7f\x4f\x28\x75\x67\x30\x4d\x30\x8b\x30\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\xd5\x30\xa9\x30\xfc\x30\xde\x30\xc3\x30\xc8\x30\x4c\x30\x42\x30\x8a\x30\x7e\x30\x5b\x30\x93\x30\x67\x30\x57\x30\x5f\x30\x00"/*@ L"�}�X�N�`��p�A���t�@�`�����l���o�b�t�@�Ŏg�p�ł���e�N�X�`���t�H�[�}�b�g������܂���ł���" @*/ ) ;
	}

	// ��ʃT�C�Y�����܂� 2 ��n��̒l������o��
	for( w = 1 ; w < ( UINT )Width  ; w <<= 1 ){}
	for( h = 1 ; h < ( UINT )Height ; h <<= 1 ){}
	MASKD3D9.MaskTextureSizeX = ( int )w ;
	MASKD3D9.MaskTextureSizeY = ( int )h ;

	// �}�X�N�p�C���[�W�e�N�X�`���̍쐬
	if( MASKD3D9.MaskImageTexture == NULL )
	{
		if( Direct3DDevice9_CreateTexture( w, h, 1, D_D3DUSAGE_DYNAMIC, GD3D9.Device.Caps.MaskAlphaFormat, D_D3DPOOL_DEFAULT, &MASKD3D9.MaskImageTexture, NULL ) != D_D3D_OK )
			return DxLib_ErrorUTF16LE( "\xde\x30\xb9\x30\xaf\x30\x28\x75\xa4\x30\xe1\x30\xfc\x30\xb8\x30\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x00"/*@ L"�}�X�N�p�C���[�W�e�N�X�`���̍쐬�Ɏ��s���܂���" @*/ ) ;

		// �}�X�N�p�C���[�W�e�N�X�`���̏�����
		{
			D_D3DLOCKED_RECT LockRect ;
			int i, Width ;
			BYTE *Dest ;

			if( MASKD3D9.MaskImageTexture->LockRect( 0, &LockRect, NULL, 0 ) == D_D3D_OK )
			{
				Dest = ( BYTE * )LockRect.pBits ;
				Width = Graphics_D3D9_GetD3DFormatColorData( GD3D9.Device.Caps.MaskAlphaFormat )->PixelByte * MASKD3D9.MaskTextureSizeX ;
				for( i = 0 ; i < MASKD3D9.MaskTextureSizeY ; i ++, Dest += LockRect.Pitch )
					_MEMSET( Dest, 0, ( size_t )Width ) ;

				MASKD3D9.MaskImageTexture->UnlockRect( 0 ) ;
			}
		}
	}

	// �}�X�N�p�X�N���[���e�N�X�`���̍쐬
	if( MASKD3D9.MaskScreenTexture == NULL )
	{
		if( Direct3DDevice9_CreateTexture( w, h, 1, D_D3DUSAGE_RENDERTARGET, GD3D9.Device.Caps.ScreenFormat, D_D3DPOOL_DEFAULT, &MASKD3D9.MaskScreenTexture, NULL ) != D_D3D_OK )
		{
			return DxLib_ErrorUTF16LE( "\xde\x30\xb9\x30\xaf\x30\x28\x75\xb9\x30\xaf\x30\xea\x30\xfc\x30\xf3\x30\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x00"/*@ L"�}�X�N�p�X�N���[���e�N�X�`���̍쐬�Ɏ��s���܂���" @*/ ) ;
		}
	}

	// �}�X�N�p�X�N���[���T�[�t�F�X�̎擾
	if( MASKD3D9.MaskScreenTexture && MASKD3D9.MaskScreenSurface == NULL )
	{
		MASKD3D9.MaskScreenTexture->GetSurfaceLevel( 0, &MASKD3D9.MaskScreenSurface ) ;
	}

	// ����I��
	return 0 ;
}

// �}�X�N�X�N���[�����쐬����֐�
extern	int			Mask_D3D9_CreateScreenFunction_Timing2_PF( int MaskBufferSizeXOld, int MaskBufferSizeYOld )
{
	RECT UpdateRect ;
	int i ;

	// �}�X�N�p�C���[�W�e�N�X�`���ɓ��e��]��
	UpdateRect.left   = 0 ;
	UpdateRect.top    = 0 ;
	UpdateRect.right  = MaskBufferSizeXOld ;
	UpdateRect.bottom = MaskBufferSizeYOld ;
	Mask_D3D9_UpdateMaskImageTexture_PF( &UpdateRect ) ;

//	MASKD3D9.MaskImageSurfaceOld->Release() ;
//	MASKD3D9.MaskImageSurfaceOld = NULL ;

	MASKD3D9.MaskImageTextureOld->Release() ;
	MASKD3D9.MaskImageTextureOld = NULL ;

//	MASKD3D9.MaskMemImageSurfaceOld->Release() ;
//	MASKD3D9.MaskMemImageSurfaceOld = NULL ;

	// �}�X�N�p�X�N���[���e�N�X�`���ɍ��܂ł̓��e��]��
	Direct3DDevice9_StretchRect(
		MASKD3D9.MaskScreenSurfaceOld, &UpdateRect,
		MASKD3D9.MaskScreenSurface,    &UpdateRect, D_D3DTEXF_NONE ) ;

	// �����_�[�^�[�Q�b�g�ɂ���Ă�����ύX����
	for( i = 0 ; i < DX_RENDERTARGET_COUNT ; i ++ )
	{
		if( GD3D9.Device.State.TargetSurface[ i ] == MASKD3D9.MaskScreenSurfaceOld )
		{
			Graphics_D3D9_DeviceState_SetRenderTarget( MASKD3D9.MaskScreenSurface, i ) ;
		}
	}

	MASKD3D9.MaskScreenSurfaceOld->Release() ;
	MASKD3D9.MaskScreenSurfaceOld = NULL ;

	MASKD3D9.MaskScreenTextureOld->Release() ;
	MASKD3D9.MaskScreenTextureOld = NULL ;

	// ����I��
	return 0 ;
}

// �}�X�N�X�N���[�����ꎞ�폜����
extern	int			Mask_D3D9_ReleaseSurface_PF( void )
{
//	// �}�X�N�������C���[�W�p�T�[�t�F�X�̉��
//	if( MASKD3D9.MaskMemImageSurface )
//	{
//		MASKD3D9.MaskMemImageSurface->Release() ;
//		MASKD3D9.MaskMemImageSurface = NULL ;
//	}
//
//	// �}�X�N�C���[�W�p�T�[�t�F�X�̉��
//	if( MASKD3D9.MaskImageSurface )
//	{
//		MASKD3D9.MaskImageSurface->Release() ;
//		MASKD3D9.MaskImageSurface = NULL ;
//	}

	// �}�X�N�C���[�W�p�e�N�X�`���̉��
	if( MASKD3D9.MaskImageTexture )
	{
		MASKD3D9.MaskImageTexture->Release() ;
		MASKD3D9.MaskImageTexture = NULL ;
	}

	// �}�X�N�X�N���[���T�[�t�F�X�̉��
	if( MASKD3D9.MaskScreenSurface )
	{
		MASKD3D9.MaskScreenSurface->Release() ;
		MASKD3D9.MaskScreenSurface = NULL ;
	}

	// �}�X�N�X�N���[���e�N�X�`���̉��
	if( MASKD3D9.MaskScreenTexture )
	{
		MASKD3D9.MaskScreenTexture->Release() ;
		MASKD3D9.MaskScreenTexture = NULL ;
	}

	// ����I��
	return 0 ;
}

// �}�X�N�g�p���[�h��ύX
extern	int			Mask_D3D9_SetUseMaskScreenFlag_PF( void )
{
	IMAGEDATA *Image ;

	// �`���̉摜�f�[�^�A�h���X���擾����
	Image = Graphics_Image_GetData( GSYS.DrawSetting.TargetScreen[ 0 ] ) ;

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �`����ύX����

	// �}�X�N�T�[�t�F�X�����݂��Ă��Ċ��L���ȏꍇ�̓}�X�N�T�[�t�F�X��`��Ώ̂ɂ���
	if( MASKD.MaskValidFlag && MASKD3D9.MaskScreenSurface )
	{
		Graphics_D3D9_DeviceState_SetRenderTarget( MASKD3D9.MaskScreenSurface ) ;
	}
	else
	// �`��\�摜���L���ȏꍇ�͕`��\�摜��`��Ώۂɂ���
	if( Image )
	{
		if( Image->Hard.Draw[ 0 ].Tex->PF->D3D9.RenderTargetSurface )
		{
			Graphics_D3D9_DeviceState_SetRenderTarget( Image->Hard.Draw[ 0 ].Tex->PF->D3D9.RenderTargetSurface ) ;
		}
		else
		{
			Graphics_D3D9_DeviceState_SetRenderTarget( Image->Hard.Draw[ 0 ].Tex->PF->D3D9.Surface[ GSYS.DrawSetting.TargetScreenSurface[ 0 ] ] ) ;
		}
	}
	else
	{
		// ����ȊO�̏ꍇ�̓T�u�o�b�N�o�b�t�@���L���ȏꍇ�̓T�u�o�b�N�o�b�t�@��`��Ώ̂ɂ���
		Graphics_D3D9_DeviceState_SetRenderTarget( GD3D9.Device.Screen.SubBackBufferSurface ? GD3D9.Device.Screen.SubBackBufferSurface : GD3D9.Device.Screen.BackBufferSurface ) ;
	}

	// �g�p����y�o�b�t�@�̃Z�b�g�A�b�v
	Graphics_Screen_SetupUseZBuffer() ;

	// �r���[�|�[�g�����ɖ߂�
	GD3D9.Device.DrawSetting.CancelSettingEqualCheck = TRUE ;
	Graphics_D3D9_DeviceState_SetViewportEasy( GSYS.DrawSetting.DrawArea.left, GSYS.DrawSetting.DrawArea.top, GSYS.DrawSetting.DrawArea.right, GSYS.DrawSetting.DrawArea.bottom ) ;
	GD3D9.Device.DrawSetting.CancelSettingEqualCheck = FALSE ;

	// ����I��
	return 0 ;
}

// �}�X�N���g�p�����`��̑O�ɌĂԊ֐�( ���łɃT�u�o�b�t�@���g�p�����`��G���A�@�\���g�p���Ă���ꍇ�̏���������Ă��܂��Ă���� )
extern	int			Mask_D3D9_DrawBeginFunction_PF( RECT *Rect )
{
	IMAGEDATA *Image ;

	// �`���̉摜�f�[�^�A�h���X���擾����
	Image = Graphics_Image_GetData( GSYS.DrawSetting.TargetScreen[ 0 ] ) ;

	// �}�X�N���g�p���Ă���Ƃ��̂ݓ��ʂȏ���������
	if( MASKD.MaskValidFlag && MASKD3D9.MaskScreenSurface )
	{
		// �}�X�N���g�p���Ă���ꍇ�͕`�����}�X�N�X�N���[���ɂ���
		MASKD3D9.UseBackupSurface = MASKD3D9.MaskScreenSurface ;

		// �ŏI�o�͐�̌���
		
		// �`��\�摜���`���̏ꍇ�͂�����A
		// ����ȊO�̏ꍇ�̓T�u�o�b�N�o�b�t�@��
		// �g�p���Ă���ꍇ�̓T�u�o�b�N�o�b�t�@���A
		// �����ł͂Ȃ��ꍇ�̓o�b�N�o�b�t�@���o�͐�ɂ���
		if( Image )
		{
			MASKD3D9.DestTargetSurface = Image->Orig->Hard.Tex[ 0 ].PF->D3D9.Surface[ GSYS.DrawSetting.TargetScreenSurface[ 0 ] ] ;
		}
		else
		{
			MASKD3D9.DestTargetSurface = GD3D9.Device.Screen.SubBackBufferSurface ? GD3D9.Device.Screen.SubBackBufferSurface : GD3D9.Device.Screen.BackBufferSurface ;
		}

		// �ŏI�o�͐悩��}�X�N�X�N���[���Ɍ��݂̕`��󋵂��R�s�[����
		Direct3DDevice9_StretchRect(
			MASKD3D9.DestTargetSurface, Rect,
			MASKD3D9.MaskScreenSurface, Rect, D_D3DTEXF_NONE ) ; 
	}

	// ����I��
	return 0 ;
}

// �}�X�N���g�p�����`��̌�ɌĂԊ֐�( ���łɃT�u�o�b�t�@���g�p�����`��G���A�@�\���g�p���Ă���ꍇ�̏���������Ă��܂��Ă���� )
extern	int			Mask_D3D9_DrawAfterFunction_PF( RECT *Rect )
{
	IMAGEDATA *MaskScreenImage = NULL ;
	int UseZBufferFlag ;
//	VERTEX_2D Vert[ 4 ] ;
	VERTEX_BLENDTEX_2D Vert[ 4 ] ;
	GRAPHICS_HARDDATA_DIRECT3D9_BLENDINFO BlendInfo ;

	// �}�X�N���g�p���Ă���ꍇ�Ƃ��Ȃ��ꍇ�ŏ����𕪊�
	if( MASKD.MaskValidFlag == FALSE || MASKD3D9.MaskScreenSurface == NULL)
	{
		return 0 ;
	}

	// �}�X�N�p�O���t�B�b�N�n���h�����L���ȏꍇ�̓}�X�N�p�O���t�B�b�N�n���h���̏����擾����
	if( MASKD.MaskScreenGraphHandle != 0 )
	{
		MaskScreenImage = Graphics_Image_GetData( MASKD.MaskScreenGraphHandle ) ;

		// ���ɖ����ɂȂ��Ă�����ݒ�� 0 �ɂ���
		if( MaskScreenImage == NULL )
		{
			MASKD.MaskScreenGraphHandle = 0 ;
		}
	}

#if 1

	// �`����ύX
	Graphics_D3D9_DeviceState_SetRenderTarget( MASKD3D9.DestTargetSurface ) ;
	Graphics_D3D9_BeginScene() ;

	// �y�o�b�t�@���g�p���Ȃ��ݒ�ɂ���
	UseZBufferFlag = GD3D9.Device.State.ZEnable ;
	Graphics_D3D9_DeviceState_SetZEnable( FALSE ) ;

	// �V�F�[�_�[���Z�b�g����Ă�����͂���
	Graphics_D3D9_DeviceState_ResetVertexShader() ;
	Graphics_D3D9_DeviceState_ResetPixelShader() ;
	Graphics_D3D9_DeviceState_SetVertexBuffer( NULL ) ;
	Graphics_D3D9_DeviceState_SetIndexBuffer( NULL ) ;

	// �r���[�|�[�g�����ɖ߂�
	GD3D9.Device.DrawSetting.CancelSettingEqualCheck = TRUE ;
	Graphics_D3D9_DeviceState_SetViewportEasy( GSYS.DrawSetting.DrawArea.left, GSYS.DrawSetting.DrawArea.top, GSYS.DrawSetting.DrawArea.right, GSYS.DrawSetting.DrawArea.bottom ) ;
	GD3D9.Device.DrawSetting.CancelSettingEqualCheck = FALSE ;

	// �}�X�N�A���t�@�`�����l���ƃ}�X�N�J���[�o�b�t�@��Z�����ĕ`��

	// �}�X�N�X�N���[���p�摜���g�p���Ă���ꍇ�̓A���t�@�u�����h�A�g�p���Ă��Ȃ��ꍇ�̓A���t�@�e�X�g���s��
	_MEMSET( &BlendInfo, 0, sizeof( BlendInfo ) ) ;
	if( MaskScreenImage != NULL )
	{
		BlendInfo.AlphaTestEnable          = FALSE ;
		BlendInfo.AlphaRef                 = 0 ;
	//	BlendInfo.AlphaFunc                = MASKD.MaskReverseEffectFlag ? D_D3DCMP_GREATER : D_D3DCMP_LESS ;
		BlendInfo.AlphaFunc                = MASKD.MaskReverseEffectFlag ? D_D3DCMP_NOTEQUAL : D_D3DCMP_EQUAL ;
		BlendInfo.AlphaBlendEnable         = TRUE ;
		BlendInfo.SeparateAlphaBlendEnable = FALSE ;
		BlendInfo.FactorColor              = 0xffffffff ;
		BlendInfo.SrcBlend                 = D_D3DBLEND_SRCALPHA ;
		BlendInfo.DestBlend                = D_D3DBLEND_INVSRCALPHA ;
		BlendInfo.BlendOp                  = D_D3DBLENDOP_ADD ;
		BlendInfo.SrcBlendAlpha            = -1 ;
		BlendInfo.DestBlendAlpha           = -1 ;
		BlendInfo.BlendOpAlpha             = -1 ;
		BlendInfo.UseTextureStageNum       = 3 ;

		BlendInfo.TextureStageInfo[ 0 ].ResultTempARG     = -1 ;
		BlendInfo.TextureStageInfo[ 0 ].Texture           = ( void * )MASKD3D9.MaskScreenTexture ;
		BlendInfo.TextureStageInfo[ 0 ].TextureCoordIndex = 0 ;
		BlendInfo.TextureStageInfo[ 0 ].AlphaARG1         = D_D3DTA_CURRENT ;
		BlendInfo.TextureStageInfo[ 0 ].AlphaARG2         = -1 ;
		BlendInfo.TextureStageInfo[ 0 ].AlphaOP           = D_D3DTOP_SELECTARG1 ;
		BlendInfo.TextureStageInfo[ 0 ].ColorARG1         = D_D3DTA_TEXTURE ;
		BlendInfo.TextureStageInfo[ 0 ].ColorARG2         = -1 ;
		BlendInfo.TextureStageInfo[ 0 ].ColorOP           = D_D3DTOP_SELECTARG1 ;

		BlendInfo.TextureStageInfo[ 1 ].ResultTempARG     = -1 ;
		BlendInfo.TextureStageInfo[ 1 ].Texture           = ( void * )( MaskScreenImage != NULL ? MaskScreenImage->Hard.Draw[ 0 ].Tex->PF->D3D9.Texture : MASKD3D9.MaskImageTexture ) ;
		BlendInfo.TextureStageInfo[ 1 ].TextureCoordIndex = 1 ;
		if( MASKD.MaskReverseEffectFlag )
		{
			BlendInfo.TextureStageInfo[ 1 ].AlphaARG1         = D_D3DTA_TEXTURE ;
			BlendInfo.TextureStageInfo[ 1 ].AlphaARG2         = -1 ;
			BlendInfo.TextureStageInfo[ 1 ].AlphaOP           = D_D3DTOP_SELECTARG1 ;
		}
		else
		{
			BlendInfo.TextureStageInfo[ 1 ].AlphaARG1         = D_D3DTA_TFACTOR ;
			BlendInfo.TextureStageInfo[ 1 ].AlphaARG2         = D_D3DTA_TEXTURE ;
			BlendInfo.TextureStageInfo[ 1 ].AlphaOP           = D_D3DTOP_SUBTRACT ;
		}
		BlendInfo.TextureStageInfo[ 1 ].ColorARG1         = D_D3DTA_CURRENT ;
		BlendInfo.TextureStageInfo[ 1 ].ColorARG2         = -1 ;
		BlendInfo.TextureStageInfo[ 1 ].ColorOP           = D_D3DTOP_SELECTARG1 ;

		BlendInfo.TextureStageInfo[ 2 ].ResultTempARG     = -1 ;
		BlendInfo.TextureStageInfo[ 2 ].Texture           = NULL ;
		BlendInfo.TextureStageInfo[ 2 ].TextureCoordIndex = 0 ;
		BlendInfo.TextureStageInfo[ 2 ].ColorARG1         = D_D3DTA_TEXTURE ;
		BlendInfo.TextureStageInfo[ 2 ].ColorARG2         = D_D3DTA_DIFFUSE ;
		BlendInfo.TextureStageInfo[ 2 ].ColorOP           = D_D3DTOP_DISABLE ;
		BlendInfo.TextureStageInfo[ 2 ].AlphaARG1         = D_D3DTA_TEXTURE ;
		BlendInfo.TextureStageInfo[ 2 ].AlphaARG2         = D_D3DTA_DIFFUSE ;
		BlendInfo.TextureStageInfo[ 2 ].AlphaOP           = D_D3DTOP_DISABLE ;

		// �u�����h���̕ύX
		Graphics_D3D9_DeviceState_SetUserBlendInfo( &BlendInfo, FALSE, FALSE, FALSE ) ;

		// DX_MASKGRAPH_CH_A �ȊO�̏ꍇ�͂����Ńs�N�Z���V�F�[�_�[��ݒ肷��
		if( GSYS.HardInfo.UseShader && MASKD.MaskScreenGraphHandleUseChannel != DX_MASKGRAPH_CH_A )
		{
			Graphics_D3D9_DeviceState_SetPixelShader(
				MASKD.MaskReverseEffectFlag ? 
					GD3D9.Device.Shader._2D.MaskEffect_UseGraphHandle_ReverseEffect_PixelShader[ MASKD.MaskScreenGraphHandleUseChannel ] :
					GD3D9.Device.Shader._2D.MaskEffect_UseGraphHandle_PixelShader[ MASKD.MaskScreenGraphHandleUseChannel ]
			) ;
		}
	}
	else
	{
		BlendInfo.AlphaTestEnable          = TRUE ;
		BlendInfo.AlphaRef                 = 0 ;
	//	BlendInfo.AlphaFunc                = MASKD.MaskReverseEffectFlag ? D_D3DCMP_GREATER : D_D3DCMP_LESS ;
		BlendInfo.AlphaFunc                = MASKD.MaskReverseEffectFlag ? D_D3DCMP_NOTEQUAL : D_D3DCMP_EQUAL ;
		BlendInfo.AlphaBlendEnable         = FALSE ;
		BlendInfo.SeparateAlphaBlendEnable = FALSE ;
		BlendInfo.FactorColor              = 0xffffffff ;
		BlendInfo.SrcBlend                 = -1 ;
		BlendInfo.DestBlend                = -1 ;
		BlendInfo.BlendOp                  = -1 ;
		BlendInfo.SrcBlendAlpha            = -1 ;
		BlendInfo.DestBlendAlpha           = -1 ;
		BlendInfo.BlendOpAlpha             = -1 ;
		BlendInfo.UseTextureStageNum       = 3 ;

		BlendInfo.TextureStageInfo[ 0 ].ResultTempARG     = -1 ;
		BlendInfo.TextureStageInfo[ 0 ].Texture           = ( void * )MASKD3D9.MaskScreenTexture ;
		BlendInfo.TextureStageInfo[ 0 ].TextureCoordIndex = 0 ;
		BlendInfo.TextureStageInfo[ 0 ].AlphaARG1         = D_D3DTA_CURRENT ;
		BlendInfo.TextureStageInfo[ 0 ].AlphaARG2         = -1 ;
		BlendInfo.TextureStageInfo[ 0 ].AlphaOP           = D_D3DTOP_SELECTARG1 ;
		BlendInfo.TextureStageInfo[ 0 ].ColorARG1         = D_D3DTA_TEXTURE ;
		BlendInfo.TextureStageInfo[ 0 ].ColorARG2         = -1 ;
		BlendInfo.TextureStageInfo[ 0 ].ColorOP           = D_D3DTOP_SELECTARG1 ;

		BlendInfo.TextureStageInfo[ 1 ].ResultTempARG     = -1 ;
		BlendInfo.TextureStageInfo[ 1 ].Texture           = ( void * )( MaskScreenImage != NULL ? MaskScreenImage->Hard.Draw[ 0 ].Tex->PF->D3D9.Texture : MASKD3D9.MaskImageTexture ) ;
		BlendInfo.TextureStageInfo[ 1 ].TextureCoordIndex = 1 ;
		BlendInfo.TextureStageInfo[ 1 ].AlphaARG1         = D_D3DTA_TEXTURE ;
		BlendInfo.TextureStageInfo[ 1 ].AlphaARG2         = -1 ;
		BlendInfo.TextureStageInfo[ 1 ].AlphaOP           = D_D3DTOP_SELECTARG1 ;
		BlendInfo.TextureStageInfo[ 1 ].ColorARG1         = D_D3DTA_CURRENT ;
		BlendInfo.TextureStageInfo[ 1 ].ColorARG2         = -1 ;
		BlendInfo.TextureStageInfo[ 1 ].ColorOP           = D_D3DTOP_SELECTARG1 ;

		BlendInfo.TextureStageInfo[ 2 ].ResultTempARG     = -1 ;
		BlendInfo.TextureStageInfo[ 2 ].Texture           = NULL ;
		BlendInfo.TextureStageInfo[ 2 ].TextureCoordIndex = 0 ;
		BlendInfo.TextureStageInfo[ 2 ].ColorARG1         = D_D3DTA_TEXTURE ;
		BlendInfo.TextureStageInfo[ 2 ].ColorARG2         = D_D3DTA_DIFFUSE ;
		BlendInfo.TextureStageInfo[ 2 ].ColorOP           = D_D3DTOP_DISABLE ;
		BlendInfo.TextureStageInfo[ 2 ].AlphaARG1         = D_D3DTA_TEXTURE ;
		BlendInfo.TextureStageInfo[ 2 ].AlphaARG2         = D_D3DTA_DIFFUSE ;
		BlendInfo.TextureStageInfo[ 2 ].AlphaOP           = D_D3DTOP_DISABLE ;

		// �u�����h���̕ύX
		Graphics_D3D9_DeviceState_SetUserBlendInfo( &BlendInfo, FALSE, FALSE, FALSE ) ;
	}

	// �V�F�[�_�[���g�p����ꍇ�͂����Ńs�N�Z���V�F�[�_�[��ݒ肷��
//	if( GSYS.HardInfo.UseShader && GD3D9.NormalDraw_NotUsePixelShader == FALSE )
//	{
//		Graphics_D3D9_DeviceState_SetPixelShader( GD3D9.Device.Shader._2D.MaskEffectPixelShader ) ;
//	}

	// �`�揀��
	Graphics_D3D9_BeginScene() ;

	// ���_�̏���
	Vert[ 2 ].pos.x = Vert[ 0 ].pos.x = ( float )Rect->left   - 0.5f ;
	Vert[ 1 ].pos.y = Vert[ 0 ].pos.y = ( float )Rect->top    - 0.5f ;

	Vert[ 3 ].pos.x = Vert[ 1 ].pos.x = ( float )Rect->right  - 0.5f ;
	Vert[ 3 ].pos.y = Vert[ 2 ].pos.y = ( float )Rect->bottom - 0.5f ;

	Vert[ 2 ].u1 = Vert[ 0 ].u1 = ( float )Rect->left   / ( float )MASKD3D9.MaskTextureSizeX ;
	Vert[ 1 ].v1 = Vert[ 0 ].v1 = ( float )Rect->top    / ( float )MASKD3D9.MaskTextureSizeY ;
	Vert[ 3 ].u1 = Vert[ 1 ].u1 = ( float )Rect->right  / ( float )MASKD3D9.MaskTextureSizeX ;
	Vert[ 3 ].v1 = Vert[ 2 ].v1 = ( float )Rect->bottom / ( float )MASKD3D9.MaskTextureSizeY ;

	if( MaskScreenImage != NULL )
	{
		Vert[ 2 ].u2 = Vert[ 0 ].u2 = ( float )Rect->left   / ( float )MaskScreenImage->Hard.Draw[ 0 ].Tex->TexWidth ;
		Vert[ 1 ].v2 = Vert[ 0 ].v2 = ( float )Rect->top    / ( float )MaskScreenImage->Hard.Draw[ 0 ].Tex->TexHeight ;
		Vert[ 3 ].u2 = Vert[ 1 ].u2 = ( float )Rect->right  / ( float )MaskScreenImage->Hard.Draw[ 0 ].Tex->TexWidth ;
		Vert[ 3 ].v2 = Vert[ 2 ].v2 = ( float )Rect->bottom / ( float )MaskScreenImage->Hard.Draw[ 0 ].Tex->TexHeight ;
	}
	else
	{
		Vert[ 2 ].u2 = Vert[ 0 ].u2 = Vert[ 0 ].u1 ;
		Vert[ 1 ].v2 = Vert[ 0 ].v2 = Vert[ 0 ].v1 ;
		Vert[ 3 ].u2 = Vert[ 1 ].u2 = Vert[ 1 ].u1 ;
		Vert[ 3 ].v2 = Vert[ 2 ].v2 = Vert[ 2 ].v1 ;
	}

	Vert[ 0 ].color =
	Vert[ 1 ].color =
	Vert[ 2 ].color =
	Vert[ 3 ].color = 0xffffffff ;

	Vert[ 0 ].specular =
	Vert[ 1 ].specular =
	Vert[ 2 ].specular =
	Vert[ 3 ].specular = 0 ;

	Vert[ 0 ].pos.z = 
	Vert[ 1 ].pos.z = 
	Vert[ 2 ].pos.z = 
	Vert[ 3 ].pos.z = 0.0f ;

	Vert[ 0 ].rhw = 
	Vert[ 1 ].rhw = 
	Vert[ 2 ].rhw = 
	Vert[ 3 ].rhw = 1.0f ;

	// �`��
//	Graphics_D3D9_DeviceState_SetFVF( VERTEXFVF_2D ) ;
	Graphics_D3D9_DeviceState_SetFVF( VERTEXFVF_BLENDTEX_2D ) ;
	Direct3DDevice9_DrawPrimitiveUP( D_D3DPT_TRIANGLESTRIP, 2, Vert, sizeof( VERTEX_BLENDTEX_2D /* VERTEX_2D */ ) ) ;
	Graphics_D3D9_EndScene() ;
	Graphics_D3D9_BeginScene() ;

	// �V�F�[�_�[���g�p����ꍇ�͂����Ńs�N�Z���V�F�[�_�[�𖳌��ɂ���
//	if( GSYS.HardInfo.UseShader && GD3D9.NormalDraw_NotUsePixelShader == FALSE )
//	{
//		Graphics_D3D9_DeviceState_ResetPixelShader() ;
//	}

	// �y�o�b�t�@�̐ݒ�����ɖ߂�
	Graphics_D3D9_DeviceState_SetZEnable( UseZBufferFlag ) ;

#else
	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO
	Graphics_D3D9_EndScene() ;

	Direct3DDevice9_StretchRect(
		UseBackupSurface,  &Rect,
		DestTargetSurface, &Rect, D_D3DTEXF_NONE ) ;
#endif

	// �`�����}�X�N�T�[�t�F�X�ɂ���
	Graphics_D3D9_DeviceState_SetRenderTarget( MASKD3D9.MaskScreenSurface ) ;

	// �r���[�|�[�g�����ɖ߂�
	GD3D9.Device.DrawSetting.CancelSettingEqualCheck = TRUE ;
	Graphics_D3D9_DeviceState_SetViewportEasy( GSYS.DrawSetting.DrawArea.left, GSYS.DrawSetting.DrawArea.top, GSYS.DrawSetting.DrawArea.right, GSYS.DrawSetting.DrawArea.bottom ) ;
	GD3D9.Device.DrawSetting.CancelSettingEqualCheck = FALSE ;

	// ����I��
	return 0 ;
}

// �}�X�N�X�N���[�����w��̐F�œh��Ԃ�
extern	int			Mask_D3D9_FillMaskScreen_PF( int Flag )
{
	D_D3DLOCKED_RECT LockRect ;
	int i, Width = 0 ;
	BYTE *Dest ;

	if( MASKD3D9.MaskImageTexture->LockRect( 0, &LockRect, NULL, 0 ) == D_D3D_OK )
	{
		Flag = Flag ? 0xff : 0 ;

		switch( GD3D9.Device.Caps.MaskAlphaFormat )
		{
		case D_D3DFMT_A8R8G8B8 : Width = GSYS.Screen.MainScreenSizeX * 4 ; break ;
		case D_D3DFMT_A4R4G4B4 : Width = GSYS.Screen.MainScreenSizeX * 2 ; break ;
		case D_D3DFMT_A1R5G5B5 : Width = GSYS.Screen.MainScreenSizeX * 2 ; break ;
		}

		Dest = ( BYTE * )LockRect.pBits ;
		for( i = 0 ; i < GSYS.Screen.MainScreenSizeY ; i ++, Dest += LockRect.Pitch )
		{
			_MEMSET( Dest, ( unsigned char )Flag, ( size_t )Width ) ;
		}

		MASKD3D9.MaskImageTexture->UnlockRect( 0 ) ;
	}

//	if( MASKD3D9.MaskMemImageSurface->LockRect( &LockRect, NULL, 0 ) == D_D3D_OK )
//	{
//		Flag = Flag ? 0xff : 0 ;
//
//		switch( GD3D9.Device.Caps.MaskAlphaFormat )
//		{
//		case D_D3DFMT_A8R8G8B8 : Width = GSYS.Screen.MainScreenSizeX * 4 ; break ;
//		case D_D3DFMT_A4R4G4B4 : Width = GSYS.Screen.MainScreenSizeX * 2 ; break ;
//		case D_D3DFMT_A1R5G5B5 : Width = GSYS.Screen.MainScreenSizeX * 2 ; break ;
//		}
//
//		Dest = ( BYTE * )LockRect.pBits ;
//		for( i = 0 ; i < GSYS.Screen.MainScreenSizeY ; i ++, Dest += LockRect.Pitch )
//		{
//			_MEMSET( Dest, ( unsigned char )Flag, Width ) ;
//		}
//
//		MASKD3D9.MaskMemImageSurface->UnlockRect() ;
//
//		Direct3DDevice9_UpdateSurface(
//			MASKD3D9.MaskMemImageSurface,
//			NULL,
//			MASKD3D9.MaskImageSurface,
//			NULL
//		);
//	}

	// ����I��
	return 0 ;
}

// �w��̈�̃}�X�N�C���[�W�e�N�X�`�����X�V����
extern	int			Mask_D3D9_UpdateMaskImageTexture_PF( RECT *Rect )
{
	D_D3DLOCKED_RECT LockRect ;
	int Width, Height ;
	BYTE *Dest, *Src ;
//	POINT DestPoint ;
	DWORD DestAdd, SrcAdd ;
#ifdef DX_NON_INLINE_ASM
	int i ;
#endif

	RectClipping_Inline( Rect, &GSYS.DrawSetting.DrawArea ) ;
	if( Rect->left == Rect->right || Rect->top == Rect->bottom ) return 0 ;

	Width = Rect->right - Rect->left ;
	Height = Rect->bottom - Rect->top ;

	// �}�X�N�C���[�W�e�N�X�`�������b�N����
	if( MASKD3D9.MaskImageTexture->LockRect( 0, &LockRect, Rect, 0 ) != D_D3D_OK )
		return -1 ;
//	if( MASKD3D9.MaskMemImageSurface->LockRect( &LockRect, Rect, 0 ) != D_D3D_OK )
//		return -1 ;

	// �]�����̏���
	Src    = MASKD.MaskBuffer + MASKD.MaskBufferPitch * Rect->top + Rect->left ;
	SrcAdd = ( DWORD )( MASKD.MaskBufferPitch - Width ) ;

	// �w�蕔���C���[�W�̓]��
	Dest = ( BYTE * )LockRect.pBits ;
	switch( GD3D9.Device.Caps.MaskAlphaFormat )
	{
	case D_D3DFMT_A8R8G8B8 :
		DestAdd = ( DWORD )( LockRect.Pitch - Width * 4 ) ;
#ifdef DX_NON_INLINE_ASM
		do
		{
			i = Width ;
			do
			{
				Dest[ 3 ] = *Src ;
				Dest += 4 ;
				Src ++ ;
			}while( -- i != 0 ) ;
			Dest += DestAdd ;
			Src += SrcAdd ;
		}while( -- Height != 0 ) ;
#else
		__asm
		{
			MOV		EDI, Dest
			MOV		ESI, Src
			MOV		ECX, Height
LOOP_A8R8G8B8_1:
			MOV		EDX, Width
LOOP_A8R8G8B8_2:
			MOV		AL, [ ESI ]
			MOV		[ EDI + 3 ], AL
			INC		ESI
			ADD		EDI, 4
			DEC		EDX
			JNZ		LOOP_A8R8G8B8_2

			ADD		EDI, DestAdd
			ADD		ESI, SrcAdd
			DEC		Height
			JNZ		LOOP_A8R8G8B8_1
		}
#endif
		break ;

	case D_D3DFMT_A1R5G5B5 :
	case D_D3DFMT_A4R4G4B4 :
		DestAdd = ( DWORD )( LockRect.Pitch - Width * 2 ) ;
#ifdef DX_NON_INLINE_ASM
		do
		{
			i = Width ;
			do
			{
				Dest[ 1 ] = *Src ;
				Dest += 2 ;
				Src ++ ;
			}while( -- i != 0 ) ;
			Dest += DestAdd ;
			Src += SrcAdd ;
		}while( -- Height != 0 ) ;
#else
		__asm
		{
			MOV		EDI, Dest
			MOV		ESI, Src
			MOV		ECX, Height
LOOP_A1R5G5B5_1:
			MOV		EDX, Width
LOOP_A1R5G5B5_2:
			MOV		AL, [ ESI ]
			MOV		[ EDI + 1 ], AL
			INC		ESI
			ADD		EDI, 2
			DEC		EDX
			JNZ		LOOP_A1R5G5B5_2

			ADD		EDI, DestAdd
			ADD		ESI, SrcAdd
			DEC		Height
			JNZ		LOOP_A1R5G5B5_1
		}
#endif
		break ;
	}

	// ���b�N����������
	MASKD3D9.MaskImageTexture->UnlockRect( 0 ) ;
//	MASKD3D9.MaskMemImageSurface->UnlockRect() ;
//
//	DestPoint.x = Rect->left ;
//	DestPoint.y = Rect->top ;
//	Direct3DDevice9_UpdateSurface(
//		MASKD3D9.MaskMemImageSurface,
//		Rect,
//		MASKD3D9.MaskImageSurface,
//		&DestPoint
//	);

	// �I��
	return 0 ;
}




#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

#endif // DX_NON_MASK

#endif // DX_NON_DIRECT3D9

#endif // DX_NON_GRAPHICS

