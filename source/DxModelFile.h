// -------------------------------------------------------------------------------
// 
// 		�c�w���C�u����		���f���f�[�^�t�@�C��
// 
// 				Ver 3.16d
// 
// -------------------------------------------------------------------------------

#ifndef __DXMODELFILE_H__
#define __DXMODELFILE_H__

// �C���N���[�h ------------------------------

#include "DxCompileConfig.h"
#include "DxLib.h"
#include "DxStatic.h"

#ifndef DX_NON_MODEL

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// �}�N����` --------------------------------

#define MV1_TRIANGLE_LIST_INDEX_TYPE_U8						(0x0000)				// �C���f�b�N�X�l�� unsigned char �^
#define MV1_TRIANGLE_LIST_INDEX_TYPE_U16					(0x0001)				// �C���f�b�N�X�l�� unsigned short �^
#define MV1_TRIANGLE_LIST_INDEX_TYPE_U32					(0x0002)				// �C���f�b�N�X�l�� unsigned dword �^

#define MV1_TRIANGLE_LIST_FLAG_MVERT_INDEX_MASK				(0x0003)				// ���b�V�����_�C���f�b�N�X�^�C�v�̃}�X�N( MV1_TRIANGLE_LIST_INDEX_TYPE_U32 �� )
#define MV1_TRIANGLE_LIST_FLAG_INDEX_MASK					(0x000c)				// ���_�C���f�b�N�X�^�C�v�̃}�X�N( MV1_TRIANGLE_LIST_INDEX_TYPE_U32 �� )

#define MV1_FRAME_NORMAL_TYPE_NONE							(0x0000)				// �@���͖���( �����v�Z���� )
#define MV1_FRAME_NORMAL_TYPE_S8							(0x0001)				// �@���� signed char �^
#define MV1_FRAME_NORMAL_TYPE_S16							(0x0002)				// �@���� signed short �^
#define MV1_FRAME_NORMAL_TYPE_F32							(0x0003)				// �@���� float �^

#define MV1_FRAME_MATRIX_INDEX_TYPE_U8						(0x0000)				// �s��C���f�b�N�X�� unsigned char �^
#define MV1_FRAME_MATRIX_INDEX_TYPE_U16						(0x0001)				// �s��C���f�b�N�X�� unsigned short �^

#define MV1_FRAME_MATRIX_WEIGHT_TYPE_U8						(0x0000)				// �s��E�G�C�g�l�� unsigned char �^
#define MV1_FRAME_MATRIX_WEIGHT_TYPE_U16					(0x0001)				// �s��E�G�C�g�l�� unsigned short �^

#define MV1_FRAME_VERT_FLAG_NORMAL_TYPE_MASK				(0x0003)				// �@���^�C�v�}�X�N( MV1_FRAME_NORMAL_TYPE_NONE �� )
#define MV1_FRAME_VERT_FLAG_POSITION_B16					(0x0004)				// ���̃t���O�������Ă�������W�l��16�r�b�g
#define MV1_FRAME_VERT_FLAG_MATRIX_WEIGHT_NONE				(0x0008)				// ���̃t���O�������Ă�����E�G�C�g�l�͖���
#define MV1_FRAME_VERT_FLAG_MATRIX_INDEX_MASK				(0x0010)				// �s��C���f�b�N�X�^�C�v�}�X�N( MV1_FRAME_MATRIX_INDEX_TYPE_U8 �� )
#define MV1_FRAME_VERT_FLAG_MATRIX_WEIGHT_MASK				(0x0020)				// �s��E�G�C�g�^�C�v�}�X�N( MV1_FRAME_MATRIX_WEIGHT_TYPE_U8 �� )
#define MV1_FRAME_VERT_FLAG_NOMRAL_TANGENT_BINORMAL			(0x0040)				// ���̃t���O�������Ă�����@�����́A�@���A�ڐ��A�]�@��������


#define MV1_MESH_VERT_INDEX_TYPE_NONE						(0x0000)				// �C���f�b�N�X�l�͖���
#define MV1_MESH_VERT_INDEX_TYPE_U8							(0x0001)				// �C���f�b�N�X�l�� unsigned char �^
#define MV1_MESH_VERT_INDEX_TYPE_U16						(0x0002)				// �C���f�b�N�X�l�� unsigned short �^
#define MV1_MESH_VERT_INDEX_TYPE_U32						(0x0003)				// �C���f�b�N�X�l�� unsigned int �^

#define MV1_MESH_VERT_FLAG_POS_IND_TYPE_MASK				(0x0003)				// ���W�C���f�b�N�X�^�C�v�}�X�N( MV1_MESH_VERT_INDEX_TYPE_U8 �� )
#define MV1_MESH_VERT_FLAG_NRM_IND_TYPE_MASK				(0x000c)				// �@���C���f�b�N�X�^�C�v�}�X�N( MV1_MESH_VERT_INDEX_TYPE_U8 �� )
#define MV1_MESH_VERT_FLAG_UV_U16							(0x0010)				// ���̃t���O�������Ă�����t�u�l��16�r�b�g
#define MV1_MESH_VERT_FLAG_COMMON_COLOR						(0x0020)				// ���̃t���O�������Ă����璸�_�J���[�͋��ʂň��
#define MV1_MESH_VERT_FLAG_NON_TOON_OUTLINE					(0x0040)				// ���̃t���O�������Ă�����e���_�Ƀg�D�[���̗֊s����`�悷�邩�ǂ����̏�񂪊i�[����Ă���


#define MV1_TEXTURE_FLAG_REVERSE							(0x0001)				// ���̃t���O�������Ă�����摜�𔽓]����
#define MV1_TEXTURE_FLAG_BMP32_ALL_ZERO_ALPHA_TO_XRGB8		(0x0002)				// ���̃t���O�������Ă�����R�Q�r�b�g�a�l�o�̃A���t�@�l�����ׂĂO�̂Ƃ��� XRGB8 �Ƃ��Ĉ���
#define MV1_TEXTURE_FLAG_VALID_SCALE_UV						(0x0004)				// ���̃t���O�������Ă�����t�u���W�̃X�P�[�����L��


#define MV1_ANIM_KEYSET_FLAG_KEY_ONE						(0x0001)				// ���̃t���O�������Ă�����L�[�̐��͈��
#define MV1_ANIM_KEYSET_FLAG_KEYNUM_B						(0x0002)				// ���̃t���O�������Ă�����L�[�̐��͂P�o�C�g
#define MV1_ANIM_KEYSET_FLAG_KEYNUM_W						(0x0004)				// ���̃t���O�������Ă�����L�[�̐��͂Q�o�C�g
#define MV1_ANIM_KEYSET_FLAG_TIME_UNIT						(0x0008)				// ���̃t���O�������Ă�����^�C���z��̑���ɓ��Ԋu�^�C����񂪓����Ă���
#define MV1_ANIM_KEYSET_FLAG_TIME_UNIT_ST_W					(0x0010)				// ���̃t���O�������Ă�����^�C���J�n�l��WORD�l
#define MV1_ANIM_KEYSET_FLAG_TIME_UNIT_ST_Z					(0x0020)				// ���̃t���O�������Ă�����^�C���J�n�l�͂O
#define MV1_ANIM_KEYSET_FLAG_TIME_UNIT_UN_W					(0x0040)				// ���̃t���O�������Ă�����^�C���Ԋu�l��WORD�l
#define MV1_ANIM_KEYSET_FLAG_TIME_BIT16						(0x0080)				// ���̃t���O�������Ă�����^�C���z��̒l���P�U�r�b�g�l
#define MV1_ANIM_KEYSET_FLAG_KEY_BIT16						(0x0100)				// ���̃t���O�������Ă�����L�[�͂P�U�r�b�g�l
#define MV1_ANIM_KEYSET_FLAG_KEY_MP_PP						(0x0200)				// ���̃t���O�������Ă�����L�[�� -PI �` PI �� 0�`65535 �ŕ\��������
#define MV1_ANIM_KEYSET_FLAG_KEY_Z_TP						(0x0400)				// ���̃t���O�������Ă�����L�[�� 0.0f �` 2 * PI �� 0�`65535 �ŕ\��������

// �\���̒�` --------------------------------

// �v���g�^�C�v�錾
struct MV1_MESH_SET_F1 ;
struct MV1_FRAME_F1 ;
struct MV1_MESH_F1 ;
struct MV1_ANIMSET_F1 ;

// ��]���\����
struct MV1_ROTATE_F1
{
	int										Type ;								// ��]�^�C�v( MV1_ROTATE_TYPE_XYZROT �� )
	union
	{
		VECTOR								XYZRot ;							// XYZ��]�p
		FLOAT4								Qt ;								// �N�H�[�^�j�I����]�p
		MATRIX_4X4CT_F						Mat ;								// �s���]�p
	} ;
} ;

// 16bit�f�[�^�^�C�v�p�⏕���\����
struct MV1_ANIM_KEY_16BIT_F1
{
	BYTE									Min ;								// �ŏ��l( bit7:�O���ǂ���( 0:0�ȊO 1:0 )  bit6:����(0:+ 1:-)  bit5:�搔����(0:+ 1:-) bit4�`0:�搔(�ő�10��15��) ) 
	BYTE									Unit ;								// 16bit�l�P�ӂ�̒l( bit7:�搔����(0:+ 1:-) bit6�`4:�搔(�ő�10��7��) bit3�`0:��Z�����l( 0�`15 ) )
} ;

// �A�j���[�V�����L�[(�S�~�S�s��̂S��ڂ�(0,0,0,1)�Œ�̍s��^�C�v)�f�[�^�\����
struct MV1_ANIM_KEY_MATRIX4X4C_F1
{
	float									Matrix[ 4 ][ 3 ] ;					// �s��
} ;

// �A�j���[�V�����L�[(�R�~�R�s��^�C�v)�f�[�^�\����
struct MV1_ANIM_KEY_MATRIX3X3_F1
{
	float									Matrix[ 3 ][ 3 ] ;					// �s��
} ;

// �A�j���[�V�����L�[(�R�~�R�s��P�U�r�b�g�^�C�v)�f�[�^�\����
struct MV1_ANIM_KEY_MATRIX3X3_B16_F1
{
	WORD									Matrix[ 3 ][ 3 ] ;					// �s��
} ;

// �A�j���[�V�����L�[���X�g��f�[�^�\����
/*
	KeyData �̏��i�[����

	��{���

		Type �� MV1_ANIMKEY_DATATYPE_SHAPE �̏ꍇ
			WORD								TargetShapeIndex ;		// �Ώۂ̃V�F�C�v�C���f�b�N�X
	
		Flag �� MV1_ANIM_KEYSET_FLAG_KEY_ONE ���t���Ă��Ȃ��ꍇ
			Flag �� MV1_ANIM_KEYSET_FLAG_KEYNUM_B ���t���Ă���ꍇ
				BYTE							Num ;					// �L�[�̐�

			Flag �� MV1_ANIM_KEYSET_FLAG_KEYNUM_W ���t���Ă���ꍇ
				WORD							Num ;					// �L�[�̐�

			Flag �� MV1_ANIM_KEYSET_FLAG_KEYNUM_W �� MV1_ANIM_KEYSET_FLAG_KEYNUM_B ���t���Ă��Ȃ��ꍇ
				DWORD							Num ;					// �L�[�̐�


			Flag �� MV1_ANIM_KEYSET_FLAG_TIME_BIT16 ���t���Ă���ꍇ
				MV1_ANIM_KEY_16BIT_F1			Time16BSub ;			// 16bit�^�C���l�p�⏕���
				WORD �^�C���l��

			Flag �� MV1_ANIM_KEYSET_FLAG_TIME_BIT16 �� MV1_ANIM_KEYSET_FLAG_TIME_UNIT �����Ă��Ȃ��ꍇ
				float �^�C���l��

			Flag �� MV1_ANIM_KEYSET_FLAG_TIME_UNIT ���t���Ă���ꍇ
				Flag �� MV1_ANIM_KEYSET_FLAG_TIME_UNIT_ST_W ���t���Ă���ꍇ
					WORD						StartTime ;				// �J�n����

				Flag �� MV1_ANIM_KEYSET_FLAG_TIME_UNIT_ST_Z �� MV1_ANIM_KEYSET_FLAG_TIME_UNIT_ST_W �����Ă��Ȃ��ꍇ
					float						StartTime ;				// �J�n����

				Flag �� MV1_ANIM_KEYSET_FLAG_TIME_UNIT_UN_W ���t���Ă���ꍇ
					WORD						UnitTime ;				// �P�ʎ���

				Flag �� MV1_ANIM_KEYSET_FLAG_TIME_UNIT_UN_W ���t���Ă��Ȃ��ꍇ
					float						UnitTime ;				// �P�ʎ���

		Flag �� MV1_ANIM_KEYSET_FLAG_VAL_MP_PP �� MV1_ANIM_KEYSET_FLAG_VAL_Z_TP ���t���ĂȂ��āA���� MV1_ANIM_KEYSET_FLAG_KEY_BIT16 ���t���Ă���ꍇ
			MV1_ANIM_KEY_16BIT_F1				Key16BSub ;				// 16bit�L�[�l�p�⏕���


	�L�[���
		�e�t�H�[�}�b�g�ɉ������f�[�^

*/
struct MV1_ANIM_KEYSET_F1
{
	char									Type ;								// �L�[�^�C�v( MV1_ANIMKEY_TYPE_QUATERNION �� )
	char									DataType ;							// �f�[�^�^�C�v( MV1_ANIMKEY_DATATYPE_ROTATE �� )
	WORD									Flag ;								// �t���O
	DWORD/*void* */							KeyData ;							// �L�[�f�[�^

	DWORD									UserData[ 1 ] ;						// �O����`�̏��

	DWORD									Padding[ 2 ] ;
} ;

// �A�j���[�V�����f�[�^�\����
struct MV1_ANIM_F1
{
	int										Index ;								// �C���f�b�N�X

	DWORD/*MV1_ANIMSET_F1* */				Container ;							// ���̃A�j���[�V�����������Ă���A�j���[�V�����Z�b�g�ւ̃|�C���^

	int										TargetFrameIndex ;					// �ΏۂƂȂ�t���[���̃C���f�b�N�X
	float									MaxTime ;							// �e�L�[�Z�b�g�̒��ň�Ԓ����L�[�Z�b�g�̎��Ԓl
	int										RotateOrder ;						// ��]�I�[�_�[( MV1_ROTATE_ORDER_XYZ �� )

	int										KeySetNum ;							// �L�[�Z�b�g�̐�
	DWORD/*MV1_ANIM_KEYSET_F1* */			KeySet ;							// �L�[�Z�b�g�z��ւ̃|�C���^

	DWORD									UserData[ 2 ] ;						// �O����`�̏��

	DWORD									Padding[ 2 ] ;
} ;

// �A�j���[�V�����Z�b�g�\����
struct MV1_ANIMSET_F1
{
	DWORD/*MV1_ANIMSET_F1* */				DimPrev ;							// �O�̃f�[�^�ւ̃|�C���^
	DWORD/*MV1_ANIMSET_F1* */				DimNext ;							// ���̃f�[�^�ւ̃|�C���^
	DWORD/*char* */							Name ;								// �A�j���[�V�����Z�b�g��

	int										Index ;								// �C���f�b�N�X
	float									MaxTime ;							// �e�A�j���[�V�����̒��ň�Ԓ����A�j���[�V�����̎��Ԓl
	int										AnimNum ;							// �A�j���[�V�����̐�
	DWORD/*MV1_ANIM_F1* */					Anim ;								// �A�j���[�V�������X�g

	DWORD									UserData[ 4 ] ;						// �O����`�̏��

	DWORD									Flag ;								// �t���O( 0bit:���Z�A�j���[�V������  1bit:�L�[�̕�Ԃ͍s����`��Ԃ�  2bit:���[�v�A�j���[�V������ )
	DWORD									Padding[ 3 ] ;
} ;

// �e�N�X�`���\����
struct MV1_TEXTURE_F1
{
	DWORD/*MV1_TEXTURE_F1* */				DimPrev ;							// �O�̃f�[�^�ւ̃|�C���^
	DWORD/*MV1_TEXTURE_F1* */				DimNext ;							// ���̃f�[�^�ւ̃|�C���^
	DWORD/*char* */							Name ;								// �e�N�X�`���I�u�W�F�N�g�ɂ���ꂽ���O
	int										Index ;								// �C���f�b�N�X

	DWORD/*char* */							ColorFilePath ;						// �J���[�`�����l���p�摜�t�@�C���̑��΃p�X
	DWORD/*char* */							AlphaFilePath ;						// �A���t�@�`�����l���p�摜�t�@�C���̑��΃p�X
	int										BumpImageFlag ;						// �J���[�`�����l�����o���v�}�b�v���ǂ���( TRUE:�o���v�}�b�v  FALSE:�Ⴄ )
	float									BumpImageNextPixelLength ;			// �o���v�}�b�v�摜�̏ꍇ�ׂ̗̃s�N�Z���Ƃ̋���

	int										AddressModeU ;						// �A�h���X���[�h( MV1_TEXTURE_ADDRESS_MODE_WRAP �� )
	int										AddressModeV ;						// �A�h���X���[�h( MV1_TEXTURE_ADDRESS_MODE_WRAP �� )
	int										FilterMode ;						// �t�B���^�����O���[�h( MV1_TEXTURE_FILTER_MODE_POINT �� )

	DWORD									UserData[ 2 ] ;						// �O����`�̏��

	BYTE									Flag ;								// �t���O( MV1_TEXTURE_FLAG_REVERSE �� )
	BYTE									Padding1[ 3 ] ;

	float									ScaleU ;							// �t���W�̃X�P�[�����O�l( �t���O MV1_TEXTURE_FLAG_VALID_SCALE_UV �������Ă���ꍇ�̂ݗL�� )
	float									ScaleV ;							// �u���W�̃X�P�[�����O�l( �t���O MV1_TEXTURE_FLAG_VALID_SCALE_UV �������Ă���ꍇ�̂ݗL�� )
	DWORD									Padding[ 1 ] ;
} ;

// �}�e���A�����C���[�\����
struct MV1_MATERIAL_LAYER_F1
{
	int										Texture ;							// ���f���e�N�X�`���C���f�b�N�X( MV1_MODEL_F1.Textue �z��̃C���f�b�N�X )
	int										BlendType ;							// �u�����h�^�C�v( MV1_LAYERBLEND_TYPE_ADDITIVE �� )

	DWORD									Padding[ 4 ] ;
} ;

// �}�e���A���g�D�[�������_�����O�p���\����
struct MV1_MATERIAL_TOON_F1
{
	int										Type ;								// �}�e���A���^�C�v( DX_MATERIAL_TYPE_NORMAL �Ȃ� )

	int										DiffuseGradTexture ;				// ( �g�D�[�������_�����O�ł̂ݎg�p )�f�B�t���[�Y�J���[�O���f�[�V�����e�N�X�`���C���f�b�N�X�A�������Ă��郉�C�g�̃f�B�t���[�Y�J���[�łt�l�����܂���́A�����̏ꍇ�́|�P( MV1_MODEL_BASE.Textue �z��̃C���f�b�N�X )
	int										SpecularGradTexture ;				// ( �g�D�[�������_�����O�ł̂ݎg�p )�X�y�L�����J���[�O���f�[�V�����e�N�X�`���C���f�b�N�X�A�������Ă��郉�C�g�̃X�y�L�����J���[�łt�l�����܂���́A�����̏ꍇ�́|�P( MV1_MODEL_BASE.Textue �z��̃C���f�b�N�X )
	int										DiffuseGradBlendType ;				// ( �g�D�[�������_�����O�ł̂ݎg�p )�f�B�t���[�Y�O���f�[�V�����e�N�X�`���̃u�����h�^�C�v( DX_MATERIAL_BLENDTYPE_TRANSLUCENT �Ȃ� )
	int										SpecularGradBlendType ;				// ( �g�D�[�������_�����O�ł̂ݎg�p )�X�y�L�����O���f�[�V�����e�N�X�`���̃u�����h�^�C�v( DX_MATERIAL_BLENDTYPE_ADDITIVE �Ȃ� )
	float									OutLineWidth ;						// ( �g�D�[�������_�����O�ł̂ݎg�p )�֊s���̕�( 0.0f �` 1.0f )
	COLOR_F									OutLineColor ;						// ( �g�D�[�������_�����O�ł̂ݎg�p )�֊s���̐F
	float									OutLineDotWidth ;					// ( �g�D�[�������_�����O�ł̂ݎg�p )�֊s���̃h�b�g�P�ʂł̕�
	BYTE									EnableSphereMap ;					// ( �g�D�[�������_�����O�ł̂ݎg�p )�X�t�B�A�}�b�v�̏�񂪗L�����ǂ���( 1:�L��  0:���� )
	BYTE									SphereMapBlendType ;				// ( �g�D�[�������_�����O�ł̂ݎg�p )�X�t�B�A�}�b�v�e�N�X�`���̃u�����h�^�C�v( DX_MATERIAL_BLENDTYPE_ADDITIVE �Ȃ� )
	short									SphereMapTexture ;					// ( �g�D�[�������_�����O�ł̂ݎg�p )�X�t�B�A�}�b�v�e�N�X�`���C���f�b�N�X

	DWORD									Padding[ 2 ] ;
} ;

// �}�e���A���\����
struct MV1_MATERIAL_F1
{
	DWORD/*MV1_MATERIAL_F1* */				DimPrev ;							// �O�̃f�[�^�ւ̃|�C���^
	DWORD/*MV1_MATERIAL_F1* */				DimNext ;							// ���̃f�[�^�ւ̃|�C���^
	DWORD/*char* */							Name ;								// ���O
	int										Index ;								// �C���f�b�N�X

	COLOR_F									Diffuse ;							// �f�B�t���[�Y�F
	COLOR_F									Ambient ;							// �A���r�G���g�F
	COLOR_F									Specular ;							// �X�y�L�����[�F
	COLOR_F									Emissive ;							// �G�~�b�V�u�F
	float									Power ;								// �X�y�L�����n�C���C�g�̃p���[
	float									Alpha ;								// �A���t�@�l

	int										DiffuseLayerNum ;					// �f�B�t���[�Y�J���[�̃��C���[��
	MV1_MATERIAL_LAYER_F1					DiffuseLayer[ 8 ] ;					// �f�B�t���[�Y�J���[�̃��C���[���( �O���C���[�� BlendType �͖�������� )
	int										SpecularLayerNum ;					// �X�y�L�����}�b�v�̃��C���[��
	MV1_MATERIAL_LAYER_F1					SpecularLayer[ 8 ] ;				// �X�y�L�����}�b�v�̃��C���[���( �O���C���[�� BlendType �͖�������� )
	int										NormalLayerNum ;					// �@���}�b�v�̃��C���[��
	MV1_MATERIAL_LAYER_F1					NormalLayer[ 8 ] ;					// �@���}�b�v�̃��C���[���( �O���C���[�� BlendType �͖�������� )

	int										UseAlphaTest ;						// �A���t�@�e�X�g���g�p���邩�ǂ���
	int										AlphaFunc ;							// �A���t�@�e�X�g���[�h( ���̂Ƃ��떢�g�p )
	int										AlphaRef ;							// �A���t�@�e�X�g��臒l

	int										DrawBlendMode ;						// �o�͎��̃u�����h���[�h( DX_BLENDMODE_NOBLEND �� )
	int										DrawBlendParam ;					// �o�͎��̃u�����h�p�����[�^

	DWORD									UserData[ 4 ] ;						// �O����`�̏��

	DWORD/*MV1_MATERIAL_TOON_F1* */			ToonInfo ;						// �g�D�[�������_�����O�p�̏��A�����ꍇ�� NULL
	DWORD									Padding[ 3 ] ;
} ;

// ���C�g�\����
struct MV1_LIGHT_F1
{
	DWORD/*MV1_LIGHT_F1* */					DimPrev ;							// �O�̃f�[�^�ւ̃|�C���^
	DWORD/*MV1_LIGHT_F1* */					DimNext ;							// ���̃f�[�^�ւ̃|�C���^
	DWORD/*char* */							Name ;								// ���O
	int										Index ;								// �C���f�b�N�X

	int										FrameIndex ;						// ���C�g�������Ă���t���[���̃C���f�b�N�X
	int										Type ;								// ���C�g�^�C�v( MV1_LIGHT_TYPE_POINT �� )
	COLOR_F									Diffuse ;							// �f�B�t���[�Y�J���[
	COLOR_F									Specular ;							// �X�y�L�����J���[
	COLOR_F									Ambient ;							// �A���r�G���g�J���[
	float									Range ;								// �X�|�b�g���C�g�̗L������
	float									Falloff ;							// �t�H�[���I�t
	float									Attenuation0 ;						// ���C�g�p�����[�^�O
	float									Attenuation1 ;						// ���C�g�p�����[�^�P
	float									Attenuation2 ;						// ���C�g�p�����[�^�Q
	float									Theta ;								// �X�|�b�g���C�g�̓����R�[���̊p�x�͈�
	float									Phi ;								// �X�|�b�g���C�g�̊O���R�[���̊p�x�͈�

	DWORD									UserData[ 2 ] ;						// �O����`�̏��
} ;

// �P�U�r�b�g���W�l�p�⏕���
struct MV1_POSITION_16BIT_SUBINFO_F1
{
	float									Min ;								// �ŏ��l
	float									Width ;								// ��
} ;

// �g���C�A���O�����X�g�\����
/*
	MeshVertexIndexAndIndexData �̏��

		�g�p�{�[�����
			VertexType ��
				MV1_VERTEX_TYPE_NORMAL        �̏ꍇ�@�@�Ȃ�
				MV1_VERTEX_TYPE_SKIN_4BONE    �̏ꍇ�@�@�ŏ��̂Q�o�C�g�Ɏg�p�{�[���̐��A���̌�ɂP�{�[���ӂ�Q�o�C�g�Ŏg�p�{�[���C���f�b�N�X�l���g�p�{�[����������
				MV1_VERTEX_TYPE_SKIN_8BONE    �̏ꍇ�@�@�ŏ��̂Q�o�C�g�Ɏg�p�{�[���̐��A���̌�ɂP�{�[���ӂ�Q�o�C�g�Ŏg�p�{�[���C���f�b�N�X�l���g�p�{�[����������
				MV1_VERTEX_TYPE_SKIN_FREEBONE �̏ꍇ    �ő�g�p�{�[�������Q�o�C�g�Ŋi�[

		VertexNum �̐�����

			���b�V�����_�C���f�b�N�X
				Flag �� MV1_TRIANGLE_LIST_FLAG_MVERT_INDEX_MASK �ɉ������l


		IndexNum �̐�����
		
			���_�C���f�b�N�X
				Flag �� MV1_TRIANGLE_LIST_FLAG_INDEX_MASK �ɉ������l
*/
struct MV1_TRIANGLE_LIST_F1
{
	DWORD/*MV1_TRIANGLE_LIST_F1* */			DimPrev ;						// �O�̃f�[�^�ւ̃|�C���^
	DWORD/*MV1_TRIANGLE_LIST_F1* */			DimNext ;						// ���̃f�[�^�ւ̃|�C���^
	int										Index ;								// �C���f�b�N�X

	DWORD/*MV1_MESH_F1* */					Container ;							// ���̃g���C�A���O�����X�g�������Ă��郁�b�V���ւ̃|�C���^
	unsigned short							VertexType ;						// ���_�^�C�v( MV1_VERTEX_TYPE_NORMAL �Ȃ� )
	unsigned short							Flag ;								// �t���O( MV1_TRIANGLE_LIST_FLAG_NORMAL_S8 �� )
	unsigned short							VertexNum ;							// ���_�f�[�^�̐�
	unsigned short							IndexNum ;							// ���_�C���f�b�N�X�̐�
	DWORD/*void* */							MeshVertexIndexAndIndexData ;		// ���b�V�����_�C���f�b�N�X�f�[�^�ƒ��_�C���f�b�N�X�f�[�^

	DWORD									Padding[ 2 ] ;
} ;

// �X�L�����b�V���p�{�[�����g�p����t���[���̏��
struct MV1_SKIN_BONE_USE_FRAME_F1
{
	int										Index ;								// �{�[�����g�p���Ă���t���[���̃C���f�b�N�X
	int										MatrixIndex ;						// ���̃{�[�����Z�b�g����Ă���t���[�����C���f�b�N�X( MV1_FRAME_BASE.UseSkinBone �̃C���f�b�N�X )
} ;

// �X�L�����b�V���p�{�[�����
struct MV1_SKIN_BONE_F1
{
	DWORD/*MV1_SKIN_BONE_F1* */				DimPrev ;							// �O�̃f�[�^�ւ̃|�C���^
	DWORD/*MV1_SKIN_BONE_F1* */				DimNext ;							// ���̃f�[�^�ւ̃|�C���^
	int										Index ;								// �C���f�b�N�X

	int										BoneFrame ;							// �{�[���Ƃ��Ďg�p����t���[��
	MATRIX_4X4CT_F							ModelLocalMatrix ;					// ���f�����W����{�[���̃��[�J�����W�ɕϊ����邽�߂̍s��
	int										ModelLocalMatrixIsTranslateOnly ;	// ���f�����W����{�[���̃��[�J�����W�ɕϊ����邽�߂̍s�񂪕��s�ړ��݂̂��ǂ���( 1:���s�ړ��̂�  0:��]���܂� )
	int										UseFrameNum ;						// ���̃{�[�����g�p����t���[���̐�
	DWORD/*MV1_SKIN_BONE_USE_FRAME_F1* */	UseFrame ;							// ���̃{�[�����g�p����t���[���̏��

	DWORD									Padding[ 2 ] ;
} ;

// ��ԕύX�Ǘ��p��{���\����
struct MV1_CHANGE_F1
{
	DWORD									Target ;							// ��ԕω������������ۂɘ_���a����ׂ��Ώۂ̊J�n�������A�h���X�ւ̃I�t�Z�b�g( �S�o�C�g�P�� )
	DWORD/*DWORD* */						Fill ;								// ��ԕω������������ۂɘ_���a����t���O�f�[�^
	DWORD									CheckBit ;							// �����̃t���O�r�b�g���������r�b�g���
	DWORD									Size ;								// ��ԕω������������ۂɘ_���a����t���O�f�[�^�̃T�C�Y( DWORD �P�� )
} ;

// ���b�V���\����
/*
	VertexData �̏��i�[����

		�ŏ��Ɉ����

			VertFlag �� MV1_MESH_VERT_FLAG_COMMON_COLOR ���t���Ă���ꍇ
				COLOR_U8			DiffuseColor ;				// ���ʃf�B�t���[�Y�F
				COLOR_U8			SpecularColor ;				// ���ʃX�y�L�����F

		VertexNum �̐�����

			���W�C���f�b�N�X
				VertFlag �� MV1_MESH_VERT_FLAG_POS_IND_TYPE_MASK �ɉ������l

		VertexNum �̐�����

			�@���C���f�b�N�X
				VertFlag �� MV1_MESH_VERT_FLAG_NRM_IND_TYPE_MASK �ɉ������l

		VertexNum �̐�����

			���_�J���[
				VertFlag �� MV1_MESH_VERT_FLAG_COMMON_COLOR ���t���Ă���ꍇ�͖����A����ꍇ�� COLOR_U8 �^�ŁA�f�B�t���[�Y�F�A�X�y�L�����F�̏�

		VertexNum �̐�����

			�t�u�l
				U,V �̏��� UVUnitNum * UVSetUnitNum �̐�����
				VertFlag �� MV1_MESH_VERT_FLAG_UV_U16 ���t���Ă����� WORD�^( 65535 �� 1.0 �Ƃ���Œ菬���_�l )�A����ȊO�̏ꍇ�� float�^

		VertFlag �� MV1_MESH_VERT_FLAG_NON_TOON_OUTLINE ���t���Ă����ꍇ VertexNum �̐�����

			�g�D�[�������_�����O���ɗ֊s����`�悵�Ȃ����ǂ����̏�񂪂P���_�P�r�b�g�Ŋi�[�A�]�肪�W�r�b�g�����̏ꍇ�͂��̃o�C�g���p�f�B���O�Ŏg��
*/
struct MV1_MESH_F1
{
	DWORD/*MV1_MESH_F1* */					DimPrev ;							// �O�̃f�[�^�ւ̃|�C���^
	DWORD/*MV1_MESH_F1* */					DimNext ;							// ���̃f�[�^�ւ̃|�C���^

	int										Index ;								// �C���f�b�N�X

	DWORD/*MV1_FRAME_F1* */					Container ;							// ���̃��b�V���������Ă���t���[���ւ̃|�C���^
	DWORD/*MV1_MATERIAL_F1* */				Material ;							// �g�p����}�e���A���ւ̃|�C���^

	MV1_CHANGE_F1							ChangeInfo ;						// ��ԕω��Ǘ��p��{���

	int										UseVertexDiffuseColor ;				// ���_�̃f�B�t���[�Y�J���[���}�e���A���̃f�B�t���[�Y�J���[�̑���Ɏg�p���邩�ǂ���( TRUE:�g�p����  FALSE:�g�p���Ȃ� )
	int										UseVertexSpecularColor ;			// ���_�̃X�y�L�����J���[���}�e���A���̃X�y�L�����J���[�̑���Ɏg�p���邩�ǂ���( TRUE:�g�p����  FALSE:�g�p���Ȃ� )
	BYTE									NotOneDiffuseAlpha ;				// �f�B�t���[�Y�J���[�̃A���t�@�l�� 100% �ȊO�̂��̂����邩�ǂ���( 1:����  0:�Ȃ� )
	BYTE									Shape ;								// �V�F�C�v���b�V�����ǂ���( 1:�V�F�C�v���b�V��  0:�V�F�C�v���b�V���ł͂Ȃ� )
	BYTE									Padding2[ 2 ] ;

	int										TriangleListNum ;					// �g���C�A���O�����X�g�̐�
	DWORD/*MV1_TRIANGLE_LIST_F1* */			TriangleList ;						// �g���C�A���O�����X�g�̃��X�g�ւ̃|�C���^

	BYTE									Visible ;							// �\���t���O( 1:�\������  0:�\�����Ȃ� )
	BYTE									BackCulling ;						// �o�b�N�J�����O�����邩�ǂ���( 2:�E���J�����O  1:�����J�����O  0:���Ȃ� )

	BYTE									UVSetUnitNum ;						// ��̍��W�f�[�^�Ɋ܂܂��e�N�X�`�����W�Z�b�g�̐�
	BYTE									UVUnitNum ;							// ��̍��W�f�[�^�Ɋ܂܂��e�N�X�`�����W�̐�

	int										VertFlag ;							// ���_�f�[�^�Ɋւ���t���O( MV1_MESH_VERT_FLAG_POS_IND_TYPE_MASK �� )

	int										VertexNum ;							// ���_�̐�
	int										FaceNum ;							// �ʂ̐�
	DWORD/*void* */							VertexData ;						// ���_�f�[�^

	DWORD									UserData[ 4 ] ;						// �O����`�̏��

	DWORD									Padding[ 4 ] ;
} ;

// �V�F�C�v���_�f�[�^�\����
struct MV1_SHAPE_VERTEX_F1
{
	int										TargetMeshVertex ;					// �ΏۂƂȂ钸�_�ԍ�( MV1_MESH_F1.Vertex �ɑ΂���C���f�b�N�X )
	VECTOR									Position ;							// ���W( ���̍��W�ɑ΂��鍷�� )
	VECTOR									Normal ;							// �@��
} ;

// �V�F�C�v���b�V���\����
struct MV1_SHAPE_MESH_F1
{
	DWORD/*MV1_SHAPE_MESH_F1* */			DimPrev ;							// �O�̃f�[�^�ւ̃|�C���^
	DWORD/*MV1_SHAPE_MESH_F1* */			DimNext ;							// ���̃f�[�^�ւ̃|�C���^

	int										Index ;								// �C���f�b�N�X

	DWORD/*MV1_MESH_F1* */					TargetMesh ;						// �ΏۂƂȂ郁�b�V��

	WORD									IsVertexPress ;						// ���_�f�[�^�����k����Ă��邩�ǂ���( 1:���k����Ă���  0:����Ă��Ȃ� )
	WORD									VertexPressParam ;					// ���_�f�[�^�̈��k�p�����[�^
	DWORD									VertexNum ;							// ���_�f�[�^�̐�
	DWORD/*MV1_SHAPE_VERTEX_F1* */			Vertex ;							// ���_�f�[�^�z��ւ̃|�C���^

	DWORD									UserData[ 4 ] ;						// �O����`�̏��

	DWORD									Padding[ 4 ] ;
} ;

// �V�F�C�v�\����
struct MV1_SHAPE_F1
{
	DWORD/*MV1_SHAPE_F1* */					DimPrev ;							// �O�̃f�[�^�ւ̃|�C���^
	DWORD/*MV1_SHAPE_F1* */					DimNext ;							// ���̃f�[�^�ւ̃|�C���^

	DWORD/*char* */							Name ;								// ���O
	int										Index ;								// �C���f�b�N�X

	DWORD/*MV1_FRAME_F1* */					Container ;							// ���̃V�F�C�v�������Ă���t���[���̃|�C���^

	int										MeshNum ;							// �V�F�C�v���b�V���̐�
	DWORD/*MV1_SHAPE_MESH_F1* */			Mesh ;								// �V�F�C�v���b�V���z��ւ̃|�C���^

	DWORD									UserData[ 4 ] ;						// �O����`�̏��

	DWORD									Padding[ 4 ] ;
} ;

// �t���[���ɕt������V�F�C�v���
struct MV1_FRAME_SHAPE_F1
{
	int										ShapeNum ;							// �t���[���ɕt������V�F�C�v���̐�
	DWORD/*MV1_SHAPE_F1* */					Shape ;								// �V�F�C�v���z��ւ̃|�C���^
} ;

// �K�w�f�[�^�\����
/*
	PositionAndNormalData �̏��i�[����

		�ŏ��Ɉ����

			VertFlag �� MV1_FRAME_VERT_FLAG_POSITION_B16 ���t���Ă���ꍇ
				MV1_POSITION_16BIT_SUBINFO_F1	x, y, z ;				// ���W�l x, y, z ��16�r�b�g���p�⏕���

		PositionNum �̐�����

			���W�f�[�^
				VertFlag �� MV1_FRAME_VERT_FLAG_POSITION_B16 ���t���Ă���ꍇ WORD�^�A�t���Ă��Ȃ���� float�^

		PositionNum �̐�����

			�X�L�j���O���AVertFlag �� MV1_FRAME_VERT_FLAG_MATRIX_WEIGHT_NONE ���t���Ă���ꍇ�͖���
				�C���f�b�N�X�l�A�E�G�C�g�l�̏��ɃC���f�b�N�X�l�� -1 �ɂȂ邩�AMaxBoneBlendNum �̐��ɒB����܂ŁA�C���f�b�N�X�l�� -1 �̌��ɃE�G�C�g�l�͖���
				�C���f�b�N�X�l�� VertFlag �� MV1_FRAME_VERT_FLAG_MATRIX_INDEX_MASK  �ɉ������l
				�E�G�C�g�l��     VertFlag �� MV1_FRAME_VERT_FLAG_MATRIX_WEIGHT_MASK �ɉ������l


		�@�����AVertFlag �� MV1_FRAME_VERT_FLAG_NORMAL_TYPE_MASK �̒l�� MV1_FRAME_NORMAL_TYPE_NONE �̏ꍇ�͖���
		NormalNum �̐�����

			�@���f�[�^
				VertFlag �� MV1_FRAME_VERT_FLAG_NORMAL_TYPE_MASK �ɉ������l

*/
struct MV1_FRAME_F1
{
	DWORD/*MV1_FRAME_F1* */					DimPrev ;							// �f�[�^�z���̑O�̊K�w�f�[�^�ւ̃|�C���^
	DWORD/*MV1_FRAME_F1* */					DimNext ;							// �f�[�^�z���̎��̊K�w�f�[�^�ւ̃|�C���^

	DWORD/*char* */							Name ;								// ���O
	int										Index ;								// �C���f�b�N�X

	MV1_CHANGE_F1							ChangeDrawMaterialInfo ;			// �`��p�}�e���A����ԕω��Ǘ��p��{���
	MV1_CHANGE_F1							ChangeMatrixInfo ;					// �s���ԕω��Ǘ��p��{���

	int										TotalMeshNum ;						// �����̉��w�ɂ��郁�b�V���̑���
	int										TotalChildNum ;						// �����̉��w�ɂ���t���[���̐�
	DWORD/*MV1_FRAME_F1* */					Parent ;							// �e�K�w�ւ̃|�C���^
	DWORD/*MV1_FRAME_F1* */					FirstChild ;						// �q�K�w�ւ̃|�C���^(��[)
	DWORD/*MV1_FRAME_F1* */					LastChild ;							// �q�K�w�ւ̃|�C���^(���[)
	DWORD/*MV1_FRAME_F1* */					Prev ;								// �Z�K�w�ւ̃|�C���^
	DWORD/*MV1_FRAME_F1* */					Next ;								// ��K�w�ւ̃|�C���^

	VECTOR									Translate ;							// ���s�ړ�
	VECTOR									Scale ;								// �X�P�[��
	VECTOR									Rotate ;							// ��]
	int										RotateOrder ;						// ��]�I�[�_�[
	FLOAT4									Quaternion ;						// ��]( �N�H�[�^�j�I�� )

	DWORD									Flag ;								// �e��t���O( MV1_FRAMEFLAG_VISIBLE �Ȃ� )

	int										IsSkinMesh ;						// ���̃t���[���Ɋ܂܂�郁�b�V���̒��ɃX�L�����b�V�����܂܂�邩�ǂ���( TRUE:�܂܂��  FALSE:�܂܂�Ȃ� )
	int										TriangleNum ;						// ���̃t���[���Ɋ܂܂��|���S���̐�
	int										VertexNum ;							// ���̃t���[���Ɋ܂܂�钸�_�f�[�^�̐�
	int										MeshNum ;							// ���b�V���̐�
	DWORD/*MV1_MESH_F1* */					Mesh ;								// ���b�V�����X�g�ւ̃|�C���^

	int										SkinBoneNum ;						// ���̃t���[�����^�[�Q�b�g�Ƃ���{�[���̐�
	DWORD/*MV1_SKIN_BONE_F1* */				SkinBone ;							// ���̃t���[�����^�[�Q�b�g�Ƃ���ŏ��̃{�[���ւ̃|�C���^

	int										UseSkinBoneNum ;					// ���̃t���[�����g�p���Ă���{�[���̐�
	DWORD/*MV1_SKIN_BONE_F1** */			UseSkinBone ;						// ���̃t���[�����g�p���Ă���{�[���ւ̃|�C���^�̔z��

	DWORD/*MV1_LIGHT_F1* */					Light ;								// ���C�g���ւ̃|�C���^

	unsigned short							VertFlag ;							// ���_�f�[�^�Ɋւ���t���O
	unsigned short							MaxBoneBlendNum ;					// ��̍��W�f�[�^�Ŏg�p����{�[���E�G�C�g���̍ő吔

	float									SmoothingAngle ;					// �����@���v�Z�̏ꍇ�̃X���[�W���O���s�����ǂ�����臒l( �P�ʂ̓��W�A�� )
	int										AutoCreateNormal ;					// �@���̎����������g�p���邩�ǂ���( TRUE:�g�p����  FALSE:�g�p���Ȃ� )

	int										PositionNum ;						// ���W�̐�
	int										NormalNum ;							// �@���̐�
	DWORD/*void* */							PositionAndNormalData ;				// ���W�Ɩ@���f�[�^

	DWORD									UserData[ 4 ] ;						// �O����`�̏��

	DWORD/*MV1_FRAME_SHAPE_F1* */			FrameShape ;						// �V�F�C�v���( �����ꍇ�� NULL )

	VECTOR									PreRotate ;							// �O��]
	VECTOR									PostRotate ;						// ���]

	DWORD									Padding[ 8 ] ;
} ;

// �������Z�p���̃f�[�^�\����
struct MV1_PHYSICS_RIGIDBODY_F1
{
	DWORD/*MV1_PHYSICS_RIGIDBODY_F1* */		DimPrev ;							// �O�̃f�[�^�ւ̃|�C���^
	DWORD/*MV1_PHYSICS_RIGIDBODY_F1* */		DimNext ;							// ���̃f�[�^�ւ̃|�C���^

	DWORD/*char* */							Name ;								// ���O
	int										Index ;								// �C���f�b�N�X

	DWORD/*MV1_FRAME_F1* */					TargetFrame ;						// �ΏۂƂȂ�t���[��

	int										RigidBodyGroupIndex ;				// ���̃O���[�v�ԍ�
	DWORD									RigidBodyGroupTarget ;				// ���̃O���[�v�Ώ�
	int										ShapeType ;							// �`��( 0:��  1:��  2:�J�v�Z�� )
	float									ShapeW ;							// ��
	float									ShapeH ;							// ����
	float									ShapeD ;							// ���s
	VECTOR									Position ;							// �ʒu
	VECTOR									Rotation ;							// ��]( ���W�A�� )
	float									RigidBodyWeight ;					// ����
	float									RigidBodyPosDim ;					// �ړ���
	float									RigidBodyRotDim ;					// ��]��
	float									RigidBodyRecoil ;					// ������
	float									RigidBodyFriction ;					// ���C��
	int										RigidBodyType ;						// ���̃^�C�v( 0:Bone�Ǐ]  1:�������Z  2:�������Z(Bone�ʒu���킹) )
	int										NoCopyToBone ;						// �{�[���̍s��𕨗��ɓK�p���Ȃ����ǂ���

	DWORD									UserData[ 4 ] ;						// �O����`�̏��

	DWORD									Padding[ 4 ] ;
} ;

// �������Z�p���̃W���C���g�f�[�^�\����
struct MV1_PHYSICS_JOINT_F1
{
	DWORD/*MV1_PHYSICS_JOINT_F1* */			DimPrev ;							// �O�̃f�[�^�ւ̃|�C���^
	DWORD/*MV1_PHYSICS_JOINT_F1* */			DimNext ;							// ���̃f�[�^�ւ̃|�C���^

	DWORD/*char* */							Name ;								// ���O
	int										Index ;								// �C���f�b�N�X

	DWORD/*MV1_PHYSICS_RIGIDBODY_F1* */		RigidBodyA ;						// �ڑ��捄�̂`
	DWORD/*MV1_PHYSICS_RIGIDBODY_F1* */		RigidBodyB ;						// �ڑ��捄�̂a
	VECTOR									Position ;							// �ʒu
	VECTOR									Rotation ;							// ��]( ���W�A�� )
	VECTOR									ConstrainPosition1 ;				// �ړ������l�P
	VECTOR									ConstrainPosition2 ;				// �ړ������l�Q
	VECTOR									ConstrainRotation1 ;				// ��]�����l�P
	VECTOR									ConstrainRotation2 ;				// ��]�����l�Q
	VECTOR									SpringPosition ;					// �΂ˈړ��l
	VECTOR									SpringRotation ;					// �΂ˉ�]�l

	DWORD									UserData[ 4 ] ;						// �O����`�̏��

	DWORD									Padding[ 4 ] ;
} ;

// �t�@�C���ɕt�����镨�����Z�֘A�̏��
struct MV1_FILEHEAD_PHYSICS_F1
{
	float									WorldGravity ;						// �d�̓p�����[�^

	int										RigidBodyNum ;						// ���̃f�[�^�̐�
	DWORD/*MV1_PHYSICS_RIGIDBODY_F1* */		RigidBody ;							// ���̃f�[�^�z��ւ̃|�C���^

	int										JointNum ;							// �W���C���g�f�[�^�\����
	DWORD/*MV1_PHYSICS_JOINT_F1* */			Joint ;								// �W���C���g�f�[�^�z��ւ̃|�C���^

	DWORD									UserData[ 4 ] ;						// �O����`�̏��

	DWORD									Padding[ 4 ] ;
} ;

// �t�@�C���ɕt������V�F�C�v�֘A�̏��
struct MV1_FILEHEAD_SHAPE_F1
{
	int										FrameNum ;							// �t���[���ɕt������V�F�C�v�f�[�^�̐�
	DWORD/*MV1_FRAME_SHAPE_F1* */			Frame ;								// �t���[���ɕt������V�F�C�v�f�[�^�z��ւ̃|�C���^

	int										DataNum ;							// �V�F�C�v�f�[�^�̐�
	DWORD/*MV1_SHAPE_F1* */					Data ;								// �V�F�C�v�f�[�^�z��ւ̃|�C���^
	int										MeshNum ;							// �V�F�C�v���b�V���f�[�^�̐�
	DWORD/*MV1_SHAPE_MESH_F1* */			Mesh ;								// �V�F�C�v���b�V���f�[�^�z��ւ̃|�C���^
	int										VertexNum ;							// �V�F�C�v���_�f�[�^�̐�
	DWORD/*MV1_SHAPE_VERTEX_F1* */			Vertex ;							// �V�F�C�v���_�f�[�^�z��ւ̃|�C���^
	int										PressVertexDataSize ;				// ���k���ꂽ�V�F�C�v���_�f�[�^�̃T�C�Y
	DWORD/*void* */							PressVertexData ;					// ���k���ꂽ�V�F�C�v���_�f�[�^�ւ̃|�C���^
	int										TargetMeshVertexNum ;				// �V�F�C�v�Ώۂ̃��b�V���̒��_�̑���
	int										ShapeVertexUnitSize ;				// �V�F�C�v�p���_��ӂ�̃o�C�g��

	int										NormalPositionNum ;					// �V�F�C�v�p�g���C�A���O�����X�g�̃{�[����񖳂����W�f�[�^�̐�
	int										SkinPosition4BNum ;					// �V�F�C�v�p�g���C�A���O�����X�g�̂S�{�[���X�L�j���O���b�V�����W�f�[�^�̐�
	int										SkinPosition8BNum ;					// �V�F�C�v�p�g���C�A���O�����X�g�̂W�{�[���X�L�j���O���b�V�����W�f�[�^�̐�
	int										SkinPositionFREEBSize ;				// �V�F�C�v�p�g���C�A���O�����X�g�̂X�{�[���ȏ�X�L�j���O���b�V�����W�f�[�^�̃T�C�Y

	DWORD									UserData[ 4 ] ;						// �O����`�̏��

	DWORD									Padding[ 4 ] ;
} ;

// �t�@�C���w�b�_
struct MV1MODEL_FILEHEADER_F1
{
	BYTE									CheckID[ 4 ] ;						// �t�@�C���`���`�F�b�N�p�h�c( "MV11" )
	DWORD									Version ;							// �o�[�W����

	int										RightHandType ;						// �E����W�n���ǂ���( TRUE:�E����W�n  FALSE:������W�n )
	int										AutoCreateNormal ;					// �@���̎����������g�p���邩�ǂ���( TRUE:�g�p����  FALSE:�g�p���Ȃ� )

	int										ChangeDrawMaterialTableSize ;		// �`��}�e���A���̕ύX���Ǘ��p�ɕK�v�ȃf�[�^�̑��e��
	int										ChangeMatrixTableSize ;				// �s��̕ύX���Ǘ��p�ɕK�v�ȃf�[�^�̑��e��
	DWORD/*DWORD* */						ChangeDrawMaterialTable ;			// �`��p�}�e���A���ύX�m�F�p�r�b�g�f�[�^
	DWORD/*DWORD* */						ChangeMatrixTable ;					// �s��ύX�m�F�p�r�b�g�f�[�^

	int										FrameNum ;							// �K�w�f�[�^�̐�
	DWORD/*MV1_FRAME_F1* */					Frame ;								// �K�w�f�[�^���X�g�ւ̃|�C���^
	int										TopFrameNum ;						// �ŏ�ʊK�w�̃t���[���̐�
	DWORD/*MV1_FRAME_F1* */					FirstTopFrame ;						// �ŏ�ʊK�w�̍ŏ��̃t���[���ւ̃|�C���^
	DWORD/*MV1_FRAME_F1* */					LastTopFrame ;						// �ŏ�ʊK�w�̍Ō�̃t���[���ւ̃|�C���^
	int										FrameUseSkinBoneNum ;				// �t���[�����g�p���Ă���{�[���̑���
	DWORD/*MV1_SKIN_BONE_F1** */			FrameUseSkinBone ;					// �t���[�����g�p���Ă���{�[���ւ̃|�C���^

	int										MaterialNum ;						// �}�e���A���̐�
	DWORD/*MV1_MATERIAL_F1* */				Material ;							// �}�e���A���f�[�^���X�g�ւ̃|�C���^

	int										TextureNum ;						// �e�N�X�`���̐�
	DWORD/*MV1_TEXTURE_F1* */				Texture ;							// �e�N�X�`���f�[�^���X�g�ւ̃|�C���^

	int										MeshNum ;							// ���b�V���̐�
	DWORD/*MV1_MESH_F1* */					Mesh ;								// ���b�V���f�[�^���X�g�ւ̃|�C���^

	int										LightNum ;							// ���C�g�̐�
	DWORD/*MV1_LIGHT_F1* */					Light ;								// ���C�g�f�[�^�z��ւ̃|�C���^

	int										SkinBoneNum ;						// �X�L�j���O���b�V���̃{�[���Ƃ��Ďg�p����t���[���̏��̐�
	DWORD/*MV1_SKIN_BONE_F1* */				SkinBone ;							// �X�L�j���O���b�V���̃{�[���Ƃ��Ďg�p����t���[���̏��
	int										SkinBoneUseFrameNum ;				// �X�L�j���O���b�V���̃{�[�����g�p����t���[���̐�
	DWORD/*MV1_SKIN_BONE_USE_FRAME_F1** */	SkinBoneUseFrame ;					// �X�L�j���O���b�V���̃{�[�����g�p����t���[��

	int										TriangleListNum ;					// �g���C�A���O�����X�g�̐�
	DWORD/*MV1_TRIANGLE_LIST_F1* */			TriangleList ;						// �g���C�A���O�����X�g�̃��X�g�ւ̃|�C���^

	DWORD/*void* */							VertexData ;						// ���_�f�[�^
	DWORD									VertexDataSize ;					// ���_�f�[�^�̃T�C�Y

	int										TriangleListNormalPositionNum ;		// �Z�[�u�O�̃{�[����񖳂����W�f�[�^�̐�
	int										TriangleListSkinPosition4BNum ;		// �Z�[�u�O�̂S�{�[���X�L�j���O���b�V�����W�f�[�^�̐�
	int										TriangleListSkinPosition8BNum ;		// �Z�[�u�O�̂W�{�[���X�L�j���O���b�V�����W�f�[�^�̐�
	int										TriangleListSkinPositionFREEBSize ;	// �Z�[�u�O�̂X�{�[���ȏ�X�L�j���O���b�V�����W�f�[�^�̃T�C�Y
	int										MeshPositionSize ;					// ���b�V�����W�̃f�[�^�T�C�Y
	int										MeshNormalNum ;						// ���b�V���@���̐�
	int										MeshVertexSize ;					// ���b�V�����_���̃f�[�^�T�C�Y
	int										MeshFaceNum ;						// ���b�V���ʏ��̐�
	int										MeshVertexIndexNum ;				// ���_�C���f�b�N�X�f�[�^�̑���
	int										TriangleListIndexNum ;				// �g���C�A���O�����X�g�̃C���f�b�N�X�f�[�^�̑���

	int										TriangleNum ;						// �O�p�`�̐�
	int										TriangleListVertexNum ;				// �g���C�A���O�����X�g�̒��_�f�[�^�̐�

	int										StringSize ;						// ������f�[�^�̃T�C�Y
	DWORD/*char* */							StringBuffer ;						// ������f�[�^�ւ̃|�C���^

	int										OriginalAnimKeyDataSize ;			// �ۑ��O�̃A�j���[�V�����L�[�f�[�^�̃T�C�Y
	int										AnimKeyDataSize ;					// �A�j���[�V�����L�[�f�[�^�T�C�Y
	DWORD/*void* */							AnimKeyData ;						// �A�j���[�V�����L�[�f�[�^�ւ̃|�C���^

	int										AnimKeySetNum ;						// �A�j���[�V�����L�[�Z�b�g�̐�
	int										AnimKeySetUnitSize ;				// �A�j���[�V�����L�[�Z�b�g�\���̈�ӂ�̃T�C�Y
	DWORD/*MV1_ANIM_KEYSET_F1* */			AnimKeySet ;						// �A�j���[�V�����L�[�Z�b�g���X�g�ւ̃|�C���^

	int										AnimNum ;							// �A�j���[�V�����̐�
	int										AnimUnitSize ;						// �A�j���[�V�����\���̈�ӂ�̃T�C�Y
	DWORD/*MV1_ANIM_F1* */					Anim ;								// �A�j���[�V�����f�[�^���X�g�ւ̃|�C���^

	int										AnimSetNum ;						// �A�j���[�V�����Z�b�g�̐�
	DWORD/*MV1_ANIMSET_F1* */				AnimSet ;							// �A�j���[�V�����Z�b�g���X�g�ւ̃|�C���^

	DWORD									UserData[ 4 ] ;						// �O����`�̏��

	DWORD/*MV1_FILEHEAD_SHAPE_F1* */		Shape ;								// �V�F�C�v�Ɋւ�����A���݂��Ȃ��ꍇ�� NULL
	DWORD/*MV1_FILEHEAD_PHYSICS_F1* */		Physics ;							// �������Z�p�̏��A���݂��Ȃ��ꍇ�� NULL

	BYTE									MaterialNumberOrderDraw ;			// ���蓖�Ă��Ă���}�e���A���̔ԍ����Ⴂ���b�V������`�悷�邩�ǂ���( 1:����  0:���Ȃ� )
	BYTE									IsStringUTF8 ;						// StringBuffer �Ɋi�[����Ă��镶���� UTF-8 ���ǂ���( 1:UTF-8  0:Shift-JIS )

	BYTE									Padding1[ 2 ] ;
	DWORD									Padding2[ 13 ] ;
} ;


// �f�[�^�錾 --------------------------------

// �֐��v���g�^�C�v�錾 ----------------------

#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

#endif	// DX_NON_MODEL

#endif

