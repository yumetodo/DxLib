// -------------------------------------------------------------------------------
// 
// 		�c�w���C�u����		WindowsOS�p�`�揈���v���O�����w�b�_�t�@�C��
// 
// 				Ver 3.16d
// 
// -------------------------------------------------------------------------------

#ifndef __DXGRAPHICSWIN_H__
#define __DXGRAPHICSWIN_H__

#include "../DxCompileConfig.h"

#ifndef DX_NON_GRAPHICS

// �C���N���[�h ------------------------------------------------------------------
#include "DxGraphicsD3D9.h"
#include "DxGraphicsD3D11.h"

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// �}�N����` --------------------------------------------------------------------

#define GRAWIN		GraphicsManage_Win

#define GRAPHICS_API_NONE						(0)
#define GRAPHICS_API_DIRECT3D9_WIN32			(1)
#define GRAPHICS_API_DIRECT3D11_WIN32			(2)

// �\���̒�` --------------------------------------------------------------------

// Windows�p �n�[�h�E�G�A�����_�����O�p�I���W�i���摜�e�N�X�`�����̍\����
struct IMAGEDATA_ORIG_HARD_TEX_PF
{
	union
	{
		int									Dummy ;
#ifndef DX_NON_DIRECT3D11
		IMAGEDATA_ORIG_HARD_TEX_DIRECT3D11	D3D11 ;							// Direct3D11�p�I���W�i���摜�e�N�X�`�����
#endif // DX_NON_DIRECT3D11
#ifndef DX_NON_DIRECT3D9
		IMAGEDATA_ORIG_HARD_TEX_DIRECT3D9	D3D9 ;							// Direct3D9�p�I���W�i���摜�e�N�X�`�����
#endif // DX_NON_DIRECT3D9
	} ;
} ;

// Windows�p �V���h�E�}�b�v���ˑ����̍\����
struct SHADOWMAPDATA_PF
{
	union
	{
		int									Dummy ;
#ifndef DX_NON_DIRECT3D11
		SHADOWMAPDATA_DIRECT3D11			D3D11 ;								// Direct3D11�p�V���h�E�}�b�v���
#endif // DX_NON_DIRECT3D11
#ifndef DX_NON_DIRECT3D9
		SHADOWMAPDATA_DIRECT3D9				D3D9 ;								// Direct3D9�p�V���h�E�}�b�v���
#endif // DX_NON_DIRECT3D9
	} ;
} ;

// Windows�p �V�F�[�_�[�n���h�����ˑ����̍\����
struct SHADERHANDLEDATA_PF
{
	union
	{
		int									Dummy ;
#ifndef DX_NON_DIRECT3D11
		SHADERHANDLEDATA_DIRECT3D11		D3D11 ;								// Direct3D11�p�V�F�[�_�[�n���h�����
#endif // DX_NON_DIRECT3D11
#ifndef DX_NON_DIRECT3D9
		SHADERHANDLEDATA_DIRECT3D9		D3D9 ;								// Direct3D9�p�V�F�[�_�[�n���h�����
#endif // DX_NON_DIRECT3D9
	} ;
} ;

// Windows�p �V�F�[�_�[�p�萔�o�b�t�@�n���h�����ˑ����̍\����
struct SHADERCONSTANTBUFFERHANDLEDATA_PF
{
	union
	{
		int									Dummy ;
#ifndef DX_NON_DIRECT3D11
		SHADERCONSTANTBUFFERHANDLEDATA_DIRECT3D11		D3D11 ;				// Direct3D11�p�V�F�[�_�[�p�萔�o�b�t�@�n���h�����
#endif // DX_NON_DIRECT3D11
#ifndef DX_NON_DIRECT3D9
		SHADERCONSTANTBUFFERHANDLEDATA_DIRECT3D9		D3D9 ;				// Direct3D9�p�V�F�[�_�[�p�萔�o�b�t�@�n���h�����
#endif // DX_NON_DIRECT3D9
	} ;
} ;

// Windows�p ���_�o�b�t�@�n���h���Ŏg�p������ˑ����̍\����
struct VERTEXBUFFERHANDLEDATA_PF
{
	union
	{
		int									Dummy ;
#ifndef DX_NON_DIRECT3D11
		VERTEXBUFFERHANDLEDATA_DIRECT3D11	D3D11 ;							// Direct3D11�p���_�o�b�t�@�n���h�����
#endif // DX_NON_DIRECT3D11
#ifndef DX_NON_DIRECT3D9
		VERTEXBUFFERHANDLEDATA_DIRECT3D9	D3D9 ;							// Direct3D9�p���_�o�b�t�@�n���h�����
#endif // DX_NON_DIRECT3D9
	} ;
} ;

// Windows�p �C���f�b�N�X�o�b�t�@�n���h���Ŏg�p������ˑ����̍\����
struct INDEXBUFFERHANDLEDATA_PF
{
	union
	{
		int									Dummy ;
#ifndef DX_NON_DIRECT3D11
		INDEXBUFFERHANDLEDATA_DIRECT3D11	D3D11 ;							// Direct3D11�p�C���f�b�N�X�o�b�t�@�n���h�����
#endif // DX_NON_DIRECT3D11
#ifndef DX_NON_DIRECT3D9
		INDEXBUFFERHANDLEDATA_DIRECT3D9		D3D9 ;							// Direct3D9�p�C���f�b�N�X�o�b�t�@�n���h�����
#endif // DX_NON_DIRECT3D9
	} ;
} ;

// Windows �Ɉˑ����Ă���ݒ���
struct GRAPHICS_SETTING_WIN
{
	int									DisableAeroFlag ;					// Aero �𖳌��ɂ��邩�ǂ����̃t���O( TRUE:�����ɂ���  FALSE:�����ɂ��Ȃ� )
	int									UseGraphicsAPI ;					// �g�p����O���t�B�b�N�XAPI( GRAPHICS_API_DIRECT3D9_WIN32 �Ȃ� )
	int									NotUseDirect3D11 ;					// �`�揈���� Direct3D11 ���g�p���Ȃ����ǂ���( TRUE:�g�p���Ȃ�  FALSE:�g�p���� )
	int									UseMultiThread ;					// �}���`�X���b�h�Ή��t���O���g�����ǂ���

	HWND								ScreenFlipTargetWindow ;				// ScreenFlip �̑ΏۂƂ���E�C���h�E
} ;

// �`�揈���֌W�� Windows �Ɉˑ����Ă�����̍\����
struct GRAPHICSMANAGE_WIN
{
	int									UseChangeDisplaySettings ;			// ChangeDisplaySettings ���g�p���ĉ�ʃ��[�h��ύX�������ǂ���( TRUE:ChangeDisplaySettings ���g�p����  FALSE:ChangeDisplaySettings �͎g�p���Ă��Ȃ� )

	GRAPHICS_SETTING_WIN				Setting ;							// Windows �Ɉˑ����Ă���ݒ���

//	RECT								LastCopySrcRect ;					// �Ō�Ƀt�B�b�g�]�������Ƃ��̃R�s�[����`
//	RECT								LastCopyDestRect ;					// �Ō�Ƀt�B�b�g�]�������Ƃ��̃R�s�[���`

	int									FullScreenBlackFillCounter ;		// �t���X�N���[�����[�h�ŉ�ʑS�̂����œh��Ԃ��J�E���^�[
} ;

// �������ϐ��錾 --------------------------------------------------------------

extern GRAPHICSMANAGE_WIN GraphicsManage_Win ;

// �֐��v���g�^�C�v�錾-----------------------------------------------------------

// DirectX �֘A�֐�
extern	void	Graphics_ReleaseDirectXObject( void ) ;							// DirectX �̃I�u�W�F�N�g���������

extern	void	Graphics_Win_MessageLoop_Process( void ) ;						// ���b�Z�[�W���[�v���ɍs���ׂ����������s����
extern	void	Graphics_Win_WM_ACTIVATE_ActiveProcess( void ) ;				// WM_ACTIVATE ���b�Z�[�W�̏����ŁA�E�C���h�E���[�h���ɃA�N�e�B�u�ɂȂ����ۂɌĂ΂��֐�

// �ݒ�֌W�֐�
extern	int		Graphics_Win_SetDialogBoxMode( int Flag ) ;						// �_�C�A���O�{�b�N�X���[�h��ύX����


#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

#endif // DX_NON_GRAPHICS

#endif // __DXGRAPHICSWIN_H__
