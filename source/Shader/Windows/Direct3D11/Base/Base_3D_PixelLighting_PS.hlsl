#include "../PixelShader.h"


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
	float1 Fog             : TEXCOORD5 ;	// �t�H�O�p�����[�^( x )�ƃV���h�E�}�b�v�Q�̃��C�g���W( y )

#if SHADOWMAP
	float3 ShadowMap0Pos   : TEXCOORD6 ;	// �V���h�E�}�b�v�O�̃��C�g���W( x, y, z )
	float3 ShadowMap1Pos   : TEXCOORD7 ;	// �V���h�E�}�b�v�P�̃��C�g���W( x, y, z )
	float3 ShadowMap2Pos   : TEXCOORD8 ;	// �V���h�E�}�b�v�Q�̃��C�g���W( x, y, z )
#endif // SHADOWMAP
} ;

// �s�N�Z���V�F�[�_�[�̏o��
struct PS_OUTPUT
{
	float4 Color0          : SV_TARGET0 ;	// �F
} ;



SamplerState g_DiffuseMapSampler            : register( s0 ) ;		// �f�B�t���[�Y�}�b�v�e�N�X�`��
Texture2D    g_DiffuseMapTexture            : register( t0 ) ;		// �f�B�t���[�Y�}�b�v�e�N�X�`��

#if BUMPMAP
SamplerState g_NormalMapSampler             : register( s1 ) ;		// �@���}�b�v�e�N�X�`��
Texture2D    g_NormalMapTexture             : register( t1 ) ;		// �@���}�b�v�e�N�X�`��
#endif // BUMPMAP

#if USE_SPETEX
SamplerState g_SpecularMapSampler           : register( s2 ) ;		// �X�y�L�����}�b�v�e�N�X�`��
Texture2D    g_SpecularMapTexture           : register( t2 ) ;		// �X�y�L�����}�b�v�e�N�X�`��
#endif // USE_SPETEX

#if SUBTEXTUREMODE != 0
SamplerState g_SubSampler                   : register( s7 ) ;		// �T�u�e�N�X�`��
Texture2D    g_SubTexture                   : register( t7 ) ;		// �T�u�e�N�X�`��
#endif // SUBTEXTUREMODE != 0

#if SHADOWMAP
SamplerState g_ShadowMap0Sampler            : register( s8  ) ;		// �V���h�E�}�b�v�O�e�N�X�`��
Texture2D    g_ShadowMap0Texture            : register( t8  ) ;		// �V���h�E�}�b�v�O�e�N�X�`��

SamplerState g_ShadowMap1Sampler            : register( s9  ) ;		// �V���h�E�}�b�v�P�e�N�X�`��
Texture2D    g_ShadowMap1Texture            : register( t9  ) ;		// �V���h�E�}�b�v�P�e�N�X�`��

SamplerState g_ShadowMap2Sampler            : register( s10 ) ;		// �V���h�E�}�b�v�Q�e�N�X�`��
Texture2D    g_ShadowMap2Texture            : register( t10 ) ;		// �V���h�E�}�b�v�Q�e�N�X�`��
#endif


#define SHADOWMAP0_ENABLE_LGT0 g_ShadowMap.Data[ 0 ].Enable_Light0
#define SHADOWMAP0_ENABLE_LGT1 g_ShadowMap.Data[ 0 ].Enable_Light1
#define SHADOWMAP0_ENABLE_LGT2 g_ShadowMap.Data[ 0 ].Enable_Light2
#define SHADOWMAP0_ENABLE_LGT3 0.0f
#define SHADOWMAP0_ENABLE_LGT4 0.0f
#define SHADOWMAP0_ENABLE_LGT5 0.0f

#define SHADOWMAP1_ENABLE_LGT0 g_ShadowMap.Data[ 1 ].Enable_Light0
#define SHADOWMAP1_ENABLE_LGT1 g_ShadowMap.Data[ 1 ].Enable_Light1
#define SHADOWMAP1_ENABLE_LGT2 g_ShadowMap.Data[ 1 ].Enable_Light2
#define SHADOWMAP1_ENABLE_LGT3 0.0f
#define SHADOWMAP1_ENABLE_LGT4 0.0f
#define SHADOWMAP1_ENABLE_LGT5 0.0f

#define SHADOWMAP2_ENABLE_LGT0 g_ShadowMap.Data[ 2 ].Enable_Light0
#define SHADOWMAP2_ENABLE_LGT1 g_ShadowMap.Data[ 2 ].Enable_Light1
#define SHADOWMAP2_ENABLE_LGT2 g_ShadowMap.Data[ 2 ].Enable_Light2
#define SHADOWMAP2_ENABLE_LGT3 0.0f
#define SHADOWMAP2_ENABLE_LGT4 0.0f
#define SHADOWMAP2_ENABLE_LGT5 0.0f





// main�֐�
PS_OUTPUT main( PS_INPUT PSInput )
{
	PS_OUTPUT	PSOutput ;
	float4		TextureDiffuseColor ;
	float4		TextureSpecularColor ;
	
	float		DiffuseAngleGen ;
	float3		TotalDiffuse ;
	float3		TotalSpecular ;
	float		Temp ;
	float3		TempF3 ;
	float3		Normal ;
	float3		V_to_Eye ;
	float3		VNrm ;
	float3		VTan ;
	float3		VBin ;
	float3		lLightTemp ;
	float		lLightDistancePow2 ;
	float		lLightGen ;
	float3		lLightDir ;
	float		lLightDirectionCosA ;
	float3		ShadowRate ;
	float		ShadowGen ;

#if SHADOWMAP
	float2		DepthTexCoord ;
	float4		TextureDepth ;
//	float4		variance_md_p_pmax ;
#endif
	
#if SUBTEXTUREMODE != 0
	float4 SubTextureColor;
#endif  // SUBTEXTUREMODE != 0

	// �ڐ��E�]�@���E�@���𐳋K��
	VNrm = normalize( PSInput.VNormal ) ;
#ifdef BUMPMAP
	VTan = normalize( PSInput.VTan ) ;
	VBin = normalize( PSInput.VBin ) ;
#endif // BUMPMAP

#ifdef BUMPMAP
	// ���_���W���王�_�ւ̃x�N�g����ڒ��Ԃɓ��e�����㐳�K�����ĕۑ�
	TempF3.x = dot( VTan, -PSInput.VPosition.xyz ) ;
	TempF3.y = dot( VBin, -PSInput.VPosition.xyz ) ;
	TempF3.z = dot( VNrm, -PSInput.VPosition.xyz ) ;
	V_to_Eye = normalize( TempF3 ) ;
#else // BUMPMAP
	// ���_���W���王�_�ւ̃x�N�g���𐳋K��
	V_to_Eye = normalize( -PSInput.VPosition ) ;
#endif // BUMPMAP

	// �@���̏���
	#if BUMPMAP
		// �@���� 0�`1 �̒l�� -1.0�`1.0 �ɕϊ�����
		Normal = ( g_NormalMapTexture.Sample( g_NormalMapSampler, PSInput.TexCoords0_1.xy ).rgb - 0.5f ) * 2.0f ;
	#else // BUMPMAP
		Normal = normalize( PSInput.VNormal ) ;
	#endif // BUMPMAP

	// �f�B�t���[�Y�e�N�X�`���J���[���擾
	TextureDiffuseColor = g_DiffuseMapTexture.Sample( g_DiffuseMapSampler, PSInput.TexCoords0_1.xy ) ;

#ifdef INVERSE_COLOR
	TextureDiffuseColor.rgb = 1.0f - TextureDiffuseColor.rgb;
#endif
	
#ifdef USE_SPE
	#ifdef USE_SPETEX
	// �f�X�y�L�����e�N�X�`���J���[���擾
	TextureSpecularColor = g_SpecularMapTexture.Sample( g_SpecularMapSampler, PSInput.TexCoords0_1.xy ) ;
#ifdef INVERSE_COLOR
	TextureSpecularColor.rgb = 1.0f - TextureSpecularColor.rgb;
#endif
	#endif // USE_SPETEX
#endif // USE_SPE

#if SUBTEXTUREMODE != 0
	// �T�u�e�N�X�`���J���[���擾
	SubTextureColor = g_SubTexture.Sample( g_SubSampler, PSInput.TexCoords0_1.zw );
#endif  // SUBTEXTUREMODE != 0

	// �T�u�e�N�X�`���J���[�ƃf�B�t���[�Y�e�N�X�`���J���[�̊|�����킹
#if SUBTEXTUREMODE == 1  // ���u�����h( MV1_LAYERBLEND_TYPE_TRANSLUCENT + 1 )
	TextureDiffuseColor.rgb = lerp( TextureDiffuseColor.rgb, SubTextureColor.rgb, SubTextureColor.a );
#endif // SUBTEXTUREMODE == 1

#if SUBTEXTUREMODE == 2  // ���Z( MV1_LAYERBLEND_TYPE_ADDITIVE + 1 )
	TextureDiffuseColor.rgb += SubTextureColor.rgb;
#endif // SUBTEXTUREMODE == 2

#if SUBTEXTUREMODE == 3  // ��Z( MV1_LAYERBLEND_TYPE_MODULATE + 1 )
	TextureDiffuseColor.rgb *= SubTextureColor.rgb;
#endif // SUBTEXTUREMODE == 3

#if SUBTEXTUREMODE == 4  // ��Z�~�Q( MV1_LAYERBLEND_TYPE_MODULATE2 + 1 )
	TextureDiffuseColor.rgb *= SubTextureColor.rgb * 2.0f;
#endif // SUBTEXTUREMODE == 4

	// ���C�g���g���ꍇ *******************************************************************( �J�n )

	// �f�B�t���[�Y�J���[�ƃX�y�L�����J���[�̒~�ϒl��������
	TotalDiffuse = 0.0f ;
#ifdef USE_SPE
	TotalSpecular = 0.0f ;
#endif // USE_SPE
	
	#if	SHADOWMAP
		// ���_�̃e�N�X�`�����W�l���͈͓��̏ꍇ�̂ݏ�������
		if( PSInput.ShadowMap0Pos.x < -1.0f || PSInput.ShadowMap0Pos.x > 1.0f ||
		    PSInput.ShadowMap0Pos.y < -1.0f || PSInput.ShadowMap0Pos.y > 1.0f ||
		    PSInput.ShadowMap0Pos.z <  0.0f || PSInput.ShadowMap0Pos.z > 1.0f )
		{
			ShadowRate.x = 1.0f;
		}
		else
		{
			// �[�x�e�N�X�`���̍��W���Z�o
			// PSInput.ShadowMap0Pos.xy �� -1.0f �` 1.0f �̒l�Ȃ̂ŁA����� 0.0f �` 1.0f �̒l�ɂ���
			DepthTexCoord.x = ( PSInput.ShadowMap0Pos.x + 1.0f ) / 2.0f;

			// y�͍X�ɏ㉺���]
			DepthTexCoord.y = 1.0f - ( PSInput.ShadowMap0Pos.y + 1.0f ) / 2.0f;

			// �[�x�o�b�t�@�e�N�X�`������[�x���擾
			TextureDepth = g_ShadowMap0Texture.Sample( g_ShadowMap0Sampler, DepthTexCoord );

			// �e�N�X�`���ɋL�^����Ă���[�x( +�␳�l )���y�l���傫�������牜�ɂ���Ƃ������ƂŌ��������ő�ɂ���
			ShadowRate.x = smoothstep( PSInput.ShadowMap0Pos.z - g_ShadowMap.Data[ 0 ].GradationParam, PSInput.ShadowMap0Pos.z, TextureDepth.r + g_ShadowMap.Data[ 0 ].AdjustDepth ) ;
		}

		// ���_�̃e�N�X�`�����W�l���͈͓��̏ꍇ�̂ݏ�������
		if( PSInput.ShadowMap1Pos.x < -1.0f || PSInput.ShadowMap1Pos.x > 1.0f ||
		    PSInput.ShadowMap1Pos.y < -1.0f || PSInput.ShadowMap1Pos.y > 1.0f ||
		    PSInput.ShadowMap1Pos.z <  0.0f || PSInput.ShadowMap1Pos.z > 1.0f )
		{
			ShadowRate.y = 1.0f;
		}
		else
		{
			// �[�x�e�N�X�`���̍��W���Z�o
			// PSInput.ShadowMap2Pos_ShadowMap3PosX.xy �� -1.0f �` 1.0f �̒l�Ȃ̂ŁA����� 0.0f �` 1.0f �̒l�ɂ���
			DepthTexCoord.x = ( PSInput.ShadowMap1Pos.x + 1.0f ) / 2.0f;

			// y�͍X�ɏ㉺���]
			DepthTexCoord.y = 1.0f - ( PSInput.ShadowMap1Pos.y + 1.0f ) / 2.0f;

			// �[�x�o�b�t�@�e�N�X�`������[�x���擾
			TextureDepth = g_ShadowMap1Texture.Sample( g_ShadowMap1Sampler, DepthTexCoord );

			// �e�N�X�`���ɋL�^����Ă���[�x( +�␳�l )���y�l���傫�������牜�ɂ���Ƃ������ƂŌ��������ő�ɂ���
			ShadowRate.y = smoothstep( PSInput.ShadowMap1Pos.z - g_ShadowMap.Data[ 1 ].GradationParam, PSInput.ShadowMap1Pos.z, TextureDepth.r + g_ShadowMap.Data[ 1 ].AdjustDepth ) ;
		}

		// ���_�̃e�N�X�`�����W�l���͈͓��̏ꍇ�̂ݏ�������
		if( PSInput.ShadowMap2Pos.x < -1.0f || PSInput.ShadowMap2Pos.x > 1.0f ||
		    PSInput.ShadowMap2Pos.y < -1.0f || PSInput.ShadowMap2Pos.y > 1.0f ||
		    PSInput.ShadowMap2Pos.z <  0.0f || PSInput.ShadowMap2Pos.z > 1.0f )
		{
			ShadowRate.z = 1.0f;
		}
		else
		{
			// �[�x�e�N�X�`���̍��W���Z�o
			// PSInput.ShadowMap2Pos.x �� PSInput.ShadowMap2Pos.y �� -1.0f �` 1.0f �̒l�Ȃ̂ŁA����� 0.0f �` 1.0f �̒l�ɂ���
			DepthTexCoord.x = ( PSInput.ShadowMap2Pos.x + 1.0f ) / 2.0f;

			// y�͍X�ɏ㉺���]
			DepthTexCoord.y = 1.0f - ( PSInput.ShadowMap2Pos.y + 1.0f ) / 2.0f;

			// �[�x�o�b�t�@�e�N�X�`������[�x���擾
			TextureDepth = g_ShadowMap2Texture.Sample( g_ShadowMap2Sampler, DepthTexCoord );

			// �e�N�X�`���ɋL�^����Ă���[�x( +�␳�l )���y�l���傫�������牜�ɂ���Ƃ������ƂŌ��������ő�ɂ���
			ShadowRate.z = smoothstep( PSInput.ShadowMap2Pos.z - g_ShadowMap.Data[ 2 ].GradationParam, PSInput.ShadowMap2Pos.z, TextureDepth.r + g_ShadowMap.Data[ 2 ].AdjustDepth ) ;
		}
	#else // SHADOWMAP
		ShadowRate.x = 1.0f ;
		ShadowRate.y = 1.0f ;
		ShadowRate.z = 1.0f ;
	#endif // SHADOWMAP


	
// ���C�g0�̏��� +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �J�n )
#if LG0_USE

	#if LG0_DIR

		// ���C�g�̕����v�Z
		lLightDir = g_Common.Light[ 0 ].Direction ;

	#else  // LG0_DIR

		// ���C�g�����x�N�g���̌v�Z
		lLightDir = normalize( PSInput.VPosition.xyz - g_Common.Light[ 0 ].Position ) ;

	#endif  // LG0_DIR
	

	
	// �����E�X�|�b�g���C�g�����l�v�Z =======================================( �J�n )
	#if LG0_SPOT || LG0_POINT

		// ���������v�Z ------------------

		// ���_�ƃ��C�g�ʒu�Ƃ̋����̓������߂�
		lLightTemp = PSInput.VPosition.xyz - g_Common.Light[ 0 ].Position ;
		lLightDistancePow2 = dot( lLightTemp, lLightTemp ) ;

		// �������̌v�Z lLightGen = 1 / ( �����l0 + �����l1 * ���� + �����l2 * ( ���� * ���� ) )
		lLightGen = 1.0f / ( g_Common.Light[ 0 ].Attenuation0 + g_Common.Light[ 0 ].Attenuation1 * sqrt( lLightDistancePow2 ) + g_Common.Light[ 0 ].Attenuation2 * lLightDistancePow2 ) ;

		// --------------------------------


		// �X�|�b�g���C�g�����v�Z --------
		#if LG0_SPOT

			// ���C�g�����x�N�g���ƃ��C�g�ʒu���璸�_�ʒu�ւ̃x�N�g���̓���( ���� Cos a )���v�Z 
			lLightDirectionCosA = dot( lLightDir, g_Common.Light[ 0 ].Direction ) ;

			// �X�|�b�g���C�g�����v�Z  pow( falloff, ( ( Cos a - Cos f ) / ( Cos q - Cos f ) ) )
			lLightGen *= saturate( pow( abs( max( lLightDirectionCosA - g_Common.Light[ 0 ].SpotParam0, 0.0f ) * g_Common.Light[ 0 ].SpotParam1 ), g_Common.Light[ 0 ].FallOff ) ) ;

		#endif // LG0_SPOT
		// --------------------------------


		// �L�������O�������猸�������ő�ɂ��鏈��
		lLightGen *= step( lLightDistancePow2, g_Common.Light[ 0 ].RangePow2 ) ;

	#else // LG0_SPOT || LG0_POINT
	
		lLightGen = 1.0f ;
		
	#endif // LG0_SPOT || LG0_POINT
	
	// �����E�X�|�b�g���C�g�����l�v�Z =======================================( �I�� )

	
	// �e�ɂ�錸�����Z�o
	ShadowGen = max( ShadowRate.x, SHADOWMAP0_ENABLE_LGT0 ) *
	            max( ShadowRate.y, SHADOWMAP1_ENABLE_LGT0 ) *
	            max( ShadowRate.z, SHADOWMAP2_ENABLE_LGT0 ) ;


	// �f�B�t���[�Y�F�v�Z

	// �f�B�t���[�Y�p�x�������v�Z
	DiffuseAngleGen = saturate( dot( Normal, -lLightDir ) ) ;
	
	// �f�B�t���[�Y�J���[�~�ϒl += ( ���C�g�̃f�B�t���[�Y�J���[ * �f�B�t���[�Y�J���[ * �f�B�t���[�Y�J���[�p�x������ + ���C�g�̃A���r�G���g�J���[�ƃ}�e���A���̃A���r�G���g�J���[����Z��������  ) * �����E�X�|�b�g���C�g�̊p�x������
	TotalDiffuse += ( g_Common.Light[ 0 ].Diffuse * PSInput.Diffuse.xyz * DiffuseAngleGen * ShadowGen + g_Common.Light[ 0 ].Ambient.xyz ) * lLightGen ;




	// �X�y�L�����J���[�v�Z
	#ifdef USE_SPE

		// �n�[�t�x�N�g���̌v�Z
		TempF3 = normalize( V_to_Eye - lLightDir ) ;
		
		// Temp = pow( max( 0.0f, N * H ), g_Material.Power.x )
		Temp = pow( max( 0.0f, dot( Normal, TempF3 ) ), g_Common.Material.Power ) ;

		// �X�y�L�����J���[�~�ϒl += Temp * �����E�X�|�b�g���C�g�̊p�x������ * ���C�g�̃X�y�L�����J���[
		TotalSpecular += Temp * lLightGen * g_Common.Light[ 0 ].Specular * ShadowGen ;

	#endif // USE_SPE


#endif // LG0_USE
// ���C�g0�̏��� +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �I�� )









	
// ���C�g1�̏��� +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �J�n )
#if LG1_USE

	#if LG1_DIR

		// ���C�g�̕����v�Z
		lLightDir = g_Common.Light[ 1 ].Direction ;

	#else  // LG1_DIR

		// ���C�g�����x�N�g���̌v�Z
		lLightDir = normalize( PSInput.VPosition.xyz - g_Common.Light[ 1 ].Position ) ;

	#endif  // LG1_DIR
	

	
	// �����E�X�|�b�g���C�g�����l�v�Z =======================================( �J�n )
	#if LG1_SPOT || LG1_POINT

		// ���������v�Z ------------------

		// ���_�ƃ��C�g�ʒu�Ƃ̋����̓������߂�
		lLightTemp = PSInput.VPosition.xyz - g_Common.Light[ 1 ].Position ;
		lLightDistancePow2 = dot( lLightTemp, lLightTemp ) ;

		// �������̌v�Z lLightGen = 1 / ( �����l0 + �����l1 * ���� + �����l2 * ( ���� * ���� ) )
		lLightGen = 1.0f / ( g_Common.Light[ 1 ].Attenuation0 + g_Common.Light[ 1 ].Attenuation1 * sqrt( lLightDistancePow2 ) + g_Common.Light[ 1 ].Attenuation2 * lLightDistancePow2 ) ;

		// --------------------------------


		// �X�|�b�g���C�g�����v�Z --------
		#if LG1_SPOT

			// ���C�g�����x�N�g���ƃ��C�g�ʒu���璸�_�ʒu�ւ̃x�N�g���̓���( ���� Cos a )���v�Z 
			lLightDirectionCosA = dot( lLightDir, g_Common.Light[ 1 ].Direction ) ;

			// �X�|�b�g���C�g�����v�Z  pow( falloff, ( ( Cos a - Cos f ) / ( Cos q - Cos f ) ) )
			lLightGen *= saturate( pow( abs( max( lLightDirectionCosA - g_Common.Light[ 1 ].SpotParam0, 0.0f ) * g_Common.Light[ 1 ].SpotParam1 ), g_Common.Light[ 1 ].FallOff ) ) ;

		#endif // LG1_SPOT
		// --------------------------------


		// �L�������O�������猸�������ő�ɂ��鏈��
		lLightGen *= step( lLightDistancePow2, g_Common.Light[ 1 ].RangePow2 ) ;

	#else // LG1_SPOT || LG1_POINT
	
		lLightGen = 1.0f ;
		
	#endif // LG1_SPOT || LG1_POINT
	
	// �����E�X�|�b�g���C�g�����l�v�Z =======================================( �I�� )

	
	// �e�ɂ�錸�����Z�o
	ShadowGen = max( ShadowRate.x, SHADOWMAP0_ENABLE_LGT1 ) *
	            max( ShadowRate.y, SHADOWMAP1_ENABLE_LGT1 ) *
	            max( ShadowRate.z, SHADOWMAP2_ENABLE_LGT1 ) ;


	// �f�B�t���[�Y�F�v�Z

	// �f�B�t���[�Y�p�x�������v�Z
	DiffuseAngleGen = saturate( dot( Normal, -lLightDir ) ) ;
	
	// �f�B�t���[�Y�J���[�~�ϒl += ( ���C�g�̃f�B�t���[�Y�J���[ * �f�B�t���[�Y�J���[ * �f�B�t���[�Y�J���[�p�x������ + ���C�g�̃A���r�G���g�J���[�ƃ}�e���A���̃A���r�G���g�J���[����Z��������  ) * �����E�X�|�b�g���C�g�̊p�x������
	TotalDiffuse += ( g_Common.Light[ 1 ].Diffuse * PSInput.Diffuse.xyz * DiffuseAngleGen * ShadowGen + g_Common.Light[ 1 ].Ambient.xyz ) * lLightGen ;




	// �X�y�L�����J���[�v�Z
	#ifdef USE_SPE

		// �n�[�t�x�N�g���̌v�Z
		TempF3 = normalize( V_to_Eye - lLightDir ) ;
		
		// Temp = pow( max( 0.0f, N * H ), g_Material.Power.x )
		Temp = pow( max( 0.0f, dot( Normal, TempF3 ) ), g_Common.Material.Power ) ;

		// �X�y�L�����J���[�~�ϒl += Temp * �����E�X�|�b�g���C�g�̊p�x������ * ���C�g�̃X�y�L�����J���[
		TotalSpecular += Temp * lLightGen * g_Common.Light[ 1 ].Specular * ShadowGen ;

	#endif // USE_SPE


#endif // LG1_USE
// ���C�g1�̏��� +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �I�� )









	
// ���C�g2�̏��� +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �J�n )
#if LG2_USE

	#if LG2_DIR

		// ���C�g�̕����v�Z
		lLightDir = g_Common.Light[ 2 ].Direction ;

	#else  // LG2_DIR

		// ���C�g�����x�N�g���̌v�Z
		lLightDir = normalize( PSInput.VPosition.xyz - g_Common.Light[ 2 ].Position ) ;

	#endif  // LG2_DIR
	

	
	// �����E�X�|�b�g���C�g�����l�v�Z =======================================( �J�n )
	#if LG2_SPOT || LG2_POINT

		// ���������v�Z ------------------

		// ���_�ƃ��C�g�ʒu�Ƃ̋����̓������߂�
		lLightTemp = PSInput.VPosition.xyz - g_Common.Light[ 2 ].Position ;
		lLightDistancePow2 = dot( lLightTemp, lLightTemp ) ;

		// �������̌v�Z lLightGen = 1 / ( �����l0 + �����l1 * ���� + �����l2 * ( ���� * ���� ) )
		lLightGen = 1.0f / ( g_Common.Light[ 2 ].Attenuation0 + g_Common.Light[ 2 ].Attenuation1 * sqrt( lLightDistancePow2 ) + g_Common.Light[ 2 ].Attenuation2 * lLightDistancePow2 ) ;

		// --------------------------------


		// �X�|�b�g���C�g�����v�Z --------
		#if LG2_SPOT

			// ���C�g�����x�N�g���ƃ��C�g�ʒu���璸�_�ʒu�ւ̃x�N�g���̓���( ���� Cos a )���v�Z 
			lLightDirectionCosA = dot( lLightDir, g_Common.Light[ 2 ].Direction ) ;

			// �X�|�b�g���C�g�����v�Z  pow( falloff, ( ( Cos a - Cos f ) / ( Cos q - Cos f ) ) )
			lLightGen *= saturate( pow( abs( max( lLightDirectionCosA - g_Common.Light[ 2 ].SpotParam0, 0.0f ) * g_Common.Light[ 2 ].SpotParam1 ), g_Common.Light[ 2 ].FallOff ) ) ;

		#endif // LG2_SPOT
		// --------------------------------


		// �L�������O�������猸�������ő�ɂ��鏈��
		lLightGen *= step( lLightDistancePow2, g_Common.Light[ 2 ].RangePow2 ) ;

	#else // LG2_SPOT || LG2_POINT
	
		lLightGen = 1.0f ;
		
	#endif // LG2_SPOT || LG2_POINT
	
	// �����E�X�|�b�g���C�g�����l�v�Z =======================================( �I�� )

	
	// �e�ɂ�錸�����Z�o
	ShadowGen = max( ShadowRate.x, SHADOWMAP0_ENABLE_LGT2 ) *
	            max( ShadowRate.y, SHADOWMAP1_ENABLE_LGT2 ) *
	            max( ShadowRate.z, SHADOWMAP2_ENABLE_LGT2 ) ;


	// �f�B�t���[�Y�F�v�Z

	// �f�B�t���[�Y�p�x�������v�Z
	DiffuseAngleGen = saturate( dot( Normal, -lLightDir ) ) ;
	
	// �f�B�t���[�Y�J���[�~�ϒl += ( ���C�g�̃f�B�t���[�Y�J���[ * �f�B�t���[�Y�J���[ * �f�B�t���[�Y�J���[�p�x������ + ���C�g�̃A���r�G���g�J���[�ƃ}�e���A���̃A���r�G���g�J���[����Z��������  ) * �����E�X�|�b�g���C�g�̊p�x������
	TotalDiffuse += ( g_Common.Light[ 2 ].Diffuse * PSInput.Diffuse.xyz * DiffuseAngleGen * ShadowGen + g_Common.Light[ 2 ].Ambient.xyz ) * lLightGen ;




	// �X�y�L�����J���[�v�Z
	#ifdef USE_SPE

		// �n�[�t�x�N�g���̌v�Z
		TempF3 = normalize( V_to_Eye - lLightDir ) ;
		
		// Temp = pow( max( 0.0f, N * H ), g_Material.Power.x )
		Temp = pow( max( 0.0f, dot( Normal, TempF3 ) ), g_Common.Material.Power ) ;

		// �X�y�L�����J���[�~�ϒl += Temp * �����E�X�|�b�g���C�g�̊p�x������ * ���C�g�̃X�y�L�����J���[
		TotalSpecular += Temp * lLightGen * g_Common.Light[ 2 ].Specular * ShadowGen ;

	#endif // USE_SPE


#endif // LG2_USE
// ���C�g2�̏��� +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �I�� )









	
// ���C�g3�̏��� +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �J�n )
#if LG3_USE

	#if LG3_DIR

		// ���C�g�̕����v�Z
		lLightDir = g_Common.Light[ 3 ].Direction ;

	#else  // LG3_DIR

		// ���C�g�����x�N�g���̌v�Z
		lLightDir = normalize( PSInput.VPosition.xyz - g_Common.Light[ 3 ].Position ) ;

	#endif  // LG3_DIR
	

	
	// �����E�X�|�b�g���C�g�����l�v�Z =======================================( �J�n )
	#if LG3_SPOT || LG3_POINT

		// ���������v�Z ------------------

		// ���_�ƃ��C�g�ʒu�Ƃ̋����̓������߂�
		lLightTemp = PSInput.VPosition.xyz - g_Common.Light[ 3 ].Position ;
		lLightDistancePow2 = dot( lLightTemp, lLightTemp ) ;

		// �������̌v�Z lLightGen = 1 / ( �����l0 + �����l1 * ���� + �����l2 * ( ���� * ���� ) )
		lLightGen = 1.0f / ( g_Common.Light[ 3 ].Attenuation0 + g_Common.Light[ 3 ].Attenuation1 * sqrt( lLightDistancePow2 ) + g_Common.Light[ 3 ].Attenuation2 * lLightDistancePow2 ) ;

		// --------------------------------


		// �X�|�b�g���C�g�����v�Z --------
		#if LG3_SPOT

			// ���C�g�����x�N�g���ƃ��C�g�ʒu���璸�_�ʒu�ւ̃x�N�g���̓���( ���� Cos a )���v�Z 
			lLightDirectionCosA = dot( lLightDir, g_Common.Light[ 3 ].Direction ) ;

			// �X�|�b�g���C�g�����v�Z  pow( falloff, ( ( Cos a - Cos f ) / ( Cos q - Cos f ) ) )
			lLightGen *= saturate( pow( abs( max( lLightDirectionCosA - g_Common.Light[ 3 ].SpotParam0, 0.0f ) * g_Common.Light[ 3 ].SpotParam1 ), g_Common.Light[ 3 ].FallOff ) ) ;

		#endif // LG3_SPOT
		// --------------------------------


		// �L�������O�������猸�������ő�ɂ��鏈��
		lLightGen *= step( lLightDistancePow2, g_Common.Light[ 3 ].RangePow2 ) ;

	#else // LG3_SPOT || LG3_POINT
	
		lLightGen = 1.0f ;
		
	#endif // LG3_SPOT || LG3_POINT
	
	// �����E�X�|�b�g���C�g�����l�v�Z =======================================( �I�� )

	
	// �e�ɂ�錸�����Z�o
	ShadowGen = max( ShadowRate.x, SHADOWMAP0_ENABLE_LGT3 ) *
	            max( ShadowRate.y, SHADOWMAP1_ENABLE_LGT3 ) *
	            max( ShadowRate.z, SHADOWMAP2_ENABLE_LGT3 ) ;


	// �f�B�t���[�Y�F�v�Z

	// �f�B�t���[�Y�p�x�������v�Z
	DiffuseAngleGen = saturate( dot( Normal, -lLightDir ) ) ;
	
	// �f�B�t���[�Y�J���[�~�ϒl += ( ���C�g�̃f�B�t���[�Y�J���[ * �f�B�t���[�Y�J���[ * �f�B�t���[�Y�J���[�p�x������ + ���C�g�̃A���r�G���g�J���[�ƃ}�e���A���̃A���r�G���g�J���[����Z��������  ) * �����E�X�|�b�g���C�g�̊p�x������
	TotalDiffuse += ( g_Common.Light[ 3 ].Diffuse * PSInput.Diffuse.xyz * DiffuseAngleGen * ShadowGen + g_Common.Light[ 3 ].Ambient.xyz ) * lLightGen ;




	// �X�y�L�����J���[�v�Z
	#ifdef USE_SPE

		// �n�[�t�x�N�g���̌v�Z
		TempF3 = normalize( V_to_Eye - lLightDir ) ;
		
		// Temp = pow( max( 0.0f, N * H ), g_Material.Power.x )
		Temp = pow( max( 0.0f, dot( Normal, TempF3 ) ), g_Common.Material.Power ) ;

		// �X�y�L�����J���[�~�ϒl += Temp * �����E�X�|�b�g���C�g�̊p�x������ * ���C�g�̃X�y�L�����J���[
		TotalSpecular += Temp * lLightGen * g_Common.Light[ 3 ].Specular * ShadowGen ;

	#endif // USE_SPE


#endif // LG3_USE
// ���C�g3�̏��� +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �I�� )









	
// ���C�g4�̏��� +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �J�n )
#if LG4_USE

	#if LG4_DIR

		// ���C�g�̕����v�Z
		lLightDir = g_Common.Light[ 4 ].Direction ;

	#else  // LG4_DIR

		// ���C�g�����x�N�g���̌v�Z
		lLightDir = normalize( PSInput.VPosition.xyz - g_Common.Light[ 4 ].Position ) ;

	#endif  // LG4_DIR
	

	
	// �����E�X�|�b�g���C�g�����l�v�Z =======================================( �J�n )
	#if LG4_SPOT || LG4_POINT

		// ���������v�Z ------------------

		// ���_�ƃ��C�g�ʒu�Ƃ̋����̓������߂�
		lLightTemp = PSInput.VPosition.xyz - g_Common.Light[ 4 ].Position ;
		lLightDistancePow2 = dot( lLightTemp, lLightTemp ) ;

		// �������̌v�Z lLightGen = 1 / ( �����l0 + �����l1 * ���� + �����l2 * ( ���� * ���� ) )
		lLightGen = 1.0f / ( g_Common.Light[ 4 ].Attenuation0 + g_Common.Light[ 4 ].Attenuation1 * sqrt( lLightDistancePow2 ) + g_Common.Light[ 4 ].Attenuation2 * lLightDistancePow2 ) ;

		// --------------------------------


		// �X�|�b�g���C�g�����v�Z --------
		#if LG4_SPOT

			// ���C�g�����x�N�g���ƃ��C�g�ʒu���璸�_�ʒu�ւ̃x�N�g���̓���( ���� Cos a )���v�Z 
			lLightDirectionCosA = dot( lLightDir, g_Common.Light[ 4 ].Direction ) ;

			// �X�|�b�g���C�g�����v�Z  pow( falloff, ( ( Cos a - Cos f ) / ( Cos q - Cos f ) ) )
			lLightGen *= saturate( pow( abs( max( lLightDirectionCosA - g_Common.Light[ 4 ].SpotParam0, 0.0f ) * g_Common.Light[ 4 ].SpotParam1 ), g_Common.Light[ 4 ].FallOff ) ) ;

		#endif // LG4_SPOT
		// --------------------------------


		// �L�������O�������猸�������ő�ɂ��鏈��
		lLightGen *= step( lLightDistancePow2, g_Common.Light[ 4 ].RangePow2 ) ;

	#else // LG4_SPOT || LG4_POINT
	
		lLightGen = 1.0f ;
		
	#endif // LG4_SPOT || LG4_POINT
	
	// �����E�X�|�b�g���C�g�����l�v�Z =======================================( �I�� )

	
	// �e�ɂ�錸�����Z�o
	ShadowGen = max( ShadowRate.x, SHADOWMAP0_ENABLE_LGT4 ) *
	            max( ShadowRate.y, SHADOWMAP1_ENABLE_LGT4 ) *
	            max( ShadowRate.z, SHADOWMAP2_ENABLE_LGT4 ) ;


	// �f�B�t���[�Y�F�v�Z

	// �f�B�t���[�Y�p�x�������v�Z
	DiffuseAngleGen = saturate( dot( Normal, -lLightDir ) ) ;
	
	// �f�B�t���[�Y�J���[�~�ϒl += ( ���C�g�̃f�B�t���[�Y�J���[ * �f�B�t���[�Y�J���[ * �f�B�t���[�Y�J���[�p�x������ + ���C�g�̃A���r�G���g�J���[�ƃ}�e���A���̃A���r�G���g�J���[����Z��������  ) * �����E�X�|�b�g���C�g�̊p�x������
	TotalDiffuse += ( g_Common.Light[ 4 ].Diffuse * PSInput.Diffuse.xyz * DiffuseAngleGen * ShadowGen + g_Common.Light[ 4 ].Ambient.xyz ) * lLightGen ;




	// �X�y�L�����J���[�v�Z
	#ifdef USE_SPE

		// �n�[�t�x�N�g���̌v�Z
		TempF3 = normalize( V_to_Eye - lLightDir ) ;
		
		// Temp = pow( max( 0.0f, N * H ), g_Material.Power.x )
		Temp = pow( max( 0.0f, dot( Normal, TempF3 ) ), g_Common.Material.Power ) ;

		// �X�y�L�����J���[�~�ϒl += Temp * �����E�X�|�b�g���C�g�̊p�x������ * ���C�g�̃X�y�L�����J���[
		TotalSpecular += Temp * lLightGen * g_Common.Light[ 4 ].Specular * ShadowGen ;

	#endif // USE_SPE


#endif // LG4_USE
// ���C�g4�̏��� +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �I�� )









	
// ���C�g5�̏��� +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �J�n )
#if LG5_USE

	#if LG5_DIR

		// ���C�g�̕����v�Z
		lLightDir = g_Common.Light[ 5 ].Direction ;

	#else  // LG5_DIR

		// ���C�g�����x�N�g���̌v�Z
		lLightDir = normalize( PSInput.VPosition.xyz - g_Common.Light[ 5 ].Position ) ;

	#endif  // LG5_DIR
	

	
	// �����E�X�|�b�g���C�g�����l�v�Z =======================================( �J�n )
	#if LG5_SPOT || LG5_POINT

		// ���������v�Z ------------------

		// ���_�ƃ��C�g�ʒu�Ƃ̋����̓������߂�
		lLightTemp = PSInput.VPosition.xyz - g_Common.Light[ 5 ].Position ;
		lLightDistancePow2 = dot( lLightTemp, lLightTemp ) ;

		// �������̌v�Z lLightGen = 1 / ( �����l0 + �����l1 * ���� + �����l2 * ( ���� * ���� ) )
		lLightGen = 1.0f / ( g_Common.Light[ 5 ].Attenuation0 + g_Common.Light[ 5 ].Attenuation1 * sqrt( lLightDistancePow2 ) + g_Common.Light[ 5 ].Attenuation2 * lLightDistancePow2 ) ;

		// --------------------------------


		// �X�|�b�g���C�g�����v�Z --------
		#if LG5_SPOT

			// ���C�g�����x�N�g���ƃ��C�g�ʒu���璸�_�ʒu�ւ̃x�N�g���̓���( ���� Cos a )���v�Z 
			lLightDirectionCosA = dot( lLightDir, g_Common.Light[ 5 ].Direction ) ;

			// �X�|�b�g���C�g�����v�Z  pow( falloff, ( ( Cos a - Cos f ) / ( Cos q - Cos f ) ) )
			lLightGen *= saturate( pow( abs( max( lLightDirectionCosA - g_Common.Light[ 5 ].SpotParam0, 0.0f ) * g_Common.Light[ 5 ].SpotParam1 ), g_Common.Light[ 5 ].FallOff ) ) ;

		#endif // LG5_SPOT
		// --------------------------------


		// �L�������O�������猸�������ő�ɂ��鏈��
		lLightGen *= step( lLightDistancePow2, g_Common.Light[ 5 ].RangePow2 ) ;

	#else // LG5_SPOT || LG5_POINT
	
		lLightGen = 1.0f ;
		
	#endif // LG5_SPOT || LG5_POINT
	
	// �����E�X�|�b�g���C�g�����l�v�Z =======================================( �I�� )

	
	// �e�ɂ�錸�����Z�o
	ShadowGen = max( ShadowRate.x, SHADOWMAP0_ENABLE_LGT5 ) *
	            max( ShadowRate.y, SHADOWMAP1_ENABLE_LGT5 ) *
	            max( ShadowRate.z, SHADOWMAP2_ENABLE_LGT5 ) ;


	// �f�B�t���[�Y�F�v�Z

	// �f�B�t���[�Y�p�x�������v�Z
	DiffuseAngleGen = saturate( dot( Normal, -lLightDir ) ) ;
	
	// �f�B�t���[�Y�J���[�~�ϒl += ( ���C�g�̃f�B�t���[�Y�J���[ * �f�B�t���[�Y�J���[ * �f�B�t���[�Y�J���[�p�x������ + ���C�g�̃A���r�G���g�J���[�ƃ}�e���A���̃A���r�G���g�J���[����Z��������  ) * �����E�X�|�b�g���C�g�̊p�x������
	TotalDiffuse += ( g_Common.Light[ 5 ].Diffuse * PSInput.Diffuse.xyz * DiffuseAngleGen * ShadowGen + g_Common.Light[ 5 ].Ambient.xyz ) * lLightGen ;




	// �X�y�L�����J���[�v�Z
	#ifdef USE_SPE

		// �n�[�t�x�N�g���̌v�Z
		TempF3 = normalize( V_to_Eye - lLightDir ) ;
		
		// Temp = pow( max( 0.0f, N * H ), g_Material.Power.x )
		Temp = pow( max( 0.0f, dot( Normal, TempF3 ) ), g_Common.Material.Power ) ;

		// �X�y�L�����J���[�~�ϒl += Temp * �����E�X�|�b�g���C�g�̊p�x������ * ���C�g�̃X�y�L�����J���[
		TotalSpecular += Temp * lLightGen * g_Common.Light[ 5 ].Specular * ShadowGen ;

	#endif // USE_SPE


#endif // LG5_USE
// ���C�g5�̏��� +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �I�� )















	// �o�̓J���[�v�Z +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �J�n )

	// TotalDiffuse = ���C�g�f�B�t���[�Y�J���[�~�ϒl + ( �}�e���A���̃A���r�G���g�J���[�ƃO���[�o���A���r�G���g�J���[����Z�������̂ƃ}�e���A���G�~�b�V�u�J���[�����Z�������� )
	TotalDiffuse += g_Common.Material.Ambient_Emissive.rgb ;

	#ifdef USE_SPE
		#ifdef USE_SPETEX
			// TextureSpecularColor = ���C�g�̃X�y�L�����J���[�~�ϒl * �X�y�L�����J���[ * �X�y�L�����e�N�X�`���J���[
			TextureSpecularColor.xyz = g_SpecularMapTexture.Sample( g_SpecularMapSampler, PSInput.TexCoords0_1.xy ).xyz * TotalSpecular * PSInput.Specular.xyz ;
		#else  // USE_SPETEX
			// TextureSpecularColor = ���C�g�̃X�y�L�����J���[�~�ϒl * �X�y�L�����J���[
			TextureSpecularColor.xyz = TotalSpecular * PSInput.Specular.xyz ;
		#endif // USE_SPETEX

		// �o�̓J���[ = TotalDiffuse * �e�N�X�`���J���[ + TextureSpecularColor

		PSOutput.Color0.rgb = TextureDiffuseColor.rgb * TotalDiffuse + TextureSpecularColor.rgb ;
		PSOutput.Color0.a = TextureDiffuseColor.a * g_Base.FactorColor.a * PSInput.Diffuse.a ;

	#else  // USE_SPE
		// �o�̓J���[ = TotalDiffuse * �e�N�X�`���J���[
		PSOutput.Color0.rgb = TotalDiffuse * TextureDiffuseColor.rgb ;

		// �o�̓� = �e�N�X�`���� * �f�B�t���[�Y�� * ��惿
		PSOutput.Color0.a = TextureDiffuseColor.a * g_Base.FactorColor.a * PSInput.Diffuse.a ;

	#endif // USE_SPE

	// �o�̓J���[�v�Z +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �I�� )


	// ���C�g���g���ꍇ *******************************************************************( �I�� )

#ifdef PRE_MUL_ALPHA
	PSOutput.Color0.rgb *= PSInput.Diffuse.a ;
#endif

#ifdef MUL_X_4
	PSOutput.Color0.rgb *= 4.0f ;
#endif

#ifdef MUL_BLEND
	PSOutput.Color0 = lerp( 1.0f, PSOutput.Color0, PSOutput.Color0.a ) ;
#endif

	// �A���t�@�e�X�g
	AlphaTest( PSOutput.Color0.a ) ;

	// �t�H�O����
	PSOutput.Color0.rgb = lerp( g_Common.Fog.Color.rgb, PSOutput.Color0.rgb, saturate( PSInput.Fog.x ) );

	return PSOutput ;
}

