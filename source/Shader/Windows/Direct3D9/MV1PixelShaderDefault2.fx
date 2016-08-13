#include "DxPixelShaderFxCode2.h"

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
	
	float DiffuseAngleGen ;
	float4 TotalDiffuse ;
	float4 TotalSpecular ;
	float4 TotalLightDiffuse ;
	float3 TotalAmbient ;
	float TotalAngleGen ;
	float Temp ;
	float3 TempF3 ;
	float4 TempF4 ;
	float3 Normal ;
	float3 V_to_Eye ;
	float3 VNrm ;
	float3 VTan ;
	float3 VBin ;
	float3 lLightTemp ;
	float lLightDistancePow2 ;
	float lLightGen ;
	float3 lLightDir ;
	float lLightDirectionCosA ;
	float3 ShadowRate ;
	float ShadowGen ;

#if SHADOWMAP
	float2 DepthTexCoord ;
	float4 TextureDepth ;
//	float4 variance_md_p_pmax ;
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
		Normal = ( tex2D( NormalMapTexture, PSInput.TexCoords0_1.xy ).rgb - cfZeroHalfOneTwo.y ) * cfZeroHalfOneTwo.w ;
	#else // BUMPMAP
		Normal = normalize( PSInput.VNormal ) ;
	#endif // BUMPMAP

	// �f�B�t���[�Y�e�N�X�`���J���[���擾
	TextureDiffuseColor = tex2D( DiffuseMapTexture, PSInput.TexCoords0_1.xy ) ;
	
#ifdef USE_SPE
	#ifdef USE_SPETEX
	// �f�X�y�L�����e�N�X�`���J���[���擾
	TextureSpecularColor = tex2D( SpecularMapTexture, PSInput.TexCoords0_1.xy ) ;
	#endif // USE_SPETEX
#endif // USE_SPE

#if SUBTEXTUREMODE != 0
	// �T�u�e�N�X�`���J���[���擾
	SubTextureColor = tex2D( SubTexture, PSInput.TexCoords0_1.zw );
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
	TotalDiffuse = cfZeroHalfOneTwo.x ;
#ifdef USE_SPE
	TotalSpecular = cfZeroHalfOneTwo.x ;
#endif // USE_SPE
	
#if TOON
	#if TOON == 1
		TotalLightDiffuse = cfZeroHalfOneTwo.x ;
	#else // TOON == 1
		TotalAmbient = cfZeroHalfOneTwo.x ;
		TotalAngleGen = cfZeroHalfOneTwo.x ;
	#endif
#endif // TOON

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
			// PSInput.ShadowMap2Pos_ShadowMap3PosX.xy �� -1.0f �` 1.0f �̒l�Ȃ̂ŁA����� 0.0f �` 1.0f �̒l�ɂ���
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
		    PSInput.Fog_ShadowMap3PosZ.y           <  0.0f || PSInput.Fog_ShadowMap3PosZ.y           > 1.0f )
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
			ShadowRate.z = smoothstep( PSInput.Fog_ShadowMap3PosZ.y - cfShadowMap2_Enbl1_2_ShadowMap3_DAdj_Grad.w, PSInput.Fog_ShadowMap3PosZ.y, TextureDepth.r + cfShadowMap2_Enbl1_2_ShadowMap3_DAdj_Grad.z ) ;
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
		lLightDir = cfLight[ 0 ].Direction ;

	#else  // LG0_DIR

		// ���C�g�����x�N�g���̌v�Z
		lLightDir = normalize( PSInput.VPosition.xyz - cfLight[ 0 ].Position.xyz ) ;

	#endif  // LG0_DIR
	

	
	// �����E�X�|�b�g���C�g�����l�v�Z =======================================( �J�n )
	#if LG0_SPOT || LG0_POINT

		// ���������v�Z ------------------

		// ���_�ƃ��C�g�ʒu�Ƃ̋����̓������߂�
		lLightTemp = PSInput.VPosition.xyz - cfLight[ 0 ].Position.xyz ;
		lLightDistancePow2 = dot( lLightTemp, lLightTemp ) ;

		// �������̌v�Z lLightGen = 1 / ( �����l0 + �����l1 * ���� + �����l2 * ( ���� * ���� ) )
		lLightGen = cfZeroHalfOneTwo.z / ( cfLight[ 0 ].Range_FallOff_AT0_AT1.z + cfLight[ 0 ].Range_FallOff_AT0_AT1.w * sqrt( lLightDistancePow2 ) + cfLight[ 0 ].AT2_SpotP0_SpotP1.x * lLightDistancePow2 ) ;

		// --------------------------------


		// �X�|�b�g���C�g�����v�Z --------
		#if LG0_SPOT

			// ���C�g�����x�N�g���ƃ��C�g�ʒu���璸�_�ʒu�ւ̃x�N�g���̓���( ���� Cos a )���v�Z 
			lLightDirectionCosA = dot( lLightDir, cfLight[ 0 ].Direction ) ;

			// �X�|�b�g���C�g�����v�Z  pow( falloff, ( ( Cos a - Cos f ) / ( Cos q - Cos f ) ) )
			lLightGen *= saturate( pow( abs( max( lLightDirectionCosA - cfLight[ 0 ].AT2_SpotP0_SpotP1.y, 0.0f ) * cfLight[ 0 ].AT2_SpotP0_SpotP1.z ), cfLight[ 0 ].Range_FallOff_AT0_AT1.y ) ) ;

		#endif // LG0_SPOT
		// --------------------------------


		// �L�������O�������猸�������ő�ɂ��鏈��
		lLightGen *= step( lLightDistancePow2, cfLight[ 0 ].Range_FallOff_AT0_AT1.x ) ;

	#else // LG0_SPOT || LG0_POINT
	
		lLightGen = 1.0f ;
		
	#endif // LG0_SPOT || LG0_POINT
	
	// �����E�X�|�b�g���C�g�����l�v�Z =======================================( �I�� )

	
	// �e�ɂ�錸�����Z�o
	ShadowGen = max( ShadowRate.x, SHADOWMAP1_ENABLE_LGT0 ) *
	            max( ShadowRate.y, SHADOWMAP2_ENABLE_LGT0 ) *
	            max( ShadowRate.z, SHADOWMAP3_ENABLE_LGT0 ) ;


	// �f�B�t���[�Y�F�v�Z

	// �f�B�t���[�Y�p�x�������v�Z
	DiffuseAngleGen = saturate( dot( Normal, -lLightDir ) ) ;
	
	#if TOON
		#if TOON == 1
			// �g�D�[���̏ꍇ�͂��̎��_�ł͒��_or�}�e���A���̃f�B�t���[�Y�J���[�͎g�p���Ȃ�
			// �f�B�t���[�Y�J���[�~�ϒl += ( ���C�g�̃f�B�t���[�Y�J���[ * �f�B�t���[�Y�J���[�p�x������ + ���C�g�̃A���r�G���g�J���[�ƃ}�e���A���̃A���r�G���g�J���[����Z��������  ) * �����E�X�|�b�g���C�g�̊p�x������
			TempF4 = cfLight[ 0 ].Diffuse * DiffuseAngleGen ;
			TotalLightDiffuse += TempF4 * lLightGen * ShadowGen ;
			TotalDiffuse += ( TempF4 + cfLight[ 0 ].Ambient ) * lLightGen ;

		#else // TOON == 1
			// �g�D�[���Q�̏ꍇ�̓f�B�t���[�Y�J���[�ƃ��C�g�̊p�x�����͕ʁX�ɕۑ����Ă���
			// �f�B�t���[�Y�J���[�~�ϒl += ���C�g�̃f�B�t���[�Y�J���[
			// ���C�g�̃A���r�G���g�J���[�̒~�ϒl += ���C�g�̃A���r�G���g�J���[
			// ���C�g�̊p�x�������̒~�ϒl += ���C�g�̊p�x������
			TotalDiffuse += cfLight[ 0 ].Diffuse ;

			TotalAmbient += cfLight[ 0 ].Ambient.rgb ;
			TotalAngleGen += DiffuseAngleGen * ShadowGen * lLightGen ;

		#endif // TOON == 1
	#else // TOON
	
		// �f�B�t���[�Y�J���[�~�ϒl += ( ���C�g�̃f�B�t���[�Y�J���[ * �f�B�t���[�Y�J���[ * �f�B�t���[�Y�J���[�p�x������ + ���C�g�̃A���r�G���g�J���[�ƃ}�e���A���̃A���r�G���g�J���[����Z��������  ) * �����E�X�|�b�g���C�g�̊p�x������
		TotalDiffuse += ( cfLight[ 0 ].Diffuse * PSInput.Diffuse * DiffuseAngleGen * ShadowGen + cfLight[ 0 ].Ambient ) * lLightGen ;

	#endif // TOON




	// �X�y�L�����J���[�v�Z
	#ifdef USE_SPE

		// �n�[�t�x�N�g���̌v�Z
		TempF3 = normalize( V_to_Eye - lLightDir ) ;
		
		// Temp = pow( max( 0.0f, N * H ), cfMaterial.Power.x )
		Temp = pow( max( cfZeroHalfOneTwo.x, dot( Normal, TempF3 ) ), cfMaterial.Power.x ) ;

		// �X�y�L�����J���[�~�ϒl += Temp * �����E�X�|�b�g���C�g�̊p�x������ * ���C�g�̃X�y�L�����J���[
		TotalSpecular += Temp * lLightGen * cfLight[ 0 ].Specular * ShadowGen ;

	#endif // USE_SPE


#endif // LG0_USE
// ���C�g0�̏��� +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �I�� )









	
// ���C�g1�̏��� +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �J�n )
#if LG1_USE

	#if LG1_DIR

		// ���C�g�̕����v�Z
		lLightDir = cfLight[ 1 ].Direction ;

	#else  // LG1_DIR

		// ���C�g�����x�N�g���̌v�Z
		lLightDir = normalize( PSInput.VPosition.xyz - cfLight[ 1 ].Position.xyz ) ;

	#endif  // LG1_DIR
	

	
	// �����E�X�|�b�g���C�g�����l�v�Z =======================================( �J�n )
	#if LG1_SPOT || LG1_POINT

		// ���������v�Z ------------------

		// ���_�ƃ��C�g�ʒu�Ƃ̋����̓������߂�
		lLightTemp = PSInput.VPosition.xyz - cfLight[ 1 ].Position.xyz ;
		lLightDistancePow2 = dot( lLightTemp, lLightTemp ) ;

		// �������̌v�Z lLightGen = 1 / ( �����l0 + �����l1 * ���� + �����l2 * ( ���� * ���� ) )
		lLightGen = cfZeroHalfOneTwo.z / ( cfLight[ 1 ].Range_FallOff_AT0_AT1.z + cfLight[ 1 ].Range_FallOff_AT0_AT1.w * sqrt( lLightDistancePow2 ) + cfLight[ 1 ].AT2_SpotP0_SpotP1.x * lLightDistancePow2 ) ;

		// --------------------------------


		// �X�|�b�g���C�g�����v�Z --------
		#if LG1_SPOT

			// ���C�g�����x�N�g���ƃ��C�g�ʒu���璸�_�ʒu�ւ̃x�N�g���̓���( ���� Cos a )���v�Z 
			lLightDirectionCosA = dot( lLightDir, cfLight[ 1 ].Direction ) ;

			// �X�|�b�g���C�g�����v�Z  pow( falloff, ( ( Cos a - Cos f ) / ( Cos q - Cos f ) ) )
			lLightGen *= saturate( pow( abs( max( lLightDirectionCosA - cfLight[ 1 ].AT2_SpotP0_SpotP1.y, 0.0f ) * cfLight[ 1 ].AT2_SpotP0_SpotP1.z ), cfLight[ 1 ].Range_FallOff_AT0_AT1.y ) ) ;

		#endif // LG1_SPOT
		// --------------------------------


		// �L�������O�������猸�������ő�ɂ��鏈��
		lLightGen *= step( lLightDistancePow2, cfLight[ 1 ].Range_FallOff_AT0_AT1.x ) ;

	#else // LG1_SPOT || LG1_POINT
	
		lLightGen = 1.0f ;
		
	#endif // LG1_SPOT || LG1_POINT
	
	// �����E�X�|�b�g���C�g�����l�v�Z =======================================( �I�� )

	
	// �e�ɂ�錸�����Z�o
	ShadowGen = max( ShadowRate.x, SHADOWMAP1_ENABLE_LGT1 ) *
	            max( ShadowRate.y, SHADOWMAP2_ENABLE_LGT1 ) *
	            max( ShadowRate.z, SHADOWMAP3_ENABLE_LGT1 ) ;


	// �f�B�t���[�Y�F�v�Z

	// �f�B�t���[�Y�p�x�������v�Z
	DiffuseAngleGen = saturate( dot( Normal, -lLightDir ) ) ;
	
	#if TOON
		#if TOON == 1
			// �g�D�[���̏ꍇ�͂��̎��_�ł͒��_or�}�e���A���̃f�B�t���[�Y�J���[�͎g�p���Ȃ�
			// �f�B�t���[�Y�J���[�~�ϒl += ( ���C�g�̃f�B�t���[�Y�J���[ * �f�B�t���[�Y�J���[�p�x������ + ���C�g�̃A���r�G���g�J���[�ƃ}�e���A���̃A���r�G���g�J���[����Z��������  ) * �����E�X�|�b�g���C�g�̊p�x������
			TempF4 = cfLight[ 1 ].Diffuse * DiffuseAngleGen ;
			TotalLightDiffuse += TempF4 * lLightGen * ShadowGen ;
			TotalDiffuse += ( TempF4 + cfLight[ 1 ].Ambient ) * lLightGen ;

		#else // TOON == 1
			// �g�D�[���Q�̏ꍇ�̓f�B�t���[�Y�J���[�ƃ��C�g�̊p�x�����͕ʁX�ɕۑ����Ă���
			// �f�B�t���[�Y�J���[�~�ϒl += ���C�g�̃f�B�t���[�Y�J���[
			// ���C�g�̃A���r�G���g�J���[�̒~�ϒl += ���C�g�̃A���r�G���g�J���[
			// ���C�g�̊p�x�������̒~�ϒl += ���C�g�̊p�x������
			TotalDiffuse += cfLight[ 1 ].Diffuse ;

			TotalAmbient += cfLight[ 1 ].Ambient.rgb ;
			TotalAngleGen += DiffuseAngleGen * ShadowGen * lLightGen ;

		#endif // TOON == 1
	#else // TOON
	
		// �f�B�t���[�Y�J���[�~�ϒl += ( ���C�g�̃f�B�t���[�Y�J���[ * �f�B�t���[�Y�J���[ * �f�B�t���[�Y�J���[�p�x������ + ���C�g�̃A���r�G���g�J���[�ƃ}�e���A���̃A���r�G���g�J���[����Z��������  ) * �����E�X�|�b�g���C�g�̊p�x������
		TotalDiffuse += ( cfLight[ 1 ].Diffuse * PSInput.Diffuse * DiffuseAngleGen * ShadowGen + cfLight[ 1 ].Ambient ) * lLightGen ;

	#endif // TOON




	// �X�y�L�����J���[�v�Z
	#ifdef USE_SPE

		// �n�[�t�x�N�g���̌v�Z
		TempF3 = normalize( V_to_Eye - lLightDir ) ;
		
		// Temp = pow( max( 0.0f, N * H ), cfMaterial.Power.x )
		Temp = pow( max( cfZeroHalfOneTwo.x, dot( Normal, TempF3 ) ), cfMaterial.Power.x ) ;

		// �X�y�L�����J���[�~�ϒl += Temp * �����E�X�|�b�g���C�g�̊p�x������ * ���C�g�̃X�y�L�����J���[
		TotalSpecular += Temp * lLightGen * cfLight[ 1 ].Specular * ShadowGen ;

	#endif // USE_SPE


#endif // LG1_USE
// ���C�g1�̏��� +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �I�� )









	
// ���C�g2�̏��� +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �J�n )
#if LG2_USE

	#if LG2_DIR

		// ���C�g�̕����v�Z
		lLightDir = cfLight[ 2 ].Direction ;

	#else  // LG2_DIR

		// ���C�g�����x�N�g���̌v�Z
		lLightDir = normalize( PSInput.VPosition.xyz - cfLight[ 2 ].Position.xyz ) ;

	#endif  // LG2_DIR
	

	
	// �����E�X�|�b�g���C�g�����l�v�Z =======================================( �J�n )
	#if LG2_SPOT || LG2_POINT

		// ���������v�Z ------------------

		// ���_�ƃ��C�g�ʒu�Ƃ̋����̓������߂�
		lLightTemp = PSInput.VPosition.xyz - cfLight[ 2 ].Position.xyz ;
		lLightDistancePow2 = dot( lLightTemp, lLightTemp ) ;

		// �������̌v�Z lLightGen = 1 / ( �����l0 + �����l1 * ���� + �����l2 * ( ���� * ���� ) )
		lLightGen = cfZeroHalfOneTwo.z / ( cfLight[ 2 ].Range_FallOff_AT0_AT1.z + cfLight[ 2 ].Range_FallOff_AT0_AT1.w * sqrt( lLightDistancePow2 ) + cfLight[ 2 ].AT2_SpotP0_SpotP1.x * lLightDistancePow2 ) ;

		// --------------------------------


		// �X�|�b�g���C�g�����v�Z --------
		#if LG2_SPOT

			// ���C�g�����x�N�g���ƃ��C�g�ʒu���璸�_�ʒu�ւ̃x�N�g���̓���( ���� Cos a )���v�Z 
			lLightDirectionCosA = dot( lLightDir, cfLight[ 2 ].Direction ) ;

			// �X�|�b�g���C�g�����v�Z  pow( falloff, ( ( Cos a - Cos f ) / ( Cos q - Cos f ) ) )
			lLightGen *= saturate( pow( abs( max( lLightDirectionCosA - cfLight[ 2 ].AT2_SpotP0_SpotP1.y, 0.0f ) * cfLight[ 2 ].AT2_SpotP0_SpotP1.z ), cfLight[ 2 ].Range_FallOff_AT0_AT1.y ) ) ;

		#endif // LG2_SPOT
		// --------------------------------


		// �L�������O�������猸�������ő�ɂ��鏈��
		lLightGen *= step( lLightDistancePow2, cfLight[ 2 ].Range_FallOff_AT0_AT1.x ) ;

	#else // LG2_SPOT || LG2_POINT
	
		lLightGen = 1.0f ;
		
	#endif // LG2_SPOT || LG2_POINT
	
	// �����E�X�|�b�g���C�g�����l�v�Z =======================================( �I�� )

	
	// �e�ɂ�錸�����Z�o
	ShadowGen = max( ShadowRate.x, SHADOWMAP1_ENABLE_LGT2 ) *
	            max( ShadowRate.y, SHADOWMAP2_ENABLE_LGT2 ) *
	            max( ShadowRate.z, SHADOWMAP3_ENABLE_LGT2 ) ;


	// �f�B�t���[�Y�F�v�Z

	// �f�B�t���[�Y�p�x�������v�Z
	DiffuseAngleGen = saturate( dot( Normal, -lLightDir ) ) ;
	
	#if TOON
		#if TOON == 1
			// �g�D�[���̏ꍇ�͂��̎��_�ł͒��_or�}�e���A���̃f�B�t���[�Y�J���[�͎g�p���Ȃ�
			// �f�B�t���[�Y�J���[�~�ϒl += ( ���C�g�̃f�B�t���[�Y�J���[ * �f�B�t���[�Y�J���[�p�x������ + ���C�g�̃A���r�G���g�J���[�ƃ}�e���A���̃A���r�G���g�J���[����Z��������  ) * �����E�X�|�b�g���C�g�̊p�x������
			TempF4 = cfLight[ 2 ].Diffuse * DiffuseAngleGen ;
			TotalLightDiffuse += TempF4 * lLightGen * ShadowGen ;
			TotalDiffuse += ( TempF4 + cfLight[ 2 ].Ambient ) * lLightGen ;

		#else // TOON == 1
			// �g�D�[���Q�̏ꍇ�̓f�B�t���[�Y�J���[�ƃ��C�g�̊p�x�����͕ʁX�ɕۑ����Ă���
			// �f�B�t���[�Y�J���[�~�ϒl += ���C�g�̃f�B�t���[�Y�J���[
			// ���C�g�̃A���r�G���g�J���[�̒~�ϒl += ���C�g�̃A���r�G���g�J���[
			// ���C�g�̊p�x�������̒~�ϒl += ���C�g�̊p�x������
			TotalDiffuse += cfLight[ 2 ].Diffuse ;

			TotalAmbient += cfLight[ 2 ].Ambient.rgb ;
			TotalAngleGen += DiffuseAngleGen * ShadowGen * lLightGen ;

		#endif // TOON == 1
	#else // TOON
	
		// �f�B�t���[�Y�J���[�~�ϒl += ( ���C�g�̃f�B�t���[�Y�J���[ * �f�B�t���[�Y�J���[ * �f�B�t���[�Y�J���[�p�x������ + ���C�g�̃A���r�G���g�J���[�ƃ}�e���A���̃A���r�G���g�J���[����Z��������  ) * �����E�X�|�b�g���C�g�̊p�x������
		TotalDiffuse += ( cfLight[ 2 ].Diffuse * PSInput.Diffuse * DiffuseAngleGen * ShadowGen + cfLight[ 2 ].Ambient ) * lLightGen ;

	#endif // TOON




	// �X�y�L�����J���[�v�Z
	#ifdef USE_SPE

		// �n�[�t�x�N�g���̌v�Z
		TempF3 = normalize( V_to_Eye - lLightDir ) ;
		
		// Temp = pow( max( 0.0f, N * H ), cfMaterial.Power.x )
		Temp = pow( max( cfZeroHalfOneTwo.x, dot( Normal, TempF3 ) ), cfMaterial.Power.x ) ;

		// �X�y�L�����J���[�~�ϒl += Temp * �����E�X�|�b�g���C�g�̊p�x������ * ���C�g�̃X�y�L�����J���[
		TotalSpecular += Temp * lLightGen * cfLight[ 2 ].Specular * ShadowGen ;

	#endif // USE_SPE


#endif // LG2_USE
// ���C�g2�̏��� +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �I�� )









	
// ���C�g3�̏��� +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �J�n )
#if LG3_USE

	#if LG3_DIR

		// ���C�g�̕����v�Z
		lLightDir = cfLight[ 3 ].Direction ;

	#else  // LG3_DIR

		// ���C�g�����x�N�g���̌v�Z
		lLightDir = normalize( PSInput.VPosition.xyz - cfLight[ 3 ].Position.xyz ) ;

	#endif  // LG3_DIR
	

	
	// �����E�X�|�b�g���C�g�����l�v�Z =======================================( �J�n )
	#if LG3_SPOT || LG3_POINT

		// ���������v�Z ------------------

		// ���_�ƃ��C�g�ʒu�Ƃ̋����̓������߂�
		lLightTemp = PSInput.VPosition.xyz - cfLight[ 3 ].Position.xyz ;
		lLightDistancePow2 = dot( lLightTemp, lLightTemp ) ;

		// �������̌v�Z lLightGen = 1 / ( �����l0 + �����l1 * ���� + �����l2 * ( ���� * ���� ) )
		lLightGen = cfZeroHalfOneTwo.z / ( cfLight[ 3 ].Range_FallOff_AT0_AT1.z + cfLight[ 3 ].Range_FallOff_AT0_AT1.w * sqrt( lLightDistancePow2 ) + cfLight[ 3 ].AT2_SpotP0_SpotP1.x * lLightDistancePow2 ) ;

		// --------------------------------


		// �X�|�b�g���C�g�����v�Z --------
		#if LG3_SPOT

			// ���C�g�����x�N�g���ƃ��C�g�ʒu���璸�_�ʒu�ւ̃x�N�g���̓���( ���� Cos a )���v�Z 
			lLightDirectionCosA = dot( lLightDir, cfLight[ 3 ].Direction ) ;

			// �X�|�b�g���C�g�����v�Z  pow( falloff, ( ( Cos a - Cos f ) / ( Cos q - Cos f ) ) )
			lLightGen *= saturate( pow( abs( max( lLightDirectionCosA - cfLight[ 3 ].AT2_SpotP0_SpotP1.y, 0.0f ) * cfLight[ 3 ].AT2_SpotP0_SpotP1.z ), cfLight[ 3 ].Range_FallOff_AT0_AT1.y ) ) ;

		#endif // LG3_SPOT
		// --------------------------------


		// �L�������O�������猸�������ő�ɂ��鏈��
		lLightGen *= step( lLightDistancePow2, cfLight[ 3 ].Range_FallOff_AT0_AT1.x ) ;

	#else // LG3_SPOT || LG3_POINT
	
		lLightGen = 1.0f ;
		
	#endif // LG3_SPOT || LG3_POINT
	
	// �����E�X�|�b�g���C�g�����l�v�Z =======================================( �I�� )

	
	// �e�ɂ�錸�����Z�o
	ShadowGen = max( ShadowRate.x, SHADOWMAP1_ENABLE_LGT3 ) *
	            max( ShadowRate.y, SHADOWMAP2_ENABLE_LGT3 ) *
	            max( ShadowRate.z, SHADOWMAP3_ENABLE_LGT3 ) ;


	// �f�B�t���[�Y�F�v�Z

	// �f�B�t���[�Y�p�x�������v�Z
	DiffuseAngleGen = saturate( dot( Normal, -lLightDir ) ) ;
	
	#if TOON
		#if TOON == 1
			// �g�D�[���̏ꍇ�͂��̎��_�ł͒��_or�}�e���A���̃f�B�t���[�Y�J���[�͎g�p���Ȃ�
			// �f�B�t���[�Y�J���[�~�ϒl += ( ���C�g�̃f�B�t���[�Y�J���[ * �f�B�t���[�Y�J���[�p�x������ + ���C�g�̃A���r�G���g�J���[�ƃ}�e���A���̃A���r�G���g�J���[����Z��������  ) * �����E�X�|�b�g���C�g�̊p�x������
			TempF4 = cfLight[ 3 ].Diffuse * DiffuseAngleGen ;
			TotalLightDiffuse += TempF4 * lLightGen * ShadowGen ;
			TotalDiffuse += ( TempF4 + cfLight[ 3 ].Ambient ) * lLightGen ;

		#else // TOON == 1
			// �g�D�[���Q�̏ꍇ�̓f�B�t���[�Y�J���[�ƃ��C�g�̊p�x�����͕ʁX�ɕۑ����Ă���
			// �f�B�t���[�Y�J���[�~�ϒl += ���C�g�̃f�B�t���[�Y�J���[
			// ���C�g�̃A���r�G���g�J���[�̒~�ϒl += ���C�g�̃A���r�G���g�J���[
			// ���C�g�̊p�x�������̒~�ϒl += ���C�g�̊p�x������
			TotalDiffuse += cfLight[ 3 ].Diffuse ;

			TotalAmbient += cfLight[ 3 ].Ambient.rgb ;
			TotalAngleGen += DiffuseAngleGen * ShadowGen * lLightGen ;

		#endif // TOON == 1
	#else // TOON
	
		// �f�B�t���[�Y�J���[�~�ϒl += ( ���C�g�̃f�B�t���[�Y�J���[ * �f�B�t���[�Y�J���[ * �f�B�t���[�Y�J���[�p�x������ + ���C�g�̃A���r�G���g�J���[�ƃ}�e���A���̃A���r�G���g�J���[����Z��������  ) * �����E�X�|�b�g���C�g�̊p�x������
		TotalDiffuse += ( cfLight[ 3 ].Diffuse * PSInput.Diffuse * DiffuseAngleGen * ShadowGen + cfLight[ 3 ].Ambient ) * lLightGen ;

	#endif // TOON




	// �X�y�L�����J���[�v�Z
	#ifdef USE_SPE

		// �n�[�t�x�N�g���̌v�Z
		TempF3 = normalize( V_to_Eye - lLightDir ) ;
		
		// Temp = pow( max( 0.0f, N * H ), cfMaterial.Power.x )
		Temp = pow( max( cfZeroHalfOneTwo.x, dot( Normal, TempF3 ) ), cfMaterial.Power.x ) ;

		// �X�y�L�����J���[�~�ϒl += Temp * �����E�X�|�b�g���C�g�̊p�x������ * ���C�g�̃X�y�L�����J���[
		TotalSpecular += Temp * lLightGen * cfLight[ 3 ].Specular * ShadowGen ;

	#endif // USE_SPE


#endif // LG3_USE
// ���C�g3�̏��� +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �I�� )









	
// ���C�g4�̏��� +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �J�n )
#if LG4_USE

	#if LG4_DIR

		// ���C�g�̕����v�Z
		lLightDir = cfLight[ 4 ].Direction ;

	#else  // LG4_DIR

		// ���C�g�����x�N�g���̌v�Z
		lLightDir = normalize( PSInput.VPosition.xyz - cfLight[ 4 ].Position.xyz ) ;

	#endif  // LG4_DIR
	

	
	// �����E�X�|�b�g���C�g�����l�v�Z =======================================( �J�n )
	#if LG4_SPOT || LG4_POINT

		// ���������v�Z ------------------

		// ���_�ƃ��C�g�ʒu�Ƃ̋����̓������߂�
		lLightTemp = PSInput.VPosition.xyz - cfLight[ 4 ].Position.xyz ;
		lLightDistancePow2 = dot( lLightTemp, lLightTemp ) ;

		// �������̌v�Z lLightGen = 1 / ( �����l0 + �����l1 * ���� + �����l2 * ( ���� * ���� ) )
		lLightGen = cfZeroHalfOneTwo.z / ( cfLight[ 4 ].Range_FallOff_AT0_AT1.z + cfLight[ 4 ].Range_FallOff_AT0_AT1.w * sqrt( lLightDistancePow2 ) + cfLight[ 4 ].AT2_SpotP0_SpotP1.x * lLightDistancePow2 ) ;

		// --------------------------------


		// �X�|�b�g���C�g�����v�Z --------
		#if LG4_SPOT

			// ���C�g�����x�N�g���ƃ��C�g�ʒu���璸�_�ʒu�ւ̃x�N�g���̓���( ���� Cos a )���v�Z 
			lLightDirectionCosA = dot( lLightDir, cfLight[ 4 ].Direction ) ;

			// �X�|�b�g���C�g�����v�Z  pow( falloff, ( ( Cos a - Cos f ) / ( Cos q - Cos f ) ) )
			lLightGen *= saturate( pow( abs( max( lLightDirectionCosA - cfLight[ 4 ].AT2_SpotP0_SpotP1.y, 0.0f ) * cfLight[ 4 ].AT2_SpotP0_SpotP1.z ), cfLight[ 4 ].Range_FallOff_AT0_AT1.y ) ) ;

		#endif // LG4_SPOT
		// --------------------------------


		// �L�������O�������猸�������ő�ɂ��鏈��
		lLightGen *= step( lLightDistancePow2, cfLight[ 4 ].Range_FallOff_AT0_AT1.x ) ;

	#else // LG4_SPOT || LG4_POINT
	
		lLightGen = 1.0f ;
		
	#endif // LG4_SPOT || LG4_POINT
	
	// �����E�X�|�b�g���C�g�����l�v�Z =======================================( �I�� )

	
	// �e�ɂ�錸�����Z�o
	ShadowGen = max( ShadowRate.x, SHADOWMAP1_ENABLE_LGT4 ) *
	            max( ShadowRate.y, SHADOWMAP2_ENABLE_LGT4 ) *
	            max( ShadowRate.z, SHADOWMAP3_ENABLE_LGT4 ) ;


	// �f�B�t���[�Y�F�v�Z

	// �f�B�t���[�Y�p�x�������v�Z
	DiffuseAngleGen = saturate( dot( Normal, -lLightDir ) ) ;
	
	#if TOON
		#if TOON == 1
			// �g�D�[���̏ꍇ�͂��̎��_�ł͒��_or�}�e���A���̃f�B�t���[�Y�J���[�͎g�p���Ȃ�
			// �f�B�t���[�Y�J���[�~�ϒl += ( ���C�g�̃f�B�t���[�Y�J���[ * �f�B�t���[�Y�J���[�p�x������ + ���C�g�̃A���r�G���g�J���[�ƃ}�e���A���̃A���r�G���g�J���[����Z��������  ) * �����E�X�|�b�g���C�g�̊p�x������
			TempF4 = cfLight[ 4 ].Diffuse * DiffuseAngleGen ;
			TotalLightDiffuse += TempF4 * lLightGen * ShadowGen ;
			TotalDiffuse += ( TempF4 + cfLight[ 4 ].Ambient ) * lLightGen ;

		#else // TOON == 1
			// �g�D�[���Q�̏ꍇ�̓f�B�t���[�Y�J���[�ƃ��C�g�̊p�x�����͕ʁX�ɕۑ����Ă���
			// �f�B�t���[�Y�J���[�~�ϒl += ���C�g�̃f�B�t���[�Y�J���[
			// ���C�g�̃A���r�G���g�J���[�̒~�ϒl += ���C�g�̃A���r�G���g�J���[
			// ���C�g�̊p�x�������̒~�ϒl += ���C�g�̊p�x������
			TotalDiffuse += cfLight[ 4 ].Diffuse ;

			TotalAmbient += cfLight[ 4 ].Ambient.rgb ;
			TotalAngleGen += DiffuseAngleGen * ShadowGen * lLightGen ;

		#endif // TOON == 1
	#else // TOON
	
		// �f�B�t���[�Y�J���[�~�ϒl += ( ���C�g�̃f�B�t���[�Y�J���[ * �f�B�t���[�Y�J���[ * �f�B�t���[�Y�J���[�p�x������ + ���C�g�̃A���r�G���g�J���[�ƃ}�e���A���̃A���r�G���g�J���[����Z��������  ) * �����E�X�|�b�g���C�g�̊p�x������
		TotalDiffuse += ( cfLight[ 4 ].Diffuse * PSInput.Diffuse * DiffuseAngleGen * ShadowGen + cfLight[ 4 ].Ambient ) * lLightGen ;

	#endif // TOON




	// �X�y�L�����J���[�v�Z
	#ifdef USE_SPE

		// �n�[�t�x�N�g���̌v�Z
		TempF3 = normalize( V_to_Eye - lLightDir ) ;
		
		// Temp = pow( max( 0.0f, N * H ), cfMaterial.Power.x )
		Temp = pow( max( cfZeroHalfOneTwo.x, dot( Normal, TempF3 ) ), cfMaterial.Power.x ) ;

		// �X�y�L�����J���[�~�ϒl += Temp * �����E�X�|�b�g���C�g�̊p�x������ * ���C�g�̃X�y�L�����J���[
		TotalSpecular += Temp * lLightGen * cfLight[ 4 ].Specular * ShadowGen ;

	#endif // USE_SPE


#endif // LG4_USE
// ���C�g4�̏��� +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �I�� )









	
// ���C�g5�̏��� +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �J�n )
#if LG5_USE

	#if LG5_DIR

		// ���C�g�̕����v�Z
		lLightDir = cfLight[ 5 ].Direction ;

	#else  // LG5_DIR

		// ���C�g�����x�N�g���̌v�Z
		lLightDir = normalize( PSInput.VPosition.xyz - cfLight[ 5 ].Position.xyz ) ;

	#endif  // LG5_DIR
	

	
	// �����E�X�|�b�g���C�g�����l�v�Z =======================================( �J�n )
	#if LG5_SPOT || LG5_POINT

		// ���������v�Z ------------------

		// ���_�ƃ��C�g�ʒu�Ƃ̋����̓������߂�
		lLightTemp = PSInput.VPosition.xyz - cfLight[ 5 ].Position.xyz ;
		lLightDistancePow2 = dot( lLightTemp, lLightTemp ) ;

		// �������̌v�Z lLightGen = 1 / ( �����l0 + �����l1 * ���� + �����l2 * ( ���� * ���� ) )
		lLightGen = cfZeroHalfOneTwo.z / ( cfLight[ 5 ].Range_FallOff_AT0_AT1.z + cfLight[ 5 ].Range_FallOff_AT0_AT1.w * sqrt( lLightDistancePow2 ) + cfLight[ 5 ].AT2_SpotP0_SpotP1.x * lLightDistancePow2 ) ;

		// --------------------------------


		// �X�|�b�g���C�g�����v�Z --------
		#if LG5_SPOT

			// ���C�g�����x�N�g���ƃ��C�g�ʒu���璸�_�ʒu�ւ̃x�N�g���̓���( ���� Cos a )���v�Z 
			lLightDirectionCosA = dot( lLightDir, cfLight[ 5 ].Direction ) ;

			// �X�|�b�g���C�g�����v�Z  pow( falloff, ( ( Cos a - Cos f ) / ( Cos q - Cos f ) ) )
			lLightGen *= saturate( pow( abs( max( lLightDirectionCosA - cfLight[ 5 ].AT2_SpotP0_SpotP1.y, 0.0f ) * cfLight[ 5 ].AT2_SpotP0_SpotP1.z ), cfLight[ 5 ].Range_FallOff_AT0_AT1.y ) ) ;

		#endif // LG5_SPOT
		// --------------------------------


		// �L�������O�������猸�������ő�ɂ��鏈��
		lLightGen *= step( lLightDistancePow2, cfLight[ 5 ].Range_FallOff_AT0_AT1.x ) ;

	#else // LG5_SPOT || LG5_POINT
	
		lLightGen = 1.0f ;
		
	#endif // LG5_SPOT || LG5_POINT
	
	// �����E�X�|�b�g���C�g�����l�v�Z =======================================( �I�� )

	
	// �e�ɂ�錸�����Z�o
	ShadowGen = max( ShadowRate.x, SHADOWMAP1_ENABLE_LGT5 ) *
	            max( ShadowRate.y, SHADOWMAP2_ENABLE_LGT5 ) *
	            max( ShadowRate.z, SHADOWMAP3_ENABLE_LGT5 ) ;


	// �f�B�t���[�Y�F�v�Z

	// �f�B�t���[�Y�p�x�������v�Z
	DiffuseAngleGen = saturate( dot( Normal, -lLightDir ) ) ;
	
	#if TOON
		#if TOON == 1
			// �g�D�[���̏ꍇ�͂��̎��_�ł͒��_or�}�e���A���̃f�B�t���[�Y�J���[�͎g�p���Ȃ�
			// �f�B�t���[�Y�J���[�~�ϒl += ( ���C�g�̃f�B�t���[�Y�J���[ * �f�B�t���[�Y�J���[�p�x������ + ���C�g�̃A���r�G���g�J���[�ƃ}�e���A���̃A���r�G���g�J���[����Z��������  ) * �����E�X�|�b�g���C�g�̊p�x������
			TempF4 = cfLight[ 5 ].Diffuse * DiffuseAngleGen ;
			TotalLightDiffuse += TempF4 * lLightGen * ShadowGen ;
			TotalDiffuse += ( TempF4 + cfLight[ 5 ].Ambient ) * lLightGen ;

		#else // TOON == 1
			// �g�D�[���Q�̏ꍇ�̓f�B�t���[�Y�J���[�ƃ��C�g�̊p�x�����͕ʁX�ɕۑ����Ă���
			// �f�B�t���[�Y�J���[�~�ϒl += ���C�g�̃f�B�t���[�Y�J���[
			// ���C�g�̃A���r�G���g�J���[�̒~�ϒl += ���C�g�̃A���r�G���g�J���[
			// ���C�g�̊p�x�������̒~�ϒl += ���C�g�̊p�x������
			TotalDiffuse += cfLight[ 5 ].Diffuse ;

			TotalAmbient += cfLight[ 5 ].Ambient.rgb ;
			TotalAngleGen += DiffuseAngleGen * ShadowGen * lLightGen ;

		#endif // TOON == 1
	#else // TOON
	
		// �f�B�t���[�Y�J���[�~�ϒl += ( ���C�g�̃f�B�t���[�Y�J���[ * �f�B�t���[�Y�J���[ * �f�B�t���[�Y�J���[�p�x������ + ���C�g�̃A���r�G���g�J���[�ƃ}�e���A���̃A���r�G���g�J���[����Z��������  ) * �����E�X�|�b�g���C�g�̊p�x������
		TotalDiffuse += ( cfLight[ 5 ].Diffuse * PSInput.Diffuse * DiffuseAngleGen * ShadowGen + cfLight[ 5 ].Ambient ) * lLightGen ;

	#endif // TOON




	// �X�y�L�����J���[�v�Z
	#ifdef USE_SPE

		// �n�[�t�x�N�g���̌v�Z
		TempF3 = normalize( V_to_Eye - lLightDir ) ;
		
		// Temp = pow( max( 0.0f, N * H ), cfMaterial.Power.x )
		Temp = pow( max( cfZeroHalfOneTwo.x, dot( Normal, TempF3 ) ), cfMaterial.Power.x ) ;

		// �X�y�L�����J���[�~�ϒl += Temp * �����E�X�|�b�g���C�g�̊p�x������ * ���C�g�̃X�y�L�����J���[
		TotalSpecular += Temp * lLightGen * cfLight[ 5 ].Specular * ShadowGen ;

	#endif // USE_SPE


#endif // LG5_USE
// ���C�g5�̏��� +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �I�� )















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

				PSOutput.Color0.a = TextureDiffuseColor.a * cfFactorColor.a * PSInput.Diffuse.a ;

				#if TOON_DIFOP_MUL
					PSOutput.Color0.a *= ToonDiffuseGradColor.a ;
				#endif // TOON_DIFOP_MUL
				
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
					PSOutput.Color0.a = TextureDiffuseColor.a * cfFactorColor.a * PSInput.Diffuse.a * ToonSphereMapColor.a ;

				#else // TOON_SPHEREOP_MUL

					// �o�̓A���t�@ = �e�N�X�`���A���t�@ * Factor�A���t�@ * ���_�A���t�@
					PSOutput.Color0.a = TextureDiffuseColor.a * cfFactorColor.a * PSInput.Diffuse.a ;

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
			PSOutput.Color0.a = TextureDiffuseColor.a * cfFactorColor.a * PSInput.Diffuse.a ;

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

				// �o�̓� = �e�N�X�`���� * �f�B�t���[�Y�� * ��惿
				PSOutput.Color0.a = TextureDiffuseColor.a * cfFactorColor.a * PSInput.Diffuse.a ;

				// ��Z�̏ꍇ�̓��l���O���f�[�V�������Ə�Z����
				#if TOON_DIFOP_MUL
					PSOutput.Color0.a *= ToonDiffuseGradColor.a ;
				#endif // TOON_DIFOP_MUL

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

					PSOutput.Color0.a = TextureDiffuseColor.a * cfFactorColor.a * PSInput.Diffuse.a * ToonSphereMapColor.a ;

				#else // TOON_SPHEREOP_MUL

					PSOutput.Color0.a = TextureDiffuseColor.a * cfFactorColor.a * PSInput.Diffuse.a ;

				#endif // TOON_SPHEREOP_MUL

			#endif		// TOON == 1
		#else  // TOON
			// �o�̓J���[ = TotalDiffuse * �e�N�X�`���J���[
			PSOutput.Color0.rgb = TotalDiffuse.rgb * TextureDiffuseColor.rgb ;

			// �o�̓� = �e�N�X�`���� * �f�B�t���[�Y�� * ��惿
			PSOutput.Color0.a = TextureDiffuseColor.a * cfFactorColor.a * PSInput.Diffuse.a ;
		#endif // TOON

	#endif // USE_SPE

	// �t�H�O����
	PSOutput.Color0.rgb = lerp( cfFogColor.rgb, PSOutput.Color0.rgb, saturate( PSInput.Fog_ShadowMap3PosZ.x ) );

	// �A���t�@��Z�J���[
	if( cfMulAlphaColor.x > 0.5f )
	{
		PSOutput.Color0.rgb *= PSOutput.Color0.a;
	}

	// �o�̓J���[�v�Z +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �I�� )


	// ���C�g���g���ꍇ *******************************************************************( �I�� )

	return PSOutput ;
}

