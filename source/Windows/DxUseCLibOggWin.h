// -------------------------------------------------------------------------------
// 
// 		�c�w���C�u����		WindowsOS�pOgg�֌W�w�b�_�t�@�C��
// 
// 				Ver 3.16d
// 
// -------------------------------------------------------------------------------

#ifndef __DXUSECLIBOGGWIN_H__
#define __DXUSECLIBOGGWIN_H__

// �C���N���[�h ------------------------------------------------------------------
#include "../DxCompileConfig.h"
#include "../DxLib.h"
#include "DxUseCLibOggD3D9.h"
#include "DxUseCLibOggD3D11.h"

#if !defined( DX_NON_OGGVORBIS ) || !defined( DX_NON_OGGTHEORA )

//namespace DxLib
//{

// �}�N����` --------------------------------------------------------------------

// �\���̒�` --------------------------------------------------------------------

#ifndef DX_NON_OGGTHEORA

// Ogg Theora �f�R�[�h�����p���ˑ��f�[�^�\����
struct DECODE_THEORA_PF
{
	union
	{
		int									Dummy ;
#ifndef DX_NON_DIRECT3D11
		DECODE_THEORA_DIRECT3D11			D3D11 ;					// Direct3D11�p Ogg Theora �f�R�[�h�����p�f�[�^�\����
#endif // DX_NON_DIRECT3D11
#ifndef DX_NON_DIRECT3D9
		DECODE_THEORA_DIRECT3D9				D3D9 ;					// Direct3D9�p Ogg Theora �f�R�[�h�����p�f�[�^�\����
#endif // DX_NON_DIRECT3D9
	} ;
} ;

#endif // DX_NON_OGGTHEORA

// �������ϐ��錾 --------------------------------------------------------------

// �֐��v���g�^�C�v�錾-----------------------------------------------------------

//}

#endif // #if !defined( DX_NON_OGGVORBIS ) || !defined( DX_NON_OGGTHEORA )

#endif // __DXUSECLIBOGGWIN_H__
