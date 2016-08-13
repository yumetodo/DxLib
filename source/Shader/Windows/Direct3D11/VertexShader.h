// -------------------------------------------------------------------------------
// 
// 		�c�w���C�u����		Direct3D11 ���_�V�F�[�_�[��`�R�[�h
// 
// 				Ver 3.16d
// 
// -------------------------------------------------------------------------------

// �C���N���[�h ------------------------------------------------------------------

#include "DataType.h"
#include "..\..\..\Main\Windows\DxShader_VS_D3D11.h"

// �\���̒�` --------------------------------------------------------------------

// �萔��` ----------------------------------------------------------------------

// ���_�V�F�[�_�[�E�s�N�Z���V�F�[�_�[���ʃp�����[�^
cbuffer cbD3D11_CONST_BUFFER_COMMON					: register( b0 )
{
	DX_D3D11_CONST_BUFFER_COMMON				g_Common ;
} ;

// ��{�p�����[�^
cbuffer cbD3D11_CONST_BUFFER_VS_BASE				: register( b1 )
{
	DX_D3D11_VS_CONST_BUFFER_BASE				g_Base ;
} ;

// ���̑��̍s��
cbuffer cbD3D11_CONST_BUFFER_VS_OTHERMATRIX			: register( b2 )
{
	DX_D3D11_VS_CONST_BUFFER_OTHERMATRIX		g_OtherMatrix ;
} ;

// �X�L�j���O���b�V���p�́@���[�J���@���@���[���h�s��
cbuffer cbD3D11_CONST_BUFFER_VS_LOCALWORLDMATRIX	: register( b3 )
{
	DX_D3D11_VS_CONST_BUFFER_LOCALWORLDMATRIX	g_LocalWorldMatrix ;
} ;
