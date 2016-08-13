// -------------------------------------------------------------------------------
// 
// 		�c�w���C�u����		�u�l�c�t�@�C���f�[�^�\���̃w�b�_
// 
// 				Ver 3.16d
// 
// -------------------------------------------------------------------------------

#ifndef __DXMODELLOADERVMD_H__
#define __DXMODELLOADERVMD_H__

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

// VMD�L�[�f�[�^( 111byte )
struct VMD_KEY
{
	BYTE	Data[ 111 ] ;
/*
	char	Name[ 15 ] ;						// ���O
	DWORD	Frame ;								// �t���[��
	float	Position[ 3 ] ;						// ���W
	float	Quaternion[ 4 ] ;					// �N�H�[�^�j�I��
	float	PosXBezier[ 4 ] ;					// ���W�w�p�x�W�F�Ȑ����
	float	PosYBezier[ 4 ] ;					// ���W�x�p�x�W�F�Ȑ����
	float	PosZBezier[ 4 ] ;					// ���W�y�p�x�W�F�Ȑ����
	float	RotBezier[ 4 ] ;					// ��]�p�x�W�F�Ȑ����
*/
} ;

// VMD�\��L�[�f�[�^( 23byte )
struct VMD_FACE_KEY
{
	BYTE	Data[ 23 ] ;						// �f�[�^
/*
	char	Name[ 15 ] ;						// �\�
	DWORD	Frame ;								// �t���[��
	float	Factor ;							// �u�����h��
*/
} ;

struct VMD_CAMERA_LENGTH
{
	DWORD Count;
};

// VMD�J�����L�[�f�[�^
struct VMD_CAMERA 
{
	BYTE	Data[ 61 ] ;						// �f�[�^
/*
	DWORD	FrameNo;							//  4:  0:�t���[���ԍ�
	float	Length;								//  8:  4: -(����)
	float	Location[3];						// 20:  8:�ʒu
	float	Rotate[3];							// 32: 20:�I�C���[�p // X���͕��������]���Ă���̂Œ���
	BYTE	Interpolation[24];					// 56: 32:��ԏ�� // �����炭[6][4](������)
	DWORD	ViewingAngle;						// 60: 56:����
	BYTE	Perspective;						// 61: 60:�ˉe�J�������ǂ��� 0:�ˉe�J���� 1:���ˉe�J����
*/
};

struct VMD_LIGHT_LENGTH
{
	DWORD Count;
};

struct VMD_LIGHT 
{
	DWORD flameNo;
	DWORD RGB[3];
	DWORD Loc[3];
}; 

// VMD�ǂ݂��ݏ����p�L�[�\����
struct VMD_READ_KEY_INFO
{
	DWORD	Frame ;								// �t���[��
	float	Position[ 3 ] ;						// ���W
	float	Quaternion[ 4 ] ;					// �N�H�[�^�j�I��
	int		Linear[ 4 ] ;						// ���`���ǂ���
	float	PosXBezier[ 4 ] ;					// ���W�w�p�x�W�F�Ȑ����
	float	PosYBezier[ 4 ] ;					// ���W�x�p�x�W�F�Ȑ����
	float	PosZBezier[ 4 ] ;					// ���W�y�p�x�W�F�Ȑ����
	float	RotBezier[ 4 ] ;					// ��]�p�x�W�F�Ȑ����
	VECTOR	*MVRPosKey ;						// �Z�o�������W�L�[�ւ̃|�C���^
	FLOAT4	*MVRRotKey ;						// �Z�o������]�L�[�ւ̃|�C���^
	VMD_READ_KEY_INFO *Prev ;					// �O�̃L�[�ւ̃A�h���X
	VMD_READ_KEY_INFO *Next ;					// ���̃L�[�ւ̃A�h���X
} ;

// VMD�ǂݍ��ݏ����p�m�[�h�\����
struct VMD_READ_NODE_INFO
{
	DWORD				KeyNum ;				// �L�[�̐�
	DWORD				MaxFrame ;				// �ő�t���[���ԍ�
	DWORD				MinFrame ;				// �ŏ��t���[���ԍ�
	char				Name[ 16 ] ;			// ���O
	VMD_READ_KEY_INFO	*FirstKey ;				// �L�[���X�g�̐擪
} ;

// VMD�ǂ݂��ݏ����p�\��L�[�\����
struct VMD_READ_FACE_KEY_INFO
{
	DWORD	Frame ;								// �t���[��
	float	Factor ;							// �u�����h��
	VMD_READ_FACE_KEY_INFO *Prev ;				// �O�̃L�[�ւ̃A�h���X
	VMD_READ_FACE_KEY_INFO *Next ;				// ���̃L�[�ւ̃A�h���X
} ;

// VMD�ǂݍ��ݏ����p�\��ʏ��\����
struct VMD_READ_FACE_KEY_SET_INFO
{
	DWORD					KeyNum ;			// �L�[�̐�
	DWORD					MaxFrame ;			// �ő�t���[���ԍ�
	DWORD					MinFrame ;			// �ŏ��t���[���ԍ�
	char					Name[ 16 ] ;		// ���O
	VMD_READ_FACE_KEY_INFO	*FirstKey ;			// �L�[���X�g�̐擪
} ;


// VMD�ǂݍ��ݏ����p�J�����L�[�\����
struct VMD_READ_CAMERA_KEY_INFO
{
	DWORD	Frame;								//  4:  0:�t���[���ԍ�
	float	Length;								//  8:  4: -(����)
	float	Location[3];						// 20:  8:�ʒu
	float	Rotate[3];							// 32: 20:�I�C���[�p // X���͕��������]���Ă���̂Œ���
//	BYTE	Interpolation[24];					// 56: 32:��ԏ�� // �����炭[6][4](������)
	int		Linear[ 6 ] ;						// ���`���ǂ���
	float	PosXBezier[ 4 ] ;					// ���W�w�p�x�W�F�Ȑ����
	float	PosYBezier[ 4 ] ;					// ���W�x�p�x�W�F�Ȑ����
	float	PosZBezier[ 4 ] ;					// ���W�y�p�x�W�F�Ȑ����
	float	RotBezier[ 4 ] ;					// ��]�p�x�W�F�Ȑ����
	float	LenBezier[ 4 ] ;					// �����p�x�W�F�Ȑ����
	float	ViewAngBezier[ 4 ] ;				// ����p�p�x�W�F�Ȑ����
	DWORD	ViewingAngle;						// 60: 56:����p
	BYTE	Perspective;						// 61: 60:�ˉe�J�������ǂ��� 0:�ˉe�J���� 1:���ˉe�J����
	VMD_READ_CAMERA_KEY_INFO *Prev ;			// �O�̃L�[�ւ̃A�h���X
	VMD_READ_CAMERA_KEY_INFO *Next ;			// ���̃L�[�ւ̃A�h���X
} ;

// VMD�ǂݍ��ݏ����p�J�����\����
struct VMD_READ_CAMERA_INFO
{
	DWORD						KeyNum ;		// �L�[�̐�
	DWORD						MaxFrame ;		// �ő�t���[���ԍ�
	DWORD						MinFrame ;		// �ŏ��t���[���ԍ�
	VMD_READ_CAMERA_KEY_INFO	*FirstKey ;		// �L�[���X�g�̐擪
} ;

// VMD�ǂ݂��ݏ����p�\����
struct VMD_READ_INFO
{
	DWORD						NodeNum ;			// �m�[�h�̐�
	VMD_READ_NODE_INFO			*Node ;				// �m�[�h�z��

	DWORD						FaceKeySetNum ;		// �\��L�[�Z�b�g�̐�
	VMD_READ_FACE_KEY_SET_INFO	*FaceKeySet ;		// �\��L�[�Z�b�g�z��

	VMD_READ_KEY_INFO			*KeyBuffer ;		// �L�[�o�b�t�@
	VMD_READ_FACE_KEY_INFO		*FaceKeyBuffer ;	// �\��L�[�o�b�t�@
	VMD_READ_CAMERA_KEY_INFO	*CamKeyBuffer ;		// �J�����L�[�o�b�t�@

	VMD_READ_CAMERA_INFO		*Camera ;			// �J�����L�[�̏��

	DWORD						MaxTime ;			// �A�j���[�V�����̑�����
} ;

// �֐��錾 -------------------------------------

// �ꕔ�������ȗ�����s��̐ς����߂�
__inline void MV1LoadModelToVMD_CreateMultiplyMatrix( MATRIX *Out, MATRIX *In1, MATRIX *In2 )
{
	Out->m[0][0] = In1->m[0][0] * In2->m[0][0] + In1->m[0][1] * In2->m[1][0] + In1->m[0][2] * In2->m[2][0] ;
	Out->m[0][1] = In1->m[0][0] * In2->m[0][1] + In1->m[0][1] * In2->m[1][1] + In1->m[0][2] * In2->m[2][1] ;
	Out->m[0][2] = In1->m[0][0] * In2->m[0][2] + In1->m[0][1] * In2->m[1][2] + In1->m[0][2] * In2->m[2][2] ;
	Out->m[0][3] = 0.0f ;

	Out->m[1][0] = In1->m[1][0] * In2->m[0][0] + In1->m[1][1] * In2->m[1][0] + In1->m[1][2] * In2->m[2][0] ;
	Out->m[1][1] = In1->m[1][0] * In2->m[0][1] + In1->m[1][1] * In2->m[1][1] + In1->m[1][2] * In2->m[2][1] ;
	Out->m[1][2] = In1->m[1][0] * In2->m[0][2] + In1->m[1][1] * In2->m[1][2] + In1->m[1][2] * In2->m[2][2] ;
	Out->m[1][3] = 0.0f ;

	Out->m[2][0] = In1->m[2][0] * In2->m[0][0] + In1->m[2][1] * In2->m[1][0] + In1->m[2][2] * In2->m[2][0] ;
	Out->m[2][1] = In1->m[2][0] * In2->m[0][1] + In1->m[2][1] * In2->m[1][1] + In1->m[2][2] * In2->m[2][1] ;
	Out->m[2][2] = In1->m[2][0] * In2->m[0][2] + In1->m[2][1] * In2->m[1][2] + In1->m[2][2] * In2->m[2][2] ;
	Out->m[2][3] = 0.0f ;

	Out->m[3][0] = In1->m[3][0] * In2->m[0][0] + In1->m[3][1] * In2->m[1][0] + In1->m[3][2] * In2->m[2][0] + In2->m[3][0] ;
	Out->m[3][1] = In1->m[3][0] * In2->m[0][1] + In1->m[3][1] * In2->m[1][1] + In1->m[3][2] * In2->m[2][1] + In2->m[3][1] ;
	Out->m[3][2] = In1->m[3][0] * In2->m[0][2] + In1->m[3][1] * In2->m[1][2] + In1->m[3][2] * In2->m[2][2] + In2->m[3][2] ;
	Out->m[3][3] = 1.0f ;
}

// ��]���������̍s��̐ς����߂�( �R�~�R�ȊO�̕����ɂ͒l��������Ȃ� )
__inline void MV1LoadModelToVMD_CreateMultiplyMatrixRotOnly( MATRIX *Out, MATRIX *In1, MATRIX *In2 )
{
	Out->m[0][0] = In1->m[0][0] * In2->m[0][0] + In1->m[0][1] * In2->m[1][0] + In1->m[0][2] * In2->m[2][0] ;
	Out->m[0][1] = In1->m[0][0] * In2->m[0][1] + In1->m[0][1] * In2->m[1][1] + In1->m[0][2] * In2->m[2][1] ;
	Out->m[0][2] = In1->m[0][0] * In2->m[0][2] + In1->m[0][1] * In2->m[1][2] + In1->m[0][2] * In2->m[2][2] ;

	Out->m[1][0] = In1->m[1][0] * In2->m[0][0] + In1->m[1][1] * In2->m[1][0] + In1->m[1][2] * In2->m[2][0] ;
	Out->m[1][1] = In1->m[1][0] * In2->m[0][1] + In1->m[1][1] * In2->m[1][1] + In1->m[1][2] * In2->m[2][1] ;
	Out->m[1][2] = In1->m[1][0] * In2->m[0][2] + In1->m[1][1] * In2->m[1][2] + In1->m[1][2] * In2->m[2][2] ;

	Out->m[2][0] = In1->m[2][0] * In2->m[0][0] + In1->m[2][1] * In2->m[1][0] + In1->m[2][2] * In2->m[2][0] ;
	Out->m[2][1] = In1->m[2][0] * In2->m[0][1] + In1->m[2][1] * In2->m[1][1] + In1->m[2][2] * In2->m[2][1] ;
	Out->m[2][2] = In1->m[2][0] * In2->m[0][2] + In1->m[2][1] * In2->m[1][2] + In1->m[2][2] * In2->m[2][2] ;
}

// ���������肵�����������ȍ��W�ϊ��v�Z
__inline void MV1LoadModelToVMD_VectorTransform( VECTOR &Out, VECTOR &InVec, MATRIX &InMatrix )
{
	Out.x = InVec.x * InMatrix.m[0][0] + InVec.y * InMatrix.m[1][0] + InVec.z * InMatrix.m[2][0] + InMatrix.m[3][0] ;
	Out.y = InVec.x * InMatrix.m[0][1] + InVec.y * InMatrix.m[1][1] + InVec.z * InMatrix.m[2][1] + InMatrix.m[3][1] ;
	Out.z = InVec.x * InMatrix.m[0][2] + InVec.y * InMatrix.m[1][2] + InVec.z * InMatrix.m[2][2] + InMatrix.m[3][2] ;
}

// �N�H�[�^�j�I���̏�Z
__inline FLOAT4 QuatMul( FLOAT4 &q1, FLOAT4 &q2 )
{
	FLOAT4 res ;

	res.x = q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y ;
	res.y = q1.w * q2.y - q1.x * q2.z + q1.y * q2.w + q1.z * q2.x ;
	res.z = q1.w * q2.z + q1.x * q2.y - q1.y * q2.x + q1.z * q2.w ;
	res.w = q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z ;

	return res;
}

// �N�H�[�^�j�I������s��ɕϊ�
__inline void QuatConvertToMatrix( MATRIX &mat, FLOAT4 &q, VECTOR &trans )
{
	float sx = q.x * q.x * 2.0f ;
	float sy = q.y * q.y * 2.0f ;
	float sz = q.z * q.z * 2.0f ;
	float cx = q.y * q.z * 2.0f ;
	float cy = q.x * q.z * 2.0f ;
	float cz = q.x * q.y * 2.0f ;
	float wx = q.w * q.x * 2.0f ;
	float wy = q.w * q.y * 2.0f ;
	float wz = q.w * q.z * 2.0f ;

	mat.m[0][0] = 1.0f - ( sy + sz ) ;	mat.m[0][1] = cz + wz ;				mat.m[0][2] = cy - wy ;				mat.m[0][3] = 0.0f ;
	mat.m[1][0] = cz - wz ;				mat.m[1][1] = 1.0f - ( sx + sz ) ;	mat.m[1][2] = cx + wx ;				mat.m[1][3] = 0.0f ;
	mat.m[2][0] = cy + wy ;				mat.m[2][1] = cx - wx ;				mat.m[2][2] = 1.0f - ( sx + sy ) ;	mat.m[2][3] = 0.0f ;
	mat.m[3][0] = trans.x ;				mat.m[3][1] = trans.y ;				mat.m[3][2] = trans.z ;				mat.m[3][3] = 1.0f ;
}

// �s�񂩂�N�H�[�^�j�I���ɕϊ�
__inline void QuatConvertFromMatrix( FLOAT4 &q, MATRIX &mat )
{
	float s ;
	float tr = mat.m[0][0] + mat.m[1][1] + mat.m[2][2] + 1.0f ;
	if( tr >= 1.0f )
	{
		s = 0.5f / _SQRT( tr ) ;
		q.w = 0.25f / s ;
		q.x = ( mat.m[1][2] - mat.m[2][1] ) * s ;
		q.y = ( mat.m[2][0] - mat.m[0][2] ) * s ;
		q.z = ( mat.m[0][1] - mat.m[1][0] ) * s ;
	}
	else
	{
		float max ;
		max = mat.m[1][1] > mat.m[2][2] ? mat.m[1][1] : mat.m[2][2] ;
		
		if( max < mat.m[0][0] )
		{
			s = _SQRT( mat.m[0][0] - ( mat.m[1][1] + mat.m[2][2] ) + 1.0f ) ;

			float x = s * 0.5f ;
			s = 0.5f / s ;
			q.x = x ;
			q.y = ( mat.m[0][1] + mat.m[1][0] ) * s ;
			q.z = ( mat.m[2][0] + mat.m[0][2] ) * s ;
			q.w = ( mat.m[1][2] - mat.m[2][1] ) * s ;
		}
		else
		if( max == mat.m[1][1] )
		{
			s = _SQRT( mat.m[1][1] - ( mat.m[2][2] + mat.m[0][0]) + 1.0f ) ;

			float y = s * 0.5f ;
			s = 0.5f / s ;
			q.x = ( mat.m[0][1] + mat.m[1][0] ) * s ;
			q.y = y ;
			q.z = ( mat.m[1][2] + mat.m[2][1] ) * s ;
			q.w = ( mat.m[2][0] - mat.m[0][2] ) * s ;
		}
		else
		{
			s = _SQRT( mat.m[2][2] - (mat.m[0][0] + mat.m[1][1]) + 1.0f ) ;

			float z = s * 0.5f ;
			s = 0.5f / s ;
			q.x = ( mat.m[2][0] + mat.m[0][2] ) * s ;
			q.y = ( mat.m[1][2] + mat.m[2][1] ) * s ;
			q.z = z ;
			q.w = ( mat.m[0][1] - mat.m[1][0] ) * s ;
		}
	}
}

__inline static void VmdCalcLine( int &Linear, float &Rate, float &RateH, float &X1, float &X2, float &Y1, float &Y2 )
{
	if( Linear )
	{
		RateH = Rate ;
	}
	else
	{
		int l ;
		float T, InvT ;

		for( l = 0, T = Rate, InvT = 1.0f - T ; l < 32 ; l ++ )
		{
			float TempX = InvT * InvT * T * X1 + InvT * T * T * X2 + T * T * T - Rate ;
			if( TempX < 0.0001f && TempX > -0.0001f ) break ;
			T -= TempX * 0.5f ;
			InvT = 1.0f - T ;
		}
		RateH = InvT * InvT * T * Y1 + InvT * T * T * Y2 + T * T * T ;
	}
}

// �N�H�[�^�j�I���̐��K��
extern void QuaternionNormalize( FLOAT4 *Out, FLOAT4 *Src ) ;

// �N�H�[�^�j�I������XYZ����]�̎擾
extern void QuaternionToEuler( VECTOR *Angle, const FLOAT4 *Qt ) ;

// XYZ��]�l����N�H�[�^�j�I���̍쐬
extern void QuaternionCreateEuler( FLOAT4 *Qt, const VECTOR *Angle ) ;

// ���������肵�����������ȋt�s��v�Z
extern void MV1LoadModelToVMD_InverseMatrix( MATRIX &InMatrix, MATRIX &OutMatrix ) ;

// �u�l�c�t�@�C���̊�{����ǂݍ���( -1:�G���[ )
extern int LoadVMDBaseData( VMD_READ_INFO *VmdData, void *DataBuffer, int DataSize ) ;

// �u�l�c�t�@�C���̊�{���̌�n��������
extern int TerminateVMDBaseData( VMD_READ_INFO *VmdData ) ;

// �J�����̃A�j���[�V������ǂݍ��݂悤�f�[�^�ɒǉ�����
extern int SetupVMDCameraAnim( VMD_READ_INFO *VmdData, MV1_MODEL_R *RModel, const wchar_t *Name, MV1_ANIMSET_R *AnimSet ) ;

// VMD�t�@�C���o�C�i�����������ɓǂݍ���
extern int LoadFile_VMD(
	void **					VmdData,
	int *					FileSize,
	int						FileNumber,
	MV1_FILE_READ_FUNC *	FileReadFunc,
	const wchar_t *			Name,
	int						NameLen,
	const wchar_t *			CurrentDir,
	int *					LoopMotionFlag,
	int *					DisablePhysicsFlag,
	float *					BaseGravity,
	int *					GravityNo,
	int *					GravityEnable,
	VECTOR *				Gravity
) ;


#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

#endif

#endif
