#include "DxPixelShaderFxCode.h"

// main�֐�
PS_OUTPUT main( PS_INPUT PSInput )
{
	PS_OUTPUT PSOutput ;
	float4 TextureDiffuseColor ;
	float4 TextureSpecularColor ;
	float4 ToonSpecularGradColor ;
	float4 ToonDiffuseGradColor ;
	float4 ToonSphereMapColor ;
	float4 ToonRGBtoVMaxRGBVolumeColor ;
	float3 Normal ;
	float4 TotalDiffuse ;
	float4 TotalSpecular ;
	float4 TotalLightDiffuse ;
	float3 TotalAmbient ;
	float TotalAngleGen ;
	float DiffuseAngleGen ;
	float4 TempF4 ;
	float3 TempF3 ;
	float Temp ;
	float3 ShadowRate ;
	float ShadowGen ;
	float ParamAlpha;

#if SHADOWMAP
	float2 DepthTexCoord ;
	float4 TextureDepth ;
//	float4 variance_md_p_pmax ;
#endif

	ParamAlpha = PSInput.Diffuse.a * cfFactorColor.a;

#if SUBTEXTUREMODE != 0
	float4 SubTextureColor;
#endif  // SUBTEXTUREMODE != 0

/*
#if SHADOWMAP_DRAW
	// �V���h�E�}�b�v�ւ̕`�� ************************************************************( �J�n )

		// �y�l��F�Ƃ��ďo��
		PSOutput.Color0.r = PSInput.ShadowMap1Pos_ShadowMap3PosX.z;
//		PSOutput.Color0.g = PSInput.ShadowMap1Pos_ShadowMap3PosX.z * PSInput.ShadowMap1Pos_ShadowMap3PosX.z + 0.25f * ( ddx( PSInput.ShadowMap1Pos_ShadowMap3PosX.z ) * ddx( PSInput.ShadowMap1Pos_ShadowMap3PosX.z ) + ddy( PSInput.ShadowMap1Pos_ShadowMap3PosX.z ) * ddy( PSInput.ShadowMap1Pos_ShadowMap3PosX.z ) ) ;
		PSOutput.Color0.g = 0.0f ;
		PSOutput.Color0.b = 0.0f ;

		// �����ɂȂ�Ȃ��悤�ɃA���t�@�͕K���P
		PSOutput.Color0.a = 1.0f ;

	// �V���h�E�}�b�v�ւ̕`�� ************************************************************( �I�� )
#else // SHADOWMAP_DRAW
*/
		TextureDiffuseColor = tex2D( DiffuseMapTexture, PSInput.TexCoords0_1.xy ) ;
	
	#ifdef USE_SPE
		#ifdef USE_SPETEX
		TextureSpecularColor = tex2D( SpecularMapTexture, PSInput.TexCoords0_1.xy ) ;
		#endif // USE_SPETEX
	#endif // USE_SPE

	#if SUBTEXTUREMODE != 0
		SubTextureColor = tex2D( SubTexture, PSInput.TexCoords0_1.zw ) ;
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
							TempF3 = Normal * float3( 0.5f, -0.5f, 0.0f ) + cfZeroHalfOneTwo.yyy ;
							ToonSphereMapColor = tex2D( ToonSphereMapTexture, TempF3.xy ) ;

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
				PSOutput.Color0.r = PSInput.ShadowMap1Pos_ShadowMap3PosX.z;
//				PSOutput.Color0.g = PSInput.ShadowMap1Pos_ShadowMap3PosX.z * PSInput.ShadowMap1Pos_ShadowMap3PosX.z + 0.25f * ( ddx( PSInput.ShadowMap1Pos_ShadowMap3PosX.z ) * ddx( PSInput.ShadowMap1Pos_ShadowMap3PosX.z ) + ddy( PSInput.ShadowMap1Pos_ShadowMap3PosX.z ) * ddy( PSInput.ShadowMap1Pos_ShadowMap3PosX.z ) ) ;
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
					Normal = ( tex2D( NormalMapTexture, PSInput.TexCoords0_1.xy ).rgb - cfZeroHalfOneTwo.y ) * cfZeroHalfOneTwo.w ;

				#else // BUMPMAP

					Normal = PSInput.Normal_Fog.xyz ;

				#endif // BUMPMAP


				#ifdef    PHONG
					Normal = normalize( Normal ) ;
				#endif

				#if	SHADOWMAP
					// ���_�̃e�N�X�`�����W�l���͈͓��̏ꍇ�̂ݏ�������
					if( PSInput.ShadowMap1Pos_ShadowMap3PosX.x < -1.0f || PSInput.ShadowMap1Pos_ShadowMap3PosX.x > 1.0f ||
						PSInput.ShadowMap1Pos_ShadowMap3PosX.y < -1.0f || PSInput.ShadowMap1Pos_ShadowMap3PosX.y > 1.0f ||
						PSInput.ShadowMap1Pos_ShadowMap3PosX.z <  0.0f || PSInput.ShadowMap1Pos_ShadowMap3PosX.z > 1.0f )
					{
						ShadowRate.x = 1.0f;
					}
					else
					{
						// �[�x�e�N�X�`���̍��W���Z�o
						// PSInput.ShadowMap1Pos_ShadowMap3PosX.xy �� -1.0f �` 1.0f �̒l�Ȃ̂ŁA����� 0.0f �` 1.0f �̒l�ɂ���
						DepthTexCoord.x = ( PSInput.ShadowMap1Pos_ShadowMap3PosX.x + 1.0f ) / 2.0f;

						// y�͍X�ɏ㉺���]
						DepthTexCoord.y = 1.0f - ( PSInput.ShadowMap1Pos_ShadowMap3PosX.y + 1.0f ) / 2.0f;

						// �[�x�o�b�t�@�e�N�X�`������[�x���擾
						TextureDepth = tex2D( ShadowMap1Texture, DepthTexCoord );

						// �e�N�X�`���ɋL�^����Ă���[�x( +�␳�l )���y�l���傫�������牜�ɂ���Ƃ������ƂŌ��������ő�ɂ���
						ShadowRate.x = smoothstep( PSInput.ShadowMap1Pos_ShadowMap3PosX.z - cfShadowMap1_DAdj_Grad_Enbl0_1.y, PSInput.ShadowMap1Pos_ShadowMap3PosX.z, TextureDepth.r + cfShadowMap1_DAdj_Grad_Enbl0_1.x ) ;
					}

					// ���_�̃e�N�X�`�����W�l���͈͓��̏ꍇ�̂ݏ�������
					if( PSInput.ShadowMap2Pos_ShadowMap3PosY.x < -1.0f || PSInput.ShadowMap2Pos_ShadowMap3PosY.x > 1.0f ||
						PSInput.ShadowMap2Pos_ShadowMap3PosY.y < -1.0f || PSInput.ShadowMap2Pos_ShadowMap3PosY.y > 1.0f ||
						PSInput.ShadowMap2Pos_ShadowMap3PosY.z <  0.0f || PSInput.ShadowMap2Pos_ShadowMap3PosY.z > 1.0f )
					{
						ShadowRate.y = 1.0f;
					}
					else
					{
						// �[�x�e�N�X�`���̍��W���Z�o
						// PSInput.ShadowMap2Pos_ShadowMap3PosY.xy �� -1.0f �` 1.0f �̒l�Ȃ̂ŁA����� 0.0f �` 1.0f �̒l�ɂ���
						DepthTexCoord.x = ( PSInput.ShadowMap2Pos_ShadowMap3PosY.x + 1.0f ) / 2.0f;

						// y�͍X�ɏ㉺���]
						DepthTexCoord.y = 1.0f - ( PSInput.ShadowMap2Pos_ShadowMap3PosY.y + 1.0f ) / 2.0f;

						// �[�x�o�b�t�@�e�N�X�`������[�x���擾
						TextureDepth = tex2D( ShadowMap2Texture, DepthTexCoord );

						// �e�N�X�`���ɋL�^����Ă���[�x( +�␳�l )���y�l���傫�������牜�ɂ���Ƃ������ƂŌ��������ő�ɂ���
						ShadowRate.y = smoothstep( PSInput.ShadowMap2Pos_ShadowMap3PosY.z - cfShadowMap1_Enb2_ShadowMap2_DAdj_Grad_Enbl0.z, PSInput.ShadowMap2Pos_ShadowMap3PosY.z, TextureDepth.r + cfShadowMap1_Enb2_ShadowMap2_DAdj_Grad_Enbl0.y ) ;
					}

					// ���_�̃e�N�X�`�����W�l���͈͓��̏ꍇ�̂ݏ�������
					if( PSInput.ShadowMap1Pos_ShadowMap3PosX.w < -1.0f || PSInput.ShadowMap1Pos_ShadowMap3PosX.w > 1.0f ||
						PSInput.ShadowMap2Pos_ShadowMap3PosY.w < -1.0f || PSInput.ShadowMap2Pos_ShadowMap3PosY.w > 1.0f ||
						PSInput.V_to_Eye_ShadowMap3PosZ.w      <  0.0f || PSInput.V_to_Eye_ShadowMap3PosZ.w      > 1.0f )
					{
						ShadowRate.z = 1.0f;
					}
					else
					{
						// �[�x�e�N�X�`���̍��W���Z�o
						// PSInput.ShadowMap1Pos_ShadowMap3PosX.w �� PSInput.ShadowMap2Pos_ShadowMap3PosY.w �� -1.0f �` 1.0f �̒l�Ȃ̂ŁA����� 0.0f �` 1.0f �̒l�ɂ���
						DepthTexCoord.x = ( PSInput.ShadowMap1Pos_ShadowMap3PosX.w + 1.0f ) / 2.0f;

						// y�͍X�ɏ㉺���]
						DepthTexCoord.y = 1.0f - ( PSInput.ShadowMap2Pos_ShadowMap3PosY.w + 1.0f ) / 2.0f;

						// �[�x�o�b�t�@�e�N�X�`������[�x���擾
						TextureDepth = tex2D( ShadowMap3Texture, DepthTexCoord );

						// �e�N�X�`���ɋL�^����Ă���[�x( +�␳�l )���y�l���傫�������牜�ɂ���Ƃ������ƂŌ��������ő�ɂ���
						ShadowRate.z = smoothstep( PSInput.V_to_Eye_ShadowMap3PosZ.w - cfShadowMap2_Enbl1_2_ShadowMap3_DAdj_Grad.w, PSInput.V_to_Eye_ShadowMap3PosZ.w, TextureDepth.r + cfShadowMap2_Enbl1_2_ShadowMap3_DAdj_Grad.z ) ;
					}

/*
					variance_md_p_pmax.x = TextureDepth.g - ( TextureDepth.r * TextureDepth.r ) ;
					variance_md_p_pmax.y = TextureDepth.r - ( PSInput.ShadowMap1Pos_ShadowMap3PosX.z + 1.0f ) ;
					variance_md_p_pmax.z = variance_md_p_pmax.x / ( variance_md_p_pmax.x + variance_md_p_pmax.y * variance_md_p_pmax.y ) ;
					ShadowRate = max( saturate( variance_md_p_pmax.z ), ( PSInput.ShadowMap1Pos_ShadowMap3PosX.z + 1.0f ) <= TextureDepth.r + cfShadowMap_DAdj_Grad.x ) ;
*/
/*
					variance_md_p_pmax.x = TextureDepth.g - ( TextureDepth.r * TextureDepth.r ) ;
					variance_md_p_pmax.y = TextureDepth.r - PSInput.ShadowMap1Pos_ShadowMap3PosX.z ;
					variance_md_p_pmax.z = variance_md_p_pmax.x / ( variance_md_p_pmax.x + variance_md_p_pmax.y * variance_md_p_pmax.y ) ;
//					ShadowRate = max( saturate( variance_md_p_pmax.z ), PSInput.ShadowMap1Pos_ShadowMap3PosX.z <= TextureDepth.r + cfShadowMap_DAdj_Grad.x ) ;
					ShadowRate = saturate( variance_md_p_pmax.z ) ;
*/
/*
					variance_md_p_pmax.z = smoothstep( PSInput.ShadowMap1Pos_ShadowMap3PosX.z - 0.02f, PSInput.ShadowMap1Pos_ShadowMap3PosX.z, TextureDepth.r ) ;
					variance_md_p_pmax.x = max( TextureDepth.g - ( TextureDepth.r * TextureDepth.r ), 0.0f ) ;
					variance_md_p_pmax.y = PSInput.ShadowMap1Pos_ShadowMap3PosX.z - TextureDepth.r ;
					variance_md_p_pmax.y = variance_md_p_pmax.y * variance_md_p_pmax.y ;
					variance_md_p_pmax.y = max( variance_md_p_pmax.y, 0.001f ) ;
					variance_md_p_pmax.w = variance_md_p_pmax.x / ( variance_md_p_pmax.x + variance_md_p_pmax.y ) ;
					variance_md_p_pmax.w = clamp( variance_md_p_pmax.w, 0.0f, 1.0f ) ;
//					variance_md_p_pmax.w = clamp( ( variance_md_p_pmax.w - 0.2f ) / ( 1.0f - 0.2f ), 0.0f, 1.0f ) ;
//					ShadowRate = clamp( max( variance_md_p_pmax.z, variance_md_p_pmax.w ), 0.0f, 1.0f ) ;
					ShadowRate = clamp( variance_md_p_pmax.z, 0.0f, 1.0f ) ;
*/
/*
					if( saturate( PSInput.ShadowMap1Pos_ShadowMap3PosX.z ) < TextureDepth.r + cfShadowMap_DAdj_Grad.x )
			 		{
						ShadowRate += 1.0f / 5.0f ;
					}

					// �[�x�o�b�t�@�e�N�X�`������[�x���擾
					DepthTexCoord.x -= 1.0f / 4096.0f ;
					TextureDepth = tex2D( ShadowMapTexture, DepthTexCoord );
					if( saturate( PSInput.ShadowMap1Pos_ShadowMap3PosX.z ) < TextureDepth.r + cfShadowMap_DAdj_Grad.x )
			 		{
						ShadowRate += 1.0f / 5.0f ;
					}
					DepthTexCoord.x += 2.0f / 4096.0f ;
					TextureDepth = tex2D( ShadowMapTexture, DepthTexCoord );
					if( saturate( PSInput.ShadowMap1Pos_ShadowMap3PosX.z ) < TextureDepth.r + cfShadowMap_DAdj_Grad.x )
			 		{
						ShadowRate += 1.0f / 5.0f ;
					}
					DepthTexCoord.x -= 1.0f / 4096.0f ;
					DepthTexCoord.y -= 1.0f / 4096.0f ;
					TextureDepth = tex2D( ShadowMapTexture, DepthTexCoord );
					if( saturate( PSInput.ShadowMap1Pos_ShadowMap3PosX.z ) < TextureDepth.r + cfShadowMap_DAdj_Grad.x )
			 		{
						ShadowRate += 1.0f / 5.0f ;
					}
					DepthTexCoord.y += 2.0f / 4096.0f ;
					TextureDepth = tex2D( ShadowMapTexture, DepthTexCoord );
					if( saturate( PSInput.ShadowMap1Pos_ShadowMap3PosX.z ) < TextureDepth.r + cfShadowMap_DAdj_Grad.x )
			 		{
						ShadowRate += 1.0f / 5.0f ;
					}
*/
				#else // SHADOWMAP
					ShadowRate.x = 1.0f ;
					ShadowRate.y = 1.0f ;
					ShadowRate.z = 1.0f ;
				#endif // SHADOWMAP

				// �f�B�t���[�Y�J���[�ƃX�y�L�����J���[�̒~�ϒl��������
				TotalDiffuse = cfZeroHalfOneTwo.x ;
				TotalSpecular = cfZeroHalfOneTwo.x ;
				#if TOON
					#if TOON == 1
						TotalLightDiffuse = cfZeroHalfOneTwo.x ;
					#else // TOON == 1
						TotalAmbient = cfZeroHalfOneTwo.x ;
						TotalAngleGen = cfZeroHalfOneTwo.x ;
					#endif
				#endif // TOON




				
// ���C�g0�̏��� +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �J�n )
#if LG0_USE

	// �f�B�t���[�Y�F�v�Z

	// DiffuseAngleGen = �f�B�t���[�Y�p�x�������v�Z
	DiffuseAngleGen = saturate( dot( Normal, PSInput.Light0_Dir_Gen.xyz ) ) ;
	
	// �e�ɂ�錸�����Z�o
	ShadowGen = max( ShadowRate.x, SHADOWMAP1_ENABLE_LGT0 ) *
	            max( ShadowRate.y, SHADOWMAP2_ENABLE_LGT0 ) *
	            max( ShadowRate.z, SHADOWMAP3_ENABLE_LGT0 ) ;

	#if LG0_DIR

		#if TOON
			#if TOON == 1
				// �g�D�[���̏ꍇ�͂��̎��_�ł͒��_or�}�e���A���̃f�B�t���[�Y�J���[�͎g�p���Ȃ�
				// �f�B�t���[�Y�J���[�~�ϒl += ���C�g�̃f�B�t���[�Y�J���[ * �f�B�t���[�Y�J���[�p�x������ + ���C�g�̃A���r�G���g�J���[�ƃ}�e���A���̃A���r�G���g�J���[����Z�������� 
				TempF4 = cfLight[ 0 ].Diffuse * DiffuseAngleGen * ShadowGen ;
				TotalLightDiffuse += TempF4 ;
				TotalDiffuse += TempF4 + cfLight[ 0 ].Ambient ;

			#else // TOON == 1
				// �g�D�[���Q�̏ꍇ�̓f�B�t���[�Y�J���[�ƃ��C�g�̊p�x�����͕ʁX�ɕۑ����Ă���
				// �f�B�t���[�Y�J���[�~�ϒl += ���C�g�̃f�B�t���[�Y�J���[
				// ���C�g�̊p�x�������̒~�ϒl += ���C�g�̊p�x������
				// ���C�g�̃A���r�G���g�J���[�̒~�ϒl += ���C�g�̃A���r�G���g�J���[
				TotalDiffuse += cfLight[ 0 ].Diffuse ;

				TotalAmbient += cfLight[ 0 ].Ambient.rgb ;
				TotalAngleGen += DiffuseAngleGen * ShadowGen ;

			#endif // TOON == 1
		#else // TOON
			// �f�B�t���[�Y�J���[�~�ϒl += ���C�g�̃f�B�t���[�Y�J���[ * �f�B�t���[�Y�J���[ * �f�B�t���[�Y�J���[�p�x������ + ���C�g�̃A���r�G���g�J���[�ƃ}�e���A���̃A���r�G���g�J���[����Z�������� 
			TotalDiffuse += cfLight[ 0 ].Diffuse * PSInput.Diffuse * DiffuseAngleGen * ShadowGen + cfLight[ 0 ].Ambient ;

		#endif //TOON

	#else // LG0_DIR

		#if TOON
			#if TOON == 1
				// �g�D�[���̏ꍇ�͂��̎��_�ł͒��_or�}�e���A���̃f�B�t���[�Y�J���[�͎g�p���Ȃ�
				// �f�B�t���[�Y�J���[�~�ϒl += ( ���C�g�̃f�B�t���[�Y�J���[ * �f�B�t���[�Y�J���[�p�x������ + ���C�g�̃A���r�G���g�J���[�ƃ}�e���A���̃A���r�G���g�J���[����Z��������  ) * �����E�X�|�b�g���C�g�̊p�x������
				TempF4 = cfLight[ 0 ].Diffuse * DiffuseAngleGen * ShadowGen ;
				TotalLightDiffuse += TempF4 * PSInput.Light0_Dir_Gen.w ;
				TotalDiffuse += ( TempF4 + cfLight[ 0 ].Ambient ) * PSInput.Light0_Dir_Gen.w ;

			#else // TOON == 1
				// �g�D�[���Q�̏ꍇ�̓f�B�t���[�Y�J���[�ƃ��C�g�̊p�x�����͕ʁX�ɕۑ����Ă���
				// �f�B�t���[�Y�J���[�~�ϒl += ���C�g�̃f�B�t���[�Y�J���[
				// ���C�g�̃A���r�G���g�J���[�̒~�ϒl += ���C�g�̃A���r�G���g�J���[
				// ���C�g�̊p�x�������̒~�ϒl += ���C�g�̊p�x������
				TotalDiffuse += cfLight[ 0 ].Diffuse ;

				TotalAmbient += cfLight[ 0 ].Ambient.rgb ;
				TotalAngleGen += DiffuseAngleGen * ShadowGen * PSInput.Light0_Dir_Gen.w ;

			#endif // TOON == 1
		#else // TOON
			// �f�B�t���[�Y�J���[�~�ϒl += ( ���C�g�̃f�B�t���[�Y�J���[ * �f�B�t���[�Y�J���[ * �f�B�t���[�Y�J���[�p�x������ + ���C�g�̃A���r�G���g�J���[�ƃ}�e���A���̃A���r�G���g�J���[����Z��������  ) * �����E�X�|�b�g���C�g�̊p�x������
			TotalDiffuse += ( cfLight[ 0 ].Diffuse * PSInput.Diffuse * DiffuseAngleGen * ShadowGen + cfLight[ 0 ].Ambient ) * PSInput.Light0_Dir_Gen.w ;
			

		#endif // TOON

	#endif // LG0_DIR


	// �X�y�L�����J���[�v�Z
	#ifdef USE_SPE

		// �n�[�t�x�N�g���̌v�Z
		TempF3 = PSInput.V_to_Eye_ShadowMap3PosZ.xyz + PSInput.Light0_Dir_Gen.xyz ;
		#ifdef    PHONG
			TempF3 = normalize( TempF3 ) ;
		#else  // PHONG
			TempF3 *= 0.5f ;
		#endif // PHONG

		// Temp = pow( max( 0.0f, N * H ), cfMaterial.Power.x )
		Temp = pow( max( 0.0f, dot( Normal, TempF3 ) ), cfMaterial.Power.x ) ;

		#if LG0_DIR

			// �X�y�L�����J���[�~�ϒl += Temp * ���C�g�̃X�y�L�����J���[
			TotalSpecular += Temp * cfLight[ 0 ].Specular * ShadowGen ;

		#else // LG0_DIR

			// �X�y�L�����J���[�~�ϒl += Temp * �����E�X�|�b�g���C�g�̊p�x������ * ���C�g�̃X�y�L�����J���[
			TotalSpecular += Temp * PSInput.Light0_Dir_Gen.w * cfLight[ 0 ].Specular * ShadowGen ;

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
	ShadowGen = max( ShadowRate.x, SHADOWMAP1_ENABLE_LGT1 ) *
	            max( ShadowRate.y, SHADOWMAP2_ENABLE_LGT1 ) *
	            max( ShadowRate.z, SHADOWMAP3_ENABLE_LGT1 ) ;

	#if LG1_DIR

		#if TOON
			#if TOON == 1
				// �g�D�[���̏ꍇ�͂��̎��_�ł͒��_or�}�e���A���̃f�B�t���[�Y�J���[�͎g�p���Ȃ�
				// �f�B�t���[�Y�J���[�~�ϒl += ���C�g�̃f�B�t���[�Y�J���[ * �f�B�t���[�Y�J���[�p�x������ + ���C�g�̃A���r�G���g�J���[�ƃ}�e���A���̃A���r�G���g�J���[����Z�������� 
				TempF4 = cfLight[ 1 ].Diffuse * DiffuseAngleGen * ShadowGen ;
				TotalLightDiffuse += TempF4 ;
				TotalDiffuse += TempF4 + cfLight[ 1 ].Ambient ;

			#else // TOON == 1
				// �g�D�[���Q�̏ꍇ�̓f�B�t���[�Y�J���[�ƃ��C�g�̊p�x�����͕ʁX�ɕۑ����Ă���
				// �f�B�t���[�Y�J���[�~�ϒl += ���C�g�̃f�B�t���[�Y�J���[
				// ���C�g�̊p�x�������̒~�ϒl += ���C�g�̊p�x������
				// ���C�g�̃A���r�G���g�J���[�̒~�ϒl += ���C�g�̃A���r�G���g�J���[
				TotalDiffuse += cfLight[ 1 ].Diffuse ;

				TotalAmbient += cfLight[ 1 ].Ambient.rgb ;
				TotalAngleGen += DiffuseAngleGen * ShadowGen ;

			#endif // TOON == 1
		#else // TOON
			// �f�B�t���[�Y�J���[�~�ϒl += ���C�g�̃f�B�t���[�Y�J���[ * �f�B�t���[�Y�J���[ * �f�B�t���[�Y�J���[�p�x������ + ���C�g�̃A���r�G���g�J���[�ƃ}�e���A���̃A���r�G���g�J���[����Z�������� 
			TotalDiffuse += cfLight[ 1 ].Diffuse * PSInput.Diffuse * DiffuseAngleGen * ShadowGen + cfLight[ 1 ].Ambient ;

		#endif //TOON

	#else // LG1_DIR

		#if TOON
			#if TOON == 1
				// �g�D�[���̏ꍇ�͂��̎��_�ł͒��_or�}�e���A���̃f�B�t���[�Y�J���[�͎g�p���Ȃ�
				// �f�B�t���[�Y�J���[�~�ϒl += ( ���C�g�̃f�B�t���[�Y�J���[ * �f�B�t���[�Y�J���[�p�x������ + ���C�g�̃A���r�G���g�J���[�ƃ}�e���A���̃A���r�G���g�J���[����Z��������  ) * �����E�X�|�b�g���C�g�̊p�x������
				TempF4 = cfLight[ 1 ].Diffuse * DiffuseAngleGen * ShadowGen ;
				TotalLightDiffuse += TempF4 * PSInput.Light1_Dir_Gen.w ;
				TotalDiffuse += ( TempF4 + cfLight[ 1 ].Ambient ) * PSInput.Light1_Dir_Gen.w ;

			#else // TOON == 1
				// �g�D�[���Q�̏ꍇ�̓f�B�t���[�Y�J���[�ƃ��C�g�̊p�x�����͕ʁX�ɕۑ����Ă���
				// �f�B�t���[�Y�J���[�~�ϒl += ���C�g�̃f�B�t���[�Y�J���[
				// ���C�g�̃A���r�G���g�J���[�̒~�ϒl += ���C�g�̃A���r�G���g�J���[
				// ���C�g�̊p�x�������̒~�ϒl += ���C�g�̊p�x������
				TotalDiffuse += cfLight[ 1 ].Diffuse ;

				TotalAmbient += cfLight[ 1 ].Ambient.rgb ;
				TotalAngleGen += DiffuseAngleGen * ShadowGen * PSInput.Light1_Dir_Gen.w ;

			#endif // TOON == 1
		#else // TOON
			// �f�B�t���[�Y�J���[�~�ϒl += ( ���C�g�̃f�B�t���[�Y�J���[ * �f�B�t���[�Y�J���[ * �f�B�t���[�Y�J���[�p�x������ + ���C�g�̃A���r�G���g�J���[�ƃ}�e���A���̃A���r�G���g�J���[����Z��������  ) * �����E�X�|�b�g���C�g�̊p�x������
			TotalDiffuse += ( cfLight[ 1 ].Diffuse * PSInput.Diffuse * DiffuseAngleGen * ShadowGen + cfLight[ 1 ].Ambient ) * PSInput.Light1_Dir_Gen.w ;
			

		#endif // TOON

	#endif // LG1_DIR


	// �X�y�L�����J���[�v�Z
	#ifdef USE_SPE

		// �n�[�t�x�N�g���̌v�Z
		TempF3 = PSInput.V_to_Eye_ShadowMap3PosZ.xyz + PSInput.Light1_Dir_Gen.xyz ;
		#ifdef    PHONG
			TempF3 = normalize( TempF3 ) ;
		#else  // PHONG
			TempF3 *= 0.5f ;
		#endif // PHONG

		// Temp = pow( max( 0.0f, N * H ), cfMaterial.Power.x )
		Temp = pow( max( 0.0f, dot( Normal, TempF3 ) ), cfMaterial.Power.x ) ;

		#if LG1_DIR

			// �X�y�L�����J���[�~�ϒl += Temp * ���C�g�̃X�y�L�����J���[
			TotalSpecular += Temp * cfLight[ 1 ].Specular * ShadowGen ;

		#else // LG1_DIR

			// �X�y�L�����J���[�~�ϒl += Temp * �����E�X�|�b�g���C�g�̊p�x������ * ���C�g�̃X�y�L�����J���[
			TotalSpecular += Temp * PSInput.Light1_Dir_Gen.w * cfLight[ 1 ].Specular * ShadowGen ;

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
	ShadowGen = max( ShadowRate.x, SHADOWMAP1_ENABLE_LGT2 ) *
	            max( ShadowRate.y, SHADOWMAP2_ENABLE_LGT2 ) *
	            max( ShadowRate.z, SHADOWMAP3_ENABLE_LGT2 ) ;

	#if LG2_DIR

		#if TOON
			#if TOON == 1
				// �g�D�[���̏ꍇ�͂��̎��_�ł͒��_or�}�e���A���̃f�B�t���[�Y�J���[�͎g�p���Ȃ�
				// �f�B�t���[�Y�J���[�~�ϒl += ���C�g�̃f�B�t���[�Y�J���[ * �f�B�t���[�Y�J���[�p�x������ + ���C�g�̃A���r�G���g�J���[�ƃ}�e���A���̃A���r�G���g�J���[����Z�������� 
				TempF4 = cfLight[ 2 ].Diffuse * DiffuseAngleGen * ShadowGen ;
				TotalLightDiffuse += TempF4 ;
				TotalDiffuse += TempF4 + cfLight[ 2 ].Ambient ;

			#else // TOON == 1
				// �g�D�[���Q�̏ꍇ�̓f�B�t���[�Y�J���[�ƃ��C�g�̊p�x�����͕ʁX�ɕۑ����Ă���
				// �f�B�t���[�Y�J���[�~�ϒl += ���C�g�̃f�B�t���[�Y�J���[
				// ���C�g�̊p�x�������̒~�ϒl += ���C�g�̊p�x������
				// ���C�g�̃A���r�G���g�J���[�̒~�ϒl += ���C�g�̃A���r�G���g�J���[
				TotalDiffuse += cfLight[ 2 ].Diffuse ;

				TotalAmbient += cfLight[ 2 ].Ambient.rgb ;
				TotalAngleGen += DiffuseAngleGen * ShadowGen ;

			#endif // TOON == 1
		#else // TOON
			// �f�B�t���[�Y�J���[�~�ϒl += ���C�g�̃f�B�t���[�Y�J���[ * �f�B�t���[�Y�J���[ * �f�B�t���[�Y�J���[�p�x������ + ���C�g�̃A���r�G���g�J���[�ƃ}�e���A���̃A���r�G���g�J���[����Z�������� 
			TotalDiffuse += cfLight[ 2 ].Diffuse * PSInput.Diffuse * DiffuseAngleGen * ShadowGen + cfLight[ 2 ].Ambient ;

		#endif //TOON

	#else // LG2_DIR

		#if TOON
			#if TOON == 1
				// �g�D�[���̏ꍇ�͂��̎��_�ł͒��_or�}�e���A���̃f�B�t���[�Y�J���[�͎g�p���Ȃ�
				// �f�B�t���[�Y�J���[�~�ϒl += ( ���C�g�̃f�B�t���[�Y�J���[ * �f�B�t���[�Y�J���[�p�x������ + ���C�g�̃A���r�G���g�J���[�ƃ}�e���A���̃A���r�G���g�J���[����Z��������  ) * �����E�X�|�b�g���C�g�̊p�x������
				TempF4 = cfLight[ 2 ].Diffuse * DiffuseAngleGen * ShadowGen ;
				TotalLightDiffuse += TempF4 * PSInput.Light2_Dir_Gen.w ;
				TotalDiffuse += ( TempF4 + cfLight[ 2 ].Ambient ) * PSInput.Light2_Dir_Gen.w ;

			#else // TOON == 1
				// �g�D�[���Q�̏ꍇ�̓f�B�t���[�Y�J���[�ƃ��C�g�̊p�x�����͕ʁX�ɕۑ����Ă���
				// �f�B�t���[�Y�J���[�~�ϒl += ���C�g�̃f�B�t���[�Y�J���[
				// ���C�g�̃A���r�G���g�J���[�̒~�ϒl += ���C�g�̃A���r�G���g�J���[
				// ���C�g�̊p�x�������̒~�ϒl += ���C�g�̊p�x������
				TotalDiffuse += cfLight[ 2 ].Diffuse ;

				TotalAmbient += cfLight[ 2 ].Ambient.rgb ;
				TotalAngleGen += DiffuseAngleGen * ShadowGen * PSInput.Light2_Dir_Gen.w ;

			#endif // TOON == 1
		#else // TOON
			// �f�B�t���[�Y�J���[�~�ϒl += ( ���C�g�̃f�B�t���[�Y�J���[ * �f�B�t���[�Y�J���[ * �f�B�t���[�Y�J���[�p�x������ + ���C�g�̃A���r�G���g�J���[�ƃ}�e���A���̃A���r�G���g�J���[����Z��������  ) * �����E�X�|�b�g���C�g�̊p�x������
			TotalDiffuse += ( cfLight[ 2 ].Diffuse * PSInput.Diffuse * DiffuseAngleGen * ShadowGen + cfLight[ 2 ].Ambient ) * PSInput.Light2_Dir_Gen.w ;
			

		#endif // TOON

	#endif // LG2_DIR


	// �X�y�L�����J���[�v�Z
	#ifdef USE_SPE

		// �n�[�t�x�N�g���̌v�Z
		TempF3 = PSInput.V_to_Eye_ShadowMap3PosZ.xyz + PSInput.Light2_Dir_Gen.xyz ;
		#ifdef    PHONG
			TempF3 = normalize( TempF3 ) ;
		#else  // PHONG
			TempF3 *= 0.5f ;
		#endif // PHONG

		// Temp = pow( max( 0.0f, N * H ), cfMaterial.Power.x )
		Temp = pow( max( 0.0f, dot( Normal, TempF3 ) ), cfMaterial.Power.x ) ;

		#if LG2_DIR

			// �X�y�L�����J���[�~�ϒl += Temp * ���C�g�̃X�y�L�����J���[
			TotalSpecular += Temp * cfLight[ 2 ].Specular * ShadowGen ;

		#else // LG2_DIR

			// �X�y�L�����J���[�~�ϒl += Temp * �����E�X�|�b�g���C�g�̊p�x������ * ���C�g�̃X�y�L�����J���[
			TotalSpecular += Temp * PSInput.Light2_Dir_Gen.w * cfLight[ 2 ].Specular * ShadowGen ;

		#endif // LG2_DIR

	#endif // USE_SPE

#endif // LG2_USE
// ���C�g2�̏��� +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �I�� )


















				// �o�̓J���[�v�Z +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �J�n )

				#if TOON != 2
					// TotalDiffuse = ���C�g�f�B�t���[�Y�J���[�~�ϒl + ( �}�e���A���̃A���r�G���g�J���[�ƃO���[�o���A���r�G���g�J���[����Z�������̂ƃ}�e���A���G�~�b�V�u�J���[�����Z�������� )
					TotalDiffuse += cfAmbient_Emissive ;
				#else // TOON != 2
					// �A���r�G���g�J���[�̒~�ϒl += �}�e���A���̃A���r�G���g�J���[�ƃO���[�o���A���r�G���g�J���[����Z�������̂ƃ}�e���A���G�~�b�V�u�J���[�����Z��������
					TotalAmbient += cfAmbient_Emissive.rgb ;
				#endif  // TOON != 2

				#ifdef USE_SPE
					#if TOON
						#if TOON == 1
							// �g�D�[���P�̎��̏o�͌v�Z
							ToonSpecularGradColor = tex2D( ToonSpecularGradTexture, max( TotalSpecular.b, max( TotalSpecular.r, TotalSpecular.g ) ) ) ;
						
							#ifdef USE_SPETEX
								TextureSpecularColor.rgb *= ToonSpecularGradColor.rgb ;
							#else  // USE_SPETEX
								TextureSpecularColor = ToonSpecularGradColor ;
							#endif // USE_SPETEX
						
							TextureSpecularColor.rgb *= tex3D( ToonRGBtoVMaxRGBVolumeTexture, TotalSpecular.rgb ).rgb * PSInput.Specular.rgb ;

							TextureDiffuseColor.rgb *= tex3D( ToonRGBtoVMaxRGBVolumeTexture, TotalLightDiffuse.rgb ).rgb * PSInput.Diffuse.rgb ;

							ToonDiffuseGradColor = tex2D( ToonDiffuseGradTexture, max( TotalDiffuse.b, max( TotalDiffuse.r, TotalDiffuse.g ) ) ) ;

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
							ToonDiffuseGradColor = tex2D( ToonDiffuseGradTexture, TotalAngleGen ) ;

							// �o�� = saturate( saturate( �A���r�G���g�J���[�̒~�ϒl + �}�e���A���̃f�B�t���[�Y�J���[ * ���C�g�̃f�B�t���[�Y�J���[�̒~�ϒl ) * �e�N�X�`���O���f�[�V�����J���[ + �X�y�L�����J���[�~�ϒl * �}�e���A���̃X�y�L�����J���[ ) * �e�N�X�`���J���[
							PSOutput.Color0.rgb = saturate( saturate( TotalDiffuse.rgb * PSInput.Diffuse.rgb + TotalAmbient ) * ToonDiffuseGradColor.rgb + PSInput.Specular.rgb * TotalSpecular.rgb ) * TextureDiffuseColor.rgb ;
						
							#if TOON_SPHEREOP_MUL || TOON_SPHEREOP_ADD
								// �X�t�B�A�}�b�v�̐F���擾
								TempF3 = Normal * float3( 0.5f, -0.5f, 0.0f ) + cfZeroHalfOneTwo.yyy ;
								ToonSphereMapColor = tex2D( ToonSphereMapTexture, TempF3.xy ) ;
							
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
							TextureSpecularColor = tex2D( SpecularMapTexture, PSInput.TexCoords0_1.xy ) * TotalSpecular * PSInput.Specular ;
						#else  // USE_SPETEX
							// TextureSpecularColor = ���C�g�̃X�y�L�����J���[�~�ϒl * �X�y�L�����J���[
							TextureSpecularColor = TotalSpecular * PSInput.Specular ;
						#endif // USE_SPETEX

						// �o�̓J���[ = TotalDiffuse * �e�N�X�`���J���[ + TextureSpecularColor

						PSOutput.Color0.rgb = TextureDiffuseColor.rgb * TotalDiffuse.rgb + TextureSpecularColor.rgb ;
						PSOutput.Color0.a = TextureDiffuseColor.a * ParamAlpha ;

					#endif // TOON

				#else  // USE_SPE

					#if TOON
						#if TOON == 1
							ToonDiffuseGradColor = tex2D( ToonDiffuseGradTexture, max( TotalDiffuse.b, max( TotalDiffuse.r, TotalDiffuse.g ) ) ) ;
							TextureDiffuseColor.rgb *= tex3D( ToonRGBtoVMaxRGBVolumeTexture, TotalLightDiffuse.rgb ).rgb * PSInput.Diffuse.rgb ;

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
							ToonDiffuseGradColor = tex2D( ToonDiffuseGradTexture, TotalAngleGen ) ;

							// �o�� = ( �A���r�G���g�J���[�̒~�ϒl + �}�e���A���̃f�B�t���[�Y�J���[ * ���C�g�̃f�B�t���[�Y�J���[�̒~�ϒl ) * �e�N�X�`���O���f�[�V�����J���[ * �e�N�X�`���J���[
							PSOutput.Color0.rgb = saturate( TotalDiffuse.rgb * PSInput.Diffuse.rgb + TotalAmbient.rgb ) * ToonDiffuseGradColor.rgb * TextureDiffuseColor.rgb ;

							#if TOON_SPHEREOP_MUL || TOON_SPHEREOP_ADD
								// �X�t�B�A�}�b�v�̐F���擾
								TempF3 = Normal * float3( 0.5f, -0.5f, 0.0f ) + cfZeroHalfOneTwo.yyy ;
								ToonSphereMapColor = tex2D( ToonSphereMapTexture, TempF3.xy ) ;
							
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
						PSOutput.Color0.rgb = TotalDiffuse.rgb * TextureDiffuseColor.rgb ;

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

	#if SM_3
		// �t�H�O����
		PSOutput.Color0.rgb = lerp( cfFogColor.rgb, PSOutput.Color0.rgb, saturate( PSInput.Normal_Fog.w ) );
	#endif // SM_3

	// �A���t�@��Z�J���[
	if( cfMulAlphaColor.x > 0.5f )
	{
		PSOutput.Color0.rgb *= PSOutput.Color0.a;
	}

// #endif // SHADOWMAP_DRAW

	return PSOutput ;
}

