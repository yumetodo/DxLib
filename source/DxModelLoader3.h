// -------------------------------------------------------------------------------
// 
// 		�c�w���C�u����		���f���f�[�^�ǂݍ��ݏ����R�p�w�b�_
// 
// 				Ver 3.16d
// 
// -------------------------------------------------------------------------------

#ifndef __DXMODELLOADER3_H__
#define __DXMODELLOADER3_H__

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

// �f�[�^�^��` ---------------------------------

// PMD�f�[�^�w�b�_( 283byte )
struct PMD_HEADER
{
	BYTE	Data[ 283 ] ;						// �f�[�^
/*
	char	Magic[ 3 ] ;						// "Pmd"
	float	Version ;							// 1.0f ( 0x3f800000 )
	char	Name[ 20 ] ;						// ���O
	char	Comment[ 256 ] ;					// �R�����g
*/
} ;

// ���_�f�[�^��ӂ�̏��( 38byte )
struct PMD_VERTEX
{
	float	Position[ 3 ] ;						// ���W
	float	Normal[ 3 ] ;						// �@��
	float	Uv[ 2 ] ;							// �e�N�X�`�����W
	WORD	BoneNo[ 2 ] ;						// �{�[���ԍ�
	BYTE	BoneWeight ;						// BoneNo[ 0 ] �̃{�[���̉e���x( 0�`100 ) BoneNo[ 1 ] �̉e���x��( 100 - BoneWeight )
	BYTE	Edge ;								// �G�b�W�t���O  0:�G�b�W�L��  1:�G�b�W����
} ;

// ���_�f�[�^
struct PMD_VERTEX_DATA
{
	DWORD	Count ;				// ���_�f�[�^�̐�
	/* ������ PMD_VERTEX �� Count �̐��������݂��� */
} ;

// �ʃ��X�g
struct PMD_FACE_DATA
{
	DWORD	VertexCount ;		// ���_�̐�( �ʂ̐��� VertexCount / 3 )
	/* ������ WORD �̒��_�C���f�b�N�X�� VertexCount �̐��������݂��� */
} ;

// �}�e���A����ӂ�̏��( 70byte )
struct PMD_MATERIAL
{
	BYTE	Data[ 70 ] ;						// �f�[�^
/*
	float	DiffuseR, DiffuseG, DiffuseB ;		// �f�B�t���[�Y�J���[
	float	Alpha ;								// ���l
	float	Specularity ;						// �X�y�L�����W��
	float	SpecularR, SpecularG, SpecularB ;	// �X�y�L�����J���[
	float	AmbientR, AmbientG, AmbientB ;		// �A���r�G���g�J���[
	BYTE	ToolImage ;							// �g�D�[�������_�����O�p�̃e�N�X�`���̃C���f�b�N�X
	BYTE	Edge ;								// �֊s�E�e
	DWORD	FaceVertexCount ;					// ���̃}�e���A�����g�p����ʒ��_�̐�
	char	TextureFileName[ 20 ] ;				// �e�N�X�`���t�@�C����
*/
} ;

// �}�e���A���̏��
struct PMD_MATERIAL_DATA
{
	DWORD	Count ;								// �}�e���A���̐�
	/* ������ PMD_MATERIAL �� Count �̐��������݂��� */
} ;

// �{�[����ӂ�̏��( 39byte )
struct PMD_BONE
{
	BYTE	Data[ 39 ] ;						// �f�[�^
/*
	char	Name[ 20 ] ;						//  0:�{�[����
	WORD	Parent ;							// 20:�e�{�[��( �����ꍇ�� 0xffff )
	WORD	TailPosBone ;						// 22:Tail �ʒu�̃{�[��( �`�F�[�����[�̏ꍇ�� 0xffff )
	BYTE	Type ;								// 24:�{�[���̃^�C�v( 0:��]  1:��]�ƈړ�  2:IK  3:�s��  4:IK�e����  5:��]�e����  6:IK�ڑ���  7:��\��  8:�P��  9:��]�^�� )
	WORD	IkParent ;							// 25:IK�{�[���ԍ�( �e��IK�{�[��  �����ꍇ�� 0xffff )
	float	HeadPos[ 3 ] ;						// 27:�w�b�h�̈ʒu
*/
} ;

// �{�[���̏��
struct PMD_BONE_DATA
{
	WORD	Count ;								// �{�[���̐�
	/* ������ PMD_BONE �� Count �̐��������݂��� */
} ;

// IK�f�[�^��ӂ�̏��( ChainBoneIndex �𔲂����T�C�Y 11byte )
struct PMD_IK
{
	BYTE	Data[ 11 ] ;						// �f�[�^
/*
	WORD	Bone ;								// IK�{�[��
	WORD	TargetBone ;						// IK�^�[�Q�b�g�{�[��
	BYTE	ChainLength ;						// IK�`�F�[���̒���(�q�̐�)
	WORD	Iterations ;						// �ċN���Z��
	float	ControlWeight ;						// IK�̉e���x
	WORD	ChainBoneIndexs[ ChainLength ] ;	// IK�e�����̃{�[���ԍ�
*/
} ;

// IK�̏��
struct PMD_IK_DATA
{
	WORD	Count ;								// IK�f�[�^�̐�
	/* ������ PMD_IK �� Count �̐��������݂��� */
} ;

// �\��̒��_���
struct PMD_SKIN_VERT
{
	DWORD	TargetVertexIndex ;					// �Ώۂ̒��_�̃C���f�b�N�X
	float	Position[ 3 ] ;						// ���W( PMD_SKIN �� SkinType �� 0 �Ȃ��΍��W�A����ȊO�Ȃ� base �ɑ΂��鑊�΍��W )
} ;

// �\���ӂ�̏��( Vertex �𔲂����T�C�Y 25byte )
struct PMD_SKIN
{
	BYTE	Data[ 25 ] ;						// �f�[�^
/*
	char	Name[ 20 ] ;						// �\�
	DWORD	VertexCount ;						// ���_�̐�
	BYTE	SkinType ;							// �\��̎��( 0:base 1�F�܂�A2�F�ځA3�F���b�v�A4�F���̑� )
	PMD_SKIN_VERT Vertex[ VertexCount ] ;		// �\��p�̒��_�f�[�^
*/
} ;

// �\��̏��
struct PMD_SKIN_DATA
{
	WORD	Count ;								// �\��f�[�^�̐�
	/* ������ PMD_SKIN �� Count �̐��������݂��� */
} ;

// �������Z�f�[�^��ӂ�̏��( 83byte )
struct PMD_PHYSICS
{
	BYTE	Data[ 83 ] ;	
/*
	char	RigidBodyName[ 20 ] ;				//  0 : ���̖�
	WORD	RigidBodyRelBoneIndex ;				// 20 : ���̊֘A�{�[���ԍ�
	BYTE	RigidBodyGroupIndex ;				// 22 : ���̃O���[�v�ԍ�
	WORD	RigidBodyGroupTarget ;				// 23 : ���̃O���[�v�Ώ�
	BYTE	ShapeType ;							// 25 : �`��( 0:��  1:��  2:�J�v�Z�� )
	float	ShapeW ;							// 26 : ��
	float	ShapeH ;							// 30 : ����
	float	ShapeD ;							// 34 : ���s
	float	Position[ 3 ] ;						// 38 : �ʒu
	float	Rotation[ 3 ] ;						// 50 : ��]( ���W�A�� )
	float	RigidBodyWeight ;					// 62 : ����
	float	RigidBodyPosDim ;					// 66 : �ړ���
	float	RigidBodyRotDim ;					// 70 : ��]��
	float	RigidBodyRecoil ;					// 74 : ������
	float	RigidBodyFriction ;					// 78 : ���C��
	BYTE	RigidBodyType ;						// 82 : ���̃^�C�v( 0:Bone�Ǐ]  1:�������Z  2:�������Z(Bone�ʒu���킹) )
*/
} ;

// �������Z�f�[�^�̏��
struct PMD_PHYSICS_DATA
{
	DWORD	Count ;								// ���̃f�[�^�̐�
	/* ������ PMD_PHYSICS �� Count �̐��������݂��� */
} ;

// �������Z�p�W���C���g�f�[�^��ӂ�̏��( 124byte )
struct PMD_PHYSICS_JOINT
{
	char	Name[ 20 ] ;						// ���O
	DWORD	RigidBodyA ;						// �ڑ��捄�̂`
	DWORD	RigidBodyB ;						// �ڑ��捄�̂a
	float	Position[ 3 ] ;						// �ʒu
	float	Rotation[ 3 ] ;						// ��]( ���W�A�� )
	float	ConstrainPosition1[ 3 ] ;			// �ړ������l�P
	float	ConstrainPosition2[ 3 ] ;			// �ړ������l�Q
	float	ConstrainRotation1[ 3 ] ;			// ��]�����l�P
	float	ConstrainRotation2[ 3 ] ;			// ��]�����l�Q
	float	SpringPosition[ 3 ] ;				// �΂ˈړ��l
	float	SpringRotation[ 3 ] ;				// �΂ˉ�]�l
} ;

// �������Z�p�W���C���g���
struct PMD_PHYSICS_JOINT_DATA
{
	DWORD	Count ;								// �W���C���g�f�[�^�̐�
	/* ������ PMD_PHYSICS_JOINT �� Count �̐��������݂��� */
} ;

// VMD�ǂݍ��ݏ����pPMD�{�[���f�[�^
struct PMD_READ_BONE_INFO
{
	PMD_BONE			*Base ;					// �{�[����f�[�^
	MV1_ANIM_R			*Anim ;					// ���̃{�[���p�̃A�j���[�V�������ւ̃|�C���^
	MV1_FRAME_R			*Frame ;				// ���̃{�[���p�̃t���[�����ւ̃|�C���^

	VECTOR				*KeyPos ;				// ���W�A�j���[�V�����L�[�z��̐擪
	FLOAT4				*KeyRot ;				// ��]�A�j���[�V�����L�[�z��̐擪
	float				*KeyPosTime ;			// ���W�A�j���[�V�����^�C���L�[�z��̐擪
	float				*KeyRotTime ;			// ��]�A�j���[�V�����^�C���L�[�z��̐擪
	MATRIX				*KeyMatrix ;			// �s��A�j���[�V�����L�[�z��̐擪
	MATRIX				*KeyMatrix2 ;			// �s��A�j���[�V�����L�[�z��̐擪( IsIKChild �� TRUE �̃{�[���p )

	int					Type ;					// �^�C�v
	int					IsPhysics ;				// �������g�p���Ă��邩�ǂ���
	int					IsIK ;					// �h�j�{�[�����ǂ���
	int					IsIKAnim ;				// �h�j�A�j���[�V���������邩�ǂ���
	int					IsIKChild ;				// �h�j�A�j���[�V��������{�[���̎q�{�[��( ���h�j�̉e�����󂯂Ȃ��{�[�� )���ǂ���
	int					IKLimitAngle ;			// �h�j���Ɋp�x���������邩�ǂ���
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

// VMD�ǂݍ��ݏ����pPMDIK�{�[���f�[�^
struct PMD_READ_IK_INFO
{
	PMD_IK				*Base ;					// IK�{�[����f�[�^
	PMD_READ_BONE_INFO	*Bone ;					// IK�^�[�Q�b�g�{�[��
	PMD_READ_BONE_INFO	*TargetBone ;			// IK��[�{�[��

	WORD				LimitAngleIK ;			// �p�x�����{�[�����܂܂ꂽ�h�j���ǂ���( 0:�Ⴄ 1:���� ) 
	WORD				Iterations ;			// �ċN���Z��
	float				ControlWeight ;			// IK�̉e���x
	float				IKTotalLength ;			// IK�����̍�������`�F�C���̍Ō�܂ł̋���

	int					ChainBoneNum ;			// IK�`�F�[���̒���(�q�̐�)
	WORD				*ChainBone ;			// IK�e�����̃{�[���ւ̃C���f�b�N�X�z��ւ̃|�C���^

	PMD_READ_IK_INFO	*Prev ;					// ���X�g�̑O�̃f�[�^�ւ̃A�h���X
	PMD_READ_IK_INFO	*Next ;					// ���X�g�̎��̃f�[�^�ւ̃A�h���X
} ;

// PMD�ǂݍ��ݏ����p�������Z�����p�f�[�^
struct PMD_READ_PHYSICS_INFO
{
	PMD_PHYSICS			*Base ;					// ��f�[�^

	char				Name[ 24 ] ;			// ���O
	WORD				BoneIndex ;				// �Ώۂ̃{�[��

	PMD_READ_BONE_INFO	*Bone ;					// �{�[���̏��ւ̃|�C���^
	int					ShapeType ;				// �`��^�C�v( 0:��  1:��  2:�J�v�Z�� )
	int					RigidBodyType ;			// ���̃^�C�v( 0:Bone�Ǐ]  1:�������Z  2:�������Z(Bone�ʒu���킹) )
	bool				NoCopyToBone ;			// �{�[���֍��̂̏�Ԃ�����킷�s����R�s�[���Ȃ����ǂ���

	DWORD				BulletInfo[ 160 ] ;		// BULLET_RIGIDBODY_INFO ���i�[����o�b�t�@
} ;

// PMD�ǂݍ��ݏ����pPMD�������Z�����p�W���C���g�f�[�^
struct PMD_READ_PHYSICS_JOINT_INFO
{
	PMD_PHYSICS_JOINT	*Base ;					// �W���C���g��f�[�^

	DWORD				BulletInfo[ 16 ] ;		// BULLET_JOINT_INFO ���i�[����o�b�t�@
} ;


// �o�l�c�ǂݍ��ݏ������̕����֌W�̏���Z�߂��\����
struct DX_MODELLOADER3_PMD_PHYSICS_INFO
{
	VECTOR Gravity ;
	int MotionTotalFrameNum ;

	int LoopMotionFlag ;
	int LoopMotionNum ;

	int	PhysicsCalcPrecision ;

	int PmdPhysicsNum ;
	PMD_READ_PHYSICS_INFO *PmdPhysicsInfoDim ;

	int PmdPhysicsJointNum ;
	PMD_READ_PHYSICS_JOINT_INFO *PmdPhysicsJointInfoDim ;

	MV1_ANIMKEYSET_R *KeyMatrixSet ;

	DWORD BulletPhysicsDataBuffer[ 32 ] ;			// BULLET_PHYSICS ���i�[����o�b�t�@
} ;


// �֐��錾 -------------------------------------

#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

#ifndef DX_NON_BULLET_PHYSICS

extern int SetupPhysicsObject_PMDPhysicsInfo(		DX_MODELLOADER3_PMD_PHYSICS_INFO *MLPhysicsInfo, int DisablePhysicsFile ) ;
extern int ReleasePhysicsObject_PMDPhysicsInfo(		DX_MODELLOADER3_PMD_PHYSICS_INFO *MLPhysicsInfo ) ;
extern int OneFrameProcess_PMDPhysicsInfo(			DX_MODELLOADER3_PMD_PHYSICS_INFO *MLPhysicsInfo, int FrameNo, int LoopNo, bool FPS60, int ValidNextRate, int TimeDivNum ) ;
extern int CheckDisablePhysicsAnim_PMDPhysicsInfo(	DX_MODELLOADER3_PMD_PHYSICS_INFO *MLPhysicsInfo, int PhysicsIndex ) ;

extern int SetupPhysicsObject_ModelPhysicsInfo(		MV1_MODEL *Model ) ;
extern int ReleasePhysicsObject_ModelPhysicsInfo(	MV1_MODEL *Model ) ;
extern int ResetState_ModelPhysicsInfo(				MV1_MODEL *Model ) ;
extern int StepSimulation_ModelPhysicsInfo(			MV1_MODEL *Model, float TimeStep ) ;
extern int SetWorldGravity_ModelPhysiceInfo(		MV1_MODEL *Model, VECTOR Gravity ) ;

#endif

#endif
#endif
