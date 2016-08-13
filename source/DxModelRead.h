// -------------------------------------------------------------------------------
// 
// 		�c�w���C�u����		���f���f�[�^�ǂݍ��݃w�b�_
// 
// 				Ver 3.16d
// 
// -------------------------------------------------------------------------------

#ifndef __DXMODELREAD_H__
#define __DXMODELREAD_H__

#include "DxCompileConfig.h"

#ifndef DX_NON_MODEL

// �C���N���[�h ---------------------------------
#include "DxLib.h"
#include "DxStatic.h"
#include "DxModel.h"

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// �}�N����` -----------------------------------

#define MV1_READ_MAX_UV_NUM						(8)				// ��̒��_�Ŏg�p�ł���ő�t�u��
#define MV1_READ_MAX_MESH_MATERIAL_NUM			(1024)			// ��̃��b�V���Ŏg�p�ł���}�e���A���̍ő吔
#define MV1_READ_MAX_MESH_SKINWEIGHT_NUM		(4096)			// ��̃��b�V���Ŏg�p�ł���X�L���E�G�C�g�̍ő吔

// �f�[�^�^��` ---------------------------------

// ���C�g�\����
struct MV1_LIGHT_R
{
	MV1_LIGHT_R				*DataPrev ;						// (�ǂݎ���p)�f�[�^�I�ɑO�̃��C�g
	MV1_LIGHT_R				*DataNext ;						// (�ǂݎ���p)�f�[�^�I�Ɏ��̃��C�g
	int						Index ;							// (�ǂݎ���p)�C���f�b�N�X
#ifndef UNICODE
	char					*NameA ;						// (�ǂݎ���p)���O
#endif
	wchar_t					*NameW ;						// (�ǂݎ���p)���O

	int						FrameIndex ;					// ���C�g�������Ă���t���[���̃C���f�b�N�X
	int						Type ;							// ���C�g�^�C�v( MV1_LIGHT_TYPE_POINT �� )
	COLOR_F					Diffuse ;						// �f�B�t���[�Y�J���[
	COLOR_F					Specular ;						// �X�y�L�����J���[
	COLOR_F					Ambient ;						// �A���r�G���g�J���[
	float					Range ;							// �X�|�b�g���C�g�̗L������
	float					Falloff ;						// �t�H�[���I�t
	float					Attenuation0 ;					// ���C�g�p�����[�^�O
	float					Attenuation1 ;					// ���C�g�p�����[�^�P
	float					Attenuation2 ;					// ���C�g�p�����[�^�Q
	float					Theta ;							// �X�|�b�g���C�g�̓����R�[���̊p�x�͈�
	float					Phi ;							// �X�|�b�g���C�g�̊O���R�[���̊p�x�͈�
} ;

// �A�j���[�V�����L�[�Z�b�g���\����
struct MV1_ANIMKEYSET_R
{
	MV1_ANIMKEYSET_R					*DataPrev ;				// (�ǂݎ���p)�f�[�^�I�Ɉ�O�̃A�j���[�V�����L�[�Z�b�g
	MV1_ANIMKEYSET_R					*DataNext ;				// (�ǂݎ���p)�f�[�^�I�Ɉ��̃A�j���[�V�����L�[�Z�b�g
	int									Index ;					// (�ǂݎ���p)�C���f�b�N�X

	char								Type ;					// �L�[�^�C�v( MV1_ANIMKEY_TYPE_QUATERNION �� )
	char								DataType ;				// �f�[�^�^�C�v( MV1_ANIMKEY_DATATYPE_ROTATE �� )
	char								TimeType ;				// ���ԃ^�C�v( MV1_ANIMKEY_TIME_TYPE_KEY �� )
	int									TargetShapeIndex ;		// �Ώۂ̃V�F�C�v�C���f�b�N�X( DataType �� MV1_ANIMKEY_DATATYPE_SHAPE �̏ꍇ�̂ݗL�� )

	float								TotalTime ;				// ������
	int									Num ;					// �L�[�̐�

	float								UnitTime ;				// �P�ʎ���( TimeType �� MV1_ANIMKEY_TIME_TYPE_ONE �̏ꍇ�̂ݗL�� )
	union
	{
		float							*KeyTime ;				// �L�[���Ԕz��ւ̃|�C���^( TimeType �� MV1_ANIMKEY_TIME_TYPE_KEY �̏ꍇ�̂ݗL�� )
		float							StartTime ;				// �J�n����( TimeType �� MV1_ANIMKEY_TIME_TYPE_ONE �̏ꍇ�̂ݗL�� )
	} ;

	union
	{
		FLOAT4							*KeyFloat4 ;			// float4�L�[�z��ւ̃|�C���^
		VECTOR							*KeyVector ;			// �x�N�^�[�L�[�z��ւ̃|�C���^
		MV1_ANIM_KEY_MATRIX4X4C			*KeyMatrix4x4C ;		// �S��ڏȗ��S�~�S�s��L�[�z��ւ̃|�C���^
		MV1_ANIM_KEY_MATRIX3X3			*KeyMatrix3x3 ;			// �R���R�s��L�[�z��ւ̃|�C���^
		float							*KeyFlat ;				// �t���b�g�L�[�z��ւ̃|�C���^
		float							*KeyLinear ;			// ���`��ԃL�[�z��ւ̃|�C���^
	} ;

	MV1_ANIMKEYSET_R					*Prev ;					// (�ǂݎ���p)�O�̃A�j���[�V�����L�[�Z�b�g�ւ̃|�C���^
	MV1_ANIMKEYSET_R					*Next ;					// (�ǂݎ���p)���̃A�j���[�V�����L�[�Z�b�g�ւ̃|�C���^
} ;

// �A�j���[�V�������\����
struct MV1_ANIM_R
{
	struct MV1_ANIMSET_R	*Container ;					// (�ǂݎ���p)���̃f�[�^���܂�ł���A�j���[�V�����Z�b�g�ւ̃|�C���^

	MV1_ANIM_R				*DataPrev ;						// (�ǂݎ���p)�f�[�^�I�Ɉ�O�̃A�j���[�V����
	MV1_ANIM_R				*DataNext ;						// (�ǂݎ���p)�f�[�^�I�Ɉ��̃A�j���[�V����
	int						Index ;							// (�ǂݎ���p)�C���f�b�N�X

	int						TargetFrameIndex ;				// �ΏۂƂȂ�m�[�h�̃C���f�b�N�X
	float					MaxTime ;						// �e�L�[�Z�b�g�̒��ň�Ԓ����L�[�Z�b�g�̎��Ԓl
	int						RotateOrder ;					// ��]�I�[�_�[( MV1_ROTATE_ORDER_XYZ �� )

	int						AnimKeySetNum ;					// (�ǂݎ���p)�L�[�Z�b�g�̐�
	MV1_ANIMKEYSET_R		*AnimKeySetFirst ;				// (�ǂݎ���p)�擪�̃L�[�Z�b�g
	MV1_ANIMKEYSET_R		*AnimKeySetLast ;				// (�ǂݎ���p)���[�̃L�[�Z�b�g

	MV1_ANIM_R				*Prev ;							// (�ǂݎ���p)�O�̃A�j���[�V����
	MV1_ANIM_R				*Next ;							// (�ǂݎ���p)���̃A�j���[�V����
} ;

// �A�j���[�V�����Z�b�g���\����
struct MV1_ANIMSET_R
{
	MV1_ANIMSET_R			*DataPrev ;						// (�ǂݎ���p)�f�[�^�I�Ɉ�O�̃A�j���[�V�����Z�b�g
	MV1_ANIMSET_R			*DataNext ;						// (�ǂݎ���p)�f�[�^�I�Ɉ��̃A�j���[�V�����Z�b�g
	int						Index ;							// (�ǂݎ���p)�C���f�b�N�X
#ifndef UNICODE
	const char				*NameA ;						// (�ǂݎ���p)���O
#endif
	const wchar_t			*NameW ;						// (�ǂݎ���p)���O

	float					StartTime ;						// �J�n����
	float					EndTime ;						// �I������
	int						IsAddAnim ;						// ���Z�A�j���[�V�������ǂ���( 1:���Z�A�j���[�V����  0:��Βl�A�j���[�V���� )
	int						IsMatrixLinearBlend ;			// �e�L�[�̕�Ԃ��s��P�ʂŐ��`��Ԃ����邩�ǂ����̃t���O( 1:�s��Ő��`���  0:�v�f�P�ʂŕ�� )
	int						IsLoopAnim ;					// ���[�v�p�A�j���[�V�������ǂ���( 1:���[�v�A�j���[�V����  0:�ʏ�A�j���[�V���� )

	int						AnimNum ;						// (�ǂݎ���p)�A�j���[�V�����f�[�^�̐�
	MV1_ANIM_R				*AnimFirst ;					// (�ǂݎ���p)�擪�̃A�j���[�V����
	MV1_ANIM_R				*AnimLast ;						// (�ǂݎ���p)���[�̃A�j���[�V����
} ;

// �e�N�X�`�����\����
struct MV1_TEXTURE_R
{
	MV1_TEXTURE_R			*DataPrev ;						// (�ǂݎ���p)�f�[�^�I�ɑO�̃e�N�X�`��
	MV1_TEXTURE_R			*DataNext ;						// (�ǂݎ���p)�f�[�^�I�Ɏ��̃e�N�X�`��
	int						Index ;							// (�ǂݎ���p)�C���f�b�N�X
#ifndef UNICODE
	const char				*NameA ;						// (�ǂݎ���p)�I�u�W�F�N�g�l�[��
#endif
	const wchar_t			*NameW ;						// (�ǂݎ���p)�I�u�W�F�N�g�l�[��

	void					*UserData ;						// ���[�U�[�f�[�^

	int						AddressModeU ;					// �A�h���X���[�h( MV1_TEXTURE_ADDRESS_MODE_WRAP �� )
	int						AddressModeV ;					// �A�h���X���[�h( MV1_TEXTURE_ADDRESS_MODE_WRAP �� )
	float					ScaleU ;						// �t���W�̃X�P�[�����O�l
	float					ScaleV ;						// �u���W�̃X�P�[�����O�l
	int						FilterMode ;					// �t�B���^�����O���[�h( MV1_TEXTURE_FILTER_MODE_POINT �� )

#ifndef UNICODE
	char					*ColorFileNameA ;				// (�ǂݎ���p)�f�B�t���[�Y�e�N�X�`���t�@�C����
#endif
	wchar_t					*ColorFileNameW ;				// (�ǂݎ���p)�f�B�t���[�Y�e�N�X�`���t�@�C����
#ifndef UNICODE
	char					*AlphaFileNameA ;				// (�ǂݎ���p)�A���t�@�`�����l���p�e�N�X�`���t�@�C����
#endif
	wchar_t					*AlphaFileNameW ;				// (�ǂݎ���p)�A���t�@�`�����l���p�e�N�X�`���t�@�C����
	int						BumpMapFlag ;					// (�ǂݎ���p)�J���[�`�����l�����o���v�}�b�v���ǂ���( TRUE:�o���v�}�b�v  FALSE:�Ⴄ )
	float					BumpMapNextPixelLength ;		// (�ǂݎ���p)�J���[�`�����l�����o���v�}�b�v�������ꍇ�́A�ׂ̃s�N�Z���Ƃ̋���
	int						BlendType ;						// �u�����h�^�C�v( MV1_LAYERBLEND_TYPE_TRANSLUCENT �� )

	int						ReverseFlag ;					// �摜�𔽓]���邩�ǂ���( 1:���]����  0:���]���Ȃ� )
	int						Bmp32AllZeroAlphaToXRGB8Flag ;	// �R�Q�r�b�g�a�l�o�̃A���t�@�l���S���O��������XRGB8�Ƃ��邩�ǂ���( 1:��������  0:�������Ȃ� )
} ;

// �}�e���A�����\����
struct MV1_MATERIAL_R
{
	MV1_MATERIAL_R			*DataPrev ;						// (�ǂݎ���p)�f�[�^�I�ɑO�̃}�e���A��
	MV1_MATERIAL_R			*DataNext ;						// (�ǂݎ���p)�f�[�^�I�Ɏ��̃}�e���A��
	int						Index ;							// (�ǂݎ���p)�C���f�b�N�X
#ifndef UNICODE
	const char				*NameA ;						// (�ǂݎ���p)���O
#endif
	const wchar_t			*NameW ;						// (�ǂݎ���p)���O
	const char				*SubName ;						// �ǂݍ��ݏ������̖��O�����������Ɏg�p���閼�O

	void					*UserData ;						// ���[�U�[�f�[�^

	int						Type ;							// �}�e���A���^�C�v( DX_MATERIAL_TYPE_NORMAL �Ȃ� )

	int						DisableBackCulling ;			// �o�b�N�J�����O���s��Ȃ����ǂ���( TRUE:�o�b�N�������O���s��Ȃ�  FALSE:�o�b�N�������O���s�� )

	int						DiffuseTexNum ;					// �f�B�t���[�Y�e�N�X�`���̐�
	MV1_TEXTURE_R			*DiffuseTexs[ 256 ] ;			// �f�B�t���[�Y�e�N�X�`���ւ̃|�C���^�̔z��
	int						SpecularTexNum ;				// �X�y�L�����e�N�X�`���̐�
	MV1_TEXTURE_R			*SpecularTexs[ 256 ] ;			// �X�y�L�����e�N�X�`���ւ̃|�C���^�̔z��
	int						NormalTexNum ;					// �@���}�b�v�e�N�X�`���̐�
	MV1_TEXTURE_R			*NormalTexs[ 256 ] ;			// �@���}�b�v�e�N�X�`���ւ̃|�C���^�̔z��

	COLOR_F					Ambient ;						// �A���r�G���g
	COLOR_F					Diffuse ;						// �f�B�t���[�Y
	COLOR_F					Specular ;						// �X�y�L����
	COLOR_F					Emissive ;						// �G�~�b�V�u
//	float					Shininess ;						// ����
//	float					Reflection ;					// ����
	float					Power ;							// ���˂̋���
//	float					Alpha ;							// �A���t�@�l

	int						DiffuseGradTextureDefault ;		// ( �g�D�[�������_�����O�ł̂ݎg�p )�f�B�t���[�Y�J���[�O���f�[�V�����e�N�X�`����������Ȃ������Ƃ��Ɏg�p����f�t�H���g�O���f�[�V�����e�N�X�`���^�C�v
	int						SpecularGradTextureDefault ;	// ( �g�D�[�������_�����O�ł̂ݎg�p )�X�y�L�����J���[�O���f�[�V�����e�N�X�`����������Ȃ������Ƃ��Ɏg�p����f�t�H���g�O���f�[�V�����e�N�X�`���^�C�v
	MV1_TEXTURE_R			*DiffuseGradTexture ;			// ( �g�D�[�������_�����O�ł̂ݎg�p )�f�B�t���[�Y�J���[�O���f�[�V�����e�N�X�`���A�������Ă��郉�C�g�̃f�B�t���[�Y�J���[�łt�l�����܂����
	MV1_TEXTURE_R			*SpecularGradTexture ;			// ( �g�D�[�������_�����O�ł̂ݎg�p )�X�y�L�����J���[�O���f�[�V�����e�N�X�`���A�������Ă��郉�C�g�̃X�y�L�����J���[�łt�l�����܂����
	MV1_TEXTURE_R			*SphereMapTexture ;				// ( �g�D�[�������_�����O�ł̂ݎg�p )�X�t�B�A�}�b�v�e�N�X�`���C���f�b�N�X
	int						DiffuseGradBlendType ;			// ( �g�D�[�������_�����O�ł̂ݎg�p )�f�B�t���[�Y�O���f�[�V�����e�N�X�`���̃u�����h�^�C�v( MV1_LAYERBLEND_TYPE_TRANSLUCENT �Ȃ� )
	int						SpecularGradBlendType ;			// ( �g�D�[�������_�����O�ł̂ݎg�p )�X�y�L�����O���f�[�V�����e�N�X�`���̃u�����h�^�C�v( MV1_LAYERBLEND_TYPE_ADDITIVE �Ȃ� )
	int						SphereMapBlendType ;			// ( �g�D�[�������_�����O�ł̂ݎg�p )�X�t�B�A�}�b�v�e�N�X�`���̃u�����h�^�C�v( DX_MATERIAL_BLENDTYPE_ADDITIVE �Ȃ� )
	float					OutLineWidth ;					// ( �g�D�[�������_�����O�ł̂ݎg�p )�֊s���̕�
	float					OutLineDotWidth ;				// ( �g�D�[�������_�����O�ł̂ݎg�p )�֊s���̃h�b�g�P�ʂł̕�
	COLOR_F					OutLineColor ;					// ( �g�D�[�������_�����O�ł̂ݎg�p )�֊s���̐F

	int						DrawBlendMode ;					// �o�͎��̃u�����h���[�h( DX_BLENDMODE_NOBLEND �� )
	int						DrawBlendParam ;				// �o�͎��̃u�����h�p�����[�^
} ;

// �ʏ��\����
struct MV1_MESHFACE_R
{
	DWORD					IndexNum ;							// ���_�C���f�b�N�X�̐�
	DWORD					PolygonNum ;						// (�ǂݎ���p)�R�p�`�|���S���̐�
	DWORD					*VertexIndex ;						// ���_�C���f�b�N�X
	DWORD					*NormalIndex ;						// �@���C���f�b�N�X
	DWORD					*VertexColorIndex ;					// ���_�J���[�C���f�b�N�X
	DWORD					*UVIndex[ MV1_READ_MAX_UV_NUM ] ;	// �t�u�C���f�b�N�X
	DWORD					MaterialIndex ;						// �}�e���A���C���f�b�N�X
	VECTOR					Normal ;							// �ʂ̖@��( �@�������v�Z���Ɏg�p )
} ;

// �g���C�A���O�����X�g�p�̏��(�R���o�[�g�v���O�������g�p)
struct MV1_TRIANGLELIST_R
{
	int						UseBoneNum ;						// �g�p���Ă���{�[���̐�
	unsigned short			UseBone[ MV1_TRIANGLE_LIST_USE_BONE_MAX_NUM ] ;	// �{�[���̃��X�g( Mesh.SkinWeights �z��̃C���f�b�N�X )
	int						PolyNum ;							// �|���S���̐�
} ;

// �}�e���A�����̃|���S�����(�R���o�[�g�v���O�������g�p)
struct MV1_MATERIAL_POLY_R
{
	DWORD					TypeNum[ MV1_VERTEX_TYPE_NUM ] ;	// �^�C�v�ʂ̃g���C�A���O���̐�( �O�p�`�|���S�����Z )
	int						MaxBoneCount ;						// ��̃|���S���Ɋւ���Ă���ő�{�[����
	int						SimpleTriangleListNum ;				// ���̃��b�V���̃g���C�A���O�����X�g�̐�
	MV1_TRIANGLELIST_R		SimpleTriangleList[ 128 ] ;			// ���̃��b�V���̃g���C�A���O�����X�g�̏��
	int						SkinB4TriangleListNum ;				// �{�[���S�ȓ��̃X�L�j���O���b�V���̃g���C�A���O�����X�g�̐�
	MV1_TRIANGLELIST_R		SkinB4TriangleList[ 128 ] ;			// �{�[���S�ȓ��̃X�L�j���O���b�V���̃g���C�A���O�����X�g�̏��
	int						SkinB8TriangleListNum ;				// �{�[���S�ȓ��̃X�L�j���O���b�V���̃g���C�A���O�����X�g�̐�
	MV1_TRIANGLELIST_R		SkinB8TriangleList[ 128 ] ;			// �{�[���S�ȓ��̃X�L�j���O���b�V���̃g���C�A���O�����X�g�̏��
} ;

// �X�L�����̂P���_������̏��
struct MV1_SKIN_WEIGHT_ONE_R
{
	DWORD					TargetVertex ;						// �ΏۂƂȂ钸�_�ԍ�
	float					Weight ;							// �E�G�C�g
} ;

// �X�L�����
struct MV1_SKIN_WEIGHT_R
{
	void					*UserData ;							// ���[�U�[�f�[�^

	MATRIX					ModelLocalMatrix ;					// �{�[�����[�J���̍��W�ɂ��邽�߂̍s��
	DWORD					TargetFrameUserID ;					// �Ώۃt���[���̃��[�U�[�h�c
	DWORD					TargetFrame ;						// �Ώۃt���[���̃C���f�b�N�X
	DWORD					DataNum ;							// �f�[�^�̐�
	MV1_SKIN_WEIGHT_ONE_R	*Data ;								// �f�[�^�z��
} ;

// �f�[�^���b�V�����\����
struct MV1_MESH_R
{
	MV1_MESH_R				*DataPrev ;											// (�ǂݎ���p)�f�[�^�I�ɑO�̃��b�V��
	MV1_MESH_R				*DataNext ;											// (�ǂݎ���p)�f�[�^�I�Ɏ��̃��b�V��
	int						Index ;												// (�ǂݎ���p)�C���f�b�N�X

	void					*UserData ;											// ���[�U�[�f�[�^

	DWORD					FaceNum ;											// (�ǂݎ���p)�ʂ̐�
	MV1_MESHFACE_R			*Faces ;											// (�ǂݎ���p)�ʏ��z��
	DWORD					*FaceIndexBuffer ;									// (�ǂݎ���p)�ʂ̃C���f�b�N�X�o�b�t�@
	DWORD					FaceUnitMaxIndexNum ;								// (�ǂݎ���p)�������̖ʂ̍ő�C���f�b�N�X�l

	DWORD					PositionNum ;										// ���_�ʒu���̐�
	VECTOR					*Positions ;										// ���_�ʒu���z��
	float					*PositionToonOutLineScale ;							// �g�D�[�������_�����O���̗֊s���̃X�P�[��

	DWORD					NormalNum ;											// �@�����̐�
	VECTOR					*Normals ;											// �@�����z��
	BYTE					*NormalSetFlag ;									// �@�����Z�b�g�t���O( �@�������Z�o�����p )
	VECTOR					*Binormals ;										// �]�@�����z��
	VECTOR					*Tangents ;											// �ڐ����z��

	DWORD					VertexColorNum ;									// ���_�J���[�̐�
	COLOR_F					*VertexColors ;										// ���_�J���[�z��

	DWORD					UVNum[ MV1_READ_MAX_UV_NUM ] ;						// �t�u���̐�
	FLOAT4					*UVs[ MV1_READ_MAX_UV_NUM ] ;						// �t�u���z��( x=u y=v )

	DWORD					MaterialNum ;										// �}�e���A���̐�
	MV1_MATERIAL_R			*Materials[ MV1_READ_MAX_MESH_MATERIAL_NUM ] ;		// �}�e���A���̏��ւ̃|�C���^�̔z��

	DWORD					SkinWeightsNum ;									// �X�L�����b�V���̃{�[���̐�
	MV1_SKIN_WEIGHT_R		*SkinWeights[ MV1_READ_MAX_MESH_SKINWEIGHT_NUM ] ;	// �X�L�����b�V�����ւ̃|�C���^�z��

	// �R���o�[�g�v���O�������g�p
	DWORD					MaxBoneUseNum ;										// (�ǂݎ���p)��̒��_�f�[�^���u�����h���Ă���{�[���̍ő吔
	DWORD					TriangleNum ;										// (�ǂݎ���p)�O�p�`�|���S���̐�
	DWORD					TriangleListNum ;									// (�ǂݎ���p)�O�p�`�|���S�����X�g�̐�
	DWORD					VertexDataTotalSize ;								// (�ǂݎ���p)���_�f�[�^�̑��T�C�Y
	DWORD					IndexDataTotalSize ;								// (�ǂݎ���p)���_�C���f�b�N�X�f�[�^�̑��T�C�Y

	DWORD					TypeNumVertexDataSize[ MV1_VERTEX_TYPE_NUM ] ;		// (�ǂݎ���p)�e�^�C�v�̒��_�f�[�^�̑��T�C�Y
	DWORD					TypeNumIndexDataSize[ MV1_VERTEX_TYPE_NUM ] ;		// (�ǂݎ���p)�e�^�C�v�̒��_�C���f�b�N�X�f�[�^�̑��T�C�Y

	BITLIST					SkinVerticesBlend ;									// (�ǂݎ���p)�e���_���ǂ̃{�[���̉e�����󂯂�̂����AnVertices �̐���������
	BITLIST					SkinFacesBlend ;									// (�ǂݎ���p)�e�ʂ��ǂ̃{�[���̉e�����󂯂�̂����A nFaces �̐��������� 
	BYTE					*SkinFaceType ;										// (�ǂݎ���p)�g���C�A���O���^�C�v�z��AnFaces �̐���������
	float					*SkinVerticeWeightInfo ;							// (�ǂݎ���p)�e���_�̃{�[���E�G�C�g�l
	WORD					*SkinFaceBoneNum ;									// (�ǂݎ���p)�e���b�V���Ɋւ���Ă���{�[���̐�
	WORD					*FaceUseTriangleList ;								// (�ǂݎ���p)�e�t�F�C�X���g�p����g���C�A���O�����X�g�ԍ�
	MV1_MATERIAL_POLY_R		*MaterialPolyList ;									// (�ǂݎ���p)�}�e���A�����̃|���S�����

//	VECTOR					*Binormals ;										// (�ǂݎ���p)�]�@���z��
//	VECTOR					*Tangents ;											// (�ǂݎ���p)�ڐ��z��

	DWORD					IsShapeMesh ;										// (�ǂݎ���p)�V�F�C�v���b�V�����ǂ���

	struct MV1_FRAME_R		*Container ;										// (�ǂݎ���p)���̃��b�V���������Ă���t���[��
	MV1_MESH_R				*Next ;												// (�ǂݎ���p)���̃��b�V��
	MV1_MESH_R				*Prev ;												// (�ǂݎ���p)�O�̃��b�V��
} ;

// �V�F�C�v���_�f�[�^�\����
struct MV1_SHAPE_VERTEX_R
{
	int						TargetPositionIndex ;			// �Ώۂ̍��W�f�[�^�C���f�b�N�X( MV1_MESH_R.Positions �z��̗v�f�ԍ� )
	VECTOR					Position ;						// ���W( ���̍��W�ɑ΂��鍷�� )
	VECTOR					Normal ;						// �@��( ���̖@���ɑ΂��鍷�� )
} ;

// �V�F�C�v���\����
struct MV1_SHAPE_R
{
	MV1_SHAPE_R				*DataPrev ;						// (�ǂݎ���p)�f�[�^�I�ɑO�̃t���[��
	MV1_SHAPE_R				*DataNext ;						// (�ǂݎ���p)�f�[�^�I�Ɏ��̃t���[��
	int						Index ;							// (�ǂݎ���p)�C���f�b�N�X
#ifndef UNICODE
	const char				*NameA ;						// (�ǂݎ���p)���O
#endif
	const wchar_t			*NameW ;						// (�ǂݎ���p)���O

	MV1_MESH_R				*TargetMesh ;					// �Ώۃ��b�V��
	int						ValidVertexNormal ;				// ���_�f�[�^�̖@�����L�����ǂ���( TRUE:�L��  FALSE:���� )
	int						VertexNum ;						// ���_�̐�
	MV1_SHAPE_VERTEX_R		*Vertex ;						// �V�F�C�v���_�Ŕz��ւ̃|�C���^

	DWORD					*NextTable ;					// (�ǂݎ���p)�V�F�C�v�Ώۂ̍��W�v�f�ɑΉ����� Vertex �z��̗v�f�ԍ����i�[���ꂽ�e�[�u��
	struct MV1_FRAME_R		*Container ;					// (�ǂݎ���p)���̃V�F�C�v�������Ă���t���[��
	MV1_SHAPE_R				*Next ;							// (�ǂݎ���p)���̃��b�V��
	MV1_SHAPE_R				*Prev ;							// (�ǂݎ���p)�O�̃��b�V��
} ;

// �������Z�p���̃f�[�^�\����
struct MV1_PHYSICS_RIGIDBODY_R
{
	MV1_PHYSICS_RIGIDBODY_R	*DataPrev ;						// (�ǂݎ���p)�f�[�^�I�ɑO�̍��̃f�[�^
	MV1_PHYSICS_RIGIDBODY_R	*DataNext ;						// (�ǂݎ���p)�f�[�^�I�Ɏ��̍��̃f�[�^
	int						Index ;							// (�ǂݎ���p)�C���f�b�N�X
#ifndef UNICODE
	const char				*NameA ;						// (�ǂݎ���p)���O
#endif
	const wchar_t			*NameW ;						// (�ǂݎ���p)���O

	struct MV1_FRAME_R		*TargetFrame ;					// (�ǂݎ���p)�ΏۂƂȂ�t���[��

	int						RigidBodyGroupIndex ;			// ���̃O���[�v�ԍ�
	DWORD					RigidBodyGroupTarget ;			// ���̃O���[�v�Ώ�
	int						ShapeType ;						// �`��( 0:��  1:��  2:�J�v�Z�� )
	float					ShapeW ;						// ��
	float					ShapeH ;						// ����
	float					ShapeD ;						// ���s
	VECTOR					Position ;						// �ʒu
	VECTOR					Rotation ;						// ��]( ���W�A�� )
	float					RigidBodyWeight ;				// ����
	float					RigidBodyPosDim ;				// �ړ���
	float					RigidBodyRotDim ;				// ��]��
	float					RigidBodyRecoil ;				// ������
	float					RigidBodyFriction ;				// ���C��
	int						RigidBodyType ;					// ���̃^�C�v( 0:Bone�Ǐ]  1:�������Z  2:�������Z(Bone�ʒu���킹) )
	int						NoCopyToBone ;					// �{�[���̍s��𕨗��ɓK�p���Ȃ����ǂ���
} ;

// �������Z�p���̃W���C���g�f�[�^�\����
struct MV1_PHYSICS_JOINT_R
{
	MV1_PHYSICS_JOINT_R		*DataPrev ;						// (�ǂݎ���p)�f�[�^�I�ɑO�̍��̃W���C���g�f�[�^
	MV1_PHYSICS_JOINT_R		*DataNext ;						// (�ǂݎ���p)�f�[�^�I�Ɏ��̍��̃W���C���g�f�[�^
	int						Index ;							// (�ǂݎ���p)�C���f�b�N�X
#ifndef UNICODE
	const char				*NameA ;						// (�ǂݎ���p)���O
#endif
	const wchar_t			*NameW ;						// (�ǂݎ���p)���O

	DWORD					RigidBodyA ;					// �ڑ��捄�̂`�̃f�[�^�C���f�b�N�X
	DWORD					RigidBodyB ;					// �ڑ��捄�̂a�̃f�[�^�C���f�b�N�X
	VECTOR					Position ;						// �ʒu
	VECTOR					Rotation ;						// ��]( ���W�A�� )
	VECTOR					ConstrainPosition1 ;			// �ړ������l�P
	VECTOR					ConstrainPosition2 ;			// �ړ������l�Q
	VECTOR					ConstrainRotation1 ;			// ��]�����l�P
	VECTOR					ConstrainRotation2 ;			// ��]�����l�Q
	VECTOR					SpringPosition ;				// �΂ˈړ��l
	VECTOR					SpringRotation ;				// �΂ˉ�]�l
} ;

// �f�[�^�t���[�����\����
struct MV1_FRAME_R
{
	MV1_FRAME_R				*DataPrev ;						// (�ǂݎ���p)�f�[�^�I�ɑO�̃t���[��
	MV1_FRAME_R				*DataNext ;						// (�ǂݎ���p)�f�[�^�I�Ɏ��̃t���[��
	int						Index ;							// (�ǂݎ���p)�C���f�b�N�X(�m�[�h��ǉ�����ƒl���ω�����\���L��)
#ifndef UNICODE
	const char				*NameA ;						// (�ǂݎ���p)���O
#endif
	const wchar_t			*NameW ;						// (�ǂݎ���p)���O

	int						TotalMeshNum ;					// (�ǂݎ���p)�����̉��w�ɂ��郁�b�V���̑���
	int						TotalChildNum ;					// (�ǂݎ���p)�����̉��w�ɂ���t���[���̐�

	int						Visible ;						// �\���A��\���ݒ�( 0:��\��  1:�\�� )

	DWORD					UserID ;						// ���[�U�[���ݒ�ł���h�c
	void					*UserData ;						// ���[�U�[�f�[�^

	VECTOR					TempVector ;					// �Z�b�g�A�b�v��ƂŎg���Ă����ėp�x�N�g��
	MATRIX					LocalWorldMatrix ;				// �Z�b�g�A�b�v��ƂŎg���Ă������[�J�������[���h�ϊ��s��
	MATRIX					Matrix ;						// �ϊ��s��
	VECTOR					Translate ;						// ���s�ړ�
	VECTOR					Scale ;							// �X�P�[��
	VECTOR					Rotate ;						// ��]
	int						RotateOrder ;					// ��]�I�[�_�[( MV1_ROTATE_ORDER_XYZ�� )
	FLOAT4					Quaternion ;					// ��]( �N�H�[�^�j�I�� )
	int						IgnoreParentTransform ;			// �e�̍��W�ϊ��𖳎����邩�ǂ���( 1:�������� 0:�������Ȃ� )

	int						EnablePreRotate ;				// �O��]���L����( 1:�L��  0:���� )
	int						EnablePostRotate ;				// ���]���L����( 1:�L��  0:���� )
	VECTOR					PreRotate ;						// �O��]
	VECTOR					PostRotate ;					// ���]

	MV1_LIGHT_R				*Light ;						// ���C�g�̏��( ���C�g�ł͖����ꍇ�� NULL )

	DWORD					MaxBoneUseNum ;					// (�ǂݎ���p)��̒��_�f�[�^���u�����h���Ă���{�[���̍ő吔
	float					SmoothingAngle ;				// �@�����X���[�W���O����ʂƖʂ̍ő�p�x��( �P�ʂ̓��W�A�� )

	int						MeshNum ;						// (�ǂݎ���p)���b�V���̐�
	MV1_MESH_R				*MeshFirst ;					// (�ǂݎ���p)�擪�̃��b�V���ւ̃|�C���^
	MV1_MESH_R				*MeshLast ;						// (�ǂݎ���p)���[�̃��b�V���ւ̃|�C���^

	int						ShapeNum ;						// (�ǂݎ���p)�V�F�C�v�f�[�^�̐�
	MV1_SHAPE_R				*ShapeFirst ;					// (�ǂݎ���p)�擪�̃V�F�C�v�f�[�^�ւ̃|�C���^
	MV1_SHAPE_R				*ShapeLast ;					// (�ǂݎ���p)���[�̃V�F�C�v�f�[�^�ւ̃|�C���^

	MV1_FRAME_R				*Parent ;						// (�ǂݎ���p)�e�m�[�h
	MV1_FRAME_R				*ChildFirst ;					// (�ǂݎ���p)���j�m�[�h
	MV1_FRAME_R				*ChildLast ;					// (�ǂݎ���p)�����q�m�[�h
	MV1_FRAME_R				*Next ;							// (�ǂݎ���p)��m�[�h
	MV1_FRAME_R				*Prev ;							// (�ǂݎ���p)�Z�m�[�h

	MV1_FRAME_BASE			*MV1Frame ;						// (�ǂݎ���p)���̃t���[���Ɋ֘A�t����ꂽ MV1_FRAME_BASE �\���̂ւ̃|�C���^
} ;

// �f�[�^�\����
struct MV1_MODEL_R
{
	MEMINFO					*Mem ;							// (�ǂݎ���p)�������A�h���X�ۑ��f�[�^�ւ̃|�C���^

	int						CharCodeFormat ;					// char�ł̃I�u�W�F�N�g�ǉ��֐����Ă΂ꂽ�ۂɑz�肷�镶�������R�[�h�`��
	wchar_t					*FilePath ;						// �t�@�C���p�X�ւ̃|�C���^( ���̕������ MV1RAddString ���g�p���Ă͂����Ȃ� )
	wchar_t					*Name ;							// ���O�ւ̃|�C���^( ���̕������ MV1RAddString ���g�p���Ă͂����Ȃ� )
	int						MeshFaceRightHand ;				// �ʏ�񂪉E��n�̕��тɂȂ��Ă��邩�ǂ���( TRUE:�Ȃ��Ă���  FALSE:�Ȃ��Ă��Ȃ� )
	int						AutoCreateNormal ;				// �@���̎����������g�p���邩�ǂ���( TRUE:�g�p����  FALSE:�g�p���Ȃ� )
	DWORD					AnimKeyDataSize ;				// �A�j���[�V�����L�[�f�[�^�̃T�C�Y
	int						TranslateIsBackCulling ;		// �������������o�b�N�J�����O�����邩�ǂ���( 1:����  0:���Ȃ� )
	int						AnimDataNotDecomposition ;		// �A�j���f�[�^�̕������s��Ȃ����ǂ���( 1:�s��Ȃ�  0:�s�� )
	int						MaterialNumberOrderDraw ;		// ���蓖�Ă��Ă���}�e���A���̔ԍ����Ⴂ���̂���`�悷�邩�ǂ���( 1:����  0:���Ȃ� )

	int						TotalMV1MeshNum ;				// (�ǂݎ���p)MV1_MESH_BASE ���Z�̃��b�V���̐�
	int						ChangeDrawMaterialTableSize ;	// (�ǂݎ���p)�`��}�e���A���̕ύX���Ǘ��p�ɕK�v�ȃf�[�^�̑��e��
	int						ChangeMatrixTableSize ;			// (�ǂݎ���p)�s��̕ύX���Ǘ��p�ɕK�v�ȃf�[�^�̑��e��

	DWORD					FrameNum ;						// (�ǂݎ���p)�m�[�h�̐�
	MV1_FRAME_R				*FrameFirst ;					// (�ǂݎ���p)�f�[�^�I�ɐ擪�̃m�[�h
	MV1_FRAME_R				*FrameLast ;					// (�ǂݎ���p)�f�[�^�I�ɖ��[�̃m�[�h

	DWORD					MaterialNum ;					// (�ǂݎ���p)�}�e���A���̐�
	MV1_MATERIAL_R			*MaterialFirst ;				// (�ǂݎ���p)�f�[�^�I�ɐ擪�̃}�e���A��
	MV1_MATERIAL_R			*MaterialLast ;					// (�ǂݎ���p)�f�[�^�I�ɖ��[�̃}�e���A��

	DWORD					TextureNum ;					// (�ǂݎ���p)�e�N�X�`���̐�
	MV1_TEXTURE_R			*TextureFirst ;					// (�ǂݎ���p)�f�[�^�I�ɐ擪�̃e�N�X�`��
	MV1_TEXTURE_R			*TextureLast ;					// (�ǂݎ���p)�f�[�^�I�ɖ��[�̃e�N�X�`��

	DWORD					MeshNum ;						// (�ǂݎ���p)���b�V���̐�
	MV1_MESH_R				*MeshFirst ;					// (�ǂݎ���p)�f�[�^�I�ɐ擪�̃��b�V��
	MV1_MESH_R				*MeshLast ;						// (�ǂݎ���p)�f�[�^�I�ɖ��[�̃��b�V��

	DWORD					ShapeNum ;						// (�ǂݎ���p)�V�F�C�v�̐�
	MV1_SHAPE_R				*ShapeFirst ;					// (�ǂݎ���p)�f�[�^�I�ɐ擪�̃V�F�C�v
	MV1_SHAPE_R				*ShapeLast ;					// (�ǂݎ���p)�f�[�^�I�ɖ��[�̃V�F�C�v

	DWORD					PhysicsRigidBodyNum ;			// (�ǂݎ���p)�������Z�p���̂̐�
	MV1_PHYSICS_RIGIDBODY_R	*PhysicsRigidBodyFirst ;		// (�ǂݎ���p)�f�[�^�I�ɐ擪�̕������Z�p����
	MV1_PHYSICS_RIGIDBODY_R	*PhysicsRigidBodyLast ;			// (�ǂݎ���p)�f�[�^�I�ɖ��[�̕������Z�p����

	DWORD					PhysicsJointNum ;				// (�ǂݎ���p)�������Z�p���̃W���C���g�̐�
	MV1_PHYSICS_JOINT_R		*PhysicsJointFirst ;			// (�ǂݎ���p)�f�[�^�I�ɐ擪�̕������Z�p���̃W���C���g
	MV1_PHYSICS_JOINT_R		*PhysicsJointLast ;				// (�ǂݎ���p)�f�[�^�I�ɖ��[�̕������Z�p���̃W���C���g

	DWORD					LightNum ;						// (�ǂݎ���p)���C�g�̐�
	MV1_LIGHT_R				*LightFirst ;					// (�ǂݎ���p)�f�[�^�I�ɐ擪�̃��C�g
	MV1_LIGHT_R				*LightLast ;					// (�ǂݎ���p)�f�[�^�I�ɖ��[�̃��C�g

	DWORD					AnimSetNum ;					// (�ǂݎ���p)�A�j���[�V�����Z�b�g�̐�
	MV1_ANIMSET_R			*AnimSetFirst ;					// (�ǂݎ���p)�f�[�^�I�ɐ擪�̃A�j���[�V�����Z�b�g
	MV1_ANIMSET_R			*AnimSetLast ;					// (�ǂݎ���p)�f�[�^�I�ɖ��[�̃A�j���[�V�����Z�b�g

	DWORD					AnimNum ;						// (�ǂݎ���p)�A�j���[�V�����̐�
	MV1_ANIM_R				*AnimFirst ;					// (�ǂݎ���p)�f�[�^�I�ɐ擪�̃A�j���[�V����
	MV1_ANIM_R				*AnimLast ;						// (�ǂݎ���p)�f�[�^�I�ɖ��[�̃A�j���[�V����

	DWORD					AnimKeySetNum ;					// (�ǂݎ���p)�A�j���[�V�����L�[�Z�b�g�̐�
	MV1_ANIMKEYSET_R		*AnimKeySetFirst ;				// (�ǂݎ���p)�f�[�^�I�ɐ擪�̃A�j���[�V�����L�[�Z�b�g
	MV1_ANIMKEYSET_R		*AnimKeySetLast ;				// (�ǂݎ���p)�f�[�^�I�ɖ��[�̃A�j���[�V�����L�[�Z�b�g

	int						ShapeNormalPositionNum ;		// (�ǂݎ���p)�V�F�C�v�p�{�[����񖳂����W�f�[�^�̐�
	int						ShapeSkinPosition4BNum ;		// (�ǂݎ���p)�V�F�C�v�p�S�{�[���X�L�j���O���b�V�����W�f�[�^�̐�
	int						ShapeSkinPosition8BNum ;		// (�ǂݎ���p)�V�F�C�v�p�W�{�[���X�L�j���O���b�V�����W�f�[�^�̐�
	int						ShapeSkinPositionFREEBSize ;	// (�ǂݎ���p)�V�F�C�v�p�X�{�[���ȏ�X�L�j���O���b�V�����W�f�[�^�̃T�C�Y

	DWORD					MeshMaterialNum ;				// (�ǂݎ���p)���b�V���Ŏg�p����Ă���}�e���A���̐�
	DWORD					TriangleNum ;					// (�ǂݎ���p)�R�p�`�|���S���̐�
	DWORD					TriangleListNum	;				// (�ǂݎ���p)�g���C�A���O�����X�g�̐�
	DWORD					IndexNum ;						// (�ǂݎ���p)���_�C���f�b�N�X�̐�
	int						NormalPositionNum ;				// (�ǂݎ���p)�{�[����񖳂����W�f�[�^�̐�
	int						SkinPosition4BNum ;				// (�ǂݎ���p)�S�{�[���X�L�j���O���b�V�����W�f�[�^�̐�
	int						SkinPosition8BNum ;				// (�ǂݎ���p)�W�{�[���X�L�j���O���b�V�����W�f�[�^�̐�
	int						SkinPositionFREEBSize ;			// (�ǂݎ���p)�X�{�[���ȏ�X�L�j���O���b�V�����W�f�[�^�̃T�C�Y
	int						MeshPositionSize ;				// (�ǂݎ���p)���b�V�����W�̃f�[�^�T�C�Y
	int						MeshNormalNum ;					// (�ǂݎ���p)���b�V���@���̐�
	int						MeshVertexSize ;				// (�ǂݎ���p)���b�V�����_���̃f�[�^�T�C�Y
	int						MeshFaceNum ;					// (�ǂݎ���p)���b�V���ʏ��̐�
	DWORD					MeshVertexIndexNum ;			// (�ǂݎ���p)���b�V�����_�f�[�^�C���f�b�N�X�̑���
	DWORD					StringSizeA ;					// (�ǂݎ���p)������f�[�^�̃T�C�Y
	DWORD					StringSizeW ;					// (�ǂݎ���p)������f�[�^�̃T�C�Y
	DWORD					ShapeMeshNum ;					// (�ǂݎ���p)�V�F�C�v���b�V���̐�
	DWORD					ShapeVertexNum ;				// (�ǂݎ���p)�V�F�C�v���_�f�[�^�̐�

	DWORD					SkinWeightNum ;					// (�ǂݎ���p)�X�L�����b�V���̃E�G�C�g�Ɋւ�����̐�
} ;

// �֐��錾 -------------------------------------

extern	int					MV1InitReadModel( MV1_MODEL_R *ReadModel ) ;							// �ǂݍ��ݏ����p���f���\���̂̏�����
extern	int					MV1TermReadModel( MV1_MODEL_R *ReadModel ) ;							// �ǂݍ��ݏ����p���f���\���̂̌�n��

#ifndef UNICODE
extern	char				*MV1RAddString(					MV1_MODEL_R *ReadModel, const char    *String ) ;							// ������̒ǉ�
#endif
extern	wchar_t				*MV1RAddStringW(				MV1_MODEL_R *ReadModel, const wchar_t *String ) ;							// ������̒ǉ�
#ifndef UNICODE
extern	char				*MV1RAddStringWToA(				MV1_MODEL_R *ReadModel, const wchar_t *String ) ;							// ������̒ǉ�
#endif
extern	wchar_t				*MV1RAddStringAToW(				MV1_MODEL_R *ReadModel, const char    *String ) ;							// ������̒ǉ�
extern	MV1_FRAME_R			*MV1RAddFrame(					MV1_MODEL_R *ReadModel, const char    *Name, MV1_FRAME_R *Parent ) ;		// �t���[���̒ǉ�
extern	MV1_FRAME_R			*MV1RAddFrameW(					MV1_MODEL_R *ReadModel, const wchar_t *Name, MV1_FRAME_R *Parent ) ;		// �t���[���̒ǉ�
extern	MV1_MESH_R			*MV1RAddMesh(					MV1_MODEL_R *ReadModel, MV1_FRAME_R *Frame ) ;								// ���b�V���̒ǉ�
extern	void				MV1RSubMesh(					MV1_MODEL_R *ReadModel, MV1_FRAME_R *Frame, MV1_MESH_R *Mesh ) ;			// ���b�V���̍폜
extern	int					MV1RSetupMeshFaceBuffer(		MV1_MODEL_R *ReadModel, MV1_MESH_R *Mesh, int FaceNum, int MaxIndexNum ) ;	// ���b�V���̖ʗp�̃o�b�t�@���Z�b�g�A�b�v����
extern	MV1_SHAPE_R			*MV1RAddShape(					MV1_MODEL_R *ReadModel, const char    *Name, MV1_FRAME_R *Frame ) ;			// �V�F�C�v�f�[�^�̒ǉ�
extern	MV1_SHAPE_R			*MV1RAddShapeW(					MV1_MODEL_R *ReadModel, const wchar_t *Name, MV1_FRAME_R *Frame ) ;			// �V�F�C�v�f�[�^�̒ǉ�
extern	MV1_PHYSICS_RIGIDBODY_R *MV1RAddPhysicsRididBody(	MV1_MODEL_R *ReadModel, const char    *Name, MV1_FRAME_R *TargetFrame ) ;	// �������Z�p���̃f�[�^�̒ǉ�
extern	MV1_PHYSICS_RIGIDBODY_R *MV1RAddPhysicsRididBodyW(	MV1_MODEL_R *ReadModel, const wchar_t *Name, MV1_FRAME_R *TargetFrame ) ;	// �������Z�p���̃f�[�^�̒ǉ�
extern	MV1_PHYSICS_JOINT_R *MV1RAddPhysicsJoint(			MV1_MODEL_R *ReadModel, const char    *Name ) ;								// �������Z�p���̃W���C���g�f�[�^�̒ǉ�
extern	MV1_PHYSICS_JOINT_R *MV1RAddPhysicsJointW(			MV1_MODEL_R *ReadModel, const wchar_t *Name ) ;								// �������Z�p���̃W���C���g�f�[�^�̒ǉ�
extern	MV1_MATERIAL_R		*MV1RAddMaterial(				MV1_MODEL_R *ReadModel, const char    *Name ) ;								// �}�e���A���̒ǉ�
extern	MV1_MATERIAL_R		*MV1RAddMaterialW(				MV1_MODEL_R *ReadModel, const wchar_t *Name ) ;								// �}�e���A���̒ǉ�
extern	MV1_MATERIAL_R		*MV1RGetMaterial(				MV1_MODEL_R *ReadModel, int Index ) ;										// �w��C���f�b�N�X�̃}�e���A�����擾����
extern	MV1_TEXTURE_R		*MV1RAddTexture(				MV1_MODEL_R *ReadModel, const char    *Name, const char    *ColorFilePath, const char    *AlphaFilePath = NULL, int BumpMapFlag = FALSE, float BumpMapNextPixelLength = 0.1f, bool FilePathDoubleCancel = true, bool ReverseFlag = false, bool Bmp32AllZeroAlphaToXRGB8Flag = true ) ;	// �e�N�X�`���̒ǉ�
extern	MV1_TEXTURE_R		*MV1RAddTextureW(				MV1_MODEL_R *ReadModel, const wchar_t *Name, const wchar_t *ColorFilePath, const wchar_t *AlphaFilePath = NULL, int BumpMapFlag = FALSE, float BumpMapNextPixelLength = 0.1f, bool FilePathDoubleCancel = true, bool ReverseFlag = false, bool Bmp32AllZeroAlphaToXRGB8Flag = true ) ;	// �e�N�X�`���̒ǉ�
extern	MV1_SKIN_WEIGHT_R	*MV1RAddSkinWeight( 			MV1_MODEL_R *ReadModel ) ;													// �X�L�����̒ǉ�
extern	MV1_ANIMSET_R		*MV1RAddAnimSet(				MV1_MODEL_R *ReadModel, const char    *Name ) ;								// �A�j���[�V�����Z�b�g�̒ǉ�
extern	MV1_ANIMSET_R		*MV1RAddAnimSetW(				MV1_MODEL_R *ReadModel, const wchar_t *Name ) ;								// �A�j���[�V�����Z�b�g�̒ǉ�
extern	MV1_ANIM_R			*MV1RAddAnim(					MV1_MODEL_R *ReadModel, MV1_ANIMSET_R *AnimSet ) ;							// �A�j���[�V�����̒ǉ�
extern	MV1_ANIMKEYSET_R	*MV1RAddAnimKeySet(				MV1_MODEL_R *ReadModel, MV1_ANIM_R *Anim ) ;								// �A�j���[�V�����L�[�Z�b�g�̒ǉ�
extern	MV1_LIGHT_R			*MV1RAddLight(					MV1_MODEL_R *ReadModel, const char    *Name ) ;								// ���C�g�̒ǉ�
extern	MV1_LIGHT_R			*MV1RAddLightW(					MV1_MODEL_R *ReadModel, const wchar_t *Name ) ;								// ���C�g�̒ǉ�
extern	int					MV1GetFrameIndex(				MV1_MODEL_R *ReadModel, DWORD UserID ) ;									// �w��̃t���[���h�c�����m�[�h�̃C���f�b�N�X�𓾂�
extern	int					MV1RLoadFile(  const char    *FilePath, void **FileImage, int *FileSize ) ;									// �t�@�C�����ۂ��Ɠǂݍ���( -1:�ǂݍ��ݎ��s )
extern	int					MV1RLoadFileW( const wchar_t *FilePath, void **FileImage, int *FileSize ) ;									// �t�@�C�����ۂ��Ɠǂݍ���( -1:�ǂݍ��ݎ��s )
extern	int					MV1RMakeMatrix( VECTOR *PreRotate, VECTOR *Rotate, VECTOR *PostRotate, VECTOR *Scale, VECTOR *Translate, VECTOR *MatrixRXYZ, MATRIX *DestBuffer, int RotateOrder = MV1_ROTATE_ORDER_XYZ ) ;	// ���W�ϊ��s��̍쐬

extern	int					MV1LoadModelToReadModel( const MV1LOADMODEL_GPARAM *GParam, MV1_MODEL_R *ReadModel, const wchar_t *CurrentDir, const MV1_FILE_READ_FUNC *ReadFunc, int ASyncThread ) ;	// �ǂݍ��ݏ����p���f�������{���f���f�[�^���쐬����

#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

#endif	// DX_NON_MODEL

#endif

