//-----------------------------------------------------------------------------
// 
// 		�c�w���C�u����		Ogg�֌W�v���O����( Direct3D11 )
// 
//  	Ver 3.16d
// 
//-----------------------------------------------------------------------------

// �c�w���C�u�����쐬���p��`
#define __DX_MAKE

#include "../DxCompileConfig.h"

#ifndef DX_NON_DIRECT3D11

#if !defined( DX_NON_OGGVORBIS ) || !defined( DX_NON_OGGTHEORA )

// �C���N���[�h----------------------------------------------------------------
#include "../DxLib.h"
#include "../DxStatic.h"
#include "../DxUseCLib.h"
#include "../DxGraphics.h"
#include "../DxLog.h"
#include "DxGraphicsAPIWin.h"
#include "DxUseCLibOggD3D11.h"
#include "DxUseCLibOggWin.h"

//namespace DxLib
//{

// �}�N����`------------------------------------------------------------------

// �^��`----------------------------------------------------------------------

// �f�[�^�錾------------------------------------------------------------------


// �֐��v���g�^�C�v�錾 -------------------------------------------------------

// �v���O����------------------------------------------------------------------

// ���ˑ��֐�

#ifndef DX_NON_OGGTHEORA

// �n�[�h�E�G�A�@�\���L���ȏꍇ�� YUV �t�H�[�}�b�g�̈ꎞ�ۑ��p�e�N�X�`�����쐬����
extern	int	TheoraDecode_D3D11_CreateSurface_PF( DECODE_THEORA *DT )
{
	// ���̂Ƃ���YUV�t�H�[�}�b�g�ɂ͑Ή����Ȃ�
	DT->YUVImageFourCC = 0 ;
	DT->ValidYUVSurface = FALSE ;

	// ����I��
	return 0 ;
}

extern	void	TheoraDecode_D3D11_ReleaseSurface_PF( DECODE_THEORA *DT )
{
}

// �J�����g�t���[����RGB�摜���쐬����( �߂�l  1:�쐬���ꂽ  0:����Ȃ����� )
extern	int		TheoraDecode_D3D11_SetupImage_PF( DECODE_THEORA *DT, volatile THEORA_STOCKFRAME *Stock, int ASyncThread )
{
	// ����I��
	return 0 ;
}

// �ꎞ�o�b�t�@�� YUV �t�H�[�}�b�g�̃e�N�X�`���𓾂�
extern	const void *TheoraDecode_D3D11_GetYUVImage_PF( DECODE_THEORA *DT )
{
	return NULL ;
}

#endif // DX_NON_OGGTHEORA



//}

#endif // #if !defined( DX_NON_OGGVORBIS ) || !defined( DX_NON_OGGTHEORA )

#endif // DX_NON_DIRECT3D11

