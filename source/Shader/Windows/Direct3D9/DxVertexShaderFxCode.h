// -------------------------------------------------------------------------------
// 
// 		�c�w���C�u����		���_�V�F�[�_�[��`�R�[�h
// 
// 				Ver 3.16d
// 
// -------------------------------------------------------------------------------

// ���ʕ���

#include "../../../Main/Windows/DxShader_VS_D3D9.h"

// ���_�V�F�[�_�[�̓���
struct VS_INPUT
{
	float4 Position        : POSITION ;			// ���W( ���[�J����� )
	float3 Normal          : NORMAL0 ;			// �@��( ���[�J����� )
	float4 Diffuse         : COLOR0 ;			// �f�B�t���[�Y�J���[
	float4 Specular        : COLOR1 ;			// �X�y�L�����J���[
	float4 TexCoords0      : TEXCOORD0 ;		// �e�N�X�`�����W
	float4 TexCoords1      : TEXCOORD1 ;		// �T�u�e�N�X�`�����W

#ifdef BUMPMAP
	// �o���v�}�b�v
	float3 Tan             : TANGENT0 ;			// �ڐ�( ���[�J����� )
	float3 Bin             : BINORMAL0 ;		// �]�@��( ���[�J����� )
#endif // BUMPMAP

#ifdef SKINMESH
	// �X�L�j���O���b�V��
	int4   BlendIndices0   : BLENDINDICES0 ;	// �{�[�������p Float�^�萔�z��C���f�b�N�X�O
	float4 BlendWeight0    : BLENDWEIGHT0 ;		// �{�[�������p�E�G�C�g�l�O

	#ifdef BONE8

		int4   BlendIndices1   : BLENDINDICES1 ;	// �{�[�������p Float�^�萔�z��C���f�b�N�X�P
		float4 BlendWeight1    : BLENDWEIGHT1 ;		// �{�[�������p�E�G�C�g�l�P

	#endif // BONE8

#endif // SKINMESH
} ;

// ���_�V�F�[�_�[�̏o��
struct VS_OUTPUT
{
	float4 Position        : POSITION ;		// ���W( �v���W�F�N�V������� )
	float4 Diffuse         : COLOR0 ;		// �f�B�t���[�Y�J���[
#if USE_SPE
	float4 Specular        : COLOR1 ;		// �X�y�L�����J���[
#endif
	float4 TexCoords0_1    : TEXCOORD0 ;	// �e�N�X�`�����W�ƃT�u�e�N�X�`�����W( x:�e�N�X�`�����Wx  y:�e�N�X�`�����Wy  z:�T�u�e�N�X�`�����Wx  w:�T�u�e�N�X�`�����Wy )
#if ( ( BUMPMAP || PHONG ) && USE_SPE ) || SHADOWMAP
	float4 V_to_Eye_ShadowMap3PosZ : TEXCOORD1 ;	// ���_���W���王���ւ̃x�N�g��( �r���[��� )�ƃV���h�E�}�b�v�R�̃��C�gZ���W( w )
#endif
#if ( BUMPMAP == 0 && PHONG ) || SM_3
	float4 Normal_Fog      : TEXCOORD2 ;	// x.y.z:�@��( �r���[��� )  w:SM3.0�p�̃t�H�O�p�����[�^
#endif
#if LG0_USE && ( BUMPMAP || PHONG )
	float4 Light0_Dir_Gen  : TEXCOORD3 ;	// xyz:���C�g�O�̋t�x�N�g��( �r���[��� ) w:���C�g�O�������
#endif
#if LG1_USE && ( BUMPMAP || PHONG )
	float4 Light1_Dir_Gen  : TEXCOORD4 ;	// xyz:���C�g�P�̋t�x�N�g��( �r���[��� ) w:���C�g�P�������
#endif
#if LG2_USE && ( BUMPMAP || PHONG )
	float4 Light2_Dir_Gen  : TEXCOORD5 ;	// xyz:���C�g�Q�̋t�x�N�g��( �r���[��� ) w:���C�g�Q�������
#endif

#if SHADOWMAP || SHADOWMAP_DRAW
	float4 ShadowMap1Pos_ShadowMap3PosX   : TEXCOORD6 ;	// �V���h�E�}�b�v�P�̃��C�g���W( x, y, z )�ƃV���h�E�}�b�v�R�̃��C�gX���W( w )
#endif
#if SHADOWMAP
	float4 ShadowMap2Pos_ShadowMap3PosY   : TEXCOORD7 ;	// �V���h�E�}�b�v�Q�̃��C�g���W( x, y, z )�ƃV���h�E�}�b�v�R�̃��C�gY���W( w )
#endif

#if SM_3 == 0 && ( FOG_LINEAR || FOG_EXP || FOG_EXP2 )
	float  Fog             : FOG ;			// �t�H�O
#endif
} ;

// ���_�V�F�[�_�[�̃}�e���A���p�����[�^
struct VS_CONST_MATERIAL
{
	float4 Diffuse ;				// �f�B�t���[�Y�J���[
	float4 Specular ;				// �X�y�L�����J���[
	float4 Power ;					// �X�y�L�����̋���
} ;

// ���_�V�F�[�_�[�̃��C�g�p�����[�^
struct VS_CONST_LIGHT
{
	float4 Position ;				// ���W( �r���[��� )
	float3 Direction ;				// ����( �r���[��� )
	float4 Diffuse ;				// �f�B�t���[�Y�J���[
	float4 Specular ;				// �X�y�L�����J���[
	float4 Ambient ;				// �A���r�G���g�J���[
	float4 Range_FallOff_AT0_AT1 ;	// x:�L������  y:�X�|�b�g���C�g�pFllOff  z:�����ɂ�錸�������p�p�����[�^�O  w:�����ɂ�錸�������p�p�����[�^�P
	float4 AT2_SpotP0_SpotP1 ;		// x:�����ɂ�錸�������p�p�����[�^�Q  y:�X�|�b�g���C�g�p�p�����[�^�O( cos( Phi / 2.0f ) )  z:�X�|�b�g���C�g�p�p�����[�^�P( 1.0f / ( cos( Theta / 2.0f ) - cos( Phi / 2.0f ) ) )
} ;

// ���_�V�F�[�_�[�̃��C�g��������p�p�����[�^
struct VS_CONST_LIGHT_BOOL
{
	bool Use ;						// �g�p���Ă��邩�ǂ���
	bool Point_Or_Spot ;			// �|�C���g���C�g�Ⴕ���̓X�|�b�g���C�g��
	bool Spot ;						// �X�|�b�g���C�g��
} ;



// C++ ���Őݒ肷��萔�̒�`
float4              cfZeroOne                                                  : register( c0  /* DX_VS_CONSTF_ZERO_ONE                        */ ) ;	// �O�ƂP�̒萔( x:0.0f  y:1.0f )
float4              cfAmbient_Emissive                                         : register( c1  /* DX_VS_CONSTF_AMBIENT_EMISSIVE                */ ) ;	// �G�~�b�V�u�J���[�{�A���r�G���g�J���[( �G�~�b�V�u�J���[ + �}�e���A���A���r�G���g�J���[ * �O���[�o���A���r�G���g�J���[ )
float4              cfProjectionMatrix[ 4 ]                                    : register( c2  /* DX_VS_CONSTF_PROJECTION_MAT                  */ ) ;	// �r���[�@���@�v���W�F�N�V�����s��
float4              cfViewMatrix[ 3 ]                                          : register( c6  /* DX_VS_CONSTF_VIEW_MAT                        */ ) ;	// ���[���h�@���@�r���[�s��
float4              cfFog                                                      : register( c10 /* DX_VS_CONSTF_FOG                             */ ) ;	// �t�H�O�p�p�����[�^( x:end/(end - start)  y:-1/(end - start)  z:density  w:���R�ΐ��̒� )

VS_CONST_MATERIAL   cfMaterial                                                 : register( c11 /* DX_VS_CONSTF_MATERIAL_START                  */ ) ;	// �}�e���A���p�����[�^
VS_CONST_LIGHT      cfLight[ DX_VS_CONSTF_LIGHT_NUM ]                          : register( c14 /* DX_VS_CONSTF_LIGHT_START                     */ ) ;	// ���C�g�p�����[�^
float4              cfToonOutLineSize                                          : register( c42 /* DX_VS_CONSTF_TOON_OUTLINE_SIZE               */ ) ;	// �g�D�[���̗֊s���̑傫��
float4              cfDifSpeSource                                             : register( c43 /* DX_VS_CONSTF_DIF_SPE_SOURCE                  */ ) ;	// x:�f�B�t���[�Y�J���[( 0.0f:�}�e���A��  1.0f:���_ )  y:�X�y�L�����J���[( 0.0f:�}�e���A��  1.0f:���_ )

float4              cfShadowMap1LightViewProjectionMatrix[ 4 ]                 : register( c44 /* DX_VS_CONSTF_SHADOWMAP1_LIGHT_VIEW_PROJECTION_MAT */ ) ;	// �V���h�E�}�b�v�P�p�̃��C�g�r���[�s��ƃ��C�g�ˉe�s�����Z��������
float4              cfShadowMap2LightViewProjectionMatrix[ 4 ]                 : register( c48 /* DX_VS_CONSTF_SHADOWMAP2_LIGHT_VIEW_PROJECTION_MAT */ ) ;	// �V���h�E�}�b�v�Q�p�̃��C�g�r���[�s��ƃ��C�g�ˉe�s�����Z��������
float4              cfShadowMap3LightViewProjectionMatrix[ 4 ]                 : register( c52 /* DX_VS_CONSTF_SHADOWMAP3_LIGHT_VIEW_PROJECTION_MAT */ ) ;	// �V���h�E�}�b�v�R�p�̃��C�g�r���[�s��ƃ��C�g�ˉe�s�����Z��������

float4              cfTextureMatrix[ DX_VS_CONSTF_TEXTURE_MATRIX_NUM ][ 2 ]    : register( c88 /* DX_VS_CONSTF_TEXTURE_MATRIX_START            */ ) ;	// �e�N�X�`�����W����p�s��
float4              cfLocalWorldMatrix[ DX_VS_CONSTF_WORLD_MAT_NUM * 3 ]       : register( c94 /* DX_VS_CONSTF_WORLD_MAT_START                 */ ) ;	// ���[�J���@���@���[���h�s��

bool                cbFogLinear                                                : register( b0  /* DX_VS_CONSTB_FOG_LINEAR                      */ ) ;	// ���`�t�H�O���|���邩�ǂ���
bool                cbFogExp                                                   : register( b1  /* DX_VS_CONSTB_FOG_EXP                         */ ) ;	// �w���֐��t�H�O���|���邩�ǂ���
bool                cbFogExp2                                                  : register( b2  /* DX_VS_CONSTB_FOG_EXP2                        */ ) ;	// �w���֐��t�H�O�Q���|���邩�ǂ���
bool                cbFogUse                                                   : register( b3  /* DX_VS_CONSTB_FOG_USE                         */ ) ;	// �t�H�O���|���邩�ǂ���
VS_CONST_LIGHT_BOOL cbLightBool[ DX_VS_CONSTB_LIGHT_NUM ]                      : register( b4  /* DX_VS_CONSTB_LIGHT_START                     */ ) ;	// ���C�g��������p�p�����[�^



#define ZERO	                cfZeroOne.x
#define ONE                     cfZeroOne.y

#define FOG_LINEAR_ADD          cfFog.x
#define FOG_LINEAR_DIV          cfFog.y
#define FOG_EXP_DENSITY         cfFog.z
#define FOG_EXP_E               cfFog.w

#ifdef SKINMESH
#define LOCAL_WORLD_MAT         lLocalWorldMatrix
#else
#define LOCAL_WORLD_MAT         cfLocalWorldMatrix
#endif
/*
#ifdef DIF_MAT
#define DIF                     cfMaterial.Diffuse
#endif

#ifdef DIF_VTX
#define DIF                     VSInput.Diffuse
#endif

#ifdef SPE_MAT
#define SPE                     cfMaterial.Specular
#endif

#ifdef SPE_VTX
#define SPE                     VSInput.Specular
#endif
*/
#define TEXMTX0_U               cfTextureMatrix[ 0 ][ 0 ]
#define TEXMTX0_V               cfTextureMatrix[ 0 ][ 1 ]

#if SM_3
#define FOG_OUT                 VSOutput.Normal_Fog.w
#else
#define FOG_OUT                 VSOutput.Fog
#endif



