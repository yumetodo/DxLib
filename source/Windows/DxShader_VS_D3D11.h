// -------------------------------------------------------------------------------
// 
// 		�c�w���C�u����		�o�[�e�b�N�X�V�F�[�_�[�֘A��`
// 
// 				Ver 3.16d
// 
// -------------------------------------------------------------------------------

#ifndef __DXSHADER_VS_D3D11_H__
#define __DXSHADER_VS_D3D11_H__

// �C���N���[�h ---------------------------------

#include "DxShader_Common_D3D11.h"

// �}�N����` -----------------------------------

#define DX_D3D11_VS_CONST_TEXTURE_MATRIX_NUM			3			// �e�N�X�`�����W�ϊ��s��̓]�u�s��̐�
#define DX_D3D11_VS_CONST_WORLD_MAT_NUM					54			// �g���C�A���O�����X�g��œ����Ɏg�p���郍�[�J�������[���h�s��̍ő吔

// �f�[�^�^��` ---------------------------------

// ��{�p�����[�^
struct DX_D3D11_VS_CONST_BUFFER_BASE
{
	DX_D3D11_SHADER_FLOAT4		AntiViewportMatrix[ 4 ] ;										// �A���`�r���[�|�[�g�s��
	DX_D3D11_SHADER_FLOAT4		ProjectionMatrix[ 4 ] ;											// �r���[�@���@�v���W�F�N�V�����s��
	DX_D3D11_SHADER_FLOAT4		ViewMatrix[ 3 ] ;												// ���[���h�@���@�r���[�s��
	DX_D3D11_SHADER_FLOAT4		LocalWorldMatrix[ 3 ] ;											// ���[�J���@���@���[���h�s��

	DX_D3D11_SHADER_FLOAT4		ToonOutLineSize ;												// �g�D�[���̗֊s���̑傫��
	DX_D3D11_SHADER_FLOAT		DiffuseSource ;													// �f�B�t���[�Y�J���[( 0.0f:�}�e���A��  1.0f:���_ )
	DX_D3D11_SHADER_FLOAT		SpecularSource ;												// �X�y�L�����J���[(   0.0f:�}�e���A��  1.0f:���_ )
	DX_D3D11_SHADER_FLOAT		MulSpecularColor ;												// �X�y�L�����J���[�l�ɏ�Z����l( �X�y�L�������������Ŏg�p )
	DX_D3D11_SHADER_FLOAT		Padding ;
} ;

// ���̑��̍s��
struct DX_D3D11_VS_CONST_BUFFER_OTHERMATRIX
{
	DX_D3D11_SHADER_FLOAT4		ShadowMapLightViewProjectionMatrix[ 3 ][ 4 ] ;					// �V���h�E�}�b�v�p�̃��C�g�r���[�s��ƃ��C�g�ˉe�s�����Z��������
	DX_D3D11_SHADER_FLOAT4		TextureMatrix[ DX_D3D11_VS_CONST_TEXTURE_MATRIX_NUM ][ 2 ] ;	// �e�N�X�`�����W����p�s��
} ;

// �X�L�j���O���b�V���p�́@���[�J���@���@���[���h�s��
struct DX_D3D11_VS_CONST_BUFFER_LOCALWORLDMATRIX
{
	DX_D3D11_SHADER_FLOAT4		Matrix[ DX_D3D11_VS_CONST_WORLD_MAT_NUM * 3 ] ;					// ���[�J���@���@���[���h�s��
} ;

#endif // __DXSHADER_VS_D3D11_H__
