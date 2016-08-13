#include "DxVertexShaderFxCode.h"

// main�֐�
VS_OUTPUT main( VS_INPUT VSInput )
{
	VS_OUTPUT VSOutput ;
	int4 lBoneFloatIndex ;
	float4 lLocalWorldMatrix[ 3 ] ;
	float4 lWorldPosition ;
	float4 lViewPosition ;
	float3 lWorldNrm ;
	float3 lWorldTan ;
	float3 lWorldBin ;
	float3 lViewNrm ;
	float3 lViewTan ;
	float3 lViewBin ;
	float3 lBumpTemp ;
	float4 lTotalDiffuse ;
	float4 lTotalSpecular ;
	float3 lLightDir ;
	float lTotalGen ;
	float4 lLightTemp ;
	float lLightDistancePow2 ;
	float lLightGen ;
	float lLightDirectionCosA ;
	float3 lLightHalfVec ;
	float4 lLightLitParam ;
	float4 lLightLitDest ;
	float4 lDifColor;
#if USE_SPE
	float4 lSpeColor;
#endif
#if SHADOWMAP
	float4 lLViewPosition ;
#endif

#if ( ( BUMPMAP || PHONG ) && USE_SPE ) || SHADOWMAP
	VSOutput.V_to_Eye_ShadowMap3PosZ.xyzw = cfZeroOne.x;
#endif
	
	lDifColor = cfDifSpeSource.x > 0.5f ? VSInput.Diffuse : cfMaterial.Diffuse;
	
#if USE_SPE
	lSpeColor = cfDifSpeSource.y > 0.5f ? VSInput.Specular : cfMaterial.Specular;
#endif

#if ( BUMPMAP == 0 && PHONG ) || SM_3
	VSOutput.Normal_Fog = float4( 0.0f, 0.0f, 0.0f, 1.0f ) ;
#endif

	// ���_���W�ϊ� ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �J�n )

	#ifdef SKINMESH

		// �X�L�����b�V��

		// �u�����h�s��̍쐬
		lBoneFloatIndex = VSInput.BlendIndices0 ;
		lLocalWorldMatrix[ 0 ]  = cfLocalWorldMatrix[ lBoneFloatIndex.x + 0 ] * VSInput.BlendWeight0.xxxx;
		lLocalWorldMatrix[ 1 ]  = cfLocalWorldMatrix[ lBoneFloatIndex.x + 1 ] * VSInput.BlendWeight0.xxxx;
		lLocalWorldMatrix[ 2 ]  = cfLocalWorldMatrix[ lBoneFloatIndex.x + 2 ] * VSInput.BlendWeight0.xxxx;

		lLocalWorldMatrix[ 0 ] += cfLocalWorldMatrix[ lBoneFloatIndex.y + 0 ] * VSInput.BlendWeight0.yyyy;
		lLocalWorldMatrix[ 1 ] += cfLocalWorldMatrix[ lBoneFloatIndex.y + 1 ] * VSInput.BlendWeight0.yyyy;
		lLocalWorldMatrix[ 2 ] += cfLocalWorldMatrix[ lBoneFloatIndex.y + 2 ] * VSInput.BlendWeight0.yyyy;

		lLocalWorldMatrix[ 0 ] += cfLocalWorldMatrix[ lBoneFloatIndex.z + 0 ] * VSInput.BlendWeight0.zzzz;
		lLocalWorldMatrix[ 1 ] += cfLocalWorldMatrix[ lBoneFloatIndex.z + 1 ] * VSInput.BlendWeight0.zzzz;
		lLocalWorldMatrix[ 2 ] += cfLocalWorldMatrix[ lBoneFloatIndex.z + 2 ] * VSInput.BlendWeight0.zzzz;

		lLocalWorldMatrix[ 0 ] += cfLocalWorldMatrix[ lBoneFloatIndex.w + 0 ] * VSInput.BlendWeight0.wwww;
		lLocalWorldMatrix[ 1 ] += cfLocalWorldMatrix[ lBoneFloatIndex.w + 1 ] * VSInput.BlendWeight0.wwww;
		lLocalWorldMatrix[ 2 ] += cfLocalWorldMatrix[ lBoneFloatIndex.w + 2 ] * VSInput.BlendWeight0.wwww;

		#ifdef BONE8

			lBoneFloatIndex = VSInput.BlendIndices1 ;
			lLocalWorldMatrix[ 0 ] += cfLocalWorldMatrix[ lBoneFloatIndex.x + 0 ] * VSInput.BlendWeight1.xxxx;
			lLocalWorldMatrix[ 1 ] += cfLocalWorldMatrix[ lBoneFloatIndex.x + 1 ] * VSInput.BlendWeight1.xxxx;
			lLocalWorldMatrix[ 2 ] += cfLocalWorldMatrix[ lBoneFloatIndex.x + 2 ] * VSInput.BlendWeight1.xxxx;

			lLocalWorldMatrix[ 0 ] += cfLocalWorldMatrix[ lBoneFloatIndex.y + 0 ] * VSInput.BlendWeight1.yyyy;
			lLocalWorldMatrix[ 1 ] += cfLocalWorldMatrix[ lBoneFloatIndex.y + 1 ] * VSInput.BlendWeight1.yyyy;
			lLocalWorldMatrix[ 2 ] += cfLocalWorldMatrix[ lBoneFloatIndex.y + 2 ] * VSInput.BlendWeight1.yyyy;

			lLocalWorldMatrix[ 0 ] += cfLocalWorldMatrix[ lBoneFloatIndex.z + 0 ] * VSInput.BlendWeight1.zzzz;
			lLocalWorldMatrix[ 1 ] += cfLocalWorldMatrix[ lBoneFloatIndex.z + 1 ] * VSInput.BlendWeight1.zzzz;
			lLocalWorldMatrix[ 2 ] += cfLocalWorldMatrix[ lBoneFloatIndex.z + 2 ] * VSInput.BlendWeight1.zzzz;

			lLocalWorldMatrix[ 0 ] += cfLocalWorldMatrix[ lBoneFloatIndex.w + 0 ] * VSInput.BlendWeight1.wwww;
			lLocalWorldMatrix[ 1 ] += cfLocalWorldMatrix[ lBoneFloatIndex.w + 1 ] * VSInput.BlendWeight1.wwww;
			lLocalWorldMatrix[ 2 ] += cfLocalWorldMatrix[ lBoneFloatIndex.w + 2 ] * VSInput.BlendWeight1.wwww;

		#endif // BONE8

	#endif	// SKINMESH

	#ifdef TOONOUTLINE

		// ���W�v�Z( ���[�J�������[���h )
		lWorldPosition.x = dot( VSInput.Position, LOCAL_WORLD_MAT[ 0 ] ) ;
		lWorldPosition.y = dot( VSInput.Position, LOCAL_WORLD_MAT[ 1 ] ) ;
		lWorldPosition.z = dot( VSInput.Position, LOCAL_WORLD_MAT[ 2 ] ) ;
		lWorldPosition.w = 1.0f ;

		lWorldNrm.x = dot( VSInput.Normal, LOCAL_WORLD_MAT[ 0 ].xyz ) ;
		lWorldNrm.y = dot( VSInput.Normal, LOCAL_WORLD_MAT[ 1 ].xyz ) ;
		lWorldNrm.z = dot( VSInput.Normal, LOCAL_WORLD_MAT[ 2 ].xyz ) ;

		// �֊s���̑������@�������Ɉړ�����
		lWorldPosition.xyz += lWorldNrm * cfToonOutLineSize.x ;

		// ���[���h���r���[
		lViewPosition.x = dot( lWorldPosition, cfViewMatrix[ 0 ] ) ;
		lViewPosition.y = dot( lWorldPosition, cfViewMatrix[ 1 ] ) ;
		lViewPosition.z = dot( lWorldPosition, cfViewMatrix[ 2 ] ) ;
		lViewPosition.w = 1.0f ;

		// �r���[���v���W�F�N�V����
		VSOutput.Position.x = dot( lViewPosition, cfProjectionMatrix[ 0 ] ) ;
		VSOutput.Position.y = dot( lViewPosition, cfProjectionMatrix[ 1 ] ) ;
		VSOutput.Position.z = dot( lViewPosition, cfProjectionMatrix[ 2 ] ) ;
		VSOutput.Position.w = dot( lViewPosition, cfProjectionMatrix[ 3 ] ) ;

	#else  // TOONOUTLINE

		// ���W�v�Z( ���[�J�����r���[���v���W�F�N�V���� )
		lWorldPosition.x = dot( VSInput.Position, LOCAL_WORLD_MAT[ 0 ] ) ;
		lWorldPosition.y = dot( VSInput.Position, LOCAL_WORLD_MAT[ 1 ] ) ;
		lWorldPosition.z = dot( VSInput.Position, LOCAL_WORLD_MAT[ 2 ] ) ;
		lWorldPosition.w = 1.0f ;

		lViewPosition.x = dot( lWorldPosition, cfViewMatrix[ 0 ] ) ;
		lViewPosition.y = dot( lWorldPosition, cfViewMatrix[ 1 ] ) ;
		lViewPosition.z = dot( lWorldPosition, cfViewMatrix[ 2 ] ) ;
		lViewPosition.w = 1.0f ;

		VSOutput.Position.x = dot( lViewPosition, cfProjectionMatrix[ 0 ] ) ;
		VSOutput.Position.y = dot( lViewPosition, cfProjectionMatrix[ 1 ] ) ;
		VSOutput.Position.z = dot( lViewPosition, cfProjectionMatrix[ 2 ] ) ;
		VSOutput.Position.w = dot( lViewPosition, cfProjectionMatrix[ 3 ] ) ;

	#endif // TOONOUTLINE

	#ifdef BUMPMAP

		// �]�@���A�ڐ��A�@�����r���[��Ԃɓ��e����
		lWorldTan.x = dot( VSInput.Tan, LOCAL_WORLD_MAT[ 0 ].xyz ) ;
		lWorldTan.y = dot( VSInput.Tan, LOCAL_WORLD_MAT[ 1 ].xyz ) ;
		lWorldTan.z = dot( VSInput.Tan, LOCAL_WORLD_MAT[ 2 ].xyz ) ;

		lWorldBin.x = dot( VSInput.Bin, LOCAL_WORLD_MAT[ 0 ].xyz ) ;
		lWorldBin.y = dot( VSInput.Bin, LOCAL_WORLD_MAT[ 1 ].xyz ) ;
		lWorldBin.z = dot( VSInput.Bin, LOCAL_WORLD_MAT[ 2 ].xyz ) ;

		lWorldNrm.x = dot( VSInput.Normal, LOCAL_WORLD_MAT[ 0 ].xyz ) ;
		lWorldNrm.y = dot( VSInput.Normal, LOCAL_WORLD_MAT[ 1 ].xyz ) ;
		lWorldNrm.z = dot( VSInput.Normal, LOCAL_WORLD_MAT[ 2 ].xyz ) ;

		lViewTan.x = dot( lWorldTan, cfViewMatrix[ 0 ].xyz ) ;
		lViewTan.y = dot( lWorldTan, cfViewMatrix[ 1 ].xyz ) ;
		lViewTan.z = dot( lWorldTan, cfViewMatrix[ 2 ].xyz ) ;

		lViewBin.x = dot( lWorldBin, cfViewMatrix[ 0 ].xyz ) ;
		lViewBin.y = dot( lWorldBin, cfViewMatrix[ 1 ].xyz ) ;
		lViewBin.z = dot( lWorldBin, cfViewMatrix[ 2 ].xyz ) ;

		lViewNrm.x = dot( lWorldNrm, cfViewMatrix[ 0 ].xyz ) ;
		lViewNrm.y = dot( lWorldNrm, cfViewMatrix[ 1 ].xyz ) ;
		lViewNrm.z = dot( lWorldNrm, cfViewMatrix[ 2 ].xyz ) ;

	#endif	// BUMPMAP


	// ���_���W�ϊ� ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �I�� )

	#if LG_USE == 0 || BUMPMAP == 1 || PHONG == 1

		// ���C�g���g�p���Ȃ����A�o���v�}�b�v���̓t�H���V�F�[�f�B���O�̏ꍇ�̓}�e���A��( or ���_ )�J���[�����̂܂܏o��
		VSOutput.Diffuse  = lDifColor ;

		#if USE_SPE
			VSOutput.Specular = lSpeColor ;
		#endif
		
		#if BUMPMAP == 0 && PHONG
		
			// �@�����r���[��Ԃ̊p�x�ɕϊ�( LOCAL_WORLD_MAT �̓X�L�j���O���b�V�� �̏ꍇ�� r0 ���g���̂� mov r0, ZERO ����Ɏ��s����K�v������ )
			lWorldNrm.x = dot( VSInput.Normal, LOCAL_WORLD_MAT[ 0 ].xyz ) ;
			lWorldNrm.y = dot( VSInput.Normal, LOCAL_WORLD_MAT[ 1 ].xyz ) ;
			lWorldNrm.z = dot( VSInput.Normal, LOCAL_WORLD_MAT[ 2 ].xyz ) ;

			lViewNrm.x = dot( lWorldNrm, cfViewMatrix[ 0 ].xyz ) ;
			lViewNrm.y = dot( lWorldNrm, cfViewMatrix[ 1 ].xyz ) ;
			lViewNrm.z = dot( lWorldNrm, cfViewMatrix[ 2 ].xyz ) ;

			// �@����ۑ�
			VSOutput.Normal_Fog.xyz = lViewNrm ;

		#endif // PHONG

	#endif // LG_USE == 0 || BUMPMAP == 1 || PHONG == 1





	// ���C�g�̏��� +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �J�n )
	#if LG_USE

		#ifdef BUMPMAP

			#if USE_SPE
				// ���_���W���王�_�ւ̃x�N�g����ڒ��Ԃɓ��e�����㐳�K�����ĕۑ�
				lBumpTemp.x = dot( lViewTan, -lViewPosition.xyz ) ;
				lBumpTemp.y = dot( lViewBin, -lViewPosition.xyz ) ;
				lBumpTemp.z = dot( lViewNrm, -lViewPosition.xyz ) ;
				VSOutput.V_to_Eye_ShadowMap3PosZ.xyz = normalize( lBumpTemp ) ;
			#endif

		#else // BUMPMAP 

			// �@�����r���[��Ԃ̊p�x�ɕϊ�( LOCAL_WORLD_MAT �̓X�L�j���O���b�V�� �̏ꍇ�� r0 ���g���̂� mov r0, ZERO ����Ɏ��s����K�v������ )
			lWorldNrm.x = dot( VSInput.Normal, LOCAL_WORLD_MAT[ 0 ].xyz ) ;
			lWorldNrm.y = dot( VSInput.Normal, LOCAL_WORLD_MAT[ 1 ].xyz ) ;
			lWorldNrm.z = dot( VSInput.Normal, LOCAL_WORLD_MAT[ 2 ].xyz ) ;

			lViewNrm.x = dot( lWorldNrm, cfViewMatrix[ 0 ].xyz ) ;
			lViewNrm.y = dot( lWorldNrm, cfViewMatrix[ 1 ].xyz ) ;
			lViewNrm.z = dot( lWorldNrm, cfViewMatrix[ 2 ].xyz ) ;

			// �f�B�t���[�Y�J���[�ƃX�y�L�����J���[�̒~�ϒl�̏�����
			lTotalDiffuse = float4( 0, 0, 0, 0 ) ;
			lTotalSpecular = float4( 0, 0, 0, 0 ) ;

			#ifdef PHONG

				#if USE_SPE
					// ���_���W���王�_�ւ̃x�N�g���𐳋K�����ĕۑ�
					VSOutput.V_to_Eye_ShadowMap3PosZ.xyz = normalize( -lViewPosition.xyz ) ;
				#endif

				// �@����ۑ�
				VSOutput.Normal_Fog.xyz = lViewNrm ;

			#endif // PHONG

		#endif // BUMPMAP







		// ���C�g0�̏��� ***********************************************************************************************************************************( �J�n )
#if LG0_USE

	#ifdef ALL_VS
		if( cbLightBool[ 0 ].Use )
		{
	#endif

	// �f�B�t���[�Y�J���[�p�x�����v�Z�p�̃��C�g�̕��������O�v�Z ======( �J�n )

	#if LG0_DIR

		// ���C�g�̕����v�Z
		lLightDir = cfLight[ 0 ].Direction ;

	#else  // LG0_DIR

		// ���C�g�����x�N�g���̌v�Z
		lLightDir = normalize( lViewPosition.xyz - cfLight[ 0 ].Position.xyz ) ;

	#endif  // LG0_DIR

	// ���C�g�̏���������
	VSOutput.Light0_Dir_Gen = ZERO ;

	// ���C�g�̌����̋t�x�N�g����ڒn��Ԃɕϊ�
	#ifdef BUMPMAP

		VSOutput.Light0_Dir_Gen.x = dot( lViewTan, -lLightDir ) ;
		VSOutput.Light0_Dir_Gen.y = dot( lViewBin, -lLightDir ) ;
		VSOutput.Light0_Dir_Gen.z = dot( lViewNrm, -lLightDir ) ;

	#elif PHONG // BUMPMAP

		// ���C�g�̌����̋t�x�N�g����ۑ�
		VSOutput.Light0_Dir_Gen.xyz = -lLightDir ;

	#endif // BUMPMAP

	// �f�B�t���[�Y�J���[�p�x�����v�Z�p�̃��C�g�̕��������O�v�Z ======( �I�� )


	// �����E�X�|�b�g���C�g�����l�v�Z ==========================( �J�n )
	#if LG0_SPOT || LG0_POINT

		#ifdef ALL_VS
			lTotalGen = 1.0f;
			if( cbLightBool[ 0 ].Point_Or_Spot )
			{
		#endif // ALL_VS

		// ���������v�Z ------------------

		// ���_�ƃ��C�g�ʒu�Ƃ̋����̓������߂� lLightTemp.x = ( POS - L0_POS ) * ( POS - L0_POS )
		lLightTemp = lViewPosition - cfLight[ 0 ].Position ;
		lLightTemp.w = 0.0f;
		lLightDistancePow2 = dot( lLightTemp, lLightTemp ) ;

		// �������̌v�Z lLightGen = 1 / ( �����l0 + �����l1 * ���� + �����l2 * ( ���� * ���� ) )
		lLightGen = 1.0f / ( cfLight[ 0 ].Range_FallOff_AT0_AT1.z + cfLight[ 0 ].Range_FallOff_AT0_AT1.w * sqrt( lLightDistancePow2 ) + cfLight[ 0 ].AT2_SpotP0_SpotP1.x * lLightDistancePow2 ) ;

		// �X�|�b�g���C�g�����v�Z --------

		#if LG0_SPOT

			#ifdef ALL_VS
				if( cbLightBool[ 0 ].Spot )
				{
			#endif // ALL_VS

			// ���C�g�����x�N�g���ƃ��C�g�ʒu���璸�_�ʒu�ւ̃x�N�g���̓���( ���� Cos a )���v�Z 
			lLightDirectionCosA = dot( lLightDir, cfLight[ 0 ].Direction ) ;

			// �X�|�b�g���C�g�����v�Z  pow( falloff, ( ( Cos a - Cos f ) / ( Cos q - Cos f ) ) )
			lLightGen *= saturate( pow( abs( max( lLightDirectionCosA - cfLight[ 0 ].AT2_SpotP0_SpotP1.y, 0.0f ) * cfLight[ 0 ].AT2_SpotP0_SpotP1.z ), cfLight[ 0 ].Range_FallOff_AT0_AT1.y ) ) ;

			#ifdef ALL_VS
				} // if( cbLightBool[ 0 ].Spot )
			#endif

		#endif // LG0_SPOT

		// �L�������O�������猸�������ő�ɂ��鏈��
		lLightGen *= step( lLightDistancePow2, cfLight[ 0 ].Range_FallOff_AT0_AT1.x ) ;

		// �o���v�}�b�v or �t�H���V�F�[�f�B���O�p�Ɍ�������ۑ�����
		#if PHONG || BUMPMAP
			VSOutput.Light0_Dir_Gen.w = lLightGen ;
		#endif // PHONG || BUMPMAP

		#ifdef ALL_VS
			} // if( cbLightBool[ 0 ].Point_Or_Spot )
		#endif

	#endif // LG0_SPOT || LG0_POINT
	// �����E�X�|�b�g���C�g�����l�v�Z =========================( �I�� )




	// �f�B�t���[�Y���C�g�ƃX�y�L�������C�g�̊p�x�����v�Z =======( �J�n )
	#if BUMPMAP == 0 && PHONG == 0

		// �@���ƃ��C�g�̋t�����x�N�g���Ƃ̓��ς� lLightLitParam.x �ɃZ�b�g
		lLightLitParam.x = dot( lViewNrm, -lLightDir ) ;

		#ifdef USE_SPE

			// �n�[�t�x�N�g���̌v�Z norm( ( norm( ���_�ʒu���王�_�ւ̃x�N�g�� ) + ���C�g�̕��� ) )
			lLightHalfVec = normalize( normalize( -lViewPosition.xyz ) - lLightDir ) ;

			// �@���ƃn�[�t�x�N�g���̓��ς� lLightLitParam.y �ɃZ�b�g
			lLightLitParam.y = dot( lLightHalfVec, lViewNrm ) ;

			// �X�y�L�������˗��� lLightLitParam.w �ɃZ�b�g
			lLightLitParam.w = cfMaterial.Power.x ;

			// ���C�g�v�Z
			lLightLitDest = lit( lLightLitParam.x, lLightLitParam.y, lLightLitParam.w ) ;

		#else // USE_SPE

			lLightLitDest.y = max( lLightLitParam.x, 0.0f ) ;

		#endif // USE_SPE

	#endif // BUMPMAP == 0 && PHONG == 0
	// �f�B�t���[�Y���C�g�ƃX�y�L�������C�g�̊p�x�����v�Z =======( �I�� )





	// �f�B�t���[�Y�J���[�v�Z =======================================( �J�n )

	#if BUMPMAP == 0 && PHONG == 0

		#if LG0_SPOT || LG0_POINT

			// �f�B�t���[�Y���C�g�~�ϒl += �����E�X�|�b�g���C�g�p�x�����l * ( �f�B�t���[�Y�p�x�����v�Z���� * �f�B�t���[�Y�J���[ * ���C�g�̃f�B�t���[�Y�J���[ + ���C�g�̃A���r�G���g�J���[�ƃ}�e���A���̃A���r�G���g�J���[����Z�������� )
			lTotalDiffuse += lLightGen * ( lLightLitDest.y * cfLight[ 0 ].Diffuse * lDifColor + cfLight[ 0 ].Ambient ) ;

		#else  // LG0_SPOT || LG0_POINT

			// �f�B�t���[�Y���C�g�~�ϒl += �f�B�t���[�Y�p�x�����v�Z���� * �f�B�t���[�Y�J���[ * ���C�g�̃f�B�t���[�Y�J���[ + ���C�g�̃A���r�G���g�J���[�ƃ}�e���A���̃A���r�G���g�J���[����Z��������
			lTotalDiffuse += lLightLitDest.y * cfLight[ 0 ].Diffuse * lDifColor + cfLight[ 0 ].Ambient ;

		#endif // LG0_SPOT || LG0_POINT

	#endif // BUMPMAP == 0 && PHONG == 0

	// �f�B�t���[�Y�J���[�v�Z =======================================( �I�� )





	// �X�y�L�����J���[�v�Z =======================================( �J�n )

	#ifdef USE_SPE

		#if BUMPMAP == 0 && PHONG == 0

			#if LG0_SPOT || LG0_POINT || ALL_VS

				// �X�y�L�������C�g�~�ϒl += �X�y�L�����p�x�����v�Z���� * �����E�X�|�b�g���C�g���� * ���C�g�̃X�y�L�����J���[
				lTotalSpecular += lLightGen * lLightLitDest.z * cfLight[ 0 ].Specular ;

			#else	// LG0_SPOT || LG0_POINT || ALL_VS

				// �X�y�L�������C�g�~�ϒl += �X�y�L�����p�x�����v�Z���� * ���C�g�̃X�y�L�����J���[
				lTotalSpecular += lLightLitDest.z * cfLight[ 0 ].Specular ;

			#endif	// LG0_SPOT || LG0_POINT || ALL_VS

		#endif // BUMPMAP == 0 && PHONG == 0

	#endif // USE_SPE

	// �X�y�L�����J���[�v�Z =======================================( �I�� )




	#ifdef ALL_VS
		} // if( cbLightBool[ 0 ].Use )
	#endif

#endif  // LG0_USE
// ���C�g0�̏��� ***********************************************************************************************************************************( �I�� )








		// ���C�g1�̏��� ***********************************************************************************************************************************( �J�n )
#if LG1_USE

	#ifdef ALL_VS
		if( cbLightBool[ 1 ].Use )
		{
	#endif

	// �f�B�t���[�Y�J���[�p�x�����v�Z�p�̃��C�g�̕��������O�v�Z ======( �J�n )

	#if LG1_DIR

		// ���C�g�̕����v�Z
		lLightDir = cfLight[ 1 ].Direction ;

	#else  // LG1_DIR

		// ���C�g�����x�N�g���̌v�Z
		lLightDir = normalize( lViewPosition.xyz - cfLight[ 1 ].Position.xyz ) ;

	#endif  // LG1_DIR

	// ���C�g�̏���������
	VSOutput.Light1_Dir_Gen = ZERO ;

	// ���C�g�̌����̋t�x�N�g����ڒn��Ԃɕϊ�
	#ifdef BUMPMAP

		VSOutput.Light1_Dir_Gen.x = dot( lViewTan, -lLightDir ) ;
		VSOutput.Light1_Dir_Gen.y = dot( lViewBin, -lLightDir ) ;
		VSOutput.Light1_Dir_Gen.z = dot( lViewNrm, -lLightDir ) ;

	#elif PHONG // BUMPMAP

		// ���C�g�̌����̋t�x�N�g����ۑ�
		VSOutput.Light1_Dir_Gen.xyz = -lLightDir ;

	#endif // BUMPMAP

	// �f�B�t���[�Y�J���[�p�x�����v�Z�p�̃��C�g�̕��������O�v�Z ======( �I�� )


	// �����E�X�|�b�g���C�g�����l�v�Z ==========================( �J�n )
	#if LG1_SPOT || LG1_POINT

		#ifdef ALL_VS
			lTotalGen = 1.0f;
			if( cbLightBool[ 1 ].Point_Or_Spot )
			{
		#endif // ALL_VS

		// ���������v�Z ------------------

		// ���_�ƃ��C�g�ʒu�Ƃ̋����̓������߂� lLightTemp.x = ( POS - L1_POS ) * ( POS - L1_POS )
		lLightTemp = lViewPosition - cfLight[ 1 ].Position ;
		lLightTemp.w = 0.0f;
		lLightDistancePow2 = dot( lLightTemp, lLightTemp ) ;

		// �������̌v�Z lLightGen = 1 / ( �����l0 + �����l1 * ���� + �����l2 * ( ���� * ���� ) )
		lLightGen = 1.0f / ( cfLight[ 1 ].Range_FallOff_AT0_AT1.z + cfLight[ 1 ].Range_FallOff_AT0_AT1.w * sqrt( lLightDistancePow2 ) + cfLight[ 1 ].AT2_SpotP0_SpotP1.x * lLightDistancePow2 ) ;

		// �X�|�b�g���C�g�����v�Z --------

		#if LG1_SPOT

			#ifdef ALL_VS
				if( cbLightBool[ 1 ].Spot )
				{
			#endif // ALL_VS

			// ���C�g�����x�N�g���ƃ��C�g�ʒu���璸�_�ʒu�ւ̃x�N�g���̓���( ���� Cos a )���v�Z 
			lLightDirectionCosA = dot( lLightDir, cfLight[ 1 ].Direction ) ;

			// �X�|�b�g���C�g�����v�Z  pow( falloff, ( ( Cos a - Cos f ) / ( Cos q - Cos f ) ) )
			lLightGen *= saturate( pow( abs( max( lLightDirectionCosA - cfLight[ 1 ].AT2_SpotP0_SpotP1.y, 0.0f ) * cfLight[ 1 ].AT2_SpotP0_SpotP1.z ), cfLight[ 1 ].Range_FallOff_AT0_AT1.y ) ) ;

			#ifdef ALL_VS
				} // if( cbLightBool[ 1 ].Spot )
			#endif

		#endif // LG1_SPOT

		// �L�������O�������猸�������ő�ɂ��鏈��
		lLightGen *= step( lLightDistancePow2, cfLight[ 1 ].Range_FallOff_AT0_AT1.x ) ;

		// �o���v�}�b�v or �t�H���V�F�[�f�B���O�p�Ɍ�������ۑ�����
		#if PHONG || BUMPMAP
			VSOutput.Light1_Dir_Gen.w = lLightGen ;
		#endif // PHONG || BUMPMAP

		#ifdef ALL_VS
			} // if( cbLightBool[ 1 ].Point_Or_Spot )
		#endif

	#endif // LG1_SPOT || LG1_POINT
	// �����E�X�|�b�g���C�g�����l�v�Z =========================( �I�� )




	// �f�B�t���[�Y���C�g�ƃX�y�L�������C�g�̊p�x�����v�Z =======( �J�n )
	#if BUMPMAP == 0 && PHONG == 0

		// �@���ƃ��C�g�̋t�����x�N�g���Ƃ̓��ς� lLightLitParam.x �ɃZ�b�g
		lLightLitParam.x = dot( lViewNrm, -lLightDir ) ;

		#ifdef USE_SPE

			// �n�[�t�x�N�g���̌v�Z norm( ( norm( ���_�ʒu���王�_�ւ̃x�N�g�� ) + ���C�g�̕��� ) )
			lLightHalfVec = normalize( normalize( -lViewPosition.xyz ) - lLightDir ) ;

			// �@���ƃn�[�t�x�N�g���̓��ς� lLightLitParam.y �ɃZ�b�g
			lLightLitParam.y = dot( lLightHalfVec, lViewNrm ) ;

			// �X�y�L�������˗��� lLightLitParam.w �ɃZ�b�g
			lLightLitParam.w = cfMaterial.Power.x ;

			// ���C�g�v�Z
			lLightLitDest = lit( lLightLitParam.x, lLightLitParam.y, lLightLitParam.w ) ;

		#else // USE_SPE

			lLightLitDest.y = max( lLightLitParam.x, 0.0f ) ;

		#endif // USE_SPE

	#endif // BUMPMAP == 0 && PHONG == 0
	// �f�B�t���[�Y���C�g�ƃX�y�L�������C�g�̊p�x�����v�Z =======( �I�� )





	// �f�B�t���[�Y�J���[�v�Z =======================================( �J�n )

	#if BUMPMAP == 0 && PHONG == 0

		#if LG1_SPOT || LG1_POINT

			// �f�B�t���[�Y���C�g�~�ϒl += �����E�X�|�b�g���C�g�p�x�����l * ( �f�B�t���[�Y�p�x�����v�Z���� * �f�B�t���[�Y�J���[ * ���C�g�̃f�B�t���[�Y�J���[ + ���C�g�̃A���r�G���g�J���[�ƃ}�e���A���̃A���r�G���g�J���[����Z�������� )
			lTotalDiffuse += lLightGen * ( lLightLitDest.y * cfLight[ 1 ].Diffuse * lDifColor + cfLight[ 1 ].Ambient ) ;

		#else  // LG1_SPOT || LG1_POINT

			// �f�B�t���[�Y���C�g�~�ϒl += �f�B�t���[�Y�p�x�����v�Z���� * �f�B�t���[�Y�J���[ * ���C�g�̃f�B�t���[�Y�J���[ + ���C�g�̃A���r�G���g�J���[�ƃ}�e���A���̃A���r�G���g�J���[����Z��������
			lTotalDiffuse += lLightLitDest.y * cfLight[ 1 ].Diffuse * lDifColor + cfLight[ 1 ].Ambient ;

		#endif // LG1_SPOT || LG1_POINT

	#endif // BUMPMAP == 0 && PHONG == 0

	// �f�B�t���[�Y�J���[�v�Z =======================================( �I�� )





	// �X�y�L�����J���[�v�Z =======================================( �J�n )

	#ifdef USE_SPE

		#if BUMPMAP == 0 && PHONG == 0

			#if LG1_SPOT || LG1_POINT || ALL_VS

				// �X�y�L�������C�g�~�ϒl += �X�y�L�����p�x�����v�Z���� * �����E�X�|�b�g���C�g���� * ���C�g�̃X�y�L�����J���[
				lTotalSpecular += lLightGen * lLightLitDest.z * cfLight[ 1 ].Specular ;

			#else	// LG1_SPOT || LG1_POINT || ALL_VS

				// �X�y�L�������C�g�~�ϒl += �X�y�L�����p�x�����v�Z���� * ���C�g�̃X�y�L�����J���[
				lTotalSpecular += lLightLitDest.z * cfLight[ 1 ].Specular ;

			#endif	// LG1_SPOT || LG1_POINT || ALL_VS

		#endif // BUMPMAP == 0 && PHONG == 0

	#endif // USE_SPE

	// �X�y�L�����J���[�v�Z =======================================( �I�� )




	#ifdef ALL_VS
		} // if( cbLightBool[ 1 ].Use )
	#endif

#endif  // LG1_USE
// ���C�g1�̏��� ***********************************************************************************************************************************( �I�� )








		// ���C�g2�̏��� ***********************************************************************************************************************************( �J�n )
#if LG2_USE

	#ifdef ALL_VS
		if( cbLightBool[ 2 ].Use )
		{
	#endif

	// �f�B�t���[�Y�J���[�p�x�����v�Z�p�̃��C�g�̕��������O�v�Z ======( �J�n )

	#if LG2_DIR

		// ���C�g�̕����v�Z
		lLightDir = cfLight[ 2 ].Direction ;

	#else  // LG2_DIR

		// ���C�g�����x�N�g���̌v�Z
		lLightDir = normalize( lViewPosition.xyz - cfLight[ 2 ].Position.xyz ) ;

	#endif  // LG2_DIR

	// ���C�g�̏���������
	VSOutput.Light2_Dir_Gen = ZERO ;

	// ���C�g�̌����̋t�x�N�g����ڒn��Ԃɕϊ�
	#ifdef BUMPMAP

		VSOutput.Light2_Dir_Gen.x = dot( lViewTan, -lLightDir ) ;
		VSOutput.Light2_Dir_Gen.y = dot( lViewBin, -lLightDir ) ;
		VSOutput.Light2_Dir_Gen.z = dot( lViewNrm, -lLightDir ) ;

	#elif PHONG // BUMPMAP

		// ���C�g�̌����̋t�x�N�g����ۑ�
		VSOutput.Light2_Dir_Gen.xyz = -lLightDir ;

	#endif // BUMPMAP

	// �f�B�t���[�Y�J���[�p�x�����v�Z�p�̃��C�g�̕��������O�v�Z ======( �I�� )


	// �����E�X�|�b�g���C�g�����l�v�Z ==========================( �J�n )
	#if LG2_SPOT || LG2_POINT

		#ifdef ALL_VS
			lTotalGen = 1.0f;
			if( cbLightBool[ 2 ].Point_Or_Spot )
			{
		#endif // ALL_VS

		// ���������v�Z ------------------

		// ���_�ƃ��C�g�ʒu�Ƃ̋����̓������߂� lLightTemp.x = ( POS - L2_POS ) * ( POS - L2_POS )
		lLightTemp = lViewPosition - cfLight[ 2 ].Position ;
		lLightTemp.w = 0.0f;
		lLightDistancePow2 = dot( lLightTemp, lLightTemp ) ;

		// �������̌v�Z lLightGen = 1 / ( �����l0 + �����l1 * ���� + �����l2 * ( ���� * ���� ) )
		lLightGen = 1.0f / ( cfLight[ 2 ].Range_FallOff_AT0_AT1.z + cfLight[ 2 ].Range_FallOff_AT0_AT1.w * sqrt( lLightDistancePow2 ) + cfLight[ 2 ].AT2_SpotP0_SpotP1.x * lLightDistancePow2 ) ;

		// �X�|�b�g���C�g�����v�Z --------

		#if LG2_SPOT

			#ifdef ALL_VS
				if( cbLightBool[ 2 ].Spot )
				{
			#endif // ALL_VS

			// ���C�g�����x�N�g���ƃ��C�g�ʒu���璸�_�ʒu�ւ̃x�N�g���̓���( ���� Cos a )���v�Z 
			lLightDirectionCosA = dot( lLightDir, cfLight[ 2 ].Direction ) ;

			// �X�|�b�g���C�g�����v�Z  pow( falloff, ( ( Cos a - Cos f ) / ( Cos q - Cos f ) ) )
			lLightGen *= saturate( pow( abs( max( lLightDirectionCosA - cfLight[ 2 ].AT2_SpotP0_SpotP1.y, 0.0f ) * cfLight[ 2 ].AT2_SpotP0_SpotP1.z ), cfLight[ 2 ].Range_FallOff_AT0_AT1.y ) ) ;

			#ifdef ALL_VS
				} // if( cbLightBool[ 2 ].Spot )
			#endif

		#endif // LG2_SPOT

		// �L�������O�������猸�������ő�ɂ��鏈��
		lLightGen *= step( lLightDistancePow2, cfLight[ 2 ].Range_FallOff_AT0_AT1.x ) ;

		// �o���v�}�b�v or �t�H���V�F�[�f�B���O�p�Ɍ�������ۑ�����
		#if PHONG || BUMPMAP
			VSOutput.Light2_Dir_Gen.w = lLightGen ;
		#endif // PHONG || BUMPMAP

		#ifdef ALL_VS
			} // if( cbLightBool[ 2 ].Point_Or_Spot )
		#endif

	#endif // LG2_SPOT || LG2_POINT
	// �����E�X�|�b�g���C�g�����l�v�Z =========================( �I�� )




	// �f�B�t���[�Y���C�g�ƃX�y�L�������C�g�̊p�x�����v�Z =======( �J�n )
	#if BUMPMAP == 0 && PHONG == 0

		// �@���ƃ��C�g�̋t�����x�N�g���Ƃ̓��ς� lLightLitParam.x �ɃZ�b�g
		lLightLitParam.x = dot( lViewNrm, -lLightDir ) ;

		#ifdef USE_SPE

			// �n�[�t�x�N�g���̌v�Z norm( ( norm( ���_�ʒu���王�_�ւ̃x�N�g�� ) + ���C�g�̕��� ) )
			lLightHalfVec = normalize( normalize( -lViewPosition.xyz ) - lLightDir ) ;

			// �@���ƃn�[�t�x�N�g���̓��ς� lLightLitParam.y �ɃZ�b�g
			lLightLitParam.y = dot( lLightHalfVec, lViewNrm ) ;

			// �X�y�L�������˗��� lLightLitParam.w �ɃZ�b�g
			lLightLitParam.w = cfMaterial.Power.x ;

			// ���C�g�v�Z
			lLightLitDest = lit( lLightLitParam.x, lLightLitParam.y, lLightLitParam.w ) ;

		#else // USE_SPE

			lLightLitDest.y = max( lLightLitParam.x, 0.0f ) ;

		#endif // USE_SPE

	#endif // BUMPMAP == 0 && PHONG == 0
	// �f�B�t���[�Y���C�g�ƃX�y�L�������C�g�̊p�x�����v�Z =======( �I�� )





	// �f�B�t���[�Y�J���[�v�Z =======================================( �J�n )

	#if BUMPMAP == 0 && PHONG == 0

		#if LG2_SPOT || LG2_POINT

			// �f�B�t���[�Y���C�g�~�ϒl += �����E�X�|�b�g���C�g�p�x�����l * ( �f�B�t���[�Y�p�x�����v�Z���� * �f�B�t���[�Y�J���[ * ���C�g�̃f�B�t���[�Y�J���[ + ���C�g�̃A���r�G���g�J���[�ƃ}�e���A���̃A���r�G���g�J���[����Z�������� )
			lTotalDiffuse += lLightGen * ( lLightLitDest.y * cfLight[ 2 ].Diffuse * lDifColor + cfLight[ 2 ].Ambient ) ;

		#else  // LG2_SPOT || LG2_POINT

			// �f�B�t���[�Y���C�g�~�ϒl += �f�B�t���[�Y�p�x�����v�Z���� * �f�B�t���[�Y�J���[ * ���C�g�̃f�B�t���[�Y�J���[ + ���C�g�̃A���r�G���g�J���[�ƃ}�e���A���̃A���r�G���g�J���[����Z��������
			lTotalDiffuse += lLightLitDest.y * cfLight[ 2 ].Diffuse * lDifColor + cfLight[ 2 ].Ambient ;

		#endif // LG2_SPOT || LG2_POINT

	#endif // BUMPMAP == 0 && PHONG == 0

	// �f�B�t���[�Y�J���[�v�Z =======================================( �I�� )





	// �X�y�L�����J���[�v�Z =======================================( �J�n )

	#ifdef USE_SPE

		#if BUMPMAP == 0 && PHONG == 0

			#if LG2_SPOT || LG2_POINT || ALL_VS

				// �X�y�L�������C�g�~�ϒl += �X�y�L�����p�x�����v�Z���� * �����E�X�|�b�g���C�g���� * ���C�g�̃X�y�L�����J���[
				lTotalSpecular += lLightGen * lLightLitDest.z * cfLight[ 2 ].Specular ;

			#else	// LG2_SPOT || LG2_POINT || ALL_VS

				// �X�y�L�������C�g�~�ϒl += �X�y�L�����p�x�����v�Z���� * ���C�g�̃X�y�L�����J���[
				lTotalSpecular += lLightLitDest.z * cfLight[ 2 ].Specular ;

			#endif	// LG2_SPOT || LG2_POINT || ALL_VS

		#endif // BUMPMAP == 0 && PHONG == 0

	#endif // USE_SPE

	// �X�y�L�����J���[�v�Z =======================================( �I�� )




	#ifdef ALL_VS
		} // if( cbLightBool[ 2 ].Use )
	#endif

#endif  // LG2_USE
// ���C�g2�̏��� ***********************************************************************************************************************************( �I�� )















		// �S�Ẵf�B�t���[�Y�v�f�����킹�ďo�̓��W�X�^�ɃZ�b�g =====( �J�n )

		#if BUMPMAP == 0 && PHONG == 0

			// �o�̓f�B�t���[�Y�J���[ = ���C�g�f�B�t���[�Y�J���[�~�ϒl + ( �}�e���A���̃A���r�G���g�J���[�ƃO���[�o���A���r�G���g�J���[����Z�������̂ƃ}�e���A���G�~�b�V�u�J���[�����Z�������� )
			VSOutput.Diffuse = lTotalDiffuse + cfAmbient_Emissive ;

			// �A���t�@�̓f�B�t���[�Y�J���[�̃A���t�@�����̂܂܎g��
			VSOutput.Diffuse.w = lDifColor.w ;

		#endif // BUMPMAP == 0 && PHONG == 0

		// �S�Ẵf�B�t���[�Y�v�f�����킹�ďo�̓��W�X�^�ɃZ�b�g =====( �I�� )





		// �S�ẴX�y�L�����v�f�����킹�ďo�̓��W�X�^�ɃZ�b�g =====( �J�n )

		#if USE_SPE && BUMPMAP == 0 && PHONG == 0

			// �o�̓X�y�L�����J���[ = ���C�g�X�y�L�����J���[�~�ϒl * �X�y�L�����J���[
			VSOutput.Specular = lTotalSpecular * lSpeColor ;

		#endif // USE_SPE && BUMPMAP == 0 && PHONG == 0

		// �S�ẴX�y�L�����v�f�����킹�ďo�̓��W�X�^�ɃZ�b�g =====( �I�� )






	#endif // LG_USE
	// ���C�g�̏��� +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �I�� )















	// �t�H�O�v�Z =============================================( �J�n )

	#if FOG_LINEAR || FOG_EXP || FOG_EXP2

		#ifdef FOG_LINEAR

			// ���`�t�H�O�v�Z
			FOG_OUT = lViewPosition.z * FOG_LINEAR_DIV + FOG_LINEAR_ADD ;

		#endif

		#ifdef FOG_EXP

			// �w���t�H�O�v�Z 1.0f / pow( e, ���� * density )
			FOG_OUT = 1.0f / pow( abs( FOG_EXP_E ), lViewPosition.z * FOG_EXP_DENSITY ) ;

		#endif

		#ifdef FOG_EXP2

			// �w���t�H�O�Q�v�Z 1.0f / pow( e, ( ���� * density ) * ( ���� * density ) )
			FOG_OUT = 1.0f / pow( abs( FOG_EXP_E ), ( lViewPosition.z * FOG_EXP_DENSITY ) * ( lViewPosition.z * FOG_EXP_DENSITY ) ) ;

		#endif

	#endif // FOG_LINEAR || FOG_EXP || FOG_EXP2

	// �t�H�O�v�Z =============================================( �I�� )

#if SHADOWMAP_DRAW
	// �ˉe���W���e�N�X�`�����W�Ƃ��ďo�͂��� =================( �J�n )

	VSOutput.ShadowMap1Pos_ShadowMap3PosX.xyz = VSOutput.Position.xyz ;
	VSOutput.ShadowMap1Pos_ShadowMap3PosX.w   = cfZeroOne.y;

	// �ˉe���W���e�N�X�`�����W�Ƃ��ďo�͂��� =================( �I�� )
#endif


#if SHADOWMAP
	// �[�x�e�p�̃��C�g���猩���ˉe���W���Z�o =================( �J�n )

	// ���[���h���W�����C�g�P�̎ˉe���W�ɕϊ�
	VSOutput.ShadowMap1Pos_ShadowMap3PosX.x = dot( lWorldPosition, cfShadowMap1LightViewProjectionMatrix[ 0 ] ) ;
	VSOutput.ShadowMap1Pos_ShadowMap3PosX.y = dot( lWorldPosition, cfShadowMap1LightViewProjectionMatrix[ 1 ] ) ;
	VSOutput.ShadowMap1Pos_ShadowMap3PosX.z = dot( lWorldPosition, cfShadowMap1LightViewProjectionMatrix[ 2 ] ) ;

	// ���[���h���W�����C�g�Q�̎ˉe���W�ɕϊ�
	VSOutput.ShadowMap2Pos_ShadowMap3PosY.x = dot( lWorldPosition, cfShadowMap2LightViewProjectionMatrix[ 0 ] ) ;
	VSOutput.ShadowMap2Pos_ShadowMap3PosY.y = dot( lWorldPosition, cfShadowMap2LightViewProjectionMatrix[ 1 ] ) ;
	VSOutput.ShadowMap2Pos_ShadowMap3PosY.z = dot( lWorldPosition, cfShadowMap2LightViewProjectionMatrix[ 2 ] ) ;

	// ���[���h���W�����C�g�R�̎ˉe���W�ɕϊ�
	VSOutput.ShadowMap1Pos_ShadowMap3PosX.w = dot( lWorldPosition, cfShadowMap3LightViewProjectionMatrix[ 0 ] ) ;
	VSOutput.ShadowMap2Pos_ShadowMap3PosY.w = dot( lWorldPosition, cfShadowMap3LightViewProjectionMatrix[ 1 ] ) ;
	VSOutput.V_to_Eye_ShadowMap3PosZ.w      = dot( lWorldPosition, cfShadowMap3LightViewProjectionMatrix[ 2 ] ) ;

	// �[�x�e�p�̃��C�g���猩���ˉe���W���Z�o =================( �I�� )
#endif

	// �e�N�X�`�����W�̃Z�b�g
	VSOutput.TexCoords0_1.x = dot( VSInput.TexCoords0, cfTextureMatrix[ 0 ][ 0 ] ) ;
	VSOutput.TexCoords0_1.y = dot( VSInput.TexCoords0, cfTextureMatrix[ 0 ][ 1 ] ) ;

//#ifdef SUBTEXTURE
	VSOutput.TexCoords0_1.z = dot( VSInput.TexCoords1, cfTextureMatrix[ 0 ][ 0 ] ) ;
	VSOutput.TexCoords0_1.w = dot( VSInput.TexCoords1, cfTextureMatrix[ 0 ][ 1 ] ) ;
//#endif // SUBTEXTURE


	return VSOutput ;
}

