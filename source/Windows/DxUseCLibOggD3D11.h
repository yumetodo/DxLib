// -------------------------------------------------------------------------------
// 
// 		�c�w���C�u����		Ogg�֌W�w�b�_�t�@�C��( Direct3D11 )
// 
// 				Ver 3.16d
// 
// -------------------------------------------------------------------------------

#ifndef __DXUSECLIBOGGD3D11_H__
#define __DXUSECLIBOGGD3D11_H__

#include "../DxCompileConfig.h"

#ifndef DX_NON_DIRECT3D11

// �C���N���[�h ------------------------------------------------------------------
#include "../DxLib.h"
#include "../DxUseCLibOgg.h"

#if !defined( DX_NON_OGGVORBIS ) || !defined( DX_NON_OGGTHEORA )

//namespace DxLib
//{

// �}�N����` --------------------------------------------------------------------

// �\���̒�` --------------------------------------------------------------------

#ifndef DX_NON_OGGTHEORA

// Direct3D11�p Ogg Theora �f�R�[�h�����p���ˑ��f�[�^�\����
struct DECODE_THEORA_DIRECT3D11
{
	int Dummy ;
} ;

#endif // DX_NON_OGGTHEORA

// �������ϐ��錾 --------------------------------------------------------------

// �֐��v���g�^�C�v�錾-----------------------------------------------------------

#ifndef DX_NON_OGGTHEORA

// ���ˑ��֐�
extern	int		TheoraDecode_D3D11_CreateSurface_PF(	 DECODE_THEORA *DT ) ;																// �n�[�h�E�G�A�@�\���L���ȏꍇ�� YUV �t�H�[�}�b�g�̈ꎞ�ۑ��p�e�N�X�`�����쐬����
extern	void	TheoraDecode_D3D11_ReleaseSurface_PF( DECODE_THEORA *DT ) ;
extern	int		TheoraDecode_D3D11_SetupImage_PF( DECODE_THEORA *DT, volatile THEORA_STOCKFRAME *Stock, int ASyncThread ) ;				// �J�����g�t���[����RGB�摜���쐬����( �߂�l  1:�쐬���ꂽ  0:����Ȃ����� )
extern	const void *TheoraDecode_D3D11_GetYUVImage_PF( DECODE_THEORA *DT ) ;												// �ꎞ�o�b�t�@�� YUV �t�H�[�}�b�g�̃e�N�X�`���𓾂�

#endif // DX_NON_OGGTHEORA

//}

#endif // #if !defined( DX_NON_OGGVORBIS ) || !defined( DX_NON_OGGTHEORA )

#endif // DX_NON_DIRECT3D11

#endif // __DXUSECLIBOGGD3D11_H__
