// -------------------------------------------------------------------------------
// 
// 		�c�w���C�u����		�s�N�Z���V�F�[�_�[��`�R�[�h
// 
// 				Ver 3.16d
// 
// -------------------------------------------------------------------------------

// ���ʕ���

// �s�N�Z���V�F�[�_�[�̓���
struct PS_INPUT
{
	float4 Diffuse         : COLOR0 ;		// �f�B�t���[�Y�J���[
#if USE_SPE
	float4 Specular        : COLOR1 ;		// �X�y�L�����J���[
#endif // USE_SPE
	float4 TexCoords0_1    : TEXCOORD0 ;	// xy:�e�N�X�`�����W zw:�T�u�e�N�X�`�����W
	float3 VPosition       : TEXCOORD1 ;	// ���_���W���王���ւ̃x�N�g��( �r���[��� )
	float3 VNormal         : TEXCOORD2 ;	// �@��( �r���[��� )
#ifdef BUMPMAP
	float3 VTan            : TEXCOORD3 ;    // �ڐ�( �r���[��� )
	float3 VBin            : TEXCOORD4 ;    // �]�@��( �r���[��� )
#endif // BUMPMAP
	float2 Fog_ShadowMap3PosZ             : TEXCOORD5 ;	// �t�H�O�p�����[�^( x )�ƃV���h�E�}�b�v�R�̃��C�g���W( y )

#if SHADOWMAP
	float4 ShadowMap1Pos_ShadowMap3PosX   : TEXCOORD6 ;	// �V���h�E�}�b�v�P�̃��C�g���W( x, y, z )�ƃV���h�E�}�b�v�R�̃��C�gX���W( w )
	float4 ShadowMap2Pos_ShadowMap3PosY   : TEXCOORD7 ;	// �V���h�E�}�b�v�Q�̃��C�g���W( x, y, z )�ƃV���h�E�}�b�v�R�̃��C�gY���W( w )
#endif // SHADOWMAP
} ;

// �s�N�Z���V�F�[�_�[�̏o��
struct PS_OUTPUT
{
	float4 Color0          : COLOR0 ;	// �F
} ;

// �}�e���A���p�����[�^
struct MATERIAL
{
	float4 Diffuse ;      // �f�B�t���[�Y�J���[
	float4 Specular ;     // �X�y�L�����J���[
	float4 Power ;        // �X�y�L�����̋���
} ;

// ���C�g�p�����[�^
struct LIGHT
{
	float4 Position ;               // ���W( �r���[��� )
	float3 Direction ;              // ����( �r���[��� )
	float4 Diffuse ;                // �f�B�t���[�Y�J���[
	float4 Specular ;               // �X�y�L�����J���[
	float4 Ambient ;                // �A���r�G���g�J���[�ƃ}�e���A���̃A���r�G���g�J���[����Z��������
	float4 Range_FallOff_AT0_AT1 ;  // x:�L������  y:�X�|�b�g���C�g�pFallOff  z:�����ɂ�錸�������p�p�����[�^�O  w:�����ɂ�錸�������p�p�����[�^�P
	float4 AT2_SpotP0_SpotP1 ;      // x:�����ɂ�錸�������p�p�����[�^�Q  y:�X�|�b�g���C�g�p�p�����[�^�O( cos( Phi / 2.0f ) )  z:�X�|�b�g���C�g�p�p�����[�^�P( 1.0f / ( cos( Theta / 2.0f ) - cos( Phi / 2.0f ) ) )
} ;



// C++ ���Őݒ肷��萔�̒�`
sampler  DiffuseMapTexture             : register( s0 ) ;		// �f�B�t���[�Y�}�b�v�e�N�X�`��

#if BUMPMAP
sampler  NormalMapTexture              : register( s1 ) ;		// �@���}�b�v�e�N�X�`��
#endif // BUMPMAP

#if USE_SPETEX
sampler  SpecularMapTexture            : register( s2 ) ;		// �X�y�L�����}�b�v�e�N�X�`��
#endif // USE_SPETEX

#if TOON
sampler  ToonDiffuseGradTexture        : register( s3 ) ;		// �g�D�[�������_�����O�p�f�B�t���[�Y�J���[�O���f�[�V�����e�N�X�`��
sampler  ToonSpecularGradTexture       : register( s4 ) ;		// �g�D�[�������_�����O�p�X�y�L�����J���[�O���f�[�V�����e�N�X�`��
#if TOON_SPHEREOP_MUL || TOON_SPHEREOP_ADD
sampler  ToonSphereMapTexture          : register( s5 ) ;		// �g�D�[�������_�����O�p�X�t�B�A�}�b�v�e�N�X�`��
#endif // TOON_SPHEREOP_MUL || TOON_SPHEREOP_ADD
sampler  ToonRGBtoVMaxRGBVolumeTexture : register( s6 ) ;		// �g�D�[�������_�����O�pRGB�P�x�O�a�����p�{�����[���e�N�X�`��
#endif // TOON

#if SUBTEXTUREMODE != 0
sampler  SubTexture					   : register( s7 ) ;		// �T�u�e�N�X�`��
#endif // SUBTEXTUREMODE != 0

#if SHADOWMAP
sampler  ShadowMap1Texture             : register( s8 ) ;		// �V���h�E�}�b�v�P�e�N�X�`��
sampler  ShadowMap2Texture             : register( s9 ) ;		// �V���h�E�}�b�v�Q�e�N�X�`��
sampler  ShadowMap3Texture             : register( s10 ) ;		// �V���h�E�}�b�v�R�e�N�X�`��
#endif

float4   cfZeroHalfOneTwo              : register( c0 /* DX_PS_CONSTF_ZERO_HALF_ONE_TWO           */ ) ;		// x=0.0f, y=0.5f, z=1.0f, w=2.0f
float4   cfAmbient_Emissive            : register( c1 /* DX_PS_CONSTF_AMBIENT_EMISSIVE            */ ) ;		// �G�~�b�V�u�J���[ + �}�e���A���A���r�G���g�J���[ * �O���[�o���A���r�G���g�J���[

MATERIAL cfMaterial                    : register( c2 /* DX_PS_CONSTF_MATERIAL_START              */ ) ;		// �}�e���A���p�����[�^

float4   cfFactorColor                 : register( c5 /* DX_PS_CONSTF_FACTORCOLOR                 */ ) ;		// �A���t�@�l��
float4   cfToonOutLineColor            : register( c6 /* DX_PS_CONSTF_TOON_OUTLINE_COLOR          */ ) ;		// �g�D�[�������_�����O�̗֊s���̐F
float4   cfToonOutLineWidth            : register( c7 /* DX_PS_CONSTF_TOON_OUTLINE_WIDTH          */ ) ;		// �g�D�[�������_�����O�̗֊s���̑���
float4   cfFogColor                    : register( c8 /* DX_PS_CONSTF_FOG_COLOR                   */ ) ;		// �t�H�O�J���[

float4   cfShadowMap1_DAdj_Grad_Enbl0_1               : register( c18 /* DX_PS_CONSTF_SHADOW1_DADJ_GRAD_ENBL0_1             */ ) ;		// �V���h�E�}�b�v�P�p�̏��( x:臒l�[�x�␳�l  y:�O���f�[�V�����͈�  zw:���C�g�O�E�P�ւ̓K�p���)
float4   cfShadowMap1_Enb2_ShadowMap2_DAdj_Grad_Enbl0 : register( c19 /* DX_PS_CONSTF_SHADOW1_ENBL2_SHADOW2_DADJ_GRAD_ENBL0 */ ) ;		// �V���h�E�}�b�v�P�p�̏��( x:���C�g�Q�ւ̓K�p��� )�A�V���h�E�}�b�v�Q�p�̏��( y:臒l�[�x�␳�l  z:�O���f�[�V�����͈�  w:���C�g�O�ւ̓K�p��� )
float4   cfShadowMap2_Enbl1_2_ShadowMap3_DAdj_Grad    : register( c20 /* DX_PS_CONSTF_SHADOW2_ENBL1_2_SHADOW3_DADJ_GRAD     */ ) ;		// �V���h�E�}�b�v�Q�p�̏��( xy:���C�g�P�E�Q�ւ̓K�p���)�A�V���h�E�}�b�v�R�p�̏��( z:臒l�[�x�␳�l  w:�O���f�[�V�����͈� )
float4   cfShadowMap3_Enbl0_1_2                       : register( c21 /* DX_PS_CONSTF_SHADOW3_ENBL0_1_2                     */ ) ;		// �V���h�E�}�b�v�R�p�̏��( xyz:���C�g�O�`�Q�ւ̓K�p��� )
float4   cfMulAlphaColor                : register( c22 /* DX_PS_CONSTF_MUL_ALPHA_COLOR           */ ) ;		// �J���[�ɃA���t�@�l����Z���邩�ǂ���( x( 0.0f:��Z���Ȃ�  1.0f:��Z���� ) yzw( ���g�p ) )

LIGHT    cfLight[ 6 ]                  : register( c32 /* DX_PS_CONSTF_LIGHT2_START               */ ) ;		// ���C�g�p�����[�^

#define SHADOWMAP1_ENABLE_LGT0 cfShadowMap1_DAdj_Grad_Enbl0_1.z
#define SHADOWMAP1_ENABLE_LGT1 cfShadowMap1_DAdj_Grad_Enbl0_1.w
#define SHADOWMAP1_ENABLE_LGT2 cfShadowMap1_Enb2_ShadowMap2_DAdj_Grad_Enbl0.x
#define SHADOWMAP1_ENABLE_LGT3 cfZeroHalfOneTwo.x
#define SHADOWMAP1_ENABLE_LGT4 cfZeroHalfOneTwo.x
#define SHADOWMAP1_ENABLE_LGT5 cfZeroHalfOneTwo.x

#define SHADOWMAP2_ENABLE_LGT0 cfShadowMap1_Enb2_ShadowMap2_DAdj_Grad_Enbl0.w
#define SHADOWMAP2_ENABLE_LGT1 cfShadowMap2_Enbl1_2_ShadowMap3_DAdj_Grad.x
#define SHADOWMAP2_ENABLE_LGT2 cfShadowMap2_Enbl1_2_ShadowMap3_DAdj_Grad.y
#define SHADOWMAP2_ENABLE_LGT3 cfZeroHalfOneTwo.x
#define SHADOWMAP2_ENABLE_LGT4 cfZeroHalfOneTwo.x
#define SHADOWMAP2_ENABLE_LGT5 cfZeroHalfOneTwo.x

#define SHADOWMAP3_ENABLE_LGT0 cfShadowMap3_Enbl0_1_2.x
#define SHADOWMAP3_ENABLE_LGT1 cfShadowMap3_Enbl0_1_2.y
#define SHADOWMAP3_ENABLE_LGT2 cfShadowMap3_Enbl0_1_2.z
#define SHADOWMAP3_ENABLE_LGT3 cfZeroHalfOneTwo.x
#define SHADOWMAP3_ENABLE_LGT4 cfZeroHalfOneTwo.x
#define SHADOWMAP3_ENABLE_LGT5 cfZeroHalfOneTwo.x




