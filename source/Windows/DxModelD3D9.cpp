//-----------------------------------------------------------------------------
// 
// 		�c�w���C�u����		���f���f�[�^����v���O����( Direct3D9 )
// 
//  	Ver 3.16d
// 
//-----------------------------------------------------------------------------

// �c�w���C�u�����쐬���p��`
#define __DX_MAKE

#include "../DxCompileConfig.h"

#ifndef DX_NON_DIRECT3D9

#ifndef DX_NON_MODEL

// �C���N���[�h----------------------------------------------------------------
#include "DxGraphicsWin.h"
#include "DxGraphicsD3D9.h"
#include "DxModelD3D9.h"
#include "DxModelWin.h"
#include "DxWinAPI.h"
#include "DxWindow.h"
#include "DxShader_PS_D3D9.h"
#include "DxShader_VS_D3D9.h"
#include "../DxMask.h"

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// �}�N����`------------------------------------------------------------------

#define WORD_MAX		0x10000

// �^��`----------------------------------------------------------------------

// �f�[�^�錾------------------------------------------------------------------

// �P�ʍs��
static MATRIX IdentityMat =
{
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f,
} ;

// �t�u���W���e�u�e�e�[�u��
static DWORD UVNumFVFTable[] =
{
	D_D3DFVF_TEX0,
	D_D3DFVF_TEX1 | D_D3DFVF_TEXCOORDSIZE4( 0 ),
	D_D3DFVF_TEX2 | D_D3DFVF_TEXCOORDSIZE4( 0 ) | D_D3DFVF_TEXCOORDSIZE4( 1 ),
	D_D3DFVF_TEX3 | D_D3DFVF_TEXCOORDSIZE4( 0 ) | D_D3DFVF_TEXCOORDSIZE4( 1 ) | D_D3DFVF_TEXCOORDSIZE4( 2 ),
	D_D3DFVF_TEX4 | D_D3DFVF_TEXCOORDSIZE4( 0 ) | D_D3DFVF_TEXCOORDSIZE4( 1 ) | D_D3DFVF_TEXCOORDSIZE4( 2 ) | D_D3DFVF_TEXCOORDSIZE4( 3 ),
	D_D3DFVF_TEX5 | D_D3DFVF_TEXCOORDSIZE4( 0 ) | D_D3DFVF_TEXCOORDSIZE4( 1 ) | D_D3DFVF_TEXCOORDSIZE4( 2 ) | D_D3DFVF_TEXCOORDSIZE4( 3 ) | D_D3DFVF_TEXCOORDSIZE4( 4 ),
	D_D3DFVF_TEX6 | D_D3DFVF_TEXCOORDSIZE4( 0 ) | D_D3DFVF_TEXCOORDSIZE4( 1 ) | D_D3DFVF_TEXCOORDSIZE4( 2 ) | D_D3DFVF_TEXCOORDSIZE4( 3 ) | D_D3DFVF_TEXCOORDSIZE4( 4 ) | D_D3DFVF_TEXCOORDSIZE4( 5 ),
	D_D3DFVF_TEX7 | D_D3DFVF_TEXCOORDSIZE4( 0 ) | D_D3DFVF_TEXCOORDSIZE4( 1 ) | D_D3DFVF_TEXCOORDSIZE4( 2 ) | D_D3DFVF_TEXCOORDSIZE4( 3 ) | D_D3DFVF_TEXCOORDSIZE4( 4 ) | D_D3DFVF_TEXCOORDSIZE4( 5 ) | D_D3DFVF_TEXCOORDSIZE4( 6 ),
	D_D3DFVF_TEX8 | D_D3DFVF_TEXCOORDSIZE4( 0 ) | D_D3DFVF_TEXCOORDSIZE4( 1 ) | D_D3DFVF_TEXCOORDSIZE4( 2 ) | D_D3DFVF_TEXCOORDSIZE4( 3 ) | D_D3DFVF_TEXCOORDSIZE4( 4 ) | D_D3DFVF_TEXCOORDSIZE4( 5 ) | D_D3DFVF_TEXCOORDSIZE4( 6 ) | D_D3DFVF_TEXCOORDSIZE4( 7 )
} ;

D_D3DTEXTUREFILTERTYPE DrawModeToFilterTable[][ 3 ] =
{
	{ D_D3DTEXF_POINT,       D_D3DTEXF_POINT,  D_D3DTEXF_NONE   },
	{ D_D3DTEXF_LINEAR,      D_D3DTEXF_LINEAR, D_D3DTEXF_NONE   },
	{ D_D3DTEXF_ANISOTROPIC, D_D3DTEXF_LINEAR, D_D3DTEXF_LINEAR },
} ;

MV1_MODEL_MANAGE_DIRECT3D9 MV1Man_D3D9 ;

// �֐��v���g�^�C�v�錾 -------------------------------------------------------

// �w��̃��b�V���̃}�e���A�����Z�b�g�A�b�v����
static void MV1_D3D9_SetupMeshDrawMaterial( MV1_MODEL_BASE * /*ModelBase*/, GRAPHICS_HARDDATA_DIRECT3D9_BLENDINFO *BlendInfo, MV1_MESH *Mesh, bool UseShader, int SpecularEnable ) ;

// �g�D�[���ׂ̈̃}�e���A�����Z�b�g�A�b�v����
__inline void MV1_D3D9_SetupToonOutLineMeshDrawMaterialCommon( GRAPHICS_HARDDATA_DIRECT3D9_BLENDINFO *BlendInfo, MV1_MESH *Mesh, float OutLineWidth ) ;

// �w��̃g���C�A���O�����X�g�̃e���|���������Z�b�g�A�b�v����
static bool MV1_D3D9_SetupTListTempSimpleVertex( MV1_TRIANGLE_LIST_BASE *MBTList ) ;

// �g�D�[���g���C�A���O�����X�g�̃��C�e�B���O���s��
static void MV1_D3D9_TriangleListToonLighting( MV1_TRIANGLE_LIST_BASE *MBTList, bool Normalize = false, MATRIX *LocalWorldMatrix = NULL ) ;

// �w��̃��b�V���̃g�D�[�������_�����O�̗֊s���`��p�̃}�e���A�����Z�b�g�A�b�v����
static void MV1_D3D9_SetupToonOutLineMeshDrawMaterial( MV1_MODEL_BASE *ModelBase, GRAPHICS_HARDDATA_DIRECT3D9_BLENDINFO *BlendInfo, MV1_MESH *Mesh, bool UseShader ) ;

// �w��̃g���C�A���O�����X�g�̗֊s���p�e���|���������Z�b�g�A�b�v����
static bool MV1_D3D9_SetupTListTempToonOutLineSimpleVertex( MV1_TRIANGLE_LIST_BASE *MBTList ) ;

// �v���O����------------------------------------------------------------------

// �w��̃��b�V���̃}�e���A�����Z�b�g�A�b�v����
static void MV1_D3D9_SetupMeshDrawMaterial( MV1_MODEL_BASE * /*ModelBase*/, GRAPHICS_HARDDATA_DIRECT3D9_BLENDINFO *BlendInfo, MV1_MESH *Mesh, bool UseShader, int SpecularEnable )
{
	GRAPHICS_HARDDATA_DIRECT3D9_TEXTURESTAGEINFO *StageInfo ;
	MV1_TEXTURE_BASE *TexBase ;
	MV1_TEXTURE *Tex ;
	MV1_FRAME *Frame ;
	MV1_MODEL *Model ;
	MV1_MATERIAL * RST MMaterial ;
	int StageNum, BlendMode ;
	int TextureUVScaling ;
	float TextureScaleU ;
	float TextureScaleV ;

	// ���ʕ����̃Z�b�g
	Frame = Mesh->Container ;
	Model = Frame->Container ;
	MMaterial = Mesh->Material ;
	BlendMode = Mesh->DrawBlendMode != -1 ? Mesh->DrawBlendMode : MMaterial->DrawBlendMode ;

	if( UseShader == FALSE && ( MMaterial->BaseData->Type == DX_MATERIAL_TYPE_TOON || MMaterial->BaseData->Type == DX_MATERIAL_TYPE_TOON_2 ) )
	{
		SpecularEnable = FALSE ;
	}
	Graphics_D3D9_DeviceState_SetSpecularEnable( SpecularEnable ) ;

	// �f�B�t���[�Y�e�N�X�`���̃X�P�[���� 1.0f �ȊO���𒲂ׂ�
	TextureUVScaling = FALSE ;
	if( Mesh->Material->DiffuseLayerNum > 0 )
	{
		Tex = &Model->Texture[ Mesh->Material->DiffuseLayer[ 0 ].Texture ] ;

		if( Tex->ScaleU - 1.0f >  0.0001f || Tex->ScaleU - 1.0f < -0.0001f ||
			Tex->ScaleV - 1.0f >  0.0001f || Tex->ScaleV - 1.0f < -0.0001f )
		{
			TextureUVScaling = TRUE ;
			TextureScaleU = Tex->ScaleU ;
			TextureScaleV = Tex->ScaleV ;
		}
	}

	// �V�F�[�_�[���g�p�\���ǂ����ŏ����𕪊�
	if( UseShader )
//	if( Shader )
//	if( 0 )
	{
		// �e�N�X�`�����W�ϊ��p�����[�^�̃Z�b�g
		if( Frame->TextureAddressTransformUse == FALSE && TextureUVScaling == FALSE )
		{
			static float IdentityData[ 2 ][ 4 ] =
			{
				1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
			} ;
			Graphics_D3D9_ShaderConstant_InfoSet_SetParam(
				&GD3D9.Device.Shader.ShaderConstantInfo,
				DX_SHADERCONSTANTTYPE_VS_FLOAT,
				DX_SHADERCONSTANTSET_MV1,
				DX_VS_CONSTF_TEXTURE_MATRIX_START,
				IdentityData[ 0 ],
				2,
				FALSE
			) ;
		}
		else
		{
			float Data[ 2 ][ 4 ] ;
			Data[ 0 ][ 0 ] = Frame->TextureAddressTransformMatrix.m[ 0 ][ 0 ] ;
			Data[ 0 ][ 1 ] = Frame->TextureAddressTransformMatrix.m[ 1 ][ 0 ] ;
			Data[ 0 ][ 2 ] = Frame->TextureAddressTransformMatrix.m[ 2 ][ 0 ] ;
			Data[ 0 ][ 3 ] = Frame->TextureAddressTransformMatrix.m[ 3 ][ 0 ] ;
			Data[ 1 ][ 0 ] = Frame->TextureAddressTransformMatrix.m[ 0 ][ 1 ] ;
			Data[ 1 ][ 1 ] = Frame->TextureAddressTransformMatrix.m[ 1 ][ 1 ] ;
			Data[ 1 ][ 2 ] = Frame->TextureAddressTransformMatrix.m[ 2 ][ 1 ] ;
			Data[ 1 ][ 3 ] = Frame->TextureAddressTransformMatrix.m[ 3 ][ 1 ] ;
			if( TextureUVScaling )
			{
				Data[ 0 ][ 0 ] *= TextureScaleU ;
				Data[ 1 ][ 1 ] *= TextureScaleV ;
			}
			Graphics_D3D9_ShaderConstant_InfoSet_SetParam(
				&GD3D9.Device.Shader.ShaderConstantInfo,
				DX_SHADERCONSTANTTYPE_VS_FLOAT,
				DX_SHADERCONSTANTSET_MV1,
				DX_VS_CONSTF_TEXTURE_MATRIX_START,
				Data[ 0 ],
				2,
				FALSE
			) ;
		}

		// ���܂łƃ}�e���A�����Ⴄ�ꍇ�̓}�e���A���̃Z�b�g
//		if( MV1Man.SetMaterial != MBMesh->Material )
		{
			// �e�N�X�`���̃Z�b�g�A�b�v
			StageInfo = BlendInfo->TextureStageInfo ;
			BlendInfo->UseTextureStageNum = 0 ;

			StageInfo[ 0 ].Texture = NULL ;
			StageInfo[ 1 ].Texture = NULL ;
			StageInfo[ 2 ].Texture = NULL ;
			StageInfo[ 3 ].Texture = NULL ;
			StageInfo[ 4 ].Texture = NULL ;
			StageInfo[ 5 ].Texture = NULL ;
			StageInfo[ 6 ].Texture = NULL ;
			StageInfo[ 7 ].Texture = NULL ;
			StageInfo[ 8 ].Texture = NULL ;
			StageInfo[ 9 ].Texture = NULL ;
			StageInfo[ 10 ].Texture = NULL ;
			StageInfo[ 11 ].Texture = NULL ;

			StageInfo[ 0 ].TextureCoordIndex = 0 ;
			StageInfo[ 0 ].ResultTempARG = FALSE ;
			if( Mesh->Material->DiffuseLayerNum )
			{
				BlendInfo->UseTextureStageNum = 1 ;
				Tex = &Model->Texture[ Mesh->Material->DiffuseLayer[ 0 ].Texture ] ;
				TexBase = Tex->BaseData ;
				     if( Tex->UseUserGraphHandle     ) StageInfo[ 0 ].Texture = ( void * )( DWORD_PTR )Tex->UserGraphHandle ;
				else if( Tex->UseGraphHandle         ) StageInfo[ 0 ].Texture = ( void * )( DWORD_PTR )Tex->GraphHandle ;
				else if( TexBase->UseUserGraphHandle ) StageInfo[ 0 ].Texture = ( void * )( DWORD_PTR )TexBase->UserGraphHandle ;
				else                                   StageInfo[ 0 ].Texture = ( void * )( DWORD_PTR )TexBase->GraphHandle ;
				Graphics_D3D9_DeviceState_SetTextureAddressUVW( Tex->AddressModeU, Tex->AddressModeV, D_D3DTADDRESS_CLAMP, 0 ) ;

				Graphics_D3D9_DeviceState_SetSampleFilterMode( 0, D_D3DTSS_MINFILTER, DrawModeToFilterTable[ Tex->FilterMode ][ 0 ] ) ;
				Graphics_D3D9_DeviceState_SetSampleFilterMode( 0, D_D3DTSS_MAGFILTER, DrawModeToFilterTable[ Tex->FilterMode ][ 1 ] ) ;
				Graphics_D3D9_DeviceState_SetSampleFilterMode( 0, D_D3DTSS_MIPFILTER, DrawModeToFilterTable[ Tex->FilterMode ][ 2 ] ) ;
			}
			else
			{
				// �f�t�H���g�e�N�X�`���n���h���̏������`�F�b�N
				if( MV1Man.TexNoneHandle < 0 )
				{
					MV1SetupTexNoneHandle() ;
					Graphics_D3D9_BeginScene() ;
				}
				BlendInfo->UseTextureStageNum = 1 ;
				StageInfo[ 0 ].Texture = ( void * )( DWORD_PTR )MV1Man.TexNoneHandle ;
			}

			StageInfo[ 1 ].TextureCoordIndex = 1 ;
			StageInfo[ 1 ].ResultTempARG = FALSE ;
			if( Mesh->BaseData->Material->NormalLayerNum )
			{
				BlendInfo->UseTextureStageNum = 2 ;
				Tex = &Model->Texture[ Mesh->BaseData->Material->NormalLayer[ 0 ].Texture ] ;
				TexBase = Tex->BaseData ;
				     if( Tex->UseUserGraphHandle     ) StageInfo[ 1 ].Texture = ( void * )( DWORD_PTR )Tex->UserGraphHandle ;
				else if( Tex->UseGraphHandle         ) StageInfo[ 1 ].Texture = ( void * )( DWORD_PTR )Tex->GraphHandle ;
				else if( TexBase->UseUserGraphHandle ) StageInfo[ 1 ].Texture = ( void * )( DWORD_PTR )TexBase->UserGraphHandle ;
				else                                   StageInfo[ 1 ].Texture = ( void * )( DWORD_PTR )TexBase->GraphHandle ;
				Graphics_D3D9_DeviceState_SetTextureAddressUVW( Tex->AddressModeU, Tex->AddressModeV, D_D3DTADDRESS_CLAMP, 1 ) ;

				Graphics_D3D9_DeviceState_SetSampleFilterMode( 1, D_D3DTSS_MINFILTER, DrawModeToFilterTable[ Tex->FilterMode ][ 0 ] ) ;
				Graphics_D3D9_DeviceState_SetSampleFilterMode( 1, D_D3DTSS_MAGFILTER, DrawModeToFilterTable[ Tex->FilterMode ][ 1 ] ) ;
				Graphics_D3D9_DeviceState_SetSampleFilterMode( 1, D_D3DTSS_MIPFILTER, DrawModeToFilterTable[ Tex->FilterMode ][ 2 ] ) ;
			}

			StageInfo[ 2 ].TextureCoordIndex = 2 ;
			StageInfo[ 2 ].ResultTempARG = FALSE ;
			if( Mesh->Material->SpecularLayerNum )
			{
				BlendInfo->UseTextureStageNum = 3 ;
				Tex = &Model->Texture[ Mesh->Material->SpecularLayer[ 0 ].Texture ] ;
				TexBase = Tex->BaseData ;
				     if( Tex->UseUserGraphHandle     ) StageInfo[ 2 ].Texture = ( void * )( DWORD_PTR )Tex->UserGraphHandle ;
				else if( Tex->UseGraphHandle         ) StageInfo[ 2 ].Texture = ( void * )( DWORD_PTR )Tex->GraphHandle ;
				else if( TexBase->UseUserGraphHandle ) StageInfo[ 2 ].Texture = ( void * )( DWORD_PTR )TexBase->UserGraphHandle ;
				else                                   StageInfo[ 2 ].Texture = ( void * )( DWORD_PTR )TexBase->GraphHandle ;
				Graphics_D3D9_DeviceState_SetTextureAddressUVW( Tex->AddressModeU, Tex->AddressModeV, D_D3DTADDRESS_CLAMP, 2 ) ;

				Graphics_D3D9_DeviceState_SetSampleFilterMode( 2, D_D3DTSS_MINFILTER, DrawModeToFilterTable[ Tex->FilterMode ][ 0 ] ) ;
				Graphics_D3D9_DeviceState_SetSampleFilterMode( 2, D_D3DTSS_MAGFILTER, DrawModeToFilterTable[ Tex->FilterMode ][ 1 ] ) ;
				Graphics_D3D9_DeviceState_SetSampleFilterMode( 2, D_D3DTSS_MIPFILTER, DrawModeToFilterTable[ Tex->FilterMode ][ 2 ] ) ;
			}
			else
			{
				if( Mesh->Material->BaseData->Type == DX_MATERIAL_TYPE_TOON || Mesh->Material->BaseData->Type == DX_MATERIAL_TYPE_TOON_2 )
				{
					// �f�t�H���g�e�N�X�`���n���h���̏������`�F�b�N
					if( MV1Man.TexNoneHandle < 0 )
					{
						MV1SetupTexNoneHandle() ;
						Graphics_D3D9_BeginScene() ;
					}
					StageInfo[ 2 ].Texture = ( void * )( DWORD_PTR )MV1Man.TexNoneHandle ;
				}
			}

			if( Mesh->Material->BaseData->Type == DX_MATERIAL_TYPE_TOON || Mesh->Material->BaseData->Type == DX_MATERIAL_TYPE_TOON_2 )
			{
				BlendInfo->UseTextureStageNum = 5 ;

				if( Mesh->Material->DiffuseGradTexture >= 0 )
				{
					Tex = &Model->Texture[ Mesh->Material->DiffuseGradTexture ] ;
					TexBase = Tex->BaseData ;
						 if( Tex->UseUserGraphHandle     ) StageInfo[ 3 ].Texture = ( void * )( DWORD_PTR )Tex->UserGraphHandle ;
					else if( Tex->UseGraphHandle         ) StageInfo[ 3 ].Texture = ( void * )( DWORD_PTR )Tex->GraphHandle ;
					else if( TexBase->UseUserGraphHandle ) StageInfo[ 3 ].Texture = ( void * )( DWORD_PTR )TexBase->UserGraphHandle ;
					else                                   StageInfo[ 3 ].Texture = ( void * )( DWORD_PTR )TexBase->GraphHandle ;
					Graphics_D3D9_DeviceState_SetTextureAddressUVW( Tex->AddressModeU, Tex->AddressModeV, D_D3DTADDRESS_CLAMP, 3 ) ;

					Graphics_D3D9_DeviceState_SetSampleFilterMode( 3, D_D3DTSS_MINFILTER, DrawModeToFilterTable[ Tex->FilterMode ][ 0 ] ) ;
					Graphics_D3D9_DeviceState_SetSampleFilterMode( 3, D_D3DTSS_MAGFILTER, DrawModeToFilterTable[ Tex->FilterMode ][ 1 ] ) ;
					Graphics_D3D9_DeviceState_SetSampleFilterMode( 3, D_D3DTSS_MIPFILTER, DrawModeToFilterTable[ Tex->FilterMode ][ 2 ] ) ;
				}
				else
				{
					if( Mesh->Material->BaseData->Type == DX_MATERIAL_TYPE_TOON )
					{
						StageInfo[ 3 ].Texture = ( void * )( DWORD_PTR )MV1GetDefaultToonTexture( Mesh->Material->DiffuseGradTexture ) ;
						Graphics_D3D9_BeginScene() ;
					}
					else
					{
						StageInfo[ 3 ].Texture = ( void * )( DWORD_PTR )MV1Man.TexNoneHandle ;
					}
				}
				StageInfo[ 3 ].TextureCoordIndex = 3 ;
				StageInfo[ 3 ].ResultTempARG = FALSE ;

				if( Mesh->Material->SpecularGradTexture >= 0 )
				{
					Tex = &Model->Texture[ Mesh->Material->SpecularGradTexture ] ;
					TexBase = Tex->BaseData ;
						 if( Tex->UseUserGraphHandle     ) StageInfo[ 4 ].Texture = ( void * )( DWORD_PTR )Tex->UserGraphHandle ;
					else if( Tex->UseGraphHandle         ) StageInfo[ 4 ].Texture = ( void * )( DWORD_PTR )Tex->GraphHandle ;
					else if( TexBase->UseUserGraphHandle ) StageInfo[ 4 ].Texture = ( void * )( DWORD_PTR )TexBase->UserGraphHandle ;
					else                                   StageInfo[ 4 ].Texture = ( void * )( DWORD_PTR )TexBase->GraphHandle ;
					Graphics_D3D9_DeviceState_SetTextureAddressUVW( Tex->AddressModeU, Tex->AddressModeV, D_D3DTADDRESS_CLAMP, 4 ) ;

					Graphics_D3D9_DeviceState_SetSampleFilterMode( 4, D_D3DTSS_MINFILTER, DrawModeToFilterTable[ Tex->FilterMode ][ 0 ] ) ;
					Graphics_D3D9_DeviceState_SetSampleFilterMode( 4, D_D3DTSS_MAGFILTER, DrawModeToFilterTable[ Tex->FilterMode ][ 1 ] ) ;
					Graphics_D3D9_DeviceState_SetSampleFilterMode( 4, D_D3DTSS_MIPFILTER, DrawModeToFilterTable[ Tex->FilterMode ][ 2 ] ) ;
				}
				else
				{
					StageInfo[ 4 ].Texture = ( void * )( DWORD_PTR )MV1GetDefaultToonTexture( Mesh->Material->SpecularGradTexture ) ;
					Graphics_D3D9_BeginScene() ;
				}
				StageInfo[ 4 ].TextureCoordIndex = 4 ;
				StageInfo[ 4 ].ResultTempARG = FALSE ;

				if( Mesh->Material->SphereMapTexture >= 0 )
				{
					if( MV1Man.TexNoneHandle < 0 )
					{
						MV1SetupTexNoneHandle() ;
						Graphics_D3D9_BeginScene() ;
					}

					BlendInfo->UseTextureStageNum = 6 ;
					Tex = &Model->Texture[ Mesh->Material->SphereMapTexture ] ;
					TexBase = Tex->BaseData ;
						 if( Tex->UseUserGraphHandle                                ) StageInfo[ 5 ].Texture = ( void * )( DWORD_PTR )Tex->UserGraphHandle ;
					else if( Tex->UseGraphHandle  && Tex->IsDefaultTexture == FALSE ) StageInfo[ 5 ].Texture = ( void * )( DWORD_PTR )Tex->GraphHandle ;
					else if( TexBase->UseUserGraphHandle                            ) StageInfo[ 5 ].Texture = ( void * )( DWORD_PTR )TexBase->UserGraphHandle ;
					else if( TexBase->IsDefaultTexture == FALSE                     ) StageInfo[ 5 ].Texture = ( void * )( DWORD_PTR )TexBase->GraphHandle ;
					else
					{
						if( Mesh->Material->SphereMapBlendType == DX_MATERIAL_BLENDTYPE_MODULATE )
						{
							StageInfo[ 5 ].Texture = ( void * )( DWORD_PTR )MV1Man.TexNoneHandle ;
						}
						else
						{
							StageInfo[ 5 ].Texture = ( void * )( DWORD_PTR )MV1Man.TexNoneBlackHandle ;
						}
					}
					Graphics_D3D9_DeviceState_SetTextureAddressUVW( Tex->AddressModeU, Tex->AddressModeV, D_D3DTADDRESS_CLAMP, 5 ) ;

					Graphics_D3D9_DeviceState_SetSampleFilterMode( 5, D_D3DTSS_MINFILTER, DrawModeToFilterTable[ Tex->FilterMode ][ 0 ] ) ;
					Graphics_D3D9_DeviceState_SetSampleFilterMode( 5, D_D3DTSS_MAGFILTER, DrawModeToFilterTable[ Tex->FilterMode ][ 1 ] ) ;
					Graphics_D3D9_DeviceState_SetSampleFilterMode( 5, D_D3DTSS_MIPFILTER, DrawModeToFilterTable[ Tex->FilterMode ][ 2 ] ) ;

					StageInfo[ 5 ].TextureCoordIndex = 5 ;
					StageInfo[ 5 ].ResultTempARG = FALSE ;
				}

				Graphics_D3D9_DeviceState_SetSampleFilterMode( 6, D_D3DTSS_MINFILTER, D_D3DTEXF_LINEAR ) ;
				Graphics_D3D9_DeviceState_SetSampleFilterMode( 6, D_D3DTSS_MAGFILTER, D_D3DTEXF_LINEAR ) ;
				Graphics_D3D9_DeviceState_SetSampleFilterMode( 6, D_D3DTSS_MIPFILTER, D_D3DTEXF_LINEAR ) ;
				Graphics_D3D9_DeviceState_SetTextureAddressUVW( DX_TEXADDRESS_CLAMP, DX_TEXADDRESS_CLAMP, D_D3DTADDRESS_CLAMP, 6 ) ;

				{
					float OutLineData[ 8 ] ;

					OutLineData[ 0 ] = Mesh->Material->OutLineColor.r ;
					OutLineData[ 1 ] = Mesh->Material->OutLineColor.g ;
					OutLineData[ 2 ] = Mesh->Material->OutLineColor.b ;
					OutLineData[ 3 ] = Mesh->Material->OutLineColor.a ;
					OutLineData[ 4 ] = 1.0f / Mesh->Material->OutLineWidth ;
					OutLineData[ 5 ] = 1.0f / Mesh->Material->OutLineWidth ;
					OutLineData[ 6 ] = 1.0f / Mesh->Material->OutLineWidth ;
					OutLineData[ 7 ] = 1.0f / Mesh->Material->OutLineWidth ;
					Graphics_D3D9_ShaderConstant_InfoSet_SetParam(
						&GD3D9.Device.Shader.ShaderConstantInfo,
						DX_SHADERCONSTANTTYPE_PS_FLOAT,
						DX_SHADERCONSTANTSET_MV1,
						DX_PS_CONSTF_TOON_OUTLINE_COLOR,
						OutLineData,
						2,
						FALSE
					) ;
				}
			}

			StageInfo[ 7 ].TextureCoordIndex = 0 ;
			StageInfo[ 7 ].ResultTempARG = FALSE ;
			if( Mesh->Material->DiffuseLayerNum > 1 )
			{
				BlendInfo->UseTextureStageNum = 8 ;
				Tex = &Model->Texture[ Mesh->Material->DiffuseLayer[ 1 ].Texture ] ;
				TexBase = Tex->BaseData ;
				     if( Tex->UseUserGraphHandle     ) StageInfo[ 7 ].Texture = ( void * )( DWORD_PTR )Tex->UserGraphHandle ;
				else if( Tex->UseGraphHandle         ) StageInfo[ 7 ].Texture = ( void * )( DWORD_PTR )Tex->GraphHandle ;
				else if( TexBase->UseUserGraphHandle ) StageInfo[ 7 ].Texture = ( void * )( DWORD_PTR )TexBase->UserGraphHandle ;
				else                                   StageInfo[ 7 ].Texture = ( void * )( DWORD_PTR )TexBase->GraphHandle ;
				Graphics_D3D9_DeviceState_SetTextureAddressUVW( Tex->AddressModeU, Tex->AddressModeV, D_D3DTADDRESS_CLAMP, 7 ) ;

				Graphics_D3D9_DeviceState_SetSampleFilterMode( 7, D_D3DTSS_MINFILTER, DrawModeToFilterTable[ Tex->FilterMode ][ 0 ] ) ;
				Graphics_D3D9_DeviceState_SetSampleFilterMode( 7, D_D3DTSS_MAGFILTER, DrawModeToFilterTable[ Tex->FilterMode ][ 1 ] ) ;
				Graphics_D3D9_DeviceState_SetSampleFilterMode( 7, D_D3DTSS_MIPFILTER, DrawModeToFilterTable[ Tex->FilterMode ][ 2 ] ) ;
			}

			// �V���h�E�}�b�v�̐ݒ�
			{
				int i ;

				for( i = 0 ; i < MAX_USE_SHADOWMAP_NUM ; i ++ )
				{
					StageInfo[ 8 + i ].TextureCoordIndex = 0 ;
					StageInfo[ 8 + i ].ResultTempARG = FALSE ;
					if( GSYS.DrawSetting.ShadowMap[ i ] > 0 )
					{
						StageInfo[ 8 + i ].Texture = ( void * )( DWORD_PTR )GSYS.DrawSetting.ShadowMap[ i ] ;
						Graphics_D3D9_DeviceState_SetTextureAddressUVW( DX_TEXADDRESS_CLAMP, DX_TEXADDRESS_CLAMP, D_D3DTADDRESS_CLAMP, 8 + i ) ;

						Graphics_D3D9_DeviceState_SetSampleFilterMode( 8 + i, D_D3DTSS_MINFILTER, D_D3DTEXF_LINEAR ) ;
						Graphics_D3D9_DeviceState_SetSampleFilterMode( 8 + i, D_D3DTSS_MAGFILTER, D_D3DTEXF_LINEAR ) ;
						Graphics_D3D9_DeviceState_SetSampleFilterMode( 8 + i, D_D3DTSS_MIPFILTER, D_D3DTEXF_LINEAR ) ;
					}
					else
					{
						StageInfo[ 8 + i ].Texture = NULL ;
					}
				}
				BlendInfo->UseTextureStageNum = 9 + MAX_USE_SHADOWMAP_NUM - 1 ;
			}

			// ���[�U�[�ݒ���g�p����ꍇ�͂����Ńe�N�X�`�����Z�b�g
			if( MV1Man.UseOrigShaderFlag )
			{
				int i ;
				int LastNo ;

				LastNo = -1 ;
				for( i = 0 ; i < USE_TEXTURESTAGE_NUM ; i ++ )
				{
					if( GSYS.DrawSetting.UserShaderRenderInfo.SetTextureGraphHandle[ i ] == 0 )
					{
						if( BlendInfo->UseTextureStageNum <= i )
						{
							StageInfo[ i ].Texture = NULL ;
							StageInfo[ i ].TextureCoordIndex = 0 ;
							StageInfo[ i ].ResultTempARG = FALSE ;
						}
						continue ;
					}

					LastNo = i ;
					StageInfo[ i ].Texture = ( void * )( DWORD_PTR )GSYS.DrawSetting.UserShaderRenderInfo.SetTextureGraphHandle[ i ] ;
					Graphics_D3D9_DeviceState_SetTextureAddressUVW( GSYS.DrawSetting.TexAddressModeU[ i ], GSYS.DrawSetting.TexAddressModeV[ i ], GSYS.DrawSetting.TexAddressModeW[ i ], i ) ;

					Graphics_D3D9_DeviceState_SetSampleFilterMode( i, D_D3DTSS_MINFILTER, DrawModeToFilterTable[ GSYS.DrawSetting.DrawMode ][ 0 ] ) ;
					Graphics_D3D9_DeviceState_SetSampleFilterMode( i, D_D3DTSS_MAGFILTER, DrawModeToFilterTable[ GSYS.DrawSetting.DrawMode ][ 1 ] ) ;
					Graphics_D3D9_DeviceState_SetSampleFilterMode( i, D_D3DTSS_MIPFILTER, DrawModeToFilterTable[ GSYS.DrawSetting.DrawMode ][ 2 ] ) ;
				}

				if( LastNo > 0 && LastNo + 1 > BlendInfo->UseTextureStageNum )
				{
					BlendInfo->UseTextureStageNum = LastNo + 1 ;
				}
			}

			// �ݒ�𔽉f
			Graphics_D3D9_DeviceState_SetUserBlendInfo( BlendInfo, TRUE ) ;

			// �d�l�o�O�̂��߂U�Ԃ̃e�N�X�`���̂ݓ��ʑΉ�
			if( Mesh->Material->BaseData->Type == DX_MATERIAL_TYPE_TOON )
			{
				if( StageInfo[ 6 ].Texture == NULL )
				{
					Direct3DDevice9_SetTexture( 6, GD3D9.RGBtoVMaxRGBVolumeTexture ) ;
					GD3D9.Device.State.BlendInfo.TextureStageInfo[ 6 ].Texture = GD3D9.RGBtoVMaxRGBVolumeTexture ;
				}
			}

			// �g�p����e�N�X�`�����W�����Z�b�g
			Graphics_D3D9_DeviceState_ResetTextureCoord() ;
		}
	}
	else
	{
		// �e�N�X�`�����W�ϊ��ݒ�
		if( TextureUVScaling == FALSE )
		{
			Graphics_DrawSetting_SetTextureAddressTransformMatrix_Direct( Frame->TextureAddressTransformUse, &Frame->TextureAddressTransformMatrix, 0 ) ;
			Graphics_DrawSetting_SetTextureAddressTransformMatrix_Direct( Frame->TextureAddressTransformUse, &Frame->TextureAddressTransformMatrix, 1 ) ;
			Graphics_DrawSetting_SetTextureAddressTransformMatrix_Direct( Frame->TextureAddressTransformUse, &Frame->TextureAddressTransformMatrix, 2 ) ;
			Graphics_DrawSetting_SetTextureAddressTransformMatrix_Direct( Frame->TextureAddressTransformUse, &Frame->TextureAddressTransformMatrix, 3 ) ;
		}
		else
		{
			MATRIX TempUVMatrix ;

			if( Frame->TextureAddressTransformUse )
			{
				TempUVMatrix = Frame->TextureAddressTransformMatrix ;
			}
			else
			{
				CreateIdentityMatrix( &TempUVMatrix ) ;
			}
			TempUVMatrix.m[ 0 ][ 0 ] *= TextureScaleU ;
			TempUVMatrix.m[ 1 ][ 1 ] *= TextureScaleV ;
			Graphics_DrawSetting_SetTextureAddressTransformMatrix_Direct( TRUE, &TempUVMatrix, 0 ) ;
			Graphics_DrawSetting_SetTextureAddressTransformMatrix_Direct( TRUE, &TempUVMatrix, 1 ) ;
			Graphics_DrawSetting_SetTextureAddressTransformMatrix_Direct( TRUE, &TempUVMatrix, 2 ) ;
			Graphics_DrawSetting_SetTextureAddressTransformMatrix_Direct( TRUE, &TempUVMatrix, 3 ) ;
		}

		// ���܂łƃ}�e���A�����Ⴄ�ꍇ�̓}�e���A���̃Z�b�g
//		if( MV1Man.SetMaterial != Mesh->Material )
		{
			// �e�N�X�`���̃Z�b�g�A�b�v
			StageInfo = BlendInfo->TextureStageInfo ;
			StageNum = 0 ;

			StageInfo[ StageNum ].Texture = NULL ;
			if( Mesh->Material->DiffuseLayerNum )
			{
				Tex = &Model->Texture[ Mesh->Material->DiffuseLayer[ 0 ].Texture ] ;
				TexBase = Tex->BaseData ;
					 if( Tex->UseUserGraphHandle     ) StageInfo[ StageNum ].Texture = ( void * )( DWORD_PTR )Tex->UserGraphHandle ;
				else if( Tex->UseGraphHandle         ) StageInfo[ StageNum ].Texture = ( void * )( DWORD_PTR )Tex->GraphHandle ;
				else if( TexBase->UseUserGraphHandle ) StageInfo[ StageNum ].Texture = ( void * )( DWORD_PTR )TexBase->UserGraphHandle ;
				else                                   StageInfo[ StageNum ].Texture = ( void * )( DWORD_PTR )TexBase->GraphHandle ;
				Graphics_D3D9_DeviceState_SetTextureAddressUVW( Tex->AddressModeU, Tex->AddressModeV, D_D3DTADDRESS_CLAMP, StageNum ) ;

				Graphics_D3D9_DeviceState_SetSampleFilterMode( StageNum, D_D3DTSS_MINFILTER, DrawModeToFilterTable[ Tex->FilterMode ][ 0 ] ) ;
				Graphics_D3D9_DeviceState_SetSampleFilterMode( StageNum, D_D3DTSS_MAGFILTER, DrawModeToFilterTable[ Tex->FilterMode ][ 1 ] ) ;
				Graphics_D3D9_DeviceState_SetSampleFilterMode( StageNum, D_D3DTSS_MIPFILTER, DrawModeToFilterTable[ Tex->FilterMode ][ 2 ] ) ;
			}
			else
			{
				// �f�t�H���g�e�N�X�`���n���h���̏������`�F�b�N
				if( MV1Man.TexNoneHandle < 0 )
				{
					MV1SetupTexNoneHandle() ;
					Graphics_D3D9_BeginScene() ;
				}
				StageInfo[ StageNum ].Texture = ( void * )( DWORD_PTR )MV1Man.TexNoneHandle ;
			}
			StageInfo[ StageNum ].TextureCoordIndex = 0 ;
			StageInfo[ StageNum ].ResultTempARG = FALSE ;
			StageInfo[ StageNum ].ColorARG1 = D_D3DTA_TEXTURE ;
			StageInfo[ StageNum ].ColorARG2 = D_D3DTA_DIFFUSE ;
			StageInfo[ StageNum ].ColorOP   = D_D3DTOP_MODULATE ;
			StageInfo[ StageNum ].AlphaARG1 = D_D3DTA_TEXTURE ;
			StageInfo[ StageNum ].AlphaARG2 = D_D3DTA_DIFFUSE ;
			StageInfo[ StageNum ].AlphaOP   = D_D3DTOP_MODULATE ;
			StageNum ++ ;

			// �g�D�[�������_�����O�̏ꍇ
			if( MMaterial->BaseData->Type == DX_MATERIAL_TYPE_TOON || MMaterial->BaseData->Type == DX_MATERIAL_TYPE_TOON_2 )
			{
				// �X�t�B�A�}�b�v������ꍇ�̓e�N�X�`���~�f�B�t���[�Y�J���[�ɏ�Z����
				if( MMaterial->BaseData->Type == DX_MATERIAL_TYPE_TOON_2 && MMaterial->SphereMapTexture >= 0 && Mesh->Material->SphereMapBlendType != DX_MATERIAL_BLENDTYPE_NONE )
				{
					Tex = &Model->Texture[ Mesh->Material->SphereMapTexture ] ;
					TexBase = Tex->BaseData ;
						 if( Tex->UseUserGraphHandle                               ) StageInfo[ StageNum ].Texture = ( void * )( DWORD_PTR )Tex->UserGraphHandle ;
					else if( Tex->UseGraphHandle && Tex->IsDefaultTexture == FALSE ) StageInfo[ StageNum ].Texture = ( void * )( DWORD_PTR )Tex->UserGraphHandle ;
					else if( TexBase->UseUserGraphHandle                           ) StageInfo[ StageNum ].Texture = ( void * )( DWORD_PTR )TexBase->UserGraphHandle ;
					else if( TexBase->IsDefaultTexture == FALSE                    ) StageInfo[ StageNum ].Texture = ( void * )( DWORD_PTR )TexBase->GraphHandle ;
					else
					{
						if( MMaterial->SphereMapBlendType == DX_MATERIAL_BLENDTYPE_MODULATE )
						{
							StageInfo[ StageNum ].Texture = ( void * )( DWORD_PTR )MV1Man.TexNoneHandle ;
						}
						else
						{
							StageInfo[ StageNum ].Texture = ( void * )( DWORD_PTR )MV1Man.TexNoneBlackHandle ;
						}
					}
					Graphics_D3D9_DeviceState_SetTextureAddressUVW( DX_TEXADDRESS_CLAMP, DX_TEXADDRESS_CLAMP, D_D3DTADDRESS_CLAMP, StageNum ) ;

					Graphics_D3D9_DeviceState_SetSampleFilterMode( StageNum, D_D3DTSS_MINFILTER, DrawModeToFilterTable[ Tex->FilterMode ][ 0 ] ) ;
					Graphics_D3D9_DeviceState_SetSampleFilterMode( StageNum, D_D3DTSS_MAGFILTER, DrawModeToFilterTable[ Tex->FilterMode ][ 1 ] ) ;
					Graphics_D3D9_DeviceState_SetSampleFilterMode( StageNum, D_D3DTSS_MIPFILTER, DrawModeToFilterTable[ Tex->FilterMode ][ 2 ] ) ;


					StageInfo[ StageNum ].TextureCoordIndex = 2 ;
					StageInfo[ StageNum ].ResultTempARG = FALSE ;
					switch( Mesh->Material->SphereMapBlendType )
					{
					case DX_MATERIAL_BLENDTYPE_MODULATE :
						StageInfo[ StageNum ].ColorARG1 = D_D3DTA_TEXTURE ;
						StageInfo[ StageNum ].ColorARG2 = D_D3DTA_CURRENT ;
						StageInfo[ StageNum ].ColorOP   = D_D3DTOP_MODULATE ;
						StageInfo[ StageNum ].AlphaARG1 = D_D3DTA_CURRENT ;
						StageInfo[ StageNum ].AlphaARG2 = D_D3DTA_CURRENT ;
						StageInfo[ StageNum ].AlphaOP   = D_D3DTOP_SELECTARG1 ;
						break ;

					case DX_MATERIAL_BLENDTYPE_ADDITIVE :
						StageInfo[ StageNum ].ColorARG1 = D_D3DTA_TEXTURE ;
						StageInfo[ StageNum ].ColorARG2 = D_D3DTA_CURRENT ;
						StageInfo[ StageNum ].ColorOP   = D_D3DTOP_ADD ;
						StageInfo[ StageNum ].AlphaARG1 = D_D3DTA_CURRENT ;
						StageInfo[ StageNum ].AlphaARG2 = D_D3DTA_CURRENT ;
						StageInfo[ StageNum ].AlphaOP   = D_D3DTOP_SELECTARG1 ;
						break ;
					}
					StageNum ++ ;
				}

				// �f�B�t���[�Y�O���f�[�V�����e�N�X�`�����Z�b�g
				if( Mesh->Material->DiffuseGradTexture != -1 )
				{
					Tex = &Model->Texture[ Mesh->Material->DiffuseGradTexture ] ;
					TexBase = Tex->BaseData ;
						 if( Tex->UseUserGraphHandle     ) StageInfo[ StageNum ].Texture = ( void * )( DWORD_PTR )Tex->UserGraphHandle ;
					else if( Tex->UseGraphHandle         ) StageInfo[ StageNum ].Texture = ( void * )( DWORD_PTR )Tex->GraphHandle ;
					else if( TexBase->UseUserGraphHandle ) StageInfo[ StageNum ].Texture = ( void * )( DWORD_PTR )TexBase->UserGraphHandle ;
					else                                   StageInfo[ StageNum ].Texture = ( void * )( DWORD_PTR )TexBase->GraphHandle ;
					Graphics_D3D9_DeviceState_SetTextureAddressUVW( DX_TEXADDRESS_CLAMP, DX_TEXADDRESS_CLAMP, D_D3DTADDRESS_CLAMP, StageNum ) ;

					Graphics_D3D9_DeviceState_SetSampleFilterMode( StageNum, D_D3DTSS_MINFILTER, DrawModeToFilterTable[ Tex->FilterMode ][ 0 ] ) ;
					Graphics_D3D9_DeviceState_SetSampleFilterMode( StageNum, D_D3DTSS_MAGFILTER, DrawModeToFilterTable[ Tex->FilterMode ][ 1 ] ) ;
					Graphics_D3D9_DeviceState_SetSampleFilterMode( StageNum, D_D3DTSS_MIPFILTER, DrawModeToFilterTable[ Tex->FilterMode ][ 2 ] ) ;
				}
				else
				{
					StageInfo[ StageNum ].Texture = ( void * )( DWORD_PTR )MV1GetDefaultToonTexture( Mesh->Material->DiffuseGradTexture ) ;
					Graphics_D3D9_BeginScene() ;
				}

				StageInfo[ StageNum ].TextureCoordIndex = 1 ;
				StageInfo[ StageNum ].ResultTempARG = FALSE ;
				switch( Mesh->Material->DiffuseGradBlendType )
				{
				case DX_MATERIAL_BLENDTYPE_TRANSLUCENT :
					StageInfo[ StageNum ].ColorARG1 = D_D3DTA_TEXTURE ;
					StageInfo[ StageNum ].ColorARG2 = D_D3DTA_CURRENT ;
					StageInfo[ StageNum ].ColorOP   = D_D3DTOP_BLENDTEXTUREALPHA ;
					StageInfo[ StageNum ].AlphaARG1 = D_D3DTA_CURRENT ;
					StageInfo[ StageNum ].AlphaARG2 = D_D3DTA_CURRENT ;
					StageInfo[ StageNum ].AlphaOP   = D_D3DTOP_SELECTARG1 ;
					break ;

				case DX_MATERIAL_BLENDTYPE_MODULATE :
					StageInfo[ StageNum ].ColorARG1 = D_D3DTA_TEXTURE ;
					StageInfo[ StageNum ].ColorARG2 = D_D3DTA_CURRENT ;
					StageInfo[ StageNum ].ColorOP   = D_D3DTOP_MODULATE ;
					StageInfo[ StageNum ].AlphaARG1 = D_D3DTA_CURRENT ;
					StageInfo[ StageNum ].AlphaARG2 = D_D3DTA_CURRENT ;
					StageInfo[ StageNum ].AlphaOP   = D_D3DTOP_SELECTARG1 ;
					break ;
				}
				StageNum ++ ;

				// �g�D�[���^�C�v�ɂ���ď����𕪊�
				switch( MMaterial->BaseData->Type )
				{
				case DX_MATERIAL_TYPE_TOON :
					// �e���|�������W�X�^���g�p�ł��Ȃ��ꍇ�� DX_MATERIAL_BLENDTYPE_ADDITIVE ���w�肳��Ă����ꍇ�̓X�y�L�����p�Œ�e�N�X�`�����g�p����
					if( ( GD3D9.Device.Caps.ValidTexTempRegFlag == TRUE || Mesh->Material->SpecularGradBlendType != DX_MATERIAL_BLENDTYPE_ADDITIVE ) && Mesh->Material->SpecularGradTexture != -1 )
					{
						Tex = &Model->Texture[ Mesh->Material->SpecularGradTexture ] ;
						TexBase = Tex->BaseData ;
							 if( Tex->UseUserGraphHandle     ) StageInfo[ StageNum ].Texture = ( void * )( DWORD_PTR )Tex->UserGraphHandle ;
						else if( Tex->UseGraphHandle         ) StageInfo[ StageNum ].Texture = ( void * )( DWORD_PTR )Tex->GraphHandle ;
						else if( TexBase->UseUserGraphHandle ) StageInfo[ StageNum ].Texture = ( void * )( DWORD_PTR )TexBase->UserGraphHandle ;
						else                                   StageInfo[ StageNum ].Texture = ( void * )( DWORD_PTR )TexBase->GraphHandle ;
						Graphics_D3D9_DeviceState_SetTextureAddressUVW( DX_TEXADDRESS_CLAMP, DX_TEXADDRESS_CLAMP, D_D3DTADDRESS_CLAMP, StageNum ) ;

						Graphics_D3D9_DeviceState_SetSampleFilterMode( StageNum, D_D3DTSS_MINFILTER, DrawModeToFilterTable[ Tex->FilterMode ][ 0 ] ) ;
						Graphics_D3D9_DeviceState_SetSampleFilterMode( StageNum, D_D3DTSS_MAGFILTER, DrawModeToFilterTable[ Tex->FilterMode ][ 1 ] ) ;
						Graphics_D3D9_DeviceState_SetSampleFilterMode( StageNum, D_D3DTSS_MIPFILTER, DrawModeToFilterTable[ Tex->FilterMode ][ 2 ] ) ;
					}
					else
					{
						StageInfo[ StageNum ].Texture = ( void * )( DWORD_PTR )MV1GetDefaultToonTexture( Mesh->Material->SpecularGradTexture ) ;
						Graphics_D3D9_BeginScene() ;
					}

					StageInfo[ StageNum ].TextureCoordIndex = 2 ;

					// �e���|�������W�X�^���g�p�ł��Ȃ��ꍇ�͌Œ�p�����[�^
					if( GD3D9.Device.Caps.ValidTexTempRegFlag == FALSE )
					{
						StageInfo[ StageNum ].ResultTempARG = FALSE ;

						// ���Z�Œ�
						switch( Mesh->Material->SpecularGradBlendType )
						{
						case DX_MATERIAL_BLENDTYPE_TRANSLUCENT :
							StageInfo[ StageNum ].ColorARG1 = D_D3DTA_TEXTURE ;
							StageInfo[ StageNum ].ColorARG2 = D_D3DTA_CURRENT ;
							StageInfo[ StageNum ].ColorOP   = D_D3DTOP_BLENDTEXTUREALPHA ;
							StageInfo[ StageNum ].AlphaARG1 = D_D3DTA_CURRENT ;
							StageInfo[ StageNum ].AlphaARG2 = D_D3DTA_CURRENT ;
							StageInfo[ StageNum ].AlphaOP   = D_D3DTOP_SELECTARG1 ;
							break ;

						case DX_MATERIAL_BLENDTYPE_ADDITIVE :
							StageInfo[ StageNum ].ColorARG1 = D_D3DTA_TEXTURE ;
							StageInfo[ StageNum ].ColorARG2 = D_D3DTA_CURRENT ;
							StageInfo[ StageNum ].ColorOP   = D_D3DTOP_ADD ;
							StageInfo[ StageNum ].AlphaARG1 = D_D3DTA_CURRENT ;
							StageInfo[ StageNum ].AlphaARG2 = D_D3DTA_CURRENT ;
							StageInfo[ StageNum ].AlphaOP   = D_D3DTOP_SELECTARG1 ;
							break ;
						}
					}
					else
					{
						switch( Mesh->Material->SpecularGradBlendType )
						{
						case DX_MATERIAL_BLENDTYPE_TRANSLUCENT :
							StageInfo[ StageNum - 1 ].ResultTempARG = TRUE ;

							StageInfo[ StageNum ].ResultTempARG = FALSE ;
							StageInfo[ StageNum ].ColorARG1 = D_D3DTA_TEXTURE ;
							StageInfo[ StageNum ].ColorARG2 = D_D3DTA_SPECULAR ;
							StageInfo[ StageNum ].ColorOP   = D_D3DTOP_MODULATE ;
							StageInfo[ StageNum ].AlphaARG1 = D_D3DTA_CURRENT ;
							StageInfo[ StageNum ].AlphaARG2 = D_D3DTA_CURRENT ;
							StageInfo[ StageNum ].AlphaOP   = D_D3DTOP_SELECTARG1 ;
							StageNum ++ ;

							StageInfo[ StageNum ].Texture = NULL ;
							StageInfo[ StageNum ].TextureCoordIndex = 0 ;
							StageInfo[ StageNum ].ResultTempARG = FALSE ;
							StageInfo[ StageNum ].ColorARG1 = D_D3DTA_CURRENT ;
							StageInfo[ StageNum ].ColorARG2 = D_D3DTA_TEMP ;
							StageInfo[ StageNum ].ColorOP   = D_D3DTOP_BLENDCURRENTALPHA ;
							StageInfo[ StageNum ].AlphaARG1 = D_D3DTA_TEMP ;
							StageInfo[ StageNum ].AlphaARG2 = D_D3DTA_CURRENT ;
							StageInfo[ StageNum ].AlphaOP   = D_D3DTOP_SELECTARG1 ;
							StageNum ++ ;
							break ;

						case DX_MATERIAL_BLENDTYPE_ADDITIVE :
							StageInfo[ StageNum - 1 ].ResultTempARG = TRUE ;

							StageInfo[ StageNum ].ResultTempARG = FALSE ;
							StageInfo[ StageNum ].ColorARG1 = D_D3DTA_TEXTURE ;
							StageInfo[ StageNum ].ColorARG2 = D_D3DTA_SPECULAR ;
							StageInfo[ StageNum ].ColorOP   = D_D3DTOP_MODULATE ;
							StageInfo[ StageNum ].AlphaARG1 = D_D3DTA_CURRENT ;
							StageInfo[ StageNum ].AlphaARG2 = D_D3DTA_CURRENT ;
							StageInfo[ StageNum ].AlphaOP   = D_D3DTOP_SELECTARG1 ;
							StageNum ++ ;

							StageInfo[ StageNum ].Texture = NULL ;
							StageInfo[ StageNum ].TextureCoordIndex = 0 ;
							StageInfo[ StageNum ].ResultTempARG = FALSE ;
							StageInfo[ StageNum ].ColorARG1 = D_D3DTA_TEMP ;
							StageInfo[ StageNum ].ColorARG2 = D_D3DTA_CURRENT ;
							StageInfo[ StageNum ].ColorOP   = D_D3DTOP_ADD ;
							StageInfo[ StageNum ].AlphaARG1 = D_D3DTA_TEMP ;
							StageInfo[ StageNum ].AlphaARG2 = D_D3DTA_CURRENT ;
							StageInfo[ StageNum ].AlphaOP   = D_D3DTOP_SELECTARG1 ;
							break ;
						}
					}

					StageNum ++ ;
					break ;

				case DX_MATERIAL_TYPE_TOON_2 :
					StageInfo[ StageNum ].Texture = NULL ;
					StageInfo[ StageNum ].TextureCoordIndex = -1 ;
					StageInfo[ StageNum ].ResultTempARG = FALSE ;

					// ���Z�Œ�
					StageInfo[ StageNum ].ColorARG1 = D_D3DTA_SPECULAR ;
					StageInfo[ StageNum ].ColorARG2 = D_D3DTA_CURRENT ;
					StageInfo[ StageNum ].ColorOP   = D_D3DTOP_ADD ;
					StageInfo[ StageNum ].AlphaARG1 = D_D3DTA_CURRENT ;
					StageInfo[ StageNum ].AlphaARG2 = D_D3DTA_CURRENT ;
					StageInfo[ StageNum ].AlphaOP   = D_D3DTOP_SELECTARG1 ;

					StageNum ++ ;
					break ;
				}
			}
			else
			// �g�D�[���V�F�[�f�B���O�ł͂Ȃ��ꍇ�Ń}���`�e�N�X�`���̏ꍇ
			if( Mesh->Material->DiffuseLayerNum >= 2 )
			{
				Tex = &Model->Texture[ Mesh->Material->DiffuseLayer[ 1 ].Texture ] ;
				TexBase = Tex->BaseData ;
					 if( Tex->UseUserGraphHandle     ) StageInfo[ StageNum ].Texture = ( void * )( DWORD_PTR )Tex->UserGraphHandle ;
				else if( Tex->UseGraphHandle         ) StageInfo[ StageNum ].Texture = ( void * )( DWORD_PTR )Tex->GraphHandle ;
				else if( TexBase->UseUserGraphHandle ) StageInfo[ StageNum ].Texture = ( void * )( DWORD_PTR )TexBase->UserGraphHandle ;
				else                                   StageInfo[ StageNum ].Texture = ( void * )( DWORD_PTR )TexBase->GraphHandle ;
				Graphics_D3D9_DeviceState_SetTextureAddressUVW( Tex->AddressModeU, Tex->AddressModeV, D_D3DTADDRESS_CLAMP, StageNum ) ;

				Graphics_D3D9_DeviceState_SetSampleFilterMode( StageNum, D_D3DTSS_MINFILTER, DrawModeToFilterTable[ Tex->FilterMode ][ 0 ] ) ;
				Graphics_D3D9_DeviceState_SetSampleFilterMode( StageNum, D_D3DTSS_MAGFILTER, DrawModeToFilterTable[ Tex->FilterMode ][ 1 ] ) ;
				Graphics_D3D9_DeviceState_SetSampleFilterMode( StageNum, D_D3DTSS_MIPFILTER, DrawModeToFilterTable[ Tex->FilterMode ][ 2 ] ) ;

				StageInfo[ StageNum ].TextureCoordIndex = 1 ;
				StageInfo[ StageNum ].ResultTempARG = FALSE ;
				StageInfo[ StageNum ].ColorARG1 = D_D3DTA_TEXTURE ;
				StageInfo[ StageNum ].ColorARG2 = D_D3DTA_CURRENT ;

				switch( Mesh->Material->DiffuseLayer[ 1 ].BlendType )
				{
				default :
				case MV1_LAYERBLEND_TYPE_TRANSLUCENT :
					StageInfo[ StageNum ].ColorOP   = D_D3DTOP_BLENDTEXTUREALPHA ;
					break ;

				case MV1_LAYERBLEND_TYPE_ADDITIVE :
					StageInfo[ StageNum ].ColorOP   = D_D3DTOP_ADD  ;
					break ;

				case MV1_LAYERBLEND_TYPE_MODULATE :
					StageInfo[ StageNum ].ColorOP   = D_D3DTOP_MODULATE ;
					break ;

				case MV1_LAYERBLEND_TYPE_MODULATE2 :
					StageInfo[ StageNum ].ColorOP   = D_D3DTOP_MODULATE2X ;
					break ;
				}

				StageInfo[ StageNum ].AlphaARG1 = D_D3DTA_CURRENT ;
				StageInfo[ StageNum ].AlphaARG2 = D_D3DTA_CURRENT ;
				StageInfo[ StageNum ].AlphaOP   = D_D3DTOP_SELECTARG1 ;
				StageNum ++ ;
			}

			StageInfo[ StageNum ].ResultTempARG = FALSE ;
			switch( BlendMode )
			{
			case DX_BLENDMODE_MULA :
				if( BlendInfo->FactorColor != 0xffffffff )
				{
					StageInfo[ StageNum ].Texture   = NULL ;
					StageInfo[ StageNum ].TextureCoordIndex = 0 ;
					StageInfo[ StageNum ].ColorARG1 = D_D3DTA_CURRENT ;
					StageInfo[ StageNum ].ColorARG2 = D_D3DTA_TFACTOR ;
					StageInfo[ StageNum ].ColorOP   = D_D3DTOP_BLENDFACTORALPHA ;
					StageInfo[ StageNum ].AlphaARG1 = D_D3DTA_CURRENT ;
					StageInfo[ StageNum ].AlphaARG2 = D_D3DTA_DIFFUSE ;
					StageInfo[ StageNum ].AlphaOP   = D_D3DTOP_SELECTARG1 ;
					StageNum ++ ;
				}
				break ;

			default :
				if( BlendInfo->FactorColor != 0xffffffff )
				{
					StageInfo[ StageNum ].Texture   = NULL ;
					StageInfo[ StageNum ].TextureCoordIndex = 0 ;
					StageInfo[ StageNum ].ColorARG1 = D_D3DTA_CURRENT ;
					StageInfo[ StageNum ].ColorARG2 = D_D3DTA_DIFFUSE ;
					StageInfo[ StageNum ].ColorOP   = D_D3DTOP_SELECTARG1 ;
					StageInfo[ StageNum ].AlphaARG1 = D_D3DTA_CURRENT ;
					StageInfo[ StageNum ].AlphaARG2 = D_D3DTA_TFACTOR ;
					StageInfo[ StageNum ].AlphaOP   = D_D3DTOP_MODULATE ;
					StageNum ++ ;
				}
				break ;
			}

/*
			if( BlendMode == DX_BLENDMODE_INVSRC )
			{
				StageInfo[ StageNum ].Texture   = NULL ;
				StageInfo[ StageNum ].TextureCoordIndex = 0 ;
				StageInfo[ StageNum ].ColorARG1 = D_D3DTA_CURRENT | D_D3DTA_COMPLEMENT ;
				StageInfo[ StageNum ].ColorARG2 = D_D3DTA_DIFFUSE ;
				StageInfo[ StageNum ].ColorOP   = D_D3DTOP_SELECTARG1 ;
				StageInfo[ StageNum ].AlphaARG1 = D_D3DTA_CURRENT ;
				StageInfo[ StageNum ].AlphaARG2 = D_D3DTA_DIFFUSE ;
				StageInfo[ StageNum ].AlphaOP   = D_D3DTOP_SELECTARG1 ;
				StageNum ++ ;
			}
*/

			StageInfo[ StageNum ].Texture   = NULL ;
			StageInfo[ StageNum ].TextureCoordIndex = 0 ;
			StageInfo[ StageNum ].ResultTempARG = FALSE ;
			StageInfo[ StageNum ].ColorARG1 = D_D3DTA_TEXTURE ;
			StageInfo[ StageNum ].ColorARG2 = D_D3DTA_DIFFUSE ;
			StageInfo[ StageNum ].ColorOP   = D_D3DTOP_DISABLE ;
			StageInfo[ StageNum ].AlphaARG1 = D_D3DTA_TEXTURE ;
			StageInfo[ StageNum ].AlphaARG2 = D_D3DTA_DIFFUSE ;
			StageInfo[ StageNum ].AlphaOP   = D_D3DTOP_DISABLE ;
			StageNum ++ ;

			BlendInfo->UseTextureStageNum = StageNum ;

			// �ݒ�𔽉f
			Graphics_D3D9_DeviceState_SetUserBlendInfo( BlendInfo, FALSE ) ;
		}
	}
}

// �g�D�[���ׂ̈̃}�e���A�����Z�b�g�A�b�v����
__inline void MV1_D3D9_SetupToonOutLineMeshDrawMaterialCommon(
	GRAPHICS_HARDDATA_DIRECT3D9_BLENDINFO *BlendInfo, MV1_MESH *Mesh, float OutLineWidth )
{
	MV1_MESH_BASE *MBMesh ;
	GRAPHICS_HARDDATA_DIRECT3D9_BLENDINFO *pBlendInfo ; 
	MATERIALPARAM Material ;
	MV1_MATERIAL * RST MMaterial ;
	MV1_MATERIAL_BASE * RST MBMaterial ;
	MV1_FRAME * RST Frame ;

	Frame = Mesh->Container ;
	MBMesh = Mesh->BaseData ;
	MMaterial = Mesh->Material ;
	MBMaterial = MMaterial->BaseData ;

	// �}�e���A���̃Z�b�g
	Material.Diffuse = MMaterial->OutLineColor ;
	Material.Diffuse.a = 1.0f ;
	Material.Specular.r = 0.0f ;
	Material.Specular.g = 0.0f ;
	Material.Specular.b = 0.0f ;
	Material.Specular.a = 0.0f ;
	Material.Emissive = Material.Specular ;
	Material.Ambient  = Material.Specular ;
	Material.Power = 0.0f ;
	GSYS.Light.ChangeMaterial = 1 ;
	GD3D9.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;
	Graphics_D3D9_DeviceState_SetMaterial( ( D_D3DMATERIAL9 * )&Material ) ;

	// �֊s���̑������Z�b�g����
	{
		float Data[ 4 ] ;

		Data[ 0 ] = OutLineWidth ;
		Data[ 1 ] = OutLineWidth ;
		Data[ 2 ] = OutLineWidth ;
		Data[ 3 ] = OutLineWidth ;
		Graphics_D3D9_ShaderConstant_InfoSet_SetParam(
			&GD3D9.Device.Shader.ShaderConstantInfo,
			DX_SHADERCONSTANTTYPE_VS_FLOAT,
			DX_SHADERCONSTANTSET_MV1,
			DX_VS_CONSTF_TOON_OUTLINE_SIZE,
			Data,
			1,
			FALSE
		) ;
	}

	// �J�����O�ݒ�
	Graphics_D3D9_DeviceState_SetCullMode( 2 ) ; 

	// �u�����h�ݒ�̃Z�b�g�A�b�v
	BlendInfo->AlphaTestEnable = MMaterial->UseAlphaTest ;
	BlendInfo->AlphaFunc = MMaterial->AlphaFunc ;
	BlendInfo->AlphaRef = MMaterial->AlphaRef ;
	BlendInfo->FactorColor = 0x00ffffff | ( ( ( DWORD )_FTOL( Mesh->SetupDrawMaterial.OpacityRate * ( Mesh->DrawBlendParam != -1 ? Mesh->DrawBlendParam : MMaterial->DrawBlendParam ) ) ) << 24 ) ;
	pBlendInfo = &GD3D9.Device.State.BlendInfo ;
	switch( Mesh->DrawBlendMode != -1 ? Mesh->DrawBlendMode : MMaterial->DrawBlendMode )
	{
	case DX_BLENDMODE_NOBLEND :
		BlendInfo->AlphaBlendEnable = FALSE ;
		BlendInfo->SrcBlend         = pBlendInfo->SrcBlend ;
		BlendInfo->DestBlend        = pBlendInfo->DestBlend ;
		BlendInfo->BlendOp          = pBlendInfo->BlendOp ;
		break ;

	case DX_BLENDMODE_ALPHA :
		BlendInfo->AlphaBlendEnable = TRUE ;
		BlendInfo->SrcBlend         = D_D3DBLEND_SRCALPHA ;
		BlendInfo->DestBlend        = D_D3DBLEND_INVSRCALPHA ;
		BlendInfo->BlendOp          = D_D3DBLENDOP_ADD ;
		break ;

	case DX_BLENDMODE_ADD :
		BlendInfo->AlphaBlendEnable = TRUE ;
		BlendInfo->SrcBlend         = D_D3DBLEND_SRCALPHA ;
		BlendInfo->DestBlend        = D_D3DBLEND_ONE ;
		BlendInfo->BlendOp          = D_D3DBLENDOP_ADD ;
		break ;

	case DX_BLENDMODE_INVSRC :
		BlendInfo->AlphaBlendEnable = TRUE ;
		BlendInfo->SrcBlend         = D_D3DBLEND_SRCALPHA ;
		BlendInfo->DestBlend        = D_D3DBLEND_INVSRCALPHA ;
		BlendInfo->BlendOp          = D_D3DBLENDOP_ADD ;
		break ;

	case DX_BLENDMODE_MUL :
	case DX_BLENDMODE_MULA :
		BlendInfo->AlphaBlendEnable = TRUE ;
		BlendInfo->SrcBlend         = D_D3DBLEND_ZERO ;
		BlendInfo->DestBlend        = D_D3DBLEND_SRCCOLOR ;
		BlendInfo->BlendOp          = D_D3DBLENDOP_ADD ;
		break ;
	}
	BlendInfo->UseTextureStageNum = 1 ;

	// �X�y�L�����𖳌��ɂ���
	Graphics_D3D9_DeviceState_SetSpecularEnable( FALSE ) ;

	// ���C�e�B���O�𖳌��ɂ���
	Graphics_D3D9_DeviceState_SetLighting( FALSE ) ;
}

// �w��̃g���C�A���O�����X�g�̃e���|���������Z�b�g�A�b�v����
static bool MV1_D3D9_SetupTListTempSimpleVertex( MV1_TRIANGLE_LIST_BASE *MBTList )
{
	int							i ;
	int							j ;
	int							MeshVertUnitSize ;
	DWORD						DestUnitSize ;
	int							VertexNum ;
	int							UVNum ;
	DWORD						*MeshVertIndex ;
	MV1_VERTEX_SIMPLE_D3D9		*SMPVert ;
	MV1_MESH_VERTEX				*MeshVert ;
	MV1_MESH_VERTEX				*MVert ;
	MV1_TLIST_NORMAL_POS		*Norm ;
	MV1_TRIANGLE_LIST_BASE_PF	*MBTListPF ;

	MBTListPF = ( MV1_TRIANGLE_LIST_BASE_PF * )MBTList->PFBuffer ;

	// ���Ɋm�ۂ���Ă����牽�������ɏI��
	if( MBTListPF->D3D9.TempSimpleVertex )
		return true ;

	// �������̊m��
	UVNum = MBTList->Container->UVSetUnitNum ;
	MeshVertUnitSize = MBTList->Container->VertUnitSize ;
	DestUnitSize = sizeof( MV1_VERTEX_SIMPLE_D3D9 ) + ( UVNum - 1 ) * sizeof( float ) * 4 ;
	MBTList->TempUnitSize = ( unsigned short )DestUnitSize ;
	MBTListPF->D3D9.TempSimpleVertex = ( MV1_VERTEX_SIMPLE_D3D9 * )DXALLOC( DestUnitSize * MBTList->VertexNum ) ;
	if( MBTListPF->D3D9.TempSimpleVertex == NULL )
		return false ;

	// ���_�̐����Z�b�g
	VertexNum = MBTList->VertexNum ;

	// ���ʃf�[�^���R�s�[����
	SMPVert  = MBTListPF->D3D9.TempSimpleVertex ;
	MeshVert = MBTList->Container->Vertex ;
	MeshVertIndex = MBTList->MeshVertexIndex ;
	for( i = 0 ; i < VertexNum ; i ++,
		SMPVert = ( MV1_VERTEX_SIMPLE_D3D9 * )( ( BYTE * )SMPVert + DestUnitSize ),
		MeshVertIndex ++ )
	{
		MVert = ( MV1_MESH_VERTEX * )( ( BYTE * )MeshVert + MeshVertUnitSize * *MeshVertIndex ) ;
		SMPVert->DiffuseColor  = MVert->DiffuseColor ;
		SMPVert->SpecularColor = MVert->SpecularColor ;
		for( j = 0 ; j < UVNum ; j ++ )
		{
			SMPVert->UVs[ j ][ 0 ] = MVert->UVs[ j ][ 0 ] ;
			SMPVert->UVs[ j ][ 1 ] = MVert->UVs[ j ][ 1 ] ;
			SMPVert->UVs[ j ][ 2 ] = 1.0f ;
			SMPVert->UVs[ j ][ 3 ] = 1.0f ;
		}
	}

	// ���̃��b�V���̏ꍇ�̂ݍ��W�Ɩ@���������Ă��܂�
	if( MBTList->VertexType == MV1_VERTEX_TYPE_NORMAL )
	{
		SMPVert = MBTListPF->D3D9.TempSimpleVertex ;
		Norm    = ( MV1_TLIST_NORMAL_POS * )ADDR16( MBTList->NormalPosition ) ;
		for( i = 0 ; i < VertexNum ; i ++, Norm ++, SMPVert = ( MV1_VERTEX_SIMPLE_D3D9 * )( ( BYTE * )SMPVert + DestUnitSize ) )
		{
			SMPVert->Position = *( ( VECTOR * )&Norm->Position ) ;
			SMPVert->Normal   = *( ( VECTOR * )&Norm->Normal ) ;
		}
	}

	// ����I��
	return true ;
}

// �F��HSV�ɕϊ�������A�P�x���ő�ɂ������� RGB �̒l���擾����
__inline void RGBtoVMaxRGB( float R, float G, float B, float &RD, float &GD, float &BD )
{
	float h, s ;

	if( R + G + B < 0.00001f )
	{
		RD = 1.0f ;
		GD = 1.0f ;
		BD = 1.0f ;
		return ;
	}

	if( R > G )
	{
		if( R > B )
		{
			if( G > B )
			{
				// R > G > B
				if( R - B < 0.00001f )
				{
					RD = 1.0f ;
					GD = 1.0f ;
					BD = 1.0f ;
				}
				else
				{
					h = ( G - B ) / ( R - B ) ;
					s = ( R - B ) / R ;

					RD = 1.0f ;
					GD = 1.0f - ( 1.0f - h ) * s ;
					BD = 1.0f - s ;
				}
			}
			else
			{
				// R > B > G
				if( R - G < 0.00001f )
				{
					RD = 1.0f ;
					GD = 1.0f ;
					BD = 1.0f ;
				}
				else
				{
					h = ( B - G ) / ( R - G ) ;
					s = ( R - G ) / R ;

					RD = 1.0f ;
					GD = 1.0f -      s ;
					BD = 1.0f - ( 1.0f - h ) * s ;
				}
			}
		}
		else
		{
			// B > R > G
			if( B - G < 0.00001f )
			{
				RD = 1.0f ;
				GD = 1.0f ;
				BD = 1.0f ;
			}
			else
			{
				h = ( R - G ) / ( B - G ) ;
				s = ( B - G ) / B ;

				RD = 1.0f - ( 1.0f - h ) * s ;
				GD = 1.0f -                s ;
				BD = 1.0f ;
			}
		}
	}
	else
	{
		if( G > B )
		{
			if( R > B )
			{
				// G > R > B
				if( G - B < 0.00001f )
				{
					RD = 1.0f ;
					GD = 1.0f ;
					BD = 1.0f ;
				}
				else
				{
					h = ( R - B ) / ( G - B ) ;
					s = ( G - B ) / G ;

					RD = 1.0f - ( 1.0f - h ) * s ;
					GD = 1.0f ;
					BD = 1.0f -                s ;
				}
			}
			else
			{
				// G > B > R
				if( G - R < 0.00001f )
				{
					RD = 1.0f ;
					GD = 1.0f ;
					BD = 1.0f ;
				}
				else
				{
					h = ( B - R ) / ( G - R ) ;
					s = ( G - R ) / G ;

					RD = 1.0f -                s ;
					GD = 1.0f ;
					BD = 1.0f - ( 1.0f - h ) * s ;
				}
			}
		}
		else
		{
			// B > G > R
			if( B - R < 0.00001f )
			{
				RD = 1.0f ;
				GD = 1.0f ;
				BD = 1.0f ;
			}
			else
			{
				h = ( G - R ) / ( B - R ) ;
				s = ( B - R ) / B ;

				RD = 1.0f -                s ;
				GD = 1.0f - ( 1.0f - h ) * s ;
				BD = 1.0f ;
			}
		}
	}
}

// �g�D�[���g���C�A���O�����X�g�̃��C�e�B���O���s��
static void MV1_D3D9_TriangleListToonLighting( MV1_TRIANGLE_LIST_BASE *MBTList, bool Normalize, MATRIX *LocalWorldMatrix )
{
	DWORD						VertexNum ;
	MV1_VERTEX_SIMPLE_D3D9		*SMPVert ;
	MV1_MESH_VERTEX				*MeshVert ;
	MV1_MESH_VERTEX				*MVert ;
	MV1_TRIANGLE_LIST_BASE_PF	*MBTListPF ;
	DWORD						i ;
	DWORD						j ;
	DWORD						UnitSize ;
	DWORD						IdentMat ;
	DWORD						MeshVertUnitSize ;
	VECTOR						Position ;
	VECTOR						Normal ;
	D_D3DLIGHT9					*Light ;
	float						PLLength ;
	float						DirRate ;
	float						LengthRate ;
	float						SpotRate ;
	float						SpeRate ;
	float						DiffuseV ;
	float						SpecularV ;
	COLOR_F						Diffuse ;
	COLOR_F						Specular ;
	COLOR_F						LightDiffuse ;
	COLOR_F						TempColor ;
	COLOR_F						Ambient ;
	VECTOR						LightDir ;
	VECTOR						PLVec ;
	VECTOR						HalfVec ;
	VECTOR						PCVec ;
	VECTOR						PLDir ;
	VECTOR						ViewNormal ;
	bool						UseVertDif ;
	bool						UseVertSpe ;
	FLOAT4						*LParam ;
	FLOAT4						*ConstF ;
	DWORD						*MeshVertIndex ;
	float						LightCosPhi[ 8 ] ;
	float						LightCosTheta[ 8 ] ;
	float						Sin ;
	float						tmpf ;
	bool						ToonType2 ;
	int							Temp ;
	bool						SphereMap ;
	float						rd ;
	float						gd ;
	float						bd ;
	float						DirRateTotal ;
	COLOR_U8					MatDifColor ;
	COLOR_U8					MatSpeColor ;

	MBTListPF = ( MV1_TRIANGLE_LIST_BASE_PF * )MBTList->PFBuffer ;

	UseVertDif = MBTList->Container->UseVertexDiffuseColor != 0 ;
	UseVertSpe = MBTList->Container->UseVertexSpecularColor != 0 ;

	ConstF = GD3D9.Device.Shader.ShaderConstantInfo.FixInfo[ DX_SHADERCONSTANTTYPE_VS_FLOAT ].Float4 ;
	Temp = _FTOL( ConstF[ DX_VS_CONSTF_MAT_DIFFUSE  + DX_VS_CONSTF_MATERIAL_START ].x * 255.0f ) ;
	MatDifColor.r = ( BYTE )( Temp > 255 ? 255 : ( Temp < 0 ? 0 : Temp ) ) ;
	Temp = _FTOL( ConstF[ DX_VS_CONSTF_MAT_DIFFUSE  + DX_VS_CONSTF_MATERIAL_START ].y * 255.0f ) ;
	MatDifColor.g = ( BYTE )( Temp > 255 ? 255 : ( Temp < 0 ? 0 : Temp ) ) ;
	Temp = _FTOL( ConstF[ DX_VS_CONSTF_MAT_DIFFUSE  + DX_VS_CONSTF_MATERIAL_START ].z * 255.0f ) ;
	MatDifColor.b = ( BYTE )( Temp > 255 ? 255 : ( Temp < 0 ? 0 : Temp ) ) ;
	Temp = _FTOL( ConstF[ DX_VS_CONSTF_MAT_DIFFUSE  + DX_VS_CONSTF_MATERIAL_START ].w * 255.0f ) ;
	MatDifColor.a = ( BYTE )( Temp > 255 ? 255 : ( Temp < 0 ? 0 : Temp ) ) ;

	Temp = _FTOL( ConstF[ DX_VS_CONSTF_MAT_SPECULAR  + DX_VS_CONSTF_MATERIAL_START ].x * 255.0f ) ;
	MatSpeColor.r = ( BYTE )( Temp > 255 ? 255 : ( Temp < 0 ? 0 : Temp ) ) ;
	Temp = _FTOL( ConstF[ DX_VS_CONSTF_MAT_SPECULAR  + DX_VS_CONSTF_MATERIAL_START ].y * 255.0f ) ;
	MatSpeColor.g = ( BYTE )( Temp > 255 ? 255 : ( Temp < 0 ? 0 : Temp ) ) ;
	Temp = _FTOL( ConstF[ DX_VS_CONSTF_MAT_SPECULAR  + DX_VS_CONSTF_MATERIAL_START ].z * 255.0f ) ;
	MatSpeColor.b = ( BYTE )( Temp > 255 ? 255 : ( Temp < 0 ? 0 : Temp ) ) ;
	Temp = _FTOL( ConstF[ DX_VS_CONSTF_MAT_SPECULAR  + DX_VS_CONSTF_MATERIAL_START ].w * 255.0f ) ;
	MatSpeColor.a = ( BYTE )( Temp > 255 ? 255 : ( Temp < 0 ? 0 : Temp ) ) ;

	Light = GD3D9.Device.State.LightParam ;
	for( i = 0 ; i < ( DWORD )GSYS.Light.EnableNum ; i ++, Light ++ )
	{
		_SINCOS( Light->Phi   / 2.0f, &Sin, &LightCosPhi[ i ]   ) ;
		_SINCOS( Light->Theta / 2.0f, &Sin, &LightCosTheta[ i ] ) ;
	}

	ToonType2        = MBTList->Container->Material->Type == DX_MATERIAL_TYPE_TOON_2 ;
	SphereMap        = ToonType2 && MBTList->Container->Material->SphereMapTexture >= 0 ;

	VertexNum        = ( DWORD )MBTList->VertexNum ;
	SMPVert          = MBTListPF->D3D9.TempSimpleVertex ;
	UnitSize         = MBTList->TempUnitSize ;
	IdentMat         = ( DWORD )MV1Man.WorldMatrixIsIdentity ;
	MeshVert         = MBTList->Container->Vertex ;
	MeshVertIndex    = MBTList->MeshVertexIndex ;
	MeshVertUnitSize = ( DWORD )MBTList->Container->VertUnitSize ;
	for( i = 0 ; i < VertexNum ; i ++, SMPVert = ( MV1_VERTEX_SIMPLE_D3D9 * )( ( DWORD_PTR )SMPVert + UnitSize ), MeshVertIndex ++ )
	{
		MVert = ( MV1_MESH_VERTEX * )( ( BYTE * )MeshVert + MeshVertUnitSize * *MeshVertIndex ) ;

		if( LocalWorldMatrix == NULL )
		{
			Position = SMPVert->Position ;
			Normal  = SMPVert->Normal ;
		}
		else
		{
			Position = VTransform( SMPVert->Position, *LocalWorldMatrix ) ;
			Normal   = VNorm( VTransformSR( SMPVert->Normal, *LocalWorldMatrix ) ) ;
		}

		if( Normalize )
		{
			float RevSize ;

			RevSize = 1.0f / _SQRT( Normal.x * Normal.x + Normal.y * Normal.y + Normal.z * Normal.z ) ; 
			Normal.x *= RevSize ;
			Normal.y *= RevSize ;
			Normal.z *= RevSize ;
		}

		Diffuse.r = 0.0f ;
		Diffuse.g = 0.0f ;
		Diffuse.b = 0.0f ;
		Diffuse.a = 0.0f ;
		Specular.r = 0.0f ;
		Specular.g = 0.0f ;
		Specular.b = 0.0f ;
		Specular.a = 0.0f ;
		Ambient.r = 0.0f ;
		Ambient.g = 0.0f ;
		Ambient.b = 0.0f ;
		Ambient.a = 0.0f ;
		LightDiffuse.r = 0.0f ;
		LightDiffuse.g = 0.0f ;
		LightDiffuse.b = 0.0f ;
		LightDiffuse.a = 0.0f ;
		DirRateTotal = 0.0f ;

		Light = GD3D9.Device.State.LightParam ;
		for( j = 0 ; j < ( DWORD )GSYS.Light.EnableNum ; j ++, Light ++ )
		{
			LParam = &ConstF[ DX_VS_CONSTF_LIGHT_START + DX_VS_CONSTF_LIGHT_UNITSIZE * j ] ;

			// �f�B�t���[�Y�J���[�̌v�Z
			{
				if( Light->Type == DX_LIGHTTYPE_DIRECTIONAL )
				{
					// ���C�g�̊p�x���Z�b�g
					LightDir.x = Light->Direction.x ;
					LightDir.y = Light->Direction.y ;
					LightDir.z = Light->Direction.z ;

					// �p�x�������v�Z
					DirRate = ( -LightDir.x * Normal.x ) + ( -LightDir.y * Normal.y ) + ( -LightDir.z * Normal.z ) ;
					if( DirRate < 0.0f ) DirRate = 0.0f ;

					if( ToonType2 )
					{
						DirRateTotal += DirRate ;
						Diffuse.r += LParam[ DX_VS_CONSTF_LGT_DIFFUSE ].x ;
						Diffuse.g += LParam[ DX_VS_CONSTF_LGT_DIFFUSE ].y ;
						Diffuse.b += LParam[ DX_VS_CONSTF_LGT_DIFFUSE ].z ;
						Ambient.r += LParam[ DX_VS_CONSTF_LGT_AMBIENT ].x ;
						Ambient.g += LParam[ DX_VS_CONSTF_LGT_AMBIENT ].y ;
						Ambient.b += LParam[ DX_VS_CONSTF_LGT_AMBIENT ].z ;
					}
					else
					{
						// �f�B�t�[�Y���C�g�~�ϒl += �f�B�t�[�Y�p�x�����v�Z���� * ���C�g�̃f�B�t�[�Y�F + ���C�g�̃A���r�G���g�J���[�ƃ}�e���A���̃A���r�G���g�J���[����Z��������
						TempColor.r = DirRate * LParam[ DX_VS_CONSTF_LGT_DIFFUSE ].x ;
						TempColor.g = DirRate * LParam[ DX_VS_CONSTF_LGT_DIFFUSE ].y ;
						TempColor.b = DirRate * LParam[ DX_VS_CONSTF_LGT_DIFFUSE ].z ;
						LightDiffuse.r += TempColor.r ;
						LightDiffuse.g += TempColor.g ;
						LightDiffuse.b += TempColor.b ;
						Diffuse.r += TempColor.r + LParam[ DX_VS_CONSTF_LGT_AMBIENT ].x ;
						Diffuse.g += TempColor.g + LParam[ DX_VS_CONSTF_LGT_AMBIENT ].y ;
						Diffuse.b += TempColor.b + LParam[ DX_VS_CONSTF_LGT_AMBIENT ].z ;
					}
				}
				else
				{
					PLVec = VSub( VGet( Light->Position.x, Light->Position.y, Light->Position.z ), Position ) ;
					PLLength = VSize( PLVec ) ;
					PLDir.x = PLVec.x / PLLength ;
					PLDir.y = PLVec.y / PLLength ;
					PLDir.z = PLVec.z / PLLength ;

					// ���C�g�̕������Z�b�g
					LightDir.x = -PLDir.x ;
					LightDir.y = -PLDir.y ;
					LightDir.z = -PLDir.z ;

					// �����������v�Z
					LengthRate = 1.0f / ( LParam[ DX_VS_CONSTF_LGT_RANGE_FALLOFF_AT0_AT1 ].z + LParam[ DX_VS_CONSTF_LGT_RANGE_FALLOFF_AT0_AT1 ].w * PLLength + LParam[ DX_VS_CONSTF_LGT_AT2_SPOTP0_SPOTP1 ].x * PLLength * PLLength ) ;

					// �X�|�b�g���C�g�̏ꍇ�X�|�b�g���C�g�������v�Z���s��
					if( Light->Type == DX_LIGHTTYPE_SPOT )
					{
						float CosA ;

						// �X�|�b�g���C�g�̏ꍇ
						CosA = -Light->Direction.x * PLDir.x + -Light->Direction.y * PLDir.y + -Light->Direction.z * PLDir.z ;
						if( CosA < LightCosPhi[ j ] )
						{
							SpotRate = 0.0f ;
						}
						else
						if( CosA > LightCosTheta[ j ] )
						{
							SpotRate = 1.0f ;
						}
						else
						{
							SpotRate = ( CosA - LightCosPhi[ j ] ) / ( LightCosTheta[ j ] - LightCosPhi[ j ] ) ;
						}
					}
					else
					{
						// �|�C���g���C�g�̏ꍇ

						SpotRate = 1.0f ;
					}

					// �p�x�������v�Z
					DirRate = ( -LightDir.x * Normal.x ) + ( -LightDir.y * Normal.y ) + ( -LightDir.z * Normal.z ) ;
					if( DirRate < 0.0f ) DirRate = 0.0f ;

					if( ToonType2 )
					{
						DirRateTotal += DirRate * LengthRate * SpotRate ;
						Diffuse.r += LParam[ DX_VS_CONSTF_LGT_DIFFUSE ].x ;
						Diffuse.g += LParam[ DX_VS_CONSTF_LGT_DIFFUSE ].y ;
						Diffuse.b += LParam[ DX_VS_CONSTF_LGT_DIFFUSE ].z ;
						Ambient.r += LParam[ DX_VS_CONSTF_LGT_AMBIENT ].x ;
						Ambient.g += LParam[ DX_VS_CONSTF_LGT_AMBIENT ].y ;
						Ambient.b += LParam[ DX_VS_CONSTF_LGT_AMBIENT ].z ;
					}
					else
					{
						// �f�B�t�[�Y���C�g�~�ϒl += �����E�X�|�b�g���C�g�p�x�����l * ( �f�B�t�[�Y�p�x�����v�Z���� * ���C�g�̃f�B�t�[�Y�F + ���C�g�̃A���r�G���g�J���[�ƃ}�e���A���̃A���r�G���g�J���[����Z�������� )
						tmpf = DirRate * LengthRate * SpotRate ;
						LightDiffuse.r += tmpf * LParam[ DX_VS_CONSTF_LGT_DIFFUSE ].x ;
						LightDiffuse.g += tmpf * LParam[ DX_VS_CONSTF_LGT_DIFFUSE ].y ;
						LightDiffuse.b += tmpf * LParam[ DX_VS_CONSTF_LGT_DIFFUSE ].z ;
						Diffuse.r +=
							( DirRate *
							  LParam[ DX_VS_CONSTF_LGT_DIFFUSE ].x +
							  LParam[ DX_VS_CONSTF_LGT_AMBIENT ].y ) * LengthRate * SpotRate ;
						Diffuse.g +=
							( DirRate *
							  LParam[ DX_VS_CONSTF_LGT_DIFFUSE ].x +
							  LParam[ DX_VS_CONSTF_LGT_AMBIENT ].y ) * LengthRate * SpotRate ;
						Diffuse.b +=
							( DirRate *
							  LParam[ DX_VS_CONSTF_LGT_DIFFUSE ].z +
							  LParam[ DX_VS_CONSTF_LGT_AMBIENT ].z ) * LengthRate * SpotRate ;
					}
				}
			}

			// �X�y�L�����J���[�̌v�Z
			{
				VECTOR ViewPos, ViewNorm, ViewLightDir ;
				float LDot ;

				// �r���[��Ԃł̒��_���W�A�@���A���C�g�̌������v�Z
				ViewPos = VTransform( Position, GSYS.DrawSetting.ViewMatrixF ) ;
				ViewNorm = VTransformSR( Normal, GSYS.DrawSetting.ViewMatrixF ) ;
				ViewLightDir = VTransformSR( LightDir, GSYS.DrawSetting.ViewMatrixF ) ;

				// �n�[�t�x�N�g���̌v�Z
				PCVec = VNorm( VScale( ViewPos, -1.0f ) ) ;
				HalfVec = VNorm( VSub( PCVec, ViewLightDir ) ) ;

				// �X�y�L�����J���[���������v�Z
				LDot = VDot( ViewNorm, HalfVec ) ;
				SpeRate = _POW( LDot, ConstF[ DX_VS_CONSTF_MAT_POWER + DX_VS_CONSTF_MATERIAL_START ].x ) ;
				if( SpeRate < 0.0f ) SpeRate = 0.0f ;

				if( Light->Type == DX_LIGHTTYPE_DIRECTIONAL )
				{
					// r1 += �X�y�L�����p�x�����v�Z���� * ���C�g�̃X�y�L�����F
					Specular.r += 
						SpeRate *
						LParam[ DX_VS_CONSTF_LGT_SPECULAR ].x ;
					Specular.g += 
						SpeRate *
						LParam[ DX_VS_CONSTF_LGT_SPECULAR ].y ;
					Specular.b += 
						SpeRate *
						LParam[ DX_VS_CONSTF_LGT_SPECULAR ].z ;
				}
				else
				{
					// r1 += �X�y�L�����p�x�����v�Z���� * �����E�X�|�b�g���C�g���� * ���C�g�̃X�y�L�����F
					tmpf = LengthRate * SpotRate * SpeRate ;
					Specular.r +=
						tmpf *
						LParam[ DX_VS_CONSTF_LGT_SPECULAR ].x ;
					Specular.g +=
						tmpf *
						LParam[ DX_VS_CONSTF_LGT_SPECULAR ].y ;
					Specular.b +=
						tmpf *
						LParam[ DX_VS_CONSTF_LGT_SPECULAR ].z ;
				}
			}
		}

		if( ToonType2 )
		{
			Ambient.r += ConstF[ DX_VS_CONSTF_AMBIENT_EMISSIVE ].x ;
			Ambient.g += ConstF[ DX_VS_CONSTF_AMBIENT_EMISSIVE ].y ;
			Ambient.b += ConstF[ DX_VS_CONSTF_AMBIENT_EMISSIVE ].z ;

			if( UseVertDif )
			{
				TempColor.r = Diffuse.r * MVert->DiffuseColor.r + Ambient.r * 255.0f ;
				TempColor.g = Diffuse.g * MVert->DiffuseColor.g + Ambient.g * 255.0f ;
				TempColor.b = Diffuse.b * MVert->DiffuseColor.b + Ambient.b * 255.0f ;
				TempColor.a = MVert->DiffuseColor.a ;
			}
			else
			{
				TempColor.r = Diffuse.r * MatDifColor.r + Ambient.r * 255.0f ;
				TempColor.g = Diffuse.g * MatDifColor.g + Ambient.g * 255.0f ;
				TempColor.b = Diffuse.b * MatDifColor.b + Ambient.b * 255.0f ;
				TempColor.a = MatDifColor.a ;
			}

			     if( TempColor.r > 254.9999f ) TempColor.r = 254.9999f ;
			else if( TempColor.r < 0.0f      ) TempColor.r = 0.0f ;
			     if( TempColor.g > 254.9999f ) TempColor.g = 254.9999f ;
			else if( TempColor.g < 0.0f      ) TempColor.g = 0.0f ;
			     if( TempColor.b > 254.9999f ) TempColor.b = 254.9999f ;
			else if( TempColor.b < 0.0f      ) TempColor.b = 0.0f ;

			     if( Specular.r > 254.9999f ) Specular.r = 254.9999f ;
			else if( Specular.r < 0.0f      ) Specular.r = 0.0f ;
			     if( Specular.g > 254.9999f ) Specular.g = 254.9999f ;
			else if( Specular.g < 0.0f      ) Specular.g = 0.0f ;
			     if( Specular.b > 254.9999f ) Specular.b = 254.9999f ;
			else if( Specular.b < 0.0f      ) Specular.b = 0.0f ;

			SMPVert->DiffuseColor.r = ( BYTE )_FTOL( TempColor.r ) ;
			SMPVert->DiffuseColor.g = ( BYTE )_FTOL( TempColor.g ) ;
			SMPVert->DiffuseColor.b = ( BYTE )_FTOL( TempColor.b ) ;
			SMPVert->DiffuseColor.a = UseVertDif ? MVert->DiffuseColor.a : MatDifColor.a ;

			if( UseVertSpe )
			{
				SMPVert->SpecularColor.r = ( BYTE )_FTOL( Specular.r * MVert->SpecularColor.r ) ;
				SMPVert->SpecularColor.g = ( BYTE )_FTOL( Specular.g * MVert->SpecularColor.g ) ;
				SMPVert->SpecularColor.b = ( BYTE )_FTOL( Specular.b * MVert->SpecularColor.b ) ;
				SMPVert->SpecularColor.a = MVert->SpecularColor.a ;
			}
			else
			{
				SMPVert->SpecularColor.r = ( BYTE )_FTOL( Specular.r * MatSpeColor.r ) ;
				SMPVert->SpecularColor.g = ( BYTE )_FTOL( Specular.g * MatSpeColor.g ) ;
				SMPVert->SpecularColor.b = ( BYTE )_FTOL( Specular.b * MatSpeColor.b ) ;
				SMPVert->SpecularColor.a = MatSpeColor.a ;
			}

			SMPVert->UVs[ 1 ][ 0 ] = DirRateTotal ;
			SMPVert->UVs[ 1 ][ 1 ] = DirRateTotal ;

			if( SphereMap )
			{
				ViewNormal = VTransformSR( Normal, GSYS.DrawSetting.ViewMatrixF ) ;
				SMPVert->UVs[ 2 ][ 0 ] = ViewNormal.x *  0.5f + 0.5f ;
				SMPVert->UVs[ 2 ][ 1 ] = ViewNormal.y * -0.5f + 0.5f ;
			}
		}
		else
		{
			Diffuse.r += ConstF[ DX_VS_CONSTF_AMBIENT_EMISSIVE ].x ;
			Diffuse.g += ConstF[ DX_VS_CONSTF_AMBIENT_EMISSIVE ].y ;
			Diffuse.b += ConstF[ DX_VS_CONSTF_AMBIENT_EMISSIVE ].z ;

			RGBtoVMaxRGB( LightDiffuse.r, LightDiffuse.g, LightDiffuse.b, rd, gd, bd ) ; 
			if( UseVertDif )
			{
				SMPVert->DiffuseColor.r = ( BYTE )_FTOL( rd * MVert->DiffuseColor.r ) ;
				SMPVert->DiffuseColor.g = ( BYTE )_FTOL( gd * MVert->DiffuseColor.g ) ;
				SMPVert->DiffuseColor.b = ( BYTE )_FTOL( bd * MVert->DiffuseColor.b ) ;
				SMPVert->DiffuseColor.a = MVert->DiffuseColor.a ;
			}
			else
			{
				SMPVert->DiffuseColor.r = ( BYTE )_FTOL( rd * MatDifColor.r ) ;
				SMPVert->DiffuseColor.g = ( BYTE )_FTOL( gd * MatDifColor.g ) ;
				SMPVert->DiffuseColor.b = ( BYTE )_FTOL( bd * MatDifColor.b ) ;
				SMPVert->DiffuseColor.a = MatDifColor.a ;
			}

			RGBtoVMaxRGB( Specular.r, Specular.g, Specular.b, rd, gd, bd ) ; 

			if( UseVertSpe )
			{
				SMPVert->SpecularColor.r = ( BYTE )_FTOL( rd * MVert->SpecularColor.r ) ;
				SMPVert->SpecularColor.g = ( BYTE )_FTOL( gd * MVert->SpecularColor.g ) ;
				SMPVert->SpecularColor.b = ( BYTE )_FTOL( bd * MVert->SpecularColor.b ) ;
				SMPVert->SpecularColor.a = MVert->SpecularColor.a ;
			}
			else
			{
				SMPVert->SpecularColor.r = ( BYTE )_FTOL( rd * MatSpeColor.r ) ;
				SMPVert->SpecularColor.g = ( BYTE )_FTOL( gd * MatSpeColor.g ) ;
				SMPVert->SpecularColor.b = ( BYTE )_FTOL( bd * MatSpeColor.b ) ;
				SMPVert->SpecularColor.a = MatSpeColor.a ;
			}

//			SMPVert->DiffuseColor  = UseVertDif ? MVert->DiffuseColor  : MatDifColor ;
//			SMPVert->SpecularColor = UseVertSpe ? MVert->SpecularColor : MatSpeColor ;

/*
			SMPVert->DiffuseColor.r = ( BYTE )_FTOL( Diffuse * 255.0f ) ;
			SMPVert->DiffuseColor.g = ( BYTE )_FTOL( Diffuse * 255.0f ) ;
			SMPVert->DiffuseColor.b = ( BYTE )_FTOL( Diffuse * 255.0f ) ;

			SMPVert->SpecularColor.r = ( BYTE )_FTOL( Specular * 255.0f ) ;
			SMPVert->SpecularColor.g = ( BYTE )_FTOL( Specular * 255.0f ) ;
			SMPVert->SpecularColor.b = ( BYTE )_FTOL( Specular * 255.0f ) ;
*/
//			SMPVert->DiffuseColor.r = 255 ;
//			SMPVert->DiffuseColor.g = 255 ;
//			SMPVert->DiffuseColor.b = 255 ;
//			SMPVert->SpecularColor.r = 0 ;
//			SMPVert->SpecularColor.g = 0 ;
//			SMPVert->SpecularColor.b = 0 ;

//			DiffuseV  = Diffuse.x  * 0.29891f + Diffuse.y  * 0.58661f * Diffuse.z  * 0.11448f ;
//			SpecularV = Specular.x * 0.29891f + Specular.y * 0.58661f * Specular.z * 0.11448f ;
//			DiffuseV  = Diffuse.x  * 0.33333f + Diffuse.y  * 0.33333f * Diffuse.z  * 0.33333f ;
//			SpecularV = Specular.x * 0.33333f + Specular.y * 0.33333f * Specular.z * 0.33333f ;
//			SMPVert->UVs[ 1 ][ 0 ] = DiffuseV * 2.0f;
//			SMPVert->UVs[ 1 ][ 1 ] = DiffuseV * 2.0f;
//			SMPVert->UVs[ 2 ][ 0 ] = SpecularV * 2.0f;
//			SMPVert->UVs[ 2 ][ 1 ] = SpecularV * 2.0f;
			DiffuseV  = Diffuse.r  > Diffuse.g  ? ( Diffuse.r  > Diffuse.b  ? Diffuse.r  : Diffuse.b  ) : Diffuse.g  > Diffuse.b  ? Diffuse.g  : Diffuse.b  ;
			SpecularV = Specular.r > Specular.g ? ( Specular.r > Specular.b ? Specular.r : Specular.b ) : Specular.g > Specular.b ? Specular.g : Specular.b ;
			SMPVert->UVs[ 1 ][ 0 ] = DiffuseV ;
			SMPVert->UVs[ 1 ][ 1 ] = DiffuseV ;
			SMPVert->UVs[ 2 ][ 0 ] = SpecularV ;
			SMPVert->UVs[ 2 ][ 1 ] = SpecularV ;
		}
	}
}

// �w��̃��b�V���̊�{�����̃}�e���A�����Z�b�g�A�b�v����
__inline bool MV1_D3D9_SetupMeshDrawMaterialCommon(
	int *VertexShaderIndex, int *PixelShaderIndex,
	int *VertexShaderIndex_PL, int *PixelShaderIndex_PL,
	GRAPHICS_HARDDATA_DIRECT3D9_BLENDINFO *BlendInfo, MV1_MESH *Mesh, int *SpecularEnable )
{
	MV1_MESH_BASE *MBMesh ;
	GRAPHICS_HARDDATA_DIRECT3D9_BLENDINFO *pBlendInfo ; 
	MATERIALPARAM Material ;
	MV1_MATERIAL * RST MMaterial ;
	MV1_MATERIAL_BASE * RST MBMaterial ;
	MV1_FRAME * RST Frame ;

	Frame = Mesh->Container ;
	MBMesh = Mesh->BaseData ;
	MMaterial = Mesh->Material ;
	MBMaterial = MMaterial->BaseData ;

	// �}�e���A���̃Z�b�g
	if( Mesh->SetupDrawMaterial.UseColorScale )
	{
		// �A���t�@���O�̏ꍇ�͕`�悵�Ȃ�
		if( Mesh->SetupDrawMaterial.DiffuseScale.a * MMaterial->Diffuse.a <= 0.0000001f )
			return false ;

		Material.Diffuse.r = MMaterial->Diffuse.r * Mesh->SetupDrawMaterial.DiffuseScale.r ;
		Material.Diffuse.g = MMaterial->Diffuse.g * Mesh->SetupDrawMaterial.DiffuseScale.g ;
		Material.Diffuse.b = MMaterial->Diffuse.b * Mesh->SetupDrawMaterial.DiffuseScale.b ;
		Material.Diffuse.a = MMaterial->Diffuse.a * Mesh->SetupDrawMaterial.DiffuseScale.a ;
		Material.Specular.r = MMaterial->Specular.r * Mesh->SetupDrawMaterial.SpecularScale.r ;
		Material.Specular.g = MMaterial->Specular.g * Mesh->SetupDrawMaterial.SpecularScale.g ;
		Material.Specular.b = MMaterial->Specular.b * Mesh->SetupDrawMaterial.SpecularScale.b ;
		Material.Specular.a = MMaterial->Specular.a * Mesh->SetupDrawMaterial.SpecularScale.a ;
		Material.Emissive.r = MMaterial->Emissive.r * Mesh->SetupDrawMaterial.EmissiveScale.r ;
		Material.Emissive.g = MMaterial->Emissive.g * Mesh->SetupDrawMaterial.EmissiveScale.g ;
		Material.Emissive.b = MMaterial->Emissive.b * Mesh->SetupDrawMaterial.EmissiveScale.b ;
		Material.Emissive.a = MMaterial->Emissive.a * Mesh->SetupDrawMaterial.EmissiveScale.a ;
		Material.Ambient.r = MMaterial->Ambient.r * Mesh->SetupDrawMaterial.AmbientScale.r ;
		Material.Ambient.g = MMaterial->Ambient.g * Mesh->SetupDrawMaterial.AmbientScale.g ;
		Material.Ambient.b = MMaterial->Ambient.b * Mesh->SetupDrawMaterial.AmbientScale.b ;
		Material.Ambient.a = MMaterial->Ambient.a * Mesh->SetupDrawMaterial.AmbientScale.a ;
	}
	else
	{
		// �A���t�@���O�̏ꍇ�͕`�悵�Ȃ�
		if( MMaterial->Diffuse.a <= 0.0000001f )
			return false ;

		Material.Diffuse   = MMaterial->Diffuse ;
		Material.Specular  = MMaterial->Specular ;
		Material.Ambient   = MMaterial->Ambient ;
		Material.Emissive  = MMaterial->Emissive ;
	}
	Material.Power = MMaterial->Power ;
	GSYS.Light.ChangeMaterial = 1 ;
	GD3D9.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;
	Graphics_D3D9_DeviceState_SetMaterial( ( D_D3DMATERIAL9 * )&Material ) ;

	// �J�����O�ݒ�
	Graphics_D3D9_DeviceState_SetCullMode( GSYS.DrawSetting.ShadowMapDraw ? FALSE : MBMesh->BackCulling ) ; 

	// �u�����h�ݒ�̃Z�b�g�A�b�v
	BlendInfo->SeparateAlphaBlendEnable = FALSE ;
	BlendInfo->SrcBlendAlpha            = -1 ;
	BlendInfo->DestBlendAlpha           = -1 ;
	BlendInfo->BlendOpAlpha             = -1 ;
	BlendInfo->AlphaTestEnable          = MMaterial->UseAlphaTest ;
	BlendInfo->AlphaFunc                = MMaterial->AlphaFunc ;
	BlendInfo->AlphaRef                 = MMaterial->AlphaRef ;
	BlendInfo->FactorColor              = 0x00ffffff | ( ( ( DWORD )_FTOL( Mesh->SetupDrawMaterial.OpacityRate * ( Mesh->DrawBlendParam != -1 ? Mesh->DrawBlendParam : MMaterial->DrawBlendParam ) ) ) << 24 ) ;
	pBlendInfo = &GD3D9.Device.State.BlendInfo ;
	switch( GSYS.DrawSetting.ShadowMapDraw ? DX_BLENDMODE_NOBLEND : ( Mesh->DrawBlendMode != -1 ? Mesh->DrawBlendMode : MMaterial->DrawBlendMode ) )
	{
	case DX_BLENDMODE_NOBLEND :
		BlendInfo->AlphaBlendEnable = FALSE ;
		BlendInfo->SrcBlend         = pBlendInfo->SrcBlend ;
		BlendInfo->DestBlend        = pBlendInfo->DestBlend ;
		BlendInfo->BlendOp          = pBlendInfo->BlendOp ;
		break ;

	case DX_BLENDMODE_ALPHA :
		BlendInfo->AlphaBlendEnable = TRUE ;
		if( Frame->Container->UseDrawMulAlphaColor )
		{
			BlendInfo->SrcBlend     = D_D3DBLEND_ONE ;
		}
		else
		{
			BlendInfo->SrcBlend     = D_D3DBLEND_SRCALPHA ;
		}
		BlendInfo->DestBlend        = D_D3DBLEND_INVSRCALPHA ;
		BlendInfo->BlendOp          = D_D3DBLENDOP_ADD ;
		break ;

	case DX_BLENDMODE_ADD :
		BlendInfo->AlphaBlendEnable = TRUE ;
		if( Frame->Container->UseDrawMulAlphaColor )
		{
			BlendInfo->SrcBlend     = D_D3DBLEND_ONE ;
		}
		else
		{
			BlendInfo->SrcBlend     = D_D3DBLEND_SRCALPHA ;
		}
		BlendInfo->DestBlend        = D_D3DBLEND_ONE ;
		BlendInfo->BlendOp          = D_D3DBLENDOP_ADD ;
		break ;

	case DX_BLENDMODE_INVSRC :
		BlendInfo->AlphaBlendEnable = TRUE ;
		BlendInfo->SrcBlend         = D_D3DBLEND_SRCALPHA ;
		BlendInfo->DestBlend        = D_D3DBLEND_INVSRCALPHA ;
		BlendInfo->BlendOp          = D_D3DBLENDOP_ADD ;
		break ;

	case DX_BLENDMODE_MUL :
	case DX_BLENDMODE_MULA :
		BlendInfo->AlphaBlendEnable = TRUE ;
		BlendInfo->SrcBlend         = D_D3DBLEND_ZERO ;
		BlendInfo->DestBlend        = D_D3DBLEND_SRCCOLOR ;
		BlendInfo->BlendOp          = D_D3DBLENDOP_ADD ;
		break ;

	case DX_BLENDMODE_SRCCOLOR :
		BlendInfo->AlphaBlendEnable = TRUE ;
		BlendInfo->SrcBlend         = D_D3DBLEND_ONE ;
		BlendInfo->DestBlend        = D_D3DBLEND_ZERO ;
		BlendInfo->BlendOp          = D_D3DBLENDOP_ADD ;
		break ;
	}
	BlendInfo->UseTextureStageNum = MMaterial->DiffuseLayerNum ;

	// �X�y�L�������L�����ǂ������Z�b�g����
	*SpecularEnable = 
		(
		MBMesh->UseVertexSpecularColor ||
		Material.Specular.r > 0.00001f ||
		Material.Specular.g > 0.00001f ||
		Material.Specular.b > 0.00001f
		) ;
	if( GSYS.HardInfo.UseShader == FALSE && ( MMaterial->BaseData->Type == DX_MATERIAL_TYPE_TOON || MMaterial->BaseData->Type == DX_MATERIAL_TYPE_TOON_2 ) )
	{
		*SpecularEnable = FALSE ;
	}
	Graphics_D3D9_DeviceState_SetSpecularEnable( *SpecularEnable ) ;

	if( GSYS.HardInfo.UseShader == FALSE && ( MMaterial->BaseData->Type == DX_MATERIAL_TYPE_TOON || MMaterial->BaseData->Type == DX_MATERIAL_TYPE_TOON_2 ) )
	{
		// �V�F�[�_�[���g�����g�D�[�������_�����O�̏ꍇ�͕K�����_�J���[���g�p����

		// �f�B�t���[�Y�}�e���A���Ƃ��Ē��_�J���[���g�p���邩�ǂ������Z�b�g
		Graphics_D3D9_DeviceState_SetDiffuseMaterialSource( TRUE ) ;

		// �X�y�L�����}�e���A���Ƃ��Ē��_�J���[���g�p���邩�ǂ������Z�b�g
		Graphics_D3D9_DeviceState_SetSpecularMaterialSource( TRUE ) ;
	}
	else
	{
		// �f�B�t���[�Y�}�e���A���Ƃ��Ē��_�J���[���g�p���邩�ǂ������Z�b�g
		Graphics_D3D9_DeviceState_SetDiffuseMaterialSource( MBMesh->UseVertexDiffuseColor ) ;

		// �X�y�L�����}�e���A���Ƃ��Ē��_�J���[���g�p���邩�ǂ������Z�b�g
		Graphics_D3D9_DeviceState_SetSpecularMaterialSource( MBMesh->UseVertexSpecularColor ) ;
	}

	// �g�D�[�������_�����O�̏ꍇ�̓��C�e�B���O�v�Z�͍s��Ȃ�
	if( MMaterial->BaseData->Type == DX_MATERIAL_TYPE_TOON || MMaterial->BaseData->Type == DX_MATERIAL_TYPE_TOON_2 )
	{
		Graphics_D3D9_DeviceState_SetLighting( FALSE ) ;
	}
	else
	{
		NS_SetUseLighting( GSYS.Light.ProcessDisable ? FALSE : TRUE ) ;
	}

	// �g�D�[�������_�����O�̃Z�b�g
	if( MMaterial->BaseData->Type == DX_MATERIAL_TYPE_TOON || MMaterial->BaseData->Type == DX_MATERIAL_TYPE_TOON_2 )
	{
		switch( MMaterial->BaseData->Type )
		{
		case DX_MATERIAL_TYPE_TOON :
			*PixelShaderIndex    += D3D9_PIXELSHADER_TOON( 1 ) ;
			*PixelShaderIndex    += D3D9_PIXELSHADER_TOONTYPE( 0 ) ;
			*PixelShaderIndex_PL += D3D9_PIXELLIGHTING_PIXELSHADER_TYPE( D3D9_PIXELLIGHTING_PIXELSHADER_TYPE_TOON ) ;
			*PixelShaderIndex_PL += D3D9_PIXELLIGHTING_PIXELSHADER_TOONTYPE( 0 ) ;
			if( MMaterial->DiffuseGradBlendType == MV1_LAYERBLEND_TYPE_MODULATE )
			{
				*PixelShaderIndex    += D3D9_PIXELSHADER_TOONDIFBLDOP( 1 ) ;
				*PixelShaderIndex_PL += D3D9_PIXELLIGHTING_PIXELSHADER_TOONDIFBLDOP( 1 ) ;
			}
			if( MMaterial->SpecularGradBlendType == MV1_LAYERBLEND_TYPE_ADDITIVE && *SpecularEnable )
			{
				*PixelShaderIndex    += D3D9_PIXELSHADER_TOONSPCBLDOP( 1 ) ;
				*PixelShaderIndex_PL += D3D9_PIXELLIGHTING_PIXELSHADER_TOONSPCBLDOP( 1 ) ;
			}
			break ;

		case DX_MATERIAL_TYPE_TOON_2 :
			*PixelShaderIndex    += D3D9_PIXELSHADER_TOON( 1 ) ;
			*PixelShaderIndex    += D3D9_PIXELSHADER_TOONTYPE( 1 ) ;
			*PixelShaderIndex_PL += D3D9_PIXELLIGHTING_PIXELSHADER_TYPE( D3D9_PIXELLIGHTING_PIXELSHADER_TYPE_TOON ) ;
			*PixelShaderIndex_PL += D3D9_PIXELLIGHTING_PIXELSHADER_TOONTYPE( 1 ) ;
			if( MMaterial->SphereMapTexture >= 0 )
			{
				if( MMaterial->SphereMapBlendType == MV1_LAYERBLEND_TYPE_MODULATE )
				{
					*PixelShaderIndex    += D3D9_PIXELSHADER_TOONSPHEREOP( 1 ) ;
					*PixelShaderIndex_PL += D3D9_PIXELLIGHTING_PIXELSHADER_TOONSPHEREOP( 1 ) ;
				}
				else
				if( MMaterial->SphereMapBlendType == MV1_LAYERBLEND_TYPE_ADDITIVE )
				{
					*PixelShaderIndex    += D3D9_PIXELSHADER_TOONSPHEREOP( 2 ) ;
					*PixelShaderIndex_PL += D3D9_PIXELLIGHTING_PIXELSHADER_TOONSPHEREOP( 2 ) ;
				}
			}
			break ;
		}
	}

	// �T�u�e�N�X�`�����g�p���邩�ǂ������Z�b�g
	if( MBMaterial->DiffuseLayerNum > 1 )
	{
		*PixelShaderIndex    += D3D9_PIXELSHADER_MULTITEX( MBMaterial->DiffuseLayer[ 1 ].BlendType + 1 ) ;
		*PixelShaderIndex_PL += D3D9_PIXELLIGHTING_PIXELSHADER_MULTITEX( MBMaterial->DiffuseLayer[ 1 ].BlendType + 1 ) ;
	}

	// �}�e���A���Ƀo���v�}�b�v���܂܂��ꍇ�̓V�F�[�_�[�C���f�b�N�X��␳
	if( MBMaterial->NormalLayerNum )
	{
		*VertexShaderIndex    += D3D9_VERTEXSHADER_BUMPMAP( 1 ) ;
		*PixelShaderIndex     += D3D9_PIXELSHADER_BUMPMAP( 1 ) ;

		*VertexShaderIndex_PL += D3D9_PIXELLIGHTING_VERTEXSHADER_BUMPMAP( 1 ) ;
		*PixelShaderIndex_PL  += D3D9_PIXELLIGHTING_PIXELSHADER_BUMPMAP( 1 ) ;
	}

	// �}�e���A���ɃX�y�L�������܂܂��ꍇ�̓X�y�L�����C���f�b�N�X��ǉ�
	if( *SpecularEnable )
	{
		*VertexShaderIndex    += D3D9_VERTEXSHADER_SPECULAR( 1 ) ;
		*PixelShaderIndex     += D3D9_PIXELSHADER_SPECULAR( 1 ) ;

		*PixelShaderIndex_PL  += D3D9_PIXELLIGHTING_PIXELSHADER_SPECULAR( 1 ) ;
	}

	// �}�e���A���ɃX�y�L�����}�b�v���܂܂��ꍇ�̓X�y�L�����}�b�v�C���f�b�N�X��ǉ�
	if( MMaterial->SpecularLayerNum )
	{
		*PixelShaderIndex    += D3D9_PIXELSHADER_SPECULARMAP( 1 ) ;
		*PixelShaderIndex_PL += D3D9_PIXELLIGHTING_PIXELSHADER_SPECULARMAP( 1 ) ;
	}

	return true ;
}

// �w��̃��b�V���̃g�D�[�������_�����O�̗֊s���p�̃}�e���A�����Z�b�g�A�b�v����
static void MV1_D3D9_SetupToonOutLineMeshDrawMaterial(
	MV1_MODEL_BASE * /*ModelBase*/,
	GRAPHICS_HARDDATA_DIRECT3D9_BLENDINFO *BlendInfo,
	MV1_MESH *Mesh,
	bool UseShader )
{
	GRAPHICS_HARDDATA_DIRECT3D9_TEXTURESTAGEINFO *StageInfo ;
	MV1_FRAME *Frame ;
	MV1_MODEL *Model ;
	MV1_MATERIAL * RST MMaterial ;
	int StageNum, BlendMode ;

	// ���ʕ����̃Z�b�g

	Frame = Mesh->Container ;
	Model = Frame->Container ;
	MMaterial = Mesh->Material ;
	BlendMode = Mesh->DrawBlendMode != -1 ? Mesh->DrawBlendMode : MMaterial->DrawBlendMode ;

	// �f�t�H���g�e�N�X�`���n���h���̏������`�F�b�N
	if( MV1Man.TexNoneHandle < 0 )
	{
		MV1SetupTexNoneHandle() ;
		Graphics_D3D9_BeginScene() ;
	}

	// �V�F�[�_�[���g�p�\���ǂ����ŏ����𕪊�
	if( UseShader )
	{
		// �e�N�X�`���̃Z�b�g�A�b�v
		StageInfo = BlendInfo->TextureStageInfo ;

		BlendInfo->UseTextureStageNum = 3 ;
		StageInfo[ 0 ].TextureCoordIndex = 0 ;
		StageInfo[ 0 ].Texture = ( void * )( DWORD_PTR )MV1Man.TexNoneHandle ;

		// �ݒ�𔽉f
		Graphics_D3D9_DeviceState_SetUserBlendInfo( BlendInfo, TRUE ) ;

		// �g�p����e�N�X�`�����W�����Z�b�g
		Graphics_D3D9_DeviceState_ResetTextureCoord() ;
	}
	else
	{
		// �e�N�X�`���̃Z�b�g�A�b�v
		StageInfo = BlendInfo->TextureStageInfo ;

		BlendInfo->UseTextureStageNum = 1 ;
		StageInfo[ 0 ].Texture = ( void * )( DWORD_PTR )MV1Man.TexNoneHandle ;

		StageInfo[ 0 ].TextureCoordIndex = 0 ;
		StageInfo[ 0 ].ResultTempARG = FALSE ;
		StageInfo[ 0 ].ColorARG1 = D_D3DTA_TEXTURE ;
		StageInfo[ 0 ].ColorARG2 = D_D3DTA_DIFFUSE ;
		StageInfo[ 0 ].ColorOP   = D_D3DTOP_MODULATE ;
		StageInfo[ 0 ].AlphaARG1 = D_D3DTA_TEXTURE ;
		StageInfo[ 0 ].AlphaARG2 = D_D3DTA_DIFFUSE ;
		StageInfo[ 0 ].AlphaOP   = D_D3DTOP_MODULATE ;
		StageNum = 1 ;

		switch( BlendMode )
		{
		case DX_BLENDMODE_MULA :
			if( BlendInfo->FactorColor != 0xffffffff )
			{
				StageInfo[ StageNum ].Texture   = NULL ;
				StageInfo[ StageNum ].TextureCoordIndex = 0 ;
				StageInfo[ StageNum ].ColorARG1 = D_D3DTA_CURRENT ;
				StageInfo[ StageNum ].ColorARG2 = D_D3DTA_TFACTOR ;
				StageInfo[ StageNum ].ColorOP   = D_D3DTOP_BLENDFACTORALPHA ;
				StageInfo[ StageNum ].AlphaARG1 = D_D3DTA_CURRENT ;
				StageInfo[ StageNum ].AlphaARG2 = D_D3DTA_DIFFUSE ;
				StageInfo[ StageNum ].AlphaOP   = D_D3DTOP_SELECTARG1 ;
				StageNum ++ ;
			}
			break ;

		default :
			if( BlendInfo->FactorColor != 0xffffffff )
			{
				StageInfo[ StageNum ].Texture   = NULL ;
				StageInfo[ StageNum ].TextureCoordIndex = 0 ;
				StageInfo[ StageNum ].ColorARG1 = D_D3DTA_CURRENT ;
				StageInfo[ StageNum ].ColorARG2 = D_D3DTA_DIFFUSE ;
				StageInfo[ StageNum ].ColorOP   = D_D3DTOP_SELECTARG1 ;
				StageInfo[ StageNum ].AlphaARG1 = D_D3DTA_CURRENT ;
				StageInfo[ StageNum ].AlphaARG2 = D_D3DTA_TFACTOR ;
				StageInfo[ StageNum ].AlphaOP   = D_D3DTOP_MODULATE ;
				StageNum ++ ;
			}
			break ;
		}
		StageInfo[ StageNum ].ResultTempARG = FALSE ;

		StageInfo[ StageNum ].Texture   = NULL ;
		StageInfo[ StageNum ].TextureCoordIndex = 0 ;
		StageInfo[ StageNum ].ResultTempARG = FALSE ;
		StageInfo[ StageNum ].ColorARG1 = D_D3DTA_TEXTURE ;
		StageInfo[ StageNum ].ColorARG2 = D_D3DTA_DIFFUSE ;
		StageInfo[ StageNum ].ColorOP   = D_D3DTOP_DISABLE ;
		StageInfo[ StageNum ].AlphaARG1 = D_D3DTA_TEXTURE ;
		StageInfo[ StageNum ].AlphaARG2 = D_D3DTA_DIFFUSE ;
		StageInfo[ StageNum ].AlphaOP   = D_D3DTOP_DISABLE ;
		StageNum ++ ;

		BlendInfo->UseTextureStageNum = StageNum ;

		// �ݒ�𔽉f
		Graphics_D3D9_DeviceState_SetUserBlendInfo( BlendInfo, FALSE ) ;
	}
}

// �w��̃g���C�A���O�����X�g�̗֊s���p�e���|���������Z�b�g�A�b�v����
static bool MV1_D3D9_SetupTListTempToonOutLineSimpleVertex( MV1_TRIANGLE_LIST_BASE *MBTList )
{
	MV1_VERTEX_SIMPLE_TOL_D3D9	*TOLVert ;
	MV1_TLIST_NORMAL_POS		*Norm ;
	MV1_TRIANGLE_LIST_BASE_PF	*MBTListPF ;
	DWORD						i ;
	DWORD						VertexNum ;

	MBTListPF = ( MV1_TRIANGLE_LIST_BASE_PF * )MBTList->PFBuffer ;

	// ���Ɋm�ۂ���Ă����牽�������ɏI��
	if( MBTListPF->D3D9.TempToonOutLineSimpleVertex )
		return true ;

	// �������̊m��
	MBTListPF->D3D9.TempToonOutLineSimpleVertex = ( MV1_VERTEX_SIMPLE_TOL_D3D9 * )DXALLOC( sizeof( MV1_VERTEX_SIMPLE_TOL_D3D9 ) * MBTList->VertexNum + 16 ) ;
	if( MBTListPF->D3D9.TempToonOutLineSimpleVertex == NULL )
		return false ;

	// ���̃��b�V���̏ꍇ�̂ݍ��W���v�Z���Ă��܂�
	if( MBTList->VertexType == MV1_VERTEX_TYPE_NORMAL )
	{
		float OutLineWidth ;

		OutLineWidth = MBTList->Container->Material->OutLineWidth ;

		TOLVert = ( MV1_VERTEX_SIMPLE_TOL_D3D9 * )( ( ( DWORD_PTR )MBTListPF->D3D9.TempToonOutLineSimpleVertex + 15 ) / 16 * 16 ) ;
		Norm    = ( MV1_TLIST_NORMAL_POS * )ADDR16( MBTList->NormalPosition ) ;
		VertexNum = ( DWORD )MBTList->VertexNum ;
		for( i = 0 ; i < VertexNum ; i ++, Norm ++, TOLVert ++ )
		{
			TOLVert->Position = VAdd( *( ( VECTOR * )&Norm->Position ), VScale( *( ( VECTOR * )&Norm->Normal ), OutLineWidth ) ) ;
		}
	}

	// ����I��
	return true ;
}























// ���ˑ��֐�

// ���f���@�\�̌�n��
extern int MV1_D3D9_Terminate_PF( void )
{
	return 0 ;
}

// ���f���f�[�^�n���h���̌�n��
extern int MV1_D3D9_TerminateModelBaseHandle_PF( MV1_MODEL_BASE *ModelBase )
{
	// �I��
	return 0 ;
}

// �g���C�A���O�����X�g�̈ꎞ�����p�̃o�b�t�@���J������
extern int MV1_D3D9_TerminateTriangleListBaseTempBuffer_PF( MV1_TRIANGLE_LIST_BASE *MBTList )
{
	MV1_TRIANGLE_LIST_BASE_PF *MBTListPF ;

	MBTListPF = ( MV1_TRIANGLE_LIST_BASE_PF * )MBTList->PFBuffer ;

	if( MBTListPF->D3D9.TempSimpleVertex )
	{
		DXFREE( MBTListPF->D3D9.TempSimpleVertex ) ;
		MBTListPF->D3D9.TempSimpleVertex = NULL ;
	}
	if( MBTListPF->D3D9.TempToonOutLineSimpleVertex )
	{
		DXFREE( MBTListPF->D3D9.TempToonOutLineSimpleVertex ) ;
		MBTListPF->D3D9.TempToonOutLineSimpleVertex = NULL ;
	}

	// �I��
	return 0 ;
}

// ���������`��֌W�̏����Z�b�g�A�b�v����
extern	void			MV1_D3D9_SetupPackDrawInfo_PF( MV1_MODEL_BASE *ModelBase )
{
	MV1_TRIANGLE_LIST_BASE *MBTList ;
	int MaxPackDrawNum ;
	int UseBoneNum ;
	int i ;

	// �ő哯���`�搔������o��
	ModelBase->PackDrawMaxNum = WORD_MAX / ModelBase->TriangleListMinVertexNum ;

	MaxPackDrawNum = WORD_MAX / ModelBase->TriangleListMinIndexNum ;
	if( ModelBase->PackDrawMaxNum > MaxPackDrawNum )
	{
		ModelBase->PackDrawMaxNum = MaxPackDrawNum ;
	}

	MaxPackDrawNum = DX_VS_CONSTF_WORLD_MAT_NUM / ModelBase->TriangleListMinMatrixNum ;
	if( ModelBase->PackDrawMaxNum > MaxPackDrawNum )
	{
		ModelBase->PackDrawMaxNum = MaxPackDrawNum ;
	}

	MBTList = ModelBase->TriangleList ;
	for( i = 0 ; i < ModelBase->TriangleListNum ; i ++, MBTList ++ )
	{
		switch( MBTList->VertexType )
		{
		case MV1_VERTEX_TYPE_NORMAL :
			UseBoneNum = 1 ;
			break ;

		case MV1_VERTEX_TYPE_SKIN_FREEBONE :
			continue ;

		default :
			UseBoneNum = MBTList->UseBoneNum ;
			break ;
		}

		MBTList->PackDrawMaxNum = WORD_MAX / MBTList->VertexNum ;

		MaxPackDrawNum = WORD_MAX / ( MBTList->IndexNum + MBTList->ToonOutLineIndexNum ) ;
		if( MBTList->PackDrawMaxNum > MaxPackDrawNum )
		{
			MBTList->PackDrawMaxNum = MaxPackDrawNum ;
		}

		MaxPackDrawNum = DX_VS_CONSTF_WORLD_MAT_NUM / UseBoneNum ;
		if( MBTList->PackDrawMaxNum > MaxPackDrawNum )
		{
			MBTList->PackDrawMaxNum = MaxPackDrawNum ;
		}
	}
}

// ���f����f�[�^�̒��_�o�b�t�@�̃Z�b�g�A�b�v������( -1:�G���[ )
extern	int				MV1_D3D9_SetupVertexBufferBase_PF( int MV1ModelBaseHandle, int DuplicateNum, int ASyncThread )
{
	int i, j, k, l, Type, UVType, Bump, VertexNum, UVNum, MeshVertSize, BaseMatrixIndex ;
	MV1_MODEL_BASE *MBase ;
	MV1_MESH_NORMAL *MeshNorm, *MNorm ;
	MV1_MESH_VERTEX *MeshVert, *MVert ;
	MV1_TRIANGLE_LIST_BASE *MBTList ;
	MV1_VERTEXBUFFER *VBuf[ 3 ][ 2 ][ 9 ], *TVBuf, *TVBuf2 ;
	DWORD *MVInd ;
	DWORD TexOrParam ;
	int IndexNum ;

	// �n�[�h�E�G�A�s���k���g�p�ł��Ȃ��ꍇ�͉������Ȃ�
	if( GSYS.Setting.ValidHardware == FALSE || ( GD3D9.Device.Caps.VertexHardwareProcess == FALSE && GD3D9.Device.Shader.ValidVertexShader == FALSE ) )
		return 0 ;

	// �n���h���`�F�b�N
	if( MV1BMDLCHK( MV1ModelBaseHandle, MBase ) )
		return -1 ;

	// ���ɃZ�b�g�A�b�v����Ă���ꍇ�͉��������I��
	if( MBase->SetupVertexBuffer )
		return 0 ;

	// ��]����I�u�W�F�N�g�R�s�[�̐���ۑ�����
	MBase->ObjectDuplicateNum = DuplicateNum ;

	// ���_�o�b�t�@�̐��𒲂ׂ�
	_MEMSET( MBase->VertexBufferFirst, 0, sizeof( MBase->VertexBufferFirst ) ) ;
	_MEMSET( MBase->VertexBufferLast, 0, sizeof( MBase->VertexBufferLast ) ) ;
	_MEMSET( MBase->VertexBufferNum, 0, sizeof( MBase->VertexBufferNum ) ) ;
	MBase->TotalVertexBufferNum = 0 ;
	_MEMSET( VBuf, 0, sizeof( VBuf ) ) ;
	MBTList = MBase->TriangleList ;
	for( i = 0 ; i < MBase->TriangleListNum ; i ++, MBTList ++ )
	{
		// ���_�^�C�v���Z�b�g
		UVType = MBTList->Container->UVSetUnitNum ;
		switch( MBTList->VertexType )
		{
		case MV1_VERTEX_TYPE_NORMAL :
			Type = MBase->UsePackDraw ? 1 : 0 ;
			break ;

		case MV1_VERTEX_TYPE_SKIN_4BONE :
			Type = 1 ;
			break ;

		case MV1_VERTEX_TYPE_SKIN_8BONE :
			Type = 2 ;
			break ;

		default :
			continue ;
		}

		// ���������`��ɑΉ�����ꍇ�͒��_���͍ő哯���`��\���{�ɂȂ�
		VertexNum = MBTList->VertexNum ;
		IndexNum  = MBTList->IndexNum + MBTList->ToonOutLineIndexNum ;
		if( MBase->UsePackDraw )
		{
			VertexNum *= MBTList->PackDrawMaxNum ;
			IndexNum  *= MBTList->PackDrawMaxNum ;
		}

		// �o���v�}�b�v�̗L�����Z�b�g
		Bump = MBTList->Container->Material->NormalLayerNum ? 1 : 0 ;

		// �����̐���ۑ�����
//		MBTList->VertexDuplicateNum = ObjectDuplicateNum ;

		// �^�C�v�̃o�b�t�@���m�ۂ���Ă��Ȃ�������m��
		if( VBuf[ Type ][ Bump ][ UVType ] == NULL )
		{
			MBase->TotalVertexBufferNum ++ ;
			MBase->VertexBufferNum[ Type ][ Bump ][ UVType ] ++ ;
			VBuf[ Type ][ Bump ][ UVType ] = ( MV1_VERTEXBUFFER * )DXALLOC( sizeof( MV1_VERTEXBUFFER ) + sizeof( MV1_VERTEXBUFFER_PF ) ) ;
			_MEMSET( VBuf[ Type ][ Bump ][ UVType ], 0, sizeof( MV1_VERTEXBUFFER ) + sizeof( MV1_VERTEXBUFFER_PF ) ) ;
			VBuf[ Type ][ Bump ][ UVType ]->PF = ( MV1_VERTEXBUFFER_PF * )( VBuf[ Type ][ Bump ][ UVType ] + 1 ) ;
			MBase->VertexBufferFirst[ Type ][ Bump ][ UVType ] = VBuf[ Type ][ Bump ][ UVType ] ;
			MBase->VertexBufferLast[ Type ][ Bump ][ UVType ]  = VBuf[ Type ][ Bump ][ UVType ] ;
		}
		TVBuf = VBuf[ Type ][ Bump ][ UVType ] ;

		// �K�萔�𒴂��Ă��܂��ꍇ�͎��̃o�b�t�@���쐬
		if( TVBuf->IndexCount  + IndexNum  >= MV1_INDEXBUFFER_MAX_INDEXNUM ||
			TVBuf->VertexCount + VertexNum >= MV1_VERTEXBUFFER_MAX_VERTNUM )
		{
			MBase->TotalVertexBufferNum ++ ;
			MBase->VertexBufferNum[ Type ][ Bump ][ UVType ] ++ ;
			TVBuf->DataNext = ( MV1_VERTEXBUFFER * )DXALLOC( sizeof( MV1_VERTEXBUFFER ) + sizeof( MV1_VERTEXBUFFER_PF ) ) ;
			_MEMSET( TVBuf->DataNext, 0, sizeof( MV1_VERTEXBUFFER ) + sizeof( MV1_VERTEXBUFFER_PF ) ) ;
			TVBuf->DataNext->PF = ( MV1_VERTEXBUFFER_PF * )( TVBuf->DataNext + 1 ) ;
			TVBuf->DataNext->DataPrev = TVBuf ;
			MBase->VertexBufferLast[ Type ][ Bump ][ UVType ] = TVBuf->DataNext ;
			VBuf[ Type ][ Bump ][ UVType ] = TVBuf->DataNext ;

			TVBuf = VBuf[ Type ][ Bump ][ UVType ] ;
		}

		// �g�p����o�b�t�@��ۑ�
		MBTList->VertexBuffer = TVBuf ;

		// �J�n�A�h���X��ۑ�
		MBTList->VBStartVertex               = TVBuf->VertexCount ;
		MBTList->VBStartIndex                = TVBuf->IndexCount ;
		if( MBase->UsePackDraw )
		{
			MBTList->ToonOutLineVBStartIndex = MBTList->VBStartIndex + MBTList->IndexNum * MBTList->PackDrawMaxNum ;
		}
		else
		{
			MBTList->ToonOutLineVBStartIndex = MBTList->VBStartIndex + MBTList->IndexNum ;
		}

		// ���𑫂�
		TVBuf->VertexCount += VertexNum ;
		TVBuf->IndexCount  += IndexNum ;

	}

	// �������_���Ȃ��o�b�t�@�͍폜����
	for( i = 0 ; i < 3 ; i ++ )
	{
		for( j = 0 ; j < 2 ; j ++ )
		{
			for( k = 0 ; k < 9 ; k ++ )
			{
				TVBuf = MBase->VertexBufferLast[ i ][ j ][ k ] ;
				if( TVBuf == NULL || TVBuf->IndexCount != 0 ) continue ;

				MBase->VertexBufferLast[ i ][ j ][ k ] = TVBuf->DataPrev ;
				if( TVBuf->DataPrev ) TVBuf->DataPrev->DataNext = NULL ;
				DXFREE( TVBuf ) ;

				MBase->TotalVertexBufferNum -- ;
				MBase->VertexBufferNum[ i ][ j ][ k ] -- ;
			}
		}
	}

	// ���_�o�b�t�@���쐬����
	for( i = 0 ; i < 3 ; i ++ )
	{
		for( j = 0 ; j < 2 ; j ++ )
		{
			for( k = 0 ; k < 9 ; k ++ )
			{
				TexOrParam = 0 ;
				switch( k )
				{
				case 1 : TexOrParam = D_D3DFVF_TEX1 | D_D3DFVF_TEXCOORDSIZE4( 0 ) ; break ;
				case 2 : TexOrParam = D_D3DFVF_TEX2 | D_D3DFVF_TEXCOORDSIZE4( 0 ) | D_D3DFVF_TEXCOORDSIZE4( 1 ) ; break ;
				case 3 : TexOrParam = D_D3DFVF_TEX3 | D_D3DFVF_TEXCOORDSIZE4( 0 ) | D_D3DFVF_TEXCOORDSIZE4( 1 ) | D_D3DFVF_TEXCOORDSIZE4( 2 ) ; break ;
				case 4 : TexOrParam = D_D3DFVF_TEX4 | D_D3DFVF_TEXCOORDSIZE4( 0 ) | D_D3DFVF_TEXCOORDSIZE4( 1 ) | D_D3DFVF_TEXCOORDSIZE4( 2 ) | D_D3DFVF_TEXCOORDSIZE4( 3 ) ; break ;
				case 5 : TexOrParam = D_D3DFVF_TEX5 | D_D3DFVF_TEXCOORDSIZE4( 0 ) | D_D3DFVF_TEXCOORDSIZE4( 1 ) | D_D3DFVF_TEXCOORDSIZE4( 2 ) | D_D3DFVF_TEXCOORDSIZE4( 3 ) | D_D3DFVF_TEXCOORDSIZE4( 4 ) ; break ;
				case 6 : TexOrParam = D_D3DFVF_TEX6 | D_D3DFVF_TEXCOORDSIZE4( 0 ) | D_D3DFVF_TEXCOORDSIZE4( 1 ) | D_D3DFVF_TEXCOORDSIZE4( 2 ) | D_D3DFVF_TEXCOORDSIZE4( 3 ) | D_D3DFVF_TEXCOORDSIZE4( 4 ) | D_D3DFVF_TEXCOORDSIZE4( 5 ) ; break ;
				case 7 : TexOrParam = D_D3DFVF_TEX7 | D_D3DFVF_TEXCOORDSIZE4( 0 ) | D_D3DFVF_TEXCOORDSIZE4( 1 ) | D_D3DFVF_TEXCOORDSIZE4( 2 ) | D_D3DFVF_TEXCOORDSIZE4( 3 ) | D_D3DFVF_TEXCOORDSIZE4( 4 ) | D_D3DFVF_TEXCOORDSIZE4( 5 ) | D_D3DFVF_TEXCOORDSIZE4( 6 ) ; break ;
				case 8 : TexOrParam = D_D3DFVF_TEX8 | D_D3DFVF_TEXCOORDSIZE4( 0 ) | D_D3DFVF_TEXCOORDSIZE4( 1 ) | D_D3DFVF_TEXCOORDSIZE4( 2 ) | D_D3DFVF_TEXCOORDSIZE4( 3 ) | D_D3DFVF_TEXCOORDSIZE4( 4 ) | D_D3DFVF_TEXCOORDSIZE4( 5 ) | D_D3DFVF_TEXCOORDSIZE4( 6 ) | D_D3DFVF_TEXCOORDSIZE4( 7 ) ; break ;
				}

				VBuf[ i ][ j ][ k ] = MBase->VertexBufferFirst[ i ][ j ][ k ] ;
				for( TVBuf = VBuf[ i ][ j ][ k ] ; TVBuf ; TVBuf = TVBuf->DataNext )
				{
					// ���_�^�C�v�ɂ���ď����𕪊�
					switch( i )
					{
					case 0 :
						// ���̃��b�V���̏ꍇ
						if( j == 0 )
						{
							// �o���v�}�b�v����

							// ���_�o�b�t�@���쐬
							TVBuf->UnitSize = sizeof( MV1_VERTEX_SIMPLE_D3D9 ) + ( k - 1 ) * sizeof( float ) * 4 ;
							TVBuf->PF->D3D9.FVF = D_D3DFVF_XYZ | D_D3DFVF_NORMAL | D_D3DFVF_DIFFUSE | D_D3DFVF_SPECULAR | TexOrParam ;
							if( Direct3DDevice9_CreateVertexBuffer_ASync(
									TVBuf->UnitSize * TVBuf->VertexCount,
									D_D3DUSAGE_WRITEONLY,
									TVBuf->PF->D3D9.FVF,
									D_D3DPOOL_DEFAULT,
									&TVBuf->PF->D3D9.VertexBuffer,
									ASyncThread
								) < 0 )
								goto ERR ;

							// ���b�N
							if( Direct3DVertexBuffer9_Lock_ASync( TVBuf->PF->D3D9.VertexBuffer, 0, 0, &TVBuf->VertexBufferL, 0, ASyncThread ) != D_D3D_OK )
								goto ERR ;

							// �C���f�b�N�X�o�b�t�@���쐬
							if( Direct3DDevice9_CreateIndexBuffer_ASync(
									sizeof( WORD ) * TVBuf->IndexCount,
									D_D3DUSAGE_WRITEONLY,
									D_D3DFMT_INDEX16,
									D_D3DPOOL_DEFAULT,
									&TVBuf->PF->D3D9.IndexBuffer,
									ASyncThread ) < 0 )
								goto ERR ;

							// ���b�N
							if( Direct3DIndexBuffer9_Lock_ASync( TVBuf->PF->D3D9.IndexBuffer, 0, 0, &TVBuf->IndexBufferL, 0, ASyncThread ) != D_D3D_OK )
								goto ERR ;
						}
						else
						{
							// �o���v�}�b�v�t�����̃��b�V���̏ꍇ

							// ���_�V�F�[�_�[���g�p�ł��Ȃ��ꍇ�͍쐬���Ȃ�
							if( GSYS.HardInfo.UseShader == FALSE ) break ; 

							// ���_�o�b�t�@���쐬
							TVBuf->UnitSize = sizeof( MV1_VERTEX_SIMPLE_BUMP_D3D9 ) + ( k - 1 ) * sizeof( float ) * 4 ;
							TVBuf->PF->D3D9.FVF = 0 ;
							if( Direct3DDevice9_CreateVertexBuffer_ASync(
									TVBuf->UnitSize * TVBuf->VertexCount,
									D_D3DUSAGE_WRITEONLY,
									0,
									D_D3DPOOL_DEFAULT,
									&TVBuf->PF->D3D9.VertexBuffer,
									ASyncThread
								) < 0 )
								goto ERR ;

							// ���b�N
							if( Direct3DVertexBuffer9_Lock_ASync( TVBuf->PF->D3D9.VertexBuffer, 0, 0, &TVBuf->VertexBufferL, 0, ASyncThread ) != D_D3D_OK )
								goto ERR ;

							// �C���f�b�N�X�o�b�t�@���쐬
							if( Direct3DDevice9_CreateIndexBuffer_ASync(
									sizeof( WORD ) * TVBuf->IndexCount,
									D_D3DUSAGE_WRITEONLY,
									D_D3DFMT_INDEX16,
									D_D3DPOOL_DEFAULT,
									&TVBuf->PF->D3D9.IndexBuffer,
									ASyncThread ) < 0 )
								goto ERR ;

							// ���b�N
							if( Direct3DIndexBuffer9_Lock_ASync( TVBuf->PF->D3D9.IndexBuffer, 0, 0, &TVBuf->IndexBufferL, 0, ASyncThread ) != D_D3D_OK )
								goto ERR ;
						}
						break ;

					case 1 :
						// �S�{�[���ȓ��̃X�L�j���O���b�V���̏ꍇ

						if( j == 0 )
						{
							// ���_�V�F�[�_�[���g�p�ł��Ȃ��ꍇ�͍쐬���Ȃ�
							if( GSYS.HardInfo.UseShader == FALSE ) break ; 

							// ���_�o�b�t�@���쐬
							TVBuf->UnitSize = sizeof( MV1_VERTEX_SKIN_B4_D3D9 ) + ( k - 1 ) * sizeof( float ) * 4 ;
							TVBuf->PF->D3D9.FVF = 0 ;
							if( Direct3DDevice9_CreateVertexBuffer_ASync(
									TVBuf->UnitSize * TVBuf->VertexCount,
									D_D3DUSAGE_WRITEONLY,
									0,
									D_D3DPOOL_DEFAULT,
									&TVBuf->PF->D3D9.VertexBuffer,
									ASyncThread
								) < 0 )
								goto ERR ;

							// ���b�N
							if( Direct3DVertexBuffer9_Lock_ASync( TVBuf->PF->D3D9.VertexBuffer, 0, 0, &TVBuf->VertexBufferL, 0, ASyncThread ) != D_D3D_OK )
								goto ERR ;

							// �C���f�b�N�X�o�b�t�@���쐬
							if( Direct3DDevice9_CreateIndexBuffer_ASync(
									sizeof( WORD ) * TVBuf->IndexCount,
									D_D3DUSAGE_WRITEONLY,
									D_D3DFMT_INDEX16,
									D_D3DPOOL_DEFAULT,
									&TVBuf->PF->D3D9.IndexBuffer,
									ASyncThread ) < 0 )
								goto ERR ;

							// ���b�N
							if( Direct3DIndexBuffer9_Lock_ASync( TVBuf->PF->D3D9.IndexBuffer, 0, 0, &TVBuf->IndexBufferL, 0, ASyncThread ) != D_D3D_OK )
								goto ERR ;
						}
						else
						{
							// �o���v�}�b�v�t���S�{�[���ȓ��̃X�L�j���O���b�V���̏ꍇ

							// ���_�V�F�[�_�[���g�p�ł��Ȃ��ꍇ�͍쐬���Ȃ�
							if( GSYS.HardInfo.UseShader == FALSE ) break ; 

							// ���_�o�b�t�@���쐬
							TVBuf->UnitSize = sizeof( MV1_VERTEX_SKIN_B4_BUMP_D3D9 ) + ( k - 1 ) * sizeof( float ) * 4 ;
							TVBuf->PF->D3D9.FVF = 0 ;
							if( Direct3DDevice9_CreateVertexBuffer_ASync(
									TVBuf->UnitSize * TVBuf->VertexCount,
									D_D3DUSAGE_WRITEONLY,
									0,
									D_D3DPOOL_DEFAULT,
									&TVBuf->PF->D3D9.VertexBuffer,
									ASyncThread
								) < 0 )
								goto ERR ;

							// ���b�N
							if( Direct3DVertexBuffer9_Lock_ASync( TVBuf->PF->D3D9.VertexBuffer, 0, 0, &TVBuf->VertexBufferL, 0, ASyncThread ) != D_D3D_OK )
								goto ERR ;

							// �C���f�b�N�X�o�b�t�@���쐬
							if( Direct3DDevice9_CreateIndexBuffer_ASync(
									sizeof( WORD ) * TVBuf->IndexCount,
									D_D3DUSAGE_WRITEONLY,
									D_D3DFMT_INDEX16,
									D_D3DPOOL_DEFAULT,
									&TVBuf->PF->D3D9.IndexBuffer,
									ASyncThread ) < 0 )
								goto ERR ;

							// ���b�N
							if( Direct3DIndexBuffer9_Lock_ASync( TVBuf->PF->D3D9.IndexBuffer, 0, 0, &TVBuf->IndexBufferL, 0, ASyncThread ) != D_D3D_OK )
								goto ERR ;
						}
						break ;


					case 2 :
						// �W�{�[���ȓ��̃X�L�j���O���b�V���̏ꍇ

						if( j == 0 )
						{
							// ���_�V�F�[�_�[���g�p�ł��Ȃ��ꍇ�͍쐬���Ȃ�
							if( GSYS.HardInfo.UseShader == FALSE ) break ; 

							// ���_�o�b�t�@���쐬
							TVBuf->UnitSize = sizeof( MV1_VERTEX_SKIN_B8_D3D9 ) + ( k - 1 ) * sizeof( float ) * 4 ;
							TVBuf->PF->D3D9.FVF = 0 ;
							if( Direct3DDevice9_CreateVertexBuffer_ASync(
									TVBuf->UnitSize * TVBuf->VertexCount,
									D_D3DUSAGE_WRITEONLY,
									0,
									D_D3DPOOL_DEFAULT,
									&TVBuf->PF->D3D9.VertexBuffer,
									ASyncThread
								) < 0 )
								goto ERR ;

							// ���b�N
							if( Direct3DVertexBuffer9_Lock_ASync( TVBuf->PF->D3D9.VertexBuffer, 0, 0, &TVBuf->VertexBufferL, 0, ASyncThread ) != D_D3D_OK )
								goto ERR ;

							// �C���f�b�N�X�o�b�t�@���쐬
							if( Direct3DDevice9_CreateIndexBuffer_ASync(
									sizeof( WORD ) * TVBuf->IndexCount,
									D_D3DUSAGE_WRITEONLY,
									D_D3DFMT_INDEX16,
									D_D3DPOOL_DEFAULT,
									&TVBuf->PF->D3D9.IndexBuffer,
									ASyncThread ) < 0 )
								goto ERR ;

							// ���b�N
							if( Direct3DIndexBuffer9_Lock_ASync( TVBuf->PF->D3D9.IndexBuffer, 0, 0, &TVBuf->IndexBufferL, 0, ASyncThread ) != D_D3D_OK )
								goto ERR ;
						}
						else
						{
							// �o���v�}�b�v�t���S�{�[���ȓ��̃X�L�j���O���b�V���̏ꍇ

							// ���_�V�F�[�_�[���g�p�ł��Ȃ��ꍇ�͍쐬���Ȃ�
							if( GSYS.HardInfo.UseShader == FALSE ) break ; 

							// ���_�o�b�t�@���쐬
							TVBuf->UnitSize = sizeof( MV1_VERTEX_SKIN_B8_BUMP_D3D9 ) + ( k - 1 ) * sizeof( float ) * 4 ;
							TVBuf->PF->D3D9.FVF = 0 ;
							if( Direct3DDevice9_CreateVertexBuffer_ASync(
									TVBuf->UnitSize * TVBuf->VertexCount,
									D_D3DUSAGE_WRITEONLY,
									0,
									D_D3DPOOL_DEFAULT,
									&TVBuf->PF->D3D9.VertexBuffer,
									ASyncThread
								) < 0 )
								goto ERR ;

							// ���b�N
							if( Direct3DVertexBuffer9_Lock_ASync( TVBuf->PF->D3D9.VertexBuffer, 0, 0, &TVBuf->VertexBufferL, 0, ASyncThread ) != D_D3D_OK )
								goto ERR ;

							// �C���f�b�N�X�o�b�t�@���쐬
							if( Direct3DDevice9_CreateIndexBuffer_ASync(
									sizeof( WORD ) * TVBuf->IndexCount,
									D_D3DUSAGE_WRITEONLY,
									D_D3DFMT_INDEX16,
									D_D3DPOOL_DEFAULT,
									&TVBuf->PF->D3D9.IndexBuffer,
									ASyncThread ) < 0 )
								goto ERR ;

							// ���b�N
							if( Direct3DIndexBuffer9_Lock_ASync( TVBuf->PF->D3D9.IndexBuffer, 0, 0, &TVBuf->IndexBufferL, 0, ASyncThread ) != D_D3D_OK )
								goto ERR ;
						}
						break ;
					}
				}
			}
		}
	}

	// ���_�o�b�t�@�Ƀf�[�^���i�[����
	MBTList = MBase->TriangleList ;
	for( i = 0 ; i < MBase->TriangleListNum ; i ++, MBTList ++ )
	{
		if( MBTList->VertexBuffer == NULL ||
			MBTList->VertexBuffer->PF->D3D9.VertexBuffer == NULL ||
			MBTList->VertexBuffer->PF->D3D9.IndexBuffer == NULL
			) continue ;

		// �o���v�}�b�v�̗L�����Z�b�g
		Bump = MBTList->Container->Material->NormalLayerNum ? 1 : 0 ;

		// �g�p���钸�_�o�b�t�@���Z�b�g
		TVBuf = MBTList->VertexBuffer ;

		// ���_�^�C�v�ɂ���ď����𕪊�
		MVInd        = MBTList->MeshVertexIndex ;
		VertexNum    = MBTList->VertexNum ;
		UVNum        = MBTList->Container->UVSetUnitNum ;
		MeshNorm     = MBTList->Container->Container->Normal ;
		MeshVertSize = MBTList->Container->VertUnitSize ;
		MeshVert     = MBTList->Container->Vertex ;
		switch( MBTList->VertexType )
		{
		case MV1_VERTEX_TYPE_NORMAL :
			if( MBase->UsePackDraw )
			{
				if( Bump )
				{
					MV1_VERTEX_SKIN_B4_BUMP_D3D9 *Dest, *DestBase ;
					MV1_TLIST_NORMAL_POS *Src ;

					Src      = ( MV1_TLIST_NORMAL_POS * )ADDR16( MBTList->NormalPosition ) ;
					DestBase = ( MV1_VERTEX_SKIN_B4_BUMP_D3D9 * )( ( BYTE * )TVBuf->VertexBufferL + MBTList->VBStartVertex * MBTList->VertexBuffer->UnitSize ) ;
					Dest     = DestBase ;
					for( j = 0 ; j < VertexNum ; j ++, Dest = ( MV1_VERTEX_SKIN_B4_BUMP_D3D9 * )( ( BYTE * )Dest + MBTList->VertexBuffer->UnitSize ), Src ++, MVInd ++ )
					{
						MVert = ( MV1_MESH_VERTEX   * )( ( BYTE * )MeshVert + MeshVertSize * *MVInd ) ;
						MNorm = MeshNorm + MVert->NormalIndex ;

						Dest->Position      = *( ( VECTOR * )&Src->Position ) ;
						Dest->MatrixIndex[ 0 ]  = 0 ;
						Dest->MatrixIndex[ 1 ]  = 0 ;
						Dest->MatrixIndex[ 2 ]  = 0 ;
						Dest->MatrixIndex[ 3 ]  = 0 ;
						Dest->MatrixWeight[ 0 ] = 1.0f ;
						Dest->MatrixWeight[ 1 ] = 0.0f ;
						Dest->MatrixWeight[ 2 ] = 0.0f ;
						Dest->MatrixWeight[ 3 ] = 0.0f ;
						Dest->Normal        = MNorm->Normal ;
						Dest->Binormal      = MNorm->Binormal ;
						Dest->Tangent       = MNorm->Tangent ;
						Dest->DiffuseColor  = MVert->DiffuseColor ;
						Dest->SpecularColor = MVert->SpecularColor ;

						for( k = 0 ; k < UVNum ; k ++ )
						{
							Dest->UVs[ k ][ 0 ] = MVert->UVs[ k ][ 0 ] ;
							Dest->UVs[ k ][ 1 ] = MVert->UVs[ k ][ 1 ] ;
							Dest->UVs[ k ][ 2 ] = 1.0f ;
							Dest->UVs[ k ][ 3 ] = 1.0f ;
						}
					}

					BaseMatrixIndex = 1 * 3 ;
					for( k = 1 ; k < MBTList->PackDrawMaxNum ; k ++, BaseMatrixIndex += 1 * 3 )
					{
						_MEMCPY( Dest, DestBase, MBTList->VertexBuffer->UnitSize * VertexNum ) ;
						for( j = 0 ; j < VertexNum ; j ++, Dest = ( MV1_VERTEX_SKIN_B4_BUMP_D3D9 * )( ( BYTE * )Dest + MBTList->VertexBuffer->UnitSize ) )
						{
							Dest->MatrixIndex[ 0 ] = ( BYTE )BaseMatrixIndex ;
						}
					}
				}
				else
				{
					MV1_VERTEX_SKIN_B4_D3D9 *Dest, *DestBase ;
					MV1_TLIST_NORMAL_POS *Src ;

					Src      = ( MV1_TLIST_NORMAL_POS * )ADDR16( MBTList->NormalPosition ) ;
					DestBase = ( MV1_VERTEX_SKIN_B4_D3D9 * )( ( BYTE * )TVBuf->VertexBufferL + MBTList->VBStartVertex * MBTList->VertexBuffer->UnitSize ) ;
					Dest     = DestBase ;
					for( j = 0 ; j < VertexNum ; j ++, Dest = ( MV1_VERTEX_SKIN_B4_D3D9 * )( ( BYTE * )Dest + MBTList->VertexBuffer->UnitSize ), Src ++, MVInd ++ )
					{
						MVert = ( MV1_MESH_VERTEX   * )( ( BYTE * )MeshVert + MeshVertSize * *MVInd ) ;
						MNorm = MeshNorm + MVert->NormalIndex ;

						Dest->Position      = *( ( VECTOR * )&Src->Position ) ;
						Dest->MatrixIndex[ 0 ]  = 0 ;
						Dest->MatrixIndex[ 1 ]  = 0 ;
						Dest->MatrixIndex[ 2 ]  = 0 ;
						Dest->MatrixIndex[ 3 ]  = 0 ;
						Dest->MatrixWeight[ 0 ] = 1.0f ;
						Dest->MatrixWeight[ 1 ] = 0.0f ;
						Dest->MatrixWeight[ 2 ] = 0.0f ;
						Dest->MatrixWeight[ 3 ] = 0.0f ;
						Dest->Normal        = MNorm->Normal ;
						Dest->DiffuseColor  = MVert->DiffuseColor ;
						Dest->SpecularColor = MVert->SpecularColor ;

						for( k = 0 ; k < UVNum ; k ++ )
						{
							Dest->UVs[ k ][ 0 ] = MVert->UVs[ k ][ 0 ] ;
							Dest->UVs[ k ][ 1 ] = MVert->UVs[ k ][ 1 ] ;
							Dest->UVs[ k ][ 2 ] = 1.0f ;
							Dest->UVs[ k ][ 3 ] = 1.0f ;
						}
					}

					BaseMatrixIndex = 1 * 3 ;
					for( k = 1 ; k < MBTList->PackDrawMaxNum ; k ++, BaseMatrixIndex += 1 * 3 )
					{
						_MEMCPY( Dest, DestBase, MBTList->VertexBuffer->UnitSize * VertexNum ) ;
						for( j = 0 ; j < VertexNum ; j ++, Dest = ( MV1_VERTEX_SKIN_B4_D3D9 * )( ( BYTE * )Dest + MBTList->VertexBuffer->UnitSize ) )
						{
							Dest->MatrixIndex[ 0 ] = ( BYTE )BaseMatrixIndex ;
						}
					}
				}
			}
			else
			{
				if( Bump )
				{
					MV1_VERTEX_SIMPLE_BUMP_D3D9 *Dest ;
					MV1_TLIST_NORMAL_POS *Src ;

					Src = ( MV1_TLIST_NORMAL_POS * )ADDR16( MBTList->NormalPosition ) ;
					Dest = ( MV1_VERTEX_SIMPLE_BUMP_D3D9 * )( ( BYTE * )TVBuf->VertexBufferL + MBTList->VBStartVertex * MBTList->VertexBuffer->UnitSize ) ;
					for( j = 0 ; j < VertexNum ; j ++, Dest = ( MV1_VERTEX_SIMPLE_BUMP_D3D9 * )( ( BYTE * )Dest + MBTList->VertexBuffer->UnitSize ), Src ++, MVInd ++ )
					{
						MVert = ( MV1_MESH_VERTEX   * )( ( BYTE * )MeshVert + MeshVertSize * *MVInd ) ;
						MNorm = MeshNorm + MVert->NormalIndex ;

						Dest->Position      = *( ( VECTOR * )&Src->Position ) ;
						Dest->Normal        = MNorm->Normal ;
						Dest->Binormal      = MNorm->Binormal ;
						Dest->Tangent       = MNorm->Tangent ;
						Dest->DiffuseColor  = MVert->DiffuseColor ;
						Dest->SpecularColor = MVert->SpecularColor ;

						for( k = 0 ; k < UVNum ; k ++ )
						{
							Dest->UVs[ k ][ 0 ] = MVert->UVs[ k ][ 0 ] ;
							Dest->UVs[ k ][ 1 ] = MVert->UVs[ k ][ 1 ] ;
							Dest->UVs[ k ][ 2 ] = 1.0f ;
							Dest->UVs[ k ][ 3 ] = 1.0f ;
						}
					}
				}
				else
				{
					MV1_VERTEX_SIMPLE_D3D9 *Dest ;
					MV1_TLIST_NORMAL_POS *Src ;

					Src = ( MV1_TLIST_NORMAL_POS * )ADDR16( MBTList->NormalPosition ) ;
					Dest = ( MV1_VERTEX_SIMPLE_D3D9 * )( ( BYTE * )TVBuf->VertexBufferL + MBTList->VBStartVertex * MBTList->VertexBuffer->UnitSize ) ;
					for( j = 0 ; j < VertexNum ; j ++, Dest = ( MV1_VERTEX_SIMPLE_D3D9 * )( ( BYTE * )Dest + MBTList->VertexBuffer->UnitSize ), Src ++, MVInd ++ )
					{
						MVert = ( MV1_MESH_VERTEX   * )( ( BYTE * )MeshVert + MeshVertSize * *MVInd ) ;
						MNorm = MeshNorm + MVert->NormalIndex ;

						Dest->Position      = *( ( VECTOR * )&Src->Position ) ;
						Dest->Normal        = MNorm->Normal ;
						Dest->DiffuseColor  = MVert->DiffuseColor ;
						Dest->SpecularColor = MVert->SpecularColor ;

						for( k = 0 ; k < UVNum ; k ++ )
						{
							Dest->UVs[ k ][ 0 ] = MVert->UVs[ k ][ 0 ] ;
							Dest->UVs[ k ][ 1 ] = MVert->UVs[ k ][ 1 ] ;
							Dest->UVs[ k ][ 2 ] = 1.0f ;
							Dest->UVs[ k ][ 3 ] = 1.0f ;
						}
					}
				}
			}
			break ;

		case MV1_VERTEX_TYPE_SKIN_4BONE :
			if( Bump )
			{
				MV1_VERTEX_SKIN_B4_BUMP_D3D9 *Dest, *DestBase ;
				MV1_TLIST_SKIN_POS_4B *Src ;

				Src      = ( MV1_TLIST_SKIN_POS_4B * )ADDR16( MBTList->SkinPosition4B ) ;
				DestBase = ( MV1_VERTEX_SKIN_B4_BUMP_D3D9 * )( ( BYTE * )TVBuf->VertexBufferL + MBTList->VBStartVertex * MBTList->VertexBuffer->UnitSize ) ;
				Dest     = DestBase ;
				for( j = 0 ; j < VertexNum ; j ++, Dest = ( MV1_VERTEX_SKIN_B4_BUMP_D3D9 * )( ( BYTE * )Dest + MBTList->VertexBuffer->UnitSize ), Src ++, MVInd ++ )
				{
					MVert = ( MV1_MESH_VERTEX   * )( ( BYTE * )MeshVert + MeshVertSize * *MVInd ) ;
					MNorm = MeshNorm + MVert->NormalIndex ;

					Dest->Position          = *( ( VECTOR * )&Src->Position ) ;
					Dest->MatrixIndex[ 0 ]  = ( BYTE )( Src->MatrixIndex[ 0 ] * 3 ) ;
					Dest->MatrixIndex[ 1 ]  = ( BYTE )( Src->MatrixIndex[ 1 ] * 3 ) ;
					Dest->MatrixIndex[ 2 ]  = ( BYTE )( Src->MatrixIndex[ 2 ] * 3 ) ;
					Dest->MatrixIndex[ 3 ]  = ( BYTE )( Src->MatrixIndex[ 3 ] * 3 ) ;
					Dest->MatrixWeight[ 0 ] = Src->MatrixWeight [ 0 ] ;
					Dest->MatrixWeight[ 1 ] = Src->MatrixWeight [ 1 ] ;
					Dest->MatrixWeight[ 2 ] = Src->MatrixWeight [ 2 ] ;
					Dest->MatrixWeight[ 3 ] = Src->MatrixWeight [ 3 ] ;
					Dest->Normal        = MNorm->Normal ;
					Dest->Binormal      = MNorm->Binormal ;
					Dest->Tangent       = MNorm->Tangent ;
					Dest->DiffuseColor  = MVert->DiffuseColor ;
					Dest->SpecularColor = MVert->SpecularColor ;

					for( k = 0 ; k < UVNum ; k ++ )
					{
						Dest->UVs[ k ][ 0 ] = MVert->UVs[ k ][ 0 ] ;
						Dest->UVs[ k ][ 1 ] = MVert->UVs[ k ][ 1 ] ;
						Dest->UVs[ k ][ 2 ] = 1.0f ;
						Dest->UVs[ k ][ 3 ] = 1.0f ;
					}
				}

				if( MBase->UsePackDraw )
				{
					BaseMatrixIndex = MBTList->UseBoneNum * 3 ;
					for( k = 1 ; k < MBTList->PackDrawMaxNum ; k ++, BaseMatrixIndex += MBTList->UseBoneNum * 3 )
					{
						_MEMCPY( Dest, DestBase, MBTList->VertexBuffer->UnitSize * VertexNum ) ;
						for( j = 0 ; j < VertexNum ; j ++, Dest = ( MV1_VERTEX_SKIN_B4_BUMP_D3D9 * )( ( BYTE * )Dest + MBTList->VertexBuffer->UnitSize ) )
						{
							Dest->MatrixIndex[ 0 ]  += ( BYTE )BaseMatrixIndex ;
							Dest->MatrixIndex[ 1 ]  += ( BYTE )BaseMatrixIndex ;
							Dest->MatrixIndex[ 2 ]  += ( BYTE )BaseMatrixIndex ;
							Dest->MatrixIndex[ 3 ]  += ( BYTE )BaseMatrixIndex ;
						}
					}
				}
			}
			else
			{
				MV1_VERTEX_SKIN_B4_D3D9 *Dest, *DestBase ;
				MV1_TLIST_SKIN_POS_4B *Src ;

				Src      = ( MV1_TLIST_SKIN_POS_4B * )ADDR16( MBTList->SkinPosition4B ) ;
				DestBase = ( MV1_VERTEX_SKIN_B4_D3D9 * )( ( BYTE * )TVBuf->VertexBufferL + MBTList->VBStartVertex * MBTList->VertexBuffer->UnitSize ) ;
				Dest     = DestBase ;
				for( j = 0 ; j < VertexNum ; j ++, Dest = ( MV1_VERTEX_SKIN_B4_D3D9 * )( ( BYTE * )Dest + MBTList->VertexBuffer->UnitSize ), Src ++, MVInd ++ )
				{
					MVert = ( MV1_MESH_VERTEX   * )( ( BYTE * )MeshVert + MeshVertSize * *MVInd ) ;
					MNorm = MeshNorm + MVert->NormalIndex ;

					Dest->Position          = *( ( VECTOR * )&Src->Position ) ;
					Dest->MatrixIndex[ 0 ]  = ( BYTE )( Src->MatrixIndex[ 0 ] * 3 ) ;
					Dest->MatrixIndex[ 1 ]  = ( BYTE )( Src->MatrixIndex[ 1 ] * 3 ) ;
					Dest->MatrixIndex[ 2 ]  = ( BYTE )( Src->MatrixIndex[ 2 ] * 3 ) ;
					Dest->MatrixIndex[ 3 ]  = ( BYTE )( Src->MatrixIndex[ 3 ] * 3 ) ;
					Dest->MatrixWeight[ 0 ] = Src->MatrixWeight [ 0 ] ;
					Dest->MatrixWeight[ 1 ] = Src->MatrixWeight [ 1 ] ;
					Dest->MatrixWeight[ 2 ] = Src->MatrixWeight [ 2 ] ;
					Dest->MatrixWeight[ 3 ] = Src->MatrixWeight [ 3 ] ;
					Dest->Normal        = MNorm->Normal ;
					Dest->DiffuseColor  = MVert->DiffuseColor ;
					Dest->SpecularColor = MVert->SpecularColor ;

					for( k = 0 ; k < UVNum ; k ++ )
					{
						Dest->UVs[ k ][ 0 ] = MVert->UVs[ k ][ 0 ] ;
						Dest->UVs[ k ][ 1 ] = MVert->UVs[ k ][ 1 ] ;
						Dest->UVs[ k ][ 2 ] = 1.0f ;
						Dest->UVs[ k ][ 3 ] = 1.0f ;
					}
				}

				if( MBase->UsePackDraw )
				{
					BaseMatrixIndex = MBTList->UseBoneNum * 3 ;
					for( k = 1 ; k < MBTList->PackDrawMaxNum ; k ++, BaseMatrixIndex += MBTList->UseBoneNum * 3 )
					{
						_MEMCPY( Dest, DestBase, MBTList->VertexBuffer->UnitSize * VertexNum ) ;
						for( j = 0 ; j < VertexNum ; j ++, Dest = ( MV1_VERTEX_SKIN_B4_D3D9 * )( ( BYTE * )Dest + MBTList->VertexBuffer->UnitSize ) )
						{
							Dest->MatrixIndex[ 0 ]  += ( BYTE )BaseMatrixIndex ;
							Dest->MatrixIndex[ 1 ]  += ( BYTE )BaseMatrixIndex ;
							Dest->MatrixIndex[ 2 ]  += ( BYTE )BaseMatrixIndex ;
							Dest->MatrixIndex[ 3 ]  += ( BYTE )BaseMatrixIndex ;
						}
					}
				}
			}
			break ;

		case MV1_VERTEX_TYPE_SKIN_8BONE :
			if( Bump )
			{
				MV1_VERTEX_SKIN_B8_BUMP_D3D9 *Dest, *DestBase ;
				MV1_TLIST_SKIN_POS_8B *Src ;

				Src      = ( MV1_TLIST_SKIN_POS_8B * )ADDR16( MBTList->SkinPosition8B ) ;
				DestBase = ( MV1_VERTEX_SKIN_B8_BUMP_D3D9 * )( ( BYTE * )TVBuf->VertexBufferL + MBTList->VBStartVertex * MBTList->VertexBuffer->UnitSize ) ;
				Dest     = DestBase ;
				for( j = 0 ; j < VertexNum ; j ++, Dest = ( MV1_VERTEX_SKIN_B8_BUMP_D3D9 * )( ( BYTE * )Dest + MBTList->VertexBuffer->UnitSize ), Src ++, MVInd ++ )
				{
					MVert = ( MV1_MESH_VERTEX   * )( ( BYTE * )MeshVert + MeshVertSize * *MVInd ) ;
					MNorm = MeshNorm + MVert->NormalIndex ;

					Dest->Position          = *( ( VECTOR * )&Src->Position ) ;
					Dest->MatrixIndex[ 0 ]  = ( BYTE )( Src->MatrixIndex1[ 0 ] * 3 ) ;
					Dest->MatrixIndex[ 1 ]  = ( BYTE )( Src->MatrixIndex1[ 1 ] * 3 ) ;
					Dest->MatrixIndex[ 2 ]  = ( BYTE )( Src->MatrixIndex1[ 2 ] * 3 ) ;
					Dest->MatrixIndex[ 3 ]  = ( BYTE )( Src->MatrixIndex1[ 3 ] * 3 ) ;
					Dest->MatrixIndex[ 4 ]  = ( BYTE )( Src->MatrixIndex2[ 0 ] * 3 ) ;
					Dest->MatrixIndex[ 5 ]  = ( BYTE )( Src->MatrixIndex2[ 1 ] * 3 ) ;
					Dest->MatrixIndex[ 6 ]  = ( BYTE )( Src->MatrixIndex2[ 2 ] * 3 ) ;
					Dest->MatrixIndex[ 7 ]  = ( BYTE )( Src->MatrixIndex2[ 3 ] * 3 ) ;
					Dest->MatrixWeight[ 0 ] = Src->MatrixWeight [ 0 ] ;
					Dest->MatrixWeight[ 1 ] = Src->MatrixWeight [ 1 ] ;
					Dest->MatrixWeight[ 2 ] = Src->MatrixWeight [ 2 ] ;
					Dest->MatrixWeight[ 3 ] = Src->MatrixWeight [ 3 ] ;
					Dest->MatrixWeight[ 4 ] = Src->MatrixWeight [ 4 ] ;
					Dest->MatrixWeight[ 5 ] = Src->MatrixWeight [ 5 ] ;
					Dest->MatrixWeight[ 6 ] = Src->MatrixWeight [ 6 ] ;
					Dest->MatrixWeight[ 7 ] = Src->MatrixWeight [ 7 ] ;
					Dest->Normal        = MNorm->Normal ;
					Dest->Binormal      = MNorm->Binormal ;
					Dest->Tangent       = MNorm->Tangent ;
					Dest->DiffuseColor  = MVert->DiffuseColor ;
					Dest->SpecularColor = MVert->SpecularColor ;

					for( k = 0 ; k < UVNum ; k ++ )
					{
						Dest->UVs[ k ][ 0 ] = MVert->UVs[ k ][ 0 ] ;
						Dest->UVs[ k ][ 1 ] = MVert->UVs[ k ][ 1 ] ;
						Dest->UVs[ k ][ 2 ] = 1.0f ;
						Dest->UVs[ k ][ 3 ] = 1.0f ;
					}
				}

				if( MBase->UsePackDraw )
				{
					BaseMatrixIndex = MBTList->UseBoneNum * 3 ;
					for( k = 1 ; k < MBTList->PackDrawMaxNum ; k ++, BaseMatrixIndex += MBTList->UseBoneNum * 3 )
					{
						_MEMCPY( Dest, DestBase, MBTList->VertexBuffer->UnitSize * VertexNum ) ;
						for( j = 0 ; j < VertexNum ; j ++, Dest = ( MV1_VERTEX_SKIN_B8_BUMP_D3D9 * )( ( BYTE * )Dest + MBTList->VertexBuffer->UnitSize ) )
						{
							Dest->MatrixIndex[ 0 ]  += ( BYTE )BaseMatrixIndex ;
							Dest->MatrixIndex[ 1 ]  += ( BYTE )BaseMatrixIndex ;
							Dest->MatrixIndex[ 2 ]  += ( BYTE )BaseMatrixIndex ;
							Dest->MatrixIndex[ 3 ]  += ( BYTE )BaseMatrixIndex ;
							Dest->MatrixIndex[ 4 ]  += ( BYTE )BaseMatrixIndex ;
							Dest->MatrixIndex[ 5 ]  += ( BYTE )BaseMatrixIndex ;
							Dest->MatrixIndex[ 6 ]  += ( BYTE )BaseMatrixIndex ;
							Dest->MatrixIndex[ 7 ]  += ( BYTE )BaseMatrixIndex ;
						}
					}
				}
			}
			else
			{
				MV1_VERTEX_SKIN_B8_D3D9 *Dest, *DestBase ;
				MV1_TLIST_SKIN_POS_8B *Src ;

				Src      = ( MV1_TLIST_SKIN_POS_8B * )ADDR16( MBTList->SkinPosition8B ) ;
				DestBase = ( MV1_VERTEX_SKIN_B8_D3D9 * )( ( BYTE * )TVBuf->VertexBufferL + MBTList->VBStartVertex * MBTList->VertexBuffer->UnitSize ) ;
				Dest     = DestBase ;
				for( j = 0 ; j < VertexNum ; j ++, Dest = ( MV1_VERTEX_SKIN_B8_D3D9 * )( ( BYTE * )Dest + MBTList->VertexBuffer->UnitSize ), Src ++, MVInd ++ )
				{
					MVert = ( MV1_MESH_VERTEX   * )( ( BYTE * )MeshVert + MeshVertSize * *MVInd ) ;
					MNorm = MeshNorm + MVert->NormalIndex ;

					Dest->Position          = *( ( VECTOR * )&Src->Position ) ;
					Dest->MatrixIndex[ 0 ]  = ( BYTE )( Src->MatrixIndex1[ 0 ] * 3 ) ;
					Dest->MatrixIndex[ 1 ]  = ( BYTE )( Src->MatrixIndex1[ 1 ] * 3 ) ;
					Dest->MatrixIndex[ 2 ]  = ( BYTE )( Src->MatrixIndex1[ 2 ] * 3 ) ;
					Dest->MatrixIndex[ 3 ]  = ( BYTE )( Src->MatrixIndex1[ 3 ] * 3 ) ;
					Dest->MatrixIndex[ 4 ]  = ( BYTE )( Src->MatrixIndex2[ 0 ] * 3 ) ;
					Dest->MatrixIndex[ 5 ]  = ( BYTE )( Src->MatrixIndex2[ 1 ] * 3 ) ;
					Dest->MatrixIndex[ 6 ]  = ( BYTE )( Src->MatrixIndex2[ 2 ] * 3 ) ;
					Dest->MatrixIndex[ 7 ]  = ( BYTE )( Src->MatrixIndex2[ 3 ] * 3 ) ;
					Dest->MatrixWeight[ 0 ] = Src->MatrixWeight [ 0 ] ;
					Dest->MatrixWeight[ 1 ] = Src->MatrixWeight [ 1 ] ;
					Dest->MatrixWeight[ 2 ] = Src->MatrixWeight [ 2 ] ;
					Dest->MatrixWeight[ 3 ] = Src->MatrixWeight [ 3 ] ;
					Dest->MatrixWeight[ 4 ] = Src->MatrixWeight [ 4 ] ;
					Dest->MatrixWeight[ 5 ] = Src->MatrixWeight [ 5 ] ;
					Dest->MatrixWeight[ 6 ] = Src->MatrixWeight [ 6 ] ;
					Dest->MatrixWeight[ 7 ] = Src->MatrixWeight [ 7 ] ;
					Dest->Normal        = MNorm->Normal ;
					Dest->DiffuseColor  = MVert->DiffuseColor ;
					Dest->SpecularColor = MVert->SpecularColor ;

					for( k = 0 ; k < UVNum ; k ++ )
					{
						Dest->UVs[ k ][ 0 ] = MVert->UVs[ k ][ 0 ] ;
						Dest->UVs[ k ][ 1 ] = MVert->UVs[ k ][ 1 ] ;
						Dest->UVs[ k ][ 2 ] = 1.0f ;
						Dest->UVs[ k ][ 3 ] = 1.0f ;
					}
				}

				if( MBase->UsePackDraw )
				{
					BaseMatrixIndex = MBTList->UseBoneNum * 3 ;
					for( k = 1 ; k < MBTList->PackDrawMaxNum ; k ++, BaseMatrixIndex += MBTList->UseBoneNum * 3 )
					{
						_MEMCPY( Dest, DestBase, MBTList->VertexBuffer->UnitSize * VertexNum ) ;
						for( j = 0 ; j < VertexNum ; j ++, Dest = ( MV1_VERTEX_SKIN_B8_D3D9 * )( ( BYTE * )Dest + MBTList->VertexBuffer->UnitSize ) )
						{
							Dest->MatrixIndex[ 0 ]  += ( BYTE )BaseMatrixIndex ;
							Dest->MatrixIndex[ 1 ]  += ( BYTE )BaseMatrixIndex ;
							Dest->MatrixIndex[ 2 ]  += ( BYTE )BaseMatrixIndex ;
							Dest->MatrixIndex[ 3 ]  += ( BYTE )BaseMatrixIndex ;
							Dest->MatrixIndex[ 4 ]  += ( BYTE )BaseMatrixIndex ;
							Dest->MatrixIndex[ 5 ]  += ( BYTE )BaseMatrixIndex ;
							Dest->MatrixIndex[ 6 ]  += ( BYTE )BaseMatrixIndex ;
							Dest->MatrixIndex[ 7 ]  += ( BYTE )BaseMatrixIndex ;
						}
					}
				}
			}
			break ;
		}

		// �C���f�b�N�X�f�[�^�̏�������
		{
			WORD *Dest, *ToonDest ;
			int DestBaseIndex ;
			int ToonDestBaseIndex ;

			Dest     = ( WORD * )( ( BYTE * )TVBuf->IndexBufferL + MBTList->VBStartIndex            * sizeof( WORD ) ) ;
			ToonDest = ( WORD * )( ( BYTE * )TVBuf->IndexBufferL + MBTList->ToonOutLineVBStartIndex * sizeof( WORD ) ) ;
			_MEMCPY( Dest, MBTList->Index, sizeof( WORD ) * MBTList->IndexNum ) ;
			if( MBTList->ToonOutLineIndexNum > 0 )
			{
				_MEMCPY( ToonDest, MBTList->ToonOutLineIndex, sizeof( WORD ) * MBTList->ToonOutLineIndexNum ) ;
			}

			if( MBase->UsePackDraw )
			{
				Dest              += MBTList->IndexNum ;
				DestBaseIndex      = MBTList->VertexNum ;

				ToonDest          += MBTList->ToonOutLineIndexNum ;
				ToonDestBaseIndex  = MBTList->VertexNum ;
				for( j = 1 ; j < MBTList->PackDrawMaxNum ; j ++ )
				{
					_MEMCPY( Dest, MBTList->Index, sizeof( WORD ) * MBTList->IndexNum ) ;
					for( k = 0 ; k < MBTList->IndexNum ; k ++, Dest ++ )
					{
						*Dest += ( WORD )DestBaseIndex ;
					}
					DestBaseIndex += MBTList->VertexNum ;

					if( MBTList->ToonOutLineIndexNum > 0 )
					{
						_MEMCPY( ToonDest, MBTList->ToonOutLineIndex, sizeof( WORD ) * MBTList->ToonOutLineIndexNum ) ;
						for( k = 0 ; k < MBTList->ToonOutLineIndexNum ; k ++, ToonDest ++ )
						{
							*ToonDest += ( WORD )ToonDestBaseIndex ;
						}
						ToonDestBaseIndex += MBTList->VertexNum ;
					}
				}
			}
		}
	}

	// ���b�N����
	for( i = 0 ; i < 3 ; i ++ )
	{
		for( j = 0 ; j < 2 ; j ++ )
		{
			for( k = 0 ; k < 9 ; k ++ )
			{
				for( TVBuf = MBase->VertexBufferFirst[ i ][ j ][ k ] ; TVBuf ; TVBuf = TVBuf2 )
				{
					TVBuf2 = TVBuf->DataNext ;

					// �o�b�t�@���m�ۂł��Ă��Ȃ�������������
					if( TVBuf->PF->D3D9.VertexBuffer == NULL ||
						TVBuf->PF->D3D9.IndexBuffer  == NULL )
					{
						if( TVBuf->DataPrev != NULL )
						{
							TVBuf->DataPrev->DataNext = TVBuf->DataNext ;
						}
						else
						{
							MBase->VertexBufferFirst[ i ][ j ][ k ] = TVBuf->DataNext ;
						}

						if( TVBuf->DataNext != NULL )
						{
							TVBuf->DataNext->DataPrev = TVBuf->DataPrev ;
						}
						else
						{
							MBase->VertexBufferLast[ i ][ j ][ k ] = TVBuf->DataPrev ;
						}

						if( TVBuf->PF->D3D9.VertexBuffer )
						{
							Direct3D9_ObjectRelease_ASync( TVBuf->PF->D3D9.VertexBuffer, ASyncThread ) ;
							TVBuf->PF->D3D9.VertexBuffer = NULL ;
						}

						if( TVBuf->PF->D3D9.IndexBuffer )
						{
							Direct3D9_ObjectRelease_ASync( TVBuf->PF->D3D9.IndexBuffer, ASyncThread ) ;
							TVBuf->PF->D3D9.IndexBuffer = NULL ;
						}

						// ���̃o�b�t�@���g�p���Ă����g���C�A���O�����X�g�̃|�C���^���N���A����
						MBTList = MBase->TriangleList ;
						for( l = 0 ; l < MBase->TriangleListNum ; l ++, MBTList ++ )
						{
							if( MBTList->VertexBuffer != TVBuf ) continue ;
							MBTList->VertexBuffer = NULL ;
						}

						DXFREE( TVBuf ) ;
						MBase->VertexBufferNum[ i ][ j ][ k ] -- ;
						MBase->TotalVertexBufferNum -- ;
					}
					else
					{
						// �m�ۂł��Ă����烍�b�N����������
						if( TVBuf->PF->D3D9.VertexBuffer )
							Direct3DVertexBuffer9_Unlock_ASync( TVBuf->PF->D3D9.VertexBuffer, ASyncThread ) ;

						if( TVBuf->PF->D3D9.IndexBuffer )
							Direct3DIndexBuffer9_Unlock_ASync( TVBuf->PF->D3D9.IndexBuffer, ASyncThread ) ;
					}
				}
			}
		}
	}

	// �Z�b�g�A�b�v�����t���O�𗧂Ă�
	MBase->SetupVertexBuffer = TRUE ;

	// ���̃n���h�����g�p���Ă��郂�f���̒��_�o�b�t�@���Z�b�g�A�b�v����
	{
		MV1_MODEL *Model ;

		if( HandleManageArray[ DX_HANDLETYPE_MODEL ].InitializeFlag )
		{
			for( i = HandleManageArray[ DX_HANDLETYPE_MODEL ].AreaMin ; i <= HandleManageArray[ DX_HANDLETYPE_MODEL ].AreaMax ; i ++ )
			{
				Model = ( MV1_MODEL * )HandleManageArray[ DX_HANDLETYPE_MODEL ].Handle[ i ] ;
				if( Model == NULL ) continue ;
#ifndef DX_NON_ASYNCLOAD
				if( Model->HandleInfo.ASyncLoadCount != 0 ) continue ;
#endif // DX_NON_ASYNCLOAD
				if( Model->BaseData != MBase ) continue ;

				MV1_SetupVertexBuffer_PF( Model->HandleInfo.Handle, ASyncThread ) ;
			}
		}
	}

	// �I��
	return 0 ;

ERR:
	// �m�ۂ������_�o�b�t�@�Ȃǂ����
	for( i = 0 ; i < 3 ; i ++ )
	{
		for( j = 0 ; j < 2 ; j ++ )
		{
			for( k = 0 ; k < 9 ; k ++ )
			{
				for( TVBuf = MBase->VertexBufferFirst[ i ][ j ][ k ] ; TVBuf ; TVBuf = TVBuf2 )
				{
					if( TVBuf->PF->D3D9.VertexBuffer )
					{
						Direct3DVertexBuffer9_Unlock_ASync( TVBuf->PF->D3D9.VertexBuffer, ASyncThread ) ;
						Direct3D9_ObjectRelease_ASync( TVBuf->PF->D3D9.VertexBuffer, ASyncThread ) ;
					}

					if( TVBuf->PF->D3D9.IndexBuffer )
					{
						Direct3DIndexBuffer9_Unlock_ASync( TVBuf->PF->D3D9.IndexBuffer, ASyncThread ) ;
						Direct3D9_ObjectRelease_ASync( TVBuf->PF->D3D9.IndexBuffer, ASyncThread ) ;
					}

					TVBuf2 = TVBuf->DataNext ;
					DXFREE( TVBuf ) ;
				}
				MBase->VertexBufferFirst[ i ][ j ][ k ] = NULL ;
				MBase->VertexBufferLast[ i ][ j ][ k ] = NULL ;
				MBase->VertexBufferNum[ i ][ j ][ k ] = 0 ;
			}
		}
	}
	MBase->TotalVertexBufferNum = 0 ;

	// �I��
	return -1 ;
}

// ���f���f�[�^�̒��_�o�b�t�@�̃Z�b�g�A�b�v������( -1:�G���[ )
extern int MV1_D3D9_SetupVertexBuffer_PF( int MHandle, int ASyncThread )
{
	MV1_MODEL              *Model ;
	MV1_MODEL_BASE         *MBase ;
	MV1_MESH               *Mesh ;
	MV1_MESH_BASE          *MBMesh ;
	MV1_FRAME_BASE         *MBFrame ;
	MV1_FRAME              *Frame ;
	MV1_TRIANGLE_LIST      *TList ;
	MV1_TRIANGLE_LIST_BASE *MBTList ;
	int                    i ;
	int                    l ;
	int                    m ;
	bool                   ValidVertexBuffer ;

	// ����������Ă��Ȃ�������G���[
	if( MV1Man.Initialize == false )
	{
		return -1 ;
	}

	// �A�h���X�擾
	if( ASyncThread )
	{
		if( MV1MDLCHK_ASYNC( MHandle, Model ) )
			return -1 ;
	}
	else
	{
		if( MV1MDLCHK( MHandle, Model ) )
			return -1 ;
	}
	MBase = Model->BaseData ;

	// ���_�o�b�t�@���g�p���邩�ǂ����̃t���O���Z�b�g�A�b�v���Ă���
	ValidVertexBuffer = true ;
	if( GSYS.Setting.ValidHardware == FALSE || ( GD3D9.Device.Caps.VertexHardwareProcess == FALSE && GD3D9.Device.Shader.ValidVertexShader == FALSE ) )
		ValidVertexBuffer = false ;

	// ���_�o�b�t�@���g�p����\���������ꍇ�͉��������ɏI��
	if( ValidVertexBuffer == false && GSYS.HardInfo.UseShader == FALSE )
		return 0 ;

	// �V�F�C�v���g�p����Ă���t���[���̂ݏ�������
	MBFrame = MBase->Frame ;
	Frame = Model->Frame ;
	for( i = 0 ; i < MBase->FrameNum ; i ++, Frame ++, MBFrame ++ )
	{
		if( MBFrame->ShapeNum == 0 ) continue ;

		MBMesh = MBFrame->Mesh ;
		Mesh = Frame->Mesh ;
		for( l = 0 ; l < MBFrame->MeshNum ; l ++, MBMesh ++, Mesh ++ )
		{
			if( MBMesh->Shape == 0 ) continue ;

			MBTList = MBMesh->TriangleList ;
			TList = Mesh->TriangleList ;
			for( m = 0 ; m < MBMesh->TriangleListNum ; m ++, MBTList ++, TList = ( MV1_TRIANGLE_LIST * )( ( BYTE * )TList + sizeof( MV1_TRIANGLE_LIST ) + sizeof( MV1_TRIANGLE_LIST_PF ) ) )
			{
				// ���_�o�b�t�@���g�p���邩�ǂ����ŏ����𕪊�
				if( ( MBTList->VertexType == MV1_VERTEX_TYPE_NORMAL && ValidVertexBuffer == true ) ||
					( MBTList->VertexType != MV1_VERTEX_TYPE_NORMAL && GSYS.HardInfo.UseShader )
					)
				{
					// ���_�o�b�t�@���Z�b�g�A�b�v����Ă��Ȃ������炷��
					if( TList->PF->D3D9.VertexBuffer == NULL && MBTList->VertexBuffer != NULL )
					{
						Direct3DDevice9_CreateVertexBuffer_ASync(
							MBTList->VertexBuffer->UnitSize * MBTList->VertexNum,
							D_D3DUSAGE_WRITEONLY | D_D3DUSAGE_DYNAMIC,
							MBTList->VertexBuffer->PF->D3D9.FVF,
							D_D3DPOOL_DEFAULT,
							&TList->PF->D3D9.VertexBuffer,
							ASyncThread
						) ;

						// �V�F�C�v�̃Z�b�g�A�b�v���s���Ă��Ȃ���Ԃɂ���
						Model->ShapeChangeFlag = true ;
						TList->Container->Container->ShapeChangeFlag = true ;
					}
				}
			}
		}
	}

	// �I��
	return 0 ;
}

// ���_�o�b�t�@�̌�n��������( -1:�G���[ )
extern	int				MV1_D3D9_TerminateVertexBufferBase_PF( int MV1ModelBaseHandle )
{
	int i, j, k ;
	MV1_MODEL_BASE *MBase ;
	MV1_MODEL *Model ;
	MV1_TRIANGLE_LIST_BASE *MBTList ;
	MV1_VERTEXBUFFER *TVBuf, *TVBuf2 ;

	// �n���h���`�F�b�N
	if( MV1BMDLCHK( MV1ModelBaseHandle, MBase ) )
		return -1 ;
	if( MBase == NULL ) return -1 ;

	// ���_�o�b�t�@���Z�b�g�A�b�v����Ă��Ȃ��ꍇ�͉��������I��
	if( MBase->SetupVertexBuffer == FALSE )
		return 0 ;

	// �m�ۂ������_�o�b�t�@�Ȃǂ����
	for( i = 0 ; i < 3 ; i ++ )
	{
		for( j = 0 ; j < 2 ; j ++ )
		{
			for( k = 0 ; k < 9 ; k ++ )
			{
				for( TVBuf = MBase->VertexBufferFirst[ i ][ j ][ k ] ; TVBuf ; )
				{
					if( TVBuf->PF->D3D9.VertexBuffer )
					{
						Direct3D9_ObjectRelease_ASync( TVBuf->PF->D3D9.VertexBuffer ) ;
					}

					if( TVBuf->PF->D3D9.IndexBuffer )
					{
						Direct3D9_ObjectRelease_ASync( TVBuf->PF->D3D9.IndexBuffer ) ;
					}

					TVBuf2 = TVBuf->DataNext ;
					DXFREE( TVBuf ) ;
					TVBuf = TVBuf2 ;
				}
				MBase->VertexBufferFirst[ i ][ j ][ k ] = NULL ;
				MBase->VertexBufferLast[ i ][ j ][ k ] = NULL ;
				MBase->VertexBufferNum[ i ][ j ][ k ] = 0 ;
			}
		}
	}
	MBase->TotalVertexBufferNum = 0 ;

	// �g���C�A���O�����X�g�̐������J��Ԃ�
	MBTList = MBase->TriangleList ;
	for( i = 0 ; i < MBase->TriangleListNum ; i ++, MBTList ++ )
	{
		// ���_�o�b�t�@�̏����N���A
		MBTList->VertexBuffer = NULL ;
		MBTList->VBStartVertex = 0 ;
		MBTList->VBStartIndex = 0 ;
	}

	// ���_�o�b�t�@�Z�b�g�A�b�v�t���O��|��
	MBase->SetupVertexBuffer = FALSE ;

	// ���̃n���h�����g�p���Ă��郂�f���̒��_�o�b�t�@���������
	if( HandleManageArray[ DX_HANDLETYPE_MODEL ].InitializeFlag )
	{
		for( i = HandleManageArray[ DX_HANDLETYPE_MODEL ].AreaMin ; i <= HandleManageArray[ DX_HANDLETYPE_MODEL ].AreaMax ; i ++ )
		{
			Model = ( MV1_MODEL * )HandleManageArray[ DX_HANDLETYPE_MODEL ].Handle[ i ] ;
			if( Model == NULL ) continue ;
#ifndef DX_NON_ASYNCLOAD
			if( Model->HandleInfo.ASyncLoadCount != 0 ) continue ;
#endif // DX_NON_ASYNCLOAD
			if( Model->BaseData != MBase ) continue ;

			MV1_TerminateVertexBuffer_PF( Model->HandleInfo.Handle ) ;
		}
	}

	// �I��
	return 0 ;
}

// ���_�o�b�t�@�̌�n��������( -1:�G���[ )
extern	int				MV1_D3D9_TerminateVertexBuffer_PF( int MV1ModelHandle )
{
	int i ;
	MV1_MODEL *Model ;
	MV1_TRIANGLE_LIST *TList ;

	// �n���h���`�F�b�N
	if( MV1MDLCHK_ASYNC( MV1ModelHandle, Model ) )
		return -1 ;

	// �g���C�A���O�����X�g�̐������J��Ԃ�
	TList = Model->TriangleList ;
	for( i = 0 ; i < Model->BaseData->TriangleListNum ; i ++, TList = ( MV1_TRIANGLE_LIST * )( ( BYTE * )TList + sizeof( MV1_TRIANGLE_LIST ) + sizeof( MV1_TRIANGLE_LIST_PF ) ) )
	{
		// ���_�o�b�t�@�̏����N���A
		if( TList->PF->D3D9.VertexBuffer )
		{
			Direct3D9_ObjectRelease_ASync( TList->PF->D3D9.VertexBuffer ) ;
			TList->PF->D3D9.VertexBuffer = NULL ;
		}
	}

	// �I��
	return 0 ;
}

// �V�F�C�v�f�[�^�̃Z�b�g�A�b�v������
extern	int				MV1_D3D9_SetupShapeVertex_PF( int MHandle )
{
	MV1_MODEL *Model ;
	MV1_MODEL_BASE *MBase ;
	MV1_MESH *Mesh ;
	MV1_MESH_BASE *MBMesh ;
	MV1_FRAME_BASE *MBFrame ;
	MV1_FRAME *Frame ;
	MV1_TRIANGLE_LIST *TList ;
	MV1_TRIANGLE_LIST_BASE *MBTList ;
	MV1_SHAPE *Shape ;
	MV1_SHAPE_BASE *MBShape ;
	MV1_SHAPE_MESH_BASE *MBShapeMesh ;
	MV1_SHAPE_VERTEX *ShapeVertex, *SVert ;
	MV1_SHAPE_VERTEX_BASE *MBShapeVertex ;
	MV1_MESH_VERTEX *MeshVertex ;
	MV1_MESH_NORMAL *MeshNorm, *MNorm ;
	MV1_MESH_VERTEX *MeshVert, *MVert ;
	MV1_MODEL_ANIM *MAnim, *MAnim2 ;
	MV1_MODEL_ANIMSET *MAnimSet ;
	int i, j, k, l, m, VertexNum, UVNum, MeshVertSize ;
	DWORD DestUnitSize ;
	DWORD *MVInd ;
	float Rate ;
	bool ValidVertexBuffer ;
	int Bump ;
	void *Vert ;

	// ����������Ă��Ȃ�������G���[
	if( MV1Man.Initialize == false ) return -1 ;

	// �A�h���X�擾
	if( MV1MDLCHK( MHandle, Model ) )
		return -1 ;
	MBase = Model->BaseData ;

	// �V�F�C�v�̏�Ԃ��ω����Ă��Ȃ��ꍇ�͉������Ȃ�
	if( Model->ShapeChangeFlag == false ) return 0 ;
	Model->ShapeChangeFlag = false ;

	// ���_�o�b�t�@���g�p���邩�ǂ����̃t���O���Z�b�g�A�b�v���Ă���
	ValidVertexBuffer = true ;
	if( GSYS.Setting.ValidHardware == FALSE || ( GD3D9.Device.Caps.VertexHardwareProcess == FALSE && GD3D9.Device.Shader.ValidVertexShader == FALSE ) )
		ValidVertexBuffer = false ;

	// �V�F�C�v���g�p����Ă��郁�b�V���̂ݏ�������
	MBMesh = MBase->Mesh ;
	Mesh = Model->Mesh ;
	for( i = 0 ; i < MBase->MeshNum ; i ++, Mesh ++, MBMesh ++ )
	{
		if( MBMesh->Shape == 0 ) continue ;

		// �V�F�C�v���ω����Ă��Ȃ��ꍇ�͉������Ȃ�
		if( Mesh->Container->ShapeChangeFlag == false ) continue ;

		// �������_�f�[�^�̍쐬
		MBFrame = MBMesh->Container ;

		// �ŏ��͊�̒��_�f�[�^���Z�b�g
		ShapeVertex = Mesh->ShapeVertex ;
		MeshVertex = MBMesh->Vertex ;
		for( j = 0 ; j < MBMesh->VertexNum ; j ++, ShapeVertex ++, MeshVertex = ( MV1_MESH_VERTEX * )( ( BYTE * )MeshVertex + MBMesh->VertUnitSize ) )
		{
			ShapeVertex->Position = ( ( MV1_MESH_POSITION * )( ( BYTE * )MBFrame->Position + MeshVertex->PositionIndex * MBFrame->PosUnitSize ) )->Position ;
			ShapeVertex->Normal = MBFrame->Normal[ MeshVertex->NormalIndex ].Normal ;
		}
	}

	// �V�F�C�v���g�p����Ă���t���[���̂ݏ�������
	MBFrame = MBase->Frame ;
	Frame = Model->Frame ;
	MAnim = Model->Anim ;
	for( i = 0 ; i < MBase->FrameNum ; i ++, Frame ++, MBFrame ++, MAnim += Model->AnimSetMaxNum )
	{
		if( MBFrame->ShapeNum == 0 ) continue ;

		// �V�F�C�v���ω����Ă��Ȃ��ꍇ�͉������Ȃ�
		if( Frame->ShapeChangeFlag == false ) continue ;
		Frame->ShapeChangeFlag = false ;

		// �V�F�C�v���g�p���Ȃ��t���O�������Ă������{��񂻂̂܂܂��g�p����
		if( Model->ShapeDisableFlag == false )
		{
			Shape = Frame->Shape ;
			for( j = 0 ; j < MBFrame->ShapeNum ; j ++, Shape ++ )
			{
				MBShape = Shape->BaseData ;

				// �u�����h�����擾����
				Rate = Shape->Rate ;
				MAnim2 = MAnim ;
				MAnimSet = Model->AnimSet ;
				for( k = 0 ; k < Model->AnimSetMaxNum ; k ++, MAnim2 ++, MAnimSet ++ )
				{
					if( MAnim2->Use == false )
						continue ;

					if( MAnimSet->DisableShapeFlag )
						continue ;

					Rate += MAnim2->BlendRate * MAnim2->Anim->ShapeRate[ j ] ;
				}
				if( Rate < 0.0000001f ) continue ;

				MBShapeMesh = MBShape->Mesh ;
				for( k = 0 ; k < MBShape->MeshNum ; k ++, MBShapeMesh ++ )
				{
					Mesh = MBShapeMesh->TargetMesh - MBase->Mesh + Model->Mesh ;
					MBMesh = Mesh->BaseData ;

					MBShapeVertex = MBShapeMesh->Vertex ;
					for( l = 0 ; ( DWORD )l < MBShapeMesh->VertexNum ; l ++, MBShapeVertex ++ )
					{
						ShapeVertex = &Mesh->ShapeVertex[ MBShapeVertex->TargetMeshVertex ] ;
						ShapeVertex->Position.x += MBShapeVertex->Position.x * Rate ;
						ShapeVertex->Position.y += MBShapeVertex->Position.y * Rate ;
						ShapeVertex->Position.z += MBShapeVertex->Position.z * Rate ;
						ShapeVertex->Normal.x += MBShapeVertex->Normal.x * Rate ;
						ShapeVertex->Normal.y += MBShapeVertex->Normal.y * Rate ;
						ShapeVertex->Normal.z += MBShapeVertex->Normal.z * Rate ;
					}
				}
			}
		}

		MBMesh = MBFrame->Mesh ;
		Mesh = Frame->Mesh ;
		for( l = 0 ; l < MBFrame->MeshNum ; l ++, MBMesh ++, Mesh ++ )
		{
			if( MBMesh->Shape == 0 ) continue ;

			MeshNorm = MBMesh->Container->Normal ;
			UVNum = MBMesh->UVSetUnitNum ;
			MeshVertSize = MBMesh->VertUnitSize ;
			MeshVert = MBMesh->Vertex ;
			ShapeVertex = Mesh->ShapeVertex ;
			Bump = MBMesh->Material->NormalLayerNum ? 1 : 0 ;

			MBTList = MBMesh->TriangleList ;
			TList = Mesh->TriangleList ;
			for( m = 0 ; m < MBMesh->TriangleListNum ; m ++, MBTList ++, TList = ( MV1_TRIANGLE_LIST * )( ( BYTE * )TList + sizeof( MV1_TRIANGLE_LIST ) + sizeof( MV1_TRIANGLE_LIST_PF ) ) )
			{
				MVInd = MBTList->MeshVertexIndex ;
				VertexNum = MBTList->VertexNum ;

				// ���_�o�b�t�@���g�p���邩�ǂ����ŏ����𕪊�
				if( ( MBTList->VertexType == MV1_VERTEX_TYPE_NORMAL && ValidVertexBuffer == true ) ||
					( MBTList->VertexType != MV1_VERTEX_TYPE_NORMAL && GSYS.HardInfo.UseShader )
					)
				{
					if( TList->PF->D3D9.VertexBuffer == NULL ) continue ;

					// ���_�f�[�^���X�V����
					if( Direct3DVertexBuffer9_Lock_ASync( TList->PF->D3D9.VertexBuffer, 0, 0, &Vert, D_D3DLOCK_DISCARD ) != D_D3D_OK ) continue ;

					DestUnitSize = MBTList->VertexBuffer->UnitSize ;
					switch( MBTList->VertexType )
					{
					case MV1_VERTEX_TYPE_NORMAL :
						if( Bump )
						{
							MV1_VERTEX_SIMPLE_BUMP_D3D9 *Dest ;
							MV1_TLIST_NORMAL_POS *Src ;

							Src = ( MV1_TLIST_NORMAL_POS * )ADDR16( MBTList->NormalPosition ) ;
							Dest = ( MV1_VERTEX_SIMPLE_BUMP_D3D9 * )Vert ;
							for( j = 0 ; j < VertexNum ; j ++, Dest = ( MV1_VERTEX_SIMPLE_BUMP_D3D9 * )( ( BYTE * )Dest + DestUnitSize ), Src ++, MVInd ++ )
							{
								MVert = ( MV1_MESH_VERTEX   * )( ( BYTE * )MeshVert + MeshVertSize * *MVInd ) ;
								MNorm = MeshNorm + MVert->NormalIndex ;
								SVert = ShapeVertex + *MVInd ;

								Dest->Position      = SVert->Position ;
								Dest->Normal        = MNorm->Normal ;
								Dest->Binormal      = MNorm->Binormal ;
								Dest->Tangent       = MNorm->Tangent ;
								Dest->DiffuseColor  = MVert->DiffuseColor ;
								Dest->SpecularColor = MVert->SpecularColor ;

								for( k = 0 ; k < UVNum ; k ++ )
								{
									Dest->UVs[ k ][ 0 ] = MVert->UVs[ k ][ 0 ] ;
									Dest->UVs[ k ][ 1 ] = MVert->UVs[ k ][ 1 ] ;
									Dest->UVs[ k ][ 2 ] = 1.0f ;
									Dest->UVs[ k ][ 3 ] = 1.0f ;
								}
							}
						}
						else
						{
							MV1_VERTEX_SIMPLE_D3D9 *Dest ;
							MV1_TLIST_NORMAL_POS *Src ;

							Src = ( MV1_TLIST_NORMAL_POS * )ADDR16( MBTList->NormalPosition ) ;
							Dest = ( MV1_VERTEX_SIMPLE_D3D9 * )Vert ;
							for( j = 0 ; j < VertexNum ; j ++, Dest = ( MV1_VERTEX_SIMPLE_D3D9 * )( ( BYTE * )Dest + DestUnitSize ), Src ++, MVInd ++ )
							{
								MVert = ( MV1_MESH_VERTEX   * )( ( BYTE * )MeshVert + MeshVertSize * *MVInd ) ;
								MNorm = MeshNorm + MVert->NormalIndex ;
								SVert = ShapeVertex + *MVInd ;

								Dest->Position      = SVert->Position ;
								Dest->Normal        = MNorm->Normal ;
								Dest->DiffuseColor  = MVert->DiffuseColor ;
								Dest->SpecularColor = MVert->SpecularColor ;

								for( k = 0 ; k < UVNum ; k ++ )
								{
									Dest->UVs[ k ][ 0 ] = MVert->UVs[ k ][ 0 ] ;
									Dest->UVs[ k ][ 1 ] = MVert->UVs[ k ][ 1 ] ;
									Dest->UVs[ k ][ 2 ] = 1.0f ;
									Dest->UVs[ k ][ 3 ] = 1.0f ;
								}
							}
						}
						break ;

					case MV1_VERTEX_TYPE_SKIN_4BONE :
						if( Bump )
						{
							MV1_VERTEX_SKIN_B4_BUMP_D3D9 *Dest ;
							MV1_TLIST_SKIN_POS_4B *Src ;

							Src = ( MV1_TLIST_SKIN_POS_4B * )ADDR16( MBTList->SkinPosition4B ) ;
							Dest = ( MV1_VERTEX_SKIN_B4_BUMP_D3D9 * )Vert ;
							for( j = 0 ; j < VertexNum ; j ++, Dest = ( MV1_VERTEX_SKIN_B4_BUMP_D3D9 * )( ( BYTE * )Dest + DestUnitSize ), Src ++, MVInd ++ )
							{
								MVert = ( MV1_MESH_VERTEX   * )( ( BYTE * )MeshVert + MeshVertSize * *MVInd ) ;
								MNorm = MeshNorm + MVert->NormalIndex ;
								SVert = ShapeVertex + *MVInd ;

								Dest->Position          = SVert->Position ;
								Dest->MatrixIndex[ 0 ]  = ( BYTE )( Src->MatrixIndex[ 0 ] * 3 ) ;
								Dest->MatrixIndex[ 1 ]  = ( BYTE )( Src->MatrixIndex[ 1 ] * 3 ) ;
								Dest->MatrixIndex[ 2 ]  = ( BYTE )( Src->MatrixIndex[ 2 ] * 3 ) ;
								Dest->MatrixIndex[ 3 ]  = ( BYTE )( Src->MatrixIndex[ 3 ] * 3 ) ;
								Dest->MatrixWeight[ 0 ] = Src->MatrixWeight [ 0 ] ;
								Dest->MatrixWeight[ 1 ] = Src->MatrixWeight [ 1 ] ;
								Dest->MatrixWeight[ 2 ] = Src->MatrixWeight [ 2 ] ;
								Dest->MatrixWeight[ 3 ] = Src->MatrixWeight [ 3 ] ;
								Dest->Normal        = MNorm->Normal ;
								Dest->Binormal      = MNorm->Binormal ;
								Dest->Tangent       = MNorm->Tangent ;
								Dest->DiffuseColor  = MVert->DiffuseColor ;
								Dest->SpecularColor = MVert->SpecularColor ;

								for( k = 0 ; k < UVNum ; k ++ )
								{
									Dest->UVs[ k ][ 0 ] = MVert->UVs[ k ][ 0 ] ;
									Dest->UVs[ k ][ 1 ] = MVert->UVs[ k ][ 1 ] ;
									Dest->UVs[ k ][ 2 ] = 1.0f ;
									Dest->UVs[ k ][ 3 ] = 1.0f ;
								}
							}
						}
						else
						{
							MV1_VERTEX_SKIN_B4_D3D9 *Dest ;
							MV1_TLIST_SKIN_POS_4B *Src ;

							Src = ( MV1_TLIST_SKIN_POS_4B * )ADDR16( MBTList->SkinPosition4B ) ;
							Dest = ( MV1_VERTEX_SKIN_B4_D3D9 * )Vert ;
							for( j = 0 ; j < VertexNum ; j ++, Dest = ( MV1_VERTEX_SKIN_B4_D3D9 * )( ( BYTE * )Dest + DestUnitSize ), Src ++, MVInd ++ )
							{
								MVert = ( MV1_MESH_VERTEX   * )( ( BYTE * )MeshVert + MeshVertSize * *MVInd ) ;
								MNorm = MeshNorm + MVert->NormalIndex ;
								SVert = ShapeVertex + *MVInd ;

								Dest->Position          = SVert->Position ;
								Dest->MatrixIndex[ 0 ]  = ( BYTE )( Src->MatrixIndex[ 0 ] * 3 ) ;
								Dest->MatrixIndex[ 1 ]  = ( BYTE )( Src->MatrixIndex[ 1 ] * 3 ) ;
								Dest->MatrixIndex[ 2 ]  = ( BYTE )( Src->MatrixIndex[ 2 ] * 3 ) ;
								Dest->MatrixIndex[ 3 ]  = ( BYTE )( Src->MatrixIndex[ 3 ] * 3 ) ;
								Dest->MatrixWeight[ 0 ] = Src->MatrixWeight [ 0 ] ;
								Dest->MatrixWeight[ 1 ] = Src->MatrixWeight [ 1 ] ;
								Dest->MatrixWeight[ 2 ] = Src->MatrixWeight [ 2 ] ;
								Dest->MatrixWeight[ 3 ] = Src->MatrixWeight [ 3 ] ;
								Dest->Normal        = MNorm->Normal ;
								Dest->DiffuseColor  = MVert->DiffuseColor ;
								Dest->SpecularColor = MVert->SpecularColor ;

								for( k = 0 ; k < UVNum ; k ++ )
								{
									Dest->UVs[ k ][ 0 ] = MVert->UVs[ k ][ 0 ] ;
									Dest->UVs[ k ][ 1 ] = MVert->UVs[ k ][ 1 ] ;
									Dest->UVs[ k ][ 2 ] = 1.0f ;
									Dest->UVs[ k ][ 3 ] = 1.0f ;
								}
							}
						}
						break ;

					case MV1_VERTEX_TYPE_SKIN_8BONE :
						if( Bump )
						{
							MV1_VERTEX_SKIN_B8_BUMP_D3D9 *Dest ;
							MV1_TLIST_SKIN_POS_8B *Src ;

							Src = ( MV1_TLIST_SKIN_POS_8B * )ADDR16( MBTList->SkinPosition8B ) ;
							Dest = ( MV1_VERTEX_SKIN_B8_BUMP_D3D9 * )Vert ;
							for( j = 0 ; j < VertexNum ; j ++, Dest = ( MV1_VERTEX_SKIN_B8_BUMP_D3D9 * )( ( BYTE * )Dest + DestUnitSize ), Src ++, MVInd ++ )
							{
								MVert = ( MV1_MESH_VERTEX   * )( ( BYTE * )MeshVert + MeshVertSize * *MVInd ) ;
								MNorm = MeshNorm + MVert->NormalIndex ;
								SVert = ShapeVertex + *MVInd ;

								Dest->Position          = SVert->Position ;
								Dest->MatrixIndex[ 0 ]  = ( BYTE )( Src->MatrixIndex1[ 0 ] * 3 ) ;
								Dest->MatrixIndex[ 1 ]  = ( BYTE )( Src->MatrixIndex1[ 1 ] * 3 ) ;
								Dest->MatrixIndex[ 2 ]  = ( BYTE )( Src->MatrixIndex1[ 2 ] * 3 ) ;
								Dest->MatrixIndex[ 3 ]  = ( BYTE )( Src->MatrixIndex1[ 3 ] * 3 ) ;
								Dest->MatrixIndex[ 4 ]  = ( BYTE )( Src->MatrixIndex2[ 0 ] * 3 ) ;
								Dest->MatrixIndex[ 5 ]  = ( BYTE )( Src->MatrixIndex2[ 1 ] * 3 ) ;
								Dest->MatrixIndex[ 6 ]  = ( BYTE )( Src->MatrixIndex2[ 2 ] * 3 ) ;
								Dest->MatrixIndex[ 7 ]  = ( BYTE )( Src->MatrixIndex2[ 3 ] * 3 ) ;
								Dest->MatrixWeight[ 0 ] = Src->MatrixWeight [ 0 ] ;
								Dest->MatrixWeight[ 1 ] = Src->MatrixWeight [ 1 ] ;
								Dest->MatrixWeight[ 2 ] = Src->MatrixWeight [ 2 ] ;
								Dest->MatrixWeight[ 3 ] = Src->MatrixWeight [ 3 ] ;
								Dest->MatrixWeight[ 4 ] = Src->MatrixWeight [ 4 ] ;
								Dest->MatrixWeight[ 5 ] = Src->MatrixWeight [ 5 ] ;
								Dest->MatrixWeight[ 6 ] = Src->MatrixWeight [ 6 ] ;
								Dest->MatrixWeight[ 7 ] = Src->MatrixWeight [ 7 ] ;
								Dest->Normal        = MNorm->Normal ;
								Dest->Binormal      = MNorm->Binormal ;
								Dest->Tangent       = MNorm->Tangent ;
								Dest->DiffuseColor  = MVert->DiffuseColor ;
								Dest->SpecularColor = MVert->SpecularColor ;

								for( k = 0 ; k < UVNum ; k ++ )
								{
									Dest->UVs[ k ][ 0 ] = MVert->UVs[ k ][ 0 ] ;
									Dest->UVs[ k ][ 1 ] = MVert->UVs[ k ][ 1 ] ;
									Dest->UVs[ k ][ 2 ] = 1.0f ;
									Dest->UVs[ k ][ 3 ] = 1.0f ;
								}
							}
						}
						else
						{
							MV1_VERTEX_SKIN_B8_D3D9 *Dest ;
							MV1_TLIST_SKIN_POS_8B *Src ;

							Src = ( MV1_TLIST_SKIN_POS_8B * )ADDR16( MBTList->SkinPosition8B ) ;
							Dest = ( MV1_VERTEX_SKIN_B8_D3D9 * )Vert ;
							for( j = 0 ; j < VertexNum ; j ++, Dest = ( MV1_VERTEX_SKIN_B8_D3D9 * )( ( BYTE * )Dest + DestUnitSize ), Src ++, MVInd ++ )
							{
								MVert = ( MV1_MESH_VERTEX   * )( ( BYTE * )MeshVert + MeshVertSize * *MVInd ) ;
								MNorm = MeshNorm + MVert->NormalIndex ;
								SVert = ShapeVertex + *MVInd ;

								Dest->Position          = SVert->Position ;
								Dest->MatrixIndex[ 0 ]  = ( BYTE )( Src->MatrixIndex1[ 0 ] * 3 ) ;
								Dest->MatrixIndex[ 1 ]  = ( BYTE )( Src->MatrixIndex1[ 1 ] * 3 ) ;
								Dest->MatrixIndex[ 2 ]  = ( BYTE )( Src->MatrixIndex1[ 2 ] * 3 ) ;
								Dest->MatrixIndex[ 3 ]  = ( BYTE )( Src->MatrixIndex1[ 3 ] * 3 ) ;
								Dest->MatrixIndex[ 4 ]  = ( BYTE )( Src->MatrixIndex2[ 0 ] * 3 ) ;
								Dest->MatrixIndex[ 5 ]  = ( BYTE )( Src->MatrixIndex2[ 1 ] * 3 ) ;
								Dest->MatrixIndex[ 6 ]  = ( BYTE )( Src->MatrixIndex2[ 2 ] * 3 ) ;
								Dest->MatrixIndex[ 7 ]  = ( BYTE )( Src->MatrixIndex2[ 3 ] * 3 ) ;
								Dest->MatrixWeight[ 0 ] = Src->MatrixWeight [ 0 ] ;
								Dest->MatrixWeight[ 1 ] = Src->MatrixWeight [ 1 ] ;
								Dest->MatrixWeight[ 2 ] = Src->MatrixWeight [ 2 ] ;
								Dest->MatrixWeight[ 3 ] = Src->MatrixWeight [ 3 ] ;
								Dest->MatrixWeight[ 4 ] = Src->MatrixWeight [ 4 ] ;
								Dest->MatrixWeight[ 5 ] = Src->MatrixWeight [ 5 ] ;
								Dest->MatrixWeight[ 6 ] = Src->MatrixWeight [ 6 ] ;
								Dest->MatrixWeight[ 7 ] = Src->MatrixWeight [ 7 ] ;
								Dest->Normal        = MNorm->Normal ;
								Dest->DiffuseColor  = MVert->DiffuseColor ;
								Dest->SpecularColor = MVert->SpecularColor ;

								for( k = 0 ; k < UVNum ; k ++ )
								{
									Dest->UVs[ k ][ 0 ] = MVert->UVs[ k ][ 0 ] ;
									Dest->UVs[ k ][ 1 ] = MVert->UVs[ k ][ 1 ] ;
									Dest->UVs[ k ][ 2 ] = 1.0f ;
									Dest->UVs[ k ][ 3 ] = 1.0f ;
								}
							}
						}
						break ;
					}

					Direct3DVertexBuffer9_Unlock( TList->PF->D3D9.VertexBuffer ) ;
				}

				if( TList->PF->D3D9.VertexBuffer == NULL )
				{
					// ���_�o�b�t�@���g�p���Ȃ��ꍇ

					MVInd = MBTList->MeshVertexIndex ;
					switch( MBTList->VertexType )
					{
					case MV1_VERTEX_TYPE_NORMAL :
						{
							MV1_TLIST_NORMAL_POS *Dest ;

							Dest = ( MV1_TLIST_NORMAL_POS * )ADDR16( TList->NormalPosition ) ;
							for( j = 0 ; j < VertexNum ; j ++, Dest ++, MVInd ++ )
							{
								SVert = ShapeVertex + *MVInd ;
								Dest->Position.x = SVert->Position.x ;
								Dest->Position.y = SVert->Position.y ;
								Dest->Position.z = SVert->Position.z ;
							}
						}
						break ;

					case MV1_VERTEX_TYPE_SKIN_4BONE :
						{
							MV1_TLIST_SKIN_POS_4B *Dest ;

							Dest = ( MV1_TLIST_SKIN_POS_4B * )ADDR16( TList->SkinPosition4B ) ;
							for( j = 0 ; j < VertexNum ; j ++, Dest ++, MVInd ++ )
							{
								SVert = ShapeVertex + *MVInd ;
								Dest->Position.x = SVert->Position.x ;
								Dest->Position.y = SVert->Position.y ;
								Dest->Position.z = SVert->Position.z ;
							}
						}
						break ;

					case MV1_VERTEX_TYPE_SKIN_8BONE :
						{
							MV1_TLIST_SKIN_POS_8B *Dest ;

							Dest = ( MV1_TLIST_SKIN_POS_8B * )ADDR16( TList->SkinPosition8B ) ;
							for( j = 0 ; j < VertexNum ; j ++, Dest ++, MVInd ++ )
							{
								SVert = ShapeVertex + *MVInd ;
								Dest->Position = SVert->Position ;
							}
						}
						break ;

					case MV1_VERTEX_TYPE_SKIN_FREEBONE :
						{
							MV1_TLIST_SKIN_POS_FREEB *Dest ;

							Dest = ( MV1_TLIST_SKIN_POS_FREEB * )ADDR16( TList->SkinPositionFREEB ) ;
							for( j = 0 ; j < VertexNum ; j ++, Dest = ( MV1_TLIST_SKIN_POS_FREEB * )( ( BYTE * )Dest + MBTList->PosUnitSize), MVInd ++ )
							{
								SVert = ShapeVertex + *MVInd ;
								Dest->Position.x = SVert->Position.x ;
								Dest->Position.y = SVert->Position.y ;
								Dest->Position.z = SVert->Position.z ;
							}
						}
						break ;
					}
				}
			}
		}
	}

	// �I��
	return 0 ;
}

// �R�c���f���̃����_�����O�̏������s��
extern	int				MV1_D3D9_BeginRender_PF( MV1_MODEL *Model )
{
	int i ;
	int ShaderSetMask ;

	// �g�p����V�F�[�_�[�萔�Z�b�g��ύX
	ShaderSetMask = DX_SHADERCONSTANTSET_MASK_LIB | DX_SHADERCONSTANTSET_MASK_MV1 ;
	if( MV1Man.UseOrigShaderFlag && GSYS.HardInfo.UseShader )
	{
		ShaderSetMask |= DX_SHADERCONSTANTSET_MASK_USER ;
	}
	Graphics_D3D9_ShaderConstant_InfoSet_SetUseState( &GD3D9.Device.Shader.ShaderConstantInfo, ShaderSetMask ) ;

#ifndef DX_NON_MASK
	// �}�X�N����
	if( MASKD.MaskValidFlag )
	{
		if( MASKD.FullScreenMaskUpdateFlag == FALSE )
		{
			RECT MaskRect ;
			MaskRect.left   = GSYS.DrawSetting.DrawArea.left ;
			MaskRect.right  = GSYS.DrawSetting.DrawArea.right ;
			MaskRect.top    = GSYS.DrawSetting.DrawArea.top ;
			MaskRect.bottom = GSYS.DrawSetting.DrawArea.bottom ;
			MASK_BEGIN( MaskRect )
			MASKD.FullScreenMaskUpdateFlag = TRUE ;
		}
	}
#endif

	// �o�b�t�@�ɂ��܂��Ă��钸�_���o��
	NS_RenderVertex() ;

	// ���C�g�̐ݒ肪�ύX����Ă����甽�f����
	Graphics_Light_RefreshState() ;

	// �R�c�`��p�̎ˉe�s����Z�b�g����
//	Graphics_DrawSetting_SetUse2DProjectionMatrix( FALSE ) ;

	// ���X�ݒ肳��Ă������[�J�������[���h�s���ۑ�����
//	NS_GetTransformToWorldMatrix( &MV1Man.OrigLocalWorldMatrix ) ;
	MV1Man.WorldMatrixIsIdentity = FALSE ;

	// �ݒ�̒l��ۑ�����
/*	MV1Man.BackupUseZBufferFlag = GSYS.EnableZBufferFlag ;
	MV1Man.BackupWriteZBufferFlag = GSYS.WriteZBufferFlag ;
	MV1Man.BackupUseCullingFlag = GSYS.DrawSetting.CullMode ;
	MV1Man.BackupShadeMode = D_GetShadeMode() ;
	MV1Man.BackupTexAddressMode = GSYS.TexAddressMode ;
	for( i = 0 ; i < 8 ; i ++ )
	{
		MV1Man.BackupMagFilterMode[ i ] = D_GetSampleFilterMode( i, D_D3DTSS_MAGFILTER ) ;
		MV1Man.BackupMinFilterMode[ i ] = D_GetSampleFilterMode( i, D_D3DTSS_MINFILTER ) ;
		MV1Man.BackupMipFilterMode[ i ] = D_GetSampleFilterMode( i, D_D3DTSS_MIPFILTER ) ;
	}
*/
	// �r�M���V�[��
	Graphics_D3D9_BeginScene() ;

	// �y�o�b�t�@�ݒ�𔽉f����
	Graphics_D3D9_DeviceState_SetZEnable( Model->EnableZBufferFlag ) ;
	Graphics_D3D9_DeviceState_SetZWriteEnable( Model->WriteZBufferFlag ) ;
	Graphics_D3D9_DeviceState_SetZFunc( Model->ZBufferCmpType ) ;
	Graphics_D3D9_DeviceState_SetDepthBias( Model->ZBias ) ;

	// �V�F�[�f�B���O���[�h�̓O���[�V�F�[�f�B���O
	Graphics_D3D9_DeviceState_SetShadeMode( D_D3DSHADE_GOURAUD ) ;

	// �A���t�@�e�X�g�̐ݒ肪����ꍇ�͐ݒ肪���f����Ă��Ȃ�������l��ݒ肷��
/*	if( GSYS.DrawSetting.AlphaTestMode != GD3D9.Device.DrawSetting.AlphaTestMode ||
		GSYS.DrawSetting.AlphaTestParam != GD3D9.Device.DrawSetting.AlphaTestParam )
	{
		Graphics_D3D9_DrawSetting_SetDrawAlphaTest( GSYS.DrawSetting.AlphaTestMode, GSYS.DrawSetting.AlphaTestParam ) ;
	}
*/

	// �t�H�O�̐ݒ�𔽉f����
	Graphics_DrawSetting_ApplyLibFogToHardware() ;

	// �ٕ����t�B���^�����O�̍ő原����ݒ肷��
	for( i = 0 ; i < 3 ; i ++ )
	{
		Graphics_D3D9_DeviceState_SetMaxAnisotropy( Model->MaxAnisotropy, i ) ;
	}

	// ���C���[�t���[���`�悷�邩�ǂ�����ݒ肷��
	Graphics_D3D9_DeviceState_SetFillMode( Model->WireFrame ? D_D3DFILL_WIREFRAME : D_D3DFILL_SOLID ) ;

	// �S�ẴT���v���͂Ƃ肠�������j�A�t�B���^�����O
/*	for( i = 0 ; i < 3 ; i ++ )
	{
		Graphics_D3D9_DeviceState_SetSampleFilterMode( i, D_D3DTSS_MAGFILTER, ( D_D3DTEXTUREFILTERTYPE )Model->SampleFilterMode ) ;
		Graphics_D3D9_DeviceState_SetSampleFilterMode( i, D_D3DTSS_MINFILTER, ( D_D3DTEXTUREFILTERTYPE )Model->SampleFilterMode ) ;
		Graphics_D3D9_DeviceState_SetSampleFilterMode( i, D_D3DTSS_MIPFILTER, ( D_D3DTEXTUREFILTERTYPE )Model->SampleFilterMode ) ;
	}
	GSYS.DrawSetting.DrawMode = -1 ;
*/

	GSYS.DrawSetting.MatchHardwareWorldMatrix = FALSE ;
	GSYS.DrawSetting.MatchHardwareMatrix = FALSE ;

	GD3D9.Device.State.DrawMode = DX_DRAWMODE_OTHER ;
	GD3D9.Device.DrawSetting.AlphaTestMode = -1 ;
	GD3D9.Device.DrawSetting.AlphaTestParam = 0 ;
	GD3D9.Device.DrawSetting.DrawPrepAlwaysFlag = TRUE ;

	// �A���t�@�l�Ə�Z�����J���[�ŕ`�悷�邩�ǂ������V�F�[�_�[�ɃZ�b�g����
	{
		float Data[ 4 ] ;

		Data[ 0 ] = Model->UseDrawMulAlphaColor ? 1.0f : 0.0f ;
		Data[ 1 ] = 0.0f ;
		Data[ 2 ] = 0.0f ;
		Data[ 3 ] = 0.0f ;
		Graphics_D3D9_ShaderConstant_InfoSet_SetParam(
			&GD3D9.Device.Shader.ShaderConstantInfo,
			DX_SHADERCONSTANTTYPE_PS_FLOAT,
			DX_SHADERCONSTANTSET_LIB,
			DX_PS_CONSTF_MUL_ALPHA_COLOR,
			Data,
			1,
			TRUE
		) ;
	}

	// �I��
	return 0 ;
}

// �R�c���f���̃����_�����O�̌�n�����s��
extern	int				MV1_D3D9_EndRender_PF( void )
{
//	int i ;

	// �ݒ�̒l�����ɖ߂�
/*	NS_SetUseZBufferFlag( MV1Man.BackupUseZBufferFlag ) ;
	NS_SetWriteZBufferFlag( MV1Man.BackupWriteZBufferFlag ) ;
	NS_SetUseCullingFlag( MV1Man.BackupUseCullingFlag ) ;

	// �V�F�[�f�B���O���[�h��߂�
	Graphics_D3D9_DeviceState_SetShadeMode( MV1Man.BackupShadeMode ) ;

	// �e�N�X�`���A�h���X���[�h�����ɖ߂�
	NS_SetTextureAddressMode( MV1Man.BackupTexAddressMode ) ;

	// �S�ẴT���v����߂�
	for( i = 0 ; i < 8 ; i ++ )
	{
		Graphics_D3D9_DeviceState_SetSampleFilterMode( i, D_D3DTSS_MAGFILTER, MV1Man.BackupMagFilterMode[ i ] ) ;
		Graphics_D3D9_DeviceState_SetSampleFilterMode( i, D_D3DTSS_MINFILTER, MV1Man.BackupMinFilterMode[ i ] ) ;
		Graphics_D3D9_DeviceState_SetSampleFilterMode( i, D_D3DTSS_MIPFILTER, MV1Man.BackupMipFilterMode[ i ] ) ;
	}
*/

#ifndef DX_NON_MASK
	// �}�X�N����
	if( MASKD.MaskValidFlag )
	{
//		RECT MaskRect ;
//		MaskRect.left   = GSYS.DrawSetting.DrawArea.left ;
//		MaskRect.right  = GSYS.DrawSetting.DrawArea.right ;
//		MaskRect.top    = GSYS.DrawSetting.DrawArea.top ;
//		MaskRect.bottom = GSYS.DrawSetting.DrawArea.bottom ;
//		MASK_END( MaskRect )
	}
#endif

	// ���X�ݒ肳��Ă������[���h���r���[�g�����X�t�H�[���s������ɖ߂�
//	NS_SetTransformToWorld( &MV1Man.OrigLocalWorldMatrix ) ;

	// �I��
	return 0 ;
}

// ���b�V���`�敔���𔲂��o��������
extern	void			MV1_D3D9_DrawMesh_PF( MV1_MESH *Mesh, int TriangleListIndex )
{
//	MV1_TEXTURE                *     Tex ;
	MV1_TRIANGLE_LIST          * RST TList ;
	MV1_TRIANGLE_LIST_BASE     * RST MBTList ;
	MV1_TRIANGLE_LIST_BASE_PF  *     MBTListPF ;
	MV1_VERTEX_SIMPLE_D3D9     * RST SMPVert ;
	MV1_VERTEX_SIMPLE_TOL_D3D9 * RST TONVert ;
	MV1_SKINBONE_BLEND         * RST VBlend ;
	MV1_MESH_BASE              * RST MBMesh ;
	MV1_MODEL_BASE             * RST MBase ;
	MV1_MODEL                  * RST Model ;
	MV1_FRAME                  * RST Frame ;
	MV1_FRAME_BASE             * RST MBFrame ;
	MV1_TLIST_SKIN_POS_4B      * RST PosSK4B ;
	MV1_TLIST_SKIN_POS_8B      * RST PosSK8B ;
	MV1_TLIST_SKIN_POS_FREEB   * RST PosSKFB ;
	MATRIX_4X4CT                     BlendMat ;
	MATRIX_4X4CT               * RST Mat ;
	MATRIX_4X4CT               * RST pSkinBoneMatrix[ MV1_TRIANGLE_LIST_USE_BONE_MAX_NUM ] ;
	int                              i ;
	int                              j ;
	int                              k ;
	int                              MaxBoneNum ;
	int                              VertexNum ;
	DWORD                            SrcUnitSize ;
	int                              DestUnitSize ;
	float                            Weight ;
//	MATRIX                           Tenti ;
	int                              NowVertexShaderIndex ;
	int                              NowPixelShaderIndex ;
	int                              VertexShaderIndex ;
	int                              PixelShaderIndex ;
	int                              NowVertexShaderIndex_PL ;
	int                              NowPixelShaderIndex_PL ;
	int                              VertexShaderIndex_PL ;
	int                              PixelShaderIndex_PL ;
	GRAPHICS_HARDDATA_DIRECT3D9_BLENDINFO              BlendInfo ;
//	int                              Shader = NS_GetNowCount() % 2000 > 1000 ? 1 : 0 ;
	int                              SetupShaderMaterial ;
	int                              SpecularEnable ;
	MATRIX_4X4CT_F                   TempMatrixTable[ MV1_TRIANGLE_LIST_USE_BONE_MAX_NUM ] ;
	MATRIX_4X4CT_D                   TempMatrixTableD[ MV1_TRIANGLE_LIST_USE_BONE_MAX_NUM ] ;
	bool                             IsScaling ;
	float                            Scale ;
	int                              BumpMap ;
	int                              SkinMesh ;
	int                              UVNum ;
	int                              TOutLine ;
	int                              UseWorldViewMatrix ;

	Frame   = Mesh->Container ;
	Model   = Frame->Container ;
	MBFrame = Frame->BaseData ;
	MBase   = Model->BaseData ;
	MBMesh  = Mesh->BaseData ;

	// �`��f�o�C�X�������������牽�����Ȃ�
	if( Graphics_Hardware_CheckValid_PF() == 0 ) return ;

	UseWorldViewMatrix = FALSE ;
	if( GSYS.DrawSetting.Large3DPositionSupport )
	{
		for( i = 0 ; i < MAX_USE_SHADOWMAP_NUM ; i ++ )
		{
			if( GSYS.DrawSetting.ShadowMap[ i ] != 0 )
			{
				break ;
			}
		}
		if( i != MAX_USE_SHADOWMAP_NUM )
		{
			UseWorldViewMatrix = TRUE ;
		}
	}

	VertexShaderIndex    = GD3D9.Device.Shader.UseBaseVertexShaderIndex ;
	PixelShaderIndex     = GD3D9.Device.Shader.UseBasePixelShaderIndex ;
	VertexShaderIndex_PL = GD3D9.Device.Shader.UseBaseVertexShaderIndex_PL ;
	PixelShaderIndex_PL  = GD3D9.Device.Shader.UseBasePixelShaderIndex_PL ;

	// �}�e���A���̋��ʕ����̃Z�b�g�A�b�v
	if( MV1_D3D9_SetupMeshDrawMaterialCommon(
			&VertexShaderIndex,    &PixelShaderIndex,
			&VertexShaderIndex_PL, &PixelShaderIndex_PL,
			&BlendInfo, Mesh, &SpecularEnable ) == false )
			return ;

	{
		int type, sv, sdm, sk, bp, fg, lind, spc ;

		type = D3D9_VERTEXSHADER_GET_TYPE( VertexShaderIndex ) ;
		sv   = D3D9_VERTEXSHADER_GET_SHADERMODEL( VertexShaderIndex ) ;
		sdm  = D3D9_VERTEXSHADER_GET_SHADOWMAP( VertexShaderIndex ) ;
		sk   = D3D9_VERTEXSHADER_GET_MESHTYPE( VertexShaderIndex ) ;
		bp   = D3D9_VERTEXSHADER_GET_BUMPMAP( VertexShaderIndex ) ;
		fg   = D3D9_VERTEXSHADER_GET_FOGMODE( VertexShaderIndex ) ;
		lind = D3D9_VERTEXSHADER_GET_LIGHTINDEX( VertexShaderIndex ) ;
		spc  = D3D9_VERTEXSHADER_GET_SPECULAR( VertexShaderIndex ) ;
		type = 0 ;
	}

	{
		int type, sv, sdm, mtx, toon, ttype, tspop, tdbop, tsbop, sm, bp, lind, spc ;

		type  = D3D9_PIXELSHADER_GET_TYPE( PixelShaderIndex ) ;
		sv    = D3D9_PIXELSHADER_GET_SHADERMODEL( PixelShaderIndex ) ;
		sdm   = D3D9_PIXELSHADER_GET_SHADOWMAP( PixelShaderIndex ) ;
		mtx   = D3D9_PIXELSHADER_GET_MULTITEX( PixelShaderIndex ) ;
		toon  = D3D9_PIXELSHADER_GET_TOON( PixelShaderIndex ) ;
		ttype = D3D9_PIXELSHADER_GET_TOONTYPE( PixelShaderIndex ) ;
		tspop = D3D9_PIXELSHADER_GET_TOONSPHEREOP( PixelShaderIndex ) ;
		tdbop = D3D9_PIXELSHADER_GET_TOONDIFBLDOP( PixelShaderIndex ) ;
		tsbop = D3D9_PIXELSHADER_GET_TOONSPCBLDOP( PixelShaderIndex ) ;
		sm    = D3D9_PIXELSHADER_GET_SPECULARMAP( PixelShaderIndex ) ;
		bp    = D3D9_PIXELSHADER_GET_BUMPMAP( PixelShaderIndex ) ;
		lind  = D3D9_PIXELSHADER_GET_LIGHTINDEX( PixelShaderIndex ) ;
		spc   = D3D9_PIXELSHADER_GET_SPECULAR( PixelShaderIndex ) ;
	}

	{
		int sdm, sk, bp, fg ;

		sdm = D3D9_PIXELLIGHTING_VERTEXSHADER_GET_SHADOWMAP( VertexShaderIndex_PL ) ;
		sk  = D3D9_PIXELLIGHTING_VERTEXSHADER_GET_SKINMESH( VertexShaderIndex_PL ) ;
		bp  = D3D9_PIXELLIGHTING_VERTEXSHADER_GET_BUMPMAP( VertexShaderIndex_PL ) ;
		fg  = D3D9_PIXELLIGHTING_VERTEXSHADER_GET_FOGMODE( VertexShaderIndex_PL ) ;
	}

	{
		int type, sdm, mtx, ttype, tspop, tdbop, tsbop, sm, bp, lind, spc ;

		type  = D3D9_PIXELLIGHTING_PIXELSHADER_GET_TYPE( PixelShaderIndex_PL ) ;
		sdm   = D3D9_PIXELLIGHTING_PIXELSHADER_GET_SHADOWMAP( PixelShaderIndex_PL ) ;
		mtx   = D3D9_PIXELLIGHTING_PIXELSHADER_GET_MULTITEX( PixelShaderIndex_PL ) ;
		ttype = D3D9_PIXELLIGHTING_PIXELSHADER_GET_TOONTYPE( PixelShaderIndex_PL ) ;
		tspop = D3D9_PIXELLIGHTING_PIXELSHADER_GET_TOONSPHEREOP( PixelShaderIndex_PL ) ;
		tdbop = D3D9_PIXELLIGHTING_PIXELSHADER_GET_TOONDIFBLDOP( PixelShaderIndex_PL ) ;
		tsbop = D3D9_PIXELLIGHTING_PIXELSHADER_GET_TOONSPCBLDOP( PixelShaderIndex_PL ) ;
		sm    = D3D9_PIXELLIGHTING_PIXELSHADER_GET_SPECULARMAP( PixelShaderIndex_PL ) ;
		bp    = D3D9_PIXELLIGHTING_PIXELSHADER_GET_BUMPMAP( PixelShaderIndex_PL ) ;
		lind  = D3D9_PIXELLIGHTING_PIXELSHADER_GET_LIGHTINDEX( PixelShaderIndex_PL ) ;
		spc   = D3D9_PIXELLIGHTING_PIXELSHADER_GET_SPECULAR( PixelShaderIndex_PL ) ;
	}

	// �ʏ탁�b�V���̔�\���t���O�������Ă�����`�悵�Ȃ�
	if( Model->MeshCategoryHide[ DX_MV1_MESHCATEGORY_NORMAL ] == FALSE )
	{
		// �g���C�A���O�����X�g�̐������J��Ԃ�
		TList = Mesh->TriangleList ;
		MBTList = MBMesh->TriangleList ;
		SetupShaderMaterial = -1 ;
		for( i = 0 ; i < MBMesh->TriangleListNum ; i ++ , MBTList ++, TList = ( MV1_TRIANGLE_LIST * )( ( BYTE * )TList + sizeof( MV1_TRIANGLE_LIST ) + sizeof( MV1_TRIANGLE_LIST_PF ) ) )
		{
			MBTListPF = ( MV1_TRIANGLE_LIST_BASE_PF * )MBTList->PFBuffer ;

			// �g���C�A���O�����X�g�̎w�肪����ꍇ�͂���ȊO�̃g���C�A���O�����X�g�͕`�悵�Ȃ�
			if( TriangleListIndex >= 0 && i != TriangleListIndex )
				continue ;

			// ���_�̃^�C�v�ɂ���ď����𕪊�
			NowVertexShaderIndex = VertexShaderIndex ;
			NowPixelShaderIndex = PixelShaderIndex ;

			NowVertexShaderIndex_PL = VertexShaderIndex_PL ;
			NowPixelShaderIndex_PL = PixelShaderIndex_PL ;
			switch( MBTList->VertexType )
			{
				// ���̃g���C�A���O�����X�g
			case MV1_VERTEX_TYPE_NORMAL :
				if( 1 )
	//			if( Shader )
	//			if( 0 )
				{
	//				MATRIX BlendMat2, BlendMat3 ;

					// �V�F�[�_�[���g�p���ĕ`��

					if( MBTList->VertexBuffer == NULL || MBTList->VertexBuffer->PF->D3D9.VertexBuffer == NULL || MBTList->VertexBuffer->PF->D3D9.IndexBuffer == NULL )
						goto NONSDSIMPLE ;

					// ���������`��̏ꍇ�͂S�{�[���X�L�j���O���b�V���Ƃ��ĕ`�悷��
					if( MBase->UsePackDraw )
					{
						goto SD4BONESKIN ;
					}

					// ���[�U�[�ݒ���g�p����ꍇ
					if( MV1Man.UseOrigShaderFlag )
					{
						// ���_�V�F�[�_�[�̃Z�b�g�A�b�v
						if( GSYS.DrawSetting.UserShaderRenderInfo.SetVertexShaderHandle > 0 )
						{
							if( Graphics_D3D9_DeviceState_SetVertexShaderToHandle( GSYS.DrawSetting.UserShaderRenderInfo.SetVertexShaderHandle ) < 0 )
								goto NONSDSIMPLE ;
						}

						// �s�N�Z���V�F�[�_�[�̃Z�b�g�A�b�v
						if( GSYS.DrawSetting.UserShaderRenderInfo.SetPixelShaderHandle > 0 )
						{
							if( Graphics_D3D9_DeviceState_SetPixelShaderToHandle( GSYS.DrawSetting.UserShaderRenderInfo.SetPixelShaderHandle ) < 0 )
								goto NONSDSIMPLE ;
						}

						// �V�F�[�_�[�̃Z�b�g�A�b�v
						if( Graphics_D3D9_Shader_Model_Setup( 
							GSYS.DrawSetting.UserShaderRenderInfo.SetVertexShaderHandle > 0 ? -1 : NowVertexShaderIndex, NowVertexShaderIndex_PL,
							GSYS.DrawSetting.UserShaderRenderInfo.SetPixelShaderHandle  > 0 ? -1 : NowPixelShaderIndex,  NowPixelShaderIndex_PL ) == FALSE )
						{
							goto NONSDSIMPLE ;
						}
					}
					else
					{
						// �f�t�H���g����

						// �V�F�[�_�[�̃Z�b�g�A�b�v
						if( Graphics_D3D9_Shader_Model_Setup( 
								NowVertexShaderIndex, NowVertexShaderIndex_PL,
								NowPixelShaderIndex,  NowPixelShaderIndex_PL ) == FALSE )
						{
							goto NONSDSIMPLE ;
						}
					}

					// ���_�f�[�^�̃Z�b�g�A�b�v
					TOutLine = 0 ;

					BumpMap = D3D9_VERTEXSHADER_GET_BUMPMAP( NowVertexShaderIndex ) ;
					SkinMesh = D3D9_VERTEXSHADER_GET_MESHTYPE( NowVertexShaderIndex ) ;
/*
					TOutLine = VS_GET_TOONOUTLINE( NowVertexShaderIndex ) ;
					TOutLine = 0 ;

					BumpMap = VS_GET_BUMPMAP( NowVertexShaderIndex ) ;
					SkinMesh = VS_GET_SKINMESH( NowVertexShaderIndex ) ;
*/
					if( TOutLine == 1 )
					{
						UVNum = 0 ;
					}
					else
					{
						UVNum = Mesh->Material->DiffuseLayerNum >= 2 ? 2 : 1 ;
					}
					Graphics_D3D9_DeviceState_SetMV1VertexDeclaration( BumpMap, SkinMesh, UVNum ) ;

					// ���_�o�b�t�@�̃Z�b�g�A�b�v
					if( TList->PF->D3D9.VertexBuffer )
					{
						Graphics_D3D9_DeviceState_SetVertexBuffer( TList->PF->D3D9.VertexBuffer,                 ( int )MBTList->VertexBuffer->UnitSize ) ;
					}
					else
					{
						Graphics_D3D9_DeviceState_SetVertexBuffer( MBTList->VertexBuffer->PF->D3D9.VertexBuffer, ( int )MBTList->VertexBuffer->UnitSize ) ;
					}

					// �}�e���A���̃Z�b�g�A�b�v
					if( SetupShaderMaterial != 1 )
					{
						MV1_D3D9_SetupMeshDrawMaterial( MBase, &BlendInfo, Mesh, true, SpecularEnable ) ;
						SetupShaderMaterial = 1 ;
					}

					// �C���f�b�N�X�o�b�t�@�̃Z�b�g�A�b�v
					Graphics_D3D9_DeviceState_SetIndexBuffer( MBTList->VertexBuffer->PF->D3D9.IndexBuffer ) ;

					// �g�p���郍�[�J�������[���h�s��̗p��
					if( GSYS.DrawSetting.Large3DPositionSupport )
					{
						if( UseWorldViewMatrix )
						{
							ConvertMatrix4x4cDToMatrix4x4cF( &TempMatrixTable[ 0 ], &Frame->LocalWorldMatrix.md ) ;
						}
						else
						{
							MATRIX_4X4CT_D ViewMat ;

							ConvertMatrixDToMatrix4x4cD( &ViewMat, &GSYS.DrawSetting.ViewMatrix ) ;
							UnSafeMultiplyMatrix4X4CTD( &TempMatrixTableD[ 0 ], &Frame->LocalWorldMatrix.md, &ViewMat ) ;
							ConvertMatrix4x4cDToMatrix4x4cF( &TempMatrixTable[ 0 ], &TempMatrixTableD[ 0 ] ) ;

							Graphics_D3D9_DeviceState_SetViewMatrix( &IdentityMat ) ;
						}

						Graphics_D3D9_ShaderConstant_InfoSet_SetParam(
							&GD3D9.Device.Shader.ShaderConstantInfo,
							DX_SHADERCONSTANTTYPE_VS_FLOAT,
							DX_SHADERCONSTANTSET_LIB,
							DX_VS_CONSTF_WORLD_MAT_START,
							&TempMatrixTable[ 0 ],
							3,
							FALSE
						) ;
					}
					else
					{
						Graphics_D3D9_ShaderConstant_InfoSet_SetParam(
							&GD3D9.Device.Shader.ShaderConstantInfo,
							DX_SHADERCONSTANTTYPE_VS_FLOAT,
							DX_SHADERCONSTANTSET_LIB,
							DX_VS_CONSTF_WORLD_MAT_START,
							&Frame->LocalWorldMatrix,
							3,
							FALSE
						) ;
					}

#ifndef NOT_RENDER
					// �`��
					Direct3DDevice9_DrawIndexedPrimitive(
						D_D3DPT_TRIANGLELIST,
						TList->PF->D3D9.VertexBuffer ? NULL : MBTList->VBStartVertex,
						0,
						( UINT )MBTList->VertexNum,
						( UINT )MBTList->VBStartIndex,
						( UINT )( MBTList->IndexNum / 3 ) ) ;
#endif // NOT_RENDER

					if( GSYS.DrawSetting.Large3DPositionSupport && UseWorldViewMatrix == FALSE )
					{
						Graphics_D3D9_DeviceState_SetViewMatrix( &GSYS.DrawSetting.ViewMatrixF ) ;
					}
				}
				else
				{
	NONSDSIMPLE :
					if( Frame->ValidLocalWorldMatrixNM == false )
					{
						Frame->ValidLocalWorldMatrixNM = true ;
						ConvertMatrix4x4cToMatrixF( &Frame->LocalWorldMatrixNM, &Frame->LocalWorldMatrix ) ;
	//					CreateTransposeMatrix( &Frame->LocalWorldMatrixNM, &Frame->LocalWorldMatrix ) ;
					}

					// �s��̃Z�b�g
					Graphics_DrawSetting_SetTransformToWorld_Direct( &Frame->LocalWorldMatrixNM ) ;
					MV1Man.WorldMatrixIsIdentity = FALSE ;

					// �}�e���A���̃Z�b�g�A�b�v
					if( SetupShaderMaterial != 0 )
					{
						MV1_D3D9_SetupMeshDrawMaterial( MBase, &BlendInfo, Mesh, false, SpecularEnable ) ;
						SetupShaderMaterial = 0 ;
					}

					// �Œ�@�\�p�C�v���C���ɖ߂�
					Graphics_D3D9_DeviceState_ResetVertexShader() ;
					Graphics_D3D9_DeviceState_ResetPixelShader() ;
#ifndef NOT_RENDER

					// �e���|�����o�b�t�@���Z�b�g�A�b�v����Ă��Ȃ�������Z�b�g�A�b�v
					if( MBTListPF->D3D9.TempSimpleVertex == NULL )
						MV1_D3D9_SetupTListTempSimpleVertex( MBTList ) ;

					// �V�F�C�v���b�V���������ꍇ�͂����Ńe���|�����o�b�t�@�ɓ��e����������
					if( MBMesh->Shape )
					{
						MV1_TLIST_NORMAL_POS * RST Norm ;

						// �P���_������̃f�[�^�T�C�Y���Z�o
						DestUnitSize = MBTList->TempUnitSize ;

						// �f�[�^�̃Z�b�g
						SMPVert   = MBTListPF->D3D9.TempSimpleVertex ;
						VertexNum = MBTList->VertexNum ;
						Norm      = ( MV1_TLIST_NORMAL_POS * )ADDR16( TList->NormalPosition ) ;

						for( j = 0 ; j < VertexNum ; j ++, Norm ++, SMPVert = ( MV1_VERTEX_SIMPLE_D3D9  * )( ( BYTE * )SMPVert + DestUnitSize ) )
						{
							SMPVert->Position.x = Norm->Position.x ;
							SMPVert->Position.y = Norm->Position.y ;
							SMPVert->Position.z = Norm->Position.z ;
						}
					}

					// �g�D�[���������ꍇ�̓��C�e�B���O�v�Z�������ł���
					if( Mesh->Material->BaseData->Type == DX_MATERIAL_TYPE_TOON || Mesh->Material->BaseData->Type == DX_MATERIAL_TYPE_TOON_2 )
					{
						// �X�P�[�����O����邩�ǂ������`�F�b�N
						IsScaling = false ;
						Scale = Frame->LocalWorldMatrixNM.m[ 0 ][ 0 ] * Frame->LocalWorldMatrixNM.m[ 0 ][ 0 ] +
								Frame->LocalWorldMatrixNM.m[ 1 ][ 0 ] * Frame->LocalWorldMatrixNM.m[ 1 ][ 0 ] +
								Frame->LocalWorldMatrixNM.m[ 2 ][ 0 ] * Frame->LocalWorldMatrixNM.m[ 2 ][ 0 ] ;
						if( Scale < 0.999f || Scale > 1.001 ) IsScaling = true ;

						Scale = Frame->LocalWorldMatrixNM.m[ 0 ][ 1 ] * Frame->LocalWorldMatrixNM.m[ 0 ][ 1 ] +
								Frame->LocalWorldMatrixNM.m[ 1 ][ 1 ] * Frame->LocalWorldMatrixNM.m[ 1 ][ 1 ] +
								Frame->LocalWorldMatrixNM.m[ 2 ][ 1 ] * Frame->LocalWorldMatrixNM.m[ 2 ][ 1 ] ;
						if( Scale < 0.999f || Scale > 1.001 ) IsScaling = true ;

						Scale = Frame->LocalWorldMatrixNM.m[ 0 ][ 2 ] * Frame->LocalWorldMatrixNM.m[ 0 ][ 2 ] +
								Frame->LocalWorldMatrixNM.m[ 1 ][ 2 ] * Frame->LocalWorldMatrixNM.m[ 1 ][ 2 ] +
								Frame->LocalWorldMatrixNM.m[ 2 ][ 2 ] * Frame->LocalWorldMatrixNM.m[ 2 ][ 2 ] ;
						if( Scale < 0.999f || Scale > 1.001 ) IsScaling = true ;

						MV1_D3D9_TriangleListToonLighting( MBTList, IsScaling, &Frame->LocalWorldMatrixNM ) ;
					}

					Graphics_D3D9_DeviceState_SetVertexShader( NULL ) ;
					Graphics_D3D9_DeviceState_SetPixelShader( NULL ) ;
					Graphics_D3D9_DeviceState_SetIndexBuffer( NULL ) ;
					Graphics_D3D9_DeviceState_SetVertexBuffer( NULL ) ;
					Graphics_D3D9_DeviceState_SetFVF( ( int )( D_D3DFVF_XYZ | D_D3DFVF_NORMAL | D_D3DFVF_DIFFUSE | D_D3DFVF_SPECULAR | UVNumFVFTable[ MBMesh->UVSetUnitNum + 2 ] ) ) ;

					Direct3DDevice9_DrawIndexedPrimitiveUP(
						D_D3DPT_TRIANGLELIST,
						0,
						( UINT )MBTList->VertexNum,
						( UINT )( MBTList->IndexNum / 3 ),
						MBTList->Index,
						D_D3DFMT_INDEX16,
						MBTListPF->D3D9.TempSimpleVertex,
						( UINT )MBTList->TempUnitSize ) ;
#endif // NOT_RENDER
				}
				break ;

				// �S�{�[���ȓ��g���C�A���O�����X�g�ƂW�{�[���ȓ��g���C�A���O�����X�g
			case MV1_VERTEX_TYPE_SKIN_4BONE :
			case MV1_VERTEX_TYPE_SKIN_8BONE :
SD4BONESKIN:

				if( MBTList->VertexType == MV1_VERTEX_TYPE_SKIN_8BONE )
				{
					NowVertexShaderIndex    += D3D9_VERTEXSHADER_MESHTYPE( 2 ) ;
					NowVertexShaderIndex_PL += D3D9_PIXELLIGHTING_VERTEXSHADER_SKINMESH( 2 ) ;
				}
				else
				{
					NowVertexShaderIndex    += D3D9_VERTEXSHADER_MESHTYPE( 1 ) ;
					NowVertexShaderIndex_PL += D3D9_PIXELLIGHTING_VERTEXSHADER_SKINMESH( 1 ) ;
				}
	//			if( 0 )
				if( 1 )
	//			if( Shader )
				{
					int n ;
	//				D_IDirect3DVertexShader9 *UseVS ;

					// �V�F�[�_�[���g�p���ĕ`��

					if( MBTList->VertexBuffer == NULL || MBTList->VertexBuffer->PF->D3D9.VertexBuffer == NULL || MBTList->VertexBuffer->PF->D3D9.IndexBuffer == NULL )
						goto NONSDSKIN ;

					// ���[�U�[�ݒ���g�p����ꍇ
					if( MV1Man.UseOrigShaderFlag )
					{
						// ���_�V�F�[�_�[�̃Z�b�g�A�b�v
						if( GSYS.DrawSetting.UserShaderRenderInfo.SetVertexShaderHandle > 0 )
						{
							if( Graphics_D3D9_DeviceState_SetVertexShaderToHandle( GSYS.DrawSetting.UserShaderRenderInfo.SetVertexShaderHandle ) < 0 )
								goto NONSDSKIN ;
						}

						// �s�N�Z���V�F�[�_�[�̃Z�b�g�A�b�v
						if( GSYS.DrawSetting.UserShaderRenderInfo.SetPixelShaderHandle > 0 )
						{
							if( Graphics_D3D9_DeviceState_SetPixelShaderToHandle( GSYS.DrawSetting.UserShaderRenderInfo.SetPixelShaderHandle ) < 0 )
								goto NONSDSKIN ;
						}

						// �V�F�[�_�[�̃Z�b�g�A�b�v
						if( Graphics_D3D9_Shader_Model_Setup( 
							GSYS.DrawSetting.UserShaderRenderInfo.SetVertexShaderHandle > 0 ? -1 : NowVertexShaderIndex, NowVertexShaderIndex_PL,
							GSYS.DrawSetting.UserShaderRenderInfo.SetPixelShaderHandle  > 0 ? -1 : NowPixelShaderIndex,  NowPixelShaderIndex_PL ) == FALSE )
						{
							goto NONSDSKIN ;
						}
					}
					else
					{
						// �f�t�H���g����

						// �V�F�[�_�[�̃Z�b�g�A�b�v
						if( Graphics_D3D9_Shader_Model_Setup( 
								NowVertexShaderIndex, NowVertexShaderIndex_PL,
								NowPixelShaderIndex,  NowPixelShaderIndex_PL ) == FALSE )
						{
							goto NONSDSKIN ;
						}
					}

					// ���_�f�[�^�̃Z�b�g�A�b�v
					TOutLine = 0 ;

					BumpMap  = D3D9_VERTEXSHADER_GET_BUMPMAP( NowVertexShaderIndex ) ;
					SkinMesh = D3D9_VERTEXSHADER_GET_MESHTYPE( NowVertexShaderIndex ) ;
/*
					TOutLine = VS_GET_TOONOUTLINE( NowVertexShaderIndex ) ;
					TOutLine = 0 ;

					BumpMap = VS_GET_BUMPMAP( NowVertexShaderIndex ) ;
					SkinMesh = VS_GET_SKINMESH( NowVertexShaderIndex ) ;
*/

					if( TOutLine == 1 )
					{
						UVNum = 0 ;
					}
					else
					{
						UVNum = Mesh->Material->DiffuseLayerNum >= 2 ? 2 : 1 ;
					}
					Graphics_D3D9_DeviceState_SetMV1VertexDeclaration( BumpMap, SkinMesh, UVNum ) ;

					// �}�e���A���̃Z�b�g�A�b�v
					if( SetupShaderMaterial != 1 )
					{
						MV1_D3D9_SetupMeshDrawMaterial( MBase, &BlendInfo, Mesh, true, SpecularEnable ) ;
						SetupShaderMaterial = 1 ;
					}

					// ���_�o�b�t�@�̃Z�b�g�A�b�v
					if( TList->PF->D3D9.VertexBuffer )
					{
						Graphics_D3D9_DeviceState_SetVertexBuffer( TList->PF->D3D9.VertexBuffer,                 ( int )MBTList->VertexBuffer->UnitSize ) ;
					}
					else
					{
						Graphics_D3D9_DeviceState_SetVertexBuffer( MBTList->VertexBuffer->PF->D3D9.VertexBuffer, ( int )MBTList->VertexBuffer->UnitSize ) ;
					}

					// �C���f�b�N�X�o�b�t�@�̃Z�b�g�A�b�v
					Graphics_D3D9_DeviceState_SetIndexBuffer( MBTList->VertexBuffer->PF->D3D9.IndexBuffer ) ;

					// ���������`�悩�ǂ����ŏ����𕪊�
					if( MBase->UsePackDraw )
					{
						int l ;
						int UsePackIndex ;
						int TotalDrawStockNum ;
						int DrawStockNum ;
						MATRIX_4X4CT_F *PackDrawMatrix ;

						if( MBTList->VertexType == MV1_VERTEX_TYPE_NORMAL )
						{
							PackDrawMatrix = Model->PackDrawMatrix + MBase->SkinBoneNum + ( Frame - Model->Frame ) ;
						}
						else
						{
							PackDrawMatrix = Model->PackDrawMatrix ;
						}

						UsePackIndex = 0 ;
						for( TotalDrawStockNum = Model->PackDrawStockNum ; TotalDrawStockNum > 0 ; )
						{
							DrawStockNum = TotalDrawStockNum ;
							if( DrawStockNum > MBTList->PackDrawMaxNum )
							{
								DrawStockNum = MBTList->PackDrawMaxNum ;
							}

							if( MBTList->VertexType == MV1_VERTEX_TYPE_NORMAL )
							{
								for( l = 0 ; l < DrawStockNum ; l ++, PackDrawMatrix += MBase->PackDrawMatrixUnitNum )
								{
									TempMatrixTable[ l ] = *PackDrawMatrix ;
								}
							}
							else
							{
								int MatrixIndex ;

								MatrixIndex = 0 ;
								for( l = 0 ; l < DrawStockNum ; l ++, PackDrawMatrix += MBase->PackDrawMatrixUnitNum )
								{
									for( n = 0 ; n < MBTList->UseBoneNum ; n ++, MatrixIndex ++ )
									{
										TempMatrixTable[ MatrixIndex ] = PackDrawMatrix[ Frame->UseSkinBoneMatrix[ MBTList->UseBone[ n ] ] - Model->SkinBoneMatrix ] ;
									}
								}
							}

							Graphics_D3D9_ShaderConstant_InfoSet_SetParam(
								&GD3D9.Device.Shader.ShaderConstantInfo,
								DX_SHADERCONSTANTTYPE_VS_FLOAT,
								DX_SHADERCONSTANTSET_LIB,
								DX_VS_CONSTF_WORLD_MAT_START,
								TempMatrixTable,
								( MBTList->VertexType == MV1_VERTEX_TYPE_NORMAL ? 1 : MBTList->UseBoneNum ) * DrawStockNum * 3,
								FALSE
							) ;
#ifndef NOT_RENDER
							Direct3DDevice9_DrawIndexedPrimitive(
								D_D3DPT_TRIANGLELIST,
								TList->PF->D3D9.VertexBuffer ? NULL : MBTList->VBStartVertex,
								0,
								( UINT )( MBTList->VertexNum * DrawStockNum ),
								( UINT )MBTList->VBStartIndex,
								( UINT )( MBTList->IndexNum / 3 * DrawStockNum ) ) ;
#endif

							TotalDrawStockNum -= DrawStockNum ;
							UsePackIndex      += DrawStockNum ;
						}
					}
					else
					{
						if( GSYS.DrawSetting.Large3DPositionSupport )
						{
							MATRIX_4X4CT_D ViewMat ;

							if( UseWorldViewMatrix )
							{
								for( n = 0 ; n < MBTList->UseBoneNum ; n ++ )
								{
									ConvertMatrix4x4cDToMatrix4x4cF( &TempMatrixTable[ n ], &Frame->UseSkinBoneMatrix[ MBTList->UseBone[ n ] ]->md ) ;
								}
							}
							else
							{
								ConvertMatrixDToMatrix4x4cD( &ViewMat, &GSYS.DrawSetting.ViewMatrix ) ;
								for( n = 0 ; n < MBTList->UseBoneNum ; n ++ )
								{
									UnSafeMultiplyMatrix4X4CTD( &TempMatrixTableD[ n ], &Frame->UseSkinBoneMatrix[ MBTList->UseBone[ n ] ]->md, &ViewMat )  ;
									ConvertMatrix4x4cDToMatrix4x4cF( &TempMatrixTable[ n ], &TempMatrixTableD[ n ] ) ;
								}

								Graphics_D3D9_DeviceState_SetViewMatrix( &IdentityMat ) ;
							}
						}
						else
						{
							for( n = 0 ; n < MBTList->UseBoneNum ; n ++ )
							{
								TempMatrixTable[ n ] = Frame->UseSkinBoneMatrix[ MBTList->UseBone[ n ] ]->mf ;
							}
						}
						Graphics_D3D9_ShaderConstant_InfoSet_SetParam(
							&GD3D9.Device.Shader.ShaderConstantInfo,
							DX_SHADERCONSTANTTYPE_VS_FLOAT,
							DX_SHADERCONSTANTSET_LIB,
							DX_VS_CONSTF_WORLD_MAT_START,
							TempMatrixTable,
							MBTList->UseBoneNum * 3,
							FALSE
						) ;
#ifndef NOT_RENDER
						Direct3DDevice9_DrawIndexedPrimitive(
							D_D3DPT_TRIANGLELIST,
							TList->PF->D3D9.VertexBuffer ? NULL : MBTList->VBStartVertex,
							0,
							( UINT )MBTList->VertexNum,
							( UINT )MBTList->VBStartIndex,
							( UINT )( MBTList->IndexNum / 3 ) ) ;
#endif
						if( GSYS.DrawSetting.Large3DPositionSupport && UseWorldViewMatrix == FALSE )
						{
							Graphics_D3D9_DeviceState_SetViewMatrix( &GSYS.DrawSetting.ViewMatrixF ) ;
						}
					}
				}
				else
				{
	NONSDSKIN:
					// �}�e���A���̃Z�b�g�A�b�v
					if( SetupShaderMaterial != 0 )
					{
						MV1_D3D9_SetupMeshDrawMaterial( MBase, &BlendInfo, Mesh, false, SpecularEnable ) ;
						SetupShaderMaterial = 0 ;
					}

					// �Œ�@�\�p�C�v���C���ɖ߂�
					Graphics_D3D9_DeviceState_ResetVertexShader() ;
					Graphics_D3D9_DeviceState_ResetPixelShader() ;

					// �P�ʍs����Z�b�g
					if( MV1Man.WorldMatrixIsIdentity == FALSE )
					{
						MV1Man.WorldMatrixIsIdentity = TRUE ;
						Graphics_DrawSetting_SetTransformToWorld_Direct( &IdentityMat ) ;
					}

					// �e���|�����o�b�t�@���Z�b�g�A�b�v����Ă��Ȃ�������Z�b�g�A�b�v
					if( MBTListPF->D3D9.TempSimpleVertex == NULL )
						MV1_D3D9_SetupTListTempSimpleVertex( MBTList ) ;

					// �P���_������̃f�[�^�T�C�Y���Z�o
					DestUnitSize = MBTList->TempUnitSize ;

					// �f�[�^�̃Z�b�g
					SMPVert    = MBTListPF->D3D9.TempSimpleVertex ;
					MaxBoneNum = MBTList->MaxBoneNum ;
					VertexNum  = MBTList->VertexNum ;

					// �g�p����s��̏������s��
					for( j = 0 ; j < MBTList->UseBoneNum ; j ++ )
					{
						pSkinBoneMatrix[ j ] = Frame->UseSkinBoneMatrix[ MBTList->UseBone[ j ] ] ;
					}

					switch( MBTList->VertexType )
					{
					case MV1_VERTEX_TYPE_SKIN_4BONE :
						PosSK4B = MBMesh->Shape ? ( MV1_TLIST_SKIN_POS_4B * )ADDR16( TList->SkinPosition4B ) : ( MV1_TLIST_SKIN_POS_4B * )ADDR16( MBTList->SkinPosition4B ) ;

#ifdef VISUALCPP_2005
#ifndef DX_NON_INLINE_ASM
						if( GSYS.DrawSetting.Large3DPositionSupport == FALSE && WinData.UseSSEFlag )
						{
							__asm
							{
								mov esi, PosSK4B
								mov edi, SMPVert
	MV1_VERTEX_TYPE_SKIN_4BONE_LOOP:
								mov eax, [ esi + 44 ]
								movaps xmm0, [ esi + 0 ]

								mov edx, eax
								mov ebx, eax
								mov ecx, eax
								shr ebx, 8
								shr ecx, 16
								shr eax, 24
								and edx, 0xff
								and ebx, 0xff
								and ecx, 0xff
								mov edx, [ pSkinBoneMatrix + edx * 4 ]
								mov ebx, [ pSkinBoneMatrix + ebx * 4 ]
								mov ecx, [ pSkinBoneMatrix + ecx * 4 ]
								mov eax, [ pSkinBoneMatrix + eax * 4 ]
								movaps xmm1, xmm0
								shufps xmm1, xmm0, 0
								movaps xmm5, [ edx ]
								movaps xmm6, [ edx + 16 ]
								movaps xmm7, [ edx + 32 ]
								mulps xmm5, xmm1
								mulps xmm6, xmm1
								mulps xmm7, xmm1

								movaps xmm1, xmm0
								shufps xmm1, xmm0, 0x55
								movaps xmm2, [ ebx ]
								movaps xmm3, [ ebx + 16 ]
								movaps xmm4, [ ebx + 32 ]
								mulps xmm2, xmm1
								mulps xmm3, xmm1
								mulps xmm4, xmm1
								addps xmm5, xmm2
								addps xmm6, xmm3
								addps xmm7, xmm4

								movaps xmm1, xmm0
								shufps xmm1, xmm0, 0xaa
								movaps xmm2, [ ecx ]
								movaps xmm3, [ ecx + 16 ]
								movaps xmm4, [ ecx + 32 ]
								mulps xmm2, xmm1
								mulps xmm3, xmm1
								mulps xmm4, xmm1
								addps xmm5, xmm2
								addps xmm6, xmm3
								addps xmm7, xmm4

								movaps xmm1, xmm0
								shufps xmm1, xmm0, 0xff
								movaps xmm2, [ eax ]
								movaps xmm3, [ eax + 16 ]
								movaps xmm4, [ eax + 32 ]
								mulps xmm2, xmm1
								mulps xmm3, xmm1
								mulps xmm4, xmm1
								addps xmm5, xmm2			// xmm5 = 0w 0z 0y 0x
								addps xmm6, xmm3			// xmm6 = 1w 1z 1y 1x
								addps xmm7, xmm4			// xmm7 = 2w 2z 2y 2x

								mov edx, [ edi + 24 ]
								movaps xmm0, xmm5			// xmm0 = 0w 0z 0y 0x
								movaps xmm2, xmm5			// xmm2 = 0w 0z 0y 0x

								unpcklps xmm0, xmm6			// xmm0 = 1y 0y 1x 0x
								unpckhps xmm2, xmm6			// xmm2 = 1w 0w 1z 0z

								movaps xmm3, xmm2			// xmm3 = 1w 0w 1z 0z
								shufps xmm3, xmm7, 0xfe		// xmm3 = 2w 2w 1w 0w  ����   xmm7:11 xmm7:11 xmm3:11 xmm3:10 = 0xfe

								shufps  xmm2, xmm7, 0xe4	// xmm2 = 2w 2z 1z 0z  ����   xmm7:11 xmm7:10 xmm2:01 xmm2:00 = 0xe4

								movaps  xmm1, xmm0			// xmm1 = 1y 0y 1x 0x
								movlhps xmm0, xmm7			// xmm0 = 2y 2x 1x 0x  ����
								shufps  xmm1, xmm7, 0x9e	// xmm1 = 2z 2y 1y 0y  ����   xmm7:10 xmm7:01 xmm1:11 xmm1:10 = 0x9e

								movaps xmm4, [ esi + 16 ]
								movaps xmm7, [ esi + 32 ]
								movaps xmm5, xmm4
								movaps xmm6, xmm4
								shufps xmm5, xmm5, 0x55
								shufps xmm6, xmm6, 0xaa
								shufps xmm4, xmm4, 0x00

								mulps xmm5, xmm1
								mulps xmm6, xmm2
								mulps xmm4, xmm0
								addps xmm6, xmm3
								addps xmm4, xmm5
								addps xmm4, xmm6
								movups [ edi ], xmm4

								movaps xmm5, xmm7
								movaps xmm6, xmm7
								shufps xmm5, xmm5, 0x55
								shufps xmm6, xmm6, 0xaa
								shufps xmm7, xmm7, 0x00

								mulps xmm5, xmm1
								mulps xmm6, xmm2
								mulps xmm7, xmm0
								addps xmm5, xmm6
								addps xmm5, xmm7
								movups [ edi + 12 ], xmm5
								mov [ edi + 24 ], edx

								add esi, 48
								add edi, DestUnitSize
								dec VertexNum
								jnz MV1_VERTEX_TYPE_SKIN_4BONE_LOOP
							} ;
						}
						else
#endif
#endif // VISUALCPP_2005
						{
							if( GSYS.DrawSetting.Large3DPositionSupport )
							{
								for( j = 0 ; j < VertexNum ; j ++, PosSK4B ++,
									SMPVert = ( MV1_VERTEX_SIMPLE_D3D9  * )( ( BYTE * )SMPVert + DestUnitSize ) )
								{
									// �u�����h�s��̍쐬
									Weight = PosSK4B->MatrixWeight[ 0 ] ;

									Mat = pSkinBoneMatrix[ PosSK4B->MatrixIndex[ 0 ] ] ;

									if( Weight == 1.0f )
									{
										VectorTransform4X4CTD(   &SMPVert->Position, ( VECTOR * )&PosSK4B->Position, &Mat->md ) ;
										VectorTransformSR4X4CTD( &SMPVert->Normal,               &PosSK4B->Normal,   &Mat->md ) ;
									}
									else
									{
										UnSafeMatrix4X4CT_D_Eq_D_Mul_S( &BlendMat.md, &Mat->md, Weight ) ;
										for( k = 1 ; k < 4 ; k ++ )
										{
											Weight = PosSK4B->MatrixWeight[ k ] ;

											if( Weight == 0.0f ) continue ;

											Mat = pSkinBoneMatrix[ PosSK4B->MatrixIndex[ k ] ] ;
											UnSafeMatrix4X4CT_D_EqPlus_D_Mul_S( &BlendMat.md, &Mat->md, Weight ) ;
										}

										VectorTransform4X4CTD(   &SMPVert->Position, ( VECTOR * )&PosSK4B->Position, &BlendMat.md ) ;
										VectorTransformSR4X4CTD( &SMPVert->Normal,               &PosSK4B->Normal,   &BlendMat.md ) ;
									}
								}
							}
							else
							{
								for( j = 0 ; j < VertexNum ; j ++, PosSK4B ++,
									SMPVert = ( MV1_VERTEX_SIMPLE_D3D9  * )( ( BYTE * )SMPVert + DestUnitSize ) )
								{
									// �u�����h�s��̍쐬
									Weight = PosSK4B->MatrixWeight[ 0 ] ;

									Mat = pSkinBoneMatrix[ PosSK4B->MatrixIndex[ 0 ] ] ;

									if( Weight == 1.0f )
									{
										VectorTransform4X4CTF(   &SMPVert->Position, ( VECTOR * )&PosSK4B->Position, &Mat->mf ) ;
										VectorTransformSR4X4CTF( &SMPVert->Normal,               &PosSK4B->Normal,   &Mat->mf ) ;
									}
									else
									{
										UnSafeMatrix4X4CT_F_Eq_F_Mul_S( &BlendMat.mf, &Mat->mf, Weight ) ;
										for( k = 1 ; k < 4 ; k ++ )
										{
											Weight = PosSK4B->MatrixWeight[ k ] ;

											if( Weight == 0.0f ) continue ;

											Mat = pSkinBoneMatrix[ PosSK4B->MatrixIndex[ k ] ] ;
											UnSafeMatrix4X4CT_F_EqPlus_F_Mul_S( &BlendMat.mf, &Mat->mf, Weight ) ;
										}

										VectorTransform4X4CTF(   &SMPVert->Position, ( VECTOR * )&PosSK4B->Position, &BlendMat.mf ) ;
										VectorTransformSR4X4CTF( &SMPVert->Normal,               &PosSK4B->Normal,   &BlendMat.mf ) ;
									}
								}
							}
						}
						break ;

					case MV1_VERTEX_TYPE_SKIN_8BONE :
						PosSK8B = MBMesh->Shape ? ( MV1_TLIST_SKIN_POS_8B * )ADDR16( TList->SkinPosition8B ) : ( MV1_TLIST_SKIN_POS_8B * )ADDR16( MBTList->SkinPosition8B ) ;
						if( GSYS.DrawSetting.Large3DPositionSupport )
						{
							for( j = 0 ; j < VertexNum ; j ++, PosSK8B ++,
								SMPVert = ( MV1_VERTEX_SIMPLE_D3D9  * )( ( BYTE * )SMPVert + DestUnitSize ) )
							{
								// �u�����h�s��̍쐬
								Weight = PosSK8B->MatrixWeight[ 0 ] ;

								Mat = pSkinBoneMatrix[ PosSK8B->MatrixIndex1[ 0 ] ] ;

								if( Weight == 1.0f )
								{
									VectorTransform4X4CTD(   &SMPVert->Position, &PosSK8B->Position, &Mat->md ) ;
									VectorTransformSR4X4CTD( &SMPVert->Normal,   &PosSK8B->Normal,   &Mat->md ) ;
								}
								else
								{
									UnSafeMatrix4X4CT_D_Eq_D_Mul_S( &BlendMat.md, &Mat->md, Weight ) ;
									for( k = 1 ; k < 4 ; k ++ )
									{
										Weight = PosSK8B->MatrixWeight[ k ] ;

										if( Weight == 0.0f ) continue ;

										Mat = pSkinBoneMatrix[ PosSK8B->MatrixIndex1[ k ] ] ;
										UnSafeMatrix4X4CT_D_EqPlus_D_Mul_S( &BlendMat.md, &Mat->md, Weight ) ;
									}

									for( k = 0 ; k < 4 ; k ++ )
									{
										Weight = PosSK8B->MatrixWeight[ k + 4 ] ;

										if( Weight == 0.0f ) continue ;

										Mat = pSkinBoneMatrix[ PosSK8B->MatrixIndex2[ k ] ] ;
										UnSafeMatrix4X4CT_D_EqPlus_D_Mul_S( &BlendMat.md, &Mat->md, Weight ) ;
									}

									VectorTransform4X4CTD(   &SMPVert->Position, &PosSK8B->Position, &BlendMat.md ) ;
									VectorTransformSR4X4CTD( &SMPVert->Normal,   &PosSK8B->Normal,   &BlendMat.md ) ;
								}
							}
						}
						else
						{
							for( j = 0 ; j < VertexNum ; j ++, PosSK8B ++,
								SMPVert = ( MV1_VERTEX_SIMPLE_D3D9  * )( ( BYTE * )SMPVert + DestUnitSize ) )
							{
								// �u�����h�s��̍쐬
								Weight = PosSK8B->MatrixWeight[ 0 ] ;

								Mat = pSkinBoneMatrix[ PosSK8B->MatrixIndex1[ 0 ] ] ;

								if( Weight == 1.0f )
								{
									VectorTransform4X4CTF(   &SMPVert->Position, &PosSK8B->Position, &Mat->mf ) ;
									VectorTransformSR4X4CTF( &SMPVert->Normal,   &PosSK8B->Normal,   &Mat->mf ) ;
								}
								else
								{
									UnSafeMatrix4X4CT_F_Eq_F_Mul_S( &BlendMat.mf, &Mat->mf, Weight ) ;
									for( k = 1 ; k < 4 ; k ++ )
									{
										Weight = PosSK8B->MatrixWeight[ k ] ;

										if( Weight == 0.0f ) continue ;

										Mat = pSkinBoneMatrix[ PosSK8B->MatrixIndex1[ k ] ] ;
										UnSafeMatrix4X4CT_F_EqPlus_F_Mul_S( &BlendMat.mf, &Mat->mf, Weight ) ;
									}

									for( k = 0 ; k < 4 ; k ++ )
									{
										Weight = PosSK8B->MatrixWeight[ k + 4 ] ;

										if( Weight == 0.0f ) continue ;

										Mat = pSkinBoneMatrix[ PosSK8B->MatrixIndex2[ k ] ] ;
										UnSafeMatrix4X4CT_F_EqPlus_F_Mul_S( &BlendMat.mf, &Mat->mf, Weight ) ;
									}

									VectorTransform4X4CTF(   &SMPVert->Position, &PosSK8B->Position, &BlendMat.mf ) ;
									VectorTransformSR4X4CTF( &SMPVert->Normal,   &PosSK8B->Normal,   &BlendMat.mf ) ;
								}
							}
						}
						break ;
					}

					// �g�D�[���������ꍇ�̓��C�e�B���O�v�Z�������ł���
					if( Mesh->Material->BaseData->Type == DX_MATERIAL_TYPE_TOON || Mesh->Material->BaseData->Type == DX_MATERIAL_TYPE_TOON_2 )
					{
						// �X�P�[�����O����邩�ǂ������`�F�b�N
						IsScaling = false ;
						if( GSYS.DrawSetting.Large3DPositionSupport )
						{
							for( j = 0 ; j < MBTList->UseBoneNum ; j ++ )
							{
								Scale = ( float )(
										pSkinBoneMatrix[ j ]->md.m[ 0 ][ 0 ] * pSkinBoneMatrix[ j ]->md.m[ 0 ][ 0 ] +
										pSkinBoneMatrix[ j ]->md.m[ 0 ][ 1 ] * pSkinBoneMatrix[ j ]->md.m[ 0 ][ 1 ] +
										pSkinBoneMatrix[ j ]->md.m[ 0 ][ 2 ] * pSkinBoneMatrix[ j ]->md.m[ 0 ][ 2 ] ) ;
								if( Scale < 0.999f || Scale > 1.001 ) IsScaling = true ;

								Scale = ( float )( 
										pSkinBoneMatrix[ j ]->md.m[ 1 ][ 0 ] * pSkinBoneMatrix[ j ]->md.m[ 1 ][ 0 ] +
										pSkinBoneMatrix[ j ]->md.m[ 1 ][ 1 ] * pSkinBoneMatrix[ j ]->md.m[ 1 ][ 1 ] +
										pSkinBoneMatrix[ j ]->md.m[ 1 ][ 2 ] * pSkinBoneMatrix[ j ]->md.m[ 1 ][ 2 ] ) ;
								if( Scale < 0.999f || Scale > 1.001 ) IsScaling = true ;

								Scale = ( float )(
										pSkinBoneMatrix[ j ]->md.m[ 2 ][ 0 ] * pSkinBoneMatrix[ j ]->md.m[ 2 ][ 0 ] +
										pSkinBoneMatrix[ j ]->md.m[ 2 ][ 1 ] * pSkinBoneMatrix[ j ]->md.m[ 2 ][ 1 ] +
										pSkinBoneMatrix[ j ]->md.m[ 2 ][ 2 ] * pSkinBoneMatrix[ j ]->md.m[ 2 ][ 2 ] ) ;
								if( Scale < 0.999f || Scale > 1.001 ) IsScaling = true ;
							}
						}
						else
						{
							for( j = 0 ; j < MBTList->UseBoneNum ; j ++ )
							{
								Scale = pSkinBoneMatrix[ j ]->mf.m[ 0 ][ 0 ] * pSkinBoneMatrix[ j ]->mf.m[ 0 ][ 0 ] +
										pSkinBoneMatrix[ j ]->mf.m[ 0 ][ 1 ] * pSkinBoneMatrix[ j ]->mf.m[ 0 ][ 1 ] +
										pSkinBoneMatrix[ j ]->mf.m[ 0 ][ 2 ] * pSkinBoneMatrix[ j ]->mf.m[ 0 ][ 2 ] ;
								if( Scale < 0.999f || Scale > 1.001 ) IsScaling = true ;

								Scale = pSkinBoneMatrix[ j ]->mf.m[ 1 ][ 0 ] * pSkinBoneMatrix[ j ]->mf.m[ 1 ][ 0 ] +
										pSkinBoneMatrix[ j ]->mf.m[ 1 ][ 1 ] * pSkinBoneMatrix[ j ]->mf.m[ 1 ][ 1 ] +
										pSkinBoneMatrix[ j ]->mf.m[ 1 ][ 2 ] * pSkinBoneMatrix[ j ]->mf.m[ 1 ][ 2 ] ;
								if( Scale < 0.999f || Scale > 1.001 ) IsScaling = true ;

								Scale = pSkinBoneMatrix[ j ]->mf.m[ 2 ][ 0 ] * pSkinBoneMatrix[ j ]->mf.m[ 2 ][ 0 ] +
										pSkinBoneMatrix[ j ]->mf.m[ 2 ][ 1 ] * pSkinBoneMatrix[ j ]->mf.m[ 2 ][ 1 ] +
										pSkinBoneMatrix[ j ]->mf.m[ 2 ][ 2 ] * pSkinBoneMatrix[ j ]->mf.m[ 2 ][ 2 ] ;
								if( Scale < 0.999f || Scale > 1.001 ) IsScaling = true ;
							}
						}

						MV1_D3D9_TriangleListToonLighting( MBTList, IsScaling, NULL ) ;
					}

					// �`��
#ifndef NOT_RENDER
					Graphics_D3D9_DeviceState_SetVertexShader( NULL ) ;
					Graphics_D3D9_DeviceState_SetPixelShader( NULL ) ;
					Graphics_D3D9_DeviceState_SetIndexBuffer( NULL ) ;
					Graphics_D3D9_DeviceState_SetVertexBuffer( NULL ) ;
					Graphics_D3D9_DeviceState_SetFVF( ( int )( D_D3DFVF_XYZ | D_D3DFVF_NORMAL | D_D3DFVF_DIFFUSE | D_D3DFVF_SPECULAR | UVNumFVFTable[ MBMesh->UVSetUnitNum + 2 ] ) ) ;
					Direct3DDevice9_DrawIndexedPrimitiveUP(
						D_D3DPT_TRIANGLELIST,
						0,
						( UINT )MBTList->VertexNum,
						( UINT )( MBTList->IndexNum / 3 ),
						MBTList->Index,
						D_D3DFMT_INDEX16,
						MBTListPF->D3D9.TempSimpleVertex,
						( UINT )MBTList->TempUnitSize ) ;
#endif
				}
				break ;


				// �{�[�����������g���C�A���O�����X�g
			case MV1_VERTEX_TYPE_SKIN_FREEBONE :
				// �}�e���A���̃Z�b�g�A�b�v
				if( SetupShaderMaterial != 0 )
				{
					MV1_D3D9_SetupMeshDrawMaterial( MBase, &BlendInfo, Mesh, false, SpecularEnable ) ;
					SetupShaderMaterial = 0 ;
				}

				// �Œ�@�\�p�C�v���C���ɖ߂�
				Graphics_D3D9_DeviceState_ResetVertexShader() ;
				Graphics_D3D9_DeviceState_ResetPixelShader() ;

				// �P�ʍs����Z�b�g
				if( MV1Man.WorldMatrixIsIdentity == FALSE )
				{
					MV1Man.WorldMatrixIsIdentity = TRUE ;
					Graphics_DrawSetting_SetTransformToWorld_Direct( &IdentityMat ) ;
				}

				// �e���|�����o�b�t�@���Z�b�g�A�b�v����Ă��Ȃ�������Z�b�g�A�b�v
				if( MBTListPF->D3D9.TempSimpleVertex == NULL )
					MV1_D3D9_SetupTListTempSimpleVertex( MBTList ) ;

				// �P���_�̃T�C�Y���Z�o
				SrcUnitSize  = MBTList->PosUnitSize ;
				DestUnitSize = MBTList->TempUnitSize ;

				// �f�[�^�̃Z�b�g
				SMPVert = MBTListPF->D3D9.TempSimpleVertex ;
				PosSKFB = MBMesh->Shape ? ( MV1_TLIST_SKIN_POS_FREEB * )ADDR16( TList->SkinPositionFREEB ) : ( MV1_TLIST_SKIN_POS_FREEB * )ADDR16( MBTList->SkinPositionFREEB ) ;

				MaxBoneNum = MBTList->MaxBoneNum ;
				VertexNum  = MBTList->VertexNum ;

				// �g�p���Ă���{�[���̍ő吔�� MV1_TRIANGLE_LIST_USE_BONE_MAX_NUM �ȉ���������e�[�u�����g�p����
				if( MBFrame->UseSkinBoneNum < MV1_TRIANGLE_LIST_USE_BONE_MAX_NUM )
				{
					// �g�p����s��̏������s��
					for( j = 0 ; j < MBFrame->UseSkinBoneNum ; j ++ )
					{
						pSkinBoneMatrix[ j ] = Frame->UseSkinBoneMatrix[ j ] ;
					}

					if( GSYS.DrawSetting.Large3DPositionSupport )
					{
						for( j = 0 ; j < VertexNum ; j ++,
							SMPVert = ( MV1_VERTEX_SIMPLE_D3D9        * )( ( BYTE * )SMPVert + DestUnitSize ),
							PosSKFB = ( MV1_TLIST_SKIN_POS_FREEB * )( ( BYTE * )PosSKFB + SrcUnitSize ) )
						{
							// �g���Ă���{�[�����ЂƂ̏ꍇ�̓u�����h�s��̍쐬�̓X�L�b�v����
							if( PosSKFB->MatrixWeight[ 0 ].W == 1.0f )
							{
								Mat = pSkinBoneMatrix[ PosSKFB->MatrixWeight[ 0 ].Index ] ;

								VectorTransform4X4CTD(   &SMPVert->Position, ( VECTOR * )&PosSKFB->Position, &Mat->md ) ;
								VectorTransformSR4X4CTD( &SMPVert->Normal,   ( VECTOR * )&PosSKFB->Normal,   &Mat->md ) ;
							}
							else
							{
								// �u�����h�s��̍쐬
								VBlend = PosSKFB->MatrixWeight ;
								Weight = VBlend->W ;

								// �O�Ԗڂ͉��Z�ł͂Ȃ��̂ŕʏ���
								Mat = pSkinBoneMatrix[ PosSKFB->MatrixWeight[ 0 ].Index ] ;
								UnSafeMatrix4X4CT_D_Eq_D_Mul_S( &BlendMat.md, &Mat->md, Weight ) ;
								VBlend ++ ;

								// �P�Ԗڂ���͉��Z
								for( k = 1 ; k < MaxBoneNum && VBlend->Index != -1 ; k ++, VBlend ++ )
								{
									Weight = VBlend->W ;

									if( Weight == 0.0f ) continue ;

									Mat = pSkinBoneMatrix[ VBlend->Index ] ;
									UnSafeMatrix4X4CT_D_EqPlus_D_Mul_S( &BlendMat.md, &Mat->md, Weight ) ;
								}

								// ���_���W�ƃu�����h�s�����Z
								VectorTransform4X4CTD(   &SMPVert->Position, ( VECTOR * )&PosSKFB->Position, &BlendMat.md ) ;
								VectorTransformSR4X4CTD( &SMPVert->Normal,   ( VECTOR * )&PosSKFB->Normal,   &BlendMat.md ) ;
							}
						}
					}
					else
					{
						for( j = 0 ; j < VertexNum ; j ++,
							SMPVert = ( MV1_VERTEX_SIMPLE_D3D9        * )( ( BYTE * )SMPVert + DestUnitSize ),
							PosSKFB = ( MV1_TLIST_SKIN_POS_FREEB * )( ( BYTE * )PosSKFB + SrcUnitSize ) )
						{
							// �g���Ă���{�[�����ЂƂ̏ꍇ�̓u�����h�s��̍쐬�̓X�L�b�v����
							if( PosSKFB->MatrixWeight[ 0 ].W == 1.0f )
							{
								Mat = pSkinBoneMatrix[ PosSKFB->MatrixWeight[ 0 ].Index ] ;

								VectorTransform4X4CTF(   &SMPVert->Position, ( VECTOR * )&PosSKFB->Position, &Mat->mf ) ;
								VectorTransformSR4X4CTF( &SMPVert->Normal,   ( VECTOR * )&PosSKFB->Normal,   &Mat->mf ) ;
							}
							else
							{
								// �u�����h�s��̍쐬
								VBlend = PosSKFB->MatrixWeight ;
								Weight = VBlend->W ;

								// �O�Ԗڂ͉��Z�ł͂Ȃ��̂ŕʏ���
								Mat = pSkinBoneMatrix[ PosSKFB->MatrixWeight[ 0 ].Index ] ;
								UnSafeMatrix4X4CT_F_Eq_F_Mul_S( &BlendMat.mf, &Mat->mf, Weight ) ;
								VBlend ++ ;

								// �P�Ԗڂ���͉��Z
								for( k = 1 ; k < MaxBoneNum && VBlend->Index != -1 ; k ++, VBlend ++ )
								{
									Weight = VBlend->W ;

									if( Weight == 0.0f ) continue ;

									Mat = pSkinBoneMatrix[ VBlend->Index ] ;
									UnSafeMatrix4X4CT_F_EqPlus_F_Mul_S( &BlendMat.mf, &Mat->mf, Weight ) ;
								}

								// ���_���W�ƃu�����h�s�����Z
								VectorTransform4X4CTF(   &SMPVert->Position, ( VECTOR * )&PosSKFB->Position, &BlendMat.mf ) ;
								VectorTransformSR4X4CTF( &SMPVert->Normal,   ( VECTOR * )&PosSKFB->Normal,   &BlendMat.mf ) ;
							}
						}
					}
				}
				else
				{
					if( GSYS.DrawSetting.Large3DPositionSupport )
					{
						for( j = 0 ; j < VertexNum ; j ++,
							SMPVert = ( MV1_VERTEX_SIMPLE_D3D9        * )( ( BYTE * )SMPVert + DestUnitSize ),
							PosSKFB = ( MV1_TLIST_SKIN_POS_FREEB * )( ( BYTE * )PosSKFB + SrcUnitSize ) )
						{
							// �g���Ă���{�[�����ЂƂ̏ꍇ�̓u�����h�s��̍쐬�̓X�L�b�v����
							if( PosSKFB->MatrixWeight[ 0 ].W == 1.0f )
							{
								Mat = Frame->UseSkinBoneMatrix[ PosSKFB->MatrixWeight[ 0 ].Index ] ;

								// ���_���W�ƃu�����h�s�����Z
								VectorTransform4X4CTD(   &SMPVert->Position, ( VECTOR * )&PosSKFB->Position, &Mat->md ) ;
								VectorTransformSR4X4CTD( &SMPVert->Normal,   ( VECTOR * )&PosSKFB->Normal,   &Mat->md ) ;
							}
							else
							{
								// �u�����h�s��̍쐬
								VBlend = PosSKFB->MatrixWeight ;
								Weight = VBlend->W ;

								// �O�Ԗڂ͉��Z�ł͂Ȃ��̂ŕʏ���
								Mat = Frame->UseSkinBoneMatrix[ VBlend->Index ] ;
								UnSafeMatrix4X4CT_D_Eq_D_Mul_S( &BlendMat.md, &Mat->md, Weight ) ;
								VBlend ++ ;

								// �P�Ԗڂ���͉��Z
								for( k = 1 ; k < MaxBoneNum && VBlend->Index != -1 ; k ++, VBlend ++ )
								{
									Weight = VBlend->W ;

									if( Weight == 0.0f ) continue ;

									Mat = Frame->UseSkinBoneMatrix[ VBlend->Index ] ;
									UnSafeMatrix4X4CT_D_EqPlus_D_Mul_S( &BlendMat.md, &Mat->md, Weight ) ;
								}

								// ���_���W�ƃu�����h�s�����Z
								VectorTransform4X4CTD(   &SMPVert->Position, ( VECTOR * )&PosSKFB->Position, &BlendMat.md ) ;
								VectorTransformSR4X4CTD( &SMPVert->Normal,   ( VECTOR * )&PosSKFB->Normal,   &BlendMat.md ) ;
							}
						}
					}
					else
					{
						for( j = 0 ; j < VertexNum ; j ++,
							SMPVert = ( MV1_VERTEX_SIMPLE_D3D9        * )( ( BYTE * )SMPVert + DestUnitSize ),
							PosSKFB = ( MV1_TLIST_SKIN_POS_FREEB * )( ( BYTE * )PosSKFB + SrcUnitSize ) )
						{
							// �g���Ă���{�[�����ЂƂ̏ꍇ�̓u�����h�s��̍쐬�̓X�L�b�v����
							if( PosSKFB->MatrixWeight[ 0 ].W == 1.0f )
							{
								Mat = Frame->UseSkinBoneMatrix[ PosSKFB->MatrixWeight[ 0 ].Index ] ;

								// ���_���W�ƃu�����h�s�����Z
								VectorTransform4X4CTF(   &SMPVert->Position, ( VECTOR * )&PosSKFB->Position, &Mat->mf ) ;
								VectorTransformSR4X4CTF( &SMPVert->Normal,   ( VECTOR * )&PosSKFB->Normal,   &Mat->mf ) ;
							}
							else
							{
								// �u�����h�s��̍쐬
								VBlend = PosSKFB->MatrixWeight ;
								Weight = VBlend->W ;

								// �O�Ԗڂ͉��Z�ł͂Ȃ��̂ŕʏ���
								Mat = Frame->UseSkinBoneMatrix[ VBlend->Index ] ;
								UnSafeMatrix4X4CT_F_Eq_F_Mul_S( &BlendMat.mf, &Mat->mf, Weight ) ;
								VBlend ++ ;

								// �P�Ԗڂ���͉��Z
								for( k = 1 ; k < MaxBoneNum && VBlend->Index != -1 ; k ++, VBlend ++ )
								{
									Weight = VBlend->W ;

									if( Weight == 0.0f ) continue ;

									Mat = Frame->UseSkinBoneMatrix[ VBlend->Index ] ;
									UnSafeMatrix4X4CT_F_EqPlus_F_Mul_S( &BlendMat.mf, &Mat->mf, Weight ) ;
								}

								// ���_���W�ƃu�����h�s�����Z
								VectorTransform4X4CTF(   &SMPVert->Position, ( VECTOR * )&PosSKFB->Position, &BlendMat.mf ) ;
								VectorTransformSR4X4CTF( &SMPVert->Normal,   ( VECTOR * )&PosSKFB->Normal,   &BlendMat.mf ) ;
							}
						}
					}
				}

				// �g�D�[���������ꍇ�̓��C�e�B���O�v�Z�������ł���
				if( Mesh->Material->BaseData->Type == DX_MATERIAL_TYPE_TOON || Mesh->Material->BaseData->Type == DX_MATERIAL_TYPE_TOON_2 )
				{
					// �X�P�[�����O����邩�ǂ������`�F�b�N
					IsScaling = false ;
					if( GSYS.DrawSetting.Large3DPositionSupport )
					{
						for( j = 0 ; j < Frame->BaseData->UseSkinBoneNum ; j ++ )
						{
							Scale = ( float )(
									Frame->UseSkinBoneMatrix[ j ]->md.m[ 0 ][ 0 ] * Frame->UseSkinBoneMatrix[ j ]->md.m[ 0 ][ 0 ] +
									Frame->UseSkinBoneMatrix[ j ]->md.m[ 0 ][ 1 ] * Frame->UseSkinBoneMatrix[ j ]->md.m[ 0 ][ 1 ] +
									Frame->UseSkinBoneMatrix[ j ]->md.m[ 0 ][ 2 ] * Frame->UseSkinBoneMatrix[ j ]->md.m[ 0 ][ 2 ] ) ;
							if( Scale < 0.999f || Scale > 1.001 ) IsScaling = true ;

							Scale = ( float )(
									Frame->UseSkinBoneMatrix[ j ]->md.m[ 1 ][ 0 ] * Frame->UseSkinBoneMatrix[ j ]->md.m[ 1 ][ 0 ] +
									Frame->UseSkinBoneMatrix[ j ]->md.m[ 1 ][ 1 ] * Frame->UseSkinBoneMatrix[ j ]->md.m[ 1 ][ 1 ] +
									Frame->UseSkinBoneMatrix[ j ]->md.m[ 1 ][ 2 ] * Frame->UseSkinBoneMatrix[ j ]->md.m[ 1 ][ 2 ] ) ;
							if( Scale < 0.999f || Scale > 1.001 ) IsScaling = true ;

							Scale = ( float )(
									Frame->UseSkinBoneMatrix[ j ]->md.m[ 2 ][ 0 ] * Frame->UseSkinBoneMatrix[ j ]->md.m[ 2 ][ 0 ] +
									Frame->UseSkinBoneMatrix[ j ]->md.m[ 2 ][ 1 ] * Frame->UseSkinBoneMatrix[ j ]->md.m[ 2 ][ 1 ] +
									Frame->UseSkinBoneMatrix[ j ]->md.m[ 2 ][ 2 ] * Frame->UseSkinBoneMatrix[ j ]->md.m[ 2 ][ 2 ] ) ;
							if( Scale < 0.999f || Scale > 1.001 ) IsScaling = true ;
						}
					}
					else
					{
						for( j = 0 ; j < Frame->BaseData->UseSkinBoneNum ; j ++ )
						{
							Scale = Frame->UseSkinBoneMatrix[ j ]->mf.m[ 0 ][ 0 ] * Frame->UseSkinBoneMatrix[ j ]->mf.m[ 0 ][ 0 ] +
									Frame->UseSkinBoneMatrix[ j ]->mf.m[ 0 ][ 1 ] * Frame->UseSkinBoneMatrix[ j ]->mf.m[ 0 ][ 1 ] +
									Frame->UseSkinBoneMatrix[ j ]->mf.m[ 0 ][ 2 ] * Frame->UseSkinBoneMatrix[ j ]->mf.m[ 0 ][ 2 ] ;
							if( Scale < 0.999f || Scale > 1.001 ) IsScaling = true ;

							Scale = Frame->UseSkinBoneMatrix[ j ]->mf.m[ 1 ][ 0 ] * Frame->UseSkinBoneMatrix[ j ]->mf.m[ 1 ][ 0 ] +
									Frame->UseSkinBoneMatrix[ j ]->mf.m[ 1 ][ 1 ] * Frame->UseSkinBoneMatrix[ j ]->mf.m[ 1 ][ 1 ] +
									Frame->UseSkinBoneMatrix[ j ]->mf.m[ 1 ][ 2 ] * Frame->UseSkinBoneMatrix[ j ]->mf.m[ 1 ][ 2 ] ;
							if( Scale < 0.999f || Scale > 1.001 ) IsScaling = true ;

							Scale = Frame->UseSkinBoneMatrix[ j ]->mf.m[ 2 ][ 0 ] * Frame->UseSkinBoneMatrix[ j ]->mf.m[ 2 ][ 0 ] +
									Frame->UseSkinBoneMatrix[ j ]->mf.m[ 2 ][ 1 ] * Frame->UseSkinBoneMatrix[ j ]->mf.m[ 2 ][ 1 ] +
									Frame->UseSkinBoneMatrix[ j ]->mf.m[ 2 ][ 2 ] * Frame->UseSkinBoneMatrix[ j ]->mf.m[ 2 ][ 2 ] ;
							if( Scale < 0.999f || Scale > 1.001 ) IsScaling = true ;
						}
					}
					MV1_D3D9_TriangleListToonLighting( MBTList, IsScaling, NULL ) ;
				}

				// �`��
#ifndef NOT_RENDER
				Graphics_D3D9_DeviceState_SetVertexShader( NULL ) ;
				Graphics_D3D9_DeviceState_SetPixelShader( NULL ) ;
				Graphics_D3D9_DeviceState_SetIndexBuffer( NULL ) ;
				Graphics_D3D9_DeviceState_SetVertexBuffer( NULL ) ;
				Graphics_D3D9_DeviceState_SetFVF( ( int )( D_D3DFVF_XYZ | D_D3DFVF_NORMAL | D_D3DFVF_DIFFUSE | D_D3DFVF_SPECULAR | UVNumFVFTable[ MBMesh->UVSetUnitNum + 2 ] ) ) ;
				Direct3DDevice9_DrawIndexedPrimitiveUP(
					D_D3DPT_TRIANGLELIST,
					0,
					( UINT )MBTList->VertexNum,
					( UINT )( MBTList->IndexNum / 3 ),
					MBTList->Index,
					D_D3DFMT_INDEX16,
					MBTListPF->D3D9.TempSimpleVertex,
					( UINT )MBTList->TempUnitSize ) ;
#endif
				break ;
			}
		}
	}

	// �g�D�[��������ꍇ�͗֊s����`��
	if( ( ( Model->MeshCategoryHide[ DX_MV1_MESHCATEGORY_OUTLINE ]             == FALSE && MV1Man.UseOrigShaderFlag == FALSE ) ||
		  ( Model->MeshCategoryHide[ DX_MV1_MESHCATEGORY_OUTLINE_ORIG_SHADER ] == FALSE && MV1Man.UseOrigShaderFlag == TRUE  ) ) && 
		( Mesh->Material->BaseData->Type == DX_MATERIAL_TYPE_TOON || Mesh->Material->BaseData->Type == DX_MATERIAL_TYPE_TOON_2 ) )
	{
		// �֊s���̕`��
		if( Mesh->Material->OutLineWidth    > 0.000001f ||
			Mesh->Material->OutLineDotWidth > 0.000001f )
		{
			float OutLineWidth ;
			DWORD OutLineColor ;

			// �֊s���̑������Z�b�g
			OutLineWidth = Mesh->Material->OutLineWidth /*/ Mesh->Container->Container->Scale*/ ;

			// �����X�N���[�����W��łP�h�b�g�ɖ����Ȃ��ꍇ�͑�������
			{
				VECTOR_D WorldPos1, WorldPos2 ;
				VECTOR_D ScreenPos1, ScreenPos2 ;
				double Width, DotWidth ;

				if( GSYS.DrawSetting.Large3DPositionSupport )
				{
					WorldPos1.x = Frame->LocalWorldMatrix.md.m[ 0 ][ 3 ] ;
					WorldPos1.y = Frame->LocalWorldMatrix.md.m[ 1 ][ 3 ] ;
					WorldPos1.z = Frame->LocalWorldMatrix.md.m[ 2 ][ 3 ] ;
				}
				else
				{
					WorldPos1.x = Frame->LocalWorldMatrix.mf.m[ 0 ][ 3 ] ;
					WorldPos1.y = Frame->LocalWorldMatrix.mf.m[ 1 ][ 3 ] ;
					WorldPos1.z = Frame->LocalWorldMatrix.mf.m[ 2 ][ 3 ] ;
				}
				ScreenPos1 = NS_ConvWorldPosToScreenPosD( WorldPos1 ) ;

				WorldPos2.x = WorldPos1.x + GSYS.DrawSetting.ViewMatrix.m[ 0 ][ 0 ] * OutLineWidth ;
				WorldPos2.y = WorldPos1.y + GSYS.DrawSetting.ViewMatrix.m[ 1 ][ 0 ] * OutLineWidth ;
				WorldPos2.z = WorldPos1.z + GSYS.DrawSetting.ViewMatrix.m[ 2 ][ 0 ] * OutLineWidth ;
				ScreenPos2 = NS_ConvWorldPosToScreenPosD( WorldPos2 ) ;

				Width = ScreenPos1.x - ScreenPos2.x ;
				if( Width < 0.0 )
				{
					Width = -Width ;
				}

				DotWidth = Mesh->Material->OutLineDotWidth ;
				if( DotWidth < 0.0000001 )
				{
					DotWidth = 1.125 ;
				}

				if( Width < DotWidth )
				{
					if( Width < 0.00000001 )
					{
						OutLineWidth *= 10000000.0f ;
					}
					else
					{
						OutLineWidth *= ( float )( DotWidth / Width ) ;
					}
				}
			}

			VertexShaderIndex     = D3D9_VERTEXSHADER_TYPE( D3D9_VERTEXSHADER_TYPE_TOON_OUTLINE ) + D3D9_VERTEXSHADER_FOGMODE( GD3D9.Device.State.FogEnable ? GD3D9.Device.State.FogMode : DX_FOGMODE_NONE ) ;
			PixelShaderIndex      = 0 ;

			VertexShaderIndex_PL = 0 ;
			PixelShaderIndex_PL  = 0 ;

			// �}�e���A���̋��ʕ����̃Z�b�g�A�b�v
			MV1_D3D9_SetupToonOutLineMeshDrawMaterialCommon( &BlendInfo, Mesh, OutLineWidth ) ;

			// �g���C�A���O�����X�g�̐������J��Ԃ�
			TList = Mesh->TriangleList ;
			MBTList = MBMesh->TriangleList ;
			SetupShaderMaterial = -1 ;
			for( i = 0 ; i < MBMesh->TriangleListNum ; i ++ , MBTList ++, TList = ( MV1_TRIANGLE_LIST * )( ( BYTE * )TList + sizeof( MV1_TRIANGLE_LIST ) + sizeof( MV1_TRIANGLE_LIST_PF ) ) )
			{
				MBTListPF = ( MV1_TRIANGLE_LIST_BASE_PF * )MBTList->PFBuffer ;

				// �g���C�A���O�����X�g�̎w�肪����ꍇ�͂���ȊO�̃g���C�A���O�����X�g�͕`�悵�Ȃ�
				if( TriangleListIndex >= 0 && i != TriangleListIndex )
					continue ;

				// �g�D�[���̗֊s���p�̃|���S���������ꍇ�͉������Ȃ�
				if( MBTList->ToonOutLineIndexNum == 0 ) continue ;

				// ���_�̃^�C�v�ɂ���ď����𕪊�
				NowVertexShaderIndex     = VertexShaderIndex ;
				NowPixelShaderIndex      = PixelShaderIndex ;

				NowVertexShaderIndex_PL = VertexShaderIndex_PL ;
				NowPixelShaderIndex_PL  = PixelShaderIndex_PL ;

				switch( MBTList->VertexType )
				{
					// ���̃g���C�A���O�����X�g
				case MV1_VERTEX_TYPE_NORMAL :
					if( 1 )
		//			if( Shader )
		//			if( 0 )
					{
		//				MATRIX BlendMat2, BlendMat3 ;

						// �V�F�[�_�[���g�p���ĕ`��
						if( MBTList->VertexBuffer == NULL || MBTList->VertexBuffer->PF->D3D9.VertexBuffer == NULL || MBTList->VertexBuffer->PF->D3D9.IndexBuffer == NULL )
							goto T_NONSDSIMPLE ;

						// ���������`��̏ꍇ�͂S�{�[���X�L�j���O���b�V���Ƃ��ĕ`�悷��
						if( MBase->UsePackDraw )
						{
							goto T_SD4BONESKIN ;
						}

						// ���[�U�[�ݒ���g�p����ꍇ
						if( MV1Man.UseOrigShaderFlag )
						{
							// ���_�V�F�[�_�[�̃Z�b�g�A�b�v
							if( GSYS.DrawSetting.UserShaderRenderInfo.SetVertexShaderHandle > 0 )
							{
								if( Graphics_D3D9_DeviceState_SetVertexShaderToHandle( GSYS.DrawSetting.UserShaderRenderInfo.SetVertexShaderHandle ) < 0 )
									goto T_NONSDSIMPLE ;
							}

							// �s�N�Z���V�F�[�_�[�̃Z�b�g�A�b�v
							if( GSYS.DrawSetting.UserShaderRenderInfo.SetPixelShaderHandle > 0 )
							{
								if( Graphics_D3D9_DeviceState_SetPixelShaderToHandle( GSYS.DrawSetting.UserShaderRenderInfo.SetPixelShaderHandle ) < 0 )
									goto T_NONSDSIMPLE ;
							}

							// �V�F�[�_�[�̃Z�b�g�A�b�v
							if( Graphics_D3D9_Shader_Model_Setup( 
								GSYS.DrawSetting.UserShaderRenderInfo.SetVertexShaderHandle > 0 ? -1 : NowVertexShaderIndex, NowVertexShaderIndex_PL,
								GSYS.DrawSetting.UserShaderRenderInfo.SetPixelShaderHandle  > 0 ? -1 : NowPixelShaderIndex,  NowPixelShaderIndex_PL ) == FALSE )
							{
								goto T_NONSDSIMPLE ;
							}
						}
						else
						{
							// �f�t�H���g����

							// �V�F�[�_�[�̃Z�b�g�A�b�v
							if( Graphics_D3D9_Shader_Model_Setup( 
									NowVertexShaderIndex, NowVertexShaderIndex_PL,
									NowPixelShaderIndex,  NowPixelShaderIndex_PL ) == FALSE )
							{
								goto T_NONSDSIMPLE ;
							}
						}

						// ���_�f�[�^�̃Z�b�g�A�b�v
						TOutLine = 0 ;

						BumpMap = D3D9_VERTEXSHADER_GET_BUMPMAP( NowVertexShaderIndex ) ;
						SkinMesh = D3D9_VERTEXSHADER_GET_MESHTYPE( NowVertexShaderIndex ) ;
/*
						TOutLine = VS_GET_TOONOUTLINE( NowVertexShaderIndex ) ;
						TOutLine = 0 ;

						BumpMap = VS_GET_BUMPMAP( NowVertexShaderIndex ) ;
						SkinMesh = VS_GET_SKINMESH( NowVertexShaderIndex ) ;
*/
						UVNum = TOutLine == 1 ? 0 : 1 ;
						Graphics_D3D9_DeviceState_SetMV1VertexDeclaration( BumpMap, SkinMesh, UVNum ) ;

						// ���_�o�b�t�@�̃Z�b�g�A�b�v
						if( TList->PF->D3D9.VertexBuffer )
						{
							Graphics_D3D9_DeviceState_SetVertexBuffer( TList->PF->D3D9.VertexBuffer,                 ( int )MBTList->VertexBuffer->UnitSize ) ;
						}
						else
						{
							Graphics_D3D9_DeviceState_SetVertexBuffer( MBTList->VertexBuffer->PF->D3D9.VertexBuffer, ( int )MBTList->VertexBuffer->UnitSize ) ;
						}

						// �}�e���A���̃Z�b�g�A�b�v
						if( SetupShaderMaterial != 1 )
						{
							MV1_D3D9_SetupToonOutLineMeshDrawMaterial( MBase, &BlendInfo, Mesh, true ) ;
							SetupShaderMaterial = 1 ;
						}

						// �C���f�b�N�X�o�b�t�@�̃Z�b�g�A�b�v
						Graphics_D3D9_DeviceState_SetIndexBuffer( MBTList->VertexBuffer->PF->D3D9.IndexBuffer ) ;

						// �g�p���郍�[�J�������[���h�s��̗p��
						if( GSYS.DrawSetting.Large3DPositionSupport )
						{
							if( UseWorldViewMatrix )
							{
								ConvertMatrix4x4cDToMatrix4x4cF( &TempMatrixTable[ 0 ], &Frame->LocalWorldMatrix.md ) ;
							}
							else
							{
								MATRIX_4X4CT_D ViewMat ;

								ConvertMatrixDToMatrix4x4cD( &ViewMat, &GSYS.DrawSetting.ViewMatrix ) ;
								UnSafeMultiplyMatrix4X4CTD( &TempMatrixTableD[ 0 ], &Frame->LocalWorldMatrix.md, &ViewMat ) ;
								ConvertMatrix4x4cDToMatrix4x4cF( &TempMatrixTable[ 0 ], &TempMatrixTableD[ 0 ] ) ;

								Graphics_D3D9_DeviceState_SetViewMatrix( &IdentityMat ) ;
							}

							Graphics_D3D9_ShaderConstant_InfoSet_SetParam(
								&GD3D9.Device.Shader.ShaderConstantInfo,
								DX_SHADERCONSTANTTYPE_VS_FLOAT,
								DX_SHADERCONSTANTSET_LIB,
								DX_VS_CONSTF_WORLD_MAT_START,
								&TempMatrixTable[ 0 ],
								3,
								FALSE
							) ;
						}
						else
						{
							Graphics_D3D9_ShaderConstant_InfoSet_SetParam(
								&GD3D9.Device.Shader.ShaderConstantInfo,
								DX_SHADERCONSTANTTYPE_VS_FLOAT,
								DX_SHADERCONSTANTSET_LIB,
								DX_VS_CONSTF_WORLD_MAT_START,
								&Frame->LocalWorldMatrix.mf,
								3,
								FALSE
							) ;
						}

#ifndef NOT_RENDER
						// �`��
						Direct3DDevice9_DrawIndexedPrimitive(
							D_D3DPT_TRIANGLELIST,
							TList->PF->D3D9.VertexBuffer ? 0 : MBTList->VBStartVertex,
							0,
							( UINT )MBTList->VertexNum,
							( UINT )MBTList->ToonOutLineVBStartIndex,
							( UINT )( MBTList->ToonOutLineIndexNum / 3 ) ) ;
#endif // NOT_RENDER
						if( GSYS.DrawSetting.Large3DPositionSupport && UseWorldViewMatrix == FALSE )
						{
							Graphics_D3D9_DeviceState_SetViewMatrix( &GSYS.DrawSetting.ViewMatrixF ) ;
						}
					}
					else
					{
		T_NONSDSIMPLE :
						if( Frame->ValidLocalWorldMatrixNM == false )
						{
							Frame->ValidLocalWorldMatrixNM = true ;
							ConvertMatrix4x4cToMatrixF( &Frame->LocalWorldMatrixNM, &Frame->LocalWorldMatrix ) ;
		//					CreateTransposeMatrix( &Frame->LocalWorldMatrixNM, &Frame->LocalWorldMatrix ) ;
						}

						// �s��̃Z�b�g
						Graphics_DrawSetting_SetTransformToWorld_Direct( &Frame->LocalWorldMatrixNM ) ;
						MV1Man.WorldMatrixIsIdentity = FALSE ;

						// �}�e���A���̃Z�b�g�A�b�v
						if( SetupShaderMaterial != 0 )
						{
							MV1_D3D9_SetupToonOutLineMeshDrawMaterial( MBase, &BlendInfo, Mesh, false ) ;
							SetupShaderMaterial = 0 ;
						}

						// �Œ�@�\�p�C�v���C���ɖ߂�
						Graphics_D3D9_DeviceState_ResetVertexShader() ;
						Graphics_D3D9_DeviceState_ResetPixelShader() ;

#ifndef NOT_RENDER

						// �e���|�����o�b�t�@���Z�b�g�A�b�v����Ă��Ȃ�������Z�b�g�A�b�v
						if( MBTListPF->D3D9.TempSimpleVertex == NULL )
							MV1_D3D9_SetupTListTempSimpleVertex( MBTList ) ;

						// �e���|�����o�b�t�@���Z�b�g�A�b�v����Ă��Ȃ�������Z�b�g�A�b�v
						if( MBTListPF->D3D9.TempToonOutLineSimpleVertex == NULL )
							MV1_D3D9_SetupTListTempToonOutLineSimpleVertex( MBTList ) ;

						// �V�F�C�v���b�V���������ꍇ�͂����Ńe���|�����o�b�t�@�ɓ��e����������
						if( MBMesh->Shape )
						{
							MV1_TLIST_NORMAL_POS * RST Norm ;

							// �P���_������̃f�[�^�T�C�Y���Z�o
							DestUnitSize = MBTList->TempUnitSize ;

							// �f�[�^�̃Z�b�g
							TONVert   = ( MV1_VERTEX_SIMPLE_TOL_D3D9 * )( ( ( DWORD_PTR )MBTListPF->D3D9.TempToonOutLineSimpleVertex + 15 ) / 16 * 16 ) ;
							VertexNum = MBTList->VertexNum ;
							Norm      = ( MV1_TLIST_NORMAL_POS * )ADDR16( TList->NormalPosition ) ;

							for( j = 0 ; j < VertexNum ; j ++, Norm ++, TONVert ++ )
							{
								TONVert->Position.x = Norm->Position.x + Norm->Normal.x * OutLineWidth ;
								TONVert->Position.y = Norm->Position.y + Norm->Normal.y * OutLineWidth ;
								TONVert->Position.z = Norm->Position.z + Norm->Normal.z * OutLineWidth ;
							}
						}

						// �P���_������̃f�[�^�T�C�Y���Z�o
						DestUnitSize = MBTList->TempUnitSize ;

						// �f�[�^�̃Z�b�g
						TONVert = ( MV1_VERTEX_SIMPLE_TOL_D3D9 * )( ( ( DWORD_PTR )MBTListPF->D3D9.TempToonOutLineSimpleVertex + 15 ) / 16 * 16 ) ;
						SMPVert = MBTListPF->D3D9.TempSimpleVertex ;
						OutLineColor =
							( 255                                                    << 24 ) |
							( ( DWORD )_FTOL( Mesh->Material->OutLineColor.r * 255 ) << 16 ) |
							( ( DWORD )_FTOL( Mesh->Material->OutLineColor.g * 255 ) << 8  ) |
							( ( DWORD )_FTOL( Mesh->Material->OutLineColor.b * 255 )       ) ;
						VertexNum = MBTList->VertexNum ;
						for( j = 0 ; j < VertexNum ; j ++, TONVert ++, SMPVert = ( MV1_VERTEX_SIMPLE_D3D9  * )( ( BYTE * )SMPVert + DestUnitSize ) )
						{
							TONVert->Position.x = SMPVert->Position.x + SMPVert->Normal.x * OutLineWidth ;
							TONVert->Position.y = SMPVert->Position.y + SMPVert->Normal.y * OutLineWidth ;
							TONVert->Position.z = SMPVert->Position.z + SMPVert->Normal.z * OutLineWidth ;
							*( ( DWORD * )&TONVert->DiffuseColor ) = OutLineColor ;
						}


						Graphics_D3D9_DeviceState_SetVertexShader( NULL ) ;
						Graphics_D3D9_DeviceState_SetPixelShader( NULL ) ;
						Graphics_D3D9_DeviceState_SetIndexBuffer( NULL ) ;
						Graphics_D3D9_DeviceState_SetVertexBuffer( NULL ) ;
						Graphics_D3D9_DeviceState_SetFVF( D_D3DFVF_XYZ | D_D3DFVF_DIFFUSE ) ;
						Direct3DDevice9_DrawIndexedPrimitiveUP(
							D_D3DPT_TRIANGLELIST,
							0,
							( UINT )MBTList->VertexNum,
							( UINT )( MBTList->ToonOutLineIndexNum / 3 ),
							MBTList->ToonOutLineIndex,
							D_D3DFMT_INDEX16,
							( MV1_VERTEX_SIMPLE_TOL_D3D9 * )( ( ( DWORD_PTR )MBTListPF->D3D9.TempToonOutLineSimpleVertex + 15 ) / 16 * 16 ),
							sizeof( MV1_VERTEX_SIMPLE_TOL_D3D9 ) ) ;
#endif // NOT_RENDER
					}
					break ;

					// �S�{�[���ȓ��g���C�A���O�����X�g�ƂW�{�[���ȓ��g���C�A���O�����X�g
				case MV1_VERTEX_TYPE_SKIN_4BONE :
				case MV1_VERTEX_TYPE_SKIN_8BONE :
T_SD4BONESKIN:
					if( MBTList->VertexType == MV1_VERTEX_TYPE_SKIN_4BONE )
					{
						NowVertexShaderIndex    += D3D9_VERTEXSHADER_MESHTYPE( 1 ) ;
						NowVertexShaderIndex_PL += D3D9_PIXELLIGHTING_VERTEXSHADER_SKINMESH( 1 ) ;
					}
					else
					{
						NowVertexShaderIndex    += D3D9_VERTEXSHADER_MESHTYPE( 2 ) ;
						NowVertexShaderIndex_PL += D3D9_PIXELLIGHTING_VERTEXSHADER_SKINMESH( 2 ) ;
					}
		//			if( 0 )
					if( 1 )
		//			if( Shader )
					{
						int n ;
		//				D_IDirect3DVertexShader9 *UseVS ;

						// �V�F�[�_�[���g�p���ĕ`��

						if( MBTList->VertexBuffer == NULL || MBTList->VertexBuffer->PF->D3D9.VertexBuffer == NULL || MBTList->VertexBuffer->PF->D3D9.IndexBuffer == NULL )
							goto T_NONSDSKIN ;

						// ���[�U�[�ݒ���g�p����ꍇ
						if( MV1Man.UseOrigShaderFlag )
						{
							// ���_�V�F�[�_�[�̃Z�b�g�A�b�v
							if( GSYS.DrawSetting.UserShaderRenderInfo.SetVertexShaderHandle > 0 )
							{
								if( Graphics_D3D9_DeviceState_SetVertexShaderToHandle( GSYS.DrawSetting.UserShaderRenderInfo.SetVertexShaderHandle ) < 0 )
									goto T_NONSDSKIN ;
							}

							// �s�N�Z���V�F�[�_�[�̃Z�b�g�A�b�v
							if( GSYS.DrawSetting.UserShaderRenderInfo.SetPixelShaderHandle > 0 )
							{
								if( Graphics_D3D9_DeviceState_SetPixelShaderToHandle( GSYS.DrawSetting.UserShaderRenderInfo.SetPixelShaderHandle ) < 0 )
									goto T_NONSDSKIN ;
							}

							// �V�F�[�_�[�̃Z�b�g�A�b�v
							if( Graphics_D3D9_Shader_Model_Setup( 
								GSYS.DrawSetting.UserShaderRenderInfo.SetVertexShaderHandle > 0 ? -1 : NowVertexShaderIndex, NowVertexShaderIndex_PL,
								GSYS.DrawSetting.UserShaderRenderInfo.SetPixelShaderHandle  > 0 ? -1 : NowPixelShaderIndex,  NowPixelShaderIndex_PL ) == FALSE )
							{
								goto T_NONSDSKIN ;
							}
						}
						else
						{
							// �f�t�H���g����

							// �V�F�[�_�[�̃Z�b�g�A�b�v
							if( Graphics_D3D9_Shader_Model_Setup( 
									NowVertexShaderIndex, NowVertexShaderIndex_PL,
									NowPixelShaderIndex,  NowPixelShaderIndex_PL ) == FALSE )
							{
								goto T_NONSDSKIN ;
							}
						}

						// ���_�f�[�^�̃Z�b�g�A�b�v
						TOutLine = 0 ;

						BumpMap = D3D9_VERTEXSHADER_GET_BUMPMAP( NowVertexShaderIndex ) ;
						SkinMesh = D3D9_VERTEXSHADER_GET_MESHTYPE( NowVertexShaderIndex ) ;
/*
						TOutLine = VS_GET_TOONOUTLINE( NowVertexShaderIndex ) ;
						TOutLine = 0 ;

						BumpMap = VS_GET_BUMPMAP( NowVertexShaderIndex ) ;
						SkinMesh = VS_GET_SKINMESH( NowVertexShaderIndex ) ;
*/

						UVNum = TOutLine == 1 ? 0 : 1 ;
						Graphics_D3D9_DeviceState_SetMV1VertexDeclaration( BumpMap, SkinMesh, UVNum ) ;

						// �}�e���A���̃Z�b�g�A�b�v
						if( SetupShaderMaterial != 1 )
						{
							MV1_D3D9_SetupToonOutLineMeshDrawMaterial( MBase, &BlendInfo, Mesh, true ) ;
							SetupShaderMaterial = 1 ;
						}

						// ���_�o�b�t�@�̃Z�b�g�A�b�v
						if( TList->PF->D3D9.VertexBuffer )
						{
							Graphics_D3D9_DeviceState_SetVertexBuffer( TList->PF->D3D9.VertexBuffer,                 ( int )MBTList->VertexBuffer->UnitSize ) ;
						}
						else
						{
							Graphics_D3D9_DeviceState_SetVertexBuffer( MBTList->VertexBuffer->PF->D3D9.VertexBuffer, ( int )MBTList->VertexBuffer->UnitSize ) ;
						}

						// �C���f�b�N�X�o�b�t�@�̃Z�b�g�A�b�v
						Graphics_D3D9_DeviceState_SetIndexBuffer( MBTList->VertexBuffer->PF->D3D9.IndexBuffer ) ;

						// ���������`�悩�ǂ����ŏ����𕪊�
						if( MBase->UsePackDraw )
						{
							int l ;
							int UsePackIndex ;
							int TotalDrawStockNum ;
							int DrawStockNum ;
							MATRIX_4X4CT_F *PackDrawMatrix ;

							if( MBTList->VertexType == MV1_VERTEX_TYPE_NORMAL )
							{
								PackDrawMatrix = Model->PackDrawMatrix + MBase->SkinBoneNum + ( Frame - Model->Frame ) ;
							}
							else
							{
								PackDrawMatrix = Model->PackDrawMatrix ;
							}

							UsePackIndex = 0 ;
							for( TotalDrawStockNum = Model->PackDrawStockNum ; TotalDrawStockNum > 0 ; )
							{
								DrawStockNum = TotalDrawStockNum ;
								if( DrawStockNum > MBTList->PackDrawMaxNum )
								{
									DrawStockNum = MBTList->PackDrawMaxNum ;
								}

								if( MBTList->VertexType == MV1_VERTEX_TYPE_NORMAL )
								{
									for( l = 0 ; l < DrawStockNum ; l ++, PackDrawMatrix += MBase->PackDrawMatrixUnitNum )
									{
										TempMatrixTable[ l ] = *PackDrawMatrix ;
									}
								}
								else
								{
									int MatrixIndex ;

									MatrixIndex = 0 ;
									for( l = 0 ; l < DrawStockNum ; l ++, PackDrawMatrix += MBase->PackDrawMatrixUnitNum )
									{
										for( n = 0 ; n < MBTList->UseBoneNum ; n ++, MatrixIndex ++ )
										{
											TempMatrixTable[ MatrixIndex ] = PackDrawMatrix[ Frame->UseSkinBoneMatrix[ MBTList->UseBone[ n ] ] - Model->SkinBoneMatrix ] ;
										}
									}
								}

								Graphics_D3D9_ShaderConstant_InfoSet_SetParam(
									&GD3D9.Device.Shader.ShaderConstantInfo,
									DX_SHADERCONSTANTTYPE_VS_FLOAT,
									DX_SHADERCONSTANTSET_LIB,
									DX_VS_CONSTF_WORLD_MAT_START,
									TempMatrixTable,
									( MBTList->VertexType == MV1_VERTEX_TYPE_NORMAL ? 1 : MBTList->UseBoneNum ) * DrawStockNum * 3,
									FALSE
								) ;
#ifndef NOT_RENDER
								Direct3DDevice9_DrawIndexedPrimitive(
									D_D3DPT_TRIANGLELIST,
									TList->PF->D3D9.VertexBuffer ? 0 : MBTList->VBStartVertex,
									0,
									( UINT )( MBTList->VertexNum * DrawStockNum ),
									( UINT )MBTList->ToonOutLineVBStartIndex,
									( UINT )( MBTList->ToonOutLineIndexNum / 3 * DrawStockNum ) ) ;
#endif

								TotalDrawStockNum -= DrawStockNum ;
								UsePackIndex      += DrawStockNum ;
							}
						}
						else
						{
							if( GSYS.DrawSetting.Large3DPositionSupport )
							{
								if( UseWorldViewMatrix )
								{
									for( n = 0 ; n < MBTList->UseBoneNum ; n ++ )
									{
										ConvertMatrix4x4cDToMatrix4x4cF( &TempMatrixTable[ n ], &Frame->UseSkinBoneMatrix[ MBTList->UseBone[ n ] ]->md ) ;
									}
								}
								else
								{
									MATRIX_4X4CT_D ViewMat ;

									ConvertMatrixDToMatrix4x4cD( &ViewMat, &GSYS.DrawSetting.ViewMatrix ) ;
									for( n = 0 ; n < MBTList->UseBoneNum ; n ++ )
									{
										UnSafeMultiplyMatrix4X4CTD( &TempMatrixTableD[ n ], &Frame->UseSkinBoneMatrix[ MBTList->UseBone[ n ] ]->md, &ViewMat )  ;
										ConvertMatrix4x4cDToMatrix4x4cF( &TempMatrixTable[ n ], &TempMatrixTableD[ n ] ) ;
									}

									Graphics_D3D9_DeviceState_SetViewMatrix( &IdentityMat ) ;
								}
							}
							else
							{
								for( n = 0 ; n < MBTList->UseBoneNum ; n ++ )
								{
									TempMatrixTable[ n ] = Frame->UseSkinBoneMatrix[ MBTList->UseBone[ n ] ]->mf ;
								}
							}
							Graphics_D3D9_ShaderConstant_InfoSet_SetParam(
								&GD3D9.Device.Shader.ShaderConstantInfo,
								DX_SHADERCONSTANTTYPE_VS_FLOAT,
								DX_SHADERCONSTANTSET_LIB,
								DX_VS_CONSTF_WORLD_MAT_START,
								TempMatrixTable,
								MBTList->UseBoneNum * 3,
								FALSE
							) ;
#ifndef NOT_RENDER
							Direct3DDevice9_DrawIndexedPrimitive(
								D_D3DPT_TRIANGLELIST,
								TList->PF->D3D9.VertexBuffer ? 0 : MBTList->VBStartVertex,
								0,
								( UINT )MBTList->VertexNum,
								( UINT )MBTList->ToonOutLineVBStartIndex,
								( UINT )( MBTList->ToonOutLineIndexNum / 3 ) ) ;
#endif	// NOT_RENDER
							if( GSYS.DrawSetting.Large3DPositionSupport && UseWorldViewMatrix == FALSE )
							{
								Graphics_D3D9_DeviceState_SetViewMatrix( &GSYS.DrawSetting.ViewMatrixF ) ;
							}
						}
					}
					else
					{
		T_NONSDSKIN:
						// �}�e���A���̃Z�b�g�A�b�v
						if( SetupShaderMaterial != 0 )
						{
							MV1_D3D9_SetupToonOutLineMeshDrawMaterial( MBase, &BlendInfo, Mesh, false ) ;
							SetupShaderMaterial = 0 ;
						}

						// �Œ�@�\�p�C�v���C���ɖ߂�
						Graphics_D3D9_DeviceState_ResetVertexShader() ;
						Graphics_D3D9_DeviceState_ResetPixelShader() ;

						// �P�ʍs����Z�b�g
						if( MV1Man.WorldMatrixIsIdentity == FALSE )
						{
							MV1Man.WorldMatrixIsIdentity = TRUE ;
							Graphics_DrawSetting_SetTransformToWorld_Direct( &IdentityMat ) ;
						}

						// �e���|�����o�b�t�@���Z�b�g�A�b�v����Ă��Ȃ�������Z�b�g�A�b�v
						if( MBTListPF->D3D9.TempSimpleVertex == NULL )
							MV1_D3D9_SetupTListTempSimpleVertex( MBTList ) ;

						// �e���|�����o�b�t�@���Z�b�g�A�b�v����Ă��Ȃ�������Z�b�g�A�b�v
						if( MBTListPF->D3D9.TempToonOutLineSimpleVertex == NULL )
							MV1_D3D9_SetupTListTempToonOutLineSimpleVertex( MBTList ) ;

						// �P���_������̃f�[�^�T�C�Y���Z�o
						DestUnitSize = MBTList->TempUnitSize ;

						// �f�[�^�̃Z�b�g
						TONVert    = ( MV1_VERTEX_SIMPLE_TOL_D3D9 * )( ( ( DWORD_PTR )MBTListPF->D3D9.TempToonOutLineSimpleVertex + 15 ) / 16 * 16 ) ;
						SMPVert    = MBTListPF->D3D9.TempSimpleVertex ;
						VertexNum  = MBTList->VertexNum ;
						OutLineColor =
							( 255                                                    << 24 ) |
							( ( DWORD )_FTOL( Mesh->Material->OutLineColor.r * 255 ) << 16 ) |
							( ( DWORD )_FTOL( Mesh->Material->OutLineColor.g * 255 ) << 8  ) |
							( ( DWORD )_FTOL( Mesh->Material->OutLineColor.b * 255 )       ) ;

						for( j = 0 ; j < VertexNum ; j ++, TONVert ++, SMPVert = ( MV1_VERTEX_SIMPLE_D3D9  * )( ( BYTE * )SMPVert + DestUnitSize ) )
						{
							TONVert->Position.x = SMPVert->Position.x + SMPVert->Normal.x * OutLineWidth ;
							TONVert->Position.y = SMPVert->Position.y + SMPVert->Normal.y * OutLineWidth ;
							TONVert->Position.z = SMPVert->Position.z + SMPVert->Normal.z * OutLineWidth ;
							*( ( DWORD * )&TONVert->DiffuseColor ) = OutLineColor ;
						}

#ifndef NOT_RENDER
						// �`��
						Graphics_D3D9_DeviceState_SetVertexShader( NULL ) ;
						Graphics_D3D9_DeviceState_SetPixelShader( NULL ) ;
						Graphics_D3D9_DeviceState_SetIndexBuffer( NULL ) ;
						Graphics_D3D9_DeviceState_SetVertexBuffer( NULL ) ;
						Graphics_D3D9_DeviceState_SetFVF( D_D3DFVF_XYZ | D_D3DFVF_DIFFUSE ) ;
						Direct3DDevice9_DrawIndexedPrimitiveUP(
							D_D3DPT_TRIANGLELIST,
							0,
							( UINT )MBTList->VertexNum,
							( UINT )( MBTList->ToonOutLineIndexNum / 3 ),
							MBTList->ToonOutLineIndex,
							D_D3DFMT_INDEX16,
							( MV1_VERTEX_SIMPLE_TOL_D3D9 * )( ( ( DWORD_PTR )MBTListPF->D3D9.TempToonOutLineSimpleVertex + 15 ) / 16 * 16 ),
							sizeof( MV1_VERTEX_SIMPLE_TOL_D3D9 ) ) ;
#endif	// NOT_RENDER
					}
					break ;

					// �{�[�����������g���C�A���O�����X�g
				case MV1_VERTEX_TYPE_SKIN_FREEBONE :
					goto T_NONSDSKIN ;
				}
			}
		}

		// ���C�e�B���O�̗L���ݒ�����ɖ߂�
		Graphics_D3D9_DeviceState_SetLighting( GSYS.Light.ProcessDisable ? FALSE : TRUE ) ;
	}

//	SetUserBlendInfo( NULL ) ;
}



#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

#endif // DX_NON_DIRECT3D9

#endif // DX_NON_MODEL

