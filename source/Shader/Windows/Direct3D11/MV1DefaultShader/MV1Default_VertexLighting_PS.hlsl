#include "../PixelShader.h"


// �s�N�Z���V�F�[�_�[�̓���
struct PS_INPUT
{
	float4 Diffuse         : COLOR0 ;		// �f�B�t���[�Y�J���[
	float4 Specular        : COLOR1 ;		// �X�y�L�����J���[
	float4 TexCoords0_1    : TEXCOORD0 ;	// �e�N�X�`�����W�ƃT�u�e�N�X�`�����W( x:�e�N�X�`�����Wx  y:�e�N�X�`�����Wy  z:�T�u�e�N�X�`�����Wx  w:�T�u�e�N�X�`�����Wy )

	float3 V_to_Eye        : TEXCOORD1 ;	// ���_���W���王���ւ̃x�N�g��( �r���[��� )
	float4 Normal_Fog      : TEXCOORD2 ;	// x.y.z:�@��( �r���[��� )  w:�t�H�O�p�����[�^

#if LG0_USE
	float4 Light0_Dir_Gen  : TEXCOORD3 ;	// xyz:���C�g�O�̋t�x�N�g��( �r���[��� ) w:���C�g�O�������
#endif
#if LG1_USE
	float4 Light1_Dir_Gen  : TEXCOORD4 ;	// xyz:���C�g�P�̋t�x�N�g��( �r���[��� ) w:���C�g�P�������
#endif
#if LG2_USE
	float4 Light2_Dir_Gen  : TEXCOORD5 ;	// xyz:���C�g�Q�̋t�x�N�g��( �r���[��� ) w:���C�g�Q�������
#endif

#if SHADOWMAP || SHADOWMAP_DRAW
	float3 ShadowMap0Pos   : TEXCOORD6 ;	// �V���h�E�}�b�v�O�̃��C�g���W( x, y, z )
#endif
#if SHADOWMAP
	float3 ShadowMap1Pos   : TEXCOORD7 ;	// �V���h�E�}�b�v�P�̃��C�g���W( x, y, z )
	float3 ShadowMap2Pos   : TEXCOORD8 ;	// �V���h�E�}�b�v�Q�̃��C�g���W( x, y, z )
#endif
} ;

// �s�N�Z���V�F�[�_�[�̏o��
struct PS_OUTPUT
{
	float4 Color0           : SV_TARGET0 ;	// �F
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

#if TOON
SamplerState g_ToonDiffuseGradSampler       : register( s3 ) ;		// �g�D�[�������_�����O�p�f�B�t���[�Y�J���[�O���f�[�V�����e�N�X�`��
Texture2D    g_ToonDiffuseGradTexture       : register( t3 ) ;		// �g�D�[�������_�����O�p�f�B�t���[�Y�J���[�O���f�[�V�����e�N�X�`��

SamplerState g_ToonSpecularGradSampler      : register( s4 ) ;		// �g�D�[�������_�����O�p�X�y�L�����J���[�O���f�[�V�����e�N�X�`��
Texture2D    g_ToonSpecularGradTexture      : register( t4 ) ;		// �g�D�[�������_�����O�p�X�y�L�����J���[�O���f�[�V�����e�N�X�`��

#if TOON_SPHEREOP_MUL || TOON_SPHEREOP_ADD
SamplerState g_ToonSphereMapSampler         : register( s5 ) ;		// �g�D�[�������_�����O�p�X�t�B�A�}�b�v�e�N�X�`��
Texture2D    g_ToonSphereMapTexture         : register( t5 ) ;		// �g�D�[�������_�����O�p�X�t�B�A�}�b�v�e�N�X�`��
#endif // TOON_SPHEREOP_MUL || TOON_SPHEREOP_ADD
SamplerState g_ToonRGBtoVMaxRGBVolumeSampler: register( s6 ) ;		// �g�D�[�������_�����O�pRGB�P�x�O�a�����p�{�����[���e�N�X�`��
Texture3D    g_ToonRGBtoVMaxRGBVolumeTexture: register( t6 ) ;		// �g�D�[�������_�����O�pRGB�P�x�O�a�����p�{�����[���e�N�X�`��
#endif // TOON

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




// main�֐�
PS_OUTPUT main( PS_INPUT PSInput )
{
	PS_OUTPUT	PSOutput ;
	float4		TextureDiffuseColor ;
	float4		TextureSpecularColor ;
	float4		ToonSpecularGradColor ;
	float4		ToonDiffuseGradColor ;
	float4		ToonSphereMapColor ;
	float4		ToonRGBtoVMaxRGBVolumeColor ;
	float3		Normal ;
	float3		TotalDiffuse ;
	float3		TotalSpecular ;
	float3		TotalLightDiffuse ;
	float3		TotalAmbient ;
	float		TotalAngleGen ;
	float		DiffuseAngleGen ;
	float3		TempF3 ;
	float		Temp ;
	float3		ShadowRate ;
	float		ShadowGen ;
	float		ParamAlpha;

#if SHADOWMAP
	float2		DepthTexCoord ;
	float4		TextureDepth ;
//	float4		variance_md_p_pmax ;
#endif

	ParamAlpha = PSInput.Diffuse.a * g_Base.FactorColor.a;

#if SUBTEXTUREMODE != 0
	float4 SubTextureColor;
#endif  // SUBTEXTUREMODE != 0

	TextureDiffuseColor =  g_DiffuseMapTexture.Sample( g_DiffuseMapSampler, PSInput.TexCoords0_1.xy ) ;

#ifdef USE_SPE
	#ifdef USE_SPETEX
	TextureSpecularColor = g_SpecularMapTexture.Sample( g_SpecularMapSampler, PSInput.TexCoords0_1.xy ) ;
	#endif // USE_SPETEX
#endif // USE_SPE

#if SUBTEXTUREMODE != 0
	SubTextureColor = g_SubTexture.Sample( g_SubSampler, PSInput.TexCoords0_1.zw ) ;
#endif  // SUBTEXTUREMODE != 0

#if SUBTEXTUREMODE == 1  // ���u�����h( MV1_LAYERBLEND_TYPE_TRANSLUCENT + 1 )
	TextureDiffuseColor.rgb = lerp( TextureDiffuseColor.rgb, SubTextureColor.rgb, SubTextureColor.a ) ;
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

	// ���C�g���g��Ȃ��ꍇ�̓J���[�����̂܂܏o�� *****************************************( �J�n )
	#ifndef LG_USE

		// �V���h�E�}�b�v�ւ̕`�� *********************************************************( �J�n )
		#if SHADOWMAP_DRAW
			#if TOON
				#if TOON == 1
					// �o�̓� = �e�N�X�`���� * �f�B�t���[�Y�� * ��惿
					PSOutput.Color0.a = TextureDiffuseColor.a * ParamAlpha ;

				#else	// TOON == 1
					#if TOON_SPHEREOP_MUL
						Normal = normalize( PSInput.Normal_Fog.xyz ) ;
					
						// �X�t�B�A�}�b�v�̐F���擾
						TempF3 = Normal * float3( 0.5f, -0.5f, 0.0f ) + 0.5f ;
						ToonSphereMapColor = g_ToonSphereMapTexture.Sample( g_ToonSphereMapSampler, TempF3.xy ) ;

						PSOutput.Color0.a = TextureDiffuseColor.a * ParamAlpha * ToonSphereMapColor.a ;

					#else // TOON_SPHEREOP_MUL

						PSOutput.Color0.a = TextureDiffuseColor.a * ParamAlpha ;

					#endif // TOON_SPHEREOP_MUL

				#endif		// TOON == 1
			#else  // TOON
			
				// �o�̓� = �e�N�X�`���� * �f�B�t���[�Y�� * ��惿
				PSOutput.Color0.a = TextureDiffuseColor.a * ParamAlpha ;
				
			#endif // TOON
			
			// �y�l��F�Ƃ��ďo��
			PSOutput.Color0.r = PSInput.ShadowMap0Pos.z;
			PSOutput.Color0.g = 0.0f ;
			PSOutput.Color0.b = 0.0f ;
		
		// �V���h�E�}�b�v�ւ̕`�� *********************************************************( �I�� )
		#else // SHADOWMAP_DRAW
		
			#ifdef USE_SPE

				#ifdef USE_SPETEX

					PSOutput.Color0 = PSInput.Specular * TextureSpecularColor + PSInput.Diffuse * TextureDiffuseColor ;

				#else  // USE_SPETEX

					PSOutput.Color0 = PSInput.Specular + PSInput.Diffuse * TextureDiffuseColor ;

				#endif // USE_SPETEX

			#else  // USE_SPE

				PSOutput.Color0 = PSInput.Diffuse * TextureDiffuseColor ;

			#endif // USE_SPE

			PSOutput.Color0.a = TextureDiffuseColor.a * ParamAlpha ;
			
		#endif // SHADOWMAP_DRAW

	// ���C�g���g��Ȃ��ꍇ�̓J���[�����̂܂܏o�� *****************************************( �I�� )
	#else // LG_USE
	// ���C�g���g���ꍇ *******************************************************************( �J�n )

		// �o���v�}�b�v or �t�H���V�F�[�f�B���O�̏ꍇ =========================================( �J�n )
		#if BUMPMAP || PHONG

			#if BUMPMAP

				// �@���� 0�`1 �̒l�� -1.0�`1.0 �ɕϊ�����
				Normal = ( g_NormalMapTexture.Sample( g_NormalMapSampler, PSInput.TexCoords0_1.xy ).rgb - 0.5f ) * 2.0f ;

			#else // BUMPMAP

				Normal = PSInput.Normal_Fog.xyz ;

			#endif // BUMPMAP


			#ifdef    PHONG
				Normal = normalize( Normal ) ;
			#endif

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
					// PSInput.ShadowMap1Pos.xy �� -1.0f �` 1.0f �̒l�Ȃ̂ŁA����� 0.0f �` 1.0f �̒l�ɂ���
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

			// �f�B�t���[�Y�J���[�ƃX�y�L�����J���[�̒~�ϒl��������
			TotalDiffuse  = 0.0f ;
			TotalSpecular = 0.0f ;
			#if TOON
				#if TOON == 1
					TotalLightDiffuse = 0.0f ;
				#else // TOON == 1
					TotalAmbient = 0.0f ;
					TotalAngleGen = 0.0f ;
				#endif
			#endif // TOON




			
// ���C�g0�̏��� +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �J�n )
#if LG0_USE

	// �f�B�t���[�Y�F�v�Z

	// DiffuseAngleGen = �f�B�t���[�Y�p�x�������v�Z
	DiffuseAngleGen = saturate( dot( Normal, PSInput.Light0_Dir_Gen.xyz ) ) ;
	
	// �e�ɂ�錸�����Z�o
	ShadowGen = max( ShadowRate.x, g_ShadowMap.Data[ 0 ].Enable_Light0 ) *
	            max( ShadowRate.y, g_ShadowMap.Data[ 1 ].Enable_Light0 ) *
	            max( ShadowRate.z, g_ShadowMap.Data[ 2 ].Enable_Light0 ) ;

	#if LG0_DIR

		#if TOON
			#if TOON == 1
				// �g�D�[���̏ꍇ�͂��̎��_�ł͒��_or�}�e���A���̃f�B�t���[�Y�J���[�͎g�p���Ȃ�
				// �f�B�t���[�Y�J���[�~�ϒl += ���C�g�̃f�B�t���[�Y�J���[ * �f�B�t���[�Y�J���[�p�x������ + ���C�g�̃A���r�G���g�J���[�ƃ}�e���A���̃A���r�G���g�J���[����Z�������� 
				TempF3             = g_Common.Light[ 0 ].Diffuse * DiffuseAngleGen * ShadowGen ;
				TotalLightDiffuse += TempF3 ;
				TotalDiffuse      += TempF3 + g_Common.Light[ 0 ].Ambient.xyz ;

			#else // TOON == 1
				// �g�D�[���Q�̏ꍇ�̓f�B�t���[�Y�J���[�ƃ��C�g�̊p�x�����͕ʁX�ɕۑ����Ă���
				// �f�B�t���[�Y�J���[�~�ϒl   += ���C�g�̃f�B�t���[�Y�J���[
				// ���C�g�̊p�x�������̒~�ϒl += ���C�g�̊p�x������
				// ���C�g�̃A���r�G���g�J���[�̒~�ϒl += ���C�g�̃A���r�G���g�J���[
				TotalDiffuse  += g_Common.Light[ 0 ].Diffuse ;

				TotalAmbient  += g_Common.Light[ 0 ].Ambient.rgb ;
				TotalAngleGen += DiffuseAngleGen * ShadowGen ;

			#endif // TOON == 1
		#else // TOON
			// �f�B�t���[�Y�J���[�~�ϒl += ���C�g�̃f�B�t���[�Y�J���[ * �f�B�t���[�Y�J���[ * �f�B�t���[�Y�J���[�p�x������ + ���C�g�̃A���r�G���g�J���[�ƃ}�e���A���̃A���r�G���g�J���[����Z�������� 
			TotalDiffuse += g_Common.Light[ 0 ].Diffuse * PSInput.Diffuse.xyz * DiffuseAngleGen * ShadowGen + g_Common.Light[ 0 ].Ambient.xyz ;

		#endif //TOON

	#else // LG0_DIR

		#if TOON
			#if TOON == 1
				// �g�D�[���̏ꍇ�͂��̎��_�ł͒��_or�}�e���A���̃f�B�t���[�Y�J���[�͎g�p���Ȃ�
				// �f�B�t���[�Y�J���[�~�ϒl += ( ���C�g�̃f�B�t���[�Y�J���[ * �f�B�t���[�Y�J���[�p�x������ + ���C�g�̃A���r�G���g�J���[�ƃ}�e���A���̃A���r�G���g�J���[����Z��������  ) * �����E�X�|�b�g���C�g�̊p�x������
				TempF3             = g_Common.Light[ 0 ].Diffuse * DiffuseAngleGen * ShadowGen ;
				TotalLightDiffuse += TempF3 * PSInput.Light0_Dir_Gen.w ;
				TotalDiffuse      += ( TempF3 + g_Common.Light[ 0 ].Ambient.xyz ) * PSInput.Light0_Dir_Gen.w ;

			#else // TOON == 1
				// �g�D�[���Q�̏ꍇ�̓f�B�t���[�Y�J���[�ƃ��C�g�̊p�x�����͕ʁX�ɕۑ����Ă���
				// �f�B�t���[�Y�J���[�~�ϒl           += ���C�g�̃f�B�t���[�Y�J���[
				// ���C�g�̃A���r�G���g�J���[�̒~�ϒl += ���C�g�̃A���r�G���g�J���[
				// ���C�g�̊p�x�������̒~�ϒl         += ���C�g�̊p�x������
				TotalDiffuse  += g_Common.Light[ 0 ].Diffuse ;

				TotalAmbient  += g_Common.Light[ 0 ].Ambient.rgb ;
				TotalAngleGen += DiffuseAngleGen * ShadowGen * PSInput.Light0_Dir_Gen.w ;

			#endif // TOON == 1
		#else // TOON
			// �f�B�t���[�Y�J���[�~�ϒl += ( ���C�g�̃f�B�t���[�Y�J���[ * �f�B�t���[�Y�J���[ * �f�B�t���[�Y�J���[�p�x������ + ���C�g�̃A���r�G���g�J���[�ƃ}�e���A���̃A���r�G���g�J���[����Z��������  ) * �����E�X�|�b�g���C�g�̊p�x������
			TotalDiffuse += ( g_Common.Light[ 0 ].Diffuse * PSInput.Diffuse.xyz * DiffuseAngleGen * ShadowGen + g_Common.Light[ 0 ].Ambient.xyz ) * PSInput.Light0_Dir_Gen.w ;
			
		#endif // TOON

	#endif // LG0_DIR


	// �X�y�L�����J���[�v�Z
	#ifdef USE_SPE

		// �n�[�t�x�N�g���̌v�Z
		TempF3 = PSInput.V_to_Eye + PSInput.Light0_Dir_Gen.xyz ;
		#ifdef    PHONG
			TempF3 = normalize( TempF3 ) ;
		#else  // PHONG
			TempF3 *= 0.5f ;
		#endif // PHONG

		// Temp = pow( max( 0.0f, N * H ), g_Common.Material.Power )
		Temp = pow( max( 0.0f, dot( Normal, TempF3 ) ), g_Common.Material.Power ) ;

		#if LG0_DIR

			// �X�y�L�����J���[�~�ϒl += Temp * ���C�g�̃X�y�L�����J���[
			TotalSpecular += Temp * g_Common.Light[ 0 ].Specular * ShadowGen ;

		#else // LG0_DIR

			// �X�y�L�����J���[�~�ϒl += Temp * �����E�X�|�b�g���C�g�̊p�x������ * ���C�g�̃X�y�L�����J���[
			TotalSpecular += Temp * PSInput.Light0_Dir_Gen.w * g_Common.Light[ 0 ].Specular * ShadowGen ;

		#endif // LG0_DIR

	#endif // USE_SPE

#endif // LG0_USE
// ���C�g0�̏��� +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �I�� )









			
// ���C�g1�̏��� +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �J�n )
#if LG1_USE

	// �f�B�t���[�Y�F�v�Z

	// DiffuseAngleGen = �f�B�t���[�Y�p�x�������v�Z
	DiffuseAngleGen = saturate( dot( Normal, PSInput.Light1_Dir_Gen.xyz ) ) ;
	
	// �e�ɂ�錸�����Z�o
	ShadowGen = max( ShadowRate.x, g_ShadowMap.Data[ 0 ].Enable_Light1 ) *
	            max( ShadowRate.y, g_ShadowMap.Data[ 1 ].Enable_Light1 ) *
	            max( ShadowRate.z, g_ShadowMap.Data[ 2 ].Enable_Light1 ) ;

	#if LG1_DIR

		#if TOON
			#if TOON == 1
				// �g�D�[���̏ꍇ�͂��̎��_�ł͒��_or�}�e���A���̃f�B�t���[�Y�J���[�͎g�p���Ȃ�
				// �f�B�t���[�Y�J���[�~�ϒl += ���C�g�̃f�B�t���[�Y�J���[ * �f�B�t���[�Y�J���[�p�x������ + ���C�g�̃A���r�G���g�J���[�ƃ}�e���A���̃A���r�G���g�J���[����Z�������� 
				TempF3             = g_Common.Light[ 1 ].Diffuse * DiffuseAngleGen * ShadowGen ;
				TotalLightDiffuse += TempF3 ;
				TotalDiffuse      += TempF3 + g_Common.Light[ 1 ].Ambient.xyz ;

			#else // TOON == 1
				// �g�D�[���Q�̏ꍇ�̓f�B�t���[�Y�J���[�ƃ��C�g�̊p�x�����͕ʁX�ɕۑ����Ă���
				// �f�B�t���[�Y�J���[�~�ϒl   += ���C�g�̃f�B�t���[�Y�J���[
				// ���C�g�̊p�x�������̒~�ϒl += ���C�g�̊p�x������
				// ���C�g�̃A���r�G���g�J���[�̒~�ϒl += ���C�g�̃A���r�G���g�J���[
				TotalDiffuse  += g_Common.Light[ 1 ].Diffuse ;

				TotalAmbient  += g_Common.Light[ 1 ].Ambient.rgb ;
				TotalAngleGen += DiffuseAngleGen * ShadowGen ;

			#endif // TOON == 1
		#else // TOON
			// �f�B�t���[�Y�J���[�~�ϒl += ���C�g�̃f�B�t���[�Y�J���[ * �f�B�t���[�Y�J���[ * �f�B�t���[�Y�J���[�p�x������ + ���C�g�̃A���r�G���g�J���[�ƃ}�e���A���̃A���r�G���g�J���[����Z�������� 
			TotalDiffuse += g_Common.Light[ 1 ].Diffuse * PSInput.Diffuse.xyz * DiffuseAngleGen * ShadowGen + g_Common.Light[ 1 ].Ambient.xyz ;

		#endif //TOON

	#else // LG1_DIR

		#if TOON
			#if TOON == 1
				// �g�D�[���̏ꍇ�͂��̎��_�ł͒��_or�}�e���A���̃f�B�t���[�Y�J���[�͎g�p���Ȃ�
				// �f�B�t���[�Y�J���[�~�ϒl += ( ���C�g�̃f�B�t���[�Y�J���[ * �f�B�t���[�Y�J���[�p�x������ + ���C�g�̃A���r�G���g�J���[�ƃ}�e���A���̃A���r�G���g�J���[����Z��������  ) * �����E�X�|�b�g���C�g�̊p�x������
				TempF3             = g_Common.Light[ 1 ].Diffuse * DiffuseAngleGen * ShadowGen ;
				TotalLightDiffuse += TempF3 * PSInput.Light1_Dir_Gen.w ;
				TotalDiffuse      += ( TempF3 + g_Common.Light[ 1 ].Ambient.xyz ) * PSInput.Light1_Dir_Gen.w ;

			#else // TOON == 1
				// �g�D�[���Q�̏ꍇ�̓f�B�t���[�Y�J���[�ƃ��C�g�̊p�x�����͕ʁX�ɕۑ����Ă���
				// �f�B�t���[�Y�J���[�~�ϒl           += ���C�g�̃f�B�t���[�Y�J���[
				// ���C�g�̃A���r�G���g�J���[�̒~�ϒl += ���C�g�̃A���r�G���g�J���[
				// ���C�g�̊p�x�������̒~�ϒl         += ���C�g�̊p�x������
				TotalDiffuse  += g_Common.Light[ 1 ].Diffuse ;

				TotalAmbient  += g_Common.Light[ 1 ].Ambient.rgb ;
				TotalAngleGen += DiffuseAngleGen * ShadowGen * PSInput.Light1_Dir_Gen.w ;

			#endif // TOON == 1
		#else // TOON
			// �f�B�t���[�Y�J���[�~�ϒl += ( ���C�g�̃f�B�t���[�Y�J���[ * �f�B�t���[�Y�J���[ * �f�B�t���[�Y�J���[�p�x������ + ���C�g�̃A���r�G���g�J���[�ƃ}�e���A���̃A���r�G���g�J���[����Z��������  ) * �����E�X�|�b�g���C�g�̊p�x������
			TotalDiffuse += ( g_Common.Light[ 1 ].Diffuse * PSInput.Diffuse.xyz * DiffuseAngleGen * ShadowGen + g_Common.Light[ 1 ].Ambient.xyz ) * PSInput.Light1_Dir_Gen.w ;
			
		#endif // TOON

	#endif // LG1_DIR


	// �X�y�L�����J���[�v�Z
	#ifdef USE_SPE

		// �n�[�t�x�N�g���̌v�Z
		TempF3 = PSInput.V_to_Eye + PSInput.Light1_Dir_Gen.xyz ;
		#ifdef    PHONG
			TempF3 = normalize( TempF3 ) ;
		#else  // PHONG
			TempF3 *= 0.5f ;
		#endif // PHONG

		// Temp = pow( max( 0.0f, N * H ), g_Common.Material.Power )
		Temp = pow( max( 0.0f, dot( Normal, TempF3 ) ), g_Common.Material.Power ) ;

		#if LG1_DIR

			// �X�y�L�����J���[�~�ϒl += Temp * ���C�g�̃X�y�L�����J���[
			TotalSpecular += Temp * g_Common.Light[ 1 ].Specular * ShadowGen ;

		#else // LG1_DIR

			// �X�y�L�����J���[�~�ϒl += Temp * �����E�X�|�b�g���C�g�̊p�x������ * ���C�g�̃X�y�L�����J���[
			TotalSpecular += Temp * PSInput.Light1_Dir_Gen.w * g_Common.Light[ 1 ].Specular * ShadowGen ;

		#endif // LG1_DIR

	#endif // USE_SPE

#endif // LG1_USE
// ���C�g1�̏��� +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �I�� )









			
// ���C�g2�̏��� +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �J�n )
#if LG2_USE

	// �f�B�t���[�Y�F�v�Z

	// DiffuseAngleGen = �f�B�t���[�Y�p�x�������v�Z
	DiffuseAngleGen = saturate( dot( Normal, PSInput.Light2_Dir_Gen.xyz ) ) ;
	
	// �e�ɂ�錸�����Z�o
	ShadowGen = max( ShadowRate.x, g_ShadowMap.Data[ 0 ].Enable_Light2 ) *
	            max( ShadowRate.y, g_ShadowMap.Data[ 1 ].Enable_Light2 ) *
	            max( ShadowRate.z, g_ShadowMap.Data[ 2 ].Enable_Light2 ) ;

	#if LG2_DIR

		#if TOON
			#if TOON == 1
				// �g�D�[���̏ꍇ�͂��̎��_�ł͒��_or�}�e���A���̃f�B�t���[�Y�J���[�͎g�p���Ȃ�
				// �f�B�t���[�Y�J���[�~�ϒl += ���C�g�̃f�B�t���[�Y�J���[ * �f�B�t���[�Y�J���[�p�x������ + ���C�g�̃A���r�G���g�J���[�ƃ}�e���A���̃A���r�G���g�J���[����Z�������� 
				TempF3             = g_Common.Light[ 2 ].Diffuse * DiffuseAngleGen * ShadowGen ;
				TotalLightDiffuse += TempF3 ;
				TotalDiffuse      += TempF3 + g_Common.Light[ 2 ].Ambient.xyz ;

			#else // TOON == 1
				// �g�D�[���Q�̏ꍇ�̓f�B�t���[�Y�J���[�ƃ��C�g�̊p�x�����͕ʁX�ɕۑ����Ă���
				// �f�B�t���[�Y�J���[�~�ϒl   += ���C�g�̃f�B�t���[�Y�J���[
				// ���C�g�̊p�x�������̒~�ϒl += ���C�g�̊p�x������
				// ���C�g�̃A���r�G���g�J���[�̒~�ϒl += ���C�g�̃A���r�G���g�J���[
				TotalDiffuse  += g_Common.Light[ 2 ].Diffuse ;

				TotalAmbient  += g_Common.Light[ 2 ].Ambient.rgb ;
				TotalAngleGen += DiffuseAngleGen * ShadowGen ;

			#endif // TOON == 1
		#else // TOON
			// �f�B�t���[�Y�J���[�~�ϒl += ���C�g�̃f�B�t���[�Y�J���[ * �f�B�t���[�Y�J���[ * �f�B�t���[�Y�J���[�p�x������ + ���C�g�̃A���r�G���g�J���[�ƃ}�e���A���̃A���r�G���g�J���[����Z�������� 
			TotalDiffuse += g_Common.Light[ 2 ].Diffuse * PSInput.Diffuse.xyz * DiffuseAngleGen * ShadowGen + g_Common.Light[ 2 ].Ambient.xyz ;

		#endif //TOON

	#else // LG2_DIR

		#if TOON
			#if TOON == 1
				// �g�D�[���̏ꍇ�͂��̎��_�ł͒��_or�}�e���A���̃f�B�t���[�Y�J���[�͎g�p���Ȃ�
				// �f�B�t���[�Y�J���[�~�ϒl += ( ���C�g�̃f�B�t���[�Y�J���[ * �f�B�t���[�Y�J���[�p�x������ + ���C�g�̃A���r�G���g�J���[�ƃ}�e���A���̃A���r�G���g�J���[����Z��������  ) * �����E�X�|�b�g���C�g�̊p�x������
				TempF3             = g_Common.Light[ 2 ].Diffuse * DiffuseAngleGen * ShadowGen ;
				TotalLightDiffuse += TempF3 * PSInput.Light2_Dir_Gen.w ;
				TotalDiffuse      += ( TempF3 + g_Common.Light[ 2 ].Ambient.xyz ) * PSInput.Light2_Dir_Gen.w ;

			#else // TOON == 1
				// �g�D�[���Q�̏ꍇ�̓f�B�t���[�Y�J���[�ƃ��C�g�̊p�x�����͕ʁX�ɕۑ����Ă���
				// �f�B�t���[�Y�J���[�~�ϒl           += ���C�g�̃f�B�t���[�Y�J���[
				// ���C�g�̃A���r�G���g�J���[�̒~�ϒl += ���C�g�̃A���r�G���g�J���[
				// ���C�g�̊p�x�������̒~�ϒl         += ���C�g�̊p�x������
				TotalDiffuse  += g_Common.Light[ 2 ].Diffuse ;

				TotalAmbient  += g_Common.Light[ 2 ].Ambient.rgb ;
				TotalAngleGen += DiffuseAngleGen * ShadowGen * PSInput.Light2_Dir_Gen.w ;

			#endif // TOON == 1
		#else // TOON
			// �f�B�t���[�Y�J���[�~�ϒl += ( ���C�g�̃f�B�t���[�Y�J���[ * �f�B�t���[�Y�J���[ * �f�B�t���[�Y�J���[�p�x������ + ���C�g�̃A���r�G���g�J���[�ƃ}�e���A���̃A���r�G���g�J���[����Z��������  ) * �����E�X�|�b�g���C�g�̊p�x������
			TotalDiffuse += ( g_Common.Light[ 2 ].Diffuse * PSInput.Diffuse.xyz * DiffuseAngleGen * ShadowGen + g_Common.Light[ 2 ].Ambient.xyz ) * PSInput.Light2_Dir_Gen.w ;
			
		#endif // TOON

	#endif // LG2_DIR


	// �X�y�L�����J���[�v�Z
	#ifdef USE_SPE

		// �n�[�t�x�N�g���̌v�Z
		TempF3 = PSInput.V_to_Eye + PSInput.Light2_Dir_Gen.xyz ;
		#ifdef    PHONG
			TempF3 = normalize( TempF3 ) ;
		#else  // PHONG
			TempF3 *= 0.5f ;
		#endif // PHONG

		// Temp = pow( max( 0.0f, N * H ), g_Common.Material.Power )
		Temp = pow( max( 0.0f, dot( Normal, TempF3 ) ), g_Common.Material.Power ) ;

		#if LG2_DIR

			// �X�y�L�����J���[�~�ϒl += Temp * ���C�g�̃X�y�L�����J���[
			TotalSpecular += Temp * g_Common.Light[ 2 ].Specular * ShadowGen ;

		#else // LG2_DIR

			// �X�y�L�����J���[�~�ϒl += Temp * �����E�X�|�b�g���C�g�̊p�x������ * ���C�g�̃X�y�L�����J���[
			TotalSpecular += Temp * PSInput.Light2_Dir_Gen.w * g_Common.Light[ 2 ].Specular * ShadowGen ;

		#endif // LG2_DIR

	#endif // USE_SPE

#endif // LG2_USE
// ���C�g2�̏��� +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �I�� )


















			// �o�̓J���[�v�Z +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �J�n )

			#if TOON != 2
				// TotalDiffuse = ���C�g�f�B�t���[�Y�J���[�~�ϒl + ( �}�e���A���̃A���r�G���g�J���[�ƃO���[�o���A���r�G���g�J���[����Z�������̂ƃ}�e���A���G�~�b�V�u�J���[�����Z�������� )
				TotalDiffuse += g_Common.Material.Ambient_Emissive.xyz ;
			#else // TOON != 2
				// �A���r�G���g�J���[�̒~�ϒl += �}�e���A���̃A���r�G���g�J���[�ƃO���[�o���A���r�G���g�J���[����Z�������̂ƃ}�e���A���G�~�b�V�u�J���[�����Z��������
				TotalAmbient += g_Common.Material.Ambient_Emissive.rgb ;
			#endif  // TOON != 2

			#ifdef USE_SPE
				#if TOON
					#if TOON == 1
						// �g�D�[���P�̎��̏o�͌v�Z
						ToonSpecularGradColor = g_ToonSpecularGradTexture.Sample( g_ToonSpecularGradSampler, max( TotalSpecular.b, max( TotalSpecular.r, TotalSpecular.g ) ) ) ;
					
						#ifdef USE_SPETEX
							TextureSpecularColor.rgb *= ToonSpecularGradColor.rgb ;
						#else  // USE_SPETEX
							TextureSpecularColor = ToonSpecularGradColor ;
						#endif // USE_SPETEX
					
						TextureSpecularColor.rgb *= g_ToonRGBtoVMaxRGBVolumeTexture.Sample( g_ToonRGBtoVMaxRGBVolumeSampler, TotalSpecular ).rgb * PSInput.Specular.rgb ;

						TextureDiffuseColor.rgb *= g_ToonRGBtoVMaxRGBVolumeTexture.Sample( g_ToonRGBtoVMaxRGBVolumeSampler, TotalLightDiffuse ).rgb * PSInput.Diffuse.rgb ;

						ToonDiffuseGradColor = g_ToonDiffuseGradTexture.Sample( g_ToonDiffuseGradSampler, max( TotalDiffuse.b, max( TotalDiffuse.r, TotalDiffuse.g ) ) ) ;

						#if TOON_DIFOP_MUL
							PSOutput.Color0 = TextureDiffuseColor * ToonDiffuseGradColor ;
						#endif // TOON_DIFOP_MUL
						#if TOON_DIFOP_TRANS
							PSOutput.Color0 = lerp( TextureDiffuseColor, ToonDiffuseGradColor, ToonDiffuseGradColor.a ) ;
						#endif // TOON_DIFOP_TRANS

						#if TOON_SPEOP_ADD
							PSOutput.Color0 = PSOutput.Color0 + TextureSpecularColor ;
						#endif // TOON_SPEOP_ADD
						#if TOON_SPEOP_TRANS
							PSOutput.Color0 = lerp( PSOutput.Color0, TextureSpecularColor, TextureSpecularColor.a ) ;
						#endif // TOON_SPEOP_TRANS

						#if TOON_DIFOP_MUL
							ParamAlpha *= ToonDiffuseGradColor.a ;
						#endif // TOON_DIFOP_MUL

						PSOutput.Color0.a = TextureDiffuseColor.a * ParamAlpha ;
					
					#else  // TOON == 1
						// �g�D�[���Q�̎��̏o�͌v�Z
						ToonDiffuseGradColor = g_ToonDiffuseGradTexture.Sample( g_ToonDiffuseGradSampler, TotalAngleGen ) ;

						// �o�� = saturate( saturate( �A���r�G���g�J���[�̒~�ϒl + �}�e���A���̃f�B�t���[�Y�J���[ * ���C�g�̃f�B�t���[�Y�J���[�̒~�ϒl ) * �e�N�X�`���O���f�[�V�����J���[ + �X�y�L�����J���[�~�ϒl * �}�e���A���̃X�y�L�����J���[ ) * �e�N�X�`���J���[
						PSOutput.Color0.rgb = saturate( saturate( TotalDiffuse * PSInput.Diffuse.rgb + TotalAmbient ) * ToonDiffuseGradColor.rgb + PSInput.Specular.rgb * TotalSpecular ) * TextureDiffuseColor.rgb ;
					
						#if TOON_SPHEREOP_MUL || TOON_SPHEREOP_ADD
							// �X�t�B�A�}�b�v�̐F���擾
							TempF3 = Normal * float3( 0.5f, -0.5f, 0.0f ) + 0.5f ;
							ToonSphereMapColor = g_ToonSphereMapTexture.Sample( g_ToonSphereMapSampler, TempF3.xy ) ;
						
							#if TOON_SPHEREOP_MUL
								// �X�t�B�A�}�b�v�̐F���o�͂ɏ�Z
								PSOutput.Color0.rgb *= ToonSphereMapColor.rgb ;
							#endif // TOON_SPHEREOP_MUL
						
							#if TOON_SPHEREOP_ADD
								// �X�t�B�A�}�b�v�̐F���o�͂ɉ��Z
								PSOutput.Color0.rgb += ToonSphereMapColor.rgb ;
							#endif // TOON_SPHEREOP_ADD
						
						#endif // TOON_SPHEREOP_MUL || TOON_SPHEREOP_ADD

						#if TOON_SPHEREOP_MUL 

							// �o�̓A���t�@ = �e�N�X�`���A���t�@ * Factor�A���t�@ * ���_�A���t�@ * �X�t�B�A�}�b�v�A���t�@
							PSOutput.Color0.a = TextureDiffuseColor.a * ParamAlpha * ToonSphereMapColor.a ;

						#else // TOON_SPHEREOP_MUL

							// �o�̓A���t�@ = �e�N�X�`���A���t�@ * Factor�A���t�@ * ���_�A���t�@
							PSOutput.Color0.a = TextureDiffuseColor.a * ParamAlpha ;

						#endif // TOON_SPHEREOP_MUL

					#endif   // TOON == 1
				#else  // TOON

					#ifdef USE_SPETEX
						// TextureSpecularColor = ���C�g�̃X�y�L�����J���[�~�ϒl * �X�y�L�����J���[ * �X�y�L�����e�N�X�`���J���[
						TextureSpecularColor.xyz = g_SpecularMapTexture.Sample( g_SpecularMapSampler, PSInput.TexCoords0_1.xy ).xyz * TotalSpecular * PSInput.Specular.xyz ;
					#else  // USE_SPETEX
						// TextureSpecularColor = ���C�g�̃X�y�L�����J���[�~�ϒl * �X�y�L�����J���[
						TextureSpecularColor.xyz = TotalSpecular.xyz * PSInput.Specular.xyz ;
					#endif // USE_SPETEX

					// �o�̓J���[ = TotalDiffuse * �e�N�X�`���J���[ + TextureSpecularColor

					PSOutput.Color0.rgb = TextureDiffuseColor.rgb * TotalDiffuse + TextureSpecularColor.rgb ;
					PSOutput.Color0.a = TextureDiffuseColor.a * ParamAlpha ;

				#endif // TOON

			#else  // USE_SPE

				#if TOON
					#if TOON == 1
						ToonDiffuseGradColor = g_ToonDiffuseGradTexture.Sample( g_ToonDiffuseGradSampler, max( TotalDiffuse.b, max( TotalDiffuse.r, TotalDiffuse.g ) ) ) ;
						TextureDiffuseColor.rgb *= g_ToonRGBtoVMaxRGBVolumeTexture.Sample( g_ToonRGBtoVMaxRGBVolumeSampler, TotalLightDiffuse ).rgb * PSInput.Diffuse.rgb ;

						#if TOON_DIFOP_MUL
							// �o�̓J���[ = �e�N�X�`���J���[ * �f�B�t���[�Y�J���[�~�ϒl * �O���f�[�V�����J���[
							PSOutput.Color0 = TextureDiffuseColor * ToonDiffuseGradColor ;
						#endif // TOON_DIFOP_MUL
						#if TOON_DIFOP_TRANS
							// �o�̓J���[ = �O���f�[�V�����J���[ * �O���f�[�V������ + ( 1 - �O���f�[�V������ ) * ( �e�N�X�`���J���[ * �f�B�t���[�Y�J���[�~�ϒl )
							PSOutput.Color0 = lerp( TextureDiffuseColor, ToonDiffuseGradColor, ToonDiffuseGradColor.a ) ;
						#endif // TOON_DIFOP_TRANS

						// ��Z�̏ꍇ�̓��l���O���f�[�V�������Ə�Z����
						#if TOON_DIFOP_MUL
							ParamAlpha *= ToonDiffuseGradColor.a ;
						#endif // TOON_DIFOP_MUL

						// �o�̓� = �e�N�X�`���� * �f�B�t���[�Y�� * ��惿
						PSOutput.Color0.a = TextureDiffuseColor.a * ParamAlpha ;

					#else	// TOON == 1
						// �g�D�[���Q�̎��̏o�͌v�Z
						ToonDiffuseGradColor = g_ToonDiffuseGradTexture.Sample( g_ToonDiffuseGradSampler, TotalAngleGen ) ;

						// �o�� = ( �A���r�G���g�J���[�̒~�ϒl + �}�e���A���̃f�B�t���[�Y�J���[ * ���C�g�̃f�B�t���[�Y�J���[�̒~�ϒl ) * �e�N�X�`���O���f�[�V�����J���[ * �e�N�X�`���J���[
						PSOutput.Color0.rgb = saturate( TotalDiffuse * PSInput.Diffuse.rgb + TotalAmbient.rgb ) * ToonDiffuseGradColor.rgb * TextureDiffuseColor.rgb ;

						#if TOON_SPHEREOP_MUL || TOON_SPHEREOP_ADD
							// �X�t�B�A�}�b�v�̐F���擾
							TempF3 = Normal * float3( 0.5f, -0.5f, 0.0f ) + 0.5f ;
							ToonSphereMapColor = g_ToonSphereMapTexture.Sample( g_ToonSphereMapSampler, TempF3.xy ) ;
						
							#if TOON_SPHEREOP_MUL
								// �X�t�B�A�}�b�v�̐F���o�͂ɏ�Z
								PSOutput.Color0.rgb *= ToonSphereMapColor.rgb ;
							#endif // TOON_SPHEREOP_MUL
						
							#if TOON_SPHEREOP_ADD
								// �X�t�B�A�}�b�v�̐F���o�͂ɉ��Z
								PSOutput.Color0.rgb += ToonSphereMapColor.rgb ;
							#endif // TOON_SPHEREOP_ADD
						
						#endif // TOON_SPHEREOP_MUL || TOON_SPHEREOP_ADD

						#if TOON_SPHEREOP_MUL

							PSOutput.Color0.a = TextureDiffuseColor.a * ParamAlpha * ToonSphereMapColor.a ;

						#else // TOON_SPHEREOP_MUL

							PSOutput.Color0.a = TextureDiffuseColor.a * ParamAlpha ;

						#endif // TOON_SPHEREOP_MUL

					#endif		// TOON == 1
				#else  // TOON
					// �o�̓J���[ = TotalDiffuse * �e�N�X�`���J���[
					PSOutput.Color0.rgb = TotalDiffuse * TextureDiffuseColor.rgb ;

					// �o�̓� = �e�N�X�`���� * �f�B�t���[�Y�� * ��惿
					PSOutput.Color0.a = TextureDiffuseColor.a * ParamAlpha ;
				#endif // TOON

			#endif // USE_SPE

			// �o�̓J���[�v�Z +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �I�� )


		// �o���v�}�b�v or �t�H���V�F�[�f�B���O�̏ꍇ =========================================( �I�� )
		#else  // BUMPMAP || PHONG
			// �o���v�}�b�v or �t�H���V�F�[�f�B���O�ł͂Ȃ��ꍇ ===================================( �J�n )

			#ifdef USE_SPE

				#ifdef USE_SPETEX

					PSOutput.Color0.rgb = TextureDiffuseColor.rgb * PSInput.Diffuse.rgb + TextureSpecularColor.rgb * PSInput.Specular.rgb ;

				#else  // USE_SPETEX

					PSOutput.Color0.rgb = TextureDiffuseColor.rgb * PSInput.Diffuse.rgb + PSInput.Specular.rgb ;

				#endif // USE_SPETEX

			#else  // USE_SPE

				PSOutput.Color0.rgb = TextureDiffuseColor.rgb * PSInput.Diffuse.rgb ;

			#endif // USE_SPE

			PSOutput.Color0.a   = TextureDiffuseColor.a   * ParamAlpha ;

		#endif // BUMPMAP || PHONG
		// �o���v�}�b�v or �t�H���V�F�[�f�B���O�ł͂Ȃ��ꍇ ===================================( �I�� )

	#endif // LG_USE
	// ���C�g���g���ꍇ *******************************************************************( �I�� )

	// �A���t�@�e�X�g
	AlphaTest( PSOutput.Color0.a ) ;

	// �t�H�O����
	PSOutput.Color0.rgb = lerp( g_Common.Fog.Color.rgb, PSOutput.Color0.rgb, saturate( PSInput.Normal_Fog.w ) );

	// �A���t�@��Z�J���[
	if( g_Base.MulAlphaColor.x > 0.5f )
	{
		PSOutput.Color0.rgb *= PSOutput.Color0.a;
	}

	return PSOutput ;
}

