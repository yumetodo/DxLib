#include "DxVertexShaderFxCode2.h"

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
#if SHADOWMAP
	float4 lLViewPosition ;
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
	
	// ���W( �r���[��� )��ۑ�
	VSOutput.VPosition = lViewPosition.xyz;
	
	// �@�����v�Z
	lWorldNrm.x = dot( VSInput.Normal, LOCAL_WORLD_MAT[ 0 ].xyz ) ;
	lWorldNrm.y = dot( VSInput.Normal, LOCAL_WORLD_MAT[ 1 ].xyz ) ;
	lWorldNrm.z = dot( VSInput.Normal, LOCAL_WORLD_MAT[ 2 ].xyz ) ;

	lViewNrm.x = dot( lWorldNrm, cfViewMatrix[ 0 ].xyz ) ;
	lViewNrm.y = dot( lWorldNrm, cfViewMatrix[ 1 ].xyz ) ;
	lViewNrm.z = dot( lWorldNrm, cfViewMatrix[ 2 ].xyz ) ;

	// �@��( �r���[��� )��ۑ�
	VSOutput.VNormal = lViewNrm;

	#ifdef BUMPMAP

		// �]�@���A�ڐ����r���[��Ԃɓ��e����
		lWorldTan.x = dot( VSInput.Tan, LOCAL_WORLD_MAT[ 0 ].xyz ) ;
		lWorldTan.y = dot( VSInput.Tan, LOCAL_WORLD_MAT[ 1 ].xyz ) ;
		lWorldTan.z = dot( VSInput.Tan, LOCAL_WORLD_MAT[ 2 ].xyz ) ;

		lWorldBin.x = dot( VSInput.Bin, LOCAL_WORLD_MAT[ 0 ].xyz ) ;
		lWorldBin.y = dot( VSInput.Bin, LOCAL_WORLD_MAT[ 1 ].xyz ) ;
		lWorldBin.z = dot( VSInput.Bin, LOCAL_WORLD_MAT[ 2 ].xyz ) ;

		lViewTan.x = dot( lWorldTan, cfViewMatrix[ 0 ].xyz ) ;
		lViewTan.y = dot( lWorldTan, cfViewMatrix[ 1 ].xyz ) ;
		lViewTan.z = dot( lWorldTan, cfViewMatrix[ 2 ].xyz ) ;

		lViewBin.x = dot( lWorldBin, cfViewMatrix[ 0 ].xyz ) ;
		lViewBin.y = dot( lWorldBin, cfViewMatrix[ 1 ].xyz ) ;
		lViewBin.z = dot( lWorldBin, cfViewMatrix[ 2 ].xyz ) ;
		
		// �]�@���A�ڐ�( �r���[��� )��ۑ�
		VSOutput.VTan = lViewTan;
		VSOutput.VBin = lViewBin;

	#endif	// BUMPMAP

	// �f�B�t���[�Y�J���[���Z�b�g
	VSOutput.Diffuse = cfDifSpeSource.x > 0.5f ? VSInput.Diffuse : cfMaterial.Diffuse;
	
	// �X�y�L�����J���[���Z�b�g
	VSOutput.Specular = cfDifSpeSource.y > 0.5f ? VSInput.Specular : cfMaterial.Specular;

	// ���_���W�ϊ� ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �I�� )





	// �t�H�O�v�Z =============================================( �J�n )

	#if FOG_LINEAR || FOG_EXP || FOG_EXP2

		#ifdef FOG_LINEAR

			// ���`�t�H�O�v�Z
			VSOutput.Fog_ShadowMap3PosZ.x = lViewPosition.z * FOG_LINEAR_DIV + FOG_LINEAR_ADD ;

		#endif

		#ifdef FOG_EXP

			// �w���t�H�O�v�Z 1.0f / pow( e, ���� * density )
			VSOutput.Fog_ShadowMap3PosZ.x = 1.0f / pow( abs( FOG_EXP_E ), lViewPosition.z * FOG_EXP_DENSITY ) ;

		#endif

		#ifdef FOG_EXP2

			// �w���t�H�O�Q�v�Z 1.0f / pow( e, ( ���� * density ) * ( ���� * density ) )
			VSOutput.Fog_ShadowMap3PosZ.x = 1.0f / pow( abs( FOG_EXP_E ), ( lViewPosition.z * FOG_EXP_DENSITY ) * ( lViewPosition.z * FOG_EXP_DENSITY ) ) ;

		#endif

	#else // FOG_LINEAR || FOG_EXP || FOG_EXP2
	
		VSOutput.Fog_ShadowMap3PosZ.x = 1.0f;

	#endif // FOG_LINEAR || FOG_EXP || FOG_EXP2

	// �t�H�O�v�Z =============================================( �I�� )

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
	VSOutput.Fog_ShadowMap3PosZ.y           = dot( lWorldPosition, cfShadowMap3LightViewProjectionMatrix[ 2 ] ) ;

	// �[�x�e�p�̃��C�g���猩���ˉe���W���Z�o =================( �I�� )
#else

	VSOutput.Fog_ShadowMap3PosZ.y = 0.0f;

#endif

	// �e�N�X�`�����W�̃Z�b�g
	VSOutput.TexCoords0_1.x = dot( VSInput.TexCoords0, cfTextureMatrix[ 0 ][ 0 ] ) ;
	VSOutput.TexCoords0_1.y = dot( VSInput.TexCoords0, cfTextureMatrix[ 0 ][ 1 ] ) ;

	VSOutput.TexCoords0_1.z = dot( VSInput.TexCoords1, cfTextureMatrix[ 0 ][ 0 ] ) ;
	VSOutput.TexCoords0_1.w = dot( VSInput.TexCoords1, cfTextureMatrix[ 0 ][ 1 ] ) ;

	return VSOutput ;
}

