#include "../VertexShader.h"

// ���_�V�F�[�_�[�̓���
struct VS_INPUT
{
	float3 Position        : POSITION0 ;		// ���W( ���[�J����� )
	float3 Normal          : NORMAL ;			// �@��
	float4 Diffuse         : COLOR0 ;			// �f�B�t���[�Y�J���[
	float4 Specular        : COLOR1 ;			// �X�y�L�����J���[
	float2 TexCoords0      : TEXCOORD0 ;		// �e�N�X�`�����W�O
	float2 TexCoords1      : TEXCOORD1 ;		// �e�N�X�`�����W�P

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
	float4 Diffuse         : COLOR0 ;		// �f�B�t���[�Y�J���[
	float4 Specular        : COLOR1 ;		// �X�y�L�����J���[
	float4 TexCoords0_1    : TEXCOORD0 ;	// �e�N�X�`�����W�ƃT�u�e�N�X�`�����W( x:�e�N�X�`�����Wx  y:�e�N�X�`�����Wy  z:�T�u�e�N�X�`�����Wx  w:�T�u�e�N�X�`�����Wy )

	float3 V_to_Eye        : TEXCOORD1 ;	// ���_���W���王���ւ̃x�N�g��( �r���[��� )�ƃV���h�E�}�b�v�Q�̃��C�gZ���W( w )
	float4 Normal_Fog      : TEXCOORD2 ;	// x.y.z:�@��( �r���[��� )  w:SM3.0�p�̃t�H�O�p�����[�^

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

	float4 Position        : SV_POSITION ;	// ���W( �v���W�F�N�V������� )
} ;


#ifdef SKINMESH
#define LOCAL_WORLD_MAT         lLocalWorldMatrix
#else
#define LOCAL_WORLD_MAT         g_Base.LocalWorldMatrix
#endif

#define FOG_OUT                 VSOutput.Normal_Fog.w

// 3D�p
VS_OUTPUT VS3D_Normal( VS_INPUT VSInput )
{
	VS_OUTPUT	VSOutput ;
	int4		lBoneFloatIndex ;
	float4		lLocalWorldMatrix[ 3 ] ;
	float4		lLocalPosition ;
	float4		lWorldPosition ;
	float4		lViewPosition ;
	float3		lWorldNrm ;
	float3		lWorldTan ;
	float3		lWorldBin ;
	float3		lViewNrm ;
	float3		lViewTan ;
	float3		lViewBin ;
	float3		lBumpTemp ;
	float4		lTotalDiffuse ;
	float4		lTotalSpecular ;
	float3		lLightDir ;
	float		lTotalGen ;
	float3		lLightTemp ;
	float		lLightDistancePow2 ;
	float		lLightGen ;
	float		lLightDirectionCosA ;
	float3		lLightHalfVec ;
	float4		lLightLitParam ;
	float4		lLightLitDest ;
	float4		lDifColor;
	float4      lTextureCoordTemp ;
#if USE_SPE
	float4		lSpeColor;
#endif
#if SHADOWMAP
	float4		lLViewPosition ;
#endif

	lDifColor = g_Base.DiffuseSource  > 0.5f ? VSInput.Diffuse  : g_Common.Material.Diffuse ;
	
#if USE_SPE
	lSpeColor = ( g_Base.SpecularSource > 0.5f ? VSInput.Specular : g_Common.Material.Specular ) * g_Base.MulSpecularColor ;
#endif

	VSOutput.V_to_Eye   = float3( 0.0f, 0.0f, 0.0f ) ;
	VSOutput.Specular   = float4( 0.0f, 0.0f, 0.0f, 0.0f ) ;
	VSOutput.Normal_Fog = float4( 0.0f, 0.0f, 0.0f, 1.0f ) ;

	// ���_���W�ϊ� ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �J�n )

	#ifdef SKINMESH

		// �X�L�����b�V��

		// �u�����h�s��̍쐬
		lBoneFloatIndex = VSInput.BlendIndices0 ;
		lLocalWorldMatrix[ 0 ]  = g_LocalWorldMatrix.Matrix[ lBoneFloatIndex.x + 0 ] * VSInput.BlendWeight0.xxxx;
		lLocalWorldMatrix[ 1 ]  = g_LocalWorldMatrix.Matrix[ lBoneFloatIndex.x + 1 ] * VSInput.BlendWeight0.xxxx;
		lLocalWorldMatrix[ 2 ]  = g_LocalWorldMatrix.Matrix[ lBoneFloatIndex.x + 2 ] * VSInput.BlendWeight0.xxxx;

		lLocalWorldMatrix[ 0 ] += g_LocalWorldMatrix.Matrix[ lBoneFloatIndex.y + 0 ] * VSInput.BlendWeight0.yyyy;
		lLocalWorldMatrix[ 1 ] += g_LocalWorldMatrix.Matrix[ lBoneFloatIndex.y + 1 ] * VSInput.BlendWeight0.yyyy;
		lLocalWorldMatrix[ 2 ] += g_LocalWorldMatrix.Matrix[ lBoneFloatIndex.y + 2 ] * VSInput.BlendWeight0.yyyy;

		lLocalWorldMatrix[ 0 ] += g_LocalWorldMatrix.Matrix[ lBoneFloatIndex.z + 0 ] * VSInput.BlendWeight0.zzzz;
		lLocalWorldMatrix[ 1 ] += g_LocalWorldMatrix.Matrix[ lBoneFloatIndex.z + 1 ] * VSInput.BlendWeight0.zzzz;
		lLocalWorldMatrix[ 2 ] += g_LocalWorldMatrix.Matrix[ lBoneFloatIndex.z + 2 ] * VSInput.BlendWeight0.zzzz;

		lLocalWorldMatrix[ 0 ] += g_LocalWorldMatrix.Matrix[ lBoneFloatIndex.w + 0 ] * VSInput.BlendWeight0.wwww;
		lLocalWorldMatrix[ 1 ] += g_LocalWorldMatrix.Matrix[ lBoneFloatIndex.w + 1 ] * VSInput.BlendWeight0.wwww;
		lLocalWorldMatrix[ 2 ] += g_LocalWorldMatrix.Matrix[ lBoneFloatIndex.w + 2 ] * VSInput.BlendWeight0.wwww;

		#ifdef BONE8

			lBoneFloatIndex = VSInput.BlendIndices1 ;
			lLocalWorldMatrix[ 0 ] += g_LocalWorldMatrix.Matrix[ lBoneFloatIndex.x + 0 ] * VSInput.BlendWeight1.xxxx;
			lLocalWorldMatrix[ 1 ] += g_LocalWorldMatrix.Matrix[ lBoneFloatIndex.x + 1 ] * VSInput.BlendWeight1.xxxx;
			lLocalWorldMatrix[ 2 ] += g_LocalWorldMatrix.Matrix[ lBoneFloatIndex.x + 2 ] * VSInput.BlendWeight1.xxxx;

			lLocalWorldMatrix[ 0 ] += g_LocalWorldMatrix.Matrix[ lBoneFloatIndex.y + 0 ] * VSInput.BlendWeight1.yyyy;
			lLocalWorldMatrix[ 1 ] += g_LocalWorldMatrix.Matrix[ lBoneFloatIndex.y + 1 ] * VSInput.BlendWeight1.yyyy;
			lLocalWorldMatrix[ 2 ] += g_LocalWorldMatrix.Matrix[ lBoneFloatIndex.y + 2 ] * VSInput.BlendWeight1.yyyy;

			lLocalWorldMatrix[ 0 ] += g_LocalWorldMatrix.Matrix[ lBoneFloatIndex.z + 0 ] * VSInput.BlendWeight1.zzzz;
			lLocalWorldMatrix[ 1 ] += g_LocalWorldMatrix.Matrix[ lBoneFloatIndex.z + 1 ] * VSInput.BlendWeight1.zzzz;
			lLocalWorldMatrix[ 2 ] += g_LocalWorldMatrix.Matrix[ lBoneFloatIndex.z + 2 ] * VSInput.BlendWeight1.zzzz;

			lLocalWorldMatrix[ 0 ] += g_LocalWorldMatrix.Matrix[ lBoneFloatIndex.w + 0 ] * VSInput.BlendWeight1.wwww;
			lLocalWorldMatrix[ 1 ] += g_LocalWorldMatrix.Matrix[ lBoneFloatIndex.w + 1 ] * VSInput.BlendWeight1.wwww;
			lLocalWorldMatrix[ 2 ] += g_LocalWorldMatrix.Matrix[ lBoneFloatIndex.w + 2 ] * VSInput.BlendWeight1.wwww;

		#endif // BONE8

	#endif	// SKINMESH

	// ���[�J�����W����
	lLocalPosition.xyz  = VSInput.Position ;
	lLocalPosition.w    = 1.0f ;

	// ���W�v�Z( ���[�J�����r���[���v���W�F�N�V���� )
	lWorldPosition.x = dot( lLocalPosition, LOCAL_WORLD_MAT[ 0 ] ) ;
	lWorldPosition.y = dot( lLocalPosition, LOCAL_WORLD_MAT[ 1 ] ) ;
	lWorldPosition.z = dot( lLocalPosition, LOCAL_WORLD_MAT[ 2 ] ) ;
	lWorldPosition.w = 1.0f ;

	lViewPosition.x = dot( lWorldPosition, g_Base.ViewMatrix[ 0 ] ) ;
	lViewPosition.y = dot( lWorldPosition, g_Base.ViewMatrix[ 1 ] ) ;
	lViewPosition.z = dot( lWorldPosition, g_Base.ViewMatrix[ 2 ] ) ;
	lViewPosition.w = 1.0f ;

	VSOutput.Position.x = dot( lViewPosition, g_Base.ProjectionMatrix[ 0 ] ) ;
	VSOutput.Position.y = dot( lViewPosition, g_Base.ProjectionMatrix[ 1 ] ) ;
	VSOutput.Position.z = dot( lViewPosition, g_Base.ProjectionMatrix[ 2 ] ) ;
	VSOutput.Position.w = dot( lViewPosition, g_Base.ProjectionMatrix[ 3 ] ) ;

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

		lViewTan.x = dot( lWorldTan, g_Base.ViewMatrix[ 0 ].xyz ) ;
		lViewTan.y = dot( lWorldTan, g_Base.ViewMatrix[ 1 ].xyz ) ;
		lViewTan.z = dot( lWorldTan, g_Base.ViewMatrix[ 2 ].xyz ) ;

		lViewBin.x = dot( lWorldBin, g_Base.ViewMatrix[ 0 ].xyz ) ;
		lViewBin.y = dot( lWorldBin, g_Base.ViewMatrix[ 1 ].xyz ) ;
		lViewBin.z = dot( lWorldBin, g_Base.ViewMatrix[ 2 ].xyz ) ;

		lViewNrm.x = dot( lWorldNrm, g_Base.ViewMatrix[ 0 ].xyz ) ;
		lViewNrm.y = dot( lWorldNrm, g_Base.ViewMatrix[ 1 ].xyz ) ;
		lViewNrm.z = dot( lWorldNrm, g_Base.ViewMatrix[ 2 ].xyz ) ;

	#endif	// BUMPMAP


	// ���_���W�ϊ� ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �I�� )

	#if LG_USE == 0 || BUMPMAP == 1 || PHONG == 1

		// ���C�g���g�p���Ȃ����A�o���v�}�b�v���̓t�H���V�F�[�f�B���O�̏ꍇ�̓}�e���A��( or ���_ )�J���[�����̂܂܏o��
		VSOutput.Diffuse  = lDifColor ;

		#if USE_SPE
			VSOutput.Specular = lSpeColor ;
		#endif
		
		#if BUMPMAP == 0 && PHONG
		
			// �@�����r���[��Ԃ̊p�x�ɕϊ�( LOCAL_WORLD_MAT �̓X�L�j���O���b�V�� �̏ꍇ�� r0 ���g���̂� mov r0, 0.0f ����Ɏ��s����K�v������ )
			lWorldNrm.x = dot( VSInput.Normal, LOCAL_WORLD_MAT[ 0 ].xyz ) ;
			lWorldNrm.y = dot( VSInput.Normal, LOCAL_WORLD_MAT[ 1 ].xyz ) ;
			lWorldNrm.z = dot( VSInput.Normal, LOCAL_WORLD_MAT[ 2 ].xyz ) ;

			lViewNrm.x = dot( lWorldNrm, g_Base.ViewMatrix[ 0 ].xyz ) ;
			lViewNrm.y = dot( lWorldNrm, g_Base.ViewMatrix[ 1 ].xyz ) ;
			lViewNrm.z = dot( lWorldNrm, g_Base.ViewMatrix[ 2 ].xyz ) ;

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
				VSOutput.V_to_Eye = normalize( lBumpTemp ) ;
			#endif

		#else // BUMPMAP 

			// �@�����r���[��Ԃ̊p�x�ɕϊ�( LOCAL_WORLD_MAT �̓X�L�j���O���b�V�� �̏ꍇ�� r0 ���g���̂� mov r0, 0.0f ����Ɏ��s����K�v������ )
			lWorldNrm.x = dot( VSInput.Normal, LOCAL_WORLD_MAT[ 0 ].xyz ) ;
			lWorldNrm.y = dot( VSInput.Normal, LOCAL_WORLD_MAT[ 1 ].xyz ) ;
			lWorldNrm.z = dot( VSInput.Normal, LOCAL_WORLD_MAT[ 2 ].xyz ) ;

			lViewNrm.x = dot( lWorldNrm, g_Base.ViewMatrix[ 0 ].xyz ) ;
			lViewNrm.y = dot( lWorldNrm, g_Base.ViewMatrix[ 1 ].xyz ) ;
			lViewNrm.z = dot( lWorldNrm, g_Base.ViewMatrix[ 2 ].xyz ) ;

			// �f�B�t���[�Y�J���[�ƃX�y�L�����J���[�̒~�ϒl�̏�����
			lTotalDiffuse  = float4( 0, 0, 0, 0 ) ;
			lTotalSpecular = float4( 0, 0, 0, 0 ) ;

			#ifdef PHONG

				#if USE_SPE
					// ���_���W���王�_�ւ̃x�N�g���𐳋K�����ĕۑ�
					VSOutput.V_to_Eye = normalize( -lViewPosition.xyz ) ;
				#endif

				// �@����ۑ�
				VSOutput.Normal_Fog.xyz = lViewNrm ;

			#endif // PHONG

		#endif // BUMPMAP







		// ���C�g0�̏��� ***********************************************************************************************************************************( �J�n )
#if LG0_USE

	// �f�B�t���[�Y�J���[�p�x�����v�Z�p�̃��C�g�̕��������O�v�Z ======( �J�n )

	#if LG0_DIR

		// ���C�g�̕����v�Z
		lLightDir = g_Common.Light[ 0 ].Direction ;

	#else  // LG0_DIR

		// ���C�g�����x�N�g���̌v�Z
		lLightDir = normalize( lViewPosition.xyz - g_Common.Light[ 0 ].Position ) ;

	#endif  // LG0_DIR

	// ���C�g�̏���������
	VSOutput.Light0_Dir_Gen = 0.0f ;

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

		// ���������v�Z ------------------

		// ���_�ƃ��C�g�ʒu�Ƃ̋����̓������߂� lLightTemp.x = ( POS - L0_POS ) * ( POS - L0_POS )
		lLightTemp = lViewPosition.xyz - g_Common.Light[ 0 ].Position ;
		lLightDistancePow2 = dot( lLightTemp, lLightTemp ) ;

		// �������̌v�Z lLightGen = 1 / ( �����l0 + �����l1 * ���� + �����l2 * ( ���� * ���� ) )
		lLightGen = 1.0f / ( g_Common.Light[ 0 ].Attenuation0 + g_Common.Light[ 0 ].Attenuation1 * sqrt( lLightDistancePow2 ) + g_Common.Light[ 0 ].Attenuation2 * lLightDistancePow2 ) ;

		// �X�|�b�g���C�g�����v�Z --------

		#if LG0_SPOT

			// ���C�g�����x�N�g���ƃ��C�g�ʒu���璸�_�ʒu�ւ̃x�N�g���̓���( ���� Cos a )���v�Z 
			lLightDirectionCosA = dot( lLightDir, g_Common.Light[ 0 ].Direction.xyz ) ;

			// �X�|�b�g���C�g�����v�Z  pow( falloff, ( ( Cos a - Cos f ) / ( Cos q - Cos f ) ) )
			lLightGen *= saturate( pow( abs( max( lLightDirectionCosA - g_Common.Light[ 0 ].SpotParam0, 0.0f ) * g_Common.Light[ 0 ].SpotParam1 ), g_Common.Light[ 0 ].FallOff ) ) ;

		#endif // LG0_SPOT

		// �L�������O�������猸�������ő�ɂ��鏈��
		lLightGen *= step( lLightDistancePow2, g_Common.Light[ 0 ].RangePow2 ) ;

		// �o���v�}�b�v or �t�H���V�F�[�f�B���O�p�Ɍ�������ۑ�����
		#if PHONG || BUMPMAP
			VSOutput.Light0_Dir_Gen.w = lLightGen ;
		#endif // PHONG || BUMPMAP

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
			lLightLitParam.w = g_Common.Material.Power ;

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
			lTotalDiffuse.xyz += lLightGen * ( lLightLitDest.y * g_Common.Light[ 0 ].Diffuse * lDifColor.xyz + g_Common.Light[ 0 ].Ambient.xyz ) ;

		#else  // LG0_SPOT || LG0_POINT

			// �f�B�t���[�Y���C�g�~�ϒl += �f�B�t���[�Y�p�x�����v�Z���� * �f�B�t���[�Y�J���[ * ���C�g�̃f�B�t���[�Y�J���[ + ���C�g�̃A���r�G���g�J���[�ƃ}�e���A���̃A���r�G���g�J���[����Z��������
			lTotalDiffuse.xyz += lLightLitDest.y * g_Common.Light[ 0 ].Diffuse * lDifColor.xyz + g_Common.Light[ 0 ].Ambient.xyz ;

		#endif // LG0_SPOT || LG0_POINT

	#endif // BUMPMAP == 0 && PHONG == 0

	// �f�B�t���[�Y�J���[�v�Z =======================================( �I�� )





	// �X�y�L�����J���[�v�Z =======================================( �J�n )

	#ifdef USE_SPE

		#if BUMPMAP == 0 && PHONG == 0

			#if LG0_SPOT || LG0_POINT

				// �X�y�L�������C�g�~�ϒl += �X�y�L�����p�x�����v�Z���� * �����E�X�|�b�g���C�g���� * ���C�g�̃X�y�L�����J���[
				lTotalSpecular.xyz += lLightGen * lLightLitDest.z * g_Common.Light[ 0 ].Specular ;

			#else	// LG0_SPOT || LG0_POINT

				// �X�y�L�������C�g�~�ϒl += �X�y�L�����p�x�����v�Z���� * ���C�g�̃X�y�L�����J���[
				lTotalSpecular.xyz += lLightLitDest.z * g_Common.Light[ 0 ].Specular ;

			#endif	// LG0_SPOT || LG0_POINT

		#endif // BUMPMAP == 0 && PHONG == 0

	#endif // USE_SPE

	// �X�y�L�����J���[�v�Z =======================================( �I�� )




#endif  // LG0_USE
// ���C�g0�̏��� ***********************************************************************************************************************************( �I�� )








		// ���C�g1�̏��� ***********************************************************************************************************************************( �J�n )
#if LG1_USE

	// �f�B�t���[�Y�J���[�p�x�����v�Z�p�̃��C�g�̕��������O�v�Z ======( �J�n )

	#if LG1_DIR

		// ���C�g�̕����v�Z
		lLightDir = g_Common.Light[ 1 ].Direction ;

	#else  // LG1_DIR

		// ���C�g�����x�N�g���̌v�Z
		lLightDir = normalize( lViewPosition.xyz - g_Common.Light[ 1 ].Position ) ;

	#endif  // LG1_DIR

	// ���C�g�̏���������
	VSOutput.Light1_Dir_Gen = 0.0f ;

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

		// ���������v�Z ------------------

		// ���_�ƃ��C�g�ʒu�Ƃ̋����̓������߂� lLightTemp.x = ( POS - L1_POS ) * ( POS - L1_POS )
		lLightTemp = lViewPosition.xyz - g_Common.Light[ 1 ].Position ;
		lLightDistancePow2 = dot( lLightTemp, lLightTemp ) ;

		// �������̌v�Z lLightGen = 1 / ( �����l0 + �����l1 * ���� + �����l2 * ( ���� * ���� ) )
		lLightGen = 1.0f / ( g_Common.Light[ 1 ].Attenuation0 + g_Common.Light[ 1 ].Attenuation1 * sqrt( lLightDistancePow2 ) + g_Common.Light[ 1 ].Attenuation2 * lLightDistancePow2 ) ;

		// �X�|�b�g���C�g�����v�Z --------

		#if LG1_SPOT

			// ���C�g�����x�N�g���ƃ��C�g�ʒu���璸�_�ʒu�ւ̃x�N�g���̓���( ���� Cos a )���v�Z 
			lLightDirectionCosA = dot( lLightDir, g_Common.Light[ 1 ].Direction.xyz ) ;

			// �X�|�b�g���C�g�����v�Z  pow( falloff, ( ( Cos a - Cos f ) / ( Cos q - Cos f ) ) )
			lLightGen *= saturate( pow( abs( max( lLightDirectionCosA - g_Common.Light[ 1 ].SpotParam0, 0.0f ) * g_Common.Light[ 1 ].SpotParam1 ), g_Common.Light[ 1 ].FallOff ) ) ;

		#endif // LG1_SPOT

		// �L�������O�������猸�������ő�ɂ��鏈��
		lLightGen *= step( lLightDistancePow2, g_Common.Light[ 1 ].RangePow2 ) ;

		// �o���v�}�b�v or �t�H���V�F�[�f�B���O�p�Ɍ�������ۑ�����
		#if PHONG || BUMPMAP
			VSOutput.Light1_Dir_Gen.w = lLightGen ;
		#endif // PHONG || BUMPMAP

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
			lLightLitParam.w = g_Common.Material.Power ;

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
			lTotalDiffuse.xyz += lLightGen * ( lLightLitDest.y * g_Common.Light[ 1 ].Diffuse * lDifColor.xyz + g_Common.Light[ 1 ].Ambient.xyz ) ;

		#else  // LG1_SPOT || LG1_POINT

			// �f�B�t���[�Y���C�g�~�ϒl += �f�B�t���[�Y�p�x�����v�Z���� * �f�B�t���[�Y�J���[ * ���C�g�̃f�B�t���[�Y�J���[ + ���C�g�̃A���r�G���g�J���[�ƃ}�e���A���̃A���r�G���g�J���[����Z��������
			lTotalDiffuse.xyz += lLightLitDest.y * g_Common.Light[ 1 ].Diffuse * lDifColor.xyz + g_Common.Light[ 1 ].Ambient.xyz ;

		#endif // LG1_SPOT || LG1_POINT

	#endif // BUMPMAP == 0 && PHONG == 0

	// �f�B�t���[�Y�J���[�v�Z =======================================( �I�� )





	// �X�y�L�����J���[�v�Z =======================================( �J�n )

	#ifdef USE_SPE

		#if BUMPMAP == 0 && PHONG == 0

			#if LG1_SPOT || LG1_POINT

				// �X�y�L�������C�g�~�ϒl += �X�y�L�����p�x�����v�Z���� * �����E�X�|�b�g���C�g���� * ���C�g�̃X�y�L�����J���[
				lTotalSpecular.xyz += lLightGen * lLightLitDest.z * g_Common.Light[ 1 ].Specular ;

			#else	// LG1_SPOT || LG1_POINT

				// �X�y�L�������C�g�~�ϒl += �X�y�L�����p�x�����v�Z���� * ���C�g�̃X�y�L�����J���[
				lTotalSpecular.xyz += lLightLitDest.z * g_Common.Light[ 1 ].Specular ;

			#endif	// LG1_SPOT || LG1_POINT

		#endif // BUMPMAP == 0 && PHONG == 0

	#endif // USE_SPE

	// �X�y�L�����J���[�v�Z =======================================( �I�� )




#endif  // LG1_USE
// ���C�g1�̏��� ***********************************************************************************************************************************( �I�� )








		// ���C�g2�̏��� ***********************************************************************************************************************************( �J�n )
#if LG2_USE

	// �f�B�t���[�Y�J���[�p�x�����v�Z�p�̃��C�g�̕��������O�v�Z ======( �J�n )

	#if LG2_DIR

		// ���C�g�̕����v�Z
		lLightDir = g_Common.Light[ 2 ].Direction ;

	#else  // LG2_DIR

		// ���C�g�����x�N�g���̌v�Z
		lLightDir = normalize( lViewPosition.xyz - g_Common.Light[ 2 ].Position ) ;

	#endif  // LG2_DIR

	// ���C�g�̏���������
	VSOutput.Light2_Dir_Gen = 0.0f ;

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

		// ���������v�Z ------------------

		// ���_�ƃ��C�g�ʒu�Ƃ̋����̓������߂� lLightTemp.x = ( POS - L2_POS ) * ( POS - L2_POS )
		lLightTemp = lViewPosition.xyz - g_Common.Light[ 2 ].Position ;
		lLightDistancePow2 = dot( lLightTemp, lLightTemp ) ;

		// �������̌v�Z lLightGen = 1 / ( �����l0 + �����l1 * ���� + �����l2 * ( ���� * ���� ) )
		lLightGen = 1.0f / ( g_Common.Light[ 2 ].Attenuation0 + g_Common.Light[ 2 ].Attenuation1 * sqrt( lLightDistancePow2 ) + g_Common.Light[ 2 ].Attenuation2 * lLightDistancePow2 ) ;

		// �X�|�b�g���C�g�����v�Z --------

		#if LG2_SPOT

			// ���C�g�����x�N�g���ƃ��C�g�ʒu���璸�_�ʒu�ւ̃x�N�g���̓���( ���� Cos a )���v�Z 
			lLightDirectionCosA = dot( lLightDir, g_Common.Light[ 2 ].Direction.xyz ) ;

			// �X�|�b�g���C�g�����v�Z  pow( falloff, ( ( Cos a - Cos f ) / ( Cos q - Cos f ) ) )
			lLightGen *= saturate( pow( abs( max( lLightDirectionCosA - g_Common.Light[ 2 ].SpotParam0, 0.0f ) * g_Common.Light[ 2 ].SpotParam1 ), g_Common.Light[ 2 ].FallOff ) ) ;

		#endif // LG2_SPOT

		// �L�������O�������猸�������ő�ɂ��鏈��
		lLightGen *= step( lLightDistancePow2, g_Common.Light[ 2 ].RangePow2 ) ;

		// �o���v�}�b�v or �t�H���V�F�[�f�B���O�p�Ɍ�������ۑ�����
		#if PHONG || BUMPMAP
			VSOutput.Light2_Dir_Gen.w = lLightGen ;
		#endif // PHONG || BUMPMAP

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
			lLightLitParam.w = g_Common.Material.Power ;

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
			lTotalDiffuse.xyz += lLightGen * ( lLightLitDest.y * g_Common.Light[ 2 ].Diffuse * lDifColor.xyz + g_Common.Light[ 2 ].Ambient.xyz ) ;

		#else  // LG2_SPOT || LG2_POINT

			// �f�B�t���[�Y���C�g�~�ϒl += �f�B�t���[�Y�p�x�����v�Z���� * �f�B�t���[�Y�J���[ * ���C�g�̃f�B�t���[�Y�J���[ + ���C�g�̃A���r�G���g�J���[�ƃ}�e���A���̃A���r�G���g�J���[����Z��������
			lTotalDiffuse.xyz += lLightLitDest.y * g_Common.Light[ 2 ].Diffuse * lDifColor.xyz + g_Common.Light[ 2 ].Ambient.xyz ;

		#endif // LG2_SPOT || LG2_POINT

	#endif // BUMPMAP == 0 && PHONG == 0

	// �f�B�t���[�Y�J���[�v�Z =======================================( �I�� )





	// �X�y�L�����J���[�v�Z =======================================( �J�n )

	#ifdef USE_SPE

		#if BUMPMAP == 0 && PHONG == 0

			#if LG2_SPOT || LG2_POINT

				// �X�y�L�������C�g�~�ϒl += �X�y�L�����p�x�����v�Z���� * �����E�X�|�b�g���C�g���� * ���C�g�̃X�y�L�����J���[
				lTotalSpecular.xyz += lLightGen * lLightLitDest.z * g_Common.Light[ 2 ].Specular ;

			#else	// LG2_SPOT || LG2_POINT

				// �X�y�L�������C�g�~�ϒl += �X�y�L�����p�x�����v�Z���� * ���C�g�̃X�y�L�����J���[
				lTotalSpecular.xyz += lLightLitDest.z * g_Common.Light[ 2 ].Specular ;

			#endif	// LG2_SPOT || LG2_POINT

		#endif // BUMPMAP == 0 && PHONG == 0

	#endif // USE_SPE

	// �X�y�L�����J���[�v�Z =======================================( �I�� )




#endif  // LG2_USE
// ���C�g2�̏��� ***********************************************************************************************************************************( �I�� )















		// �S�Ẵf�B�t���[�Y�v�f�����킹�ďo�̓��W�X�^�ɃZ�b�g =====( �J�n )

		#if BUMPMAP == 0 && PHONG == 0

			// �o�̓f�B�t���[�Y�J���[ = ���C�g�f�B�t���[�Y�J���[�~�ϒl + ( �}�e���A���̃A���r�G���g�J���[�ƃO���[�o���A���r�G���g�J���[����Z�������̂ƃ}�e���A���G�~�b�V�u�J���[�����Z�������� )
			VSOutput.Diffuse = lTotalDiffuse + g_Common.Material.Ambient_Emissive ;

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
			FOG_OUT = lViewPosition.z * g_Common.Fog.LinearDiv + g_Common.Fog.LinearAdd ;

		#endif

		#ifdef FOG_EXP

			// �w���t�H�O�v�Z 1.0f / pow( e, ���� * density )
			FOG_OUT = 1.0f / pow( abs( g_Common.Fog.E ), lViewPosition.z * g_Common.Fog.Density ) ;

		#endif

		#ifdef FOG_EXP2

			// �w���t�H�O�Q�v�Z 1.0f / pow( e, ( ���� * density ) * ( ���� * density ) )
			FOG_OUT = 1.0f / pow( abs( g_Common.Fog.E ), ( lViewPosition.z * g_Common.Fog.Density ) * ( lViewPosition.z * g_Common.Fog.Density ) ) ;

		#endif

	#endif // FOG_LINEAR || FOG_EXP || FOG_EXP2

	// �t�H�O�v�Z =============================================( �I�� )

#if SHADOWMAP_DRAW
	// �ˉe���W���e�N�X�`�����W�Ƃ��ďo�͂��� =================( �J�n )

	VSOutput.ShadowMap0Pos = VSOutput.Position.xyz ;

	// �ˉe���W���e�N�X�`�����W�Ƃ��ďo�͂��� =================( �I�� )
#endif


#if SHADOWMAP
	// �[�x�e�p�̃��C�g���猩���ˉe���W���Z�o =================( �J�n )

	// ���[���h���W���V���h�E�}�b�v�O�̃��C�g�ݒ�̎ˉe���W�ɕϊ�
	VSOutput.ShadowMap0Pos.x = dot( lWorldPosition, g_OtherMatrix.ShadowMapLightViewProjectionMatrix[ 0 ][ 0 ] ) ;
	VSOutput.ShadowMap0Pos.y = dot( lWorldPosition, g_OtherMatrix.ShadowMapLightViewProjectionMatrix[ 0 ][ 1 ] ) ;
	VSOutput.ShadowMap0Pos.z = dot( lWorldPosition, g_OtherMatrix.ShadowMapLightViewProjectionMatrix[ 0 ][ 2 ] ) ;

	// ���[���h���W���V���h�E�}�b�v�P�̃��C�g�ݒ�̎ˉe���W�ɕϊ�
	VSOutput.ShadowMap1Pos.x = dot( lWorldPosition, g_OtherMatrix.ShadowMapLightViewProjectionMatrix[ 1 ][ 0 ] ) ;
	VSOutput.ShadowMap1Pos.y = dot( lWorldPosition, g_OtherMatrix.ShadowMapLightViewProjectionMatrix[ 1 ][ 1 ] ) ;
	VSOutput.ShadowMap1Pos.z = dot( lWorldPosition, g_OtherMatrix.ShadowMapLightViewProjectionMatrix[ 1 ][ 2 ] ) ;

	// ���[���h���W���V���h�E�}�b�v�Q�̃��C�g�ݒ�̎ˉe���W�ɕϊ�
	VSOutput.ShadowMap2Pos.x = dot( lWorldPosition, g_OtherMatrix.ShadowMapLightViewProjectionMatrix[ 2 ][ 0 ] ) ;
	VSOutput.ShadowMap2Pos.y = dot( lWorldPosition, g_OtherMatrix.ShadowMapLightViewProjectionMatrix[ 2 ][ 1 ] ) ;
	VSOutput.ShadowMap2Pos.z = dot( lWorldPosition, g_OtherMatrix.ShadowMapLightViewProjectionMatrix[ 2 ][ 2 ] ) ;

	// �[�x�e�p�̃��C�g���猩���ˉe���W���Z�o =================( �I�� )
#endif

	// �e�N�X�`�����W�̃Z�b�g
	lTextureCoordTemp.zw = 1.0f ;

	lTextureCoordTemp.xy = VSInput.TexCoords0 ;
	VSOutput.TexCoords0_1.x = dot( lTextureCoordTemp, g_OtherMatrix.TextureMatrix[ 0 ][ 0 ] ) ;
	VSOutput.TexCoords0_1.y = dot( lTextureCoordTemp, g_OtherMatrix.TextureMatrix[ 0 ][ 1 ] ) ;

	lTextureCoordTemp.xy = VSInput.TexCoords1 ;
	VSOutput.TexCoords0_1.z = dot( lTextureCoordTemp, g_OtherMatrix.TextureMatrix[ 0 ][ 0 ] ) ;
	VSOutput.TexCoords0_1.w = dot( lTextureCoordTemp, g_OtherMatrix.TextureMatrix[ 0 ][ 1 ] ) ;

	return VSOutput ;
}
