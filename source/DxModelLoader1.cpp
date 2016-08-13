// -------------------------------------------------------------------------------
// 
// 		�c�w���C�u����		�e�a�w���f���f�[�^�ǂݍ��݃v���O����
// 
// 				Ver 3.16d
// 
// -------------------------------------------------------------------------------

#define __DX_MAKE

#include "DxModelRead.h"

#ifndef DX_NON_MODEL
#ifdef DX_LOAD_FBX_MODEL

// �C���N���[�h ---------------------------------
#include "DxLog.h"

#include "fbxsdk.h"

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// �}�N����` -----------------------------------

// �f�[�^�錾 -----------------------------------

// �e�a�w�f�[�^�\����
struct FBX_MODEL
{
	FbxManager					*pManager ;
	FbxIOSettings				*pIOSettings ;
	FbxImporter					*pImporter ;
	FbxScene					*pScene ;
	FbxNode						*pNode ;
} ;

// �֐��錾 -------------------------------------

static int AnalyseFbx( MV1_MODEL_R *RModel, FBX_MODEL *Model ) ;														// FBX�t�@�C���̉��( 0:����  -1:���s )
static int AnalyseFbxNode( MV1_MODEL_R *RModel, FBX_MODEL *Model, MV1_FRAME_R *ParentFrame, FbxNode *pFbxNode ) ;		// �m�[�h�̉��( -1:�G���[ )
static int GetFbxAnimInfo( MV1_MODEL_R *RModel, FBX_MODEL *Model, MV1_FRAME_R *Frame, MV1_ANIMSET_R *AnimSet, MV1_ANIM_R **Anim, int DataType, FbxAnimCurve *FbxCurve, bool Reverse = false, bool DeggToRad = false ) ;	// �e�a�w�J�[�u����A�j���[�V���������擾����( -1:�G���[ )
static MV1_TEXTURE_R *FbxAddTexture( MV1_MODEL_R *RModel, FbxTexture *_FbxTexture ) ;				// �e�a�w�e�N�X�`����ǉ�����

// �v���O���� -----------------------------------

// �e�a�w�e�N�X�`����ǉ�����
static MV1_TEXTURE_R *FbxAddTexture( MV1_MODEL_R *RModel, FbxTexture *_FbxTexture )
{
	MV1_TEXTURE_R *Texture ;
	FbxFileTexture *pFbxFileTexture ;
	FbxProceduralTexture *pProceduralTexture ;
//	char *ANSIBuffer ;
//	char *ANSIBuffer2 ;
//	size_t ANSISize ;
//	size_t ANSISize2 ;
	int i ;

	pFbxFileTexture = FbxCast< FbxFileTexture >( _FbxTexture ) ;
	pProceduralTexture = FbxCast< FbxProceduralTexture >( _FbxTexture ) ;

	// �t�@�C���e�N�X�`���ł͂Ȃ��ꍇ�̓G���[
	if( pFbxFileTexture == NULL )
		return NULL ;

	// ���f����Ɋ��ɂ��̃e�N�X�`���̏�񂪂���ꍇ�̓A�h���X�����ۑ�����
	Texture = RModel->TextureFirst ;
	for( i = 0 ; i < ( int )RModel->TextureNum && Texture->UserData != _FbxTexture ; i ++, Texture = Texture->DataNext ){}
	if( i != RModel->TextureNum )
		return Texture ;

	// �e�N�X�`���̒ǉ�
//	FbxUTF8ToAnsi( _FbxTexture->GetName(),                 ANSIBuffer,  &ANSISize  ) ;
//	FbxUTF8ToAnsi( pFbxFileTexture->GetRelativeFileName(), ANSIBuffer2, &ANSISize2 ) ;
	Texture = MV1RAddTexture( RModel, _FbxTexture->GetName(), pFbxFileTexture->GetRelativeFileName(), NULL, FALSE, 0.1f, false ) ;
	if( Texture == NULL )
	{
		return NULL ;
	}

	// �e�a�w�̃A�h���X��ۑ�
	Texture->UserData = _FbxTexture ;

	// ���b�v���[�h��ۑ�
	Texture->AddressModeU = _FbxTexture->GetWrapModeU() == FbxTexture::eRepeat ? DX_TEXADDRESS_WRAP : DX_TEXADDRESS_CLAMP ;
	Texture->AddressModeV = _FbxTexture->GetWrapModeV() == FbxTexture::eRepeat ? DX_TEXADDRESS_WRAP : DX_TEXADDRESS_CLAMP ;

	// �t�u�X�P�[����ۑ�
	Texture->ScaleU = ( float )_FbxTexture->GetScaleU() ;
	Texture->ScaleV = ( float )_FbxTexture->GetScaleV() ;

	// �A�h���X��Ԃ�
	return Texture ;
}

// �e�a�w�J�[�u����A�j���[�V���������擾����( -1:�G���[ )
static int GetFbxAnimInfo(
	MV1_MODEL_R *RModel,
	FBX_MODEL *Model,
	MV1_FRAME_R *Frame,
	MV1_ANIMSET_R *AnimSet,
	MV1_ANIM_R **AnimP,
	int DataType,
	FbxAnimCurve *pFbxCurve,
	bool Reverse,
	bool DeggToRad
)
{
	MV1_ANIMKEYSET_R *KeySet ;
	float *KeyLinear ;
	float *KeyTime ;
	MV1_ANIM_R *Anim ;
	float FirstValue ;
	bool AllFirstValue ;
	int i, KeyNum ;

	// �J�[�u�� NULL �������牽�����Ȃ�
	if( pFbxCurve == NULL ) return 0 ;

	// �L�[�����������牽�����Ȃ�
	KeyNum = pFbxCurve->KeyGetCount() ;
	if( KeyNum == 0 )
		return 0 ;

	// �A�j���[�V�������܂��ǉ�����Ă��Ȃ��ꍇ�͒ǉ�
	if( *AnimP == NULL )
	{
		*AnimP = MV1RAddAnim( RModel, AnimSet ) ;
		if( *AnimP == NULL )
		{
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\x46\x00\x62\x00\x78\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x3a\x00\x20\x00\xa2\x30\xcb\x30\xe1\x30\xfc\x30\xb7\x30\xe7\x30\xf3\x30\xaa\x30\xd6\x30\xb8\x30\xa7\x30\xaf\x30\xc8\x30\x6e\x30\xfd\x8f\xa0\x52\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Fbx Load : �A�j���[�V�����I�u�W�F�N�g�̒ǉ��Ɏ��s���܂���\n" @*/ )) ;
			return -1 ;
		}
	}
	Anim = *AnimP ;

	// �L�[�Z�b�g�̒ǉ�
	KeySet = MV1RAddAnimKeySet( RModel, Anim ) ;
	if( KeySet == NULL )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\x46\x00\x62\x00\x78\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x3a\x00\x20\x00\xa2\x30\xcb\x30\xe1\x30\xfc\x30\xb7\x30\xe7\x30\xf3\x30\xad\x30\xfc\x30\xbb\x30\xc3\x30\xc8\x30\xaa\x30\xd6\x30\xb8\x30\xa7\x30\xaf\x30\xc8\x30\x6e\x30\xfd\x8f\xa0\x52\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Fbx Load : �A�j���[�V�����L�[�Z�b�g�I�u�W�F�N�g�̒ǉ��Ɏ��s���܂���\n" @*/ )) ;
		return -1 ;
	}

	// �f�[�^�^�C�v���Z�b�g
	KeySet->DataType = DataType ;

	// �L�[�^�C�v�͂Ƃ肠�������`���
	KeySet->Type = MV1_ANIMKEY_TYPE_LINEAR ;

	// �L�[���i�[���邽�߂̃������̈���m��
	KeySet->KeyLinear = ( float * )ADDMEMAREA( ( sizeof( float ) + sizeof( float ) ) * KeyNum, &RModel->Mem ) ;
	if( KeySet->KeyLinear == NULL )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\x46\x00\x62\x00\x78\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x3a\x00\x20\x00\xa2\x30\xcb\x30\xe1\x30\xfc\x30\xb7\x30\xe7\x30\xf3\x30\xad\x30\xfc\x30\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\x5f\x30\x81\x30\x6e\x30\xe1\x30\xe2\x30\xea\x30\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Fbx Load : �A�j���[�V�����L�[���i�[���邽�߂̃������̊m�ۂɎ��s���܂���\n" @*/ )) ;
		return -1 ;
	}
	KeySet->KeyTime = ( float * )( KeySet->KeyLinear + KeyNum ) ;

	// �ŏ��̃L�[�̒l���擾
	AllFirstValue = true ;
	FirstValue = static_cast< float >( pFbxCurve->KeyGetValue( 0 ) ) ;
	if( Reverse   ) FirstValue = -FirstValue ;
	if( DeggToRad ) FirstValue =  FirstValue * DX_PI_F / 180.0f ;

	// �L�[�̐������J��Ԃ�
	KeyLinear = KeySet->KeyLinear ;
	KeyTime = KeySet->KeyTime ;
	KeySet->TotalTime = 0.0f ;
	for( i = 0 ; i < KeyNum ; i ++ )
	{
		// ���Ԃ̎擾
		*KeyTime = ( float )pFbxCurve->KeyGetTime( i ).GetFrameCountPrecise() ;
		if( KeySet->TotalTime < *KeyTime ) KeySet->TotalTime = *KeyTime ;
		if( *KeyTime < AnimSet->StartTime ) AnimSet->StartTime = *KeyTime ;
		if( *KeyTime > AnimSet->EndTime   ) AnimSet->EndTime   = *KeyTime ;

		// �l�̎擾
		*KeyLinear = static_cast< float >( pFbxCurve->KeyGetValue( i ) ) ;
		if( Reverse ) *KeyLinear = -*KeyLinear ;
		if( DeggToRad ) *KeyLinear = *KeyLinear * DX_PI_F / 180.0f ;

		// �ŏ��̃p�����[�^�Ɠ�������r����
		if( AllFirstValue && FirstValue != *KeyLinear )
			AllFirstValue = false ;

		// ��ȏ㓯���L�[�������Ă���ꍇ�͓�������
		if( KeySet->Num >= 2 &&
			KeyLinear[ -2 ] == KeyLinear[ -1 ] &&
			KeyLinear[ -1 ] == KeyLinear[  0 ] )
		{
			KeyTime[ -1 ] = KeyTime[ 0 ] ;
		}
		else
		{
			// �L�[�̐������Z����
			KeySet->Num ++ ;
			KeyLinear ++ ;
			KeyTime ++ ;
		}
	}

	// �ŏ��̃L�[�ƑS�������ꍇ�̓L�[�̐����P�ɂ���
	if( AllFirstValue )
	{
		KeySet->Num = 1 ;
	}

	// �L�[�f�[�^�̃T�C�Y�����Z����
	RModel->AnimKeyDataSize += ( sizeof( float ) + sizeof( float ) ) * KeySet->Num ;

	// �I��
	return 0 ;
}

// �m�[�h�̉��( -1:�G���[ )
static int AnalyseFbxNode( MV1_MODEL_R *RModel, FBX_MODEL *Model, MV1_FRAME_R *ParentFrame, FbxNode *pFbxNode )
{
	FbxNode *pFbxChildNode ;
	FbxNodeAttribute *FbxAttr ;
	FbxVector4 *FbxVec ;
	FbxVector4 *FbxShapeVec ;
	FbxMesh *_FbxMesh ;
	FbxLayerElement::EMappingMode FbxMappingMode ;
	FbxLayerElement::EReferenceMode FbxRefMode ;
	FbxBlendShape *_FbxBlendShape ;
	FbxBlendShapeChannel *_FbxBlendShapeChannel ;
	FbxGeometryElementNormal *FbxNormalElem ;
	FbxGeometryElementBinormal *FbxBinormalElem ;
	FbxGeometryElementTangent *FbxTangentElem ;
	FbxGeometryElementVertexColor *FbxVertexColorElem ;
	FbxGeometryElementUV *FbxUVElem ;
	FbxGeometryElementMaterial *FbxMaterialElem ;
	FbxSurfaceMaterial *FbxMaterial, **FbxMaterialDim ;
	FbxSurfaceLambert *FbxLambert ;
	FbxSurfacePhong *FbxPhong ;
	FbxTexture *_FbxTexture ;
	FbxLayeredTexture *_FbxLayeredTexture ;
	FbxSkin *_FbxSkin ;
	FbxShape *_FbxShape ; 
	FbxCluster *_FbxCluster ;
	MV1_FRAME_R *Frame ;
	MV1_MESH_R *Mesh ;
	MV1_MESHFACE_R *MeshFace ;
	MV1_MATERIAL_R *Material ;
	MV1_SKIN_WEIGHT_R *SkinWeight ;
	MV1_SHAPE_R *Shape ;
	MV1_SHAPE_VERTEX_R *ShapeVert ;
	VECTOR *MeshPos ;
	MATRIX ReverseMat ;
	int i, j, k, l, Num, LayerNum, Index, MaterialNum ;
	char UTF16LE_Buffer[ 1024 ] ;

	// FbxNode �� NULL ��������g�b�v�m�[�h���Z�b�g�A�b�v����
	if( pFbxNode == NULL )
	{
		pFbxNode = Model->pScene->GetRootNode() ;
		Frame = NULL ;
	}
	else
	{
		// �t���[����ǉ�
		Frame = MV1RAddFrame( RModel, pFbxNode->GetName(), ParentFrame ) ;
		if( Frame == NULL )
		{
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\x46\x00\x62\x00\x78\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x3a\x00\x20\x00\xd5\x30\xec\x30\xfc\x30\xe0\x30\xaa\x30\xd6\x30\xb8\x30\xa7\x30\xaf\x30\xc8\x30\x6e\x30\xfd\x8f\xa0\x52\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Fbx Load : �t���[���I�u�W�F�N�g�̒ǉ��Ɏ��s���܂���\n" @*/ )) ;
			return -1 ;
		}

		// �E����W�n���獶����W�n�ɕϊ����邽�߂̍s��̏���
		CreateIdentityMatrix( &ReverseMat ) ;
		ReverseMat.m[ 2 ][ 2 ] = -1.0f ;

		// FBX�m�[�h�̃A�h���X��ۑ�
		Frame->UserData = pFbxNode ;

		// ���W�ϊ��l�̃p�����[�^���擾����
		{
			FbxVector4 lTmpVector;

			Frame->Translate.x = ( float ) pFbxNode->LclTranslation.Get()[ 0 ] ;
			Frame->Translate.y = ( float ) pFbxNode->LclTranslation.Get()[ 1 ] ;
			Frame->Translate.z = ( float )-pFbxNode->LclTranslation.Get()[ 2 ] ;

			Frame->Scale.x = ( float )pFbxNode->LclScaling.Get()[ 0 ] ;
			Frame->Scale.y = ( float )pFbxNode->LclScaling.Get()[ 1 ] ;
			Frame->Scale.z = ( float )pFbxNode->LclScaling.Get()[ 2 ] ;

			Frame->Rotate.x = ( float )( -pFbxNode->LclRotation.Get()[ 0 ] * DX_PI / 180.0f ) ;
			Frame->Rotate.y = ( float )( -pFbxNode->LclRotation.Get()[ 1 ] * DX_PI / 180.0f ) ;
			Frame->Rotate.z = ( float )(  pFbxNode->LclRotation.Get()[ 2 ] * DX_PI / 180.0f ) ;

			Frame->RotateOrder = ( int )pFbxNode->RotationOrder.Get() ;

			lTmpVector = pFbxNode->GetPreRotation( FbxNode::eSourcePivot ) ;
			Frame->PreRotate.x = ( float )lTmpVector[ 0 ] ;
			Frame->PreRotate.y = ( float )lTmpVector[ 1 ] ;
			Frame->PreRotate.z = ( float )lTmpVector[ 2 ] ;
			if( *( ( DWORD * )&Frame->PreRotate.x ) != 0 || 
				*( ( DWORD * )&Frame->PreRotate.y ) != 0 ||
				*( ( DWORD * )&Frame->PreRotate.z ) != 0 )
			{
				Frame->PreRotate.x = -Frame->PreRotate.x * DX_PI_F / 180.0f ;
				Frame->PreRotate.y = -Frame->PreRotate.y * DX_PI_F / 180.0f ;
				Frame->PreRotate.z =  Frame->PreRotate.z * DX_PI_F / 180.0f ;
				Frame->EnablePreRotate = 1 ;
			}

			lTmpVector = pFbxNode->GetPostRotation( FbxNode::eSourcePivot ) ;
			Frame->PostRotate.x = ( float )lTmpVector[ 0 ] ;
			Frame->PostRotate.y = ( float )lTmpVector[ 1 ] ;
			Frame->PostRotate.z = ( float )lTmpVector[ 2 ] ;
			if( *( ( DWORD * )&Frame->PostRotate.x ) != 0 || 
				*( ( DWORD * )&Frame->PostRotate.y ) != 0 ||
				*( ( DWORD * )&Frame->PostRotate.z ) != 0 )
			{
				Frame->PostRotate.x = -Frame->PostRotate.x * DX_PI_F / 180.0f ;
				Frame->PostRotate.y = -Frame->PostRotate.y * DX_PI_F / 180.0f ;
				Frame->PostRotate.z =  Frame->PostRotate.z * DX_PI_F / 180.0f ;
				Frame->EnablePostRotate = 1 ;
			}
		}

		// �A�g���r���[�g���擾
		FbxAttr = pFbxNode->GetNodeAttribute() ;
		if( FbxAttr )
		{
			// ���b�V���m�[�h�̏ꍇ�̓��b�V�����擾����
			if( FbxAttr->GetAttributeType() == FbxNodeAttribute::eMesh )
			{
				// ���b�V����ǉ�
				Mesh = MV1RAddMesh( RModel, Frame ) ;
				if( Mesh == NULL )
				{
					DXST_ERRORLOGFMT_ADDUTF16LE(( "\x46\x00\x62\x00\x78\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x3a\x00\x20\x00\xe1\x30\xc3\x30\xb7\x30\xe5\x30\xaa\x30\xd6\x30\xb8\x30\xa7\x30\xaf\x30\xc8\x30\x6e\x30\xfd\x8f\xa0\x52\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Fbx Load : ���b�V���I�u�W�F�N�g�̒ǉ��Ɏ��s���܂���\n" @*/ )) ;
					return -1 ;
				}

				// ���b�V���̃A�h���X���擾
				_FbxMesh = ( FbxMesh * )FbxAttr ;

				// ���C���[�̐����擾
				LayerNum = _FbxMesh->GetLayerCount() ;

				// �ʂ̏����擾
				{
					// �ʏ����i�[���郁�����̈�̊m��
					if( MV1RSetupMeshFaceBuffer( RModel, Mesh, _FbxMesh->GetPolygonCount(), 4 ) < 0 )
					{
						DXST_ERRORLOGFMT_ADDUTF16LE(( "\x46\x00\x62\x00\x78\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x3a\x00\x20\x00\x62\x97\xc5\x60\x31\x58\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Fbx Load : �ʏ����i�[���郁�����̊m�ۂɎ��s���܂���\n" @*/ )) ;
						return -1 ;
					}

					// �ʂ̏����擾
					MeshFace = Mesh->Faces ;
					for( i = 0 ; i < ( int )Mesh->FaceNum ; i ++, MeshFace ++ )
					{
						// �C���f�b�N�X�̐����擾
						MeshFace->IndexNum = _FbxMesh->GetPolygonSize( i ) ;
						if( Mesh->FaceUnitMaxIndexNum < MeshFace->IndexNum )
						{
							if( MV1RSetupMeshFaceBuffer( RModel, Mesh, Mesh->FaceNum, MeshFace->IndexNum ) < 0 )
							{
								DXST_ERRORLOGFMT_ADDUTF16LE(( "\x46\x00\x62\x00\x78\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x3a\x00\x20\x00\x62\x97\xc5\x60\x31\x58\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x6e\x30\x8d\x51\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Fbx Load : �ʏ����i�[���郁�����̍Ċm�ۂɎ��s���܂���\n" @*/ )) ;
								return -1 ;
							}
							MeshFace = Mesh->Faces + i ;
						}

						// �C���f�b�N�X���擾
						for( j = 0 ; j < ( int )MeshFace->IndexNum ; j ++ )
						{
							MeshFace->VertexIndex[ j ] = _FbxMesh->GetPolygonVertex( i, j ) ;
						}
					}
				}

				// ���_���W�̏����擾
				{
					// ���W���̐����擾
					Mesh->PositionNum = _FbxMesh->GetControlPointsCount() ;

					// ���W�����i�[���郁�����̈�̊m��
					Mesh->Positions = ( VECTOR * )ADDMEMAREA( sizeof( VECTOR ) * Mesh->PositionNum, &RModel->Mem ) ;
					if( Mesh->Positions == NULL )
					{
						DXST_ERRORLOGFMT_ADDUTF16LE(( "\x46\x00\x62\x00\x78\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x3a\x00\x20\x00\x02\x98\xb9\x70\xa7\x5e\x19\x6a\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Fbx Load : ���_���W���i�[���郁�����̊m�ۂɎ��s���܂���\n" @*/ )) ;
						return -1 ;
					}

					// ���W�̏����擾
					MeshPos = Mesh->Positions ;
					FbxVec = _FbxMesh->GetControlPoints() ;
					for( i = 0 ; i < ( int )Mesh->PositionNum ; i ++, FbxVec ++, MeshPos ++ )
					{
						MeshPos->x = ( float ) FbxVec->mData[ 0 ] ;
						MeshPos->y = ( float ) FbxVec->mData[ 1 ] ;
						MeshPos->z = ( float )-FbxVec->mData[ 2 ] ;
					}
				}

				// �@���̏����擾
				{

					// �@���G�������g�̐������J��Ԃ�
					for( i = 0 ; i < _FbxMesh->GetElementNormalCount() ; i ++ )
					{
			            FbxNormalElem = _FbxMesh->GetElementNormal( i ) ;

						// ���[�h�̎擾
						FbxRefMode     = FbxNormalElem->GetReferenceMode() ;
						FbxMappingMode = FbxNormalElem->GetMappingMode() ;

						switch( FbxRefMode )
						{
						case FbxGeometryElement::eDirect :
							// �@�����̐����擾
							Mesh->NormalNum = FbxNormalElem->GetDirectArray().GetCount() ;

							// �@�������i�[���郁�����̈�̊m��
							Mesh->Normals = ( VECTOR * )ADDMEMAREA( sizeof( VECTOR ) * Mesh->NormalNum, &RModel->Mem ) ;
							if( Mesh->Normals == NULL )
							{
								DXST_ERRORLOGFMT_ADDUTF16LE(( "\x46\x00\x62\x00\x78\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x3a\x00\x20\x00\xd5\x6c\xda\x7d\xc5\x60\x31\x58\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Fbx Load : �@�������i�[���郁�����̊m�ۂɎ��s���܂���\n" @*/ )) ;
								return -1 ;
							}

							// ���ڃ��[�h�̏ꍇ�͂��̂܂ܑ��
							for( j = 0 ; ( DWORD )j < Mesh->NormalNum ; j ++ )
							{
								Mesh->Normals[ j ].x = ( float ) FbxNormalElem->GetDirectArray().GetAt( j )[ 0 ] ;
								Mesh->Normals[ j ].y = ( float ) FbxNormalElem->GetDirectArray().GetAt( j )[ 1 ] ;
								Mesh->Normals[ j ].z = ( float )-FbxNormalElem->GetDirectArray().GetAt( j )[ 2 ] ;
							}
							break ;

						case FbxGeometryElement::eIndexToDirect :
							// �C���f�b�N�X�̐����擾
							Mesh->NormalNum = FbxNormalElem->GetIndexArray().GetCount() ;

							// �@�������i�[���郁�����̈�̊m��
							Mesh->Normals = ( VECTOR * )ADDMEMAREA( sizeof( VECTOR ) * Mesh->NormalNum, &RModel->Mem ) ;
							if( Mesh->Normals == NULL )
							{
								DXST_ERRORLOGFMT_ADDUTF16LE(( "\x46\x00\x62\x00\x78\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x3a\x00\x20\x00\xd5\x6c\xda\x7d\xc5\x60\x31\x58\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Fbx Load : �@�������i�[���郁�����̊m�ۂɎ��s���܂���\n" @*/ )) ;
								return -1 ;
							}

							// �C���f�b�N�X���[�h�̏ꍇ�͊ԐڎQ�Ƒ��
							for( j = 0 ; ( DWORD )j < Mesh->NormalNum ; j ++ )
							{
								Index = FbxNormalElem->GetIndexArray().GetAt( j ) ;
								Mesh->Normals[ j ].x = ( float ) FbxNormalElem->GetDirectArray().GetAt( Index )[ 0 ] ;
								Mesh->Normals[ j ].y = ( float ) FbxNormalElem->GetDirectArray().GetAt( Index )[ 1 ] ;
								Mesh->Normals[ j ].z = ( float )-FbxNormalElem->GetDirectArray().GetAt( Index )[ 2 ] ;
							}
							break ;

						default :
							DXST_ERRORLOGFMT_ADDUTF16LE(( "\x46\x00\x62\x00\x78\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x3a\x00\x20\x00\x5e\x97\xfe\x5b\xdc\x5f\x6e\x30\xd5\x6c\xda\x7d\xea\x30\xd5\x30\xa1\x30\xec\x30\xf3\x30\xb9\x30\xe2\x30\xfc\x30\xc9\x30\x4c\x30\x7f\x4f\x28\x75\x55\x30\x8c\x30\x66\x30\x44\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Fbx Load : ��Ή��̖@�����t�@�����X���[�h���g�p����Ă��܂���\n" @*/ )) ;
							return -1 ;
						}

						// �ʂ̊e���_�ɑΉ�����@���̏����擾
						switch( FbxMappingMode )
						{
						case FbxGeometryElement::eByControlPoint :
							// �@���C���f�b�N�X�͒��_�C���f�b�N�X�Ɠ������Ȃ�
							MeshFace = Mesh->Faces ;
							for( j = 0 ; ( DWORD )j < Mesh->FaceNum ; j ++, MeshFace ++ )
							{
								MeshFace->NormalIndex[ 0 ] = MeshFace->VertexIndex[ 0 ] ;
								MeshFace->NormalIndex[ 1 ] = MeshFace->VertexIndex[ 1 ] ;
								MeshFace->NormalIndex[ 2 ] = MeshFace->VertexIndex[ 2 ] ;
								MeshFace->NormalIndex[ 3 ] = MeshFace->VertexIndex[ 3 ] ;
							}
							break ;

						case FbxGeometryElement::eByPolygonVertex :
							// �@���C���f�b�N�X�͕ʌő��݂���
							MeshFace = Mesh->Faces ;
							k = 0 ;
							for( j = 0 ; ( DWORD )j < Mesh->FaceNum ; j ++, MeshFace ++ )
							{
								for( l = 0 ; ( DWORD )l < MeshFace->IndexNum ; l ++ )
								{
									MeshFace->NormalIndex[ l ] = k ;
									k ++ ;

									// �@��̐�������Ȃ��o�O�f�[�^�̑Ή�
									if( k == Mesh->NormalNum )
									{
										k = 0 ;
									}
								}
							}
							break ;

						default :
							DXST_ERRORLOGFMT_ADDUTF16LE(( "\x46\x00\x62\x00\x78\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x3a\x00\x20\x00\x5e\x97\xfe\x5b\xdc\x5f\x6e\x30\xd5\x6c\xda\x7d\xde\x30\xc3\x30\xd4\x30\xf3\x30\xb0\x30\xe2\x30\xfc\x30\xc9\x30\x4c\x30\x7f\x4f\x28\x75\x55\x30\x8c\x30\x66\x30\x44\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Fbx Load : ��Ή��̖@���}�b�s���O���[�h���g�p����Ă��܂���\n" @*/ )) ;
							return -1 ;
						}
					}

					// �]�@���G�������g�̐������J��Ԃ�
					for( i = 0 ; i < _FbxMesh->GetElementBinormalCount() ; i ++ )
					{
			            FbxBinormalElem = _FbxMesh->GetElementBinormal( i ) ;

						// ���[�h�̎擾
						FbxRefMode     = FbxBinormalElem->GetReferenceMode() ;
						FbxMappingMode = FbxBinormalElem->GetMappingMode() ;

						switch( FbxRefMode )
						{
						case FbxGeometryElement::eDirect :
							// �]�@�����̐����@�����̐��ƈقȂ��Ă����疳��
							if( Mesh->NormalNum != FbxBinormalElem->GetDirectArray().GetCount() )
							{
								continue ;
							}

							// �]�@�������i�[���郁�����̈�̊m��
							Mesh->Binormals = ( VECTOR * )ADDMEMAREA( sizeof( VECTOR ) * Mesh->NormalNum, &RModel->Mem ) ;
							if( Mesh->Binormals == NULL )
							{
								DXST_ERRORLOGFMT_ADDUTF16LE(( "\x46\x00\x62\x00\x78\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x3a\x00\x20\x00\x93\x5f\xd5\x6c\xda\x7d\xc5\x60\x31\x58\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Fbx Load : �]�@�������i�[���郁�����̊m�ۂɎ��s���܂���\n" @*/ )) ;
								return -1 ;
							}

							// ���ڃ��[�h�̏ꍇ�͂��̂܂ܑ��
							for( j = 0 ; ( DWORD )j < Mesh->NormalNum ; j ++ )
							{
								Mesh->Binormals[ j ].x = ( float ) FbxBinormalElem->GetDirectArray().GetAt( j )[ 0 ] ;
								Mesh->Binormals[ j ].y = ( float ) FbxBinormalElem->GetDirectArray().GetAt( j )[ 1 ] ;
								Mesh->Binormals[ j ].z = ( float )-FbxBinormalElem->GetDirectArray().GetAt( j )[ 2 ] ;
							}
							break ;

						case FbxGeometryElement::eIndexToDirect :
							// �C���f�b�N�X�̐����@�����̐��ƈقȂ��Ă����疳��
							if( Mesh->NormalNum != FbxBinormalElem->GetIndexArray().GetCount() )
							{
								continue ;
							}

							// �]�@�������i�[���郁�����̈�̊m��
							Mesh->Binormals = ( VECTOR * )ADDMEMAREA( sizeof( VECTOR ) * Mesh->NormalNum, &RModel->Mem ) ;
							if( Mesh->Binormals == NULL )
							{
								DXST_ERRORLOGFMT_ADDUTF16LE(( "\x46\x00\x62\x00\x78\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x3a\x00\x20\x00\x93\x5f\xd5\x6c\xda\x7d\xc5\x60\x31\x58\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Fbx Load : �]�@�������i�[���郁�����̊m�ۂɎ��s���܂���\n" @*/ )) ;
								return -1 ;
							}

							// �C���f�b�N�X���[�h�̏ꍇ�͊ԐڎQ�Ƒ��
							for( j = 0 ; ( DWORD )j < Mesh->NormalNum ; j ++ )
							{
								Index = FbxBinormalElem->GetIndexArray().GetAt( j ) ;
								Mesh->Binormals[ j ].x = ( float ) FbxBinormalElem->GetDirectArray().GetAt( Index )[ 0 ] ;
								Mesh->Binormals[ j ].y = ( float ) FbxBinormalElem->GetDirectArray().GetAt( Index )[ 1 ] ;
								Mesh->Binormals[ j ].z = ( float )-FbxBinormalElem->GetDirectArray().GetAt( Index )[ 2 ] ;
							}
							break ;

						default :
							DXST_ERRORLOGFMT_ADDUTF16LE(( "\x46\x00\x62\x00\x78\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x3a\x00\x20\x00\x5e\x97\xfe\x5b\xdc\x5f\x6e\x30\x93\x5f\xd5\x6c\xda\x7d\xea\x30\xd5\x30\xa1\x30\xec\x30\xf3\x30\xb9\x30\xe2\x30\xfc\x30\xc9\x30\x4c\x30\x7f\x4f\x28\x75\x55\x30\x8c\x30\x66\x30\x44\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Fbx Load : ��Ή��̏]�@�����t�@�����X���[�h���g�p����Ă��܂���\n" @*/ )) ;
							return -1 ;
						}
					}

					// �ڐ��G�������g�̐������J��Ԃ�
					for( i = 0 ; i < _FbxMesh->GetElementTangentCount() ; i ++ )
					{
			            FbxTangentElem = _FbxMesh->GetElementTangent( i ) ;

						// ���[�h�̎擾
						FbxRefMode     = FbxTangentElem->GetReferenceMode() ;
						FbxMappingMode = FbxTangentElem->GetMappingMode() ;

						switch( FbxRefMode )
						{
						case FbxGeometryElement::eDirect :
							// �ڐ����̐����@�����̐��ƈقȂ��Ă����疳��
							if( Mesh->NormalNum != FbxTangentElem->GetDirectArray().GetCount() )
							{
								continue ;
							}

							// �ڐ������i�[���郁�����̈�̊m��
							Mesh->Tangents = ( VECTOR * )ADDMEMAREA( sizeof( VECTOR ) * Mesh->NormalNum, &RModel->Mem ) ;
							if( Mesh->Tangents == NULL )
							{
								DXST_ERRORLOGFMT_ADDUTF16LE(( "\x46\x00\x62\x00\x78\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x3a\x00\x20\x00\xa5\x63\xda\x7d\xc5\x60\x31\x58\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Fbx Load : �ڐ������i�[���郁�����̊m�ۂɎ��s���܂���\n" @*/ )) ;
								return -1 ;
							}

							// ���ڃ��[�h�̏ꍇ�͂��̂܂ܑ��
							for( j = 0 ; ( DWORD )j < Mesh->NormalNum ; j ++ )
							{
								Mesh->Tangents[ j ].x = ( float ) FbxTangentElem->GetDirectArray().GetAt( j )[ 0 ] ;
								Mesh->Tangents[ j ].y = ( float ) FbxTangentElem->GetDirectArray().GetAt( j )[ 1 ] ;
								Mesh->Tangents[ j ].z = ( float )-FbxTangentElem->GetDirectArray().GetAt( j )[ 2 ] ;
							}
							break ;

						case FbxGeometryElement::eIndexToDirect :
							// �C���f�b�N�X�̐����@�����̐��ƈقȂ��Ă����疳��
							if( Mesh->NormalNum != FbxTangentElem->GetIndexArray().GetCount() )
							{
								continue ;
							}

							// �ڐ������i�[���郁�����̈�̊m��
							Mesh->Tangents = ( VECTOR * )ADDMEMAREA( sizeof( VECTOR ) * Mesh->NormalNum, &RModel->Mem ) ;
							if( Mesh->Tangents == NULL )
							{
								DXST_ERRORLOGFMT_ADDUTF16LE(( "\x46\x00\x62\x00\x78\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x3a\x00\x20\x00\xa5\x63\xda\x7d\xc5\x60\x31\x58\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Fbx Load : �ڐ������i�[���郁�����̊m�ۂɎ��s���܂���\n" @*/ )) ;
								return -1 ;
							}

							// �C���f�b�N�X���[�h�̏ꍇ�͊ԐڎQ�Ƒ��
							for( j = 0 ; ( DWORD )j < Mesh->NormalNum ; j ++ )
							{
								Index = FbxTangentElem->GetIndexArray().GetAt( j ) ;
								Mesh->Tangents[ j ].x = ( float ) FbxTangentElem->GetDirectArray().GetAt( Index )[ 0 ] ;
								Mesh->Tangents[ j ].y = ( float ) FbxTangentElem->GetDirectArray().GetAt( Index )[ 1 ] ;
								Mesh->Tangents[ j ].z = ( float )-FbxTangentElem->GetDirectArray().GetAt( Index )[ 2 ] ;
							}
							break ;

						default :
							DXST_ERRORLOGFMT_ADDUTF16LE(( "\x46\x00\x62\x00\x78\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x3a\x00\x20\x00\x5e\x97\xfe\x5b\xdc\x5f\x6e\x30\xa5\x63\xda\x7d\xea\x30\xd5\x30\xa1\x30\xec\x30\xf3\x30\xb9\x30\xe2\x30\xfc\x30\xc9\x30\x4c\x30\x7f\x4f\x28\x75\x55\x30\x8c\x30\x66\x30\x44\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Fbx Load : ��Ή��̐ڐ����t�@�����X���[�h���g�p����Ă��܂���\n" @*/ )) ;
							return -1 ;
						}
					}
				}

				// ���_�J���[�̏����擾
				{
					// ���_�J���[�G�������g�̐������J��Ԃ�
					for( i = 0 ; i < _FbxMesh->GetElementVertexColorCount() ; i ++ )
					{
						FbxVertexColorElem = _FbxMesh->GetElementVertexColor( i ) ;

						// ���[�h�̎擾
						FbxMappingMode = FbxVertexColorElem->GetMappingMode() ;
						FbxRefMode = FbxVertexColorElem->GetReferenceMode() ;

						// ���_�J���[���̎擾
						switch( FbxRefMode )
						{
						case FbxGeometryElement::eDirect :
							// ���_�J���[���̐����擾
							Mesh->VertexColorNum = FbxVertexColorElem->GetDirectArray().GetCount() ;

							// ���_�J���[�����i�[���郁�����̈�̊m��
							Mesh->VertexColors = ( COLOR_F * )ADDMEMAREA( sizeof( COLOR_F ) * Mesh->VertexColorNum, &RModel->Mem ) ;
							if( Mesh->VertexColors == NULL )
							{
								DXST_ERRORLOGFMT_ADDUTF16LE(( "\x46\x00\x62\x00\x78\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x3a\x00\x20\x00\x02\x98\xb9\x70\xab\x30\xe9\x30\xfc\x30\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Fbx Load : ���_�J���[���i�[���郁�����̊m�ۂɎ��s���܂���\n" @*/ )) ;
								return -1 ;
							}

							// ���ڃ��[�h�̏ꍇ�͂��̂܂ܑ��
							for( j = 0 ; ( DWORD )j < Mesh->VertexColorNum ; j ++ )
							{
								Mesh->VertexColors[ j ].r = ( float )FbxVertexColorElem->GetDirectArray().GetAt( j ).mRed ;
								Mesh->VertexColors[ j ].g = ( float )FbxVertexColorElem->GetDirectArray().GetAt( j ).mGreen ;
								Mesh->VertexColors[ j ].b = ( float )FbxVertexColorElem->GetDirectArray().GetAt( j ).mBlue ;
								Mesh->VertexColors[ j ].a = ( float )FbxVertexColorElem->GetDirectArray().GetAt( j ).mAlpha ;
							}
							break ;

						case FbxGeometryElement::eIndexToDirect :
							// �C���f�b�N�X�̐����擾
							Mesh->VertexColorNum = FbxVertexColorElem->GetIndexArray().GetCount() ;

							// ���_�J���[�����i�[���郁�����̈�̊m��
							Mesh->VertexColors = ( COLOR_F * )ADDMEMAREA( sizeof( COLOR_F ) * Mesh->VertexColorNum, &RModel->Mem ) ;
							if( Mesh->VertexColors == NULL )
							{
								DXST_ERRORLOGFMT_ADDUTF16LE(( "\x46\x00\x62\x00\x78\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x3a\x00\x20\x00\x02\x98\xb9\x70\xab\x30\xe9\x30\xfc\x30\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Fbx Load : ���_�J���[���i�[���郁�����̊m�ۂɎ��s���܂���\n" @*/ )) ;
								return -1 ;
							}

							// �C���f�b�N�X���[�h�̏ꍇ�͊ԐڎQ�Ƒ��
							for( j = 0 ; ( DWORD )j < Mesh->VertexColorNum ; j ++ )
							{
								Index = FbxVertexColorElem->GetIndexArray().GetAt( j ) ;
								Mesh->VertexColors[ j ].r = ( float )FbxVertexColorElem->GetDirectArray().GetAt( Index ).mRed ;
								Mesh->VertexColors[ j ].g = ( float )FbxVertexColorElem->GetDirectArray().GetAt( Index ).mGreen ;
								Mesh->VertexColors[ j ].b = ( float )FbxVertexColorElem->GetDirectArray().GetAt( Index ).mBlue ;
								Mesh->VertexColors[ j ].a = ( float )FbxVertexColorElem->GetDirectArray().GetAt( Index ).mAlpha ;
							}
							break ;

						default :
							DXST_ERRORLOGFMT_ADDUTF16LE(( "\x46\x00\x62\x00\x78\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x3a\x00\x20\x00\x5e\x97\xfe\x5b\xdc\x5f\x6e\x30\x02\x98\xb9\x70\xab\x30\xe9\x30\xfc\x30\xea\x30\xd5\x30\xa1\x30\xec\x30\xf3\x30\xb9\x30\xe2\x30\xfc\x30\xc9\x30\x4c\x30\x7f\x4f\x28\x75\x55\x30\x8c\x30\x66\x30\x44\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Fbx Load : ��Ή��̒��_�J���[���t�@�����X���[�h���g�p����Ă��܂���\n" @*/ )) ;
							return -1 ;
						}

						// �ʂ̊e���_�ɑΉ����钸�_�J���[�̏����擾
						switch( FbxMappingMode )
						{
						case FbxGeometryElement::eByControlPoint :
							// ���_�J���[�C���f�b�N�X�͒��_�C���f�b�N�X�Ɠ������Ȃ�
							MeshFace = Mesh->Faces ;
							for( j = 0 ; ( DWORD )j < Mesh->FaceNum ; j ++, MeshFace ++ )
							{
								MeshFace->VertexColorIndex[ 0 ] = MeshFace->VertexIndex[ 0 ] ;
								MeshFace->VertexColorIndex[ 1 ] = MeshFace->VertexIndex[ 1 ] ;
								MeshFace->VertexColorIndex[ 2 ] = MeshFace->VertexIndex[ 2 ] ;
								MeshFace->VertexColorIndex[ 3 ] = MeshFace->VertexIndex[ 3 ] ;
							}
							break ;

						case FbxGeometryElement::eByPolygonVertex :
							// ���_�J���[�C���f�b�N�X�͕ʌő��݂���
							MeshFace = Mesh->Faces ;
							k = 0 ;
							for( j = 0 ; ( DWORD )j < Mesh->FaceNum ; j ++, MeshFace ++ )
							{
								for( l = 0 ; ( DWORD )l < MeshFace->IndexNum ; l ++, k ++ )
									MeshFace->VertexColorIndex[ l ] = k ;
							}
							break ;

						default :
							DXST_ERRORLOGFMT_ADDUTF16LE(( "\x46\x00\x62\x00\x78\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x3a\x00\x20\x00\x5e\x97\xfe\x5b\xdc\x5f\x6e\x30\x02\x98\xb9\x70\xab\x30\xe9\x30\xfc\x30\xde\x30\xc3\x30\xd4\x30\xf3\x30\xb0\x30\xe2\x30\xfc\x30\xc9\x30\x4c\x30\x7f\x4f\x28\x75\x55\x30\x8c\x30\x66\x30\x44\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Fbx Load : ��Ή��̒��_�J���[�}�b�s���O���[�h���g�p����Ă��܂���\n" @*/ )) ;
							return -1 ;
						}
					}
				}

				// �t�u�̏����擾
				{
					// �t�u�G�������g�̐������J��Ԃ�
					for( i = 0 ; i < _FbxMesh->GetElementUVCount() ; i ++ )
					{
						FbxUVElem = _FbxMesh->GetElementUV( i ) ;

						// ���[�h�̎擾
						FbxMappingMode = FbxUVElem->GetMappingMode() ;
						FbxRefMode = FbxUVElem->GetReferenceMode() ;

						// �t�u���̎擾
						switch( FbxRefMode )
						{
						case FbxGeometryElement::eDirect :
							// �t�u���̐����擾
							Mesh->UVNum[ i ] = FbxUVElem->GetDirectArray().GetCount() ;

							// �t�u�����i�[���郁�����̈�̊m��
							Mesh->UVs[ i ] = ( FLOAT4 * )ADDMEMAREA( sizeof( FLOAT4 ) * Mesh->UVNum[ i ], &RModel->Mem ) ;
							if( Mesh->UVs[ i ] == NULL )
							{
								DXST_ERRORLOGFMT_ADDUTF16LE(( "\x46\x00\x62\x00\x78\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x3a\x00\x20\x00\x55\x00\x56\x00\xa7\x5e\x19\x6a\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Fbx Load : UV���W���i�[���郁�����̊m�ۂɎ��s���܂���\n" @*/ )) ;
								return -1 ;
							}

							// ���ڃ��[�h�̏ꍇ�͂��̂܂ܑ��
							for( j = 0 ; ( DWORD )j < Mesh->UVNum[ i ] ; j ++ )
							{
								Mesh->UVs[ i ][ j ].x = ( float )FbxUVElem->GetDirectArray().GetAt( j )[ 0 ] ;
								Mesh->UVs[ i ][ j ].y = 1.0f - ( float )FbxUVElem->GetDirectArray().GetAt( j )[ 1 ] ;
							}
							break ;

						case FbxGeometryElement::eIndexToDirect :
							// �C���f�b�N�X�̐����擾
							Mesh->UVNum[ i ] = FbxUVElem->GetIndexArray().GetCount() ;

							// �t�u�����i�[���郁�����̈�̊m��
							Mesh->UVs[ i ] = ( FLOAT4 * )ADDMEMAREA( sizeof( FLOAT4 ) * Mesh->UVNum[ i ], &RModel->Mem ) ;
							if( Mesh->UVs[ i ] == NULL )
							{
								DXST_ERRORLOGFMT_ADDUTF16LE(( "\x46\x00\x62\x00\x78\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x3a\x00\x20\x00\x55\x00\x56\x00\xa7\x5e\x19\x6a\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Fbx Load : UV���W���i�[���郁�����̊m�ۂɎ��s���܂���\n" @*/ )) ;
								return -1 ;
							}

							// �C���f�b�N�X���[�h�̏ꍇ�͊ԐڎQ�Ƒ��
							for( j = 0 ; ( DWORD )j < Mesh->UVNum[ i ] ; j ++ )
							{
								Index = FbxUVElem->GetIndexArray().GetAt( j ) ;
								Mesh->UVs[ i ][ j ].x = ( float )FbxUVElem->GetDirectArray().GetAt( Index )[ 0 ] ;
								Mesh->UVs[ i ][ j ].y = 1.0f - ( float )FbxUVElem->GetDirectArray().GetAt( Index )[ 1 ] ;
							}
							break ;

						default :
							DXST_ERRORLOGFMT_ADDUTF16LE(( "\x46\x00\x62\x00\x78\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x3a\x00\x20\x00\x5e\x97\xfe\x5b\xdc\x5f\x6e\x30\x35\xff\x36\xff\xea\x30\xd5\x30\xa1\x30\xec\x30\xf3\x30\xb9\x30\xe2\x30\xfc\x30\xc9\x30\x4c\x30\x7f\x4f\x28\x75\x55\x30\x8c\x30\x66\x30\x44\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Fbx Load : ��Ή��̂t�u���t�@�����X���[�h���g�p����Ă��܂���\n" @*/ )) ;
							return -1 ;
						}

						// �ʂ̊e���_�ɑΉ�����t�u�̏����擾
						switch( FbxMappingMode )
						{
						case FbxGeometryElement::eByControlPoint :
							// �t�u�C���f�b�N�X�͒��_�C���f�b�N�X�Ɠ������Ȃ�
							MeshFace = Mesh->Faces ;
							for( j = 0 ; ( DWORD )j < Mesh->FaceNum ; j ++, MeshFace ++ )
							{
								MeshFace->UVIndex[ i ][ 0 ] = MeshFace->VertexIndex[ 0 ] ;
								MeshFace->UVIndex[ i ][ 1 ] = MeshFace->VertexIndex[ 1 ] ;
								MeshFace->UVIndex[ i ][ 2 ] = MeshFace->VertexIndex[ 2 ] ;
								MeshFace->UVIndex[ i ][ 3 ] = MeshFace->VertexIndex[ 3 ] ;
							}
							break ;

						case FbxGeometryElement::eByPolygonVertex :
							// �t�u�C���f�b�N�X�͕ʌő��݂���
							MeshFace = Mesh->Faces ;
							k = 0 ;
							for( j = 0 ; ( DWORD )j < Mesh->FaceNum ; j ++, MeshFace ++ )
							{
								for( l = 0 ; ( DWORD )l < MeshFace->IndexNum ; l ++, k ++ )
									MeshFace->UVIndex[ i ][ l ] = k ;
							}
							break ;

						default :
							DXST_ERRORLOGFMT_ADDUTF16LE(( "\x46\x00\x62\x00\x78\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x3a\x00\x20\x00\x5e\x97\xfe\x5b\xdc\x5f\x6e\x30\x35\xff\x36\xff\xde\x30\xc3\x30\xd4\x30\xf3\x30\xb0\x30\xe2\x30\xfc\x30\xc9\x30\x4c\x30\x7f\x4f\x28\x75\x55\x30\x8c\x30\x66\x30\x44\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Fbx Load : ��Ή��̂t�u�}�b�s���O���[�h���g�p����Ă��܂���\n" @*/ )) ;
							return -1 ;
						}
					}
				}

				// �}�e���A���̏����擾
				{
					// �}�e���A���̐����擾
					Mesh->MaterialNum = pFbxNode->GetMaterialCount() ;

					// �}�e���A��������ꍇ�̂ݏ���
					if( Mesh->MaterialNum != 0 )
					{
						// �}�e���A���̐������J��Ԃ�
						for( i = 0 ; ( DWORD )i < Mesh->MaterialNum ; i ++ )
						{
							// �}�e���A���̃A�h���X���擾
							FbxMaterial = pFbxNode->GetMaterial( i ) ;

							// ���f����Ɋ��ɂ��̃}�e���A���̏�񂪂���ꍇ�̓A�h���X�����ۑ�����
							Material = RModel->MaterialFirst ;
							for( j = 0 ; ( DWORD )j < RModel->MaterialNum && Material->UserData != FbxMaterial ; j ++, Material = Material->DataNext ){}
							if( j != RModel->MaterialNum )
							{
								Mesh->Materials[ i ] = Material ;
							}
							else
							{
								// �}�e���A���̒ǉ�
//								FbxUTF8ToAnsi( FbxMaterial->GetName(), ANSIBuffer, &ANSISize ) ;
								Material = MV1RAddMaterial( RModel, FbxMaterial->GetName() ) ;
								if( Material == NULL )
								{
									DXST_ERRORLOGFMT_ADDUTF16LE(( "\x46\x00\x62\x00\x78\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x3a\x00\x20\x00\xde\x30\xc6\x30\xea\x30\xa2\x30\xeb\x30\xaa\x30\xd6\x30\xb8\x30\xa7\x30\xaf\x30\xc8\x30\x6e\x30\xfd\x8f\xa0\x52\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Fbx Load : �}�e���A���I�u�W�F�N�g�̒ǉ��Ɏ��s���܂���\n" @*/ )) ;
									return -1 ;
								}
								Material->SubName = FbxMaterial->GetName() ;

								Mesh->Materials[ i ] = Material ;

								// �e�a�w�̃A�h���X��ۑ�
								Material->UserData = FbxMaterial ;

								// �}�e���A���̃^�C�v�������o�[�g�ł��t�H���ł������ꍇ�͕���
								if( FbxMaterial->GetClassId().Is( FbxSurfaceLambert::ClassId ) ||
									FbxMaterial->GetClassId().Is( FbxSurfacePhong::ClassId ) )
								{
									// �����o�[�g�̏����擾����
									FbxLambert = ( FbxSurfaceLambert * )FbxMaterial ;

									// ��{�I�ȏ����擾

									// �A���r�G���g�J���[
									Material->Ambient.r = ( float )FbxLambert->Ambient.Get()[ 0 ] ;
									Material->Ambient.g = ( float )FbxLambert->Ambient.Get()[ 1 ] ;
									Material->Ambient.b = ( float )FbxLambert->Ambient.Get()[ 2 ] ;
									Material->Ambient.a = 0.0f ;

									// �f�B�t���[�Y�J���[
									Material->Diffuse.r = ( float )FbxLambert->Diffuse.Get()[ 0 ] ;
									Material->Diffuse.g = ( float )FbxLambert->Diffuse.Get()[ 1 ] ;
									Material->Diffuse.b = ( float )FbxLambert->Diffuse.Get()[ 2 ] ;
									Material->Diffuse.a = 1.0f ;

									// �G�~�b�V�u�J���[
									Material->Emissive.r = ( float )FbxLambert->Emissive.Get()[ 0 ] ;
									Material->Emissive.g = ( float )FbxLambert->Emissive.Get()[ 1 ] ;
									Material->Emissive.b = ( float )FbxLambert->Emissive.Get()[ 2 ] ;
									Material->Emissive.a = 0.0f ;

									// �A���t�@�l��ۑ�
	//								Material->Diffuse.a = 1.0f - FbxLambert->TransparencyFactor.Get() ;

									// �t�H���}�e���A���̏ꍇ�̓t�H���}�e���A���̏����擾����
									if( FbxMaterial->GetClassId().Is( FbxSurfacePhong::ClassId ) )
									{
										FbxPhong = ( FbxSurfacePhong * )FbxMaterial ;

										// �X�y�L�����J���[
										Material->Specular.r = ( float )FbxPhong->Specular.Get()[ 0 ] ;
										Material->Specular.g = ( float )FbxPhong->Specular.Get()[ 1 ] ;
										Material->Specular.b = ( float )FbxPhong->Specular.Get()[ 2 ] ;
										Material->Specular.a = 0.0f ;

										// ����
	//									Material->Shininess = FbxPhong->Shininess.Get() ;

										// ����
	//									Material->Reflection = FbxPhong->ReflectionFactor.Get() ;
										//Material->Power = ( float )FbxPhong->ReflectionFactor.Get() ;
										Material->Power = ( float )FbxPhong->Shininess.Get() ;
									}
								}
								else
								{
									DXST_ERRORLOGFMT_ADDUTF16LE(( "\x46\x00\x62\x00\x78\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x3a\x00\x20\x00\x5e\x97\xfe\x5b\xdc\x5f\x6e\x30\xde\x30\xc6\x30\xea\x30\xa2\x30\xeb\x30\xe2\x30\xfc\x30\xc9\x30\x4c\x30\x7f\x4f\x28\x75\x55\x30\x8c\x30\x66\x30\x44\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Fbx Load : ��Ή��̃}�e���A�����[�h���g�p����Ă��܂���\n" @*/ )) ;
									return -1 ;
								}

								// �g�p���Ă���e�N�X�`���̏����擾����
								{
									FbxProperty _FbxProperty ;
									FbxLayeredTexture::EBlendMode BlendMode ;
									int LayeredTexNum, NormalTexNum ;

									// �f�B�t���[�Y�}�e���A���v���p�e�B�̎擾
									{
										_FbxProperty = FbxMaterial->FindProperty( FbxSurfaceMaterial::sDiffuse ) ;

										// ���C���[�h�e�N�X�`���̏ꍇ�Ƃ���ȊO�ŏ����𕪊�
										if( _FbxProperty.GetSrcObject< FbxLayeredTexture >( 0 ) != NULL )
										{
											// �Q�ȏ�̃��C���[�ɂ͑Ή����Ă��Ȃ�
											if( _FbxProperty.GetSrcObject< FbxLayeredTexture >( 1 ) != NULL )
											{
												DXST_ERRORLOGFMT_ADDUTF16LE(( "\x46\x00\x62\x00\x78\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x3a\x00\x20\x00\x44\x00\x69\x00\x66\x00\x66\x00\x75\x00\x73\x00\x65\x00\x20\x00\xde\x30\xc6\x30\xea\x30\xa2\x30\xeb\x30\x6f\x30\x11\xff\xec\x30\xa4\x30\xe4\x30\xfc\x30\xe5\x4e\x0a\x4e\x6b\x30\x6f\x30\xfe\x5b\xdc\x5f\x57\x30\x66\x30\x44\x30\x7e\x30\x5b\x30\x93\x30\x0a\x00\x00"/*@ L"Fbx Load : Diffuse �}�e���A���͂P���C���[�ȏ�ɂ͑Ή����Ă��܂���\n" @*/ )) ;
												return -1 ;
											}

											// �e�N�X�`���̐������J��Ԃ�
											LayeredTexNum = 1 ;
											for( j = 0 ; j < LayeredTexNum ; j ++ )
											{
												// �e�N�X�`���̃A�h���X���擾
												_FbxLayeredTexture = _FbxProperty.GetSrcObject< FbxLayeredTexture >( j ) ;

												// ���C���[�̒��Ɋ܂܂�Ă���e�N�X�`���̐����擾����
												NormalTexNum = _FbxLayeredTexture->GetSrcObjectCount< FbxTexture >() ;

												// �e�N�X�`���̐������J��Ԃ�
												for( k = 0 ; k < NormalTexNum ; k ++ )
												{
													_FbxTexture = _FbxLayeredTexture->GetSrcObject< FbxTexture >( k ) ;
													if( _FbxTexture == NULL ) continue ;

													// ���f���ɒǉ�
													Material->DiffuseTexs[ Material->DiffuseTexNum ] = FbxAddTexture( RModel, _FbxTexture ) ;
													if( Material->DiffuseTexs[ Material->DiffuseTexNum ] == NULL ) 
													{
														DXST_ERRORLOGFMT_ADDUTF16LE(( "\x46\x00\x62\x00\x78\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x3a\x00\x20\x00\x44\x00\x69\x00\x66\x00\x66\x00\x75\x00\x73\x00\x65\x00\x20\x00\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\xaa\x30\xd6\x30\xb8\x30\xa7\x30\xaf\x30\xc8\x30\x6e\x30\xfd\x8f\xa0\x52\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Fbx Load : Diffuse �e�N�X�`���I�u�W�F�N�g�̒ǉ��Ɏ��s���܂���\n" @*/ )) ;
														return -1 ;
													}

													// �������@���擾����
													_FbxLayeredTexture->GetTextureBlendMode( k, BlendMode ) ;
													switch( BlendMode )
													{
													case FbxLayeredTexture::eTranslucent : Material->DiffuseTexs[ Material->DiffuseTexNum ]->BlendType = MV1_LAYERBLEND_TYPE_TRANSLUCENT ; break ;
													case FbxLayeredTexture::eAdditive :    Material->DiffuseTexs[ Material->DiffuseTexNum ]->BlendType = MV1_LAYERBLEND_TYPE_ADDITIVE ;    break ;
													case FbxLayeredTexture::eModulate :    Material->DiffuseTexs[ Material->DiffuseTexNum ]->BlendType = MV1_LAYERBLEND_TYPE_MODULATE ;    break ;
													case FbxLayeredTexture::eModulate2 :   Material->DiffuseTexs[ Material->DiffuseTexNum ]->BlendType = MV1_LAYERBLEND_TYPE_MODULATE2 ;   break ;
													}

													// �e�N�X�`���̐����C���N�������g
													Material->DiffuseTexNum ++ ;
												}
											}
										}
										else
										{
											// �ʏ�̃e�N�X�`���̐����擾
											Material->DiffuseTexNum = _FbxProperty.GetSrcObjectCount< FbxTexture >() ;

											// �g�p���Ă���ꍇ�͏���
											if( Material->DiffuseTexNum != 0 )
											{
												// �e�N�X�`���̐������J��Ԃ�
												for( j = 0 ; j < Material->DiffuseTexNum ; j ++ )
												{
													// �e�N�X�`���̃A�h���X���擾
													_FbxTexture = _FbxProperty.GetSrcObject< FbxTexture >( j ) ;

													// ���f���ɒǉ�
													Material->DiffuseTexs[ j ] = FbxAddTexture( RModel, _FbxTexture ) ;
													if( Material->DiffuseTexs[ j ] == NULL ) 
													{
														DXST_ERRORLOGFMT_ADDUTF16LE(( "\x46\x00\x62\x00\x78\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x3a\x00\x20\x00\x44\x00\x69\x00\x66\x00\x66\x00\x75\x00\x73\x00\x65\x00\x20\x00\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\xaa\x30\xd6\x30\xb8\x30\xa7\x30\xaf\x30\xc8\x30\x6e\x30\xfd\x8f\xa0\x52\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Fbx Load : Diffuse �e�N�X�`���I�u�W�F�N�g�̒ǉ��Ɏ��s���܂���\n" @*/ )) ;
														return -1 ;
													}
												}
											}
										}
									}

									// �X�y�L�����}�e���A���v���p�e�B�̎擾
									{
										_FbxProperty = FbxMaterial->FindProperty( FbxSurfaceMaterial::sSpecular ) ;

										// ���C���[�h�e�N�X�`���̏ꍇ�Ƃ���ȊO�ŏ����𕪊�
										LayeredTexNum = _FbxProperty.GetSrcObjectCount< FbxLayeredTexture >() ;
										if( LayeredTexNum )
										{
											// �Q�ȏ�̃��C���[�ɂ͑Ή����Ă��Ȃ�
											if( LayeredTexNum > 1 )
											{
												DXST_ERRORLOGFMT_ADDUTF16LE(( "\x46\x00\x62\x00\x78\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x3a\x00\x20\x00\x53\x00\x70\x00\x65\x00\x63\x00\x75\x00\x6c\x00\x61\x00\x72\x00\x20\x00\xde\x30\xc6\x30\xea\x30\xa2\x30\xeb\x30\x6f\x30\x11\xff\xec\x30\xa4\x30\xe4\x30\xfc\x30\xe5\x4e\x0a\x4e\x6b\x30\x6f\x30\xfe\x5b\xdc\x5f\x57\x30\x66\x30\x44\x30\x7e\x30\x5b\x30\x93\x30\x0a\x00\x00"/*@ L"Fbx Load : Specular �}�e���A���͂P���C���[�ȏ�ɂ͑Ή����Ă��܂���\n" @*/ )) ;
												return -1 ;
											}

											// �e�N�X�`���̐������J��Ԃ�
											for( j = 0 ; j < LayeredTexNum ; j ++ )
											{
												// �e�N�X�`���̃A�h���X���擾
												_FbxLayeredTexture = _FbxProperty.GetSrcObject< FbxLayeredTexture >( j ) ;

												// ���C���[�̒��Ɋ܂܂�Ă���e�N�X�`���̐����擾����
												NormalTexNum = _FbxLayeredTexture->GetSrcObjectCount< FbxTexture >() ;

												// �e�N�X�`���̐������J��Ԃ�
												for( k = 0 ; k < NormalTexNum ; k ++ )
												{
													_FbxTexture = _FbxLayeredTexture->GetSrcObject< FbxTexture >( k ) ;
													if( _FbxTexture == NULL ) continue ;

													// ���f���ɒǉ�
													Material->SpecularTexs[ Material->SpecularTexNum ] = FbxAddTexture( RModel, _FbxTexture ) ;
													if( Material->SpecularTexs[ Material->SpecularTexNum ] == NULL ) 
													{
														DXST_ERRORLOGFMT_ADDUTF16LE(( "\x46\x00\x62\x00\x78\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x3a\x00\x20\x00\x53\x00\x70\x00\x65\x00\x63\x00\x75\x00\x6c\x00\x61\x00\x72\x00\x20\x00\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\xaa\x30\xd6\x30\xb8\x30\xa7\x30\xaf\x30\xc8\x30\x6e\x30\xfd\x8f\xa0\x52\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Fbx Load : Specular �e�N�X�`���I�u�W�F�N�g�̒ǉ��Ɏ��s���܂���\n" @*/ )) ;
														return -1 ;
													}

													// �������@���擾����
													_FbxLayeredTexture->GetTextureBlendMode( k, BlendMode ) ;
													switch( BlendMode )
													{
													case FbxLayeredTexture::eTranslucent : Material->SpecularTexs[ Material->SpecularTexNum ]->BlendType = MV1_LAYERBLEND_TYPE_TRANSLUCENT ; break ;
													case FbxLayeredTexture::eAdditive :    Material->SpecularTexs[ Material->SpecularTexNum ]->BlendType = MV1_LAYERBLEND_TYPE_ADDITIVE ;    break ;
													case FbxLayeredTexture::eModulate :    Material->SpecularTexs[ Material->SpecularTexNum ]->BlendType = MV1_LAYERBLEND_TYPE_MODULATE ;    break ;
													case FbxLayeredTexture::eModulate2 :   Material->SpecularTexs[ Material->SpecularTexNum ]->BlendType = MV1_LAYERBLEND_TYPE_MODULATE2 ;   break ;
													}

													// �e�N�X�`���̐����C���N�������g
													Material->SpecularTexNum ++ ;
												}
											}
										}
										else
										{
											// �ʏ�̃e�N�X�`���̐����擾
											Material->SpecularTexNum = _FbxProperty.GetSrcObjectCount< FbxTexture >() ;

											// �g�p���Ă���ꍇ�͏���
											if( Material->SpecularTexNum != 0 )
											{
												// �e�N�X�`���̐������J��Ԃ�
												for( j = 0 ; j < Material->SpecularTexNum ; j ++ )
												{
													// �e�N�X�`���̃A�h���X���擾
													_FbxTexture = _FbxProperty.GetSrcObject< FbxTexture >( j ) ;

													// ���f���ɒǉ�
													Material->SpecularTexs[ j ] = FbxAddTexture( RModel, _FbxTexture ) ;
													if( Material->SpecularTexs[ j ] == NULL ) 
													{
														DXST_ERRORLOGFMT_ADDUTF16LE(( "\x46\x00\x62\x00\x78\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x3a\x00\x20\x00\x53\x00\x70\x00\x65\x00\x63\x00\x75\x00\x6c\x00\x61\x00\x72\x00\x20\x00\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\xaa\x30\xd6\x30\xb8\x30\xa7\x30\xaf\x30\xc8\x30\x6e\x30\xfd\x8f\xa0\x52\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Fbx Load : Specular �e�N�X�`���I�u�W�F�N�g�̒ǉ��Ɏ��s���܂���\n" @*/ )) ;
														return -1 ;
													}
												}
											}
										}
									}

									// �@���}�b�v�}�e���A���v���p�e�B�̎擾
									{
										_FbxProperty = FbxMaterial->FindProperty( FbxSurfaceMaterial::sNormalMap ) ;

										// ���C���[�h�e�N�X�`���̏ꍇ�Ƃ���ȊO�ŏ����𕪊�
										LayeredTexNum = _FbxProperty.GetSrcObjectCount< FbxLayeredTexture >() ;
										if( LayeredTexNum )
										{
											// �Q�ȏ�̃��C���[�ɂ͑Ή����Ă��Ȃ�
											if( LayeredTexNum > 1 )
											{
												DXST_ERRORLOGFMT_ADDUTF16LE(( "\x46\x00\x62\x00\x78\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x3a\x00\x20\x00\x42\x00\x75\x00\x6d\x00\x70\x00\x20\x00\xde\x30\xc6\x30\xea\x30\xa2\x30\xeb\x30\x6f\x30\x11\xff\xec\x30\xa4\x30\xe4\x30\xfc\x30\xe5\x4e\x0a\x4e\x6b\x30\x6f\x30\xfe\x5b\xdc\x5f\x57\x30\x66\x30\x44\x30\x7e\x30\x5b\x30\x93\x30\x0a\x00\x00"/*@ L"Fbx Load : Bump �}�e���A���͂P���C���[�ȏ�ɂ͑Ή����Ă��܂���\n" @*/ )) ;
												return -1 ;
											}

											// �e�N�X�`���̐������J��Ԃ�
											for( j = 0 ; j < LayeredTexNum ; j ++ )
											{
												// �e�N�X�`���̃A�h���X���擾
												_FbxLayeredTexture = _FbxProperty.GetSrcObject< FbxLayeredTexture >( j ) ;

												// ���C���[�̒��Ɋ܂܂�Ă���e�N�X�`���̐����擾����
												NormalTexNum = _FbxLayeredTexture->GetSrcObjectCount< FbxTexture >() ;

												// �e�N�X�`���̐������J��Ԃ�
												for( k = 0 ; k < NormalTexNum ; k ++ )
												{
													_FbxTexture = _FbxLayeredTexture->GetSrcObject< FbxTexture >( k ) ;
													if( _FbxTexture == NULL ) continue ;

													// ���f���ɒǉ�
													Material->NormalTexs[ Material->NormalTexNum ] = FbxAddTexture( RModel, _FbxTexture ) ;
													if( Material->NormalTexs[ Material->NormalTexNum ] == NULL ) 
													{
														DXST_ERRORLOGFMT_ADDUTF16LE(( "\x46\x00\x62\x00\x78\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x3a\x00\x20\x00\x42\x00\x75\x00\x6d\x00\x70\x00\x20\x00\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\xaa\x30\xd6\x30\xb8\x30\xa7\x30\xaf\x30\xc8\x30\x6e\x30\xfd\x8f\xa0\x52\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Fbx Load : Bump �e�N�X�`���I�u�W�F�N�g�̒ǉ��Ɏ��s���܂���\n" @*/ )) ;
														return -1 ;
													}

													// �������@���擾����
													_FbxLayeredTexture->GetTextureBlendMode( k, BlendMode ) ;
													switch( BlendMode )
													{
													case FbxLayeredTexture::eTranslucent : Material->NormalTexs[ Material->NormalTexNum ]->BlendType = MV1_LAYERBLEND_TYPE_TRANSLUCENT ; break ;
													case FbxLayeredTexture::eAdditive :    Material->NormalTexs[ Material->NormalTexNum ]->BlendType = MV1_LAYERBLEND_TYPE_ADDITIVE ;    break ;
													case FbxLayeredTexture::eModulate :    Material->NormalTexs[ Material->NormalTexNum ]->BlendType = MV1_LAYERBLEND_TYPE_MODULATE ;    break ;
													case FbxLayeredTexture::eModulate2 :   Material->NormalTexs[ Material->NormalTexNum ]->BlendType = MV1_LAYERBLEND_TYPE_MODULATE2 ;   break ;
													}

													// �e�N�X�`���̐����C���N�������g
													Material->NormalTexNum ++ ;
												}
											}
										}
										else
										{
											// �ʏ�̃e�N�X�`���̐����擾
											Material->NormalTexNum = _FbxProperty.GetSrcObjectCount< FbxTexture >() ;

											// �g�p���Ă���ꍇ�͏���
											if( Material->NormalTexNum != 0 )
											{
												// �e�N�X�`���̐������J��Ԃ�
												for( j = 0 ; j < Material->NormalTexNum ; j ++ )
												{
													// �e�N�X�`���̃A�h���X���擾
													_FbxTexture = _FbxProperty.GetSrcObject< FbxTexture >( j ) ;

													// ���f���ɒǉ�
													Material->NormalTexs[ j ] = FbxAddTexture( RModel, _FbxTexture ) ;
													if( Material->NormalTexs[ j ] == NULL ) 
													{
														DXST_ERRORLOGFMT_ADDUTF16LE(( "\x46\x00\x62\x00\x78\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x3a\x00\x20\x00\x42\x00\x75\x00\x6d\x00\x70\x00\x20\x00\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\xaa\x30\xd6\x30\xb8\x30\xa7\x30\xaf\x30\xc8\x30\x6e\x30\xfd\x8f\xa0\x52\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Fbx Load : Bump �e�N�X�`���I�u�W�F�N�g�̒ǉ��Ɏ��s���܂���\n" @*/ )) ;
														return -1 ;
													}
												}
											}
										}
									}
								}
							}
						}

						// �e�ʂɑΉ�����}�e���A���̏����擾����
						{
							// �}�e���A���G�������g�̐������J��Ԃ�
						    for( i = 0 ; i < _FbxMesh->GetElementMaterialCount() ; i ++ )
							{
								FbxMaterialElem = _FbxMesh->GetElementMaterial( i ) ;

								// ���[�h�̎擾
								FbxMappingMode = FbxMaterialElem->GetMappingMode() ;
								FbxRefMode = FbxMaterialElem->GetReferenceMode() ;

								// �}�e���A�����̎擾
								switch( FbxRefMode )
								{
								case FbxGeometryElement::eDirect :
									// �}�e���A�����̐����擾
									MaterialNum = pFbxNode->GetMaterialCount() ;

									// �}�e���A�������i�[���郁�����̈�̊m��
									FbxMaterialDim = ( FbxSurfaceMaterial ** )DXCALLOC( sizeof( FbxSurfaceMaterial * ) * MaterialNum ) ;
									if( FbxMaterialDim == NULL ) 
									{
										DXST_ERRORLOGFMT_ADDUTF16LE(( "\x46\x00\x62\x00\x78\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x3a\x00\x20\x00\xde\x30\xc6\x30\xea\x30\xa2\x30\xeb\x30\xea\x30\xb9\x30\xc8\x30\x92\x30\x00\x4e\x42\x66\xdd\x4f\x58\x5b\x59\x30\x8b\x30\xd0\x30\xc3\x30\xd5\x30\xa1\x30\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Fbx Load : �}�e���A�����X�g���ꎞ�ۑ�����o�b�t�@�̊m�ۂɎ��s���܂���\n" @*/ )) ;
										return -1 ;
									}

									// ���ڃ��[�h�̏ꍇ�͂��̂܂ܑ��
									for( j = 0 ; j < MaterialNum ; j ++ )
									{
										FbxMaterialDim[ j ] = pFbxNode->GetMaterial( j ) ;
									}
									break ;

								case FbxGeometryElement::eIndexToDirect :
									// �C���f�b�N�X�̐����擾
									MaterialNum = FbxMaterialElem->GetIndexArray().GetCount() ;

									// �}�e���A�������i�[���郁�����̈�̊m��
									FbxMaterialDim = ( FbxSurfaceMaterial ** )DXCALLOC( sizeof( FbxSurfaceMaterial * ) * MaterialNum ) ;
									if( FbxMaterialDim == NULL ) 
									{
										DXST_ERRORLOGFMT_ADDUTF16LE(( "\x46\x00\x62\x00\x78\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x3a\x00\x20\x00\xde\x30\xc6\x30\xea\x30\xa2\x30\xeb\x30\xea\x30\xb9\x30\xc8\x30\x92\x30\x00\x4e\x42\x66\xdd\x4f\x58\x5b\x59\x30\x8b\x30\xd0\x30\xc3\x30\xd5\x30\xa1\x30\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Fbx Load : �}�e���A�����X�g���ꎞ�ۑ�����o�b�t�@�̊m�ۂɎ��s���܂���\n" @*/ )) ;
										return -1 ;
									}

									// �C���f�b�N�X���[�h�̏ꍇ�͊ԐڎQ�Ƒ��
									for( j = 0 ; j < MaterialNum ; j ++ )
									{
										Index = FbxMaterialElem->GetIndexArray().GetAt( j ) ;
										FbxMaterialDim[ j ] = pFbxNode->GetMaterial( Index ) ;
									}
									break ;

								default :
									DXST_ERRORLOGFMT_ADDUTF16LE(( "\x46\x00\x62\x00\x78\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x3a\x00\x20\x00\x5e\x97\xfe\x5b\xdc\x5f\x6e\x30\xde\x30\xc6\x30\xea\x30\xa2\x30\xeb\x30\xea\x30\xd5\x30\xa1\x30\xec\x30\xf3\x30\xb9\x30\xe2\x30\xfc\x30\xc9\x30\x4c\x30\x7f\x4f\x28\x75\x55\x30\x8c\x30\x66\x30\x44\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Fbx Load : ��Ή��̃}�e���A�����t�@�����X���[�h���g�p����Ă��܂���\n" @*/ )) ;
									return -1 ;
								}

								// �ʂ̊e���_�ɑΉ�����}�e���A���̏����擾
								switch( FbxMappingMode )
								{
								case FbxGeometryElement::eByPolygon :
									// �P�|���S���ɂP�}�e���A��
									MeshFace = Mesh->Faces ;
									for( j = 0 ; ( DWORD )j < Mesh->FaceNum ; j ++, MeshFace ++ )
									{
										// ���蓖�Ă��Ă���}�e���A�������f�����̂ǂ̃}�e���A���ɓ�����̂��𒲂ׂ�
										for( k = 0 ; ( DWORD )k < Mesh->MaterialNum && _STRCMP( Mesh->Materials[ k ]->SubName, FbxMaterialDim[ j ]->GetName() ) != 0 ; k ++ ){}
										if( k == Mesh->MaterialNum )
										{
//											FbxUTF8ToAnsi( FbxMaterialDim[ j ]->GetName(), ANSIBuffer, &ANSISize ) ;
											ConvString( FbxMaterialDim[ j ]->GetName(), DX_CHARCODEFORMAT_UTF8, ( char * )UTF16LE_Buffer, sizeof( UTF16LE_Buffer ), DX_CHARCODEFORMAT_UTF16LE ) ; 
											DXST_ERRORLOGFMT_ADDUTF16LE(( "\x46\x00\x62\x00\x78\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x3a\x00\x20\x00\xce\x30\xfc\x30\xc9\x30\x85\x51\x6b\x30\x00\x4e\xf4\x81\x59\x30\x8b\x30\xde\x30\xc6\x30\xea\x30\xa2\x30\xeb\x30\x4c\x30\x8b\x89\x64\x30\x4b\x30\x8a\x30\x7e\x30\x5b\x30\x93\x30\x67\x30\x57\x30\x5f\x30\x20\x00\x65\x00\x42\x00\x59\x00\x5f\x00\x43\x00\x4f\x00\x4e\x00\x54\x00\x52\x00\x4f\x00\x4c\x00\x5f\x00\x50\x00\x4f\x00\x49\x00\x4e\x00\x54\x00\x20\x00\x2c\x00\x20\x00\x25\x00\x73\x00\x20\x00\x0a\x00\x00"/*@ L"Fbx Load : �m�[�h���Ɉ�v����}�e���A����������܂���ł��� eBY_CONTROL_POINT , %s \n" @*/, UTF16LE_Buffer )) ;
											DXFREE( FbxMaterialDim ) ;
											return -1 ;
										}

										// �C���f�b�N�X��ۑ�
										MeshFace->MaterialIndex = k ;
									}
									break ;

								case FbxGeometryElement::eAllSame :
									// ���b�V���S�̂łP�}�e���A��

									// ���蓖�Ă��Ă���}�e���A�������f�����̂ǂ̃}�e���A���ɓ�����̂��𒲂ׂ�
									for( k = 0 ; ( DWORD )k < Mesh->MaterialNum && _STRCMP( Mesh->Materials[ k ]->SubName, FbxMaterialDim[ 0 ]->GetName() ) != 0 ; k ++ ){}
									if( k == Mesh->MaterialNum )
									{
										DXFREE( FbxMaterialDim ) ;
										DXST_ERRORLOGFMT_ADDUTF16LE(( "\x46\x00\x62\x00\x78\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x3a\x00\x20\x00\xce\x30\xfc\x30\xc9\x30\x85\x51\x6b\x30\x00\x4e\xf4\x81\x59\x30\x8b\x30\xde\x30\xc6\x30\xea\x30\xa2\x30\xeb\x30\x4c\x30\x8b\x89\x64\x30\x4b\x30\x8a\x30\x7e\x30\x5b\x30\x93\x30\x67\x30\x57\x30\x5f\x30\x20\x00\x65\x00\x41\x00\x4c\x00\x4c\x00\x5f\x00\x53\x00\x41\x00\x4d\x00\x45\x00\x0a\x00\x00"/*@ L"Fbx Load : �m�[�h���Ɉ�v����}�e���A����������܂���ł��� eALL_SAME\n" @*/ )) ;
										return -1 ;
									}

									// ���ׂẴ��b�V���Ɍ������C���f�b�N�X���Z�b�g
									MeshFace = Mesh->Faces ;
									for( j = 0 ; ( DWORD )j < Mesh->FaceNum ; j ++, MeshFace ++ )
									{
										// �C���f�b�N�X��ۑ�
										MeshFace->MaterialIndex = k ;
									}
									break ;

								default :
									DXFREE( FbxMaterialDim ) ;
									DXST_ERRORLOGFMT_ADDUTF16LE(( "\x46\x00\x62\x00\x78\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x3a\x00\x20\x00\x5e\x97\xfe\x5b\xdc\x5f\x6e\x30\xde\x30\xc6\x30\xea\x30\xa2\x30\xeb\x30\xde\x30\xc3\x30\xd4\x30\xf3\x30\xb0\x30\xe2\x30\xfc\x30\xc9\x30\x4c\x30\x7f\x4f\x28\x75\x55\x30\x8c\x30\x66\x30\x44\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Fbx Load : ��Ή��̃}�e���A���}�b�s���O���[�h���g�p����Ă��܂���\n" @*/ )) ;
									return -1 ;
								}

								DXFREE( FbxMaterialDim ) ;
							}
						}
					}
				}

				// �X�L�����b�V���̏����擾
				if( _FbxMesh->GetDeformerCount( FbxDeformer::eSkin ) )
				{
					int PointNum, ClusterCount ;
					int *Point ;
					double *Weight ;
					FbxAMatrix _FbxMatrix ;
					MATRIX InvMatrix ;
					BYTE *PositionFillFlag ;

					// �Q�ȏ�̃X�L���ɂ͖��Ή�
					if( _FbxMesh->GetDeformerCount( FbxDeformer::eSkin ) > 1 )
					{
						DXST_ERRORLOGFMT_ADDUTF16LE(( "\x46\x00\x62\x00\x78\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x3a\x00\x20\x00\x11\xff\xe1\x30\xc3\x30\xb7\x30\xe5\x30\x6b\x30\x07\x89\x70\x65\x6e\x30\xb9\x30\xad\x30\xf3\x30\xe1\x30\xc3\x30\xb7\x30\xe5\x30\x6b\x30\x6f\x30\xfe\x5b\xdc\x5f\x57\x30\x66\x30\x44\x30\x7e\x30\x5b\x30\x93\x30\x0a\x00\x00"/*@ L"Fbx Load : �P���b�V���ɕ����̃X�L�����b�V���ɂ͑Ή����Ă��܂���\n" @*/ )) ;
						return -1 ;
					}

					// �X�L�����b�V�����̎擾
					_FbxSkin = ( FbxSkin * )_FbxMesh->GetDeformer( 0, FbxDeformer::eSkin ) ;

					// �N���X�^�̐����擾
					ClusterCount = _FbxSkin->GetClusterCount() ;

					// �e���_�ɃE�G�C�g�l���ݒ肳�ꂽ���ǂ������m�F���邽�߂̃t���O���i�[���邽�߂̃������̈���m�ۂ���
					PositionFillFlag = ( BYTE * )ADDMEMAREA( ( Mesh->PositionNum + 31 ) / 32 * 4, &RModel->Mem ) ;
					if( PositionFillFlag == NULL )
					{
						DXST_ERRORLOGFMT_ADDUTF16LE(( "\x46\x00\x62\x00\x78\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x3a\x00\x20\x00\xa7\x5e\x19\x6a\x6b\x30\xfe\x5b\x59\x30\x8b\x30\xb9\x30\xad\x30\xf3\x30\xa6\x30\xa8\x30\xa4\x30\xc8\x30\x4c\x30\x58\x5b\x28\x57\x59\x30\x8b\x30\x4b\x30\x69\x30\x46\x30\x4b\x30\x92\x30\xba\x78\x8d\x8a\x59\x30\x8b\x30\x5f\x30\x81\x30\x6e\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Fbx Load : ���W�ɑ΂���X�L���E�G�C�g�����݂��邩�ǂ������m�F���邽�߂̃������̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
						return -1 ;
					}
					_MEMSET( PositionFillFlag, 0, ( Mesh->PositionNum + 31 ) / 32 * 4 ) ;

					// �L���ȏ�񂪂���N���X�^���擾����
					for( i = 0 ; i < ClusterCount ; i ++ )
					{
						FbxAMatrix FbxTransMatrix ;
						MATRIX TransMatrix ;

						_FbxCluster = _FbxSkin->GetCluster( i ) ;
						if( _FbxCluster->GetControlPointIndicesCount() == 0 ) continue ;

						// �X�L���E�G�C�g���̒ǉ�
						SkinWeight = MV1RAddSkinWeight( RModel ) ;
						Mesh->SkinWeights[ Mesh->SkinWeightsNum ] = SkinWeight ;
						Mesh->SkinWeights[ Mesh->SkinWeightsNum ]->UserData = _FbxCluster ;
						Mesh->SkinWeightsNum ++ ;

						// �e���_�ւ̉e�������i�[���邽�߂̃������̊m��
						SkinWeight->DataNum = _FbxCluster->GetControlPointIndicesCount() ;
						SkinWeight->Data = ( MV1_SKIN_WEIGHT_ONE_R * )ADDMEMAREA( sizeof( MV1_SKIN_WEIGHT_ONE_R ) * SkinWeight->DataNum, &RModel->Mem ) ;
						if( SkinWeight->Data == NULL )
						{
							DXST_ERRORLOGFMT_ADDUTF16LE(( "\x46\x00\x62\x00\x78\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x3a\x00\x20\x00\xb9\x30\xad\x30\xf3\x30\xa6\x30\xa8\x30\xa4\x30\xc8\x30\xc5\x60\x31\x58\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Fbx Load : �X�L���E�G�C�g�����i�[���郁�����̊m�ۂɎ��s���܂���\n" @*/ )) ;
							return -1 ;
						}

						// �e�������擾����
						PointNum = _FbxCluster->GetControlPointIndicesCount() ;
						Point = _FbxCluster->GetControlPointIndices() ;
						Weight = _FbxCluster->GetControlPointWeights() ;
						for( j = 0 ; ( DWORD )j < SkinWeight->DataNum ; j ++ )
						{
							SkinWeight->Data[ j ].TargetVertex = Point[ j ] ;
							SkinWeight->Data[ j ].Weight = ( float )Weight[ j ] ;
							PositionFillFlag[ Point[ j ] / 8 ] |= 1 << ( Point[ j ] & 7 ) ;
						}

						// �{�[���̃��[�J�����W�ɗ��Ƃ����ނ��߂̍s��̎擾
						_FbxCluster->GetTransformLinkMatrix( _FbxMatrix ) ;
						_FbxCluster->GetTransformMatrix( FbxTransMatrix ) ;
						InvMatrix.m[ 0 ][ 0 ] = ( float )_FbxMatrix.Double44()[ 0 ][ 0 ] ;
						InvMatrix.m[ 0 ][ 1 ] = ( float )_FbxMatrix.Double44()[ 0 ][ 1 ] ;
						InvMatrix.m[ 0 ][ 2 ] = ( float )_FbxMatrix.Double44()[ 0 ][ 2 ] ;
						InvMatrix.m[ 0 ][ 3 ] = ( float )_FbxMatrix.Double44()[ 0 ][ 3 ] ;

						InvMatrix.m[ 1 ][ 0 ] = ( float )_FbxMatrix.Double44()[ 1 ][ 0 ] ;
						InvMatrix.m[ 1 ][ 1 ] = ( float )_FbxMatrix.Double44()[ 1 ][ 1 ] ;
						InvMatrix.m[ 1 ][ 2 ] = ( float )_FbxMatrix.Double44()[ 1 ][ 2 ] ;
						InvMatrix.m[ 1 ][ 3 ] = ( float )_FbxMatrix.Double44()[ 1 ][ 3 ] ;

						InvMatrix.m[ 2 ][ 0 ] = ( float )_FbxMatrix.Double44()[ 2 ][ 0 ] ;
						InvMatrix.m[ 2 ][ 1 ] = ( float )_FbxMatrix.Double44()[ 2 ][ 1 ] ;
						InvMatrix.m[ 2 ][ 2 ] = ( float )_FbxMatrix.Double44()[ 2 ][ 2 ] ;
						InvMatrix.m[ 2 ][ 3 ] = ( float )_FbxMatrix.Double44()[ 2 ][ 3 ] ;

						InvMatrix.m[ 3 ][ 0 ] = ( float )_FbxMatrix.Double44()[ 3 ][ 0 ] ;
						InvMatrix.m[ 3 ][ 1 ] = ( float )_FbxMatrix.Double44()[ 3 ][ 1 ] ;
						InvMatrix.m[ 3 ][ 2 ] = ( float )_FbxMatrix.Double44()[ 3 ][ 2 ] ;
						InvMatrix.m[ 3 ][ 3 ] = ( float )_FbxMatrix.Double44()[ 3 ][ 3 ] ;

						TransMatrix.m[ 0 ][ 0 ] = ( float )FbxTransMatrix.Double44()[ 0 ][ 0 ] ;
						TransMatrix.m[ 0 ][ 1 ] = ( float )FbxTransMatrix.Double44()[ 0 ][ 1 ] ;
						TransMatrix.m[ 0 ][ 2 ] = ( float )FbxTransMatrix.Double44()[ 0 ][ 2 ] ;
						TransMatrix.m[ 0 ][ 3 ] = ( float )FbxTransMatrix.Double44()[ 0 ][ 3 ] ;

						TransMatrix.m[ 1 ][ 0 ] = ( float )FbxTransMatrix.Double44()[ 1 ][ 0 ] ;
						TransMatrix.m[ 1 ][ 1 ] = ( float )FbxTransMatrix.Double44()[ 1 ][ 1 ] ;
						TransMatrix.m[ 1 ][ 2 ] = ( float )FbxTransMatrix.Double44()[ 1 ][ 2 ] ;
						TransMatrix.m[ 1 ][ 3 ] = ( float )FbxTransMatrix.Double44()[ 1 ][ 3 ] ;

						TransMatrix.m[ 2 ][ 0 ] = ( float )FbxTransMatrix.Double44()[ 2 ][ 0 ] ;
						TransMatrix.m[ 2 ][ 1 ] = ( float )FbxTransMatrix.Double44()[ 2 ][ 1 ] ;
						TransMatrix.m[ 2 ][ 2 ] = ( float )FbxTransMatrix.Double44()[ 2 ][ 2 ] ;
						TransMatrix.m[ 2 ][ 3 ] = ( float )FbxTransMatrix.Double44()[ 2 ][ 3 ] ;

						TransMatrix.m[ 3 ][ 0 ] = ( float )FbxTransMatrix.Double44()[ 3 ][ 0 ] ;
						TransMatrix.m[ 3 ][ 1 ] = ( float )FbxTransMatrix.Double44()[ 3 ][ 1 ] ;
						TransMatrix.m[ 3 ][ 2 ] = ( float )FbxTransMatrix.Double44()[ 3 ][ 2 ] ;
						TransMatrix.m[ 3 ][ 3 ] = ( float )FbxTransMatrix.Double44()[ 3 ][ 3 ] ;

						CreateInverseMatrix( &TransMatrix, &TransMatrix ) ;
						CreateMultiplyMatrix( &InvMatrix, &InvMatrix, &TransMatrix ) ;

						CreateMultiplyMatrix( &InvMatrix, &ReverseMat, &InvMatrix ) ;
						CreateMultiplyMatrix( &InvMatrix, &InvMatrix, &ReverseMat ) ;

						CreateInverseMatrix( &SkinWeight->ModelLocalMatrix, &InvMatrix ) ;
					}

					// �E�G�C�g�l�����蓖�Ă��Ă��Ȃ����_���������炱�̃{�[���̏����ɂ���
					{
						int i, j, k, CheckNum ;
						int Result ;

						// �E�G�C�g�l�̖������_�����邩�`�F�b�N
						CheckNum = Mesh->PositionNum / 32 ;
						for( i = 0 ; i < CheckNum && ( ( DWORD * )PositionFillFlag )[ i ] == 0xffffffff ; i ++ ){}
						Result  = i != CheckNum ? 1 : 0 ;

						CheckNum = Mesh->PositionNum % 32 ;
						for( j = 0 ; j < CheckNum && ( ( ( BYTE * )( &( ( DWORD * )PositionFillFlag )[ i ] ) )[ j / 8 ] & ( 1 << ( j % 8 ) ) ) != 0 ; j ++ ){}
						Result |= j != CheckNum ? 1 : 0 ;

						// ���������炱�̃t���[���̏����Ƃ��ẴE�G�C�g����ǉ�
						if( Result == 1 )
						{
							int WeightNoneNum ;
							DWORD CheckData ;
							BYTE CheckDataByte ;

							// ���߂ăE�G�C�g�l�̖������_�̐��𐔂���
							WeightNoneNum = 0 ;
							CheckNum = Mesh->PositionNum / 32 ;
							for( i = 0 ; i < CheckNum ; i ++ )
							{
								CheckData = ( ( DWORD * )PositionFillFlag )[ i ] ;
								for( j = 0 ; j < 32 ; j ++ )
								{
									if( ( CheckData & ( 1 << j ) ) == 0 )
									{
										WeightNoneNum ++ ;
									}
								}
							}
							CheckData = ( ( DWORD * )PositionFillFlag )[ i ] ;
							CheckNum = Mesh->PositionNum % 32 ;
							for( j = 0 ; j < CheckNum ; j ++ )
							{
								if( ( ( ( BYTE * )&CheckData )[ j / 8 ] & ( 1 << ( j % 8 ) ) ) == 0 )
								{
									WeightNoneNum ++ ;
								}
							}

							// �E�G�C�g���̒ǉ�
							SkinWeight = MV1RAddSkinWeight( RModel ) ;
							Mesh->SkinWeights[ Mesh->SkinWeightsNum ] = SkinWeight ;
							Mesh->SkinWeights[ Mesh->SkinWeightsNum ]->UserData = NULL ;
							Mesh->SkinWeightsNum ++ ;
							CreateIdentityMatrix( &SkinWeight->ModelLocalMatrix ) ;

							// �e���_�ւ̉e�������i�[���邽�߂̃������̊m��
							SkinWeight->DataNum = WeightNoneNum ;
							SkinWeight->Data = ( MV1_SKIN_WEIGHT_ONE_R * )ADDMEMAREA( sizeof( MV1_SKIN_WEIGHT_ONE_R ) * SkinWeight->DataNum, &RModel->Mem ) ;
							if( SkinWeight->Data == NULL )
							{
								DXST_ERRORLOGFMT_ADDUTF16LE(( "\x46\x00\x62\x00\x78\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x3a\x00\x20\x00\xb9\x30\xad\x30\xf3\x30\xa6\x30\xa8\x30\xa4\x30\xc8\x30\xc5\x60\x31\x58\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x20\x00\x32\x00\x0a\x00\x00"/*@ L"Fbx Load : �X�L���E�G�C�g�����i�[���郁�����̊m�ۂɎ��s���܂��� 2\n" @*/ )) ;
								return -1 ;
							}

							// �e�������Z�b�g����
							WeightNoneNum = 0 ;
							CheckNum = Mesh->PositionNum / 8 ;
							k = 0 ;
							for( i = 0 ; i < CheckNum ; i ++ )
							{
								CheckDataByte = PositionFillFlag[ i ] ;
								for( j = 0 ; j < 8 ; j ++, k ++ )
								{
									if( ( CheckDataByte & ( 1 << j ) ) == 0 )
									{
										SkinWeight->Data[ WeightNoneNum ].TargetVertex = k ;
										SkinWeight->Data[ WeightNoneNum ].Weight = 1.0f ;
										WeightNoneNum ++ ;
									}
								}
							}
							CheckDataByte = PositionFillFlag[ i ] ;
							CheckNum = Mesh->PositionNum % 8 ;
							for( j = 0 ; j < CheckNum ; j ++, k ++ )
							{
								if( ( CheckDataByte & ( 1 << j ) ) == 0 )
								{
									SkinWeight->Data[ WeightNoneNum ].TargetVertex = k ;
									SkinWeight->Data[ WeightNoneNum ].Weight = 1.0f ;
									WeightNoneNum ++ ;
								}
							}
						}
					}
				}

				// �V�F�C�v�̏����擾
				if( _FbxMesh->GetDeformerCount( FbxDeformer::eBlendShape ) > 0 )
				{
					int ChannelCount ;

					// �Q�ȏ�̃V�F�C�v�ɂ͖��Ή�
					if( _FbxMesh->GetDeformerCount( FbxDeformer::eBlendShape ) > 1 )
					{
						DXST_ERRORLOGFMT_ADDUTF16LE(( "\x46\x00\x62\x00\x78\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x3a\x00\x20\x00\x11\xff\xe1\x30\xc3\x30\xb7\x30\xe5\x30\x6b\x30\x07\x89\x70\x65\x6e\x30\xd6\x30\xec\x30\xf3\x30\xc9\x30\xb7\x30\xa7\x30\xa4\x30\xd7\x30\x6b\x30\x6f\x30\xfe\x5b\xdc\x5f\x57\x30\x66\x30\x44\x30\x7e\x30\x5b\x30\x93\x30\x0a\x00\x00"/*@ L"Fbx Load : �P���b�V���ɕ����̃u�����h�V�F�C�v�ɂ͑Ή����Ă��܂���\n" @*/ )) ;
						return -1 ;
					}

					_FbxBlendShape = ( FbxBlendShape * )_FbxMesh->GetDeformer( 0, FbxDeformer::eBlendShape ) ;

					ChannelCount = _FbxBlendShape->GetBlendShapeChannelCount() ;

					for( i = 0 ; i < ChannelCount ; i ++ )
					{
						_FbxBlendShapeChannel = _FbxBlendShape->GetBlendShapeChannel( i ) ;
						if( _FbxBlendShapeChannel == NULL )
						{
							continue ;
						}

						_FbxShape = _FbxBlendShapeChannel->GetTargetShape( 0 ) ;
						if( _FbxShape == NULL )
						{
							continue ;
						}

						// �V�F�C�v���̒ǉ�
//						FbxUTF8ToAnsi( _FbxShape->GetName(), ANSIBuffer, &ANSISize ) ;
						Shape = MV1RAddShape( RModel, _FbxShape->GetName(), Frame ) ; 

						// �Ώۃ��b�V���̃Z�b�g
						Shape->TargetMesh = Mesh ;

						// �@���͖���
						Shape->ValidVertexNormal = FALSE ;

						// ���_�̐���ۑ�
						Shape->VertexNum = _FbxShape->GetControlPointsCount() ;

						// ���_�f�[�^���i�[���郁�����̈�̊m��
						Shape->Vertex = ( MV1_SHAPE_VERTEX_R * )ADDMEMAREA( sizeof( MV1_SHAPE_VERTEX_R ) * Shape->VertexNum, &RModel->Mem ) ;
						if( Shape->Vertex == NULL )
						{
							DXST_ERRORLOGFMT_ADDUTF16LE(( "\x46\x00\x62\x00\x78\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x3a\x00\x20\x00\xb7\x30\xa7\x30\xa4\x30\xd7\x30\x02\x98\xb9\x70\xc7\x30\xfc\x30\xbf\x30\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Fbx Load : �V�F�C�v���_�f�[�^���i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
							return -1 ;
						}

						// ���_�f�[�^�𖄂߂�
						ShapeVert = Shape->Vertex ;
						FbxShapeVec = _FbxShape->GetControlPoints() ;
						FbxVec = _FbxMesh->GetControlPoints() ;
						for( j = 0 ; j < Shape->VertexNum ; j ++, ShapeVert ++, FbxShapeVec ++, FbxVec ++ )
						{
							ShapeVert->TargetPositionIndex = j ;
							ShapeVert->Position.x = ( float ) ( FbxShapeVec->mData[ 0 ] - FbxVec->mData[ 0 ] ) ;
							ShapeVert->Position.y = ( float ) ( FbxShapeVec->mData[ 1 ] - FbxVec->mData[ 1 ] ) ;
							ShapeVert->Position.z = ( float )-( FbxShapeVec->mData[ 2 ] - FbxVec->mData[ 2 ] ) ;
							ShapeVert->Normal.x = 0.0f ;
							ShapeVert->Normal.y = 0.0f ;
							ShapeVert->Normal.z = 0.0f ;
						}
					}
				}
			}
			else
			// ���C�g�m�[�h�̏ꍇ�̓��C�g���擾����
			if( FbxAttr->GetAttributeType() == FbxNodeAttribute::eLight )
			{
				FbxLight *_FbxLight ;

				_FbxLight = ( FbxLight * )FbxAttr ;

				// ���C�g�̒ǉ�
//				FbxUTF8ToAnsi( pFbxNode->GetName(), ANSIBuffer, &ANSISize ) ;
				Frame->Light = MV1RAddLight( RModel, pFbxNode->GetName() ) ;
				if( Frame->Light == NULL )
				{
					DXST_ERRORLOGFMT_ADDUTF16LE(( "\x46\x00\x62\x00\x78\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x3a\x00\x20\x00\xe9\x30\xa4\x30\xc8\x30\xaa\x30\xd6\x30\xb8\x30\xa7\x30\xaf\x30\xc8\x30\x6e\x30\xfd\x8f\xa0\x52\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Fbx Load : ���C�g�I�u�W�F�N�g�̒ǉ��Ɏ��s���܂���\n" @*/ )) ;
					return -1 ;
				}

				// ���C�g�̏����擾����
				switch( _FbxLight->LightType.Get() )
				{
				case FbxLight::ePoint :
					Frame->Light->Type = MV1_LIGHT_TYPE_POINT ;
					break ;

				case FbxLight::eSpot :
					Frame->Light->Type = MV1_LIGHT_TYPE_SPOT ;
					break ;

				case FbxLight::eDirectional :
					Frame->Light->Type = MV1_LIGHT_TYPE_DIRECTIONAL ;
					break ;
				}
				Frame->Light->Diffuse.r = ( float )_FbxLight->Color.Get()[ 0 ] ;
				Frame->Light->Diffuse.g = ( float )_FbxLight->Color.Get()[ 1 ] ;
				Frame->Light->Diffuse.b = ( float )_FbxLight->Color.Get()[ 2 ] ;
				Frame->Light->Range = ( float )_FbxLight->Intensity.Get() / 10.0f ;
				Frame->Light->Falloff = 0.1f ;
				Frame->Light->Attenuation0 = 1.0f ;
				Frame->Light->Attenuation1 = 0.0f ;
				Frame->Light->Attenuation2 = 0.0f ;
				Frame->Light->Theta = ( float )_FbxLight->InnerAngle.Get() ;
				Frame->Light->Phi = ( float )_FbxLight->OuterAngle.Get() ;
				Frame->Light->FrameIndex = Frame->Index ;
			}
		}
	}

	// �q�̃m�[�h����͂���
	Num = pFbxNode->GetChildCount() ;
	for( i = 0 ; i < Num ; i ++ )
	{
		pFbxChildNode = pFbxNode->GetChild( i ) ;
		if( AnalyseFbxNode( RModel, Model, Frame, pFbxChildNode ) == -1 )
			return -1 ;
	}

	// �I��
	return 0 ;
}

// FBX�t�@�C���̉��( 0:����  -1:���s )
static int AnalyseFbx( MV1_MODEL_R *RModel, FBX_MODEL *Model )
{
	int i, j, k ;
	MV1_MESH_R *Mesh ;
	MV1_FRAME_R *TargetFrame ;
	MV1_SKIN_WEIGHT_R *SkinWeight ;
	void *Node ;
//	char *ANSIBuffer ;
//	size_t ANSISize ;

	// �m�[�h�̉��
	if( AnalyseFbxNode( RModel, Model, NULL, NULL ) == -1 )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\x46\x00\x62\x00\x78\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x3a\x00\x20\x00\xce\x30\xfc\x30\xc9\x30\x6e\x30\xe3\x89\x90\x67\x67\x30\xa8\x30\xe9\x30\xfc\x30\x4c\x30\x7a\x76\x1f\x75\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Fbx Load : �m�[�h�̉�͂ŃG���[���������܂���\n" @*/ )) ;
		return -1 ;
	}

	// �X�L���E�G�C�g���Ɋ֘A����t���[��������o���Ă���
	{
		// ���b�V���̐������J��Ԃ�
		Mesh = RModel->MeshFirst ;
		for( i = 0 ; ( DWORD )i < RModel->MeshNum ; i ++, Mesh = Mesh->DataNext )
		{
			// �X�L���E�G�C�g�̐������J��Ԃ�
			for( j = 0 ; ( DWORD )j < Mesh->SkinWeightsNum ; j ++ )
			{
				SkinWeight = Mesh->SkinWeights[ j ] ;

				// UserData �� NULL �̏ꍇ�̓��b�V�����������Ă���t���[�����Ώۃt���[��
				if( SkinWeight->UserData == NULL )
				{
					TargetFrame = Mesh->Container ;
					SkinWeight->TargetFrame = TargetFrame->Index ;
				}
				else
				{
					// ��v����t���[������������
					Node = ( ( FbxCluster * )SkinWeight->UserData )->GetLink() ;
					TargetFrame = RModel->FrameFirst ;
					for( k = 0 ; ( DWORD )k < RModel->FrameNum && TargetFrame->UserData != Node ; k ++, TargetFrame = TargetFrame->DataNext ){}
					SkinWeight->TargetFrame = k ;
				}
			}
		}
	}

	// �A�j���[�V�����̉��
	{
		int TakeNum ;
		MV1_ANIMSET_R *AnimSet ;
		MV1_ANIM_R *Anim ;
		MV1_FRAME_R *Frame ;
		FbxAnimStack *lFbxAnimStack ;

		// �A�j���[�V�����Z�b�g�̐����擾
		TakeNum = Model->pScene->GetSrcObjectCount< FbxAnimStack >() ;

		// �A�j���[�V�����̐������J��Ԃ�
		for( i = 0 ; i < TakeNum ; i ++ )
		{
			// �e�C�N�����擾����
			lFbxAnimStack = Model->pScene->GetSrcObject< FbxAnimStack >( i ) ;

			// �A�j���[�V������񂪖��������炱���ŏI��
			if( lFbxAnimStack == NULL ) continue ;
			if( lFbxAnimStack->GetMemberCount< FbxAnimLayer >() == 0 ) continue ;

			// �A�j���[�V�����Z�b�g��ǉ�
//			FbxUTF8ToAnsi( lFbxAnimStack->GetName(), ANSIBuffer, &ANSISize ) ;
			AnimSet = MV1RAddAnimSet( RModel, lFbxAnimStack->GetName() ) ;
			if( AnimSet == NULL )
			{
//				DeleteAndClear( FbxTakeName ) ;
				DXST_ERRORLOGFMT_ADDUTF16LE(( "\x46\x00\x62\x00\x78\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x3a\x00\x20\x00\xa2\x30\xcb\x30\xe1\x30\xfc\x30\xb7\x30\xe7\x30\xf3\x30\xbb\x30\xc3\x30\xc8\x30\xaa\x30\xd6\x30\xb8\x30\xa7\x30\xaf\x30\xc8\x30\x6e\x30\xfd\x8f\xa0\x52\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Fbx Load : �A�j���[�V�����Z�b�g�I�u�W�F�N�g�̒ǉ��Ɏ��s���܂���\n" @*/ )) ;
				return -1 ;
			}

			// �J�n�����ƏI�������̏�����
			AnimSet->StartTime =  1000000000.0f ;
			AnimSet->EndTime   = -1000000000.0f ;

			FbxAnimLayer *pFbxAnimLayer = lFbxAnimStack->GetMember< FbxAnimLayer >( 0 ) ;

			// �A�j���[�V�����̏����擾����
			Frame = RModel->FrameFirst ;
			for( j = 0 ; ( DWORD )j < RModel->FrameNum ; j ++, Frame = Frame->DataNext )
			{
				// �e�C�N�m�[�h�̎擾
				FbxNode *pFbxNode = ( FbxNode * )Frame->UserData ;

				// �J�[�u�f�[�^�����݂���v�f�̃A�j���[�V���������擾����
				Anim = NULL ;
				if( GetFbxAnimInfo( RModel, Model, Frame, AnimSet, &Anim, MV1_ANIMKEY_DATATYPE_TRANSLATE_X, pFbxNode->LclTranslation.GetCurve( pFbxAnimLayer, FBXSDK_CURVENODE_COMPONENT_X )              ) == -1 ) return -1 ;
				if( GetFbxAnimInfo( RModel, Model, Frame, AnimSet, &Anim, MV1_ANIMKEY_DATATYPE_TRANSLATE_Y, pFbxNode->LclTranslation.GetCurve( pFbxAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y )              ) == -1 ) return -1 ;
				if( GetFbxAnimInfo( RModel, Model, Frame, AnimSet, &Anim, MV1_ANIMKEY_DATATYPE_TRANSLATE_Z, pFbxNode->LclTranslation.GetCurve( pFbxAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z ), true        ) == -1 ) return -1 ;
				if( GetFbxAnimInfo( RModel, Model, Frame, AnimSet, &Anim, MV1_ANIMKEY_DATATYPE_SCALE_X,     pFbxNode->LclScaling.GetCurve(     pFbxAnimLayer, FBXSDK_CURVENODE_COMPONENT_X )              ) == -1 ) return -1 ;
				if( GetFbxAnimInfo( RModel, Model, Frame, AnimSet, &Anim, MV1_ANIMKEY_DATATYPE_SCALE_Y,     pFbxNode->LclScaling.GetCurve(     pFbxAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y )              ) == -1 ) return -1 ;
				if( GetFbxAnimInfo( RModel, Model, Frame, AnimSet, &Anim, MV1_ANIMKEY_DATATYPE_SCALE_Z,     pFbxNode->LclScaling.GetCurve(     pFbxAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z )              ) == -1 ) return -1 ;
				if( GetFbxAnimInfo( RModel, Model, Frame, AnimSet, &Anim, MV1_ANIMKEY_DATATYPE_ROTATE_X,    pFbxNode->LclRotation.GetCurve(    pFbxAnimLayer, FBXSDK_CURVENODE_COMPONENT_X ), true,  true ) == -1 ) return -1 ;
				if( GetFbxAnimInfo( RModel, Model, Frame, AnimSet, &Anim, MV1_ANIMKEY_DATATYPE_ROTATE_Y,    pFbxNode->LclRotation.GetCurve(    pFbxAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y ), true,  true ) == -1 ) return -1 ;
				if( GetFbxAnimInfo( RModel, Model, Frame, AnimSet, &Anim, MV1_ANIMKEY_DATATYPE_ROTATE_Z,    pFbxNode->LclRotation.GetCurve(    pFbxAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z ), false, true ) == -1 ) return -1 ;

				// �L�[�Z�b�g��������������炱�̃m�[�h�ɂ̓A�j���[�V�����������Ƃ�������
				if( Anim == NULL )
					continue ;

				// ���Ԃ̃Z�b�g
				Anim->MaxTime = AnimSet->EndTime ;

				// �m�[�h�̖��O��ۑ�
				Anim->TargetFrameIndex = Frame->Index ;

				// ��]�I�[�_�[�̃Z�b�g
				Anim->RotateOrder = Frame->RotateOrder ;
			}
		}
	}

	// ����
	return 0 ;
}

extern int MV1LoadModelToFBX( const MV1_MODEL_LOAD_PARAM *LoadParam, int ASyncThread )
{
	int NewHandle = -1 ;
	int iFileFormat = -1 ;
	int ErrorFlag = 1 ;
	FBX_MODEL FbxModel ;
	MV1_MODEL_R RModel ;
	int Major, Minor, Revision ;
	char UTF8Buffer[ 512 * 3 + 16 ] ;

	// �ǂݍ��݂悤�f�[�^�̏�����
	MV1InitReadModel( &RModel ) ;
	RModel.MeshFaceRightHand = TRUE ;

	// ���f�����ƃt�@�C�����ƕ����R�[�h�`�����Z�b�g
	RModel.CharCodeFormat = DX_CHARCODEFORMAT_UTF8 ;
	RModel.FilePath = ( wchar_t * )DXALLOC( ( _WCSLEN( LoadParam->FilePath ) + 1 ) * sizeof( wchar_t ) ) ;
	RModel.Name     = ( wchar_t * )DXALLOC( ( _WCSLEN( LoadParam->Name     ) + 1 ) * sizeof( wchar_t ) ) ;
	_WCSCPY( RModel.FilePath, LoadParam->FilePath ) ;
	_WCSCPY( RModel.Name,     LoadParam->Name ) ;

	// FBX���f���f�[�^���O������
	_MEMSET( &FbxModel, 0, sizeof( FbxModel ) ) ;

	// SDK�}�l�[�W������
	FbxModel.pManager = FbxManager::Create();
	if( FbxModel.pManager == NULL )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\x46\x00\x62\x00\x78\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x3a\x00\x20\x00\x46\x00\x42\x00\x58\x00\x20\x00\x4d\x00\x61\x00\x6e\x00\x61\x00\x67\x00\x65\x00\x72\x00\x20\x00\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Fbx Load : FBX Manager �̍쐬�Ɏ��s���܂���\n" @*/ )) ;
		goto FUNCTIONEND ;
	}
	
	// IOSettings �N���X�̍쐬
	FbxModel.pIOSettings = FbxIOSettings::Create( FbxModel.pManager, IOSROOT ) ;
	FbxModel.pManager->SetIOSettings( FbxModel.pIOSettings ) ;

	// �v���O�C���p�X�̐ݒ�
	{
		FbxString lPath = FbxGetApplicationDirectory() ;
		FbxModel.pManager->LoadPluginsDirectory( lPath.Buffer() ) ;
	}

	// �V�[���̍쐬
	FbxModel.pScene = FbxScene::Create( FbxModel.pManager, "Scene" ) ;
	if( FbxModel.pScene == NULL )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\x46\x00\x62\x00\x78\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x3a\x00\x20\x00\x46\x00\x42\x00\x58\x00\x20\x00\x53\x00\x63\x00\x65\x00\x6e\x00\x65\x00\x20\x00\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Fbx Load : FBX Scene �̍쐬�Ɏ��s���܂���\n" @*/ )) ;
		goto FUNCTIONEND ;
	}

	// �C���|�[�^�[�̍쐬
	FbxModel.pImporter = FbxImporter::Create( FbxModel.pManager, "" ) ;
	if( FbxModel.pImporter == NULL )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\x46\x00\x62\x00\x78\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x3a\x00\x20\x00\xa4\x30\xf3\x30\xdd\x30\xfc\x30\xbf\x30\xfc\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Fbx Load : �C���|�[�^�[�̍쐬�Ɏ��s���܂���\n" @*/ )) ;
		goto FUNCTIONEND ;
	}

	// FBX�t�H�[�}�b�g�̃`�F�b�N
	ConvString( ( const char * )LoadParam->FilePath, WCHAR_T_CHARCODEFORMAT, ( char * )UTF8Buffer, sizeof( UTF8Buffer ), DX_CHARCODEFORMAT_UTF8 ) ;
	if( FbxModel.pManager->GetIOPluginRegistry()->DetectReaderFileFormat( UTF8Buffer, iFileFormat ) == false )
	{
		goto FUNCTIONEND ;
	}

	// �ǂݍ���
	if( FbxModel.pImporter->Initialize( UTF8Buffer, iFileFormat, FbxModel.pIOSettings ) == false )
	{
		goto FUNCTIONEND ;
	}

	// �e�a�w���`�F�b�N
	if( FbxModel.pImporter->IsFBX() == false )
		goto FUNCTIONEND ;

	// �ǂݎ����̐ݒ�
	FbxModel.pIOSettings->SetBoolProp( IMP_FBX_MATERIAL,		true ) ;
	FbxModel.pIOSettings->SetBoolProp( IMP_FBX_TEXTURE,			true ) ;
	FbxModel.pIOSettings->SetBoolProp( IMP_FBX_LINK,			true ) ;
	FbxModel.pIOSettings->SetBoolProp( IMP_FBX_SHAPE,			true ) ;
	FbxModel.pIOSettings->SetBoolProp( IMP_FBX_GOBO,			true ) ;
	FbxModel.pIOSettings->SetBoolProp( IMP_FBX_ANIMATION,		true ) ;
	FbxModel.pIOSettings->SetBoolProp( IMP_FBX_GLOBAL_SETTINGS, true ) ;

	// �C���|�[�g
	if( FbxModel.pImporter->Import( FbxModel.pScene ) == false )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\x46\x00\x62\x00\x78\x00\x20\x00\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x3a\x00\x20\x00\xb7\x30\xfc\x30\xf3\x30\x6e\x30\xa4\x30\xf3\x30\xdd\x30\xfc\x30\xc8\x30\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Fbx Load : �V�[���̃C���|�[�g�Ɏ��s���܂���\n" @*/ )) ;
		goto FUNCTIONEND ;
	}

	// �o�[�W�������擾
	FbxModel.pImporter->GetFileVersion( Major, Minor, Revision ) ;

	// �C���|�[�^�̍폜
	FbxModel.pImporter->Destroy() ;
	FbxModel.pImporter = NULL ;

	// �m�[�h����J��
	if( AnalyseFbx( &RModel, &FbxModel ) == -1 )
		goto FUNCTIONEND ;

	// ���f����f�[�^�n���h���̍쐬
	NewHandle = MV1LoadModelToReadModel( &LoadParam->GParam, &RModel, LoadParam->CurrentDir, LoadParam->FileReadFunc, ASyncThread ) ;
	if( NewHandle < 0 ) goto FUNCTIONEND ;

	// �G���[�t���O��|��
	ErrorFlag = 0 ;

FUNCTIONEND :

	// �G���[�t���O�������Ă����烂�f���n���h�������
	if( ErrorFlag == 1 && NewHandle != -1 )
	{
		MV1SubModelBase( NewHandle ) ;
		NewHandle = -1 ;
	}

	// �C���|�[�^�[�̉��
	if( FbxModel.pImporter )
	{
		FbxModel.pImporter->Destroy() ;
		FbxModel.pImporter = NULL ;
	}

	if( FbxModel.pIOSettings )
	{
		FbxModel.pIOSettings->Destroy() ;
		FbxModel.pIOSettings = NULL ;
	}

	// �V�[���̉��
	if( FbxModel.pScene )
	{
		FbxModel.pScene->Destroy() ;
		FbxModel.pScene = NULL ;
	}

	// SDK�}�l�[�W���̉��
	if( FbxModel.pManager )
	{
		FbxModel.pManager->Destroy() ;
		FbxModel.pManager = NULL ;
	}

	// �ǂݍ��݃��f���̌�n��
	MV1TermReadModel( &RModel ) ;

	// �n���h����Ԃ�
	return NewHandle ;
}

#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

#endif

#endif
