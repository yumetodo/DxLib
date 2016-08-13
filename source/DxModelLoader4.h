// -------------------------------------------------------------------------------
// 
// 		�c�w���C�u����		���f���f�[�^�ǂݍ��ݏ����S�p�w�b�_
// 
// 				Ver 3.16d
// 
// -------------------------------------------------------------------------------

#ifndef __DXMODELLOADER4_H__
#define __DXMODELLOADER4_H__

#include "DxCompileConfig.h"

#ifndef DX_NON_MODEL

// �C���N���[�h ---------------------------------
#include "DxLib.h"
#include "DxStatic.h"
#include "DxModel.h"
#include "DxModelRead.h"

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// �}�N����` -----------------------------------

#define PMX_MAX_IKLINKNUM		(64)			// �Ή�����IK�����N�̍ő吔

// �f�[�^�^��` ---------------------------------

// PMX�t�@�C���̏����i�[����\����
struct PMX_BASEINFO
{
	BYTE	EncodeType ;						// �����R�[�h�̃G���R�[�h�^�C�v 0:UTF16 1:UTF8
	BYTE	UVNum ;								// �ǉ��t�u�� ( 0�`4 )
	BYTE	VertexIndexSize ;					// ���_Index�T�C�Y ( 1 or 2 or 4 )
	BYTE	TextureIndexSize ;					// �e�N�X�`��Index�T�C�Y ( 1 or 2 or 4 )
	BYTE	MaterialIndexSize ;					// �}�e���A��Index�T�C�Y ( 1 or 2 or 4 )
	BYTE	BoneIndexSize ;						// �{�[��Index�T�C�Y ( 1 or 2 or 4 )
	BYTE	MorphIndexSize ;					// ���[�tIndex�T�C�Y ( 1 or 2 or 4 )
	BYTE	RigidIndexSize ;					// ����Index�T�C�Y ( 1 or 2 or 4 )
} ;

// ���_�f�[�^
struct PMX_VERTEX
{
	float	Position[ 3 ] ;						// ���W
	float	Normal[ 3 ] ;						// �@��
	float	UV[ 2 ] ;							// �W��UV�l
	float	AddUV[ 4 ][ 4 ] ;					// �ǉ�UV�l
	BYTE	WeightType ;						// �E�G�C�g�^�C�v( 0:BDEF1 1:BDEF2 2:BDEF4 3:SDEF )
	int		BoneIndex[ 4 ] ;					// �{�[���C���f�b�N�X
	float	BoneWeight[ 4 ] ;					// �{�[���E�F�C�g
	float	SDEF_C[ 3 ] ;						// SDEF-C
	float	SDEF_R0[ 3 ] ;						// SDEF-R0
	float	SDEF_R1[ 3 ] ;						// SDEF-R1
	float	ToonEdgeScale ;						// �g�D�[���G�b�W�̃X�P�[��
} ;

// �ʃ��X�g
struct PMX_FACE
{
	int		VertexIndex[ 3 ] ;
} ;

// �e�N�X�`�����
struct PMX_TEXTURE
{
	wchar_t	TexturePath[ 512 ] ;
} ;

// �}�e���A�����
struct PMX_MATERIAL
{
	wchar_t	Name[ 128 ] ;						// ���O

	float	Diffuse[ 4 ] ;						// �f�B�t���[�Y�J���[
	float	Specular[ 3 ] ;						// �X�y�L�����J���[
	float	SpecularPower ;						// �X�y�L�����萔
	float	Ambient[ 3 ] ;						// �A���r�G���g�J���[

	BYTE	CullingOff ;						// ���ʕ`��
	BYTE	GroundShadow ;						// �n�ʉe
	BYTE	SelfShadowMap ;						// �Z���t�V���h�E�}�b�v�ւ̕`��
	BYTE	SelfShadowDraw ;					// �Z���t�V���h�E�̕`��
	BYTE	EdgeDraw ;							// �G�b�W�̕`��

	float	EdgeColor[ 4 ] ;					// �G�b�W�J���[
	float	EdgeSize ;							// �G�b�W�T�C�Y

	int		TextureIndex ;						// �ʏ�e�N�X�`���C���f�b�N�X
	int		SphereTextureIndex ;				// �X�t�B�A�e�N�X�`���C���f�b�N�X
	BYTE	SphereMode ;						// �X�t�B�A���[�h( 0:���� 1:��Z 2:���Z 3:�T�u�e�N�X�`��(�ǉ�UV1��x,y��UV�Q�Ƃ��Ēʏ�e�N�X�`���`����s��)

	BYTE	ToonFlag ;							// ���L�g�D�[���t���O( 0:��Toon 1:���LToon )
	int		ToonTextureIndex ;					// �g�D�[���e�N�X�`���C���f�b�N�X

	int		MaterialFaceNum ;					// �}�e���A�����K������Ă���ʂ̐�
} ;

// �h�j�����N���
struct PMX_IKLINK
{
	int		BoneIndex ;							// �����N�{�[���̃C���f�b�N�X
	BYTE	RotLockFlag ;						// ��]����( 0:OFF 1:ON )
	float	RotLockMin[ 3 ] ;					// ��]�����A����
	float	RotLockMax[ 3 ] ;					// ��]�����A���
} ;

// �h�j���
struct PMX_IK
{
	int		TargetBoneIndex ;					// IK�^�[�Q�b�g�̃{�[���C���f�b�N�X
	int		LoopNum ;							// IK�v�Z�̃��[�v��
	float	RotLimit ;							// �v�Z���ӂ�̐����p�x

	int		LinkNum ;							// �h�j�����N�̐�
	PMX_IKLINK Link[ PMX_MAX_IKLINKNUM ] ;		// �h�j�����N���
} ;

// �{�[�����
struct PMX_BONE
{
	wchar_t	Name[ 128 ] ;						// ���O
	float	Position[ 3 ] ;						// ���W
	int		ParentBoneIndex ;					// �e�{�[���C���f�b�N�X
	int		TransformLayer ;					// �ό`�K�w
	
	BYTE	Flag_LinkDest ;						// �ڑ���
	BYTE	Flag_EnableRot ;					// ��]���ł��邩
	BYTE	Flag_EnableMov ;					// �ړ����ł��邩
	BYTE	Flag_Disp ;							// �\��
	BYTE	Flag_EnableControl ;				// ���삪�ł��邩
	BYTE	Flag_IK ;							// IK
	BYTE	Flag_AddRot ;						// ��]�t�^
	BYTE	Flag_AddMov ;						// �ړ��t�^
	BYTE	Flag_LockAxis ;						// ���Œ�
	BYTE	Flag_LocalAxis ;					// ���[�J����
	BYTE	Flag_AfterPhysicsTransform ;		// ������ό`
	BYTE	Flag_OutParentTransform ;			// �O���e�ό`

	float	OffsetPosition[ 3 ] ;				// �I�t�Z�b�g���W
	int		LinkBoneIndex ;						// �ڑ���{�[���C���f�b�N�X
	int		AddParentBoneIndex ;				// �t�^�e�{�[���C���f�b�N�X
	float	AddRatio ;							// �t�^��
	float	LockAxisVector[ 3 ] ;				// ���Œ莞�̎��̕����x�N�g��
	float	LocalAxisXVector[ 3 ] ;				// ���[�J�����̂w��
	float	LocalAxisZVector[ 3 ] ;				// ���[�J�����̂y��
	int		OutParentTransformKey ;				// �O���e�ό`���� Key�l

	PMX_IK	IKInfo ;							// �h�j���
} ;

// ���_���[�t���
struct PMX_MORPH_VERTEX
{
	int		Index ;								// ���_�C���f�b�N�X
	float	Offset[ 3 ] ;						// ���_���W�I�t�Z�b�g
} ;

// �t�u���[�t���
struct PMX_MORPH_UV
{
	int		Index ;								// ���_�C���f�b�N�X
	FLOAT4	Offset ;							// ���_�t�u�I�t�Z�b�g
} ;

// �{�[�����[�t���
struct PMX_MORPH_BONE
{
	int		Index ;								// �{�[���C���f�b�N�X
	float	Offset[ 3 ] ;						// ���W�I�t�Z�b�g
	float	Quat[ 4 ] ;							// ��]�N�H�[�^�j�I��
} ;

// �ގ����[�t���
struct PMX_MORPH_MATERIAL
{
	int		Index ;								// �}�e���A���C���f�b�N�X
	BYTE	CalcType ;							// �v�Z�^�C�v( 0:��Z  1:���Z )
	float	Diffuse[ 4 ] ;						// �f�B�t���[�Y�J���[
	float	Specular[ 3 ] ;						// �X�y�L�����J���[
	float	SpecularPower ;						// �X�y�L�����W��
	float	Ambient[ 3 ] ;						// �A���r�G���g�J���[
	float	EdgeColor[ 4 ] ;					// �G�b�W�J���[
	float	EdgeSize ;							// �G�b�W�T�C�Y
	float	TextureScale[ 4 ] ;					// �e�N�X�`���W��
	float	SphereTextureScale[ 4 ] ;			// �X�t�B�A�e�N�X�`���W��
	float	ToonTextureScale[ 4 ] ;				// �g�D�[���e�N�X�`���W��
} ;

// �O���[�v���[�t
struct PMX_MORPH_GROUP
{
	int		Index ;								// ���[�t�C���f�b�N�X
	float	Ratio ;								// ���[�t��
} ;

// ���[�t���
struct PMX_MORPH
{
	wchar_t	Name[ 128 ] ;						// ���O

	BYTE	ControlPanel ;						// ����p�l��
	BYTE	Type ;								// ���[�t�̎��  0:�O���[�v 1:���_ 2:�{�[�� 3:UV 4:�ǉ�UV1 5:�ǉ�UV2 6:�ǉ�UV3 7:�ǉ�UV4 8:�ގ�

	int		DataNum ;							// ���[�t���̐�

	union
	{
		PMX_MORPH_VERTEX *Vertex ;				// ���_���[�t
		PMX_MORPH_UV *UV ;						// UV���[�t
		PMX_MORPH_BONE *Bone ;					// �{�[�����[�t
		PMX_MORPH_MATERIAL *Material ;			// �}�e���A�����[�t
		PMX_MORPH_GROUP *Group ;				// �O���[�v���[�t
	} ;
} ;

// ���̏��
struct PMX_RIGIDBODY
{
	wchar_t	Name[ 128 ] ;						// ���O

	int		BoneIndex ;							// �Ώۃ{�[���ԍ�

	BYTE	RigidBodyGroupIndex ;				// ���̃O���[�v�ԍ�
	WORD	RigidBodyGroupTarget ;				// ���̃O���[�v�Ώ�

	BYTE	ShapeType ;							// �`��( 0:��  1:��  2:�J�v�Z�� )
	float	ShapeW ;							// ��
	float	ShapeH ;							// ����
	float	ShapeD ;							// ���s

	float	Position[ 3 ] ;						// �ʒu
	float	Rotation[ 3 ] ;						// ��]( ���W�A�� )

	float	RigidBodyWeight ;					// ����
	float	RigidBodyPosDim ;					// �ړ�����
	float	RigidBodyRotDim ;					// ��]����
	float	RigidBodyRecoil ;					// ������
	float	RigidBodyFriction ;					// ���C��

	BYTE	RigidBodyType ;						// ���̃^�C�v( 0:Bone�Ǐ]  1:�������Z  2:�������Z(Bone�ʒu���킹) )
} ;

// �W���C���g���
struct PMX_JOINT
{
	wchar_t	Name[ 128 ] ;						// ���O

	BYTE	Type ;								// ���( 0:�X�v�����O6DOF ( PMX2.0 �ł� 0 �̂� )

	int		RigidBodyAIndex ;					// �ڑ��捄�̂`
	int		RigidBodyBIndex ;					// �ڑ��捄�̂a

	float	Position[ 3 ] ;						// �ʒu
	float	Rotation[ 3 ] ;						// ��]( ���W�A�� )

	float	ConstrainPositionMin[ 3 ] ;			// �ړ�����-����
	float	ConstrainPositionMax[ 3 ] ;			// �ړ�����-���
	float	ConstrainRotationMin[ 3 ] ;			// ��]����-����
	float	ConstrainRotationMax[ 3 ] ;			// ��]����-���

	float	SpringPosition[ 3 ] ;				// �o�l�萔-�ړ�
	float	SpringRotation[ 3 ] ;				// �o�l�萔-��]
} ;

// VMD�ǂݍ��ݏ����pPMX�{�[���f�[�^
struct PMX_READ_BONE_INFO
{
	PMX_BONE			*Base ;					// �{�[����f�[�^
	MV1_ANIM_R			*Anim ;					// ���̃{�[���p�̃A�j���[�V�������ւ̃|�C���^
	MV1_FRAME_R			*Frame ;				// ���̃{�[���p�̃t���[�����ւ̃|�C���^

	VECTOR				*KeyPos ;				// ���W�A�j���[�V�����L�[�z��̐擪
	FLOAT4				*KeyRot ;				// ��]�A�j���[�V�����L�[�z��̐擪
	float				*KeyPosTime ;			// ���W�A�j���[�V�����^�C���L�[�z��̐擪
	float				*KeyRotTime ;			// ��]�A�j���[�V�����^�C���L�[�z��̐擪
	MATRIX				*KeyMatrix ;			// �s��A�j���[�V�����L�[�z��̐擪
	MATRIX				*KeyMatrix2 ;			// �s��A�j���[�V�����L�[�z��̐擪( IsIKChild �� TRUE �̃{�[���p )

	int					IsPhysics ;				// �������g�p���Ă��邩�ǂ���
	int					IsAddParent ;			// �t�^�e�@�\���g�p���Ă��邩�ǂ���
	int					IsIK ;					// �h�j�{�[�����ǂ���
	int					IsIKAnim ;				// �h�j�A�j���[�V���������邩�ǂ���
	int					IsIKChild ;				// �h�j�A�j���[�V��������{�[���̎q�{�[��( ���h�j�̉e�����󂯂Ȃ��{�[�� )���ǂ���
	MATRIX				LocalMatrix ;			// �\�z�������[�J���s��
	int					SetupLocalWorldMatrix ;	// ���[�J�������[���h�s�񂪍\�z����Ă��邩�ǂ���
	MATRIX				LocalWorldMatrix ;		// �\�z�������[�J�������[���h�s��
	VECTOR				Translate ;				// ���s�ړ��l
	FLOAT4				Rotate ;				// ��]�l
	VECTOR				OrgTranslate ;			// ���_����̏����ʒu

	MATRIX				IKmat ;					// IK�����Ŏg�p����s��\����
	FLOAT4				IKQuat ;				// IK�����Ŏg�p����N�H�[�^�j�I��

	VECTOR				InitTranslate ;			// ���s�ړ��l�̏����l
	FLOAT4				InitRotate ;			// ��]�l�̏����l
#ifndef DX_NON_BULLET_PHYSICS
	int					PhysicsIndex ;			// �������Z�p���̔ԍ�
	int					SetupPhysicsAnim ;		// �{�[���A�j���[�V������t�������ǂ���
#endif

	struct VMD_READ_NODE_INFO	*Node ;			// �{�[���Ɗ֘A�t�����Ă���m�[�h
	struct VMD_READ_KEY_INFO	*NowKey ;		// ���ݍĐ����Ă���L�[
} ;

// VMD�ǂݍ��ݏ����pPMXIK�{�[���f�[�^
struct PMX_READ_IK_INFO
{
	PMX_IK				*Base ;					// IK�{�[����f�[�^
	PMX_READ_BONE_INFO	*Bone ;					// IK�^�[�Q�b�g�{�[��
	PMX_READ_BONE_INFO	*TargetBone ;			// IK��[�{�[��

//	WORD				LimitAngleIK ;			// �p�x�����{�[�����܂܂ꂽ�h�j���ǂ���( 0:�Ⴄ 1:���� ) 
//	WORD				Iterations ;			// �ċN���Z��
//	float				ControlWeight ;			// IK�̉e���x
//	float				IKTotalLength ;			// IK�����̍�������`�F�C���̍Ō�܂ł̋���

//	int					ChainBoneNum ;			// IK�`�F�[���̒���(�q�̐�)
//	WORD				*ChainBone ;			// IK�e�����̃{�[���ւ̃C���f�b�N�X�z��ւ̃|�C���^

	PMX_READ_IK_INFO	*Prev ;					// ���X�g�̑O�̃f�[�^�ւ̃A�h���X
	PMX_READ_IK_INFO	*Next ;					// ���X�g�̎��̃f�[�^�ւ̃A�h���X
} ;

// PMX�ǂݍ��ݏ����p�������Z�����p�f�[�^
struct PMX_READ_PHYSICS_INFO
{
	PMX_RIGIDBODY		*Base ;					// ��f�[�^

	PMX_READ_BONE_INFO	*Bone ;					// �{�[���̏��ւ̃|�C���^
	bool				NoCopyToBone ;			// �{�[���֍��̂̏�Ԃ�����킷�s����R�s�[���Ȃ����ǂ���

	DWORD				BulletInfo[ 320 ] ;		// BULLET_RIGIDBODY_INFO ���i�[����o�b�t�@
} ;

// PMX�ǂݍ��ݏ����pPMD�������Z�����p�W���C���g�f�[�^
struct PMX_READ_PHYSICS_JOINT_INFO
{
	PMX_JOINT			*Base ;					// �W���C���g��f�[�^

	DWORD				BulletInfo[ 32 ] ;		// BULLET_JOINT_INFO ���i�[����o�b�t�@
} ;

// �o�l�w�ǂݍ��ݏ������̕����֌W�̏���Z�߂��\����
struct DX_MODELLOADER3_PMX_PHYSICS_INFO
{
	VECTOR Gravity ;
	int MotionTotalFrameNum ;

	int LoopMotionFlag ;
	int LoopMotionNum ;

	int	PhysicsCalcPrecision ;

	int PmxPhysicsNum ;
	PMX_READ_PHYSICS_INFO *PmxPhysicsInfoDim ;

	int PmxPhysicsJointNum ;
	PMX_READ_PHYSICS_JOINT_INFO *PmxPhysicsJointInfoDim ;

	MV1_ANIMKEYSET_R *KeyMatrixSet ;

	DWORD BulletPhysicsDataBuffer[ 32 ] ;			// BULLET_PHYSICS ���i�[����o�b�t�@
} ;


// �֐��錾 -------------------------------------

#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

#ifndef DX_NON_BULLET_PHYSICS

extern int SetupPhysicsObject_PMXPhysicsInfo(		DX_MODELLOADER3_PMX_PHYSICS_INFO *MLPhysicsInfoint, int DisablePhysicsFile ) ;
extern int ReleasePhysicsObject_PMXPhysicsInfo(		DX_MODELLOADER3_PMX_PHYSICS_INFO *MLPhysicsInfo ) ;
extern int OneFrameProcess_PMXPhysicsInfo(			DX_MODELLOADER3_PMX_PHYSICS_INFO *MLPhysicsInfo, int FrameNo, int LoopNo, bool FPS60, int ValidNextRate, int TimeDivNum ) ;
extern int CheckDisablePhysicsAnim_PMXPhysicsInfo(	DX_MODELLOADER3_PMX_PHYSICS_INFO *MLPhysicsInfo, int PhysicsIndex ) ;

#endif


#endif
#endif
