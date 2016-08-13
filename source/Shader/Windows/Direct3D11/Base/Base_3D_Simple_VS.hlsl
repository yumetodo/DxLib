#include "../VertexShader.h"

// ���_�V�F�[�_�[�̓���
struct VS_INPUT
{
	float3 Position        : POSITION0 ;		// ���W( ���[�J����� )

#if USE_VERTEX3DSHADER
	float4 SubPosition     : POSITION1 ;		// �⏕���W
#endif // USE_VERTEX3DSHADER

#if USE_VERTEX3DSHADER || USE_VERTEX3D
	float3 Normal          : NORMAL ;			// �@��
#endif // USE_VERTEX3DSHADER || USE_VERTEX3D

#if USE_VERTEX3DSHADER
	float3 Tangent         : TANGENT ;			// �ڐ�
	float3 Binormal        : BINORMAL ;			// �]�@��
#endif // USE_VERTEX3DSHADER

	float4 Diffuse         : COLOR0 ;			// �f�B�t���[�Y�J���[

#if USE_VERTEX3DSHADER || USE_VERTEX3D
	float4 Specular        : COLOR1 ;			// �X�y�L�����J���[
#endif // USE_VERTEX3DSHADER || USE_VERTEX3D

#if USE_TEXCOORD0 || USE_VERTEX3DSHADER || USE_VERTEX3D
	float2 TexCoords0      : TEXCOORD0 ;		// �e�N�X�`�����W�O
#endif // USE_TEXCOORD0 || USE_VERTEX3DSHADER || USE_VERTEX3D

#if USE_VERTEX3DSHADER || USE_VERTEX3D
	float2 TexCoords1      : TEXCOORD1 ;		// �e�N�X�`�����W�P
#endif // USE_VERTEX3DSHADER || USE_VERTEX3D
} ;

// ���_�V�F�[�_�[�̏o��
struct VS_OUTPUT
{
	float4 Position        : SV_POSITION ;		// ���W( �v���W�F�N�V������� )
	float4 Diffuse         : COLOR0 ;			// �f�B�t���[�Y�J���[
	float2 TexCoords0      : TEXCOORD0 ;		// �e�N�X�`�����W�O
	float2 TexCoords1      : TEXCOORD1 ;		// �e�N�X�`�����W�P
} ;

// 3D�p
VS_OUTPUT VS3D_Normal( VS_INPUT VSInput )
{
	VS_OUTPUT VSOutput ;
	float4    LocalPosition ;
	float4    WorldPosition ;
	float4    ViewPosition ;
	float4    lTextureCoordTemp ;


	// ���[�J�����W�̏���
	LocalPosition.xyz   = VSInput.Position ;
	LocalPosition.w     = 1.0f ;


	// ���W�ϊ�
	WorldPosition.x     = dot( LocalPosition, g_Base.LocalWorldMatrix[ 0 ] ) ;
	WorldPosition.y     = dot( LocalPosition, g_Base.LocalWorldMatrix[ 1 ] ) ;
	WorldPosition.z     = dot( LocalPosition, g_Base.LocalWorldMatrix[ 2 ] ) ;
	WorldPosition.w     = 1.0f;
	
	ViewPosition.x      = dot( WorldPosition, g_Base.ViewMatrix[ 0 ] ) ;
	ViewPosition.y      = dot( WorldPosition, g_Base.ViewMatrix[ 1 ] ) ;
	ViewPosition.z      = dot( WorldPosition, g_Base.ViewMatrix[ 2 ] ) ;
	ViewPosition.w      = 1.0f;
	
	VSOutput.Position.x = dot( ViewPosition, g_Base.ProjectionMatrix[ 0 ] ) ;
	VSOutput.Position.y = dot( ViewPosition, g_Base.ProjectionMatrix[ 1 ] ) ;
	VSOutput.Position.z = dot( ViewPosition, g_Base.ProjectionMatrix[ 2 ] ) ;
	VSOutput.Position.w = dot( ViewPosition, g_Base.ProjectionMatrix[ 3 ] ) ;


	// �p�����[�^�Z�b�g
	VSOutput.Diffuse    = VSInput.Diffuse ;

	lTextureCoordTemp.zw = 1.0f ;
#if USE_TEXCOORD0 || USE_VERTEX3DSHADER || USE_VERTEX3D
	lTextureCoordTemp.xy = VSInput.TexCoords0 ;
	VSOutput.TexCoords0.x = dot( lTextureCoordTemp, g_OtherMatrix.TextureMatrix[ 0 ][ 0 ] ) ;
	VSOutput.TexCoords0.y = dot( lTextureCoordTemp, g_OtherMatrix.TextureMatrix[ 0 ][ 1 ] ) ;
#else  // USE_TEXCOORD0 || USE_VERTEX3DSHADER || USE_VERTEX3D
	VSOutput.TexCoords0 = 0.0f ;
#endif // USE_TEXCOORD0 || USE_VERTEX3DSHADER || USE_VERTEX3D

#if USE_VERTEX3DSHADER || USE_VERTEX3D
	lTextureCoordTemp.xy = VSInput.TexCoords1 ;
	VSOutput.TexCoords1.x = dot( lTextureCoordTemp, g_OtherMatrix.TextureMatrix[ 0 ][ 0 ] ) ;
	VSOutput.TexCoords1.y = dot( lTextureCoordTemp, g_OtherMatrix.TextureMatrix[ 0 ][ 1 ] ) ;
#else  // USE_VERTEX3DSHADER || USE_VERTEX3D
	VSOutput.TexCoords1 = 0.0f ;
#endif // USE_VERTEX3DSHADER || USE_VERTEX3D
	
	return VSOutput ;
}
