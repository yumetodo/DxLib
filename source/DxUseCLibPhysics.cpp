// -------------------------------------------------------------------------------
// 
// 		�c�w���C�u����		�������Z����
// 
// 				Ver 3.16d
// 
// -------------------------------------------------------------------------------

// �c�w���C�u�����쐬���p��`
#define __DX_MAKE

#include "DxCompileConfig.h"

#if !defined( DX_NON_MODEL ) && !defined( DX_NON_BULLET_PHYSICS )

// �C���N���[�h ------------------------------------------------------------------
#include "DxUseCLib.h"
#include "DxLib.h"
#include "DxStatic.h"
#include "DxModel.h"
#include "DxModelRead.h"
#include "btBulletDynamicsCommon.h"
#include "DxModelLoader3.h"
#include "DxModelLoader4.h"

//namespace DxLib
//{

// �}�N����` -----------------------------------

// �f�[�^�^�錾 ---------------------------------

// Bullet �̍��̂�����������ۂɎg�p������
struct BULLET_RIGIDBODY_SETUP_INFO
{
	int					RigidBodyGroupIndex ;	// ���̃O���[�v�ԍ�
	DWORD				RigidBodyGroupTarget ;	// ���̃O���[�v�Ώ�
	int					ShapeType ;				// �`��( 0:��  1:��  2:�J�v�Z�� )
	float				ShapeW ;				// ��
	float				ShapeH ;				// ����
	float				ShapeD ;				// ���s
	VECTOR				Position ;				// �ʒu
	VECTOR				Rotation ;				// ��]
	float				RigidBodyWeight ;		// ����
	float				RigidBodyPosDim ;		// �ړ���
	float				RigidBodyRotDim ;		// ��]��
	float				RigidBodyRecoil ;		// ������
	float				RigidBodyFriction ;		// ���C��
	int					RigidBodyType ;			// ���̃^�C�v( 0:Bone�Ǐ]  1:�������Z  2:�������Z(Bone�ʒu���킹) )
	MATRIX				*InitializeMatrix ;		// �s��̏����l
} ;

// Bullet �̍��̃W���C���g������������ۂɎg�p������
struct BULLET_JOINT_SETUP_INFO
{
	D_btRigidBody		*RigidBodyA ;			// �ڑ��捄�̂`
	D_btRigidBody		*RigidBodyB ;			// �ڑ��捄�̂a
	VECTOR				Position ;				// �ʒu
	VECTOR				Rotation ;				// ��]( ���W�A�� )
	VECTOR				ConstrainPosition1 ;	// �ړ������l�P
	VECTOR				ConstrainPosition2 ;	// �ړ������l�Q
	VECTOR				ConstrainRotation1 ;	// ��]�����l�P
	VECTOR				ConstrainRotation2 ;	// ��]�����l�Q
	VECTOR				SpringPosition ;		// �΂ˈړ��l
	VECTOR				SpringRotation ;		// �΂ˉ�]�l
} ;

// Bullet �̍��̏����Ŏg�p����f�[�^
struct BULLET_RIGIDBODY_INFO
{
	int					DisableFlag ;			// �����t���O

	D_btCollisionShape	*btColShape ;			// �`��f�[�^
	D_btRigidBody		*btRigdBody ;			// ���̃f�[�^

	D_btTransform		bttrBoneOffset ;		// �{�[���̃I�t�Z�b�g
	D_btTransform		bttrInvBoneOffset ;		// �{�[���̋t�I�t�Z�b�g
	struct D_btKinematicMotionState *btMotionState ;
} ;

// Bullet �̍��̃W���C���g�����Ŏg�p����f�[�^
struct BULLET_JOINT_INFO
{
	int									DisableFlag ;			// �����t���O
	D_btGeneric6DofSpringConstraint	*	btcConstraint ;
} ;

// Bullet �� Kinematic�p���[�V�����X�e�[�g
struct D_btKinematicMotionState : public D_btMotionState
{
	D_btTransform			GraphicsWorldTrans ;
/*
	D_btTransform			BoneOffset ;
//	D_btTransform			InvBoneOffset ;
	PMD_READ_BONE_INFO		*Bone ;
	PMD_READ_PHYSICS_INFO	*Physics ;

	D_btKinematicMotionState(
		const D_btTransform	&boneOffset,
		PMD_READ_BONE_INFO	*pBone,
		PMD_READ_PHYSICS_INFO *pPhysics )
		:
		BoneOffset( boneOffset ),
		Bone( pBone ),
		Physics( pPhysics )
	{
//		InvBoneOffset = BoneOffset.inverse() ;
//		Flush( true ) ;
	}
*/
	D_btKinematicMotionState(){};

#if 0
	void Flush(bool reset)
	{
		if( Physics->RigidBodyType == 0 || Physics->NoCopyToBone || reset)
		{
			D_btTransform temp ;

			temp.setFromOpenGLMatrix( ( D_btScalar * )&Bone->LocalWorldMatrix ) ;
			GraphicsWorldTrans = temp * BoneOffset ;
		}
/*		else
		{
			D_btTransform temp2 ;

			temp2 = GraphicsWorldTrans * InvBoneOffset ;

			temp2.getOpenGLMatrix( ( D_btScalar * )&Bone->LocalWorldMatrix ) ;
		}
*/	}
#endif

	///synchronizes world transform from user to physics
	virtual void getWorldTransform( D_btTransform &centerOfMassWorldTrans ) const
	{
		centerOfMassWorldTrans = GraphicsWorldTrans ;
	}

	///synchronizes world transform from physics to user
	///Bullet only calls the update of worldtransform for active objects
	virtual void setWorldTransform( const D_btTransform &centerOfMassWorldTrans )
	{
		GraphicsWorldTrans = centerOfMassWorldTrans ;
	}
} ;

// bullet physics �̏����ɕK�v�ȃI�u�W�F�N�g���l�߂��\����
struct BULLET_PHYSICS
{
	D_btDefaultCollisionConfiguration		*CollisionConfig ;			// �R���W�����R���t�B�O
	D_btCollisionDispatcher					*CollisionDispatcher ;		// �R���W�����f�B�X�p�b�`��
	D_btAxisSweep3							*OverlappingPairCache ;		// �R���W�������[���h�͈̔�
	D_btSequentialImpulseConstraintSolver	*Solver ;					// �����v�Z�\���o
	D_btDiscreteDynamicsWorld				*World ;					// ���[���h
//	D_btContinuousDynamicsWorld				*World ;					// ���[���h

	D_btCollisionShape						*GroundShape ;				// ���R���W����
} ;

// �֐��錾 -------------------------------------

static int BulletPhysics_Initialize( BULLET_PHYSICS *BulletPhysicsData, VECTOR Gravity ) ;		// BulletPhysics �̏�����
static int BulletPhysics_Terminate( BULLET_PHYSICS *BulletPhysicsData ) ;						// BulletPhysics �̌�n��

static int BulletPhysics_SetupRigidBody(	BULLET_PHYSICS *BulletPhysicsData,  BULLET_RIGIDBODY_INFO *RigidBodyInfo,	BULLET_RIGIDBODY_SETUP_INFO	*SetupInfo ) ;
static int BulletPhysics_SetupJoint(		BULLET_PHYSICS *BulletPhysicsData,  BULLET_JOINT_INFO	  *JointInfo,		BULLET_JOINT_SETUP_INFO		*SetupInfo ) ;
static int BulletPhysics_ReleaseRigidBody(	BULLET_RIGIDBODY_INFO *RigidBodyInfo ) ;
static int BulletPhysics_ReleaseJoint(		BULLET_JOINT_INFO	  *JointInfo ) ;
static int BulleyPhysics_ResetRigidBody(	BULLET_RIGIDBODY_INFO *RigidBodyInfo, MATRIX *Matrix ) ;

// ���������肵�����������ȋt�s��v�Z
static void ModelLoader3_InverseMatrix( MATRIX &InMatrix, MATRIX &OutMatrix ) ;

// PMD�p�̍��̂Ƀ{�[���̏�Ԃ𔽉f����֐�
static void PMD_PhysicsMotionState_Flush( D_btKinematicMotionState *pbtMotionState, PMD_READ_PHYSICS_INFO *pPhysics ) ;

// ���f���̍��̂Ƀ{�[���̏�Ԃ𔽉f����֐�
static void Model_PhysicsMotionState_Flush( D_btKinematicMotionState *pbtMotionState, MV1_PHYSICS_RIGIDBODY *pRigidBody ) ;

// �v���O���� -----------------------------------

// BulletPhysics �̏�����
static int BulletPhysics_Initialize( BULLET_PHYSICS *BulletPhysicsData, VECTOR Gravity )
{
	D_btTransform trGroundTransform ;

	// �[��������
	_MEMSET( BulletPhysicsData, 0, sizeof( *BulletPhysicsData ) ) ;

	// �R���W�����R���t�B�O���쐬����
	BulletPhysicsData->CollisionConfig = new D_btDefaultCollisionConfiguration() ;

	// �R���W�����f�B�X�p�b�`�����쐬����
	BulletPhysicsData->CollisionDispatcher = new D_btCollisionDispatcher( BulletPhysicsData->CollisionConfig ) ;

	// �R���W�������[���h�̍ő�T�C�Y���w�肷��
	BulletPhysicsData->OverlappingPairCache = new D_btAxisSweep3( D_btVector3( -50000.0f, -50000.0f, -50000.0f ), D_btVector3(  50000.0f,  50000.0f,  50000.0f ), 2048 ) ;

	// �S���v�Z�\���o���쐬����
	BulletPhysicsData->Solver = new D_btSequentialImpulseConstraintSolver() ;

	// ���[���h�̍쐬
	BulletPhysicsData->World = new D_btDiscreteDynamicsWorld( BulletPhysicsData->CollisionDispatcher, BulletPhysicsData->OverlappingPairCache, BulletPhysicsData->Solver, BulletPhysicsData->CollisionConfig ) ;
//	BulletPhysicsData->World = new D_btContinuousDynamicsWorld( BulletPhysicsData->CollisionDispatcher, BulletPhysicsData->OverlappingPairCache, BulletPhysicsData->Solver, BulletPhysicsData->CollisionConfig ) ;

	// �d�͐ݒ�
	BulletPhysicsData->World->setGravity( D_btVector3( Gravity.x, Gravity.y, Gravity.z ) ) ;

	//-----------------------------------------------------
	// ���p�Ƃ��Ė������ʂ��쐬
//	BulletPhysicsData->GroundShape = new D_btStaticPlaneShape( D_btVector3( 0.0f, 1.0f, 0.0f ), 0.0f ) ;

	// ���̃g�����X�t�H�[����ݒ�
//	trGroundTransform.setIdentity() ;

	// MotionState���쐬����B���̂̎p��������������
//	D_btMotionState *pMotionState = new D_btDefaultMotionState( trGroundTransform ) ;

	// ���̂��쐬����
	// ���� 0.0�A�����e���\�� 0.0 �Ȃ炱�̍��͓̂����Ȃ�
//	D_btRigidBody::D_btRigidBodyConstructionInfo rbInfo( 0.0f, pMotionState, BulletPhysicsData->GroundShape, D_btVector3( 0.0f, 0.0f, 0.0f ) ) ;

	// �������[���h�ɏ���ǉ�
//	BulletPhysicsData->World->addRigidBody( new D_btRigidBody( rbInfo ) ) ;

	return 0 ;
}

// BulletPhysics �̌�n��
static int BulletPhysics_Terminate( BULLET_PHYSICS *BulletPhysicsData )
{
	int i ;
	D_btCollisionObject *pObj ;
	D_btTypedConstraint *pConst ;
	D_btRigidBody *pRigidBody ;

	if( BulletPhysicsData->World == NULL )
		return -1 ;

	for( i = BulletPhysicsData->World->getNumConstraints() - 1 ; i >= 0 ; i -- )
	{
		pConst = BulletPhysicsData->World->getConstraint( i ) ;
		BulletPhysicsData->World->removeConstraint( pConst ) ;

		delete pConst ;
	}

	for( i = BulletPhysicsData->World->getNumCollisionObjects() - 1 ; i >= 0 ; i -- )
	{
		pObj = BulletPhysicsData->World->getCollisionObjectArray()[ i ] ;
		pRigidBody = D_btRigidBody::upcast( pObj ) ;

		if( pRigidBody && pRigidBody->getMotionState() )
			delete pRigidBody->getMotionState() ;

		BulletPhysicsData->World->removeCollisionObject( pObj ) ;

		delete pObj ;
	}

	if( BulletPhysicsData->GroundShape )
	{
		delete BulletPhysicsData->GroundShape ;
		BulletPhysicsData->GroundShape = NULL ;
	}

	if( BulletPhysicsData->World )
	{
		delete BulletPhysicsData->World ;
		BulletPhysicsData->World = NULL ;
	}

	if( BulletPhysicsData->Solver )
	{
		delete BulletPhysicsData->Solver ;
		BulletPhysicsData->Solver = NULL ;
	}

	if( BulletPhysicsData->OverlappingPairCache )
	{
		delete BulletPhysicsData->OverlappingPairCache ;
		BulletPhysicsData->OverlappingPairCache = NULL ;
	}

	if( BulletPhysicsData->CollisionDispatcher )
	{
		delete BulletPhysicsData->CollisionDispatcher ;
		BulletPhysicsData->CollisionDispatcher = NULL ;
	}

	if( BulletPhysicsData->CollisionConfig )
	{
		delete BulletPhysicsData->CollisionConfig ;
		BulletPhysicsData->CollisionConfig = NULL ;
	}

	return 0 ;
}

static int BulletPhysics_SetupRigidBody( BULLET_PHYSICS *BulletPhysicsData, BULLET_RIGIDBODY_INFO *RigidBodyInfo, BULLET_RIGIDBODY_SETUP_INFO *SetupInfo )
{
	RigidBodyInfo->DisableFlag = FALSE ;

	// �{�[���I�t�Z�b�g�p�g�����X�t�H�[���쐬
	D_btMatrix3x3	btmRotationMat ;
	btmRotationMat.setEulerZYX( SetupInfo->Rotation.x, SetupInfo->Rotation.y, SetupInfo->Rotation.z ) ;
	RigidBodyInfo->bttrBoneOffset.setIdentity() ;
	RigidBodyInfo->bttrBoneOffset.setOrigin( D_btVector3( SetupInfo->Position.x, SetupInfo->Position.y, SetupInfo->Position.z ) ) ;
	RigidBodyInfo->bttrBoneOffset.setBasis( btmRotationMat ) ;
	RigidBodyInfo->bttrInvBoneOffset = RigidBodyInfo->bttrBoneOffset.inverse() ;

	// �V�F�C�v�̍쐬
	switch( SetupInfo->ShapeType )
	{
	case 0 : RigidBodyInfo->btColShape = new D_btSphereShape( SetupInfo->ShapeW ) ; break ;														// ��
	case 1 : RigidBodyInfo->btColShape = new D_btBoxShape( D_btVector3( SetupInfo->ShapeW, SetupInfo->ShapeH, SetupInfo->ShapeD ) ) ; break ;	// ��
	case 2 : RigidBodyInfo->btColShape = new D_btCapsuleShape( SetupInfo->ShapeW, SetupInfo->ShapeH ) ; break ;									// �J�v�Z��
	}

	// ���ʂƊ����e���\���̐ݒ�
	D_btScalar	btsMass( 0.0f ) ;
	D_btVector3	btv3LocalInertia( 0.0f, 0.0f ,0.0f ) ;

	// �{�[���Ǐ]�łȂ��ꍇ�͎��ʂ�ݒ�
	if( SetupInfo->RigidBodyType != 0 )
		btsMass = SetupInfo->RigidBodyWeight ;

	// �����e���\���̌v�Z
	if( SetupInfo->RigidBodyType != 0 )
		RigidBodyInfo->btColShape->calculateLocalInertia( btsMass, btv3LocalInertia ) ;

	// MotionState�̍쐬
	D_btMotionState *pbtMotionState ;
	pbtMotionState = new D_btKinematicMotionState() ;
	RigidBodyInfo->btMotionState = ( D_btKinematicMotionState * )pbtMotionState ;

	// ���[�V�����X�e�[�g�̒��̍s���������
	D_btTransform temp ;
	temp.setFromOpenGLMatrix( ( D_btScalar * )SetupInfo->InitializeMatrix ) ;
	RigidBodyInfo->btMotionState->GraphicsWorldTrans = temp * RigidBodyInfo->bttrBoneOffset ;

	// ���̂̃p�����[�^�̐ݒ�
	D_btRigidBody::D_btRigidBodyConstructionInfo btRbInfo( btsMass, pbtMotionState, RigidBodyInfo->btColShape, btv3LocalInertia ) ;
	btRbInfo.m_linearDamping  = SetupInfo->RigidBodyPosDim ;	// �ړ���
	btRbInfo.m_angularDamping = SetupInfo->RigidBodyRotDim ;	// ��]��
	btRbInfo.m_restitution    = SetupInfo->RigidBodyRecoil ;	// ������
	btRbInfo.m_friction       = SetupInfo->RigidBodyFriction ;	// ���C��
	btRbInfo.m_additionalDamping = true ;

	// ���̂̍쐬
	RigidBodyInfo->btRigdBody = new D_btRigidBody( btRbInfo ) ;

	// Kinematic�ݒ�
	if( SetupInfo->RigidBodyType == 0 )
	{
		RigidBodyInfo->btRigdBody->setCollisionFlags( RigidBodyInfo->btRigdBody->getCollisionFlags() | D_btCollisionObject::D_CF_KINEMATIC_OBJECT ) ;
		RigidBodyInfo->btRigdBody->setActivationState( D_DISABLE_DEACTIVATION ) ;
	}
	RigidBodyInfo->btRigdBody->setSleepingThresholds( 0.0f, 0.0f ) ;

	// ���̂��V�~�����[�V�������[���h�ɒǉ�
	BulletPhysicsData->World->addRigidBody( RigidBodyInfo->btRigdBody, ( short )( 0x0001 << SetupInfo->RigidBodyGroupIndex ), ( short )SetupInfo->RigidBodyGroupTarget ) ;

	// �I��
	return 0 ;
}

static int BulletPhysics_SetupJoint( BULLET_PHYSICS *BulletPhysicsData,  BULLET_JOINT_INFO *JointInfo, BULLET_JOINT_SETUP_INFO *SetupInfo )
{
	JointInfo->DisableFlag = FALSE ;

	// ��]�s��쐬
	D_btMatrix3x3	btmRotationMat ;
	btmRotationMat.setEulerZYX( SetupInfo->Rotation.x, SetupInfo->Rotation.y, SetupInfo->Rotation.z ) ;

	// �R���X�g���C���g�̃g�����X�t�H�[�����쐬
	D_btTransform bttrTransform ;
	bttrTransform.setIdentity() ;
	bttrTransform.setOrigin( D_btVector3( SetupInfo->Position.x, SetupInfo->Position.y, SetupInfo->Position.z ) ) ;
	bttrTransform.setBasis( btmRotationMat ) ;

	// ����A,B���猩���R���X�g���C���g���쐬 
	JointInfo->btcConstraint = new D_btGeneric6DofSpringConstraint(
		*SetupInfo->RigidBodyA,
		*SetupInfo->RigidBodyB,
		SetupInfo->RigidBodyA->getWorldTransform().inverse() * bttrTransform,
		SetupInfo->RigidBodyB->getWorldTransform().inverse() * bttrTransform, true ) ;

	// �e�퐧���p�����[�^�̃Z�b�g
	JointInfo->btcConstraint->setLinearLowerLimit( D_btVector3( SetupInfo->ConstrainPosition1.x, SetupInfo->ConstrainPosition1.y, SetupInfo->ConstrainPosition1.z ) ) ;
	JointInfo->btcConstraint->setLinearUpperLimit( D_btVector3( SetupInfo->ConstrainPosition2.x, SetupInfo->ConstrainPosition2.y, SetupInfo->ConstrainPosition2.z ) ) ;

	JointInfo->btcConstraint->setAngularLowerLimit( D_btVector3( SetupInfo->ConstrainRotation1.x, SetupInfo->ConstrainRotation1.y, SetupInfo->ConstrainRotation1.z ) ) ;
	JointInfo->btcConstraint->setAngularUpperLimit( D_btVector3( SetupInfo->ConstrainRotation2.x, SetupInfo->ConstrainRotation2.y, SetupInfo->ConstrainRotation2.z ) ) ;

	// 0 : translation X
	if( SetupInfo->SpringPosition.x != 0.0f )
	{
		JointInfo->btcConstraint->enableSpring( 0, true ) ;
		JointInfo->btcConstraint->setStiffness( 0, SetupInfo->SpringPosition.x ) ;
	}

	// 1 : translation Y
	if( SetupInfo->SpringPosition.y != 0.0f )
	{
		JointInfo->btcConstraint->enableSpring( 1, true ) ;
		JointInfo->btcConstraint->setStiffness( 1, SetupInfo->SpringPosition.y ) ;
	}

	// 2 : translation Z
	if( SetupInfo->SpringPosition.z != 0.0f )
	{
		JointInfo->btcConstraint->enableSpring( 2, true ) ;
		JointInfo->btcConstraint->setStiffness( 2, SetupInfo->SpringPosition.z ) ;
	}

	// 3 : rotation X (3rd Euler rotational around new position of X axis, range [-PI+epsilon, PI-epsilon] )
	// 4 : rotation Y (2nd Euler rotational around new position of Y axis, range [-PI/2+epsilon, PI/2-epsilon] )
	// 5 : rotation Z (1st Euler rotational around Z axis, range [-PI+epsilon, PI-epsilon] )
	JointInfo->btcConstraint->enableSpring( 3, true ) ; JointInfo->btcConstraint->setStiffness( 3, SetupInfo->SpringRotation.x ) ;
	JointInfo->btcConstraint->enableSpring( 4, true ) ; JointInfo->btcConstraint->setStiffness( 4, SetupInfo->SpringRotation.y ) ;
	JointInfo->btcConstraint->enableSpring( 5, true ) ; JointInfo->btcConstraint->setStiffness( 5, SetupInfo->SpringRotation.z ) ;

	JointInfo->btcConstraint->calculateTransforms() ;
	JointInfo->btcConstraint->setEquilibriumPoint() ;

	// �V�~�����[�V�������[���h�ɒǉ�
	BulletPhysicsData->World->addConstraint( JointInfo->btcConstraint ) ;

	// �I��
	return 0 ;
}

static int BulletPhysics_ReleaseRigidBody( BULLET_RIGIDBODY_INFO *RigidBodyInfo )
{
	if( RigidBodyInfo->DisableFlag )
	{
		return 0 ;
	}

	if( RigidBodyInfo->btColShape )
	{
		delete RigidBodyInfo->btColShape ;
		RigidBodyInfo->btColShape = NULL ;
	}

	RigidBodyInfo->btMotionState = NULL ;
	RigidBodyInfo->btRigdBody = NULL ;

	// �I��
	return 0 ;
}

static int BulletPhysics_ReleaseJoint( BULLET_JOINT_INFO *JointInfo )
{
	if( JointInfo->DisableFlag )
	{
		return 0 ;
	}

	JointInfo->btcConstraint = NULL ;

	// �I��
	return 0 ;
}

static int BulleyPhysics_ResetRigidBody( BULLET_RIGIDBODY_INFO *RigidBodyInfo, MATRIX *Matrix )
{
	D_btTransform	bttrRbTransform ;

	if( RigidBodyInfo->DisableFlag )
	{
		return 0 ;
	}

	bttrRbTransform.setFromOpenGLMatrix( ( D_btScalar * )Matrix ) ;
	RigidBodyInfo->btMotionState->GraphicsWorldTrans = bttrRbTransform * RigidBodyInfo->bttrBoneOffset ;
	RigidBodyInfo->btRigdBody->setCenterOfMassTransform( RigidBodyInfo->btMotionState->GraphicsWorldTrans ) ;

	RigidBodyInfo->btRigdBody->setInterpolationWorldTransform( RigidBodyInfo->btRigdBody->getCenterOfMassTransform() ) ;
	RigidBodyInfo->btRigdBody->setLinearVelocity(               D_btVector3( 0.0f, 0.0f, 0.0f ) ) ;
	RigidBodyInfo->btRigdBody->setAngularVelocity(              D_btVector3( 0.0f, 0.0f, 0.0f ) ) ;
	RigidBodyInfo->btRigdBody->setInterpolationLinearVelocity(  D_btVector3( 0.0f, 0.0f, 0.0f ) ) ;
	RigidBodyInfo->btRigdBody->setInterpolationAngularVelocity( D_btVector3( 0.0f, 0.0f, 0.0f ) ) ;
	RigidBodyInfo->btRigdBody->clearForces() ;

	// �I��
	return 0 ;
}


// ���������肵�����������ȋt�s��v�Z
static void ModelLoader3_InverseMatrix( MATRIX &InMatrix, MATRIX &OutMatrix )
{
	OutMatrix.m[ 0 ][ 0 ] = InMatrix.m[ 0 ][ 0 ] ;
	OutMatrix.m[ 0 ][ 1 ] = InMatrix.m[ 1 ][ 0 ] ;
	OutMatrix.m[ 0 ][ 2 ] = InMatrix.m[ 2 ][ 0 ] ;
	OutMatrix.m[ 0 ][ 3 ] = 0.0f ;
	OutMatrix.m[ 1 ][ 0 ] = InMatrix.m[ 0 ][ 1 ] ;
	OutMatrix.m[ 1 ][ 1 ] = InMatrix.m[ 1 ][ 1 ] ;
	OutMatrix.m[ 1 ][ 2 ] = InMatrix.m[ 2 ][ 1 ] ;
	OutMatrix.m[ 1 ][ 3 ] = 0.0f ;
	OutMatrix.m[ 2 ][ 0 ] = InMatrix.m[ 0 ][ 2 ] ;
	OutMatrix.m[ 2 ][ 1 ] = InMatrix.m[ 1 ][ 2 ] ;
	OutMatrix.m[ 2 ][ 2 ] = InMatrix.m[ 2 ][ 2 ] ;
	OutMatrix.m[ 2 ][ 3 ] = 0.0f ;

	OutMatrix.m[ 3 ][ 0 ] = -( InMatrix.m[ 3 ][ 0 ] * OutMatrix.m[0][0] + InMatrix.m[ 3 ][ 1 ] * OutMatrix.m[1][0] + InMatrix.m[ 3 ][ 2 ] * OutMatrix.m[2][0] ) ;
	OutMatrix.m[ 3 ][ 1 ] = -( InMatrix.m[ 3 ][ 0 ] * OutMatrix.m[0][1] + InMatrix.m[ 3 ][ 1 ] * OutMatrix.m[1][1] + InMatrix.m[ 3 ][ 2 ] * OutMatrix.m[2][1] ) ;
	OutMatrix.m[ 3 ][ 2 ] = -( InMatrix.m[ 3 ][ 0 ] * OutMatrix.m[0][2] + InMatrix.m[ 3 ][ 1 ] * OutMatrix.m[1][2] + InMatrix.m[ 3 ][ 2 ] * OutMatrix.m[2][2] ) ;
	OutMatrix.m[ 3 ][ 3 ] = 1.0f ;
}

// PMD�p�̍��̂Ƀ{�[���̏�Ԃ𔽉f����֐�
static void PMD_PhysicsMotionState_Flush( bool Reset, D_btKinematicMotionState *pbtMotionState, PMD_READ_PHYSICS_INFO *pPhysics )
{
	BULLET_RIGIDBODY_INFO *BulletRigidBodyInfo = ( BULLET_RIGIDBODY_INFO * )pPhysics->BulletInfo ;

	if( pPhysics->RigidBodyType == 0 || pPhysics->NoCopyToBone || Reset )
	{
		D_btTransform temp ;

		temp.setFromOpenGLMatrix( ( D_btScalar * )&pPhysics->Bone->LocalWorldMatrix ) ;
		pbtMotionState->GraphicsWorldTrans = temp * BulletRigidBodyInfo->bttrBoneOffset ;
	}
/*	else
	{
		D_btTransform temp2 ;

		temp2 = GraphicsWorldTrans * InvBoneOffset ;

		temp2.getOpenGLMatrix( ( D_btScalar * )&pPhysics->Bone->LocalWorldMatrix ) ;
	}
*/
}

// PMX�p�̍��̂Ƀ{�[���̏�Ԃ𔽉f����֐�
static void PMX_PhysicsMotionState_Flush( bool Reset, D_btKinematicMotionState *pbtMotionState, PMX_READ_PHYSICS_INFO *pPhysics )
{
	BULLET_RIGIDBODY_INFO *BulletRigidBodyInfo = ( BULLET_RIGIDBODY_INFO * )pPhysics->BulletInfo ;

	if( pPhysics->Base->RigidBodyType == 0 || pPhysics->NoCopyToBone || Reset )
	{
		D_btTransform temp ;

		temp.setFromOpenGLMatrix( ( D_btScalar * )&pPhysics->Bone->LocalWorldMatrix ) ;
		pbtMotionState->GraphicsWorldTrans = temp * BulletRigidBodyInfo->bttrBoneOffset ;
	}
/*	else
	{
		D_btTransform temp2 ;

		temp2 = GraphicsWorldTrans * InvBoneOffset ;

		temp2.getOpenGLMatrix( ( D_btScalar * )&pPhysics->Bone->LocalWorldMatrix ) ;
	}
*/
}

// ���f���̍��̂Ƀ{�[���̏�Ԃ𔽉f����֐�
static void Model_PhysicsMotionState_Flush( D_btKinematicMotionState *pbtMotionState, MV1_PHYSICS_RIGIDBODY *pRigidBody )
{
	BULLET_RIGIDBODY_INFO *BulletRigidBodyInfo = ( BULLET_RIGIDBODY_INFO * )pRigidBody->BulletInfo ;
	MATRIX tempMatrix ;

	if( pRigidBody->BaseData->RigidBodyType == 0 || pRigidBody->BaseData->NoCopyToBone )
	{
		D_btTransform temp ;

		ConvertMatrix4x4cToMatrixF( &tempMatrix, &pRigidBody->TargetFrame->LocalWorldMatrix ) ;
		temp.setFromOpenGLMatrix( ( D_btScalar * )&tempMatrix ) ;
		pbtMotionState->GraphicsWorldTrans = temp * BulletRigidBodyInfo->bttrBoneOffset ;
	}
/*	else
	{
		D_btTransform temp2 ;

		temp2 = GraphicsWorldTrans * InvBoneOffset ;

		temp2.getOpenGLMatrix( ( D_btScalar * )&pPhysics->Bone->LocalWorldMatrix ) ;
	}
*/
}


extern int SetupPhysicsObject_ModelPhysicsInfo( MV1_MODEL *Model )
{
	int i ;
	MV1_PHYSICS_RIGIDBODY *RigidBody ;
	MV1_PHYSICS_JOINT *Joint ;
	MV1_PHYSICS_RIGIDBODY_BASE *RigidBodyBase ;
	MV1_PHYSICS_JOINT_BASE *JointBase ;
	BULLET_RIGIDBODY_INFO *BulletRigidBodyInfo ;
	BULLET_JOINT_INFO *BulletJointInfo ;
	BULLET_PHYSICS *Bullet = ( BULLET_PHYSICS * )Model->BulletPhysicsDataBuffer ;
	BULLET_RIGIDBODY_SETUP_INFO RigidBodySetupInfo ;
	BULLET_JOINT_SETUP_INFO JointSetupInfo ;
	MATRIX InitializeMatrix ;

	// BulletPhysics �̏�����
	BulletPhysics_Initialize( Bullet, VGet( 0.0f, Model->PhysicsGravity, 0.0f ) ) ;

	// ���̃I�u�W�F�N�g�̃Z�b�g�A�b�v
	RigidBody = Model->PhysicsRigidBody ;
	for( i = 0 ; i < Model->BaseData->PhysicsRigidBodyNum ; i ++, RigidBody ++ )
	{
		BulletRigidBodyInfo = ( BULLET_RIGIDBODY_INFO * )RigidBody->BulletInfo ;
		RigidBodyBase = RigidBody->BaseData ;

		ConvertMatrix4x4cToMatrixF( &InitializeMatrix, &RigidBody->TargetFrame->LocalWorldMatrix ) ;

		// ���̂̏�����
		RigidBodySetupInfo.RigidBodyGroupIndex = RigidBodyBase->RigidBodyGroupIndex ;
		RigidBodySetupInfo.RigidBodyGroupTarget = RigidBodyBase->RigidBodyGroupTarget ;
		RigidBodySetupInfo.ShapeType = RigidBodyBase->ShapeType ;
		RigidBodySetupInfo.ShapeW = RigidBodyBase->ShapeW ;
		RigidBodySetupInfo.ShapeH = RigidBodyBase->ShapeH ;
		RigidBodySetupInfo.ShapeD = RigidBodyBase->ShapeD ;
		RigidBodySetupInfo.Position = RigidBodyBase->Position ;
		RigidBodySetupInfo.Rotation = RigidBodyBase->Rotation ;
		RigidBodySetupInfo.RigidBodyWeight = RigidBodyBase->RigidBodyWeight ;
		RigidBodySetupInfo.RigidBodyPosDim = RigidBodyBase->RigidBodyPosDim ;
		RigidBodySetupInfo.RigidBodyRotDim = RigidBodyBase->RigidBodyRotDim ;
		RigidBodySetupInfo.RigidBodyRecoil = RigidBodyBase->RigidBodyRecoil ;
		RigidBodySetupInfo.RigidBodyFriction = RigidBodyBase->RigidBodyFriction ;
		RigidBodySetupInfo.RigidBodyType = RigidBodyBase->RigidBodyType ;
		RigidBodySetupInfo.InitializeMatrix = &InitializeMatrix ;
		BulletPhysics_SetupRigidBody( Bullet, BulletRigidBodyInfo, &RigidBodySetupInfo ) ;
	}

	// �W���C���g�I�u�W�F�N�g�̃Z�b�g�A�b�v
	Joint = Model->PhysicsJoint ;
	for( i = 0 ; i < Model->BaseData->PhysicsJointNum ; i ++, Joint ++ )
	{
		BulletJointInfo = ( BULLET_JOINT_INFO * )Joint->BulletInfo ;

		JointBase = Joint->BaseData ;

		// �W���C���g�̏�����
		JointSetupInfo.RigidBodyA = ( ( BULLET_RIGIDBODY_INFO * )Model->PhysicsRigidBody[ JointBase->RigidBodyA->Index ].BulletInfo )->btRigdBody ;
		JointSetupInfo.RigidBodyB = ( ( BULLET_RIGIDBODY_INFO * )Model->PhysicsRigidBody[ JointBase->RigidBodyB->Index ].BulletInfo )->btRigdBody ;
		JointSetupInfo.Position = JointBase->Position ;
		JointSetupInfo.Rotation = JointBase->Rotation ;
		JointSetupInfo.ConstrainPosition1 = JointBase->ConstrainPosition1 ;
		JointSetupInfo.ConstrainPosition2 = JointBase->ConstrainPosition2 ;
		JointSetupInfo.ConstrainRotation1 = JointBase->ConstrainRotation1 ;
		JointSetupInfo.ConstrainRotation2 = JointBase->ConstrainRotation2 ;
		JointSetupInfo.SpringPosition = JointBase->SpringPosition ;
		JointSetupInfo.SpringRotation = JointBase->SpringRotation ;
		BulletPhysics_SetupJoint( Bullet, BulletJointInfo, &JointSetupInfo ) ;
	}

	// �I��
	return 0 ;
}

extern int ReleasePhysicsObject_ModelPhysicsInfo( MV1_MODEL *Model )
{
	int i ;
	BULLET_RIGIDBODY_INFO *BulletRigidBodyInfo ;
	BULLET_JOINT_INFO *BulletJointInfo ;
	MV1_PHYSICS_RIGIDBODY *RigidBody ;
	MV1_PHYSICS_JOINT *Joint ;
	BULLET_PHYSICS *Bullet = ( BULLET_PHYSICS * )Model->BulletPhysicsDataBuffer ;

	// ���̃I�u�W�F�N�g�̌�n��
	RigidBody = Model->PhysicsRigidBody ;
	for( i = 0 ; i < Model->BaseData->PhysicsRigidBodyNum ; i ++, RigidBody ++ )
	{
		BulletRigidBodyInfo = ( BULLET_RIGIDBODY_INFO * )RigidBody->BulletInfo ;

		BulletPhysics_ReleaseRigidBody( BulletRigidBodyInfo ) ;
	}

	// �W���C���g�I�u�W�F�N�g�̌�n��
	Joint = Model->PhysicsJoint ;
	for( i = 0 ; i < Model->BaseData->PhysicsJointNum ; i ++, Joint ++ )
	{
		BulletJointInfo = ( BULLET_JOINT_INFO * )Joint->BulletInfo ;

		BulletPhysics_ReleaseJoint( BulletJointInfo ) ;
	}

	// BulletPhysics �̌�n��
	BulletPhysics_Terminate( Bullet ) ;

	// �I��
	return 0 ;
}

extern int ResetState_ModelPhysicsInfo( MV1_MODEL *Model )
{
	BULLET_RIGIDBODY_INFO *BulletRigidBodyInfo ;
	MV1_PHYSICS_RIGIDBODY *PhysicsRigidBody ;
	int i ;

	PhysicsRigidBody = Model->PhysicsRigidBody ;
	for( i = 0 ; i < Model->BaseData->PhysicsRigidBodyNum ; i ++, PhysicsRigidBody ++ )
	{
		BulletRigidBodyInfo = ( BULLET_RIGIDBODY_INFO * )PhysicsRigidBody->BulletInfo ;

		if( BulletRigidBodyInfo->DisableFlag )
		{
			continue ;
		}

		if( PhysicsRigidBody->TargetFrame->ValidLocalWorldMatrixNM == false )
		{
			PhysicsRigidBody->TargetFrame->ValidLocalWorldMatrixNM = true ;
			ConvertMatrix4x4cToMatrixF( &PhysicsRigidBody->TargetFrame->LocalWorldMatrixNM, &PhysicsRigidBody->TargetFrame->LocalWorldMatrix ) ;
		}

		BulleyPhysics_ResetRigidBody( BulletRigidBodyInfo, &PhysicsRigidBody->TargetFrame->LocalWorldMatrixNM ) ;
	}

	Model->PhysicsResetRequestFlag = TRUE ;

	// �I��
	return 0 ;
}

extern int StepSimulation_ModelPhysicsInfo( MV1_MODEL *Model, float TimeStep )
{
	BULLET_RIGIDBODY_INFO *BulletRigidBodyInfo ;
	MV1_PHYSICS_RIGIDBODY *PhysicsRigidBody ;
	BULLET_PHYSICS *Bullet = ( BULLET_PHYSICS * )Model->BulletPhysicsDataBuffer ;
	MATRIX tempMatrix ;
	int i ;
	float FixedTimeStep ;

	// ���Z�b�g���N�G�X�g������ꍇ�͍ŏ��Ƀ{�[���̈ʒu���킹
	if( Model->PhysicsResetRequestFlag )
	{
		Model->PhysicsResetRequestFlag = FALSE ;

		// �{�[���ʒu���킹
		PhysicsRigidBody = Model->PhysicsRigidBody ;
		for( i = 0 ; i < Model->BaseData->PhysicsRigidBodyNum ; i ++, PhysicsRigidBody ++ )
		{
			BulletRigidBodyInfo = ( BULLET_RIGIDBODY_INFO * )PhysicsRigidBody->BulletInfo ;

			Model_PhysicsMotionState_Flush( BulletRigidBodyInfo->btMotionState, PhysicsRigidBody ) ;
		}
	}

	// �������������s����
	FixedTimeStep = TimeStep < 1.0f / 60.0f ? TimeStep : 1.0f / 60.0f ;
	Bullet->World->stepSimulation( TimeStep, 1, FixedTimeStep ) ;

	// �{�[���ʒu���킹
	PhysicsRigidBody = Model->PhysicsRigidBody ;
	for( i = 0 ; i < Model->BaseData->PhysicsRigidBodyNum ; i ++, PhysicsRigidBody ++ )
	{
		BulletRigidBodyInfo = ( BULLET_RIGIDBODY_INFO * )PhysicsRigidBody->BulletInfo ;

		Model_PhysicsMotionState_Flush( BulletRigidBodyInfo->btMotionState, PhysicsRigidBody ) ;
	}

	// ���݂̍��̂̏�Ԃ�\���s����擾����
	PhysicsRigidBody = Model->PhysicsRigidBody ;
	for( i = 0 ; i < Model->BaseData->PhysicsRigidBodyNum ; i ++, PhysicsRigidBody ++ )
	{
		if( PhysicsRigidBody->BaseData->RigidBodyType == 0 || PhysicsRigidBody->BaseData->NoCopyToBone ) continue ;

		BulletRigidBodyInfo = ( BULLET_RIGIDBODY_INFO * )PhysicsRigidBody->BulletInfo ;

		( BulletRigidBodyInfo->btRigdBody->getCenterOfMassTransform() * BulletRigidBodyInfo->bttrInvBoneOffset ).getOpenGLMatrix( ( float * )&tempMatrix );
		ConvertMatrixFToMatrix4x4c( &PhysicsRigidBody->TargetFrame->LocalWorldMatrix, &tempMatrix ) ;
	}

	// �I��
	return 0 ;
}

extern int SetWorldGravity_ModelPhysiceInfo( MV1_MODEL *Model, VECTOR Gravity )
{
	BULLET_PHYSICS *Bullet = ( BULLET_PHYSICS * )Model->BulletPhysicsDataBuffer ;

	Bullet->World->setGravity( D_btVector3( Gravity.x, Gravity.y, Gravity.z ) ) ;

	// �I��
	return 0 ;
}

extern int SetupPhysicsObject_PMDPhysicsInfo(
	DX_MODELLOADER3_PMD_PHYSICS_INFO *MLPhysicsInfo,
	int DisablePhysicsFile
)
{
	int i ;
	PMD_READ_PHYSICS_INFO *PhysicsInfo ;
	PMD_READ_PHYSICS_JOINT_INFO *JointInfo ;
	BULLET_RIGIDBODY_INFO *BulletRigidBodyInfo ;
	BULLET_JOINT_INFO *BulletJointInfo ;
	PMD_PHYSICS_JOINT *PmdPJoint ;
	BULLET_PHYSICS *Bullet = ( BULLET_PHYSICS * )MLPhysicsInfo->BulletPhysicsDataBuffer ;
	BULLET_RIGIDBODY_SETUP_INFO RigidBodySetupInfo ;
	BULLET_JOINT_SETUP_INFO JointSetupInfo ;

	// BulletPhysics �̏�����
	BulletPhysics_Initialize( Bullet, MLPhysicsInfo->Gravity ) ;

	// ���̃I�u�W�F�N�g�̃Z�b�g�A�b�v
	PhysicsInfo = MLPhysicsInfo->PmdPhysicsInfoDim ;
	for( i = 0 ; i < MLPhysicsInfo->PmdPhysicsNum ; i ++, PhysicsInfo ++ )
	{
		BulletRigidBodyInfo = ( BULLET_RIGIDBODY_INFO * )PhysicsInfo->BulletInfo ;

		// �������`�F�b�N
		BulletRigidBodyInfo->DisableFlag = MV1LoadModelDisablePhysicsNameCheck_ShiftJIS( PhysicsInfo->Name, DisablePhysicsFile ) ;

		// �����̏ꍇ�͏������������s��Ȃ�
		if( BulletRigidBodyInfo->DisableFlag )
		{
			continue ;
		}

		// ���̂̏�����
		RigidBodySetupInfo.RigidBodyGroupIndex = ( int )PhysicsInfo->Base->Data[ 22 ] ;
		RigidBodySetupInfo.RigidBodyGroupTarget = *( ( WORD * )&PhysicsInfo->Base->Data[ 23 ] ) ;
		RigidBodySetupInfo.ShapeType = PhysicsInfo->ShapeType ;
		RigidBodySetupInfo.ShapeW = *( ( float * )&PhysicsInfo->Base->Data[ 26 ] ) ;
		RigidBodySetupInfo.ShapeH = *( ( float * )&PhysicsInfo->Base->Data[ 30 ] ) ;
		RigidBodySetupInfo.ShapeD = *( ( float * )&PhysicsInfo->Base->Data[ 34 ] ) ;
		RigidBodySetupInfo.Position.x = *( ( float * )&PhysicsInfo->Base->Data[ 38 ] ) ;
		RigidBodySetupInfo.Position.y = *( ( float * )&PhysicsInfo->Base->Data[ 42 ] ) ;
		RigidBodySetupInfo.Position.z = *( ( float * )&PhysicsInfo->Base->Data[ 46 ] ) ;
		RigidBodySetupInfo.Rotation.x = *( ( float * )&PhysicsInfo->Base->Data[ 50 ] ) ;
		RigidBodySetupInfo.Rotation.y = *( ( float * )&PhysicsInfo->Base->Data[ 54 ] ) ;
		RigidBodySetupInfo.Rotation.z = *( ( float * )&PhysicsInfo->Base->Data[ 58 ] ) ;
		RigidBodySetupInfo.RigidBodyWeight = *( ( float * )&PhysicsInfo->Base->Data[ 62 ] ) ;
		RigidBodySetupInfo.RigidBodyPosDim = *( ( float * )&PhysicsInfo->Base->Data[ 66 ] ) ;
		RigidBodySetupInfo.RigidBodyRotDim = *( ( float * )&PhysicsInfo->Base->Data[ 70 ] ) ;
		RigidBodySetupInfo.RigidBodyRecoil = *( ( float * )&PhysicsInfo->Base->Data[ 74 ] ) ;
		RigidBodySetupInfo.RigidBodyFriction = *( ( float * )&PhysicsInfo->Base->Data[ 78 ] ) ;
		RigidBodySetupInfo.RigidBodyType = PhysicsInfo->RigidBodyType ;
		RigidBodySetupInfo.InitializeMatrix = &PhysicsInfo->Bone->LocalWorldMatrix ;
		BulletPhysics_SetupRigidBody( Bullet, BulletRigidBodyInfo, &RigidBodySetupInfo ) ;
	}

	// �W���C���g�I�u�W�F�N�g�̃Z�b�g�A�b�v
	JointInfo = MLPhysicsInfo->PmdPhysicsJointInfoDim ;
	for( i = 0 ; i < MLPhysicsInfo->PmdPhysicsJointNum ; i ++, JointInfo ++ )
	{
		BULLET_RIGIDBODY_INFO *BulletRigidBodyInfoA ;
		BULLET_RIGIDBODY_INFO *BulletRigidBodyInfoB ;

		BulletJointInfo = ( BULLET_JOINT_INFO * )JointInfo->BulletInfo ;

		PmdPJoint = JointInfo->Base ;

		// �ǂ��炩�̍��̂��������`�F�b�N
		BulletRigidBodyInfoA = ( BULLET_RIGIDBODY_INFO * )MLPhysicsInfo->PmdPhysicsInfoDim[ PmdPJoint->RigidBodyA ].BulletInfo ;
		BulletRigidBodyInfoB = ( BULLET_RIGIDBODY_INFO * )MLPhysicsInfo->PmdPhysicsInfoDim[ PmdPJoint->RigidBodyB ].BulletInfo ;
		if( BulletRigidBodyInfoA->DisableFlag ||
			BulletRigidBodyInfoB->DisableFlag )
		{
			BulletJointInfo->DisableFlag = TRUE ;
		}
		else
		{
			BulletJointInfo->DisableFlag = FALSE ;
		}

		// �����̏ꍇ�͏������������s��Ȃ�
		if( BulletJointInfo->DisableFlag )
		{
			continue ;
		}

		// �W���C���g�̏�����
		JointSetupInfo.RigidBodyA = BulletRigidBodyInfoA->btRigdBody ;
		JointSetupInfo.RigidBodyB = BulletRigidBodyInfoB->btRigdBody ;
		JointSetupInfo.Position.x = PmdPJoint->Position[ 0 ] ;
		JointSetupInfo.Position.y = PmdPJoint->Position[ 1 ] ;
		JointSetupInfo.Position.z = PmdPJoint->Position[ 2 ] ;
		JointSetupInfo.Rotation.x = PmdPJoint->Rotation[ 0 ] ;
		JointSetupInfo.Rotation.y = PmdPJoint->Rotation[ 1 ] ;
		JointSetupInfo.Rotation.z = PmdPJoint->Rotation[ 2 ] ;
		JointSetupInfo.ConstrainPosition1.x = PmdPJoint->ConstrainPosition1[ 0 ] ;
		JointSetupInfo.ConstrainPosition1.y = PmdPJoint->ConstrainPosition1[ 1 ] ;
		JointSetupInfo.ConstrainPosition1.z = PmdPJoint->ConstrainPosition1[ 2 ] ;
		JointSetupInfo.ConstrainPosition2.x = PmdPJoint->ConstrainPosition2[ 0 ] ;
		JointSetupInfo.ConstrainPosition2.y = PmdPJoint->ConstrainPosition2[ 1 ] ;
		JointSetupInfo.ConstrainPosition2.z = PmdPJoint->ConstrainPosition2[ 2 ] ;
		JointSetupInfo.ConstrainRotation1.x = PmdPJoint->ConstrainRotation1[ 0 ] ;
		JointSetupInfo.ConstrainRotation1.y = PmdPJoint->ConstrainRotation1[ 1 ] ;
		JointSetupInfo.ConstrainRotation1.z = PmdPJoint->ConstrainRotation1[ 2 ] ;
		JointSetupInfo.ConstrainRotation2.x = PmdPJoint->ConstrainRotation2[ 0 ] ;
		JointSetupInfo.ConstrainRotation2.y = PmdPJoint->ConstrainRotation2[ 1 ] ;
		JointSetupInfo.ConstrainRotation2.z = PmdPJoint->ConstrainRotation2[ 2 ] ;
		JointSetupInfo.SpringPosition.x = PmdPJoint->SpringPosition[ 0 ] ;
		JointSetupInfo.SpringPosition.y = PmdPJoint->SpringPosition[ 1 ] ;
		JointSetupInfo.SpringPosition.z = PmdPJoint->SpringPosition[ 2 ] ;
		JointSetupInfo.SpringRotation.x = PmdPJoint->SpringRotation[ 0 ] * DX_PI_F / 180.0f ;
		JointSetupInfo.SpringRotation.y = PmdPJoint->SpringRotation[ 1 ] * DX_PI_F / 180.0f ;
		JointSetupInfo.SpringRotation.z = PmdPJoint->SpringRotation[ 2 ] * DX_PI_F / 180.0f ;
		BulletPhysics_SetupJoint( Bullet, BulletJointInfo, &JointSetupInfo ) ;
	}

	// �I��
	return 0 ;
}

extern int ReleasePhysicsObject_PMDPhysicsInfo(
	DX_MODELLOADER3_PMD_PHYSICS_INFO *MLPhysicsInfo
)
{
	int i ;
	BULLET_RIGIDBODY_INFO *BulletRigidBodyInfo ;
	BULLET_JOINT_INFO *BulletJointInfo ;
	PMD_READ_PHYSICS_INFO *PhysicsInfo ;
	PMD_READ_PHYSICS_JOINT_INFO *JointInfo ;
	BULLET_PHYSICS *Bullet = ( BULLET_PHYSICS * )MLPhysicsInfo->BulletPhysicsDataBuffer ;

	// ���̃I�u�W�F�N�g�̌�n��
	PhysicsInfo = MLPhysicsInfo->PmdPhysicsInfoDim ;
	for( i = 0 ; i < MLPhysicsInfo->PmdPhysicsNum ; i ++, PhysicsInfo ++ )
	{
		BulletRigidBodyInfo = ( BULLET_RIGIDBODY_INFO * )PhysicsInfo->BulletInfo ;

		BulletPhysics_ReleaseRigidBody( BulletRigidBodyInfo ) ;
	}

	// �W���C���g�I�u�W�F�N�g�̌�n��
	JointInfo = MLPhysicsInfo->PmdPhysicsJointInfoDim ;
	for( i = 0 ; i < MLPhysicsInfo->PmdPhysicsJointNum ; i ++, JointInfo ++ )
	{
		BulletJointInfo = ( BULLET_JOINT_INFO * )JointInfo->BulletInfo ;

		BulletPhysics_ReleaseJoint( BulletJointInfo ) ;
	}

	// BulletPhysics �̌�n��
	BulletPhysics_Terminate( Bullet ) ;

	// �I��
	return 0 ;
}

extern int OneFrameProcess_PMDPhysicsInfo(
	DX_MODELLOADER3_PMD_PHYSICS_INFO *MLPhysicsInfo,
	int FrameNo,
	int LoopNo,
	bool FPS60,
	int ValidNextRate,
	int TimeDivNum
)
{
	BULLET_RIGIDBODY_INFO *BulletRigidBodyInfo ;
	BULLET_PHYSICS *Bullet = ( BULLET_PHYSICS * )MLPhysicsInfo->BulletPhysicsDataBuffer ;
	PMD_READ_PHYSICS_INFO *PhysicsInfo ;
	float UnitTime ;
	int j ;
	int k ;

	UnitTime = ( 1 / 60.0f ) / ( float )TimeDivNum ;

	// �ŏ��̃t���[���ŏ����ʒu�ɃZ�b�g
	if( FrameNo == 0 && LoopNo == 0 )
	{
		PhysicsInfo = MLPhysicsInfo->PmdPhysicsInfoDim ;
		for( j = 0 ; j < MLPhysicsInfo->PmdPhysicsNum ; j ++, PhysicsInfo ++ )
		{
			BulletRigidBodyInfo = ( BULLET_RIGIDBODY_INFO * )PhysicsInfo->BulletInfo ;

			if( BulletRigidBodyInfo->DisableFlag )
			{
				continue ;
			}

			BulleyPhysics_ResetRigidBody( BulletRigidBodyInfo, &PhysicsInfo->Bone->LocalWorldMatrix ) ;

			PMD_PhysicsMotionState_Flush( false, BulletRigidBodyInfo->btMotionState, PhysicsInfo ) ;
		}

		// �ŏ��̃t���[���͏�Ԃŗ����������邽�߂ɂR�b����
		{
			int   LoopNum ;

			LoopNum  = 180 * TimeDivNum ;
			for( j = 0 ; j < LoopNum ; j ++ )
			{
				Bullet->World->stepSimulation( UnitTime, 1, UnitTime ) ;

				// �{�[���ʒu���킹
				PhysicsInfo = MLPhysicsInfo->PmdPhysicsInfoDim ;
				for( k = 0 ; k < MLPhysicsInfo->PmdPhysicsNum ; k ++, PhysicsInfo ++ )
				{
					BulletRigidBodyInfo = ( BULLET_RIGIDBODY_INFO * )PhysicsInfo->BulletInfo ;

					if( BulletRigidBodyInfo->DisableFlag )
					{
						continue ;
					}

					PMD_PhysicsMotionState_Flush( false, BulletRigidBodyInfo->btMotionState, PhysicsInfo ) ;
				}
			}
		}
	}
	else
	{
		// �����������P�t���[�������s����
		Bullet->World->stepSimulation( UnitTime, 1, UnitTime ) ;

		// �{�[���ʒu���킹
		PhysicsInfo = MLPhysicsInfo->PmdPhysicsInfoDim ;
		for( j = 0 ; j < MLPhysicsInfo->PmdPhysicsNum ; j ++, PhysicsInfo ++ )
		{
			BulletRigidBodyInfo = ( BULLET_RIGIDBODY_INFO * )PhysicsInfo->BulletInfo ;

			if( BulletRigidBodyInfo->DisableFlag )
			{
				continue ;
			}

			PMD_PhysicsMotionState_Flush( false, BulletRigidBodyInfo->btMotionState, PhysicsInfo ) ;
//			BulletRigidBodyInfo->btMotionState->Flush( false ) ;					
		}
	}

	// ���݂̍��̂̏�Ԃ�\���s����擾����
	PhysicsInfo = MLPhysicsInfo->PmdPhysicsInfoDim ;
	for( j = 0 ; j < MLPhysicsInfo->PmdPhysicsNum ; j ++, PhysicsInfo ++ )
	{
		if( PhysicsInfo->RigidBodyType == 0 || PhysicsInfo->NoCopyToBone ) continue ;

		BulletRigidBodyInfo = ( BULLET_RIGIDBODY_INFO * )PhysicsInfo->BulletInfo ;

		if( BulletRigidBodyInfo->DisableFlag )
		{
			continue ;
		}

		( BulletRigidBodyInfo->btRigdBody->getCenterOfMassTransform() * BulletRigidBodyInfo->bttrInvBoneOffset ).getOpenGLMatrix( ( float * )&PhysicsInfo->Bone->LocalWorldMatrix );
	}

	// �������Z���s���{�[���̍s��L�[���Z�o����
	if( MLPhysicsInfo->LoopMotionFlag == FALSE || LoopNo >= 2 )
	{
		if( ValidNextRate == FALSE && ( FPS60 || ( FPS60 == false && FrameNo % 2 == 0 ) ) )
		{
			int DestIndex ;
			MATRIX TempMatrix ;
			float MulParam = 1.0f ;
			MATRIX *DestMatrix ;

			DestIndex = FPS60 ? FrameNo : FrameNo / 2 ;
			if( MLPhysicsInfo->LoopMotionFlag == TRUE )
			{
				MulParam = 1.0f / ( MLPhysicsInfo->LoopMotionNum - 1 ) ;
			}

			PhysicsInfo = MLPhysicsInfo->PmdPhysicsInfoDim ;
			for( j = 0 ; j < MLPhysicsInfo->PmdPhysicsNum ; j ++, PhysicsInfo ++ )
			{
				BulletRigidBodyInfo = ( BULLET_RIGIDBODY_INFO * )PhysicsInfo->BulletInfo ;

				if( BulletRigidBodyInfo->DisableFlag )
				{
					continue ;
				}

				if( PhysicsInfo->RigidBodyType == 0 || PhysicsInfo->NoCopyToBone ) continue ;

				DestMatrix = &PhysicsInfo->Bone->KeyMatrix[ DestIndex ] ;

				if( PhysicsInfo->Bone->Frame->Parent )
				{
					MATRIX InvParentBoneLWM ;

					// �������Z���瓾����s��̓��[���h�s��Ȃ̂ŁA������{�[���̃��[�J���s��ɕϊ�����
					ModelLoader3_InverseMatrix( ( ( PMD_READ_BONE_INFO * )PhysicsInfo->Bone->Frame->Parent->UserData )->LocalWorldMatrix, InvParentBoneLWM ) ;
					CreateMultiplyMatrix( &TempMatrix, &PhysicsInfo->Bone->LocalWorldMatrix, &InvParentBoneLWM ) ;
				}
				else
				{
					TempMatrix = PhysicsInfo->Bone->LocalWorldMatrix ;
				}

				if( MLPhysicsInfo->LoopMotionFlag == FALSE )
				{
					*DestMatrix = TempMatrix ;
				}
				else
				{
					if( MLPhysicsInfo->MotionTotalFrameNum >= 6 && FrameNo > MLPhysicsInfo->MotionTotalFrameNum - 6 )
					{
						float Rate ;
						MATRIX *FirstMatrix ;

						Rate = ( float )( FrameNo - ( MLPhysicsInfo->MotionTotalFrameNum - 6 ) ) / 6.0f ;
						FirstMatrix = &PhysicsInfo->Bone->KeyMatrix[ 0 ] ;

						DestMatrix->m[ 0 ][ 0 ] = ( FirstMatrix->m[ 0 ][ 0 ] - TempMatrix.m[ 0 ][ 0 ] ) * Rate + TempMatrix.m[ 0 ][ 0 ] ;
						DestMatrix->m[ 0 ][ 1 ] = ( FirstMatrix->m[ 0 ][ 1 ] - TempMatrix.m[ 0 ][ 1 ] ) * Rate + TempMatrix.m[ 0 ][ 1 ] ;
						DestMatrix->m[ 0 ][ 2 ] = ( FirstMatrix->m[ 0 ][ 2 ] - TempMatrix.m[ 0 ][ 2 ] ) * Rate + TempMatrix.m[ 0 ][ 2 ] ;
						DestMatrix->m[ 0 ][ 3 ] = ( FirstMatrix->m[ 0 ][ 3 ] - TempMatrix.m[ 0 ][ 3 ] ) * Rate + TempMatrix.m[ 0 ][ 3 ] ;
						DestMatrix->m[ 1 ][ 0 ] = ( FirstMatrix->m[ 1 ][ 0 ] - TempMatrix.m[ 1 ][ 0 ] ) * Rate + TempMatrix.m[ 1 ][ 0 ] ;
						DestMatrix->m[ 1 ][ 1 ] = ( FirstMatrix->m[ 1 ][ 1 ] - TempMatrix.m[ 1 ][ 1 ] ) * Rate + TempMatrix.m[ 1 ][ 1 ] ;
						DestMatrix->m[ 1 ][ 2 ] = ( FirstMatrix->m[ 1 ][ 2 ] - TempMatrix.m[ 1 ][ 2 ] ) * Rate + TempMatrix.m[ 1 ][ 2 ] ;
						DestMatrix->m[ 1 ][ 3 ] = ( FirstMatrix->m[ 1 ][ 3 ] - TempMatrix.m[ 1 ][ 3 ] ) * Rate + TempMatrix.m[ 1 ][ 3 ] ;
						DestMatrix->m[ 2 ][ 0 ] = ( FirstMatrix->m[ 2 ][ 0 ] - TempMatrix.m[ 2 ][ 0 ] ) * Rate + TempMatrix.m[ 2 ][ 0 ] ;
						DestMatrix->m[ 2 ][ 1 ] = ( FirstMatrix->m[ 2 ][ 1 ] - TempMatrix.m[ 2 ][ 1 ] ) * Rate + TempMatrix.m[ 2 ][ 1 ] ;
						DestMatrix->m[ 2 ][ 2 ] = ( FirstMatrix->m[ 2 ][ 2 ] - TempMatrix.m[ 2 ][ 2 ] ) * Rate + TempMatrix.m[ 2 ][ 2 ] ;
						DestMatrix->m[ 2 ][ 3 ] = ( FirstMatrix->m[ 2 ][ 3 ] - TempMatrix.m[ 2 ][ 3 ] ) * Rate + TempMatrix.m[ 2 ][ 3 ] ;
						DestMatrix->m[ 3 ][ 0 ] = ( FirstMatrix->m[ 3 ][ 0 ] - TempMatrix.m[ 3 ][ 0 ] ) * Rate + TempMatrix.m[ 3 ][ 0 ] ;
						DestMatrix->m[ 3 ][ 1 ] = ( FirstMatrix->m[ 3 ][ 1 ] - TempMatrix.m[ 3 ][ 1 ] ) * Rate + TempMatrix.m[ 3 ][ 1 ] ;
						DestMatrix->m[ 3 ][ 2 ] = ( FirstMatrix->m[ 3 ][ 2 ] - TempMatrix.m[ 3 ][ 2 ] ) * Rate + TempMatrix.m[ 3 ][ 2 ] ;
						DestMatrix->m[ 3 ][ 3 ] = ( FirstMatrix->m[ 3 ][ 3 ] - TempMatrix.m[ 3 ][ 3 ] ) * Rate + TempMatrix.m[ 3 ][ 3 ] ;
					}
					else
					{
						*DestMatrix = TempMatrix ;
					}
				}
			}
		}
	}

	// �I��
	return 0 ;
}

extern int CheckDisablePhysicsAnim_PMDPhysicsInfo(
	DX_MODELLOADER3_PMD_PHYSICS_INFO *MLPhysicsInfo,
	int PhysicsIndex
)
{
	PMD_READ_PHYSICS_INFO *PhysicsInfo = &MLPhysicsInfo->PmdPhysicsInfoDim[ PhysicsIndex ] ;
	BULLET_RIGIDBODY_INFO *BulletRigidBodyInfo = ( BULLET_RIGIDBODY_INFO * )PhysicsInfo->BulletInfo ;

	return BulletRigidBodyInfo->DisableFlag || PhysicsInfo->RigidBodyType == 0 || PhysicsInfo->NoCopyToBone ;
}










extern int SetupPhysicsObject_PMXPhysicsInfo(
	DX_MODELLOADER3_PMX_PHYSICS_INFO *MLPhysicsInfo,
	int DisablePhysicsFile
)
{
	int i ;
	PMX_READ_PHYSICS_INFO *PhysicsInfo ;
	PMX_READ_PHYSICS_JOINT_INFO *JointInfo ;
	BULLET_RIGIDBODY_INFO *BulletRigidBodyInfo ;
	BULLET_JOINT_INFO *BulletJointInfo ;
	PMX_JOINT *PmxPJoint ;
	BULLET_PHYSICS *Bullet = ( BULLET_PHYSICS * )MLPhysicsInfo->BulletPhysicsDataBuffer ;
	BULLET_RIGIDBODY_SETUP_INFO RigidBodySetupInfo ;
	BULLET_JOINT_SETUP_INFO JointSetupInfo ;

	// BulletPhysics �̏�����
	BulletPhysics_Initialize( Bullet, MLPhysicsInfo->Gravity ) ;

	// ���̃I�u�W�F�N�g�̃Z�b�g�A�b�v
	PhysicsInfo = MLPhysicsInfo->PmxPhysicsInfoDim ;
	for( i = 0 ; i < MLPhysicsInfo->PmxPhysicsNum ; i ++, PhysicsInfo ++ )
	{
		BulletRigidBodyInfo = ( BULLET_RIGIDBODY_INFO * )PhysicsInfo->BulletInfo ;

		// �������`�F�b�N
		BulletRigidBodyInfo->DisableFlag = MV1LoadModelDisablePhysicsNameCheck_WCHAR_T( ( WORD * )PhysicsInfo->Base->Name, DisablePhysicsFile ) ;

		// �����̏ꍇ�͏������������s��Ȃ�
		if( BulletRigidBodyInfo->DisableFlag )
		{
			continue ;
		}

		// ���̂̏�����
		RigidBodySetupInfo.RigidBodyGroupIndex = PhysicsInfo->Base->RigidBodyGroupIndex ;
		RigidBodySetupInfo.RigidBodyGroupTarget = PhysicsInfo->Base->RigidBodyGroupTarget ;
		RigidBodySetupInfo.ShapeType = PhysicsInfo->Base->ShapeType ;
		RigidBodySetupInfo.ShapeW = PhysicsInfo->Base->ShapeW ;
		RigidBodySetupInfo.ShapeH = PhysicsInfo->Base->ShapeH ;
		RigidBodySetupInfo.ShapeD = PhysicsInfo->Base->ShapeD ;
		RigidBodySetupInfo.Position.x = PhysicsInfo->Base->Position[ 0 ] - PhysicsInfo->Bone->LocalWorldMatrix.m[ 3 ][ 0 ] ;
		RigidBodySetupInfo.Position.y = PhysicsInfo->Base->Position[ 1 ] - PhysicsInfo->Bone->LocalWorldMatrix.m[ 3 ][ 1 ] ;
		RigidBodySetupInfo.Position.z = PhysicsInfo->Base->Position[ 2 ] - PhysicsInfo->Bone->LocalWorldMatrix.m[ 3 ][ 2 ] ;
		RigidBodySetupInfo.Rotation.x = PhysicsInfo->Base->Rotation[ 0 ] ;
		RigidBodySetupInfo.Rotation.y = PhysicsInfo->Base->Rotation[ 1 ] ;
		RigidBodySetupInfo.Rotation.z = PhysicsInfo->Base->Rotation[ 2 ] ;
		RigidBodySetupInfo.RigidBodyWeight = PhysicsInfo->Base->RigidBodyWeight ;
		RigidBodySetupInfo.RigidBodyPosDim = PhysicsInfo->Base->RigidBodyPosDim ;
		RigidBodySetupInfo.RigidBodyRotDim = PhysicsInfo->Base->RigidBodyRotDim ;
		RigidBodySetupInfo.RigidBodyRecoil = PhysicsInfo->Base->RigidBodyRecoil ;
		RigidBodySetupInfo.RigidBodyFriction = PhysicsInfo->Base->RigidBodyFriction ;
		RigidBodySetupInfo.RigidBodyType = PhysicsInfo->Base->RigidBodyType ;
		RigidBodySetupInfo.InitializeMatrix = &PhysicsInfo->Bone->LocalWorldMatrix ;
		BulletPhysics_SetupRigidBody( Bullet, BulletRigidBodyInfo, &RigidBodySetupInfo ) ;
	}

	// �W���C���g�I�u�W�F�N�g�̃Z�b�g�A�b�v
	JointInfo = MLPhysicsInfo->PmxPhysicsJointInfoDim ;
	for( i = 0 ; i < MLPhysicsInfo->PmxPhysicsJointNum ; i ++, JointInfo ++ )
	{
		BULLET_RIGIDBODY_INFO *BulletRigidBodyInfoA ;
		BULLET_RIGIDBODY_INFO *BulletRigidBodyInfoB ;

		BulletJointInfo = ( BULLET_JOINT_INFO * )JointInfo->BulletInfo ;

		PmxPJoint = JointInfo->Base ;

		// �ǂ��炩�̍��̂��������`�F�b�N
		BulletRigidBodyInfoA = ( BULLET_RIGIDBODY_INFO * )MLPhysicsInfo->PmxPhysicsInfoDim[ PmxPJoint->RigidBodyAIndex ].BulletInfo ;
		BulletRigidBodyInfoB = ( BULLET_RIGIDBODY_INFO * )MLPhysicsInfo->PmxPhysicsInfoDim[ PmxPJoint->RigidBodyBIndex ].BulletInfo ;
		if( BulletRigidBodyInfoA->DisableFlag ||
			BulletRigidBodyInfoB->DisableFlag )
		{
			BulletJointInfo->DisableFlag = TRUE ;
		}
		else
		{
			BulletJointInfo->DisableFlag = FALSE ;
		}

		// �����̏ꍇ�͏������������s��Ȃ�
		if( BulletJointInfo->DisableFlag )
		{
			continue ;
		}

		// �W���C���g�̏�����
		JointSetupInfo.RigidBodyA = BulletRigidBodyInfoA->btRigdBody ;
		JointSetupInfo.RigidBodyB = BulletRigidBodyInfoB->btRigdBody ;
		JointSetupInfo.Position.x = PmxPJoint->Position[ 0 ] ;
		JointSetupInfo.Position.y = PmxPJoint->Position[ 1 ] ;
		JointSetupInfo.Position.z = PmxPJoint->Position[ 2 ] ;
		JointSetupInfo.Rotation.x = PmxPJoint->Rotation[ 0 ] ;
		JointSetupInfo.Rotation.y = PmxPJoint->Rotation[ 1 ] ;
		JointSetupInfo.Rotation.z = PmxPJoint->Rotation[ 2 ] ;
		JointSetupInfo.ConstrainPosition1.x = PmxPJoint->ConstrainPositionMin[ 0 ] ;
		JointSetupInfo.ConstrainPosition1.y = PmxPJoint->ConstrainPositionMin[ 1 ] ;
		JointSetupInfo.ConstrainPosition1.z = PmxPJoint->ConstrainPositionMin[ 2 ] ;
		JointSetupInfo.ConstrainPosition2.x = PmxPJoint->ConstrainPositionMax[ 0 ] ;
		JointSetupInfo.ConstrainPosition2.y = PmxPJoint->ConstrainPositionMax[ 1 ] ;
		JointSetupInfo.ConstrainPosition2.z = PmxPJoint->ConstrainPositionMax[ 2 ] ;
		JointSetupInfo.ConstrainRotation1.x = PmxPJoint->ConstrainRotationMin[ 0 ] ;
		JointSetupInfo.ConstrainRotation1.y = PmxPJoint->ConstrainRotationMin[ 1 ] ;
		JointSetupInfo.ConstrainRotation1.z = PmxPJoint->ConstrainRotationMin[ 2 ] ;
		JointSetupInfo.ConstrainRotation2.x = PmxPJoint->ConstrainRotationMax[ 0 ] ;
		JointSetupInfo.ConstrainRotation2.y = PmxPJoint->ConstrainRotationMax[ 1 ] ;
		JointSetupInfo.ConstrainRotation2.z = PmxPJoint->ConstrainRotationMax[ 2 ] ;
		JointSetupInfo.SpringPosition.x = PmxPJoint->SpringPosition[ 0 ] ;
		JointSetupInfo.SpringPosition.y = PmxPJoint->SpringPosition[ 1 ] ;
		JointSetupInfo.SpringPosition.z = PmxPJoint->SpringPosition[ 2 ] ;
		JointSetupInfo.SpringRotation.x = PmxPJoint->SpringRotation[ 0 ] * DX_PI_F / 180.0f ;
		JointSetupInfo.SpringRotation.y = PmxPJoint->SpringRotation[ 1 ] * DX_PI_F / 180.0f ;
		JointSetupInfo.SpringRotation.z = PmxPJoint->SpringRotation[ 2 ] * DX_PI_F / 180.0f ;
		BulletPhysics_SetupJoint( Bullet, BulletJointInfo, &JointSetupInfo ) ;
	}

	// �I��
	return 0 ;
}

extern int ReleasePhysicsObject_PMXPhysicsInfo(
	DX_MODELLOADER3_PMX_PHYSICS_INFO *MLPhysicsInfo
)
{
	int i ;
	BULLET_RIGIDBODY_INFO *BulletRigidBodyInfo ;
	BULLET_JOINT_INFO *BulletJointInfo ;
	PMX_READ_PHYSICS_INFO *PhysicsInfo ;
	PMX_READ_PHYSICS_JOINT_INFO *JointInfo ;
	BULLET_PHYSICS *Bullet = ( BULLET_PHYSICS * )MLPhysicsInfo->BulletPhysicsDataBuffer ;

	// ���̃I�u�W�F�N�g�̌�n��
	PhysicsInfo = MLPhysicsInfo->PmxPhysicsInfoDim ;
	for( i = 0 ; i < MLPhysicsInfo->PmxPhysicsNum ; i ++, PhysicsInfo ++ )
	{
		BulletRigidBodyInfo = ( BULLET_RIGIDBODY_INFO * )PhysicsInfo->BulletInfo ;

		BulletPhysics_ReleaseRigidBody( BulletRigidBodyInfo ) ;
	}

	// �W���C���g�I�u�W�F�N�g�̌�n��
	JointInfo = MLPhysicsInfo->PmxPhysicsJointInfoDim ;
	for( i = 0 ; i < MLPhysicsInfo->PmxPhysicsJointNum ; i ++, JointInfo ++ )
	{
		BulletJointInfo = ( BULLET_JOINT_INFO * )JointInfo->BulletInfo ;

		BulletPhysics_ReleaseJoint( BulletJointInfo ) ;
	}

	// BulletPhysics �̌�n��
	BulletPhysics_Terminate( Bullet ) ;

	// �I��
	return 0 ;
}

extern int OneFrameProcess_PMXPhysicsInfo(
	DX_MODELLOADER3_PMX_PHYSICS_INFO *MLPhysicsInfo,
	int FrameNo,
	int LoopNo,
	bool FPS60,
	int ValidNextRate,
	int TimeDivNum
)
{
	BULLET_RIGIDBODY_INFO *BulletRigidBodyInfo ;
	BULLET_PHYSICS *Bullet = ( BULLET_PHYSICS * )MLPhysicsInfo->BulletPhysicsDataBuffer ;
	PMX_READ_PHYSICS_INFO *PhysicsInfo ;
	float UnitTime ;
	int j ;
	int k ;

	UnitTime = ( 1 / 60.0f ) / ( float )TimeDivNum ;

	// �ŏ��̃t���[���ŏ����ʒu�ɃZ�b�g
	if( FrameNo == 0 && LoopNo == 0 )
	{
		PhysicsInfo = MLPhysicsInfo->PmxPhysicsInfoDim ;
		for( j = 0 ; j < MLPhysicsInfo->PmxPhysicsNum ; j ++, PhysicsInfo ++ )
		{
			BulletRigidBodyInfo = ( BULLET_RIGIDBODY_INFO * )PhysicsInfo->BulletInfo ;

			if( BulletRigidBodyInfo->DisableFlag )
			{
				continue ;
			}

			BulleyPhysics_ResetRigidBody( BulletRigidBodyInfo, &PhysicsInfo->Bone->LocalWorldMatrix ) ;

			PMX_PhysicsMotionState_Flush( false, BulletRigidBodyInfo->btMotionState, PhysicsInfo ) ;
		}

		// �ŏ��̃t���[���͏�Ԃŗ����������邽�߂ɂR�b����
		{
			int   LoopNum ;

			LoopNum  = 180 * TimeDivNum ;
			for( j = 0 ; j < LoopNum ; j ++ )
			{
				Bullet->World->stepSimulation( UnitTime, 1, UnitTime ) ;

				// �{�[���ʒu���킹
				PhysicsInfo = MLPhysicsInfo->PmxPhysicsInfoDim ;
				for( k = 0 ; k < MLPhysicsInfo->PmxPhysicsNum ; k ++, PhysicsInfo ++ )
				{
					BulletRigidBodyInfo = ( BULLET_RIGIDBODY_INFO * )PhysicsInfo->BulletInfo ;

					if( BulletRigidBodyInfo->DisableFlag )
					{
						continue ;
					}

					PMX_PhysicsMotionState_Flush( false, BulletRigidBodyInfo->btMotionState, PhysicsInfo ) ;
				}
			}
		}
	}
	else
	{
		// �����������P�t���[�������s����
		Bullet->World->stepSimulation( UnitTime, 1, UnitTime ) ;

		// �{�[���ʒu���킹
		PhysicsInfo = MLPhysicsInfo->PmxPhysicsInfoDim ;
		for( j = 0 ; j < MLPhysicsInfo->PmxPhysicsNum ; j ++, PhysicsInfo ++ )
		{
			BulletRigidBodyInfo = ( BULLET_RIGIDBODY_INFO * )PhysicsInfo->BulletInfo ;

			if( BulletRigidBodyInfo->DisableFlag )
			{
				continue ;
			}

			PMX_PhysicsMotionState_Flush( false, BulletRigidBodyInfo->btMotionState, PhysicsInfo ) ;
//			BulletRigidBodyInfo->btMotionState->Flush( false ) ;					
		}
	}

	// ���݂̍��̂̏�Ԃ�\���s����擾����
	PhysicsInfo = MLPhysicsInfo->PmxPhysicsInfoDim ;
	for( j = 0 ; j < MLPhysicsInfo->PmxPhysicsNum ; j ++, PhysicsInfo ++ )
	{
		if( PhysicsInfo->Base->RigidBodyType == 0 || PhysicsInfo->NoCopyToBone ) continue ;

		BulletRigidBodyInfo = ( BULLET_RIGIDBODY_INFO * )PhysicsInfo->BulletInfo ;

		if( BulletRigidBodyInfo->DisableFlag )
		{
			continue ;
		}

		( BulletRigidBodyInfo->btRigdBody->getCenterOfMassTransform() * BulletRigidBodyInfo->bttrInvBoneOffset ).getOpenGLMatrix( ( float * )&PhysicsInfo->Bone->LocalWorldMatrix );
	}

	// �������Z���s���{�[���̍s��L�[���Z�o����
	if( MLPhysicsInfo->LoopMotionFlag == FALSE || LoopNo >= 2 )
	{
		if( ValidNextRate == FALSE && ( FPS60 || ( FPS60 == false && FrameNo % 2 == 0 ) ) )
		{
			int DestIndex ;
			MATRIX TempMatrix ;
			float MulParam = 1.0f ;
			MATRIX *DestMatrix ;

			DestIndex = FPS60 ? FrameNo : FrameNo / 2 ;
			if( MLPhysicsInfo->LoopMotionFlag == TRUE )
			{
				MulParam = 1.0f / ( MLPhysicsInfo->LoopMotionNum - 1 ) ;
			}

			PhysicsInfo = MLPhysicsInfo->PmxPhysicsInfoDim ;
			for( j = 0 ; j < MLPhysicsInfo->PmxPhysicsNum ; j ++, PhysicsInfo ++ )
			{
				BulletRigidBodyInfo = ( BULLET_RIGIDBODY_INFO * )PhysicsInfo->BulletInfo ;

				if( BulletRigidBodyInfo->DisableFlag )
				{
					continue ;
				}

				if( PhysicsInfo->Base->RigidBodyType == 0 || PhysicsInfo->NoCopyToBone ) continue ;

				DestMatrix = &PhysicsInfo->Bone->KeyMatrix[ DestIndex ] ;

				if( PhysicsInfo->Bone->Frame->Parent )
				{
					MATRIX InvParentBoneLWM ;

					// �������Z���瓾����s��̓��[���h�s��Ȃ̂ŁA������{�[���̃��[�J���s��ɕϊ�����
					ModelLoader3_InverseMatrix( ( ( PMX_READ_BONE_INFO * )PhysicsInfo->Bone->Frame->Parent->UserData )->LocalWorldMatrix, InvParentBoneLWM ) ;
					CreateMultiplyMatrix( &TempMatrix, &PhysicsInfo->Bone->LocalWorldMatrix, &InvParentBoneLWM ) ;
				}
				else
				{
					TempMatrix = PhysicsInfo->Bone->LocalWorldMatrix ;
				}

				if( MLPhysicsInfo->LoopMotionFlag == FALSE )
				{
					*DestMatrix = TempMatrix ;
				}
				else
				{
					if( MLPhysicsInfo->MotionTotalFrameNum >= 6 && FrameNo > MLPhysicsInfo->MotionTotalFrameNum - 6 )
					{
						float Rate ;
						MATRIX *FirstMatrix ;

						Rate = ( float )( FrameNo - ( MLPhysicsInfo->MotionTotalFrameNum - 6 ) ) / 6.0f ;
						FirstMatrix = &PhysicsInfo->Bone->KeyMatrix[ 0 ] ;

						DestMatrix->m[ 0 ][ 0 ] = ( FirstMatrix->m[ 0 ][ 0 ] - TempMatrix.m[ 0 ][ 0 ] ) * Rate + TempMatrix.m[ 0 ][ 0 ] ;
						DestMatrix->m[ 0 ][ 1 ] = ( FirstMatrix->m[ 0 ][ 1 ] - TempMatrix.m[ 0 ][ 1 ] ) * Rate + TempMatrix.m[ 0 ][ 1 ] ;
						DestMatrix->m[ 0 ][ 2 ] = ( FirstMatrix->m[ 0 ][ 2 ] - TempMatrix.m[ 0 ][ 2 ] ) * Rate + TempMatrix.m[ 0 ][ 2 ] ;
						DestMatrix->m[ 0 ][ 3 ] = ( FirstMatrix->m[ 0 ][ 3 ] - TempMatrix.m[ 0 ][ 3 ] ) * Rate + TempMatrix.m[ 0 ][ 3 ] ;
						DestMatrix->m[ 1 ][ 0 ] = ( FirstMatrix->m[ 1 ][ 0 ] - TempMatrix.m[ 1 ][ 0 ] ) * Rate + TempMatrix.m[ 1 ][ 0 ] ;
						DestMatrix->m[ 1 ][ 1 ] = ( FirstMatrix->m[ 1 ][ 1 ] - TempMatrix.m[ 1 ][ 1 ] ) * Rate + TempMatrix.m[ 1 ][ 1 ] ;
						DestMatrix->m[ 1 ][ 2 ] = ( FirstMatrix->m[ 1 ][ 2 ] - TempMatrix.m[ 1 ][ 2 ] ) * Rate + TempMatrix.m[ 1 ][ 2 ] ;
						DestMatrix->m[ 1 ][ 3 ] = ( FirstMatrix->m[ 1 ][ 3 ] - TempMatrix.m[ 1 ][ 3 ] ) * Rate + TempMatrix.m[ 1 ][ 3 ] ;
						DestMatrix->m[ 2 ][ 0 ] = ( FirstMatrix->m[ 2 ][ 0 ] - TempMatrix.m[ 2 ][ 0 ] ) * Rate + TempMatrix.m[ 2 ][ 0 ] ;
						DestMatrix->m[ 2 ][ 1 ] = ( FirstMatrix->m[ 2 ][ 1 ] - TempMatrix.m[ 2 ][ 1 ] ) * Rate + TempMatrix.m[ 2 ][ 1 ] ;
						DestMatrix->m[ 2 ][ 2 ] = ( FirstMatrix->m[ 2 ][ 2 ] - TempMatrix.m[ 2 ][ 2 ] ) * Rate + TempMatrix.m[ 2 ][ 2 ] ;
						DestMatrix->m[ 2 ][ 3 ] = ( FirstMatrix->m[ 2 ][ 3 ] - TempMatrix.m[ 2 ][ 3 ] ) * Rate + TempMatrix.m[ 2 ][ 3 ] ;
						DestMatrix->m[ 3 ][ 0 ] = ( FirstMatrix->m[ 3 ][ 0 ] - TempMatrix.m[ 3 ][ 0 ] ) * Rate + TempMatrix.m[ 3 ][ 0 ] ;
						DestMatrix->m[ 3 ][ 1 ] = ( FirstMatrix->m[ 3 ][ 1 ] - TempMatrix.m[ 3 ][ 1 ] ) * Rate + TempMatrix.m[ 3 ][ 1 ] ;
						DestMatrix->m[ 3 ][ 2 ] = ( FirstMatrix->m[ 3 ][ 2 ] - TempMatrix.m[ 3 ][ 2 ] ) * Rate + TempMatrix.m[ 3 ][ 2 ] ;
						DestMatrix->m[ 3 ][ 3 ] = ( FirstMatrix->m[ 3 ][ 3 ] - TempMatrix.m[ 3 ][ 3 ] ) * Rate + TempMatrix.m[ 3 ][ 3 ] ;
					}
					else
					{
						*DestMatrix = TempMatrix ;
					}

					/*
					DestMatrix->m[ 0 ][ 0 ] += TempMatrix.m[ 0 ][ 0 ] * MulParam ;
					DestMatrix->m[ 0 ][ 1 ] += TempMatrix.m[ 0 ][ 1 ] * MulParam ;
					DestMatrix->m[ 0 ][ 2 ] += TempMatrix.m[ 0 ][ 2 ] * MulParam ;
					DestMatrix->m[ 0 ][ 3 ] += TempMatrix.m[ 0 ][ 3 ] * MulParam ;
					DestMatrix->m[ 1 ][ 0 ] += TempMatrix.m[ 1 ][ 0 ] * MulParam ;
					DestMatrix->m[ 1 ][ 1 ] += TempMatrix.m[ 1 ][ 1 ] * MulParam ;
					DestMatrix->m[ 1 ][ 2 ] += TempMatrix.m[ 1 ][ 2 ] * MulParam ;
					DestMatrix->m[ 1 ][ 3 ] += TempMatrix.m[ 1 ][ 3 ] * MulParam ;
					DestMatrix->m[ 2 ][ 0 ] += TempMatrix.m[ 2 ][ 0 ] * MulParam ;
					DestMatrix->m[ 2 ][ 1 ] += TempMatrix.m[ 2 ][ 1 ] * MulParam ;
					DestMatrix->m[ 2 ][ 2 ] += TempMatrix.m[ 2 ][ 2 ] * MulParam ;
					DestMatrix->m[ 2 ][ 3 ] += TempMatrix.m[ 2 ][ 3 ] * MulParam ;
					DestMatrix->m[ 3 ][ 0 ] += TempMatrix.m[ 3 ][ 0 ] * MulParam ;
					DestMatrix->m[ 3 ][ 1 ] += TempMatrix.m[ 3 ][ 1 ] * MulParam ;
					DestMatrix->m[ 3 ][ 2 ] += TempMatrix.m[ 3 ][ 2 ] * MulParam ;
					DestMatrix->m[ 3 ][ 3 ] += TempMatrix.m[ 3 ][ 3 ] * MulParam ;
					*/
				}
/*
				if( PhysicsInfo->Bone->Frame->Parent )
				{
					MATRIX InvParentBoneLWM ;

					// �������Z���瓾����s��̓��[���h�s��Ȃ̂ŁA������{�[���̃��[�J���s��ɕϊ�����
					ModelLoader3_InverseMatrix( ( ( PMX_READ_BONE_INFO * )PhysicsInfo->Bone->Frame->Parent->UserData )->LocalWorldMatrix, InvParentBoneLWM ) ;
					CreateMultiplyMatrix( &PhysicsInfo->Bone->KeyMatrix[ DestIndex ], &PhysicsInfo->Bone->LocalWorldMatrix, &InvParentBoneLWM ) ;
				}
				else
				{
					PhysicsInfo->Bone->KeyMatrix[ DestIndex ] = PhysicsInfo->Bone->LocalWorldMatrix ;
				}
*/
			}
		}
	}

	// �I��
	return 0 ;
}

extern int CheckDisablePhysicsAnim_PMXPhysicsInfo(
	DX_MODELLOADER3_PMX_PHYSICS_INFO *MLPhysicsInfo,
	int PhysicsIndex
)
{
	PMX_READ_PHYSICS_INFO *PhysicsInfo = &MLPhysicsInfo->PmxPhysicsInfoDim[ PhysicsIndex ] ;
	BULLET_RIGIDBODY_INFO *BulletRigidBodyInfo = ( BULLET_RIGIDBODY_INFO * )PhysicsInfo->BulletInfo ;

	return BulletRigidBodyInfo->DisableFlag || PhysicsInfo->Base->RigidBodyType == 0 || PhysicsInfo->NoCopyToBone ;
}






//}

#endif // defined( DX_NON_MODEL ) && defined( DX_NON_BULLET_PHYSICS )





