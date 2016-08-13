// -------------------------------------------------------------------------------
// 
// 		�c�w���C�u����		���Z�v���O�����w�b�_�t�@�C��
// 
// 				Ver 3.16d
// 
// -------------------------------------------------------------------------------

#ifndef __DXMATH_H__
#define __DXMATH_H__

// �C���N���[�h ------------------------------------------------------------------
#include "DxCompileConfig.h"
#include "DxLib.h"

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// �}�N����` --------------------------------------------------------------------

// �\���̒�` --------------------------------------------------------------------

// �e�[�u��-----------------------------------------------------------------------

// �������ϐ��錾 --------------------------------------------------------------

// �֐��v���g�^�C�v�錾-----------------------------------------------------------

extern	void	TriangleBarycenter_Base(  const VECTOR   *TrianglePos1, const VECTOR   *TrianglePos2, const VECTOR   *TrianglePos3, const VECTOR   *Position, float  *TrianglePos1Weight, float  *TrianglePos2Weight, float  *TrianglePos3Weight ) ;	// �w��̍��W����O�p�`�̏d�S�����߂�
extern	void	TriangleBarycenter_BaseD( const VECTOR_D *TrianglePos1, const VECTOR_D *TrianglePos2, const VECTOR_D *TrianglePos3, const VECTOR_D *Position, double *TrianglePos1Weight, double *TrianglePos2Weight, double *TrianglePos3Weight ) ;	// �w��̍��W����O�p�`�̏d�S�����߂�

extern VECTOR	Get_Triangle_Point_MinPosition(  VECTOR   Point, VECTOR   TrianglePos1, VECTOR   TrianglePos2, VECTOR   TrianglePos3 ) ;			// �_�Ɉ�ԋ߂��O�p�`��̍��W�𓾂�
extern VECTOR_D	Get_Triangle_Point_MinPositionD( VECTOR_D Point, VECTOR_D TrianglePos1, VECTOR_D TrianglePos2, VECTOR_D TrianglePos3 ) ;			// �_�Ɉ�ԋ߂��O�p�`��̍��W�𓾂�
extern VECTOR	Get_Line_Point_MinPosition(  VECTOR   Point, VECTOR   LinePos1, VECTOR   LinePos2 ) ;											// �_�Ɉ�ԋ߂�����̍��W�𓾂�
extern VECTOR_D	Get_Line_Point_MinPositionD( VECTOR_D Point, VECTOR_D LinePos1, VECTOR_D LinePos2 ) ;											// �_�Ɉ�ԋ߂�����̍��W�𓾂�
extern int		Get_TwoPlane_Line(  VECTOR   Normal1, float  Dist1, VECTOR   Normal2, float  Dist2, VECTOR   *OutDir, VECTOR   *OutPos ) ;			// ��̕��ʂ�����������𓾂�( �߂�l�� -1 �̏ꍇ�́A��̖ʂ͕��s )
extern int		Get_TwoPlane_LineD( VECTOR_D Normal1, double Dist1, VECTOR_D Normal2, double Dist2, VECTOR_D *OutDir, VECTOR_D *OutPos ) ;			// ��̕��ʂ�����������𓾂�( �߂�l�� -1 �̏ꍇ�́A��̖ʂ͕��s )
extern int		Get_TwoLine_MinLength_Rate(  VECTOR   Direction1, VECTOR   Position1, float  *OutRate1, VECTOR   Direction2, VECTOR   Position2, float  *OutRate2 ) ;		// ��̐��̍ŋߓ_�̂��ꂼ��̕����x�N�g���̊g��l�𓾂�
extern int		Get_TwoLine_MinLength_RateD( VECTOR_D Direction1, VECTOR_D Position1, double *OutRate1, VECTOR_D Direction2, VECTOR_D Position2, double *OutRate2 ) ;		// ��̐��̍ŋߓ_�̂��ꂼ��̕����x�N�g���̊g��l�𓾂�

extern void		CreateNormalizePlane(  FLOAT4  *Plane, VECTOR   *Position, VECTOR   *Normal ) ;												// ���ʏ�̓_�ƕ��ʂ̖@�����琳�K�����ꂽ���ʃp�����[�^���Z�o����
extern void		CreateNormalizePlaneD( DOUBLE4 *Plane, VECTOR_D *Position, VECTOR_D *Normal ) ;												// ���ʏ�̓_�ƕ��ʂ̖@�����琳�K�����ꂽ���ʃp�����[�^���Z�o����
extern void		SetMathScreenSize( int SizeX, int SizeY ) ;																				// DxMath.cpp �Ŏg�p�����ʂ̃T�C�Y��ݒ肷��

// ��`�̃N���b�s���O
__inline int RectClipping_Inline( RECT *Rect, const RECT *ClippuRect )
{
	     if( Rect->left   > Rect->right        ) Rect->right  = Rect->left ;
	else if( Rect->right  < Rect->left         ) Rect->left   = Rect->right	;

		 if( Rect->top    > Rect->bottom	   ) Rect->bottom = Rect->top ;
	else if( Rect->bottom < Rect->top		   ) Rect->top    = Rect->bottom ;

		 if( Rect->bottom > ClippuRect->bottom ) Rect->bottom = ClippuRect->bottom ;
	else if( Rect->bottom < ClippuRect->top	   ) Rect->bottom = ClippuRect->top ;

		 if( Rect->top	  > ClippuRect->bottom ) Rect->top    = ClippuRect->bottom ;
	else if( Rect->top	  < ClippuRect->top    ) Rect->top    = ClippuRect->top ;


		 if( Rect->right  > ClippuRect->right  ) Rect->right  = ClippuRect->right ;
	else if( Rect->right  < ClippuRect->left   ) Rect->right  = ClippuRect->left ;

		 if( Rect->left	  > ClippuRect->right  ) Rect->left   = ClippuRect->right ;
	else if( Rect->left	  < ClippuRect->left   ) Rect->left   = ClippuRect->left ;

	return 0 ;
}

#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

#endif // __DXMATH_H__
