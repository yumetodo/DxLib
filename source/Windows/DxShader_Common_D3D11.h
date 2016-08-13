// -------------------------------------------------------------------------------
// 
// 		�c�w���C�u����		Direct3D11 �V�F�[�_�[���ʒ�`�R�[�h
// 
// 				Ver 3.16d
// 
// -------------------------------------------------------------------------------

#ifndef __DXSHADER_COMMON_D3D11_H__
#define __DXSHADER_COMMON_D3D11_H__

// �}�N����` -----------------------------------

#define DX_D3D11_COMMON_CONST_LIGHT_NUM			(6)		// ���ʃp�����[�^�̃��C�g�̍ő吔

// �\���̒�` --------------------------------------------------------------------

// �}�e���A���p�����[�^
struct DX_D3D11_CONST_MATERIAL
{
	DX_D3D11_SHADER_FLOAT4		Diffuse ;				// �f�B�t���[�Y�J���[
	DX_D3D11_SHADER_FLOAT4		Specular ;				// �X�y�L�����J���[
	DX_D3D11_SHADER_FLOAT4		Ambient_Emissive ;		// �}�e���A���G�~�b�V�u�J���[ + �}�e���A���A���r�G���g�J���[ * �O���[�o���A���r�G���g�J���[

	DX_D3D11_SHADER_FLOAT		Power ;					// �X�y�L�����̋���
	DX_D3D11_SHADER_FLOAT3		Padding ;				// �p�f�B���O
} ;

// �t�H�O�p�����[�^
struct DX_D3D11_VS_CONST_FOG
{
	DX_D3D11_SHADER_FLOAT		LinearAdd ;				// �t�H�O�p�p�����[�^ end / ( end - start )
	DX_D3D11_SHADER_FLOAT		LinearDiv ;				// �t�H�O�p�p�����[�^ -1  / ( end - start )
	DX_D3D11_SHADER_FLOAT		Density ;				// �t�H�O�p�p�����[�^ density
	DX_D3D11_SHADER_FLOAT		E ;						// �t�H�O�p�p�����[�^ ���R�ΐ��̒�

	DX_D3D11_SHADER_FLOAT4		Color ;					// �J���[
} ;

// ���C�g�p�����[�^
struct DX_D3D11_CONST_LIGHT
{
	DX_D3D11_SHADER_INT			Type ;					// ���C�g�^�C�v( DX_LIGHTTYPE_POINT �Ȃ� )
	DX_D3D11_SHADER_INT3		Padding1 ;				// �p�f�B���O�P

	DX_D3D11_SHADER_FLOAT3		Position ;				// ���W( �r���[��� )
	DX_D3D11_SHADER_FLOAT		RangePow2 ;				// �L�������̂Q��

	DX_D3D11_SHADER_FLOAT3		Direction ;				// ����( �r���[��� )
	DX_D3D11_SHADER_FLOAT		FallOff ;				// �X�|�b�g���C�g�pFallOff

	DX_D3D11_SHADER_FLOAT3		Diffuse ;				// �f�B�t���[�Y�J���[
	DX_D3D11_SHADER_FLOAT		SpotParam0 ;			// �X�|�b�g���C�g�p�p�����[�^�O( cos( Phi / 2.0f ) )

	DX_D3D11_SHADER_FLOAT3		Specular ;				// �X�y�L�����J���[
	DX_D3D11_SHADER_FLOAT		SpotParam1 ;			// �X�|�b�g���C�g�p�p�����[�^�P( 1.0f / ( cos( Theta / 2.0f ) - cos( Phi / 2.0f ) ) )

	DX_D3D11_SHADER_FLOAT4		Ambient ;				// �A���r�G���g�J���[�ƃ}�e���A���̃A���r�G���g�J���[����Z��������

	DX_D3D11_SHADER_FLOAT		Attenuation0 ;			// �����ɂ�錸�������p�p�����[�^�O
	DX_D3D11_SHADER_FLOAT		Attenuation1 ;			// �����ɂ�錸�������p�p�����[�^�P
	DX_D3D11_SHADER_FLOAT		Attenuation2 ;			// �����ɂ�錸�������p�p�����[�^�Q
	DX_D3D11_SHADER_FLOAT		Padding2 ;				// �p�f�B���O�Q
} ;

// �s�N�Z���V�F�[�_�[�E���_�V�F�[�_�[���ʃp�����[�^
struct DX_D3D11_CONST_BUFFER_COMMON
{
	DX_D3D11_CONST_LIGHT		Light[ DX_D3D11_COMMON_CONST_LIGHT_NUM ] ;			// ���C�g�p�����[�^
	DX_D3D11_CONST_MATERIAL		Material ;				// �}�e���A���p�����[�^
	DX_D3D11_VS_CONST_FOG		Fog ;					// �t�H�O�p�����[�^
} ;

#endif // DXSHADER_COMMON_D3D11

