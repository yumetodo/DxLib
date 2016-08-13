// ----------------------------------------------------------------------------
// 
// 		�c�w���C�u����		���Z�v���O����
// 
// 				Ver 3.16d
// 
// ----------------------------------------------------------------------------

// �c�w���C�u�����쐬���p��`
#define __DX_MAKE

// �C���N���[�h----------------------------------------------------------------
#include "DxMath.h"
#include "DxLib.h"
#include "DxStatic.h"
#include "DxUseCLib.h"
#include "DxBaseFunc.h"
#include "DxMemory.h"

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// �}�N����`------------------------------------------------------------------

// �\���̐錾------------------------------------------------------------------

// �f�[�^�錾------------------------------------------------------------------

int MathScreenSizeX ;
int MathScreenSizeY ;

// �v���O�����R�[�h------------------------------------------------------------

// DxMath.cpp �Ŏg�p�����ʂ̃T�C�Y��ݒ肷��
extern void SetMathScreenSize( int SizeX, int SizeY )
{
	MathScreenSizeX = SizeX ;
	MathScreenSizeY = SizeY ;
}

// �_�Ɉ�ԋ߂��O�p�`��̍��W�𓾂�
extern VECTOR	Get_Triangle_Point_MinPosition( VECTOR Point, VECTOR TrianglePos1, VECTOR TrianglePos2, VECTOR TrianglePos3 )
{
	VECTOR Line12, Line23, Line31, Line1P, Line2P, Line3P, Result ;
	float Dot1P2, Dot1P3, Dot2P1, Dot2P3, Dot2PH, Dot3P1, Dot3P2, Dot3PH, OPA, OPB, OPC, Div, t, v, w ;

	VectorSub( &Line12, &TrianglePos2, &TrianglePos1 ) ;
	VectorSub( &Line31, &TrianglePos1, &TrianglePos3 ) ;
	VectorSub( &Line1P, &Point,        &TrianglePos1 ) ;
	Dot1P2 = VectorInnerProduct( &Line12, &Line1P ) ;
	Dot1P3 = VectorInnerProduct( &Line31, &Line1P ) ;
	if( Dot1P2 <= 0.0f && Dot1P3 >= 0.0f ) return TrianglePos1 ;

	VectorSub( &Line23, &TrianglePos3, &TrianglePos2 ) ;
	VectorSub( &Line2P, &Point,        &TrianglePos2 ) ;
	Dot2P1 = VectorInnerProduct( &Line12, &Line2P ) ;
	Dot2P3 = VectorInnerProduct( &Line23, &Line2P ) ;
	if( Dot2P1 >= 0.0f && Dot2P3 <= 0.0f ) return TrianglePos2 ;

	Dot2PH = VectorInnerProduct( &Line31, &Line2P ) ;
	OPC = Dot1P2 * -Dot2PH - Dot2P1 * -Dot1P3 ;	// �����O�����W���P����
	if( OPC <= 0.0f && Dot1P2 >= 0.0f && Dot2P1 <= 0.0f )
	{
		t = Dot1P2 / ( Dot1P2 - Dot2P1 ) ;
		Result.x = TrianglePos1.x + Line12.x * t ;
		Result.y = TrianglePos1.y + Line12.y * t ;
		Result.z = TrianglePos1.z + Line12.z * t ;
		return Result ;
	}

	VectorSub( &Line3P, &Point,        &TrianglePos3 ) ;
	Dot3P1 = VectorInnerProduct( &Line31, &Line3P ) ;
	Dot3P2 = VectorInnerProduct( &Line23, &Line3P ) ;
	if( Dot3P1 <= 0.0f && Dot3P2 >= 0.0f ) return TrianglePos3 ;

	Dot3PH = VectorInnerProduct( &Line12, &Line3P ) ;
	OPB = Dot3PH * -Dot1P3 - Dot1P2 * -Dot3P1 ;	// �����O�����W���P����
	if( OPB <= 0.0f && Dot1P3 <= 0.0f && Dot3P1 >= 0.0f )
	{
		t = Dot3P1 / ( Dot3P1 - Dot1P3 ) ;
		Result.x = TrianglePos3.x + Line31.x * t ;
		Result.y = TrianglePos3.y + Line31.y * t ;
		Result.z = TrianglePos3.z + Line31.z * t ;
		return Result ;
	}

	OPA = Dot2P1 * -Dot3P1 - Dot3PH * -Dot2PH ;	// �����O�����W���P����
	if( OPA <= 0.0f && ( -Dot2PH - Dot2P1 ) >= 0.0f && ( Dot3PH + Dot3P1 ) >= 0.0f )
	{
		t = ( -Dot2PH - Dot2P1 ) / ( ( -Dot2PH - Dot2P1 ) + ( Dot3PH + Dot3P1 ) ) ;
		Result.x = TrianglePos2.x + Line23.x * t ;
		Result.y = TrianglePos2.y + Line23.y * t ;
		Result.z = TrianglePos2.z + Line23.z * t ;
		return Result ;
	}

	Div = 1.0f / ( OPA + OPB + OPC ) ;
	v = OPB * Div ;
	w = OPC * Div ;
	Result.x = TrianglePos1.x + Line12.x * v - Line31.x * w ;
	Result.y = TrianglePos1.y + Line12.y * v - Line31.y * w ;
	Result.z = TrianglePos1.z + Line12.z * v - Line31.z * w ;
	return Result ;
}

// �_�Ɉ�ԋ߂��O�p�`��̍��W�𓾂�
extern VECTOR_D	Get_Triangle_Point_MinPositionD( VECTOR_D Point, VECTOR_D TrianglePos1, VECTOR_D TrianglePos2, VECTOR_D TrianglePos3 )
{
	VECTOR_D Line12, Line23, Line31, Line1P, Line2P, Line3P, Result ;
	double Dot1P2, Dot1P3, Dot2P1, Dot2P3, Dot2PH, Dot3P1, Dot3P2, Dot3PH, OPA, OPB, OPC, Div, t, v, w ;

	VectorSubD( &Line12, &TrianglePos2, &TrianglePos1 ) ;
	VectorSubD( &Line31, &TrianglePos1, &TrianglePos3 ) ;
	VectorSubD( &Line1P, &Point,        &TrianglePos1 ) ;
	Dot1P2 = VectorInnerProductD( &Line12, &Line1P ) ;
	Dot1P3 = VectorInnerProductD( &Line31, &Line1P ) ;
	if( Dot1P2 <= 0.0 && Dot1P3 >= 0.0 ) return TrianglePos1 ;

	VectorSubD( &Line23, &TrianglePos3, &TrianglePos2 ) ;
	VectorSubD( &Line2P, &Point,        &TrianglePos2 ) ;
	Dot2P1 = VectorInnerProductD( &Line12, &Line2P ) ;
	Dot2P3 = VectorInnerProductD( &Line23, &Line2P ) ;
	if( Dot2P1 >= 0.0 && Dot2P3 <= 0.0 ) return TrianglePos2 ;

	Dot2PH = VectorInnerProductD( &Line31, &Line2P ) ;
	OPC = Dot1P2 * -Dot2PH - Dot2P1 * -Dot1P3 ;	// �����O�����W���P����
	if( OPC <= 0.0 && Dot1P2 >= 0.0 && Dot2P1 <= 0.0 )
	{
		t = Dot1P2 / ( Dot1P2 - Dot2P1 ) ;
		Result.x = TrianglePos1.x + Line12.x * t ;
		Result.y = TrianglePos1.y + Line12.y * t ;
		Result.z = TrianglePos1.z + Line12.z * t ;
		return Result ;
	}

	VectorSubD( &Line3P, &Point,        &TrianglePos3 ) ;
	Dot3P1 = VectorInnerProductD( &Line31, &Line3P ) ;
	Dot3P2 = VectorInnerProductD( &Line23, &Line3P ) ;
	if( Dot3P1 <= 0.0 && Dot3P2 >= 0.0 ) return TrianglePos3 ;

	Dot3PH = VectorInnerProductD( &Line12, &Line3P ) ;
	OPB = Dot3PH * -Dot1P3 - Dot1P2 * -Dot3P1 ;	// �����O�����W���P����
	if( OPB <= 0.0 && Dot1P3 <= 0.0 && Dot3P1 >= 0.0 )
	{
		t = Dot3P1 / ( Dot3P1 - Dot1P3 ) ;
		Result.x = TrianglePos3.x + Line31.x * t ;
		Result.y = TrianglePos3.y + Line31.y * t ;
		Result.z = TrianglePos3.z + Line31.z * t ;
		return Result ;
	}

	OPA = Dot2P1 * -Dot3P1 - Dot3PH * -Dot2PH ;	// �����O�����W���P����
	if( OPA <= 0.0 && ( -Dot2PH - Dot2P1 ) >= 0.0 && ( Dot3PH + Dot3P1 ) >= 0.0 )
	{
		t = ( -Dot2PH - Dot2P1 ) / ( ( -Dot2PH - Dot2P1 ) + ( Dot3PH + Dot3P1 ) ) ;
		Result.x = TrianglePos2.x + Line23.x * t ;
		Result.y = TrianglePos2.y + Line23.y * t ;
		Result.z = TrianglePos2.z + Line23.z * t ;
		return Result ;
	}

	Div = 1.0 / ( OPA + OPB + OPC ) ;
	v = OPB * Div ;
	w = OPC * Div ;
	Result.x = TrianglePos1.x + Line12.x * v - Line31.x * w ;
	Result.y = TrianglePos1.y + Line12.y * v - Line31.y * w ;
	Result.z = TrianglePos1.z + Line12.z * v - Line31.z * w ;
	return Result ;
}

// �_�Ɉ�ԋ߂�����̍��W�𓾂�
extern VECTOR	Get_Line_Point_MinPosition( VECTOR Point, VECTOR LinePos1, VECTOR LinePos2 )
{
	float Length12, t ;
	float Dot12_1P ;
	VECTOR Line12, Line1P ;
	VECTOR Result ;

	Line12.x = LinePos2.x - LinePos1.x ;
	Line12.y = LinePos2.y - LinePos1.y ;
	Line12.z = LinePos2.z - LinePos1.z ;
	Line1P.x = Point.x - LinePos1.x ;
	Line1P.y = Point.y - LinePos1.y ;
	Line1P.z = Point.z - LinePos1.z ;
	Dot12_1P = Line12.x * Line1P.x + Line12.y * Line1P.y + Line12.z * Line1P.z ;
	if( Dot12_1P <= 0.0f )
	{
		return LinePos1 ;
	}

	Length12 = Line12.x * Line12.x + Line12.y * Line12.y + Line12.z * Line12.z ;
	if( Length12 == 0.0f )
	{
		return LinePos1 ;
	}

	if( Length12 < Dot12_1P )
	{
		return LinePos2 ;
	}

	t = Dot12_1P / Length12 ;
	Result.x = LinePos1.x + Line12.x * t ;
	Result.y = LinePos1.y + Line12.y * t ;
	Result.z = LinePos1.z + Line12.z * t ;
	return Result ;
}

// �_�Ɉ�ԋ߂�����̍��W�𓾂�
extern VECTOR_D	Get_Line_Point_MinPositionD( VECTOR_D Point, VECTOR_D LinePos1, VECTOR_D LinePos2 )
{
	double Length12, t ;
	double Dot12_1P ;
	VECTOR_D Line12, Line1P ;
	VECTOR_D Result ;

	Line12.x = LinePos2.x - LinePos1.x ;
	Line12.y = LinePos2.y - LinePos1.y ;
	Line12.z = LinePos2.z - LinePos1.z ;
	Line1P.x = Point.x - LinePos1.x ;
	Line1P.y = Point.y - LinePos1.y ;
	Line1P.z = Point.z - LinePos1.z ;
	Dot12_1P = Line12.x * Line1P.x + Line12.y * Line1P.y + Line12.z * Line1P.z ;
	if( Dot12_1P <= 0.0 )
	{
		return LinePos1 ;
	}

	Length12 = Line12.x * Line12.x + Line12.y * Line12.y + Line12.z * Line12.z ;
	if( Length12 == 0.0 )
	{
		return LinePos1 ;
	}

	if( Length12 < Dot12_1P )
	{
		return LinePos2 ;
	}

	t = Dot12_1P / Length12 ;
	Result.x = LinePos1.x + Line12.x * t ;
	Result.y = LinePos1.y + Line12.y * t ;
	Result.z = LinePos1.z + Line12.z * t ;
	return Result ;
}

// ��̕��ʂ�����������𓾂�
extern int Get_TwoPlane_Line( VECTOR Normal1, float Dist1, VECTOR Normal2, float Dist2, VECTOR *OutDir, VECTOR *OutPos )
{
	float d11, d12, d22, div, k1, k2 ;

	*OutDir = VCross( Normal1, Normal2 ) ;
	if( VDot( *OutDir, *OutDir ) < 0.000001f ) return -1 ;

	d11 = VDot( Normal1, Normal1 ) ;
	d12 = VDot( Normal1, Normal2 ) ;
	d22 = VDot( Normal2, Normal2 ) ;
	div = d11 * d22 - d12 * d12 ;
	k1 = ( Dist1 * d22 - Dist2 * d12 ) / div ;
	k2 = ( Dist2 * d11 - Dist1 * d12 ) / div ;
	*OutPos = VAdd( VScale( Normal1, k1 ), VScale( Normal2, k2 ) ) ;

	return 0 ;
}

// ��̕��ʂ�����������𓾂�
extern int Get_TwoPlane_LineD( VECTOR_D Normal1, double Dist1, VECTOR_D Normal2, double Dist2, VECTOR_D *OutDir, VECTOR_D *OutPos )
{
	double d11, d12, d22, div, k1, k2 ;

	*OutDir = VCrossD( Normal1, Normal2 ) ;
	if( VDotD( *OutDir, *OutDir ) < 0.000001 ) return -1 ;

	d11 = VDotD( Normal1, Normal1 ) ;
	d12 = VDotD( Normal1, Normal2 ) ;
	d22 = VDotD( Normal2, Normal2 ) ;
	div = d11 * d22 - d12 * d12 ;
	k1 = ( Dist1 * d22 - Dist2 * d12 ) / div ;
	k2 = ( Dist2 * d11 - Dist1 * d12 ) / div ;
	*OutPos = VAddD( VScaleD( Normal1, k1 ), VScaleD( Normal2, k2 ) ) ;

	return 0 ;
}

// ��̐��̍ŋߓ_�̂��ꂼ��̕����x�N�g���̊g��l�𓾂�
extern int	Get_TwoLine_MinLength_Rate( VECTOR Direction1, VECTOR Position1, float *OutRate1, VECTOR Direction2, VECTOR Position2, float *OutRate2 )
{
	VECTOR r ;
	float a, b, c, d, e, f, s, t ;

	r = VSub( Position1, Position2 ) ;
	a = VDot( Direction1, Direction1 ) ;
	b = VDot( Direction1, Direction2 ) ;
	c = VDot( Direction1, r ) ;
	e = VDot( Direction2, Direction2 ) ;
	f = VDot( Direction2, r ) ;

	d = a * e - b * b ;
	s = ( b * f - c * e ) / d ;
	t = ( a * f - b * c ) / d ;

	if( OutRate1 ) *OutRate1 = s ;
	if( OutRate2 ) *OutRate2 = t ;

	return 0 ;
}

// ��̐��̍ŋߓ_�̂��ꂼ��̕����x�N�g���̊g��l�𓾂�
extern int	Get_TwoLine_MinLength_RateD( VECTOR_D Direction1, VECTOR_D Position1, double *OutRate1, VECTOR_D Direction2, VECTOR_D Position2, double *OutRate2 )
{
	VECTOR_D r ;
	double a, b, c, d, e, f, s, t ;

	r = VSubD( Position1, Position2 ) ;
	a = VDotD( Direction1, Direction1 ) ;
	b = VDotD( Direction1, Direction2 ) ;
	c = VDotD( Direction1, r ) ;
	e = VDotD( Direction2, Direction2 ) ;
	f = VDotD( Direction2, r ) ;

	d = a * e - b * b ;
	s = ( b * f - c * e ) / d ;
	t = ( a * f - b * c ) / d ;

	if( OutRate1 ) *OutRate1 = s ;
	if( OutRate2 ) *OutRate2 = t ;

	return 0 ;
}


// ���Z���C�u����

// ���ʏ�̓_�ƕ��ʂ̖@�����琳�K�����ꂽ���ʃp�����[�^���Z�o����
extern void CreateNormalizePlane( FLOAT4 *Plane, VECTOR *Position, VECTOR *Normal )
{
	float fTemp ;

	fTemp = _SQRT( Normal->x * Normal->x + Normal->y * Normal->y + Normal->z * Normal->z ) ; 
	Plane->x = Normal->x / fTemp ;
	Plane->y = Normal->y / fTemp ;
	Plane->z = Normal->z / fTemp ;
	Plane->w = -( Plane->x * Position->x + Plane->y * Position->y + Plane->z * Position->z ) ;
}

// ���ʏ�̓_�ƕ��ʂ̖@�����琳�K�����ꂽ���ʃp�����[�^���Z�o����
extern void CreateNormalizePlaneD( DOUBLE4 *Plane, VECTOR_D *Position, VECTOR_D *Normal )
{
	double fTemp ;

	fTemp = _SQRTD( Normal->x * Normal->x + Normal->y * Normal->y + Normal->z * Normal->z ) ; 
	Plane->x = Normal->x / fTemp ;
	Plane->y = Normal->y / fTemp ;
	Plane->z = Normal->z / fTemp ;
	Plane->w = -( Plane->x * Position->x + Plane->y * Position->y + Plane->z * Position->z ) ;
}

// �x�N�g���̊O�ς����߂�
__inline static VECTOR VectorGaiseki( const VECTOR &a, const VECTOR &b )
{
	VECTOR out ;

	out.x = a.y * b.z - a.z * b.y ;
	out.y = -( a.x * b.z - a.z * b.x ) ;
	out.z = a.x * b.y - a.y * b.x ;

	return out ;
}

// �x�N�g���̊O�ς����߂�
__inline static VECTOR_D VectorGaisekiD( const VECTOR_D &a, const VECTOR_D &b )
{
	VECTOR_D out ;

	out.x = a.y * b.z - a.z * b.y ;
	out.y = -( a.x * b.z - a.z * b.x ) ;
	out.z = a.x * b.y - a.y * b.x ;

	return out ;
}

// �x�N�g���̍������߂�
__inline static VECTOR VectorSa( const VECTOR &a, const VECTOR &b )
{
	VECTOR out ;

	out.x = a.x - b.x ;
	out.y = a.y - b.y ;
	out.z = a.z - b.z ;

	return out ;
}

// �x�N�g���̍������߂�
__inline static VECTOR_D VectorSaD( const VECTOR_D &a, const VECTOR_D &b )
{
	VECTOR_D out ;

	out.x = a.x - b.x ;
	out.y = a.y - b.y ;
	out.z = a.z - b.z ;

	return out ;
}

// float�^�v�f�̍s���double�^�v�f�̍s��ɕϊ�����
extern int ConvertMatrixFtoD( MATRIX_D *Out, const MATRIX *In )
{
	Out->m[ 0 ][ 0 ] = In->m[ 0 ][ 0 ] ;
	Out->m[ 0 ][ 1 ] = In->m[ 0 ][ 1 ] ;
	Out->m[ 0 ][ 2 ] = In->m[ 0 ][ 2 ] ;
	Out->m[ 0 ][ 3 ] = In->m[ 0 ][ 3 ] ;

	Out->m[ 1 ][ 0 ] = In->m[ 1 ][ 0 ] ;
	Out->m[ 1 ][ 1 ] = In->m[ 1 ][ 1 ] ;
	Out->m[ 1 ][ 2 ] = In->m[ 1 ][ 2 ] ;
	Out->m[ 1 ][ 3 ] = In->m[ 1 ][ 3 ] ;

	Out->m[ 2 ][ 0 ] = In->m[ 2 ][ 0 ] ;
	Out->m[ 2 ][ 1 ] = In->m[ 2 ][ 1 ] ;
	Out->m[ 2 ][ 2 ] = In->m[ 2 ][ 2 ] ;
	Out->m[ 2 ][ 3 ] = In->m[ 2 ][ 3 ] ;

	Out->m[ 3 ][ 0 ] = In->m[ 3 ][ 0 ] ;
	Out->m[ 3 ][ 1 ] = In->m[ 3 ][ 1 ] ;
	Out->m[ 3 ][ 2 ] = In->m[ 3 ][ 2 ] ;
	Out->m[ 3 ][ 3 ] = In->m[ 3 ][ 3 ] ;

	return 0 ;
}

// double�^�v�f�̍s���float�^�v�f�̍s��ɕϊ�����
extern int ConvertMatrixDtoF( MATRIX *Out, const MATRIX_D *In )
{
	Out->m[ 0 ][ 0 ] = ( float )In->m[ 0 ][ 0 ] ;
	Out->m[ 0 ][ 1 ] = ( float )In->m[ 0 ][ 1 ] ;
	Out->m[ 0 ][ 2 ] = ( float )In->m[ 0 ][ 2 ] ;
	Out->m[ 0 ][ 3 ] = ( float )In->m[ 0 ][ 3 ] ;

	Out->m[ 1 ][ 0 ] = ( float )In->m[ 1 ][ 0 ] ;
	Out->m[ 1 ][ 1 ] = ( float )In->m[ 1 ][ 1 ] ;
	Out->m[ 1 ][ 2 ] = ( float )In->m[ 1 ][ 2 ] ;
	Out->m[ 1 ][ 3 ] = ( float )In->m[ 1 ][ 3 ] ;

	Out->m[ 2 ][ 0 ] = ( float )In->m[ 2 ][ 0 ] ;
	Out->m[ 2 ][ 1 ] = ( float )In->m[ 2 ][ 1 ] ;
	Out->m[ 2 ][ 2 ] = ( float )In->m[ 2 ][ 2 ] ;
	Out->m[ 2 ][ 3 ] = ( float )In->m[ 2 ][ 3 ] ;

	Out->m[ 3 ][ 0 ] = ( float )In->m[ 3 ][ 0 ] ;
	Out->m[ 3 ][ 1 ] = ( float )In->m[ 3 ][ 1 ] ;
	Out->m[ 3 ][ 2 ] = ( float )In->m[ 3 ][ 2 ] ;
	Out->m[ 3 ][ 3 ] = ( float )In->m[ 3 ][ 3 ] ;

	return 0 ;
}

// �P�ʍs����쐬����
extern int CreateIdentityMatrix( MATRIX *Out )
{
	static const MATRIX m =
	{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	} ;

	_MEMCPY( Out, &m, sizeof( MATRIX ) ) ;

	// �I��
	return 0 ;
}

// �P�ʍs����쐬����
extern int CreateIdentityMatrixD( MATRIX_D *Out )
{
	static const MATRIX_D m =
	{
		1.0, 0.0, 0.0, 0.0 ,
		0.0, 1.0, 0.0, 0.0 ,
		0.0, 0.0, 1.0, 0.0 ,
		0.0, 0.0, 0.0, 1.0 
	} ;

	_MEMCPY( Out, &m, sizeof( MATRIX_D ) ) ;

	// �I��
	return 0 ;
}

// �r���[�s����쐬����
extern int CreateLookAtMatrix( MATRIX *Out, const VECTOR *Eye, const VECTOR *At, const VECTOR *Up )
{
	VECTOR va, vu, vx, vy, vz ;

	// ���_�������_�̃x�N�g���𓾂�
	va = VectorSa( *At, *Eye ) ;

	// ���_�ɐ����ȃx�N�g���𓾂�
	vu = *Up ;

	// �V�w���̃x�N�g�����Z�o
	vx = VectorGaiseki( vu, va ) ;

	// �V�x���̃x�N�g�����Z�o
	vy = VectorGaiseki( va, vx ) ;

	// �V�y���̃x�N�g�����Z�b�g
	vz = va ;

	// �e�x�N�g���̐��K��
	VectorNormalize( &vx, &vx ) ;
	VectorNormalize( &vy, &vy ) ;
	VectorNormalize( &vz, &vz ) ;

	// �r���[�s��̍쐬
	Out->m[0][0] = vx.x ; Out->m[0][1] = vy.x ; Out->m[0][2] = vz.x ; Out->m[0][3] = 0.0f ;
	Out->m[1][0] = vx.y ; Out->m[1][1] = vy.y ; Out->m[1][2] = vz.y ; Out->m[1][3] = 0.0f ;
	Out->m[2][0] = vx.z ; Out->m[2][1] = vy.z ; Out->m[2][2] = vz.z ; Out->m[2][3] = 0.0f ;
	Out->m[3][3] = 1.0f ;

	Out->m[3][0] = - ( Eye->x * vx.x + Eye->y * vx.y + Eye->z * vx.z ) ;
	Out->m[3][1] = - ( Eye->x * vy.x + Eye->y * vy.y + Eye->z * vy.z ) ;
	Out->m[3][2] = - ( Eye->x * vz.x + Eye->y * vz.y + Eye->z * vz.z ) ;

/*
	VECTOR v, v2 ;

	// ���s�������� Z ���̊��x�N�g�����擾����B����́A
	// �����_�ɑ΂��鎋�_�Ƃ͈قȂ�B
	VECTOR vView = VectorSa( *At, *Eye ) ;
 
	float fLength = ( float )_SQRT( vView.x * vView.x + vView.y * vView.y + vView.z * vView.z ) ;
	if( fLength < 1e-6f ) return -1;
 
	// z ���̊��x�N�g���𐳋K������B
	vView.x /= fLength;
	vView.y /= fLength;
	vView.z /= fLength;
 
	// ���ς��擾���āA������x�N�g���ɑ΂���AZ ���̊��x�N�g����
	// �ˉe���v�Z����B���̎ˉe�� y ���̊��x�N�g���ł���B
	float fDotProduct = Up->x * vView.x + Up->y * vView.y + Up->z * vView.z ;

	v.x = vView.x * fDotProduct ;
	v.y = vView.y * fDotProduct ;
	v.z = vView.z * fDotProduct ;
	VECTOR vUp = VectorSa( *Up, v ) ;
 
	// �s���ȏ�����x�N�g������͂������߂�
	// �x�N�g�����[���ɋ߂��Ȃ����ꍇ�́A�x�N�g�����f�t�H���g�l�ɂ���B
	if( 1e-6f > ( fLength = ( float )_SQRT( vUp.x * vUp.x + vUp.y * vUp.y + vUp.z * vUp.z ) ) )
	{
		v.x = vView.x * vView.y ;
		v.y = vView.y * vView.y ;
		v.z = vView.z * vView.y ;

		v2.x = 0.0f ; v2.y = 1.0f ; v2.z = 0.0f ;
		vUp = VectorSa( v2, v ) ;
 
		// �x�N�g�����Ȃ��[���ɋ߂���΁A�ʂ̎��𗘗p����B
		if( 1e-6f > ( fLength = ( float )_SQRT( vUp.x * vUp.x + vUp.y * vUp.y + vUp.z * vUp.z ) ) )
		{
			v.x = vView.x * vView.z ;
			v.y = vView.y * vView.z ;
			v.z = vView.z * vView.z ;

			v2.x = 0.0f ; v2.y = 0.0f ; v2.z = 1.0f ;
			vUp = VectorSa( v2, v ) ;
 
			if( 1e-6f > ( fLength = ( float )_SQRT( vUp.x * vUp.x + vUp.y * vUp.y + vUp.z * vUp.z ) ) )
				return -1 ;
		}
	}
 
	// y ���̊��x�N�g���𐳋K������B
	vUp.x /= fLength;
	vUp.y /= fLength;
	vUp.z /= fLength;
 
	// x ���̊��x�N�g���́Ay ������� z ���̊��x�N�g����
	// �O�ςƂ��ĊȒP�ɋ��߂���B
	VECTOR vRight = VectorGaiseki( vUp, vView );
	
	// �s��̍쐬���J�n����B�擪�� 3 �s�ɂ́A
	// �r���[�𒍎��_�ŉ�]���邽�߂̊��x�N�g�����i�[����B
	Out->m[0][0] = vRight.x;	Out->m[0][1] = vUp.x;	Out->m[0][2] = vView.x;	Out->m[0][3] = 0.0f;
	Out->m[1][0] = vRight.y;	Out->m[1][1] = vUp.y;	Out->m[1][2] = vView.y;	Out->m[1][3] = 0.0f;
	Out->m[2][0] = vRight.z;	Out->m[2][1] = vUp.z;	Out->m[2][2] = vView.z;	Out->m[2][3] = 0.0f;
	Out->m[3][3] = 1.0f;

  
	// �ړ��l�����s���� (��]�͂܂����_�ɂ���)�B
	Out->m[3][0] = - ( Eye->x * vRight.x + Eye->y * vRight.y + Eye->z * vRight.z ) ;
	Out->m[3][1] = - ( Eye->x * vUp.x + Eye->y * vUp.y + Eye->z * vUp.z ) ;
	Out->m[3][2] = - ( Eye->x * vView.x + Eye->y * vView.y + Eye->z * vView.z );
*/
	return 0;
}

// �r���[�s����쐬����
extern int CreateLookAtMatrixD( MATRIX_D *Out, const VECTOR_D *Eye, const VECTOR_D *At, const VECTOR_D *Up )
{
	VECTOR_D va, vu, vx, vy, vz ;

	// ���_�������_�̃x�N�g���𓾂�
	va = VectorSaD( *At, *Eye ) ;

	// ���_�ɐ����ȃx�N�g���𓾂�
	vu = *Up ;

	// �V�w���̃x�N�g�����Z�o
	vx = VectorGaisekiD( vu, va ) ;

	// �V�x���̃x�N�g�����Z�o
	vy = VectorGaisekiD( va, vx ) ;

	// �V�y���̃x�N�g�����Z�b�g
	vz = va ;

	// �e�x�N�g���̐��K��
	VectorNormalizeD( &vx, &vx ) ;
	VectorNormalizeD( &vy, &vy ) ;
	VectorNormalizeD( &vz, &vz ) ;

	// �r���[�s��̍쐬
	Out->m[0][0] = vx.x ; Out->m[0][1] = vy.x ; Out->m[0][2] = vz.x ; Out->m[0][3] = 0.0 ;
	Out->m[1][0] = vx.y ; Out->m[1][1] = vy.y ; Out->m[1][2] = vz.y ; Out->m[1][3] = 0.0 ;
	Out->m[2][0] = vx.z ; Out->m[2][1] = vy.z ; Out->m[2][2] = vz.z ; Out->m[2][3] = 0.0 ;
	Out->m[3][3] = 1.0 ;

	Out->m[3][0] = - ( Eye->x * vx.x + Eye->y * vx.y + Eye->z * vx.z ) ;
	Out->m[3][1] = - ( Eye->x * vy.x + Eye->y * vy.y + Eye->z * vy.z ) ;
	Out->m[3][2] = - ( Eye->x * vz.x + Eye->y * vz.y + Eye->z * vz.z ) ;

	return 0;
}

// �r���[�s����쐬����
extern int CreateLookAtMatrix2( MATRIX *Out, const VECTOR *Eye, double XZAngle, double Oira )
{
	VECTOR At, Up ;
	float Sin, Cos, x ;

	At.x = 1.0f ;
	At.y = 0.0f ;
	At.z = 0.0f ;

	Up.x = 0.0f ;
	Up.y = 1.0f ;
	Up.z = 0.0f ;

	// �I�C���[��]
	_SINCOS_PLATFORM( (float)Oira, &Sin, &Cos ) ;
//	Sin = ( float )( sin( Oira ) ) ;
//	Cos = ( float )( cos( Oira ) ) ;

	x = At.x * Cos - At.y * Sin ;
	At.y = At.x * Sin + At.y * Cos ;
	At.x = x ;

	x = Up.x * Cos - Up.y * Sin ;
	Up.y = Up.x * Sin + Up.y * Cos ;
	Up.x = x ;


	// �w�y��]
	_SINCOS_PLATFORM( (float)XZAngle, &Sin, &Cos ) ;
//	Sin = ( float )( sin( XZAngle ) ) ;
//	Cos = ( float )( cos( XZAngle ) ) ;

	x = At.x * Cos - At.z * Sin ;
	At.z = At.x * Sin + At.z * Cos ;
	At.x = x ;

	x = Up.x * Cos - Up.z * Sin ;
	Up.z = Up.x * Sin + Up.z * Cos ;
	Up.x = x ;


	// ���s�ړ�
	At.x += Eye->x ;
	At.y += Eye->y ;
	At.z += Eye->z ;

/*	Up.x += Eye->x ;
	Up.y += Eye->y ;
	Up.z += Eye->z ;
*/

	// �s��쐬
	return CreateLookAtMatrix( Out, Eye, &At, &Up ) ; 
}

// �r���[�s����쐬����
extern int CreateLookAtMatrix2D( MATRIX_D *Out, const VECTOR_D *Eye, double XZAngle, double Oira )
{
	VECTOR_D At, Up ;
	double Sin, Cos, x ;

	At.x = 1.0 ;
	At.y = 0.0 ;
	At.z = 0.0 ;

	Up.x = 0.0 ;
	Up.y = 1.0 ;
	Up.z = 0.0 ;

	// �I�C���[��]
	_SINCOSD( Oira, &Sin, &Cos ) ;

	x = At.x * Cos - At.y * Sin ;
	At.y = At.x * Sin + At.y * Cos ;
	At.x = x ;

	x = Up.x * Cos - Up.y * Sin ;
	Up.y = Up.x * Sin + Up.y * Cos ;
	Up.x = x ;


	// �w�y��]
	_SINCOSD( XZAngle, &Sin, &Cos ) ;

	x = At.x * Cos - At.z * Sin ;
	At.z = At.x * Sin + At.z * Cos ;
	At.x = x ;

	x = Up.x * Cos - Up.z * Sin ;
	Up.z = Up.x * Sin + Up.z * Cos ;
	Up.x = x ;


	// ���s�ړ�
	At.x += Eye->x ;
	At.y += Eye->y ;
	At.z += Eye->z ;

	// �s��쐬
	return CreateLookAtMatrixD( Out, Eye, &At, &Up ) ; 
}


// �r���[�s����쐬����(�E����W�n�p)
extern int CreateLookAtMatrixRH( MATRIX *Out, const VECTOR *Eye, const VECTOR *At, const VECTOR *Up )
{
	VECTOR va, vu, vx, vy, vz ;

	// ���_�������_�̃x�N�g���𓾂�
	va = VectorSa( *Eye, *At ) ;

	// ���_�ɐ����ȃx�N�g���𓾂�
	vu = *Up ;

	// �V�w���̃x�N�g�����Z�o
	vx = VectorGaiseki( vu, va ) ;

	// �V�x���̃x�N�g�����Z�o
	vy = VectorGaiseki( va, vx ) ;

	// �V�y���̃x�N�g�����Z�b�g
	vz = va ;

	// �e�x�N�g���̐��K��
	VectorNormalize( &vx, &vx ) ;
	VectorNormalize( &vy, &vy ) ;
	VectorNormalize( &vz, &vz ) ;

	// �r���[�s��̍쐬
	Out->m[0][0] = vx.x ; Out->m[0][1] = vy.x ; Out->m[0][2] = vz.x ; Out->m[0][3] = 0.0f ;
	Out->m[1][0] = vx.y ; Out->m[1][1] = vy.y ; Out->m[1][2] = vz.y ; Out->m[1][3] = 0.0f ;
	Out->m[2][0] = vx.z ; Out->m[2][1] = vy.z ; Out->m[2][2] = vz.z ; Out->m[2][3] = 0.0f ;
	Out->m[3][3] = 1.0f ;

	Out->m[3][0] = - ( Eye->x * vx.x + Eye->y * vx.y + Eye->z * vx.z ) ;
	Out->m[3][1] = - ( Eye->x * vy.x + Eye->y * vy.y + Eye->z * vy.z ) ;
	Out->m[3][2] = - ( Eye->x * vz.x + Eye->y * vz.y + Eye->z * vz.z ) ;

	return 0;
}

// �r���[�s����쐬����(�E����W�n�p)
extern int CreateLookAtMatrixRHD( MATRIX_D *Out, const VECTOR_D *Eye, const VECTOR_D *At, const VECTOR_D *Up )
{
	VECTOR_D va, vu, vx, vy, vz ;

	// ���_�������_�̃x�N�g���𓾂�
	va = VectorSaD( *Eye, *At ) ;

	// ���_�ɐ����ȃx�N�g���𓾂�
	vu = *Up ;

	// �V�w���̃x�N�g�����Z�o
	vx = VectorGaisekiD( vu, va ) ;

	// �V�x���̃x�N�g�����Z�o
	vy = VectorGaisekiD( va, vx ) ;

	// �V�y���̃x�N�g�����Z�b�g
	vz = va ;

	// �e�x�N�g���̐��K��
	VectorNormalizeD( &vx, &vx ) ;
	VectorNormalizeD( &vy, &vy ) ;
	VectorNormalizeD( &vz, &vz ) ;

	// �r���[�s��̍쐬
	Out->m[0][0] = vx.x ; Out->m[0][1] = vy.x ; Out->m[0][2] = vz.x ; Out->m[0][3] = 0.0 ;
	Out->m[1][0] = vx.y ; Out->m[1][1] = vy.y ; Out->m[1][2] = vz.y ; Out->m[1][3] = 0.0 ;
	Out->m[2][0] = vx.z ; Out->m[2][1] = vy.z ; Out->m[2][2] = vz.z ; Out->m[2][3] = 0.0 ;
	Out->m[3][3] = 1.0 ;

	Out->m[3][0] = - ( Eye->x * vx.x + Eye->y * vx.y + Eye->z * vx.z ) ;
	Out->m[3][1] = - ( Eye->x * vy.x + Eye->y * vy.y + Eye->z * vy.z ) ;
	Out->m[3][2] = - ( Eye->x * vz.x + Eye->y * vz.y + Eye->z * vz.z ) ;

	return 0;
}


// �s��̐ς����߂�
extern int CreateMultiplyMatrix( MATRIX *Out, const MATRIX *In1, const MATRIX *In2 )
{
	MATRIX OM ;
	
	OM.m[0][0] = In1->m[0][0] * In2->m[0][0] + In1->m[0][1] * In2->m[1][0] + In1->m[0][2] * In2->m[2][0] + In1->m[0][3] * In2->m[3][0] ;
	OM.m[0][1] = In1->m[0][0] * In2->m[0][1] + In1->m[0][1] * In2->m[1][1] + In1->m[0][2] * In2->m[2][1] + In1->m[0][3] * In2->m[3][1] ;
	OM.m[0][2] = In1->m[0][0] * In2->m[0][2] + In1->m[0][1] * In2->m[1][2] + In1->m[0][2] * In2->m[2][2] + In1->m[0][3] * In2->m[3][2] ;
	OM.m[0][3] = In1->m[0][0] * In2->m[0][3] + In1->m[0][1] * In2->m[1][3] + In1->m[0][2] * In2->m[2][3] + In1->m[0][3] * In2->m[3][3] ;

	OM.m[1][0] = In1->m[1][0] * In2->m[0][0] + In1->m[1][1] * In2->m[1][0] + In1->m[1][2] * In2->m[2][0] + In1->m[1][3] * In2->m[3][0] ;
	OM.m[1][1] = In1->m[1][0] * In2->m[0][1] + In1->m[1][1] * In2->m[1][1] + In1->m[1][2] * In2->m[2][1] + In1->m[1][3] * In2->m[3][1] ;
	OM.m[1][2] = In1->m[1][0] * In2->m[0][2] + In1->m[1][1] * In2->m[1][2] + In1->m[1][2] * In2->m[2][2] + In1->m[1][3] * In2->m[3][2] ;
	OM.m[1][3] = In1->m[1][0] * In2->m[0][3] + In1->m[1][1] * In2->m[1][3] + In1->m[1][2] * In2->m[2][3] + In1->m[1][3] * In2->m[3][3] ;

	OM.m[2][0] = In1->m[2][0] * In2->m[0][0] + In1->m[2][1] * In2->m[1][0] + In1->m[2][2] * In2->m[2][0] + In1->m[2][3] * In2->m[3][0] ;
	OM.m[2][1] = In1->m[2][0] * In2->m[0][1] + In1->m[2][1] * In2->m[1][1] + In1->m[2][2] * In2->m[2][1] + In1->m[2][3] * In2->m[3][1] ;
	OM.m[2][2] = In1->m[2][0] * In2->m[0][2] + In1->m[2][1] * In2->m[1][2] + In1->m[2][2] * In2->m[2][2] + In1->m[2][3] * In2->m[3][2] ;
	OM.m[2][3] = In1->m[2][0] * In2->m[0][3] + In1->m[2][1] * In2->m[1][3] + In1->m[2][2] * In2->m[2][3] + In1->m[2][3] * In2->m[3][3] ;

	OM.m[3][0] = In1->m[3][0] * In2->m[0][0] + In1->m[3][1] * In2->m[1][0] + In1->m[3][2] * In2->m[2][0] + In1->m[3][3] * In2->m[3][0] ;
	OM.m[3][1] = In1->m[3][0] * In2->m[0][1] + In1->m[3][1] * In2->m[1][1] + In1->m[3][2] * In2->m[2][1] + In1->m[3][3] * In2->m[3][1] ;
	OM.m[3][2] = In1->m[3][0] * In2->m[0][2] + In1->m[3][1] * In2->m[1][2] + In1->m[3][2] * In2->m[2][2] + In1->m[3][3] * In2->m[3][2] ;
	OM.m[3][3] = In1->m[3][0] * In2->m[0][3] + In1->m[3][1] * In2->m[1][3] + In1->m[3][2] * In2->m[2][3] + In1->m[3][3] * In2->m[3][3] ;

	if( Out ) *Out = OM ;

	// �I��
	return 0 ;
}


// �s��̐ς����߂�
extern int CreateMultiplyMatrixD( MATRIX_D *Out, const MATRIX_D *In1, const MATRIX_D *In2 )
{
	MATRIX_D OM ;
	
	OM.m[0][0] = In1->m[0][0] * In2->m[0][0] + In1->m[0][1] * In2->m[1][0] + In1->m[0][2] * In2->m[2][0] + In1->m[0][3] * In2->m[3][0] ;
	OM.m[0][1] = In1->m[0][0] * In2->m[0][1] + In1->m[0][1] * In2->m[1][1] + In1->m[0][2] * In2->m[2][1] + In1->m[0][3] * In2->m[3][1] ;
	OM.m[0][2] = In1->m[0][0] * In2->m[0][2] + In1->m[0][1] * In2->m[1][2] + In1->m[0][2] * In2->m[2][2] + In1->m[0][3] * In2->m[3][2] ;
	OM.m[0][3] = In1->m[0][0] * In2->m[0][3] + In1->m[0][1] * In2->m[1][3] + In1->m[0][2] * In2->m[2][3] + In1->m[0][3] * In2->m[3][3] ;

	OM.m[1][0] = In1->m[1][0] * In2->m[0][0] + In1->m[1][1] * In2->m[1][0] + In1->m[1][2] * In2->m[2][0] + In1->m[1][3] * In2->m[3][0] ;
	OM.m[1][1] = In1->m[1][0] * In2->m[0][1] + In1->m[1][1] * In2->m[1][1] + In1->m[1][2] * In2->m[2][1] + In1->m[1][3] * In2->m[3][1] ;
	OM.m[1][2] = In1->m[1][0] * In2->m[0][2] + In1->m[1][1] * In2->m[1][2] + In1->m[1][2] * In2->m[2][2] + In1->m[1][3] * In2->m[3][2] ;
	OM.m[1][3] = In1->m[1][0] * In2->m[0][3] + In1->m[1][1] * In2->m[1][3] + In1->m[1][2] * In2->m[2][3] + In1->m[1][3] * In2->m[3][3] ;

	OM.m[2][0] = In1->m[2][0] * In2->m[0][0] + In1->m[2][1] * In2->m[1][0] + In1->m[2][2] * In2->m[2][0] + In1->m[2][3] * In2->m[3][0] ;
	OM.m[2][1] = In1->m[2][0] * In2->m[0][1] + In1->m[2][1] * In2->m[1][1] + In1->m[2][2] * In2->m[2][1] + In1->m[2][3] * In2->m[3][1] ;
	OM.m[2][2] = In1->m[2][0] * In2->m[0][2] + In1->m[2][1] * In2->m[1][2] + In1->m[2][2] * In2->m[2][2] + In1->m[2][3] * In2->m[3][2] ;
	OM.m[2][3] = In1->m[2][0] * In2->m[0][3] + In1->m[2][1] * In2->m[1][3] + In1->m[2][2] * In2->m[2][3] + In1->m[2][3] * In2->m[3][3] ;

	OM.m[3][0] = In1->m[3][0] * In2->m[0][0] + In1->m[3][1] * In2->m[1][0] + In1->m[3][2] * In2->m[2][0] + In1->m[3][3] * In2->m[3][0] ;
	OM.m[3][1] = In1->m[3][0] * In2->m[0][1] + In1->m[3][1] * In2->m[1][1] + In1->m[3][2] * In2->m[2][1] + In1->m[3][3] * In2->m[3][1] ;
	OM.m[3][2] = In1->m[3][0] * In2->m[0][2] + In1->m[3][1] * In2->m[1][2] + In1->m[3][2] * In2->m[2][2] + In1->m[3][3] * In2->m[3][2] ;
	OM.m[3][3] = In1->m[3][0] * In2->m[0][3] + In1->m[3][1] * In2->m[1][3] + In1->m[3][2] * In2->m[2][3] + In1->m[3][3] * In2->m[3][3] ;

	if( Out ) *Out = OM ;

	// �I��
	return 0 ;
}

// �ˉe�s����쐬����
extern int CreatePerspectiveFovMatrix( MATRIX *Out, float fov, float zn, float zf, float aspect  )
{
	float Sin, Cos ;

	if( aspect < 0.0f )
	{
		if( MathScreenSizeX == 0 || MathScreenSizeY == 0 )
		{
			aspect = ( float )DEFAULT_SCREEN_SIZE_Y / ( float )DEFAULT_SCREEN_SIZE_X ;
		}
		else
		{
			aspect = ( float )MathScreenSizeY / ( float )MathScreenSizeX ;
		}
	}

	_SINCOS_PLATFORM( fov / 2, &Sin, &Cos ) ;

    if( _FABS( zf - zn ) < 0.0001f ) return -1 ;
    if( _FABS( Sin )     < 0.0001f ) return -1 ;

//  float w = aspect * ( cosf(fov/2)/sinf(fov/2) );
//  float h =   1.0f * ( cosf(fov/2)/sinf(fov/2) );
	float w = aspect * ( Cos / Sin ) ;
	float h =   1.0f * ( Cos / Sin ) ;
    float Q = zf / ( zf - zn ) ;

    _MEMSET( Out, 0, sizeof(MATRIX) );
    Out->m[0][0] = w ;
    Out->m[1][1] = h ;
    Out->m[2][2] = Q ;
    Out->m[3][2] = -Q * zn ;
    Out->m[2][3] = 1.0f ;
	Out->m[3][3] = 0.0f ;

    return 0;
}

// �ˉe�s����쐬����
extern int CreatePerspectiveFovMatrixD( MATRIX_D *Out, double fov, double zn, double zf, double aspect  )
{
	double Sin, Cos ;

	if( aspect < 0.0 )
	{
		if( MathScreenSizeX == 0 || MathScreenSizeY == 0 )
		{
			aspect = ( float )DEFAULT_SCREEN_SIZE_Y / ( float )DEFAULT_SCREEN_SIZE_X ;
		}
		else
		{
			aspect = ( double )MathScreenSizeY / ( double )MathScreenSizeX ;
		}
	}

	_SINCOSD( fov / 2, &Sin, &Cos ) ;

    if( _DABS( zf - zn ) < 0.0001 ) return -1 ;
    if( _DABS( Sin )     < 0.0001 ) return -1 ;

	double w = aspect * ( Cos / Sin ) ;
	double h =    1.0 * ( Cos / Sin ) ;
    double Q = zf / ( zf - zn ) ;

    _MEMSET( Out, 0, sizeof( MATRIX_D ) );
    Out->m[0][0] = w ;
    Out->m[1][1] = h ;
    Out->m[2][2] = Q ;
    Out->m[3][2] = -Q * zn ;
    Out->m[2][3] = 1.0 ;
	Out->m[3][3] = 0.0 ;

    return 0;
}

// �ˉe�s����쐬����(�E����W�n�p)
extern int CreatePerspectiveFovMatrixRH( MATRIX *Out, float fov, float zn, float zf, float aspect )
{
	float Sin, Cos ;

	if( aspect < 0.0f )
	{
		if( MathScreenSizeX == 0 || MathScreenSizeY == 0 )
		{
			aspect = ( float )DEFAULT_SCREEN_SIZE_Y / ( float )DEFAULT_SCREEN_SIZE_X ;
		}
		else
		{
			aspect = ( float )MathScreenSizeY / ( float )MathScreenSizeX ;
		}
	}

	_SINCOS_PLATFORM( fov / 2, &Sin, &Cos ) ;

    if( _FABS(zn-zf) < 0.0001f ) return -1 ;
    if( _FABS( Sin ) < 0.0001f ) return -1;

//  float w = aspect * ( cosf(fov/2)/sinf(fov/2) );
//  float h =   1.0f * ( cosf(fov/2)/sinf(fov/2) );
	float w = aspect * ( Cos / Sin ) ;
	float h =   1.0f * ( Cos / Sin ) ;
    float Q = zf / ( zn - zf ) ;

    _MEMSET( Out, 0, sizeof(MATRIX) );
    Out->m[0][0] = w;
    Out->m[1][1] = h;
    Out->m[2][2] = Q;
    Out->m[3][2] = Q*zn;
    Out->m[2][3] = -1.0f;
	Out->m[3][3] = 0.0f ;

    return 0;
}

// �ˉe�s����쐬����(�E����W�n�p)
extern int CreatePerspectiveFovMatrixRHD( MATRIX_D *Out, double fov, double zn, double zf, double aspect )
{
	double Sin, Cos ;

	if( aspect < 0.0 )
	{
		if( MathScreenSizeX == 0 || MathScreenSizeY == 0 )
		{
			aspect = ( float )DEFAULT_SCREEN_SIZE_Y / ( float )DEFAULT_SCREEN_SIZE_X ;
		}
		else
		{
			aspect = ( double )MathScreenSizeY / ( double )MathScreenSizeX ;
		}
	}

	_SINCOSD( fov / 2, &Sin, &Cos ) ;

    if( _DABS(zn-zf) < 0.0001 ) return -1 ;
    if( _DABS( Sin ) < 0.0001 ) return -1;

	double w = aspect * ( Cos / Sin ) ;
	double h =    1.0 * ( Cos / Sin ) ;
    double Q = zf / ( zn - zf ) ;

    _MEMSET( Out, 0, sizeof(MATRIX_D) ) ;
    Out->m[0][0] = w ;
    Out->m[1][1] = h ;
    Out->m[2][2] = Q ;
    Out->m[3][2] = Q*zn ;
    Out->m[2][3] = -1.0 ;
	Out->m[3][3] = 0.0 ;

    return 0;
}

// ���ˉe�s����쐬����
extern int CreateOrthoMatrix( MATRIX *Out, float size, float zn, float zf, float aspect )
{
	float w, h ;

	if( aspect < 0.0f )
	{
		if( MathScreenSizeX == 0 || MathScreenSizeY == 0 )
		{
			aspect = ( float )DEFAULT_SCREEN_SIZE_Y / ( float )DEFAULT_SCREEN_SIZE_X ;
		}
		else
		{
			aspect = ( float )MathScreenSizeY / ( float )MathScreenSizeX ;
		}
	}

    if( _FABS( zn - zf ) < 0.0001f ) return -1 ;

	w = size / aspect ;
	h = size ;

    _MEMSET( Out, 0, sizeof( MATRIX ) ) ;
    Out->m[ 0 ][ 0 ] = 2.0f / w ;
    Out->m[ 1 ][ 1 ] = 2.0f / h ;
    Out->m[ 2 ][ 2 ] = 1.0f / ( zf - zn ) ;
    Out->m[ 3 ][ 2 ] = zn   / ( zn - zf ) ;
	Out->m[ 3 ][ 3 ] = 1.0f ;

    return 0;
}

// ���ˉe�s����쐬����
extern int CreateOrthoMatrixD( MATRIX_D *Out, double size, double zn, double zf, double aspect )
{
	double w, h ;

	if( aspect < 0.0 )
	{
		if( MathScreenSizeX == 0 || MathScreenSizeY == 0 )
		{
			aspect = ( float )DEFAULT_SCREEN_SIZE_Y / ( float )DEFAULT_SCREEN_SIZE_X ;
		}
		else
		{
			aspect = ( double )MathScreenSizeY / ( double )MathScreenSizeX ;
		}
	}

    if( _DABS( zn - zf ) < 0.0001 ) return -1 ;

	w = size / aspect ;
	h = size ;

    _MEMSET( Out, 0, sizeof( MATRIX_D ) ) ;
    Out->m[ 0 ][ 0 ] = 2.0 / w ;
    Out->m[ 1 ][ 1 ] = 2.0 / h ;
    Out->m[ 2 ][ 2 ] = 1.0 / ( zf - zn ) ;
    Out->m[ 3 ][ 2 ] = zn  / ( zn - zf ) ;
	Out->m[ 3 ][ 3 ] = 1.0 ;

    return 0;
}

// ���ˉe�s����쐬����(�E����W�n�p)
extern int CreateOrthoMatrixRH( MATRIX *Out, float size, float zn, float zf, float aspect )
{
	float w, h ;

	if( aspect < 0.0f )
	{
		if( MathScreenSizeX == 0 || MathScreenSizeY == 0 )
		{
			aspect = ( float )DEFAULT_SCREEN_SIZE_Y / ( float )DEFAULT_SCREEN_SIZE_X ;
		}
		else
		{
			aspect = ( float )MathScreenSizeY / ( float )MathScreenSizeX ;
		}
	}

    if( _FABS( zn - zf ) < 0.0001f ) return -1 ;

	w = size / aspect ;
	h = size ;

    _MEMSET( Out, 0, sizeof( MATRIX ) ) ;
    Out->m[ 0 ][ 0 ] = 2.0f / w ;
    Out->m[ 1 ][ 1 ] = 2.0f / h ;
    Out->m[ 2 ][ 2 ] = 1.0f / ( zn - zf ) ;
    Out->m[ 3 ][ 2 ] = zn   / ( zn - zf ) ;
	Out->m[ 3 ][ 3 ] = 1.0f ;

    return 0;
}

// ���ˉe�s����쐬����(�E����W�n�p)
extern int CreateOrthoMatrixRHD( MATRIX_D *Out, double size, double zn, double zf, double aspect )
{
	double w, h ;

	if( aspect < 0.0 )
	{
		if( MathScreenSizeX == 0 || MathScreenSizeY == 0 )
		{
			aspect = ( float )DEFAULT_SCREEN_SIZE_Y / ( float )DEFAULT_SCREEN_SIZE_X ;
		}
		else
		{
			aspect = ( double )MathScreenSizeY / ( double )MathScreenSizeX ;
		}
	}

    if( _DABS( zn - zf ) < 0.0001 ) return -1 ;

	w = size / aspect ;
	h = size ;

    _MEMSET( Out, 0, sizeof( MATRIX_D ) ) ;
    Out->m[ 0 ][ 0 ] = 2.0 / w ;
    Out->m[ 1 ][ 1 ] = 2.0 / h ;
    Out->m[ 2 ][ 2 ] = 1.0 / ( zn - zf ) ;
    Out->m[ 3 ][ 2 ] = zn   / ( zn - zf ) ;
	Out->m[ 3 ][ 3 ] = 1.0 ;

    return 0;
}

// �X�P�[�����O�s����쐬����
extern int CreateScalingMatrix( MATRIX *Out, float sx, float sy, float sz )
{
	_MEMSET( Out, 0, sizeof( MATRIX ) ) ;

	Out->m[0][0] = sx ;
	Out->m[1][1] = sy ;
	Out->m[2][2] = sz ;
	Out->m[3][3] = 1.0f ;

	// �I��
	return 0 ;
}

// �X�P�[�����O�s����쐬����
extern int CreateScalingMatrixD( MATRIX_D *Out, double sx, double sy, double sz )
{
	_MEMSET( Out, 0, sizeof( MATRIX_D ) ) ;

	Out->m[0][0] = sx ;
	Out->m[1][1] = sy ;
	Out->m[2][2] = sz ;
	Out->m[3][3] = 1.0 ;

	// �I��
	return 0 ;
}

// �w���𒆐S�Ƃ�����]�s����쐬����
extern int CreateRotationXMatrix( MATRIX *Out, float Angle )
{
	float Sin, Cos ;

	_SINCOS_PLATFORM( Angle, &Sin, &Cos ) ;
//	Sin = sinf( Angle ) ;
//	Cos = cosf( Angle ) ;

	_MEMSET( Out, 0, sizeof( MATRIX ) ) ;
	Out->m[0][0] = 1.0f ;
	Out->m[1][1] = Cos ;
	Out->m[1][2] = Sin ;
	Out->m[2][1] = -Sin ;
	Out->m[2][2] = Cos ;
	Out->m[3][3] = 1.0f ;

	return 0 ;
}

// �w���𒆐S�Ƃ�����]�s����쐬����
extern int CreateRotationXMatrixD( MATRIX_D *Out, double Angle )
{
	double Sin, Cos ;

	_SINCOSD( Angle, &Sin, &Cos ) ;

	_MEMSET( Out, 0, sizeof( MATRIX_D ) ) ;
	Out->m[0][0] =  1.0 ;
	Out->m[1][1] =  Cos ;
	Out->m[1][2] =  Sin ;
	Out->m[2][1] = -Sin ;
	Out->m[2][2] =  Cos ;
	Out->m[3][3] =  1.0 ;

	return 0 ;
}

// �x���𒆐S�Ƃ�����]�s����쐬����
extern int CreateRotationYMatrix( MATRIX *Out, float Angle )
{
	float Sin, Cos ;

	_SINCOS_PLATFORM( Angle, &Sin, &Cos ) ;
//	Sin = sinf( Angle ) ;
//	Cos = cosf( Angle ) ;

	_MEMSET( Out, 0, sizeof( MATRIX ) ) ;
	Out->m[0][0] = Cos ;
	Out->m[0][2] = -Sin ;
	Out->m[2][0] = Sin ;
	Out->m[2][2] = Cos ;
	Out->m[1][1] = 1.0f ;
	Out->m[3][3] = 1.0f ;

	return 0 ;
}

// �x���𒆐S�Ƃ�����]�s����쐬����
extern int CreateRotationYMatrixD( MATRIX_D *Out, double Angle )
{
	double Sin, Cos ;

	_SINCOSD( Angle, &Sin, &Cos ) ;

	_MEMSET( Out, 0, sizeof( MATRIX_D ) ) ;
	Out->m[0][0] =  Cos ;
	Out->m[0][2] = -Sin ;
	Out->m[2][0] =  Sin ;
	Out->m[2][2] =  Cos ;
	Out->m[1][1] =  1.0 ;
	Out->m[3][3] =  1.0 ;

	return 0 ;
}

// �y���𒆐S�Ƃ�����]�s����쐬����
extern int CreateRotationZMatrix( MATRIX *Out, float Angle )
{
	float Sin, Cos ;

	_SINCOS_PLATFORM( Angle, &Sin, &Cos ) ;
//	Sin = sinf( Angle ) ;
//	Cos = cosf( Angle ) ;

	_MEMSET( Out, 0, sizeof( MATRIX ) ) ;
	Out->m[0][0] = Cos ;
	Out->m[0][1] = Sin ;
	Out->m[1][0] = -Sin ;
	Out->m[1][1] = Cos ;
	Out->m[2][2] = 1.0f ;
	Out->m[3][3] = 1.0f ;

	return 0 ;
}

// �y���𒆐S�Ƃ�����]�s����쐬����
extern int CreateRotationZMatrixD( MATRIX_D *Out, double Angle )
{
	double Sin, Cos ;

	_SINCOSD( Angle, &Sin, &Cos ) ;

	_MEMSET( Out, 0, sizeof( MATRIX_D ) ) ;
	Out->m[0][0] =  Cos ;
	Out->m[0][1] =  Sin ;
	Out->m[1][0] = -Sin ;
	Out->m[1][1] =  Cos ;
	Out->m[2][2] =  1.0 ;
	Out->m[3][3] =  1.0 ;

	return 0 ;
}

// ���s�ړ��s����쐬����
extern int CreateTranslationMatrix( MATRIX *Out, float x, float y, float z )
{
	_MEMSET( Out, 0, sizeof( MATRIX ) ) ;
	Out->m[3][0] = x ;
	Out->m[3][1] = y ;
	Out->m[3][2] = z ;
	Out->m[3][3] = 1.0f ;
	Out->m[0][0] = 1.0f ;
	Out->m[1][1] = 1.0f ;
	Out->m[2][2] = 1.0f ;

	return 0 ;
}

// ���s�ړ��s����쐬����
extern int CreateTranslationMatrixD( MATRIX_D *Out, double x, double y, double z )
{
	_MEMSET( Out, 0, sizeof( MATRIX_D ) ) ;
	Out->m[3][0] = x ;
	Out->m[3][1] = y ;
	Out->m[3][2] = z ;
	Out->m[3][3] = 1.0 ;
	Out->m[0][0] = 1.0 ;
	Out->m[1][1] = 1.0 ;
	Out->m[2][2] = 1.0 ;

	return 0 ;
}

// �]�u�s����쐬����
extern int CreateTransposeMatrix( MATRIX *Out, const MATRIX *In )
{
	MATRIX I1 = *In ;
	Out->m[0][0] = I1.m[0][0] ;
	Out->m[0][1] = I1.m[1][0] ;
	Out->m[0][2] = I1.m[2][0] ;
	Out->m[0][3] = I1.m[3][0] ;

	Out->m[1][0] = I1.m[0][1] ;
	Out->m[1][1] = I1.m[1][1] ;
	Out->m[1][2] = I1.m[2][1] ;
	Out->m[1][3] = I1.m[3][1] ;

	Out->m[2][0] = I1.m[0][2] ;
	Out->m[2][1] = I1.m[1][2] ;
	Out->m[2][2] = I1.m[2][2] ;
	Out->m[2][3] = I1.m[3][2] ;

	Out->m[3][0] = I1.m[0][3] ;
	Out->m[3][1] = I1.m[1][3] ;
	Out->m[3][2] = I1.m[2][3] ;
	Out->m[3][3] = I1.m[3][3] ;

	return 0 ;
}

// �]�u�s����쐬����
extern int CreateTransposeMatrixD( MATRIX_D *Out, const MATRIX_D *In )
{
	MATRIX_D I1 = *In ;
	Out->m[0][0] = I1.m[0][0] ;
	Out->m[0][1] = I1.m[1][0] ;
	Out->m[0][2] = I1.m[2][0] ;
	Out->m[0][3] = I1.m[3][0] ;

	Out->m[1][0] = I1.m[0][1] ;
	Out->m[1][1] = I1.m[1][1] ;
	Out->m[1][2] = I1.m[2][1] ;
	Out->m[1][3] = I1.m[3][1] ;

	Out->m[2][0] = I1.m[0][2] ;
	Out->m[2][1] = I1.m[1][2] ;
	Out->m[2][2] = I1.m[2][2] ;
	Out->m[2][3] = I1.m[3][2] ;

	Out->m[3][0] = I1.m[0][3] ;
	Out->m[3][1] = I1.m[1][3] ;
	Out->m[3][2] = I1.m[2][3] ;
	Out->m[3][3] = I1.m[3][3] ;

	return 0 ;
}

// �t�s����쐬����
extern int CreateInverseMatrix( MATRIX *Out, const MATRIX *In )
{
#if 1
#if 0
	MATRIX matTemp;
	_MEMCPY( &matTemp, In, sizeof(MATRIX) );
	int i, j, k ;

	CreateIdentityMatrix( Out );

	//�|���o���@
	for( int i = 0 ; i < 4 ; i++ )
	{
		float buf = 1 / matTemp.m[i][i];
		for( j = 0 ; j < 4 ; j++ )
		{
			matTemp.m[i][j] *= buf;
			Out->m[i][j] *= buf;
		}
		for( j = 0 ; j < 4 ; j++ )
		{
			if( i != j )
			{
				buf = matTemp.m[j][i];
				for( k = 0 ; k < 4 ; k++ )
				{
					matTemp.m[j][k] -= matTemp.m[i][k] * buf;
					Out->m[j][k] -= Out->m[i][k] * buf;
				}
			}
		}
	}
	return 0 ;
#endif

#if 1
	MATRIX InT ;
	float detA ;

	InT = *In ;
	detA = 
	InT.m[ 0 ][ 0 ] * InT.m[ 1 ][ 1 ] * InT.m[ 2 ][ 2 ] * InT.m[ 3 ][ 3 ] +
	InT.m[ 0 ][ 0 ] * InT.m[ 1 ][ 2 ] * InT.m[ 2 ][ 3 ] * InT.m[ 3 ][ 1 ] +
	InT.m[ 0 ][ 0 ] * InT.m[ 1 ][ 3 ] * InT.m[ 2 ][ 1 ] * InT.m[ 3 ][ 2 ] +
	InT.m[ 0 ][ 1 ] * InT.m[ 1 ][ 0 ] * InT.m[ 2 ][ 3 ] * InT.m[ 3 ][ 2 ] +
	InT.m[ 0 ][ 1 ] * InT.m[ 1 ][ 2 ] * InT.m[ 2 ][ 0 ] * InT.m[ 3 ][ 3 ] +
	InT.m[ 0 ][ 1 ] * InT.m[ 1 ][ 3 ] * InT.m[ 2 ][ 2 ] * InT.m[ 3 ][ 0 ] +
	InT.m[ 0 ][ 2 ] * InT.m[ 1 ][ 0 ] * InT.m[ 2 ][ 1 ] * InT.m[ 3 ][ 3 ] +
	InT.m[ 0 ][ 2 ] * InT.m[ 1 ][ 1 ] * InT.m[ 2 ][ 3 ] * InT.m[ 3 ][ 0 ] +
	InT.m[ 0 ][ 2 ] * InT.m[ 1 ][ 3 ] * InT.m[ 2 ][ 0 ] * InT.m[ 3 ][ 1 ] +
	InT.m[ 0 ][ 3 ] * InT.m[ 1 ][ 0 ] * InT.m[ 2 ][ 2 ] * InT.m[ 3 ][ 1 ] +
	InT.m[ 0 ][ 3 ] * InT.m[ 1 ][ 1 ] * InT.m[ 2 ][ 0 ] * InT.m[ 3 ][ 2 ] +
	InT.m[ 0 ][ 3 ] * InT.m[ 1 ][ 2 ] * InT.m[ 2 ][ 1 ] * InT.m[ 3 ][ 0 ] -

	InT.m[ 0 ][ 0 ] * InT.m[ 1 ][ 1 ] * InT.m[ 2 ][ 3 ] * InT.m[ 3 ][ 2 ] -
	InT.m[ 0 ][ 0 ] * InT.m[ 1 ][ 2 ] * InT.m[ 2 ][ 1 ] * InT.m[ 3 ][ 3 ] -
	InT.m[ 0 ][ 0 ] * InT.m[ 1 ][ 3 ] * InT.m[ 2 ][ 2 ] * InT.m[ 3 ][ 1 ] -
	InT.m[ 0 ][ 1 ] * InT.m[ 1 ][ 0 ] * InT.m[ 2 ][ 2 ] * InT.m[ 3 ][ 3 ] -
	InT.m[ 0 ][ 1 ] * InT.m[ 1 ][ 2 ] * InT.m[ 2 ][ 3 ] * InT.m[ 3 ][ 0 ] -
	InT.m[ 0 ][ 1 ] * InT.m[ 1 ][ 3 ] * InT.m[ 2 ][ 0 ] * InT.m[ 3 ][ 2 ] -
	InT.m[ 0 ][ 2 ] * InT.m[ 1 ][ 0 ] * InT.m[ 2 ][ 3 ] * InT.m[ 3 ][ 1 ] -
	InT.m[ 0 ][ 2 ] * InT.m[ 1 ][ 1 ] * InT.m[ 2 ][ 0 ] * InT.m[ 3 ][ 3 ] -
	InT.m[ 0 ][ 2 ] * InT.m[ 1 ][ 3 ] * InT.m[ 2 ][ 1 ] * InT.m[ 3 ][ 0 ] -
	InT.m[ 0 ][ 3 ] * InT.m[ 1 ][ 0 ] * InT.m[ 2 ][ 1 ] * InT.m[ 3 ][ 2 ] -
	InT.m[ 0 ][ 3 ] * InT.m[ 1 ][ 1 ] * InT.m[ 2 ][ 2 ] * InT.m[ 3 ][ 0 ] -
	InT.m[ 0 ][ 3 ] * InT.m[ 1 ][ 2 ] * InT.m[ 2 ][ 0 ] * InT.m[ 3 ][ 1 ] ;

	if( detA < 0.0000001f && detA > - 0.0000001f )
	{
		return -1 ;
	}

	Out->m[ 0 ][ 0 ] =
	(
		InT.m[ 1 ][ 1 ] * InT.m[ 2 ][ 2 ] * InT.m[ 3 ][ 3 ] +
		InT.m[ 1 ][ 2 ] * InT.m[ 2 ][ 3 ] * InT.m[ 3 ][ 1 ] +
		InT.m[ 1 ][ 3 ] * InT.m[ 2 ][ 1 ] * InT.m[ 3 ][ 2 ] -
		InT.m[ 1 ][ 1 ] * InT.m[ 2 ][ 3 ] * InT.m[ 3 ][ 2 ] -
		InT.m[ 1 ][ 2 ] * InT.m[ 2 ][ 1 ] * InT.m[ 3 ][ 3 ] -
		InT.m[ 1 ][ 3 ] * InT.m[ 2 ][ 2 ] * InT.m[ 3 ][ 1 ]
	) / detA ;

	Out->m[ 0 ][ 1 ] =
	(	
		InT.m[ 0 ][ 1 ] * InT.m[ 2 ][ 3 ] * InT.m[ 3 ][ 2 ] +
		InT.m[ 0 ][ 2 ] * InT.m[ 2 ][ 1 ] * InT.m[ 3 ][ 3 ] +
		InT.m[ 0 ][ 3 ] * InT.m[ 2 ][ 2 ] * InT.m[ 3 ][ 1 ] -
		InT.m[ 0 ][ 1 ] * InT.m[ 2 ][ 2 ] * InT.m[ 3 ][ 3 ] -
		InT.m[ 0 ][ 2 ] * InT.m[ 2 ][ 3 ] * InT.m[ 3 ][ 1 ] -
		InT.m[ 0 ][ 3 ] * InT.m[ 2 ][ 1 ] * InT.m[ 3 ][ 2 ]
	) / detA ;

	Out->m[ 0 ][ 2 ] =
	(	
		InT.m[ 0 ][ 1 ] * InT.m[ 1 ][ 2 ] * InT.m[ 3 ][ 3 ] +
		InT.m[ 0 ][ 2 ] * InT.m[ 1 ][ 3 ] * InT.m[ 3 ][ 1 ] +
		InT.m[ 0 ][ 3 ] * InT.m[ 1 ][ 1 ] * InT.m[ 3 ][ 2 ] -
		InT.m[ 0 ][ 1 ] * InT.m[ 1 ][ 3 ] * InT.m[ 3 ][ 2 ] -
		InT.m[ 0 ][ 2 ] * InT.m[ 1 ][ 1 ] * InT.m[ 3 ][ 3 ] -
		InT.m[ 0 ][ 3 ] * InT.m[ 1 ][ 2 ] * InT.m[ 3 ][ 1 ]
	) / detA ;

	Out->m[ 0 ][ 3 ] =
	(	
		InT.m[ 0 ][ 1 ] * InT.m[ 1 ][ 3 ] * InT.m[ 2 ][ 2 ] +
		InT.m[ 0 ][ 2 ] * InT.m[ 1 ][ 1 ] * InT.m[ 2 ][ 3 ] +
		InT.m[ 0 ][ 3 ] * InT.m[ 1 ][ 2 ] * InT.m[ 2 ][ 1 ] -
		InT.m[ 0 ][ 1 ] * InT.m[ 1 ][ 2 ] * InT.m[ 2 ][ 3 ] -
		InT.m[ 0 ][ 2 ] * InT.m[ 1 ][ 3 ] * InT.m[ 2 ][ 1 ] -
		InT.m[ 0 ][ 3 ] * InT.m[ 1 ][ 1 ] * InT.m[ 2 ][ 2 ]
	) / detA ;

	Out->m[ 1 ][ 0 ] =
	(	
		InT.m[ 1 ][ 0 ] * InT.m[ 2 ][ 3 ] * InT.m[ 3 ][ 2 ] +
		InT.m[ 1 ][ 2 ] * InT.m[ 2 ][ 0 ] * InT.m[ 3 ][ 3 ] +
		InT.m[ 1 ][ 3 ] * InT.m[ 2 ][ 2 ] * InT.m[ 3 ][ 0 ] -
		InT.m[ 1 ][ 0 ] * InT.m[ 2 ][ 2 ] * InT.m[ 3 ][ 3 ] -
		InT.m[ 1 ][ 2 ] * InT.m[ 2 ][ 3 ] * InT.m[ 3 ][ 0 ] -
		InT.m[ 1 ][ 3 ] * InT.m[ 2 ][ 0 ] * InT.m[ 3 ][ 2 ]
	) / detA ;

	Out->m[ 1 ][ 1 ] = 
	(	
		InT.m[ 0 ][ 0 ] * InT.m[ 2 ][ 2 ] * InT.m[ 3 ][ 3 ] +
		InT.m[ 0 ][ 2 ] * InT.m[ 2 ][ 3 ] * InT.m[ 3 ][ 0 ] +
		InT.m[ 0 ][ 3 ] * InT.m[ 2 ][ 0 ] * InT.m[ 3 ][ 2 ] -
		InT.m[ 0 ][ 0 ] * InT.m[ 2 ][ 3 ] * InT.m[ 3 ][ 2 ] -
		InT.m[ 0 ][ 2 ] * InT.m[ 2 ][ 0 ] * InT.m[ 3 ][ 3 ] -
		InT.m[ 0 ][ 3 ] * InT.m[ 2 ][ 2 ] * InT.m[ 3 ][ 0 ]
	) / detA ;

	Out->m[ 1 ][ 2 ] = 
	(	
		InT.m[ 0 ][ 0 ] * InT.m[ 1 ][ 3 ] * InT.m[ 3 ][ 2 ] +
		InT.m[ 0 ][ 2 ] * InT.m[ 1 ][ 0 ] * InT.m[ 3 ][ 3 ] +
		InT.m[ 0 ][ 3 ] * InT.m[ 1 ][ 2 ] * InT.m[ 3 ][ 0 ] -
		InT.m[ 0 ][ 0 ] * InT.m[ 1 ][ 2 ] * InT.m[ 3 ][ 3 ] -
		InT.m[ 0 ][ 2 ] * InT.m[ 1 ][ 3 ] * InT.m[ 3 ][ 0 ] -
		InT.m[ 0 ][ 3 ] * InT.m[ 1 ][ 0 ] * InT.m[ 3 ][ 2 ]
	) / detA ;

	Out->m[ 1 ][ 3 ] = 
	(	
		InT.m[ 0 ][ 0 ] * InT.m[ 1 ][ 2 ] * InT.m[ 2 ][ 3 ] +
		InT.m[ 0 ][ 2 ] * InT.m[ 1 ][ 3 ] * InT.m[ 2 ][ 0 ] +
		InT.m[ 0 ][ 3 ] * InT.m[ 1 ][ 0 ] * InT.m[ 2 ][ 2 ] -
		InT.m[ 0 ][ 0 ] * InT.m[ 1 ][ 3 ] * InT.m[ 2 ][ 2 ] -
		InT.m[ 0 ][ 2 ] * InT.m[ 1 ][ 0 ] * InT.m[ 2 ][ 3 ] -
		InT.m[ 0 ][ 3 ] * InT.m[ 1 ][ 2 ] * InT.m[ 2 ][ 0 ]
	) / detA ;

	Out->m[ 2 ][ 0 ] = 
	(
		InT.m[ 1 ][ 0 ] * InT.m[ 2 ][ 1 ] * InT.m[ 3 ][ 3 ] +
		InT.m[ 1 ][ 1 ] * InT.m[ 2 ][ 3 ] * InT.m[ 3 ][ 0 ] +
		InT.m[ 1 ][ 3 ] * InT.m[ 2 ][ 0 ] * InT.m[ 3 ][ 1 ] -
		InT.m[ 1 ][ 0 ] * InT.m[ 2 ][ 3 ] * InT.m[ 3 ][ 1 ] -
		InT.m[ 1 ][ 1 ] * InT.m[ 2 ][ 0 ] * InT.m[ 3 ][ 3 ] -
		InT.m[ 1 ][ 3 ] * InT.m[ 2 ][ 1 ] * InT.m[ 3 ][ 0 ]
	) / detA ;

	Out->m[ 2 ][ 1 ] = 
	(
		InT.m[ 0 ][ 0 ] * InT.m[ 2 ][ 3 ] * InT.m[ 3 ][ 1 ] +
		InT.m[ 0 ][ 1 ] * InT.m[ 2 ][ 0 ] * InT.m[ 3 ][ 3 ] +
		InT.m[ 0 ][ 3 ] * InT.m[ 2 ][ 1 ] * InT.m[ 3 ][ 0 ] -
		InT.m[ 0 ][ 0 ] * InT.m[ 2 ][ 1 ] * InT.m[ 3 ][ 3 ] -
		InT.m[ 0 ][ 1 ] * InT.m[ 2 ][ 3 ] * InT.m[ 3 ][ 0 ] -
		InT.m[ 0 ][ 3 ] * InT.m[ 2 ][ 0 ] * InT.m[ 3 ][ 1 ]
	) / detA ;

	Out->m[ 2 ][ 2 ] = 
	(
		InT.m[ 0 ][ 0 ] * InT.m[ 1 ][ 1 ] * InT.m[ 3 ][ 3 ] +
		InT.m[ 0 ][ 1 ] * InT.m[ 1 ][ 3 ] * InT.m[ 3 ][ 0 ] +
		InT.m[ 0 ][ 3 ] * InT.m[ 1 ][ 0 ] * InT.m[ 3 ][ 1 ] -
		InT.m[ 0 ][ 0 ] * InT.m[ 1 ][ 3 ] * InT.m[ 3 ][ 1 ] -
		InT.m[ 0 ][ 1 ] * InT.m[ 1 ][ 0 ] * InT.m[ 3 ][ 3 ] -
		InT.m[ 0 ][ 3 ] * InT.m[ 1 ][ 1 ] * InT.m[ 3 ][ 0 ]
	) / detA ;

	Out->m[ 2 ][ 3 ] = 
	(
		InT.m[ 0 ][ 0 ] * InT.m[ 1 ][ 3 ] * InT.m[ 2 ][ 1 ] +
		InT.m[ 0 ][ 1 ] * InT.m[ 1 ][ 0 ] * InT.m[ 2 ][ 3 ] +
		InT.m[ 0 ][ 3 ] * InT.m[ 1 ][ 1 ] * InT.m[ 2 ][ 0 ] -
		InT.m[ 0 ][ 0 ] * InT.m[ 1 ][ 1 ] * InT.m[ 2 ][ 3 ] -
		InT.m[ 0 ][ 1 ] * InT.m[ 1 ][ 3 ] * InT.m[ 2 ][ 0 ] -
		InT.m[ 0 ][ 3 ] * InT.m[ 1 ][ 0 ] * InT.m[ 2 ][ 1 ]
	) / detA ;

	Out->m[ 3 ][ 0 ] = 
	(
		InT.m[ 1 ][ 0 ] * InT.m[ 2 ][ 2 ] * InT.m[ 3 ][ 1 ] +
		InT.m[ 1 ][ 1 ] * InT.m[ 2 ][ 0 ] * InT.m[ 3 ][ 2 ] +
		InT.m[ 1 ][ 2 ] * InT.m[ 2 ][ 1 ] * InT.m[ 3 ][ 0 ] -
		InT.m[ 1 ][ 0 ] * InT.m[ 2 ][ 1 ] * InT.m[ 3 ][ 2 ] -
		InT.m[ 1 ][ 1 ] * InT.m[ 2 ][ 2 ] * InT.m[ 3 ][ 0 ] -
		InT.m[ 1 ][ 2 ] * InT.m[ 2 ][ 0 ] * InT.m[ 3 ][ 1 ]
	) / detA ;

	Out->m[ 3 ][ 1 ] = 
	(
		InT.m[ 0 ][ 0 ] * InT.m[ 2 ][ 1 ] * InT.m[ 3 ][ 2 ] +
		InT.m[ 0 ][ 1 ] * InT.m[ 2 ][ 2 ] * InT.m[ 3 ][ 0 ] +
		InT.m[ 0 ][ 2 ] * InT.m[ 2 ][ 0 ] * InT.m[ 3 ][ 1 ] -
		InT.m[ 0 ][ 0 ] * InT.m[ 2 ][ 2 ] * InT.m[ 3 ][ 1 ] -
		InT.m[ 0 ][ 1 ] * InT.m[ 2 ][ 0 ] * InT.m[ 3 ][ 2 ] -
		InT.m[ 0 ][ 2 ] * InT.m[ 2 ][ 1 ] * InT.m[ 3 ][ 0 ]
	) / detA ;

	Out->m[ 3 ][ 2 ] = 
	(
		InT.m[ 0 ][ 0 ] * InT.m[ 1 ][ 2 ] * InT.m[ 3 ][ 1 ] +
		InT.m[ 0 ][ 1 ] * InT.m[ 1 ][ 0 ] * InT.m[ 3 ][ 2 ] +
		InT.m[ 0 ][ 2 ] * InT.m[ 1 ][ 1 ] * InT.m[ 3 ][ 0 ] -
		InT.m[ 0 ][ 0 ] * InT.m[ 1 ][ 1 ] * InT.m[ 3 ][ 2 ] -
		InT.m[ 0 ][ 1 ] * InT.m[ 1 ][ 2 ] * InT.m[ 3 ][ 0 ] -
		InT.m[ 0 ][ 2 ] * InT.m[ 1 ][ 0 ] * InT.m[ 3 ][ 1 ]
	) / detA ;

	Out->m[ 3 ][ 3 ] = 
	(
		InT.m[ 0 ][ 0 ] * InT.m[ 1 ][ 1 ] * InT.m[ 2 ][ 2 ] +
		InT.m[ 0 ][ 1 ] * InT.m[ 1 ][ 2 ] * InT.m[ 2 ][ 0 ] +
		InT.m[ 0 ][ 2 ] * InT.m[ 1 ][ 0 ] * InT.m[ 2 ][ 1 ] -
		InT.m[ 0 ][ 0 ] * InT.m[ 1 ][ 2 ] * InT.m[ 2 ][ 1 ] -
		InT.m[ 0 ][ 1 ] * InT.m[ 1 ][ 0 ] * InT.m[ 2 ][ 2 ] -
		InT.m[ 0 ][ 2 ] * InT.m[ 1 ][ 1 ] * InT.m[ 2 ][ 0 ]
	) / detA ;
/*
	MATRIX resmat ;

	CreateMultiplyMatrix( &resmat, &InT, Out ) ;
	if( resmat.m[ 0 ][ 0 ] <= 0.9999f   || resmat.m[ 0 ][ 0 ] >= 1.0001f ||
		resmat.m[ 1 ][ 1 ] <= 0.9999f   || resmat.m[ 1 ][ 1 ] >= 1.0001f ||
		resmat.m[ 2 ][ 2 ] <= 0.9999f   || resmat.m[ 2 ][ 2 ] >= 1.0001f ||
		resmat.m[ 3 ][ 3 ] <= 0.9999f   || resmat.m[ 3 ][ 3 ] >= 1.0001f ||

		resmat.m[ 0 ][ 1 ] <= -0.00001f || resmat.m[ 0 ][ 1 ] >= 0.0001f ||
		resmat.m[ 0 ][ 2 ] <= -0.00001f || resmat.m[ 0 ][ 2 ] >= 0.0001f ||
		resmat.m[ 0 ][ 3 ] <= -0.00001f || resmat.m[ 0 ][ 3 ] >= 0.0001f ||

		resmat.m[ 1 ][ 0 ] <= -0.00001f || resmat.m[ 1 ][ 0 ] >= 0.0001f ||
		resmat.m[ 1 ][ 2 ] <= -0.00001f || resmat.m[ 1 ][ 2 ] >= 0.0001f ||
		resmat.m[ 1 ][ 3 ] <= -0.00001f || resmat.m[ 1 ][ 3 ] >= 0.0001f ||

		resmat.m[ 2 ][ 0 ] <= -0.00001f || resmat.m[ 2 ][ 0 ] >= 0.0001f ||
		resmat.m[ 2 ][ 1 ] <= -0.00001f || resmat.m[ 2 ][ 1 ] >= 0.0001f ||
		resmat.m[ 2 ][ 3 ] <= -0.00001f || resmat.m[ 2 ][ 3 ] >= 0.0001f ||

		resmat.m[ 3 ][ 0 ] <= -0.00001f || resmat.m[ 3 ][ 0 ] >= 0.0001f ||
		resmat.m[ 3 ][ 1 ] <= -0.00001f || resmat.m[ 3 ][ 1 ] >= 0.0001f ||
		resmat.m[ 3 ][ 2 ] <= -0.00001f || resmat.m[ 3 ][ 2 ] >= 0.0001f )
	{
		detA = 0.0f ;
	}
*/
	return 0 ;

#else
	int i, j, k;
	double t, u, det;

	*Out = *In ;
	det = 1;
	for (k = 0; k < 4; k++) {
		t = Out->m[k][k];  det *= t;
		for (i = 0; i < 4; i++) Out->m[k][i] /= t;
		Out->m[k][k] = 1 / t;
		for (j = 0; j < 4; j++)
			if (j != k) {
				u = Out->m[j][k];
				for (i = 0; i < 4; i++)
					if (i != k) Out->m[j][i] -= Out->m[k][i] * u;
					else        Out->m[j][i] = -u / t;
			}
	}
	return det;
#endif

#else
	MATRIX mat, tempmat, resmat ;
	int i, j, loop ;
	float fDat, fDat2 ;
	float mat_8x4[ 4 ][ 8 ] ;
	int flag ;
	float *pF ;
	float *pD ;

	tempmat = *In ;

	for( i = 0 ; i < 4 ; i ++ )
	{
		pF = In->m[ i ] ;
		for( j = 0 ; j < 4 ; j ++, pF ++ ) mat_8x4[ i ][ j ] = ( double )*pF ;
		pD  = mat_8x4[ i ] + 4 ;
		for( j = 0 ; j < 4 ; j ++ )
		{
			*pD = i == j ? 1.0 : 0.0 ;
			pD ++ ;
		}
	}

	flag = 1;
	for( loop = 0 ; loop < 4 ; loop ++ )
	{
		fDat = mat_8x4[ loop ][ loop ] ;
		if( fDat != 1.0 )
		{
			if( fDat == 0.0 )
			{
				for( i = loop + 1 ; i < 4 ; i ++ )
				{
					fDat = mat_8x4[ i ][ loop ] ;
					if( fDat != 0.0 ) break ;
				}
				if( i >= 4 )
				{
					flag = 0 ;
					break ;
				}

				for( j = 0 ; j < 8 ; j++ )
				{
					fDat = mat_8x4[ i ][ j ] ;
					mat_8x4[ i ][ j ] = mat_8x4[ loop ][ j ] ;
					mat_8x4[ loop ][ j ] = fDat ;
				}
				fDat = mat_8x4[ loop ][ loop ] ;
			}

			for( i = 0 ; i < 8 ; i ++ ) mat_8x4[ loop ][ i ] /= fDat ;
		}

		for( i = 0 ; i < 4 ; i ++ )
		{
			if( i != loop )
			{
				fDat = mat_8x4[ i ][ loop ] ;
				if( fDat != 0.0f )
				{
					for( j = 0 ; j < 8 ; j ++ )
					{
						fDat2 = mat_8x4[ loop ][ j ] * fDat ;
						mat_8x4[ i ][ j ] -= fDat2 ;
					}
				}
			}
		}
	}

	if( flag )
	{
		for( i = 0 ; i < 4 ; i ++ )
		{
			pF = mat.m[ i ] ;
			pD = mat_8x4[ i ] + 4 ;
			for( j = 0 ; j < 4; j ++ )
			{
				*pF = ( float )( *pD ) ;
				pF ++ ;
				pD ++ ;
			}
		}
	}
	else
	{
		NS_CreateIdentityMatrix( &mat ) ;
	}

	*Out = mat ;

	NS_CreateMultiplyMatrix( &resmat, &tempmat, Out ) ;
	if( resmat.m[ 0 ][ 0 ] <= 0.9999f   || resmat.m[ 0 ][ 0 ] >= 1.0001f ||
		resmat.m[ 1 ][ 1 ] <= 0.9999f   || resmat.m[ 1 ][ 1 ] >= 1.0001f ||
		resmat.m[ 2 ][ 2 ] <= 0.9999f   || resmat.m[ 2 ][ 2 ] >= 1.0001f ||
		resmat.m[ 3 ][ 3 ] <= 0.9999f   || resmat.m[ 3 ][ 3 ] >= 1.0001f ||

		resmat.m[ 0 ][ 1 ] <= -0.00001f || resmat.m[ 0 ][ 1 ] >= 0.0001f ||
		resmat.m[ 0 ][ 2 ] <= -0.00001f || resmat.m[ 0 ][ 2 ] >= 0.0001f ||
		resmat.m[ 0 ][ 3 ] <= -0.00001f || resmat.m[ 0 ][ 3 ] >= 0.0001f ||

		resmat.m[ 1 ][ 0 ] <= -0.00001f || resmat.m[ 1 ][ 0 ] >= 0.0001f ||
		resmat.m[ 1 ][ 2 ] <= -0.00001f || resmat.m[ 1 ][ 2 ] >= 0.0001f ||
		resmat.m[ 1 ][ 3 ] <= -0.00001f || resmat.m[ 1 ][ 3 ] >= 0.0001f ||

		resmat.m[ 2 ][ 0 ] <= -0.00001f || resmat.m[ 2 ][ 0 ] >= 0.0001f ||
		resmat.m[ 2 ][ 1 ] <= -0.00001f || resmat.m[ 2 ][ 1 ] >= 0.0001f ||
		resmat.m[ 2 ][ 3 ] <= -0.00001f || resmat.m[ 2 ][ 3 ] >= 0.0001f ||

		resmat.m[ 3 ][ 0 ] <= -0.00001f || resmat.m[ 3 ][ 0 ] >= 0.0001f ||
		resmat.m[ 3 ][ 1 ] <= -0.00001f || resmat.m[ 3 ][ 1 ] >= 0.0001f ||
		resmat.m[ 3 ][ 2 ] <= -0.00001f || resmat.m[ 3 ][ 2 ] >= 0.0001f )
	{
		flag = 8 ;
	}

	if( flag ) return 1 ;

	return 0 ;
#endif
}

// �t�s����쐬����
extern int CreateInverseMatrixD( MATRIX_D *Out, const MATRIX_D *In )
{
	MATRIX_D InT ;
	double detA ;

	InT = *In ;
	detA = 
	InT.m[ 0 ][ 0 ] * InT.m[ 1 ][ 1 ] * InT.m[ 2 ][ 2 ] * InT.m[ 3 ][ 3 ] +
	InT.m[ 0 ][ 0 ] * InT.m[ 1 ][ 2 ] * InT.m[ 2 ][ 3 ] * InT.m[ 3 ][ 1 ] +
	InT.m[ 0 ][ 0 ] * InT.m[ 1 ][ 3 ] * InT.m[ 2 ][ 1 ] * InT.m[ 3 ][ 2 ] +
	InT.m[ 0 ][ 1 ] * InT.m[ 1 ][ 0 ] * InT.m[ 2 ][ 3 ] * InT.m[ 3 ][ 2 ] +
	InT.m[ 0 ][ 1 ] * InT.m[ 1 ][ 2 ] * InT.m[ 2 ][ 0 ] * InT.m[ 3 ][ 3 ] +
	InT.m[ 0 ][ 1 ] * InT.m[ 1 ][ 3 ] * InT.m[ 2 ][ 2 ] * InT.m[ 3 ][ 0 ] +
	InT.m[ 0 ][ 2 ] * InT.m[ 1 ][ 0 ] * InT.m[ 2 ][ 1 ] * InT.m[ 3 ][ 3 ] +
	InT.m[ 0 ][ 2 ] * InT.m[ 1 ][ 1 ] * InT.m[ 2 ][ 3 ] * InT.m[ 3 ][ 0 ] +
	InT.m[ 0 ][ 2 ] * InT.m[ 1 ][ 3 ] * InT.m[ 2 ][ 0 ] * InT.m[ 3 ][ 1 ] +
	InT.m[ 0 ][ 3 ] * InT.m[ 1 ][ 0 ] * InT.m[ 2 ][ 2 ] * InT.m[ 3 ][ 1 ] +
	InT.m[ 0 ][ 3 ] * InT.m[ 1 ][ 1 ] * InT.m[ 2 ][ 0 ] * InT.m[ 3 ][ 2 ] +
	InT.m[ 0 ][ 3 ] * InT.m[ 1 ][ 2 ] * InT.m[ 2 ][ 1 ] * InT.m[ 3 ][ 0 ] -

	InT.m[ 0 ][ 0 ] * InT.m[ 1 ][ 1 ] * InT.m[ 2 ][ 3 ] * InT.m[ 3 ][ 2 ] -
	InT.m[ 0 ][ 0 ] * InT.m[ 1 ][ 2 ] * InT.m[ 2 ][ 1 ] * InT.m[ 3 ][ 3 ] -
	InT.m[ 0 ][ 0 ] * InT.m[ 1 ][ 3 ] * InT.m[ 2 ][ 2 ] * InT.m[ 3 ][ 1 ] -
	InT.m[ 0 ][ 1 ] * InT.m[ 1 ][ 0 ] * InT.m[ 2 ][ 2 ] * InT.m[ 3 ][ 3 ] -
	InT.m[ 0 ][ 1 ] * InT.m[ 1 ][ 2 ] * InT.m[ 2 ][ 3 ] * InT.m[ 3 ][ 0 ] -
	InT.m[ 0 ][ 1 ] * InT.m[ 1 ][ 3 ] * InT.m[ 2 ][ 0 ] * InT.m[ 3 ][ 2 ] -
	InT.m[ 0 ][ 2 ] * InT.m[ 1 ][ 0 ] * InT.m[ 2 ][ 3 ] * InT.m[ 3 ][ 1 ] -
	InT.m[ 0 ][ 2 ] * InT.m[ 1 ][ 1 ] * InT.m[ 2 ][ 0 ] * InT.m[ 3 ][ 3 ] -
	InT.m[ 0 ][ 2 ] * InT.m[ 1 ][ 3 ] * InT.m[ 2 ][ 1 ] * InT.m[ 3 ][ 0 ] -
	InT.m[ 0 ][ 3 ] * InT.m[ 1 ][ 0 ] * InT.m[ 2 ][ 1 ] * InT.m[ 3 ][ 2 ] -
	InT.m[ 0 ][ 3 ] * InT.m[ 1 ][ 1 ] * InT.m[ 2 ][ 2 ] * InT.m[ 3 ][ 0 ] -
	InT.m[ 0 ][ 3 ] * InT.m[ 1 ][ 2 ] * InT.m[ 2 ][ 0 ] * InT.m[ 3 ][ 1 ] ;

	if( detA < 0.0000001 && detA > - 0.0000001 )
	{
		return -1 ;
	}

	Out->m[ 0 ][ 0 ] =
	(
		InT.m[ 1 ][ 1 ] * InT.m[ 2 ][ 2 ] * InT.m[ 3 ][ 3 ] +
		InT.m[ 1 ][ 2 ] * InT.m[ 2 ][ 3 ] * InT.m[ 3 ][ 1 ] +
		InT.m[ 1 ][ 3 ] * InT.m[ 2 ][ 1 ] * InT.m[ 3 ][ 2 ] -
		InT.m[ 1 ][ 1 ] * InT.m[ 2 ][ 3 ] * InT.m[ 3 ][ 2 ] -
		InT.m[ 1 ][ 2 ] * InT.m[ 2 ][ 1 ] * InT.m[ 3 ][ 3 ] -
		InT.m[ 1 ][ 3 ] * InT.m[ 2 ][ 2 ] * InT.m[ 3 ][ 1 ]
	) / detA ;

	Out->m[ 0 ][ 1 ] =
	(	
		InT.m[ 0 ][ 1 ] * InT.m[ 2 ][ 3 ] * InT.m[ 3 ][ 2 ] +
		InT.m[ 0 ][ 2 ] * InT.m[ 2 ][ 1 ] * InT.m[ 3 ][ 3 ] +
		InT.m[ 0 ][ 3 ] * InT.m[ 2 ][ 2 ] * InT.m[ 3 ][ 1 ] -
		InT.m[ 0 ][ 1 ] * InT.m[ 2 ][ 2 ] * InT.m[ 3 ][ 3 ] -
		InT.m[ 0 ][ 2 ] * InT.m[ 2 ][ 3 ] * InT.m[ 3 ][ 1 ] -
		InT.m[ 0 ][ 3 ] * InT.m[ 2 ][ 1 ] * InT.m[ 3 ][ 2 ]
	) / detA ;

	Out->m[ 0 ][ 2 ] =
	(	
		InT.m[ 0 ][ 1 ] * InT.m[ 1 ][ 2 ] * InT.m[ 3 ][ 3 ] +
		InT.m[ 0 ][ 2 ] * InT.m[ 1 ][ 3 ] * InT.m[ 3 ][ 1 ] +
		InT.m[ 0 ][ 3 ] * InT.m[ 1 ][ 1 ] * InT.m[ 3 ][ 2 ] -
		InT.m[ 0 ][ 1 ] * InT.m[ 1 ][ 3 ] * InT.m[ 3 ][ 2 ] -
		InT.m[ 0 ][ 2 ] * InT.m[ 1 ][ 1 ] * InT.m[ 3 ][ 3 ] -
		InT.m[ 0 ][ 3 ] * InT.m[ 1 ][ 2 ] * InT.m[ 3 ][ 1 ]
	) / detA ;

	Out->m[ 0 ][ 3 ] =
	(	
		InT.m[ 0 ][ 1 ] * InT.m[ 1 ][ 3 ] * InT.m[ 2 ][ 2 ] +
		InT.m[ 0 ][ 2 ] * InT.m[ 1 ][ 1 ] * InT.m[ 2 ][ 3 ] +
		InT.m[ 0 ][ 3 ] * InT.m[ 1 ][ 2 ] * InT.m[ 2 ][ 1 ] -
		InT.m[ 0 ][ 1 ] * InT.m[ 1 ][ 2 ] * InT.m[ 2 ][ 3 ] -
		InT.m[ 0 ][ 2 ] * InT.m[ 1 ][ 3 ] * InT.m[ 2 ][ 1 ] -
		InT.m[ 0 ][ 3 ] * InT.m[ 1 ][ 1 ] * InT.m[ 2 ][ 2 ]
	) / detA ;

	Out->m[ 1 ][ 0 ] =
	(	
		InT.m[ 1 ][ 0 ] * InT.m[ 2 ][ 3 ] * InT.m[ 3 ][ 2 ] +
		InT.m[ 1 ][ 2 ] * InT.m[ 2 ][ 0 ] * InT.m[ 3 ][ 3 ] +
		InT.m[ 1 ][ 3 ] * InT.m[ 2 ][ 2 ] * InT.m[ 3 ][ 0 ] -
		InT.m[ 1 ][ 0 ] * InT.m[ 2 ][ 2 ] * InT.m[ 3 ][ 3 ] -
		InT.m[ 1 ][ 2 ] * InT.m[ 2 ][ 3 ] * InT.m[ 3 ][ 0 ] -
		InT.m[ 1 ][ 3 ] * InT.m[ 2 ][ 0 ] * InT.m[ 3 ][ 2 ]
	) / detA ;

	Out->m[ 1 ][ 1 ] = 
	(	
		InT.m[ 0 ][ 0 ] * InT.m[ 2 ][ 2 ] * InT.m[ 3 ][ 3 ] +
		InT.m[ 0 ][ 2 ] * InT.m[ 2 ][ 3 ] * InT.m[ 3 ][ 0 ] +
		InT.m[ 0 ][ 3 ] * InT.m[ 2 ][ 0 ] * InT.m[ 3 ][ 2 ] -
		InT.m[ 0 ][ 0 ] * InT.m[ 2 ][ 3 ] * InT.m[ 3 ][ 2 ] -
		InT.m[ 0 ][ 2 ] * InT.m[ 2 ][ 0 ] * InT.m[ 3 ][ 3 ] -
		InT.m[ 0 ][ 3 ] * InT.m[ 2 ][ 2 ] * InT.m[ 3 ][ 0 ]
	) / detA ;

	Out->m[ 1 ][ 2 ] = 
	(	
		InT.m[ 0 ][ 0 ] * InT.m[ 1 ][ 3 ] * InT.m[ 3 ][ 2 ] +
		InT.m[ 0 ][ 2 ] * InT.m[ 1 ][ 0 ] * InT.m[ 3 ][ 3 ] +
		InT.m[ 0 ][ 3 ] * InT.m[ 1 ][ 2 ] * InT.m[ 3 ][ 0 ] -
		InT.m[ 0 ][ 0 ] * InT.m[ 1 ][ 2 ] * InT.m[ 3 ][ 3 ] -
		InT.m[ 0 ][ 2 ] * InT.m[ 1 ][ 3 ] * InT.m[ 3 ][ 0 ] -
		InT.m[ 0 ][ 3 ] * InT.m[ 1 ][ 0 ] * InT.m[ 3 ][ 2 ]
	) / detA ;

	Out->m[ 1 ][ 3 ] = 
	(	
		InT.m[ 0 ][ 0 ] * InT.m[ 1 ][ 2 ] * InT.m[ 2 ][ 3 ] +
		InT.m[ 0 ][ 2 ] * InT.m[ 1 ][ 3 ] * InT.m[ 2 ][ 0 ] +
		InT.m[ 0 ][ 3 ] * InT.m[ 1 ][ 0 ] * InT.m[ 2 ][ 2 ] -
		InT.m[ 0 ][ 0 ] * InT.m[ 1 ][ 3 ] * InT.m[ 2 ][ 2 ] -
		InT.m[ 0 ][ 2 ] * InT.m[ 1 ][ 0 ] * InT.m[ 2 ][ 3 ] -
		InT.m[ 0 ][ 3 ] * InT.m[ 1 ][ 2 ] * InT.m[ 2 ][ 0 ]
	) / detA ;

	Out->m[ 2 ][ 0 ] = 
	(
		InT.m[ 1 ][ 0 ] * InT.m[ 2 ][ 1 ] * InT.m[ 3 ][ 3 ] +
		InT.m[ 1 ][ 1 ] * InT.m[ 2 ][ 3 ] * InT.m[ 3 ][ 0 ] +
		InT.m[ 1 ][ 3 ] * InT.m[ 2 ][ 0 ] * InT.m[ 3 ][ 1 ] -
		InT.m[ 1 ][ 0 ] * InT.m[ 2 ][ 3 ] * InT.m[ 3 ][ 1 ] -
		InT.m[ 1 ][ 1 ] * InT.m[ 2 ][ 0 ] * InT.m[ 3 ][ 3 ] -
		InT.m[ 1 ][ 3 ] * InT.m[ 2 ][ 1 ] * InT.m[ 3 ][ 0 ]
	) / detA ;

	Out->m[ 2 ][ 1 ] = 
	(
		InT.m[ 0 ][ 0 ] * InT.m[ 2 ][ 3 ] * InT.m[ 3 ][ 1 ] +
		InT.m[ 0 ][ 1 ] * InT.m[ 2 ][ 0 ] * InT.m[ 3 ][ 3 ] +
		InT.m[ 0 ][ 3 ] * InT.m[ 2 ][ 1 ] * InT.m[ 3 ][ 0 ] -
		InT.m[ 0 ][ 0 ] * InT.m[ 2 ][ 1 ] * InT.m[ 3 ][ 3 ] -
		InT.m[ 0 ][ 1 ] * InT.m[ 2 ][ 3 ] * InT.m[ 3 ][ 0 ] -
		InT.m[ 0 ][ 3 ] * InT.m[ 2 ][ 0 ] * InT.m[ 3 ][ 1 ]
	) / detA ;

	Out->m[ 2 ][ 2 ] = 
	(
		InT.m[ 0 ][ 0 ] * InT.m[ 1 ][ 1 ] * InT.m[ 3 ][ 3 ] +
		InT.m[ 0 ][ 1 ] * InT.m[ 1 ][ 3 ] * InT.m[ 3 ][ 0 ] +
		InT.m[ 0 ][ 3 ] * InT.m[ 1 ][ 0 ] * InT.m[ 3 ][ 1 ] -
		InT.m[ 0 ][ 0 ] * InT.m[ 1 ][ 3 ] * InT.m[ 3 ][ 1 ] -
		InT.m[ 0 ][ 1 ] * InT.m[ 1 ][ 0 ] * InT.m[ 3 ][ 3 ] -
		InT.m[ 0 ][ 3 ] * InT.m[ 1 ][ 1 ] * InT.m[ 3 ][ 0 ]
	) / detA ;

	Out->m[ 2 ][ 3 ] = 
	(
		InT.m[ 0 ][ 0 ] * InT.m[ 1 ][ 3 ] * InT.m[ 2 ][ 1 ] +
		InT.m[ 0 ][ 1 ] * InT.m[ 1 ][ 0 ] * InT.m[ 2 ][ 3 ] +
		InT.m[ 0 ][ 3 ] * InT.m[ 1 ][ 1 ] * InT.m[ 2 ][ 0 ] -
		InT.m[ 0 ][ 0 ] * InT.m[ 1 ][ 1 ] * InT.m[ 2 ][ 3 ] -
		InT.m[ 0 ][ 1 ] * InT.m[ 1 ][ 3 ] * InT.m[ 2 ][ 0 ] -
		InT.m[ 0 ][ 3 ] * InT.m[ 1 ][ 0 ] * InT.m[ 2 ][ 1 ]
	) / detA ;

	Out->m[ 3 ][ 0 ] = 
	(
		InT.m[ 1 ][ 0 ] * InT.m[ 2 ][ 2 ] * InT.m[ 3 ][ 1 ] +
		InT.m[ 1 ][ 1 ] * InT.m[ 2 ][ 0 ] * InT.m[ 3 ][ 2 ] +
		InT.m[ 1 ][ 2 ] * InT.m[ 2 ][ 1 ] * InT.m[ 3 ][ 0 ] -
		InT.m[ 1 ][ 0 ] * InT.m[ 2 ][ 1 ] * InT.m[ 3 ][ 2 ] -
		InT.m[ 1 ][ 1 ] * InT.m[ 2 ][ 2 ] * InT.m[ 3 ][ 0 ] -
		InT.m[ 1 ][ 2 ] * InT.m[ 2 ][ 0 ] * InT.m[ 3 ][ 1 ]
	) / detA ;

	Out->m[ 3 ][ 1 ] = 
	(
		InT.m[ 0 ][ 0 ] * InT.m[ 2 ][ 1 ] * InT.m[ 3 ][ 2 ] +
		InT.m[ 0 ][ 1 ] * InT.m[ 2 ][ 2 ] * InT.m[ 3 ][ 0 ] +
		InT.m[ 0 ][ 2 ] * InT.m[ 2 ][ 0 ] * InT.m[ 3 ][ 1 ] -
		InT.m[ 0 ][ 0 ] * InT.m[ 2 ][ 2 ] * InT.m[ 3 ][ 1 ] -
		InT.m[ 0 ][ 1 ] * InT.m[ 2 ][ 0 ] * InT.m[ 3 ][ 2 ] -
		InT.m[ 0 ][ 2 ] * InT.m[ 2 ][ 1 ] * InT.m[ 3 ][ 0 ]
	) / detA ;

	Out->m[ 3 ][ 2 ] = 
	(
		InT.m[ 0 ][ 0 ] * InT.m[ 1 ][ 2 ] * InT.m[ 3 ][ 1 ] +
		InT.m[ 0 ][ 1 ] * InT.m[ 1 ][ 0 ] * InT.m[ 3 ][ 2 ] +
		InT.m[ 0 ][ 2 ] * InT.m[ 1 ][ 1 ] * InT.m[ 3 ][ 0 ] -
		InT.m[ 0 ][ 0 ] * InT.m[ 1 ][ 1 ] * InT.m[ 3 ][ 2 ] -
		InT.m[ 0 ][ 1 ] * InT.m[ 1 ][ 2 ] * InT.m[ 3 ][ 0 ] -
		InT.m[ 0 ][ 2 ] * InT.m[ 1 ][ 0 ] * InT.m[ 3 ][ 1 ]
	) / detA ;

	Out->m[ 3 ][ 3 ] = 
	(
		InT.m[ 0 ][ 0 ] * InT.m[ 1 ][ 1 ] * InT.m[ 2 ][ 2 ] +
		InT.m[ 0 ][ 1 ] * InT.m[ 1 ][ 2 ] * InT.m[ 2 ][ 0 ] +
		InT.m[ 0 ][ 2 ] * InT.m[ 1 ][ 0 ] * InT.m[ 2 ][ 1 ] -
		InT.m[ 0 ][ 0 ] * InT.m[ 1 ][ 2 ] * InT.m[ 2 ][ 1 ] -
		InT.m[ 0 ][ 1 ] * InT.m[ 1 ][ 0 ] * InT.m[ 2 ][ 2 ] -
		InT.m[ 0 ][ 2 ] * InT.m[ 1 ][ 1 ] * InT.m[ 2 ][ 0 ]
	) / detA ;

	return 0 ;
}

// �r���[�|�[�g�s����쐬����
extern int CreateViewportMatrix( MATRIX *Out, float CenterX, float CenterY, float Width, float Height )
{
	_MEMSET( Out, 0, sizeof( MATRIX ) ) ;
	Out->m[0][0] = Width / 2.0F ;
	Out->m[1][1] = -Height / 2.0F ;
	Out->m[2][2] = 1.0F ;
	Out->m[3][3] = 1.0F ;
	Out->m[3][0] = CenterX ;
	Out->m[3][1] = CenterY ;
	Out->m[3][2] = 0.0F ;

	return 0 ;
}

// �r���[�|�[�g�s����쐬����
extern int CreateViewportMatrixD( MATRIX_D *Out, double CenterX, double CenterY, double Width, double Height )
{
	_MEMSET( Out, 0, sizeof( MATRIX_D ) ) ;
	Out->m[0][0] = Width / 2.0 ;
	Out->m[1][1] = -Height / 2.0 ;
	Out->m[2][2] = 1.0 ;
	Out->m[3][3] = 1.0 ;
	Out->m[3][0] = CenterX ;
	Out->m[3][1] = CenterY ;
	Out->m[3][2] = 0.0 ;

	return 0 ;
}

// �w����]���x����]���y����]�����������s����쐬����
extern int CreateRotationXYZMatrix( MATRIX *Out, float XRot, float YRot, float ZRot )
{
	float SinX, CosX, SinY, CosY, SinZ, CosZ ;
	float SinXSinY, SinYCosX ;

	_SINCOS_PLATFORM( XRot, &SinX, &CosX ) ;
	_SINCOS_PLATFORM( YRot, &SinY, &CosY ) ;
	_SINCOS_PLATFORM( ZRot, &SinZ, &CosZ ) ;

	SinXSinY = SinX * SinY ;
	SinYCosX = SinY * CosX ;

	Out->m[ 0 ][ 0 ] = CosY * CosZ ;
	Out->m[ 0 ][ 1 ] = CosY * SinZ ;
	Out->m[ 0 ][ 2 ] = -SinY ;
	Out->m[ 0 ][ 3 ] = 0.0f ;
	Out->m[ 1 ][ 0 ] = SinXSinY * CosZ - CosX * SinZ ;
	Out->m[ 1 ][ 1 ] = SinXSinY * SinZ + CosX * CosZ ;
	Out->m[ 1 ][ 2 ] = SinX * CosY ;
	Out->m[ 1 ][ 3 ] = 0.0f ;
	Out->m[ 2 ][ 0 ] = SinYCosX * CosZ + SinX * SinZ ;
	Out->m[ 2 ][ 1 ] = SinYCosX * SinZ - SinX * CosZ ;
	Out->m[ 2 ][ 2 ] = CosX * CosY ;
	Out->m[ 2 ][ 3 ] = 0.0f ;
	Out->m[ 3 ][ 0 ] = 0.0f ;
	Out->m[ 3 ][ 1 ] = 0.0f ;
	Out->m[ 3 ][ 2 ] = 0.0f ;
	Out->m[ 3 ][ 3 ] = 1.0f ;

	return 0 ;
}

// �w����]���x����]���y����]�����������s����쐬����
extern int CreateRotationXYZMatrixD( MATRIX_D *Out, double XRot, double YRot, double ZRot )
{
	double SinX, CosX, SinY, CosY, SinZ, CosZ ;
	double SinXSinY, SinYCosX ;

	_SINCOSD( XRot, &SinX, &CosX ) ;
	_SINCOSD( YRot, &SinY, &CosY ) ;
	_SINCOSD( ZRot, &SinZ, &CosZ ) ;

	SinXSinY = SinX * SinY ;
	SinYCosX = SinY * CosX ;

	Out->m[ 0 ][ 0 ] = CosY * CosZ ;
	Out->m[ 0 ][ 1 ] = CosY * SinZ ;
	Out->m[ 0 ][ 2 ] = -SinY ;
	Out->m[ 0 ][ 3 ] = 0.0 ;
	Out->m[ 1 ][ 0 ] = SinXSinY * CosZ - CosX * SinZ ;
	Out->m[ 1 ][ 1 ] = SinXSinY * SinZ + CosX * CosZ ;
	Out->m[ 1 ][ 2 ] = SinX * CosY ;
	Out->m[ 1 ][ 3 ] = 0.0 ;
	Out->m[ 2 ][ 0 ] = SinYCosX * CosZ + SinX * SinZ ;
	Out->m[ 2 ][ 1 ] = SinYCosX * SinZ - SinX * CosZ ;
	Out->m[ 2 ][ 2 ] = CosX * CosY ;
	Out->m[ 2 ][ 3 ] = 0.0 ;
	Out->m[ 3 ][ 0 ] = 0.0 ;
	Out->m[ 3 ][ 1 ] = 0.0 ;
	Out->m[ 3 ][ 2 ] = 0.0 ;
	Out->m[ 3 ][ 3 ] = 1.0 ;

	return 0 ;
}

// �w����]���y����]���x����]�����������s����쐬����
extern int CreateRotationXZYMatrix( MATRIX *Out, float XRot, float YRot, float ZRot )
{
	float SinX, CosX, SinY, CosY, SinZ, CosZ ;
	float CosXSinZ, SinXSinZ ;

	_SINCOS_PLATFORM( XRot, &SinX, &CosX ) ;
	_SINCOS_PLATFORM( YRot, &SinY, &CosY ) ;
	_SINCOS_PLATFORM( ZRot, &SinZ, &CosZ ) ;

	CosXSinZ = CosX * SinZ ;
	SinXSinZ = SinX * SinZ ;

	Out->m[ 0 ][ 0 ] = CosY * CosZ ;
	Out->m[ 0 ][ 1 ] = SinZ ;
	Out->m[ 0 ][ 2 ] = CosZ * -SinY ;
	Out->m[ 0 ][ 3 ] = 0.0f ;
	Out->m[ 1 ][ 0 ] = CosXSinZ * -CosY + SinX * SinY ;
	Out->m[ 1 ][ 1 ] = CosX * CosZ ;
	Out->m[ 1 ][ 2 ] = CosXSinZ *  SinY + SinX * CosY ;
	Out->m[ 1 ][ 3 ] = 0.0f ;
	Out->m[ 2 ][ 0 ] = SinXSinZ *  CosY + CosX * SinY ;
	Out->m[ 2 ][ 1 ] = -SinX * CosZ ;
	Out->m[ 2 ][ 2 ] = SinXSinZ * -SinY + CosX * CosY ;
	Out->m[ 2 ][ 3 ] = 0.0f ;
	Out->m[ 3 ][ 0 ] = 0.0f ;
	Out->m[ 3 ][ 1 ] = 0.0f ;
	Out->m[ 3 ][ 2 ] = 0.0f ;
	Out->m[ 3 ][ 3 ] = 1.0f ;

	return 0 ;
}

// �w����]���y����]���x����]�����������s����쐬����
extern int CreateRotationXZYMatrixD( MATRIX_D *Out, double XRot, double YRot, double ZRot )
{
	double SinX, CosX, SinY, CosY, SinZ, CosZ ;
	double CosXSinZ, SinXSinZ ;

	_SINCOSD( XRot, &SinX, &CosX ) ;
	_SINCOSD( YRot, &SinY, &CosY ) ;
	_SINCOSD( ZRot, &SinZ, &CosZ ) ;

	CosXSinZ = CosX * SinZ ;
	SinXSinZ = SinX * SinZ ;

	Out->m[ 0 ][ 0 ] = CosY * CosZ ;
	Out->m[ 0 ][ 1 ] = SinZ ;
	Out->m[ 0 ][ 2 ] = CosZ * -SinY ;
	Out->m[ 0 ][ 3 ] = 0.0 ;
	Out->m[ 1 ][ 0 ] = CosXSinZ * -CosY + SinX * SinY ;
	Out->m[ 1 ][ 1 ] = CosX * CosZ ;
	Out->m[ 1 ][ 2 ] = CosXSinZ *  SinY + SinX * CosY ;
	Out->m[ 1 ][ 3 ] = 0.0 ;
	Out->m[ 2 ][ 0 ] = SinXSinZ *  CosY + CosX * SinY ;
	Out->m[ 2 ][ 1 ] = -SinX * CosZ ;
	Out->m[ 2 ][ 2 ] = SinXSinZ * -SinY + CosX * CosY ;
	Out->m[ 2 ][ 3 ] = 0.0 ;
	Out->m[ 3 ][ 0 ] = 0.0 ;
	Out->m[ 3 ][ 1 ] = 0.0 ;
	Out->m[ 3 ][ 2 ] = 0.0 ;
	Out->m[ 3 ][ 3 ] = 1.0 ;

	return 0 ;
}

// �x����]���w����]���y����]�����������s����쐬����
extern int CreateRotationYXZMatrix( MATRIX *Out, float XRot, float YRot, float ZRot )
{
	float SinX, CosX, SinY, CosY, SinZ, CosZ ;
	float SinXSinY, CosYSinX ;

	_SINCOS_PLATFORM( XRot, &SinX, &CosX ) ;
	_SINCOS_PLATFORM( YRot, &SinY, &CosY ) ;
	_SINCOS_PLATFORM( ZRot, &SinZ, &CosZ ) ;

	SinXSinY = SinX * SinY ;
	CosYSinX = CosY * SinX ;

	Out->m[ 0 ][ 0 ] = CosY * CosZ - SinXSinY * SinZ ;
	Out->m[ 0 ][ 1 ] = CosY * SinZ + SinXSinY * CosZ ;
	Out->m[ 0 ][ 2 ] = -SinY * CosX ;
	Out->m[ 0 ][ 3 ] = 0.0f ;
	Out->m[ 1 ][ 0 ] = CosX * -SinZ ;
	Out->m[ 1 ][ 1 ] = CosX * CosZ ;
	Out->m[ 1 ][ 2 ] = SinX ;
	Out->m[ 1 ][ 3 ] = 0.0f ;
	Out->m[ 2 ][ 0 ] = SinY * CosZ + CosYSinX * SinZ ;
	Out->m[ 2 ][ 1 ] = SinY * SinZ - CosYSinX * CosZ ;
	Out->m[ 2 ][ 2 ] = CosY * CosX ;
	Out->m[ 2 ][ 3 ] = 0.0f ;
	Out->m[ 3 ][ 0 ] = 0.0f ;
	Out->m[ 3 ][ 1 ] = 0.0f ;
	Out->m[ 3 ][ 2 ] = 0.0f ;
	Out->m[ 3 ][ 3 ] = 1.0f ;

	return 0 ;
}

// �x����]���w����]���y����]�����������s����쐬����
extern int CreateRotationYXZMatrixD( MATRIX_D *Out, double XRot, double YRot, double ZRot )
{
	double SinX, CosX, SinY, CosY, SinZ, CosZ ;
	double SinXSinY, CosYSinX ;

	_SINCOSD( XRot, &SinX, &CosX ) ;
	_SINCOSD( YRot, &SinY, &CosY ) ;
	_SINCOSD( ZRot, &SinZ, &CosZ ) ;

	SinXSinY = SinX * SinY ;
	CosYSinX = CosY * SinX ;

	Out->m[ 0 ][ 0 ] = CosY * CosZ - SinXSinY * SinZ ;
	Out->m[ 0 ][ 1 ] = CosY * SinZ + SinXSinY * CosZ ;
	Out->m[ 0 ][ 2 ] = -SinY * CosX ;
	Out->m[ 0 ][ 3 ] = 0.0 ;
	Out->m[ 1 ][ 0 ] = CosX * -SinZ ;
	Out->m[ 1 ][ 1 ] = CosX * CosZ ;
	Out->m[ 1 ][ 2 ] = SinX ;
	Out->m[ 1 ][ 3 ] = 0.0 ;
	Out->m[ 2 ][ 0 ] = SinY * CosZ + CosYSinX * SinZ ;
	Out->m[ 2 ][ 1 ] = SinY * SinZ - CosYSinX * CosZ ;
	Out->m[ 2 ][ 2 ] = CosY * CosX ;
	Out->m[ 2 ][ 3 ] = 0.0 ;
	Out->m[ 3 ][ 0 ] = 0.0 ;
	Out->m[ 3 ][ 1 ] = 0.0 ;
	Out->m[ 3 ][ 2 ] = 0.0 ;
	Out->m[ 3 ][ 3 ] = 1.0 ;

	return 0 ;
}

// �x����]���y����]���w����]�����������s����쐬����
extern int CreateRotationYZXMatrix( MATRIX *Out, float XRot, float YRot, float ZRot )
{
	float SinX, CosX, SinY, CosY, SinZ, CosZ ;
	float CosYSinZ, SinYSinZ ;

	_SINCOS_PLATFORM( XRot, &SinX, &CosX ) ;
	_SINCOS_PLATFORM( YRot, &SinY, &CosY ) ;
	_SINCOS_PLATFORM( ZRot, &SinZ, &CosZ ) ;

	CosYSinZ = CosY * SinZ ;
	SinYSinZ = SinY * SinZ ;

	Out->m[ 0 ][ 0 ] = CosY * CosZ ;
	Out->m[ 0 ][ 1 ] = CosYSinZ * CosX + SinY * SinX ;
	Out->m[ 0 ][ 2 ] = CosYSinZ * SinX - SinY * CosX ;
	Out->m[ 0 ][ 3 ] = 0.0f ;
	Out->m[ 1 ][ 0 ] = -SinZ ;
	Out->m[ 1 ][ 1 ] = CosZ * CosX ;
	Out->m[ 1 ][ 2 ] = CosZ * SinX ;
	Out->m[ 1 ][ 3 ] = 0.0f ;
	Out->m[ 2 ][ 0 ] = SinY * CosZ ;
	Out->m[ 2 ][ 1 ] = SinYSinZ * CosX - CosY * SinX ;
	Out->m[ 2 ][ 2 ] = SinYSinZ * SinX + CosY * CosX ;
	Out->m[ 2 ][ 3 ] = 0.0f ;
	Out->m[ 3 ][ 0 ] = 0.0f ;
	Out->m[ 3 ][ 1 ] = 0.0f ;
	Out->m[ 3 ][ 2 ] = 0.0f ;
	Out->m[ 3 ][ 3 ] = 1.0f ;

	return 0 ;
}

// �x����]���y����]���w����]�����������s����쐬����
extern int CreateRotationYZXMatrixD( MATRIX_D *Out, double XRot, double YRot, double ZRot )
{
	double SinX, CosX, SinY, CosY, SinZ, CosZ ;
	double CosYSinZ, SinYSinZ ;

	_SINCOSD( XRot, &SinX, &CosX ) ;
	_SINCOSD( YRot, &SinY, &CosY ) ;
	_SINCOSD( ZRot, &SinZ, &CosZ ) ;

	CosYSinZ = CosY * SinZ ;
	SinYSinZ = SinY * SinZ ;

	Out->m[ 0 ][ 0 ] = CosY * CosZ ;
	Out->m[ 0 ][ 1 ] = CosYSinZ * CosX + SinY * SinX ;
	Out->m[ 0 ][ 2 ] = CosYSinZ * SinX - SinY * CosX ;
	Out->m[ 0 ][ 3 ] = 0.0 ;
	Out->m[ 1 ][ 0 ] = -SinZ ;
	Out->m[ 1 ][ 1 ] = CosZ * CosX ;
	Out->m[ 1 ][ 2 ] = CosZ * SinX ;
	Out->m[ 1 ][ 3 ] = 0.0 ;
	Out->m[ 2 ][ 0 ] = SinY * CosZ ;
	Out->m[ 2 ][ 1 ] = SinYSinZ * CosX - CosY * SinX ;
	Out->m[ 2 ][ 2 ] = SinYSinZ * SinX + CosY * CosX ;
	Out->m[ 2 ][ 3 ] = 0.0 ;
	Out->m[ 3 ][ 0 ] = 0.0 ;
	Out->m[ 3 ][ 1 ] = 0.0 ;
	Out->m[ 3 ][ 2 ] = 0.0 ;
	Out->m[ 3 ][ 3 ] = 1.0 ;

	return 0 ;
}

// �y����]���w����]���x����]�����������s����쐬����
extern int CreateRotationZXYMatrix( MATRIX *Out, float XRot, float YRot, float ZRot )
{
	float SinX, CosX, SinY, CosY, SinZ, CosZ ;
	float SinXSinZ, SinXCosZ ;

	_SINCOS_PLATFORM( XRot, &SinX, &CosX ) ;
	_SINCOS_PLATFORM( YRot, &SinY, &CosY ) ;
	_SINCOS_PLATFORM( ZRot, &SinZ, &CosZ ) ;

	SinXSinZ = SinX * SinZ ;
	SinXCosZ = SinX * CosZ ;

	Out->m[ 0 ][ 0 ] = CosY *  CosZ + SinXSinZ * SinY ;
	Out->m[ 0 ][ 1 ] = CosX * SinZ ;
	Out->m[ 0 ][ 2 ] = CosZ * -SinY + SinXSinZ * CosY ;
	Out->m[ 0 ][ 3 ] = 0.0f ;
	Out->m[ 1 ][ 0 ] = CosY * -SinZ + SinXCosZ * SinY ;
	Out->m[ 1 ][ 1 ] = CosX * CosZ ;
	Out->m[ 1 ][ 2 ] = SinZ *  SinY + SinXCosZ * CosY ;
	Out->m[ 1 ][ 3 ] = 0.0f ;
	Out->m[ 2 ][ 0 ] = CosX * SinY ;
	Out->m[ 2 ][ 1 ] = -SinX ;
	Out->m[ 2 ][ 2 ] = CosX * CosY ;
	Out->m[ 2 ][ 3 ] = 0.0f ;
	Out->m[ 3 ][ 0 ] = 0.0f ;
	Out->m[ 3 ][ 1 ] = 0.0f ;
	Out->m[ 3 ][ 2 ] = 0.0f ;
	Out->m[ 3 ][ 3 ] = 1.0f ;

	return 0 ;
}

// �y����]���w����]���x����]�����������s����쐬����
extern int CreateRotationZXYMatrixD( MATRIX_D *Out, double XRot, double YRot, double ZRot )
{
	double SinX, CosX, SinY, CosY, SinZ, CosZ ;
	double SinXSinZ, SinXCosZ ;

	_SINCOSD( XRot, &SinX, &CosX ) ;
	_SINCOSD( YRot, &SinY, &CosY ) ;
	_SINCOSD( ZRot, &SinZ, &CosZ ) ;

	SinXSinZ = SinX * SinZ ;
	SinXCosZ = SinX * CosZ ;

	Out->m[ 0 ][ 0 ] = CosY *  CosZ + SinXSinZ * SinY ;
	Out->m[ 0 ][ 1 ] = CosX * SinZ ;
	Out->m[ 0 ][ 2 ] = CosZ * -SinY + SinXSinZ * CosY ;
	Out->m[ 0 ][ 3 ] = 0.0 ;
	Out->m[ 1 ][ 0 ] = CosY * -SinZ + SinXCosZ * SinY ;
	Out->m[ 1 ][ 1 ] = CosX * CosZ ;
	Out->m[ 1 ][ 2 ] = SinZ *  SinY + SinXCosZ * CosY ;
	Out->m[ 1 ][ 3 ] = 0.0 ;
	Out->m[ 2 ][ 0 ] = CosX * SinY ;
	Out->m[ 2 ][ 1 ] = -SinX ;
	Out->m[ 2 ][ 2 ] = CosX * CosY ;
	Out->m[ 2 ][ 3 ] = 0.0 ;
	Out->m[ 3 ][ 0 ] = 0.0 ;
	Out->m[ 3 ][ 1 ] = 0.0 ;
	Out->m[ 3 ][ 2 ] = 0.0 ;
	Out->m[ 3 ][ 3 ] = 1.0 ;

	return 0 ;
}

// �y����]���x����]���w����]�����������s����쐬����
extern int CreateRotationZYXMatrix( MATRIX *Out, float XRot, float YRot, float ZRot )
{
	float SinX, CosX, SinY, CosY, SinZ, CosZ ;
	float CosZSinY, SinZSinY ;

	_SINCOS_PLATFORM( XRot, &SinX, &CosX ) ;
	_SINCOS_PLATFORM( YRot, &SinY, &CosY ) ;
	_SINCOS_PLATFORM( ZRot, &SinZ, &CosZ ) ;

	CosZSinY = CosZ * SinY ;
	SinZSinY = SinZ * SinY ;

	Out->m[ 0 ][ 0 ] = CosY * CosZ ;
	Out->m[ 0 ][ 1 ] = SinZ * CosX + CosZSinY * SinX ;
	Out->m[ 0 ][ 2 ] = SinZ * SinX - CosZSinY * CosX ;
	Out->m[ 0 ][ 3 ] = 0.0f ;
	Out->m[ 1 ][ 0 ] = -SinZ * CosY ;
	Out->m[ 1 ][ 1 ] = CosZ * CosX - SinZSinY * SinX ;
	Out->m[ 1 ][ 2 ] = CosZ * SinX + SinZSinY * CosX ;
	Out->m[ 1 ][ 3 ] = 0.0f ;
	Out->m[ 2 ][ 0 ] = SinY ;
	Out->m[ 2 ][ 1 ] = CosY * -SinX ;
	Out->m[ 2 ][ 2 ] = CosY *  CosX ;
	Out->m[ 2 ][ 3 ] = 0.0f ;
	Out->m[ 3 ][ 0 ] = 0.0f ;
	Out->m[ 3 ][ 1 ] = 0.0f ;
	Out->m[ 3 ][ 2 ] = 0.0f ;
	Out->m[ 3 ][ 3 ] = 1.0f ;

	return 0 ;
}

// �y����]���x����]���w����]�����������s����쐬����
extern int CreateRotationZYXMatrixD( MATRIX_D *Out, double XRot, double YRot, double ZRot )
{
	double SinX, CosX, SinY, CosY, SinZ, CosZ ;
	double CosZSinY, SinZSinY ;

	_SINCOSD( XRot, &SinX, &CosX ) ;
	_SINCOSD( YRot, &SinY, &CosY ) ;
	_SINCOSD( ZRot, &SinZ, &CosZ ) ;

	CosZSinY = CosZ * SinY ;
	SinZSinY = SinZ * SinY ;

	Out->m[ 0 ][ 0 ] = CosY * CosZ ;
	Out->m[ 0 ][ 1 ] = SinZ * CosX + CosZSinY * SinX ;
	Out->m[ 0 ][ 2 ] = SinZ * SinX - CosZSinY * CosX ;
	Out->m[ 0 ][ 3 ] = 0.0 ;
	Out->m[ 1 ][ 0 ] = -SinZ * CosY ;
	Out->m[ 1 ][ 1 ] = CosZ * CosX - SinZSinY * SinX ;
	Out->m[ 1 ][ 2 ] = CosZ * SinX + SinZSinY * CosX ;
	Out->m[ 1 ][ 3 ] = 0.0 ;
	Out->m[ 2 ][ 0 ] = SinY ;
	Out->m[ 2 ][ 1 ] = CosY * -SinX ;
	Out->m[ 2 ][ 2 ] = CosY *  CosX ;
	Out->m[ 2 ][ 3 ] = 0.0 ;
	Out->m[ 3 ][ 0 ] = 0.0 ;
	Out->m[ 3 ][ 1 ] = 0.0 ;
	Out->m[ 3 ][ 2 ] = 0.0 ;
	Out->m[ 3 ][ 3 ] = 1.0 ;

	return 0 ;
}

// �s�񂩂�w�x�y����]�̒l���擾����( �߂�l�@-1:�W���o�����b�N����  0:���� )
extern int GetMatrixXYZRotation( const MATRIX *In, float *OutXRot, float *OutYRot, float *OutZRot )
{
	float XRot, YRot, ZRot, Sin, Cos ;

	if( ( In->m[ 0 ][ 2 ] >  0.99999f && In->m[ 0 ][ 2 ] <  1.000001f ) ||
		( In->m[ 0 ][ 2 ] < -0.99999f && In->m[ 0 ][ 2 ] > -1.000001f ) )
	{
		XRot = 0.0f ;
		YRot = -In->m[ 0 ][ 2 ] > 0.5f ? ( float )DX_PI / 2.0f : -( float )DX_PI / 2.0f ;
		ZRot = _ATAN2( -In->m[ 1 ][ 0 ], In->m[ 1 ][ 1 ] ) ;

		if( OutXRot ) *OutXRot = XRot ;
		if( OutYRot ) *OutYRot = YRot ;
		if( OutZRot ) *OutZRot = ZRot ;
		return -1 ;
	}
	else
	{
		XRot = _ATAN2( In->m[ 1 ][ 2 ], In->m[ 2 ][ 2 ] ) ;
		ZRot = _ATAN2( In->m[ 0 ][ 1 ], In->m[ 0 ][ 0 ] ) ;
		_SINCOS_PLATFORM( ZRot, &Sin, &Cos ) ;
		if( Cos < 0.000001f && Cos > -0.000001f )
		{
			YRot = _ATAN2( -In->m[ 0 ][ 2 ], In->m[ 0 ][ 1 ] / Sin ) ;
		}
		else
		{
			YRot = _ATAN2( -In->m[ 0 ][ 2 ], In->m[ 0 ][ 0 ] / Cos ) ;
		}

		if( OutXRot ) *OutXRot = XRot ;
		if( OutYRot ) *OutYRot = YRot ;
		if( OutZRot ) *OutZRot = ZRot ;
		return 0 ;
	}
}

// �s�񂩂�w�x�y����]�̒l���擾����( �߂�l�@-1:�W���o�����b�N����  0:���� )
extern int GetMatrixXYZRotationD( const MATRIX_D *In, double *OutXRot, double *OutYRot, double *OutZRot )
{
	double XRot, YRot, ZRot, Sin, Cos ;

	if( ( In->m[ 0 ][ 2 ] >  0.99999 && In->m[ 0 ][ 2 ] <  1.000001 ) ||
		( In->m[ 0 ][ 2 ] < -0.99999 && In->m[ 0 ][ 2 ] > -1.000001 ) )
	{
		XRot = 0.0 ;
		YRot = -In->m[ 0 ][ 2 ] > 0.5 ? DX_PI / 2.0 : -DX_PI / 2.0 ;
		ZRot = _ATAN2D( -In->m[ 1 ][ 0 ], In->m[ 1 ][ 1 ] ) ;

		if( OutXRot ) *OutXRot = XRot ;
		if( OutYRot ) *OutYRot = YRot ;
		if( OutZRot ) *OutZRot = ZRot ;
		return -1 ;
	}
	else
	{
		XRot = _ATAN2D( In->m[ 1 ][ 2 ], In->m[ 2 ][ 2 ] ) ;
		ZRot = _ATAN2D( In->m[ 0 ][ 1 ], In->m[ 0 ][ 0 ] ) ;
		_SINCOSD( ZRot, &Sin, &Cos ) ;
		if( Cos < 0.000001 && Cos > -0.000001 )
		{
			YRot = _ATAN2D( -In->m[ 0 ][ 2 ], In->m[ 0 ][ 1 ] / Sin ) ;
		}
		else
		{
			YRot = _ATAN2D( -In->m[ 0 ][ 2 ], In->m[ 0 ][ 0 ] / Cos ) ;
		}

		if( OutXRot ) *OutXRot = XRot ;
		if( OutYRot ) *OutYRot = YRot ;
		if( OutZRot ) *OutZRot = ZRot ;
		return 0 ;
	}
}

// �s�񂩂�w�y�x����]�̒l���擾����( �߂�l�@-1:�W���o�����b�N����  0:���� )
extern int GetMatrixXZYRotation( const MATRIX *In, float *OutXRot, float *OutYRot, float *OutZRot )
{
	float XRot, YRot, ZRot, Sin, Cos ;

	if( ( In->m[ 0 ][ 1 ] >  0.99999f && In->m[ 0 ][ 1 ] <  1.000001f ) ||
		( In->m[ 0 ][ 1 ] < -0.99999f && In->m[ 0 ][ 1 ] > -1.000001f ) )
	{
		XRot = 0.0f ;
		ZRot = In->m[ 0 ][ 1 ] > 0.5f ? ( float )DX_PI / 2.0f : -( float )DX_PI / 2.0f ;
		YRot = _ATAN2( In->m[ 2 ][ 0 ], In->m[ 2 ][ 2 ] ) ;

		if( OutXRot ) *OutXRot = XRot ;
		if( OutYRot ) *OutYRot = YRot ;
		if( OutZRot ) *OutZRot = ZRot ;
		return -1 ;
	}
	else
	{
		YRot = _ATAN2( -In->m[ 0 ][ 2 ], In->m[ 0 ][ 0 ] ) ;
		XRot = _ATAN2( -In->m[ 2 ][ 1 ], In->m[ 1 ][ 1 ] ) ;
		_SINCOS_PLATFORM( XRot, &Sin, &Cos ) ;
		if( Cos < 0.000001f && Cos > -0.000001f )
		{
			ZRot = _ATAN2( In->m[ 0 ][ 1 ], -In->m[ 2 ][ 1 ] / Sin ) ;
		}
		else
		{
			ZRot = _ATAN2( In->m[ 0 ][ 1 ],  In->m[ 1 ][ 1 ] / Cos ) ;
		}

		if( OutXRot ) *OutXRot = XRot ;
		if( OutYRot ) *OutYRot = YRot ;
		if( OutZRot ) *OutZRot = ZRot ;
		return 0 ;
	}
}

// �s�񂩂�w�y�x����]�̒l���擾����( �߂�l�@-1:�W���o�����b�N����  0:���� )
extern int GetMatrixXZYRotationD( const MATRIX_D *In, double *OutXRot, double *OutYRot, double *OutZRot )
{
	double XRot, YRot, ZRot, Sin, Cos ;

	if( ( In->m[ 0 ][ 1 ] >  0.99999 && In->m[ 0 ][ 1 ] <  1.000001 ) ||
		( In->m[ 0 ][ 1 ] < -0.99999 && In->m[ 0 ][ 1 ] > -1.000001 ) )
	{
		XRot = 0.0 ;
		ZRot = In->m[ 0 ][ 1 ] > 0.5 ? DX_PI / 2.0 : -DX_PI / 2.0 ;
		YRot = _ATAN2D( In->m[ 2 ][ 0 ], In->m[ 2 ][ 2 ] ) ;

		if( OutXRot ) *OutXRot = XRot ;
		if( OutYRot ) *OutYRot = YRot ;
		if( OutZRot ) *OutZRot = ZRot ;
		return -1 ;
	}
	else
	{
		YRot = _ATAN2D( -In->m[ 0 ][ 2 ], In->m[ 0 ][ 0 ] ) ;
		XRot = _ATAN2D( -In->m[ 2 ][ 1 ], In->m[ 1 ][ 1 ] ) ;
		_SINCOSD( XRot, &Sin, &Cos ) ;
		if( Cos < 0.000001 && Cos > -0.000001 )
		{
			ZRot = _ATAN2D( In->m[ 0 ][ 1 ], -In->m[ 2 ][ 1 ] / Sin ) ;
		}
		else
		{
			ZRot = _ATAN2D( In->m[ 0 ][ 1 ],  In->m[ 1 ][ 1 ] / Cos ) ;
		}

		if( OutXRot ) *OutXRot = XRot ;
		if( OutYRot ) *OutYRot = YRot ;
		if( OutZRot ) *OutZRot = ZRot ;
		return 0 ;
	}
}

// �s�񂩂�x�w�y����]�̒l���擾����( �߂�l�@-1:�W���o�����b�N����  0:���� )
extern int GetMatrixYXZRotation( const MATRIX *In, float *OutXRot, float *OutYRot, float *OutZRot )
{
	float XRot, YRot, ZRot, Sin, Cos ;

	if( ( In->m[ 1 ][ 2 ] >  0.99999f && In->m[ 1 ][ 2 ] <  1.000001f ) ||
		( In->m[ 1 ][ 2 ] < -0.99999f && In->m[ 1 ][ 2 ] > -1.000001f ) )
	{
		YRot = 0.0f ;
		XRot = In->m[ 1 ][ 2 ] > 0.5f ? ( float )DX_PI / 2.0f : -( float )DX_PI / 2.0f ;
		ZRot = _ATAN2( In->m[ 0 ][ 1 ], In->m[ 0 ][ 0 ] ) ;

		if( OutXRot ) *OutXRot = XRot ;
		if( OutYRot ) *OutYRot = YRot ;
		if( OutZRot ) *OutZRot = ZRot ;
		return -1 ;
	}
	else
	{
		ZRot = _ATAN2( -In->m[ 1 ][ 0 ], In->m[ 1 ][ 1 ] ) ;
		YRot = _ATAN2( -In->m[ 0 ][ 2 ], In->m[ 2 ][ 2 ] ) ;
		_SINCOS_PLATFORM( YRot, &Sin, &Cos ) ;
		if( Cos < 0.000001f && Cos > -0.000001f )
		{
			XRot = _ATAN2( In->m[ 1 ][ 2 ], -In->m[ 0 ][ 2 ] / Sin ) ;
		}
		else
		{
			XRot = _ATAN2( In->m[ 1 ][ 2 ],  In->m[ 2 ][ 2 ] / Cos ) ;
		}

		if( OutXRot ) *OutXRot = XRot ;
		if( OutYRot ) *OutYRot = YRot ;
		if( OutZRot ) *OutZRot = ZRot ;
		return 0 ;
	}
}

// �s�񂩂�x�w�y����]�̒l���擾����( �߂�l�@-1:�W���o�����b�N����  0:���� )
extern int GetMatrixYXZRotationD( const MATRIX_D *In, double *OutXRot, double *OutYRot, double *OutZRot )
{
	double XRot, YRot, ZRot, Sin, Cos ;

	if( ( In->m[ 1 ][ 2 ] >  0.99999 && In->m[ 1 ][ 2 ] <  1.000001 ) ||
		( In->m[ 1 ][ 2 ] < -0.99999 && In->m[ 1 ][ 2 ] > -1.000001 ) )
	{
		YRot = 0.0 ;
		XRot = In->m[ 1 ][ 2 ] > 0.5 ? DX_PI / 2.0 : -DX_PI / 2.0 ;
		ZRot = _ATAN2D( In->m[ 0 ][ 1 ], In->m[ 0 ][ 0 ] ) ;

		if( OutXRot ) *OutXRot = XRot ;
		if( OutYRot ) *OutYRot = YRot ;
		if( OutZRot ) *OutZRot = ZRot ;
		return -1 ;
	}
	else
	{
		ZRot = _ATAN2D( -In->m[ 1 ][ 0 ], In->m[ 1 ][ 1 ] ) ;
		YRot = _ATAN2D( -In->m[ 0 ][ 2 ], In->m[ 2 ][ 2 ] ) ;
		_SINCOSD( YRot, &Sin, &Cos ) ;
		if( Cos < 0.000001 && Cos > -0.000001 )
		{
			XRot = _ATAN2D( In->m[ 1 ][ 2 ], -In->m[ 0 ][ 2 ] / Sin ) ;
		}
		else
		{
			XRot = _ATAN2D( In->m[ 1 ][ 2 ],  In->m[ 2 ][ 2 ] / Cos ) ;
		}

		if( OutXRot ) *OutXRot = XRot ;
		if( OutYRot ) *OutYRot = YRot ;
		if( OutZRot ) *OutZRot = ZRot ;
		return 0 ;
	}
}

// �s�񂩂�x�y�w����]�̒l���擾����( �߂�l�@-1:�W���o�����b�N����  0:���� )
extern int GetMatrixYZXRotation( const MATRIX *In, float *OutXRot, float *OutYRot, float *OutZRot )
{
	float XRot, YRot, ZRot, Sin, Cos ;

	if( ( In->m[ 1 ][ 0 ] >  0.99999f && In->m[ 1 ][ 0 ] <  1.000001f ) ||
		( In->m[ 1 ][ 0 ] < -0.99999f && In->m[ 1 ][ 0 ] > -1.000001f ) )
	{
		YRot = 0.0f ;
		ZRot = -In->m[ 1 ][ 0 ] > 0.5f ? ( float )DX_PI / 2.0f : -( float )DX_PI / 2.0f ;
		XRot = _ATAN2( -In->m[ 2 ][ 1 ], In->m[ 2 ][ 2 ] ) ;

		if( OutXRot ) *OutXRot = XRot ;
		if( OutYRot ) *OutYRot = YRot ;
		if( OutZRot ) *OutZRot = ZRot ;
		return -1 ;
	}
	else
	{
		XRot = _ATAN2( In->m[ 1 ][ 2 ], In->m[ 1 ][ 1 ] ) ;
		YRot = _ATAN2( In->m[ 2 ][ 0 ], In->m[ 0 ][ 0 ] ) ;
		_SINCOS_PLATFORM( YRot, &Sin, &Cos ) ;
		if( Cos < 0.000001f && Cos > -0.000001f )
		{
			ZRot = _ATAN2( -In->m[ 1 ][ 0 ], In->m[ 2 ][ 0 ] / Sin ) ;
		}
		else
		{
			ZRot = _ATAN2( -In->m[ 1 ][ 0 ], In->m[ 0 ][ 0 ] / Cos ) ;
		}

		if( OutXRot ) *OutXRot = XRot ;
		if( OutYRot ) *OutYRot = YRot ;
		if( OutZRot ) *OutZRot = ZRot ;
		return 0 ;
	}
}

// �s�񂩂�x�y�w����]�̒l���擾����( �߂�l�@-1:�W���o�����b�N����  0:���� )
extern int GetMatrixYZXRotationD( const MATRIX_D *In, double *OutXRot, double *OutYRot, double *OutZRot )
{
	double XRot, YRot, ZRot, Sin, Cos ;

	if( ( In->m[ 1 ][ 0 ] >  0.99999 && In->m[ 1 ][ 0 ] <  1.000001 ) ||
		( In->m[ 1 ][ 0 ] < -0.99999 && In->m[ 1 ][ 0 ] > -1.000001 ) )
	{
		YRot = 0.0 ;
		ZRot = -In->m[ 1 ][ 0 ] > 0.5 ? DX_PI / 2.0 : -DX_PI / 2.0 ;
		XRot = _ATAN2D( -In->m[ 2 ][ 1 ], In->m[ 2 ][ 2 ] ) ;

		if( OutXRot ) *OutXRot = XRot ;
		if( OutYRot ) *OutYRot = YRot ;
		if( OutZRot ) *OutZRot = ZRot ;
		return -1 ;
	}
	else
	{
		XRot = _ATAN2D( In->m[ 1 ][ 2 ], In->m[ 1 ][ 1 ] ) ;
		YRot = _ATAN2D( In->m[ 2 ][ 0 ], In->m[ 0 ][ 0 ] ) ;
		_SINCOSD( YRot, &Sin, &Cos ) ;
		if( Cos < 0.000001 && Cos > -0.000001 )
		{
			ZRot = _ATAN2D( -In->m[ 1 ][ 0 ], In->m[ 2 ][ 0 ] / Sin ) ;
		}
		else
		{
			ZRot = _ATAN2D( -In->m[ 1 ][ 0 ], In->m[ 0 ][ 0 ] / Cos ) ;
		}

		if( OutXRot ) *OutXRot = XRot ;
		if( OutYRot ) *OutYRot = YRot ;
		if( OutZRot ) *OutZRot = ZRot ;
		return 0 ;
	}
}

// �s�񂩂�y�w�x����]�̒l���擾����( �߂�l�@-1:�W���o�����b�N����  0:���� )
extern int GetMatrixZXYRotation( const MATRIX *In, float *OutXRot, float *OutYRot, float *OutZRot )
{
	float XRot, YRot, ZRot, Sin, Cos ;

	if( ( In->m[ 2 ][ 1 ] >  0.99999f && In->m[ 2 ][ 1 ] <  1.000001f ) ||
		( In->m[ 2 ][ 1 ] < -0.99999f && In->m[ 2 ][ 1 ] > -1.000001f ) )
	{
		ZRot = 0.0f ;
		XRot = -In->m[ 2 ][ 1 ] > 0.5f ? ( float )DX_PI / 2.0f : -( float )DX_PI / 2.0f ;
		YRot = _ATAN2( -In->m[ 0 ][ 2 ], In->m[ 0 ][ 0 ] ) ;

		if( OutXRot ) *OutXRot = XRot ;
		if( OutYRot ) *OutYRot = YRot ;
		if( OutZRot ) *OutZRot = ZRot ;
		return -1 ;
	}
	else
	{
		YRot = _ATAN2( In->m[ 2 ][ 0 ], In->m[ 2 ][ 2 ] ) ;
		ZRot = _ATAN2( In->m[ 0 ][ 1 ], In->m[ 1 ][ 1 ] ) ;
		_SINCOS_PLATFORM( ZRot, &Sin, &Cos ) ;
		if( Cos < 0.000001f && Cos > -0.000001f )
		{
			XRot = _ATAN2( -In->m[ 2 ][ 1 ], -In->m[ 0 ][ 1 ] / Sin ) ;
		}
		else
		{
			XRot = _ATAN2( -In->m[ 2 ][ 1 ],  In->m[ 1 ][ 1 ] / Cos ) ;
		}

		if( OutXRot ) *OutXRot = XRot ;
		if( OutYRot ) *OutYRot = YRot ;
		if( OutZRot ) *OutZRot = ZRot ;
		return 0 ;
	}
}

// �s�񂩂�y�w�x����]�̒l���擾����( �߂�l�@-1:�W���o�����b�N����  0:���� )
extern int GetMatrixZXYRotationD( const MATRIX_D *In, double *OutXRot, double *OutYRot, double *OutZRot )
{
	double XRot, YRot, ZRot, Sin, Cos ;

	if( ( In->m[ 2 ][ 1 ] >  0.99999 && In->m[ 2 ][ 1 ] <  1.000001 ) ||
		( In->m[ 2 ][ 1 ] < -0.99999 && In->m[ 2 ][ 1 ] > -1.000001 ) )
	{
		ZRot = 0.0 ;
		XRot = -In->m[ 2 ][ 1 ] > 0.5 ? DX_PI / 2.0 : -DX_PI / 2.0 ;
		YRot = _ATAN2D( -In->m[ 0 ][ 2 ], In->m[ 0 ][ 0 ] ) ;

		if( OutXRot ) *OutXRot = XRot ;
		if( OutYRot ) *OutYRot = YRot ;
		if( OutZRot ) *OutZRot = ZRot ;
		return -1 ;
	}
	else
	{
		YRot = _ATAN2D( In->m[ 2 ][ 0 ], In->m[ 2 ][ 2 ] ) ;
		ZRot = _ATAN2D( In->m[ 0 ][ 1 ], In->m[ 1 ][ 1 ] ) ;
		_SINCOSD( ZRot, &Sin, &Cos ) ;
		if( Cos < 0.000001 && Cos > -0.000001 )
		{
			XRot = _ATAN2D( -In->m[ 2 ][ 1 ], -In->m[ 0 ][ 1 ] / Sin ) ;
		}
		else
		{
			XRot = _ATAN2D( -In->m[ 2 ][ 1 ],  In->m[ 1 ][ 1 ] / Cos ) ;
		}

		if( OutXRot ) *OutXRot = XRot ;
		if( OutYRot ) *OutYRot = YRot ;
		if( OutZRot ) *OutZRot = ZRot ;
		return 0 ;
	}
}

// �s�񂩂�y�x�w����]�̒l���擾����( �߂�l�@-1:�W���o�����b�N����  0:���� )
extern int GetMatrixZYXRotation( const MATRIX *In, float *OutXRot, float *OutYRot, float *OutZRot )
{
	float XRot, YRot, ZRot, Sin, Cos ;

	if( ( In->m[ 2 ][ 0 ] >  0.99999f && In->m[ 2 ][ 0 ] <  1.000001f ) ||
		( In->m[ 2 ][ 0 ] < -0.99999f && In->m[ 2 ][ 0 ] > -1.000001f ) )
	{
		ZRot = 0.0f ;
		YRot = In->m[ 2 ][ 0 ] > 0.5f ? ( float )DX_PI / 2.0f : -( float )DX_PI / 2.0f ;
		XRot = _ATAN2( In->m[ 1 ][ 2 ], In->m[ 1 ][ 1 ] ) ;

		if( OutXRot ) *OutXRot = XRot ;
		if( OutYRot ) *OutYRot = YRot ;
		if( OutZRot ) *OutZRot = ZRot ;
		return -1 ;
	}
	else
	{
		XRot = _ATAN2( -In->m[ 2 ][ 1 ], In->m[ 2 ][ 2 ] ) ;
		ZRot = _ATAN2( -In->m[ 1 ][ 0 ], In->m[ 0 ][ 0 ] ) ;
		_SINCOS_PLATFORM( ZRot, &Sin, &Cos ) ;
		if( Cos < 0.000001f && Cos > -0.000001f )
		{
			YRot = _ATAN2( In->m[ 2 ][ 0 ], -In->m[ 1 ][ 0 ] / Sin ) ;
		}
		else
		{
			YRot = _ATAN2( In->m[ 2 ][ 0 ],  In->m[ 0 ][ 0 ] / Cos ) ;
		}

		if( OutXRot ) *OutXRot = XRot ;
		if( OutYRot ) *OutYRot = YRot ;
		if( OutZRot ) *OutZRot = ZRot ;
		return 0 ;
	}
}

// �s�񂩂�y�x�w����]�̒l���擾����( �߂�l�@-1:�W���o�����b�N����  0:���� )
extern int GetMatrixZYXRotationD( const MATRIX_D *In, double *OutXRot, double *OutYRot, double *OutZRot )
{
	double XRot, YRot, ZRot, Sin, Cos ;

	if( ( In->m[ 2 ][ 0 ] >  0.99999 && In->m[ 2 ][ 0 ] <  1.000001 ) ||
		( In->m[ 2 ][ 0 ] < -0.99999 && In->m[ 2 ][ 0 ] > -1.000001 ) )
	{
		ZRot = 0.0 ;
		YRot = In->m[ 2 ][ 0 ] > 0.5 ? DX_PI / 2.0 : -DX_PI / 2.0 ;
		XRot = _ATAN2D( In->m[ 1 ][ 2 ], In->m[ 1 ][ 1 ] ) ;

		if( OutXRot ) *OutXRot = XRot ;
		if( OutYRot ) *OutYRot = YRot ;
		if( OutZRot ) *OutZRot = ZRot ;
		return -1 ;
	}
	else
	{
		XRot = _ATAN2D( -In->m[ 2 ][ 1 ], In->m[ 2 ][ 2 ] ) ;
		ZRot = _ATAN2D( -In->m[ 1 ][ 0 ], In->m[ 0 ][ 0 ] ) ;
		_SINCOSD( ZRot, &Sin, &Cos ) ;
		if( Cos < 0.000001 && Cos > -0.000001 )
		{
			YRot = _ATAN2D( In->m[ 2 ][ 0 ], -In->m[ 1 ][ 0 ] / Sin ) ;
		}
		else
		{
			YRot = _ATAN2D( In->m[ 2 ][ 0 ],  In->m[ 0 ][ 0 ] / Cos ) ;
		}

		if( OutXRot ) *OutXRot = XRot ;
		if( OutYRot ) *OutYRot = YRot ;
		if( OutZRot ) *OutZRot = ZRot ;
		return 0 ;
	}
}

// float�^�̃x�N�g����double�^�̃x�N�g���ɕϊ�����
extern int VectorConvertFtoD( VECTOR_D *Out, const VECTOR   *In )
{
	Out->x = In->x ;
	Out->y = In->y ;
	Out->z = In->z ;

	return 0 ;
}

// double�^�̃x�N�g����float�^�̃x�N�g���ɕϊ�����
extern int VectorConvertDtoF( VECTOR   *Out, const VECTOR_D *In )
{
	Out->x = ( float )In->x ;
	Out->y = ( float )In->y ;
	Out->z = ( float )In->z ;

	return 0 ;
}

// �x�N�g���𐳋K������
extern int VectorNormalize( VECTOR *Out, const VECTOR *In )
{
	float r ;

	r = ( float )_SQRT( In->x * In->x + In->y * In->y + In->z * In->z ) ;
	Out->x = In->x / r ;
	Out->y = In->y / r ;
	Out->z = In->z / r ;

	// �I��
	return 0 ;
}

// �x�N�g���𐳋K������
extern int VectorNormalizeD( VECTOR_D *Out, const VECTOR_D *In )
{
	double r ;

	r = _SQRTD( In->x * In->x + In->y * In->y + In->z * In->z ) ;
	Out->x = In->x / r ;
	Out->y = In->y / r ;
	Out->z = In->z / r ;

	// �I��
	return 0 ;
}

// �x�N�g�����X�J���[�{����
extern int VectorScale( VECTOR *Out, const VECTOR *In, float Scale )
{
	Out->x = In->x * Scale ;
	Out->y = In->y * Scale ;
	Out->z = In->z * Scale ;

	// �I��
	return 0 ;
}

// �x�N�g�����X�J���[�{����
extern int VectorScaleD( VECTOR_D *Out, const VECTOR_D *In, double Scale )
{
	Out->x = In->x * Scale ;
	Out->y = In->y * Scale ;
	Out->z = In->z * Scale ;

	// �I��
	return 0 ;
}

// �x�N�g���̊|���Z������
extern int VectorMultiply( VECTOR *Out, const VECTOR *In1, const VECTOR *In2 )
{
	Out->x = In1->x * In2->x ;
	Out->y = In1->y * In2->y ;
	Out->z = In1->z * In2->z ;

	// �I��
	return 0 ;
}

// �x�N�g���̊|���Z������
extern int VectorMultiplyD( VECTOR_D *Out, const VECTOR_D *In1, const VECTOR_D *In2 )
{
	Out->x = In1->x * In2->x ;
	Out->y = In1->y * In2->y ;
	Out->z = In1->z * In2->z ;

	// �I��
	return 0 ;
}

// Out = In1 - In2 �̃x�N�g���v�Z������
extern int VectorSub( VECTOR *Out, const VECTOR *In1, const VECTOR *In2 ) 
{
	Out->x = In1->x - In2->x ;
	Out->y = In1->y - In2->y ;
	Out->z = In1->z - In2->z ;

	// �I��
	return 0 ;
}

// Out = In1 - In2 �̃x�N�g���v�Z������
extern int VectorSubD( VECTOR_D *Out, const VECTOR_D *In1, const VECTOR_D *In2 ) 
{
	Out->x = In1->x - In2->x ;
	Out->y = In1->y - In2->y ;
	Out->z = In1->z - In2->z ;

	// �I��
	return 0 ;
}

// Out = In1 + In2 �̃x�N�g���v�Z������ 
extern int VectorAdd( VECTOR *Out, const VECTOR *In1, const VECTOR *In2 )
{
	Out->x = In1->x + In2->x ;
	Out->y = In1->y + In2->y ;
	Out->z = In1->z + In2->z ;

	// �I��
	return 0 ;
}

// Out = In1 + In2 �̃x�N�g���v�Z������ 
extern int VectorAddD( VECTOR_D *Out, const VECTOR_D *In1, const VECTOR_D *In2 )
{
	Out->x = In1->x + In2->x ;
	Out->y = In1->y + In2->y ;
	Out->z = In1->z + In2->z ;

	// �I��
	return 0 ;
}

// In1��In2�̊O�ς��v�Z����
extern int VectorOuterProduct( VECTOR *Out, const VECTOR *In1, const VECTOR *In2 )
{
	VECTOR in1 = *In1 ;
	VECTOR in2 = *In2 ;
	
	Out->x = in1.y * in2.z - in1.z * in2.y ;
	Out->y = in1.z * in2.x - in1.x * in2.z ;
	Out->z = in1.x * in2.y - in1.y * in2.x ;
	
	// �I��
	return 0 ;
}

// In1��In2�̊O�ς��v�Z����
extern int VectorOuterProductD( VECTOR_D *Out, const VECTOR_D *In1, const VECTOR_D *In2 )
{
	VECTOR_D in1 = *In1 ;
	VECTOR_D in2 = *In2 ;
	
	Out->x = in1.y * in2.z - in1.z * in2.y ;
	Out->y = in1.z * in2.x - in1.x * in2.z ;
	Out->z = in1.x * in2.y - in1.y * in2.x ;
	
	// �I��
	return 0 ;
}

// In1��In2�̓��ς��v�Z����
extern float VectorInnerProduct( const VECTOR *In1, const VECTOR *In2 )
{
	return In1->x * In2->x + In1->y * In2->y + In1->z * In2->z ;
}

// In1��In2�̓��ς��v�Z����
extern double VectorInnerProductD( const VECTOR_D *In1, const VECTOR_D *In2 )
{
	return In1->x * In2->x + In1->y * In2->y + In1->z * In2->z ;
}

// �x�N�g���̂w�������ɂ�����]���s��
extern int VectorRotationX( VECTOR *Out, const VECTOR *In, double Angle )
{
	float f ;
	float Sin, Cos ;

	_SINCOS_PLATFORM( (float)Angle, &Sin, &Cos ) ;
	Out->x = In->x ;
	f      = In->z * Cos - In->y * Sin ;
	Out->y = In->z * Sin + In->y * Cos ;
	Out->z = f ;

	// �I��
	return 0 ;
}

// �x�N�g���̂w�������ɂ�����]���s��
extern int VectorRotationXD( VECTOR_D *Out, const VECTOR_D *In, double Angle )
{
	double f ;
	double Sin, Cos ;

	_SINCOSD( Angle, &Sin, &Cos ) ;
	Out->x = In->x ;
	f      = In->z * Cos - In->y * Sin ;
	Out->y = In->z * Sin + In->y * Cos ;
	Out->z = f ;

	// �I��
	return 0 ;
}

// �x�N�g���̂x�������ɂ�����]���s��
extern int VectorRotationY( VECTOR *Out, const VECTOR *In, double Angle )
{
	float f ;
	float Sin, Cos ;

	_SINCOS_PLATFORM( (float)Angle, &Sin, &Cos ) ;
	Out->y = In->y ;
	f      = In->x * Cos - In->z * Sin ;
	Out->z = In->x * Sin + In->z * Cos ;
	Out->x = f ;

	// �I��
	return 0 ;
}

// �x�N�g���̂x�������ɂ�����]���s��
extern int VectorRotationYD( VECTOR_D *Out, const VECTOR_D *In, double Angle )
{
	double f ;
	double Sin, Cos ;

	_SINCOSD( Angle, &Sin, &Cos ) ;
	Out->y = In->y ;
	f      = In->x * Cos - In->z * Sin ;
	Out->z = In->x * Sin + In->z * Cos ;
	Out->x = f ;

	// �I��
	return 0 ;
}

// �x�N�g���̂y�������ɂ�����]���s��
extern int VectorRotationZ( VECTOR *Out, const VECTOR *In, double Angle )
{
	float f ;
	float Sin, Cos ;

	_SINCOS_PLATFORM( (float)Angle, &Sin, &Cos ) ;
	Out->z = In->z ;
	f      = In->x * Cos - In->y * Sin ;
	Out->y = In->x * Sin + In->y * Cos ;
	Out->x = f ;

	// �I��
	return 0 ;
}

// �x�N�g���̂y�������ɂ�����]���s��
extern int VectorRotationZD( VECTOR_D *Out, const VECTOR_D *In, double Angle )
{
	double f ;
	double Sin, Cos ;

	_SINCOSD( Angle, &Sin, &Cos ) ;
	Out->z = In->z ;
	f      = In->x * Cos - In->y * Sin ;
	Out->y = In->x * Sin + In->y * Cos ;
	Out->x = f ;

	// �I��
	return 0 ;
}



// �x�N�g���s���4x4�����s�����Z����( w �� 1 �Ɖ��� )
extern int VectorTransform( VECTOR *Out, const VECTOR *InVec, const MATRIX *InMatrix )
{
	VECTOR InV = *InVec ;

	Out->x = InV.x * InMatrix->m[0][0] + InV.y * InMatrix->m[1][0] + InV.z * InMatrix->m[2][0] + InMatrix->m[3][0] ;
	Out->y = InV.x * InMatrix->m[0][1] + InV.y * InMatrix->m[1][1] + InV.z * InMatrix->m[2][1] + InMatrix->m[3][1] ;
	Out->z = InV.x * InMatrix->m[0][2] + InV.y * InMatrix->m[1][2] + InV.z * InMatrix->m[2][2] + InMatrix->m[3][2] ;

	return 0 ;
}

// �x�N�g���s���4x4�����s�����Z����( w �� 1 �Ɖ��� )
extern int VectorTransformD( VECTOR_D *Out, const VECTOR_D *InVec, const MATRIX_D *InMatrix )
{
	VECTOR_D InV = *InVec ;

	Out->x = InV.x * InMatrix->m[0][0] + InV.y * InMatrix->m[1][0] + InV.z * InMatrix->m[2][0] + InMatrix->m[3][0] ;
	Out->y = InV.x * InMatrix->m[0][1] + InV.y * InMatrix->m[1][1] + InV.z * InMatrix->m[2][1] + InMatrix->m[3][1] ;
	Out->z = InV.x * InMatrix->m[0][2] + InV.y * InMatrix->m[1][2] + InV.z * InMatrix->m[2][2] + InMatrix->m[3][2] ;

	return 0 ;
}

// �x�N�g���s���4x4�����s��̉�]�����݂̂���Z����
extern int VectorTransformSR( VECTOR *Out, const VECTOR *InVec, const MATRIX *InMatrix )
{
	VECTOR InV = *InVec ;

	Out->x = InV.x * InMatrix->m[0][0] + InV.y * InMatrix->m[1][0] + InV.z * InMatrix->m[2][0] ;
	Out->y = InV.x * InMatrix->m[0][1] + InV.y * InMatrix->m[1][1] + InV.z * InMatrix->m[2][1] ;
	Out->z = InV.x * InMatrix->m[0][2] + InV.y * InMatrix->m[1][2] + InV.z * InMatrix->m[2][2] ;

	return 0 ;
}

// �x�N�g���s���4x4�����s��̉�]�����݂̂���Z����
extern int VectorTransformSRD( VECTOR_D *Out, const VECTOR_D *InVec, const MATRIX_D *InMatrix )
{
	VECTOR_D InV = *InVec ;

	Out->x = InV.x * InMatrix->m[0][0] + InV.y * InMatrix->m[1][0] + InV.z * InMatrix->m[2][0] ;
	Out->y = InV.x * InMatrix->m[0][1] + InV.y * InMatrix->m[1][1] + InV.z * InMatrix->m[2][1] ;
	Out->z = InV.x * InMatrix->m[0][2] + InV.y * InMatrix->m[1][2] + InV.z * InMatrix->m[2][2] ;

	return 0 ;
}

// �x�N�g���s���4x4�����s�����Z����( w �̗v�f��n�� )
extern int VectorTransform4( VECTOR *Out, float *V4Out, const VECTOR *InVec, const float *V4In, const MATRIX *InMatrix )
{
	VECTOR invec ;
	float v4in ;

	invec = *InVec ;
	v4in = *V4In ;
	Out->x = invec.x * InMatrix->m[0][0] + invec.y * InMatrix->m[1][0] + invec.z * InMatrix->m[2][0] + v4in * InMatrix->m[3][0]  ;
	Out->y = invec.x * InMatrix->m[0][1] + invec.y * InMatrix->m[1][1] + invec.z * InMatrix->m[2][1] + v4in * InMatrix->m[3][1]  ;
	Out->z = invec.x * InMatrix->m[0][2] + invec.y * InMatrix->m[1][2] + invec.z * InMatrix->m[2][2] + v4in * InMatrix->m[3][2]  ;
	*V4Out = invec.x * InMatrix->m[0][3] + invec.y * InMatrix->m[1][3] + invec.z * InMatrix->m[2][3] + v4in * InMatrix->m[3][3]  ;

	return 0 ;
}

// �x�N�g���s���4x4�����s�����Z����( w �̗v�f��n�� )
extern int VectorTransform4D( VECTOR_D *Out, double *V4Out, const VECTOR_D *InVec, const double *V4In, const MATRIX_D *InMatrix )
{
	VECTOR_D invec ;
	double v4in ;

	invec = *InVec ;
	v4in = *V4In ;
	Out->x = invec.x * InMatrix->m[0][0] + invec.y * InMatrix->m[1][0] + invec.z * InMatrix->m[2][0] + v4in * InMatrix->m[3][0]  ;
	Out->y = invec.x * InMatrix->m[0][1] + invec.y * InMatrix->m[1][1] + invec.z * InMatrix->m[2][1] + v4in * InMatrix->m[3][1]  ;
	Out->z = invec.x * InMatrix->m[0][2] + invec.y * InMatrix->m[1][2] + invec.z * InMatrix->m[2][2] + v4in * InMatrix->m[3][2]  ;
	*V4Out = invec.x * InMatrix->m[0][3] + invec.y * InMatrix->m[1][3] + invec.z * InMatrix->m[2][3] + v4in * InMatrix->m[3][3]  ;

	return 0 ;
}

// ��̐����̍Őڋߓ_������͂���
extern int Segment_Segment_Analyse( const VECTOR *SegmentAPos1, const VECTOR *SegmentAPos2, const VECTOR *SegmentBPos1, const VECTOR *SegmentBPos2, SEGMENT_SEGMENT_RESULT *Result )
{
	VECTOR segA1_2 ;
	VECTOR segB1_2 ;
	VECTOR segB1_A1 ;
	VECTOR tp ;
	float a, b, c, d, e, f, w, s, t = 0.0f ;
	float tmpA, tmpB ;
	SEGMENT_POINT_RESULT spres ;

	VectorSub( &segA1_2, SegmentAPos2, SegmentAPos1 ) ;
	VectorSub( &segB1_2, SegmentBPos2, SegmentBPos1 ) ;
	VectorSub( &segB1_A1, SegmentAPos1, SegmentBPos1 ) ;
	a =  VectorInnerProduct( &segA1_2, &segA1_2 ) ;
	b = -VectorInnerProduct( &segA1_2, &segB1_2 ) ;
	c = -b ;
	d = -VectorInnerProduct( &segB1_2, &segB1_2 ) ;
	e = -VectorInnerProduct( &segA1_2, &segB1_A1 ) ;
	f = -VectorInnerProduct( &segB1_2, &segB1_A1 ) ;

	// SegmentAPos1 - SegmentAPos2 �܂��� SegmentBPos1 - SegmentBPos2 �̋���������Ȃ��[���ɋ߂����ǂ����̃`�F�b�N
	tmpA = a < 0.0f ? -a : a ;
	tmpB = d < 0.0f ? -d : d ;
	if( tmpA < 0.00000001f )
	{
		Result->SegA_MinDist_Pos1_Pos2_t = 0.0f ;
		Result->SegA_MinDist_Pos = *SegmentAPos1 ;
		if( tmpB < 0.00000001f )
		{
			Result->SegB_MinDist_Pos1_Pos2_t = 0.0f ;
			Result->SegB_MinDist_Pos = *SegmentBPos1 ;
			Result->SegA_SegB_MinDist_Square = VSquareSize( segB1_A1 ) ;
		}
		else
		{
			Segment_Point_Analyse( SegmentBPos1, SegmentBPos2, SegmentAPos1, &spres ) ;

			Result->SegB_MinDist_Pos1_Pos2_t = spres.Seg_MinDist_Pos1_Pos2_t ;
			Result->SegB_MinDist_Pos = spres.Seg_MinDist_Pos ;
			Result->SegA_SegB_MinDist_Square = spres.Seg_Point_MinDist_Square ;
		}
		return 0 ;
	}
	else
	if( tmpB < 0.00000001f )
	{
		Result->SegB_MinDist_Pos1_Pos2_t = 0.0f ;
		Result->SegB_MinDist_Pos = *SegmentBPos1 ;

		Segment_Point_Analyse( SegmentAPos1, SegmentAPos2, SegmentBPos1, &spres ) ;

		Result->SegA_MinDist_Pos1_Pos2_t = spres.Seg_MinDist_Pos1_Pos2_t ;
		Result->SegA_MinDist_Pos = spres.Seg_MinDist_Pos ;
		Result->SegA_SegB_MinDist_Square = spres.Seg_Point_MinDist_Square ;

		return 0 ;
	}

	s = -0.1f ;
	w = a * d - b * c ;
	if( w <= -0.00000001f )
	{
		// �N���[�����̌���
		s = ( e * d - b * f ) / w ;
		t = ( a * f - e * c ) / w ;
	}

	// ��̐������k�ނ��Ă�����_�ƌ��Ȃ��ē_���m�̋�����Ԃ�
	if( a <= 0.0f && -d <= 0.0f )
	{
		Result->SegA_MinDist_Pos1_Pos2_t = 0.0f ;
		Result->SegB_MinDist_Pos1_Pos2_t = 0.0f ;
		Result->SegA_MinDist_Pos = *SegmentAPos1 ;
		Result->SegB_MinDist_Pos = *SegmentBPos1 ;
		Result->SegA_SegB_MinDist_Square = VSquareSize( segB1_A1 ) ;

		return 0 ;
	}

	if( a <= 0.0f )
	{
		s = 0.0f ;
		if( t > 1.0f || t < 0.0f )
			t = t > 1.0f ? 1.0f : 0.0f ;
	}
	else
	if( -d <= 0.0f )
	{
		t = 0.0f ;
		if( s > 1.0f || s < 0.0f )
			s = s > 1.0f ? 1.0f : 0.0f ;
	}
	else
	{
		if( s > 1.0f || s < 0.0f )
		{
			s = s > 1.0f ? 1.0f : 0.0f ;

			t = ( -b * s + -f ) / -d ;		// �K�E�X�����@
			if( t > 1.0f || t < 0.0f )
			{
				t = t > 1.0f ? 1.0f : 0.0f ;
				s = ( -b * t - -e ) / a ;	// �K�E�X�����@
					 if( s > 1.0f ) s = 1.0f ;
				else if( s < 0.0f ) s = 0.0f ;
			}
		}
		else
		if( t > 1.0f || t < 0.0f )
		{
			t = t > 1.0f ? 1.0f : 0.0f ;
			s = ( -b * t - -e ) / a ;		// �K�E�X�����@

			if( s > 1.0f || s < 0.0f )
			{
				s = s > 1.0f ? 1.0f : 0.0f ;
				t = ( -b * s + -f ) / -d ;	// �K�E�X�����@
					 if( t > 1.0f ) t = 1.0f ;
				else if( t < 0.0f ) t = 0.0f ;
			}
		}
	}

//	NS_DrawLine( SegmentAPos1->x, SegmentAPos1->y, SegmentAPos2->x, SegmentAPos2->y, GetColor( 255,255,255 ), 1 ) ;
//	NS_DrawLine( SegmentBPos1->x, SegmentBPos1->y, SegmentBPos2->x, SegmentBPos2->y, GetColor( 255,0,0 ), 1 ) ;
//	NS_DrawFormatString( 0, 0, GetColor( 255,255,255 ), "s:%.3f t:%.3f", s, t ) ;

	Result->SegA_MinDist_Pos1_Pos2_t = s ;
	Result->SegB_MinDist_Pos1_Pos2_t = t ;

	VectorScale( &Result->SegA_MinDist_Pos, &segA1_2, s ) ;
	VectorAdd( &Result->SegA_MinDist_Pos, &Result->SegA_MinDist_Pos, SegmentAPos1 ) ;

	VectorScale( &Result->SegB_MinDist_Pos, &segB1_2, t ) ;
	VectorAdd( &Result->SegB_MinDist_Pos, &Result->SegB_MinDist_Pos, SegmentBPos1 ) ;

//	NS_DrawBox( Result->SegA_MinDist_Pos->x - 2, Result->SegA_MinDist_Pos->y - 2, Result->SegA_MinDist_Pos->x + 2, Result->SegA_MinDist_Pos->y + 2, GetColor( 0,255,0 ), TRUE ) ;
//	NS_DrawBox( Result->SegB_MinDist_Pos->x - 2, Result->SegB_MinDist_Pos->y - 2, Result->SegB_MinDist_Pos->x + 2, Result->SegB_MinDist_Pos->y + 2, GetColor( 255,255,0 ), TRUE ) ;
//	NS_DrawCube3D( Result->SegA_MinDist_Pos->x - 10.0f, Result->SegA_MinDist_Pos->y - 10.0f, Result->SegA_MinDist_Pos->z - 10.0f, 
//					Result->SegA_MinDist_Pos->x + 10.0f, Result->SegA_MinDist_Pos->y + 10.0f, Result->SegA_MinDist_Pos->z + 10.0f, GetColor( 255,0,0 ), TRUE ) ;
//	NS_DrawCube3D( Result->SegB_MinDist_Pos->x - 10.0f, Result->SegB_MinDist_Pos->y - 10.0f, Result->SegB_MinDist_Pos->z - 10.0f, 
//					Result->SegB_MinDist_Pos->x + 10.0f, Result->SegB_MinDist_Pos->y + 10.0f, Result->SegB_MinDist_Pos->z + 10.0f, GetColor( 255,0,255 ), TRUE ) ;

	VectorSub( &tp, &Result->SegA_MinDist_Pos, &Result->SegB_MinDist_Pos ) ;
	Result->SegA_SegB_MinDist_Square = VSquareSize( tp ) ;

	return 0 ;
}

// ��̐����̍Őڋߓ_������͂���
extern int Segment_Segment_AnalyseD( const VECTOR_D *SegmentAPos1, const VECTOR_D *SegmentAPos2, const VECTOR_D *SegmentBPos1, const VECTOR_D *SegmentBPos2, SEGMENT_SEGMENT_RESULT_D *Result )
{
	VECTOR_D segA1_2 ;
	VECTOR_D segB1_2 ;
	VECTOR_D segB1_A1 ;
	VECTOR_D tp ;
	double a, b, c, d, e, f, w, s, t = 0.0 ;
	double tmpA, tmpB ;
	SEGMENT_POINT_RESULT_D spres ;

	VectorSubD( &segA1_2, SegmentAPos2, SegmentAPos1 ) ;
	VectorSubD( &segB1_2, SegmentBPos2, SegmentBPos1 ) ;
	VectorSubD( &segB1_A1, SegmentAPos1, SegmentBPos1 ) ;
	a =  VectorInnerProductD( &segA1_2, &segA1_2 ) ;
	b = -VectorInnerProductD( &segA1_2, &segB1_2 ) ;
	c = -b ;
	d = -VectorInnerProductD( &segB1_2, &segB1_2 ) ;
	e = -VectorInnerProductD( &segA1_2, &segB1_A1 ) ;
	f = -VectorInnerProductD( &segB1_2, &segB1_A1 ) ;

	// SegmentAPos1 - SegmentAPos2 �܂��� SegmentBPos1 - SegmentBPos2 �̋���������Ȃ��[���ɋ߂����ǂ����̃`�F�b�N
	tmpA = a < 0.0 ? -a : a ;
	tmpB = d < 0.0 ? -d : d ;
	if( tmpA < 0.00000001 )
	{
		Result->SegA_MinDist_Pos1_Pos2_t = 0.0 ;
		Result->SegA_MinDist_Pos = *SegmentAPos1 ;
		if( tmpB < 0.00000001 )
		{
			Result->SegB_MinDist_Pos1_Pos2_t = 0.0 ;
			Result->SegB_MinDist_Pos = *SegmentBPos1 ;
			Result->SegA_SegB_MinDist_Square = VSquareSizeD( segB1_A1 ) ;
		}
		else
		{
			Segment_Point_AnalyseD( SegmentBPos1, SegmentBPos2, SegmentAPos1, &spres ) ;

			Result->SegB_MinDist_Pos1_Pos2_t = spres.Seg_MinDist_Pos1_Pos2_t ;
			Result->SegB_MinDist_Pos = spres.Seg_MinDist_Pos ;
			Result->SegA_SegB_MinDist_Square = spres.Seg_Point_MinDist_Square ;
		}
		return 0 ;
	}
	else
	if( tmpB < 0.00000001 )
	{
		Result->SegB_MinDist_Pos1_Pos2_t = 0.0 ;
		Result->SegB_MinDist_Pos = *SegmentBPos1 ;

		Segment_Point_AnalyseD( SegmentAPos1, SegmentAPos2, SegmentBPos1, &spres ) ;

		Result->SegA_MinDist_Pos1_Pos2_t = spres.Seg_MinDist_Pos1_Pos2_t ;
		Result->SegA_MinDist_Pos = spres.Seg_MinDist_Pos ;
		Result->SegA_SegB_MinDist_Square = spres.Seg_Point_MinDist_Square ;

		return 0 ;
	}

	s = -0.1 ;
	w = a * d - b * c ;
	if( w <= -0.00000001 )
	{
		// �N���[�����̌���
		s = ( e * d - b * f ) / w ;
		t = ( a * f - e * c ) / w ;
	}

	// ��̐������k�ނ��Ă�����_�ƌ��Ȃ��ē_���m�̋�����Ԃ�
	if( a <= 0.0 && -d <= 0.0 )
	{
		Result->SegA_MinDist_Pos1_Pos2_t = 0.0 ;
		Result->SegB_MinDist_Pos1_Pos2_t = 0.0 ;
		Result->SegA_MinDist_Pos = *SegmentAPos1 ;
		Result->SegB_MinDist_Pos = *SegmentBPos1 ;
		Result->SegA_SegB_MinDist_Square = VSquareSizeD( segB1_A1 ) ;

		return 0 ;
	}

	if( a <= 0.0 )
	{
		s = 0.0 ;
		if( t > 1.0 || t < 0.0 )
			t = t > 1.0 ? 1.0 : 0.0 ;
	}
	else
	if( -d <= 0.0 )
	{
		t = 0.0 ;
		if( s > 1.0 || s < 0.0 )
			s = s > 1.0 ? 1.0 : 0.0 ;
	}
	else
	{
		if( s > 1.0 || s < 0.0 )
		{
			s = s > 1.0 ? 1.0 : 0.0 ;

			t = ( -b * s + -f ) / -d ;		// �K�E�X�����@
			if( t > 1.0 || t < 0.0 )
			{
				t = t > 1.0 ? 1.0 : 0.0 ;
				s = ( -b * t - -e ) / a ;	// �K�E�X�����@
					 if( s > 1.0 ) s = 1.0 ;
				else if( s < 0.0 ) s = 0.0 ;
			}
		}
		else
		if( t > 1.0 || t < 0.0 )
		{
			t = t > 1.0 ? 1.0 : 0.0 ;
			s = ( -b * t - -e ) / a ;		// �K�E�X�����@

			if( s > 1.0 || s < 0.0 )
			{
				s = s > 1.0 ? 1.0 : 0.0 ;
				t = ( -b * s + -f ) / -d ;	// �K�E�X�����@
					 if( t > 1.0 ) t = 1.0 ;
				else if( t < 0.0 ) t = 0.0 ;
			}
		}
	}

	Result->SegA_MinDist_Pos1_Pos2_t = s ;
	Result->SegB_MinDist_Pos1_Pos2_t = t ;

	VectorScaleD( &Result->SegA_MinDist_Pos, &segA1_2, s ) ;
	VectorAddD( &Result->SegA_MinDist_Pos, &Result->SegA_MinDist_Pos, SegmentAPos1 ) ;

	VectorScaleD( &Result->SegB_MinDist_Pos, &segB1_2, t ) ;
	VectorAddD( &Result->SegB_MinDist_Pos, &Result->SegB_MinDist_Pos, SegmentBPos1 ) ;

	VectorSubD( &tp, &Result->SegA_MinDist_Pos, &Result->SegB_MinDist_Pos ) ;
	Result->SegA_SegB_MinDist_Square = VSquareSizeD( tp ) ;

	return 0 ;
}

// �����Ɠ_�̍Őڋߓ_������͂���
extern int Segment_Point_Analyse( const VECTOR *SegmentPos1, const VECTOR *SegmentPos2, const VECTOR *PointPos, SEGMENT_POINT_RESULT *Result )
{
	float SizeSquSeg1_2 ;
	float DotSeg1_2_Pnt ;
	VECTOR Seg1_2, Seg1_Pnt ;

	VectorSub( &Seg1_2, SegmentPos2, SegmentPos1 ) ;
	VectorSub( &Seg1_Pnt, PointPos, SegmentPos1 ) ;
	DotSeg1_2_Pnt = VDot( Seg1_2, Seg1_Pnt ) ;
	if( DotSeg1_2_Pnt <= 0.0f )
	{
		Result->Seg_MinDist_Pos = *SegmentPos1 ;
		Result->Seg_MinDist_Pos1_Pos2_t = 0.0f ;
		Result->Seg_Point_MinDist_Square = VSquareSize( VSub( *PointPos, Result->Seg_MinDist_Pos ) ) ;

		return 0 ;
	}

	SizeSquSeg1_2 = VSquareSize( Seg1_2 ) ;
	if( SizeSquSeg1_2 == 0.0f )
	{
		Result->Seg_MinDist_Pos = *SegmentPos1 ;
		Result->Seg_MinDist_Pos1_Pos2_t = 0.0f ;
		Result->Seg_Point_MinDist_Square = VSquareSize( VSub( *PointPos, Result->Seg_MinDist_Pos ) ) ;

		return 0 ;
	}

	if( SizeSquSeg1_2 < DotSeg1_2_Pnt )
	{
		Result->Seg_MinDist_Pos = *SegmentPos2 ;
		Result->Seg_MinDist_Pos1_Pos2_t = 1.0f ;
		Result->Seg_Point_MinDist_Square = VSquareSize( VSub( *PointPos, Result->Seg_MinDist_Pos ) ) ;

		return 0 ;
	}

	Result->Seg_MinDist_Pos1_Pos2_t = DotSeg1_2_Pnt / SizeSquSeg1_2 ;
	Result->Seg_MinDist_Pos = VAdd( VScale( Seg1_2, Result->Seg_MinDist_Pos1_Pos2_t ), *SegmentPos1 ) ;
	Result->Seg_Point_MinDist_Square = VSquareSize( VSub( *PointPos, Result->Seg_MinDist_Pos ) ) ;

	return 0 ;
}

// �����Ɠ_�̍Őڋߓ_������͂���
extern int Segment_Point_AnalyseD( const VECTOR_D *SegmentPos1, const VECTOR_D *SegmentPos2, const VECTOR_D *PointPos, SEGMENT_POINT_RESULT_D *Result )
{
	double SizeSquSeg1_2 ;
	double DotSeg1_2_Pnt ;
	VECTOR_D Seg1_2, Seg1_Pnt ;

	VectorSubD( &Seg1_2, SegmentPos2, SegmentPos1 ) ;
	VectorSubD( &Seg1_Pnt, PointPos, SegmentPos1 ) ;
	DotSeg1_2_Pnt = VDotD( Seg1_2, Seg1_Pnt ) ;
	if( DotSeg1_2_Pnt <= 0.0 )
	{
		Result->Seg_MinDist_Pos = *SegmentPos1 ;
		Result->Seg_MinDist_Pos1_Pos2_t = 0.0 ;
		Result->Seg_Point_MinDist_Square = VSquareSizeD( VSubD( *PointPos, Result->Seg_MinDist_Pos ) ) ;

		return 0 ;
	}

	SizeSquSeg1_2 = VSquareSizeD( Seg1_2 ) ;
	if( SizeSquSeg1_2 == 0.0 )
	{
		Result->Seg_MinDist_Pos = *SegmentPos1 ;
		Result->Seg_MinDist_Pos1_Pos2_t = 0.0 ;
		Result->Seg_Point_MinDist_Square = VSquareSizeD( VSubD( *PointPos, Result->Seg_MinDist_Pos ) ) ;

		return 0 ;
	}

	if( SizeSquSeg1_2 < DotSeg1_2_Pnt )
	{
		Result->Seg_MinDist_Pos = *SegmentPos2 ;
		Result->Seg_MinDist_Pos1_Pos2_t = 1.0 ;
		Result->Seg_Point_MinDist_Square = VSquareSizeD( VSubD( *PointPos, Result->Seg_MinDist_Pos ) ) ;

		return 0 ;
	}

	Result->Seg_MinDist_Pos1_Pos2_t = DotSeg1_2_Pnt / SizeSquSeg1_2 ;
	Result->Seg_MinDist_Pos = VAddD( VScaleD( Seg1_2, Result->Seg_MinDist_Pos1_Pos2_t ), *SegmentPos1 ) ;
	Result->Seg_Point_MinDist_Square = VSquareSizeD( VSubD( *PointPos, Result->Seg_MinDist_Pos ) ) ;

	return 0 ;
}

// �����ƎO�p�`�̍Őڋߓ_������͂���
extern int Segment_Triangle_Analyse( const VECTOR *SegmentPos1, const VECTOR *SegmentPos2, const VECTOR *TrianglePos1, const VECTOR *TrianglePos2, const VECTOR *TrianglePos3, SEGMENT_TRIANGLE_RESULT *Result )
{
	bool Project1, Project2 ;
	VECTOR Seg1_TriPro ;
	VECTOR Seg2_TriPro ;
	float Seg1_TriPro_w1 ;
	float Seg1_TriPro_w2 ;
	float Seg1_TriPro_w3 ;
	float Seg2_TriPro_w1 ;
	float Seg2_TriPro_w2 ;
	float Seg2_TriPro_w3 ;
	VECTOR Seg1_2 ;
	VECTOR Tri1_Seg1 ;
	VECTOR Tri1_Seg2 ;
	VECTOR TriNorm ;
	VECTOR tv ;
	VECTOR Tri1_2, Tri1_3 ;
	float Seg1_TriProSeg1_Dist = 0.0f ;
	float Seg2_TriProSeg2_Dist = 0.0f ;
	int MinType ;
	float MinLen ;
	SEGMENT_SEGMENT_RESULT Seg_Tri1_2_Res ;
	SEGMENT_SEGMENT_RESULT Seg_Tri2_3_Res ;
	SEGMENT_SEGMENT_RESULT Seg_Tri3_1_Res ;
	float TriPos_SegPos1_2_t ;
	VECTOR Seg_TriPos ;
	float Seg_TriPos_TriPos1_w ;
	float Seg_TriPos_TriPos2_w ;
	float Seg_TriPos_TriPos3_w ;

	VectorSub( &Seg1_2, SegmentPos2, SegmentPos1 ) ;
	VectorSub( &Tri1_2, TrianglePos2, TrianglePos1 ) ;
	VectorSub( &Tri1_3, TrianglePos3, TrianglePos1 ) ;
	VectorOuterProduct( &TriNorm, &Tri1_2, &Tri1_3 ) ;
	VectorNormalize( &TriNorm, &TriNorm ) ;

	// �O�p�`���������ʏ�ɒ��_�𓊉e����
	VectorSub( &Tri1_Seg1, SegmentPos1, TrianglePos1 ) ;
	Seg1_TriProSeg1_Dist = VectorInnerProduct( &Tri1_Seg1, &TriNorm ) ;
	VectorScale( &tv, &TriNorm, Seg1_TriProSeg1_Dist ) ;
	VectorSub( &Seg1_TriPro, SegmentPos1, &tv ) ;
	if( Seg1_TriProSeg1_Dist < 0.0f ) Seg1_TriProSeg1_Dist = -Seg1_TriProSeg1_Dist ;
	

//	NS_DrawCube3D( Seg1_TriPro.x - 10.0f, Seg1_TriPro.y - 10.0f, Seg1_TriPro.z - 10.0f, 
//					Seg1_TriPro.x + 10.0f, Seg1_TriPro.y + 10.0f, Seg1_TriPro.z + 10.0f, GetColor( 0,255,255 ), TRUE ) ;

	// �O�p�`���ɒ��_�����݂��邩�ǂ����𒲂ׂ�
	TriangleBarycenter_Base( TrianglePos1, TrianglePos2, TrianglePos3, &Seg1_TriPro, &Seg1_TriPro_w1, &Seg1_TriPro_w2, &Seg1_TriPro_w3 ) ;
	Project1 = Seg1_TriPro_w1 >= 0.0f && Seg1_TriPro_w1 <= 1.0f && Seg1_TriPro_w2 >= 0.0f && Seg1_TriPro_w2 <= 1.0f && Seg1_TriPro_w3 >= 0.0f && Seg1_TriPro_w3 <= 1.0f ;

	// �O�p�`���������ʏ�ɒ��_�𓊉e����
	VectorSub( &Tri1_Seg2, SegmentPos2, TrianglePos1 ) ;
	Seg2_TriProSeg2_Dist = VectorInnerProduct( &Tri1_Seg2, &TriNorm ) ;
	VectorScale( &tv, &TriNorm, Seg2_TriProSeg2_Dist ) ;
	VectorSub( &Seg2_TriPro, SegmentPos2, &tv ) ;
	if( Seg2_TriProSeg2_Dist < 0.0f ) Seg2_TriProSeg2_Dist = -Seg2_TriProSeg2_Dist ;

	// �O�p�`���ɒ��_�����݂��邩�ǂ����𒲂ׂ�
	TriangleBarycenter_Base( TrianglePos1, TrianglePos2, TrianglePos3, &Seg2_TriPro, &Seg2_TriPro_w1, &Seg2_TriPro_w2, &Seg2_TriPro_w3 ) ;
	Project2 = Seg2_TriPro_w1 >= 0.0f && Seg2_TriPro_w1 <= 1.0f && Seg2_TriPro_w2 >= 0.0f && Seg2_TriPro_w2 <= 1.0f && Seg2_TriPro_w3 >= 0.0f && Seg2_TriPro_w3 <= 1.0f ;


	// �O�p�`���\�����ʂƐ����Ƃ̐ړ_�Əd�S���Z�o����
	TriPos_SegPos1_2_t = Seg1_TriProSeg1_Dist / ( Seg1_TriProSeg1_Dist + Seg2_TriProSeg2_Dist ) ;
	VectorScale( &Seg_TriPos, &Seg1_2, TriPos_SegPos1_2_t ) ;
	VectorAdd( &Seg_TriPos, &Seg_TriPos, SegmentPos1 ) ;

	TriangleBarycenter_Base( TrianglePos1, TrianglePos2, TrianglePos3, &Seg_TriPos,
		&Seg_TriPos_TriPos1_w, &Seg_TriPos_TriPos2_w, &Seg_TriPos_TriPos3_w ) ;


	// �����ƎO�p�`���ڂ��Ă��邩�`�F�b�N
	if( Seg_TriPos_TriPos1_w >= 0.0f && Seg_TriPos_TriPos1_w <= 1.0f &&
		Seg_TriPos_TriPos2_w >= 0.0f && Seg_TriPos_TriPos2_w <= 1.0f &&
		Seg_TriPos_TriPos3_w >= 0.0f && Seg_TriPos_TriPos3_w <= 1.0f )
	{
		Result->Seg_Tri_MinDist_Square = 0.0f ;

		Result->Seg_MinDist_Pos1_Pos2_t = TriPos_SegPos1_2_t ;
		Result->Seg_MinDist_Pos = Seg_TriPos ;

		Result->Tri_MinDist_Pos1_w = Seg_TriPos_TriPos1_w ;
		Result->Tri_MinDist_Pos2_w = Seg_TriPos_TriPos2_w ;
		Result->Tri_MinDist_Pos3_w = Seg_TriPos_TriPos3_w ;

		Result->Tri_MinDist_Pos = Result->Seg_MinDist_Pos ;
		return 0 ;
	}

	// �ǂ�����O�p�`�͈̔͂ɂ������ꍇ
	if( Project1 && Project2 )
	{
		// �Е��̑��ɂǂ���̒��_������ꍇ�́A���߂��ق��̒��_�ƕ��ʂƂ̋��������ʂɂ���
		if( Seg1_TriProSeg1_Dist < Seg2_TriProSeg2_Dist )
		{
			Result->Seg_Tri_MinDist_Square = Seg1_TriProSeg1_Dist * Seg1_TriProSeg1_Dist ;
			Result->Seg_MinDist_Pos1_Pos2_t = 0.0f ;
			Result->Seg_MinDist_Pos = *SegmentPos1 ;
			Result->Tri_MinDist_Pos1_w = Seg1_TriPro_w1 ;
			Result->Tri_MinDist_Pos2_w = Seg1_TriPro_w2 ;
			Result->Tri_MinDist_Pos3_w = Seg1_TriPro_w3 ;
			Result->Tri_MinDist_Pos = Seg1_TriPro ;
		}
		else
		{
			Result->Seg_Tri_MinDist_Square = Seg2_TriProSeg2_Dist * Seg2_TriProSeg2_Dist ;
			Result->Seg_MinDist_Pos1_Pos2_t = 1.0f ;
			Result->Seg_MinDist_Pos = *SegmentPos2 ;
			Result->Tri_MinDist_Pos1_w = Seg2_TriPro_w1 ;
			Result->Tri_MinDist_Pos2_w = Seg2_TriPro_w2 ;
			Result->Tri_MinDist_Pos3_w = Seg2_TriPro_w3 ;
			Result->Tri_MinDist_Pos = Seg2_TriPro ;
		}
		return 0 ;
	}

	// �����ł͂Ȃ��ꍇ�͎O�p�`�̂R�ӂƐ����̋����𑪂�A��ԋ������Z���ӂ�T��
	Segment_Segment_Analyse( SegmentPos1, SegmentPos2, TrianglePos1, TrianglePos2, &Seg_Tri1_2_Res ) ;
	Segment_Segment_Analyse( SegmentPos1, SegmentPos2, TrianglePos2, TrianglePos3, &Seg_Tri2_3_Res ) ;
	Segment_Segment_Analyse( SegmentPos1, SegmentPos2, TrianglePos3, TrianglePos1, &Seg_Tri3_1_Res ) ;

	MinLen = Seg_Tri1_2_Res.SegA_SegB_MinDist_Square ;
	MinType = 0 ;
	if( Seg_Tri2_3_Res.SegA_SegB_MinDist_Square < MinLen )
	{
		MinType = 1 ;
		MinLen = Seg_Tri2_3_Res.SegA_SegB_MinDist_Square ;
	}
	if( Seg_Tri3_1_Res.SegA_SegB_MinDist_Square < MinLen )
	{
		MinType = 2 ;
		MinLen = Seg_Tri3_1_Res.SegA_SegB_MinDist_Square ;
	}
	if( Project1 && Seg1_TriProSeg1_Dist * Seg1_TriProSeg1_Dist < MinLen )
	{
		MinType = 3 ;
		MinLen = Seg1_TriProSeg1_Dist * Seg1_TriProSeg1_Dist ;
	}
	if( Project2 && Seg2_TriProSeg2_Dist * Seg2_TriProSeg2_Dist < MinLen )
	{
		MinType = 4 ;
		MinLen = Seg2_TriProSeg2_Dist * Seg2_TriProSeg2_Dist ;
	}

	Result->Seg_Tri_MinDist_Square = MinLen ;
	switch( MinType )
	{
	case 0 :
		Result->Seg_MinDist_Pos1_Pos2_t = Seg_Tri1_2_Res.SegA_MinDist_Pos1_Pos2_t ;
		Result->Seg_MinDist_Pos = Seg_Tri1_2_Res.SegA_MinDist_Pos ;
		Result->Tri_MinDist_Pos1_w = 1.0f - Seg_Tri1_2_Res.SegB_MinDist_Pos1_Pos2_t ;
		Result->Tri_MinDist_Pos2_w =        Seg_Tri1_2_Res.SegB_MinDist_Pos1_Pos2_t ;
		Result->Tri_MinDist_Pos3_w = 0.0f ;
		Result->Tri_MinDist_Pos = Seg_Tri1_2_Res.SegB_MinDist_Pos ;
		break ;

	case 1 :
		Result->Seg_MinDist_Pos1_Pos2_t = Seg_Tri2_3_Res.SegA_MinDist_Pos1_Pos2_t ;
		Result->Seg_MinDist_Pos = Seg_Tri2_3_Res.SegA_MinDist_Pos ;
		Result->Tri_MinDist_Pos1_w = 0.0f ;
		Result->Tri_MinDist_Pos2_w = 1.0f - Seg_Tri2_3_Res.SegB_MinDist_Pos1_Pos2_t ;
		Result->Tri_MinDist_Pos3_w =        Seg_Tri2_3_Res.SegB_MinDist_Pos1_Pos2_t ;
		Result->Tri_MinDist_Pos = Seg_Tri2_3_Res.SegB_MinDist_Pos ;
		break ;

	case 2 :
		Result->Seg_MinDist_Pos1_Pos2_t = Seg_Tri3_1_Res.SegA_MinDist_Pos1_Pos2_t ;
		Result->Seg_MinDist_Pos = Seg_Tri3_1_Res.SegA_MinDist_Pos ;
		Result->Tri_MinDist_Pos1_w =        Seg_Tri3_1_Res.SegB_MinDist_Pos1_Pos2_t ;
		Result->Tri_MinDist_Pos2_w = 0.0f ;
		Result->Tri_MinDist_Pos3_w = 1.0f - Seg_Tri3_1_Res.SegB_MinDist_Pos1_Pos2_t ;
		Result->Tri_MinDist_Pos = Seg_Tri3_1_Res.SegB_MinDist_Pos ;
		break ;

	case 3 :
		Result->Seg_MinDist_Pos1_Pos2_t = 0.0f ;
		Result->Seg_MinDist_Pos = *SegmentPos1 ;
		Result->Tri_MinDist_Pos1_w = Seg1_TriPro_w1 ;
		Result->Tri_MinDist_Pos2_w = Seg1_TriPro_w2 ;
		Result->Tri_MinDist_Pos3_w = Seg1_TriPro_w3 ;
		Result->Tri_MinDist_Pos = Seg1_TriPro ;
		break ;

	case 4 :
		Result->Seg_MinDist_Pos1_Pos2_t = 1.0f ;
		Result->Seg_MinDist_Pos = *SegmentPos2 ;
		Result->Tri_MinDist_Pos1_w = Seg2_TriPro_w1 ;
		Result->Tri_MinDist_Pos2_w = Seg2_TriPro_w2 ;
		Result->Tri_MinDist_Pos3_w = Seg2_TriPro_w3 ;
		Result->Tri_MinDist_Pos = Seg2_TriPro ;
		break ;
	}

	return 0 ;
}

// �����ƎO�p�`�̍Őڋߓ_������͂���
extern int Segment_Triangle_AnalyseD( const VECTOR_D *SegmentPos1, const VECTOR_D *SegmentPos2, const VECTOR_D *TrianglePos1, const VECTOR_D *TrianglePos2, const VECTOR_D *TrianglePos3, SEGMENT_TRIANGLE_RESULT_D *Result )
{
	bool Project1, Project2 ;
	VECTOR_D Seg1_TriPro ;
	VECTOR_D Seg2_TriPro ;
	double Seg1_TriPro_w1 ;
	double Seg1_TriPro_w2 ;
	double Seg1_TriPro_w3 ;
	double Seg2_TriPro_w1 ;
	double Seg2_TriPro_w2 ;
	double Seg2_TriPro_w3 ;
	VECTOR_D Seg1_2 ;
	VECTOR_D Tri1_Seg1 ;
	VECTOR_D Tri1_Seg2 ;
	VECTOR_D TriNorm ;
	VECTOR_D tv ;
	VECTOR_D Tri1_2, Tri1_3 ;
	double Seg1_TriProSeg1_Dist = 0.0 ;
	double Seg2_TriProSeg2_Dist = 0.0 ;
	int MinType ;
	double MinLen ;
	SEGMENT_SEGMENT_RESULT_D Seg_Tri1_2_Res ;
	SEGMENT_SEGMENT_RESULT_D Seg_Tri2_3_Res ;
	SEGMENT_SEGMENT_RESULT_D Seg_Tri3_1_Res ;
	double TriPos_SegPos1_2_t ;
	VECTOR_D Seg_TriPos ;
	double Seg_TriPos_TriPos1_w ;
	double Seg_TriPos_TriPos2_w ;
	double Seg_TriPos_TriPos3_w ;

	VectorSubD( &Seg1_2, SegmentPos2, SegmentPos1 ) ;
	VectorSubD( &Tri1_2, TrianglePos2, TrianglePos1 ) ;
	VectorSubD( &Tri1_3, TrianglePos3, TrianglePos1 ) ;
	VectorOuterProductD( &TriNorm, &Tri1_2, &Tri1_3 ) ;
	VectorNormalizeD( &TriNorm, &TriNorm ) ;

	// �O�p�`���������ʏ�ɒ��_�𓊉e����
	VectorSubD( &Tri1_Seg1, SegmentPos1, TrianglePos1 ) ;
	Seg1_TriProSeg1_Dist = VectorInnerProductD( &Tri1_Seg1, &TriNorm ) ;
	VectorScaleD( &tv, &TriNorm, Seg1_TriProSeg1_Dist ) ;
	VectorSubD( &Seg1_TriPro, SegmentPos1, &tv ) ;
	if( Seg1_TriProSeg1_Dist < 0.0 ) Seg1_TriProSeg1_Dist = -Seg1_TriProSeg1_Dist ;
	

	// �O�p�`���ɒ��_�����݂��邩�ǂ����𒲂ׂ�
	TriangleBarycenter_BaseD( TrianglePos1, TrianglePos2, TrianglePos3, &Seg1_TriPro, &Seg1_TriPro_w1, &Seg1_TriPro_w2, &Seg1_TriPro_w3 ) ;
	Project1 = Seg1_TriPro_w1 >= 0.0 && Seg1_TriPro_w1 <= 1.0 && Seg1_TriPro_w2 >= 0.0 && Seg1_TriPro_w2 <= 1.0 && Seg1_TriPro_w3 >= 0.0 && Seg1_TriPro_w3 <= 1.0 ;

	// �O�p�`���������ʏ�ɒ��_�𓊉e����
	VectorSubD( &Tri1_Seg2, SegmentPos2, TrianglePos1 ) ;
	Seg2_TriProSeg2_Dist = VectorInnerProductD( &Tri1_Seg2, &TriNorm ) ;
	VectorScaleD( &tv, &TriNorm, Seg2_TriProSeg2_Dist ) ;
	VectorSubD( &Seg2_TriPro, SegmentPos2, &tv ) ;
	if( Seg2_TriProSeg2_Dist < 0.0 ) Seg2_TriProSeg2_Dist = -Seg2_TriProSeg2_Dist ;

	// �O�p�`���ɒ��_�����݂��邩�ǂ����𒲂ׂ�
	TriangleBarycenter_BaseD( TrianglePos1, TrianglePos2, TrianglePos3, &Seg2_TriPro, &Seg2_TriPro_w1, &Seg2_TriPro_w2, &Seg2_TriPro_w3 ) ;
	Project2 = Seg2_TriPro_w1 >= 0.0 && Seg2_TriPro_w1 <= 1.0 && Seg2_TriPro_w2 >= 0.0 && Seg2_TriPro_w2 <= 1.0 && Seg2_TriPro_w3 >= 0.0 && Seg2_TriPro_w3 <= 1.0 ;


	// �O�p�`���\�����ʂƐ����Ƃ̐ړ_�Əd�S���Z�o����
	TriPos_SegPos1_2_t = Seg1_TriProSeg1_Dist / ( Seg1_TriProSeg1_Dist + Seg2_TriProSeg2_Dist ) ;
	VectorScaleD( &Seg_TriPos, &Seg1_2, TriPos_SegPos1_2_t ) ;
	VectorAddD( &Seg_TriPos, &Seg_TriPos, SegmentPos1 ) ;

	TriangleBarycenter_BaseD( TrianglePos1, TrianglePos2, TrianglePos3, &Seg_TriPos,
		&Seg_TriPos_TriPos1_w, &Seg_TriPos_TriPos2_w, &Seg_TriPos_TriPos3_w ) ;


	// �����ƎO�p�`���ڂ��Ă��邩�`�F�b�N
	if( Seg_TriPos_TriPos1_w >= 0.0 && Seg_TriPos_TriPos1_w <= 1.0 &&
		Seg_TriPos_TriPos2_w >= 0.0 && Seg_TriPos_TriPos2_w <= 1.0 &&
		Seg_TriPos_TriPos3_w >= 0.0 && Seg_TriPos_TriPos3_w <= 1.0 )
	{
		Result->Seg_Tri_MinDist_Square = 0.0 ;

		Result->Seg_MinDist_Pos1_Pos2_t = TriPos_SegPos1_2_t ;
		Result->Seg_MinDist_Pos = Seg_TriPos ;

		Result->Tri_MinDist_Pos1_w = Seg_TriPos_TriPos1_w ;
		Result->Tri_MinDist_Pos2_w = Seg_TriPos_TriPos2_w ;
		Result->Tri_MinDist_Pos3_w = Seg_TriPos_TriPos3_w ;

		Result->Tri_MinDist_Pos = Result->Seg_MinDist_Pos ;
		return 0 ;
	}

	// �ǂ�����O�p�`�͈̔͂ɂ������ꍇ
	if( Project1 && Project2 )
	{
		// �Е��̑��ɂǂ���̒��_������ꍇ�́A���߂��ق��̒��_�ƕ��ʂƂ̋��������ʂɂ���
		if( Seg1_TriProSeg1_Dist < Seg2_TriProSeg2_Dist )
		{
			Result->Seg_Tri_MinDist_Square = Seg1_TriProSeg1_Dist * Seg1_TriProSeg1_Dist ;
			Result->Seg_MinDist_Pos1_Pos2_t = 0.0 ;
			Result->Seg_MinDist_Pos = *SegmentPos1 ;
			Result->Tri_MinDist_Pos1_w = Seg1_TriPro_w1 ;
			Result->Tri_MinDist_Pos2_w = Seg1_TriPro_w2 ;
			Result->Tri_MinDist_Pos3_w = Seg1_TriPro_w3 ;
			Result->Tri_MinDist_Pos = Seg1_TriPro ;
		}
		else
		{
			Result->Seg_Tri_MinDist_Square = Seg2_TriProSeg2_Dist * Seg2_TriProSeg2_Dist ;
			Result->Seg_MinDist_Pos1_Pos2_t = 1.0 ;
			Result->Seg_MinDist_Pos = *SegmentPos2 ;
			Result->Tri_MinDist_Pos1_w = Seg2_TriPro_w1 ;
			Result->Tri_MinDist_Pos2_w = Seg2_TriPro_w2 ;
			Result->Tri_MinDist_Pos3_w = Seg2_TriPro_w3 ;
			Result->Tri_MinDist_Pos = Seg2_TriPro ;
		}
		return 0 ;
	}

	// �����ł͂Ȃ��ꍇ�͎O�p�`�̂R�ӂƐ����̋����𑪂�A��ԋ������Z���ӂ�T��
	Segment_Segment_AnalyseD( SegmentPos1, SegmentPos2, TrianglePos1, TrianglePos2, &Seg_Tri1_2_Res ) ;
	Segment_Segment_AnalyseD( SegmentPos1, SegmentPos2, TrianglePos2, TrianglePos3, &Seg_Tri2_3_Res ) ;
	Segment_Segment_AnalyseD( SegmentPos1, SegmentPos2, TrianglePos3, TrianglePos1, &Seg_Tri3_1_Res ) ;

	MinLen = Seg_Tri1_2_Res.SegA_SegB_MinDist_Square ;
	MinType = 0 ;
	if( Seg_Tri2_3_Res.SegA_SegB_MinDist_Square < MinLen )
	{
		MinType = 1 ;
		MinLen = Seg_Tri2_3_Res.SegA_SegB_MinDist_Square ;
	}
	if( Seg_Tri3_1_Res.SegA_SegB_MinDist_Square < MinLen )
	{
		MinType = 2 ;
		MinLen = Seg_Tri3_1_Res.SegA_SegB_MinDist_Square ;
	}
	if( Project1 && Seg1_TriProSeg1_Dist * Seg1_TriProSeg1_Dist < MinLen )
	{
		MinType = 3 ;
		MinLen = Seg1_TriProSeg1_Dist * Seg1_TriProSeg1_Dist ;
	}
	if( Project2 && Seg2_TriProSeg2_Dist * Seg2_TriProSeg2_Dist < MinLen )
	{
		MinType = 4 ;
		MinLen = Seg2_TriProSeg2_Dist * Seg2_TriProSeg2_Dist ;
	}

	Result->Seg_Tri_MinDist_Square = MinLen ;
	switch( MinType )
	{
	case 0 :
		Result->Seg_MinDist_Pos1_Pos2_t = Seg_Tri1_2_Res.SegA_MinDist_Pos1_Pos2_t ;
		Result->Seg_MinDist_Pos = Seg_Tri1_2_Res.SegA_MinDist_Pos ;
		Result->Tri_MinDist_Pos1_w = 1.0 - Seg_Tri1_2_Res.SegB_MinDist_Pos1_Pos2_t ;
		Result->Tri_MinDist_Pos2_w =        Seg_Tri1_2_Res.SegB_MinDist_Pos1_Pos2_t ;
		Result->Tri_MinDist_Pos3_w = 0.0 ;
		Result->Tri_MinDist_Pos = Seg_Tri1_2_Res.SegB_MinDist_Pos ;
		break ;

	case 1 :
		Result->Seg_MinDist_Pos1_Pos2_t = Seg_Tri2_3_Res.SegA_MinDist_Pos1_Pos2_t ;
		Result->Seg_MinDist_Pos = Seg_Tri2_3_Res.SegA_MinDist_Pos ;
		Result->Tri_MinDist_Pos1_w = 0.0 ;
		Result->Tri_MinDist_Pos2_w = 1.0 - Seg_Tri2_3_Res.SegB_MinDist_Pos1_Pos2_t ;
		Result->Tri_MinDist_Pos3_w =        Seg_Tri2_3_Res.SegB_MinDist_Pos1_Pos2_t ;
		Result->Tri_MinDist_Pos = Seg_Tri2_3_Res.SegB_MinDist_Pos ;
		break ;

	case 2 :
		Result->Seg_MinDist_Pos1_Pos2_t = Seg_Tri3_1_Res.SegA_MinDist_Pos1_Pos2_t ;
		Result->Seg_MinDist_Pos = Seg_Tri3_1_Res.SegA_MinDist_Pos ;
		Result->Tri_MinDist_Pos1_w =        Seg_Tri3_1_Res.SegB_MinDist_Pos1_Pos2_t ;
		Result->Tri_MinDist_Pos2_w = 0.0 ;
		Result->Tri_MinDist_Pos3_w = 1.0 - Seg_Tri3_1_Res.SegB_MinDist_Pos1_Pos2_t ;
		Result->Tri_MinDist_Pos = Seg_Tri3_1_Res.SegB_MinDist_Pos ;
		break ;

	case 3 :
		Result->Seg_MinDist_Pos1_Pos2_t = 0.0 ;
		Result->Seg_MinDist_Pos = *SegmentPos1 ;
		Result->Tri_MinDist_Pos1_w = Seg1_TriPro_w1 ;
		Result->Tri_MinDist_Pos2_w = Seg1_TriPro_w2 ;
		Result->Tri_MinDist_Pos3_w = Seg1_TriPro_w3 ;
		Result->Tri_MinDist_Pos = Seg1_TriPro ;
		break ;

	case 4 :
		Result->Seg_MinDist_Pos1_Pos2_t = 1.0 ;
		Result->Seg_MinDist_Pos = *SegmentPos2 ;
		Result->Tri_MinDist_Pos1_w = Seg2_TriPro_w1 ;
		Result->Tri_MinDist_Pos2_w = Seg2_TriPro_w2 ;
		Result->Tri_MinDist_Pos3_w = Seg2_TriPro_w3 ;
		Result->Tri_MinDist_Pos = Seg2_TriPro ;
		break ;
	}

	return 0 ;
}

// �O�p�`�Ɠ_�̍Őڋߓ_������͂���
extern int Triangle_Point_Analyse( const VECTOR *TrianglePos1, const VECTOR *TrianglePos2, const VECTOR *TrianglePos3, const VECTOR *PointPos, TRIANGLE_POINT_RESULT *Result )
{
	VECTOR Tri1_2 ;
	VECTOR Tri2_3 ;
	VECTOR Tri3_1 ;
	VECTOR Tri1_Pnt ;
	VECTOR Tri2_Pnt ;
	VECTOR Tri3_Pnt ;
	float Dot1P2 ;
	float Dot1P3 ;
	float Dot2P1 ;
	float Dot2P3 ;
	float Dot2PH ;
	float Dot3P1 ;
	float Dot3P2 ;
	float Dot3PH ;
	float OPA ;
	float OPB ;
	float OPC ;
	float Div ;
	float t ;
	float v ;
	float w ;

	VectorSub( &Tri1_2,   TrianglePos2, TrianglePos1 ) ;
	VectorSub( &Tri3_1,   TrianglePos1, TrianglePos3 ) ;
	VectorSub( &Tri1_Pnt, PointPos,     TrianglePos1 ) ;
	Dot1P2 = VectorInnerProduct( &Tri1_2, &Tri1_Pnt ) ;
	Dot1P3 = VectorInnerProduct( &Tri3_1, &Tri1_Pnt ) ;
	if( Dot1P2 <= 0.0f && Dot1P3 >= 0.0f )
	{
		Result->Tri_Pnt_MinDist_Square = VSquareSize( VSub( *TrianglePos1, *PointPos ) ) ;
		Result->Tri_MinDist_Pos1_w = 1.0f ;
		Result->Tri_MinDist_Pos2_w = 0.0f ;
		Result->Tri_MinDist_Pos3_w = 0.0f ;
		Result->Tri_MinDist_Pos = *TrianglePos1 ;
		return 0 ;
	}

	VectorSub( &Tri2_3,   TrianglePos3, TrianglePos2 ) ;
	VectorSub( &Tri2_Pnt, PointPos,     TrianglePos2 ) ;
	Dot2P1 = VectorInnerProduct( &Tri1_2, &Tri2_Pnt ) ;
	Dot2P3 = VectorInnerProduct( &Tri2_3, &Tri2_Pnt ) ;
	if( Dot2P1 >= 0.0f && Dot2P3 <= 0.0f )
	{
		Result->Tri_Pnt_MinDist_Square = VSquareSize( VSub( *TrianglePos2, *PointPos ) ) ;
		Result->Tri_MinDist_Pos1_w = 0.0f ;
		Result->Tri_MinDist_Pos2_w = 1.0f ;
		Result->Tri_MinDist_Pos3_w = 0.0f ;
		Result->Tri_MinDist_Pos = *TrianglePos2 ;
		return 0 ;
	}

	Dot2PH = VectorInnerProduct( &Tri3_1, &Tri2_Pnt ) ;
	OPC = Dot1P2 * -Dot2PH - Dot2P1 * -Dot1P3 ;	// �����O�����W���P����
	if( OPC <= 0.0f && Dot1P2 >= 0.0f && Dot2P1 <= 0.0f )
	{
		t = Dot1P2 / ( Dot1P2 - Dot2P1 ) ;
		Result->Tri_MinDist_Pos.x = TrianglePos1->x + Tri1_2.x * t ;
		Result->Tri_MinDist_Pos.y = TrianglePos1->y + Tri1_2.y * t ;
		Result->Tri_MinDist_Pos.z = TrianglePos1->z + Tri1_2.z * t ;
		Result->Tri_MinDist_Pos1_w = 1.0f - t ;
		Result->Tri_MinDist_Pos2_w = t ;
		Result->Tri_MinDist_Pos3_w = 0.0f ;
		Result->Tri_Pnt_MinDist_Square = VSquareSize( VSub( Result->Tri_MinDist_Pos, *PointPos ) ) ;
		return 0 ;
	}

	VectorSub( &Tri3_Pnt, PointPos,        TrianglePos3 ) ;
	Dot3P1 = VectorInnerProduct( &Tri3_1, &Tri3_Pnt ) ;
	Dot3P2 = VectorInnerProduct( &Tri2_3, &Tri3_Pnt ) ;
	if( Dot3P1 <= 0.0f && Dot3P2 >= 0.0f )
	{
		Result->Tri_Pnt_MinDist_Square = VSquareSize( VSub( *TrianglePos3, *PointPos ) ) ;
		Result->Tri_MinDist_Pos1_w = 0.0f ;
		Result->Tri_MinDist_Pos2_w = 0.0f ;
		Result->Tri_MinDist_Pos3_w = 1.0f ;
		Result->Tri_MinDist_Pos = *TrianglePos3 ;
		return 0 ;
	}

	Dot3PH = VectorInnerProduct( &Tri1_2, &Tri3_Pnt ) ;
	OPB = Dot3PH * -Dot1P3 - Dot1P2 * -Dot3P1 ;	// �����O�����W���P����
	if( OPB <= 0.0f && Dot1P3 <= 0.0f && Dot3P1 >= 0.0f )
	{
		t = Dot3P1 / ( Dot3P1 - Dot1P3 ) ;
		Result->Tri_MinDist_Pos.x = TrianglePos3->x + Tri3_1.x * t ;
		Result->Tri_MinDist_Pos.y = TrianglePos3->y + Tri3_1.y * t ;
		Result->Tri_MinDist_Pos.z = TrianglePos3->z + Tri3_1.z * t ;
		Result->Tri_MinDist_Pos1_w = t ;
		Result->Tri_MinDist_Pos2_w = 0 ;
		Result->Tri_MinDist_Pos3_w = 1.0f - t ;
		Result->Tri_Pnt_MinDist_Square = VSquareSize( VSub( Result->Tri_MinDist_Pos, *PointPos ) ) ;
		return 0 ;
	}

	OPA = Dot2P1 * -Dot3P1 - Dot3PH * -Dot2PH ;	// �����O�����W���P����
	if( OPA <= 0.0f && ( -Dot2PH - Dot2P1 ) >= 0.0f && ( Dot3PH + Dot3P1 ) >= 0.0f )
	{
		t = ( -Dot2PH - Dot2P1 ) / ( ( -Dot2PH - Dot2P1 ) + ( Dot3PH + Dot3P1 ) ) ;
		Result->Tri_MinDist_Pos.x = TrianglePos2->x + Tri2_3.x * t ;
		Result->Tri_MinDist_Pos.y = TrianglePos2->y + Tri2_3.y * t ;
		Result->Tri_MinDist_Pos.z = TrianglePos2->z + Tri2_3.z * t ;
		Result->Tri_MinDist_Pos1_w = 0.0f ;
		Result->Tri_MinDist_Pos2_w = 1.0f - t ;
		Result->Tri_MinDist_Pos3_w = t ;
		Result->Tri_Pnt_MinDist_Square = VSquareSize( VSub( Result->Tri_MinDist_Pos, *PointPos ) ) ;
		return 0 ;
	}

	Div = 1.0f / ( OPA + OPB + OPC ) ;
	v = OPB * Div ;
	w = OPC * Div ;
	Result->Tri_MinDist_Pos.x = TrianglePos1->x + Tri1_2.x * v - Tri3_1.x * w ;
	Result->Tri_MinDist_Pos.y = TrianglePos1->y + Tri1_2.y * v - Tri3_1.y * w ;
	Result->Tri_MinDist_Pos.z = TrianglePos1->z + Tri1_2.z * v - Tri3_1.z * w ;
	Result->Tri_MinDist_Pos2_w = v ;
	Result->Tri_MinDist_Pos3_w = w ;
	Result->Tri_MinDist_Pos1_w = 1.0f - v - w ;
	Result->Tri_Pnt_MinDist_Square = VSquareSize( VSub( Result->Tri_MinDist_Pos, *PointPos ) ) ;
	return 0 ;
}

// �O�p�`�Ɠ_�̍Őڋߓ_������͂���
extern int Triangle_Point_AnalyseD( const VECTOR_D *TrianglePos1, const VECTOR_D *TrianglePos2, const VECTOR_D *TrianglePos3, const VECTOR_D *PointPos, TRIANGLE_POINT_RESULT_D *Result )
{
	VECTOR_D Tri1_2 ;
	VECTOR_D Tri2_3 ;
	VECTOR_D Tri3_1 ;
	VECTOR_D Tri1_Pnt ;
	VECTOR_D Tri2_Pnt ;
	VECTOR_D Tri3_Pnt ;
	double Dot1P2 ;
	double Dot1P3 ;
	double Dot2P1 ;
	double Dot2P3 ;
	double Dot2PH ;
	double Dot3P1 ;
	double Dot3P2 ;
	double Dot3PH ;
	double OPA ;
	double OPB ;
	double OPC ;
	double Div ;
	double t ;
	double v ;
	double w ;

	VectorSubD( &Tri1_2,   TrianglePos2, TrianglePos1 ) ;
	VectorSubD( &Tri3_1,   TrianglePos1, TrianglePos3 ) ;
	VectorSubD( &Tri1_Pnt, PointPos,     TrianglePos1 ) ;
	Dot1P2 = VectorInnerProductD( &Tri1_2, &Tri1_Pnt ) ;
	Dot1P3 = VectorInnerProductD( &Tri3_1, &Tri1_Pnt ) ;
	if( Dot1P2 <= 0.0 && Dot1P3 >= 0.0 )
	{
		Result->Tri_Pnt_MinDist_Square = VSquareSizeD( VSubD( *TrianglePos1, *PointPos ) ) ;
		Result->Tri_MinDist_Pos1_w = 1.0 ;
		Result->Tri_MinDist_Pos2_w = 0.0 ;
		Result->Tri_MinDist_Pos3_w = 0.0 ;
		Result->Tri_MinDist_Pos = *TrianglePos1 ;
		return 0 ;
	}

	VectorSubD( &Tri2_3,   TrianglePos3, TrianglePos2 ) ;
	VectorSubD( &Tri2_Pnt, PointPos,     TrianglePos2 ) ;
	Dot2P1 = VectorInnerProductD( &Tri1_2, &Tri2_Pnt ) ;
	Dot2P3 = VectorInnerProductD( &Tri2_3, &Tri2_Pnt ) ;
	if( Dot2P1 >= 0.0 && Dot2P3 <= 0.0 )
	{
		Result->Tri_Pnt_MinDist_Square = VSquareSizeD( VSubD( *TrianglePos2, *PointPos ) ) ;
		Result->Tri_MinDist_Pos1_w = 0.0 ;
		Result->Tri_MinDist_Pos2_w = 1.0 ;
		Result->Tri_MinDist_Pos3_w = 0.0 ;
		Result->Tri_MinDist_Pos = *TrianglePos2 ;
		return 0 ;
	}

	Dot2PH = VectorInnerProductD( &Tri3_1, &Tri2_Pnt ) ;
	OPC = Dot1P2 * -Dot2PH - Dot2P1 * -Dot1P3 ;	// �����O�����W���P����
	if( OPC <= 0.0 && Dot1P2 >= 0.0 && Dot2P1 <= 0.0 )
	{
		t = Dot1P2 / ( Dot1P2 - Dot2P1 ) ;
		Result->Tri_MinDist_Pos.x = TrianglePos1->x + Tri1_2.x * t ;
		Result->Tri_MinDist_Pos.y = TrianglePos1->y + Tri1_2.y * t ;
		Result->Tri_MinDist_Pos.z = TrianglePos1->z + Tri1_2.z * t ;
		Result->Tri_MinDist_Pos1_w = 1.0 - t ;
		Result->Tri_MinDist_Pos2_w = t ;
		Result->Tri_MinDist_Pos3_w = 0.0 ;
		Result->Tri_Pnt_MinDist_Square = VSquareSizeD( VSubD( Result->Tri_MinDist_Pos, *PointPos ) ) ;
		return 0 ;
	}

	VectorSubD( &Tri3_Pnt, PointPos,        TrianglePos3 ) ;
	Dot3P1 = VectorInnerProductD( &Tri3_1, &Tri3_Pnt ) ;
	Dot3P2 = VectorInnerProductD( &Tri2_3, &Tri3_Pnt ) ;
	if( Dot3P1 <= 0.0 && Dot3P2 >= 0.0 )
	{
		Result->Tri_Pnt_MinDist_Square = VSquareSizeD( VSubD( *TrianglePos3, *PointPos ) ) ;
		Result->Tri_MinDist_Pos1_w = 0.0 ;
		Result->Tri_MinDist_Pos2_w = 0.0 ;
		Result->Tri_MinDist_Pos3_w = 1.0 ;
		Result->Tri_MinDist_Pos = *TrianglePos3 ;
		return 0 ;
	}

	Dot3PH = VectorInnerProductD( &Tri1_2, &Tri3_Pnt ) ;
	OPB = Dot3PH * -Dot1P3 - Dot1P2 * -Dot3P1 ;	// �����O�����W���P����
	if( OPB <= 0.0 && Dot1P3 <= 0.0 && Dot3P1 >= 0.0 )
	{
		t = Dot3P1 / ( Dot3P1 - Dot1P3 ) ;
		Result->Tri_MinDist_Pos.x = TrianglePos3->x + Tri3_1.x * t ;
		Result->Tri_MinDist_Pos.y = TrianglePos3->y + Tri3_1.y * t ;
		Result->Tri_MinDist_Pos.z = TrianglePos3->z + Tri3_1.z * t ;
		Result->Tri_MinDist_Pos1_w = t ;
		Result->Tri_MinDist_Pos2_w = 0 ;
		Result->Tri_MinDist_Pos3_w = 1.0 - t ;
		Result->Tri_Pnt_MinDist_Square = VSquareSizeD( VSubD( Result->Tri_MinDist_Pos, *PointPos ) ) ;
		return 0 ;
	}

	OPA = Dot2P1 * -Dot3P1 - Dot3PH * -Dot2PH ;	// �����O�����W���P����
	if( OPA <= 0.0 && ( -Dot2PH - Dot2P1 ) >= 0.0 && ( Dot3PH + Dot3P1 ) >= 0.0 )
	{
		t = ( -Dot2PH - Dot2P1 ) / ( ( -Dot2PH - Dot2P1 ) + ( Dot3PH + Dot3P1 ) ) ;
		Result->Tri_MinDist_Pos.x = TrianglePos2->x + Tri2_3.x * t ;
		Result->Tri_MinDist_Pos.y = TrianglePos2->y + Tri2_3.y * t ;
		Result->Tri_MinDist_Pos.z = TrianglePos2->z + Tri2_3.z * t ;
		Result->Tri_MinDist_Pos1_w = 0.0 ;
		Result->Tri_MinDist_Pos2_w = 1.0 - t ;
		Result->Tri_MinDist_Pos3_w = t ;
		Result->Tri_Pnt_MinDist_Square = VSquareSizeD( VSubD( Result->Tri_MinDist_Pos, *PointPos ) ) ;
		return 0 ;
	}

	Div = 1.0 / ( OPA + OPB + OPC ) ;
	v = OPB * Div ;
	w = OPC * Div ;
	Result->Tri_MinDist_Pos.x = TrianglePos1->x + Tri1_2.x * v - Tri3_1.x * w ;
	Result->Tri_MinDist_Pos.y = TrianglePos1->y + Tri1_2.y * v - Tri3_1.y * w ;
	Result->Tri_MinDist_Pos.z = TrianglePos1->z + Tri1_2.z * v - Tri3_1.z * w ;
	Result->Tri_MinDist_Pos2_w = v ;
	Result->Tri_MinDist_Pos3_w = w ;
	Result->Tri_MinDist_Pos1_w = 1.0 - v - w ;
	Result->Tri_Pnt_MinDist_Square = VSquareSizeD( VSubD( Result->Tri_MinDist_Pos, *PointPos ) ) ;
	return 0 ;
}

// ���ʂƓ_�̍ŋߓ_������͂���
extern int Plane_Point_Analyse( const VECTOR *PlanePos, const VECTOR *PlaneNormal, const VECTOR *PointPos, PLANE_POINT_RESULT *Result )
{
	float t ;
	VECTOR Plane_Pnt ;
	float DotPlaneNormal_Plane_Pnt ;

	VectorSub( &Plane_Pnt, PointPos, PlanePos ) ;
	DotPlaneNormal_Plane_Pnt = VDot( *PlaneNormal, Plane_Pnt ) ;

	Result->Pnt_Plane_Normal_Side = DotPlaneNormal_Plane_Pnt >= 0.0f ? 1 : 0 ;

	t = DotPlaneNormal_Plane_Pnt / VDot( *PlaneNormal, *PlaneNormal ) ;
	Result->Plane_MinDist_Pos = VSub( *PointPos, VScale( *PlaneNormal, t ) ) ;
	Result->Plane_Pnt_MinDist_Square = VSquareSize( VScale( *PlaneNormal, t ) ) ;

	return 0 ;
}

// ���ʂƓ_�̍ŋߓ_������͂���
extern int Plane_Point_AnalyseD( const VECTOR_D *PlanePos, const VECTOR_D *PlaneNormal, const VECTOR_D *PointPos, PLANE_POINT_RESULT_D *Result )
{
	double t ;
	VECTOR_D Plane_Pnt ;
	double DotPlaneNormal_Plane_Pnt ;

	VectorSubD( &Plane_Pnt, PointPos, PlanePos ) ;
	DotPlaneNormal_Plane_Pnt = VDotD( *PlaneNormal, Plane_Pnt ) ;

	Result->Pnt_Plane_Normal_Side = DotPlaneNormal_Plane_Pnt >= 0.0 ? 1 : 0 ;

	t = DotPlaneNormal_Plane_Pnt / VDotD( *PlaneNormal, *PlaneNormal ) ;
	Result->Plane_MinDist_Pos = VSubD( *PointPos, VScaleD( *PlaneNormal, t ) ) ;
	Result->Plane_Pnt_MinDist_Square = VSquareSizeD( VScaleD( *PlaneNormal, t ) ) ;

	return 0 ;
}

// �w��̍��W����O�p�`�̏d�S�����߂�
extern void TriangleBarycenter( VECTOR TrianglePos1, VECTOR TrianglePos2, VECTOR TrianglePos3, VECTOR Position, float *TrianglePos1Weight, float *TrianglePos2Weight, float *TrianglePos3Weight )
{
	TriangleBarycenter_Base( &TrianglePos1, &TrianglePos2, &TrianglePos3, &Position, TrianglePos1Weight, TrianglePos2Weight, TrianglePos3Weight ) ;
}

// �w��̍��W����O�p�`�̏d�S�����߂�
extern void TriangleBarycenterD( VECTOR_D TrianglePos1, VECTOR_D TrianglePos2, VECTOR_D TrianglePos3, VECTOR_D Position, double *TrianglePos1Weight, double *TrianglePos2Weight, double *TrianglePos3Weight )
{
	TriangleBarycenter_BaseD( &TrianglePos1, &TrianglePos2, &TrianglePos3, &Position, TrianglePos1Weight, TrianglePos2Weight, TrianglePos3Weight ) ;
}

// �w��̍��W����O�p�`�̏d�S�����߂�
extern void TriangleBarycenter_Base( const VECTOR *TrianglePos1, const VECTOR *TrianglePos2, const VECTOR *TrianglePos3, const VECTOR *Position, float *TrianglePos1Weight, float *TrianglePos2Weight, float *TrianglePos3Weight )
{
	VECTOR v1, v2, v3 ;
	float d11, d12, d22, d13, d23, divn ;

	VectorSub( &v1, TrianglePos2, TrianglePos1 ) ;
	VectorSub( &v2, TrianglePos3, TrianglePos1 ) ;
	VectorSub( &v3, Position, TrianglePos1 ) ;

	d11 = VectorInnerProduct( &v1, &v1 ) ;
	d12 = VectorInnerProduct( &v2, &v1 ) ;
	d22 = VectorInnerProduct( &v2, &v2 ) ;
	d13 = VectorInnerProduct( &v1, &v3 ) ;
	d23 = VectorInnerProduct( &v2, &v3 ) ;
	divn = d11 * d22 - d12 * d12 ;
	*TrianglePos2Weight = ( d13 * d22 - d12 * d23 ) / divn ;
	*TrianglePos3Weight = ( d11 * d23 - d13 * d12 ) / divn ;
	*TrianglePos1Weight = 1.0f - *TrianglePos2Weight - *TrianglePos3Weight ;
}

// �w��̍��W����O�p�`�̏d�S�����߂�
extern void TriangleBarycenter_BaseD( const VECTOR_D *TrianglePos1, const VECTOR_D *TrianglePos2, const VECTOR_D *TrianglePos3, const VECTOR_D *Position, double *TrianglePos1Weight, double *TrianglePos2Weight, double *TrianglePos3Weight )
{
	VECTOR_D v1, v2, v3 ;
	double d11, d12, d22, d13, d23, divn ;

	VectorSubD( &v1, TrianglePos2, TrianglePos1 ) ;
	VectorSubD( &v2, TrianglePos3, TrianglePos1 ) ;
	VectorSubD( &v3, Position, TrianglePos1 ) ;

	d11 = VectorInnerProductD( &v1, &v1 ) ;
	d12 = VectorInnerProductD( &v2, &v1 ) ;
	d22 = VectorInnerProductD( &v2, &v2 ) ;
	d13 = VectorInnerProductD( &v1, &v3 ) ;
	d23 = VectorInnerProductD( &v2, &v3 ) ;
	divn = d11 * d22 - d12 * d12 ;
	*TrianglePos2Weight = ( d13 * d22 - d12 * d23 ) / divn ;
	*TrianglePos3Weight = ( d11 * d23 - d13 * d12 ) / divn ;
	*TrianglePos1Weight = 1.0f - *TrianglePos2Weight - *TrianglePos3Weight ;
}

// ��̐����̍ŋߓ_�Ԃ̋����𓾂�
extern float Segment_Segment_MinLength( VECTOR SA1, VECTOR SA2, VECTOR SB1, VECTOR SB2 )
{
	return _SQRT( Segment_Segment_MinLength_Square( SA1, SA2, SB1, SB2 ) ) ;
}

// ��̐����̍ŋߓ_�Ԃ̋����𓾂�
extern double Segment_Segment_MinLengthD( VECTOR_D SA1, VECTOR_D SA2, VECTOR_D SB1, VECTOR_D SB2 )
{
	return _SQRTD( Segment_Segment_MinLength_SquareD( SA1, SA2, SB1, SB2 ) ) ;
}

// ��̐����������Ƃ��߂Â��_�̋����̓��𓾂�
extern float Segment_Segment_MinLength_Square( VECTOR SA1, VECTOR SA2, VECTOR SB1, VECTOR SB2 )
{
	VECTOR da, db, p, tp ;
	VECTOR e1, e2 ;
	float a, b, c, d, e, f, w, s, t = 0.0f ;
	float tmpA, tmpB ;

	VectorSub( &da, &SA2, &SA1 ) ;
	VectorSub( &db, &SB2, &SB1 ) ;
	VectorSub( &p, &SA1, &SB1 ) ;
	a =  VectorInnerProduct( &da, &da ) ;
	b = -VectorInnerProduct( &da, &db ) ;
	c = -b ;
	d = -VectorInnerProduct( &db, &db ) ;
	e = -VectorInnerProduct( &da, &p ) ;
	f = -VectorInnerProduct( &db, &p ) ;

	// SA1 - SA2 �܂��� SB1 - SB2 �̋���������Ȃ��[���ɋ߂����ǂ����̃`�F�b�N
	tmpA = a < 0.0f ? -a : a ;
	tmpB = d < 0.0f ? -d : d ;
	if( tmpA < 0.00000001f )
	{
		if( tmpB < 0.00000001f )
		{
			return VSquareSize( VSub( SA1, SB1 ) ) ;
		}
		else
		{
			return Segment_Point_MinLength_Square( SB1, SB2, SA1 ) ;
		}
	}
	else
	if( tmpB < 0.00000001f )
	{
		return Segment_Point_MinLength_Square( SA1, SA2, SB1 ) ;
	}

	s = -0.1f ;
	w = a * d - b * c ;
	if( w <= -0.00000001f )
	{
		// �N���[�����̌���
		s = ( e * d - b * f ) / w ;
		t = ( a * f - e * c ) / w ;
	}

	// ��̐������k�ނ��Ă�����_�ƌ��Ȃ��ē_���m�̋�����Ԃ�
	if( a <= 0.0f && -d <= 0.0f )
	{
		VectorSub( &tp, &SA1, &SB1 ) ;
		return _SQRT( VectorInnerProduct( &tp, &tp ) ) ;
	}

	if( a <= 0.0f )
	{
		s = 0.0f ;
		if( t > 1.0f || t < 0.0f )
			t = t > 1.0f ? 1.0f : 0.0f ;
	}
	else
	if( -d <= 0.0f )
	{
		t = 0.0f ;
		if( s > 1.0f || s < 0.0f )
			s = s > 1.0f ? 1.0f : 0.0f ;
	}
	else
	{
		if( s > 1.0f || s < 0.0f )
		{
			s = s > 1.0f ? 1.0f : 0.0f ;

			t = ( -b * s + -f ) / -d ;		// �K�E�X�����@
			if( t > 1.0f || t < 0.0f )
			{
				t = t > 1.0f ? 1.0f : 0.0f ;
				s = ( -b * t - -e ) / a ;	// �K�E�X�����@
					 if( s > 1.0f ) s = 1.0f ;
				else if( s < 0.0f ) s = 0.0f ;
			}
		}
		else
		if( t > 1.0f || t < 0.0f )
		{
			t = t > 1.0f ? 1.0f : 0.0f ;
			s = ( -b * t - -e ) / a ;		// �K�E�X�����@

			if( s > 1.0f || s < 0.0f )
			{
				s = s > 1.0f ? 1.0f : 0.0f ;
				t = ( -b * s + -f ) / -d ;	// �K�E�X�����@
					 if( t > 1.0f ) t = 1.0f ;
				else if( t < 0.0f ) t = 0.0f ;
			}
		}
	}

//	NS_DrawLine( SA1.x, SA1.y, SA2.x, SA2.y, GetColor( 255,255,255 ), 1 ) ;
//	NS_DrawLine( SB1.x, SB1.y, SB2.x, SB2.y, GetColor( 255,0,0 ), 1 ) ;
//	NS_DrawFormatString( 0, 0, GetColor( 255,255,255 ), "s:%.3f t:%.3f", s, t ) ;

	VectorScale( &e1, &da, s ) ;
	VectorAdd( &e1, &e1, &SA1 ) ;

	VectorScale( &e2, &db, t ) ;
	VectorAdd( &e2, &e2, &SB1 ) ;

//	NS_DrawBox( e1.x - 2, e1.y - 2, e1.x + 2, e1.y + 2, GetColor( 0,255,0 ), TRUE ) ;
//	NS_DrawBox( e2.x - 2, e2.y - 2, e2.x + 2, e2.y + 2, GetColor( 255,255,0 ), TRUE ) ;
//	NS_DrawCube3D( e1.x - 10.0f, e1.y - 10.0f, e1.z - 10.0f, 
//					e1.x + 10.0f, e1.y + 10.0f, e1.z + 10.0f, GetColor( 255,0,0 ), TRUE ) ;
//	NS_DrawCube3D( e2.x - 10.0f, e2.y - 10.0f, e2.z - 10.0f, 
//					e2.x + 10.0f, e2.y + 10.0f, e2.z + 10.0f, GetColor( 255,0,255 ), TRUE ) ;

	VectorSub( &tp, &e1, &e2 ) ;
	return VectorInnerProduct( &tp, &tp ) ;
}

// ��̐����������Ƃ��߂Â��_�̋����̓��𓾂�
extern double Segment_Segment_MinLength_SquareD( VECTOR_D SA1, VECTOR_D SA2, VECTOR_D SB1, VECTOR_D SB2 )
{
	VECTOR_D da, db, p, tp ;
	VECTOR_D e1, e2 ;
	double a, b, c, d, e, f, w, s, t = 0.0 ;
	double tmpA, tmpB ;

	VectorSubD( &da, &SA2, &SA1 ) ;
	VectorSubD( &db, &SB2, &SB1 ) ;
	VectorSubD( &p, &SA1, &SB1 ) ;
	a =  VectorInnerProductD( &da, &da ) ;
	b = -VectorInnerProductD( &da, &db ) ;
	c = -b ;
	d = -VectorInnerProductD( &db, &db ) ;
	e = -VectorInnerProductD( &da, &p ) ;
	f = -VectorInnerProductD( &db, &p ) ;

	// SA1 - SA2 �܂��� SB1 - SB2 �̋���������Ȃ��[���ɋ߂����ǂ����̃`�F�b�N
	tmpA = a < 0.0 ? -a : a ;
	tmpB = d < 0.0 ? -d : d ;
	if( tmpA < 0.00000001 )
	{
		if( tmpB < 0.00000001 )
		{
			return VSquareSizeD( VSubD( SA1, SB1 ) ) ;
		}
		else
		{
			return Segment_Point_MinLength_SquareD( SB1, SB2, SA1 ) ;
		}
	}
	else
	if( tmpB < 0.00000001 )
	{
		return Segment_Point_MinLength_SquareD( SA1, SA2, SB1 ) ;
	}

	s = -0.1 ;
	w = a * d - b * c ;
	if( w <= -0.00000001 )
	{
		// �N���[�����̌���
		s = ( e * d - b * f ) / w ;
		t = ( a * f - e * c ) / w ;
	}

	// ��̐������k�ނ��Ă�����_�ƌ��Ȃ��ē_���m�̋�����Ԃ�
	if( a <= 0.0 && -d <= 0.0 )
	{
		VectorSubD( &tp, &SA1, &SB1 ) ;
		return _SQRTD( VectorInnerProductD( &tp, &tp ) ) ;
	}

	if( a <= 0.0 )
	{
		s = 0.0 ;
		if( t > 1.0 || t < 0.0 )
			t = t > 1.0 ? 1.0 : 0.0 ;
	}
	else
	if( -d <= 0.0 )
	{
		t = 0.0 ;
		if( s > 1.0 || s < 0.0 )
			s = s > 1.0 ? 1.0 : 0.0 ;
	}
	else
	{
		if( s > 1.0 || s < 0.0 )
		{
			s = s > 1.0 ? 1.0 : 0.0 ;

			t = ( -b * s + -f ) / -d ;		// �K�E�X�����@
			if( t > 1.0 || t < 0.0 )
			{
				t = t > 1.0 ? 1.0 : 0.0 ;
				s = ( -b * t - -e ) / a ;	// �K�E�X�����@
					 if( s > 1.0 ) s = 1.0 ;
				else if( s < 0.0 ) s = 0.0 ;
			}
		}
		else
		if( t > 1.0 || t < 0.0 )
		{
			t = t > 1.0 ? 1.0 : 0.0 ;
			s = ( -b * t - -e ) / a ;		// �K�E�X�����@

			if( s > 1.0 || s < 0.0 )
			{
				s = s > 1.0 ? 1.0 : 0.0 ;
				t = ( -b * s + -f ) / -d ;	// �K�E�X�����@
					 if( t > 1.0 ) t = 1.0 ;
				else if( t < 0.0 ) t = 0.0 ;
			}
		}
	}

	VectorScaleD( &e1, &da, s ) ;
	VectorAddD( &e1, &e1, &SA1 ) ;

	VectorScaleD( &e2, &db, t ) ;
	VectorAddD( &e2, &e2, &SB1 ) ;

	VectorSubD( &tp, &e1, &e2 ) ;
	return VectorInnerProductD( &tp, &tp ) ;
}

// �����ƎO�p�`�̍ŋߓ_�Ԃ̋����𓾂� 
extern float Segment_Triangle_MinLength( VECTOR SegmentPos1, VECTOR SegmentPos2, VECTOR TrianglePos1, VECTOR TrianglePos2, VECTOR TrianglePos3 )
{
	return _SQRT( Segment_Triangle_MinLength_Square(  SegmentPos1,  SegmentPos2,  TrianglePos1,  TrianglePos2,  TrianglePos3 ) ) ;
}

// �����ƎO�p�`�̍ŋߓ_�Ԃ̋����𓾂� 
extern double Segment_Triangle_MinLengthD( VECTOR_D SegmentPos1, VECTOR_D SegmentPos2, VECTOR_D TrianglePos1, VECTOR_D TrianglePos2, VECTOR_D TrianglePos3 )
{
	return _SQRTD( Segment_Triangle_MinLength_SquareD(  SegmentPos1,  SegmentPos2,  TrianglePos1,  TrianglePos2,  TrianglePos3 ) ) ;
}

// �����ƎO�p�`�̍ŋߓ_�Ԃ̋����̓��𓾂� 
extern float Segment_Triangle_MinLength_Square( VECTOR SegmentPos1, VECTOR SegmentPos2, VECTOR TrianglePos1, VECTOR TrianglePos2, VECTOR TrianglePos3 )
{
	bool Touei1, Touei2 ;
	VECTOR st1, st2 ;
	VECTOR stw1, stw2 ;
	VECTOR tv, tnorm ;
	VECTOR v12, v13 ;
	float st1u, st1v, st1w ;
	float st2u, st2v, st2w ;
	float l1 = 0.0f, l2 = 0.0f ;
	float l12, l23, l31 ;
	float MinLen ;
	HITRESULT_LINE HitResult ;

	// �������O�p�`���т��Ă����狗���͂O
	HitResult = HitCheck_Line_Triangle( SegmentPos1, SegmentPos2, TrianglePos1, TrianglePos2, TrianglePos3 ) ;
	if( HitResult.HitFlag )
	{
		return 0.0f ;
	}

	VectorSub( &v12, &TrianglePos2, &TrianglePos1 ) ;
	VectorSub( &v13, &TrianglePos3, &TrianglePos1 ) ;
	VectorOuterProduct( &tnorm, &v12, &v13 ) ;
	VectorNormalize( &tnorm, &tnorm ) ;

	// �O�p�`���������ʏ�ɒ��_�𓊉e����
	VectorSub( &tv, &SegmentPos1, &TrianglePos1 ) ;
	stw1 = tv ;
	VectorScale( &tv, &tnorm, VectorInnerProduct( &tv, &tnorm ) ) ;
	VectorSub( &st1, &SegmentPos1, &tv ) ;

//	NS_DrawCube3D( st1.x - 10.0f, st1.y - 10.0f, st1.z - 10.0f, 
//					st1.x + 10.0f, st1.y + 10.0f, st1.z + 10.0f, GetColor( 0,255,255 ), TRUE ) ;

	// �O�p�`���ɒ��_�����݂��邩�ǂ����𒲂ׂ�
	TriangleBarycenter_Base( &TrianglePos1, &TrianglePos2, &TrianglePos3, &st1, &st1u, &st1v, &st1w ) ;
	Touei1 = !( st1u <= 0.0f || st1u >= 1.0f || st1v <= 0.0f || st1v >= 1.0f || st1w <= 0.0f || st1w >= 1.0f ) ;
	if( Touei1 )
	{
		VectorSub( &tv, &SegmentPos1, &st1 ) ;
		l1 = VectorInnerProduct( &tv, &tv ) ;
	}

	// �O�p�`���������ʏ�ɒ��_�𓊉e����
	VectorSub( &tv, &SegmentPos2, &TrianglePos1 ) ;
	stw2 = tv ;
	VectorScale( &tv, &tnorm, VectorInnerProduct( &tv, &tnorm ) ) ;
	VectorSub( &st2, &SegmentPos2, &tv ) ;

	// �O�p�`���ɒ��_�����݂��邩�ǂ����𒲂ׂ�
	TriangleBarycenter_Base( &TrianglePos1, &TrianglePos2, &TrianglePos3, &st2, &st2u, &st2v, &st2w ) ;
	Touei2 = !( st2u <= 0.0f || st2u >= 1.0f || st2v <= 0.0f || st2v >= 1.0f || st2w <= 0.0f || st2w >= 1.0f ) ;
	if( Touei2 )
	{
		VectorSub( &tv, &SegmentPos2, &st2 ) ;
		l2 = VectorInnerProduct( &tv, &tv ) ;
	}

	// �ǂ�����O�p�`�͈̔͂ɂ������ꍇ
	if( Touei1 && Touei2 )
	{
		float i1, i2 ;

		// �����̊e���_���O�p�`�̑O��ɂ��邩����
		i1 = VectorInnerProduct( &tnorm, &stw1 ) ;
		i2 = VectorInnerProduct( &tnorm, &stw2 ) ;
		if( ( i1 < 0.0f && i2 < 0.0f ) || ( i1 >= 0.0f && i2 >= 0.0f ) )
		{
			// �Е��̑��ɂǂ���̒��_������ꍇ�́A���߂��ق��̒��_�ƕ��ʂƂ̋��������ʂɂ���
			if( l1 < l2 )
			{
				return l1 ;
			}
			else
			{
				return l2 ;
			}
		}
		else
		{
			// �e���_���O��ɂ���ꍇ�͐ڂ��Ă���Ƃ�������
			return 0.0f ;
		}
	}

	// �����ł͂Ȃ��ꍇ�͎O�p�`�̂R�ӂƐ����̋����𑪂�A��ԋ������Z���ӂ�T��
	l12 = Segment_Segment_MinLength_Square( SegmentPos1, SegmentPos2, TrianglePos1, TrianglePos2 ) ;
	l23 = Segment_Segment_MinLength_Square( SegmentPos1, SegmentPos2, TrianglePos2, TrianglePos3 ) ;
	l31 = Segment_Segment_MinLength_Square( SegmentPos1, SegmentPos2, TrianglePos3, TrianglePos1 ) ;

	MinLen = l12 ;
	if( l23 < MinLen )
	{
		MinLen = l23 ;
	}
	if( l31 < MinLen )
	{
		MinLen = l31 ;
	}
	if( Touei1 && l1 < MinLen )
	{
		MinLen = l1 ;
	}
	if( Touei2 && l2 < MinLen )
	{
		MinLen = l2 ;
	}

	return MinLen ;
}

// �����ƎO�p�`�̍ŋߓ_�Ԃ̋����̓��𓾂� 
extern double Segment_Triangle_MinLength_SquareD( VECTOR_D SegmentPos1, VECTOR_D SegmentPos2, VECTOR_D TrianglePos1, VECTOR_D TrianglePos2, VECTOR_D TrianglePos3 )
{
	bool Touei1, Touei2 ;
	VECTOR_D st1, st2 ;
	VECTOR_D stw1, stw2 ;
	VECTOR_D tv, tnorm ;
	VECTOR_D v12, v13 ;
	double st1u, st1v, st1w ;
	double st2u, st2v, st2w ;
	double l1 = 0.0, l2 = 0.0 ;
	double l12, l23, l31 ;
	double MinLen ;
	HITRESULT_LINE_D HitResult ;

	// �������O�p�`���т��Ă����狗���͂O
	HitResult = HitCheck_Line_TriangleD( SegmentPos1, SegmentPos2, TrianglePos1, TrianglePos2, TrianglePos3 ) ;
	if( HitResult.HitFlag )
	{
		return 0.0 ;
	}

	VectorSubD( &v12, &TrianglePos2, &TrianglePos1 ) ;
	VectorSubD( &v13, &TrianglePos3, &TrianglePos1 ) ;
	VectorOuterProductD( &tnorm, &v12, &v13 ) ;
	VectorNormalizeD( &tnorm, &tnorm ) ;

	// �O�p�`���������ʏ�ɒ��_�𓊉e����
	VectorSubD( &tv, &SegmentPos1, &TrianglePos1 ) ;
	stw1 = tv ;
	VectorScaleD( &tv, &tnorm, VectorInnerProductD( &tv, &tnorm ) ) ;
	VectorSubD( &st1, &SegmentPos1, &tv ) ;

	// �O�p�`���ɒ��_�����݂��邩�ǂ����𒲂ׂ�
	TriangleBarycenter_BaseD( &TrianglePos1, &TrianglePos2, &TrianglePos3, &st1, &st1u, &st1v, &st1w ) ;
	Touei1 = !( st1u <= 0.0 || st1u >= 1.0 || st1v <= 0.0 || st1v >= 1.0 || st1w <= 0.0 || st1w >= 1.0 ) ;
	if( Touei1 )
	{
		VectorSubD( &tv, &SegmentPos1, &st1 ) ;
		l1 = VectorInnerProductD( &tv, &tv ) ;
	}

	// �O�p�`���������ʏ�ɒ��_�𓊉e����
	VectorSubD( &tv, &SegmentPos2, &TrianglePos1 ) ;
	stw2 = tv ;
	VectorScaleD( &tv, &tnorm, VectorInnerProductD( &tv, &tnorm ) ) ;
	VectorSubD( &st2, &SegmentPos2, &tv ) ;

	// �O�p�`���ɒ��_�����݂��邩�ǂ����𒲂ׂ�
	TriangleBarycenter_BaseD( &TrianglePos1, &TrianglePos2, &TrianglePos3, &st2, &st2u, &st2v, &st2w ) ;
	Touei2 = !( st2u <= 0.0 || st2u >= 1.0 || st2v <= 0.0 || st2v >= 1.0 || st2w <= 0.0 || st2w >= 1.0 ) ;
	if( Touei2 )
	{
		VectorSubD( &tv, &SegmentPos2, &st2 ) ;
		l2 = VectorInnerProductD( &tv, &tv ) ;
	}

	// �ǂ�����O�p�`�͈̔͂ɂ������ꍇ
	if( Touei1 && Touei2 )
	{
		double i1, i2 ;

		// �����̊e���_���O�p�`�̑O��ɂ��邩����
		i1 = VectorInnerProductD( &tnorm, &stw1 ) ;
		i2 = VectorInnerProductD( &tnorm, &stw2 ) ;
		if( ( i1 < 0.0 && i2 < 0.0 ) || ( i1 >= 0.0 && i2 >= 0.0 ) )
		{
			// �Е��̑��ɂǂ���̒��_������ꍇ�́A���߂��ق��̒��_�ƕ��ʂƂ̋��������ʂɂ���
			if( l1 < l2 )
			{
				return l1 ;
			}
			else
			{
				return l2 ;
			}
		}
		else
		{
			// �e���_���O��ɂ���ꍇ�͐ڂ��Ă���Ƃ�������
			return 0.0 ;
		}
	}

	// �����ł͂Ȃ��ꍇ�͎O�p�`�̂R�ӂƐ����̋����𑪂�A��ԋ������Z���ӂ�T��
	l12 = Segment_Segment_MinLength_SquareD( SegmentPos1, SegmentPos2, TrianglePos1, TrianglePos2 ) ;
	l23 = Segment_Segment_MinLength_SquareD( SegmentPos1, SegmentPos2, TrianglePos2, TrianglePos3 ) ;
	l31 = Segment_Segment_MinLength_SquareD( SegmentPos1, SegmentPos2, TrianglePos3, TrianglePos1 ) ;

	MinLen = l12 ;
	if( l23 < MinLen )
	{
		MinLen = l23 ;
	}
	if( l31 < MinLen )
	{
		MinLen = l31 ;
	}
	if( Touei1 && l1 < MinLen )
	{
		MinLen = l1 ;
	}
	if( Touei2 && l2 < MinLen )
	{
		MinLen = l2 ;
	}

	return MinLen ;
}

// �����Ɠ_�̈�ԋ߂������𓾂�
extern float Segment_Point_MinLength( VECTOR SegmentPos1, VECTOR SegmentPos2, VECTOR PointPos )
{
	return _SQRT( Segment_Point_MinLength_Square(  SegmentPos1,  SegmentPos2,  PointPos ) ) ;
}

// �����Ɠ_�̈�ԋ߂������𓾂�
extern double Segment_Point_MinLengthD( VECTOR_D SegmentPos1, VECTOR_D SegmentPos2, VECTOR_D PointPos )
{
	return _SQRTD( Segment_Point_MinLength_SquareD(  SegmentPos1,  SegmentPos2,  PointPos ) ) ;
}

// �����Ɠ_�̈�ԋ߂������̓��𓾂�
extern float Segment_Point_MinLength_Square( VECTOR SegmentPos1, VECTOR SegmentPos2, VECTOR PointPos )
{
	VECTOR MinPosition, Sa ;

	// ���Ɠ_�̍ŋߓ_���擾����
	MinPosition = Get_Line_Point_MinPosition( PointPos, SegmentPos1, SegmentPos2 ) ;

	// �ŋߓ_�Ɠ_�̋�����Ԃ�
	Sa.x = MinPosition.x - PointPos.x ;
	Sa.y = MinPosition.y - PointPos.y ;
	Sa.z = MinPosition.z - PointPos.z ;
	return Sa.x * Sa.x + Sa.y * Sa.y + Sa.z * Sa.z ;
}

// �����Ɠ_�̈�ԋ߂������̓��𓾂�
extern double Segment_Point_MinLength_SquareD( VECTOR_D SegmentPos1, VECTOR_D SegmentPos2, VECTOR_D PointPos )
{
	VECTOR_D MinPosition, Sa ;

	// ���Ɠ_�̍ŋߓ_���擾����
	MinPosition = Get_Line_Point_MinPositionD( PointPos, SegmentPos1, SegmentPos2 ) ;

	// �ŋߓ_�Ɠ_�̋�����Ԃ�
	Sa.x = MinPosition.x - PointPos.x ;
	Sa.y = MinPosition.y - PointPos.y ;
	Sa.z = MinPosition.z - PointPos.z ;
	return Sa.x * Sa.x + Sa.y * Sa.y + Sa.z * Sa.z ;
}

// �O�p�`�Ɠ_�̈�ԋ߂������𓾂�
extern float Triangle_Point_MinLength( VECTOR TrianglePos1, VECTOR TrianglePos2, VECTOR TrianglePos3, VECTOR PointPos )
{
	return _SQRT( Triangle_Point_MinLength_Square(  TrianglePos1,  TrianglePos2,  TrianglePos3,  PointPos ) ) ;
}

// �O�p�`�Ɠ_�̈�ԋ߂������𓾂�
extern double Triangle_Point_MinLengthD( VECTOR_D TrianglePos1, VECTOR_D TrianglePos2, VECTOR_D TrianglePos3, VECTOR_D PointPos )
{
	return _SQRTD( Triangle_Point_MinLength_SquareD(  TrianglePos1,  TrianglePos2,  TrianglePos3,  PointPos ) ) ;
}

// �_�Ɉ�ԋ߂����ʏ�̍��W�𓾂�
extern VECTOR Plane_Point_MinLength_Position( VECTOR PlanePos, VECTOR PlaneNormal, VECTOR PointPos )
{
	float t ;

	t = VDot( PlaneNormal, VSub( PointPos, PlanePos ) ) / VDot( PlaneNormal, PlaneNormal ) ;
	return VSub( PointPos, VScale( PlaneNormal, t ) ) ;
}

// �_�Ɉ�ԋ߂����ʏ�̍��W�𓾂�
extern VECTOR_D Plane_Point_MinLength_PositionD( VECTOR_D PlanePos, VECTOR_D PlaneNormal, VECTOR_D PointPos )
{
	double t ;

	t = VDotD( PlaneNormal, VSubD( PointPos, PlanePos ) ) / VDotD( PlaneNormal, PlaneNormal ) ;
	return VSubD( PointPos, VScaleD( PlaneNormal, t ) ) ;
}

// ���ʂƓ_�̈�ԋ߂������𓾂�
extern float Plane_Point_MinLength( VECTOR PlanePos, VECTOR PlaneNormal, VECTOR PointPos )
{
	float t ;

	t = VDot( PlaneNormal, VSub( PointPos, PlanePos ) ) / VDot( PlaneNormal, PlaneNormal ) ;
	return VSize( VScale( PlaneNormal, t ) ) ;
}

// ���ʂƓ_�̈�ԋ߂������𓾂�
extern double Plane_Point_MinLengthD( VECTOR_D PlanePos, VECTOR_D PlaneNormal, VECTOR_D PointPos )
{
	double t ;

	t = VDotD( PlaneNormal, VSubD( PointPos, PlanePos ) ) / VDotD( PlaneNormal, PlaneNormal ) ;
	return VSizeD( VScaleD( PlaneNormal, t ) ) ;
}

// �O�p�`�Ɠ_�̈�ԋ߂������̓��𓾂�
extern float Triangle_Point_MinLength_Square( VECTOR TrianglePos1, VECTOR TrianglePos2, VECTOR TrianglePos3, VECTOR PointPos )
{
	VECTOR MinPosition, Sa ;

	// �_�ƎO�p�`�̍ŋߓ_���擾����
	MinPosition = Get_Triangle_Point_MinPosition( PointPos, TrianglePos1, TrianglePos2, TrianglePos3 ) ;

	// �ŋߓ_�Ƃ̋�����Ԃ�
	Sa.x = MinPosition.x - PointPos.x ;
	Sa.y = MinPosition.y - PointPos.y ;
	Sa.z = MinPosition.z - PointPos.z ;
	return Sa.x * Sa.x + Sa.y * Sa.y + Sa.z * Sa.z ;
}

// �O�p�`�Ɠ_�̈�ԋ߂������̓��𓾂�
extern double Triangle_Point_MinLength_SquareD( VECTOR_D TrianglePos1, VECTOR_D TrianglePos2, VECTOR_D TrianglePos3, VECTOR_D PointPos )
{
	VECTOR_D MinPosition, Sa ;

	// �_�ƎO�p�`�̍ŋߓ_���擾����
	MinPosition = Get_Triangle_Point_MinPositionD( PointPos, TrianglePos1, TrianglePos2, TrianglePos3 ) ;

	// �ŋߓ_�Ƃ̋�����Ԃ�
	Sa.x = MinPosition.x - PointPos.x ;
	Sa.y = MinPosition.y - PointPos.y ;
	Sa.z = MinPosition.z - PointPos.z ;
	return Sa.x * Sa.x + Sa.y * Sa.y + Sa.z * Sa.z ;
}

// ��̎O�p�`�̍ŋߓ_�Ԃ̋����𓾂�
extern float Triangle_Triangle_MinLength( VECTOR Triangle1Pos1, VECTOR Triangle1Pos2, VECTOR Triangle1Pos3, VECTOR Triangle2Pos1, VECTOR Triangle2Pos2, VECTOR Triangle2Pos3 )
{
	return _SQRT( Triangle_Triangle_MinLength_Square( Triangle1Pos1, Triangle1Pos2, Triangle1Pos3, Triangle2Pos1, Triangle2Pos2, Triangle2Pos3 ) ) ;
}

// ��̎O�p�`�̍ŋߓ_�Ԃ̋����𓾂�
extern double Triangle_Triangle_MinLengthD( VECTOR_D Triangle1Pos1, VECTOR_D Triangle1Pos2, VECTOR_D Triangle1Pos3, VECTOR_D Triangle2Pos1, VECTOR_D Triangle2Pos2, VECTOR_D Triangle2Pos3 )
{
	return _SQRTD( Triangle_Triangle_MinLength_SquareD( Triangle1Pos1, Triangle1Pos2, Triangle1Pos3, Triangle2Pos1, Triangle2Pos2, Triangle2Pos3 ) ) ;
}

// ��̎O�p�`�̍ŋߓ_�Ԃ̋����̓��𓾂�
extern float Triangle_Triangle_MinLength_Square( VECTOR Triangle1Pos1, VECTOR Triangle1Pos2, VECTOR Triangle1Pos3, VECTOR Triangle2Pos1, VECTOR Triangle2Pos2, VECTOR Triangle2Pos3 )
{
	float MinDist ;
	float TempMinDist ;

	MinDist = Segment_Triangle_MinLength_Square( Triangle1Pos1, Triangle1Pos2, Triangle2Pos1, Triangle2Pos2, Triangle2Pos3 ) ;
	if( MinDist < 0.0000001f )
	{
		return 0.0f ;
	}

	TempMinDist = Segment_Triangle_MinLength_Square( Triangle1Pos2, Triangle1Pos3, Triangle2Pos1, Triangle2Pos2, Triangle2Pos3 ) ;
	if( TempMinDist < 0.0000001f )
	{
		return 0.0f ;
	}
	if( MinDist > TempMinDist )
	{
		MinDist = TempMinDist ;
	}

	TempMinDist = Segment_Triangle_MinLength_Square( Triangle1Pos3, Triangle1Pos1, Triangle2Pos1, Triangle2Pos2, Triangle2Pos3 ) ;
	if( TempMinDist < 0.0000001f )
	{
		return 0.0f ;
	}
	if( MinDist > TempMinDist )
	{
		MinDist = TempMinDist ;
	}


	TempMinDist = Segment_Triangle_MinLength_Square( Triangle2Pos1, Triangle2Pos2, Triangle1Pos1, Triangle1Pos2, Triangle1Pos3 ) ;
	if( TempMinDist < 0.0000001f )
	{
		return 0.0f ;
	}
	if( MinDist > TempMinDist )
	{
		MinDist = TempMinDist ;
	}

	TempMinDist = Segment_Triangle_MinLength_Square( Triangle2Pos2, Triangle2Pos3, Triangle1Pos1, Triangle1Pos2, Triangle1Pos3 ) ;
	if( TempMinDist < 0.0000001f )
	{
		return 0.0f ;
	}
	if( MinDist > TempMinDist )
	{
		MinDist = TempMinDist ;
	}

	TempMinDist = Segment_Triangle_MinLength_Square( Triangle2Pos3, Triangle2Pos1, Triangle1Pos1, Triangle1Pos2, Triangle1Pos3 ) ;
	if( TempMinDist < 0.0000001f )
	{
		return 0.0f ;
	}
	if( MinDist > TempMinDist )
	{
		MinDist = TempMinDist ;
	}

	return MinDist ;
}

// ��̎O�p�`�̍ŋߓ_�Ԃ̋����̓��𓾂�
extern double Triangle_Triangle_MinLength_SquareD( VECTOR_D Triangle1Pos1, VECTOR_D Triangle1Pos2, VECTOR_D Triangle1Pos3, VECTOR_D Triangle2Pos1, VECTOR_D Triangle2Pos2, VECTOR_D Triangle2Pos3 )
{
	double MinDist ;
	double TempMinDist ;

	MinDist = Segment_Triangle_MinLength_SquareD( Triangle1Pos1, Triangle1Pos2, Triangle2Pos1, Triangle2Pos2, Triangle2Pos3 ) ;
	if( MinDist < 0.0000001 )
	{
		return 0.0 ;
	}

	TempMinDist = Segment_Triangle_MinLength_SquareD( Triangle1Pos2, Triangle1Pos3, Triangle2Pos1, Triangle2Pos2, Triangle2Pos3 ) ;
	if( TempMinDist < 0.0000001 )
	{
		return 0.0 ;
	}
	if( MinDist > TempMinDist )
	{
		MinDist = TempMinDist ;
	}

	TempMinDist = Segment_Triangle_MinLength_SquareD( Triangle1Pos3, Triangle1Pos1, Triangle2Pos1, Triangle2Pos2, Triangle2Pos3 ) ;
	if( TempMinDist < 0.0000001 )
	{
		return 0.0 ;
	}
	if( MinDist > TempMinDist )
	{
		MinDist = TempMinDist ;
	}


	TempMinDist = Segment_Triangle_MinLength_SquareD( Triangle2Pos1, Triangle2Pos2, Triangle1Pos1, Triangle1Pos2, Triangle1Pos3 ) ;
	if( TempMinDist < 0.0000001 )
	{
		return 0.0 ;
	}
	if( MinDist > TempMinDist )
	{
		MinDist = TempMinDist ;
	}

	TempMinDist = Segment_Triangle_MinLength_SquareD( Triangle2Pos2, Triangle2Pos3, Triangle1Pos1, Triangle1Pos2, Triangle1Pos3 ) ;
	if( TempMinDist < 0.0000001 )
	{
		return 0.0 ;
	}
	if( MinDist > TempMinDist )
	{
		MinDist = TempMinDist ;
	}

	TempMinDist = Segment_Triangle_MinLength_SquareD( Triangle2Pos3, Triangle2Pos1, Triangle1Pos1, Triangle1Pos2, Triangle1Pos3 ) ;
	if( TempMinDist < 0.0000001 )
	{
		return 0.0 ;
	}
	if( MinDist > TempMinDist )
	{
		MinDist = TempMinDist ;
	}

	return MinDist ;
}




// �O�p�`�Ɛ��̓����蔻��
extern HITRESULT_LINE HitCheck_Line_Triangle( VECTOR LinePos1, VECTOR LinePos2, VECTOR TrianglePos1, VECTOR TrianglePos2, VECTOR TrianglePos3 )
{
	VECTOR LineL1L2, LineL1T1, LineL1T2, LineL1T3, OP ;
	VECTOR LineL1C, LineL2C ;
	float u, v, w, totalr ;
	HITRESULT_LINE Result ;

	Result.HitFlag = 0 ;

	VectorSub( &LineL1L2, &LinePos2, &LinePos1 ) ;
	VectorSub( &LineL1T1, &TrianglePos1, &LinePos1 ) ;
	VectorSub( &LineL1T2, &TrianglePos2, &LinePos1 ) ;
	VectorSub( &LineL1T3, &TrianglePos3, &LinePos1 ) ;

	VectorOuterProduct( &OP, &LineL1L2, &LineL1T2 ) ;
	u = VectorInnerProduct( &OP, &LineL1T3 ) ;

	VectorOuterProduct( &OP, &LineL1L2, &LineL1T3 ) ;
	v = VectorInnerProduct( &OP, &LineL1T1 ) ;

	VectorOuterProduct( &OP, &LineL1L2, &LineL1T1 ) ;
	w = VectorInnerProduct( &OP, &LineL1T2 ) ;

	if( ( ( u < 0.0f || v < 0.0f || w < 0.0f ) &&
		  ( u > 0.0f || v > 0.0f || w > 0.0f ) ) ||
		( u < 0.0000001f && u > -0.0000001f &&
		  v < 0.0000001f && v > -0.0000001f &&
		  w < 0.0000001f && w > -0.0000001f ) )
		return Result ;

	totalr = 1.0f / ( u + v + w ) ;
	u *= totalr ;
	v *= totalr ;
	w *= totalr ;
	Result.Position.x = TrianglePos1.x * u + TrianglePos2.x * v + TrianglePos3.x * w ;
	Result.Position.y = TrianglePos1.y * u + TrianglePos2.y * v + TrianglePos3.y * w ;
	Result.Position.z = TrianglePos1.z * u + TrianglePos2.z * v + TrianglePos3.z * w ;

	VectorSub( &LineL1C, &LinePos1, &Result.Position ) ;
	VectorSub( &LineL2C, &LinePos2, &Result.Position ) ;
	if( VectorInnerProduct( &LineL1C, &LineL2C ) <= 0.0f )
	{
		Result.HitFlag = 1 ;
	}

/*
	if( TriangleHit1_2T || TriangleHit1_3T )
	{
		VECTOR LineT1T2 ;
		VECTOR LineT1T3 ;
		VECTOR normLineT1T2X ;
		VECTOR normLineT1T2Y ;
		VECTOR normLineT1T2Z ;
		VECTOR ResPosS ;
		VECTOR DLineT1T2 ;
		VECTOR DLineT1T3 ;
		VECTOR DResPos ;
		float tempF ;
		float LineT1T2T ;
		float LineT1T3T ;

		VectorSub( &LineT1T2, &TrianglePos2, &TrianglePos1 ) ;
		VectorSub( &LineT1T3, &TrianglePos3, &TrianglePos1 ) ;
		VectorNormalize( &normLineT1T2X, &LineT1T2 ) ; 

		VectorOuterProduct( &normLineT1T2Z, &normLineT1T2X, &LineT1T3 ) ;
		VectorOuterProduct( &normLineT1T2Y, &normLineT1T2X, &normLineT1T2Z ) ;
		VectorNormalize( &normLineT1T2Y, &normLineT1T2Y ) ; 
		VectorNormalize( &normLineT1T2Z, &normLineT1T2Z ) ; 

		VectorSub( &ResPosS, &Result.Position, &TrianglePos1 ) ; 
		DResPos.x = normLineT1T2X.x * ResPosS.x + normLineT1T2X.y * ResPosS.y + normLineT1T2X.z * ResPosS.z ;
		DResPos.y = normLineT1T2Y.x * ResPosS.x + normLineT1T2Y.y * ResPosS.y + normLineT1T2Y.z * ResPosS.z ;
		DResPos.z = normLineT1T2Z.x * ResPosS.x + normLineT1T2Z.y * ResPosS.y + normLineT1T2Z.z * ResPosS.z ;

		DLineT1T2.x = normLineT1T2X.x * LineT1T2.x + normLineT1T2X.y * LineT1T2.y + normLineT1T2X.z * LineT1T2.z ;
		DLineT1T2.y = normLineT1T2Y.x * LineT1T2.x + normLineT1T2Y.y * LineT1T2.y + normLineT1T2Y.z * LineT1T2.z ;
		DLineT1T2.z = normLineT1T2Z.x * LineT1T2.x + normLineT1T2Z.y * LineT1T2.y + normLineT1T2Z.z * LineT1T2.z ;

		DLineT1T3.x = normLineT1T2X.x * LineT1T3.x + normLineT1T2X.y * LineT1T3.y + normLineT1T2X.z * LineT1T3.z ;
		DLineT1T3.y = normLineT1T2Y.x * LineT1T3.x + normLineT1T2Y.y * LineT1T3.y + normLineT1T2Y.z * LineT1T3.z ;
		DLineT1T3.z = normLineT1T2Z.x * LineT1T3.x + normLineT1T2Z.y * LineT1T3.y + normLineT1T2Z.z * LineT1T3.z ;

		tempF = DLineT1T3.x * DLineT1T2.y - DLineT1T3.y * DLineT1T2.x ;
		if( _FABS( tempF ) > 0.000001f )
		{
			LineT1T3T = ( DResPos.x * DLineT1T2.y - DResPos.y * DLineT1T2.x ) / tempF ;
			if( _FABS( DLineT1T2.x ) > 0.0001f )
			{
				LineT1T2T = ( DResPos.x - DLineT1T3.x * LineT1T3T ) / DLineT1T2.x ;
			}
			else
			{
				LineT1T2T = ( DResPos.y - DLineT1T3.y * LineT1T3T ) / DLineT1T2.y ;
			}
		}
		else
		{
			tempF = DLineT1T2.x * DLineT1T3.y - DLineT1T2.y * DLineT1T3.x ;
			LineT1T2T = ( DResPos.x * DLineT1T3.y - DResPos.y * DLineT1T3.x ) / tempF ;
			if( _FABS( DLineT1T3.x ) > 0.0001f )
			{
				LineT1T3T = ( DResPos.x - DLineT1T2.x * LineT1T2T ) / DLineT1T3.x ;
			}
			else
			{
				LineT1T3T = ( DResPos.y - DLineT1T2.y * LineT1T2T ) / DLineT1T3.y ;
			}
		}

		if( TriangleHit1_2T ) *TriangleHit1_2T = LineT1T2T ;
		if( TriangleHit1_3T ) *TriangleHit1_3T = LineT1T3T ;
	}
*/

	// �I��
	return Result ;
}

// �O�p�`�Ɛ��̓����蔻��
extern HITRESULT_LINE_D HitCheck_Line_TriangleD( VECTOR_D LinePos1, VECTOR_D LinePos2, VECTOR_D TrianglePos1, VECTOR_D TrianglePos2, VECTOR_D TrianglePos3 )
{
	VECTOR_D LineL1L2, LineL1T1, LineL1T2, LineL1T3, OP ;
	VECTOR_D LineL1C, LineL2C ;
	double u, v, w, totalr ;
	HITRESULT_LINE_D Result ;

	Result.HitFlag = 0 ;

	VectorSubD( &LineL1L2, &LinePos2, &LinePos1 ) ;
	VectorSubD( &LineL1T1, &TrianglePos1, &LinePos1 ) ;
	VectorSubD( &LineL1T2, &TrianglePos2, &LinePos1 ) ;
	VectorSubD( &LineL1T3, &TrianglePos3, &LinePos1 ) ;

	VectorOuterProductD( &OP, &LineL1L2, &LineL1T2 ) ;
	u = VectorInnerProductD( &OP, &LineL1T3 ) ;

	VectorOuterProductD( &OP, &LineL1L2, &LineL1T3 ) ;
	v = VectorInnerProductD( &OP, &LineL1T1 ) ;

	VectorOuterProductD( &OP, &LineL1L2, &LineL1T1 ) ;
	w = VectorInnerProductD( &OP, &LineL1T2 ) ;

	if( ( ( u < 0.0 || v < 0.0 || w < 0.0 ) &&
		  ( u > 0.0 || v > 0.0 || w > 0.0 ) ) ||
		( u < 0.0000001 && u > -0.0000001 &&
		  v < 0.0000001 && v > -0.0000001 &&
		  w < 0.0000001 && w > -0.0000001 ) )
		return Result ;

	totalr = 1.0 / ( u + v + w ) ;
	u *= totalr ;
	v *= totalr ;
	w *= totalr ;
	Result.Position.x = TrianglePos1.x * u + TrianglePos2.x * v + TrianglePos3.x * w ;
	Result.Position.y = TrianglePos1.y * u + TrianglePos2.y * v + TrianglePos3.y * w ;
	Result.Position.z = TrianglePos1.z * u + TrianglePos2.z * v + TrianglePos3.z * w ;

	VectorSubD( &LineL1C, &LinePos1, &Result.Position ) ;
	VectorSubD( &LineL2C, &LinePos2, &Result.Position ) ;
	if( VectorInnerProductD( &LineL1C, &LineL2C ) <= 0.0 )
	{
		Result.HitFlag = 1 ;
	}

	// �I��
	return Result ;
}


// �O�p�`�ƎO�p�`�̓����蔻��( TRUE:�������Ă���  FALSE:�������Ă��Ȃ� )
extern int HitCheck_Triangle_Triangle( VECTOR Triangle1Pos1, VECTOR Triangle1Pos2, VECTOR Triangle1Pos3, 
											    VECTOR Triangle2Pos1, VECTOR Triangle2Pos2, VECTOR Triangle2Pos3 )
{
	VECTOR N1, N2, CLDir, CLPos, sv21, sv31 ;
	float D1, D2, L1[ 3 ], L2[ 3 ], R1[ 2 ], R2[ 2 ], f ;
	VECTOR LD1[ 2 ], LP1[ 2 ], LD2[ 2 ], LP2[ 2 ] ;

	// �v�Z�덷���������ɂ����悤�ɒl������������
	if( Triangle1Pos1.x > 1000.0f || Triangle1Pos1.x < -1000.0f ||
		Triangle1Pos2.x > 1000.0f || Triangle1Pos2.x < -1000.0f ||
		Triangle1Pos3.x > 1000.0f || Triangle1Pos3.x < -1000.0f || 
		Triangle2Pos1.x > 1000.0f || Triangle2Pos1.x < -1000.0f ||
		Triangle2Pos2.x > 1000.0f || Triangle2Pos2.x < -1000.0f ||
		Triangle2Pos3.x > 1000.0f || Triangle2Pos3.x < -1000.0f )
	{
		Triangle1Pos1.x *= 0.0001f ; Triangle1Pos2.x *= 0.0001f ; Triangle1Pos3.x *= 0.0001f ;
		Triangle2Pos1.x *= 0.0001f ; Triangle2Pos2.x *= 0.0001f ; Triangle2Pos3.x *= 0.0001f ;
	}
	if( Triangle1Pos1.y > 1000.0f || Triangle1Pos1.y < -1000.0f ||
		Triangle1Pos2.y > 1000.0f || Triangle1Pos2.y < -1000.0f ||
		Triangle1Pos3.y > 1000.0f || Triangle1Pos3.y < -1000.0f || 
		Triangle2Pos1.y > 1000.0f || Triangle2Pos1.y < -1000.0f ||
		Triangle2Pos2.y > 1000.0f || Triangle2Pos2.y < -1000.0f ||
		Triangle2Pos3.y > 1000.0f || Triangle2Pos3.y < -1000.0f )
	{
		Triangle1Pos1.y *= 0.0001f ; Triangle1Pos2.y *= 0.0001f ; Triangle1Pos3.y *= 0.0001f ;
		Triangle2Pos1.y *= 0.0001f ; Triangle2Pos2.y *= 0.0001f ; Triangle2Pos3.y *= 0.0001f ;
	}
	if( Triangle1Pos1.z > 1000.0f || Triangle1Pos1.z < -1000.0f ||
		Triangle1Pos2.z > 1000.0f || Triangle1Pos2.z < -1000.0f ||
		Triangle1Pos3.z > 1000.0f || Triangle1Pos3.z < -1000.0f || 
		Triangle2Pos1.z > 1000.0f || Triangle2Pos1.z < -1000.0f ||
		Triangle2Pos2.z > 1000.0f || Triangle2Pos2.z < -1000.0f ||
		Triangle2Pos3.z > 1000.0f || Triangle2Pos3.z < -1000.0f )
	{
		Triangle1Pos1.z *= 0.0001f ; Triangle1Pos2.z *= 0.0001f ; Triangle1Pos3.z *= 0.0001f ;
		Triangle2Pos1.z *= 0.0001f ; Triangle2Pos2.z *= 0.0001f ; Triangle2Pos3.z *= 0.0001f ;
	}

	// �O�p�`�Q�̒��_���O�p�`�P���������ʂ̕Б��ɑ��݂���ꍇ�͓������Ă��Ȃ�
	sv21 = VSub( Triangle1Pos2, Triangle1Pos1 ) ;
	sv31 = VSub( Triangle1Pos3, Triangle1Pos1 ) ;
	N1 = VNorm( VCross( sv21, sv31 ) ) ;
	D1 = VDot( N1, Triangle1Pos1 ) ;
	L1[ 0 ] = VDot( N1, Triangle2Pos1 ) - D1 ;
	L1[ 1 ] = VDot( N1, Triangle2Pos2 ) - D1 ;
	L1[ 2 ] = VDot( N1, Triangle2Pos3 ) - D1 ;
	if( L1[ 0 ] < 0.0f )
	{
		if( L1[ 1 ] < 0.0f )
		{
			if( L1[ 2 ] < 0.0f )
				return FALSE ;

			LD1[ 0 ] = VSub( Triangle2Pos3, Triangle2Pos2 ) ;
			LP1[ 0 ] = Triangle2Pos2 ;
			LD1[ 1 ] = VSub( Triangle2Pos3, Triangle2Pos1 ) ;
			LP1[ 1 ] = Triangle2Pos1 ;
		}
		else
		{
			if( L1[ 2 ] < 0.0f )
			{
				LD1[ 0 ] = VSub( Triangle2Pos2, Triangle2Pos1 ) ;
				LP1[ 0 ] = Triangle2Pos1 ;
				LD1[ 1 ] = VSub( Triangle2Pos2, Triangle2Pos3 ) ;
				LP1[ 1 ] = Triangle2Pos3 ;
			}
			else
			{
				LD1[ 0 ] = VSub( Triangle2Pos1, Triangle2Pos2 ) ;
				LP1[ 0 ] = Triangle2Pos2 ;
				LD1[ 1 ] = VSub( Triangle2Pos1, Triangle2Pos3 ) ;
				LP1[ 1 ] = Triangle2Pos3 ;
			}
		}
	}
	else
	{
		if( L1[ 1 ] < 0.0f )
		{
			if( L1[ 2 ] < 0.0f )
			{
				LD1[ 0 ] = VSub( Triangle2Pos1, Triangle2Pos2 ) ;
				LP1[ 0 ] = Triangle2Pos2 ;
				LD1[ 1 ] = VSub( Triangle2Pos1, Triangle2Pos3 ) ;
				LP1[ 1 ] = Triangle2Pos3 ;
			}
			else
			{
				LD1[ 0 ] = VSub( Triangle2Pos2, Triangle2Pos1 ) ;
				LP1[ 0 ] = Triangle2Pos1 ;
				LD1[ 1 ] = VSub( Triangle2Pos2, Triangle2Pos3 ) ;
				LP1[ 1 ] = Triangle2Pos3 ;
			}
		}
		else
		{
			if( L1[ 2 ] < 0.0f )
			{
				LD1[ 0 ] = VSub( Triangle2Pos3, Triangle2Pos2 ) ;
				LP1[ 0 ] = Triangle2Pos2 ;
				LD1[ 1 ] = VSub( Triangle2Pos3, Triangle2Pos1 ) ;
				LP1[ 1 ] = Triangle2Pos1 ;
			}
			else return FALSE ;
		}
	}

	// �O�p�`�P�̒��_���O�p�`�Q���������ʂ̕Б��ɑ��݂���ꍇ�͓������Ă��Ȃ�
	N2 = VNorm( VCross( VSub( Triangle2Pos2, Triangle2Pos1 ), VSub( Triangle2Pos3, Triangle2Pos1 ) ) ) ;
	D2 = VDot( N2, Triangle2Pos1 ) ;
	L2[ 0 ] = VDot( N2, Triangle1Pos1 ) - D2 ;
	L2[ 1 ] = VDot( N2, Triangle1Pos2 ) - D2 ;
	L2[ 2 ] = VDot( N2, Triangle1Pos3 ) - D2 ;
	if( L2[ 0 ] < 0.0f )
	{
		if( L2[ 1 ] < 0.0f )
		{
			if( L2[ 2 ] < 0.0f )
				return FALSE ;

			LD2[ 0 ] = VSub( Triangle1Pos3, Triangle1Pos2 ) ;
			LP2[ 0 ] = Triangle1Pos2 ;
			LD2[ 1 ] = VSub( Triangle1Pos3, Triangle1Pos1 ) ;
			LP2[ 1 ] = Triangle1Pos1 ;
		}
		else
		{
			if( L2[ 2 ] < 0.0f )
			{
				LD2[ 0 ] = VSub( Triangle1Pos2, Triangle1Pos1 ) ;
				LP2[ 0 ] = Triangle1Pos1 ;
				LD2[ 1 ] = VSub( Triangle1Pos2, Triangle1Pos3 ) ;
				LP2[ 1 ] = Triangle1Pos3 ;
			}
			else
			{
				LD2[ 0 ] = VSub( Triangle1Pos1, Triangle1Pos2 ) ;
				LP2[ 0 ] = Triangle1Pos2 ;
				LD2[ 1 ] = VSub( Triangle1Pos1, Triangle1Pos3 ) ;
				LP2[ 1 ] = Triangle1Pos3 ;
			}
		}
	}
	else
	{
		if( L2[ 1 ] < 0.0f )
		{
			if( L2[ 2 ] < 0.0f )
			{
				LD2[ 0 ] = VSub( Triangle1Pos1, Triangle1Pos2 ) ;
				LP2[ 0 ] = Triangle1Pos2 ;
				LD2[ 1 ] = VSub( Triangle1Pos1, Triangle1Pos3 ) ;
				LP2[ 1 ] = Triangle1Pos3 ;
			}
			else
			{
				LD2[ 0 ] = VSub( Triangle1Pos2, Triangle1Pos1 ) ;
				LP2[ 0 ] = Triangle1Pos1 ;
				LD2[ 1 ] = VSub( Triangle1Pos2, Triangle1Pos3 ) ;
				LP2[ 1 ] = Triangle1Pos3 ;
			}
		}
		else
		{
			if( L2[ 2 ] < 0.0f )
			{
				LD2[ 0 ] = VSub( Triangle1Pos3, Triangle1Pos2 ) ;
				LP2[ 0 ] = Triangle1Pos2 ;
				LD2[ 1 ] = VSub( Triangle1Pos3, Triangle1Pos1 ) ;
				LP2[ 1 ] = Triangle1Pos1 ;
			}
			else return FALSE ;
		}
	}

	// ��̕��ʂ�����������𓾂�
	Get_TwoPlane_Line( N1, D1, N2, D2, &CLDir, &CLPos ) ;

	// ����������Ɗe�ʂ̕ӂƂ̌�_�̈ʒu�𓾂�
	Get_TwoLine_MinLength_Rate( LD1[ 0 ], LP1[ 0 ], NULL, CLDir, CLPos, &R1[ 0 ] ) ;
	Get_TwoLine_MinLength_Rate( LD1[ 1 ], LP1[ 1 ], NULL, CLDir, CLPos, &R1[ 1 ] ) ;
	Get_TwoLine_MinLength_Rate( LD2[ 0 ], LP2[ 0 ], NULL, CLDir, CLPos, &R2[ 0 ] ) ;
	Get_TwoLine_MinLength_Rate( LD2[ 1 ], LP2[ 1 ], NULL, CLDir, CLPos, &R2[ 1 ] ) ;
	if( R1[ 0 ] > R1[ 1 ] ){ f = R1[ 0 ] ; R1[ 0 ] = R1[ 1 ] ; R1[ 1 ] = f ; }
	if( R2[ 0 ] > R2[ 1 ] ){ f = R2[ 0 ] ; R2[ 0 ] = R2[ 1 ] ; R2[ 1 ] = f ; }

	// �d�Ȃ��Ă����瓖�����Ă���
	return  ( R1[ 0 ] <= R2[ 0 ] && R1[ 1 ] >= R2[ 0 ] ) ||
			( R2[ 0 ] <= R1[ 0 ] && R2[ 1 ] >= R1[ 0 ] ) ? TRUE : FALSE ;
}

// �O�p�`�ƎO�p�`�̓����蔻��( TRUE:�������Ă���  FALSE:�������Ă��Ȃ� )
extern int HitCheck_Triangle_TriangleD( VECTOR_D Triangle1Pos1, VECTOR_D Triangle1Pos2, VECTOR_D Triangle1Pos3, 
											    VECTOR_D Triangle2Pos1, VECTOR_D Triangle2Pos2, VECTOR_D Triangle2Pos3 )
{
	VECTOR_D N1, N2, CLDir, CLPos, sv21, sv31 ;
	double D1, D2, L1[ 3 ], L2[ 3 ], R1[ 2 ], R2[ 2 ], f ;
	VECTOR_D LD1[ 2 ], LP1[ 2 ], LD2[ 2 ], LP2[ 2 ] ;

	// �O�p�`�Q�̒��_���O�p�`�P���������ʂ̕Б��ɑ��݂���ꍇ�͓������Ă��Ȃ�
	sv21 = VSubD( Triangle1Pos2, Triangle1Pos1 ) ;
	sv31 = VSubD( Triangle1Pos3, Triangle1Pos1 ) ;
	N1 = VNormD( VCrossD( sv21, sv31 ) ) ;
	D1 = VDotD( N1, Triangle1Pos1 ) ;
	L1[ 0 ] = VDotD( N1, Triangle2Pos1 ) - D1 ;
	L1[ 1 ] = VDotD( N1, Triangle2Pos2 ) - D1 ;
	L1[ 2 ] = VDotD( N1, Triangle2Pos3 ) - D1 ;
	if( L1[ 0 ] < 0.0 )
	{
		if( L1[ 1 ] < 0.0 )
		{
			if( L1[ 2 ] < 0.0 )
				return FALSE ;

			LD1[ 0 ] = VSubD( Triangle2Pos3, Triangle2Pos2 ) ;
			LP1[ 0 ] = Triangle2Pos2 ;
			LD1[ 1 ] = VSubD( Triangle2Pos3, Triangle2Pos1 ) ;
			LP1[ 1 ] = Triangle2Pos1 ;
		}
		else
		{
			if( L1[ 2 ] < 0.0 )
			{
				LD1[ 0 ] = VSubD( Triangle2Pos2, Triangle2Pos1 ) ;
				LP1[ 0 ] = Triangle2Pos1 ;
				LD1[ 1 ] = VSubD( Triangle2Pos2, Triangle2Pos3 ) ;
				LP1[ 1 ] = Triangle2Pos3 ;
			}
			else
			{
				LD1[ 0 ] = VSubD( Triangle2Pos1, Triangle2Pos2 ) ;
				LP1[ 0 ] = Triangle2Pos2 ;
				LD1[ 1 ] = VSubD( Triangle2Pos1, Triangle2Pos3 ) ;
				LP1[ 1 ] = Triangle2Pos3 ;
			}
		}
	}
	else
	{
		if( L1[ 1 ] < 0.0 )
		{
			if( L1[ 2 ] < 0.0 )
			{
				LD1[ 0 ] = VSubD( Triangle2Pos1, Triangle2Pos2 ) ;
				LP1[ 0 ] = Triangle2Pos2 ;
				LD1[ 1 ] = VSubD( Triangle2Pos1, Triangle2Pos3 ) ;
				LP1[ 1 ] = Triangle2Pos3 ;
			}
			else
			{
				LD1[ 0 ] = VSubD( Triangle2Pos2, Triangle2Pos1 ) ;
				LP1[ 0 ] = Triangle2Pos1 ;
				LD1[ 1 ] = VSubD( Triangle2Pos2, Triangle2Pos3 ) ;
				LP1[ 1 ] = Triangle2Pos3 ;
			}
		}
		else
		{
			if( L1[ 2 ] < 0.0 )
			{
				LD1[ 0 ] = VSubD( Triangle2Pos3, Triangle2Pos2 ) ;
				LP1[ 0 ] = Triangle2Pos2 ;
				LD1[ 1 ] = VSubD( Triangle2Pos3, Triangle2Pos1 ) ;
				LP1[ 1 ] = Triangle2Pos1 ;
			}
			else return FALSE ;
		}
	}

	// �O�p�`�P�̒��_���O�p�`�Q���������ʂ̕Б��ɑ��݂���ꍇ�͓������Ă��Ȃ�
	N2 = VNormD( VCrossD( VSubD( Triangle2Pos2, Triangle2Pos1 ), VSubD( Triangle2Pos3, Triangle2Pos1 ) ) ) ;
	D2 = VDotD( N2, Triangle2Pos1 ) ;
	L2[ 0 ] = VDotD( N2, Triangle1Pos1 ) - D2 ;
	L2[ 1 ] = VDotD( N2, Triangle1Pos2 ) - D2 ;
	L2[ 2 ] = VDotD( N2, Triangle1Pos3 ) - D2 ;
	if( L2[ 0 ] < 0.0 )
	{
		if( L2[ 1 ] < 0.0 )
		{
			if( L2[ 2 ] < 0.0 )
				return FALSE ;

			LD2[ 0 ] = VSubD( Triangle1Pos3, Triangle1Pos2 ) ;
			LP2[ 0 ] = Triangle1Pos2 ;
			LD2[ 1 ] = VSubD( Triangle1Pos3, Triangle1Pos1 ) ;
			LP2[ 1 ] = Triangle1Pos1 ;
		}
		else
		{
			if( L2[ 2 ] < 0.0 )
			{
				LD2[ 0 ] = VSubD( Triangle1Pos2, Triangle1Pos1 ) ;
				LP2[ 0 ] = Triangle1Pos1 ;
				LD2[ 1 ] = VSubD( Triangle1Pos2, Triangle1Pos3 ) ;
				LP2[ 1 ] = Triangle1Pos3 ;
			}
			else
			{
				LD2[ 0 ] = VSubD( Triangle1Pos1, Triangle1Pos2 ) ;
				LP2[ 0 ] = Triangle1Pos2 ;
				LD2[ 1 ] = VSubD( Triangle1Pos1, Triangle1Pos3 ) ;
				LP2[ 1 ] = Triangle1Pos3 ;
			}
		}
	}
	else
	{
		if( L2[ 1 ] < 0.0 )
		{
			if( L2[ 2 ] < 0.0 )
			{
				LD2[ 0 ] = VSubD( Triangle1Pos1, Triangle1Pos2 ) ;
				LP2[ 0 ] = Triangle1Pos2 ;
				LD2[ 1 ] = VSubD( Triangle1Pos1, Triangle1Pos3 ) ;
				LP2[ 1 ] = Triangle1Pos3 ;
			}
			else
			{
				LD2[ 0 ] = VSubD( Triangle1Pos2, Triangle1Pos1 ) ;
				LP2[ 0 ] = Triangle1Pos1 ;
				LD2[ 1 ] = VSubD( Triangle1Pos2, Triangle1Pos3 ) ;
				LP2[ 1 ] = Triangle1Pos3 ;
			}
		}
		else
		{
			if( L2[ 2 ] < 0.0 )
			{
				LD2[ 0 ] = VSubD( Triangle1Pos3, Triangle1Pos2 ) ;
				LP2[ 0 ] = Triangle1Pos2 ;
				LD2[ 1 ] = VSubD( Triangle1Pos3, Triangle1Pos1 ) ;
				LP2[ 1 ] = Triangle1Pos1 ;
			}
			else return FALSE ;
		}
	}

	// ��̕��ʂ�����������𓾂�
	Get_TwoPlane_LineD( N1, D1, N2, D2, &CLDir, &CLPos ) ;

	// ����������Ɗe�ʂ̕ӂƂ̌�_�̈ʒu�𓾂�
	Get_TwoLine_MinLength_RateD( LD1[ 0 ], LP1[ 0 ], NULL, CLDir, CLPos, &R1[ 0 ] ) ;
	Get_TwoLine_MinLength_RateD( LD1[ 1 ], LP1[ 1 ], NULL, CLDir, CLPos, &R1[ 1 ] ) ;
	Get_TwoLine_MinLength_RateD( LD2[ 0 ], LP2[ 0 ], NULL, CLDir, CLPos, &R2[ 0 ] ) ;
	Get_TwoLine_MinLength_RateD( LD2[ 1 ], LP2[ 1 ], NULL, CLDir, CLPos, &R2[ 1 ] ) ;
	if( R1[ 0 ] > R1[ 1 ] ){ f = R1[ 0 ] ; R1[ 0 ] = R1[ 1 ] ; R1[ 1 ] = f ; }
	if( R2[ 0 ] > R2[ 1 ] ){ f = R2[ 0 ] ; R2[ 0 ] = R2[ 1 ] ; R2[ 1 ] = f ; }

	// �d�Ȃ��Ă����瓖�����Ă���
	return  ( R1[ 0 ] <= R2[ 0 ] && R1[ 1 ] >= R2[ 0 ] ) ||
			( R2[ 0 ] <= R1[ 0 ] && R2[ 1 ] >= R1[ 0 ] ) ? TRUE : FALSE ;
}


// ���Ƌ��̓����蔻��( TRUE:�������Ă���  FALSE:�������Ă��Ȃ� )
extern int HitCheck_Line_Sphere( VECTOR LinePos1, VECTOR LinePos2, VECTOR SphereCenterPos, float SphereR )
{
	VECTOR MinPosition, Sa ;

	// ���ƎO�p�`�̍ŋߓ_���擾����
	MinPosition = Get_Line_Point_MinPosition( SphereCenterPos, LinePos1, LinePos2 ) ;

	// �ŋߓ_�Ƃ̋��������̔��a���߂����ǂ����œ������Ă��邩�ǂ����𔻒�
	Sa.x = MinPosition.x - SphereCenterPos.x ;
	Sa.y = MinPosition.y - SphereCenterPos.y ;
	Sa.z = MinPosition.z - SphereCenterPos.z ;
	return Sa.x * Sa.x + Sa.y * Sa.y + Sa.z * Sa.z <= SphereR * SphereR ? TRUE : FALSE ;
}

// ���Ƌ��̓����蔻��( TRUE:�������Ă���  FALSE:�������Ă��Ȃ� )
extern int HitCheck_Line_SphereD( VECTOR_D LinePos1, VECTOR_D LinePos2, VECTOR_D SphereCenterPos, double SphereR )
{
	VECTOR_D MinPosition, Sa ;

	// ���ƎO�p�`�̍ŋߓ_���擾����
	MinPosition = Get_Line_Point_MinPositionD( SphereCenterPos, LinePos1, LinePos2 ) ;

	// �ŋߓ_�Ƃ̋��������̔��a���߂����ǂ����œ������Ă��邩�ǂ����𔻒�
	Sa.x = MinPosition.x - SphereCenterPos.x ;
	Sa.y = MinPosition.y - SphereCenterPos.y ;
	Sa.z = MinPosition.z - SphereCenterPos.z ;
	return Sa.x * Sa.x + Sa.y * Sa.y + Sa.z * Sa.z <= SphereR * SphereR ? TRUE : FALSE ;
}


// ���Ɣ��̓����蔻��
extern HITRESULT_LINE HitCheck_Line_Cube( VECTOR LinePos1, VECTOR LinePos2, VECTOR CubePos1, VECTOR CubePos2 )
{
	float Tmp ;
	HITRESULT_LINE HitResult ;
	VECTOR CubePos[ 2 ][ 4 ] ;
	HITRESULT_LINE Result ;
	float MinDistanceSquare ;
	float DistanceSquare ;
	int i ;
	static int PosIndex[ 12 ][ 3 ][ 2 ] =
	{
		{ { 0, 0 }, { 0, 1 }, { 0, 2 } },
		{ { 0, 3 }, { 0, 2 }, { 0, 1 } },

		{ { 1, 0 }, { 1, 1 }, { 1, 2 } },
		{ { 1, 3 }, { 1, 2 }, { 1, 1 } },

		{ { 0, 0 }, { 0, 2 }, { 1, 0 } },
		{ { 1, 2 }, { 1, 0 }, { 0, 2 } },

		{ { 0, 1 }, { 0, 3 }, { 1, 1 } },
		{ { 1, 3 }, { 1, 1 }, { 0, 3 } },

		{ { 0, 1 }, { 0, 0 }, { 1, 1 } },
		{ { 1, 0 }, { 1, 1 }, { 0, 0 } },

		{ { 0, 2 }, { 0, 3 }, { 1, 2 } },
		{ { 1, 3 }, { 1, 2 }, { 0, 3 } },
	} ;

	// �߂�l��������
	Result.HitFlag = FALSE ;
	Result.Position = VGet( -1.0f, -1.0f, -1.0f ) ;

	// CubePos1 �̒l��� CubePos2 �̒l�̂ق����������Ȃ��Ă��������ւ���
	if( CubePos1.x > CubePos2.x ){ Tmp = CubePos1.x ; CubePos1.x = CubePos2.x ; CubePos2.x = Tmp ; }
	if( CubePos1.y > CubePos2.y ){ Tmp = CubePos1.y ; CubePos1.y = CubePos2.y ; CubePos2.y = Tmp ; }
	if( CubePos1.z > CubePos2.z ){ Tmp = CubePos1.z ; CubePos1.z = CubePos2.z ; CubePos2.z = Tmp ; }

	// ���������͈̔͊O�ɂ��邩�ȈՃ`�F�b�N
	if( ( LinePos1.x < CubePos1.x && LinePos2.x < CubePos1.x ) ||
		( LinePos1.y < CubePos1.y && LinePos2.y < CubePos1.y ) ||
		( LinePos1.z < CubePos1.z && LinePos2.z < CubePos1.z ) ||
		( LinePos1.x > CubePos2.x && LinePos2.x > CubePos2.x ) ||
		( LinePos1.y > CubePos2.y && LinePos2.y > CubePos2.y ) ||
		( LinePos1.z > CubePos2.z && LinePos2.z > CubePos2.z ) )
	{
		return Result ;
	}

	// ���������͈͓̔��ɂ��邩�ȈՃ`�F�b�N
	if( ( LinePos1.x >= CubePos1.x && LinePos1.x <= CubePos2.x && LinePos2.x >= CubePos1.x && LinePos2.x <= CubePos2.x ) &&
		( LinePos1.y >= CubePos1.y && LinePos1.y <= CubePos2.y && LinePos2.y >= CubePos1.y && LinePos2.y <= CubePos2.y ) &&
		( LinePos1.z >= CubePos1.z && LinePos1.z <= CubePos2.z && LinePos2.z >= CubePos1.z && LinePos2.z <= CubePos2.z ) )
	{
		Result.Position = LinePos1 ;
		return Result ;
	}

	// ���̖ʂ��|���S���Ƃ��Č����`�F�b�N
	CubePos[ 0 ][ 0 ].x = CubePos1.x ;
	CubePos[ 0 ][ 0 ].y = CubePos1.y ;
	CubePos[ 0 ][ 0 ].z = CubePos1.z ;

	CubePos[ 0 ][ 1 ].x = CubePos2.x ;
	CubePos[ 0 ][ 1 ].y = CubePos1.y ;
	CubePos[ 0 ][ 1 ].z = CubePos1.z ;

	CubePos[ 0 ][ 2 ].x = CubePos1.x ;
	CubePos[ 0 ][ 2 ].y = CubePos1.y ;
	CubePos[ 0 ][ 2 ].z = CubePos2.z ;

	CubePos[ 0 ][ 3 ].x = CubePos2.x ;
	CubePos[ 0 ][ 3 ].y = CubePos1.y ;
	CubePos[ 0 ][ 3 ].z = CubePos2.z ;

	CubePos[ 1 ][ 0 ].x = CubePos1.x ;
	CubePos[ 1 ][ 0 ].y = CubePos2.y ;
	CubePos[ 1 ][ 0 ].z = CubePos1.z ;

	CubePos[ 1 ][ 1 ].x = CubePos2.x ;
	CubePos[ 1 ][ 1 ].y = CubePos2.y ;
	CubePos[ 1 ][ 1 ].z = CubePos1.z ;

	CubePos[ 1 ][ 2 ].x = CubePos1.x ;
	CubePos[ 1 ][ 2 ].y = CubePos2.y ;
	CubePos[ 1 ][ 2 ].z = CubePos2.z ;

	CubePos[ 1 ][ 3 ].x = CubePos2.x ;
	CubePos[ 1 ][ 3 ].y = CubePos2.y ;
	CubePos[ 1 ][ 3 ].z = CubePos2.z ;

	MinDistanceSquare = 0.0f ;

	for( i = 0; i < 12; i ++ )
	{
		HitResult = HitCheck_Line_Triangle(
			LinePos1,
			LinePos2,
			CubePos[ PosIndex[ i ][ 0 ][ 0 ] ][ PosIndex[ i ][ 0 ][ 1 ] ],
			CubePos[ PosIndex[ i ][ 1 ][ 0 ] ][ PosIndex[ i ][ 1 ][ 1 ] ],
			CubePos[ PosIndex[ i ][ 2 ][ 0 ] ][ PosIndex[ i ][ 2 ][ 1 ] ]
		) ;
		if( HitResult.HitFlag )
		{
			DistanceSquare = VSquareSize( VSub( LinePos1, HitResult.Position ) ) ;
			if( Result.HitFlag == FALSE || MinDistanceSquare > DistanceSquare )
			{
				Result.HitFlag = TRUE ;
				MinDistanceSquare = DistanceSquare ;
				Result.Position = HitResult.Position ;
			}
		}
	}

	return Result ;
}

// ���Ɣ��̓����蔻��
extern HITRESULT_LINE_D HitCheck_Line_CubeD( VECTOR_D LinePos1, VECTOR_D LinePos2, VECTOR_D CubePos1, VECTOR_D CubePos2 )
{
	double Tmp ;
	HITRESULT_LINE_D HitResult ;
	VECTOR_D CubePos[ 2 ][ 4 ] ;
	HITRESULT_LINE_D Result ;
	double MinDistanceSquare ;
	double DistanceSquare ;
	int i ;
	static int PosIndex[ 12 ][ 3 ][ 2 ] =
	{
		{ { 0, 0 }, { 0, 1 }, { 0, 2 } },
		{ { 0, 3 }, { 0, 2 }, { 0, 1 } },

		{ { 1, 0 }, { 1, 1 }, { 1, 2 } },
		{ { 1, 3 }, { 1, 2 }, { 1, 1 } },

		{ { 0, 0 }, { 0, 2 }, { 1, 0 } },
		{ { 1, 2 }, { 1, 0 }, { 0, 2 } },

		{ { 0, 1 }, { 0, 3 }, { 1, 1 } },
		{ { 1, 3 }, { 1, 1 }, { 0, 3 } },

		{ { 0, 1 }, { 0, 0 }, { 1, 1 } },
		{ { 1, 0 }, { 1, 1 }, { 0, 0 } },

		{ { 0, 2 }, { 0, 3 }, { 1, 2 } },
		{ { 1, 3 }, { 1, 2 }, { 0, 3 } },
	} ;

	// �߂�l��������
	Result.HitFlag = FALSE ;
	Result.Position = VGetD( -1.0, -1.0, -1.0 ) ;

	// CubePos1 �̒l��� CubePos2 �̒l�̂ق����������Ȃ��Ă��������ւ���
	if( CubePos1.x > CubePos2.x ){ Tmp = CubePos1.x ; CubePos1.x = CubePos2.x ; CubePos2.x = Tmp ; }
	if( CubePos1.y > CubePos2.y ){ Tmp = CubePos1.y ; CubePos1.y = CubePos2.y ; CubePos2.y = Tmp ; }
	if( CubePos1.z > CubePos2.z ){ Tmp = CubePos1.z ; CubePos1.z = CubePos2.z ; CubePos2.z = Tmp ; }

	// ���������͈̔͊O�ɂ��邩�ȈՃ`�F�b�N
	if( ( LinePos1.x < CubePos1.x && LinePos2.x < CubePos1.x ) ||
		( LinePos1.y < CubePos1.y && LinePos2.y < CubePos1.y ) ||
		( LinePos1.z < CubePos1.z && LinePos2.z < CubePos1.z ) ||
		( LinePos1.x > CubePos2.x && LinePos2.x > CubePos2.x ) ||
		( LinePos1.y > CubePos2.y && LinePos2.y > CubePos2.y ) ||
		( LinePos1.z > CubePos2.z && LinePos2.z > CubePos2.z ) )
	{
		return Result ;
	}

	// ���������͈͓̔��ɂ��邩�ȈՃ`�F�b�N
	if( ( LinePos1.x >= CubePos1.x && LinePos1.x <= CubePos2.x && LinePos2.x >= CubePos1.x && LinePos2.x <= CubePos2.x ) &&
		( LinePos1.y >= CubePos1.y && LinePos1.y <= CubePos2.y && LinePos2.y >= CubePos1.y && LinePos2.y <= CubePos2.y ) &&
		( LinePos1.z >= CubePos1.z && LinePos1.z <= CubePos2.z && LinePos2.z >= CubePos1.z && LinePos2.z <= CubePos2.z ) )
	{
		Result.Position = LinePos1 ;
		return Result ;
	}

	// ���̖ʂ��|���S���Ƃ��Č����`�F�b�N
	CubePos[ 0 ][ 0 ].x = CubePos1.x ;
	CubePos[ 0 ][ 0 ].y = CubePos1.y ;
	CubePos[ 0 ][ 0 ].z = CubePos1.z ;

	CubePos[ 0 ][ 1 ].x = CubePos2.x ;
	CubePos[ 0 ][ 1 ].y = CubePos1.y ;
	CubePos[ 0 ][ 1 ].z = CubePos1.z ;

	CubePos[ 0 ][ 2 ].x = CubePos1.x ;
	CubePos[ 0 ][ 2 ].y = CubePos1.y ;
	CubePos[ 0 ][ 2 ].z = CubePos2.z ;

	CubePos[ 0 ][ 3 ].x = CubePos2.x ;
	CubePos[ 0 ][ 3 ].y = CubePos1.y ;
	CubePos[ 0 ][ 3 ].z = CubePos2.z ;

	CubePos[ 1 ][ 0 ].x = CubePos1.x ;
	CubePos[ 1 ][ 0 ].y = CubePos2.y ;
	CubePos[ 1 ][ 0 ].z = CubePos1.z ;

	CubePos[ 1 ][ 1 ].x = CubePos2.x ;
	CubePos[ 1 ][ 1 ].y = CubePos2.y ;
	CubePos[ 1 ][ 1 ].z = CubePos1.z ;

	CubePos[ 1 ][ 2 ].x = CubePos1.x ;
	CubePos[ 1 ][ 2 ].y = CubePos2.y ;
	CubePos[ 1 ][ 2 ].z = CubePos2.z ;

	CubePos[ 1 ][ 3 ].x = CubePos2.x ;
	CubePos[ 1 ][ 3 ].y = CubePos2.y ;
	CubePos[ 1 ][ 3 ].z = CubePos2.z ;

	MinDistanceSquare = 0.0 ;

	for( i = 0; i < 12; i ++ )
	{
		HitResult = HitCheck_Line_TriangleD(
			LinePos1,
			LinePos2,
			CubePos[ PosIndex[ i ][ 0 ][ 0 ] ][ PosIndex[ i ][ 0 ][ 1 ] ],
			CubePos[ PosIndex[ i ][ 1 ][ 0 ] ][ PosIndex[ i ][ 1 ][ 1 ] ],
			CubePos[ PosIndex[ i ][ 2 ][ 0 ] ][ PosIndex[ i ][ 2 ][ 1 ] ]
		) ;
		if( HitResult.HitFlag )
		{
			DistanceSquare = VSquareSizeD( VSubD( LinePos1, HitResult.Position ) ) ;
			if( Result.HitFlag == FALSE || MinDistanceSquare > DistanceSquare )
			{
				Result.HitFlag = TRUE ;
				MinDistanceSquare = DistanceSquare ;
				Result.Position = HitResult.Position ;
			}
		}
	}

	return Result ;
}


// ���Ƌ��̓����蔻��( TRUE:�������Ă���  FALSE:�������Ă��Ȃ� )
extern int HitCheck_Sphere_Sphere( VECTOR Sphere1CenterPos, float Sphere1R, VECTOR Sphere2CenterPos, float Sphere2R )
{
	VECTOR Sa ;

	// ��̋��̒��S���W�̋�������̋��̔��a�𑫂����l��菬�����ꍇ�͓������Ă���
	Sa.x = Sphere1CenterPos.x - Sphere2CenterPos.x ;
	Sa.y = Sphere1CenterPos.y - Sphere2CenterPos.y ;
	Sa.z = Sphere1CenterPos.z - Sphere2CenterPos.z ;
	return Sa.x * Sa.x + Sa.y * Sa.y + Sa.z * Sa.z <= ( Sphere1R + Sphere2R ) * ( Sphere1R + Sphere2R ) ? TRUE : FALSE ;
}

// ���Ƌ��̓����蔻��( TRUE:�������Ă���  FALSE:�������Ă��Ȃ� )
extern int HitCheck_Sphere_SphereD( VECTOR_D Sphere1CenterPos, double Sphere1R, VECTOR_D Sphere2CenterPos, double Sphere2R )
{
	VECTOR_D Sa ;

	// ��̋��̒��S���W�̋�������̋��̔��a�𑫂����l��菬�����ꍇ�͓������Ă���
	Sa.x = Sphere1CenterPos.x - Sphere2CenterPos.x ;
	Sa.y = Sphere1CenterPos.y - Sphere2CenterPos.y ;
	Sa.z = Sphere1CenterPos.z - Sphere2CenterPos.z ;
	return Sa.x * Sa.x + Sa.y * Sa.y + Sa.z * Sa.z <= ( Sphere1R + Sphere2R ) * ( Sphere1R + Sphere2R ) ? TRUE : FALSE ;
}

// ���ƃJ�v�Z���̓����蔻��( TRUE:�������Ă���  FALSE:�������Ă��Ȃ� )
extern int HitCheck_Sphere_Capsule( VECTOR SphereCenterPos, float SphereR, VECTOR CapPos1, VECTOR CapPos2, float CapR )
{
	return Segment_Point_MinLength_Square( CapPos1, CapPos2, SphereCenterPos ) <= ( CapR + SphereR ) * ( CapR + SphereR ) ? TRUE : FALSE ;
}

// ���ƃJ�v�Z���̓����蔻��( TRUE:�������Ă���  FALSE:�������Ă��Ȃ� )
extern int HitCheck_Sphere_CapsuleD( VECTOR_D SphereCenterPos, double SphereR, VECTOR_D CapPos1, VECTOR_D CapPos2, double CapR )
{
	return Segment_Point_MinLength_SquareD( CapPos1, CapPos2, SphereCenterPos ) <= ( CapR + SphereR ) * ( CapR + SphereR ) ? TRUE : FALSE ;
}

// ���ƎO�p�`�̓����蔻��( TRUE:�������Ă���  FALSE:�������Ă��Ȃ� )
extern int HitCheck_Sphere_Triangle( VECTOR SphereCenterPos, float SphereR, VECTOR TrianglePos1, VECTOR TrianglePos2, VECTOR TrianglePos3 )
{
	VECTOR MinPosition, Sa ;

	// ���ƎO�p�`�̍ŋߓ_���擾����
	MinPosition = Get_Triangle_Point_MinPosition( SphereCenterPos, TrianglePos1, TrianglePos2, TrianglePos3 ) ;

	// �ŋߓ_�Ƃ̋��������̔��a���߂����ǂ����œ������Ă��邩�ǂ����𔻒�
	Sa.x = MinPosition.x - SphereCenterPos.x ;
	Sa.y = MinPosition.y - SphereCenterPos.y ;
	Sa.z = MinPosition.z - SphereCenterPos.z ;
	return Sa.x * Sa.x + Sa.y * Sa.y + Sa.z * Sa.z <= SphereR * SphereR ? TRUE : FALSE ;
}

// ���ƎO�p�`�̓����蔻��( TRUE:�������Ă���  FALSE:�������Ă��Ȃ� )
extern int HitCheck_Sphere_TriangleD( VECTOR_D SphereCenterPos, double SphereR, VECTOR_D TrianglePos1, VECTOR_D TrianglePos2, VECTOR_D TrianglePos3 )
{
	VECTOR_D MinPosition, Sa ;

	// ���ƎO�p�`�̍ŋߓ_���擾����
	MinPosition = Get_Triangle_Point_MinPositionD( SphereCenterPos, TrianglePos1, TrianglePos2, TrianglePos3 ) ;

	// �ŋߓ_�Ƃ̋��������̔��a���߂����ǂ����œ������Ă��邩�ǂ����𔻒�
	Sa.x = MinPosition.x - SphereCenterPos.x ;
	Sa.y = MinPosition.y - SphereCenterPos.y ;
	Sa.z = MinPosition.z - SphereCenterPos.z ;
	return Sa.x * Sa.x + Sa.y * Sa.y + Sa.z * Sa.z <= SphereR * SphereR ? TRUE : FALSE ;
}

// �J�v�Z�����m�̓����蔻��( TRUE:�������Ă���  FALSE:�������Ă��Ȃ� )
extern int HitCheck_Capsule_Capsule( VECTOR Cap1Pos1, VECTOR Cap1Pos2, float Cap1R, VECTOR Cap2Pos1, VECTOR Cap2Pos2, float Cap2R )
{
	float MinLength ;

	MinLength = Segment_Segment_MinLength_Square( Cap1Pos1, Cap1Pos2, Cap2Pos1, Cap2Pos2 ) ;
	return MinLength < ( Cap1R + Cap2R ) * ( Cap1R + Cap2R ) ? TRUE : FALSE ;
}

// �J�v�Z�����m�̓����蔻��( TRUE:�������Ă���  FALSE:�������Ă��Ȃ� )
extern int HitCheck_Capsule_CapsuleD( VECTOR_D Cap1Pos1, VECTOR_D Cap1Pos2, double Cap1R, VECTOR_D Cap2Pos1, VECTOR_D Cap2Pos2, double Cap2R )
{
	double MinLength ;

	MinLength = Segment_Segment_MinLength_SquareD( Cap1Pos1, Cap1Pos2, Cap2Pos1, Cap2Pos2 ) ;
	return MinLength < ( Cap1R + Cap2R ) * ( Cap1R + Cap2R ) ? TRUE : FALSE ;
}

// �J�v�Z���ƎO�p�`�̓����蔻��( TRUE:�������Ă���  FALSE:�������Ă��Ȃ� )
extern int HitCheck_Capsule_Triangle( VECTOR CapPos1, VECTOR CapPos2, float CapR, VECTOR TrianglePos1, VECTOR TrianglePos2, VECTOR TrianglePos3 )
{
	float MinLength ;

	MinLength = Segment_Triangle_MinLength_Square( CapPos1, CapPos2, TrianglePos1, TrianglePos2, TrianglePos3 ) ;
	return MinLength < CapR * CapR ? TRUE : FALSE ;
}

// �J�v�Z���ƎO�p�`�̓����蔻��( TRUE:�������Ă���  FALSE:�������Ă��Ȃ� )
extern int HitCheck_Capsule_TriangleD( VECTOR_D CapPos1, VECTOR_D CapPos2, double CapR, VECTOR_D TrianglePos1, VECTOR_D TrianglePos2, VECTOR_D TrianglePos3 )
{
	double MinLength ;

	MinLength = Segment_Triangle_MinLength_SquareD( CapPos1, CapPos2, TrianglePos1, TrianglePos2, TrianglePos3 ) ;
	return MinLength < CapR * CapR ? TRUE : FALSE ;
}

// ��`�̃N���b�s���O
extern int RectClipping( RECT *Rect, const RECT *ClippuRect )
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

// ��`�� left �� right ���l���傫���ꍇ�Ȃǂ̌���␳����
extern int RectAdjust( RECT *Rect )
{
	int Temp;

	if( Rect->left > Rect->right  ){ Temp = Rect->left; Rect->left = Rect->right;  Rect->right  = Temp; }
	if( Rect->top  > Rect->bottom ){ Temp = Rect->top ; Rect->top  = Rect->bottom; Rect->bottom = Temp; }

	return 0;
}

// ��`�̕��ƍ��������߂�
extern int GetRectSize( const RECT *Rect, int *Width, int *Height )
{
	if( Width  ) *Width  = Rect->right  - Rect->left;
	if( Height ) *Height = Rect->bottom - Rect->top;

	return 0;
}

// �P�ʍs��𓾂�
extern MATRIX MGetIdent( void )
{
	static MATRIX Result = 
	{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f 
	} ;
	return Result ;
}

// �P�ʍs��𓾂�
extern MATRIX_D MGetIdentD( void )
{
	static MATRIX_D Result = 
	{
		1.0, 0.0, 0.0, 0.0,
		0.0, 1.0, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		0.0, 0.0, 0.0, 1.0 
	} ;
	return Result ;
}

// �s��̏�Z���s��
extern MATRIX MMult( MATRIX In1, MATRIX In2 )
{
	MATRIX Result =
	{
		In1.m[0][0] * In2.m[0][0] + In1.m[0][1] * In2.m[1][0] + In1.m[0][2] * In2.m[2][0] + In1.m[0][3] * In2.m[3][0],
		In1.m[0][0] * In2.m[0][1] + In1.m[0][1] * In2.m[1][1] + In1.m[0][2] * In2.m[2][1] + In1.m[0][3] * In2.m[3][1],
		In1.m[0][0] * In2.m[0][2] + In1.m[0][1] * In2.m[1][2] + In1.m[0][2] * In2.m[2][2] + In1.m[0][3] * In2.m[3][2],
		In1.m[0][0] * In2.m[0][3] + In1.m[0][1] * In2.m[1][3] + In1.m[0][2] * In2.m[2][3] + In1.m[0][3] * In2.m[3][3],

		In1.m[1][0] * In2.m[0][0] + In1.m[1][1] * In2.m[1][0] + In1.m[1][2] * In2.m[2][0] + In1.m[1][3] * In2.m[3][0],
		In1.m[1][0] * In2.m[0][1] + In1.m[1][1] * In2.m[1][1] + In1.m[1][2] * In2.m[2][1] + In1.m[1][3] * In2.m[3][1],
		In1.m[1][0] * In2.m[0][2] + In1.m[1][1] * In2.m[1][2] + In1.m[1][2] * In2.m[2][2] + In1.m[1][3] * In2.m[3][2],
		In1.m[1][0] * In2.m[0][3] + In1.m[1][1] * In2.m[1][3] + In1.m[1][2] * In2.m[2][3] + In1.m[1][3] * In2.m[3][3],

		In1.m[2][0] * In2.m[0][0] + In1.m[2][1] * In2.m[1][0] + In1.m[2][2] * In2.m[2][0] + In1.m[2][3] * In2.m[3][0],
		In1.m[2][0] * In2.m[0][1] + In1.m[2][1] * In2.m[1][1] + In1.m[2][2] * In2.m[2][1] + In1.m[2][3] * In2.m[3][1],
		In1.m[2][0] * In2.m[0][2] + In1.m[2][1] * In2.m[1][2] + In1.m[2][2] * In2.m[2][2] + In1.m[2][3] * In2.m[3][2],
		In1.m[2][0] * In2.m[0][3] + In1.m[2][1] * In2.m[1][3] + In1.m[2][2] * In2.m[2][3] + In1.m[2][3] * In2.m[3][3],

		In1.m[3][0] * In2.m[0][0] + In1.m[3][1] * In2.m[1][0] + In1.m[3][2] * In2.m[2][0] + In1.m[3][3] * In2.m[3][0],
		In1.m[3][0] * In2.m[0][1] + In1.m[3][1] * In2.m[1][1] + In1.m[3][2] * In2.m[2][1] + In1.m[3][3] * In2.m[3][1],
		In1.m[3][0] * In2.m[0][2] + In1.m[3][1] * In2.m[1][2] + In1.m[3][2] * In2.m[2][2] + In1.m[3][3] * In2.m[3][2],
		In1.m[3][0] * In2.m[0][3] + In1.m[3][1] * In2.m[1][3] + In1.m[3][2] * In2.m[2][3] + In1.m[3][3] * In2.m[3][3]
	} ;
	return Result ;
}

// �s��̏�Z���s��
extern MATRIX_D MMultD( MATRIX_D In1, MATRIX_D In2 )
{
	MATRIX_D Result =
	{
		In1.m[0][0] * In2.m[0][0] + In1.m[0][1] * In2.m[1][0] + In1.m[0][2] * In2.m[2][0] + In1.m[0][3] * In2.m[3][0],
		In1.m[0][0] * In2.m[0][1] + In1.m[0][1] * In2.m[1][1] + In1.m[0][2] * In2.m[2][1] + In1.m[0][3] * In2.m[3][1],
		In1.m[0][0] * In2.m[0][2] + In1.m[0][1] * In2.m[1][2] + In1.m[0][2] * In2.m[2][2] + In1.m[0][3] * In2.m[3][2],
		In1.m[0][0] * In2.m[0][3] + In1.m[0][1] * In2.m[1][3] + In1.m[0][2] * In2.m[2][3] + In1.m[0][3] * In2.m[3][3],

		In1.m[1][0] * In2.m[0][0] + In1.m[1][1] * In2.m[1][0] + In1.m[1][2] * In2.m[2][0] + In1.m[1][3] * In2.m[3][0],
		In1.m[1][0] * In2.m[0][1] + In1.m[1][1] * In2.m[1][1] + In1.m[1][2] * In2.m[2][1] + In1.m[1][3] * In2.m[3][1],
		In1.m[1][0] * In2.m[0][2] + In1.m[1][1] * In2.m[1][2] + In1.m[1][2] * In2.m[2][2] + In1.m[1][3] * In2.m[3][2],
		In1.m[1][0] * In2.m[0][3] + In1.m[1][1] * In2.m[1][3] + In1.m[1][2] * In2.m[2][3] + In1.m[1][3] * In2.m[3][3],

		In1.m[2][0] * In2.m[0][0] + In1.m[2][1] * In2.m[1][0] + In1.m[2][2] * In2.m[2][0] + In1.m[2][3] * In2.m[3][0],
		In1.m[2][0] * In2.m[0][1] + In1.m[2][1] * In2.m[1][1] + In1.m[2][2] * In2.m[2][1] + In1.m[2][3] * In2.m[3][1],
		In1.m[2][0] * In2.m[0][2] + In1.m[2][1] * In2.m[1][2] + In1.m[2][2] * In2.m[2][2] + In1.m[2][3] * In2.m[3][2],
		In1.m[2][0] * In2.m[0][3] + In1.m[2][1] * In2.m[1][3] + In1.m[2][2] * In2.m[2][3] + In1.m[2][3] * In2.m[3][3],

		In1.m[3][0] * In2.m[0][0] + In1.m[3][1] * In2.m[1][0] + In1.m[3][2] * In2.m[2][0] + In1.m[3][3] * In2.m[3][0],
		In1.m[3][0] * In2.m[0][1] + In1.m[3][1] * In2.m[1][1] + In1.m[3][2] * In2.m[2][1] + In1.m[3][3] * In2.m[3][1],
		In1.m[3][0] * In2.m[0][2] + In1.m[3][1] * In2.m[1][2] + In1.m[3][2] * In2.m[2][2] + In1.m[3][3] * In2.m[3][2],
		In1.m[3][0] * In2.m[0][3] + In1.m[3][1] * In2.m[1][3] + In1.m[3][2] * In2.m[2][3] + In1.m[3][3] * In2.m[3][3]
	} ;
	return Result ;
}

// �s��̃X�P�[�����O���s��
extern MATRIX MScale( MATRIX InM, float Scale )
{
	MATRIX Result =
	{
		InM.m[0][0] * Scale, InM.m[0][1] * Scale, InM.m[0][2] * Scale, InM.m[0][3] * Scale, 
		InM.m[1][0] * Scale, InM.m[1][1] * Scale, InM.m[1][2] * Scale, InM.m[1][3] * Scale, 
		InM.m[2][0] * Scale, InM.m[2][1] * Scale, InM.m[2][2] * Scale, InM.m[2][3] * Scale, 
		InM.m[3][0] * Scale, InM.m[3][1] * Scale, InM.m[3][2] * Scale, InM.m[3][3] * Scale
	} ;
	return Result ;
}

// �s��̃X�P�[�����O���s��
extern MATRIX_D MScaleD( MATRIX_D InM, double Scale )
{
	MATRIX_D Result =
	{
		InM.m[0][0] * Scale, InM.m[0][1] * Scale, InM.m[0][2] * Scale, InM.m[0][3] * Scale, 
		InM.m[1][0] * Scale, InM.m[1][1] * Scale, InM.m[1][2] * Scale, InM.m[1][3] * Scale, 
		InM.m[2][0] * Scale, InM.m[2][1] * Scale, InM.m[2][2] * Scale, InM.m[2][3] * Scale, 
		InM.m[3][0] * Scale, InM.m[3][1] * Scale, InM.m[3][2] * Scale, InM.m[3][3] * Scale
	} ;
	return Result ;
}

// �s��̑����Z���s��
extern MATRIX MAdd( MATRIX In1, MATRIX In2 )
{
	MATRIX Result =
	{
		In1.m[0][0] + In2.m[0][0], In1.m[0][1] + In2.m[0][1], In1.m[0][2] + In2.m[0][2], In1.m[0][3] + In2.m[0][3], 
		In1.m[1][0] + In2.m[1][0], In1.m[1][1] + In2.m[1][1], In1.m[1][2] + In2.m[1][2], In1.m[1][3] + In2.m[1][3], 
		In1.m[2][0] + In2.m[2][0], In1.m[2][1] + In2.m[2][1], In1.m[2][2] + In2.m[2][2], In1.m[2][3] + In2.m[2][3], 
		In1.m[3][0] + In2.m[3][0], In1.m[3][1] + In2.m[3][1], In1.m[3][2] + In2.m[3][2], In1.m[3][3] + In2.m[3][3]
	} ;
	return Result ;
}

// �s��̑����Z���s��
extern MATRIX_D MAddD( MATRIX_D In1, MATRIX_D In2 )
{
	MATRIX_D Result =
	{
		In1.m[0][0] + In2.m[0][0], In1.m[0][1] + In2.m[0][1], In1.m[0][2] + In2.m[0][2], In1.m[0][3] + In2.m[0][3], 
		In1.m[1][0] + In2.m[1][0], In1.m[1][1] + In2.m[1][1], In1.m[1][2] + In2.m[1][2], In1.m[1][3] + In2.m[1][3], 
		In1.m[2][0] + In2.m[2][0], In1.m[2][1] + In2.m[2][1], In1.m[2][2] + In2.m[2][2], In1.m[2][3] + In2.m[2][3], 
		In1.m[3][0] + In2.m[3][0], In1.m[3][1] + In2.m[3][1], In1.m[3][2] + In2.m[3][2], In1.m[3][3] + In2.m[3][3]
	} ;
	return Result ;
}

// �g��s��𓾂�
extern MATRIX MGetScale( VECTOR Scale )
{
	MATRIX Result =
	{
		Scale.x,    0.0f,    0.0f, 0.0f,
		   0.0f, Scale.y,    0.0f, 0.0f,
		   0.0f,    0.0f, Scale.z, 0.0f,
		   0.0f,    0.0f,    0.0f, 1.0f
	} ;
	return Result ;
}

// �g��s��𓾂�
extern MATRIX_D MGetScaleD( VECTOR_D Scale )
{
	MATRIX_D Result =
	{
		Scale.x,     0.0,     0.0, 0.0,
		    0.0, Scale.y,     0.0, 0.0,
		    0.0,     0.0, Scale.z, 0.0,
		    0.0,     0.0,     0.0, 1.0
	} ;
	return Result ;
}

// �w����]�s��𓾂�
extern MATRIX MGetRotX( float XAxisRotate )
{
	float Sin, Cos ;
	_SINCOS_PLATFORM( XAxisRotate, &Sin, &Cos ) ;

	MATRIX Result =
	{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f,  Cos,  Sin, 0.0f,
		0.0f, -Sin,  Cos, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	} ;
	return Result ;
}

// �w����]�s��𓾂�
extern MATRIX_D MGetRotXD( double XAxisRotate )
{
	double Sin, Cos ;
	_SINCOSD( XAxisRotate, &Sin, &Cos ) ;

	MATRIX_D Result =
	{
		1.0,  0.0,  0.0, 0.0,
		0.0,  Cos,  Sin, 0.0,
		0.0, -Sin,  Cos, 0.0,
		0.0,  0.0,  0.0, 1.0
	} ;
	return Result ;
}

// �x����]�s��𓾂�
extern MATRIX MGetRotY( float YAxisRotate )
{
	float Sin, Cos ;
	_SINCOS_PLATFORM( YAxisRotate, &Sin, &Cos ) ;

	MATRIX Result =
	{
		 Cos, 0.0f, -Sin, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		 Sin, 0.0f,  Cos, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	} ;
	return Result ;
}

// �x����]�s��𓾂�
extern MATRIX_D MGetRotYD( double YAxisRotate )
{
	double Sin, Cos ;
	_SINCOSD( YAxisRotate, &Sin, &Cos ) ;

	MATRIX_D Result =
	{
		 Cos, 0.0, -Sin, 0.0,
		 0.0, 1.0,  0.0, 0.0,
		 Sin, 0.0,  Cos, 0.0,
		 0.0, 0.0,  0.0, 1.0
	} ;
	return Result ;
}

// �y����]�s��𓾂�
extern MATRIX MGetRotZ( float ZAxisRotate )
{
	float Sin, Cos ;
	_SINCOS_PLATFORM( ZAxisRotate, &Sin, &Cos ) ;

	MATRIX Result =
	{
		 Cos,  Sin, 0.0f, 0.0f,
		-Sin,  Cos, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	} ;
	return Result ;
}

// �y����]�s��𓾂�
extern MATRIX_D MGetRotZD( double ZAxisRotate )
{
	double Sin, Cos ;
	_SINCOSD( ZAxisRotate, &Sin, &Cos ) ;

	MATRIX_D Result =
	{
		 Cos,  Sin, 0.0, 0.0,
		-Sin,  Cos, 0.0, 0.0,
		 0.0,  0.0, 1.0, 0.0,
		 0.0,  0.0, 0.0, 1.0
	} ;
	return Result ;
}

// �w�莲�Ŏw��p�x��]����s��𓾂�
extern MATRIX MGetRotAxis( VECTOR RotateAxis, float Rotate )
{
	MATRIX Result ;
	VECTOR xv = { 1.0f, 0.0f, 0.0f }, yv = { 0.0f, 1.0f, 0.0f }, zv = { 0.0f, 0.0f, 1.0f } ;
	VECTOR xv2, yv2, zv2, xv3, yv3, zv3 ;
	float f, Sin, Cos ;

	zv2 = VNorm( RotateAxis ) ;
	yv2 = VGet( 0.0f, 1.0f, 0.0f ) ;
	if( VSquareSize( VCross( yv2, zv2 ) ) < 0.00001f )
		yv2 = VGet( 0.0f, 0.0f, 1.0f ) ;
	xv2 = VCross( zv2, yv2 ) ;
	yv2 = VNorm( VCross( xv2, zv2 ) ) ;
	xv2 = VNorm( xv2 ) ;

	xv3.x = xv2.x * xv.x + xv2.y * xv.y + xv2.z * xv.z ;
	xv3.y = yv2.x * xv.x + yv2.y * xv.y + yv2.z * xv.z ;
	xv3.z = zv2.x * xv.x + zv2.y * xv.y + zv2.z * xv.z ;

	yv3.x = xv2.x * yv.x + xv2.y * yv.y + xv2.z * yv.z ;
	yv3.y = yv2.x * yv.x + yv2.y * yv.y + yv2.z * yv.z ;
	yv3.z = zv2.x * yv.x + zv2.y * yv.y + zv2.z * yv.z ;

	zv3.x = xv2.x * zv.x + xv2.y * zv.y + xv2.z * zv.z ;
	zv3.y = yv2.x * zv.x + yv2.y * zv.y + yv2.z * zv.z ;
	zv3.z = zv2.x * zv.x + zv2.y * zv.y + zv2.z * zv.z ;

	_SINCOS_PLATFORM( Rotate, &Sin, &Cos ) ;
	f     = xv3.x * Cos - xv3.y * Sin ;
	xv3.y = xv3.x * Sin + xv3.y * Cos ;
	xv3.x = f ;

	f     = yv3.x * Cos - yv3.y * Sin ;
	yv3.y = yv3.x * Sin + yv3.y * Cos ;
	yv3.x = f ;

	f     = zv3.x * Cos - zv3.y * Sin ;
	zv3.y = zv3.x * Sin + zv3.y * Cos ;
	zv3.x = f ;

	xv.x = xv2.x * xv3.x + yv2.x * xv3.y + zv2.x * xv3.z ;
	xv.y = xv2.y * xv3.x + yv2.y * xv3.y + zv2.y * xv3.z ;
	xv.z = xv2.z * xv3.x + yv2.z * xv3.y + zv2.z * xv3.z ;

	yv.x = xv2.x * yv3.x + yv2.x * yv3.y + zv2.x * yv3.z ;
	yv.y = xv2.y * yv3.x + yv2.y * yv3.y + zv2.y * yv3.z ;
	yv.z = xv2.z * yv3.x + yv2.z * yv3.y + zv2.z * yv3.z ;

	zv.x = xv2.x * zv3.x + yv2.x * zv3.y + zv2.x * zv3.z ;
	zv.y = xv2.y * zv3.x + yv2.y * zv3.y + zv2.y * zv3.z ;
	zv.z = xv2.z * zv3.x + yv2.z * zv3.y + zv2.z * zv3.z ;

	Result.m[ 0 ][ 0 ] = xv.x ;
	Result.m[ 1 ][ 0 ] = xv.y ;
	Result.m[ 2 ][ 0 ] = xv.z ;
	Result.m[ 3 ][ 0 ] = 0.0f ;

	Result.m[ 0 ][ 1 ] = yv.x ;
	Result.m[ 1 ][ 1 ] = yv.y ;
	Result.m[ 2 ][ 1 ] = yv.z ;
	Result.m[ 3 ][ 1 ] = 0.0f ;

	Result.m[ 0 ][ 2 ] = zv.x ;
	Result.m[ 1 ][ 2 ] = zv.y ;
	Result.m[ 2 ][ 2 ] = zv.z ;
	Result.m[ 3 ][ 2 ] = 0.0f ;

	Result.m[ 0 ][ 3 ] = 0.0f ;
	Result.m[ 1 ][ 3 ] = 0.0f ;
	Result.m[ 2 ][ 3 ] = 0.0f ;
	Result.m[ 3 ][ 3 ] = 1.0f ;

	return Result ;
}

// �w�莲�Ŏw��p�x��]����s��𓾂�
extern MATRIX_D MGetRotAxisD( VECTOR_D RotateAxis, double Rotate )
{
	MATRIX_D Result ;
	VECTOR_D xv = { 1.0, 0.0, 0.0 }, yv = { 0.0, 1.0, 0.0 }, zv = { 0.0, 0.0, 1.0 } ;
	VECTOR_D xv2, yv2, zv2, xv3, yv3, zv3 ;
	double f, Sin, Cos ;

	zv2 = VNormD( RotateAxis ) ;
	yv2 = VGetD( 0.0, 1.0, 0.0 ) ;
	if( VSquareSizeD( VCrossD( yv2, zv2 ) ) < 0.00001 )
		yv2 = VGetD( 0.0, 0.0, 1.0 ) ;
	xv2 = VCrossD( zv2, yv2 ) ;
	yv2 = VNormD( VCrossD( xv2, zv2 ) ) ;
	xv2 = VNormD( xv2 ) ;

	xv3.x = xv2.x * xv.x + xv2.y * xv.y + xv2.z * xv.z ;
	xv3.y = yv2.x * xv.x + yv2.y * xv.y + yv2.z * xv.z ;
	xv3.z = zv2.x * xv.x + zv2.y * xv.y + zv2.z * xv.z ;

	yv3.x = xv2.x * yv.x + xv2.y * yv.y + xv2.z * yv.z ;
	yv3.y = yv2.x * yv.x + yv2.y * yv.y + yv2.z * yv.z ;
	yv3.z = zv2.x * yv.x + zv2.y * yv.y + zv2.z * yv.z ;

	zv3.x = xv2.x * zv.x + xv2.y * zv.y + xv2.z * zv.z ;
	zv3.y = yv2.x * zv.x + yv2.y * zv.y + yv2.z * zv.z ;
	zv3.z = zv2.x * zv.x + zv2.y * zv.y + zv2.z * zv.z ;

	_SINCOSD( Rotate, &Sin, &Cos ) ;
	f     = xv3.x * Cos - xv3.y * Sin ;
	xv3.y = xv3.x * Sin + xv3.y * Cos ;
	xv3.x = f ;

	f     = yv3.x * Cos - yv3.y * Sin ;
	yv3.y = yv3.x * Sin + yv3.y * Cos ;
	yv3.x = f ;

	f     = zv3.x * Cos - zv3.y * Sin ;
	zv3.y = zv3.x * Sin + zv3.y * Cos ;
	zv3.x = f ;

	xv.x = xv2.x * xv3.x + yv2.x * xv3.y + zv2.x * xv3.z ;
	xv.y = xv2.y * xv3.x + yv2.y * xv3.y + zv2.y * xv3.z ;
	xv.z = xv2.z * xv3.x + yv2.z * xv3.y + zv2.z * xv3.z ;

	yv.x = xv2.x * yv3.x + yv2.x * yv3.y + zv2.x * yv3.z ;
	yv.y = xv2.y * yv3.x + yv2.y * yv3.y + zv2.y * yv3.z ;
	yv.z = xv2.z * yv3.x + yv2.z * yv3.y + zv2.z * yv3.z ;

	zv.x = xv2.x * zv3.x + yv2.x * zv3.y + zv2.x * zv3.z ;
	zv.y = xv2.y * zv3.x + yv2.y * zv3.y + zv2.y * zv3.z ;
	zv.z = xv2.z * zv3.x + yv2.z * zv3.y + zv2.z * zv3.z ;

	Result.m[ 0 ][ 0 ] = xv.x ;
	Result.m[ 1 ][ 0 ] = xv.y ;
	Result.m[ 2 ][ 0 ] = xv.z ;
	Result.m[ 3 ][ 0 ] = 0.0 ;

	Result.m[ 0 ][ 1 ] = yv.x ;
	Result.m[ 1 ][ 1 ] = yv.y ;
	Result.m[ 2 ][ 1 ] = yv.z ;
	Result.m[ 3 ][ 1 ] = 0.0 ;

	Result.m[ 0 ][ 2 ] = zv.x ;
	Result.m[ 1 ][ 2 ] = zv.y ;
	Result.m[ 2 ][ 2 ] = zv.z ;
	Result.m[ 3 ][ 2 ] = 0.0 ;

	Result.m[ 0 ][ 3 ] = 0.0 ;
	Result.m[ 1 ][ 3 ] = 0.0 ;
	Result.m[ 2 ][ 3 ] = 0.0 ;
	Result.m[ 3 ][ 3 ] = 1.0 ;

	return Result ;
}

// In1 �̌������� In2 �̌����֕ϊ������]�s��𓾂�
extern MATRIX MGetRotVec2( VECTOR In1, VECTOR In2 )
{
	VECTOR av ;
	float rad ;

	av = VCross( In1, In2 ) ;
	if( VSquareSize( av ) < 0.0000001f )
		av = VGet( 1.0f, 0.0f, 0.0f ) ;
	rad = VRad( In1, In2 ) ;
	return MGetRotAxis( av, rad ) ;
}

// In1 �̌������� In2 �̌����֕ϊ������]�s��𓾂�
extern MATRIX_D MGetRotVec2D( VECTOR_D In1, VECTOR_D In2 )
{
	VECTOR_D av ;
	double rad ;

	av = VCrossD( In1, In2 ) ;
	if( VSquareSizeD( av ) < 0.0000001 )
		av = VGetD( 1.0, 0.0, 0.0 ) ;
	rad = VRadD( In1, In2 ) ;
	return MGetRotAxisD( av, rad ) ;
}

// ���s�ړ��s��𓾂�
extern MATRIX MGetTranslate( VECTOR Trans )
{
	MATRIX Result =
	{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		Trans.x, Trans.y, Trans.z, 1.0f
	} ;

	return Result ;
}

// ���s�ړ��s��𓾂�
extern MATRIX_D MGetTranslateD( VECTOR_D Trans )
{
	MATRIX_D Result =
	{
		1.0, 0.0, 0.0, 0.0,
		0.0, 1.0, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		Trans.x, Trans.y, Trans.z, 1.0
	} ;

	return Result ;
}

// �w��̂R�����[�J���̃x�N�g������{����̃x�N�g���ɕϊ�����s��𓾂�
// x' = XAxis.x * x + YAixs.x * y + ZAxis.z * z + Pos.x
// y' = XAxis.y * x + YAixs.y * y + ZAxis.y * z + Pos.y
// z' = XAxis.z * x + YAixs.z * y + ZAxis.z * z + Pos.z
extern MATRIX MGetAxis1( VECTOR XAxis, VECTOR YAxis, VECTOR ZAxis, VECTOR Pos )
{
	MATRIX Result =
	{
		XAxis.x, XAxis.y, XAxis.z, 0.0f,
		YAxis.x, YAxis.y, YAxis.z, 0.0f,
		ZAxis.x, ZAxis.y, ZAxis.z, 0.0f,
		Pos.x,   Pos.y,   Pos.z,   1.0f,
	} ;
	return Result ;
}

// �w��̂R�����[�J���̃x�N�g������{����̃x�N�g���ɕϊ�����s��𓾂�
// x' = XAxis.x * x + YAixs.x * y + ZAxis.z * z + Pos.x
// y' = XAxis.y * x + YAixs.y * y + ZAxis.y * z + Pos.y
// z' = XAxis.z * x + YAixs.z * y + ZAxis.z * z + Pos.z
extern MATRIX_D MGetAxis1D( VECTOR_D XAxis, VECTOR_D YAxis, VECTOR_D ZAxis, VECTOR_D Pos )
{
	MATRIX_D Result =
	{
		XAxis.x, XAxis.y, XAxis.z, 0.0,
		YAxis.x, YAxis.y, YAxis.z, 0.0,
		ZAxis.x, ZAxis.y, ZAxis.z, 0.0,
		Pos.x,   Pos.y,   Pos.z,   1.0,
	} ;
	return Result ;
}

// ��{����̃x�N�g�����w��̂R����ɓ��e�����x�N�g���ɕϊ�����s��𓾂�
// x' = XAxis.x * ( x - Pos.x ) + XAxis.y * ( x - Pos.x ) + XAxis.z * ( x - Pos.x )
// y' = YAxis.x * ( x - Pos.x ) + YAxis.y * ( x - Pos.x ) + YAxis.z * ( x - Pos.x )
// z' = ZAxis.x * ( x - Pos.x ) + ZAxis.y * ( x - Pos.x ) + ZAxis.z * ( x - Pos.x )
extern MATRIX MGetAxis2( VECTOR XAxis, VECTOR YAxis, VECTOR ZAxis, VECTOR Pos )
{
	MATRIX Result =
	{
		XAxis.x, YAxis.x, ZAxis.x, 0.0f,
		XAxis.y, YAxis.y, ZAxis.y, 0.0f,
		XAxis.z, YAxis.z, ZAxis.z, 0.0f,
		-( Pos.x * XAxis.x + Pos.y * XAxis.y + Pos.z * XAxis.z ),
		-( Pos.x * YAxis.x + Pos.y * YAxis.y + Pos.z * YAxis.z ),
		-( Pos.x * ZAxis.x + Pos.y * ZAxis.y + Pos.z * ZAxis.z ), 1.0f
	} ;
	return Result ;
}

// ��{����̃x�N�g�����w��̂R����ɓ��e�����x�N�g���ɕϊ�����s��𓾂�
// x' = XAxis.x * ( x - Pos.x ) + XAxis.y * ( x - Pos.x ) + XAxis.z * ( x - Pos.x )
// y' = YAxis.x * ( x - Pos.x ) + YAxis.y * ( x - Pos.x ) + YAxis.z * ( x - Pos.x )
// z' = ZAxis.x * ( x - Pos.x ) + ZAxis.y * ( x - Pos.x ) + ZAxis.z * ( x - Pos.x )
extern MATRIX_D MGetAxis2D( VECTOR_D XAxis, VECTOR_D YAxis, VECTOR_D ZAxis, VECTOR_D Pos )
{
	MATRIX_D Result =
	{
		XAxis.x, YAxis.x, ZAxis.x, 0.0,
		XAxis.y, YAxis.y, ZAxis.y, 0.0,
		XAxis.z, YAxis.z, ZAxis.z, 0.0,
		-( Pos.x * XAxis.x + Pos.y * XAxis.y + Pos.z * XAxis.z ),
		-( Pos.x * YAxis.x + Pos.y * YAxis.y + Pos.z * YAxis.z ),
		-( Pos.x * ZAxis.x + Pos.y * ZAxis.y + Pos.z * ZAxis.z ), 1.0
	} ;
	return Result ;
}

// �]�u�s��𓾂�
extern MATRIX MTranspose( MATRIX InM )
{
	MATRIX Result =
	{
		InM.m[ 0 ][ 0 ], InM.m[ 1 ][ 0 ], InM.m[ 2 ][ 0 ], InM.m[ 3 ][ 0 ], 
		InM.m[ 0 ][ 1 ], InM.m[ 1 ][ 1 ], InM.m[ 2 ][ 1 ], InM.m[ 3 ][ 1 ], 
		InM.m[ 0 ][ 2 ], InM.m[ 1 ][ 2 ], InM.m[ 2 ][ 2 ], InM.m[ 3 ][ 2 ], 
		InM.m[ 0 ][ 3 ], InM.m[ 1 ][ 3 ], InM.m[ 2 ][ 3 ], InM.m[ 3 ][ 3 ]
	} ;
	return Result ;
}

// �]�u�s��𓾂�
extern MATRIX_D MTransposeD( MATRIX_D InM )
{
	MATRIX_D Result =
	{
		InM.m[ 0 ][ 0 ], InM.m[ 1 ][ 0 ], InM.m[ 2 ][ 0 ], InM.m[ 3 ][ 0 ], 
		InM.m[ 0 ][ 1 ], InM.m[ 1 ][ 1 ], InM.m[ 2 ][ 1 ], InM.m[ 3 ][ 1 ], 
		InM.m[ 0 ][ 2 ], InM.m[ 1 ][ 2 ], InM.m[ 2 ][ 2 ], InM.m[ 3 ][ 2 ], 
		InM.m[ 0 ][ 3 ], InM.m[ 1 ][ 3 ], InM.m[ 2 ][ 3 ], InM.m[ 3 ][ 3 ]
	} ;
	return Result ;
}

// �t�s��𓾂�
extern MATRIX MInverse( MATRIX InM )
{
	MATRIX Result ;

	if( CreateInverseMatrix( &Result, &InM ) < 0 )
	{
		return MGetIdent() ;
	}
	
	return Result ;
}

// �t�s��𓾂�
extern MATRIX_D MInverseD( MATRIX_D InM )
{
	MATRIX_D Result ;

	if( CreateInverseMatrixD( &Result, &InM ) < 0 )
	{
		return MGetIdentD() ;
	}
	
	return Result ;
}

// �g��s��̂w���A�x���A�y���̊g�嗦�𓾂�
extern VECTOR MGetSize( MATRIX InM )
{
	VECTOR Result ;

	Result.x = _SQRT( InM.m[ 0 ][ 0 ] * InM.m[ 0 ][ 0 ] + InM.m[ 1 ][ 0 ] * InM.m[ 1 ][ 0 ] + InM.m[ 2 ][ 0 ] * InM.m[ 2 ][ 0 ] ) ;
	Result.y = _SQRT( InM.m[ 0 ][ 1 ] * InM.m[ 0 ][ 1 ] + InM.m[ 1 ][ 1 ] * InM.m[ 1 ][ 1 ] + InM.m[ 2 ][ 1 ] * InM.m[ 2 ][ 1 ] ) ;
	Result.z = _SQRT( InM.m[ 0 ][ 2 ] * InM.m[ 0 ][ 2 ] + InM.m[ 1 ][ 2 ] * InM.m[ 1 ][ 2 ] + InM.m[ 2 ][ 2 ] * InM.m[ 2 ][ 2 ] ) ;

	return Result ;
}

// �g��s��̂w���A�x���A�y���̊g�嗦�𓾂�
extern VECTOR_D MGetSizeD( MATRIX_D InM )
{
	VECTOR_D Result ;

	Result.x = _SQRTD( InM.m[ 0 ][ 0 ] * InM.m[ 0 ][ 0 ] + InM.m[ 1 ][ 0 ] * InM.m[ 1 ][ 0 ] + InM.m[ 2 ][ 0 ] * InM.m[ 2 ][ 0 ] ) ;
	Result.y = _SQRTD( InM.m[ 0 ][ 1 ] * InM.m[ 0 ][ 1 ] + InM.m[ 1 ][ 1 ] * InM.m[ 1 ][ 1 ] + InM.m[ 2 ][ 1 ] * InM.m[ 2 ][ 1 ] ) ;
	Result.z = _SQRTD( InM.m[ 0 ][ 2 ] * InM.m[ 0 ][ 2 ] + InM.m[ 1 ][ 2 ] * InM.m[ 1 ][ 2 ] + InM.m[ 2 ][ 2 ] * InM.m[ 2 ][ 2 ] ) ;

	return Result ;
}

#if 0

// �x�N�g���l�̐���
extern VECTOR VGet( float x, float y, float z )
{
	VECTOR Result = { x, y, z } ;
	return Result ;
}

// �x�N�g���̉��Z
extern VECTOR VAdd( VECTOR In1, VECTOR In2 )
{
	VECTOR Result = { In1.x + In2.x, In1.y + In2.y, In1.z + In2.z } ;
	return Result ;
}

// �x�N�g���̌��Z
extern VECTOR VSub( VECTOR In1, VECTOR In2 )
{
	VECTOR Result = { In1.x - In2.x, In1.y - In2.y, In1.z - In2.z } ;
	return Result ;
}

// �x�N�g���̓���
extern float VDot( VECTOR In1, VECTOR In2 )
{
	return In1.x * In2.x + In1.y * In2.y + In1.z * In2.z ;
}

// �x�N�g���̊O��
extern VECTOR VCross( VECTOR In1, VECTOR In2 )
{
	VECTOR Result =
	{
		In1.y * In2.z - In1.z * In2.y,
		In1.z * In2.x - In1.x * In2.z,
		In1.x * In2.y - In1.y * In2.x
	} ;
	return Result ;
}

// �x�N�g���̃X�P�[�����O
extern VECTOR VScale( VECTOR In, float Scale )
{
	VECTOR Result = { In.x * Scale, In.y * Scale, In.z * Scale } ;
	return Result ;
}

#endif

// �x�N�g���̐��K��
extern VECTOR VNorm( VECTOR In )
{
	float Square ;
	float Size ;

	Square = In.x * In.x + In.y * In.y + In.z * In.z ;
	if( Square < 0.0000001f )
	{
		return VGet( -1.0f, -1.0f, -1.0f ) ;
	}

	Size = _SQRT( Square ) ;
	VECTOR Result = { In.x / Size, In.y / Size, In.z / Size } ;
	return Result ;
}

// �x�N�g���̐��K��
extern VECTOR_D VNormD( VECTOR_D In )
{
	double Square ;
	double Size ;

	Square = In.x * In.x + In.y * In.y + In.z * In.z ;
	if( Square < 0.0000001 )
	{
		return VGetD( -1.0, -1.0, -1.0 ) ;
	}

	Size = _SQRTD( Square ) ;
	VECTOR_D Result = { In.x / Size, In.y / Size, In.z / Size } ;
	return Result ;
}

// �x�N�g���̃T�C�Y
extern float  VSize( VECTOR In )
{
	float Square ;
	Square = In.x * In.x + In.y * In.y + In.z * In.z ;
	return Square < 0.0000001f ? 0.0f : _SQRT( Square ) ;
}

// �x�N�g���̃T�C�Y
extern double  VSizeD( VECTOR_D In )
{
	double Square ;
	Square = In.x * In.x + In.y * In.y + In.z * In.z ;
	return Square < 0.0000001 ? 0.0 : _SQRTD( Square ) ;
}

#if 0

// �x�N�g���̃T�C�Y�̂Q��
extern float VSquareSize( VECTOR In )
{
	return In.x * In.x + In.y * In.y + In.z * In.z ;
}

// �s����g�������W�ϊ�
extern VECTOR VTransform( VECTOR InV, MATRIX InM )
{
	VECTOR Result =
	{
		InV.x * InM.m[0][0] + InV.y * InM.m[1][0] + InV.z * InM.m[2][0] + InM.m[3][0],
		InV.x * InM.m[0][1] + InV.y * InM.m[1][1] + InV.z * InM.m[2][1] + InM.m[3][1],
		InV.x * InM.m[0][2] + InV.y * InM.m[1][2] + InV.z * InM.m[2][2] + InM.m[3][2]
	} ;
	return Result ;
}

// �s����g�������W�ϊ�( �X�P�[�����O�{��]�����̂� )
extern VECTOR VTransformSR( VECTOR InV, MATRIX InM )
{
	VECTOR Result =
	{
		InV.x * InM.m[0][0] + InV.y * InM.m[1][0] + InV.z * InM.m[2][0],
		InV.x * InM.m[0][1] + InV.y * InM.m[1][1] + InV.z * InM.m[2][1],
		InV.x * InM.m[0][2] + InV.y * InM.m[1][2] + InV.z * InM.m[2][2]
	} ;
	return Result ;
}

#endif

// ��̃x�N�g���������p�̃R�T�C���l�𓾂�
extern float VCos( VECTOR In1, VECTOR In2 )
{
	float Result ;

	Result = VDot( VNorm( In1 ), VNorm( In2 ) ) ;
	if( Result < -1.0f )
	{
		return -1.0f ;
	}
	else
	if( Result > 1.0f )
	{
		return 1.0f ;
	}
	return Result ;
}

// ��̃x�N�g���������p�̃R�T�C���l�𓾂�
extern double VCosD( VECTOR_D In1, VECTOR_D In2 )
{
	double Result ;

	Result = VDotD( VNormD( In1 ), VNormD( In2 ) ) ;
	if( Result < -1.0 )
	{
		return -1.0 ;
	}
	else
	if( Result > 1.0 )
	{
		return 1.0 ;
	}
	return Result ;
}

// ��̃x�N�g���������p�̊p�x�𓾂�( �P�ʁF���W�A�� )
extern float VRad( VECTOR In1, VECTOR In2 )
{
	return _ACOS( VCos( In1, In2 ) ) ;
}

// ��̃x�N�g���������p�̊p�x�𓾂�( �P�ʁF���W�A�� )
extern double VRadD( VECTOR_D In1, VECTOR_D In2 )
{
	return _ACOSD( VCosD( In1, In2 ) ) ;
}










#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE










